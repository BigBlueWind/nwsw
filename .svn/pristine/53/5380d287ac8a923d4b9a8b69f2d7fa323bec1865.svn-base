/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#include "PW_PALMessageParser.h"
#include "PW_PALParameterParser.h"
#include "PW_PALFormats.h"
#include "PW_PALCommon.h"

unsigned int DecodeMassageHeader(unsigned char* pucBuffer,
                                 SMessageHeader* psMessageHeader)
{
    unsigned int  uiIndex = 0, uiLength = 0;
    unsigned char ucValue = 0;
    unsigned char ucTemp = 0;
    unsigned short usValue = 0;
    unsigned short usRecovery = 0;

    ucValue = *(pucBuffer);
    uiIndex++;
    ucTemp = ucValue;
    ucTemp = ucTemp >> 5;

    psMessageHeader->m_ucReserved = ucTemp;

    ucTemp = ucValue;
    ucTemp = ucTemp << 3;
    ucTemp = ucTemp >> 5;

    psMessageHeader->m_ucVersion = ucTemp;

    ucTemp = ucValue;
    ucTemp = ucTemp << 6;
    ucTemp = ucTemp >> 6;
    usValue = usValue | ucTemp;

    ucTemp = 0;
    ucValue = *(pucBuffer + 1);
    uiIndex++;
    usRecovery = usRecovery | usValue;
    usRecovery = usRecovery << 14;
    usRecovery = usRecovery | ucValue ;

    psMessageHeader->m_usType = usRecovery;

    uiLength = GetIntFromBuffer(pucBuffer + 2); 

    uiIndex += 4;

    psMessageHeader->m_uiLength = uiLength;
    return uiIndex;
}

unsigned int DecodeDLControlRequest(unsigned char* pucBuffer,
                                    SDLControlRequest* psDLControlRequest)
{
    unsigned int uiIndex = 0;
    
    uiIndex += DecodeMassageHeader(pucBuffer,
        &(psDLControlRequest->m_sMessageHeader));
    if((psDLControlRequest->m_sMessageHeader).m_usType
        != SCHDCNTRLREQ)
    {
        return 0;
    }
    else
    {
        uiIndex += Decode_TFUCNTRLREQINFO_Parameter(pucBuffer + uiIndex,
            &(psDLControlRequest->m_sTfuCntrlReqInfo));
    }
    return uiIndex;
}

unsigned int  DecodeRandomAccessIndication(unsigned char* pucBuffer, 
                                           SRandomAccessIndication* psRandomAccessIndication)
{
    unsigned int uiIndex = 0;
    uiIndex += DecodeMassageHeader(pucBuffer,
        &(psRandomAccessIndication-> m_sMessageHeader));

    if((psRandomAccessIndication-> m_sMessageHeader).m_usType
        != RANDACSIND)
    {
        return 0;
    }
    else
    {
        uiIndex += Decode_TFURAREQINDINFO_Parameter(pucBuffer +uiIndex,
            &(psRandomAccessIndication-> m_sTfuRaReqIndInfo));
    }

    return uiIndex;
}

unsigned int DecodeULReceptionRequest(unsigned char* pucBuffer,
                                      SULReceptionRequest* psULReceptionRequest)
{
    unsigned int uiIndex = 0;

    uiIndex += DecodeMassageHeader(pucBuffer, &(psULReceptionRequest->m_sMessageHeader));

    if((psULReceptionRequest->m_sMessageHeader).m_usType != ULRECEPREQ)
    {
        return 0;
    }
    else
    {
        uiIndex += Decode_TFURECPREQINFO_Parameter(pucBuffer + uiIndex,
            &(psULReceptionRequest->m_sTfuRecpReqInfo));
    }

    return uiIndex;
}


unsigned int DecodeDLDataRequest(unsigned char* pucBuffer, 
                                 SDLDataRequest* psDLDataRequest)
{
    unsigned int uiIndex = 0;

    uiIndex += DecodeMassageHeader(pucBuffer, &(psDLDataRequest-> m_sMessageHeader));

    if((psDLDataRequest-> m_sMessageHeader).m_usType != DLDATAREQ )
    {
        return 0;
    }
    else
    {
        uiIndex += Decode_TFUDATREQINFO_Parameter(pucBuffer + uiIndex, 
            &(psDLDataRequest-> m_sTfuDatReqInfo));
    }
    return uiIndex;
}

unsigned int DecodeULDataIndication(unsigned char* pucBuffer,
                                    SULDataIndication* psULDataIndication)
{
    unsigned int uiIndex = 0;

    uiIndex += DecodeMassageHeader(pucBuffer, &(psULDataIndication->m_sMessageHeader));

    if((psULDataIndication->m_sMessageHeader).m_usType != ULDATAIND)
    {
        return 0 ;
    }
    else
    {
        uiIndex += Decode_TFUDATINDINFO_Parameter(pucBuffer + uiIndex,
            &(psULDataIndication->m_sTfuDatIndInfo));
    }

    return uiIndex;
}

unsigned int DecodeULCRCIndication(unsigned char* pucBuffer,
                                   SULCRCIndication* psULCRCIndication)
{
    unsigned int uiIndex = 0;

    uiIndex += DecodeMassageHeader(pucBuffer, &(psULCRCIndication->m_sMessageHeader));
    
    if((psULCRCIndication->m_sMessageHeader).m_usType != ULCRCIND)
    {
        return 0;
    }
    else
    {
        uiIndex += Decode_TFUCRCINDINFO_Parameter(pucBuffer + uiIndex,
            &(psULCRCIndication->m_STfuCrcIndInfo));
    }

    return uiIndex;
}

unsigned int DecodeULCQIIndication(unsigned char* pucBuffer,
                                   SULCQIIndication* psULCQIIndication)
{
    unsigned int uiIndex = 0;
    
    uiIndex += DecodeMassageHeader(pucBuffer, &(psULCQIIndication->m_sMessageHeader));

    if((psULCQIIndication->m_sMessageHeader).m_usType != ULCQIIND)
    {
        return 0;
    }
    else
    {
        uiIndex += Decode_TFUULCQIINDINFO_Parameter(pucBuffer + uiIndex,
            &(psULCQIIndication->m_sTfuULCqiIndInfo));
    }

    return uiIndex;
}

unsigned int DecodeULSRIndication(unsigned char* pucBuffer,
                                   SULSRIndication* psULSRIndication)
{
    unsigned int uiIndex = 0;

    uiIndex += DecodeMassageHeader(pucBuffer, &(psULSRIndication->m_sMessageHeader));
    
    if((psULSRIndication->m_sMessageHeader).m_usType != ULSRIND)
    {
        return 0;
    }
    else
    {
        uiIndex += Decode_TFUSRINDINFO_Parameter(pucBuffer + uiIndex,
            &(psULSRIndication->m_sTfuSrIndInfo));
    }

    return uiIndex;
}

unsigned short GetMessageType(unsigned char* pucBuffer)
{
    unsigned int  uiIndex = 0;
    unsigned char ucValue = 0;
    unsigned char ucTemp = 0;
    unsigned short usValue = 0;
    unsigned short usRecovery = 0;

    ucValue = *(pucBuffer);
    uiIndex++;
    ucTemp = ucValue;
    ucTemp = ucTemp >> 5;

    ucTemp = ucValue;
    ucTemp = ucTemp << 3;
    ucTemp = ucTemp >> 5;

    ucTemp = ucValue;
    ucTemp = ucTemp << 6;
    ucTemp = ucTemp >> 6;
    usValue = usValue | ucTemp;

    ucTemp = 0;
    ucValue = *(pucBuffer + 1);
    uiIndex++;
    usRecovery = usRecovery | usValue;
    usRecovery = usRecovery << 14;
    usRecovery = usRecovery | ucValue ;

    return usRecovery;
}
