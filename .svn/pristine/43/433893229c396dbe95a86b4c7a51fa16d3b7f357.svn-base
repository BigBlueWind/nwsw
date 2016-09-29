/**
 * @file    bsm_s1ap_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between BSM and S1AP
 * @author :  Ramprasad Reddy Chinta
 * Date   :
 * Change History:
 *
 */

#include "bsm_s1ap_msg_print.h"

void printMsgBsmS1apConfigReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmS1apConfigReq *msg =  (MsgBsmS1apConfigReq*)data;
  logMsgParams("%s","{\n");

  logMsgParams(
      " transId=%d\n enodebId=%d\n enodebName=%s\n maxConnectedUes=%d\n plmnMCC=%s\n plmnMNC=%s\n tac=%d\n"
      " defaultPagCycle=%d\n mmeIpAddr=%s\n enableSecureMmeLink=%d\n enableSecureGtpTunnels=%d\n numSctpStreams=%d\n",
      msg->transId, msg->enodebId, msg->enodebName, msg->maxConnectedUes, msg->plmnMCC, msg->plmnMNC,
      msg->tac, msg->defaultPagCycle, msg->mmeConfig.mmeIpAddr, msg->mmeConfig.enableSecureMmeLink,
      msg->mmeConfig.enableSecureGtpTunnels, msg->mmeConfig.numSctpStreams
       );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgBsmS1apInitConfigReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmS1apInitConfigReq *msg =  (MsgBsmS1apInitConfigReq*)data;
  logMsgParams("%s","{\n");
  logMsgParams(
      " transId=%d\n enodebStaticIpAddr=%s\n enodebNetmaskAddr=%s\n" ,
        msg->transId, msg->enodebStaticIpAddr, msg->enodebNetmaskAddr );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgS1apBsmS1LinkUpInd(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgS1apBsmLinkUpInd *msg = (MsgS1apBsmLinkUpInd*)data;
  logMsgParams( "{\n MME IP Addr=%s\n", msg->mmeIpAddr ) ;
  logMsgParams("} Total length = %d\n", len );
}

void printMsgS1apBsmS1LinkDownInd(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgS1apBsmLinkDownInd *msg = (MsgS1apBsmLinkDownInd*)data;
  logMsgParams( "{\n MME IP Addr=%s\n", msg->mmeIpAddr ) ;
  logMsgParams("} Total length = %d\n", len );
}

void printMsgS1apBsmS1LinkClosedInd(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgS1apBsmLinkClosedInd *msg = (MsgS1apBsmLinkClosedInd*)data;
  logMsgParams( "{\n MME IP Addr=%s\n", msg->mmeIpAddr ) ;
  logMsgParams("} Total length = %d\n", len );
}

void printMsgS1apBsmS1ResetInd(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgS1apBsmS1ResetInd *msg = (MsgS1apBsmS1ResetInd*)data;
  logMsgParams( "{\n MME IP Addr=%s\n", msg->mmeIpAddr ) ;
  logMsgParams("} Total length = %d\n", len );
}
