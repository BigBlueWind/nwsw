//-------------------------------------------------------------------------------------------
/** @file prach_util.c
 *
 * @brief LTE PHY Random Access Channel Utilities
 * @author Mindspeed Technologies
 * @version $Revision: 1.13 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#if !defined _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<math.h>

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
#include "basic_op_cc.h"

//-------------------------------------------------------------------------------------------
/** @brief ComputePrachEvec:  Do FFT, to get 839 frequency subcarriers {z}, for DFT-spread cazac sequence
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param start - INPUT, starting location
 *  @param end - INPUT, ending location
 *  @param phi -   INPUT, subcSpacing
 *  @param K -   INPUT, subcOffset
 *  @param k0 -   INPUT, PRACH Parameter
 *  @param N -   INPUT, length of output stream
 *  @param eVec -   OUTPUT, FFT output
 *
 *  @return (None)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |start        |         |Input         |1        |Real          |(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |end          |         |Input         |1        |Real          |(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |phi          |         |Input         |1        |Real          |(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |K            |         |Input         |1        |Real          |(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |K0           |         |Input         |1        |Real          |(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |N            |         |Input         |1        |Real          |(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |eVec         |         |Output        |1        |pComplex      |(1:0:15)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |return       |         |              |        |               |            |           |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |ComputePrachEvec                     |
 *  ------------------------------------------------------------
 *  |Input Streams       |                                     |
 *  ------------------------------------------------------------
 *  |Output Streams      |eVec                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |start, end, phi, K, K0, N            |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  ComputePrachEvec:  Do FFT, to get 839 frequency subcarriers {z}, for DFT-spread cazac sequence
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
void ComputePrachEvec(S16 start, S16 end, S16 phi, S16 K, S16 k0, S16 N, S16 *eVec)
{
    S32 L_temp1;
    S16 temp1, temp2, temp;
    S16 multiplier;
    S32 i;
    S16 index;
    S16 kb;

    //Compute phi+K*(k0+1/2)
    L_temp1 = L_mult(K, k0);
    temp1 = extract_l(L_shr(L_temp1, 1));
    temp2 = shr(K, 1); // K is always even ( 12 (FDD) or 2 (TDD))
    temp = add(phi, add(temp1, temp2));

    multiplier = divide_floor(N, 4096); // N is a multiple of 4096

    // cos(-2*PI*(phi+K*(k0+1/2))/N) == cos(-2*PI*(phi+K*(k0+1/2))/(multiplier*4096)
    for (i = 0; i < end - start; i++)
    {
        //L_temp1 = L_mult(i+start,temp1);
        //L_temp1 = L_shr(L_temp1, 1);
        L_temp1 = L_shr(L_mult(temp, (S16) (i + start)), (S16) (1)); // extra shift to undo
        // the extra shift in L_mult

        //Map index to the proper entry depending on 4096
        index = (S16) (L_abs(L_temp1) % N);
        index = mult_r(index, invTab[multiplier - 1]);

        kb = index;
        if (kb < 2048)
        {
            eVec[2 * i] = Table_FFT16b_4096[2 * kb];
            if (L_temp1 >= 0)
            {
                eVec[2 * i + 1] = Table_FFT16b_4096[2 * kb + 1];
            }
            else
            {
                eVec[2 * i + 1] = negate(Table_FFT16b_4096[2 * kb + 1]);
            }
            // printf("eEvecL %f %f \n", eEvec[2*i]/32768.0, eEvec[2*i+1]/32768.0);
        }
        else
        {
            //			kb = MMIN(4096-kb,2047);
            temp1 = 4096 - kb;
            kb = 2047;
            if (temp1 < kb)
            {
                kb = temp1;
            }

            eVec[2 * i] = Table_FFT16b_4096[2 * kb];

            if (L_temp1 >= 0)
            {
                eVec[2 * i + 1] = negate(Table_FFT16b_4096[2 * kb + 1]);
            }
            else
            {
                eVec[2 * i + 1] = Table_FFT16b_4096[2 * kb + 1];
            }

            // printf("eEvecH%f %f \n", eEvec[2*i]/32768.0, eEvec[2*i+1]/32768.0);
        }
        // getchar();
    }
}

//-------------------------------------------------------------------------------------------
/** @brief ComputePrach_x_u:  Locally generates cazac root sequence
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param u - INPUT, u'th root
 *  @param Nzc -   INPUT, length of output sequence
 *  @param x_u -   OUTPUT, CAZAC root sequence
 *
 *  @return (None)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |u            |         |Input         |1        |Real          |(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |Nzc          |         |Input         |1        |Real          |(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |x_u          |         |Output        |1        |pComplex      |(1:0:15)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |return       |         |              |        |               |            |           |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |ComputePrach_x_u                     |
 *  ------------------------------------------------------------
 *  |Input Streams       |                                     |
 *  ------------------------------------------------------------
 *  |Output Streams      |x_u                                  |
 *  ------------------------------------------------------------
 *  |Config Parameters   |u, Nzc                               |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  ComputePrach_x_u:  Locally generates cazac root sequence, given the u^th root
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
void ComputePrach_x_u(S16 u, S16 Nzc, S16 x_u[])
{
    S16 n;
    S32 temp1;

    for (n = 0; n < Nzc; n++) // Nzc is 839 for FDD
    {
        temp1 = (u * n * (n + 1)) / 2;
        if ((u * n * (n + 1)) % 2 != 0)
        {
            printf("WARNING ODD \n");
        }
        if (temp1 >= Nzc)
        {
            temp1 = temp1 % Nzc;
        }
        x_u[2 * n] = W16t0839[2 * temp1]; //exp(-2*j*pi*k/N)
        x_u[2 * n + 1] = W16t0839[2 * temp1 + 1];
    }
}

//-------------------------------------------------------------------------------------------
/** @brief ComputePrach_IDFT_839:  Special IDFT computation
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param k - INPUT, index
 *  @param Nzc -   INPUT, length of output sequence
 *  @param eVec -   OUTPUT, IDFT sequence
 *
 *  @return (None)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |u            |         |Input         |1        |Real          |(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |Nzc          |         |Input         |1        |Real          |(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |eVec         |         |Output        |1        |pComplex      |(1:0:15)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |return       |         |              |        |               |            |           |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |ComputePrach_IDFT_839                |
 *  ------------------------------------------------------------
 *  |Input Streams       |                                     |
 *  ------------------------------------------------------------
 *  |Output Streams      |eVec                                 |
 *  ------------------------------------------------------------
 *  |Config Parameters   |k, Nzc                               |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  ComputePrach_IDFT_839:  Special IDFT computation
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
void ComputePrach_IDFT_839(S16 k, S16 Nzc, S16 eVec[])
{
    S16 n;
    S32 temp1;

    for (n = 0; n < Nzc; n++) // Nzc is 839 for FDD
    {
        temp1 = k * n;
        if (temp1 >= Nzc)
        {
            temp1 = temp1 % Nzc;
        }
        eVec[2 * n] = W16t0839[2 * temp1]; // exp(2*j*pi*k/N)
        eVec[2 * n + 1] = negate(W16t0839[2 * temp1 + 1]);
    }
}

//-------------------------------------------------------------------------------------------
/** @brief FirDownsample:  FIR filter, with downsampling of the output rate
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param in           - INPUT, input data stream
 *  @param firLpfMid6   - INPUT, FIR LowPass filter coefficients
 *  @param USR          - INPUT, undersampling rate
 *  @param delay        - INPUT, delay
 *  @param L            - INPUT, length of input stream
 *  @param Nf           - INPUT, length of FIR filter
 *  @param outd         - OUTPUT, output stream
 *  @param length_outd  - OUTPUT, length of output stream
 *
 *  @return (None)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |in           |         |Input         |        |pComplex       |(1:0:15)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |firLpMid6    |         |Input         |        |pReal          |(1:0:15)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |USR          |         |Input         |        |Real           |(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |delay        |         |Input         |        |Real           |(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |L            |         |Input         |        |Real           |(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |Nf           |         |Input         |        |Real           |(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |outd         |         |Output        |        |pComplex       |(1:0:15)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |length_outd  |         |Output        |        |Real           |(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |return       |         |              |        |               |            |           |       |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |FirDownsample                        |
 *  ------------------------------------------------------------
 *  |Input Streams       |in, firLpfMid6                       |
 *  ------------------------------------------------------------
 *  |Output Streams      |outd,                                |
 *  ------------------------------------------------------------
 *  |Config Parameters   |USR, delay, L, Nf, length_outd       |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  FirDownsample:  Downsamples input stream given the downsampling rate.
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
void FirDownsample(S16 in[], S16 firLpfMid6[], S16 USR, S16 delay, S32 L, S16 Nf, S16 outd[],
                   S16 *length_outd)
{
    S32 i, k, n, N;
    S16 nsub;
    S16 expo_norm;
    S16 delsam, delsamOut;
    S16 *in_d, *temp_vec, *out;
    S32 L_temp1, L_temp2;

    //init input array and pointers to normalize for zero filtering delay
    delsam = delay % USR;

    //    in_d = svector(0, 2 * (Nf + L - delsam) - 1);
    //    temp_vec = svector(0, 2 * (Nf + L - delsam) - 1);
    //    out = svector(0, 2 * (Nf + L - delsam) - 1);

    in_d = MemAlloc(2 * (Nf + L - delsam) * sizeof(S16), DDR_MEM, 1);
    temp_vec = MemAlloc(2 * (Nf + L - delsam) * sizeof(S16), DDR_MEM, 1);
    out = MemAlloc(2 * (Nf + L - delsam) * sizeof(S16), DDR_MEM, 1);

    N = Nf + L - delsam;
    for (i = 0; i < 2 * (L - delsam); i++)
    {
        in_d[i] = in[i + 2 * delsam]; //remove some input samples to align delay
    }
    expo_norm = block_norm(in_d, temp_vec, 2 * N, 0);

    //do initial filtering while energy is building up in the delay line
    nsub = 0;
    for (n = 0; n < Nf; n += USR)
    {
        L_temp1 = 0;
        L_temp2 = 0;
        //complex samples * real filter
        for (k = 0; k <= n; k++)
        {
            L_temp1 = L_mac(L_temp1, temp_vec[2 * (n - k)], firLpfMid6[k]);
            L_temp2 = L_mac(L_temp2, temp_vec[2 * (n - k) + 1], firLpfMid6[k]);
        }
        out[2 * nsub] = round_s(L_shr(L_temp1, expo_norm));
        out[2 * nsub + 1] = round_s(L_shr(L_temp2, expo_norm));
        // printf("SUB %hd %f %f\n", nsub, out[2*nsub]/8192.0, out[2*nsub+1]/8192.0);
        nsub += 1;
    }

    //continue full length filtering
    for (n = Nf; n < N; n += USR)
    {
        L_temp1 = 0;
        L_temp2 = 0;
        //complex samples * real filter
        for (k = 0; k < Nf; k++)
        {
            L_temp1 = L_mac(L_temp1, temp_vec[2 * (n - k)], firLpfMid6[k]);
            L_temp2 = L_mac(L_temp2, temp_vec[2 * (n - k) + 1], firLpfMid6[k]);
        }
        out[2 * nsub] = round_s(L_shr(L_temp1, expo_norm));
        out[2 * nsub + 1] = round_s(L_shr(L_temp2, expo_norm));
        // printf("SUB %hd %f %f\n", nsub, out[2*nsub]/8192.0, out[2*nsub+1]/8192.0);
        nsub += 1;
    }

    //finish deleting samples to normalize delay!
    delsamOut = divide_floor(delay, USR); // //floor(delay/USR);
    for (i = 0; i < 2 * (nsub - delsamOut); i++)
    {
        outd[i] = out[i + 2 * delsamOut]; //remove some output samples to align delay
    }
    *length_outd = nsub - delsamOut;

    //    free_svector(in_d, 0, 2 * (Nf + L - delsam) - 1);
    //    free_svector(temp_vec, 0,2 * (Nf + L - delsam) - 1);
    //    free_svector(out, 0, 2 * (Nf + L - delsam) - 1);
    MemFree(in_d, DDR_MEM);
    MemFree(temp_vec, DDR_MEM);
    MemFree(out, DDR_MEM);

    //for ( i = 0; i < (nsub - delsamOut); i++)
    //printf("OUTD %f %f\n", outd[2*i]/8192.0, outd[2*i+1]/8192.0);
}

