/*****************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#include "PW_PALCommon.h"
#include "MemoryDebug.h"

unsigned int GetBufferFromShort(unsigned char* pucBuffer, unsigned short usVal)
{
    unsigned int uiIndex = 0;
    unsigned short usValue;
    unsigned char ucValue;

    pucBuffer[uiIndex] = 0;
    usValue = usVal;
    usValue = usValue >> 8;
    ucValue = (unsigned char)usValue;
    pucBuffer[uiIndex] = pucBuffer[uiIndex] | ucValue;
    uiIndex++;

    pucBuffer[uiIndex] = 0;
    usValue = usVal;
    usValue = usValue << 8;
    usValue = usValue >> 8;
    ucValue = (unsigned char)usValue;
    pucBuffer[uiIndex] = pucBuffer[uiIndex] | ucValue;
    uiIndex++;

    return uiIndex;
}

unsigned int GetBufferFromInt(unsigned char* pucBuffer, unsigned int uiVal)
{
    unsigned int uiIndex = 0;
    unsigned int ui_value;
    unsigned char ucValue;

    pucBuffer[uiIndex] = 0;
    ui_value = uiVal;
    ui_value = ui_value >> 24;
    ucValue = (unsigned char)ui_value;
    pucBuffer[uiIndex] = pucBuffer[uiIndex] | ucValue;
    uiIndex++;

    pucBuffer[uiIndex] = 0;
    ui_value = uiVal;
    ui_value = ui_value << 8;
    ui_value = ui_value >> 24;
    ucValue = (unsigned char)ui_value;
    pucBuffer[uiIndex] = pucBuffer[uiIndex] | ucValue;
    uiIndex++;

    pucBuffer[uiIndex] = 0;
    ui_value = uiVal;
    ui_value = ui_value << 16;
    ui_value = ui_value >> 24;
    ucValue = (unsigned char)ui_value;
    pucBuffer[uiIndex] = pucBuffer[uiIndex] | ucValue;
    uiIndex++;

    pucBuffer[uiIndex] = 0;
    ui_value = uiVal;
    ui_value = ui_value << 24;
    ui_value = ui_value >> 24;
    ucValue = (unsigned char)ui_value;
    pucBuffer[uiIndex] = pucBuffer[uiIndex] | ucValue;
    uiIndex++;

    return uiIndex;
}

/*****************  Parsing Functions   *****************/


unsigned short GetShortFromBuffer(unsigned char* pucBuffer)
{
    unsigned short Value;

    Value = (*(pucBuffer));
    Value <<= 8u;
    Value |= (*(pucBuffer+1));

    return Value;
}

unsigned int GetIntFromBuffer(unsigned char* pucBuffer)
{
    unsigned int Value;

    Value = (*(pucBuffer));
    Value <<= 8u;
    Value |=(*(pucBuffer+1));
    Value <<= 8u;
    Value |= (*(pucBuffer+2));
    Value <<= 8u;
    Value |= (*(pucBuffer+3));

    return Value;
}

unsigned long long GetLongLongFromBuffer(unsigned char* pucBuffer)
{
    unsigned int Value;

    Value = (*(pucBuffer));
    Value <<= 8u;
    Value |=(*(pucBuffer+1));
    Value <<= 8u;
    Value |= (*(pucBuffer+2));
    Value <<= 8u;
    Value |= (*(pucBuffer+3));
    Value <<= 8u;
    Value |= (*(pucBuffer+4));
    Value <<= 8u;
    Value |= (*(pucBuffer+5));
    Value <<= 8u;
    Value |= (*(pucBuffer+6));
    Value <<= 8u;
    Value |= (*(pucBuffer+7));

    return Value;
}


/******** Freeing Memory Of PALFormats Structures ************/

void FreeSDLControlRequest(SDLControlRequest* psDLControlRequest)
{
    if(psDLControlRequest != NULL)
        FreeSTfuCntrlReqInfo(&(psDLControlRequest->m_sTfuCntrlReqInfo));
}
void FreeSRandomAccessIndication(SRandomAccessIndication* 
                                 psRandomAccessIndication)
{
    if(psRandomAccessIndication != NULL)
        FreeSTfuRaReqIndInfo(&(psRandomAccessIndication->m_sTfuRaReqIndInfo));
}
void FreeSULReceptionRequest(SULReceptionRequest* psULReceptionRequest)
{
    if(psULReceptionRequest != NULL)
        FreeSTfuRecpReqInfo(&(psULReceptionRequest->m_sTfuRecpReqInfo));
}

void FreeSDLDataRequest(SDLDataRequest* psDLDataRequest)
{
    if(psDLDataRequest != NULL)
        FreeSTfuDatReqInfo(&(psDLDataRequest->m_sTfuDatReqInfo));
}

void FreeSULDataIndication(SULDataIndication* psULDataIndication)
{
    if(psULDataIndication != NULL)
        FreeSTfuDatIndInfo(&(psULDataIndication->m_sTfuDatIndInfo));
}
void FreeSULCRCIndication(SULCRCIndication* psULCRCIndication)
{
    if(psULCRCIndication != NULL)
        FreeSTfuCrcIndInfo(&(psULCRCIndication->m_STfuCrcIndInfo));
}
void FreeSULCQIIndication(SULCQIIndication* psULCQIIndication)
{
    if(psULCQIIndication != NULL)
        FreeSTfuULCqiIndInfo(&(psULCQIIndication->m_sTfuULCqiIndInfo));
}

void FreeSTfuCntrlReqInfo(STfuCntrlReqInfo* psTfuCntrlReqInfo)
{
    unsigned int i = 0;
    if(psTfuCntrlReqInfo !=NULL)
    {
        if(psTfuCntrlReqInfo->m_ppsULTFUPDCCHInfo != NULL)
        {
            for( i = 0; i < psTfuCntrlReqInfo->m_usNumberOfULPDCCHInfo; i++)
            {  
                if(psTfuCntrlReqInfo->m_ppsULTFUPDCCHInfo[i] != NULL)
                {
                    UTILFree(psTfuCntrlReqInfo->m_ppsULTFUPDCCHInfo[i]);
                    psTfuCntrlReqInfo->m_ppsULTFUPDCCHInfo[i] = NULL;
                }
            }
            UTILFree(psTfuCntrlReqInfo->m_ppsULTFUPDCCHInfo);
            psTfuCntrlReqInfo->m_ppsULTFUPDCCHInfo = NULL;
        }

        if(psTfuCntrlReqInfo->m_ppsDLTFUPDCCHInfo != NULL)
        {
            for( i = 0; i < psTfuCntrlReqInfo->m_usNumberOfDLPDCCHInfo; i++)
            {
                if(psTfuCntrlReqInfo->m_ppsDLTFUPDCCHInfo[i] != NULL)
                {
                    UTILFree(psTfuCntrlReqInfo->m_ppsDLTFUPDCCHInfo[i]);
                    psTfuCntrlReqInfo->m_ppsDLTFUPDCCHInfo[i] = NULL;
                }
            }
            UTILFree(psTfuCntrlReqInfo->m_ppsDLTFUPDCCHInfo);
            psTfuCntrlReqInfo->m_ppsDLTFUPDCCHInfo = NULL;
        }

        if(psTfuCntrlReqInfo->m_ppsTfuPHICHInfo != NULL)
        { 
            for( i = 0; i < psTfuCntrlReqInfo->m_usNumberOfPHICHINFO; i++)
            { 
                if(psTfuCntrlReqInfo->m_ppsTfuPHICHInfo[i]!=NULL)
                {
                    UTILFree(psTfuCntrlReqInfo->m_ppsTfuPHICHInfo[i]);
                    psTfuCntrlReqInfo->m_ppsTfuPHICHInfo[i] = NULL;
                }
            }
            UTILFree(psTfuCntrlReqInfo->m_ppsDLTFUPDCCHInfo);
            psTfuCntrlReqInfo->m_ppsDLTFUPDCCHInfo = NULL;
        }    


    }
}

void FreeSTfuRaReqIndInfo(STfuRaReqIndInfo* psTfuRaReqIndInfo)
{
    unsigned int i = 0, j = 0;
    if(psTfuRaReqIndInfo != NULL)
    {
        if(psTfuRaReqIndInfo->m_ppsTfuRachInfo != NULL)
        {
            for(i = 0; i < psTfuRaReqIndInfo->m_usNumberOfTFURACHINFO; i++)
            {
                if(psTfuRaReqIndInfo->m_ppsTfuRachInfo[i] != NULL)
                {
                    if(psTfuRaReqIndInfo->m_ppsTfuRachInfo[i]->m_ppsTfuRaReqInfo !=NULL)
                    {
                        for( j = 0; j < psTfuRaReqIndInfo->m_ppsTfuRachInfo[i]->m_usNumberOfTFURAREQINFO; j++) 
                        {
                            if(psTfuRaReqIndInfo->m_ppsTfuRachInfo[i]->m_ppsTfuRaReqInfo[j] != NULL)
                            {
                                UTILFree(psTfuRaReqIndInfo->m_ppsTfuRachInfo[i]->m_ppsTfuRaReqInfo[j]);
                                psTfuRaReqIndInfo->m_ppsTfuRachInfo[i]->m_ppsTfuRaReqInfo[j] = NULL;
                            }
                        }
                        UTILFree(psTfuRaReqIndInfo->m_ppsTfuRachInfo[i]->m_ppsTfuRaReqInfo);
                        psTfuRaReqIndInfo->m_ppsTfuRachInfo[i]->m_ppsTfuRaReqInfo = NULL;
                    }
                    UTILFree(psTfuRaReqIndInfo->m_ppsTfuRachInfo[i]);
                    psTfuRaReqIndInfo->m_ppsTfuRachInfo[i] = NULL;
                }
            }
            UTILFree(psTfuRaReqIndInfo->m_ppsTfuRachInfo);
            psTfuRaReqIndInfo->m_ppsTfuRachInfo = NULL;
        }        
    }
}


void FreeSTfuRecpReqInfo(STfuRecpReqInfo* psTfuRecpReqInfo)
{
    unsigned int i = 0; 
    if(psTfuRecpReqInfo !=NULL)
    {   
        if(psTfuRecpReqInfo->m_ppsTfuUeRecpReqInfo != NULL)
        {
            for( i = 0; i < psTfuRecpReqInfo->m_usNumberOfUE; i++)
            {
                if(psTfuRecpReqInfo->m_ppsTfuUeRecpReqInfo[i] !=NULL)
                {
                    UTILFree(psTfuRecpReqInfo->m_ppsTfuUeRecpReqInfo[i]);
                    psTfuRecpReqInfo->m_ppsTfuUeRecpReqInfo[i] = NULL;
                }
            }
            UTILFree(psTfuRecpReqInfo->m_ppsTfuUeRecpReqInfo);
            psTfuRecpReqInfo->m_ppsTfuUeRecpReqInfo = NULL;
        }

    }
}
void FreeSTfuDatReqInfo(STfuDatReqInfo* psTfuDatReqInfo)
{
    unsigned int  i = 0, j = 0;
    if(psTfuDatReqInfo != NULL)
    {
        if(psTfuDatReqInfo->m_pucBCHBuffer != NULL)
        {
            UTILFree(psTfuDatReqInfo->m_pucBCHBuffer);
            psTfuDatReqInfo->m_pucBCHBuffer = NULL;
        }

        if(psTfuDatReqInfo->m_ppsTfuDatReqPduInfo != NULL)
        {
            for(i = 0; i < psTfuDatReqInfo->m_usNumberOfTFUDATREQPDUINFO; i++)
            {
                if(psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[i] != NULL)
                {
                    if(psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[i]->m_puiMacPdubufferLen != NULL)
                    {
                        UTILFree(psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[i]->m_puiMacPdubufferLen);
                        psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[i]->m_puiMacPdubufferLen = NULL;

                    }
                    if(psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[i]->m_ppucMacPduBuffer != NULL)
                    {
                        for( j = 0; j < psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[i]->m_ucNumberOfTB; j++)
                        {
                            if(psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[i]->m_ppucMacPduBuffer[j] != NULL)
                            {
                                UTILFree(psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[i]->m_ppucMacPduBuffer[j]);
                                psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[i]->m_ppucMacPduBuffer[j] = NULL;
                            }
                        }
                        UTILFree(psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[i]->m_ppucMacPduBuffer);
                        psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[i]->m_ppucMacPduBuffer = NULL;
                    }
                    UTILFree(psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[i]);
                    psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[i] = NULL;
                }

            }
            UTILFree(psTfuDatReqInfo->m_ppsTfuDatReqPduInfo);
            psTfuDatReqInfo->m_ppsTfuDatReqPduInfo = NULL;
        }
    }
}
void FreeSTfuDatIndInfo(STfuDatIndInfo* psTfuDatIndInfo)
{
    unsigned int i = 0;
    if(psTfuDatIndInfo != NULL)
    {
        if(psTfuDatIndInfo->m_ppsTfuDatInfo != NULL)
        {
            for(i = 0; i < psTfuDatIndInfo->m_usNumberOfTFUDATINFO; i++)
            {
                if(psTfuDatIndInfo->m_ppsTfuDatInfo[i] != NULL)
                {
                    if(psTfuDatIndInfo->m_ppsTfuDatInfo[i]->m_pucDataPduBuffer != NULL)
                    {
                        UTILFree(psTfuDatIndInfo->m_ppsTfuDatInfo[i]->m_pucDataPduBuffer);
                        psTfuDatIndInfo->m_ppsTfuDatInfo[i]->m_pucDataPduBuffer = NULL;
                    }

                    UTILFree(psTfuDatIndInfo->m_ppsTfuDatInfo[i]);
                    psTfuDatIndInfo->m_ppsTfuDatInfo[i] = NULL;
                }
            }
            UTILFree(psTfuDatIndInfo->m_ppsTfuDatInfo);
            psTfuDatIndInfo->m_ppsTfuDatInfo = NULL;
        }

    }
}
void FreeSTfuCrcIndInfo(STfuCrcIndInfo* psTfuCrcIndInfo)
{
    unsigned int i = 0;
    if(psTfuCrcIndInfo != NULL)
    {
        if(psTfuCrcIndInfo->m_ppsTfuCrcInfo != NULL)
        {
            for( i = 0; i < psTfuCrcIndInfo->m_usNumberOfTFUCRCINFO; i++)
            {
                if(psTfuCrcIndInfo->m_ppsTfuCrcInfo[i]!= NULL)
                {
                    UTILFree(psTfuCrcIndInfo->m_ppsTfuCrcInfo[i]);
                    psTfuCrcIndInfo->m_ppsTfuCrcInfo[i] = NULL;
                }
            }
            UTILFree(psTfuCrcIndInfo->m_ppsTfuCrcInfo);
            psTfuCrcIndInfo->m_ppsTfuCrcInfo = NULL;
        }   
    }
}

void FreeSTfuULCqiIndInfo(STfuULCqiIndInfo* psTfuULCqiIndInfo)
{
    unsigned int i = 0, j = 0;
    if(psTfuULCqiIndInfo != NULL)
    {
        if(psTfuULCqiIndInfo->m_ppsTfuULCqiRpt != NULL)
        {
            for(i = 0; i < psTfuULCqiIndInfo->m_usNumberOFTFUULCQIRPT; i++)
            {
                if(psTfuULCqiIndInfo->m_ppsTfuULCqiRpt[i] != NULL)
                {
                    if(psTfuULCqiIndInfo->m_ppsTfuULCqiRpt[i]->m_ppsTfuULCqiInfo != NULL)
                    {
                        for(j = 0; j < psTfuULCqiIndInfo->m_ppsTfuULCqiRpt[i]->m_ucNumberOfSubBands; j++)
                        {  
                            if(psTfuULCqiIndInfo->m_ppsTfuULCqiRpt[i]->m_ppsTfuULCqiInfo[j] != NULL)
                            {
                                UTILFree(psTfuULCqiIndInfo->m_ppsTfuULCqiRpt[i]->m_ppsTfuULCqiInfo[j]);
                                psTfuULCqiIndInfo->m_ppsTfuULCqiRpt[i]->m_ppsTfuULCqiInfo[j] = NULL;
                            }
                        }
                        UTILFree(psTfuULCqiIndInfo->m_ppsTfuULCqiRpt[i]->m_ppsTfuULCqiInfo);
                        psTfuULCqiIndInfo->m_ppsTfuULCqiRpt[i]->m_ppsTfuULCqiInfo = NULL;
                    }
                    UTILFree(psTfuULCqiIndInfo->m_ppsTfuULCqiRpt[i]);
                    psTfuULCqiIndInfo->m_ppsTfuULCqiRpt[i] = NULL;
                }
            }
            UTILFree(psTfuULCqiIndInfo->m_ppsTfuULCqiRpt);
            psTfuULCqiIndInfo->m_ppsTfuULCqiRpt = NULL;
        }

    }

}

