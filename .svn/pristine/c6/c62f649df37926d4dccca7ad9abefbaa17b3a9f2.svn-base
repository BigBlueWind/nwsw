/** @file lte_mac_state.c
 *
 * @brief Implemenataion for updating mac-state into work-cycle
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "LtePhyL2Api.h"
#include "resultcodes.h"

#include "LteMacTestCfgMgr.h" // MAX_PDSCH_CHANNELS
#include "lte_mac_state.h"

extern V32 globalTTITickCount;


// -----------------------------------------------------------------------------------------------------
/** Set current Frame/Subframe Number (for Tx)
 *
 * @return resultcode
 */
RESULTCODE setFrameSubFrameNum(void)
{
    PLTE_MAC_STATE             plte_mac_state = get_lte_mac_state();

    plte_mac_state->rx_frame_num = globalTTITickCount/10;
    plte_mac_state->rx_sub_frame_num = (U8)(globalTTITickCount - plte_mac_state->rx_frame_num*10);

    return SUCCESS;
}


/** Set current Frame/Subframe Number (for Tx)
 *
 * @return resultcode
 */
RESULTCODE setTxFrameSubFrameNum(void)
{
    PLTE_MAC_STATE             plte_mac_state = get_lte_mac_state();

    if (plte_mac_state->lte_pdsch_harq_state.ena_udp_stream)
    {
        plte_mac_state->tx_frame_num = (globalTTITickCount)/10;
        plte_mac_state->tx_sub_frame_num = (globalTTITickCount)%10;
/*
#ifdef   LOWLATENCY
        plte_mac_state->tx_frame_num = (globalTTITickCount+1)/10;
        plte_mac_state->tx_sub_frame_num = (globalTTITickCount+1)%10;
#else
        plte_mac_state->tx_frame_num = (globalTTITickCount+2)/10;
        plte_mac_state->tx_sub_frame_num = (globalTTITickCount+2)%10;
#endif*/
    }
    else
    {
        plte_mac_state->tx_frame_num = (globalTTITickCount)/10;
        plte_mac_state->tx_sub_frame_num = (globalTTITickCount)%10;
    }
    return SUCCESS;
}


// -----------------------------------------------------------------------------------------------------
/** Get current TTI Number
 *
 * @return TTI Number
 */
U32 get_globalTTITickCount(void)
{
    return globalTTITickCount;
}


/** Get current System Frame Number
 *
 * @return System Frame Number
 */
U32 getFrameNum(void)
{
    PLTE_MAC_STATE             plte_mac_state = get_lte_mac_state();

    return plte_mac_state->rx_frame_num;
}


/** Get current Subframe Number
 *
 * @return Subframe Number
 */
U8 getSubFrameNum(void)
{
    PLTE_MAC_STATE             plte_mac_state = get_lte_mac_state();

    return plte_mac_state->rx_sub_frame_num;
}


/** Get current System Frame Number (for Tx)
 *
 * @return System Frame Number (for Tx)
 */
U32 getTxFrameNum(void)
{
    PLTE_MAC_STATE             plte_mac_state = get_lte_mac_state();
    return plte_mac_state->tx_frame_num;
}


/** Get current System SubFrame Number (for Tx)
 *
 * @return System SubFrame Number (for Yx)
 */
U8 getTxSubFrameNum(void)
{
    PLTE_MAC_STATE             plte_mac_state = get_lte_mac_state();

    return plte_mac_state->tx_sub_frame_num;
}

