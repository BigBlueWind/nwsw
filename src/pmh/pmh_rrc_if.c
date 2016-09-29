/**
 * @file    pmh_rrc_if.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will contains functions to process different messages received from RRC
 *
 * @author  Ramprasad Reddy Chinta
 * @date    19-02-2011
 * @details This file contains functions to process different messages received from RRC
 */

#include "pmh.h"

/**
 * @param   msgIn Message received
 * @param   pmhContext PMH Context
 * @details This function processes MSG_RRC_PMH_GEN_STAT_RSP message receive from RRC.
 **/
PmhState pmhProcRrcGenStsRsp(
    IN Msg           *msgIn,
    IO PmhContext    *pmhContext
    )
{
  MsgSmPmhRrcGenStatRsp  *rcvdRsp = (MsgSmPmhRrcGenStatRsp *)msgIn->data;
  RrcGenCounters  *rowOfTable   = (RrcGenCounters*)getFirstRowOfTable(RRCGENCOUNTERS);

  copyBytes( &(rcvdRsp->rrcGenCounters), sizeof(RrcGenCounters), rowOfTable );
  rowOfTable->index = 0;

  /* Update last collected time in logStatus table */
  getAndFillDateAndTimeStamp( MAX_SIZE_RRCGENCOUNTERS_LASTCOLLECTEDAT, rowOfTable->lastCollectedAt );

  /* Check if response was received due to get cmd on this table, if yes, update the table and return */
  if ( rcvdRsp->transId )
  {
    if(TRUE EQ pmhContext->resetAllCounters)
    {
      pmhContext->rcvdClearCounterRsp.rrcGenCounter    = TRUE;
      pmhProcClearCounterRsp( pmhContext, rcvdRsp->transId );
    }
    else
    {
      sendGetObjRspSuccToBsm( rcvdRsp->transId, pmhContext );
    }
    return pmhContext->pmhState;
  }

  /* Update the runtime counter flags */
  pmhContext->rcvdCounterRsp.rrcGenCounter = TRUE;

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
    writeToFileRrcGenCounterTable(rowOfTable, pmhContext->fp);
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
 * @details This function processes MSG_RRC_PMH_CELL_STAT_RSP message receive from RRC.
 **/
PmhState pmhProcRrcCellStsRsp(
    IN Msg           *msgIn,
    IO PmhContext    *pmhContext
    )
{
  MsgSmPmhRrcCellStatRsp  *rcvdRsp    = (MsgSmPmhRrcCellStatRsp *)msgIn->data;
  RrcCellCounters  *rowOfTable = (RrcCellCounters*)getFirstRowOfTable(RRCCELLCOUNTERS);

  copyBytes( &(rcvdRsp->rrcCellCounters), sizeof(RrcCellCounters), rowOfTable );

  rowOfTable->index = 0;

  getAndFillDateAndTimeStamp( MAX_SIZE_RRCCELLCOUNTERS_LASTCOLLECTEDAT, rowOfTable->lastCollectedAt );

  /* Check if response was received due to get cmd on this table, if yes, update the table and return */
  if ( rcvdRsp->transId )
  {
    if(TRUE EQ pmhContext->resetAllCounters)
    {
      pmhContext->rcvdClearCounterRsp.rrcCellCounter    = TRUE;
      pmhProcClearCounterRsp( pmhContext, rcvdRsp->transId );
    }
    else
    {
      sendGetObjRspSuccToBsm( rcvdRsp->transId, pmhContext );
    }
    return pmhContext->pmhState;
  }

  /* Update the runtime counter flags */
  pmhContext->rcvdCounterRsp.rrcCellCounter = TRUE;

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
    writeToFileRrcCellCounterTable(rowOfTable, pmhContext->fp);
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
 * @details This function creates and sends MSG_PMH_SM_RRC_GEN_STAT_REQ to RRC.
 * */
void createAndSendRrcGenStatReq(
  IN CmdRef   *cmdRef,
  IN TagInfo  *tagInfo,
  IO PmhContext  *pmhContext
  )
{
  UINT32                 transId = 0 ;
  BsErrorValue           error;
  MsgPmhSmRrcGenStatReq  req;
  clearBytes(&req, sizeof(MsgPmhSmRrcGenStatReq));

  /* Allocate transId and store CmdRef and TagInfo*/
  error = pmhAllocTransId(cmdRef, tagInfo, &transId, pmhContext);
  if(BS_NO_ERROR NOTEQ error)
  {
    sendGetObjRspFailToBsm(cmdRef, tagInfo, &error);
    return;
  }
  req.transId          = transId;
  req.resetAllCounters = FALSE;
  createAndSendMsg( MSG_PMH_SM_RRC_GEN_STAT_REQ, (UINT8 *)&req, sizeof(MsgPmhSmRrcGenStatReq) );
}

/**
 * @param   cmdRef  Cmd Ref
 * @param   tagInfo tag information
 * @param   cellId  cell Id
 * @param   pmhContext PMH Context
 * @details This function creates and sends MSG_PMH_SM_RRC_CELL_STAT_REQ to RRC.
 * */
void createAndSendRrcCellStatReq(
  IN CmdRef      *cmdRef,
  IN TagInfo     *tagInfo,
  IN UINT8        cellId,
  IO PmhContext  *pmhContext
  )
{
  UINT32                 transId = 0 ;
  BsErrorValue           error;
  MsgPmhSmRrcCellStatReq  req;
  clearBytes(&req, sizeof(MsgPmhSmRrcCellStatReq));

  /* Allocate transId and store CmdRef and TagInfo*/
  error = pmhAllocTransId(cmdRef, tagInfo, &transId, pmhContext);
  if(BS_NO_ERROR NOTEQ error)
  {
    sendGetObjRspFailToBsm(cmdRef, tagInfo, &error);
    return;
  }
  req.transId = transId;
  req.cellId = cellId;
  req.resetAllCounters = FALSE;
  createAndSendMsg( MSG_PMH_SM_RRC_CELL_STAT_REQ, (UINT8 *)&req, sizeof(MsgPmhSmRrcCellStatReq) );
}
