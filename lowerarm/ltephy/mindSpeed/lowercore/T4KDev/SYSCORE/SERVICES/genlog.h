//-------------------------------------------------------------------------------------------
/** @file genlog.h
 *
 * @brief General application log, it's designed to store any
*       application information
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
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

#ifndef _GEN_LOGGER_H_
#define _GEN_LOGGER_H_

#include "4gmx.h"
#include "config.h"
#include "heaps.h"
#include "appids.h"
#include "logmgr.h"

#define RC_GENLOG_PARAM_ERROR       DEF_USRRC(APP_MODULE, GENLOG, 1)
#define RC_GENLOG_ALLOC_ERROR       DEF_USRRC(APP_MODULE, GENLOG, 2)
#define RC_GENLOG_WRITE_ERROR       DEF_USRRC(APP_MODULE, GENLOG, 3)
#define RC_GENLOG_NO_MORE_ITEMS     DEF_USRRC(APP_MODULE, GENLOG, 4)
#define RC_GENLOG_UNKNOWN_API       DEF_USRRC(APP_MODULE, GENLOG, 5)

typedef struct _GEN_LOG_CTX_
{
    LOGHANDLE   hLog;
    UINT32      Mask [8];   // 256 bits for mask
    UINT32      Lock;

}GenLogCtx;


typedef struct _GEN_LOG_RECORD_
{
    UINT32 nSize;   // The size of the data block in bytes without this header
    UINT32 ID;      // The data ID (data semantics)
    UINT32 Descr;   // The 32 bits descriptor, it's application depended value
    UINT32 Time;    // The timestamp for this record
    UINT32 Res[4];

}GenLogRecType, *PGenLogRecType;
#define GEN_LOG_EFF_REC_SIZE (4*sizeof (UINT32))        // NOTE!!!

/** @brief This function is an entry point of this module and called
           automatically from servmgr

    @return [MXRC] an error code
    \ingroup group_lte_logging
    */

MXRC GenLogEntry (void);

/** @brief This function adds data to the log

    @param nDataID      [in] - the data id, it's used in a log mask and to detect data
    @param nDataDescr   [in] - the data descriptor, any U32 value that will be assigned to data (application depended, can be 0)
    @param pData        [in] - the pointer to the data buffer
    @param nDataSize    [in] - the data size in bytes

    @return [MXRC] an error code
    \ingroup group_lte_logging
    */

MXRC GenLogAddData (UINT32 nDataID, UINT32 nDataDescr, LPVOID pData, UINT32 nDataSize);

/** @brief This function scans log and returns information about message record

    @param pMsgPos [in/out] - in: the message possition to start scanning
                             out: the position for the further scanning

    @param pRec [out] - the pointer to the storage of address to log record

    @return [MXRC] an error code
    \ingroup group_lte_logging
    */

MXRC GenLogScan (UINT32 * pMsgPos, PGenLogRecType * pRec);

/** @brif It returns pointer to the log storage: [HEADER] [rec#0]...[rec#N]
    @return [MXRC] pointer
    \ingroup group_lte_logging
    */
LPVOID GenLogGetLogStorage (void);

/** @brif It returns size of data in log in bytes
    @return [MXRC] pointer
    \ingroup group_lte_logging
    */
MXRC GenLogGetLogStorageSize (void);

/** @brif It returns size log flags (status of overwrite, etc...)
    @return [MXRC] flags
    \ingroup group_lte_logging
    */
UINT32 GenLogGetLogFlags (void);

/** @brief This code flushes log storage if log is in cachable memory
    @return [MXRC] an error code
    \ingroup group_lte_logging
    */
MXRC GenLogFlush (void);

/** @brief This function returns number of messages stored in the log
    @return [UINT32] the number of messages
    \ingroup group_lte_logging
    */
UINT32 GenLogGetMsgNum (void);

/** @brief This function enables/disables logging of message
    @return [UINT32] the number of messages
    \ingroup group_lte_logging
    */
MXRC GenLogEnableMsg (UINT32 nMsgID, UINT32 nEnb);

/** @brief This function clean log and makes number of log messages equal to 0
    @return [MXRC] an error code
    \ingroup group_lte_logging
    */
MXRC GenLogReset(void);

#endif // _GEN_LOGGER_H_

#ifdef __cplusplus
}
#endif

