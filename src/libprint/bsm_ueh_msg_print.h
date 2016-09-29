/**
 * @file    bsm_ueh_msg_print.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between PRM and UEH
 * @author :  Ritesh
 * Date   :
 * Change History:
 *
 */

#ifndef BSM_UEH_MSG_PRINT_H_
#define BSM_UEH_MSG_PRINT_H_
#include "bsm_ueh_if.h"
#include "all_cmn.h"

void printMsgBsmUehConfigReq(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgBsmUehModStateReq(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgUehBsmModStateRsp(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgConfigReq(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgActiveUeStatusReq(
  IO void    *data,
  IO UINT16  len
  );
void printMsgActiveUeStatusRsp(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgBsmUehUeEventHistoryReq(
  IO void    *data,
  IO UINT16  len
  );
void printMsgUehBsmUeEventHistoryRsp(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgUehBsmUnknownEnbUeS1apId(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgBsmUehCellBarReq(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgUehBsmCellBarSucc(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgBsmUehCellUnbarReq(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgUehBsmCellUnbarSucc(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgBsmUehUeEventHistoryLogConfigReq( 
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgUehBsmUeEventHistoryLogConfigSucc(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgUehBsmUeEventHistoryLogConfigFail(
  IO  void    *data,
  IO  UINT16  len
  );
#endif /* PRM_UEH_MSG_PRINT_H_ */
