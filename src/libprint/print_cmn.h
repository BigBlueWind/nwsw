/**
 * @file    print_cmn.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file is the contains ptrint functions which are common across
 *          all modules.
 *
 * @author     : Shweta Polepally
 * Date       : 10 Dec 2010
 * Description: Basic version
 *
 */


#ifndef PRINT_CMN_H
#define PRINT_CMN_H

#include "cmn_types.h"
#include "tlv_enc_dec.h"
#include "bsm_ueh_if.h"
#include "lth_sm_if.h"
#include "bs_alarms.h"
#include "bs_events.h"
#include "start_indication.h"
#include "ctr_id.h"

void printCmdRef(
  IN CmdRef *ref
  );
void printMsgEventMessage(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgSetMsgTrace(
  IO  void    *data,
  IO  UINT16  len
  );
char *getModExitCauseName(
  IN ModExitCause modExitCause
  );
void printMsgExitCmd(
  void    *data,
  UINT16  len
  );
void printMsgOpStartFail(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgEnbConfigSucc(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgEnbConfigFail(
  IO  void    *data,
  IO  UINT16  len
  );
char *getEnbDisableReasonName(
  IN EnbDisableReason enbDisableReason
  );
void printMsgOpStopReq(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgTraceCntrlReq(
  void    *data,
  UINT16  len
  );
void printMsgTraceCntrlCfm(
  void    *data,
  UINT16  len
  );
void printMsgTraceInd(
  void    *data,
  UINT16  len
  );
void printStartIndMsg(
    void    *data,
    UINT16  len
    );
void printMsgHello(
    void    *data,
    UINT16  len
    );
void printMsgHelloAck(
    void    *data,
    UINT16  len
    );
void PrintMsgSetLogConfig(
  void    *data,
  UINT16  len
  );
void PrintMsgDstatsCfg(
  void    *data,
  UINT16  len
  );
void PrintMsgDstatsInd(
  void    *data,
  UINT16  len
  );
#endif
