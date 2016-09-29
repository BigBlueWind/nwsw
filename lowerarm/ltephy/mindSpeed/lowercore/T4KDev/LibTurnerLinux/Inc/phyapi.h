/** @file phyapi.h
 *
 * @brief Intel PHY SAP API definitions and interfaces
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef PHYAPI_H_
#define PHYAPI_H_

#include "resultcodes.h"

//
// PHY SAP API version
//
#define PHY_SAP_API             231

//
// FEC Code Type Field Coding
//
#define FEC_QPSK_CC_12          0
#define FEC_QPSK_CC_34          1
#define FEC_16QAM_CC_12         2
#define FEC_16QAM_CC_34         3
#define FEC_64QAM_CC_12         4
#define FEC_64QAM_CC_23         5
#define FEC_64QAM_CC_34         6
#define FEC_QPSK_BTC_12         7
#define FEC_QPSK_BTC_34         8
#define FEC_16QAM_BTC_35        9
#define FEC_16QAM_BTC_45        10
#define FEC_64QAM_BTC_58        11
#define FEC_64QAM_BTC_45        12
#define FEC_QPSK_CTC_12         13
#define FEC_RESERVED            14
#define FEC_QPSK_CTC_34         15
#define FEC_16QAM_CTC_12        16
#define FEC_16QAM_CTC_34        17
#define FEC_64QAM_CTC_12        18
#define FEC_64QAM_CTC_23        19
#define FEC_64QAM_CTC_34        20
#define FEC_64QAM_CTC_56        21
#define FEC_QPSK_ZTCC_12        22
#define FEC_QPSK_ZTCC_34        23
#define FEC_16QAM_ZTCC_12       24
#define FEC_16QAM_ZTCC_34       25
#define FEC_64QAM_ZTCC_12       26
#define FEC_64QAM_ZTCC_23       27
#define FEC_64QAM_ZTCC_34       28
#define FEC_QPSK_LDPC_12        29
#define FEC_QPSK_LDPC_23A       30
#define FEC_QPSK_LDPC_34A       31
#define FEC_16QAM_LDPC_12       32
#define FEC_16QAM_LDPC_23A      33
#define FEC_16QAM_LDPC_34A      34
#define FEC_64QAM_LDPC_12       35
#define FEC_64QAM_LDPC_23A      36
#define FEC_64QAM_LDPC_34A      37
#define FEC_QPSK_LDPC_23B       38
#define FEC_QPSK_LDPC_34B       39
#define FEC_16QAM_LDPC_23B      40
#define FEC_16QAM_LDPC_34B      41
#define FEC_64QAM_LDPC_23B      42
#define FEC_64QAM_LDPC_34B      43
#define FEC_QPSK_CCIL_12        44
#define FEC_QPSK_CCIL_34        45
#define FEC_16QAM_CCIL_12       46
#define FEC_16QAM_CCIL_34       47
#define FEC_64QAM_CCIL_23       48
#define FEC_64QAM_CCIL_34       49
#define FEC_QPSK_LDPC_56        50
#define FEC_16QAM_LDPC_56       51
#define FEC_64QAM_LDPC_56       52

//
// Message Type Field Coding
//
#define PHY_RESERVED0           0
#define PHY_TXSTART_REQ         1
#define PHY_TXSTART_ACK         2
#define PHY_TXSTART_IND         3
#define PHY_TXSDU_REQ           4
#define PHY_TXSDU_ACK           5
#define PHY_TXEND_IND           6
#define PHY_RXSTART_REQ         7
#define PHY_RXSTART_ACK         8
#define PHY_RXSTART_IND         9
#define PHY_RXSDU_IND           10
#define PHY_RXEND_IND           11
#define PHY_RESERVED1           12
#define PHY_RESERVED2           13
#define PHY_RESERVED3           14
#define PHY_RXCDMA_IND          15

#define PHY_INIT_IND            19

#define PHY_CONTROL             128     // Below is data messages

#define PHY_INIT_REQ            255

//
// Error Code Field Coding
//
#define ERR_SUCCESS             0
#define ERR_INVALID             1       // Primitive is not supported (for requests)
#define ERR_RESTART             1       // Restart flag (for TXSTART_IND)
#define ERR_FEC_INVALID         2       // FEC code type is not supported
#define ERR_OVERRUN             3
#define ERR_UNDERRUN            4
#define ERR_TRANSPORT           5       // Transport Media Error
#define ERR_TXDATASIZE          6       // TX data size do not match TXVECTOR
#define ERR_VECTOR              7       // Invalid RX/TX VECTOR format

//
// Indication type
//
#define IND_DATABURST           0
#define IND_HARQACK             1
#define IND_FASTFEEDBACK        2
#define IND_HARQDATABURST       3
#define IND_MIMODATABURST       4
#define IND_MIMOHARQBURST       5

//
// Zone permutation
//
#define ZONE_PUSC               0
#define ZONE_OPT_PUSC           1
#define ZONE_AMC16              2
#define ZONE_AMC23              3
#define ZONE_AMC32              4

//
// Message segmentation bits
//
#define SEG_MIDDLE              0
#define SEG_LAST                1
#define SEG_FIRST               2
#define SEG_ENTIRE              3       // No segmentation

//
// PHY entity ID (sector, antenna, carrier etc) encoded in segmentation field
//
#define PhyEntityID(x)          ((x)>>2)

//
// Message segmentation bits encoded in segmentation field
//
#define PhySegmentation(x)      ((x)&3)

//
// Downlink subframe parameter structure
//
typedef struct DlSubframe
{
    U8 subFrameType;
    U8 frameNumber;
    U8 reserved1;
    U8 versionNumber;
    U32 reserved2;
} DL_SUB_FRAME;

//
// Generic Part of Downlink Zone Parameter Structure
//
typedef struct DlZone
{
    U8 zoneType;
    U8 zoneNumber;
    U8 startSymbolOffset;
    U8 endSymbolOffset;
    U8 permutationType;
    U8 subchannelsIndicator;
    U8 dlPermBase;
    U8 prbsID;
    U8 dedicatedPilots;
    U8 padding[3];              // 32-bit boundary
} DL_ZONE;

//
// STC Zone -Specific Part of Downlink Zone Parameter Structure
//
typedef struct DlStcZone
{
    U8 stcType;
    U8 matrixIndicator;
    U8 midiamblePresence;
    U8 midiambleBoosting;
} DL_STC_ZONE;

//
// AAS Zone -Specific Part of Downlink Zone Parameter Structure
//
typedef struct DlAasZone
{
    U8 preambleConfig;
    U8 sdmaSupported;
    U16 padding;                // 32-bit boundary
} DL_AAS_ZONE;

//
// Generic Part of Downlink Burst Parameter Structure
//
typedef struct DlBurst
{
    U8 burstType;
    U8 burstTypeExt;
    U8 burstNumber;
    U8 fecCodeType;
    U32 burstDataLength;
    U8 ofdmaSymbolOffset;
    U8 subchannelOffset;
    U8 boosting;
    U8 repetitionCoding;
} DL_BURST;

//
// Map Data Burst-Specific Part of Downlink Burst Parameter Structure
//
typedef struct DlMapBurst
{
    U16 numberSlots;
    U16 reserved;               // 32-bit boundary
} DL_MAP_BURST;

//
// Normal Data Burst-Specific Part of Downlink Burst Parameter Structure
//
typedef struct DlNormalBurst
{
    U8 numberSymbols;
    U8 numberSubchannels;
    U16 aasHandle;
} DL_NORMAL_BURST;

//
// PAPR Allocation-Specific Part of Downlink Burst Parameter Structure
//
typedef struct PaprAllocation
{
    U8 numberSymbols;
    U8 numberSubchannels;
    U16 reserved;               // 32-bit boundary
} PAPR_ALLOCATION;

//
// AAS-Specific Part of Downlink Optional Burst Parameters
//
typedef struct DlAasBurst
{
    U8 preambleModifierType;
    U8 preambleShiftIndex;
    U16 reserved;               // 32-bit boundary
} DL_AAS_BURST;

//
// MIMO-Specific Part of Downlink Optional Burst Parameters
//
typedef struct DlMimoBurst
{
    U8 matrixIndicator;
    U8 layerIndex;
    U16 reserved;               // 32-bit boundary
} DL_MIMO_BURST;

//
// Common Part of Downlink Sub-Burst Parameter Structure
//
typedef struct DlSubBurst
{
    U8 subBurstType;
    U8 subBurstNumber;
    U8 symbolOffset;
    U8 subchannelOffset;
    U16 numberSlots;
    U16 reserved1;
    U8 reserved2;
    U8 fecCodeType;
    S16 issid;
    U16 aasHandle;
    U8 boosting;
    U8 repetitionCoding;
    U32 subBurstDataLength;
} DL_SUB_BURST;

//
// HARQ Chase Combining-Specific Part of Downlink Optional Sub-Burst Parameters
//
typedef struct DlSubHarq
{
    U8 harqChannelID;
    U8 harqSeqNumber;
    U8 flushRequest;
    U8 padding;                 // 32-bit boundary
} DL_SUB_HARQ;

//
// MIMO Chase Combining-Specific Part of Downlink Optional Sub-Burst Parameters
//
typedef struct DlSubMimo
{
    U8 harqChannelID;
    U8 harqSeqNumber;
    U8 flushRequest;
    U8 layerIndex;
} DL_SUB_MIMO;

//
// Uplink Subframe Parameter Structure Format
//
typedef struct UlSubFrame
{
    U8 subFrameType;
    U8 frameNumber;
    U8 reserved;
    U8 versionNumber;
    U32 allocStartTime;
} UL_SUB_FRAME;

//
// Generic Part of Uplink Zone Parameter Structure
//
typedef struct UlZone
{
    U8 zoneType;
    U8 zoneNumber;
    U8 startSymbolOffset;
    U8 endSymbolOffset;
    U8 permutationType;
    U8 subChannelIndCfg;        // 4 bits: Use all subchannels indicator
                                // 4 bits: Disable PUSC subchannel rotation
    U8 ulPermBase;
    U8 rxAgcRangeExt;
} UL_ZONE;

//
// AAS Zone -Specific Part of Uplink Zone Parameter Structure
//
typedef struct UlAasZone
{
    U8 preambleConfig;
    U8 preambleType;
    U8 sdmaSupported;
    U8 padding;                 // 32-bit boundary
} UL_AAS_ZONE;

//
// Generic Part of Uplink Burst Parameter Structure
//
typedef struct UlBurst
{
    U8 burstType;
    U8 burstTypeExt;
    U8 burstNumber;
    U8 fecCodeType;
    U32 burstDataLength;
    U8 ofdmaSymbolOffset;
    U8 subchannelOffset;
    U8 reserved;
    U8 repetitionCoding;
    U16 issid;
    U16 aasHandle;
} UL_BURST;

//
// HARQ ACK Channel Specific Part of Uplink Burst Parameter Structure
//
typedef struct UlHarqBurst
{
    U8 numberSymbols;
    U8 numberSubChannels;
    U16 reserved;               // 32-bit boundary
} UL_HARQ_BURST;

//
// Fast Feedback Channel Specific Part of Uplink Burst Parameter Structure
//
typedef struct UlFastFeedback
{
    U8 numberSymbols;
    U8 numberSubchannels;
    U16 reserved;               // 32-bit boundary
} UL_FAST_FEEDBACK;

//
// Initial Ranging/Handover Ranging Allocation Specific Part of Uplink Burst Parameter Structure
//
typedef struct UlInitialRanging
{
    U8 numberSymbols;
    U8 numberSubChannels;
    U8 rangingMethod;
    U8 reserved1;
    U16 zoneXid;
    U16 reserved2;              // 32-bit boundary
} UL_INITIAL_RANGING;

//
// Periodic Ranging/Bandwidth Request Allocation Specific Part of Uplink Burst Parameter Structure
//
typedef struct UlPeriodicRanging
{
    U8 numberSymbols;
    U8 numberSubChannels;
    U8 rangingMethod;
    U8 reserved1;
    U16 zoneXid;
    U16 reserved2;              // 32-bit boundary
} UL_PERIODIC_RANGING;

//
// PAPR/Safety Zone Channel Specific Part of Uplink Burst Parameter Structure
//
typedef struct UlSafetyZone
{
    U8 numberSymbols;
    U8 numberSubChannels;
    U8 config;                  // 0 – UL PAPR reduction, 1 – UL Safety zone
    U8 reserved;                // 32-bit boundary
} UL_SAFETY_ZONE;

//
// Sounding Zone Allocation Specific Part of Uplink Burst Parameter Structure
//
typedef struct UlSoundingZone
{
    U8 numberSymbols;
    U8 numberSubChannels;
    U8 soundingType;
    U8 separabilityType;
    U8 maxCyclicShiftIndex;
#define decimationValue         macCyclicShiftIndex
    U8 decimationOffcet;
    U16 padding;                // 32-bit boundary
} UL_SOUNDING_ZONE;

//
// Noise Floor Calculation Allocation Specific Part of Uplink Burst Parameter Structure
//
typedef struct UlNoiseFloor
{
    U8 numberSymbols;
    U8 numberSubChannels;
    U16 padding;                // 32-bit boundary
} UL_NOISE_FLOOR;

//
// Normal Data Burst Specific Part of Uplink Burst Parameter Structure
//
typedef struct UlNormalBurst
{
    U16 numberSlots;
    U16 padding;                // 32-bit boundary
} UL_NORMAL_BURST;

//
// AAS-specific part of Uplink Optional Burst Parameters
//
typedef struct UlAasBurst
{
    U8 preambleModifierType;
    U8 reserved1;               // Doc says it is U16 but I split it
    U8 reserved2;
    U8 preambleShiftIndex;
} UL_AAS_BURST;

//
// MIMO-specific part of Uplink Optional Burst Parameters
//
typedef struct UlMimoBurst
{
    U8 matrixIndicator;
    U8 pilotPatterns;
    U8 control;                 // 4 bits - Collaborative MIMO control
                                // 4 bits - Single/Dual TX antenna SS
    U8 layerIndex;
} UL_MIMO_BURST;

//
// Common Part of Uplink Sub-Burst Parameter Structure
//
typedef struct UlSubBurst
{
    U8 subBurstType;
    U8 subBurstNumber;
    S16 issid;
    U16 aasHandle;
    U16 padding;                // 32-bit boundary
} UL_SUB_BURST;

//
// Mini-Subchannel Allocation-Specific Part of Uplink Sub-Burst Parameter Structure
//
typedef struct UlMiniSub
{
    U8 ctype;
    U8 allocationIndex;
    U16 padding;                // 32-bit boundary
} UL_MINI_SUB;

//
// Fast Feedback Allocation-Specific Part of Uplink Sub-Burst Parameter Structure
//
typedef struct UlFastFeedbackSub
{
    U32 feedbackType : 8;
    U32 reserved1 : 1;
    U32 feedbackSubType : 3;
    U32 cqichID : 12;
    U32 reserved2 : 2;
    U32 slotOffset : 6;
} UL_FAST_FEEDBACK_SUB;

//
// HARQ ACK Subchannel Allocation-Specific Part of Uplink Sub-Burst Parameter Structure
//
typedef struct UlHarqAck
{
    U32 acid : 4;
    U32 reserved : 28;
} UL_HARQ_ACK;

//
// Sounding Signal - Specific Part of Uplink Sub-Burst Parameter Structure
//
typedef struct UlSounding
{
    U8 symbolIndex;
    U8 assignmentMethod;
    U8 powerBoost;
    U8 allocationMode;
    union
    {
        // If AllocationMode == 0
        struct
        {
            U8 startFreqBand;
            U8 numberFreqBands;
        };
        // If AllocationMode == 1
        U16 bandBitmap;
    };
    union
    {
        // If Separability == 0
        U8 cyclicShiftIndex;
        // If Separability == 1
        U8 decimationOffset;
    };
    U8 reserved;
} UL_SOUNDING;

//
// Sub-Allocation - Specific Part of Uplink Sub-Burst Parameter Structure
//
typedef struct UlSubAlloc
{
    U8 symbolOffset;
    U8 subChannelOffset;
    U16 numberSlots;
    U32 dataLength;
    U8 repetitionCoding;
    U8 fecCodeType;
    U16 reserved;               // 32-bit boundary
} UL_SUB_ALLOC;

//
// HARQ Chase Combining-Specific Part of Uplink Optional Sub-Burst Parameters
//
typedef struct UlHarqSubBurst
{
    U8 channelID;
    U8 seqNumber;
    U8 flushRequest;
    U8 reserved;                // 32-bit boundary
} UL_HARQ_SUB_BURST;

//
// MIMO Chase Combining-Specific Part of Uplink Optional Sub-Burst Parameters
//
typedef struct UlMimoSubBurst
{
    U8 channelID;
    U8 seqNumber;
    U8 flushRequest;
    U8 matrix;
} UL_MIMO_SUB_BURST;

//
// PHY SAP Primitives
//
typedef struct PhyTxstartRequest
{
    U32 length : 16;
    U32 messageType : 8;
    U32 segmentation : 8;
} PHY_TXSTART_REQUEST;

typedef struct PhyTxstartConfirmation
{
    U32 nextFrameNumber : 4;
    U32 reserved : 4;
    U32 status : 8;
    U32 messageType : 8;
    U32 segmentation : 8;
} PHY_TXSTART_CONFIRMATION;

typedef struct PhyTxstartIndication
{
    U32 initFrameNumber : 24;
    U32 reserved : 8;
    U32 currFrameNumber : 4;
    U32 extFrameNumber : 4;
    U32 status : 8;
    U32 messageType : 8;
    U32 segmentation : 8;
} PHY_TXSTART_INDICATION;

typedef struct PhyTxsduRequest
{
    U32 burstNumber : 10;
    U32 splitPoint : 3;
    U32 zoneNumber : 3;
    U32 messageType : 8;
    U32 segmentation : 8;
} PHY_TXSDU_REQUEST;

typedef struct PhyTxsduConfirmation
{
    U32 burstNumber : 10;
    U32 splitPoint : 3;
    U32 zoneNumber : 3;
    U32 messageType : 8;
    U32 segmentation : 8;

    U32 nextFrameNumber : 4;
    U32 reserved2 : 4;
    U32 status : 8;
    U32 reserved1 : 16;
} PHY_TXSDU_CONFIRMATION;

typedef struct PhyTxendIndication
{
    U32 reserved : 16;
    U32 numberFrames : 4;
    U32 aasZoneDeadline : 4;
    U32 aasZoneSize : 8;

    U32 currFrameNumber : 4;
    U32 phyRequest : 4;
    U32 status : 8;
    U32 messageType : 8;
    U32 segmentation : 8;
} PHY_TXEND_INDICATION;

typedef struct PhyRxstartRequest
{
    U32 length : 16;
    U32 messageType : 8;
    U32 segmentation : 8;
} PHY_RXSTART_REQUEST;

typedef struct PhyRxstartConfirmation
{
    U32 nextFrameNumber : 4;
    U32 reserved : 4;
    U32 status : 8;
    U32 messageType : 8;
    U32 segmentation : 8;
} PHY_RXSTART_CONFIRMATION;

typedef struct PhyRxstartIndication
{
    U32 currFrameNumber : 4;
    U32 reserved : 4;
    U32 status : 8;
    U32 messageType : 8;
    U32 segmentation : 8;
} PHY_RXSTART_INDICATION;

typedef struct PhyRxsduIndication
{
    U32 issid : 16;
    U32 messageType : 8;
    U32 segmentation : 8;
    
    U32 bytesReceived : 31;
    U32 integrity : 1;
    
    U32 powerOffset : 8;
    U32 reserved : 8;
    U32 cinr : 8;
    U32 rssiLevel : 8;
    
    U32 aasHandle : 16;
    U32 aasHandleUpdate : 1;
    U32 permutationType : 3;
    U32 indicationType : 4;
    U32 acid : 4;
    U32 currFrameNumber : 4;
    
    U32 timeDeviation;
    
    U32 freqDeviation;
} PHY_RXSDU_INDICATION;

typedef struct HarqAckData
{
    U32 reserved2 : 8;
    U32 ackNak : 1;
    U32 ackValid : 1;
    U32 reserved1 : 2;
    U32 acid : 4;
    U32 issid : 16;
} HARQ_ACK_DATA;

typedef struct FastFeedbackData
{
    U32 cqichID : 16;
    U32 issid : 16;

    U32 value : 8;
    U32 reserved : 12;
    U32 feedbackSubType : 3;
    U32 feedbackValid : 1;
    U32 feedbackType : 8;
} FAST_FEEDBACK_DATA;

typedef struct PhyRxendIndication
{
    U32 currFrameNumber : 4;
    U32 aasReportPresent : 4;
    U32 status : 8;
    U32 messageType : 8;
    U32 segmentation : 8;

    U32 issid : 16;
    U32 reserved : 8;
    U32 affectedSS : 8;
} PHY_RXEND_INDICATION;

typedef struct PhyRxcdmaIndication
{
    U32 zoneXid : 16;
    U32 messageType : 8;
    U32 segmentation : 8;
    
    U32 cdmaSubChannel : 8;
    U32 reserved1 : 8;
    U32 cdmaSymbol : 8;
    U32 cdmaCode : 8;
    
    U32 powerOffset : 8;
    U32 reserved2 : 8;
    U32 cinr : 8;
    U32 rssiLevel : 8;
    
    U32 aasHandle : 16;
    U32 reserved3 : 12;
    U32 currFrameNumber : 4;
    
    U32 timeDeviation;
    U32 freqDeviation;
} PHY_RXCDMA_INDICATION;

typedef struct PhyInitIndication
{
    U32 reserved : 8;
    U32 status : 8;
    U32 messageType : 8;
    U32 segmentation : 8;
} PHY_INIT_INDICATION;

//
// Functions
//

// This is for debugging purposes
RESULTCODE LarmDispatch(U32 size, PTR data);

RESULTCODE PhySapDecode(PTR data);

RESULTCODE PhyTxStart(PHY_TXSTART_REQUEST *data);
RESULTCODE PhyTxStartAck(PHY_TXSTART_CONFIRMATION *data);
RESULTCODE PhyTxStartInd(PHY_TXSTART_INDICATION *data);
RESULTCODE PhyTxSdu(PHY_TXSDU_REQUEST *data);
RESULTCODE PhyTxSduAck(PHY_TXSDU_CONFIRMATION *data);
RESULTCODE PhyTxEndInd(PHY_TXEND_INDICATION *data);

RESULTCODE PhyRxStart(PHY_RXSTART_REQUEST *data);
RESULTCODE PhyRxStartAck(PHY_RXSTART_CONFIRMATION *data);
RESULTCODE PhyRxStartInd(PHY_RXSTART_INDICATION *data);
RESULTCODE PhyRxSduInd(PHY_RXSDU_INDICATION *data);
RESULTCODE PhyRxEndInd(PHY_RXEND_INDICATION *data);

RESULTCODE PhyHarqAck(PHY_RXSDU_INDICATION *data, HARQ_ACK_DATA *dataPtr);
RESULTCODE PhyFastFeedback(PHY_RXSDU_INDICATION *data, FAST_FEEDBACK_DATA *dataPtr);

RESULTCODE PhyRxCdmaInd(PHY_RXCDMA_INDICATION *data);

RESULTCODE PhySdu(PHY_RXSDU_INDICATION *data, PHY_RXSDU_INDICATION *dataPtr);

#endif /* PHYAPI_H_ */
