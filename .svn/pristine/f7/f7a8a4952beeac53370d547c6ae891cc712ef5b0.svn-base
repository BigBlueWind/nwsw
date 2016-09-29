/*
 * lte_bs_phy_nmm_thread.h
 */

#ifndef _LTE_BS_PHY_NMM_THREAD
#define _LTE_BS_PHY_NMM_THREAD

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "LtePhyL2Api.h"
#include "ext_phy.h"

#ifdef RT_API_PROC

#include "LteBsApiThread.h"
#include "supervisor.h"
#include "ltertconfig.h"
#include "lte_entry.h"

#define RC_LTE_NMM_PARAM_ERROR         DEF_USRRC(APP_MODULE, LTENMMAPP, 1)
#define LTE_NMM_LINKEDLIST_PARAM_ID     2


typedef struct st_nmm2phy_queue_elem
{
    U16 frame_number;
    U8  message_type;
    U8  subframe_number;
    U32 message_len;
    struct st_nmm2phy_queue_elem* next;
    U8* message_ptr;
} NMM2PHY_QUEUE_EL, *PNMM2PHY_QUEUE_EL;

typedef struct st_nmm2phy_queue
{
    PNMM2PHY_QUEUE_EL start_queue_curr;
    PNMM2PHY_QUEUE_EL tail_queue_curr;
} NMM2PHY_QUEUE, *PNMM2PHY_QUEUE;

typedef struct st_nmm_msg_head
{
    U16 msg_length;
    U8  msg_type;
} NMM_MSG_HEADER, *PNMM_MSG_HEADER;


void lte_bs_phy_nmm_fill_iq_buf(U32 , S16 * );
MXRC lte_bs_phy_nmm_init_api_thread(void);
MXRC lte_bs_phy_nmm_msg_handler(LPVOID, LPVOID);
MXRC lte_bs_phy_nmm_list_add_to_tail(U8 message_type, U32 message_len, U8* message_ptr);
MXRC lte_bs_phy_nmm_api_handler(LPVOID pClientCtx, ApiHeader *pApi, ApiHeader *pResp);

UINT32 lte_bs_phy_nmm_api_thread(LPVOID);
MXRC   lte_bs_phy_nmm_thread_init(void);
MXRC   lte_bs_phy_nmm_thread_kill(void);
void   lte_bs_phy_nmm_schedule_thread(void);



#endif

#endif
