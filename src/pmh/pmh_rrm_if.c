/**
 * @file    pmh_rrm_if.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will contains functions to process different messages received from UEH for RRM counter
 *
 * @author  Ankur Agrawal
 * @date    102-Aug-2011
 * @details This file contains functions to process different messages received from UEH for RRM counter
 */

#include "pmh.h"

/**
 * @param   msgIn Message received
 * @param   pmhContext PMH Context
 * @details This function processes MSG_UEH_PMH_RRM_STAT_RSP message receive from GTP.
 * */
PmhState pmhProcRrmStsRsp(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  )
{
  MsgUehPmhRrmStatRsp  *rcvdRsp = (MsgUehPmhRrmStatRsp *)msgIn->data;
  RrmCounters  *rowOfTable = (RrmCounters*)getFirstRowOfTable(RRMCOUNTERS);

  /* Store received counter values in global variable */
  copyBytes( &(rcvdRsp->rrmCounters), sizeof(RrmCounters), rowOfTable );
  rowOfTable->index = 0;

  /* Update last collected time in logStatus table */
  getAndFillDateAndTimeStamp( MAX_SIZE_RRMCOUNTERS_LASTCOLLECTEDAT, rowOfTable->lastCollectedAt );

  /* Check if response was received due to get cmd on this table(transId NOTEQ 0), if yes, update the table and return */
  if ( rcvdRsp->transId )
  {
    if(TRUE EQ pmhContext->resetAllCounters)
    {
      pmhContext->rcvdClearCounterRsp.rrmCounter    = TRUE;
      pmhProcClearCounterRsp( pmhContext, rcvdRsp->transId );
    }
    else
    {
      sendGetObjRspSuccToBsm( rcvdRsp->transId, pmhContext );
    }
    return pmhContext->pmhState;
  }

  /* Update the runtime counter flags */
  pmhContext->rcvdCounterRsp.rrmCounter = TRUE;

  if( NULL NOTEQ pmhContext->fp )
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
    writeToFileRrmCounterTable(rowOfTable, pmhContext->fp);
  }

  if(TRUE EQ pmhStsRspRcvdFromAllModules( &pmhContext->sendCounterReq , &pmhContext->rcvdCounterRsp ) )
  {
    doCounterCollectionProcCompletionSteps( pmhContext );
  }
  return pmhContext->pmhState;
}

/**
 * @param   cmdRef  Cmd Ref
 * @param   tagInfo tag information
 * @param   pmhContext PMH Context
 * @details This function creates and sends MSG_PMH_UEH_RRM_STAT_REQ to UEH.
 * */
void createAndSendUehRrmStatReq(
  IN CmdRef       *cmdRef,
  IN TagInfo      *tagInfo,
  IO PmhContext   *pmhContext
  )
{
  UINT32                 transId = 0 ;
  BsErrorValue           error;
  MsgPmhUehRrmStatReq  req;
  clearBytes(&req, sizeof(MsgPmhUehRrmStatReq));

  /* Allocate transId and store CmdRef and TagInfo*/
  error = pmhAllocTransId(cmdRef, tagInfo, &transId, pmhContext);
  if(BS_NO_ERROR NOTEQ error)
  {
    sendGetObjRspFailToBsm(cmdRef, tagInfo, &error);
    return;
  }
  req.transId = transId;
  req.resetAllCounters = FALSE;
  createAndSendMsg( MSG_PMH_UEH_RRM_STAT_REQ, (UINT8 *)&req, sizeof(MsgPmhUehRrmStatReq) );
}

