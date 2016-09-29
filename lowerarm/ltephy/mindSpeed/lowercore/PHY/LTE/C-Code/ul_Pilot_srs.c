//-------------------------------------------------------------------------------------------
/** @file ul_Pilot_srs.c
 *
 * @brief Add Uplink Reference (pilot) signals to the resource grid
 *           Generate Uplink SRS (sounding reference signals)
 * @author Mindspeed Technologies
 * @version $Revision: 1.22 $
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

extern U8 *DataBuff_Bank2;

#else

#include "basic_op_cc.h"

#endif

//-------------------------------------------------------------------------------------------
/** @brief Generate Uplink SRS (sounding reference signals) [3GPP TS 36.211 Rel-8, Sec.5.5.3]
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *
 *  @param   Nrb_sc               - number of resource elements in a resource block
 *  @param   groupHoppingEnabled  - Group hopping enable flag
 *  @param   m_srs                - m_srs value from srsBandwidthConfigTable
 *  @param   nCs_srs              - This UE cyclic shift
 *  @param   *u                   - PUCCH sequence-group number
 *  @param   *rSrs                - reference pilots for srs
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------
 *  |Name               |Name in  |Direction |Length  |Type          |Data Format |Valid Range |Units |
 *  |                   |Standard |          |        |              |            |            |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |Nrb_sc             |         |Input     |1       |short         |(1:15:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |groupHoppingEnabled|         |Input     |1       |short         |(1:15:0)    |[0,1]       |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |m_srs              |         |Input     |1       |short         |(1:15:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |nCs_srs            |         |Input     |1       |short         |(1:15:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |*u                 |         |pInput    |20      |short         |(1:15:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  |*rSrs              |         |pOutput   |Mrs_sc  |pComplex Array|(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------
 *  |Block Name          |gen_ul_Pilot_srs_info      |
 *  --------------------------------------------------
 *  |Input Stream        |none                       |
 *  --------------------------------------------------
 *  |Output Stream       |u, rSrs                    |
 *  --------------------------------------------------
 *  |Config Parameters   |Nrb_sc,groupHoppingEnabled,|
 *  |                    |m_srs,nCs_srs            |
 *  --------------------------------------------------
 *  |Status Parameters   |none                       |
 *  --------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function generates Uplink SRS (sounding reference signals).
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 * 3GPP TS 36.211 Rel-8, Sec.5.5.3]
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
void gen_ul_Pilot_srs_info(S16 Nrb_sc, S16 m_srs, S16 nCs_srs,
                           S16 u[], S16 v[], S16 *rSrs, S16 pUlPilots_QthRootTable[])
{
    S16 m, n, ns;
    S16 temp1;
    S32 L_temp1, L_temp2, L_temp3;
    S16 ctemp1, stemp1;
    S16 sign;
    S8 *Phi_mat_ptr;

    S16 Mrs_sc = 0, Nrs_zc = 0;
    S16 index, q;
    S16 Nrs_zcDiv;
    S32 primeNum_index;

    S16 expo;
    S16 nx;

#ifndef CEVA_INTRINSICS
    S16 x_q[2 * MAXSUBCARRIERS];
#else
    S16 *x_q;
    x_q = (S16*)(DataBuff_Bank2);
#endif

    Mrs_sc = m_srs * Nrb_sc / 2; //Mrs_sc is always an integer multiple of Nrb_sc

    Phi_mat_ptr = NULL;
    if (Mrs_sc == Nrb_sc)
    {
        Phi_mat_ptr = Phi_mat;
    }
    else if (Mrs_sc == 2 * Nrb_sc)
    {
        Phi_mat_ptr = Phi_mat_24;
    }

    for (ns = 1; ns < 2; ns++)
    {
        if (Mrs_sc <= 2 * Nrb_sc)
        {
            L_temp2 = 4096;
            for (n = 0; n < Mrs_sc; n++)
            {
                //Phi_mat[n+u*Nrb_sc]*PI/4.0 = 512*Phi_mat[n+u*Nrb_sc]*2*PI/4096.0
                //Note Tab_Cos_Sin is exp(-j*2*PI/N)
                //Need to negate Sin result, when Phi_mat[n+u*Nrb_sc] is > 0
                L_temp1 = 512*Phi_mat_ptr[n + u[ns] * Mrs_sc];
                sign = 1;

                if (Phi_mat_ptr[n + u[ns] * Mrs_sc] < 0)
                {
                    sign = 0;
                    L_temp1 = L_abs(L_temp1);
                }

                if (L_temp1 >= L_temp2)
                {
                    L_temp1 = L_abs(L_temp1) % L_temp2;
                }

                index = (S16) (L_temp1);
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
                    x_q[2 * n + 1] = Table_FFT16b_4096[2 * index + 1];
                    if (sign == 0)
                    {
                        x_q[2 * n + 1] = negate(x_q[2 * n + 1]);
                    }
                }
            }
        }
        else //Mrs_sc >= 3*Nrb_sc
        {
            primeNum_index = 0;
            while (CA_PrimeNumberTable[primeNum_index] < Mrs_sc)
                   primeNum_index++;

            if (primeNum_index)
              primeNum_index = primeNum_index - 1;

            Nrs_zc = CA_PrimeNumberTable[primeNum_index];
            Nrs_zcDiv = UlPilots_Div_Nrs_zc_Table[2 * primeNum_index];
            expo = UlPilots_Div_Nrs_zc_Table[2 * primeNum_index + 1];

            expo = expo + 2;
            L_temp1 = (2 * primeNum_index * MAX_ULPILOTS_U + u[ns] + v[ns] * MAX_ULPILOTS_U);
            q = pUlPilots_QthRootTable[L_temp1];

            //Generate reference signals for this PUSCH in this 1ms subframe
            //Generate qth root Zadoff Chu sequence, length = prime number.
            L_temp2 = 2 * Nrs_zc;
            for (m = 0; m < Nrs_zc; m++)
            {
                //cos( PI * q * m * (m+1) / (1.0*Nrs_zc) =
                //cos( 2*PI * q * m * (m+1) / (2.0*Nrs_zc)
                L_temp1 = q * m * (m + 1);
                if (L_temp1 >= L_temp2)
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
        }

        if (Mrs_sc <= 2 * Nrb_sc)
        {
            // Apply Cyclic shift
            for (n = 0; n < Mrs_sc; n++)
            {
                //  alpha[0] = 2.0 * PI * nCsrs / 8.0;
                //  alpha[1] = 2.0 * PI * nCsrs / 8.0;
                temp1 = n * nCs_srs;
                if (temp1 >= 8)
                {
                    temp1 = temp1 % 8;
                }
                ctemp1 = W16t0008[2 * temp1];
                stemp1 = negate(W16t0008[2 * temp1 + 1]); // since table has exp(-j*2*PI)
                rSrs[2 * n] = sub(mult(ctemp1, x_q[2 * n]),
                                          mult(stemp1, x_q[2 * n + 1])); // Real
                rSrs[2 * n + 1] = add(mult(ctemp1, x_q[2 * n + 1]),
                                            mult(stemp1, x_q[2 * n])); // Imag
           }// n (krange)
        }
        else
        {
            // Apply Cyclic shift
            for (n = 0; n < Mrs_sc; n++)
            {
            //  alpha[0] = 2.0 * PI * nCsrs / 8.0;
            //  alpha[1] = 2.0 * PI * nCsrs / 8.0;
            nx = n % Nrs_zc; //...formed by cyclic extension
            // of shorter length Zadoff Chu sequence
            temp1 = n * nCs_srs;
            if (temp1 >= 8)
            {
                temp1 = temp1 % 8;
            }
            ctemp1 = W16t0008[2 * temp1];
            stemp1 = negate(W16t0008[2 * temp1 + 1]); // since table has exp(-j*2*PI)
            rSrs[2 * n] = sub(mult(ctemp1, x_q[2 * nx]),
                                      mult(stemp1, x_q[2 * nx + 1])); // Real
            rSrs[2 * n + 1] = add(mult(ctemp1, x_q[2 * nx + 1]),
                                        mult(stemp1, x_q[2 * nx])); // Imag
            }// n (krange)
        }
    }// ns

    for (n = 0; n < 2 * Mrs_sc; n++)
    {
        rSrs[n] = shr(rSrs[n], 1);
        //in Derotation operation
    }

    return;
}

