/** @file lte_nmm_state.h
 *
 * @brief Defenitions and prototypes for processing msg from nmm
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
 
#ifndef LTE_NMM_STATE_H
#define LTE_NMM_STATE_H

#define NMM_IND_SIZE            1024
#define MAX_NUM_NMM_INTO_BUF    64

//---------------------------------------------------------------------------------------------------
typedef struct 
{
    U32 cnt_nmm_rssi_meas_ind;
    U32 cnt_local_nmm_rssi_meas_ind;

    U32 RxNmmSet;
    U32 RxNmmCntr;
    U32 RxNmmSize;    

    U8* RxNmmBuf;
    
} LTE_NMM_CTX, *PLTE_NMM_CTX;

PLTE_NMM_CTX get_lte_nmm_ctx(void);
void add_nmm_ind_to_buf(U8 *buf, U32 size);
void drain_nmm_ind_buf(void);

#endif //LTE_NMM_STATE_H
