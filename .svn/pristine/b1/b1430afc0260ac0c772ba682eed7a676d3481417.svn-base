//-------------------------------------------------------------------------------------------
/** @file UMTSNwlMsgPro.c
 *
 * @brief API message processer 
 * @author Mindspeed Technologies
 * @version $Revision: 1.3.4.8 $
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

#include "typedef.h"
#include "apimgr.h"
#include "Debug.h"
#include "UMTSNwlMsgPro.h"
#include "UMTSNwlCellSearchCtrl.h"
#include "ext_phy.h"
#include "prototypes.h"
#include "stream.h"
#include "globalstoragearrays.h"
#include "LteBsApiThread.h"
#include "4gmx_thread.h"
#include "ltertconfig.h"


#ifdef RT_MODE
#include "ad9361radio.h"
#endif

#include "UMTSNwlBchDecod.h"
#include "UMTSNwlCellSearchArm.h"


S8	 *gUMTSNwlPscBuf;


NWL_CommInfor  *pNwlCommInfo;
NWL_CELLSRCHINFO *pCellSrchMsg;

/*TestMac*/
static LTE_PHY_API_THREAD_CTX lte_bs_phy_umtsnmm_api_ctx;


U8    g3GNwlStartFlag = 0;
U8    gCellSrcStartFlag = 0;
U8    gBchDecodeStartFlag = 0;
U8    gCellSearchBusyFlag = 0;
U8 	  gBchDecodeBusyFlag = 0;

U8 *UMTSNwl_RX_data_addr;
S16  	*gUMTSNwlAntData0; // for ant0
S16 *           UMTSNwl_iq_storage = NULL;

typedef struct _tagUMTS_NWL_THREAD_CTX {
    HANDLE          hNWLPhyThread;

} UMTS_NWL_THREAD_CTX, *PUMTS_NWL_THREAD_CTX;

static UMTS_NWL_THREAD_CTX NwlPhyCtx;

U32      UMTSNwl_iq_storage_curr_idx;
U32	UMTSNwlRadioType = 0;
volatile U32 UMTSNwlRadioStatus = 0;

volatile NCNBVAR U16 UMTSNwl_iq_subframe_store_done;
U32  UMTSNwl_subframe_count = 0;

volatile U16 CellSearchNum = 0;
volatile U16 CellSearchPsc[MAX_PSC_LIST];
 U8 gmeasurementPeriod = 0;

 U8 gUmtsnmmMode=0xff;

extern void CellSrchAntDataRev(U8 *DataBuf, U32 DataSz);
extern void CellSrchPscSeqRev(U8 *DataBuf, U32 DataSz);
extern void GetCLBchAntData(U8 *DataBuf, U32 DataSz);
extern void GetCLScrCode(U8 *DataBuf, U32 DataSz);

#ifndef _WIN32
typedef struct tPhyInitCtx
{
    SYSCORECFG cfg;
    FEC_CONFIG_REGS FecConfigRegs;
}UMTSNWL_PHYINITCTX, *UMTSNWL_PPHYINITCTX;

UMTSNWL_PHYINITCTX UMTSNwl_phyInitCtx;
#endif


PUMTS_NWL_THREAD_CTX UMTSNwl_get_ctx()
{
    return &NwlPhyCtx;
}


U32 NWL_3GNwlInitCheck(U8* pIn)
{
    U32 ErrCode = NWL_ERR_OK;

    T_3G_INIT_NWL *IntMsg =(T_3G_INIT_NWL*)pIn;
    
    if(0x2 != IntMsg->Supported_RAT)
    {
        ErrCode = NWL_ERR_SYSTEM_ERR;
        printf("NWL init message system parameter err: %d",IntMsg->Supported_RAT );
        return ErrCode;
}

    if((DI_CPRI_EMULATOR != IntMsg->mode) && (DI_PCIE != IntMsg->mode))
    {
        ErrCode = NWL_ERR_SYSTEM_ERR;
        printf("NWL init message mode parameter err: %d",IntMsg->mode);
        return ErrCode;
    }

    return ErrCode;
}


U32 NWL_cellSearchReqCheck(U8* pIn)
{
    U32 ErrCode = NWL_ERR_OK;
    U32 PscNum;
    U32 i;
    T_3G_CELL_SEARCH_REQ *pMsg = NULL;

    pMsg = (T_3G_CELL_SEARCH_REQ*)pIn;


    if(0 == pMsg->nr_pscList)
    {
        PscNum = MAX_PSC_NUM;
    }
    else if(1 == pMsg->exhaustiveSearch)
    {
        PscNum = MAX_PSC_NUM;
    }
    else
    {
        PscNum = pMsg->nr_pscList;
        if(PscNum > MAX_PSC_LIST)
        {
            printf(" Cell Search Psc Nr Err:%d \n ", PscNum);
            ErrCode = NWL_ERR_PARA_ERR;
            return ErrCode;
        }
    }

    /*check the parameters */
    if(MAX_PSC_NUM != PscNum)
    {
        for(i=0;i<PscNum;i++)
        {
            if(MAX_PSC_NUM <= pMsg->pciList[i])
            {
                printf(" Cell Search Psc Nr Err:%d \n ", PscNum);
                ErrCode = NWL_ERR_PARA_ERR;
                return ErrCode;
            }
        }
    }

    if(0 != gCellSrcStartFlag)
    {
        ErrCode = NWL_ERR_CELLSRC_START_ALREADY;
        return ErrCode;
    }
    pCellSrchMsg->meaPeriod = pMsg->measurementPeriod;
	pCellSrchMsg->pscNum = PscNum;
	if(MAX_PSC_NUM != PscNum)
	{
		memcpy(pCellSrchMsg->psc,pMsg->pciList,PscNum*sizeof(U16));
	}
    return ErrCode;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief   cell search request message processing function

 Function NWL_cellSearchReqPro.

 @param    pIn =  message buffer.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
U32 NWL_cellSearchReqPro(U8* pIn)
{
    U32 ErrCode = NWL_ERR_OK;
    U32 PscNum;
    U32 i;
    T_3G_CELL_SEARCH_REQ *pMsg = NULL;

    pMsg = (T_3G_CELL_SEARCH_REQ*)pIn;

    if(0 == pMsg->nr_pscList)
    {
        PscNum = MAX_PSC_NUM;
    }
    else if(1 == pMsg->exhaustiveSearch)
    {
        PscNum = MAX_PSC_NUM;
    }
    else
    {
        PscNum = pMsg->nr_pscList;
    }

    UMTSNwlRadioType = 1;
    UMTSNwlRadioStatus = 1;
	UMTSNwl_subframe_count = 0;

    if(MAX_PSC_NUM == PscNum)
    {
        CellSearchNum = 0xffff;
        gmeasurementPeriod = pMsg->measurementPeriod;
       printf("Triger Cell Search \n");
    }
    else
    {
        CellSearchNum = PscNum;
        for(i=0;i<PscNum;i++)
        {
            gmeasurementPeriod = pMsg->measurementPeriod;
            CellSearchPsc[i]= pMsg->pciList[i];
           printf("Triger Cell Search \n");
        }
    }
     return ErrCode;   
}




U32 NWL_cellSearchStopCheck(U8* pIn)
{
    U32 ErrCode = NWL_ERR_OK;
    //T_3G_STOP_CELL_SEARCH_REQ *pMsg = NULL;

    //pMsg = (T_3G_STOP_CELL_SEARCH_REQ*)pIn;


    if(1 != gCellSrcStartFlag)
    {
        ErrCode = NWL_ERR_CELLSRC_STOP_ALREADY;
        return ErrCode;
    }

	if(1 == gCellSearchBusyFlag)
	{
		ErrCode = NWL_ERR_CELL_SEARCHER_BUSY;
		return ErrCode;
	}
	
    return ErrCode;
}






U32 NWL_BchConReqCheck(U8* pIn)
{
    U32 idx;
    U32 ErrCode = NWL_ERR_OK;
    T_3G_BCH_CONFIG_REQ *pMsg = NULL;
    NWL_CommInfor  *pInfor = NULL;
    U32 ProFlag = 0;


    pMsg = (T_3G_BCH_CONFIG_REQ*)pIn;

    /*check the parameters */
    if(pMsg->psc >= MAX_PSC_NUM)
    {
        printf(" Bch configure Psc Nr Err:%d \n ", pMsg->psc);
        ErrCode = NWL_ERR_PARA_ERR;
        return ErrCode;
    }

	if(pMsg->DecodeTtiNum > 64)
	{
        printf(" Bch decode TTI number Err:%d \n ", pMsg->DecodeTtiNum);
        ErrCode = NWL_ERR_PARA_ERR;
        return ErrCode;
	}

    for(idx=0;idx<NWL_MAX_PSC;idx++)
    {
        pInfor = (pNwlCommInfo + idx);
        if(1 == pInfor->SetFlag)
        {
            if(pInfor->Psc == pMsg->psc)
            {
                ProFlag = 1;
                break;
            }
        }
        else
        {
            break;
        }
    }

    if(1 != ProFlag)
    {
        printf(" Bch can not find matched PSC:  %d \n ", pMsg->psc);
        ErrCode = NWL_ERR_PSC_NOT_MATCH;
        return ErrCode;
    }

    if(0 != gBchDecodeStartFlag)
    {
        ErrCode = NWL_ERR_BCH_START_ALREADY;
        return ErrCode;
    }

     return ErrCode;   
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief   BCH decode request message processing function

 Function NWL_BchConReqPro.

 @param    pIn =  message buffer.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
U32 NWL_BchConReqPro(U8* pIn)
{
    U16 idx;
    T_3G_BCH_CONFIG_REQ *pMsg = NULL;
    NWL_CommInfor  *pInfor = NULL;
    U32 ProFlag = 0;
    U16 Psc = 0xffff;;


    pMsg = (T_3G_BCH_CONFIG_REQ*)pIn;
    Psc = pMsg->psc;

    for(idx=0;idx<NWL_MAX_PSC;idx++)
    {
        pInfor = (pNwlCommInfo + idx);
        if(pInfor->Psc == Psc)
        {
            ProFlag = 1;
            break;
        }
    }

    if(1 == ProFlag)
    {
        CLBchStartDMA(idx,Psc,pMsg->DecodeTtiNum);
    }

     return 0;   
}



U32 NWL_BchConStopCheck(U8* pIn)
{
    U32 ErrCode = NWL_ERR_OK;
    //T_3G_STOP_PBCH_REQ *pMsg = NULL;

    //pMsg = (T_3G_STOP_PBCH_REQ*)pIn;


    if(1 != gBchDecodeStartFlag)
    {
        ErrCode = NWL_ERR_BCH_STOP_ALREADY;
        return ErrCode;
    }

	if(1 == gBchDecodeBusyFlag)
	{
		ErrCode = NWL_ERR_BCH_DECODER_BUSY;
		return ErrCode;
	}
    
    return ErrCode;
}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief   send report message to high layer

 Function NWLSendToHost.

 @param    outMsg =  message buffer.
 @param    outMsgSize =  message body size.
 @param    MsgType =  message type.
 @param    MsgId =  message id.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void NWLSendToHost(U32* outMsg, U32 outMsgSize,U32 MsgType,U32 MsgId)
{
    ApiHeader *pApi = NULL;
    pApi = (ApiHeader *) outMsg;

/*
    ApiCreate(pApi, API_ENCAP_FM, pCtx->nInstanceID, pCtx->nMacInstanceId, API_TYPE_IND,
                  PHY_LTE_MSG);
*/  
    pApi->Control   = 0x10000004;
    pApi->DstInstID = 2;  // to host , if to Supervior, the value should be 1
    pApi->SrcInstID = 0x8019;
    pApi->Length    = outMsgSize;        // The payload is not present yet
    pApi->Type      = MsgType;
    pApi->MessageID = MsgId;

    // The command size was increased to the total size of parameter
    SvsrSendAPI(pApi);

    // printf("Api send %d\n", outMsgSize + API_HDR_SIZE);
    return ;
    }


void NWL_GenerateAPIMsg(U32 msgId, U8 status,U32 MsgType)
{
    U32 OutMsgBuf[10];
    U32 OutMsgSize = 0;
    T_3G_CELL_SEARCH_CNF *pCellSrc=NULL; 
    T_3G_STOP_CELL_SEARCH_CNF *pStopSrc = NULL;
    T_3G_BCH_CONFIG_CNF *pBch= NULL;
    T_3G_STOP_PBCH_CNF *pStopBch= NULL;
    

    //msgType = API_TYPE_CFG_RESP;
    //msgType = API_TYPE_IND;

    
    // Switch on msgType
    switch (msgId)
    {
        case NWL_CELL_SEARCH_CNF:
            pCellSrc = (T_3G_CELL_SEARCH_CNF*) ((U8*)(OutMsgBuf)+sizeof(ApiHeader));
            pCellSrc->errorcode = status;

            OutMsgSize = sizeof(T_3G_CELL_SEARCH_CNF);
            break;

        case NWL_STOP_CELL_SEARCH_CNF:
            pStopSrc = (T_3G_STOP_CELL_SEARCH_CNF*) ((U8*)(OutMsgBuf)+sizeof(ApiHeader));
            pStopSrc->errorcode = status;

            OutMsgSize = sizeof(T_3G_STOP_CELL_SEARCH_CNF);
            break;
                
        case NWL_BCH_CONFIG_CNF:
            pBch = (T_3G_BCH_CONFIG_CNF*) ((U8*)(OutMsgBuf)+sizeof(ApiHeader));
            pBch->errorcode = status;

            OutMsgSize = sizeof(T_3G_BCH_CONFIG_CNF);
            break;
            
        case NWL_STOP_BCH_CNF:
            pStopBch = (T_3G_STOP_PBCH_CNF*) ((U8*)(OutMsgBuf)+sizeof(ApiHeader));
            pStopBch->errorcode = status;

            OutMsgSize = sizeof(T_3G_STOP_PBCH_CNF);
            break;

        default:
            printf("NWL response message type err-- msgID:%d, msgType:%d; \n",msgId,MsgType);
            break;
    }


    NWLSendToHost(OutMsgBuf,OutMsgSize,MsgType,msgId);

    return;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief   get antenna data

 Function UMTSNwl_thread_SaveData.

 @param    p_iq =  DDR buffer which save RF data.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void UMTSNwl_thread_SaveData(U8* p_iq)
{
    PUMTS_NWL_THREAD_CTX pThCtx = UMTSNwl_get_ctx();    
    UINT32 t = GET_TICKS;

    _ASSERT((pThCtx != NULL));
    

    if(DI_CPRI_EMULATOR == gUmtsnmmMode)
    {
        UMTSNwl_RX_data_addr = (U8*)QueryCramIqBuf(1,0,0);
    }
    else
    {
        UMTSNwl_RX_data_addr = p_iq;
    }
    
    UMTSNwl_subframe_count ++;
    
	
	//MLogMark(MLOG_MARK_FRAMEBORDER, t);
    //MLogRegisterFrameSubframe(UMTSNwl_subframe_count,0);

    RexNewTTINotification ();
	
	
    MxScheduleThreadEx(pThCtx->hNWLPhyThread, 0, 0);
	
    return;
}



void UMTSNwl_iq_subframe_store_cb(void)
{
    UMTSNwl_iq_subframe_store_done = 1;
    return;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief   the callback function of save antenna data thread

 Function UMTSNwl_SaveDataFun.

 @param    none .
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void UMTSNwl_SaveDataFun(LPVOID pParam)
{
	S16            *sigIn = (S16*)UMTSNwl_RX_data_addr;
    PMDMA_FDESC     pMdma;
    U32 loop=0;
	
    if (!sigIn)
        return;

    if(UMTSNwlRadioStatus == 1)
    {
        PhyDiCopyIqUMTSNMM((PUINT8)UMTSNwl_RX_data_addr);
    	pMdma = MdmaGetSingleDesc(MDMA_HW_SYS); 
    	_ASSERT_PTR(pMdma);
    	MdmaSetCopy(pMdma);
		if(UMTSNwlRadioType == 0)
        {
			MdmaSetData(pMdma, sigIn, 61440, (LPVOID)((UINT32)UMTSNwl_iq_storage + UMTSNwl_iq_storage_curr_idx), 61440);
        }
		if(UMTSNwlRadioType == 1)
		{
			MdmaSetData(pMdma, sigIn, 61440, (LPVOID)((UINT32)gUMTSNwlAntData0 + UMTSNwl_iq_storage_curr_idx), 61440);
		}
    	UMTSNwl_iq_subframe_store_done = 0;
    	MdmaRunEx(MDMA_HW_SYS, pMdma, (void (*)(LPVOID)) UMTSNwl_iq_subframe_store_cb, NULL);
    	while(UMTSNwl_iq_subframe_store_done==0);

    	//MLogTask(2017, RESOURCE_LARM, tTotal, GET_TICKS);

    	UMTSNwl_iq_storage_curr_idx += 61440;
    	    	
		
	}
	
    /* expect to be called every ms */
    
    if ((UMTSNwl_iq_storage_curr_idx/61440) >= 1300) {
        UMTSNwl_iq_storage_curr_idx = 0;        
		UMTSNwlRadioStatus = 2;
    }
	if(2 == UMTSNwlRadioStatus)
	{
		
		CellSrchInit();
		
        if(CellSearchNum == 0xffff)
        {
            CellSrchTrigger(0xffff, gmeasurementPeriod);
        }
        else
        {
            for(loop=0;loop<CellSearchNum;loop++)
            {
                CellSrchTrigger(CellSearchPsc[loop], gmeasurementPeriod);
            }
        }
    	

		UMTSNwlRadioStatus = 3;
	}
		
	

	return ;

}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief   initialize save antenna data thread

 Function UMTS_Nwl_save_data_thread_init.

 @param    none .
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
MXRC UMTS_Nwl_save_data_thread_init()
{
    MXRC            rc = MX_OK;
    PUMTS_NWL_THREAD_CTX pThCtx = UMTSNwl_get_ctx();    
    
    rc = MxCreateThreadEx(0, (TENTRY)UMTSNwl_SaveDataFun, NULL, pThCtx, 0, &pThCtx->hNWLPhyThread);    

    if (FAILED(rc)) {
        MxRemoveThread(pThCtx->hNWLPhyThread);
        pThCtx->hNWLPhyThread = HNULL;
    }    
    
    return rc;
    
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief   kill save antenna data thread

 Function UMTS_Nwl_save_data_thread_kill.

 @param    none .
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void UMTS_Nwl_save_data_thread_kill()
{
    PUMTS_NWL_THREAD_CTX pThCtx = UMTSNwl_get_ctx();
    
    if (HANDLE_NOT_NULL(pThCtx->hNWLPhyThread))
    {
        MxRemoveThread(pThCtx->hNWLPhyThread);
    }        
}


#ifndef _WIN32


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief   umts nwl initial request message processing function

 Function NWL_3G_Init.

 @param    pIn =  message buffer.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
U32 NWL_3G_Init(U8 *pIn)
{
		
   // U32 i;
    U32 status = 0;
    STARTREQ temp;
    U32 logIqEnable;
    MXRC rc = MX_OK;
    
    U32 profID = 7;
	U32 tx_ann = 1;
	U32 rx_ann = 1;	
	U32 loopb  = 0;
	U32 rxfreq = 0;
	U32 txfreq = 0;
	    
    T_3G_INIT_NWL *IntMsg =(T_3G_INIT_NWL*)pIn;
	    
    
    if(gMode != INIT_MODE)
    {
        uart_printf(" current mode is not init mode \n");
        return FAILURE;        
    }
    
    gMode = UMTS_NWL_MODE;


    uart_printf ("\r\nAD9361 initialization: band=%d, tx-ann=%d, rx-ann=%d [%s]...\r\n", profID, tx_ann, rx_ann, loopb ? "loopback" : "no loopback");

	if (FAILED(rc = Ad9361RadioInit (profID, tx_ann, rx_ann, loopb, rxfreq, txfreq)))
	{
		uart_printf ("error,rc=%x\r\n", rc);
	}
	else
	{
		uart_printf ("OK\r\n");
	}
    		


    // Driver Init
    {
        MXRC rc = MX_OK;
        
        UMTSNWL_PPHYINITCTX pPhyInitCtx;

        pPhyInitCtx = &UMTSNwl_phyInitCtx;

        memset(pPhyInitCtx, 0, sizeof(pPhyInitCtx));

        pPhyInitCtx->FecConfigRegs.wimax_lte = FEC_HWCONF_LTE;
        pPhyInitCtx->FecConfigRegs.bp_rate_match = 0;
        pPhyInitCtx->FecConfigRegs.bp_channel_interleaver = 0;
        pPhyInitCtx->FecConfigRegs.ci_symbol_number = 0; // 12

        if (MxGetARMID() == 0)
        { /* Quad core */

            pPhyInitCtx->cfg.CpuDrivers [0] =
                SYSCORE_SYS_MDMA_DRV    |
                SYSCORE_SPU_MDMA_DRV    |
                SYSCORE_RAD_MDMA_DRV;

            pPhyInitCtx->cfg.CpuDrivers [0] |=
                SYSCORE_CEVA_DRV        |
                SYSCORE_SCHEDULER;
        }
        else
        { /* Dual core */
            pPhyInitCtx->cfg.CpuDrivers [0] =
                SYSCORE_SYS_MDMA_DRV    |
                SYSCORE_SPU_MDMA_DRV    |
                SYSCORE_RAD_MDMA_DRV;

            pPhyInitCtx->cfg.CpuDrivers [0] |=
                SYSCORE_CEVA_DRV        |
                SYSCORE_SCHEDULER;
        }

        pPhyInitCtx->cfg.PtrFecCfg = &pPhyInitCtx->FecConfigRegs;
        pPhyInitCtx->cfg.FpMode = 0;
        pPhyInitCtx->cfg.PtrSchedCfg = &gInitScheduler;

        pPhyInitCtx->cfg.RexCfg |= REX_CFG_IDFT_ON_CEVA;

        if (FAILED(rc = SysCoreConfig (&pPhyInitCtx->cfg)))
        {
            _ASSERT_RC(rc);
            uart_printf("Syscore config error [%d]\n", rc);
            return rc;
        }
    }


    gUMTSNwlAntData0 = (S16*)MxHeapAlloc (DDR1NcnbHeap, (38400*130)*4*2*sizeof(S16)); 
    UMTSNwl_iq_storage = (S16*)MxHeapAlloc (DDR1NcnbHeap, (38400*130)*4*2*sizeof(S16));     
    gUMTSNwlPscBuf = (S8 *)MemAlloc(NWL_MAX_PSC*76800, DDR_MEM, 0);
    uart_printf("Address of gAntData0 %x\r\n",gUMTSNwlAntData0);	
    uart_printf("========= Address of UMTSNwl_iq_storage %x ========= \r\n",UMTSNwl_iq_storage);


	
    /* malloc memory for share by cell search module and BCH decode module */
    pNwlCommInfo = (NWL_CommInfor *)MemAlloc(sizeof(NWL_CommInfor)*NWL_MAX_PSC, CRAM_MEM, 0);
	pCellSrchMsg = (NWL_CELLSRCHINFO*)MemAlloc(sizeof(NWL_CELLSRCHINFO), CRAM_MEM, 0);
	memset(pCellSrchMsg,0,sizeof(NWL_CELLSRCHINFO));

    
    /*create thread*/
    status |= UMTS_Nwl_save_data_thread_init();
    status |= UMTS_Nwl_cell_search_thread_init();
    status |= UMTS_Nwl_BCH_decode_thread_init();
    
    /*create TCB*/    
    Umtsnmm_CS_TcbInit();
	Umtsnmm_BchTcbInit();
        
    /*DI init*/
    PhyDiInit(NULL);    
    
    /*DI start*/
    temp.mode = IntMsg->mode;
	//temp.msgType = 17;
	//temp.phyEntityId = 25;
	temp.period = 0;
	temp.count = 0;

    if(DI_CPRI_EMULATOR == IntMsg->mode)
    {
        gUmtsnmmMode=DI_CPRI_EMULATOR;
		temp.period = 5; // the period of interrupt , the unit is ms
    }
    else
    {
        gUmtsnmmMode = DI_PCIE;
    }
        


    //====================================================================
    // MLOG
    //====================================================================
    
    if (gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_MLOG)
    {
        MLogEnableRC (1);
        MLogSetMask(0xFFFFFFFF);
    }
    else
    {
        MLogSetMask(0);
    }
                
    //====================================================================
    // API Logs
    //====================================================================
    if (gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_APILOG)
    {
        SvsrLoggerSetMask(1<<PHY_RXSTART_REQ | 1<<PHY_RXSDU_IND | 1<<PHY_RXEND_IND| 1<<PHY_RXSTATUS_IND |
                          1<<PHY_TXSTART_REQ | 1<<PHY_TXSDU_REQ |
                          1<<PHY_TXHIADCIUL_REQ | 1<<PHY_TXHISDU_REQ | 1<<PHY_TXDCIULSDU_REQ);//(1<<PHY_RXSTART_REQ | 1<<PHY_RXSDU_IND | 1<<PHY_RXEND_IND| 1<<PHY_RXSTATUS_IND);
    }
    else
    {
        SvsrLoggerSetMask(0);
    }
    //====================================================================
    // IQ Samples
    //====================================================================

    if (gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_IQLOGS)
    {
        logIqEnable = DI_LOG_TX0_LOG_ENABLED |
                      DI_LOG_TX1_LOG_ENABLED |
                      DI_LOG_RX0_LOG_ENABLED |
                      DI_LOG_RX1_LOG_ENABLED |
                      DI_LOG_TX_LOG_RR |
                      DI_LOG_RX_LOG_RR;
    }
    else
    {
        logIqEnable = (0);
    }

                                        
    PhyDiStart(0, (PSTARTREQ)(&temp), logIqEnable);
                                        
    /*init IQ index*/
    UMTSNwl_iq_storage_curr_idx = 0;
    
    uart_printf("3G NWL INIT Complete. Running on LARM 0 (3x1 mode)\r\n");

    g3GNwlStartFlag = 1;
    
    return (status);

}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief   umts nwl stop request message processing function

 Function NWL_3G_Stop.

 @param    none.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
U32 NWL_3G_Stop(void)
{
    UMTSNWL_PPHYINITCTX UMTSNwl_pPhyInitCtx = &UMTSNwl_phyInitCtx;
    MXRC rc;
    
	MxHeapFree(DDR1NcnbHeap,gUMTSNwlAntData0);
	MxHeapFree(DDR1NcnbHeap,UMTSNwl_iq_storage);

	MemFree(gUMTSNwlPscBuf,DDR_MEM);	
	MemFree(pNwlCommInfo,CRAM_MEM);
	MemFree(pCellSrchMsg,CRAM_MEM);
	
    g3GNwlStartFlag = 0;
    /*DI shutdown*/
    PhyDiShutdown(NULL);
    
    /*remove TCB*/
    Umtsnmm_CS_TcbStop();
	Umtsnmm_BchTcbStop();

	/*remove thread*/
	UMTS_Nwl_save_data_thread_kill();
	UMTS_Nwl_cell_search_thread_kill();
	UMTS_Nwl_BCH_decode_thread_kill();	
	
    PhyDiDestroy();
    if (FAILED(rc = SysCoreShutdown (&UMTSNwl_pPhyInitCtx->cfg)))
    {
        _ASSERT_RC(rc);
        uart_printf("Syscore config error [%d]\n", rc);
        return rc;
    }
    uart_printf("3G NWL stop Complete. \r\n");
    gMode = INIT_MODE;
    
    return MX_OK;
}

#endif

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief   process different message.

 Function UmtsNwlMsgpro.

 @param    inMsg =  message buffer.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
U32 UmtsNwlMsgpro(U32 *inMsg)
{
    PGENMSGDESC pMsg =(PGENMSGDESC)inMsg ;
    U32 status=0;
	
    switch (pMsg->msgType)
    {
	    case NWL_INIT_3G_NWL:
            status = NWL_3GNwlInitCheck((U8*)inMsg + sizeof(PGENMSGDESC));
            if(NWL_ERR_OK !=status)
            {
            	printf("UMTSNWL init failed! \r\n");
            }
            else
            {
                status = NWL_3G_Init((U8*)inMsg + sizeof(PGENMSGDESC));
            }
		break;

	    case NWL_STOP_3G_NWL:
            status = NWL_3G_Stop();
            if(0 != status)
            {
                //Printf("NWL_3G_Stop fail: %d \n", status);
            }
		break;
	    case NWL_CELL_SEARCH_REQ:
		//ShowHeaps();
		status = NWL_cellSearchReqCheck((U8*)inMsg + sizeof(PGENMSGDESC));
            if(NWL_ERR_OK !=status)
            {
                NWL_GenerateAPIMsg(NWL_CELL_SEARCH_CNF, status,API_TYPE_CFG_RESP);
            }
            else
            {
				NWL_cellSearchReqPro((U8*)inMsg + sizeof(PGENMSGDESC));
            }

		//ShowHeaps();
		
		break;
	     case NWL_STOP_CELL_SEARCH_REQ:
		//ShowHeaps();	
		status = NWL_cellSearchStopCheck((U8*)inMsg + sizeof(PGENMSGDESC));
            if(NWL_ERR_OK !=status)
            {
                NWL_GenerateAPIMsg(NWL_STOP_CELL_SEARCH_CNF, status,API_TYPE_CFG_RESP);
            }
            else
            {
                CellSrcStopPro(); 
            }
		//ShowHeaps();
	
		break;

	case NWL_BCH_CONFIG_REQ:
		//ShowHeaps();
            status = NWL_BchConReqCheck((U8*)inMsg + sizeof(PGENMSGDESC));
            if(NWL_ERR_OK !=status)
            {
                NWL_GenerateAPIMsg( NWL_BCH_CONFIG_CNF, status,API_TYPE_CFG_RESP);
            }
            else
            {
                CLBchInit();
                status = NWL_BchConReqPro((U8*)inMsg + sizeof(PGENMSGDESC));
            }
            
		//ShowHeaps();
		break;

	case NWL_STOP_BCH_REQ:
            status = NWL_BchConStopCheck((U8*)inMsg + sizeof(PGENMSGDESC));
            if(NWL_ERR_OK !=status)
            {
                NWL_GenerateAPIMsg( NWL_STOP_BCH_CNF, status,API_TYPE_CFG_RESP);
            }
            else
            {
                status = CLBchStopMsgPro();
            }
		break;
        default:
			status = NWL_ERR_MSGID_ERR;
            break;
    }
	
    return status;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief   message handler,fill message header.

 Function UmtsNWLHandler.

 @param    pClientCtx =  can be set NULL.
 @param    pApi =  the header of reveived message.
 @param    pResp =  the header of send message.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
APIHEC UmtsNWLHandler(LPVOID pClientCtx, ApiHeader *pApi, ApiHeader *pResp)
{
    MXRC rc = MX_OK;
    U32* inMsg = (U32*)(pApi + 1);
    U32 status;
	
	
    status=UmtsNwlMsgpro(inMsg);
    if(NWL_ERR_OK != status)
    {
        uart_printf("UMTS NWL message error id: %d \r\n", status);
        return APIHEC_OK;
    }
	
    switch (pApi->MessageID)
    {
		case 0x510a:
	      {
			CellSrchAntDataRev((U8*)(pApi+1), pApi->Length);
			break;
		}
		case 0x510f:
		{
			CellSrchPscSeqRev((U8*)(pApi+1), pApi->Length);
			break;
		}

		case 0x551f:
		{
			GetCLBchAntData((U8*)(pApi+1), pApi->Length);
			break;
		}
		case 0x552f:
		{
			GetCLScrCode((U8*)(pApi+1), pApi->Length);
			break;
		}
		
        default:
            break;

	}


    pResp->Type = API_TYPE_CFG_RESP;
    ApiAddErrorCode(pResp, rc);
    return APIHEC_OK;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief   set RF save data parameters.

 Function WCDMARadioSave.

 @param    pCtx =  can be set NULL.
 @param    pParams =  control parameters.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
UINT32 WCDMARadioSave(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
	UMTSNwlRadioType = StrToNum(pParams->ParamList[0]);
	if(UMTSNwlRadioType == 0)
	{
		MdmaShutdown();
    	MdmaInit();
	}
		
	UMTSNwlRadioStatus = 1;
	return 0;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  testmac message processer: get thread.

 Function lte_bs_phy_umtsnmm_api_get_ctx.

 @param    none.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
LTE_PHY_API_THREAD_CTX * lte_bs_phy_umtsnmm_api_get_ctx(void)
{
     return &lte_bs_phy_umtsnmm_api_ctx;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  testmac message processer: message schedule thread.

 Function lte_bs_phy_umtsnmm_schedule_api_thread.

 @param    none.
 @return    none.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
void lte_bs_phy_umtsnmm_schedule_api_thread(void)
{
    PLTE_PHY_API_THREAD_CTX thctx = lte_bs_phy_umtsnmm_api_get_ctx();

    _ASSERT((thctx != NULL));


    MxScheduleThreadEx(thctx->hLtePhyThread, 0, PHY_SMP_THREAD);

    return;
}

//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  testmac message processer: message handler.

 Function lte_bs_phy_umtsnmm_msg_handler.

 @param    client_ctx = can be set NULL.
 @param    msg = message buffer.
 @return    2.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
MXRC lte_bs_phy_umtsnmm_msg_handler(LPVOID client_ctx, LPVOID msg)
{// message comes without Transcede API header
    PLTE_PHY_API_THREAD_CTX thctx = lte_bs_phy_umtsnmm_api_get_ctx();

    thctx->pMsg = (PMAC2PHY_QUEUE_EL)msg;

    lte_bs_phy_umtsnmm_schedule_api_thread();

    return SVSR_MSG_OPT_SAVE;
}


//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  testmac message processer: message processing thread.

 Function lte_bs_phy_umtsnmm_api_thread.

 @param    param = thread struct buffer.
 @return    state.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
UINT32 lte_bs_phy_umtsnmm_api_thread(LPVOID param)
{
    PLTE_PHY_API_THREAD_CTX pThCtx = (PLTE_PHY_API_THREAD_CTX)param;
    PMAC2PHY_QUEUE_EL pMsg, curr, tmpElm;
    PGENMSGDESC pMsg1;
    U32* inMsg  = NULL;
    U32 status = 0;
    U16             len = 0;
    U32 tTotal = GET_TICKS;
    S16 nmm_cell_search_vector[20];
    S16 *p16;
    p16 = (S16 *)nmm_cell_search_vector;
    *p16++ = 0;         /* pci */
    *p16++ = 1;         /* earfcn */

    _ASSERT(pThCtx != NULL);


    pMsg = pThCtx->pMsg;
    curr = pMsg;

    while (curr)
    {
        // U8 * p8 = (U8 *)curr->MessagePtr;
        // U16 *p16;
        inMsg =  (U32*)curr->MessagePtr;
        pMsg1 = (PGENMSGDESC) inMsg;
        len = (U16) pMsg1->msgSpecific;

        SvsrLoggerAddMsgForce(pMsg1->msgType, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IDD_UMTS_NWL), pMsg1, len + sizeof(GENMSGDESC));

        status=UmtsNwlMsgpro((U32*)pMsg1);
        if(NWL_ERR_OK != status)
        {
            uart_printf("UMTS NWL message error id: %d \r\n", status);
        }

        tmpElm = curr;
        curr = curr->Next;

        /*free the message and its message content*/
        SvsrFreeIMem(tmpElm->MessagePtr);
        SvsrFreeIMem(tmpElm);

    }

    MLogTask(PID_NMM_API_THREAD, RESOURCE_LARM, tTotal, GET_TICKS);

    return status;
}



//*******************************************************************************
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
/** 
 @brief  testmac message processer: initialize message processing thread.

 Function lte_bs_phy_umtsnmm_init_api_thread.

 @param    none.
 @return    state.

 */
//doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen-doxygen
//*******************************************************************************
MXRC lte_bs_phy_umtsnmm_init_api_thread(void)
{
    MXRC rc = MX_OK;
    PLTE_PHY_API_THREAD_CTX thctx = lte_bs_phy_umtsnmm_api_get_ctx();

    _ASSERT_PTR(thctx);

    if (HANDLE_NOT_NULL(thctx->hLtePhyThread))
        return MX_OK;

    memset(thctx, 0, sizeof(LTE_PHY_API_THREAD_CTX));

    rc |= MxCreateThreadEx(API_DISPATCH_THREAD_ARM, lte_bs_phy_umtsnmm_api_thread,
                           NULL,
                           thctx,
                           0,
                           &thctx->hLtePhyThread);

    if (FAILED(rc))
    {
        MxRemoveThread(thctx->hLtePhyThread);
        thctx->hLtePhyThread = HNULL;
    }

    return rc;
}

