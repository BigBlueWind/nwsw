//-------------------------------------------------------------------------------------------
/** @file UMTSNwlBchDecodeTask.c
 *
 * @brief BCH Decoder Data processing run at CEVA
 * @author Mindspeed Technologies
 * @version $Revision: 1.3.4.5 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------
/*------commom head file-------*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<math.h>

#include "UMTSNwlCommMacro.h"
#include "UMTSNwlCellSearchCeva.h"
#include "UmtsNwlBchDecodePro.h"
#include "UMTSNwlFreEstCom.h"
#include "UMTSNwlGenDlPSC.h"
#ifndef VS_ENV
#include <dsp_lib_1641.h>
#endif


/*run at ceva*/
#include <asm-dsp.h>
#include "TCB.h" 
#include "ceva_functions.h"

#include "UMTSNwlBchDecodeTask.h"
#include "UMTSNwlCellSearchCeva.h"

#define EXTRACTTCBIOBUF(tcb, ionum)     (*((U32*)tcb->IOControlPtr+ (ionum*2)))

#ifndef VS_ENV
void AntData2xTo8xFlt2(short *X, q15 *H, int nX,  short *Y)CEVAEXTERN;
extern U8 *DataBuff_Bank1;
#endif


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  extract the parameters from tcb task for path search processing

 Function UMTSNwlPathSearchTask

 @param    pTCB =  a pointer of TCB.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void UMTSNwlPathSearchTask(PTCB pTCB)
{
    S16* Ant0Input=NULL;
    S8* pPSC=NULL;
    U32* pPDPNew=NULL;
    U32* pPDPHis=NULL;
    U32* p8XAnt=NULL;
    U32* p2ComPk=NULL;
    CellPathInfo* pPathInfo=NULL;
    U8 Count;
    U32 *pContxt=NULL;
    U8 NcLen;
    U8 FirstFlag;
    U8 ThresholdFactor;
    S16 FreOffAngle;
    S16 *pTanTable;

	
    Ant0Input = (S16 *) EXTRACTTCBIOBUF(pTCB, 0);
    pPSC = (S8 *) EXTRACTTCBIOBUF(pTCB, 1);
    pPDPNew = (U32 *) EXTRACTTCBIOBUF(pTCB, 2);
    pPDPHis = (U32 *) EXTRACTTCBIOBUF(pTCB, 3);
    p8XAnt = (U32 *) EXTRACTTCBIOBUF(pTCB, 4);
    p2ComPk = (U32 *) EXTRACTTCBIOBUF(pTCB, 5);
    pPathInfo = (CellPathInfo *) EXTRACTTCBIOBUF(pTCB, 6);
    pTanTable = (S16 *) EXTRACTTCBIOBUF(pTCB, 7);


    pContxt      = (U32*) pTCB->ContextPtr;
    Count = (U8) pContxt[0];
    FirstFlag = (U8) pContxt[1];
    NcLen = (U8) pContxt[2];
    ThresholdFactor= (U8) pContxt[3];
    FreOffAngle = (S16)pContxt[4];

    PmSearchMain(pPSC,Ant0Input,Ant0Input,Count,NcLen, 1, pPDPNew, pPDPNew,pPDPHis,
						NULL,pPathInfo, PATH_SEARCH_WIN_LEN*2,p8XAnt,NULL,p2ComPk,1,ThresholdFactor,
						FreOffAngle,pTanTable);

	
	return;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  extract the parameters from tcb task for interpolation processing

 Function UMTSNwl2XTo8XTask

 @param    pTCB =  a pointer of TCB.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void UMTSNwl2XTo8XTask(PTCB pTCB)
{
    U32* pContext = NULL;
    S16 *pIn2X = NULL;
    S16  *pOut8X = NULL;
    U32 CalcLen;
#ifndef VS_ENV
	
	short coff[28]= {
		9,2,-31,-9,113,31,197,250,-42,-24,13,8,-3,-2,
		-2,-3,8,13,-24,-42,250,197,31,113,-9,-31,2,9,	
	};
	S16 *H;
	U32 i;
#endif
	
    pContext      = (U32*) pTCB->ContextPtr;
    CalcLen = (U32) pContext[0];
	
    pIn2X = (S16 *) EXTRACTTCBIOBUF(pTCB, 0);
    pOut8X = (S16 *) EXTRACTTCBIOBUF(pTCB, 1);
	

#ifndef VS_ENV
	H = (S16*)DataBuff_Bank1;

	for(i=0;i<28;i++)
	{	
		H[i] =coff[i]<< 6;
	}
	AntData2xTo8xFlt2(pIn2X, H, (CalcLen>>2)*4,  pOut8X);
	
#else
    AntData2xTo8xFilter(pIn2X,pOut8X,CalcLen);
#endif
    return;
}




//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  extract the parameters from tcb task for update residual frequency  offset

 Function UMTSNwlUpdateReFreErrTask

 @param    pTCB =  a pointer of TCB.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void UMTSNwlUpdateReFreErrTask(PTCB pTCB)
{
    U32* pContext = NULL;
    S16 *pResiFreErr_4F = NULL;
    S16 *pResiFreErrValue = NULL;
	S32 *pIQsum=NULL;

    pContext      = (U32*) pTCB->ContextPtr;

    pResiFreErr_4F = (S16*) EXTRACTTCBIOBUF(pTCB, 0);  /*start from 0 */
    pResiFreErrValue = (S16*) EXTRACTTCBIOBUF(pTCB, 1);
	pIQsum=(S32*) EXTRACTTCBIOBUF(pTCB, 2);

    CalculFreErrEstRun(pResiFreErr_4F,pResiFreErrValue,pIQsum);

    return;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  extract the parameters from tcb task for bch demodulation processing

 Function UMTSNwlCLDemodTask

 @param    pTCB =  a pointer of TCB.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void UMTSNwlCLDemodTask(PTCB pTCB)
{

    U32 *pContext = NULL;
    U32 ConValue;
    U8 ucFingerIdx;
    S16 *psFocDeSpCpich = NULL;
    S16 *psFocDeSpPccpch = NULL;

    TDeModuTaskPara tDeMoPar;

	
    pContext      = (U32*) pTCB->ContextPtr;
    tDeMoPar.ucTSCount  = (U32) pContext[0];
    tDeMoPar.ucSampRate = (U8)pContext[1];
    tDeMoPar.FrequencyAngle = (S16)pContext[2]; 
    tDeMoPar.FingerNum= (U8)pContext[3]; 
    tDeMoPar.symIdx =(U8) pContext[4];

    for(ucFingerIdx=0;ucFingerIdx<tDeMoPar.FingerNum;ucFingerIdx++)
    {
        ConValue = (U32) pContext[5+ucFingerIdx]; 
        tDeMoPar.pPathWeight[ucFingerIdx] = ConValue&0xffff;
        tDeMoPar.ppathOffset[ucFingerIdx] = (ConValue>>16)&0xffff;
    }
	
    tDeMoPar.pAnteDataIn = (S16*) EXTRACTTCBIOBUF(pTCB, 0);  /*start from 0 */
    tDeMoPar.pucScrCode  = (S8*) EXTRACTTCBIOBUF(pTCB, 1);
    tDeMoPar.psPchMrcData = (S16*)EXTRACTTCBIOBUF(pTCB, 2);
    tDeMoPar.pstanTable_Q16 = (S16*) EXTRACTTCBIOBUF(pTCB, 3);
    tDeMoPar.psFreErrEst_4Frame = (S16*) EXTRACTTCBIOBUF(pTCB, 4);
    tDeMoPar.pucSpSf256_0 = (S8*) EXTRACTTCBIOBUF(pTCB, 5);
    tDeMoPar.pucSpSf256_1 = (S8*) EXTRACTTCBIOBUF(pTCB, 6);
    psFocDeSpCpich =(S16*)EXTRACTTCBIOBUF(pTCB, 7);
    psFocDeSpPccpch =(S16*)EXTRACTTCBIOBUF(pTCB, 8);

    tDeMoPar.psFocDeSpCpich = psFocDeSpCpich + tDeMoPar.symIdx*2;
    tDeMoPar.psFocDeSpPccpch = psFocDeSpPccpch+tDeMoPar.symIdx*2;    

    NUMTSNwlDeModuPro(&tDeMoPar);

    return;

}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  extract the parameters from tcb task for bch decode processing

 Function UMTSNwlCLDeCodeTask

 @param    pTCB =  a pointer of TCB.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void UMTSNwlCLDeCodeTask(PTCB pTCB)
{
	U8 FingerNum;
	U8 DecodeFlag;
		
	U32* pContext = NULL;

	S8 *pucBitBuffer_Ping = NULL; /* used calc decoding data; 20ms data = 540bits */
	S8 *pucBitBuffer_Pang = NULL; /* used calc decoding data; 20ms data = 540bits */
	S8  *pcc_3g_dectab_1_2 = NULL;
	S16 *pMrc_AllFinger = NULL;
	
	S32 *pMetric= NULL;
	S32 *pNewMetric=NULL;
	U8 *pTbBuf=NULL;
	    
	pContext      = (U32*) pTCB->ContextPtr;
	FingerNum = (U8)pContext[0];
	DecodeFlag = (U8)pContext[1];

	pcc_3g_dectab_1_2= (S8*) EXTRACTTCBIOBUF(pTCB, 0);
	pucBitBuffer_Ping = (S8*) EXTRACTTCBIOBUF(pTCB,1);
	pucBitBuffer_Pang = (S8*) EXTRACTTCBIOBUF(pTCB,2);
	pMrc_AllFinger = (S16*) EXTRACTTCBIOBUF(pTCB,3);

	pMetric = (S32*) EXTRACTTCBIOBUF(pTCB,4);
	pNewMetric = (S32*) EXTRACTTCBIOBUF(pTCB,5);
	pTbBuf = (U8*) EXTRACTTCBIOBUF(pTCB,6);


	
	CLDeCodeFun(pucBitBuffer_Ping,pucBitBuffer_Pang,pcc_3g_dectab_1_2,pMetric,
                            pNewMetric,pTbBuf,pMrc_AllFinger,DecodeFlag);

	return;
}




//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  extract the parameters from tcb task for channel estimation processing

 Function UMTSNwlCLChECTask

 @param    pTCB =  a pointer of TCB.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void UMTSNwlCLChECTask(PTCB pTCB)
{
	U32 SlotCnt;
	U32* pContext = NULL;
	S16 * psFocDeSpCpich = NULL;
	S16 * psFocDeSpPccpch = NULL;
	S16 *psPchMrcData = NULL;
	S16 *pChEstVal = NULL;
	U8 ucFingerNum;
        
	pContext      = (U32*) pTCB->ContextPtr;
	SlotCnt = (U32)pContext[0];
    ucFingerNum = (U8)pContext[1];

	psFocDeSpCpich= (S16*) EXTRACTTCBIOBUF(pTCB, 0);
	psFocDeSpPccpch = (S16*) EXTRACTTCBIOBUF(pTCB,1);
    psPchMrcData =(S16*) EXTRACTTCBIOBUF(pTCB,2);   
	pChEstVal= (S16*) EXTRACTTCBIOBUF(pTCB,3);
    CLChECFun(SlotCnt,psFocDeSpCpich,psFocDeSpPccpch,psPchMrcData, 
    	    pChEstVal,ucFingerNum);


	return;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  handle different task based on task id.

 Function umtsnmm_task_parser

 @param    TaskID =  the defined task id.
 @param    pTCB =  a pointer of TCB.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void umtsnmm_task_parser(U32 TaskID, PTCB pTCB)
{
    if(TaskID == TCB_PSCH_TSKID)
    {
        UMTSNwlPschTask(pTCB);
    }
    else if(TaskID == TCB_SSCH_TSKID)
    {
        UMTSNwlSschTask(pTCB);
    }
    else if(TaskID == TCB_CPICH_TSKID)
    {
        UMTSNwlCpichTask(pTCB);
    }
    else if(TaskID == TCB_FREQ_EST_TSKID)
    {
        UMTSNwlCalcFreqEstTask(pTCB);
    }
    else if(TaskID == TCB_PM_TSKID)
    {
        UMTSNwlPmTask(pTCB);
    }
    else if(TaskID == TCB_RSCP_TSKID)
    {
        UMTSNwlRSCPTask(pTCB);
    }
    else if(TaskID == TCB_AGC_TSKID)
    {
        UMTSAgcTask(pTCB);
    }
    else if(TaskID == TCB_DOWN_SAMPLE_TSKID)
    {
        UMTSDownSampleTask(pTCB);
    }
    else if(TCB_GEN_PSC_TASKID == TaskID)
    {
        UMTSNwlGenDLPSCTask(pTCB);
    }
    else if(TaskID == TASK_PATH_SEARCH)
    {
        UMTSNwlPathSearchTask(pTCB);
    }
    else if(TaskID == TASK_CL_INTERPOLATION)
    {
        UMTSNwl2XTo8XTask(pTCB);
    }
    else if(TaskID == TASK_CL_DEMODE)
    {
        UMTSNwlCLDemodTask(pTCB);
    }
    else if(TaskID == TASK_UPDATE_FRE_ERR)
    {
        UMTSNwlUpdateReFreErrTask(pTCB);
    }
    else if(TaskID == TASK_CL_DECODE)
    {
        UMTSNwlCLDeCodeTask(pTCB);
    }
    else if(TaskID == TASK_CL_CHANESTCOM)
    {
        UMTSNwlCLChECTask(pTCB);
    }

    pTCB->Status = STATUS_COMPLETE;
    return;
}

