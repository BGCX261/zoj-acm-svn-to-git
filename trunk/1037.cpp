// Gridland
// http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=1037

// m * n ��������ɵķ��󣬶����ı߳�Ϊ 1���Խ��߼������ŷ�϶���
// ��ĳ���˵���룬Ҫ��ÿ���˵�ֻ�ᾭ��һ�Σ������ص���һ������˵㣬�����·��
//
// ���Է��֣�m * n �������γ�һ���ջ�ʱ��������Ҫ m * n ���ߣ���ˣ����·�� >= m * n
// ��1���� m �� n Ϊż��ʱ�������ڷ����л���һ���ջ�·����ÿ��·�����ڱ��ϡ�
// ��2���� m �� n ��Ϊ����ʱ�������ջ�·��������һ���ڶԽ����ϣ�����֤����ֻ��һ���ڶԽ��߼��ɡ�

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