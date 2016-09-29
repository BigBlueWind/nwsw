/** @file lte_harq.c
 *
 * @brief Implemenataion of harq-processing
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT(c) 2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "LtePhyL2Api.h"
#include "resultcodes.h"
#include "LteMacTestCfgMgr.h"

#include "lte_mac_state.h"
#include "lte_pucch.h"


#ifndef MX_ARM_ENABLED
#include "udp_stream.h"
#endif



LTE_MAC_STATE   lte_mac_state;
U8              rv_table[4]={0, 2, 3, 1};

PLTE_MAC_STATE get_lte_mac_state(void)
{
    return &lte_mac_state;
}

PLTE_PDSCH_HARQ_STATE      get_lte_pdsch_harq_state(void)
{
    return (PLTE_PDSCH_HARQ_STATE)&lte_mac_state.lte_pdsch_harq_state;
}

PLTE_PDSCH_SDU_CTRL_RESEND get_lte_pdsch_sdu_ctrl_resend(U32 cw, U32 chan_id, U32 harq_id)
{
    return (PLTE_PDSCH_SDU_CTRL_RESEND)&lte_mac_state.lte_pdsch_harq_state.pdsch_sdu_ctrl_resend[cw][chan_id][harq_id];
}

PLTE_PDSCH_SDU_PAYLOAD     get_lte_pdsch_sdu_payload(U32 cw, U32 chan_id, U32 harq_id)
{
    return (PLTE_PDSCH_SDU_PAYLOAD)&lte_mac_state.lte_pdsch_harq_state.pdsch_sdu_payload[cw][chan_id][harq_id];

}


RESULTCODE lte_harq_init(void)
{
    U32 i, j, k;

    memset((char *)&lte_mac_state, 0x00, sizeof(LTE_MAC_STATE));

// set nack & tti_tick for sending first 4 tti without wait response
    for (i=0; i<MAX_HARQ_PROCESS/2; i++)
    {
        for (j=0; j<MAX_HARQ_CW; j++)
        {
            for (k=0; k<MAX_HARQ_PDSCH_CHANNELS; k++)
            {
                lte_mac_state.lte_pdsch_harq_state.pdsch_sdu_ctrl_resend[j][k][i].nack = NACK_HARQ_OK;
                lte_mac_state.lte_pdsch_harq_state.pdsch_sdu_ctrl_resend[j][k][i].tti_tick = (U32)(i-4);
            }
        }
    }
    return SUCCESS;
}


RESULTCODE lte_harq_proc_rxsdu(PRXSDUIND pRxSduInd)
{
    PLTE_PDSCH_HARQ_STATE       plte_pdsch_harq_state = get_lte_pdsch_harq_state();
    PLTE_PDSCH_CHAN_STATE       plte_pdsch_chan_state = get_lte_pdsch_chan_state();


// temparary workaround for increase stability udp_stream
#if 0
    if (pRxSduInd->chanType == (U8)PUSCH)
    {       
        if (pRxSduInd->status == 0)
        {
#ifndef MX_ARM_ENABLED
            if (plte_pdsch_harq_state->ena_udp_stream)
            {
                U32 num_bytes = udp_stream_mpeg2_bytes_wo_padding((U8*)&pRxSduInd->pRxSdu);

                if (num_bytes)
                {
                    OSAL_QUEUE *pqueue = get_osal_udp_tx_queue();
// copy pusch to tx_queue
                    OsalCyclicBuffPutData_spinlock(pqueue, (U8*)&pRxSduInd->pRxSdu, num_bytes);
                }
            }
#endif
        }
    }

    if ((plte_pdsch_harq_state->ena_harq == 0) || (plte_pdsch_chan_state->ena_dynamic_cqi_pmi_ri == 0))
        return SUCCESS;

    if (pRxSduInd->chanType == PUCCH)
    {       
        if (pRxSduInd->status == 0)
        {
            lte_pucch_update_cqi_harq(pRxSduInd);
        }
    }
#endif
    return SUCCESS;
}

U8 lte_harq_get_harq_id(U8 pdsch_idx, U32 curr_tti)
{
    PLTE_PDSCH_HARQ_STATE       plte_pdsch_harq_state = get_lte_pdsch_harq_state();
    PLTE_PDSCH_SDU_CTRL_RESEND  plte_pdsch_sdu_ctrl_resend;
    U32                         i, prev_rxsdu_tti;


// Not checked harq for udp_stream
    if (plte_pdsch_harq_state->ena_udp_stream)
        return 0;

    if (plte_pdsch_harq_state->ena_harq == 0)
        return 0;

    for (i=0; i<MAX_HARQ_PROCESS; i++)
    {
        plte_pdsch_sdu_ctrl_resend = get_lte_pdsch_sdu_ctrl_resend(0, pdsch_idx, i);

        prev_rxsdu_tti = plte_pdsch_sdu_ctrl_resend->tti_tick;
//        uart_printf("prev_rxsdu_tti: %d\n", prev_rxsdu_tti);
        if ((prev_rxsdu_tti+4) == curr_tti)
        {
            return (U8)i;
        }
    }
    return 0xff;
}



RESULTCODE lte_harq_fill_pdsch_ndi_rv(PDLSUBCHINFO pSubCh, U32 cw_id, U8 pdsch_idx, U8 harq_id)
{
    PLTE_PDSCH_HARQ_STATE       plte_pdsch_harq_state = get_lte_pdsch_harq_state();
    PLTE_PDSCH_SDU_CTRL_RESEND  pdsch_sdu_ctrl_resend = get_lte_pdsch_sdu_ctrl_resend(cw_id, pdsch_idx, harq_id);

    if (plte_pdsch_harq_state->ena_harq)
    {
        if (pdsch_sdu_ctrl_resend->nack)
        {
            pSubCh->nDi = 1;
            pdsch_sdu_ctrl_resend->rv = 0;
            pSubCh->rV  = 0;
        }
        else
        {
            if (pdsch_sdu_ctrl_resend->rv < 3)
            {
                pSubCh->nDi = 0;
                pdsch_sdu_ctrl_resend->rv++;
                pSubCh->rV  = rv_table[pdsch_sdu_ctrl_resend->rv];
            }
            else
            {
//                uart_printf("%s: all efforts have been exhausted\n", __FUNCTION__);
                pSubCh->nDi = 1;
                pdsch_sdu_ctrl_resend->rv = 0;
                pSubCh->rV  = 0;
            }
        }
        pdsch_sdu_ctrl_resend->nack = 0;
    }
    else
    {
        pSubCh->nDi =  1;
        pSubCh->rV  = 0;
    }

    return SUCCESS;
}



