/**
 * @file    deh_rh_func.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains LK-BS customer interface test cases.
 *
 * @author    mkata, kgummad, aagrawal, spolepally
 * @date      24-05-2011
 * @details   Basic version
 *
 */

/* Include files */
#include "deh.h"
#include "deh_rh_func.h"
#include "deh_rh_fillFunc.h"
#include "rh_enc.h"
#include "rh_dec.h"


/* Global Variable LK Address */
//UINT8   *lkIpAddr;
SINT32  sockFd;

/**
 * @details  This function receives data from the socket and store it in the
 *           buffer for processing.
 * @param    expMsgId : Expected Msg ID
 * @param    decBuf   : Buffer where received data will be stored for decoding.
 */
void expectLkMsg(
  IN UINT32      expMsgId,
  IO DecBuf      *decBuf
  )
{
  MailBox      dstMailbox;
  MailBox      srcMailbox;
  MsgId        msgId;
  SINT16       numBytesRcvd=0;
  UINT8        recvBuf[SIZE_RECV_DATA];

  while(1)
  {
    numBytesRcvd = recv(sockFd, &recvBuf, SIZE_RECV_DATA, SOCK_FLAGS);
    if ( numBytesRcvd <= 0 )
    {
      continue;
    }
    else
    {
      memset( decBuf, 0, sizeof(DecBuf) );
      decBuf->len   = numBytesRcvd;
      decBuf->value = &recvBuf[0];     
    }

    dstMailbox = getUint16( decBuf );
    srcMailbox = getUint16( decBuf );

    msgId      = getUint32( decBuf );

    if ( dstMailbox NOTEQ BS_MAILBOX_ID )
    {
      logError(" Destination Mail Box was expected to be = %d, received = %d",
                                      BS_MAILBOX_ID, dstMailbox  );
    }
    if ( msgId NOTEQ expMsgId )
    {
      logError(" Expected expMsgId = %d, received msgId= %d",expMsgId, msgId  );
    }
    else
    {
      break;
    }
  }
}

/**
 * @details  This function receives data from the socket and store it in the
 *           buffer for processing.
 * @param    expMsgId : Expected Msg ID
 * @param    decBuf   : Buffer where received data will be stored for decoding.
 */
TBOOL waitForRhRspMsg(
  IN UINT32      expMsgId,
  IO DecBuf      *decBuf
  )
{
  MailBox      dstMailbox;
  MailBox      srcMailbox;
  MsgId        msgId;
  SINT16       numBytesRcvd=0;
  UINT8        recvBuf[SIZE_RECV_DATA];
  DehContext   *dehContext = getDehContext();

  while(dehContext->isRhRspTimerRunning)
  {
    numBytesRcvd = recv(sockFd, &recvBuf, SIZE_RECV_DATA, SOCK_FLAGS);
    if ( numBytesRcvd <= 0 )
    {
      continue;
    }
    else
    {
      memset( decBuf, 0, sizeof(DecBuf) );
      decBuf->len   = numBytesRcvd;
      decBuf->value = &recvBuf[0];
      dstMailbox = getUint16( decBuf );
      srcMailbox = getUint16( decBuf );
      msgId      = getUint32( decBuf );

      if ( dstMailbox NOTEQ BS_MAILBOX_ID )
      {
        logError(" Destination Mail Box was expected to be = %d, received = %d",
                                          BS_MAILBOX_ID, dstMailbox  );
      }
      if ( msgId NOTEQ expMsgId )
      {
        logError(" Expected expMsgId = %d, received msgId= %d",expMsgId, msgId  );
      }
      /* If received Message is Expected Message Then Make isRhRspReceived to TRUE */
      dehContext->isRhRspReceived  = TRUE;

      break;
    }
  }  /* End Of While Loop */
  /**
   * @todo Delayed Response indication to user
   */
  if(dehContext->isRhRspReceived  EQ FALSE)
  {
    logError(" \nTimer Expired : Response for  Message %d from Radio Head is not yet received...\n ",expMsgId);
    return FALSE;
  }
  return TRUE;
}

/**
 * @details  This function will send data on UDP socket to LK.
 * @param    EncBuf  Data Buffer to be sent on the socket
 */
TBOOL sendMsgToRh(
  IN EncBuf *encBuf
  )
{
  SINT32   numBytesSent=0;
  UINT8    *lkIpAddr = NULL;
  SINT32   *rhSockFd = getSocketFd();
  lkIpAddr = getRhIpAddress();
  

  /* Send Message to LK using UDP socket */
  numBytesSent = sendUdpMsg( *rhSockFd, lkIpAddr,LK_MAILBOX_ID, encBuf->value, encBuf->len);

  /**TODO: CR: 21-May-2012: Added the logging of messages here.
          For outgoing messages: printRhPdu(OUT, encBuf);
          For incoming messages: printRhPdu(IN, decBuf); 

  */

  /**
   * @todo Need to check this condition
   */
  if(numBytesSent EQ -1 )
  {
    return FALSE;
  }
  return TRUE;
}

/**
 * @details  This function will send data on UDP socket to LK.
 * @param    EncBuf  Data Buffer to be sent on the socket
 */
TBOOL sendNotiAckMsgToRh(
  IN EncBuf *encBuf
  )
{
  SINT32   numBytesSent=0;
  UINT8    *lkIpAddr = NULL;
  SINT32   *rhSockFdNoti = getSocketFdNoti();
  lkIpAddr = getRhIpAddress();
  //lkIpAddr = LK_IP_ADDR;
  logInfo("LK IP ADDRESS is = %s",lkIpAddr);

  /* Send Message to LK using UDP socket */
  numBytesSent = sendUdpMsg( *rhSockFdNoti, lkIpAddr,LK_MAILBOX_ID, encBuf->value, encBuf->len);
  /**
   * @todo Need to check this condition
   */
  if(numBytesSent EQ -1 )
  {
    return FALSE;
  }
  return TRUE;
}

/**
 * @details  This function will carry out the Alarm Info procedure.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkAlarmInfoReq
 * @param    rspMsg : LkBsAlarmInfoRsp
 */
TBOOL dehRhAlarmInfo(
  IO LkAlarmTable   *rspMsg
  )
{
  EncBuf             encBuf;
  DecBuf             decBuf;
  LkHdr              hdr;
  BsLkAlarmInfoReq   req;
  LkBsAlarmInfoRsp   lkEncodedRsp;
  TBOOL              result;

  UNUSED(rspMsg);
  clearBytes(&lkEncodedRsp , sizeof(LkBsAlarmInfoRsp));
  clearBytes(&hdr , sizeof(LkHdr));

  fillHdr( MSG_BS_LK_ALARM_INFO_REQ, &hdr );
  /**
   * @todo Sequence Number needs to be handled
   */
  UINT32 seqNum = 0;
  fillDehRhAlarmInfoReq(seqNum, &req );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhAlarmInfoReq( &req, &encBuf );

  sendMsgToRh( &encBuf);

  expectLkMsg( MSG_LK_BS_ALARM_INFO_RSP, &decBuf);

  result = decRhDehAlarmInfoRsp( &decBuf, &lkEncodedRsp );
  /**
   * @todo Fill actual Buffer to be sent to CIH/SIH/WIH -- This may change to TLV format
   */

 // fillAlarmInfoRsp(&lkEncodedRsp,rspMsg);
  return result;
}

/**
 * @details  This function will carry out the reset procedure.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkResetReq
 * @param    rspMsg : LkBsResetRsp
 */
TBOOL dehSendRhResetReq()
{
  EncBuf             encBuf;
  LkHdr              hdr;
  BsLkResetReq       req;
  TBOOL              result;

  fillHdr( MSG_BS_LK_RESET_REQ, &hdr );
  fillDehRhResetReq( &req );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhResetReq( &req, &encBuf );
  result = sendMsgToRh( &encBuf );
  printMsgDehRhResetReq(&req, &hdr );
  return result;
}

/**
 * @details  This function will carry out the Data Path procedure.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkDataPathReq
 * @param    rspMsg : LkBsDataPathRsp
 */
TBOOL dehRhSetDataPath(
  RhDefaultCfg *rhConfig
)
{
  EncBuf             encBuf;
  LkHdr              hdr;
  BsLkDataPathReq    req;
  TBOOL              isMsgSentToRh = FALSE;

  clearBytes(&req, sizeof(BsLkDataPathReq));
  clearBytes(&hdr, sizeof(LkHdr));
  initEncBuf( &encBuf );

  fillHdr( MSG_BS_LK_DATA_PATH_REQ, &hdr );
  fillDehRhDataPathReq(&req, rhConfig );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhDataPathReq( &req, &encBuf );

  isMsgSentToRh = sendMsgToRh( &encBuf);
  logMsgOut(" Msg %s sent to RH ", "MSG_LK_BS_DATA_PATH_REQ");
  /* Log Message sent to RH */
  printMsgDehRhDataPathReq(&req, &hdr );
  return isMsgSentToRh;
}

/**
 * @details  This function will carry out the FA1 control procedure.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkFaControlReq
 * @param    rspMsg : LkBsFaControlRsp
 */
TBOOL dehRhFaControl(
  RhDefaultCfg *rhConfig
)
{
  EncBuf             encBuf;
  LkHdr              hdr;
  BsLkFaControlReq   req;
  TBOOL result       = FALSE;

  clearBytes(&req , sizeof(BsLkFaControlReq));
  clearBytes(&hdr, sizeof(LkHdr));
  initEncBuf( &encBuf );

  fillHdr( MSG_BS_LK_FA_CONTROL_REQ, &hdr );
  fillDehRhFaControlReq( &req, rhConfig );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhFaControlReq( &req, &encBuf );

  result = sendMsgToRh( &encBuf);
  logMsgOut(" Msg %s sent to RH ", "MSG_BS_LK_FA_CONTROL_REQ");
  /* Log Message sent to RH */
  printMsgDehRhFaControlReq(&req, &hdr );
  return result;
}

/**
 * @details  This function will carry out the  Set Alarm Threshold procedure.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkAlarmThresholdSettingReq
 * @param    rspMsg : RhDehAlarmThresholdSettingRsp
 */
TBOOL dehRhAlarmThresholdSet(
  RhDefaultCfg *rhConfig
)
{
  EncBuf             encBuf;
  LkHdr              hdr;
  BsLkAlarmThresholdSettingReq    req;
  TBOOL result = FALSE;

  clearBytes(&req, sizeof(BsLkAlarmThresholdSettingReq));
  clearBytes(&hdr, sizeof(LkHdr));
  initEncBuf( &encBuf );

  fillHdr( MSG_BS_LK_ALARM_THRESHOLD_SETTING_REQ, &hdr );
  fillDehRhAlarmThresholdSettingReq(&req, rhConfig );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhAlarmThresholdSettingReq( &req, &encBuf );

  result = sendMsgToRh( &encBuf);
  logMsgOut(" Msg %s sent to RH ", "MSG_BS_LK_ALARM_THRESHOLD_SETTING_REQ");
  /* Log Message sent to RH */
  printMsgDehRhAlarmThresholdSettingReq(&req, &hdr );
  return result;
}

/**
 * @details  This function will carry out the Set Delay procedure.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkDelaySettingReq
 * @param    rspMsg : LkBsDelaySettingRsp
 */
TBOOL dehRhSetDelay(
  RhDefaultCfg *rhConfig
)
{
  EncBuf                 encBuf;
  LkHdr                  hdr;
  BsLkDelaySettingReq    req;
  TBOOL                  result = FALSE;

  clearBytes(&req, sizeof(BsLkDelaySettingReq));
  clearBytes(&hdr, sizeof(LkHdr));
  initEncBuf( &encBuf );

  fillHdr( MSG_BS_LK_DELAY_SETTING_REQ, &hdr );
  fillDehRhDelaySettingReq( &req, rhConfig );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhDelaySettingReq( &req, &encBuf );
  result = sendMsgToRh( &encBuf);
  logMsgOut(" Msg %s sent to RH ", "MSG_BS_LK_DELAY_SETTING_REQ");
  /* Log Message sent to RH */
  printMsgDehRhDelaySettingReq(&req, &hdr );
  return result;
}

/**
 * @details  This function will carry out the Set Tx On/Off procedure for Path A/B.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkTxOnOffReq
 * @param    rspMsg : LkBsTxOnOffRsp
 */
TBOOL dehRhSetTxOnOff(
  IO RhPath      path,
  IO OnOff       txOnOff
  )
{
  EncBuf                 encBuf;
  LkHdr                  hdr;
  BsLkTxOnOffReq         req;
  TBOOL                  result;

  clearBytes(&req, sizeof(BsLkTxOnOffReq));
  clearBytes(&hdr, sizeof(LkHdr));
  initEncBuf( &encBuf );

  fillHdr( MSG_BS_LK_TX_ON_OFF_SET_REQ, &hdr );
  fillDehRhTxOnOffReq( &req, &path, &txOnOff );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhTxOnOffReq( &req, &encBuf );

  result = sendMsgToRh( &encBuf);
  logMsgOut(" Msg %s sent to RH ", "MSG_BS_LK_TX_ON_OFF_SET_REQ");
  /* Log Message sent to RH */
  printMsgDehRhTxOnOffReq(&req, &hdr );
  return result;
}

/**
 * @details  This function will carry out the Enable procedure for Path A.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkEnableOrDisableReq
 * @param    rspMsg : LkBsEnableOrDisableRsp
 */
TBOOL dehRhEnablePathA()
{
  EncBuf                   encBuf;
  LkHdr                    hdr;
  BsLkEnableOrDisableReq   req;
  TBOOL                    result = FALSE;

  clearBytes(&req, sizeof(BsLkEnableOrDisableReq));
  clearBytes(&hdr, sizeof(LkHdr));
  initEncBuf(&encBuf);

  fillHdr( MSG_BS_LK_ENABLE_OR_DISABLE_REQ, &hdr );
  fillDehRhEnableReqPathA( &req );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhEnableOrDisableReq( &req, &encBuf );

  result = sendMsgToRh( &encBuf);
  logMsgOut(" Msg %s sent to RH ", "MSG_BS_LK_ENABLE_OR_DISABLE_REQ");
  /* Log Message sent to RH */
  printMsgDehRhEnableReq(&req, &hdr );
  return result;
}

/**
 * @details  This function will carry out the Enable procedure for Path B.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkEnableOrDisableReq
 * @param    rspMsg : LkBsEnableOrDisableRsp
 */
TBOOL dehRhEnablePathB()
{
  EncBuf                    encBuf;
  LkHdr                     hdr;
  BsLkEnableOrDisableReq    req;
  TBOOL                     result = FALSE;

  clearBytes(&req, sizeof(BsLkEnableOrDisableReq));
  clearBytes(&hdr, sizeof(LkHdr));
  initEncBuf(&encBuf);

  fillHdr( MSG_BS_LK_ENABLE_OR_DISABLE_REQ, &hdr );
  fillDehRhEnableReqPathB( &req );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhEnableOrDisableReq( &req, &encBuf );

  result = sendMsgToRh( &encBuf);
  logMsgOut(" Msg %s sent to RH ", "MSG_BS_LK_ENABLE_OR_DISABLE_REQ");
  /* Log Message sent to RH */
  printMsgDehRhEnableReq(&req, &hdr );
  return result;
}

/**
 * @details  This function will carry out the Diable procedure for Path A.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkEnableOrDisableReq
 * @param    rspMsg : LkBsEnableOrDisableRsp
 */
TBOOL dehRhDisablePathA()
{
  EncBuf                   encBuf;
  LkHdr                    hdr;
  BsLkEnableOrDisableReq   req;
  TBOOL                    result = FALSE;

  fillHdr( MSG_BS_LK_ENABLE_OR_DISABLE_REQ, &hdr );
  fillDehRhDisableReqPathA( &req );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhEnableOrDisableReq( &req, &encBuf );

  result = sendMsgToRh( &encBuf );
  logMsgOut(" Msg %s sent to RH ", "MSG_BS_LK_ENABLE_OR_DISABLE_REQ");
  /* Log Message sent to RH */
  printMsgDehRhEnableReq(&req, &hdr );
  return result;
}

/**
 * @details  This function will carry out the Disable procedure for Path B.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkEnableOrDisableReq
 * @param    rspMsg : LkBsEnableOrDisableRsp
 */
TBOOL dehRhDisablePathB()
{
  EncBuf                   encBuf;
  LkHdr                    hdr;
  BsLkEnableOrDisableReq   req;
  TBOOL                    result = FALSE;

  fillHdr( MSG_BS_LK_ENABLE_OR_DISABLE_REQ, &hdr );
  fillDehRhDisableReqPathB( &req );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhEnableOrDisableReq( &req, &encBuf );

  result = sendMsgToRh( &encBuf);
  logMsgOut(" Msg %s sent to RH ", "MSG_BS_LK_ENABLE_OR_DISABLE_REQ");
  /* Log Message sent to RH */
  printMsgDehRhEnableReq(&req, &hdr );
  return result;
}

/**
 * @details  This function will carry out the Disable procedure for Path B.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkEnableOrDisableReq
 * @param    rspMsg : LkBsEnableOrDisableRsp
 */
TBOOL dehRhEnableOrDisablePath( UINT8 path, UINT8 control )
{
  EncBuf                   encBuf;
  LkHdr                    hdr;
  BsLkEnableOrDisableReq   req;
  TBOOL                    result = FALSE;

  fillHdr( MSG_BS_LK_ENABLE_OR_DISABLE_REQ, &hdr );
  fillDehRhEnableOrDisableReqPath( &req, path, control );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhEnableOrDisableReq( &req, &encBuf );

  result = sendMsgToRh( &encBuf);
  logMsgOut(" Msg %s sent to RH ", "MSG_BS_LK_ENABLE_OR_DISABLE_REQ");
  /* Log Message sent to RH */
  printMsgDehRhEnableReq(&req, &hdr );
  return result;
}

/**
 * @details  This function will carry out the Status Info procedure.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkStatusInfoReq
 * @param    rspMsg : LkBsStatusInfoRsp
 */
TBOOL dehRhStatusInfo(
  IO OamIfType oamIfType
  )
{
  EncBuf             encBuf;
  LkHdr              hdr;
  BsLkStatusInfoReq  req;
  TBOOL              result = FALSE;
  /* Get Command Reference
   * We are using OAM Interface number as Command Reference
   * */


  fillHdr( MSG_BS_LK_STATUS_INFO_REQ, &hdr );
  fillDehRhStatusInfoReq(&oamIfType , &req );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhStatusInfoReq( &req, &encBuf );

  result = sendMsgToRh( &encBuf);
  logMsgOut(" Msg %s sent to RH ", "MSG_BS_LK_STATUS_INFO_REQ");
  /* Log Message sent to RH */
  printMsgDehRhStatusInfoReq(&req, &hdr );
  return result;
}

/**
 * @details  This function will carry out the Firmware Info procedure.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkFwInfoReq
 * @param    rspMsg : LkBsFwInfoRsp
 */
TBOOL dehRhFirmwareInfo()
{
  EncBuf          encBuf;
  LkHdr           hdr;
  BsLkFwInfoReq   req;
  TBOOL           result;

  fillHdr( MSG_BS_LK_FW_INFO_REQ, &hdr );
  fillDehRhFwInfoReq( &req );
  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhFwInfoReq( &req, &encBuf );
  result = sendMsgToRh( &encBuf );
  logMsgOut(" Msg %s sent to RH ", "MSG_BS_LK_FW_INFO_REQ");
  printMsgDehRhFwInfoReq( &req, &hdr);
  return result;
}

/**
 * @details  This function will carry out the Hardware Info procedure.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkHwInfoReq
 * @param    rspMsg : LkBsHwInfoRsp
 */
TBOOL dehRhHardwareInfo()
{
  EncBuf           encBuf;
  LkHdr            hdr;
  BsLkHwInfoReq    req;
  TBOOL            result;

  fillHdr( MSG_BS_LK_HW_INFO_REQ, &hdr );
  fillDehRhHwInfoReq( &req );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhHwInfoReq( &req, &encBuf );

  result = sendMsgToRh( &encBuf);
  logMsgOut(" Msg %s sent to RH ", "MSG_BS_LK_HW_INFO_REQ");
  printMsgDehRhHwInfoReq( &req, &hdr);
  return result;
}

/**
 * @details  This function will carry out the History Read Info procedure.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkHistoryInfoReadReq
 * @param    rspMsg : LkBsHistoryInfoReadRsp
 */
TBOOL dehRhHistoryReadInfo(
  IO LkHistoryTable   *rspMsg
  )
{
  EncBuf                  encBuf;
  DecBuf                  decBuf;
  LkHdr                   hdr;
  BsLkHistoryInfoReadReq  req;
  TBOOL                   result;

  fillHdr( MSG_BS_LK_HISTORY_INFO_READ_REQ, &hdr );
  fillDehRhHistoryInfoReadReq( &req );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhHistoryInfoReadReq( &req, &encBuf );

  sendMsgToRh( &encBuf);

  expectLkMsg( MSG_LK_BS_HISTORY_INFO_READ_RSP, &decBuf);

  result = decRhDehHistoryInfoReadRsp( &decBuf, rspMsg);

  return result;
}

/**
 * @details  This function will carry out the History Write Info procedure.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkHistoryInfoWriteReq
 * @param    rspMsg : LkBsHistoryInfoWriteRsp
 */
TBOOL dehRhHistoryWriteInfo(
  IO LkHistoryWriteTable   *historyWriteReq
  )
{
  EncBuf                   encBuf;
  DecBuf                   decBuf;
  LkHdr                    hdr;
  BsLkHistoryInfoWriteReq  req;
  TBOOL                    result;

  fillHdr( MSG_BS_LK_HISTORY_INFO_WRITE_REQ, &hdr );
  fillDehRhHistoryInfoWriteReq( historyWriteReq, &req );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhHistoryInfoWriteReq( &req, &encBuf );

  sendMsgToRh( &encBuf);

  expectLkMsg( MSG_LK_BS_HISTORY_INFO_WRITE_RSP, &decBuf);

  result = decRhDehHistoryInfoWriteRsp( &decBuf );
  return result;
}

/**
 * @details  This function will carry out the Set DL/UL Ratio procedure.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkDlUlRatioReq
 * @param    rspMsg : LkBsDlUlRatioRsp
 */
void lkDlUlRatio()
{
  EncBuf              encBuf;
  DecBuf              decBuf;
  LkHdr               hdr;
  BsLkDlUlRatioReq    req;
  LkBsDlUlRatioRsp    rspMsg;

  fillHdr( MSG_BS_LK_DL_UL_RATIO_REQ, &hdr );
  fillDehRhDlUlRatioReq( &req );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhDlUlRatioReq( &req, &encBuf );

  sendMsgToRh( &encBuf);

  expectLkMsg( MSG_LK_BS_DL_UL_RATIO_RSP, &decBuf);

  //decRhDehDlUlRatioRsp( &decBuf, &rspMsg );
  /* Check the result */
  if( rspMsg.varResult NOTEQ 0 )
	logError(" Expected=0, received=%d", rspMsg.varResult );
}

/**
 * @details  This function will carry out the Set Carrier LO Procedure.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkCarrierLOSettingReq
 * @param    rspMsg : LkBsCarrierLoSettingRsp
 */
TBOOL dehRhSetCarrierLo(
  IN RhFANum    fANum,
  IN LoSettings loEvenOrOdd
  )
{
  EncBuf             encBuf;
  DecBuf             decBuf;
  LkHdr              hdr;
  BsLkCarrierLOSettingReq    req;
  LkBsCarrierLoSettingRsp    rspMsg;

  fillHdr( MSG_BS_LK_CARRIER_LO_SETTING_REQ, &hdr );
  fillDehRhCarrierLOSettingReq( &fANum, &loEvenOrOdd, &req );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhCarrierLOSettingReq( &req, &encBuf );

  sendMsgToRh( &encBuf);

  expectLkMsg( MSG_LK_BS_CARRIER_LO_SETTING_RSP, &decBuf);

  //decRhDehCarrierLoSettingRsp( &decBuf, &rspMsg );
  /* Check the result */
  if( rspMsg.varResult NOTEQ 0 )
  {
    logError("RhRhFaControlFailed, Expected=0, received=%d", rspMsg.varResult );
    return FALSE;
  }
  return TRUE;
}

/**
 * @details  This function will carry out the OperatingMode Info procedure.
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkStatusInfoReq
 * @param    rspMsg : LkBsStatusInfoRsp
 */
TBOOL dehRhOperatingModeInfo()
{
  EncBuf                      encBuf;
  LkHdr                       hdr;
  BsLkOperationalModeInfoReq  req;
  TBOOL						  result;

  fillHdr( MSG_BS_LK_OPERATION_MODE_INFO_REQ, &hdr );
  fillDehRhOperationalModeInfoReq( &req );
  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhOperationalModeInfoReq( &req, &encBuf );
  result = sendMsgToRh( &encBuf);
  logMsgOut(" Msg %s sent to RH ", "MSG_BS_LK_OPERATION_MODE_INFO_REQ");
  printMsgDehRhOperatingModeInfoReq( &req, &hdr);
  return result;
}

/**
 * @details  This functionis used to set Time .
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkStatusInfoReq
 * @param    rspMsg : LkBsStatusInfoRsp
 */
TBOOL dehRhSetTime(
  IN TimeTick  *time
  )
{
  EncBuf          encBuf;
  DecBuf          decBuf;
  LkHdr           hdr;
  BsLkTimeSetReq  req;
  LkBsTimeSetRsp  rspMsg;

  fillHdr( MSG_BS_LK_TIME_SET_REQ, &hdr );
  fillDehRhTimeSetReq(time, &req );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhTimeSetReq( &req, &encBuf );

  sendMsgToRh( &encBuf );

  expectLkMsg( MSG_LK_BS_TIME_SET_RSP, &decBuf);

 // decRhDehTimeSetRsp( &decBuf, &rspMsg );
  /* Check the result */
  if( rspMsg.varResult NOTEQ 0 )
  {
	logError(" RhTimeSetReqFailed, Expected=0, received=%d", rspMsg.varResult );
    return FALSE;
  }
  return TRUE;
}

/**
 * @details  This function will carry out the Rh software install procedure
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkSwInstallreq
 * @param    data   : MsgPrmDehRhSwInstallReq
 */
void dehSendRhSwInstallReq(
  IO MsgPrmDehRhSwInstallReq *data
  )
{
  EncBuf            encBuf;
  LkHdr             hdr;
  BsLkSwInstallreq  req;

  clearBytes(&req,sizeof(BsLkSwInstallreq));

  fillHdr( MSG_BS_LK_SW_INSTALL_REQ, &hdr );
  fillDehRhSwInstallReq(data, &req );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhSwInstallReq( &req, &encBuf );

  sendMsgToRh( &encBuf);
  printMsgDehRhSwInstallReq( &req, &hdr);
}

/**
 * @details  This function will carry out the Rh software Activate procedure
 *           1. Fill and encode the REQ msg ( IPC Header and Application data )
 *           2. Send the Message to LK
 *           3. Receive the RSP and decode it.
 *           4. Check the expected result.
 * @param    encBuf : Static buffer used for encode Message
 * @param    decBuf : Static buffer used for decode Message
 * @param    hdr    : LK Header Information structure
 * @param    req    : BsLkSwActivateReq
 * @param    side   : RH side to activate
 */
void dehSendRhSwActivateReq(
  IN UINT8  side
  )
{
  EncBuf            encBuf;
  LkHdr             hdr;
  BsLkSwActivateReq  req;

  fillHdr( MSG_BS_LK_SET_ACTIVE_SIDE_REQ, &hdr );
  fillDehRhSwActivateReq(side, &req );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhSwActivateReq( &req, &encBuf );

  sendMsgToRh( &encBuf);
  printMsgDehRhActivateReq( &req, &hdr);
}

/**
 * @details  This function will send Remote fusing NOTI Ack to RH
 **/
void dehSendRhFusingNotiAck()
{
  EncBuf                       encBuf;
  LkHdr                        hdr;
  BsLkRemoteFusingCompleteAck  req;

  clearBytes(&req, sizeof(BsLkRemoteFusingCompleteAck));
  clearBytes(&hdr, sizeof(LkHdr));


  fillHdr( MSG_BS_LK_FUSING_COMPLETE_ACK_ID, &hdr );
  fillDehRhRemoteFusingCompleteAck( &req );
  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhRemoteFusingCompleteAck( &req, &encBuf );
  sendMsgToRh( &encBuf );
  logMsgOut(" Msg  [%s] sent to RH ","MSG_BS_LK_FUSING_COMPLETE_ACK_ID sent to RH");
}
/**
 * @details  This function will send INIT NOTI Ack to RH
 **/

void dehSendRhNotiAck()
{
  EncBuf                 encBuf;
  LkHdr                  hdr;
  BsLkInitializationAck  req;

  clearBytes(&req, sizeof(BsLkInitializationAck));
  clearBytes(&hdr, sizeof(LkHdr));

  fillHdr( MSG_BS_LK_INITIALIZATION_ACK_ID, &hdr );
  fillDehRhInitializationAck( &req );
  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhInitializationAck( &req, &encBuf );
  sendMsgToRh( &encBuf );
  logMsgOut(" Msg  [%s] sent to RH ","MSG_BS_LK_INITIALIZATION_ACK_ID sent to RH");
  printMsgDehRhInitializationAck(&req, &hdr );
}
