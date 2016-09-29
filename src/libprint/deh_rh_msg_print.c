/**
 * @file    deh_rh_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file is used to print messages between DEH and RH
 *
 * @author     : Madhusudana Rao Kata
 * Date       : 01-11-2010
 * Description: 1. Basic version
 *
 */
#include "deh_rh_msg_print.h"

/*
 * @param    LkHdr : Header structure
 * @details  This function will log Radio Head header data.
 */
void printDehRhHeader (
  IN LkHdr      *hdr
  )
{
  logMsgInParams("hdr={dstMailbox=%d|srcMailbox=%d|msgId=%d}", hdr->dstMailbox, hdr->srcMailbox, hdr->msgId );
}

/*
 * @param    req : Pointer to BsLkInitializationAck structure
 * @param    LkHdr : Header structure
 * @details  This function fills BsLkInitializationAck structure.
 */
void printMsgDehRhInitializationAck(
  IN BsLkInitializationAck  *req,
  IO LkHdr                  *hdr
  )
{
  logMsgInParams("%s","Initialisation ACK sent to Radio head With following Parameters\n");
  logMsgInParams("%s","{\n");
  printDehRhHeader(hdr);
  logMsgInParams("  dataTickTime=%d\n  systemId=%d\n  alarmAck=%d\n", req->dataTickTime, req->systemId, req->alarmAck);
  logMsgInParams("%s","}\n");
}

/*
 * @param    BsLkResetReq : REQ structure input data
 * @param    LkHdr : Header structure
 * @details  This function will log Radio Head Reset Request
 */
void printMsgDehRhResetReq(
  IN BsLkResetReq   *req,
  IO LkHdr          *hdr
  )
{
  logMsgOut(" Msg %s sent to RH ", "MSG_BS_LK_RESET_REQ");
  logMsgOutParams("%s","{\n");
  printDehRhHeader(hdr);
  logMsgOutParams( "ies={varDummy=%d}}",req->varDummy );
}

/*
 * @param    req : Pointer to BsLkStatusInfoReq structure
 * @param    LkHdr : Header structure
 * @details  This function will log Radio Head Staus Request
 */
void printMsgDehRhStatusInfoReq(
  IN BsLkStatusInfoReq  *req,
  IO LkHdr              *hdr
 )
{
  logMsgInParams("%s","Status Request Message sent to Radio head With following Parameters\n");
  logMsgInParams("%s","{\n");
  printDehRhHeader(hdr);
  logMsgInParams( "  varSeqNum=%d\n  varReserved=%d\n",req->sequenceNo, req->varDummy );
  logMsgInParams("%s","}\n");
}


/*
 * @param    req : Pointer to BsLkDelaySettingReq structure
 * @param    LkHdr : Header structure
 * @details  This function logs Delay Settings Request Message
 */
void printMsgDehRhDelaySettingReq(
  IN BsLkDelaySettingReq  *req,
  IO LkHdr                *hdr
  )
{
  logMsgInParams("%s","Delay Setting Request Message sent to Radio head With following Parameters\n");
  logMsgInParams("%s","{\n");
  printDehRhHeader(hdr);
  logMsgInParams( "  varDummy=%d\n  sequenceNo=%d\n  dataDummy=%d\n  txDelay=%d\n  rxDelay=%d\n",req->varDummy , req->sequenceNo, req->dataDummy, req->txDelay , req->rxDelay );
  logMsgInParams("%s","}\n");
}

/*
 * @param    req : Pointer to BsLkTxOnOffReq structure
 * @param    LkHdr : Header structure
 * @details  This function logs TX On or Off Settings Request Message
 */
void printMsgDehRhTxOnOffReq(
  IN BsLkTxOnOffReq  *req,
  IO LkHdr           *hdr

  )
{
  logMsgInParams("%s","RH Tx On/Off Setting Request Message sent to Radio head With following Parameters\n");
  logMsgInParams("%s","{\n");
  printDehRhHeader(hdr);
  logMsgInParams( "  varDummy=%d\n  sequenceNo=%d\n  faId=%d\n  dataDummy1=%d\n  path=%d\n  txOnOrOff=%d\n  dataDummy2=%d\n  dataDummy3=%d\n",req->varDummy ,req->sequenceNo,req->faId,req->dataDummy1, req->path, req->txOnOrOff , req->dataDummy2, req->dataDummy3 );
  logMsgInParams("%s","}\n");
}

/*
 * @param    req : Pointer to BsLkTxOnOffReq structure
 * @param    LkHdr : Header structure
 * @details  This function logs TX Enable/Disable Settings Request Message
 */
void printMsgDehRhEnableReq(
  IN BsLkEnableOrDisableReq  *req,
  IO LkHdr                   *hdr
  )
{
  logMsgInParams("%s","Rh Enable/Disable Setting Request Message sent to Radio head With following Parameters\n");
  logMsgInParams("%s","{\n");
  printDehRhHeader(hdr);
  logMsgInParams( "  varDummy=%d\n  sequenceNo=%d\n  path=%d\n  control=%d\n  dataDummy=%d\n",req->varDummy , req->sequenceNo, req->path, req->control , req->dataDummy );
  logMsgInParams("%s","}\n");
}

/*
 * @param    req : Pointer to BsLkDataPathReq structure
 * @param    LkHdr : Header structure
 * @details  This function logs Data Path Request Message
 */
void printMsgDehRhDataPathReq(
  IN  BsLkDataPathReq   *req,
  IO LkHdr              *hdr
  )
{
  logMsgInParams("%s","Rh Data Path Setting Request Message sent to Radio head With following Parameters\n");
  logMsgInParams("%s","{\n");
  printDehRhHeader(hdr);
  logMsgInParams( "  varDummy=%d\n  iqMap[0]=%d\n  iqMap[1]=%d\n  dataDummy1=%d\n  dataDummy2=%d\n  dataDummy3=%d\n  numberOfFa=%d\n  dataDummy4=%d\n  mappingOfFa[0]=%d\n  mappingOfFa[1]=%d\n  dataDummy5=%d\n  dataDummy6=%d\n",req->varDummy , req->iqMap[0], req->iqMap[1] , req->dataDummy1 , req->dataDummy2, req->dataDummy3, req->numberOfFa, req->dataDummy4, req->mappingOfFa[0], req->mappingOfFa[1], req->dataDummy5, req->dataDummy6);
  logMsgInParams("%s","}\n");
}

/*
 * @param    req : Pointer to BsLkFaControlReq structure
 * @param    LkHdr : Header structure
 * @details  This function logs Fa Control Request Message
 */
void printMsgDehRhFaControlReq(
  IO BsLkFaControlReq   *req,
  IO LkHdr              *hdr
  )
{
  logMsgInParams("%s","Rh FA Control Setting Request Message sent to Radio head With following Parameters\n");
  logMsgInParams("%s","{\n");
  printDehRhHeader(hdr);
  logMsgInParams( "  varDummy=%d\n  sequenceNo=%d\n  faId=%d\n  dataDummy1=%d\n  dataDummy2=%d\n  dataDummy3=%d\n  txAttenPathA=%d\n  txAttenPathB=%d\n  dataDummy4=%d\n  dataDummy5=%d\n  txEarfcn=%d\n  rxEarfcn=%d\n  txPathAOnOff=%d\n  txPathBOnOff=%d\n  txPathCOnOff=%d\n  txPathDOnOff=%d\n  rxPathAOnOff=%d\n  rxPathBOnOff=%d\n  rxPathCOnOff=%d\n  rxPathDOnOff=%d\n  dataDummy6=%d\n  dataDummy7=%d\n  dataDummy8=%d\n  dataDummy9=%d\n  dataDummy10=%d\n  dataDummy11=%d\n  dataDummy12=%d\n", \
      req->varDummy , req->sequenceNo, req->faId,req->dataDummy1, req->dataDummy2, req->dataDummy3, req->txAttenPathA,req->txAttenPathB, \
      req->dataDummy4, req->dataDummy5, req->txEarfcn,req->rxEarfcn, req->txPathAOnOff , req->txPathBOnOff , req->txPathCOnOff ,req->txPathDOnOff, \
      req->rxPathAOnOff, req->rxPathBOnOff, req->rxPathCOnOff, req->rxPathDOnOff, req->dataDummy6, req->dataDummy7,req->dataDummy8, \
      req->dataDummy9, req->dataDummy10, req->dataDummy11, req->dataDummy12);
  logMsgInParams("%s","}\n");
}

/*
 * @param    req : Pointer to BsLkAlarmThresholdSettingReq structure
 * @param    LkHdr : Header structure
 * @details  This function logs Alarm Threshold Settings Request Message
 */
void printMsgDehRhAlarmThresholdSettingReq(
  IN BsLkAlarmThresholdSettingReq   *req,
  IO LkHdr                          *hdr
  )
{
  logMsgInParams("%s","Rh Alarm Threshold Setting Request Message sent to Radio head With following Parameters\n");
  logMsgInParams("%s","{\n");
  printDehRhHeader(hdr);
  logMsgInParams( "  dummy1=%d\n  sequenceNo=%d\n  dummy2=%d\n  vswrShutdownThreshold=%d\n  overPowerMinorThreshold=%d\n  lowPowerThreshold=%d\n",\
      req->dummy1, req->sequenceNo, req->dummy2 , req->vswrShutdownThreshold ,\
      req->overpowerAlarmThreshold, req->lowPowerThreshold);
  logMsgInParams("%s","}\n");
}

/*
 * @param    req : Pointer to BsLkHwInfoReq structure
 * @param    LkHdr : Header structure
 * @details  This function logs RH Hardware Information Request Message
 */
void printMsgDehRhHwInfoReq(
  IN BsLkHwInfoReq   *req,
  IO LkHdr           *hdr
  )
{
  logMsgInParams("%s","Hardware Info Request Message sent to Radio head With following Parameters\n");
  logMsgInParams("%s","{\n");
  printDehRhHeader(hdr);
  logMsgInParams( "  vardummy=%d\n",req->varDummy);
  logMsgInParams("%s","}\n");
}

/*
 * @param    req : Pointer to BsLkFwInfoReq structure
 * @param    LkHdr : Header structure
 * @details  This function logs RH Firmware Information Request Message
 */
void printMsgDehRhFwInfoReq(
  IN BsLkFwInfoReq   *req,
  IO LkHdr           *hdr
  )
{
  logMsgInParams("%s","Firmware Info Request Message sent to Radio head With following Parameters\n");
  logMsgInParams("%s","{\n");
  printDehRhHeader(hdr);
  logMsgInParams( "  vardummy=%d\n",req->varDummy);
  logMsgInParams("%s","}\n");
}

/*
 * @param    req : Pointer to BsLkOperationalModeInfoReq structure
 * @param    LkHdr : Header structure
 * @details  This function logs Operational Mode Information Request Message
 */
void printMsgDehRhOperatingModeInfoReq(
  IN BsLkOperationalModeInfoReq   *req,
  IO LkHdr           *hdr
  )
{
  logMsgInParams("%s","Operating Mode Info Request Message sent to Radio head for Health Check With following Parameters\n");
  logMsgInParams("%s","{\n");
  printDehRhHeader(hdr);
  logMsgInParams( "  vardummy=%d\n",req->varDummy);
  logMsgInParams("%s","}\n");
}

/*
 * @param    req : Pointer to BsLkSwInstallreq structure
 * @param    LkHdr : Header structure
 * @details  This function logs RH Software Installation Request Message
 */
void printMsgDehRhSwInstallReq(
  IN BsLkSwInstallreq   *req,
  IO LkHdr              *hdr
  )
{
  /* CR:21-May-2012: Change logMsgIn to logMsgOut */
  logMsgOut(" Msg %s sent to RH ", "MSG_LK_BS_SW_INSTALL_REQ");
  logMsgOutParams("%s","{");
  printDehRhHeader(hdr);
  logMsgOutParams("ies={vardummy=%u|sequenceno=%u|ipAddress=%x|fileName=%s|userName=%s|password=%s|port=%u}}",\
                 req->varDummy,req->sequenceNo,req->ipAddr,req->fileName,req->userName,req->password,req->port);
}

/*
 * @param    req : Pointer to BsLkSwActivateReq structure
 * @param    LkHdr : Header structure
 * @details  This function logs RH Software Activation Request Message
 */
void printMsgDehRhActivateReq(
  IN BsLkSwActivateReq   *req,
  IO LkHdr               *hdr
  )
{
  logMsgOut(" Msg %s sent to RH ", "MSG_LK_BS_SET_ACTIVE_SIDE_REQ");
  logMsgOutParams("%s","{\n");
  printDehRhHeader(hdr);
  logMsgOutParams( "ies={vardummy=%u|sequenceno=%u|activeSide=%u}}",req->varDummy,req->sequenceNo,req->activeSide);
}
