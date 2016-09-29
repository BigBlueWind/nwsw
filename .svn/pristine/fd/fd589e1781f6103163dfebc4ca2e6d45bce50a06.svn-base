//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL prach_ceva_fft.c
 *
 * @brief LTE PHY Global Constants
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

//
// FFT
//
// void cfft16b4096( short *xtime_real_img, int m, short *xfreq_real_img, short *Exponent2 );
//
//    xtime_real_img: (input)  pointer to the array in time domain
//                             This is an array of complex numbers, arranged as real part and img part
//                             interleave together, i.e x[0].real, x[0].img, x[1].real, x[1].img, ...
//                             The size of this array should be 2*2^m.
//
//    m             : (input)  FFT size is 2^m. Maximum value for m is 12.
//
//    xfreq_real_img: (output) pointer to array in frequency domain
//                             This is an array of complex numbers, arranged as real part and img part
//                             interleave together, i.e x[0].real, x[0].img, x[1].real, x[1].img, ...
//                             The size of this array should be 2*2^m.
//
//    Exponent2     : (output) pointer to a short variable which is the exponent of the output
//                             array, i.e. output array should be scaled by 2^(*Exponent2)
//
//
//
// IFFT
//
// void cifft16b4096( short *xfreq_real_img, int m, short *xtime_real_img, short *Exponent2 );
//
//    xfreq_real_img: (input)  pointer to the array in frequency domain
//                             This is an array of complex numbers, arranged as real part and img part
//                             interleave together, i.e x[0].real, x[0].img, x[1].real, x[1].img, ...
//                             The size of this array should be 2*2^m.
//
//    m             : (input)  IFFT size is 2^m. Maximum value for m is 12.
//
//    xtime_real_img: (output) pointer to array in time domain
//                             This is an array of complex numbers, arranged as real part and img part
//                             interleave together, i.e x[0].real, x[0].img, x[1].real, x[1].img, ...
//                             The size of this array should be 2*2^m.
//
//    Exponent2     : (output) pointer to a short variable which is the exponent of the output
//                             array, i.e. output array should be scaled by 2^(*Exponent2)
//
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "typedef.h"
#ifdef CEVA_INTRINSICS
#include    <basic_op.h>
#else
#include    "basic_op_cc.h"
#endif

//#define SHIFT_AT_EVERY_STAGE       0  // 1: right shift 1 bit at every stage regardless of the data range
#define N4ShiftAll         7771      // Right shift 1 bit at every stage if 2^m <= m4ShiftAll
//---------------------------------------------------

#define NBIT_IN       16
#define NBIT_INTERNAL 16
#define NBIT_OUT      16
#define NBIT_TBL      16
#define NBIT_I        0

//---------------------------------------------------

// exp{ -j * 2*pi/4096 * k }
// Even column -- real parts
// Odd column  -- img parts
//
extern short Table_FFT16b_4096[];

//--------------------------------------------------------------

int LeftShiftMult = 0; // 0: integer mult;  1: fractional mult
int IFFTswitch = 0; // Non-zero value for IFFT

//--------------------------------------------------------------


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a  DOXYGEN_TO_DO
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
 *  |a             |              |              |              |              |              |              |
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
short Neg16b(short a)
{
    short rv;

    if (a == ((short) 0x8000))
    {
        rv = 0x7FFF;
    }
    else
    {
        rv = -a;
    }
    return (rv);
}

//--------------------------------------------------------------


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
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
long Neg32b(long a)
{
    long rv;

    if (a == 0x80000000)
    {
        rv = 0x7FFFFFFF;
    }
    else
    {
        rv = -a;
    }
    return (rv);
}

//--------------------------------------------------------------


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
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
short Abs16b(short a)
{
    short rv;

    rv = a;
    if (a < 0)
    {
        rv = Neg16b(a);
    }
    return (rv);
}

//--------------------------------------------------------------


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
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
long Abs32b(long a)
{
    long rv;

    rv = a;
    if (a < 0)
    {
        rv = Neg32b(a);
    }
    return (rv);
}

//---------------------------------------------------
//
// Bit Reversal
//  a: input address
//  nb : number of bits


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a DOXYGEN_TO_DO
 *  @param   nb  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |nb            |              |              |              |              |              |              |
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
long bitrev(long a, long nb)
{
    long rv, inmask, outmask, i;

    rv = 0;
    inmask = 1;
    outmask = 1 << (nb - 1);
    for (i = 0; i < nb; i++)
    {
        if ((a & inmask) != 0)
        {
            rv |= outmask;
        }
        inmask <<= 1;
        outmask >>= 1;
    }

    return (rv);
}

//---------------------------------------------------
//
// Fmul16x16
//  Fractional multiplication (16x16)
//


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a DOXYGEN_TO_DO
 *  @param   b  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |b             |              |              |              |              |              |              |
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
long Fmul16x16(short a, short b)
{
    long rv;

    if ((a == (short) 0x8000) && (b == (short) 0x8000))
    {
        rv = 0x7fffffff;
    }
    else
    {
        rv = a * b;
        rv <<= 1;
    }

    return (rv);
}

//---------------------------------------------------
//
// Imul16x16
//  Integer multiplication (16x16)
//


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a DOXYGEN_TO_DO
 *  @param   b  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |b             |              |              |              |              |              |              |
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
long Imul16x16(short a, short b)
{
    long rv;

    rv = a * b;

    return (rv);
}

//---------------------------------------------------
//
// Add16bL
//  Add two 16-bit numbers, with limiting
//


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a DOXYGEN_TO_DO
 *  @param   b  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |b             |              |              |              |              |              |              |
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
short Add16bL(short a, short b)
{
    short rv, sa, sb, sc;
    sa = a & ((short) 0x8000);
    sb = b & ((short) 0x8000);
    rv = a + b;
    sc = rv & ((short) 0x8000);
    if (sa == sb)
    {
        if (sa != sc)
        {
            if (sa == 0)
            {
                rv = 0x7fff;
            }
            else
            {
                rv = (short) 0x8000;
            }
        }
    }
    return (rv);
}

//---------------------------------------------------
//
// Add32bL
//  Add two 32-bit numbers, with limiting
//


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a DOXYGEN_TO_DO
 *  @param   b  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |b             |              |              |              |              |              |              |
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
long Add32bL(long a, long b)
{
    long rv, sa, sb, sc;
    sa = a & ((long) 0x80000000);
    sb = b & ((long) 0x80000000);
    rv = a + b;
    sc = rv & ((long) 0x80000000);
    if (sa == sb)
    {
        if (sa != sc)
        {
            if (sa == 0)
            {
                rv = 0x7fffffff;
            }
            else
            {
                rv = 0x80000000;
            }
        }
    }
    return (rv);
}

//---------------------------------------------------
//
// Sub16bL
//  Compute difference of two 16-bit numbers (a-b), with limiting
//


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a DOXYGEN_TO_DO
 *  @param   b  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |b             |              |              |              |              |              |              |
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
short Sub16bL(short a, short b)
{
    short rv, nb;

    nb = Neg16b(b);
    rv = Add16bL(a, nb);

    return (rv);
}

//---------------------------------------------------
//
// Sub32bL
//  Compute difference of two 32-bit numbers (a-b), with limiting
//


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a DOXYGEN_TO_DO
 *  @param   b  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |b             |              |              |              |              |              |              |
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
long Sub32bL(long a, long b)
{
    long rv, nb;

    nb = Neg32b(b);
    rv = Add32bL(a, nb);

    return (rv);
}

//--------------------------------------------------------------
// Round 32-bit number to 16-bit number


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
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
short R32to16(long a)
{
    long rd, b;
    short rv;

    rd = 0x00008000;
    b = Add32bL(a, rd);
    rv = (short) (b >> 16);
    return (rv);
}

//--------------------------------------------------------------
// Truncate 32-bit number to 16-bit number


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
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
short T32to16(long a)
{
    short rv;

    rv = (short) (a >> 16);
    return (rv);
}

//--------------------------------------------------------------


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
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
short Exp16b(short a)
{
    short b;
    short rv;

    rv = 0;
    if (a != 0)
    {
        if (a < 0)
        {
            a = Neg16b(a);
        }
        b = (short) 0x4000;
        while ((b & a) == 0)
        {
            rv += 1;
            b >>= 1;
        }
    }

    return (rv);
}

//--------------------------------------------------------------


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
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
short Exp32b(long a)
{
    long b;
    short rv;

    rv = 0;
    if (a != 0)
    {
        if (a < 0)
        {
            a = Neg32b(a);
        }
        b = (long) 0x40000000;
        while ((b & a) == 0)
        {
            rv += 1;
            b >>= 1;
        }
    }

    return (rv);
}

//---------------------------------------------------
//
// Mpy16x16
//  Compute product of two 16-bit numbers (a*b)
//


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a DOXYGEN_TO_DO
 *  @param   b  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |b             |              |              |              |              |              |              |
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
long Mpy16x16(short a, short b)
{
    long rv;

    rv = Imul16x16(a, b);
    rv <<= LeftShiftMult;

    return (rv);
}

//---------------------------------------------------
//
// Cmad
//  Compute (a*b + c*d)
//


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a DOXYGEN_TO_DO
 *  @param   b DOXYGEN_TO_DO
 *  @param   c DOXYGEN_TO_DO
 *  @param   d  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |b             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |c             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |d             |              |              |              |              |              |              |
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
long Cmad(short a, short b, short c, short d)
{
    long rv, ab, cd;

    ab = Mpy16x16(a, b);
    cd = Mpy16x16(c, d);
    rv = Add32bL(ab, cd);

    return (rv);
}

//---------------------------------------------------
//
// Cmac3
//  Compute [Acc +  a*b + c*d]
//


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   Acc DOXYGEN_TO_DO
 *  @param   a DOXYGEN_TO_DO
 *  @param   b DOXYGEN_TO_DO
 *  @param   c DOXYGEN_TO_DO
 *  @param   d  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Acc           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |b             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |c             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |d             |              |              |              |              |              |              |
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
long Cmac3(long Acc, short a, short b, short c, short d)
{
    long rv, ab, cd, ss;

    ab = Mpy16x16(a, b);
    cd = Mpy16x16(c, d);
    ss = Add32bL(Acc, ab);
    rv = Add32bL(ss, cd);

    return (rv);
}

//---------------------------------------------------
//
// Cmas3
//  Compute [Acc +  a*b - c*d]
//


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   Acc DOXYGEN_TO_DO
 *  @param   a DOXYGEN_TO_DO
 *  @param   b DOXYGEN_TO_DO
 *  @param   c DOXYGEN_TO_DO
 *  @param   d  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Acc           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |b             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |c             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |d             |              |              |              |              |              |              |
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
long Cmas3(long Acc, short a, short b, short c, short d)
{
    long rv, ab, cd, ss;

    ab = Mpy16x16(a, b);
    cd = Mpy16x16(c, d);
    ss = Add32bL(Acc, ab);
    rv = Sub32bL(ss, cd);

    return (rv);
}

//---------------------------------------------------
//
// Cmsu3
//  Compute [Acc -  a*b - c*d]
//


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   Acc DOXYGEN_TO_DO
 *  @param   a DOXYGEN_TO_DO
 *  @param   b DOXYGEN_TO_DO
 *  @param   c DOXYGEN_TO_DO
 *  @param   d  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Acc           |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |b             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |c             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |d             |              |              |              |              |              |              |
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
long Cmsu3(long Acc, short a, short b, short c, short d)
{
    long rv, ab, cd, ss;

    ab = Mpy16x16(a, b);
    cd = Mpy16x16(c, d);
    ss = Sub32bL(Acc, ab);
    rv = Sub32bL(ss, cd);

    return (rv);
}

//---------------------------------------------------
//
// Fmul32x16
//  Fractional multiplication (32x16)
//


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   a DOXYGEN_TO_DO
 *  @param   b  DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |a             |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |b             |              |              |              |              |              |              |
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
long Fmul32x16(long a, short b)
{
    long rv, rh, rl;
    long ah, al;
    short ah16;
    unsigned short alu;

    ah = a >> 16;
    al = a & 0x0000FFFF;
    ah16 = (short) ah;

    if ((ah16 == (short) 0x8000) && (b == (short) 0x8000))
    {
        rh = 0x7fffffff;
    }
    else
    {
        //I rh = ah16 * b;
        //I rh <<= 1;
        rh = L_mult(ah16, b);
    }
    alu = (unsigned short) al;
    // rl = al * bl;
    rl = alu * b;
    rl >>= 15;

    //I rv = Add32bL( rh, rl );
    rv = L_add(rh, rl);

    return (rv);
}

//--------------------------------------------------------------
// case-2

//#define NBIT_IN_DTcase_2       16
//#define NBIT_INTERNAL_DTcase_2 32  // <============
//#define NBIT_OUT_DTcase_2      16
//#define NBIT_TBL_DTcase_2      16 // <=============
//
//#define NBIT_I_DTcase_2     12  // 4096

#define NBIT_I_16t32d     12  // 4096
long xxScratch[8192];
//extern long xScratch[ 8192 ];

#ifdef CEVA_INTRINSICS

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   * xtime_real_img __attribute__((aligned(8 DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  --------------------------------------------------------------------------------------------------------------------------------------
 *  |Name                                      |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  --------------------------------------------------------------------------------------------------------------------------------------
 *  |* xtime_real_img __attribute__((aligned(8 |              |              |              |              |              |              |
 *  --------------------------------------------------------------------------------------------------------------------------------------
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
void cfft_16t32d_case2( short * xtime_real_img __attribute__((aligned(8))), short m,
    short * xfreq_real_img __attribute__((aligned(8))), short *Exponent2 )
#else
void cfft_16t32d_case2(short *xtime_real_img, short m, short *xfreq_real_img, short *Exponent2)
#endif
{
    short i, j, N;
    short cos_real, sin_img;//, v4shift, shift_on;
    short Normalized, E;
    short t0_real, t0_img, t1_real, t1_img;
    long Lt0_real, Lt0_img, Lt1_real, Lt1_img;
    long a0, a1, a2, a3, a4, a5, a6, a7, a8, a9;
    int stage, nbfly, repeatbfly, /*w0start0, */w0start1, w0, w1, dw, dw0, dw1, ib, kb, brw0, brw1;
    int w0w0, w1w1;
    long *xScratch;

    xScratch = (long *) xxScratch;

    N = 1 << m;
    Normalized = 0;

    // stage = 0;
    repeatbfly = N >> 1;
    dw0 = 2;
    dw = 1;
    dw1 = 2;
    cos_real = 0x4000 >> (NBIT_I_16t32d - 1); // 0x0008;  // right shift input 12 bits

    w0 = 0;
    w1 = 1;
    for (j = 0; j < repeatbfly; j++)
    {
        // Do butterfly of X[w0] and X[w1]
        //
        brw0 = (short) bitrev(w0, m);

        brw1 = (short) bitrev(w1, m);

        w0w0 = brw0 << 1;
        w1w1 = brw1 << 1;
        t0_real = xtime_real_img[w0w0];
        t0_img = xtime_real_img[w0w0 + 1];
        t1_real = xtime_real_img[w1w1];
        t1_img = xtime_real_img[w1w1 + 1];

        //I a0 = Fmul16x16( t0_real, cos_real );
        a0 = L_mult(t0_real, cos_real);

        //I a1 = Fmul16x16( t0_img, cos_real );
        a1 = L_mult(t0_img, cos_real);

        //I a2 = Fmul16x16( t1_real, cos_real );
        a2 = L_mult(t1_real, cos_real);

        //I a3 = Fmul16x16( t1_img, cos_real );
        a3 = L_mult(t1_img, cos_real);

        w0w0 = w0 << 1;
        w1w1 = w1 << 1;

        //I xScratch[ w0w0 ] = Add32bL( a0, a2 );
        xScratch[w0w0] = L_add(a0, a2);

        //I xScratch[ w0w0+1 ] = Add32bL( a1, a3 );
        xScratch[w0w0 + 1] = L_add(a1, a3);

        //I xScratch[ w1w1 ] = Sub32bL( a0, a2 );
        xScratch[w1w1] = L_sub(a0, a2);

        //I xScratch[ w1w1+1 ] = Sub32bL( a1, a3 );
        xScratch[w1w1 + 1] = L_sub(a1, a3);

        w0 += dw1;
        w1 += dw1;
    }

    // Start2ndStage:

    nbfly = 2;
    repeatbfly = N >> 2;
    ib = N >> 2;
    dw = 2;
    dw0 = 4;
    dw1 = 4;
    //v4shift = (short) 0x7FFF;
    //shift_on = 1;

    for (stage = 1; stage < m; stage++)
    {
        //w0start0 = 0;
        w0start1 = 0;
        cos_real = (short) 0x7FFF; //1.0;
        sin_img = (short) 0x0000; //0.0;
        kb = 0; //b = 0.0;
        for (i = 0; i < nbfly; i++)
        {
            w0 = w0start1;
            w1 = w0 + dw;
            for (j = 0; j < repeatbfly; j++)
            {
                // Do butterfly of X[w0] and X[w1]
                //
                w0w0 = w0 << 1;
                w1w1 = w1 << 1;
                Lt0_real = xScratch[w0w0];
                Lt0_img = xScratch[w0w0 + 1];
                Lt1_real = xScratch[w1w1];
                Lt1_img = xScratch[w1w1 + 1];

                a0 = Fmul32x16(Lt1_real, cos_real);
                a1 = Fmul32x16(Lt1_img, sin_img);

                a2 = Fmul32x16(Lt1_real, sin_img);
                a3 = Fmul32x16(Lt1_img, cos_real);

                //I a4 = Sub32bL( a0, a1 );
                //I a5 = Add32bL( a2, a3 );
                a4 = L_sub(a0, a1);
                a5 = L_add(a2, a3);

                //I a6 = Sub32bL( Lt0_real, a4 );
                //I a7 = Sub32bL( Lt0_img, a5 );
                a6 = L_sub(Lt0_real, a4);
                a7 = L_sub(Lt0_img, a5);

                //I a8 = Add32bL( Lt0_real, a4 );
                //I a9 = Add32bL( Lt0_img, a5 );
                a8 = L_add(Lt0_real, a4);
                a9 = L_add(Lt0_img, a5);

                xScratch[w0w0] = a8;
                xScratch[w0w0 + 1] = a9;
                xScratch[w1w1] = a6;
                xScratch[w1w1 + 1] = a7;

                w0 += dw1;
                w1 += dw1;
            }
            w0start1 += 1; //dw;
            // update sin, cos
            kb += (ib * 4096 / N); //b += ib;
            w0w0 = kb << 1;
            //I cos_real = Table_FFT16b_4096[ 2*kb ];
            //I sin_img  = Table_FFT16b_4096[ 2*kb+1 ];
            cos_real = Table_FFT16b_4096[w0w0];
            sin_img = Table_FFT16b_4096[w0w0 + 1];
        }
        nbfly <<= 1; // nbfly *= 2;
        dw <<= 1; // dw *= 2;
        repeatbfly >>= 1;
        dw0 <<= 1; // dw0 *= 2;
        dw1 <<= 1; // dw1 *= 2;
        ib >>= 1;
    }

    a0 = 0;
    for (i = 0; i < 2 * N; i++)
    {
        a1 = Abs32b(xScratch[i]);
        if (a0 < a1)
        {
            a0 = a1;
        }
    }
    E = Exp32b(a0);
    Normalized = NBIT_I_16t32d - E;

    for (i = 0; i < 2 * N; i++)
    {
        a2 = xScratch[i] << E;
        // a2 += 0x00008000;  // rounding
        xfreq_real_img[i] = (short) (a2 >> 16);
    }

    *Exponent2 = Normalized;

    return;
}

//--------------------------------------------------------------

/*
 void cifft_16t32d_case2( short *xfreq_real_img, int m, short *xtime_real_img, short *Exponent2 )
 {
 int i, n;

 n = 1 << m;
 for ( i=0; i < n; i++ )
 {
 xtime_real_img[ 2*i ] = xfreq_real_img[ 2*i ];
 xtime_real_img[ 2*i+1 ] = Neg16b( xfreq_real_img[ 2*i+1 ] );
 }
 IFFTswitch = 0;
 cfft_16t32d_case2( xtime_real_img, m, xtime_real_img, Exponent2 );
 for ( i=0; i < n; i++ )
 {
 xtime_real_img[ 2*i+1 ] = Neg16b( xtime_real_img[ 2*i+1 ] );
 }
 *Exponent2 -= m;

 return;
 }
 */

//--------------------------------------------------------------


#ifdef CEVA_INTRINSICS

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   * xtime_real_img __attribute__((aligned(8 DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  --------------------------------------------------------------------------------------------------------------------------------------
 *  |Name                                      |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  --------------------------------------------------------------------------------------------------------------------------------------
 *  |* xtime_real_img __attribute__((aligned(8 |              |              |              |              |              |              |
 *  --------------------------------------------------------------------------------------------------------------------------------------
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
void cifft_16t32d_case2_2( short * xtime_real_img __attribute__((aligned(8))), short m,
    short * xfreq_real_img __attribute__((aligned(8))), short *Exponent2 )
#else
void cifft_16t32d_case2_2(short *xtime_real_img, short m, short *xfreq_real_img, short *Exponent2)
#endif
{
    short i, j, N;
    short cos_real, sin_img;//, v4shift, shift_on;
    short Normalized, E;
    short t0_real, t0_img, t1_real, t1_img;
    long Lt0_real, Lt0_img, Lt1_real, Lt1_img;
    long a0, a1, a2, a3, a4, a5, a6, a7, a8, a9;
    int stage, nbfly, repeatbfly, /*w0start0, */w0start1, w0, w1, dw, dw0, dw1, ib, kb, brw0, brw1;
    int w0w0, w1w1;
    long *xScratch;

    xScratch = (long *) xxScratch;

    N = 1 << m;
    Normalized = 0;

    // stage = 0;
    repeatbfly = N >> 1;
    dw0 = 2;
    dw = 1;
    dw1 = 2;
    cos_real = 0x4000 >> (NBIT_I_16t32d - 1); // 0x0008;  // right shift input 12 bits
    sin_img = negate(cos_real);

    // For IFFT, apply conjugate to the inputs
    //
    w0 = 0;
    w1 = 1;
    for (j = 0; j < repeatbfly; j++)
    {
        // Do butterfly of X[w0] and X[w1]
        //
        brw0 = (short) bitrev(w0, m);

        brw1 = (short) bitrev(w1, m);

        w0w0 = brw0 << 1;
        w1w1 = brw1 << 1;
        t0_real = xtime_real_img[w0w0];
        t0_img = xtime_real_img[w0w0 + 1];
        t1_real = xtime_real_img[w1w1];
        t1_img = xtime_real_img[w1w1 + 1];

        //I a0 = Fmul16x16( t0_real, cos_real );
        a0 = L_mult(t0_real, cos_real);

        //I a1 = Fmul16x16( t0_img, cos_real );
        //IFFT a1 = L_mult( t0_img, cos_real );
        a1 = L_mult(t0_img, sin_img);

        //I a2 = Fmul16x16( t1_real, cos_real );
        a2 = L_mult(t1_real, cos_real);

        //I a3 = Fmul16x16( t1_img, cos_real );
        //IFFT a3 = L_mult( t1_img, cos_real );
        a3 = L_mult(t1_img, sin_img);

        w0w0 = w0 << 1;
        w1w1 = w1 << 1;

        //I xScratch[ w0w0 ] = Add32bL( a0, a2 );
        xScratch[w0w0] = L_add(a0, a2);

        //I xScratch[ w0w0+1 ] = Add32bL( a1, a3 );
        xScratch[w0w0 + 1] = L_add(a1, a3);

        //I xScratch[ w1w1 ] = Sub32bL( a0, a2 );
        xScratch[w1w1] = L_sub(a0, a2);

        //I xScratch[ w1w1+1 ] = Sub32bL( a1, a3 );
        xScratch[w1w1 + 1] = L_sub(a1, a3);

        w0 += dw1;
        w1 += dw1;
    }

    // Start2ndStage:

    nbfly = 2;
    repeatbfly = N >> 2;
    ib = N >> 2;
    dw = 2;
    dw0 = 4;
    dw1 = 4;
    //v4shift = (short) 0x7FFF;
    //shift_on = 1;

    for (stage = 1; stage < m; stage++)
    {
        //w0start0 = 0;
        w0start1 = 0;
        cos_real = (short) 0x7FFF; //1.0;
        sin_img = (short) 0x0000; //0.0;
        kb = 0; //b = 0.0;
        for (i = 0; i < nbfly; i++)
        {
            w0 = w0start1;
            w1 = w0 + dw;
            for (j = 0; j < repeatbfly; j++)
            {
                // Do butterfly of X[w0] and X[w1]
                //
                w0w0 = w0 << 1;
                w1w1 = w1 << 1;
                Lt0_real = xScratch[w0w0];
                Lt0_img = xScratch[w0w0 + 1];
                Lt1_real = xScratch[w1w1];
                Lt1_img = xScratch[w1w1 + 1];

                a0 = Fmul32x16(Lt1_real, cos_real);
                a1 = Fmul32x16(Lt1_img, sin_img);

                a2 = Fmul32x16(Lt1_real, sin_img);
                a3 = Fmul32x16(Lt1_img, cos_real);

                //I a4 = Sub32bL( a0, a1 );
                //I a5 = Add32bL( a2, a3 );
                a4 = L_sub(a0, a1);
                a5 = L_add(a2, a3);

                //I a6 = Sub32bL( Lt0_real, a4 );
                //I a7 = Sub32bL( Lt0_img, a5 );
                a6 = L_sub(Lt0_real, a4);
                a7 = L_sub(Lt0_img, a5);

                //I a8 = Add32bL( Lt0_real, a4 );
                //I a9 = Add32bL( Lt0_img, a5 );
                a8 = L_add(Lt0_real, a4);
                a9 = L_add(Lt0_img, a5);

                xScratch[w0w0] = a8;
                xScratch[w0w0 + 1] = a9;
                xScratch[w1w1] = a6;
                xScratch[w1w1 + 1] = a7;

                w0 += dw1;
                w1 += dw1;
            }
            w0start1 += 1; //dw;
            // update sin, cos
            kb += (ib * 4096 / N); //b += ib;
            w0w0 = kb << 1;
            //I cos_real = Table_FFT16b_4096[ 2*kb ];
            //I sin_img  = Table_FFT16b_4096[ 2*kb+1 ];
            cos_real = Table_FFT16b_4096[w0w0];
            sin_img = Table_FFT16b_4096[w0w0 + 1];
        }
        nbfly <<= 1; // nbfly *= 2;
        dw <<= 1; // dw *= 2;
        repeatbfly >>= 1;
        dw0 <<= 1; // dw0 *= 2;
        dw1 <<= 1; // dw1 *= 2;
        ib >>= 1;
    }

    a0 = 0;
    for (i = 0; i < 2 * N; i++)
    {
        a1 = Abs32b(xScratch[i]);
        if (a0 < a1)
        {
            a0 = a1;
        }
    }
    E = Exp32b(a0);
    Normalized = NBIT_I_16t32d - E;

    // For IFFT, apply conjugate to the output
    //
    for (i = 0; i < N; i++)
    {
        a2 = xScratch[2 * i] << E;
        // a2 += 0x00008000;  // rounding
        xfreq_real_img[2 * i] = (short) (a2 >> 16);

#if 1
        a2 = L_negate(xScratch[2 * i + 1]) << E;
        // a2 += 0x00008000;  // rounding
        xfreq_real_img[2 * i + 1] = (short) (a2 >> 16);
#else
        a2 = xScratch[ 2*i+1 ] << E;
        xfreq_real_img[ 2*i+1 ] = (short) (a2 >> 16);
        xfreq_real_img[ 2*i+1 ] = Neg16b( xfreq_real_img[ 2*i+1 ] );
#endif
    }

    *Exponent2 = Normalized - m; //IFFT
    // *Exponent2 -= m;

    return;
}

