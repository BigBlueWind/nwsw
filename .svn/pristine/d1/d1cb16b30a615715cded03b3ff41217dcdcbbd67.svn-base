/**
 * @file    bsm_health_check.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions related to health check - Actions to be taken on HELLO message.
 *
 * @author:  Shweta Polepally
 * @date:    Sep 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param   msg        Received message from PRM
 * @param   bsmContext BSM Context
 * @details This function will process Hello message received in all states of BSM
 **/
inline BsmProceduralState bsmProcHelloInAllStates(
  IN Msg        *msg,
  IO BsmContext *bsmContext
  )
{
  MsgPrmModHello     *hello = (MsgPrmModHello*)msg->data;
  MsgModPrmHelloAck   helloAck;
  clearBytes( &helloAck, sizeof(MsgModPrmHelloAck) );
  helloAck.helloAckSeqNum = hello->helloSeqNum;

  createAndSendMsg( MSG_BSM_PRM_HELLO_ACK, (UINT8*)&helloAck, sizeof(MsgModPrmHelloAck) );

  return bsmContext->bsmProceduralState;
}
