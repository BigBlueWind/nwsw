/**
 * @file    print_noti.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains all function definitions used to print Messages in TLV format.
 *
 * @author     : mkata
 * Date       : 10 - 05 - 2010
 * Description:
 */

#ifndef PRINT_NOTI_H
#define PRINT_NOTI_H

#include "cmn_types.h"
#include "all_cfg.h"
#include "enumstr.h"
#include "tlv_enc_dec.h"
#include "bsm_ueh_if.h"
#include "bsm_sih_if.h"

/* Noti Msg Context */
extern NotiMsgContext     g_PrintTlvNotiMsgContext;

/* Get Noti Msg Context */
#define getNotiMsgTlvInfo()           &(g_PrintTlvNotiMsgContext)

#define tlvPrintGetNotiMsgInfo(tableId)   &(g_PrintTlvNotiMsgContext.notiMsgInfo[tableId])

/* Print TLV messages initialisation function */
void printNotiMsgTlvInit(
  IO UINT8 *globalConfigFilePath
  );

void printNotiMsgTlv(
  IO NotiObj   *notiObj
  );

void printNotiMsgInfo(
  IN NotiId notiMsgId
  );

/**
 * @param data  Contains Encoded Data in TLV format
 * @param len length of the message sent on socket
 * @details This function will Logs NotiObjReqMsg
 */
void printNotiObjReqMsg(
  IO void    *data,
  IO UINT16  len
  );
  
MsgBsmSihNotiInfo* printTlvGetNotiMsgPtrByNotiMsgId(
  IN NotiId    notiMsgId
  );

int parseNotiMsgCfg(
  IN UINT8*          configFilePath,
  IO NotiMsgContext *notiMsgCtx
  );

#endif /* PRINT_NOTI_H */
