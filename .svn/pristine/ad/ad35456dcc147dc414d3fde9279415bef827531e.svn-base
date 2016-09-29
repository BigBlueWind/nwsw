//-------------------------------------------------------------------------------------------
/** @file LtePhyL2Api.h
 *
 * @brief Defines the structures used in the LtePhyL2api
 * @author Mindspeed Technologies
 * @version $Revision: 1.70 $
 *
 * COPYRIGHT&copy; 2009-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/

//-------------------------------------------------------------------------------------------


#ifndef LTEPHYAPI_H
#define LTEPHYAPI_H

// This file defines the structures and variables used in the implementation
// of the LtePhyL2api
#ifndef CEVA_INTRINSICS
#if !defined(BER_TEST) && !defined(DL_TEST)
//	#include "ltypes.h"
#endif
#endif

#include "typedef.h"

// The following are constant definitions used in the LTE PHY L2 API
// implementation

// MAXMAPCNT corresponds to the maximum number of Resource Elements (RE's) maps
// per 1 ms subframe
// The MAXMAPCNT correspond to either a type 1 resource allocation with RBG
// subset 1 in a 20 MHz BW or a distributed VRB allocation (3GPP 36.213 section
// 7.1.6.2 and 7.1.6.3 and 36.211 6.2.3.2
#ifdef _WIN32
#define MAXMAPCNT	50
#else
#define MAXMAPCNT 50
#endif
// MAXCHSUBFRAME corresponds to the maximum number of channel descriptors
// available per subframe
// This considers a worst case of BW 20 MHZ and single Resource Block
// Allocation per Channel
#ifdef _WIN32
#ifdef HWSTATION
#define MAXCHSUBFRAME 39 //100
#else
#define MAXCHSUBFRAME 39
#endif
#else

#ifndef MAC_RT
#define MAXCHSUBFRAME 39
#else

#ifndef FDX_RT
#define MAXCHSUBFRAME 39
#else
/* This is temporary workaround to reduce size of TXVECTOR and RXVECTOR for
FDX Real-time execution. It's going to be removed as MAC side is optimized */
#define MAXCHSUBFRAME 3
#endif

#endif

#endif
// NUMSUBFRPERFRAME corresponds to the number of subframes per frame for LTE
// Mode 1 (FDD)
#define NUMSUBFRPERFRAME 10

//Defined for 20Mhz Bandwidth at the moment.  Should be doubled if we support 40Mhz
#define MAXSUBCARRIERS		1200
#define MAXRESOURCEBLOCKS    100

//maximum number of channels to run together for DL resource Element Assignment
#define MAXCHANTORUN    4

#define LENGTHMASK 0x07FF

#define         CE_Even 0
#define         CE_Any 1
#define         CE_NA -1

#define         CE_FALSE 0
#define         CE_TRUE  1

//
// Auto TX modes for PHY_START_REQ
//
#define AUTO_CPRI                       0
#define AUTO_TIMER                      1
#define AUTO_EVENT                      2
#define AUTO_COMPLETION                 3
#define AUTO_PCIE                       4
#define AUTO_DL                         0x80    // Bit-mask enables DL operation
#define AUTO_UL                         0x40    // Bit-mask enables UL operation

//
// API confirmations bitmask
//
#define CONF_TXSDU          0x00000001
#define CONF_TXSTART        0x00000002
#define CONF_RXSTART        0x00000004

enum TxModeOptions
{
    SINGLEANT = 1,
    TXDIVERSITY,
    OPNLPSPMUX,
    CLSLPSPMUX,
    MUMIMO,
    CLSLPR1PC,
    SINGLEANTP5
};

enum RadioAccessModeOptions
{
    OFDMA = 0, SC_FDMA
};

enum ChTypeOptions
{
    PDCCH = 0, PDSCH, PBCH, PCFICH, PHICH, PUSCH, PRACH, PUCCH
};

enum ChBwOptions
{
    BW_1_4_MHZ = 0,
    BW_3_0_MHZ,
    BW_5_0_MHZ,
    BW_10_0_MHZ,
    BW_15_0_MHZ,
    BW_20_0_MHZ
};

enum FrTypDuplex
{
    FDD = 0, TDD
};

enum PhyResBw
{
    PRB_12 = 12, PRB_24 = 24
};

enum NumRbspSlotOptions
{
    RB_SLOT_6 = 6,
    RB_SLOT_15 = 15,
    RB_SLOT_25 = 25,
    RB_SLOT_50 = 50,
    RB_SLOT_75 = 75,
    RB_SLOT_100 = 100
};

enum OcupBwOptions
{
    OBW_1_095 = 0, OBW_2_715, OBW_4_515, OBW_9_015, OBW_13_515, OBW_18_015
};

enum SampRateMhzOptions
{
    SRMHZ_1_92 = 0,
    SRMHZ_3_84,
    SRMHZ_7_68,
    SRMHZ_15_36,
    SRMHZ_23_04,
    SRMHZ_30_72
};

enum SampFactorOptions
{
    SF_1_3 = 0, SF_1_28, SF_1_536
};

enum TxAntPortCntOptions
{
    TXANT_ONE = 1, TXANT_TWO, TXANT_FOUR = 4
};

enum RxAntPortCntOptions
{
    RXANT_ONE = 1, RXANT_TWO, RXANT_FOUR = 4
};

enum FftSizeOptions
{
    FFTSZ_128 = 128,
    FFTSZ_256 = 256,
    FFTSZ_512 = 512,
    FFTSZ_1024 = 1024,
    FFTSZ_1536 = 1536,
    FFTSZ_2048 = 2048,
    FFTSZ_4096 = 4096
};

enum NumUsedSubCarriersOptions
{
    NUSC_72 = 72,
    NUSC_180 = 180,
    NUSC_300 = 300,
    NUSC_600 = 600,
    NUSC_900 = 900,
    NUSC_1200 = 1200
};

enum NmaxRbOptions
{
    NMAXRB_6 = 6,
    NMAXRB_15 = 15,
    NMAXRB_25 = 25,
    NMAXRB_50 = 50,
    NMAXRB_75 = 75,
    NMAXRB_100 = 100
};

#ifndef __ENVINDH__
enum StateOptions
{
    OFF = 0, ON
};
#endif

enum CyclicPrefixRatioOptions
{
    CPR_ZERO = 0, CPR_ONE_SIXTEENTH, CPR_ONE_EIGTH
};

enum CyclicPrefixTypeOptions
{
    NORMAL = 0, EXTENDED
};

enum AntennaPortOptions
{
    APZERO = 0, APONE, APTWO, APTHREE
};

enum SymbolsPerSlotOptions
{
    SL_THREE = 3, SL_SIX = 6, SL_SEVEN
};

enum ModulationIndexOptions
{
    QM_ONE = 1, QM_TWO, QM_FOUR = 4, QM_SIX = 6
};

#ifdef MSPD_API
// We are building EHDMAC code where these constants already defined
enum ModulationOptions
{
    _BPSK, _QPSK, _QAM16, _QAM64
};
#else
enum ModulationOptions
{
    BPSK, QPSK, QAM16, QAM64
};
#endif

enum SubFrameTypeOptions
{
    DLTX = 0, ULRX, ULTX, DLRX
};

enum CddOptions
{
    NOCDD = 0, LARGECDD
};

enum CodingOptions
{
    TURBOCDR = 0, TBVITCC, BLKCODCFI, BLKCODHARQ, CODEUCI
};

enum CrcLenOptions
{
    NONE = 0, EIGHT = 8, SIXTEEN = 16, TWTYFOUR = 24
};

enum ScramblerOptions
{
    DLSCR = 0, ULSCR
};

enum PucchFormatOptions
{
    FORMAT1 = 0, FORMAT1A, FORMAT1B, FORMAT2, FORMAT2A, FORMAT2B
};

enum Direction
{
    DL = 0, UL
};
enum StatusResultType
{
    PRACH_RESULT = 0, SRS_RESULT
};

#ifdef _WIN32
typedef enum
{
    MDMA_OP_COPY = 0x0000,
    MDMA_OP_HSDSCH=0x1000,
    MDMA_OP_CRC = 0x2000,
    MDMA_OP_RAND = 0x3000,
    MDMA_OP_3GDLSPCG = 0x4000,
    MDMA_OP_3GULSPCG = 0x5000,
    MDMA_OP_4SCR = 0xe000,
    MDMA_OP_4DSC = 0x7000,
    MDMA_OP_RAND_BE = 0x8000 | MDMA_OP_RAND,
    MDMA_OP_4SCR_BE = 0x8000 | MDMA_OP_4SCR

} MDMA_OP_TYPE;
#endif

// API Message Type Field coding definitions
#define PHY_TXSTART_REQ	 1
#define PHY_TXSTART_CONF 2
#define PHY_TXSTART_IND  3
#define PHY_TXSDU_REQ    4
#define PHY_TXSDU_CONF   5
#define PHY_TXEND_IND    6
#define PHY_RXSTART_REQ  7
#define PHY_RXSTART_CONF 8
#define PHY_RXSTART_IND  9
#define PHY_RXSDU_IND    10
#define PHY_RXEND_IND    11
#define PHY_INIT_REQ     12
#define PHY_INIT_IND     13
#define PHY_RXSTATUS_IND 14
#define PHY_RECONFIG_REQ 15
#define PHY_RECONFIG_CNF 16
#define PHY_START_REQ    17
#define PHY_START_CONF   18
#define PHY_STOP_REQ     19
#define PHY_STOP_CONF    20
#define PHY_STOP_IND     21

// PHY API protection mask
#define TXVEC_ACCEPTED    (1 << 0)
#define TXVEC_SDU_REJECT  (1 << 1)
#define RXVEC_ACCEPTED    (1 << 2)

// API Error Code Definitions
enum ApiErrCodeTypes {
#if !defined(SUCCESS)
    SUCCESS = 0,
#endif /* !ENODEB && !UE */
    UNSUPPORTED_PRIMITIVE = 1,
    UNSUPPORTED_FEC_CODE_TYPE,
    OVERRUN,
    UNDERRUN,
    TRANSPORTMEDIAERROR,
    TXDATASIZEMISSMATCH,
    INVALID_TX_RX_VECTOR
};

typedef struct GenMessageDescriptor
{
    BIT_FIELD msgSpecific :16;
    BIT_FIELD msgType :8;
    BIT_FIELD phyEntityId :8;

} GENMSGDESC, *PGENMSGDESC;

// Payload for PHY_START_REQ message
typedef struct PhyStartReq
{
    BIT_FIELD mode :16;
    BIT_FIELD msgType :8;
    BIT_FIELD phyEntityId :8;
    U32 count;                              // mode-dependent
    U32 period;                             // timer resolution
} STARTREQ, *PSTARTREQ;

typedef struct PhyInitIndDescriptor
{
    BIT_FIELD reserved :8;
    BIT_FIELD status :8;
    BIT_FIELD msgType :8;
    BIT_FIELD phyEntityId :8;
} INITIND, *PINITIND;

typedef struct PhyMsgIndDescriptor
{
    BIT_FIELD subFrameNum :8;
    BIT_FIELD status :8;
    BIT_FIELD msgType :8;
    BIT_FIELD phyEntityId :8;
    BIT_FIELD reserved :16;
    BIT_FIELD frameNumber :16;
} MSGIND, *PMSGIND;

typedef struct PhyTxSduReqDescriptor
{
    BIT_FIELD chanId :16;
    BIT_FIELD msgType :8;
    BIT_FIELD phyEntityId :8;
    BIT_FIELD nackAck:1;
    BIT_FIELD uciFormat:3;
    BIT_FIELD channelType:4;
    BIT_FIELD lulRbIdx:7;
    BIT_FIELD cwId :1;
    BIT_FIELD msgLen :16;
    BIT_FIELD phichGrpNumber:5;
    BIT_FIELD maxBitsperCw:27;
    U32* pTxSdu;
} TXSDUREQ, *PTXSDUREQ;

typedef struct PhyTxSduConfDescriptor
{
    BIT_FIELD subFrameNum :8;
    BIT_FIELD status :8;
    BIT_FIELD msgType :8;
    BIT_FIELD phyEntityId :8;
    BIT_FIELD chanId :16;
    BIT_FIELD frameNumber :16;
} TXSDUIND, *PTXSDUIND;

typedef struct PhyRxSduIndDescriptor
{
    BIT_FIELD channelId :16;
    BIT_FIELD msgType :8;
    BIT_FIELD phyEntityId :8;
    BIT_FIELD numBitsRx :24;
    BIT_FIELD status :8;
    BIT_FIELD frameNum :16;
    BIT_FIELD subFrameNum :8;
    BIT_FIELD chanType :8;
    U32 pRxSdu;
} RXSDUIND, *PRXSDUIND;

typedef struct PhyRxStatusIndDescriptor
{
    BIT_FIELD msgSpecific :16;
    BIT_FIELD msgType  :8;
    BIT_FIELD phyEntityId :8;
    BIT_FIELD frameNum :16;
    BIT_FIELD subFrameNum :8;
    BIT_FIELD eventNumber :8;
    BIT_FIELD statusType :8;
    U32 pEventList;
} RXSTATUSIND, *PRXSTATUSIND;

typedef struct PhyPrachStatusEventDescriptor
{
    BIT_FIELD prachConfigIdx :6;
    BIT_FIELD reserved  :10;
    BIT_FIELD timingOffset :16;
} RX_PRACH_STATUS_EVENT, *PRX_PRACH_STATUS_EVENT;


typedef struct ScramblerDescriptor
{
    U8 scramblerType;
    U16 scrinitValueinput;
} SCRAMBLERINFO, *PSCRAMBLERINFO;

typedef struct ReselemDescriptor
{
    U8 startRes;
    U8 numRes;
} RESELEMINFO, *PRESELEMINFO;

typedef struct MappingDescriptor
{
    U16 numberofEntries;
    RESELEMINFO reselmInfo[MAXMAPCNT];
} MAPPINGINFO, *PMAPPINGINFO;

typedef struct CrcDescriptor
{
    BIT_FIELD crcLength :6;
    BIT_FIELD crcScrambling :2;
} CRCINFO, *PCRCINFO;

typedef struct HarqDescriptor
{
    // Moved process number out
    BIT_FIELD reserved :3;
    // New Data Indication
    BIT_FIELD nDi :1;
    // Redundancy Version
    BIT_FIELD rV :2;
    // Flush Buffer Request
    BIT_FIELD flushReq :2;
} HARQINFO, *PHARQINFO;

typedef struct CtrlDescriptor
{
    BIT_FIELD format_type :3;
    BIT_FIELD n1_cs :3;
    BIT_FIELD delta_shift_pcch :2;
    BIT_FIELD n2_rb :8;
    BIT_FIELD n1_pcch :16;
    BIT_FIELD beta_pcch :16;
    BIT_FIELD n2_pcch :16;

} CTLINFO, *PCTLINFO;


typedef struct SoundingRefDedicated
{
   BIT_FIELD enableDisable:8;
   BIT_FIELD srsBandwidth:2;
   BIT_FIELD srsHoppingBandwidth:2;
   BIT_FIELD transmissionComb:1;
   BIT_FIELD cyclicShift:3;
   BIT_FIELD freqDomainPosition:5;
   BIT_FIELD duration:1;
   BIT_FIELD srsConfigIndex:10;
}  SRSDED, *PSRSDED;

typedef struct DlDedicatedPrecoderDescriptor
{
    // Cyclic Diversity Delay
    BIT_FIELD cddType :1;
    // Code Book Index
    BIT_FIELD codeBookIdx :4;
    BIT_FIELD reserved :27;
    U8 codebookSubsetRestriction[8];
} DLDEDICATEDPRECODERINFO, *PDLDEDICATEDPRECODERINFO;

typedef struct ModulationCodingDescriptor
{
    BIT_FIELD codingDescriptor :3;
    BIT_FIELD blockCodeConcatenation :1;
    BIT_FIELD modulationType :4;
    BIT_FIELD mcsType :8;
} MCINFO, *PMCINFO;

typedef struct DLCommonTxPowerControl
{
    S16 pilotPower;
    S16 psyncPower;
    S16 ssyncPower;
    S16 ciPower;
    U16 paprControl;
    U16 paprThreshold;
} DLCMNTXPWRCTL, *PDLCMNTXPWRCTL;

typedef struct RachControlDescriptor
{
    BIT_FIELD prachEnable :1;
    BIT_FIELD rootSequenceIndex :10;
    BIT_FIELD prachConfigIndex :6;
    BIT_FIELD highSpeedFlag :1;
    BIT_FIELD zeroCorrelationZoneConfig :4;
    BIT_FIELD prachFreqOffset :10;
} RACHCTRL, *PRACHCTRL;

typedef struct PuschConfigDedicated
{
    BIT_FIELD betaOffsetACKIndex:4;
    BIT_FIELD betaOffsetRIIndex:4;
    BIT_FIELD betaOffsetCQIIndex:4;
    BIT_FIELD padding: 4;
    BIT_FIELD reserved: 16;
} PUSCHDED, *PPUSCHDED;

typedef struct CqiPmiRireportCtrl
{
    BIT_FIELD cqiReportModeAperiodic: 3;
    BIT_FIELD nomPDSCHRSEPREOffset: 3;
    BIT_FIELD cqiReportPeriodicEnable: 2;
    BIT_FIELD cqiPUCCHResourceIndex: 11;
    BIT_FIELD cqipmiConfigIndex: 10;
    BIT_FIELD cqiFormatIndicatorPeriodic: 1;
    BIT_FIELD K: 2;
    BIT_FIELD riConfigIndex: 10;
    BIT_FIELD simultaneousAckNackAndCQI: 1;
    BIT_FIELD padding: 21;
} CQIPMIRIRPT, *PCQIPMIRIRPT;

typedef struct PucchDedicatedCtrl
{
    BIT_FIELD formatType:3;
    BIT_FIELD ackNackRepetition:1;
    BIT_FIELD repetitionFactor:6;
    BIT_FIELD n1PucchANRep:11;
    BIT_FIELD cqiPUCCHResourceIndex:11;
    BIT_FIELD srPUCCHResourceIndex:32;
} PUCCHDEDCTL, *PPUCCHDEDCTL;

typedef struct DlSubChannelDescriptor
{
    BIT_FIELD codingDescriptor :3;
    BIT_FIELD blockCodeConcatenation :1;
    BIT_FIELD modulationType :4;
    BIT_FIELD mcsType :8;
    // Moved process number out
    BIT_FIELD reserved :3;
    // New Data Indication
    BIT_FIELD nDi :1;
    // Redundancy Version
    BIT_FIELD rV :2;
    // Flush Buffer Request
    BIT_FIELD flushReq :2;
    BIT_FIELD pad:8;                 // To preserve 32 bits boundary for BIT_FIELDS
} DLSUBCHINFO, *PDLSUBCHINFO;

typedef struct DciChannelDescriptor
{
    U32 offsetNextCh;
    U16 channelId;
    S16 txpowerControl;
    BIT_FIELD crcLength :6;
    BIT_FIELD crcScrambling :2;
    BIT_FIELD channelType:5;
    BIT_FIELD reserved:3;
    BIT_FIELD numCodeWords:4;
    BIT_FIELD nLayers :4;
    BIT_FIELD transmissionMode :8;
    BIT_FIELD reserved1:16;
    BIT_FIELD scrmblerInitValue:16;
    U16 numberofEntries;
    U8  startRes;
    U8 numRes;
} DCICHANNELDESC, *PDCICHANNELDESC;

typedef struct UlSubChannelDescriptor
{
    MCINFO mcinfo; //16
    HARQINFO harqInfo;//8
    CRCINFO crcInfo;//8
    SCRAMBLERINFO scrInfo; //24
    U8 reserved; //8
    PUSCHDED puschDed;//32
    CQIPMIRIRPT cqiPmiRiRpt;
    MAPPINGINFO mapInfo; //32
} ULSUBCHINFO, *PULSUBCHINFO;

typedef struct DlChannelDescriptor
{
    U32 offsetNextCh;
    U16 channelId;
    S16 txpowerControl;
    BIT_FIELD persistEnable:1;
    BIT_FIELD repeatCycle:7;
    BIT_FIELD channelType:5;
    BIT_FIELD hCid:3;
    BIT_FIELD numCodeWords :4;
    // nLayers refers to the number of Tx layers when part of the TXVECTOR
    // and to Rx layers when used in the RXVECTOR
    BIT_FIELD nLayers :4;
    BIT_FIELD transmissionMode :8;
    DLSUBCHINFO subChInfo[2];
    CRCINFO crcInfo;
    SCRAMBLERINFO scrInfo;
    DLDEDICATEDPRECODERINFO dlPrecoderInfo;
    MAPPINGINFO mapInfo;
    U16 pad;
} DLCHANDESC, *PDLCHANDESC;

typedef struct UlChannelDescriptor
{
    U32 offsetNextCh;
    U16 channelId;
    S16 txpowerControl;
    BIT_FIELD persistEnable:1;
    BIT_FIELD repeatCycle:7;
    BIT_FIELD channelType:8;
    BIT_FIELD halfIterations:16;
    ULSUBCHINFO ulSubChInfo;
} ULCHANDESC, *PULCHANDESC;

typedef struct UlCtrlChannelDescriptor
{
    U32 offsetNextCh;
    U16 channelId;
    S16 txpowerControl;
    BIT_FIELD crcLength:8;
    BIT_FIELD channelType:8;
    BIT_FIELD reserved:16;
    BIT_FIELD scrmblerInitValue:16;
    BIT_FIELD codingDescriptor :3;
    BIT_FIELD blockCodeConcatenation :1;
    BIT_FIELD modulationType :4;
    BIT_FIELD mcsType :8;
    PUCCHDEDCTL pucchDedCtl;
    CQIPMIRIRPT cqiPmiRiRpt;
    U16 numberofEntries;
    U8 startRes;
    U8 numRes;
} ULCTRLCHDESC, *PULCTRLCHDESC;

typedef struct ULSubframeCommonControl
{
    BIT_FIELD deltaPUCCHShift :2;
    BIT_FIELD nRBCQI:7;
    BIT_FIELD nCSAn:3;
    BIT_FIELD n1PucchAN:11;
    BIT_FIELD pad:9;
    BIT_FIELD srsBandwitdhConfig:3;
    BIT_FIELD srsSubframeConfig:4;
    BIT_FIELD ackNackSRSSimultaneousTransmission:1;
    BIT_FIELD nSB:2;
    BIT_FIELD hoppingMode:1;
    BIT_FIELD puschhoppingOffset:7;
    BIT_FIELD enable64QAM:1;
    BIT_FIELD groupHoppingEnabled:1;
    BIT_FIELD groupAssignmentPUSCH:5;
    BIT_FIELD sequenceHoppingEnabled:1;
    BIT_FIELD cyclicShift:3;
    BIT_FIELD padding:3;

} ULSUBFRCMNCTRL, *PULSUBFRCMNCTRL;

typedef struct DlSubframeDescriptor
{
    U16 frameNumber;
    U8 subframeNumber;
    U8 subframeType;
    U8 antennaPortcount;
    U8 numberofChannelDescriptors;
    U16 reserved0;
    U32 offsetPowerCtrl;
    U32 offsetDCIChannels;
    U32 offsetCustomFeatures;
    BIT_FIELD  numCtrlSymbols :3;
    BIT_FIELD  phichResource :2;
    BIT_FIELD  phichDuration :3;
    BIT_FIELD  numberOfPhichChannels:8;
    BIT_FIELD  numCtrlChannels:16;
    DLCHANDESC dlCh[MAXCHSUBFRAME];
    DCICHANNELDESC dciCh[MAXCHSUBFRAME];
//  The location of this parameter is specified in the API NFD and disclosed to
//  our customers. The DLCMNTXPWRCTL field is now optional in the TXVector.
    DLCMNTXPWRCTL dlCmnPwrCtl;
} DLSUBFRDESC, *PDLSUBFRDESC;


typedef struct UlSubframeDescriptor
{
    U16 frameNumber;
    U8 subframeNumber;
    U8 subframeType;
    U8 antennaPortcount;
    U8 numberofChannelDescriptors;
    U8 numberOfCtrlChannelDescriptors;
    U8 numberSrsinSf;
    U32 offsetRachCtrlStruct;
    U32 offsetULCtrlChannels;
    U32 offsetsrsInfo;
    U32 offsetCustomFeatures;
    ULSUBFRCMNCTRL ulSfrCtrl;
    ULCHANDESC ulCh[MAXCHSUBFRAME];
    ULCTRLCHDESC ulCtlCh[MAXCHSUBFRAME];
    SRSDED srsInfo[MAXCHSUBFRAME];
    RACHCTRL rachCtrl;

} ULSUBFRDESC, *PULSUBFRDESC;


typedef struct InitParamDescriptor
{
    U8 channelBandwidth;
    U8 frtypeDuplexmode;
    U8 radioAccessMode;
    U8 physicalResourceBandwidth;
    U8 numberResourceBlocksperSlot;
    U8 phyLayerCellIdGroup;
    U8 phyLayerId;
    U8 txAntennaPortCount;
    U16 fastfwdorPreclockingNumber;
    U16 fftSize;
    U16 numberUsedSubcarriers;
    U8 nMaxDlRb;
    U8 nMaxUlRb;
    S16 referenceSignalPower;
    S16 primarySyncSignalPower;
    S16 secondarySyncSignalPower;
    U8 numDataRePerPRB;
    U8 cyclicPrefixType;
    BIT_FIELD rxAntennaPortCount:8;
    BIT_FIELD reserved:16;
    BIT_FIELD txStartConfDisable:1;
    BIT_FIELD rxStartConfDisable:1;
    BIT_FIELD pb:2;
    BIT_FIELD customExtensionReportEn:1;
    BIT_FIELD rachReportMode:1;
    BIT_FIELD txSduConfEnable:1;
    BIT_FIELD sduConfig:1;
    U16 radioFrameNumber;
    U8 subframeNumber;
    U8 slotNumber;
    S16 srsBandwidthConfig;
    S16 srsSubframeConfig;
    S16 srsSimultaneousANandSRS;
    S16 prachConfigurationIndex;
    S16 prachFrequencyOffset;
    S16 prachHighSpeedFlag;
    S16 prachCyclicShiftConfig;
    S16 prachRootSequenceIndex;
} INITPARM, *PINITPARM;

typedef struct PrecoderInputParams
{
    // Parameters from pPHY structure
    U8 Ntx_antennas;
    U8 txSubFrameNumber;
    U8 nDlSymb;
    U8 nRbSc;
    S16 RBIndexMid6Lo;
    S16 RBIndexMid6Hi;
    U8 numCtrlSymbols;
    // Parameters from pChan structure
    U8 transmissionMode;
    U8 nLayers;
    U8 codeBookIdx;
    U8 cddType;
    U16 numberofEntries;
    RESELEMINFO reselmInfo[MAXMAPCNT];
} PRECODERPARAM, *PPRECODERPARAM;

typedef struct DLResourceInputParamsARM
{
    // Parameters from pPHY structure
    U8  Ntx_antennas;
    U8  txSubFrameNumber;
    U8  nDlSymb;
    U8  nRbSc;
    S16 RBIndexMid6Lo;
    S16 RBIndexMid6Hi;
    U8  numCtrlSymbols;
    // Parameters from pChan structure
    U16 numberofEntries[MAXCHSUBFRAME];
    S16 channelType[MAXCHSUBFRAME];
} DLRESOURCEPARAM_ARM, *PDLRESOURCEPARAM_ARM;

typedef struct DLResourceInputParams
{
    // Parameters from pPHY structure
    U8  Ntx_antennas;
    U8  txSubFrameNumber;
    U8  nDlSymb;
    U8  nRbSc;
    S16 RBIndexMid6Lo;
    S16 RBIndexMid6Hi;
    U8  numCtrlSymbols;
    // Parameters from pChan structure
    U16 numberofEntries;
    S16 channelType;
    U32 Ncell_id;
} DLRESOURCEPARAM, *PDLRESOURCEPARAM;

typedef struct UlPilotsInputParams
{
    S16 Mpusch_sc;
    S16 pilotCarrierSymbol;
    S16 groupHoppingEnabled;
    S16 n1_DMRS;
    S16 n2_DMRS;
    S16 delta_ss;
    S16 Ncell_id;
    S16 Nrb_sc;
    S16 Nul_symb;
    S16 Row_RBAssignment;
    S16 Col_RBAssignment;
    S16 K;
    S16 L;
    S16 MI;
    S16 slotNum;
    S16 delta_shift_PUCCH;
    S16 betaPUCCH;
    S16 num_Pilots_slot;
    S16 N2_rb;
    S16 n1;
    S16 n2;
    S16 N1_cs;
    S16 CPType;
    S16 pucchFormat;
    S16 RSbetaPUCCH;
    S16 Nul_rb;
} ULPILOTSPARAM, *PULPILOTSPARAM;

typedef struct DemuxFDMAPhysChanParams
{
    S16 Nsym;
    S16 Nrb_sc;
    S16 Row;
    S16 K;
    S16 NusedE;
    S16 Col;
    S16 ChanType;
} DEMUXFDMAPHYSCHAN, *PDEMUXFDMAPHYSCHAN;

typedef struct ChanEstInputParams
{
//    S16 pilotCarrierSymbol;
//    S16 Mpusch_rb;
//    S16 Mpusch_sc;
    S16 df;
    U32 Fs;
    S16 Nul_symb;
    S16 Nrb_sc;
    S16 NusedE;
    S16 Nfft;
    S16 Nul_rb;
//    S16 start_address;
//    S16 islot;
} CHANESTPARAM, *PCHANESTPARAM;

typedef struct ChanEstInputParamsSrs
{
	S16 df;
	U32 Fs;
	S16 Nul_symb;
	S16 Nrb_sc;
	S16 NusedE;
	S16 Nfft;
	S16 start_address;
	S16 islot;
	S16 length_aEstSig;
	S16 length_rSrs;
} CHANESTPARAMSRS, *PCHANESTPARAMSRS;

typedef struct MsChanEstParams
{
	S16  Nused, Nfft, nDlSymb;
	S16  MI, row_a, col_a, iSymInSubframe, mo;
	U32  Fs;
	U16  df;
	S16  Noffset[4];
    S16  expoNoffset[4];
	S16  expo_fac[7];		// Max is 7 (Ndl_symb)
	S16  dummy;				// This is to make it 64-bit aligned. Not sure why... :)

} MSCHANESTPARAMS, *PMSCHANESTPARAMS;

typedef struct TxMapperMultiChanIn
{
    U8 TxPackedBitBuf0[1200];
    U8 TxPackedBitBuf1[1200];
} TXMAPPERMULTICHANIN, *PTXMAPPERMULTICHANIN;

typedef struct TxMapperMultiChanOut
{
    S16 TxMapperOutBuf0[2400];
    U32 Buf4M0symb[100];
    S16 TxMapperOutBuf1[2400];
    U32 Buf4M1symb[100];
    S16 OutputStatus;
} TXMAPPERMULTICHANOUT, *PTXMAPPERMULTICHANOUT;

typedef struct TxLayerMapperMultiChanOut
{
    S16 TxLayerMapperOutBuf[9600];
    U16 xIndex[100];
    U16 nrow_x[100];
    U16 ncol_x[100];
    U16 ncol_y[100];
    S16 OutputStatus;
} TXLAYERMAPPERMULTICHANOUT, *PTXLAYERMAPPERMULTICHANOUT;

typedef struct TxPrecoderMultiChanOut
{
    S16 TxPrecoderOutBuf[9600];
    U16 ncol_y[100];
    U16 yIndex[100];
    U16 length_y[100];
    S16 OutputStatus;
} TXPRECODERMULTICHANOUT, *PTXPRECODERMULTICHANOUT;

typedef struct DLResElemMapperMultiChanOut
{
//    S16 *aSymbol[TXANT_FOUR];
    S16 scIndex[1200];
    S16 scale_factor[100];
    S16 OutputStatus;
} DLRESELEMMAPPERMULTICHANOUT, *PDLRESELEMMAPPERMULTICHANOUT;

typedef struct Tx_Modulation_Vars
{
	U32 isym, len3, numChan, numberofControlChannels;
	U16 Ntx_antennas, numCodeWords;
    S16 Ncell_id;
    S16 CPType;
    S16 cellSector;
    S16 Ndl_rb;
    S16 Ndl_symb;
    S16 Nrb_sc;
    S16 pilotScaleFactor;
    S16 primarySyncScaleFactor;
    S16 secondarySyncScaleFactor;
    S16 CE_SyncTxAntennaNum;
    S16 slotNumber;
    S16 RBIndexMid6Lo;
    S16 RBIndexMid6Hi;
    S16 numCtrlSymbols;
} TXMODULATIONVARS, *PTXMODULATIONVARS;

typedef struct Tx_Modulation_Params
{
	U32 *_totalsizescr;
	U32 *scrIndex1, *scrIndex2;
	U8 *scrOut1, *scrOut2;
	S16 *pl_k_Isc, *phyChanScale_factor;
	S32 *pBit;
	S16 *pncol_y;

	PRESELEMINFO pReselmInfo;
	PTXMAPPERMULTICHANIN pIn_a;
	PTXMAPPERMULTICHANOUT pOut_a;
	PTXLAYERMAPPERMULTICHANOUT pOut_b;
	PTXPRECODERMULTICHANOUT pOut_c;
	PDLRESELEMMAPPERMULTICHANOUT pOut_d;
} TXMODULATIONPARAMS, *PTXMODULATIONPARAMS;

typedef struct structFilePrint
{
	char fileName[1024];
	int size;
	char type[10], mode[3];
} SFILEPRINT, *PSFILEPRINT;

#endif
