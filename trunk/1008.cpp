// Gnome Tetravex
// http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=1008

// Each square is divided into four triangles marked four numbers (range from 0 to 9).
// In a square, the triangles are the left, the top, the right and the bottom.

// In the termination state, any two adjoining squares should
// make the adjacent triangle marked with the same number.

// The first line of each game case contains one integer n, 0 <= n <= 5,
// indicating the size of the game.
// The following n*n lines describe the marking number of these triangles.
// Each line consists of four integers, which in order represent the top triangle,
// the right triangle, the bottom triangle and the left triangle of one square.
// After the last game case, the integer 0 indicates the termination of the input data set.

// Print "Game %d: Possible/Impossible\n\n", %d starts at 1.

// ԭ��
// ������з����ϡ��¡������ĸ������ι̶������ֻ���ƶ����飬���ܽ�����ת��ת��
// ������ڱ߶�ֻ��������ӳ����� ���� ���¡����ҡ�

// ���� n*n �ķ�����������������һЩԼ��������
// ��1�����ⷽ�����پ߱������ڱߣ�
// ��2��������ĸ�����ֻ�߱������ڱߣ��ҷֱ��Ƿ����������ĸ��� ���� ���ϡ����ϡ����¡����£�
// ��3������ֻ�߱������ڱߵķ��飬���ϱ�Ե�����Ե���±�Ե���ұ�Ե��ÿ����������� n �����飬
// �ҵ��߽Ǳ�ֻ�������ڱߵķ���ռ��ʱ��ÿ�����͵����������Ӧ���٣�
// ��4�������� (n-2)^2 ������߱������ڱߡ�

// �Է������Ԥ����ͳ�Ƴ�����Լ�������µ���Ϣ��
// ��1�����ڱ߷���ļ��ϣ�
// ��2�����ڱ߷���ļ��ϣ����ϱ�Ե�����Ե���±�Ե���ұ�Ե��
// ��3�����ÿ��������ڽ����ͣ�
// ��4����ͳ�ƹ��ķ��鼯�ϣ����ж���ͳ�ƣ�Ϊÿ���������ɸ�Ϊ��ϸ���ڽ����ݡ�
// ��õ������
//     ��ͳ����Ϣ��ʱ���жϳ��޽⡣
// ��������
//     ���еķ��鶼�����ڱߵģ�����ֻ��һ�ֽ⣬������˳�����������������˳�����õִ���
// ������Ҫ����Լ (n*n)! �α���������ʱ�����޷����ܡ�
// �Ż���˼·��
// ��1�����ȷ����������ڲ� (n-2)^2 ������������ڱߵķ�����ɣ�ֻ�����ڲ������н������£�
// �����������н⣬����ȶ��ڲ���������������������
// ��2���ڲ������н�󣬷ֱ�����ϡ��ҡ��¡������������⣬������ʹ����Ӧ�����ڱ߷��鼯�ϣ�
// ��3�����ñ�־λ��¼�����ķ��飬λ����Ϊ(nTop<<12)|(nRight<<8)|(nBottom<<4)|nLeft��
// ����� 8 λ���ݴ洢������������Ϊ(nTop<<9)|(nRight<<5)|(nBottom<<1)|(nLeft>>3)��
// ��λ����Ϊ (nLeft&0x07)������ͬһ��λ�õ����������Ա�����ͬ���ͷ�����ظ����롣


#include <iostream>
using namespace std;

const int NUMBER_COUNT = 10;
const int GAME_MAX_SIZE = 5;
const int SQUARE_MAX_COUNT = 25;
const int BITS_BYTE_COUNT = 4916;

enum INDEX     // һ�������ڵ���������������
{
	INDEX_TOP = 0,
	INDEX_RIGHT,
	INDEX_BOTTOM,
	INDEX_LEFT,

	INDEX_COUNT
};
enum ADJOIN_FLAG
{
    ADJOIN_FLAG_TOP = 0x01,
    ADJOIN_FLAG_RIGHT = 0x02,
    ADJOIN_FLAG_BOTTOM = 0x04,
    ADJOIN_FLAG_LEFT = 0x08,

    ADJOIN_FLAG_RB = (ADJOIN_FLAG_RIGHT | ADJOIN_FLAG_BOTTOM),  // ��������
    ADJOIN_FLAG_LB = (ADJOIN_FLAG_LEFT | ADJOIN_FLAG_BOTTOM),   // ��������
    ADJOIN_FLAG_LT = (ADJOIN_FLAG_LEFT | ADJOIN_FLAG_TOP),      // ��������
    ADJOIN_FLAG_RT = (ADJOIN_FLAG_RIGHT | ADJOIN_FLAG_TOP),     // ��������

    ADJOIN_FLAG_RBL = (ADJOIN_FLAG_RB | ADJOIN_FLAG_LEFT),      // ����������
    ADJOIN_FLAG_TLB = (ADJOIN_FLAG_LB | ADJOIN_FLAG_TOP),       // ����������
    ADJOIN_FLAG_LTR = (ADJOIN_FLAG_LT | ADJOIN_FLAG_RIGHT),     // ����������
    ADJOIN_FLAG_TRB = (ADJOIN_FLAG_RB | ADJOIN_FLAG_TOP),       // ����������

    ADJOIN_FLAG_ALL = 0x0f,     // ���ڱ�
};

struct Square;
struct AdjoinRecord		// �ڱ߷����¼����
{
	Square*	ppSquare[SQUARE_MAX_COUNT];	// ��¼ͬ���ڽӷ����ָ��
	int		nAdjoinCount;				// ����
	int		nUsedCount;					// �ѱ�ʹ�õĸ���
};
struct Square	// ���鶨��
{
    int				pnNum[INDEX_COUNT];     // ����������
    int             nAdjoinFlag;            // �ڽӱ�־
    AdjoinRecord    myAdjoin;               // ��֮���ڽӵķ����¼
    AdjoinRecord    pAdjoin4[INDEX_COUNT];  // ÿ���������ڱ߷�����ڽӼ�¼
    AdjoinRecord    pAdjoin3[INDEX_COUNT];  // ÿ���������ڱ߷�����ڽӼ�¼��ֻ��Ӧ��Ӧ�ı�Ե��
	int				nUsed;		            // <=0 δ��ʹ�ã������ѱ�ʹ��
};

// ȫ�ֱ�������

AdjoinRecord	g_ppAdjoin4Table[NUMBER_COUNT][NUMBER_COUNT];	// ���ڱ߷�������Ͻ�ƥ���¼
AdjoinRecord	g_pAdjoin3[INDEX_COUNT];	// �����ı�Ե�ķ����¼�����ҽ��������ڱ�
AdjoinRecord	g_Adjoin4;					// �߱������ڱߵķ����¼

int         g_nGameSize = 0;
Square	    g_pSquares[SQUARE_MAX_COUNT];		// ����
Square*	    g_ppGnome[GAME_MAX_SIZE][GAME_MAX_SIZE];	// ��������

// �ڽӱ߱�־��������ӳ��
const int ADJOIN_COUNT_BY_FLAG[16] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4
};

// ����ӿ�

// �Ƚ�
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
inline int InRange(int x, int l, int r)
{
    return min(max(x, l), r);
}
// λ����
inline bool SetBit(unsigned char *pucBits, int* pnNum)
{
    int nByteIndex = ((pnNum[INDEX_TOP]<<9)|(pnNum[INDEX_RIGHT]<<5)|(pnNum[INDEX_BOTTOM]<<1)|(pnNum[INDEX_LEFT]>>3));
    int nBitValue = (1 << (pnNum[INDEX_LEFT]&0x07));

    if (pucBits[nByteIndex] & nBitValue)
        return true;
    
    pucBits[nByteIndex] |= nBitValue; 

    return false;
}
// ��¼����
inline void AddAdjoinRecord(AdjoinRecord &stuRecord, Square *pSq)
{
    stuRecord.ppSquare[stuRecord.nAdjoinCount++] = pSq;
}
inline bool IsAdjoinRecordEmpty(AdjoinRecord &stuRecord)
{
    return (stuRecord.nUsedCount >= stuRecord.nAdjoinCount);
}
// �������
inline void SetUsed(Square *pSq)
{
	pSq->nUsed = 1;
}
inline void ClearUsed(Square *pSq)
{
	pSq->nUsed = 0;
}
inline bool IsUsed(Square *pSq)
{
	return (pSq->nUsed > 0);
}
// ��������
inline Square* GetGnomePrevRow(int nRow, int nCol)
{
	if (nRow <= 0)
		return NULL;
	return g_ppGnome[nRow - 1][nCol];
}
inline Square* GetGnomeNextRow(int nRow, int nCol)
{
	if (nRow >= g_nGameSize - 1)
		return NULL;
	return g_ppGnome[nRow + 1][nCol];
}
inline Square* GetGnomePrevCol(int nRow, int nCol)
{
	if (nCol <= 0)
		return NULL;
	return g_ppGnome[nRow][nCol - 1];
}
inline Square* GetGnomeNextCol(int nRow, int nCol)
{
	if (nCol >= g_nGameSize - 1)
		return NULL;
	return g_ppGnome[nRow][nCol + 1];
}
inline void SetGnomeSimple(int nRow, int nCol, Square *pSq)
{
    g_ppGnome[nRow][nCol] = pSq;
    SetUsed(pSq);
}
inline bool SetGnome(int nRow, int nCol, Square *pSq)
{
	Square *pOtherSq = NULL;

	if (g_ppGnome[nRow][nCol] != NULL)
		return false;
	pOtherSq = GetGnomePrevCol(nRow, nCol);
	if (pOtherSq != NULL && pOtherSq->pnNum[INDEX_RIGHT] != pSq->pnNum[INDEX_LEFT])
		return false;
	pOtherSq = GetGnomeNextCol(nRow, nCol);
	if (pOtherSq != NULL && pOtherSq->pnNum[INDEX_LEFT] != pSq->pnNum[INDEX_RIGHT])
		return false;
	pOtherSq = GetGnomePrevRow(nRow, nCol);
	if (pOtherSq != NULL && pOtherSq->pnNum[INDEX_BOTTOM] != pSq->pnNum[INDEX_TOP])
		return false;
	pOtherSq = GetGnomeNextRow(nRow, nCol);
	if (pOtherSq != NULL && pOtherSq->pnNum[INDEX_TOP] != pSq->pnNum[INDEX_BOTTOM])
		return false;
    SetGnomeSimple(nRow, nCol, pSq);

	return true;
}
inline void ClearGnome(int nRow, int nCol)
{
	ClearUsed(g_ppGnome[nRow][nCol]);
	g_ppGnome[nRow][nCol] = NULL;
}
// ���봦��
void InputSqaures(int nSquareCount)
{
	int i = 0, j = 0, nTmp = 0;

    memset(g_pSquares, 0, sizeof(g_pSquares));
	for (i=0; i<nSquareCount; i++)
	{
		for (j=INDEX_TOP; j<INDEX_COUNT; j++)
		{
			cin >> nTmp;
			g_pSquares[i].pnNum[j] = InRange(nTmp, 0, 9);
		}
	}
}
// Ԥ����
bool PreProc(int nSquareCount)
{
    if (g_nGameSize < 2)
        return true;

	int i = 0, j = 0, nTmp = 0;

	// �����֮ǰ������
	memset(g_ppGnome[0], 0, sizeof(g_ppGnome));
	memset(g_ppAdjoin4Table[0], 0, sizeof(g_ppAdjoin4Table));
	memset(g_pAdjoin3, 0, sizeof(g_pAdjoin3));
	memset(&g_Adjoin4, 0, sizeof(g_Adjoin4));
	// ��������ڽ���Ϣ
	for (i=0; i<nSquareCount; i++)
	{
        Square *pCurrSq = (g_pSquares + i), *pOtherSq = NULL;
		int nTop = pCurrSq->pnNum[INDEX_TOP];
		int nRight = pCurrSq->pnNum[INDEX_RIGHT];
		int nBottom = pCurrSq->pnNum[INDEX_BOTTOM];
		int nLeft = pCurrSq->pnNum[INDEX_LEFT];
		int nRow = 0, nCol = 0;

		// �����ڱ�
		for (j=0; j<nSquareCount; j++)
		{
			if (i == j)		// �����������ƥ��
				continue;
            pOtherSq = (g_pSquares + j);
			if (nTop == pOtherSq->pnNum[INDEX_BOTTOM])
            {
                pCurrSq->nAdjoinFlag |= ADJOIN_FLAG_TOP;
            }
            if (nRight == pOtherSq->pnNum[INDEX_LEFT])
            {
                pCurrSq->nAdjoinFlag |= ADJOIN_FLAG_RIGHT;
            }
            if (nBottom == pOtherSq->pnNum[INDEX_TOP])
            {
                pCurrSq->nAdjoinFlag |= ADJOIN_FLAG_BOTTOM;
            }
            if (nLeft == pOtherSq->pnNum[INDEX_RIGHT])
            {
                pCurrSq->nAdjoinFlag |= ADJOIN_FLAG_LEFT;
            }
            if (pCurrSq->nAdjoinFlag != 0)
            {
                AddAdjoinRecord(pCurrSq->myAdjoin, pOtherSq);
            }
        }
        // �ж��ڽ����
        switch (ADJOIN_COUNT_BY_FLAG[pCurrSq->nAdjoinFlag])
        {
        case 0:     // �������پ߱������ڱ�
        case 1:
            return false;
        case 2:     // ֻ�߱������ڱߵķ���ֻ�ܷ����������ĸ����䣬��ÿ������ֻ��һ��
            switch (pCurrSq->nAdjoinFlag)
            {
            case ADJOIN_FLAG_RB:        // �����ڽӣ��������Ͻ�
                nRow = nCol = 0;
                break;
            case ADJOIN_FLAG_LB:        // �����ڽӣ��������Ͻ�
                nRow = 0;
                nCol = g_nGameSize - 1;
                break;
            case ADJOIN_FLAG_LT:        // �����ڽӣ��������½�
                nRow = nCol = g_nGameSize - 1;
                break;
            case ADJOIN_FLAG_RT:        // �����ڽӣ��������½�
                nCol = 0;
                nRow = g_nGameSize - 1;
                break;
            default:
                return false;
            }
			if (!SetGnome(nRow, nCol, pCurrSq))
				return false;
            break;
        case 3:     // �߱������ڱߵķ���
            switch (pCurrSq->nAdjoinFlag)
            {
            case ADJOIN_FLAG_RBL:   // �������ڽӣ������ϱ�Ե
                nTmp = INDEX_TOP;
                break;
            case ADJOIN_FLAG_TLB:   // �������ڽӣ������ұ�Ե
                nTmp = INDEX_RIGHT;
                break;
            case ADJOIN_FLAG_LTR:   // �������ڽӣ������±�Ե
                nTmp = INDEX_BOTTOM;
                break;
            case ADJOIN_FLAG_TRB:   // �������ڽӣ��������Ե
                nTmp = INDEX_LEFT;
                break;
            }
            AddAdjoinRecord(g_pAdjoin3[nTmp], pCurrSq);
            break;
        case 4:
            AddAdjoinRecord(g_Adjoin4, pCurrSq);
            // ��ʼ�����Ͻ�ƥ���¼
            AddAdjoinRecord(g_ppAdjoin4Table[nLeft][nTop], pCurrSq);
            break;
        }
    }
    // ��һ��Ե����� n �����飬�ҵ��߽Ǳ�ռ��ʱ�����������Ӧ����
    for (i=0; i<INDEX_COUNT; i++)
    {
        switch (i)
        {
        case INDEX_TOP:
            nTmp = g_nGameSize - (g_ppGnome[0][0] != NULL ? 1 : 0);
            nTmp -= (g_ppGnome[0][g_nGameSize - 1] != NULL ? 1 : 0);
            break;
        case INDEX_RIGHT:
            nTmp = g_nGameSize - (g_ppGnome[g_nGameSize - 1][g_nGameSize - 1] != NULL ? 1 : 0);
            nTmp -= (g_ppGnome[0][g_nGameSize - 1] != NULL ? 1 : 0);
            break;
        case INDEX_BOTTOM:
            nTmp = g_nGameSize - (g_ppGnome[g_nGameSize - 1][g_nGameSize - 1] != NULL ? 1 : 0);
            nTmp -= (g_ppGnome[g_nGameSize - 1][0] != NULL ? 1 : 0);
            break;
        case INDEX_LEFT:
            nTmp = g_nGameSize - (g_ppGnome[0][0] != NULL ? 1 : 0);
            nTmp -= (g_ppGnome[g_nGameSize - 1][0] != NULL ? 1 : 0);
            break;
        }
        if (g_pAdjoin3[i].nAdjoinCount > nTmp)
            return false;
    }
    // ���ٴ��� (n-2)^2 ���߱������ڱߵķ���
    if (g_Adjoin4.nAdjoinCount < ((g_nGameSize - 2) * (g_nGameSize - 2)))
        return false;
    // ����ͳ�Ƶ��ڽ���Ϣ�ٽ���ϸ��
    for (i=0; i<nSquareCount; i++)
    {
        Square *pCurrSq = (g_pSquares + i), *pOtherSq = NULL;

        for (j=0; j<pCurrSq->myAdjoin.nAdjoinCount; j++)
        {
            pOtherSq = pCurrSq->myAdjoin.ppSquare[j];
            switch (pOtherSq->nAdjoinFlag)  // �����ڽӷ�������������ϸ��Ϣ
            {
            case ADJOIN_FLAG_RBL:       // �ϱ�Ե
                AddAdjoinRecord(pCurrSq->pAdjoin3[INDEX_TOP], pOtherSq);
                break;
            case ADJOIN_FLAG_TLB:       // �ұ�Ե
                AddAdjoinRecord(pCurrSq->pAdjoin3[INDEX_RIGHT], pOtherSq);
                break;
            case ADJOIN_FLAG_TRB:       // ���Ե
                AddAdjoinRecord(pCurrSq->pAdjoin3[INDEX_LEFT], pOtherSq);
                break;
            case ADJOIN_FLAG_LTR:       // �±�Ե
                AddAdjoinRecord(pCurrSq->pAdjoin3[INDEX_BOTTOM], pOtherSq);
                break;
            case ADJOIN_FLAG_ALL:       // ���ڱߣ��ֱ�����������
                if (pCurrSq->pnNum[INDEX_TOP] == pOtherSq->pnNum[INDEX_BOTTOM])
                {
                    AddAdjoinRecord(pCurrSq->pAdjoin4[INDEX_TOP], pOtherSq);
                }
                if (pCurrSq->pnNum[INDEX_RIGHT] == pOtherSq->pnNum[INDEX_LEFT])
                {
                    AddAdjoinRecord(pCurrSq->pAdjoin4[INDEX_RIGHT], pOtherSq);
                }
                if (pCurrSq->pnNum[INDEX_BOTTOM] == pOtherSq->pnNum[INDEX_TOP])
                {
                    AddAdjoinRecord(pCurrSq->pAdjoin4[INDEX_BOTTOM], pOtherSq);
                }
                if (pCurrSq->pnNum[INDEX_LEFT] == pOtherSq->pnNum[INDEX_RIGHT])
                {
                    AddAdjoinRecord(pCurrSq->pAdjoin4[INDEX_LEFT], pOtherSq);
                }
                break;
            }
        }
    }
	return true;
}
// ��ȡ��һ������
inline void GetNextIndex(int nRow, int nCol, int &nNextRow, int &nNextCol)
{
	if (nCol >= g_nGameSize - 1)    // ����
	{
		nNextRow = nRow + 1;
		nNextCol = 0;
	}
	else
	{
		nNextRow = nRow;
		nNextCol = nCol + 1;
	}
}
// �����������
bool TraceProc2(int nRow, int nCol, int nSquareCount)       // GameSize == 2
{
	if (nRow >= g_nGameSize)
		return true;

    Square *pSq = g_ppGnome[nRow][nCol];
	int nNextRow = 0, nNextCol = 0, i = 0;

	GetNextIndex(nRow, nCol, nNextRow, nNextCol);
    if (pSq != NULL)        // �Ѿ�����
    {
		return TraceProc2(nNextRow, nNextCol, nSquareCount);
    }
    else	// �����������
    {
		for (i=0; i<nSquareCount; i++)
		{
			pSq = (g_pSquares + i);
			if (IsUsed(pSq))
				continue;
			if (!SetGnome(nRow, nCol, pSq))
				continue;
			if (TraceProc2(nNextRow, nNextCol, nSquareCount))
				return true;
			ClearGnome(nRow, nCol);
		}
    }
    return false;
}
// �ڽ�������ȡ��һ������
inline void GetNextIndexInner(int nRow, int nCol, int &nNextRow, int &nNextCol)
{
    if (nCol >= g_nGameSize - 2)    // ����
    {
        nNextRow = nRow + 1;
        nNextCol = 1;
    }
    else
    {
        nNextRow = nRow;
        nNextCol = nCol + 1;
    }
}
inline void GetNextIndexOuter(int nRow, int nCol, int &nNextRow, int &nNextCol)
{
    // ˳ʱ��ת
    if (nRow == 0)    // �ϱ�Ե�����Ͻ�  
    {
        if (nCol < g_nGameSize - 1)
        {
            nNextRow = nRow;
            nNextCol = nCol + 1;
        }
        else
        {
            nNextRow = nRow + 1;
            nNextCol = nCol;
        }
    }
    else if (nCol == g_nGameSize - 1)   // �ұ�Ե�����½�
    {
        if (nRow < g_nGameSize - 1)
        {
            nNextRow = nRow + 1;
            nNextCol = nCol;
        }
        else
        {
            nNextRow = nRow;
            nNextCol = nCol - 1;
        }
    }
    else if (nRow == g_nGameSize - 1)   // �±�Ե�����½�
    {
        if (nCol > 0)
        {
            nNextRow = nRow;
            nNextCol = nCol - 1;
        }
        else
        {
            nNextRow = nRow - 1;
            nNextCol = nCol;
        }
    }
    else if (nCol == 0)     // ���Ե�����Ͻ�
    {
        nNextRow = nRow - 1;
        nNextCol = nCol;
    }
}
bool TraceOuterM(int nRow, int nCol, Square *pPrevSq)
{
    if (nRow == 0 && nCol == 0 && pPrevSq != NULL)
        return true;

    int nNextRow = 0, nNextCol = 0;
    Square *pCurrSq = g_ppGnome[nRow][nCol];
    
    GetNextIndexOuter(nRow, nCol, nNextRow, nNextCol);
    if (pCurrSq != NULL)        // �Ѿ����ڣ�ֻ�������ĸ���
    {
        return TraceOuterM(nNextRow, nNextCol, pCurrSq);
    }
    else    // ���
    {
        if (nRow == 0)      // ���Ͻǡ��ϱ�Ե�����Ͻ�
        {

        }
        else if (nCol == g_nGameSize - 1)   // �ұ�Ե�����½�
        {

        }
        else if (nRow == g_nGameSize - 1)   // �±�Ե�����½�
        {

        }
        else if (nCol == 0)     // ���Ե
        {

        }
    }
    return false;
}
bool TraceInnerM(int nRow, int nCol, Square *pSqL, Square *pSqT)
{
    int i = 0;
    int nNextRow = 0, nNextCol = 0;
    AdjoinRecord *pCurrAdjoin = NULL;
    Square *pCurrSq = NULL;

    if (nRow >= g_nGameSize - 1)    // �ڽ����������ɣ��ֱ�����ϡ��ҡ��¡����Ե
    {
        return TraceOuterM(0, 0, NULL);   // �����Ͻǿ�ʼ
    }
    else    // ����ڽ�������ֻ��������ڽ�
    {
        // pSqL �� pSqT �бض���һ����Ϊ NULL
        if (pSqL != NULL && pSqT != NULL)     // �����Ͻ��ڽ�
        {
            pCurrAdjoin = (g_ppAdjoin4Table[pSqL->pnNum[INDEX_RIGHT]] + pSqT->pnNum[INDEX_BOTTOM]);
        }
        else if (pSqL != NULL)   // ֻ�����ڽ�
        {
            pCurrAdjoin = (pSqL->pAdjoin4 + INDEX_RIGHT);
        }
        else if (pSqT != NULL)    // ֻ�����ڽ�
        {
            pCurrAdjoin = (pSqT->pAdjoin4 + INDEX_BOTTOM);
        }
        else
            return false;
        GetNextIndexInner(nRow, nCol, nNextRow, nNextCol);
        if (IsAdjoinRecordEmpty(*pCurrAdjoin))
            return false;
        for (i=0; i<pCurrAdjoin->nAdjoinCount; i++)
        {
            pCurrSq = pCurrAdjoin->ppSquare[i];
            if (IsUsed(pCurrSq))
                continue;
            SetGnomeSimple(nRow, nCol, pCurrSq);
            if (TraceInnerM(nNextRow, nNextCol,
                (nRow == nNextRow ? pCurrSq : NULL),
                (nRow == nNextRow ? NULL : g_ppGnome[nNextRow - 1][nNextCol])))
                return true;
            ClearGnome(nRow, nCol);
        }
    }
    return false;
}
bool TraceProcM()        // GameSize > 2
{
    int i = 0;
    int nNextRow = 0, nNextCol = 0;
    Square *pCurrSq = NULL;

    // ���ڽ����������Ͻǿ�ʼ
    GetNextIndexInner(1, 1, nNextRow, nNextCol);
    for (i=0; i<g_Adjoin4.nAdjoinCount; i++)
    {
        pCurrSq = g_Adjoin4.ppSquare[i];
        SetGnomeSimple(1, 1, pCurrSq);
        if (TraceInnerM(nNextRow, nNextCol, pCurrSq, NULL))
            return true;
        ClearGnome(1, 1);
    }
	return false;
}

int main(int argc, char* argv[])
{
    int i = 0, j = 0, nSquareCount = 0;
    int nGameNO = 0;
    bool bPossible = false;

    while (cin >> g_nGameSize && g_nGameSize > 0)
    {
        if (g_nGameSize > GAME_MAX_SIZE)
            continue;
        nGameNO++;
		nSquareCount = g_nGameSize * g_nGameSize;
        // ���뷽���е�����
		InputSqaures(nSquareCount);
		// Ԥ����
		if (bPossible = PreProc(nSquareCount))
		{
			// ���ݴ����������ҡ��������»��ݣ������е����һ��ʱ����һ��Ϊ��һ�е���ʼ
            if (g_nGameSize > 2)
            {
                bPossible = TraceProcM();
            }
            else if (g_nGameSize == 2)
            {
                bPossible = TraceProc2(0, 0, nSquareCount);
            }
		}
        cout << "Game " << nGameNO << ": " << (bPossible ? "Possible" : "Impossible") << endl << endl;
    }
    return 0;
}