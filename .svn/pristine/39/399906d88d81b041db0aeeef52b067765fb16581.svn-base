#ifndef _CELLSEARCHCEVA_H
#define _CELLSEARCHCEVA_H
#include "UMTSNwlCellSearchCom.h"
#ifndef VS_ENV
#include "..//prototypes.h"
#include "tcb.h"
#endif


#define COHERENT_SHIFT_BIT			8	// number of right shift at the output of coherent correlation
#define POWER_SHIFT_BIT				13  // square of coherent correlation 
#define SSCH_COHERENT_SHIFT_BIT		4 
#define SSCH_H_SHIFT_BIT			4
#define SSCH_POWER_SHIFT_BIT		5
#define SSCH_NONCOH_SHIFT_BIT		14
#define CPICH_COHERENT_SHIFT_BIT	9
#define CPICH_POWER_SHIFT_BIT		13
#define MAX_16_BIT_SIGN			(32767) 
#define MIN_16_BIT_SIGN			(-32768)
#define MAX_16_BIT_UNSIGN		(65536)
#define PM_FILTER_FACTOR_1			0	// IIR filter for non-coherenet results in path searching, weighting on older result; old value = 7
#define PM_FILTER_FACTOR_2			1   // weighting on new result
#define PM_FILTER_SHIFT_BIT			0	 // PM IIR filtering result shift bit; old value = 3
#define PATH_COHERENT_SHIFT_BIT		8
#define PATH_FREQ_SHIFT_BIT			16
#define PATH_FREQ_2_SHIFT_BIT		1
#define PATH_POWER_SHIFT_BIT		13

#define RSCP_COHERENT_SHIFT_BIT		8
#define RSCP_FREQ_SHIFT_BIT			16
#define RSCP_POWER_SHIFT_BIT		13

#define 	FREQ_COMP_STEP			0	// 0 ~256chip, 1~128chip,2 ~64chip, 3~32chip,4 ~16chip, 5~8chip,6 ~4chip, 7~2chip,8~1chip
#define PI2        (25735)  // Q12 of 3.14125 * 2  
#define FRE_STEP  (100)  //Q12 of 0.025  100 = 0.025*4096



#ifndef VS_ENV

void PmSearchMain(S8 Pscr[],S16 InputAnt0[],S16 InputAnt1[],U16 accCount,U16 accLen,U8 AntNum,U32 PDPAnt0[],U32 PDPAnt1[],
					U32 PDPAnt0His[],U32 PDPAnt1His[],CellPathInfo* pCellPath,U16 WinLen,U32 *p8xAnt0,U32 *p8xAnt1,U32 *pCom2AntPk,
					U8 FirstFlag,U8 thresholdFactor, S16 angle,S16* angTanTable)CEVAEXTERN;


void GolayCor(S16 Intput[],U32 accCount, U32 accLen, U8 IQFlag,U32 Output[],S32 *CeVaBuffer,U32 subWinLen)CEVAEXTERN;
void PmInsertPath(U32 PeakPower,U32  PeakDelay,U8 PeakAnt,CellPathInfo* psCellPath)CEVAEXTERN;
void CalRssi(S16 *Input,U32 Len,U32 *Pow_Out)CEVAEXTERN;
void CalCpichPow(S16 Input[],S8 Pscr[],U8 index,U8 count,S16* SignalI,S16* SignalQ,S16 angle,S16 *pAngleTable)CEVAEXTERN;
void PmIIRFilter(U32 *pAnt0New,U32 *pAnt1New,U32 *pAnt0Old,U32 *pAnt1Old,U32 AntNum, U8 FirstFlag,U16 WinLen)CEVAEXTERN;
void PmInter2xTo8xFilter(U32 *p2xDataIn,U32* p8xDataOut, U16 InputDataLen)CEVAEXTERN;
void PmCorr(S8 Pscr[],S16 InputAnt0[],S16 InputAnt1[],U8 Count,U8 PmNcLen,U8 AntNum,U32 PDPAnt0[],U32 PDPAnt1[],
				U32 *pThsd,U16 WinLen,U8 thresholdFactor, S16 angle,S16 *pAngleTable)CEVAEXTERN;

void PmPeakDect(S32 *pAnt0PkIn,S32 *pAnt1PkIn,U32 *pComb2AntPk,U32 WinLen,S32 Thsd,U8 AntNum,   CellPathInfo* psCellPath)CEVAEXTERN;

void UMTSNwlPschTask(PTCB pTCB)CEVAEXTERN;
void UMTSNwlSschTask(PTCB pTCB)CEVAEXTERN;
void UMTSNwlCpichTask(PTCB pTCB)CEVAEXTERN;
void UMTSNwlCalcFreqEstTask(PTCB pTCB)CEVAEXTERN;
void UMTSNwlPmTask(PTCB pTCB)CEVAEXTERN;
void UMTSNwlRSCPTask(PTCB pTCB)CEVAEXTERN;
void UMTSDownSampleTask(PTCB pTCB)CEVAEXTERN;
void UMTSAgcTask(PTCB pTCB)CEVAEXTERN;

void PschCorr(S16 Rxdata[],U32 accCount,U32 accLen,U32 PschPow[],S32* CevaBuff, U32 subWinLen)CEVAEXTERN;
void SschCorr(S16* pSlotAdrr,S8 ZSeq[],S8* pHSeq,U32* pOutPut)CEVAEXTERN;
void SlotMatch(U32*  pFHTRst,U8*  pGrpMap,U8* pSlotNum, U8* pCodeGrpNum, U8 thresholdFactor,U32* SschPow, U32* SschMean)CEVAEXTERN;
void PcpichCorr(S8* pPscr[],S16 InputD[],U16 coherentLen,U32 AccVal[])CEVAEXTERN;
void CalcFreErr(S8 Pscr[],S16 FreErrVale[],S16 InputAnt0[],U32 ChipNum,U8 SampleRate,U8 Count,S16* deScraData)CEVAEXTERN;
void DlRscpEst(S16* pInputAnt0,S16* pInputAnt1,CellPathInfo* pCellPath,S8 Pscr[],U8 count,S16 angle,S16 *pAngleTable)CEVAEXTERN;

#else
void PmSearchMain(S8 Pscr[],S16 InputAnt0[],S16 InputAnt1[],U16 accCount,U16 accLen,U8 AntNum,U32 PDPAnt0[],U32 PDPAnt1[],
					U32 PDPAnt0His[],U32 PDPAnt1His[],CellPathInfo* pCellPath,U16 WinLen,U32 *p8xAnt0,U32 *p8xAnt1,U32 *pCom2AntPk,
					U8 FirstFlag,U8 thresholdFactor, S16 angle,S16* angTanTable);

void PschCorr(S16 Rxdata[],U32 accCount,U32 accLen,U32 PschPow[],S32* CevaBuff, U32 subWinLen);
void SschCorr(S16* pSlotAdrr,S8 ZSeq[],S8* pHSeq,U32* pOutPut);
void SlotMatch(U32*  pFHTRst,U8*  pGrpMap,U8* pSlotNum, U8* pCodeGrpNum, U8 thresholdFactor,U32* SschPow, U32* SschMean);
void PcpichCorr(S8* pPscr[],S16 InputD[],U16 coherentLen,U32 AccVal[]);
void CalcFreErr(S8 Pscr[],S16 FreErrVale[],S16 InputAnt0[],U32 ChipNum,U8 SampleRate,U8 Count,S16* deScraData);
void DlRscpEst(S16* pInputAnt0,S16* pInputAnt1,CellPathInfo* pCellPath,S8 Pscr[],U8 count,S16 angle,S16 *pAngleTable);


#endif



#endif
