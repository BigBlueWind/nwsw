//-------------------------------------------------------------------------------------------
/** @file svsrlogger.h
 *
 * @brief The declaration of supervisor logger API
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef _WIN32

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _SVSR_LOGGER_H_
#define _SVSR_LOGGER_H_



/************************************************************************/
/*     UINT64, UINT32, UINT16 and UINT8 definition                      */
/************************************************************************/
#ifndef _UINT64_
#define _UINT64_
typedef unsigned __int64 UINT64, *PUINT64;
#endif /*_UINT64_*/

#ifndef _UINT32_
#define _UINT32_
typedef unsigned int UINT32, *PUINT32;
#endif /*_UINT32_*/

#ifndef _UINT16_
#define _UINT16_
typedef unsigned short UINT16, *PUINT16;
#endif /*_UINT16_*/

#ifndef _UINT8_
#define _UINT8_
typedef unsigned char UINT8, *PUINT8;
#endif /*_UINT8_*/

#ifndef _VUINT64_
#define _VUINT64_
typedef volatile unsigned __int64 VUINT64, *PVUINT64;
#endif /*_UINT64_*/

#ifndef _VUINT32_
#define _VUINT32_
typedef volatile unsigned int VUINT32, *PVUINT32;
#endif /*_UINT32_*/

#ifndef _VUINT16_
#define _VUINT16_
typedef volatile unsigned short VUINT16, *PVUINT16;
#endif /*_UINT16_*/

#ifndef _VUINT8_
#define _VUINT8_
typedef volatile unsigned char VUINT8, *PVUINT8;
#endif /*_UINT8_*/

/************************************************************************/
/*     SINT64, SINT32, SINT16 and SINT8 definition                      */
/************************************************************************/
#ifndef _SINT64_
#define _SINT64_
typedef __int64 SINT64, *PSINT64;
#endif /*_SINT64_*/

#ifndef _SINT32_
#define _SINT32_
typedef int SINT32, *PSINT32;
#endif /*_SINT32_*/

#ifndef _SINT16_
#define _SINT16_
typedef signed short SINT16, *PSINT16;
#endif /*_SINT16_*/

#ifndef _SINT8_
#define _SINT8_
typedef signed char SINT8, *PSINT8;
#endif /*_SINT8_*/

#ifndef _MXRC_
#define _MXRC_
typedef UINT32 MXRC;
typedef VUINT32 VMXRC;
#endif /*_MXRC_*/

#ifndef _LPVOID_
#define _LPVOID_
typedef void *LPVOID;
typedef const void *LPCVOID;
#endif /*_LPVOID_*/



#define MX_OK         0
#define APP_MODULE    0x02
#define APILOGGER     0x25
#define LOGERSTORAGE  0x70

#define DEF_USRRC(app, mod, err)  (((app) << 16)| ((mod) << 8) | (err) )

#define RC_LOGGER_ALLOC_ERROR       DEF_USRRC(APP_MODULE, APILOGGER, 1)
#define RC_LOGGER_OVERFLOW          DEF_USRRC(APP_MODULE, APILOGGER, 2)
#define RC_LOGGER_UNKNOWN_API       DEF_USRRC(APP_MODULE, APILOGGER, 3)
#define RC_LOGGER_API_ERROR         DEF_USRRC(APP_MODULE, APILOGGER, 4)
#define RC_LOGGER_HEADER_NULL       DEF_USRRC(APP_MODULE, APILOGGER, 5)
#define RC_LOGGER_CTX_NULL          DEF_USRRC(APP_MODULE, APILOGGER, 6)
#define RC_LOGGER_STORAGE_NULL      DEF_USRRC(APP_MODULE, APILOGGER, 7)
#define RC_LOGGER_DUMP_ERR          DEF_USRRC(APP_MODULE, APILOGGER, 8)

#define RC_STORAGE_OK               MX_OK
#define RC_STORAGE_ERROR            DEF_USRRC(APP_MODULE, LOGERSTORAGE, 1)
#define RC_STORAGE_OVERFLOW         DEF_USRRC(APP_MODULE, LOGERSTORAGE, 2)


#define LOGGER_VERSION              0x0300
#define LOGGER_DEF_MASK             0x0

#define LOGGER_GET_DST_ID(x)        (((x) >>  0) & 0xFF)
#define LOGGER_GET_SRC_ID(x)        (((x) >>  8) & 0xFF)
#define LOGGER_GET_PHY_ID(x)        (((x) >> 16) & 0xFF)
#define LOGGER_GET_MAC_ID(x)        (((x) >> 24) & 0xFF)

#define LOGGER_MSG_INFO(mac, phy, src, dst) (((mac) << 24) | ((phy) << 16) | ((src) << 8) | ((dst)))

#define LOGGER_FLAG_OVERFLOW        (1 << 0)

#define LOGGER_CMD_NUM              34

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

typedef struct _LOG_INFO_
{
    LPVOID  Storage;        // The pointer to the storage
    UINT32  StorageSize;
    UINT32  DataSize;       // The number of bytes in the log (detected by code)
    UINT32  ApiNum;         // The number of commands in the log detected by the code
    UINT32  Mask;
    UINT32  Flags;

    UINT16  ApiStat[LOGGER_CMD_NUM];    // The commands statistics

}LOG_INFO;

typedef struct _STORAGE_HEADER_
{
    // THE DATA IS CACHE ALIGNED!!

    VUINT32      Version;        // Log version (for the host application and for the C-CODE)
    VUINT32      Flag;           // Status flags: see: LOGGER_FLAG_
    VUINT32      MsgNum;         // The number of messages written to the log
    VUINT32      PayloadSize;    // The size of data located in the log storage
    VUINT32      LogStorSize;    // The size of log storage    
    VUINT32      StartOffset;	 // offset from Phy Start command (always first in log)
    VUINT32      StartPhySize;
    VUINT32      Res;	         // Reserved

    // THE DATA IS CACHE ALIGNED!!
}STORAGE_HEADER;

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
}API_LOG_INFO;

typedef struct _APILOGGER_CTX_
{
    UINT32          EnabledMask;
    UINT32          EnabledMaskNmm;
    STORAGE_HEADER  StorageInfo;
    FILE            *pFile;
}APILOGGERCTX;


extern APILOGGERCTX  logger_ctx;

#define _SVSR_ASSERT_PTR(x)  if ((x) == NULL) {printf("SVSRLOGGER: [PTR ASSERT]\n");while (1);}
#define _SVSR_ASSERT_RC(x)  if ((x) == 0) {printf("SVSRLOGGER: [RC ASSERT]\n");while (1);}
#define _SVSR_ASSERT(x)  if ((x) == 0) {printf("SVSRLOGGER: [ASSERT]\n");while (1);}
#define FAILED(err)               ((err) & 0xFF)
#define ROUND(x, dx)  (((x) + ((dx) - 1) ) & ~((dx) - 1))

#define svsrPrintf(...) {printf (__VA_ARGS__);}


APILOGGERCTX * SvsrLoggerGetCtx (void);
MXRC SvsrLoggerInit (UINT32 nMask);
MXRC SvsrLoggerResetStorage(void);
MXRC SvsrLoggerCloseStorage(void);
void LoggerStorageBuildHeader (UINT32 nMsgID, UINT32 nMacPhySrcDstID, UINT32 nLen, API_LOG_INFO * pHeader);
MXRC LoggerStorageAddMsg(UINT32 nMsgID, UINT32 nMacPhySrcDstID, LPVOID pMsg, UINT32 nMsgSize);
MXRC SvsrLoggerAddMsgForce (UINT32 nMsgID, UINT32 nMacPhySrcDstID, LPVOID pMsg, UINT32 nMsgSize);
MXRC SvsrLoggerAddInit (UINT32 nMsgID, UINT32 nMacPhySrcDstID, LPVOID pMsg, UINT32 nMsgSize);
MXRC SvsrLoggerAddMsg (UINT32 nMsgID, UINT32 nMacPhySrcDstID, LPVOID pMsg, UINT32 nMsgSize);

#define SvsrLoggerIncrementCounter()
#define SvsrLoggerSetResetCounter(a)
#define SvsrLoggerSetResetMarker()
#endif  //_SVSR_LOGGER_H_

#ifdef __cplusplus
}
#endif

#endif
