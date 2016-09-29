/**
 * @file    bsm_cmd_lock.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions related to Lock/Disable procedure.
 *
 * @author:  Shweta Polepally
 * @date:    Sep 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param   msgId      Message ID
 * @param   value      Value to be set - TRUE/FALSE
 * @param   bsmContext BSM Context
 * @details This function will set the value - enabled/disabled.
 */
void setEnbEnableDisableParams(
  IN MsgId       msgId,
  IN TBOOL       value,
  IO BsmContext  *bsmContext
  )
{
  switch( msgId )
  {
    case MSG_S1M_BSM_OPSTART_SUCC:
    case MSG_S1M_BSM_OPSTART_FAIL:
    case MSG_S1M_BSM_OPSTOP_SUCC:
    case MSG_S1M_BSM_LINK_DOWN_IND:
    case MSG_S1M_BSM_LINK_UP_IND:
    case MSG_S1M_BSM_RESET_IND:
      bsmContext->enbEnableDisableParams.isS1SetupSuccessful = value;
      break;
    case MSG_DEH_BSM_OPSTART_SUCC:
    case MSG_DEH_BSM_OPSTART_FAIL:
    case MSG_DEH_BSM_OPSTOP_SUCC:
      bsmContext->enbEnableDisableParams.isRfTx = value;
      break;
    case MSG_CEM_BSM_OPSTART_SUCC:
    case MSG_CEM_BSM_OPSTART_FAIL:
    case MSG_CEM_BSM_OPSTOP_SUCC:
      bsmContext->enbEnableDisableParams.isS1Tx = value;
      break;
    case MSG_PMH_BSM_OPSTART_SUCC:
    case MSG_PMH_BSM_OPSTART_FAIL:
    case MSG_PMH_BSM_OPSTOP_SUCC:
      bsmContext->enbEnableDisableParams.isPmhFunctional = value;
      break;
    case MSG_UDH_BSM_OPSTART_SUCC:
    case MSG_UDH_BSM_OPSTART_FAIL:
    case MSG_UDH_BSM_OPSTOP_SUCC:
      bsmContext->enbEnableDisableParams.isUdhFunctional = value;
      break;
    default:
      /* Nothing to set */
      logError("Set/Reset of global cannot be done. No context for Message %s", getMsgName(msgId));
      break;
  }
}

/**
 * @param   bsmContext BSM Context
 * @details This function will perform disable procedure steps when enable procedure is interrupted.
 */
void doDisableProcInitStepsWhenEnableIsInterrupted(
  IO BsmContext       *bsmContext
  )
{
  bsmContext->opStartOpStopInfo.numModulesToSendOpStop = bsmContext->opStartOpStopInfo.opStartSeqIndex-1;
  if ( 0 < bsmContext->opStartOpStopInfo.numModulesToSendOpStop )
  {
    setBsmProceduralState( bsmContext, ENODEB_WAIT_DISABLE_CMPL );
    startBsmProcTimer( TIMERNAME_WAITDISABLECOMPLETE, &bsmContext->bsmTimersInfo );
  }
}

/**
 * @param   bsmContext BSM Context
 * @details This function will perform disable procedure completion steps.
 */
void doDisableProcedureCompletionSteps(
  IO BsmContext       *bsmContext
  )
{
  stopBsmProcedureTimer( TIMERNAME_WAITDISABLECOMPLETE );
  setOperationalStateInEnbStatusTable( OPERATIONALSTATE_DISABLED );
  setBsmProceduralState( bsmContext, BS_IDLE );
}

/**
 * @param   msg        Message from PRM
 * @param   bsmContext BSM Context
 * @details This function will process MSG_XXX_BSM_OPSTOP_SUCC message.
 */
BsmProceduralState bsmProcOpStopSuccInWaitDisableState(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  setEnbEnableDisableParams( msg->msgId, FALSE, bsmContext );

  if ( TRUE EQ bsmContext->opStartOpStopInfo.opStopCmpl )
  {
    doDisableProcedureCompletionSteps( bsmContext );
  }
  else
  {
    sendOpStopReqMsg( &bsmContext->opStartOpStopInfo );
  }

  /* Update CellStatus table if OPSTOP_SUCC is received from CEM */
  if(MSG_CEM_BSM_OPSTOP_SUCC EQ msg->msgId)
  {
    bsmUpdateCellOperationalStateInfo((UINT8 *)getEnbDisableReasonName(bsmContext->enbDisableReason));
    bsmSetIsOperationalInCellStatusTable(FALSE);
  }

  return bsmContext->bsmProceduralState;
}

/**
 * @param   doDisableProcedure Do disable procedure steps.
 * @param   bsmContext  BSM module related information.
 * @details This function will carry out enodeB disabled procedure.
 *          Procedure starts with sending OPSTOP_REQ message to UEH first,
 *          followed by DEH and then S1AP on receiving success.
 */
void enbDisable(
  IN TBOOL        doDisableProcedure,
  IO BsmContext  *bsmContext
  )
{
  BsmTimersInfo   *timerInfo = getBsmTimersInfo();
  logInfo(" EnodeB Disable Reason : %s", getEnbDisableReasonName(bsmContext->enbDisableReason) );

  /* Check if trace is on, if yes , stop the trace and go for disable procedure */
  TraceStatus            *traceStatus = (TraceStatus*)getFirstRowOfTable(TRACESTATUS);
  if ( TRACESTATE_TRACEON EQ traceStatus->traceState )
  {
    logInfo("%s", "LOCK Command received. Tracing in ON. Stop Protocol Tracing.");
    //stopProtocolTracing( &bsmContext->traceInfoContext, "Trace stopped due to lock command" );
    /* Indicate SIH that Operator stopped the trace */
    //sendEventMessageTraceCompleted( traceStatus->traceFileName );
  }

  /* 1. DoDisable=TRUE and disabled/enabled => DO OPSTOP Procedure
   * 2. DoDisable=FALSE => Nothing to do */
  /* Case 2: */
  if ( (FALSE EQ doDisableProcedure) )// && (FALSE EQ isEnodeBEnabled()) )
  {
    /* Locked and disabled */
    logInfo("%s", " EnodeB is already locked and disabled.. Do Nothing  !! ");
    setBsmProceduralState( bsmContext, BS_IDLE );
    return;
  }

  /* Case 1: Previously Unlocked and disabled
   * doDisableProcedure = TRUE &
   * Enodeb should be enabled */
  /* Initialise variables */
  logInfo("%s", " EnodeB is Unlocked and Disabled/Enabled - Do disable procedure !! ");
  bsmContext->opStartOpStopInfo.opStopCmpl             = FALSE;
  bsmContext->opStartOpStopInfo.numModulesToSendOpStop = NUM_MODULES_OPSTART_STOP_SENT;

  if( USER_INITAITED_LOCK EQ bsmContext->enbDisableReason )
  {
    bsmCreateAndSendSetLedColor( LEDID_ENBOPERATIONAL, LEDCOLOR_AMBER, bsmContext );
  }
  else if( 0 NOTEQ getNumActiveCriticalAlarms() )
  {
    bsmCreateAndSendSetLedColor( LEDID_ENBOPERATIONAL, LEDCOLOR_RED, bsmContext );
  }

  setBsmProceduralState( bsmContext, ENODEB_WAIT_DISABLE_CMPL );
  startBsmProcTimer( TIMERNAME_WAITDISABLECOMPLETE, timerInfo );

  /* Sequence of Opstart and opstop is defined when BSM is initialized */
  /* Start enable procedure here */
  sendOpStopReqMsg( &bsmContext->opStartOpStopInfo );
}

/**
 * @param   setObj     SET Object Request message.
 * @param   bsmContext BSM Context
 * @details This function will carry out unlock procedure to enable the enodeB.
 *          User Initiated lock procedure.
 */
void bsmDoLock(
  IN SetObjReq   *setObj,
  IO BsmContext  *bsmContext
  )
{
  TBOOL  doDisableProcedure = FALSE;
  /* This will stop all the timers running in BSM. This is mainly required to stop all the
   * on going procedures like recovery procedures, etc at BSM. */
  stopallTimers();

  if ( FALSE EQ isEnodeBLocked() )
  {
    doClearAlarmsUponLock( );
    doDisableProcedure = TRUE;
  }

  setAdminStateInEnbStatusTable ( ADMINSTATE_LOCKED );

  /* Set the MME Link and S1 State to False */
  bsmContext->isMmeLinkEstablished                         = FALSE;
  bsmContext->enbEnableDisableParams.isS1SetupSuccessful   = FALSE;
  bsmContext->numOfRetyForMmeLinkEstablish                 = 0;

  bsmFillAndSendSetObjSucc( setObj );
  bsmContext->enbDisableReason = USER_INITAITED_LOCK;

  enbDisable( doDisableProcedure, bsmContext );
}

