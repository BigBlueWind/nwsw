//-------------------------------------------------------------------------------------------
/** @file UMTSNwlBchDecode.c
 *
 * @brief BCH Decoder Data control run at ARM
 * @author Mindspeed Technologies
 * @version $Revision: 1.3.4.9 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<math.h>


#include "TCB.h" 
#include "supervisor.h"
#include "prototypes.h"
#include "UMTSNwlCommMacro.h"
#include "4gmx_types.h"
#include "4gmx_thread.h"
#include "Mdmadrv.h"
#include "UMTSNwlBchDecod.h"
#include "UMTSNwlCellSearchArm.h"
#include "UMTSNwlMsgPro.h"
#include "UMTSNwlCrc.h"
#include "Debug.h"
#include "apimgr.h"
#include "hal.h"

U32 gBeginDataOffset;
U32 gWinLen;

#define GET_ANT_DATA_ADDR     (tCLDeModuInfo.pAnteDataInDdr + gBeginDataOffset + tCLDeModuInfo.dTCBCount* 2560*4)

typedef struct //_tCLPathSearchBufInfo
{
    U8    threshold;
    U8   PSSlotCnt; /* path search slot count*/
    U8   NcLen;
    U8   FirstFlag;
    U32 *pPDPAnt0;
    U32 *pPDPAnt1;
    U32 *pPDPAnt0Hist;
    U32 *pPDPAnt1Hist;
    U32 *pAnt0_8x;
    U32 *pAnt1_8x;
    U32 *pComb2AntPk;
    S16 FreOffsetAngle;
    S16 *pstanTable;
    CellPathInfo *ptCellPathInfo; 
}T_CLPathSearchBufInfo;



/* cell listen demodulate module tcb info  */
typedef struct  
{
    S16 *pAnteDataInDdr;  
    S16 *pAnteDataInCRam_2X;  
    S16  *pAntData_8X;
    S8 *pucScrCode;
    S16 *psDeSpData ;
    S16 *psPchMrcData;
    S16 *pstanTable_Q16;
    S16  *psFreErrEst_4Frame;
    S16 *pChEstVal;
    S8 *pucSpcode_0;
    S8 *pucSpcode_1;
    S16  *psFocDeSpCpich;
    S16 *psFocDeSpPccpch;
    U32 dTCBCount;
    U32 ucTSCount;
    U8 ucFingerNum;
    U8 ucSampRate;	
    U16 PathOffset[MAXIMUM_FINGER_NUM];
    U16 FingerWeight[MAXIMUM_FINGER_NUM];
    U32 FingerPower[MAXIMUM_FINGER_NUM];
    U32 AntOffset;
    U32 ScrCodeOffset;
    S16 FreAngle;
	U8 symIdx;

}T_CLDeModuTcbInfo;



/* cell listen demodulate module tcb info  */
typedef struct  
{
    S8 *pPingData;
    S8 *pPangData;
    S8 *pcc_3g_dectab_1_2;

    S32 *pMetric;
    S32 *pNewMetric;
    U8 *pTbBuf;
    U8 ucReportFlag;
    U8 ucTTIBoundaryNoOK;
    U8  ucCrcErrFlag;
    U8 ucFingerNum;
    U8 ucDecodeFlag;
	U8 BchDecodeTtiNum;
    S16 *pMrc_AllFinger;
}T_CLDeCodeTcbInfo;


typedef struct
{
    S16 *pResiFreErr_4F;
    S16 *pResiFreErrValue;
	S32 *pIQsum;
    U8 ucFingerNum;
}T_CLUpdateReFreErrInfo;



typedef struct
{
    U8 state;
    U16 position;  
    U32 power;
}T_FingerInfo;



typedef struct
{
    U8 FingerNum;
    T_FingerInfo tFingerInfo[PM_MAX_FINGER_NUM*2];
}T_FingerPool;

U16 gPsc; // the processed primary scrambling code

CLBCHCTXT tCLBchCtxt;
T_CLPathSearchBufInfo tCLPathSearchInfo;
T_CLDeModuTcbInfo tCLDeModuInfo;
T_CLDeCodeTcbInfo tCLDeCodeInfo;
T_CLUpdateReFreErrInfo tclUpdateReFreErr;
T_FingerPool tFingerPool;

U8   aOutBuff[60];

S8 *pucScrCodeAddr = NULL;
U8  gFirstPathSearchFlag = 1;
U32 gFristBchFlag = 1;
U32 gdErrCode = 0;
U8 gTTICounter = 0;
volatile U32 DmaCnt = 0;

extern NWL_CommInfor  *pNwlCommInfo;
extern S16 *gUMTSNwlAntData0;
extern S8   *gUMTSNwlPscBuf;
extern U8    gCellSrcStartFlag;
extern U8    gBchDecodeStartFlag;
extern U8	 gBchDecodeBusyFlag;
U8 gBchStopReadyFlag = 0;



 const S8 cc_3g_dectab_1_2[256][2] = {

         0, 3, 2, 1, 0, 3, 2, 1, 2, 1, 0, 3, 2, 1, 0, 3, 
         1, 2, 3, 0, 1, 2, 3, 0, 3, 0, 1, 2, 3, 0, 1, 2, 
         3, 0, 1, 2, 3, 0, 1, 2, 1, 2, 3, 0, 1, 2, 3, 0, 
         2, 1, 0, 3, 2, 1, 0, 3, 0, 3, 2, 1, 0, 3, 2, 1, 
         3, 0, 1, 2, 3, 0, 1, 2, 1, 2, 3, 0, 1, 2, 3, 0, 
         2, 1, 0, 3, 2, 1, 0, 3, 0, 3, 2, 1, 0, 3, 2, 1, 
         0, 3, 2, 1, 0, 3, 2, 1, 2, 1, 0, 3, 2, 1, 0, 3, 
         1, 2, 3, 0, 1, 2, 3, 0, 3, 0, 1, 2, 3, 0, 1, 2, 
         2, 1, 0, 3, 2, 1, 0, 3, 0, 3, 2, 1, 0, 3, 2, 1, 
         3, 0, 1, 2, 3, 0, 1, 2, 1, 2, 3, 0, 1, 2, 3, 0, 
         1, 2, 3, 0, 1, 2, 3, 0, 3, 0, 1, 2, 3, 0, 1, 2, 
         0, 3, 2, 1, 0, 3, 2, 1, 2, 1, 0, 3, 2, 1, 0, 3, 
         1, 2, 3, 0, 1, 2, 3, 0, 3, 0, 1, 2, 3, 0, 1, 2, 
         0, 3, 2, 1, 0, 3, 2, 1, 2, 1, 0, 3, 2, 1, 0, 3, 
         2, 1, 0, 3, 2, 1, 0, 3, 0, 3, 2, 1, 0, 3, 2, 1, 
         3, 0, 1, 2, 3, 0, 1, 2, 1, 2, 3, 0, 1, 2, 3, 0, 
         3, 0, 1, 2, 3, 0, 1, 2, 1, 2, 3, 0, 1, 2, 3, 0, 
         2, 1, 0, 3, 2, 1, 0, 3, 0, 3, 2, 1, 0, 3, 2, 1, 
         0, 3, 2, 1, 0, 3, 2, 1, 2, 1, 0, 3, 2, 1, 0, 3, 
         1, 2, 3, 0, 1, 2, 3, 0, 3, 0, 1, 2, 3, 0, 1, 2, 
         0, 3, 2, 1, 0, 3, 2, 1, 2, 1, 0, 3, 2, 1, 0, 3, 
         1, 2, 3, 0, 1, 2, 3, 0, 3, 0, 1, 2, 3, 0, 1, 2, 
         3, 0, 1, 2, 3, 0, 1, 2, 1, 2, 3, 0, 1, 2, 3, 0, 
         2, 1, 0, 3, 2, 1, 0, 3, 0, 3, 2, 1, 0, 3, 2, 1, 
         1, 2, 3, 0, 1, 2, 3, 0, 3, 0, 1, 2, 3, 0, 1, 2, 
         0, 3, 2, 1, 0, 3, 2, 1, 2, 1, 0, 3, 2, 1, 0, 3, 
         2, 1, 0, 3, 2, 1, 0, 3, 0, 3, 2, 1, 0, 3, 2, 1, 
         3, 0, 1, 2, 3, 0, 1, 2, 1, 2, 3, 0, 1, 2, 3, 0, 
         2, 1, 0, 3, 2, 1, 0, 3, 0, 3, 2, 1, 0, 3, 2, 1, 
         3, 0, 1, 2, 3, 0, 1, 2, 1, 2, 3, 0, 1, 2, 3, 0, 
         1, 2, 3, 0, 1, 2, 3, 0, 3, 0, 1, 2, 3, 0, 1, 2, 
         0, 3, 2, 1, 0, 3, 2, 1, 2, 1, 0, 3, 2, 1, 0, 3}; 


/* the tan table, the circle is 2pai */
 const S16 astanTable_Q16[256][2]=
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



enum Umtsnmm_TCBLIST_id {
    PATHSEARCH_TASK_LIST=0,
    INTERPOLATION_TASK_LIST,
    CLDEMODE_TASK_LIST,
    CLChEC_TASK_LIST,
    CLDECODE_TASK_LIST,
    UPDATE_RE_FRE_ERR_TASK_LIST
};

#define TIME_UMTSNMMLOOP 5000

#define UMTSNMM_BCH_TCBLIST_NUM   (6)
#define UMTSNMM_BCH_TCB_MAXIMUM   (10)


PTASKLIST               umtsnmm_task_list[UMTSNMM_BCH_TCBLIST_NUM] = {NULL};
PTCB                    umtsnmm_tcb_arr[UMTSNMM_BCH_TCBLIST_NUM][UMTSNMM_BCH_TCB_MAXIMUM] = { 0 };
#ifdef RT_MODE
NCNBVAR volatile U32    umtsnmm_task_list_done[UMTSNMM_BCH_TCBLIST_NUM];
NCNBVAR volatile U32    umtsnmm_task_list_time[UMTSNMM_BCH_TCBLIST_NUM];
NCNBVAR volatile U32    umtsnmm_task_list_count[UMTSNMM_BCH_TCBLIST_NUM];
#else
volatile U32    umtsnmm_task_list_done[UMTSNMM_BCH_TCBLIST_NUM];
volatile U32    umtsnmm_task_list_count[UMTSNMM_BCH_TCBLIST_NUM];
#endif



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  path searcher finger managerment modul.

 Function update the finger of path searcher result

 @param     pFingerPool =   the resource of finger pool 
 @param     pCellPath =   the result of path searcher.
 @param     FirstFlag =     the flag of the first time result of path searcher or 
 not (1for first time ,0 for not the first time).
 @return    None.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void UpdateFinger(T_FingerPool *pFingerPool, CellPathInfo* pCellPath,U8 FirstFlag)
{
    U8 idx,idx2;
    S16 distanse;
    U8 FindFlag;
	
    pFingerPool->FingerNum = 0;

    if(1 == FirstFlag)
    {
        for(idx=0;idx<PM_MAX_FINGER_NUM;idx++)
        {
            if(0 != pCellPath->Path[idx].PathPower)
            {
                pFingerPool->tFingerInfo[idx].state = FINGER_STATE_ACTIVE;
                pFingerPool->tFingerInfo[idx].position = pCellPath->Path[idx].PathOffset;
                pFingerPool->tFingerInfo[idx].power= pCellPath->Path[idx].PathPower;
                pFingerPool->FingerNum ++;
#if 0
				uart_printf("finger idx: %d, position:%d, power:%d. \r\n", idx,pCellPath->Path[idx].PathOffset,
					pCellPath->Path[idx].PathPower);
#endif
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        for(idx=0;idx<PM_MAX_FINGER_NUM*2;idx++)
        {
            if(FINGER_STATE_ACTIVE == pFingerPool->tFingerInfo[idx].state)
            {
                pFingerPool->tFingerInfo[idx].state = FINGER_STATE_HOLD;
            }
        }
    
         for(idx=0;idx<PM_MAX_FINGER_NUM;idx++)
        {
                FindFlag = 0;
                
                if(0 == pCellPath->Path[idx].PathPower)
                {
                    break;
                }

				#if 0
				uart_printf("2--finger idx: %d, position:%d, power:%d. \r\n", idx,pCellPath->Path[idx].PathOffset,
					pCellPath->Path[idx].PathPower);
				#endif

                /* keep old finger*/
                for(idx2=0;idx2<PM_MAX_FINGER_NUM*2;idx2++)
                {
                    if(FINGER_STATE_HOLD != pFingerPool->tFingerInfo[idx2].state)
                    {
                        continue;
                    }

                    distanse =pCellPath->Path[idx].PathOffset- (pFingerPool->tFingerInfo[idx2].position);

                    /*if new and old finger distance <= 1/2 chips, it belongs to same finger*/
                    if((distanse <= 4) && (distanse >= -4))
                    {
                        pFingerPool->FingerNum ++;
                        pFingerPool->tFingerInfo[idx2].state = FINGER_STATE_ACTIVE;
                        pFingerPool->tFingerInfo[idx2].power= pCellPath->Path[idx].PathPower;
						pFingerPool->tFingerInfo[idx2].position = pCellPath->Path[idx].PathOffset;
                        FindFlag = 1;
                        break;
                    }
                    
                }

                /* add new finger*/
                if(0 == FindFlag)
                {
                    for(idx2=0;idx2<PM_MAX_FINGER_NUM*2;idx2++)
                    {
                        if(FINGER_STATE_NACTIVE == pFingerPool->tFingerInfo[idx2].state)
                        {
                            pFingerPool->FingerNum ++;
                            pFingerPool->tFingerInfo[idx2].state = FINGER_STATE_ACTIVE;
                            pFingerPool->tFingerInfo[idx2].power= pCellPath->Path[idx].PathPower;
							pFingerPool->tFingerInfo[idx2].position = pCellPath->Path[idx].PathOffset;
                            break;
                        }                    
                    }
                }
            }        

            /*remove nactive finger*/
        for(idx2=0;idx2<PM_MAX_FINGER_NUM*2;idx2++)
        {
            if(FINGER_STATE_ACTIVE != pFingerPool->tFingerInfo[idx2].state)
            {
                pFingerPool->tFingerInfo[idx2].state = FINGER_STATE_NACTIVE;
                pFingerPool->tFingerInfo[idx2].power= 0;
                pFingerPool->tFingerInfo[idx2].position= 0xffff;
            }                    
        }            

        if(pFingerPool->FingerNum > PM_MAX_FINGER_NUM)
        {
            printf(" finger management err--fingernum: %d\n", pFingerPool->FingerNum);
        }
    }
    

    return;
    
}




//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  get the SFN value.

 Function GetSfn.

 @param     pIn =   the output of decoder.
 @return    SFN value.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************

U16 GetSfn(U8 *pIn) 
{
    U16 SFN = 0;

    //extract the 11 bits SFN prime information
    SFN = (*pIn ) ;
    SFN <<= 8;

    SFN += *(pIn+1);
    SFN >>= 5;
    SFN <<= 1;

    return SFN;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  Set the tcb list done flag, it is a callback function 

 Function Umtsnmm_task_list_doneCB

 @param     thisList =   tcb list index.
 @param     param =   the flag variable.
 @return    NONE.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void Umtsnmm_task_list_doneCB(TASKLIST* thisList, void* param)
{
    _ASSERT_PTR(thisList);

    *((U32*)param) = 1;
#ifdef RT_MODE
	{
		U32 listNum;
		listNum = RexGetListID(thisList) - PATHSEARCH_TASK_LIST_ID;
		MLogTask(PID_TASKLIST_EXEC_UMTSNMM_9+listNum, RESOURCE_LARM, umtsnmm_task_list_time[listNum], GET_TICKS);
	}
#endif
}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  run a appointed tcb list  

 Function Umtsnmm_runTaskList

 @param     listNum =   tcb list index.
 @return    NONE.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void Umtsnmm_runTaskList(U32 listNum)
{
    U32 taskListRet;
    PTASKLIST pTaskList = NULL;

    if (listNum > UMTSNMM_BCH_TCBLIST_NUM)
    {
        Umtsnmm_dbg_printf(("nmm_runTaskList: Invalid listNum: %d\r\n", listNum));
        return;
    }

    pTaskList = umtsnmm_task_list[listNum];

    umtsnmm_task_list_done[listNum] = 0;
#ifdef RT_MODE
    umtsnmm_task_list_time[listNum] = GET_TICKS;
#endif
    umtsnmm_task_list_count[listNum]++;

    taskListRet = ExeTSchedList(pTaskList);
    if(taskListRet != MX_OK)
    {
        Umtsnmm_dbg_printf(("ERROR: Task list %d returned %d\r\n", listNum, taskListRet));
        return;
    }
    while(umtsnmm_task_list_done[listNum] == 0);
    ReinitTaskList (pTaskList, NULL);
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/*
 @brief  transfer some paremeters to CEVA for path searcher  

 Function Umtsnmm_PathSearch_tcb
 
 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
PTCB Umtsnmm_PathSearch_tcb(PTASKLIST pTskLst,S16 Ant0Input[],S8 PscCode[],U8 AntNum,U8 NcLen,U32 PDPAnt0New[],U32 PDPAnt1New[],
	U32 PDPAnt0His[],U32 PDPAnt1His[],CellPathInfo* pCellPath,U16 WinLen,U32 *p8xAnt0,U32 *p8xAnt1,U32 *pCom2AntPk,
	U8 FirstFlag,U32 Count,U8 thresholdFactor,PTCB * dep_tcb,U16 RefillFlag, PTCB refill_tcb)
{

    PTCB pTCB = 0;
    U32* pContext = 0;

  //  nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if (RefillFlag == TCBIOBUFFILL)
    {
#ifdef USE_TCBQ
       pTCB = RexCreateTCBQ(TASK_PATH_SEARCH, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL,TIME_UMTSNMMLOOP);
#else
	pTCB = CreateTCB(TASK_PATH_SEARCH, RESOURCE_CEVA, NULL, 5 * sizeof(U32), NULL, NULL);
#endif

	pContext = (U32*) pTCB->ContextPtr;
	pContext[0] = Count;
	pContext[1] = FirstFlag;
	pContext[2] = NcLen;
      pContext[3] = thresholdFactor;
      pContext[4] = tCLPathSearchInfo.FreOffsetAngle;


	CreateTCBIOBuf(pTCB, 8);
	FillTCBIOBuf(pTCB, (U32*) Ant0Input,           2560*2*2 * sizeof(S16), IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PscCode,          512*2, IN_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PDPAnt0New,          WinLen*2*4, INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) PDPAnt0His,           WinLen*2*4, INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) p8xAnt0,           (WinLen*8+27)*4, INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) pCom2AntPk,           WinLen*8*4, INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) pCellPath,          sizeof(CellPathInfo), INOUT_BUF, NULL);
	FillTCBIOBuf(pTCB, (U32*) tCLPathSearchInfo.pstanTable,  1024, INOUT_BUF, NULL);
        

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
        pContext[0] = Count;
        pContext[1] = FirstFlag;
        pContext[2] = NcLen;
        pContext[3] = thresholdFactor;
        pContext[4] = tCLPathSearchInfo.FreOffsetAngle;
      
        ReFillTCBIOBuf(pTCB, (U32*) Ant0Input,           2560*2*2 * sizeof(S16), IN_BUF,  0);
        ReFillTCBIOBuf(pTCB, (U32*) PscCode,          512*2, IN_BUF, 1);
        ReFillTCBIOBuf(pTCB, (U32*) PDPAnt0New,          WinLen*2*4, INOUT_BUF,2);
        ReFillTCBIOBuf(pTCB, (U32*) PDPAnt0His,           WinLen*2*4, INOUT_BUF, 3);
        ReFillTCBIOBuf(pTCB, (U32*) p8xAnt0,           (WinLen*8+27)*4, INOUT_BUF,4);
        ReFillTCBIOBuf(pTCB, (U32*) pCom2AntPk,           WinLen*8*4, INOUT_BUF, 5);
        ReFillTCBIOBuf(pTCB, (U32*) pCellPath,          sizeof(CellPathInfo), INOUT_BUF, 6);
        ReFillTCBIOBuf(pTCB, (U32*) tCLPathSearchInfo.pstanTable,  1024, INOUT_BUF, 7);
    }
    
   // nmm_dbg_printf_off(("done.\r\n"));

	return pTCB;
}




//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  transfer some paremeters to CEVA for interpolation.  

 Function Umtsnmm_CLInterpolation_tcb.

 @param     pTskLst =   tcb list pointer.
 @param     pIn2X =   input data of 2X sample.
 @param     pOut8X =   output data of 8X sample.
 @param     Len =   the input data length.
 @param     refill_tcb =   the pointer of tcb.
 @param     TcbIdx =   the tcb index below a tcb list.
 @return    tcb pointer.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
PTCB Umtsnmm_CLInterpolation_tcb(PTASKLIST pTskLst,S16 *pIn2X, S16 *pOut8X,U32 Len,PTCB * dep_tcb,U16 RefillFlag, PTCB 
refill_tcb,U8 TcbIdx)
{
    PTCB pTCB = 0;
    U32* pContext = 0;

  //  nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if (RefillFlag == TCBIOBUFFILL)
    {
#ifdef USE_TCBQ
       pTCB = RexCreateTCBQ(TASK_CL_INTERPOLATION, RESOURCE_CEVA, NULL, 1 * sizeof(U32), NULL, NULL,TIME_UMTSNMMLOOP);
#else
		pTCB = CreateTCB(TASK_CL_INTERPOLATION, RESOURCE_CEVA, NULL, 1 * sizeof(U32), NULL, NULL);
#endif

		pContext = (U32*) pTCB->ContextPtr;
		pContext[0] = Len;


		CreateTCBIOBuf(pTCB, 2);
		FillTCBIOBuf(pTCB, pIn2X, Len*2*sizeof(S16), IN_BUF, NULL);
		FillTCBIOBuf(pTCB, pOut8X,Len*8*sizeof(S16), INOUT_BUF, NULL);
        

#if UMTSNMM_USE_TCB_LIST
        AddTCBToList(pTCB, pTskLst, NULL);
        //AddDependency(pTCB, 1, dep_tcb[TcbIdx]);
#else
        ExeTSchedSingle(pTCB);
        pTCB = NULL;
#endif
    }
    else 
    {
        pTCB = refill_tcb;

		pContext = (U32*) pTCB->ContextPtr;
		pContext[0] = Len;

		ReFillTCBIOBuf(pTCB, pIn2X,  Len*2*sizeof(S16), IN_BUF, 0);
		ReFillTCBIOBuf(pTCB, pOut8X,  Len*8*sizeof(S16), INOUT_BUF, 1);
    }
    
   // nmm_dbg_printf_off(("done.\r\n"));
    
	
	return pTCB;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  transfer some paremeters to CEVA for bch demodulation processing.  

 Function Umtsnmm_CLDeModSchedule_tcb.

 @param     pTskLst =   tcb list pointer.
 @param     pInfo =   the struct point of some control parameters.
 @param     refill_tcb =   the pointer of tcb.
 @param     TcbIdx =   the tcb index below a tcb list.
 @return    tcb pointer.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
PTCB Umtsnmm_CLDeModSchedule_tcb(PTASKLIST pTskLst,T_CLDeModuTcbInfo *pInfo,PTCB * dep_tcb,
    U16 RefillFlag, PTCB refill_tcb,U8 TcbIdx)
{
    U32* pContext = 0;
    U32   dContextLen = 0;
    U8 idx;
    U8 SampRate=8;
    U32 ConValue;
    U32 offset;
    PTCB pTcb=0;

    //  nmm_dbg_printf_off(("> %s... ", __FUNCTION__));

    if (RefillFlag == TCBIOBUFFILL)
    {
        dContextLen = sizeof(U32)*10;
#ifdef USE_TCBQ
        pTcb = RexCreateTCBQ(TASK_CL_DEMODE, RESOURCE_CEVA, NULL, dContextLen, NULL, NULL,TIME_UMTSNMMLOOP);
#else
        pTcb = CreateTCB(TASK_CL_DEMODE, RESOURCE_CEVA, NULL, dContextLen, NULL, NULL);
#endif

		if(pInfo)
		{
			pContext = (U32*)pTcb->ContextPtr;	
			pContext[0] = pInfo->ucTSCount;
			pContext[1] = SampRate;
			pContext[2] = pInfo->FreAngle;
			pContext[3] = pInfo->ucFingerNum;
			pContext[4] = pInfo->symIdx;
			
			for(idx=0;idx<pInfo->ucFingerNum;idx++)
			{
				ConValue = (pInfo->PathOffset[idx] << 16) + pInfo->FingerWeight[idx];
				pContext[5+idx] = ConValue;
			}
		}


        CreateTCBIOBuf(pTcb,9);
        // 13*2: due to the interpolation filter
        offset = pInfo->symIdx*(INERT_DATA_LEN*2*8+26*2)+INTERPOLATION_8X_FILLED_NUM*2;
        FillTCBIOBuf(pTcb, NULL, INERT_DATA_LEN*16*sizeof(S16),IN_BUF, NULL); 

        FillTCBIOBuf(pTcb, NULL, 2560*2 , IN_BUF, NULL); 
        FillTCBIOBuf(pTcb, NULL,  BCCH_TTI10MS_LEN*sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTcb, NULL, 256*4, IN_BUF, NULL);	
        FillTCBIOBuf(pTcb, NULL,FRE_RESIDUAL_ERR_4FRAME_SYM*2*sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTcb, NULL, 256, IN_BUF, NULL);	
        FillTCBIOBuf(pTcb, NULL, 256, IN_BUF, NULL);
        FillTCBIOBuf(pTcb, NULL, PM_MAX_FINGER_NUM*20*sizeof(S16), IN_BUF, NULL);	
        FillTCBIOBuf(pTcb, NULL, PM_MAX_FINGER_NUM*40*sizeof(S16), IN_BUF, NULL);	


#if UMTSNMM_USE_TCB_LIST
        AddTCBToList(pTcb, pTskLst, NULL);
        //AddDependency(pTcb, 1, dep_tcb[TcbIdx]);
#else
        ExeTSchedSingle(pTcb);
        pTcb = NULL;
#endif
    }
    else 
    {
        pTcb = refill_tcb;

		if(pInfo)
		{
			pContext = (U32*)pTcb->ContextPtr;	
			pContext[0] = pInfo->ucTSCount;
			pContext[1] = SampRate;
			pContext[2] = pInfo->FreAngle;
			pContext[3] = pInfo->ucFingerNum;
			pContext[4] = pInfo->symIdx;
			
			for(idx=0;idx<pInfo->ucFingerNum;idx++)
			{
				ConValue = (pInfo->PathOffset[idx] << 16) + pInfo->FingerWeight[idx];
				pContext[5+idx] = ConValue;
			}
		}
        offset = pInfo->symIdx*(INERT_DATA_LEN*2*8+26*2)+INTERPOLATION_8X_FILLED_NUM*2;
        ReFillTCBIOBuf(pTcb, pInfo->pAntData_8X+offset, INERT_DATA_LEN*16*sizeof(S16),IN_BUF, 0); 
        ReFillTCBIOBuf(pTcb, pInfo->pucScrCode, 2560*2 , IN_BUF, 1); 
        ReFillTCBIOBuf(pTcb, pInfo->psPchMrcData,  BCCH_TTI10MS_LEN*sizeof(S16), INOUT_BUF,2);
        ReFillTCBIOBuf(pTcb, pInfo->pstanTable_Q16, 256*4, IN_BUF, 3);	
        ReFillTCBIOBuf(pTcb, pInfo->psFreErrEst_4Frame,FRE_RESIDUAL_ERR_4FRAME_SYM*2*sizeof(S16), INOUT_BUF,4);
        ReFillTCBIOBuf(pTcb, pInfo->pucSpcode_0, 256, IN_BUF, 5);	
        ReFillTCBIOBuf(pTcb, pInfo->pucSpcode_1, 256, IN_BUF, 6);	
        ReFillTCBIOBuf(pTcb, pInfo->psFocDeSpCpich, PM_MAX_FINGER_NUM*20*sizeof(S16), IN_BUF,   7);	
        ReFillTCBIOBuf(pTcb, pInfo->psFocDeSpPccpch, PM_MAX_FINGER_NUM*40*sizeof(S16), IN_BUF,  8);	
        
    }
    
   // nmm_dbg_printf_off(("done.\r\n"));

	return pTcb;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  transfer some paremeters to CEVA for channel estimation&compensation.  

 Function Umtsnmm_CLChECSchedule_tcb.

 @param     pTskLst =   tcb list pointer.
 @param     pInfo =   the struct point of some control parameters.
 @param     refill_tcb =   the pointer of tcb.
 @param     TcbIdx =   the tcb index below a tcb list.
 @return    tcb pointer.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
PTCB Umtsnmm_CLChECSchedule_tcb(PTASKLIST pTskLst,T_CLDeModuTcbInfo *pInfo,PTCB * dep_tcb,
    U16 RefillFlag, PTCB refill_tcb)
{
    U32* pContext = 0;
    U32   dContextLen = 0;

    PTCB pTcb=0;

    if (RefillFlag == TCBIOBUFFILL)
    {
        dContextLen = sizeof(U32)*2;
#ifdef USE_TCBQ
        pTcb = RexCreateTCBQ(TASK_CL_CHANESTCOM, RESOURCE_CEVA, NULL, dContextLen, NULL, NULL,TIME_UMTSNMMLOOP);
#else
        pTcb = CreateTCB(TASK_CL_CHANESTCOM, RESOURCE_CEVA, NULL, dContextLen, NULL, NULL);
#endif

        CreateTCBIOBuf(pTcb,4);
        FillTCBIOBuf(pTcb, NULL,PM_MAX_FINGER_NUM*20*sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTcb, NULL,PM_MAX_FINGER_NUM*40*sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTcb, NULL,  BCCH_TTI10MS_LEN*sizeof(S16), INOUT_BUF, NULL);
        FillTCBIOBuf(pTcb, NULL, PM_MAX_FINGER_NUM*CHANNEL_COM_SLOT_NUM*2*sizeof(S16), INOUT_BUF, NULL);

#if UMTSNMM_USE_TCB_LIST
        AddTCBToList(pTcb, pTskLst, NULL);
        //AddDependency(pTcb, 1, dep_tcb[0]);
#else
        ExeTSchedSingle(pTcb);
        pTcb = NULL;
#endif
    }
    else 
    {
        pTcb = refill_tcb;
		
        pContext = (U32*)pTcb->ContextPtr;	
        pContext[0] = pInfo->ucTSCount;
        pContext[1] = pInfo->ucFingerNum;
        
        ReFillTCBIOBuf(pTcb, pInfo->psFocDeSpCpich,PM_MAX_FINGER_NUM*20*sizeof(S16), INOUT_BUF, 0);
        ReFillTCBIOBuf(pTcb, pInfo->psFocDeSpPccpch,PM_MAX_FINGER_NUM*40*sizeof(S16),  INOUT_BUF,1);
        ReFillTCBIOBuf(pTcb, pInfo->psPchMrcData,  BCCH_TTI10MS_LEN*sizeof(S16),   INOUT_BUF,2);
        ReFillTCBIOBuf(pTcb, pInfo->pChEstVal, PM_MAX_FINGER_NUM*CHANNEL_COM_SLOT_NUM*2*sizeof(S16), INOUT_BUF,3);

    }
    
    return pTcb;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  transfer some paremeters to CEVA for bch deode processing.  

 Function Umtsnmm_CLDeCodeSchedule_tcb.

 @param     pTskLst =   tcb list pointer.
 @param     pInfo =   the struct point of some control parameters.
 @param     refill_tcb =   the pointer of tcb.
 @param     TcbIdx =   the tcb index below a tcb list.
 @return    tcb pointer.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************

PTCB Umtsnmm_CLDeCodeSchedule_tcb(PTASKLIST pTskLst,T_CLDeCodeTcbInfo *pInfo,PTCB * dep_tcb,
    U16 RefillFlag, PTCB refill_tcb)
{
    U32* pContext = 0;
    U32   dContextLen = 0;

    PTCB pTcb=0;

    if (RefillFlag == TCBIOBUFFILL)
    {
        dContextLen = sizeof(U32)*5;
#ifdef USE_TCBQ
        pTcb = RexCreateTCBQ(TASK_CL_DECODE, RESOURCE_CEVA, NULL, dContextLen, NULL, NULL,TIME_UMTSNMMLOOP);
#else
        pTcb = CreateTCB(TASK_CL_DECODE, RESOURCE_CEVA, NULL, dContextLen, NULL, NULL);
#endif

        pContext = (U32*)pTcb->ContextPtr;
        pContext[0] =1;  
        pContext[1] = pInfo->ucDecodeFlag;


        CreateTCBIOBuf(pTcb,7); 
        FillTCBIOBuf(pTcb, NULL, 512 , IN_BUF, NULL);
        FillTCBIOBuf(pTcb, NULL, 540, INOUT_BUF, NULL); 
        FillTCBIOBuf(pTcb, NULL, 540,INOUT_BUF, NULL);
        FillTCBIOBuf(pTcb, NULL, 270*MAXIMUM_FINGER_NUM*sizeof(S16), INOUT_BUF, NULL);

        FillTCBIOBuf(pTcb, NULL, 256*sizeof(S32),INOUT_BUF, NULL);
        FillTCBIOBuf(pTcb, NULL, 256*sizeof(S32), INOUT_BUF, NULL); 
        FillTCBIOBuf(pTcb, NULL, 257*270 , INOUT_BUF, NULL);
            

#if UMTSNMM_USE_TCB_LIST
        AddTCBToList(pTcb, pTskLst, NULL);
        //AddDependency(pTcb, 1, dep_tcb[0]);
#else
        ExeTSchedSingle(pTcb);
        pTcb = NULL;
#endif
    }
    else 
    {
        pTcb = refill_tcb;

        pContext = (U32*)pTcb->ContextPtr;
        pContext[0] =1;  
        pContext[1] = pInfo->ucDecodeFlag;

        ReFillTCBIOBuf(pTcb, pInfo->pcc_3g_dectab_1_2, 512 , IN_BUF, 0);
        ReFillTCBIOBuf(pTcb, pInfo->pPingData, 540, INOUT_BUF, 1); 
        ReFillTCBIOBuf(pTcb, pInfo->pPangData, 540,INOUT_BUF, 2);
        ReFillTCBIOBuf(pTcb, pInfo->pMrc_AllFinger, 270*MAXIMUM_FINGER_NUM*sizeof(S16), INOUT_BUF,  3);
        ReFillTCBIOBuf(pTcb, pInfo->pMetric, 256*sizeof(S32),INOUT_BUF, 4);
        ReFillTCBIOBuf(pTcb, pInfo->pNewMetric, 256*sizeof(S32), INOUT_BUF, 5); 
        ReFillTCBIOBuf(pTcb, pInfo->pTbBuf, 257*270 , INOUT_BUF, 6);
        
    }
    
   // nmm_dbg_printf_off(("done.\r\n"));    

    
	return pTcb;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  transfer some paremeters to CEVA for calculate residual frequency  offset.  

 Function Umtsnmm_CLUpdateReFreErrSchedule_tcb.

 @param     pTskLst =   tcb list pointer.
 @param     pInfo =   the struct point of some control parameters.
 @param     refill_tcb =   the pointer of tcb.
 @param     TcbIdx =   the tcb index below a tcb list.
 @return    tcb pointer.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
PTCB Umtsnmm_CLUpdateReFreErrSchedule_tcb(PTASKLIST pTskLst,T_CLUpdateReFreErrInfo *pInfo,PTCB * dep_tcb,
    U16 RefillFlag, PTCB refill_tcb)
{
	U32* pContext = 0;
	U32   dContextLen = 0;

	PTCB pTcb=0;

//=============================
   if (RefillFlag == TCBIOBUFFILL)
    {
        dContextLen = sizeof(U32)*2;
#ifdef USE_TCBQ
        pTcb = RexCreateTCBQ(TASK_UPDATE_FRE_ERR, RESOURCE_CEVA, NULL, dContextLen, NULL, NULL,TIME_UMTSNMMLOOP);
#else
        pTcb = CreateTCB(TASK_UPDATE_FRE_ERR, RESOURCE_CEVA, NULL, dContextLen, NULL, NULL);
#endif

        pContext = (U32*)pTcb->ContextPtr;
        pContext[0] = pInfo->ucFingerNum;


	CreateTCBIOBuf(pTcb,3); 
	FillTCBIOBuf(pTcb, pInfo->pResiFreErr_4F, FRE_RESIDUAL_ERR_4FRAME_SYM*2*sizeof(S16), INOUT_BUF, NULL);
	FillTCBIOBuf(pTcb, pInfo->pResiFreErrValue, FRE_RESIDUAL_1FRAME_CALC_NUM*4*2*sizeof(S16), INOUT_BUF, NULL);	
	FillTCBIOBuf(pTcb, pInfo->pIQsum, 2*sizeof(S32), INOUT_BUF, NULL);	

#if UMTSNMM_USE_TCB_LIST
        AddTCBToList(pTcb, pTskLst, NULL);
        //AddDependency(pTcb, 1, dep_tcb[0]);
#else
        ExeTSchedSingle(pTcb);
        pTcb = NULL;
#endif
    }
    else 
    {
        pTcb = refill_tcb;

        pContext = (U32*)pTcb->ContextPtr;
        pContext[0] = pInfo->ucFingerNum;

        ReFillTCBIOBuf(pTcb, pInfo->pResiFreErr_4F, FRE_RESIDUAL_ERR_4FRAME_SYM*2*sizeof(S16), INOUT_BUF,0);
        ReFillTCBIOBuf(pTcb, pInfo->pResiFreErrValue, FRE_RESIDUAL_1FRAME_CALC_NUM*4*2*sizeof(S16), INOUT_BUF,1);
		ReFillTCBIOBuf(pTcb, pInfo->pIQsum, 2*sizeof(S32), INOUT_BUF, 2);	
    }
    
   // nmm_dbg_printf_off(("done.\r\n"));    

	return pTcb;
}




//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  initialize some global variable and allocate buffer for BCH decoder  tcb list.  

 Function Umtsnmm_BchTcbInit.

 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void Umtsnmm_BchTcbInit(void)
{
    U32  loop;
    
    /*TCBs  for path search*/
    if (umtsnmm_task_list[PATHSEARCH_TASK_LIST] == NULL)
    {
        umtsnmm_task_list[PATHSEARCH_TASK_LIST] = CreateTaskList(PATHSEARCH_TASK_LIST_ID, NULL);
        _ASSERT_PTR(umtsnmm_task_list[PATHSEARCH_TASK_LIST]);
        RegListDoneCB(umtsnmm_task_list[PATHSEARCH_TASK_LIST], Umtsnmm_task_list_doneCB,  (void*)(umtsnmm_task_list_done+PATHSEARCH_TASK_LIST));
    }
    umtsnmm_tcb_arr[PATHSEARCH_TASK_LIST][0] = Umtsnmm_PathSearch_tcb(umtsnmm_task_list[PATHSEARCH_TASK_LIST], 
                    NULL,NULL, 0,0, NULL, NULL,NULL,NULL,NULL,0,NULL,NULL,NULL,0,0,0,umtsnmm_tcb_arr[PATHSEARCH_TASK_LIST],TCBIOBUFFILL, NULL);


/*TCBs for interpolation*/
    if (umtsnmm_task_list[INTERPOLATION_TASK_LIST] == NULL) 
    {
        umtsnmm_task_list[INTERPOLATION_TASK_LIST] = CreateTaskList(INTERPOLATION_TASK_LIST_ID, NULL);
        _ASSERT_PTR(umtsnmm_task_list[INTERPOLATION_TASK_LIST]);
        RegListDoneCB(umtsnmm_task_list[INTERPOLATION_TASK_LIST], Umtsnmm_task_list_doneCB,   (void*)(umtsnmm_task_list_done+INTERPOLATION_TASK_LIST));
    }   

    for(loop=0;loop<UMTSNMM_BCH_TCB_MAXIMUM;loop++)
    {
        umtsnmm_tcb_arr[INTERPOLATION_TASK_LIST][loop] =   Umtsnmm_CLInterpolation_tcb(umtsnmm_task_list[INTERPOLATION_TASK_LIST], NULL, NULL,  0, 
            umtsnmm_tcb_arr[INTERPOLATION_TASK_LIST], TCBIOBUFFILL,  NULL,loop);
    }


/*TCBs for demodulation*/
    if (umtsnmm_task_list[CLDEMODE_TASK_LIST] == NULL) 
    {
        umtsnmm_task_list[CLDEMODE_TASK_LIST] = CreateTaskList(CLDEMODE_TASK_LIST_ID, NULL);
        _ASSERT_PTR(umtsnmm_task_list[CLDEMODE_TASK_LIST]);
        RegListDoneCB(umtsnmm_task_list[CLDEMODE_TASK_LIST], Umtsnmm_task_list_doneCB,   (void*)(umtsnmm_task_list_done+CLDEMODE_TASK_LIST));
    }   

    for(loop=0;loop<UMTSNMM_BCH_TCB_MAXIMUM;loop++)
    {
        umtsnmm_tcb_arr[CLDEMODE_TASK_LIST][loop] =   Umtsnmm_CLDeModSchedule_tcb(umtsnmm_task_list[CLDEMODE_TASK_LIST], NULL, 
            umtsnmm_tcb_arr[CLDEMODE_TASK_LIST], TCBIOBUFFILL,  NULL,loop);
    }


    /*TCBs  for channel estimation*/
    if (umtsnmm_task_list[CLChEC_TASK_LIST] == NULL)
    {
        umtsnmm_task_list[CLChEC_TASK_LIST] = CreateTaskList(CLChEC_TASK_LIST_ID, NULL);
        _ASSERT_PTR(umtsnmm_task_list[CLChEC_TASK_LIST]);
        RegListDoneCB(umtsnmm_task_list[CLChEC_TASK_LIST], Umtsnmm_task_list_doneCB,  (void*)(umtsnmm_task_list_done+CLChEC_TASK_LIST));
    }
    umtsnmm_tcb_arr[CLChEC_TASK_LIST][0] = Umtsnmm_CLChECSchedule_tcb(umtsnmm_task_list[CLChEC_TASK_LIST], 
                    NULL,umtsnmm_tcb_arr[CLChEC_TASK_LIST],TCBIOBUFFILL, NULL);
    

    /*TCBs  for BCH decoder*/
    if (umtsnmm_task_list[CLDECODE_TASK_LIST] == NULL)
    {
        umtsnmm_task_list[CLDECODE_TASK_LIST] = CreateTaskList(CLDECODE_TASK_LIST_ID, NULL);
        _ASSERT_PTR(umtsnmm_task_list[CLDECODE_TASK_LIST]);
        RegListDoneCB(umtsnmm_task_list[CLDECODE_TASK_LIST], Umtsnmm_task_list_doneCB,  (void*)(umtsnmm_task_list_done+CLDECODE_TASK_LIST));
    }
    umtsnmm_tcb_arr[CLDECODE_TASK_LIST][0] = Umtsnmm_CLDeCodeSchedule_tcb(umtsnmm_task_list[CLDECODE_TASK_LIST], 
                    NULL,umtsnmm_tcb_arr[CLDECODE_TASK_LIST],TCBIOBUFFILL, NULL);
    

    /*TCBs  for calculate residual frequency error*/
    if (umtsnmm_task_list[UPDATE_RE_FRE_ERR_TASK_LIST] == NULL)
    {
        umtsnmm_task_list[UPDATE_RE_FRE_ERR_TASK_LIST] = CreateTaskList(UPDATE_RE_FRE_ERR_TASK_LIST_ID, NULL);
        _ASSERT_PTR(umtsnmm_task_list[UPDATE_RE_FRE_ERR_TASK_LIST]);
        RegListDoneCB(umtsnmm_task_list[UPDATE_RE_FRE_ERR_TASK_LIST], Umtsnmm_task_list_doneCB,  (void*)(umtsnmm_task_list_done+UPDATE_RE_FRE_ERR_TASK_LIST));
    }
    umtsnmm_tcb_arr[UPDATE_RE_FRE_ERR_TASK_LIST][0] = Umtsnmm_CLUpdateReFreErrSchedule_tcb(umtsnmm_task_list[UPDATE_RE_FRE_ERR_TASK_LIST], 
                    NULL,umtsnmm_tcb_arr[UPDATE_RE_FRE_ERR_TASK_LIST],TCBIOBUFFILL, NULL);
    

    //nmm_dbg_printf_off(("@ %s\tARM %d\tnmm_tcb_arr %x %x\ttime %lu\r\n", __FUNCTION__, MxGetCpuID(), umtsnmm_tcb_arr[0][0], umtsnmm_tcb_arr[0][1], (unsigned long) GET_TICKS));

    return;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  free some global variable and  buffer for BCH decoder tcb list .  

 Function Umtsnmm_BchTcbStop.

 @return  none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void Umtsnmm_BchTcbStop(void)
{
    U8 i;
    for (i = 0; i < UMTSNMM_BCH_TCBLIST_NUM; i++)
    {
        if (umtsnmm_task_list[i])
        {
            DestroyTaskList(umtsnmm_task_list[i]);
            umtsnmm_task_list[i] = NULL;
        }
    }

    return;
}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  report BCH decoder result and fill message header.  

 Function DecodeDataRpt.

 @param     outMsg =   the buffer of report message body.
 @param     MsgBodyLen =   the message body length.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void DecodeDataRpt(U8 *outMsg, U16 MsgBodyLen)
{
    ApiHeader *pApi;
    //UINT32* pMsg = NULL;
    MXRC rc = MX_OK;


    pApi = (ApiHeader *) outMsg;

   // ApiCreate(pApi, API_ENCAP_FM, 0x8019, 2, API_TYPE_IND,
    //          0x5129);
	
    pApi->Control   = 0x10000004;
    pApi->DstInstID = 2;
    pApi->SrcInstID = 0x8019;
    pApi->Length    = MsgBodyLen;        // The payload is not present yet
    pApi->Type      = API_TYPE_IND;
    pApi->MessageID = NWL_BCH_DATA_IND;

    // The command size was increased to the total size of parameter
    pApi->Length = MsgBodyLen + sizeof(ApiHeader);
    rc = SvsrSendAPI(pApi);
	if(MX_OK != rc)
	{
		printf("Api send error:%d",rc);
	}

    return;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  configure tcb list and run it for path searcher.  

 Function PathSearchTskList.

 @param     pAnt0Input =   input data of antenna 0 (2X sample).
 @param     PDPAnt0New =   the PDP of antenna 0(output).
 @param     PDPAnt1New =   null.
 @param     PDPAnt0His =   the history of PDP of antenna 0.
 @param     p8xAnt0 =   the interpolation result of PDP of antenna 0.
 @param     p8xAnt1 =   null.
 @param     pCom2AntPk =   the peak of combine of twno antennas, if only one 
                 antenna, is the peak of antenna 0.
 @param     ScrCode =   scramble code.
 @param     thresholdFactor =  the threshold factor .
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void PathSearchTskList(S16* pAnt0Input,U32 PDPAnt0New[],U32 PDPAnt1New[],U32 PDPAnt0His[],U32 PDPAnt1His[],CellPathInfo* pCellPath,
	U32 *p8xAnt0,U32 *p8xAnt1,U32 *pCom2AntPk,S8 *ScrCode,U8 thresholdFactor)
{
    Umtsnmm_PathSearch_tcb(umtsnmm_task_list[PATHSEARCH_TASK_LIST],pAnt0Input,ScrCode,1,tCLPathSearchInfo.NcLen,PDPAnt0New,PDPAnt1New,
        PDPAnt0His,PDPAnt1His,pCellPath,PATH_SEARCH_WIN_LEN*2,p8xAnt0,p8xAnt1,pCom2AntPk,tCLPathSearchInfo.FirstFlag,
        tCLPathSearchInfo.PSSlotCnt,thresholdFactor,NULL,TCBIOBUFREFILL, umtsnmm_tcb_arr[PATHSEARCH_TASK_LIST][0]);

    Umtsnmm_runTaskList(PATHSEARCH_TASK_LIST);

    return;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  configure tcb list and run it for interpolation.  

 Function CLInterpolationTskList.

 @param     pIn =   input data (2X sample).
 @param     pOut =   the output data (8X sample).
 @param     DataLen =   interpolate data length.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CLInterpolationTskList(S16 *pIn, S16 *pOut, U32 DataLen)
{
    U8 idx;
    U32 offset8x;
    U32 offset2x;


    for(idx = 0; idx < 10; idx++)
    {
        offset8x = idx*(INERT_DATA_LEN*2*8+26*2);
        offset2x = idx*(INERT_DATA_LEN*2*2);
        Umtsnmm_CLInterpolation_tcb(umtsnmm_task_list[INTERPOLATION_TASK_LIST], (pIn+offset2x),(pOut+offset8x),DataLen, 
                NULL, TCBIOBUFREFILL, umtsnmm_tcb_arr[INTERPOLATION_TASK_LIST][idx],  idx);

	}

    Umtsnmm_runTaskList(INTERPOLATION_TASK_LIST);

    return;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  configure tcb list and run it for bch demodulation processing.  

 Function CLInterpolationTskList.

 @param     pInfo =   the pointer of T_CLDeModuTcbInfo struct.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CLDeModTskList(T_CLDeModuTcbInfo *pInfo)
{
    U32 i;
    for(i = 0;i < 10; i++)
    {
        pInfo->symIdx = i;
        Umtsnmm_CLDeModSchedule_tcb(umtsnmm_task_list[CLDEMODE_TASK_LIST], pInfo, 
                NULL, TCBIOBUFREFILL, umtsnmm_tcb_arr[CLDEMODE_TASK_LIST][i],i);
    }

    Umtsnmm_runTaskList(CLDEMODE_TASK_LIST);

	return;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  configure tcb list and run it for channel estimation&compensation.  

 Function CLChanECTskList.

 @param     pInfo =   the pointer of T_CLDeModuTcbInfo struct.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CLChanECTskList(T_CLDeModuTcbInfo *pInfo)
{
    Umtsnmm_CLChECSchedule_tcb(umtsnmm_task_list[CLChEC_TASK_LIST], pInfo, 
            NULL, TCBIOBUFREFILL, umtsnmm_tcb_arr[CLChEC_TASK_LIST][0]);

    Umtsnmm_runTaskList(CLChEC_TASK_LIST);
    return;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  configure tcb list and run it for bch decode processing.  

 Function CLDeCodeTsk.

 @param     pInfo =   the pointer of T_CLDeCodeTcbInfo struct.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CLDeCodeTsk(T_CLDeCodeTcbInfo *pInfo)
{
    Umtsnmm_CLDeCodeSchedule_tcb(umtsnmm_task_list[CLDECODE_TASK_LIST], pInfo, 
            NULL, TCBIOBUFREFILL, umtsnmm_tcb_arr[CLDECODE_TASK_LIST][0]);

    Umtsnmm_runTaskList(CLDECODE_TASK_LIST);
    return;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  configure tcb list and run it for update residual frequency offset.  

 Function CLUpdateReFreErrTsk.

 @param    pAntDataIn_2X =   input data.
 @param    pScrCode =   scrambling code.
 @param    ptCellPathInfo =   path search result.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void PathSearchTaskSch(S16 *pAntDataIn_2X,S8 *pScrCode,CellPathInfo *ptCellPathInfo)
{
	U32 *pPDPAnt0Hist = tCLPathSearchInfo.pPDPAnt0Hist;
	U32 *pPDPAnt1Hist = tCLPathSearchInfo.pPDPAnt1Hist;

	U32 *pPDPAnt0 = tCLPathSearchInfo.pPDPAnt0;
	U32 *pPDPAnt1 = tCLPathSearchInfo.pPDPAnt1;
	
	U32 *pAnt0_8x = tCLPathSearchInfo.pAnt0_8x;
	U32 *pAnt1_8x = tCLPathSearchInfo.pAnt1_8x;
	U32 *pComb2AntPk = tCLPathSearchInfo.pComb2AntPk;
      U8 thresholdFactor = tCLPathSearchInfo.threshold;
	pPDPAnt0Hist[0]=0;
	pPDPAnt0Hist[1]=0;
	pPDPAnt0Hist[2]=0;
	pPDPAnt0Hist[3]=0;
	pPDPAnt0Hist[4]=0;
	pPDPAnt0Hist[5]=0;
	pPDPAnt1Hist[0]=0;
	pPDPAnt1Hist[1]=0;
	pPDPAnt1Hist[2]=0;
	pPDPAnt1Hist[3]=0;
	pPDPAnt1Hist[4]=0;
	pPDPAnt1Hist[5]=0; 
	
	
	PathSearchTskList(pAntDataIn_2X, pPDPAnt0,pPDPAnt1,pPDPAnt0Hist,pPDPAnt1Hist,ptCellPathInfo,
		pAnt0_8x,pAnt1_8x,pComb2AntPk,pScrCode,thresholdFactor);

		
	return;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  configure tcb list and run it for update residual frequency offset.  

 Function CLUpdateReFreErrTsk.

 @param     pInfo =   the pointer of T_CLUpdateReFreErrInfo struct.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CLUpdateReFreErrTsk(T_CLUpdateReFreErrInfo *pInfo)
{
    Umtsnmm_CLUpdateReFreErrSchedule_tcb(umtsnmm_task_list[UPDATE_RE_FRE_ERR_TASK_LIST], pInfo, 
            NULL, TCBIOBUFREFILL, umtsnmm_tcb_arr[UPDATE_RE_FRE_ERR_TASK_LIST][0]);

    Umtsnmm_runTaskList(UPDATE_RE_FRE_ERR_TASK_LIST);
    
    return;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  callback function 

 Function ScheCLBchThread.

 @param     none.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void ScheCLBchThread(LPVOID pParam)
{
	MxScheduleThread(tCLBchCtxt.hThread);
	return;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  DMA transfer data for path search processing

 Function DMA_PathSearch.

 @param    pSrc = source address.
 @param    pDec = destination address.
 @param    DataSize = data size.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void DMA_PathSearch(S16 *pSrc,S16 *pDec,U32 DataSize)
{
    tCLBchCtxt.ThreadCtx ->ProFlag = THREAD_PATH_SEARCH;
    tCLBchCtxt.pMdma = MdmaAllocDesc(MDMA_HW_SYS,1);
    _ASSERT_PTR (tCLBchCtxt.pMdma );
     MdmaSetCopy(tCLBchCtxt.pMdma );
    MdmaSetData(tCLBchCtxt.pMdma ,  pSrc, DataSize, pDec,DataSize);
    MdmaRunEx(MDMA_HW_SYS,tCLBchCtxt.pMdma , ScheCLBchThread, NULL);

    return;
}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  free some global variable and  buffer for BCH decoder  modul once  finished.  

 Function Umtsnmm_BchTcbStop.
 
 @param     none.
 @return  none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CLBchStop(void)
{
    /*------------------------------path search-----------------------------------*/
    MemFree(tCLPathSearchInfo.pPDPAnt0Hist,CRAM_MEM);
    MemFree(tCLPathSearchInfo.pPDPAnt1Hist,CRAM_MEM);
    MemFree(tCLPathSearchInfo.pPDPAnt0,CRAM_MEM);
    MemFree(tCLPathSearchInfo.pPDPAnt1,CRAM_MEM);
    MemFree(tCLPathSearchInfo.pAnt0_8x,CRAM_MEM);
    MemFree(tCLPathSearchInfo.pAnt1_8x,CRAM_MEM);
    MemFree(tCLPathSearchInfo.pComb2AntPk,CRAM_MEM);
    MemFree(tCLPathSearchInfo.ptCellPathInfo,CRAM_MEM);
    
    /*---------------------------------deModule------------------------------------*/
    MemFree(tCLDeModuInfo.pucScrCode, CRAM_MEM);
    MemFree(tCLDeModuInfo.pAnteDataInCRam_2X , CRAM_MEM);
    MemFree(tCLDeModuInfo.pAntData_8X, CRAM_MEM);
    MemFree(tCLDeModuInfo.psPchMrcData , CRAM_MEM);
    MemFree(tCLDeModuInfo.psDeSpData  , CRAM_MEM);
    MemFree(tCLDeModuInfo.psFreErrEst_4Frame , CRAM_MEM);
    MemFree(tCLDeModuInfo.pstanTable_Q16 , CRAM_MEM);
    MemFree(tCLDeModuInfo.pChEstVal , CRAM_MEM);
    MemFree(tCLDeModuInfo.pucSpcode_0,CRAM_MEM);
    MemFree(tCLDeModuInfo.pucSpcode_1,CRAM_MEM);
    MemFree(tCLDeModuInfo.psFocDeSpCpich,CRAM_MEM);
    MemFree(tCLDeModuInfo.psFocDeSpPccpch,CRAM_MEM);
    
    /* -------------------------------decode use---------------------------------------*/
    MemFree(tCLDeCodeInfo.pPingData, CRAM_MEM);
    MemFree(tCLDeCodeInfo.pPangData, CRAM_MEM);
    MemFree(tCLDeCodeInfo.pcc_3g_dectab_1_2, CRAM_MEM);

    MemFree(tCLDeCodeInfo.pMetric , CRAM_MEM);
    MemFree(tCLDeCodeInfo.pNewMetric, CRAM_MEM);
    MemFree(tCLDeCodeInfo.pTbBuf, CRAM_MEM);

    MemFree(tCLBchCtxt.ThreadCtx,DDR_MEM);

    MemFree(tclUpdateReFreErr.pResiFreErrValue, CRAM_MEM);
    MemFree(tclUpdateReFreErr.pIQsum, CRAM_MEM);

	return;
}




//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  thread processing function of BCH decode thread  

 Function CLBchThreadCBFun.

 @param     none.
 @return  none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CLBchThreadCBFun(LPVOID pParam)
{
    U32 idx,idx2;
    S16 *pDec;
    S16 *pSrc;
    U32 DataSize = 0;
    S16 ResidualFreErrAngle = 0;
    U32  dReceCrc;
    U32	dCalcCrc;
//    U32 dErrCode ;
    U32 Temp;
	S16* p2xIn;
	PMDMA_FDESC pMdmaNext;
    T_3G_BCH_DATA_IND *ptBchDataInd = NULL;
	float angle;

	if(gBchStopReadyFlag)
	{
		tCLBchCtxt.ThreadCtx->ProFlag = THREAD_INVAILABLE;
	}

    ptBchDataInd =(T_3G_BCH_DATA_IND*)&aOutBuff[sizeof(ApiHeader)];

    DataSize =  (TS_CHIPS_NUM+gWinLen)*8; /* 8 =2*sizeof(S16)*ucSamRate*/

	/* just run once, to get the path info */
	if(THREAD_PATH_SEARCH == tCLBchCtxt.ThreadCtx->ProFlag)
	{
	      if(tCLPathSearchInfo.PSSlotCnt<tCLPathSearchInfo.NcLen)
            {
                PathSearchTaskSch(tCLDeModuInfo.pAnteDataInCRam_2X,pucScrCodeAddr+tCLPathSearchInfo.PSSlotCnt*TS_CHIPS_NUM*2, 
                                            tCLPathSearchInfo.ptCellPathInfo);

                tCLPathSearchInfo.PSSlotCnt ++;

                pSrc = GET_ANT_DATA_ADDR +tCLPathSearchInfo.PSSlotCnt*TS_CHIPS_NUM*4;
                pDec = tCLDeModuInfo.pAnteDataInCRam_2X;

                /*================ transer data for demodulation===================== */
                tCLBchCtxt.pMdma = MdmaAllocDesc(MDMA_HW_SYS,1);
                _ASSERT_PTR (tCLBchCtxt.pMdma );
                MdmaSetCopy(tCLBchCtxt.pMdma );

                MdmaSetData(tCLBchCtxt.pMdma,  pSrc, DataSize, pDec,DataSize);
                MdmaRunEx(MDMA_HW_SYS,tCLBchCtxt.pMdma, ScheCLBchThread, NULL);
            }

           if(tCLPathSearchInfo.PSSlotCnt >= tCLPathSearchInfo.NcLen)
            {
                tCLPathSearchInfo.PSSlotCnt = 0;
                tCLPathSearchInfo.FirstFlag = 0;
                tCLBchCtxt.ThreadCtx->ProFlag = THREAD_CELLLISTEN_DEMODULE;

                /* make finger management*/
                UpdateFinger(&tFingerPool,tCLPathSearchInfo.ptCellPathInfo,gFirstPathSearchFlag);

                /*must clear the buffer*/
                memset(&tCLPathSearchInfo.ptCellPathInfo->Path[0],0,sizeof(PathInfo)*MAXIMUM_FINGER_NUM);

                gFirstPathSearchFlag = 0;

                tCLDeModuInfo.ucFingerNum = 0;
                for(idx =0;idx<PM_MAX_FINGER_NUM*2;idx++)
                {
                    if(FINGER_STATE_ACTIVE == tFingerPool.tFingerInfo[idx].state)
                    {
                        #if 0
					uart_printf(" demodularion: path offset: %d, pow:%d  \n\r",tFingerPool.tFingerInfo[idx].position,
						tFingerPool.tFingerInfo[idx].power);
                        #endif
                        tCLDeModuInfo.PathOffset[tCLDeModuInfo.ucFingerNum] =  tFingerPool.tFingerInfo[idx].position;
                        tCLDeModuInfo.FingerPower[tCLDeModuInfo.ucFingerNum] = tFingerPool.tFingerInfo[idx].power;
                        tCLDeModuInfo.ucFingerNum ++;
                    }

                    if(tCLDeModuInfo.ucFingerNum >= PM_MAX_FINGER_NUM)
                    {
                        uart_printf(" demodularion module get finger num over maximum! \n\r");
                        break;
                    }
                }

			/* resort the path */
			if(1 < tCLDeModuInfo.ucFingerNum)
			{
				for(idx=0;idx<tCLDeModuInfo.ucFingerNum;idx++)
				{
					for(idx2 = 0;idx2<(tCLDeModuInfo.ucFingerNum -idx);idx2++)
					{
						if(tCLDeModuInfo.FingerPower[idx2] < tCLDeModuInfo.FingerPower[idx2+1])
						{
							Temp = tCLDeModuInfo.FingerPower[idx2];
							tCLDeModuInfo.FingerPower[idx2] = tCLDeModuInfo.FingerPower[idx2+1];
							tCLDeModuInfo.FingerPower[idx2+1] = Temp;
			
							Temp = tCLDeModuInfo.PathOffset[idx2];
							tCLDeModuInfo.PathOffset[idx2] = tCLDeModuInfo.PathOffset[idx2+1];
							tCLDeModuInfo.PathOffset[idx2+1] = Temp;
						}
					}
				}
			}


                for(idx=0;idx<tCLDeModuInfo.ucFingerNum;idx++)
            	{
            		tCLDeModuInfo.FingerWeight[idx] = 1;
            	}
				tCLBchCtxt.pMdma = MdmaAllocDesc(MDMA_HW_SYS,10);
				_ASSERT_PTR (tCLBchCtxt.pMdma );
				pMdmaNext = tCLBchCtxt.pMdma;
				for(idx = 0;idx < 10; idx++)
				{
					pSrc =GET_ANT_DATA_ADDR + idx*256*4 ;
					p2xIn = tCLDeModuInfo.pAnteDataInCRam_2X+idx*(INERT_DATA_LEN*2)*2;
					pDec = p2xIn+INTERPOLATION_2X_FILLED_NUM*2;
					memset((U8*)p2xIn,0,INTERPOLATION_2X_FILLED_NUM*4);
				
					MdmaSetCopy(pMdmaNext ); 
					MdmaSetData(pMdmaNext,	pSrc, (256+64)*4*sizeof(S16), pDec,(256+64)*4*sizeof(S16)); 			
					pMdmaNext = pMdmaNext->Next;
	
				}
				
				MdmaRunEx(MDMA_HW_SYS,tCLBchCtxt.pMdma, ScheCLBchThread, NULL); 


            }
	}
	else if(THREAD_CELLLISTEN_DEMODULE == tCLBchCtxt.ThreadCtx->ProFlag)
	{
		tCLDeModuInfo.pucScrCode = pucScrCodeAddr + (tCLDeModuInfo.ucTSCount%FRAME_SLOT_NUM)*TS_CHIPS_NUM*2;

        CLInterpolationTskList(tCLDeModuInfo.pAnteDataInCRam_2X,tCLDeModuInfo.pAntData_8X,INERT_DATA_LEN*2);

		CLDeModTskList(&tCLDeModuInfo);

        /*  channel estimation and compensation */
        CLChanECTskList(&tCLDeModuInfo);
		
		tCLDeModuInfo.dTCBCount++;
		tCLDeModuInfo.ucTSCount++;
		
		tCLBchCtxt.ThreadCtx->SlotCnt ++;			

		/* ========================decode ==============*/
		if((1 == (tCLDeModuInfo.ucTSCount %15)) && (1 != (tCLDeModuInfo.ucTSCount)))
		{
			tCLDeCodeInfo.ucFingerNum = tCLDeModuInfo.ucFingerNum;
			tCLDeCodeInfo.ucDecodeFlag= ((1 == tCLDeModuInfo.ucTSCount %30)? 1:0);
			
			CLDeCodeTsk(&tCLDeCodeInfo);
			//uart_printf("decode task :time slot count: %d  \n", tCLDeModuInfo.ucTSCount);
			
			/* =============== must clear the buffer ============================*/
			memset(tCLDeModuInfo.psPchMrcData, 0, BCCH_TTI10MS_LEN*sizeof(S16));

            if(1== tCLDeCodeInfo.ucDecodeFlag)
            {
                /* ===========	   check crc ====== ============================================ */
                /* the crc squence need to inversed */
                dReceCrc = ProGetCRCOfTBBlock((U8*)tCLDeCodeInfo.pPangData,BCH_TRBK_LEN,BCH_CRC_LEN);
                dCalcCrc = Bit_Calc_crc16((U8*)tCLDeCodeInfo.pPangData,BCH_TRBK_LEN);

                dCalcCrc &= 0xffff;
                if(dReceCrc != dCalcCrc)
                {
                    if(1 == tCLDeCodeInfo.ucTTIBoundaryNoOK)
                    {
                        /* save the 2nd frame result of 2nd-deinterleaving for decode*/ 
                        memcpy(tCLDeCodeInfo.pPangData,tCLDeCodeInfo.pPangData+ BCCH_TTI10MS_LEN,BCCH_TTI10MS_LEN);

                        /* save the result of 1st frame of residual frequency offset */
                        pDec = tCLDeModuInfo.psFreErrEst_4Frame;
                        pSrc = pDec + FRE_RESIDUAL_ERR_1FRAME_SYM*2;
                        memcpy(pDec,pSrc,FRE_RESIDUAL_ERR_1FRAME_SYM*4);

                        /* update the slot count*/
                        tCLDeModuInfo.ucTSCount -= 15;

	        			tCLBchCtxt.pMdma = MdmaAllocDesc(MDMA_HW_SYS,10);
	        			_ASSERT_PTR (tCLBchCtxt.pMdma );
	        			pMdmaNext = tCLBchCtxt.pMdma;
	        			for(idx = 0;idx < 10; idx++)
	        			{
	        				pSrc =GET_ANT_DATA_ADDR + idx*256*4 ;
	        				p2xIn = tCLDeModuInfo.pAnteDataInCRam_2X+idx*(INERT_DATA_LEN*2)*2;
	        	            pDec = p2xIn+INTERPOLATION_2X_FILLED_NUM*2;
	        				memset((U8*)p2xIn,0,INTERPOLATION_2X_FILLED_NUM*4);
	        			
	        				MdmaSetCopy(pMdmaNext ); 
	        				MdmaSetData(pMdmaNext,	pSrc, (256+64)*4*sizeof(S16), pDec,(256+64)*4*sizeof(S16));				
	        				pMdmaNext = pMdmaNext->Next;

	        			}
	        			
	        			MdmaRunEx(MDMA_HW_SYS,tCLBchCtxt.pMdma, ScheCLBchThread, NULL); 


                        /* will report crc error at odd frame  */
                        if(1 == tCLDeCodeInfo.ucReportFlag)
                        {
                                tCLDeCodeInfo.ucReportFlag=0;
                            	return;
                        }
                        else
                        {
                            tCLDeCodeInfo.ucReportFlag++;
                        }
                    }
                    
                    ptBchDataInd->crc =  CRC_FAILED;
                    ptBchDataInd->sfn =  0xffff;
                    ptBchDataInd->DataLen = 0;

					gTTICounter++;

                    uart_printf(" slot counter %d, TTI counter:%d, crc err \n\r",tCLDeModuInfo.ucTSCount,gTTICounter);
					
                }
                else
                {
                    memset(ptBchDataInd->data,0,sizeof(aOutBuff));
                    ConjBitToByte((U8*)tCLDeCodeInfo.pPangData,ptBchDataInd->data,BCH_TRBK_LEN);

                    ptBchDataInd->crc =  CRC_OK;
                    ptBchDataInd->sfn =  GetSfn(ptBchDataInd->data);
                    ptBchDataInd->DataLen = 31;
                    ptBchDataInd->psc = gPsc;
                    ptBchDataInd->numTx = 1;
					gTTICounter++;

					tCLDeCodeInfo.ucTTIBoundaryNoOK = 0;
					
                    uart_printf("\t TTI Counter:%d, crc success! sfn:%d, psc:%d.\n\r",
						gTTICounter,ptBchDataInd->sfn,gPsc);
                }

                DecodeDataRpt((U8*)(&aOutBuff[0]),ptBchDataInd->DataLen +2);	
            }

			//gdErrCode = ERR_CODE_DECODE_ERR;
		}

            if(((tCLDeCodeInfo.BchDecodeTtiNum *30 +1) <= tCLDeModuInfo.ucTSCount) 
				|| ((tCLDeCodeInfo.BchDecodeTtiNum*30+16) <= tCLDeModuInfo.dTCBCount))
            {
				uart_printf(" decoder quit. Ts:%d, Tcb counter:%d.\n\r",tCLDeModuInfo.ucTSCount,tCLDeModuInfo.dTCBCount);
                tCLBchCtxt.ThreadCtx->ProFlag = THREAD_INVAILABLE;
				gTTICounter = 0;
                NWL_GenerateAPIMsg(NWL_BCH_CONFIG_CNF,NWL_ERR_OK,API_TYPE_IND);
            }
		
		/*============ update residual frequency err============== */
            if(0 == (tCLDeModuInfo.ucTSCount % 60))
            {
                tclUpdateReFreErr.ucFingerNum = tCLDeModuInfo.ucFingerNum;
                CLUpdateReFreErrTsk(&tclUpdateReFreErr);

				angle = (float)atan((float)(*(tclUpdateReFreErr.pIQsum+1))/(float)(*(tclUpdateReFreErr.pIQsum)));
				 ResidualFreErrAngle=(S16)(angle*0x1000);


			/* must  clear the buffer */
			memset((U8*)tCLDeModuInfo.psFreErrEst_4Frame,0,FRE_RESIDUAL_ERR_4FRAME_SYM*2*sizeof(S16));
						
		//	printf(" Update frequency offset! -- old:%d, Res: %d.\n\r",tCLDeModuInfo.FreAngle,ResidualFreErrAngle);

			if((12867<(ResidualFreErrAngle+ tCLDeModuInfo.FreAngle)) || ((-12867)>(ResidualFreErrAngle+ tCLDeModuInfo.FreAngle)))
			{
				ResidualFreErrAngle = 0;
			}

                tCLDeModuInfo.FreAngle = (ResidualFreErrAngle/2+ tCLDeModuInfo.FreAngle);
				//tCLDeModuInfo.FreAngle = ResidualFreErrAngle+ tCLDeModuInfo.FreAngle;
            }

		/*========== make a path search each 4 frames =====================*/
		if(0 == (tCLDeModuInfo.ucTSCount % 60))
		{
			/* transfer data to CRAM use DMA for demodulation*/
			pSrc = GET_ANT_DATA_ADDR;
			pDec = tCLDeModuInfo.pAnteDataInCRam_2X;
			
			DMA_PathSearch(pSrc,pDec,PS_DATA_LEN);
		}
		else
		{
			
			tCLBchCtxt.pMdma = MdmaAllocDesc(MDMA_HW_SYS,10);
			_ASSERT_PTR (tCLBchCtxt.pMdma );
			pMdmaNext = tCLBchCtxt.pMdma;
			for(idx = 0;idx < 10; idx++)
			{
				pSrc =GET_ANT_DATA_ADDR + idx*256*4 ;
				p2xIn = tCLDeModuInfo.pAnteDataInCRam_2X+idx*(INERT_DATA_LEN*2)*2;
	            pDec = p2xIn+INTERPOLATION_2X_FILLED_NUM*2;
				memset((U8*)p2xIn,0,INTERPOLATION_2X_FILLED_NUM*4);
			
	      		//	memcpy(pDec,pSrc,(256+64)*4*sizeof(S16));
				MdmaSetCopy(pMdmaNext ); 
				MdmaSetData(pMdmaNext,	pSrc, (256+64)*4*sizeof(S16), pDec,(256+64)*4*sizeof(S16));				
				pMdmaNext = pMdmaNext->Next;

			}
			
			MdmaRunEx(MDMA_HW_SYS,tCLBchCtxt.pMdma, ScheCLBchThread, NULL); 
		
		}


	}
    else if(THREAD_INVAILABLE == tCLBchCtxt.ThreadCtx->ProFlag)
    {
    	gBchDecodeBusyFlag = 0;
        return;
    }
    else
    {
        gdErrCode = ERR_CODE_TASK_ERR;
    }

    return;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  dma counter used for one dma transfer data by multi-times.  

 Function UpdateDmaCounter.
 
 @param  none.
 @return  none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void UpdateDmaCounter(LPVOID pParam)
{
	DmaCnt ++;
	return;
}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  start bch decoder by DMA.  

 Function CLBchStartDMA.
 
 @param     Index = the index of arrary which set by cell searcher .
 @param     Psc = primary scrambling code .
  @param     TtiNum = total TTI number which set the decoder processed data   length .
 @return  none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CLBchStartDMA(U16 Index, U16 Psc,U8 TtiNum)
{
    S16 *pSrc;
    S16 *pDec;
    U32 DataSize;
    NWL_CommInfor  *pInfor = NULL;
    U8 idx;
    U8 OffsetSlotNum;
	
    PMDMA_FDESC pMdma;
    
#if 0
    //gPSC = pscIdx;
    for(idx=0;idx<10;idx++)
    {
        pInfor = (pNwlCommInfo + idx);
        if((1 ==pInfor->SetFlag) && (Psc == pInfor->Psc))
        {
            GetFlag = 1;
            break;
        }
    }

    if(1 != GetFlag)
    {
        printf("Can't get PSC: %d \n", Psc);
        return;
    }
#endif

    pInfor = pNwlCommInfo+Index;
#if 0
	tCLDeModuInfo.pucScrCode = pInfor->pPscBuf;

#else
	DmaCnt = 0;
	for(idx=0;idx<15;idx++)
	{
		pMdma = MdmaAllocDesc(MDMA_HW_SYS,1);
		_ASSERT_PTR (pMdma );
		 MdmaSetCopy(pMdma );
		 
		MdmaSetData(pMdma, (pInfor->pPscBuf+idx*5120), 
			        5120, (tCLDeModuInfo.pucScrCode+idx*5120),5120);
		MdmaRunEx(MDMA_HW_SYS,pMdma, UpdateDmaCounter, NULL);
	}

	
	while(1)
	{
		if(15 <= DmaCnt)
		{
			DmaCnt =0;
			break;
		}
	}
#endif

    pucScrCodeAddr = tCLDeModuInfo.pucScrCode;

    // get the angle of initial frequency offset
    //tCLDeModuInfo.FreAngle = pInfor->InitAngle;
    tCLDeModuInfo.FreAngle = pInfor->InitAngle /FOC_CHIPS_SEG_NUM;
    
    tCLPathSearchInfo.FreOffsetAngle = pInfor->InitAngle;

    gPsc = Psc;
    OffsetSlotNum = (15 - pInfor->SlotNum)%15;
    gBeginDataOffset = pInfor->WinBegin*2 + OffsetSlotNum*TS_CHIPS_NUM*4;
    gWinLen = PATH_SEARCH_WIN_LEN*2;


    DataSize = PS_DATA_LEN + gWinLen*8; //2X *2(IQ)*2(S16) 
    pSrc = tCLDeModuInfo.pAnteDataInDdr + gBeginDataOffset;

    pDec = tCLDeModuInfo.pAnteDataInCRam_2X;

    tCLPathSearchInfo.threshold = pInfor->thresholdFactor;

    DMA_PathSearch(pSrc,pDec,DataSize);

	tCLDeCodeInfo.BchDecodeTtiNum = TtiNum;

   // printf("path search data len: %d \n", DataSize);

    return;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  transfer data for bch demodulation processing

 Function StartDMA_DeModu.

 @param    none.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void StartDMA_DeModu(void)
{
    S16 *pSrc;
    S16 *pDec;
    U32 DataSize;

    DataSize = (TS_CHIPS_NUM+gWinLen)*8;
    pSrc = GET_ANT_DATA_ADDR;
    pDec = tCLDeModuInfo.pAnteDataInCRam_2X;


    tCLBchCtxt.ThreadCtx ->ProFlag = THREAD_CELLLISTEN_DEMODULE;
    tCLBchCtxt.pMdma = MdmaAllocDesc(MDMA_HW_SYS,1);
    _ASSERT_PTR (tCLBchCtxt.pMdma );
     MdmaSetCopy(tCLBchCtxt.pMdma );
    MdmaSetData(tCLBchCtxt.pMdma,  pSrc, DataSize, pDec,DataSize);
    MdmaRunEx(MDMA_HW_SYS,tCLBchCtxt.pMdma, ScheCLBchThread, NULL);

    return;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  initialize the bch decoder thread.  

 Function UMTS_Nwl_BCH_decode_thread_init.
 
 @param  none.
 @return  none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
MXRC UMTS_Nwl_BCH_decode_thread_init(void)
{
    MXRC rc = MX_OK;
//    U8 result;

    rc |= MxCreateThreadEx(0, (TENTRY)CLBchThreadCBFun, NULL, NULL, 0,
                           &(tCLBchCtxt.hThread));
    
    if (FAILED(rc))
    {
        MxRemoveThread(tCLBchCtxt.hThread);
        tCLBchCtxt.hThread = HNULL;
	  //  result = 0;
    }
    else
	{
		//result = 1;
	}	

    return rc;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  kill the bch decoder thread.  

 Function UMTS_Nwl_BCH_decode_thread_kill.
 
 @param  none.
 @return  none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void UMTS_Nwl_BCH_decode_thread_kill()
{    
    
    if (HANDLE_NOT_NULL(tCLBchCtxt.hThread))
    {
        MxRemoveThread(tCLBchCtxt.hThread);
    }        
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  initialize some global variable and allocate buffer for BCH decoder  modul.  

 Function Umtsnmm_BchTcbInit.
 
 @param  none.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void CLBchInit(void)
{
    printf("cell listen init start \n");

    gBchDecodeStartFlag = 1;
    gBchDecodeBusyFlag = 1;
    gBchStopReadyFlag = 0;

    /* =======================alloc memory in DDR========================  */
    tCLDeModuInfo.pAnteDataInDdr = gUMTSNwlAntData0;

    /*====================path search ===================================*/
    tCLPathSearchInfo.pPDPAnt0Hist = (U32*) MemAlloc((PM_SEARCH_WIN_LEN*SAMPLE_RATE+12)*sizeof(U32), CRAM_MEM, 1);
    tCLPathSearchInfo.pPDPAnt1Hist = (U32*) MemAlloc((PM_SEARCH_WIN_LEN*SAMPLE_RATE+12)*sizeof(U32), CRAM_MEM, 1);

    tCLPathSearchInfo.pPDPAnt0 = (U32*) MemAlloc((PM_SEARCH_WIN_LEN*SAMPLE_RATE)*sizeof(U32), CRAM_MEM, 1);

    tCLPathSearchInfo.pPDPAnt1 = (U32*) MemAlloc((PM_SEARCH_WIN_LEN*SAMPLE_RATE)*sizeof(U32), CRAM_MEM, 1);

    tCLPathSearchInfo.pAnt0_8x= (U32*) MemAlloc((PM_SEARCH_WIN_LEN*8+27)*sizeof(U32), CRAM_MEM, 1);
    tCLPathSearchInfo.pAnt1_8x= (U32*) MemAlloc((PM_SEARCH_WIN_LEN*8+27)*sizeof(U32), CRAM_MEM, 1);
    tCLPathSearchInfo.pComb2AntPk= (U32*) MemAlloc((PM_SEARCH_WIN_LEN*8)*sizeof(U32), CRAM_MEM, 1);
    tCLPathSearchInfo.ptCellPathInfo = (CellPathInfo*)MemAlloc(sizeof(CellPathInfo), CRAM_MEM, 1);
	

    tCLPathSearchInfo.pPDPAnt0Hist[0]=0;
    tCLPathSearchInfo.pPDPAnt0Hist[1]=0;
    tCLPathSearchInfo.pPDPAnt0Hist[2]=0;
    tCLPathSearchInfo.pPDPAnt0Hist[3]=0;
    tCLPathSearchInfo.pPDPAnt0Hist[4]=0;
    tCLPathSearchInfo.pPDPAnt0Hist[5]=0;
    tCLPathSearchInfo.pPDPAnt1Hist[0]=0;
    tCLPathSearchInfo.pPDPAnt1Hist[1]=0;
    tCLPathSearchInfo.pPDPAnt1Hist[2]=0;
    tCLPathSearchInfo.pPDPAnt1Hist[3]=0;
    tCLPathSearchInfo.pPDPAnt1Hist[4]=0;
    tCLPathSearchInfo.pPDPAnt1Hist[5]=0;

    tCLPathSearchInfo.PSSlotCnt = 0;
    tCLPathSearchInfo.NcLen = PM_ACC_SLOT_NUM;
    tCLPathSearchInfo.FirstFlag = 1;

	/*---------------------------------deModule------------------------------------*/
	/*====================== alloc memory in CRAM ========================== */
    tCLDeModuInfo.pAnteDataInCRam_2X = (S16*)MemAlloc((INERT_DATA_LEN*2)*2*sizeof(S16)*10, CRAM_MEM, 0);

    tCLDeModuInfo.pucScrCode = (S8*)MemAlloc(FRAME_CHIPS_NUM*2, CRAM_MEM, 0);
    tCLDeModuInfo.pAntData_8X = (S16*)MemAlloc((INERT_DATA_LEN*8*2*sizeof(S16)+26*4)*10, CRAM_MEM, 1);
    tCLDeModuInfo.psPchMrcData= (S16*) MemAlloc(BCCH_TTI10MS_LEN*sizeof(S16), CRAM_MEM, 1);

    tCLDeModuInfo.psDeSpData = (S16*) MemAlloc(BCCH_TTI10MS_LEN*sizeof(S16), CRAM_MEM, 1);

    /*calc frequence err */
    tCLDeModuInfo.psFreErrEst_4Frame = (S16*) MemAlloc(FRE_RESIDUAL_ERR_4FRAME_SYM*2*sizeof(S16), CRAM_MEM, 1); 

    tCLDeModuInfo.pucSpcode_0= (S8*)MemAlloc(256,CRAM_MEM,0);
    memset(tCLDeModuInfo.pucSpcode_0, 1,256);

    tCLDeModuInfo.pucSpcode_1= (S8*)MemAlloc(256,CRAM_MEM,0);
    memset(tCLDeModuInfo.pucSpcode_1, 1,128);
    memset(tCLDeModuInfo.pucSpcode_1+128, -1,128);

    /*used to store after FOC chips data, one TS*/
    tCLDeModuInfo.pChEstVal= (S16*)MemAlloc(PM_MAX_FINGER_NUM*CHANNEL_COM_SLOT_NUM*2*sizeof(S16),CRAM_MEM,1);
    tCLDeModuInfo.psFocDeSpCpich= (S16*)MemAlloc(PM_MAX_FINGER_NUM*20*sizeof(S16),CRAM_MEM,1);
    tCLDeModuInfo.psFocDeSpPccpch= (S16*)MemAlloc(PM_MAX_FINGER_NUM*40*sizeof(S16),CRAM_MEM,1);

    tCLDeModuInfo.pstanTable_Q16 = (S16*)MemAlloc(1024,CRAM_MEM,0);
    memcpy(tCLDeModuInfo.pstanTable_Q16,&astanTable_Q16[0][0],1024);

    /* =================update residual frequency err ==============*/
    tclUpdateReFreErr.pResiFreErr_4F=tCLDeModuInfo.psFreErrEst_4Frame;
    tclUpdateReFreErr.pResiFreErrValue = (S16*)MemAlloc(FRE_RESIDUAL_1FRAME_CALC_NUM*FRE_RESIDUAL_FRAME_NUM*2*sizeof(S16), CRAM_MEM, 1);
    tclUpdateReFreErr.pIQsum= (S32*)MemAlloc(2*sizeof(S32), CRAM_MEM, 1);
    /* -------------------------------decode use---------------------------------------*/
    tCLDeCodeInfo.pPingData = (S8*) MemAlloc(BCCH_TTI10MS_LEN*2, CRAM_MEM, 1);
    tCLDeCodeInfo.pPangData=  (S8*) MemAlloc(BCCH_TTI10MS_LEN*2, CRAM_MEM, 1);
    tCLDeCodeInfo.pMrc_AllFinger = tCLDeModuInfo.psPchMrcData;

    tCLDeCodeInfo.pcc_3g_dectab_1_2 =  (S8*) MemAlloc(256*2, CRAM_MEM, 1);
    memcpy(tCLDeCodeInfo.pcc_3g_dectab_1_2,&cc_3g_dectab_1_2[0][0],512);

    tCLDeCodeInfo.pMetric = (S32*) MemAlloc(256*sizeof(S32), CRAM_MEM, 1);
    tCLDeCodeInfo.pNewMetric= (S32*) MemAlloc(256*sizeof(S32), CRAM_MEM, 1);
    tCLDeCodeInfo.pTbBuf = (U8*) MemAlloc(257*270, CRAM_MEM, 1);

    /*==============================create thread=============================*/
    tCLBchCtxt.ThreadCtx = (T_CLBchThreadCtx *) MemAlloc(sizeof(T_CLBchThreadCtx), DDR_MEM, 1);

    /*====================== initial the global varible ==============================*/
     tCLBchCtxt.ThreadCtx->ProFlag = 0;
     tCLBchCtxt.ThreadCtx->SlotCnt = 0;
     tCLBchCtxt.ThreadCtx->PkIdx = 0;
    tCLBchCtxt.ThreadCtx->SlotCnt = 0;

     tCLDeModuInfo.dTCBCount = 0;
     tCLDeModuInfo.ucTSCount = 0;
     tCLDeModuInfo.ucSampRate = 2;
     tCLDeCodeInfo.ucTTIBoundaryNoOK = 1;
     tCLDeCodeInfo.ucCrcErrFlag = 0;
	 tCLDeCodeInfo.ucReportFlag=1;
     
     tCLPathSearchInfo.pstanTable = tCLDeModuInfo.pstanTable_Q16;

    return;

}




//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  bch decoder stop request processing function.  

 Function CLBchStopMsgPro.
 
 @param  none.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
U32 	CLBchStopMsgPro(void)
{
    gBchStopReadyFlag = 1;

    CLBchStop();
    gBchDecodeStartFlag = 0;
    NWL_GenerateAPIMsg(NWL_STOP_BCH_CNF, NWL_ERR_OK,API_TYPE_IND);
    return 0;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief just for VC send test data , save data into ddr memory  

 Function GetCLBchAntData.
 
 @param  DataBuf = input data.
 @param  DataSz = data size.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void GetCLBchAntData(U8 *DataBuf, U32 DataSz)
{
    U8 *Dec;

    Dec = (U8*)tCLDeModuInfo.pAnteDataInDdr;
    Dec = Dec+tCLDeModuInfo.AntOffset;
    memcpy(Dec,DataBuf,DataSz);
    tCLDeModuInfo.AntOffset += DataSz;

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
void GetCLScrCode(U8 *DataBuf, U32 DataSz)
{
    U8 *Dec;

    Dec = (U8*)tCLDeModuInfo.pucScrCode;
    Dec = Dec+tCLDeModuInfo.ScrCodeOffset;
    memcpy(Dec,DataBuf,DataSz);
    tCLDeModuInfo.ScrCodeOffset += DataSz;
    return;
}


                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
