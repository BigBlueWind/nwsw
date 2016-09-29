//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL basic_op.c
 *
 * @brief Basic ITU Math Operations
 * @author Mindspeed Technologies
 * @version $Revision: 1.26 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

/***************************************************************************
 *
 *   File Name:  basic_op.c (mathevrc.c with op counting added)
 *
 *   Purpose:  Contains functions which implement the primitive
 *     arithmetic operations.
 *
 *      The functions in this file are listed below.  Some of them are
 *      defined in terms of other basic operations.  One of the
 *      routines, saturate() is static.  This is not a basic
 *      operation, and is not referenced outside the scope of this
 *      file.
 *
 *
 *       abs_s()
 *       add()
 *       clearOverflow()
 *       divide_s()
 *       extract_h()
 *       extract_l()
 *       isOverflow()
 *       L_abs()
 *       L_add()
 *       L_add_c()
 *       L_deposit_h()
 *       L_deposit_l()
 *       L_mac()
 *       L_msu()
 *       L_mult()
 *       L_negate()
 *       L_shift_r()
 *       L_shl()
 *       L_shr()
 *       L_sub()
 *       L_sub_c()
 *       L_saturate()
 *       mac_r()
 *       msu_r()
 *       mult()
 *       mult_r()
 *       negate()
 *       norm_l()
 *       norm_s()
 *       popOverflow()
 *       round_s()
 *       saturate()
 *       setOverflow()
 *       shift_r()
 *       shl()
 *       shr()
 *       sub()
 *
 **************************************************************************/

/*_________________________________________________________________________
 |                                                                         |
 |                            Include Files                                |
 |_________________________________________________________________________|
 */
#include <stdio.h>
#include <stdlib.h>
#include "typedef.h"
#include "basic_op_cc.h"

#ifdef WMOPS_FX
#include "const_fx.h"
#include "ext_var_fx.h"
#include "lib_wmp_fx.h"
#endif

extern S16 saturate(S32 L_var1);

int giOverflow = 0;
int giOldOverflow = 0;

// local function:

/***************************************************************************/
/***************************************************************************/
/*------------------------------ Public Functions -------------------------*/
/***************************************************************************/
/***************************************************************************/

/***************************************************************************
 *
 *   FUNCTION NAME: L_saturate
 *
 *   PURPOSE:
 *
 *     Limit the input double (possibly exceeding 32 bit dynamic
 *     range) having to the 32 output wordsize.
 *
 *   INPUTS:
 *
 *     dVar1
 *                     A double whose range is
 *                     0x8 0000 0000 <= L_var1 <= 0x7 ffff ffff.
 *                     i.e. a 33 bit number. Not modified.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     L_Out
 *                     32 bit long integer (S32) where the DSP's
 *                     rules of saturation are followed:
 *                     for: 0x8000 0000 <= dVal1 <= 0x7fff ffff
 *                      input == output, no saturation
 *                     for: 0x8000 0000 > dVal1 output 0x8000 0000
 *                     for: dVal1 > 0x7fff ffff output 0x7fff ffff
 *
 *   KEYWORDS: saturation, limiting, limit, saturate, 32 bits
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   dVar1 DOXYGEN_TO_DO
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
 *  |dVar1         |              |              |              |              |              |              |
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
static S32 L_saturate(double dVar1)
{

    if (dVar1 > (double) LW_MAX)
    {
        dVar1 = (double) LW_MAX;
        giOverflow = 1;
    }
    else if (dVar1 < (double) LW_MIN)
    {
        dVar1 = (double) LW_MIN;
        giOverflow = 1;
    }

#ifdef WMOPS_FX
    counter_fx.L_saturate++;
#endif

    return ((long) dVar1);
}

/***************************************************************************
 *
 *   FUNCTION NAME: abs_s
 *
 *   PURPOSE:
 *
 *     Take the absolute value of the 16 bit input.  An input of
 *     -0x8000 results in a return value of 0x7fff.
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0x0000 0000 <= swOut <= 0x0000 7fff.
 *
 *   IMPLEMENTATION:
 *
 *     Take the absolute value of the 16 bit input.  An input of
 *     -0x8000 results in a return value of 0x7fff.
 *
 *   KEYWORDS: absolute value, abs
 *
 *************************************************************************/
//-------------------------------------------------------------------------------------------
/** @brief abs_s: Take the absolute value of the 16 bit signed input
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var - INPUT, 16bit signed number
 *
 *  @return (abs(var))
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var          |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |return       |         |Output        |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |abs_s                                |
 *  ------------------------------------------------------------
 *  |Input Streams       |var                                  |
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
 *  Take the absolute value of the 16 bit input.  An input of
 *  -0x8000 results in a return value of 0x7fff.
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 abs_s(S16 var1)
{
    S16 swOut;

    if (var1 == SW_MIN)
    {
        swOut = SW_MAXV;
        giOverflow = 1;
    }
    else
    {
        if (var1 < 0)
            swOut = -var1;
        else
            swOut = var1;
    }

#ifdef WMOPS_FX
    counter_fx.abs_s++;
#endif

    return (swOut);
}

/***************************************************************************
 *
 *   FUNCTION NAME: add
 *
 *   PURPOSE:
 *
 *     Perform the addition of the two 16 bit input variable with
 *     saturation.
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0xffff 8000 <= swOut <= 0x0000 7fff.
 *
 *   IMPLEMENTATION:
 *
 *     Perform the addition of the two 16 bit input variable with
 *     saturation.
 *
 *     swOut = var1 + var2
 *
 *     swOut is set to 0x7fff if the operation results in an
 *     overflow.  swOut is set to 0x8000 if the operation results
 *     in an underflow.
 *
 *   KEYWORDS: add, addition
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief add: Add two signed 16bit fixed point numbers with limit
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 16bit signed number
 *  @param var2 - INPUT, 16bit signed number
 *
 *  @return (var1+var2)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var1         |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var2         |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |return       |         |Output        |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |add                                  |
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
 *  Add two signed 16bit fixed point numbers with limit
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 add(S16 var1, S16 var2)
{
    S32 L_sum;
    S16 swOut;

    L_sum = (S32) var1 + var2;
    swOut = saturate(L_sum);

#ifdef WMOPS_FX
    counter_fx.add++;
#endif

    return (swOut);
}

/****************************************************************************
 *
 *     FUNCTION NAME: clearOverflow
 *
 *     PURPOSE:
 *
 *        Clear the overflow flag
 *
 *     INPUTS:
 *
 *       none
 *
 *
 *     OUTPUTS:             global overflow flag is cleared
 *                          previous value stored in giOldOverflow
 *
 *     RETURN VALUE:        previous value of overflow
 *
 *
 *     KEYWORDS: saturation, limit, overflow
 *
 ***************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   void DOXYGEN_TO_DO
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
 *  |void          |              |              |              |              |              |              |
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
int clearOverflow(void)
{
    giOldOverflow = giOverflow;
    giOverflow = 0;

#ifdef WMOPS_FX
    counter_fx.clearOverflow++;
#endif

    return (giOldOverflow);
}

/***************************************************************************
 *
 *   FUNCTION NAME: extract_h
 *
 *   PURPOSE:
 *
 *     Extract the 16 MS bits of a 32 bit S32.  Return the 16 bit
 *     number as a S16.  This is used as a "truncation" of a fractional
 *     number.
 *
 *   INPUTS:
 *
 *     L_var1
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0xffff 8000 <= swOut <= 0x0000 7fff.
 *
 *   IMPLEMENTATION:
 *
 *   KEYWORDS: assign, truncate
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   L_var1 DOXYGEN_TO_DO
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
S16 extract_h(S32 L_var1)
{
    S16 var2;

    var2 = (S16) (0x0000ffffL & (L_var1 >> 16));

#ifdef WMOPS_FX
    counter_fx.extract_h++;
#endif

    return (var2);
}

/***************************************************************************
 *
 *   FUNCTION NAME: extract_l
 *
 *   PURPOSE:
 *
 *     Extract the 16 LS bits of a 32 bit S32.  Return the 16 bit
 *     number as a S16.  The upper portion of the input S32
 *     has no impact whatsoever on the output.
 *
 *   INPUTS:
 *
 *     L_var1
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0xffff 8000 <= swOut <= 0x0000 7fff.
 *
 *
 *   KEYWORDS: extract, assign
 *
 *************************************************************************/
//-------------------------------------------------------------------------------------------
/** @brief extract_l:  Extract the 16 LS bits of a 32 bit S32.
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var - INPUT, 32bit signed number
 *
 *  @return (extract_l(var)), 16bit signed number
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var          |         |Input         |1        |Real          |(1:0:31)    |Full Range  |      |
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
 *  |Block Name          |extract_l                            |
 *  ------------------------------------------------------------
 *  |Input Streams       |var                                  |
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
 *  Extract the 16 LS bits of a 32 bit S32.  Return the 16 bit
 *  number as a S16.  The upper portion of the input S32
 *  has no impact whatsoever on the output.
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 extract_l(S32 L_var1)
{
    S16 var2;

    var2 = (S16) (0x0000ffffL & L_var1);

#ifdef WMOPS_FX
    counter_fx.extract_l++;
#endif

    return (var2);
}

/****************************************************************************
 *
 *     FUNCTION NAME: isOverflow
 *
 *     PURPOSE:
 *
 *        Check to see whether an overflow/saturation/limiting has occurred
 *
 *     INPUTS:
 *
 *       none
 *
 *
 *     OUTPUTS:             none
 *
 *     RETURN VALUE:        1 if overflow has been flagged
 *                          0 otherwise
 *
 *     KEYWORDS: saturation, limit, overflow
 *
 ***************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   void DOXYGEN_TO_DO
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
 *  |void          |              |              |              |              |              |              |
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
int isOverflow(void)
{

    return (giOverflow);
}

/***************************************************************************
 *
 *   FUNCTION NAME: L_abs
 *
 *   PURPOSE:
 *
 *     Take the absolute value of the 32 bit input.  An input of
 *     -0x8000 0000 results in a return value of 0x7fff ffff.
 *
 *   INPUTS:
 *
 *     L_var1
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     L_Out
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *
 *
 *   KEYWORDS: absolute value, abs
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief L_abs: Take the absolute value of the 32 bit signed input
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var - INPUT, 32bit signed number
 *
 *  @return (abs(var))
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var          |         |Input         |1        |Real          |(1:0:31)    |Full Range  |      |
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
 *  |Block Name          |L_abs                                |
 *  ------------------------------------------------------------
 *  |Input Streams       |var                                  |
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
 *  Take the absolute value of the 32 bit input.  An input of
 *   -0x8000 0000 results in a return value of 0x7fff ffff.
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S32 L_abs(S32 L_var1)
{
    S32 L_Out;

    if (L_var1 == LW_MIN)
    {
        L_Out = LW_MAX;
        giOverflow = 1;
    }
    else
    {
        if (L_var1 < 0)
            L_Out = -L_var1;
        else
            L_Out = L_var1;
    }

#ifdef WMOPS_FX
    counter_fx.L_abs++;
#endif

    return (L_Out);
}

/***************************************************************************
 *
 *   FUNCTION NAME: L_add
 *
 *   PURPOSE:
 *
 *     Perform the addition of the two 32 bit input variables with
 *     saturation.
 *
 *   INPUTS:
 *
 *     L_var1
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *     L_var2
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var2 <= 0x7fff ffff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     L_Out
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *   IMPLEMENTATION:
 *
 *     Perform the addition of the two 32 bit input variables with
 *     saturation.
 *
 *     L_Out = L_var1 + L_var2
 *
 *     L_Out is set to 0x7fff ffff if the operation results in an
 *     overflow.  L_Out is set to 0x8000 0000 if the operation
 *     results in an underflow.
 *
 *   KEYWORDS: add, addition
 *
 *************************************************************************/
//-------------------------------------------------------------------------------------------
/** @brief L_add: Add two signed 32bit fixed point numbers with limit
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 32bit signed number
 *  @param var2 - INPUT, 32bit signed number
 *
 *  @return (var1+var2)
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
 *  |var2         |         |Input         |1        |Real          |(1:0:31)    |Full Range  |      |
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
 *  |Block Name          |L_add                                |
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
 *  Add two signed 32bit fixed point numbers with limit
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S32 L_add(S32 L_var1, S32 L_var2)
{
    S32 L_Sum;
    double dSum;

    dSum = (double) L_var1 + (double) L_var2;
    L_Sum = L_var1 + L_var2;

    if (dSum != (double) L_Sum)
    {
        /* overflow occurred */
        L_Sum = L_saturate(dSum); // OP_COUNT(-4);
#ifdef WMOPS_FX
        counter_fx.L_saturate--;
#endif
    }

#ifdef WMOPS_FX
    counter_fx.L_add++;
#endif

    return (L_Sum);
}

/***************************************************************************
 *
 *   FUNCTION NAME: L_deposit_h
 *
 *   PURPOSE:
 *
 *     Put the 16 bit input into the 16 MSB's of the output S32.  The
 *     LS 16 bits are zeroed.
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     L_Out
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff 0000.
 *
 *
 *   KEYWORDS: deposit, assign, fractional assign
 *
 *************************************************************************/
//-------------------------------------------------------------------------------------------
/** @brief L_deposit_h: 16bit Arithmetic left of 16bit signed input
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 16bit signed number
 *
 *  @return (var1 << 16)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var1         |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |return       |         |Output        |1        |Real          |(1:0:31)    |Full Range    |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |L_deposit_h                          |
 *  ------------------------------------------------------------
 *  |Input Streams       |var1                                 |
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
 *  16bit Arithmetic left of 16bit signed input
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S32 L_deposit_h(S16 var1)
{
    S32 L_var2;

    L_var2 = (S32) var1 << 16;

#ifdef WMOPS_FX
    counter_fx.L_deposit_h++;
#endif

    return (L_var2);
}

/***************************************************************************
 *
 *   FUNCTION NAME: L_deposit_l
 *
 *   PURPOSE:
 *
 *     Put the 16 bit input into the 16 LSB's of the output S32 with
 *     sign extension i.e. the top 16 bits are set to either 0 or 0xffff.
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     L_Out
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0xffff 8000 <= L_var1 <= 0x0000 7fff.
 *
 *   KEYWORDS: deposit, assign
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
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
S32 L_deposit_l(S16 var1)
{
    S32 L_Out;

    L_Out = var1;

#ifdef WMOPS_FX
    counter_fx.L_deposit_l++;
#endif

    return (L_Out);
}

/***************************************************************************
 *
 *   FUNCTION NAME: L_mac
 *
 *   PURPOSE:
 *
 *     Multiply accumulate.  Fractionally multiply two 16 bit
 *     numbers together with saturation.  Add that result to the
 *     32 bit input with saturation.  Return the 32 bit result.
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *     L_var3
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var2 <= 0x7fff ffff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     L_Out
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *   IMPLEMENTATION:
 *
 *     Fractionally multiply two 16 bit numbers together with
 *     saturation.  The only numbers which will cause saturation on
 *     the multiply are 0x8000 * 0x8000.
 *
 *     Add that result to the 32 bit input with saturation.
 *     Return the 32 bit result.
 *
 *     Please note that this is not a true multiply accumulate as
 *     most processors would implement it.  The 0x8000*0x8000
 *     causes and overflow for this instruction.  On most
 *     processors this would cause an overflow only if the 32 bit
 *     input added to it were positive or zero.
 *
 *   KEYWORDS: mac, multiply accumulate
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief L_mac:  Multiply 2 signed 16 bit numbers and accumulate output in 32 bit signed number
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 32bit signed number
 *  @param var2 - INPUT, 16bit signed number
 *  @param var3 - INPUT, 16bit signed number
 *
 *  @return (var1 = var1 + var2*var3)
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
 *  |var3         |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
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
 *  |Block Name          |L_mac                                |
 *  ------------------------------------------------------------
 *  |Input Streams       |var1,var2,var3                      |
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
 *  Multiply accumulate.  Fractionally multiply two 16 bit
 *  numbers together with saturation.  Add that result to the
 *  32 bit input with saturation.  Return the 32 bit result.
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S32 L_mac(S32 L_var3, S16 var1, S16 var2)
{
    S32 L_product;

    L_product = (S32) var1 * var2; /* integer multiply */
    if (L_product == (S32) 0x40000000)
    {
        /* the event 0x8000 * 0x8000, the only possible saturation
         * in the multiply */
#ifdef C40_MATH_BASICOP
        // for Countach40, 0x8000*0x8000 = 0x7fffffff BEFORE accumulate
        L_product = L_saturate(2147483647.0 + (double) L_var3);
#else
        L_product = L_saturate(2147483648.0 + (double) L_var3);
#endif
#ifdef WMOPS_FX
        counter_fx.L_saturate--;
#endif
    }
    else
    {
        /* no overflow possible in mult */
        L_product = L_product << 1;
        L_product = L_add(L_var3, L_product); // OP_COUNT(-2);
#ifdef WMOPS_FX
        counter_fx.L_add--;
#endif
    }

#ifdef WMOPS_FX
    counter_fx.L_mac++;
#endif

    return (L_product);
}

/***************************************************************************
 *
 *   FUNCTION NAME: L_msu
 *
 *   PURPOSE:
 *
 *     Multiply and subtract.  Fractionally multiply two 16 bit
 *     numbers together with saturation.  Subtract that result from
 *     the 32 bit input with saturation.  Return the 32 bit result.
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *     L_var3
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var2 <= 0x7fff ffff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     L_Out
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *   IMPLEMENTATION:
 *
 *     Fractionally multiply two 16 bit numbers together with
 *     saturation.  The only numbers which will cause saturation on
 *     the multiply are 0x8000 * 0x8000.
 *
 *     Subtract that result from the 32 bit input with saturation.
 *     Return the 32 bit result.
 *
 *     Please note that this is not a true multiply accumulate as
 *     most processors would implement it.  The 0x8000*0x8000
 *     causes and overflow for this instruction.  On most
 *     processors this would cause an overflow only if the 32 bit
 *     input added to it were negative or zero.
 *
 *   KEYWORDS: mac, multiply accumulate, msu
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   L_var3 DOXYGEN_TO_DO
 *  @param   var1 DOXYGEN_TO_DO
 *  @param   var2 DOXYGEN_TO_DO
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
 *  |L_var3        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |var1          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |var2          |              |              |              |              |              |              |
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
S32 L_msu(S32 L_var3, S16 var1, S16 var2)
{
    S32 L_product;

    L_product = (S32) var1 * var2; /* integer multiply */
    if (L_product == (S32) 0x40000000)
    {
        /* the event 0x8000 * 0x8000, the only possible saturation
         * in the multiply */
#ifdef C40_MATH_BASICOP
        // for Countach40, 0x8000*0x8000 = 0x7fffffff BEFORE accumulate
        L_product = L_saturate((double) L_var3 - 2147483647.0);
#else
        L_product = L_saturate((double) L_var3 - 2147483648.0);
#endif
#ifdef WMOPS_FX
        counter_fx.L_saturate--;
#endif
    }
    else
    {
        /* no overflow possible in mult */
        L_product <<= 1;
        L_product = L_sub(L_var3, L_product);
#ifdef WMOPS_FX
        counter_fx.L_sub--;
#endif
    }

#ifdef WMOPS_FX
    counter_fx.L_msu++;
#endif

    return (L_product);
}

/***************************************************************************
 *
 *   FUNCTION NAME: L_mult
 *
 *   PURPOSE:
 *
 *     Perform a fractional multipy of the two 16 bit input numbers
 *     with saturation.  Output a 32 bit number.
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     L_Out
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *   IMPLEMENTATION:
 *
 *     Multiply the two the two 16 bit input numbers. If the
 *     result is within this range, left shift the result by one
 *     and output the 32 bit number.  The only possible overflow
 *     occurs when var1==var2==-0x8000.  In this case output
 *     0x7fff ffff.
 *
 *   KEYWORDS: multiply, mult, mpy
 *
 *************************************************************************/
//-------------------------------------------------------------------------------------------
/** @brief L_mult:  Multiply 2 signed 16 bit numbers and return output in 32 bit signed number
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 16bit signed number
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
 *  |var1         |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
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
 *  |Block Name          |L_mult                               |
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
 *  Perform a fractional multiply of the two 16 bit input numbers
 *  with saturation.  Output a 32 bit number.
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S32 L_mult(S16 var1, S16 var2)
{
    S32 L_product;

    if (var1 == SW_MIN && var2 == SW_MIN)
    {
        L_product = LW_MAX; /* overflow */
        giOverflow = 1;
    }
    else
    {
        L_product = (S32) var1 * var2; /* integer multiply */

        L_product = L_product << 1;
    }

#ifdef WMOPS_FX
    counter_fx.L_mult++;
#endif

    return (L_product);
}

/***************************************************************************
 *
 *   FUNCTION NAME: L_negate
 *
 *   PURPOSE:
 *
 *     Negate the 32 bit input. 0x8000 0000's negated value is
 *     0x7fff ffff.
 *
 *   INPUTS:
 *
 *     L_var1
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     L_Out
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0001 <= L_var1 <= 0x7fff ffff.
 *
 *   KEYWORDS: negate, negative
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   L_var1 DOXYGEN_TO_DO
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
S32 L_negate(S32 L_var1)
{
    S32 L_Out;

    if (L_var1 == LW_MIN)
    {
        L_Out = LW_MAX;
        giOverflow = 1;
    }
    else
        L_Out = -L_var1;

#ifdef WMOPS_FX
    counter_fx.L_negate++;
#endif

    return (L_Out);
}

#if 0
/***************************************************************************
 *
 *   FUNCTION NAME: L_shift_r
 *
 *   PURPOSE:
 *
 *     Shift and round_s.  Perform a shift right. After shifting, use
 *     the last bit shifted out of the LSB to round_s the result up
 *     or down.
 *
 *   INPUTS:
 *
 *     L_var1
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     L_var1
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *
 *   IMPLEMENTATION:
 *
 *     Shift and round_s.  Perform a shift right. After shifting, use
 *     the last bit shifted out of the LSB to round_s the result up
 *     or down.  This is just like shift_r above except that the
 *     input/output is 32 bits as opposed to 16.
 *
 *     if var2 is positve perform a arithmetic left shift
 *     with saturation (see L_shl() above).
 *
 *     If var2 is zero simply return L_var1.
 *
 *     If var2 is negative perform a arithmetic right shift (L_shr)
 *     of L_var1 by (-var2)+1.  Add the LS bit of the result to
 *     L_var1 shifted right (L_shr) by -var2.
 *
 *     Note that there is no constraint on var2, so if var2 is
 *     -0xffff 8000 then -var2 is 0x0000 8000, not 0x0000 7fff.
 *     This is the reason the L_shl function is used.
 *
 *
 *   KEYWORDS:
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   L_var1 DOXYGEN_TO_DO
 *  @param   var2 DOXYGEN_TO_DO
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
 *  |var2          |              |              |              |              |              |              |
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
S32 L_shift_r(S32 L_var1, S16 var2)
{
    S32 L_Out, L_rnd;

    if (var2 < -31)
    {
        L_Out = 0;
    }
    else if (var2 < 0)
    {
        /* right shift */
        L_rnd = L_shl(L_var1, (S16)(var2 + 1)) & 0x1;
        L_Out = L_add(L_shl(L_var1, var2), L_rnd);
#ifdef WMOPS_FX
        counter_fx.L_shl-=2;
        counter_fx.L_add--;
#endif
    }
    else
    {
        L_Out = L_shl(L_var1, var2);
#ifdef WMOPS_FX
        counter_fx.L_shl--;
#endif
    }

#ifdef WMOPS_FX
    counter_fx.L_shift_r++;
#endif

    return (L_Out);
}
#endif
/***************************************************************************
 *
 *   FUNCTION NAME: L_shl
 *
 *   PURPOSE:
 *
 *     Arithmetic shift left (or right).
 *     Arithmetically shift the input left by var2.   If var2 is
 *     negative then an arithmetic shift right (L_shr) of L_var1 by
 *     -var2 is performed.
 *
 *   INPUTS:
 *
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *     L_var1
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     L_Out
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *
 *   IMPLEMENTATION:
 *
 *     Arithmetically shift the 32 bit input left by var2.  This
 *     operation maintains the sign of the input number. If var2 is
 *     negative then an arithmetic shift right (L_shr) of L_var1 by
 *     -var2 is performed.  See description of L_shr for details.
 *
 *     Equivalent to the Full-Rate GSM ">> n" operation.  Note that
 *     ANSI-C does not guarantee operation of the C ">>" or "<<"
 *     operator for negative numbers.
 *
 *   KEYWORDS: shift, arithmetic shift left,
 *
 *************************************************************************/
//-------------------------------------------------------------------------------------------
/** @brief L_shl: Perform an arithmetic left shift on 32bit signed number
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 32bit signed number
 *  @param var2 - INPUT, number of shifts
 *
 *  @return (var1 >> var2)
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
 *  |var2         |         |Input         |1        |Real          |(1:15:0)    |[-16:15]    |      |
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
 *  |Block Name          |L_shl                                |
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
 *  Perform an arithmetic left shift on 32bit signed number
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S32 L_shl(S32 L_var1, S16 var2)
{

    S32 L_Mask, L_Out = 0;
    int i, iOverflow = 0;

    if (var2 == 0 || L_var1 == 0)
    {
        L_Out = L_var1;
    }
    else if (var2 < 0)
    {
        if (var2 <= -31)
        {
            if (L_var1 > 0)
                L_Out = 0;
            else
                L_Out = 0xffffffffL;
        }
        else
        {
            L_Out = L_shr(L_var1, (S16) (-var2));
#ifdef WMOPS_FX
            counter_fx.L_shr--;
#endif
        }
    }
    else
    {
        if (var2 >= 31)
            iOverflow = 1;
        else
        {
            if (L_var1 < 0)
                L_Mask = LW_SIGN; /* sign bit mask */
            else
                L_Mask = 0x0;
            L_Out = L_var1;
            for (i = 0; i < var2 && !iOverflow; i++)
            {
                /* check the sign bit */
                L_Out = (L_Out & 0x7fffffffL) << 1;
                if ((L_Mask ^ L_Out) & LW_SIGN)
                    iOverflow = 1;
            }
        }

        if (iOverflow)
        {
            //printf("************** SATURATION in L_shl!!!! ************\n");
            /* saturate */
            if (L_var1 > 0)
                L_Out = LW_MAX;
            else
                L_Out = LW_MIN;

            giOverflow = 1;
        }
    }

#ifdef WMOPS_FX
    counter_fx.L_shl++;
#endif

    return (L_Out);
}

/***************************************************************************
 *
 *   FUNCTION NAME: L_shr
 *
 *   PURPOSE:
 *
 *     Arithmetic shift right (or left).
 *     Arithmetically shift the input right by var2.   If var2 is
 *     negative then an arithmetic shift left (shl) of var1 by
 *     -var2 is performed.
 *
 *   INPUTS:
 *
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *     L_var1
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     L_Out
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *
 *   IMPLEMENTATION:
 *
 *     Arithmetically shift the input right by var2.  This
 *     operation maintains the sign of the input number. If var2 is
 *     negative then an arithmetic shift left (shl) of L_var1 by
 *     -var2 is performed.  See description of L_shl for details.
 *
 *     The input is a 32 bit number, as is the output.
 *
 *     Equivalent to the Full-Rate GSM ">> n" operation.  Note that
 *     ANSI-C does not guarantee operation of the C ">>" or "<<"
 *     operator for negative numbers.
 *
 *   KEYWORDS: shift, arithmetic shift right,
 *
 *************************************************************************/
//-------------------------------------------------------------------------------------------
/** @brief L_shr: Perform an arithmetic right shift on 32bit signed number
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 32bit signed number
 *  @param var2 - INPUT, number of shifts
 *
 *  @return (var1 >> var2)
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
 *  |var2         |         |Input         |1        |Real          |(1:15:0)    |[-16:15]    |      |
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
 *  |Block Name          |L_shr                                |
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
 *  Perform an arithmetic right shift on 32bit signed number
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S32 L_shr(S32 L_var1, S16 var2)
{

    S32 L_Mask, L_Out;

    if (var2 == 0 || L_var1 == 0)
    {
        L_Out = L_var1;
    }
    else if (var2 < 0)
    {
        /* perform a left shift */
        /*----------------------*/
        if (var2 <= -31)
        {
            /* saturate */
            if (L_var1 > 0)
            {
                L_Out = LW_MAX;
                giOverflow = 1;
            }
            else
            {
                L_Out = LW_MIN;
                giOverflow = 1;
            }
        }
        else
        {
            L_Out = L_shl(L_var1, (S16) (-var2)); //  OP_COUNT(-2);
#ifdef WMOPS_FX
            counter_fx.L_shl--;
#endif
        }
    }
    else
    {

        if (var2 >= 31)
        {
            if (L_var1 > 0)
                L_Out = 0;
            else
                L_Out = 0xffffffffL;
        }
        else
        {
            L_Mask = 0;
            if (L_var1 < 0)
            {
                L_Mask = ~L_Mask << (32 - var2);
            }
            L_var1 >>= var2;
            L_Out = L_Mask | L_var1;
        }
    }

#ifdef WMOPS_FX
    counter_fx.L_shr++;
#endif

    return (L_Out);
}

/***************************************************************************
 *
 *   FUNCTION NAME: L_sub
 *
 *   PURPOSE:
 *
 *     Perform the subtraction of the two 32 bit input variables with
 *     saturation.
 *
 *   INPUTS:
 *
 *     L_var1
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *     L_var2
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var2 <= 0x7fff ffff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     L_Out
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *   IMPLEMENTATION:
 *
 *     Perform the subtraction of the two 32 bit input variables with
 *     saturation.
 *
 *     L_Out = L_var1 - L_var2
 *
 *     L_Out is set to 0x7fff ffff if the operation results in an
 *     overflow.  L_Out is set to 0x8000 0000 if the operation
 *     results in an underflow.
 *
 *   KEYWORDS: sub, subtraction
 *
 *************************************************************************/
//-------------------------------------------------------------------------------------------
/** @brief L_sub: Subtract two signed 32bit fixed point numbers with limit
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 32bit signed number
 *  @param var2 - INPUT, 32bit signed number
 *
 *  @return (var1+var2)
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
 *  |var2         |         |Input         |1        |Real          |(1:0:31)    |Full Range  |      |
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
 *  |Block Name          |L_sub                                |
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
 *  Subtract two signed 32bit fixed point numbers with limit
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S32 L_sub(S32 L_var1, S32 L_var2)
{
    S32 L_Sum;
    double dSum;

    dSum = (double) L_var1 - (double) L_var2;
    L_Sum = L_var1 - L_var2;

    if (dSum != L_Sum)
    {
        /* overflow occurred */
        L_Sum = L_saturate(dSum);
#ifdef WMOPS_FX
        counter_fx.L_saturate--;
#endif
    }

#ifdef WMOPS_FX
    counter_fx.L_sub++;
#endif

    return (L_Sum);
}

/***************************************************************************
 *
 *   FUNCTION NAME:mac_r
 *
 *   PURPOSE:
 *
 *     Multiply accumulate and round_s.  Fractionally multiply two 16
 *     bit numbers together with saturation.  Add that result to
 *     the 32 bit input with saturation.  Finally round_s the result
 *     into a 16 bit number.
 *
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *     L_var3
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var2 <= 0x7fff ffff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0xffff 8000 <= swOut <= 0x0000 7fff.
 *
 *   IMPLEMENTATION:
 *
 *     Fractionally multiply two 16 bit numbers together with
 *     saturation.  The only numbers which will cause saturation on
 *     the multiply are 0x8000 * 0x8000.
 *
 *     Add that result to the 32 bit input with saturation.
 *     Round the 32 bit result by adding 0x0000 8000 to the input.
 *     The result may overflow due to the add.  If so, the result
 *     is saturated.  The 32 bit round_sed number is then shifted
 *     down 16 bits and returned as a S16.
 *
 *     Please note that this is not a true multiply accumulate as
 *     most processors would implement it.  The 0x8000*0x8000
 *     causes and overflow for this instruction.  On most
 *     processors this would cause an overflow only if the 32 bit
 *     input added to it were positive or zero.
 *
 *   KEYWORDS: mac, multiply accumulate, macr
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   L_var3 DOXYGEN_TO_DO
 *  @param   var1 DOXYGEN_TO_DO
 *  @param   var2 DOXYGEN_TO_DO
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
 *  |L_var3        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |var1          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |var2          |              |              |              |              |              |              |
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
S16 mac_r(S32 L_var3, S16 var1, S16 var2)
{
    return (round_s(L_mac(L_var3, var1, var2)));
}

/***************************************************************************
 *
 *   FUNCTION NAME:  msu_r
 *
 *   PURPOSE:
 *
 *     Multiply subtract and round_s.  Fractionally multiply two 16
 *     bit numbers together with saturation.  Subtract that result from
 *     the 32 bit input with saturation.  Finally round_s the result
 *     into a 16 bit number.
 *
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *     L_var3
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var2 <= 0x7fff ffff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0xffff 8000 <= swOut <= 0x0000 7fff.
 *
 *   IMPLEMENTATION:
 *
 *     Fractionally multiply two 16 bit numbers together with
 *     saturation.  The only numbers which will cause saturation on
 *     the multiply are 0x8000 * 0x8000.
 *
 *     Subtract that result from the 32 bit input with saturation.
 *     Round the 32 bit result by adding 0x0000 8000 to the input.
 *     The result may overflow due to the add.  If so, the result
 *     is saturated.  The 32 bit round_sed number is then shifted
 *     down 16 bits and returned as a S16.
 *
 *     Please note that this is not a true multiply accumulate as
 *     most processors would implement it.  The 0x8000*0x8000
 *     causes and overflow for this instruction.  On most
 *     processors this would cause an overflow only if the 32 bit
 *     input added to it were positive or zero.
 *
 *   KEYWORDS: mac, multiply accumulate, macr
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   L_var3 DOXYGEN_TO_DO
 *  @param   var1 DOXYGEN_TO_DO
 *  @param   var2 DOXYGEN_TO_DO
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
 *  |L_var3        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |var1          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |var2          |              |              |              |              |              |              |
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
S16 msu_r(S32 L_var3, S16 var1, S16 var2)
{
    return (round_s(L_msu(L_var3, var1, var2)));
}

/***************************************************************************
 *
 *   FUNCTION NAME: mult
 *
 *   PURPOSE:
 *
 *     Perform a fractional multipy of the two 16 bit input numbers
 *     with saturation and truncation.
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0xffff 8000 <= swOut <= 0x0000 7fff.
 *
 *   IMPLEMENTATION:
 *
 *     Perform a fractional multipy of the two 16 bit input
 *     numbers.  If var1 == var2 == -0x8000, output 0x7fff.
 *     Otherwise output var1*var2 >> 15.  The output is a
 *     16 bit number.
 *
 *   KEYWORDS: mult, mulitply, mpy
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief mult: Multiply two signed 16bit fixed point numbers
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 16bit signed number
 *  @param var2 - INPUT, 16bit signed number
 *
 *  @return (var1*var2 >> 15)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var1         |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var2         |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |return       |         |Output        |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |mult                                 |
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
 *  Multiply two signed 16bit fixed point numbers
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 mult(S16 var1, S16 var2)
{
    S32 L_product;
    S16 swOut;

    L_product = L_mult(var1, var2);
    swOut = extract_h(L_product);

#ifdef WMOPS_FX
    counter_fx.mult++;
    counter_fx.L_mult--;
    counter_fx.extract_h--;
#endif

    return (swOut);
}

/***************************************************************************
 *
 *   FUNCTION NAME: mult_r
 *
 *   PURPOSE:
 *
 *     Perform a fractional multipy and round_s of the two 16 bit
 *     input numbers with saturation.
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0xffff 8000 <= swOut <= 0x0000 7fff.
 *
 *   IMPLEMENTATION:
 *
 *     This routine is defined as the concatenation of the multiply
 *     operation and the round_s operation.
 *
 *     The fractional multiply (L_mult) produces a saturated 32 bit
 *     output.  This is followed by a an add of 0x0000 8000 to the
 *     32 bit result.  The result may overflow due to the add.  If
 *     so, the result is saturated.  The 32 bit round_sed number is
 *     then shifted down 16 bits and returned as a S16.
 *
 *
 *   KEYWORDS: multiply and round_s, round_s, mult_r, mpyr
 *
 *************************************************************************/
//---------------------------------------------------------------------------------------------------------
/** @brief mult_r:  Multiply 2 signed 16 bit numbers with rounding and return output in 16 bit signed number
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 16bit signed number
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
 *  |var1         |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
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
 *  |Block Name          |mult_r                               |
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
 *  Perform a fractional multiply and round_s of the two 16 bit
 *  input numbers with saturation.
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 mult_r(S16 var1, S16 var2)
{
    S16 swOut;

    swOut = round_s(L_mult(var1, var2));

#ifdef WMOPS_FX
    counter_fx.mult_r++;
    counter_fx.L_mult--;
    counter_fx.round_s--;
#endif

    return (swOut);
}

/***************************************************************************
 *
 *   FUNCTION NAME: negate
 *
 *   PURPOSE:
 *
 *     Negate the 16 bit input. 0x8000's negated value is 0x7fff.
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0xffff 8001 <= swOut <= 0x0000 7fff.
 *
 *   KEYWORDS: negate, negative, invert
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief negate: Negate the 16 bit input. 0x8000's negated value is 0x7fff.
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 16bit signed number
 *
 *  @return (-var1)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var1         |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |return       |         |Output        |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |negate                               |
 *  ------------------------------------------------------------
 *  |Input Streams       |var1                                 |
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
 *  Negate the 16 bit input. 0x8000's negated value is 0x7fff.
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 negate(S16 var1)
{
    S16 swOut;

    if (var1 == SW_MIN)
    {
        swOut = SW_MAXV;
        giOverflow = 1;
    }
    else
        swOut = -var1;

#ifdef WMOPS_FX
    counter_fx.negate++;
#endif

    return (swOut);
}

/***************************************************************************
 *
 *   FUNCTION NAME: norm_l
 *
 *   PURPOSE:
 *
 *     Get normalize shift count:
 *
 *     A 32 bit number is input (possiblly unnormalized).  Output
 *     the positive (or zero) shift count required to normalize the
 *     input.
 *
 *   INPUTS:
 *
 *     L_var1
 *                     32 bit long signed integer (S32) whose value
 *                     falls in the range
 *                     0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0 <= swOut <= 31
 *
 *
 *
 *   IMPLEMENTATION:
 *
 *     Get normalize shift count:
 *
 *     A 32 bit number is input (possiblly unnormalized).  Output
 *     the positive (or zero) shift count required to normalize the
 *     input.
 *
 *     If zero in input, return 0 as the shift count.
 *
 *     For non-zero numbers, count the number of left shift
 *     required to get the number to fall into the range:
 *
 *     0x4000 0000 >= normlzd number >= 0x7fff ffff (positive number)
 *     or
 *     0x8000 0000 <= normlzd number < 0xc000 0000 (negative number)
 *
 *     Return the number of shifts.
 *
 *     This instruction corresponds exactly to the Full-Rate "norm"
 *     instruction.
 *
 *   KEYWORDS: norm, normalization
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   L_var1 DOXYGEN_TO_DO
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
S16 norm_l(S32 L_var1)
{
    S16 swShiftCnt;

    if (L_var1 != 0)
    {
        if (!(L_var1 & LW_SIGN))
        {
            /* positive input */
            for (swShiftCnt = 0; !(L_var1 <= LW_MAX && L_var1 >= 0x40000000L); swShiftCnt++)
            {
                L_var1 = L_var1 << 1;
            }
        }
        else
        {
            /* negative input */
            for (swShiftCnt = 0; !(L_var1 >= LW_MIN && L_var1 < (S32) 0xc0000000L); swShiftCnt++)
            {
                L_var1 = L_var1 << 1;
            }
        }
    }
    else
    {
        swShiftCnt = 0;
    }

#ifdef WMOPS_FX
    counter_fx.norm_l++;
#endif

    return (swShiftCnt);
}

/***************************************************************************
 *
 *   FUNCTION NAME: norm_s
 *
 *   PURPOSE:
 *
 *     Get normalize shift count:
 *
 *     A 16 bit number is input (possiblly unnormalized).  Output
 *     the positive (or zero) shift count required to normalize the
 *     input.
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0 <= swOut <= 15
 *
 *
 *
 *   IMPLEMENTATION:
 *
 *     Get normalize shift count:
 *
 *     A 16 bit number is input (possiblly unnormalized).  Output
 *     the positive (or zero) shift count required to normalize the
 *     input.
 *
 *     If zero in input, return 0 as the shift count.
 *
 *     For non-zero numbers, count the number of left shift
 *     required to get the number to fall into the range:
 *
 *     0x4000 >= normlzd number >= 0x7fff (positive number)
 *     or
 *     0x8000 <= normlzd number <  0xc000 (negative number)
 *
 *     Return the number of shifts.
 *
 *     This instruction corresponds exactly to the Full-Rate "norm"
 *     instruction.
 *
 *   KEYWORDS: norm, normalization
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
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
S16 norm_s(S16 var1)
{
    short swShiftCnt;
    S32 L_var1;

    L_var1 = L_deposit_h(var1);
    swShiftCnt = norm_l(L_var1);

#ifdef WMOPS_FX
    counter_fx.norm_s++;
    counter_fx.L_deposit_h--;
    counter_fx.norm_l--;
#endif

    return (swShiftCnt);
}

/****************************************************************************
 *
 *     FUNCTION NAME: popOverflow
 *
 *     PURPOSE:
 *
 *        Pull the old overflow state from the "stack".  Replace the current
 *        overflow status with its predecessor.
 *
 *     INPUTS:
 *
 *       none
 *
 *
 *     OUTPUTS:             none
 *
 *     RETURN VALUE:        value of datum about the be lost (usually the
 *                          temporary saturation state)
 *
 *     KEYWORDS: saturation, limit, overflow
 *
 ***************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   void DOXYGEN_TO_DO
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
 *  |void          |              |              |              |              |              |              |
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
int popOverflow(void)
{
    int i;
    i = giOverflow;
    giOverflow = giOldOverflow;
    return (i);
}

/****************************************************************************
 *
 *     FUNCTION NAME: set overflow
 *
 *     PURPOSE:
 *
 *        Clear the overflow flag
 *
 *     INPUTS:
 *
 *       none
 *
 *
 *     OUTPUTS:             global overflow flag is cleared
 *                          previous value stored in giOldOverflow
 *
 *     RETURN VALUE:        previous value of overflow
 *
 *
 *     KEYWORDS: saturation, limit, overflow
 *
 ***************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   void DOXYGEN_TO_DO
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
 *  |void          |              |              |              |              |              |              |
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
int setOverflow(void)
{

    giOldOverflow = giOverflow;
    giOverflow = 1;
    return (giOldOverflow);
}

/***************************************************************************
 *
 *   FUNCTION NAME: shl
 *
 *   PURPOSE:
 *
 *     Arithmetically shift the input left by var2.
 *
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0xffff 8000 <= swOut <= 0x0000 7fff.
 *
 *   IMPLEMENTATION:
 *
 *     If Arithmetically shift the input left by var2.  If var2 is
 *     negative then an arithmetic shift right (shr) of var1 by
 *     -var2 is performed.  See description of shr for details.
 *     When an arithmetic shift left is performed the var2 LS bits
 *     are zero filled.
 *
 *     The only exception is if the left shift causes an overflow
 *     or underflow.  In this case the LS bits are not modified.
 *     The number returned is 0x8000 in the case of an underflow or
 *     0x7fff in the case of an overflow.
 *
 *     The shl is equivalent to the Full-Rate GSM "<< n" operation.
 *     Note that ANSI-C does not guarantee operation of the C ">>"
 *     or "<<" operator for negative numbers - it is not specified
 *     whether this shift is an arithmetic or logical shift.
 *
 *   KEYWORDS: asl, arithmetic shift left, shift
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief shl: Perform an arithmetic left shift on 16bit signed number
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 16bit signed number
 *  @param var2 - INPUT, number of shifts
 *
 *  @return (var1 >> var2)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var1         |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var2         |         |Input         |1        |Real          |(1:15:0)    |[-16:15]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  |return       |         |Output        |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |shl                                  |
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
 *  Perform an arithmetic left shift on 16bit signed number
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 shl(S16 var1, S16 var2)
{
    S16 swOut;
    S32 L_Out;

    if (var2 == 0 || var1 == 0)
    {
        swOut = var1;
    }
    else if (var2 < 0)
    {
        /* perform a right shift */
        /*-----------------------*/
        if (var2 <= -15)
        {
            if (var1 < 0)
                swOut = (S16) 0xffff;
            else
                swOut = 0x0;
        }
        else
        {
            swOut = shr(var1, (S16) (-var2));
#ifdef WMOPS_FX
            counter_fx.shr--;
#endif
        }
    }
    else
    {
        /* var2 > 0 */
        if (var2 >= 15)
        {
            /* saturate */
            if (var1 > 0)
                swOut = SW_MAXV;
            else
                swOut = SW_MIN;
            giOverflow = 1;
        }
        else
        {
            L_Out = (S32) var1 * (1 << var2);

            swOut = (S16) L_Out; /* copy low portion to swOut, overflow
             * could have hpnd */
            if (swOut != L_Out)
            {
                /* overflow  */
                if (var1 > 0)
                    swOut = SW_MAXV; /* saturate */
                else
                    swOut = SW_MIN; /* saturate */
                giOverflow = 1;
            }
        }
    }

#ifdef WMOPS_FX
    counter_fx.shl++;
#endif

    return (swOut);
}

/***************************************************************************
 *
 *   FUNCTION NAME: shr
 *
 *   PURPOSE:
 *
 *     Arithmetic shift right (or left).
 *     Arithmetically shift the input right by var2.   If var2 is
 *     negative then an arithmetic shift left (shl) of var1 by
 *     -var2 is performed.
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0xffff 8000 <= swOut <= 0x0000 7fff.
 *
 *   IMPLEMENTATION:
 *
 *     Arithmetically shift the input right by var2.  This
 *     operation maintains the sign of the input number. If var2 is
 *     negative then an arithmetic shift left (shl) of var1 by
 *     -var2 is performed.  See description of shl for details.
 *
 *     Equivalent to the Full-Rate GSM ">> n" operation.  Note that
 *     ANSI-C does not guarantee operation of the C ">>" or "<<"
 *     operator for negative numbers.
 *
 *   KEYWORDS: shift, arithmetic shift right,
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief shr: Perform an arithmetic right shift
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 16bit signed number
 *  @param var2 - INPUT, number of shifts
 *
 *  @return (var1 >> var2)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var1         |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var2         |         |Input         |1        |Real          |(1:15:0)    |[-16:15]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  |return       |         |Output        |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |shr                                  |
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
 *  Perform an arithmetic right shift 16bit signed number
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 shr(S16 var1, S16 var2)
{
    S16 swMask, swOut;

    if (var2 == 0 || var1 == 0)
        swOut = var1;

    else if (var2 < 0)
    {
        /* perform an arithmetic left shift */
        /*----------------------------------*/
        if (var2 <= -15)
        {
            /* saturate */
            if (var1 > 0)
                swOut = SW_MAXV;
            else
                swOut = SW_MIN;
            giOverflow = 1;
        }
        else
        {
            swOut = shl(var1, (S16) (-var2)); // OP_COUNT(-1);
#ifdef WMOPS_FX
            counter_fx.shl--;
#endif
        }
    }
    else
    {
        /* positive shift count */
        /*----------------------*/

        if (var2 >= 15)
        {
            if (var1 < 0)
                swOut = (S16) 0xffff;
            else
                swOut = 0x0;
        }
        else
        {
            /* take care of sign extension */
            /*-----------------------------*/

            swMask = 0;
            if (var1 < 0)
            {
                swMask = ~swMask << (16 - var2);
            }

            var1 >>= var2;
            swOut = swMask | var1;
        }
    }

#ifdef WMOPS_FX
    counter_fx.shr++;
#endif

    return (swOut);
}

/***************************************************************************
 *
 *   FUNCTION NAME: sub
 *
 *   PURPOSE:
 *
 *     Perform the subtraction of the two 16 bit input variable with
 *     saturation.
 *
 *   INPUTS:
 *
 *     var1
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *     var2
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *   OUTPUTS:
 *
 *     none
 *
 *   RETURN VALUE:
 *
 *     swOut
 *                     16 bit short signed integer (S16) whose value
 *                     falls in the range
 *                     0xffff 8000 <= swOut <= 0x0000 7fff.
 *
 *   IMPLEMENTATION:
 *
 *     Perform the subtraction of the two 16 bit input variable with
 *     saturation.
 *
 *     swOut = var1 - var2
 *
 *     swOut is set to 0x7fff if the operation results in an
 *     overflow.  swOut is set to 0x8000 if the operation results
 *     in an underflow.
 *
 *   KEYWORDS: sub, subtraction
 *
 *************************************************************************/

//-------------------------------------------------------------------------------------------
/** @brief sub: Substract two signed 16bit fixed point numbers with limit
 *
 *  <1> Hardware Resource:
 *  ARM/Ceva
 *
 *  @param var1 - INPUT, 16bit signed number
 *  @param var2 - INPUT, 16bit signed number
 *
 *  @return (var1-var2)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |         |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var1         |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |var2         |         |Input         |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |return       |         |Output        |1        |Real          |(1:0:15)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------
 *  |Block Name          |sub                                  |
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
 *  Substract two signed 16bit fixed point numbers with limit
 *
 *  <5> Cycle Count Formula:
 *  1 cycle
 *
 *  <6> References:
 *
 *  \ingroup group_lte_utilities
 */
//-------------------------------------------------------------------------------------------
S16 sub(S16 var1, S16 var2)
{
    S32 L_diff;
    S16 swOut;

    L_diff = (S32) var1 - var2;
    swOut = saturate(L_diff);

#ifdef WMOPS_FX
    counter_fx.sub++;
#endif

    return (swOut);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : div_s                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Produces a result which is the fractional integer division of var1 by  |
 |   var2; var1 and var2 must be positive and var2 must be greater or equal  |
 |   to var1; the result is positive (leading bit equal to 0) and truncated  |
 |   to 16 bits.                                                             |
 |   If var1 = var2 then div(var1,var2) = 32767.                             |
 |                                                                           |
 |   Complexity weight : 18                                                  |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (S16) whose value falls in the |
 |             range : 0x0000 0000 <= var1 <= var2 and var2 != 0.            |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (S16) whose value falls in the |
 |             range : var1 <= var2 <= 0x0000 7fff and var2 != 0.            |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (S16) whose value falls in the |
 |             range : 0x0000 0000 <= var_out <= 0x0000 7fff.                |
 |             It's a Q15 value (point between b15 and b14).                 |
 |___________________________________________________________________________|
 */

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   var1 DOXYGEN_TO_DO
 *  @param   var2 DOXYGEN_TO_DO
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
 *  |var1          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |var2          |              |              |              |              |              |              |
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
S16 div_s(S16 var1, S16 var2)
{
    S16 var_out = (S16) 0;
    S16 iteration;
    S32 L_num;
    S32 L_denom;

    if ((var1 > var2) || (var1 < (S16) 0) || (var2 < (S16) 0))
    {
        printf("Division Error\n");
        exit(0);
    }

    if (var2 == (S16) 0)
    {
        printf("Division by 0, Fatal error \n");
        exit(0);
    }

    if (var1 == (S16) 0)
    {
        var_out = (S16) 0;
    }
    else
    {
        if (var1 == var2)
        {
            var_out = MAX_16;
        }
        else
        {
            L_num = L_deposit_l(var1);
            L_denom = L_deposit_l(var2);

            for (iteration = (S16) 0; iteration < (S16) 15; iteration++)
            {
                var_out <<= 1;
                L_num <<= 1;

                if (L_num >= L_denom)
                {
                    L_num = L_sub(L_num, L_denom);
                    var_out = add(var_out, (S16) 1);
                }
            }
        }
    }

    return (var_out);
}

