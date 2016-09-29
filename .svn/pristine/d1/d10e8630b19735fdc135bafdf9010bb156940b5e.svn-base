//-------------------------------------------------------------------------------------------
/** @file bs_rx_feq.c
 *
 * @brief LTE Base Station Receiver FEQ Implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.18 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------


#ifdef _WIN32
#define WIN_BLD 1
#else
#define WIN_BLD 0
#endif

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
#include "math_ext32.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#define CEVA_BUILD 1
#else
#include "basic_op_cc.h"
#define CEVA_BUILD 0

#define round(x)    round_s(x)

#endif

//-------------------------------------------------------------------------------------------
/** @brief LTE Base Station Rx, Frequency Equalizer
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param reEst1          - pointer to be array to be equalized
 *  @param reDet           - pointer to be final output array after equalization
 *  @param ChEst_pwr_fx    - pointer to channel estimate powers
 *  @param expo_reEst      - pointer to exponent for all elements in reEst1
 *  @param expo_reEst_norm - pointer to exponent for all elements in reEst1 normalize
 *  @param Mpusch_sc       - length of inputs and outputs (number of subcarriers)
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name           |Name in  |Direction  |Length     |Type          |Data Format |Valid Range |Units |
 *  |               |Standard |           |           |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |reEst1         |         |pInput     |Mpusch_sc  |pComplex Array|(1:0:31)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |reDet          |         |pOutput    |Mpusch_sc  |Real          |(1:0:31)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |ChEst_pwr_fx   |         |pInput     |Mpusch_sc  |Real          |(1:0:31)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |expo_reEst     |         |pInput     |Mpusch_sc  |Real          |(1:15:0)    |[-16,15]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  |expo_reEst_norm|         |Input      |1          |Real          |(1:15:0)    |[-16,15]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  |Mpusch_sc      |         |Input      |1          |Real          |(1:15:0)    |[0:1200]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |bs_rx_feq32b                      |
 *  ---------------------------------------------------------
 *  |Input Streams       |reEst1,ChEst_pwr_fx,expo_reEst,   |
 *  |                    |expo_reEst_norm                   |
 *  ---------------------------------------------------------
 *  |Output Streams      |reDet                             |
 *  ---------------------------------------------------------
 *  |Config Parameters   |Mpusch_sc                         |
 *  ---------------------------------------------------------
 *  |Status Parameters   |none                              |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  Frequency Equalizer.  Takes the output from the MRC algorithm and divides <BR>
 *  by the power of the Channel Estimate.
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 v8.7, Sec.5.3, "Uplink: Physical Uplink Shared Channel"
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
void bs_rx_feq32b(S32 reEst1[], S16 reDet[], S32 ChEst_pwr_fx[], S16 expo_reEst[],
                  S16* expo_reEst_norm, S32 noiseVarAvg, S16 Mpusch_sc)
{
    S16 k;
    S16 temp[2] =
    { 0 }, temp2x0, temp2x1;
    S16 max_expo;
    S32 L_temp;

    max_expo = -32768;
    for (k = 0; k < Mpusch_sc; k += 2)
    {
        ChEst_pwr_fx[k] = L_add(ChEst_pwr_fx[k], noiseVarAvg);
		ChEst_pwr_fx[k + 1] = L_add(ChEst_pwr_fx[k + 1], noiseVarAvg);
        expo_reEst[k] = norm_l(ChEst_pwr_fx[k]);
        L_temp = L_shl(ChEst_pwr_fx[k], expo_reEst[k]);

        //divide_tab: temp is in Q(29+16),Q(45)
        //29 from Tab, 16 from round_s before tab
        temp2x0 = round(L_temp);

        expo_reEst[k + 1] = norm_l(ChEst_pwr_fx[k + 1]);
        L_temp = L_shl(ChEst_pwr_fx[k + 1], expo_reEst[k + 1]);

        //divide_tab: temp is in Q(29+16),Q(45)
        //29 from Tab, 16 from round_s before tab
        temp2x1 = round(L_temp);

#ifdef CEVA_INTRINSICS
        divide_tab_lookup2_A( temp2x0, temp2x1, temp );
#endif

        if (ChEst_pwr_fx[k] == 0)
        {
            temp[0] = 0;
            expo_reEst[k] = 0;
        }
#ifndef CEVA_INTRINSICS
        else
        {
            temp[0] = divide_tab_lookup(temp2x0);
        }
#endif
        if (expo_reEst[k] > max_expo)
        {
            max_expo = expo_reEst[k];
        }

        // Output maximum ratio-combined value (real part)
        reEst1[2 * k] = L_mpy_ls(reEst1[2 * k], temp[0]);
        // Output maximum ratio-combined value (imaginary part)
        reEst1[2 * k + 1] = L_mpy_ls(reEst1[2 * k + 1], temp[0]);

        if (ChEst_pwr_fx[k + 1] == 0)
        {
            temp[1] = 0;
            expo_reEst[k + 1] = 0;
        }
#ifndef CEVA_INTRINSICS
        else
        {
            temp[1] = divide_tab_lookup(temp2x1);
        }
#endif
        if (expo_reEst[k + 1] > max_expo)
        {
            max_expo = expo_reEst[k + 1];
        }

        // Output maximum ratio-combined value (real part)
        reEst1[2 * k + 2] = L_mpy_ls(reEst1[2 * k + 2], temp[1]);
        // Output maximum ratio-combined value (imaginary part)
        reEst1[2 * k + 3] = L_mpy_ls(reEst1[2 * k + 3], temp[1]);
    }

    //
    // There is one more if Mpusch_sc is odd
    //
    if ((Mpusch_sc & 1) != 0 && Mpusch_sc > 1)
    {
        k = Mpusch_sc - 1;
        ChEst_pwr_fx[k] = L_add(ChEst_pwr_fx[k], noiseVarAvg);
		ChEst_pwr_fx[k + 1] = L_add(ChEst_pwr_fx[k + 1], noiseVarAvg);
        if (ChEst_pwr_fx[k] == 0)
        {
            temp[0] = 0;
            expo_reEst[k] = 0;
        }
        else
        {
            expo_reEst[k] = norm_l(ChEst_pwr_fx[k]);
            L_temp = L_shl(ChEst_pwr_fx[k], expo_reEst[k]);

            //divide_tab: temp is in Q(29+16),Q(45)
            //29 from Tab, 16 from round_s before tab
            temp2x0 = round(L_temp);
#ifdef CEVA_INTRINSICS
            divide_tab_lookup2_A( temp2x0, temp2x0, temp );
#else
            temp[0] = divide_tab_lookup(temp2x0);
#endif
        }
        if (expo_reEst[k] > max_expo)
        {
            max_expo = expo_reEst[k];
        }

        // Output maximum ratio-combined value (real part)
        reEst1[2 * k] = L_mpy_ls(reEst1[2 * k], temp[0]);
        // Output maximum ratio-combined value (imaginary part)
        reEst1[2 * k + 1] = L_mpy_ls(reEst1[2 * k + 1], temp[0]);
    }

    for (k = 0; k < Mpusch_sc; k++)
    {
        reEst1[2 * k] = L_shl(reEst1[2 * k], (S16) (expo_reEst[k] - max_expo));
        reEst1[2 * k + 1] = L_shl(reEst1[2 * k + 1], (S16) (expo_reEst[k] - max_expo));
    }

    temp[0] = norm32to16(reEst1, reDet, 2 * Mpusch_sc, 0);

    *expo_reEst_norm = max_expo - temp[0];

    return;
}
