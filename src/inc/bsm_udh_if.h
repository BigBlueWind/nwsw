/**
 * @file   bsm_udh_if.h
 * @brief  This file contains the messages that are exchanged between BSM and UDH
 * @date   24-May-2011 Created Intial Version mkata
 * @author mkata
 */

#ifndef BSM_UDH_IF_H_
#define BSM_UDH_IF_H_

#include "cmn_types.h"
#include "ueh_cfg.h"


/** @addtogroup if_group
 *  @{
 */

/** @name BSM_UDH_MESSAGES
 *  @{
 */

/* UE Status */

typedef struct s_MsgBsmUdhCfgReq
{
  UINT16  transId; /* transaction ID */
  UINT16  ueInactivityCheckIntervalSecs;
  UINT16  startRnti;
  UINT16  maxConnectedUes;
  UINT16  ulInactivityIntervalInSecs;
  UINT8   ueThroughtputTimerInSecs;
  UINT8   memoryDiagCfg;
} MsgBsmUdhCfgReq;

typedef EnbConfigSucc MsgUdhBsmCfgSucc;
typedef EnbConfigFail MsgUdhBsmCfgFail;

/* Active Rab Status */
typedef struct s_MsgBsmUdhActiveRabStatusReq
{
  CmdRef   cmdRef;
  UINT32   internalUeIdentity; /* if sender sends 0, interpret as 'all' */
} MsgBsmUdhActiveRabStatusReq;

/* MsgUehBsmActiveRabStatusRsp (Response to MsgBsmUehActiveRabStatusReq) */
typedef struct s_MsgUdhBsmActiveRabStatusRsp
{
  CmdRef           cmdRef;
  UINT16           numElem;
  ActiveRabStatus  activeRabStatus[MAX_ROWS_IN_ACTIVERABSTATUS_TABLE];
} MsgUdhBsmActiveRabStatusRsp;

typedef struct s_MsgBsmUdhModStateReq
{
  CmdRef          cmdRef;
  TagInfo         tagInfo;
} MsgBsmUdhModStateReq;

typedef struct s_MsgUdhBsmModStateRsp
{
  CmdRef          cmdRef;
  TagInfo         tagInfo;
  UINT32          numErabs;
  UINT32          aggregateDLThroughputKbps;
  UINT32          aggregateULThroughputKbps;
} MsgUdhBsmModStateRsp;
/* RAB History Request */
typedef struct s_MsgBsmUdhRabHistoryReq
{
  CmdRef          cmdRef;
} MsgBsmUdhRabHistoryReq;

typedef struct s_MsgUdhBsmRabHistoryRsp
{
  CmdRef          cmdRef;
  UINT16          numElem;
  RabHistory      rabHistory[MAX_ROWS_IN_RABHISTORY_TABLE];
} MsgUdhBsmRabHistoryRsp;

/** @}
 */

/** @}
 */


#endif /* BSM_UDH_IF_H_ */
