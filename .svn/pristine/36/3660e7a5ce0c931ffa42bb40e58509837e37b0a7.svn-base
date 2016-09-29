//-------------------------------------------------------------------------------------------
/** ATfile ms_rx_sync.h
 *
 * @brief PHY Mobile Station Receiver
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT&copy; 2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

/*
 *    ms_rx_sync.h
 */

#ifndef MS_RX_SYNC_H
#define MS_RX_SYNC_H

#include "typedef.h"

#define STRUCT64   1   // 0: use compiler defined __int64;  1: locally define 64-bit data type

#define N_CIC_STAGES      4
#define LEN_FIR          27
#define LEN_HBFIR        87  //45  // (LEN_HBFIR >> 1) should be an odd number
#define LEN_ZC           64
#define LEN_XCORR       256

#define LEN_MAX_CP     1024
#define LEN_EXTRA_CP     30           // Number of extra samples before CP

#define PHASE_THRSH    3000


#if STRUCT64

typedef struct {
    S32 H32;
    U32 L32;
} S64;

#else  //STRUCT64

typedef  __int64 S64;

#endif  //STRUCT64



#define LEN_GATEBUF        4096
#define LEN_GATEBUF_MASK   LEN_GATEBUF-1

typedef struct {
   S16 GateBuf[2*(LEN_GATEBUF)];
   S32 SegmentStartLoc;
   S32 NewLoc;

   S16 *pActiveGateBuf;
   S16 ActiveGate;

} LTE_MS_SYMBOLGATE;



#define MS_F_TRCK_WAIT          0
#define MS_F_TRCK_BUF_CP        1
#define MS_F_TRCK_SKIP_MID      2
#define MS_F_TRCK_BUF_SYMBEND   3
#define MS_F_TRCK_PHASE_EST     4
#define MS_F_TRCK_NEXT_SYMB     5


typedef struct {

   LTE_MS_SYMBOLGATE *pLte_ms_symbolgate;

   S32 Fs;                                 // Sampling frequency of the input data
   S16 State;
   S16 Symbol_l;
   S16 Symbol_Length;
   S16 len_cp0;
   S16 len_cp1;
   S16 len_cp;
   S16 EstimatedPhase;
   S16 Input_Idx;                          // Idx to the input samples IQin[]
   S16 CP_Idx;
   S16 SymbEnd_Idx;
   S16 nSample_Symb;                       // Sample count for the current symbol when collecting the data
   S16 Buf_0[2*(2*LEN_EXTRA_CP+LEN_MAX_CP)];
   S16 Buf_1[2*(2*LEN_EXTRA_CP+LEN_MAX_CP)];
   S16 *pBuf_CP;
   S16 *pBuf_SymbEnd;

   S16 Kp;
   S16 Ki;
   S16 Kd;
   S32 LF_y1;
   S16 PhaseErr;

   S16 Symbol_IQ_Ready;
   S16 Current_l;
   S16 *pSymbol_IQ;

} LTE_MS_FREQ_TRACK;


typedef struct {

   LTE_MS_FREQ_TRACK *pLte_ms_freqtracking;
   LTE_MS_SYMBOLGATE *pLte_ms_symbolgate;

   S32 Fs;                              // Sampling frequency of the input data
   S32 FsDesired;                       // Desired sampling freq at decimator output. E.g 15000*64 = 960000

   S16 NptsIn;                          // Number of IQ samples (in IQin[]) to be processed
   U32 InSampleCount;                   // Keep track of the input sample number
   U32 SegmentStartCount;               // Current data segment starts at InSampleCount
   U32 SegmentNumber;                   // Input data segment number

   S16 CIC_R;                           // Decimation factor of the CIC filter
   S16 CIC_Comb_M;                      // M value of the comb filter in the CIC filter. Should be 1 or 2.
   S16 CIC_Clock;                       // CIC clock
   S16 CIC_Bits_Gain;                   // Number of bits gained by CIC filter
   S64 CIC_real_si[N_CIC_STAGES+1];     // Delay elements of the integrators (real part)
   S64 CIC_real_sc[N_CIC_STAGES+1][2];  // Delay elements of the combs (real part)
   S64 CIC_imag_si[N_CIC_STAGES+1];     // Delay elements of the integrators (imag part)
   S64 CIC_imag_sc[N_CIC_STAGES+1][2];  // Delay elements of the combs (imag part)
   S16 DecimatorDelay;                  // Number of input samples delay by the decimator
   U32 CIC_out_n;                       // CIC output count

   S16 FIR_Clock;                       // FIR clock
   S16 FIR_Down;                        // Down sampling factor for the FIR
   S16 HBFIR_Clock;                     // HBFIR clock
   S16 HBFIR_Down;                      // Down sampling factor for the HBFIR
   S16 FIR_real_in;                     // FIR input sample
   S16 FIR_imag_in;
   S16 FIR_real_Buf[LEN_FIR];
   S16 FIR_imag_Buf[LEN_FIR];
   S16 FIR_real_out;                    // FIR output sample
   S16 FIR_imag_out;
   S16 HBFIR_real_Buf[LEN_HBFIR];
   S16 HBFIR_imag_Buf[LEN_HBFIR];
   S16 HBFIR_real_out;                  // HBFIR output sample
   S16 HBFIR_imag_out;
   U32 DEC_out_n;                       // Decimator output count

   S16 AGC_On_Off;                      // 1: ON;  0:OFF
   S16 AGC_Alpha;
   S16 AGC_Beta;
   S16 AGC_1_Beta;
   S16 AGC_Thrsh;
   S32 AGC_Gain32;
   S16 AGC_Gain16;
   S16 AGC_Shfl;
   S16 AGC_Eav;
   S32 AGC_Small_Thrsh;
   S16 AGC_real_out;                    // AGC output sample
   S16 AGC_imag_out;

   S16 xcorr_buf[2*LEN_XCORR];          // Circular buffer for correlation
   S16 xcorr_next_loc;                  // Index to xcorr_buf[] for putting next new sample
   U32 xc25maxloc;
   U32 xc29maxloc;
   U32 xc34maxloc;
   U16 xc25max;
   U16 xc29max;
   U16 xc34max;
   U16 xc25maxBufIdx;
   U16 xc29maxBufIdx;
   U16 xc34maxBufIdx;

   S16 PSyncFound;                      // This variable will be set to 1 if the primary sync is found
   S16 PSyncBufIdx;                     // index to the sample #63
   U32 PSyncDEC_out_n;
   S16 NID_1;
   S16 NID_2;
   S16 Subframe;
   S16 NID_cell;
   S16 SymbolStartingIdx;               // Offset to the current input data segment where the next symbol start
                                        // when PSync and SSync are comfirmed.

   S16 SSyncEst[LEN_ZC];

} LTE_MS_SYNC;


void Lte_ms_sync_search_init( LTE_MS_SYNC *pLte_ms_sync, LTE_MS_FREQ_TRACK *pLte_ms_freqtracking,
                              LTE_MS_SYMBOLGATE *pLte_ms_symbolgate, S32 Fs );
void Lte_ms_symb_search( S16 nIQin, S16 *IQin, LTE_MS_SYNC *pLte_ms_sync, S16 *pnSymbol_IQ, S16 *pSymbol_IQ,
                         S16 *pSymbol_l );
void Lte_ms_sync_search( S16 nIQin, S16 *IQin, LTE_MS_SYNC *pLte_ms_sync );


#if STRUCT64
// #include "struct64.h"
void Assign32to64( S64 *p64, S32 v32 );
void Assign64( S64 *p64, S32 H, U32 L );
void Add64( S64 *pC64, S64 *pA64, S64 *pB64 );
void Sub64( S64 *pC64, S64 *pA64, S64 *pB64 );
void Shr64( S64 *pC64, S64 *pA64, S16 nbits );
S32 GetL32( S64 *pC64 );

#endif

#endif
