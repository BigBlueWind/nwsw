//-------------------------------------------------------------------------------------------
/** @file svsrloggerstorage.h
 *
 * @brief The declaration of supervisor logger API
 * @author Mindspeed Technologies
 * @version $Revision: 1.5 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _SVSRLOGGERSTORAGE_H_
#define _SVSRLOGGERSTORAGE_H_


#include "4gmx.h"
#include "appids.h"

#define LOGGER_VERSION              0x0300

#define RC_STORAGE_OK               MX_OK
#define RC_STORAGE_ERROR         	DEF_USRRC(APP_MODULE, LOGERSTORAGE, 1)
#define RC_STORAGE_OVERFLOW			DEF_USRRC(APP_MODULE, LOGERSTORAGE, 2)

// RAD MDMA Logging enabled/dissabled
//#define LOGGER_RADMDMA_COPY_ENABLED

// For greater values MDMA copy preferable
// (Value received by ticks meassurement for real-life system)
#define LOGGER_MEMCOPY_VS_MDMA_LIMIT_SIZE  1500

#define PHY_TXSTART_REQ     1
#define PHY_TXSTART_CONF    2
#define PHY_TXSTART_IND     3
#define PHY_TXSDU_REQ       4
#define PHY_TXSDU_CONF      5
#define PHY_TXEND_IND       6
#define PHY_RXSTART_REQ     7
#define PHY_RXSTART_CONF    8
#define PHY_RXSTART_IND     9
#define PHY_RXSDU_IND       10
#define PHY_RXEND_IND       11
#define PHY_INIT_REQ        12
#define PHY_INIT_IND        13
#define PHY_RXSTATUS_IND    14
#define PHY_RECONFIG_REQ    15
#define PHY_RECONFIG_CNF    16
#define PHY_START_REQ       17
#define PHY_START_CONF      18
#define PHY_STOP_REQ        19
#define PHY_STOP_CONF       20
#define PHY_STOP_IND        21
#define PHY_TXHIADCIUL_REQ  22
#define PHY_TXHISDU_REQ     23
#define PHY_TXDCIULSDU_REQ  24


typedef struct _STORAGE_HEADER_
{
    // THE DATA IS CACHE ALIGNED!!

    VUINT32      Version;        // Log version (for the host application and for the C-CODE)
    VUINT32      Flag;           // Status flags: see: LOGGER_FLAG_
    VUINT32      MsgNum;         // The number of messages written to the log
    VUINT32      PayloadSize;    // The size of data located in the log storage
    VUINT32      LogStorSize;    // The size of log storage    
    VUINT32		 StartOffset;	 // offset from Phy Start command (always first in log)
	VUINT32		 StartPhySize;
    VUINT32      Res;	         // Reserved

    // THE DATA IS CACHE ALIGNED!!

}STORAGE_HEADER, *pSTORAGE_HEADER;

typedef struct _API_LOG_INFO_
{
    UINT32 PayloadLen;  // Just length of payload, it does not include length of header
    UINT32 Timestamp;
    UINT32 Message;
    UINT8  Source;
    UINT8  Destination;
    UINT8  PHY_ID;
    UINT8  MAC_ID;
    UINT32 CurrentTTI;
	LPVOID nextTTI;
	UINT8  roundup;
	UINT8  end;
	UINT8  Reserved[6];
}API_LOG_INFO, *pAPI_LOG_INFO;

typedef struct _STORAGE_CTX_
{
	pSTORAGE_HEADER hdr;
	pAPI_LOG_INFO 	curTTIstart;
	pAPI_LOG_INFO 	curApi;
    UINT32	LastOffset;	 // offset from Phy Start command (always first in log)  
	UINT32  PaddingSize;
	UINT32	SubframeNum;
	UINT32  TTIreset;
}STORAGE_CTX, *pSTORAGE_CTX;

UINT32 LoggerStorageGetLogVersion(pSTORAGE_CTX ctx);
pSTORAGE_CTX LoggerStorageCreate(PUINT8 *ptr, UINT32 size);
MXRC LoggerStorageReset(pSTORAGE_CTX ctx, UINT32 size);
MXRC LoggerStorageAddMsg(pSTORAGE_CTX ctx, UINT32 nMsgID, UINT32 nMacPhySrcDstID, LPVOID pMsg, UINT32 nMsgSize);
API_LOG_INFO * LoggerStorageGetMsg (pSTORAGE_CTX ctx, UINT32 nMsgIndex);
API_LOG_INFO * LoggerStorageGetNextMsg (pSTORAGE_CTX ctx, UINT32 * pnMsg);

#endif

#ifdef __cplusplus
}
#endif

