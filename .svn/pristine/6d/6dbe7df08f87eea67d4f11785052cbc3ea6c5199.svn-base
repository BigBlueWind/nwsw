/**
 * @file    bsm_sw_activate.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions for software activation procedure at BSM.
 *
 * @author:  Shweta Polepally
 * @date:    22 Nov 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param   msg        Message received from PRM
 * @param   bsmContext BSM module information.
 * @details This function handles SW_ACTIVATE_RSP message from PRM module and relay it to OAM-IF.
 */
BsmProceduralState bsmProcSwActivateRsp(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  SetObjReq               setObjReq;
  clearBytes(&setObjReq, sizeof(SetObjReq));

  decSetObjReqMsg( (UINT8*)msg->data, msg->len, &setObjReq );
  bsmFillAndSendSetObjSucc(&setObjReq);
  if (TRUE EQ bsmContext->emsRegSucc )
  {
    sendEventEnbReboot("Software Activation");
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param   msg        Message received from PRM
 * @param   bsmContext BSM module information.
 * @details This function handles SW_ACTIVATE_REJ message from PRM module and relay it to OAM-IF.
 */
BsmProceduralState bsmProcSwActivateRej(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  SetObjRspFail                setObjFail;
  clearBytes(&setObjFail, sizeof(SetObjRspFail));

  decSetObjRspFailMsg((UINT8*)msg->data, msg->len, &setObjFail );
  if (TRUE EQ bsmContext->emsRegSucc )
    setBsmProceduralState( bsmContext, BS_IDLE );
  else
    setBsmProceduralState( bsmContext, BS_WAIT_REGISTRATION_CMPL );

  bsmCreateAndSendSetObjFail(&setObjFail);
  return bsmContext->bsmProceduralState;
}

/**
 * @param   msg        Message received from PRM
 * @param   bsmContext BSM module information.
 * @details This function handles SW_ACTIVATE_SUCC message from PRM module.
 *          MSG_BSM_SIH_NOTI_IND with id=ALARM_ID_SW_ACTIVATE_SUCCESS will be posted to SIH.
 */
BsmProceduralState bsmProcSwActivateSucc(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  MsgPrmBsmSwActivateSucc  *succMsg = (MsgPrmBsmSwActivateSucc*)msg->data;

  if (TRUE EQ bsmContext->emsRegSucc )
  {
    sendEventSwActivationSuccess( succMsg->swType, succMsg->version );
  }
  /* else NOTI will be dropped */
  return ( bsmContext->bsmProceduralState );
}

/**
 * @param   msg        Message received from PRM
 * @param   bsmContext BSM module information.
 * @details This function handles SW_ACTIVATE_FAIL message from PRM module,
 *          MSG_BSM_SIH_NOTI_IND with id=ALARM_ID_SW_ACTIVATE_FAILURE will be posted to SIH.
 */
BsmProceduralState bsmProcSwActivateFail(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  MsgPrmBsmSwActivateFail  *failMsg = (MsgPrmBsmSwActivateFail*)msg->data;

  if (TRUE EQ bsmContext->emsRegSucc )
  {
    /* Send SwActivationFailed Event to SIH */
    sendEventSwActivationFailed( failMsg->swType, failMsg->version,  errorCodeStr(failMsg->errorDesc.errorNum) );
    sendEventEnbReboot("SW Activation Failed. Fallback performed");
  }
  /* else NOTI will be dropped */
  return ( bsmContext->bsmProceduralState );
}

/**
 * @param   msg        Message received from PRM
 * @param   bsmContext BSM module information.
 * @details This function handles SW_ACTIVATE_RSP message from PRM module and relay it to OAM-IF.
 */
BsmProceduralState bsmProcEnbRebootRsp(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  SetObjReq               setObjReq;
  clearBytes(&setObjReq, sizeof(SetObjReq));

  decSetObjReqMsg( (UINT8*)msg->data, msg->len, &setObjReq );
  bsmFillAndSendSetObjSucc(&setObjReq);
  if (TRUE EQ bsmContext->emsRegSucc )
  {
    sendEventEnbReboot("Operator Initiated Reboot");
  }
  return bsmContext->bsmProceduralState;
}
