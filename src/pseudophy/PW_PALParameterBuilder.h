/*****************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#pragma once

unsigned int BuildParameterHeader(unsigned short usType, unsigned char* pucTxBuffer );

void   AdjustParameterHeaderLength(unsigned int uiLength, unsigned char* pucTxBuffer);

unsigned int BuildParameter(unsigned short usType, unsigned char* pucTxBuffer);

unsigned int Build_TFUCNTRLREQINFO_Parameter(unsigned char* pucTxBuffer,
                                             STfuCntrlReqInfo* psTfuCntrlReqInfo);
unsigned int Build_TFUPDCCHINFO_Parameter(unsigned char* pucTxBuffer,  
                                          STfuPdcchInfo* psTfuPdcchInfo);
unsigned int Build_TFUPHICHINFO_Parameter(unsigned char* pucTxBuffer, 
                                          STfuPhiChInfo* psTfuPhiChInfo);
unsigned int Build_TFURAREQINDINFO_Parameter(unsigned char* pucTxBuffer,
                                             STfuRaReqIndInfo* psTfuRaReqIndInfo);
unsigned int Build_TFURACHINFO_Parameter(unsigned char* pucTxBuffer, 
                                         STfuRachInfo* psTfuRachInfo);
unsigned int Build_TFURAREQINFO_Parameter(unsigned char* pucTxBuffer,
                                          STfuRaReqInfo* psTfuRaReqInfo);
unsigned int Build_TFURECPREQINFO_Parameter(unsigned char*pucTxBuffer,  
                                            STfuRecpReqInfo* psTfuRecpReqInfo);
unsigned int Build_TFUUERECPREQINFO_Parameter(unsigned char* pucTxBuffer, 
                                              STfuUeRecpReqInfo* psTfuUeRecpReqInfo);
unsigned int Build_TFUDATREQINFO_Parameter(unsigned char*pucTxBuffer,  
                                           STfuDatReqInfo* psTfuDatReqInfo);
unsigned int Build_TFUDATREQPDUINFO_Parameter(unsigned char* pucTxBuffer,
                                              STfuDatReqPduInfo* psTfuDatReqPduInfo) ;
unsigned int Build_TFUPDSCHDCIINFO_Parameter(unsigned char* pucTxBuffer,
                                             STfuPdschDciInfo* psTfuPdschDciInfo);
unsigned int Build_TFUDATINDINFO_Parameter(unsigned char*pucTxBuffer,
                                           STfuDatIndInfo* psTfuDatIndInfo);
unsigned int  Build_TFUDATINFO_Parameter(unsigned char* pucTxBuffer,
                                         STfuDatInfo* psTfuDatInfo);
unsigned int Build_TFUCRCINDINFO_Parameter(unsigned char* pucTxBuffer,
                                           STfuCrcIndInfo* psTfuCrcIndInfo);
unsigned int Build_TFUCRCINFO_Parameter(unsigned char* pucTxBuffer, 
                                        STfuCrcInfo* psTfuCrcInfo);
unsigned int Build_TFUULCQIINDINFO_Parameter(unsigned char* pucTxBuffer,
                                             STfuULCqiIndInfo* psTfuULCqiIndInfo);
unsigned int Build_TFUULCQIRPT_Parameter(unsigned char* pucTxBuffer,
                                         STfuULCqiRpt* psTfuULCqiRpt);
unsigned int Build_TFUULCQIINFO_Parameter(unsigned char* pucTxBuffer,
                                          STfuULCqiInfo* psTfuULCqiInfo);
unsigned int Build_TFUSUBBANDCQIINFO_Parameter(unsigned char* pucTxBuffer,
                                               STfuSubBandCqiInfo* psTfuSubBandCqiInfo);
unsigned int Build_TFUSUBBANDINFO_Parameter(unsigned char* pucTxBuffer,
                                            STfuSubBandInfo* psTfuSubBandInfo);
unsigned int Build_CMLTETIMINGINFO_Parameter(unsigned char* pucTxBuffer , 
                                             STimingInfo* psTimingInfo);
unsigned int Build_TFUSRINDINFO_Parameter(unsigned char* pucTxBuffer,
                                           STfuSrIndInfo* psTfuSrIndInfo);
unsigned int Build_TFUSRINFO_Parameter(unsigned char* pucTxBuffer, 
                                        STfuSrInfo* psTfuSrInfo);
