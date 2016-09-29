/******************************************************************************** 
**	Copyright (c) 2011, Mindspeed
**			All Rights Reserved.
**
**  File: DeInterleaving_2nd
**  Created By:  Qingyu Gao	
**	Created On:  07/20/2011
**  Purpose:
**
**	History:
**	Programmer		Date		Rev		Description
**  ----------------------------------------------------------------------------
**	Guice		06/1/04	0.1		Created.
********************************************************************************/

#ifndef  DE_INTERLEAVING_2ND_H
#define  DE_INTERLEAVING_2ND_H

#include<stdio.h>
#include <string.h> 

#ifndef VS_ENV
#include "..//prototypes.h"
#endif

#include "UMTSNwlCommMacro.h"

#ifdef VS_ENV
void De2Interleaving_PCCPCH(const S8 *pucInData,S8 *pucOutData);
#else
void De2Interleaving_PCCPCH(const S8 *pucInData,S8 *pucOutData)CEVAEXTERN;
#endif

#endif
