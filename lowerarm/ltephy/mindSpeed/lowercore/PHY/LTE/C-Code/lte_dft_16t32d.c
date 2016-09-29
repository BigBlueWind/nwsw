//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL lte_dft_16t32d.c
 *
 * @brief DFT/IDFT function (floating point version)
 * @author Mindspeed Technologies
 * @version $Revision: 1.16 $
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
#include "dft_table16.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#else
#include    "basic_op_cc.h"
#endif

long xIn32[4096];
long xIn_cjg32[4096];
long Xk32[4096];
short SqNInv = 0x7FFF;
short EsqN = 0;
short xIn_cjg16[4096];
short Xk16[4096];

//------------------------------------------------------------------------------


/******************************************************************************
 Name:        MULT16BY32
 Function:    16 BY 32 signed fractional multiply
 Arguments:    IN1    16-bit signed input
 IN2    32-bit signed input
 Description:
 Function MULT16BY32 returns the 32-bit signed fractional
 product of a 16-bit signed fractional input and another 32-bit
 signed fractional input.

 *****************************************************************************/
#if 0 //CEVA_INTRINSICS

static long int MULT16BY32(short int IN1, long int IN2)
{

    unsigned short int XL;
    short int XU;
    long int AC;

    XL = (unsigned short) extract_l(IN2);
    XU = extract_h(IN2);
    AC = L_mpysu40(IN1,XL);
    AC = AC >> 16;
    AC = AC + L_mult(IN1,XU);

    return (AC);

}
#else

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   IN1 DOXYGEN_TO_DO
 *  @param   IN2 DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |IN1           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |IN2           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
long MULT16BY32(short IN1, long IN2)
{

    unsigned short XL;
    short XU;
    long AC;

    XL = (unsigned short) extract_l(IN2);
    XU = extract_h(IN2);
    AC = IN1 * XL;
    AC = AC >> 15;
    AC = AC + L_mult(IN1, XU);

    return (AC);

}
#endif //def CEVA_INTRINSICS
//------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *x32 DOXYGEN_TO_DO
 *  @param   N DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *  @param   *Exp2  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*x32          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |N             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Exp2         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void DFT_ScaleSqN_16t32d(long *x32, long N, short *Xk, short *Exp2)
{
    int i;
    long xMax, a;
    short nrm;

    xMax = 0;
    for (i = 0; i < 2 * N; i++)
    {
        x32[i] = MULT16BY32(SqNInv, x32[i]); // x32[i] * SqNInv
        a = abs(x32[i]);
        if (xMax < a)
            xMax = a;
    }
    nrm = norm_l(xMax);
    for (i = 0; i < 2 * N; i++)
    {
        Xk[i] = extract_h(L_shl(x32[i], nrm));
    }
    nrm = EsqN - nrm; // EsqN is the exponent of the scaling factor 1/sqrt(N)
    *Exp2 += nrm;
    return;
}

//------------------------------------------------------------------------------
//
// Convert 16-bit to 32-bit, with 11 bits head room


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *x16 DOXYGEN_TO_DO
 *  @param   short N DOXYGEN_TO_DO
 *  @param   *x32 DOXYGEN_TO_DO
 *  @param   *Exp2  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*x16          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |short N       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*x32          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Exp2         |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void Convert_16to32(short *x16, unsigned short N, long *x32, short *Exp2)
{
    int i;
    short nrm, sl;

    nrm = 11;
    sl = 16 - nrm;
    for (i = 0; i < 2 * N; i++)
    {
        x32[i] = ((long) x16[i]) << sl;
    }
    *Exp2 += nrm;
    return;
}

#if 0

//------------------------------------------------------------------------------

// 3-point FFT
// Input
//     xn : array of 3 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 3 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0003 (double *xn, double *Xk)
{
    double W3_1c, W3_1d, W3_2c, W3_2d;

    W3_1c = W0003[2];
    W3_1d = W0003[3];
    W3_2c = W0003[4];
    W3_2d = W0003[5];

    // Xk[0] = xn[0] + xn[1] + xn[2]
    //
    Xk[0] = xn[0] + xn[2] + xn[4];
    Xk[1] = xn[1] + xn[3] + xn[5];

    // Xk[1] = xn[0] + xn[1]*W3(1) + xn[2]*W3(2)
    //
    Xk[2] = xn[0] + (xn[2]*W3_1c - xn[3]*W3_1d ) + (xn[4]*W3_2c - xn[5]*W3_2d);
    Xk[3] = xn[1] + (xn[2]*W3_1d + xn[3]*W3_1c ) + (xn[4]*W3_2d + xn[5]*W3_2c);

    // Xk[2] = xn[0] + xn[1]*W3(2) + xn[2]*W3(1)
    //
    Xk[4] = xn[0] + (xn[2]*W3_2c - xn[3]*W3_2d ) + (xn[4]*W3_1c - xn[5]*W3_1d);
    Xk[5] = xn[1] + (xn[2]*W3_2d + xn[3]*W3_2c ) + (xn[4]*W3_1d + xn[5]*W3_1c);

    return;
}

#endif

//------------------------------------------------------------------------------

// 3-point FFT
// Input
//     xn : array of 3 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 3 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0003_16t32d(long *xn, long *Xk)
{
    short W3_1c, W3_1d, W3_2c, W3_2d;
    long g, h;

    W3_1c = W16t0003[2];
    W3_1d = W16t0003[3];
    W3_2c = W16t0003[4];
    W3_2d = W16t0003[5];

    // Xk[0] = xn[0] + xn[1] + xn[2]
    //
    g = L_add(xn[0], xn[2]);
    Xk[0] = L_add(g, xn[4]);

    g = L_add(xn[1], xn[3]);
    Xk[1] = L_add(g, xn[5]);

    // // Xk[1] = xn[0] + xn[1]*W3(1) + xn[2]*W3(2)
    // //
    // Xk[2] = xn[0] + (xn[2]*W3_1c - xn[3]*W3_1d ) + (xn[4]*W3_2c - xn[5]*W3_2d);
    g = MULT16BY32(W3_1c, xn[2]);
    h = L_add(xn[0], g);
    g = MULT16BY32(W3_1d, xn[3]);
    h = L_sub(h, g);
    g = MULT16BY32(W3_2c, xn[4]);
    h = L_add(h, g);
    g = MULT16BY32(W3_2d, xn[5]);
    Xk[2] = L_sub(h, g);

    // Xk[3] = xn[1] + (xn[2]*W3_1d + xn[3]*W3_1c ) + (xn[4]*W3_2d + xn[5]*W3_2c);
    g = MULT16BY32(W3_1d, xn[2]);
    h = L_add(xn[1], g);
    g = MULT16BY32(W3_1c, xn[3]);
    h = L_add(h, g);
    g = MULT16BY32(W3_2d, xn[4]);
    h = L_add(h, g);
    g = MULT16BY32(W3_2c, xn[5]);
    Xk[3] = L_add(h, g);

    // // Xk[2] = xn[0] + xn[1]*W3(2) + xn[2]*W3(1)
    // //
    // Xk[4] = xn[0] + (xn[2]*W3_2c - xn[3]*W3_2d ) + (xn[4]*W3_1c - xn[5]*W3_1d);
    g = MULT16BY32(W3_2c, xn[2]);
    h = L_add(xn[0], g);
    g = MULT16BY32(W3_2d, xn[3]);
    h = L_sub(h, g);
    g = MULT16BY32(W3_1c, xn[4]);
    h = L_add(h, g);
    g = MULT16BY32(W3_1d, xn[5]);
    Xk[4] = L_sub(h, g);

    // Xk[5] = xn[1] + (xn[2]*W3_2d + xn[3]*W3_2c ) + (xn[4]*W3_1d + xn[5]*W3_1c);
    g = MULT16BY32(W3_2d, xn[2]);
    h = L_add(xn[1], g);
    g = MULT16BY32(W3_2c, xn[3]);
    h = L_add(h, g);
    g = MULT16BY32(W3_1d, xn[4]);
    h = L_add(h, g);
    g = MULT16BY32(W3_1c, xn[5]);
    Xk[5] = L_add(h, g);

    return;
}

#if 0
//------------------------------------------------------------------------------

// 4-point FFT
// Input
//     xn : array of 4 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 4 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0004 (double *xn, double *Xk)
{
    // X(0) = x(0) + x(1) + x(2) + x(3)
    Xk[0] = xn[0] + xn[2] + xn[4] + xn[6];
    Xk[1] = xn[1] + xn[3] + xn[5] + xn[7];

    // X(1) = x(0) + x(1) * (-i) + x(2) * (-1) + x(3) * i
    Xk[2] = xn[0] + xn[3] - xn[4] - xn[7];
    Xk[3] = xn[1] - xn[2] - xn[5] + xn[6];

    // X(2) = x(0) - x(1) + x(2) - x(3)
    Xk[4] = xn[0] - xn[2] + xn[4] - xn[6];
    Xk[5] = xn[1] - xn[3] + xn[5] - xn[7];

    // X(3) = x(0) + x(1)*i - x(2) - x(3)*i
    Xk[6] = xn[0] - xn[3] - xn[4] + xn[7];
    Xk[7] = xn[1] + xn[2] - xn[5] - xn[6];

    return;
}
#endif

//------------------------------------------------------------------------------

// 4-point FFT
// Input
//     xn : array of 4 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 4 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0004_16t32d(long *xn, long *Xk)
{
    long g, h;

    // // X(0) = x(0) + x(1) + x(2) + x(3)
    // Xk[0] = xn[0] + xn[2] + xn[4] + xn[6];
    g = L_add(xn[0], xn[2]);
    h = L_add(xn[4], xn[6]);
    Xk[0] = L_add(g, h);

    // Xk[1] = xn[1] + xn[3] + xn[5] + xn[7];
    g = L_add(xn[1], xn[3]);
    h = L_add(xn[5], xn[7]);
    Xk[1] = L_add(g, h);

    // // X(1) = x(0) + x(1) * (-i) + x(2) * (-1) + x(3) * i
    // Xk[2] = xn[0] + xn[3] - xn[4] - xn[7];
    g = L_add(xn[0], xn[3]);
    h = L_add(xn[4], xn[7]);
    Xk[2] = L_sub(g, h);

    // Xk[3] = xn[1] - xn[2] - xn[5] + xn[6];
    g = L_add(xn[1], xn[6]);
    h = L_add(xn[2], xn[5]);
    Xk[3] = L_sub(g, h);

    // // X(2) = x(0) - x(1) + x(2) - x(3)
    // Xk[4] = xn[0] - xn[2] + xn[4] - xn[6];
    g = L_add(xn[0], xn[4]);
    h = L_add(xn[2], xn[6]);
    Xk[4] = L_sub(g, h);

    // Xk[5] = xn[1] - xn[3] + xn[5] - xn[7];
    g = L_add(xn[1], xn[5]);
    h = L_add(xn[3], xn[7]);
    Xk[5] = L_sub(g, h);

    // // X(3) = x(0) + x(1)*i - x(2) - x(3)*i
    // Xk[6] = xn[0] - xn[3] - xn[4] + xn[7];
    g = L_add(xn[0], xn[7]);
    h = L_add(xn[3], xn[4]);
    Xk[6] = L_sub(g, h);

    // Xk[7] = xn[1] + xn[2] - xn[5] - xn[6];
    g = L_add(xn[1], xn[2]);
    h = L_add(xn[5], xn[6]);
    Xk[7] = L_sub(g, h);

    return;
}

#if 0
//------------------------------------------------------------------------------

// 5-point FFT
// Input
//     xn : array of 5 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 5 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0005 (double *xn, double *Xk)
{
    int i, k, p;
    double a, b, c, d;

    Xk[0] = xn[0]+xn[2]+xn[4]+xn[6]+xn[8];
    Xk[1] = xn[1]+xn[3]+xn[5]+xn[7]+xn[9];
    for (i=1; i < 5; i++)
    {
        Xk[2*i] = xn[0];
        Xk[2*i+1] = xn[1];
        for (k=1; k < 5; k++)
        {
            p = ((i * k) % 5) * 5;
            a = xn[2*k];
            b = xn[2*k+1];
            c = W0025[2*p];
            d = W0025[2*p+1];
            Xk[2*i] += (a*c - b*d);
            Xk[2*i+1] += (a*d + b*c);
        }
    }
    return;
}
#endif

//------------------------------------------------------------------------------

// 5-point FFT
// Input
//     xn : array of 5 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 5 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0005_16t32d(long *xn, long *Xk)
{
    int i, k, p;
    long a32, b32;
    short c16, d16;
    long g, h, u, v;

    // Xk[0] = xn[0]+xn[2]+xn[4]+xn[6]+xn[8];
    a32 = L_add(xn[0], xn[2]);
    b32 = L_add(xn[4], xn[6]);
    a32 = L_add(a32, xn[8]);
    Xk[0] = L_add(a32, b32);

    // Xk[1] = xn[1]+xn[3]+xn[5]+xn[7]+xn[9];
    a32 = L_add(xn[1], xn[3]);
    b32 = L_add(xn[5], xn[7]);
    a32 = L_add(a32, xn[9]);
    Xk[1] = L_add(a32, b32);

    for (i = 1; i < 5; i++)
    {
        // Xk[2*i] = xn[0];
        u = xn[0];
        // Xk[2*i+1] = xn[1];
        v = xn[1];
        for (k = 1; k < 5; k++)
        {
            p = ((i * k) % 5) * 5;
            a32 = xn[2 * k];
            b32 = xn[2 * k + 1];
            c16 = W16t0025[2 * p];
            d16 = W16t0025[2 * p + 1];

            // Xk[2*i] += (a*c - b*d);
            g = MULT16BY32(c16, a32);
            h = MULT16BY32(d16, b32);
            g = L_sub(g, h);
            u = L_add(u, g);

            // Xk[2*i+1] += (a*d + b*c);
            g = MULT16BY32(d16, a32);
            h = MULT16BY32(c16, b32);
            g = L_add(g, h);
            v = L_add(v, g);
        }
        Xk[2 * i] = u;
        Xk[2 * i + 1] = v;
    }
    return;
}

#if 0
//------------------------------------------------------------------------------

// 25-point FFT
// Input
//     xn : array of 25 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 25 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0025 (double *xn, double *Xk)
{
    int l, q;
    int i, k;
    double a, b, c, d;
    double xlm[20];
    double Xpq[20];

    for (l=0; l < 5; l++)
    {
        for (i=0; i < 5; i++)
        {
            xlm[2*i] = xn[2*(l+5*i)];
            xlm[2*i+1] = xn[2*(l+5*i)+1];
        }
        FFT0005 (xlm, Xpq);
        for (q=0; q < 5; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = W0025[2*k];
            d = W0025[2*k+1];
            i = 5*l + q;
            Xk[2*i] = (a*c - b*d);
            Xk[2*i+1] = (a*d + b*c);
        }
    }

    for (q=0; q < 5; q++)
    {
        for (i=0; i < 5; i++)
        {
            xlm[2*i] = Xk[2*(q+5*i)];
            xlm[2*i+1] = Xk[2*(q+5*i)+1];
        }
        FFT0005 (xlm, Xpq);
        for (i=0; i < 5; i++)
        {
            Xk[2*(q+5*i)] = Xpq[2*i];
            Xk[2*(q+5*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

//------------------------------------------------------------------------------

// 25-point FFT
// Input
//     xn : array of 25 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 25 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0025_16t32d(long *xn, long *Xk)
{
    int l, q;
    int i, k;
    long a, b;
    short c, d;
    long xlm[10];
    long Xpq[10];
    long g, h;

    for (l = 0; l < 5; l++)
    {
        for (i = 0; i < 5; i++)
        {
            xlm[2 * i] = xn[2 * (l + 5 * i)];
            xlm[2 * i + 1] = xn[2 * (l + 5 * i) + 1];
        }
        FFT0005_16t32d(xlm, Xpq);
        for (q = 0; q < 5; q++)
        {
            k = l * q;
            a = Xpq[2 * q];
            b = Xpq[2 * q + 1];
            c = W16t0025[2 * k];
            d = W16t0025[2 * k + 1];
            i = 5 * l + q;

            // Xk[2*i] = (a*c - b*d);
            g = MULT16BY32(c, a);
            h = MULT16BY32(d, b);
            Xk[2 * i] = L_sub(g, h);

            // Xk[2*i+1] = (a*d + b*c);
            g = MULT16BY32(d, a);
            h = MULT16BY32(c, b);
            Xk[2 * i + 1] = L_add(g, h);
        }
    }

    for (q = 0; q < 5; q++)
    {
        for (i = 0; i < 5; i++)
        {
            xlm[2 * i] = Xk[2 * (q + 5 * i)];
            xlm[2 * i + 1] = Xk[2 * (q + 5 * i) + 1];
        }
        FFT0005_16t32d(xlm, Xpq);
        for (i = 0; i < 5; i++)
        {
            Xk[2 * (q + 5 * i)] = Xpq[2 * i];
            Xk[2 * (q + 5 * i) + 1] = Xpq[2 * i + 1];
        }
    }

    return;
}

#if 0
//------------------------------------------------------------------------------

// 8-point FFT
// Input
//     xn : array of 8 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 8 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0008 (double *xn, double *Xk)
{
    int l, q;
    int i, k;
    double a, b, c, d;
    double xlm[10];
    double Xpq[10];

    for (l=0; l < 4; l++)
    {
        for (i=0; i < 2; i++)
        {
            xlm[2*i] = xn[2*(l+4*i)];
            xlm[2*i+1] = xn[2*(l+4*i)+1];
        }
        // 2-point DFT
        //
        // Xk[0] = xn[0] + xn[1]
        //
        Xpq[0] = xlm[0] + xlm[2];
        Xpq[1] = xlm[1] + xlm[3];

        // Xk[1] = xn[0] - xn[1]
        //
        Xpq[2] = xlm[0] - xlm[2];
        Xpq[3] = xlm[1] - xlm[3];

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q=0; q < 2; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = W0008[2*k];
            d = W0008[2*k+1];
            i = 2*l + q;
            Xk[2*i] = (a*c - b*d);
            Xk[2*i+1] = (a*d + b*c);
        }
    }

    // 4-point DFT for the 2 columns
    //
    for (q=0; q < 2; q++)
    {
        for (i=0; i < 4; i++)
        {
            xlm[2*i] = Xk[2*(q+2*i)];
            xlm[2*i+1] = Xk[2*(q+2*i)+1];
        }
        FFT0004 (xlm, Xpq);
        for (i=0; i < 4; i++)
        {
            Xk[2*(q+2*i)] = Xpq[2*i];
            Xk[2*(q+2*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

//------------------------------------------------------------------------------

// 8-point FFT
// Input
//     xn : array of 8 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 8 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0008_16t32d(long *xn, long *Xk)
{
    int l, q;
    int i, k;
    long a, b;
    short c, d;
    long xlm[10];
    long Xpq[10];
    long g, h;

    for (l = 0; l < 4; l++)
    {
        for (i = 0; i < 2; i++)
        {
            xlm[2 * i] = xn[2 * (l + 4 * i)];
            xlm[2 * i + 1] = xn[2 * (l + 4 * i) + 1];
        }
        // // 2-point DFT
        // //
        // // Xk[0] = xn[0] + xn[1]
        // //
        // Xpq[0] = xlm[0] + xlm[2];
        Xpq[0] = L_add(xlm[0], xlm[2]);

        // Xpq[1] = xlm[1] + xlm[3];
        Xpq[1] = L_add(xlm[1], xlm[3]);

        // // Xk[1] = xn[0] - xn[1]
        // //
        // Xpq[2] = xlm[0] - xlm[2];
        Xpq[2] = L_sub(xlm[0], xlm[2]);

        // Xpq[3] = xlm[1] - xlm[3];
        Xpq[3] = L_sub(xlm[1], xlm[3]);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q = 0; q < 2; q++)
        {
            k = l * q;
            a = Xpq[2 * q];
            b = Xpq[2 * q + 1];
            c = W16t0008[2 * k];
            d = W16t0008[2 * k + 1];
            i = 2 * l + q;

            // Xk[2*i] = (a*c - b*d);
            g = MULT16BY32(c, a);
            h = MULT16BY32(d, b);
            Xk[2 * i] = L_sub(g, h);

            // Xk[2*i+1] = (a*d + b*c);
            g = MULT16BY32(d, a);
            h = MULT16BY32(c, b);
            Xk[2 * i + 1] = L_add(g, h);
        }
    }

    // 4-point DFT for the 2 columns
    //
    for (q = 0; q < 2; q++)
    {
        for (i = 0; i < 4; i++)
        {
            xlm[2 * i] = Xk[2 * (q + 2 * i)];
            xlm[2 * i + 1] = Xk[2 * (q + 2 * i) + 1];
        }
        FFT0004_16t32d(xlm, Xpq);
        for (i = 0; i < 4; i++)
        {
            Xk[2 * (q + 2 * i)] = Xpq[2 * i];
            Xk[2 * (q + 2 * i) + 1] = Xpq[2 * i + 1];
        }
    }

    return;
}

#if 0
//------------------------------------------------------------------------------

// 9-point FFT
// Input
//     xn : array of 9 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 9 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0009 (double *xn, double *Xk)
{
    int l, q;
    int i, k;
    double a, b, c, d;
    double W3_1c, W3_1d, W3_2c, W3_2d;
    double xlm[20];
    double Xpq[20];

    W3_1c = W0003[2];
    W3_1d = W0003[3];
    W3_2c = W0003[4];
    W3_2d = W0003[5];
    for (l=0; l < 3; l++)
    {
        for (i=0; i < 3; i++)
        {
            xlm[2*i] = xn[2*(l+3*i)];
            xlm[2*i+1] = xn[2*(l+3*i)+1];
        }
        // 3-point DFT
        //
        // Xk[0] = xn[0] + xn[1] + xn[2]
        //
        Xpq[0] = xlm[0] + xlm[2] + xlm[4];
        Xpq[1] = xlm[1] + xlm[3] + xlm[5];

        // Xk[1] = xn[0] + xn[1]*W3(1) + xn[2]*W3(2)
        //
        Xpq[2] = xlm[0] + (xlm[2]*W3_1c - xlm[3]*W3_1d ) + (xlm[4]*W3_2c - xlm[5]*W3_2d);
        Xpq[3] = xlm[1] + (xlm[2]*W3_1d + xlm[3]*W3_1c ) + (xlm[4]*W3_2d + xlm[5]*W3_2c);

        // Xk[2] = xn[0] + xn[1]*W3(2) + xn[2]*W3(1)
        //
        Xpq[4] = xlm[0] + (xlm[2]*W3_2c - xlm[3]*W3_2d ) + (xlm[4]*W3_1c - xlm[5]*W3_1d);
        Xpq[5] = xlm[1] + (xlm[2]*W3_2d + xlm[3]*W3_2c ) + (xlm[4]*W3_1d + xlm[5]*W3_1c);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q=0; q < 3; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = W0009[2*k];
            d = W0009[2*k+1];
            i = 3*l + q;
            Xk[2*i] = (a*c - b*d);
            Xk[2*i+1] = (a*d + b*c);
        }
    }

    // 3-point DFT for the 3 columns
    //
    for (q=0; q < 3; q++)
    {
        for (i=0; i < 3; i++)
        {
            xlm[2*i] = Xk[2*(q+3*i)];
            xlm[2*i+1] = Xk[2*(q+3*i)+1];
        }
        FFT0003 (xlm, Xpq);
        for (i=0; i < 3; i++)
        {
            Xk[2*(q+3*i)] = Xpq[2*i];
            Xk[2*(q+3*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

//------------------------------------------------------------------------------

// 9-point FFT
// Input
//     xn : array of 9 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 9 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0009_16t32d(long *xn, long *Xk)
{
    int l, q;
    int i, k;
    long a, b;
    short c, d;
    short W3_1c, W3_1d, W3_2c, W3_2d;
    long xlm[10];
    long Xpq[10];
    long g, h, u;

    W3_1c = W16t0003[2];
    W3_1d = W16t0003[3];
    W3_2c = W16t0003[4];
    W3_2d = W16t0003[5];
    for (l = 0; l < 3; l++)
    {
        for (i = 0; i < 3; i++)
        {
            xlm[2 * i] = xn[2 * (l + 3 * i)];
            xlm[2 * i + 1] = xn[2 * (l + 3 * i) + 1];
        }
        // // 3-point DFT
        // //
        // // Xk[0] = xn[0] + xn[1] + xn[2]
        // //
        // Xpq[0] = xlm[0] + xlm[2] + xlm[4];
        g = L_add(xlm[0], xlm[2]);
        Xpq[0] = L_add(g, xlm[4]);

        // Xpq[1] = xlm[1] + xlm[3] + xlm[5];
        g = L_add(xlm[1], xlm[3]);
        Xpq[1] = L_add(g, xlm[5]);

        // // Xk[1] = xn[0] + xn[1]*W3(1) + xn[2]*W3(2)
        // //
        // Xpq[2] = xlm[0] + (xlm[2]*W3_1c - xlm[3]*W3_1d ) + (xlm[4]*W3_2c - xlm[5]*W3_2d);
        g = MULT16BY32(W3_1c, xlm[2]);
        u = L_add(xlm[0], g);
        h = MULT16BY32(W3_1d, xlm[3]);
        u = L_sub(u, h);
        g = MULT16BY32(W3_2c, xlm[4]);
        u = L_add(u, g);
        h = MULT16BY32(W3_2d, xlm[5]);
        Xpq[2] = L_sub(u, h);

        // Xpq[3] = xlm[1] + (xlm[2]*W3_1d + xlm[3]*W3_1c ) + (xlm[4]*W3_2d + xlm[5]*W3_2c);
        g = MULT16BY32(W3_1d, xlm[2]);
        u = L_add(xlm[1], g);
        h = MULT16BY32(W3_1c, xlm[3]);
        u = L_add(u, h);
        g = MULT16BY32(W3_2d, xlm[4]);
        u = L_add(u, g);
        h = MULT16BY32(W3_2c, xlm[5]);
        Xpq[3] = L_add(u, h);

        // // Xk[2] = xn[0] + xn[1]*W3(2) + xn[2]*W3(1)
        // //
        // Xpq[4] = xlm[0] + (xlm[2]*W3_2c - xlm[3]*W3_2d ) + (xlm[4]*W3_1c - xlm[5]*W3_1d);
        g = MULT16BY32(W3_2c, xlm[2]);
        u = L_add(xlm[0], g);
        h = MULT16BY32(W3_2d, xlm[3]);
        u = L_sub(u, h);
        g = MULT16BY32(W3_1c, xlm[4]);
        u = L_add(u, g);
        h = MULT16BY32(W3_1d, xlm[5]);
        Xpq[4] = L_sub(u, h);

        // Xpq[5] = xlm[1] + (xlm[2]*W3_2d + xlm[3]*W3_2c ) + (xlm[4]*W3_1d + xlm[5]*W3_1c);
        g = MULT16BY32(W3_2d, xlm[2]);
        u = L_add(xlm[1], g);
        h = MULT16BY32(W3_2c, xlm[3]);
        u = L_add(u, h);
        g = MULT16BY32(W3_1d, xlm[4]);
        u = L_add(u, g);
        h = MULT16BY32(W3_1c, xlm[5]);
        Xpq[5] = L_add(u, h);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q = 0; q < 3; q++)
        {
            k = l * q;
            a = Xpq[2 * q];
            b = Xpq[2 * q + 1];
            c = W16t0009[2 * k];
            d = W16t0009[2 * k + 1];
            i = 3 * l + q;

            // Xk[2*i] = (a*c - b*d);
            g = MULT16BY32(c, a);
            h = MULT16BY32(d, b);
            Xk[2 * i] = L_sub(g, h);

            // Xk[2*i+1] = (a*d + b*c);
            g = MULT16BY32(d, a);
            h = MULT16BY32(c, b);
            Xk[2 * i + 1] = L_add(g, h);
        }
    }

    // 3-point DFT for the 3 columns
    //
    for (q = 0; q < 3; q++)
    {
        for (i = 0; i < 3; i++)
        {
            xlm[2 * i] = Xk[2 * (q + 3 * i)];
            xlm[2 * i + 1] = Xk[2 * (q + 3 * i) + 1];
        }
        FFT0003_16t32d(xlm, Xpq);
        for (i = 0; i < 3; i++)
        {
            Xk[2 * (q + 3 * i)] = Xpq[2 * i];
            Xk[2 * (q + 3 * i) + 1] = Xpq[2 * i + 1];
        }
    }

    return;
}

#if 0
//------------------------------------------------------------------------------

// 12-point FFT
// Input
//     xn : array of 12 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 12 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0012 (double *xn, double *Xk)
{
    int l, q;
    int i, k;
    double a, b, c, d;
    double W3_1c, W3_1d, W3_2c, W3_2d;
    double xlm[20];
    double Xpq[20];

    W3_1c = W0003[2];
    W3_1d = W0003[3];
    W3_2c = W0003[4];
    W3_2d = W0003[5];
    for (l=0; l < 4; l++)
    {
        for (i=0; i < 3; i++)
        {
            xlm[2*i] = xn[2*(l+4*i)];
            xlm[2*i+1] = xn[2*(l+4*i)+1];
        }
        // 3-point DFT
        //
        // Xk[0] = xn[0] + xn[1] + xn[2]
        //
        Xpq[0] = xlm[0] + xlm[2] + xlm[4];
        Xpq[1] = xlm[1] + xlm[3] + xlm[5];

        // Xk[1] = xn[0] + xn[1]*W3(1) + xn[2]*W3(2)
        //
        Xpq[2] = xlm[0] + (xlm[2]*W3_1c - xlm[3]*W3_1d ) + (xlm[4]*W3_2c - xlm[5]*W3_2d);
        Xpq[3] = xlm[1] + (xlm[2]*W3_1d + xlm[3]*W3_1c ) + (xlm[4]*W3_2d + xlm[5]*W3_2c);

        // Xk[2] = xn[0] + xn[1]*W3(2) + xn[2]*W3(1)
        //
        Xpq[4] = xlm[0] + (xlm[2]*W3_2c - xlm[3]*W3_2d ) + (xlm[4]*W3_1c - xlm[5]*W3_1d);
        Xpq[5] = xlm[1] + (xlm[2]*W3_2d + xlm[3]*W3_2c ) + (xlm[4]*W3_1d + xlm[5]*W3_1c);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q=0; q < 3; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = W0012[2*k];
            d = W0012[2*k+1];
            i = 3*l + q;
            Xk[2*i] = (a*c - b*d);
            Xk[2*i+1] = (a*d + b*c);
        }
    }

    // 4-point DFT for the 3 columns
    //
    for (q=0; q < 3; q++)
    {
        for (i=0; i < 4; i++)
        {
            xlm[2*i] = Xk[2*(q+3*i)];
            xlm[2*i+1] = Xk[2*(q+3*i)+1];
        }
        FFT0004 (xlm, Xpq);
        for (i=0; i < 4; i++)
        {
            Xk[2*(q+3*i)] = Xpq[2*i];
            Xk[2*(q+3*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

//------------------------------------------------------------------------------

// 12-point FFT
// Input
//     xn : array of 12 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 12 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0012_16t32d(long *xn, long *Xk)
{
    int l, q;
    int i, k;
    long a, b;
    short c, d;
    short W3_1c, W3_1d, W3_2c, W3_2d;
    long xlm[10];
    long Xpq[10];
    long g, h, u;

    W3_1c = W16t0003[2];
    W3_1d = W16t0003[3];
    W3_2c = W16t0003[4];
    W3_2d = W16t0003[5];
    for (l = 0; l < 4; l++)
    {
        for (i = 0; i < 3; i++)
        {
            xlm[2 * i] = xn[2 * (l + 4 * i)];
            xlm[2 * i + 1] = xn[2 * (l + 4 * i) + 1];
        }
        // // 3-point DFT
        // //
        // // Xk[0] = xn[0] + xn[1] + xn[2]
        // //
        // Xpq[0] = xlm[0] + xlm[2] + xlm[4];
        g = L_add(xlm[0], xlm[2]);
        Xpq[0] = L_add(g, xlm[4]);

        // Xpq[1] = xlm[1] + xlm[3] + xlm[5];
        g = L_add(xlm[1], xlm[3]);
        Xpq[1] = L_add(g, xlm[5]);

        // // Xk[1] = xn[0] + xn[1]*W3(1) + xn[2]*W3(2)
        // //
        // Xpq[2] = xlm[0] + (xlm[2]*W3_1c - xlm[3]*W3_1d ) + (xlm[4]*W3_2c - xlm[5]*W3_2d);
        g = MULT16BY32(W3_1c, xlm[2]);
        u = L_add(xlm[0], g);
        h = MULT16BY32(W3_1d, xlm[3]);
        u = L_sub(u, h);
        g = MULT16BY32(W3_2c, xlm[4]);
        u = L_add(u, g);
        h = MULT16BY32(W3_2d, xlm[5]);
        Xpq[2] = L_sub(u, h);

        // Xpq[3] = xlm[1] + (xlm[2]*W3_1d + xlm[3]*W3_1c ) + (xlm[4]*W3_2d + xlm[5]*W3_2c);
        g = MULT16BY32(W3_1d, xlm[2]);
        u = L_add(xlm[1], g);
        h = MULT16BY32(W3_1c, xlm[3]);
        u = L_add(u, h);
        g = MULT16BY32(W3_2d, xlm[4]);
        u = L_add(u, g);
        h = MULT16BY32(W3_2c, xlm[5]);
        Xpq[3] = L_add(u, h);

        // // Xk[2] = xn[0] + xn[1]*W3(2) + xn[2]*W3(1)
        // //
        // Xpq[4] = xlm[0] + (xlm[2]*W3_2c - xlm[3]*W3_2d ) + (xlm[4]*W3_1c - xlm[5]*W3_1d);
        g = MULT16BY32(W3_2c, xlm[2]);
        u = L_add(xlm[0], g);
        h = MULT16BY32(W3_2d, xlm[3]);
        u = L_sub(u, h);
        g = MULT16BY32(W3_1c, xlm[4]);
        u = L_add(u, g);
        h = MULT16BY32(W3_1d, xlm[5]);
        Xpq[4] = L_sub(u, h);

        // Xpq[5] = xlm[1] + (xlm[2]*W3_2d + xlm[3]*W3_2c ) + (xlm[4]*W3_1d + xlm[5]*W3_1c);
        g = MULT16BY32(W3_2d, xlm[2]);
        u = L_add(xlm[1], g);
        h = MULT16BY32(W3_2c, xlm[3]);
        u = L_add(u, h);
        g = MULT16BY32(W3_1d, xlm[4]);
        u = L_add(u, g);
        h = MULT16BY32(W3_1c, xlm[5]);
        Xpq[5] = L_add(u, h);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q = 0; q < 3; q++)
        {
            k = l * q;
            a = Xpq[2 * q];
            b = Xpq[2 * q + 1];
            c = W16t0012[2 * k];
            d = W16t0012[2 * k + 1];
            i = 3 * l + q;

            // Xk[2*i] = (a*c - b*d);
            g = MULT16BY32(c, a);
            h = MULT16BY32(d, b);
            Xk[2 * i] = L_sub(g, h);

            // Xk[2*i+1] = (a*d + b*c);
            g = MULT16BY32(d, a);
            h = MULT16BY32(c, b);
            Xk[2 * i + 1] = L_add(g, h);
        }
    }

    // 4-point DFT for the 3 columns
    //
    for (q = 0; q < 3; q++)
    {
        for (i = 0; i < 4; i++)
        {
            xlm[2 * i] = Xk[2 * (q + 3 * i)];
            xlm[2 * i + 1] = Xk[2 * (q + 3 * i) + 1];
        }
        FFT0004_16t32d(xlm, Xpq);
        for (i = 0; i < 4; i++)
        {
            Xk[2 * (q + 3 * i)] = Xpq[2 * i];
            Xk[2 * (q + 3 * i) + 1] = Xpq[2 * i + 1];
        }
    }

    return;
}

#if 0
//------------------------------------------------------------------------------

// 15-point FFT
// Input
//     xn : array of 15 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 15 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0015 (double *xn, double *Xk)
{
    int l, q;
    int i, k;
    double a, b, c, d;
    double W3_1c, W3_1d, W3_2c, W3_2d;
    double xlm[20];
    double Xpq[20];

    W3_1c = W0003[2];
    W3_1d = W0003[3];
    W3_2c = W0003[4];
    W3_2d = W0003[5];
    for (l=0; l < 5; l++)
    {
        for (i=0; i < 3; i++)
        {
            xlm[2*i] = xn[2*(l+5*i)];
            xlm[2*i+1] = xn[2*(l+5*i)+1];
        }
        // 3-point DFT
        //
        // Xk[0] = xn[0] + xn[1] + xn[2]
        //
        Xpq[0] = xlm[0] + xlm[2] + xlm[4];
        Xpq[1] = xlm[1] + xlm[3] + xlm[5];

        // Xk[1] = xn[0] + xn[1]*W3(1) + xn[2]*W3(2)
        //
        Xpq[2] = xlm[0] + (xlm[2]*W3_1c - xlm[3]*W3_1d ) + (xlm[4]*W3_2c - xlm[5]*W3_2d);
        Xpq[3] = xlm[1] + (xlm[2]*W3_1d + xlm[3]*W3_1c ) + (xlm[4]*W3_2d + xlm[5]*W3_2c);

        // Xk[2] = xn[0] + xn[1]*W3(2) + xn[2]*W3(1)
        //
        Xpq[4] = xlm[0] + (xlm[2]*W3_2c - xlm[3]*W3_2d ) + (xlm[4]*W3_1c - xlm[5]*W3_1d);
        Xpq[5] = xlm[1] + (xlm[2]*W3_2d + xlm[3]*W3_2c ) + (xlm[4]*W3_1d + xlm[5]*W3_1c);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q=0; q < 3; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = W0015[2*k];
            d = W0015[2*k+1];
            i = 3*l + q;
            Xk[2*i] = (a*c - b*d);
            Xk[2*i+1] = (a*d + b*c);
        }
    }

    // 5-point DFT for the 3 columns
    //
    for (q=0; q < 3; q++)
    {
        for (i=0; i < 5; i++)
        {
            xlm[2*i] = Xk[2*(q+3*i)];
            xlm[2*i+1] = Xk[2*(q+3*i)+1];
        }
        FFT0005 (xlm, Xpq);
        for (i=0; i < 5; i++)
        {
            Xk[2*(q+3*i)] = Xpq[2*i];
            Xk[2*(q+3*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

//------------------------------------------------------------------------------

// 15-point FFT
// Input
//     xn : array of 15 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 15 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0015_16t32d(long *xn, long *Xk)
{
    int l, q;
    int i, k;
    long a, b;
    short c, d;
    short W3_1c, W3_1d, W3_2c, W3_2d;
    long xlm[20];
    long Xpq[20];
    long g, h, u;

    W3_1c = W16t0003[2];
    W3_1d = W16t0003[3];
    W3_2c = W16t0003[4];
    W3_2d = W16t0003[5];
    for (l = 0; l < 5; l++)
    {
        for (i = 0; i < 3; i++)
        {
            xlm[2 * i] = xn[2 * (l + 5 * i)];
            xlm[2 * i + 1] = xn[2 * (l + 5 * i) + 1];
        }
        // // 3-point DFT
        // //
        // // Xk[0] = xn[0] + xn[1] + xn[2]
        // //
        // Xpq[0] = xlm[0] + xlm[2] + xlm[4];
        g = L_add(xlm[0], xlm[2]);
        Xpq[0] = L_add(g, xlm[4]);

        // Xpq[1] = xlm[1] + xlm[3] + xlm[5];
        g = L_add(xlm[1], xlm[3]);
        Xpq[1] = L_add(g, xlm[5]);

        // // Xk[1] = xn[0] + xn[1]*W3(1) + xn[2]*W3(2)
        // //
        // Xpq[2] = xlm[0] + (xlm[2]*W3_1c - xlm[3]*W3_1d ) + (xlm[4]*W3_2c - xlm[5]*W3_2d);
        g = MULT16BY32(W3_1c, xlm[2]);
        u = L_add(xlm[0], g);
        h = MULT16BY32(W3_1d, xlm[3]);
        u = L_sub(u, h);
        g = MULT16BY32(W3_2c, xlm[4]);
        u = L_add(u, g);
        h = MULT16BY32(W3_2d, xlm[5]);
        Xpq[2] = L_sub(u, h);

        // Xpq[3] = xlm[1] + (xlm[2]*W3_1d + xlm[3]*W3_1c ) + (xlm[4]*W3_2d + xlm[5]*W3_2c);
        g = MULT16BY32(W3_1d, xlm[2]);
        u = L_add(xlm[1], g);
        h = MULT16BY32(W3_1c, xlm[3]);
        u = L_add(u, h);
        g = MULT16BY32(W3_2d, xlm[4]);
        u = L_add(u, g);
        h = MULT16BY32(W3_2c, xlm[5]);
        Xpq[3] = L_add(u, h);

        // // Xk[2] = xn[0] + xn[1]*W3(2) + xn[2]*W3(1)
        // //
        // Xpq[4] = xlm[0] + (xlm[2]*W3_2c - xlm[3]*W3_2d ) + (xlm[4]*W3_1c - xlm[5]*W3_1d);
        g = MULT16BY32(W3_2c, xlm[2]);
        u = L_add(xlm[0], g);
        h = MULT16BY32(W3_2d, xlm[3]);
        u = L_sub(u, h);
        g = MULT16BY32(W3_1c, xlm[4]);
        u = L_add(u, g);
        h = MULT16BY32(W3_1d, xlm[5]);
        Xpq[4] = L_sub(u, h);

        // Xpq[5] = xlm[1] + (xlm[2]*W3_2d + xlm[3]*W3_2c ) + (xlm[4]*W3_1d + xlm[5]*W3_1c);
        g = MULT16BY32(W3_2d, xlm[2]);
        u = L_add(xlm[1], g);
        h = MULT16BY32(W3_2c, xlm[3]);
        u = L_add(u, h);
        g = MULT16BY32(W3_1d, xlm[4]);
        u = L_add(u, g);
        h = MULT16BY32(W3_1c, xlm[5]);
        Xpq[5] = L_add(u, h);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q = 0; q < 3; q++)
        {
            k = l * q;
            a = Xpq[2 * q];
            b = Xpq[2 * q + 1];
            c = W16t0015[2 * k];
            d = W16t0015[2 * k + 1];
            i = 3 * l + q;

            // Xk[2*i] = (a*c - b*d);
            g = MULT16BY32(c, a);
            h = MULT16BY32(d, b);
            Xk[2 * i] = L_sub(g, h);

            // Xk[2*i+1] = (a*d + b*c);
            g = MULT16BY32(d, a);
            h = MULT16BY32(c, b);
            Xk[2 * i + 1] = L_add(g, h);
        }
    }

    // 5-point DFT for the 3 columns
    //
    for (q = 0; q < 3; q++)
    {
        for (i = 0; i < 5; i++)
        {
            xlm[2 * i] = Xk[2 * (q + 3 * i)];
            xlm[2 * i + 1] = Xk[2 * (q + 3 * i) + 1];
        }
        FFT0005_16t32d(xlm, Xpq);
        for (i = 0; i < 5; i++)
        {
            Xk[2 * (q + 3 * i)] = Xpq[2 * i];
            Xk[2 * (q + 3 * i) + 1] = Xpq[2 * i + 1];
        }
    }

    return;
}

#if 0
//------------------------------------------------------------------------------

// 16-point FFT
// Input
//     xn : array of 16 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 16 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0016 (double *xn, double *Xk)
{
    int l, q;
    int i, k;
    double a, b, c, d;
    int L, M;
    double xlm[10];
    double Xpq[10];

    M = 4;
    L = 4;

    for (l=0; l < L; l++)
    {
        for (i=0; i < M; i++)
        {
            xlm[2*i] = xn[2*(l+L*i)];
            xlm[2*i+1] = xn[2*(l+L*i)+1];
        }
        // M-point DFT
        //
        FFT0004 (xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q=0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = W0016[2*k];
            d = W0016[2*k+1];
            i = M*l + q;
            Xk[2*i] = (a*c - b*d);
            Xk[2*i+1] = (a*d + b*c);
        }
    }

    // L-point DFT for the M columns
    //
    for (q=0; q < M; q++)
    {
        for (i=0; i < L; i++)
        {
            xlm[2*i] = Xk[2*(q+M*i)];
            xlm[2*i+1] = Xk[2*(q+M*i)+1];
        }
        FFT0004 (xlm, Xpq);
        for (i=0; i < L; i++)
        {
            Xk[2*(q+M*i)] = Xpq[2*i];
            Xk[2*(q+M*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

//------------------------------------------------------------------------------

// 16-point FFT
// Input
//     xn : array of 16 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 16 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0016_16t32d(long *xn, long *Xk)
{
    int l, q;
    int i, k;
    long a, b;
    short c, d;
    int L, M;
    long xlm[10];
    long Xpq[10];
    long g, h;

    M = 4;
    L = 4;

    for (l = 0; l < L; l++)
    {
        for (i = 0; i < M; i++)
        {
            xlm[2 * i] = xn[2 * (l + L * i)];
            xlm[2 * i + 1] = xn[2 * (l + L * i) + 1];
        }
        // M-point DFT
        //
        FFT0004_16t32d(xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q = 0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2 * q];
            b = Xpq[2 * q + 1];
            c = W16t0016[2 * k];
            d = W16t0016[2 * k + 1];
            i = M * l + q;

            // Xk[2*i] = (a*c - b*d);
            g = MULT16BY32(c, a);
            h = MULT16BY32(d, b);
            Xk[2 * i] = L_sub(g, h);

            // Xk[2*i+1] = (a*d + b*c);
            g = MULT16BY32(d, a);
            h = MULT16BY32(c, b);
            Xk[2 * i + 1] = L_add(g, h);
        }
    }

    // L-point DFT for the M columns
    //
    for (q = 0; q < M; q++)
    {
        for (i = 0; i < L; i++)
        {
            xlm[2 * i] = Xk[2 * (q + M * i)];
            xlm[2 * i + 1] = Xk[2 * (q + M * i) + 1];
        }
        FFT0004_16t32d(xlm, Xpq);
        for (i = 0; i < L; i++)
        {
            Xk[2 * (q + M * i)] = Xpq[2 * i];
            Xk[2 * (q + M * i) + 1] = Xpq[2 * i + 1];
        }
    }

    return;
}

#if 0
//------------------------------------------------------------------------------

// 27-point FFT
// Input
//     xn : array of 27 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 27 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0027 (double *xn, double *Xk)
{
    int l, q;
    int i, k;
    double a, b, c, d;
    int L, M;
    double xlm[20];
    double Xpq[20];

    M = 9;
    L = 3;

    for (l=0; l < L; l++)
    {
        for (i=0; i < M; i++)
        {
            xlm[2*i] = xn[2*(l+L*i)];
            xlm[2*i+1] = xn[2*(l+L*i)+1];
        }
        // M-point DFT
        //
        FFT0009 (xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q=0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = W0027[2*k];
            d = W0027[2*k+1];
            i = M*l + q;
            Xk[2*i] = (a*c - b*d);
            Xk[2*i+1] = (a*d + b*c);
        }
    }

    // L-point DFT for the M columns
    //
    for (q=0; q < M; q++)
    {
        for (i=0; i < L; i++)
        {
            xlm[2*i] = Xk[2*(q+M*i)];
            xlm[2*i+1] = Xk[2*(q+M*i)+1];
        }
        FFT0003 (xlm, Xpq);
        for (i=0; i < L; i++)
        {
            Xk[2*(q+M*i)] = Xpq[2*i];
            Xk[2*(q+M*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

//------------------------------------------------------------------------------

// 27-point FFT
// Input
//     xn : array of 27 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 27 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0027_16t32d(long *xn, long *Xk)
{
    int l, q;
    int i, k;
    long a, b;
    short c, d;
    int L, M;
    long xlm[20];
    long Xpq[20];
    long g, h;

    M = 9;
    L = 3;

    for (l = 0; l < L; l++)
    {
        for (i = 0; i < M; i++)
        {
            xlm[2 * i] = xn[2 * (l + L * i)];
            xlm[2 * i + 1] = xn[2 * (l + L * i) + 1];
        }
        // M-point DFT
        //
        FFT0009_16t32d(xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q = 0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2 * q];
            b = Xpq[2 * q + 1];
            c = W16t0027[2 * k];
            d = W16t0027[2 * k + 1];
            i = M * l + q;

            // Xk[2*i] = (a*c - b*d);
            g = MULT16BY32(c, a);
            h = MULT16BY32(d, b);
            Xk[2 * i] = L_sub(g, h);

            // Xk[2*i+1] = (a*d + b*c);
            g = MULT16BY32(d, a);
            h = MULT16BY32(c, b);
            Xk[2 * i + 1] = L_add(g, h);
        }
    }

    // L-point DFT for the M columns
    //
    for (q = 0; q < M; q++)
    {
        for (i = 0; i < L; i++)
        {
            xlm[2 * i] = Xk[2 * (q + M * i)];
            xlm[2 * i + 1] = Xk[2 * (q + M * i) + 1];
        }
        FFT0003_16t32d(xlm, Xpq);
        for (i = 0; i < L; i++)
        {
            Xk[2 * (q + M * i)] = Xpq[2 * i];
            Xk[2 * (q + M * i) + 1] = Xpq[2 * i + 1];
        }
    }

    return;
}

#if 0
//------------------------------------------------------------------------------

// 81-point FFT
// Input
//     xn : array of 81 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 81 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0081 (double *xn, double *Xk)
{
    int l, q;
    int i, k;
    double a, b, c, d;
    int L, M;
    double xlm[60];
    double Xpq[60];

    M = 27;
    L = 3;

    for (l=0; l < L; l++)
    {
        for (i=0; i < M; i++)
        {
            xlm[2*i] = xn[2*(l+L*i)];
            xlm[2*i+1] = xn[2*(l+L*i)+1];
        }
        // M-point DFT
        //
        FFT0027 (xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q=0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = W0081[2*k];
            d = W0081[2*k+1];
            i = M*l + q;
            Xk[2*i] = (a*c - b*d);
            Xk[2*i+1] = (a*d + b*c);
        }
    }

    // L-point DFT for the M columns
    //
    for (q=0; q < M; q++)
    {
        for (i=0; i < L; i++)
        {
            xlm[2*i] = Xk[2*(q+M*i)];
            xlm[2*i+1] = Xk[2*(q+M*i)+1];
        }
        FFT0003 (xlm, Xpq);
        for (i=0; i < L; i++)
        {
            Xk[2*(q+M*i)] = Xpq[2*i];
            Xk[2*(q+M*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

#if 0
//------------------------------------------------------------------------------

// 81-point FFT
// Input
//     xn : array of 81 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 81 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0081_16t32d (long *xn, long *Xk)
{
    int l, q;
    int i, k;
    long a, b;
    short c, d;
    int L, M;
    long xlm[20];
    long Xpq[20];
    long g, h;

    M = 27;
    L = 3;

    for (l=0; l < L; l++)
    {
        for (i=0; i < M; i++)
        {
            xlm[2*i] = xn[2*(l+L*i)];
            xlm[2*i+1] = xn[2*(l+L*i)+1];
        }
        // M-point DFT
        //
        FFT0027_16t32d (xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q=0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = W16t0081[2*k];
            d = W16t0081[2*k+1];
            i = M*l + q;

            // Xk[2*i] = (a*c - b*d);
            g = MULT16BY32(c, a);
            h = MULT16BY32(d, b);
            Xk[2*i] = L_sub(g, h);

            // Xk[2*i+1] = (a*d + b*c);
            g = MULT16BY32(d, a);
            h = MULT16BY32(c, b);
            Xk[2*i+1] = L_add(g, h);
        }
    }

    // L-point DFT for the M columns
    //
    for (q=0; q < M; q++)
    {
        for (i=0; i < L; i++)
        {
            xlm[2*i] = Xk[2*(q+M*i)];
            xlm[2*i+1] = Xk[2*(q+M*i)+1];
        }
        FFT0003_16t32d (xlm, Xpq);
        for (i=0; i < L; i++)
        {
            Xk[2*(q+M*i)] = Xpq[2*i];
            Xk[2*(q+M*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

#if 0
//------------------------------------------------------------------------------

// 24-point FFT
// Input
//     xn : array of 24 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 24 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0024 (double *xn, double *Xk)
{
    int l, q;
    int i, k;
    double a, b, c, d;
    int L, M;
    double xlm[60];
    double Xpq[60];

    M = 8;
    L = 3;

    for (l=0; l < L; l++)
    {
        for (i=0; i < M; i++)
        {
            xlm[2*i] = xn[2*(l+L*i)];
            xlm[2*i+1] = xn[2*(l+L*i)+1];
        }
        // M-point DFT
        //
        FFT0008 (xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q=0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = W0024[2*k];
            d = W0024[2*k+1];
            i = M*l + q;
            Xk[2*i] = (a*c - b*d);
            Xk[2*i+1] = (a*d + b*c);
        }
    }

    // L-point DFT for the M columns
    //
    for (q=0; q < M; q++)
    {
        for (i=0; i < L; i++)
        {
            xlm[2*i] = Xk[2*(q+M*i)];
            xlm[2*i+1] = Xk[2*(q+M*i)+1];
        }
        FFT0003 (xlm, Xpq);
        for (i=0; i < L; i++)
        {
            Xk[2*(q+M*i)] = Xpq[2*i];
            Xk[2*(q+M*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

//------------------------------------------------------------------------------

// 24-point FFT
// Input
//     xn : array of 24 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 24 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0024_16t32d(long *xn, long *Xk)
{
    int l, q;
    int i, k;
    long a, b;
    short c, d;
    int L, M;
    long xlm[20];
    long Xpq[20];
    long g, h;

    M = 8;
    L = 3;

    for (l = 0; l < L; l++)
    {
        for (i = 0; i < M; i++)
        {
            xlm[2 * i] = xn[2 * (l + L * i)];
            xlm[2 * i + 1] = xn[2 * (l + L * i) + 1];
        }
        // M-point DFT
        //
        FFT0008_16t32d(xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q = 0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2 * q];
            b = Xpq[2 * q + 1];
            c = W16t0024[2 * k];
            d = W16t0024[2 * k + 1];
            i = M * l + q;

            // Xk[2*i] = (a*c - b*d);
            g = MULT16BY32(c, a);
            h = MULT16BY32(d, b);
            Xk[2 * i] = L_sub(g, h);

            // Xk[2*i+1] = (a*d + b*c);
            g = MULT16BY32(d, a);
            h = MULT16BY32(c, b);
            Xk[2 * i + 1] = L_add(g, h);
        }
    }

    // L-point DFT for the M columns
    //
    for (q = 0; q < M; q++)
    {
        for (i = 0; i < L; i++)
        {
            xlm[2 * i] = Xk[2 * (q + M * i)];
            xlm[2 * i + 1] = Xk[2 * (q + M * i) + 1];
        }
        FFT0003_16t32d(xlm, Xpq);
        for (i = 0; i < L; i++)
        {
            Xk[2 * (q + M * i)] = Xpq[2 * i];
            Xk[2 * (q + M * i) + 1] = Xpq[2 * i + 1];
        }
    }

    return;
}

#if 0
//------------------------------------------------------------------------------

// 30-point FFT
// Input
//     xn : array of 30 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 30 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0030 (double *xn, double *Xk)
{
    int l, q;
    int i, k;
    double a, b, c, d;
    int L, M;
    double *pTable;
    double xlm[40];
    double Xpq[40];

    M = 15;
    L = 2;
    pTable = W0030;

    for (l=0; l < L; l++)
    {
        for (i=0; i < M; i++)
        {
            xlm[2*i] = xn[2*(l+L*i)];
            xlm[2*i+1] = xn[2*(l+L*i)+1];
        }
        // M-point DFT
        //
        FFT0015 (xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q=0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = pTable[2*k];
            d = pTable[2*k+1];
            i = M*l + q;
            Xk[2*i] = (a*c - b*d);
            Xk[2*i+1] = (a*d + b*c);
        }
    }

    // L-point DFT for the M columns
    //
    for (q=0; q < M; q++)
    {
        for (i=0; i < L; i++)
        {
            xlm[2*i] = Xk[2*(q+M*i)];
            xlm[2*i+1] = Xk[2*(q+M*i)+1];
        }

        // 2-point DFT
        //
        // Xk[0] = xn[0] + xn[1]
        //
        Xpq[0] = xlm[0] + xlm[2];
        Xpq[1] = xlm[1] + xlm[3];

        // Xk[1] = xn[0] - xn[1]
        //
        Xpq[2] = xlm[0] - xlm[2];
        Xpq[3] = xlm[1] - xlm[3];

        for (i=0; i < L; i++)
        {
            Xk[2*(q+M*i)] = Xpq[2*i];
            Xk[2*(q+M*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

//------------------------------------------------------------------------------

// 30-point FFT
// Input
//     xn : array of 30 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 30 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0030_16t32d(long *xn, long *Xk)
{
    int l, q;
    int i, k;
    long a, b;
    short c, d;
    int L, M;
    short *pTable;
    long xlm[32];
    long Xpq[32];
    long g, h;

    M = 15;
    L = 2;
    pTable = W16t0030;

    for (l = 0; l < L; l++)
    {
        for (i = 0; i < M; i++)
        {
            xlm[2 * i] = xn[2 * (l + L * i)];
            xlm[2 * i + 1] = xn[2 * (l + L * i) + 1];
        }
        // M-point DFT
        //
        FFT0015_16t32d(xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q = 0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2 * q];
            b = Xpq[2 * q + 1];
            c = pTable[2 * k];
            d = pTable[2 * k + 1];
            i = M * l + q;

            // Xk[2*i] = (a*c - b*d);
            g = MULT16BY32(c, a);
            h = MULT16BY32(d, b);
            Xk[2 * i] = L_sub(g, h);

            // Xk[2*i+1] = (a*d + b*c);
            g = MULT16BY32(d, a);
            h = MULT16BY32(c, b);
            Xk[2 * i + 1] = L_add(g, h);
        }
    }

    // L-point DFT for the M columns
    //
    for (q = 0; q < M; q++)
    {
        for (i = 0; i < L; i++)
        {
            xlm[2 * i] = Xk[2 * (q + M * i)];
            xlm[2 * i + 1] = Xk[2 * (q + M * i) + 1];
        }

        // // 2-point DFT
        // //
        // // Xk[0] = xn[0] + xn[1]
        // //
        // Xpq[0] = xlm[0] + xlm[2];
        Xpq[0] = L_add(xlm[0], xlm[2]);

        // Xpq[1] = xlm[1] + xlm[3];
        Xpq[1] = L_add(xlm[1], xlm[3]);

        // // Xk[1] = xn[0] - xn[1]
        // //
        // Xpq[2] = xlm[0] - xlm[2];
        Xpq[2] = L_sub(xlm[0], xlm[2]);

        // Xpq[3] = xlm[1] - xlm[3];
        Xpq[3] = L_sub(xlm[1], xlm[3]);

        for (i = 0; i < L; i++)
        {
            Xk[2 * (q + M * i)] = Xpq[2 * i];
            Xk[2 * (q + M * i) + 1] = Xpq[2 * i + 1];
        }
    }

    return;
}

#if 0
//------------------------------------------------------------------------------

// 36-point FFT
// Input
//     xn : array of 36 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 36 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0036 (double *xn, double *Xk)
{
    int l, q;
    int i, k;
    double a, b, c, d;
    int L, M;
    double *pTable;
    double xlm[60];
    double Xpq[60];

    M = 9;
    L = 4;
    pTable = W0036;

    for (l=0; l < L; l++)
    {
        for (i=0; i < M; i++)
        {
            xlm[2*i] = xn[2*(l+L*i)];
            xlm[2*i+1] = xn[2*(l+L*i)+1];
        }
        // M-point DFT
        //
        FFT0009 (xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q=0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = pTable[2*k];
            d = pTable[2*k+1];
            i = M*l + q;
            Xk[2*i] = (a*c - b*d);
            Xk[2*i+1] = (a*d + b*c);
        }
    }

    // L-point DFT for the M columns
    //
    for (q=0; q < M; q++)
    {
        for (i=0; i < L; i++)
        {
            xlm[2*i] = Xk[2*(q+M*i)];
            xlm[2*i+1] = Xk[2*(q+M*i)+1];
        }
        FFT0004 (xlm, Xpq);
        for (i=0; i < L; i++)
        {
            Xk[2*(q+M*i)] = Xpq[2*i];
            Xk[2*(q+M*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

//------------------------------------------------------------------------------

// 36-point FFT
// Input
//     xn : array of 36 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 36 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0036_16t32d(long *xn, long *Xk)
{
    int l, q;
    int i, k;
    long a, b;
    short c, d;
    int L, M;
    short *pTable;
    long xlm[20];
    long Xpq[20];
    long g, h;

    M = 9;
    L = 4;
    pTable = W16t0036;

    for (l = 0; l < L; l++)
    {
        for (i = 0; i < M; i++)
        {
            xlm[2 * i] = xn[2 * (l + L * i)];
            xlm[2 * i + 1] = xn[2 * (l + L * i) + 1];
        }
        // M-point DFT
        //
        FFT0009_16t32d(xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q = 0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2 * q];
            b = Xpq[2 * q + 1];
            c = pTable[2 * k];
            d = pTable[2 * k + 1];
            i = M * l + q;

            // Xk[2*i] = (a*c - b*d);
            g = MULT16BY32(c, a);
            h = MULT16BY32(d, b);
            Xk[2 * i] = L_sub(g, h);

            // Xk[2*i+1] = (a*d + b*c);
            g = MULT16BY32(d, a);
            h = MULT16BY32(c, b);
            Xk[2 * i + 1] = L_add(g, h);
        }
    }

    // L-point DFT for the M columns
    //
    for (q = 0; q < M; q++)
    {
        for (i = 0; i < L; i++)
        {
            xlm[2 * i] = Xk[2 * (q + M * i)];
            xlm[2 * i + 1] = Xk[2 * (q + M * i) + 1];
        }
        FFT0004_16t32d(xlm, Xpq);
        for (i = 0; i < L; i++)
        {
            Xk[2 * (q + M * i)] = Xpq[2 * i];
            Xk[2 * (q + M * i) + 1] = Xpq[2 * i + 1];
        }
    }

    return;
}

#if 0
//------------------------------------------------------------------------------

// 32-point FFT
// Input
//     xn : array of 32 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 32 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0032 (double *xn, double *Xk)
{
    int l, q;
    int i, k;
    double a, b, c, d;
    int L, M;
    double *pTable;
    double xlm[20];
    double Xpq[20];

    M = 8;
    L = 4;
    pTable = W0032;

    for (l=0; l < L; l++)
    {
        for (i=0; i < M; i++)
        {
            xlm[2*i] = xn[2*(l+L*i)];
            xlm[2*i+1] = xn[2*(l+L*i)+1];
        }
        // M-point DFT
        //
        FFT0008 (xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q=0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = pTable[2*k];
            d = pTable[2*k+1];
            i = M*l + q;
            Xk[2*i] = (a*c - b*d);
            Xk[2*i+1] = (a*d + b*c);
        }
    }

    // L-point DFT for the M columns
    //
    for (q=0; q < M; q++)
    {
        for (i=0; i < L; i++)
        {
            xlm[2*i] = Xk[2*(q+M*i)];
            xlm[2*i+1] = Xk[2*(q+M*i)+1];
        }
        FFT0004 (xlm, Xpq);
        for (i=0; i < L; i++)
        {
            Xk[2*(q+M*i)] = Xpq[2*i];
            Xk[2*(q+M*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

//------------------------------------------------------------------------------

// 32-point FFT
// Input
//     xn : array of 32 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 32 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0032_16t32d(long *xn, long *Xk)
{
    int l, q;
    int i, k;
    long a, b;
    short c, d;
    int L, M;
    short *pTable;
    long xlm[20];
    long Xpq[20];
    long g, h;

    M = 8;
    L = 4;
    pTable = W16t0032;

    for (l = 0; l < L; l++)
    {
        for (i = 0; i < M; i++)
        {
            xlm[2 * i] = xn[2 * (l + L * i)];
            xlm[2 * i + 1] = xn[2 * (l + L * i) + 1];
        }
        // M-point DFT
        //
        FFT0008_16t32d(xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q = 0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2 * q];
            b = Xpq[2 * q + 1];
            c = pTable[2 * k];
            d = pTable[2 * k + 1];
            i = M * l + q;

            // Xk[2*i] = (a*c - b*d);
            g = MULT16BY32(c, a);
            h = MULT16BY32(d, b);
            Xk[2 * i] = L_sub(g, h);

            // Xk[2*i+1] = (a*d + b*c);
            g = MULT16BY32(d, a);
            h = MULT16BY32(c, b);
            Xk[2 * i + 1] = L_add(g, h);
        }
    }

    // L-point DFT for the M columns
    //
    for (q = 0; q < M; q++)
    {
        for (i = 0; i < L; i++)
        {
            xlm[2 * i] = Xk[2 * (q + M * i)];
            xlm[2 * i + 1] = Xk[2 * (q + M * i) + 1];
        }
        FFT0004_16t32d(xlm, Xpq);
        for (i = 0; i < L; i++)
        {
            Xk[2 * (q + M * i)] = Xpq[2 * i];
            Xk[2 * (q + M * i) + 1] = Xpq[2 * i + 1];
        }
    }

    return;
}

#if 0
//------------------------------------------------------------------------------

// 45-point FFT
// Input
//     xn : array of 45 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 45 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0045 (double *xn, double *Xk)
{
    int l, q;
    int i, k;
    double a, b, c, d;
    int L, M;
    double *pTable;
    double xlm[60];
    double Xpq[60];

    M = 9;
    L = 5;
    pTable = W0045;

    for (l=0; l < L; l++)
    {
        for (i=0; i < M; i++)
        {
            xlm[2*i] = xn[2*(l+L*i)];
            xlm[2*i+1] = xn[2*(l+L*i)+1];
        }
        // M-point DFT
        //
        FFT0009 (xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q=0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = pTable[2*k];
            d = pTable[2*k+1];
            i = M*l + q;
            Xk[2*i] = (a*c - b*d);
            Xk[2*i+1] = (a*d + b*c);
        }
    }

    // L-point DFT for the M columns
    //
    for (q=0; q < M; q++)
    {
        for (i=0; i < L; i++)
        {
            xlm[2*i] = Xk[2*(q+M*i)];
            xlm[2*i+1] = Xk[2*(q+M*i)+1];
        }
        FFT0005 (xlm, Xpq);
        for (i=0; i < L; i++)
        {
            Xk[2*(q+M*i)] = Xpq[2*i];
            Xk[2*(q+M*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

//------------------------------------------------------------------------------

// 45-point FFT
// Input
//     xn : array of 45 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 45 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0045_16t32d(long *xn, long *Xk)
{
    int l, q;
    int i, k;
    long a, b;
    short c, d;
    int L, M;
    short *pTable;
    long xlm[20];
    long Xpq[20];
    long g, h;

    M = 9;
    L = 5;
    pTable = W16t0045;

    for (l = 0; l < L; l++)
    {
        for (i = 0; i < M; i++)
        {
            xlm[2 * i] = xn[2 * (l + L * i)];
            xlm[2 * i + 1] = xn[2 * (l + L * i) + 1];
        }
        // M-point DFT
        //
        FFT0009_16t32d(xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q = 0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2 * q];
            b = Xpq[2 * q + 1];
            c = pTable[2 * k];
            d = pTable[2 * k + 1];
            i = M * l + q;

            // Xk[2*i] = (a*c - b*d);
            g = MULT16BY32(c, a);
            h = MULT16BY32(d, b);
            Xk[2 * i] = L_sub(g, h);

            // Xk[2*i+1] = (a*d + b*c);
            g = MULT16BY32(d, a);
            h = MULT16BY32(c, b);
            Xk[2 * i + 1] = L_add(g, h);
        }
    }

    // L-point DFT for the M columns
    //
    for (q = 0; q < M; q++)
    {
        for (i = 0; i < L; i++)
        {
            xlm[2 * i] = Xk[2 * (q + M * i)];
            xlm[2 * i + 1] = Xk[2 * (q + M * i) + 1];
        }
        FFT0005_16t32d(xlm, Xpq);
        for (i = 0; i < L; i++)
        {
            Xk[2 * (q + M * i)] = Xpq[2 * i];
            Xk[2 * (q + M * i) + 1] = Xpq[2 * i + 1];
        }
    }

    return;
}

#if 0
//------------------------------------------------------------------------------

// 48-point FFT
// Input
//     xn : array of 48 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 48 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0048 (double *xn, double *Xk)
{
    int l, q;
    int i, k;
    double a, b, c, d;
    int L, M;
    double *pTable;
    double xlm[60];
    double Xpq[60];

    M = 12;
    L = 4;
    pTable = W0048;

    for (l=0; l < L; l++)
    {
        for (i=0; i < M; i++)
        {
            xlm[2*i] = xn[2*(l+L*i)];
            xlm[2*i+1] = xn[2*(l+L*i)+1];
        }
        // M-point DFT
        //
        FFT0012 (xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q=0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = pTable[2*k];
            d = pTable[2*k+1];
            i = M*l + q;
            Xk[2*i] = (a*c - b*d);
            Xk[2*i+1] = (a*d + b*c);
        }
    }

    // L-point DFT for the M columns
    //
    for (q=0; q < M; q++)
    {
        for (i=0; i < L; i++)
        {
            xlm[2*i] = Xk[2*(q+M*i)];
            xlm[2*i+1] = Xk[2*(q+M*i)+1];
        }
        FFT0004 (xlm, Xpq);
        for (i=0; i < L; i++)
        {
            Xk[2*(q+M*i)] = Xpq[2*i];
            Xk[2*(q+M*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

//------------------------------------------------------------------------------

// 48-point FFT
// Input
//     xn : array of 48 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 48 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0048_16t32d(long *xn, long *Xk)
{
    int l, q;
    int i, k;
    long a, b;
    short c, d;
    int L, M;
    short *pTable;
    long xlm[30];
    long Xpq[30];
    long g, h;

    M = 12;
    L = 4;
    pTable = W16t0048;

    for (l = 0; l < L; l++)
    {
        for (i = 0; i < M; i++)
        {
            xlm[2 * i] = xn[2 * (l + L * i)];
            xlm[2 * i + 1] = xn[2 * (l + L * i) + 1];
        }
        // M-point DFT
        //
        FFT0012_16t32d(xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q = 0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2 * q];
            b = Xpq[2 * q + 1];
            c = pTable[2 * k];
            d = pTable[2 * k + 1];
            i = M * l + q;

            // Xk[2*i] = (a*c - b*d);
            g = MULT16BY32(c, a);
            h = MULT16BY32(d, b);
            Xk[2 * i] = L_sub(g, h);

            // Xk[2*i+1] = (a*d + b*c);
            g = MULT16BY32(d, a);
            h = MULT16BY32(c, b);
            Xk[2 * i + 1] = L_add(g, h);
        }
    }

    // L-point DFT for the M columns
    //
    for (q = 0; q < M; q++)
    {
        for (i = 0; i < L; i++)
        {
            xlm[2 * i] = Xk[2 * (q + M * i)];
            xlm[2 * i + 1] = Xk[2 * (q + M * i) + 1];
        }
        FFT0004_16t32d(xlm, Xpq);
        for (i = 0; i < L; i++)
        {
            Xk[2 * (q + M * i)] = Xpq[2 * i];
            Xk[2 * (q + M * i) + 1] = Xpq[2 * i + 1];
        }
    }

    return;
}

#if 0
//------------------------------------------------------------------------------

// 60-point FFT
// Input
//     xn : array of 60 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of 60 complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   *Xk DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*Xk           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void FFT0060 (double *xn, double *Xk)
{
    int l, q;
    int i, k;
    double a, b, c, d;
    int L, M;
    double *pTable;
    double xlm[60];
    double Xpq[60];

    M = 15;
    L = 4;
    pTable = W0060;

    for (l=0; l < L; l++)
    {
        for (i=0; i < M; i++)
        {
            xlm[2*i] = xn[2*(l+L*i)];
            xlm[2*i+1] = xn[2*(l+L*i)+1];
        }
        // M-point DFT
        //
        FFT0015 (xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q=0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = pTable[2*k];
            d = pTable[2*k+1];
            i = M*l + q;
            Xk[2*i] = (a*c - b*d);
            Xk[2*i+1] = (a*d + b*c);
        }
    }

    // L-point DFT for the M columns
    //
    for (q=0; q < M; q++)
    {
        for (i=0; i < L; i++)
        {
            xlm[2*i] = Xk[2*(q+M*i)];
            xlm[2*i+1] = Xk[2*(q+M*i)+1];
        }
        FFT0004 (xlm, Xpq);
        for (i=0; i < L; i++)
        {
            Xk[2*(q+M*i)] = Xpq[2*i];
            Xk[2*(q+M*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

#if 0
//------------------------------------------------------------------------------

// N-point FFT
// Input
//     xn : array of M*L complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of M*L complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   short M DOXYGEN_TO_DO
 *  @param   short L DOXYGEN_TO_DO
 *  @param   (*FFT0M DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |short M       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |short L       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |(*FFT0M       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void LteDFT2D_1 (double *xn, unsigned short M, unsigned short L, void (*FFT0M)(), void (*FFT0L)(), double *pTwiddlwTableN,
    double *Xk)
{
    int l, q;
    int i, k;
    double a, b, c, d;
    double xlm[512];
    double Xpq[512];

    for (l=0; l < L; l++)
    {
        for (i=0; i < M; i++)
        {
            xlm[2*i] = xn[2*(l+L*i)];
            xlm[2*i+1] = xn[2*(l+L*i)+1];
        }
        // M-point DFT
        //
        FFT0M (xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q=0; q < M; q++)
        {
            k = l * q;
            a = Xpq[2*q];
            b = Xpq[2*q+1];
            c = pTwiddlwTableN[2*k];
            d = pTwiddlwTableN[2*k+1];
            i = M*l + q;
            Xk[2*i] = (a*c - b*d);
            Xk[2*i+1] = (a*d + b*c);
        }
    }

    // L-point DFT for the M columns
    //
    for (q=0; q < M; q++)
    {
        for (i=0; i < L; i++)
        {
            xlm[2*i] = Xk[2*(q+M*i)];
            xlm[2*i+1] = Xk[2*(q+M*i)+1];
        }
        FFT0L (xlm, Xpq);
        for (i=0; i < L; i++)
        {
            Xk[2*(q+M*i)] = Xpq[2*i];
            Xk[2*(q+M*i)+1] = Xpq[2*i+1];
        }
    }

    return;
}
#endif

//------------------------------------------------------------------------------

// N-point FFT (16t32d version)
// Input
//     xn : array of M*L complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of M*L complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   *xn DOXYGEN_TO_DO
 *  @param   short M DOXYGEN_TO_DO
 *  @param   short L DOXYGEN_TO_DO
 *  @param   (*FFT0M DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*xn           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |short M       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |short L       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |(*FFT0M       |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void LteDFT2D_16t32d(long *xn, unsigned short M, unsigned short L, void(*FFT0M)(), void(*FFT0L)(),
                     short *pTwiddlwTableN, unsigned short wStep, long *Xk)
{
    int l, q;
    int i, k;
    long a, b;
    short c, d;
    long xlm[100];
    long Xpq[100];
    long g, h;

    for (l = 0; l < L; l++)
    {
        for (i = 0; i < M; i++)
        {
            xlm[2 * i] = xn[2 * (l + L * i)];
            xlm[2 * i + 1] = xn[2 * (l + L * i) + 1];
        }
        // M-point DFT
        //
        FFT0M(xlm, Xpq);

        // Multiply the DFT results by W_N^(l*q)
        //
        for (q = 0; q < M; q++)
        {
            k = l * q * wStep;
            a = Xpq[2 * q];
            b = Xpq[2 * q + 1];
            c = pTwiddlwTableN[2 * k];
            d = pTwiddlwTableN[2 * k + 1];
            i = M * l + q;

            // Xk[2*i] = (a*c - b*d);
            g = MULT16BY32(c, a);
            h = MULT16BY32(d, b);
            Xk[2 * i] = L_sub(g, h);

            // Xk[2*i+1] = (a*d + b*c);
            g = MULT16BY32(d, a);
            h = MULT16BY32(c, b);
            Xk[2 * i + 1] = L_add(g, h);
        }
    }

    // L-point DFT for the M columns
    //
    for (q = 0; q < M; q++)
    {
        for (i = 0; i < L; i++)
        {
            xlm[2 * i] = Xk[2 * (q + M * i)];
            xlm[2 * i + 1] = Xk[2 * (q + M * i) + 1];
        }
        FFT0L(xlm, Xpq);
        for (i = 0; i < L; i++)
        {
            Xk[2 * (q + M * i)] = Xpq[2 * i];
            Xk[2 * (q + M * i) + 1] = Xpq[2 * i + 1];
        }
    }

    return;
}

//------------------------------------------------------------------------------

// N-point DFT (16t32d version)
// Input
//     xn : array of N complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     Xk : array of N complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL LteDFT_16t32d function (fixed point version)
 *         16-bit table, 32-bit data, both input and output are 16-bit
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *  @param xn    (input) pointer to the input array to be transformed.
 *                       This is an array of complex numbers, arranged as real part and img part
 *                       interleave together, i.e xn[0].real, xn[0].img, xn[1].real, xn[1].img, ...
 *                       The size of this array is 2*N.
 *
 *  @param N     (input) DFT size is N. The value N must be one of the value in following list.
 *                       {12, 24, 36, 48, 60, 72, 96, 108, 120, 144, 180, 192, 216, 240, 288,
 *                       300, 324, 360, 384, 432, 480, 540, 576, 600, 648, 720, 768, 864,
 *                       900, 960, 972, 1080, 1152, 1200, 1296}
 *
 *  @param Xk   (output) pointer to the output array.
 *                       This is an array of complex numbers, arranged as real part and img part
 *                       interleave together, i.e Xk[0].real, Xk[0].img, Xk[1].real, Xk[1].img, ...
 *                       The size of this array is 2*N.
 *
 *  @param Exp2 (output) Return the the exponent of the output array. All the elements of the output
 *                       array have a scaling factor of 2^Exp2
 *
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------
 *  |Name   |TCB Type       |Size         |Data Format    |Valid Range   |Default        |Units        |
 *  ----------------------------------------------------------------------------------------------------
 *  |xn     |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  |N      |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  |Xk     |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  |Exp2   |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteDFT_16t32d               |
 *  ---------------------------------------------------
 *  |Input Streams       |DOXYGEN_TO_DO               |
 *  ---------------------------------------------------
 *  |Output Streams      |DOXYGEN_TO_DO               |
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
 * This function performs DFT (Discrete Fourier Transform) on a complex valued array of N points.
 * The value N is in the form of N = [2^(p) * 3^(q) * 5^(r)], where p, q, and r are positive
 * integers. The values of N supported in this function are listed below.
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
void LteDFT_16t32d(short *xn, unsigned short N, short *Xk, short *Exp2)
{
    unsigned short M, L = 0, wStep;
    void (*pFFT0M)() = NULL;
    void (*pFFT0L)() = NULL;
    short *pTwiddlwTableN = NULL;
    short E2 = 0;

    // SqNInv = (short) (32768.0/sqrt((double)N) + 0.5);
    wStep = 1;
    M = 0;
    switch (N)
    {

#if 0
        // Disable basic blocks.
        // These codes are for testing only

        case 3:
        E2 = 0;
        Convert_16to32 (xn, N, xIn32, &E2);
        M = 0;
        FFT0003_16t32d (xIn32, Xk32);
        SqNInv = 18919;
        EsqN = 0;
        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
        break;

        case 4:
        E2 = 0;
        Convert_16to32 (xn, N, xIn32, &E2);
        M = 0;
        FFT0004_16t32d (xIn32, Xk32);
        SqNInv = 16384;
        EsqN = 0;
        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
        break;

        case 5:
        E2 = 0;
        Convert_16to32 (xn, N, xIn32, &E2);
        M = 0;
        FFT0005_16t32d (xIn32, Xk32);
        SqNInv = 29309;
        EsqN = -1;
        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
        break;

        case 25:
        E2 = 0;
        Convert_16to32 (xn, N, xIn32, &E2);
        M = 0;
        FFT0025_16t32d (xIn32, Xk32);
        SqNInv = 26214;
        EsqN = -2;
        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
        break;

        case 8:
        E2 = 0;
        Convert_16to32 (xn, N, xIn32, &E2);
        M = 0;
        FFT0008_16t32d (xIn32, Xk32);
        SqNInv = 23170;
        EsqN = -1;
        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
        break;

        case 9:
        E2 = 0;
        Convert_16to32 (xn, N, xIn32, &E2);
        M = 0;
        FFT0009_16t32d (xIn32, Xk32);
        SqNInv = 21845;
        EsqN = -1;
        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
        break;

        case 12:
        E2 = 0;
        Convert_16to32 (xn, N, xIn32, &E2);
        M = 0;
        FFT0012_16t32d (xIn32, Xk32);
        SqNInv = 18919;
        EsqN = -1;
        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
        break;

        case 15:
        E2 = 0;
        Convert_16to32 (xn, N, xIn32, &E2);
        M = 0;
        FFT0015_16t32d (xIn32, Xk32);
        SqNInv = 16921;
        EsqN = -1;
        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
        break;

        case 16:
        E2 = 0;
        Convert_16to32 (xn, N, xIn32, &E2);
        M = 0;
        FFT0016_16t32d (xIn32, Xk32);
        SqNInv = 16384;
        EsqN = -1;
        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
        break;

        case 24:
        E2 = 0;
        Convert_16to32 (xn, N, xIn32, &E2);
        M = 0;
        FFT0024_16t32d (xIn32, Xk32);
        SqNInv = 26755;
        EsqN = -2;
        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
        break;

        case 27:
        E2 = 0;
        Convert_16to32 (xn, N, xIn32, &E2);
        M = 0;
        FFT0027_16t32d (xIn32, Xk32);
        SqNInv = 25225;
        EsqN = -2;
        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
        break;

        case 30:
        E2 = 0;
        Convert_16to32 (xn, N, xIn32, &E2);
        M = 0;
        FFT0030_16t32d (xIn32, Xk32);
        SqNInv = 23930;
        EsqN = -2;
        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
        break;

        case 32:
        E2 = 0;
        Convert_16to32 (xn, N, xIn32, &E2);
        M = 0;
        FFT0032_16t32d (xIn32, Xk32);
        SqNInv = 23170;
        EsqN = -2;
        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
        break;

        case 36:
        E2 = 0;
        Convert_16to32 (xn, N, xIn32, &E2);
        M = 0;
        FFT0036_16t32d (xIn32, Xk32);
        SqNInv = 21845;
        EsqN = -2;
        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
        break;

        case 45:
        E2 = 0;
        Convert_16to32 (xn, N, xIn32, &E2);
        M = 0;
        FFT0045_16t32d (xIn32, Xk32);
        SqNInv = 19539;
        EsqN = -2;
        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
        break;

        case 48:
        E2 = 0;
        Convert_16to32 (xn, N, xIn32, &E2);
        M = 0;
        FFT0048_16t32d (xIn32, Xk32);
        SqNInv = 18919;
        EsqN = -2;
        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
        break;

        case 60:
        M = 15;
        L = 4;
        pFFT0M = FFT0015_16t32d;
        pFFT0L = FFT0004_16t32d;
        pTwiddlwTableN = W16t0960;
        wStep = 16;
        SqNInv = 16921;
        EsqN = -2;
        break;
#endif

        case 12:
            E2 = 0;
            Convert_16to32(xn, N, xIn32, &E2);
            M = 0;
            FFT0012_16t32d(xIn32, Xk32);
            SqNInv = 18919;
            EsqN = -1;
            DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
            *Exp2 = E2;
            break;
        case 24:
            E2 = 0;
            Convert_16to32(xn, N, xIn32, &E2);
            M = 0;
            FFT0024_16t32d(xIn32, Xk32);
            SqNInv = 26755;
            EsqN = -2;
            DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
            *Exp2 = E2;
            break;
        case 36:
            E2 = 0;
            Convert_16to32(xn, N, xIn32, &E2);
            M = 0;
            FFT0036_16t32d(xIn32, Xk32);
            SqNInv = 21845;
            EsqN = -2;
            DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
            *Exp2 = E2;
            break;
        case 48:
            E2 = 0;
            Convert_16to32(xn, N, xIn32, &E2);
            M = 0;
            FFT0048_16t32d(xIn32, Xk32);
            SqNInv = 18919;
            EsqN = -2;
            DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
            *Exp2 = E2;
            break;

        case 60:
            M = 15;
            L = 4;
            pFFT0M = FFT0015_16t32d;
            pFFT0L = FFT0004_16t32d;
            pTwiddlwTableN = W16t0960;
            wStep = 16;
            SqNInv = 16921;
            EsqN = -2;
            break;

        case 72:
            M = 9;
            L = 8;
            pFFT0M = FFT0009_16t32d;
            pFFT0L = FFT0008_16t32d;
            pTwiddlwTableN = W16t1152;
            wStep = 16;
            SqNInv = 30894;
            EsqN = -3;
            break;

        case 96:
            M = 12;
            L = 8;
            pFFT0M = FFT0012_16t32d;
            pFFT0L = FFT0008_16t32d;
            pTwiddlwTableN = W16t0768;
            wStep = 8;
            SqNInv = 26755;
            EsqN = -3;
            break;

        case 108:
            M = 12;
            L = 9;
            pFFT0M = FFT0012_16t32d;
            pFFT0L = FFT0009_16t32d;
            pTwiddlwTableN = W16t0864;
            wStep = 8;
            SqNInv = 25225;
            EsqN = -3;
            break;

        case 120:
            M = 15;
            L = 8;
            pFFT0M = FFT0015_16t32d;
            pFFT0L = FFT0008_16t32d;
            pTwiddlwTableN = W16t0960;
            wStep = 8;
            SqNInv = 23930;
            EsqN = -3;
            break;

        case 144:
            M = 12;
            L = 12;
            pFFT0M = FFT0012_16t32d;
            pFFT0L = FFT0012_16t32d;
            pTwiddlwTableN = W16t1152;
            wStep = 8;
            SqNInv = 21845;
            EsqN = -3;
            break;

        case 180:
            M = 15;
            L = 12;
            pFFT0M = FFT0015_16t32d;
            pFFT0L = FFT0012_16t32d;
            pTwiddlwTableN = W16t0720;
            wStep = 4;
            SqNInv = 19539;
            EsqN = -3;
            break;

        case 192:
            M = 24;
            L = 8;
            pFFT0M = FFT0024_16t32d;
            pFFT0L = FFT0008_16t32d;
            pTwiddlwTableN = W16t0768;
            wStep = 4;
            SqNInv = 18919;
            EsqN = -3;
            break;

        case 216:
            M = 24;
            L = 9;
            pFFT0M = FFT0024_16t32d;
            pFFT0L = FFT0009_16t32d;
            pTwiddlwTableN = W16t0864;
            wStep = 4;
            SqNInv = 17837;
            EsqN = -3;
            break;

        case 240:
            M = 16;
            L = 15;
            pFFT0M = FFT0016_16t32d;
            pFFT0L = FFT0015_16t32d;
            pTwiddlwTableN = W16t0960;
            wStep = 4;
            SqNInv = 16921;
            EsqN = -3;
            break;

        case 288:
            M = 36;
            L = 8;
            pFFT0M = FFT0036_16t32d;
            pFFT0L = FFT0008_16t32d;
            pTwiddlwTableN = W16t1152;
            wStep = 4;
            SqNInv = 30894;
            EsqN = -4;
            break;

        case 300:
            M = 25;
            L = 12;
            pFFT0M = FFT0025_16t32d;
            pFFT0L = FFT0012_16t32d;
            pTwiddlwTableN = W16t1200;
            wStep = 4;
            SqNInv = 30270;
            EsqN = -4;
            break;

        case 324:
            M = 27;
            L = 12;
            pFFT0M = FFT0027_16t32d;
            pFFT0L = FFT0012_16t32d;
            pTwiddlwTableN = W16t1296;
            wStep = 4;
            SqNInv = 29127;
            EsqN = -4;
            break;

        case 360:
            M = 24;
            L = 15;
            pFFT0M = FFT0024_16t32d;
            pFFT0L = FFT0015_16t32d;
            pTwiddlwTableN = W16t0720;
            wStep = 2;
            SqNInv = 27632;
            EsqN = -4;
            break;

        case 384:
            M = 24;
            L = 16;
            pFFT0M = FFT0024_16t32d;
            pFFT0L = FFT0016_16t32d;
            pTwiddlwTableN = W16t0768;
            wStep = 2;
            SqNInv = 26755;
            EsqN = -4;
            break;

        case 432:
            M = 27;
            L = 16;
            pFFT0M = FFT0027_16t32d;
            pFFT0L = FFT0016_16t32d;
            pTwiddlwTableN = W16t0864;
            wStep = 2;
            SqNInv = 25225;
            EsqN = -4;
            break;

        case 480:
            M = 32;
            L = 15;
            pFFT0M = FFT0032_16t32d;
            pFFT0L = FFT0015_16t32d;
            pTwiddlwTableN = W16t0960;
            wStep = 2;
            SqNInv = 23930;
            EsqN = -4;
            break;

        case 540:
            M = 36;
            L = 15;
            pFFT0M = FFT0036_16t32d;
            pFFT0L = FFT0015_16t32d;
            pTwiddlwTableN = W16t1080;
            wStep = 2;
            SqNInv = 22562;
            EsqN = -4;
            break;

        case 576:
            M = 36;
            L = 16;
            pFFT0M = FFT0036_16t32d;
            pFFT0L = FFT0016_16t32d;
            pTwiddlwTableN = W16t1152;
            wStep = 2;
            SqNInv = 21845;
            EsqN = -4;
            break;

        case 600:
            M = 25;
            L = 24;
            pFFT0M = FFT0025_16t32d;
            pFFT0L = FFT0024_16t32d;
            pTwiddlwTableN = W16t1200;
            wStep = 2;
            SqNInv = 21404;
            EsqN = -4;
            break;

        case 648:
            M = 27;
            L = 24;
            pFFT0M = FFT0027_16t32d;
            pFFT0L = FFT0024_16t32d;
            pTwiddlwTableN = W16t1296;
            wStep = 2;
            SqNInv = 20596;
            EsqN = -4;
            break;

        case 720:
            M = 45;
            L = 16;
            pFFT0M = FFT0045_16t32d;
            pFFT0L = FFT0016_16t32d;
            pTwiddlwTableN = W16t0720;
            wStep = 1;
            SqNInv = 19539;
            EsqN = -4;
            break;

        case 768:
            M = 32;
            L = 24;
            pFFT0M = FFT0032_16t32d;
            pFFT0L = FFT0024_16t32d;
            pTwiddlwTableN = W16t0768;
            wStep = 1;
            SqNInv = 18919;
            EsqN = -4;
            break;

        case 864:
            M = 32;
            L = 27;
            pFFT0M = FFT0032_16t32d;
            pFFT0L = FFT0027_16t32d;
            pTwiddlwTableN = W16t0864;
            wStep = 1;
            SqNInv = 17837;
            EsqN = -4;
            break;

        case 900:
            M = 36;
            L = 25;
            pFFT0M = FFT0036_16t32d;
            pFFT0L = FFT0025_16t32d;
            pTwiddlwTableN = W16t0900;
            wStep = 1;
            SqNInv = 17476;
            EsqN = -4;
            break;

        case 960:
            M = 32;
            L = 30;
            pFFT0M = FFT0032_16t32d;
            pFFT0L = FFT0030_16t32d;
            pTwiddlwTableN = W16t0960;
            wStep = 1;
            SqNInv = 16921;
            EsqN = -4;
            break;

        case 972:
            M = 36;
            L = 27;
            pFFT0M = FFT0036_16t32d;
            pFFT0L = FFT0027_16t32d;
            pTwiddlwTableN = W16t0972;
            wStep = 1;
            SqNInv = 16817;
            EsqN = -4;
            break;

        case 1080:
            M = 45;
            L = 24;
            pFFT0M = FFT0045_16t32d;
            pFFT0L = FFT0024_16t32d;
            pTwiddlwTableN = W16t1080;
            wStep = 1;
            SqNInv = 31907;
            EsqN = -5;
            break;

        case 1152:
            M = 36;
            L = 32;
            pFFT0M = FFT0036_16t32d;
            pFFT0L = FFT0032_16t32d;
            pTwiddlwTableN = W16t1152;
            wStep = 1;
            SqNInv = 30894;
            EsqN = -5;
            break;

        case 1200:
            M = 48;
            L = 25;
            pFFT0M = FFT0048_16t32d;
            pFFT0L = FFT0025_16t32d;
            pTwiddlwTableN = W16t1200;
            wStep = 1;
            SqNInv = 30270;
            EsqN = -5;
            break;

        case 1296:
            M = 48;
            L = 27;
            pFFT0M = FFT0048_16t32d;
            pFFT0L = FFT0027_16t32d;
            pTwiddlwTableN = W16t1296;
            wStep = 1;
            SqNInv = 29127;
            EsqN = -5;
            break;

        default:
            M = 0;
#ifdef _WIN32
            printf("[LteDFT_16t32d_16t32d] Unsupported DFT size N = %d\n", N);
#endif
            break;
    }
    if (M != 0)
    {
        E2 = 0;
        Convert_16to32(xn, N, xIn32, &E2);
        LteDFT2D_16t32d(xIn32, M, L, pFFT0M, pFFT0L, pTwiddlwTableN, wStep, Xk32);

        DFT_ScaleSqN_16t32d(Xk32, N, Xk, &E2);
        *Exp2 = E2;
    }
    return;
}

//------------------------------------------------------------------------------

// N-point IDFT (16t32d version)
// Input
//     xIn  : array of N complex values. stored as Real(0), Img(0), Real(1), Img(1), ....
// Output
//     yOut : array of N complex values. stored as Real(0), Img(0), Real(1), Img(1), ....


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL LteIDFT_16t32d function (fixed point version)
 *         16-bit table, 32-bit data, both input and output are 16-bit
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *  @param xIn    (input) pointer to the input array to be transformed.
 *                        This is an array of complex numbers, arranged as real part and img part
 *                        interleave together, i.e xIn[0].real, xIn[0].img, xIn[1].real, xIn[1].img, ...
 *                        The size of this array is 2*N.
 *
 *  @param N      (input) IDFT size is N. The value N must be one of the value in following list.
 *                        {12, 24, 36, 48, 60, 72, 96, 108, 120, 144, 180, 192, 216, 240, 288,
 *                        300, 324, 360, 384, 432, 480, 540, 576, 600, 648, 720, 768, 864,
 *                        900, 960, 972, 1080, 1152, 1200, 1296}
 *
 *  @param yOut  (output) pointer to the output array.
 *                        This is an array of complex numbers, arranged as real part and img part
 *                        interleave together, i.e yOut[0].real, yOut[0].img, yOut[1].real, yOut[1].img, ...
 *                        The size of this array is 2*N.
 *
 *
 *  @param Exp2  (output) Return the the exponent of the output array. All the elements of the output
 *                        array have a scaling factor of 2^Exp2
 *
 *  @return None
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------
 *  |Name   |TCB Type       |Size         |Data Format    |Valid Range   |Default        |Units        |
 *  ----------------------------------------------------------------------------------------------------
 *  |xIn    |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  |N      |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  |yOut   |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  |Exp2   |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|DOXYGEN_TO_DO  |DOXYGEN_TO_DO |DOXYGEN_TO_DO  |DOXYGEN_TO_DO|
 *  ----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteIDFT_16t32d              |
 *  ---------------------------------------------------
 *  |Input Streams       |DOXYGEN_TO_DO               |
 *  ---------------------------------------------------
 *  |Output Streams      |DOXYGEN_TO_DO               |
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
 * This function performs IDFT (Inverse Discrete Fourier Transform) on a complex valued array of N points.
 * The value N is in the form of N = [2^(p) * 3^(q) * 5^(r)], where p, q, and r are positive integers.
 * The values of N supported in this function are listed below.
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
void LteIDFT_16t32d(short *xIn, unsigned short N, short *yOut, short *Exp2)
{
    int i;

    for (i = 0; i < N; i++)
    {
        xIn_cjg16[2 * i] = xIn[2 * i];
        xIn_cjg16[2 * i + 1] = -xIn[2 * i + 1];
    }

    LteDFT_16t32d(xIn_cjg16, N, yOut, Exp2);

    for (i = 0; i < N; i++)
    {
        yOut[2 * i + 1] = -yOut[2 * i + 1];
    }
    return;
}

