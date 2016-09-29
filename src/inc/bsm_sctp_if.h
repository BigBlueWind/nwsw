/**
 * @file    bsm_sctp_if.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains the messages that are exchanged between BSM and SCTP
 * @author  Akash Dutta (for enbConfig ICD)
 * @date    20 Sep 2010
 *
 * @details SCTP : SCTP stack
 */

#ifndef BSM_SCTP_IF_H
#define BSM_SCTP_IF_H

#include "cmn_types.h"
#include "msg_ids.h"
#include "bs_tables.h"

/** @addtogroup if_group
 *  @{
 */

/** @name BSM_SCTP_MESSAGES
 *  @{
 */

/* Initial Configuration */
typedef struct s_MsgBsmSctpInitConfigReq
{
  UINT16            transId; /* transaction ID */
  UINT8             enodebIpAddr[MAX_SIZE_ENBSTATUS_ENODEBIPADDRESS + 1];
} MsgBsmSctpInitConfigReq;

typedef EnbConfigSucc MsgSctpBsmInitConfigSucc;
typedef EnbConfigFail MsgSctpBsmInitConfigFail;

/** @}
 */

/** @}
 */

#endif /* BSM_SCTP_IF_H */
