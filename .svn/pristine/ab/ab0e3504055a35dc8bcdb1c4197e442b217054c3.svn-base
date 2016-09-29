/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#pragma once

#include "PW_PALDefinition.h"

#ifdef LINUX
#include <sys/time.h>
#include <sys/types.h>
#endif

#ifdef __cplusplus 
extern "C" {
#endif

#define PHY_TXSTART_REQ	 1
#define PHY_TXSTART_CONF 2
#define PHY_TXSTART_IND  3
#define PHY_TXSDU_REQ    4
#define PHY_TXSDU_CONF   5
#define PHY_TXEND_IND    6
#define PHY_RXSTART_REQ  7
#define PHY_RXSTART_CONF 8
#define PHY_RXSTART_IND  9
#define PHY_RXSDU_IND    10
#define PHY_RXEND_IND    11
#define PHY_INIT_REQ     12
#define PHY_INIT_IND     13
#define PHY_RXSTATUS_IND 14
#define PHY_RECONFIG_REQ 15
#define PHY_RECONFIG_CNF 16
#define PHY_START_REQ    17
#define PHY_START_CONF   18
#define PHY_STOP_REQ     19
#define PHY_STOP_CONF    20

#define PHY_STOP_IND        21
#define PHY_TXHIADCIUL_REQ  22
#define PHY_TXHISDU_REQ     23
#define PHY_TXDCIULSDU_REQ  24

int PW_PALInitLibrary();
int PW_PALCloseLibrary();

/*Mindspeed API specific functions*/
int PW_PALSendPhyTxStartIndication(unsigned short usSFN, unsigned char ucSubframe);
int PW_PALSendPhyRxStartIndication(unsigned char ucSubframe);
int PW_PALSendPhyRxSduIndicationForRACH(unsigned char ucSubframe, unsigned char ucPreambleId);
int PW_PALSendPhyRxSduIndicationForULSCH(unsigned short usSFN, unsigned char ucSubframe,
    unsigned int uiCrnti, unsigned int uiMacPduSize, unsigned char* pucMacPdu);

int PW_PALReceivePhyTxStartRequest(unsigned char ucSubframe);

int PW_PALReceivePhyTxSduRequest(unsigned short usChannelId,
                                 unsigned char ucChannelType,
                                 unsigned short usPhySduSizeInBytes,
                                 unsigned char* pucPhySdu,
                                 unsigned short usRnti);

#ifdef __cplusplus 
}
#endif
