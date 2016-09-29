
/******************************************************************************** 
**	Copyright (c) 2011, Mindspeed
**			All Rights Reserved.
**
**  File:
**  Created By:  Qingyu Gao	
**	Created On:  07/20/2011
**  Purpose:
**
**	History:
**	Programmer		Date		Rev		Description
**  ----------------------------------------------------------------------------
**	Guice		06/1/04	0.1		Created.
********************************************************************************/


#ifndef  DE_SCRAMBLING_H
#define  DE_SCRAMBLING_H
#ifndef VS_ENV
#include "..//prototypes.h"
#endif

#include "UMTSNwlCommMacro.h"

#ifdef VS_ENV
void descram_TS(S16 * const pucDataIn, S16 *pucDataOut, S8 * const pucScrcode, const U32 dSampRate, const U32 dLen);
#else
void descram_TS(S16 * const pucDataIn, S16 *pucDataOut, S8 * const pucScrcode, const U32 dSampRate, const U32 dLen)CEVAEXTERN;
#endif

#endif


