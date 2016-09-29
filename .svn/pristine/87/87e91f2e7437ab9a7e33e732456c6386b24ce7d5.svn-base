//-------------------------------------------------------------------------------------------
/** @file apimgr.h
 *
 * @brief The declaration of API command parser
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

#ifndef _API_MGR_H_
#define _API_MGR_H_

#include "4gmx.h"

#define PARAM_HDR_SIZE          (sizeof (ApiParamHeader))
#define API_HDR_SIZE            (sizeof (ApiHeader))
#define API_EXT_HDR_SIZE        (sizeof (ExtApiHeader))

/*************************************************************
*       The list of supported versions                       *
**************************************************************/
#define API_HEADER_VERSION_1    ((UINT32)1)
#define API_SUPPORTED_VERSION   API_HEADER_VERSION_1

#define API_VERSION(x)          ((x) << 28)
#define API_IS_EXTENDED(api)    ((api)->Control & API_EXTENDED_HEADER)
#define API_IS_ACK_SUPPRESS(api)((api)->Control & API_SUPPRESS_ACK)
#define API_IS_FIRST_CHUNK(api) ((api)->Control & API_FIRST_CHUNK )
#define API_IS_USE_TID(api) ((api)->Control & API_USE_TID )

#define API_TID_SHIFT 8
#define API_TID_MASK 0x0000FF00
/*************************************************************
*       The list of API commands types                       *
**************************************************************/
typedef enum _API_TYPES_
{
    API_TYPE_RES        = 0,
    API_TYPE_CFG_PARAM  = 1,
    API_TYPE_CFG_RESP   = 2,
    API_TYPE_QRY_PARAM  = 3,
    API_TYPE_QRY_RESP   = 4,
    API_TYPE_IND        = 5,
    API_TYPE_DATA       = 6,
    API_TYPE_SYSINFO       = 0xFAFA,
}ApiTypesID;

typedef enum
{
    API_DEF_FLAGS       = (0 << 0), // The default control flags configuration
    API_SUPPRESS_ACK    = (1 << 0),
    API_EXTENDED_HEADER = (1 << 1),
    API_ENCAP_FM        = (1 << 2), // Encapsulation of foreign message
    API_FIRST_CHUNK     = (1 << 3),
    API_USE_TID         = (1 << 4)  // Use transaction ID

}ApiCtrlFlags;

typedef struct _API_HEADER_
{
    UINT32      Control;        // See: ApiCtrlFlags
    UINT32      Length;         // The message length
    UINT16      Type;           // The type of API command see:ApiTypesID
    UINT16      SrcInstID;      // The source instance ID
    UINT16      DstInstID;      // The destination instance ID
    UINT16      MessageID;      // The message ID

}ApiHeader, * PApiHeader;

typedef struct
{
    UINT16  ParamID;
    UINT16  ParamLength;    // The payload length + length of this header in bytes

}ApiParamHeader, * PApiParamHeader;


typedef struct _API_PARAM_
{
    UINT16  ParamID;
    UINT16  ParamLength;    // The payload length + length of this header in bytes
    UINT32  ParamData[1];

}ApiParam, * PApiParam;

typedef enum
{
    PARAM_ID_INVALID    = 0,
    PARAM_ID_ERROR_CODE = 1

}ApiParamID;

typedef struct _EXT_API_HEADER_
{
    UINT32      Control;        // See: ApiCtrlFlags
    UINT32      Length;         // The message length
    UINT16      Type;           // The type of API command see:ApiTypesID
    UINT16      SrcInstID;      // The source instance ID
    UINT16      DstInstID;      // The destination instance ID
    UINT16      MessageID;      // The message ID

    UINT32      TotalSize;      // The total size of payload (approx upto 2 MB)

}ExtApiHeader, *PExtApiHeader;

typedef struct _LINKEDLIST_API_ELEM
{
    UINT16 frameNumber;
    UINT8 MessageType;
    UINT8 subframeNumber;
    UINT32 MessageLen;
    struct _LINKEDLIST_API_ELEM* Next;
    UINT8* MessagePtr;
} LinkedListApiElem, *PLinkedListApiElem;

/** @brief The function returns a pointer to the API command payload

    @param pApi [in] - the pointer to the API command header

    @return [PUINT8] the pointer to the API payload */

INLINE PUINT8 GetApiPayload(const ApiHeader * pApi)
{
    return ((PUINT8)pApi + sizeof (ApiHeader));
}

/** @brief The function returns a pointer to the extened API command payload

    @param pApi [in] - the pointer to the API command header

    @return [PUINT8] the pointer to the API payload */

INLINE PUINT8 GetExtApiPayload (ExtApiHeader * pApi)
{
    return (PUINT8)pApi + sizeof (ExtApiHeader);
}

/** @brief This function returns the number of bytes of extended API
           command payload

    @param pApi [in] - an extended API command

    @return [UINT32] number of bytes */

INLINE UINT32 GetChunkPayloadSize (ExtApiHeader * pApi)
{
    // the length contains the size of payload (not a size of standard API header)
    // the additional fields of extended header are treated like a payload
    // therefore real size of payload should be calculated by substraction of a
    // size of additional fields

    return (pApi->Length - (sizeof (ExtApiHeader) - sizeof (ApiHeader)));
}

/** @brief This function returns version number of API header format

    @param pApi [in] - the pointer to the API command header

    @return [UINT32] a version number */

INLINE UINT32 GetApiVersion (const ApiHeader * pApi)
{
    _ASSERT_PTR(pApi);

    return ((pApi->Control >> 28) & 0xF);
}

/** @brief The function returns a total size of parameter (payload + header)

    @Note: The size if alignment to UINT32

    @return [UINT32] a total size of parameter block*/

INLINE UINT32 GetParamTotalSize (const ApiParam * pParam)
{
    return sizeof(ApiParamHeader) + ROUND(pParam->ParamLength, sizeof (UINT32));
}

/** @brief This function returns a pointer to the next parameter header

    @param pParam [in] - the pointer to the parameter header (It CANNOT be NULL)

    @return [ApiParam *] the pointert to the header of the next parameter*/

INLINE ApiParam * GetNextParam (const ApiParam * pParam )
{
    return (ApiParam *)(((PUINT8)pParam) + sizeof (ApiParamHeader) +
                        ROUND(pParam->ParamLength, 4));
}

/** @brief  This function initializes API command and returns a pointer
            to the API header

    @param pMem     [out] - the pointer to the buffer where data be placed
    @param nCtrl    [in]  - the control fields of API command
    @param srcInst  [in]  - the source instance ID
    @param dstInst  [in]  - the destination instance ID
    @param type     [in]  - the type of API command
    @param ID       [in]  - the API command Message ID

    @return [ApiHeader *] the pointer to the command header */

ApiHeader * ApiCreate (LPVOID pMem, UINT32 nCtrl, UINT16 srcInst, UINT16 dstInst,
                       UINT16 type, UINT16 ID);

/** @brief  The command returns a total number of the command parameters

    @param pApi [in] - the pointer to the command's header

    @return [UINT32] the total number of command parameters*/

UINT32 ApiGetParamNum (ApiHeader * pApi);

/** @brief  The command returns a pointer to the command parameter specified
            by the index, the index should be started from zero [0 ... n]

    @param pApi [in] - the pointer to the command header
    @param nNum [in] - the parameter index

    @return [ApiParam *] the pointer to the API command parameter */

ApiParam * ApiGetParam (ApiHeader * pApi, UINT16 nNum);

/** @brief This function returns a pointer to the API command parameter specified
           by the parameter id.

    @param pApi    [in] - the pointer to the API command
    @param nParamID[in] - the parameter ID

    @return [ApiParam *] parameter descriptor or NULL*/

ApiParam * ApiGetParamByID (ApiHeader * pApi, UINT16 nParamID);


/** @brief  The command returns a pointer to the command parameter specified by the
            index, the index should be started from zero [0 ... n]

    @param pApi [in] - the pointer to the command header
    @param nNum [in] - the current parameter or NULL for the first param

    @return [ApiParam *] the pointer to the API command parameter or NULL */

ApiParam * ApiGetNextParam (ApiHeader * pApi, ApiParam * pCurParam);

/** @brief The function returns a size of parameter payload

    @param pParam [in] - the parameter descriptor

    @return [UINT32] parameter data size */

UINT32 ApiGetParamDataSize (ApiParam * pParam);


/** @brief The function returns a total size of command in bytes

    @param pApi [in] - the pointer to the API header

    @return [UINT32] the total number of bytes */

UINT32 ApiGetTotalSize (ApiHeader * pApi);

/** @brief The function adds new one parameter to the API command and returns
           a pointer to the free place for the future parameter.

    @param pApi        [in] - the pointer to the API command header
    @param pParamPlace [in] - the pointer to the place for new parameter or NULL
    @param nParamID    [in] - the parameter ID
    @param pParamData  [in] - the parameter data or NULL (nDataSize == 0)
    @param nDataSize   [in] - the data size

    @return [ApiParam *] the pointer to the place for new one parameter */

ApiParam * ApiAddParam (ApiHeader * pApi, ApiParam * pParamPlace,
                        UINT16 nParamID, LPVOID pParamData, UINT16 nDataSize);

/** @brief This function adds new one parameter to the API command and
           marks this parameter as error code (id of error code is reserved)

    @param pApi     [in] - the pointer to the API command
    @param nErrCode [in] - the error code

    @return [ApiParam *] the pointer to the free place for other parameter*/

ApiParam * ApiAddErrorCode (ApiHeader * pApi, UINT32 nErrCode);

/** @brief The function adds new one parameter to the API command and returns
           a pointer to the free place for the future parameter.

    @param pApi        [in] - the pointer to the API command header
    @param pParamData  [in] - the parameter data or NULL (nDataSize == 0)
    @param nDataSize   [in] - the data size
    @param nBSizeLimit [in] - the size of buffer where API is located

    @return [ApiParam *] the pointer to the place for new one parameter */
ApiParam * ApiAddBypass (ApiHeader * pApi, LPVOID pParamData, UINT16 nDataSize, UINT32 nBSizeLimit);

/** @brief This fuction returns 32 bits value

    @param pApi         [in] - the API header
    @param nParamID     [in] - the parameter ID
    @param nDefVal      [in] - the default value if parameter cannot be found, or param size > 4 bytes

    @return [UINT32] an error code */

UINT32 ApiGetParamU32 (ApiHeader * pApi, UINT32 nParamID, UINT32 nDefVal);

#endif /*_API_MGR_H_*/

#ifdef __cplusplus
}
#endif

