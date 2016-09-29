/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#include "PW_PALFormats.h"
#include "PW_PALParameterBuilder.h"
#include "PW_PALCommon.h"
#include "MemoryDebug.h"
#include <string.h>

unsigned int BuildParameter(unsigned short usType, unsigned char* pucTxBuffer)
{
    unsigned short  uiIndex = 0;

    uiIndex = BuildParameterHeader(usType, pucTxBuffer);

    /* 
    Calling each parameter funtion  using switch cases.

    */
    return uiIndex; 

}

unsigned int BuildParameterHeader(unsigned short usType, unsigned char* pucTxBuffer)
{
    unsigned char ucReserved;
    unsigned short usParameterType;
    unsigned int uiParameterLength;
    unsigned char ucValue = 0;
    unsigned int uiIndex = 0;
    unsigned int usValue;


    ucReserved = 0;
    usParameterType = usType;


    *(pucTxBuffer + uiIndex) = 0;

    ucValue = ucReserved;
    ucValue = ucValue << 2;
    *(pucTxBuffer + uiIndex) = *(pucTxBuffer + uiIndex) | ucValue;

    usValue = usParameterType;
    usValue = usValue << 6;
    usValue = usValue >> 14;
    ucValue = (unsigned char ) usValue;
    *(pucTxBuffer + uiIndex) = *(pucTxBuffer + uiIndex) | ucValue;

    uiIndex++;

    *(pucTxBuffer + uiIndex) = 0;

    usValue = usParameterType;
    usValue = usValue << 8;
    usValue = usValue >> 8;
    ucValue = (unsigned char) usValue;
    *(pucTxBuffer +uiIndex) = *(pucTxBuffer + uiIndex) | ucValue;

    uiIndex++;

    uiIndex += GetBufferFromInt(pucTxBuffer + uiIndex, uiParameterLength);
    return uiIndex;
}

void  AdjustParameterHeaderLength(unsigned int uiLength, unsigned char* pucTxBuffer)
{
    unsigned int uiIndex = 0;
    uiIndex += GetBufferFromInt(pucTxBuffer + 2, uiLength);
}

unsigned int Build_TFUCNTRLREQINFO_Parameter(unsigned char* pucTxBuffer,
                                             STfuCntrlReqInfo* psTfuCntrlReqInfo)
{
    unsigned int uiIndex = 0, i = 0;
    unsigned char ucValue = 0;
    unsigned short usNumberOfULPDCCHInfo = 0;
    unsigned short usNumberOfDLPDCCHInfo = 0;
    unsigned short usNumberOfPHICHINFO = 0;

    usNumberOfULPDCCHInfo = psTfuCntrlReqInfo->m_usNumberOfULPDCCHInfo;
    usNumberOfDLPDCCHInfo = psTfuCntrlReqInfo->m_usNumberOfDLPDCCHInfo;
    usNumberOfPHICHINFO = psTfuCntrlReqInfo->m_usNumberOfPHICHINFO;

    uiIndex = BuildParameterHeader(TFUCNTRLREQINFO, pucTxBuffer);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex,
        psTfuCntrlReqInfo->m_usCellId);

    uiIndex += Build_CMLTETIMINGINFO_Parameter(pucTxBuffer + uiIndex, 
        &(psTfuCntrlReqInfo->m_sULTimingInfo));
    
    uiIndex += Build_CMLTETIMINGINFO_Parameter(pucTxBuffer + uiIndex, 
        &(psTfuCntrlReqInfo->m_sDLTimingInfo));

    *(pucTxBuffer + uiIndex ) = 0;
    ucValue = psTfuCntrlReqInfo->m_ucCFI ;
    *(pucTxBuffer + uiIndex ) = *(pucTxBuffer + uiIndex )|ucValue;
    uiIndex ++;

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, usNumberOfULPDCCHInfo);

    for(i = 0; i < usNumberOfULPDCCHInfo; i++)
    {
        uiIndex += Build_TFUPDCCHINFO_Parameter(pucTxBuffer + uiIndex,
            psTfuCntrlReqInfo->m_ppsULTFUPDCCHInfo[i]);
    }
    
    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, usNumberOfDLPDCCHInfo);

    for(i = 0; i < usNumberOfDLPDCCHInfo; i++)
    {
        uiIndex += Build_TFUPDCCHINFO_Parameter(pucTxBuffer + uiIndex,
            psTfuCntrlReqInfo->m_ppsULTFUPDCCHInfo[i]);
    }
    
    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, usNumberOfPHICHINFO);
    
    for( i = 0; i < usNumberOfPHICHINFO; i++)
    {
        uiIndex += Build_TFUPHICHINFO_Parameter(pucTxBuffer + uiIndex,
            psTfuCntrlReqInfo->m_ppsTfuPHICHInfo[i]);
    }


    AdjustParameterHeaderLength(uiIndex,  pucTxBuffer);

    return uiIndex;
}

unsigned int Build_TFUPDCCHINFO_Parameter(unsigned char* pucTxBuffer,  
                                          STfuPdcchInfo* psTfuPdcchInfo) 
{
    unsigned int uiIndex = 0;
    unsigned char ucValue = 0;

    uiIndex += BuildParameterHeader(TFUPDCCHINFO, pucTxBuffer);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex,  psTfuPdcchInfo->m_usRNTI);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, psTfuPdcchInfo->m_usCCE);

    *(pucTxBuffer + uiIndex ) = 0;
    ucValue = psTfuPdcchInfo->m_ucAGGREGATIONLEVEL;
    *(pucTxBuffer + uiIndex ) = *(pucTxBuffer + uiIndex )|ucValue;
    uiIndex ++;

    AdjustParameterHeaderLength(uiIndex,  pucTxBuffer);


    return uiIndex;
}

unsigned int Build_TFUPHICHINFO_Parameter(unsigned char* pucTxBuffer, 
                                          STfuPhiChInfo* psTfuPhiChInfo)
{
    unsigned int uiIndex = 0;
    unsigned char ucValue = 0;

    uiIndex = BuildParameterHeader(TFUPHICHINFO, pucTxBuffer);

    *(pucTxBuffer + uiIndex ) = 0;
    ucValue =  psTfuPhiChInfo->m_ucRBSTART;
    *(pucTxBuffer + uiIndex ) = *(pucTxBuffer + uiIndex )|ucValue;
    uiIndex ++;

    *(pucTxBuffer + uiIndex ) = 0;
    ucValue = psTfuPhiChInfo->m_ucNDMRS ;
    *(pucTxBuffer + uiIndex ) = *(pucTxBuffer + uiIndex )|ucValue;
    uiIndex ++;

    *(pucTxBuffer + uiIndex ) = 0;
    ucValue =  psTfuPhiChInfo->m_ucISACK;
    *(pucTxBuffer + uiIndex ) = *(pucTxBuffer + uiIndex )|ucValue;
    uiIndex ++;

    AdjustParameterHeaderLength(uiIndex,  pucTxBuffer);

    return uiIndex;
}


unsigned int Build_TFURAREQINDINFO_Parameter(unsigned char* pucTxBuffer,
                                             STfuRaReqIndInfo* psTfuRaReqIndInfo)
{
    unsigned int uiIndex = 0,i=0;
    unsigned short usNumberOfTFURACHINFO = 0;
    usNumberOfTFURACHINFO = psTfuRaReqIndInfo->m_usNumberOfTFURACHINFO;
        
    uiIndex = BuildParameterHeader(TFURAREQINDINFO, pucTxBuffer);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, psTfuRaReqIndInfo->m_usCellId);

    uiIndex += Build_CMLTETIMINGINFO_Parameter(pucTxBuffer + uiIndex, 
        &(psTfuRaReqIndInfo->m_sTimingInfo) );

    uiIndex += GetBufferFromShort(pucTxBuffer +uiIndex, usNumberOfTFURACHINFO );

    for(i = 0; i < usNumberOfTFURACHINFO; i++)
    {
        uiIndex += Build_TFURACHINFO_Parameter(pucTxBuffer + uiIndex,
            psTfuRaReqIndInfo->m_ppsTfuRachInfo[i]);
            
    }

    AdjustParameterHeaderLength(uiIndex,  pucTxBuffer); 

    return uiIndex;
}

unsigned int Build_TFURACHINFO_Parameter(unsigned char* pucTxBuffer, 
                                         STfuRachInfo* psTfuRachInfo) 
{
    unsigned int uiIndex = 0, i = 0;
    unsigned short usNumberOfTFURAREQINFO = 0;
    usNumberOfTFURAREQINFO  = psTfuRachInfo->m_usNumberOfTFURAREQINFO;

    uiIndex = BuildParameterHeader(TFURACHINFO, pucTxBuffer);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, psTfuRachInfo->m_usRaRNTI);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, usNumberOfTFURAREQINFO);

    for(i = 0; i < usNumberOfTFURAREQINFO; i++)
    {
        uiIndex += Build_TFURAREQINFO_Parameter(pucTxBuffer + uiIndex,  
            psTfuRachInfo->m_ppsTfuRaReqInfo[i]);
    }

    AdjustParameterHeaderLength(uiIndex,  pucTxBuffer);

    return uiIndex;
}

unsigned int Build_TFURAREQINFO_Parameter(unsigned char* pucTxBuffer,
                                          STfuRaReqInfo* psTfuRaReqInfo)
{
    unsigned int uiIndex = 0; 
    unsigned char ucValue = 0;

    uiIndex = BuildParameterHeader(TFURAREQINFO, pucTxBuffer);

    *(pucTxBuffer + uiIndex) = 0;
    ucValue = psTfuRaReqInfo->m_ucPreambleId;
    *(pucTxBuffer + uiIndex ) = *(pucTxBuffer + uiIndex) | ucValue;

    uiIndex++;

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, psTfuRaReqInfo->m_usTimingAdvance);

    AdjustParameterHeaderLength(uiIndex,  pucTxBuffer); 

    return uiIndex;
}

unsigned int Build_TFURECPREQINFO_Parameter(unsigned char*pucTxBuffer,  
                                            STfuRecpReqInfo* psTfuRecpReqInfo)
{
    unsigned int uiIndex = 0, i = 0;
    unsigned short usNumberOfUE = 0;
    usNumberOfUE = psTfuRecpReqInfo->m_usNumberOfUE;

    uiIndex = BuildParameterHeader(TFURECPREQINFO, pucTxBuffer);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, psTfuRecpReqInfo->m_usCellId);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, usNumberOfUE);

    for(i = 0; i < usNumberOfUE; i++)
    { 
        uiIndex += Build_TFUUERECPREQINFO_Parameter(pucTxBuffer + uiIndex, 
            psTfuRecpReqInfo->m_ppsTfuUeRecpReqInfo[i]);
    }


    AdjustParameterHeaderLength(uiIndex,  pucTxBuffer); 

    return uiIndex;
}

unsigned int Build_TFUUERECPREQINFO_Parameter(unsigned char* pucTxBuffer, 
                                              STfuUeRecpReqInfo* psTfuUeRecpReqInfo)
{
    unsigned int uiIndex = 0;

    uiIndex = BuildParameterHeader(TFUUERECPREQINFO, pucTxBuffer);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, psTfuUeRecpReqInfo->m_usRNTI);

    AdjustParameterHeaderLength(uiIndex,  pucTxBuffer); 

    return uiIndex;
}

unsigned int Build_TFUDATREQINFO_Parameter(unsigned char*pucTxBuffer,  
                                           STfuDatReqInfo* psTfuDatReqInfo)
{
    unsigned int uiIndex = 0, i = 0;
    unsigned short usLengthOfBCHBuffer = 0;
    unsigned short usNumberOfTFUDATREQPDUINFO = 0;
    
    usLengthOfBCHBuffer = psTfuDatReqInfo->m_usLengthOfBCHBuffer;
    usNumberOfTFUDATREQPDUINFO = psTfuDatReqInfo->m_usNumberOfTFUDATREQPDUINFO;


    uiIndex = BuildParameterHeader(TFUDATREQINFO, pucTxBuffer);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, psTfuDatReqInfo->m_usCellId);

    uiIndex += Build_CMLTETIMINGINFO_Parameter(pucTxBuffer + uiIndex, 
        &(psTfuDatReqInfo->m_sTimingInfo));

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, usLengthOfBCHBuffer);

    UTILMemcpy(pucTxBuffer + uiIndex, psTfuDatReqInfo->m_pucBCHBuffer, usLengthOfBCHBuffer);
    uiIndex += usLengthOfBCHBuffer;

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex,usNumberOfTFUDATREQPDUINFO);

    for(i = 0; i < usNumberOfTFUDATREQPDUINFO; i++)
    {
        uiIndex += Build_TFUDATREQPDUINFO_Parameter(pucTxBuffer + uiIndex,
            psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[i]);
            
    }

    AdjustParameterHeaderLength(uiIndex,  pucTxBuffer); 

    return uiIndex ;
}

unsigned int Build_TFUDATREQPDUINFO_Parameter(unsigned char* pucTxBuffer,
                                              STfuDatReqPduInfo* psTfuDatReqPduInfo) 
{
    unsigned int uiIndex = 0, i = 0;
    unsigned char ucValue = 0;

    uiIndex = BuildParameterHeader(TFUDATREQPDUINFO, pucTxBuffer);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, psTfuDatReqPduInfo->m_usRNTI);

    uiIndex += Build_TFUPDSCHDCIINFO_Parameter(pucTxBuffer + uiIndex,
        &(psTfuDatReqPduInfo->m_sTfuPdschDciInfo));

    *(pucTxBuffer + uiIndex ) = 0;
    ucValue = psTfuDatReqPduInfo->m_ucNumberOfTB;
    *(pucTxBuffer + uiIndex ) = *(pucTxBuffer + uiIndex )|ucValue;
    uiIndex ++;

    for(i = 0; i < ucValue; i++)
    {
        uiIndex += GetBufferFromInt(pucTxBuffer + uiIndex, psTfuDatReqPduInfo->m_puiMacPdubufferLen[i]);
    }

    for( i = 0; i < ucValue; i++)
    {
        UTILMemcpy(pucTxBuffer + uiIndex, psTfuDatReqPduInfo->m_ppucMacPduBuffer[i], 
            psTfuDatReqPduInfo->m_puiMacPdubufferLen[i]);
        uiIndex += psTfuDatReqPduInfo->m_puiMacPdubufferLen[i];
    }

    AdjustParameterHeaderLength(uiIndex,  pucTxBuffer); 

    return uiIndex;
}


unsigned int Build_TFUPDSCHDCIINFO_Parameter(unsigned char* pucTxBuffer,
                                             STfuPdschDciInfo* psTfuPdschDciInfo)
{
    unsigned int uiIndex = 0;
    uiIndex = BuildParameterHeader(TFUPDSCHDCIINFO, pucTxBuffer);

    AdjustParameterHeaderLength(uiIndex,  pucTxBuffer);
    return uiIndex;

}


unsigned int Build_TFUDATINDINFO_Parameter(unsigned char*pucTxBuffer,
                                           STfuDatIndInfo* psTfuDatIndInfo)
{
    unsigned int uiIndex = 0, i ;
    unsigned short usNumberOfTFUDATINFO = 0;
    usNumberOfTFUDATINFO = psTfuDatIndInfo->m_usNumberOfTFUDATINFO;

    uiIndex = BuildParameterHeader(TFUDATINDINFO, pucTxBuffer);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, psTfuDatIndInfo->m_usCellId);

    uiIndex += Build_CMLTETIMINGINFO_Parameter(pucTxBuffer + uiIndex,
        &(psTfuDatIndInfo->m_sTimingInfo));

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, usNumberOfTFUDATINFO);

    for(i = 0; i < usNumberOfTFUDATINFO; i++)
    {
        uiIndex += Build_TFUDATINFO_Parameter(pucTxBuffer + uiIndex,
            psTfuDatIndInfo->m_ppsTfuDatInfo[i]); 
    }

    AdjustParameterHeaderLength(uiIndex,  pucTxBuffer); 

    return uiIndex;

}

unsigned int  Build_TFUDATINFO_Parameter(unsigned char* pucTxBuffer,
                                         STfuDatInfo* psTfuDatInfo)
{
    unsigned int uiIndex = 0;
    unsigned short usDATA_PDU_LENGTH = 0;
    usDATA_PDU_LENGTH  = psTfuDatInfo->m_uiDataPduLength;

    uiIndex =  BuildParameterHeader(TFUDATINFO, pucTxBuffer);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, psTfuDatInfo->m_usRNTI);

    uiIndex += GetBufferFromInt(pucTxBuffer + uiIndex, usDATA_PDU_LENGTH);

    UTILMemcpy(pucTxBuffer + uiIndex, psTfuDatInfo->m_pucDataPduBuffer, usDATA_PDU_LENGTH );

    uiIndex += usDATA_PDU_LENGTH;

    AdjustParameterHeaderLength(uiIndex,  pucTxBuffer); 

    return uiIndex;
}

unsigned int Build_TFUCRCINDINFO_Parameter(unsigned char* pucTxBuffer,
                                           STfuCrcIndInfo* psTfuCrcIndInfo)
{
    unsigned int uiIndex = 0, i = 0 ;
    unsigned short usNumberOfTFUCRCINFO = 0;
    usNumberOfTFUCRCINFO = psTfuCrcIndInfo->m_usNumberOfTFUCRCINFO;

    uiIndex =  BuildParameterHeader(TFUCRCINDINFO, pucTxBuffer);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, psTfuCrcIndInfo->m_usCellId);

    uiIndex += Build_CMLTETIMINGINFO_Parameter(pucTxBuffer + uiIndex, 
        &(psTfuCrcIndInfo->m_sTimingInfo));   

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, usNumberOfTFUCRCINFO);

    for(i = 0; i < usNumberOfTFUCRCINFO; i++)
    {
        uiIndex += Build_TFUCRCINFO_Parameter(pucTxBuffer + uiIndex, 
            psTfuCrcIndInfo->m_ppsTfuCrcInfo[i]);
    }

    AdjustParameterHeaderLength(uiIndex, pucTxBuffer); 

    return uiIndex;
}

unsigned int Build_TFUCRCINFO_Parameter(unsigned char* pucTxBuffer, 
                                        STfuCrcInfo* psTfuCrcInfo) 
{
    unsigned int uiIndex = 0;
    unsigned char ucValue = 0;

    uiIndex =  BuildParameterHeader(TFUCRCINFO, pucTxBuffer);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, psTfuCrcInfo->m_usRNTI);

    *(pucTxBuffer + uiIndex ) = 0;

    ucValue = psTfuCrcInfo->m_ucISFAILURE;
    *(pucTxBuffer + uiIndex ) = *(pucTxBuffer + uiIndex )|ucValue;
    uiIndex ++;

    *(pucTxBuffer + uiIndex ) = 0;
    ucValue = psTfuCrcInfo->m_ucRV;
    *(pucTxBuffer + uiIndex ) = *(pucTxBuffer + uiIndex )|ucValue;
    uiIndex ++;

    AdjustParameterHeaderLength(uiIndex, pucTxBuffer);

    return uiIndex;
}

unsigned int Build_TFUULCQIINDINFO_Parameter(unsigned char* pucTxBuffer,
                                             STfuULCqiIndInfo* psTfuULCqiIndInfo)
{
    unsigned int uiIndex = 0, i = 0;
    unsigned short usNumberOFTFUULCQIRPT = 0;
    uiIndex =  BuildParameterHeader(TFUULCQIINDINFO, pucTxBuffer);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, psTfuULCqiIndInfo->m_usCellId);

    uiIndex += Build_CMLTETIMINGINFO_Parameter(pucTxBuffer + uiIndex,
        &(psTfuULCqiIndInfo->m_sTimingInfo));

    usNumberOFTFUULCQIRPT = psTfuULCqiIndInfo->m_usNumberOFTFUULCQIRPT;

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, usNumberOFTFUULCQIRPT);

    for( i = 0; i < usNumberOFTFUULCQIRPT; i++)
    {
        uiIndex += Build_TFUULCQIRPT_Parameter(pucTxBuffer + uiIndex,
            psTfuULCqiIndInfo->m_ppsTfuULCqiRpt[i]);
    }
    return uiIndex;

}
unsigned int Build_TFUULCQIRPT_Parameter(unsigned char* pucTxBuffer,
                                         STfuULCqiRpt* psTfuULCqiRpt)
{
    unsigned int uiIndex = 0, i = 0;
    unsigned char ucValue = 0;
    unsigned char ucNumberOfSubBands = 0;
    ucNumberOfSubBands = psTfuULCqiRpt->m_ucNumberOfSubBands;
    
    uiIndex += BuildParameterHeader(TFUULCQIRPT, pucTxBuffer);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, 
        psTfuULCqiRpt->m_usRNTI);

    *(pucTxBuffer + uiIndex ) = 0;
    ucValue = psTfuULCqiRpt->m_ucISTXPORT0;
    *(pucTxBuffer + uiIndex ) = *(pucTxBuffer + uiIndex )|ucValue;
    uiIndex ++;
    
    *(pucTxBuffer + uiIndex ) = 0;
    ucValue = psTfuULCqiRpt->m_ucWideCqi;
    *(pucTxBuffer + uiIndex ) = *(pucTxBuffer + uiIndex )|ucValue;
    uiIndex ++;

    *(pucTxBuffer + uiIndex ) = 0;
    ucValue = ucNumberOfSubBands;
    *(pucTxBuffer + uiIndex ) = *(pucTxBuffer + uiIndex )|ucValue;
    uiIndex ++;
    
    for(i = 0; i < TFU_MAX_UL_SUBBAND; i++)
    {
        uiIndex += Build_TFUULCQIINFO_Parameter(pucTxBuffer + uiIndex,
             psTfuULCqiRpt->m_ppsTfuULCqiInfo[i]);
    }

    return uiIndex;
}

unsigned int Build_TFUULCQIINFO_Parameter(unsigned char* pucTxBuffer,
                                          STfuULCqiInfo* psTfuULCqiInfo)
{
    unsigned int uiIndex = 0;
    uiIndex += BuildParameterHeader(TFUULCQIINFO, pucTxBuffer);

    uiIndex += Build_TFUSUBBANDCQIINFO_Parameter(pucTxBuffer + uiIndex,
        &(psTfuULCqiInfo->m_sTfuSubBandCqiInfo));

    return uiIndex;
}

unsigned int Build_TFUSUBBANDCQIINFO_Parameter(unsigned char* pucTxBuffer,
                                               STfuSubBandCqiInfo* psTfuSubBandCqiInfo)
{
    unsigned int uiIndex = 0;
    unsigned char ucValue = 0;
    uiIndex += BuildParameterHeader(TFUSUBBANDCQIINFO, pucTxBuffer);

    uiIndex += Build_TFUSUBBANDINFO_Parameter(pucTxBuffer + uiIndex,
        &(psTfuSubBandCqiInfo->m_sTfuSubBandInfo));

    *(pucTxBuffer + uiIndex ) = 0;
    ucValue = psTfuSubBandCqiInfo->m_ucCQIIndex;
    *(pucTxBuffer + uiIndex ) = *(pucTxBuffer + uiIndex )|ucValue;
    uiIndex ++;
    
    return uiIndex;
}
unsigned int Build_TFUSUBBANDINFO_Parameter(unsigned char* pucTxBuffer,
                                            STfuSubBandInfo* psTfuSubBandInfo)
{
    unsigned int uiIndex = 0;
    unsigned char ucValue = 0;
    
    uiIndex += BuildParameterHeader(TFUSUBBANDINFO, pucTxBuffer);
    
    *(pucTxBuffer + uiIndex ) = 0;
    ucValue = psTfuSubBandInfo->m_ucRBSTART;
    *(pucTxBuffer + uiIndex ) = *(pucTxBuffer + uiIndex )|ucValue;
    uiIndex ++;

    *(pucTxBuffer + uiIndex ) = 0;
    ucValue = psTfuSubBandInfo->m_ucNumberOfRB;
    *(pucTxBuffer + uiIndex ) = *(pucTxBuffer + uiIndex )|ucValue;
    uiIndex ++;
    
    return uiIndex;
}
unsigned int Build_CMLTETIMINGINFO_Parameter(unsigned char* pucTxBuffer , 
                                             STimingInfo* psTimingInfo)
{
    unsigned int uiIndex = 0;
    unsigned char ucValue = 0;

    uiIndex =   GetBufferFromShort(pucTxBuffer, psTimingInfo->m_usSFN);

    *(pucTxBuffer + uiIndex) = 0;
    ucValue = psTimingInfo->m_ucSubframe;

    *(pucTxBuffer + uiIndex) = *(pucTxBuffer + uiIndex) | ucValue;

    uiIndex ++;

    return uiIndex;
}

unsigned int Build_TFUSRINDINFO_Parameter(unsigned char* pucTxBuffer,
                                           STfuSrIndInfo* psTfuSrIndInfo)
{
    unsigned int uiIndex = 0, i = 0 ;
    unsigned short usNumberOfTFUSRINFO = 0;
    usNumberOfTFUSRINFO = psTfuSrIndInfo->m_usNumberOfTFUSRINFO;

    uiIndex =  BuildParameterHeader(TFUSRINDINFO, pucTxBuffer);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, psTfuSrIndInfo->m_usCellId);

    uiIndex += Build_CMLTETIMINGINFO_Parameter(pucTxBuffer + uiIndex, 
        &(psTfuSrIndInfo->m_sTimingInfo));

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, usNumberOfTFUSRINFO);

    for(i = 0; i < usNumberOfTFUSRINFO; i++)
    {
        uiIndex += Build_TFUSRINFO_Parameter(pucTxBuffer + uiIndex, 
            psTfuSrIndInfo->m_ppsTfuSrInfo[i]);
    }

    AdjustParameterHeaderLength(uiIndex, pucTxBuffer);

    return uiIndex;
}

unsigned int Build_TFUSRINFO_Parameter(unsigned char* pucTxBuffer, 
                                        STfuSrInfo* psTfuSrInfo)
{
    unsigned int uiIndex = 0;

    uiIndex =  BuildParameterHeader(TFUSRINFO, pucTxBuffer);

    uiIndex += GetBufferFromShort(pucTxBuffer + uiIndex, psTfuSrInfo->m_usRNTI);

    AdjustParameterHeaderLength(uiIndex, pucTxBuffer);

    return uiIndex;
}
