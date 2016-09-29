/**
 * @file    pmh_gtp_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between PMH and GTP
 * @author : Ankur Agrawal
 * Date   : 07-Mar-2011
 * Change History:
 *
 */

#include "pmh_gtp_msg_print.h"

void printMsgGtpPmhGenStatRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgSmPmhGtpGenStatRsp *msg = (MsgSmPmhGtpGenStatRsp*)data;
  logMsgParams( "{\n transId=%d\n numTxMsg=%d\n numRxMsg=%d\n numActvTnls=%d\n numErrInd=%d\n rxDesgMsg=%d\n invVerRx=%d\n invProtTypeRx=%d\n",\
              msg->transId, msg->gtpGenCounters.numTxMsg,msg->gtpGenCounters.numRxMsg, msg->gtpGenCounters.numActvTnls, msg->gtpGenCounters.numErrInd, msg->gtpGenCounters.rxDesgMsg, msg->gtpGenCounters.invVerRx, msg->gtpGenCounters.invProtTypeRx);
  logMsgParams( " invMsgRx=%d\n invMsgLenRx=%d\n invIeOrderRx=%d\n invIeLenRx=%d\n invIeFormatRx=%d\n invManIeLenRx=%d\n missMandIeRx=%d\n",\
              msg->gtpGenCounters.invMsgRx, msg->gtpGenCounters.invMsgLenRx, msg->gtpGenCounters.invIeOrderRx, msg->gtpGenCounters.invIeLenRx, msg->gtpGenCounters.invIeFormatRx, msg->gtpGenCounters.invManIeLenRx, msg->gtpGenCounters.missMandIeRx);
  logMsgParams( " unSuppExtHdrRx=%d\n wrngExtHdrRx=%d\n msgUnableCompRx=%d\n msgUnableCompTx=%d\n missMandIeTx=%d\n memAllocFldTx=%d\n",\
              msg->gtpGenCounters.unSuppExtHdrRx, msg->gtpGenCounters.wrngExtHdrRx, msg->gtpGenCounters.msgUnableCompRx, msg->gtpGenCounters.msgUnableCompTx, msg->gtpGenCounters.missMandIeTx, msg->gtpGenCounters.memAllocFldTx);
  logMsgParams( " invMsgTx=%d\n invTeidTx=%d\n invTeidRx=%d\n errSeqNoTx=%d\n errSeqNoRx=%d\n tpduExcdTx=%d\n invDatTypeTx=%d\n",\
              msg->gtpGenCounters.invMsgTx, msg->gtpGenCounters.invTeidTx, msg->gtpGenCounters.invTeidRx, msg->gtpGenCounters.errSeqNoTx, msg->gtpGenCounters.errSeqNoRx, msg->gtpGenCounters.tpduExcdTx, msg->gtpGenCounters.invDatTypeTx);
  logMsgParams("} Total length = %d\n", len );
}


void printMsgPmhGtpGenStatReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhSmGtpGenStatReq *msg = (MsgPmhSmGtpGenStatReq*)data;
  logMsgParams( "{\n transId=%d\n resetAllCounters=%d\n",msg->transId, msg->resetAllCounters);
  logMsgParams("} Total length = %d\n", len );
}
