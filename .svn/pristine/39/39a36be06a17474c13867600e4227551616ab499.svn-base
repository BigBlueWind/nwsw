/** @file lte_mac_state.h
 *
 * @brief Defenitions and prototypes for updating mac-state into work-cycle
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef LTE_MAC_STATE_H
#define LTE_MAC_STATE_H

#define MAX_HARQ_PROCESS            8
//#define MAX_SDU_SIZE_BYTES          (75376>>3)
#define MAX_SDU_SIZE_BYTES          (36000>>3)
#define MAX_HARQ_PDSCH_CHANNELS     (MAX_PDSCH_CHANNELS>>2)
#define MAX_HARQ_CW                 2

typedef enum 
{
    NACK_HARQ_REPEAT = 0,
    NACK_HARQ_OK = 1
}NACK_HARQ;


//---------------------------------------------------------------------------------------------------
typedef struct 
{
    U8                         nack;
    U8                         rv;
    U8                         need_resend;
    U8                         ndi;
    U32                        tti_tick;
} LTE_PDSCH_SDU_CTRL_RESEND, *PLTE_PDSCH_SDU_CTRL_RESEND;

typedef struct
{
    U8                          payload[MAX_SDU_SIZE_BYTES];
    U32                         len;
} LTE_PDSCH_SDU_PAYLOAD, *PLTE_PDSCH_SDU_PAYLOAD;

//---------------------------------------------------------------------------------------------------
typedef struct
{

    U8                      numCodeWords;
    DLSUBCHINFO             sub_ch_info[2];
    CRCINFO                 crc_info;
    SCRAMBLERINFO           scr_info;
    DLDEDICATEDPRECODERINFO dl_precoder_info;

// extra parameter for MAC
    U8                      numCodeWords_updating;
    DLSUBCHINFO             sub_ch_info_updating[2];
    DLDEDICATEDPRECODERINFO dl_precoder_info_updating;  // new parameter for change cqi/pmi/ri (the changes must copy to dl_precoder_info from "lte_pdsch_frame_num" value)
    U32                     lte_pdsch_frame_num;        // 

    U8                      ena_update_cqi_pmi;
    U8                      ena_update_ri;

} LTE_PDSCH_CHAN_DESC, *PLTE_PDSCH_CHAN_DESC;

//---------------------------------------------------------------------------------------------------
typedef struct
{
    LTE_PDSCH_SDU_CTRL_RESEND   pdsch_sdu_ctrl_resend[MAX_HARQ_CW][MAX_HARQ_PDSCH_CHANNELS][MAX_HARQ_PROCESS];
    LTE_PDSCH_SDU_PAYLOAD       pdsch_sdu_payload[MAX_HARQ_CW][MAX_HARQ_PDSCH_CHANNELS][MAX_HARQ_PROCESS];

    U32                         harq_count_cnt;

    U8                          ena_harq;
    U8                          ena_udp_stream;

} LTE_PDSCH_HARQ_STATE, *PLTE_PDSCH_HARQ_STATE;

typedef struct
{
    LTE_PDSCH_CHAN_DESC     lte_pdsch_chan_desc[MAX_PDSCH_CHANNELS];
    U8                      ena_dynamic_cqi_pmi_ri;
    U8                      sched_num_sfn_ri;
    U8                      sched_num_sfn_cqi;
} LTE_PDSCH_CHAN_STATE, *PLTE_PDSCH_CHAN_STATE;


typedef struct
{
    U32                     sCCE[MAX_PDSCH_CHANNELS][10];
    U32                     cce_idx[MAX_PDSCH_CHANNELS];
    U32                     cce_idx_prev[MAX_PDSCH_CHANNELS];
} LTE_PDCCH_CHAN_STATE, *PLTE_PDCCH_CHAN_STATE;


//---------------------------------------------------------------------------------------------------
typedef struct
{
    LTE_PDSCH_HARQ_STATE          lte_pdsch_harq_state;
    LTE_PDSCH_CHAN_STATE          lte_pdsch_chan_state;
    LTE_PDCCH_CHAN_STATE          lte_pdcch_chan_state;

    U8                            tx_sub_frame_num;
    U8                            rx_sub_frame_num;

    U32                           tx_frame_num;
    U32                           rx_frame_num;
} LTE_MAC_STATE, *PLTE_MAC_STATE;


PLTE_MAC_STATE             get_lte_mac_state(void);
PLTE_PDSCH_HARQ_STATE      get_lte_pdsch_harq_state(void);

PLTE_PDSCH_CHAN_STATE      get_lte_pdsch_chan_state(void);
PLTE_PDSCH_CHAN_DESC       get_lte_pdsch_chan_desc(U32 chan_id);

PLTE_PDSCH_SDU_PAYLOAD     get_lte_pdsch_sdu_payload(U32 cw, U32 chanId, U32 harq_id);
PLTE_PDSCH_SDU_CTRL_RESEND get_lte_pdsch_sdu_ctrl_resend(U32 cw, U32 chanId, U32 harq_id);

PLTE_PDCCH_CHAN_STATE      get_lte_pdcch_chan_state(void);

RESULTCODE lte_harq_init(void);
RESULTCODE lte_harq_proc_rxsdu(PRXSDUIND pRxSduInd);
U8         lte_harq_get_harq_id(U8 pdsch_idx, U32 curr_tti);
RESULTCODE lte_harq_fill_pdsch_ndi_rv(PDLSUBCHINFO pSubCh, U32 cw_id, U8 pdsch_idx, U8 harq_id);

RESULTCODE setTxFrameSubFrameNum(void);
RESULTCODE setFrameSubFrameNum(void);

U32 get_globalTTITickCount(void);

U8  getSubFrameNum(void);
U32 getFrameNum(void);

U32 getTxFrameNum(void);
U8  getTxSubFrameNum(void);
U8  getTxSubFrameNumMinus1(void);


#endif
