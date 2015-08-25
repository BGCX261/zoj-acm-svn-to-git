// Undercut
// http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=1057

// m & n is in [1, 5], m >= n
// if m == n		m get 0, n get 0
// if m == n + 1	m get 0, n get (m+n)
// if m > n + 1		m get m, n get 0
// if m == 2 && n == 1		m get 0, n get 6.

#include <iostream>

using namespace std;

const int MAX_ROUND = 20;
int g_nPoint[6][6];		// ·ÖÊý

int main(int argc, char* argv[])
{
	int pnCards[40], *pnA = pnCards, *pnB = NULL;
	int nRound(0), i(0), nAPoints(0), nBPoints(0), nA(0), nB(0);
	bool bFirst = true;

	memset(g_nPoint, 0, sizeof(g_nPoint));
	g_nPoint[1][2] = 6;
	for (i=2; i<6; i++)
	{
		g_nPoint[i][i + 1] = i + i + 1;
		for (nA=(i-2); nA>0; nA--)
		{
			g_nPoint[i][nA] = i;
		}
	}
	while (cin >> nRound)
	{
		if (nRound > MAX_ROUND)
			continue;
		if (nRound <= 0)
			break;
		for (i=0; i<(nRound<<1); i++)
		{
			cin >> pnCards[i];
		}
		pnB = pnCards + nRound;
		nAPoints = 0;
		nBPoints = 0;
		for (i=0; i<nRound; i++)
		{
			nA = pnA[i];
			nB = pnB[i];
			nAPoints += g_nPoint[nA][nB];
			nBPoints += g_nPoint[nB][nA];
		}
		if (bFirst)
			bFirst = false;
		else
			cout << endl;
		cout << "A has " << nAPoints << " points. B has " << nBPoints << " points." << endl;
	}
	return 0;
}