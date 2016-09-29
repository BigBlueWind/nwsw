/**
 * @file    bsm_fth_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between BSM and FTH
 * @author :  Lakshmi Narayana Madala
 * Date   :
 * Change History:
 *
 */

#include "bsm_fth_if.h"
#include "print_tlv.h"

void printMsgFthBsmSwDownloadFail(
  void    *data,
  UINT16  len
  )
{
  MsgFthBsmDownloadFail  *msg = (MsgFthBsmDownloadFail*) data;
  logMsgParams( "{\n BsErrorValue=%s\n FileName=%s\n} Total length = %d\n", errorCodeStr(msg->bsErrorValue), msg->fileName, len );
}

void printMsgFthBsmSwDownloadCmpInd(
  void    *data,
  UINT16  len
  )
{
  MsgFthBsmDownloadCmpInd  *msg = (MsgFthBsmDownloadCmpInd*) data;
  logMsgParams( "{\n FileName=%s\n} Total length = %d\n", msg->fileName, len );
}

void printMsgFthBsmSwUploadCmpInd(
  void    *data,
  UINT16  len
  )
{
  MsgFthBsmUploadCmpInd  *msg = (MsgFthBsmUploadCmpInd*) data;
  logMsgParams( "{\n FileName=%s\n deleteAfterUpload=%d\n} Total length = %d\n", msg->fileName, msg->deleteAfterUpload, len );
}

void printMsgFthBsmSwUploadFail(
  void    *data,
  UINT16  len
  )
{
  MsgFthBsmUploadFail  *msg = (MsgFthBsmUploadFail*) data;
  logMsgParams( "{\n BsErrorValue=%s\n FileName=%s\n} Total length = %d\n", errorCodeStr(msg->bsErrorValue), msg->fileName, len);
}
