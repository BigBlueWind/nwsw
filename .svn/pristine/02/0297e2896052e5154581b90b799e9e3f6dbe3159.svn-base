#ifndef EXTERN_CFNS
#define EXTERN_CFNS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<math.h>

#include "UMTSNwlCommMacro.h"
#ifndef VS_ENV
#include "..//prototypes.h"
#endif

// Define number of 3G 1/2 rate CC encoder internal states
#define     P_CC_3G_STATE_1_2       256

// Define number of 3G 1/3 rate CC encoder internal states
#define     P_CC_3G_STATE_1_3       256


#ifndef VS_ENV
//void dec_cc_3g_1_2(S8 IN[], S8 OUT[], S16 N,S8 *pucDectab);
void dec_cc_3g_1_2(S8 IN[], S8 OUT[], S16 N,S8 *pucDectab,S32 *metric, S32 *newmetric,U8 *tb_buf)CEVAEXTERN;
#else
void dec_cc_3g_1_2(S8 IN[], S8 OUT[], S16 N,S8 *pucDectab,S32 *metric, S32 *newmetric,U8 *tb_buf);
#endif
//void dec_cc_3g_1_2(S8 IN[], S8 OUT[], S16 N);
//void dec_cc_3g_1_3(S8 IN[], U8 OUT[], S32 N);


#endif
