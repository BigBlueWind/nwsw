//-------------------------------------------------------------------------------------------
/** @file svsrlogger.h
 *
 * @brief The declaration of supervisor logger API
 * @author Mindspeed Technologies
 * @version $Revision: 1.30 $
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

#ifndef _SVSR_LOGGER_H_
#define _SVSR_LOGGER_H_

#include "config.h"

#include "4gmx.h"
#include "appids.h"
#include "svsrloggerstorage.h"

//#define LOGGER_TIME_MEASUREMENT_ENABLED

#define RC_LOGGER_ALLOC_ERROR       DEF_USRRC(APP_MODULE, APILOGGER, 1)
#define RC_LOGGER_OVERFLOW          DEF_USRRC(APP_MODULE, APILOGGER, 2)
#define RC_LOGGER_UNKNOWN_API       DEF_USRRC(APP_MODULE, APILOGGER, 3)
#define RC_LOGGER_API_ERROR         DEF_USRRC(APP_MODULE, APILOGGER, 4)
#define RC_LOGGER_HEADER_NULL       DEF_USRRC(APP_MODULE, APILOGGER, 5)
#define RC_LOGGER_CTX_NULL          DEF_USRRC(APP_MODULE, APILOGGER, 6)
#define RC_LOGGER_STORAGE_NULL      DEF_USRRC(APP_MODULE, APILOGGER, 7)

#define LOGGER_DEF_MASK             0x0

#define LOGGER_GET_DST_ID(x)        (((x) >>  0) & 0xFF)
#define LOGGER_GET_SRC_ID(x)        (((x) >>  8) & 0xFF)
#define LOGGER_GET_PHY_ID(x)        (((x) >> 16) & 0xFF)
#define LOGGER_GET_MAC_ID(x)        (((x) >> 24) & 0xFF)

#define LOGGER_MSG_INFO(mac, phy, src, dst) (((mac) << 24) | ((phy) << 16) | ((src) << 8) | ((dst)))

#define LOGGER_FLAG_OVERFLOW        (1 << 0)

#define LOGGER_CMD_NUM              34

#define LOGGER_MAX_LOGGING_SPACE   1000000 // Ticks (Some arbitrary large Tick Counter)

typedef struct _APILOGGER_CTX_
{
    UINT32      EnabledMask;
    UINT32      EnabledMaskNmm;
    pSTORAGE_CTX  StoragePtr;
    VUINT32     ResetMarker;
    VUINT32     ResetMsgNum;
    UINT32      StorageSize;
    VUINT32     Lock;
    VUINT32     Irq[CPU_NUM];
    VUINT32     Resetcounter;
    VUINT32     Currentresetcounter;
    VUINT32     DisableLogging;  // Used for APIPlayer Mode

#ifdef LOGGER_TIME_MEASUREMENT_ENABLED
    UINT32      MaxTime[LOGGER_CMD_NUM];
    UINT32      AvgTime[LOGGER_CMD_NUM];
    UINT32      MinTime[LOGGER_CMD_NUM];
    UINT32      MsgSize[LOGGER_CMD_NUM];
#endif

}APILOGGERCTX;

/** @brief This structure is placed at the beginning of the log
           and can be used to read log information */

typedef struct _LOG_INFO_
{
    LPVOID  Storage;        // The pointer to the storage
    UINT32  StorageSize;
    UINT32  DataSize;       // The number of bytes in the log (detected by code)
    UINT32  ApiNum;         // The number of commands in the log detected by the code
    UINT32  FirstSF;        // First subframe index in the log
    UINT32  LastSF;         // Last subframe index in the log
    UINT32  TotalSF;        // Total quantity of subframes in the log
    UINT32  Mask;
    UINT32  Flags;
    UINT32  ResetMarker;
    UINT32  ResetCouter, CurrentResetCounter;
    UINT16  ApiStat[LOGGER_CMD_NUM];    // The commands statistics

#ifdef LOGGER_TIME_MEASUREMENT_ENABLED
    UINT32  MaxTime[LOGGER_CMD_NUM];
    UINT32  AvgTime[LOGGER_CMD_NUM];
    UINT32  MinTime[LOGGER_CMD_NUM];
    UINT32  MsgSize[LOGGER_CMD_NUM];
#endif

}LOG_INFO;

// Total number of traced elements in circle
#define SVSRLOG_TRACE_BUF_ELEM_NUM 500
// The number of last traced elements to be printed out on some event
#define SVSRLOG_TRACE_BUF_ELEM_PRINT 200

typedef struct _SVSRLOG_TRACE_BUF_ELEM
{
    UINT32 data[10];
}SVSRLOG_TRACE_BUF_ELEM;

extern APILOGGERCTX  logger_ctx;

BOOL SvsrLogIsCopyCompleted(void);
MXRC SvsrLoggerEntry (void);
MXRC SvsrLoggerInit (UINT32 nMask, UINT32 nLogSize);
MXRC SvsrLoggerSetMask (UINT32 nMask);
MXRC SvsrLoggerSetMaskNmm (UINT32 nMask);

UINT32 SvsrLoggerGetMask (void);
UINT32 SvsrLoggerGetMaskNmm (void);

MXRC SvsrLoggerDisable (void);
MXRC SvsrLoggerEnable (void);

UINT32 SvsrLoggerGetFlag (void);
MXRC SvsrLoggerAddInit  (UINT32 nMsgID, UINT32 nMacPhySrcDstID, LPVOID pMsg, UINT32 nMsgSize);
MXRC SvsrLoggerAddMsg   (UINT32 nMsgID, UINT32 nMacPhySrcDstID, LPVOID pMsg, UINT32 nMsgSize);
MXRC SvsrLoggerAddMsgForce (UINT32 nMsgID, UINT32 nMacPhySrcDstID, LPVOID pMsg, UINT32 nMsgSize);

UINT32 SvsrLoggerGetMsgNum (void);

API_LOG_INFO * SvsrLoggerGetMsg (UINT32 nMsgIndex);
API_LOG_INFO * SvsrLoggerGetNextMsg (UINT32 * pnMsg);

INLINE PUINT8 SvsrGetApiPayload(API_LOG_INFO * pHdr)
{
    return ((PUINT8)pHdr) + sizeof (API_LOG_INFO);
}

MXRC SvsrLoggerFlush (void);
MXRC SvsrLoggerRestore(UINT32 bInvCache);
MXRC SvsrLoggerGetInfo (LOG_INFO * pInfo);
MXRC SvsrLoggerPrintInfo (void);
MXRC SvsrLoggerResetStorage(unsigned int lockState);

UINT32 SvsrGetSize (void);

LPVOID SvsrLoggerGetLocation (void);
UINT32 SvsrLoggerGetSize (void);
UINT32 SvsrLoggerGetStorageSize (void);

UINT32 SvsrLoggerIncrementCounter(void);
UINT32 SvsrLoggerSetTTICycleLimit(unsigned int Ctr);
void SvsrLoggerSetResetMarker(void);

UINT32 SvsrLoggerInitTraceBuf(UINT32 ElementsNum);
UINT32 SvsrLoggerAddTraceBuf(UINT32 data1, UINT32 data2, UINT32 data3, UINT32 data4, UINT32 data5, UINT32 data6,
    UINT32 data7, UINT32 data8, UINT32 data9, UINT32 data10);
void SvsrLoggerPrintTraceBuf(void);
LPVOID SvsrLoggerGetStorageCtx (void);



#endif  //_SVSR_LOGGER_H_

#ifdef __cplusplus
}
#endif
