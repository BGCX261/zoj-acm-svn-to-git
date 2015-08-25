// Crashing Balloon
// http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=1003

#include <iostream>
#include <vector>

using namespace std;

typedef vector<int>				_CMyIntArr;
typedef vector<_CMyIntArr*>		_CMyFactor;

const int MAX_RANGE = 100;

_CMyFactor g_FactorLow, g_FactorBig;

// 清除某个因子式数组
void clear_factor(_CMyFactor &arrFactor)
{
	int i = 0;
	for (i=0; i<(int)arrFactor.size(); i++)
	{
		delete arrFactor[i];
	}
	arrFactor.clear();
}
// 获得一个数在 2~n 内的所有约数，从大到小的排列
void get_divisor(_CMyIntArr &arrDivisor, int nValue, int nRange)
{
	int i = 0;

	if (nValue <= nRange)		// 推入自身
	{
		arrDivisor.push_back(nValue);
	}
	for (i=min((nValue>>1),nRange); i>1; i--)	// 获得所有的约数
	{
		if ((nValue % i) == 0)
			arrDivisor.push_back(i);
	}
}
// 测试
void test_factor(_CMyFactor &arrFactor, _CMyIntArr &arrStack, int nStartPos, int nValue, _CMyIntArr &arrDivisor)
{
	if (nValue == 1)		// 找到一组因子
	{
		_CMyIntArr *pArrOne = new _CMyIntArr(arrStack);
		arrFactor.push_back(pArrOne);
		return;
	}
	if (nStartPos >= (int)arrDivisor.size())
		return;
	for (; nStartPos<(int)arrDivisor.size(); nStartPos++)
	{
		int nDivisor = arrDivisor[nStartPos];

		if ((nValue % nDivisor) == 0)
		{
			arrStack.push_back(nDivisor);
			test_factor(arrFactor, arrStack, nStartPos + 1, (nValue / nDivisor), arrDivisor);
			arrStack.pop_back();
		}
		else
		{
			test_factor(arrFactor, arrStack, nStartPos + 1, nValue, arrDivisor);
		}
	}
}
// 分解一个数在 2~n 内的所有因子组合
bool decompose(_CMyFactor &arrFactor, int nValue, int nRange)
{
	_CMyIntArr arrDivisor;

	get_divisor(arrDivisor, nValue, nRange);
	if (arrDivisor.size() <= 0)
		return false;

	_CMyIntArr arrOne;

	test_factor(arrFactor, arrOne, 0, nValue, arrDivisor);
	if (arrFactor.size() <= 0)
		return false;

	return true;
}
// 小数向大数发起挑战
int challenge(int nLower, int nBigger)
{
	if (nBigger <= MAX_RANGE)		// 两个数都在 100 以内则挑战失败
		return nBigger;

	clear_factor(g_FactorLow);
	clear_factor(g_FactorBig);
	// 分解大小数的所有因子
	if (!decompose(g_FactorLow, nLower, MAX_RANGE))		// 小数不存在则挑战失败
		return nBigger;
	if (!decompose(g_FactorBig, nBigger, MAX_RANGE))	// 小数存在而大数不存在则挑战成功
		return nLower;
	// 进行互斥性测试
	bool bUsed[101] = { false }, bBigWin = false;
	int i = 0, j = 0, m = 0, n = 0;
	_CMyIntArr *pArrLow = NULL, *pArrBig = NULL;

	for (i=0; i<(int)g_FactorLow.size(); i++)
	{
		pArrLow = g_FactorLow[i];
		// 设置占位
		for (j=0; j<(int)pArrLow->size(); j++)
		{
			bUsed[(*pArrLow)[j]] = true;
		}
		for (m=0; m<(int)g_FactorBig.size(); m++)
		{
			pArrBig = g_FactorBig[m];
			bBigWin = true;
			for (n=0; n<(int)pArrBig->size(); n++)
			{
				if (bUsed[(*pArrBig)[n]])	// 被占用
				{
					bBigWin = false;
					break;
				}
			}
			if (bBigWin)	// 挑战失败
				break;
		}
		if (bBigWin)	// 挑战失败
			break;
		// 清空占位
		for (j=0; j<(int)pArrLow->size(); j++)
		{
			bUsed[(*pArrLow)[j]] = false;
		}
	}
	return (bBigWin ? nBigger : nLower);
}

int main(int argc, char* argv[])
{
	int nLeft = 0, nRight = 0;

	while (cin >> nLeft >> nRight)
	{
		cout << challenge(min(nLeft, nRight), max(nLeft, nRight)) << endl;
	}
	clear_factor(g_FactorLow);
	clear_factor(g_FactorBig);

	return 0;
}