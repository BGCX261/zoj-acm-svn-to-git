// I Think I Need a Houseboat
// http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=1049

//						y
//					|
//					|###
//					|	###
//		land		|	   ##
//					|		 #
//	----------------|---------#------ x
//					|
//		water		|
//					|
//					|
//					|
// shrinking 50 square miles each year in a semicircle form with the center (0, 0)
// the radius increases, but grads goes down, because at last a few increasing means a large area
//
// S = 1/2 * PI * r^2
// Eroding: (x^2 + y^2) < r^2 <= 100 * n / PI

#include <iostream>
#include <vector>
using namespace std;

struct MyProperty
{
	double	x, y;
};

double g_pdAreaByYear[200];
const double AREA_COEF = 100.0 / 3.1415926535897932384626433832795;

int big_year(double dR)
{
    return 0;
}
int get_year(MyProperty &proper)
{
    double dR = proper.x * proper.x + proper.y * proper.y;
    int i = 0;

    for (i=0; i<200; i++)
    {
        if (dR < g_pdAreaByYear[i])
            return (i + 1);
    }
    return i;//big_year(dR);
}
int main(int argc, char* argv[])
{
	int nCount(0), i(0);
    vector<MyProperty> vecProp;

    for (i=0; i<200; i++)
    {
        g_pdAreaByYear[i] = (i + 1) * AREA_COEF;
    }
	if (cin >> nCount && nCount >= 0)
	{
        vecProp.resize(nCount);
		for (i=0; i<nCount; i++)
		{
            cin >> vecProp[i].x >> vecProp[i].y;
        }
        for (i=0; i<nCount; i++)
        {
            cout << "Property " << (i + 1) << ": This property will begin eroding in year ";
            cout << get_year(vecProp[i]) << "." << endl;
        }
		cout << "END OF OUTPUT." << endl;
	}
	return 0;
}