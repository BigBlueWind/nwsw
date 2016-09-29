/**
 * @file    imc.c
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @brief   This file contains the functions definitions for Inter Module Communication
 *
 * @author  Manu Sharma
 * @author  Lakshmi Narayan Madala
 * @author  Shweta Polepally
 *
 * @date    09 Sep 2010   Shweta - Renamed MAX_MSG_ID to MSG_ID_MAX
 */

#include "log.h"
#include "imc_udp.h"
#include "imc.h"
#include "bs_err.h"
#include "imc_msg_desc.h"
#include "msg_names.h"
#include "cmn_types.h"
#include "imc_unix.h"

ImcContext g_ImcContext;

/*Message Counters changes started */
TBOOL msgCntrsRunning ;
typedef struct s_IdName {
  UINT32 id;
  const String50 name;
} IdName;
const IdName c_errCauseNameTab[] =
{
{ERR_NUM_OF_BYTES_MISMATCH, "ERR_NUM_OF_BYTES_MISMATCH"},
{ERR_INVLD_MSG_DESC_RCVD, "ERR_INVLD_MSG_DESC_RCVD"},
{ERR_MISCELLANEOUS, "ERR_MISCELLANEOUS"}
};

inline const UINT8* getErrCauseName(IN  MsgRcvErrCause errCauseId )
{
  if(!(isErrorCauseIdValid(errCauseId)))
  {
    return "INVALID CAUSE ID (VALID RANGE: 0 to 2)";
  }
  else
  {
    return &c_errCauseNameTab[errCauseId].name[0];
  }
}
/* Message Counters Changes Ended */

/** Not Called by any Function */
void initModAddrInImcContext(
    ModuleId  modId,
    UINT16    udpPort,
    UINT8     *ipv4
    )
{
  ImcContext*  imcContext = NULL;
  ModAddr modAddr;

  imcContext = getGlobalImcContext();
  modAddr.udpPort    = udpPort;
  modAddr.unixSendFd = 0;
  modAddr.udpSendFd  = 0;
  modAddr.recvFd     = 0;
  strncpy(&(modAddr.ipv4[0]), ipv4, LEN_IPV4_ADDR);

  imcContext->modAddrTab[modId] = modAddr;
}
/**
 * @param modId  The module identifier
 * @details
 * This function returns the module address of a module,
 * This is used while sending a message to a module,
 * Get Module address
 * Send the message
 */
ModAddr *getModAddr(
  IN ModuleId modId
  )
{
  ImcContext *imcContext;

  if( modId > MODID_MAX )
  {
    BS_ERROR_UNKNOWN_MOD_ID(modId);
    return NULL;
  }
  imcContext = getGlobalImcContext();
  return ( &(imcContext->modAddrTab[modId]) );
}

/**
 * @param   msgId  The message Identifier
 * @details This function returns the number of used instances of a message
 */
inline UINT16 getMsgInst(
  IN MsgId msgId
)
{
  return ((getGlobalImcContext())->numMsgInst[msgId]);
}

/**
 * @param msgId  The message Identifier
 * @details
 * This function increments the number of active message instances of msgId by one.
 * This is called when a message is received at recv buffer
 */
inline void incMsgInst(
  IN MsgId msgId
  )
{
  (getGlobalImcContext())->numMsgInst[msgId]++;
}

/**
 * @param   msgId  The message Identifier
 * @details This function decrements the number of active message instances of msgId by one.
 *          This is called from freeMsgInst.
 */
void decMsgInst(
  IN MsgId msgId
  )
{
  UINT16 inst;
  if(!isMsgIdValid(msgId))
  {
    BS_ERROR_UNKNOWN_MSG(msgId);
    return;
  }
  inst = getMsgInst(msgId);
  if(inst <= 0)
  {
    BS_ERROR_NO_MSG_ALLOCATED(inst, msgId );
    return;
  }
  (getGlobalImcContext())->numMsgInst[msgId]--;
}

/**
 * @param   msgDesc  The structure that holds the message description
 * @details This function checks whether the number of messages used at a given time
 * is within the allowed levels, and logs error if the limit is exceeded.
 * This functions helps us in finding memory leaks when a received message is
 * not freed
 */
void checkMsgInstThreshold(
  IN MsgId msgId
 )
{
  IN MsgDesc  *msgDesc = getMsgDesc(msgId);

  if((getMsgInst(msgId)) >= msgDesc->maxInstances)
  {
    BS_ERROR_MSG_INST_THRESH_EXCEEDED(msgDesc->maxInstances, getMsgInst(msgId), msgId );
    if( msgDesc->maxInstances EQ 0 )
    {
      BS_ERROR_MAX_MSG_INST_ZERO();
    }
  }
}

/**
 * @param   msgId  The message to be created
 * @param   data The data to be sent in the message
 * @param   dataLen The length of message
 * @details Creates an instance of msgId.
 * if data != NULL, dataLen bytes from data are copied into the msg.
 * Otherwise, data field of the msg is zero filled.
 * Warns if the instance count exceeds maxInstances.
 */
Msg *createMsgInst(
  IN MsgId  msgId,
  IN UINT8  *data,
  IN UINT16 dataLen
)
{
  UINT16         msgLen=0;
  UINT16         payloadLen=0;
  Msg           *msg=NULL;
  const MsgDesc *msgDesc=NULL;

  msgDesc = getMsgDesc(msgId);
  if(!msgDesc) return NULL;

  checkMsgInstThreshold(msgId);

  if(dataLen NOTEQ 0)
  {
    /* Create a message of the length specified by user.
     * Users should use this method if length is variable
     * and thus cannot be determined at compile time.
     */
    payloadLen = dataLen;
  }
  else
  {
    /* create a message whose length is set at compile time */
    payloadLen = msgDesc->len;
  }

  msgLen = sizeof(Msg) + payloadLen;

  msg = (Msg*) malloc(msgLen);
  if(!msg)
  {
    BS_ERROR_MSG_CREATE_FAILED(msgId );
    return NULL;
  }
  incMsgInst(msgId);

  msg->msgId = msgId;
  msg->src   = msgDesc->src;
  msg->dest  = msgDesc->dest;
  msg->len   = payloadLen;
#ifdef VIA_IMCS
  msg->via   = MODID_IMCS;   /* Always Fixed */
#else
  msg->via   = 0;   /* Always Fixed */
#endif
  if(data)
  {
    memcpy(msg->data, data, payloadLen);
  }
  else
  {
    memset(msg->data, 0, payloadLen);
  }
  return msg;
}

void freeMsgIn(
    FR Msg       *msgIn
    )
{
  freeMsg(msgIn);
}

/**
* @param   msg  The message to be freed
* @details This function decrements the message counter and frees the message
*/
void freeMsgInst(
  IO Msg* msg
  )
{
  if(!msg) return;
  decMsgInst(msg->msgId);
  free(msg);
}

/**
 * @param   modId  The identifier of module in framework
 * @details This function creates the send and recv file descriptors,
 *          for recv file descriptor the port is fixed
 */
TBOOL registerForImc(
  IN ModuleId modId
)
{
  ModAddr    *modAddr    = getModAddr(modId);

  if(modAddr->imcMethod EQ UDP_SOCK)
  {
    int sendUdpPort = modAddr->udpPort + 1;
    /* start listening to receive port */
    logInfo("Listen UDP Port=%d, fd=%d", modAddr->udpPort, modAddr->recvFd );
    openUdpSocket(modAddr->udpPort, TRUE,  &(modAddr->recvFd) );
    logInfo("Send UDP Port=%d, fd=%d", modAddr->udpPort, modAddr->udpSendFd );
    return openUdpSocket(sendUdpPort, TRUE, &(modAddr->udpSendFd) );
  }
  else if(modAddr->imcMethod EQ UNIX_SOCK)
  {
    /* start listening to receive file */
    logInfo("Opening UNIX Domain Socket on Port = %s", modAddr->unixSockFilePath );
    openUnixSocket(&modAddr->unixSockFilePath[0], FALSE, &(modAddr->unixSendFd) );
    return openUnixSocket(&modAddr->unixSockFilePath[0], TRUE,  &(modAddr->recvFd) );
  }
  else
  {
    logError("Invalid IMC Method = %d", modAddr->imcMethod);
    modExit(modId, &(getModuleContext()->exeName[0]));
    exit(EXIT_SUCCESS);
  }

}

/**
 * @param   modId  The identifier of module in framework
 * @details This is the last function in framework to be executed
 * This functions closes the send and recv file descriptors
 */
void deregisterForImc(
  IN ModuleId modId
  )
{
  ModAddr *modAddr = getModAddr(modId);
  UINT16  ec       = 0;
  Msg     *msg     = NULL;

  /* Clear the recv queue */
  do
  {
    msg = recvMsg(modId, &ec);
    freeMsg(msg);
  } while(msg NOTEQ NULL);

  /* Close UDP Sockets */
  close(modAddr->udpSendFd);
  modAddr->udpSendFd = 0;

  /* Close Unix Sockets */
  close(modAddr->unixSendFd);
  modAddr->unixSendFd = 0;

  unlink(modAddr->unixSockFilePath);

  close(modAddr->recvFd);
  modAddr->recvFd = 0;
}


/**
 * @param   msg  The message to be sent
 * @details Send a message that was created previously,
 * Free the message after sending it over UDP
 */
void sendMsgInst(
    CONS Msg *msg
    )
{
  LogContext  *logContext = getLogContext();

  ModAddr *destModAddr = NULL;
  ModAddr *selfModAddr = NULL;
  const MsgDesc *msgDesc     = NULL;
#ifdef VIA_IMCS
  ModAddr *viaModAddr  = NULL;
#endif
  destModAddr = getModAddr(msg->dest);
  selfModAddr = getModAddr(msg->src);
#ifdef VIA_IMCS
  viaModAddr  = getModAddr(msg->via);
#endif
  SINT32 numBytesSent;
  SndMsgCountersCtx *sndMsgCountersCtx;
  
  if(TRUE EQ msgCntrsRunning)
  {
    sndMsgCountersCtx = getSendMsgCntrContext();
  }  

  if(!destModAddr)
  {
    BS_ERROR_SEND_MSG_INST_FAILED(msg->dest, msg->msgId);
    if(TRUE EQ msgCntrsRunning)
    {
      /* Updating the Send message error counter for the message ID */
      sndMsgCountersCtx->msgSentErrCntr[msg->msgId]++;    
    }
    freeMsg(msg);
    return;
  }

#ifdef VIA_IMCS
  numBytesSent = sendUdpMsg(selfModAddr->sendFd,
   viaModAddr->ipv4,
   viaModAddr->traceCollectorUdpPort,
   (UINT8*) msg,
   sizeof(Msg)+msg->len
  );
#else

     if(destModAddr->imcMethod EQ UDP_SOCK)
     {
        numBytesSent = sendUdpMsg(selfModAddr->udpSendFd,
                                  destModAddr->ipv4,
                                  destModAddr->udpPort,
                                  (UINT8*) msg,
                                  sizeof(Msg)+msg->len
                                  );
     }
     else if(destModAddr->imcMethod EQ UNIX_SOCK)
     {
        numBytesSent = sendUnixSockMsg(selfModAddr->unixSendFd,
                                       &destModAddr->unixSockFilePath[0],
                                       (UINT8*) msg,
                                       sizeof(Msg)+msg->len
                                       );
     }

#endif

/* Msg Counters changes started */
  if((numBytesSent EQ 0) || (numBytesSent < (SINT32)(sizeof(Msg)+msg->len)))
  {
    if(TRUE EQ msgCntrsRunning)
    {
      /* Updating the Send message error counter for the message ID */
      sndMsgCountersCtx->msgSentErrCntr[msg->msgId]++;
    }
  }
  /* Updating the Max Size of message seen till now */
  if( getTotalMsgSize(msg) > getMaxMsgSize() )
  {
	logProfile("Max Message Size=%d (prev=%d) bytes", getTotalMsgSize(msg), getMaxMsgSize());
	setMaxMsgSize(getTotalMsgSize(msg));
  }
/* Message Counter Changes Ended*/

  msgDesc = getMsgDesc( msg->msgId);
  logMsgOut( "[MsgId=%d, Name=%s Size=%d]", msg->msgId, getMsgName(msg->msgId), numBytesSent);
  if(logContext->logConfig.logMask1 & LOG_MASK_MSGOUT_02)
  {
    if( NULL NOTEQ msgDesc->printFunc)
    {
      msgDesc->printFunc( (void*)msg->data, msg->len );
    }
  }

#if 0 /* commented code as currently we are not caring about message trace table. */

  /* Code for Message Tracing */
  msgDesc = getMsgDesc( msg->msgId);
  if( (MSGTRACEDIRECTION_MSGOUT EQ msgDesc->msgTraceDirection ) ||
      (MSGTRACEDIRECTION_MSGINBOTHDIREC EQ msgDesc->msgTraceDirection ) )
  {
    if( MSGTRACELEVEL_MSGIDONLY EQ msgDesc->msgTraceLevel  )
    {
      logMsg( "[MSG OUT:Id=%d, Name=%s Size=%d]", msg->msgId, getMsgName(msg->msgId), numBytesSent);
    }
    else if( MSGTRACELEVEL_MSGWITHPARAMS EQ msgDesc->msgTraceLevel )
    {
      if( NULL NOTEQ msgDesc->printFunc )
      {
        logMsg( "[MSG OUT:Id=%d, Name=%s Size=%d]", msg->msgId, getMsgName(msg->msgId), numBytesSent);
        msgDesc->printFunc( (void*)msg->data, msg->len );
      }
      else
      {
        logMsg( "%s", "No traceFunction Defined, Printing only Msgname");
        logMsg( "[MSG OUT:Id=%d, Name=%s Size=%d]", msg->msgId, getMsgName(msg->msgId), numBytesSent);
      }
    }
  }
#endif

  freeMsg(msg);
}

/**
 * @param msgId  The message to be created
 * @param data The data to be sent in the message
 * @param len The length of message.
 * @details Creates an instance of msgId
 *          Copies len bytes from data[] into the msg
 *          Sends it (to destination determined from msgDec)
 */
TBOOL createAndSendMsg(
  IN MsgId     msgId,
  IN UINT8     *data,
  IN UINT16    len
)
{
  Msg *msg;
  SndMsgCountersCtx *sndMsgCountersCtx ;

  if(TRUE EQ msgCntrsRunning)
  {
    sndMsgCountersCtx = getSendMsgCntrContext();
  }
  
  msg = createMsgInst(msgId, data, len);
  
  if(msg) {
    sendMsg(msg);
    if(TRUE EQ msgCntrsRunning)
    {
      /* Updating the Send message counter for the message ID */
      sndMsgCountersCtx->msgSentCntr[msgId]++;
    }
    return TRUE;
  }
  if(TRUE EQ msgCntrsRunning)
  {
    /* Updating the Send message error counter for the message ID */
    sndMsgCountersCtx->msgSentErrCntr[msgId]++;    
  } 
  return FALSE;
}

/**
 * @param modId The module is identified by this identifier in framework
 * @param ec    The error code returned by OS
 * @details    This function checks for reception of message at module identified by modId.
 *             if no message is waiting in the queue then NULL is returned.
 */
Msg *recvMsg(
  IN ModuleId modId,
  IO UINT16   *ec
  )
{
  ModAddr *modAddr        = getModAddr(modId);
  LogContext  *logContext = getLogContext();
  RcvMsgCountersCtx  *rcvMsgCountersCtx;
  const MsgDesc      *msgDesc            = NULL;
  SINT32              numBytesRcvd;
  static UINT8        buf[SIZE_RECV_DATA]; /* keep it on heap */
  UINT16              lenOfMsg;
  Msg                *msg;

  if(TRUE EQ msgCntrsRunning)
  {
    rcvMsgCountersCtx = getRcvMsgCntrContext();
  }

  numBytesRcvd = recv(modAddr->recvFd, &buf[0], SIZE_RECV_DATA, SOCK_FLAGS);

  if( numBytesRcvd EQ -1 )
  {
    return NULL;
  }

  lenOfMsg = getMsgLen(buf);
  if(lenOfMsg NOTEQ numBytesRcvd)
  {
    BS_ERROR_DATA_RECD_NOT_SAME( numBytesRcvd, lenOfMsg )
    *ec = BS_ERR_LENGTH_MISMATCH_FOR_RCVD_MSG;
    if(TRUE EQ msgCntrsRunning)
    {
      /* Updating the Receive message error counter for the error cause */
      rcvMsgCountersCtx->msgRcvdErrCntr[ERR_NUM_OF_BYTES_MISMATCH]++;    
    }
    return NULL;
  }
  msg = (Msg*) malloc(lenOfMsg);
  if(!msg)
  {
    BS_ERROR_INTERNAL_ERROR_OCCURED( "malloc failed" );
    *ec = BS_ERR_INTERNAL_ERROR_OCCURED;
    return NULL;
  }

  memcpy(msg, &buf[0], lenOfMsg);

  /* Code for tracing the message */
  msgDesc = getMsgDesc( msg->msgId);
  if( NULL EQ msgDesc )
  {
    BS_ERROR_UNKNOWN_MSG_RCVD( modId, msg->msgId );
    if(TRUE EQ msgCntrsRunning)
    {
      /* Updating the Receive message error counter for the error cause */
      rcvMsgCountersCtx->msgRcvdErrCntr[ERR_INVLD_MSG_DESC_RCVD]++; 
    }
    return NULL;
  }

  logMsgIn( "[MsgId=%d, Name=%s Size=%d]", msg->msgId, getMsgName(msg->msgId), numBytesRcvd);
  if(logContext->logConfig.logMask1 & LOG_MASK_MSGIN_02)
  {
    if( NULL NOTEQ msgDesc->printFunc)
    {
      msgDesc->printFunc( (void*)msg->data, msg->len );
    }
  }

#if 0 /* commented code as currently we are not caring about message trace table. */
  if( (MSGTRACEDIRECTION_MSGIN EQ msgDesc->msgTraceDirection ) ||
      (MSGTRACEDIRECTION_MSGINBOTHDIREC EQ msgDesc->msgTraceDirection ) )
  {
    if( MSGTRACELEVEL_MSGIDONLY EQ msgDesc->msgTraceLevel  )
    {
      logMsg( "[MSG IN:Id=%d, Name=%s Size=%d]", msg->msgId, getMsgName(msg->msgId), numBytesRcvd);
    }
    else if( (MSGTRACELEVEL_MSGWITHPARAMS EQ msgDesc->msgTraceLevel))
    {
      if( NULL NOTEQ msgDesc->printFunc)
      {
        logMsg( "[MSG IN:Id=%d, Name=%s Size=%d]", msg->msgId, getMsgName(msg->msgId), numBytesRcvd);
        msgDesc->printFunc( (void*)msg->data, msg->len );
      }
      else
      {
        logMsg( "%s", "No traceFunction Defined, Printing only Msgname");
        logMsg( "[MSG IN:Id=%d, Name=%s Size=%d]", msg->msgId, getMsgName(msg->msgId), numBytesRcvd);
      }
    }
  }
#endif

  incMsgInst(msg->msgId);
  if(TRUE EQ msgCntrsRunning)
  {
    /* Updating the Receive message sucessful counter for the message Id */
    rcvMsgCountersCtx->msgRcvdCntr[msg->msgId]++; 
  }
  return msg;
}

/* Msg Counters changes started */

/**
 * @details    This function starts handling of message counters.
 */
TBOOL startMsgCntrs(void)
{
  /* Make the Counters state to Running */
  msgCntrsRunning = TRUE;
  return TRUE;
}

/**
 * @details    This function stops handling of message counters.
 */
TBOOL stopMsgCntrs(void)
{
  /* Make the Counters state to Not running */
  msgCntrsRunning = FALSE;
  return TRUE;
}

/**
 * @details    This function logs all the send message counters.
 */
TBOOL logSendMsgCntrs(void)
{
  int loopIndex ;
  SndMsgCountersCtx *sndMsgCntrsCtx = getSendMsgCntrContext();

  logInfo("%s", "=================================================================");
  logInfo("%s", "IMC Counters: Messages Sent successfully: (msgId) msgName: count)"); 
  for( loopIndex =MSG_UNDEFINED+1; loopIndex < MSG_ID_MAX;loopIndex++ )
    if(sndMsgCntrsCtx->msgSentCntr[loopIndex] NOTEQ 0) /*To avoid redundancy in printing */
      logInfo("(%d) %s: %d times ", loopIndex, getMsgName(loopIndex), sndMsgCntrsCtx->msgSentCntr[loopIndex]); 
  logInfo("%s", "=================================================================");
  return TRUE;
}

/**
 * @details    This function logs all the send message error counters.
 */
TBOOL logSendMsgErrCntrs(void)
{
  int loopIndex ;
  SndMsgCountersCtx *sndMsgCntrsCtx = getSendMsgCntrContext();

  logInfo("%s", "=================================================================");
  logInfo("%s", "IMC Counters: Messages Sending Failed: (msgId) msgName: count)"); 
  for(loopIndex = MSG_UNDEFINED+1;loopIndex < MSG_ID_MAX;loopIndex++ )
    if(sndMsgCntrsCtx->msgSentErrCntr[loopIndex] NOTEQ 0) /*To avoid redundancy in printing */
      logInfo("(%d) %s: %d ", loopIndex, getMsgName(loopIndex), sndMsgCntrsCtx->msgSentErrCntr[loopIndex]); 
  logInfo("%s", "=================================================================");
  return TRUE;
}

/**
 * @details    This function logs all the received message counters.
 */
TBOOL logRcvdMsgCntrs(void)

{
  int loopIndex ;
  RcvMsgCountersCtx  *rcvMsgCountersCtx = getRcvMsgCntrContext();

  logInfo("%s", "=================================================================");
  logInfo("%s", "IMC Counters: Messages received successfully: (msgId) msgName: count)"); 
  for(loopIndex =MSG_UNDEFINED+1;loopIndex < MSG_ID_MAX;loopIndex++ )
    if(rcvMsgCountersCtx->msgRcvdCntr[loopIndex] NOTEQ 0) /*To avoid redundancy in printing */
      logInfo("(%d) %s:%d",loopIndex, getMsgName(loopIndex), rcvMsgCountersCtx->msgRcvdCntr[loopIndex]); 
  logInfo("%s", "=================================================================");
  return TRUE;
}

/**
 * @details    This function logs all the received message error counters.
 */
TBOOL logRcvdMsgErrCntrs(void)
{
  int loopIndex ;
  RcvMsgCountersCtx  *rcvMsgCountersCtx = getRcvMsgCntrContext();

  logInfo("%s", "=================================================================");
  logInfo("%s", "IMC Counters: Messages received in error ((ErrorNumber) ErrorName: numFailures"); 
  for(loopIndex =0;loopIndex < MAX_ERROR_CAUSES;loopIndex++ )
    if(rcvMsgCountersCtx->msgRcvdErrCntr[loopIndex] NOTEQ 0) /*To avoid redundancy in printing */
      logInfo("(%d) %s : %d", loopIndex, getErrCauseName(loopIndex), rcvMsgCountersCtx->msgRcvdErrCntr[loopIndex]); 
  logInfo("%s", "=================================================================");
  return TRUE;
}

/**
 * @details    This function logs message counters for given MsgID.
 */
TBOOL logMsgCntrForMsgId(MsgId msgId
)
{
  RcvMsgCountersCtx  *rcvMsgCountersCtx = getRcvMsgCntrContext();
  SndMsgCountersCtx *sndMsgCntrsCtx = getSendMsgCntrContext();

  if(isMsgIdValid(msgId))
  {
    logInfo("Message %s(MsgId:%d) Sent OK %d times, Send failed %d times, Recd OK %d times", getMsgName(msgId), 
      msgId, sndMsgCntrsCtx->msgSentCntr[msgId], sndMsgCntrsCtx->msgSentErrCntr[msgId], 
      rcvMsgCountersCtx->msgRcvdCntr[msgId]);
    return TRUE;
  }
  else
  {
    logError("Invalid Message ID %d being received ",msgId);
    return FALSE;
  }
}

/**
 * @details    This function logs received message error counters for given error cause.
 */
TBOOL logRcvdMsgErrCntrForErrCause(
MsgRcvErrCause  errorCauseId
)
{
  RcvMsgCountersCtx  *rcvMsgCountersCtx = getRcvMsgCntrContext();

  if(isErrorCauseIdValid(errorCauseId))
  {
    logInfo("Message Reception failed: %d times for ErrorCause: %s(Error Cause Id: %d)", rcvMsgCountersCtx->msgRcvdErrCntr[errorCauseId], getErrCauseName(errorCauseId), errorCauseId);
    return TRUE;
  }
  else
  {
    logError("Invalid Error Cause ID %d being received ",errorCauseId);
    return FALSE;
  }
}

/**
 * @details    This function Resets all the send message counters.
 */
TBOOL resetSendMsgCntrs(void)
{
  SndMsgCountersCtx *sndMsgCountersCtx = getSendMsgCntrContext();
  clearBytes(sndMsgCountersCtx->msgSentCntr, sizeof(sndMsgCountersCtx->msgSentCntr));
  return TRUE;
}

/**
 * @details    This function resets all the send message error counters.
 */
TBOOL resetSendMsgErrCntrs(void)
{
  SndMsgCountersCtx *sndMsgCountersCtx = getSendMsgCntrContext();
  clearBytes(sndMsgCountersCtx->msgSentErrCntr, sizeof(sndMsgCountersCtx->msgSentErrCntr));
  return TRUE;
}

/**
 * @details    This function resets all the received message counters.
 */
TBOOL resetRcvdMsgCntrs(void)
{
  RcvMsgCountersCtx *rcvMsgCountersCtx = getRcvMsgCntrContext() ;
  clearBytes(rcvMsgCountersCtx->msgRcvdCntr, sizeof(rcvMsgCountersCtx->msgRcvdCntr));
  return TRUE;
}

/**
 * @details    This function resets all the received message error counters.
 */
TBOOL resetRcvdMsgErrCntrs(void)
{
  RcvMsgCountersCtx *rcvMsgCountersCtx = getRcvMsgCntrContext() ;
  clearBytes(rcvMsgCountersCtx->msgRcvdErrCntr, sizeof(rcvMsgCountersCtx->msgRcvdErrCntr));
  return TRUE;
}

/**
 * @details    This function resets message counters for given MsgID..
 */
TBOOL resetMsgCntrForMsgId(
MsgId msgId
)
{
  RcvMsgCountersCtx  *rcvMsgCountersCtx = getRcvMsgCntrContext();
  SndMsgCountersCtx *sndMsgCntrsCtx = getSendMsgCntrContext();

  if(isMsgIdValid(msgId))
  {
    sndMsgCntrsCtx->msgSentCntr[msgId] = 0;
    sndMsgCntrsCtx->msgSentErrCntr[msgId] = 0;
    rcvMsgCountersCtx->msgRcvdCntr[msgId] = 0;
    return TRUE;
  }
  else
  {
    logError("Invalid Message ID %d being received ",msgId);
    return FALSE;
  }
}

/**
 * @details    This function resets received message error counters for given error cause.
 */
TBOOL resetRcvdMsgErrCntrForErrCause(
MsgRcvErrCause errorCauseId
) 
{
  RcvMsgCountersCtx  *rcvMsgCountersCtx = getRcvMsgCntrContext();

  if(isErrorCauseIdValid(errorCauseId))
  {
    rcvMsgCountersCtx->msgRcvdErrCntr[errorCauseId] = 0;
    return TRUE;
  }
  else
  {
    logError("Invalid Error Cause ID %d being received ",errorCauseId);
    return FALSE;
  }
}

/* Message Counters Changes Ended */

