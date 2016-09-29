/**
 * @file    bsm_fth_if.h
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief   This file contains the messages that are exchanged between BSM and FTH
 * @author  Lakshmi Narayana Madala
 *
 * @details FTH : FILE TRANSFER HANDLER
 */

#ifndef BSM_FTH_IF_H
#define BSM_FTH_IF_H

#include "cmn_types.h"
#include "msg_ids.h"
#include "imc_msg_desc.h"
#include "log.h"
#include "bs_tables.h"
#include "tlv_enc_dec.h"

typedef struct s_FthCfgTable
{
  String256    downloadDir;
  IntervalSec  connTimeOutInterval;
  IntervalMin  downloadTimeOutInterval;
  IntervalMin  uploadTimeOutInterval;
  UINT32       maxFileSize;
  UINT32       maxDownloadDirSize;
} FthCfgTable;

/** @addtogroup if_group
 *  @{
 */

/** @name BSM_FTH_STRUCTURES
 *  @{
 */

/* Upload Messages */

typedef SetObjReq        MsgBsmFthSetObjReq;
typedef SetObjRspSucc    MsgFthBsmSetObjSucc;
typedef SetObjRspFail    MsgFthBsmSetObjFail;
typedef GetObjReq        MsgBsmFthGetObjReq;
typedef GetObjRspSucc    MsgFthBsmGetObjSucc;

typedef struct s_MsgFthBsmUploadFail
{
  UINT8          fileName[MAX_SIZE_FILEUPLOAD_FILENAME + 1 ];
  BsErrorValue   bsErrorValue;
} MsgFthBsmUploadFail;

typedef struct s_MsgFthBsmUploadCmpInd
{
  UINT8   fileName[MAX_SIZE_FILEUPLOAD_FILENAME + 1 ];
  TBOOL   deleteAfterUpload;
} MsgFthBsmUploadCmpInd;

typedef struct  s_MsgFthBsmDownloadFail
{
  UINT8          fileName[MAX_SIZE_FILEDOWNLOAD_FILENAME + 1 ];
  BsErrorValue   bsErrorValue;
} MsgFthBsmDownloadFail;

typedef struct  s_MsgFthBsmDownloadCmpInd
{
  UINT8   fileName[MAX_SIZE_FILEDOWNLOAD_FILENAME + 1 ];
} MsgFthBsmDownloadCmpInd;

/* Others */
typedef MsgTrace MsgBsmFthSetMsgTrace;

typedef SetLogConfig MsgBsmFthSetLogCfg;

/** @}
 */

/** @}
 */

#endif /* BSM_FTH_IF_H */
