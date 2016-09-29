/** @file unified_diag.c
 *
 * @brief Unified Diagnostics module
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
#include "unified_diag.h"
#include "sirqassign.h"
#include "heaps.h"
#include "network/arp.h"
#include "network/udp.h"
#include "network/ip.h"

#ifdef UNIDIAG_ENABLED

UNIDIAGCONTXT gUniDiagContxt;

void UniDiagOverCmdIntf(UINT16 nID, LPVOID pData, UINT16 nDataSize);
void UniDiagOverUDP(UINT16 nID, LPVOID pData, UINT16 nDataSize);
MXRC UniDiagStreamRedirection(ApiHeader *pApi);

#define MAX_UDP_UD_PAYLOAD_SIZE (NET_ETHERNET_MAX_PAYLOAD_SIZE - udphdr_size - iphdr_size-sizeof(EXTRAUDHEADER))


INLINE UNIDIAGCONTXT* UniDiagGetCtx(void)
{
    return &gUniDiagContxt;
}

MXRC UniDiagConfigure(ApiHeader *pApi)
{
    ApiParam * pParam = NULL;
    PUNIDIAGCONTXT pCtx = UniDiagGetCtx();
    MXRC rc = MX_OK;

    while ((pParam = ApiGetNextParam(pApi, pParam)) != NULL)
    {
        switch(pParam->ParamID)
        {
            case PAR_API_MESSAGE_REQUEST:
                pCtx->API_Message_Option = *pParam->ParamData;
                break;
            default:

                break;
        }
    }

    return rc;
}

MXRC UniDiagStreamEnable(ApiHeader *pApi)
{
    ApiParam * pParam = NULL;
    PUNIDIAGCONTXT pCtx = UniDiagGetCtx();
    MXRC rc = MX_OK;

    while ((pParam = ApiGetNextParam(pApi, pParam)) != NULL)
    {
        switch(pParam->ParamID)
        {
            case PAR_API_MESSAGE_REQUEST:
                    pCtx->StreamEnableMask |=API_MESSAGE_MASK;
                break;
            default:
                break;
        }
    }


    return rc;
}

MXRC UniDiagStreamDisable(ApiHeader *pApi)
{
    ApiParam * pParam = NULL;
    PUNIDIAGCONTXT pCtx = UniDiagGetCtx();
    MXRC rc = MX_OK;

    while ((pParam = ApiGetNextParam(pApi, pParam)) != NULL)
    {
        switch(pParam->ParamID)
        {
            case PAR_API_MESSAGE_REQUEST:
                    pCtx->StreamEnableMask &= ~API_MESSAGE_MASK;
                break;
            default:
                break;
        }
    }

    return rc;

}

MXRC UniDiagStreamRedirection(ApiHeader *pApi)
{
    ApiParam * pParam = NULL;
    PUNIDIAGCONTXT pCtx = UniDiagGetCtx();
    MXRC rc = MX_OK;

    pParam = ApiGetParamByID(pApi, PAR_UNIDIAG_INTERFACE);

    if(pParam == NULL)
        return RC_UNIDIAG_NO_PARAM;

    if(pCtx->UD_interface == pParam->ParamData[0])
        return MX_OK;


    switch(pParam->ParamData[0])
    {
        case UNIDIAG_INTF_DEFAULT_CMD_INTF:
            if(pCtx->UD_interface == UNIDIAG_INTF_ETH)
            {
                if(pCtx->soc)
                {
                    net_close(pCtx->soc);
                    pCtx->soc = NET_NO_SOCKETS;
                }
            }

            pCtx->UD_interface= UNIDIAG_INTF_DEFAULT_CMD_INTF;
            pCtx->transmit = UniDiagOverCmdIntf;

            break;

        case UNIDIAG_INTF_ETH:
            {
                UINT32 ip;
                UINT16 port;

                pParam = ApiGetParamByID(pApi, PAR_UNIDIAG_DEST_IPADDR);
                if(pParam == NULL)
                    return RC_UNIDIAG_NO_NETWORK_DEST;
                ip = pParam->ParamData[0];

                pParam = ApiGetParamByID(pApi, PAR_UNIDIAG_DEST_PORT);
                if(pParam == NULL)
                    return RC_UNIDIAG_NO_NETWORK_DEST;
                port = pParam->ParamData[0] & 0x0000FFFF;

                pParam = ApiGetParamByID(pApi, PAR_UNIDIAG_DEST_MACADDR);
                if(pParam == NULL)
                    return RC_UNIDIAG_NO_NETWORK_DEST;

                if(pParam->ParamLength !=6)
                    return RC_UNIDIAG_PARAM_LEN_ERROR;

                rc = UniDiagSetStreamOverUDP(ip,port,(byte *)pParam->ParamData);

            }
            break;
         default:
            return RC_UNIDIAG_PARAM_ERROR;
    }


    return rc;

}

/** @brief This function is called to process API commands targeted to the
           lower ARM Unified Diagnostics module, the InstanceID = 0x1 (the global: 0x8001)

    @param pData [in] - the pointer to the context [it set to NULL]
    @param pApi  [in] - the received API command
    @param pResp [out]- the pointer to the response

    @return [APIHEC]

    \ingroup group_ud_services
    */
APIHEC UniDiagAPIHandler(LPVOID pData, ApiHeader * pApi, ApiHeader * pResp)
{
    MXRC rc = MX_OK;

    if(pApi->Type == API_TYPE_CFG_PARAM)
    {
        switch (pApi->MessageID)
        {

            case DIAG_CONFIGURE:
                rc = UniDiagConfigure(pApi);
                break;

            case DIAG_STREAM_ENA:
                rc = UniDiagStreamEnable(pApi);
                break;
            case DIAG_STREAM_DIS:
                rc = UniDiagStreamDisable(pApi);
                break;
            case DIAG_REDIRECT:
                rc = UniDiagStreamRedirection(pApi);
                break;
            case DIAG_SYS_SHTDWN:
                MxRaiseInterCpu(0, SOFT_ICPU_SYS_SHDWN);
                break;
            default:
                rc = RC_UNIDIAG_UNSUP_API;


        }
    }
    else if (pApi->Type == API_TYPE_QRY_PARAM)
    {

    }
    else
    {
        rc = RC_UNIDIAG_UNSUP_TYPE;
    }

    ApiAddErrorCode(pResp, rc);


    return APIHEC_OK;
}

/** @brif The function initializes unified diagnostics manager.

    @return [MXRC] error code
    \ingroup group_ud_services
    */

MXRC UniDiagInit (void)
{
    MXRC rc = MX_OK;
    PUNIDIAGCONTXT pCtx = UniDiagGetCtx(); 

    memset(pCtx,0,sizeof(UNIDIAGCONTXT));
	pCtx->UD_interface = UNIDIAG_INTF_DEFAULT_CMD_INTF;
    pCtx->transmit = UniDiagOverCmdIntf;

    rc = SvsrRegClient(IID_UNIDIAG, UniDiagAPIHandler, NULL);
    return rc;
}

void UniDiagOverCmdIntf(UINT16 nID, LPVOID pData, UINT16 nDataSize)
{
    ApiHeader* pApi = SvsrAllocICApi();

    if(pApi)
    {
        ApiCreate(pApi,
                API_VERSION(API_SUPPORTED_VERSION),
                IID_UNIDIAG,
                UARM_INSTID_HOST,
                API_TYPE_IND,
                DIAG_IND);
        ApiAddParam(pApi,
                NULL,
                nID,
                pData,
                nDataSize);

        SvsrSendICApi(pApi);
    }



}

void UniDiagOverUDP(UINT16 nID, LPVOID pData, UINT16 nDataSize)
{
    PUNIDIAGCONTXT pCtx = UniDiagGetCtx();
    LPVOID pbuff = MxHeapAlloc(AppHeap,sizeof(EXTRAUDHEADER)
        +sizeof(ApiHeader)+sizeof(ApiParamHeader)+ ROUND(nDataSize,sizeof (UINT32))+sizeof (UINT32) );

    if(pbuff)
    {
        UINT32 left_size = 0;
        UINT32 timestamp = MxGetTicks();
        UINT16 seqnum = pCtx->seqnum++;
        UINT16 fragment_num = 0;
        UINT8* pPayload =(UINT8 *)pbuff + sizeof(EXTRAUDHEADER);


        ApiCreate(pPayload,
                API_VERSION(API_SUPPORTED_VERSION),
                IID_UNIDIAG,
                UARM_INSTID_HOST,
                API_TYPE_IND,
                DIAG_IND);
        ApiAddParam((ApiHeader*)pPayload,
                NULL,
                nID,
                pData,
                nDataSize);

        left_size = ApiGetTotalSize((ApiHeader*)pPayload);
        // fragment message if size is greater than max ethernet frame size.
        while (left_size )
        {
            UINT16 size_to_send = left_size > MAX_UDP_UD_PAYLOAD_SIZE ? MAX_UDP_UD_PAYLOAD_SIZE: left_size;
            EXTRAUDHEADER* pUDHeader =  (EXTRAUDHEADER*) (pPayload - sizeof(EXTRAUDHEADER));

            pUDHeader->seqnum = seqnum;
            pUDHeader->fragment_num= fragment_num++;
            pUDHeader->timestamp = timestamp;

            net_send(pCtx->soc, (const byte *)pUDHeader, size_to_send, 0);

            left_size  -= size_to_send ;
            pPayload     += size_to_send;
        }

        MxHeapFree(AppHeap, pbuff);
    }


}

void UniDiagOverUDPEx(UINT16 nID, LPVOID pData, UINT16 nDataSize)
{
	ApiParamHeader* pParam;
    ApiHeader* pApi;
	UINT16 size_to_send;
	EXTRAUDHEADER* pUDHeader;
	PUNIDIAGCONTXT pCtx = UniDiagGetCtx();
    UINT16 seqnum = pCtx->seqnum++;

    UINT32 timestamp = MxGetTicks();

	if(nDataSize > 1024)
		return;

	pUDHeader 	=  (EXTRAUDHEADER*) (((PUINT8) pData) - sizeof(ApiHeader) - sizeof(ApiParamHeader) - sizeof(EXTRAUDHEADER));
	pApi 		= (ApiHeader* )((PUINT8)pUDHeader + sizeof(EXTRAUDHEADER));
	pParam		= (ApiParamHeader*)((PUINT8)pApi + sizeof (ApiHeader));

    ApiCreate(pApi,
            API_VERSION(API_SUPPORTED_VERSION),
            IID_UNIDIAG,
            UARM_INSTID_HOST,
            API_TYPE_IND,
            DIAG_IND);

    pParam->ParamID = nID;
    pParam->ParamLength = nDataSize;
    pApi->Length += sizeof(ApiParamHeader) + pParam->ParamLength;
	
    //pParam = GetNextParam(pParam);
    //pParam->ParamLength = 0;
    //pParam->ParamID = PARAM_ID_INVALID;

    size_to_send = ApiGetTotalSize((ApiHeader*)pApi) + sizeof(EXTRAUDHEADER);

    pUDHeader->seqnum = seqnum;
    pUDHeader->fragment_num = 0;
    pUDHeader->timestamp = timestamp;

    net_send(pCtx->soc, (const byte *)pUDHeader, size_to_send, 0);
    
}


void UniDiagStreamingAPI(UINT16 nID, ApiHeader* pApi)
{
    PUNIDIAGCONTXT pCtx = UniDiagGetCtx();
    BOOL bSend = FALSE;

    //bypass diag_ind to avoid inf loop
    if(pApi->MessageID == DIAG_IND)
        return;

    if(!(pCtx->StreamEnableMask&API_MESSAGE_MASK))
        return;

    switch(nID)
    {
        case PAR_API_MESSAGE_TX:
            bSend = pCtx->API_Message_Option & API_TX_MASK;
            break;
        case PAR_API_MESSAGE_RX:
            bSend = pCtx->API_Message_Option & API_RX_MASK;
            break;
        default:
            break;
    }

    if(bSend)
    {
        if(pCtx->transmit)
            pCtx->transmit(nID, (LPVOID)pApi, ApiGetTotalSize(pApi));

    }

}

void UniDiagStreamingAPIEx(UINT16 nID, ApiHeader* pApi)
{
    PUNIDIAGCONTXT pCtx = UniDiagGetCtx();
    BOOL bSend = FALSE;

    //bypass diag_ind to avoid inf loop
    if(pApi->MessageID == DIAG_IND)
        return;

    if(!(pCtx->StreamEnableMask&API_MESSAGE_MASK))
        return;

    switch(nID)
    {
        case PAR_API_MESSAGE_TX:
            bSend = pCtx->API_Message_Option & API_TX_MASK;
            break;
        case PAR_API_MESSAGE_RX:
            bSend = pCtx->API_Message_Option & API_RX_MASK;
            break;
        default:
            break;
    }

    if(bSend)
    {
        if(pCtx->transmitex)
            pCtx->transmitex(nID, (LPVOID)pApi, ApiGetTotalSize(pApi));

    }

}


MXRC UniDiagSetAPIMessageOption(UINT32 option)
{
    PUNIDIAGCONTXT pCtx = UniDiagGetCtx();
    pCtx->API_Message_Option = option;
    return MX_OK;
}


MXRC UniDiagSetStreamEnableMask(UINT32 mask)
{
    PUNIDIAGCONTXT pCtx = UniDiagGetCtx();
    pCtx->StreamEnableMask = mask;
    return MX_OK;;
}

UINT32 UniDiagGetStreamEnableMask(void)
{
    PUNIDIAGCONTXT pCtx = UniDiagGetCtx();
    return pCtx->StreamEnableMask;
}

MXRC UniDiagSetStreamOverUDP(UINT32 ip, UINT16 port, UINT8* mac)
{
    PUNIDIAGCONTXT pCtx = UniDiagGetCtx();
    MXRC rc = MX_OK;
    net_sockaddr tmp;
    tmp.addr_family= 2;  //AF_INET

    rc = net_init();
    if(rc)
        return rc;

    pCtx->soc=net_connection(NET_SOCKTYPE_DGRAM, NET_PROTO_UDP);
    if(pCtx->soc == NET_NO_SOCKETS)
        return RC_UNIDIAG_NO_NETWORK_SOCKET;


    tmp.addr.ipv4addr.addr = ip;
    tmp.addr.ipv4addr.port = port;
    if(mac)
    {
        //crack the arp table if the dest IP is not in same LAN.
        net_arp_upd_entry(tmp.addr.ipv4addr.addr,mac);
    }

    net_connect(pCtx->soc, &tmp);

    pCtx->UD_interface= UNIDIAG_INTF_ETH;
    pCtx->transmit = UniDiagOverUDP;
    pCtx->transmitex = UniDiagOverUDPEx;

    return rc;
}

#endif
