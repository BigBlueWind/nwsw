/**
 * @file    pmh_sctp_if.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will contains functions to process different messages received from SCTP
 *
 * @author  Ramprasad Reddy Chinta
 * @date    19-02-2011
 * @details This file contains functions to process different messages received from SCTP
 */

#include "pmh.h"

/**
 * @param   msgIn Message received
 * @param   pmhContext PMH Context
 * @brief This function processes MSG_SCTP_PMH_GEN_STAT_RSP message receive from SCTP.
 **/
PmhState pmhProcSctpGenStsRsp(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  )
{
  MsgSmPmhSctpGenStatRsp *rcvdRsp = (MsgSmPmhSctpGenStatRsp *)msgIn->data;
  SctpGenCounters *rowOfTable = (SctpGenCounters*)getFirstRowOfTable(SCTPGENCOUNTERS);

  copyBytes( &(rcvdRsp->sctpGenCounters), sizeof(SctpGenCounters), rowOfTable );
  rowOfTable->index = 0;

  /* Update last collected time in logStatus table */
  getAndFillDateAndTimeStamp( MAX_SIZE_SCTPGENCOUNTERS_LASTCOLLECTEDAT, rowOfTable->lastCollectedAt );

  /* Check if response was received due to get cmd on this table, if yes, update the table and return */
  if ( rcvdRsp->transId )
  {
    if(TRUE EQ pmhContext->resetAllCounters)
    {
      pmhContext->rcvdClearCounterRsp.sctpGenCounter    = TRUE;
      pmhProcClearCounterRsp( pmhContext, rcvdRsp->transId );
    }
    else
    {
      sendGetObjRspSuccToBsm( rcvdRsp->transId, pmhContext );
    }
    return pmhContext->pmhState;
  }

  pmhContext->rcvdCounterRsp.sctpGenCounter = TRUE;
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
    writeToFileSctpGenCounterTable(rowOfTable, pmhContext->fp);
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
 * @details This function creates and sends MSG_PMH_SM_SCTP_GEN_STAT_REQ to SCTP.
 * */
void createAndSendSctpGenStatReq(
  IN CmdRef      *cmdRef,
  IN TagInfo     *tagInfo,
  IO PmhContext  *pmhContext
  )
{
  UINT32                 transId = 0 ;
  BsErrorValue           error;
  MsgPmhSmSctpGenStatReq  req;
  clearBytes(&req, sizeof(MsgPmhSmSctpGenStatReq));

  /* Allocate transId and store CmdRef and TagInfo*/
  error = pmhAllocTransId(cmdRef, tagInfo, &transId, pmhContext);
  if(BS_NO_ERROR NOTEQ error)
  {
    sendGetObjRspFailToBsm(cmdRef, tagInfo, &error);
    return;
  }
  req.transId          = transId;
  req.resetAllCounters = FALSE;
  createAndSendMsg( MSG_PMH_SM_SCTP_GEN_STAT_REQ, (UINT8 *)&req, sizeof(MsgPmhSmSctpGenStatReq) );
}

