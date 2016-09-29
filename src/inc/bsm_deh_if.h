/**
 * @file    bsm_deh_if.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains the messages that are exchanged between BSM and DEH
 * @author  Amar Ramteke
 * @author  Madhu Kata
 *
 * @details DEH : DEVICE HANDLER
 */

#ifndef BSM_DEH_IF_H
#define BSM_DEH_IF_H

#include "cmn_types.h"
#include "msg_ids.h"
#include "imc_msg_desc.h"
#include "log.h"

#include "rh_deh_if.h"
#include "lte_bb_gps.h"
#include "lte_bb_led.h"

#define LEDID_MAX LEDNUM_MAX_LED+1 /* +1 is required as LEDNUM_MAX_LED enum starts from 0 value, max led = 4 */
#define ROWS_IN_SET_IP_ADDR_REQ  INTERFACE_MAX

#define ETHERNET_BACKHAUL  1
#define ETHERNET_DEBUG     2

typedef enum e_InterfaceType
{
  INTERFACE_BACKHAUL = 0,
  INTERFACE_DEBUG,
  INTERFACE_MAX
} InterfaceType;

typedef enum e_TypeOfIpAddr
{
  IPADDR_STATIC = 0,
  IPADDR_DHCP
} TypeOfIpAddr;

typedef struct s_IpAddrSetReq
{
  InterfaceType  interfaceType;
  TypeOfIpAddr   typeOfIpAddr;
  IpAddrStr      ipAddress;
  IpAddrStr      netMask;
} IpAddrSetReq;

typedef struct s_MsgBsmDehSetIpAddrReq
{
  IpAddrSetReq   ipAddrSetReq[ROWS_IN_SET_IP_ADDR_REQ];
} MsgBsmDehSetIpAddrReq;

typedef struct s_IpAddrSetRsp
{
  InterfaceType  interfaceType;
  TypeOfIpAddr   typeOfIpAddr;
  IpAddrStr      ipAddress;
  IpAddrStr      netMask;
  TBOOL          isSetSucc;
  ErrorDesc      errDesc;     /* Applicable only when isSetSucc = FALSE */
} IpAddrSetRsp;

typedef struct s_MsgDehBsmSetIpAddrRsp
{
  IpAddrSetRsp   ipAddrSetRsp[ROWS_IN_SET_IP_ADDR_REQ];
} MsgDehBsmSetIpAddrRsp;

typedef struct DeviceOpState
{
  ComponentType              componentType;
  UINT8                      componentInstance;
  ComponentOperationalState  componentOperationalState;
  UINT8                      infoString[MAX_SIZE_COMPONENTSTATUS_INFOSTRING + 1];    /* info string */
  /*Below fields to be interpreted if opState = DOWN */
  ErrorDesc                  bsErrorForDeviceDown;
  UINT8                      errorString[MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1];   /* Error string */

  /* Remove later
   * DeviceType   - RH             GPS               ETH
   * DeviceInstance - 0             0                 1
   * OpState    - DOWN             DOWN              DOWN
   * infoString - PATH_A       NULL              NULL
   * AlarmCause - lowGain          notResp           ethDevDown
   * errorString - "lowGain = -20dB"  |  NULL   |  " Mac Address/"  */
} ComponentOpState;

/** @addtogroup if_group
 *  @{
 */

/** @name BSM_DEH_MESSAGES
 *  @{
 */

typedef struct s_MsgDehBsmDeviceInitSucc
{
  UINT8              numOfDevices;   /* numOfDevices =  MAX_DEVICES */
  ComponentOpState   componentOpState[MAX_ROWS_IN_COMPONENTSTATUS_TABLE];
} MsgDehBsmDeviceInitSucc;


typedef MsgDehBsmDeviceInitSucc  MsgDehBsmDeviceInitFail;

typedef ComponentOpState         MsgDehBsmComponentDownInd;

typedef ComponentOpState         MsgDehBsmComponentUpInd;

typedef struct s_MsgBsmDehSetLedColor
{
  LedId    ledId;
  LedColor ledColor;
} MsgBsmDehSetLedColor;

/* OP-START */
typedef ErrorDesc MsgDehBsmOpstartFail;

/* Radio Head(LK) Configuration request */
typedef struct s_MsgBsmDehRhConfigReq
{
  UINT16   transId;
  UINT32   radioHeadId;
  /*0: no use
  5: use IQ mapping in 5MHz Ch BW
  10: use IQ mapping in 10MHz Ch BW
  20: use IQ mapping in 20MHz Ch BW
  In this case, C2 will be used for an extended C1. Then C2 IQ Map will be not available.*/
  C1IQMap c1IQMap;
  /*Real Range: 0~27.0 [dB]*/
  UINT32   txAttenPathA;
  /*Real Range: 0~27.0 [dB]*/
  UINT32   txAttenPathB;
  /*Real Range: 0~3000.00 [MHz]*/
  UINT32   txEarfcn;
  /*Real Range: 0~3000.00 [MHz]*/
  UINT32   rxEarfcn;
  /*Real Range  :0.0 to 300[us] */
  UINT32   txDelay;
  /*Real Range  :0.0 to 300[us] */
  UINT32   rxDelay;
  /*RSSI Level Real Range: -327.60 ~ -0.01 dBm*/
  //SINT32   rssiThreshold;
  /*Real Range: 0.0 ~ 8.0[dB]*/
  UINT32   vswrShutdownAlarmThreshold;
  /*Digital IQ Level for Tx*/
  UINT32   overpowerMinorAlarmThreshold;
  /*Digital IQ Level for Tx*/
  UINT32   lowpowerAlarmThreshold;
} __attribute__((__packed__))MsgBsmDehRhConfigReq;

/* Configuration Request response */
typedef EnbConfigSucc MsgDehBsmRhConfigSucc;
typedef EnbConfigFail MsgDehBsmRhConfigFail;

typedef MsgBsmDehRhConfigReq MsgBsmDehRhInitConfigReq;
typedef EnbConfigSucc MsgDehBsmRhInitConfigSucc;
typedef EnbConfigFail MsgDehBsmRhInitConfigFail;

typedef struct s_MsgDehBsmGpsDetailSucc
{
  UINT8      latitude[MAX_SIZE_LOCATIONSTATUS_LATITUDE + 1 ];
  UINT8      longitude[MAX_SIZE_LOCATIONSTATUS_LONGITUDE + 1 ];
  TimeInfo   time;
  DateInfo   date;
} __attribute__((__packed__))MsgDehBsmGpsDetailSucc;

typedef struct s_MsgDehBsmGpsStatusSucc
{
  UINT8      latitude[MAX_SIZE_LOCATIONSTATUS_LATITUDE + 1 ];
  UINT8      longitude[MAX_SIZE_LOCATIONSTATUS_LONGITUDE + 1 ];
  DateInfo   date;
  TimeInfo   time;
} __attribute__((__packed__))MsgDehBsmGpsStatusSucc;

typedef struct s_MsgDehBsmGpsDetailFail
{
  String256  errDetail;
} __attribute__((__packed__))MsgDehBsmGpsDetailFail;

typedef struct s_MsgBsmDehGetEnbAddrReq
{
  CmdRef         cmdRef;
  TagInfo        tagInfo;
} __attribute__((__packed__))MsgBsmDehGetEnbAddrReq;

typedef struct s_MsgDehBsmGetEnbAddrRsp
{
  CmdRef         cmdRef;
  TagInfo        tagInfo;
  MACADDRESS     backhaulPortMacAddress[MAX_SIZE_ENBADDRESSES_BACKHAULPORTMACADDRESS + 1 ];
  MACADDRESS     debugPortMacAddress[MAX_SIZE_ENBADDRESSES_DEBUGPORTMACADDRESS + 1 ];
  UINT8          productId[MAX_SIZE_ENBADDRESSES_UNIQUEPRODUCTID + 1];
} __attribute__((__packed__))MsgDehBsmGetEnbAddrRsp;

typedef struct s_MsgDehBsmGetEnbAddrFail
{
  CmdRef         cmdRef;
  TagInfo        tagInfo;
  BsErrorValue   bsError;
} __attribute__((__packed__))MsgDehBsmGetEnbAddrFail;

typedef SetLogConfig MsgBsmDehSetLogCfg;

/** @}
 */

/** @}
 */

#endif /* BSM_DEH_IF_H */
