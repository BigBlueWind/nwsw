/**
 * @file    pmh_mac_if.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will contains functions to process different messages received from MAC
 *
 * @author  Ramprasad Reddy Chinta
 * @date    19-02-2011
 * @details This file contains functions to process different messages received from MAC
 */

#include "pmh.h"

/**
 * @param  transId  transId
 * @param   ctx    PMH context
 * @details This function updates GTP user plane counters table and writes to file
 * */
static TBOOL updatePmLogFileForMacUplaneCounters(
  IN UINT32     transId,
  IO PmhContext *ctx
  )
{
  MacUplaneCounters  *rowOfTable = (MacUplaneCounters*)getFirstRowOfTable(MACUPLANECOUNTERS);

  /* Update last collected time in logStatus table */
  getAndFillDateAndTimeStamp( MAX_SIZE_MACUPLANECOUNTERS_LASTCOLLECTEDAT, rowOfTable->lastCollectedAt );

  logInfo("ctx->rspRspFromMacRclAndMacPhyCounter = %d", ctx->rspRspFromMacRclAndMacPhyCounterForGetCmd );
  if( EXPECTED_RSP_FROM_MAC_COUNTERS EQ ctx->rspRspFromMacRclAndMacPhyCounterForGetCmd )
  {
    ctx->rspRspFromMacRclAndMacPhyCounterForGetCmd = 0;
    if ( transId )
    {
      if(TRUE EQ ctx->resetAllCounters)
      {
        ctx->rspRspFromMacRclAndMacPhyCounter = 0;
        ctx->rcvdClearCounterRsp.macUplaneCounter = TRUE;
        pmhProcClearCounterRsp( ctx, transId );
      }
      else
      {
        sendGetObjRspSuccToBsm( transId, ctx );
      }
      return FALSE;
    }
  }
  if(transId)
  {
    return FALSE;
  }

  logInfo("ctx->rspRspFromMacRclAndMacPhyCounter = %d", ctx->rspRspFromMacRclAndMacPhyCounter );
  /* Write counter values to file if both RLC and PHY statistics are received */
  if( EXPECTED_RSP_FROM_MAC_COUNTERS EQ ctx->rspRspFromMacRclAndMacPhyCounter )
  {
    ctx->rspRspFromMacRclAndMacPhyCounter = 0;
    ctx->rcvdCounterRsp.macUplaneCounter = TRUE;
    if(ctx->fp NOTEQ NULL)
    {
      /* This check of file exists is required, because operator can delete the
       * file using upload cmd with delete file after upload option */
      /* In this case PMH should create a new file with make a new entry in the pmLogFileStatusTable */
      if ( FALSE EQ checkIfCurrentStsLogFileExists( ctx->currPmLogFileName ) )
      {
        logInfo(" Current logging statistics file does not exists Filename:%s", ctx->currPmLogFileName );
        pmhClearPmLogFileStatusRow( ctx->currPmLogFileName );
        /* Open PM Log file to collect remaining counter values */
        if(TRUE EQ pmhOpenPmLogFile(ctx))
        {
          pmhAddEntryInPmLogFileStatusTable(ctx);
        }
        logInfo(" New file is created [filename:%s] "
            " and further will be done in this file", ctx->currPmLogFileName );
      }
      writeToFileMacUplaneCounterTable(rowOfTable, ctx->fp);
    }
    return TRUE;
  }
  return TRUE;
}

/**
 * @param   msgIn Message received
 * @param   pmhContext PMH Context
 * @details This function processes MSG_MAC_PMH_GEN_STAT_RSP message receive from MAC.
 * */
PmhState pmhProcMacGenStsRsp(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  )
{
  MsgSmPmhMacGenStatRsp *rcvdRsp    = (MsgSmPmhMacGenStatRsp *)msgIn->data;
  MacGenCounters *rowOfTable = (MacGenCounters*)getFirstRowOfTable(MACGENCOUNTERS);

  /* Store received counter values in global variable */
  copyBytes( &(rcvdRsp->macGenCounters), sizeof(MacGenCounters), rowOfTable );
  rowOfTable->index = 0;

  /* Update last collected time in logStatus table */
  getAndFillDateAndTimeStamp( MAX_SIZE_MACGENCOUNTERS_LASTCOLLECTEDAT, rowOfTable->lastCollectedAt );

  /* Check if response was received due to get cmd on this table, if yes, update the table and return */
  if ( rcvdRsp->transId)
  {
    if(TRUE EQ pmhContext->resetAllCounters)
    {
      pmhContext->rcvdClearCounterRsp.macGenCounter = TRUE;
      pmhProcClearCounterRsp( pmhContext, rcvdRsp->transId );
    }
    else
    {
      sendGetObjRspSuccToBsm( rcvdRsp->transId, pmhContext );
    }
    return pmhContext->pmhState;
  }

  /* Update the runtime counter flags */
  pmhContext->rcvdCounterRsp.macGenCounter = TRUE;

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
    writeToFileMacGenCounterTable(rowOfTable, pmhContext->fp);
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
 * @details This function processes MSG_MAC_PMH_RLCSAP_STAT_RSP message receive from MAC.
 * */
PmhState pmhProcMacRlcsapStatRsp(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  )
{
  MsgSmPmhMacRlcsapStatRsp  *rcvdRsp    = (MsgSmPmhMacRlcsapStatRsp *)msgIn->data;
  MacUplaneCounters  *rowOfTable = (MacUplaneCounters*)getFirstRowOfTable(MACUPLANECOUNTERS);

  pmhContext->rspRspFromMacRclAndMacPhyCounterForGetCmd++;

  /* Store received counter values in global variable */
  rowOfTable->numOfSDUTxRlcMacIf = rcvdRsp->macRlcsapCounters.numOfSDUTxRlcMacIf;
  rowOfTable->numOfSDURxRlcMacIf = rcvdRsp->macRlcsapCounters.numOfSDURxRlcMacIf;
  rowOfTable->numSDUDropRlcMacIf = rcvdRsp->macRlcsapCounters.numSDUDropRlcMacIf;
  rowOfTable->index = 0;

  pmhContext->rspRspFromMacRclAndMacPhyCounter++;
  if ( FALSE EQ updatePmLogFileForMacUplaneCounters(rcvdRsp->transId, pmhContext) )
  {
    pmhContext->rspRspFromMacRclAndMacPhyCounter--;
    return pmhContext->pmhState;
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
 * @details This function processes MSG_MAC_PMH_PHYSAP_STAT_RSP message receive from MAC.
 * */
PmhState pmhProcMacPhysapStatRsp(
    IN Msg           *msgIn,
    IO PmhContext    *pmhContext
    )
{
  MsgSmPmhMacPhysapStatRsp  *rcvdRsp    = (MsgSmPmhMacPhysapStatRsp *)msgIn->data;
  MacUplaneCounters  *rowOfTable = getDataMacUplaneCountersTable(SINGLE_ROW_TABLE-1);

  pmhContext->rspRspFromMacRclAndMacPhyCounterForGetCmd++;

  /* Store received counter values in global variable */
  rowOfTable->numOfPduTxL2L1If = rcvdRsp->macPhysapCounters.numOfPduTxL2L1If;
  rowOfTable->numOfPduRxL2L1If = rcvdRsp->macPhysapCounters.numOfPduRxL2L1If;
  rowOfTable->numPduDropL2L1If = rcvdRsp->macPhysapCounters.numPduDropL2L1If;
  rowOfTable->index = 0;

  pmhContext->rspRspFromMacRclAndMacPhyCounter++;
  if ( FALSE EQ updatePmLogFileForMacUplaneCounters(rcvdRsp->transId, pmhContext) )
  {
    pmhContext->rspRspFromMacRclAndMacPhyCounter--;
    return pmhContext->pmhState;
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
 * @details This function creates and sends MSG_PMH_SM_MAC_GEN_STAT_REQ to MAC.
 * */
void createAndSendMacGenStatReq(
  IN CmdRef       *cmdRef,
  IN TagInfo      *tagInfo,
  IO PmhContext   *pmhContext
  )
{
  UINT32                 transId = 0 ;
  BsErrorValue           error;
  MsgPmhSmMacGenStatReq  req;
  clearBytes(&req, sizeof(MsgPmhSmMacGenStatReq));

  /* Allocate transId and store CmdRef and TagInfo*/
  error = pmhAllocTransId(cmdRef, tagInfo, &transId, pmhContext);
  if(BS_NO_ERROR NOTEQ error)
  {
    sendGetObjRspFailToBsm(cmdRef, tagInfo, &error);
    return;
  }
  req.transId          = transId;
  req.resetAllCounters = FALSE;
  createAndSendMsg( MSG_PMH_SM_MAC_GEN_STAT_REQ, (UINT8 *)&req, sizeof(MsgPmhSmMacGenStatReq) );
}

/**
 * @param   cmdRef  Cmd Ref
 * @param   tagInfo tag information
 * @param   pmhContext PMH Context
 * @details This function creates and sends MSG_PMH_SM_MAC_RLCSAP_STAT_REQ and MSG_PMH_SM_MAC_PHYSAP_STAT_REQ to MAC.
 * */
void createAndSendMacUplaneStatReq(
  IN CmdRef      *cmdRef,
  IN TagInfo     *tagInfo,
  IO PmhContext  *pmhContext
  )
{
  UINT32                 transId = 0 ;
  BsErrorValue           error;

  MsgPmhSmMacRlcsapStatReq  rlcSapReq;
  MsgPmhSmMacPhysapStatReq  phySapReq;

  clearBytes(&rlcSapReq, sizeof(MsgPmhSmMacRlcsapStatReq));
  clearBytes(&phySapReq, sizeof(MsgPmhSmMacPhysapStatReq));

  /* Allocate transId and store CmdRef and TagInfo*/
  error = pmhAllocTransId(cmdRef, tagInfo, &transId, pmhContext);
  if(BS_NO_ERROR NOTEQ error)
  {
    sendGetObjRspFailToBsm(cmdRef, tagInfo, &error);
    return;
  }
  rlcSapReq.transId          = transId;
  rlcSapReq.resetAllCounters = FALSE;
  phySapReq.transId          = transId;
  phySapReq.resetAllCounters = FALSE;
  createAndSendMsg( MSG_PMH_SM_MAC_RLCSAP_STAT_REQ, (UINT8 *)&rlcSapReq, sizeof(MsgPmhSmMacRlcsapStatReq) );
  createAndSendMsg( MSG_PMH_SM_MAC_PHYSAP_STAT_REQ, (UINT8 *)&phySapReq, sizeof(MsgPmhSmMacPhysapStatReq) );

}
