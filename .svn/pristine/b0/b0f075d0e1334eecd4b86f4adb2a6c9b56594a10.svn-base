//-------------------------------------------------------------------------------------------
/** @file logmgr.c
 *
 * @brief Logs manager, to create/open/control the log objects
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "logmgr.h"
#include "mdmadrv.h"
#include "supervisor.h"

static LogMgrCtx logmgr_ctx;

INLINE LogMgrCtx * LogMgrGetCtx (void)
{
    return &logmgr_ctx;
}

/** @brief This function is used to find out unused place
           in array to register new one log descriptor,
           this is used only for statistics, so the performance
           is not critical in this case

    @param hLog [in] the log handler

    @return [MXRC] an error code

    *  \ingroup group_lte_logging

    */

MXRC LogMgrRegLogHandler (LOGHANDLE hLog)
{
    LogMgrCtx * pCtx = LogMgrGetCtx();
    UINT32 i;

    _ASSERT_PTR (pCtx);

    for (i = 0; i < LOGMGR_MAX_NUM_HANDLE; i++)
    {
        if (pCtx->hLogHandArr [i] == NULL)
        {
            pCtx->nLogNum ++;
            pCtx->hLogHandArr [i] = hLog;
            return LOGMGR_RC_OK;
        }
    }

    return LOGMGR_RC_REG_ERROR;
}

/** @brief This function is used to remove log handler from the array
           of created descriptors

    @param pLog [in] - the log handler
    @return [MXRC] .

    *  \ingroup group_lte_logging

    */

MXRC LogMgrUnregLogHandler (LOGHANDLE pLog)
{
    LogMgrCtx * pCtx = LogMgrGetCtx();
    UINT32 i;

    _ASSERT_PTR (pCtx);

    for (i = 0; i < LOGMGR_MAX_NUM_HANDLE; i++)
    {
        if (pCtx->hLogHandArr [i] == pLog)
        {
            pCtx->nLogNum --;
            pCtx->hLogHandArr [i] = NULL;
            return LOGMGR_RC_OK;
        }
    }

    return LOGMGR_RC_FREE_ERROR;
}

/** @brief This function initializes log manager and should be called before usage of any logmgr API
    @return [MXRC] an error code

    *  \ingroup group_lte_logging

    */

MXRC LogMgrInit (void)
{
    LogMgrCtx * pCtx = LogMgrGetCtx();
    MXRC rc = LOGMGR_RC_OK;

    _ASSERT_PTR(pCtx);

    if (pCtx->nInit != 0)
        return LOGMGR_RC_ALREADY_INITED;

    memset (pCtx, 0, sizeof (*pCtx));

    pCtx->nInit = 1;

    return rc;
}

/** @brief This function returns the number of created/opened logs in the system
    @return [UINT32] an error code

    *  \ingroup group_lte_logging

    */

UINT32 LogMgrGetLogNum (void)
{
    LogMgrCtx * pCtx = LogMgrGetCtx();

    _ASSERT_PTR(pCtx);

    return pCtx->nLogNum;
}

/** @brief This function returns the log handle
    @param nIndex [in] - the log index [0 ... LogMgrGetLogNum (void)-1]
    @return [LOGHANDLE] log handle

    *  \ingroup group_lte_logging

    */

LOGHANDLE LogMgrGetLog (UINT32 nIndex)
{
    LogMgrCtx * pCtx = LogMgrGetCtx();
    UINT32 i, nCurIndex=0;

    _ASSERT_PTR(pCtx);

    for (i = 0; i < LOGMGR_MAX_NUM_HANDLE; i++)
    {
        if (pCtx->hLogHandArr [i] != NULL)
        {
            if (nCurIndex == nIndex)
                return pCtx->hLogHandArr [i];

            nCurIndex++;
        }
    }

    return NULL;
}

/** @brief This function creates the log and returns a log handle that should be used
           to control this log

    @param nCtrl    [in] - the log control flags, see: LOG_CTRL_xxx
    @param hHeap    [in] - the heap handler, this heap will be used to allocate the storage
    @param nSize    [in] - the reqired size of the storage
    @param hLog     [out]- the pointer to the log handle

    @return [MXRC] an error code

    *  \ingroup group_lte_logging

    */

MXRC LogMgrCreateLog (UINT32 nCtrl, HANDLE hHeap, UINT32 nSize, LOGHANDLE * hLog)
{
    LogHandleCtx * pLog;

    pLog = (LogHandleCtx *)MxHeapAlloc (hHeap, ROUND(sizeof (LogHandleCtx), 32) + nSize);

    if (pLog == NULL)
        return LOGMGR_RC_ALLOC_LOG_ERROR;

    memset (pLog, 0, sizeof (LogHandleCtx));

    pLog->Version  = LOG_VERSION;
    pLog->Ctrl     = nCtrl;
    pLog->CurOffs  = 0;
    pLog->Storage  = (PUINT8)((UINT32)pLog + ROUND(sizeof(LogHandleCtx), 32));
    pLog->Flags    = 0;
    pLog->hHeap    = hHeap;
    pLog->Size     = nSize;

    *hLog = pLog;

    return LogMgrRegLogHandler (pLog);
}

/** @brief This function creates the log in an extern storage and returns a log handle that should be used
           to control this log

    @param nCtrl    [in] - the log control flags, see: LOG_CTRL_xxx
    @param pStorage [in] - the extern storage to the log
    @param nSize    [in] - the reqired size of the storage
    @param hLog     [out]- the pointer to the log handle

    @return [MXRC] an error code

    *  \ingroup group_lte_logging

    */

MXRC LogMgrCreateLogEx (UINT32 nCtrl, LPVOID pStorage, UINT32 nSize, LOGHANDLE * hLog)
{
    LogHandleCtx * pLog;

    pLog = (LogHandleCtx *)pStorage;

    if (pLog == NULL)
        return LOGMGR_RC_ALLOC_LOG_ERROR;

    pLog->Version  = LOG_VERSION;
    pLog->Ctrl     = nCtrl;
    pLog->CurOffs  = 0;
    pLog->Storage  = (PUINT8)((UINT32)pStorage + ROUND(sizeof(LogHandleCtx), 32));
    pLog->Flags    = 0;
    pLog->hHeap    = HNULL;
    pLog->Size     = nSize;

    *hLog = pLog;

    return LogMgrRegLogHandler (pLog);
}

/** @brief This function opens preloaded log and returns the log handle
    @param pLogStorage [in] - the pointer to the log storage
    @param hLog        [out]- the pointer to the log handle

    @return [MXRC]  the error code

    *  \ingroup group_lte_logging

    */

MXRC LogMgrOpenLog (LPVOID pLogStorage, LOGHANDLE * hLog)
{
    LogHandleCtx * pLog;

    pLog = (LogHandleCtx *)pLogStorage;

    if (pLog == NULL)
        return LOGMGR_RC_ALLOC_LOG_ERROR;

    pLog->Ctrl     |= (LOG_CTRL_OPENED);
    pLog->Storage  = (PUINT8)((UINT32)pLog + ROUND(sizeof(LogHandleCtx), 32));
    pLog->hHeap    = HNULL;

    *hLog = pLog;

    return LogMgrRegLogHandler (pLog);
}

/** @brief The function writes data to the log

    @param hLog [in] - the log descriptor
    @param pData[in] - the pointer to the source data planned to add to the log
    @param nSize[in] - the size of source data in bytes

    @return [MXRC] an error code

    *  \ingroup group_lte_logging

    */

MXRC LogMgrWriteData(LOGHANDLE hLog, LPVOID pData, UINT32 nSize)
{
    MXRC rc = LOGMGR_RC_OK;
    IRQTYPE irq = 0;

    if (hLog->Ctrl & LOG_CTRL_SYNCPU)
    {
        irq = ARM_INT_disable();
        MxGetLock(&hLog->Locked);
    }

    _ASSERT_PTR(hLog);
    _ASSERT_PTR(pData);

    if (nSize == 0)
    {
        if (hLog->Ctrl & LOG_CTRL_SYNCPU)
        {
            MxReleaseLock(&hLog->Locked);
            ARM_INT_restore(irq);
        }

        return LOGMGR_RC_OK;
    }

    if ((hLog->CurOffs + nSize) > hLog->Size)
    {
        hLog->Flags |= LOG_FLAG_OVERFLOW;

        if (hLog->Ctrl & LOG_CTRL_SYNCPU)
        {
            MxReleaseLock(&hLog->Locked);
            ARM_INT_restore(irq);
        }

        return LOGMGR_RC_OVERFLOW;
    }

    if (hLog->Ctrl & LOG_CTRL_MEMCPY)
    {
        memcpy (hLog->Storage + hLog->CurOffs, pData, nSize);
        hLog->CurOffs += nSize;
    }
    else if (hLog->Ctrl & LOG_CTRL_SYS_MDMA)
    {
        rc = MdmaCopy(MDMA_HW_SYS, hLog->Storage + hLog->CurOffs, pData, nSize);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);

            if (hLog->Ctrl & LOG_CTRL_SYNCPU)
            {
                MxReleaseLock(&hLog->Locked);
                ARM_INT_restore(irq);
            }

            return rc;
        }

        // cache clean is not needed,
        // header is another cache line
        hLog->CurOffs += nSize;
    }
    else if (hLog->Ctrl & LOG_CTRL_SPU_MDMA)
    {
        rc = MdmaCopy (MDMA_HW_SPU, hLog->Storage + hLog->CurOffs, pData, nSize);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);

            if (hLog->Ctrl & LOG_CTRL_SYNCPU)
            {
                MxReleaseLock(&hLog->Locked);
                ARM_INT_restore(irq);
            }

            return rc;
        }

        // cache clean is not needed,
        // header is another cache line
        hLog->CurOffs += nSize;
    }
    else if (hLog->Ctrl & LOG_CTRL_RAD_MDMA)
    {
        rc = MdmaCopy (MDMA_HW_RAD, hLog->Storage + hLog->CurOffs, pData, nSize);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);

            if (hLog->Ctrl & LOG_CTRL_SYNCPU)
            {
                MxReleaseLock(&hLog->Locked);
                ARM_INT_restore(irq);
            }

            return rc;
        }

        // cache clean is not needed,
        // header is another cache line
        hLog->CurOffs += nSize;
    }
    else
    {
        rc = LOGMGR_RC_UNKNOWN_ENGINE;
    }

    _ASSERT_RC(rc);

    hLog->WrNum ++;

    if (hLog->Ctrl & LOG_CTRL_SYNCPU)
    {
        MxReleaseLock(&hLog->Locked);
        ARM_INT_restore(irq);
    }

    return rc;
}

/** @brief The function writes data to the log

    @param hLog         [in] - the log descriptor
    @param nCopyEngine  [in] - the id of copy engine: SYS-MDMA, SPU-MDMA, RAD-MDMA, just memcpy, see: LOG_CTRL_xxx
    @param pData        [in] - the pointer to the source data planned to add to the log
    @param nSize        [in] - the size of source data in bytes

    @return [MXRC] an error code

    *  \ingroup group_lte_logging

    */

MXRC LogMgrWriteDataEx(LOGHANDLE hLog, UINT32 nCopyEngine, LPVOID pData, UINT32 nSize)
{
    MXRC rc = LOGMGR_RC_OK;
    IRQTYPE irq = 0;

    if (hLog->Ctrl & LOG_CTRL_SYNCPU)
    {
        irq = ARM_INT_disable();
        MxGetLock(&hLog->Locked);
    }

    _ASSERT_PTR(hLog);
    _ASSERT_PTR(pData);

    if (nSize == 0)
    {
        if (hLog->Ctrl & LOG_CTRL_SYNCPU)
        {
            MxReleaseLock(&hLog->Locked);
            ARM_INT_restore(irq);
        }

        return LOGMGR_RC_OK;
    }

    if ((hLog->CurOffs + nSize) > hLog->Size)
    {
        hLog->Flags |= LOG_FLAG_OVERFLOW;

        if (hLog->Ctrl & LOG_CTRL_SYNCPU)
        {
            MxReleaseLock(&hLog->Locked);
            ARM_INT_restore(irq);
        }

        return LOGMGR_RC_OVERFLOW;
    }

    if (nCopyEngine & LOG_CTRL_MEMCPY)
    {
        memcpy (hLog->Storage + hLog->CurOffs, pData, nSize);
        hLog->CurOffs += nSize;
    }
    else if (nCopyEngine & LOG_CTRL_SYS_MDMA)
    {
        rc = MdmaCopy(MDMA_HW_SYS, hLog->Storage + hLog->CurOffs, pData, nSize);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);

            if (hLog->Ctrl & LOG_CTRL_SYNCPU)
            {
                MxReleaseLock(&hLog->Locked);
                ARM_INT_restore(irq);
            }

            return rc;
        }

        // cache clean is not needed,
        // header is another cache line
        hLog->CurOffs += nSize;
    }
    else if (nCopyEngine & LOG_CTRL_SPU_MDMA)
    {
        rc = MdmaCopy (MDMA_HW_SPU, hLog->Storage + hLog->CurOffs, pData, nSize);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);

            if (hLog->Ctrl & LOG_CTRL_SYNCPU)
            {
                MxReleaseLock(&hLog->Locked);
                ARM_INT_restore(irq);
            }

            return rc;
        }

        // cache clean is not needed,
        // header is another cache line
        hLog->CurOffs += nSize;
    }
    else if (nCopyEngine & LOG_CTRL_RAD_MDMA)
    {
        rc = MdmaCopy (MDMA_HW_RAD, hLog->Storage + hLog->CurOffs, pData, nSize);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);

            if (hLog->Ctrl & LOG_CTRL_SYNCPU)
            {
                MxReleaseLock(&hLog->Locked);
                ARM_INT_restore(irq);
            }

            return rc;
        }

        // cache clean is not needed,
        // header is another cache line
        hLog->CurOffs += nSize;
    }
    else
    {
        rc = LOGMGR_RC_UNKNOWN_ENGINE;
    }

    _ASSERT_RC(rc);

    hLog->WrNum ++;

    if (hLog->Ctrl & LOG_CTRL_SYNCPU)
    {
        MxReleaseLock(&hLog->Locked);
        ARM_INT_restore(irq);
    }

    return rc;
}

/** @brief The function reserves a storage in the log and returns pointers to this storage

    @param hLog         [in] - the log descriptor
    @param nSize        [in] - the size of data in bytes

    @return [LPVOID] the point to the reserved storage

    \ingroup group_lte_logging
    */

LPVOID LogMgrAllocSpace (LOGHANDLE hLog, UINT32 nSize)
{
    IRQTYPE irq = 0;
    LPVOID pData = NULL;

    if (nSize == 0)
        return pData;

    _ASSERT_PTR(hLog);

    if (hLog->Ctrl & LOG_CTRL_SYNCPU)
    {
        irq = ARM_INT_disable();
        MxGetLock(&hLog->Locked);
    }

    if (hLog->CurOffs + nSize <= hLog->Size)
    {
        pData = (hLog->Storage + hLog->CurOffs);
        hLog->CurOffs += nSize;
        hLog->WrNum ++;
    }

    if (hLog->Ctrl & LOG_CTRL_SYNCPU)
    {
        MxReleaseLock(&hLog->Locked);
        ARM_INT_restore(irq);
    }

    return pData;
}

/** @brief The function reads data from the log

    @param hLog     [in]      - the log descriptor
    @param pDstData [in]      - the pointer to the destination buffer
    @param nLogAddr [in]      - the log address, 0 based, upto logsize
    @param pnSize   [in/out] - [in]: the required size of data in bytes, [out]: read to destination buffer

    @return [MXRC] an error code

    *  \ingroup group_lte_logging

    */

MXRC LogMgrReadData(LOGHANDLE hLog, LPVOID pDstData, UINT32 nLogAddr, UINT32 * pnSize)
{
    UINT32 nRealSize;
    IRQTYPE irq = 0;

    if (hLog->Ctrl & LOG_CTRL_SYNCPU)
    {
        irq = ARM_INT_disable();
        MxGetLock(&hLog->Locked);
    }

    _ASSERT_PTR(hLog);
    _ASSERT_PTR(pDstData);
    _ASSERT_PTR(pnSize);

    if (nLogAddr < hLog->CurOffs)
    {
        nRealSize = *pnSize;

        if (nLogAddr + nRealSize > hLog->CurOffs)
            nRealSize = hLog->CurOffs - nLogAddr;

            memcpy (pDstData, hLog->Storage + nLogAddr, nRealSize);
    }
    else
    {
        nRealSize = 0;
    }

    *pnSize = nRealSize;

    if (hLog->Ctrl & LOG_CTRL_SYNCPU)
    {
        MxReleaseLock(&hLog->Locked);
        ARM_INT_restore(irq);
    }

    return LOGMGR_RC_OK;
}

/** @brief This function returns the pointer to the log storage
    @param hLog [in] - the loghandle
    @return [LPVOID]

    *  \ingroup group_lte_logging

    */

LPVOID LogMgrGetLogStorage (LOGHANDLE hLog)
{
    _ASSERT_PTR(hLog);
    return hLog->Storage;
}

/** @brief This function returns the pointer to the log header (header + storage)
    @param hLog [in] - the loghandle
    @return [LPVOID]

    *  \ingroup group_lte_logging

    */
LPVOID LogMgrGetLogHead (LOGHANDLE hLog)
{
    _ASSERT_PTR(hLog);
    return hLog;
}

/** @brief This function returns total size of log storage
    @param hLog [in] - the loghandle
    @return [UINT32] the total size of log storage (not the data added the log)

    *  \ingroup group_lte_logging

    */

UINT32 LogMgrGetLogStorageSize (LOGHANDLE hLog)
{
    _ASSERT_PTR(hLog);
    return hLog->Size;
}

/** @brief This function returns size of log data + size of log header
    @param hLog [in] - the loghandle
    @return [UINT32] This function returns size of log data + size of log header

    *  \ingroup group_lte_logging

    */
UINT32 LogMgrGetLogHeadSize (LOGHANDLE hLog)
{
    _ASSERT_PTR(hLog);
    return hLog->CurOffs + sizeof (LogHandleCtx);
}

/** @brief This function returns size of data added to the storage
    @param hLog [in] - the loghandle
    @return [UINT32] the size of log data

    *  \ingroup group_lte_logging

    */

UINT32 LogMgrGetLogDataSize (LOGHANDLE hLog)
{
    _ASSERT_PTR(hLog);
    return hLog->CurOffs;
}

/** @brief This function flushes the data from the cache to memory
    @param hLog [in] - the log handle
    @return [MXRC] an error code

    *  \ingroup group_lte_logging

    */

MXRC LogMgrFlushLog (LOGHANDLE hLog)
{
    _ASSERT_PTR(hLog);
    _ASSERT_PTR(hLog->Storage);
    _ASSERT(hLog->Size != 0);

    MxCacheClean(hLog->Storage, hLog->Size);

    return LOGMGR_RC_OK;
}

/** @brief This function reloads (invalidates cache) data from the cache to memory
    @param hLog [in] - the log handle
    @return [MXRC] an error code

    *  \ingroup group_lte_logging

    */

MXRC LogMgrReloadLog (LOGHANDLE hLog)
{
    _ASSERT_PTR(hLog);
    _ASSERT_PTR(hLog->Storage);
    _ASSERT(hLog->Size != 0);

    MxCacheInvalidate(hLog->Storage, hLog->Size);

    return LOGMGR_RC_OK;
}

/** @brief This function returns log status flags
    @param hLog [in] - the loghandle
    @return [UINT32] the log flags, see: LOG_FLAGS_xxx

    *  \ingroup group_lte_logging

    */

UINT32 LogMgrGetLogFlags (LOGHANDLE hLog)
{
    _ASSERT_PTR(hLog);
    return hLog->Flags;
}

/** @brief This function resets the log buffer state to start
    @param hLog [in] - the log handle
    @return [UINT32] the size of log data

    *  \ingroup group_lte_logging

    */
MXRC LogMgrResetLog (LOGHANDLE hLog)
{
    IRQTYPE irq = 0;

    if (hLog->Ctrl & LOG_CTRL_SYNCPU)
    {
        irq = ARM_INT_disable();
        MxGetLock(&hLog->Locked);
    }

    _ASSERT_PTR(hLog);

    hLog->CurOffs = 0;
    hLog->Flags = 0;
    hLog->WrNum = 0;

    if (hLog->Ctrl & LOG_CTRL_SYNCPU)
    {
        MxReleaseLock(&hLog->Locked);
        ARM_INT_restore(irq);
    }

    return LOGMGR_RC_OK;
}


/** @brief This function returns number of write operations done for this log
    @param hLog [in] - the log handle
    @return [UINT32] write number

    *  \ingroup group_lte_logging

    */
UINT32 LogMgrGetWrNum (LOGHANDLE hLog)
{
    _ASSERT_PTR(hLog);

    return hLog->WrNum;
}

/** @brief This function closes the log and frees data if log was created (not opened)
    @param hLog [in] - the log handle
    @return [MXRC] an error code

    *  \ingroup group_lte_logging

    */
MXRC LogMgrCloseLog (LOGHANDLE hLog)
{
    // only for memcpy function we need to flush data
    // in case of MDMA we do not need to do it

    if ((hLog->Ctrl & LOG_CTRL_MEMCPY) != 0)
        LogMgrFlushLog (hLog);

    // to free a storage, if the storage was allocated
    if ((hLog->Ctrl & LOG_CTRL_OPENED) == 0 && (hLog->Storage != NULL))
    {
        if (HANDLE_NOT_NULL(hLog->hHeap))
            MxHeapFree (hLog->hHeap, (LPVOID)(hLog));
    }

    // to remove the log handle from the list
    // of registered objects
    return LogMgrUnregLogHandler(hLog);
}

