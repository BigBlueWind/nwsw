//-------------------------------------------------------------------------------------------
/** @file Stream.c
 *
 * @brief The API builder(it's used to build API command based on console inputs)
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "stream.h"

/**
    @brief The function is converting UINT32 value from little endian to big endian

    @param UINT32 nVal [in]    -    value

    @return [UINT32]  value in big endian format
    \ingroup group_lte_services

*/
UINT32 Stream_UINT32ToBE (UINT32 nVal)
{
    UINT32 nNew = 0;

    nNew |= ((nVal >>  0) & 0xFF); nNew <<= 8;
    nNew |= ((nVal >>  8) & 0xFF); nNew <<= 8;
    nNew |= ((nVal >> 16) & 0xFF); nNew <<= 8;
    nNew |= ((nVal >> 24) & 0xFF);

    return nNew;
}

/**
    @brief The function is converting UINT32 value from big endian to little endian

    @param UINT32 nVal [in]    -    value

    @return [UINT32]  value in little endian format
    \ingroup group_lte_services
*/
UINT32 Stream_UINT32ToLE (UINT32 nVal)
{
    return Stream_UINT32ToBE (nVal);
}

/**
    @brief The function is converting UINT16 value from little endian to big endian

    @param UINT16 nVal [in]    -    value

    @return [UINT16]  value in big endian format
    \ingroup group_lte_services
*/
UINT16     Stream_UINT16ToBE (UINT16 nVal)
{
    UINT16 nNew = 0;

    nNew |= ((nVal >>  0) & 0xFF); nNew <<= 8;
    nNew |= ((nVal >>  8) & 0xFF);

    return nNew;
}

/**
    @brief The function is converting UINT16 value from big endian to little endian

    @param UINT16 nVal [in]    -    value

    @return [UINT16]  value in little endian format
    \ingroup group_lte_services
*/
UINT16 Stream_UINT16ToLE (UINT16 nVal)
{
    return Stream_UINT16ToBE (nVal);
}


/**    @brief The function moves data from an input buffer to an output buffer

    @param UINT8 * pInData [in]        -    a pointer to the input data
    @param UINT8 * pOutBuf [out]    -    a pointer to the output buffer
    @param UINT32* pnOffs  [in/out] -    a pointer to the output buffer index
    @param UINT32  nSize   [in]        -    a size of input  buffer

    @return [BOOL]

    \ingroup group_lte_services
    */

BOOL Stream_Store (UINT8 * pInData, UINT8 * pOutBuf, UINT32* pnOffs, UINT32 nSize)
{
    UINT32 i = 0;

    if (pInData == NULL || pOutBuf == NULL || pnOffs == NULL)
        return FALSE;

    for (i = 0 ; i < nSize; i++)
    {
        pOutBuf [ (*pnOffs) ] = pInData [i];

        (*pnOffs) ++;
    }

    return TRUE;
}

/**    @brief The function moves data from an input buffer to an output buffer

    @param UINT8 * pInData [in]        -    a pointer to the input data
    @param UINT8 * pOutBuf [out]    -    a pointer to the output buffer
    @param UINT32* pnOffs  [in/out] -    a pointer to the output buffer index
    @param UINT32  nSize   [in]        -    a size of input  buffer

    @return [BOOL]

    \ingroup group_lte_services
    */

BOOL Stream_Load  (UINT8 * pOutData, UINT8 * pInBuf, UINT32* pnOffs, UINT32 nSize)
{
    UINT32 i = 0;

    if (pOutData == NULL || pInBuf == NULL || pnOffs == NULL)
        return FALSE;

    for (i = 0 ; i < nSize; i++)
    {
        pOutData [i] = pInBuf [ (*pnOffs)  ];
        (*pnOffs) ++;
    }

    return TRUE;
}

UINT32  Stream_Power (UINT32 nVal, UINT32 nPower)
{
    UINT32 nRes = 1;
    UINT32 i;

    for (i = 1; i <= nPower; i++)
    {
        nRes *= nVal;
    }

    return nRes;
}
/**
    @brief The function converts string numeric value to UINT32 value

    @param char * pStr [in]     -   a pointer to string numeric value
    @param UINT32    nBase[in]  -   numeric system basis

    @return [UINT32]  value

    \ingroup group_lte_services

    */

UINT32  Stream_aux_StrToUINT32 (char * pStr, UINT32 nBase)
{
    UINT32 nValue = 0;
    UINT32 i = 0;
    UINT32 nBegin = 0;
    UINT32 nEnd   = 0;

    if (pStr == NULL)
        return 0;

    // skipping all spacebars
    while (pStr [i] == ' ' || pStr [i] == '\t')
    {
        nBegin ++;
        i ++;
    }

    // if string has not any data
    if (pStr [nBegin] == 0x0)
        return 0;

    // shifting pointer of the begin
    if ((pStr [nBegin + 1] == 'x' ||pStr [nBegin + 1] == 'X') && nBase == 16)
    {
        nBegin  += 2;
        i       += 2;
    }

    // move pointer to end of string
    while (pStr [i + 1] && pStr [i + 1] != ' ' && pStr [i + 1] != '\t')
    {
        i++;
    }

    nEnd = i;

    while (i >= nBegin && i <= nEnd)
    {
        if (pStr [i] >= '0' && pStr [i] <= '9')
        {
            nValue += (UINT32)(pStr [i] - '0') * Stream_Power (nBase, nEnd - i);
        }
        else if (pStr [i] >= 'A' && pStr [i] <= 'F' && nBase == 16)
        {
            nValue += (UINT32)(pStr [i] - 'A' + 10) * Stream_Power (nBase, nEnd - i);
        }
        else if (pStr [i]>= 'a' && pStr [i] <= 'f'&& nBase == 16)
        {
            nValue += (UINT32)(pStr [i] - 'a' + 10) * Stream_Power (nBase, nEnd - i);
        }
        else // if we get unknown symbol
        {
            return 0;
        }

        i --;
    }

    return nValue;
}



/** @brief The function converts string numeric value to UINT32 value

    @param char * pStr [in] -   a pointer to string numeric value in HEX format

    @return [UINT32]  UINT32 value
    \ingroup group_lte_services
    */

UINT32  Stream_HexStrToUINT32 (char * pStr)
{
    return Stream_aux_StrToUINT32 (pStr, 16);
}

/** @brief The function converts string numeric value to UINT32 value

    @param char * pStr [in] -   pointer to string numeric value in decimac format

    @return [UINT32]  UINT32 value
    \ingroup group_lte_services
    */

UINT32  Stream_StrToUINT32 (char * pStr)
{
    if (pStr[0] == '0' && (pStr[1] == 'x' || pStr[1] == 'X'))
        return Stream_aux_StrToUINT32 (pStr, 16);
    else
        return Stream_aux_StrToUINT32 (pStr, 10);
}

/**    @brief The function converts string text to UINT32 value and stores this value
            in the buffer (in the little endian format)

    @param char * pStr [in]        - a pointer to the text value
    @param UINT8 * pOutBuf[out] - a pointer to the results buffer

    @return [UINT32]  digital value
    \ingroup group_lte_services
    */

UINT32 Stream_StrToLEUINT32 (char * pStr, UINT8 * pOutBuf)
{
    UINT32 nVal = 0;

    if (pStr == NULL)
        return 0;

    if (pStr [0] == '0' && (pStr [1] == 'x' || pStr [1] == 'X'))
    {
        nVal = Stream_aux_StrToUINT32 (pStr, 16);
    }
    else
    {
        nVal = Stream_aux_StrToUINT32 (pStr, 10);
    }

    pOutBuf [0] = (UINT8)((nVal >> 0)  & 0xFF);
    pOutBuf [1] = (UINT8)((nVal >> 8)  & 0xFF);
    pOutBuf [2] = (UINT8)((nVal >> 16) & 0xFF);
    pOutBuf [3] = (UINT8)((nVal >> 24) & 0xFF);

    return nVal;
}


/**    @brief The function converts string to UINT32 value and stores this value
            to the buffer (in the big-endian format)

    @param char * pStr [in]    -    a pointer to the text value
    @param UINT8 * pOutBuf[out]-  a pointer to the result buffer

    @return [UINT32]  digital value
    \ingroup group_lte_services
    */

UINT32 Stream_StrToBEUINT32 (char * pStr, UINT8 * pOutBuf)
{
    UINT32 nVal = 0;

    if (pStr == NULL)
        return 0;

    if (pStr [0] == '0' && (pStr [1] == 'x' || pStr [1] == 'X'))
    {
        nVal = Stream_aux_StrToUINT32 (pStr, 16);
    }
    else
    {
        nVal = Stream_aux_StrToUINT32 (pStr, 10);
    }

    pOutBuf [0] = (UINT8)((nVal >> 24)  & 0xFF);
    pOutBuf [1] = (UINT8)((nVal >> 16)  & 0xFF);
    pOutBuf [2] = (UINT8)((nVal >> 8)   & 0xFF);
    pOutBuf [3] = (UINT8)((nVal >> 0)   & 0xFF);

    return nVal;
}

/**    @brief The function converts string to UINT16 value and stores this value
            to the buffer (in the little endian format)

    @param char * pStr [in]    -    a pointer to the text value
    @param UINT8 * pOutBuf[out]-  a pointer to the result buffer

    @return [UINT32]  digital value
    \ingroup group_lte_services
    */

UINT32 Stream_StrToLEUINT16 (char * pStr, UINT8 * pOutBuf)
{
    UINT16 nVal = 0;

    if (pStr == NULL)
        return 0;

    if (pStr [0] == '0' && (pStr [1] == 'x' || pStr [1] == 'X'))
    {
        nVal = (UINT16)Stream_aux_StrToUINT32 (pStr, 16);
    }
    else
    {
        nVal = (UINT16)Stream_aux_StrToUINT32 (pStr, 10);
    }

    pOutBuf [0] = (UINT8)((nVal >> 0)  & 0xFF);
    pOutBuf [1] = (UINT8)((nVal >> 8)  & 0xFF);

    return nVal;
}


/**    @brief The function converts string to UINT16 value and stores this value
            to the buffer (in the big-endian format)

    @param char * pStr [in]    -    pointer to text value
    @param UINT8 * pOutBuf[out]-   pointer to results buffer

    @return [UINT32]  digital value
    \ingroup group_lte_services
*/
UINT32 Stream_StrToBEUINT16 (char * pStr, UINT8 * pOutBuf)
{
    UINT16 nVal = 0;

    if (pStr == NULL)
        return 0;

    if (pStr [0] == '0' && (pStr [1] == 'x' || pStr [1] == 'X'))
    {
        nVal = (UINT16)Stream_aux_StrToUINT32 (pStr, 16);
    }
    else
    {
        nVal = (UINT16)Stream_aux_StrToUINT32 (pStr, 10);
    }

    pOutBuf [0] = (UINT8)((nVal >> 8)  & 0xFF);
    pOutBuf [1] = (UINT8)((nVal >> 0)  & 0xFF);

    return nVal;
}

/**    @brief The function converts text to UINT8 value and stores this value
            to the buffer

    @param char * pStr [in]    -    a pointer to the text value
    @param UINT8 * pOutBuf[out]-  a pointer to the result buffer

    @return [UINT8]  digital value
    \ingroup group_lte_services
    */

UINT8 Stream_StrToUINT8 (char * pStr, UINT8 * pOutBuf)
{
    UINT8 nVal = 0;

    if (pStr == NULL)
        return 0;

    if (pStr [0] == '0' && (pStr [1] == 'x' || pStr [1] == 'X'))
    {
        nVal = (UINT8)Stream_aux_StrToUINT32 (pStr, 16);
    }
    else
    {
        nVal = (UINT8)Stream_aux_StrToUINT32 (pStr, 10);
    }

    if (pOutBuf != NULL)
    {
        pOutBuf [0] = nVal;
    }

    return nVal;
}


/**    @brief The function loads ip address from the text description (format of ip address is IPv4)

    @param const char * pStrIP     [in]    -    the text description of ip address
    @param UINT8 *         pOutBuf [out]    -    the numeric representation of IP address

    @return [BOOL]  result of parsing
    \ingroup group_lte_services
    */

BOOL Stream_LoadIPv4 (const char * pStrIP, UINT8 * pOutBuf)
{
    UINT8   nIPIndex = 0, nBufIndex = 0, nItemNum = 0;
    char buf [5];

    if (pStrIP == NULL || pOutBuf == NULL)
        return FALSE;

    while (pStrIP [nIPIndex] && nItemNum < 4)
    {
        if (pStrIP [nIPIndex] == '.')
        {
            if (nBufIndex == 0)
                return FALSE;

            buf [nBufIndex] = 0;

            pOutBuf [nItemNum ++] = Stream_StrToUINT8(buf, NULL);

            nBufIndex = 0;
        }
        else
        {
            if (nBufIndex + 1 >= sizeof(buf))
                return FALSE;

            if (pStrIP [nIPIndex] >= '0' && pStrIP [nIPIndex] <= '9')
            {
                buf [nBufIndex ++] = pStrIP [nIPIndex];
            }
            else
            {
                return FALSE;
            }
        }

        nIPIndex ++;
    }

    if (nItemNum < 4 && nBufIndex)
    {
        pOutBuf [nItemNum ++] = Stream_StrToUINT8(buf, NULL);
    }

    if (nItemNum < 4)
        return FALSE;

    return TRUE;
}


/**    @brief The function converts ASCIIZ text to UINT32 value

    @param char * pStr [in]    -    a pointer to ASCIIZ string

    @return [UINT32]  value
    \ingroup group_lte_services
    */

UINT32 StrToNum (char * pStr)
{
    return Stream_StrToUINT32(pStr);
}

