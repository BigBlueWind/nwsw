/** @file lte_nmm_state.c
 *
 * @brief Implemenataion for processing msg from nmm
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "resultcodes.h"
#include "apidefs.h"
#include "LtePhyL2Api.h"
#include "apimgr.h"
#include "appids.h"
#include "supervisor.h"
#include "appprintf.h"

#include "lte_nmm_state.h"

LTE_NMM_CTX     lte_nmm_ctx={0, 0, 0, 0, 0, NULL};

/** Return pointer to lte_nmm_ctx
 *
 *  @return pointer to  lte_nmm_ctx
 */
PLTE_NMM_CTX get_lte_nmm_ctx(void)
{
    return (PLTE_NMM_CTX)&lte_nmm_ctx;
}

/** Copies new NMM indication to the fifo buffer
 *
 * @param Buf Pointer to the NMM ind
 * @param Size Size of this NMM ind
 */
void add_nmm_ind_to_buf(U8 *buf, U32 size)
{
    PLTE_NMM_CTX plte_nmm_ctx = (PLTE_NMM_CTX)get_lte_nmm_ctx();
    
    if (((plte_nmm_ctx->RxNmmSet + size) <= plte_nmm_ctx->RxNmmSize) && (plte_nmm_ctx->RxNmmBuf != NULL))
    {
        memcpy(plte_nmm_ctx->RxNmmBuf + plte_nmm_ctx->RxNmmSet, buf, size);
        plte_nmm_ctx->RxNmmSet += ROUND(size,sizeof(U32));
        plte_nmm_ctx->RxNmmCntr++;
    }
    else
    {
        uart_printf("%s: buffer (%d bytes) overflow\r\n", __FUNCTION__, plte_nmm_ctx->RxNmmSize);
    }
}

/** @breif Forwards contents of the internal fifo buffer of received SDUs to Host
 *         like a single I-CPU message
 *
 *  @return 
 */
void drain_nmm_ind_buf(void)
{
    PLTE_NMM_CTX plte_nmm_ctx = (PLTE_NMM_CTX)get_lte_nmm_ctx();

    if (plte_nmm_ctx->RxNmmSet)
    {
        ApiHeader* pInd = NULL;

        // Alloc I-CPU message
        pInd = SvsrAllocICApi();
        if (pInd == NULL)
        {
            uart_printf("no memory in I-CPU block\r\n");
            return;
        }

        // Create and send I-CPU message
        pInd = ApiCreate(pInd, API_DEF_FLAGS, IID_LTE_MAC, UARM_INSTID_DEFMAC, API_TYPE_CFG_RESP, MAC_NMM_IND);

        ApiAddParam(pInd, NULL, PAR_BIN_DATA, plte_nmm_ctx->RxNmmBuf, plte_nmm_ctx->RxNmmSet);

        SvsrSendICApi(pInd);
    }
    else
    {
        uart_printf("nothing nmm send\r\n");

    }

    return;
}


