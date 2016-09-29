/**
 * @file    pmh_rrc_msg_print.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains all function definitions used to print Messages between PMH and GTP
 *
 * @author     : Ankur Agrawal
 * Date       : 17-Mar-2011
 * Description:
 */

#include "all_cmn.h"
#include "ueh_pmh_if.h"

#ifndef PMH_RRC_MSG_PRINT_H_
#define PMH_RRC_MSG_PRINT_H_
void printMsgPmhSmRrcGenStatReq(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgSmPmhRrcGenStatRsp(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgPmhSmRrcCellStatReq(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgRrcPmhCellStatRsp(
  IO  void    *data,
  IO  UINT16  len
  );
#endif /* PMH_RRC_MSG_PRINT_H_ */
