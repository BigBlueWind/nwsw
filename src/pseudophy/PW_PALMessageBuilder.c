/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#include "PW_PALFormats.h"
#include "PW_PALMessageBuilder.h"
#include "PW_PALParameterBuilder.h"
#include "PW_PALCommon.h"

unsigned int BuildMessageHeader(unsigned short usType, unsigned char* pucTxBuffer)
{
    unsigned char ucReserved;
    unsigned char ucVersion;
    unsigned short usMessageType;
    unsigned int uiMessageLength;
    unsigned int uiIndex = 0;
    unsigned char ucValue;
    unsigned short usValue;

    ucReserved = 0;
    ucVersion = PAL_VERSION;
    usMessageType = usType;

    *(pucTxBuffer + uiIndex) = 0;
    ucValue = ucReserved;
    ucValue = ucValue << 5;
    *(pucTxBuffer + uiIndex) = *(pucTxBuffer + uiIndex) | ucValue;

    ucValue = ucVersion;
    ucValue = ucValue << 5;
    ucValue = ucValue >> 3;
    *(pucTxBuffer + uiIndex) = *(pucTxBuffer + uiIndex) | ucValue;

    usValue = usMessageType;
    usValue = usValue << 6;
    usValue = usValue >> 14;
    ucValue = (unsigned char)usValue;
    *(pucTxBuffer + uiIndex) = *(pucTxBuffer + uiIndex) | ucValue;

    uiIndex++;

    *(pucTxBuffer + uiIndex) = 0;

    usValue = usMessageType;
    usValue = usValue << 8;
    usValue = usValue >> 8;
    ucValue = (unsigned char)usValue;
    *(pucTxBuffer + uiIndex) = *(pucTxBuffer + uiIndex) | ucValue;

    uiIndex++;

    uiIndex += GetBufferFromInt(pucTxBuffer + uiIndex, uiMessageLength);

    return uiIndex;
}

void AdjustMessageHeaderLength(unsigned int uiLength, unsigned char* pucTxBuffer)
{
    unsigned int uiIndex = 0;

    uiIndex += GetBufferFromInt(pucTxBuffer+2, uiLength) ;

}

unsigned int BuildDLControlRequest(unsigned char* pucTxBuffer,
                                   STfuCntrlReqInfo* psTfuCntrlReqInfo)
{
    unsigned int uiIndex = 0;

    uiIndex = BuildMessageHeader(SCHDCNTRLREQ, pucTxBuffer);

    uiIndex += Build_TFUCNTRLREQINFO_Parameter(pucTxBuffer + uiIndex,
        psTfuCntrlReqInfo);

    AdjustMessageHeaderLength(uiIndex, pucTxBuffer);

    return uiIndex;
}

unsigned int BuildRandomAccessIndication(unsigned char* pucTxBuffer,
                                         STfuRaReqIndInfo* psTfuRaReqIndInfo)
{
    unsigned int uiIndex = 0;

    uiIndex += BuildMessageHeader(RANDACSIND , pucTxBuffer);


    uiIndex += Build_TFURAREQINDINFO_Parameter(pucTxBuffer + uiIndex,
        psTfuRaReqIndInfo);


    AdjustParameterHeaderLength(uiIndex,  pucTxBuffer); 

    return uiIndex;
}

unsigned int BuildULReceptionRequest(unsigned char*pucTxBuffer,
                                     STfuRecpReqInfo* psTfuRecpReqInfo)
{
    unsigned int uiIndex = 0 ;

    uiIndex += BuildMessageHeader(ULRECEPREQ, pucTxBuffer);

    uiIndex += Build_TFURECPREQINFO_Parameter(pucTxBuffer + uiIndex, 
        psTfuRecpReqInfo);

    AdjustParameterHeaderLength(uiIndex, pucTxBuffer);

    return uiIndex;
}

unsigned int BuildDLDataRequest(unsigned char*pucTxBuffer,  
                                STfuDatReqInfo* psTfuDatReqInfo) 
{
    unsigned int uiIndex = 0;

    uiIndex += BuildMessageHeader(DLDATAREQ,pucTxBuffer);


    uiIndex += Build_TFUDATREQINFO_Parameter(pucTxBuffer + uiIndex,  
        psTfuDatReqInfo);

    AdjustParameterHeaderLength(uiIndex, pucTxBuffer);

    return uiIndex;

}

unsigned int BuildULDataIndication(unsigned char*pucTxBuffer,
                                   STfuDatIndInfo* psTfuDatIndInfo)
{
    unsigned int uiIndex = 0;

    uiIndex += BuildMessageHeader(ULDATAIND,pucTxBuffer);


    uiIndex += Build_TFUDATINDINFO_Parameter(pucTxBuffer + uiIndex, 
        psTfuDatIndInfo);

    AdjustParameterHeaderLength(uiIndex, pucTxBuffer);

    return uiIndex;
}

unsigned int BuildULCRCIndication(unsigned char* pucTxBuffer,
                                  STfuCrcIndInfo* psTfuCrcIndInfo)
{
    unsigned int uiIndex = 0;

    uiIndex += BuildMessageHeader(ULCRCIND, pucTxBuffer);

    uiIndex += Build_TFUCRCINDINFO_Parameter(pucTxBuffer + uiIndex,
        psTfuCrcIndInfo);

    AdjustParameterHeaderLength(uiIndex, pucTxBuffer);

    return uiIndex;

}

unsigned int BuildULCQIIndication(unsigned char* pucTxBuffer,
                                  STfuULCqiIndInfo* psTfuULCqiIndInfo)
{
    unsigned int uiIndex = 0;

    uiIndex += BuildMessageHeader(ULCQIIND, pucTxBuffer);

    uiIndex += Build_TFUULCQIINDINFO_Parameter(pucTxBuffer + uiIndex,
        psTfuULCqiIndInfo);

    AdjustParameterHeaderLength(uiIndex, pucTxBuffer);

    return uiIndex;
}

unsigned int BuildULSRIndication(unsigned char* pucTxBuffer,
                                  STfuSrIndInfo* psTfuSrIndInfo)
{
    unsigned int uiIndex = 0;

    uiIndex += BuildMessageHeader(ULSRIND, pucTxBuffer);

    uiIndex += Build_TFUSRINDINFO_Parameter(pucTxBuffer + uiIndex,
        psTfuSrIndInfo);

    AdjustParameterHeaderLength(uiIndex, pucTxBuffer);

    return uiIndex;
}
