//-------------------------------------------------------------------------------------------
/** @file ul_Pilot.c
 *
 * @brief Add Uplink Reference (pilot) signals to the resource grid
 * @author Mindspeed Technologies
 * @version $Revision: 1.31.2.1 $
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
#include "math_ext32.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>

extern U8 *DataBuff_Bank1;
extern U8 *DataBuff_Bank2;

#else

#include "basic_op_cc.h"

#endif

//-------------------------------------------------------------------------------------------
/** @brief Add Uplink Reference (pilot) signals to the resource grid [3GPP TS 36.211 v8.4.0, Sec.5.5]
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param a            = OUTPUT, Pointer to resource element grid for this antenna
 *  @param nPRS         = INPUT, Psuedo Random Sequence used to determine Pilots
 *  @param u            = INPUT, Group hopping number
 *  @param v            = INPUT, Sequence hopping number
 *  @param ULPilotInput = INPUT, Structure containing input parameters
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction|Length           |Type          |Data Format |Valid Range |Units |
 *  |            |Standard |         |                 |              |            |            |      |
 *  ----------------------------------------------------------------------------------------------------
 *  |a           |         |pOutput  |MI*K*L           |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------
 *  |nPRS        |         |pInput   |20               |Real          |(1:15:0)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------
 *  |u           |         |pInput   |2                |Real          |(1:15:0)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------
 *  |v           |         |pInput   |2                |Real          |(1:15:0)    |Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------
 *  |ULPilotInput|         |Input    |1                |Structure     |            |            |      |
 *  ----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |ul_Pilot_fx                 |
 *  ---------------------------------------------------
 *  |Input Streams       |nPRS, u, v                  |
 *  ---------------------------------------------------
 *  |Output Streams      |a                           |
 *  ---------------------------------------------------
 *  |Config Parameters   |ULPilotInput                |
 *  ---------------------------------------------------
 *  |Status Parameters   |none                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 * <4> Detailed Description:
 * Add Uplink Reference (pilot) signals to the resource grid [3GPP TS 36.211 v8.4.0, Sec.5.5]
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 v8.4.0, Sec.5.5, "Uplink: Reference Signals"
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
void ul_Pilot_fx(S16 a[], S16 nPRS[], ULRXINPUTPARAMS *pULPilotInput, S16 u[], S16 v[],
                 S16 pUlPilots_QthRootTable[], S32 ChanNum)
{
    S16 Mrs_sc, Nrs_zc = 0, Nrs_zcDiv;
    S32 slot;

    S16 q;
    S16 ncs[2];
    S32 n, primeNum_index;
    S16 length_rPUSCH;
    S16 temp1, temp3, expo;
    S32 L_temp1, L_temp2;

    //S16 L, K;
    S16 div_12, rbar_uvI, rbar_uvQ;
    S16 ctemp1, stemp1;
    S16 rPUSCH1, rPUSCH2;

    S8 *Phi_mat_ptr;
    S16 sign;
    S32 step = 0;
    S32 index;

#ifndef CEVA_INTRINSICS
    S32 L_temp3, m;

    S16 x_q[2 * MAXSUBCARRIERS];
#else
    S16 *x_q;

    x_q = (S16*)(DataBuff_Bank2);
#endif

    //K = pULPilotInput->K;
    //L = pULPilotInput->L;
    Mrs_sc = pULPilotInput->Mpusch_sc[ChanNum]; //= Mpusch_rb * Nrb_sc, from PUSCH bandwidth RBAssignment

    //Nrs_zc = %largest prime number such that Nrs_zc < Mrs_sc (so < 110*12=1320)
    primeNum_index = 0;
    while (CA_PrimeNumberTable[primeNum_index] < Mrs_sc)
        primeNum_index++;

    if (primeNum_index)
        primeNum_index = primeNum_index - 1;

    Nrs_zc = CA_PrimeNumberTable[primeNum_index];
    Nrs_zcDiv = UlPilots_Div_Nrs_zc_Table[2 * primeNum_index];
    expo = UlPilots_Div_Nrs_zc_Table[2 * primeNum_index + 1];
    length_rPUSCH = 2 * Mrs_sc;
    expo = expo + 2;

    for (slot = 0; slot < 2; slot++)
    {
        //qbar = Nrs_zc * (u+1.0)/31.0;
        //q = floor(qbar + 0.5) + v*pow(-1.0, floor(2*qbar));

        //Calculation of qth root of Zadoff-Chu sequence done with table lookup
        L_temp1 = (2 * primeNum_index * MAX_ULPILOTS_U + u[slot] + v[slot] * MAX_ULPILOTS_U);
        q = pUlPilots_QthRootTable[L_temp1];

        //set ncs from input parameters
        ncs[slot] = (pULPilotInput->n1_DMRS + pULPilotInput->n2_DMRS[ChanNum] + nPRS[slot]) % 12;

        //Generate reference signals for this PUSCH in this 1ms subframe
        //Generate qth root Zadoff Chu sequence, length = prime number.

        if (Mrs_sc >= 3 * pULPilotInput->Nrb_sc)
        {
#ifndef CEVA_INTRINSICS
            L_temp2 = 2 * Nrs_zc;
            for (m = 0; m < Nrs_zc; m++)
            {
                //cos( PI * q * m * (m+1) / (1.0*Nrs_zc) =
                //cos( 2*PI * q * m * (m+1) / (2.0*Nrs_zc)
                L_temp1 = q * m * (m + 1);
                if (L_temp1 > L_temp2)
                {
                    L_temp1 = L_temp1 % L_temp2;
                }

                L_temp3 = L_mult((S16) L_temp1, Nrs_zcDiv);
                L_temp3 = L_shr(L_temp3, expo);
                index = mult_r((S16) (L_temp3), 4096);

                if (index < 2048)
                {
                    x_q[2 * m] = Table_FFT16b_4096[2 * index];
                    x_q[2 * m + 1] = Table_FFT16b_4096[2 * index + 1];
                }
                else
                {
                    index = index - 2048;
                    x_q[2 * m] = negate(Table_FFT16b_4096[2 * index]);
                    x_q[2 * m + 1] = negate(Table_FFT16b_4096[2 * index + 1]);
                }
            }
#else
            //void ul_Pilot_fx_part1(S16 Nrs_zc, S16 q, S16 Nrs_zcDiv, S16 expo, S16 x_q[]);
            ul_Pilot_fx_part1_A(Nrs_zc, q, Nrs_zcDiv, expo, x_q);
#endif
        }
        else
        {
#ifndef CEVA_INTRINSICS
            Phi_mat_ptr = NULL;
            if (Mrs_sc == pULPilotInput->Nrb_sc)
            {
                Phi_mat_ptr = Phi_mat;
                step = 12;
            }
            else if (Mrs_sc == 2 * pULPilotInput->Nrb_sc)
            {
                Phi_mat_ptr = Phi_mat_24;
                step = 24;
            }

            for (n = 0; n < Mrs_sc; n++)
            {
                //Phi_mat[n+u*Nrb_sc]*PI/4.0 = Phi_mat[n+u*Nrb_sc]*2*PI/8.0
                //	                         = 2*PI*512*Phi_mat[n+u*Nrb_sc]/4096.0
                //Note Table_FFT16b_4096 is exp(-j*2*PI/N)
                //Need to negate Sin result, when Phi_mat[n+u*Nrb_sc] is > 0
                L_temp1 = Phi_mat_ptr[n + u[slot] * step] * 512;
                sign = 1;
                if (Phi_mat_ptr[n + u[slot] * step] < 0)
                {
                    sign = 0;
                    L_temp1 = L_abs(L_temp1);
                }
                L_temp2 = 4096;

                if (L_temp1 > L_temp2)
                {
                    L_temp1 = L_abs(L_temp1) % L_temp2;
                }
                index = L_temp1;
                if (index < 2048)
                {
                    x_q[2 * n] = Table_FFT16b_4096[2 * index];
                    x_q[2 * n + 1] = Table_FFT16b_4096[2 * index + 1];
                    if (sign == 1)
                    {
                        x_q[2 * n + 1] = negate(x_q[2 * n + 1]);
                    }
                }
                else
                {
                    index = index - 2048;
                    x_q[2 * n] = negate(Table_FFT16b_4096[2 * index]);
                    x_q[2 * n + 1] = negate(Table_FFT16b_4096[2 * index + 1]);
                    if (sign == 1)
                    {
                        x_q[2 * n + 1] = negate(x_q[2 * n + 1]);
                    }
                }
            }
#else
            ul_Pilot_fx_part2_A(Mrs_sc, pULPilotInput->Nrb_sc, u[slot], x_q);
#endif
        }

        // Generate base sequence and
        // Apply Cyclic shift
        if (Mrs_sc >= 3 * pULPilotInput->Nrb_sc)
        {
#ifndef CEVA_INTRINSICS
            div_12 = 2731;
            for (n = 0; n < Nrs_zc; n++)
            {
                //  alpha[0] = 2.0 * PI * ncs[0] / 12.0;
                //  alpha[1] = 2.0 * PI * ncs[1] / 12.0;
                temp1 = (S16) (n * ncs[slot]);

                rbar_uvI = x_q[2 * n];
                rbar_uvQ = x_q[2 * n + 1];

                temp3 = mult(temp1, div_12);

                L_temp1 = temp1 - temp3 * 12;

                ctemp1 = W16t0012[2 * L_temp1];
                stemp1 = negate(W16t0012[2 * L_temp1 + 1]); // since table has exp(-j*2*PI)

                L_temp1 = L_mult(ctemp1, rbar_uvI);
                rPUSCH1 = msu_r(L_temp1, stemp1, rbar_uvQ);

                L_temp2 = L_mult(ctemp1, rbar_uvQ);
                rPUSCH2 = mac_r(L_temp2, stemp1, rbar_uvI);

                a[2 * n + slot * length_rPUSCH] = shr(rPUSCH1, 1);
                a[2 * n + slot * length_rPUSCH + 1] = shr(rPUSCH2, 1);
            }

            for (n = Nrs_zc; n < Mrs_sc; n++)
            {
                index = n - Nrs_zc;

                temp1 = (S16) (n * ncs[slot]);

                rbar_uvI = x_q[2 * index];
                rbar_uvQ = x_q[2 * index + 1];

                temp3 = mult(temp1, div_12);

                L_temp1 = temp1 - temp3 * 12;

                ctemp1 = W16t0012[2 * L_temp1];
                stemp1 = negate(W16t0012[2 * L_temp1 + 1]); // since table has exp(-j*2*PI)

                L_temp1 = L_mult(ctemp1, rbar_uvI);
                rPUSCH1 = msu_r(L_temp1, stemp1, rbar_uvQ);

                L_temp2 = L_mult(ctemp1, rbar_uvQ);
                rPUSCH2 = mac_r(L_temp2, stemp1, rbar_uvI);

                a[2 * n + slot * length_rPUSCH] = shr(rPUSCH1, 1);
                a[2 * n + slot * length_rPUSCH + 1] = shr(rPUSCH2, 1);
            }
#else
            //void ul_Pilot_fx_part3(S16 Nrs_zc, S16 Mrs_sc, S16 length_rPUSCH, S16 ncsSlot, S32 slot, S16 x_q[], S16 a[]);
            ul_Pilot_fx_part3_A(Nrs_zc, 0 * ncs[slot], length_rPUSCH, ncs[slot], slot, x_q, a);
            ul_Pilot_fx_part3_A((Mrs_sc - Nrs_zc), Nrs_zc * ncs[slot], length_rPUSCH, ncs[slot], slot, x_q, &a[2*Nrs_zc]);
#endif
        }
        else
        {
            div_12 = 2731;
            for (n = 0; n < Mrs_sc; n++)
            {
                //  alpha[0] = 2.0 * PI * ncs[0] / 12.0;
                //  alpha[1] = 2.0 * PI * ncs[1] / 12.0;
                temp1 = (S16) (n * ncs[slot]);

                rbar_uvI = x_q[2 * n];
                rbar_uvQ = x_q[2 * n + 1];

                temp3 = mult(temp1, div_12);

                L_temp1 = temp1 - temp3 * 12;

                ctemp1 = W16t0012[2 * L_temp1];
                stemp1 = negate(W16t0012[2 * L_temp1 + 1]); // since table has exp(-j*2*PI)

                L_temp1 = L_mult(ctemp1, rbar_uvI);
                rPUSCH1 = msu_r(L_temp1, stemp1, rbar_uvQ);

                L_temp2 = L_mult(ctemp1, rbar_uvQ);
                rPUSCH2 = mac_r(L_temp2, stemp1, rbar_uvI);

                a[2 * n + slot * length_rPUSCH] = shr(rPUSCH1, 1);
                a[2 * n + slot * length_rPUSCH + 1] = shr(rPUSCH2, 1);
            }
        }
    }//slot

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL LTE channel coding code block segmentation function.
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *   @param RBAssignment = array of ordered pairs (ResourceBlockNumber, NumberOfConsecutiveRBForThisUser) defining the resource blocks assigned to this transport block
 *   @param Msymb  = number of elements (complex modulation symbols) to be mapped into the resource block pair
 *   @return k_l  = array of (k,l) coordinates for consecutive complex modulation symbols
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name         |TCB Type       |Size         |Data Format    |Valid Range   |Default        |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |RBAssignment |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------------
 *  |Msymb        |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------------
 *  |k_l          |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------------
 *  |Block Name          |ULPilotResourceElementAssignment_fx |
 *  -----------------------------------------------------------
 *  |Inputs Stream       |DOXYGEN_TO_DO                       |
 *  -----------------------------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO                       |
 *  -----------------------------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO                       |
 *  -----------------------------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO                       |
 *  -----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  Uplink Pilots, Resource Element Assignment
 *  Determine the frequency_symbol index in the resource grid that each complex constellation
 *  point for this Pilot sequence will be mapped to.<BR><BR>
 *
 *  Pilot values are mapped in sequence starting with rPUSCH(0) to the same set of physical
 *  resource blocks used for the corresponding PUSCH transmission defined in [TS 36.211 Sec.5.3.4].
 *  The mapping to resource elements (k,l), with l=3 for normal cyclic prefix and l=2 for extended
 *  cyclic prefix, in the subframe shall be in increasing order of first k, then the slot number.<BR><BR>
 *
 *  <B>Notes:</B>
 *  -# Resource blocks assigned for transmitting this data contain resource elements that cannot be used
 *     for data, i.e. reference, sync and reserved subcarriers.
 *  -# Also, it is assumed the MAC layer has resolved all conflicts in resource block assignments between different
 *     physical channels. No conflicts are resolved here. Any such conflicts reaching the PHY would result in
 *     the blocks of data processed earlier being overwritten
 *  -# The number of complex modulated data values written to each antenna port is the same and the subcarriers available
 *     for the transmission of data is the same for each antenna. So this routine is independent of the number of
 *     antenna ports. The mapping of pilot and data
 *     carriers in the resource blocks is identical for both 1/2ms slots of a 1ms subframe.
 *  -# The subcarriers not available for data transmission are those used for; reference, sync, reserved.
 *
 * <5> Cycle Count Formula:
 * DOXYGEN_TO_DO
 *
 * <6> References:
 * DOXYGEN_TO_DO
 *
 * \ingroup DOXYGEN_TO_DO
 *
 **/
//-------------------------------------------------------------------------------------------
#ifndef CEVA_INTRINSICS

void ULPilotResourceElementAssignment_fx(S16 *RBAssignment, ULRXINPUTPARAMS *ULPilotInput, S16 Msymb,
                                         S16 *k_l_ul_row0, S16 *k_l_ul_row1, S16 *numrows_k_l,
                                         S16 *numcols_k_l)
{
    S16 kIndex, lIndex;
    S32 k;
    S16 k0;
    S32 row;
    S16 kIndexRange[12]; // since max entry in NumREinSubfrmSym_PDSCH is 12
    S16 length_kIndexRange, *kIndexRange_ptr;
    S16 nPRB, NumRBs;
    S32 n;
    S32 islot;
    S16 NUM = 0;

    //printf("ULP_INFO %hd %hd %hd %hd %hd %hd\n", pilotCarrierSymbol, Nrb_sc, Msymb, Nul_symb, ROW, COL);

    //init loop counters and pointers
    kIndex = 0; //initial frequency index
    for (k = 0; k < ULPilotInput->Nrb_sc; k++)
        kIndexRange[k] = (S16) k;

    length_kIndexRange = ULPilotInput->Nrb_sc;
    for (islot = 0; islot < 2; islot++)
    {
        lIndex = (S16) (islot * ULPilotInput->Nul_symb + ULPilotInput->pilotCarrierSymbol); //pilots are always
        //sent on one symbol of a slot
        for (row = 0; row < 1; row++)
        {
            nPRB = RBAssignment[2 * row];
            NumRBs = RBAssignment[2 * row + 1];

            //resource blocks in this assignment
            for (n = 0; n < NumRBs; n++)
            {
                //Pilots use all Resource Elements of this symbol
                k0 = nPRB * ULPilotInput->Nrb_sc; // frequency index for first resource element
                // in the first resource block of this resource
                // assignment ordered pair
                kIndexRange_ptr = &kIndexRange[0];
                for (k = kIndex; k <= kIndex + kIndexRange[length_kIndexRange - 1]; k++)
                {
                    k_l_ul_row0[k] = k0 + (*kIndexRange_ptr++);
                    k_l_ul_row1[k] = lIndex;
                    NUM++;
                }
                kIndex = kIndex + length_kIndexRange;
                nPRB = nPRB + 1; //increment resource block number
            } //for n loop
        } // for row loop
    } // for islot loop

    *numrows_k_l = 2; // For now it is always 2.
    *numcols_k_l = NUM;

    return;
}

//------------------------------------------------------------------------------------------------
// !!!!!!!!!!!!!! TEMPORARY. NOW PREFER TO DEFINE a() AS a.symbol().element() INSTEAD OF MULTIDIMENSIONAL ARRAY.
// !!!!!!!!!!!!!! WHEN UPDATE BS_TX, REPLACE THE FUNCTION IN THE STANDALONE FILE WITH THIS ONE...
//------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL LTE channel coding code block segmentation function.
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *   @param a  = resource element grid for this antenna, possibly already partially completed with precoded data symbol values from other physical channels<BR>
 *             = a(0,0),a(0,1),a(0,2),...,a(0,L-1)<BR>
 *               a(1,0),a(1,1),a(1,2),...,a(1,L-1)<BR>
 *               ...<BR>
 *               a(K-1,0),a(K-1,1),...,a(K-1,L-1)<BR>
 *   @param y  = new data symbol values, to be added<BR>
 *             = y(0),y(1),...,y(Msymb-1)
 *   @param k_l    = array of (k,l) coordinates for consecutive complex modulation symbols
 *   @return a  = updated resource element grid for this antenna, updated with values of y, at locations k,l defined by k_l
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------
 *  |Name   |TCB Type       |Size         |Data Format    |Valid Range   |Default        |Units        |
 *  ----------------------------------------------------------------------------------------------------
 *  |a      |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  |y      |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  |k_l    |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |UL_ResourceElementMapper_fx |
 *  ---------------------------------------------------
 *  |Inputs Stream       |DOXYGEN_TO_DO               |
 *  ---------------------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO               |
 *  ---------------------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO               |
 *  ---------------------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO               |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * Mapping of Shared Channel Data symbols to Resource Elements:-
 *
 * <5> Cycle Count Formula:
 * DOXYGEN_TO_DO
 *
 * <6> References:
 * DOXYGEN_TO_DO
 *
 * \ingroup DOXYGEN_TO_DO
 *
 **/
//-------------------------------------------------------------------------------------------
void UL_ResourceElementMapper_fx(S16 *a, S16 *y, S16 *k_l_ul_row0, S16 *k_l_ul_row1,
                                 S16 numrows_k_l, S16 numcols_k_l, S16 numcols_a, S16 length_y)
{
    S16 Map_symb;
    S32 k, l, m;

    //init loop counters and pointers
    Map_symb = length_y;

#ifdef _WIN32
    //sanity check of array sizes
    if (numcols_k_l != Map_symb)
    phy_printf("error in ResourceElementMapper(): number of elements in y (Map_symb) does not match number of elements in k_l");
#endif

    //loop to copy precoded data symbol values to resource elements, at locations defined by the k_l matrix
    for (m = 0; m < Map_symb; m++)
    {
        k = k_l_ul_row0[m]; //get frequency index, k
        l = k_l_ul_row1[m]; //get time index, l
        a[2 * l * numcols_a + 2 * k] = y[2 * m]; //write to resource element (Real)
        a[2 * l * numcols_a + 2 * k + 1] = y[2 * m + 1]; //write to resource element (Imag)
    }
    return;
}

#endif

//-------------------------------------------------------------------------------------------
/** @brief Config Channel parameters for Uplink Pilots
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *  @param Ncell_id     - Cell_ID for current active PHY
 *  @param delta_ss     - delta_ss parameter for current active PHY
 *  @param groupHopping - Group Hopping flag
 *  @param Nul_symb     - Number of Uplink symbols per slot
 *  @param Mpusch_sc    - Number of subcarriers used
 *  @param Nrb_sc       - Number of subcarriers per resource block
 *  @param cvec_u       - PseudoRandomGen Sequence for group hopping
 *  @param u            - Group hopping number
 *  @param PRSbits      - PseudoRandomGen Sequence for Sequence hopping
 *  @param v            - Sequence hopping number
 *  @param nPRS         - Psuedo Random Sequence used to determine Pilots
 *  @param bitRevTable  - Table to bit reverse 8-bit numbers
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction |Length  |Type         |Data Format |Valid Range |Units  |
 *  |             |Standard |          |        |             |            |            |       |
 *  ---------------------------------------------------------------------------------------------
 *  |Ncell_id     |         |Input     |1       |short        |(1:15:0)    |Full Range  |       |
 *  ---------------------------------------------------------------------------------------------
 *  |delta_ss     |         |Input     |1       |short        |(1:15:0)    |Full Range  |       |
 *  ---------------------------------------------------------------------------------------------
 *  |groupHopping |         |Input     |1       |short        |(1:15:0)    |Full Range  |       |
 *  ---------------------------------------------------------------------------------------------
 *  |Nul_symb     |         |Input     |1       |short        |(1:15:0)    |Full Range  |       |
 *  ---------------------------------------------------------------------------------------------
 *  |Mpusch_sc    |         |Input     |1       |short        |(1:15:0)    |Full Range  |       |
 *  ---------------------------------------------------------------------------------------------
 *  |cvec_u       |         |pInput    |140     |unsigned byte|(0:8:0)     |Full Range  |       |
 *  ---------------------------------------------------------------------------------------------
 *  |u            |         |pOutput   |20      |unsigned byte|(0:8:0)     |Full Range  |       |
 *  ---------------------------------------------------------------------------------------------
 *  |PRSbits      |         |pInput    |140     |unsigned byte|(0:8:0)     |Full Range  |       |
 *  ---------------------------------------------------------------------------------------------
 *  |v            |         |pOutput   |20      |unsigned byte|(0:8:0)     |Full Range  |       |
 *  ---------------------------------------------------------------------------------------------
 *  |nPRS         |         |pOutput   |20      |short        |(1:15:0)    |Full Range  |       |
 *  ---------------------------------------------------------------------------------------------
 *  |bitRevTable  |         |pInput    |256     |short        |(1:15:0)    |Full Range  |       |
 *  ---------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------
 *  |Block Name          |ul_ConfigPilots_Pusch          |
 *  ------------------------------------------------------
 *  |Input Streams       |cvec_u,PRSbits,bitRevTable     |
 *  ------------------------------------------------------
 *  |Output Streams      |nPRS,u,v                       |
 *  ------------------------------------------------------
 *  |Config Parameters   |Ncell_id,delta_ss,groupHopping,|
 *  |                    |sequenceHoping, Nul_symb,      |
 *  |                    |Mpusch_sc,Nrb_sc               |
 *  ------------------------------------------------------
 *  |Status Parameters   |none                           |
 *  ------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Config Channel parameters for Uplink Pilots. <BR>
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Uplink"<BR>
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
//-------------------------------------------------------------------------------------------
void ul_ConfigPilots_Pusch(ULRXINPUTPARAMS *ULPilotInput, U8 *cvec_u, S16 *u,
                           U8 *PRSbits, S16 *v, S16 *nPRS, S16 *bitRevTable, S32 ChanNum)
{
    S32 islot, index, nul_symb;
    S16 fPUSCH_ss, div_30;
    S16 temp, temp1, temp2, bit_rev;
    S16 slotNum;
#ifdef CEVA_INTRINSICS
    S16 *fgh;

    fgh = (S16*)(&DataBuff_Bank1[0]);
#else
    S16 fgh[20];
#endif
    //
    //Generate Pilot subcarrier random number sequence for each PUSCH [3GPP TS 36.211 v.8.5.0, Sec.5.5.2.1.1]
    //
    //(This sequence depends on the Cell ID and PUSCH id)
    //delta_ss = integer from set {0,1,...,29}, configured by higher layers
    //init pseudorandom seq generator at beginning of radio frame

    fPUSCH_ss = (ULPilotInput->Ncell_id + ULPilotInput->delta_ss) % 30;
    nul_symb = ULPilotInput->Nul_symb;
    slotNum = ULPilotInput->slotNum;

    // Generate Random sequence for entire frame of length 8*7*20
    memset(fgh, 0, 20 * 2);
    memset(v, 0, 20 * 2);
    temp = 6 * ULPilotInput->Nrb_sc;
    index = slotNum & 0x7;
    div_30 = 1093;
    if (ULPilotInput->groupHoppingEnabled == 1) //Group Hopping is enabled
    {
        for (islot = slotNum; islot < (slotNum + 2); islot++)
        {
            bit_rev = bitRevTable[cvec_u[islot]];
            temp1 = mult(bit_rev, div_30);
            fgh[islot] = bit_rev - temp1 * 30; //fgh[islot] = bit_rev % 30;
        }
    }
    else
    {
        for (islot = slotNum; islot < (slotNum + 2); islot++)
        {
            //Group Hopping is disabled && sequenceHopping enabled
            if ((ULPilotInput->sequenceHoppingEnabled == 1) && (ULPilotInput->Mpusch_sc[ChanNum] >= temp))
            {
                bit_rev = bitRevTable[PRSbits[islot >> 3]];
                v[islot] = ((bit_rev) >> index) & 0x1;
                index++;
                index = index & 0x7;
            }
        }
    }

    for (islot = slotNum; islot < (slotNum + 2); islot++)
    {
        temp1 = (fgh[islot] + fPUSCH_ss);
        temp2 = mult(temp1, div_30);
        u[islot] = temp1 - temp2 * 30; //u[islot] = (fgh[islot] + fPUSCH_ss) % 30;

        bit_rev = bitRevTable[PRSbits[islot * nul_symb]];
        nPRS[islot] = bit_rev;
    }
}
