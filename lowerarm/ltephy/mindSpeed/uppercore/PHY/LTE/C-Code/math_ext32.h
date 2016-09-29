//-------------------------------------------------------------------------------------------
/** @file math_ext32.h
 *
 * @brief 32-bit math function declarations.
 * @author Mindspeed Technologies
 *
 * COPYRIGHT&copy; 2009-2010 Mindspeed Technologies.
 *
 * ALL RIGHTS RESERVED
 *     Copyright (C) 1997-1998 Telecommunications Industry Association.
 *     All rights reserved.
 * Note:  Reproduction and use of this software for the design and
 *     development of North American Wideband CDMA Digital
 *     Cellular Telephony Standards is authorized by the TIA.
 *     The TIA does not authorize the use of this software for any
 *     other purpose.
 *
 *     The availability of this software does not provide any license
 *     by implication, estoppel, or otherwise under any patent rights
 *     of TIA member companies or others covering any use of the
 *     contents herein.
 *
 *     Any copies of this software or derivative works must include
 *     this and all other proprietary notices.
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------


#ifndef MATHEXT32_H
#define MATHEXT32_H

/*_________________________________________________________________________
 |                                                                         |
 |                            Function Prototypes                          |
 |_________________________________________________________________________|
 */

// DPF:
S32 L_mpy_ll(S32 L_var1, S32 L_var2);
S32 L_mpy_ls(S32 L_var2, S16 var1);

// non-DPF:
#ifdef _WIN32
S32 L_mpy_ll_nonDPF(S32 L_var1, S32 L_var2);
S32 L_mpy_ls_nonDPF(S32 L_var2, S16 var1);
#endif

#endif /* MATHEXT32_H */
