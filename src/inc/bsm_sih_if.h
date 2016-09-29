/**
 * @file    bsm_sih_if.h
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief   This file contains the messages exchanged between BSM and SIH
 *
 * @author :
 *
 */

#ifndef BSM_SIH_IF_H
#define BSM_SIH_IF_H

#include "cmn_types.h"
#include "bs_alarms.h"
#include "noti_msg_get_set.h"

/** @addtogroup if_group
 *  @{
 */

/** @name BSM_SIH_STRUCTURES
 *  @{
 */

typedef struct s_MsgBsmSihInitConfigReq
{
  UINT16    transId;
  /* This is same as ip address present in EnbConfig Table */
  UINT8   emsAddr[MAX_SIZE_ENBREGISTRATIONCONFIG_EMSADDR + 1 ];
  UINT8   snmpCommunityString[MAX_SIZE_ENBREGISTRATIONCONFIG_SNMPCOMMUNITYSTRING + 1 ];
} MsgBsmSihInitConfigReq;

typedef EnbConfigSucc MsgSihBsmInitConfigSucc;
typedef EnbConfigFail MsgSihBsmInitConfigFail;

typedef SetLogConfig MsgBsmSihSetLogCfg;

/** @}
 */

/** @}
 */

typedef struct s_NotiMsgColInfo
{
  ColName       colName;
  UINT32        colId;
  DataType      dataType;
  UINT16        length;
} NotiMsgColInfo;

typedef struct s_MsgBsmSihNotiInfo
{
  NotiId	  notiMsgId;
  NotiMsgName	  notiMsgName;
  UINT32          noOfColumns;
  NotiMsgColInfo  notiMsgColInfo[MAX_ELEMENTS_IN_TABLE];
}MsgBsmSihNotiInfo;

typedef struct s_NotiMsgContext
{
  UINT32      	     noOfvalidNotiMsg;
  MsgBsmSihNotiInfo  notiMsgInfo[MAX_NUM_NOTI_MSGS];
} NotiMsgContext;

#endif /* BSM_SIH_IF_H */
