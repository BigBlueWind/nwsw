/**
  * @file    bsm_pmh_msg_print.c
  * @details Copyright Powerwave Technologies, Inc., 2010
  * @brief This file contains the print functions for the messages that are exchanged
  *          between BSM and PMH
  * @author :  Ramprasad Reddy
  * Date   :
  * Change History:
  *
  */

#include "bsm_pmh_msg_print.h"

void printMsgBsmPmhConfigReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  UINT32 i;
  MsgBsmPmhConfigReq *msg = (MsgBsmPmhConfigReq*)data;

  logMsgParams("{\n transId=%d\n pmProfileId=%d\n sampleInterval=%d\n pmFileCreationInterval=%d\n collectMacCounters=%d\n collectRlcCounters=%d\n", \
               msg->transId, msg->pmCounterLogConfig.pmProfileId, msg->pmCounterLogConfig.sampleInterval, msg->pmCounterLogConfig.pmFileCreationInterval, msg->pmCounterLogConfig.collectMacGenCounters, msg->pmCounterLogConfig.collectRlcGenCounters );
  logMsgParams(" collectPdcpCounters=%d\n collectGtpCounters=%d\n collectSctpCounters=%d\n collectS1APCounters=%d\n collectRRCCounters=%d\n", \
               msg->pmCounterLogConfig.collectPdcpGenCounters, msg->pmCounterLogConfig.collectGtpGenCounters, msg->pmCounterLogConfig.collectSctpGenCounters, msg->pmCounterLogConfig.collectS1apGenCounters, msg->pmCounterLogConfig.collectRrcGenCounters );
  logMsgParams(" collectIPCounters=%d\n collectEthernetCounters=%d\n", \
               msg->pmCounterLogConfig.collectIPCounters, msg->pmCounterLogConfig.collectEthernetCounters );

  /* Printing the CounterConfig table */
  logMsgParams( "%s", "CounterConfig:\n");
  logMsgParams("%s"," index \t counterId \t profileId \t isEnabled \n" );
  for (i=0;i<MAX_ROWS_IN_COUNTERCONFIG_TABLE;i++)
    logMsgParams(" %d \t %d        \t %d            \t %d \n", msg->counterConfig[i].index, msg->counterConfig[i].counterId, msg->counterConfig[i].profileId, msg->counterConfig[i].isEnabled );

  logMsgParams( "} Total length :%d \n", len );
}

void printMsgBsmPmhCntrInputConfigReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmPmhCntrInputConfigReq *msg = (MsgBsmPmhCntrInputConfigReq*)data;

  logMsgParams("{\n transId=%d\n cellId=%d\n peerId=%d\n", msg->transId, msg->cellId, msg->peerId );
  logMsgParams( "} Total length :%d \n", len );
}

void printCntrFileAvailableMsg(
    IO  void    *data,
    IO  UINT16  len
    )
{
  MsgPmhBsmCntrFileAvailable *msg = (MsgPmhBsmCntrFileAvailable*)data;
  logMsgParams( "{\n pmLogFileName=%s\n} Total length :%d\n", \
      msg->pmLogFileName, len );
}

void printCntrCollectionFailMsg(
    IO  void    *data,
    IO  UINT16  len
    )
{
  MsgPmhBsmCntrCollectFail *msg = (MsgPmhBsmCntrCollectFail*)data;
  logMsgParams( "{\n errorNum=%s\n failReason=%s\n} Total length :%d\n", \
      errorCodeStr(msg->errorDesc.errorNum), msg->errorDesc.failReason, len );
}

void printCntrFileUploadedMsg(
    IO  void    *data,
    IO  UINT16  len
    )
{
  MsgBsmPmhPmFileUploaded *msg = (MsgBsmPmhPmFileUploaded*)data;
  logMsgParams( "{\n pmLogFileName=%s\n} Total length :%d\n", \
      msg->pmLogFileName, len );
}

void printMsgCounterClearReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmPmhCounterClearReq *msg =  (MsgBsmPmhCounterClearReq*)data;
  logMsgParams( "%s", "{\n") ;
  printCmdRef(&msg->cmdRef);
  logMsgParams("} Total length = %d\n", len );
}

void printMsgCounterClearSucc(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhBsmCounterClearSucc *msg =  (MsgPmhBsmCounterClearSucc*)data;
  logMsgParams( "%s", "{\n") ;
  printCmdRef(&msg->cmdRef);
  logMsgParams("} Total length = %d\n", len );
}

void printMsgCounterClearFail(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhBsmCounterClearFail *msg =  (MsgPmhBsmCounterClearFail*)data;
  logMsgParams( "%s", "{\n") ;
  printCmdRef(&msg->cmdRef);
  logMsgParams( "Error = %s\n", errorCodeStr(msg->bsError)) ;
  logMsgParams("} Total length = %d\n", len );
}

void printMsgBsmPmhDiagCounterCmd(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmPmhDiagCounterCmd *msg =  (MsgBsmPmhDiagCounterCmd*)data;
  logMsgParams( "%s", "{\n") ;
  printCmdRef(&msg->cmdRef);
  logMsgParams( "EnbCommand %s\n", eNbCommandStr(msg->eNbCommand)) ;
  logMsgParams("} Total length = %d\n", len );
}

void printMsgPmhBsmDiagCounterCmdRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhBsmDiagCounterCmdRsp *msg =  (MsgPmhBsmDiagCounterCmdRsp*)data;
  logMsgParams( "%s", "{\n") ;
  printCmdRef(&msg->cmdRef);
  logMsgParams( "EnbCommand %s\n", eNbCommandStr(msg->eNbCommand)) ;
  logMsgParams("} Total length = %d\n", len );
}

void printMsgPmhBsmMeasurementCompleteInd(
    IO  void    *data,
    IO  UINT16  len
    )
{
  MsgPmhBsmMeasurementCompleteInd *msg = (MsgPmhBsmMeasurementCompleteInd*)data;
  logMsgParams( "{\n profileId=%d\n cause=%s\n} Total length :%d\n", \
      msg->profileId, msg->cause, len );
}
