/**
 * @file    bsm_prm_if.h
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief This file contains the messages between BSM and PRM.
 * @date  08Sep2010 Added MsgPrmBsmModUpInd, MsgPrmBsmModDownInd  -->  Manu Sharma
 * @date  22Nov2010 Added MsgBsmPrmSwActivateReq message for Software Activation --> Shweta
 * @date  25Jan2010 Added MsgPrmBsmConfigInstallReq message for config installation --> Shweta
 */

#ifndef BSM_PRM_IF_H
#define BSM_PRM_IF_H

#include "cmn_types.h"
#include "tlv_enc_dec.h"
#include "bs_events.h"
#include "bs_alarms.h"


typedef UINT32    ProcessId;

/** @addtogroup if_group
 *  @{
 */

/** @name BSM_PRM_MESSAGES
 *  @{
 */

typedef struct  s_MsgBsmPrmUpdateTimerReq
{
  TimerName   timerName;
  TimeUnit    timeUnit;
  UINT32      duration;
} MsgBsmPrmUpdateTimerReq;

/* MESSAGES */

typedef struct s_MsgPrmBsmAllModulesUpInd
{
  UINT8      numModUp;
  ModuleId   moduleId[MODID_MAX];
} MsgPrmBsmAllModulesUpInd;

typedef struct s_MsgPrmBsmModUpInd
{
  ModuleId      moduleId;
  ErrorDesc     errorDescForPrevModDown;
} MsgPrmBsmModUpInd;

typedef struct s_MsgPrmBsmModDownInd
{
  ErrorDesc     errorDesc;
  ModuleId      moduleId;
} MsgPrmBsmModDownInd;

/* MsgBsmPrmStartInd has no IEs */
/* MsgPrmBsmHello has no IEs */
/* MsgBsmPrmHelloAck has no IEs */
/* MsgBsmPrmRebootCmd has no IEs */
/* MsgBsmPrmRestartCmd has no IEs */
/* MsgBsmPrmShutdownCmd has no IEs */

typedef SetObjReq                MsgBsmPrmSwActivateReq;

typedef SetObjRspSucc            MsgPrmBsmSwActivateRsp ;

typedef SetObjRspFail            MsgPrmBsmSwActivateRej ;

typedef struct s_MsgPrmBsmSwActivateSucc
{
  SwType   swType;
  UINT8    version[MAX_SIZE_SWACTIVATIONSTATUS_VERSION];
} MsgPrmBsmSwActivateSucc;

typedef struct s_MsgPrmBsmSwActivateFail
{
  ErrorDesc    errorDesc;
  SwType       swType;
  UINT8        version[MAX_SIZE_SWACTIVATIONSTATUS_VERSION];
} MsgPrmBsmSwActivateFail;

typedef SetObjReq	         MsgBsmPrmSwInstallReq ;

typedef SetObjRspSucc        MsgPrmBsmSwInstallRsp ;

typedef SetObjRspFail        MsgPrmBsmSwInstallRej ;

typedef SetObjReq		     MsgBsmPrmSetObjReq;


typedef struct s_MsgPrmBsmSwInstallSucc
{
  UINT8         filename[MAX_SIZE_SWINSTALL_FILENAME + 1 ];
  UINT8         version[MAX_SIZE_SWACTIVATIONSTATUS_VERSION+1];
} MsgPrmBsmSwInstallSucc;

typedef struct s_MsgPrmBsmSwInstallFail
{
  BsErrorValue  error;
  UINT8         filename[MAX_SIZE_SWINSTALL_FILENAME + 1 ];
} MsgPrmBsmSwInstallFail;


typedef struct s_MsgPrmBsmConfigInstallReq
{
  CmdRef       cmdRef;
  UINT8        configFilePath[MAX_SIZE_SWINSTALL_FILENAME + 1];  /* Path where config file is placed */
  UINT8        filename[MAX_SIZE_SWINSTALL_FILENAME + 1 ];       /* Name of the Config file */
} MsgPrmBsmConfigInstallReq;

typedef struct s_MsgBsmPrmConfigInstallSucc
{
  UINT8        configFilePath[MAX_SIZE_SWINSTALL_FILENAME + 1];  /* Path where config file is placed */
  UINT8        filename[MAX_SIZE_SWINSTALL_FILENAME + 1 ];       /* Name of the Config file */
} MsgBsmPrmConfigInstallSucc;

typedef struct s_MsgBsmPrmConfigInstallFail
{
  ErrorDesc    errorDesc;
  UINT8        configFilePath[MAX_SIZE_SWINSTALL_FILENAME + 1];  /* Path where config file is placed */
  UINT8        filename[MAX_SIZE_SWINSTALL_FILENAME + 1 ];       /* Name of the Config file */
} MsgBsmPrmConfigInstallFail;

typedef SetLogConfig MsgBsmPrmSetLogCfg;
/** @}
 */

/** @}
 */

#endif /* BSM_PRM_IF_H */
