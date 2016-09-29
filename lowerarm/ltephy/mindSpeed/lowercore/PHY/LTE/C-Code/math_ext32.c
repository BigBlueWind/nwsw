//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL math_ext32.c
 *
 * @brief Contains fundamental math functions dealing with 32bit numbers
 * @author Mindspeed Technologies
 * @version $Revision: 1.17 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

/*_________________________________________________________________________
 |                                                                         |
 |                                  Include                                |
 |_________________________________________________________________________|
 */
//#include "mmodesim.h"
#include <math.h>

#include "typedef.h"
#include "basic_op_cc.h"
//#include "main_fx.h"

#ifdef WMOPS_FX
#include "const_fx.h"
#endif

#ifdef SUPERSIM
#include "asm_ctl.h"
#endif

/*_________________________________________________________________________
 |                                                                         |
 |                                  Define                                 |
 |_________________________________________________________________________|
 */

#define SW_MIN (short)0x8000           /* smallest Ram */
#define SW_MAXV (short)0x7fff           /* largest Ram */

#define TOL 4

#if SWITCH_MPY  // allows switching between old and new mpy fns
S16 use_old_mpys = 0;
#endif

//#define C40_MATH_BASICOP 1

/*_________________________________________________________________________
 |                                                                         |
 |                                Functions                                |
 |_________________________________________________________________________|
 */

/****************************************************************************
 *
 *     FUNCTION NAME: L_mpy_ll
 *
 *     PURPOSE:    Multiply a 32 bit number (L_var1) and a 32 bit number
 *                 (L_var2), and return a 32 bit result.
 *
 *     INPUTS:
 *
 *       L_var1             A S32 input variable
 *
 *       L_var2             A S32 input variable
 *
 *     OUTPUTS:             none
 *
 *     IMPLEMENTATION:
 *
 *        Performs a 31x31 bit multiply, Complexity=24 Ops.
 *
 *        Let x1x0, or y1y0, be the two constituent halves
 *        of a 32 bit number.  This function performs the
 *        following:
 *
 *        low = ((x0 >> 1)*(y0 >> 1)) >> 16     (low * low)
 *        mid1 = [(x1 * (y0 >> 1)) >> 1 ]       (high * low)
 *        mid2 = [(y1 * (x0 >> 1)) >> 1]        (high * low)
 *        mid =  (mid1 + low + mid2) >> 14      (sum so far)
 *        output = (y1*x1) + mid                (high * high)
 *
 *
 *     RETURN VALUE:        A S32 value
 *
 *     KEYWORDS: mult,mpy,multiplication
 *
 ***************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   L_var1 DOXYGEN_TO_DO
 *  @param   L_var2 DOXYGEN_TO_DO
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
 *  |L_var1        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |L_var2        |              |              |              |              |              |              |
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
S32 L_mpy_ll(S32 L_var1, S32 L_var2)
{
    S16 swLow1, swLow2, swHigh1, swHigh2;
    S32 L_varOut, L_mid1, L_mid2;
#ifndef C40_MATH_BASICOP
    S32 L_mid, L_low;
#endif
#ifdef C40_MATH_BASICOP
    S16 mid1, mid2;
#endif

#if MPY_LL_NONDPF_ASM
    S32 nonDPF_out;
#endif

#ifdef WMOPS_FX
    counter_fx.L_mpy_ll++;

    counter_fx.shr -= 2;
    counter_fx.extract_l -= 2;
    counter_fx.extract_h -= 2;
    counter_fx.L_add -= 2;
#ifdef C40_MATH_BASICOP
    counter_fx.L_mult -= 2;
    counter_fx.mult -= 2;
    counter_fx.L_shl -= 1;
#else
    counter_fx.L_mult -= 3;
    counter_fx.L_shr -= 4;
    counter_fx.L_mac--;
#endif
#endif

    swLow1 = shr(extract_l(L_var1), 1);
    swLow1 = SW_MAXV & swLow1;

    swLow2 = shr(extract_l(L_var2), 1);
    swLow2 = SW_MAXV & swLow2;
    swHigh1 = extract_h(L_var1);
    swHigh2 = extract_h(L_var2);

#ifdef C40_MATH_BASICOP
    // verbatim translation of asm macro M_L_mpy_ll with "prefix" blank:
    L_varOut = L_mult(swHigh1, swHigh2);
    mid1 = mult(swHigh1, swLow2);
    L_mid1 = L_mult(mid1, 1);
    L_varOut = L_add(L_varOut, L_mid1);
    mid2 = mult(swHigh2, swLow1);
    L_mid2 = L_shl(mid2, 1);
    L_varOut = L_add(L_varOut, L_mid2);
#else
    L_low = L_mult(swLow1, swLow2);
    L_low = L_shr(L_low, 16);
    L_mid1 = L_mult(swHigh1, swLow2);
    L_mid1 = L_shr(L_mid1, 1);
    L_mid = L_add(L_mid1, L_low);

    L_mid2 = L_mult(swHigh2, swLow1);
    L_mid2 = L_shr(L_mid2, 1);
    L_mid = L_add(L_mid, L_mid2);

    L_mid = L_shr(L_mid, 14);
    L_varOut = L_mac(L_mid, swHigh1, swHigh2);
#endif

#if MPY_LL_NONDPF_ASM
    // just for extensive test of nonDPF version
    //nonDPF_out = L_mpy_ll_nonDPF(L_var1, L_var2);
    nonDPF_out = L_mpy_ll_nonDPF_fx2a(L_var1, L_var2);
    if(abs(nonDPF_out - L_varOut) > TOL)
    printf("mpy_ll nonDPF = %08x, DPF = %08x, diff = %d\n",
        nonDPF_out, L_varOut, nonDPF_out-L_varOut);
#endif

    return (L_varOut);
}

/*----------------------------------------------------------------------------*/

/****************************************************************************
 *
 *     FUNCTION NAME: L_mpy_ls
 *
 *     PURPOSE:    Multiply a 32 bit number (L_var2) and a 16 bit
 *                 number (var1) returning a 32 bit result. L_var2
 *                 is truncated to 31 bits prior to executing the
 *                 multiply.
 *
 *     INPUTS:
 *
 *       L_var2             A S32 input variable
 *
 *       var1               A S16 input variable
 *
 *     OUTPUTS:             none
 *
 *     RETURN VALUE:        A S32 value
 *
 *     KEYWORDS: mult,mpy,multiplication
 *
 ***************************************************************************/
//-------------------------------------------------------------------------------------------
/** @brief L_mpy_ls: Multiply 32bit signed number by 16bit signed number
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 32bit signed number
 *  @param var2 - INPUT, 16bit signed number
 *
 *  @return (var1*var2)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var1         |         |Input         |1        |Real          |(1:0:31)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var2         |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |return       |         |Output        |1        |Real          |(1:0:31)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |L_mpy_ls                             |
 *  ------------------------------------------------------------
 *  |Input Streams       |var1,var2                            |
 *  ------------------------------------------------------------
 *  |Output Streams      |return value                         |
 *  ------------------------------------------------------------
 *  |Config Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  |Status Parameters   |none                                 |
 *  ------------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Multiply a 32 bit number (L_var2) and a 16 bit<BR>
 *  number (var1) returning a 32 bit result. L_var2<BR>
 *  is truncated to 31 bits prior to executing the<BR>
 *  multiply.<BR>
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S32 L_mpy_ls(S32 L_var2, S16 var1)
{
    S32 L_varOut;
    S16 swtemp;

#if MPY_LS_NONDPF_ASM
    S32 nonDPF_out;
#endif

#ifdef WMOPS_FX
    counter_fx.L_mpy_ls++;

    counter_fx.shr--;
    counter_fx.extract_l--;
    counter_fx.L_mult--;
    counter_fx.L_shr--;
    counter_fx.L_mac--;
#endif

    swtemp = shr(extract_l(L_var2), 1);
    swtemp = (short) 32767 & (short) swtemp;

    L_varOut = L_mult(var1, swtemp);
    L_varOut = L_shr(L_varOut, 15);

    if ((var1 == -32768) && (extract_h(L_var2) == -32768))
    {
        L_varOut++;
        L_varOut--;
    }

#ifdef C40_MATH_BASICOP
    L_varOut &= 0xfffffffe; // to match asm version of L_mpy_ls
#endif

    L_varOut = L_mac(L_varOut, var1, extract_h(L_var2));

#if MPY_LS_NONDPF_ASM
    // just for extensive test of nonDPF version
    //nonDPF_out = L_mpy_ls_nonDPF(L_var2, var1);
    nonDPF_out = L_mpy_ls_nonDPF_fx2a(L_var2, var1);
    if(abs(nonDPF_out - L_varOut) > TOL)
    printf("mpy_ls nonDPF = %08x, DPF = %08x, diff = %d\n",
        nonDPF_out, L_varOut, nonDPF_out-L_varOut);
#endif

    return (L_varOut);

}

/****************************************************************************
 *
 *     FUNCTION NAME: L_mpy_ll_nonDPF
 *
 *     PURPOSE:    Multiply a 32 bit number (L_var1) and a 32 bit number
 *                 (L_var2), and return a 32 bit result.
 *
 *     INPUTS:
 *
 *       L_var1             A S32 input variable
 *
 *       L_var2             A S32 input variable
 *
 *     OUTPUTS:             none
 *
 *     IMPLEMENTATION:
 *
 *        Performs a 32x32 bit multiply, Complexity=24 Ops.
 *
 *        Let x1x0, or y1y0, be the two constituent halves
 *        of a 32 bit number.
 *
 *     RETURN VALUE:        A S32 value
 *
 *     KEYWORDS: mult,mpy,multiplication
 *
 ***************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   L_var1 DOXYGEN_TO_DO
 *  @param   L_var2 DOXYGEN_TO_DO
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
 *  |L_var1        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |L_var2        |              |              |              |              |              |              |
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
S32 L_mpy_ll_nonDPF(S32 L_var1, S32 L_var2)
{

    U16 uwLow1, uwLow2;
    S16 swHigh1, swHigh2;
    S40 a0, a2, atmp;

#if SWITCH_MPY  // allows switching between old and new mpy fns
    if(use_old_mpys)
    return(L_mpy_ll(L_var1, L_var2));
    else
    {
#endif

#ifdef WMOPS_FX
    //counter_fx.L_mpy_ll_nonDPF++;
    counter_fx.L_mpy_ll++;
#endif

    // mimics this:
    //	a0 = (US)(X)*b3 * (SU)*b4	|| a2 = (US)*b3 * (SU)(X)*b4;
    //	a1 =     (X)*b3 *  (X)*b4, noupdate;
    //	// 2 stalls
    //	a0 = a0H:a0C + a2H:a2C;
    //	a0 = a0C:a0L +L a1C:a1L;


    uwLow1 = (U16) (0x0000ffff & L_var1);
    uwLow2 = (U16) (0x0000ffff & L_var2);
    swHigh1 = extract_h(L_var1);
    swHigh2 = extract_h(L_var2);

    a0 = (S40) swHigh1 * (S40) uwLow2 * 2.; // us

    a2 = (S40) uwLow1 * (S40) swHigh2 * 2.; // us

    a0 = floor(a0 / 65536.) + floor(a2 / 65536.);

    //a2 += ( (S40)swHigh1 * (Word64)swHigh2 ) * 65536 * 2;
    // only need this test for signed*signed
    if ((swHigh1 == (S16) 0x8000) && (swHigh2 == (S16) 0x8000))
        atmp = (S40) 0x7fffffff;
    else
        atmp = (S40) swHigh1 * (S40) swHigh2 * 2.;

    a0 = a0 + atmp;

    return ((S32) a0);

#if SWITCH_MPY
}
#endif

}

/****************************************************************************
 *
 *     FUNCTION NAME: L_mpy_ls_nonDPF
 *
 *     PURPOSE:    Multiply a 32 bit number (L_var2) and a 16 bit
 *                 number (var1) returning a 32 bit result
 *
 *     INPUTS:
 *
 *       L_var2             A S32 input variable
 *
 *       var1               A S16 input variable
 *
 *     OUTPUTS:             none
 *
 *     RETURN VALUE:        A S32 value
 *
 *     KEYWORDS: mult,mpy,multiplication
 *
 ***************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   L_var2 DOXYGEN_TO_DO
 *  @param   var1 DOXYGEN_TO_DO
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
 *  |L_var2        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |var1          |              |              |              |              |              |              |
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
S32 L_mpy_ls_nonDPF(S32 L_var2, S16 var1)
{

    U16 uwLow2;
    S16 swHigh2;
    S40 a0, a2;

#if SWITCH_MPY  // allows switching between old and new mpy fns
    if(use_old_mpys)
    return(L_mpy_ls(L_var2, var1));
    else
    {
#endif

#ifdef WMOPS_FX
    //counter_fx.L_mpy_ls_nonDPF++;
    counter_fx.L_mpy_ls++;
#endif

    //  sample code from Marty Budrovic that this is meant to mimic:
    //  b0 (short) -> input 1, hrH:hrL is input 2
    //  output in a0C:a0L
    //
    //      a0 = (SU)hr * (SS)*b0++;  // b0=16b, hr=hrH:hrL=32b
    //      a0 = a0H:a0C +L a2C:a2L;

    uwLow2 = (U16) (0x0000ffff & L_var2);
    swHigh2 = extract_h(L_var2);

    a0 = (S40) uwLow2 * (S40) var1 * 2; // us

    // only need this test for signed*signed
    if ((var1 == (S16) 0x8000) && (swHigh2 == (S16) 0x8000))
        a2 = (S40) 0x7fffffff;
    else
        a2 = (S40) swHigh2 * (S40) var1 * 2; // ss

    a0 = a0 + (a2 * 65536.);

    return ((S32) floor(a0 / 65536.));

#if SWITCH_MPY
}
#endif

}

