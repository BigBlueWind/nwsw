/**
 * @file    pmh_s1ap_msg_print.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains all function definitions used to print Messages between PMH and S1AP
 *
 * @author     : Ankur Agrawal
 * Date       : 08-Jul-2011
 * Description:
 */

#ifndef PMH_S1AP_MSG_PRINT_H_
#define PMH_S1AP_MSG_PRINT_H_

#include "all_cmn.h"
#include "ueh_pmh_if.h"

void printMsgPmhSmS1apGenStatReq(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgPmhSmS1apPeerNodeStatReq(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgSmPmhS1apGenStatRsp(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgSmPmhS1apPeerNodeStatRsp(
  IO  void    *data,
  IO  UINT16  len
  );

#endif /* PMH_S1AP_MSG_PRINT_H_ */
