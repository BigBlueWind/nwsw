/**
 * @file    print_cmn.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file is the contains ptrint functions which are common across
 *          all modules.
 *
 * @author     : Shweta Polepally
 * Date       : 10 Dec 2010
 * Description: Basic version
 *
 */


#include "print_cmn.h"

void printCmdRef(
  IN CmdRef *ref
  )
{
  logMsgParams(" origin:%d\n cmdRefNum:%d\n instanceId:%d\n",ref->origin, ref->cmdRefNum,ref->instanceId );
}

void printMsgSetMsgTrace(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgTrace *msg =  (MsgTrace*)data;
  logMsgParams("%s","{\n");
  logMsgParams( "{\n MsgId=%d\n msgName=%s\n msgTraceLevel(0-TRACE_DISABLE,1-TRACE_MSG_ID,2-TRACE_MSG_WITH_PARAMS)=%d\n msgTraceDirection(0-TRACE_MSG_COMING_IN,1-TRACE_MSG_GOING_OUT,2-TRACE_MSG_IN_BOTH_DIRECTIONS,)=%d\n }\n",\
                        msg->msgId, getMsgName(msg->msgId), msg->msgTraceLevel, msg->msgTraceDirection  );
  logMsgParams("} Total length = %d\n", len );
}

char *getModExitCauseName(
  IN ModExitCause modExitCause
  )
{
  switch(modExitCause)
  {
    case MODEXITCAUSE_RESTART:                  return "MODEXITCAUSE_RESTART";
    case MODEXITCAUSE_REBOOT:                   return "MODEXITCAUSE_REBOOT";
    case MODEXITCAUSE_SWACT:                    return "MODEXITCAUSE_SWACT";
    case MODEXITCAUSE_L2L3_PATCH_INSTALL:            return "MODEXITCAUSE_PATCH_INSTALL";
    case MODEXITCAUSE_L2L3_PATCH_INSTALL_FAILED:     return "MODEXITCAUSE_PATCH_INSTALL_FAILED";
    default: return "Module Exit reason not defined. Check print function";
  }
}

void printMsgExitCmd(
  IO  void    *data,
  IO  UINT16  len
    )
{
  MsgPrmModExitCmd *msg =  (MsgPrmModExitCmd*)data;
  logMsgParams("%s","{\n");
  logMsgParams( " modExitCause=%s\n", getModExitCauseName(msg->modExitCause) );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgOpStartFail(
  IO  void    *data,
  IO  UINT16  len
  )
{
  OpStartFail *msg = (OpStartFail*)data;
  logMsgParams( "{\n errorNum=%s\n failReason=%s\n", errorCodeStr(msg->errorNum), msg->failReason ) ;
  logMsgParams("} Total length = %d\n", len );
}

void printMsgEnbConfigSucc(
  IO  void    *data,
  IO  UINT16  len
  )
{
  EnbConfigSucc *msg = (EnbConfigSucc*)data;
  logMsgParams( "{\n transId=%d\n", msg->transId ) ;
  logMsgParams("} Total length = %d\n", len );
}

void printMsgEnbConfigFail(
  IO  void    *data,
  IO  UINT16  len
  )
{
  EnbConfigFail *msg = (EnbConfigFail*)data;
  logMsgParams( "{\n transId=%d\n errorNum=%s\n failReason=%s\n", msg->transId, errorCodeStr(msg->errorDesc.errorNum), msg->errorDesc.failReason ) ;
  logMsgParams("} Total length = %d\n", len );
}

char *getEnbDisableReasonName(
  IN EnbDisableReason enbDisableReason
  )
{
  switch( enbDisableReason )
  {
    case USER_INITAITED_LOCK:       return "USER_INITAITED_LOCK";
    case MOD_DOWN_OCCURED:          return "MOD_DOWN_OCCURED";
    case S1AP_LINK_DOWN:            return "S1AP_LINK_DOWN";
    case DEVICE_DOWN:               return "DEVICE_DOWN";
    case UEH_OPSTART_FAIL:          return "UEH_OPSTART_FAIL";
    case S1AP_OPSTART_FAIL:         return "S1AP_OPSTART_FAIL";
    case PMH_OPSTART_FAIL:          return "PMH_OPSTART_FAIL";
    case DEH_OPSTART_FAIL:          return "DEH_OPSTART_FAIL";
    case CRITICAL_ALARM_GENERATED:  return "CRITICAL_ALARM_GENERATED";
    default: return "Disable reason not defined. Check print func";
  }
}

void printMsgOpStopReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmModOpStopReq *msg = (MsgBsmModOpStopReq*)data;
  logMsgParams( "{\n enbDisableReason=%s\n", getEnbDisableReasonName(msg->enbDisableReason) ) ;
  logMsgParams("} Total length = %d\n", len );
}

void printMsgTraceCntrlReq(
  void    *data,
  UINT16  len
  )
{
  MsgLthSmTraceCntrlReq *msg = (MsgLthSmTraceCntrlReq*)data;
  logMsgParams("%s","{\n");
  logMsgParams( "traceCtrlCmd=%s\n", traceCommandStr(msg->traceCtrlCmd) );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgTraceCntrlCfm(
  void    *data,
  UINT16  len
  )
{
  MsgSmLthTraceCntrlCfm *msg = (MsgSmLthTraceCntrlCfm*)data;
  logMsgParams("%s","{\n");
  logMsgParams( "trcCntrlCfm=%d\n", msg->traceCtrlConfirm );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgTraceInd(
    void    *data,
    UINT16  len
    )
{
  MsgSmLthTrcInd *msg = (MsgSmLthTrcInd *)data;
  UINT8            cnt;

  logMsgParams("%s","{\n");
  logMsgParams( "traceLen=%d\ntraceMsg=", msg->traceLen );

  for(cnt = 0;cnt < msg->traceLen;cnt++)
    logMsgParams( "%#2x ", msg->traceMsg[cnt] );

  logMsgParams( "\nseconds=%d\nnano seconds=%d\n", msg->traceLen,msg->traceMsg,msg->timeVal.tv_sec,msg->timeVal.tv_nsec );
  logMsgParams("} Total length = %d\n", len );
}

void printStartIndMsg(
    void    *data,
    UINT16  len
    )
{
  MsgStartInd *msg =  (MsgStartInd*)data;
  logMsgParams("%s","{\n");
  logMsgParams( " svnVersion=%d\n", msg->svnVersion  );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgHello(
    void    *data,
    UINT16  len
    )
{
  MsgPrmModHello *msg =  (MsgPrmModHello*)data;
  logMsgParams("%s","{\n");
  logMsgParams( " helloSeqNum=%d\n", msg->helloSeqNum  );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgHelloAck(
    void    *data,
    UINT16  len
    )
{
  MsgModPrmHelloAck *msg =  (MsgModPrmHelloAck*)data;
  logMsgParams("%s","{\n");
  logMsgParams( " helloAckSeqNum=%d\n", msg->helloAckSeqNum  );
  logMsgParams("} Total length = %d\n", len );
}
void PrintMsgSetLogConfig(
  void    *data,
  UINT16  len
  )
{
  SetLogConfig *msg = (SetLogConfig *) data;

  logMsgParams( "{\nModuleId: %s\n logMask1=%d\n",getModuleName(msg->moduleId), msg->logMask1);
  logMsgParams("} Total length = %d\n", len );
}

void PrintMsgDstatsCfg(
  void *data,
  UINT16 len
 )
{
  MsgPmhTfuDStatsCfgReq *msg = (MsgPmhTfuDStatsCfgReq *)data;
  logMsgParams("%s","{\n");
  logMsgParams( " Command=%d\n Interval_ms=%d\n", msg->cmd, msg->interval_ms  );
  logMsgParams("} Total length = %d\n", len );
}
void PrintMsgDstatsInd(
  void *data,
  UINT16 len
 )
{
  MsgTfuPmhDStatsInd *msg = (MsgTfuPmhDStatsInd *)data;
  logMsgParams("%s","{\n");
  logMsgParams( " Number of Element=%d\n ", msg->n);
#if 0 /* Msg contents printed in pmh event handling */
  logMsgParams( "%s"," DCounterId        val\n");
  for(i=0; i<msg->n; i++)
  {
    logMsgParams( " %d           %d\n", msg->counterList[i].id, msg->counterList[i].val );
  }
#endif 
  logMsgParams("} Total length = %d\n", len );
}
