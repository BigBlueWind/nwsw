/** @file rrc_main.c
 *
 * @brief RRC implementation for eNodeB
 * @author Mindspeed Technologies
 * @version $Revision: 1.21 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef ENODEB

#include "osal.h"
#ifdef __MINDSPEED_RCT_CODEC__
#include "rct_lte_types.h"
#endif
#include "rrc_types.h"
#include "ctrlmsg.h"
#include "ltemac.h"
//#include "LteMac2RrcInterface.h"

PtRRCCtx RRC_Ctx;

extern OSAL_QUEUE bRrcToMac;


U32 MAC_Init(void);
U32 RRC_ConnectionRequest(void* Ctx);


U32 RRC_Ctx_Init(HANDLE pHeap)
{
    OSAL_PADDRESS pBlockToUpper;

    RRC_Ctx = malloc(sizeof(tRRCCtx));
    if (RRC_Ctx == NULL)
    {
        printf("EnodeB Rrc Init Error: context allocated error\n");
        return 1;
    }
    RRC_Ctx->hHeap = pHeap;

    /*
    *    queue which collect the responses and indication to upper layer
    */
     if(OsalAllocHeap(&RRC_Ctx->hHeap,
         &pBlockToUpper,
         10 * sizeof(U32)))
    {
        printf("EnodeB Rrc Init Error: cannot allocate the RRc block\n");
        return 1;
    }

    RRC_Ctx->Status = Idle;

    return SUCCESS;
}

void RRC_Deactivate(void)
{
    RRC_Ctx->Status = Idle;
    printf("EnodeB Rrc State Info: Rrc is the idle\n");
}

void RRC_Activate(void)
{
    RRC_Ctx->Status = Connected;
    printf("EnodeB Rrc State Info: Rrc is the connection mode\n");
}

void RRC_Destroy(void)
{
    RRC_Ctx->Status = Idle;
    OsalDestroyQueue(&ToRRCQueue);
    OsalDestroyHeap(&RRC_Ctx->hHeap);
    printf("EnodeB Rrc State Info: Rrc connection released\n");
}

RESULTCODE RRC_ConnectSetup(void* pParam, U16 ParamLen)
{
    RESULTCODE ret = SUCCESS;
    OsalEnqueue(&bRrcToMac, pParam);
    return ret;
}

RESULTCODE RRC_ConnectRequest(void* handle, void* pParam, U16 ParamLen)
{
    PMSGHEADER msg = (PMSGHEADER) handle;
    rrcInfoData* p_rrc_info = (rrcInfoData*)pParam;
#ifdef __MINDSPEED_RCT_CODEC__
    TRRCConnectionRequestR8IEs* ReqParam = (TRRCConnectionRequestR8IEs*)(p_rrc_info->data_p);
#endif
    void* pUe = NULL;
    U32 Len = 0;
    PPARAMHEADER param_temp = (PPARAMHEADER) &msg->param[0];
    RESULTCODE res = SUCCESS;

    param_temp->paramID = MAC_CREATE_UE_ENTITY;

    param_temp->data[0] = p_rrc_info->ueIdx;//ReqParam->ueIdentity;//  //!! UEId from MME
#ifdef __MINDSPEED_RCT_CODEC__
    param_temp->data[1] = ReqParam->establishmentCause; //priority
#else
    param_temp->data[1] = 0; //priority
#endif
    param_temp->data[2] = 0xAA;// DlMod
    param_temp->data[3] = 0xBB;// UlMod
    param_temp->data[4] = 0xDD;// DLMaxRB
    param_temp->data[5] = 0xEE;// ULMaxRB
    param_temp->data[6] = 0xDD;// DLCodRate
    param_temp->data[7] = 0xEE;// UlCodRate
    param_temp->length = sizeof(U32) * 8;
    Len = param_temp->length + sizeof(PPARAMHEADER);

    param_temp += Len;

    param_temp->paramID = PAR_CREATE_LC;
    param_temp->data[0] = p_rrc_info->ueIdx;//ReqParam->ueIdentity;
    param_temp->data[1] = DCCH;
    param_temp->data[2] = 1; // default LC parameters
    param_temp->length = sizeof(U32) * 10;

    Len += param_temp->length + sizeof(PPARAMHEADER);

    param_temp += Len;

    param_temp->paramID = 0;
    param_temp->length = 0;

    msg->length += sizeof(PARAMHEADER);

    return res;
}
RESULTCODE RRC_DebugData(void* pParam, U16 ParamLen)
{
    OsalEnqueue(&bRrcToMac, pParam);
    return SUCCESS;
}

#endif /* ENODEB */
