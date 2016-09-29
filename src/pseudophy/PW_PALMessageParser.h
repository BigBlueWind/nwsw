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

unsigned int DecodeMassageHeader(unsigned char* pucBuffer,
                                 SMessageHeader* psMessageHeader);
unsigned int DecodeDLControlRequest(unsigned char* pucBuffer,
                                    SDLControlRequest* psDLControlRequest);
unsigned int DecodeRandomAccessIndication(unsigned char* pucBuffer, 
                                          SRandomAccessIndication* psRandomAccessIndication);
unsigned int DecodeULReceptionRequest(unsigned char* pucBuffer,
                                      SULReceptionRequest* psULReceptionRequest);
unsigned int DecodeDLDataRequest(unsigned char* pucBuffer, 
                                 SDLDataRequest* psDLDataRequest);
unsigned int DecodeULDataIndication(unsigned char* pucBuffer,
                                    SULDataIndication* psULDataIndication);
unsigned int DecodeULCRCIndication(unsigned char* pucBuffer,
                                   SULCRCIndication* psULCRCIndication);
unsigned int DecodeULCQIIndication(unsigned char* pucBuffer,
                                   SULCQIIndication* psULCQIIndication);
unsigned int DecodeULSRIndication(unsigned char* pucBuffer,
                                   SULSRIndication* psULSRIndication);
unsigned short GetMessageType(unsigned char* pucBuffer);

