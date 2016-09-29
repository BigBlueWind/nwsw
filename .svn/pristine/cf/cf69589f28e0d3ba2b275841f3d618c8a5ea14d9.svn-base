/** @file MAC.c
 *
 * @brief Check API processing logic
 * @author Mindspeed Technologies
 * @version $Revision: 1.73 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#if defined (MSMAC) || defined (UE)

#include "osal.h"
#include "msmac.h"
#include "macMemUtil.h"
#include "ctrlmsg.h"
#include "msgroute.h"
#include "phyiface.h"
#include "LtePhyL2Api.h"
#include "rrciface.h"
#include "rrc_types.h"
#include "maciface.h"
#include "lteuemaculscheduler.h"
#include "LteMac2PhyInterface.h"
#include "LteMacRlcInterface.h"



#define MSG_MAXSIZE         16384
#define MAX_MAC_QUEUE_LEN   6

extern OSAL_SEMAPHORE ToRRCSema;
extern OSAL_QUEUE ToRRCQueue;
extern OSAL_MEMARRAY RRC_MAC_MsgArray;

// RRC - MAC
OSAL_SEMAPHORE LteMacDlSema;
OSAL_QUEUE bRrcToMac;

OSAL_TASK LteMacDlTaskId;
OSAL_TASK LteMacUlTaskId;

MS_DESC LteMsMac;
MS_DESC* pMsMac;

DLSUBFRDESC DlSubFrame;
ULSUBFRDESC UlSubFrame;

MAC_PDU_DESC MacPduDesc[MAXCHSUBFRAME];

void MacCreateDlCtx(void);
void MacCreateUlCtx(void);
RESULTCODE UeLcTestCreate(PUE_DL_CTX DlUeCtx,
                    PUE_UL_CTX UlUeCtx);

void PhyProcessFromMacMsg(OSAL_SIZE msgSize, PGENMSGDESC pMsg);
int QueueRrcToMac(U16* logicChanID, PTR pBuf, OSAL_SIZE blockSize);
int QueueMacToRrc(U16* logicChanID, PTR pBuf, OSAL_SIZE blockSize);
void  MSSendMsg(HANDLE h, PTR pMsg, OSAL_SIZE msgSize);


/** @brief Uplink task
 *
 */
void LteMacDlTask(PTR pParam)
{
    MAC_PDU_DESC* pMacPduDesc;

    while (1)
    {
        if (OsalWaitForSema(&LteMacDlSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            /* for current subframe frame */
            LteMacSendToPhyMessage(PHY_RXSTART_REQ, &UlSubFrame, 1);
            pMacPduDesc = &MacPduDesc[0];
            if (pMsMac->schedulingSchema == UE_MAIN_SCHEMA)
            {
                RrcDataTransmit();
                LteUeMacUlScheduler(pMsMac, &UlSubFrame);
            }

#if defined (UE)
            //Alan: since current "4GCPlugin_LTEMS_PHY.dll" donesn't responde
            //"PHY_TXEND_IND", thus here we clean up all old "UlSubFrame".
            LteMacReleaseSubFrame(&UlSubFrame);
#endif
            UlSubFrame.numberofChannelDescriptors++;
            UlSubFrame.ulCh[0].channelId = 0;
            UlSubFrame.ulCh[0].channelType = PUSCH;
            LteMacSendToPhyMessage(PHY_TXSTART_REQ, &DlSubFrame, 1);
        }
        else
        {
            break;
        }
    }
}

/** @brief Downlink task
 *
 */

void LteMacUlTask(PTR pParam)
{
    PMSGHEADER buffer = MsgAllocBuffer(MSG_COMMON);
    OSAL_SIZE msgSize;
    PGENMSGDESC pMsg = (PGENMSGDESC) MsgGetDataOffset(buffer);
    Mac2PhyInterface *pPhy = &pBsMacDesc->Phy;

    if (buffer == 0)
    {
        return;
    }
    while (1)
    {
        // This is blocking call
        if (pPhy->rxCb(pPhy->fromPhy, MsgGetDataOffset(buffer), &msgSize) == SUCCESS)
        {
            LteMacProcessFromPhyMessage(pMsg, &DlSubFrame, &UlSubFrame, pMsMac);
        }
    }
}
static RESULTCODE LteMsMacConfig(U16 instanceID, MAC_INIT_CONFIG *pMacInitConfig)
{
    int i, j;
    for (i = 0; i < MAXCHSUBFRAME; i++)
    {
        MacPduDesc[i].NumSduBlock = 0;
        for (j = 0; j < MAX_SDU_BLOCKS; j++)
        {
            MacPduDesc[i].pSduBlock[j] = NULL;
            MacPduDesc[i].SduBlockLen[j] = 0;
        }
        MacPduDesc[i].pUe = NULL;
    }

    memset(&DlSubFrame, 0, sizeof(DlSubFrame));
    memset(&UlSubFrame, 0, sizeof(UlSubFrame));

    pMsMac = &LteMsMac;

    memset(pMsMac, 0, sizeof(MS_DESC));
    pBsMacDesc = &pMsMac;

    pBsMacDesc->frameNumber = 0;
    pBsMacDesc->subFrameNumber = 0;
    pBsMacDesc->globalTtiCount = 0;
    pBsMacDesc->sduCount = 0;
    pBsMacDesc->errorCount = 0;

    pBsMacDesc->InstanceID = instanceID;
    pBsMacDesc->predefConfig = pMacInitConfig->macConfMode;
    if (LtePhyParamInit(&pBsMacDesc->initParm,
                        &DlSubFrame,
                        &UlSubFrame,
                        pBsMacDesc->predefConfig) != SUCCESS)
    {
        printf("Error LtePhyParamInit\n");
    }
    return SUCCESS;
}
/* Function Name: LteMacInit
*Inputs:         HANDLE pHeap - MAC heap
*                U16 instanceID - MAC Id
*                MAC_INIT_CONFIG *pMacInitConfig - MAC init config
*                PhyTxCb txCb - phy tx buffer callback
*                PhyRxCb rxCb - phy rx buffer callback
*                HANDLE toPhy
*                HANDLE fromPhy)
* Outputs :        UE context, DlUeCtx,UlUeCtx
* Returns:         SUCCESS or FAILURE
* Description:
 *
 */
RESULTCODE LteMacInit(HANDLE pHeap,
                U16 instanceID,
                MAC_INIT_CONFIG *pMacInitConfig,
                PhyTxCb txCb,
                PhyRxCb rxCb,
                HANDLE toPhy,
                HANDLE fromPhy)
{
    OSAL_PADDRESS RRC_MACMemBlock;

    pMacInitConfig->macConfMode = MACCONF_FDX0;
    if(LteMsMacConfig(instanceID, pMacInitConfig)
        !=SUCCESS)
    {
        printf("Ms Mac init config error\n");
        return FAILURE;

    }
    MacCreateDlCtx();
    MacCreateUlCtx();
    pMsMac->schedulingSchema = UE_MAIN_SCHEMA;
    pMsMac->ueId = 0x77;
    if (UeLcTestCreate(&pMsMac->DlCtx, &pMsMac->UlCtx)!= SUCCESS)
    {
        printf("MAC CREATE ERROR: cannot create the  test channels\n");
        return FAILURE;
    }


    if (OsalCreateSema(&LteMacDlSema) != SUCCESS)
    {
        printf("Error create LteMacDlSema\n");
        return FAILURE;
    }
    /*
    *   queue and sema to set data from upper layer
    */
     if(OsalAllocHeap(&pHeap,
         &RRC_MACMemBlock,
         MAX_MAC_QUEUE_LEN * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
        printf(C4KFW "ERROR: RRC: No memory [%s]\n", __FUNCTION__);
        return FAILURE;
    }
    OsalCreateQueue(&bRrcToMac, RRC_MACMemBlock, MAX_MAC_QUEUE_LEN * sizeof(OSAL_PADDRESS));
    if (LtePhyInterfaceInit(&pBsMacDesc->Phy,
                            txCb,
                            rxCb,
                            toPhy,
                            fromPhy) != SUCCESS)
    {
        printf(C4KFW "ERROR: PHY interface init\n");
        return FAILURE;
    }
    if (CcchMgrInit() != SUCCESS)
    {
        printf(C4KFW "ERROR: CCCH manager init\n");
        return FAILURE;
    }
    OsalCreateTask(&LteMacDlTaskId, (OSAL_PADDRESS) &LteMacDlTask, NULL, "LteMacDlTask", OSAL_PRIORITY_NORMAL, (OSAL_PADDRESS) 0, 0);
    OsalCreateTask(&LteMacUlTaskId, (OSAL_PADDRESS) &LteMacUlTask, NULL, "LteMacUlTask", OSAL_PRIORITY_NORMAL, (OSAL_PADDRESS) 0, 0);
    OsalActivateTask(&LteMacDlTaskId);
    OsalActivateTask(&LteMacUlTaskId);

    return SUCCESS;
}

RESULTCODE UeLcTestCreate(PUE_DL_CTX DlUeCtx,
                    PUE_UL_CTX UlUeCtx)
{
    RESULTCODE Status = SUCCESS;
/*RESULTCODE LteUeMacCreateLogicChannel(void* DlUeCtx,
                                        void* UlUeCtx,
                                        U8 tag,
                                        U16 LogicChanID,
                                        U16 LogicChanType,
                                        U32 LrcMode);
                                        */
 // create  Ligical Channels for debug
    if (( Status = LteUeMacCreateLogicChannel(&DlUeCtx->DlDesc,
                                            NULL,
                                            PAR_DL_LC_CREATE,
                                            0,
                                            CCCH ,
                                            0)) != SUCCESS)
    {
        return Status;
    }
    if (( Status = LteUeMacCreateLogicChannel(NULL,
                                            &UlUeCtx->UlDesc,
                                            PAR_UL_LC_CREATE,
                                            0,
                                            CCCH,
                                            0)) != SUCCESS)
    {
          return Status;
    }
    if (( Status = LteUeMacCreateLogicChannel(&DlUeCtx->DlDesc,
                                                NULL,
                                                PAR_DL_LC_CREATE,
                                                1,
                                                DCCH,
                                                0)) != SUCCESS)
    {
          return Status;
    }
    if (( Status = LteUeMacCreateLogicChannel(NULL,
                                                &UlUeCtx->UlDesc,
                                                PAR_UL_LC_CREATE,
                                                1,
                                                DCCH,
                                                0)) != SUCCESS)
    {
          return Status;
    }

   if (( Status = LteUeMacCreateLogicChannel(&DlUeCtx->DlDesc,
                                                NULL,
                                                PAR_DL_LC_CREATE,
                                                2,
                                                DTCH,
                                                0)) != SUCCESS)
    {
          return Status;
    }
    if (( Status = LteUeMacCreateLogicChannel(NULL,
                                                &UlUeCtx->UlDesc,
                                                PAR_UL_LC_CREATE,
                                                2,
                                                DTCH,
                                                0)) != SUCCESS)
    {
          return Status;
    }
    return Status;
}
RESULTCODE LteMacDestroy(void)
{
    if (LteMacDlTaskId)
    {
        OsalDestroyTask(&LteMacDlTaskId);
        LteMacDlTaskId = 0;
    }
    if (LteMacUlTaskId)
    {
        OsalDestroyTask(&LteMacUlTaskId);
        LteMacUlTaskId = 0;
    }

    return SUCCESS;
}

void MacCreateDlCtx(void)
{
    pMsMac->DlCtx.DlDesc.ueId = 0x88;
    pMsMac->DlCtx.DlDesc.numLogicalChannels = 0;
    pMsMac->DlCtx.LteMacGetDataBlock = (MacRlcIoFunc)QueueRrcToMac;
    pMsMac->DlCtx.LteMacPutDataBlock = (MacRlcIoFunc)RlcDataRcv;
    memset(pMsMac->DlCtx.DlDesc.logicalChannel, 0, sizeof(PTR)*MAX_NUMBER_OF_LOGICAL_CHANNEL);

}
void MacCreateUlCtx(void)
{
    pMsMac->UlCtx.UlDesc.ueId = 0x88;
    pMsMac->UlCtx.UlDesc.numLogicalChannels = 0;
    pMsMac->UlCtx.LteMacGetDataBlock = (MacRlcIoFunc)QueueRrcToMac;
    pMsMac->UlCtx.LteMacPutDataBlock = (MacRlcIoFunc)RlcDataRcv;
    memset(pMsMac->UlCtx.UlDesc.logicalChannel, 0, sizeof(PTR)*MAX_NUMBER_OF_LOGICAL_CHANNEL);

}

U32 GetUeId()
{
    return pMsMac->ueId;
}


#endif /* MSMAC || UE */
