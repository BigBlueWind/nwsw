

// -------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL lte_bs_phy_nmm_fdd_lib.h
 *
 * @brief LTE FDD listening library library
 * @author Mindspeed Technologies
 * @version $Revision: 1.1.4.1.2.18 $
 *
 * COPYRIGHT&copy; 2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
// -------------------------------------------------------------------------------------------


#ifndef LTE_BS_FDD_LISTENING_LIB
#define LTE_BS_FDD_LISTENING_LIB

#include "prototypes.h"

#if defined MX_ARM_ENABLED || defined EVM
#define NMM_CEVA_MM 0
#else

#ifdef SCHEDULER
#define NMM_CEVA_MM 1
#else
#define NMM_CEVA_MM 0
#endif
#endif

#define NMM_CEVA_DBG 0

/* FIXME (OS) - to be removed */
#if !defined(_WIN32 ) && !defined(USE_LINUX) && !defined(CEVA_INTRINSICS)
#define FEC_FDESC_ENABLED
#define MDMA_FDESC_ENABLED
#include "FECdrv.h"
#include "mdmadrv.h"
#endif


#define SIZEOF_IQ_PAIR                  (2*sizeof(S16))
#define PSS_IDX_TRACKING_SIZE           32

#define MAX_HBFIR_DECIMATION_STAGE      5
#define HBFIR_DECIMATION_LEN            63
#define FFT_SIZE_096MHZ                 64
#define SUBFR_SIZE_096MHZ               (FFT_SIZE_096MHZ*15)

#define DECIM_32_GRP_DELAY              961
#define DECIM_16_GRP_DELAY              465
#define DECIM_8_GRP_DELAY               217
#define DECIM_4_GRP_DELAY               93
#define DECIM_2_GRP_DELAY               31

#define ONE_DIV_BY_SIX                  5461
#define NB_CELL_SPECIFIC_REF_SIGNAL_GRP     6
#define REF_SIGNAL_PER_RB               2
#define REF_SIGNAL_PER_SLOT             2
#define PILOTS_SYM_PER_FRAME            (20 * REF_SIGNAL_PER_SLOT)      /* 20 slots x REF_SIGNAL_PER_RB */


#define REF_SIGNAL_SC_SPACING           6
#define MODULO6(x)                      ((x)-6*(((x)*5462)>>15))


#define PCI_SYNC_NB_GRP                 167

#define PCFICH_NUM_RE                   16
#define PCFICH_NUM_RG                   4

#define PCFI_SIZE_IN_BITS               (2*PCFICH_NUM_RE)


#define MAX_SYM_PER_SUBFR               14
#define MAX_SYMB_AND_CP_SIZE            (FFTSZ_2048 + FFTSZ_2048/8)

#define N_BW_CASE                       6
/*
 * SSS is 1 sym behind SSS + CP (max is long CP = 1/4 * sym size)
 */
#define SSS_SEARCH_RANGE                (FFT_SIZE_096MHZ + FFT_SIZE_096MHZ/4)
#define NMM_RSSI_RSRP_MAX               -16
#define NMM_RSSI_RSRP_MIN               -144
#define LONG_CP_BASE_SIZE               32
#define SHORT_CP0_BASE_SIZE             10
#define SHORT_CP_BASE_SIZE              9
#define PSS_SHORT_CP_096MHZ             SHORT_CP_BASE_SIZE      /* (4.5 IQ's) */
#define PSS_LONG_CP_096MHZ              LONG_CP_BASE_SIZE       /* (16 IQ's) */
#define SYM_PER_SUBFR_LONG_CP           12
#define SYM_PER_SUBFR_SHORT_CP          14


#define MAX_CP_SIZE                     512
#define MAX_DECIM_GRP_DELAY             DECIM_32_GRP_DELAY
#define LEN_EXTRA_CP                    40

#define SYMBOL(x)                       (x)

#define NMM_CELL_SEARCH_MAX_TIME_MS     500  /* 0.5 second */

#define SINGLE_NMM_CELL_SEARCH_IND_API_SIZE 4
#define MAX_CELL_FOUND_LIST             16

#define PBCH_SYM_START_SHORT_CP         7
#define PBCH_SYM_END_SHORT_CP           10

#define PBCH_SYM_START_LONG_CP          6
#define PBCH_SYM_END_LONG_CP            9

#define PBCH_SUBFRAME                   0
#define PBCH_SYM_PER_FRAME              4
#define PBCH_NB_FRAME                   4
#define PBCH_RE_PER_FRAME               240
#define PBCH_SIZE                       (PBCH_NB_FRAME*PBCH_RE_PER_FRAME)
#define PBCH_NB_RB                      6
#define PBCH_BW                         (PBCH_NB_RB * PRB_12)
#define PBCH_NUM_SYM                    4

#define PBCH_SIZE_IN_BITS               (PBCH_RE_PER_FRAME*8)

#define MIB_SIZE_IN_BITS                24
#define PBCH_NB_RB                      6

#define MAX_MIB_SUCCESS_DECODED         2
#define MAX_MIB_FAILED_DECODED          100     /* 1 sec */
#define MAX_SIB_FAILED_DECODED          50     /* 1 sec */
#define MAX_SIB_SUCCESS_DECODED         1

#define SUBFRAME_PER_RADIO              10
#define NMM_PBCH_DATA_IND_API_SIZE      9

#define MAX_BCCH_DATA_LIST              32
#define MAX_BCCH_SIZE                   2048
#define NMM_BCCH_DATA_IND_API_SIZE      ((MAX_BCCH_DATA_LIST * (MAX_BCCH_SIZE) + 5) + 1)

#define PDCCH_SYM_START                 0
#define PCFICH_SYM                      0
#define PHICH_SYM                       0
#define MAX_PHICH_GRP 					50
#define MAX_CFI 						3
#define PHICH_DUR_EXTENTED 				1
#define MAX_PDCCH_PER_FRAME 			4
#define PDCCH_MAX_SIZE_IN_BITS          (2 * 576)       /* FIXME (OS) - tmp dbg, to be investigated */

#define PDCCH_FORMAT0_NBITS 72
#define PDCCH_FORMAT1_NBITS 144
#define PDCCH_FORMAT2_NBITS 288
#define PDCCH_FORMAT3_NBITS 576

#define CRC16_NBYTES 2
#define CRC24A_NBYTES 3
#define CRC16_NBITS (CRC16_NBYTES << 3)
#define CRC24A_NBITS (CRC24A_NBYTES << 3)

#define BITS_PER_CCE 					72
#define MAX_N_DCI_BYTE                  4

#define MAX_PDSCH_PER_SUBFRAME 			13
#define P_GCRC24A						0xC3267D
#define P_GCRC24B						0xC00031

#define P_ENCOUT_SIZ					18528   // Define maximum length of decoder's soft decision input array.
#define P_MAX_SFDEC_LEN					18528   // Define maximum allowable rate matching redundancy factor.  This number
#define P_MAX_SFDEC_AVG					5       // maximum number of soft decisions that can be averaged
#define P_MAX_RATEM_LEN					(P_MAX_SFDEC_LEN*P_MAX_SFDEC_AVG)       // Define maximum length of rate matching output array

#define MAX_SC_OFDM_SYMB                1200
#define N_TS_SUBFRM                     2
#define MAX_N_DL_PRB                    100

enum MIB_DECODE_SUCCESS {
    MIB_OK = 0,
    MIB_FAILED = 1
};

typedef struct mib_desc {
    BIT_FIELD       spare:2;
    BIT_FIELD       system_frame_number:8;
    BIT_FIELD       phich_resource:2;
    BIT_FIELD       phich_duration:1;
    BIT_FIELD       dl_bandwith:3;
} MIBDESC;
#define MAX_N_SIB_TYPES 12
#define MAX_N_SIB_MAPPING_INFO_MSGS 32
typedef enum {
    sib_type2 = 0,              /* sib1 periodicity not stored, known */
    sib_type3,
    sib_type4,
    sib_type5,
    sib_type6,
    sib_type7,
    sib_type8,
    sib_type9,
    sib_type10,
    sib_type11,
    sib_type12_v920,
    sib_type13_v920
} sib_type_t;

typedef enum {
    rf8 = 0,
    rf16,
    rf32,
    rf64,
    rf128,
    rf256,
    si_unkown
} si_periodicity_t;
typedef struct {
    S16            *start;
    S16             len;
    S16             cp_size;
    S16             subframe;
    S16             symb_index;
    S16             fft_done;
} SYMBOL_INFO;


typedef struct {
    SYMBOL_INFO     symb_info[MAX_SYM_PER_SUBFR + 1];
    S16             cell_pci;
    S16             cell_cp;
    S16             cell_subframe;
    S32             cell_time_offset;
    S16             cell_current_sym_idx;
    U8              cell_current_sym_cnt;
	U16				cell_detect_num;
    S16             rsrp_measure_time;
    S32             avg_rsrp;
    S32             cell_freqency_offset;
    U16             pbch_re_per_frame;
    /*
     * 48*2 + 72 *2 = 240 RE
     */
    S16            *pbch_buf;
    S16             pbch_buf_idx;
    /*
     * FFT scale
     */
    S16            *pbch_scale;
    /*
     * PBCH sym tracker
     */
    S16             pbch_sym_idx;
    U32             cinit_PBCH;
    // S8 pbch_scrambling_seq[PBCH_SIZE_IN_BITS];
    U8             *pbch_scrambling_seq;

    /*
     * MIB content
     */
    U16             sfn;
    U8              phich_duration;
    U8              phich_resource;
    U8              bw_index;
    U8              dl_tx_bw;
    U8              SI_DCI_size[2]; //0 for 1c; 1 for 1a;
    U8              cell_antennas;
    U8              mib_decoded_success_cnt;
    U8              mib_decoded_failed_cnt;

    /*
     * CFI
     */
    U8              cfi_decoded;
    U8              cfi_val;
    /*
     * CFI RE index
     */
    S16             pcfi_re_idx[PCFICH_NUM_RE];
    /*
     * CFI RE group
     */
    S16             pcfi_rg_idx[PCFICH_NUM_RG];

    /*
     * HI
     */
    S16             phich_re_idx[MAX_PHICH_GRP * QUADRUPLET];
    S16             phich_sym_idx[MAX_PHICH_GRP * QUADRUPLET];
    S16             num_phich_re;

    /*
     * PDCCH
     */
    S16            *pdcch_buf[MAX_PDCCH_PER_FRAME];
    U16            *pdcch_index[MAX_PDCCH_PER_FRAME];
    S16             pdcch_buf_idx;
    S16            *pdcch_scale[MAX_PDCCH_PER_FRAME];
    S16             pdcch_re_num[MAX_PDCCH_PER_FRAME];
    U8             *pdcch_scrambling_seq;
    U8              pdcch_decoded;

    /*
     * SIB1 Content
     */
    U8              cellBarred;
    U32             cellIdentity;
    U8              cellReservedForOperatorUse;
    U8              csgIdentity;
    U8              csgIndication;
    U8              freqBandIndicator;
    U8              imsEmergencySupport;
    U8              intraFreqReselection;
    U32             plmnIdentityList;
    U8              pMax;
    U8              qQualMin;
    U8              qQualMinOffset;
    U8              qRxLevMinOffset;
    U8              sibMappingInfo;
    U8              siPeriodicity;
    U8              siWindowLength;
    U8              systemInfoValueTag;
    U32             trackingAreaCode;
    U8              si_periodicity[MAX_N_SIB_TYPES];    /* cast si_periodicity_t to U8 because of enum size mismatch between arm & ceva */

    U8              pdcch_extracted_cnt;

    U8              sib_decoded_success_cnt;
    U8              sib_decoded_failed_cnt;

    U8              dci_data[MAX_N_DCI_BYTE];

    /* dci message parameters */
    U8              dci_type_1a;        // 1: 1A, 0: 1C
    U8              dci_decoded_success;
    /* common to 1A / 1C */
    U16             rb_allocation;
    U8              tpc_command;
    U8              redundancy;
    U8              modulation_coding;
    U8              localized_distributed;
    U8              format_flag;
    U8              gap_value;
    S16             transport_block_size;
    U8              num_resource_blocs;
    U8              PDSCH_PRB_set[MAX_N_DL_PRB*N_TS_SUBFRM];

    U8              BCCH_type; // 0: SIB 1; 1: other SIBs
    U16             SI_subfrm_counter; // in a window !!! DCI_1C_TBD

    /* used for PDSCH extracting */
    S16            *pdsch_buf;
    U16             N_pdsch_re_symb[MAX_PDSCH_PER_SUBFRAME];
    U32             inv_noise_var; // low for inv; high for norm_l

} CELL_PARAMS;


typedef struct {
    CELL_PARAMS     cell_param[MAX_CELL_PER_CARRIER];

    U32             cell_search_pss_cand_tracking[PSS_IDX_TRACKING_SIZE];
    // S16 hbfir_state_buf[(HBFIR_DECIMATION_LEN * MAX_HBFIR_DECIMATION_STAGE * 2)];
    S16            *hbfir_state_buf;

    /*
     * storage for SSS, long CP = 1/4 x 64 , PSS and 64 additional points
     */
    // S16 cell_search_xcorr_buf[2 * (2 * FFT_SIZE_096MHZ + SSS_SEARCH_RANGE)];
    S16            *cell_search_xcorr_buf;

    /*
     * storage for sym (Max CP siz + max decimation filter grp delay
     */
    S16             cell_search_sym_bound_buf[2 * (MAX_CP_SIZE + MAX_DECIM_GRP_DELAY + LEN_EXTRA_CP)];
    /*
     * keep track of decimated sample count
     * U32 will wrap in 1h14min after cell search start
     */
    U32             cell_search_sample_count;
    U8              cell_search_pss_cand_tracking_idx;

    /*
     * downsampler
     */
    S16            *hbfir_real_buf_stage0;
    S16            *hbfir_imag_buf_stage0;
    S16            *hbfir_real_buf_stage1;
    S16            *hbfir_imag_buf_stage1;
    S16            *hbfir_real_buf_stage2;
    S16            *hbfir_imag_buf_stage2;
    S16            *hbfir_real_buf_stage3;
    S16            *hbfir_imag_buf_stage3;
    S16            *hbfir_real_buf_stage4;
    S16            *hbfir_imag_buf_stage4;
    /*
     * AGC
     */
    S32            *agc_gain;
    S32            *agc_avg_nrg;

    /*
     * PSS XCor
     */
    S16            *xc25,
                   *xc29,
                   *xc34;
    S16            *xcorr_arr;
    S16            *psync_found_arr;
	S16				xcorr_idx;
    /*
     * detected cell characteristics
     */
    U16             nb_detected_cell;
    U16             nb_reported_cell;
    U16             nb_detected_requested_cell;

    U16             earfcn;

} LISTENING_CELL_SEARCH_DESC;
extern U8       MIB_BW_TO_RB[6];

#ifndef CEVA_INTRINSICS
/*
 * Prototypes
 */
int             lte_bs_phy_nmm_downsampling(S16 *, S32, S16 *, S32, S16 *, S16 *, S16 *, S16 *, S16 *, S16 *, S16 *, S16 *, S16 *, S16 *)CEVAEXTERN;
void            lte_bs_phy_nmm_downsampling_init(void *)CEVAEXTERN;
void            lte_bs_phy_nmm_pss_xcorr(S16 *, S16 *, S16 *, S16)CEVAEXTERN;
void            cfft_16t32d_case2(short *, short, short *, short *)CEVAEXTERN;
void            lte_bs_phy_nmm_ssync_est_channel_estimate(S16 *, S16 *, S16 *, S16 *, S16, S16)CEVAEXTERN;
void            lte_bs_phy_nmm_ssync_decode_sss(S16 *, S16 *, S16, S16 *, S16 *)CEVAEXTERN;
void            lte_bs_phy_nmm_extract_dlpilots(S16, S16 *, S16, S16 *, S16, S16, S16, S16 *, S16 *)CEVAEXTERN;
void            lte_bs_phy_nmm_get_pcfi_idx(CELL_PARAMS *)CEVAEXTERN;
void            lte_bs_phy_nmm_decode_pcfi(CELL_PARAMS *, S16 *, S16, S16)CEVAEXTERN;
S16             lte_bs_phy_nmm_get_phich_idx(CELL_PARAMS *)CEVAEXTERN;
S16             lte_bs_phy_nmm_get_pdcch_idx(CELL_PARAMS *, S16, S16 *)CEVAEXTERN;
void            lte_bs_phy_nmm_decode_pdcch(CELL_PARAMS *, S16)CEVAEXTERN;
void            lte_bs_phy_nmm_decode_dci(CELL_PARAMS *)CEVAEXTERN;
void            lte_bs_phy_nmm_decode_pdsch(CELL_PARAMS *, S16, S16, S16, SYMBOL_INFO *, S16, U8 *,U8*)CEVAEXTERN;
void            lte_bs_phy_nmm_reshuffle_pdcch_bits(S16,  S16 *, S16, S16)CEVAEXTERN;
void            lte_bs_phy_nmm_pdcch_chan_decoding(S8 *, U8 *, S32, S32, S16, S16, U8 *)CEVAEXTERN;
//void            lte_bs_phy_nmm_sib1_decode(CELL_PARAMS *, U8 *)CEVAEXTERN;
void            lte_bs_phy_nmm_demod_qpsk(S32 *, S8 *, S16, S32)CEVAEXTERN;
// void lte_bs_phy_nmm_pdcch_demod(S32 *, S16, S8 *, S16, U32, U8 *)CEVAEXTERN;
void            lte_bs_phy_nmm_pdcch_demod(S16 *, S16, S16, S8 *, U32, U8 *)CEVAEXTERN;

/*
 * TCB Wrappers
 */
PTCB            lte_bs_phy_nmm_downsampling_tcb(PTASKLIST, S16 *, S32, S16 *, S32, S16 *, S16 *, S16 *, S16 *, S16 *, S16 *, S16 *, S16 *, S16 *, S16 *, U16, PTCB)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_agc_tcb(PTASKLIST, S32 *, S32 *, S16 *, S16, PTCB *, U16, PTCB)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_xcorr_loop_tcb(PTASKLIST, LISTENING_CELL_SEARCH_DESC *, S16 *, S16 *, S16 *, PTCB *, U16, PTCB)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_pss_candidate_selection_loop_tcb(PTASKLIST, S16 *, S16 *, PTCB *, U16, PTCB)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_fft64_tcb(PTASKLIST pTskLst, S16 * pin, S16 * freq, S16 * exp, PTCB refill_tcb)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_ssync_chanest_tcb(PTASKLIST, S16 *, S16 *, S16 *, S16)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_chanest_tcb(PTASKLIST, S16 *, S16 *, S16 *, S16, S16, PTCB *, S16, PTCB refill_tcb)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_sync_decode_sss_tcb(PTASKLIST, S16 *, S16, S16 *, S16 *, PTCB *, S16, PTCB refill_tcb)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_get_rsrp_tcb(PTASKLIST, S16, SYMBOL_INFO *, S16, S16, S16, S32 *, S16, S16, S16, PTCB refill_tcb)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_pseudo_random_seq_gen_tcb(PTASKLIST, S32, U32, U8 *,PTCB refill_tcb)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_extract_pbch_tcb(PTASKLIST, S16, SYMBOL_INFO *, S16, S16, S16 *, S16 *, S16 *,U16,U32,PTCB refill_tcb)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_demod_pbch_tcb(PTASKLIST,S16 *, S16, S16 *, S8 *,PTCB refill_tcb)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_extract_pcfi_tcb(PTASKLIST, S16, S16, SYMBOL_INFO *, S16 *, S16 *, S16 *, S16 *,U16,U32,PTCB refill_tcb)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_extract_pdcch_tcb(PTASKLIST, S16, S16, SYMBOL_INFO *, S16 *, S16, S16 *, S16 *, S16 *,U16,U32,PTCB refill_tcb)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_pdcch_demod_tcb(PTASKLIST, S16 *, S16, S16, S8 *, S16, U32, U8 *,PTCB refill_tcb)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_extract_pdsch_tcb(PTASKLIST, S16, S16, SYMBOL_INFO *, S16, S16 *, S16 *, S16 *,U16,U8*,U16,U16,U16,U16*,U32,PTCB refill_tcb,U16)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_channel_estimate_tcb(PTASKLIST , S16 *, S16 , S16 , S16 , S16 , S16 , S16 *,U16,U32*, S32 *,PTCB refill_tcb)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_pss_foe_tcb(PTASKLIST , S16 *, S16 , S32 *, PTCB)CEVAEXTERN;
PTCB            lte_bs_phy_nmm_freqOffset_comp_tcb(PTASKLIST pTskLst, S16 *pin, S32 foeEst, S16 symbolPos, S16 length, PTCB)CEVAEXTERN;

#else

extern U8      *DataBuff_Bank1;
extern U8      *DataBuff_Bank2;

void            lte_bs_phy_nmm_tcb_parser(U32, PTCB) CEVAEXTERN;

void            lte_bs_phy_nmm_downsampling_total_ceva(S16 *, S32, S16 *) CEVAEXTERN;
int             lte_bs_phy_nmm_downsampling_ceva(S16 *, S32, S16 *, S16 *, S16 *, S16 *, S16 *, S16 *) CEVAEXTERN;
void            lte_bs_phy_nmm_hbfir_state_upd_x1641(S16, S16, S16 *, S16 *, S16) CEVAEXTERN;
void            lte_bs_phy_nmm_hbfir_total_x1641(S16 *, S16 *, U32) CEVAEXTERN;
void            lte_bs_phy_nmm_ssync_agc_x1641(S32 *, S16 *, S16 *, S16 *, S16 *, S16 *, S16) CEVAEXTERN;
void            lte_bs_phy_nmm_pss_xcorr_x1641(S16 *, S16 *, S16 *, S16) CEVAEXTERN;
void            cfft_16t32d(short *, short, short *, short *) CEVAEXTERN;
void            lte_bs_phy_nmm_ssync_est_channel_estimate_i1641(S16 *, S16 *, S16 *, S16 *, S16 *, S16) CEVAEXTERN;
void            lte_bs_phy_nmm_ssync_decode_sss_ceva(S16 *, S16, S16 *, S16 *) CEVAEXTERN;
void            lte_bs_phy_nmm_ssync_decode_sss(S16 *, S16 *, S16, S16 *, S16 *)CEVAEXTERN;
void            lte_bs_phy_nmm_ssync_decode_sss_part1_x1641(S16 *, S16, S16 *) CEVAEXTERN;
void            lte_bs_phy_nmm_ssync_decode_sss_part2_i1641(S16 *, S16 *, S16 *) CEVAEXTERN;
void            lte_bs_phy_nmm_ssync_decode_sss_part3_x1641(S16 *, S16, S16, S16 *) CEVAEXTERN;
void            lte_bs_phy_nmm_ssync_decode_sss_part4_i1641( S16 *,  S16 *, S16 *) CEVAEXTERN;
void            lte_bs_phy_swap_fftout_dcskip_ceva(S16 *, S16 *, S32, S32, S16 *) CEVAEXTERN;
void            lte_bs_phy_nmm_extract_dlpilots_ceva(S16, S16 *, S16, S16 *, S16, S16, S16, S16 *, S16 *) CEVAEXTERN;

#endif
/*
 * Prototypes
 */
void            lte_bs_phy_nmm_ssync_agc(S32 *, S32 *, S16 *, S16) CEVAEXTERN;
void            lte_bs_phy_nmm_xcorr_loop(S16 *, S16 *, S16 *, S16 *, S32 *) CEVAEXTERN;
void            lte_bs_phy_nmm_pss_candidate_selection_loop(S16 *, S16 *) CEVAEXTERN;
void            lte_bs_phy_nmm_sss_gen_c0c1(S16, S16 *, S16 *) CEVAEXTERN;
void            lte_bs_phy_nmm_sss_gen_z1m1st(S16, S16 *) CEVAEXTERN;
void            lte_bs_phy_nmm_get_rsrp(S16, S16 *, S16, S16, S16, S16, S32*, S16, S16, S16) CEVAEXTERN;
void            lte_bs_phy_nmm_ssync_loop(LISTENING_CELL_SEARCH_DESC *, S16 *, S16 *) CEVAEXTERN;
void            PseudoRandomSeqGen(S32, U32, U8 *) CEVAEXTERN;
void            lte_bs_phy_nmm_demod_qpsk(S32*, S8 *, S16, S32) CEVAEXTERN;

void            lte_bs_phy_nmm_channel_estimate(S16 *, S16 , S16 , S16 , S16 , S16 , S16 *,U16,U32*, S32 *)CEVAEXTERN;

void            lte_bs_phy_nmm_extract_pbch(S16, S16 *, S16, S16, S16, S16 *, S16 *, S16 *,U16,U32)CEVAEXTERN;
void            lte_bs_phy_nmm_demod_pbch(S16 *, S16, S16 *, S8 *) CEVAEXTERN;

void            lte_bs_phy_nmm_extract_pcfi(S16, S16 *, S16, S16 *, S16 *, S16 *, S16 *,U16,U32)CEVAEXTERN;
void            lte_bs_phy_nmm_extract_pdcch(S16, S16 *, S16, S16 *, S16, S16 *, S16 *, S16 *,U16,U32)CEVAEXTERN;
void            lte_bs_phy_nmm_extract_pdsch(S16, S16 *, S16, S16, S16 *, S16 *, S16 *,U16,U8*,U16,U16,U16,U16*,U32,U16)CEVAEXTERN;

void            lte_bs_phy_nmm_pdcch_demod(S16 *, S16, S16, S8 *, U32, U8 *) CEVAEXTERN;
void            lte_bs_phy_nmm_subblock_deinterleaver_pdcch(S16*, S16, S16, S16*) CEVAEXTERN;
void            lte_bs_phy_nmm_reshuffle_pdcch_bits(S16, S16 *, S16, S16) CEVAEXTERN;
void            lte_bs_phy_nmm_channelEst_comp(S16 *, S16 *, S16 ,U16,U16,U16*,U32,U16) CEVAEXTERN;
void            lte_bs_phy_nmm_pss_foe(S16 *, S16 *, S32 *) CEVAEXTERN;
void            lte_bs_phy_nmm_freqOffset_comp(S16 *, S32 , S16 , S16) CEVAEXTERN;

/*
 * TCB Tasks
 */
void            lte_bs_phy_nmm_downsampling_task(PTCB) CEVAEXTERN;
void            lte_bs_phy_nmm_agc_task(PTCB) CEVAEXTERN;
void            lte_bs_phy_nmm_xcorr_loop_task(PTCB) CEVAEXTERN;
void            lte_bs_phy_nmm_pss_candidate_selection_loop_task(PTCB) CEVAEXTERN;
void            lte_bs_phy_nmm_fft64_task(PTCB) CEVAEXTERN;
void            lte_bs_phy_nmm_chanest_task(PTCB) CEVAEXTERN;
void            lte_bs_phy_nmm_decode_sss_task(PTCB) CEVAEXTERN;
void            lte_bs_phy_nmm_get_rsrp_task(PTCB tcb) CEVAEXTERN;
void            lte_bs_phy_nmm_pseudo_random_seq_gen_task(PTCB) CEVAEXTERN;
void            lte_bs_phy_nmm_extract_pbch_task(PTCB) CEVAEXTERN;
void            lte_bs_phy_nmm_demod_pbch_task(PTCB) CEVAEXTERN;
void            lte_bs_phy_nmm_extract_pcfi_task(PTCB tcb) CEVAEXTERN;
void            lte_bs_phy_nmm_extract_pdcch_task(PTCB tcb) CEVAEXTERN;
void            lte_bs_phy_nmm_pdcch_demod_task(PTCB tcb) CEVAEXTERN;
void            lte_bs_phy_nmm_extract_pdsch_task(PTCB tcb) CEVAEXTERN;
void            lte_bs_phy_nmm_channel_estimate_task(PTCB tcb) CEVAEXTERN;
void            lte_bs_phy_nmm_pss_foe_task(PTCB tcb) CEVAEXTERN;
void			lte_bs_phy_nmm_freqOffset_comp_task(PTCB tcb) CEVAEXTERN;
#endif
