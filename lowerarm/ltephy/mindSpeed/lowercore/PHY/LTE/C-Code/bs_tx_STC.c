//-------------------------------------------------------------------------------------------
/** @file bs_tx_STC.c
 *
 * @brief LTE Base Station Transmit STC Implementation
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
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#else
#include "basic_op_cc.h"
#endif

//-------------------------------------------------------------------------------------------
/** @brief Base Station, transmitter, Transmit Diversity mapping
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param d0                  - INPUT, Codeword0 IQ samples
 *  @param M0symb              - INPUT, length of d0
 *  @param num_bs_Ntx_antennas - INPUT, number of transmit antennas
 *  @param y                   - OUTPUT, Pointer to output array
 *
 *  @return    			None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------
 *  |Name               |Name in  |Direction|Length  |Type          |Data Format |Valid Range |Units  |
 *  |                   |Standard |         |        |              |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |d0                 |         |pInput   |M0symb* |pComplex Array|(1:1:14)    |Full Range  |       |
 *  |                   |         |         |num_bs_ |              |            |            |       |
 *  |                   |         |         |Ntx_ante|              |            |            |       |
 *  |                   |         |         |nnas    |              |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |M0symb             |         |Input    |1       |Real          |(1:31:0)    |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |num_bs_Ntx_antennas|         |Input    |1       |Real          |(1:15:0)    |[2,4]       |       |
 *  -------------------------------------------------------------------------------------------------
 *  |y                  |         |pOutput  |M0symb* |pComplex Array|(1:1:14)    |Full Range  |       |
 *  |                   |         |         |num_bs_ |              |            |            |       |
 *  |                   |         |         |Ntx_ante|              |            |            |       |
 *  |                   |         |         |nnas    |              |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |STC                         |
 *  ---------------------------------------------------
 *  |Input Streams       |d0                          |
 *  ---------------------------------------------------
 *  |Output Streams      |y                           |
 *  ---------------------------------------------------
 *  |Config Parameters   |M0symb,num_bs_Ntx_antennas  |
 *  ---------------------------------------------------
 *  |Status Parameters   |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function performs Space Time Coding 
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6.3.3.3, "Layer Mapping for Transmit diversity"<BR>
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void STC(S16 d0[], S32 M0symb, S16 num_bs_Ntx_antennas, S16 *y)
{
    S32 Map_symb;
    S32 index;
    S16 P;
    S32 i1, i2, i3, i4, P2, k, j;
    S16 d0_0, d0_1, d0_2, d0_3, d0_4, d0_5, d0_6, d0_7;

    Map_symb = M0symb;
    P = num_bs_Ntx_antennas; // for convenience
    //Precoding for Transmit Diversity
    P2 = (S32) add(P, P);

    if (num_bs_Ntx_antennas == 2)
    {
        /*
         for(index = 0; index < Map_symb/2; index++)
         {
         // First Row
         y[P*2*index+0] = mult(d0[4*index+0], 23170); // d(2i)
         y[P*2*index+1] = mult(d0[4*index+1], 23170);

         // Second Row
         y[P*2*index+2*Map_symb+0] = mult(d0[4*index+2], -23170); // d(2i+1)
         y[P*2*index+2*Map_symb+1] = mult(d0[4*index+3], 23170);

         // First Row
         y[P*2*index+2] = mult(d0[4*index+2], 23170); // d(2i+1)
         y[P*2*index+3] = mult(d0[4*index+3], 23170);

         // Second Row
         y[P*2*index+2*Map_symb+2] = mult(d0[4*index+0], 23170); // d(2i)
         y[P*2*index+2*Map_symb+3] = mult(d0[4*index+1], -23170);
         } //index
         */
        k = Map_symb >> 1;
        i1 = 0;
        i2 = L_add(Map_symb, Map_symb);
        j = 0;
        for (index = 0; index < k; index++, i1 += P2, i2 += P2)
        {
            d0_0 = d0[j++];
            d0_1 = d0[j++];
            d0_2 = d0[j++];
            d0_3 = d0[j++];
            // First Row
            y[i1] = mult(d0_0, 23170); // d(2i)
            y[i1 + 1] = mult(d0_1, 23170);

            // Second Row
            y[i2] = mult(d0_2, -23170); // d(2i+1)
            y[i2 + 1] = mult(d0_3, 23170);

            // First Row
            y[i1 + 2] = mult(d0_2, 23170); // d(2i+1)
            y[i1 + 3] = mult(d0_3, 23170);

            // Second Row
            y[i2 + 2] = mult(d0_0, 23170); // d(2i)
            y[i2 + 3] = mult(d0_1, -23170);
        } //index

    }
    else
    { //P==4
        /*
         for(index = 0; index < Map_symb/4; index++)
         {
         // First Row
         y[P*2*index+0] = mult(d0[8*index+0], 23170); // d(4i)
         y[P*2*index+1] = mult(d0[8*index+1], 23170);

         // Third Row
         y[P*2*index+2*2*Map_symb+0] = mult(d0[8*index+2], -23170); // d(4i+1)
         y[P*2*index+2*2*Map_symb+1] = mult(d0[8*index+3], 23170);

         // First Row
         y[P*2*index+2] = mult(d0[8*index+2], 23170); // d(4i+1)
         y[P*2*index+3] = mult(d0[8*index+3], 23170);

         // Third Row
         y[P*2*index+2*2*Map_symb+2] = mult(d0[8*index+0], 23170); // d(4i)
         y[P*2*index+2*2*Map_symb+3] = mult(d0[8*index+1], -23170);

         // Second Row
         y[P*2*index+2*Map_symb+4] = mult(d0[8*index+4], 23170); // d(4i+2)
         y[P*2*index+2*Map_symb+5] = mult(d0[8*index+5], 23170);

         // Fourth Row
         y[P*2*index+3*2*Map_symb+4] = mult(d0[8*index+6], -23170); // d(4i+3)
         y[P*2*index+3*2*Map_symb+5] = mult(d0[8*index+7], 23170);

         // Second Row
         y[P*2*index+2*Map_symb+6] = mult(d0[8*index+6], 23170); // d(4i+3)
         y[P*2*index+2*Map_symb+7] = mult(d0[8*index+7], 23170);

         // Fourth Row
         y[P*2*index+3*2*Map_symb+6] = mult(d0[8*index+4], 23170); // d(4i+2)
         y[P*2*index+3*2*Map_symb+7] = mult(d0[8*index+5], -23170);
         } //index
         */

        k = Map_symb >> 1;
        i1 = 0;
        i2 = L_add(Map_symb, Map_symb);
        i3 = L_add(i2, i2);
        i4 = L_add(i2, i3);
        j = 0;
        for (index = 0; index < Map_symb / 4; index++, i1 += P2, i2 += P2, i3 += P2, i4 += P2)
        {
            d0_0 = d0[j++];
            d0_1 = d0[j++];
            d0_2 = d0[j++];
            d0_3 = d0[j++];
            d0_4 = d0[j++];
            d0_5 = d0[j++];
            d0_6 = d0[j++];
            d0_7 = d0[j++];
            // First Row
            y[i1] = mult(d0_0, 23170); // d(4i)
            y[i1 + 1] = mult(d0_1, 23170);

            // Third Row
            y[i3] = mult(d0_2, -23170); // d(4i+1)
            y[i3 + 1] = mult(d0_3, 23170);

            // First Row
            y[i1 + 2] = mult(d0_2, 23170); // d(4i+1)
            y[i1 + 3] = mult(d0_3, 23170);

            // Third Row
            y[i3 + 2] = mult(d0_0, 23170); // d(4i)
            y[i3 + 3] = mult(d0_1, -23170);

            // Second Row
            y[i2 + 4] = mult(d0_4, 23170); // d(4i+2)
            y[i2 + 5] = mult(d0_5, 23170);

            // Fourth Row
            y[i4 + 4] = mult(d0_6, -23170); // d(4i+3)
            y[i4 + 5] = mult(d0_7, 23170);

            // Second Row
            y[i2 + 6] = mult(d0_6, 23170); // d(4i+3)
            y[i2 + 7] = mult(d0_7, 23170);

            // Fourth Row
            y[i4 + 6] = mult(d0_4, 23170); // d(4i+2)
            y[i4 + 7] = mult(d0_5, -23170);
        } //index
    }
}

//-------------------------------------------------------------------------------------------
/** @brief STC_type1: Base Station, transmitter, Transmit Diversity mapping for PHICH Type 1
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param d0                  - INPUT, Codeword0 IQ samples
 *  @param M0symb              - INPUT, length of d0
 *  @param y                   - OUTPUT, Pointer to output array
 *
 *  @return    			None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------
 *  |Name      |Name in  |Direction|Length  |Type          |Data Format |Valid Range |Units  |
 *  |          |Standard |         |        |              |            |            |       |
 *  ------------------------------------------------------------------------------------------
 *  |d0        |         |pInput   |8       |pComplex Array|(1:1:14)    |Full Range  |       |
 *  ------------------------------------------------------------------------------------------
 *  |M0symb    |         |Input    |1       |Real          |(1:31:0)    |            |       |
 *  ------------------------------------------------------------------------------------------
 *  |y         |         |pOutput  |2*2*    |pComplex Array|(1:1:14)    |Full Range  |       |
 *  |          |         |         |Map_symb|              |            |            |       |
 *  |          |         |         |+8      |              |            |            |       |
 *  ------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |STC_type1                   |
 *  ---------------------------------------------------
 *  |Input Streams       |d0                          |
 *  ---------------------------------------------------
 *  |Output Streams      |y                           |
 *  ---------------------------------------------------
 *  |Config Parameters   |M0symb                      |
 *  ---------------------------------------------------
 *  |Status Parameters   |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function performs Space Time Coding Type 1 for PHICH channel
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6.9, "Physical hybrid ARQ indicator channel"<BR>
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void STC_type1(S16 d0[], S32 M0symb, S16 *y)
{
    S32 Map_symb;

    Map_symb = M0symb;

    //Precoding for Transmit Diversity
    // First Row
    y[0] = mult(d0[0], 23170);
    y[1] = mult(d0[1], 23170);

    // Third Row
    y[2 * 2 * Map_symb + 0] = mult(d0[2], -23170);
    y[2 * 2 * Map_symb + 1] = mult(d0[3], 23170);

    // First Row
    y[2] = mult(d0[2], 23170);
    y[3] = mult(d0[3], 23170);

    // Third Row
    y[2 * 2 * Map_symb + 2] = mult(d0[0], 23170);
    y[2 * 2 * Map_symb + 3] = mult(d0[1], -23170);

    // First Row
    y[4] = mult(d0[4], 23170);
    y[5] = mult(d0[5], 23170);

    // Third Row
    y[2 * 2 * Map_symb + 4] = mult(d0[6], -23170);
    y[2 * 2 * Map_symb + 5] = mult(d0[7], 23170);

    // First Row
    y[6] = mult(d0[6], 23170);
    y[7] = mult(d0[7], 23170);

    // Third Row
    y[2 * 2 * Map_symb + 6] = mult(d0[4], 23170);
    y[2 * 2 * Map_symb + 7] = mult(d0[5], -23170);

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief STC_type1: Base Station, transmitter, Transmit Diversity mapping for PHICH Type 2
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param d0                  - INPUT, Codeword0 IQ samples
 *  @param M0symb              - INPUT, length of d0
 *  @param y                   - OUTPUT, Pointer to output array
 *
 *  @return    			None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------
 *  |Name      |Name in  |Direction|Length  |Type          |Data Format |Valid Range |Units  |
 *  |          |Standard |         |        |              |            |            |       |
 *  ------------------------------------------------------------------------------------------
 *  |d0        |         |pInput   |8       |pComplex Array|(1:1:14)    |Full Range  |       |
 *  ------------------------------------------------------------------------------------------
 *  |M0symb    |         |Input    |1       |Real          |(1:31:0)    |            |       |
 *  ------------------------------------------------------------------------------------------
 *  |y         |         |pOutput  |3*2*    |pComplex Array|(1:1:14)    |Full Range  |       |
 *  |          |         |         |Map_symb|              |            |            |       |
 *  |          |         |         |+8      |              |            |            |       |
 *  ------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |STC_type2                   |
 *  ---------------------------------------------------
 *  |Input Streams       |d0                          |
 *  ---------------------------------------------------
 *  |Output Streams      |y                           |
 *  ---------------------------------------------------
 *  |Config Parameters   |M0symb                      |
 *  ---------------------------------------------------
 *  |Status Parameters   |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function performs Space Time Coding Type 2 for PHICH channel
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6.9, "Physical hybrid ARQ indicator channel"<BR>
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void STC_type2(S16 d0[], S32 M0symb, S16 *y)
{
    S32 Map_symb;

    Map_symb = M0symb;

    //Precoding for Transmit Diversity

    // Second Row
    y[2 * Map_symb + 0] = mult(d0[0], 23170);
    y[2 * Map_symb + 1] = mult(d0[1], 23170);

    // Fourth Row
    y[3 * 2 * Map_symb + 0] = mult(d0[2], -23170);
    y[3 * 2 * Map_symb + 1] = mult(d0[3], 23170);

    // Second Row
    y[2 * Map_symb + 2] = mult(d0[2], 23170);
    y[2 * Map_symb + 3] = mult(d0[3], 23170);

    // Fourth Row
    y[3 * 2 * Map_symb + 2] = mult(d0[0], 23170);
    y[3 * 2 * Map_symb + 3] = mult(d0[1], -23170);

    // Second Row
    y[2 * Map_symb + 4] = mult(d0[4], 23170);
    y[+2 * Map_symb + 5] = mult(d0[4], 23170);

    // Fourth Row
    y[3 * 2 * Map_symb + 4] = mult(d0[6], -23170);
    y[3 * 2 * Map_symb + 5] = mult(d0[7], 23170);

    // Second Row
    y[2 * Map_symb + 6] = mult(d0[6], 23170);
    y[2 * Map_symb + 7] = mult(d0[7], 23170);

    // Fourth Row
    y[3 * 2 * Map_symb + 6] = mult(d0[4], 23170);
    y[3 * 2 * Map_symb + 7] = mult(d0[5], -23170);

    return;
}

