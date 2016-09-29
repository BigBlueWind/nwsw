// -------------------------------------------------------------------------------------------
/** @file lte_bs_phy_nmm_fdd_lib.c
 *
 * @brief This file implements the function for an LTE FDD NMM 
 * @author Mindspeed Technologies
 * @version $Revision: 1.1.4.1.2.27 $
 *
 * COPYRIGHT&copy; 2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
// -------------------------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#include <asm-insn.h>
#include "global.h"
#endif

#include "typedef.h"
#include "ltephydef.h"
#include "ext_phy.h"
#include "lte_bs_phy_nmm_fdd.h"
#include "lte_bs_phy_nmm_tables.h"
#include "prototypes.h"

#ifndef CEVA_INTRINSICS
#include "basic_op.h"
#include "IDFT.h"
#endif

#ifdef _WIN32
#include "Appids.h"
#include "svsrlogger_c_code.h"
#endif

#ifdef __CC_ARM
#include "svsrlogger_c_code.h"
#endif

#if NMM_CEVA_DBG
char           *arm_dbg;
extern char    *ceva_dbg;
#endif

//#ifdef __CC_ARM
#if 0

#define __ASM_BEG __asm {
#define __ASM_END }

static __inline Word32
sub_armv6(Word16 var1, Word16 var2)
{
#pragma push
#pragma O0
    Word32          L_diff;

__ASM_BEG
    SUBS L_diff,var1,var2
    SSAT L_diff, 16, L_diff
__ASM_END
    return (L_diff);
#pragma pop
}

static __inline Word32
L_mult_armv6(Word16 var1, Word16 var2)
{
    Word32          r0;

__ASM_BEG
    SMULBB r0, var1, var2
    QADD   r0, r0,r0
__ASM_END
    return r0;
}

static __inline Word32
L_mac_armv6(Word32 L_var3, Word16 var1, Word16 var2)
{
    Word32          L_product;

__ASM_BEG
    SMULBB L_product, var1, var2
    QDADD  L_product, L_var3,L_product
__ASM_END
    return (L_product);
}

#pragma push
#pragma O0

static __inline Word32
L_shl_armv6(Word32 in, Word32 shift)
{
    Word32          out,
                    tmp;

    tmp = 0x7fffffff;
__ASM_BEG
    MOV out, in, LSL shift
    TEQ in, out,ASR shift
    EORNE out,tmp,in,ASR#31
__ASM_END
    return out;
}

#pragma pop

#define sub_opt sub_armv6
#define L_mult_opt L_mult_armv6
#define L_mac_opt L_mac_armv6
#define L_shl_opt  L_shl_armv6

#else
#define sub_opt sub
#define L_mult_opt L_mult
#define L_mac_opt L_mac
#define L_shl_opt  L_shl
#endif


#define NMM_DWNSAMPL_HARDCODED_AT_10MHZ  1

void nmm_runTaskList(U32 listNum);


#ifdef CEVA_INTRINSICS

extern U8      *DataBuff_Bank1;
extern U8      *DataBuff_Bank2;
extern U8      *DataBuff_Bank3;

#else

/* to convert the BW in resource blocks, into index in tables (we should check if there is no similar method somewhere else) */
extern void lte_bs_phy_nmm_byPassMultTcb (TCB ** ppTCB, UINT32 nTcbNum, UINT32 nCtrl);

U8              MIB_BW_TO_RB[6] = { NMAXRB_6, NMAXRB_15, NMAXRB_25, NMAXRB_50, NMAXRB_75, NMAXRB_100 };

/* transport block size for DL-SCH (book table 171 , only 3 first columns, and only up to 9 line (up to TBS=8, i.e., restricted to what we need)) */
static S16      TBS_TABLE[27][2] = {     //
{32,56},
{56,88},
{72,144},
{104,176},
{120,208},
{144,224},
{176,256},
{224,328},
{256,392},
{296,456},
{328,504},
{376,584},
{440,680},
{488,744},
{552,840},
{600,904},
{632,968},
{696,1064},
{776,1160},
{840,1288},
{904,1384},
{1000,1480},
{1064,1608},
{1128,1736},
{1192,1800},
{1256,1864},
{1480,2216}
};

static U16 TBS_TABLE_1C[32]={
    /*0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15*/
40,56,72,120,136,144,176,208,224,256,280,296,328,336,392,488,
/*16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31*/
552,600,632,696,776,840,904,1000,1064,1128,1224,1288,1384,1480,1608,1736};

/* number of resource allocation bits depending on BW, in DCI 1A */
static S16      DCI_1A_RB_ALLOCATION_BITS[6] = { 5, 7, 9, 11, 12, 13 };


/* number of resource allocation bits depending on BW, in DCI 1C */
static S16      DCI_1C_RB_ALLOCATION_BITS[6] = { 3, 5, 7, 7, 8, 9 };

static U16      N_gap_LUT[2][N_BW_CASE]={{3,8,12,27,32,48},{0,0,0,9,16,16}};
static U16      N_a_DL_VRB_LUT[2][N_BW_CASE] = {{3,7,12,11,16,24},{0,0,0,9,16,24}};
static U16      inv_N_a_DL_VRB_LUT[2][N_BW_CASE]={{10923,4682,2731,2979,2048,1366},
                {0,0,0,3641,2048,1366}};

static U16      N_t_DL_VRB_LUT[2][N_BW_CASE]={{6,14,24,46,64,96},
                {0,0,0,18,32,32}};
static U16      inv_N_t_DL_VRB_LUT[2][N_BW_CASE]={{10923,4682,2731,1425,1024,683},
                {0,0,0,7282,4096,4096}};

static U16      DVRB_N_row_LUT[2][N_BW_CASE]={{2,4,6,12,16,24},{0,0,0,6,8,8}};

#if NMM_BET == 1
S16             ref_PSync_f[2 * FFT_SIZE_096MHZ];
S16             ref_SSync_f[2 * FFT_SIZE_096MHZ];
S16             ref_SSyncExp2,
                ref_PSyncExp2;
S16             ref_SSyncEst[2 * FFT_SIZE_096MHZ];
S16             ref_pci;
S16             ref_subframe;
#define NMM_BET_CHECK_FFT(x) nmm_bet_check_fft x
#define NMM_BET_CHECK_CHANEST(x) nmm_bet_check_chanest x
#define NMM_BET_CHECK_CHANEST_HSS(x) nmm_bet_check_chanest_hss x
#define NMM_BET_CHECK_DECODE_SSS(x) nmm_bet_check_decode_sss x
static void     lte_bs_phy_nmm_hbfir_state_upd(S16, S16, S16 *, S16 *);
static void     lte_bs_phy_nmm_hbfir(S16 *, S16 *, S16 *, S16 *);

#else
#define NMM_BET_CHECK_FFT(x)
#define NMM_BET_CHECK_CHANEST(x)
#define NMM_BET_CHECK_CHANEST_HSS(x)
#define NMM_BET_CHECK_DECODE_SSS(x)

#endif


#endif

extern PTASKLIST nmm_task_list[NMM_NUM_LIST];
extern PTCB      nmm_tcb_arr[NMM_NUM_LIST][MAX_NMM_NUM_TCB_PER_LIST];
extern U16       N_PBCH_TCB;
extern U16       N_PDSCH_tcb;

#ifdef FEC_FDESC_ENABLED

static volatile U32 fec_nmm_cc_stat;
#endif

#define SQRT_2  0x5a82 // in Q(16,2)
#define N_INT_SOFT_OUT 3
#define CRC_LEN 16              /* in bits */
#define CONST_1_3       (10923) //Q(16,1)
#define CONST_1_6 10923 //Q(16,0)
#define MAX_PDCCH_COLUMN    4
#define MAX_RS_SYMBOL_TIME  6
#define MAX_RS_SYMBOL       12
#define N_RS_SYMB_ANT0      4

#define QPSK_1    759250125     // 0x2D413CCD
#define QPSK_p5   379625063

#define  MAX_SYNC_CAND 10
S16            *PSync_f_arr[MAX_SYNC_CAND] = { 0 };
S16            *PSyncExp_arr[MAX_SYNC_CAND] = { 0 };
S32            *Psync_foe_arr[MAX_SYNC_CAND] = { 0 };

S16            *SSync0_f_arr[MAX_SYNC_CAND] = { 0 };
S16            *SSync0Exp_arr[MAX_SYNC_CAND] = { 0 };

S16            *SSync1_f_arr[MAX_SYNC_CAND] = { 0 };
S16            *SSync1Exp_arr[MAX_SYNC_CAND] = { 0 };

S16            *SSync2_f_arr[MAX_SYNC_CAND] = { 0 };
S16            *SSync2Exp_arr[MAX_SYNC_CAND] = { 0 };

S16            *SSync0Est_arr[MAX_SYNC_CAND] = { 0 };
S16            *SSync1Est_arr[MAX_SYNC_CAND] = { 0 };
S16            *SSync2Est_arr[MAX_SYNC_CAND] = { 0 };
S16            *pci0_arr[MAX_SYNC_CAND] = { 0 };
S16            *pci1_arr[MAX_SYNC_CAND] = { 0 };
S16            *pci2_arr[MAX_SYNC_CAND] = { 0 };

S16            *subframe0_arr[MAX_SYNC_CAND] = { 0 };
S16            *subframe1_arr[MAX_SYNC_CAND] = { 0 };
S16            *subframe2_arr[MAX_SYNC_CAND] = { 0 };

S16            *data_SSS_PSS_arr[MAX_SYNC_CAND] = { 0 };


/* need to be increased if several cells are detected @ the same time */
S16            *dl_pilot_arr[MAX_CELL_FOR_RSRP_SCAN * REF_SIGNAL_PER_SLOT * 2 * MAX_ANTENNA_FOR_NMM];
S16            *dl_pilot_exp_arr[MAX_CELL_FOR_RSRP_SCAN * REF_SIGNAL_PER_SLOT * 2 * MAX_ANTENNA_FOR_NMM];
S16            *dl_pilot_rsrp_arr[MAX_CELL_FOR_RSRP_SCAN * REF_SIGNAL_PER_SLOT * 2 * MAX_ANTENNA_FOR_NMM];

S16             sync_cand = 0;

S16             sync_cand_tcb_track;

/*
 * X7 workaround
 */
#define P_MIN_K     20
#define P_MAX_K     68
#define P_NUM_K     (P_MAX_K - P_MIN_K + 1)
extern S8       tmpsftdeca[3 * P_MAX_K];
extern S8       tmpsftdecb[3 * P_MAX_K * 2];
extern U8       tmpout[(P_MAX_K + P_MAX_K + 5) >> 3];

extern S16      CC_Knew_Table[P_NUM_K];
extern S16      CC_RM_Remapping_Start_Table[49];
extern S16      CC_RM_Remapping_Table[2412];

#define SWAP16(_v) ((((_v) & 0xFF00) >> 8) | \
                    (((_v) & 0x00FF) << 8))

#define SIB1_MAX_LEN16 346

#define PACK_2BITS(ptr) ((ptr)[0] << 1 | (ptr)[1])
#define PACK_3BITS(ptr) ((ptr)[0] << 2 | (ptr)[1] << 1 | (ptr)[2])
#define PACK_4BITS(ptr) ((ptr)[0] << 3 | (ptr)[1] << 2 | (ptr)[2] << 1 | (ptr)[3])
#define PACK_5BITS(ptr) ((ptr)[0] << 4 | (ptr)[1] << 3 | (ptr)[2] << 2 | (ptr)[3] << 1 | (ptr)[4])


#define SIB1_DECODE_DBG 0

#if SIB1_DECODE_DBG
#define PLMN_IDENTITY_LIST_MAX_SIZE 6
#define SUBFRAME_ASSIGNMENT_NBITS 3
#define SPECIAL_SUBFRAME_PATTERNS 4
#define Q_RX_LEV_MIN_MIN (-70)  /* q_rx_lev_min = (-70..-22) */
#define Q_RX_LEV_MIN_OFFSET_MIN 1       /* q_rx_lev_min_offset = (1..8) */
#define P_MAX_MIN (-30)         /* p_max (-30..33) */
#define FREQ_BAND_INDICATOR_MIN 1       /* freq_band_indicator (1..64) */

typedef struct {
    U16             mcc;
    U16             mnc;
    U8              cell_reserved_for_operator_use;
} plmn_identity_list_t;

typedef struct {
    U8              subframe_assignment[SUBFRAME_ASSIGNMENT_NBITS];
    U8              special_subframe_patterns[SPECIAL_SUBFRAME_PATTERNS];
} tdd_config_t;
#endif

#define BCCH_CHOICE_NBITS 2
#define SIB1_PREAMBLE_NBITS 3
#define P_MAX_MASK (1 << 2)
#define CELL_ACCESS_RELATED_INFO_PREAMBLE_NBITS 1
#define PLMN_IDENTITY_LIST_LEN_NBITS 3
#define PLMN_IDENTITY_LIST_LEN_MIN 1    /* plmn_identity_list = (1..6) */
#define PLMN_IDENTITY_MCC_LEN 3
#define PLMN_IDENTITY_MCC_NBITS 4
#define PLMN_IDENTITY_MNC_MIN 2 /* mnc = (2..3) */
#define PLMN_IDENTITY_MNC_NBITS 4
#define TRACKING_AREA_CODE_NBITS 16
#define CELL_IDENTITY_NBITS 28
#define CELL_BARRED_NBITS 1
#define INTRA_FREQ_RESELECTION_NBITS 1
#define CSG_INDICATION_NBITS 1
#define CSG_IDENTITY_NBITS 27
#define CELL_SELECTION_INFO_PREAMBLE_NBITS 1
#define Q_RX_LEV_MIN_NBITS 6
#define Q_RX_LEV_MIN_OFFSET_NBITS 3
#define P_MAX_NBITS 6
#define FREQ_BAND_INDICATOR_NBITS 6
#define SCHEDULING_INFO_LIST_LEN_NBITS 5
#define SCHEDULING_INFO_LIST_MIN 1      /* scheduling_info_list = (1..32) */
#define SI_PERIODICITY_NBITS 3
#define SIB_MAPPING_INFO_MIN 0  /* sib_mapping_info = (0..31) */
#define SIB_MAPPING_INFO_NBITS 5
#define SIB_TYPE_EXTENSION_NBITS 1
#define SIB_TYPE_NBITS 4
#define SIB_TYPE_ENUM_OFFSET 1
#define TDD_CONFIG_MASK (1 << 1)
#define TDD_CONFIG_NBITS 7
#define SI_WINDOW_LENGTH_NBITS 3
#define SYSTEM_INFO_VALUE_TAG_NBITS 5

#define BCCH_DL_SCH_CHOICE_0_1_SIB1 01

typedef struct {
    U8              choice[BCCH_CHOICE_NBITS];
    U8              sib1_preamble[SIB1_PREAMBLE_NBITS];
    U8              cell_acces_related_info_preamble[CELL_ACCESS_RELATED_INFO_PREAMBLE_NBITS];
    U8              plmn_identity_list_len[PLMN_IDENTITY_LIST_LEN_NBITS];
    U8              next;
} bcch_dl_sch_message_preamble_unpacked_t;

typedef struct {
    U8              tracking_area_code[TRACKING_AREA_CODE_NBITS];
    U8              cell_identity[CELL_IDENTITY_NBITS];
    U8              cell_barred[CELL_BARRED_NBITS];
    U8              intra_freq_reselection[INTRA_FREQ_RESELECTION_NBITS];
    U8              csg_indication[CSG_INDICATION_NBITS];
    U8              csg_identity[CSG_IDENTITY_NBITS];
    U8              next;
} cell_access_related_info_part2_unpacked_t;

typedef struct {
    U8              preamble[CELL_SELECTION_INFO_PREAMBLE_NBITS];
    U8              q_rx_lev_min[Q_RX_LEV_MIN_NBITS];
    U8              q_rx_lev_min_offset[Q_RX_LEV_MIN_OFFSET_NBITS];
    U8              next;
} cell_selection_info_unpacked_t;

typedef struct {
    U8              si_window_length[SI_WINDOW_LENGTH_NBITS];
    U8              system_info_value_tag[SYSTEM_INFO_VALUE_TAG_NBITS];
} sib1_message_part2_unpacked_t;



#ifdef CEVA_INTRINSICS

/*
 * ---------
 * CEVA CODE
 * ---------
 */

//-------------------------------------------------------------------------------------------
/** @brief Implements the downsampling process 
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param IQin        Pointer to the Input IQ samples memory
 *  @param nIQin       Number of Input IQ samples 
 *  @param IQout_cram  Pointer to the output downsampled data
 *  @param hbfir_real_buf_stage0_cram   Pointer to hbfir buffer of stage0
 *  @param hbfir_real_buf_stage1_cram   Pointer to hbfir buffer of stage1
 *  @param hbfir_real_buf_stage2_cram   Pointer to hbfir buffer of stage2
 *  @param hbfir_real_buf_stage3_cram   Pointer to hbfir buffer of stage3
 *  @param hbfir_real_buf_stage4_cram   Pointer to hbfir buffer of stage4
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |IQin         |         |pInput         |nIQin    |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |nIQin        |         |Input          |1        |signed int     |(1:31:0)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |IQout_cram        |         |pOutput        |nIQin/16 |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |hbfir_real_buf_stage0_cram    |         |pInput/pOutput |63       |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |hbfir_real_buf_stage1_cram    |         |pInput/pOutput |63       |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |hbfir_real_buf_stage2_cram    |         |pInput/pOutput |63       |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |hbfir_real_buf_stage3_cram    |         |pInput/pOutput |63       |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |hbfir_real_buf_stage4_cram    |         |pInput/pOutput |63       |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_downsampling_ceva |
 *  --------------------------------------------------------
 *  |Input Streams       |IQin, hbfir_real_buf_stage0_cram, hbfir_real_buf_stage1_cram, hbfir_real_buf_stage2_cram, hbfir_real_buf_stage3_cram, hbfir_real_buf_stage4_cram |
 *  --------------------------------------------------------
 *  |Output Streams      |IQout_cram, hbfir_real_buf_stage0_cram, hbfir_real_buf_stage1_cram, hbfir_real_buf_stage2_cram, hbfir_real_buf_stage3_cram, hbfir_real_buf_stage4_cram                  |
 *  --------------------------------------------------------
 *  |Config Parameters   |nIQin                            |
 *  --------------------------------------------------------
 *  |Status Parameters   |none                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to downsample the input data by 16 factor, using four HBFIR
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_cell_search
 *
 **/
void
lte_bs_phy_nmm_downsampling_total_ceva(S16 * IQinTL, S32 nIQin, S16 * IQout)
{
    S16            *pIQinTL0 = &IQinTL[HBFIR_DECIMATION_LEN * 2 * 4];
    S16            *pIQinTL1 = &IQinTL[HBFIR_DECIMATION_LEN * 2 * 3];
    S16            *pIQinTL2 = &IQinTL[HBFIR_DECIMATION_LEN * 2 * 2];
    S16            *pIQinTL3 = &IQinTL[HBFIR_DECIMATION_LEN * 2 * 1];
    // S16 * pIQinTL4 = &IQinTL[HBFIR_DECIMATION_LEN * 2 * 0];

    lte_bs_phy_nmm_hbfir_total_x1641(&pIQinTL1[HBFIR_DECIMATION_LEN * 2], &pIQinTL0[HBFIR_DECIMATION_LEN * 2], nIQin >> 2);
    lte_bs_phy_nmm_hbfir_total_x1641(&pIQinTL2[HBFIR_DECIMATION_LEN * 2], &pIQinTL1[HBFIR_DECIMATION_LEN * 2], nIQin >> 3);
    lte_bs_phy_nmm_hbfir_total_x1641(&pIQinTL3[HBFIR_DECIMATION_LEN * 2], &pIQinTL2[HBFIR_DECIMATION_LEN * 2], nIQin >> 4);
    lte_bs_phy_nmm_hbfir_total_x1641(IQout, &pIQinTL3[HBFIR_DECIMATION_LEN * 2], nIQin >> 5);
    /* tmp - 10MHz hard-coded */
    /* lte_bs_phy_nmm_hbfir_total_x1641(&pIQinTL4[HBFIR_DECIMATION_LEN * 2], &pIQinTL3[HBFIR_DECIMATION_LEN * 2], nIQin>>5); lte_bs_phy_nmm_hbfir_total_x1641(IQout,
     * &pIQinTL4[HBFIR_DECIMATION_LEN * 2], nIQin>>6); */
}


int
lte_bs_phy_nmm_downsampling_ceva(S16 * IQin, S32 nIQin, S16 * IQout_cram,
                                 S16 * hbfir_real_buf_stage0_cram,
                                 S16 * hbfir_real_buf_stage1_cram,
                                 S16 * hbfir_real_buf_stage2_cram,
                                 S16 * hbfir_real_buf_stage3_cram, S16 * hbfir_real_buf_stage4_cram)
{
    // LISTENING_CELL_SEARCH_DESC *this = (LISTENING_CELL_SEARCH_DESC *) cell_search;
    S16            *hbfir_buf = (S16 *) DataBuff_Bank1;
    S16            *hbfir_stage0 = &hbfir_buf[HBFIR_DECIMATION_LEN * 2 * 4];
    S16            *hbfir_stage1 = &hbfir_buf[HBFIR_DECIMATION_LEN * 2 * 3];
    S16            *hbfir_stage2 = &hbfir_buf[HBFIR_DECIMATION_LEN * 2 * 2];
    S16            *hbfir_stage3 = &hbfir_buf[HBFIR_DECIMATION_LEN * 2 * 1];
    S16            *hbfir_stage4 = &hbfir_buf[HBFIR_DECIMATION_LEN * 2 * 0];
    S16            *IQout;
    S16            *IQin_ceva;
    S32             in_size = nIQin * 2;

    MemCpyFromExt((void *) hbfir_buf, (void *) hbfir_real_buf_stage0_cram, HBFIR_DECIMATION_LEN * MAX_HBFIR_DECIMATION_STAGE * 2 * sizeof(S16));
    IQin_ceva = hbfir_buf + HBFIR_DECIMATION_LEN * MAX_HBFIR_DECIMATION_STAGE * 2;
    MemCpyFromExt((void *) IQin_ceva, (void *) IQin, in_size * sizeof(S16));

    IQout = IQin_ceva + in_size;

    lte_bs_phy_nmm_downsampling_total_ceva(hbfir_buf, nIQin, IQout);

    MemCpyToExt((void *) IQout_cram, (void *) IQout, (SUBFR_SIZE_096MHZ * 2) * sizeof(S16));

    MemCpyToExt((void *) hbfir_real_buf_stage0_cram, (void *) &hbfir_stage4[2 * (nIQin >> 4)], 2 * HBFIR_DECIMATION_LEN * sizeof(S16));
    MemCpyToExt((void *) hbfir_real_buf_stage1_cram, (void *) &hbfir_stage3[2 * (nIQin >> 3)], 2 * HBFIR_DECIMATION_LEN * sizeof(S16));
    MemCpyToExt((void *) hbfir_real_buf_stage2_cram, (void *) &hbfir_stage2[2 * (nIQin >> 2)], 2 * HBFIR_DECIMATION_LEN * sizeof(S16));
    MemCpyToExt((void *) hbfir_real_buf_stage3_cram, (void *) &hbfir_stage1[2 * (nIQin >> 1)], 2 * HBFIR_DECIMATION_LEN * sizeof(S16));
    MemCpyToExt((void *) hbfir_real_buf_stage4_cram, (void *) &hbfir_stage0[2 * (nIQin >> 0)], 2 * HBFIR_DECIMATION_LEN * sizeof(S16));
}


//-------------------------------------------------------------------------------------------
/** @brief SSS Demodulation
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param SSync_f	  Pointer to SSS data memory
 *  @param PSync_f	  Pointer to PSS data memory
 *  @param exp_hsphs  Pointer to the look-up table for derotation
 *  @param pZC		  Pointer to ZC sequence in frequency domain
 *  @param SSyncEst	  Pointer to the demodulated SSS memory
 *  @param lp_cp_flag Flag to indicate CP format
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |SSync_f	  |         |pInput		    |      |pComplex Array |(1:0:15)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  |PSync_f	  |         |pInput		    |      |pComplex Array |(1:0:15)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  |exp_hsphs	  |         |pInput		    |      |pComplex Array |(1:0:15)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  |pZC		  |         |pInput		    |      |pComplex Array |(1:0:15)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  |SSyncEst	  |         |pOutput		|         |pComplex Array          |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |lp_cp_flag	  |         |Input			|1        |Real		      |(1:31:0)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_ssync_est_channel_estimate_i1641|
 *  --------------------------------------------------------
 *  |Input Streams       |SSync_f,PSync_f,exp_hsphs,pZC	   |
 *  --------------------------------------------------------
 *  |Output Streams      |SSyncEst						   |
 *  --------------------------------------------------------
 *  |Config Parameters   |lp_cp_flag 					   |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do channel estimation according the PSS data and local ZC sequence, then use the channel estimation info to demodulate SSS data
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_cell_search
 *
 **/
void
lte_bs_phy_nmm_ssync_est_channel_estimate_i1641(S16 * SSync_f, S16 * PSync_f, S16 *exp_hsphs, S16 *pZC, S16 * SSyncEst, S16 lp_cp_flag)
{
    S16             k;
    S32             ac,
                    ad,
                    bc,
                    bd;
                    //cc;
    S16             AccR;
    S16             AccR1,
                    AccI,
                    AccI1;
    //S16             expo,
    //                expo1,
    //                expo2,
    //                temp1;
//    S32             norm_val_in;
    S16             tmp16;
                    //t16;
                    //tmpb16;

    register unsigned int moda __asm__("a15");

    moda = mov_moda_acZ();

    lbf_uimm3_ps(0x2);          // after mulitiply shift left 1bit
    lbf_uimm5_satmod(0x1a);

    for (k = 0; k < (FFT_SIZE_096MHZ); k++) {   // FFT_SIZE_096MHZ=64

        if (lp_cp_flag == SHORT_CP) {
            /* compute SSS1(i)*expo(i) : derotation due to half sample shift */
            ac = mpy_acXl_acYl_acZ(*SSync_f, *exp_hsphs);
            ad = mpy_acXl_acYl_acZ(*SSync_f, *(exp_hsphs + 1));
            bc = mpy_acXl_acYl_acZ(*(SSync_f + 1), *exp_hsphs);
            bd = mpy_acXl_acYl_acZ(*(SSync_f + 1), *(exp_hsphs + 1));

            AccR = extract_h(ac);
            tmp16 = extract_h(bd);
            AccR = sub_acW_acX_acZ(AccR, tmp16);

            AccI = extract_h(ad);
            tmp16 = extract_h(bc);
            AccI = add_acW_acX_acZ(AccI, tmp16);
        }
        else {
            // no derotation this time because no half sample shift
            AccR = *SSync_f;
            AccI = *(SSync_f + 1);
        }

        SSync_f = SSync_f + 2;  // added by crystal
        exp_hsphs = exp_hsphs + 2;  // added by crystal

        /* compute channel estimate from pilots: InvChanEst */
        ac = mpy_acXl_acYl_acZ(*pZC, *PSync_f);
        ad = mpy_acXl_acYl_acZ(*pZC, *(PSync_f + 1));
        ad = mac_acXl_acYl_acZ(*(pZC + 1), *PSync_f, ad);       // bc
        ac = mac_acXl_acYl_acZ(*(pZC + 1), -*(PSync_f + 1), ac);        // ac+bd

#if ORIG_CODE
        cc = mpy_acXl_acYl_acZ(*PSync_f, *PSync_f);
        cc = mac_acXl_acYl_acZ(-*(PSync_f + 1), -*(PSync_f + 1), cc);   // dd=...

        pZC = pZC + 2;
        PSync_f = PSync_f + 2;

        /* real part */
        expo1 = exp_acW_acZ_zero(ac);

        ac = shift_acW_acX_acZ(ac, expo1 - 1);
        expo2 = exp_acW_acZ_zero(cc);

        cc = shift_acW_acX_acZ(cc, expo2);
        tmp16 = extract_h(ac);
        tmpb16 = extract_h(cc);
        t16 = abs_acW_acZ(tmp16);

        temp1 = divide_s(t16, tmpb16);
        expo = -expo1 + 1 + expo2;      // -15 -8 -expo1+1 + 8 + expo2 + 15;
        AccR1 = shift_acW_acX_acZ(temp1, expo);
        if (tmp16 < 0) {
            AccR1 = -AccR1;
        }

        /* imaginary part */
        expo1 = exp_acW_acZ_zero(ad);
        norm_val_in = shift_acW_acX_acZ(ad, expo1 - 1);
        tmp16 = extract_h(norm_val_in);

        t16 = abs_acW_acZ(tmp16);
        temp1 = divide_s(t16, tmpb16);
        expo = -expo1 + 1 + expo2;      // -15 -8 -expo1+1 + 8 + expo2 + 15;
        AccI1 = shift_acW_acX_acZ(temp1, expo);
        if (tmp16 < 0)
            AccI1 = -AccI1;

        /* compute derotated SSS1(i)*InvChanEst */
        ac = mpy_acXl_acYl_acZ(AccR, AccR1);
        // ad = mpy_acXl_acYl_acZ(AccR, AccI1);
        // ad = mac_acXl_acYl_acZ(AccI, AccR1, ad);//bc=...
        bd = mpy_acXl_acYl_acZ(AccI, AccI1);    // bd=...

        AccR = extract_h(ac);
        tmp16 = extract_h(bd);
        AccR = sub_acW_acX_acZ(AccR, tmp16);

        *SSyncEst = 1;

        if (AccR < 0) {
            *SSyncEst = -1;
        }
        SSyncEst = SSyncEst + 1;        // added by Crystal
    }
    SSyncEst = SSyncEst - FFT_SIZE_096MHZ;

    *SSyncEst = 0;
    *(SSyncEst + 32) = 0;

#else
        AccR1 = shift_acW_imm7_acZ_ar(ac, -8);
        AccI1 = shift_acW_imm7_acZ_ar(ad, -8);

        /* compute derotated SSS1(i)*InvChanEst */
        ac = mpy_acXl_acYl_acZ(AccR, AccR1);
        ad = mpy_acXl_acYl_acZ(AccR, AccI1);
        ad = mac_acXl_acYl_acZ(AccI, AccR1, ad);    //bc=...
        ac = msu_acXl_acYl_acZ(AccI, AccI1, ac);    // bd=...

        *SSyncEst = extract_h(ac);
        *(SSyncEst+1) = extract_h(ad);

        pZC = pZC + 2;
        PSync_f = PSync_f + 2;
        SSyncEst = SSyncEst + 2;
    }

#endif

    mov_acX_moda(moda);
}


void
lte_bs_phy_nmm_ssync_decode_sss_part2_i1641(S16 * d_even, S16 * m1st, S16 * xaccMax)
{
    S16             xacc,
                    xaccb;
    S32             j,
                    k;
    S16            *S0_REFp,
                   *p,
                   *pd;

    S0_REFp = S0_REF;
    pd = d_even;

    /* Search for m_1st */
    for (j = 0; j < 31; j++) {
        p = S0_REFp;

        xacc = mpy_acXl_acYl_acZ(d_even[0], S0_REF[j]);
        xaccb = mpy_acXl_acYl_acZ(d_even[15], S0_REF[j + 15]);
        for (k = 1; k < 15; k++) {
            xacc = mac_acXl_acYl_acZ(d_even[k], S0_REF[j + k], xacc);
            xaccb = mac_acXl_acYl_acZ(d_even[k + 15], S0_REF[j + k + 15], xaccb);
        }
        xaccb = mac_acXl_acYl_acZ(d_even[30], S0_REF[j + 30], xaccb);
        xacc = add_acW_acX_acZ(xacc, xaccb);

        xacc = abs_acW_acZ(xacc);
        if (*xaccMax < xacc) {
            *xaccMax = xacc;
            *m1st = (S16) j;
        }
    }
}


void
lte_bs_phy_nmm_ssync_decode_sss_part4_i1641( S16 * d_odd, S16 * m2nd, S16 * xaccMax)
{
    S16             xacc,
                    xaccb;

    S32             j,
                    k;

    for (j = 0; j < 31; j++) {
        xacc = mpy_acXl_acYl_acZ(d_odd[0], S0_REF[j]);
        xaccb = mpy_acXl_acYl_acZ(d_odd[15], S0_REF[j + 15]);
        for (k = 1; k < 15; k++) {

            xacc = mac_acXl_acYl_acZ(d_odd[k], S0_REF[j + k], xacc);
            xaccb = mac_acXl_acYl_acZ(d_odd[k + 15], S0_REF[j + k + 15], xaccb);
        }
        xaccb = mac_acXl_acYl_acZ(d_odd[30], S0_REF[j + 30], xaccb);
        xacc = add_acW_acX_acZ(xacc, xaccb);

        xacc = abs_acW_acZ(xacc);

        if (*xaccMax < xacc) {
            *xaccMax = xacc;
            *m2nd = (S16) j;
        }
    }
}


void
lte_bs_phy_nmm_ssync_decode_sss_ceva(S16 * SSyncEst, S16 NID_2, S16 * NID_cell, S16 * Subframe)
{
    S16             good,
                    goodnum;
    S16            *p_d_even,
                   *p_d_odd;
    S16             d_evendata[32],
                    d_odddata[32];
    S16             xaccMax1 = 0;
    S16             xaccMax2 = 0;
    S16             m1st,
                    m2nd;
    S16             m0,
                    m1;
    S32             k;
    S16             xacc;

    *NID_cell = -1;

    p_d_even = d_evendata;
    p_d_odd = d_odddata;

    lte_bs_phy_nmm_ssync_decode_sss_part1_x1641(SSyncEst, NID_2, p_d_even);

    lte_bs_phy_nmm_ssync_decode_sss_part2_i1641(p_d_even, &m1st, &xaccMax1);

    if (xaccMax1 < 20) {
        return;
    }

    goodnum = good;
    lte_bs_phy_nmm_ssync_decode_sss_part3_x1641(SSyncEst, NID_2, m1st, p_d_odd);

    lte_bs_phy_nmm_ssync_decode_sss_part4_i1641( p_d_odd,  &m2nd, &xaccMax2);

    if (xaccMax2 < 20) {
        return;
    }

    if (m1st == m2nd) {
        return;
    }

    if (m1st < m2nd) {
        *Subframe = 0;
        m0 = m1st;
        m1 = m2nd;
    }
    else {
        *Subframe = 5;
        m0 = m2nd;
        m1 = m1st;
    }

    /* compute NID_1 from m0 and m1 */
    k = m1 - m0;
    xacc = (S16) ((k - 1) * 30 + m0);
    switch (k) {
    case 3:
        xacc -= 1;
        break;
    case 4:
        xacc -= 3;
        break;
    case 5:
        xacc -= 6;
        break;
    case 6:
        xacc -= 10;
        break;
    case 7:
        xacc -= 15;
        break;
    }

    if (xacc > PCI_SYNC_NB_GRP) {
        return;
    }

    *NID_cell = 3 * xacc + NID_2;

    return;
}


//-------------------------------------------------------------------------------------------
/** @brief implement FFT DC Skip
 *
 *  <1> Hardware Resource:
 *  CEVA
 * 
 *  @param input	  Pointer to input data memory
 *  @param output     Pointer to output data memory	
 *  @param length  	  Number of input data length
 *  @param offset  	  Number of offset
 *  @param expo_mat	  Pointer to exponent number
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |input		  |         |pInput		|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |output		  |         |pOutput		|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |length |         |Input		|		  |Real  |(1:31:0)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  |offset |         |Input		|		  |Real  |(1:31:0)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  |expo_mat		  |         |pInput		|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_swap_fftout_dcskip_ceva  |
 *  --------------------------------------------------------
 *  |Input Streams       |input,expo_mat    			   |
 *  --------------------------------------------------------
 *  |Output Streams      |output						   |
 *  --------------------------------------------------------
 *  |Config Parameters   |length,offset					   | 
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do dc skip on fft output data
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_phy	
 *
 **/
void
lte_bs_phy_swap_fftout_dcskip_ceva(S16 * input, S16 * output, S32 length, S32 offset, S16 * expo_mat)
{
    S32             k;

    /* store detected resource grid from selected raw demodulated values */
    for (k = 0; k < length; k++) {
        /* get low frequencies */
        output[2 * k] = input[2 * k + offset];
        output[2 * k + 1] = input[2 * k + offset + 1];

        /* get Upper frequencies */
        output[2 * k + 2 * length] = input[2 * k + 2];
        output[2 * k + 2 * length + 1] = input[2 * k + 3];
    }

    output[4 * length] = *expo_mat;
}



//-------------------------------------------------------------------------------------------
/** @brief Implements the DL Pilos Extraction
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param expoNfft      Exponent size of FFT 
 *  @param pin           Pointer to the Input IQ samples memory
 *  @param nIQin         Number of Input IQ samples 
 *  @param symb_index    symbol index
 *  @param fft_done_ptr  Flag to indicate the complete of FFT
 *  @param cell_pci      cell PCI
 *  @param cell_cp       cell CP format
 *  @param bandwidth_rb  RB number which should be extracted the DL pilots
 *  @param dl_pilot      Pointer to the DL pilots memory
 *  @param expo_fac      Exponent number of FFT output
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length      |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |            |               |           |            |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |expoNfft     |         |Input          |1           |signed short   |(1:15:0)   |            |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |pin          |         |pInput         |1<<expoNfft |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |symb_index   |         |Input          |1           |signed short   |(1:15:0)   |[0:13]      |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |fft_done_ptr |         |pOutput        |1           |pReal		     |(1:15:0)   |[0, 1]      |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |cell_pci     |         |Input          |1           |signed short   |(1:15:0)   |[0:503]     |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |cell_cp      |         |Input          |1           |signed short   |(1:15:0)   |[0,1]       |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |bandwidth_rb |         |Input          |1           |signed short   |(1:15:0)   |[6:100]     |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |dl_pilot     |         |pOutput		|            |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |expo_fac     |         |pOutput        |1           |pReal	         |(1:15:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_extract_dlpilots_ceva |
 *  ------------------------------------------------------------
 *  |Input Streams       |pin,								   |
 *  ------------------------------------------------------------
 *  |Output Streams      |fft_done_ptr, dl_pilot, expo_fac     |
 *  ------------------------------------------------------------
 *  |Config Parameters   |expoNfft, symb_index, cell_pci,	   |
 *  |					 |cell_cp, bandwidth_rb                |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do FFT on the input data, then extract the required DL Pilot on the FFT output
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_cell_search
 *
 **/
void
lte_bs_phy_nmm_extract_dlpilots_ceva(S16 expoNfft, S16 * pin, S16 symb_index, S16 * fft_done_ptr, S16 cell_pci, S16 cell_cp, S16 bandwidth_rb, S16 * dl_pilot, S16 * expo_fac)
{
    S32             length = (1 << expoNfft);
    S16             pilot_grp = MODULO6(cell_pci);
    S16             pilot_idx;
    S16             nb_subcarriers_of_interest = bandwidth_rb * PRB_12;
    S16             i;
    S16            *p16,
                   *q16;
    S16            *fft_out;    // [2 * FFTSZ_2048];
    S16            *re_array;   // [2 * FFTSZ_2048];
    S16            *timedomain_ceva;
    S16             fft_size = (2 * (1 << expoNfft));
    S16             expo_ceva;

#if NMM_CEVA_DBG
    sprintf(ceva_dbg, "*ceva_dbg* pin %d (%x)\r\n", pin[0], pin);
#endif

    /* pin points after CP */
    timedomain_ceva = (S16 *) DataBuff_Bank1;
    fft_out = timedomain_ceva + fft_size;
    re_array = fft_out + fft_size;
    MemCpyFromExt((void *) timedomain_ceva, (void *) pin, fft_size * sizeof(S16));

    cfft_16t32d(timedomain_ceva, expoNfft, fft_out, &expo_ceva);
    *expo_fac = expo_ceva - 5;

    /* FFT to RE mapping + DC skipping */
    lte_bs_phy_swap_fftout_dcskip_ceva(fft_out, re_array, nb_subcarriers_of_interest / 2, (S32) (2 * length - nb_subcarriers_of_interest), expo_fac);

    *fft_done_ptr = TRUE;

    /* pilot extraction */
    if ((cell_cp == SHORT_CP && (symb_index == SYMBOL(0) || symb_index == SYMBOL(7))) || (cell_cp == LONG_CP && (symb_index == SYMBOL(0) || symb_index == SYMBOL(6)))) {
        /* start idx is pilot_grp */
        pilot_idx = pilot_grp;
    }
    else {
        /* start idx is (pilot_grp + 3) modulo 6 */
        pilot_idx = MODULO6(pilot_grp + 3);
    }

    p16 = dl_pilot;
    q16 = &re_array[2 * pilot_idx];
    for (i = 0; i < REF_SIGNAL_PER_RB * bandwidth_rb; i++) {
        *p16++ = q16[0];
        *p16++ = q16[1];
        q16 += (REF_SIGNAL_SC_SPACING * 2);
    }
}


#else

/*
 * --------
 * ARM CODE
 * --------
 */

static          U32
count_set_bits(U32 v)
{
    U32             c;

    v = v - ((v >> 1) & 0x55555555);    // reuse input as temporary
    v = (v & 0x33333333) + ((v >> 2) & 0x33333333);     // temp
    c = ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24; // count
    return c;
}


/*
 * Downsampling initialization
 */
void
lte_bs_phy_nmm_downsampling_init(void *cell_search)
{
    LISTENING_CELL_SEARCH_DESC *this = (LISTENING_CELL_SEARCH_DESC *) cell_search;

    this->hbfir_state_buf = MemAlloc(HBFIR_DECIMATION_LEN * MAX_HBFIR_DECIMATION_STAGE * 2 * sizeof(S16), CRAM_MEM, 1);

    this->hbfir_real_buf_stage0 = &this->hbfir_state_buf[0 * HBFIR_DECIMATION_LEN];
    this->hbfir_imag_buf_stage0 = &this->hbfir_state_buf[1 * HBFIR_DECIMATION_LEN];
    this->hbfir_real_buf_stage1 = &this->hbfir_state_buf[2 * HBFIR_DECIMATION_LEN];
    this->hbfir_imag_buf_stage1 = &this->hbfir_state_buf[3 * HBFIR_DECIMATION_LEN];
    this->hbfir_real_buf_stage2 = &this->hbfir_state_buf[4 * HBFIR_DECIMATION_LEN];
    this->hbfir_imag_buf_stage2 = &this->hbfir_state_buf[5 * HBFIR_DECIMATION_LEN];
    this->hbfir_real_buf_stage3 = &this->hbfir_state_buf[6 * HBFIR_DECIMATION_LEN];
    this->hbfir_imag_buf_stage3 = &this->hbfir_state_buf[7 * HBFIR_DECIMATION_LEN];
    this->hbfir_real_buf_stage4 = &this->hbfir_state_buf[8 * HBFIR_DECIMATION_LEN];
    this->hbfir_imag_buf_stage4 = &this->hbfir_state_buf[9 * HBFIR_DECIMATION_LEN];
}


//-------------------------------------------------------------------------------------------
/** @brief implement FFT DC Skip
 *
 *  <1> Hardware Resource:
 *  ARM
 * 
 *  @param input	  Pointer to input data memory
 *  @param output     Pointer to output data memory	
 *  @param length  	  Number of input data length
 *  @param offset  	  Number of offset
 *  @param expo_mat	  Pointer to exponent number
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |input		  |         |pInput		|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |output		  |         |pOutput		|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |length |         |Input		|		  |Real  |(1:31:0)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  |offset |         |Input		|		  |Real  |(1:31:0)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  |expo_mat		  |         |pInput		|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_swap_fftout_dcskip    |
 *  --------------------------------------------------------
 *  |Input Streams       |input,expo_mat    			   |
 *  --------------------------------------------------------
 *  |Output Streams      |output						   |
 *  --------------------------------------------------------
 *  |Config Parameters   |length,offset					   | 
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do dc skip on fft output data
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_phy	
 *
 **/
static void
lte_bs_phy_swap_fftout_dcskip(S16 * input, S16 * output, S32 length, S32 offset, S16 * expo_mat)
{
    S32             k;

    // store detected resource grid from selected raw demodulated values
    for (k = 0; k < length; k++) {
        /*
         * Get low frequencies
         */
        output[2 * k] = input[2 * k + offset];
        output[2 * k + 1] = input[2 * k + offset + 1];

        /*
         * Get Upper frequencies
         */
        output[2 * k + 2 * length] = input[2 * k + 2];
        output[2 * k + 2 * length + 1] = input[2 * k + 3];
    }
}


//-------------------------------------------------------------------------------------------
/** @brief Implements the DL Pilos Extraction
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param expoNfft      Exponent size of FFT 
 *  @param pin           Pointer to the Input IQ samples memory
 *  @param nIQin         Number of Input IQ samples 
 *  @param symb_index    symbol index
 *  @param fft_done_ptr  Flag to indicate the complete of FFT
 *  @param cell_pci      cell PCI
 *  @param cell_cp       cell CP format
 *  @param bandwidth_rb  RB number which should be extracted the DL pilots
 *  @param dl_pilot      Pointer to the DL pilots memory
 *  @param expo_fac      Exponent number of FFT output
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length      |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |            |               |           |            |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |expoNfft     |         |Input          |1           |signed short   |(1:15:0)   |            |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |pin          |         |pInput         |1<<expoNfft |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |symb_index   |         |Input          |1           |signed short   |(1:15:0)   |[0:13]      |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |fft_done_ptr |         |pOutput        |1           |pReal		     |(1:15:0)   |[0, 1]      |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |cell_pci     |         |Input          |1           |signed short   |(1:15:0)   |[0:503]     |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |cell_cp      |         |Input          |1           |signed short   |(1:15:0)   |[0,1]       |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |bandwidth_rb |         |Input          |1           |signed short   |(1:15:0)   |[6:100]     |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |dl_pilot     |         |pOutput		|            |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |expo_fac     |         |pOutput        |1           |pReal	         |(1:15:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_extract_dlpilots      |
 *  ------------------------------------------------------------
 *  |Input Streams       |pin,								   |
 *  ------------------------------------------------------------
 *  |Output Streams      |fft_done_ptr, dl_pilot, expo_fac     |
 *  ------------------------------------------------------------
 *  |Config Parameters   |expoNfft, symb_index, cell_pci,	   |
 *  |					 |cell_cp, bandwidth_rb                |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do FFT on the input data, then extract the required DL Pilot on the FFT output
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_cell_search
 *
 **/

void
lte_bs_phy_nmm_extract_dlpilots(S16 expoNfft, S16 * pin, S16 symb_index, S16 * fft_done_ptr, S16 cell_pci, S16 cell_cp, S16 bandwidth_rb, S16 * dl_pilot, S16 * expo_fac)
{
    S32             length = (1 << expoNfft);
    S16             pilot_grp = MODULO6(cell_pci);
    S16             pilot_idx;
    S16             nb_subcarriers_of_interest = bandwidth_rb * PRB_12;
    S16             i;
    S16            *p16,
                   *q16;
    S16             fft_out[2 * FFTSZ_2048];
    S16             re_array[2 * FFTSZ_2048];

    /* pin points after CP */

    /* FFT */
    fftcg_c_16t24d(pin, (int) expoNfft, fft_out, expo_fac);

    /* FFT to RE mapping + DC skipping */
    lte_bs_phy_swap_fftout_dcskip(fft_out, re_array, nb_subcarriers_of_interest / 2, (S32) (2 * length - nb_subcarriers_of_interest), expo_fac);
    DUMP_ARR_ID_UART(re_array, (nb_subcarriers_of_interest * 2), "d", 0, 1);

    *fft_done_ptr = TRUE;

    /* pilot extraction */
    if ((cell_cp == SHORT_CP && (symb_index == SYMBOL(0) || symb_index == SYMBOL(7))) || (cell_cp == LONG_CP && (symb_index == SYMBOL(0) || symb_index == SYMBOL(6)))) {
        /* start idx is pilot_grp */
        pilot_idx = pilot_grp;
    }
    else {
        /* start idx is (pilot_grp + 3) modulo 6 */
        pilot_idx = MODULO6(pilot_grp + 3);
    }

    nmm_dbg_printf_off(("@@@ pilot_idx %d symb_index %d cell_pci %d \r\n", pilot_idx, symb_index, cell_pci));
    p16 = dl_pilot;
    q16 = &re_array[2 * pilot_idx];
    for (i = 0; i < REF_SIGNAL_PER_RB * bandwidth_rb; i++) {
        *p16++ = q16[0];
        *p16++ = q16[1];
        q16 += (REF_SIGNAL_SC_SPACING * 2);

    }
}


#ifdef FEC_FDESC_ENABLED

void
fec_nmm_done(LPVOID a)
{
    fec_nmm_cc_stat = 1;

    return;
}

#endif

/*
 * PBCH rx FEC decoding
 */

void
lte_bs_phy_nmm_pbch_chan_coding(S8 in[], U8 out[], S32 nin, S32 nout, S16 nant, S16 scf, U8 * stat)
{
    S32             nencin;
    U8              gencrc[2],
                   *deccrc;
    S8             *scfin;
#ifdef FEC_FDESC_ENABLED
    PFEC_UL_FDESC   pFecUL;
    volatile PFEC_CONFIG_REGS pFecConfigRegs;
    ULHWCTX         hw_ctx = { 0 };
    S8             *DescAdr[1];
    UINT32          DescLen[1];
#endif

    /* Compute encoder input length in bits (+16 is for additional CRC) */
    nencin = nout + CRC_LEN;

    /* Allocate temporary space for scaled input array */
    scfin = (S8 *) MemAlloc(nin * sizeof(S8), DDR_MEM, 1);

    /* Scale soft-decision input array */
    FSCALEB(in, scfin, nin, scf);

#ifndef FEC_FDESC_ENABLED
    /* Run simulation for FEC hardware decoder */
    fec_decode_hdwr_sim(0,      // Type of encoding or decoding (0=CC, 1=CTC)
                        (U16) nencin,   // Code block size
                        0,      // Type of CRC
                        0,      // Number of initial filler bits in code block
                        0,      // Coding rate
                        0,      // Number of decoder 1/2 iterations
                        0,      // CTC decoder automatic stop select
                        0,      // Decoder bypass
                        0,      // HARQ soft-combining enable
                        (U32) nin,      // Rate matching E parameter
                        0,      // Rate matching k0 parameter
                        (U16) 0,        // Rate matching Ncb parameter
                        2,      // Number of modulation bits used (2,4,6)
                        1,      // Bypass channel de-interleaver
                        0,      // Number of channel interleaver columns
                        NULL,   // Channel de-interleaver descriptor starting addresses
                        (S32 *) 0,      // Channel de-interleaver descriptor byte counts
                        0,      // Channel de-interleaver number of descriptors
                        (S8 *) scfin,   // Input buffer pointer
                        (U8 *) out,     // Output buffer pointer
                        0);     // Code block index for debug only
#else
    /* allocate temporary space for descriptors via driver */
    pFecUL = FecUlAllocDesc(1);

    /* configure FEC control registers */
    pFecConfigRegs = (PFEC_CONFIG_REGS) FEC_CONFIG_REGS_BASE;
    pFecConfigRegs->bp_channel_interleaver = 1;

    /* set hardware context */
    memset(&hw_ctx, 0, sizeof(hw_ctx));

    hw_ctx.CTC_CCn = 0;
    hw_ctx.CRC_Sel = 0;
    hw_ctx.Modulation = 1;
    hw_ctx.HalfInteration = 0;
    hw_ctx.Bypass_Rate_Match = 0;
    hw_ctx.Rate_Index = 0;
    hw_ctx.Enable_Combine = 0;
    hw_ctx.Bypass_Decode_Enable = 0;
    hw_ctx.Dynamic_Stop_Iteration = 0;
    hw_ctx.BlockSize = nencin;
    hw_ctx.Filler = 0;
    hw_ctx.E_Param = nin;
    hw_ctx.LTERateMaching_K0 = 0;

    /* create FEC CTC decoder control descriptor via driver */
    DescAdr[0] = scfin;
    DescLen[0] = nin;

    FecUlSetHwCtx(pFecUL, hw_ctx);
    FecUlSetData(pFecUL, (LPVOID *) DescAdr, DescLen, out, (nencin + 7) >> 3);

    /* Inform FEC driver to enable CC decoder finished interrupt. FEC driver will call function FecRxCCDone when decoder is finished. */
    FecUlSetLastDesc(pFecUL, (void (*)(LPVOID)) fec_nmm_done, (LPVOID) NULL);

    /* Initialize decoder finished status flag. Function FecRxCCDone will set status flag to 1 when decoder has finished. */
    fec_nmm_cc_stat = 0;

    /* run FEC CC decoder via driver and wait until finished */
    FecUlRun(pFecUL);

    while (fec_nmm_cc_stat == 0);
#endif

    deccrc = &out[nout >> 3];

    /* Use received data to re-compute 16-bit CRC */
    calc_gcrc16(out, gencrc, nout);

    DUMP_ARR_ID_UART(out, nencin >> 3, "02x", 0, 1);

    /* Scramble CRC bits according to antenna configuration. If 2 antenna ports, then scramble using 0xFFFF */
    if (nant == 2) {
        gencrc[0] = gencrc[0] ^ 0xFF;
        gencrc[1] = gencrc[1] ^ 0xFF;
    }
    /* Else if 4 antenna ports, then scramble using 0x5555 */
    else if (nant == 4) {
        gencrc[0] = gencrc[0] ^ 0x55;
        gencrc[1] = gencrc[1] ^ 0x55;
    }

    /* Initialize output status = 0 */
    *stat = 0;

    /* Set bit0 of status if CRC is correct */
    if ((deccrc[0] == gencrc[0]) && (deccrc[1] == gencrc[1]))
        *stat = 1;

    /* De-allocate temporary space */
    MemFree(scfin, DDR_MEM);

    return;
}


//-------------------------------------------------------------------------------------------
/** @brief MIB Decode
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param current_cell		  Pointer to the cell parameter structure
 *  @param mib				  Pointer to the decoded mib memory
 *  @param low_sfn_index	  low sfn index
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |current_cell |         |pInput         |1        |pStructure     |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |mib          |         |pOutput        |1        |pReal Array    |(0:8:0)    |full range  |Byte  |
 *  ---------------------------------------------------------------------------------------------------
 *  |low_sfn_index|         |Input          |1        |Real           |(0:8:0)    |full range  |Byte  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_mib_decode        |
 *  --------------------------------------------------------
 *  |Input Streams       |current_cell                     |
 *  --------------------------------------------------------
 *  |Output Streams      |mib							   |
 *  --------------------------------------------------------
 *  |Config Parameters   |low_sfn_index					   |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to decode MIB
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_MIB 
 *
 **/
static void
lte_bs_phy_nmm_mib_decode(CELL_PARAMS * current_cell, U8 * mib, U8 low_sfn_index)
{
    MIBDESC        *pmib = (MIBDESC *) mib;
    U16             loc_mib = mib[0] << 8 | mib[1];
    U8             DCI_1A_size_LUT[N_BW_CASE]={21,22,25,27,27,28};
    U8             DCI_1C_size_LUT[N_BW_CASE]={8,10,12,13,14,15};

    pmib = (MIBDESC *) & loc_mib;

    current_cell->bw_index = pmib->dl_bandwith;
    current_cell->dl_tx_bw = MIB_BW_TO_RB[pmib->dl_bandwith];
    current_cell->SI_DCI_size[0] = DCI_1C_size_LUT[current_cell->bw_index];
    current_cell->SI_DCI_size[1] = DCI_1A_size_LUT[current_cell->bw_index];

    current_cell->phich_duration = pmib->phich_duration;
    current_cell->phich_resource = pmib->phich_resource;
    current_cell->sfn = (pmib->system_frame_number << 2) + low_sfn_index;

    nmm_dbg_printf_off(("@@ lte_bs_phy_nmm_mib_decode %04x (bw %d dur %d res %d sfn %d)\r\n", *pmib, current_cell->dl_tx_bw, pmib->phich_duration, pmib->phich_resource,
                        current_cell->sfn));
}


void
lte_bs_phy_nmm_extract_pcfi_ini(S16 expoNfft, S16 bw, SYMBOL_INFO * sym, S16 * pcfi_re_idx, S16 * pcfi, S16 * expo_fac)
{
    S16            *pin = sym->start;
    S32             length = (1 << expoNfft);
    S16             nb_subcarriers_of_interest = bw * PRB_12;
    S16             i;
    S16            *p16,
                   *q16;
    S16             fft_out[2 * FFTSZ_2048];
    S16             re_array[2 * FFTSZ_2048];

    /* skip CP */
    pin = pin + sym->cp_size * 2;

    /* FFT */
    fftcg_c_16t24d(pin, (int) expoNfft, fft_out, expo_fac);

    /* FFT to RE mapping + DC skipping */
    lte_bs_phy_swap_fftout_dcskip(fft_out, re_array, nb_subcarriers_of_interest / 2, (S32) (2 * length - nb_subcarriers_of_interest), expo_fac);
    DUMP_ARR_ID_UART(re_array, (nb_subcarriers_of_interest * 2), "d", 0, 1);

    sym->fft_done = TRUE;

    /* PCFI RE extraction */
    p16 = pcfi;
    q16 = re_array;
    for (i = 0; i < PCFICH_NUM_RE; i++) {
        *p16++ = q16[2 * pcfi_re_idx[i]];
        *p16++ = q16[2 * pcfi_re_idx[i] + 1];
    }
}


static void
lte_bs_phy_nmm_convert_soft_hard_bit(S8 * pSoft, S8 * pHard, S32 L)
{
    S32             i;

    for (i = 0; i < L; i++) {
        if (pSoft[i] < 0)
            pHard[i] = 1;
        else
            pHard[i] = 0;
    }
}


//-------------------------------------------------------------------------------------------
/** @brief PCFI Decode
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param current_cell		  Pointer to the cell parameter structure
 *  @param pcfi				  Pointer to the pcfi data memory
 *  @param slot_num	     	  slot number
 *  @param expo	     	      exponent number of the pcfi data
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |current_cell |         |pInput/pOutput |1        |pStructure     |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pcfi         |         |pInput         |1        |pComplex Array |(1:15:0)    |full range |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |slot_num     |         |Input          |1        |Real           |(1:15:0)    |[0:19]     |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |expo		  |         |Input          |1        |Real           |(1:15:0)    |full range |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_decode_pcfi       |
 *  --------------------------------------------------------
 *  |Input Streams       |current_cell,pcfi                |
 *  --------------------------------------------------------
 *  |Output Streams      |current_cell					   |
 *  --------------------------------------------------------
 *  |Config Parameters   |slot_num, expo				   |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to decode PCFI
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.212
 *
 *  \ingroup group_lte_nmm_SIB 
 *
 **/
void
lte_bs_phy_nmm_decode_pcfi(CELL_PARAMS * current_cell, S16 * pcfi, S16 slot_num, S16 expo)
{
    S16             i;
    S32             pcfi32[2 * PCFICH_NUM_RE];
    S32            *pcfi32_ptr = pcfi32;
    S8              bDetSoft[2 * PCFICH_NUM_RE],
                    bDetHard[2 * PCFICH_NUM_RE];
    S16             NShift;
    U32             L_tmp3;
    U8             *cvec;       // [PCFI_SIZE_IN_BITS];
    S16             temp;
    U32             cinit = 0;
    U8              cvecByteHard[4];
    U32            *p32 = (U32 *) cvecByteHard;
    U32             cur_cfi;
    S8              cfi_idx = 0xff;
    U32             min_xcor = 0xffffffff;

    nmm_dbg_printf_off(("@@ %s\r\n", __FUNCTION__));

    /* generate 32 bit PCFI content with individual symbol scaling */
    for (i = 0; i < (PCFICH_NUM_RE); i++) {
        *pcfi32_ptr++ = L_shl(*pcfi++, expo);
        *pcfi32_ptr++ = L_shl(*pcfi++, expo);
    }

    DUMP_ARR_ID_UART(pcfi32, (2 * PCFICH_NUM_RE), "d", 0, 1);

    NShift = block_lnorm(pcfi32, (2 * PCFICH_NUM_RE), 0);

    /* QPSK PCFI demodulation */
    lte_bs_phy_nmm_demod_qpsk(pcfi32, bDetSoft, NShift, (S32) (2 * PCFICH_NUM_RE));

    /* Hard conversion demodulation */
    lte_bs_phy_nmm_convert_soft_hard_bit(bDetSoft, bDetHard, (S32) (2 * PCFICH_NUM_RE));

    DUMP_ARR_ID_UART(bDetSoft, PCFICH_NUM_RE, "d", 0, 1);
    DUMP_ARR_ID_UART(bDetHard, PCFICH_NUM_RE, "d", 0, 1);

    /* PCFI scrambling sequence init, depend on slot ID and PCI */
    temp = (slot_num >> 1) + 1;
    L_tmp3 = (2 * current_cell->cell_pci + 1) << 9;

    /* cinit = floor(ns/2)*2^9 + Ncell_id; */
    cinit = temp * L_tmp3 + current_cell->cell_pci;

    cvec = MemAlloc(PCFI_SIZE_IN_BITS * sizeof(U8), CRAM_MEM, 1);
    lte_bs_phy_nmm_pseudo_random_seq_gen_tcb(NULL, PCFI_SIZE_IN_BITS, cinit, cvec,
        nmm_tcb_arr[3][2+NUM_SYMB_PBCH]);

    lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[3], (2+NUM_SYMB_PBCH), BYPASS_TCB);
    lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[3]+3+NUM_SYMB_PBCH, (N_PBCH_TCB-(3+NUM_SYMB_PBCH)), BYPASS_TCB);
    nmm_runTaskList(3);

    for (i = 0; i < PCFI_SIZE_IN_BITS; i++) {
        /* scrambled sequence = input sequence xor'd with pseudorandom sequence */
        if (cvec[i] != 0) {
            /*
             * No need to do soft desrambling
             * bDetSoft[i] = -bDetSoft[i];
             */
            bDetHard[i] = !bDetHard[i];
        }
    }

    MemFree(cvec, CRAM_MEM);

    PACKBB((U8 *) bDetHard, cvecByteHard, 2 * PCFICH_NUM_RE, 0);
    DUMP_ARR_ID_UART(cvecByteHard, 4, "02x", 0, 1);

    cur_cfi = *p32;

    for (i = 0; i < MAX_CFI; i++) {
        U32             xcor = cur_cfi ^ CFI_DW[i];
        nmm_dbg_printf_off(("@@ PCFI xcor %x, set bits : %d\r\n", xcor, count_set_bits(xcor)));
        xcor = count_set_bits(xcor);
        if (xcor < min_xcor) {
            min_xcor = xcor;
            cfi_idx = (U8) i;
        }
    }

    /* 1.4 Mhz Case */
    if (current_cell->dl_tx_bw == NMAXRB_6)
        cfi_idx++;

    if (current_cell->phich_duration == PHICH_DUR_EXTENTED && cfi_idx < 2) {
        /*
         * Do not validate CFI, as for extended PHICH duration, we need at least 3 Ctrl sym
         */
        nmm_dbg_printf_off(("@@ CFI %d does not match MIB PHICH extented duration config \r\n", cfi_idx));
        return;
    }

    if ((cfi_idx < 0) || (cfi_idx > MAX_CFI + 1)) {
        nmm_dbg_printf_off(("**error: unexpected cfi value (%d)\r\n", cfi_idx));
        return;
    }

    current_cell->cfi_val = cfi_idx;
    current_cell->cfi_decoded++;
}

//-------------------------------------------------------------------------------------------
/** @brief PBCH Decode
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param current_cell		  Pointer to the cell parameter structure
 *  @param pbch 			  Pointer to the pbch data memory
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |current_cell |         |pInput/pOutput |1        |pStructure     |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pbch         |         |pInput         |1        |pComplex Array |(1:15:0)    |full range |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_decode_pbch       |
 *  --------------------------------------------------------
 *  |Input Streams       |current_cell,pbch                |
 *  --------------------------------------------------------
 *  |Output Streams      |current_cell					   |
 *  --------------------------------------------------------
 *  |Config Parameters   |none							   |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to decode PBCH
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.212
 *
 *  \ingroup group_lte_nmm_MIB 
 *
 **/
U8
lte_bs_phy_nmm_decode_pbch(CELL_PARAMS * current_cell, S16 * pbch)
{
    S16             i;
    S8             *bdet_soft;
    S8              by[PBCH_SIZE_IN_BITS];
    U8             *cvec;
    U8              dec_out[PBCH_SIZE_IN_BITS / 4];
    U8              crc_status;
    S16             subidx;
    U8              frame_index = 0;
    volatile U32    nmm_ticks;
    U8              num_antenna;
	S16				pbch_size;

    bdet_soft = MemAlloc(2 * PBCH_RE_PER_FRAME * sizeof(S8), CRAM_MEM, 1);
    lte_bs_phy_nmm_demod_pbch_tcb(NULL,pbch, current_cell->pbch_re_per_frame,current_cell->pbch_scale, bdet_soft,
        nmm_tcb_arr[3][1+NUM_SYMB_PBCH]);

    lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[3], (1+NUM_SYMB_PBCH), BYPASS_TCB);
    lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[3]+2+NUM_SYMB_PBCH, N_PBCH_TCB-(NUM_SYMB_PBCH+2), BYPASS_TCB);
    nmm_runTaskList(3);

    nmm_ticks = GET_TICKS;
    cvec = current_cell->pbch_scrambling_seq;

    /* blind decoding, as we do not know system frame number and cell num of antennas */
        num_antenna = current_cell->cell_antennas;
        /* reset 2 last bits of SFN */
        frame_index = 0;
		pbch_size = current_cell->pbch_re_per_frame * 8;
        /* try to indentify modulo 4 frame number by looking through 4 scrambled seq part */
        for (subidx = 0; subidx < pbch_size; subidx += (pbch_size / 4)) {
            for (i = 0; i < (pbch_size / 4); i++) {
                /* soft scrambled sequence = input sequence xor'd with pseudorandom sequence */
                by[i] = bdet_soft[i];
                if (cvec[i + subidx] != 0) {
                    if(by[i]==-128)
                        by[i]=127;
                    else
                        by[i] = -by[i];
                }
            }

            /* rate dematching decode, subblock deinterleave, viterbi decoding, antenna CRC scrambling & check */
            lte_bs_phy_nmm_pbch_chan_coding(by, dec_out, (pbch_size / 4), MIB_SIZE_IN_BITS, (S16) num_antenna, (S16) (-32768), &crc_status);

            if (crc_status)
                goto mib_decoded;

            frame_index++;
        }

        if (!crc_status) {
            goto nmm_decode_pbch_end;
        }

    MLogTask(PID_NMM_MIB_PBCH_DECODE, RESOURCE_LARM, nmm_ticks, GET_TICKS);

  mib_decoded:
    lte_bs_phy_nmm_mib_decode(current_cell, dec_out, frame_index);
    current_cell->mib_decoded_success_cnt++;

  nmm_decode_pbch_end:
    MemFree(bdet_soft, CRAM_MEM);

  return crc_status;
}

//-------------------------------------------------------------------------------------------
/** @brief PCFI index calculation
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param current_cell		  Pointer to the cell parameter structure
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |current_cell |         |pInput/pOutput |1        |pStructure     |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_get_pcfi_idx      |
 *  --------------------------------------------------------
 *  |Input Streams       |current_cell	                   |
 *  --------------------------------------------------------
 *  |Output Streams      |current_cell					   |
 *  --------------------------------------------------------
 *  |Config Parameters   |none							   |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to get pcfi index
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_SIB 
 *
 **/
void
lte_bs_phy_nmm_get_pcfi_idx(CELL_PARAMS * current_cell)
{
    S16             k_bar;
    S16             antenna_index = 0;  // Ntx_ant =<2;
    S16             temp = 0;
    S16             temp1,
                    temp16;
    S16             k,
                    j;
    S16             kk_bar,
                    kk_bar_mod;
    S16            *tab_ptrPdcch = NULL;
    S16             Ncell_id = current_cell->cell_pci;
    S16             Ndl_rb = current_cell->dl_tx_bw;
    S16             Nrb_sc = PRB_12;
    S16            *k_l_row0 = current_cell->pcfi_re_idx;
    S16            *RG_Assigned_PCFIH = current_cell->pcfi_rg_idx;

    k_bar = (Nrb_sc >> 1) * ((U16) Ncell_id % (Ndl_rb << 1));

    switch (Ncell_id % 3) {
    case 0:
        tab_ptrPdcch = REinSubfrmSym_PDCCH_ROT0;
        break;
    case 1:
        tab_ptrPdcch = REinSubfrmSym_PDCCH_ROT1;
        break;
    case 2:
        tab_ptrPdcch = REinSubfrmSym_PDCCH_ROT2;
        break;
    }

    temp1 = Ndl_rb * Nrb_sc;
    for (j = 0; j < 4; j++) {
        temp16 = (S16) (temp >> 1);
        temp += Ndl_rb;
        kk_bar = k_bar + (S16) temp16 *Nrb_sc / 2;
        kk_bar_mod = kk_bar % (S16) temp1;
        for (k = 0; k < QUADRUPLET; k++)
            k_l_row0[k + (j << 2)] = tab_ptrPdcch[3 * 12 * antenna_index + k] + kk_bar_mod;

        temp16 = divide_floor(kk_bar_mod, 6);
        RG_Assigned_PCFIH[j] = temp16;
    }
    DUMP_ARR_ID_UART(k_l_row0, QUADRUPLET * 4, "d", 0, 0);
    DUMP_ARR_ID_UART(RG_Assigned_PCFIH, QUADRUPLET * 4, "d", 0, 0);
}

//-------------------------------------------------------------------------------------------
/** @brief PHICH index calculation
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param current_cell		  Pointer to the cell parameter structure
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |current_cell |         |pInput/pOutput |1        |pStructure     |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_get_phich_idx      |
 *  --------------------------------------------------------
 *  |Input Streams       |current_cell	                   |
 *  --------------------------------------------------------
 *  |Output Streams      |current_cell					   |
 *  --------------------------------------------------------
 *  |Config Parameters   |none							   |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to get phich index
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_SIB 
 *
 **/
S16
lte_bs_phy_nmm_get_phich_idx(CELL_PARAMS * current_cell)
{
    S16             index_RB = 0;
    S16             N_PHICH_Grp,
                    N_PHICH_Grp_Prime;
    S16             Ncell_id = current_cell->cell_pci;
    S16             nDlRb = current_cell->dl_tx_bw;
    S16             hi_duration = current_cell->phich_duration;
    S16             index_Ng = current_cell->phich_resource;
    S16             Ntx_ant = current_cell->cell_antennas;
    S16            *RG_Assigned_PCFIH = current_cell->pcfi_rg_idx;
    S16             q,
                    k,
                    m;
    S16             antenna_index;
    S16             NUM = 0;
    S16             loc;
    U16             n_bar,
                    n_bar_mod;
    S16             ni[3],
                    ll[3],
                    l;
    S16             temp1,
                    temp2;
    S16            *RG_Available_PHICH,
                   *RG_temp;
    S16            *tab_ptrPdcch;
    S16             Nrb_sc = PRB_12;
    S16             cell_cp = current_cell->cell_cp;
    S16            *k_l_row0 = current_cell->phich_re_idx;
    S16            *k_l_row1 = current_cell->phich_sym_idx;

    switch (nDlRb) {
    case 6:
        index_RB = 0;
        break;
    case 15:
        index_RB = 1;
        break;
    case 25:
        index_RB = 2;
        break;
    case 50:
        index_RB = 3;
        break;
    case 75:
        index_RB = 4;
        break;
    case 100:
        index_RB = 5;
        break;
    }

    N_PHICH_Grp = TabN_PHICH_GRP[index_Ng * 6 + index_RB];
    if (cell_cp == LONG_CP) {
        N_PHICH_Grp = 2 * N_PHICH_Grp;
    }

    if (cell_cp == LONG_CP) {
        if ((N_PHICH_Grp % 2) == 1)
            N_PHICH_Grp_Prime = N_PHICH_Grp / 2 + 1;
        else
            N_PHICH_Grp_Prime = N_PHICH_Grp / 2;
    }
    else
        N_PHICH_Grp_Prime = N_PHICH_Grp;

    antenna_index = 0;
    if (Ntx_ant == 4)
        antenna_index = 1;

    tab_ptrPdcch = NULL;
    switch (Ncell_id % 3) {
    case 0:
        tab_ptrPdcch = REinSubfrmSym_PDCCH_ROT0;
        break;
    case 1:
        tab_ptrPdcch = REinSubfrmSym_PDCCH_ROT1;
        break;
    case 2:
        tab_ptrPdcch = REinSubfrmSym_PDCCH_ROT2;
        break;
    }

    if (hi_duration == 1)       /* consider only FDD and non-MBSFN for now */
        for (l = 0; l < 3; l++)
            ll[l] = l;
    else
        for (l = 0; l < 3; l++)
            ll[l] = 0;
    /*
     * Precompute number of REG not assigned to PCFICH in each of the first 3 symbols
     */

    /*
     * Symbol 0
     */
    ni[0] = 2 * nDlRb - 4;

    /* 2 because there are 2 REG(Quadruplet) per RB due to pilots 4, REG taken by PCFICH */
    RG_Available_PHICH = svector(0, ni[0] - 1);
    RG_temp = svector(0, 2 * nDlRb - 1);

    for (l = 0; l < 2 * nDlRb; l++) {
        RG_temp[l] = l;
    }

    for (k = 0; k < 4; k++) {
        RG_temp[RG_Assigned_PCFIH[k]]=-1;
    }


    m = 0;
    for (l = 0; l < 2 * nDlRb; l++) {
        if (RG_temp[l] != -1) {
            RG_Available_PHICH[m] = l;
            m = m + 1;
        }
    }

    /*
     * Symbol 1
     */
    if (Ntx_ant != 4)
        ni[SYMBOL(1)] = 3 * nDlRb;      /* no dtx pilots */
    else
        ni[SYMBOL(1)] = 2 * nDlRb;      /* dtx pilots */

    /*
     * Symbol 2
     */
    ni[SYMBOL(2)] = 3 * nDlRb;  /* no pilots */

    for (m = 0; m < N_PHICH_Grp_Prime; m++) {
        /* 3 REGs (12/4) */
        for (q = 0; q < 3; q++) {
            /*
             * !!!! Need FIX for high PCI values , Ncell_id * ni[ll[q]] > MAX16
             */
            temp1 = divide_floor((U16) (Ncell_id * ni[ll[q]]), (U16) ni[0]);
            temp2 = divide_floor((U16) (q * ni[ll[q]]), 3);
            n_bar = ((U16) (temp1 + m + temp2)) % ni[ll[q]];

            if (ll[q] == SYMBOL(0)) {
                n_bar = RG_Available_PHICH[n_bar];
                /* convert to REs */
                n_bar_mod = n_bar * (Nrb_sc / 2);
            }
            else if (ll[q] == SYMBOL(1)) {
                if (Ntx_ant != 4) {
                    n_bar_mod = n_bar_mod = n_bar * (Nrb_sc / 3);
                }
                else {
                    n_bar_mod = n_bar * (Nrb_sc / 2);
                }
            }
            else {
                n_bar_mod = n_bar * (Nrb_sc / 3);
            }

            for (k = 0; k < QUADRUPLET; k++) {
                loc = tab_ptrPdcch[3 * 12 * antenna_index + 12 * ll[q] + k] + n_bar_mod;
                k_l_row0[NUM] = loc;
                k_l_row1[NUM] = ll[q];
                NUM++;
            }
        }
    }

    DUMP_ARR_ID_UART(k_l_row0, NUM, "d", 0, 1);
    DUMP_ARR_ID_UART(k_l_row1, NUM, "d", 0, 1);

    free_svector(RG_Available_PHICH, 0, ni[0] - 1);
    free_svector(RG_temp, 0, 2 * nDlRb - 1);
    current_cell->num_phich_re = NUM;

    return NUM;
}


void
lte_bs_phy_nmm_extract_pdcch_ini(CELL_PARAMS * current_cell, S16 expoNfft, S16 bw, SYMBOL_INFO * sym, S16 * pdcch_re_idx, S16 num_pdcch_re, S16 * pdcch, S16 * expo_fac)
{

    S16            *pin = sym->start;
    S32             length = (1 << expoNfft);
    S16             nb_subcarriers_of_interest = bw * PRB_12;
    S16             i;
    S16             fft_out[2 * FFTSZ_2048];
    S16             re_array[2 * FFTSZ_2048];

    nmm_dbg_printf_off(("[%s]\r\n", __FUNCTION__));
    /* skip CP */
    DUMP_ARR_ID_UART(pin, 20, "d", 1, 0);
    pin = pin + sym->cp_size * 2;

    /* FFT */
    fftcg_c_16t24d(pin, (int) expoNfft, fft_out, expo_fac);
    DUMP_ARR_ID_UART(fft_out, 20, "d", 1, 0);

    /* FFT to RE mapping + DC skipping */
    lte_bs_phy_swap_fftout_dcskip(fft_out, re_array, nb_subcarriers_of_interest / 2, (S32) (2 * length - nb_subcarriers_of_interest), expo_fac);

    sym->fft_done = TRUE;

    /* PDCCH RE extraction */
    for (i = 0; i < num_pdcch_re; i++) {
        pdcch[2 * i] = re_array[2 * pdcch_re_idx[i]];
        pdcch[2 * i + 1] = re_array[2 * pdcch_re_idx[i] + 1];
    }

    current_cell->pdcch_extracted_cnt++;
}


void
printmessage(int gui)
{

    nmm_dbg_printf_off(("\t== [%d] NMM_RSSI_MEAS_IND==\r\n", gui));


}


//-------------------------------------------------------------------------------------------
/** @brief SSS Detection
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param pcell_search	    Pointer to cell_search parameter structure
 *  @param IQdecimated		Pointer to input data memory
 *  @param PSyncFound_cand	Pointer to PSS search result
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pcell_search |         |pInput/pOutput	|1        |pStructure         |  |  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |IQdecimated  |         |pInput		    |         |pComplex Array |(1:0:15)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  |PSyncFound_cand |      |pInput		    |         |pReal Array |(1:0:15)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_ssync_loop		   |
 *  --------------------------------------------------------
 *  |Input Streams       |pcell_search, IQdecimated, PSyncFound_cand |
 *  --------------------------------------------------------
 *  |Output Streams      |pcell_search		   |
 *  --------------------------------------------------------
 *  |Config Parameters   |none   						   |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function do SSS detection including four processes.
 *  1. estimate the frequency offset based on PSS data, then compensate the foe
 *	2. do FFT on PSS and SSS symbol
 *  3. demodulate SSS data with channel estimation
 *  4. SSS detection
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_cell_search 
 *
 **/
void
lte_bs_phy_nmm_ssync_loop(LISTENING_CELL_SEARCH_DESC * pcell_search, S16 * IQdecimated, S16 * PSyncFound_cand)
{
    S16            *pin,
                    PSyncFound,
					startPos;
    S16             i;

    sync_cand = 0;
    sync_cand_tcb_track = NMM_SYNC_FFT_SCP0_TCB;

    for (i = 0; i < MAX_CELL_FOUND_LIST; i ++) {
        PSyncFound = PSyncFound_cand[i*2];

        /* if a candidate PSS is found we try to find PSS */
        if (PSyncFound < 3) {
            S16            *PSync_f;
            S16            *SSync0_f;
            S16            *SSync0Exp,
                           *PSyncExp;
            S16            *SSync1_f;
            S16            *SSync1Exp;
            S16            *SSync2_f;
            S16            *SSync2Exp;
            S16            *SSync0Est;
            S16            *SSync1Est;
            S16            *SSync2Est;
            S16             NID_2;
            S16            *pci0,
                           *subframe0;
            S16            *pci1,
                           *subframe1;
            S16            *pci2,
                           *subframe2;
            S32            *PSync_foe;
            S16            *data_SSS_PSS;
            PLISTENINGDESC pListening = LteBsGetNmmCtx();

            /* keep track of PSS candidtate timing */
            nmm_dbg_printf_off(("@@@ PSS candidate at root %d startPos %d peak %d pnr %d \r\n", PSyncFound, PSyncFound_cand[i*2+1],
				PSyncFound_cand[i*2 + MAX_CELL_FOUND_LIST*2], PSyncFound_cand[i*2 + MAX_CELL_FOUND_LIST*2+1]));

            /* tmp work-around - should never happen */
            if (sync_cand == MAX_NUM_CAND)
                return;

			NID_2 = PSyncFound_cand[i*2];
			startPos = PSyncFound_cand[i*2+1] * 2;
			if (startPos < (2 * (FFT_SIZE_096MHZ - 1)))
				pin = &pcell_search->cell_search_xcorr_buf[startPos + (2 * SSS_SEARCH_RANGE)];
			else
				pin = &IQdecimated[startPos - (2 * (FFT_SIZE_096MHZ - 1))];

            /* start first with short CP, case shifted by 5 */
            PSync_f = (S16 *) MemAlloc(2 * FFT_SIZE_096MHZ * sizeof(S16), CRAM_MEM, 1);
            PSyncExp = (S16 *) MemAlloc(sizeof(S16), CRAM_MEM, 1);

            SSync0_f = (S16 *) MemAlloc(2 * FFT_SIZE_096MHZ * sizeof(S16), CRAM_MEM, 1);
            SSync0Exp = (S16 *) MemAlloc(sizeof(S16), CRAM_MEM, 1);

            SSync1_f = (S16 *) MemAlloc(2 * FFT_SIZE_096MHZ * sizeof(S16), CRAM_MEM, 1);
            SSync1Exp = (S16 *) MemAlloc(sizeof(S16), CRAM_MEM, 1);

            SSync2_f = (S16 *) MemAlloc(2 * FFT_SIZE_096MHZ * sizeof(S16), CRAM_MEM, 1);
            SSync2Exp = (S16 *) MemAlloc(sizeof(S16), CRAM_MEM, 1);

            SSync0Est = (S16 *) MemAlloc(2 * FFT_SIZE_096MHZ * sizeof(S16), CRAM_MEM, 1);
            SSync1Est = (S16 *) MemAlloc(2 * FFT_SIZE_096MHZ * sizeof(S16), CRAM_MEM, 1);
            SSync2Est = (S16 *) MemAlloc(2 * FFT_SIZE_096MHZ * sizeof(S16), CRAM_MEM, 1);

            pci0 = MemAlloc(sizeof(S16), CRAM_MEM, 1);
            subframe0 = MemAlloc(sizeof(S16), CRAM_MEM, 1);
            pci1 = MemAlloc(sizeof(S16), CRAM_MEM, 1);
            subframe1 = MemAlloc(sizeof(S16), CRAM_MEM, 1);
            pci2 = MemAlloc(sizeof(S16), CRAM_MEM, 1);
            subframe2 = MemAlloc(sizeof(S16), CRAM_MEM, 1);

            data_SSS_PSS = (S16 *) MemAlloc(2 * (FFT_SIZE_096MHZ * 2 + 16) * sizeof(S16), CRAM_MEM, 1);
            PSync_foe = (S32 *) MemAlloc(sizeof(S32), CRAM_MEM, 1);

            PSync_f_arr[sync_cand] = PSync_f;
            PSyncExp_arr[sync_cand] = PSyncExp;

            SSync0_f_arr[sync_cand] = SSync0_f;
            SSync0Exp_arr[sync_cand] = SSync0Exp;

            SSync1_f_arr[sync_cand] = SSync1_f;
            SSync1Exp_arr[sync_cand] = SSync1Exp;

            SSync2_f_arr[sync_cand] = SSync2_f;
            SSync2Exp_arr[sync_cand] = SSync2Exp;

            SSync0Est_arr[sync_cand] = SSync0Est;
            SSync1Est_arr[sync_cand] = SSync1Est;
            SSync2Est_arr[sync_cand] = SSync2Est;

            data_SSS_PSS_arr[sync_cand] = data_SSS_PSS;

            pci0_arr[sync_cand] = pci0;
            pci1_arr[sync_cand] = pci1;
            pci2_arr[sync_cand] = pci2;
            subframe0_arr[sync_cand] = subframe0;
            subframe1_arr[sync_cand] = subframe1;
            subframe2_arr[sync_cand] = subframe2;

            Psync_foe_arr[sync_cand] = PSync_foe;

            // FOE based on PSS and correct the frequency offset
            memcpy(data_SSS_PSS, pin - (SSS_SEARCH_RANGE * 2), 2 * (FFT_SIZE_096MHZ + SSS_SEARCH_RANGE) * sizeof(S16));
            lte_bs_phy_nmm_pss_foe_tcb(nmm_task_list[3], data_SSS_PSS, NID_2, PSync_foe, nmm_tcb_arr[3][3+NUM_SYMB_PBCH]);

            /*bypass the following TCBs*/
            lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[3], 3+NUM_SYMB_PBCH, BYPASS_TCB);
            lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[3] + 4+NUM_SYMB_PBCH, N_PBCH_TCB - (4+NUM_SYMB_PBCH), BYPASS_TCB);
            nmm_runTaskList(3);

            lte_bs_phy_nmm_fft64_tcb(nmm_task_list[1], data_SSS_PSS + ((FFT_SIZE_096MHZ + 16) * 2), PSync_f, PSyncExp,nmm_tcb_arr[1][0]);
            lte_bs_phy_nmm_fft64_tcb(nmm_task_list[1], data_SSS_PSS + 11 * 2, SSync0_f, SSync0Exp,nmm_tcb_arr[1][1]);
            lte_bs_phy_nmm_fft64_tcb(nmm_task_list[1], data_SSS_PSS + 12 * 2, SSync1_f, SSync1Exp,nmm_tcb_arr[1][2]);
            lte_bs_phy_nmm_fft64_tcb(nmm_task_list[1], data_SSS_PSS, SSync2_f, SSync2Exp,nmm_tcb_arr[1][3]);


            lte_bs_phy_nmm_chanest_tcb(nmm_task_list[1], SSync0_f, PSync_f, SSync0Est, NID_2, SHORT_CP, NULL, 0,nmm_tcb_arr[1][4]);
            lte_bs_phy_nmm_chanest_tcb(nmm_task_list[1], SSync1_f, PSync_f, SSync1Est, NID_2, SHORT_CP, NULL, 0,nmm_tcb_arr[1][5]);
            lte_bs_phy_nmm_chanest_tcb(nmm_task_list[1], SSync2_f, PSync_f, SSync2Est, NID_2, LONG_CP, NULL, 0,nmm_tcb_arr[1][6]);


            lte_bs_phy_nmm_sync_decode_sss_tcb(nmm_task_list[1], SSync0Est, NID_2, pci0, subframe0, NULL,0,nmm_tcb_arr[1][7]);
            lte_bs_phy_nmm_sync_decode_sss_tcb(nmm_task_list[1], SSync1Est, NID_2, pci1, subframe1, NULL,0,nmm_tcb_arr[1][8]);
            lte_bs_phy_nmm_sync_decode_sss_tcb(nmm_task_list[1], SSync2Est, NID_2, pci2, subframe2, NULL,0,nmm_tcb_arr[1][9]);


            /*
             * Run second task list: fft, chanest, decode.
             */
            nmm_runTaskList(1);

            sync_cand++;
        }
		else
			break;
    }
}


/*
 * Cross correlation
 */
void
lte_bs_phy_nmm_pss_xcorr(S16 * x, S16 * y, S16 * xc, S16 len)
{
    S32             i,
                    ishf;
    S16             a,
                    b,
                    c,
                    d;
    S32             ac,
                    bd,
                    ad,
                    bc;
    S32             AccR,
                    AccI;

    ishf = 6;
    AccR = 0;
    AccI = 0;
    for (i = 0; i < len; i++) {
        a = x[2 * i];
        b = -x[2 * i + 1];
        c = y[2 * i];
        d = y[2 * i + 1];

        ac = L_mult(a, c) >> ishf;
        bd = L_mult(b, d) >> ishf;
        ad = L_mult(a, d) >> ishf;
        bc = L_mult(b, c) >> ishf;

        AccR = L_add(AccR, ac);
        AccR = L_sub(AccR, bd);
        AccI = L_add(AccI, ad);
        AccI = L_add(AccI, bc);
    }
    a = extract_h(AccR);
    b = extract_h(AccI);
    AccR = L_mult(a, a);
    AccR = L_mac(AccR, b, b);

    AccR = L_shl(AccR, 4);

    *xc = extract_h(AccR);
}


//-------------------------------------------------------------------------------------------
/** @brief PDCCH Index Calculation
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param current_cell Pointer to the cell parameter structure
 *  @param sym_idx      symbol index 
 *  @param ppdcch       Pointer to the output PDCCH index memory
 *
 *  @return num_pdcch_re Number of PDCCH RE
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |current_cell |         |pInput         |1        |pStructure     |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |sym_idx      |         |Input          |1        |signed int     |(1:15:0)   |[0:13]      |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |ppdcch       |         |pOutput        |         |pReal Array    |(1:15:0)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |num_pdcch_re |         |Output         |1        |signed int     |(1:15:0)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_get_pdcch_idx     |
 *  --------------------------------------------------------
 *  |Input Streams       |current_cell                     |
 *  --------------------------------------------------------
 *  |Output Streams      |ppdcch                           |
 *  --------------------------------------------------------
 *  |Config Parameters   |sym_idx                          |
 *  --------------------------------------------------------
 *  |Status Parameters   |num_pdcch_re                     |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to calculate the PDCCH index
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_SIB 
 *
 **/
S16
lte_bs_phy_nmm_get_pdcch_idx(CELL_PARAMS * current_cell, S16 sym_idx, S16 * ppdcch)
{
    S16            *pcfi_re_idx = current_cell->pcfi_re_idx;
    S16            *phich_re_idx = current_cell->phich_re_idx;
    S16             num_phich_re = current_cell->num_phich_re;
    S16             subcarriers = current_cell->dl_tx_bw * PRB_12;
    S16             i;
    S16             num_pdcch_re = 0;
    U16             minVshift = current_cell->cell_pci%3;
#if 1
    S16            *phich_sym_idx = current_cell->phich_sym_idx;
    //S16             num_pilot_and_dtx = current_cell->dl_tx_bw * 4;
    U8             *pdcch_re_map;


    pdcch_re_map = MemAlloc(subcarriers * sizeof(U8), DDR_MEM, 1);


    /* mark hich re */
    for (i = 0; i < num_phich_re; i++)
    {
        if(phich_sym_idx[i]==sym_idx)
        {
            pdcch_re_map[phich_re_idx[i]] = 1;
        }
    }

    /* cycle consuming method but need only be done once */

    /* If symbol 0 we must remove PCFICH, PHICH, DTX and pilots */

    if (sym_idx == SYMBOL(0)) {

        /* mark cfi re */
        for (i = 0; i < PCFICH_NUM_RE; i++)
            pdcch_re_map[pcfi_re_idx[i]] = 1;
    }

    /* If symbol 1 and >2 antennas remove DTX and pilots else take the whole column */

    if (sym_idx == SYMBOL(1)) {

        if (current_cell->cell_antennas <= 2) {
            minVshift = 4; /*No RS*/
        }
    }

    /* If symbol 2 or 3 take thw whole column */

    if (sym_idx == SYMBOL(2)) {
         minVshift = 4; /*No RS*/
    }

    if (sym_idx == SYMBOL(3)) {
        if(current_cell->cell_cp == SHORT_CP)
        {
            minVshift = 4; /*No RS*/
        }
    }
    /* symbol 0 */
    for (i = 0; i < subcarriers; i++) {
        if ((pdcch_re_map[i] == 0) && (i % 3 != minVshift)) {
            *ppdcch++ = i;
            num_pdcch_re++;
        }
    }

     MemFree(pdcch_re_map, DDR_MEM);
#else
    U8             *pdcch_re_map;

    pdcch_re_map = MemAlloc(subcarriers * sizeof(U8), DDR_MEM, 1);

    /* mark cfi re */
    for (i = 0; i < PCFICH_NUM_RE; i++)
        pdcch_re_map[pcfi_re_idx[i]] = 1;

    /* mark hich re */
    for (i = 0; i < num_phich_re; i++)
        pdcch_re_map[phich_re_idx[i]] = 1;

    /* symbol 0 */
    for (i = 0; i < subcarriers; i++) {
        if ((pdcch_re_map[i] == 0) && (i % 3 != 0)) {   /* !FIXME! hard-coded */
            *ppdcch++ = i;
            num_pdcch_re++;
        }
    }

    MemFree(pdcch_re_map, DDR_MEM);

#endif
    return num_pdcch_re;
}


void
                lte_bs_phy_nmm_pdcch_chan_decoding_cref(S8 in[], U8 out[], S32 nin, S32 nout, S16 nant, S16 scf, U8 * stat);

                
//-------------------------------------------------------------------------------------------
/** @brief PDCCH Decode
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param current_cell Pointer to the cell parameter structure
 *  @param slot_num     slot index 
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |current_cell |         |pInput/pOutput |1        |pStructure     |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |slot_num     |         |Input          |1        |signed int     |(1:15:0)   |[0:19]      |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_decode_pdcch      |
 *  --------------------------------------------------------
 *  |Input Streams       |current_cell                     |
 *  --------------------------------------------------------
 *  |Output Streams      |current_cell                     |
 *  --------------------------------------------------------
 *  |Config Parameters   |slot_num                         |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to blind search PDCCH with format 1A and 1C
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.212
 *
 *  \ingroup group_lte_nmm_SIB 
 *
 **/
void
lte_bs_phy_nmm_decode_pdcch(CELL_PARAMS * current_cell, S16 slot_num)
{
    S16             i,
                    k,
                    Quad,
                    NusedE = 0;

    // S32 *pdcch32;
    S8             *bDetSoft;
    // S16 NShift;
    U8             *dec_out;
    U8              crc_status;
    U32             cinit = 0;
    S8              data_aggregated_8[2 * PDCCH_FORMAT3_NBITS];
//    S8              data_aggregated_4[2 * PDCCH_FORMAT2_NBITS];
    S16             temp;
    // S16 numPDCCHQuadruplets;
    S16            *pdcch = NULL;

    S16             *pdcch_colum[MAX_PDCCH_COLUMN];

    U16             *pbcch_indices_column[MAX_PDCCH_COLUMN] ;

    U16             index_column[MAX_PDCCH_COLUMN];

    U16             div_fac[MAX_PDCCH_COLUMN] = {CONST_1_6,16384,16384,16384};
    U16             mult_fac[MAX_PDCCH_COLUMN] = {6,4,4,4};
    U32             tmpU32;


    U16             num_done_column[MAX_PDCCH_COLUMN] = {0,0,0,0};
    U16             total_read,N_pdcch_col,read_flag;
    volatile U32    nmm_ticks;
    /**/
    U16             n_DCI_L,N_DCI_L=2;
    U16             DCI_L_set[2] = {8,4};
    U16             DCI_M_set[2] = {2,4};
    U16             N_bits_L;
    U8              n_DCI_size,N_DCI_size=2;

    for(i=0;i<MAX_PDCCH_COLUMN;i++)
    {
        pdcch_colum[i] = current_cell->pdcch_buf[SYMBOL(i)];
        pbcch_indices_column[i] = current_cell->pdcch_index[SYMBOL(i)];
    }
    if(current_cell->cell_antennas==4)
    {
        div_fac[1] = CONST_1_6;
        mult_fac[1] = 6;
    }
    if(current_cell->cell_cp==LONG_CP)
    {
        div_fac[3] = CONST_1_6;
        mult_fac[3] = 6;
    }
    nmm_dbg_printf_off(("@@ %s (cfi %d)\r\n", __FUNCTION__, current_cell->cfi_val));

    /* First we need to re-order the quadruplets taken on the 1-3 OFDM symbols according to CFI */


    NusedE = 0;
    N_pdcch_col = current_cell->cfi_val+1;
    for(i=0;i<N_pdcch_col;i++)
    {
        tmpU32 = (*pbcch_indices_column[i]++)*div_fac[i];
        index_column[i] = extract_h(tmpU32);
        index_column[i] = index_column[i] * mult_fac[i];
        NusedE += current_cell->pdcch_re_num[i];
    }

    pdcch = (S16 *) MemAlloc(NusedE * 2 * sizeof(S16),CRAM_MEM,1);

    total_read = 0;

    while(total_read < NusedE)
    {
        for(i=0;i<N_pdcch_col;i++)
        {
            read_flag = 1;
            for(k=0;k<N_pdcch_col;k++)
            {
                if(k!=i)
                {
                    if((index_column[i] > index_column[k]) &&
                        (num_done_column[k] < current_cell->pdcch_re_num[k]))
                        read_flag = 0;
                }
            }
            read_flag &= num_done_column[i] < current_cell->pdcch_re_num[i];
            if(read_flag == 1)
            {
                for (Quad = 0; Quad < 4; Quad++) {
                    *pdcch++ = *pdcch_colum[i]++; /* real part */
                    *pdcch++ = *pdcch_colum[i]++; /* imaginary part */


                }
                num_done_column[i]+=4;
                pbcch_indices_column[i] += 4;
                tmpU32 = (*pbcch_indices_column[i])*div_fac[i];
                index_column[i] = extract_h(tmpU32)*mult_fac[i];

            } //read
        } //col loop

        total_read = 0;
        for(i=0;i<N_pdcch_col;i++)
        {
            total_read += num_done_column[i];
        }
    } // while


    pdcch -= NusedE*2;

    dec_out = MemAlloc(2 * NusedE * sizeof(U32), DDR_MEM, 1);
    bDetSoft = MemAlloc(2 * NusedE * sizeof(U32), CRAM_MEM, 1);
#if ORIG_CODE
    // numPDCCHQuadruplets = divide_floor(NusedE, QUADRUPLET);
    numPDCCHQuadruplets = NusedE >> 2;
    lte_bs_phy_nmm_reshuffle_pdcch_bits(current_cell->cell_pci, pdcch, pdcch, numPDCCHQuadruplets, 2);

    DUMP_ARR_ID_UART(pdcch, (2 * NusedE), "d", 0, 1);

    /* Convert to 32 bit PDCCH content with individual symbol scaling */
    pdcch32 = MemAlloc(2 * NusedE * sizeof(U32), CRAM_MEM, 1);
    bDetSoft = MemAlloc(2 * NusedE * sizeof(U32), CRAM_MEM, 1);

    for (k = 0; k < NusedE; k++) {
        pdcch32[2 * k] = *pdcch++;
        pdcch32[2 * k + 1] = *pdcch++;
    }
#endif

#if ORIG_CODE

    NShift = block_lnorm(pdcch32, (2 * NusedE), 0);
    DUMP_ARR_ID_UART(pdcch32, (2 * NusedE), "d", 0, 1);

    /* Demodulate the PDCCH */
    lte_bs_phy_nmm_demod_qpsk(pdcch32, bDetSoft, NShift, (S32) (2 * NusedE));
    DUMP_ARR_ID_UART(bDetSoft, 2 * NusedE, "d", 0, 1);

    /*
     * PDCCH scrambling sequence init, depend on slot ID and PCI
     */
    temp = slot_num >> 1;
    temp = temp << 9;

    cinit = temp + current_cell->cell_pci;

    lte_bs_phy_nmm_pseudo_random_seq_gen_tcb(NULL, current_cell->pdcch_re_num[0], cinit, current_cell->pdcch_scrambling_seq);
    DUMP_ARR_ID_UART(current_cell->pdcch_scrambling_seq, 2 * NusedE, "d", 0, 1);

    for (i = 0; i < 2 * NusedE; i++) {
        /* descrambled sequence = input sequence xor'd with pseudorandom sequence */
        if (current_cell->pdcch_scrambling_seq[i] != 0) {
            bDetSoft[i] = -bDetSoft[i];
        }
    }

#else
    temp = slot_num >> 1;
    temp = temp << 9;
    cinit = temp + current_cell->cell_pci;

    lte_bs_phy_nmm_pdcch_demod_tcb(NULL, pdcch, NusedE, current_cell->cell_pci,
		bDetSoft, NusedE, cinit, current_cell->pdcch_scrambling_seq,
        nmm_tcb_arr[4][NMM_PDCCH_DEMOD_TCB]);

    lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[4], NMM_PDCCH_DEMOD_TCB, BYPASS_TCB);
    lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[4]+NMM_PDCCH_DEMOD_TCB+1, (N_PDSCH_tcb-NMM_PDCCH_DEMOD_TCB-1), BYPASS_TCB);
    nmm_runTaskList(4);

#endif

    DUMP_ARR_ID_UART(bDetSoft, 2 * NusedE, "d", 0, 1);

    /*
     *   Blind decoding: only common space needed, i.e., 6 candidates (2 candidates with aggregation 8 and
     *   4 candidates with aggregation 4)
     */

    nmm_ticks = GET_TICKS;

    /*
    U16             n_DCI_size,N_DCI_size=2;*/

    for (n_DCI_L=0;n_DCI_L<N_DCI_L;n_DCI_L++)
    {
        N_bits_L = DCI_L_set[n_DCI_L]*BITS_PER_CCE;

        for (k = 0; k < DCI_M_set[n_DCI_L]; k++)
        {

            if(((k + 1) * N_bits_L) <= (2 * NusedE))
            {
                for (i = k * N_bits_L; i < (k + 1) * N_bits_L; i++)
                    data_aggregated_8[i - k * N_bits_L] = bDetSoft[i];

                for(n_DCI_size=0;n_DCI_size<N_DCI_size;n_DCI_size++)
                {
                    lte_bs_phy_nmm_pdcch_chan_decoding(data_aggregated_8, dec_out, N_bits_L,
                        current_cell->SI_DCI_size[n_DCI_size], (S16) current_cell->cell_antennas, (S16) (-32768),
                        &crc_status);
                    DUMP_ARR_ID_UART(dec_out, current_cell->SI_DCI_size[n_DCI_size], "d", 0, 1);

                    if (crc_status) {
                        nmm_dbg_printf_off(("VALID SI-RNTI DCI FOUND from aggreg %d",DCI_L_set[n_DCI_L]));
                        current_cell->dci_type_1a = n_DCI_size;
                        goto dci_decoded;
                    }
                }
            }

        }
    }

    current_cell->pdcch_extracted_cnt--;
    goto end_decode_pdcch;

  dci_decoded:
    current_cell->pdcch_decoded = 1;
    current_cell->pdcch_extracted_cnt--;        /* we decrement it so that in the future we look for the nest one */
    for (i = 0; i < MAX_N_DCI_BYTE; i++)
        current_cell->dci_data[i] = dec_out[i];

  end_decode_pdcch:
    MemFree(bDetSoft, CRAM_MEM);
    MemFree(dec_out, DDR_MEM);
    MemFree(pdcch,CRAM_MEM);
    MLogTask(PID_NMM_SIB1_PDCCH_DECODE, RESOURCE_LARM, nmm_ticks, GET_TICKS);
}


//-------------------------------------------------------------------------------------------
/** @brief DVRB Mapping
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param gapType    slot index 
 *  @param start_RB   slot index 
 *  @param L_RB		  slot index 
 *  @param bw_index   slot index 
 *  @param PRB_set    Pointer to PRB set memory
 *
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |gapType      |         |Input          |1        |signed int     |(1:15:0)   |[0:19]      |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |start_RB     |         |Input          |1        |signed int     |(1:15:0)   |[0:19]      |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |L_RB		  |         |Input          |1        |signed int     |(1:15:0)   |[0:19]      |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |bw_index	  |         |Input          |1        |signed int     |(1:15:0)   |[0:19]      |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |PRB_set	  |         |pOutput		|         |pReal Array    |(1:15:0)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_decode_pdcch       |
 *  ---------------------------------------------------------
 *  |Input Streams       |none								|
 *  ---------------------------------------------------------
 *  |Output Streams      |PRB_set							|
 *  ---------------------------------------------------------
 *  |Config Parameters   |gapType, start_RB, L_RB, bw_index |                         |
 *  ---------------------------------------------------------
 *  |Status Parameters   |none		                        |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to map the DVRB to PRB
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_SIB 
 *
 **/
static void DVRB2PRB(U8 gapType,U16 start_RB,U16 L_RB,U16 bw_index,
              U8 *PRB_set)
{
    U16 N_t_DL_VRB = N_t_DL_VRB_LUT[gapType][bw_index];
    U16 inv_N_t_DL_VRB = inv_N_t_DL_VRB_LUT[gapType][bw_index];
    U16 N_row = DVRB_N_row_LUT[gapType][bw_index];
    U16 N_null = 4 * N_row - N_t_DL_VRB;
    U16 n_VRB,n_RB,n_a_RB;
    U16 q_num,n_t_VRB;
    U16 n_t_PRB_offset,n_t_PRB[2];
    U32 tmpU32;
    U16 Npart1 = N_t_DL_VRB - N_null;
    U16 n_TS;
    U16 min_n_PRB,min_n_PRB_index;
    U8* p_PRB_set;

    for(n_RB=0;n_RB<L_RB;n_RB++)
    {
        n_VRB = start_RB+n_RB;
        tmpU32 = n_VRB * inv_N_t_DL_VRB;
        q_num = tmpU32 >> 16;
        n_t_PRB_offset = q_num * N_t_DL_VRB;
        n_t_VRB = n_VRB - n_t_PRB_offset;


        n_t_PRB[0] = N_row * (n_t_VRB & 3) + (n_t_VRB >> 2);

        if(N_null != 0)
        {
            if(n_t_VRB >= Npart1)
            {
                if((n_t_VRB&1) == 1)
                {
                    n_t_PRB[0] = N_row + (n_t_VRB >> 1);
                }
                else
                {
                    n_t_PRB[0] = (n_t_VRB >> 1) - N_row + (N_null >> 1);
                }

            }
            else
            {
                if((n_t_VRB & 3) >= 2)
                {
                    n_t_PRB[0] -= (N_null >> 1);
                }
            }
        }

        n_t_PRB[1] = n_t_PRB[0] + (N_t_DL_VRB>>1);
        if(n_t_PRB[1] >= N_t_DL_VRB)
            n_t_PRB[1] -= N_t_DL_VRB;

        for(n_TS=0; n_TS < N_TS_SUBFRM; n_TS++)
        {
            p_PRB_set = PRB_set + n_TS * L_RB;
            p_PRB_set[n_RB] = n_t_PRB[n_TS] + n_t_PRB_offset;
            if(p_PRB_set[n_RB] >= (N_t_DL_VRB>>1))
                p_PRB_set[n_RB] += (N_gap_LUT[gapType][bw_index] - (N_t_DL_VRB>>1));
        }

    }
    /*sort the PRBs*/
    for(n_TS=0; n_TS < N_TS_SUBFRM; n_TS++)
    {
        p_PRB_set = PRB_set + n_TS * L_RB;
        for(n_RB=0;n_RB<L_RB;n_RB++)
        {
            min_n_PRB = p_PRB_set[n_RB];
            min_n_PRB_index = (U8)n_RB;
            for(n_a_RB=n_RB+1;n_a_RB<L_RB;n_a_RB++)
            {
                if(p_PRB_set[n_a_RB] < min_n_PRB)
                {
                    min_n_PRB = p_PRB_set[n_a_RB];
                    min_n_PRB_index = (U8)n_a_RB;
                }
            }
            /*swap*/
            if(min_n_PRB_index != n_RB)
            {
                p_PRB_set[min_n_PRB_index] = p_PRB_set[n_RB];
                p_PRB_set[n_RB] = (U8)min_n_PRB;
            }
        }
    }
}


//-------------------------------------------------------------------------------------------
/** @brief DCI Decode
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param current_cell Pointer to the cell parameter structure
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |current_cell |         |pInput/pOutput |1        |pStructure     |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_decode_dci        |
 *  --------------------------------------------------------
 *  |Input Streams       |current_cell                     |
 *  --------------------------------------------------------
 *  |Output Streams      |current_cell                     |
 *  --------------------------------------------------------
 *  |Config Parameters   |non	                           |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to decode DCI of PDCCH
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.212
 *
 *  \ingroup group_lte_nmm_SIB 
 *
 **/
void
lte_bs_phy_nmm_decode_dci(CELL_PARAMS * current_cell)
{
    S16             resource_allocation_bits;
    U8             *dci_bits;
    U8              i;
    U8              current_bit = 0;
    U8              num_allocated_RBs_forTBS;   // for transport block size
    U8              TBS_index;  // for transport block size
    U16             q_num,r_num,N_PRB=0,RB_start=0;
    U16             N_DL_RB_LUT[6]={6,15,25,50,75,100};
    U32             inv_N_DL_RB_LUT[6]={715827883,286331154,171798692,85899346,57266231,42949673};//Q(32,0)
    U32             tmpU32,tmpU32_2,inv_N_DL_RB;
    U16             tmpU16;
    U16             N_step_RB;
    U16 N_a_DL_VRB,inv_N_a_DL_VRB;
    U16             rv_k;

    dci_bits = (U8 *) MemAlloc(2 * current_cell->SI_DCI_size[1] * sizeof(U8), DDR_MEM, 1);

    if (current_cell->dci_type_1a == 1) {
        /*
         *   Format 1A: (see TR 36.212, section 5.3.3.1.3) -> THIS IS A LONGER ASSIGNMENT
         *   first bit should be 1 (if not it is not SI-RNTI): 1bit
         *   Localized/Distributed VRB assignment flag : 1bit
         *   Resource block assignment : log2( Nrb * (Nrb + 1)/2 ) bits
         *   Modulation and coding: 5 bits
         *   HARQ process number:  3 bits (FDD) or 4 bits (TDD)
         *   New data indicator: 1 bit
         *   Redundancy version: 2 bits
         *   TPC command for PUCCH: 2 bits
         *   Downlink Assignment Index (only in TDD!): 2bits
         */



        UNPACKBB(current_cell->dci_data, dci_bits, current_cell->SI_DCI_size[1], 0);
        DUMP_ARR_ID_UART(dci_bits, current_cell->SI_DCI_size[1], "d", 0, 1);

        if (dci_bits[current_bit++] == 1) {  // this is a 1A message so first bit should be 1
            /* localized/distributed */
            current_cell->localized_distributed = dci_bits[current_bit++];

            /* resource allocation */
            resource_allocation_bits = DCI_1A_RB_ALLOCATION_BITS[current_cell->bw_index];
            current_cell->rb_allocation = 0;
            for (i = 0; i < resource_allocation_bits; i++)
                current_cell->rb_allocation += dci_bits[current_bit++] << (resource_allocation_bits - i - 1);

            /*RIV decoding*/
            for(i=0;i<6;i++)
            {
                if(N_DL_RB_LUT[i]==current_cell->dl_tx_bw)
                    break;
            }
            inv_N_DL_RB = inv_N_DL_RB_LUT[i];
            tmpU16 = inv_N_DL_RB & 0xffff;
            tmpU32 = current_cell->rb_allocation*tmpU16;
            tmpU32 = tmpU32 >> 16;

            tmpU16 = inv_N_DL_RB >> 16;
            tmpU32_2 = current_cell->rb_allocation*tmpU16;
            tmpU32_2 += tmpU32;

            q_num = tmpU32_2 >> 16;

            r_num = current_cell->rb_allocation - q_num*current_cell->dl_tx_bw;
            if((q_num+r_num)<current_cell->dl_tx_bw)
            {
                N_PRB = q_num+1;
                RB_start = r_num;
            }
            else
            {
                N_PRB = current_cell->dl_tx_bw+1-q_num;
                RB_start = current_cell->dl_tx_bw-1-r_num;
            }
            if(current_cell->localized_distributed == 0)
            {
                for(i=(U8)RB_start;i<(U8)(RB_start+N_PRB);i++)
                {
                    current_cell->PDSCH_PRB_set[i-RB_start]=(U8)i;
                    current_cell->PDSCH_PRB_set[N_PRB+i-RB_start]=(U8)i;
                }
            }

            /* modulation and coding */
            current_cell->modulation_coding = 0;
            for (i = 0; i < 5; i++)
                current_cell->modulation_coding += dci_bits[current_bit++] << (5 - i - 1);

            /* harq */
            for (i = 0; i < 3; i++)
                current_bit++;

            /* gap value (NDI)*/
            current_cell->gap_value = 0;
            if((current_cell->dl_tx_bw >= 50) && (current_cell->localized_distributed==1))
                current_cell->gap_value = dci_bits[current_bit++];
            else
                current_bit++;

            /* redundancy */
            current_cell->redundancy = 0;
            for (i = 0; i < 2; i++)
                current_cell->redundancy += dci_bits[current_bit++] << (2 - i - 1);

            /* tpc command */
            current_cell->tpc_command = 0;
            for (i = 0; i < 2; i++)
                current_cell->tpc_command += dci_bits[current_bit++] << (2 - i - 1);

            if ((current_cell->tpc_command & 1) == 1)
                num_allocated_RBs_forTBS = 3;
            else
                num_allocated_RBs_forTBS = 2;
            TBS_index = current_cell->modulation_coding;
            current_cell->transport_block_size = TBS_TABLE[TBS_index][num_allocated_RBs_forTBS - 2];
            current_cell->num_resource_blocs = (U8)N_PRB;

            /* padding not used*/

            current_cell->dci_decoded_success = 1;

        }
    }
    else {
        /*
         *   Format 1C: (see TR 36.212, section 5.3.3.1.4) -> THIS IS A SHORT ASSIGNMENT USING QPSK
         *   Gap value (only present if Nrb>50): 1bit
         *   Resource block assignment : log2( ( Nrb / Nrdstep ) * ( ( Nrb / Nrbstep +1 ) / 2 ) ) bits
         *   Transport block size index: 5 bits
         */

        UNPACKBB(current_cell->dci_data, dci_bits, current_cell->SI_DCI_size[0], 0);
        DUMP_ARR_ID_UART(dci_bits, current_cell->SI_DCI_size[0], "d", 0, 1);

        current_cell->localized_distributed = 1;

        /* gap value */
        current_cell->gap_value = 0;
        if (current_cell->dl_tx_bw >= 50)
            current_cell->gap_value = dci_bits[current_bit++];

        /* resource block allocation */
        current_cell->rb_allocation = 0;
        resource_allocation_bits = DCI_1C_RB_ALLOCATION_BITS[current_cell->bw_index];
        for (i = 0; i < resource_allocation_bits; i++)
            current_cell->rb_allocation += dci_bits[current_bit++] << (resource_allocation_bits - i - 1);

        /* modulation and coding */
        current_cell->modulation_coding = 0;
        for (i = 0; i < 5; i++)
            current_cell->modulation_coding += dci_bits[current_bit++] << (5 - i - 1);


        /* redundancy */
        rv_k = 0;
        if(current_cell->BCCH_type == 0)
        {
            rv_k = (current_cell->sfn >> 1) & 3;

        }
        else if(current_cell->BCCH_type == 1)
        {
            rv_k = current_cell->SI_subfrm_counter & 3;
        }

        current_cell->redundancy = ((3*rv_k+1)>>1) & 3;

        /* transport block size*/
        current_cell->transport_block_size =TBS_TABLE_1C[current_cell->modulation_coding];

        /* num RB */
        N_step_RB = 2;
        if(current_cell->dl_tx_bw >= 50)
            N_step_RB=4;

        /*RIV decoding*/
        N_a_DL_VRB = N_a_DL_VRB_LUT[current_cell->gap_value][current_cell->bw_index];
        inv_N_a_DL_VRB = inv_N_a_DL_VRB_LUT[current_cell->gap_value][current_cell->bw_index];
        tmpU32 = inv_N_a_DL_VRB * current_cell->rb_allocation;
        q_num = (U16)(tmpU32 >> 15);
        r_num = current_cell->rb_allocation - q_num*N_a_DL_VRB;

        if((q_num+r_num)<N_a_DL_VRB)
        {
            N_PRB = (q_num+1)*N_step_RB;
            RB_start = r_num*N_step_RB;
        }
        else
        {
            N_PRB = (N_a_DL_VRB+1-q_num)*N_step_RB;
            RB_start = (N_a_DL_VRB-1-r_num)*N_step_RB;
        }

        current_cell->num_resource_blocs = (U8)N_PRB;

        current_cell->dci_decoded_success = 1;
    }

    /*DVRB mapping*/
    if(current_cell->localized_distributed == 1)
    {
        DVRB2PRB(current_cell->gap_value,RB_start,N_PRB,current_cell->bw_index,
            current_cell->PDSCH_PRB_set);
    }

    MemFree(dci_bits, DDR_MEM);
}

//-------------------------------------------------------------------------------------------
/** @brief PDSCH Decode
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param current_cell		  Pointer to the cell parameter structure
 *  @param expoNfft			  Exponent size of FFT 
 *  @param num_pdsch_symbols  Number of PDSCH symbols 
 *  @param bw			      Bandwidth 
 *  @param slot_number        Slot number
 *  @param sib				  Pointer to the decoded sib memory
 *  @param sib_packed		  Pointer to the packed sib memory
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |current_cell |         |pInput         |1        |pStructure     |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |expoNfft     |         |input          |1        |signed short   |(1:15:0)   |full range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |num_pdsch_symbols |    |input          |1        |signed short   |(1:15:0)   |full range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |bw           |         |input          |1        |signed short   |(1:15:0)   |full range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |slot_number  |         |input          |1        |signed short   |(1:15:0)   |full range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |sib          |         |pOutput        |         |pReal Array    |(0:8:0)    |full range  |Byte  |
 *  ---------------------------------------------------------------------------------------------------
 *  |sib_packed   |         |pOutput        |         |pReal Array    |(0:8:0)    |full range  |Byte  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_decode_pdsch      |
 *  --------------------------------------------------------
 *  |Input Streams       |current_cell                     |
 *  --------------------------------------------------------
 *  |Output Streams      |sib,sib_packed				   |
 *  --------------------------------------------------------
 *  |Config Parameters   |expoNfft,num_pdsch_symbols,bw,   |
 *	|					 |slot_number   				   |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to decode PDSCH
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.212
 *
 *  \ingroup group_lte_nmm_SIB 
 *
 **/
void
// lte_bs_phy_nmm_decode_pdsch(CELL_PARAMS * current_cell, S16 expoNfft, S16 symbol, S16 bw, SYMBOL_INFO * sym, S16 slot_number, U8 * sib)
lte_bs_phy_nmm_decode_pdsch(CELL_PARAMS * current_cell, S16 expoNfft, S16 num_pdsch_symbols,
                            S16 bw, SYMBOL_INFO * sym, S16 slot_number, U8 * sib,U8 *current_decoded_sib_packed)
{
    S16             i,
                    j;
    S16             bit_counter = 0;
    U32             L_tmp1,
                    L_tmp2,
                    L_tmp3;
    U8             *cvec;
    S16             temp;
    U32             q;
    U32             cinit = 0;

    //S16             pilot_grp = MODULO6(current_cell->cell_pci);

    S16             num_symbs_to_decode;
    U16             isym;
    S16             num_RBs_to_decode;
    S8              *bDetSoft;
    U32             SI_RNTI = 65535;
    RMParameters   *prm;
    U16             N_pdsch_re,max_N_pdsch_re,pdsch_re_offset;
    volatile U32    t = 0;

#ifdef FEC_FDESC_ENABLED
    PFEC_UL_FDESC   pFecUL;
    PMDMA_FDESC     pMdma;
    volatile PFEC_CONFIG_REGS pFecConfigRegs;
    ULHWCTX         hw_ctx = { 0 };
    S8             *DescAdr[1];
    UINT32          DescLen[1];
    U8             *gencrc,
                   *crcout;
#else
    U8              gencrc[CRC24A_NBYTES];
#endif
    S16             Kr;
    U16             NL;
    S16             Qm;
    U8             *deccrc;
    U8              crc_ok = FALSE;

    num_RBs_to_decode = current_cell->num_resource_blocs;

    /* Once we reached le last symbol of the frame, we have the whole PDSCH data */
    // if (((current_cell->cell_cp == SHORT_CP) && (SYMBOL(symbol) == 13)) || ((current_cell->cell_cp == LONG_CP) && (SYMBOL(symbol) == 11))) {
    if (1) {

        t = GET_TICKS;


        if (current_cell->cell_cp == SHORT_CP)
            num_symbs_to_decode = SYM_PER_SUBFR_SHORT_CP - (current_cell->cfi_val+1);
        else
            num_symbs_to_decode = SYM_PER_SUBFR_LONG_CP - (current_cell->cfi_val+1);

        /* QPSK demodulation */
        bDetSoft = MemAlloc(num_RBs_to_decode*PRB_12*num_symbs_to_decode*2*sizeof(S8),CRAM_MEM,0);

        pdsch_re_offset = 0;
        max_N_pdsch_re = num_RBs_to_decode*PRB_12;
        for (i = 0; i < num_symbs_to_decode; i++) {
            isym = i + current_cell->cfi_val+1;

            N_pdsch_re = current_cell->N_pdsch_re_symb[isym-1];
            for (j = 0; j < N_pdsch_re; j++) {
                bDetSoft[bit_counter++] = (S8)shr(current_cell->pdsch_buf[pdsch_re_offset*2+2 * j],8);
                bDetSoft[bit_counter++] = (S8)shr(current_cell->pdsch_buf[pdsch_re_offset*2+2 * j + 1],8);
            }
            pdsch_re_offset += max_N_pdsch_re;
        }

        /* descrambling */
        cvec = (U8 *) MemAlloc(bit_counter * sizeof(U8), CRAM_MEM, 1);

        /* one one code word so q=0 */
        q = 0;
        /* Init scrambler state */
        L_tmp1 = SI_RNTI << 14;
        L_tmp2 = q << 13;
        temp = (U32) divide_floor(slot_number, 2);
        L_tmp3 = temp << 9;

        cinit = L_tmp1 + L_tmp2 + L_tmp3 + current_cell->cell_pci;

        lte_bs_phy_nmm_pseudo_random_seq_gen_tcb(NULL, bit_counter, cinit, cvec,
            nmm_tcb_arr[3][2+NUM_SYMB_PBCH]);
        lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[3], (2+NUM_SYMB_PBCH), BYPASS_TCB);
        lte_bs_phy_nmm_byPassMultTcb(nmm_tcb_arr[3]+3+NUM_SYMB_PBCH, (N_PBCH_TCB-(3+NUM_SYMB_PBCH)), BYPASS_TCB);
        nmm_runTaskList(3);

        for (i = 0; i < bit_counter; i++) {
            /* scrambled sequence = input sequence xor'd with pseudorandom sequence */
            if (cvec[i] == 0) {
                if(bDetSoft[i]==-128)
                    bDetSoft[i] = 0x7f;
                else
                    bDetSoft[i] = -bDetSoft[i];
            }
        }

        MemFree(cvec, CRAM_MEM);

        prm = (RMParameters *) MemAlloc(sizeof(RMParameters), DDR_MEM, 0);

        Qm = 2;
        Kr = current_cell->transport_block_size + CRC24A_NBITS;
        NL = 1;
        if(current_cell->cell_antennas > 1)
        {
            NL=2;
        }
        Compute_Rate_Matching_Parameters(Kr, 1, NL, bit_counter, Qm, 0, 0, prm);



#ifndef FEC_FDESC_ENABLED
        fec_decode_hdwr_sim(1,  // Type of decoding, 1=CTC)
                            Kr, // Code block size
                            0,  // If only one code block (which is the case) then there is no attached CRC24B (crctype = 0).
                            0,  // Number of initial filler bits in code block
                            0,  // Coding rate
                            10, // Number of decoder 1/2 iterations
                            0,  // CTC decoder automatic stop select
                            0,  // Decoder bypass
                            0,  // HARQ soft-combining enable
                            prm->E,     // Rate matching E parameter
                            prm->k0[current_cell->redundancy], // Rate matching k0 parameter
                            prm->Ncb,   // Rate matching Ncb parameter
                            Qm, // Number of modulation bits used (2,4,6)
                            1,  // Bypass channel de-interleaver
                            0,  // Number of channel interleaver columns
                            0,  // DescAdr, // Channel de-interleaver descriptor starting addresses
                            0,  // (S32*)DescLen, // Channel de-interleaver descriptor byte counts
                            0,  // Channel de-interleaver number of descriptors
                            bDetSoft,   // Input buffer pointer
                            current_decoded_sib_packed, // Output buffer pointer
                            0); // Code block index for debug only

        ComputeCRC24(current_decoded_sib_packed, current_cell->transport_block_size >> 3, P_GCRC24A, gencrc);
        DUMP_ARR_ID(gencrc, CRC24A_NBYTES, "x", 1, 0);
        deccrc = &current_decoded_sib_packed[current_cell->transport_block_size >> 3];
        DUMP_ARR_ID_UART(deccrc, CRC24A_NBYTES, "x", 1, 0);

        crc_ok = ((deccrc[0] == gencrc[0]) && (deccrc[1] == gencrc[1]) && (deccrc[2] == gencrc[2]));
#else
        t = GET_TICKS;

        /* allocate temporary space for descriptors via driver */
        pFecUL = FecUlAllocDesc(1);

        /* configure FEC control registers */
        pFecConfigRegs = (PFEC_CONFIG_REGS) FEC_CONFIG_REGS_BASE;
        pFecConfigRegs->bp_channel_interleaver = 1;

        /* set hardware context */
        memset(&hw_ctx, 0, sizeof(hw_ctx));

        hw_ctx.CTC_CCn = 1;
        hw_ctx.CRC_Sel = 0;
        hw_ctx.Modulation = Qm >> 1;
        hw_ctx.HalfInteration = 10;
        hw_ctx.Bypass_Rate_Match = 0;
        hw_ctx.Rate_Index = 0;
        hw_ctx.Enable_Combine = 0;
        hw_ctx.Bypass_Decode_Enable = 0;
        hw_ctx.Dynamic_Stop_Iteration = 0;

        // Configure FEC CTC decoder via driver.
        hw_ctx.BlockSize = Kr;
        hw_ctx.Filler = 0;
        hw_ctx.E_Param = prm->E;
        hw_ctx.LTERateMaching_K0 = prm->k0[current_cell->redundancy] >> 1;

        /* create FEC CTC decoder control descriptor via driver */
        DescAdr[0] = bDetSoft;
        DescLen[0] = prm->E;

        FecUlSetHwCtx(pFecUL, hw_ctx);
        FecUlSetData(pFecUL, (LPVOID *) DescAdr, DescLen, current_decoded_sib_packed, Kr);      /* FIXME (OS) outlen to be reviewed */

        /* Inform FEC driver to enable CC decoder finished interrupt. FEC driver will call function FecRxCCDone when decoder is finished. */
        FecUlSetLastDesc(pFecUL, (void (*)(LPVOID)) fec_nmm_done, (LPVOID) NULL);

        /* Initialize decoder finished status flag. Function FecRxCCDone will set status flag to 1 when decoder has finished. */
        fec_nmm_cc_stat = 0;

        /* run FEC CC decoder via driver and wait until finished */
        FecUlRun(pFecUL);

        while (fec_nmm_cc_stat == 0);

        crcout = MemAlloc(Kr >> 3, CRAM_MEM, 1);

        pMdma = MdmaGetSingleDesc(MDMA_HW_SYS);
        _ASSERT_PTR(pMdma);
        MdmaSetCRC(pMdma, 0, 0x864CFB, 1);
        MdmaSetData(pMdma, current_decoded_sib_packed, current_cell->transport_block_size >> 3, crcout, Kr >> 3);
        MdmaRunEx(MDMA_HW_SYS, pMdma, NULL, NULL);

        gencrc = &crcout[current_cell->transport_block_size >> 3];
        DUMP_ARR_ID_UART(gencrc, CRC24A_NBYTES, "x", 0, 0);
        MemFree(crcout, CRAM_MEM);

        deccrc = &current_decoded_sib_packed[current_cell->transport_block_size >> 3];
        DUMP_ARR_ID_UART(deccrc, CRC24A_NBYTES, "x", 0, 0);

        crc_ok = ((deccrc[2] == gencrc[0]) && (deccrc[1] == gencrc[1]) && (deccrc[0] == gencrc[2]));
#endif


        UNPACKBB(current_decoded_sib_packed, sib, Kr, 0);

        DUMP_ARR_ID_UART(sib, Kr, "d", 0, 1);
        DUMP_ARR_ID_UART(current_decoded_sib_packed, Kr >> 3, "x", 0, 0);

        /* check crc */
        if (crc_ok)
        {
            //lte_bs_phy_nmm_sib1_decode(current_cell, sib);
            current_cell->sib_decoded_success_cnt++;
        }
        else
        {
            current_cell->sib_decoded_failed_cnt ++;
        }

        /* we need to redecode before finding another SIB */
        current_cell->pdcch_decoded = 0;

        MemFree(prm,DDR_MEM);
    }

    MemFree(bDetSoft,CRAM_MEM);
}


void
x7_work_arounds(S32 * K, S32 * nin, S8 ** in, S8 ** psft, U8 ** pout)
{
    S32             Knew,
                    ac,
                    i,
                    j,
                    m,
                    n;

    Knew = CC_Knew_Table[*K - P_MIN_K];
    m = 3 * *K;
    n = *nin - m;

    if (n > 0) {
        j = 0;

        for (i = 0; i < n; i++) {
            ac = (*in)[j] + (*in)[m + i];

            if (ac > 127)
                ac = 127;
            if (ac < -128)
                ac = -128;

            (*in)[j] = (S8) ac;
            j = j + 1;
            if (j == m)
                j = 0;

        }

        *nin = m;
    }
    else if ((n < 0) && (*K != Knew)) {
        for (i = 0; i < *nin; i++)
            tmpsftdeca[i] = (*in)[i];

        for (i = *nin; i < m; i++)
            tmpsftdeca[i] = 0;

        *psft = tmpsftdeca;
    }

    if (Knew != *K) {
        n = CC_RM_Remapping_Start_Table[*K - P_MIN_K];
        for (i = 0; i < Knew; i++) {
            j = CC_RM_Remapping_Table[n + i];
            tmpsftdecb[i] = (*psft)[j];
            tmpsftdecb[i + Knew] = (*psft)[j + *K];
            tmpsftdecb[i + Knew + Knew] = (*psft)[j + *K + *K];
        }

        *K = Knew;
        *nin = 3 * Knew;
        *psft = tmpsftdecb;
    }
}

/*
 * PDCCH rx FEC decoding
 * FIXME (OS) - could be merged with pbch_chan_decoding
 */
void
lte_bs_phy_nmm_pdcch_chan_decoding(S8 in[], U8 out[], S32 nin, S32 nout, S16 nant, S16 scf, U8 * stat)
{
    S32             nencin;
    U8              tmp_out[1024] = { 0 };      /* FIXME (OS) - tmp */
    U8              gencrc[2],
                    deccrc[2];
    S8             *scfin;
#ifdef FEC_FDESC_ENABLED
    PFEC_UL_FDESC   pFecUL;
    volatile PFEC_CONFIG_REGS pFecConfigRegs;
    ULHWCTX         hw_ctx = { 0 };
    S8             *DescAdr[1];
    UINT32          DescLen[1];
    volatile U32    t = 0;
#endif
    S8             *psft = in;

    nmm_dbg_printf_off(("[%s]\r\n", __FUNCTION__));

    /* Compute encoder input length in bits (+16 is for additional CRC) */
    nencin = nout + CRC16_NBITS;

    /* Allocate temporary space for scaled input array */
    scfin = (S8 *) MemAlloc(nin * sizeof(S8), DDR_MEM, 1);

    /* Scale soft-decision input array */
    FSCALEB(in, scfin, nin, scf);
    psft = scfin;

    /* odd nout leads to FEC HW lockup => X7 workarounds */
    x7_work_arounds(&nencin, &nin, &scfin, &psft, &out);
    DUMP_ARR_ID_UART(psft, nin, "d", 0, 0);

#ifndef FEC_FDESC_ENABLED
    /* Run simulation for FEC hardware decoder */
    fec_decode_hdwr_sim(0,      // Type of encoding or decoding (0=CC, 1=CTC)
                        (U16) nencin,   // Code block size
                        0,      // Type of CRC
                        0,      // Number of initial filler bits in code block
                        0,      // Coding rate
                        0,      // Number of decoder 1/2 iterations
                        0,      // CTC decoder automatic stop select
                        0,      // Decoder bypass
                        0,      // HARQ soft-combining enable
                        (U32) nin,      // Rate matching E parameter
                        0,      // Rate matching k0 parameter
                        (U16) 0,        // Rate matching Ncb parameter
                        2,      // Number of modulation bits used (2,4,6)
                        1,      // Bypass channel de-interleaver
                        0,      // Number of channel interleaver columns
                        NULL,   // Channel de-interleaver descriptor starting addresses
                        (S32 *) 0,      // Channel de-interleaver descriptor byte counts
                        0,      // Channel de-interleaver number of descriptors
                        (S8 *) psft,    // Input buffer pointer
                        (U8 *) out,     // Output buffer pointer
                        0);     // Code block index for debug only
#else

    /* If ARM touched input buffer, then flush it out of ARM cache */
    MxCacheFlush(psft, nin);

    t = GET_TICKS;

    /* configure FEC control registers */
    pFecConfigRegs = (PFEC_CONFIG_REGS) FEC_CONFIG_REGS_BASE;
    pFecConfigRegs->bp_channel_interleaver = 1;

    pFecUL = FecUlAllocDesc(1);

    memset(&hw_ctx, 0, sizeof(hw_ctx));
    hw_ctx.CTC_CCn = 0;
    hw_ctx.CRC_Sel = 0;
    hw_ctx.Modulation = 1;
    hw_ctx.HalfInteration = 0;
    hw_ctx.Bypass_Rate_Match = 0;
    hw_ctx.Rate_Index = 0;
    hw_ctx.Enable_Combine = 0;
    hw_ctx.Bypass_Decode_Enable = 0;
    hw_ctx.Dynamic_Stop_Iteration = 0;
    hw_ctx.BlockSize = nencin;  // Kr;
    hw_ctx.Filler = 0;          // F;
    hw_ctx.E_Param = nin;
    hw_ctx.LTERateMaching_K0 = 0;

    /* create FEC CTC decoder control descriptor via driver */
    DescAdr[0] = psft;
    DescLen[0] = nin;

    FecUlSetHwCtx(pFecUL, hw_ctx);
    // FecUlSetData(pFecUL, (LPVOID *) DescAdr, (LPVOID) DescLen, (U8 *) out, ((nencin + 7) >> 3));
    FecUlSetData(pFecUL, (LPVOID *) DescAdr, (LPVOID) DescLen, (U8 *) out, nencin);
    FecUlSetLastDesc(pFecUL, (void (*)(LPVOID)) fec_nmm_done, (LPVOID) NULL);

    /* Initialize decoder finished status flag. Function FecRxCCDone will set status flag to 1 when decoder has finished. */
    fec_nmm_cc_stat = 0;

    /* run FEC CC decoder via driver and wait until finished */
    FecUlRun(pFecUL);

    while (fec_nmm_cc_stat == 0);
    MLogTask(PID_NMM_SIB1_PDCCH_DECODE, RESOURCE_FECUL, t, GET_TICKS);

    /* revert to default value */
    pFecConfigRegs->bp_channel_interleaver = 0;

#endif
    DUMP_ARR_ID_UART(out, nout, "d", 0, 0);

    /* tmp */
    UNPACKBB(out, tmp_out, nencin, 0);
    PACKBB(tmp_out, out, nout, 0);
    PACKBB(&tmp_out[nout], deccrc, 16, 0);

    /* Use received data to re-compute 16-bit CRC */
    calc_gcrc16(out, gencrc, nout);
    DUMP_ARR_ID_UART(gencrc, 2, "d", 0, 1);

    /* scramble CRC bits with SI-RNTI (SI-RNTI = FFFF) */
    gencrc[0] = gencrc[0] ^ 0xFF;
    gencrc[1] = gencrc[1] ^ 0xFF;

    DUMP_ARR_ID_UART(out, nencin >> 3, "02x", 0, 1);
    DUMP_ARR_ID_UART(deccrc, 2, "d", 0, 1);
    DUMP_ARR_ID_UART(gencrc, 2, "d", 0, 1);

    /* Initialize output status = 0 */
    *stat = 0;

    /* Set bit0 of status if CRC is correct */
    if ((deccrc[0] == gencrc[0]) && (deccrc[1] == gencrc[1]))
        *stat = 1;

    /* De-allocate temporary space */
    MemFree(scfin, DDR_MEM);

    return;
}


void
lte_bs_phy_nmm_pdcch_chan_decoding_cref(S8 in[], U8 out[], S32 nin, S32 nout, S16 nant, S16 scf, U8 * stat)
{

    S32             i,
                    j,
                    k,
                    m;
    S32             nencin,
                    nencout;
    S8             *decin;
    U8             *decout;
    U16            *decinmap;
    U8              gencrc[2],
                    deccrc[2];
    S8             *scfin;

    // Compute encoder input length in bits (+16 is for additional CRC)
    nencin = nout + 16;

    // Compute rate encoder output length in bits
    nencout = 3 * nencin;

    // Allocate temporary space for decoder input array
    decin = (S8 *) malloc(nencout * sizeof(S8));

    // Allocate temporary space for decoder output array
    decout = (U8 *) malloc(nencin * sizeof(U8));

    // Allocate temporary space for rate-matching mapping output array
    decinmap = (U16 *) malloc(nin * sizeof(U16));

    // Allocate temporary space for scaled input array
    scfin = (S8 *) malloc(nin * sizeof(S8));

    // Scale soft-decision input array
    FSCALEB(in, scfin, nin, scf);
    DUMP_ARR_ID_UART(scfin, nin, "d", 0, 0);

    // Run rate matching to determine how soft-decisions map into
    // decoder input array
    CC_Rx_Rate_Matching(nencin, nin, decinmap, NULL);

    // Clear decoder input soft-decision array
    for (i = 0; i < nencout; i++) {
        decin[i] = 0;
    }

    // For each input soft-decision
    for (i = 0; i < nin; i++) {
        // Determine which encoder Output Streams(d0, d1 or d2)
        j = (decinmap[i] >> 14) & 3;

        // Determine relative location within stream
        k = decinmap[i] & 0x3fff;

        // Compute corresponding decoder input array index
        m = k + k + k + j;

        // Add scaled soft-decision to decoder input array element and limit to +-127
        j = decin[m] + scfin[i];
        if (j > 127)
            j = 127;
        if (j < -127)
            j = -127;
        decin[m] = (S8) (j);

    }

    // If using Turbo Best CC decoder function (included in Matlab DLL)
#ifdef USETURBOBESTCCDECODER

    // Run Turbo Best CC decoder
    TurboBest_CC_Decoder_1_3(decin, decout, nencin, 8, 96);

#else

    // Run MindSpeed 1/3 rate CC soft-decision decoder
    cc_decoder_1_3(decin, decout, (S16) nencin, NULL);

#endif

    DUMP_ARR_ID_UART(decout, nencin, "d", 0, 1);

    // Pack decoder output bits (without CRC) into output bytes
    PACKBB(decout, out, nout, 0);

    DUMP_ARR_ID_UART(out, nencin, "d", 0, 1);

    // Pack decoder ouput bits for 16-bit CRC into CRC bytes
    PACKBB(&decout[nout], deccrc, 16, 0);

    // Use received data to re-compute 16-bit CRC
    calc_gcrc16(out, gencrc, nout);

    /* DUMP_ARR_ID_UART(decout, 21, "02x", 1, 1); DUMP_ARR_ID_UART(deccrc, 2, "02x", 1, 1); DUMP_ARR_ID_UART(gencrc, 2, "02x", 1, 1); */


    DUMP_ARR_ID_UART(gencrc, 2, "d", 0, 1);


    // scramble CRC bits with SI-RNTI (SI-RNTI = FFFF)
    gencrc[0] = gencrc[0] ^ 0xFF;
    gencrc[1] = gencrc[1] ^ 0xFF;

    // Scramble CRC bits according to antenna configuration
    // If 2 antenna ports, then scramble using 0xFFFF
    /* if (nant == 2) { gencrc[0] = gencrc[0] ^ 0xFF; gencrc[1] = gencrc[1] ^ 0xFF; } // Else if 4 antenna ports, then scramble using 0x5555 else if (nant == 4) { gencrc[0] =
     * gencrc[0] ^ 0x55; gencrc[1] = gencrc[1] ^ 0x55; } */
    // Initialize output status = 0
    *stat = 0;

    DUMP_ARR_ID_UART(decout, 27, "d", 0, 1);
    DUMP_ARR_ID_UART(deccrc, 2, "d", 0, 1);
    DUMP_ARR_ID_UART(gencrc, 2, "d", 0, 1);

    // Set status=1 if both generated and decoded CRC are similar
    if ((deccrc[0] == gencrc[0]) && (deccrc[1] == gencrc[1]))
        *stat = 1;



    // De-allocate temporary space
    free(decin);
    free(decout);
    free(decinmap);
    free(scfin);

    return;

}


/* warning: for len <= 32b only */
U32
extract_part(U8 * message, U16 offset, U16 len)
{
    S16             i = len - 1;
    U8             *p8 = message + offset;
    U32             val = 0;

    if (!len)
        return (val);

    do {
        val |= (*p8++ << i);
    } while (--i >= 0);

    return (val);
}


U32
pack_nbits(U8 * ptr, U16 len)
{
    S16             i = len - 1;
    U32             val = 0;

    if (!len)
        return (val);

    do {
        val |= (*ptr++ << i);
    } while (--i >= 0);

    return (val);
}


#if 0
static void
lte_bs_phy_nmm_sib1_decode(CELL_PARAMS * current_cell, U8 * sib)
{
    bcch_dl_sch_message_preamble_unpacked_t *bcch_dl_sch_message_p;
    cell_access_related_info_part2_unpacked_t *cell_access_related_info_part2_p;
    cell_selection_info_unpacked_t *cell_selection_info_p;
    U8             *curr_message_ptr;
    U8              bcch_dl_sch_choice_0_1;
    U8              sib1_preamble;
    U8              cell_access_related_info_preamble;
    U8              plmn_identity_list_len;
    U8              scheduling_info_list_len;
    U8              si_mapping_info_len;
    si_periodicity_t curr_si_periodicity;
    U8              sib_type_extension;
    U8              sib_type;
    U8              si_periodicity_index;
#if SIB1_PART2
    sib1_message_part2_unpacked_t *sib1_message_part2_p;
    U8              si_window_length;
    U8              system_info_value_tag;
#endif
    U32             i,
                    j;
#if SIB1_DECODE_DBG == 1
    tdd_config_t   *tdd_config_p;
#endif

    bcch_dl_sch_message_p = (bcch_dl_sch_message_preamble_unpacked_t *) sib;
    bcch_dl_sch_choice_0_1 = PACK_2BITS(bcch_dl_sch_message_p->choice);

    /* exit if not SIB1 message */
    if (bcch_dl_sch_choice_0_1 != BCCH_DL_SCH_CHOICE_0_1_SIB1)
        return;

#if SIB1_DECODE_DBG == 1
    nmm_dbg_printf_off(("\n--SIB1 decode--\r\n"));
    nmm_dbg_printf_off(("  bcch_choice = %d\r\n", bcch_dl_sch_choice_0_1));
    nmm_dbg_printf_off(("  plmn_identity_list = "));
#endif

    sib1_preamble = PACK_3BITS(bcch_dl_sch_message_p->sib1_preamble);
    cell_access_related_info_preamble = bcch_dl_sch_message_p->cell_acces_related_info_preamble[0];
    plmn_identity_list_len = PLMN_IDENTITY_LIST_LEN_MIN + PACK_3BITS(bcch_dl_sch_message_p->plmn_identity_list_len);

    curr_message_ptr = &bcch_dl_sch_message_p->next;

    /* plmn-identitylist, length depends on MCC (optional) and MNC (2..3) */
    for (i = 0; i < plmn_identity_list_len; i++) {
        U8              mcc_len = *curr_message_ptr++;  /* 1b preamble */
        U8              mnc_len;
        if (mcc_len) {
#if SIB1_DECODE_DBG == 1
            nmm_dbg_printf_off(("%d,", pack_nbits(curr_message_ptr, (PLMN_IDENTITY_MCC_LEN * PLMN_IDENTITY_MCC_NBITS))));
#endif
            curr_message_ptr += (PLMN_IDENTITY_MCC_LEN * PLMN_IDENTITY_MCC_NBITS);
        }
        mnc_len = PLMN_IDENTITY_MNC_MIN + *curr_message_ptr++;  /* 1b length: 2..3 */

#if SIB1_DECODE_DBG == 1
        nmm_dbg_printf_off(("%d,%d ", pack_nbits(curr_message_ptr, (mnc_len * PLMN_IDENTITY_MNC_NBITS)), curr_message_ptr[mnc_len * PLMN_IDENTITY_MNC_NBITS]));
#endif
        curr_message_ptr += (mnc_len * PLMN_IDENTITY_MNC_NBITS);
        curr_message_ptr++;     /* cell_reserved_for_operator_use (1b) */
    }

    /* remaining cell_access_related_info fields */
    cell_access_related_info_part2_p = (cell_access_related_info_part2_unpacked_t *) curr_message_ptr;
    curr_message_ptr = (cell_access_related_info_preamble) ? &cell_access_related_info_part2_p->next : (U8 *) & cell_access_related_info_part2_p->csg_identity;

#if SIB1_DECODE_DBG == 1
    nmm_dbg_printf_off(("\n  tracking_area_code = 0x%x\r\n", pack_nbits((U8 *) & cell_access_related_info_part2_p->tracking_area_code, TRACKING_AREA_CODE_NBITS)));
    nmm_dbg_printf_off(("  cell_identity = 0x%x\r\n", pack_nbits((U8 *) & cell_access_related_info_part2_p->cell_identity, CELL_IDENTITY_NBITS)));
    nmm_dbg_printf_off(("  cell_barred = %d\r\n", cell_access_related_info_part2_p->cell_barred[0]));
    nmm_dbg_printf_off(("  intra_freq_reselection = %d\r\n", cell_access_related_info_part2_p->intra_freq_reselection[0]));
    nmm_dbg_printf_off(("  csg_indication = %d\r\n", cell_access_related_info_part2_p->csg_indication[0]));
    if (cell_access_related_info_preamble)
        nmm_dbg_printf_off(("  csg_identity = 0x%x\r\n", pack_nbits((U8 *) & cell_access_related_info_part2_p->csg_identity, CSG_IDENTITY_NBITS)));
#endif

    /* cell_selection_info, length depends on preamble (q_rx_lev_min_offset optional */
    cell_selection_info_p = (cell_selection_info_unpacked_t *) curr_message_ptr;

#if SIB1_DECODE_DBG == 1
    nmm_dbg_printf_off(("  cell_selection_info\r\n"));
    nmm_dbg_printf_off(("    q_rx_lev_min = %d\r\n", Q_RX_LEV_MIN_MIN + pack_nbits(cell_selection_info_p->q_rx_lev_min, 6)));
    if (cell_selection_info_p->preamble[0])
        nmm_dbg_printf_off(("    q_rx_lev_min_offset = %d\r\n", Q_RX_LEV_MIN_OFFSET_MIN + PACK_3BITS(cell_selection_info_p->q_rx_lev_min_offset)));
#endif

    curr_message_ptr = (cell_selection_info_p->preamble[0]) ? &cell_selection_info_p->next : (U8 *) & cell_selection_info_p->q_rx_lev_min_offset;

#if SIB1_DECODE_DBG == 1
    if (sib1_preamble & P_MAX_MASK) {
        nmm_dbg_printf_off(("  p_max = %d\r\n", P_MAX_MIN + pack_nbits(curr_message_ptr, P_MAX_NBITS)));
        nmm_dbg_printf_off(("  freq_band_indicator = %d\r\n", FREQ_BAND_INDICATOR_MIN + pack_nbits(&curr_message_ptr[P_MAX_NBITS], FREQ_BAND_INDICATOR_NBITS)));
    }
    else
        nmm_dbg_printf_off(("  freq_band_indicator = %d\r\n", FREQ_BAND_INDICATOR_MIN + pack_nbits(curr_message_ptr, FREQ_BAND_INDICATOR_NBITS)));
#endif

    /* p_max (opt) & freq_band_indicator */
    curr_message_ptr += (sib1_preamble & P_MAX_MASK) ? (P_MAX_NBITS + FREQ_BAND_INDICATOR_NBITS) : FREQ_BAND_INDICATOR_NBITS;

    /* scheduling_info_list, 1st periodicity fields apply to SIB2, others are described using mapping_info lists */
    scheduling_info_list_len = SCHEDULING_INFO_LIST_MIN + PACK_5BITS(curr_message_ptr);
    curr_message_ptr += SCHEDULING_INFO_LIST_LEN_NBITS;
    for (i = 0; i < scheduling_info_list_len; i++) {
        curr_si_periodicity = (si_periodicity_t) PACK_3BITS(curr_message_ptr);
        curr_message_ptr += SI_PERIODICITY_NBITS;
        if (i == 0)
            current_cell->si_periodicity[sib_type2] = curr_si_periodicity;      /* always associated with first signalled periodicity */
        si_mapping_info_len = SIB_MAPPING_INFO_MIN + PACK_5BITS(curr_message_ptr);
        curr_message_ptr += SIB_MAPPING_INFO_NBITS;
        for (j = 0; j < si_mapping_info_len; j++) {
            sib_type_extension = *curr_message_ptr++;
            sib_type = PACK_4BITS(curr_message_ptr);
            curr_message_ptr += SIB_TYPE_NBITS;
            si_periodicity_index = SIB_TYPE_ENUM_OFFSET + sib_type + sib_type_extension;
            if (si_periodicity_index < MAX_N_SIB_TYPES)
                current_cell->si_periodicity[si_periodicity_index] = curr_si_periodicity;
        }
    }

#if SIB1_DECODE_DBG == 1
    nmm_dbg_printf_off(("  scheduling_info_list ="));
    for (i = 0; i < MAX_N_SIB_TYPES; i++)
        if (current_cell->si_periodicity[i] != si_unkown)
            nmm_dbg_printf_off((" (%d:%d)", i, current_cell->si_periodicity[i]));
    nmm_dbg_printf_off(("\r\n"));
    if (sib1_preamble & TDD_CONFIG_MASK) {
        tdd_config_p = (tdd_config_t *) curr_message_ptr;
        nmm_dbg_printf_off(("  tdd_config\r\n"));
        nmm_dbg_printf_off(("    subframe_assignment = %d\r\n", PACK_3BITS(tdd_config_p->subframe_assignment)));
        nmm_dbg_printf_off(("    special_subframe_patterns = %d\r\n", PACK_4BITS(tdd_config_p->special_subframe_patterns)));
    }
#endif

    /* tdd_config */
    if (sib1_preamble & TDD_CONFIG_MASK)
        curr_message_ptr += TDD_CONFIG_NBITS;

#if SIB1_PART2
    /* si_windown_length & system_info_value_tag */
    sib1_message_part2_p = (sib1_message_part2_unpacked_t *) curr_message_ptr;
    si_window_length = pack_nbits(sib1_message_part2_p->si_window_length, SI_WINDOW_LENGTH_NBITS);
    system_info_value_tag = pack_nbits(sib1_message_part2_p->system_info_value_tag, SYSTEM_INFO_VALUE_TAG_NBITS);

#if SIB1_DECODE_DBG == 1
    nmm_dbg_printf_off(("  si_window_length = %d\r\n", si_window_length));
    nmm_dbg_printf_off(("  system_info_value_tag = %d\r\n", system_info_value_tag));
    nmm_dbg_printf_off(("\r\n"));
#endif
#endif
    /* end of SIB1 message skipped */

    //current_cell->sib_decoded_success_cnt++;
}
#endif

#if NMM_BET == 1 || !defined USECEVA || defined WIN32

static void
lte_bs_phy_nmm_hbfir_state_upd(S16 real_in, S16 imag_in, S16 * HBFIR_real_Buf, S16 * HBFIR_imag_Buf)
{
    S32             i;

    /* shift the delay line */
    for (i = HBFIR_DECIMATION_LEN - 1; i > 0; i--) {
        HBFIR_real_Buf[i] = HBFIR_real_Buf[i - 1];
        HBFIR_imag_Buf[i] = HBFIR_imag_Buf[i - 1];
    }

    /* Add the new input sample */
    HBFIR_real_Buf[0] = real_in;
    HBFIR_imag_Buf[0] = imag_in;
}


static void
lte_bs_phy_nmm_hbfir(S16 * HBFIR_real_out, S16 * HBFIR_imag_out, S16 * HBFIR_real_Buf, S16 * HBFIR_imag_Buf)
{
    S32             i,
                    k;
    S32             acc_real,
                    acc_imag;

    /* The delay line has been updated */
    acc_real = 0;
    acc_imag = 0;

    /* Left part */
    k = 0;
    for (i = 0; i < (HBFIR_DECIMATION_LEN / 4 + 1); i++) {
        acc_real = L_mac(acc_real, HBFIR_SYNC[i], HBFIR_real_Buf[k]);
        acc_imag = L_mac(acc_imag, HBFIR_SYNC[i], HBFIR_imag_Buf[k]);
        k += 2;
    }

    /* center */
    k--;
    i = (HBFIR_DECIMATION_LEN / 4 + 1);
    acc_real = L_mac(acc_real, HBFIR_SYNC[i], HBFIR_real_Buf[k]);
    acc_imag = L_mac(acc_imag, HBFIR_SYNC[i], HBFIR_imag_Buf[k]);

    /* Right part */
    k++;
    for (i = (HBFIR_DECIMATION_LEN / 4 + 2); i < (HBFIR_DECIMATION_LEN / 2 + 2); i++) {
        acc_real = L_mac(acc_real, HBFIR_SYNC[i], HBFIR_real_Buf[k]);
        acc_imag = L_mac(acc_imag, HBFIR_SYNC[i], HBFIR_imag_Buf[k]);
        k += 2;
    }
    *HBFIR_real_out = extract_h(acc_real);
    *HBFIR_imag_out = extract_h(acc_imag);
}


void
Lte_ms_sync_hbfir_total(S16 * HBFIR_out, S16 * HBFIR_Buf, int nIQin)
{
    S32             i,
                    j,
                    k;
    S32             acc_real,
                    acc_imag;
    S16            *pHBFIR_Buf;

    for (j = 0; j < nIQin; j++) {       // modify version
        pHBFIR_Buf = &HBFIR_Buf[4 * j + 0];
        acc_real = 0;
        acc_imag = 0;

        // Left part
        k = 0;
        for (i = 0; i < (HBFIR_DECIMATION_LEN / 4 + 1); i++) {  // #define LEN_HBFIR1 75;(LEN_HBFIR1 / 4 + 1)=19
            acc_real = L_mac(acc_real, HBFIR_SYNC[i], pHBFIR_Buf[k + 0]);       // HBFIR_real_Buf:input
            acc_imag = L_mac(acc_imag, HBFIR_SYNC[i], pHBFIR_Buf[k + 1]);       // hbfir_ms_sync1:Coef
            k -= 4;
        }

        // center
        k += 2;
        i = (HBFIR_DECIMATION_LEN / 4 + 1);
        acc_real = L_mac(acc_real, HBFIR_SYNC[i], pHBFIR_Buf[k + 0]);
        acc_imag = L_mac(acc_imag, HBFIR_SYNC[i], pHBFIR_Buf[k + 1]);

        // Right part
        k -= 2;
        for (i = (HBFIR_DECIMATION_LEN / 4 + 2); i < (HBFIR_DECIMATION_LEN / 2 + 2); i++) {
            acc_real = L_mac(acc_real, HBFIR_SYNC[i], pHBFIR_Buf[k + 0]);
            acc_imag = L_mac(acc_imag, HBFIR_SYNC[i], pHBFIR_Buf[k + 1]);
            k -= 4;
        }

        if ((0x77f8 == extract_h(acc_real)) || (0x77f8 == extract_h(acc_imag)))
            j += 0;
        HBFIR_out[2 * j + 0] = extract_h(acc_real);
        HBFIR_out[2 * j + 1] = extract_h(acc_imag);
    }
}

void
HBfir_downsampling_32_2(S16 * IQinTL, int nIQin, S16 * IQout)
{
    S16            *pIQinTL0 = &IQinTL[HBFIR_DECIMATION_LEN * 2 * 4];
    S16            *pIQinTL1 = &IQinTL[HBFIR_DECIMATION_LEN * 2 * 3];
    S16            *pIQinTL2 = &IQinTL[HBFIR_DECIMATION_LEN * 2 * 2];
    S16            *pIQinTL3 = &IQinTL[HBFIR_DECIMATION_LEN * 2 * 1];
    S16            *pIQinTL4 = &IQinTL[HBFIR_DECIMATION_LEN * 2 * 0];

    Lte_ms_sync_hbfir_total(&pIQinTL1[HBFIR_DECIMATION_LEN * 2], &pIQinTL0[HBFIR_DECIMATION_LEN * 2], nIQin >> 1);
    Lte_ms_sync_hbfir_total(&pIQinTL2[HBFIR_DECIMATION_LEN * 2], &pIQinTL1[HBFIR_DECIMATION_LEN * 2], nIQin >> 2);
    Lte_ms_sync_hbfir_total(&pIQinTL3[HBFIR_DECIMATION_LEN * 2], &pIQinTL2[HBFIR_DECIMATION_LEN * 2], nIQin >> 3);
    Lte_ms_sync_hbfir_total(IQout, &pIQinTL3[HBFIR_DECIMATION_LEN * 2], nIQin >> 4);
    /* tmp - 10MHz hard-coded */
    /* Lte_ms_sync_hbfir_total(&pIQinTL4[HBFIR_DECIMATION_LEN * 2], &pIQinTL3[HBFIR_DECIMATION_LEN * 2], nIQin>>4); Lte_ms_sync_hbfir_total(IQout, &pIQinTL4[HBFIR_DECIMATION_LEN *
     * 2], nIQin>>5); */
}


void
Lte_ms_sync_backup_hbfir_data(S16 * HBFIR_real_imag_Buf, S16 * IQin, int nIQin)
{
    S32             i;
    for (i = 0; i < HBFIR_DECIMATION_LEN; i++) {
        HBFIR_real_imag_Buf[2 * i + 0] = IQin[2 * i + 0];
        HBFIR_real_imag_Buf[2 * i + 1] = IQin[2 * i + 1];
    }
}

int
lte_bs_phy_nmm_downsampling(S16 * IQin, S32 nIQin, S16 * IQout_cram, S32 factor,
                            S16 * hbfir_real_buf_stage0_cram, S16 * hbfir_imag_buf_stage0_cram,
                            S16 * hbfir_real_buf_stage1_cram, S16 * hbfir_imag_buf_stage1_cram,
                            S16 * hbfir_real_buf_stage2_cram, S16 * hbfir_imag_buf_stage2_cram,
                            S16 * hbfir_real_buf_stage3_cram, S16 * hbfir_imag_buf_stage3_cram, S16 * hbfir_real_buf_stage4_cram, S16 * hbfir_imag_buf_stage4_cram)
{
    // LISTENING_CELL_SEARCH_DESC *this = (LISTENING_CELL_SEARCH_DESC *) cell_search;
    S32             i,
                    j;

    S16             in_size = (S16) nIQin * 2;
    S16             IQinTL[HBFIR_DECIMATION_LEN * 2 * 5 + 2 * SUBFR_SIZE_096MHZ * 8 * 2];
    S16            *pIQinTL0 = &IQinTL[HBFIR_DECIMATION_LEN * 2 * 4];
    S16            *pIQinTL1 = &IQinTL[HBFIR_DECIMATION_LEN * 2 * 3];
    S16            *pIQinTL2 = &IQinTL[HBFIR_DECIMATION_LEN * 2 * 2];
    S16            *pIQinTL3 = &IQinTL[HBFIR_DECIMATION_LEN * 2 * 1];
    S16            *pIQinTL4 = &IQinTL[HBFIR_DECIMATION_LEN * 2 * 0];
    S16            *pHBFIR_real_imag_Buf_stage0 = &hbfir_real_buf_stage0_cram[HBFIR_DECIMATION_LEN * 2 * 4];
    S16            *pHBFIR_real_imag_Buf_stage1 = &hbfir_real_buf_stage0_cram[HBFIR_DECIMATION_LEN * 2 * 3];
    S16            *pHBFIR_real_imag_Buf_stage2 = &hbfir_real_buf_stage0_cram[HBFIR_DECIMATION_LEN * 2 * 2];
    S16            *pHBFIR_real_imag_Buf_stage3 = &hbfir_real_buf_stage0_cram[HBFIR_DECIMATION_LEN * 2 * 1];
    S16            *pHBFIR_real_imag_Buf_stage4 = &hbfir_real_buf_stage0_cram[HBFIR_DECIMATION_LEN * 2 * 0];

    j = 0;

    for (i = 0; i < nIQin * 2; i++)
        pIQinTL0[HBFIR_DECIMATION_LEN * 2 + i] = IQin[i];

    for (i = 0; i < HBFIR_DECIMATION_LEN * 2 * 5; i++)
        IQinTL[i] = hbfir_real_buf_stage0_cram[i];

    HBfir_downsampling_32_2(IQinTL, nIQin, IQout_cram);

    Lte_ms_sync_backup_hbfir_data(pHBFIR_real_imag_Buf_stage4, &pIQinTL4[2 * (nIQin >> 4)], HBFIR_DECIMATION_LEN);
    Lte_ms_sync_backup_hbfir_data(pHBFIR_real_imag_Buf_stage3, &pIQinTL3[2 * (nIQin >> 3)], HBFIR_DECIMATION_LEN);
    Lte_ms_sync_backup_hbfir_data(pHBFIR_real_imag_Buf_stage2, &pIQinTL2[2 * (nIQin >> 2)], HBFIR_DECIMATION_LEN);
    Lte_ms_sync_backup_hbfir_data(pHBFIR_real_imag_Buf_stage1, &pIQinTL1[2 * (nIQin >> 1)], HBFIR_DECIMATION_LEN);
    Lte_ms_sync_backup_hbfir_data(pHBFIR_real_imag_Buf_stage0, &pIQinTL0[2 * (nIQin >> 0)], HBFIR_DECIMATION_LEN);

    return 0;
}


void
lte_bs_phy_nmm_ssync_est_channel_estimate(S16 * SSync_f, S16 * PSync_f, S16 *exp_hsphs, S16 * SSyncEst, S16 PSS_root, S16 lp_cp_flag)
{
    S16             k;
    S32             ac,
                    bd,
                    ad,
                    bc;
    S16             AccR,
                    AccR1,
                    AccI,
                    AccI1;
    S32             num1,
                    num2;
    S16             tmp16;
    S16            *pZC;

    // pZChsphs = ZC25_FD_HSPHS;

    // NID_2 = 0;
    // added
    pZC = ZC25_FD;

    if (PSS_root == 1) {
        // pZChsphs = ZC29_FD_HSPHS;
        // NID_2 = 1;
        // added
        pZC = ZC29_FD;
    }
    if (PSS_root == 2) {
        // pZChsphs = ZC34_FD_HSPHS;
        // NID_2 = 2;
        // added
        pZC = ZC34_FD;
    }
    // ishf = 1;

    for (k = 0; k < FFT_SIZE_096MHZ; k++) {

        if (lp_cp_flag == SHORT_CP) {
            /*
             * compute SSS1(i)*expo(i) : derotation due to half sample shift
             */
            ac = L_mult(SSync_f[2 * k], exp_hsphs[2 * k]);
            ad = L_mult(SSync_f[2 * k], exp_hsphs[2 * k + 1]);
            bc = L_mult(SSync_f[2 * k + 1], exp_hsphs[2 * k]);
            bd = L_mult(SSync_f[2 * k + 1], exp_hsphs[2 * k + 1]);

            AccR = extract_h(ac);
            tmp16 = extract_h(bd);
            AccR = sub(AccR, tmp16);

            AccI = extract_h(ad);
            tmp16 = extract_h(bc);
            AccI = add(AccI, tmp16);
        }
        else {
            /*
             * no derotation this time because no half sample shift
             */
            AccR = SSync_f[2 * k];
            AccI = SSync_f[2 * k + 1];
        }

        /*
         * compute channel estimate from pilots: InvChanEst
         */

        ac = L_mult(pZC[2 * k], PSync_f[2 * k]);
        ad = L_mult(pZC[2 * k], -PSync_f[2 * k + 1]);
        bc = L_mult(pZC[2 * k + 1], PSync_f[2 * k]);
        bd = L_mult(pZC[2 * k + 1], -PSync_f[2 * k + 1]);

        num1 = L_add(ac, bd); // num1 = ac+bd
        num2 = L_sub(bc, ad); // num2 = bc-ad

        AccR1 = (S16) (num1 >> 8);
        AccI1 = (S16) (num2 >> 8);

        /*
         * compute derotated SSS1(i)*InvChanEst
         */
        ac = L_mult(AccR, AccR1);
        ad = L_mult(AccR, AccI1);
        bc = L_mult(AccI, AccR1);
        bd = L_mult(AccI, AccI1);

        AccR = extract_h(ac);
        tmp16 = extract_h(bd);
        SSyncEst[2*k] = sub(AccR, tmp16);

        AccI = extract_h(ad);
        tmp16 = extract_h(bc);
        SSyncEst[2*k+1] = add(AccI, tmp16);
    }

}
#endif

#endif
/*
 * -----------
 * COMMON CODE
 * -----------
 */

 
//-------------------------------------------------------------------------------------------
/** @brief implement PBCH extraction
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param expoNfft        Exponent size of FFT 
 *  @param pin             Pointer to the Input IQ samples memory
 *  @param symb_index      symbol index
 *  @param cell_pci        cell PCI
 *  @param cell_cp         cell CP format
 *  @param channelEst_buf  Pointer to channel estimation memory
 *  @param pbch            Pointer to the pbch memory
 *  @param expo_fac        Exponent number of FFT output
 *  @param Ntx_ante        Antenna number of cell TX
 *  @param inv_noise_var   Inverse number of noise variance
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length      |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |            |               |           |            |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |expoNfft     |         |Input          |1           |signed short   |(1:15:0)   |            |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |pin          |         |pInput         |1<<expoNfft |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |symb_index   |         |Input          |1           |signed short   |(1:15:0)   |[0:13]      |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |cell_pci     |         |Input          |1           |signed short   |(1:15:0)   |[0:503]     |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |cell_cp      |         |Input          |1           |signed short   |(1:15:0)   |[0,1]       |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |channelEst_buf |       |pInput         |1           |pComplex Array |(1:15:0)   |[6:100]     |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |pbch         |         |pOutput		|            |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |expo_fac     |         |pOutput        |1           |pReal	         |(1:15:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |Ntx_ante     |         |Input          |1           |unsigned short |(1:15:0)   |[1,2,4]     |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |inv_noise_var |        |Input          |1           |unsigned int   |(0:32:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_extract_pbch      |
 *  --------------------------------------------------------
 *  |Input Streams       |pin,channelEst_buf		       |
 *  --------------------------------------------------------
 *  |Output Streams      |pbch, expo_fac				   |
 *  --------------------------------------------------------
 *  |Config Parameters   |expoNfft,symb_index,cell_pci,    |
 *  |				     |cell_cp,Ntx_ante,inv_noise_var   |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to extract pbch from time-domain OFDM symbol
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_MIB 
 *
 **/
void
lte_bs_phy_nmm_extract_pbch(S16 expoNfft, S16 * pin, S16 symb_index, S16 cell_pci,
                            S16 cell_cp, S16 * channelEst_buf, S16 * pbch, S16 * expo_fac,
                            U16 Ntx_ante,U32 inv_noise_var)
{
    S32             length = (1 << expoNfft);
    S16             pilot_grp = MODULO6(cell_pci);
    S16             pilot_idx,
                    pilot_idx_dtx;
    S16             nb_subcarriers_of_interest = PBCH_NB_RB * PRB_12;
    S16             i;
    S16            *p16,
                   *q16;
#ifdef CEVA_INTRINSICS
    S16            *fft_out;
    S16            *re_array;
    S16            *timedomain_ceva;
    S16             fft_size = (2 * (1 << expoNfft));
    S16             expo_ceva;
#else
    S16             fft_out[2 * FFTSZ_2048];
    S16             re_array[2 * FFTSZ_2048];
#endif
    U16             pbch_re_indices[PBCH_NB_RB * PRB_12];
    U16             N_pbch_re;

#ifdef CEVA_INTRINSICS
    /* pin points after CP */
    timedomain_ceva = (S16 *) DataBuff_Bank1;
    fft_out = timedomain_ceva + fft_size;
    re_array = fft_out + fft_size;
    MemCpyFromExt((void *) timedomain_ceva, (void *) pin, fft_size * sizeof(S16));

    cfft_16t32d(timedomain_ceva, expoNfft, fft_out, &expo_ceva);
    *expo_fac = expo_ceva - 5;
    /* FFT to RE mapping + DC skipping */
    lte_bs_phy_swap_fftout_dcskip_ceva(fft_out, re_array, nb_subcarriers_of_interest / 2, (S32) (2 * length - nb_subcarriers_of_interest), expo_fac);

#else
    /* FFT */
    fftcg_c_16t24d(pin, (int) expoNfft, fft_out, expo_fac);

    /* FFT to RE mapping + DC skipping */
    lte_bs_phy_swap_fftout_dcskip(fft_out, re_array, nb_subcarriers_of_interest / 2, (S32) (2 * length - nb_subcarriers_of_interest), expo_fac);
    DUMP_ARR_ID_UART(re_array, (nb_subcarriers_of_interest * 2), "d", 0, 1);

#endif


    N_pbch_re = 0;
    /* PBCH RE and channel estimation RE extraction */
	if (((cell_cp == SHORT_CP) && ((symb_index == SYMBOL(7)) || (symb_index == SYMBOL(8)))) ||
        ((cell_cp == LONG_CP)  && ((symb_index == SYMBOL(6)) || (symb_index == SYMBOL(7))  || (symb_index == SYMBOL(9))))) {
        /* RS + DTX location */
        pilot_idx = pilot_grp;
        pilot_idx_dtx = MODULO6(pilot_grp + 3);
        pilot_idx = MIN(pilot_idx, pilot_idx_dtx);

        /* init PBCH index is right after 1st RS or DTX */
        p16 = pbch;
        q16 = re_array;
        for (i = 0; i < pilot_idx; i++) {
            *p16++ = *q16++;
            *p16++ = *q16++;
            pbch_re_indices[N_pbch_re++] = i;
        }
        /* skip RS / DTX */
        q16 += (1 * 2);

        for (i = 0; i < (PBCH_NB_RB * 4 - 1); i++) {
            *p16++ = *q16++;
            *p16++ = *q16++;
            *p16++ = *q16++;
            *p16++ = *q16++;

            pbch_re_indices[N_pbch_re++] = i*3+(pilot_idx+1);
            pbch_re_indices[N_pbch_re++] = i*3+1+(pilot_idx+1);

            /* skip RS / DTX */
            q16 += (1 * 2);
        }

        for (i = pilot_idx; i < 2; i++) {
            *p16++ = *q16++;
            *p16++ = *q16++;

            pbch_re_indices[N_pbch_re++] = (i-pilot_idx)+(PBCH_NB_RB * 4 - 1)*3+(pilot_idx+1);
        }
    }
    else {
        /* No RS DTX in the last 2 syms */
        memcpy(pbch, re_array, 2 * nb_subcarriers_of_interest * sizeof(S16));
        for(i=0;i<PBCH_NB_RB*PRB_12;i++)
        {
            pbch_re_indices[N_pbch_re++] = i;
        }
    }

    /* channel compensation */
    lte_bs_phy_nmm_channelEst_comp(pbch, channelEst_buf, N_pbch_re / 2,
        Ntx_ante,PBCH_BW,pbch_re_indices,inv_noise_var,0);
}

//-------------------------------------------------------------------------------------------
/** @brief implement PCFI extraction
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param expoNfft        Exponent size of FFT 
 *  @param pin             Pointer to the Input IQ samples memory
 *  @param bw		       bandwidth
 *  @param pcfi_re_idx     Pointer to pcfi index
 *  @param channelEst_buf  Pointer to channel estimation memory
 *  @param pcfi            Pointer to the pbch memory
 *  @param expo_fac        Exponent number of FFT output
 *  @param Ntx_ante        Antenna number of cell TX
 *  @param inv_noise_var   Inverse number of noise variance
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length      |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |            |               |           |            |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |expoNfft     |         |Input          |1           |signed short   |(1:15:0)   |            |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |pin          |         |pInput         |1<<expoNfft |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |bw           |         |Input          |1           |signed short   |(1:15:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |pcfi_re_idx  |         |pInput         |            |pReal Array    |(1:15:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |channelEst_buf |       |pInput         |            |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |pbch         |         |pOutput		|            |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |expo_fac     |         |pOutput        |1           |pReal	         |(1:15:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |Ntx_ante     |         |Input          |1           |unsigned short |(1:15:0)   |[1,2,4]     |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |inv_noise_var |        |Input          |1           |unsigned int   |(0:32:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_extract_pcfi      |
 *  --------------------------------------------------------
 *  |Input Streams       |pin,pcfi_re_idx,channelEst_buf   |
 *  --------------------------------------------------------
 *  |Output Streams      |pcfi, expo_fac				   |
 *  --------------------------------------------------------
 *  |Config Parameters   |expoNfft,bw,Ntx_ante,inv_noise_var |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to extract pcfi from time-domain OFDM symbol
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_SIB 
 *
 **/
void
lte_bs_phy_nmm_extract_pcfi(S16 expoNfft, S16 * pin, S16 bw, S16 * pcfi_re_idx,
                            S16 * channelEst_buf, S16 * pcfi, S16 * expo_fac,U16 Ntx_ante,
                            U32 inv_noise_var)
{
    S32             length = (1 << expoNfft);
    S16             nb_subcarriers_of_interest = bw * PRB_12;
    S16             i;
    S16            *p16,
                   *q16;
#ifdef CEVA_INTRINSICS
    S16            *fft_out;
    S16            *re_array;
    S16            *timedomain_ceva;
    S16             fft_size = (2 * (1 << expoNfft));
    S16             expo_ceva;
#else
    S16             fft_out[2 * FFTSZ_2048];
    S16             re_array[2 * FFTSZ_2048];
#endif

#ifdef CEVA_INTRINSICS
    /* pin points after CP */
    timedomain_ceva = (S16 *) DataBuff_Bank1;
    fft_out = timedomain_ceva + fft_size;
    re_array = fft_out + fft_size;
    MemCpyFromExt((void *) timedomain_ceva, (void *) pin, fft_size * sizeof(S16));

    cfft_16t32d(timedomain_ceva, expoNfft, fft_out, &expo_ceva);
    *expo_fac = expo_ceva - 5;

    /* FFT to RE mapping + DC skipping */
    lte_bs_phy_swap_fftout_dcskip_ceva(fft_out, re_array, nb_subcarriers_of_interest / 2, (S32) (2 * length - nb_subcarriers_of_interest), expo_fac);

#else
    nmm_dbg_printf_off(("[%s]\n", __FUNCTION__));

    /* FFT */
    fftcg_c_16t24d(pin, (int) expoNfft, fft_out, expo_fac);
    DUMP_ARR_ID_UART(fft_out, 20, "d", 1, 0);

    /* FFT to RE mapping + DC skipping */
    lte_bs_phy_swap_fftout_dcskip(fft_out, re_array, nb_subcarriers_of_interest / 2, (S32) (2 * length - nb_subcarriers_of_interest), expo_fac);
    DUMP_ARR_ID_UART(re_array, (nb_subcarriers_of_interest * 2), "d", 0, 1);
#endif

    /* PCFI RE extraction */
    p16 = pcfi;
    q16 = re_array;
    for (i = 0; i < PCFICH_NUM_RE; i++) {
        *p16++ = q16[2 * pcfi_re_idx[i]];
        *p16++ = q16[2 * pcfi_re_idx[i] + 1];
    }

    /* derotation compensation */
    lte_bs_phy_nmm_channelEst_comp(pcfi, channelEst_buf, PCFICH_NUM_RE / 2,
        Ntx_ante,nb_subcarriers_of_interest,(U16*)pcfi_re_idx,inv_noise_var,0);
}

//-------------------------------------------------------------------------------------------
/** @brief implement PDCCH extraction
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param expoNfft        Exponent size of FFT 
 *  @param pin             Pointer to the Input IQ samples memory
 *  @param bw		       bandwidth
 *  @param pdcch_re_idx    Pointer to pdcch re index
 *  @param num_pdcch_re    Number of pdcch re
 *  @param channelEst_buf  Pointer to channel estimation memory
 *  @param pdcch           Pointer to the pbch memory
 *  @param expo_fac        Exponent number of FFT output
 *  @param Ntx_ante        Antenna number of cell TX
 *  @param inv_noise_var   Inverse number of noise variance
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length      |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |            |               |           |            |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |expoNfft     |         |Input          |1           |signed short   |(1:15:0)   |            |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |pin          |         |pInput         |1<<expoNfft |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |bw           |         |Input          |1           |signed short   |(1:15:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |pdcch_re_idx |         |pInput         |            |pReal Array    |(1:15:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |num_pdcch_re |         |Input          |1           |signed short   |(1:15:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |channelEst_buf |       |pInput         |            |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |pdcch        |         |pOutput		|            |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |expo_fac     |         |pOutput        |1           |pReal	         |(1:15:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |Ntx_ante     |         |Input          |1           |unsigned short |(1:15:0)   |[1,2,4]     |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |inv_noise_var |        |Input          |1           |unsigned int   |(0:32:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_extract_pdcch      |
 *  ---------------------------------------------------------
 *  |Input Streams       |pin,pdcch_re_idx,channelEst_buf   |
 *  --------------------------------------------------------
 *  |Output Streams      |pdcch, expo_fac				    |
 *  ---------------------------------------------------------
 *  |Config Parameters   |expoNfft,bw,num_pdcch_re,Ntx_ante,|
 *  |					 |inv_noise_var					    |
 *  ---------------------------------------------------------
 *  |Status Parameters   |none		                        |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to extract pdcch from time-domain OFDM symbol
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_SIB 
 *
 **/
void
lte_bs_phy_nmm_extract_pdcch(S16 expoNfft, S16 * pin, S16 bw, S16 * pdcch_re_idx,
                             S16 num_pdcch_re, S16 * channelEst_buf, S16 * pdcch,
                             S16 * expo_fac,U16 Ntx_ante,U32 inv_noise_var)
{
    S32             length = (1 << expoNfft);
    S16             nb_subcarriers_of_interest = bw * PRB_12;
    S16             i;
#ifdef CEVA_INTRINSICS
    S16            *fft_out;
    S16            *re_array;
    S16            *timedomain_ceva;
    S16             fft_size = (2 * (1 << expoNfft));
    S16             expo_ceva;
#else
    S16             fft_out[2 * FFTSZ_2048];
    S16             re_array[2 * FFTSZ_2048];
#endif


#ifdef CEVA_INTRINSICS
    /* pin points after CP */
    timedomain_ceva = (S16 *) DataBuff_Bank1;
    fft_out = timedomain_ceva + fft_size;
    re_array = fft_out + fft_size;
    MemCpyFromExt((void *) timedomain_ceva, (void *) pin, fft_size * sizeof(S16));

    cfft_16t32d(timedomain_ceva, expoNfft, fft_out, &expo_ceva);
    *expo_fac = expo_ceva - 5;

    /* FFT to RE mapping + DC skipping */
    lte_bs_phy_swap_fftout_dcskip_ceva(fft_out, re_array, nb_subcarriers_of_interest / 2, (S32) (2 * length - nb_subcarriers_of_interest), expo_fac);

#else
    nmm_dbg_printf_off(("[%s]\n", __FUNCTION__));
    /* skip CP */
    DUMP_ARR_ID_UART(pin, 20, "d", 1, 0);

    /* FFT */
    fftcg_c_16t24d(pin, (int) expoNfft, fft_out, expo_fac);
    DUMP_ARR_ID_UART(fft_out, 20, "d", 1, 0);

    /* FFT to RE mapping + DC skipping */
    lte_bs_phy_swap_fftout_dcskip(fft_out, re_array, nb_subcarriers_of_interest / 2, (S32) (2 * length - nb_subcarriers_of_interest), expo_fac);

#endif
    /* PDCCH RE extraction */
    for (i = 0; i < num_pdcch_re; i++) {
        pdcch[2 * i] = re_array[2 * pdcch_re_idx[i]];
        pdcch[2 * i + 1] = re_array[2 * pdcch_re_idx[i] + 1];
    }

    /* derotation compensation */
    lte_bs_phy_nmm_channelEst_comp(pdcch, channelEst_buf, num_pdcch_re / 2,
        Ntx_ante,nb_subcarriers_of_interest,(U16*)pdcch_re_idx,inv_noise_var,0);
}

//-------------------------------------------------------------------------------------------
/** @brief implement PDSCH extraction
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param expoNfft        Exponent size of FFT 
 *  @param pin             Pointer to the Input IQ samples memory
 *  @param bw		       bandwidth
 *  @param num_pdsch_rb    Number of pdsch rb
 *  @param channelEst_buf  Pointer to channel estimation memory
 *  @param pdsch           Pointer to the pbch memory
 *  @param expo_fac        Exponent number of FFT output
 *  @param Ntx_ante        Antenna number of cell TX
 *  @param pdsch_prb_set   Point to pdsch prb set memory
 *  @param cell_cp         cell CP format
 *  @param isym            symbol index
 *  @param cell_id         cell PCI
 *  @param p_N_pdsch_re    Point to pdsch re
 *  @param inv_noise_var   Inverse number of noise variance
 *  @param subsfn          subsfn
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length      |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |            |               |           |            |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |expoNfft     |         |Input          |1           |signed short   |(1:15:0)   |            |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |pin          |         |pInput         |1<<expoNfft |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |bw           |         |Input          |1           |signed short   |(1:15:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |num_pdsch_rb |         |Input          |1           |signed short   |(1:15:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |channelEst_buf |       |pInput         |            |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |pdsch        |         |pOutput		|            |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |expo_fac     |         |pOutput        |1           |pReal	         |(1:15:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |Ntx_ante     |         |Input          |1           |unsigned short |(1:15:0)   |[1,2,4]     |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |pdsch_prb_set |        |pInput         |            |pReal Array    |(1:15:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |cell_cp      |         |Input          |1           |unsigned short |(0:16:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |isym		  |         |Input          |            |unsigned short |(0:16:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |cell_id      |         |Input   		|            |unsigned short |(0:16:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |p_N_pdsch_re |         |pOutput        |1           |pReal          |(1:15:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |inv_noise_var |        |Input          |1           |unsigned int   |(0:32:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  |subsfn		  |         |Input          |1           |unsigned short |(0:16:0)   |Full Range  |      |
 *  ------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_extract_pdcch     |
 *  --------------------------------------------------------
 *  |Input Streams       |pin,pdsch_prb_set,channelEst_buf |
 *  --------------------------------------------------------
 *  |Output Streams      |pdsch, expo_fac, pdsch_prb_set   |
 *  --------------------------------------------------------
 *  |Config Parameters   |expoNfft,bw,num_pdsch_rb,Ntx_ante,|
 *  |					 |cell_cp,isym,cell_id,inv_noise_var,|
 *  |					 |subsfn							|
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to extract pdcch from time-domain OFDM symbol
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_SIB 
 *
 **/
void
lte_bs_phy_nmm_extract_pdsch(S16 expoNfft, S16 * pin, S16 bw, S16 num_pdsch_rb,
                             S16 * channelEst_buf, S16 * pdsch, S16 * expo_fac,
                             U16 Ntx_ante,U8* pdsch_prb_set,U16 cell_cp,U16 isym,
                             U16 cell_id,U16*p_N_pdsch_re,U32 inv_noise_var,U16 subsfn)
{
    S32             length = (1 << expoNfft);
    S16             nb_subcarriers_of_interest = bw * PRB_12;
    S16             i,j,j0,j1;
    U16             centStartRb,centEndRb;
    U16             nPrb;
#ifdef CEVA_INTRINSICS
    S16            *fft_out;
    S16            *re_array;
    S16            *timedomain_ceva;
    S16             fft_size = (2 * (1 << expoNfft));
    S16             expo_ceva;
	U16 			*pdsch_k_indices;
#else
    S16             fft_out[2 * FFTSZ_2048];
    S16             re_array[2 * FFTSZ_2048];
    U16             pdsch_k_indices[MAX_SC_OFDM_SYMB];
#endif

    U16             Ndl_symb,start_pilot_index=0,pilot_index_inc=0,pilot_idx2;
    U16             pilot_idx;
    U16             N_pdsch_re,cond1,cond2,cond3;
    U16             pilot_grp;
    U32             tmpU32;
    U16             n_TS;

    tmpU32 = cell_id*CONST_1_6;
    pilot_grp = tmpU32 >> 16;
    pilot_grp = cell_id - pilot_grp*6;

#ifdef CEVA_INTRINSICS
    /* pin points after CP */
    timedomain_ceva = (S16 *) DataBuff_Bank1;
    fft_out = timedomain_ceva + fft_size;
    re_array = fft_out + fft_size;
	pdsch_k_indices = (U16*)(re_array + nb_subcarriers_of_interest * 2);
    MemCpyFromExt((void *) timedomain_ceva, (void *) pin, fft_size * sizeof(S16));

    cfft_16t32d(timedomain_ceva, expoNfft, fft_out, &expo_ceva);
    *expo_fac = expo_ceva - 5;

    /* FFT to RE mapping + DC skipping */
    lte_bs_phy_swap_fftout_dcskip_ceva(fft_out, re_array, nb_subcarriers_of_interest / 2, (S32) (2 * length - nb_subcarriers_of_interest), expo_fac);

#else
    nmm_dbg_printf_off(("[%s]\n", __FUNCTION__));

    /* FFT : pdsch scale stores the exponent */
    fftcg_c_16t24d(pin, (int) expoNfft, fft_out, expo_fac);

    /* FFT to RE mapping + DC skipping */
    lte_bs_phy_swap_fftout_dcskip(fft_out, re_array, nb_subcarriers_of_interest / 2, (S32) (2 * length - nb_subcarriers_of_interest), expo_fac);
#endif
    /* we extract the data starting from index */

    Ndl_symb = (cell_cp == SHORT_CP?(SYM_PER_SUBFR_SHORT_CP/2):(SYM_PER_SUBFR_LONG_CP/2));

    cond1 = ((isym == (Ndl_symb - 3))||
        (isym == Ndl_symb) || (isym == (2*Ndl_symb - 3)));
    cond2 = ((isym == 1)||(isym == Ndl_symb+1)) && (Ntx_ante==4);

    if(cond1 || cond2)
    {
        if(Ntx_ante==1)
        {
            start_pilot_index = pilot_grp;
            if(((isym==Ndl_symb - 3)||(isym==2*Ndl_symb - 3)))
            {
                start_pilot_index = MODULO6(pilot_grp + 3);
            }
            pilot_index_inc = 6;
        }
        else if ((Ntx_ante==2) || (Ntx_ante==4))
        {
            pilot_idx2 = MODULO6(pilot_grp + 3);
            start_pilot_index = MIN(pilot_grp, pilot_idx2);
            pilot_index_inc = 3;
        }
    }
    else
    {
        start_pilot_index = PRB_12;
    }

    n_TS = 0;
    if(isym >= Ndl_symb)
        n_TS = 1;

    cond3 = ((subsfn==0) || (subsfn==5)) && ((isym == (Ndl_symb-1)) || (isym == (Ndl_symb-2)));
    cond3 |= (subsfn==0) && (isym>=Ndl_symb) && (isym<(Ndl_symb+NUM_SYMB_PBCH));

    centStartRb = (bw - PBCH_NB_RB) >> 1;
    centEndRb = centStartRb + PBCH_NB_RB - 1;

    N_pdsch_re = 0;
    for (i = 0; i < num_pdsch_rb; i++){
        nPrb = pdsch_prb_set[i+num_pdsch_rb*n_TS];
        pilot_idx = start_pilot_index;

        j0 = 0;
        j1 = PRB_12;
        if(cond3)
        {
            if((bw & 1) == 0)
            {
                if((nPrb >= centStartRb) && (nPrb <= centEndRb))
                {
                    j1=0;
                }
            }
            else
            {
                if(nPrb == centStartRb)
                {
                    j1 = PRB_12 >> 1;
                }
                else if((nPrb > centStartRb) && (nPrb <= centEndRb))
                {
                    j1 = 0;
                }
                else if(nPrb == (centEndRb+1))
                {
                    j0 = PRB_12 >> 1;
                }
            }

        }

        for (j = j0; j < j1; j++) {
            if (j == pilot_idx) {
                /* DTX or pilot */
                /* we do not take this value and update the next index */
                pilot_idx += pilot_index_inc;
            }
            else {
                pdsch[N_pdsch_re*2] = re_array[(nPrb*PRB_12+j)*2];
                pdsch[N_pdsch_re*2+1] = re_array[(nPrb*PRB_12+j)*2+1];

                pdsch_k_indices[N_pdsch_re++] = nPrb*PRB_12+j;
            }
        }//j loop
    }//i loop

#if 0
    {
        FILE * fid;
        U16 i;
        if(isym == 1)
            fid = fopen("pdsch_re.txt","wt");
        else
            fid = fopen("pdsch_re.txt","at");

        for (i=0;i<2 * nb_subcarriers_of_interest;i++)
        {
            fprintf(fid,"%d\n",re_array[i]);
        }
        fclose(fid);
    }
#endif

#if 0
    {
        FILE * fid;
        U16 i;
        if(isym == 1)
            fid = fopen("pdsch_re.txt","wt");
        else
            fid = fopen("pdsch_re.txt","at");

        for (i=0;i<2 * nb_subcarriers_of_interest;i++)
        {
            fprintf(fid,"%d\n",re_array[i]);
        }
        fclose(fid);
    }
#endif

    /* derotation compensation */
    lte_bs_phy_nmm_channelEst_comp(pdsch, channelEst_buf, N_pdsch_re / 2,
        Ntx_ante,nb_subcarriers_of_interest,pdsch_k_indices,inv_noise_var,1);

    *p_N_pdsch_re = N_pdsch_re;
#if 0
    {
        FILE * fid;
        U16 i;
        if(isym == 1)
            fid = fopen("pdsch_extract.txt","wt");
        else
            fid = fopen("pdsch_extract.txt","at");

        for (i=0;i<2 * N_pdsch_re;i++)
        {
            fprintf(fid,"%d\n",pdsch[i]);
        }
        fclose(fid);
    }
#endif
}


//-------------------------------------------------------------------------------------------
/** @brief implement channel estimation
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param Rx_RS_Symbol		  Pointer to the time-domain RS symbol memory
 *  @param ts_num			  time slot number
 *  @param cell_pci			  cell pci
 *  @param cell_cp			  cell CP format
 *  @param Ndl_rb   		  Number of downlink RB
 *  @param expo_nfft		  Number of FFT exponent
 *  @param chEst_cram		  Pointer to channel estimation memory
 *  @param Ntx_ante			  Number of TX antenna
 *  @param inv_noise_var	  Pointer to inverse of noise variance
 *  @param foeEst			  Pointer to frequency offset estimation
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |Rx_RS_Symbol |         |pInput			|1        |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |chEst_cram	  |         |pOutput		|		  |pComplex Array |(1:0:15)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  |inv_noise_var|         |pOutput	    |         |pReal Array    |(0:32:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_channel_estimate  |
 *  --------------------------------------------------------
 *  |Input Streams       |Rx_RS_Symbol   		           |
 *  --------------------------------------------------------
 *  |Output Streams      |chEst_cram,inv_noise_var,foeEst  |
 *  --------------------------------------------------------
 *  |Config Parameters   |ts_num,cell_pci,cell_cp,Ndl_rb,  | 
 *  |					 |expo_nfft,Ntx_ante			   |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do channel estimation based on DL RS
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_cell_search	
 *
 **/
void
lte_bs_phy_nmm_channel_estimate(S16 *Rx_RS_Symbol, S16 ts_num, S16 cell_pci, S16 cell_cp,
								S16 Ndl_rb, S16 expo_nfft, S16 *chEst_cram,U16 Ntx_ante,
								U32 *inv_noise_var,S32 *foeEst)
{

    U16 i, k, isym,tsymb;
    S16 phaseOffset;
    S32 Err[2], Err1[2], Err2[2]; // Real and Imag
    S16 delta[2] = {0};
    S32 L_temp1;
    S32 L_temp2;

    S16 temp1, temp2;
    S16 val1, val2, val3;

    S16 Nused;
    S16 Ndl_symb = (cell_cp == SHORT_CP) ? PBCH_SYM_START_SHORT_CP : PBCH_SYM_START_LONG_CP;
    S16 vshift = MODULO6(cell_pci);
    U16 index, index1, index2, index3;

    S16 expo_fac[MAX_RS_SYMBOL_TIME], expo_max;

    S16 *adet;
    S16 *paPilots[MAX_RS_SYMBOL_TIME];

    S16 *pChEst;
	S32 H_corr[4], H_pow;
	S32 DLRS_foe;
    S16 pilotScaleFactor;

#ifdef CEVA_INTRINSICS
	S16 expo_ceva;
    S16 *aPilots1, *aPilots2, *aPilots3, *aPilots4, *aPilots5, *aPilots6;
	S16 *chEst;
	S16 *fft_out;
	S16 *timedomain_ceva;
	S16 fft_size = (2 * (1 << expo_nfft));
#else
    S16 aPilots1[1200 * 2], aPilots2[1200 * 2], aPilots3[1200 * 2],
        aPilots4[1200 * 2], aPilots5[1200 * 2], aPilots6[1200 * 2];
    S16 chEst[2400*MAX_RS_SYMBOL];
	S16 fft_out[2 * FFTSZ_2048];
#endif


    U16 N_RS_symb,N_RS_symb_time,n_RS_symb_time;
    U16 RS_symb_time_arr[MAX_RS_SYMBOL_TIME];
    U16 RS_Ntx_limit_arr[MAX_RS_SYMBOL_TIME] = {1,1,1,1,3,3};
    U16 vshift2 = MODULO6(vshift+3);
    U16 minVshift;

    U16 RS_v_arr[MAX_RS_SYMBOL] = {0,1,0,1,/*ante 0*/
        1,0,1,0,/*ante 1*/
        0,1,/*ante2*/
        1,0};/*ante3*/

    /*v shift in generated pilots*/
    U16 RS_v2_arr[MAX_RS_SYMBOL] = {0,1,0,1,/*ante 0*/
        0,1,0,1,/*ante 1*/
        0,1,/*ante2*/
        0,1};/*ante3*/

    /* index in for aDet and PaPilots*/
    U16 RS_symb_time_symb[MAX_RS_SYMBOL] = {0,1,2,3,/*ante 0*/
        0,1,2,3,/*ante 1*/
        4,5,/*ante2*/
        4,5};/*ante3*/

    U16 n_tx_ante;
    S16 *pH0,*pH1;
    S16 intp_r,intp_i;
    S16 tmp_r,tmp_i;
    U32 sum_pwr_l = 0;
    U16 sum_pwr_h = 0;

//    U32 tmp_r32;
//    S16 h00r,h00i,h01r,h01i,h10r,h10i,h11r,h11i;
    U16 estimate_chan_gain = 1;
    U32 noise_var;
    //1/((Ndl_rb * 2 - 1)*(N_RS_SYMB_ANT0-1)*1.5) in Q(16,0)
    U16 inv_factor_noise_LUT[6] = {1324,502,297,147,98,73};
    U16 N_DL_RB_LUT[6]={6,15,25,50,75,100};
    U16 tmpU16;
    U32 tmpU32,tmpU32_2;

//U16 RS_symb_time_arr[MAX_RS_SYMBOL_TIME]={0,Ndl_symb-3,Ndl_symb,Ndl_symb*2-3,1,Ndl_symb+1};
    RS_symb_time_arr[0] = 0;
    RS_symb_time_arr[1] = Ndl_symb-3;
    RS_symb_time_arr[2] = Ndl_symb;
    RS_symb_time_arr[3] = Ndl_symb*2-3;
    RS_symb_time_arr[4] = 1;
    RS_symb_time_arr[5] = Ndl_symb+1;

    for(i=0;i<MAX_RS_SYMBOL;i++)
    {
        RS_v_arr[i] = (1-RS_v_arr[i])*vshift + RS_v_arr[i]*vshift2;
        RS_v2_arr[i] = (1-RS_v2_arr[i])*vshift + RS_v2_arr[i]*vshift2;

    }

    // Init variables
    Nused = Ndl_rb * PRB_12;
    minVshift = vshift;
    if(vshift2<vshift)
        minVshift = vshift2;

    switch(Ntx_ante){
        case 1:
            N_RS_symb_time = 4;
            N_RS_symb = 4;
            break;

        case 2:
            N_RS_symb_time = 4;
            N_RS_symb = 8;
            break;

        case 4:
            N_RS_symb_time = 6;
            N_RS_symb = 12;
            break;
        default:
            N_RS_symb_time = 4;
            N_RS_symb = 4;
            break;
    };

#ifdef CEVA_INTRINSICS
    /* pin points after CP */
    timedomain_ceva = (S16 *) DataBuff_Bank1;
    fft_out = timedomain_ceva + fft_size;
    //adet = fft_out + fft_size;
    adet = (S16*)(DataBuff_Bank1 + 0x6040); // cfft_16t32d use as internal mem from 0x4040 to 0x6040
	aPilots1 = adet + Nused * 2 * N_RS_symb_time;
	aPilots2 = aPilots1 + Nused * 2;
	aPilots3 = aPilots2 + Nused * 2;
	aPilots4 = aPilots3 + Nused * 2;
	aPilots5 = aPilots4 + Nused * 2;
	aPilots6 = aPilots5 + Nused * 2;
	chEst = aPilots6 + Nused * 2;
#else
    /*adet[0-3] for ante 0/1; adet[4-5] for ante 2/3        */
    adet = MemAlloc(sizeof(S16) * 2 * Nused * N_RS_symb_time, CRAM_MEM, 1);

#endif




    expo_max = -16;

    // step 1: Do FFT : Rx_RS_Symbol[0-3] for ante 0/1; Rx_RS_Symbol[4-5] for ante 2/3
    for (k = 0; k < N_RS_symb_time; k++)
    {
#ifdef CEVA_INTRINSICS
        MemCpyFromExt((void *) timedomain_ceva, (void *) (Rx_RS_Symbol + fft_size * k), fft_size * sizeof(S16));

        cfft_16t32d(timedomain_ceva, expo_nfft, fft_out, &expo_ceva);
        expo_fac[k] = expo_ceva - 5;

        /* FFT to RE mapping + DC skipping */
        lte_bs_phy_swap_fftout_dcskip_ceva(fft_out, adet + Nused * 2 * k, Nused >> 1, (S32) (fft_size - Nused), expo_fac);
#else
        /* FFT */
        fftcg_c_16t24d(Rx_RS_Symbol + 2 * (1 << expo_nfft) * k, expo_nfft, fft_out, &expo_fac[k]);
        /* FFT to RE mapping + DC skipping */
        lte_bs_phy_swap_fftout_dcskip(fft_out, adet + 2 * Nused * k, Nused / 2, (S32) (2 * (1 << expo_nfft) - Nused), &expo_fac[k]);
#endif
        if (expo_max < expo_fac[k])
            expo_max = expo_fac[k];
    }

    // align the expo_fac of adet to expo_max
    for (k = 0; k < N_RS_symb_time; k++)
    {
        if (expo_fac[k] < expo_max)
        {
            index1 = 2 * Nused * k;
            for (i = 0; i < 2 * Nused; i++)
            {
                index = index1 + i;
                adet[index]  = (S16)L_shr(adet[index], (S16) (expo_max - expo_fac[k]));
            }
        }
    }


    // step 2: generate local DL pilot of timeslot 0
    /*paPilots: [0-3] for ante 0; [4-5] for ante 2*/
    pilotScaleFactor = 32767; //actually 2^14 is used
    paPilots[0] = aPilots1;
    paPilots[1] = aPilots2;
    paPilots[2] = aPilots3;
    paPilots[3] = aPilots4;
    paPilots[4] = aPilots5;
    paPilots[5] = aPilots6;

    for(n_RS_symb_time=0;n_RS_symb_time<N_RS_symb_time;n_RS_symb_time++)
    {
        DlPilot(cell_pci, cell_cp, Ndl_rb, Ndl_symb, PRB_12, RS_Ntx_limit_arr[n_RS_symb_time],
             pilotScaleFactor, RS_symb_time_arr[n_RS_symb_time], ts_num,
             &paPilots[n_RS_symb_time-(RS_Ntx_limit_arr[n_RS_symb_time]-1)]);
    }


    //-----------------------------------------------------------------------------------------------------
    // Step 3: Channel Estimation at pilot locations. i.e. initial channel estimation by division of raw detected values with pilots

    /*RS symbol: 0-3 for ante0; 4-7 for ante1; 8-9 for ante2; 10-11 for ante3*/
    for (isym = 0; isym < N_RS_symb; isym++)
    {
        tsymb = RS_symb_time_symb[isym];
        for (k = 0; k < Nused; k++)
        {
            chEst[2 * (k + isym * Nused)] = 0;
            chEst[2 * (k + isym * Nused) + 1] = 0;
        }

        val1 = Nused * 2 * tsymb + 2 * RS_v_arr[isym]; /*offset for aDet from the OFDM symbol and vshift*/
        val2 = 2 * RS_v2_arr[isym];  /*offset for papilot from vshift*/
        val3 = Nused * 2 * isym  + 2 * RS_v_arr[isym]; /*offset for chest from the OFDM symbol and vshift*/
        for (i = 0; i < Ndl_rb * 2; i++)
        {
            index1 = 2 * (6 * i) + val1; //for adet; // to account for storing a matrix column by column (symbol based)
            index =  2 * (6 * i) + val2; /*for papilots*/
            index3 = 2 * (6 * i) + val3; /*for chest*/
            //        chEst[index] = add(mult(adet[index], aPilots1[lindex]),
            //                    mult(adet[index+1],aPilots1[lindex+1]));
            //        chEst[index+1] = sub(mult(adet[index+1], aPilots1[lindex]),
            //                    mult(adet[index],aPilots1[lindex+1]));

            L_temp1 = L_mult(adet[index1], paPilots[tsymb][index]);
            L_temp1 = L_mac(L_temp1, adet[index1 + 1], paPilots[tsymb][index + 1]);
            chEst[index3] = extract_h(L_temp1);

            L_temp2 = L_mult(adet[index1 + 1], paPilots[tsymb][index]);
            L_temp2 = L_msu(L_temp2, adet[index1], paPilots[tsymb][index + 1]);
            chEst[index3 + 1] = extract_h(L_temp2);
        }
    }

    // End Step 3

    /*Noise estimation*/

    if(estimate_chan_gain == 1){
        sum_pwr_l = 0;
        sum_pwr_h = 0;
        for (isym = 0; isym < (N_RS_SYMB_ANT0-1); isym++)
        {
            if(RS_v_arr[isym]<RS_v_arr[isym+1])
            {
                pH0 = &chEst[Nused * 2 * isym  + 2 * RS_v_arr[isym]];
                pH1 = &chEst[Nused * 2 * (isym+1)  + 2 * RS_v_arr[isym+1]];

            }
            else
            {
                pH1 = &chEst[Nused * 2 * isym  + 2 * RS_v_arr[isym]];
                pH0 = &chEst[Nused * 2 * (isym+1)  + 2 * RS_v_arr[isym+1]];
            }


            for (i = 0; i < Ndl_rb * 2 - 1; i++)
            {
                intp_r = add(shr(pH0[0],2), shr(pH0[6*2],2));
                intp_i = add(shr(pH0[1],2), shr(pH0[6*2+1],2));
                tmp_r = sub(intp_r,shr(pH1[0],1));
                tmp_i = sub(intp_i,shr(pH1[1],1));

                L_temp1 = L_mult(tmp_r,tmp_r);
                L_temp2 = L_mac(L_temp1,tmp_i,tmp_i); //Q(32,5)
                L_temp2 = L_shl(L_temp2,1); //Q(32,4)

                sum_pwr_l = sum_pwr_l + L_temp2;
                sum_pwr_h += (sum_pwr_l >>31 ) & 1;
                sum_pwr_l &= 0x7fffffff;

                pH0 += 6*2;
                pH1 += 6*2;
            }
        }

        /*combine the sum power*/
        sum_pwr_l |= ((sum_pwr_h&1)<<31);
        sum_pwr_h >>= 1;

        /*mean of noise power*/
        for(val1=0;val1<6;val1++)
        {
            if(N_DL_RB_LUT[val1]==Ndl_rb)
                break;
        }
        val1 = inv_factor_noise_LUT[val1];

        tmpU16 = sum_pwr_l & 0xffff;
        tmpU32 = (tmpU16 * val1) >> 16;

        tmpU16 = (sum_pwr_l >> 16) & 0xffff;
        tmpU32_2 = tmpU16 * val1;
        tmpU32 += tmpU32_2;

        tmpU32_2 = (sum_pwr_h * val1) << 16;
        noise_var = tmpU32 + tmpU32_2;

        if(noise_var == 0)
        {
            noise_var = 1;

        }
        /*inv noise var*/
        val1 = norm_l(noise_var); // val1 in [0,30]
        L_temp1 = L_shl(noise_var,val1); //Q(32,4-val1)
        val2 = extract_h(L_temp1); //Q(16,4-val1); decimal 12+val1
        val2 = divide_tab_lookup(val2); //Q(16,-13) for Q(16,16); Q(16,-13+12+val1) = Q(16,-1+val1)

        L_temp1 = val1 << 16;
        *inv_noise_var = (val2 & 0xffff) | L_temp1;

    }



	// End Step 3


    /*
    S16 inverse_length_ip[6] =
    { 23831, 18079, 21400, 21183, 28150, 21077 };// 1/([12 30 50 100 150 200] -1)
    S16 inverse_length_ip_expo[6] =
    { 3, 4, 5, 6, 7, 7 }; // Correspnding Q in addition to Q15  */
    switch (Nused)
    {
        case 72:
            temp1 = 23831;
            temp2 = 3;
            break;
        case 180:
            temp1 = 18079;
            temp2 = 4;
            break;
        case 300:
            temp1 = 21400;
            temp2 = 5;
            break;
        case 600:
            temp1 = 21183;
            temp2 = 6;
            break;
        case 900:
            temp1 = 28150;
            temp2 = 7;
            break;
        case 1200:
            temp1 = 21077;
            temp2 = 7;
            break;
        default:
#ifdef _WIN32
            printf("Nused is out of bounds %hd\r\n",Nused);
            exit(1);
#else
            return;
#endif
            //break;
    }

	// Do frequency offset estimation [0 4; 7 11]
    //Frequency domain linear interpolation
    if (vshift > 2)     //min(MODULO6(3 + vshift), vshift)
	{
        index1 = vshift - 3;

		for (isym = 0; isym < 4; isym+=2)	// interpolation for symbol 0, 7
		{
			index2 = (index1 + Nused * isym) * 2;
			delta[0] = (chEst[index2 + 18] - chEst[index2 + 6]);
			delta[1] = (chEst[index2 + 19] - chEst[index2 + 7]);

			chEst[index2] = chEst[index2 + 6] - (delta[0] >> 1);
			chEst[index2 + 1] = chEst[index2 + 7] - (delta[1] >> 1);

			for (k = 1; k < Ndl_rb * 2; k++)
			{
				index2 += 12;
				delta[0] = (chEst[index2 + 6] - chEst[index2 - 6]);
				delta[1] = (chEst[index2 + 7] - chEst[index2 - 5]);

				chEst[index2] = chEst[index2 + 6] - (delta[0] >> 1);
				chEst[index2 + 1] = chEst[index2 + 7] - (delta[1] >> 1);
			}
		}
	}
    else
	{
        index1 = vshift;
		for (isym = 1; isym < 4; isym+=2)	// interpolation for symbol 4, 11
		{
			index2 = (index1 + Nused * isym) * 2;
			delta[0] = (chEst[index2 + 18] - chEst[index2 + 6]);
			delta[1] = (chEst[index2 + 19] - chEst[index2 + 7]);

			chEst[index2] = chEst[index2 + 6] - (delta[0] >> 1);
			chEst[index2 + 1] = chEst[index2 + 7] - (delta[1] >> 1);

			for (k = 1; k < Ndl_rb * 2; k++)
			{
				index2 += 12;
				delta[0] = (chEst[index2 + 6] - chEst[index2 - 6]);
				delta[1] = (chEst[index2 + 7] - chEst[index2 - 5]);

				chEst[index2] = chEst[index2 + 6] - (delta[0] >> 1);
				chEst[index2 + 1] = chEst[index2 + 7] - (delta[1] >> 1);
			}
		}
	}

	if (vshift > 2)     //min(MODULO6(3 + vshift), vshift)
		index1 = vshift - 3;
	else
		index1 = vshift;

	H_pow = 0;
	for (isym = 0; isym < 4; isym+=2)	// corrlation between symbol 0&4, 7&11
	{
		H_corr[isym] = 0;
		H_corr[isym+1] = 0;
		index2 = (index1 + Nused * isym) * 2;
		index3 = (index1 + Nused * (isym+1)) * 2;
		for (k = 0; k < Ndl_rb * 2; k++, index2 += 12, index3 += 12)
		{
			// (a-b*j)*(c+d*j) = (ac + bd) + j*(ad - bc)
			L_temp1 = L_mult(mult(chEst[index2], temp1), chEst[index3]);
			H_corr[isym] = L_add(H_corr[isym], L_shr(L_temp1, temp2));

			L_temp1 = L_mult(mult(chEst[index2 + 1], temp1), chEst[index3 + 1]);
			H_corr[isym] = L_add(H_corr[isym], L_shr(L_temp1, temp2));

			L_temp1 = L_mult(mult(chEst[index2], temp1), chEst[index3 + 1]);
			H_corr[isym + 1] = L_add(H_corr[isym + 1], L_shr(L_temp1, temp2));

			L_temp1 = L_mult(mult(chEst[index2 + 1], temp1), chEst[index3]);
			H_corr[isym + 1] = L_sub(H_corr[isym + 1], L_shr(L_temp1, temp2));

			L_temp1 = L_mult(mult(chEst[index2], temp1), chEst[index2]);
			H_pow = L_add(H_pow, L_shr(L_temp1, temp2+1));

			L_temp1 = L_mult(mult(chEst[index3], temp1), chEst[index3]);
			H_pow = L_add(H_pow, L_shr(L_temp1, temp2+1));

		}
	}

	H_corr[0] += H_corr[2];
	H_corr[1] += H_corr[3];
	phaseOffset = get_angle_fx(H_corr[0], H_corr[1]);     // Q13, floor(atan(H_corr[1]/H_corr[0]) * 2^13)
	DLRS_foe = L_mult(phaseOffset, 1115) >> 2; // Q13, 15.36e6/(1024*4+72*4) = 3503.65Hz, 3503.65/(2*pi)= 557.62

	//threshold: 0.0112*2^15 = 367
	H_pow = L_mult(extract_h(H_pow), 367);
	H_corr[2] = L_mult(L_shr(H_corr[0], 15), L_shr(H_corr[0], 15));
	H_corr[2] = L_add(H_corr[2], L_mult(L_shr(H_corr[1], 15), L_shr(H_corr[1], 15)));
	if (H_corr[2] > H_pow)
	{
		*foeEst = DLRS_foe;
		if (*foeEst == 0)
			*foeEst = 1;
	}
	else
		*foeEst = 0;

    //-----------------------------------------------------------------------------------------------------
    // Step 4: Do fine timing adjustment to remove the phase rotation with frequency.
    //calculate time offset, only use the central 6 RB

    Err1[0] = 0; // Real
    Err1[1] = 0; // Imag

    val1 = vshift + (Ndl_rb - 6) * 6;
    for (i = 0; i < 5 * 2; i++)
    {
		if (i == 5)	// bypass the DLRS across DC
			val1 += 12;

        index1 = 2 * (val1 + 6 * i);
        index2 = 2 * (val1 + 6 * (i + 1));

        L_temp1 = L_mult(mult(chEst[index1], 23831), chEst[index2]);
        Err1[0] = L_add(Err1[0], L_shr(L_temp1, 3));

        L_temp1 = L_mult(mult(chEst[index1 + 1], 23831), chEst[index2 + 1]);
        Err1[0] = L_add(Err1[0], L_shr(L_temp1, 3));

        L_temp1 = L_mult(mult(chEst[index1], 23831), chEst[index2 + 1]);
        Err1[1] = L_add(Err1[1], L_shr(L_temp1, 3));

        L_temp1 = L_mult(mult(chEst[index1 + 1], 23831), chEst[index2]);
        Err1[1] = L_sub(Err1[1], L_shr(L_temp1, 3));
    }

    // after conversion to floating point need to multiply by ([12 30 50 100 150 200]-1)
    // to match floating point since we scale down by the inverse in order to avoid saturation

    Err2[0] = 0;
    Err2[1] = 0;
    val1 = Nused + MODULO6(3 + vshift) + (Ndl_rb - 6) * 6;
    for (i = 0; i < 5 * 2; i++)
    {
        // Err2 = sum(conj(chEst(mi+CE_MI1).symbol(nDlSymb-3+CE_MI1).element(i1+CE_MI1)) .* chEst(mi+CE_MI1).symbol(nDlSymb-3+CE_MI1).element(i2+CE_MI1));

		if (i == 5)	// bypass the DLRS across DC
			val1 += 12;

        index1 = 2 * (val1 + 6 * i);
        index2 = 2 * (val1 + 6 * (i + 1));

        L_temp1 = L_mult(mult(chEst[index1], 23831), chEst[index2]);
        Err2[0] = L_add(Err2[0], L_shr(L_temp1, 3));

        L_temp1 = L_mult(mult(chEst[index1 + 1], 23831), chEst[index2 + 1]);
        Err2[0] = L_add(Err2[0], L_shr(L_temp1, 3));

        L_temp1 = L_mult(mult(chEst[index1], 23831), chEst[index2 + 1]);
        Err2[1] = L_add(Err2[1], L_shr(L_temp1, 3));

        L_temp1 = L_mult(mult(chEst[index1 + 1], 23831), chEst[index2]);
        Err2[1] = L_sub(Err2[1], L_shr(L_temp1, 3));
    }

    //average over two reference symbols    &expo_fac[0]
    Err[0] = L_add(Err1[0], Err2[0]);
    Err[1] = L_add(Err1[1], Err2[1]);

    phaseOffset = get_angle_fx(Err[0], Err[1]);     // Q13, floor(atan(Err[1]/Err[0]) * 2^13)
	// 1/(2*pi*6/1024) * 2^10 = 27814
	*(foeEst+1) = L_shr(L_mult(phaseOffset, 27814), 24);

    //-----------------------------------------------------------------------------------------------------
    // Step 5: Do linear interpolation in frequency domain.
    // combine the channel estimation of two timeslots

    for(n_tx_ante=0;n_tx_ante<Ntx_ante;n_tx_ante++)
    {
        if(n_tx_ante<2)
        {
            for (isym = n_tx_ante*4; isym <= n_tx_ante*4+1; isym++)
            {
                val1 = Nused * 2 * isym + 2 * RS_v_arr[isym]; /*offset for the 1st RS symb from OFDM symb*/
                val2 = Nused * 2 * 2; /*offset for the 2nd RS symb relative to the 1st RS symbol*/
                val3 = Nused * 2 * n_tx_ante + 2 * RS_v_arr[isym];/*offset for dest due to ante and v*/

                for (i = 0; i < Ndl_rb * 2; i++)
                {
                    index1 = 2 * (6 * i) + val1;
                    index2 = index1 + val2;
                    index3 = 2 * (6 * i) + val3;

                    temp1 = shr(chEst[index1], 1);
                    temp2 = shr(chEst[index2], 1);
                    chEst[index3] = add(temp1, temp2);

                    temp1 = shr(chEst[index1 + 1], 1);
                    temp2 = shr(chEst[index2 + 1], 1);
                    chEst[index3 + 1] = add(temp1, temp2);
                }
            }
        }
        else
        {
            for (isym = n_tx_ante*2+4; isym <= n_tx_ante*2+5; isym++)
            {
                val1 = Nused * 2 * isym + 2 * RS_v_arr[isym]; /*offset for the source RS symb */
                val2 = Nused * 2 * n_tx_ante + 2 * RS_v_arr[isym]; /*offset for the dest RS symb*/

                for (i = 0; i < Ndl_rb * 2; i++)
                {
                    index1 = 2 * (6 * i) + val1;
                    index2 = 2 * (6 * i) + val2;
                    chEst[index2] = chEst[index1] ;
                    chEst[index2 + 1] = chEst[index1+1];
                }
            }
        }

        //Frequency domain moving average, TBD

        //Frequency domain linear interpolation

        pChEst = chEst + Nused*2*n_tx_ante;
        index1 = minVshift;
        
        if (index1 > 0)
        {
            index2 = index1 << 1;

            delta[0] = (pChEst[index2 + 6] - pChEst[index2]);
            delta[1] = (pChEst[index2 + 7] - pChEst[index2 + 1]);

            delta[0] = mult_r(delta[0], CONST_1_3);
            delta[1] = mult_r(delta[1], CONST_1_3);
            for (k = 0; k < index1; k++)
            {
                pChEst[k << 1] = pChEst[index2] - (index1 - k) * delta[0];
                pChEst[(k << 1) + 1] = pChEst[index2 + 1] - (index1 - k) * delta[1];
            }
        }

        for (i = 0; i < Ndl_rb * 2 * 2 - 1; i++)
        {
            index2 = (index1 + 3 * i) << 1;

            if (i == (Ndl_rb * 2 - 1))
            {
                delta[0] = (pChEst[index2 + 6] - pChEst[index2]) >> 2;
                delta[1] = (pChEst[index2 + 7] - pChEst[index2 + 1]) >> 2;

                for (k = 1; k < (3 - index1); k++)
                {
                    pChEst[index2 + (k << 1)] = pChEst[index2] + k * delta[0];
                    pChEst[index2 + (k << 1) + 1] = pChEst[index2 + 1] + k * delta[1];
                }
                for (k = (3 - index1); k < 3; k++)
                {
                    pChEst[index2 + (k << 1)] = pChEst[index2] + (k+1) * delta[0];
                    pChEst[index2 + (k << 1) + 1] = pChEst[index2 + 1] + (k+1) * delta[1];
                }
            }
            else
            {
                delta[0] = (pChEst[index2 + 6] - pChEst[index2]);
                delta[1] = (pChEst[index2 + 7] - pChEst[index2 + 1]);
                delta[0] = mult_r(delta[0], CONST_1_3);
                delta[1] = mult_r(delta[1], CONST_1_3);

                for (k = 1; k < 3; k++)
                {
                    pChEst[index2 + (k << 1)] = pChEst[index2] + k * delta[0];
                    pChEst[index2 + (k << 1) + 1] = pChEst[index2 + 1] + k * delta[1];
                }
            }

        }

        if (index1 < 2)
        {
            index2 = (index1 + 3 * (Ndl_rb * 2 * 2 - 1)) << 1;

            delta[0] = (pChEst[index2] - pChEst[index2 - 6]);
            delta[1] = (pChEst[index2 + 1] - pChEst[index2 - 5]);
            delta[0] = mult_r(delta[0], CONST_1_3);
            delta[1] = mult_r(delta[1], CONST_1_3);

            for (k = index1 + 1; k < 3; k++)
            {
                pChEst[index2 + (k << 1)] = pChEst[index2] + k * delta[0];
                pChEst[index2 + (k << 1) + 1] = pChEst[index2 + 1] + k * delta[1];
            }
        }

    }

#ifdef CEVA_INTRINSICS
    MemCpyToExt((void *) chEst_cram, (void *) chEst, 2 * Nused * sizeof(S16)*Ntx_ante);
#else
    memcpy(chEst_cram, chEst, 2 * Nused * sizeof(S16)*Ntx_ante);
	MemFree(adet,CRAM_MEM);
#endif

#if 0
    {
        FILE * fid;
        U16 i;
        fid = fopen("che.txt","wt");

        for (i=0;i<2 * Nused *Ntx_ante;i++)
        {
            fprintf(fid,"%d\n",chEst[i]);
        }
        fclose(fid);
    }
#endif

}


//-------------------------------------------------------------------------------------------
/** @brief implement channel estimation
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param xcorr_buf		  Pointer to last input data memory
 *  @param IQdecimated		  Pointer to input data memory
 *  @param xcorr_arr		  Pointer to correlation result memory
 *  @param agc_shift		  Number of shift due to AGC
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |Rx_RS_Symbol |         |pInput			|1        |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |chEst_cram	  |         |pOutput		|		  |pComplex Array |(1:0:15)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  |inv_noise_var|         |pOutput	    |         |pReal Array    |(0:32:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_xcorr_loop		   |
 *  --------------------------------------------------------
 *  |Input Streams       |xcorr_buf,IQdecimated,zc_cand    |
 *  --------------------------------------------------------
 *  |Output Streams      |xcorr_arr					       |
 *  --------------------------------------------------------
 *  |Config Parameters   |agc_shift		    			   |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do correlation on rx data and local PSS sequence
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_cell_search	
 *
 **/
void
lte_bs_phy_nmm_xcorr_loop(S16 * xcorr_buf, S16 * IQdecimated, S16 * zc_cand, S16 * xcorr_arr, S32 * agc_gain)
{
    S16            *pin;
    S16             i;
	S16				xcorr_tmp;
	S16				shift;

	if (agc_gain[1] == 255)
		shift = 0;
	else
		shift = (S16) (agc_gain[0] - agc_gain[1]) * 2;

    /* restore previous samples (64 points) */
    memcpy(&xcorr_buf[2 * (FFT_SIZE_096MHZ - 1 + SSS_SEARCH_RANGE)], IQdecimated, (2 * (FFT_SIZE_096MHZ - 1)) * sizeof(S16));

    for (i = 0; i < (2 * SUBFR_SIZE_096MHZ); i += 2) {
        if (i < (2 * (FFT_SIZE_096MHZ - 1)))
            pin = &xcorr_buf[i + (2 * SSS_SEARCH_RANGE)];
        else
            pin = &IQdecimated[i - (2 * (FFT_SIZE_096MHZ - 1))];

#ifdef CEVA_INTRINSICS
        lte_bs_phy_nmm_pss_xcorr_x1641(pin, zc_cand, &xcorr_tmp, FFT_SIZE_096MHZ);
#else
        lte_bs_phy_nmm_pss_xcorr(pin, zc_cand, &xcorr_tmp, FFT_SIZE_096MHZ);
#endif

		if (shift >= 0)
		{
			*xcorr_arr = add(*xcorr_arr, xcorr_tmp >> (shift+3));
		}
		else if (shift < 0)
		{
			*xcorr_arr = add((*xcorr_arr) >> (-shift), xcorr_tmp >> 3);
		}

		//*xcorr_arr = xcorr_tmp;

        xcorr_arr++ ;
    }
}


//-------------------------------------------------------------------------------------------
/** @brief implement PSS decision based on PSS correlation results
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param xcorr_arr_ptr 	  Pointer to PSS correlation memory
 *  @param psync_found_arr	  Pointer to PSS decision result
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |xcorr_arr_ptr |         |pInput			|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |psync_found_arr |         |pOutput		|		  |pComplex Array |(1:0:15)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_pss_candidate_selection_loop		   |
 *  --------------------------------------------------------
 *  |Input Streams       |xcorr_arr_ptr					   |
 *  --------------------------------------------------------
 *  |Output Streams      |psync_found_arr				   |
 *  --------------------------------------------------------
 *  |Config Parameters   |				    			   |
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to PSS decision based on the PSS correlation results
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_cell_search	
 *
 **/
void
lte_bs_phy_nmm_pss_candidate_selection_loop(S16 * xcorr_arr_ptr, S16 * psync_found_arr)
{
    S16             i, j, uIdx, cnt, minIdx, minValue, maxIdx, maxValue, index;
    S16            *xcorr_arr;
	S32				noiseSum;

#ifdef CEVA_INTRINSICS
	S16			   *peakValue_arr, *peakIdx_arr;
	S16			   *Pnr_arr;
	S16			   *tmpWin;
	xcorr_arr = (S16 *) DataBuff_Bank1;
	peakValue_arr = xcorr_arr + SUBFR_SIZE_096MHZ;
	peakIdx_arr = peakValue_arr + MAX_CELL_FOUND_LIST * 3;
	Pnr_arr = peakIdx_arr + MAX_CELL_FOUND_LIST * 3;
	tmpWin = Pnr_arr + MAX_CELL_FOUND_LIST * 3;
#else
	S16				peakValue_arr[MAX_CELL_FOUND_LIST * 3], peakIdx_arr[MAX_CELL_FOUND_LIST * 3];
	S16				Pnr_arr[MAX_CELL_FOUND_LIST * 3];
	S16				tmpWin[64];
#endif

	for (uIdx = 0; uIdx < 3; uIdx++)
	{
#ifdef CEVA_INTRINSICS
		MemCpyFromExt((void *) xcorr_arr, (void *) (xcorr_arr_ptr + uIdx * SUBFR_SIZE_096MHZ), SUBFR_SIZE_096MHZ * sizeof(S16));
#else
		xcorr_arr = xcorr_arr_ptr + uIdx * SUBFR_SIZE_096MHZ;
#endif

		for (cnt = 0; cnt < MAX_CELL_FOUND_LIST; cnt++)
		{
			//find the max among the correlations
			index = uIdx*MAX_CELL_FOUND_LIST+cnt;
			peakValue_arr[index] = 0;
			for (i = 0; i < SUBFR_SIZE_096MHZ; i++)
			{
				if (peakValue_arr[index] < xcorr_arr[i])
				{
					peakValue_arr[index] = xcorr_arr[i];
					peakIdx_arr[index] = i;
				}
			}

			//choose the min value in the previous window
			minValue = peakValue_arr[index];
			for (i = 0; i < 64; i++)
			{
				minIdx = peakIdx_arr[index] + i - 65;
				if (minIdx < 0)
					minIdx += SUBFR_SIZE_096MHZ;

				if (minValue > xcorr_arr[minIdx])
					minValue = xcorr_arr[minIdx];
			}

			//set the max to the min value
			for (i = 0; i < 3; i++)
			{
				minIdx = peakIdx_arr[index] - i + 1;
				if (minIdx < 0)
					minIdx += SUBFR_SIZE_096MHZ;

				xcorr_arr[minIdx] = minValue;
			}

		}

		//calculate the PNR
		for (cnt = 0; cnt < MAX_CELL_FOUND_LIST; cnt++)
		{
			index = uIdx*MAX_CELL_FOUND_LIST+cnt;
			for (i = 0; i < 64; i++)
			{
				minIdx = peakIdx_arr[index] + i - 65;
				if (minIdx < 0)
					minIdx += SUBFR_SIZE_096MHZ;

				tmpWin[i] = xcorr_arr[minIdx];
			}

			// select minimum 32 values as noise
			noiseSum = 0;
			for (j = 0; j < 32; j++)
			{
				minValue = tmpWin[j];
				minIdx = j;

				for (i = j + 1; i < 64; i++)
				{
					if (minValue > tmpWin[i])
					{
						minIdx = i;
						minValue = tmpWin[i];
					}
				}

				noiseSum += minValue;
				tmpWin[minIdx] = tmpWin[j];
			}

			noiseSum >>= 5;
			if (noiseSum < 10)
				noiseSum = 10;

			noiseSum = divide_s(1, (S16) noiseSum);	//Q15

			Pnr_arr[index] = extract_h(L_mult(peakValue_arr[index], (S16) noiseSum));
		}
	}

	//select the max 32 PNR values
	for (cnt = 0; cnt < MAX_CELL_FOUND_LIST; cnt++)
	{
		maxValue = Pnr_arr[0];
		maxIdx = 0;
		for (i = 1; i < MAX_CELL_FOUND_LIST * 3; i++)
		{
			if (maxValue < Pnr_arr[i])
			{
				maxValue = Pnr_arr[i];
				maxIdx = i;
			}
		}

		if (maxValue >= 16 )	//1.9647 * 8
		{
			if (maxIdx < MAX_CELL_FOUND_LIST)	// root
				psync_found_arr[cnt*2] = 0;
			else if (maxIdx < MAX_CELL_FOUND_LIST * 2)
				psync_found_arr[cnt*2] = 1;
			else
				psync_found_arr[cnt*2] = 2;

			psync_found_arr[cnt*2 + 1] = peakIdx_arr[maxIdx];
			psync_found_arr[cnt*2 + MAX_CELL_FOUND_LIST*2] = peakValue_arr[maxIdx];
			psync_found_arr[cnt*2 + MAX_CELL_FOUND_LIST*2 + 1] = Pnr_arr[maxIdx];

			Pnr_arr[maxIdx] = 0;
		}
		else
		{
			psync_found_arr[cnt*2] = 3; // indicate end of valid cell
			return;
		}
	}
}


//-------------------------------------------------------------------------------------------
/** @brief implement RSRP Measurement
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param expoNfft        Exponent size of FFT 
 *  @param pin             Pointer to the Input IQ samples memory
 *  @param symb_index	   symbol index
 *  @param cell_pci		   cell pci
 *  @param cell_cp         cell CP format
 *  @param bandwidth_rb    Number of RB which used to calculate RSRP
 *  @param rsrp            Pointer to the rsrp memory
 *  @param rsrp_scale      scale factor for rsrp
 *  @param num_pilots      Number of pilots
 *  @param size_inverse    Number of inversed rsrp measure period
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pin |         |pInput			|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |rsrp |         |pOutput		|		  |pReal Array |(1:0:31)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_get_rsrp		   |
 *  --------------------------------------------------------
 *  |Input Streams       |pin							   |
 *  --------------------------------------------------------
 *  |Output Streams      |rsrp		    				   |
 *  --------------------------------------------------------
 *  |Config Parameters   |expoNfft,symb_index,cell_pci,cell_cp |
 *  |					 |bandwidth_rb,rsrp_scale,num_pilots,size_inverse  
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do rsrp measurement
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_cell_search	
 *
 **/
void
lte_bs_phy_nmm_get_rsrp(S16 expoNfft, S16 * pin, S16 symb_index, S16 cell_pci, S16 cell_cp, S16 bandwidth_rb, S32 * rsrp, S16 rsrp_scale,
                        S16 num_pilots, S16 size_inverse)
{
	S16 fft_done_ptr, expo_fac;
    S16 i;
    S32 L_temp1, acc;

#ifdef CEVA_INTRINSICS
	S16 *dl_pilot;
    dl_pilot = (S16 *) DataBuff_Bank2;
    lte_bs_phy_nmm_extract_dlpilots_ceva(expoNfft, pin, symb_index, &fft_done_ptr, cell_pci, cell_cp, bandwidth_rb, dl_pilot, &expo_fac);
#else
	S16 dl_pilot[2 * NMAXRB_100 * REF_SIGNAL_PER_RB];
    lte_bs_phy_nmm_extract_dlpilots(expoNfft, pin, symb_index, &fft_done_ptr, cell_pci, cell_cp, bandwidth_rb, dl_pilot, &expo_fac);
#endif

    acc = 0;
    for (i = 0; i < 2 * num_pilots; i += 2) {
        L_temp1 = L_mult(dl_pilot[i], dl_pilot[i]);
        L_temp1 = L_mac(L_temp1, dl_pilot[i + 1], dl_pilot[i + 1]);
        acc = L_mac(acc, size_inverse, extract_h(L_temp1));
    }

	acc = L_shl(acc, expo_fac * 2);

	if (acc > 32767)
	{
		acc = L_shr(acc, 15);
		if (acc > 32767)
			acc = 32767;
		L_temp1 = L_shr(L_mult((S16) acc, rsrp_scale), 1);
	}
	else
		L_temp1 = L_shr(L_mult((S16) acc, rsrp_scale), 16);

	*rsrp = L_add(*rsrp, L_temp1);
}


//-------------------------------------------------------------------------------------------
/** @brief implement QPSK demodulation
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param RXIN            Pointer to the Input data memory
 *  @param bDetSoft  	   Pointer to demodulated data memory
 *  @param iShift		   Number of shift
 *  @param iLength         Number of RXIN
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pin |         |pInput			|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |rsrp |         |pOutput		|		  |pReal Array |(1:0:31)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_demod_qpsk		   |
 *  --------------------------------------------------------
 *  |Input Streams       |RXIN							   |
 *  --------------------------------------------------------
 *  |Output Streams      |bDetSoft		    			   |
 *  --------------------------------------------------------
 *  |Config Parameters   |iShift, iLength				   | 
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do QPSK demodulation
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_phy	
 *
 **/
void
lte_bs_phy_nmm_demod_qpsk(S32 RXIN[], S8 * bDetSoft, S16 iShift, S32 iLength)
{
    S32             i,
                    j;
    S16             I,
                    Q;

    for (i = 0, j = 0; i < iLength; i += 2) {
        I = extract_h(L_shl(RXIN[i], iShift));
        Q = extract_h(L_shl(RXIN[i + 1], iShift));

        bDetSoft[j++] = (S8) (I >> 8);
        bDetSoft[j++] = (S8) (Q >> 8);

    }
}


//-------------------------------------------------------------------------------------------
/** @brief implement PBCH demodulation
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param pbch            Pointer to the input pbch memory
 *  @param pbch_scale      Number of scale of input pbch data
 *  @param bDetSoft  	   Pointer to demodulated data memory
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pbch |         |pInput			|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |bDetSoft |         |pOutput		|		  |pReal Array |(1:0:31)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_demod_pbch		   |
 *  --------------------------------------------------------
 *  |Input Streams       |pbch,pbch_scale				   |
 *  --------------------------------------------------------
 *  |Output Streams      |bDetSoft		    			   |
 *  --------------------------------------------------------
 *  |Config Parameters   |none				   | 
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do pbch demodulation
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_MIB	
 *
 **/
void
lte_bs_phy_nmm_demod_pbch(S16 * pbch, S16 pbch_re_per_frame, S16 * pbch_scale, S8 * bdet_soft)
{
    int             i;
    U16             isym;
	U16				nused_e[PBCH_NUM_SYM] = {48, 48, PBCH_BW, PBCH_BW};
    //S32             pbch32[2 * PBCH_RE_PER_FRAME];
    //S32            *pbch32_ptr = pbch32;
    S32            *pbch32,
                   *pbch32_ptr;
    S16             nshift;

    /* rewind 4 symbols to get individual symbol scale */
    S16            *pexpo = &pbch_scale[0];

#ifdef CEVA_INTRINSICS
    pbch32 = (S32*)(DataBuff_Bank1);
#else
    pbch32 = MemAlloc(2 * pbch_re_per_frame * sizeof(S32), CRAM_MEM, 0);
#endif

    pbch32_ptr = pbch32;
	nused_e[PBCH_NUM_SYM-1] = pbch_re_per_frame - 48*2 - PBCH_BW;
    /* generate 32 bit PBCH content with individual symbol scaling */
    for (isym = 0; isym < PBCH_NUM_SYM; isym++) {
        for (i = 0; i < nused_e[isym]; i++) {
            *pbch32_ptr++ = L_shl(*pbch++, pexpo[isym]);
            *pbch32_ptr++ = L_shl(*pbch++, pexpo[isym]);
        }
    }

    nshift = block_lnorm(pbch32, (2 * pbch_re_per_frame), 0);

    /* QPSK PBCH demodulation */
    lte_bs_phy_nmm_demod_qpsk(pbch32, bdet_soft, nshift, (S32) (2 * pbch_re_per_frame));

#ifndef CEVA_INTRINSICS
    MemFree(pbch32, CRAM_MEM);
#endif

}

//-------------------------------------------------------------------------------------------
/** @brief implement channel estimation compensation
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param input            Pointer to the input data memory
 *  @param channelEst_buf   Pointer to channel estimation memory
 *  @param length			Number of input data length
 *  @param Ntx_ante         Number of tx antenna
 *  @param Nused		    Number of used RB
 *  @param k_indices		k index
 *  @param inv_noise_var_flt   Number of inverse of noise variance
 *  @param use_noise_var	flag to using noise variance
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pbch |         |pInput			|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |bDetSoft |         |pOutput		|		  |pReal Array |(1:0:31)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_channelEst_comp   |
 *  --------------------------------------------------------
 *  |Input Streams       |input,channelEst_buf,k_indices   |
 *  --------------------------------------------------------
 *  |Output Streams      |bDetSoft		    			   |
 *  --------------------------------------------------------
 *  |Config Parameters   |lengthm,Ntx_ante,Nused,		   |
 *  |					 |inv_noise_var_flt,use_noise_var  | 
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do channel estimation compensation
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_MIB	
 *
 **/
void
lte_bs_phy_nmm_channelEst_comp(S16 *input, S16 *channelEst_buf, S16 length,U16 Ntx_ante,
                               U16 Nused,U16* k_indices,U32 inv_noise_var_flt,U16 use_noise_var)
{
    S16         k;
    S16         index1, index2,index3;
    S32         L_temp1;
    S32         L_temp2;
    S32         L_temp1a;
    S32         L_temp2a;
    S32         L_temp3;
    S16         val1, val2, val3, val4;
    S16         val1a, val2a, val3a, val4a;
//    U16         Nrx = 1;
    S16         d0r,d0i,d1r,d1i,h00r,h00i,h01r,h01i,h10r,h10i,h11r,h11i;
    U16         k_index;
    U16         scale;
    S16         shft;
    U16         inv_noise_var = inv_noise_var_flt & 0xffff;
    U16         inv_exp = inv_noise_var_flt >> 16;

    //apply de-rotation to raw demodulated values

    /*rx data in Q(16,1);
    che in Q(16,2);
    inv noise var in Q(16,-1+exp)*/

    if(Ntx_ante == 1)
    {

        if(use_noise_var==1)
        {
            L_temp1 = L_mult(inv_noise_var,SQRT_2); //Q(32,exp)
            scale = extract_h(L_temp1); // in Q(16,exp)
            shft = inv_exp+1+1-N_INT_SOFT_OUT; // 2nd 1 for *2
        }
        else
        {
            scale = 0x7fff;
            shft = 0;
        }

        for (k = 0; k < length; k++) {

            index1 = 4 * k;
            index2 = 4 * k;

            k_index = k_indices[k*2];
            val1 = channelEst_buf[k_index*2];
            val2 = channelEst_buf[k_index*2 + 1];
            k_index = k_indices[k*2+1];
            val1a = channelEst_buf[k_index*2];
            val2a = channelEst_buf[k_index*2 + 1];

            L_temp1 = L_mult(val1, input[index2]);          //Q(16,2),Q(16,1); Q(32,2)
            L_temp2 = L_mult(val1, input[index2 + 1]);
            L_temp1a = L_mult(val1a, input[index2 + 2]);
            L_temp2a = L_mult(val1a, input[index2 + 3]);

            val3 = mac_r(L_temp1, val2, input[index2 + 1]); //Q(16,2)
            val4 = msu_r(L_temp2, val2, input[index2]);
            val3a = mac_r(L_temp1a, val2a, input[index2 + 3]);
            val4a = msu_r(L_temp2a, val2a, input[index2 + 2]);


            L_temp1 = L_mult(val3,scale); //Q(32,2+exp-1) = Q(32,exp+1)
            L_temp2 = L_mult(val4,scale);
            L_temp1a = L_mult(val3a,scale);
            L_temp2a = L_mult(val4a,scale);

            if(shft >=0)
            {
                input[index2] = extract_h(L_shl(L_temp1,shft));  //Q(16,N_INT_SOFT_OUT)
                input[index2+1] = extract_h(L_shl(L_temp2,shft));
                input[index2+2] = extract_h(L_shl(L_temp1a,shft));
                input[index2+3] = extract_h(L_shl(L_temp2a,shft));

            }
            else
            {
                input[index2] = extract_h(L_shr(L_temp1,shft));
                input[index2+1] = extract_h(L_shr(L_temp2,shft));
                input[index2+2] = extract_h(L_shr(L_temp1a,shft));
                input[index2+3] = extract_h(L_shr(L_temp2a,shft));
            }

        }
    }
    else {

        index1 = 0; /*for data*/
        if(Ntx_ante == 2)
        {
            val2 = 0;
            val3 = Nused*2;
        }
        else
        {
            val2 = Nused*2;
            val3 = Nused*4;
        }

        if(use_noise_var==1)
        {
            scale = inv_noise_var;
            shft = (inv_exp+1)+2-N_INT_SOFT_OUT; // 2nd 2 for *4
        }
        else
        {
            scale = 0x7fff;
            shft = 0;
        }

        for (k = 0; k < length; k++) {
           d0r = input[index1];
           d0i = input[index1+1];
           d1r = input[index1+2];
           d1i = input[index1+3];

           k_index = k_indices[k*2];
           index2 = k_index*2 + (k & 1)*val2; /*for channel ante 0 or 0/1*/
           index3 = index2 + val3; /*for channel ante 1 or 2/3*/
           h00r = channelEst_buf[index2];
           h00i = channelEst_buf[index2+1];
           h10r = shr(channelEst_buf[index3],1);
           h10i = shr(channelEst_buf[index3+1],1);

           k_index = k_indices[k*2+1];
           index2 = k_index*2 + (k & 1)*val2; /*for channel ante 0 or 0/1*/
           index3 = index2 + val3; /*for channel ante 1 or 2/3*/
           h01r = channelEst_buf[index2];
           h01i = channelEst_buf[index2+1];
           h11r = shr(channelEst_buf[index3],1);
           h11i = shr(channelEst_buf[index3+1],1);

           /*real x0*/
           L_temp1 = L_mult(h00r, d0r);
           L_temp1 = L_mac(L_temp1,h00i,d0i);
           L_temp1 = L_mac(L_temp1,h11r,d1r);
           val1a = mac_r(L_temp1,h11i,d1i);         //Q(16,3)
           L_temp1a = L_mult(val1a,scale);       //Q(32,3+(-1+exp)-1)=Q(32,1+exp)


           /*imag x0*/
           L_temp1 = L_mult(h00r, d0i);
           L_temp1 = L_msu(L_temp1,h00i,d0r);
           L_temp1 = L_mac(L_temp1,h11i,d1r);
           val1a = msu_r(L_temp1,h11r,d1i);
           L_temp2 = L_mult(val1a,scale);

           /*real x1*/
           L_temp1 = L_mult(h01r, d1r);
           L_temp1 = L_mac(L_temp1,h01i,d1i);
           L_temp1 = L_msu(L_temp1,h10r,d0r);
           val1a = msu_r(L_temp1,h10i,d0i);
           L_temp2a = L_mult(val1a,scale);

           /*imag x1*/
           L_temp1 = L_mult(h01r, d1i);
           L_temp1 = L_msu(L_temp1,h01i,d1r);
           L_temp1 = L_msu(L_temp1,h10i,d0r);
           val1a = mac_r(L_temp1,h10r,d0i);
           L_temp3 = L_mult(val1a,scale);


            if(shft >=0)
            {
                input[index1] = extract_h(L_shl(L_temp1a,shft));  //Q(16,N_INT_SOFT_OUT)
                input[index1+1] = extract_h(L_shl(L_temp2,shft));
                input[index1+2] = extract_h(L_shl(L_temp2a,shft));
                input[index1+3] = extract_h(L_shl(L_temp3,shft));

            }
            else
            {
                input[index1] = extract_h(L_shr(L_temp1a,shft));
                input[index1+1] = extract_h(L_shr(L_temp2,shft));
                input[index1+2] = extract_h(L_shr(L_temp2a,shft));
                input[index1+3] = extract_h(L_shr(L_temp3,shft));
            }

           index1 += 4;
        }
    }
}

//-------------------------------------------------------------------------------------------
/** @brief implement frequency offset compensation
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param pin	            Pointer to the input data memory
 *  @param freqOffset	    Number of estimated frequency offset
 *  @param startPos         start position
 *  @param length			Number of input data length
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pin |         |pInput/pOutput		|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |freqOffset |         |Input		|		  |Real  |(1:0:31)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_freqOffset_comp   |
 *  --------------------------------------------------------
 *  |Input Streams       |pin							   |
 *  --------------------------------------------------------
 *  |Output Streams      |pin		    				   |
 *  --------------------------------------------------------
 *  |Config Parameters   |freqOffset,startPos,length	   | 
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do frequency offset compensation
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_phy	
 *
 **/
void
lte_bs_phy_nmm_freqOffset_comp(S16 *pin, S32 freqOffset, S16 startPos, S16 length)
{
    S32             i;
    S16             a,
                    b,
                    c,
                    d;
    S32             ac,
                    bd,
                    ad,
                    bc;
    S16             tblMap,
                    shift,
                    noffset,
                    NegExpoNoffset;
    S16             *fk;
    S16             *pderotation_buf;


#ifdef CEVA_INTRINSICS
    fk = (S16*)(DataBuff_Bank1);
    pderotation_buf = fk + length;
#else
    fk = MemAlloc(length * sizeof(S16), CRAM_MEM, 0);
    pderotation_buf = MemAlloc(2 * length * sizeof(S16), CRAM_MEM, 0);
#endif

    tblMap = 4474; // Q24, floor(4096/15.36e6*2^24), the phase is independent of the FFT size as the derotation factor will use 4096 look-up table directly
    shift = norm_l(freqOffset) - 16;    // let L_shl(freqOffset, shift) be S16
    noffset = mult(tblMap, (S16) L_shl(freqOffset, shift));  // Q24-15+shift = Q9+shift
    NegExpoNoffset = -(shift+9);

    for (i = 0; i < length; i++)
        fk[i] = (S16) (startPos + i);

#ifdef CEVA_INTRINSICS
    Compute_Derotation_PartOne_A(length, noffset, NegExpoNoffset, 1, fk, pderotation_buf);
#else
    Compute_Derotation_PartOne(length, noffset, NegExpoNoffset, 1, fk, pderotation_buf);
#endif

    for (i = 0; i < length; i++)
    {
        a = pderotation_buf[2 * i];
        b = pderotation_buf[2 * i + 1];
        c = pin[2 * i];
        d = pin[2 * i + 1];

        ac = L_mult(a, c);
        bd = L_mult(b, d);
        ad = L_mult(a, d);
        bc = L_mult(b, c);

        pin[2 * i] = extract_h(L_sub(ac, bd));
        pin[2 * i + 1] = extract_h(L_add(ad, bc));
    }

#ifndef CEVA_INTRINSICS
    MemFree(fk, CRAM_MEM);
    MemFree(pderotation_buf, CRAM_MEM);
#endif

}

//-------------------------------------------------------------------------------------------
/** @brief implement frequency offset estimation base on PSS
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param pin	            Pointer to the input data memory
 *  @param pZC			    Pointer to ZC sequence in time domain
 *  @param PSync_foe        Pointer to foe result
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pin |         |pInput		|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |PSync_foe |         |pOutput		|		  |Real  |(1:0:31)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_pss_foe		   |
 *  --------------------------------------------------------
 *  |Input Streams       |pin, pZC						   |
 *  --------------------------------------------------------
 *  |Output Streams      |PSync_foe		    			   |
 *  --------------------------------------------------------
 *  |Config Parameters   |none							   | 
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do frequency offset estimation based on PSS
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_cell_search	
 *
 **/
void
lte_bs_phy_nmm_pss_foe(S16 *pin, S16 *pZC, S32 *PSync_foe)
{
    S32             i,
                    j;
    S16             a,
                    b,
                    c,
                    d;
    S32             accI,
                    accQ,
                    ac,
                    bd,
                    ad,
                    bc;
    S16             phaseOffset;
    S16             *h_est;

#ifdef CEVA_INTRINSICS
    h_est = (S16*)(DataBuff_Bank1);
#else
    h_est = MemAlloc(4 * sizeof(S16), CRAM_MEM, 0);
#endif

    accI = 0;
    accQ = 0;
    j = FFT_SIZE_096MHZ + 16;
    for (i = 0; i < FFT_SIZE_096MHZ / 2; i++, j++) {
        a = pZC[2 * i];
        b = -pZC[2 * i + 1];
        c = pin[2 * j];
        d = pin[2 * j + 1];

        ac = L_mult(a, c);
        bd = L_mult(b, d);
        ad = L_mult(a, d);
        bc = L_mult(b, c);

        accI += L_shr(L_sub(ac, bd), 5);
        accQ += L_shr(L_add(ad, bc), 5);
    }
    h_est[0] = extract_h(accI);
    h_est[1] = extract_h(accQ);

    accI = 0;
    accQ = 0;
    for (i = FFT_SIZE_096MHZ / 2; i < FFT_SIZE_096MHZ; i++, j++) {
        a = pZC[2 * i];
        b = -pZC[2 * i + 1];
        c = pin[2 * j];
        d = pin[2 * j + 1];

        ac = L_mult(a, c);
        bd = L_mult(b, d);
        ad = L_mult(a, d);
        bc = L_mult(b, c);

        accI += L_shr(L_sub(ac, bd), 5);
        accQ += L_shr(L_add(ad, bc), 5);
    }
    h_est[2] = extract_h(accI);
    h_est[3] = extract_h(accQ);

    // (a-bj)*(c+dj) = (ac + bd) + j*(ad -bc)
    ac = L_mult(h_est[0], h_est[2]);
    bd = L_mult(h_est[1], h_est[3]);
    ad = L_mult(h_est[0], h_est[3]);
    bc = L_mult(h_est[1], h_est[2]);

    accI = L_add(ac, bd);
    accQ = L_sub(ad, bc);

    phaseOffset = get_angle_fx(accI, accQ);     // Q13, floor(atan(accQ/accI) * 2^13)

    *PSync_foe = L_mult(phaseOffset, 2387); // Q13, unit: Hz, floor(0.96e6/(2*pi)/32) = 4774, use 2387 is due to L_mult will left shift 1

    // compensate frequency offset
    lte_bs_phy_nmm_freqOffset_comp(pin, L_shr(*PSync_foe, 13) * 16, 0, FFT_SIZE_096MHZ*2+16); // decimate rate is 16

#ifndef CEVA_INTRINSICS
    MemFree(h_est, CRAM_MEM);
#endif
}


//-------------------------------------------------------------------------------------------
/** @brief implement PDCCH de-interleaver
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param in_bits	        Pointer to the input bit memory
 *  @param dlen			    Number of data length
 *  @param Qm			    Number of Qm
 *  @param out_bits         Pointer to out bit memory
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |in_bits |         |pInput		|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |out_bits |         |pOutput		|		  |Real  |(1:0:31)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_subblock_deinterleaver_pdcch		   |
 *  --------------------------------------------------------
 *  |Input Streams       |in_bits						   |
 *  --------------------------------------------------------
 *  |Output Streams      |out_bits		    			   |
 *  --------------------------------------------------------
 *  |Config Parameters   |dlen,Qm						   | 
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do PDCCH deinterleaver
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_SIB	
 *
 **/
void
lte_bs_phy_nmm_subblock_deinterleaver_pdcch(S16 in_bits[], S16 dlen, S16 Qm,
                                            S16 out_bits[])
{
    S16             i,
                    j;
    S16             irow,
                    icol,
                    yPtr;
    S16             ND,
                    iv,
                    ii;
    S16             ColTCSubblock,
                    RowTCSubblock;
    S16             index;
    S16           **ymat;
    S16           **ypmat;
    S16            *y;
    S16            *dummy = NULL;
    U32             SizeP;
    U32             SizeD;
    S16            *tmp;

    // Define constants, internal to this function

    // Step (1.1) Define number of columns in matrix
    ColTCSubblock = 32;

    // Step (1.2) Define number of rows in matrix
    RowTCSubblock = 0;
    while (dlen > RowTCSubblock * ColTCSubblock) {
        RowTCSubblock = RowTCSubblock + 1;
    }

    SizeP = RUP32B(RowTCSubblock * sizeof(S16 *));
    SizeD = RowTCSubblock * ColTCSubblock * sizeof(S16);
#if ORIG_CODE
    ymat = smatrix0(RowTCSubblock - 1, ColTCSubblock - 1);
    ypmat = smatrix0(RowTCSubblock - 1, ColTCSubblock - 1);
#else

#ifndef CEVA_INTRINSICS
    ymat = (S16 **) MemAlloc(SizeP + SizeD, CRAM_MEM, 0);
    ypmat = (S16 **) MemAlloc(SizeP + SizeD, CRAM_MEM, 0);
    y = svector(0, RowTCSubblock * ColTCSubblock - 1);
#else
    ymat = (S16 **) DataBuff_Bank2;
    ypmat = ymat + SizeP + SizeD;
    y = (S16 *) (ypmat + SizeP + SizeD);
#endif

    tmp = (S16 *) ((S8 *) ymat + SizeP);
    memset(tmp, 0, SizeD);
    for (i = 0; i < RowTCSubblock; i++) {
        ymat[i] = tmp;
        tmp += ColTCSubblock;
    }
    tmp = (S16 *) ((S8 *) ypmat + SizeP);
    memset(tmp, 0, SizeD);
    for (i = 0; i < RowTCSubblock; i++) {
        ypmat[i] = tmp;
        tmp += ColTCSubblock;
    }
#endif

    // Step (1.3) Copy sub-blocks to rectangular matrix
    ND = 0;
    if (RowTCSubblock * ColTCSubblock > dlen) {
        ND = RowTCSubblock * ColTCSubblock - dlen;
#ifndef CEVA_INTRINSICS
        dummy = svector(0, ND - 1);
#else
        dummy = y + RowTCSubblock * ColTCSubblock;
#endif
        for (i = 0; i < ND; i++) {
            dummy[i] = -1;
        }
    }

    // first, copy to vector (length Row x Col), starting with ND
    // dummy <NULL> bits if necessary to pad to this length
    for (j = 0; j < ND; j++) {
        y[j] = dummy[j];
    }
    for (j = 0; j < dlen; j++) {
        y[j + ND] = j;
    }

    // copy vector elements to rectangular matrix, one row at a time
    yPtr = 0;
    for (irow = 0; irow < RowTCSubblock; irow++) {
        for (icol = 0; icol < ColTCSubblock; icol++) {
            ymat[irow][icol] = y[yPtr + icol];
            // printf("YMAT %hd %hd\r\n", irow, ymat[irow][icol]);
        }
        yPtr = yPtr + ColTCSubblock;
    }

    // Step (1.4) do 1st kind of inter-column permutation
    for (irow = 0; irow < RowTCSubblock; irow++) {
        for (icol = 0; icol < ColTCSubblock; icol++) {
            ypmat[irow][icol] = ymat[irow][CA_RMCTC_InterColPermConv[icol]];
            // printf("YPMAT %hd %hd %hd\r\n", irow, CA_RMCTC_InterColPermConv[icol], ypmat[irow][icol]);
        }
    }

    // Step (1.5) read sub-block interleaver output bits from columns
    iv = 0;                     // initial index to output bit
    // vector of bits, v, = consecutive columns of bits from yp (transposed for row vector)
    for (icol = 0; icol < ColTCSubblock; icol++) {
        for (irow = 0; irow < RowTCSubblock; irow++) {
            // Keep only non Null
            if (ypmat[irow][icol] != -1) {
                index = ypmat[irow][icol];
                for (ii = 0; ii < Qm * QUADRUPLET; ii++) {
                    out_bits[ii + Qm * QUADRUPLET * index] = in_bits[iv];
                    // printf("OUT %hd %hd %hd %hd %hd \r\n", index, icol, irow, ii, out_bits[ii+Qm*QUADRUPLET*index]);
                    iv++;
                }
            }
        }
    }
#ifndef CEVA_INTRINSICS
#if ORIG_CODE
    free_smatrix0(ymat);
    free_smatrix0(ypmat);
#else
    MemFree(ymat, CRAM_MEM);
    MemFree(ypmat, CRAM_MEM);
#endif
    free_svector(y, 0, RowTCSubblock * ColTCSubblock - 1);
    if (ND > 0) {
        free_svector(dummy, 0, ND - 1);
    }
#endif
    return;
}

//-------------------------------------------------------------------------------------------
/** @brief implement PDCCH bit reshuffle
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param bUserDetSoft     Pointer to the PDCCH bit memory
 *  @param pci			    Number of pci
 *  @param numPDCCHQuadruplets   Number of PDCCH quadruplets
 *  @param Qm			    Number of Qm
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |bUserDetSoft |         |pInput		|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pci |         |Input		|		  |Real  |(1:0:31)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_reshuffle_pdcch_bits |
 *  --------------------------------------------------------
 *  |Input Streams       |bUserDetSoft					   |
 *  --------------------------------------------------------
 *  |Output Streams      |bUserDetSoft		    		   |
 *  --------------------------------------------------------
 *  |Config Parameters   |pci,numPDCCHQuadruplets,Qm	   | 
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do PDCCH deinterleaver
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_SIB	
 *
 **/
#if ORIG_CODE
void
lte_bs_phy_nmm_reshuffle_pdcch_bits(S16 pci, S16 * bUserDetHard, S16 * bUserDetSoft, S16 numPDCCHQuadruplets, S16 Qm)
{
    S16             start_adress,
                    offset;
    S16             i,
                    ii;
    S16            *tempvec1,
                   *tempvec2;

    tempvec1 = svector(0, Qm * QUADRUPLET * numPDCCHQuadruplets - 1);
    tempvec2 = svector(0, Qm * QUADRUPLET * numPDCCHQuadruplets - 1);

    /* Undo Cyclic Shift */
    start_adress = pci % numPDCCHQuadruplets;
    offset = start_adress * Qm * QUADRUPLET;

    for (i = 0; i < numPDCCHQuadruplets - start_adress; i++) {
        for (ii = 0; ii < Qm * QUADRUPLET; ii++) {
            tempvec1[Qm * QUADRUPLET * i + ii + offset] = bUserDetHard[ii + i * Qm * QUADRUPLET];
            tempvec2[Qm * QUADRUPLET * i + ii + offset] = bUserDetSoft[ii + i * Qm * QUADRUPLET];
        }
    }

    for (i = 0; i < start_adress; i++) {
        for (ii = 0; ii < Qm * QUADRUPLET; ii++) {
            tempvec1[Qm * QUADRUPLET * i + ii] = bUserDetHard[ii + i * Qm * QUADRUPLET + offset];
            tempvec2[Qm * QUADRUPLET * i + ii] = bUserDetSoft[ii + i * Qm * QUADRUPLET + offset];
        }
    }

    /* Undo SubBlock Interleaver */
    SubBlockDeInterleaver_PDCCH(tempvec1, numPDCCHQuadruplets, Qm, bUserDetHard);
    SubBlockDeInterleaver_PDCCH(tempvec2, numPDCCHQuadruplets, Qm, bUserDetSoft);

    free_svector(tempvec1, 0, Qm * QUADRUPLET * numPDCCHQuadruplets - 1);
    free_svector(tempvec2, 0, Qm * QUADRUPLET * numPDCCHQuadruplets - 1);
}
#else
void
lte_bs_phy_nmm_reshuffle_pdcch_bits(S16 pci, S16 * bUserDetSoft,
									S16 numPDCCHQuadruplets, S16 Qm)
{
    S16             start_adress,
                    offset;
    S16             i,
                    ii;
    S16            *tempvec;

#ifndef CEVA_INTRINSICS
    tempvec = svector(0, Qm * QUADRUPLET * numPDCCHQuadruplets - 1);
#else
    tempvec = (S16 *) DataBuff_Bank1;
#endif

    /* Undo Cyclic Shift */
    start_adress = pci % numPDCCHQuadruplets;
    offset = start_adress * Qm * QUADRUPLET;

    for (i = 0; i < numPDCCHQuadruplets - start_adress; i++) {
        for (ii = 0; ii < Qm * QUADRUPLET; ii++) {
            tempvec[Qm * QUADRUPLET * i + ii + offset] = bUserDetSoft[ii + i * Qm * QUADRUPLET];
        }
    }

    offset = (numPDCCHQuadruplets - start_adress)* Qm * QUADRUPLET;
    for (i = 0; i < start_adress; i++) {
        for (ii = 0; ii < Qm * QUADRUPLET; ii++) {
            tempvec[Qm * QUADRUPLET * i + ii] = bUserDetSoft[ii + i * Qm * QUADRUPLET + offset];
        }
    }

    /* Undo SubBlock Interleaver */
    lte_bs_phy_nmm_subblock_deinterleaver_pdcch(tempvec, numPDCCHQuadruplets, Qm, bUserDetSoft);

#ifndef CEVA_INTRINSICS
    free_svector(tempvec, 0, Qm * QUADRUPLET * numPDCCHQuadruplets - 1);
#endif
}
#endif

//-------------------------------------------------------------------------------------------
/** @brief implement PDCCH demodulation
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param pdcch     Pointer to the PDCCH data memory
 *  @param NusedE    Number of used subcarriers
 *  @param pci		 Number of pci
 *  @param bDetSoft  Pointer to demodulation PDCCH data memory
 *  @param cinit	 Number of cinit
 *  @param pdcch_scrambling_seq  Pointer to PDCCH scramble data memory
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pdcch		  |         |pInput		|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pci |         |Input		|		  |Real  |(1:0:31)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_pdcch_demod	   |
 *  --------------------------------------------------------
 *  |Input Streams       |pdcch							   |
 *  --------------------------------------------------------
 *  |Output Streams      |bDetSoft,pdcch_scrambling_seq	   |
 *  --------------------------------------------------------
 *  |Config Parameters   |NusedE,pci,cinit,Qm			   | 
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do PDCCH demodulation
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_SIB	
 *
 **/
void
lte_bs_phy_nmm_pdcch_demod(S16 * pdcch, S16 NusedE, S16 pci, S8 * bDetSoft,
                           U32 cinit, U8 * pdcch_scrambling_seq)
{
    S16             nshift;
    S16             i;
    S16             numPDCCHQuadruplets;
    S32            *pdcch32;
    U16             k;
    U16             NdeScramBits;

#ifndef CEVA_INTRINSICS
    pdcch32 = MemAlloc(2 * NusedE * sizeof(U32), CRAM_MEM, 1);
#else
    pdcch32 = (S32 *) DataBuff_Bank3;
#endif

    numPDCCHQuadruplets = NusedE >> 2;
    lte_bs_phy_nmm_reshuffle_pdcch_bits(pci, pdcch, numPDCCHQuadruplets, 2);

    /* Convert to 32 bit PDCCH content with individual symbol scaling */
    for (k = 0; k < NusedE; k++) {
        pdcch32[2 * k] = *pdcch++;
        pdcch32[2 * k + 1] = *pdcch++;
    }

    nshift = block_lnorm(pdcch32, (2 * NusedE), 0);

    /* Demodulate the PDCCH */
    lte_bs_phy_nmm_demod_qpsk(pdcch32, bDetSoft, nshift, (S32) (2 * NusedE));

    NdeScramBits = PDCCH_MAX_SIZE_IN_BITS;
    if((2 * NusedE) < NdeScramBits)
    {
        NdeScramBits = 2 * NusedE;
    }

    PseudoRandomSeqGen(NdeScramBits, cinit, pdcch_scrambling_seq);

    for (i = 0; i < NdeScramBits; i++) {
        /* descrambled sequence = input sequence xor'd with pseudorandom sequence */
        if (pdcch_scrambling_seq[i] != 0)
		{
            if(bDetSoft[i]==-128)
                bDetSoft[i]=127;
            else
                bDetSoft[i] = -bDetSoft[i];
		}
    }
#ifndef CEVA_INTRINSICS
    MemFree(pdcch32, CRAM_MEM);
#endif
}


//-------------------------------------------------------------------------------------------
/** @brief implement c0 and c1 of sss sequence generation
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param NID_2	 Number of NID_2
 *  @param c0		 Pointer to c0 memory
 *  @param c1		 Pointer to c1 memory
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pdcch		  |         |pInput		|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pci |         |Input		|		  |Real  |(1:0:31)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_sss_gen_c0c1	   |
 *  --------------------------------------------------------
 *  |Input Streams       |								   |
 *  --------------------------------------------------------
 *  |Output Streams      |c0, c1						   |
 *  --------------------------------------------------------
 *  |Config Parameters   |NID_2							   | 
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to generate c0 and c1 of SSS sequence
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_cell_search	
 *
 **/
void
lte_bs_phy_nmm_sss_gen_c0c1(S16 NID_2, S16 * c0, S16 * c1)
{
    S16             x[FFT_SIZE_096MHZ / 2];
    S16             c[FFT_SIZE_096MHZ / 2];
    S32             i,
                    n0,
                    n1;

    x[0] = 0;
    x[1] = 0;
    x[2] = 0;
    x[3] = 0;
    x[4] = 1;
    for (i = 0; i <= 25; i++) {
        x[i + 5] = x[i + 3] ^ x[i];
    }
    for (i = 0; i <= 30; i++) {
        c[i] = 1 - x[i] - x[i];
    }
    n0 = NID_2;
    n1 = 3 + NID_2;
    for (i = 0; i <= 30; i++) {
        c0[i] = c[n0];
        c1[i] = c[n1];
        n0++;
        n1++;
        if (n0 >= 31)
            n0 -= 31;
        if (n1 >= 31)
            n1 -= 31;
    }
}

//-------------------------------------------------------------------------------------------
/** @brief implement z1 and m1 of sss sequence generation
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param m1st  	 Number of m1st
 *  @param pz1m1st	 Pointer to pz1m1st memory
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pdcch		  |         |pInput		|      |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |pci |         |Input		|		  |Real  |(1:0:31)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_sss_gen_c0c1	   |
 *  --------------------------------------------------------
 *  |Input Streams       |none							   |
 *  --------------------------------------------------------
 *  |Output Streams      |pz1m1st						   |
 *  --------------------------------------------------------
 *  |Config Parameters   |m1st							   | 
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to generate z1 and m1 of SSS sequence
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_cell_search	
 *
 **/
void
lte_bs_phy_nmm_sss_gen_z1m1st(S16 m1st, S16 * pz1m1st)
{
    S16             x[FFT_SIZE_096MHZ / 2];
    S16             z[FFT_SIZE_096MHZ / 2];
    S32             i,
                    n,
                    mm8;

    x[0] = 0;
    x[1] = 0;
    x[2] = 0;
    x[3] = 0;
    x[4] = 1;
    for (i = 0; i <= 25; i++) {
        x[i + 5] = (x[i + 4] + x[i + 2] + x[i + 1] + x[i]) & 1;
    }
    for (i = 0; i <= 30; i++) {
        z[i] = 1 - x[i] - x[i];
    }
    mm8 = m1st & 0x0007;
    for (n = 0; n <= 30; n++) {
        pz1m1st[n] = z[mm8];
        mm8++;
        if (mm8 >= 31)
            mm8 -= 31;
    }
}

//-------------------------------------------------------------------------------------------
/** @brief implement SSS decision
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param SSyncEst  Pointer to SSS data memory	 
 *  @param s0_ref    Pointer to S0 reference sequence
 *  @param NID_2	 Number of NID_2	 
 *  @param NID_cell  Pointer to detected cell NID
 *  @param Subframe	 Pointer to detected subframe
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |SSyncEst     |         |pInput		    |         |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |s0_ref		  |         |pInput		    |         |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |NID_2		  |         |Input		    |         |Real           |(1:15:0)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |NID_cell	  |         |pOutput	    |         |pReal          |(1:15:0)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |Subframe     |         |pOutput	    |		  |pReal          |(1:15:0)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_ssync_decode_sss  |
 *  --------------------------------------------------------
 *  |Input Streams       |SSyncEst,s0_ref				   |
 *  --------------------------------------------------------
 *  |Output Streams      |NID_cell,Subframe				   |
 *  --------------------------------------------------------
 *  |Config Parameters   |NID_2							   | 
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to get SSS pass the threshold decision
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_cell_search	
 *
 **/
void
lte_bs_phy_nmm_ssync_decode_sss(S16 * SSyncEst, S16 *s0_ref, S16 NID_2, S16 * NID_cell, S16 * Subframe)
{
    // LISTENING_CELL_SEARCH_DESC *this = (LISTENING_CELL_SEARCH_DESC *) cell_search;
    S16             d[FFT_SIZE_096MHZ*2];
    S16             d_even[FFT_SIZE_096MHZ];
    S16             d_odd[FFT_SIZE_096MHZ];
    S16             c0[FFT_SIZE_096MHZ / 2];
    S16             c1[FFT_SIZE_096MHZ / 2];
    S16             m1st,
                    m2nd,
                    m0,
                    m1;
    S16            *pz1m1st,
                    NID_1;
    S32             xaccI,
                    xaccQ,
                    xaccPow,
                    xaccMax;
    S32             rxPow,
                    rxPowSum,
                    SSS_thresh;
    S32             j,
                    k;

    *NID_cell = -1;

    j = 33;
    for (k = 0; k < 31; k++, j++) {
        d[k*2] = SSyncEst[j*2];
        d[k*2+1] = SSyncEst[j*2+1];
    }
    j = 1;
    for (k = 31; k < 62; k++, j++) {
        d[k*2] = SSyncEst[j*2];
        d[k*2+1] = SSyncEst[j*2+1];
    }

    rxPowSum = 0;
    for (k = 0; k < 31; k++) {
        d_even[k*2] = d[k*4];
        d_even[k*2+1] = d[k*4 + 1];
        d_odd[k*2] = d[k*4 + 2];
        d_odd[k*2+1] = d[k*4 + 3];

        rxPow = L_mult(d[k*4], d[k*4]);
        rxPowSum += rxPow >> 6;
        rxPow = L_mult(d[k*4+1], d[k*4+1]);
        rxPowSum += rxPow >> 6;
        rxPow = L_mult(d[k*4+2], d[k*4+2]);
        rxPowSum += rxPow >> 6;
        rxPow = L_mult(d[k*4+3], d[k*4+3]);
        rxPowSum += rxPow >> 6;
    }
    SSS_thresh = (rxPowSum >> 7) * 12;  //0.0945 * 2^7 = 12.1

    lte_bs_phy_nmm_sss_gen_c0c1(NID_2, c0, c1);

    /*
     * Divide d[2n]/c0[n] --> s0^(m0)[n]
     */
    for (k = 0; k < 31; k++) {
        d_even[k*2] = d_even[k*2] * c0[k];
        d_even[k*2+1] = d_even[k*2+1] * c0[k];
    }

    /*
     * Search for m_1st
     */
    xaccMax = 0;
    m1st = -1;
    for (j = 0; j < 31; j++) {
        xaccI = 0;
        xaccQ = 0;
        for (k = 0; k < 31; k++) {
            xaccI = xaccI + d_even[k*2] * s0_ref[j + k];
            xaccQ = xaccQ + d_even[k*2+1] * s0_ref[j + k];
        }
        xaccI >>= 5;
        xaccQ >>= 5;
        xaccPow = L_mult((S16) xaccI, (S16) xaccI);
        xaccPow += L_mult((S16) xaccQ, (S16) xaccQ);
        if (xaccMax < xaccPow) {
            xaccMax = xaccPow;
            m1st = (S16) j;
        }
    }
    if (xaccMax < SSS_thresh) {
        /*
         * Not a SSync --> abort the qualification process
         */
        m1st = (S16) - 1;
        // nmm_dbg_printf_off(("@@@ line %d : Failed on m1st searching. xaccMax = %d\r\n", __LINE__, xaccMax));
        return;

    }

    pz1m1st = d;                // re-use the array d[]
    lte_bs_phy_nmm_sss_gen_z1m1st(m1st, pz1m1st);

    /*
     * Divide d[2n+1]/(c1[n]*z1m1st) --> s1^(m1)[n]
     */
    for (k = 0; k < 31; k++) {
        d_odd[k*2] = d_odd[k*2] * c1[k] * pz1m1st[k];
        d_odd[k*2+1] = d_odd[k*2+1] * c1[k] * pz1m1st[k];
    }

    /*
     * Search for m_2nd
     */
    xaccMax = 0;
    m2nd = -1;
    for (j = 0; j < 31; j++) {
        xaccI = 0;
        xaccQ = 0;
        for (k = 0; k < 31; k++) {
            xaccI = xaccI + d_odd[k*2] * s0_ref[j + k];
            xaccQ = xaccQ + d_odd[k*2+1] * s0_ref[j + k];
        }
        xaccI >>= 5;
        xaccQ >>= 5;
        xaccPow = L_mult((S16) xaccI, (S16) xaccI);
        xaccPow += L_mult((S16) xaccQ, (S16) xaccQ);

        if (xaccMax < xaccPow) {
            xaccMax = xaccPow;
            m2nd = (S16) j;
        }
    }
    if (xaccMax < SSS_thresh) {
        /*
         * Not a SSync --> abort the qualification process
         */
        m2nd = (S16) - 1;
        // nmm_dbg_printf_off(("@@@ line %d : Failed on m2nd searching. xaccMax = %d\r\n", __LINE__, xaccMax));
        return;
    }

    if (m1st == m2nd) {
        /*
         * Why m1st == m2nd?
         */
        // nmm_dbg_printf_off(("@@@ line %d : Failed on m1st == m2nd\r\n", __LINE__));
        return;
    }

    if (m1st < m2nd) {
        *Subframe = 0;
        m0 = m1st;
        m1 = m2nd;
    }
    else {
        *Subframe = 5;
        m0 = m2nd;
        m1 = m1st;
    }

    /*
     * Compute NID_1 from m0 and m1
     */
    k = m1 - m0;
    NID_1 = (S16) ((k - 1) * 30 + m0);
    switch (k) {
    case 3:
        NID_1 -= 1;
        break;
    case 4:
        NID_1 -= 3;
        break;
    case 5:
        NID_1 -= 6;
        break;
    case 6:
        NID_1 -= 10;
        break;
    case 7:
        NID_1 -= 15;
        break;
    }

    if (NID_1 > PCI_SYNC_NB_GRP) {
        // nmm_dbg_printf_off(("@@@ line %d : Computed NID_1 (%d) > 167 ???.\r\n", __LINE__, k));
        return;
    }

    *NID_cell = 3 * NID_1 + NID_2;
    return;
}


//-------------------------------------------------------------------------------------------
/** @brief implement AGC function
 *
 *  <1> Hardware Resource:
 *  CEVA
 * 
 *  @param agc_gain   Pointer to agc shift factor
 *  @param agc_avg_nrg  Pointer to average data power after AGC
 *  @param dataPtr    Pointer to input/output data memory	
 *  @param len	  	  Number of input data length
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction      |Length   |Type           |Data Format|Valid Range |Units |
 *  |             |standard |               |         |               |           |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |agc_gain	  |         |pOutput	    | len     |pReal          |(1:31:0)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |agc_avg_nrg  |         |pOutput	    | len     |pReal          |(1:31:0)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |dataPtr	  |         |pInput/pOutput	| len     |pComplex Array |(1:0:15)   |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |len          |         |Input		    |		  |Real           |(1:15:0)   |Full Range  |	  |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |lte_bs_phy_nmm_ssync_agc		   |
 *  --------------------------------------------------------
 *  |Input Streams       |dataPtr						   |
 *  --------------------------------------------------------
 *  |Output Streams      |agc_gain,agc_avg_nrg,dataPtr	   |
 *  --------------------------------------------------------
 *  |Config Parameters   |len							   | 
 *  --------------------------------------------------------
 *  |Status Parameters   |none		                       |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * This function is to do AGC for downsampling data
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_nmm_cell_search	
 *
 **/
 void
lte_bs_phy_nmm_ssync_agc(S32 * agc_gain, S32 * agc_avg_nrg, S16 * dataPtr, S16 len)
{
    // LISTENING_CELL_SEARCH_DESC *this = (LISTENING_CELL_SEARCH_DESC *) cell_search;
    int             i = len;
    S32             AGC_Eav;
    U32             pwrLow, pwrHigh;
    S16             maxIn, shift;

    pwrLow = 0;
    pwrHigh = 0;
    maxIn = 0;
    for (i = 0; i < len; i++)
    {
        pwrLow += L_mult(dataPtr[i * 2]>>1, dataPtr[i * 2]);
        pwrLow += L_mult(dataPtr[i * 2 + 1]>>1, dataPtr[i * 2 + 1]);

        if (pwrLow >= 0x80000000)   // pwrLow use lower 31bits
        {
            pwrHigh++;
            pwrLow -= 0x80000000;
        }

        if (abs(dataPtr[i * 2]) > maxIn)
            maxIn = abs(dataPtr[i * 2]);

        if (abs(dataPtr[i * 2 + 1]) > maxIn)
            maxIn = abs(dataPtr[i * 2 + 1]);
    }

    if (pwrHigh)    // 1/(64*15), floor(1/(64*15)*2^15) = 34
    {
        pwrLow = (pwrLow >> 15) | (pwrHigh << 16);
        AGC_Eav = (S32) (pwrLow * 34);
    }
    else
        AGC_Eav = (S32) (((pwrLow >> 6) * 34) >> 9);

    shift = norm_l(maxIn) - 17;
    for (i = 0; i < len*2; i++)
    {
        dataPtr[i] = shl(dataPtr[i], shift);
    }

    if (AGC_Eav > 64)
        AGC_Eav = L_shl(AGC_Eav, shift*2);

    *agc_gain = shift;
    *agc_avg_nrg = AGC_Eav;
}


