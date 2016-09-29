/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#pragma once

#include "PW_PALFormats.h"

unsigned int BuildMessageHeader(unsigned short usType, unsigned char* pucTxBuffer);
void  AdjustMessageHeaderLength(unsigned int uiLength, unsigned char* pucTxBuffer);

unsigned int BuildDLControlRequest(unsigned char* pucTxBuffer,
                                   STfuCntrlReqInfo* psTfuCntrlReqInfo);
unsigned int BuildRandomAccessIndication(unsigned char* pucTxBuffer,
                                         STfuRaReqIndInfo* psTfuRaReqIndInfo);                                         
unsigned int BuildULReceptionRequest(unsigned char*pucTxBuffer,  
                                     STfuRecpReqInfo* psTfuRecpReqInfo);
unsigned int BuildDLDataRequest(unsigned char*pucTxBuffer,  
                                STfuDatReqInfo* psTfuDatReqInfo);
unsigned int BuildULDataIndication(unsigned char*pucTxBuffer,
                                   STfuDatIndInfo* psTfuDatIndInfo);
unsigned int BuildULCRCIndication(unsigned char* pucTxBuffer,
                                  STfuCrcIndInfo* psTfuCrcIndInfo);
unsigned int BuildULCQIIndication(unsigned char* pucTxBuffer,
                                  STfuULCqiIndInfo* psTfuULCqiIndInfo);
unsigned int BuildULSRIndication(unsigned char* pucTxBuffer,
                                  STfuSrIndInfo* psTfuSrIndInfo);
