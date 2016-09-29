/**
 * @file    bsm_prm_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between BSM and PRM
 * @author :  Ramprasad Reddy Chinta
 * Date   :
 * Change History:
 *
 */

#include "bsm_prm_msg_print.h"

void printMsgPrmBsmAllModulesUpInd(
    void    *data,
    UINT16  len
    )
{
  MsgPrmBsmAllModulesUpInd *msg = (MsgPrmBsmAllModulesUpInd*) data;
  UINT8                     index = 0;

  logMsgParams("%s","{\n");

  for(index=0;index<msg->numModUp;index++)
    logMsgParams( " %d moduleName=%s\n", index+1, getModuleName(msg->moduleId[index]) );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgModUpInd(
    void    *data,
    UINT16  len
    )
{
  MsgPrmBsmModUpInd *msg = (MsgPrmBsmModUpInd*) data;
  logMsgParams("%s","{\n");
  logMsgParams( " moduleName=%s\n bsError=%s\n bsErrorString=%s\n", getModuleName(msg->moduleId),
                                                                    errorCodeStr(msg->errorDescForPrevModDown.errorNum),
                                                                    msg->errorDescForPrevModDown.failReason );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgModDownInd(
    void    *data,
    UINT16  len
    )
{
  MsgPrmBsmModDownInd *msg = (MsgPrmBsmModDownInd*) data;
  logMsgParams("%s","{\n");
  logMsgParams( " moduleName=%s\n bsError=%s\n bsErrorString=%s\n", getModuleName(msg->moduleId), errorCodeStr(msg->errorDesc.errorNum), msg->errorDesc.failReason );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgPrmBsmSwInstallCmpl(
  void    *data,
  UINT16  len
  )
{
  MsgPrmBsmSwInstallSucc *msg = (MsgPrmBsmSwInstallSucc*) data;
  logMsgParams("{\nversion=%s\n fileName=%s\n", msg->version, msg->filename );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgPrmBsmSwInstallFail(
  void    *data,
  UINT16  len
  )
{
  MsgPrmBsmSwInstallFail *msg = (MsgPrmBsmSwInstallFail*) data;

  logMsgParams( "{\n error=%s\n fileName=%s\n", errorCodeStr(msg->error), msg->filename );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgPrmBsmConfigInstallReq(
  void    *data,
  UINT16  len
  )
{
  MsgPrmBsmConfigInstallReq *msg = (MsgPrmBsmConfigInstallReq*) data;

  logMsgParams( "{\n filePath=%s\n fileName=%s\n", msg->configFilePath, msg->filename );
  logMsgParams("} Total length = %d\n", len );
}

void PrintMsgBsmPrmConfigInstallSucc(
  void    *data,
  UINT16  len
  )
{
  MsgBsmPrmConfigInstallSucc *msg = (MsgBsmPrmConfigInstallSucc*) data;

  logMsgParams( "{\n filePath=%s\n fileName=%s\n", msg->configFilePath, msg->filename );
  logMsgParams("} Total length = %d\n", len );
}

void PrintMsgBsmPrmConfigInstallFail(
  void    *data,
  UINT16  len
  )
{
  MsgBsmPrmConfigInstallFail *msg = (MsgBsmPrmConfigInstallFail*) data;

  logMsgParams( "{\n filePath=%s\n fileName=%s\n bsErrorValue=%s\n failReason=%s\n",\
               msg->configFilePath, msg->filename, errorCodeStr(msg->errorDesc.errorNum), msg->errorDesc.failReason );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgPrmBrmSwActSucc(
    void    *data,
    UINT16  len
    )
{
  MsgPrmBsmSwActivateSucc *msg = (MsgPrmBsmSwActivateSucc*) data;

  logMsgParams( "{\n version=%s", msg->version );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgPrmBrmSwActFail(
    void    *data,
    UINT16  len
    )
{
  MsgPrmBsmSwActivateFail *msg = (MsgPrmBsmSwActivateFail*) data;

  logMsgParams( "{\n version=%s", msg->version );
  logMsgParams( "\n errorNum=%s", errorCodeStr(msg->errorDesc.errorNum) );
  logMsgParams( "\n failReason=%s", msg->errorDesc.failReason );
  logMsgParams("} Total length = %d\n", len );
}
