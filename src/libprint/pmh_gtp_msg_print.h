/**
 * @file    pmh_gtp_msg_print.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains all function definitions used to print Messages between PMH and GTP
 *
 * @author     : Ankur Agrawal
 * Date       : 07-Mar-2011
 * Description:
 */
#include "all_cmn.h"
#include "ueh_pmh_if.h"

#ifndef PMH_GTP_MSG_PRINT_H_
#define PMH_GTP_MSG_PRINT_H_
void printMsgGtpPmhGenStatRsp(
  IO  void    *data,
  IO  UINT16  len
  );
void printMsgPmhGtpGenStatReq(
  IO  void    *data,
  IO  UINT16  len
  );

#endif /* PMH_GTP_MSG_PRINT_H_ */
