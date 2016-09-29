/** @file enbMac.c
 *
 * @brief MAC initialization functions specific to 4GMX OS (EmbeddedMAC)
 * @author Mindspeed Technologies
 * @version $Revision: 1.24 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include <string.h>
#include "bsmac.h"
#include "LtePhyL2Api.h"
#include "enbMacIface.h"
#include "enbMac2PhyIface.h"
#include "LteBsMacDlScheduler.h"
#include "LteBsMacUlScheduler.h"
#include "LteMacTestCfgMgr.h"
#include "lte_entry.h"

/****************************************************************************
 * Global Variables
 ****************************************************************************/

BS_CONTEXT *pBsMacDesc = NULL;
DLSUBFRDESC DlSubFrame;
ULSUBFRDESC UlSubFrame;
V32 globalTTITickCount;

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

static BS_CONTEXT BsMacDesc;

/****************************************************************************
 * External Functions
 ****************************************************************************/

extern void SetMacInstanceId(UINT32 MacId);
extern UINT32 GetMacInstanceId(void);

/****************************************************************************
 * Module Local Functions
 ****************************************************************************/

/** Initializes default PHY parameters depending on configuration mode
 *
 * @param pMacInitConfig Pointer to configuration structure
 * @return Standard result code
 */
static RESULTCODE eNbMacConfig(MAC_INIT_CONFIG *pMacInitConfig)
{
    memset(&DlSubFrame, 0, sizeof(DlSubFrame));
    memset(&UlSubFrame, 0, sizeof(UlSubFrame));

    pBsMacDesc = &BsMacDesc;
    memset(pBsMacDesc, 0, sizeof(BS_CONTEXT));

    // Prepare PHY_INIT.req messages (84xxx-SWG-009-A)
    switch (pMacInitConfig->macConfMode)
    {
        case MACCONF_FDX0:
        case MACCONF_FDX1:
            LtePhyIfInitDefault(&pBsMacDesc->initParm);
            pBsMacDesc->initParm.rxSduLineMonitorInfoenable
                = pMacInitConfig->phyInitParam.rxSduLineMonitorInfoenable;
            break;
        case MACCONF_MAIN:
            memcpy(&pBsMacDesc->initParm, &pMacInitConfig->phyInitParam, sizeof(INITPARM));
            pBsMacDesc->schedulingSchema = MAIN_SCHEMA;
            break;
        case MACCONF_RCT:
            memcpy(&pBsMacDesc->initParm, &pMacInitConfig->phyInitParam, sizeof(INITPARM));
            pBsMacDesc->schedulingSchema = RCT_SCHEMA;
            break;
        default:
            printf("Incorrect MAC config mode [%i]\n", pMacInitConfig->macConfMode);
            return FAILURE;
    }

    pBsMacDesc->isMIBconfigured = FALSE;
    pBsMacDesc->HarqEna = HARQ_OFF;

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
RESULTCODE eNbMacInit(U16 instanceID, 
                      MAC_INIT_CONFIG *pMacInitConfig, 
                      PhyTxCb txCb, 
                      PhyRxCb rxCb,
                      PTR toPhy, 
                      PTR fromPhy)
{
    TESTMGRINIT testMgrConf = { 0 };

    testMgrConf.configId = pMacInitConfig->configId;
    testMgrConf.ConsoleValsNum = pMacInitConfig->ConsoleValsNum;
    if(testMgrConf.ConsoleValsNum)
    memcpy(testMgrConf.ConsoleVals, pMacInitConfig->ConsoleVals, sizeof(testMgrConf.ConsoleVals));

    if (eNbMacConfig(pMacInitConfig) != SUCCESS)
    {
        printf("ERROR: DL scheduler init\n");
        return FAILURE;
    }

    if (LtePhyIfaceInit(&pBsMacDesc->Phy, 
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

    SetMacInstanceId(instanceID);
    SetUNmmInstanceId(instanceID);

	lte_bs_phy_umtsnmm_init_api_thread();

    return SUCCESS;
}

/** Stops EmbeddedMAC operation and destroys its variables
 *
 * @return Standard result code
 */
RESULTCODE eNbMacDestroy(void)
{
    RESULTCODE ret = SUCCESS;

    if(GetMacInstanceId() != 0)
    {
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
        if (LtePhyIfaceClean(&pBsMacDesc->Phy) != SUCCESS)
        {
            printf("PHY interface module clean error\n");
            return SUCCESS;
        }

        SetMacInstanceId(0);
    }

    SetUNmmInstanceId(0);

    return ret;
}

/** Starts or stops periodic PHY operation triggered by TTI
 *  Will send PHY_START.req or PHY_STOP.req message (84xxx-SWG-009-A)
 *  depending on AutoSw parameter
 *
 * @param AutoSw Starts (0) or stops (1) PHY operation
 * @param PhyInstanceId PHY instance ID associated with this MAC
 * @return Standard return code
 */
RESULTCODE eNbMacAutoRun(PhyAutoSwitch AutoSw, U32 PhyInstanceId)
{
    RESULTCODE ret = SUCCESS;
    STARTREQ startReq;
    PHY_AUTORUN_CTX* StartAutoRun = &pBsMacDesc->PhyAutoCtx;

    if(pBsMacDesc == NULL)
    {
        uart_printf("pBsMacDesc is NULL, please do CreateMac first\r\n");
        return FAILURE;
    }

    if (AutoSw == PhyAutoStart)
    {
        startReq.mode = (U16) StartAutoRun->AutoMode;
        startReq.msgType = (U8) PHY_START_REQ;
        startReq.phyEntityId = (U8) PhyInstanceId;
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
        ret = LteMacSendToPhyMessage(PHY_STOP_REQ, &startReq, 1);
        StartAutoRun->RunEna = FALSE;
    }
    else
    {
        startReq.mode = 0;
        startReq.msgType = PHY_SHUTDOWN_REQ;
        startReq.phyEntityId = PhyInstanceId;
        ret = LteMacSendToPhyMessage(PHY_SHUTDOWN_REQ, &startReq, 1);
        StartAutoRun->RunEna = FALSE;
    }

    return ret;
}
