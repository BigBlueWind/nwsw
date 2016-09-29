#ifndef __FECDRV_H__
#define __FECDRV_H__

#include <wtypes.h>
#include "tcb.h"
#include "basetypes.h"

//////////////////////////////////////////////////////////////////////////
// constants
#define BCTRL_LEN_MASK      0x0000FFFF
#define BCTRL_BLAST_MASK    0x00010000

#define FEC_IDMA_BCONTROL_BLAST 0x00010000
#define FEC_IDMA_BCONTROL_BLEN  0x0000FFFF
#define FEC_BDESC_MAX_NUM       14
#define FEC_CI_RMUX_MAX12       1536
#define FEC_CI_RMUX_MAX14       1312
#define FEC_STR_MAX             260
#define FEC_PARAMS_MAX_NUM      100
#define FEC_FRAMES_MAX_NUM      16

#define CRC24A					1
#define CRC24B					2

#define BPSK					0
#define QPSK					1
#define QAM16					2
#define QAM64					3

#define FEC_CC					0
#define FEC_CTC					1

#define FEC_LTE					0
#define FEC_WIMAX				1

// DL hardware context
typedef struct tDLHWCTX {
	// W1
	unsigned IRQEN:1; // 0
	unsigned W1_Unused1:3; // 3:1
	unsigned BOffset:8; // 11:4
	unsigned W1_Unused2:3; // 14:12
	unsigned E_Param:17; // 31:15
	// W2
	unsigned CTC_CCn:1; // 0
	unsigned CRC_Sel:3; // 3:1
	unsigned LTERateMaching_K0:14; // 17:4
	unsigned BlockSize:13; // 30:18
	unsigned Reserved:1;           // 31
	// W3
	unsigned spid:2; // 1:0
	unsigned Modulation:2; // 3:2
	unsigned Ncb_14_13:2;         // 5:4
	unsigned Bypass_Encoder:1; // 6
	unsigned Bypass_Interleaver:1; // 7
	unsigned Rate_Index:3; // 10:8
	unsigned Harq_spid:2;          // 12:11, LTE: Scramble_mask
	unsigned Harq_rep_end:1;       // 13,    LTE: Scramble_en
	unsigned Harq_req:1;           // 14,    LTE: BypassRateMatching
	unsigned Harq_en:1;            // 15
	unsigned Reserved_1:1;         // 16
	unsigned Multi_FEC_blk:1;      // 17
	unsigned Filler:13;            // 30:18
	unsigned Reserved_2:1;         // 31
} DLHWCTX, *PDLHWCTX;

// UL hardware context
typedef struct tULHWCTX {
	// W1
	unsigned IRQEN:1; // 0
	unsigned W1_Unused1:3; // 3:1
	unsigned BOffset:8; // 11:4
	unsigned W1_Unused2:3; // 14:12
	unsigned E_Param:17; // 31:15
	// W2
	unsigned CTC_CCn:1; // 0
	unsigned CRC_Sel:3; // 3:1
	unsigned LTERateMaching_K0:14; // 17:4
	unsigned BlockSize:13; // 30:18
	unsigned Reserved:1; // 31
	// W3
	unsigned Modulation:2; // 1:0
	unsigned HalfInteration:6; // 7:2
	unsigned Rate_Index:3; // 10:8
	unsigned Reserved_1:3; // 13:11
	unsigned Bypass_Rate_Match:1; // 14
	unsigned Dynamic_Stop_Threshold:2; // 16:15
	unsigned Dynamic_Stop_Iteration:1; // 17
	unsigned Filler:13; // 30:18
	unsigned Reserved_2:1; // 31
} ULHWCTX, *PULHWCTX;

// control registers
typedef struct tReg {
// config0
    unsigned wimax_lte:1;               // [0]
    unsigned reserved_0:15;             // [15:1]
    unsigned loopback_en_0:1;           // [16]
    unsigned loopback_en_1:1;           // [17]
    unsigned bp_channel_interleaver:1;  // [18] LTE UL
    unsigned bp_rate_match:1;           // [19] LTE UL
    unsigned bp_block_interleaver:1;    // [20] WiMAX UL
    unsigned bp_subblock_interleaver:1; // [21] LTE UL CC
    unsigned bp_harq_soft_combine:1;    // [22] UL
    unsigned bp_decoder:1;              // [23]
    unsigned ci_symbol_number:1;        // [24] LTE UL
    unsigned convergence_length:3;      // [27:25] LTE UL CC
    unsigned reserved_2:1;              // [28]
    unsigned send_zerolen_desc:1;       // [29]
    unsigned reserved_3:2;              // [31:30]
// config1
    unsigned scrambler_mask_sel:1;      // [0]
    unsigned reserved_4:3;              // [3:1]
    unsigned scrambler_mask_b:12;       // [15:4]
    unsigned reserved_5:4;              // [19:16]
    unsigned scrambler_mask_a:12;       // [31:20]
// config2
    unsigned dyn_stop_thresh_0:8;       // [7:0]
    unsigned dyn_stop_thresh_1:8;       // [15:8]
    unsigned dyn_stop_thresh_2:8;       // [23:16]
    unsigned dyn_stop_thresh_3:8;       // [31:24]
// config3
    unsigned config3:32;                // [31:0]
// status0
    unsigned tc1000_monitor:8;          // [7:0]
    unsigned reserved_6:8;              // [15:8]
    unsigned tc1000_idle:1;             // [16]
    unsigned tc1000_error:2;            // [18:17]
    unsigned tc1000_warning:3;          // [21:19]
    unsigned tc1000_status:3;           // [25:22]
    unsigned reserved_7:7;              // [31:26]
// status1
    unsigned tc1700_monitor:8;          // [7:0]
    unsigned reserved_8:8;              // [15:8]
    unsigned tc1700_idle:1;             // [16]
    unsigned tc1700_crc_error:1;        // [17]
    unsigned reserved_9:1;              // [18]
    unsigned tc1700_ber:13;             // [31:19]
// inbound burst length
    unsigned inbound_dma_burst_len;
// outbound burst length
    unsigned outbound_dma_burst_len;
} Reg, *PReg;

//////////////////////////////////////////////////////////////////////////
typedef struct _FEC_IO_CTX {
    U16  OperationMode;
    U16  CodingType;
    U16  BlockSize;
    U16  CrcSelector;
    U16  FillerBits;
    U16  Modulation;
    U16  CodingRate;
    U16  HalfIterations;
    U16  DynamicStopIteration;
    U16  DynamicStopThreshold;
    U16  ScrambleEnable;
    U16  ScrambleMask;
    U16  SubpacketId;
    U16  BpBlockInterleaving;
    U16  BpEncoder;
    U16  BpDecoder;
    U16  EnableCombine;
    U16  BpRateMatching;
    U16  RateMatchingE;
    U16  RateMatchingk0;
    U16  RateMatchingNcb;
    U16  BpChannelInterleaving;
    U16  CiSymbolNumber;
    U16  CiInsertions[14];
} FEC_IO_CTX, *PFEC_IO_CTX;

// buffer descriptor
typedef struct tBDesc {
    PCHAR       BPtr;
    unsigned    BControl;
} BDesc, *PBDesc;

// frame descriptor
typedef struct tFDesc {
/* 4 */     struct tFDesc*      Next;
/* 8 */     unsigned			FControl;
/* 12 */    unsigned			FStatus0;
/* 16 */    unsigned			FStatus1;
/* 20 */    BDesc				TxBDesc; // Outbound BDesc
/* 28 */    BDesc				RxBDesc[FEC_BDESC_MAX_NUM]; // Inbound BDescs
} FDesc, *PFDesc;

#ifdef __cplusplus
extern "C" {
#endif

	// init driver
	BOOL FECInit(PReg pRegDrv);

	// Finalize driver
	void FECFinit();

	// process TCB
	BOOL FECExec(PTCB pTcb);

#ifdef __cplusplus
}
#endif

#endif __FECDRV_H__