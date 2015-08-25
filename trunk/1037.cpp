// Gridland
// http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=1037

// m * n 个顶点组成的方阵，顶点间的边长为 1，对角线计算符合欧氏定理
// 从某个端点进入，要求每个端点只会经过一次，而最后回到第一个进入端点，求最短路径
//
// 可以发现，m * n 个顶点形成一个闭环时，最少需要 m * n 条边，因此，最短路径 >= m * n
// （1）当 m 或 n 为偶数时，可以在方阵中画出一条闭环路径，每条路径都在边上。
// （2）当 m 与 n 都为奇数时，这条闭环路径至少有一条在对角线上，可以证明，只需一条在对角线即可。

#include <iostream>
#include <vector>
using namespace std;

struct Scenario
{
    int m, n;
};

const double DIAG_LEN = 0.4142135623730950488016887242097;

double get_path(Scenario &scen)
{
    int nPath = scen.m * scen.n;

    if ((scen.m & 0x01) == 0 || (scen.n & 0x01) == 0)
        return (double)nPath;

    return ((double)nPath + DIAG_LEN);
}
int main(int argc, char* argv[])
{
	int nCount(0), i(0);
    vector<Scenario> vecScen;

	if (cin >> nCount && nCount >= 0)
	{
        vecScen.resize(nCount);
		for (i=0; i<nCount; i++)
		{
            cin >> vecScen[i].m >> vecScen[i].n;
        }
        cout.setf(ios_base::fixed);
        cout.precision(2);
        for (i=0; i<nCount; i++)
        {
            cout << "Scenario #" << (i + 1) << ":" << endl;
            cout << get_path(vecScen[i]) << endl << endl;
        }
	}
    cin >> i;
	return 0;
}