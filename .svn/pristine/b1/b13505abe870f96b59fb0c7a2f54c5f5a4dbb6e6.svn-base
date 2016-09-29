#ifndef _CELLSEARCHARM_H
#define _CELLSEARCHARM_H
#include "UMTSNwlCellSearchCom.h"

#define   ERRORRST						-1
#define   CSUCCESS						0
#define	SRCH_DATA_NUM				      130

#define   PSCH_CORR_WIN_LEN			2560



typedef struct 
{
	U8	pschAccLen;  // the psch accumulation length , in unit of  frames.
	U8	sschAccLen;	 // the ssch accumulation length , in unit of  frames.
	U8	pcpichAccLen; // the pcpich accumulation length , in unit of  frames.
	U8	pmAccLen; // the path monitor length , in unit of  slots.
	U8	rscpAccLen;
	U8	pcpichConherentLen; // in unit of 256 chips, 1=256chip, 2=512chip..
	U8	pschThsdFactor;
	U8	sschThsdFactor;
	U8	pcpichThsdFactor;
	U8	pmThsdFactor;
	U8	rxAntNumber;
	U16	pschSubWinLen; //the sub window length of PSCH detecting. e.g. if there are 10 CEVA, each sub window is 256 chips to get 2560 PSCH powers
	U8	maxPmTskNum; //the maximun number of path minitor task per group
}CellSrchConfigPara_t;


/*Cell Search Module share these informations with BCH Module */
typedef struct
{
    U8 SetFlag;// rwhen the value is '1' means  the struct is available
    U8 FingerNum;
    U16 Psc;
    S8 * pPscBuf;
    U32 WinBegin;
    U8 SlotNum;
    S16 InitAngle;
    U8 thresholdFactor;
    U16 PathOffset[PM_MAX_FINGER_NUM];
}NWL_CommInfor;



void SetSearchCfgPara(CellSrchConfigPara_t *pCellSrchPara,U8 meaPeroid);
void SetSharePara(U8 index,CellPathInfo *pCellInfo, S16 angle,NWL_CommInfor  *pNwlCommInfo );
void ZSeqGen(S8* pcOutputAdrr);
void PmParaCala(CellGrpDetcInfo_t *cellGrpDetcInfo,PschPeakPos_t  *pschPeakPos,PschPeak_t *pschPeakInfo,U16 PkIdx);
U32	PscMeanCalc(U32* pPschAnt0Pow,U32* pPschAnt1Pow,U8 antNum);
void PschPeakDetc(U32 Ant0Power[],U32 Ant1Power[],U32 Thsd,U8 AntNum,PschPeakPos_t* PeakInfo);
void CellPscDetec(U8 conherentLen, U16 PkIdx,U8 ThsdFactor,PschPeak_t *pschPeakInfo );
void GetFreAngle(CellGrpDetcInfo_t *cellGrpDetcInfo,U8 tskIdx, U8 tskNum);

#endif

