// Fire Net
// http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=1002

#include <iostream>
#include <string>
using namespace std;

int FillHousesNO(int ppnMap[][4], int nRow, int nCol, int nMask, int nFill, int nMapSize)
{
    if (ppnMap[nRow][nCol] == -1 || ppnMap[nRow][nCol] != nMask)
        return 0;

    int i = 0;

    // ��䣬�������������ǽ��ͣ�������з�������
    ppnMap[nRow][nCol] = nFill;
    for (i=nCol-1; i>=0; i--)   // ˮƽ����
    {
        if (ppnMap[nRow][i] == -1)
            break;
        if (ppnMap[nRow][i] == nMask)
        {
            ppnMap[nRow][i] = nFill;
        }
    }
    for (i=nCol+1; i<nMapSize; i++)     // ˮƽ����
    {
        if (ppnMap[nRow][i] == -1)
            break;
        if (ppnMap[nRow][i] == nMask)
        {
            ppnMap[nRow][i] = nFill;
        }
    }
    for (i=nRow-1; i>=0; i--)   // ��ֱ����
    {
        if (ppnMap[i][nCol] == -1)
            break;
        if (ppnMap[i][nCol] == nMask)
        {
            ppnMap[i][nCol] = nFill;
        }
    }
    for (i=nRow+1; i<nMapSize; i++)   // ��ֱ����
    {
        if (ppnMap[i][nCol] == -1)
            break;
        if (ppnMap[i][nCol] == nMask)
        {
            ppnMap[i][nCol] = nFill;
        }
    }
    return 1;
}
int GetMaxHouses(int ppnMap[][4], int nHousesCount, int nMapSize)
{
    int i = 0, j = 0;
    int nMaxCount = nHousesCount, nTmpCount = 0;

    for (; i<nMapSize; i++)
    {
        for (j=0; j<nMapSize; j++)
        {
            if (ppnMap[i][j] == -1 || ppnMap[i][j] != 0)
                continue;
            nTmpCount = nHousesCount + 1;
            FillHousesNO(ppnMap, i, j, 0, nTmpCount, nMapSize);
            nMaxCount = max(nMaxCount, GetMaxHouses(ppnMap, nTmpCount, nMapSize));
            FillHousesNO(ppnMap, i, j, nTmpCount, 0, nMapSize);
        }
    }
    return nMaxCount;
}

int main(int argc, char* argv[])
{
    int nMapSize = 0, i = 0, j = 0;
    int ppnMap[4][4] = { 0 };      // -1 ��ʾǽ��0 ��ʾ�գ����� 0 ������ʾ����
    string strLine;

    // ѭ����������
    // �߽�������
    //  ��1��the number 0 signals the end
    //  ��2��the size of the city will be at most 4
    //  ��3��a '.' indicating an open space and an uppercase 'X' indicating a wall
    while (cin >> nMapSize && nMapSize != 0)
    {
        if (nMapSize < 0 || nMapSize > 4)
            continue;

        for (i=0; i<nMapSize; i++)  // ����
        {
            cin >> strLine;
            // ��� MAP
            memset(ppnMap[i], 0, nMapSize * sizeof(int));
            for (j=0; j<min((int)strLine.size(), nMapSize); j++)
            {
                ppnMap[i][j] = (strLine[j] == 'X' ? -1 : 0);
            }
        }
        cout << GetMaxHouses(ppnMap, 0, nMapSize) << endl;
    }
    return 0;
}