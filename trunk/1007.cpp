// Numerical Summation of a Series
// http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=1007

// f(x) = sum(1 / (k * (k+x)))	k = 1, 2, 3, ...
// A table by x = 0.000, 0.001, 0.002, ..., 2.000
// All entries of the table must have an absolute error less than 0.5e-12 (12 digits of precision)

// f(1) = 1
// f(x) - f(1) = (1-x) * sum(1 / (k * (k+1) * (k+x))) = (1-x) * d(x)
// so d(x) = sum(1 / (k * (k+1) * (k+x))) < sum(1 / (k^3))
// when k = n+1, ... and r > 1, there are sum(1 / (k^r)) < I(n), where I(n) = n^(1-r) / (r-1)
// so let I(n) < e-13  =>  n = 2236068

// delta = d(0) - d(1) = sum(1 / (k^2 * (k+1)^2))
// d(1) - d(2) = sum(1 / (k * (k+1)^2 * (k+2))) < d(0) - d(1)
// when k = m+1, ..., we have delta < I(m) = m^-3 / 3
// let I(m) < 0.5e-12  =>  m = 8736

// ��ˣ�ֻ��Ҫ�� d(x) �� m ��ټ��� dm(1) ��ƫ�Ƽ��ɣ���ƫ������ֵֻ����⵽ n �
// ע�⣬˫���ȸ���������ܱ�� 15 λС��������������ʼ�����㡣

#include <stdio.h>

const int n = 2236068;
const int m = 8736;

int main(int argc, char* argv[])
{
	double dDelta = 0.0, dLast = 0.0, dCurr = 0.0, dI = 0.0, dTmp = 0.0;
	double pdX[2001] = { 0.0 }, pdSum[2001] = { 0.0 };
	int i = 0, j = 0;

	// ��ʼ�� x ��ֵ
	for (j=0; j<=2000; j++)
	{
		pdX[j] = (double)j * 0.001;
	}
	// ����� x=1 ʱ�� n �� m+1 ��ĺ� dm(1)�����������Ϊ�˿��Ǿ��ȵı������
	dLast = 1.0 / (double)(n + 1);
	dLast *= dLast;
	for (i=n; i>m; i--)
	{
		dCurr = 1.0 / (double)i;
		dDelta += (dCurr * dLast);
		dLast = dCurr * dCurr;
	}
	// �� 0.000 �� 2.000 ��� d(x)���м����� 1.000
	dLast = 1.0 / (double)(m + 1);
	for (i=m; i>0; i--)
	{
		dI = (double)i;
		dCurr = 1.0 / dI;
		dTmp = dCurr * dLast;
		dLast = dCurr;
		for (j=0; j<1000; j++)
		{
			pdSum[j] += (dTmp / (dI + pdX[j]));
		}
		for (j=1001; j<=2000; j++)
		{
			pdSum[j] += (dTmp / (dI + pdX[j]));
		}
	}
	// ��� f(x) = (d(x) + dm(1)) * (1 - x) + f(1)
	for (j=0; j<=2000; j++)
	{
		dI = pdX[j];
		dTmp = (pdSum[j] + dDelta) * (1.0 - dI) + 1.0;
		printf("%5.3f %16.12f\n", dI, dTmp);
	}
	return 0;
}