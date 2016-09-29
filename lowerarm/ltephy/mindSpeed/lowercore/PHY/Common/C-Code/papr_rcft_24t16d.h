//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL papr_rcft_24t16d.h
 *
 * @brief DOXYGEN_TO_DO
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT&copy; 2008-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef PAPRRCFT16T24D_H
#define PAPRRCFT16T24D_H

// PAPR mode
//
//------------------------------------------------------------------
// LTE
//
#define PAPR_01_4_MHZ_L_0   0   //  1.4MHz, no over-sampling
#define PAPR_03_0_MHZ_L_0   1   //  3.0MHz, no over-sampling
#define PAPR_05_0_MHZ_L_0   2   //  5.0MHz, no over-sampling
#define PAPR_10_0_MHZ_L_0   3   // 10.0MHz, no over-sampling
#define PAPR_15_0_MHZ_L_0   4   // 15.0MHz, no over-sampling
#define PAPR_20_0_MHZ_L_0   5   // 20.0MHz, no over-sampling
#define PAPR_01_4_MHZ_L_1   10   //  1.4MHz, 2 time over-sampling
#define PAPR_03_0_MHZ_L_1   11   //  3.0MHz, 2 time over-sampling
#define PAPR_05_0_MHZ_L_1   12   //  5.0MHz, 2 time over-sampling
#define PAPR_10_0_MHZ_L_1   13   // 10.0MHz, 2 time over-sampling
#define PAPR_15_0_MHZ_L_1   14   // 15.0MHz, 2 time over-sampling
#define PAPR_20_0_MHZ_L_1   15   // 20.0MHz, 2 time over-sampling
//------------------------------------------------------------------
// WiMAX
// Oversampling is required in WiMAX
//
#define PAPR_WIMAX_PUSC_0128_L_1   20   //  WiMAX PUSC nFFT= 128, Nused=  85, 2 time over-sampling
#define PAPR_WIMAX_PUSC_0512_L_1   21   //  WiMAX PUSC nFFT= 512, Nused= 421, 2 time over-sampling
#define PAPR_WIMAX_PUSC_1024_L_1   22   //  WiMAX PUSC nFFT=1024, Nused= 841, 2 time over-sampling
#define PAPR_WIMAX_PUSC_2048_L_1   23   //  WiMAX PUSC nFFT=2048, Nused=1681, 2 time over-sampling
#define PAPR_WIMAX_FUSC_0128_L_1   30   //  WiMAX FUSC nFFT= 128, Nused= 107, 2 time over-sampling
#define PAPR_WIMAX_FUSC_0512_L_1   31   //  WiMAX FUSC nFFT= 512, Nused= 427, 2 time over-sampling
#define PAPR_WIMAX_FUSC_1024_L_1   32   //  WiMAX FUSC nFFT=1024, Nused= 851, 2 time over-sampling
#define PAPR_WIMAX_FUSC_2048_L_1   33   //  WiMAX FUSC nFFT=2048, Nused=1703, 2 time over-sampling

//------------------------------------------------------------------

short PAPR_RCFT_24t16d(short PAPR_mode, short Npts, short *x, short PowerThrsh, short *xbar);

#endif

