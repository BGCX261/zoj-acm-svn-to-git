// HangOver
// http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=1045

// sum(n) = 1/2 + 1/3 + 1/4 + ... + 1/(n+1)
// give c to find n, satisfied sum(n) >= c.
// 0.01 <= c <= 5.20

#include <iostream>
#include <vector>
using namespace std;

const double MIN_CARD_LEN = 0.01;
const double MAX_CARD_LEN = 5.20;

int main(int argc, char* argv[])
{
    vector<double> vecCardLen;
    int i = 1;
    double dLen(0.0);

    while (dLen <= MAX_CARD_LEN)
    {
        dLen += (1.0 / (double)(i + 1));
        vecCardLen.push_back(dLen);
        i++;
    }
	while (cin >> dLen && dLen > 0.0)
	{
        if (dLen < MIN_CARD_LEN || dLen > MAX_CARD_LEN)
            continue;
        for (i=0; i<(int)vecCardLen.size(); i++)
        {
            if (dLen <= vecCardLen[i])
                break;
        }
		cout << (i + 1) << " card(s)" << endl;
	}
	return 0;
}