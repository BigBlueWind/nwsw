/**
 * @file    bsm_ueh_if.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains the messages that are exchanged between BSM and UEH
 * @author  Akash Dutta (for ICD)
 * @date    13 Sep 2010
 *
 * @details  UEH : UE handler (control plane UE manager)
 */

#ifndef BSM_UEH_IF_H
#define BSM_UEH_IF_H

#include "cmn_types.h"
#include "msg_ids.h"
#include "bs_tables.h"

#define START_TRACE    1
#define CONTINUE_TRACE 1
#define STOP_TRACE     0
#define NUM_ZERO       0

#define BSM_UEH_MAX_RNTI          65523   /* 0xFFF3 == 65523*/

typedef struct s_SysInfo4Config
{
  UINT16                        neighEutraPhyCellId;
  IntraFreqNeighCellQOffsetCell intraFreqNeighCellQOffsetCell;
}SysInfo4Config;

/** @addtogroup if_group
 *  @{
 */

/** @name BSM_UEH_MESSAGES
 *  @{
 */
/*BS-1799 cell barring procedure*/
typedef enum e_CellBarred
{
  CELLBARRED_BARRED = 0,
  CELLBARRED_NOTBARRED
} CellBarred;

/* MSG_BSM_UEH_ENB_CONFIG_REQ */
typedef struct s_MsgBsmUehEnbConfigReq
{
  UINT16                transId; /* transaction ID */
  UINT32                enodebId;
  CellBarred            cellBarred;
  CellConfig            cellCfg;
  SchedulerConfig       schCfg;
  PhyConfig             phyCfg;
  SysInfo1Config        sib1Cfg;
  SysInfo2Config        sib2Cfg;
  SysInfo3Config        sib3Cfg;
  QosConfig             qosCfg;
  SysInfo4Config        sib4Cfg;
  EutraHoProfileConfig  eutraHoProfileConfig;
  UINT8                 numEutraNeighborCells;/* Indicates the number of valid elements present in the array below */
  EutraNeighCellConfig  eutraNeighCellConfig[MAX_ROWS_IN_EUTRANEIGHCELLCONFIG_TABLE];
  SrsConfigDedicated    srsCfgDedicated;
  SecurityConfig        securityConfig;
} __attribute__((__packed__)) MsgBsmUehEnbConfigReq;

typedef EnbConfigSucc MsgUehBsmEnbConfigSucc;
typedef EnbConfigFail MsgUehBsmEnbConfigFail;



typedef struct s_MsgBsmUehActiveUeStatusReq
{
  CmdRef          cmdRef;
} MsgBsmUehActiveUeStatusReq;

typedef struct s_MsgUehBsmActiveUeStatusRsp
{
  CmdRef          cmdRef;
  UINT16          numElem;
  ActiveUeStatus  activeUeStatus[MAX_ROWS_IN_ACTIVEUESTATUS_TABLE];
} MsgUehBsmActiveUeStatusRsp;

typedef struct s_MsgBsmUehModStateReq
{
  CmdRef          cmdRef;
  TagInfo         tagInfo;
} MsgBsmUehModStateReq;

typedef struct s_MsgUehBsmModStateRsp
{
  CmdRef          cmdRef;
  TagInfo         tagInfo;
  UINT32          numOfUesConnected;
} MsgUehBsmModStateRsp;

/* UE Event history Log Config messages */
typedef struct s_MsgBsmUehUeEventHistoryLogConfigReq
{
  CmdRef                  cmdRef;
  UeEventHistoryLogConfig ueEventHistoryLogConfig;
}MsgBsmUehUeEventHistoryLogConfigReq;

typedef MsgBsmUehUeEventHistoryLogConfigReq MsgUehBsmUeEventHistoryLogConfigSucc;
typedef MsgBsmUehUeEventHistoryLogConfigReq MsgUehBsmUeEventHistoryLogConfigFail;

/* UE Event History Request */
typedef struct s_MsgBsmUehUeEventHistoryReq
{
  CmdRef          cmdRef;
} MsgBsmUehUeEventHistoryReq;

typedef struct s_MsgUehBsmUeEventHistoryRsp
{
  CmdRef          cmdRef;
  UINT16          numElem;
  UeEventHistory  ueEventHistory[MAX_ROWS_IN_UEEVENTHISTORY_TABLE];
} MsgUehBsmUeEventHistoryRsp;

typedef struct s_MsgUehBsmUnknownEnbUeS1apId
{
  UINT16      ueId;
}MsgUehBsmUnknownEnbUeS1apId;

/*BS-1799 cell barring*/
typedef struct s_MsgBsmUehCellBarReq
{
  UINT16                transId;
  UINT8                 cellId;
} MsgBsmUehCellBarReq;

typedef struct s_MsgUehBsmCellBarSucc
{
  UINT16                transId;
  UINT16                sfnReceived;
  UINT16                sfnNewSIBTx;
} MsgUehBsmCellBarSucc;

typedef struct s_MsgBsmUehCellUnbarReq
{
  UINT16                transId;
  UINT8                 cellId;
} MsgBsmUehCellUnbarReq;

typedef struct s_MsgUehBsmCellUnbarSucc
{
  UINT16                transId;
  UINT16                sfnReceived;
  UINT16                sfnNewSIBTx;
} MsgUehBsmCellUnbarSucc;

/** @}
 */

/** @}
 */
#endif /* BSM_UEH_IF_H */
