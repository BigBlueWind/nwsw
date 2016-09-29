//-------------------------------------------------------------------------------------------
/** @file ltephy.h
 *
 * @brief This file defines the structures, constants and variables
 *        used in the implementation of an LTE PHY
 * @author Mindspeed Technologies
 * @version $Revision: 1.224 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef LTEPHY_H

#define LTEPHY_H
#include "LtePhyL2Api.h"
#include "ltephydef.h"
#include "tcb.h"

#if !defined WIN32 && !defined CEVA_INTRINSICS
#include "mdmadrv.h"
#include "fpdrv.h"
#endif

#ifndef CEVA_INTRINSICS
#if defined (_WIN32)
#include "tcbscheduler.h"
#else
#include "rex.h"
#include "mlog.h"
#endif
#else
#define PTASKLIST U32
#define SCHEDSTRC U32
#endif

#define MOD_QPSK  1
#define MOD_16QAM 2
#define MOD_64QAM 3

/* Define for maximum number of antenna port
 * NOTE: We currently do not differentiate Number of
 * antenna ports and the physical number of antennas */
#define MAX_NUM_ANT_PORT                    ( MAX_NUM_ANT )

/* Defines for Resource Element Group (REG)  */
#define NUM_REG_FOR_SYMBOL_0                ( 2 )
#define NUM_SC_PER_REG_SYMBOL0              ( 6 )
#define NUM_REG_FOR_SYMBOL_1_TWO_ANT_PORT   ( 3 )
#define NUM_SC_PER_REG_SYM1_TWO_ANT_PORT    ( 4 )
#define NUM_REG_FOR_SYMBOL_1_FOUR_ANT_PORT  ( 2 )
#define NUM_SC_PER_REG_SYM1_FOUR_ANT_PORT   ( 6 )
#define NUM_REG_FOR_SYMBOL_2                ( 3 )
#define NUM_SC_PER_REG_SYMBOL2              ( 4 )
#define MAX_REG_PER_SYMBOL                  ( 3 )

#define PBCHDATASIZE 480

/* BCCH Payload in Bytes */
#define BCCH_PAYLOAD_IN_BYTES   ( 3 )

/* Physical Broadcast Channel Size in bits (for Normal CP) */
#define PBCH_LENGTH_IN_BITS     (1920)

/* Physical Broadcast Channel Size in bits (for Extended CP) */
//#define PBCH_LENGTH_IN_BITS_EXTENDED_CP (1728)

/* PBCH size in bytes */
#define PBCH_LEN_IN_BYTES       (PBCH_LENGTH_IN_BITS>>3)

/* Buffer size for PBCH (at initialization)
 * Size of the buffer = 1920*256 bits
 * 1920 bits = size of PBCH bits
 * 256 = all combinations of frame numbers
 * saved as U8 */
#define PBCH_INIT_MEM_BUFFER_SIZE           ( PBCH_LEN_IN_BYTES*256 )

/* Defines related to Control Format Indicator (PCFICH) channel */
#define MAX_DL_CONTROL_SYM                  ( 3 )
#define NUMBER_OF_CFI_BITS                  ( 32 )
#define NUM_BITS_PER_BYTE                   ( 8 )
#define NUM_REG_ALLOC_FOR_CFI               ( NUMBER_OF_CFI_BITS>>3 )
/* Buffer size to save PCFICH symbol buffer generated at Initialization
 * 3 (for 3 control symbols) * 32 (Number of CFI Bits ) * 20 (for 20 slot)
 * NOTE: we need 16 symbols * 10 sub-frames * MAX_NUM_ANT_PORTS so
 * we use NUM_SLOTS * NUM_PCFICH_SYMBOLS
 * data-type S16 */
#define PCFICH_INIT_MEM_BUFFER_SIZE         ( MAX_DL_CONTROL_SYM * NUMBER_OF_CFI_BITS*NUM_SLOTS )
/* Memory size for PCFICH Resource element buffer */
#define PCFICH_INIT_RE_BUFFER_SIZE          ( NUMBER_OF_CFI_BITS/2 )

/* Defines related to Hybrid Indicator (HI) (PHICH) channel */
#define NUMBER_OF_HI_BITS_NORMAL_CP         ( 12 )
#define NUMBER_OF_HI_BITS_EXTENDED_CP       ( 6 )
#define MAX_NUM_SYM_ALLOC_FOR_PHICH         ( 3 )
#define NUM_PHICH_SEQ_INDX_NORMAL_CP        ( 8 )
#define NUM_REG_ALLOC_FOR_PHICH_GROUP       ( NUMBER_OF_HI_BITS_NORMAL_CP>>2 )
/*Buffer size to save PHICH symbol buffer generated at Initialization
 * Size = 12 symbols * 8 seqIndices *10 sub-frames * 4 bytes * numBsTxAntPorts
 * data-type S16 */
#define PHICH_INIT_MEM_BUFFER_SIZE          ( NUMBER_OF_HI_BITS_NORMAL_CP * \
                                              NUM_PHICH_SEQ_INDX_NORMAL_CP * \
                                              (NUM_SLOTS/2) * MAX_NUM_ANT_PORT )
/* Memory size for PHICH Resource element buffer */
#define PHICH_INIT_RE_BUFFER_SIZE           ( NUMBER_OF_HI_BITS_NORMAL_CP * MAX_PHICH )

/* For PDCCH */
#define NUM_COL_IN_PDCCH_INTERLEAVER        ( 32 )
#define NUM_SHIFT_PDCCH_INTERLEAVER         ( 5 )
#define MAX_PDCCH_BYTES                     ( 800 )
/*Buffer size to save PDCCH permute table buffer generated at Initialization
 * Size = (200 + (200+300) + (200 + 300 + 300) )
 * = (2*MAX_NUM_RB (for Resource element group, symbol 1 )for 1 control symbol
 * = (5*MAX_NUM_RB for 2 control symbols )
 * = (8*MAX_NUM_RB for 3 control symbols )
 * data-type S16 */
#define PDCCH_INIT_PERMUTE_TABLE_BUFFER_SIZE    ( 15*MAX_NUM_RB )
/* Memory size for PDCCH Resource element buffer
 * Size = 1200*3 (data-type S16 ) */
#define PDCCH_INIT_RE_BUFFER_SIZE               ( MAX_NUM_RB * N_RB_SC * MAX_DL_CONTROL_SYM )
/* Memory size of PDCCH Quadruplets buffer size */
#define PDCCH_INIT_NUM_QUADRUPLETS_BUFFER_SIZE  ( MAX_DL_CONTROL_SYM )

/* For DL cell specific reference sequence */
#define MAX_NUM_DL_CELL_SPECIFIC_SYM_IN_SLOT    ( 3 )
#define MAX_LTE_DL_RB_STD_DEFINED               ( 110 )
/* Size of the buffer holding DL cell specific reference sequence symbols
 * data-type: S16 */
#define DL_CRS_INIT_SYM_BUFFER_SIZE             ( MAX_NUM_DL_CELL_SPECIFIC_SYM_IN_SLOT * \
                                                  4 * MAX_NUM_RB * NUM_SLOTS )

#define PRACH_DETECTION_THRESHOLD               ( 20 )
#define PRACH_DETECTION_THRESHOLD_NOMINAL       ( 20 )      // Used by Ceva. DONT Change this value
#define PRACH_DETECTION_THRESHOLD_GOOD          ( 100 )     // Used by PHY Stats

#define GetDiff(Prev, Curr) ((Prev < Curr) ? (Curr - Prev) : ((Curr + 10240) - Prev))

#define UL_TA_AVERAGING_TIME                    32
#define UL_TA_AVERAGING_MIN_RESOURCE_BLOCKS     10
#define UL_TA_AVERAGING_MIN_CQI_DB              -3
#define UL_TA_AVERAGING_MIN_CQI                 ((UL_TA_AVERAGING_MIN_CQI_DB * 2) + 128)

#define UL_CQI_AVERAGING_PERIOD                 64
#define UL_CQI_AVERAGING_COUNT                  25
#define DEFAULT_CQI_RET_VAL                     0
#define SINR_INIT_VALUE                         20000


#define LTE_BS_PHY_ENABLE_MLOG               1<<0
#define LTE_BS_PHY_ENABLE_APILOG             1<<1
#define LTE_BS_PHY_ENABLE_IQLOGS             1<<2
#define LTE_BS_PHY_ENABLE_PHYSTATS           1<<3
#define LTE_BS_PHY_ENABLE_APIERRORCHECK      1<<4




//*****************************************************************************
//**********                        UPLINK HARQ                      **********
//*****************************************************************************

typedef struct harq_descrpitor_structure
{
    //*************************************************************************
    // FEC hardware de-interleaver descriptor addresses (for each code block)
    // for user for each HARQ iteration.

    S8 *harq_DescAdr[P_HARQ_PROC][P_HARQ_USERS][P_HARQ_COMB][13][60];

    //*************************************************************************
    // FEC hardware de-interleaver descriptor lengths (for each code block)
    // for user for each HARQ iteration.

    S32 harq_DescLen[P_HARQ_PROC][P_HARQ_USERS][P_HARQ_COMB][13][60];

    //*************************************************************************
    // FEC hardware de-interleaver number of descriptors (for each code block)
    // for user for each HARQ iteration.

    S32 harq_DescN[P_HARQ_PROC][P_HARQ_USERS][P_HARQ_COMB][13];


} HARQ_DESCRIPTOR_STRUCT, *PHARQ_DESCRIPTOR_T;

typedef struct harq_structure
{

    //*************************************************************************
    // Soft decision storage buffers.
    // There are separate buffers for each group within each process.
    // The maximum individual buffer length is equal to the maximum possible
    // soft decisions that can be generated each subframe.  The active users
    // share space within each of the buffers.

    S8 harq_sft_buf[P_HARQ_PROC][P_HARQ_COMB][P_HARQ_SFDEC];

    //*************************************************************************
    // Current group to use for storing soft decisions.
    // The number of possible groups is equal to the maximum number of allowed
    // transmissions per process.

    S16 harq_grp[P_HARQ_PROC];

    //*************************************************************************
    // Current HARQ iteration for user this subframe.

    S16 harq_iter[P_HARQ_PROC][P_HARQ_USERS];

    //*************************************************************************
    // Rate matching rvidx (0-3) for user for each HARQ iteration.

    S16 harq_rvidx[P_HARQ_PROC][P_HARQ_USERS][P_HARQ_COMB];

    //*************************************************************************
    // Pointers to buffers being used to store soft decisions for the user.

    S8 *harq_sft_pos[P_HARQ_PROC][P_HARQ_USERS][P_HARQ_COMB];

    //*************************************************************************
    // Rate matching E (for each code block) for user for each HARQ iteration.

    S32 harq_E[P_HARQ_PROC][P_HARQ_USERS][P_HARQ_COMB][13];

    //*************************************************************************
    // Rate matching k0 (for each code block) for user for each HARQ iteration.

    S16 harq_k0[P_HARQ_PROC][P_HARQ_USERS][P_HARQ_COMB][13];

    //*************************************************************************
    // Modulation vector constellation size Qm for user for each HARQ iteration.

    S16 harq_Qm[P_HARQ_PROC][P_HARQ_USERS][P_HARQ_COMB];

#if 0
    //*************************************************************************
    // FEC hardware de-interleaver descriptor addresses (for each code block)
    // for user for each HARQ iteration.

    S8 *harq_DescAdr[P_HARQ_PROC][P_HARQ_USERS][P_HARQ_COMB][13][60];

    //*************************************************************************
    // FEC hardware de-interleaver descriptor lengths (for each code block)
    // for user for each HARQ iteration.

    S32 harq_DescLen[P_HARQ_PROC][P_HARQ_USERS][P_HARQ_COMB][13][60];

    //*************************************************************************
    // FEC hardware de-interleaver number of descriptors (for each code block)
    // for user for each HARQ iteration.

    S32 harq_DescN[P_HARQ_PROC][P_HARQ_USERS][P_HARQ_COMB][13];
#endif

    PHARQ_DESCRIPTOR_T      pHarqDescriptor;

    //*************************************************************************
    // Current HARQ process for subframe (0-7).
    // The process increments every subframe.

    S16 harq_prc;

    //*************************************************************************
    // Current user index.

//    S16 harq_user;

    //*************************************************************************

    //FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    // Arrays used only for floating point simulation
#ifdef _WIN32

    //*************************************************************************
    // Soft decision storage buffers.
    // There are separate buffers for each group within each process.
    // The maximum individual buffer length is equal to the maximum possible
    // soft decisions that can be generated each subframe.  The active users
    // share space within each of the buffers.

    double harq_sft_buf_fl[P_HARQ_PROC][P_HARQ_COMB][P_HARQ_SFDEC];

    //*************************************************************************
    // Pointers to buffers  being used to store soft decisions for the user.

    double *harq_sft_pos_fl[P_HARQ_PROC][P_HARQ_USERS][P_HARQ_COMB];

    //*************************************************************************
    // FEC hardware de-interleaver descriptor addresses (for each code block)
    // for user for each HARQ iteration.

    double *harq_DescAdr_fl[P_HARQ_PROC][P_HARQ_USERS][P_HARQ_COMB][13][60];

    //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
    // If using weighted HARQ combining, then add weights to structure.
#if USE_WEIGHTED_HARQ_COMB

    //*************************************************************************
    // Soft decision weighting factor for user this subframe.

    double harq_weight_fl[P_HARQ_PROC][P_HARQ_USERS][P_HARQ_COMB];

    //*************************************************************************

#endif
    //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW

    //FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#endif

} HARQS, *PHARQS;

//*****************************************************************************


//*****************************************************************************
// RNTI to Phy Variables Index Mapping

#define NUM_FRAMES_BEFORE_CLEAR_RNTI_INDEX     4
#define NUM_UNIQUE_RNTI                        65536
#define RNTI_MAP_NOT_USED                      -1
#define RNTI_VARS_NOT_USED                     0
#define RNTI_VARS_USED                         1

// Keep Track of User Indices
typedef struct used_index_vars
{
    S8 index;
    S32 last_frame_used;
    U32 RNTI;
    U32 flushed;
    S32 mErrAvgCh[2];
    S16 mErrExpo;
    U8 mSetDefaults;
    U16 NumRbAvg;
    U16 reserved;
    S32 timeAveragedFreqErr[2];

    // Sinr for PUCCH and PUSCH
    S32 avgPucchSinr, avgPuschSinr;
    U32 pucchSinrCnt, puschSinrCnt;
    U32 pucchSinrPrevTti, puschSinrPrevTti;
    S32 pucchSinrPrevSinr, puschSinrPrevSinr;

    // Timing Advance Averaging for PUSCH
    U32 avgPuschTa;
    U32 puschTaCnt;
} USED_INDEX_VARS, *PUSED_INDEX_VARS;

typedef struct BsRxRntiMapping
{
    // Map RNTI to Phy Index
    U32 max_used_index;
    S8 rnti_map[NUM_UNIQUE_RNTI];
    USED_INDEX_VARS used_index[NUM_CONNECTED_USERS];
} BS_RX_RNTI_MAPPING, *PBS_RX_RNTI_MAPPING;

//*****************************************************************************

#if defined(CEVA_INTRINSICS) || defined(_WIN32)

#define MAX_IDFT_TRANSACTIONS_PER_SYMBOL   4
#define MAX_IDFT_SIZE_SMALLER_TRANSACTION  216
#define NUM_IDFT_INPUT_DMA_DESC            1    // 1 Input
#define NUM_IDFT_OUTPUT_DMA_DESC           MAX_IDFT_TRANSACTIONS_PER_SYMBOL    // 4 Output (3 Dummy and 1 Real)
#define NUM_IDFT_CONFIG_TABLE_DMA_DESC    (MAX_IDFT_TRANSACTIONS_PER_SYMBOL*2)    // (1 Small Config, 1 Mult Tables) x 4
#define NUM_IDFT_OUTPUT_DESC_START        (NUM_IDFT_CONFIG_TABLE_DMA_DESC + 1)    // 1 Input + all config /mult tables
#define MAX_NUM_IDFT_DMA_DESC             (NUM_IDFT_INPUT_DMA_DESC + NUM_IDFT_OUTPUT_DMA_DESC + NUM_IDFT_CONFIG_TABLE_DMA_DESC) // 13 descriptors in total

#endif

#define FPDRV_NUM_FP                       10
#define FP_DRV_MAX_NUM_USERS_PER_TTI       10

#define BCTRL_BLAST_MASK        0x00010000

#ifndef _MAP_DRV_IDFT_CONFIG_
#define _MAP_DRV_IDFT_CONFIG_
typedef struct tMAP_DRV_IDFT_CONFIG
{
    U16 InputLoc[2];      // 2: I/Q
    U16 OutputLoc[2];     // 2: I/Q
    U16 ConfigLoc[2];     // 2: I/Q
    U16 MultTableLoc[2];  // 2: I/Q
} MAP_DRV_IDFT_CONFIG, *PMAP_DRV_IDFT_CONFIG;
#endif // _MAP_DRV_IDFT_CONFIG_

#ifndef _MAP_DRV_IDFT_MULT_TABLE_
#define _MAP_DRV_IDFT_MULT_TABLE_
typedef struct tMAP_DRV_IDFT_MULT_TABLE
{
    U8 *TableDMALoc;
    U32 TableSize;
}MAP_DRV_IDFT_MULT_TABLE, *PMAP_DRV_IDFT_MULT_TABLE;
#endif // _MAP_DRV_IDFT_MULT_TABLE_

#ifndef _MAP_DMA_TR_CTRL_CEVA_
#define _MAP_DMA_TR_CTRL_CEVA_
typedef struct tMAP_DMA_TR_CTRL_CEVA
{
    //U64  FpId:5;
    //U64  LoadType:2;
    //U64  LoadCont:1;
    //U64  StartBank:8;
    //U64  SegStart:1;
    //U64  BusSize:2;
    //U64  DataType:1;
    //U64  TrInt:3;
    //U64  StartAddr:11;
    //U64  SegCount:11;
    //U64  TrId:6;
    //U64  Res:13;

    // Since Ceva Does not support U64, since 2 dummy U32s. Fields will not be modified but memcpy will be used
    U32 a;
    U32 b;
} MAP_DMA_TR_CTRL_CEVA, *PMAP_DMA_TR_CTRL_CEVA;
#endif //_MAP_DMA_TR_CTRL_CEVA_


#define IDFT_CONFIG_TABLE_SIZE             ((sizeof(MAP_DRV_IDFT_CONFIG)*FP_DRV_MAX_NUM_USERS_PER_TTI + 8)*MAX_IDFT_TRANSACTIONS_PER_SYMBOL)
#define IDFT_MULT_TABLE_SIZE               ((sizeof(MAP_DRV_IDFT_MULT_TABLE)*FP_DRV_MAX_NUM_USERS_PER_TTI)*MAX_IDFT_TRANSACTIONS_PER_SYMBOL)


#ifndef _DMABDESC_
#define _DMABDESC_
typedef struct tDMABDESC
{
    U32     BPtr;
    U32     BCtrl;
} DMABDESC, *PDMABDESC;
#endif // _DMABDESC_

#ifndef _DMAFDESC_
#define _DMAFDESC_
typedef struct tDMAFDESC
{
    struct  tDMAFDESC   *NextFDesc;
    U32             FControl;
    U32             FStatus0;
    U32             FStatus1;
    DMABDESC        OutBDesc;
    DMABDESC        InBDesc;
} DMAFDESC, *PDMAFDESC;
#endif // _DMAFDESC_


typedef struct tMAP_SETUP_DESCRIPTOR
{
    // Input from TCB Creation
    U16 *pIdftSizeArr;
    U32 ArrSize;
    U32 SymMask;
    U32 fftIfftSize;
    U32 fftIfftCtxNum[4];
    U32 numRxAnt, numTxAnt;
    PTCB pTcb;
    PDMAFDESC pIdftFDescHead;
    PDMAFDESC pFftFDescHead0;
    PDMAFDESC pFftFDescHead1;
    PDMAFDESC pIfftFDescHead0;

    // Inputs from Driver
    MAP_DMA_TR_CTRL_CEVA FftInTrCtrl128;
    MAP_DMA_TR_CTRL_CEVA FftInTrCtrl;
    MAP_DMA_TR_CTRL_CEVA IfftInTrCtrl0;
    MAP_DMA_TR_CTRL_CEVA IfftInTrCtrl1;
    MAP_DMA_TR_CTRL_CEVA IdftInTrCtrl[FPDRV_NUM_FP][3];
    MAP_DMA_TR_CTRL_CEVA IdftOutTrCtrl[FPDRV_NUM_FP];
    U32 IdftOutputSize;
    PDMAFDESC pReadyListHead[FPDRV_NUM_FP];
    PDMAFDESC pReadyListTail[FPDRV_NUM_FP];
    U32 IdftFpStart;
    U32 IdftFpStop;
    U8  *pConfigTables[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    U8  *pMultTables[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];

    // Tables
    U8  *fp_idft_size_index_map;
    U16 *fp_idft_big_config_start;
    U16 *fp_idft_mult_table_start;
    U16 *fp_idft_tables[35];
    U16 *fp_idft_table_sizes_with_offset;
}MAP_SETUP_DESCRIPTOR, *PMAP_SETUP_DESCRIPTOR;

//*****************************************************************************

typedef struct BsRxChMeas_Info
{
    S16 Reserved;
    S16 TimingAdvance;
} BSRXCHMeas, *PBSRXCHMeas;

typedef struct UlRxSduParams
{
    U16 nRNTI[MAXCHSUBFRAME];
    U16 dlCqiPmiSizeBits[MAXCHSUBFRAME];
    U8  pucch_mac_sdu[MAXCHSUBFRAME][2];

    U8 stat[MAXCHSUBFRAME];
} ULRXSDUPARAMS, *PULRXSDUPARAMS;

typedef struct UlRxInputParams
{
    U16 rxFrameNumber;
    U16 subframeNumber;

    //Common for all functions
    S16 numChan;
    S16 numPUSCH;
    S16 numPUCCH;
    S16 TotalMsc;
    S16 slotNum;
    S16 Nrb_sc;
    S16 Nul_symb;
    S16 K;
    S16 L;
    S16 MI;
    S16 MO;
    S16 MO_1;
    U16 ChanId[MAXCHSUBFRAME];
    S16 subcStart[MAXCHSUBFRAME];
    U8  RBStart[MAXCHSUBFRAME];
    U8  NumRBs[MAXCHSUBFRAME];
    S8  physChanType[MAXCHSUBFRAME];
    S8  Qm[MAXCHSUBFRAME];
    S16 Mpusch_sc[MAXCHSUBFRAME];

    // IDFT
    S32 NumIdftDesc;
    U8* ppConfigTables[4];
    U8* ppMultTables[4];
    U32 pConfigTablesSize[4];

    // Demapper
    S32 DescrInBufSizePusch;
    S32 DescrInBufSizePucch;

    //For PUSCH and PUCCH Channel Estimate
    S16 df;
    U32 Fs;
    S16 NusedE;
    S16 Nfft;
    S16 Nul_rb;

    //Channel Measurement, TimingAdv and SINR
    S32 mErrAvg[2*MAXCHSUBFRAME];
    S16 mErrExpo[MAXCHSUBFRAME];
    U16 SnrSubfr[4*MAXCHSUBFRAME];      // Estimated subframe SNR for each antenna
    U16 SnrPerRB[MAXRESOURCEBLOCKS];    // Estimated SNR per resource block
    U8  SnrPerRB_dB[MAXRESOURCEBLOCKS];
    U8  ul_CQI[MAXCHSUBFRAME];          //SINR in api-dB units, calculated in channel estimate
    U8  softDecSinrdB[MAXCHSUBFRAME];   //SINR in api-dB units, calculated in demapper soft decisions
    S16 freqOffset[MAXCHSUBFRAME];      //frequency offset in Hz
    S32 timeAveragedFreqErr[2*MAXCHSUBFRAME]; //[S15.16] error signal transferred from/to active channel buffer
    U8  chanType_rb[MAXRESOURCEBLOCKS]; //channel type received in each RB

    BSRXCHMeas BsRxChMeas[MAXCHSUBFRAME];

    //For DeMapper
    U32 G[MAXRESOURCEBLOCKS];

    //Common for all channels, Uplink Pilots
    S16 Ncell_id;
    S16 CPType;
    S16 groupHoppingEnabled;
    S16 sequenceHoppingEnabled;
    S16 n1_DMRS;
    S16 delta_ss;

    //Common for PUSCH and PUCCH Uplink Pilots
    S16 pilotCarrierSymbol;
    S8  n2_DMRS[MAXCHSUBFRAME];

    //Specific for PUCCH
    S8  simSRHarq[MAXCHSUBFRAME];
    S16 srPUCCHResourceIndex[MAXCHSUBFRAME];

    //Specific for PUCCH Uplink Pilots
    S8  delta_shift_PUCCH;
    S16 betaPUCCH[MAXCHSUBFRAME];
    S16 num_Pilots_slot[MAXCHSUBFRAME];
    S8  N2_rb;

    S16 n1[MAXCHSUBFRAME];
    S16 n2[MAXCHSUBFRAME];
    S8  N1_cs;
    S8  pucchFormat[MAXCHSUBFRAME];
    S16 RSbetaPUCCH[MAXCHSUBFRAME];
    S16 Pucch_fgh[NUM_SLOTS];
    S16 Pucch_uSeqGrpNum[NUM_SLOTS];
    S16 Pucch_uSeqGrpNum_GrHopping[NUM_SLOTS];
    S16 Pucch_ncs_cell[7 * NUM_SLOTS];

    // SR
    S16 srdetected[MAXCHSUBFRAME];

    PUSED_INDEX_VARS usedIndexVars[MAXCHSUBFRAME];
    U8 NumRbAvg[MAXCHSUBFRAME];

    //Multidimensional array dimensions, for linearized array element access.
    //(Temporary, until convert to conventional use of multidimensional arrays)
    //
    //Note 1: C stores multidimensional arrays with rightmost index incrementing through memory locations
    //Note 2: Complex arrays use Re=index*2+0, Im=index*2+1 directly, no Offset variable for the ReIm=x2

    //- var[rxAnt,subc,ReIm]
    S32 antSubcReIm_antOffset;

    //- var[slot,subc,ReIm]
    S32 slotSubcReIm_slotOffset;

    //- var[rxAnt,slot,subc,ReIm]
    S32 slotAntSubcReIm_slotOffset;
    S32 slotAntSubcReIm_antOffset;

    //- var[sym,rxAnt,subc,ReIm]
    S32 symAntSubcReIm_symOffset;
    S32 symAntSubcReIm_antOffset;

    //- var[sym,subc,ReIm]
    S32 symSubcReIm_symOffset;

    //- var[RB,rAnt,cAnt,ReIm]
    S32 rbRAntCAntReIm_rbOffset;
    S32 rbRAntCAntReIm_rAntOffset;

    //- var[slot,RB,rAnt,cAnt,ReIm]
    S32 slotRbRAntCAntReIm_slotOffset;
    S32 slotRbRAntCAntReIm_rbOffset;
    S32 slotRbRAntCAntReIm_rAntOffset;

    //- var[slot,rxAnt,chan,Sym,ReIm]
    S32 slotAntChanSymReIm_slotOffset;
    S32 slotAntChanSymReIm_antOffset;
    S32 slotAntChanSymReIm_chanOffset;

    //- var[slot,rxAnt,chan,ReIm]
    S32 slotAntChanReIm_slotOffset;
    S32 slotAntChanReIm_antOffset;

    //- var[slot,rAnt,chan]
    S32 slotAntChan_slotOffset;
    S32 slotAntChan_antOffset;

    //- var[slot,rAnt,rb]
    S32 slotAntRb_slotOffset;
    S32 slotAntRb_antOffset;

    //- var[chan,RxAnt,ReIm]
    S32 antChanReIm_antOffset;

    //- var[chan,psym,subc,ReIm]
    S32 chanPsymSubcReIm_chanOffset;
    S32 chanPsymSubcReIm_psymOffset;
    U32 harqRntiIndex[MAXCHSUBFRAME];
    S32 Multi_PUCCH_PerRB[MAXCHSUBFRAME];
    S32 PucchValidTimeOffset[MAXCHSUBFRAME];

    // Reed Muller Decoding Confidence Metric
    U16 rmconf[MAXCHSUBFRAME];

    U32 currTti;

    U32* pMdmaDescr;

    U16 RipPerRB_dB[MAXRESOURCEBLOCKS]; // Estimated RIP per resource block
    U16 Rssi_dB[MAXCHSUBFRAME];
    S16 ThermalNoisePwr;
    S16 unusedRB;

    S16 Temperature[8];
    U16 rxIntMeasFlag;
    U16 rxTherNoiseFlag;
    U16 rssiMeasFlag;
    U16 rssiChanMeasFlag[MAXCHSUBFRAME];
    U16 Ncp0;
    U16 Ncp;

    U32 isDummyApiRunning;
} ULRXINPUTPARAMS, *PULRXINPUTPARAMS, BSRXINPUTPARAMS, *PBSRXINPUTPARAMS;

typedef struct UlRxFecParams
{
    S8  channelType[MAXCHSUBFRAME];
    S8  rvidx[MAXCHSUBFRAME];
    S8  nDi[MAXCHSUBFRAME];
    S8  Qm[MAXCHSUBFRAME];
    U16 nRNTI[MAXCHSUBFRAME];

    U8  pucch_mac_sdu[MAXCHSUBFRAME][2];
    S16 pucchFormat[MAXCHSUBFRAME];
    S16 refSubFrame[MAXCHSUBFRAME];

    U16 refFrame[MAXCHSUBFRAME];
    U16 runSubFrCtr[MAXCHSUBFRAME];

    S16 HalfIterations;
    U16 numChan;
    S8 *genBuf;

    S16 Mpusch_rb[MAXCHSUBFRAME];
    S16 Mpusch_sc[MAXCHSUBFRAME];
    S16 nrbi[MAXCHSUBFRAME];

    //For multiplex control and data
    S16 nACK[MAXCHSUBFRAME];
    S16 nRI[MAXCHSUBFRAME];
    S16 nr1CQI[MAXCHSUBFRAME];
    S16 nrg1CQI[MAXCHSUBFRAME];

    S8  betaOffsetACKIndex[MAXCHSUBFRAME];
    S8  betaOffsetRIIndex[MAXCHSUBFRAME];
    S8  betaOffsetCQIIndex[MAXCHSUBFRAME];
    S8  nsymi[MAXCHSUBFRAME];

    U32 numbitsACK[MAXCHSUBFRAME];
    U32 numbitsRI[MAXCHSUBFRAME];
    U32 numbitsCQI[MAXCHSUBFRAME];
    U32 numBitsRx[MAXCHSUBFRAME];
    U32 numBytes[MAXCHSUBFRAME];
    S32 G[MAXCHSUBFRAME];

    U32 muxPUSCHAckNack;
    U32 muxPUSCHCqiRi;
    U32 TotalACK_Bits;
    U32 TotalRI_Bits;
    U32 TotalCQI_Bits;

    U16 symPosACK[MAX_PUSCH_DATASYMB_PER_SUBF];
    U16 symPosRI[MAX_PUSCH_DATASYMB_PER_SUBF];
    U16 symPosCQI[MAX_PUSCH_DATASYMB_PER_SUBF];

    U16 numSymPosACK[MAXCHSUBFRAME][MAX_PUSCH_DATASYMB_PER_SUBF];
    U16 numSymPosRI[MAXCHSUBFRAME][MAX_PUSCH_DATASYMB_PER_SUBF];
    U16 numSymPosCQI[MAXCHSUBFRAME][MAX_PUSCH_DATASYMB_PER_SUBF];

    U32 muxPUSCH_User[MAXCHSUBFRAME];
    U32 ACK_User_offset[MAXCHSUBFRAME];
    U32 RI_User_offset[MAXCHSUBFRAME];
    U32 CQI_User_offset[MAXCHSUBFRAME];

    S8  *by[MAXCHSUBFRAME];
    U8  *mac_sdu[MAXCHSUBFRAME];
    U8  riStore[MAXCHSUBFRAME];      // Storage for RI  info
    U8  ackStore[MAXCHSUBFRAME];     // Storage for ACK info
    U8  cqiStore[MAXCHSUBFRAME][32]; // Storage for CQI info
    U16 rmconf[MAXCHSUBFRAME];

    S16 Itbs[MAXCHSUBFRAME];   // 3GPP TS 36.213 Table 7.1.7.2.1-1 Transport block size table index (0 - 27)
    S16 Nprbm1[MAXCHSUBFRAME]; // 3GPP TS 36.213 Table 7.1.7.2.1-1 Transport block size table Nprb-1 (0 - 109)

    S16 harq_user[MAXCHSUBFRAME];

} ULRXFECPARAMS, *PULRXFECPARAMS;



typedef struct DlControlParams
{
    S16* a;
    S16* PHICH_Bit;
    S16* PHICH_SequenceIndex;
    U16* pdcchRNTI;
    U8* scrSeq;
    S16* Grp;
    S16 numTxAntennas;
    S16 numPDCCHSymbols;
    S16 numcols_a;
    U32 number_of_phich_channels;
    S16 CPType;
    U32 Ncell_id;
    S16 controlIndicatorScaleFactor;
    S16 N_PHICH_Grp;
    S16 PHICH_Grp_Idx1;
    S16 scalefactorPDCCH;
    S16 scalefactorPHICH;
    S16 pilotScaleFactor;
    S16 numGroups;
    S16 Ndl_rb;
    S16 Nrb_sc;
    S16 slotNumber;
    S16 stepsize_a;
    U32 transmode;
    S16 numCtrlSymbols;
    U32 number_of_pdcch_channels;
    U16 numPDCCH;
    U16 numDCI0;
    S16 isym;
    S16 pilotPowerdB_EPRE; //pilotPower;
    S16 PHICH_SequenceIndex1[MAX_PHICH];
    S16 PHICH_Bit1[MAX_PHICH];
    S16 PHICH_Grp[MAX_PHICH];
    S16 StartCCECtl[MAXCHSUBFRAME];
    S16 NumCCECtl[MAXCHSUBFRAME];

    // To store the APIs
    U8* startPDCCH;
    U8* inputPDCCH;
    U8* ptrInputPDCCH;
} IOParams_DlControl, *PIOParams_DlControl;

typedef struct SRS_Params
{
    S16 df;
    U32 Fs;
    S16 Nul_symb;
    S16 Nrb_sc;
    S16 NusedE;
    S16 Nfft;
    S16 numSrs;
    S16 mo;
    S16 K;
    S16 L;
    S16 Nul_rb;
    S16 srsPilotSymbol;
    S32 stepsize_aDet;
    S16 srsBandwidthTab;
    S16 srsBandwidthConfig;
    S16 frameNumber;
    S16 subframeNumber;
} IOParams_SRS, *PIOParams_SRS;

typedef struct SRS_PilotsParams
{
    S16 subframeNumber;
    S16 Nrb_sc;
    S16 Nused;
    U32 Ncell_id;
    S16 groupHoppingEnabled;
    S16 sequenceHoppingEnabled;
    S16 numSrs;
    S16 delta_ss;
} IOParams_SRS_Pilots, *PIOParams_SRS_Pilots;

// IDFT for SRS
typedef struct {

    short N;          // IDFT size
    // short nFactors;   // Number of factor for decompose the IDFT size N
    // short Factor[6];  // List of the factors
    short *pxF16;     // pointer to input buffer
    short *pxT16;     // pointer to output buffer
    short Exp2;
    int   *pxTbuf;    // pointer to buffer for intermediate data (32-bit)

    short TwddQCol;
    short TwddNptInEachCol;
    short *pTwdd[5];
    int   TwddOffset[5];

    short *p3ptSqrN;
    short E3ptSqrN;
    int ColOffset3ptSqrN;
    int N3ptInEachCol;
    int Offset3ptSqrN;   // (N/4)/3

    short *pShffIdx;
    short nShffIdx;

    short *pTableArray;

} IDFTparam;

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


typedef struct PUSCH_Info
{
    U8 cvec[7 * NUM_SLOTS]; //array of zeroes
    U8 cvec_u[7 * NUM_SLOTS];
    U8 PRSbits[7 * NUM_SLOTS];

} BS_PUSCH, *PBS_PUSCH;

typedef struct SRS_Info
{
    S16 srsBandwidthTab;
    S16 srsPilotSymbol;
    S16 srsEnable;
    S16 srsBandwidthConfig; //C_srs, Tables 5.5.3.2-x [3GPP TS 36.211, Sec.5.5.3]
    S16 srsSubframeConfig; //Table 5.5.3.3-1 [3GPP TS 36.211, Sec.5.5.3]
    S16 srsSimultaneousANandSRS; //[3GPP TS 36.213, Sec.8.2]
    S16 srsSubframeEnable[10];
    S16 u[NUM_SLOTS];
    //S16 v[NUM_SLOTS]; // Moved to dedicated
} BS_SRS, *PBS_SRS;

typedef struct RxChan_SRS_Info
{
    U16 enable[MAX_UL_SRS];
    U16 cdmEnable[MAX_UL_SRS]; // 1=Code Division Multiplexing enabled.
    // i.e. Multiple SRS are sharing this frequency allocation
    // with different cyclic shifts

    U16 msIndex[MAX_UL_SRS]; // index to MobileStation (/ MIMO input antenna),
    // that this SRS channel is sent from

    U16 cdmGroupIndex[MAX_UL_SRS]; // SRS channels in the same CDM group need to be configured
    // in consecutive SRS Channel numbers - within a CDM group,
    // each SRS must have a different cyclic shift

    U16 cyclicShift[MAX_UL_SRS]; // nCs_srs, [3GPP TS 36.211, Sec.5.5.3.1]
    // within a CDM group all the following parameters must be the same

    U16 bandwidth[MAX_UL_SRS]; // =3 for CAZAC24, =2 for ZadoffChu48,
    // Tables 5.5.3.2-x [3GPP TS 36.211, Sec.5.5.3]

    U16 hoppingBandwidth[MAX_UL_SRS]; //b_hop, [3GPP TS 36.211, Sec.5.5.3.2]
    U16 freqDomainPosition[MAX_UL_SRS]; //n_rrc, [3GPP TS 36.211, Sec.5.5.3.2].
    U16 duration[MAX_UL_SRS]; //Duration, [3GPP TS 36.213, Sec.8.2].
    U16 configIndex[MAX_UL_SRS]; //I_srs, [3GPP TS 36.213, table8.2-1].
    U16 transmissionComb[MAX_UL_SRS]; //k_tc, [3GPP TS 36.211, Sec.5.5.3.2]

    U16 StartRb[MAX_UL_SRS];
    U16 EndRb[MAX_UL_SRS];
    S16 v[NUM_SLOTS * MAX_UL_SRS];
} BS_RX_CHAN_SRS, *PBS_RX_CHAN_SRS;

typedef struct PRACH_Info
{
    S16 MO;
    S16 iAntenna;
    S16 Nzc;
    S16 K;
    S16 phi;
    S16 k0;
    S16 Nseq;
    S16 Ncp;
    S16 FIRLength;
    S16 dummy;
    S32 length_adcPrach;

    S32 SegmSize;
    S32 nSegm;
    S32 iSegm;
    S32 AdcNextIdx; // Idx to the pAdcBuf[] for the next segment

    S16 demod_start;
    S16 demod_end;
    S16 demod_istart;
    S16 demod_idx;
    S16 demod_inv;
    S16 demod_temp;
    S32 demod_step_size;

    S16 DSR; // Down sampling rate
    S16 Nf; // FIR filter length
    S16 lpfDelay;
    S16 IdxDown;
    S16 nStart;

    S16 numRootSeq;
    S16 u[32];

    S16 Cv;
    S16 vLen;
    S16 Ncs;

    S16 dummy2;
    S16 nPeakDetected;
    S16 iseqPeakDetected[32];
    S16 locPeakDetected[32];
    S16 preambleCntPeakDetected[32];
    S16 searchWinPeakDetected[32];
    U16 prachDetPHR[32];

} BS_PRACH, *PBS_PRACH;


typedef struct BS_RX_PRACH_Info
{
    S16 bs_prach_enable;
    S16 bs_prach_configurationIndex;
    S16 bs_prach_frequencyOffset;
    S16 bs_prach_rootSequenceIndex;
    S16 bs_prach_highSpeedFlag;
    S16 bs_prach_cyclicShiftConfig;
    S16 bs_prach_Nseq;
    S16 bs_prach_Ncp;
    S16 bs_prach_preambleFormat;
    S16 bs_prach_preambleSeqLength;
    S16 bs_prach_subcSpacing;
    S16 bs_prach_subcOffset;
    S16 bs_prach_subcSpacingRatio;
    S16 bs_prach_nRa_prb;
    S16 bs_prach_k0;
    S16 bs_prach_preambleSubframeEnable[10];
    S16 bs_prach_TseqMicrosec;
} BS_RX_PRACH, *PBS_RX_PRACH;

typedef struct MS_DET_Info
{
    S16 num;
    S16 cyclicShift[64];
    S16 tDelMicrosecEst[64];
    U16 peakToAvgPowerRatio[64];
} MS_DET, *PMS_DET;

typedef struct BS_RX_PRACHDet_Info
{
    S16 bs_prachDet_downsampleRate;
    S16 bs_prachDet_lpfDelay;
    S16 bs_prachDet_firLpf_length;
    S16 bs_prachDet_numRootSeq;
    S16 bs_prachDet_numCyclicShiftsPerRootSeq;
    S16 bs_prachDet_cyclicShift;
    S16 bs_prachDet_enable;
    S16 bs_prachDet_uOffset;
    S16 bs_prachDet_Ncs;
    S16 bs_prachDet_u[64];
    S16 bs_prachDet_tsamOffset[64];
    S16 bs_prachDet_rootSeqDet[64];
    S16 bs_prachDet_tsamOffsetEst[64];
    MS_DET bs_prachDet_msDet;
} BS_RX_PRACHDet, *PBS_RX_PRACHDet;


typedef struct BsRxPointers
{
    S16 *pDerotation;
    S16 *pDependencyArray,  *pDependencyArray1;
    S16 *pDependencyArray2, *pDependencyArray3;
    S16 *pDependencyArray4;

    S16 *pExpo_mat;
    S16 *pExpo_ch_pusch, *pExpo_ch_pucch;
    S16 *pExpo_norm_reEst;
    S16 *pChEst_ul;

    S16 *r_alpha_uv;
    S32 *pErrAvg;
    S16 *pErrExpo;
    S32 *pErrAvgOut;
    S32 *mErrTemp;
    S16 *mErrExpoTemp;

    S8 *pMuxCtrl_SoftDec;

    S32 *rxInPwrAvg_PerRbPilot;
    S16 *pPwrAvg_Pilot_expo;
    S32 *rxInPwrAvg_PerRbDataSym;
    S16 *pPwrAvg_Data_expo;

    S32 *pSnrPilot;
    S32 **ppSnrPerRb_Pilot;
    S32 *pSnrPerRb;
    S16 *pucchChan[NUM_USER_PUCCH_TCBS];
    S32 *pnoiseVar;

    S16 *paEstSig;
    S16 *pReDet0, *pBs_aPilots;
    S16 *bs_aPilots_pucch;
    S16 *nOc, *Sdescramb;
    S16 *pAvgReDet_pucchACK;
    S16 *pChEst_pucch;
    S16 *pAvgReDet_pucchCQI;
    S32 *pAllPucchPwrAvg_AntSlot;
    S32 *pPucchPwrAvg;

    // SRS Stuff
    S16 *pSrsChEst, *pRawSrsChEst;
    S16 *pSrsPilots;
    S16 *pLengthSrsPilots, *nCsSrs;
    S32  *pSrsSnrPerRb;
    U8   *pSrsAvgSnrPerRbdB;
    PBSRXCHMeas pSrsChMeas;

    S32 *pErrSub_pucch;
    S16 *pChEst_ul_pucch;
    S16 *pErrSubExpo_pucch;
    S32 *pRxInRbPwrAvg_AntSlot;
    S32 *pRxInRip;
    S32 *pRxRssi_AntSlot;
    S16 *pInitFlag;
    U8  *pRbUsedFlag;
} BS_RX_POINTERS, *PBS_RX_POINTERS;


typedef struct BS_Rx_Info
{
    S16 Nrx_antennas;
    S16 numMS;
    S16 Nul_rb;
    S16 Nul_symb;
    S16 Ndata_rb;
    S16 frameNumber;
    S16 subframeNumber;
    S16 slotNumber;
    S16 numSrsChan;
    S16 numPUCCHSymbols;
    S16 *isym_eff;
    S16 *adcbuf[MAXNUMLAYERS];

    U32 *rs_grid;
    S16 *derotation;
    S16 *expo_mat;
    S16 *aDet;
    S16 *mhcs;
    S16 *bs_aPilots;
    S16 *bs_aPilots_pucch;
    S16 *ul_pilot_offset;
    S16 *aP;
    S8 *bUserDetSoft;

    S16 *srsChEst;

    // Task Lists and TCBs
    U32 isExeTaskListReady;
    U32 isExePrachTaskListReady[2];
    U32 isExeSrsTaskListReady;

    U32 rxListId;

    PTCB pTaskRx1[MAX_NUM_TCB];     // Used for PUSCH and PUCCH
    PTCB pTaskRx2[4];               // Used for PRACH
    PTCB pTaskRx4[4];               // Used for PRACH
    PTCB pTaskRx3[MAX_NUM_TCB];     // Used for SRS
    PTCB pMuxCtrlTcbCtx1;           // Used for MUX PUSCH FEC decoding
    PTCB pMuxCtrlTcbCtx2;           // Used for MUX PUSCH FEC decoding
    PTCB pFftTcb[2];                // Needed only for MLOG
    PTCB pSnrSubframeTask;
    PTCB pRipRssiSubframeTask;
    PTCB pFftBlockTcbs1[16];
    PTCB pFftBlockTcbs3[16];
    PTCB pTasksRxBypass[MAX_NUM_TCB];

    PTASKLIST pExeTaskList[1];      // Used for PUSCH and PUCCH
    PTASKLIST pExePrachTaskList[2]; // Used for PRACH
    PTASKLIST pExeSrsTaskList[1];   // Used for SRS

    // Used for Device mode only
    U32  fftBlockTcbsCnt1;
    U32  fftBlockTcbsCnt3;
    U32 *pFftCell[2];
    U32 *pIdftCell[NUM_IDFT_CELL_DEPENDENCY];

    PMAP_SETUP_DESCRIPTOR pMapSetupDesc;
    U8*  pIdftDriverBuf;

    U32 rxSduCtx;

    PULRXINPUTPARAMS pUlRxInputParams;
    PULRXSDUPARAMS   pUlRxSduParams;
    PULRXFECPARAMS   pUlRxFecParams[2];
    U32              RxFecParam_Context;
    U32              MuxControlTcbCtx;
    U32              RxPrach_Context;
    U32              prachListId[2];
    U32              prachReturnCtx[2];
    U32              srsListId;

    PBS_PUSCH        pBsPusch;
    PBS_RX_CHAN_SRS  pBsRxChanSrs;

    // Allocation for General buffers for BsRx
    PBS_RX_POINTERS  pBsRxPointers;
    U8              *GenULCramBuf;
    U32              GenULCramBufSize;
} BS_RX, *PBS_RX;

typedef struct TxSystemInfoInner
{
    S16 *aSymbol[TXANT_FOUR];
} TXSYSTEMINFOINNER, *PTXSYSTEMINFOINNER;

typedef struct TxMapperMultiChanIn
{
    U8 TxPackedBitBuf0[1200];
    U8 TxPackedBitBuf1[1200];
} TXMAPPERMULTICHANIN, *PTXMAPPERMULTICHANIN;

typedef struct TxMapperMultiChanOut
{
    S16 TxMapperOutBuf0[2400];
    U32 Buf4M0symb[MAXCHSUBFRAME];
    S16 TxMapperOutBuf1[2400];
    U32 Buf4M1symb[MAXCHSUBFRAME];
    S16 OutputStatus;
} TXMAPPERMULTICHANOUT, *PTXMAPPERMULTICHANOUT;

typedef struct TxLayerMapperMultiChanOut
{
    S16 TxLayerMapperOutBuf[9600];
    U16 xIndex[MAXCHSUBFRAME];
    U16 nrow_x[MAXCHSUBFRAME];
    U16 ncol_x[MAXCHSUBFRAME];
    U16 ncol_y[MAXCHSUBFRAME];
    S16 OutputStatus;
} TXLAYERMAPPERMULTICHANOUT, *PTXLAYERMAPPERMULTICHANOUT;

typedef struct TxPrecoderMultiChanOut
{
    S16 TxPrecoderOutBuf[9600];
    U16 ncol_y[MAXCHSUBFRAME];
    U16 yIndex[MAXCHSUBFRAME];
    U16 length_y[MAXCHSUBFRAME];
    S16 OutputStatus;
} TXPRECODERMULTICHANOUT, *PTXPRECODERMULTICHANOUT;

typedef struct DLResElemMapperMultiChanOut
{
    S16 scIndex[1200];
    S16 scale_factor[MAXCHSUBFRAME];
    S16 OutputStatus;
} DLRESELEMMAPPERMULTICHANOUT, *PDLRESELEMMAPPERMULTICHANOUT;

typedef struct DlTxInputParams
{
    U16 txFrameNumber;
    U16 subframeNumber;
    S16 frameDuplexType;

    U32 NumSubCars;
    U32 numChan;
    U32 numCtrlChan;
    U32 numPhichChan;
    U16 Ntx_antennas;
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
    S16 Ng;
    S16 Pb;

    U16 chanId[MAXCHSUBFRAME];
    U16 channelType[MAXCHSUBFRAME];
    S16 phyChanScale_factor[MAXCHSUBFRAME];

    S16 transmissionMode[MAXCHSUBFRAME];
    S16 numCodeWords[MAXCHSUBFRAME];
    S16 numTxLayers[MAXCHSUBFRAME];
    S16 numberofEntries[MAXCHSUBFRAME];
    S16 codeBookIdx[MAXCHSUBFRAME];
    S16 cddType[MAXCHSUBFRAME];
    U16 nRNTI[MAXCHSUBFRAME];

    RESELEMINFO reselmInfo[MAXCHSUBFRAME][MAXMAPCNT];

    S16 modType0[MAXCHSUBFRAME]; //modType for codeWord 0
    S16 Qm0[MAXCHSUBFRAME];
    S16 rV0[MAXCHSUBFRAME];
    U32 G0[MAXCHSUBFRAME];
    U16 mcsType0[MAXCHSUBFRAME];

    S16 modType1[MAXCHSUBFRAME]; //modType for codeWord 1
    S16 Qm1[MAXCHSUBFRAME];
    S16 rV1[MAXCHSUBFRAME];
    U32 G1[MAXCHSUBFRAME];
    U16 mcsType1[MAXCHSUBFRAME];

    //Variables below, each user can have 14 unique values
    U32 Borrowed0[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];       //for codeword 0
    U32 length_b0[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];    //for codeword 0
    U32 length_pb0[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];   //for codeword 0
    U32 Nsubc0[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];       //for codeword 0
    U32 M0symb[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];       //for codeword 0
    U16 nrow_x0[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];      //for codeword 0
    U16 ncol_x0[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];      //for codeword 0
    U16 ncol_y0[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];      //for codeword 0
    U32 pbIndex0[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];     //for codeword 0

    U32 Borrowed1[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];       //for codeword 1
    U32 length_b1[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];    //for codeword 1
    U32 length_pb1[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];   //for codeword 1
    U32 Nsubc1[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];       //for codeword 1
    U32 M1symb[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];       //for codeword 1
    U16 nrow_x1[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];      //for codeword 1
    U16 ncol_x1[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];      //for codeword 1
    U16 ncol_y1[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];      //for codeword 1
    U32 pbIndex1[MAXCHSUBFRAME * MAX_SYM_PER_SUBFRAME];     //for codeword 1

    S16 Ndl_rbPrs;
    S16 numRE_Prs;
    S16 PoSpilotScaleFactor;
    S16 PRS_EnableFlag;
    S16 DistributedExistFlag; // Common
    S16 TT;
    S16 DistributedFlag[MAXCHSUBFRAME];
    S16 NgapFlag[MAXCHSUBFRAME];
    U16 refSubFrame[MAXCHSUBFRAME];
    U16 refFrame[MAXCHSUBFRAME];
    U16 runSubFrCtr[MAXCHSUBFRAME];
    U16 ueCategory[MAXCHSUBFRAME];
    S16 txChanIdMappingTable[MAXCHSUBFRAME];

    U8* startPBCH;
    U8* ptrPBCH;
    U32 *pBchDataBuffer;

    S16* nVRB_Table;
    S16* CA_NumREinRBPairsMid6_Bcst_all;
    S16* CA_NumREinRBPairsMid6_Sync;
    S16* CA_NumREinRBPairsMid6_Bcst;
} DLTXINPUTPARAMS, *PDLTXINPUTPARAMS;

typedef struct Tx_Modulation_Params
{
    U32 *_totalsizescr;
    U32 *scrIndex1, *scrIndex2;
    U8 *scrOut1, *scrOut2;
    S16 *pncol_y;

    PTXMAPPERMULTICHANIN pIn_a;
    PTXMAPPERMULTICHANOUT pOut_a;
    PTXLAYERMAPPERMULTICHANOUT pOut_b;
    PTXPRECODERMULTICHANOUT pOut_c;
    PDLRESELEMMAPPERMULTICHANOUT pOut_d;
} TXMODULATIONPARAMS, *PTXMODULATIONPARAMS;

typedef struct TxSystemInfo
{
    TXSYSTEMINFOINNER a;
} TXSYSTEMINFO, *PTXSYSTEMINFO;

typedef struct TxSymChanBufDesc
{
    U16 bitFirst0;
    U16 bitFirst1;
    U16 size0;
    U16 size1;
} TXSYMBUF, *PTXSYMBUF;

typedef struct TxSymBufs
{
    U16 sizeLocRE;
    U16 sizeBuf0;
    U16 sizeBuf1;
    U16 locRE[1200];
} TXSYMBUFS, *PTXSYMBUFS;

typedef struct ltePhyBsCommonDlInitStruct
{
    /* pointer to BCCH data buffer - size of buffer = 1920 x 256 bits */
    U32 *pBchDataBuffer;
    /* pointer to CFI data buffer  */
    S16 *pCfiMemBuffer;
    /* pointer to resource map table for PCFICH */
    S16 *pCfiReMapBuffer;
    /* pointer to PHICH symbol buffer */
    S16 *pHIMemBuffer;
    /* pointer to resource map table for PHICH */
    S16 *pHIREMapTable;
    /* pointer to PDCCH permutation table */
    S16 *pPDCCHPermuteTable;
    /* pointer to length of PDCCH Quadruplets per symbol */
    S16 *pPDCCHQuadrupletsLen;
    /* pointer to resource map table for symbol0 - PDCCH*/
    S16 *pPDCCHReMapTable;
    /* pointer to memory to save DL cell specific reference seq */
    S16 *pDLCellSpecificRefSeq;
    /* pointer to memory to save DL control scrambling seqs */
    U8 *pDLCellSpecificScrSeq;

}BS_DL_INIT_MEMBUF, *PBSDLINITMEMBUF;

typedef struct ltePhyBsDlInitParamStructure
{
    /* cell Id */
    S16     cellId;
    /* Downlink Bandwidth (from MIB so goes from 0-5, should use enum)*/
    S16     dlBandwidth;
    /* Cyclif prefix type (should be enum) */
    S16     cyclicPrefixType;
    /* phich resource (from MIB should be enum?) */
    S16     phichResource;
    /* phich duration (from MIB should be enum?) */
    S16     phichDuration;
    /* number of Base-station transmit antenna port (device/system configuration)*/
    S16     numBsTxAntPort;
    /* number of RB in the DL (derived from MIB's dl bandwidth - needed here?)*/
    S16     numDlRb;
    /* number of PHICH group */
    S16     numPHICHGrp;
} BS_DL_INIT_PARAM, *PBSDLINITPARAM;

typedef struct BS_Tx_Info
{
    U16 Ntx_antennas;
    U16 subframeNumber;

    // nDLRb is the number of resource elements per slot as a function of BW in the
    // downlink direction(Ndl_rb)
    U16 Ndl_rb;

    // nDlSymb is the number of Downlink Symbols per slot it is derived from
    // cpType and the nRbSc as follows cpType=0 then nDlSymb=7
    // cpType=1 and nRbSc=12 then nDlSymb=6 else nDlSymb=3 (Ndl_symb)
    U16 Ndl_symb;

    S16 RBIndexMid6Lo;
    S16 RBIndexMid6Hi;
    S16* sync2_d[2];
    S32 SyncTxAntennaNum;

    U8 *GenDLCramBuf;
    U32 GenDLCramBufSize;

    S16 row_a;
    S16 col_a;
    S16 *a;
    S16 *ifftOut[MAXNUMLAYERS];

    PTXSYSTEMINFO      *pTxSys;
    PTXSYMBUFS         *pTxSymBufs;
    PTXSYMBUF           pTxSymBuf[MAX_SYM_PER_SUBFRAME];
    PIOParams_DlControl pDlCntrl_TxInPars;
    PDLTXINPUTPARAMS    pTxInputParams;
    PBSDLINITMEMBUF     pBsDlInitMemBuf;    /* Pointer to DL memory structure for tables */
    PBSDLINITPARAM      pBsDlInitParam;     /* For common DL initalization parameters */

    // TCB and Task List
    U32 isExeTaskListReady;
    U32 txListId;
    U32 isListRunning;
    U16 NumTasksTotalTX;

    PTCB pTaskTx[MAX_NUM_TCB];
    PTCB pSetupBufsTCB;
    PTCB pDlControlTCB[4];
    PTCB pModTCB[7];
    PTCB pIfftTCB[2];
    PTASKLIST pExeTaskList;
} BS_TX, *PBS_TX;

typedef struct BS_Info
{
    // CPType corresponds to the cyclic prefix type: 0 normal 1 extended
    // and is derived from cyclicPrefixType in the INITPARM info
    S16 CPType;
    S16 cellGroup;
    S16 cellSector;
    S16 profile;
    S16 N1_id;
    S16 N2_id;
    S16 Ncell_id;
    // frameDuplexType can be either FDD (Frame Type 1) or TDD (FrameType 2)
    U16 frameDuplexType;
    U16 BW;
    // Number of Subcarriers per slot as a function of BW or numberUsedSubcarriers
    U16 Nused;
    U16 n;
    // nRbSc number of subcarriers per Resource Block as a function of the
    // subcarrier spacing corresponds to physicalResourceBandwidth (Nrb_sc)
    U16 Nrb_sc;
    U16 Nsymb;
    // nFft FFT size as a function of Bandwidth corresponds to ffftSize (Nfft)
    U16 Nfft;
    // expoNfft is the power of 2 for the particular FFT base on Bandwidth
    // The only deviation is 15 MHz that requires a special algorithm that
    // performs decimation in time and splits the process into 3 512 FFT's and
    // a radix 3 stage
    S16 expoNfft;
    // Ncp number of samples in cyclic prefix, at the fft sample rate for all
    // symbols except the first one as a function of bandwidth and Cyclic Prefix
    U16 Ncp;
    // Ncp0 number of samples in cyclic prefix, at the fft sample rate for the
    // first symbol as a function of bandwidth and Cyclic
    U16 Ncp0;
    // nRb corresponds to the number of Resource Blocks as a function of the BW
    // so it is a value in the range of 6 to 100 corresponds to nMaxDlRb and
    // nMaxUlRb   (Nrb)
    U16 Nrb;
    // Fs sampling frequency as a function of the BW, it can be derived from a
    // table lookup from the samplingRateoverMHz (Fs) (In matlab it uses n but it
    // is not necessary to avoid division)
    U32 Fs;
    // df subcarrier frequency spacing it can be derived from
    // nRbSc if 12 then 15 KHz spacing else 7.5 KHz
    S16 baseRB;
    U16 df;
    S16 Tusym;
    S16 Tsam;
    S16 Tcp;
    S16 Tcp0;
    S16 Tsym;
    S16 Tsym0;
    S16 CarrierIndices[256];
    S16 GuardLowCarrierIndices[38];
    S16 GuardHighCarrierIndices[37];
    PBS_SRS pBsSrs;
    PBS_RX_PRACH pBsRxPrach[2];
    PBS_PRACH pBsRxPrachCram[2];
    PBS_RX_PRACHDet pBsRxPrachDet[2];
    S8 P_b;
} BS, *PBS;

typedef struct RateMatch_Info
{
    S32 E;
    S16 k0;
    S16 Ncb;
} RateMatchParms, *PRateMatchParms;

typedef struct RateMatchingParameters
{
    S32 E;
    S16 k0[4];
    S16 Ncb;
    S32 E2;     // Alternate E to be used when code block index exceeds NumCodeBlockUseE
    S16 NumCodeBlockUseE;   // Code block index limit for using E
} RMParameters, *PRMParameters;

typedef struct LtePhyULChanDescriptor
{
    // Start Resource Block Pair or CCE for this allocation
    U8 startRb;
    // Total Resource Block Pairs used in this allocation
    U8 totalRbUsed;
    // Channel defined flag, set for persistent entries. For dynamic entries
    // set after TXSTART req and clear after TXEND indication
    U8 chDefined;
    // Subframe number that marks the first time the allocation was defined
    U8 refSubFrame;
    // Frame number that together with the subframe number allows us to find
    // out if the allocation has expired or not
    U16 refFrame;
    // Running counter in subframes to determine time out and period
    U16 runSubFrCtr;
    // Storage for ri info
    U8 riStore;
    // Storage for Ack info
    U8 ackStore;
    // Number of bits for CQI during current sf
    U32 numbitsCQI;
    // Number of bits of RI during current sf
    U32 numbitsRI;
    // Number of bits for ACK/NACK during current sf
    U32 numbitsACK;
    // Stoarge for cqi info
    U8 cqiStore[32];
    // scTxRbInfo contains the number of pair of RB's used for codeword0 and
    // codeword1 and are updated during txSdu processing
    U16 scTxRbInfo[2];
    // pFecTxOut is an array of pointers for each subchannel Fec Block Output
    U8* pFecTxOut[2];
    // fecOutBits is an array with the number of output bits present
    // in the tx FEC out(Expected ie G0,G1 in Matlab code)
    U32 fecInOutBits[2];
    // Number of bytes received for each codeword from the MAC
    U32 numBytes[2];
    // pFecRxIn is an array of pointers for each subchannel receive Fec In
    U8* pFecRxIn[2];
    // RbMap is used for debugging purposes as it is easy to visualize the
    // Pair of resource blocks mapping
    // This is a bit map for RB's allocation for this channel, it is the result
    // of processing the allocations of both subchannels
    // The bit map is as follows
    // RbMap[0] RB0, RB1,..., RB31
    // RbMap[1] RB32,......., RB63
    // RbMap[2] RB64,.........RB95
    // RbMap[3] RB96.. RB99
    // Although 36.211 and 36.213 have 110 as maximum number of RB's in certain
    // tables, the 36.804 document that specifies the BS Tx & Rx only requires
    // 100 RB's for 20 MHz Bandwith, 110 corresponds to 22 MHz BW which it
    // is not currently listed as a required BW
    U32 RbMap[4];
    //    SUBCHRXINFO scRxInfo[2];
    U32 rxBits[2];
    S16 *d0IQ[MAX_SYM_PER_SUBFRAME];
    S16 *d1IQ[MAX_SYM_PER_SUBFRAME];
    S32 length_d0IQ[MAX_SYM_PER_SUBFRAME];
    S32 length_d1IQ[MAX_SYM_PER_SUBFRAME];
    S32 M0symb[MAX_SYM_PER_SUBFRAME];
    S32 M1symb[MAX_SYM_PER_SUBFRAME];
    S16 *x_layer[MAX_SYM_PER_SUBFRAME];
    S16 *y_precoder[MAX_SYM_PER_SUBFRAME];
    S32 nrow_x[MAX_SYM_PER_SUBFRAME];
    S32 ncol_x[MAX_SYM_PER_SUBFRAME];
    S16 ncol_y[MAX_SYM_PER_SUBFRAME];
    S16 phyChanScale_factor;
    S16 phyChanPower;
    S16 l_k_Nsc[MAX_SYM_PER_SUBFRAME];
    S16 *l_k_Isc[MAX_SYM_PER_SUBFRAME]; // old K_L in reverse
    S32 pBit[2*MAX_SYM_PER_SUBFRAME]; // to accomodate 2 codewords
    ULCHANDESC chi;

    S16 Itbs[2];   // 3GPP TS 36.213 Table 7.1.7.2.1-1 Transport block size table index (0 - 27)
    S16 Nprbm1[2]; // 3GPP TS 36.213 Table 7.1.7.2.1-1 Transport block size table Nprb-1 (0 - 109)

} ULCHDESC, *PULCHDESC;

// The following structures are partly based on information contained in
// matlab file phy_bs_config.m (The matlab variables are stated in
// parenthesis in the comments

typedef struct LtePhyDescriptor
{
    // Pointer to Tx IQ Samples storage indexed by layer
    S16* sigOut[MAXNUMLAYERS];
    // Pointer to Rx IQ Samples storage indexed by layer
    S16* sigIn[MAXNUMLAYERS];
    // txFrameNumber
    U16 txFrameNumber;
    // rxFrameNumber
    U16 rxFrameNumber;
    //  dacTxScale corresponds to the scale factor for the Tx IQ Samples
    S16 dacTxScale;

    U32 confEnableMask; // API confirmations (TXSDU, TXSTART, RXSTART, )

    PBS pBs;
    PBS_TX pBsTx;
    PBS_RX pBsRx;

    S8  *descrInBufs;
    S8  *genBuf;
    U32 *descrIndex;
    U32 *demapOutSize;

    PHARQS pharq;
    PBS_RX_RNTI_MAPPING pRntiMapping;
} LTEPHYDESC, *PLTEPHYDESC;

// Array of Pointers to LTEPHYDESC indexed by the InstanceID
//extern PLTEPHYDESC PhyInst[MAXNUMPHYINSTANCES];

typedef struct ChEntry
{
    U32 fecInOutBits[2];
    U32 numBytes[2];
    U32 numRxAnts;
} CHENTRY, *PCHENTRY;

typedef struct RcvrIODescriptor
{
    CHENTRY chentry[LTEMAXCHAN];
} RXIODESC, *PRXIODESC;


typedef struct tIOParams_SetupIFFT_Bufs
{
    S16* ifft_in;
    S16* a;
    S16 row_a;
    S16 Nfft;
    S16 col_a;
    S16 isym;
    S16 mi;
    S16 Ncpsam;
} IOParams_SetupIFFT_Bufs, *PIOParams_SetupIFFT_Bufs;

typedef struct tIOParams_AddCP
{
    S16* ptr_dac_samp;
    S16* ifft_out;
    S16 Nfft;
    S16 Ncpsam;
} IOParams_AddCP, *PIOParams_AddCP;


//  FFT IO params
typedef struct tag_IO_FFTParams
{
    S16 Exp;
    U16 FFT_Order;
    S16* ExpAddr;
    S16 isym;
    S16 slotNum;
    S16 num_sym;
    S16 numSubCar;
    S16 CPType;
    S16 numTxAnt;
    S16 Ncp0;
    S16 Ncp;
} IOParams_FFT, *PIOParams_FFT;

typedef struct tIOParams_SetupBufs
{
    U32 isymstart;
    U32 isymstop;
    U32 ichanStart;
    U32 numChan;
    U16 Ntx_antennas;
    S16 RBIndexMid6Lo;
    S16 RBIndexMid6Hi;
} IOParams_SetupBufs, *PIOParams_SetupBufs;

////////////////////////////////////////////
// Structures for PHY Diagnostics

typedef struct tMapMEM
{
    char varName[80];
    void *varPtr;
    U32 varSize;
    U16 varType;
    U16 memId;

} MAPMEM, *PMAPMEM;

typedef struct tSizeofMEM
{
    U32 sizeofCRAMdouble;
    U32 sizeofCRAMs32;
    U32 sizeofCRAMu32;
    U32 sizeofCRAMs16;
    U32 sizeofCRAM;
    U32 sizeofCRAM_EstFx;

    U32 sizeofDDRdouble;
    U32 sizeofDDRs32;
    U32 sizeofDDRu32;
    U32 sizeofDDRs16;
    U32 sizeofDDR;
    U32 sizeofDDR_EstFx;

    U16 MapMEMIndex;
    MAPMEM MapMEM[100];

} SIZEOFMEM, *PSIZEOFMEM;


typedef struct tLTE_PHY_UL_SHARED_PARAMS
{
    U32* pFecUlStart;
    U32* pMdmaStart;
    U32 IndexMap[MAXCHSUBFRAME];
    U32 size[MAXCHSUBFRAME];
    U8* fecOut[MAXCHSUBFRAME];
    U8* mdmaOut[MAXCHSUBFRAME];
}LTE_PHY_UL_SHARED_PARAMS, *PLTE_PHY_UL_SHARED_PARAMS;

////////////////////////////////////////////

#endif /* LTEPHY_H */
