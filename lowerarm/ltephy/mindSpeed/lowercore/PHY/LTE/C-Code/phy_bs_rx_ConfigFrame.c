//-------------------------------------------------------------------------------------------
/** @file phy_bs_rx_ConfigFrame.c
 *
 * @brief Setup Config parameters for Uplink at beginning of each radio frame.
 * @author Mindspeed Technologies
 * @version $Revision: 1.15 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
#include "math_ext32.h"
#include "basic_op_cc.h"

//-------------------------------------------------------------------------------------------
// phy_bs_rx_ConfigFrame()
//
/** @brief Base Station receiver, Parameters configuration at frame level of PRACH
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param bsRxPrach       Pointer to BS_RX_PRACH info structure
 *  @param bsRxPrachDet    Pointer to BS_RX_PRACHDet info structure
 *
 *  @return             None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------------------------------------
 *  |Name            |Name in   |Direction      |Length                     |Type                       |Data Format |Valid Range |Units|
 *  |                |Standard  |               |                           |                           |            |            |     |
 *  -----------------------------------------------------------------------------------------------------------------------------------
 *  |ul_prach       |          |input          |1                          |pStructure(BS_RX_PRACH)    |            |            |     |
 *  ------------------------------------------------------------------------------------------------------------------------------------
 *  |ul_prachDet    |          |input          |1                          |pStructure(BS_RX_PRACHDet) |            |            |     |
 *  ------------------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |phy_bs_rx_ConfigFrame       |
 *  ---------------------------------------------------
 *  |Input Streams       |                            |
 *  ---------------------------------------------------
 *  |Output Streams      |                            |
 *  ---------------------------------------------------
 *  |Config Parameters   |bsRxPrach, bsRxPrachDet    |
 *  ---------------------------------------------------
 *  |Status Parameters   |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  Base Station receiver, parameter configuration for Random Access Channel processing. This
 *  intermediate function configures/derives relevant parameters for PRACH processing.
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
void phy_bs_rx_ConfigFrame(BS_RX_PRACH *ul_prach, BS_RX_PRACHDet *ul_prachDet)
{
    S16 highSpeedFlag;
    S16 NcsConfig;

    //------------------------------------------------------------------------------------------------------------------

    //---------------------------------------------------------------------------------------------------------------
    //(4) Init PRACH Preamble detection arrays for this radio frame [3GPP TS 36.211 Rel.8, Sec.5.7]

    if (ul_prach->bs_prach_enable == 1)
    {
        //extract local variables from data structures
        highSpeedFlag = ul_prach->bs_prach_highSpeedFlag;
        NcsConfig = ul_prach->bs_prach_cyclicShiftConfig;
        //
        //init prachDet basic parameters
        //
        //get the number of different u'th root sequences used in the set of 64 preamble sequences for this BS
        ul_prachDet->bs_prachDet_numRootSeq = CA_prachPreambleGenTable_numU[NcsConfig + 16
            * highSpeedFlag];
        //get the number of different cyclic shifts used with each u'th root sequence
        ul_prachDet->bs_prachDet_numCyclicShiftsPerRootSeq
            = CA_prachPreambleGenTable_numV[NcsConfig + 16 * highSpeedFlag];
        //
        //For the 64 possible cazac/zadoff-chu sequences for this base station, calculate the
        //time index for the correlation peak with ideal channel (no delay, or time dispersion)
        //
        //This time index in all 64 cases is given by
        //  k=mod(-u*Cv,Nzc)
        // where,
        // u      = u'th root zadoff-chu sequence, in x_u
        // Cv     = cyclic shift of the u'th root sequence used in the MS, x_uv
        //        = 0, for the locally generated correlating sequence in the BS
        // Nzc    = length of zadoff-chu sequence, x_u
        //for (ics = 0; ics < 64; ics++)
        //{
        //    // i.e. sweep all 64 possible cazac sequences for this cell
        //    // init pointers to u'th root sequence
        //    ul_prachDet->bs_prachDet_cyclicShift = ics;
        //    ul_prachDet->bs_prachDet_enable = 1;
        //    ul_prachDet->bs_prachDet_uOffset = 0; //don't force uOffset. Go through all 64 cyclic shifts normally
        //    //generate parameters for this x_uv sequence
        //    ul_PrachPreamble_Config(ul_prach, ul_prachDet, &u, &Cv, &vLen, &Ncs);
        //    //Calculate time index for this correlation peak, under ideal channel conditions
        //    Ltemp = -u * Cv;
        //    if (Ltemp >= 0)
        //    {
        //        ul_prachDet->bs_prachDet_tsamOffset[ics] = Ltemp % Nzc;
        //    }
        //    else
        //    {
        //        temp = (S16) divide_floor(Ltemp, Nzc);
        //        ul_prachDet->bs_prachDet_tsamOffset[ics] = (S16) Ltemp - temp * Nzc;
        //    }
        //    //printf("tsamOffset %hd \n", ul_prachDet->bs_prachDet_tsamOffset[ics]);
        //}
    } //bsRx.prach.enable
}
