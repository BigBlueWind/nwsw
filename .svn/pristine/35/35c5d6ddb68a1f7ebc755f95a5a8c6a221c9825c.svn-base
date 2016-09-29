/**
 * @file    pmh_gtp_if.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will contains functions to process different messages received from GTP
 *
 * @author  Ramprasad Reddy Chinta
 * @date    19-02-2011
 * @details This file contains functions to process different messages received from GTP
 */

#include "pmh.h"

/**
 * @param   msgIn Message received
 * @param   pmhContext PMH Context
 * @details This function processes MSG_GTP_PMH_GEN_STAT_RSP message receive from GTP.
 * */
PmhState pmhProcGtpGenStsRsp(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  )
{
  MsgSmPmhGtpGenStatRsp  *rcvdRsp = (MsgSmPmhGtpGenStatRsp *)msgIn->data;
  GtpGenCounters  *rowOfTable = (GtpGenCounters*)getFirstRowOfTable(GTPGENCOUNTERS);

  /* Store received counter values in global variable */
  copyBytes( &(rcvdRsp->gtpGenCounters), sizeof(GtpGenCounters), rowOfTable );
  rowOfTable->index = 0;

  /* Update last collected time in logStatus table */
  getAndFillDateAndTimeStamp( MAX_SIZE_GTPGENCOUNTERS_LASTCOLLECTEDAT, rowOfTable->lastCollectedAt );

  /* Check if response was received due to get cmd on this table, if yes, update the table and return */
  if ( rcvdRsp->transId )
  {
    if(TRUE EQ pmhContext->resetAllCounters)
    {
      pmhContext->rcvdClearCounterRsp.gtpGenCounter    = TRUE;
      pmhProcClearCounterRsp(pmhContext, rcvdRsp->transId );
    }
    else
    {
      sendGetObjRspSuccToBsm( rcvdRsp->transId, pmhContext );
    }
    return pmhContext->pmhState;
  }

  /* Update the runtime counter flags */
  pmhContext->rcvdCounterRsp.gtpGenCounter = TRUE;

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
    writeToFileGtpGenCounterTable(rowOfTable, pmhContext->fp);
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
 * @details This function creates and sends MSG_PMH_SM_EGTP_GEN_STAT_REQ to EGTP.
 * */
void createAndSendGtpGenStatReq(
  IN CmdRef      *cmdRef,
  IN TagInfo     *tagInfo,
  IO PmhContext  *pmhContext
  )
{
  UINT32                 transId = 0 ;
  BsErrorValue           error;
  MsgPmhSmGtpGenStatReq  req;
  clearBytes(&req, sizeof(MsgPmhSmGtpGenStatReq));

  /* Allocate transId and store CmdRef and TagInfo*/
  error = pmhAllocTransId(cmdRef, tagInfo, &transId, pmhContext);
  if(BS_NO_ERROR NOTEQ error)
  {
    sendGetObjRspFailToBsm(cmdRef, tagInfo, &error);
    return;
  }
  req.transId          = transId;
  req.resetAllCounters = FALSE;
  createAndSendMsg( MSG_PMH_SM_EGTP_GEN_STAT_REQ, (UINT8 *)&req, sizeof(MsgPmhSmGtpGenStatReq) );
}

/**
 * @param   cmdRef  Cmd Ref
 * @param   tagInfo tag information
 * @param   pmhContext PMH Context
 * @details This function creates and sends MSG_PMH_UDH_DSTATS_REQ to RRC.
 * */
void createAndSendUdhDStatReq(
IN DStatsCmd cmd,
IN UINT32    interval_ms
)
{
  MsgPmhUdhDStatsCfgReq  req;
  clearBytes(&req, sizeof(MsgPmhUdhDStatsCfgReq));

  req.interval_ms = interval_ms; /* TODO: Add proper value */
  req.cmd = cmd;
  createAndSendMsg( MSG_PMH_UDH_DSTATS_REQ, (UINT8 *)&req, sizeof(MsgPmhUdhDStatsCfgReq) );
}

/**
 * @param   msgIn Message received
 * @param   pmhContext PMH Context
 * @details This function processes MSG_GTP_PMH_GEN_STAT_RSP message receive from GTP.
 * */
PmhState pmhProcUdhDStatsInd(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  )
{
  MsgUdhPmhDStatsInd *dStatsInd = (MsgUdhPmhDStatsInd *)msgIn->data;
  UINT16 index = 0;
  UINT8 *ctrName = NULL;
  UINT32 offset = 0 ;
  for(index = 0; index < dStatsInd->n;index ++)
  {
     ctrName = getIcpuCtrName(dStatsInd->counterList[index].id);
     offset += snprintf((pmhContext->logBuf + offset), sizeof(pmhContext->logBuf), "%s=%d|",ctrName,dStatsInd->counterList[index].val);
  }
    logInfo("NumCounters:%d",dStatsInd->n);
    logInfo("%s",pmhContext->logBuf);
    return pmhContext->pmhState;
}
