//-------------------------------------------------------------------------------------------
/** @file genlog.c
 *
 * @brief General application log, it's designed to store any
*       application information
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "genlog.h"
#include "appprintf.h"
#include "supervisor.h"
#include "larmmsgid.h"
#include "mdmadrv.h"

GenLogCtx gen_ctx;

INLINE GenLogCtx * GenLogGetCtx (void)
{
    return &gen_ctx;
}

MXRC GenLogCfg (GenLogCtx * pCtx, ApiHeader * pApi)
{
    ApiParam * pParam = ApiGetParamByID (pApi, 2);
    UINT32 i;

    if (pParam == NULL)
        return RC_GENLOG_PARAM_ERROR;

    i = 0;

    while (i < (pParam->ParamLength / sizeof (UINT32)))
    {
        pCtx->Mask [i] = pParam->ParamData[i];
        i++;
    }

    return MX_OK;
}

MXRC GenLogWriteInfo (GenLogCtx * pCtx, ApiHeader * pResp)
{
    UINT32 data;

    data = (UINT32)LogMgrGetLogHead (pCtx->hLog);
    ApiAddParam(pResp, NULL, 2, &data, sizeof (data));

    data = (UINT32)LogMgrGetLogHeadSize(pCtx->hLog);
    ApiAddParam(pResp, NULL, 3, &data, sizeof (data));

    data = (UINT32)LogMgrGetWrNum(pCtx->hLog);
    ApiAddParam(pResp, NULL, 4, &data, sizeof (data));

    ApiAddParam(pResp, NULL, 5, &pCtx->Mask[0], sizeof (pCtx->Mask));

    return MX_OK;
}

APIHEC GenLogApihandler (LPVOID lpData, ApiHeader * pApi, ApiHeader * pResp)
{
    GenLogCtx * pCtx = GenLogGetCtx();
    MXRC rc = RC_GENLOG_UNKNOWN_API;

    _ASSERT_PTR (pCtx);

    if (IS_CFG_API(pApi, GEN_LOG_CFG))
    {
        rc = GenLogCfg (pCtx, pApi);
    }

    if (IS_QRY_API(pApi, GEN_LOG_GET_INFO))
    {
        rc = GenLogWriteInfo (pCtx, pResp);
    }

    if (IS_CFG_API(pApi, GEN_LOG_RESET))
    {
        rc = MX_OK;
        LogMgrResetLog (pCtx->hLog);
    }

    if (IS_CFG_API(pApi, GEN_LOG_FLUSH))
    {
        GenLogFlush ();
    }

    ApiAddErrorCode(pResp, rc);

    return APIHEC_OK;
}

/** @brief This function is an entry point of this module and called
           automatically from servmgr

    @return [MXRC] an error code

    *  \ingroup group_lte_logging

    */

MXRC GenLogEntry (void)
{
    GenLogCtx * pCtx = GenLogGetCtx();
    MXRC rc;

    _ASSERT_PTR(pCtx);

    memset (pCtx, 0, sizeof (*pCtx));

    rc = LogMgrCreateLog(GEN_LOG_OPTIONS, GEN_LOG_HEAP, GEN_LOG_BUF_SIZE, &pCtx->hLog);

    if (FAILED(rc))
    {
        uart_printf ("GEN-LOG: allocation storage error!, rc=0x%08x\r\n", rc);
        return rc;
    }

    rc = SvsrRegClient (IID_GEN_LOGGER, GenLogApihandler, pCtx);

    if (FAILED(rc))
    {
        uart_printf ("GEN-LOG: svsr registering error, rc=0x%08x\r\n", rc);
        return rc;
    }

    //uart_printf (" GEN-LOGGER STORAGE PTR: 0x%08x\r\n", LogMgrGetLogHead (pCtx->hLog));
    //uart_printf ("GEN-LOGGER STORAGE SIZE: 0x%08x\r\n", LogMgrGetLogStorageSize(pCtx->hLog));

    return MX_OK;
}

/** @brief This function adds data to the log

    @param nDataID      [in] - the data id, it's used in a log mask and to detect data
    @param nDataDescr   [in] - the data descriptor, any U32 value that will be assigned to data (application depended, can be 0)
    @param pData        [in] - the pointer to the data buffer
    @param nDataSize    [in] - the data size in bytes

    @return [MXRC] an error code

    *  \ingroup group_lte_logging

    */

MXRC GenLogAddData (UINT32 nDataID, UINT32 nDataDescr, LPVOID pData, UINT32 nDataSize)
{
    GenLogCtx * pCtx = GenLogGetCtx();
    PGenLogRecType pLogRec;
    MXRC rc = 0;

#if 0
    GenLogRecType CacheRec;
#endif

    _ASSERT_PTR(pCtx);
    _ASSERT (nDataID < sizeof (pCtx->Mask) * 8);

    if ((pCtx->Mask [nDataID >> 5] & (1 << (nDataID % 32))) == 0)
        return MX_OK;

    pLogRec = (PGenLogRecType)LogMgrAllocSpace (pCtx->hLog, sizeof (GenLogRecType) + nDataSize);

    if (pLogRec == NULL)
        return RC_GENLOG_ALLOC_ERROR;

    // to start copying of payload
    // in parallel with header (if MDMA is used)

    if (pData != NULL)
    {
        if (GEN_LOG_OPTIONS & LOG_CTRL_RAD_MDMA)
            rc = MdmaCopy (MDMA_HW_RAD, (pLogRec + 1), pData, nDataSize);
        else if (GEN_LOG_OPTIONS & LOG_CTRL_SPU_MDMA)
            rc = MdmaCopy (MDMA_HW_SPU, (pLogRec + 1), pData, nDataSize);
        else if (GEN_LOG_OPTIONS & LOG_CTRL_SYS_MDMA)
            rc = MdmaCopy (MDMA_HW_SYS, (pLogRec + 1), pData, nDataSize);
        else
            memcpy ((pLogRec + 1), pData, nDataSize);
    }

    if (FAILED(rc))
    {
        uart_printf ("GEN-LOGGER:FAILED to stard MDMA(mask=0x%x), rc=0x%x\r\n", GEN_LOG_OPTIONS, rc);

        _ASSERT_RC(rc);

        return rc;
    }

#if 0
    // to create the header in cache
    // to increase the speed
    CacheRec.nSize = nDataSize;
    CacheRec.ID    = nDataID;
    CacheRec.Descr = nDataDescr;

    // this operation more faster (burst mode is used)
    memcpy (pLogRec, &CacheRec, GEN_LOG_EFF_REC_SIZE);

    // the last operation to keep ticks as precise as possible
    pLogRec->Time  = GetTIMETICK ();
#else
    pLogRec->nSize = nDataSize;
    pLogRec->ID    = nDataID;
    pLogRec->Descr = nDataDescr;
    pLogRec->Time  = GetTIMETICK ();
#endif

    return MX_OK;
}

/** @brief This function scans log and returns information about message record

    @param pMsgPos [in/out] - in: the message possition to start scanning
                             out: the position for the further scanning

    @param pRec [out] - the pointer to the storage of address to log record

    @return [MXRC] an error code

    *  \ingroup group_lte_logging

    */

MXRC GenLogScan (UINT32 * pMsgPos, PGenLogRecType * pRec)
{
    GenLogCtx * pCtx = GenLogGetCtx();
    PGenLogRecType pScanRec;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pMsgPos);
    _ASSERT_PTR(pRec);

    if ((*pMsgPos) >= LogMgrGetLogDataSize(pCtx->hLog))
        return RC_GENLOG_NO_MORE_ITEMS;

    pScanRec = (PGenLogRecType)((PUINT8)LogMgrGetLogStorage(pCtx->hLog) + (*pMsgPos));

    (*pRec) = pScanRec;

    // to move the offset
    (*pMsgPos) += (sizeof (GenLogRecType) + pScanRec->nSize);

    return MX_OK;
}

/** @brif It returns pointer to the log storage: [HEADER] [rec#0]...[rec#N]
    @return [MXRC] pointer

    *  \ingroup group_lte_logging

    */
LPVOID GenLogGetLogStorage (void)
{
    GenLogCtx * pCtx = GenLogGetCtx();

    _ASSERT_PTR(pCtx);

    return LogMgrGetLogHead (pCtx->hLog);
}

/** @brif It returns size of data located in a log in bytes
    @return [MXRC] pointer

    *  \ingroup group_lte_logging

    */
MXRC GenLogGetLogStorageSize (void)
{
    GenLogCtx * pCtx = GenLogGetCtx();
    _ASSERT_PTR(pCtx);
    return LogMgrGetLogHeadSize(pCtx->hLog);
}
/** @brif It returns size log flags (status of overwrite, etc...)
    @return [MXRC] flags

    *  \ingroup group_lte_logging

    */
UINT32 GenLogGetLogFlags (void)
{
    GenLogCtx * pCtx = GenLogGetCtx();
    _ASSERT_PTR(pCtx);
    return LogMgrGetLogFlags(pCtx->hLog);
}

/** @brief This code flushes log storage if log is in cachable memory
    @return [MXRC] an error code

    *  \ingroup group_lte_logging

    */
MXRC GenLogFlush (void)
{
    GenLogCtx * pCtx = GenLogGetCtx();
    _ASSERT_PTR(pCtx);
    MxCacheClean (LogMgrGetLogHead (pCtx->hLog), LogMgrGetLogHeadSize(pCtx->hLog));
    return MX_OK;
}

/** @brief This function returns number of messages stored in the log
    @return [UINT32] the number of messages

    *  \ingroup group_lte_logging

    */
UINT32 GenLogGetMsgNum (void)
{
    GenLogCtx * pCtx = GenLogGetCtx();
    _ASSERT_PTR(pCtx);
    return LogMgrGetWrNum(pCtx->hLog);
}

/** @brief This function enables/disables logging of message
    @return [MXRC] an error code

    *  \ingroup group_lte_logging

    */
MXRC GenLogEnableMsg (UINT32 nMsgID, UINT32 nEnb)
{
    GenLogCtx * pCtx = GenLogGetCtx();
    IRQTYPE irq;
    _ASSERT_PTR(pCtx);

    _ASSERT (nMsgID < sizeof (pCtx->Mask) * 8);

    irq = ARM_INT_disable ();
    MxGetLock (&pCtx->Lock);

    if (nEnb)
        pCtx->Mask [nMsgID >> 5] |= 1 << (nMsgID % 32);
    else
        pCtx->Mask [nMsgID >> 5] &= ~(1 << (nMsgID % 32));

    MxReleaseLock (&pCtx->Lock);
    ARM_INT_restore (irq);

    return MX_OK;
}


/** @brief This function clean log and makes number of log messages equal to 0
    @return [MXRC] an error code

    *  \ingroup group_lte_logging

    */
MXRC GenLogReset(void)
{
    GenLogCtx * pCtx = GenLogGetCtx();
    _ASSERT_PTR(pCtx);
    return LogMgrResetLog (pCtx->hLog);
}

