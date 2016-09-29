#ifndef _CELLSEARCHCTRL_H
#define _CELLSEARCHCTRL_H

#include "ltephydef.h"
#include "gputil_fx.h"
//#include "tcbscheduler.h"
#include "mdmadrv.h"
#include "4gmx_types.h"
#include "4gmx_thread.h"
#include "apimgr.h"
#include "tcb.h"

#include "UMTSNwlCellSearchArm.h"
#include "UMTSNwlMsgPro.h"


#define   PSCH_STEP						1
#define   SSCH_STEP						2
#define	PCPICH_STEP					3
#define	PM_STEP						4
#define	RSCP_STEP						5
#define   FREQ_ESTI_STEP                            6

#define   MAX_PEAK_NUM					256
#define   MAX_CELLLSRCH_NUM			16








typedef struct 
{
	U8 ProFlag;
	U16 SlotCnt;
	U8 antFlag;
	U8 pmGrpNum;
	U8 pmGrpIdx;
	U8 pmTskNumLastGrp;
	U16 PkIdx;
	
} CellSrchThreadCtx_t;



typedef struct 
{
    ApiHeader header;
	T_3G_CELL_SEARCH_IND tCellSearchInd;

}CellSrchRpt_t;

typedef struct 
{
	CellGrpDetcInfo_t *cellGrpDetcInfo;
	S16* agcIn;//agc 0918
    S16* agcOut;
	S32*	pschTmpBuf;
	U32	agcFactor;
	U32* rawPow;
	PschPeak_t *pschPeakInfo;
	PschPeakPos_t  *pschPeakPos;
	CellSrchConfigPara_t *pCellSrchPara;   
	CellSrchThreadCtx_t *ThreadCtx;
	CellSrchRpt_t *cellSrchRpt;
    S16* pAngTanTable;
	PMDMA_FDESC pMdma;

	U32 *PDPAnt0;
	U32 *PDPAnt1;

	U32 *PDPAnt0Hist;
	U32 *PDPAnt1Hist;

	U32 *PDPAnt0_8x;
	U32 *PDPAnt1_8x;
	U32 *PDP2AntCombine;


	U32 *pschAnt0Pow;
	U32 *pschAnt1Pow;

	S8 *Zseq;

	S16 *RxAnt0Data;
	S16 *RxAnt1Data;
	S16 *RxAnt0DataDDR;
	S16 *RxAnt1DataDDR;
	U32  AntOffset;
	U32  PrscOffset;

	U16 SetPscFlag;
	U16 SetPscValue;
	S8 *PScTemp;
	S8 *PScrSeqFrame;
	S8 *PscSeqSlot;
	S8 *HadamaSeq16;
	U8 *GrpSlotMapTbl;
	
	PTASKLIST TaskList1;
	PTASKLIST  TaskList2;
    HANDLE hCellSrchThread;
}CellSrchDesc_t;


void CellSrchTrigger(U16 PscIdx ,U8 MeasPeriod );


void CellSrchAntDataRev(U8 *DataBuf, U32 DataSz);
void CellSrchPscSeqRev(U8 *DataBuf, U32 DataSz);

void  CellSrchInit(void);
void CellSrchStopStep1(void);
void CellSrchStopStep2(void);
MXRC UMTS_Nwl_cell_search_thread_init(void);
void UMTS_Nwl_cell_search_thread_kill(void);
void Umtsnmm_CS_TcbInit(void);
void Umtsnmm_CS_TcbStop(void);
void CellSrcStopPro(void);

#endif

