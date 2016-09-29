/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#pragma once

#pragma pack(push, 1)

#define TFU_MAX_TB              6
#define TFU_MAX_UL_SUBBAND      10

/*******  Message Types   *************/

#define SCHDCNTRLREQ                1
#define RANDACSIND                  2
#define ULRECEPREQ                  3
#define DLDATAREQ                   4
#define ULDATAIND                   5
#define ULCRCIND                    6
#define ULCQIIND                    7
#define ULSRIND                     8

/******* Parameter Types  *************/

#define TFUCNTRLREQINFO     512
#define TFUPDCCHINFO        513
#define TFUPHICHINFO        514
#define TFURAREQINDINFO     515
#define TFURACHINFO         516
#define TFURAREQINFO        517
#define TFURECPREQINFO      518
#define TFUUERECPREQINFO    519
#define TFUDATREQINFO       523
#define TFUDATREQPDUINFO    524
#define TFUPDSCHDCIINFO     525
#define TFUDATINDINFO       526
#define TFUDATINFO          527
#define TFUCRCINDINFO       528
#define TFUCRCINFO          529
#define TFUULCQIINDINFO     530
#define TFUULCQIRPT         531
#define TFUTTIINDINFO       532
#define TFUULCQIINFO        533
#define TFUSUBBANDCQIINFO   534
#define TFUSUBBANDINFO      535
#define TFUSRINDINFO        536
#define TFUSRINFO           537

/*********** Parameters Related Structure  ***************/

typedef unsigned char TBuffer[100];

typedef struct SParameterHeader
{
    unsigned char       m_ucReserved;
    unsigned short      m_usType;
    unsigned int        m_uiLength;
}SParameterHeader;

typedef struct STimingInfo
{
    unsigned short     m_usSFN;
    unsigned char      m_ucSubframe;
}STimingInfo;

/***********  Control Request Information Parameter ********/

typedef struct STfuPhiChInfo
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned char               m_ucRBSTART;
    unsigned char               m_ucNDMRS;
    unsigned char               m_ucISACK;

}STfuPhiChInfo;

typedef struct STfuPdcchInfo
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned short              m_usRNTI;
    unsigned short              m_usCCE;
    unsigned char               m_ucAGGREGATIONLEVEL;
}STfuPdcchInfo;

typedef struct STfuCntrlReqInfo
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned short              m_usCellId;
    struct STimingInfo          m_sULTimingInfo;
    struct STimingInfo          m_sDLTimingInfo;
    unsigned char               m_ucCFI;
    unsigned short              m_usNumberOfULPDCCHInfo;
    struct STfuPdcchInfo**      m_ppsULTFUPDCCHInfo;
    unsigned short              m_usNumberOfDLPDCCHInfo;
    struct STfuPdcchInfo**      m_ppsDLTFUPDCCHInfo;
    unsigned short              m_usNumberOfPHICHINFO;
    struct STfuPhiChInfo**      m_ppsTfuPHICHInfo;    

}STfuCntrlReqInfo;

/*********** Random Access Parameter *****************/
typedef struct STfuRaReqInfo
{
    struct SParameterHeader    m_sParameterHeader;
    unsigned char              m_ucPreambleId;
    unsigned short             m_usTimingAdvance;
}STfuRaReqInfo;

typedef struct STfuRachInfo
{
    struct SParameterHeader    m_sParameterHeader;
    unsigned short             m_usRaRNTI;
    unsigned short             m_usNumberOfTFURAREQINFO;
    struct STfuRaReqInfo**     m_ppsTfuRaReqInfo;
}STfuRachInfo;

typedef struct STfuRaReqIndInfo
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned short              m_usCellId;
    struct STimingInfo          m_sTimingInfo;
    unsigned short              m_usNumberOfTFURACHINFO;
    struct STfuRachInfo**       m_ppsTfuRachInfo;
}STfuRaReqIndInfo;

/********** Reception Request Information Parameter **********/

typedef struct STfuUeRecpReqInfo
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned short              m_usRNTI;
}STfuUeRecpReqInfo;

typedef struct STfuRecpReqInfo
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned short              m_usCellId;
    unsigned short              m_usNumberOfUE;
    struct STfuUeRecpReqInfo**  m_ppsTfuUeRecpReqInfo;
}STfuRecpReqInfo;

/*********** DL Data Request Parameters     **************/

typedef struct STfuPdschDciInfo
{
    struct SParameterHeader     m_sParameterHeader;	
}STfuPdschDciInfo;

typedef struct STfuDatReqPduInfo
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned short              m_usRNTI;
    struct STfuPdschDciInfo     m_sTfuPdschDciInfo;
    unsigned char               m_ucNumberOfTB;
    unsigned int*               m_puiMacPdubufferLen;
    unsigned char**             m_ppucMacPduBuffer;
}STfuDatReqPduInfo;

typedef struct STfuDatReqInfo
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned short              m_usCellId;
    struct STimingInfo          m_sTimingInfo;
    unsigned short              m_usLengthOfBCHBuffer;
    unsigned char*              m_pucBCHBuffer;
    unsigned short              m_usNumberOfTFUDATREQPDUINFO;
    struct STfuDatReqPduInfo**  m_ppsTfuDatReqPduInfo;
}STfuDatReqInfo;

/*********** Data Indication Information Parameter   *******/

typedef struct STfuDatInfo
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned short              m_usRNTI;
    unsigned int                m_uiDataPduLength;
    unsigned char*              m_pucDataPduBuffer;
}STfuDatInfo;

typedef struct STfuDatIndInfo
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned short              m_usCellId;
    struct STimingInfo          m_sTimingInfo;
    unsigned short              m_usNumberOfTFUDATINFO;
    struct STfuDatInfo**        m_ppsTfuDatInfo;
}STfuDatIndInfo;

/************** CRC Indication Information Parameter ************/

typedef struct STfuCrcInfo
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned short              m_usRNTI;
    unsigned char               m_ucISFAILURE;
    unsigned char               m_ucRV;
}STfuCrcInfo;

typedef struct STfuCrcIndInfo
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned short              m_usCellId;
    struct STimingInfo          m_sTimingInfo;
    unsigned short              m_usNumberOfTFUCRCINFO;
    struct STfuCrcInfo**        m_ppsTfuCrcInfo;
}STfuCrcIndInfo;

/************** CQI Indication Parameters *********************/

typedef struct STfuSubBandInfo 
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned char               m_ucRBSTART;
    unsigned char               m_ucNumberOfRB;
}STfuSubBandInfo;

typedef struct STfuSubBandCqiInfo
{
    struct SParameterHeader     m_sParameterHeader;
    struct STfuSubBandInfo      m_sTfuSubBandInfo;
    unsigned char               m_ucCQIIndex;
}STfuSubBandCqiInfo;

typedef struct STfuULCqiInfo
{
    struct SParameterHeader     m_sParameterHeader;
    struct STfuSubBandCqiInfo   m_sTfuSubBandCqiInfo;
}STfuULCqiInfo;

typedef struct STfuULCqiRpt
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned short              m_usRNTI;
    unsigned char               m_ucISTXPORT0;
    unsigned char               m_ucWideCqi;
    unsigned char               m_ucNumberOfSubBands;
    struct STfuULCqiInfo**      m_ppsTfuULCqiInfo;
}STfuULCqiRpt;

typedef struct STfuULCqiIndInfo
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned short              m_usCellId;
    struct STimingInfo          m_sTimingInfo;
    unsigned short              m_usNumberOFTFUULCQIRPT;
    struct STfuULCqiRpt**       m_ppsTfuULCqiRpt;
}STfuULCqiIndInfo;

/************** TTI Indication Parameters *********************/

typedef struct STfuTtiIndInfo
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned short              m_usCellId;
    STimingInfo                 m_sTimingInfo;
}STfuTtiIndInfo;

/************** SR Indication Parameters *********************/

typedef struct STfuSrInfo
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned short              m_usRNTI;
} STfuSrInfo;

typedef struct STfuSrIndInfo
{
    struct SParameterHeader     m_sParameterHeader;
    unsigned short              m_usCellId;
    STimingInfo                 m_sTimingInfo;
    unsigned short              m_usNumberOfTFUSRINFO;
    struct STfuSrInfo**         m_ppsTfuSrInfo;
} STfuSrIndInfo;

/************** Message Related Structures  *********************/

typedef struct SMessageHeader
{
    unsigned char       m_ucVersion;
    unsigned char       m_ucReserved;
    unsigned short      m_usType;
    unsigned int        m_uiLength;
}SMessageHeader;

typedef struct SDLControlRequest
{
    struct SMessageHeader       m_sMessageHeader;
    struct STfuCntrlReqInfo     m_sTfuCntrlReqInfo;
}SDLControlRequest;

typedef struct SRandomAccessIndication
{
    struct SMessageHeader       m_sMessageHeader;
    struct STfuRaReqIndInfo     m_sTfuRaReqIndInfo;
}SRandomAccessIndication;

typedef struct SULReceptionRequest
{
    struct SMessageHeader       m_sMessageHeader;
    struct STfuRecpReqInfo      m_sTfuRecpReqInfo;
}SULReceptionRequest;

typedef struct SDLDataRequest
{
    struct SMessageHeader       m_sMessageHeader;
    struct STfuDatReqInfo       m_sTfuDatReqInfo;
}SDLDataRequest;

typedef struct SULDataIndication
{
    struct SMessageHeader       m_sMessageHeader;
    struct STfuDatIndInfo       m_sTfuDatIndInfo;
}SULDataIndication;

typedef struct SULCRCIndication
{
    struct SMessageHeader       m_sMessageHeader;
    struct STfuCrcIndInfo       m_STfuCrcIndInfo;
}SULCRCIndication;

typedef struct SULCQIIndication
{
    struct SMessageHeader       m_sMessageHeader;
    struct STfuULCqiIndInfo     m_sTfuULCqiIndInfo;
}SULCQIIndication;

typedef struct SSRIndication
{
    struct SMessageHeader       m_sMessageHeader;
    struct STfuSrIndInfo        m_sTfuSrIndInfo;
} SULSRIndication;

#pragma pack( pop )
