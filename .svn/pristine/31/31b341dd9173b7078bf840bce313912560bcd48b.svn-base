/**
 * @file    bsm_pmh_if.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains the messages that are exchanged between BSM and PMH
 * @author  Ramprasad Reddy
 * @date    16 Feb 2011
 */

#ifndef BSM_PMH_IF_H_
#define BSM_PMH_IF_H_

#include "cmn_types.h"
#include "msg_ids.h"
#include "bs_tables.h"
#include "bs_events.h"

/** @addtogroup if_group
 *  @{
 */

/** @name BSM_PMH_MESSAGES
 *  @{
 */

typedef struct s_MsgBsmPmhConfigReq
{
  UINT32             enodebId;
  UINT16             transId;
  PmCounterLogConfig pmCounterLogConfig;
  CounterConfig      counterConfig[MAX_ROWS_IN_COUNTERCONFIG_TABLE];
} MsgBsmPmhConfigReq;

typedef EnbConfigSucc MsgPmhBsmConfigSucc;
typedef EnbConfigFail MsgPmhBsmConfigFail;

typedef struct s_MsgBsmPmhCntrInputConfigReq
{
  UINT16             transId;
  UINT8              cellId;
  UINT32             peerId;
} MsgBsmPmhCntrInputConfigReq;

/* OP-START */
typedef ErrorDesc MsgPmhBsmOpstartFail;

typedef EnbConfigSucc MsgPmhBsmCntrInputConfigSucc;
typedef EnbConfigFail MsgPmhBsmCntrInputConfigFail;

typedef struct s_MsgPmhBsmCntrCollectFail
{
  ErrorDesc	errorDesc;
} __attribute__((__packed__))MsgPmhBsmCntrCollectFail;

typedef struct s_MsgPmhBsmCntrFileAvailable
{
  FilePath    pmLogFileName;
} __attribute__((__packed__))MsgPmhBsmCntrFileAvailable;

typedef struct s_MsgBsmPmhPmFileUpload
{
  FilePath    pmLogFileName;
} __attribute__((__packed__))MsgBsmPmhPmFileUploaded;

typedef SetLogConfig MsgBsmPmhSetLogCfg;

typedef struct s_MsgBsmPmhCounterClearReq
{
  CmdRef    cmdRef;
} __attribute__((__packed__))MsgBsmPmhCounterClearReq;

typedef struct s_MsgPmhBsmCounterClearSucc
{
  CmdRef    cmdRef;
} __attribute__((__packed__))MsgPmhBsmCounterClearSucc;

typedef struct s_MsgPmhBsmCounterClearFail
{
  CmdRef         cmdRef;
  BsErrorValue   bsError;
} __attribute__((__packed__))MsgPmhBsmCounterClearFail;

typedef struct s_MsgBsmPmhDiagCounterCmd
{
  CmdRef         cmdRef;
  ENbCommand     eNbCommand;
} __attribute__((__packed__))MsgBsmPmhDiagCounterCmd;

typedef struct s_MsgPmhBsmDiagCounterCmdRsp
{
  CmdRef       cmdRef;
  ENbCommand   eNbCommand;
} __attribute__((__packed__))MsgPmhBsmDiagCounterCmdRsp;

typedef struct s_MsgPmhBsmMeasurementCompleteInd
{
  ProfileId    profileId;
  UINT8        cause[MAX_SIZE_COUNTERCOLLECTIONSTATUS_STOPPEDREASON +1];
} MsgPmhBsmMeasurementCompleteInd;
/** @}
 */

/** @}
 */

#endif /* BSM_PMH_IF_H_ */
