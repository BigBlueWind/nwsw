#ifndef _LTE_TABLE_TDD_H
#define _LTE_TABLE_TDD_H

#include "osal.h"
#include "resultcodes.h"
#include "basetypes.h"
typedef struct stTRACHSFQuadruple 
{
	U8 fRA;  // frequency resource index 
	U8 t0RA; // [0,1,2] - indicates whether the resource is reoccurring in all radio frames, in even radio frames, or in odd radio frames, respectively
	U8 t1RA; //   [0,1] - indicates whether the random access resource is located in first half frame or in second half frame, respectively
	U8 t2RA; // [0,1,2] - the uplink subframe number where the preamble starts, counting from 0 at the first uplink subframe between 
			   //			two consecutive downlink-to-uplink switch points, with the exception of preamble format 4 where t2RA is denoted 
			   //			as * (marked as 8 in table below).
			   //       9 - N/A value.
}TRACHSFQuadruple;

// RACHMapping[PRACH config index][TDD config][SFMap]
extern const TRACHSFQuadruple RACHMapping[64][7][6];
extern const U8 **E_TM_2MapTDD[6];
extern const U8 **E_TM_1_2MapTDD[6];
extern const U8 **E_TM_1_2MapTDD_UE2[6];
extern const U8 **E_TM_3_2MapTDD[6];
extern const U8 **E_TM_3_2MapTDD_UE2[6];
extern const U8 **E_TM_3_3MapTDD[6];
extern const U8 **E_TM_3_3MapTDD_UE2[6];
extern const U8 **E_TM_x_1MapTDD[6];
#endif /*_LTE_TABLE_TDD_H*/