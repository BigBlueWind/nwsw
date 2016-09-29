/**
 * @file    lth_sm_if.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file is the starting point for LTH module
 *
 * @author  Muktheshwar Madishetty
 * @date    12-10-2012
 * @details contains lth sm interface data structures
 */
 
#ifndef LTH_SM_IF_H
#define LTH_SM_IF_H


#include "cmn_types.h"
#define MAX_LEN_TRACE_MSG 255

typedef enum e_TraceConfigMod
{
  TRACECONFIGMOD_RRC,
  TRACECONFIGMOD_S1AP,
  TRACECONFIGMOD_PDCP,
  TRACECONFIGMOD_RLC
} TraceConfigMod;

typedef struct s_MsgLthSmTraceCntrlReq
{
  TraceConfigMod protocolName;
  TraceCommand  traceCtrlCmd;
}__attribute__((__packed__)) MsgLthSmTraceCntrlReq;

typedef struct s_MsgSmLthTraceCntrlCfm
{
  TraceConfigMod protocolName;
  TBOOL traceCtrlConfirm;
}__attribute__((__packed__)) MsgSmLthTraceCntrlCfm;


typedef struct s_MsgSmLthTrcInd
{
  UINT32   traceLen;
  struct timespec timeVal;
  TraceConfigMod protocolName;
  UINT8   traceMsg[MAX_LEN_TRACE_MSG];
}MsgSmLthTrcInd;
#endif /* LTH_SM_IF_H */
