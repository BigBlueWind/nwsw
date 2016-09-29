/**
 * @file    pmh_mac_msg_print.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains all function definitions used to print Messages between PMH and MAC
 *
 * @author     : Ankur Agrawal
 * Date       : 09-Mar-2011
 * Description:
 */
#include "all_cmn.h"
#include "ueh_pmh_if.h"

#ifndef PMH_MAC_MSG_PRINT_H_
#define PMH_MAC_MSG_PRINT_H_

void printMsgPmhSmMacGenStatReq(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgPmhSmMacRlcsapStatReq(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgPmhSmMacPhysapStatReq(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgSmPmhMacGenStatRsp(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgSmPmhMacRlcsapStatRsp(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgSmPmhMacPhysapStatRsp(
  IO  void    *data,
  IO  UINT16  len
  );
#endif /* PMH_MAC_MSG_PRINT_H_ */
