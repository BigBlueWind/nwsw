/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "PW_PALFormats.h"
#include "PW_PALParameterParser.h"
#include "PW_PALCommon.h"
#include "MemoryDebug.h"

unsigned int DecodeParameterHeader(unsigned char* pucBuffer, SParameterHeader* psParameterHeader)
{
    unsigned int  uiIndex = 0, uiLength = 0;
    unsigned char ucValue = 0;
    unsigned char ucTemp = 0;
    unsigned short usValue = 0;
    unsigned short usRecovery = 0;

    ucValue = *(pucBuffer);
    uiIndex++;
    ucTemp = ucValue;
    ucTemp = ucTemp >> 2;

    psParameterHeader->m_ucReserved = ucTemp;

    ucTemp = ucValue;
    ucTemp = ucTemp << 6;
    ucTemp = ucTemp >> 6;
    usValue = usValue | ucTemp;

    ucTemp = 0;
    ucValue = *(pucBuffer + 1);
    uiIndex++;
    usRecovery = usRecovery | usValue;
    usRecovery = usRecovery << 8;
    usRecovery = usRecovery | ucValue ;


    psParameterHeader->m_usType = usRecovery;

    uiLength = GetIntFromBuffer(pucBuffer + 2); 

    uiIndex += 4;

    psParameterHeader->m_uiLength = uiLength;

    return uiIndex ;
}

unsigned int Decode_TimingInfo_Parameter(unsigned char* pucBuffer, STimingInfo* psTimingInfo)
{
    unsigned short usValue = 0;
    unsigned char ucValue  = 0;
    unsigned int uiIndex = 0 ; 

    usValue = GetShortFromBuffer(pucBuffer);
    uiIndex +=2;

    psTimingInfo-> m_usSFN = usValue;

    ucValue = *(pucBuffer + 2);
    uiIndex ++;

    psTimingInfo-> m_ucSubframe = ucValue;

    return uiIndex;
}





unsigned int Decode_TFUCNTRLREQINFO_Parameter(unsigned char* pucBuffer,
                                              STfuCntrlReqInfo* psTfuCntrlReqInfo)
{
    unsigned int uiIndex = 0, i = 0;
    unsigned short usNumberOfULPDCCHInfo = 0;
    unsigned short usNumberOfDLPDCCHInfo = 0;
    unsigned short usNumberOfPHICHINFO = 0;

    uiIndex += DecodeParameterHeader(pucBuffer, 
        &(psTfuCntrlReqInfo->m_sParameterHeader));

    if((psTfuCntrlReqInfo->m_sParameterHeader).m_usType != TFUCNTRLREQINFO)
    { 
        return 0;
    }
    else
    {
        psTfuCntrlReqInfo-> m_usCellId = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex += 2;

        uiIndex += Decode_TimingInfo_Parameter(pucBuffer + uiIndex,
            &(psTfuCntrlReqInfo->m_sULTimingInfo));

        uiIndex += Decode_TimingInfo_Parameter(pucBuffer + uiIndex,
            &(psTfuCntrlReqInfo->m_sDLTimingInfo));

        psTfuCntrlReqInfo->m_ucCFI = *(pucBuffer + uiIndex);
        uiIndex++;

        usNumberOfULPDCCHInfo = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex += 2;
        psTfuCntrlReqInfo->m_usNumberOfULPDCCHInfo = usNumberOfULPDCCHInfo;

        if(usNumberOfULPDCCHInfo > 0)
        {
            psTfuCntrlReqInfo->m_ppsULTFUPDCCHInfo = (struct STfuPdcchInfo**) UTILMalloc(sizeof
                (STfuPdcchInfo*)*usNumberOfULPDCCHInfo);

            for(i = 0; i < usNumberOfULPDCCHInfo; i++)
            {
                psTfuCntrlReqInfo->m_ppsULTFUPDCCHInfo[i] = (struct STfuPdcchInfo*)UTILMalloc(
                    sizeof(STfuPdcchInfo));

                uiIndex += Decode_TFUPDCCHINFO_Parameter(pucBuffer + uiIndex,
                    psTfuCntrlReqInfo->m_ppsULTFUPDCCHInfo[i]);

            }
        }

        usNumberOfDLPDCCHInfo = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex += 2;

        psTfuCntrlReqInfo->m_usNumberOfDLPDCCHInfo = usNumberOfDLPDCCHInfo;

        if(usNumberOfDLPDCCHInfo > 0)
        {
            psTfuCntrlReqInfo->m_ppsDLTFUPDCCHInfo = (struct STfuPdcchInfo**) UTILMalloc(sizeof
                (STfuPdcchInfo*)*usNumberOfDLPDCCHInfo);

            for( i = 0; i < usNumberOfDLPDCCHInfo; i++)
            {
                psTfuCntrlReqInfo->m_ppsDLTFUPDCCHInfo[i] = (struct STfuPdcchInfo*)UTILMalloc(
                    sizeof(STfuPdcchInfo));

                uiIndex += Decode_TFUPDCCHINFO_Parameter(pucBuffer + uiIndex,
                    psTfuCntrlReqInfo->m_ppsDLTFUPDCCHInfo[i]);
            }
        }
        usNumberOfPHICHINFO = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex += 2;

        psTfuCntrlReqInfo->m_usNumberOfPHICHINFO = usNumberOfPHICHINFO;

        if(usNumberOfPHICHINFO > 0)
        {

            psTfuCntrlReqInfo->m_ppsTfuPHICHInfo = (struct STfuPhiChInfo**) UTILMalloc(sizeof
                (STfuPhiChInfo*)*usNumberOfPHICHINFO);

            for( i = 0;  i < usNumberOfPHICHINFO; i++)
            {
                psTfuCntrlReqInfo->m_ppsTfuPHICHInfo[i] = (struct STfuPhiChInfo*) UTILMalloc(
                    sizeof(STfuPhiChInfo));

                uiIndex += Decode_TFUPHICHINFO_Parameter(pucBuffer + uiIndex ,
                    psTfuCntrlReqInfo->m_ppsTfuPHICHInfo[i]);
            }
        }
    }
    return uiIndex;

}
unsigned int Decode_TFUPDCCHINFO_Parameter(unsigned char* pucBuffer,
                                           STfuPdcchInfo* psTfuPdcchInfo)
{
    unsigned int uiIndex = 0;

    uiIndex += DecodeParameterHeader(pucBuffer,
        &(psTfuPdcchInfo->m_sParameterHeader));

    if((psTfuPdcchInfo->m_sParameterHeader).m_usType != TFUPDCCHINFO)
    { 
        return 0;
    }
    else
    {
        psTfuPdcchInfo->m_usRNTI = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex += 2;

        psTfuPdcchInfo->m_usCCE = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex += 2;

        psTfuPdcchInfo->m_ucAGGREGATIONLEVEL = *(pucBuffer + uiIndex);
        uiIndex++;  
    }
    return uiIndex;
}

unsigned int Decode_TFUPHICHINFO_Parameter(unsigned char* pucBuffer,
                                           STfuPhiChInfo* psTfuPhiChInfo)
{
    unsigned int uiIndex = 0;

    uiIndex += DecodeParameterHeader(pucBuffer, 
        &(psTfuPhiChInfo->m_sParameterHeader));

    if((psTfuPhiChInfo->m_sParameterHeader).m_usType != TFUPHICHINFO)
    { 
        return 0;
    }
    else
    {
        psTfuPhiChInfo->m_ucRBSTART = *(pucBuffer+uiIndex);
        uiIndex++;

        psTfuPhiChInfo->m_ucNDMRS = *(pucBuffer+uiIndex);
        uiIndex++;

        psTfuPhiChInfo->m_ucISACK = *(pucBuffer+uiIndex);
        uiIndex++;
    }

    return uiIndex ;
}
unsigned int Decode_TFURAREQINDINFO_Parameter(unsigned char* pucBuffer,
                                              STfuRaReqIndInfo* psTfuRaReqIndInfo)
{
    unsigned int uiIndex = 0, i = 0;
    unsigned short usCellId = 0;
    unsigned short usNumberOfTFURACHINFO = 0;

    uiIndex =  DecodeParameterHeader(pucBuffer, 
        &(psTfuRaReqIndInfo->m_sParameterHeader));

    if((psTfuRaReqIndInfo->m_sParameterHeader).m_usType != TFURAREQINDINFO)
    { 
        return 0;
    }
    else
    {
        usCellId = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex += 2;

        psTfuRaReqIndInfo-> m_usCellId = usCellId;

        uiIndex += Decode_TimingInfo_Parameter(pucBuffer + uiIndex, 
            &(psTfuRaReqIndInfo->m_sTimingInfo));

        usNumberOfTFURACHINFO = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex += 2;

        psTfuRaReqIndInfo->m_usNumberOfTFURACHINFO = usNumberOfTFURACHINFO;

        if(usNumberOfTFURACHINFO > 0)
        {

            psTfuRaReqIndInfo-> m_ppsTfuRachInfo =  (struct STfuRachInfo**) UTILMalloc((sizeof      
                (STfuRachInfo*))*usNumberOfTFURACHINFO);
            for(i = 0 ; i < usNumberOfTFURACHINFO; i++)
            {
                psTfuRaReqIndInfo-> m_ppsTfuRachInfo[i] = (struct STfuRachInfo*) UTILMalloc(
                    sizeof(STfuRachInfo));

                uiIndex += Decode_TFURACHINFO_Parameter(pucBuffer + uiIndex, 
                    psTfuRaReqIndInfo-> m_ppsTfuRachInfo[i]);

            }
        }
    }
    return uiIndex;
}

unsigned int Decode_TFURACHINFO_Parameter(unsigned char* pucBuffer,
                                          STfuRachInfo* psTfuRachInfo)
{
    unsigned int uiIndex = 0, i = 0;  
    unsigned short usRaRNTI = 0;
    unsigned short usNumberOfTFURAREQINFO = 0;


    uiIndex =   DecodeParameterHeader(pucBuffer,
        &(psTfuRachInfo->m_sParameterHeader));


    if((psTfuRachInfo->m_sParameterHeader).m_usType != TFURACHINFO)
    { 
        return 0;
    }
    else
    {

        usRaRNTI = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex += 2;

        psTfuRachInfo-> m_usRaRNTI = usRaRNTI;

        usNumberOfTFURAREQINFO = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex += 2;

        psTfuRachInfo->m_usNumberOfTFURAREQINFO = usNumberOfTFURAREQINFO; 

        if(usNumberOfTFURAREQINFO > 0)
        {

            psTfuRachInfo-> m_ppsTfuRaReqInfo = (struct STfuRaReqInfo**)UTILMalloc(
                (sizeof (STfuRaReqInfo*))*usNumberOfTFURAREQINFO);

            for( i = 0 ; i < usNumberOfTFURAREQINFO; i++)
            {
                psTfuRachInfo-> m_ppsTfuRaReqInfo[i] = ( struct STfuRaReqInfo*)UTILMalloc(
                    sizeof(STfuRaReqInfo));

                uiIndex += Decode_TFURAREQINFO_Parameter(pucBuffer+uiIndex, 
                    psTfuRachInfo-> m_ppsTfuRaReqInfo[i]);
            }
        }
    }

    return uiIndex;

}

unsigned int Decode_TFURAREQINFO_Parameter(unsigned char*pucBuffer, 
                                           STfuRaReqInfo* psTfuRaReqInfo)
{
    unsigned int uiIndex = 0;
    unsigned char ucPreambleId = 0;
    unsigned short usTimingAdvance = 0;


    uiIndex =  DecodeParameterHeader(pucBuffer, 
        &(psTfuRaReqInfo->m_sParameterHeader));

    if((psTfuRaReqInfo->m_sParameterHeader).m_usType != TFURAREQINFO)
    { 
        return 0;
    }
    else
    {
        ucPreambleId = *(pucBuffer + uiIndex);
        uiIndex++;

        psTfuRaReqInfo->m_ucPreambleId = ucPreambleId;

        usTimingAdvance = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex += 2;

        psTfuRaReqInfo->m_usTimingAdvance = usTimingAdvance;
    }
    return uiIndex;

}

unsigned int Decode_TFURECPREQINFO_Parameter(unsigned char* pucBuffer,
                                             STfuRecpReqInfo* psTfuRecpReqInfo)
{
    unsigned int uiIndex = 0, i = 0;
    unsigned short usNumberOfUE = 0;

    uiIndex += DecodeParameterHeader(pucBuffer,
        &(psTfuRecpReqInfo->m_sParameterHeader));

    if((psTfuRecpReqInfo->m_sParameterHeader).m_usType != TFURECPREQINFO)
    { 
        return 0;
    }
    else
    {
        psTfuRecpReqInfo->m_usCellId = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex +=2;

        usNumberOfUE = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex +=2;

        psTfuRecpReqInfo->m_usNumberOfUE = usNumberOfUE;

        if(usNumberOfUE > 0)
        {

            psTfuRecpReqInfo->m_ppsTfuUeRecpReqInfo =  (struct STfuUeRecpReqInfo**) UTILMalloc((sizeof      
                (STfuUeRecpReqInfo*))*usNumberOfUE);
            for(i = 0; i < usNumberOfUE; i++)
            {
                psTfuRecpReqInfo->m_ppsTfuUeRecpReqInfo[i] =  (struct STfuUeRecpReqInfo*) UTILMalloc(
                    sizeof (STfuUeRecpReqInfo));

                uiIndex += Decode_TFUUERECPREQINFO_Parameter(pucBuffer + uiIndex,
                    psTfuRecpReqInfo->m_ppsTfuUeRecpReqInfo[i]);
            }
        }
    }
    return uiIndex;
}
unsigned int Decode_TFUUERECPREQINFO_Parameter(unsigned char* pucBuffer,
                                               STfuUeRecpReqInfo* psTfuUeRecpReqInfo)
{
    unsigned int uiIndex = 0;

    uiIndex += DecodeParameterHeader(pucBuffer,
        &(psTfuUeRecpReqInfo->m_sParameterHeader));

    if((psTfuUeRecpReqInfo->m_sParameterHeader).m_usType != TFUUERECPREQINFO)
    { 
        return 0;
    }
    else
    {
        psTfuUeRecpReqInfo->m_usRNTI = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex +=2;
    }

    return uiIndex;
}

unsigned int Decode_TFUDATREQINFO_Parameter( unsigned char* pucBuffer, 
                                            STfuDatReqInfo* psTfuDatReqInfo )
{
    unsigned int uiIndex = 0 , i = 0;
    unsigned short usCellId = 0;
    unsigned short usLengthOfBCHBuffer = 0;
    unsigned short usNumberOfTFUDATREQPDUINFO = 0;

    uiIndex = DecodeParameterHeader(pucBuffer, 
        &(psTfuDatReqInfo-> m_sParameterHeader));

    if((psTfuDatReqInfo->m_sParameterHeader).m_usType != TFUDATREQINFO)
    { 
        return 0;
    }
    else
    {
        usCellId = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex +=2;

        psTfuDatReqInfo-> m_usCellId = usCellId;

        uiIndex += Decode_TimingInfo_Parameter(pucBuffer + uiIndex,
            &(psTfuDatReqInfo->m_sTimingInfo));

        usLengthOfBCHBuffer = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex +=2;
        psTfuDatReqInfo->m_usLengthOfBCHBuffer = usLengthOfBCHBuffer;

        if(usLengthOfBCHBuffer > 0)
        {
            psTfuDatReqInfo->m_pucBCHBuffer = (unsigned char*)UTILMalloc(
                sizeof(char)*usLengthOfBCHBuffer);
        }

        UTILMemcpy(psTfuDatReqInfo->m_pucBCHBuffer, pucBuffer + uiIndex, usLengthOfBCHBuffer);
        uiIndex += usLengthOfBCHBuffer;

        usNumberOfTFUDATREQPDUINFO = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex +=2;
        psTfuDatReqInfo->m_usNumberOfTFUDATREQPDUINFO = usNumberOfTFUDATREQPDUINFO; 

        if(usNumberOfTFUDATREQPDUINFO > 0)
        {

            psTfuDatReqInfo->m_ppsTfuDatReqPduInfo = (struct STfuDatReqPduInfo**)UTILMalloc
                ((sizeof(STfuDatReqPduInfo*))*usNumberOfTFUDATREQPDUINFO);
            for(i = 0; i < usNumberOfTFUDATREQPDUINFO; i++)
            {
                psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[i] = (struct STfuDatReqPduInfo*)UTILMalloc
                    (sizeof(STfuDatReqPduInfo));
                uiIndex += Decode_TFUDATREQPDUINFO_Parameter(pucBuffer + uiIndex, 
                    psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[i]);
            }
        }

    }
    return uiIndex;
}

unsigned int Decode_TFUDATREQPDUINFO_Parameter(unsigned char* pucBuffer,
                                               STfuDatReqPduInfo* psTfuDatReqPduInfo)
{
    unsigned int uiIndex = 0, i = 0;
    unsigned short usRNTI = 0;
    unsigned char ucNumberOfTB = 0;

    uiIndex = DecodeParameterHeader(pucBuffer, 
        &(psTfuDatReqPduInfo-> m_sParameterHeader));

    if((psTfuDatReqPduInfo->m_sParameterHeader).m_usType != TFUDATREQPDUINFO)
    { 
        return 0;
    }
    else
    {

        usRNTI = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex +=2;
        psTfuDatReqPduInfo-> m_usRNTI = usRNTI;

        uiIndex += Decode_TFUPDSCHDCIINFO_Parameter(pucBuffer + uiIndex, 
            &(psTfuDatReqPduInfo->m_sTfuPdschDciInfo));

        ucNumberOfTB = *(pucBuffer + uiIndex);
        uiIndex ++;
        psTfuDatReqPduInfo-> m_ucNumberOfTB = ucNumberOfTB;

        if(ucNumberOfTB > 0)
        {

            psTfuDatReqPduInfo-> m_ppucMacPduBuffer = (unsigned char**)UTILMalloc(
                (sizeof(char*)*ucNumberOfTB));
            psTfuDatReqPduInfo->m_puiMacPdubufferLen = (unsigned int*)UTILMalloc(
                (sizeof(int)*ucNumberOfTB));

            for(i = 0; i < ucNumberOfTB; i++)
            {
                psTfuDatReqPduInfo->m_puiMacPdubufferLen[i] = GetIntFromBuffer(pucBuffer+uiIndex);
                uiIndex +=4;
            }
            for(i = 0; i < ucNumberOfTB; i++)
            {
                psTfuDatReqPduInfo-> m_ppucMacPduBuffer[i] = (unsigned char*)UTILMalloc(
                    sizeof(char)*(psTfuDatReqPduInfo->m_puiMacPdubufferLen[i]));

                UTILMemcpy(psTfuDatReqPduInfo-> m_ppucMacPduBuffer[i], pucBuffer + uiIndex, 
                    psTfuDatReqPduInfo->m_puiMacPdubufferLen[i]);

                uiIndex += psTfuDatReqPduInfo->m_puiMacPdubufferLen[i];
            }

        }
        else
        {
            /* Some log Error has to be implemented */
        }

    }
    return uiIndex;

}

unsigned int Decode_TFUPDSCHDCIINFO_Parameter(unsigned char* pucBuffer, 
                                              STfuPdschDciInfo * psTfuPdschDciInfo)
{
    unsigned int uiIndex = 0;

    uiIndex += DecodeParameterHeader(pucBuffer, 
        &(psTfuPdschDciInfo-> m_sParameterHeader));

    if((psTfuPdschDciInfo->m_sParameterHeader).m_usType != TFUPDSCHDCIINFO)
    { 
        return 0;
    }
    else
    {
        /* For future implementation */
    }

    return uiIndex;
}


unsigned int Decode_TFUDATINDINFO_Parameter(unsigned char* pucBuffer,
                                            STfuDatIndInfo* psTfuDatIndInfo)
{
    unsigned int uiIndex = 0, i = 0;
    unsigned short usNumberOfTFUDATINFO = 0;

    uiIndex += DecodeParameterHeader(pucBuffer,
        &(psTfuDatIndInfo->m_sParameterHeader));

    if((psTfuDatIndInfo->m_sParameterHeader).m_usType != TFUDATINDINFO)
    { 
        return 0;
    }
    else
    {
        psTfuDatIndInfo->m_usCellId = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex +=2;

        uiIndex += Decode_TimingInfo_Parameter(pucBuffer + uiIndex,
            &(psTfuDatIndInfo->m_sTimingInfo));

        usNumberOfTFUDATINFO = GetShortFromBuffer(pucBuffer+uiIndex);
        uiIndex +=2;

        if(usNumberOfTFUDATINFO > 0)
        {

            psTfuDatIndInfo->m_ppsTfuDatInfo = (struct STfuDatInfo**) UTILMalloc(
                sizeof(STfuDatInfo*)*usNumberOfTFUDATINFO);
            for( i = 0; i < usNumberOfTFUDATINFO; i++)
            {
                psTfuDatIndInfo->m_ppsTfuDatInfo[i] = (struct STfuDatInfo*) UTILMalloc(
                    sizeof(STfuDatInfo));

                uiIndex += Decode_TFUDATINFO_Parameter(pucBuffer + uiIndex,
                    psTfuDatIndInfo->m_ppsTfuDatInfo[i]);
            }
        }


    }

    return uiIndex;
}
unsigned int Decode_TFUDATINFO_Parameter(unsigned char* pucBuffer,
                                         STfuDatInfo* psTfuDatInfo)
{
    unsigned int uiIndex = 0;
    unsigned int uiDataPduLength = 0;
    uiIndex += DecodeParameterHeader(pucBuffer,
        &(psTfuDatInfo->m_sParameterHeader));

    if((psTfuDatInfo->m_sParameterHeader).m_usType != TFUDATINFO)
    { 
        return 0;
    }
    else
    {
        psTfuDatInfo->m_usRNTI = GetShortFromBuffer(pucBuffer + uiIndex);
        uiIndex +=2;

        uiDataPduLength = GetIntFromBuffer(pucBuffer + uiIndex);
        uiIndex +=4;
        psTfuDatInfo->m_uiDataPduLength = uiDataPduLength;

        if( uiDataPduLength > 0)
        {

            psTfuDatInfo->m_pucDataPduBuffer = (unsigned char*)UTILMalloc(
                sizeof(char)*uiDataPduLength);

            UTILMemcpy(psTfuDatInfo->m_pucDataPduBuffer, pucBuffer + uiIndex, uiDataPduLength);
            uiIndex += uiDataPduLength;
        }

    }
    return uiIndex;
}

unsigned int Decode_TFUCRCINDINFO_Parameter(unsigned char* pucBuffer,
                                            STfuCrcIndInfo* psTfuCrcIndInfo)
{
    unsigned int uiIndex = 0, i = 0;
    unsigned short usNumberOfTFUCRCINFO = 0;

    uiIndex += DecodeParameterHeader(pucBuffer,
        &(psTfuCrcIndInfo->m_sParameterHeader));

    if((psTfuCrcIndInfo->m_sParameterHeader).m_usType != TFUCRCINDINFO)
    { 
        return 0;
    }
    else
    {
        psTfuCrcIndInfo->m_usCellId = GetShortFromBuffer(pucBuffer + uiIndex);
        uiIndex +=2;

        uiIndex += Decode_TimingInfo_Parameter(pucBuffer + uiIndex,
            &(psTfuCrcIndInfo->m_sTimingInfo));

        usNumberOfTFUCRCINFO = GetShortFromBuffer(pucBuffer + uiIndex);
        uiIndex +=2;

        if(usNumberOfTFUCRCINFO > 0)
        {

            psTfuCrcIndInfo->m_ppsTfuCrcInfo = (struct STfuCrcInfo**) UTILMalloc(
                sizeof(STfuCrcInfo*)*usNumberOfTFUCRCINFO);

            for( i = 0; i < usNumberOfTFUCRCINFO; i++)
            {
                psTfuCrcIndInfo->m_ppsTfuCrcInfo[i] = (struct STfuCrcInfo*) UTILMalloc(
                    sizeof(STfuCrcInfo));

                uiIndex += Decode_TFUCRCINFO_Parameter(pucBuffer + uiIndex,
                    psTfuCrcIndInfo->m_ppsTfuCrcInfo[i]);

            }
        }

    }
    return uiIndex;
}

unsigned int Decode_TFUCRCINFO_Parameter( unsigned char* pucBuffer,
                                         STfuCrcInfo* psTfuCrcInfo)
{
    unsigned int uiIndex = 0;

    uiIndex += DecodeParameterHeader(pucBuffer,
        &(psTfuCrcInfo->m_sParameterHeader));

    if((psTfuCrcInfo->m_sParameterHeader).m_usType != TFUCRCINFO)
    { 
        return 0;
    }
    else
    {
        psTfuCrcInfo->m_usRNTI = GetShortFromBuffer(pucBuffer + uiIndex);
        uiIndex +=2;

        psTfuCrcInfo->m_ucISFAILURE = *(pucBuffer + uiIndex);
        uiIndex++;

        psTfuCrcInfo->m_ucRV = *(pucBuffer + uiIndex);
        uiIndex++;

    }
    return uiIndex;
}

unsigned int Decode_TFUULCQIINDINFO_Parameter(unsigned char* pucBuffer,
                                              STfuULCqiIndInfo* psTfuULCqiIndInfo)
{
    unsigned int uiIndex = 0, i = 0;
    unsigned short usNumberOFTFUULCQIRPT = 0;

    uiIndex += DecodeParameterHeader(pucBuffer,
        &(psTfuULCqiIndInfo->m_sParameterHeader));

    if((psTfuULCqiIndInfo->m_sParameterHeader).m_usType != TFUULCQIINDINFO)
    { 
        return 0;
    }
    else
    {
        psTfuULCqiIndInfo->m_usCellId = GetShortFromBuffer(pucBuffer + uiIndex);
        uiIndex +=2;

        uiIndex += Decode_TimingInfo_Parameter(pucBuffer + uiIndex,
            &(psTfuULCqiIndInfo->m_sTimingInfo));

        usNumberOFTFUULCQIRPT = GetShortFromBuffer(pucBuffer + uiIndex);
        uiIndex +=2;

        if(usNumberOFTFUULCQIRPT > 0)
        {

            psTfuULCqiIndInfo->m_ppsTfuULCqiRpt = (struct STfuULCqiRpt**) UTILMalloc(
                sizeof(STfuULCqiRpt*)*usNumberOFTFUULCQIRPT);
            for( i = 0; i < usNumberOFTFUULCQIRPT; i++)
            {
                psTfuULCqiIndInfo->m_ppsTfuULCqiRpt[i] = (struct STfuULCqiRpt*)UTILMalloc(
                    sizeof(STfuULCqiRpt));

                uiIndex += Decode_TFUULCQIRPT_Parameter(pucBuffer + uiIndex,
                    psTfuULCqiIndInfo->m_ppsTfuULCqiRpt[i]);
            }        
        }
    }
    return uiIndex;
}
unsigned int Decode_TFUULCQIRPT_Parameter(unsigned char* pucBuffer,
                                          STfuULCqiRpt* psTfuULCqiRpt)
{
    unsigned int uiIndex = 0, i = 0;
    unsigned int uiTemp = 0;
    unsigned char ucNumberOfSubBands = 0;

    uiIndex += DecodeParameterHeader(pucBuffer,
        &(psTfuULCqiRpt->m_sParameterHeader));


    if((psTfuULCqiRpt->m_sParameterHeader).m_usType != TFUULCQIRPT)
    { 
        return 0;
    }
    else
    {
        psTfuULCqiRpt->m_usRNTI = GetShortFromBuffer(pucBuffer + uiIndex);
        uiIndex +=2;

        psTfuULCqiRpt->m_ucISTXPORT0 = *(pucBuffer + uiIndex);
        uiIndex++;

        psTfuULCqiRpt->m_ucWideCqi = *(pucBuffer + uiIndex);
        uiIndex++;

        ucNumberOfSubBands = *(pucBuffer + uiIndex);
        uiIndex++;
        psTfuULCqiRpt->m_ucNumberOfSubBands = ucNumberOfSubBands;

        if(ucNumberOfSubBands > 0)
        {

            psTfuULCqiRpt->m_ppsTfuULCqiInfo = (struct STfuULCqiInfo**) UTILMalloc(
                sizeof(STfuULCqiInfo*)*ucNumberOfSubBands);

            if( ucNumberOfSubBands <= TFU_MAX_UL_SUBBAND)
            {
                for(i = 0; i < ucNumberOfSubBands; i++)
                {
                    psTfuULCqiRpt->m_ppsTfuULCqiInfo[i] = (struct STfuULCqiInfo*)UTILMalloc(
                        sizeof(STfuULCqiInfo));

                    uiTemp = Decode_TFUULCQIINFO_Parameter(pucBuffer + uiIndex,
                        psTfuULCqiRpt->m_ppsTfuULCqiInfo[i]);

                    uiIndex += uiTemp;
                }

                uiIndex += (uiTemp*(TFU_MAX_UL_SUBBAND - ucNumberOfSubBands));

            }
        }
        else
        {
            /* For Log Error */
        }
    }
    return uiIndex;
}

unsigned int  Decode_TFUULCQIINFO_Parameter(unsigned char* pucBuffer,
                                            STfuULCqiInfo* psTfuULCqiInfo)
{
    unsigned int uiIndex = 0;

    uiIndex += DecodeParameterHeader(pucBuffer,
        &(psTfuULCqiInfo->m_sParameterHeader));

    if((psTfuULCqiInfo->m_sParameterHeader).m_usType != TFUULCQIINFO)
    { 
        return 0;
    }
    else
    {
        uiIndex += Decode_TFUSUBBANDCQIINFO_Parameter(pucBuffer + uiIndex,
            &(psTfuULCqiInfo->m_sTfuSubBandCqiInfo));
    }
    return uiIndex;
}
unsigned int Decode_TFUSUBBANDCQIINFO_Parameter(unsigned char* pucBuffer,
                                                STfuSubBandCqiInfo* psTfuSubBandCqiInfo)
{
    unsigned int uiIndex = 0;

    uiIndex += DecodeParameterHeader(pucBuffer,
        &(psTfuSubBandCqiInfo->m_sParameterHeader));

    if((psTfuSubBandCqiInfo->m_sParameterHeader).m_usType != TFUSUBBANDCQIINFO)
    { 
        return 0;
    }
    else
    {

        uiIndex += Decode_TFUSUBBANDINFO_Parameter(pucBuffer + uiIndex,
            &(psTfuSubBandCqiInfo->m_sTfuSubBandInfo));

        psTfuSubBandCqiInfo->m_ucCQIIndex = *(pucBuffer + uiIndex);
        uiIndex++;
    }

    return uiIndex;
}
unsigned int Decode_TFUSUBBANDINFO_Parameter(unsigned char* pucBuffer,
                                             STfuSubBandInfo* psTfuSubBandInfo)
{
    unsigned int uiIndex = 0;

    uiIndex += DecodeParameterHeader(pucBuffer,
        &(psTfuSubBandInfo->m_sParameterHeader));

    if((psTfuSubBandInfo->m_sParameterHeader).m_usType != TFUSUBBANDINFO)
    { 
        return 0;
    }
    else
    {
        psTfuSubBandInfo->m_ucRBSTART = *(pucBuffer + uiIndex);
        uiIndex++;

        psTfuSubBandInfo->m_ucNumberOfRB = *(pucBuffer + uiIndex);
        uiIndex++;
    }
    return uiIndex;
}

unsigned int Decode_TFUSRINDINFO_Parameter(unsigned char* pucBuffer,
                                           STfuSrIndInfo* psTfuSrIndInfo)
{
    unsigned int uiIndex = 0, i = 0;
    unsigned short usNumberOfTFUSRINFO = 0;

    uiIndex += DecodeParameterHeader(pucBuffer,
        &(psTfuSrIndInfo->m_sParameterHeader));

    if((psTfuSrIndInfo->m_sParameterHeader).m_usType != TFUSRINDINFO)
    {
        return 0;
    }
    else
    {
        psTfuSrIndInfo->m_usCellId = GetShortFromBuffer(pucBuffer + uiIndex);
        uiIndex +=2;

        uiIndex += Decode_TimingInfo_Parameter(pucBuffer + uiIndex,
            &(psTfuSrIndInfo->m_sTimingInfo));

        usNumberOfTFUSRINFO = GetShortFromBuffer(pucBuffer + uiIndex);
        uiIndex +=2;

        if(usNumberOfTFUSRINFO > 0)
        {
            psTfuSrIndInfo->m_ppsTfuSrInfo = (struct STfuSrInfo**) UTILMalloc(
                sizeof(STfuSrInfo*)*usNumberOfTFUSRINFO);

            for( i = 0; i < usNumberOfTFUSRINFO; i++)
            {
                psTfuSrIndInfo->m_ppsTfuSrInfo[i] = (struct STfuSrInfo*) UTILMalloc(
                    sizeof(STfuSrInfo));

                uiIndex += Decode_TFUSRINFO_Parameter(pucBuffer + uiIndex,
                    psTfuSrIndInfo->m_ppsTfuSrInfo[i]);
            }
        }
    }

    return uiIndex;
}

unsigned int Decode_TFUSRINFO_Parameter(unsigned char* pucBuffer,
                                        STfuSrInfo* psTfuSrInfo)
{
    unsigned int uiIndex = 0;

    uiIndex += DecodeParameterHeader(pucBuffer,
        &(psTfuSrInfo->m_sParameterHeader));

    if((psTfuSrInfo->m_sParameterHeader).m_usType != TFUSRINFO)
    { 
        return 0;
    }
    else
    {
        psTfuSrInfo->m_usRNTI = GetShortFromBuffer(pucBuffer + uiIndex);
        uiIndex +=2;
    }

    return uiIndex;
}
