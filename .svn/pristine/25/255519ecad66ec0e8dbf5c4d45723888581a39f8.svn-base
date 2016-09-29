

// -------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL lte_bs_phy_nmm_fdd.h
 *
 * @brief LTE FDD listening
 * @author Mindspeed Technologies
 * @version $Revision: 1.1.4.1.2.24 $
 *
 * COPYRIGHT&copy; 2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
// -------------------------------------------------------------------------------------------

#ifndef LTE_BS_FDD_LISTENING
#define LTE_BS_FDD_LISTENING

#include "ltephy.h"
#include "lte_bs_phy_nmm_radio_itf.h"
#include "lte_bs_phy_nmm_fdd_lib.h"


#ifndef WIN32
#define NMM_BET 0
#endif
#define NMM_DBG_PRINT 1

#define NMM_USE_TCB_LIST 1

#define NMM_HEAP_LEAKAGE_CONTROL 0

extern U32      nmm_err_code;

/* error codes */
#define ERR_NMM_PSEUDO_RANDOM_SEQ   (1 << 0)
#define ERR_NMM_SSYNC_CHANEST       (1 << 1)
#define ERR_NMM_SSYNC_CHANEST_HSS   (1 << 2)
#define ERR_NMM_DECODE_SSS          (1 << 3)
#define ERR_NMM_FFT64               (1 << 4)
#define ERR_NMM_XCORR               (1 << 5)
#define ERR_NMM_AGC                 (1 << 6)
#define ERR_NMM_DOWNSAMPLING        (1 << 7)

#ifndef MIN
#define MIN(x,y)                        ((x)<(y))?(x):(y)
#endif

#ifdef WIN32
#ifndef MxGetCpuID
#define MxGetCpuID() 0
#endif

#endif
#define MAX_N_TX_ANTE                   4

#define MAX_NUM_CAND 10
#define MAX_NUM_CELLRS_SUBFRM 4
#define NUM_SYMB_PBCH       4
#define MAX_NUM_PDSCH_SYMB  13

enum nmm_tcb_id {
    NMM_DOWNSAMPLING_TCB = 0,
    NMM_AGC_TCB,
    NMM_PSS_XCORR0_TCB,
    NMM_PSS_XCORR1_TCB,
    NMM_PSS_XCORR2_TCB,
    NMM_FIND_CAND_TCB,
    NMM_SYNC_FFT_SCP0_TCB
};
enum nmm_tcb_id_4{
    NMM_EXTRACT_PCFI_TCB = 0,
    NMM_EXTRACT_PDCCH_TCB,
    NMM_PDCCH_DEMOD_TCB,
    NMM_EXTRACT_PDSCH_TCB
};
enum NMM_STATE {
    NMM_ABORT = 0x00,
    NMM_IDLE,
    NMM_RSSI_MEASURING,
    NMM_RSSI_MEASURING_DONE,
    NMM_SYNC_SEARCHING,
    NMM_SYNC_SEARCHING_DONE,
    NMM_PBCH_SEARCHING,
    NMM_PBCH_SEARCHING_DONE,
    NMM_PBCH_SYNC_SEARCHING,
    NMM_SIB1_SEARCHING,
    NMM_SIB1_SEARCHING_DONE,
    NMM_FULL_BCCH_SEARCHING,
    NMM_FULL_BCCH_SEARCHING_DONE,
    NMM_TRACKING
};
enum CELL_CP {
    SHORT_CP = 0,
    LONG_CP = 1
};


extern volatile U32 nmm_iq_subframe_copy_done;
typedef struct {
    /*
     * always keep last symbol as sampling as we are not aligned during listening
     */
    S16             last_IQ_sym[2 * MAX_SYMB_AND_CP_SIZE];

    /*
     * API indication storage
     */
    U32             api_output_msg[32];

    /*
     * The list of carriers to be measured (rssi).
     */
    U16             rssi_earfcn_list[NMM_LTE_MAX_CARRIER_LIST];
    /*
     * The list of cells which should be searched (pci 0..503)
     */
    U16             cell_search_pci_list[MAX_PCI_LIST];
    /*
     * The SI periodicity of all requested SIs (80, 160, 320, 640, 1280, 2560, 5120)
     */
    U16             si_periodicity_list[MAX_SI_LIST]; // in ms
    U16             si_index_list[MAX_SI_LIST]; //DCI_1C_TBD; starts from 0

    U8              supported_radio_band_list[MAX_EUTRA_BAND];

    U8              nb_supported_radio_band;
    U8              state;
    U8              received_nmm_stop_flag;
    U16             target_state_mask;

    U8              listening_active;
    /*
     * Duplex 0 FDD, 1 TDD
     */
    U8              duplex;
    /*
     * The E-UTRA band for which the carrierList applies (1-64)
     */
    U8              freq_band_indicator;
    /*
     * The length of time the NMM measure RSSI over. In 1ms radio frames (1-255)
     */
    U16             rssi_measurement_period_ms;
    /*
     * The number of carriers which should be measured. In case no carrier (value 0)
     * is specified all carriers in the band shall be measured (0- MAX_CARRIER_LIST)
     */
    U16             nr_earfcnList;
    U8              cur_earfcn;
    U8              cur_rssi_measurement_time;
    U16             rssi_meas_avg_factor;
    U32             averaged_rssi;
    /*
     * The earfcn for which cells should be measured (1..65535)
     */
    U16             cell_search_earfcn;
    /*
     * NMM should try to find all cells on the carrier (TRUE, FALSE)
     */
    U8              cell_search_exhaustive_search;
    /*
     * The number of cells in the pciList. If 0 all cells on the carrier should be found.
     * Otherwise, depending on exhaustiveSearch flag, only the given pciList is searched
     * or the pciList is used for indicating a priority list
     * (0.. MAX_PCI_LIST)
     */
    U8              cell_search_nr_pci_list;
    /*
     * The number of resource blocks which should be used for measuring RSRP (6,15,25,50,75,100)
     */
    U8              cell_search_meas_bandwidth;
    /*
     * The length of time the NMM measure (1..255 radio frame)
     */
    U8              cell_search_rssi_measurement_period;

    /*
     * The earfcn & PCI for which PBCH be measured (1..65535)
     */
    U16             pbch_earfcn;
    U16             pbch_pci;
    U8              sib1_retry_count;
    /*
     * The number of SIs to read. (0..32)
     */
    U8              nr_si_periodicity_list;
	U8				sib_report_cnt;
    U16             sib_report_flag;
	S32				sib_end_time[MAX_N_SIB_TYPES];
    /*
     * The SI window (1,2,5,10,15,20,40)
     */
    U8              si_window;
    /*
     * The number of siWindows for which decoding of a SI should be retried. The retiries are counted per siPeriodicity. (1-8)
     */
    U8              si_retry_count;

    U8              search_timing_for_all_cells;

    S16             cell_search_meas_bandwidth_inv;
    S16             rsrp_scale;
    /*
     * flag that determines if NMM_CELL_SEARCH_IND should be reported
     * either individualy or batched when NMM_STOP_CELL_SEARCH_REQ is issued
     */
    U8              report_individual_cell_params;
    U8              do_parrallel_listening;
    S16             cell_search_time;

    LISTENING_CELL_SEARCH_DESC *pcell_search;
    S32             sniffing_time;

    S16            *IQdecimated;
    S32             last_buf_subframe_count;

	S16				dlrs_foe_cellIdx;
	S16				dlrs_foe_cnt;
	U16             di_mode;

    U16             pbch_search_cell_index;
    STARTREQ phystart_req;
    U8              dump_iq_data_mode; //0: no dump; 1: dump for cell search; 2: dump for BCCH
    U8              dump_state; //0: not dumping; 1: dumping; 2: dump done
    U32             N_buf_subframe;
    U32             N_dump_subframe;

#if !defined WIN32 && !defined CEVA_INTRINSICS
    MDMA_FDESC     *pmdma;
#endif
} LISTENINGDESC, *PLISTENINGDESC;
extern S16 *nmm_iq_storage;


PLISTENINGDESC LteBsGetNmmCtx(void) CEVAEXTERN;
U32             lte_bs_phy_nmm_parse_msg(U32, U32 *, U32, U32 *, U32 *) CEVAEXTERN;
U32             lte_bs_phy_nmm_start(U32, void *, U32) CEVAEXTERN;
U32             lte_bs_phy_nmm_stop(U32, void *, U32) CEVAEXTERN;
U32             lte_bs_phy_nmm_rssi_meas_req(U32, void *, U32) CEVAEXTERN;
U32             lte_bs_phy_nmm_stop_rssi_meas_req(U32, void *, U32) CEVAEXTERN;
U32             lte_bs_phy_nmm_cell_search_req(U32, void *, U32) CEVAEXTERN;
U32             lte_bs_phy_nmm_stop_cell_search_req(U32, void *, U32) CEVAEXTERN;
U32             lte_bs_phy_nmm_pbch_config_req(U32, void *, U32) CEVAEXTERN;
U32             lte_bs_phy_nmm_stop_pbch_req(U32, void *, U32) CEVAEXTERN;
U32             lte_bs_phy_nmm_sib1_config_req(U32, void *, U32) CEVAEXTERN;
U32             lte_bs_phy_nmm_bcch_config_req(U32, void *, U32) CEVAEXTERN;
U32				lte_bs_phy_nmm_stop_bcch_req(U32 , void *, U32) CEVAEXTERN;
void            lte_bs_phy_nmm_main(U32, S16 *, U32* outMsg, U32* outMsgSize) CEVAEXTERN;
U8              lte_bs_phy_nmm_decode_pbch(CELL_PARAMS * current_cell, S16 *) CEVAEXTERN;
void            lte_bs_phy_nmm_channel_estimate_loop(PTASKLIST , CELL_PARAMS *, S16 , S16 , S16 *) CEVAEXTERN;
void            lte_bs_phy_nmm_cell_search_clear(void) CEVAEXTERN;
void            lte_bs_phy_nmm_generate_api_msg(U32 phyInstance, U8 * inMsg, U32 inMsgSize, U8 * outMsg, U32 msgType, U32 status) CEVAEXTERN;
U32             lte_bs_phy_nmm_destroy_buffers(void) CEVAEXTERN;
#define MAX_IQ_PER_MS          (30720*2)
#define MAX_ANTENNA_FOR_NMM 1   // 2
#define MAX_CELL_FOR_RSRP_SCAN  1       // MAX_CELL_PER_CARRIER
#define MAX_CELL_FOR_PBCH_EXTRACT  1

#define NMM_IQ_STORAGE_SIZE    ((MAX_IQ_PER_MS * MAX_ANTENNA_FOR_NMM * NMM_CELL_SEARCH_MAX_TIME_MS))

#define MAX_NMM_SUBFRAME_COUNT  10240

#ifndef CEVA_INTRINSICS
#define DUMP_ARR_ID_F(arr,size,format,on_off,ID, file) \
                if(on_off){ int ii,aa; \
                                        aa = 0;\
                fprintf(file, " %s [DEAD%04x] : ",#arr,ID); \
                for(ii=0;ii<size;ii++) {\
                       fprintf(file, "%"format" ", arr[ii]);\
                        aa+=arr[ii];\
                } \
                fprintf(file, " (%d)\n",aa); \
                }

#if NMM_DBG_PRINT == 1
#ifdef __CC_ARM
#define nmm_dbg_printf(x) printf x
#define nmm_dbg_printf_off(x)
#else
#define nmm_dbg_printf(x) printf x
#define nmm_dbg_printf_off(x)
#endif

#define DUMP_ARR_ID(arr,size,format,on_off,ID) \
                if(on_off){ int ii,aa; \
                                        aa = 0;\
                printf(" %s [DEAD%04x] : ",#arr,ID); \
                for(ii=0;ii<size;ii++) {\
                        printf("%"format" ", arr[ii]);\
                        aa+=arr[ii];\
                } \
                printf(" (%d)\n",aa); \
                }

#define DUMP_ARR_ID_UART(arr,size,format,on_off,ID) \
                if(on_off){ int ii,aa; \
                                        aa = 0;\
                phy_printf(" %s [DEAD%04x] : ",#arr,ID); \
                for(ii=0;ii<size;ii++) {\
                        phy_printf("%"format" ", arr[ii]);\
                        aa+=arr[ii];\
                } \
                phy_printf(" (%d)\n",aa); \
                }
#else
#define nmm_dbg_printf(x)
#define nmm_dbg_printf_off(x)
#define DUMP_ARR_ID(arr,size,format,on_off,ID)
#define DUMP_ARR_ID_UART(arr,size,format,on_off,ID)
#endif
#endif

#endif

