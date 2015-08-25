// Jugs
// http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=1005

// (Ca, Cb, N) puzzle, use Ca and Cb jugs to get N goal, and at last N is in the B jug.
// Ca and Cb are relatively prime to one another, 0 < Ca <= Cb, N <= Cb.
//
// if N == 0 || N == Ca || N == Cb
//    success.
// if Ca + Cb > 2 * N, then A first, else B first.

#include <iostream>

using namespace std;

struct JugProperty
{
    char        chName;
    int         nValue;
    int         nCap;
};

void solve_jug(JugProperty &first, JugProperty &second, int nGoal)
{
    int nOldFirst = 0;

    while (nCount-- > 0)
    {
        if (second.nValue >= second.nCap)
        {
            cout << "empty " << second.chName << endl;
            second.nValue = 0;
        }
        if (first.nValue == 0)
        {
            cout << "fill " << first.chName << endl;
            first.nValue = first.nCap;
        }
        cout << "pour " << first.chName << " " << second.chName << endl;
        nOldFirst = first.nValue;
        first.nValue -= (second.nCap - second.nValue);
        first.nValue = (first.nValue < 0 ? 0 : first.nValue);
        second.nValue += nOldFirst;
    }
}
int main(int argc, char* argv[])
{
    int nGoal;
    JugProperty jugA, jugB;

    jugA.chName = 'A';
    jugB.chName = 'B';
    while (cin >> jugA.nCap >> jugB.nCap >> nGoal
        && jugA.nCap > 0 && jugB.nCap > 0 && nGoal >= 0
        && jugA.nCap <= jugB.nCap && nGoal <= jugB.nCap)
    {
        if (jugB.nCap > 1000) continue;
        if (nGoal == jugB.nCap)
        {
            cout << "fill " << jugB.chName << endl;
        }
        else if (nGoal == jugA.nCap)
        {
            cout << "fill " << jugA.chName << endl;
            cout << "pour " << jugA.chName << " " << jugB.chName << endl;
        }
        else if (nGoal > 0)
        {
            jugA.nValue = jugB.nValue = 0;
            if ((jugA.nCap + jugB.nCap) > (nGoal << 1))
            {
                solve_jug(jugA, jugB, nGoal);
            }
            else
            {
                solve_jug(jugB, jugA, nGoal);
            }
            if (jugB.nValue >= jugB.nCap)
            {
                cout << "empty " << jugB.chName << endl;
                jugB.nValue = 0;
            }
            if (jugB.nValue == 0)
            {
                cout << "pour " << jugA.chName << " " << jugB.chName << endl;
            }
        }
        cout << "success" << endl;
    }
    return 0;
}