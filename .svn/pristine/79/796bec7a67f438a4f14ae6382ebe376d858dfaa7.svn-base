/**
 * @file   prm_deh_if.h
 * @brief  This file contains the messages that are exchanged between PRM and DEH
 * @date   26-Apr-2012
 * @author aagrawal
 */

#ifndef PRM_DEH_IF_H_
#define PRM_DEH_IF_H_

#include "cmn_types.h"
#include "tlv_enc_dec.h"
#include "bs_events.h"
#include "bs_alarms.h"

#define MAX_FWBUILDINFO_DESC    17
#define RH_SIDE_INFO  6

typedef struct s_MsgPrmDehRhSwInstallReq
{
  UINT8        filename[MAX_SIZE_SWINSTALL_FILENAME + 1 ];       /* Name of the Config file */
} MsgPrmDehRhSwInstallReq;

typedef struct s_MsgDehPrmRhSwInstallSucc
{
  UINT8         version[MAX_SIZE_SWACTIVATIONSTATUS_VERSION+1];
} MsgDehPrmRhSwInstallSucc;

typedef struct s_MsgDehPrmRhSwInstallFail
{
  ErrorDesc    errorDesc;
} MsgDehPrmRhSwInstallFail;



typedef struct s_MsgPrmDehRhSwActivateReq
{
  UINT8         version[MAX_SIZE_SWACTIVATIONSTATUS_VERSION+1];
} MsgPrmDehRhSwActivateReq;

typedef struct s_MsgDehPrmRhSwActivateSucc
{
  UINT8         version[MAX_SIZE_SWACTIVATIONSTATUS_VERSION+1];
} MsgDehPrmRhSwActivateSucc;

typedef struct s_MsgDehPrmRhSwActivateFail
{
  UINT8        version[MAX_SIZE_SWACTIVATIONSTATUS_VERSION+1];
  ErrorDesc    errorDesc;
} MsgDehPrmRhSwActivateFail;

typedef struct s_RhSwInfo
{
  UINT8  version[MAX_SIZE_SWACTIVATIONSTATUS_VERSION+1];
  UINT8  versionBuildDate[MAX_FWBUILDINFO_DESC + 1];
  UINT8  rhSide[RH_SIDE_INFO + 1];
}RhSwInfo;

typedef struct s_MsgDehPrmRhVersionInfo
{
  RhSwInfo  runningSideInfo;
  RhSwInfo  nonRunningSideInfo;
}MsgDehPrmRhVersionInfo;
#endif /* PRM_DEH_IF_H_ */
