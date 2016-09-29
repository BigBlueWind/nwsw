//-------------------------------------------------------------------------------------------
/** @file basic_op.h (essentially identical to mathevrc.h)
 *
 * @brief Modified TIA basic operations.  Bit-exact simulation of a
 * generic 32 bit accumulator DSP chip (fractional math).  This
 * version has a latching overflow bit (giOverflow) and
 * non-compound MAC's (One where initial mult does not saturate)
 * J. Klein, Conexant Systems, Inc. 11/30/00
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT&copy; 2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * Copyright (C) 1997-1998 Telecommunications Industry Association.
 * All rights reserved.
 *
 * Note: Reproduction and use of this software for the design and
 *       development of North American Wideband CDMA Digital
 *       Cellular Telephony Standards is authorized by the TIA.
 *       The TIA does not authorize the use of this software for any
 *       other purpose.
 *
 *       The availability of this software does not provide any license
 *       by implication, estoppel, or otherwise under any patent rights
 *       of TIA member companies or others covering any use of the
 *       contents herein.
 *
 *       Any copies of this software or derivative works must include
 *       this and all other proprietary notices.
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef __MATHTIA
#define __MATHTIA

//#include "typedef.h"

#define MAX_32 (S32)0x7fffffffL
#define MIN_32 (S32)0x80000000L

#define MAX_16 (S16)0x7fff
#define MIN_16 (S16)0x8000

/*_________________________________________________________________________
 |                                                                         |
 |                            Function Prototypes                          |
 |_________________________________________________________________________|
 */

/* addition */
/************/

S16 add(S16 var1, S16 var2); /* 1 ops */
S16 sub(S16 var1, S16 var2); /* 1 ops */
S32 L_add(S32 L_var1, S32 L_var2); /* 2 ops */
S32 L_sub(S32 L_var1, S32 L_var2); /* 2 ops */

/* multiplication */
/******************/

S16 mult(S16 var1, S16 var2); /* 1 ops */
S32 L_mult(S16 var1, S16 var2); /* 1 ops */
S16 mult_r(S16 var1, S16 var2); /* 2 ops */

/* arithmetic shifts */
/*********************/

S16 shr(S16 var1, S16 var2); /* 1 ops */
S16 shl(S16 var1, S16 var2); /* 1 ops */
S32 L_shr(S32 L_var1, S16 var2); /* 2 ops */
S32 L_shl(S32 L_var1, S16 var2); /* 2 ops */
S16 shift_r(S16 var, S16 var2); /* 2 ops */
S32 L_shift_r(S32 L_var, S16 var2); /* 3 ops */

/* absolute value  */
/*******************/

S16 abs_s(S16 var1); /* 1 ops */
S32 L_abs(S32 var1); /* 3 ops */

/* multiply accumulate  */
/************************/

S32 L_mac(S32 L_var3, S16 var1, S16 var2); /* 1 op */
S16 mac_r(S32 L_var3, S16 var1, S16 var2); /* 2 op */
S32 L_msu(S32 L_var3, S16 var1, S16 var2); /* 1 op */
S16 msu_r(S32 L_var3, S16 var1, S16 var2); /* 2 op */

/* negation  */
/*************/

S16 negate(S16 var1); /* 1 ops */
S32 L_negate(S32 L_var1); /* 2 ops */

/* Accumulator manipulation */
/****************************/

S32 L_deposit_l(S16 var1); /* 1 ops */
S32 L_deposit_h(S16 var1); /* 1 ops */
S16 extract_l(S32 L_var1); /* 1 ops */
S16 extract_h(S32 L_var1); /* 1 ops */

/* Round */
/*********/

S16 round_s(S32 L_var1); /* 1 ops */

/* Normalization */
/*****************/

S16 norm_l(S32 L_var1); /* 30 ops */
S16 norm_s(S16 var1); /* 15 ops */

/* Division */
/************/
S16 divide_s(S16 var1, S16 var2); /* 18 ops */

/* Saturation manipulation routines */
/************************************/

int clearOverflow(void);
int isOverflow(void);
int popOverflow(void);
int setOverflow(void);

/* Non-saturating instructions */
/*******************************/
S32 L_add_c(S32 L_Var1, S32 L_Var2); /* 2 ops */
S32 L_sub_c(S32 L_Var1, S32 L_Var2); /* 2 ops */
S32 L_macNs(S32 L_var3, S16 var1, S16 var2); /* 1 ops */
S32 L_msuNs(S32 L_var3, S16 var1, S16 var2); /* 1 ops */

#endif /* __MATHTIA */
