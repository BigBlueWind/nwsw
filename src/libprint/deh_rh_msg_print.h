/**
 * @file    deh_rh_msg_print.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains all function definitions used to print Messages between DEH and Radio Head
 *
 * @author     : mkata
 * Date       : 10 - 05 - 2010
 * Description:
 */

#ifndef PRINT_DEH_RH_H
#define PRINT_DEH_RH_H

#include "cmn_types.h"
#include "rh_deh_if.h"

/*
 * @param    LkHdr : Header structure
 * @details  This function will log Radio Head header data.
 */
void printDehRhHeader (
  IN LkHdr      *hdr
  );

/*
 * @param    req : Pointer to BsLkInitializationAck structure
 * @param    LkHdr : Header structure
 * @details  This function fills BsLkInitializationAck structure.
 */
void printMsgDehRhInitializationAck(
  IN BsLkInitializationAck  *req,
  IO LkHdr                  *hdr
  );

/*
 * @param    BsLkResetReq : REQ structure input data
 * @param    LkHdr : Header structure
 * @details  This function will log Radio Head Reset Request
 */
void printMsgDehRhResetReq(
  IN BsLkResetReq   *req,
  IO LkHdr          *hdr
  );

/*
 * @param    req : Pointer to BsLkStatusInfoReq structure
 * @param    LkHdr : Header structure
 * @details  This function will log Radio Head Status Request
 */
void printMsgDehRhStatusInfoReq(
  IN BsLkStatusInfoReq  *req,
  IO LkHdr              *hdr
 );


/*
 * @param    req : Pointer to BsLkDelaySettingReq structure
 * @param    LkHdr : Header structure
 * @details  This function logs Delay Settings Request Message
 */
void printMsgDehRhDelaySettingReq(
  IN BsLkDelaySettingReq  *req,
  IO LkHdr                *hdr
  );

/*
 * @param    req : Pointer to BsLkTxOnOffReq structure
 * @param    LkHdr : Header structure
 * @details  This function logs TX On or Off Settings Request Message
 */
void printMsgDehRhTxOnOffReq(
  IN BsLkTxOnOffReq  *req,
  IO LkHdr           *hdr
  );

/*
 * @param    req : Pointer to BsLkTxOnOffReq structure
 * @param    LkHdr : Header structure
 * @details  This function logs TX Enable/Disable Settings Request Message
 */
void printMsgDehRhEnableReq(
  IN BsLkEnableOrDisableReq  *req,
  IO LkHdr                   *hdr
  );

/*
 * @param    req : Pointer to BsLkDataPathReq structure
 * @param    LkHdr : Header structure
 * @details  This function logs Data Path Request Message
 */
void printMsgDehRhDataPathReq(
  IN  BsLkDataPathReq   *req,
  IO LkHdr              *hdr
  );

/*
 * @param    req : Pointer to BsLkFaControlReq structure
 * @param    LkHdr : Header structure
 * @details  This function logs Fa Control Request Message
 */
void printMsgDehRhFaControlReq(
  IO BsLkFaControlReq   *req,
  IO LkHdr              *hdr
  );

/*
 * @param    req : Pointer to BsLkFaControlReq structure
 * @param    LkHdr : Header structure
 * @details  This function logs Alarm Threshold Settings Request Message
 */
void printMsgDehRhAlarmThresholdSettingReq(
  IN BsLkAlarmThresholdSettingReq   *req,
  IO LkHdr                          *hdr
  );

void printMsgDehRhHwInfoReq(
  IN BsLkHwInfoReq   *req,
  IO LkHdr           *hdr
  );

void printMsgDehRhFwInfoReq(
  IN BsLkFwInfoReq   *req,
  IO LkHdr           *hdr
  );

void printMsgDehRhOperatingModeInfoReq(
  IN BsLkOperationalModeInfoReq   *req,
  IO LkHdr           *hdr
  );
void printMsgDehRhSwInstallReq(
  IN BsLkSwInstallreq   *req,
  IO LkHdr              *hdr
  );
void printMsgDehRhActivateReq(
  IN BsLkSwActivateReq   *req,
  IO LkHdr              *hdr
  );
#endif /* PRINT_DEH_RH_H */
