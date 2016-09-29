//-------------------------------------------------------------------------------------------
/** @file bs_rx_prach.h
 *
 * @brief This file defines the data structure that is used
 *        in the implementation of PRACH
 * @author Mindspeed Technologies
 * @version $Revision: 1.15 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef BS_RX_PRACH_H
#define BS_RX_PRACH_H

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
#include "math_ext32.h"
#include "basic_op_cc.h"
#include "dft_table16.h"

#include "gputil_fx.h"

#define SEGMSIZE 3840
#define EXTDSIZE 64     //16      // Extended array size, 2*FIR_length
#define NZC      839
#define NZC2     1678

#endif  // BS_RX_PRACH_H
