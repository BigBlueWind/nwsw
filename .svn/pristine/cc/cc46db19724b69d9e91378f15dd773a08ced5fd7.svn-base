//-------------------------------------------------------------------------------------------
/** @file LtePhyL2Api.h
 *
 * @brief Defines the structures used in the LtePhyL2api
 * @author Mindspeed Technologies
 * @version $Revision: 1.128 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
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
//  #include "ltypes.h"
#endif
#endif

#include "typedef.h"

//#define _LTEAPIDUMP_    //Enables or disables the dumping of API commands between a host and PHY application

// The following are constant definitions used in the LTE PHY L2 API
// implementation

// MAXMAPCNT corresponds to the maximum number of Resource Elements (RE's) maps
// per 1 ms subframe
// The MAXMAPCNT correspond to either a type 1 resource allocation with RBG
// subset 1 in a 20 MHz BW or a distributed VRB allocation (3GPP 36.213 section
// 7.1.6.2 and 7.1.6.3 and 36.211 6.2.3.2
#ifdef _WIN32
#define MAXMAPCNT   50
#else
#define MAXMAPCNT 50
#endif

// MAXCHSUBFRAME corresponds to the maximum number of channel descriptors
// available per subframe
// This si the current capacity defined for "Turner / T40xx / T3000" products
#define MAXCHSUBFRAME 30         //old value, used for everything
//!!!!!!!!! cannot use these yet, since using linearized 'chan' arrays. "chan" vector length must be common in all arrays for now, else chanOffsets won't work!
//#define MAXCHSUBF_ULDATA  6                         //---all derived from here
//#define MAXCHSUBF_DLDATA  (MAXCHSUBF_ULDATA +2)       //extra 2 channels for BCH and SIB
//#define MAXCHSUBF_ULACK   (MAXCHSUBF_DLDATA -2)
//#define MAXCHSUBF_ULCQI   (MAXCHSUBF_DLDATA -2)
//#define MAXCHSUBF_ULCTL   (MAXCHSUBF_ULACK + MAXCHSUBF_ULCQI)
//#define MAXCHSUBF_DLACK   MAXCHSUBF_ULDATA
//#define MAXCHSUBF_DLCTL   (MAXCHSUBF_DLDATA + MAXCHSUBF_ULDATA)
//#define MAXCHSUBF_UL      (MAXCHSUBF_ULDATA + MAXCHSUBF_ULACK + MAXCHSUBF_ULCQI)
//.... or maybe more accurate to do
//#define MAXCHACTIVE   50
//#define MAXCHRRCCONNECTED  (2*MAXCHACTIVE)
//#define MAXCHSUBF_ULCQI   (MAXCHACTIVE / 5)       //for maxm periodicity 5ms
//#define MAXCHSUBF_SR      (MAXCHRRCCONNECTED / 5) //for maxm periodicity 5ms
//.... but, to miminize memory, need many different "chan" definitions in the arrays, e.g. actchan, rrcchan, srchan, cqichan, ...etc.
//.... for now, assume maxm for T4000 is; 6 chanPerTTI, 50 chanActive, 100 chanRRCConnected.
//.... So, maxm of ALL channel types is MAXCHSUBF=100/5 = 20  = MAXCHSUBFRAME
//.............
// MAXACTIVECHAN corresponds to the maximum number of active channels tracked by the PHY
#define MAXACTIVECHAN   8*MAXCHSUBFRAME
//!!!!!!!!!!!!!

// NUMSUBFRPERFRAME corresponds to the number of subframes per frame for LTE
// Mode 1 (FDD)
#define NUMSUBFRPERFRAME 10

#ifdef AC_MSPD_TDD
//Added number of UL/DL configuration definition by ArrayComm 2011-1-23.
// NUM_ULDLCONFIG corresponds to the number of UL/DL configuration for TDD
#define NUM_ULDLCONFIG 7
//End added number of UL/DL configuration definition
#endif

//Defined for 20Mhz Bandwidth at the moment.  Should be doubled if we support 40Mhz
#define MAXSUBCARRIERS      1200
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
// PHY Init Bit Masks
//

#define PHYINIT_NEW_MAC_TO_PHY_INTERFACE        1<<0
#define PHYINIT_PHY_MAINTAINS_PBCH              1<<1
#define PHYINIT_ROBUST_CONTROL                  1<<2
#define PHYINIT_USE_TXSDU_POINTER               1<<3
#define PHYINIT_TWO_MUXCONTROL_LISTS            1<<4
#define PHYINIT_DONT_FREE_NEXT_MISSED_VECTOR    1<<5

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

//
// API indications bitmask
//


enum TxModeOptions
{
    SINGLEANT = 1, TXDIVERSITY, OPNLPSPMUX, CLSLPSPMUX, MUMIMO, CLSLPR1PC, SINGLEANTP5
};

enum RadioAccessModeOptions
{
    OFDMA = 0, SC_FDMA
};

enum ChTypeOptions
{
    PDCCH = 0, PDSCH, PBCH, PCFICH, PHICH, PUSCH, PRACH, PUCCH, MEASUREMENT
};

enum ChBwOptions
{
    BW_1_4_MHZ = 0, BW_3_0_MHZ, BW_5_0_MHZ, BW_10_0_MHZ, BW_15_0_MHZ, BW_20_0_MHZ
};

enum FrTypDuplex
{
    FDD = 0, TDD
};

#ifdef AC_MSPD_TDD
//Added TDD related configuration ENUM by Arraycomm, 2011-1-23
enum UlDlCfgOptions // UL/DL configuration
{
    ULDL_CFG_0 = 0, ULDL_CFG_1, ULDL_CFG_2, ULDL_CFG_3, ULDL_CFG_4, ULDL_CFG_5, ULDL_CFG_6
};

enum SpecialSfOptions // special subframe configuration
{
    SPEC_SF_CFG_0 = 0,
    SPEC_SF_CFG_1,
    SPEC_SF_CFG_2,
    SPEC_SF_CFG_3,
    SPEC_SF_CFG_4,
    SPEC_SF_CFG_5,
    SPEC_SF_CFG_6,
    SPEC_SF_CFG_7,
    SPEC_SF_CFG_8
};

enum srsMaxUpPtsOptions //SRS in UpPTS MAX configuration
{
    SRS_MAX_UPPTS_DIS = 0,//disable srs max in upPTS
    SRS_MAX_UPPTS_ENA
//enable srs max in upPTS
};

//End added TDD related configuration ENUM


//Added ENUM for Arraycomm special functions by Arraycomm, 2011-1-23
enum CreateHichOptions
{
    CREATE_HICH_DIS = 0,//disable PHY to generate HICH
    CREATE_HICH_ENA
};

enum RachRespWindSizes
{
    RACH_RESP_WIN_SIZE_2 = 2,
    RACH_RESP_WIN_SIZE_3 = 3,
    RACH_RESP_WIN_SIZE_4 = 4,
    RACH_RESP_WIN_SIZE_5 = 5,
    RACH_RESP_WIN_SIZE_6 = 6,
    RACH_RESP_WIN_SIZE_7 = 7,
    RACH_RESP_WIN_SIZE_8 = 8,
    RACH_RESP_WIN_SIZE_10 = 10
};

enum RntpThresholdOptions
{
    RNTP_THRESHOLD_0 = 0,
    RNTP_THRESHOLD_1,
    RNTP_THRESHOLD_2,
    RNTP_THRESHOLD_3,
    RNTP_THRESHOLD_4,
    RNTP_THRESHOLD_5,
    RNTP_THRESHOLD_6,
    RNTP_THRESHOLD_7,
    RNTP_THRESHOLD_8,
    RNTP_THRESHOLD_9,
    RNTP_THRESHOLD_10,
    RNTP_THRESHOLD_11,
    RNTP_THRESHOLD_12,
    RNTP_THRESHOLD_13,
    RNTP_THRESHOLD_14,
    RNTP_THRESHOLD_15
};
//End added ENUM for Arraycomm special functions
#endif

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
    SRMHZ_1_92 = 0, SRMHZ_3_84, SRMHZ_7_68, SRMHZ_15_36, SRMHZ_23_04, SRMHZ_30_72
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
    NUSC_72 = 72, NUSC_180 = 180, NUSC_300 = 300, NUSC_600 = 600, NUSC_900 = 900, NUSC_1200 = 1200
};

enum NmaxRbOptions
{
    NMAXRB_6 = 6, NMAXRB_15 = 15, NMAXRB_25 = 25, NMAXRB_50 = 50, NMAXRB_75 = 75, NMAXRB_100 = 100
};
#if 0
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
    PRACH_RESULT = 0, SRS_RESULT, CQIRIHI_RESULT
};

#ifdef AC_MSPD_TDD
//Added phichDurationMode by Arraycomm 2010-12-01
enum phichDurationMode
{
    NORMAL_DURATION = 0, EXTENDED_DURATION
};
//End Added phichDurationMode
#endif

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

}MDMA_OP_TYPE;
#endif

// API Message Type Field coding definitions
#define PHY_TXSTART_REQ     1
#define PHY_TXSTART_CONF    2
#define PHY_TXSTART_IND     3
#define PHY_TXSDU_REQ       4
#define PHY_TXSDU_CONF      5
#define PHY_TXEND_IND       6
#define PHY_RXSTART_REQ     7
#define PHY_RXSTART_CONF    8
#define PHY_RXSTART_IND     9
#define PHY_RXSDU_IND       10
#define PHY_RXEND_IND       11
#define PHY_INIT_REQ        12
#define PHY_INIT_IND        13
#define PHY_RXSTATUS_IND    14
#define PHY_RECONFIG_REQ    15
#define PHY_RECONFIG_CNF    16
#define PHY_START_REQ       17
#define PHY_START_CONF      18
#define PHY_STOP_REQ        19
#define PHY_STOP_CONF       20
#define PHY_STOP_IND        21
#define PHY_TXHIADCIUL_REQ  22
#define PHY_TXHISDU_REQ     23
#define PHY_TXDCIULSDU_REQ  24
#define PHY_PAUSE_REQ       25
#define PHY_RESUME_REQ      26
#define PHY_ERROR_IND       27

// The following two are only for internal MSPD use
#define PHY_SHUTDOWN_REQ 254
#define PHY_SHUTDOWN_CONF 255

#ifdef AC_MSPD_TDD
//Added PHY_QUERY, PHY_MEASURE by ArrayComm 2010-12-06.
#define PHY_QUERY_REQ    501
#define PHY_QUERY_IND    502
#define PHY_MEASURE_REQ  503
#define PHY_MEASURE_IND  504
//End added PHY_QUERY, PHY_MEASURE.
#endif

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
    INVALID_TX_VECTOR,
    INVALID_RX_VECTOR,
    RX_CRC_ERROR,
    TX_TIMEOUT_MISSING_TXSDU,
    MISSMATCH_RXSDU_VS_RXVECTOR,
    TIMEOUT_TXSTART_REQ,
    TIMEOUT_RXSTART_REQ

};

typedef struct GenMessageDescriptor
{
    BIT_FIELD msgSpecific :16;
    BIT_FIELD msgType :8;
    BIT_FIELD phyEntityId :8;

} GENMSGDESC, *PGENMSGDESC;

typedef struct HiandDciUlMessageDescriptor
{
    BIT_FIELD msgLength:16;
    BIT_FIELD msgType:8;
    BIT_FIELD phyEntityId:8;
    BIT_FIELD numDciUlSdus:8;
    BIT_FIELD numHiSdus:8;
    BIT_FIELD subFrameNumber:4;
    BIT_FIELD reserved:2;
    BIT_FIELD frameNumber:10;
}HIADCIULMSGDESC, *PHIADCIULMSGDESC;

typedef struct HiInfoDescriptor
{
    BIT_FIELD msgLength:16;
    BIT_FIELD msgType:8;
    BIT_FIELD phyEntityId:8;
    BIT_FIELD nackAck:1;
    BIT_FIELD transmissionMode:3;
    BIT_FIELD phichSequenceIndex:7;
    BIT_FIELD phichGroupNumber:5;
    BIT_FIELD channelId:16;
    BIT_FIELD reserved:16;
    BIT_FIELD txPowerControl:16;
}HIINFOMSGDESC, *PHIINFOMSGDESC;

typedef struct DciUlSduHeaderDescriptor
{
    BIT_FIELD msgLength:16;
    BIT_FIELD msgType:8;
    BIT_FIELD phyEntityId:8;
    BIT_FIELD channelId:16;
    BIT_FIELD numPayloadBytes:8;
    BIT_FIELD numBitsDciUL:8;
    BIT_FIELD rnti:16;
    BIT_FIELD numCCE:8;
    BIT_FIELD startCCE:8;
    BIT_FIELD transmissionMode:16;
    BIT_FIELD txPowerControl:16;
    U32* pTxSdu;
}DCIULSDUMSG, *PDCIULSDUMSG;

// Payload for PHY_START_REQ message
typedef struct PhyStartReq
{
    BIT_FIELD mode :16;
    BIT_FIELD msgType :8;
    BIT_FIELD phyEntityId :8;
    U32 count; // mode-dependent
    U32 period; // timer resolution
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
    BIT_FIELD nackAck :1;
    BIT_FIELD uciFormat :3;
    BIT_FIELD channelType :4;
    BIT_FIELD phichSeqIndex :7;
    BIT_FIELD cwId :1;
    BIT_FIELD msgLen :16;
#ifndef AC_MSPD_TDD
    BIT_FIELD phichGrpNumber :5;
    BIT_FIELD maxBitsperCw :27;
#else
    //Modified PHICH group number by ArrayComm 2010-12-15.
    BIT_FIELD phichGrpNumber :7;
    BIT_FIELD maxBitsperCw :25;
    //End Modified PHICH group number
#endif
    U32* pTxSdu;
} TXSDUREQ, *PTXSDUREQ;

typedef struct PhyMsTxSduReqDescriptor
{
    BIT_FIELD chanId :16;
    BIT_FIELD msgType :8;
    BIT_FIELD phyEntityId :8;
    BIT_FIELD nackAck :1;
    BIT_FIELD uciFormat :3;
    BIT_FIELD channelType :4;
    BIT_FIELD phichSeqIndex :7;
    BIT_FIELD cwId :1;
    BIT_FIELD msgLen :16;
    BIT_FIELD phichGrpNumber :5;
    BIT_FIELD maxBitsperCw :27;
    U32 CQIOffset;
    U32 RIOffset;
    U32 ACKOffset;
    U32* pTxSdu;
} TXMSSDUREQ, *PTXMSSDUREQ;

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
    BIT_FIELD channelId   :16;
    BIT_FIELD msgType     :8;
    BIT_FIELD phyEntityId :8;

    BIT_FIELD numBitsRx :24;
    BIT_FIELD status    :8;

    BIT_FIELD frameNum    :16;
    BIT_FIELD subFrameNum :8;
    BIT_FIELD chanType    :8;

    BIT_FIELD srdetected    :1;
    BIT_FIELD pucchDetected :1;
    BIT_FIELD pucchType     :4;
    BIT_FIELD cqiPmiConf    :2;
    BIT_FIELD ul_CQI        :8;
    BIT_FIELD timingAdv     :16;

    S32 mErrAvgCh[2];

    S16 mErrExpo;
    U16 reserved1;

    U32 pRxSdu;

} RXSDUIND, *PRXSDUIND;

typedef struct PhyRxStatusIndDescriptor
{
    BIT_FIELD msgSpecific :16;
    BIT_FIELD msgType :8;
    BIT_FIELD phyEntityId :8;
    BIT_FIELD frameNum :16;
    BIT_FIELD subFrameNum :8;
    BIT_FIELD eventNumber :8;
    BIT_FIELD statusType :8;
    BIT_FIELD reserved : 24;
    U32 pEventList;
} RXSTATUSIND, *PRXSTATUSIND;

#ifdef AC_MSPD_TDD
//Added structures for PHY_QUERY_IND by ArrayComm 2010-12-06
typedef struct PhyQueryIndDescriptor
{
    BIT_FIELD reserved :8;
    BIT_FIELD status :8;
    BIT_FIELD msgType :8;
    BIT_FIELD phyEntityId :8;
} PHYQUERYIND, *PPHYQUERYIND;
//End added PHY_QUERY_IND structure definition.

//Added structures for PHY_MEASURE_IND by ArrayComm 2010-12-06
typedef struct PhyMeasureIndDescriptor
{
    BIT_FIELD reserved :8;
    BIT_FIELD status :8;
    BIT_FIELD msgType :8;
    BIT_FIELD phyEntityId :8;
} PHYMEASUREIND, *PPHYMEASUREIND;
//End Added PHY_MEASURE_IND structure definition.
#endif

typedef struct PhyPrachStatusEventDescriptor
{
#ifndef AC_MSPD_TDD
    BIT_FIELD prachPreambleId :6;
    BIT_FIELD reserved :10;
    BIT_FIELD timingOffset :16;
#else
    //Modified PRACH report format by ArrayComm 2010-12-15
    //Added back prachConfigIdx according to requirement of Test MAC by ArrayComm 2011-1-19
    BIT_FIELD prachConfigIdx :6;
    BIT_FIELD Reserved :26;
    //end added back prachConfigIdx according to requirement of Test MAC
    U32 estimationValue;
    BIT_FIELD prachPreambleId :8;
    BIT_FIELD frequencyIdx :8;
    BIT_FIELD timingOffset :16;
    //End modified PRACH report format
#endif
} RX_PRACH_STATUS_EVENT, *PRX_PRACH_STATUS_EVENT;

typedef struct PhySrsStatusEventDescriptor
{
    U16  srsIndex;
    U16  reserved;
    U16  timingAdvance;
    U8   numberRb;
    U8   rbStart;
    U8  *psnr;

} RX_SRS_STATUS_EVENT, *PRX_SRS_STATUS_EVENT;

typedef struct PhyCqiRiHiIndDescriptor
{
    BIT_FIELD  harq_pt:2;
    BIT_FIELD  ri_pt:2;
    BIT_FIELD  cqi_pmi_pt:8;
    BIT_FIELD  status_len:4;
    BIT_FIELD  cqiPmiConf:2;
    BIT_FIELD  chanId:14;
} RX_CQIRIHI_STATUS_EVENT, *PRX_CQIRIHI_STATUS_EVENT;
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
#ifndef AC_MSPD_TDD
    BIT_FIELD enableDisable :8;
#else
    //Added SRS ID and antenna selection by ArrayComm 2010-12-15
    BIT_FIELD srsId :16;
    BIT_FIELD reserved :16;
    BIT_FIELD enableDisable :4;
    BIT_FIELD antSelEnable :4;
    //End added SRS ID and antenna selection
#endif
    BIT_FIELD srsBandwidth :2;
    BIT_FIELD srsHoppingBandwidth :2;
    BIT_FIELD transmissionComb :1;
    BIT_FIELD cyclicShift :3;
    BIT_FIELD freqDomainPosition :5;
    BIT_FIELD duration :1;
    BIT_FIELD srsConfigIndex :10;
} SRSDED, *PSRSDED;

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
    // For PRS support
    S16 prsRa;      // prs Signal Power Ratio relative to cell Specific reference signals power (db)
    U16 reserved;
    //U16 paprControl;
    //U16 paprThreshold;
} DLCMNTXPWRCTL, *PDLCMNTXPWRCTL;

typedef struct RachControlDescriptor
{
    BIT_FIELD prachEnable :1;
    BIT_FIELD rootSequenceIndex :10;
    BIT_FIELD prachConfigIndex :6;
    BIT_FIELD highSpeedFlag :1;
    BIT_FIELD zeroCorrelationZoneConfig :4;
    BIT_FIELD prachFreqOffset :10;
    BIT_FIELD prachPreambleNumber :6;
    BIT_FIELD pad:26;
} RACHCTRL, *PRACHCTRL;

typedef struct PuschConfigDedicated
{
    BIT_FIELD betaOffsetACKIndex :4;
    BIT_FIELD betaOffsetRIIndex :4;
    BIT_FIELD betaOffsetCQIIndex :4;
    BIT_FIELD nDMRS2 :4;
#ifndef AC_MSPD_TDD
    BIT_FIELD nsymi :4;
    BIT_FIELD nACK :4;
    BIT_FIELD nrbi :8;
    BIT_FIELD nRI :8;
    BIT_FIELD nr1CQI :8;
    BIT_FIELD nrg1CQI :8;
    BIT_FIELD reserved :8;
#else
    //Added TDD ACK/NACK report mode in PUSCH by ArrayComm 2010-12-15
    BIT_FIELD ackNackReportMode :1;
    BIT_FIELD nACK :3;
    BIT_FIELD Nbundled :8;
    //End added TDD ACK/NACK report mode in PUSCH

    //Added the number of control bit and initial symbol according to MSPD's API update by ArrayComm 2011-2-22
    BIT_FIELD nsymi :4;
    BIT_FIELD nrbi :8;
    BIT_FIELD nRI :8;
    BIT_FIELD nr1CQI :8;
    BIT_FIELD nrg1CQI :8;
    //End of added the number of control bit and initial symbol according to MSPD's API update

#endif
} PUSCHDED, *PPUSCHDED;

typedef struct CqiPmiRireportCtrl
{
    BIT_FIELD cqiReportModeAperiodic :3;
    BIT_FIELD nomPDSCHRSEPREOffset :3;
    BIT_FIELD cqiReportPeriodicEnable :2;
    BIT_FIELD cqiPUCCHResourceIndex :11;
    BIT_FIELD cqipmiConfigIndex :10;
    BIT_FIELD cqiFormatIndicatorPeriodic :1;
    BIT_FIELD K :2;
    BIT_FIELD riConfigIndex :10;
    BIT_FIELD simultaneousAckNackAndCQI :1;
    BIT_FIELD padding :21;
} CQIPMIRIRPT, *PCQIPMIRIRPT;


typedef struct PucchDedicatedCtrl
{

#ifndef AC_MSPD_TDD
    BIT_FIELD formatType :3;
    BIT_FIELD ackNackRepetition :1;
    BIT_FIELD simSRHarq:1;
    BIT_FIELD repetitionFactor :4;
    BIT_FIELD msPucchSrSel:1;
    BIT_FIELD n1PucchANRep :11;
    BIT_FIELD cqiPUCCHResourceIndex :11;
    BIT_FIELD srPUCCHResourceIndex :11;
    BIT_FIELD dlCqiPmiSizeBits :8;
    BIT_FIELD harqPucchIndex :11;
    BIT_FIELD harqSizebits :2;
    BIT_FIELD n1PUCCHResourceIndex:11;
    BIT_FIELD reserved:21;
#else
    //Added TDD ACK/NACK report mode in PUCCH by ArrayComm 2010-12-15
    BIT_FIELD ackNackReportMode :1;
    BIT_FIELD formatType :3;
    BIT_FIELD ackNackRepetition :1;
    BIT_FIELD simSRHarq:1;
    BIT_FIELD repetitionFactor :3;
    BIT_FIELD msPucchSrSel:1;
    BIT_FIELD n1PucchANRep :11;
    BIT_FIELD cqiPUCCHResourceIndex :11;

    BIT_FIELD srPUCCHResourceIndex :11;
    BIT_FIELD subframeMultiplexNum :10;
    BIT_FIELD ackPUCCHResourceIndex0 :11;

    BIT_FIELD ackPUCCHResourceIndex1 :11;
    BIT_FIELD ackPUCCHResourceIndex2 :11;
    BIT_FIELD padding0 :10;

    BIT_FIELD ackPUCCHResourceIndex3 :11;
    //Added the number of control bit and initial symbol according to MSPD's API update by ArrayComm 2011-2-22
    BIT_FIELD dlCqiPmiSizeBits :8;
    BIT_FIELD harqSizebits :2;
    BIT_FIELD padding1 :11;
    //End of added the number of control bit and initial symbol according to MSPD's API update
    //End Added TDD ACK/NACK report mode in PUCCH
#endif
} PUCCHDEDCTL, *PPUCCHDEDCTL;

typedef struct TimingAdvErrAvgInfo
{
    U32 mErrAvgCh[2];
    S16 mErrExpo;
    U16 mSetDefaults;
} TIMADVERRINF, *PTIMADVERRINF;

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
    BIT_FIELD pad :8; // To preserve 32 bits boundary for BIT_FIELDS
} DLSUBCHINFO, *PDLSUBCHINFO;

typedef struct DciChannelDescriptor
{
    U32 offsetNextCh;
    U16 channelId;
    S16 txpowerControl;
    BIT_FIELD crcLength :6;
    BIT_FIELD crcScrambling :2;
    BIT_FIELD channelType :5;
    BIT_FIELD reserved :3;
    BIT_FIELD numCodeWords :4;
    BIT_FIELD nLayers :4;
    BIT_FIELD transmissionMode :8;
    BIT_FIELD reserved1 :16;
    BIT_FIELD scrmblerInitValue :16;
    U16 numberofEntries;
    U8 startRes;
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
    CQIPMIRIRPT cqiPmiRiRpt;//64
    MAPPINGINFO mapInfo; //32
} ULSUBCHINFO, *PULSUBCHINFO;

typedef struct DlChannelDescriptor
{
    U32 offsetNextCh;
    U16 channelId;
    S16 txpowerControl;
    BIT_FIELD persistEnable :1;
    BIT_FIELD repeatCycle :7;
#ifndef AC_MSPD_TDD
    BIT_FIELD channelType :5;
    BIT_FIELD hCid :3;
#else
    //Modified HARQ process number for TDD by ArrayComm 2010-12-15
    BIT_FIELD channelType :4; //Change from 5 to 4 to 32 bit align
    BIT_FIELD hCid :4; //Change from 3 bits to 4 bits for hCid, to support max 15 HARQ process in TDD.
    //End Modified HARQ process number for TDD
#endif
    // New Support for VRB distributed
    BIT_FIELD numCodeWords:2;    //Either 1 or 2
    BIT_FIELD distributedFlag:1; // 0: Resource Allocation of localized type 1: Resource Allocation of Distributed Type
    BIT_FIELD nGapFlag:1;        // 0: Gap 1 1: Gap 2 // Only used if distributedFlag is 1 otherwise this bit is a don't care
    // nLayers refers to the number of Tx layers when part of the TXVECTOR
    // and to Rx layers when used in the RXVECTOR
    BIT_FIELD nLayers :4;
    BIT_FIELD transmissionMode :4;
    BIT_FIELD ueCategory :4;
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
    BIT_FIELD persistEnable :1;
    BIT_FIELD repeatCycle :7;
    BIT_FIELD channelType :8;
#ifndef AC_MSPD_TDD
    BIT_FIELD halfIterations :16;
#else
    //Updated to support UL hopping by ArrayComm 2011-1-14
    BIT_FIELD freqHoppingEnable :1;
    BIT_FIELD ulHoppingBits :2;
    BIT_FIELD padding :5;
    BIT_FIELD halfIterations :8;
    //end updated to support UL hopping
#endif
    ULSUBCHINFO ulSubChInfo;
    TIMADVERRINF timAdvErrInfo;//96
} ULCHANDESC, *PULCHANDESC;

typedef struct UlCtrlChannelDescriptor
{
    U32 offsetNextCh;
    U16 channelId;
    S16 txpowerControl;
    BIT_FIELD crcLength :8;
    BIT_FIELD channelType :8;
    BIT_FIELD reserved :16;
    BIT_FIELD scrmblerInitValue :16;
    BIT_FIELD codingDescriptor :3;
    BIT_FIELD blockCodeConcatenation :1;
    BIT_FIELD modulationType :4;
    BIT_FIELD mcsType :8;
    PUCCHDEDCTL pucchDedCtl;//64
    CQIPMIRIRPT cqiPmiRiRpt;//64
    U16 numberofEntries;
    U8 startRes;
    U8 numRes;
    TIMADVERRINF timAdvErrInfo;//96
} ULCTRLCHDESC, *PULCTRLCHDESC;

typedef struct ULSubframeCommonControl
{
#ifndef AC_MSPD_TDD
    BIT_FIELD deltaPUCCHShift :2;
    BIT_FIELD nRBCQI :7;
    BIT_FIELD nCSAn :3;
    BIT_FIELD n1PucchAN :11;
    BIT_FIELD pad :9;
    BIT_FIELD srsBandwitdhConfig :3;
    BIT_FIELD srsSubframeConfig :4;
    BIT_FIELD ackNackSRSSimultaneousTransmission :1;
    BIT_FIELD nSB :2;
    BIT_FIELD hoppingMode :1;
    BIT_FIELD puschhoppingOffset :7;
    BIT_FIELD enable64QAM :1;
    BIT_FIELD groupHoppingEnabled :1;
    BIT_FIELD groupAssignmentPUSCH :5;
    BIT_FIELD sequenceHoppingEnabled :1;
    BIT_FIELD cyclicShift :3;
    BIT_FIELD padding :3;
#else
    //Added srsMaxUpPts for TDD by ArrayComm 2010-12-15
    BIT_FIELD deltaPUCCHShift :2;
    BIT_FIELD nRBCQI :7;
    BIT_FIELD nCSAn :3;
    BIT_FIELD n1PucchAN :11;
    //  BIT_FIELD pad0:9;
    BIT_FIELD srsBandwitdhConfig :3;
    BIT_FIELD srsSubframeConfig :4;
    BIT_FIELD ackNackSRSSimultaneousTransmission :1;
    BIT_FIELD srsMaxUpPts :1;
    //    BIT_FIELD pad1:23:

    BIT_FIELD nSB :2;
    BIT_FIELD hoppingMode :1;
    BIT_FIELD puschhoppingOffset :7;
    BIT_FIELD enable64QAM :1;
    BIT_FIELD groupHoppingEnabled :1;
    BIT_FIELD groupAssignmentPUSCH :5;
    BIT_FIELD sequenceHoppingEnabled :1;
    BIT_FIELD cyclicShift :3;
    BIT_FIELD padding :11;
    //End Added srsMaxUpPts for TDD
#endif

} ULSUBFRCMNCTRL, *PULSUBFRCMNCTRL;

typedef struct DlSubframeDescriptor
{
    U16 frameNumber;
    U8 subframeNumber;
    U8 subframeType;
    // New support for PRS
    BIT_FIELD antennaPortcount:5;
    BIT_FIELD prsAntennaPortSel:2;  // Selects the antenna(s) where the Positioning Reference Signals are trasnmitted
    BIT_FIELD prsEnable:1;          // 0 no positioning reference signals are generated in this subframe, 1 prs is generated in this subframe
    BIT_FIELD numberofChannelDescriptors:8;
    BIT_FIELD prsNumRB:8;           // Bandwidth available for prs generation (Always smaller than system bandwidth)
    BIT_FIELD reserved0:8;
    // End of support for prs (Gain control in PowerCtrl field)
    U32 offsetPowerCtrl;
    U32 offsetDCIChannels;
    U32 offsetCustomFeatures;
    BIT_FIELD numCtrlSymbols :3;
    BIT_FIELD phichResource :2;
    BIT_FIELD phichDuration :3;
    BIT_FIELD numberOfPhichChannels :8;
    BIT_FIELD numCtrlChannels :16;
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
#ifndef AC_MSPD_TDD
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
    BIT_FIELD rxAntennaPortCount :8;
    BIT_FIELD phyCfg :10;
    BIT_FIELD reserved :5;
    BIT_FIELD rxSduLineMonitorInfoenable :1;
    BIT_FIELD txStartConfDisable :1;
    BIT_FIELD rxStartConfDisable :1;
    BIT_FIELD pb :2;
    BIT_FIELD customExtensionReportEn :1;
    BIT_FIELD rachReportMode :1;
    BIT_FIELD txSduConfEnable :1;
    BIT_FIELD sduConfig :1;
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
    S16 phichResource;
    S16 phichDuration;
#else
    //Added configuration parameters for TDD by ArrayComm 2010-12-15
    BIT_FIELD channelBandwidth :8;
    BIT_FIELD frtypeDuplexmode :1;
    BIT_FIELD ulDlConfig :3;
    BIT_FIELD specialSubframeConfig :4;
    BIT_FIELD radioAccessMode :8;
    BIT_FIELD physicalResourceBandwidth :8;

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

    BIT_FIELD rxAntennaPortCount :8;
    BIT_FIELD raResponseWindowSize :4;
    BIT_FIELD createHICH :1;
    BIT_FIELD rxSduLineMonitorInfoenable :1;
    BIT_FIELD padding :10;
    BIT_FIELD txStartConfDisable :1;
    BIT_FIELD rxStartConfDisable :1;
    BIT_FIELD pb :2;
    BIT_FIELD customExtensionReportEn :1;
    BIT_FIELD rachReportMode :1;
    BIT_FIELD txSduConfEnable :1;
    BIT_FIELD sduConfig :1;
    U16 radioFrameNumber;
    U8 subframeNumber;
    U8 slotNumber;
    S16 srsBandwidthConfig;
    S16 srsSubframeConfig;
    S16 srsSimultaneousANandSRS;
    S16 prachConfigurationIndex;
    S16 phichResource;
    S16 phichDuration;

    S16 srsMaxUpPts;
    //End Added configuration parameters for TDD

    //Added RNTP threshold for RNTP measurement by ArrayComm 2011-1-14
    S16 RNTP_threshold;
    //End added RNTP threshold for RNTP measurement
    S16 prachFrequencyOffset;
    S16 prachHighSpeedFlag;
    S16 prachCyclicShiftConfig;
    S16 prachRootSequenceIndex;
#endif
} INITPARM, *PINITPARM;

typedef struct stMac2PhyApiQueueElem
{
    U16 frameNumber;
    U8 MessageType;
    U8 subframeNumber;
    U32 MessageLen;
    struct stMac2PhyApiQueueElem* Next;
    U8* MessagePtr;
} MAC2PHY_QUEUE_EL, *PMAC2PHY_QUEUE_EL;

#endif
