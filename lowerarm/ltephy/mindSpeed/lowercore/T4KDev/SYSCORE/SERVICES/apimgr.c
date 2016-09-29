//-------------------------------------------------------------------------------------------
/** @file apimgr.c
 *
 * @brief The implementation of API command parser
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "apimgr.h"
#include <string.h>

/** @brief  This function initializes API command and returns a pointer
            to the API header

    @param pMem     [out] - the pointer to the buffer where data be placed
    @param nCtrl    [in]  - the control fields of API command
    @param srcInst  [in]  - the source instance ID
    @param dstInst  [in]  - the destination instance ID
    @param type     [in]  - the type of API command
    @param ID       [in]  - the API command Message ID

    @return [ApiHeader *] the pointer to the command header

    *  \ingroup group_lte_services

    */

ApiHeader * ApiCreate (LPVOID pMem, UINT32 nCtrl, UINT16 srcInst, UINT16 dstInst,
                       UINT16 type, UINT16 ID)
{
    ApiHeader * pApi;
    ApiParam * pParam;

    _ASSERT(pMem != NULL);

    pApi = (ApiHeader *)pMem;

    pApi->Control   = nCtrl | (API_HEADER_VERSION_1 << 28);
    pApi->DstInstID = dstInst;
    pApi->SrcInstID = srcInst;
    pApi->Length    = 0;        // The payload is not present yet
    pApi->Type      = type;
    pApi->MessageID = ID;

    pParam = (ApiParam *) ((PUINT8)pApi + sizeof (ApiHeader));

    pParam->ParamLength = 0;
    pParam->ParamID     = 0;

    return pApi;
}

/** @brief  The command returns a total number of the command parameters

    @param pApi [in] - the pointer to the command's header

    @return [UINT32] the total number of command parameters

    *  \ingroup group_lte_services

    */

UINT32 ApiGetParamNum (ApiHeader * pApi)
{
    UINT32 nCount = 0;
    UINT16 nTotalSize = 0;

    ApiParam * pParam = NULL;

    _ASSERT(pApi != NULL);

    // to check the supported version

    if (GetApiVersion(pApi) > API_SUPPORTED_VERSION)
        return 0;

    pParam = (ApiParam *) ((PUINT8)pApi + sizeof (ApiHeader));

    while (pParam->ParamID != PARAM_ID_INVALID && nTotalSize < pApi->Length)
    {
        nCount ++;

        nTotalSize += GetParamTotalSize(pParam);

        pParam = GetNextParam(pParam);
    }

    return nCount;
}


/** @brief  The command returns a pointer to the command parameter specified
            by the index, the index should be started from zero [0 ... n]

    @param pApi [in] - the pointer to the command header
    @param nNum [in] - the parameter index

    @return [ApiParam *] the pointer to the API command parameter

    *  \ingroup group_lte_services

    */

ApiParam * ApiGetParam (ApiHeader * pApi, UINT16 nNum)
{
    UINT32 i;
    UINT16 nTotalSize = 0;

    ApiParam * pParam = NULL;

    _ASSERT(pApi != NULL);

    // to check the supported version

    if (GetApiVersion(pApi) > API_SUPPORTED_VERSION)
        return NULL;

    pParam = (ApiParam *) ((PUINT8)pApi + sizeof (ApiHeader));

    for (i = 0; i < nNum && (nTotalSize < pApi->Length); i++)
    {
        if (pParam->ParamID == PARAM_ID_INVALID)
            return NULL;

        nTotalSize += GetParamTotalSize(pParam);

        pParam = GetNextParam(pParam);
    }

    if (nTotalSize >= pApi->Length)
        return NULL;

    return pParam;
}

/** @brief This function returns a pointer to the API command parameter specified
           by the parameter id.

    @param pApi    [in] - the pointer to the API command
    @param nParamID[in] - the parameter ID

    @return [ApiParam *] parameter descriptor or NULL

    *  \ingroup group_lte_services

    */

ApiParam * ApiGetParamByID (ApiHeader * pApi, UINT16 nParamID)
{
    ApiParam * p = NULL;

    _ASSERT_PTR(pApi);

    // to check the supported version

    if (GetApiVersion(pApi) > API_SUPPORTED_VERSION)
        return NULL;

    while ((p = ApiGetNextParam(pApi, p)) != NULL)
    {
        if (p->ParamID == nParamID)
            return p;
    }

    return NULL;
}

/** @brief  The command returns a pointer to the command parameter specified by the
            index, the index should be started from zero [0 ... n]

    @param pApi [in] - the pointer to the command header
    @param nNum [in] - the current parameter or NULL for the first param

    @return [ApiParam *] the pointer to the API command parameter or NULL

    *  \ingroup group_lte_services

    */

ApiParam * ApiGetNextParam (ApiHeader * pApi, ApiParam * pCurParam)
{
    ApiParam * p = NULL;

    _ASSERT (pApi != NULL);

    // to check the supported version

    if (GetApiVersion(pApi) > API_SUPPORTED_VERSION)
        return NULL;

    if (pCurParam == NULL)
    {
        p = (ApiParam *) ((PUINT8)pApi + sizeof (ApiHeader));
    }
    else
    {
        if (pCurParam->ParamID != PARAM_ID_INVALID)
        {
            p = GetNextParam(pCurParam);
        }
    }

    if (p == NULL)
        return NULL;

    if (p->ParamID == PARAM_ID_INVALID)
        return NULL;

    if (((UINT32)p - ((UINT32)pApi + sizeof (ApiHeader))) >= pApi->Length)
        return NULL;

    return p;
}

/** @brief The function returns a size of parameter payload

    @param pParam [in] - the parameter descriptor

    @return [UINT32] parameter data size

    *  \ingroup group_lte_services

    */

UINT32 ApiGetParamDataSize (ApiParam * pParam)
{
    if (pParam->ParamID == PARAM_ID_INVALID)
        return 0;

    return (pParam->ParamLength);
}
/** @brief The function returns a total size of command in bytes

    @param pApi [in] - the pointer to the API header

    @return [UINT32] the total number of bytes

    *  \ingroup group_lte_services

    */

UINT32 ApiGetTotalSize (ApiHeader * pApi)
{
    _ASSERT(pApi != NULL);

    return pApi->Length + API_HDR_SIZE;
}

/** @brief The function adds new one parameter to the API command and returns
           a pointer to the free place for the future parameter.

    @param pApi        [in] - the pointer to the API command header
    @param pParamPlace [in] - the pointer to the place for new parameter or NULL
    @param nParamID    [in] - the parameter ID
    @param pParamData  [in] - the parameter data or NULL (nDataSize == 0)
    @param nDataSize   [in] - the data size

    @return [ApiParam *] the pointer to the place for new one parameter

    *  \ingroup group_lte_services

    */

ApiParam * ApiAddParam (ApiHeader * pApi, ApiParam * pParamPlace,
                        UINT16 nParamID, LPVOID pParamData, UINT16 nDataSize)
{
    ApiParam * pPlace = pParamPlace;
    UINT16 nTotalSize = 0;

    _ASSERT(pApi != NULL);

    /*to find the place for the new parameter */

    if (pPlace == NULL)
    {
        pPlace = (ApiParam *)((PUINT8)pApi + sizeof (ApiHeader));

        while (pPlace->ParamID != PARAM_ID_INVALID && (nTotalSize < pApi->Length))
        {
            nTotalSize += GetParamTotalSize(pPlace);
            pPlace = GetNextParam(pPlace);
        }
    }

    _ASSERT(pPlace != NULL);

    pPlace->ParamID = nParamID;
    pPlace->ParamLength = nDataSize;

    if (nDataSize != 0 && pParamData != NULL)
    {
        memcpy (pPlace->ParamData, pParamData, nDataSize);
    }

    // The command size was increased to the total size of parameter
    pApi->Length += GetParamTotalSize(pPlace);

    // Move to the terminator place
    pPlace = GetNextParam(pPlace);

    // To add the optional parameter terminator
    // (according to the old specification)

    pPlace->ParamLength = 0;
    pPlace->ParamID = PARAM_ID_INVALID;

    return pPlace;
}

/** @brief This function adds new one parameter to the API command and
           marks this parameter as error code (id of error code is reserved)

    @param pApi     [in] - the pointer to the API command
    @param nErrCode [in] - the error code

    @return [ApiParam *] the pointer to the free place for other parameter

    *  \ingroup group_lte_services

    */

ApiParam * ApiAddErrorCode (ApiHeader * pApi, UINT32 nErrCode)
{
    ApiParam * pParam;

    _ASSERT_PTR(pApi);

    pParam = ApiGetParamByID (pApi, PARAM_ID_ERROR_CODE);

    if (pParam != NULL)
    {
        pParam->ParamLength   = sizeof (UINT32);
        pParam->ParamData [0] = nErrCode;

        return pParam;
    }
    else
    {
        return ApiAddParam(pApi, NULL, PARAM_ID_ERROR_CODE, &nErrCode, sizeof (UINT32));
    }
}

/** @brief The function adds new one parameter to the API command and returns
           a pointer to the free place for the future parameter.

    @param pApi        [in] - the pointer to the API command header
    @param pParamData  [in] - the parameter data or NULL (nDataSize == 0)
    @param nDataSize   [in] - the data size
    @param nBSizeLimit [in] - the size of buffer where API is located

    @return [ApiParam *] the pointer to the place for new one parameter

    *  \ingroup group_lte_services

    */
ApiParam * ApiAddBypass (ApiHeader * pApi, LPVOID pParamData, UINT16 nDataSize, UINT32 nBSizeLimit)
{
    PUINT8 pPlace =  NULL;

    pPlace = (PUINT8)((PUINT8)pApi + sizeof (ApiHeader));

    _ASSERT(pPlace != NULL);
    _ASSERT(nDataSize < (nBSizeLimit - API_HDR_SIZE));

    if (nDataSize != 0 && nDataSize < (nBSizeLimit - API_HDR_SIZE))
    {
        memcpy (pPlace, pParamData, nDataSize);
    }

    // The command size was increased to the total size of parameter
    pApi->Length = nDataSize;

    return (ApiParam *)(pPlace + ROUND(nDataSize, sizeof (UINT32)));
}

/** @brief This fuction returns 32 bits value

    @param pApi         [in] - the API header
    @param nParamID     [in] - the parameter ID
    @param nDefVal      [in] - the default value if parameter cannot be found, or param size > 4 bytes

    @return [UINT32] an error code

    *  \ingroup group_lte_services

    */

UINT32 ApiGetParamU32 (ApiHeader * pApi, UINT32 nParamID, UINT32 nDefVal)
{
    ApiParam * pParam = ApiGetParamByID (pApi, nParamID);

    if (pParam == NULL)
        return nDefVal;

    if (pParam->ParamLength == sizeof (UINT32))
        return pParam->ParamData[0];

    if (pParam->ParamLength == sizeof (UINT16))
        return (UINT32)(pParam->ParamData[0] & 0xFFFF);

    if (pParam->ParamLength == sizeof (UINT8))
        return (UINT32)(pParam->ParamData[0] & 0xFF);

    return nDefVal;
}

