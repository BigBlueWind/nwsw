#ifdef __cplusplus
extern "C"
{
#endif


#ifndef __FP_IDFT_DATA_H__
#define __FP_IDFT_DATA_H__


#include "4gmx.h"

// Common Tables
extern unsigned char fp_idft_size_index_map[];
extern unsigned short fp_idft_big_config_start[];
extern unsigned short fp_idft_mult_table_start[];

extern unsigned long fp_idft_exp_twiddle_size;
extern unsigned short fp_idft_exp_twiddle[];

extern unsigned long fp_idft_big_config_size;
extern unsigned short fp_idft_big_config[];

extern unsigned short *fp_idft_tables[];
extern unsigned short fp_idft_table_sizes[];
extern unsigned short fp_idft_table_sizes_with_offset[];

// Reference Input / Output for IDFT Diagnostic
extern unsigned short fp_idft_indata[];
extern unsigned short fp_idft0012_outref[];
extern unsigned short fp_idft0024_outref[];
extern unsigned short fp_idft0036_outref[];
extern unsigned short fp_idft0048_outref[];
extern unsigned short fp_idft0060_outref[];
extern unsigned short fp_idft0072_outref[];
extern unsigned short fp_idft0096_outref[];
extern unsigned short fp_idft0108_outref[];
extern unsigned short fp_idft0120_outref[];
extern unsigned short fp_idft0144_outref[];
extern unsigned short fp_idft0180_outref[];
extern unsigned short fp_idft0192_outref[];
extern unsigned short fp_idft0216_outref[];
extern unsigned short fp_idft0240_outref[];
extern unsigned short fp_idft0288_outref[];
extern unsigned short fp_idft0300_outref[];
extern unsigned short fp_idft0324_outref[];
extern unsigned short fp_idft0360_outref[];
extern unsigned short fp_idft0384_outref[];
extern unsigned short fp_idft0432_outref[];
extern unsigned short fp_idft0480_outref[];
extern unsigned short fp_idft0540_outref[];
extern unsigned short fp_idft0576_outref[];
extern unsigned short fp_idft0600_outref[];
extern unsigned short fp_idft0648_outref[];
extern unsigned short fp_idft0720_outref[];
extern unsigned short fp_idft0768_outref[];
extern unsigned short fp_idft0864_outref[];
extern unsigned short fp_idft0900_outref[];
extern unsigned short fp_idft0960_outref[];
extern unsigned short fp_idft0972_outref[];
extern unsigned short fp_idft1080_outref[];
extern unsigned short fp_idft1152_outref[];
extern unsigned short fp_idft1200_outref[];
extern unsigned short fp_idft1296_outref[];
extern unsigned short *fp_idft_outrefs[];

#endif //__FP_IDFT_DATA_H__

#ifdef __cplusplus
}
#endif

