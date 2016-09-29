//-------------------------------------------------------------------------------------------
/** @file UMTSNwlCellSearchCtrl.c
 *
 * @brief the control functions of cell searcher  
 * @author Mindspeed Technologies
 * @version $Revision: 1.3.4.7 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------


#include "typedef.h"
#include "supervisor.h"
#include "prototypes.h"

#include "UMTSNwlCellSearchCtrl.h"

#include "UMTSNwlCommMacro.h"
#include <math.h>
#include "Debug.h"

typedef  void (*CB_FUN_TYPE) (LPVOID);

CellSrchDesc_t tCellSrchDesc;

#define PSC_OFFSET_LEN   (76800) //38400*2
extern NWL_CommInfor  *pNwlCommInfo;
extern S16  *gUMTSNwlAntData0; // for ant0
extern S8   *gUMTSNwlPscBuf;
extern U8    gCellSrcStartFlag;
extern U8    gBchDecodeStartFlag;
extern NWL_CELLSRCHINFO *pCellSrchMsg;
extern U8    gCellSearchBusyFlag;
extern S8 HadamaSeq16[16][16];
extern U8 GrpSlotMapTbl[64][15];

volatile U32 gDmaDoneFlag=0;

static const S16 astanTable_Q16[256][2]=
{
	0,		    32767,
	804,		32757,
	1607,		32727,
	2410,		32678,
	3211,		32609,
	4011,		32520,
	4807,		32412,
	5601,		32284,
	6392,		32137,
	7179,		31970,
	7961,		31785,
	8739,		31580,
	9511,		31356,
	10278,		31113,
	11038,		30851,
	11792,		30571,
	12539,		30272,
	13278,		29955,
	14009,		29621,
	14732,		29268,
	15446,		28897,
	16150,		28510,
	16845,		28105,
	17530,		27683,
	18204,		27244,
	18867,		26789,
	19519,		26318,
	20159,		25831,
	20787,		25329,
	21402,		24811,
	22004,		24278,
	22594,		23731,
	23169,		23169,
	23731,		22594,
	24278,		22004,
	24811,		21402,
	25329,		20787,
	25831,		20159,
	26318,		19519,
	26789,		18867,
	27244,		18204,
	27683,		17530,
	28105,		16845,
	28510,		16150,
	28897,		15446,
	29268,		14732,
	29621,		14009,
	29955,		13278,
	30272,		12539,
	30571,		11792,
	30851,		11038,
	31113,		10278,
	31356,		9511,
	31580,		8739,
	31785,		7961,
	31970,		7179,
	32137,		6392,
	32284,		5601,
	32412,		4807,
	32520,		4011,
	32609,		3211,
	32678,		2410,
	32727,		1607,
	32757,		804,
	32767,		0,
	32757,		-804,
	32727,		-1607,
	32678,		-2410,
	32609,		-3211,
	32520,		-4011,
	32412,		-4807,
	32284,		-5601,
	32137,		-6392,
	31970,		-7179,
	31785,		-7961,
	31580,		-8739,
	31356,		-9511,
	31113,		-10278,
	30851,		-11038,
	30571,		-11792,
	30272,		-12539,
	29955,		-13278,
	29621,		-14009,
	29268,		-14732,
	28897,		-15446,
	28510,  	-16150,
	28105,		-16845,
	27683,		-17530,
	27244,		-18204,
	26789,		-18867,
	26318,		-19519,
	25831,		-20159,
	25329,		-20787,
	24811,		-21402,
	24278,		-22004,
	23731,		-22594,
	23169,		-23169,
	22594,		-23731,
	22004,		-24278,
	21402,		-24811,
	20787,		-25329,
	20159,		-25831,
	19519,		-26318,
	18867,		-26789,
	18204,		-27244,
	17530,		-27683,
	16845,		-28105,
	16150,		-28510,
	15446,		-28897,
	14732,		-29268,
	14009,		-29621,
	13278,		-29955,
	12539,		-30272,
	11792,		-30571,
	11038,		-30851,
	10278,		-31113,
	9511,		-31356,
	8739,		-31580,
	7961,		-31785,
	7179,		-31970,
	6392,		-32137,
	5601,		-32284,
	4807,		-32412,
	4011,		-32520,
	3211,		-32609,
	2410,		-32678,
	1607,		-32727,
	804,		-32757,
	0,		    -32767,
	-804	,	-32757,
	-1607,		-32727,
	-2410,		-32678,
	-3211,		-32609,
	-4011,		-32520,
	-4807,		-32412,
	-5601,		-32284,
	-6392,		-32137,
	-7179,		-31970,
	-7961,		-31785,
	-8739,		-31580,
	-9511,		-31356,
	-10278,		-31113,
	-11038,		-30851,
	-11792,		-30571,
	-12539,		-30272,
	-13278,		-29955,
	-14009,		-29621,
	-14732,		-29268,
	-15446,		-28897,
	-16150,		-28510,
	-16845,		-28105,
	-17530,		-27683,
	-18204,		-27244,
	-18867,		-26789,
	-19519,		-26318,
	-20159,		-25831,
	-20787,		-25329,
	-21402,		-24811,
	-22004,		-24278,
	-22594,		-23731,
	-23169,		-23169,
	-23731,		-22594,
	-24278,		-22004,
	-24811,		-21402,
	-25329,		-20787,
	-25831,		-20159,
	-26318,		-19519,
	-26789,		-18867,
	-27244,		-18204,
	-27683,		-17530,
	-28105,		-16845,
	-28510,		-16150,
	-28897,		-15446,
	-29268,		-14732,
	-29621,		-14009,
	-29955,		-13278,
	-30272,		-12539,
	-30571,		-11792,
	-30851,		-11038,
	-31113,		-10278,
	-31356,		-9511,
	-31580,		-8739,
	-31785,		-7961,
	-31970,		-7179,
	-32137,		-6392,
	-32284,		-5601,
	-32412,		-4807,
	-32520,		-4011,
	-32609,		-3211,
	-32678,		-2410,
	-32727,		-1607,
	-32757,		-804,
	-32767,		0,
	-32757,		804,
	-32727,		1607,
	-32678,		2410,
	-32609,		3211,
	-32520,		4011,
	-32412,		4807,
	-32284,		5601,
	-32137,		6392,
	-31970,		7179,
	-31785,		7961,
	-31580,		8739,
	-31356,		9511,
	-31113,		10278,
	-30851,		11038,
	-30571,		11792,
	-30272,		12539,
	-29955,		13278,
	-29621,		14009,
	-29268,		14732,
	-28897,		15446,
	-28510,		16150,
	-28105,		16845,
	-27683,		17530,
	-27244,		18204,
	-26789,		18867,
	-26318,		19519,
	-25831,		20159,
	-25329,		20787,
	-24811,		21402,
	-24278,		22004,
	-23731,		22594,
	-23169,		23169,
	-22594,		23731,
	-22004,		24278,
	-21402,		24811,
	-20787,		25329,
	-20159,		25831,
	-19519,		26318,
	-18867,		26789,
	-18204,		27244,
	-17530,		27683,
	-16845,		28105,
	-16150,		28510,
	-15446,		28897,
	-14732,		29268,
	-14009,		29621,
	-13278,		29955,
	-12539,		30272,
	-11792,		30571,
	-11038,		30851,
	-10278, 	31113,
	-9511,		31356,
	-8739,		31580,
	-7961,		31785,
	-7179,		31970,
	-6392,		32137,
	-5601,		32284,
	-4807,		32412,
	-4011,		32520,
	-3211,		32609,
	-2410,		32678,
	-1607,		32727,
	-804,		32757
};



U32 PscCnt = 0;
U32 gAntOffset = 0;


#define UMTSNMM_CS_TCBLIST_NUM   (9)
#define UMTSNMM_CS_TCB_MAXIMUM (10)

PTASKLIST     umtsnmm_cs_task_list[UMTSNMM_CS_TCBLIST_NUM] = {NULL};
PTCB             umtsnmm_cs_tcb_arr[UMTSNMM_CS_TCBLIST_NUM][UMTSNMM_CS_TCB_MAXIMUM] = { 0 };
#ifdef RT_MODE
NCNBVAR volatile U32    umtsnmm_cs_task_list_done[UMTSNMM_CS_TCBLIST_NUM];
NCNBVAR volatile U32    umtsnmm_cs_task_list_time[UMTSNMM_CS_TCBLIST_NUM];
NCNBVAR volatile U32    umtsnmm_cs_task_list_count[UMTSNMM_CS_TCBLIST_NUM];
#else
volatile U32    umtsnmm_cs_task_list_done[UMTSNMM_CS_TCBLIST_NUM];
volatile U32    umtsnmm_cs_task_list_count[UMTSNMM_CS_TCBLIST_NUM];
#endif


enum Umtsnmm_TCBLIST_Src
{
    TCB_PSCH_TSKLST=0,
    TCB_SSCH_TSKLST,
    TCB_CPICH_TSKLST,
    TCB_PM_TSKLST,
    TCB_FREQ_TSKLST,
    TCB_RSCP_TSKLST,
    TCB_AGC_TSKLST,
    TCB_DOWN_SAMPLE_TSKLST,
    TCB_GEN_PSC_TASKLST
};


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  configure the TCB parameter for primary synchronization processing.

 Function Umtsnmm_CreatPschTcb.

 @param    pTskLst = the TCB resource
 @param    RxData = input data
 @param    accLen = accumulation length
 @param    accCount = accumulation counter
 @param    pschPow = a buffer to save accumulation result
 @param    pschTmpBuf = a buffer to save accumulation result
 @param    subWinLen = window length
 @param     dep_tcb =   the pointer of tcb.
 @param     RefillFlag =   the flag to show is create tcb or fill tcb.
 @param     refill_tcb =   the pointer of tcb.
 @param     TcbIdx =   the tcb index below a tcb list.
 @return    tcb pointer.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
PTCB Umtsnmm_CreatPschTcb(PTASKLIST pTskLst,S16* RxData,U8 accLen,U16 accCount,U32* pschPow,
    S32* pschTmpBuf,U16  subWinLen,PTCB * dep_tcb,U16 RefillFlag, PTCB refill_tcb,U8 tcbidx)
{
	PTCB pTCB = 0;
	U32* pContext = 0;

  //  nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if (RefillFlag == TCBIOBUFFILL)
    {
#ifdef USE_TCBQ
       pTCB = RexCreateTCBQ(TCB_PSCH_TSKID, RESOURCE_CEVA, NULL, 3 * sizeof(U32), NULL, NULL,TIME_UMTSNMMLOOP);
#else
	pTCB = CreateTCB(TCB_PSCH_TSKID, RESOURCE_CEVA, NULL, 3 * sizeof(U32), NULL, NULL);
#endif

	pContext = (U32*) pTCB->ContextPtr;
	pContext[0] = accCount;
	pContext[1] = accLen;
	pContext[2] = subWinLen;


	CreateTCBIOBuf(pTCB, 3);
	FillTCBIOBuf(pTCB, (U32*) RxData,   (subWinLen+256)*2*2 * sizeof(S16), IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) pschPow,  subWinLen*2*sizeof(U32), OUT_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) pschTmpBuf,  subWinLen*15*sizeof(U32),INOUT_BUF, NULL);
        

#if UMTSNMM_USE_TCB_LIST
        AddTCBToList(pTCB, pTskLst, NULL);
        //AddDependency(pTCB, 1, dep_tcb[tcbidx]);
#else
        ExeTSchedSingle(pTCB);
        pTCB = NULL;
#endif
    }
    else 
    {
        pTCB = refill_tcb;

		pContext = (U32*) pTCB->ContextPtr;
		pContext[0] = accCount;
		pContext[1] = accLen;
		pContext[2] = subWinLen;

		ReFillTCBIOBuf(pTCB, (U32*) RxData,   (subWinLen+256)*2*2 * sizeof(S16), IN_BUF, 0);
		ReFillTCBIOBuf(pTCB, (U32*) pschPow,  subWinLen*2*sizeof(U32), OUT_BUF, 1);
		ReFillTCBIOBuf(pTCB, (U32*) pschTmpBuf,  subWinLen*15*sizeof(U32),INOUT_BUF, 2);
    }
    
   // nmm_dbg_printf_off(("done.\r\n"));

	return pTCB;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  configure the TCB parameter for second synchronization processing.

 Function Umtsnmm_CreatSschTcb.

 @param    pTskLst = the TCB resource
 @param    RxData = input data
 @param    pZseq = Z sequence
 @param    pHadamSeq = hadam sequence
 @param    pGrpMap = group map
 @param    accLen = accumulation length
 @param    accCount = accumulation counter
 @param    pFHTRst = the result of fast hadam transfer 
 @param    pPscGrp = parimary scrambling code group 
 @param    pSlot = slot index 
 @param    PkPos = peak position
 @param    thresholdFactor = threshold factor
 @param    SschPow = the power
 @param    SschMean = the mean value of noise
 @param     dep_tcb =   the pointer of tcb.
 @param     RefillFlag =   the flag to show is create tcb or fill tcb.
 @param     refill_tcb =   the pointer of tcb.
 @param     TcbIdx =   the tcb index below a tcb list.
 @return    tcb pointer.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
PTCB Umtsnmm_CreatSschTcb(PTASKLIST pTskLst,S16 RxData[],S8 pZseq[],S8 pHadamSeq[],U8 *pGrpMap,U32 accLen, 
    U32 accCount,U32* pFHTRst,U8* pPscGrp,U8* pSlot, U32 PkPos,U8 thresholdFactor,U32* SschPow,
    U32* SschMean,PTCB * dep_tcb,U16 RefillFlag, PTCB refill_tcb,U8 tcbidx)
{

	PTCB pTCB = 0;
	U32* pContext = 0;

  //  nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if (RefillFlag == TCBIOBUFFILL)
    {
#ifdef USE_TCBQ
       pTCB = RexCreateTCBQ(TCB_SSCH_TSKID, RESOURCE_CEVA, NULL, 4 * sizeof(U32), NULL, NULL,TIME_UMTSNMMLOOP);
#else
	pTCB = CreateTCB(TCB_SSCH_TSKID, RESOURCE_CEVA, NULL, 4 * sizeof(U32), NULL, NULL);
#endif

	pContext = (U32*) pTCB->ContextPtr;
	pContext[0] = accCount;
	pContext[1] = accLen;
	pContext[2] = PkPos;
	pContext[3] = thresholdFactor;
    
	CreateTCBIOBuf(pTCB, 9);
	FillTCBIOBuf(pTCB, (U32*) RxData,  (CHIP_NUM_PER_SLOT+256)*2*2 * sizeof(S16), IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) pZseq,          256, IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) pHadamSeq,         16*16, IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) pGrpMap,          64*15, IN_BUF, NULL);
	
	FillTCBIOBuf(pTCB, pFHTRst,   15*16*sizeof(U32), INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, pPscGrp,    1, INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, pSlot,    1, INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, SschPow,    4, INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, SschMean,    4, INOUT_BUF, NULL);        

#if UMTSNMM_USE_TCB_LIST
        AddTCBToList(pTCB, pTskLst, NULL);
        //AddDependency(pTCB, 1, dep_tcb[tcbidx]);
#else
        ExeTSchedSingle(pTCB);
        pTCB = NULL;
#endif
    }
    else 
    {
        pTCB = refill_tcb;

	pContext = (U32*) pTCB->ContextPtr;
	pContext[0] = accCount;
	pContext[1] = accLen;
	pContext[2] = PkPos;
	pContext[3] = thresholdFactor;
    
	ReFillTCBIOBuf(pTCB, (U32*) RxData,  (CHIP_NUM_PER_SLOT+256)*2*2 * sizeof(S16), IN_BUF, 0);
	ReFillTCBIOBuf(pTCB, (U32*) pZseq,          256, IN_BUF, 1);
	ReFillTCBIOBuf(pTCB, (U32*) pHadamSeq,         16*16, IN_BUF, 2);
	ReFillTCBIOBuf(pTCB, (U32*) pGrpMap,          64*15, IN_BUF, 3);
	
	ReFillTCBIOBuf(pTCB, pFHTRst,   15*16*sizeof(U32), INOUT_BUF, 4);
	ReFillTCBIOBuf(pTCB, pPscGrp,    1, INOUT_BUF, 5);
	ReFillTCBIOBuf(pTCB, pSlot,    1, INOUT_BUF, 6);
	ReFillTCBIOBuf(pTCB, SschPow,    4, INOUT_BUF, 7);
	ReFillTCBIOBuf(pTCB, SschMean,    4, INOUT_BUF, 8);        
    }
    
   // nmm_dbg_printf_off(("done.\r\n"));
	return pTCB;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  configure the TCB parameter for detect scrambling code processing.

 Function Umtsnmm_CreatPcpichTcb.

 @param    pTskLst = the TCB resource
 @param    RxData = input data
 @param    PscCode = primary scrambling code
 @param    conherentLen = coherent accumulation length
 @param    pSynPeak = peak information
 @param    PkIdx = peak index
 @param    TskIdx = task index
 @param     dep_tcb =   the pointer of tcb.
 @param     RefillFlag =   the flag to show is create tcb or fill tcb.
 @param     refill_tcb =   the pointer of tcb.
 @param     TcbIdx =   the tcb index below a tcb list.
 @return    tcb pointer.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
PTCB Umtsnmm_CreatPcpichTcb(PTASKLIST pTskLst,S16 RxData[],S8 *PscCode[],U16 conherentLen,
    PschPeak_t* pSynPeak,U8 PkIdx,U8 TskIdx,PTCB * dep_tcb,U16 RefillFlag, PTCB refill_tcb,U8 tcbidx)
{

	PTCB pTCB = 0;
	U32* pContext = 0;

//  nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if (RefillFlag == TCBIOBUFFILL)
    {
#ifdef USE_TCBQ
       pTCB = RexCreateTCBQ(TCB_CPICH_TSKID, RESOURCE_CEVA, NULL, 1 * sizeof(U32), NULL, NULL,TIME_UMTSNMMLOOP);
#else
	pTCB = CreateTCB(TCB_CPICH_TSKID, RESOURCE_CEVA, NULL, 1 * sizeof(U32), NULL, NULL);
#endif

	pContext = (U32*) pTCB->ContextPtr;
	pContext[0] = conherentLen;

	CreateTCBIOBuf(pTCB, 10);
	FillTCBIOBuf(pTCB, (U32*) RxData,           conherentLen*2*2 * sizeof(S16), IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PscCode[0],          conherentLen*2, IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PscCode[1],          conherentLen*2, IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PscCode[2],           conherentLen*2, IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PscCode[3],           conherentLen*2, IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PscCode[4],           conherentLen*2, IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PscCode[5],           conherentLen*2, IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PscCode[6],           conherentLen*2, IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PscCode[7],           conherentLen*2, IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) &pSynPeak->PsynPk.CpiAcc[PkIdx][TskIdx][0],8*sizeof(U32), INOUT_BUF, NULL);

#if UMTSNMM_USE_TCB_LIST
        AddTCBToList(pTCB, pTskLst, NULL);
        //AddDependency(pTCB, 1, dep_tcb[tcbidx]);
#else
        ExeTSchedSingle(pTCB);
        pTCB = NULL;
#endif
    }
    else 
    {
        pTCB = refill_tcb;

	pContext = (U32*) pTCB->ContextPtr;
	pContext[0] = conherentLen;

	ReFillTCBIOBuf(pTCB, (U32*) RxData,  conherentLen*2*2 * sizeof(S16), IN_BUF,	0);
	ReFillTCBIOBuf(pTCB, (U32*) PscCode[0],  conherentLen*2, IN_BUF, 1);
	ReFillTCBIOBuf(pTCB, (U32*) PscCode[1],  conherentLen*2, IN_BUF, 2);
	ReFillTCBIOBuf(pTCB, (U32*) PscCode[2],  conherentLen*2, IN_BUF, 3);
	ReFillTCBIOBuf(pTCB, (U32*) PscCode[3],  conherentLen*2, IN_BUF, 4);
	ReFillTCBIOBuf(pTCB, (U32*) PscCode[4],   conherentLen*2, IN_BUF, 5);
	ReFillTCBIOBuf(pTCB, (U32*) PscCode[5],   conherentLen*2, IN_BUF, 6);
	ReFillTCBIOBuf(pTCB, (U32*) PscCode[6],   conherentLen*2, IN_BUF, 7);
	ReFillTCBIOBuf(pTCB, (U32*) PscCode[7],   conherentLen*2, IN_BUF, 8);
	ReFillTCBIOBuf(pTCB, (U32*) &pSynPeak->PsynPk.CpiAcc[PkIdx][TskIdx][0],8*sizeof(U32),
        INOUT_BUF, 	9);
     
    }
    
   // nmm_dbg_printf_off(("done.\r\n"));
    
	return pTCB;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  configure the TCB parameter for path search processing.

 Function Umtsnmm_CreatPmTcb.

 @param    pTskLst = the TCB resource
 @param    Ant0Input = input data of antenna 0
 @param    Ant1Input = input data of antenna 1
 @param    PscCode = primary scrambling code
 @param    AntNum = antenna number
 @param    accLen = accumulation length
 @param    PDPAnt0New = the power of antenna 0
 @param    PDPAnt1New = the power of antenna 1
 @param    PDPAnt0His = the history power of antenna 0
 @param    PDPAnt1His = the history power of antenna 1
 @param    pCellPath = path information
 @param    WinLen = accumulation window length
 @param    p8xAnt0 = a buffer to save interpolation for antenna 0
 @param    p8xAnt1 = a buffer to save interpolation for antenna 1
 @param    pCom2AntPk = peak information of combined data of two antenna
 @param    Count = counter
 @param    thresholdFactor = threshold factor
 @param    angle = the angle value of frequency offset
 @param    angTanTable = a static table of sin and cos function
 @param     dep_tcb =   the pointer of tcb.
 @param     RefillFlag =   the flag to show is create tcb or fill tcb.
 @param     refill_tcb =   the pointer of tcb.
 @param     TcbIdx =   the tcb index below a tcb list.
 @return    tcb pointer.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
PTCB Umtsnmm_CreatPmTcb(PTASKLIST pTskLst,S16 Ant0Input[],S16 Ant1Input[],S8 PscCode[],U8 AntNum,
    U16 accLen,U32 PDPAnt0New[],U32 PDPAnt1New[],	U32 PDPAnt0His[],U32 PDPAnt1His[],
    CellPathInfo* pCellPath,U16 WinLen,U32 *p8xAnt0,U32 *p8xAnt1,U32 *pCom2AntPk,U32 Count,
    U8 thresholdFactor,S16 angle,S16* angTanTable,PTCB * dep_tcb,U16 RefillFlag, PTCB refill_tcb,U8 tcbidx)
{

	PTCB pTCB = 0;
	U32* pContext = 0;

//  nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if (RefillFlag == TCBIOBUFFILL)
    {
#ifdef USE_TCBQ
       pTCB = RexCreateTCBQ(TCB_PM_TSKID, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL,TIME_UMTSNMMLOOP);
#else
	pTCB = CreateTCB(TCB_PM_TSKID, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL);
#endif


        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = Count;
        pContext[1] = accLen;
        pContext[2] = AntNum;
        pContext[3] = thresholdFactor;
        pContext[4] = angle;

	CreateTCBIOBuf(pTCB, 12);
	FillTCBIOBuf(pTCB, (U32*) Ant0Input,           CHIP_NUM_PER_SLOT*SAMPLE_RATE*2 * sizeof(S16), IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) Ant1Input,           CHIP_NUM_PER_SLOT*SAMPLE_RATE*2 * sizeof(S16), IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PscCode,          PM_CONHERENT_LEN*2, IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PDPAnt0New,          PM_SEARCH_WIN_LEN*SAMPLE_RATE*sizeof(U32), INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PDPAnt1New,          PM_SEARCH_WIN_LEN*SAMPLE_RATE*sizeof(U32), INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PDPAnt0His,           PM_SEARCH_WIN_LEN*SAMPLE_RATE*sizeof(U32), INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PDPAnt1His,           PM_SEARCH_WIN_LEN*SAMPLE_RATE*sizeof(U32), INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) p8xAnt0,           (PM_SEARCH_WIN_LEN*8+27)*sizeof(U32), INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) p8xAnt1,           (PM_SEARCH_WIN_LEN*8+27)*sizeof(U32), INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) pCom2AntPk,           PM_SEARCH_WIN_LEN*8*sizeof(U32), INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) pCellPath,          sizeof(CellPathInfo), INOUT_BUF, NULL);
    FillTCBIOBuf(pTCB, (U32*) angTanTable,          sizeof(S16)*256*2, IN_BUF, NULL);
    
#if UMTSNMM_USE_TCB_LIST
        AddTCBToList(pTCB, pTskLst, NULL);
        //AddDependency(pTCB, 1, dep_tcb[tcbidx]);
#else
        ExeTSchedSingle(pTCB);
        pTCB = NULL;
#endif
    }
    else 
    {
        pTCB = refill_tcb;

        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = Count;
        pContext[1] = accLen;
        pContext[2] = AntNum;
        pContext[3] = thresholdFactor;
        pContext[4] = angle;

        ReFillTCBIOBuf(pTCB, (U32*) Ant0Input,           CHIP_NUM_PER_SLOT*SAMPLE_RATE*2 * sizeof(S16), IN_BUF,  0);
        ReFillTCBIOBuf(pTCB, (U32*) Ant1Input,           CHIP_NUM_PER_SLOT*SAMPLE_RATE*2 * sizeof(S16), IN_BUF,  1);
        ReFillTCBIOBuf(pTCB, (U32*) PscCode,          PM_CONHERENT_LEN*2, IN_BUF,  2);
        ReFillTCBIOBuf(pTCB, (U32*) PDPAnt0New,          PM_SEARCH_WIN_LEN*SAMPLE_RATE*sizeof(U32), INOUT_BUF,  3);
        ReFillTCBIOBuf(pTCB, (U32*) PDPAnt1New,          PM_SEARCH_WIN_LEN*SAMPLE_RATE*sizeof(U32), INOUT_BUF, 4);
        ReFillTCBIOBuf(pTCB, (U32*) PDPAnt0His,           PM_SEARCH_WIN_LEN*SAMPLE_RATE*sizeof(U32), INOUT_BUF, 5);
        ReFillTCBIOBuf(pTCB, (U32*) PDPAnt1His,           PM_SEARCH_WIN_LEN*SAMPLE_RATE*sizeof(U32), INOUT_BUF, 6);
        ReFillTCBIOBuf(pTCB, (U32*) p8xAnt0,           (PM_SEARCH_WIN_LEN*8+27)*sizeof(U32), INOUT_BUF, 7);
        ReFillTCBIOBuf(pTCB, (U32*) p8xAnt1,           (PM_SEARCH_WIN_LEN*8+27)*sizeof(U32), INOUT_BUF,  8);
        ReFillTCBIOBuf(pTCB, (U32*) pCom2AntPk,           PM_SEARCH_WIN_LEN*8*sizeof(U32), INOUT_BUF,  9);
        ReFillTCBIOBuf(pTCB, (U32*) pCellPath,          sizeof(CellPathInfo), INOUT_BUF, 10);
        ReFillTCBIOBuf(pTCB, (U32*) angTanTable,          sizeof(S16)*256*2, IN_BUF, 11);
     
    }
    
   // nmm_dbg_printf_off(("done.\r\n"));
	return pTCB;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  configure the TCB parameter for calculate frequency offset processing.

 Function Umtsnmm_CreatFreqEstTcb.

 @param    pTskLst = the TCB resource
 @param    Ant0Input = input data of antenna 0
 @param    Ant1Input = input data of antenna 1
 @param    PscCode = primary scrambling code
 @param    AntNum = antenna number
 @param    pFreqConjSym = the symbol value 
 @param    accCount = accumulation counter
 @param    accLen = accumulation length
 @param     dep_tcb =   the pointer of tcb.
 @param     RefillFlag =   the flag to show is create tcb or fill tcb.
 @param     refill_tcb =   the pointer of tcb.
 @param     TcbIdx =   the tcb index below a tcb list.
 @return    tcb pointer.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
PTCB Umtsnmm_CreatFreqEstTcb(PTASKLIST pTskLst,S16 Ant0Input[],S16 Ant1Input[],S8 PscCode[],U8 AntNum,
S16 *pFreqConjSym,U16 accCount,U16 accLen,PTCB * dep_tcb,U16 RefillFlag, PTCB refill_tcb,U8 tcbidx)
{
	PTCB pTCB = 0;
	U32* pContext = 0;

//  nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if (RefillFlag == TCBIOBUFFILL)
    {
#ifdef USE_TCBQ
       pTCB = RexCreateTCBQ(TCB_FREQ_EST_TSKID, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL,TIME_UMTSNMMLOOP);
#else
	pTCB = CreateTCB(TCB_FREQ_EST_TSKID, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL);
#endif


	pContext = (U32*) pTCB->ContextPtr;
	pContext[0] = accCount;
	pContext[1] = accLen;
	pContext[2] = AntNum;

	CreateTCBIOBuf(pTCB, 4);
	FillTCBIOBuf(pTCB, (U32*) Ant0Input,           CHIP_NUM_PER_SLOT*2*2 * sizeof(S16), IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) Ant1Input,           CHIP_NUM_PER_SLOT*2*2 * sizeof(S16), IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PscCode,          CHIP_NUM_PER_SLOT*2, IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) pFreqConjSym,          20*sizeof(16), INOUT_BUF, NULL);
    
#if UMTSNMM_USE_TCB_LIST
        AddTCBToList(pTCB, pTskLst, NULL);
        //AddDependency(pTCB, 1, dep_tcb[tcbidx]);
#else
        ExeTSchedSingle(pTCB);
        pTCB = NULL;
#endif
    }
    else 
    {
        pTCB = refill_tcb;

		pContext = (U32*) pTCB->ContextPtr;
		pContext[0] = accCount;
		pContext[1] = accLen;
		pContext[2] = AntNum;

		ReFillTCBIOBuf(pTCB, (U32*) Ant0Input,           CHIP_NUM_PER_SLOT*2*2 * sizeof(S16), IN_BUF, 	0);
		ReFillTCBIOBuf(pTCB, (U32*) Ant1Input,           CHIP_NUM_PER_SLOT*2*2 * sizeof(S16), IN_BUF, 	1);
		ReFillTCBIOBuf(pTCB, (U32*) PscCode,          CHIP_NUM_PER_SLOT*2, IN_BUF, 2);
		ReFillTCBIOBuf(pTCB, (U32*) pFreqConjSym,          20*sizeof(16), INOUT_BUF, 	3);
    }
    
   // nmm_dbg_printf_off(("done.\r\n"));
    
	return pTCB;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  configure the TCB parameter for calculate RSCP processing.

 Function Umtsnmm_CreatRscpTcb.

 @param    pTskLst = the TCB resource
 @param    Ant0Input = input data of antenna 0
 @param    Ant1Input = input data of antenna 1
 @param    PscCode = primary scrambling code
 @param    AntNum = antenna number
 @param    pCellPath = the path information  
 @param    accCount = accumulation counter
 @param    accLen = accumulation length
 @param    angle = the angle value of frequency offset
 @param    angTanTable = a static table of sin and cos function
 @param     dep_tcb =   the pointer of tcb.
 @param     RefillFlag =   the flag to show is create tcb or fill tcb.
 @param     refill_tcb =   the pointer of tcb.
 @param     TcbIdx =   the tcb index below a tcb list.
 @return    tcb pointer.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
PTCB Umtsnmm_CreatRscpTcb(PTASKLIST pTskLst,S16 Ant0Input[],S16 Ant1Input[],S8 PscCode[],U8 AntNum,
CellPathInfo* pCellPath,U16 accCount,  U16 accLen,S16 angle,S16* angTanTable,
PTCB * dep_tcb,U16 RefillFlag, PTCB refill_tcb,U8 tcbidx)
{

	PTCB pTCB = 0;
	U32* pContext = 0;

//  nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if (RefillFlag == TCBIOBUFFILL)
    {
#ifdef USE_TCBQ
       pTCB = RexCreateTCBQ(TCB_RSCP_TSKID, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL,TIME_UMTSNMMLOOP);
#else
	pTCB = CreateTCB(TCB_RSCP_TSKID, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL);
#endif

	pContext = (U32*) pTCB->ContextPtr;
	pContext[0] = accCount;
	pContext[1] = accLen;
	pContext[2] = AntNum;
	pContext[3] = angle;
    
	CreateTCBIOBuf(pTCB, 5);
	FillTCBIOBuf(pTCB, (U32*) Ant0Input,           CHIP_NUM_PER_SLOT*2*2 * sizeof(S16), IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) Ant1Input,           CHIP_NUM_PER_SLOT*2*2 * sizeof(S16), IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PscCode,          CHIP_NUM_PER_SLOT*2, IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) pCellPath,          sizeof(CellPathInfo), INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) angTanTable,          sizeof(S16)*256*2, INOUT_BUF, NULL);
    
#if UMTSNMM_USE_TCB_LIST
        AddTCBToList(pTCB, pTskLst, NULL);
        //AddDependency(pTCB, 1, dep_tcb[tcbidx]);
#else
        ExeTSchedSingle(pTCB);
        pTCB = NULL;
#endif
    }
    else 
    {
        pTCB = refill_tcb;

		pContext = (U32*) pTCB->ContextPtr;
		pContext[0] = accCount;
		pContext[1] = accLen;
		pContext[2] = AntNum;
		pContext[3] = angle;
	    
		ReFillTCBIOBuf(pTCB, (U32*) Ant0Input,           CHIP_NUM_PER_SLOT*2*2 * sizeof(S16), IN_BUF, 	0);
		ReFillTCBIOBuf(pTCB, (U32*) Ant1Input,           CHIP_NUM_PER_SLOT*2*2 * sizeof(S16), IN_BUF, 	1);
		ReFillTCBIOBuf(pTCB, (U32*) PscCode,          CHIP_NUM_PER_SLOT*2, IN_BUF, 2);
		ReFillTCBIOBuf(pTCB, (U32*) pCellPath,          sizeof(CellPathInfo), INOUT_BUF, 	3);
		ReFillTCBIOBuf(pTCB, (U32*) angTanTable,          sizeof(S16)*256*2, INOUT_BUF, 	4);
    }
    
   // nmm_dbg_printf_off(("done.\r\n"));
    
	return pTCB;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  configure the TCB parameter for AGC processing.

 Function Umtsnmm_CreatAgcTcb.

 @param    pTskLst = the TCB resource
 @param    pRxDataIn = input data 
 @param    rawPow = the power of raw data
 @param    agcLen = the data length of calculate gain factor
 @param    TotalTcb = the number of tcb
 @param    Complete = the complete flag  
 @param     dep_tcb =   the pointer of tcb.
 @param     RefillFlag =   the flag to show is create tcb or fill tcb.
 @param     refill_tcb =   the pointer of tcb.
 @param     TcbIdx =   the tcb index below a tcb list.
 @return    tcb pointer.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
PTCB Umtsnmm_CreatAgcTcb(PTASKLIST pTskLst,S16* pRxDataIn,U32* rawPow,U32 agcLen,
    U32 TotalTcb,U32* Complete,PTCB * dep_tcb,U16 RefillFlag, PTCB refill_tcb,U8 tcbidx)
{

	PTCB pTCB = 0;
	U32* pContext = 0;

//  nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if (RefillFlag == TCBIOBUFFILL)
    {
#ifdef USE_TCBQ
        pTCB = RexCreateTCBQ(TCB_AGC_TSKID, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL,TIME_UMTSNMMLOOP);
#else
        pTCB = CreateTCB(TCB_AGC_TSKID, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL);
#endif

        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = agcLen;
        pContext[1] = TotalTcb;			
        pContext[2] = tcbidx;
		
        CreateTCBIOBuf(pTCB, 3);
        FillTCBIOBuf(pTCB, (U32*) (pRxDataIn),agcLen*8, IN_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) (rawPow),4, INOUT_BUF, NULL);
        FillTCBIOBuf(pTCB, (U32*) Complete,4, INOUT_BUF, NULL);   
    
#if UMTSNMM_USE_TCB_LIST
        AddTCBToList(pTCB, pTskLst, NULL);
        //AddDependency(pTCB, 1, dep_tcb[tcbidx]);
#else
        ExeTSchedSingle(pTCB);
        pTCB = NULL;
#endif
    }
    else 
    {
        pTCB = refill_tcb;

        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = agcLen;
        pContext[1] = TotalTcb;			
        pContext[2] = tcbidx;
		
        ReFillTCBIOBuf(pTCB, (U32*) (pRxDataIn),agcLen*8, IN_BUF, 0);
        ReFillTCBIOBuf(pTCB, (U32*) (rawPow),4, INOUT_BUF, 1);
        ReFillTCBIOBuf(pTCB, (U32*) Complete,4, INOUT_BUF, 2);   
    }
    
   // nmm_dbg_printf_off(("done.\r\n"));
    
	return pTCB;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  configure the TCB parameter for down sampling processing.

 Function Umtsnmm_CreatDownSampTcb.

 @param    pTskLst = the TCB resource
 @param    pRxDataIn = input data 
 @param    pRxDataOut = output data 
 @param    agcFactor = agc factor
 @param    TotalTcb = the number of tcb
 @param    agcLen = the data length of need AGC processing
  @param    Complete = the complete flag  
 @param    Complete1 = another complete flag  
 @param     dep_tcb =   the pointer of tcb.
 @param     RefillFlag =   the flag to show is create tcb or fill tcb.
 @param     refill_tcb =   the pointer of tcb.
 @param     TcbIdx =   the tcb index below a tcb list.
 @return    tcb pointer.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
PTCB Umtsnmm_CreatDownSampTcb(PTASKLIST pTskLst,S16* pRxDataIn,S16* pRxDataOut,U32 agcFactor,
    U32 TotalTcb,U32 agcLen,U32* Complete, U32* Complete1,PTCB * dep_tcb,U16 RefillFlag, PTCB refill_tcb,U8 tcbidx)
{

	PTCB pTCB = 0;
	U32* pContext = 0;

//  nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if (RefillFlag == TCBIOBUFFILL)
    {
#ifdef USE_TCBQ
        pTCB = RexCreateTCBQ(TCB_DOWN_SAMPLE_TSKID, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL,TIME_UMTSNMMLOOP);
#else
        pTCB = CreateTCB(TCB_DOWN_SAMPLE_TSKID, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL);
#endif

        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = agcLen;
        pContext[1] = agcFactor;			
        pContext[2] = tcbidx;
        pContext[3] = TotalTcb;
        
	CreateTCBIOBuf(pTCB, 4);
	FillTCBIOBuf(pTCB, (U32*)(pRxDataIn), agcLen*8, IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*)(pRxDataOut),agcLen*4, INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) Complete,	4, INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) Complete1, 4, INOUT_BUF, NULL);
    
#if UMTSNMM_USE_TCB_LIST
        AddTCBToList(pTCB, pTskLst, NULL);
        //AddDependency(pTCB, 1, dep_tcb[tcbidx]);
#else
        ExeTSchedSingle(pTCB);
        pTCB = NULL;
#endif
    }
    else 
    {
        pTCB = refill_tcb;

        pContext = (U32*) pTCB->ContextPtr;
        pContext[0] = agcLen;
        pContext[1] = agcFactor;			
        pContext[2] = tcbidx;
        pContext[3] = TotalTcb;

        ReFillTCBIOBuf(pTCB, (U32*) (pRxDataIn), agcLen*8, IN_BUF, 0);
        ReFillTCBIOBuf(pTCB, (U32*) (pRxDataOut),agcLen*4, INOUT_BUF, 1);
        ReFillTCBIOBuf(pTCB, (U32*) Complete,	4, INOUT_BUF, 2);
        ReFillTCBIOBuf(pTCB, (U32*) Complete1, 4, INOUT_BUF, 3);
    }
    
   // nmm_dbg_printf_off(("done.\r\n"));
    
	return pTCB;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  configure the TCB parameter for generate downlink scrambling code processing.

 Function umtsnmm_CreatGenPSCTcb.

 @param    pTskLst = the TCB resource
 @param    PscIdx = the scrambling code index 
 @param    pOut = output data 
 @param     dep_tcb =   the pointer of tcb.
 @param     RefillFlag =   the flag to show is create tcb or fill tcb.
 @param     refill_tcb =   the pointer of tcb.
 @return    tcb pointer.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
PTCB umtsnmm_CreatGenPSCTcb(PTASKLIST pTskLst,U16 PscIdx, S8 *pOut,PTCB * dep_tcb,U16 RefillFlag, PTCB refill_tcb)
{

	PTCB pTCB = 0;
	U32* pContext = 0;


//  nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if (RefillFlag == TCBIOBUFFILL)
    {
#ifdef USE_TCBQ
       pTCB = RexCreateTCBQ(TCB_GEN_PSC_TASKID, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL,TIME_UMTSNMMLOOP);
#else
	pTCB = CreateTCB(TCB_GEN_PSC_TASKID, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL);
#endif

	pContext = (U32*) pTCB->ContextPtr;
	pContext[0] = PscIdx;

	CreateTCBIOBuf(pTCB, 1);
	FillTCBIOBuf(pTCB, (U32*) pOut, PSC_OFFSET_LEN, OUT_BUF, NULL);
    
#if UMTSNMM_USE_TCB_LIST
        AddTCBToList(pTCB, pTskLst, NULL);
        //AddDependency(pTCB, 1, dep_tcb[0]);
#else
        ExeTSchedSingle(pTCB);
        pTCB = NULL;
#endif
    }
    else 
    {
        pTCB = refill_tcb;

	pContext = (U32*) pTCB->ContextPtr;
	pContext[0] = PscIdx;

	ReFillTCBIOBuf(pTCB, (U32*) pOut, PSC_OFFSET_LEN, OUT_BUF, 0);
    
    }
    
   // nmm_dbg_printf_off(("done.\r\n"));
    
	return pTCB;
}


void Umtsnmm_CS_tasklist_doneCB(TASKLIST* thisList, void* param)
{
    _ASSERT_PTR(thisList);

    *((U32*)param) = 1;
#ifdef RT_MODE
	{
		U32 listNum;
		listNum = RexGetListID(thisList) - TCB_PSCH_TSKLSTID;
		MLogTask(PID_TASKLIST_EXEC_UMTSNMM_0+listNum, RESOURCE_LARM, umtsnmm_cs_task_list_time[listNum], GET_TICKS);
	}
#endif
}

#define TCB_PSCH_NUM    2
#define TCB_CPICH_NUM    5
#define TCB_PM_NUM    5
#define TCB_FREQ_NUM    5
#define TCB_RSCP_NUM    5
#define TCB_AGC_NUM    4
#define TCB_DOWNSAMP_NUM    4


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  initilize the TCB resource.

 Function Umtsnmm_CS_TcbInit.

 @param    none.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void Umtsnmm_CS_TcbInit(void)
{
    U32  loop;
    
    /*TCBs  for psch search*/
    if (umtsnmm_cs_task_list[TCB_PSCH_TSKLST] == NULL)
    {
        umtsnmm_cs_task_list[TCB_PSCH_TSKLST] = CreateTaskList(TCB_PSCH_TSKLSTID, NULL);
        _ASSERT_PTR(umtsnmm_cs_task_list[TCB_PSCH_TSKLST]);
        RegListDoneCB(umtsnmm_cs_task_list[TCB_PSCH_TSKLST], Umtsnmm_CS_tasklist_doneCB,  (void*)(umtsnmm_cs_task_list_done+TCB_PSCH_TSKLST));
    }
    
    for(loop=0;loop<TCB_PSCH_NUM;loop++)
    {
        umtsnmm_cs_tcb_arr[TCB_PSCH_TSKLST][loop] = Umtsnmm_CreatPschTcb(umtsnmm_cs_task_list[TCB_PSCH_TSKLST], 
                    NULL, 0,0, NULL,  NULL,0,umtsnmm_cs_tcb_arr[TCB_PSCH_TSKLST],TCBIOBUFFILL, NULL,loop);
    }

/*TCBs for Ssch Search*/
    if (umtsnmm_cs_task_list[TCB_SSCH_TSKLST] == NULL) 
    {
        umtsnmm_cs_task_list[TCB_SSCH_TSKLST] = CreateTaskList(TCB_SSCH_TSKLSTID, NULL);
        _ASSERT_PTR(umtsnmm_cs_task_list[TCB_SSCH_TSKLST]);
        RegListDoneCB(umtsnmm_cs_task_list[TCB_SSCH_TSKLST], Umtsnmm_CS_tasklist_doneCB,   (void*)(umtsnmm_cs_task_list_done+TCB_SSCH_TSKLST));
    }   

    for(loop=0;loop<UMTSNMM_CS_TCB_MAXIMUM;loop++)
    {
        umtsnmm_cs_tcb_arr[TCB_SSCH_TSKLST][loop] =   Umtsnmm_CreatSschTcb(umtsnmm_cs_task_list[TCB_SSCH_TSKLST], NULL, NULL, 
        NULL,NULL, 0, 0,NULL,NULL,NULL,0,0,NULL,NULL,  umtsnmm_cs_tcb_arr[TCB_SSCH_TSKLST], TCBIOBUFFILL,  NULL,loop);
    }


/*TCBs for CPICH*/
    if (umtsnmm_cs_task_list[TCB_CPICH_TSKLST] == NULL) 
    {
        umtsnmm_cs_task_list[TCB_CPICH_TSKLST] = CreateTaskList(TCB_CPICH_TSKLSTID, NULL);
        _ASSERT_PTR(umtsnmm_cs_task_list[TCB_CPICH_TSKLST]);
        RegListDoneCB(umtsnmm_cs_task_list[TCB_CPICH_TSKLST], Umtsnmm_CS_tasklist_doneCB,   (void*)(umtsnmm_cs_task_list_done+TCB_CPICH_TSKLST));
    }   

    for(loop=0;loop<TCB_CPICH_NUM;loop++)
    {
        umtsnmm_cs_tcb_arr[TCB_CPICH_TSKLST][loop] =   Umtsnmm_CreatPcpichTcb(umtsnmm_cs_task_list[TCB_CPICH_TSKLST], NULL, 
            NULL,0,NULL,0,0, umtsnmm_cs_tcb_arr[TCB_CPICH_TSKLST], TCBIOBUFFILL,  NULL,loop);
    }


    /*TCBs  for PM*/
    if (umtsnmm_cs_task_list[TCB_PM_TSKLST] == NULL)
    {
        umtsnmm_cs_task_list[TCB_PM_TSKLST] = CreateTaskList(TCB_PM_TSKLSTID, NULL);
        _ASSERT_PTR(umtsnmm_cs_task_list[TCB_PM_TSKLST]);
        RegListDoneCB(umtsnmm_cs_task_list[TCB_PM_TSKLST], Umtsnmm_CS_tasklist_doneCB,  (void*)(umtsnmm_cs_task_list_done+TCB_PM_TSKLST));
    }
	for(loop=0;loop<TCB_PM_NUM;loop++)
	{
		umtsnmm_cs_tcb_arr[TCB_PM_TSKLST][loop] = Umtsnmm_CreatPmTcb(umtsnmm_cs_task_list[TCB_PM_TSKLST], 
					NULL,NULL,NULL,0,0,NULL,NULL,NULL,NULL,NULL,0,NULL,NULL,NULL,0,0,0,NULL,
					umtsnmm_cs_tcb_arr[TCB_PM_TSKLST],TCBIOBUFFILL, NULL,loop);
	}
	
    /*TCBs  for frequency offset*/
    if (umtsnmm_cs_task_list[TCB_FREQ_TSKLST] == NULL)
    {
        umtsnmm_cs_task_list[TCB_FREQ_TSKLST] = CreateTaskList(TCB_FREQ_TSKLSTID, NULL);
        _ASSERT_PTR(umtsnmm_cs_task_list[TCB_FREQ_TSKLST]);
        RegListDoneCB(umtsnmm_cs_task_list[TCB_FREQ_TSKLST], Umtsnmm_CS_tasklist_doneCB,  (void*)(umtsnmm_cs_task_list_done+TCB_FREQ_TSKLST));
    }
    for(loop=0;loop<TCB_FREQ_NUM;loop++)
    {
        umtsnmm_cs_tcb_arr[TCB_FREQ_TSKLST][loop] = Umtsnmm_CreatFreqEstTcb(umtsnmm_cs_task_list[TCB_FREQ_TSKLST], 
                    NULL,NULL,NULL,0,NULL,0,0, umtsnmm_cs_tcb_arr[TCB_FREQ_TSKLST],TCBIOBUFFILL, NULL,loop);
    }
    

    /*TCBs  for RSCP*/
    if (umtsnmm_cs_task_list[TCB_RSCP_TSKLST] == NULL)
    {
        umtsnmm_cs_task_list[TCB_RSCP_TSKLST] = CreateTaskList(TCB_RSCP_TSKLSTID, NULL);
        _ASSERT_PTR(umtsnmm_cs_task_list[TCB_RSCP_TSKLST]);
        RegListDoneCB(umtsnmm_cs_task_list[TCB_RSCP_TSKLST], Umtsnmm_CS_tasklist_doneCB,  (void*)(umtsnmm_cs_task_list_done+TCB_RSCP_TSKLST));
    }
    for(loop=0;loop<TCB_RSCP_NUM;loop++)
    {
        umtsnmm_cs_tcb_arr[TCB_RSCP_TSKLST][loop] = Umtsnmm_CreatRscpTcb(umtsnmm_cs_task_list[TCB_RSCP_TSKLST], 
                    NULL,NULL,NULL,0,NULL,0,0,0,NULL, umtsnmm_cs_tcb_arr[TCB_RSCP_TSKLST],TCBIOBUFFILL, NULL,loop);
    }


    /*TCBs  for AGC*/
    if (umtsnmm_cs_task_list[TCB_AGC_TSKLST] == NULL)
    {
        umtsnmm_cs_task_list[TCB_AGC_TSKLST] = CreateTaskList(TCB_AGC_TSKLSTID, NULL);
        _ASSERT_PTR(umtsnmm_cs_task_list[TCB_AGC_TSKLST]);
        RegListDoneCB(umtsnmm_cs_task_list[TCB_AGC_TSKLST], Umtsnmm_CS_tasklist_doneCB,  (void*)(umtsnmm_cs_task_list_done+TCB_AGC_TSKLST));
    }
    for(loop=0;loop<TCB_AGC_NUM;loop++)
    {
        umtsnmm_cs_tcb_arr[TCB_AGC_TSKLST][loop] = Umtsnmm_CreatAgcTcb(umtsnmm_cs_task_list[TCB_AGC_TSKLST], 
                    NULL,NULL,0,0,NULL, umtsnmm_cs_tcb_arr[TCB_AGC_TSKLST],TCBIOBUFFILL, NULL,loop);
    }


    /*TCBs  for down sampling*/
    if (umtsnmm_cs_task_list[TCB_DOWN_SAMPLE_TSKLST] == NULL)
    {
        umtsnmm_cs_task_list[TCB_DOWN_SAMPLE_TSKLST] = CreateTaskList(TCB_DOWN_SAMPLE_TSKLSTID, NULL);
        _ASSERT_PTR(umtsnmm_cs_task_list[TCB_DOWN_SAMPLE_TSKLST]);
        RegListDoneCB(umtsnmm_cs_task_list[TCB_DOWN_SAMPLE_TSKLST], Umtsnmm_CS_tasklist_doneCB,  (void*)(umtsnmm_cs_task_list_done+TCB_DOWN_SAMPLE_TSKLST));
    }
    for(loop=0;loop<TCB_DOWNSAMP_NUM;loop++)
    {
        umtsnmm_cs_tcb_arr[TCB_DOWN_SAMPLE_TSKLST][loop] = Umtsnmm_CreatDownSampTcb(umtsnmm_cs_task_list[TCB_DOWN_SAMPLE_TSKLST], 
                    NULL,NULL,0,0,0,NULL,NULL, umtsnmm_cs_tcb_arr[TCB_DOWN_SAMPLE_TSKLST],TCBIOBUFFILL, NULL,loop);
    }

    /*TCBs  for generate scrambling code*/
    if (umtsnmm_cs_task_list[TCB_GEN_PSC_TASKLST] == NULL)
    {
        umtsnmm_cs_task_list[TCB_GEN_PSC_TASKLST] = CreateTaskList(TCB_GEN_PSC_TASKLSTID, NULL);
        _ASSERT_PTR(umtsnmm_cs_task_list[TCB_GEN_PSC_TASKLST]);
        RegListDoneCB(umtsnmm_cs_task_list[TCB_GEN_PSC_TASKLST], Umtsnmm_CS_tasklist_doneCB,  (void*)(umtsnmm_cs_task_list_done+TCB_GEN_PSC_TASKLST));
    }
        umtsnmm_cs_tcb_arr[TCB_GEN_PSC_TASKLST][0] = umtsnmm_CreatGenPSCTcb(umtsnmm_cs_task_list[TCB_GEN_PSC_TASKLST], 
                   0,NULL, umtsnmm_cs_tcb_arr[TCB_GEN_PSC_TASKLST],TCBIOBUFFILL, NULL);
    

    //nmm_dbg_printf_off(("@ %s\tARM %d\tnmm_tcb_arr %x %x\ttime %lu\r\n", __FUNCTION__, MxGetCpuID(), umtsnmm_tcb_arr[0][0], umtsnmm_tcb_arr[0][1], (unsigned long) GET_TICKS));

    return;
}



void Umtsnmm_CS_TcbStop(void)
{
    U8 i;
    for (i = 0; i < UMTSNMM_CS_TCBLIST_NUM; i++)
    {
        if (umtsnmm_cs_task_list[i])
        {
            DestroyTaskList(umtsnmm_cs_task_list[i]);
            umtsnmm_cs_task_list[i] = NULL;
        }
    }

    return;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief cell searcher run TCB task list

 Function Umtsnmm_CS_runTaskList.

 @param    listNum = the index of tcb task list.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void Umtsnmm_CS_runTaskList(U32 listNum)
{
    U32 taskListRet;
    PTASKLIST pTaskList = NULL;

    if (listNum > UMTSNMM_CS_TCBLIST_NUM)
    {
        Umtsnmm_dbg_printf(("Umtsnmm_CS_runTaskList: Invalid listNum: %d\r\n", listNum));
        return;
    }

    pTaskList = umtsnmm_cs_task_list[listNum];

    umtsnmm_cs_task_list_done[listNum] = 0;
#ifdef RT_MODE
    umtsnmm_cs_task_list_time[listNum] = GET_TICKS;
#endif
    umtsnmm_cs_task_list_count[listNum]++;

    taskListRet = ExeTSchedList(pTaskList);
    if(taskListRet != MX_OK)
    {
        Umtsnmm_dbg_printf(("ERROR: Task list %d returned %d\r\n", listNum, taskListRet));
        return;
    }
    while(umtsnmm_cs_task_list_done[listNum] == 0);
    ReinitTaskList (pTaskList, NULL);
}



void Umtsnmm_CS_byPassMultTcb (TCB ** ppTCB, UINT32 nTcbNum, UINT32 nCtrl)
{
    U16 i;
    for(i=0;i<nTcbNum;i++)
    {
        MarkTCBControl(ppTCB[i], nCtrl,  NULL);
    }
}



void CellSrhFillRpt(CellSrchDesc_t *pCellSrchDesc)
{
    U32 i;
	float rscp_pow;
	float rssi_pow;
	S8 rscp_db;
	S8 rssi_db;	
	float agc_factor;
	S32 freqOffset;
	S16 angle;
	
	agc_factor = pCellSrchDesc->agcFactor;
    pCellSrchDesc->cellSrchRpt->tCellSearchInd.nr_cellFoundList = pCellSrchDesc->cellGrpDetcInfo->CellNum;

    for(i = 0; i <pCellSrchDesc->cellSrchRpt->tCellSearchInd.nr_cellFoundList; i++ )
    {
        pCellSrchDesc->cellSrchRpt->tCellSearchInd.cellFoundList[i].psc = 
            pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[i].PIndx;
		rscp_pow = (float)(pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[i].RscpAccFrame
			/pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[i].RscpAccCnt);

		rssi_pow = (float)(pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[i].RssiAccFrame
			/pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[i].RscpAccCnt);
		
		rscp_pow = rscp_pow/(agc_factor*agc_factor);
		rssi_pow = rssi_pow/(agc_factor*agc_factor);
		
		rscp_db = 10*log10(rscp_pow);
		rssi_db = 10*log10(rssi_pow);
		
		pCellSrchDesc->cellSrchRpt->tCellSearchInd.cellFoundList[i].rscp = rscp_db+30;
		angle = pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[i].angle;
		freqOffset = (angle*15000)/(2*(PI));
		pCellSrchDesc->cellSrchRpt->tCellSearchInd.cellFoundList[i].freqOffset = freqOffset/0x1000;
		pCellSrchDesc->cellSrchRpt->tCellSearchInd.cellFoundList[i].Ecio = rssi_db-rscp_db;

		printf(" Rssi: %d, Rscp:%d, Ec/Io:%d.\r\n", rssi_db,rscp_db,rssi_db-rscp_db);	
    }
    return;
    
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief cell searcher result report

 Function CellSearchRpt.

 @param    listNum = the index of tcb task list.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CellSearchRpt(U32* outMsg, U32 outMsgSize)
{
    ApiHeader * pApi;
    MXRC rc = MX_OK;


    pApi = (ApiHeader *) outMsg;

    pApi->Control   = 0x10000004;
    pApi->DstInstID = 2;
    pApi->SrcInstID = 0x8019;
    pApi->Length    = 0;        // The payload is not present yet
    pApi->Type      = API_TYPE_IND;
    pApi->MessageID = NWL_CELL_SEARCH_IND;

    // The command size was increased to the total size of parameter
    pApi->Length = outMsgSize;
    rc=SvsrSendAPI(pApi);
	if(MX_OK != rc)
	{
		printf("Cell Searcher report err: %d.\r\n", rc);
	}

    return;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief create TCB task list for Primary synchronization

 Function CreatPschTskList.

 @param    pCellSrchDesc = the output result
 @param    pRxdata = the input data
 @param    pPschPow = the buffer to save power
 @param    accCount = accumulation counter

 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CreatPschTskList(CellSrchDesc_t *pCellSrchDesc , S16* pRxdata,U32 *pPschPow,U16 accCount)
{
    U8	loop;
    U16	subWinLen;
    S16*    pRxdataTmp;
    U32*    pPschPowTmp;
    U16    pschAccLen;
    S32*  pschTmpBuf;
	
    subWinLen = pCellSrchDesc->pCellSrchPara->pschSubWinLen;
    pschAccLen = pCellSrchDesc->pCellSrchPara->pschAccLen*SLOT_NUM_PER_FREAM;
    for(loop = 0; loop < (CHIP_NUM_PER_SLOT/subWinLen); loop++)
    {
        pRxdataTmp = (pRxdata + loop*subWinLen*SAMPLE_RATE*2);
        pPschPowTmp = pPschPow + loop*subWinLen*SAMPLE_RATE;
        pschTmpBuf = pCellSrchDesc->pschTmpBuf + loop*subWinLen*15;

        Umtsnmm_CreatPschTcb(umtsnmm_cs_task_list[TCB_PSCH_TSKLST],pRxdataTmp,pschAccLen,accCount,pPschPowTmp,pschTmpBuf,subWinLen,
            NULL,TCBIOBUFREFILL, umtsnmm_cs_tcb_arr[TCB_PSCH_TSKLST][loop],loop);
    }
    
    Umtsnmm_CS_runTaskList(TCB_PSCH_TSKLST);

    return;
		
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief create TCB task list for second synchronization

 Function CreatSschTskList.

 @param    pCellSrchDesc = the output result
 @param    pAnt0Input = the input data of antenna 0
 @param    pAnt1Input = the input data of antenna 1
 @param    pZseq = Z sequence
 @param    pHadamSeq = hadam sequence
 @param    pGrpMap = group map
 @param    pSynPeak = peak information of primary synchronization
 @param    accCount = accumulation counter
 @param    PkPos =  peak position of primary synchronization
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CreatSschTskList(CellSrchDesc_t *pCellSrchDesc,S16* pAnt0Input,S16* pAnt1Input,S8 pZseq[],S8 pHadamSeq[],U8 *pGrpMap,PschPeak_t* pSynPeak,U16  accCount,PschPeakPos_t *PkPos)
{
	U32	loop;
	U32	accLen;
      U8 TcbCounter=0;
      
	accLen = pCellSrchDesc->pCellSrchPara->sschAccLen*SLOT_NUM_PER_FREAM;
	for(loop =0; loop < PkPos->PkNum ; loop++)
	{
		if(1==pCellSrchDesc->pCellSrchPara->rxAntNumber)
		{
                Umtsnmm_CreatSschTcb(umtsnmm_cs_task_list[TCB_SSCH_TSKLST],pAnt0Input,pZseq, pHadamSeq, pGrpMap,accLen, accCount,&(pSynPeak->PsynPk.FHTRst[loop][0][0]),
				&(pSynPeak->PsynPk.GrpNum[loop]),&(pSynPeak->PsynPk.SlotNum[loop]), PkPos->Position[loop],
				pCellSrchDesc->pCellSrchPara->sschThsdFactor,&(pSynPeak->PsynPk.SschPow[loop]),&(pSynPeak->PsynPk.SschMean[loop]),
                        NULL,TCBIOBUFREFILL, umtsnmm_cs_tcb_arr[TCB_SSCH_TSKLST][loop],loop);
        
                TcbCounter++;
		}
		else
		{
			if(pCellSrchDesc->pschAnt0Pow[PkPos->Position[loop]]>pCellSrchDesc->pschAnt1Pow[PkPos->Position[loop]])
			{                
                        Umtsnmm_CreatSschTcb(umtsnmm_cs_task_list[TCB_SSCH_TSKLST],pAnt0Input,pZseq, pHadamSeq, pGrpMap,accLen, accCount,&(pSynPeak->PsynPk.FHTRst[loop][0][0]),
        					&(pSynPeak->PsynPk.GrpNum[loop]),&(pSynPeak->PsynPk.SlotNum[loop]), PkPos->Position[loop],
        				pCellSrchDesc->pCellSrchPara->sschThsdFactor,&(pSynPeak->PsynPk.SschPow[loop]),&(pSynPeak->PsynPk.SschMean[loop]),
                                NULL,TCBIOBUFREFILL, umtsnmm_cs_tcb_arr[TCB_SSCH_TSKLST][loop],loop);
                        TcbCounter++;
			}
			else
			{
                         Umtsnmm_CreatSschTcb(umtsnmm_cs_task_list[TCB_SSCH_TSKLST],pAnt1Input,pZseq, pHadamSeq, pGrpMap,accLen, accCount,&(pSynPeak->PsynPk.FHTRst[loop][0][0]),
        					&(pSynPeak->PsynPk.GrpNum[loop]),&(pSynPeak->PsynPk.SlotNum[loop]), PkPos->Position[loop],
        				pCellSrchDesc->pCellSrchPara->sschThsdFactor,&(pSynPeak->PsynPk.SschPow[loop]),&(pSynPeak->PsynPk.SschMean[loop]),
                                NULL,TCBIOBUFREFILL, umtsnmm_cs_tcb_arr[TCB_SSCH_TSKLST][loop],loop);      
                        TcbCounter++;
			}
		}
	}

    Umtsnmm_CS_byPassMultTcb (umtsnmm_cs_tcb_arr[TCB_SSCH_TSKLST]+TcbCounter,  
        UMTSNMM_CS_TCB_MAXIMUM-TcbCounter, BYPASS_TCB);

    Umtsnmm_CS_runTaskList(TCB_SSCH_TSKLST);

    return;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief create TCB task list for detect scrambling code. 

 Function CreatPcpichTskList.

 @param    pRxData = the input data
 @param    pCellSrchDesc = the output result
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CreatPcpichTskList(S16* pRxData,CellSrchDesc_t *pCellSrchDesc)
{
	S8 *pPsc[8];	
	U32	loop;
	U8	taskNum;
	S16*  pRxDataTmp;
	U16	conherentLen;
	U8	peakIdx;
	conherentLen = pCellSrchDesc->pCellSrchPara->pcpichConherentLen*256;
	taskNum =CHIP_NUM_PER_SLOT/conherentLen;
	peakIdx = pCellSrchDesc->ThreadCtx->PkIdx;
	for(loop = 0; loop < taskNum; loop++)
	{
		pPsc[0] = pCellSrchDesc->PscSeqSlot +loop*conherentLen*2;
		pPsc[1] = pPsc[0]+CHIP_NUM_PER_SLOT*2;
		pPsc[2] = pPsc[1]+CHIP_NUM_PER_SLOT*2;
		pPsc[3] = pPsc[2]+CHIP_NUM_PER_SLOT*2;
		pPsc[4] = pPsc[3]+CHIP_NUM_PER_SLOT*2;
		pPsc[5] = pPsc[4]+CHIP_NUM_PER_SLOT*2;
		pPsc[6] = pPsc[5]+CHIP_NUM_PER_SLOT*2;
		pPsc[7] = pPsc[6]+CHIP_NUM_PER_SLOT*2;
		pRxDataTmp = pRxData + loop*conherentLen*2*SAMPLE_RATE;
        

        Umtsnmm_CreatPcpichTcb(umtsnmm_cs_task_list[TCB_CPICH_TSKLST],pRxDataTmp,pPsc,conherentLen,pCellSrchDesc->pschPeakInfo,peakIdx,loop,
                    NULL,TCBIOBUFREFILL, umtsnmm_cs_tcb_arr[TCB_CPICH_TSKLST][loop],loop);
	}

    if(taskNum<TCB_CPICH_NUM)
    {
        Umtsnmm_CS_byPassMultTcb (umtsnmm_cs_tcb_arr[TCB_CPICH_TSKLST]+taskNum,  
            TCB_CPICH_NUM-taskNum, BYPASS_TCB);
    }

    Umtsnmm_CS_runTaskList(TCB_CPICH_TSKLST);

    
    return;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief create TCB task list for path search.

 Function CreatPmTskList.

 @param    pRxData = the input data of antenna 0
 @param    pRxData = the input data of antenna 0
 @param    pCellSrchDesc = the cell search synchronization information
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CreatPmTskList(U32* pRxData, U32* pRxData1, CellSrchDesc_t *pCellSrchDesc)
{
    U8	tskNum;
    U8	tskIdx;
    U8   maxTskNumPerGrp;
    U8	pmGrpIdx;
    U8	grpNum;
    U32	i;
    S16*    pRxDataTmp;
    S16*    pRxDataTmp1;
    S8*	 pPsc;
    U8	antNum;
    U32* pPdpAnt0;
    U32* pPdpAnt1;
    U32* pPdpAnt0His;
    U32* pPdpAnt1His;
    CellPathInfo* pCellPath;
    U32* pPdp8xAnt0;
    U32* pPdp8xAnt1;
    U32* pPdpCom2Ant;
    U16	accLen;
    U16	accCount;
    U8	thresholdFactor;
    S16  angle;
    S16* angTanTable;
	
    pmGrpIdx = pCellSrchDesc->ThreadCtx->pmGrpIdx;
    maxTskNumPerGrp = pCellSrchDesc->pCellSrchPara->maxPmTskNum;
    grpNum = pCellSrchDesc->ThreadCtx->pmGrpNum;
    antNum = pCellSrchDesc->pCellSrchPara->rxAntNumber;
    accLen = pCellSrchDesc->pCellSrchPara->pmAccLen;
    accCount = pCellSrchDesc->ThreadCtx->SlotCnt;
    thresholdFactor = pCellSrchDesc->pCellSrchPara->pmThsdFactor;
    if(pmGrpIdx==(grpNum -1))
    {
    	tskNum = pCellSrchDesc->ThreadCtx->pmTskNumLastGrp;
    }
    else
    {
    	tskNum = maxTskNumPerGrp;
    }
	
    tskIdx = pmGrpIdx*maxTskNumPerGrp;

	for(i = 0; i < tskNum; i++)
	{
		pRxDataTmp = (S16*)(pRxData + pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[tskIdx+i].WinBegin);
		pRxDataTmp1 =(S16*)(pRxData1 + pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[tskIdx+i].WinBegin);
		pPsc = pCellSrchDesc->PscSeqSlot + i*CHIP_NUM_PER_SLOT*2;
		pPdpAnt0 = pCellSrchDesc->PDPAnt0 + i*PM_SEARCH_WIN_LEN*SAMPLE_RATE;
		pPdpAnt1 = pCellSrchDesc->PDPAnt1 + i*PM_SEARCH_WIN_LEN*SAMPLE_RATE;
		
		pPdpAnt0His = pCellSrchDesc->PDPAnt0Hist + i*(PM_SEARCH_WIN_LEN*SAMPLE_RATE+12);
		pPdpAnt1His = pCellSrchDesc->PDPAnt1Hist + i*(PM_SEARCH_WIN_LEN*SAMPLE_RATE+12);

		pPdp8xAnt0 = pCellSrchDesc->PDPAnt0_8x + i*(PM_SEARCH_WIN_LEN*8+27);
		pPdp8xAnt1 = pCellSrchDesc->PDPAnt1_8x + i*(PM_SEARCH_WIN_LEN*8+27);
		pPdpCom2Ant = pCellSrchDesc->PDP2AntCombine + i*(PM_SEARCH_WIN_LEN*8);
		pCellPath =& pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[tskIdx+i];
		angle = pCellPath->angle;
        angTanTable = pCellSrchDesc->pAngTanTable;

        Umtsnmm_CreatPmTcb(umtsnmm_cs_task_list[TCB_PM_TSKLST],pRxDataTmp,pRxDataTmp1,pPsc,antNum,accLen,pPdpAnt0,pPdpAnt1,
                pPdpAnt0His,pPdpAnt1His,pCellPath,PM_SEARCH_WIN_LEN,pPdp8xAnt0,pPdp8xAnt1,
                pPdpCom2Ant,accCount,thresholdFactor,angle,angTanTable,
                NULL,TCBIOBUFREFILL, umtsnmm_cs_tcb_arr[TCB_PM_TSKLST][i],i);
    }

    if(tskNum<TCB_PM_NUM)
    {
        Umtsnmm_CS_byPassMultTcb (umtsnmm_cs_tcb_arr[TCB_PM_TSKLST]+tskNum,  
            TCB_PM_NUM-tskNum, BYPASS_TCB);
    }

    Umtsnmm_CS_runTaskList(TCB_PM_TSKLST);
    return;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief create TCB task list for calculate frequency offset.

 Function CreatFreqEstTskList.

 @param    pRxData = the input data of antenna 0
 @param    pRxData = the input data of antenna 0
 @param    pCellSrchDesc = the cell search synchronization information
 @param    accLen = accumulation length
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CreatFreqEstTskList(U32* pRxData, U32* pRxData1, CellSrchDesc_t *pCellSrchDesc, U16 accLen)
{
	U8	tskNum;
	U8	tskIdx;
	U8   maxTskNumPerGrp;
	U8	pmGrpIdx;
	U8	grpNum;
	U32	i;
	S16*    pRxDataTmp;
	S16*    pRxDataTmp1;
	S8*	 pPsc;
	U8	antNum;

	U16	accCount;
	S16 *pfreqConjSym = NULL;
	U16 offset;

	
	pmGrpIdx = pCellSrchDesc->ThreadCtx->pmGrpIdx;
	maxTskNumPerGrp = pCellSrchDesc->pCellSrchPara->maxPmTskNum;
	grpNum = pCellSrchDesc->ThreadCtx->pmGrpNum;
	antNum = pCellSrchDesc->pCellSrchPara->rxAntNumber;
	accCount = pCellSrchDesc->ThreadCtx->SlotCnt;
	if(pmGrpIdx==(grpNum -1))
	{
		tskNum = pCellSrchDesc->ThreadCtx->pmTskNumLastGrp;
	}
	else
	{
		tskNum = maxTskNumPerGrp;
	}
	tskIdx = pmGrpIdx*maxTskNumPerGrp;

	for(i = 0; i < tskNum; i++)
	{
		pRxDataTmp = (S16*)(pRxData + pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[tskIdx+i].maxPos);
		pRxDataTmp1 =(S16*) (pRxData1 + pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[tskIdx+i].maxPos);
		pPsc = pCellSrchDesc->PscSeqSlot + i*CHIP_NUM_PER_SLOT*2;

		pfreqConjSym =& pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[tskIdx+i].freqConjSym[0][0];
		if(0 == accCount)
		{
			offset = 0;
		}
		else
		{		
			offset = 20*(accCount -1) + 18;
		}
              pfreqConjSym += offset;

            Umtsnmm_CreatFreqEstTcb(umtsnmm_cs_task_list[TCB_FREQ_TSKLST],pRxDataTmp,pRxDataTmp1,
                pPsc,antNum,pfreqConjSym,accCount, accLen,  NULL,TCBIOBUFREFILL, umtsnmm_cs_tcb_arr[TCB_FREQ_TSKLST][i],i);
	}	

    if(tskNum<TCB_FREQ_NUM)
    {
        Umtsnmm_CS_byPassMultTcb (umtsnmm_cs_tcb_arr[TCB_FREQ_TSKLST]+tskNum,  
            TCB_FREQ_NUM-tskNum, BYPASS_TCB);
    }

    Umtsnmm_CS_runTaskList(TCB_FREQ_TSKLST);

    return;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief create TCB task list for calculate RSCP.

 Function CreatRscpTskList.

 @param    pRxData = the input data of antenna 0
 @param    pRxData = the input data of antenna 0
 @param    pCellSrchDesc = the cell search synchronization information
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CreatRscpTskList(S16* pRxData,S16* pRxData1,CellSrchDesc_t *pCellSrchDesc)
{
    U8	tskNum;
    U8	tskIdx;
    U8   maxTskNumPerGrp;
    U8	pmGrpIdx;
    U8	grpNum;
    U8	antNum;
    U16	accLen;
    U16	accCount;
    S16*   pRxDataTmp;
    S16*   pRxDataTmp1;
    S8*	   pPsc;
    CellPathInfo* pCellPath;
    U32	i;
    S16  angle;
    S16* angTanTable;

    pmGrpIdx = pCellSrchDesc->ThreadCtx->pmGrpIdx;
    maxTskNumPerGrp = pCellSrchDesc->pCellSrchPara->maxPmTskNum;
    grpNum = pCellSrchDesc->ThreadCtx->pmGrpNum;
    antNum = pCellSrchDesc->pCellSrchPara->rxAntNumber;
    accLen = pCellSrchDesc->pCellSrchPara->rscpAccLen*SLOT_NUM_PER_FREAM;
    accCount = pCellSrchDesc->ThreadCtx->SlotCnt;
    if(pmGrpIdx==(grpNum -1))
    {
        tskNum = pCellSrchDesc->ThreadCtx->pmTskNumLastGrp;
    }
    else
    {
        tskNum = maxTskNumPerGrp;
    }
	
    tskIdx = pmGrpIdx*maxTskNumPerGrp;
    for( i = 0; i < tskNum; i++)
    {
        pRxDataTmp = pRxData + pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[tskIdx+i].WinBegin*2;
        pRxDataTmp1 = pRxData1 + pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[tskIdx+i].WinBegin*2;
        pPsc = pCellSrchDesc->PscSeqSlot + i*CHIP_NUM_PER_SLOT*2;	
        pCellPath =& pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[tskIdx+i];
        angle = pCellPath->angle;
        angTanTable = pCellSrchDesc->pAngTanTable;
        
        Umtsnmm_CreatRscpTcb(umtsnmm_cs_task_list[TCB_RSCP_TSKLST],pRxDataTmp,pRxDataTmp1,
                pPsc,antNum,pCellPath,accCount,accLen,angle,angTanTable,  
                NULL,TCBIOBUFREFILL, umtsnmm_cs_tcb_arr[TCB_RSCP_TSKLST][i],i);
	}

    if(tskNum<TCB_RSCP_NUM)
    {
        Umtsnmm_CS_byPassMultTcb (umtsnmm_cs_tcb_arr[TCB_RSCP_TSKLST]+tskNum,  
            TCB_RSCP_NUM-tskNum, BYPASS_TCB);
    }

    Umtsnmm_CS_runTaskList(TCB_RSCP_TSKLST);


    return;
	
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief create TCB task list for AGC.

 Function CreatAgcTskList.

 @param    pRxDataIn = the input data 
 @param    rawPow = the power of raw data
 @param    tasNum = the number of task
 @param    agcLen = the data length of calculate gain factor
 @param    Complete = the complete flag  
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CreatAgcTskList(S16* pRxDataIn,U32* rawPow,U32 tasNum,U32 agcLen,U32* Complete)
{
    U32 i;

    for( i = 0; i < tasNum; i++)
    {
            Umtsnmm_CreatAgcTcb(umtsnmm_cs_task_list[TCB_AGC_TSKLST],(pRxDataIn+i*agcLen*4*2),
                (rawPow+i),agcLen, tasNum,Complete,NULL,TCBIOBUFREFILL, umtsnmm_cs_tcb_arr[TCB_AGC_TSKLST][i],i);
    }

    if(tasNum<TCB_AGC_NUM)
    {
        Umtsnmm_CS_byPassMultTcb (umtsnmm_cs_tcb_arr[TCB_AGC_TSKLST]+tasNum,  
            TCB_AGC_NUM-tasNum, BYPASS_TCB);
    }

    Umtsnmm_CS_runTaskList(TCB_AGC_TSKLST);
     return;
}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief create TCB task list for down sampling.

 Function CreatDownSampleTskList.

 @param    pRxDataIn = input data 
 @param    pRxDataOut = output data 
 @param    agcFactor = agc factor
 @param    tasNum = the number of task
 @param    agcLen = the data length of need AGC processing
  @param    Complete = the complete flag  
 @param    Complete1 = another complete flag  
 @return    none.
 
 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CreatDownSampleTskList(S16* pRxDataIn,S16* pRxDataOut,U32 agcFactor,U32 tasNum,U32 agcLen,U32* Complete, U32* Complete1)
{
    U32 i;

    for( i = 0; i < tasNum; i++)
    {
        Umtsnmm_CreatDownSampTcb(umtsnmm_cs_task_list[TCB_DOWN_SAMPLE_TSKLST],(pRxDataIn+i*agcLen*4*2),
            (pRxDataOut+i*agcLen*2*2),agcFactor,  tasNum,agcLen,Complete, Complete1,NULL,
            TCBIOBUFREFILL, umtsnmm_cs_tcb_arr[TCB_DOWN_SAMPLE_TSKLST][i],i);
    }

    if(tasNum<TCB_DOWNSAMP_NUM)
    {
        Umtsnmm_CS_byPassMultTcb (umtsnmm_cs_tcb_arr[TCB_DOWN_SAMPLE_TSKLST]+tasNum,  
            TCB_DOWNSAMP_NUM-tasNum, BYPASS_TCB);
    }
    
    Umtsnmm_CS_runTaskList(TCB_DOWN_SAMPLE_TSKLST);
    
    return;       
}



/* generate 8 PSC */
volatile U8 DmaCounter = 0;

void AddDmaCounter(LPVOID pParam)
{
	DmaCounter ++;
	return;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief create TCB task list for generate downlink scrambling code.

 Function GenPSCTskList.

 @param    PSCGroupIdx = scrambling code group index
 @param    pPscBuf = output data 
 @param    pDdrBuffer =  DDR buffer to save 8 scrambling code.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void GenPSCTskList(U8 PSCGroupIdx, S8 *pPscBuf, S8 *pDdrBuffer)
{
	U8 tskNum = 8;
	U32 i,i2;
	U16 PscIdx = PSCGroupIdx*8;
	PMDMA_FDESC pMdma;

	for( i = 0; i < tskNum; i++)
	{   
		umtsnmm_CreatGenPSCTcb(umtsnmm_cs_task_list[TCB_GEN_PSC_TASKLST],PscIdx+i,pPscBuf,NULL,
			TCBIOBUFREFILL, umtsnmm_cs_tcb_arr[TCB_GEN_PSC_TASKLST][0]);
		
		Umtsnmm_CS_runTaskList(TCB_GEN_PSC_TASKLST);

		/*fransfer the psc to ddr*/
		for(i2=0;i2<15;i2++)
		{
			pMdma = MdmaAllocDesc(MDMA_HW_SYS,1);
			_ASSERT_PTR (pMdma );
			 MdmaSetCopy(pMdma );
			 
			MdmaSetData(pMdma, pPscBuf+i2*5120, 5120, pDdrBuffer+i*PSC_OFFSET_LEN+i2*5120,5120);
			MdmaRunEx(MDMA_HW_SYS,pMdma, AddDmaCounter, NULL);
		}

		while(1)
		{
			if(DmaCounter == 15)
			{
				DmaCounter = 0;
				break;
			}
		}
	}


    }





//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief trigger the Cell search thread to execute.

 Function ScheduleCellSrchThread.

 @param    none
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************

void ScheduleCellSrchThread(LPVOID pParam)
{
	MxScheduleThread(tCellSrchDesc.hCellSrchThread);
}

void calcPmTskId(CellSrchDesc_t* pCellSrchDesc,U8 *tskIdx, U8* tskNum)
{
	U8   maxTskNumPerGrp;
	U8	pmGrpIdx;
	U8	grpNum;
    	pmGrpIdx = pCellSrchDesc->ThreadCtx->pmGrpIdx;
	maxTskNumPerGrp = pCellSrchDesc->pCellSrchPara->maxPmTskNum;
	grpNum = pCellSrchDesc->ThreadCtx->pmGrpNum;
	if(pmGrpIdx==(grpNum -1))
	{
		*tskNum = pCellSrchDesc->ThreadCtx->pmTskNumLastGrp;
	}
	else
	{
		*tskNum = maxTskNumPerGrp;
	}
	
	*tskIdx = pmGrpIdx*maxTskNumPerGrp;
	return;
}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief calulate the pm group number and task number in each group,
	   each ceva process one task.

 Function PmTskGrpCalc.

 @param    pCellSrchDesc = pointer of cell searcher global variable
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void PmTskGrpCalc(CellSrchDesc_t *pCellSrchDesc)
{
	U8	cellNum;
	U8	grpNum;
	U8	tskNumLastGrp;
	U8	maxTskNumPerGrp;

	pCellSrchDesc->ThreadCtx->pmGrpIdx = 0;

	cellNum = pCellSrchDesc->cellGrpDetcInfo->CellNum;
	maxTskNumPerGrp = pCellSrchDesc->pCellSrchPara->maxPmTskNum;
	if(cellNum < maxTskNumPerGrp)
	{
		grpNum = 1;
		tskNumLastGrp = cellNum;
	}
	else
	{
		grpNum = cellNum/maxTskNumPerGrp;
		if(0!=(cellNum%maxTskNumPerGrp))
		{
			grpNum +=1;
			tskNumLastGrp = grpNum*maxTskNumPerGrp - cellNum;
		}
		else
		{
			tskNumLastGrp = maxTskNumPerGrp;
		}
	}
	pCellSrchDesc->ThreadCtx->pmGrpNum = grpNum;
	pCellSrchDesc->ThreadCtx->pmTskNumLastGrp = tskNumLastGrp;
	
	return ;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief DMA 2560+256 chips downlink Rx data from DDR to CRAM for PSCH correlation.

 Function DmaPschData.

 @param    pCellSrchDesc = pointer of cell searcher global variable
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void DmaPschData(CellSrchDesc_t *pCellSrchDesc)
{
	U8 PiPaFlag;
	U16 accLen;
	U32 *pDec;
	U32 *pSrc;
	
	PiPaFlag = (pCellSrchDesc->ThreadCtx->SlotCnt)&1;
	accLen = pCellSrchDesc->pCellSrchPara->pschAccLen;

	if(accLen*SLOT_NUM_PER_FREAM > pCellSrchDesc->ThreadCtx->SlotCnt)
	{
		// DMA ant 0 Rx data
		pDec =  (U32*)(pCellSrchDesc->RxAnt0Data+PiPaFlag*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);
		pSrc = (U32*)(pCellSrchDesc->RxAnt0DataDDR+(pCellSrchDesc->ThreadCtx->SlotCnt)*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);

	}
	else
	{
		// DMA ant 1 Rx data
		pDec =  (U32*)(pCellSrchDesc->RxAnt1Data+PiPaFlag*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);
		pSrc = (U32*)(pCellSrchDesc->RxAnt1DataDDR+(pCellSrchDesc->ThreadCtx->SlotCnt-accLen*SLOT_NUM_PER_FREAM)*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);
	}
	
	pCellSrchDesc->pMdma = MdmaAllocDesc(MDMA_HW_SYS,1);
	_ASSERT_PTR (pCellSrchDesc->pMdma );
	MdmaSetCopy(pCellSrchDesc->pMdma );

	MdmaSetData(pCellSrchDesc->pMdma,  pSrc, (CHIP_NUM_PER_SLOT+256)*SAMPLE_RATE*2*sizeof(S16), pDec,
	             (CHIP_NUM_PER_SLOT+256)*SAMPLE_RATE*2*sizeof(S16));
	MdmaRunEx(MDMA_HW_SYS,pCellSrchDesc->pMdma, ScheduleCellSrchThread, NULL);
	
	return;	
}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief DMA 2560 chips downlink Rx data from DDR to CRAM for SSCH correlation.

 Function DmaSschData.

 @param    pCellSrchDesc = pointer of cell searcher global variable
 @return	none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void DmaSschData(CellSrchDesc_t *pCellSrchDesc)
{
	U8 PiPaFlag;
	U8	antFlag;
	U8   antNum;
	U32 *pDec;
	U32 *pSrc;
	
	PiPaFlag = (pCellSrchDesc->ThreadCtx->SlotCnt)&1;
	antFlag = pCellSrchDesc->ThreadCtx->antFlag;
	antNum = pCellSrchDesc->pCellSrchPara->rxAntNumber;
	if(0==antFlag)
	{
		// DMA ant 0 Rx data
		pDec =  (U32*)(pCellSrchDesc->RxAnt0Data+PiPaFlag*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);
		pSrc = (U32*)(pCellSrchDesc->RxAnt0DataDDR+(pCellSrchDesc->ThreadCtx->SlotCnt)*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);

		pCellSrchDesc->pMdma =MdmaAllocDesc(MDMA_HW_SYS,1);
       	 _ASSERT_PTR (pCellSrchDesc->pMdma );
		 MdmaSetCopy(pCellSrchDesc->pMdma );
	 
       	 MdmaSetData(pCellSrchDesc->pMdma,  pSrc, (CHIP_NUM_PER_SLOT+256)*SAMPLE_RATE*2*sizeof(S16), pDec,
                         (CHIP_NUM_PER_SLOT+256)*SAMPLE_RATE*2*sizeof(S16));
		if(2==antNum)
		{
			pCellSrchDesc->ThreadCtx->antFlag = 1;
		    MdmaRunEx(MDMA_HW_SYS,pCellSrchDesc->pMdma, (CB_FUN_TYPE)DmaSschData, pCellSrchDesc);
		}
		else
		{
			MdmaRunEx(MDMA_HW_SYS,pCellSrchDesc->pMdma, ScheduleCellSrchThread, NULL);
		}
	}
	else
	{
		// DMA ant 1 Rx data
		pDec =  (U32*)(pCellSrchDesc->RxAnt1Data+PiPaFlag*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);
		pSrc = (U32*)(pCellSrchDesc->RxAnt1DataDDR+(pCellSrchDesc->ThreadCtx->SlotCnt)*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);

		pCellSrchDesc->pMdma = MdmaAllocDesc(MDMA_HW_SYS,1);
       	 _ASSERT_PTR (pCellSrchDesc->pMdma );
		 MdmaSetCopy(pCellSrchDesc->pMdma );
	 
       	 MdmaSetData(pCellSrchDesc->pMdma,  pSrc, CHIP_NUM_PER_SLOT*SAMPLE_RATE*2*sizeof(S16), pDec,
                         CHIP_NUM_PER_SLOT*SAMPLE_RATE*2*sizeof(S16));
		 pCellSrchDesc->ThreadCtx->antFlag = 0;
		 MdmaRunEx(MDMA_HW_SYS,pCellSrchDesc->pMdma, ScheduleCellSrchThread, NULL);
	}
		
}

void setDmaDoneFlag()
{
	gDmaDoneFlag =1;
	return;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief DMA scrambling codes and Rx data from DDR to CRAM.

 Function DmaPcpichData.

 @param    pCellSrchDesc = pointer of cell searcher global variable
 @param    pscGrpNo = cell primary scrambling code group
 @param    slotNum = slot number
 @return	none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void DmaPcpichData(CellSrchDesc_t *pCellSrchDesc, U8 pscGrpNo,U8 slotNum)
{
	U8 PiPaFlag;
	U32 *pDec;
	U32 *pSrc;
	U32 loop1;
	U32	groupOffset;
	U32 position;
    U8  pscSlot;
	
	PMDMA_FDESC pMdmaNext;

	groupOffset = pscGrpNo*PSCR_NUM_PER_CODEGRP*SLOT_NUM_PER_FREAM*CHIP_NUM_PER_SLOT*2;
	position = pCellSrchDesc->pschPeakPos->Position[pCellSrchDesc->ThreadCtx->PkIdx];

	for(loop1=0;loop1<PSCR_NUM_PER_CODEGRP;loop1++)
	{
		pscSlot = (pCellSrchDesc->ThreadCtx->SlotCnt+slotNum)%15;
		pDec =(U32*)(pCellSrchDesc->PscSeqSlot+loop1*CHIP_NUM_PER_SLOT*2 );
		pSrc =(U32*)(pCellSrchDesc->PScrSeqFrame+groupOffset+loop1*SLOT_NUM_PER_FREAM*CHIP_NUM_PER_SLOT*2
				+pscSlot*CHIP_NUM_PER_SLOT*2);

		gDmaDoneFlag = 0;
		pMdmaNext = MdmaAllocDesc(MDMA_HW_SYS,1);
		_ASSERT_PTR (pMdmaNext);

		MdmaSetCopy(pMdmaNext );
		MdmaSetData(pMdmaNext ,  pSrc, CHIP_NUM_PER_SLOT*2, pDec,CHIP_NUM_PER_SLOT*2);
		MdmaRunEx(MDMA_HW_SYS,pMdmaNext , (void (*)(LPVOID))setDmaDoneFlag, NULL);
		while(0 == gDmaDoneFlag);
		
	}

	if(1==pCellSrchDesc->pCellSrchPara->rxAntNumber)
	{
		PiPaFlag = (pCellSrchDesc->ThreadCtx->SlotCnt)&1;
		pDec =  (U32*)(pCellSrchDesc->RxAnt0Data+PiPaFlag*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);
		pSrc = (U32*)(pCellSrchDesc->RxAnt0DataDDR+(pCellSrchDesc->ThreadCtx->SlotCnt)*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2+position*2);
		pCellSrchDesc->ThreadCtx->antFlag = 0;
	}
	else
	{
		if(pCellSrchDesc->pschAnt0Pow[position]>pCellSrchDesc->pschAnt1Pow[position])
		{
			PiPaFlag = (pCellSrchDesc->ThreadCtx->SlotCnt)&1;
			pDec =  (U32*)(pCellSrchDesc->RxAnt0Data+PiPaFlag*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);
			pSrc = (U32*)(pCellSrchDesc->RxAnt0DataDDR+(pCellSrchDesc->ThreadCtx->SlotCnt)*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2+position*2);
			pCellSrchDesc->ThreadCtx->antFlag = 0;

		}
		else
		{
			PiPaFlag = (pCellSrchDesc->ThreadCtx->SlotCnt)&1;
			pDec =  (U32*)(pCellSrchDesc->RxAnt1Data+PiPaFlag*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);
			pSrc = (U32*)(pCellSrchDesc->RxAnt1DataDDR+(pCellSrchDesc->ThreadCtx->SlotCnt)*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2+position*2);
			pCellSrchDesc->ThreadCtx->antFlag = 1;

		}
	}
	
	pMdmaNext = MdmaAllocDesc(MDMA_HW_SYS,1);
	_ASSERT_PTR (pMdmaNext);
	MdmaSetCopy(pMdmaNext );			 
	MdmaSetData(pMdmaNext ,  pSrc, CHIP_NUM_PER_SLOT*SAMPLE_RATE*2*sizeof(S16), pDec,
	                 CHIP_NUM_PER_SLOT*SAMPLE_RATE*2*sizeof(S16));
	MdmaRunEx(MDMA_HW_SYS,pMdmaNext , ScheduleCellSrchThread, NULL);
			
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief DMA  downlink Rx data from DDR to CRAM for calculate  RSCP.

 Function DmaPmRscpData.

 @param    pCellSrchDesc = pointer of cell searcher global variable
 @return	none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void  DmaPmRscpData(CellSrchDesc_t *pCellSrchDesc)
{
	PMDMA_FDESC pMdmaNext;
	U8 PiPaFlag;
	U32 *pDec;
	U32 *pSrc;
	U8	tskNum;
	U8	tskIdx;
	U8   maxTskNumPerGrp;
	U8	pmGrpIdx;
	U8	grpNum;
	U8	i;
	U16	pscIdx;
    U8   pscSlot;

	pmGrpIdx = pCellSrchDesc->ThreadCtx->pmGrpIdx;
	maxTskNumPerGrp = pCellSrchDesc->pCellSrchPara->maxPmTskNum;
	grpNum = pCellSrchDesc->ThreadCtx->pmGrpNum;
	if(pmGrpIdx==(grpNum -1))
	{
		tskNum = pCellSrchDesc->ThreadCtx->pmTskNumLastGrp;
	}
	else
	{
		tskNum = maxTskNumPerGrp;
	}
	
	tskIdx = pmGrpIdx*maxTskNumPerGrp;
	
	if(1==pCellSrchDesc->pCellSrchPara->rxAntNumber)
	{
		pCellSrchDesc->pMdma = MdmaAllocDesc(MDMA_HW_SYS,tskNum+1);
	}
	else
	{
		pCellSrchDesc->pMdma = MdmaAllocDesc(MDMA_HW_SYS,tskNum+2);
	}
	
	_ASSERT_PTR (pCellSrchDesc->pMdma );
	pMdmaNext = pCellSrchDesc->pMdma;
	
	for( i = 0; i < tskNum; i++)
	{
		pscIdx= pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[tskIdx+i].PIndx;
        pscSlot = pCellSrchDesc->cellGrpDetcInfo->CellPathInfo[tskIdx+i].SlotNum;
        pscSlot = (pscSlot+pCellSrchDesc->ThreadCtx->SlotCnt)%15;
		pDec =(U32*)(pCellSrchDesc->PscSeqSlot + i*CHIP_NUM_PER_SLOT*2);		
		pSrc =(U32*)(pCellSrchDesc->PScrSeqFrame+pscIdx*SLOT_NUM_PER_FREAM*CHIP_NUM_PER_SLOT*2
				+pscSlot*CHIP_NUM_PER_SLOT*2);
		MdmaSetCopy(pMdmaNext );
		MdmaSetData(pMdmaNext ,  pSrc, CHIP_NUM_PER_SLOT*2, pDec,CHIP_NUM_PER_SLOT*2);
		pMdmaNext = pMdmaNext->Next;


	}
	PiPaFlag = (pCellSrchDesc->ThreadCtx->SlotCnt)&1;
	pDec =  (U32*)(pCellSrchDesc->RxAnt0Data+PiPaFlag*((2*CHIP_NUM_PER_SLOT+PM_SEARCH_WIN_LEN)*SAMPLE_RATE*2));
	pSrc = (U32*)(pCellSrchDesc->RxAnt0DataDDR+(pCellSrchDesc->ThreadCtx->SlotCnt)*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);	
	MdmaSetCopy(pMdmaNext );
	MdmaSetData(pMdmaNext ,  pSrc, (2*CHIP_NUM_PER_SLOT+PM_SEARCH_WIN_LEN)*SAMPLE_RATE*4, 
				pDec,(2*CHIP_NUM_PER_SLOT+PM_SEARCH_WIN_LEN)*SAMPLE_RATE*4);
	pMdmaNext = pMdmaNext->Next;
	
	if(2==pCellSrchDesc->pCellSrchPara->rxAntNumber)
	{
		PiPaFlag = (pCellSrchDesc->ThreadCtx->SlotCnt)&1;
		pDec =  (U32*)(pCellSrchDesc->RxAnt1Data+PiPaFlag*((2*CHIP_NUM_PER_SLOT+PM_SEARCH_WIN_LEN)*SAMPLE_RATE*2));
		pSrc = (U32*)(pCellSrchDesc->RxAnt1DataDDR+(pCellSrchDesc->ThreadCtx->SlotCnt)*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);	
		MdmaSetCopy(pMdmaNext );
		MdmaSetData(pMdmaNext ,  pSrc, (2*CHIP_NUM_PER_SLOT+PM_SEARCH_WIN_LEN)*SAMPLE_RATE*4, 
					pDec,(2*CHIP_NUM_PER_SLOT+PM_SEARCH_WIN_LEN)*SAMPLE_RATE*4);
		pMdmaNext = pMdmaNext->Next;		
	}

    MdmaRunEx(MDMA_HW_SYS,pCellSrchDesc->pMdma , ScheduleCellSrchThread, NULL);

	return;
}






//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief the function of cell search thread.

 Function CellSrchThreadMain.

 @param    none.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CellSrchThreadMain(LPVOID pParam)
{
    U8 PiPaFlag;
    U32 *pRxData;
    U32	*pRxData1;
    PMDMA_FDESC pMdma;
    U32 loop1;
    U16 accLen;
    U16	slotCnt;
    U8	antNum;
    U32	threshold;
    U8	pscGrpNo;
    U8  slotNum;
    U32 meanPow;
    U8 CellIdx;
    U8 i2;
    U8 PscGrpInit[DL_CODEGRP_NUM];
    U32 groupOffset;
    NWL_CommInfor *pNwlInfo = NULL;
    U8 tskIdx;
    U8 tskNum;
    U16 Pscidx; 
    U8 idx;
    if(PSCH_STEP==tCellSrchDesc.ThreadCtx->ProFlag)
    {
        accLen = tCellSrchDesc.pCellSrchPara->pschAccLen*SLOT_NUM_PER_FREAM;
        slotCnt = tCellSrchDesc.ThreadCtx->SlotCnt;
        antNum = tCellSrchDesc.pCellSrchPara->rxAntNumber;
        PiPaFlag = slotCnt&1;
        if(accLen>slotCnt)
        {
        	// process ant 0  psch correlation
        	pRxData =  (U32*)(tCellSrchDesc.RxAnt0Data+PiPaFlag*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);
        	CreatPschTskList(&tCellSrchDesc ,(S16*)pRxData ,tCellSrchDesc.pschAnt0Pow,slotCnt);
        }
    		
        tCellSrchDesc.ThreadCtx->SlotCnt++;
    	
    	if(antNum*accLen==tCellSrchDesc.ThreadCtx->SlotCnt)
        {
            meanPow = PscMeanCalc(tCellSrchDesc.pschAnt0Pow,tCellSrchDesc.pschAnt1Pow,antNum);			
            threshold =meanPow*tCellSrchDesc.pCellSrchPara->pschThsdFactor;
            tCellSrchDesc.ThreadCtx->PkIdx = 0;
            PschPeakDetc(tCellSrchDesc.pschAnt0Pow,tCellSrchDesc.pschAnt1Pow,threshold,antNum,tCellSrchDesc.pschPeakPos);

			if(0==tCellSrchDesc.pschPeakPos->PkNum)
            {
                tCellSrchDesc.cellSrchRpt->tCellSearchInd.nr_cellFoundList = 0;
                CellSearchRpt((U32*)tCellSrchDesc.cellSrchRpt, sizeof(CellSrchRpt_t));
            	NWL_GenerateAPIMsg(NWL_CELL_SEARCH_CNF,NWL_ERR_OK,API_TYPE_IND);
				PscCnt = 0;
				tCellSrchDesc.ThreadCtx->ProFlag = 0xff;
                gCellSearchBusyFlag = 0;
				return;
            }
            else
            {
                tCellSrchDesc.ThreadCtx->ProFlag = SSCH_STEP;
			    tCellSrchDesc.ThreadCtx->SlotCnt =0;
			    DmaSschData(&tCellSrchDesc);
            }

		}
		else
		{
			DmaPschData(&tCellSrchDesc);
		}
	}

	if(SSCH_STEP==tCellSrchDesc.ThreadCtx->ProFlag)
	{
		PiPaFlag = tCellSrchDesc.ThreadCtx->SlotCnt&1;
		pRxData =  (U32*)(tCellSrchDesc.RxAnt0Data+PiPaFlag*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);
		pRxData1 =  (U32*)(tCellSrchDesc.RxAnt1Data+PiPaFlag*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);
		accLen = tCellSrchDesc.pCellSrchPara->sschAccLen*SLOT_NUM_PER_FREAM;
		CreatSschTskList(&tCellSrchDesc,(S16*)pRxData,(S16*)pRxData1,tCellSrchDesc.Zseq,tCellSrchDesc.HadamaSeq16,tCellSrchDesc.GrpSlotMapTbl,
		tCellSrchDesc.pschPeakInfo,tCellSrchDesc.ThreadCtx->SlotCnt,tCellSrchDesc.pschPeakPos);

		tCellSrchDesc.ThreadCtx->SlotCnt++;
		
		if(accLen==tCellSrchDesc.ThreadCtx->SlotCnt)
		{
		   slotNum = tCellSrchDesc.pschPeakInfo->PsynPk.SlotNum[tCellSrchDesc.ThreadCtx->PkIdx];
            while(0xff == slotNum)
            {
                tCellSrchDesc.pschPeakInfo->PsynPk.PIndex[tCellSrchDesc.ThreadCtx->PkIdx] = 0xffff;
                tCellSrchDesc.ThreadCtx->PkIdx++;
                if(tCellSrchDesc.ThreadCtx->PkIdx==tCellSrchDesc.pschPeakPos->PkNum)
                {
					tCellSrchDesc.cellSrchRpt->tCellSearchInd.nr_cellFoundList = 0;
					CellSearchRpt((U32*)tCellSrchDesc.cellSrchRpt, sizeof(CellSrchRpt_t));
					NWL_GenerateAPIMsg(NWL_CELL_SEARCH_CNF,NWL_ERR_OK,API_TYPE_IND);
					PscCnt = 0;
					tCellSrchDesc.ThreadCtx->ProFlag = 0xff;
					gCellSearchBusyFlag = 0;
					return;
                }
                slotNum = tCellSrchDesc.pschPeakInfo->PsynPk.SlotNum[tCellSrchDesc.ThreadCtx->PkIdx];
            }
            tCellSrchDesc.ThreadCtx->SlotCnt =0;
            tCellSrchDesc.ThreadCtx->ProFlag = PCPICH_STEP;

			memset(PscGrpInit,0,DL_CODEGRP_NUM);
			for(loop1 = 0; loop1 <tCellSrchDesc.pschPeakPos->PkNum; loop1++)
			{
				pscGrpNo = tCellSrchDesc.pschPeakInfo->PsynPk.GrpNum[loop1];
				if(0==PscGrpInit[pscGrpNo])
				{
					PscGrpInit[pscGrpNo] = 1;	
					groupOffset = pscGrpNo*PSCR_NUM_PER_CODEGRP*SLOT_NUM_PER_FREAM*CHIP_NUM_PER_SLOT*2;
					GenPSCTskList(pscGrpNo,tCellSrchDesc.PScTemp,(tCellSrchDesc.PScrSeqFrame+groupOffset));	
				}
			}
			pscGrpNo = tCellSrchDesc.pschPeakInfo->PsynPk.GrpNum[tCellSrchDesc.ThreadCtx->PkIdx];
            DmaPcpichData(&tCellSrchDesc,pscGrpNo,slotNum);//if there is 512 scrambling code

		}
		else
		{
			DmaSschData(&tCellSrchDesc);
		}
	}

	if(PCPICH_STEP==tCellSrchDesc.ThreadCtx->ProFlag)
	{
		antNum = tCellSrchDesc.pCellSrchPara->rxAntNumber;
		PiPaFlag = tCellSrchDesc.ThreadCtx->SlotCnt&1;
		accLen = tCellSrchDesc.pCellSrchPara->pcpichAccLen*SLOT_NUM_PER_FREAM;
		if(0==tCellSrchDesc.ThreadCtx->antFlag)
		{
			pRxData=  (U32*)(tCellSrchDesc.RxAnt0Data+PiPaFlag*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);
		}
		else
		{
			pRxData=  (U32*)(tCellSrchDesc.RxAnt1Data+PiPaFlag*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);
		}		
		CreatPcpichTskList((S16*)pRxData,&tCellSrchDesc);

		tCellSrchDesc.ThreadCtx->SlotCnt++;
		if(accLen==tCellSrchDesc.ThreadCtx->SlotCnt)
		{
           
			CellPscDetec( tCellSrchDesc.pCellSrchPara->pcpichConherentLen, tCellSrchDesc.ThreadCtx->PkIdx,
			tCellSrchDesc.pCellSrchPara->pcpichThsdFactor,tCellSrchDesc.pschPeakInfo );

			// if detected a valid primary scrambling code, configure path monitor parameter
			if(0xffff!=tCellSrchDesc.pschPeakInfo->PsynPk.PIndex[tCellSrchDesc.ThreadCtx->PkIdx])
			{
				PmParaCala(tCellSrchDesc.cellGrpDetcInfo,tCellSrchDesc.pschPeakPos,tCellSrchDesc.pschPeakInfo,tCellSrchDesc.ThreadCtx->PkIdx);
			}
			tCellSrchDesc.ThreadCtx->SlotCnt = 0;
			tCellSrchDesc.ThreadCtx->PkIdx++;
		}
		if(tCellSrchDesc.ThreadCtx->PkIdx==tCellSrchDesc.pschPeakPos->PkNum)
		{
            if(0 != (tCellSrchDesc.cellGrpDetcInfo->CellNum))
            {
                tCellSrchDesc.ThreadCtx->ProFlag = FREQ_ESTI_STEP;
        		PmTskGrpCalc(&tCellSrchDesc);
        		DmaPmRscpData(&tCellSrchDesc);
            }
            else
            {
				tCellSrchDesc.cellSrchRpt->tCellSearchInd.nr_cellFoundList = 0;
				CellSearchRpt((U32*)tCellSrchDesc.cellSrchRpt, sizeof(CellSrchRpt_t));
				NWL_GenerateAPIMsg(NWL_CELL_SEARCH_CNF,NWL_ERR_OK,API_TYPE_IND);
				PscCnt = 0;
				tCellSrchDesc.ThreadCtx->ProFlag = 0xff;
				gCellSearchBusyFlag = 0;
				return;
            }
		}
		else
		{
		    slotNum = tCellSrchDesc.pschPeakInfo->PsynPk.SlotNum[tCellSrchDesc.ThreadCtx->PkIdx];
		    while(0xff == slotNum)
		    {
		        tCellSrchDesc.pschPeakInfo->PsynPk.PIndex[tCellSrchDesc.ThreadCtx->PkIdx] = 0xffff;
		        tCellSrchDesc.ThreadCtx->PkIdx++;

		        if(tCellSrchDesc.ThreadCtx->PkIdx==tCellSrchDesc.pschPeakPos->PkNum)
		        {
		            if(0 != (tCellSrchDesc.cellGrpDetcInfo->CellNum))
		            {
		                tCellSrchDesc.ThreadCtx->ProFlag = FREQ_ESTI_STEP;
		        		PmTskGrpCalc(&tCellSrchDesc);
		        		DmaPmRscpData(&tCellSrchDesc);
		            }
		            else
		            {
						tCellSrchDesc.cellSrchRpt->tCellSearchInd.nr_cellFoundList = 0;
						CellSearchRpt((U32*)tCellSrchDesc.cellSrchRpt, sizeof(CellSrchRpt_t));
						NWL_GenerateAPIMsg(NWL_CELL_SEARCH_CNF,NWL_ERR_OK,API_TYPE_IND);
						PscCnt = 0;
						tCellSrchDesc.ThreadCtx->ProFlag = 0xff;
						gCellSearchBusyFlag = 0;
						return;
		            }                    
		        }
		        slotNum = tCellSrchDesc.pschPeakInfo->PsynPk.SlotNum[tCellSrchDesc.ThreadCtx->PkIdx];

		    }
		    
			pscGrpNo = tCellSrchDesc.pschPeakInfo->PsynPk.GrpNum[tCellSrchDesc.ThreadCtx->PkIdx]; 
		   
		    DmaPcpichData(&tCellSrchDesc,pscGrpNo,slotNum);//if there is 512 scrambling code

		}
	
	}
    
    if(FREQ_ESTI_STEP==tCellSrchDesc.ThreadCtx->ProFlag)
	{
		PiPaFlag = tCellSrchDesc.ThreadCtx->SlotCnt&1;	
		pRxData = (U32*)(tCellSrchDesc.RxAnt0Data+PiPaFlag*((2*CHIP_NUM_PER_SLOT+PM_SEARCH_WIN_LEN)*SAMPLE_RATE*2));
		pRxData1 = (U32*)(tCellSrchDesc.RxAnt1Data+PiPaFlag*((2*CHIP_NUM_PER_SLOT+PM_SEARCH_WIN_LEN)*SAMPLE_RATE*2));
		accLen =SLOT_NUM_PER_FREAM;// use fixed 15 slot for frequency init estimation 

            CreatFreqEstTskList(pRxData,pRxData1,&tCellSrchDesc,accLen);

		tCellSrchDesc.ThreadCtx->SlotCnt ++;
		if(accLen==tCellSrchDesc.ThreadCtx->SlotCnt )
		{
			calcPmTskId(&tCellSrchDesc,&tskIdx, &tskNum);
			GetFreAngle(tCellSrchDesc.cellGrpDetcInfo,tskIdx, tskNum);

			tCellSrchDesc.ThreadCtx->pmGrpIdx++;
			tCellSrchDesc.ThreadCtx->SlotCnt = 0;
			if(tCellSrchDesc.ThreadCtx->pmGrpIdx==tCellSrchDesc.ThreadCtx->pmGrpNum)
			{
                tCellSrchDesc.ThreadCtx->ProFlag = PM_STEP;
                tCellSrchDesc.ThreadCtx->pmGrpIdx = 0;
                               
			}
			DmaPmRscpData(&tCellSrchDesc);
		}
		else
		{
			DmaPmRscpData(&tCellSrchDesc);
		}
		
	}

	if(PM_STEP==tCellSrchDesc.ThreadCtx->ProFlag)
	{
		PiPaFlag = tCellSrchDesc.ThreadCtx->SlotCnt&1;	
		pRxData = (U32*)(tCellSrchDesc.RxAnt0Data+PiPaFlag*((2*CHIP_NUM_PER_SLOT+PM_SEARCH_WIN_LEN)*SAMPLE_RATE*2));
		pRxData1 = (U32*)(tCellSrchDesc.RxAnt1Data+PiPaFlag*((2*CHIP_NUM_PER_SLOT+PM_SEARCH_WIN_LEN)*SAMPLE_RATE*2));
		accLen = tCellSrchDesc.pCellSrchPara->pmAccLen;
		CreatPmTskList(pRxData,pRxData1,&tCellSrchDesc);

		tCellSrchDesc.ThreadCtx->SlotCnt ++;
		if(accLen==tCellSrchDesc.ThreadCtx->SlotCnt )
		{
			tCellSrchDesc.ThreadCtx->pmGrpIdx++;
			tCellSrchDesc.ThreadCtx->SlotCnt = 0;
			if(tCellSrchDesc.ThreadCtx->pmGrpIdx==tCellSrchDesc.ThreadCtx->pmGrpNum)
			{
                tCellSrchDesc.ThreadCtx->ProFlag = RSCP_STEP;
                tCellSrchDesc.ThreadCtx->pmGrpIdx = 0;

                for(CellIdx=0;CellIdx<(tCellSrchDesc.cellGrpDetcInfo->CellNum);CellIdx++)
                {
                    tCellSrchDesc.cellGrpDetcInfo->CellPathInfo[CellIdx].FingNum=0;
                    for(idx=0;idx<PM_MAX_FINGER_NUM;idx++)
                    {
                       if(0!= tCellSrchDesc.cellGrpDetcInfo->CellPathInfo[CellIdx].Path[idx].PathPower)
                        {
                             tCellSrchDesc.cellGrpDetcInfo->CellPathInfo[CellIdx].FingNum++;
                        }
                    }

					// this line code should be removed if not in test case
					tCellSrchDesc.cellGrpDetcInfo->CellPathInfo[CellIdx].FingNum = 1;
					
                    Pscidx = tCellSrchDesc.cellGrpDetcInfo->CellPathInfo[CellIdx].PIndx; 
                    pNwlInfo = pNwlCommInfo+PscCnt;
                    pNwlInfo->pPscBuf = gUMTSNwlPscBuf+PscCnt*PSC_OFFSET_LEN;
                    pNwlInfo->thresholdFactor = tCellSrchDesc.pCellSrchPara->pmThsdFactor;
                                       
					for(i2=0;i2<15;i2++)
					{
						pMdma = MdmaAllocDesc(MDMA_HW_SYS,1);
						_ASSERT_PTR (pMdma );
						 MdmaSetCopy(pMdma );
						 
						//MdmaSetData(pMdma, (tCellSrchDesc.PScrSeqFrame+(Pscidx%8)*PSC_OFFSET_LEN+i2*5120), 
							//        5120, (pNwlInfo->pPscBuf+i2*5120),5120);
					
						MdmaSetData(pMdma, (tCellSrchDesc.PScrSeqFrame+Pscidx*PSC_OFFSET_LEN+i2*5120), 
								5120, (pNwlInfo->pPscBuf+i2*5120),5120);
						MdmaRunEx(MDMA_HW_SYS,pMdma, AddDmaCounter, NULL);
					}
					
					while(1)
            		{
            			if(DmaCounter == 15)
            			{
            				DmaCounter = 0;
            				break;
            			}
            		}
					

					pNwlCommInfo[PscCnt].SetFlag = 1;

					SetSharePara(PscCnt, &(tCellSrchDesc.cellGrpDetcInfo->CellPathInfo[CellIdx]),
					tCellSrchDesc.cellGrpDetcInfo->CellPathInfo[CellIdx].angle,pNwlCommInfo);
					PscCnt ++;
                }
			}
			DmaPmRscpData(&tCellSrchDesc);
		}
		else
		{
			DmaPmRscpData(&tCellSrchDesc);
		}
		
	}

	if(RSCP_STEP==tCellSrchDesc.ThreadCtx->ProFlag)
	{

		PiPaFlag = tCellSrchDesc.ThreadCtx->SlotCnt&1;	
		pRxData = (U32*)(tCellSrchDesc.RxAnt0Data+PiPaFlag*((2*CHIP_NUM_PER_SLOT+PM_SEARCH_WIN_LEN)*SAMPLE_RATE*2));
		pRxData1 = (U32*)(tCellSrchDesc.RxAnt1Data+PiPaFlag*((2*CHIP_NUM_PER_SLOT+PM_SEARCH_WIN_LEN)*SAMPLE_RATE*2));
		accLen = tCellSrchDesc.pCellSrchPara->rscpAccLen*SLOT_NUM_PER_FREAM;
		
		CreatRscpTskList((S16*)pRxData,(S16*)pRxData1,&tCellSrchDesc);
		
		tCellSrchDesc.ThreadCtx->SlotCnt ++;

		if(accLen==tCellSrchDesc.ThreadCtx->SlotCnt )
		{
			tCellSrchDesc.ThreadCtx->pmGrpIdx++;
			tCellSrchDesc.ThreadCtx->SlotCnt = 0;
			if(tCellSrchDesc.ThreadCtx->pmGrpIdx==tCellSrchDesc.ThreadCtx->pmGrpNum)
			{
				CellSrhFillRpt(&tCellSrchDesc);
				CellSearchRpt((U32*)tCellSrchDesc.cellSrchRpt, sizeof(CellSrchRpt_t));
				NWL_GenerateAPIMsg(NWL_CELL_SEARCH_CNF,NWL_ERR_OK,API_TYPE_IND);
				PscCnt = 0;
				tCellSrchDesc.ThreadCtx->ProFlag = 0xff;
				gCellSearchBusyFlag = 0;
				return;
			}
			DmaPmRscpData(&tCellSrchDesc);
		}
		else
		{
			DmaPmRscpData(&tCellSrchDesc);
		}

	}
      if(0xff==tCellSrchDesc.ThreadCtx->ProFlag)
       {       		
			gCellSearchBusyFlag = 0;
            return;
       }

    
    	//MLogTask(PID_CELLLSN_THREAD, RESOURCE_LARM, t, GET_TICKS);

}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief the interface function between cell search and RF control module.
	    when  DDR has stored 130 frames downlink Rx data, the RF control module 
	    will call this function to trigger cell search module processing.

 Function CellSrchTrigger.

 @param    PscIdx = primary scrambling code index.(0--511, and 0xffff for  search all scrambling code).
 @param    MeasPeriod = measurement period.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CellSrchTrigger(U16 PscIdx ,U8 MeasPeriod )
{
    U8 PiPaFlag;
    U32 *pDec;
    U32 *pSrc;

    /* search all 512 psc */
    if(0xffff == PscIdx)
    {
        tCellSrchDesc.SetPscFlag = 0;
        tCellSrchDesc.SetPscValue = 0xffff;
    }
    else
    {
        tCellSrchDesc.SetPscFlag = 1;
        tCellSrchDesc.SetPscValue = PscIdx;
    }
    	

    tCellSrchDesc.ThreadCtx->SlotCnt = 0;
    tCellSrchDesc.ThreadCtx->antFlag = 0;
    tCellSrchDesc.ThreadCtx ->ProFlag = PSCH_STEP;

    PiPaFlag = (tCellSrchDesc.ThreadCtx->SlotCnt)&1;
    pDec =  (U32*)(tCellSrchDesc.RxAnt0Data+PiPaFlag*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);
    pSrc = (U32*)(tCellSrchDesc.RxAnt0DataDDR+(tCellSrchDesc.ThreadCtx->SlotCnt)*CHIP_NUM_PER_SLOT*SAMPLE_RATE*2);

    tCellSrchDesc.pMdma = MdmaAllocDesc(MDMA_HW_SYS,1);
        _ASSERT_PTR (tCellSrchDesc.pMdma );
     MdmaSetCopy(tCellSrchDesc.pMdma );
    MdmaSetData(tCellSrchDesc.pMdma ,  pSrc, (CHIP_NUM_PER_SLOT+256)*SAMPLE_RATE*2*sizeof(S16), pDec,
                     (CHIP_NUM_PER_SLOT+256)*SAMPLE_RATE*2*sizeof(S16));
    MdmaRunEx(MDMA_HW_SYS,tCellSrchDesc.pMdma , ScheduleCellSrchThread, NULL);

}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief initialize cell search thread.

 Function UMTS_Nwl_cell_search_thread_init.

 @param    none.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
MXRC UMTS_Nwl_cell_search_thread_init(void)
{
    MXRC rc = MX_OK;
    rc |= MxCreateThreadEx(0, (TENTRY)CellSrchThreadMain, NULL, NULL, 0,
                           &(tCellSrchDesc.hCellSrchThread));
    
    if (FAILED(rc))
    {        
        MxRemoveThread(tCellSrchDesc.hCellSrchThread);
        tCellSrchDesc.hCellSrchThread = HNULL;
    }
    
    return rc;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief kill cell search thread.

 Function UMTS_Nwl_cell_search_thread_kill.

 @param    none.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void UMTS_Nwl_cell_search_thread_kill()
{    
    
    if (HANDLE_NOT_NULL(tCellSrchDesc.hCellSrchThread))
    {
        MxRemoveThread(tCellSrchDesc.hCellSrchThread);
    }        
}

//S16 *AntData2x;
void DmaCompleteFlag(U32* pAddr)
{
	*pAddr = AGC_DMA_DONE;
	return;
}
//agc 0918
void DmaAGCData(	U32 *pDec,U32 *pSrc,U32 Len, U32* Complete)
{
    PMDMA_FDESC pMdma;
    
    pMdma =  MdmaAllocDesc(MDMA_HW_SYS,1);
    _ASSERT_PTR (pMdma );
    MdmaSetCopy(pMdma);
    MdmaSetData(pMdma,  pSrc, Len, pDec,Len);   
    MdmaRunEx(MDMA_HW_SYS,pMdma, (CB_FUN_TYPE)DmaCompleteFlag, (LPVOID)Complete);
}

void CellSrchAGCProc(U32 agcTskNum,U32 agcLen,U32 TotalLen,U32 RmsTgt)
{
	float RawSigPow ;
	U32 GainFactor;
	U8  pipaFlag;
	S16* agcDdrIn;
	S16* agcCramInCur;
	S16* agcCramInNext;
	volatile U32* CompleteFlag;
	U32 i,j;
	
	pipaFlag = 0;
	RawSigPow = 0;
	CompleteFlag =	(U32*)(tCellSrchDesc.agcIn +2*agcLen*agcTskNum*4);
	*CompleteFlag = AGC_SOFTWARE_DONE;
	CompleteFlag =	(U32*)(tCellSrchDesc.agcIn + 2*(2*agcLen*agcTskNum*4)+4);
	*CompleteFlag = AGC_SOFTWARE_DONE;
	for(i = 0; i < ((TotalLen*4)/(agcLen*agcTskNum*4)); i++)
	{
		if(i == 0)
		{
			 agcCramInNext = tCellSrchDesc.agcIn;
			 CompleteFlag = (U32*)(agcCramInNext + (2*agcLen*agcTskNum*4));
			 agcDdrIn = &gUMTSNwlAntData0[0];
			 DmaAGCData((U32*)agcCramInNext,(U32*)agcDdrIn,sizeof(S16)*2*agcLen*agcTskNum*4,(U32*)CompleteFlag);
		}
		else
		{
			 agcCramInCur =  tCellSrchDesc.agcIn + (2*agcLen*agcTskNum*4+4)*pipaFlag;
			 pipaFlag =(pipaFlag+1)&1; 
			 agcCramInNext = tCellSrchDesc.agcIn + (2*agcLen*agcTskNum*4+4)*pipaFlag;
			 agcDdrIn = &gUMTSNwlAntData0[i*(agcLen*agcTskNum*4*2)];
	
			 CompleteFlag = (U32*)(agcCramInNext + (2*agcLen*agcTskNum*4));
			 while(*CompleteFlag!=AGC_SOFTWARE_DONE);
			 DmaAGCData((U32*)agcCramInNext,(U32*)agcDdrIn,sizeof(S16)*2*agcLen*agcTskNum*4,(U32*)CompleteFlag);
	
			 CompleteFlag = (U32*)(agcCramInCur + (2*agcLen*agcTskNum*4));
			 while(*CompleteFlag!=AGC_DMA_DONE);
			 CreatAgcTskList(agcCramInCur,tCellSrchDesc.rawPow,agcTskNum, agcLen,(U32*)CompleteFlag);
			 for(j = 0; j < agcTskNum; j++)
			 {
				RawSigPow +=tCellSrchDesc.rawPow[j];
			 }
			 
		 }
		 if(i==((TotalLen*4)/(agcLen*agcTskNum*4)-1))
		 {
		 	 
			 agcCramInCur =  tCellSrchDesc.agcIn + (2*agcLen*agcTskNum*4+4)*pipaFlag;
			 CompleteFlag = (U32*)(agcCramInCur + (2*agcLen*agcTskNum*4));
			 
			 while(*CompleteFlag!=AGC_DMA_DONE);
			 CreatAgcTskList(agcCramInCur,tCellSrchDesc.rawPow,agcTskNum, agcLen,(U32*)CompleteFlag);
			 for(j = 0; j < agcTskNum; j++)
			 {
				RawSigPow +=tCellSrchDesc.rawPow[j];
			 }
		 }
	}

	RawSigPow /= (TotalLen/agcLen);
	GainFactor = (2*RmsTgt*RmsTgt)/(RawSigPow);
	GainFactor = sqrt(GainFactor);	
	tCellSrchDesc.agcFactor = GainFactor;
	return;
}

void CellSrchDwonSampleProc(U32 agcTskNum,U32 agcLen,U32 TotalLen,U32 agcFactor)
{
	U8  pipaFlag;
	S16* agcDdrIn;
	S16* agcDdrOut;

	S16* agcCramInCur;
	S16* agcCramInNext;
	S16* agcCramOut;
	volatile U32* CompleteFlag;
	volatile U32* CompleteFlag1;
	U32 i;

	pipaFlag = 0;
	CompleteFlag =	(U32*)(tCellSrchDesc.agcIn +2*agcLen*agcTskNum*4);
	*CompleteFlag = AGC_SOFTWARE_DONE;
	CompleteFlag =	(U32*)(tCellSrchDesc.agcIn + 2*(2*agcLen*agcTskNum*4)+4);
	*CompleteFlag = AGC_SOFTWARE_DONE;
	CompleteFlag1 = (U32*)(tCellSrchDesc.agcOut + 2*(2*agcLen*agcTskNum*2)+4);
	*CompleteFlag1 = AGC_DMA_DONE;
	CompleteFlag1 = (U32*)(tCellSrchDesc.agcOut + 2*agcLen*agcTskNum*2);
	*CompleteFlag1 = AGC_DMA_DONE;
	
	TotalLen = 38400*130;
	for(i = 0; i < ((TotalLen*4)/(agcLen*agcTskNum*4)); i++)//130
	{
		
		 if(i == 0)
		 {
			  agcCramInNext = tCellSrchDesc.agcIn;
			  CompleteFlag = (U32*)(agcCramInNext + (2*agcLen*agcTskNum*4));
			  agcDdrIn = &gUMTSNwlAntData0[0];
			  DmaAGCData((U32*)agcCramInNext,(U32*)agcDdrIn,sizeof(S16)*2*agcLen*agcTskNum*4,(U32*)CompleteFlag);
		 }
		 else
		 {
	
			 agcCramOut =  tCellSrchDesc.agcOut + (2*agcLen*agcTskNum*2+4)*pipaFlag;
			 agcCramInCur =  tCellSrchDesc.agcIn + (2*agcLen*agcTskNum*4+4)*pipaFlag;
			 pipaFlag =(pipaFlag+1)&1; 
			 agcCramInNext = tCellSrchDesc.agcIn + (2*agcLen*agcTskNum*4+4)*pipaFlag;
			 agcDdrIn = &gUMTSNwlAntData0[i*(agcLen*agcTskNum*4*2)];
			 agcDdrOut = &gUMTSNwlAntData0[(i-1)*(agcLen*agcTskNum*2*2)];
	
			 CompleteFlag = (U32*)(agcCramInNext + (2*agcLen*agcTskNum*4));
			 while(*CompleteFlag!=AGC_SOFTWARE_DONE);
			 DmaAGCData((U32*)agcCramInNext,(U32*)agcDdrIn,sizeof(S16)*2*agcLen*agcTskNum*4,(U32*)CompleteFlag);
	
			 CompleteFlag = (U32*)(agcCramInCur + (2*agcLen*agcTskNum*4));
			 CompleteFlag1 = (U32*)(agcCramOut + (2*agcLen*agcTskNum*2));
			 while((*CompleteFlag!=AGC_DMA_DONE)||(*CompleteFlag1!=AGC_DMA_DONE));
			// GainFactor = 1;
			 CreatDownSampleTskList(agcCramInCur,agcCramOut,(U32)agcFactor,agcTskNum, agcLen,(U32*)CompleteFlag,(U32*)CompleteFlag1);
	
			 while(*CompleteFlag1!=AGC_SOFTWARE_DONE);
			 DmaAGCData((U32*)agcDdrOut,(U32*)agcCramOut,sizeof(S16)*2*agcLen*agcTskNum*2,(U32*)CompleteFlag1);
			 
		 }
		 if(i==((TotalLen*4)/(agcLen*agcTskNum*4)-1))
		 {
		 	 
			 agcCramInCur =  tCellSrchDesc.agcIn + (2*agcLen*agcTskNum*4+4)*pipaFlag;
			 CompleteFlag = (U32*)(agcCramInCur + (2*agcLen*agcTskNum*4));
			 agcDdrOut = &gUMTSNwlAntData0[i*(agcLen*agcTskNum*2*2)];
			 agcCramOut =  tCellSrchDesc.agcOut + (2*agcLen*agcTskNum*2+4)*pipaFlag;
			 CompleteFlag1 = (U32*)(agcCramOut + (2*agcLen*agcTskNum*2));
			 
			 while((*CompleteFlag!=AGC_DMA_DONE)||(*CompleteFlag1!=AGC_DMA_DONE));
			 CreatDownSampleTskList(agcCramInCur,agcCramOut,(U32)agcFactor,agcTskNum, agcLen,(U32*)CompleteFlag,(U32*)CompleteFlag1);
			 
			 while(*CompleteFlag1!=AGC_SOFTWARE_DONE);
			 DmaAGCData((U32*)agcDdrOut,(U32*)agcCramOut,sizeof(S16)*2*agcLen*agcTskNum*2,(U32*)CompleteFlag1);
		 }
	}
	return;
}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  Once cell search module receive cell search request message ,it will allocate resouce 
				and create thread.

 Function CellSrchInit.

 @param    none.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void  CellSrchInit(void)
{
    CellSrchConfigPara_t* pCellSrchPara;
    U32 RmsTgt=8192;
    U32 agcTskNum;
    U32 agcLen;
    U32 TotalLen; 

    gCellSrcStartFlag = 1; 
    gCellSearchBusyFlag = 1;
	
    tCellSrchDesc.pCellSrchPara = (CellSrchConfigPara_t*)MemAlloc(sizeof(CellSrchConfigPara_t), CRAM_MEM, 0);
    pCellSrchPara = tCellSrchDesc.pCellSrchPara;

    SetSearchCfgPara(pCellSrchPara,pCellSrchMsg->meaPeriod);

    //AntData2x = (S16*)MemAlloc(38400*2*2*2*2, DDR_MEM, 0);
    // allocate PDP buffer resouce for path monitor
    tCellSrchDesc.pschTmpBuf = (S32 *)MemAlloc(PSCH_CORR_WIN_LEN*15*sizeof(U32), CRAM_MEM, 0);
    tCellSrchDesc.PDPAnt0 = (U32 *)MemAlloc(pCellSrchPara->maxPmTskNum*PM_SEARCH_WIN_LEN*SAMPLE_RATE*sizeof(U32), CRAM_MEM, 0);
    tCellSrchDesc.PDPAnt0Hist = (U32 *)MemAlloc(pCellSrchPara->maxPmTskNum*(PM_SEARCH_WIN_LEN*SAMPLE_RATE+12)*sizeof(U32), CRAM_MEM, 0);
    tCellSrchDesc.PDPAnt0_8x = (U32 *)MemAlloc(pCellSrchPara->maxPmTskNum*(PM_SEARCH_WIN_LEN*8+27)*sizeof(S32), CRAM_MEM, 0);// the 27 is 2x to 8x filter delay
    if( 2 == pCellSrchPara->rxAntNumber )
    {
    	tCellSrchDesc.PDPAnt1 = (U32 *)MemAlloc(pCellSrchPara->maxPmTskNum*PM_SEARCH_WIN_LEN*SAMPLE_RATE*sizeof(U32), CRAM_MEM, 0);
    	tCellSrchDesc.PDPAnt1Hist = (U32 *)MemAlloc(pCellSrchPara->maxPmTskNum*(PM_SEARCH_WIN_LEN*SAMPLE_RATE+12)*sizeof(U32), CRAM_MEM, 0);
    	tCellSrchDesc.PDPAnt1_8x = (U32 *)MemAlloc(pCellSrchPara->maxPmTskNum*(PM_SEARCH_WIN_LEN*8+27)*sizeof(S32), CRAM_MEM, 0);// the 27 is 2x to 8x filter delay
    }
    tCellSrchDesc.PDP2AntCombine =  (U32 *)MemAlloc(pCellSrchPara->maxPmTskNum*(PM_SEARCH_WIN_LEN*8)*sizeof(U32), CRAM_MEM, 0);

    // allocate static squence buffer used for SSCH detecting
    tCellSrchDesc.Zseq = (S8 *)MemAlloc(256, CRAM_MEM, 0);
    tCellSrchDesc.HadamaSeq16 = (S8 *)MemAlloc(16*16, CRAM_MEM, 0);
    tCellSrchDesc.GrpSlotMapTbl = (U8 *)MemAlloc(64*15, CRAM_MEM, 0);


    tCellSrchDesc.pschAnt0Pow =   (U32 *)MemAlloc(CHIP_NUM_PER_SLOT*SAMPLE_RATE*sizeof(U32), CRAM_MEM, 0);
    // for path monitor for all users, the maximun data needed  is 2slot + window lenght size
    tCellSrchDesc.RxAnt0Data = (S16*) MemAlloc((2*CHIP_NUM_PER_SLOT+PM_SEARCH_WIN_LEN)*2*SAMPLE_RATE*2*sizeof(S16), CRAM_MEM, 0);
    if( 2 == pCellSrchPara->rxAntNumber )
    {
        tCellSrchDesc.pschAnt1Pow =   (U32 *)MemAlloc(CHIP_NUM_PER_SLOT*SAMPLE_RATE*sizeof(U32), CRAM_MEM, 0);
        tCellSrchDesc.RxAnt1Data = (S16*) MemAlloc((2*CHIP_NUM_PER_SLOT+PM_SEARCH_WIN_LEN)*2*SAMPLE_RATE*2*sizeof(S16), CRAM_MEM, 0);
    }

    // allocate primary scrambling code sequence buffer
#if 0 
    tCellSrchDesc.PScrSeqFrame = (S8*) MemAlloc(DL_CODEGRP_NUM*PSCR_NUM_PER_CODEGRP*CHIP_NUM_PER_SLOT*SLOT_NUM_PER_FREAM*2, DDR_MEM, 0);
#else
    tCellSrchDesc.PScTemp = (S8*) MemAlloc(CHIP_NUM_PER_SLOT*SLOT_NUM_PER_FREAM*2, CRAM_MEM, 0);
    tCellSrchDesc.PScrSeqFrame = (S8*) MemAlloc(DL_CODEGRP_NUM*PSCR_NUM_PER_CODEGRP*CHIP_NUM_PER_SLOT*SLOT_NUM_PER_FREAM*2, DDR_MEM, 0);
#endif
        
    tCellSrchDesc.PscSeqSlot = (S8*) MemAlloc(PSCR_NUM_PER_CODEGRP*CHIP_NUM_PER_SLOT*2*2, CRAM_MEM, 0);
    // allocate PSCH detecting peak informantion buffer
    // Tao modify
    //tCellSrchDesc.pschPeakInfo = (PschPeak_t *) MemAlloc(sizeof(PschPeak_t), CRAM_MEM, 0);
    tCellSrchDesc.pschPeakInfo = (PschPeak_t *) MemAlloc(sizeof(PschPeak_t), CRAM_MEM, 0);
    tCellSrchDesc.pschPeakPos= (PschPeakPos_t *) MemAlloc(sizeof(PschPeakPos_t), CRAM_MEM, 0);
    tCellSrchDesc.cellGrpDetcInfo = (CellGrpDetcInfo_t *) MemAlloc(sizeof(CellGrpDetcInfo_t), CRAM_MEM, 0);
    // allocate cell search report message buffer
    tCellSrchDesc.cellSrchRpt = (CellSrchRpt_t *)MemAlloc(sizeof(CellSrchRpt_t), CRAM_MEM, 0);
    // allocate cell search thread resource
    tCellSrchDesc.ThreadCtx = (CellSrchThreadCtx_t *) MemAlloc(sizeof(CellSrchThreadCtx_t), DDR_MEM, 1);

    tCellSrchDesc.pAngTanTable = (S16 *)MemAlloc(sizeof(S16)*256*2, CRAM_MEM, 0);


    agcTskNum = 4;
    agcLen = 640;
    tCellSrchDesc.agcIn = (S16 *)MemAlloc((sizeof(S16)*2*agcLen*agcTskNum*4+4)*2, CRAM_MEM, 0);//4
    tCellSrchDesc.agcOut = (S16 *)MemAlloc((sizeof(S16)*2*agcLen*agcTskNum*2+4)*2, CRAM_MEM, 0);//4
    tCellSrchDesc.rawPow = (U32*)MemAlloc(sizeof(U32)*2*10, CRAM_MEM, 0);//4

    TotalLen = 38400*2;
    CellSrchAGCProc( agcTskNum, agcLen, TotalLen, RmsTgt);

    TotalLen = 38400*130;
    CellSrchDwonSampleProc( agcTskNum, agcLen, TotalLen,tCellSrchDesc.agcFactor);

    tCellSrchDesc.RxAnt0DataDDR = gUMTSNwlAntData0;
    tCellSrchDesc.RxAnt1DataDDR = NULL;

    tCellSrchDesc.cellSrchRpt->tCellSearchInd.nr_cellFoundList = 0;
 
    //initialization of global variables
    memcpy(tCellSrchDesc.HadamaSeq16, &HadamaSeq16[0][0], 16*16);
    memcpy(tCellSrchDesc.GrpSlotMapTbl, &GrpSlotMapTbl[0][0], 64*15);
    memcpy(tCellSrchDesc.pAngTanTable,&astanTable_Q16[0][0],256*2*sizeof(S16));
    memset(tCellSrchDesc.pschPeakInfo,0,sizeof(PschPeak_t));
    memset(tCellSrchDesc.pschPeakPos,0,sizeof(PschPeakPos_t));
    memset(tCellSrchDesc.cellGrpDetcInfo,0,sizeof(CellGrpDetcInfo_t));
    memset(tCellSrchDesc.PDPAnt0Hist,0,pCellSrchPara->maxPmTskNum*(PM_SEARCH_WIN_LEN*SAMPLE_RATE+12)*sizeof(U32));
    memset(&tCellSrchDesc.pschPeakInfo->PsynPk.GrpNum[0],0xff,MAX_PEAK_NUM);
    ZSeqGen(tCellSrchDesc.Zseq);
        
    tCellSrchDesc.PrscOffset = 0;
    tCellSrchDesc.AntOffset = 0;

}

void  CellSrchStopStep1(void)
{
	CellSrchConfigPara_t* pCellSrchPara;
	
	pCellSrchPara = tCellSrchDesc.pCellSrchPara;

	MemFree(tCellSrchDesc.PDPAnt0 , CRAM_MEM);
	MemFree( tCellSrchDesc.PDPAnt0Hist ,CRAM_MEM);
	MemFree(tCellSrchDesc.PDPAnt0_8x , CRAM_MEM);
	MemFree(tCellSrchDesc.pschAnt0Pow, CRAM_MEM);
	MemFree(tCellSrchDesc.RxAnt0Data, CRAM_MEM);
	MemFree(tCellSrchDesc.pschTmpBuf,CRAM_MEM);
	if( 2 == pCellSrchPara->rxAntNumber )
	{
		MemFree(tCellSrchDesc.PDPAnt1 , CRAM_MEM);
		MemFree( tCellSrchDesc.PDPAnt1Hist ,CRAM_MEM);
		MemFree(tCellSrchDesc.PDPAnt1_8x , CRAM_MEM);
		MemFree(tCellSrchDesc.pschAnt1Pow, CRAM_MEM);
		MemFree(tCellSrchDesc.RxAnt1Data, CRAM_MEM);
	}
	
	MemFree(tCellSrchDesc.PDP2AntCombine, CRAM_MEM);
	MemFree(tCellSrchDesc.Zseq, CRAM_MEM);
	MemFree(tCellSrchDesc.HadamaSeq16, CRAM_MEM);
	MemFree(tCellSrchDesc.GrpSlotMapTbl , CRAM_MEM);
	MemFree(tCellSrchDesc.PscSeqSlot , CRAM_MEM);
	MemFree(tCellSrchDesc.pschPeakInfo, CRAM_MEM);
	MemFree(tCellSrchDesc.pschPeakPos, CRAM_MEM);
	MemFree(tCellSrchDesc.cellGrpDetcInfo, CRAM_MEM);
	MemFree(tCellSrchDesc.cellSrchRpt , CRAM_MEM);
	MemFree(tCellSrchDesc.ThreadCtx , DDR_MEM);
	MemFree(tCellSrchDesc.pCellSrchPara,CRAM_MEM);
	MemFree(tCellSrchDesc.PScrSeqFrame,DDR_MEM);
	MemFree(tCellSrchDesc.PScTemp,CRAM_MEM);
	MemFree(tCellSrchDesc.pAngTanTable,CRAM_MEM);
	
	MemFree(tCellSrchDesc.agcIn,CRAM_MEM);
	MemFree(tCellSrchDesc.agcOut,CRAM_MEM);
	MemFree(tCellSrchDesc.rawPow,CRAM_MEM);


	return;

}
/********************************************************************
*   Function name: CellSrchStopStep2
*   Description: release the resource used in BCH decoding
*   Input Parameters:  
*   Output parameters
*   return:
*         none
*
*********************************************************************/
void  CellSrchStopStep2(void)
{
	CellSrchConfigPara_t* pCellSrchPara;
	
	pCellSrchPara = tCellSrchDesc.pCellSrchPara;
	MemFree(tCellSrchDesc.RxAnt0DataDDR, DDR_MEM);
	MemFree(tCellSrchDesc.PScrSeqFrame, DDR_MEM);
	if( 2 == pCellSrchPara->rxAntNumber )
	{
		MemFree(tCellSrchDesc.RxAnt1DataDDR, DDR_MEM);
	}
	MemFree(tCellSrchDesc.pCellSrchPara,CRAM_MEM);
	
}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  Once cell search module receive cell search stop message ,it will free resouce 
				and kill thread.

 Function CellSrcStopPro.

 @param    none.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CellSrcStopPro(void)
{

    CellSrchStopStep1();
    gCellSrcStartFlag = 0;
    NWL_GenerateAPIMsg(NWL_STOP_CELL_SEARCH_CNF, NWL_ERR_OK,API_TYPE_IND);
    return;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief just for VC send test data , save data into ddr memory	

 Function GetCLBchAntData.
 
 @param  DataBuf = input data.
 @param  DataSz = data size.
 @return	none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CellSrchAntDataRev(U8 *DataBuf, U32 DataSz)
{
    U8 *Dec;
    Dec = (U8*)gUMTSNwlAntData0;

    Dec = Dec+gAntOffset;
    memcpy(Dec,DataBuf,DataSz);
    gAntOffset += DataSz;
    return ;
	
}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief just for VC send scrambling code , save it into cram memory  

 Function GetCLScrCode.
 
 @param  DataBuf = input data.
 @param  DataSz = data size.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CellSrchPscSeqRev(U8 *DataBuf, U32 DataSz)
{
	U8 *Dec;

	Dec = (U8*)tCellSrchDesc.PScrSeqFrame;
	Dec = Dec+tCellSrchDesc.PrscOffset;
	memcpy(Dec,DataBuf,DataSz);
	tCellSrchDesc.PrscOffset += DataSz;
	return;
	
}


