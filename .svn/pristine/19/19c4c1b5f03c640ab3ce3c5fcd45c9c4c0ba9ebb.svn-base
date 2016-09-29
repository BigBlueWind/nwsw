/**
 * @file    bsm_egtp_if.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains the messages that are exchanged between BSM and EGTP
 * @author  Akash Dutta (for ICD)
 * @date    14 Sep 2010
 *
 * @details S1AP : S1AP stack + manager
 */

#ifndef BSM_EGTP_IF_H
#define BSM_EGTP_IF_H

#include "cmn_types.h"
#include "msg_ids.h"
#include "bs_tables.h"

/** @addtogroup if_group
 *  @{
 */

/** @name BSM_EGTP_MESSAGES
 *  @{
 */

/* Initial Configuration */
typedef struct s_MsgBsmEgtpInitConfigReq
{
  UINT16            transId; /* transaction ID */
  UINT8             enodebIpAddr[MAX_SIZE_ENBSTATUS_ENODEBIPADDRESS+1];
} MsgBsmEgtpInitConfigReq;

typedef EnbConfigSucc MsgEgtpBsmInitConfigSucc;
typedef EnbConfigFail MsgEgtpBsmInitConfigFail;

/** @}
 */

/** @}
 */

#endif /* BSM_EGTP_IF_H */
