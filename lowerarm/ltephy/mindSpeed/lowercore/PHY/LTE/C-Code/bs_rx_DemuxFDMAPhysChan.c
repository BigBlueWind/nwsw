//-------------------------------------------------------------------------------------------
/** @file bs_rx_DemuxFDMAPhysChan.c
 *
 * @brief Base Station Receiver. Extract the resource elements (IQ values) for this individual user, based on the RBAssignment.
 * @author Mindspeed Technologies
 * @version $Revision: 1.17 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#ifdef _WIN32
#define WIN_BLD 1
#else
#define WIN_BLD 0
#endif

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#define CEVA_BUILD 1
#else
#include "basic_op_cc.h"
#define CEVA_BUILD 0
#endif

//------------------------------------------------------------------------------------------------
/** @brief bs_rx_DemuxFDMAPhysChan(): Base Station Receiver. Extract the resource elements (IQ values) for this individual user,
 *                                    based on the RBAssignment.
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param RBAssignment    = Pointer to Resource Block Assignment
 *  @param Nsym            = Number of symbols to be processed
 *  @param Nrb_sc          = Number of subcarriers in a resource block
 *  @param stepsize_aDet   = Number of IQ samples to skip to next Antenna
 *  @param K               = Number of resource element per symbol in resource grid 
 *  @param L               = Number of symbols in resource grid
 *  @param MO              = Number of receiving antennas 
 *  @param rs_grid         = Pointers to received Resource Grid for signal group to be processed
 *  @param ChanFormat      = Channel Format
 *  @param expo_mat        = pointer to exponent after FFT at each symbol
 *  @param expo_mat_ch     = pointer to exponent of channel estimation FFT at each symbol
 *  @param expo_mrc        = pointer to exponent of demux data after MRC combining at each symbol
 *  @param aDet            = Pointer to received Resource Grid for signal group to be processed
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction|Length           |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |         |                 |              |            |            |      |
 *  -----------------------------------------------------------------------------------------------------
 *  |RBAssignment |         |pInput   |Row_RBAssignment*|Real          |(1:15:0)    |[0,1,2,4,6] |      |
 *  |             |         |         |Col_RBAssignment |              |            |            |      |
 *  -----------------------------------------------------------------------------------------------------
 *  |Nsym         |         |input    |1                |Real          |(1:15:0)    |[0:13]      |      |
 *  -----------------------------------------------------------------------------------------------------
 *  |Nrb_sc       |         |input    |1                |Real          |(1:15:0)    |[10:12]     |      |
 *  -----------------------------------------------------------------------------------------------------
 *  |stepsize_aDet |        |output   |1                |Real          |(1:15:0)    |            |      |
 *  -----------------------------------------------------------------------------------------------------
 *  |K            |         |input    |1                |Real          |(1:15:0)    |[1:1200]    |      |
 *  -----------------------------------------------------------------------------------------------------
 *  |L            |         |input    |1                |Real          |(1:15:0)    |12,14       |      |
 *  -----------------------------------------------------------------------------------------------------
 *  |MO           |         |input    |1                |Real          |(1:15:0)    |[1,4]       |      |
 *  -----------------------------------------------------------------------------------------------------
 *  |rs_grid      |         |output   |14*MO            |pReal         |(1:0:15)    |Full Range  |      |
 *  -----------------------------------------------------------------------------------------------------
 *  |ChanFormat   |         |input    |1                |Real          |(1:15:0)    |[0:10]      |      |
 *  -----------------------------------------------------------------------------------------------------
 *  |expo_mat     |         |output   |1                |pReal         |(1:15:0)    |            |      |
 *  -----------------------------------------------------------------------------------------------------
 *  |expo_mat_ch  |         |output   |1                |pReal          |(1:15:0)   |            |      |
 *  -----------------------------------------------------------------------------------------------------
 *  |expo_mrc     |         |output   |1                |pReal          |(1:15:0)   |            |      |
 *  -----------------------------------------------------------------------------------------------------
 *  |adet         |         |input    |K*Nsym+Nsym      |pComplex Array|(1:0:15)    |Full Range  |      |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |bs_rx_DemuxFDMAPhysChan           |
 *  ---------------------------------------------------------
 *  |Input Streams       |adet,RBAssignment                 |
 *  ---------------------------------------------------------
 *  |Output Streams      |rs_grid,expo_mat,expo_mat_ch,     |
 *  |                    |expo_mrc                          |
 *  ---------------------------------------------------------
 *  |Config Parameters   |Nsym,Nrb_sc,K,L,ChanFormat, MO,   |
 *  |                    |stepsize_aDet                     |
 *  ---------------------------------------------------------
 *  |Status Parameters   |none                              |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 * <4> Detailed Description:
 * @details Base Station Receiver. Extract the resource elements (IQ values) for this individual user, based on the RBAssignment.
 *  Note 1: To save storage space, the resource elements extracted are stored sequentially in the freq dimension, without
 *        recording how many frequency elements were skipped between resource blocks. This assumes
 *        that the ChEstimation that follows does channel estimation in the frequency dimension separately for each resource
 *        block.
 *  Note 2: In the Turner device, it should be possible to do this entire function using only the DMA, so uses no Turner DSP
 *        execution cycles.
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * None
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
void bs_rx_DemuxFDMAPhysChan_fx(ULRXINPUTPARAMS *pRxInParams, S32 stepsize_aDet, U32 *rs_grid, S32 ChanNum)
{
    S32 isym, Nsym;
    S32 k0, ind, Nrb_sc;
    S16 nPRB, K, pucchFormat = -1;

    nPRB = pRxInParams->RBStart[ChanNum]  ; //resource block number
    Nrb_sc = pRxInParams->Nrb_sc;
    Nsym = pRxInParams->L;
    K = pRxInParams->K;
    if (pRxInParams->physChanType[ChanNum] == PUCCH)
        pucchFormat = pRxInParams->pucchFormat[ChanNum];

        for (isym = 0; isym < (S32) Nsym; isym++) //symbol index within SLOT
        {
            k0 = nPRB * Nrb_sc; //frequency index for first resource element in the first
        if (isym >= (Nsym >> 1) && pucchFormat >= FORMAT1) // second slot PUCCH
                k0 = K - k0 - Nrb_sc;

            ind = 2 * k0 + stepsize_aDet * isym;
        rs_grid[isym] = (U32) (ind);
    }

    return;
}

