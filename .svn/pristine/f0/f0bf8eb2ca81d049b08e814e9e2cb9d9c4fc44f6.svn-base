#ifndef _CellListen_h
#define _CellListen_h

#ifndef VS_ENV
#include "..//prototypes.h"
#endif
#include "UMTSNwlCommMacro.h"

//typedef unsigned long U32 Uint32;

/* ==========================================================================
====================================================
function: Channel frequency offset estimate  used CPICH
notes: 
author:qingyu gao
date:7/28/2011
schame: 
=============================================================================
=================================================== */



typedef struct
{
    S16 *pAnteDataIn;
    S8 *pucScrCode;
    S16 *psPchMrcData;
    S16 *pstanTable_Q16;
    S16 *psFreErrEst_4Frame;
    S8 *pucSpSf256_0;
    S8 *pucSpSf256_1;
    S16  *psFocDeSpCpich;
    S16 *psFocDeSpPccpch;
    U32 ucTSCount;
    U8 ucSampRate;
	U8 symIdx;
    S16 FrequencyAngle;
    U8 FingerNum;
    U16 pPathWeight[MAXIMUM_FINGER_NUM];
    U16 ppathOffset[MAXIMUM_FINGER_NUM];
}TDeModuTaskPara;



/* the parameters of input */
typedef struct
{
    U32   udDataIdx; /* record the index of antenna data which per 64 chips */
    S16  *psAntDataIn;
    S16  *psChipDataOut;
    S16   *psDeSp_cpich;  /* used to store cpich despread data */
    S16    *psFoc_CPich;
    S16    *psFocDeSp_Cpich;
    S16    *psFoc_Pccpch;	
    S16 	*psFocDeSpPch;	
    S16    *psMrcPch;
    S16    *psDeSpPch;
    S16	  angle;
    S16     *pTanTable;
    S16    *psFreErrEst_4F;/* 4 frames frequence err est */
    U16    FingerWeight;
    S8    *pucBuffPing;
    S8    *pucBuffPang;
    S8    *pucScrCode;
    S8     *pucSP256_0;
    S8     *pucSP256_1;
    U8 	 ucSampRate;
    U8     ucTsCntIdx;
    U8     uc4FrameTsCnt;
    U8     ucOneTSFlag;
    U16    ucOneTsLoop;
    U8     ucFingerIdx; 

}TDeModuProPara;




#ifdef VS_ENV
void AntData2xTo8xFilter(S16 *p2xDataI,S16 *p2xDataQ, S16 *p8xDataI, S16 *p8xDataQ, U32 DataLen);
void CLDeModFun(TDeModuProPara *pCL);
void NUMTSNwlDeModuPro(TDeModuTaskPara *ptDeMoPar);
void CLDeCodeFun(S8 *pPingBuff,S8 *pPangBuff,S8 *pStateTable,
				S32 *pMetric,S32 *pNewMetric,U8 *pTbBuf,S16 *pMrc,U8 DecodeFlag);
void CLChECFun(U32 SlotIdx, S16 *pCpi, S16 *pFingerSym, S16 *pPcc, S16 *pSum_out, U8 FingerNum);

#else
void CLDeModFun(TDeModuProPara *pCL) CEVAEXTERN;

void NUMTSNwlDeModuPro(TDeModuTaskPara *ptDeMoPar)CEVAEXTERN;

void CLDeCodeFun(S8 *pPingBuff,S8 *pPangBuff,S8 *pStateTable,
				S32 *pMetric,S32 *pNewMetric,U8 *pTbBuf,S16 *pMrc,U8 DecodeFlag)CEVAEXTERN;

void CLChECFun(U32 SlotIdx, S16 *pCpi, S16 *pFingerSym, S16 *pPcc, S16 *pSum_out, U8 FingerNum)CEVAEXTERN;
S8 S16_2_S8(S16 sData)CEVAEXTERN;

#endif


#endif
