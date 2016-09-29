//-------------------------------------------------------------------------------------------
/** @file PseudoRandomSeqGen.c
 *
 * @brief LTE BS Transmitter, Internal Physical Channel processing from input
 *        codewords to output resource elements
 * @author Mindspeed Technologies
 * @version $Revision: 1.28 $
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
//#include "heaps.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
extern U32 PseudoRandomSeqGen_Part1_A(U32 cinit);
extern void PseudoRandomSeqGen_Part2_A(U32 X1, U32 X2, S32 N, U8 *c);
#else
#include "basic_op_cc.h"
#endif

U32 X1Tbl[16] =
{ 0, 0x80000000, 0, 0x80000000, 0, 0x80000000, 0, 0x80000000, 0x80000000, 0, 0x80000000, 0,
  0x80000000, 0, 0x80000000, 0 };
U32 X2Tbl[16] =
{ 0, 0x80000000, 0x80000000, 0, 0x80000000, 0, 0, 0x80000000, 0x80000000, 0, 0, 0x80000000, 0,
  0x80000000, 0x80000000, 0 };

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL Implements a Length 31 Gold Code pseudo random
 *         sequence generator per 3GPP TS 36.211 Sec.7.2
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *  @param N      Number of output bits to generate
 *  @param cinit  Initialization value
 *  @param c      Output Bitstream
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------
 *  |Name   |TCB Type       |Size         |Data Format    |Valid Range   |Default        |Units        |
 *  ----------------------------------------------------------------------------------------------------
 *  |N      |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  |cinit  |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  |c      |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  |nSymb  |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  |numRb  |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |PseudoRandomSeqGen          |
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
 *  This function implements a Length 31 Gold Code pseudo random
 *  sequence generator per 3GPP TS 36.211 Sec.7.2
 *
 * <B>Input Parameters:</B>
 * - N   = number of output bits to generate
 * - ScrX2 = 2nd Gold code sequence
 *
 * <B>Outputs:</B>
 * - c  = c(0), c(1), ..., c(N-1)  = output bitstream, c(0) first in time
 *
 * <B>Note:</B><BR>
 * This function will be considered part of the FEC block since the input and
 * output are still bitstream memory format. In fact, for WiMAX and LTE,
 * should put everything up to the constellation mapper into the fec block.
 * So the output moved to the DSP modulator code is in terms of the 16-bit
 * complex values, not bitstream values!<BR>
 *
 * <B>Shift Register Implementation:</B><BR>
 * Where the pseudo random sequence c(n) is given by [3GPP TS 36.211 Sec.7.2].
 * i.e.= length-31 Gold sequence. The output sequence c(n) of length N, where
 *  n=0,1,2,...,N-1,<BR>
 * is defined by<BR>
 *  -      c(n) = (x1(n) + x2(n))mod2
 *  -  x1(n+31) = (x1(n+3) + x1(n))mod2
 *  -  x2(n+31) = (x2(n+3) + x2(i+2) + x2(n+1) + x2(n))mod2
 * with initial conditions;
 *  -  x1(0)=1, x1(n)=0 for n=1,2,...,30   %always =1, for all applications
 *  -  x2 = x2(0), x2(1), ..., x2(30)      %set depending on the application of
 *   the sequence (scrambler, reference signals, ...)
 *
 * <5> Cycle Count Formula:
 * DOXYGEN_TO_DO
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Sec.7.2
 *
 *  \ingroup group_lte_utilities
 *
 **/
//-------------------------------------------------------------------------------------------
void PseudoRandomSeqGen(S32 N, U32 cinit, U8 *c)
{

    U32 X1, X2;
#ifndef CEVA_INTRINSICS
	U32 X2x, X2a, X2b, X2c, X2d;
    U32 parity;
    S32 n;
    S32 Nhalf;
    S32 X1A, X1B, X2A = 0, X2B;
    S32 X1C, X1D, X2C, X2D;

    X2 = 0;
    for (n = 0; n < 31; n++)
    {
        X2x = cinit & PsuedoRandomGenInitValue[n];

        X2a = (X2x) & 0xFF;
        X2b = (X2x >> 8) & 0xFF;
        X2c = (X2x >> 16) & 0xFF;
        X2d = (X2x >> 24) & 0xFF;

        parity = PsuedoRandomGenParityTable[X2a];
        parity += PsuedoRandomGenParityTable[X2b];
        parity += PsuedoRandomGenParityTable[X2c];
        parity += PsuedoRandomGenParityTable[X2d];

        parity = (parity) & 0x1;

        X2 = X2 | (parity << n);
    }
#else
    X2 = PseudoRandomSeqGen_Part1_A(cinit);
#endif

    //Pre-Calculate X1 since it is always the same
    X1 = 1581799488;

#ifndef CEVA_INTRINSICS
    Nhalf = N >> 2;
    for (n = 0; n < N; n += 4)
    {
        X1A = X1 & 0x0000000F;
        X1B = (X1 >> 1) & 0x0000000F;
        X1C = (X1 >> 2) & 0x0000000F;
        X1D = (X1 >> 3) & 0x0000000F;
        X2A = X2 & 0x0000000F;
        X2B = (X2 >> 1) & 0x0000000F;
        X2C = (X2 >> 2) & 0x0000000F;
        X2D = (X2 >> 3) & 0x0000000F;

        c[n] = (U8) ((X1 ^ X2) & 1);
        X1 = (X1 | X1Tbl[X1A]) >> 1;
        X2 = (X2 | X2Tbl[X2A]) >> 1;

        c[n + 1] = (U8) ((X1 ^ X2) & 1);
        X1 = (X1 | X1Tbl[X1B]) >> 1;
        X2 = (X2 | X2Tbl[X2B]) >> 1;

        c[n + 2] = (U8) ((X1 ^ X2) & 1);
        X1 = (X1 | X1Tbl[X1C]) >> 1;
        X2 = (X2 | X2Tbl[X2C]) >> 1;

        c[n + 3] = (U8) ((X1 ^ X2) & 1);
        X1 = (X1 | X1Tbl[X1D]) >> 1;
        X2 = (X2 | X2Tbl[X2D]) >> 1;
    }

    if (N > (Nhalf << 2))
    {
        for (n = (N - (Nhalf << 2)); n < N; n++)
        {
            X1A = X1 & 0x0000000F;
            X1B = (X1 >> 1) & 0x0000000F;

            c[n] = (U8) ((X1 ^ X2) & 1);
            X1 = (X1 | X1Tbl[X1A]) >> 1;
            X2 = (X2 | X2Tbl[X2A]) >> 1;
        }
    }
#else
    PseudoRandomSeqGen_Part2_A(X1, X2, N, c);
#endif

    return;
}

#ifndef CEVA_INTRINSICS
void PseudoRandomSeqGen1(S32 N, U32 *cinit, U32 *x1, U8 *c, U32 init)
{
    U32 X1, X2, X2x, X2a, X2b, X2c, X2d;
    U32 parity;
    S32 n;
    U32 X1A, X2A = 0;

    X1 = *x1;
    if (init == 0)
    {
        X2 = *cinit;
    }
    else
    {
        X2 = 0;
        for (n = 0; n < 31; n++)
        {
            X2x = *cinit & PsuedoRandomGenInitValue[n];

            X2a = (X2x) & 0xFF;
            X2b = (X2x >> 8) & 0xFF;
            X2c = (X2x >> 16) & 0xFF;
            X2d = (X2x >> 24) & 0xFF;

            parity = PsuedoRandomGenParityTable[X2a];
            parity += PsuedoRandomGenParityTable[X2b];
            parity += PsuedoRandomGenParityTable[X2c];
            parity += PsuedoRandomGenParityTable[X2d];

            parity = (parity) & 0x1;

            X2 = X2 | (parity << n);
        }
    }

    for (n = 0; n < N; n++)
    {
        X1A = X1 & 0x0000000F;
        X2A = X2 & 0x0000000F;

        c[n] = (U8) ((X1 ^ X2) & 1);

        X1 = (X1 | X1Tbl[X1A]) >> 1;
        X2 = (X2 | X2Tbl[X2A]) >> 1;
    }

    *cinit = X2;
    *x1 = X1;

    return;
}
#endif


void PseudoRandomSeqGen2(S32 N, U32 cinit, U8 *c)
{

    U32 X1, X2;
	U32 X2x, X2a, X2b, X2c, X2d;
    U32 parity;
    S32 n;
    S32 Nhalf;
    S32 X1A, X1B, X2A = 0, X2B;
    S32 X1C, X1D, X2C, X2D;

    X2 = 0;
    for (n = 0; n < 31; n++)
    {
        X2x = cinit & PsuedoRandomGenInitValue[n];

        X2a = (X2x) & 0xFF;
        X2b = (X2x >> 8) & 0xFF;
        X2c = (X2x >> 16) & 0xFF;
        X2d = (X2x >> 24) & 0xFF;

        parity = PsuedoRandomGenParityTable[X2a];
        parity += PsuedoRandomGenParityTable[X2b];
        parity += PsuedoRandomGenParityTable[X2c];
        parity += PsuedoRandomGenParityTable[X2d];

        parity = (parity) & 0x1;

        X2 = X2 | (parity << n);
    }

    //Pre-Calculate X1 since it is always the same
    X1 = 1581799488;

    Nhalf = N >> 2;
    for (n = 0; n < N; n += 4)
    {
        X1A = X1 & 0x0000000F;
        X1B = (X1 >> 1) & 0x0000000F;
        X1C = (X1 >> 2) & 0x0000000F;
        X1D = (X1 >> 3) & 0x0000000F;
        X2A = X2 & 0x0000000F;
        X2B = (X2 >> 1) & 0x0000000F;
        X2C = (X2 >> 2) & 0x0000000F;
        X2D = (X2 >> 3) & 0x0000000F;

        c[n] = (U8) ((X1 ^ X2) & 1);
//        if(c[n] == 0)
//        {
//            c[n] = 0xff;
//        }

        X1 = (X1 | X1Tbl[X1A]) >> 1;
        X2 = (X2 | X2Tbl[X2A]) >> 1;

        c[n + 1] = (U8) ((X1 ^ X2) & 1);
//        if(c[n+1] == 0)
//        {
//            c[n+1] = 0xff;
//        }
        X1 = (X1 | X1Tbl[X1B]) >> 1;
        X2 = (X2 | X2Tbl[X2B]) >> 1;

        c[n + 2] = (U8) ((X1 ^ X2) & 1);
//        if(c[n+2] == 0)
//        {
//            c[n+2] = 0xff;
//        }
        X1 = (X1 | X1Tbl[X1C]) >> 1;
        X2 = (X2 | X2Tbl[X2C]) >> 1;

        c[n + 3] = (U8) ((X1 ^ X2) & 1);
//        if(c[n+3] == 0)
//        {
//            c[n+3] = 0xff;
//        }
        X1 = (X1 | X1Tbl[X1D]) >> 1;
        X2 = (X2 | X2Tbl[X2D]) >> 1;
    }

    if (N > (Nhalf << 2))
    {
        for (n = (N - (Nhalf << 2)); n < N; n++)
        {
            X1A = X1 & 0x0000000F;
            X1B = (X1 >> 1) & 0x0000000F;

            c[n] = (U8) ((X1 ^ X2) & 1);
//            if(c[n] == 0)
//            {
//                c[n] = 0xff;
//            }
            X1 = (X1 | X1Tbl[X1A]) >> 1;
            X2 = (X2 | X2Tbl[X2A]) >> 1;
        }
    }

    return;
}
