/**
  * @file    bsm_deh_msg_print.c
  * @details Copyright Powerwave Technologies, Inc., 2010
  * @brief This file contains the print functions for the messages that are exchanged
  *          between BSM and DEH
  * @author :  Shweta Polepally
  * Date   :
  * Change History:
  *
  */

#include "bsm_deh_msg_print.h"
#include "print_tlv.h"

void printMsgBsmDehSetIpAddrReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmDehSetIpAddrReq *msg = (MsgBsmDehSetIpAddrReq*)data;
  UINT8 index;
  logMsgParams("%s","{");
  for(index = 0; index < ROWS_IN_SET_IP_ADDR_REQ ;index++)
  {
    logMsgParams( "\n interfaceType=%d (BACKHAUL=0, DEBUG=1)", msg->ipAddrSetReq[index].interfaceType );
    logMsgParams( "\n typeOfIpAddr=%d (STATIC=0, DHCP=1)", msg->ipAddrSetReq[index].typeOfIpAddr );
    logMsgParams( "\n ipAddress=%s", msg->ipAddrSetReq[index].ipAddress );
    logMsgParams( "\n netMask=%s ", msg->ipAddrSetReq[index].netMask );
  }
  logMsgParams( "\n} Total length :%d \n", len );
}

void printMsgDehBsmSetIpAddrRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgDehBsmSetIpAddrRsp *msg = (MsgDehBsmSetIpAddrRsp*)data;
  UINT8 index;
  logMsgParams("%s","{");
  for(index = 0; index < ROWS_IN_SET_IP_ADDR_REQ ;index++)
  {
    logMsgParams( "\n interfaceType=%d (BACKHAUL=0, DEBUG=1)", msg->ipAddrSetRsp[index].interfaceType );
    logMsgParams( "\n typeOfIpAddr=%d (STATIC=0, DHCP=1)", msg->ipAddrSetRsp[index].typeOfIpAddr );
    logMsgParams( "\n ipAddress=%s", msg->ipAddrSetRsp[index].ipAddress );
    logMsgParams( "\n netMask=%s", msg->ipAddrSetRsp[index].netMask );
    logMsgParams( "\n isSetSucc=%d ", msg->ipAddrSetRsp[index].isSetSucc );
    if(FALSE EQ msg->ipAddrSetRsp[index].isSetSucc )
    {
      logMsgParams( "\n errorNum=%s ", errorCodeStr(msg->ipAddrSetRsp[index].errDesc.errorNum) );
      logMsgParams( "\n failReason=%s", msg->ipAddrSetRsp[index].errDesc.failReason );
    }
  }
  logMsgParams( "\n} Total length :%d \n", len );
}

void printMsgSetDeviceColor(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmDehSetLedColor *msg = (MsgBsmDehSetLedColor*)data;

  logMsgParams("%s","{");
  logMsgParams( "\n ledId=%s \n ledColor=%s", ledIdStr(msg->ledId), ledColorStr(msg->ledColor) );
  logMsgParams( "\n} Total length :%d \n", len );
}

static void printComponentOpState(
  IN ComponentOpState   *msg
  )
{
  logMsgParams( " componentType=%s\n componentOperationalState=%s\n componentInstance=%d\n resourceString=%s\n",
      componentTypeStr(msg->componentType), componentOperationalStateStr(msg->componentOperationalState),msg->componentInstance,
      msg->infoString );
  if ( COMPONENTOPERATIONALSTATE_DOWN EQ msg->componentOperationalState )
  {
    logMsgParams( " bsErrorForDeviceDown=%s\n bsErrorString=%s\n additionalString=%s\n",
        errorCodeStr(msg->bsErrorForDeviceDown.errorNum), msg->bsErrorForDeviceDown.failReason,
        msg->errorString );
  }
}

void printMsgDeviceInitSucc(
  IO  void    *data,
  IO  UINT16  len
  )
{
  UINT8      cnt = 0;
  MsgDehBsmDeviceInitSucc *msg = (MsgDehBsmDeviceInitSucc*)data;
  logMsgParams( "{\n numOfDevices=%d", msg->numOfDevices );
  for (cnt= 0; cnt <  msg->numOfDevices; cnt++)
  {
    printComponentOpState( &msg->componentOpState[cnt] );
  }
  logMsgParams( "\n} Total length :%d \n", len );
}

void printMsgDeviceInitFail(
  IO  void    *data,
  IO  UINT16  len
  )
{
  UINT8      cnt = 0;
  MsgDehBsmDeviceInitFail *msg = (MsgDehBsmDeviceInitFail*)data;

  logMsgParams( "{\n numOfDevices=%d", msg->numOfDevices);
  for (cnt= 0; cnt <  msg->numOfDevices; cnt++)
  {
    printComponentOpState( &msg->componentOpState[cnt] );
  }
  logMsgParams( "} Total length :%d \n", len );
}

void printMsgDehBsmComponentDownInd(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgDehBsmComponentDownInd *msg = (MsgDehBsmComponentDownInd*)data;

  logMsgParams("%s","{\n");
  printComponentOpState( msg );
  logMsgParams( "} Total length :%d \n", len );
}

void printMsgDehBsmComponentUpInd(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgDehBsmComponentUpInd *msg = (MsgDehBsmComponentUpInd*)data;

  logMsgParams("%s","{\n");
  printComponentOpState( msg );
  logMsgParams( "} Total length :%d \n", len );
}

void printMsgDehBsmGpsDetSucc(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgDehBsmGpsDetailSucc *msg = (MsgDehBsmGpsDetailSucc*)data;

  logMsgParams( "{\n longitude=%s\n latitude=%s\n time=%d:%d:%d (hh:mm:ss)\n date=%d/%d/%d (dd/mm/yyyy)\n", \
               msg->longitude, msg->latitude, msg->time.hours, msg->time.minutes, \
               msg->time.seconds, msg->date.date, msg->date.month, msg->date.year);
  logMsgParams( "} Total length :%d \n", len );
}

void printMsgDehBsmGpsDetFail(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgDehBsmGpsDetailFail *msg = (MsgDehBsmGpsDetailFail*)data;

  logMsgParams( "{\n errDetail=%s\n ", msg->errDetail);
  logMsgParams( "} Total length :%d \n", len );
}

void printMsgBsmDehRhConfigReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmDehRhConfigReq *msg = (MsgBsmDehRhConfigReq*)data;

  logMsgParams( "{\n transId=%d\n radioHeadId=%d\n c1IQMap=%d\n txAttenPathA=%d\n txAttenPathB=%d\n", \
               msg->transId, msg->radioHeadId, msg->c1IQMap, msg->txAttenPathA, msg->txAttenPathB);
  logMsgParams( " txEarfcn=%d\n rxEarfcn=%d\n", msg->txEarfcn, msg->rxEarfcn);
  logMsgParams( " txDelay=%d\n rxDelay=%d\n", msg->txDelay, msg->rxDelay);
  logMsgParams( " vswrShutdownAlarmThreshold=%d\n overpowerMinorAlarmThreshold=%d\n lowpowerAlarmThreshold=%d\n", \
               msg->vswrShutdownAlarmThreshold, msg->overpowerMinorAlarmThreshold, msg->lowpowerAlarmThreshold );

  logMsgParams( "} Total length :%d \n", len );
}

void printMsgBsmDehGetEnbAddrReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmDehGetEnbAddrReq *msg = (MsgBsmDehGetEnbAddrReq*)data;

  logMsgParams( "%s","{\n");
  logMsgParams(" origin:%d\n cmdRefNum:%d\n instanceId:%d\n",msg->cmdRef.origin, msg->cmdRef.cmdRefNum,msg->cmdRef.instanceId );
  printTlvTagInfo(&msg->tagInfo);
  logMsgParams( "} Total length :%d \n", len );
}

void printMsgDehBsmGetEnbAddrRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgDehBsmGetEnbAddrRsp *msg = (MsgDehBsmGetEnbAddrRsp*)data;

  logMsgParams( "%s","{\n");
  logMsgParams(" origin:%d\n cmdRefNum:%d\n instanceId:%d\n",msg->cmdRef.origin, msg->cmdRef.cmdRefNum,msg->cmdRef.instanceId );
  printTlvTagInfo(&(msg->tagInfo));
  logMsgParams( " backhaulPortMacAddress=%s\n debugPortMacAddress=%s\n productId=%s\n", msg->backhaulPortMacAddress, msg->debugPortMacAddress, \
		          msg->productId );
  logMsgParams( "} Total length :%d \n", len );
}

void printMsgDehBsmGetEnbAddrFail(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgDehBsmGetEnbAddrFail *msg = (MsgDehBsmGetEnbAddrFail*)data;

  logMsgParams( "%s","{\n");
  logMsgParams(" origin:%d\n cmdRefNum:%d\n instanceId:%d\n",msg->cmdRef.origin, msg->cmdRef.cmdRefNum,msg->cmdRef.instanceId );
  printTlvTagInfo(&(msg->tagInfo));
  logMsgParams( " bsError=%d\n ", msg->bsError);
  logMsgParams( "} Total length :%d \n", len );
}
