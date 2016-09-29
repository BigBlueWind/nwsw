//-------------------------------------------------------------------------------------------
/** @file ltephydef.h
 *
 * @brief This file defines constants and the external functions and variables
 *        used in the implementation of an LTE PHY
 * @author Mindspeed Technologies
 * @version $Revision: 1.126 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef LTEPHYDEF_H
#define LTEPHYDEF_H

#include "LtePhyL2Api.h"

#define RX_DEP_FFT_INIT_VAL         0x11112222
#define RX_DEP_IDFT_INIT_VAL        0x33334444
#define NUM_IDFT_CELL_DEPENDENCY    6
#define SLOT_BASED_FFT
//#define IDFT_CELL_DEPENDENCY

//#define TEST_CEVA_PROTECTION 1
//#define TEST_DLFEC_PROTECTION 1
//#define TEST_ULFEC_PROTECTION 1
//#define TEST_MDMA_PROTECTION 1

#ifndef _WIN32
//#define DEBUG_POWER_RX_SIGNAL 1
#endif

#define CONTEXT_P_PHY 1
#define CONTEXT_PLTE_PHY_UL_SDU_DESC 2

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define COMMON_MEM  0
#define DDR_MEM     1
#define CEVA_DMEM   1       //!!!!!!Probably best to remove this! Define variable inside function with maxm array dimensions. Remove definition from RxPtr malloc/free stuff.
#define IRAM_MEM    2
#define CRAM_MEM    3
#define NCNB_MEM    4
#define DDR1_MEM    5
#define DDR1_NCNB_MEM   6
#define PHY_MEM     7
//#define CACHEABLE   DDR_MEM
#define CACHEABLE   PHY_MEM

#define MAX_PHICH   50

#define TYPE_U8           0x00
#define TYPE_U16          0x01
#define TYPE_U32          0x02
#define TYPE_DOUBLE       0x03
#define TYPE_S16          0x04
#define TYPE_S32          0x05
#define TYPE_S8ASDOUBLE   0x10    //for variable definition where, will use S8 in fixed point, double in floating point
#define TYPE_S16ASDOUBLE  0x11    //for variable definition where, will use S16 in fixed point, double in floating point
#define TYPE_S32ASDOUBLE  0x12    //for variable definition where, will use S32 in fixed point, double in floating point
#define TYPE_POINTER      0x18      //assume pointers to all data types are stored as 32-bits!
#define TYPE_STRUC        0x20

#define TCB_NAME   0x10     //!!!!!!!!!!!! where is best place for this?
#define IN_CONTEXT 0x11

#define TYPE_REAL       0x00
#define TYPE_COMPLEX    0x01

#define BASESTATION    0
#define MOBILESTATION  1

#define         MAX_OUTPUT_BLK_SIZE 100800 // Tracing
#define         MAX_INPUT_TRANSPORT_BYTE_SIZE 12600
#define         MAX_OUTPUT_TRANSPORT_BIT_SIZE (8*MAX_INPUT_TRANSPORT_BYTE_SIZE)
#define         MAX_NUM_CODE_TRANSPORT_SIZE 17
#define         NUM_PARITY 24
#define         MAX_SUB_CODE_SIZE 6144
#define         NUM_TC_STREAMS 3
#define         CC_MEMORY 3
#define         MAX_ROW_RBASSIGNMENT 64
#define         MAX_COL_RBASSIGNMENT  2
#define         ROW_K_L 2
#define         COL_K_L 16000
#define         MAX_NUMBER_FILL_BITS 64
#define         NUM_SLOTS            20
#define         NUM_SLOTS_SUBFRM      2
#define         NUM_SLOTS_PER_SUBF   2
#define         N_RB_SC     12      //number of subcarriers per RB
#define         MAX_PUSCH_DATASYMB_PER_SUBF     12
#define         MAX_PUCCH_DATASYMB_PER_SUBF     12  //formats 2a, 2b, where data is sent on 2nd pilot. Used for calculating memory offsets in linear arrays
#define         MAX_PUCCH_PILOT_SYMS             3  //Maximum number of pilot symbols for PUCCH
#define         NUM_CQI_DATASYMB_PER_SUBF       10  //pucch formats 2, 2a, 2b. Always 10 CQI data symbol values per subframe
#define         NUM_CQI_DATASYMB_PER_SLOT        5  //pucch formats 2, 2a, 2b. Always 5 CQI data symbol values per slot
#define         MAX_NUM_ANT 4   //maximum number of antennas
#define         MAX_NUM_RB 100  //maximum number of resource blocks (100 for 20MHz frequency band)
#define         MAX_SYM_PER_SUBFRAME            14
#define         MAX_PUSCH_DATASYMB_PER_SUBF     12  //normally 12, 11 in subframes with SRS
#define         MAX_PUCCH_PILOTSYMB_PER_SUBF    6
#define         MAX_PUCCH_PILOTSYMB_PER_SLOT    3

// Define number of hardware channel de-interleaver columns.
#define         P_DEINT_COL             12

//Define max sizes for soft decision buffers for multiplex control in PUSCH
#define         NUM_SYM_ACK_RI          4       //Number of symbols having RI or ACK constellation points
#define         MAX_MUXCTRL_SOFT_DEC    95136   //(MAX_RBs)*(QAM64)*(N_RB_SC)*MAX_PUSCH_DATASYMB_PER_SUBF +
                                                //(3*RUP32B(MAXCHSUBFRAME)) Rounding of numBitsACK, numBitsRI, numBitsCQI

#define         PI_FX_BY2 12868
#define         PI_FX 25736

#define CRC24A        3      //number of bytes in CRC24A
#define CRC16         2      //number of bytes in CRC16
//For now, set maximum number of dependencies between tasks to be 64
//We can increase if it is needed.
#define MAX_DEPENDENCIES    64

#define         QUADRUPLET 4
#define         N_PER_ROW 6
#define         INV_6 5461
#define         INV_12 2731
#define         NUM_BITS_PUCCH_F2_PER_SYM 24 // number of bits for Format2,2B
#define         NUM_BITS_PUCCH_F2A_PER_SYM 12 // number of bits for Format2A
#define         MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT 19000 // bytes for 20MHz 100 users per antenna
#define         MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT_C 90000 // bytes for 20MHz 100 users per antenna.

#define LTE_POWER_DB_BIAS 768   // (0 * 256 where 256 corresponds to 1 db)

#define LTEMAXIQ  4*31720*4  // 4 antennas, 31720 samples, 4 bytes per sample

// This is bigger as the PBCH is 1 byte per bit in C-Only.

// Constant definitions to be used throughout the LTE PHY
// MAXNUMPHYINSTANCES corresponds to the maximum number of supported PHY
// instances
#define MAXNUMPHYINSTANCES 4
#define MAX_DL_TASK_LISTS_PER_SF 2
#define MAX_UL_TASK_LISTS_PER_SF 2

#define NUM_USER_PUCCH_TCBS     4
#define NUM_PUCCH_PER_TCB       ((MAXCHSUBFRAME + NUM_USER_PUCCH_TCBS - 1)/NUM_USER_PUCCH_TCBS)
#define STAT_SUCCESS             3
#define STAT_PUCCH_DEADZONE      4

#ifdef RT_API_PROC
#define MAX_NUM_OF_SF_CTX 2
#else
#define MAX_NUM_OF_SF_CTX 1
#endif

#define NUM_UL_SDU_CONTEXT                       4
#define PUSCH_LIST_TYPE                          0
#define PUCCH_LIST_TYPE                          1
#define OTHER_LIST_TYPE                          2


#define RXSDU_LIST_STATE__CLEAN                 (0)
#define RXSDU_LIST_STATE__PUSCH_PRESENT         (1<<0)
#define RXSDU_LIST_STATE__PUCCH_PRESENT         (1<<1)
#define RXSDU_LIST_STATE__PUSCH_SENT            (1<<2)
#define RXSDU_LIST_STATE__PUCCH_SENT            (1<<3)

#define IncrementSduCtx(RxSduCtx)   ((RxSduCtx == NUM_UL_SDU_CONTEXT-1) ? 0 : (RxSduCtx+1))
#define GetPrevSduCtx(RxSduCtx)     ((RxSduCtx == 0) ? (NUM_UL_SDU_CONTEXT-1) : (RxSduCtx-1))
#define GetNextSduCtx(RxSduCtx)     ((RxSduCtx == NUM_UL_SDU_CONTEXT-1) ? (0) : (RxSduCtx+1))


#define CheckSyncState(a,b)         ((a&b)?0:1)     // If a&b > 0, then false, else true
#define CalcMcs(Itbs, Qm)           (Itbs + ((Qm >> 1) - 1))


#define LteBsConstructSfn(frame, subFrame) ((frame * 10 + subFrame))
#define LteBsIncrementSfn(sfn) (((sfn >= 10239) ? 0 : sfn+1))
#define LteBsDecrementSfn(sfn) (((sfn == 0) ? 10239 : sfn-1))
#define LteBsGetFrameNum(sfn) ((sfn / 10))
#define LteBsGetSubFrameNum(sfn) ((sfn % 10))

#define LteBsIsCqiPmi(a) ((a > 2) ? 1 : 0)



#define UL_TASKLIST_START_IDX           0
#define DL_TASKLIST_START_IDX           2
#define PRACH_TASKLIST_START_IDX        4
#define SRS_TASKLIST_START_IDX          8

#define NMM_NUM_LIST                    5
#define MAX_NMM_NUM_TCB_PER_LIST        100
#define NMM_TASKLIST0_IDX               16
#define NMM_TASKLIST1_IDX               17
#define NMM_TASKLIST2_IDX               18
#define NMM_TASKLIST3_IDX               19
#define NMM_TASKLIST4_IDX               20

#define PID_TASKLIST_EXEC_NMM_0           (20000+NMM_TASKLIST0_IDX)
#define PID_TASKLIST_EXEC_NMM_1           (20000+NMM_TASKLIST1_IDX)
#define PID_TASKLIST_EXEC_NMM_2           (20000+NMM_TASKLIST2_IDX)
#define PID_TASKLIST_EXEC_NMM_3           (20000+NMM_TASKLIST3_IDX)
#define PID_TASKLIST_EXEC_NMM_4           (20000+NMM_TASKLIST4_IDX)

/*UMTS NWL Task list ID*/
#define  UMTSNMM_TASKLIST_ID_START          0
#define   TCB_PSCH_TSKLSTID                 (UMTSNMM_TASKLIST_ID_START+1 )
#define   TCB_SSCH_TSKLSTID                 (UMTSNMM_TASKLIST_ID_START+2 )
#define   TCB_CPICH_TSKLSTID                (UMTSNMM_TASKLIST_ID_START+3 )
#define   TCB_PM_TSKLSTID                   (UMTSNMM_TASKLIST_ID_START+4 )
#define   TCB_RSCP_TSKLSTID                 (UMTSNMM_TASKLIST_ID_START+5 )
#define   TCB_GEN_PSC_TASKLSTID             (UMTSNMM_TASKLIST_ID_START+6 )     
#define   TCB_FREQ_TSKLSTID                 (UMTSNMM_TASKLIST_ID_START+7 )
#define   TCB_DOWN_SAMPLE_TSKLSTID          (UMTSNMM_TASKLIST_ID_START+8 )
#define   TCB_AGC_TSKLSTID                  (UMTSNMM_TASKLIST_ID_START+9 )
#define   PATHSEARCH_TASK_LIST_ID           (UMTSNMM_TASKLIST_ID_START+10)
#define   UPDATE_RE_FRE_ERR_TASK_LIST_ID    (UMTSNMM_TASKLIST_ID_START+11)
#define   CLDEMODE_TASK_LIST_ID             (UMTSNMM_TASKLIST_ID_START+12)
#define   CLDECODE_TASK_LIST_ID             (UMTSNMM_TASKLIST_ID_START+13)
#define   INTERPOLATION_TASK_LIST_ID        (UMTSNMM_TASKLIST_ID_START+14)
#define   CLChEC_TASK_LIST_ID               (UMTSNMM_TASKLIST_ID_START+15)
#define  UMTSNMM_TASKID_END                 (UMTSNMM_TASKLIST_ID_START+16)



#define PID_TASKLIST_EXEC_UMTSNMM_0           (30000+TCB_PSCH_TSKLSTID)
#define PID_TASKLIST_EXEC_UMTSNMM_1           (30000+TCB_SSCH_TSKLSTID)
#define PID_TASKLIST_EXEC_UMTSNMM_2           (30000+TCB_CPICH_TSKLSTID)
#define PID_TASKLIST_EXEC_UMTSNMM_3           (30000+TCB_PM_TSKLSTID)
#define PID_TASKLIST_EXEC_UMTSNMM_4           (30000+TCB_FREQ_TSKLSTID)
#define PID_TASKLIST_EXEC_UMTSNMM_5           (30000+TCB_RSCP_TSKLSTID)
#define PID_TASKLIST_EXEC_UMTSNMM_6           (30000+TCB_AGC_TSKLSTID)
#define PID_TASKLIST_EXEC_UMTSNMM_7           (30000+TCB_DOWN_SAMPLE_TSKLSTID)
#define PID_TASKLIST_EXEC_UMTSNMM_8          (30000+TCB_GEN_PSC_TASKLSTID)
#define PID_TASKLIST_EXEC_UMTSNMM_9           (30000+PATHSEARCH_TASK_LIST_ID)
#define PID_TASKLIST_EXEC_UMTSNMM_10           (30000+UPDATE_RE_FRE_ERR_TASK_LIST_ID)
#define PID_TASKLIST_EXEC_UMTSNMM_11           (30000+CLDEMODE_TASK_LIST_ID)
#define PID_TASKLIST_EXEC_UMTSNMM_12           (30000+CLDECODE_TASK_LIST_ID)
#define PID_TASKLIST_EXEC_UMTSNMM_13           (30000+INTERPOLATION_TASK_LIST_ID)
#define PID_TASKLIST_EXEC_UMTSNMM_14           (30000+CLChEC_TASK_LIST_ID)



#define INIT_MODE       0
#define LTE_MODE        1
#define NMM_MODE        2
#define UMTS_NWL_MODE   3

//#define MAX_SDU_PER_SUBFRAME  19

// MAXNUMLAYERS corresponds to the maximum number of layers supported
// for MIMO configurations
#define MAXNUMLAYERS 4
// Maximum number of channels per subframe
//#define LTEMXSFCH 40
// LTEMAXCHAN is the maximum number of channels supported per sector
#define LTEMAXCHAN 400

// PHY Types definitions
// Base Station LTE PHY
#define BS_PHY 0
// Mobile Station LTE PHY
#define MS_PHY 1
// Carrier Spacing Options
#define CS_7500 7500
#define CS_15K  15000
// Reference number of Resource Elements per PRB pair to be used for
// Shared Data Channel allocations and Transport Block Size determination
// per R1-081284 (assumes 3 symbols for control info, 2 Antennas,
// No sync signals, No Pbch, Normal CP
#define RefRePerPrbPair 120
#define RefScPerRb 12
#define RefSymPerRb 5
// Maximum Bits Available for TB Transmission need to remove and
// compute on the fly from the TX SDU size
//#define G 6048

#define MAX_ULPILOTS_U      30
#define MAX_RM_DECODER_TABLE_SIZE   1495
#define MAX_UL_SRS  8

#define MAX_PDCCH_BUFFER_SIZE  (900)
#define MIN_SRS_RB  4

#define FAILURE 1

#define UNCODED_OUT     1
#define FEC_DEC_OUT     2
#define PROFILE_INFO    3
#define IQ_SAMPLES      4
#define SRS_CHEST_OUT   5
#define PRACH_OUT       6
#define MLOG_OUT        7
#define MLOG_SIZE       8
#define SVSR_OUT        9
#define SVSR_SIZE       10
#define TIMINGAVG_OUT   11
#define MAC_SDU_OUT     12
#define MAC_SDU_SIZE    13
#define PHYSTATS_SIZE   14
#define PHYSTATS_OUT    15
#define INV_MCS         255

//Grid search for ML algorithm for time offset
#define TO_GRID_SEARCH_SPACE    11

/* Threshold for switching the transient to steady state */
#define THRESH_TO_NUM_RB_AVG    30

#define SCHEDULE_REQUEST 0

// Bypass check for proper Transport Block Sizes from MAC Layer
#define TEST_FORCING_ANY_TBSIZE


#define DEFAULT_RSSI   (0)


//*****************************************************************************
//**********                   UPLINK HARQ COMBINING                 **********
//*****************************************************************************

#define P_FEC_DEINT_SCF         29491   // .9

// Define number of HARQ processes.
#define P_HARQ_PROC     8

// Define maximum number of transmissions allowed to be combined for
// each HARQ process.
#define P_HARQ_COMB     4

// Define maximum number of soft decisions that can be generated in one
// HARQ interval (1 subframe).  The calculation is as follows:

//      P_HARQ_SFDEC = (110*12 subcarriers/symbol)*(12 symbols/subframe)*
//                     (6 bits/subcarrier)

//      P_HARQ_SFDEC = 95040

// To ensure fastest memory access, P_HARQ_SFDEC%32 must be 0.

#define P_HARQ_SFDEC     95040

// Define maximum number of HARQ users (UEs).
#define NUM_CONNECTED_USERS  80                     // Number of Unique Connected Users
#define P_HARQ_USERS    NUM_CONNECTED_USERS

//*****************************************************************************
//**********               UPLINK WEIGHTED HARQ COMBINING            **********
//*****************************************************************************

// Control parameter USE_WEIGHTED_HARQ_COMB is used to enable (=1) or disable
// (=0) weighted HARQ combining.  Currently the use of weighted harq combining
// is only supported in the Rossetti device.

#define USE_WEIGHTED_HARQ_COMB  0   // Enable weighted HARQ combining

//*****************************************************************************
//**********                  ROSSETTI DEVICE OPTION                 **********
//*****************************************************************************

// Originally, this PHY code was designed to run on a Turner device.  However,
// now a newer device (Rossetti) is being designed that will incorporate some
// enhanced features such as weighted HARQ combining.  Control parameter
// USE_ROSSETTI_DEV is used to select (=1) or not select (=0) the new Rossetti
// device.

#define USE_ROSSETTI_DEV    0       // Use new Rossetti device

//*****************************************************************************
//**********            FLOATING POINT CTC DECODER OPTION            **********
//*****************************************************************************

// For Turner the Turbo Concept's CTC decoder is used in the FEC.  In the
// C simulations function tc1700decode(), which is a bit-exact version of this
// decoder, is normally used.  However, for the floating point simulations
// a new floating point CTC decoder function (FP_CTC_Decoder) can be used
// instead.  Control parameter USE_FP_CTC_DECODER is used to select (=1) or not
// select (=0) this new decoder.

#define USE_FP_CTC_DECODER  0       // Use new flaoting point CTC decoder

//*****************************************************************************
// Define length of table defining allowable block sizes.
#define P_BLKSIZ_TAB_LEN    188

#ifdef WIN32
#define NCNBVAR
#define CRAMVAR
#endif

#ifdef __CC_ARM
#define DATA_ALIGN(x)    __align (x)
#else
#define DATA_ALIGN(x)
#endif

#endif /* LTEPHYDEF_H */
