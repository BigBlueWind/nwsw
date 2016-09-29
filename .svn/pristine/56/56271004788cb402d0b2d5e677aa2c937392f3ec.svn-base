/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#pragma once

#include "Logger.h"

#define PN_SUCCESS 0
#define PN_FAILURE -1 

#define PN_TRUE 1
#define PN_FALSE 0 

#define SERVICEPORT             10300
#define PW_MAX_MSG_LENGTH       4096
#define PW_MAX_PRIM_LENGTH      4096
#define PW_LOG_FILE_NAME        "PW_PAL.log"
#define PW_LOG_FILE_LENGTH      200
#define NUMBER_OF_TB            1
#define MAX_SUBFRAME_COUNT      10
#define MAX_SFN_COUNT           65535
#define MAX_UE_COUNT            4
#define SI_RNTI                 65535
#define P_RNTI                  65534
#define TEMP_C_RNTI_START       104

typedef unsigned char           PW_BOOL;

typedef enum
{
    EMSPD_Reserved = 0,
    EMSPD_PHY_TXSTART_request,
    EMSPD_PHY_TXSTART_confirmation,
    EMSPD_PHY_TXSTART_indication,
    EMSPD_PHY_TXSDU_request,
    EMSPD_PHY_TXSDU_confirmation,
    EMSPD_PHY_TXEND_indication,
    EMSPD_PHY_RXSTART_request,
    EMSPD_PHY_RXSTART_confirmation,
    EMSPD_PHY_RXSTART_indication,
    EMSPD_PHY_RXSDU_indication,
    EMSPD_PHY_RXEND_indication,
    EMSPD_PHY_INIT_request,
    EMSPD_PHY_INIT_indication,
    EMSPD_PHY_RXSTATUS_indication,
    EMSPD_PHY_RECONFIG_request,
    EMSPD_PHY_RECONFIG_confirmation,
    EMSPD_PHY_START_request,
    EMSPD_PHY_START_confirmation,
    EMSPD_PHY_STOP_request,
    EMSPD_PHY_STOP_confirmation,
} PW_EMspdPhyPrimitive;

typedef struct 
{
    unsigned char                       m_ucSubframe;
} TMspdPhyTxStartRequest;

typedef struct 
{
    unsigned short                  m_usChannelId;
    unsigned char                   m_ucChannelType;
    unsigned short                  m_usPhySduSizeInBytes;
    unsigned char*                  m_pucPhySdu;
    unsigned short                  m_usRnti;
} TMspdPhyTxSduRequest;

typedef union
{
    TMspdPhyTxStartRequest              m_sMspdPhyTxStartRequest;
    TMspdPhyTxSduRequest                m_sMspdPhyTxSduRequest;
} TMspdPhyPrimitiveUnion;

typedef struct 
{
    unsigned char                   m_ucVersion;
    PW_EMspdPhyPrimitive            m_eMspdPhyPrimitive;
    TMspdPhyPrimitiveUnion          m_sPhyPrimitiveUnion;
} TDLAPIEventInfo;

typedef struct 
{
    unsigned char                   m_ucVersion;
} TTimerEventInfo;

typedef enum
{
    CH_PDCCH = 0,
    CH_PDSCH,
    CH_PBCH,
    CH_PCFICH,
    CH_PHICH,
    CH_PUSCH,
    CH_PRACH,
    CH_PUCCH,
}PW_EMspdChannelType;
