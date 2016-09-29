/**
 * @file    bsm_lth_if.h
 * @brief   Data structures for BSM and LTH interface
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author  Muktheshwar Madishetty
 * @date    12-10-2012
 *
 */

#ifndef BSM_LTH_IF_H_
#define BSM_LTH_IF_H_

#include "all_cmn.h"

#define MAX_LEN_ENBID 7

/** @addtogroup if_group
 *  @{
 */

/** @name BSM_LTH_MESSAGES
 *  @{
 */

typedef struct s_MsgBsmLthStartTraceReq
{
  FileName    fileName;
} MsgBsmLthStartTraceReq;

/* MsgLthBsmTraceComplete has no contents
 * MsgLthBsmStartTraceSucc has no contents
 * */

typedef struct s_MsgLthBsmStartTraceFail
{
  BsErrorValue bsErrorValue;
} MsgLthBsmStartTraceFail;

typedef struct s_MsgLthBsmStopTraceFail
{
  BsErrorValue bsErrorValue;
}MsgLthBsmStopTraceFail;

typedef SetLogConfig MsgBsmLthSetLogCfg;
/** @}
 */

/** @}
 */

typedef struct s_MsgBsmLthInitConfigReq
{
  UINT16   transId;
  TraceConfig    traceConfig;
} MsgBsmLthInitConfigReq;
typedef EnbConfigSucc MsgLthBsmInitConfigSucc;
#endif /* BSM_LTH_IF_H_ */
