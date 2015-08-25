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

// 原则：
// 方块具有方向，上、下、左、右四个三角形固定，因此只能移动方块，不能进行旋转或翻转。
// 方块的邻边对只能有两种映射情况 —— 上下、左右。

// 对于 n*n 的方块容器，具有如下一些约束条件：
// （1）任意方块最少具备两条邻边；
// （2）最多有四个方块只具备两条邻边，且分别是方块容器的四个角 —— 左上、右上、右下、左下；
// （3）对于只具备三条邻边的方块，如上边缘、左边缘、下边缘、右边缘，每种类型最多有 n 个方块，
// 且当边角被只有两条邻边的方块占用时，每种类型的最大数量相应减少；
// （4）最少有 (n-2)^2 个方块具备四条邻边。

// 对方块进行预处理，统计出上述约束条件下的信息：
// （1）四邻边方块的集合；
// （2）三邻边方块的集合，分上边缘、左边缘、下边缘、右边缘；
// （3）填充每个方块的邻接类型；
// （4）对统计过的方块集合，进行二次统计，为每个方块生成更为详细的邻接数据。
// 最好的情况：
//     在统计信息的时候，判断出无解。
// 最坏的情况：
//     所有的方块都是四邻边的，有且只有一种解，且数据顺序与深度优先搜索的顺序正好抵触，
// 导致需要进行约 (n*n)! 次遍历，这在时间上无法忍受。
// 优化的思路：
// （1）首先方块容器的内部 (n-2)^2 区域必须由四邻边的方块组成，只有在内部区域有解的情况下，
// 整个容器才有解，因此先对内部区域进行深度优先搜索；
// （2）内部区域有解后，分别进行上、右、下、左的外轮廓求解，并优先使用相应的三邻边方块集合；
// （3）采用标志位记录经过的方块，位索引为(nTop<<12)|(nRight<<8)|(nBottom<<4)|nLeft，
// 如果以 8 位数据存储，则子数索引为(nTop<<9)|(nRight<<5)|(nBottom<<1)|(nLeft>>3)，
// 子位索引为 (nLeft&0x07)，对于同一个位置的搜索，可以避免相同类型方块的重复进入。


#include <iostream>
using namespace std;

const int NUMBER_COUNT = 10;
const int GAME_MAX_SIZE = 5;
const int SQUARE_MAX_COUNT = 25;
const int BITS_BYTE_COUNT = 4916;

enum INDEX     // 一个方块内的三角形索引定义
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

    ADJOIN_FLAG_RB = (ADJOIN_FLAG_RIGHT | ADJOIN_FLAG_BOTTOM),  // 右下相邻
    ADJOIN_FLAG_LB = (ADJOIN_FLAG_LEFT | ADJOIN_FLAG_BOTTOM),   // 左下相邻
    ADJOIN_FLAG_LT = (ADJOIN_FLAG_LEFT | ADJOIN_FLAG_TOP),      // 左上相邻
    ADJOIN_FLAG_RT = (ADJOIN_FLAG_RIGHT | ADJOIN_FLAG_TOP),     // 右上相邻

    ADJOIN_FLAG_RBL = (ADJOIN_FLAG_RB | ADJOIN_FLAG_LEFT),      // 右下左相邻
    ADJOIN_FLAG_TLB = (ADJOIN_FLAG_LB | ADJOIN_FLAG_TOP),       // 下左上相邻
    ADJOIN_FLAG_LTR = (ADJOIN_FLAG_LT | ADJOIN_FLAG_RIGHT),     // 右上左相邻
    ADJOIN_FLAG_TRB = (ADJOIN_FLAG_RB | ADJOIN_FLAG_TOP),       // 上右下相邻

    ADJOIN_FLAG_ALL = 0x0f,     // 四邻边
};

struct Square;
struct AdjoinRecord		// 邻边方块记录定义
{
	Square*	ppSquare[SQUARE_MAX_COUNT];	// 记录同类邻接方块的指针
	int		nAdjoinCount;				// 个数
	int		nUsedCount;					// 已被使用的个数
};
struct Square	// 方块定义
{
    int				pnNum[INDEX_COUNT];     // 三角形数据
    int             nAdjoinFlag;            // 邻接标志
    AdjoinRecord    myAdjoin;               // 与之有邻接的方块记录
    AdjoinRecord    pAdjoin4[INDEX_COUNT];  // 每条边与四邻边方块的邻接记录
    AdjoinRecord    pAdjoin3[INDEX_COUNT];  // 每条边与三邻边方块的邻接记录（只对应相应的边缘）
	int				nUsed;		            // <=0 未被使用，否则已被使用
};

// 全局变量声明

AdjoinRecord	g_ppAdjoin4Table[NUMBER_COUNT][NUMBER_COUNT];	// 四邻边方块的左上角匹配记录
AdjoinRecord	g_pAdjoin3[INDEX_COUNT];	// 容器四边缘的方块记录，有且仅有三条邻边
AdjoinRecord	g_Adjoin4;					// 具备四条邻边的方块记录

int         g_nGameSize = 0;
Square	    g_pSquares[SQUARE_MAX_COUNT];		// 输入
Square*	    g_ppGnome[GAME_MAX_SIZE][GAME_MAX_SIZE];	// 方块容器

// 邻接边标志与数量的映射
const int ADJOIN_COUNT_BY_FLAG[16] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4
};

// 处理接口

// 比较
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
// 位处理
inline bool SetBit(unsigned char *pucBits, int* pnNum)
{
    int nByteIndex = ((pnNum[INDEX_TOP]<<9)|(pnNum[INDEX_RIGHT]<<5)|(pnNum[INDEX_BOTTOM]<<1)|(pnNum[INDEX_LEFT]>>3));
    int nBitValue = (1 << (pnNum[INDEX_LEFT]&0x07));

    if (pucBits[nByteIndex] & nBitValue)
        return true;
    
    pucBits[nByteIndex] |= nBitValue; 

    return false;
}
// 记录处理
inline void AddAdjoinRecord(AdjoinRecord &stuRecord, Square *pSq)
{
    stuRecord.ppSquare[stuRecord.nAdjoinCount++] = pSq;
}
inline bool IsAdjoinRecordEmpty(AdjoinRecord &stuRecord)
{
    return (stuRecord.nUsedCount >= stuRecord.nAdjoinCount);
}
// 方块操作
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
// 容器操作
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
// 输入处理
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
// 预处理
bool PreProc(int nSquareCount)
{
    if (g_nGameSize < 2)
        return true;

	int i = 0, j = 0, nTmp = 0;

	// 先清空之前的数据
	memset(g_ppGnome[0], 0, sizeof(g_ppGnome));
	memset(g_ppAdjoin4Table[0], 0, sizeof(g_ppAdjoin4Table));
	memset(g_pAdjoin3, 0, sizeof(g_pAdjoin3));
	memset(&g_Adjoin4, 0, sizeof(g_Adjoin4));
	// 逐个搜索邻接信息
	for (i=0; i<nSquareCount; i++)
	{
        Square *pCurrSq = (g_pSquares + i), *pOtherSq = NULL;
		int nTop = pCurrSq->pnNum[INDEX_TOP];
		int nRight = pCurrSq->pnNum[INDEX_RIGHT];
		int nBottom = pCurrSq->pnNum[INDEX_BOTTOM];
		int nLeft = pCurrSq->pnNum[INDEX_LEFT];
		int nRow = 0, nCol = 0;

		// 搜索邻边
		for (j=0; j<nSquareCount; j++)
		{
			if (i == j)		// 不与自身进行匹配
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
        // 判断邻接情况
        switch (ADJOIN_COUNT_BY_FLAG[pCurrSq->nAdjoinFlag])
        {
        case 0:     // 方块至少具备两条邻边
        case 1:
            return false;
        case 2:     // 只具备两条邻边的方块只能放在容器的四个角落，且每种有且只有一个
            switch (pCurrSq->nAdjoinFlag)
            {
            case ADJOIN_FLAG_RB:        // 右下邻接，则在左上角
                nRow = nCol = 0;
                break;
            case ADJOIN_FLAG_LB:        // 左下邻接，则在右上角
                nRow = 0;
                nCol = g_nGameSize - 1;
                break;
            case ADJOIN_FLAG_LT:        // 左上邻接，则在右下角
                nRow = nCol = g_nGameSize - 1;
                break;
            case ADJOIN_FLAG_RT:        // 右上邻接，则在左下角
                nCol = 0;
                nRow = g_nGameSize - 1;
                break;
            default:
                return false;
            }
			if (!SetGnome(nRow, nCol, pCurrSq))
				return false;
            break;
        case 3:     // 具备三条邻边的方块
            switch (pCurrSq->nAdjoinFlag)
            {
            case ADJOIN_FLAG_RBL:   // 右下左邻接，则在上边缘
                nTmp = INDEX_TOP;
                break;
            case ADJOIN_FLAG_TLB:   // 下左上邻接，则在右边缘
                nTmp = INDEX_RIGHT;
                break;
            case ADJOIN_FLAG_LTR:   // 右上左邻接，则在下边缘
                nTmp = INDEX_BOTTOM;
                break;
            case ADJOIN_FLAG_TRB:   // 上右下邻接，则在左边缘
                nTmp = INDEX_LEFT;
                break;
            }
            AddAdjoinRecord(g_pAdjoin3[nTmp], pCurrSq);
            break;
        case 4:
            AddAdjoinRecord(g_Adjoin4, pCurrSq);
            // 初始化左上角匹配记录
            AddAdjoinRecord(g_ppAdjoin4Table[nLeft][nTop], pCurrSq);
            break;
        }
    }
    // 任一边缘最多有 n 个方块，且当边角被占据时，最大数量相应减少
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
    // 最少存在 (n-2)^2 个具备四条邻边的方块
    if (g_Adjoin4.nAdjoinCount < ((g_nGameSize - 2) * (g_nGameSize - 2)))
        return false;
    // 对已统计的邻接信息再进行细化
    for (i=0; i<nSquareCount; i++)
    {
        Square *pCurrSq = (g_pSquares + i), *pOtherSq = NULL;

        for (j=0; j<pCurrSq->myAdjoin.nAdjoinCount; j++)
        {
            pOtherSq = pCurrSq->myAdjoin.ppSquare[j];
            switch (pOtherSq->nAdjoinFlag)  // 根据邻接方块的类型填充详细信息
            {
            case ADJOIN_FLAG_RBL:       // 上边缘
                AddAdjoinRecord(pCurrSq->pAdjoin3[INDEX_TOP], pOtherSq);
                break;
            case ADJOIN_FLAG_TLB:       // 右边缘
                AddAdjoinRecord(pCurrSq->pAdjoin3[INDEX_RIGHT], pOtherSq);
                break;
            case ADJOIN_FLAG_TRB:       // 左边缘
                AddAdjoinRecord(pCurrSq->pAdjoin3[INDEX_LEFT], pOtherSq);
                break;
            case ADJOIN_FLAG_LTR:       // 下边缘
                AddAdjoinRecord(pCurrSq->pAdjoin3[INDEX_BOTTOM], pOtherSq);
                break;
            case ADJOIN_FLAG_ALL:       // 四邻边，分别填充各个方向
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
// 获取下一个索引
inline void GetNextIndex(int nRow, int nCol, int &nNextRow, int &nNextCol)
{
	if (nCol >= g_nGameSize - 1)    // 换行
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
// 深度优先搜索
bool TraceProc2(int nRow, int nCol, int nSquareCount)       // GameSize == 2
{
	if (nRow >= g_nGameSize)
		return true;

    Square *pSq = g_ppGnome[nRow][nCol];
	int nNextRow = 0, nNextCol = 0, i = 0;

	GetNextIndex(nRow, nCol, nNextRow, nNextCol);
    if (pSq != NULL)        // 已经存在
    {
		return TraceProc2(nNextRow, nNextCol, nSquareCount);
    }
    else	// 不存在则遍历
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
// 内接容器获取下一个索引
inline void GetNextIndexInner(int nRow, int nCol, int &nNextRow, int &nNextCol)
{
    if (nCol >= g_nGameSize - 2)    // 换行
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
    // 顺时针转
    if (nRow == 0)    // 上边缘或右上角  
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
    else if (nCol == g_nGameSize - 1)   // 右边缘或右下角
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
    else if (nRow == g_nGameSize - 1)   // 下边缘或左下角
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
    else if (nCol == 0)     // 左边缘或左上角
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
    if (pCurrSq != NULL)        // 已经存在，只可能是四个角
    {
        return TraceOuterM(nNextRow, nNextCol, pCurrSq);
    }
    else    // 填充
    {
        if (nRow == 0)      // 左上角、上边缘、右上角
        {

        }
        else if (nCol == g_nGameSize - 1)   // 右边缘、右下角
        {

        }
        else if (nRow == g_nGameSize - 1)   // 下边缘、左下角
        {

        }
        else if (nCol == 0)     // 左边缘
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

    if (nRow >= g_nGameSize - 1)    // 内接容器填充完成，分别填充上、右、下、左边缘
    {
        return TraceOuterM(0, 0, NULL);   // 从左上角开始
    }
    else    // 填充内接容器，只能填充四邻接
    {
        // pSqL 与 pSqT 中必定有一个不为 NULL
        if (pSqL != NULL && pSqT != NULL)     // 有左上角邻接
        {
            pCurrAdjoin = (g_ppAdjoin4Table[pSqL->pnNum[INDEX_RIGHT]] + pSqT->pnNum[INDEX_BOTTOM]);
        }
        else if (pSqL != NULL)   // 只有左邻接
        {
            pCurrAdjoin = (pSqL->pAdjoin4 + INDEX_RIGHT);
        }
        else if (pSqT != NULL)    // 只有上邻接
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

    // 从内接容器的左上角开始
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
        // 输入方块中的三角
		InputSqaures(nSquareCount);
		// 预处理
		if (bPossible = PreProc(nSquareCount))
		{
			// 回溯处理，从左往右、从上往下回溯，当到行的最后一列时，下一个为下一行的起始
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