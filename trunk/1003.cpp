// Crashing Balloon
// http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=1003

#include <iostream>
#include <vector>

using namespace std;

typedef vector<int>				_CMyIntArr;
typedef vector<_CMyIntArr*>		_CMyFactor;

const int MAX_RANGE = 100;

_CMyFactor g_FactorLow, g_FactorBig;

// ���ĳ������ʽ����
void clear_factor(_CMyFactor &arrFactor)
{
	int i = 0;
	for (i=0; i<(int)arrFactor.size(); i++)
	{
		delete arrFactor[i];
	}
	arrFactor.clear();
}
// ���һ������ 2~n �ڵ�����Լ�����Ӵ�С������
void get_divisor(_CMyIntArr &arrDivisor, int nValue, int nRange)
{
	int i = 0;

	if (nValue <= nRange)		// ��������
	{
		arrDivisor.push_back(nValue);
	}
	for (i=min((nValue>>1),nRange); i>1; i--)	// ������е�Լ��
	{
		if ((nValue % i) == 0)
			arrDivisor.push_back(i);
	}
}
// ����
void test_factor(_CMyFactor &arrFactor, _CMyIntArr &arrStack, int nStartPos, int nValue, _CMyIntArr &arrDivisor)
{
	if (nValue == 1)		// �ҵ�һ������
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
// �ֽ�һ������ 2~n �ڵ������������
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
// С�������������ս
int challenge(int nLower, int nBigger)
{
	if (nBigger <= MAX_RANGE)		// ���������� 100 ��������սʧ��
		return nBigger;

	clear_factor(g_FactorLow);
	clear_factor(g_FactorBig);
	// �ֽ��С������������
	if (!decompose(g_FactorLow, nLower, MAX_RANGE))		// С������������սʧ��
		return nBigger;
	if (!decompose(g_FactorBig, nBigger, MAX_RANGE))	// С�����ڶ���������������ս�ɹ�
		return nLower;
	// ���л����Բ���
	bool bUsed[101] = { false }, bBigWin = false;
	int i = 0, j = 0, m = 0, n = 0;
	_CMyIntArr *pArrLow = NULL, *pArrBig = NULL;

	for (i=0; i<(int)g_FactorLow.size(); i++)
	{
		pArrLow = g_FactorLow[i];
		// ����ռλ
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
				if (bUsed[(*pArrBig)[n]])	// ��ռ��
				{
					bBigWin = false;
					break;
				}
			}
			if (bBigWin)	// ��սʧ��
				break;
		}
		if (bBigWin)	// ��սʧ��
			break;
		// ���ռλ
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