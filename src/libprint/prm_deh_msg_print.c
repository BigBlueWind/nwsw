/**
 * @file    prm_deh_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between PRM and DEH
 * @author : Ankur Agrawal
 * Date   : 26-Apr-2012
 * Change History:
 *
 */

#include "prm_deh_msg_print.h"

void printMsgPrmDehRhSwInstallReq(
  void    *data,
  UINT16  len
  )
{
  MsgPrmDehRhSwInstallReq *msg = (MsgPrmDehRhSwInstallReq*) data;

  logMsgParams( "{\n fileName=%s\n", msg->filename);
  logMsgParams("} Total length = %d\n", len );
}


void printMsgDehPrmRhSwInstallFail(
  void    *data,
  UINT16  len
  )
{
  MsgDehPrmRhSwInstallFail *msg = (MsgDehPrmRhSwInstallFail*) data;

  logMsgParams( "{\n bsErrorValue=%s\n failReason=%s\n",\
               errorCodeStr(msg->errorDesc.errorNum), msg->errorDesc.failReason );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgDehPrmRhSwInstallSucc(
  void    *data,
  UINT16  len
  )
{
  MsgDehPrmRhSwInstallSucc *msg = (MsgDehPrmRhSwInstallSucc*) data;

  logMsgParams( "{\nversion=%s\n", msg->version );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgPrmDehRhSwActivateReq(
  void    *data,
  UINT16  len
  )
{
  MsgPrmDehRhSwActivateReq *msg = (MsgPrmDehRhSwActivateReq*) data;

  logMsgParams( "{\n version=%s\n", msg->version);
  logMsgParams("} Total length = %d\n", len );
}


void printMsgDehPrmRhSwActivateFail(
  void    *data,
  UINT16  len
  )
{
  MsgDehPrmRhSwActivateFail *msg = (MsgDehPrmRhSwActivateFail*) data;

  logMsgParams( "{\n version=%s\n bsErrorValue=%s\n failReason=%s\n",\
               msg->version, errorCodeStr(msg->errorDesc.errorNum), msg->errorDesc.failReason );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgDehPrmRhVersionInfo(
  void    *data,
  UINT16  len
  )
{
  MsgDehPrmRhVersionInfo *msg = (MsgDehPrmRhVersionInfo*) data;

  logMsgParams( "{\n runningVersion=%s\n nonRunningVersion=%s\n runningVersionBuildDate=%s\n nonRunningVersionBuildDate=%s\n"
		  " runningSide=%s\n nonRunningSide=%s\n",\
               msg->runningSideInfo.version, msg->nonRunningSideInfo.version, msg->runningSideInfo.versionBuildDate,\
               msg->nonRunningSideInfo.versionBuildDate,msg->runningSideInfo.rhSide,msg->nonRunningSideInfo.rhSide );
  logMsgParams("} Total length = %d\n", len );
}


void printMsgDehPrmRhSwActivateSucc(
  void    *data,
  UINT16  len
  )
{
  MsgDehPrmRhSwActivateSucc *msg = (MsgDehPrmRhSwActivateSucc*) data;

  logMsgParams( "{\n version=%s\n", msg->version );
  logMsgParams("} Total length = %d\n", len );
}

