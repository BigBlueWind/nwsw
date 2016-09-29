/**
 * @file    bsm_s1ap_if.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains the messages that are exchanged between BSM and S1AP
 * @author  Akash Dutta (for ICD)
 * @date    14 Sep 2010
 *
 * @details S1AP : S1AP stack + manager
 */

#ifndef BSM_S1AP_IF_H
#define BSM_S1AP_IF_H

#include "cmn_types.h"
#include "msg_ids.h"
#include "bs_tables.h"
#include "bs_events.h"
#include "bs_alarms.h"

/** @addtogroup if_group
 *  @{
 */

/** @name BSM_S1AP_MESSAGES
 *  @{
 */

/* Configuration */
typedef struct s_MsgBsmS1apConfigReq
{
  UINT16                transId;  /* transaction ID */
  /* From EnodebConfig */
  UINT32                enodebId;
  UINT8                 enodebName[MAX_SIZE_ENODEBCONFIG_ENODEBNAME + 1 ];
  /* From CellConfig */
  UINT16                maxConnectedUes;
  UINT8                 plmnMCC[MAX_SIZE_CELLCONFIG_PLMNMCC+1];
  UINT8                 plmnMNC[MAX_SIZE_CELLCONFIG_PLMNMNC+1];
  UINT16                tac;
  /* From SysInfo2Config */
  DefaultPagCycle       defaultPagCycle;
  /* MmeConfig */
  MmeConfig             mmeConfig;
} __attribute__((__packed__)) MsgBsmS1apConfigReq;

/** @}
 */

/** @}
 */

typedef MsgBsmS1apConfigReq S1apCfgInfo;

/** @addtogroup if_group
 *  @{
 */

/** @name BSM_S1AP_MESSAGES
 *  @{
 */

typedef EnbConfigSucc MsgS1apBsmConfigSucc;
typedef EnbConfigFail MsgS1apBsmConfigFail;

typedef struct s_MsgUehBsmS1LinkStatus
{
  UINT8         mmeIpAddr[MAX_SIZE_MMECONFIG_MMEIPADDR + 1 ];
}MsgS1apBsmS1LinkStatus;

typedef MsgS1apBsmS1LinkStatus MsgS1apBsmLinkUpInd;
typedef MsgS1apBsmS1LinkStatus MsgS1apBsmLinkDownInd;
typedef MsgS1apBsmS1LinkStatus MsgS1apBsmLinkClosedInd;
typedef MsgS1apBsmS1LinkStatus MsgS1apBsmS1ResetInd;

/* Initial Configuration */
typedef struct s_MsgBsmS1apInitConfigReq
{
  UINT16     transId; /* transaction ID */
  UINT8      enodebStaticIpAddr[MAX_SIZE_ENODEBCONFIG_ENODEBSTATICIPADDR + 1 ];  /* from EnodebConfig */
  UINT8      enodebNetmaskAddr[MAX_SIZE_ENODEBCONFIG_ENODEBNETMASKADDR + 1 ];    /* from EnodebConfig */
} MsgBsmS1apInitConfigReq;

typedef EnbConfigSucc MsgS1apBsmInitConfigSucc;
typedef EnbConfigFail MsgS1apBsmInitConfigFail;

/** @}
 */

/** @}
 */

#endif /* BSM_S1AP_IF_H */
