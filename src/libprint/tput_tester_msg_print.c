/**
 * @file    tput_tester_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief This file contains the print functions for the
 *          messages that are exchanged between COORD and NODE A and B
 * @author         :  Shweta Polepally
 * Date           :  01 July 2010
 * Change History :
 *
 */

#include "tput_tester_msg_print.h"

void printMsgCoordNodeXSessionCreateReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgCoordNodeXSessionCreateReq *msg = (MsgCoordNodeXSessionCreateReq*)data;
  UINT8  i;

  logMsgParams( "{\n sessionId=%d\n protocolType=%d\n enableIPSec=%d\n isInitConn=%d\n maxPktSize=%d\n sendInterval=%d\n recvInterval=%d\n numPktsInEachInterval=%d\n infoInterval=%d\n numOfPkts=%d\n yourEnd.ipAddr=%s\n yourEnd.mode=%d\n yourEnd.numPorts=%d ",
                  msg->sessionId, msg->protocolType, msg->enableIPSec, msg->isInitConn, msg->maxPktSize,
                  msg->sendInterval, msg->recvInterval, msg->numPktsInEachInterval, msg->infoInterval,
                  msg->numOfPkts, msg->yourEnd.ipAddr, msg->yourEnd.mode, msg->yourEnd.numPorts );
  /* This is the continuation of the above print */
  for ( i=0; i < msg->yourEnd.numPorts; i++ )
  {
    logMsgParams( "\n yourEnd.portNum %d = %d\n ", i+1, msg->yourEnd.portNum[i] );
  }

  logMsgParams( "\n peerEnd.ipAddr =%s\n peerEnd.mode=%d\n peerEnd.numPorts=%d\n ",
             msg->peerEnd.ipAddr, msg->peerEnd.mode, msg->peerEnd.numPorts );

  for ( i=0; i < msg->yourEnd.numPorts; i++ )
  {
    logMsgParams( "\n peerEnd.portNum %d=%d\n ", i+1, msg->peerEnd.portNum[i] );
  }
  logMsgParams( "%s","} \n" );
}


void printMsgNodeXCoordSessionCreateSucc(
    IO void    *data,
    IO UINT16  len
  )
{
  MsgNodeXCoordSessionCreateSucc *msg = (MsgNodeXCoordSessionCreateSucc*)data;

  logMsgParams( "{\n sessionId=%d\n }\n  ", msg->sessionId );
}

void printMsgNodeXCoordSessionCreateFail(
    IO void    *data,
    IO UINT16  len
  )
{
  MsgNodeXCoordSessionCreateFail *msg = (MsgNodeXCoordSessionCreateFail*)data;

  logMsgParams( "{\n sessionId=%d\n reason=%d\n }\n  ", msg->sessionId, msg->reason );
}

void printMsgCoordNodeXStartDataTx(
    IO void    *data,
    IO UINT16  len
  )
{
  MsgCoordNodeXStartDataTx *msg = (MsgCoordNodeXStartDataTx*)data;

  logMsgParams( "{\n sessionId=%d\n }\n  ", msg->sessionId );
}

void printMsgNodeXCoordDataTxRxComplete(
    IO void    *data,
    IO UINT16  len
  )
{
  logMsgParams( "{\n MsgNodeCoordDataTxRxComplete doesn't have any parameters \n }\n ");
}

void printMsgCoordNodeXSessionInfoReq(
    IO void    *data,
    IO UINT16  len
  )
{
  MsgSessionInfoReq *msg = (MsgSessionInfoReq*)data;

  logMsgParams( "{\n sessionId=%d\n }\n  ", msg->sessionId );
}

void printMsgNodeXCoordSessionInfoRsp(
    IO void    *data,
    IO UINT16  len
  )
{
  MsgSessionInfoRsp *msg = (MsgSessionInfoRsp*)data;
  UINT8 i;

  logMsgParams( "{\n sessionId=%d\n numSockFds=%d\n", msg->sessionId, msg->numSockFds );
  for ( i=0; i< msg->numSockFds; i++ )
  {
    logMsgParams( "\n sockFd=%d\n numOfBytesSent=%d\n numOfBytesRcvd=%d\n numOfPktsSent=%d\n numOfPktsRcvd=%d\n numOfPktsExpected=%d\n",
                msg->sockFd[i], msg->numOfBytesSent[i], msg->numOfBytesRcvd[i],
                msg->numOfPktsSent[i], msg->numOfPktsRcvd[i], msg->numOfPktsExpected[i] );
  }
  logMsgParams( "\nCPU (usr|sys) = %ld.%06ld  | %ld.%06ld \n } \n ",
           msg->rUsage.ru_utime.tv_sec, msg->rUsage.ru_utime.tv_usec, msg->rUsage.ru_stime.tv_sec, msg->rUsage.ru_stime.tv_usec);
}

void printMsgCoordNodeXSessionDelReq(
    IO void    *data,
    IO UINT16  len
  )
{
  MsgSessionDelReq *msg = (MsgSessionDelReq*)data;

  logMsgParams( "{\n sessionId=%d\n }\n  ", msg->sessionId );
}

void printMsgNodeXCoordSessionDelSucc(
    IO void    *data,
    IO UINT16  len
  )
{
  MsgSessionDelSucc *msg = (MsgSessionDelSucc*)data;

  logMsgParams( "{\n sessionId=%d\n }\n  ", msg->sessionId );
}

void printMsgNodeXCoordSessionDelFail(
    IO void    *data,
    IO UINT16  len
  )
{
  MsgSessionDelFail *msg = (MsgSessionDelFail*)data;

  logMsgParams( "{\n sessionId=%d\n reason=%d\n }\n  ", msg->sessionId, msg->reason );
}

