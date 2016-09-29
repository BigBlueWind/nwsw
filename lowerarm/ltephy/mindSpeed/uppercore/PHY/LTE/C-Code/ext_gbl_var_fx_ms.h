//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL ext_gbl_var_fx_ms.h
 *
 * @brief DOXYGEN_TO_DO
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT&copy; 2009-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/

//-------------------------------------------------------------------------------------------


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<math.h>

#include "const_ms.h"
#ifndef EXTGBVARFXMS
#define EXTGBVARFXMS
//MOBILE STATION

extern S16 firSyncDelay;
extern S16 USR;
extern S16 *firLpfMid6;
extern S16 length_firLpfMid6;
extern S32 length_adcbuf_1;

extern S16 **rgQm;
extern S16 numrows_rgQm, numcols_rgQm;




extern S16 adcbuf[4][2*32769];

extern S16 USR;
#endif
