/**
 * @file    pmh_pdcp_if.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will contains functions to process different messages received from PDCP
 *
 * @author  Ramprasad Reddy Chinta
 * @date    17-02-2011
 * @details This file contains functions to process different messages received from PDCP
 */

#include "pmh.h"

/**
 * @param   msgIn Message received
 * @param   pmhContext PMH Context
 * @details This function processes statistics confirmation from PDCP
 * */
PmhState pmhProcPdcpGenStsCfm(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  )
{
  MsgSmPmhPdcpGenStatRsp  *pdcpGenCounters = (MsgSmPmhPdcpGenStatRsp *)msgIn->data;
  PdcpGenCounters  *rowOfTable      = (PdcpGenCounters*)getFirstRowOfTable(PDCPGENCOUNTERS);

  copyBytes( &(pdcpGenCounters->pdcpGenCounters), sizeof(PdcpGenCounters), rowOfTable );

  rowOfTable->index = 0;

  /* Update last collected time in logStatus table */
  getAndFillDateAndTimeStamp( MAX_SIZE_PDCPGENCOUNTERS_LASTCOLLECTEDAT, rowOfTable->lastCollectedAt );

  /* Check if response was received due to get cmd on this table, if yes, update the table and return */
  if ( pdcpGenCounters->transId )
  {
    if(TRUE EQ pmhContext->resetAllCounters)
    {
      pmhContext->rcvdClearCounterRsp.pdcpGenCounter    = TRUE;
      pmhProcClearCounterRsp( pmhContext, pdcpGenCounters->transId );
    }
    else
    {
      sendGetObjRspSuccToBsm( pdcpGenCounters->transId, pmhContext );
    }
    return pmhContext->pmhState;
  }

  pmhContext->rcvdCounterRsp.pdcpGenCounter = TRUE;
  if( pmhContext->fp NOTEQ NULL )
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
    writeToFilePdcpGenCounterTable( rowOfTable, pmhContext->fp );
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
 * @details This function processes statistics confirmation from PDCP
 * */
PmhState pmhProcPdcpUplaneStsCfm(
    IN Msg           *msgIn,
    IO PmhContext    *pmhContext
    )
{
  MsgSmPmhPdcpUplaneStatRsp *rcvdRsp    = (MsgSmPmhPdcpUplaneStatRsp *)msgIn->data;
  PdcpUplaneCounters *rowOfTable =  (PdcpUplaneCounters*)getFirstRowOfTable(PDCPUPLANECOUNTERS);

  copyBytes( &(rcvdRsp->pdcpUplaneCounters), sizeof(PdcpUplaneCounters), rowOfTable );

  rowOfTable->index = 0;

  /* Update the Last collected time */
  getAndFillDateAndTimeStamp( MAX_SIZE_PDCPGENCOUNTERS_LASTCOLLECTEDAT, rowOfTable->lastCollectedAt );

  /* Check if response was received due to get cmd on this table, if yes, update the table and return */
  if ( rcvdRsp->transId )
  {
    if(TRUE EQ pmhContext->resetAllCounters)
    {
      pmhContext->rcvdClearCounterRsp.pdcpUplaneCounter    = TRUE;
      pmhProcClearCounterRsp( pmhContext, rcvdRsp->transId );
    }
    else
    {
      sendGetObjRspSuccToBsm( rcvdRsp->transId, pmhContext );
    }
    return pmhContext->pmhState;
  }

  pmhContext->rcvdCounterRsp.pdcpUplaneCounter = TRUE;
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
      logInfo("New file is created [filename:%s] "
          " and further will be done in this file", pmhContext->currPmLogFileName );
    }
    writeToFilePdcpUplaneCounterTable(rowOfTable, pmhContext->fp);
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
 * @details This function creates and sends MSG_PMH_SM_PDCP_GEN_STAT_REQ to PDCP.
 * */
void createAndSendPdcpGenStatReq(
  IN CmdRef       *cmdRef,
  IN TagInfo      *tagInfo,
  IO PmhContext   *pmhContext
  )
{
  UINT32                 transId = 0 ;
  BsErrorValue           error;
  MsgPmhSmPdcpGenStatReq  req;
  clearBytes(&req, sizeof(MsgPmhSmPdcpGenStatReq));

  /* CmdRef == NULL for regular counter collection else it is for GET_OBJ_REQ */
  if(NULL EQ cmdRef)
  {
    transId = 0;
  }
  else
  {
    /* Allocate transId and store CmdRef and TagInfo*/
    error = pmhAllocTransId(cmdRef, tagInfo, &transId, pmhContext);
    if(BS_NO_ERROR NOTEQ error)
    {
      sendGetObjRspFailToBsm(cmdRef, tagInfo, &error);
      return;
    }
  }
  req.transId          = transId;
  req.resetAllCounters = FALSE;
  createAndSendMsg( MSG_PMH_SM_PDCP_GEN_STAT_REQ, (UINT8 *)&req, sizeof(MsgPmhSmPdcpGenStatReq) );
}

/**
 * @param   cmdRef  Cmd Ref
 * @param   tagInfo tag information
 * @param   pmhContext PMH Context
 * @details This function creates and sends MSG_PMH_SM_PDCP_UPLANE_STAT_REQ to PDCP.
 * */
void createAndSendPdcpUplaneStatReq(
  IN CmdRef       *cmdRef,
  IN TagInfo      *tagInfo,
  IO PmhContext   *pmhContext
  )
{
  UINT32                 transId = 0 ;
  BsErrorValue           error;
  MsgPmhSmPdcpUplaneStatReq  req;
  clearBytes(&req, sizeof(MsgPmhSmPdcpUplaneStatReq));

  /* Allocate transId and store CmdRef and TagInfo*/
  error = pmhAllocTransId(cmdRef, tagInfo, &transId, pmhContext);
  if(BS_NO_ERROR NOTEQ error)
  {
    sendGetObjRspFailToBsm(cmdRef, tagInfo, &error);
    return;
  }
  req.transId          = transId;
  req.resetAllCounters = FALSE;
  createAndSendMsg( MSG_PMH_SM_PDCP_UPLANE_STAT_REQ, (UINT8 *)&req, sizeof(MsgPmhSmPdcpUplaneStatReq) );
}
