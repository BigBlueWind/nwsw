/**
 * @file    bsm_counters.c
 * @details Copyright Powerwave Technologies, Inc., 2011
 * @brief   This file will handle messages related to performance counters.
 *
 * @author:  Shweta Polepally
 * @date:    Mar 2011
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param    msg         Message received.
 * @param    bsmContext  BSM module related information.
 * @details  This function handles COUNTER_FILE_AVAILABLE message from PMH and send notification
 *           to SIH.
 */
BsmProceduralState bsmProcPmhCounterFileAvailable(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  MsgPmhBsmCntrFileAvailable  *counterFileAvailable = (MsgPmhBsmCntrFileAvailable*)msg->data;
  sendEventCounterFileAvailable( counterFileAvailable->pmLogFileName );

  return bsmContext->bsmProceduralState;
}

/**
 * @param    msg        Message received.
 * @param    bsmContext BSM module related information.
 * @details  This function handles COUNTER_COLLECTION_FAILED message from PMH and send notification
 *           to SIH.
 */
BsmProceduralState bsmProcPmhCounterCollectionFailed(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  MsgPmhBsmCntrCollectFail  *counterCollectionFailed = (MsgPmhBsmCntrCollectFail*)msg->data;
  sendEventCounterCollectionFailed( counterCollectionFailed->errorDesc.failReason );

  return bsmContext->bsmProceduralState;
}

/**
 * @param    msg        Message received.
 * @param    bsmContext BSM module related information.
 * @details  This function handles MSG_PMH_BSM_MEASUREMENT_CMPL_IND message from PMH and send notification
 *           to SIH.
 */
BsmProceduralState bsmProcMeasCmplInd(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  MsgPmhBsmMeasurementCompleteInd *ind = (MsgPmhBsmMeasurementCompleteInd*)msg->data;
  sendEventMeasurementCompleteInd(ind );

  return bsmContext->bsmProceduralState;
}

