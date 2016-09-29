/** @file bsmac.c
 *
 * @brief Main module of LTE BS MAC
 * @author Mindspeed Technologies
 * @version $Revision: 1.121.20.1 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#if defined (BSMAC) || defined (ENODEB)

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "osal.h"
#include "bsp.h"
#include "bsmac.h"
#include "macMemUtil.h"
#include "ctrlmsg.h"
#include "msgroute.h"
#include "phyiface.h"
#ifndef AC_MSPD_TDD
#include "LtePhyL2Api.h"
#else
#include "LtePhyL2Api_TDD.h"
#endif
#include "rrc_types.h"
#include "maciface.h"
#include "LteMacRlcInterface.h"
#include "rlciface.h"

#include "LteMac2PhyInterface.h"
#include "LteBsMacDlScheduler.h"
#include "LteBsMacUlScheduler.h"
#include "LteMacUeContext.h"
#include "LteMac2RrcInterface.h"
#include "LteBsMacPdcchMgr.h"
#include "LteBsMacPdschMgr.h"
#include "LteBsMacCcch.h"
#include "LteMacTestCfgMgr.h"

#include "transport.h"

#include "lte_mac_state.h"

/****************************************************************************
 Private Definitions
 ****************************************************************************/

#define MAX_MAC_QUEUE_LEN 6

/****************************************************************************
 Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

BS_CONTEXT *pBsMacDesc = NULL;
DLSUBFRDESC DlSubFrame;
ULSUBFRDESC UlSubFrame;
MAC_PDU_DESC MacPduDesc[MAXCHSUBFRAME];
V32 globalTTITickCount;

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

static BS_CONTEXT BsMacDesc;

/****************************************************************************
 * External Functions
 ****************************************************************************/

OSAL_QUEUE bRrcToMac;
extern OSAL_SEMAPHORE ToRRCSema;
extern OSAL_QUEUE ToRRCQueue;
extern OSAL_MEMARRAY RRC_MAC_MsgArray;
extern RESULTCODE UeContextClean(void);
extern MAC_INIT_CONFIG macInitConfig;

OSAL_SEMAPHORE LteMacDlSema;
OSAL_TASK LteMacDlTaskId = 0;
OSAL_TASK LteMacUlTaskId = 0;
/* define enumeration for ul, dl and special subframe in TDD frame structure */
typedef enum {
	sfrmType_eUL = 0,
	sfrmType_eDL = 1,
	sfrmType_eSF = 2,
	sfrmType_eMax         // this is use for sanity check
}sfrmTypeEnum_t;

/* Table for UL-DL configuration  */
sfrmTypeEnum_t  frmConfigIdxTable[7][10] = {
	{ sfrmType_eDL, sfrmType_eSF, sfrmType_eUL, sfrmType_eUL, sfrmType_eUL, sfrmType_eDL, sfrmType_eSF, sfrmType_eUL, sfrmType_eUL, sfrmType_eUL},
	{ sfrmType_eDL, sfrmType_eSF, sfrmType_eUL, sfrmType_eUL, sfrmType_eDL, sfrmType_eDL, sfrmType_eSF, sfrmType_eUL, sfrmType_eUL, sfrmType_eDL},
	{ sfrmType_eDL, sfrmType_eSF, sfrmType_eUL, sfrmType_eDL, sfrmType_eDL, sfrmType_eDL, sfrmType_eSF, sfrmType_eUL, sfrmType_eDL, sfrmType_eDL},
	{ sfrmType_eDL, sfrmType_eSF, sfrmType_eUL, sfrmType_eUL, sfrmType_eUL, sfrmType_eDL, sfrmType_eDL, sfrmType_eDL, sfrmType_eDL, sfrmType_eDL},
	{ sfrmType_eDL, sfrmType_eSF, sfrmType_eUL, sfrmType_eUL, sfrmType_eDL, sfrmType_eDL, sfrmType_eDL, sfrmType_eDL, sfrmType_eDL, sfrmType_eDL},
	{ sfrmType_eDL, sfrmType_eSF, sfrmType_eUL, sfrmType_eDL, sfrmType_eDL, sfrmType_eDL, sfrmType_eDL, sfrmType_eDL, sfrmType_eDL, sfrmType_eDL},
	{ sfrmType_eDL, sfrmType_eSF, sfrmType_eUL, sfrmType_eUL, sfrmType_eUL, sfrmType_eDL, sfrmType_eSF, sfrmType_eUL, sfrmType_eUL, sfrmType_eDL}
};
U8 FactorM[7][10] =
{
    {2, 1, 0, 0, 0, 2, 1, 0, 0, 0},
    {0, 1, 0, 0, 1, 0, 1, 0, 0, 1},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {1, 1, 0, 0, 0, 1, 1, 0, 0, 1}
};
/* This table is obtained from 3GPP 36.211 Table 4.2-1
* Each row of the table corresponds to a special subframe configuration. The First
* column gives the number of IQ samples in the DwPTS for normal CP. The 2nd
* column give the number IQ samples in the DwPTS for the extended CP.
*/
static U16 specialSubframeSamplesTable[9][2] = {
	{6592,  7680 },
	{19760, 20480},
	{21952, 23040},
	{24144, 25600},
	{26336, 7680},
	{6592,  20480},
	{19760, 23040},
	{21952, 0},             /* The last 2 config indices do not apply to extended CP */
	{24144, 0}
};

/****************************************************************************
 * Module Local Functions
 ****************************************************************************/

void LteMacAssignResourceToLogicChan(U16 SubFrameType, UE_DESC *pUe, U16 Rb)
{
    U16 j, k, temp;
    LOGIC_CHAN_DESC *pLogicChan;

    if (pUe == NULL)
    {
        printf("Error Assign Resouce to LogicChan: invalid UE\n");
        return;
    }

    j = pUe->NLogicChannel;
    if (j)
    {
        // Equally divide the resource a UE received among all its
        // logic channels
        temp = Rb / j;
        for (k = LCID_MIN_LCHAN; j > 0 && k <= LCID_MAX_LCHAN; k++)
        {
            pLogicChan = pUe->pLogicChan[k - LCID_MIN_LCHAN];
            if (pLogicChan)
            {
                if (SubFrameType == DLTX)
                {
                    pLogicChan->DownlinkNumRB = temp;
                }
                else
                {
                    pLogicChan->UplinkNumRB = temp;
                }
                j--;
            }
            else
            {
                continue;
            }
        }
    }
}

void RLCDataRequest(void)
{
}

/** @brief Process the uplink subframe
 *
 *  @param Param1 Description of function parameter
 *  @param Param2 Description of another parameter
 *  @return Description of return value
 *
 *  This function is to process the received uplink subframe
 */

/** @brief Downlink task
 *
 */
void LteMacDlTask(PTR pParam)
{
    MAC_PDU_DESC* pMacPduDesc;

    while (1)
    {
        if (OsalWaitForSema(&LteMacDlSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            // Get\send messages form\to RRC
            if (pBsMacDesc->schedulingSchema != FDX_SCHEMA)
            {
                RrcProcessInterface();
                RLCDataRequest();
            }

            // Allocate resources according to current requests from UE
            if (LteBsMacUlScheduler(pBsMacDesc, &UlSubFrame) == SUCCESS)
            {
                // Now RXVECTOR is up to date and we can issue the RXSTART request
                LteMacSendToPhyMessage(PHY_RXSTART_REQ, &UlSubFrame, 0);
            }

            // TX
            pMacPduDesc = &MacPduDesc[0];
            pMacPduDesc->pUe = pBsMacDesc->pUe[0];

            // Allocates resources according to current requests
            if(LteBsMacDlScheduler(pBsMacDesc, &DlSubFrame) != SUCCESS)
                continue;

            // Encode channels
            if (pBsMacDesc->schedulingSchema == FDX_SCHEMA)
                LteMacPduEncoder(pMacPduDesc, pBsMacDesc->pUe[0], DLTX);

            LteMacSendToPhyMessage(PHY_TXSTART_REQ, &DlSubFrame, 1);
            printf("TTI %d\n", globalTTITickCount);
            globalTTITickCount++;
            setFrameSubFrameNum();
            setTxFrameSubFrameNum();
        }
        else
        {
            // Error, exit task
            break;
        }
    }
}

void LteMacDlTaskRt(PTR pParam)
{
    MAC_PDU_DESC* pMacPduDesc;

    while (1)
    {
        if (OsalWaitForSema(&LteMacDlSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            // TX
            pMacPduDesc = &MacPduDesc[0];
            pMacPduDesc->pUe = pBsMacDesc->pUe[0];

            // Encode channels
            if (pBsMacDesc->schedulingSchema == FDX_SCHEMA)
                LteMacPduEncoder(pMacPduDesc, pBsMacDesc->pUe[0], DLTX);
        }
        else
        {
            // Error, exit task
            break;
        }
    }
}
#ifdef AC_MSPD_TDD
void LteMacDlTaskTdd(PTR pParam)
{
    MAC_PDU_DESC* pMacPduDesc;
    sfrmTypeEnum_t sfrmType;
    U32 DwPtsIQSamps;
    while (1)
    {
        if (OsalWaitForSema(&LteMacDlSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            // Get\send messages form\to RRC
            if (pBsMacDesc->schedulingSchema != FDX_SCHEMA)
            {
                RrcProcessInterface();
            }

            sfrmType = frmConfigIdxTable[pBsMacDesc->initParm.ulDlConfig][globalTTITickCount%10];
            setPhich(FactorM[pBsMacDesc->initParm.ulDlConfig][globalTTITickCount%10]);
            switch (sfrmType)
		    {
		        case sfrmType_eUL:
                    pBsMacDesc->DirMode = UL;
                    if (LteBsMacUlScheduler(pBsMacDesc, &UlSubFrame) == SUCCESS)
                    {
                    // Now RXVECTOR is up to date and we can issue the RXSTART request
                       LteMacSendToPhyMessage(PHY_RXSTART_REQ, &UlSubFrame, 1);
                    }
                    break;
        		case sfrmType_eDL:
                    pBsMacDesc->DirMode = DL;
                    pMacPduDesc = &MacPduDesc[0];
                    pMacPduDesc->pUe = pBsMacDesc->pUe[0];

                    // Allocates resources according to current requests
                   pBsMacDesc->SpTdd = 0;
                   if(LteBsMacDlSPSchedulerTdd(pBsMacDesc, &DlSubFrame ) != SUCCESS)
                      continue;

                    // Encode channels
                    if (pBsMacDesc->schedulingSchema == FDX_SCHEMA)
                        LteMacPduEncoder(pMacPduDesc, pBsMacDesc->pUe[0], DLTX);

                    LteMacSendToPhyMessage(PHY_TXSTART_REQ, &DlSubFrame, 1);
                    break;
		        case sfrmType_eSF:
                    pBsMacDesc->DirMode = DL;
                    if ( pBsMacDesc->initParm.cyclicPrefixType == NORMAL ) {
					if ( pBsMacDesc->initParm.specialSubframeConfig <= 8 ) {
						// Normal CP
						DwPtsIQSamps = specialSubframeSamplesTable[pBsMacDesc->initParm.specialSubframeConfig][0];
					} else {
						printf("Error: Special subframe configuration value outside the valid range 0..8 (%d)\n",
							pBsMacDesc->initParm.specialSubframeConfig);
						 break;
					}
				}else {
					/***** Extended CP for DL  *****/
					if ( pBsMacDesc->initParm.specialSubframeConfig <= 6 ) {
						DwPtsIQSamps = specialSubframeSamplesTable[pBsMacDesc->initParm.specialSubframeConfig][1];  // DL syms
					} else {
						printf("Error: Special subframe configuration value outside the valid range 0..6 (%d)\n",
							pBsMacDesc->initParm.specialSubframeConfig);
						break;
					}
				}
                pMacPduDesc = &MacPduDesc[0];
                pMacPduDesc->pUe = pBsMacDesc->pUe[0];
                pBsMacDesc->SpTdd = 1;
                // Allocates resources according to current requests
                if(LteBsMacDlSPSchedulerTdd(pBsMacDesc, &DlSubFrame) != SUCCESS)
                      continue;

                    // Encode channels
                if (pBsMacDesc->schedulingSchema == FDX_SCHEMA)
                    LteMacPduEncoder(pMacPduDesc, pBsMacDesc->pUe[0], DLTX);

                LteMacSendToPhyMessage(PHY_TXSTART_REQ, &DlSubFrame, 1);
                break;
            }

           // TX
            printf("TTI %d\n", globalTTITickCount);
            globalTTITickCount++;
            setFrameSubFrameNum();
            setTxFrameSubFrameNum();
        }
        else
        {
            // Error, exit task
            break;
        }
    }
}
#endif
/** @brief Uplink task
 *
 */
void LteMacUlTask(PTR pParam)
{
    PMSGHEADER buffer;
    OSAL_SIZE msgSize;
    PGENMSGDESC pMsg;
    Mac2PhyInterface *pPhy = &pBsMacDesc->Phy;

    while (1)
    {
        if ((buffer = MsgAllocBuffer(MSG_COMMON)) != NULL)
        {
            msgSize = MSG_MAXSIZE;
            // This is blocking call
            // Does copying to local buffer
            if (pPhy->rxCb(pPhy->fromPhy, MsgGetDataOffset(buffer), &msgSize) == SUCCESS)
            {
                pMsg = (PGENMSGDESC) MsgGetDataOffset(buffer);

                // Only support 1 UE currently
                LteMacProcessFromPhyMessage(pMsg,
                                        &DlSubFrame,
                                        &UlSubFrame,
                                        pBsMacDesc->pUe[0]);
            }
            MsgFreeBuffer(buffer);
        }
        else
        {
            // No memory, sleep before next attempt
            OsalWaitDelay(MEM_RECOVERYTIME);
        }
    }
}

void LteMacUlTaskRt(PTR pParam)
{
    PMSGHEADER buffer;
    OSAL_SIZE msgSize;
    PGENMSGDESC pMsg;
    Mac2PhyInterface *pPhy = &pBsMacDesc->Phy;

    while (1)
    {
        if ((buffer = MsgAllocBuffer(MSG_COMMON)) != NULL)
        {
            msgSize = MSG_MAXSIZE;
            // This is blocking call
            // Does copying to local buffer
            if (pPhy->rxCb(pPhy->fromPhy, MsgGetDataOffset(buffer), &msgSize) == SUCCESS)
            {
                pMsg = (PGENMSGDESC) MsgGetDataOffset(buffer);

                // Only support 1 UE currently
                LteMacProcessFromPhyMessageRt(pMsg,
                                            &DlSubFrame,
                                            &UlSubFrame,
                                            pBsMacDesc->pUe[0]);
            }
            MsgFreeBuffer(buffer);
        }
        else
        {
            // No memory, sleep before next attempt
            OsalWaitDelay(MEM_RECOVERYTIME);
        }
    }
}
RESULTCODE MacDestroyTestLogicalChan(void)
{
    return SUCCESS;
}

RESULTCODE MacCreateTestLogicalChan(void)
{
    // TODO: This is dirty hack, change to the list of connected UEs
    UE_DESC *pUE = MacCreateUE();
    U16 chanType;
    U16 chanID;
    RESULTCODE ret = FAILURE;

    chanType = CCCH;
    chanID = LCID_MIN_LCHAN;
    ret = LteMacCreateLogicChannel(pUE, chanType, chanID);
    pUE->pLogicChan[chanType]->LteMacGetDataBlock = (MacRlcIoFunc)QueueRrcToMac;
    pUE->pLogicChan[chanType]->LCQueue = &bRrcToMac;
    pUE->pLogicChan[chanType]->BufferLength = RrcGetBufferLength;
    pUE->pLogicChan[chanType]->BufferlcID = RrcGetBufferlcID;


    chanType = DCCH;
    chanID = LCID_MIN_LCHAN;
    ret = LteMacCreateLogicChannel(pUE, chanType, chanID);
    pUE->pLogicChan[chanType]->LteMacGetDataBlock = (MacRlcIoFunc)QueueRrcToMac;
    pUE->pLogicChan[chanType]->LCQueue = &bRrcToMac;
    pUE->pLogicChan[chanType]->BufferLength = RrcGetBufferLength;
    pUE->pLogicChan[chanType]->BufferlcID = RrcGetBufferlcID;


    chanType = DTCH;
    chanID = LCID_MIN_LCHAN;
    ret = LteMacCreateLogicChannel(pUE, chanType, chanID);
    pUE->pLogicChan[chanType]->LteMacGetDataBlock = (MacRlcIoFunc)QueueRlcToMac;
    pUE->pLogicChan[chanType]->LCQueue = &RlcAmQueue;
    pUE->pLogicChan[chanType]->BufferLength = RlcGetBufferLength;
    pUE->pLogicChan[chanType]->BufferlcID = RlcGetBufferlcID;

    return ret;
}

/** Initializes default PHY parameters depending on configuration mode
 *
 * @param pMacInitConfig Pointer to configuration structure
 * @return Standard result code
 */
static RESULTCODE LteBsMacConfig(U16 instanceID, MAC_INIT_CONFIG *pMacInitConfig)
{
    int i, j;
    PBS_STATIC_CONFIG pbs_static_conf;

    for (i = 0; i < MAXCHSUBFRAME; i++)
    {
        MacPduDesc[i].NumSduBlock = 0;
        MacPduDesc[i].NumGetBlock = 0;
        MacPduDesc[i].NumPutBlock = 0;

        for (j = 0; j < MAX_SDU_BLOCKS; j++)
        {
            MacPduDesc[i].pSduBlock[j] = NULL;
            MacPduDesc[i].SduBlockLen[j] = 0;
        }
        MacPduDesc[i].pUe = NULL;
    }

    memset(&DlSubFrame, 0, sizeof(DlSubFrame));
    memset(&UlSubFrame, 0, sizeof(UlSubFrame));

    pBsMacDesc = &BsMacDesc;
    memset(pBsMacDesc, 0, sizeof(BS_CONTEXT));

    pBsMacDesc->frameNumber = 0;
    pBsMacDesc->subFrameNumber = 0;
    pBsMacDesc->globalTtiCount = 0;
    pBsMacDesc->sduCount = 0;
    pBsMacDesc->errorCount = 0;

    pBsMacDesc->InstanceID = instanceID;
    pBsMacDesc->predefConfig = pMacInitConfig->macConfMode;

    pbs_static_conf = clr_static_config();
    pbs_static_conf->phy_on_linux_rt = PhyOnLinuxRT;

    // Prepare PHY_INIT.req messages (84xxx-SWG-009-A)
    switch(pMacInitConfig->macConfMode)
    {
        case MACCONF_FDX0:
        case MACCONF_FDX1:
            if (LtePhyParamInit(&pBsMacDesc->initParm,
                                &DlSubFrame,
                                &UlSubFrame,
                                pBsMacDesc->predefConfig) != SUCCESS)
            {
                printf("Error LtePhyParamInit\n");
            }
            pBsMacDesc->initParm.rxSduLineMonitorInfoenable = \
                pMacInitConfig->phyInitParam.rxSduLineMonitorInfoenable;
            pBsMacDesc->schedulingSchema = FDX_SCHEMA;
            break;
        case MACCONF_MAIN:
            printf("MACCONF_MAIN is no more supported\n");
//            memcpy(&pBsMacDesc->initParm, &pMacInitConfig->phyInitParam, sizeof(INITPARM));
//            pBsMacDesc->schedulingSchema = MAIN_SCHEMA;
            break;
        case MACCONF_RCT:
            memcpy(&pBsMacDesc->initParm, &pMacInitConfig->phyInitParam, sizeof(INITPARM));
            pBsMacDesc->schedulingSchema = RCT_SCHEMA;
            break;
        default:
            printf("Incorrect MAC config mode [%i]\n", pMacInitConfig->macConfMode);
            return FAILURE;
    }

    if(pBsMacDesc->initParm.cyclicPrefixType == NORMAL)
        pBsMacDesc->num_IQSamps = 15 * pBsMacDesc->initParm.fftSize;
    else
        pBsMacDesc->num_IQSamps = LTEIQMAX;

    pBsMacDesc->isMIBconfigured = FALSE;

    return SUCCESS;
}

/****************************************************************************
 * Global Functions
 ****************************************************************************/

/** Platform-specific part of MAC initialization
 *  Should be called from OS-independent MacInit()
 *
 * @param instanceID MAC instance ID
 * @param pMacInitConfig MAC configuration parameters
 * @param txCb Callback function which sends API messages to PHY
 * @param rxCb Callback function which receives API messages from PHY
 * @param toPhy Application-specific parameter associated with the txCb function
 * @param fromPhy Application-specific parameter associated with the rxCb parameter
 * @return Standard result code
 */
RESULTCODE LteMacInit(HANDLE pHeap,
                      U16 instanceID,
                      MAC_INIT_CONFIG *pMacInitConfig,
                      PhyTxCb txCb,
                      PhyRxCb rxCb,
                      PTR toPhy,
                      PTR fromPhy)
{
    OSAL_PADDRESS RRC_MACMemBlock;
    TESTMGRINIT testMgrConf = {0};

    testMgrConf.configId = pMacInitConfig->configId;
    testMgrConf.ConsoleValsNum = pMacInitConfig->ConsoleValsNum;
    if(testMgrConf.ConsoleValsNum)
        memcpy(testMgrConf.ConsoleVals, pMacInitConfig->ConsoleVals, sizeof(testMgrConf.ConsoleVals));

    if (LteBsMacConfig(instanceID, pMacInitConfig) != SUCCESS)
    {
        printf("ERROR: MAC config\n");
        return FAILURE;
    }

    if(LteMacDlTaskId == 0)
    {
        if (OsalCreateSema(&LteMacDlSema) != SUCCESS)
        {
            printf("ERROR: MAC DL semaphore\n");
            return FAILURE;
        }

        if (OsalAllocHeap(&pHeap,
                          &RRC_MACMemBlock,
                          MAX_MAC_QUEUE_LEN * sizeof(OSAL_PADDRESS))
                          != SUCCESS)
        {
            printf("ERROR: RRC: No memory [%s]\n", __FUNCTION__);
            return FAILURE;
        }

        if (OsalCreateQueue(&bRrcToMac,
                            RRC_MACMemBlock,
                            (MAX_MAC_QUEUE_LEN * sizeof(OSAL_PADDRESS)))
            != SUCCESS)
        {
            printf("ERROR: RRC queue\n");
            return FAILURE;
        }

        if (UeContextInit() != SUCCESS)
        {
            printf("ERROR: UE context init\n");
            return FAILURE;
        }

        if (RrcInterfaceInit() != SUCCESS)
        {
            printf("ERROR: RRC interface init\n");
            return FAILURE;

        }
        if (PdcchMgrInit() != SUCCESS)
        {
            printf("ERROR: PDCCH manager init\n");
            return FAILURE;

        }
        if (PdschMgrInit() != SUCCESS)
        {
            printf("ERROR: PDSCH manager init\n");
            return FAILURE;

        }

        if (CcchMgrInit() != SUCCESS)
        {
            printf("ERROR: CCCH manager init\n");
            return FAILURE;
        }

        if (LtePhyInterfaceInit(&pBsMacDesc->Phy,
                                txCb,
                                rxCb,
                                toPhy,
                                fromPhy) != SUCCESS)
        {
            printf("ERROR: PHY interface init\n");
            return FAILURE;
        }

        if (LteBsMacDlSchedulerInit(pBsMacDesc) != SUCCESS)
        {
            printf("ERROR: DL scheduler init\n");
            return FAILURE;
        }

        if (LteBsMacUlSchedulerInit(pBsMacDesc) != SUCCESS)
        {
            printf("ERROR: UL scheduler init\n");
            return FAILURE;
        }

        if (TestMgrInit(&testMgrConf) != SUCCESS)
            {
                printf("ERROR: Test config manager init\n");
                return FAILURE;
            }
#ifdef AC_MSPD_TDD
        if(pBsMacDesc->initParm.frtypeDuplexmode == TDD)
        {
            OsalCreateTask(&LteMacDlTaskId,
                           (OSAL_PADDRESS) &LteMacDlTaskTdd,
                           NULL, "LteMacDlTaskTdd",
                           OSAL_PRIORITY_NORMAL, (OSAL_PADDRESS) 0, 0);
            OsalCreateTask(&LteMacUlTaskId,
                           (OSAL_PADDRESS) &LteMacUlTask,
                           NULL, "LteMacUlTask",
                           OSAL_PRIORITY_NORMAL, (OSAL_PADDRESS) 0, 0);
        }
        else
#endif
        if(MacRtMode == MACMODE_NRT)
        {

            OsalCreateTask(&LteMacDlTaskId,
                           (OSAL_PADDRESS) &LteMacDlTask,
                           NULL, "LteMacDlTask",
                           OSAL_PRIORITY_NORMAL, (OSAL_PADDRESS) 0, 0);
            OsalCreateTask(&LteMacUlTaskId,
                           (OSAL_PADDRESS) &LteMacUlTask,
                           NULL, "LteMacUlTask",
                           OSAL_PRIORITY_NORMAL, (OSAL_PADDRESS) 0, 0);
            RlcInit();
        }
        else
        {
            if (TestMgrInitRt(&testMgrConf) != SUCCESS)
            {
                printf("ERROR: Test config manager init\n");
                return FAILURE;
            }
            OsalCreateTask(&LteMacDlTaskId,
                           (OSAL_PADDRESS) &LteMacDlTaskRt,
                           NULL, "LteMacDlTask",
                           OSAL_PRIORITY_NORMAL, (OSAL_PADDRESS) 0, 0);
            OsalCreateTask(&LteMacUlTaskId,
                           (OSAL_PADDRESS) &LteMacUlTaskRt,
                           NULL, "LteMacUlTask",
                           OSAL_PRIORITY_NORMAL, (OSAL_PADDRESS) 0, 0);
        }

        // We don't support all connection procedure, so we create test UE
        // from the beginning
        MacCreateTestLogicalChan();

        OsalActivateTask(&LteMacDlTaskId);
        OsalActivateTask(&LteMacUlTaskId);
    }

    return SUCCESS;
}

/** Stops EmbeddedMAC operation and destroys its variables
 *
 * @return Standard result code
 */
RESULTCODE LteMacDestroy(void)
{
    RESULTCODE ret = SUCCESS;

    if(LteMacDlTaskId != 0)
    {
        if (MacDestroyTestLogicalChan() != SUCCESS)
        {
            printf("ERROR: Destroy test channel\n");
            return FAILURE;
        }
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
        if (TestMgrClean() != SUCCESS)
        {
            printf("Test Config Manager module clean error\n");
            return FAILURE;
        }
        if (LteBsMacUlSchedulerClean(pBsMacDesc) != SUCCESS)
        {
            printf("Uplink Scheduler module clean error\n");
            return FAILURE;
        }
        if (LteBsMacDlSchedulerClean(pBsMacDesc) != SUCCESS)
        {
            printf("Downlink Scheduler module clean error\n");
            return FAILURE;
        }
        if (LtePhyInterfaceClean(&pBsMacDesc->Phy) != SUCCESS)
        {
            printf("PHY interface module clean error\n");
            return SUCCESS;
        }
        if (CcchMgrClean() != SUCCESS)
        {
            printf("Ccch Manager module clean error\n");
            return FAILURE;
        }
        if (PdschMgrClean() != SUCCESS)
        {
            printf("Pdcch Manager module clean error\n");
            return FAILURE;
        }
        if (RrcInterfaceClean() != SUCCESS)
        {
            printf("RRC interface module clean error\n");
            return FAILURE;
        }
        if (PdcchMgrClean() != SUCCESS)
        {
            printf("Pdcch Manager module clean error\n");
            return FAILURE;
        }
        if (UeContextClean() != SUCCESS)
        {
            printf("Ue Context module clean error\n");
            return FAILURE;
        }
        OsalDestroyQueue(&bRrcToMac);
        OsalFreeHeap(OsalGlobalHeap, bRrcToMac.storage);
        OsalDestroySema(&LteMacDlSema);
    }

    return ret;
}

UE_DESC* LteMacNewUeConnected(MacRlcIoFunc LteMacGetDlDataBlock,
                              MacRlcIoFunc LteMacPutUlDataBlock)
{
    UE_DESC *pUe;

    pUe = (UE_DESC *) MacMalloc(sizeof(UE_DESC));
    if (pUe)
    {
        pBsMacDesc->pUe[pBsMacDesc->NUe++] = pUe;
        pUe->DlImcs = 28;
        pUe->UlImcs = 28;
        pUe->DlItbs = Imcs2ItbsTable[pUe->DlImcs];
        pUe->UlItbs = Imcs2ItbsTable[pUe->UlImcs];
        pUe->LteMacPutDataBlock = LteMacPutUlDataBlock;
        pUe->NLogicChannel = 0;
    }
    return pUe;
}

void *MacCreateUE(void)
{
    return LteMacNewUeConnected((MacRlcIoFunc)QueueRrcToMac,
                                (MacRlcIoFunc)QueueMacToRrc);
}

UE_DESC *GetUeContext(U32 UeId)
{
   return pBsMacDesc->pUe[UeId];
}

U32 GetUeId(void)
{
    return 0;
}

volatile U32 gPhyStopReq = 0;

/** Starts or stops periodic PHY operation triggered by TTI
 *  Will send PHY_START.req or PHY_STOP.req message (84xxx-SWG-009-A)
 *  depending on AutoSw parameter
 *
 * @param AutoSw Starts (0) or stops (1) PHY operation
 * @param PhyInstanceId PHY instance ID associated with this MAC
 * @return Standard return code
 */
RESULTCODE LteMacAutoRun(PhyAutoSwitch AutoSw, U32 PhyInstanceId)
{
    RESULTCODE ret = SUCCESS;
    STARTREQ startReq;
    PHY_AUTORUN_CTX* StartAutoRun;

    if(pBsMacDesc == NULL)
    {
        printf("pBsMacDesc is NULL, please do CreateMac first\r\n");
        return FAILURE;
    }

    StartAutoRun = &pBsMacDesc->PhyAutoCtx;

    if (AutoSw == PhyAutoStart)
    {
        startReq.mode = (U16)StartAutoRun->AutoMode;
        startReq.msgType = (U8)PHY_START_REQ;
        startReq.phyEntityId = (U8)PhyInstanceId;
        startReq.period = StartAutoRun->TimerResolution;
        startReq.count = StartAutoRun->SFCount;
        ret = LteMacSendToPhyMessage(PHY_START_REQ, &startReq, 1);
        StartAutoRun->RunEna = TRUE;
    }
    else if(AutoSw == PhyAutoStop)
    {
        startReq.mode = 0;
        startReq.msgType = PHY_STOP_REQ;
        startReq.phyEntityId = PhyInstanceId;
        gPhyStopReq = PhyAutoStop;
//        printf("set flag for send phystop request\n");
        StartAutoRun->RunEna = FALSE;
    }
    else
    {
        startReq.mode = 0;
        startReq.msgType = PHY_SHUTDOWN_REQ;
        startReq.phyEntityId = PhyInstanceId;
        if(StartAutoRun->RunEna == FALSE)
        {
            ret = LteMacSendToPhyMessage(PHY_SHUTDOWN_REQ, &startReq, 1);
        }
        else
        {
            gPhyStopReq = PhyAutoShutdown;
//            printf("set flag for send phystop request\n");
            StartAutoRun->RunEna = FALSE;
        }
    }

    return ret;
}

RESULTCODE LteMacAutoRunNmm(PhyAutoSwitch AutoSw, U32* param)
{
    RESULTCODE ret = SUCCESS;
    STARTREQ startReq;

    if (AutoSw == PhyAutoStart)
    {
        startReq.mode = param[0];
        startReq.period = param[1];
        startReq.count = param[2];
        ret = LteMacSendToPhyNmmMessage(NMM_START, &startReq, 1);
    }
    else if(AutoSw == PhyAutoStop)
    {
        ret = LteMacSendToPhyNmmMessage(NMM_STOP, NULL, 1);
    }
    else
    {
    }

    return ret;
}

RESULTCODE LteMacNmmCmd(U32 cmdId)
{
    RESULTCODE ret = SUCCESS;

    ret = LteMacSendToPhyNmmMessageTest(NMM_CMD, &cmdId, 1);

    return ret;
}

#endif /* BSMAC || ENODEB */

