//-------------------------------------------------------------------------------------------
/** @file 4gmx_debug.c
 *
 * @brief The implementation of internal 4GMX API of tracing system work
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifdef _MX_DEBUG

#include "4gmx_debug.h"
#include "4gmx_serv.h"

/** @brief The function moves <put> pointer of the trace context to the
           specified number of bytes.

    @param pCtx [in] - the system context
    @param nOffs[in] - the offset in bytes

    @return [none]

    \ingroup group_lte_4gmx_debug
*/
INLINE void MovePutPointer (PMXCONTEXT pCtx, UINT32 nOffs)
{
    pCtx->TracePut += nOffs;

    if (pCtx->TracePut >= pCtx->TraceBufEnd)
    {
        pCtx->TracePut = pCtx->TraceBufBegin +
                         ((UINT32)pCtx->TracePut - (UINT32)pCtx->TraceBufEnd);
    }
}


/** @brief The function moves <get> pointer of the trace context to the
           specified number of bytes.

    @param pCtx [in] - the system context
    @param nOffs[in] - the offset in bytes

    @return [none]

    \ingroup group_lte_4gmx_debug
*/
INLINE void MoveGetPointer (PMXCONTEXT pCtx, UINT32 nOffs)
{
    UINT32 nBlkSize = 0;
    UINT32 nCount = 0;

    while (TRUE)
    {
        pCtx->TraceBlkCount --;

        // The size of trace block (header + size of payload)

        nBlkSize = (UINT32)((TraceHeader*)pCtx->TraceGet)->Size +
                   sizeof (TraceHeader);

        pCtx->TraceGet += nBlkSize;

        if (pCtx->TraceGet >= pCtx->TraceBufEnd)
        {
            pCtx->TraceGet = pCtx->TraceBufBegin +
                             ((UINT32)pCtx->TraceGet-(UINT32)pCtx->TraceBufEnd);
        }

        nCount += nBlkSize;

        if (nCount >= nOffs)
            break;
    }


}

/** @brief The function writes data to the cyclic buffer and return the
           pointer to the free buffer place (the place after written data)

    @param pBuf    [in] - the pointer to the current place in buffer
    @param pBufBeg [in] - the pointer to the begin of buffer
    @param pBufEnd [in] - the pointer to the end of buffer (out of buffer)
    @param pData   [in] - the written data
    @param nSize   [in] - the data size in bytes

    @return [LPVOID] the pointer to the place after written data

    \ingroup group_lte_4gmx_debug
*/
static LPVOID WriteRingBuf (LPVOID pBuf, LPVOID pBufBeg, LPVOID pBufEnd,
                          LPVOID pData, UINT32 nSize)
{
    UINT32 nDirWr; // a number of bytes of direct writting

    // to get the number of bytes that can be directly written to the
    // cyclic buffer

    nDirWr = MIN(((UINT32)pBufEnd - (UINT32)pBuf), nSize);

    Mximemcpy (pBuf, pData, nDirWr);

    // to calculate the number of bytes that should be placed into the
    // buffer from the begin of buffer

    nSize -= nDirWr;

    if (nSize)
    {
        // to copy the rest bytes

        Mximemcpy (pBufBeg, ((PUINT8)pData) + nDirWr, nSize);

        return (PUINT8)pBufBeg + nSize;
    }

    pBuf = ((PUINT8)pBuf + nDirWr);

    // to return the pointer to the begin of buffer
    // if modified pointer is pointing to the end

    if (pBuf >= pBufEnd)
        return pBufBeg;

    return pBuf;
}

/** @brief The function reads debug data from the ring buffer

    @param pBuf    [in] - the pointer to the place of data writing
    @param pBufBeg [in] - the pointer to the begin of ring buffer
    @param pBufEnd [in] - the pointer to the end of buffer (out of buffer)
    @param pData   [in] - the written data
    @param nSize   [in] - the size of data in bytes

    @return [LPVOID] the pointer to the place where new data can be written

    \ingroup group_lte_4gmx_debug
*/
static LPVOID ReadRingBuf (LPVOID pBuf, LPVOID pBufBeg, LPVOID pBufEnd,
                          LPVOID pData, UINT32 nSize)
{
    UINT32 nDirWr; // a number of bytes of direct writting

    // to get the number of bytes that can be directly written to the
    // cyclic buffer

    nDirWr = MIN(((UINT32)pBufEnd - (UINT32)pBuf), nSize);

    Mximemcpy (pData, pBuf, nDirWr);

    // to calculate the number of bytes that should be placed into the
    // buffer from the begin of buffer

    nSize -= nDirWr;

    if (nSize)
    {
        // to copy the rest bytes

        Mximemcpy (((PUINT8)pData) + nDirWr, pBufBeg, nSize);

        return (PUINT8)pBufBeg + nSize;
    }

    pBuf = ((PUINT8)pBuf + nDirWr);

    // to return the pointer to the begin of buffer
    // if modified pointer is pointing to the end

    if (pBuf >= pBufEnd)
        return pBufBeg;

    return pBuf;
}

/** @brief This function calculates the number of bytes that overlapes
           specified pointer <pGet> in thering buffer.

    @param pBufSet [in] - the pointer to the current place in buffer
    @param pBufBeg [in] - the pointer to the begin of buffer
    @param pBufEnd [in] - the pointer to the end of buffer (out of buffer)
    @param pGet    [in] - the tested point
    @param nSize   [in] - the data size in bytes

    @return [LPVOID] the number of bytes

    \ingroup group_lte_4gmx_debug
*/
static UINT32 CalcOverlapedGet (LPVOID pBufSet, LPVOID pBufBeg, LPVOID pBufEnd,
                                LPVOID pGet, UINT32 nSize)
{
    UINT32 nDirect;
    UINT32 nOvr = 0;

    nDirect = MIN(((UINT32)pBufEnd - (UINT32)pBufSet), nSize);

    if (((UINT32)pBufSet + nDirect) > (UINT32)pGet && pGet > pBufSet)
    {
        nOvr = ((UINT32)pBufSet + nDirect) - (UINT32)pGet;

		return nOvr + (nSize - nDirect);
    }

    nSize -= nDirect;

    if (nSize != 0)
    {
        if ((UINT32)pBufBeg + nSize > (UINT32)pGet)
        {
            nOvr += ((UINT32)pBufBeg + nSize - (UINT32)pGet);
        }
    }

    return nOvr;
}

/** @brief The function adds information to the system debug trace

    @param pCtx  [in] - the system context
    @param id    [in] - the elenent ID
    @param pData [in] - the pointer to the element data (it can be NULL)
    @param nSize [in] - the data size (0 if pData is NULL)

    @return [MXRC] error code

    \ingroup group_lte_4gmx_debug
*/
MXRC MxWriteDebugTrace (PMXCONTEXT pCtx, TraceEventID id,
                        LPVOID pData, UINT8 nSize)
{
    TraceHeader head;
    UINT32 nOvrSize;
    MXRC rc = MX_OK;
    IRQTYPE irq;

    _ASSERT(pCtx != NULL);

    if (pCtx->TraceBufBegin == NULL)
        return MX_OK;

    _ASSERT(id < 256);
    _ASSERT(nSize < 256);

    if (pData == NULL || nSize == 0)
    {
        pData = NULL;
        nSize = 0;
    }

    nOvrSize = (UINT32)pCtx->TraceBufEnd - (UINT32)pCtx->TraceBufBegin;

    if (nOvrSize < nSize + sizeof (TraceHeader))
        return MX_TRACE_SMALL_BUF;

    MxiDisInt(&irq);

    pCtx->TraceBlkCount ++;

    nOvrSize = CalcOverlapedGet (pCtx->TracePut,
                                 pCtx->TraceBufBegin,
                                 pCtx->TraceBufEnd,
                                 pCtx->TraceGet,
                                 sizeof (TraceHeader) + nSize);

    if (nOvrSize)
    {
        MoveGetPointer(pCtx, nOvrSize);
    }

    head.ID   = (TTRACEID)id;
    head.Size = (TTRACESIZE)nSize;

    pCtx->TracePut = (PUINT8)WriteRingBuf(pCtx->TracePut,
                                    pCtx->TraceBufBegin,
                                    pCtx->TraceBufEnd,
                                    &head,
                                    sizeof (TraceHeader));

    if (pData != NULL)
    {

        pCtx->TracePut = (PUINT8)WriteRingBuf(pCtx->TracePut,
                                        pCtx->TraceBufBegin,
                                        pCtx->TraceBufEnd,
                                        pData,
                                        nSize);
    }

    MxiRestInt(irq);

    return rc;
}

/** @brief The function reads information of the system debug trace

    @param pCtx  [in]     - the system context
    @param pID   [out]    - the element ID
    @param pData [out]    - the pointer to the element data
    @param pnSize[in/out] - the data size

    @return [MXRC] error code

    \ingroup group_lte_4gmx_debug
*/
MXRC MxReadDebugTrace  (PMXCONTEXT pCtx, TraceEventID * pID,
                        LPVOID pData, PUINT8 pnSize)
{
    TraceHeader head;
    PUINT8 pGet;
    IRQTYPE irq;

    _ASSERT(pID   != NULL);
    _ASSERT(pCtx  != NULL);
    _ASSERT(pnSize!= NULL);
    _ASSERT(pData != NULL);

    if (pCtx->TraceBufBegin == NULL)
    {
        return MX_TRACE_EMPTY;
    }

    if (pCtx->TraceBlkCount == 0)
    {
        return MX_TRACE_EMPTY;
    }

    MxiDisInt(&irq);

    pGet = pCtx->TraceGet;

    pCtx->TraceGet = (PUINT8)ReadRingBuf (pCtx->TraceGet,
                                          pCtx->TraceBufBegin,
                                          pCtx->TraceBufEnd,
                                          &head, sizeof (TraceHeader));

    *pID = (TraceEventID)head.ID;

    if (head.Size > *pnSize)
    {
        // The buffer is so small and user application should
        // provide more bigger buffer

        *pnSize = head.Size;

        pCtx->TraceGet = pGet;

        MxiRestInt(irq);

        return MX_TRACE_SMALL_BUF;
    }

    *pnSize = head.Size;

    pCtx->TraceGet = (PUINT8)ReadRingBuf (pCtx->TraceGet,
                                          pCtx->TraceBufBegin,
                                          pCtx->TraceBufEnd,
                                          pData, head.Size);

    pCtx->TraceBlkCount --;

    MxiRestInt(irq);

    return MX_OK;
}


#endif /*_MX_DEBUG*/

