/**
 * @file    pmh_s1ap_if.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will contains functions to process different messages received from S1AP
 *
 * @author  Ramprasad Reddy Chinta
 * @date    19-02-2011
 * @details This file contains functions to process different messages received from S1AP
 */

#include "pmh.h"


/**
 * @param   msgIn Message received
 * @param   pmhContext PMH Context
 * @details This function processes MSG_S1AP_PMH_GEN_STAT_RSP message receive from S1AP.
 * */
PmhState pmhProcS1apGenStsRsp(
  IN Msg        *msgIn,
  IO PmhContext *pmhContext
  )
{
  MsgSmPmhS1apGenStatRsp *rcvdRsp    = (MsgSmPmhS1apGenStatRsp *)msgIn->data;
  S1apGenCounters *rowOfTable = (S1apGenCounters*)getFirstRowOfTable(S1APGENCOUNTERS);

  /* Store received counter values in global varible */
  copyBytes( &(rcvdRsp->s1apGenCounters), sizeof(S1apGenCounters), rowOfTable );
  rowOfTable->index = 0;

  /* Update last collected time in logStatus table */
  getAndFillDateAndTimeStamp( MAX_SIZE_S1APGENCOUNTERS_LASTCOLLECTEDAT, rowOfTable->lastCollectedAt );

  /* Check if response was received due to get cmd on this table, if yes, update the table and return */
  if ( rcvdRsp->transId )
  {
    if(TRUE EQ pmhContext->resetAllCounters)
    {
      pmhContext->rcvdClearCounterRsp.s1apGenCounter    = TRUE;
      pmhProcClearCounterRsp( pmhContext, rcvdRsp->transId );
    }
    else
    {
      sendGetObjRspSuccToBsm( rcvdRsp->transId, pmhContext );
    }
    return pmhContext->pmhState;
  }

  /* Update the runtime counter flags */
  pmhContext->rcvdCounterRsp.s1apGenCounter = TRUE;

  if(pmhContext->fp NOTEQ NULL)
  {
    /* This check of file exists is required, because operator can delete the
     * file using upload cmd with delete file after upload option */
    /* In this case PMH should create a new file with make a new entry in the pmLogFileStatusTable */
    if ( FALSE EQ checkIfCurrentStsLogFileExists( pmhContext->currPmLogFileName ) )
    {
      logInfo(" Current logging statistics file does not exists Filename:%s", pmhContext->currPmLogFileName );
      pmhClearPmLogFileStatusRow( pmhContext->currPmLogFileName );
      /* Open PM Log file to collect remaining counter values */
      if(TRUE EQ pmhOpenPmLogFile(pmhContext))
      {
        pmhAddEntryInPmLogFileStatusTable(pmhContext);
      }
      logInfo(" New file is created [filename:%s] "
          " and further will be done in this file", pmhContext->currPmLogFileName );
    }
    writeToFileS1apGenCounterTable(rowOfTable, pmhContext->fp);
  }

  if(TRUE EQ pmhStsRspRcvdFromAllModules( &pmhContext->sendCounterReq, &pmhContext->rcvdCounterRsp ) )
  {
    doCounterCollectionProcCompletionSteps( pmhContext );
  }
  return pmhContext->pmhState;
}

/**
 * @param   msgIn Message received
 * @param   pmhContext PMH Context
 * @details This function processes MSG_S1AP_PMH_PEER_NODE_STAT_RSP message receive from S1AP.
 * */
PmhState pmhProcS1apPeerNodeStsRsp(
  IN Msg *msgIn,
  IO PmhContext *pmhContext
  )
{
  MsgSmPmhS1apPeerNodeStatRsp *rcvdRsp    = (MsgSmPmhS1apPeerNodeStatRsp *)msgIn->data;
  S1apPeerNodeCounters *rowOfTable = (S1apPeerNodeCounters*)getFirstRowOfTable(S1APPEERNODECOUNTERS);

  /* Store received counter values in global varible */
  copyBytes( &(rcvdRsp->s1apPeerNodeCounters), sizeof(S1apPeerNodeCounters), rowOfTable );
  rowOfTable->index = 0;

  /* Update the Last collected time */
  getAndFillDateAndTimeStamp( MAX_SIZE_S1APPEERNODECOUNTERS_LASTCOLLECTEDAT, rowOfTable->lastCollectedAt );

  /* Check if response was received due to get cmd on this table, if yes, update the table and return */
  logInfo(" pmhContext->resetAllCounters = %d",pmhContext->resetAllCounters);
  if ( rcvdRsp->transId )
  {
    if(TRUE EQ pmhContext->resetAllCounters)
    {
      pmhContext->rcvdClearCounterRsp.s1apPeerNodeCounter    = TRUE;
      pmhProcClearCounterRsp( pmhContext, rcvdRsp->transId );
      logInfo("rcvdClearCounterRsp.s1apPeerNodeCounter= %d, sendClearCounterReq.s1apPeerNodeCounter= %d",
      pmhContext->rcvdClearCounterRsp.s1apPeerNodeCounter, pmhContext->sendClearCounterReq.s1apPeerNodeCounter);
    }
    else
    {
      sendGetObjRspSuccToBsm( rcvdRsp->transId, pmhContext );
    }
    return pmhContext->pmhState;
  }

  /* Update the runtime counter flags */
  pmhContext->rcvdCounterRsp.s1apGenCounter = TRUE;

  if(pmhContext->fp NOTEQ NULL)
  {
    /* This check of file exists is required, because operator can delete the
     * file using upload cmd with delete file after upload option */
    /* In this case PMH should create a new file with make a new entry in the pmLogFileStatusTable */
    if ( FALSE EQ checkIfCurrentStsLogFileExists( pmhContext->currPmLogFileName ) )
    {
      logInfo(" Current logging statistics file does not exists Filename:%s", pmhContext->currPmLogFileName );
      pmhClearPmLogFileStatusRow( pmhContext->currPmLogFileName );
      /* Open PM Log file to collect remaining counter values */
      if(TRUE EQ pmhOpenPmLogFile(pmhContext))
      {
        pmhAddEntryInPmLogFileStatusTable(pmhContext);
      }
      logInfo(" New file is created [filename:%s] and further will be done in this file", pmhContext->currPmLogFileName );
    }
    writeToFileS1apPeerNodeCounterTable(rowOfTable, pmhContext->fp);
  }

  if(TRUE EQ pmhStsRspRcvdFromAllModules( &pmhContext->sendCounterReq, &pmhContext->rcvdCounterRsp ) )
  {
    doCounterCollectionProcCompletionSteps( pmhContext );
  }
  return pmhContext->pmhState;
}


/**
 * @param   cmdRef  Cmd Ref
 * @param   tagInfo tag information
 * @param   pmhContext PMH Context
 * @details This function creates and sends MSG_PMH_SM_S1AP_GEN_STAT_REQ to S1AP.
 * */
void createAndSendS1apGenStatReq(
  IN CmdRef      *cmdRef,
  IN TagInfo     *tagInfo,
  IO PmhContext  *pmhContext
  )
{
  UINT32                 transId = 0 ;
  BsErrorValue           error;
  MsgPmhSmS1apGenStatReq  req;
  clearBytes(&req, sizeof(MsgPmhSmS1apGenStatReq));

  /* Allocate transId and store CmdRef and TagInfo*/
  error = pmhAllocTransId(cmdRef, tagInfo, &transId, pmhContext);
  if(BS_NO_ERROR NOTEQ error)
  {
    sendGetObjRspFailToBsm(cmdRef, tagInfo, &error);
    return;
  }
  req.transId          = transId;
  req.resetAllCounters = FALSE;
  createAndSendMsg( MSG_PMH_SM_S1AP_GEN_STAT_REQ, (UINT8 *)&req, sizeof(MsgPmhSmS1apGenStatReq) );
}

/**
 * @param   cmdRef  Cmd Ref
 * @param   tagInfo tag information
 * @param   peerId  Peer Id
 * @param   pmhContext PMH Context
 * @details This function creates and sends MSG_PMH_SM_S1AP_PEER_NODE_STAT_REQ to S1AP.
 * */
void createAndSendS1apPeerNodeStatReq(
  IN CmdRef       *cmdRef,
  IN TagInfo      *tagInfo,
  IN UINT32        peerId,
  IO PmhContext   *pmhContext
  )
{
  UINT32                 transId = 0 ;
  BsErrorValue           error;
  MsgPmhSmS1apPeerNodeStatReq  req;
  clearBytes(&req, sizeof(MsgPmhSmS1apPeerNodeStatReq));

  /* Allocate transId and store CmdRef and TagInfo*/
  error = pmhAllocTransId(cmdRef, tagInfo, &transId, pmhContext);
  if(BS_NO_ERROR NOTEQ error)
  {
    sendGetObjRspFailToBsm(cmdRef, tagInfo, &error);
    return;
  }
  req.transId          = transId;
  req.peerId           = peerId;
  req.resetAllCounters = FALSE;
  createAndSendMsg( MSG_PMH_SM_S1AP_PEER_NODE_STAT_REQ, (UINT8 *)&req, sizeof(MsgPmhSmS1apPeerNodeStatReq) );
}

