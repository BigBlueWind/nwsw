//-------------------------------------------------------------------------------------------
/** @file logmgr.h
 *
 * @brief Logs manager, to create/open/control the log objects
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
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

#ifndef _LOGMGR_H_
#define _LOGMGR_H_

#include "4gmx.h"
#include "config.h"
#include "heaps.h"
#include "appids.h"

#define LOGMGR_RC_OK                MX_OK
#define LOGMGR_RC_CTX_ERROR   DEF_USRRC(APP_MODULE, LOG_MGR, 1)
#define LOGMGR_RC_NOT_INITED        DEF_USRRC(APP_MODULE, LOG_MGR, 2)
#define LOGMGR_RC_ALLOC_LOG_ERROR   DEF_USRRC(APP_MODULE, LOG_MGR, 3)
#define LOGMGR_RC_OVERFLOW          DEF_USRRC(APP_MODULE, LOG_MGR, 4)
#define LOGMGR_RC_UNKNOWN_ENGINE    DEF_USRRC(APP_MODULE, LOG_MGR, 5)
#define LOGMGR_RC_FREE_ERROR        DEF_USRRC(APP_MODULE, LOG_MGR, 6)
#define LOGMGR_RC_REG_ERROR         DEF_USRRC(APP_MODULE, LOG_MGR, 7)
#define LOGMGR_RC_ALREADY_INITED    DEF_USRRC(APP_MODULE, LOG_MGR, 8)

#define LOGMGR_MAX_NUM_HANDLE	50

#define LOG_VERSION         0x0100

#define LOG_CTRL_OPENED     (1 << 0)    // Do not use it directly!
#define LOG_CTRL_SYS_MDMA   (1 << 1)
#define LOG_CTRL_SPU_MDMA   (1 << 2)
#define LOG_CTRL_RAD_MDMA   (1 << 3)
#define LOG_CTRL_MEMCPY     (1 << 4)
#define LOG_CTRL_SYNCPU     (1 << 5)

#define LOG_FLAG_OVERFLOW   (1 << 0)

typedef struct _LOG_HANDLE_
{
    UINT32      Version;
	UINT32      Ctrl;	// See: LOG_CTRL_xxx
	HANDLE      hHeap;
	UINT32      Size;
	UINT32      CurOffs;
    PUINT8      Storage;
	UINT32      Flags;
    UINT32      Locked;
    UINT32      WrNum;      // Number of write operations done for this log

}LogHandleCtx;

typedef LogHandleCtx * LOGHANDLE;

typedef struct _LOGMGR_CTX_
{
	UINT32 nInit;   // Flag of system initialization
    UINT32 nLogNum; // The number of logs

    LOGHANDLE hLogHandArr[LOGMGR_MAX_NUM_HANDLE];

}LogMgrCtx;


/** @brief This function initializes log manager and should be called before usage of any logmgr API
    @return [MXRC] an error code */

MXRC LogMgrInit (void);

/** @brief This function returns the number of created/opened logs in the system
    @return [UINT32] an error code */

UINT32 LogMgrGetLogNum (void);

/** @brief This function returns the log handle
    @param nIndex [in] - the log index [0 ... LogMgrGetLogNum (void)-1]
    @return [LOGHANDLE] log handle */

LOGHANDLE LogMgrGetLog (UINT32 nIndex);

/** @brief This function creates the log and returns a log handle that should be used
           to control this log

    @param nCtrl    [in] - the log control flags, see: LOG_CTRL_xxx
    @param hHeap    [in] - the heap handler, this heap will be used to allocate the storage
    @param nSize    [in] - the reqired size of the storage
    @param hLog     [out]- the pointer to the log handle

    @return [MXRC] an error code */

MXRC LogMgrCreateLog (UINT32 nCtrl, HANDLE hHeap, UINT32 nSize, LOGHANDLE * hLog);

/** @brief This function opens preloaded log and returns the log handle
    @param pLogStorage [in] - the pointer to the log storage
    @param hLog        [out]- the pointer to the log handle

    @return [MXRC]  the error code */

MXRC LogMgrOpenLog (LPVOID pLogStorage, LOGHANDLE * hLog);

/** @brief The function writes data to the log

    @param hLog [in] - the log descriptor
    @param pData[in] - the pointer to the source data planned to add to the log
    @param nSize[in] - the size of source data in bytes

    @return [MXRC] an error code */

MXRC LogMgrWriteData(LOGHANDLE hLog, LPVOID pData, UINT32 nSize);

/** @brief The function writes data to the log

    @param hLog         [in] - the log descriptor
    @param nCopyEngine  [in] - the id of copy engine: SYS-MDMA, SPU-MDMA, RAD-MDMA, just memcpy, see: LOG_CTRL_xxx
    @param pData        [in] - the pointer to the source data planned to add to the log
    @param nSize        [in] - the size of source data in bytes

    @return [MXRC] an error code */

MXRC LogMgrWriteDataEx(LOGHANDLE hLog, UINT32 nCopyEngine, LPVOID pData, UINT32 nSize);

/** @brief The function reserves a storage in the log and returns pointers to this storage

    @param hLog         [in] - the log descriptor
    @param nSize        [in] - the size of data in bytes

    @return [LPVOID] the point to the reserved storage */

LPVOID LogMgrAllocSpace (LOGHANDLE hLog, UINT32 nSize);

/** @brief The function reads data from the log

    @param hLog     [in]      - the log descriptor
    @param pDstData [in]      - the pointer to the destination buffer
    @param nLogAddr [in]      - the log address, 0 based, upto logsize
    @param pnSize   [in/out] - [in]: the required size of data in bytes, [out]: read to destination buffer

    @return [MXRC] an error code */

MXRC LogMgrReadData(LOGHANDLE hLog, LPVOID pDstData, UINT32 nLogAddr, UINT32 * pnSize);

/** @brief This function returns the pointer to the log storage
    @param hLog [in] - the loghandle
    @return [LPVOID]*/
LPVOID LogMgrGetLogStorage (LOGHANDLE hLog);

/** @brief This function returns the pointer to the log header (header + storage)
    @param hLog [in] - the loghandle
    @return [LPVOID]*/
LPVOID LogMgrGetLogHead (LOGHANDLE hLog);

/** @brief This function returns total size of log storage
    @param hLog [in] - the loghandle
    @return [UINT32] the total size of log storage (not the data added the log) */
UINT32 LogMgrGetLogStorageSize (LOGHANDLE hLog);

/** @brief This function returns size of log data + size of log header
    @param hLog [in] - the loghandle
    @return [UINT32] This function returns size of log data + size of log header*/
UINT32 LogMgrGetLogHeadSize (LOGHANDLE hLog);

/** @brief This function returns size of data added to the storage
    @param hLog [in] - the loghandle
    @return [UINT32] the size of log data */

UINT32 LogMgrGetLogDataSize (LOGHANDLE hLog);

/** @brief This function flushes the data from the cache to memory
    @param hLog [in] - the log handle
    @return [MXRC] an error code */

MXRC LogMgrFlushLog (LOGHANDLE hLog);

/** @brief This function reloads (invalidates cache) data from the cache to memory
    @param hLog [in] - the log handle
    @return [MXRC] an error code */

MXRC LogMgrReloadLog (LOGHANDLE hLog);

/** @brief This function returns log status flags
    @param hLog [in] - the loghandle
    @return [UINT32] the log flags, see: LOG_FLAGS_xxx*/

UINT32 LogMgrGetLogFlags (LOGHANDLE hLog);

/** @brief This function returns size of data added to the storage
    @param hLog [in] - the log handle
    @return [UINT32] the size of log data */
MXRC LogMgrResetLog (LOGHANDLE hLog);

/** @brief This function returns number of write operations done for this log
    @param hLog [in] - the log handle
    @return [UINT32] write number */
UINT32 LogMgrGetWrNum (LOGHANDLE hLog);

/** @brief This function closes the log and frees data if log was created (not opened)
    @param hLog [in] - the log handle
    @return [MXRC] an error code */
MXRC LogMgrCloseLog (LOGHANDLE hLog);

/** @brief This function creates the log in an extern storage and returns a log handle that should be used
           to control this log

    @param nCtrl    [in] - the log control flags, see: LOG_CTRL_xxx
    @param pStorage [in] - the extern storage to the log
    @param nSize    [in] - the reqired size of the storage
    @param hLog     [out]- the pointer to the log handle

    @return [MXRC] an error code */
MXRC LogMgrCreateLogEx (UINT32 nCtrl, LPVOID pStorage, UINT32 nSize, LOGHANDLE * hLog);

#endif // _LOGMGR_H_

#ifdef __cplusplus
}
#endif

