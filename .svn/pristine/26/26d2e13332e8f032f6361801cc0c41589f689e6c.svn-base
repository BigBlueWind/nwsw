/** @file tlvmsg.c
 *
 * @brief Support for TLV messages (BE/LE byte order)
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "osal.h"
#include "tlvmsg.h"
#include "ctrlmsg.h"

/**
 *
 * @param pStream
 * @return
 */
U8 TlvReadU8Inc(PTLVSTREAM *pStream)
{
    U8 res = **pStream;
    (*pStream) += 1;

    return res;
}

/**
 *
 * @param pStream
 * @return
 */
U16 TlvReadU16Inc(PTLVSTREAM *pStream)
{
    U16 res;
    U8 *ptr = (U8 *)&res;

    // For some reason these are little-endian
#if 0
    *(ptr + 0) = *((*pStream) + 1);
    *(ptr + 1) = *((*pStream) + 0);
#else
    *(ptr + 0) = *((*pStream) + 0);
    *(ptr + 1) = *((*pStream) + 1);
#endif
    (*pStream) += 2;

    return res;
}

/**
 *
 * @param pStream
 * @return
 */
U32 TlvReadU32Inc(PTLVSTREAM *pStream)
{
    U32 res;
    U8 *ptr = (U8 *)&res;

    // For some reason these are little-endian
#if 0
    *(ptr + 0) = *((*pStream) + 3);
    *(ptr + 1) = *((*pStream) + 2);
    *(ptr + 2) = *((*pStream) + 1);
    *(ptr + 3) = *((*pStream) + 0);
#else
    *(ptr + 0) = *((*pStream) + 0);
    *(ptr + 1) = *((*pStream) + 1);
    *(ptr + 2) = *((*pStream) + 2);
    *(ptr + 3) = *((*pStream) + 3);
#endif
    (*pStream) += 4;

    return res;
}

/**
 *
 * @param pStream
 * @return
 */
U8 TlvReadU8(PTLVSTREAM pStream)
{
    U8 res = *pStream;

    return res;
}

/**
 *
 * @param pStream
 * @return
 */
U16 TlvReadU16(PTLVSTREAM pStream)
{
    U16 res;
    U8 *ptr = (U8 *)&res;

    // For some reason these are little-endian
#if 0
    *(ptr + 0) = *(pStream + 1);
    *(ptr + 1) = *(pStream + 0);
#else
    *(ptr + 0) = *(pStream + 0);
    *(ptr + 1) = *(pStream + 1);
#endif

    return res;
}

/**
 *
 * @param pStream
 * @return
 */
U32 TlvReadU32(PTLVSTREAM pStream)
{
    U32 res;
    U8 *ptr = (U8 *)&res;

    // For some reason these are little-endian
#if 0
    *(ptr + 0) = *(pStream + 3);
    *(ptr + 1) = *(pStream + 2);
    *(ptr + 2) = *(pStream + 1);
    *(ptr + 3) = *(pStream + 0);
#else
    *(ptr + 0) = *(pStream + 0);
    *(ptr + 1) = *(pStream + 1);
    *(ptr + 2) = *(pStream + 2);
    *(ptr + 3) = *(pStream + 3);
#endif

    return res;
}

/**
 *
 * @param stream
 * @param data
 * @return
 */
PTLVSTREAM TlvWriteU8(PTLVSTREAM stream, U8 data)
{
    *stream = data;
    return stream + sizeof(U8);
}

/**
 *
 * @param stream
 * @param data
 * @return
 */
PTLVSTREAM TlvWriteU16(PTLVSTREAM stream, U16 data)
{
    U8 *ptr = (U8 *) &data;

    // For some reason these are little-endian
#if 0
    *(stream + 0) = *(ptr + 1);
    *(stream + 1) = *(ptr + 0);
#else
    *(stream + 0) = *(ptr + 0);
    *(stream + 1) = *(ptr + 1);
#endif
    return stream + sizeof(U16) / sizeof(U8);
}

/**
 *
 * @param stream
 * @param data
 * @return
 */
PTLVSTREAM TlvWriteU32(PTLVSTREAM stream, U32 data)
{
    U8 *ptr = (U8 *) &data;

    // For some reason these are little-endian
#if 0
    *(stream + 0) = *(ptr + 3);
    *(stream + 1) = *(ptr + 2);
    *(stream + 2) = *(ptr + 1);
    *(stream + 3) = *(ptr + 0);
#else
    *(stream + 0) = *(ptr + 0);
    *(stream + 1) = *(ptr + 1);
    *(stream + 2) = *(ptr + 2);
    *(stream + 3) = *(ptr + 3);
#endif

    return stream + sizeof(U32) / sizeof(U8);
}

/**
 *
 * @param stream
 * @param tag
 * @param paramCount
 * @return
 */
PTLVSTREAM TlvWriteTlv(PTLVSTREAM stream, int tag, int paramCount, ...)
{
    int paramType, paramData;
    int length = sizeof(U16) * 2;       // Initially includes Tag and Length itself
    va_list argptr;
    PTLVSTREAM lengthPtr, nestedTlv;
    void *paramBin;

    va_start(argptr, paramCount);

    // Write T part of a TLV
    lengthPtr = TlvWriteU16(stream, tag);
    // Write initial length
    stream = TlvWriteU16(lengthPtr, length);

    while (paramCount--)
    {
        paramType = va_arg(argptr, int);
        paramData = va_arg(argptr, int);

        switch (paramType)
        {
        case TYPE_U8:
            stream = TlvWriteU8(stream, paramData);
            length += sizeof(U8);
            break;
        case TYPE_U16:
            stream = TlvWriteU16(stream, paramData);
            length += sizeof(U16);
            break;
        case TYPE_U32:
            stream = TlvWriteU32(stream, paramData);
            length += sizeof(U32);
            break;
        case TYPE_BINARY:
            // In this case paramData explains size and next parameter is a pointer
            paramBin = va_arg(argptr, void*);
            memcpy(stream, paramBin, paramData);
            stream += paramData;
            length += paramData;
            break;
        case TYPE_TLV:
            // Was call to TlvNestedTlv
            // paramData is not used
            nestedTlv = (PTLVSTREAM) va_arg(argptr, void*);
            memcpy(stream, nestedTlv, TlvReadLen(nestedTlv));
            stream += TlvReadLen(nestedTlv);
            length += TlvReadLen(nestedTlv);
            free(nestedTlv);
            break;
        default:
            // Will crash
            return NULL;
        }
    }
    va_end(argptr);
    // Write final length at predefined position
    TlvWriteU16(lengthPtr, length);
    return stream;
}

PTLVSTREAM TlvNestedTlv(int tag, int paramCount, ...)
{
    va_list argptr;
    int paramType, paramData;
    void *paramBin;
    int length = sizeof(U16) * 2;

    // TODO: Should be 2-pass verification
    PTLVSTREAM pTlv = malloc(1024);
    PTLVSTREAM tailPtr, lengthPtr, nestedTlv;
    if (pTlv == NULL)
    {
        return NULL;
    }
    tailPtr = pTlv;

    va_start(argptr, paramCount);

    // Write Tag
    lengthPtr = TlvWriteU16(tailPtr, tag);
    // Write initial length
    tailPtr = TlvWriteU16(lengthPtr, length);

    while (paramCount--)
    {
        paramType = va_arg(argptr, int);
        paramData = va_arg(argptr, int);

        switch (paramType)
        {
        case TYPE_U8:
            tailPtr = TlvWriteU8(tailPtr, paramData);
            length += sizeof(U8);
            break;
        case TYPE_U16:
            tailPtr = TlvWriteU16(tailPtr, paramData);
            length += sizeof(U16);
            break;
        case TYPE_U32:
            tailPtr = TlvWriteU32(tailPtr, paramData);
            length += sizeof(U32);
            break;
        case TYPE_BINARY:
            // In this case paramData explains size and next parameter is a pointer
            paramBin = va_arg(argptr, void*);
            memcpy(tailPtr, paramBin, paramData);
            tailPtr += paramData;
            length += paramData;
            break;
        case TYPE_TLV:
            // Was call to TlvNestedTlv
            // paramData is not used
            nestedTlv = (PTLVSTREAM) va_arg(argptr, void*);
            memcpy(tailPtr, nestedTlv, TlvReadLen(nestedTlv));
            tailPtr += TlvReadLen(nestedTlv);
            length += TlvReadLen(nestedTlv);
            free(nestedTlv);
            break;
        default:
            // Will crash
            return NULL;
        }
    }

    va_end(argptr);
    // Update our length
    TlvWriteU16(lengthPtr, length);
    return pTlv;
}

/** Append nested TLV to the parent TLV
 *
 * @param stream
 * @param tag
 * @param paramCount
 * @return
 */
PTLVSTREAM TlvAppendTlv(PTLVSTREAM parent, int tag, int paramCount, ...)
{
    int paramType, paramData;
    int length = 4, parentLength;
    va_list argptr;
    PTLVSTREAM parentLengthPtr, lengthPtr, tailPtr;
    void *paramBin;

    va_start(argptr, paramCount);

    // Read original length
    parentLengthPtr = parent + sizeof(U16);
    parentLength = TlvReadU16(parentLengthPtr);
    // Find end of stream
    tailPtr = parent + parentLength;
    // Write T part of a TLV
    lengthPtr = TlvWriteU16(tailPtr, tag);
    // Write initial length
    tailPtr = TlvWriteU16(lengthPtr, length);

    while (paramCount--)
    {
        paramType = va_arg(argptr, int);
        paramData = va_arg(argptr, int);

        switch (paramType)
        {
        case TYPE_U8:
            tailPtr = TlvWriteU8(tailPtr, paramData);
            length += sizeof(U8);
            break;
        case TYPE_U16:
            tailPtr = TlvWriteU16(tailPtr, paramData);
            length += sizeof(U16);
            break;
        case TYPE_U32:
            tailPtr = TlvWriteU32(tailPtr, paramData);
            length += sizeof(U32);
            break;
        case TYPE_BINARY:
            // In this case paramData explains size and next parameter is a pointer
            paramBin = va_arg(argptr, void*);
            memcpy(tailPtr, paramBin, paramData);
            tailPtr += paramData;
            length += paramData;
            break;
        default:
            // Will crash
            return NULL;
        }
    }
    va_end(argptr);
    // Update our length
    TlvWriteU16(lengthPtr, length);
    // Update parent's length
    TlvWriteU16(parentLengthPtr, parentLength + length);
    return tailPtr;
}

U16 TlvReadLen(PTLVSTREAM stream)
{
    return TlvReadU16(stream + sizeof(U16)/sizeof(U8));
}

PTLVSTREAM TlvWriteLen(PTLVSTREAM stream, U16 len)
{
    TlvWriteU16(stream + sizeof(U16)/sizeof(U8), len);
    return stream + len / sizeof(U8);
}

#if 0
// Example
pPayload = TlvWriteU16(pPayload, NumUeCreated);
// ADD_UE_INFO
pPayload = TlvWriteTlv(pPayload, ADD_UE_INFO, 12,
                       TYPE_U16, 10,
                       TYPE_U8, 2,
                       TYPE_U8, 6,
                       ..........,
// SR_CONFIG_INFO
                       TYPE_TLV, TlvNestedTlv(
// SR_SETUP_INFO
                       TYPE_TLV, TlvNestedTlv()
                       ),
                       );
// CREATE_LC_REQ
pPayload = TlvWriteTlv(pPayload, CREATE_LC_REQ, 2
                       TYPE_U16, 10,
                       TYPE_U8, 2,
                       TYPE_U8, 6,
                       ..........,
                       );
#endif