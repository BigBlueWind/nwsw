//-------------------------------------------------------------------------------------------
/** @file bs_rx_prach.c
 *
 * @brief LTE PHY Base Station Receiver
 * @author Mindspeed Technologies
 * @version $Revision: 1.48.2.1.6.1 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#if !defined _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "typedef.h"

#include "bs_rx_prach.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#include "global.h"
#define CEVA_BUILD 1
#else
#include "basic_op_cc.h"
#define CEVA_BUILD 0
#endif

#define PRACH_USE_4096FFT 0
#define PRACH_USE_2048FFT 1
#define PRACH_BER_TEST    0
#define PRACH_IFFT_SIZE 2048

#define Debug_data_print 0
#define Debug_PAPR_print 0
#define Debug_Timing_Est_print 0

#if 1

#define PRACH_SETUP_MEMORY 1   // Memory is setup in the function PRACH_Rx()
//S16 PrachScratch[38384];

S16 *PrachFFTin; // = (S16 *) &(PrachScratch[0]);    //[8192+32];
S16 *SegmBuf; // = (S16 *) &(PrachScratch[8224]);    //[2*4096];            // will also be used as 4096 FFT output
S16 *eVecExt; // = (S16 *) &(PrachScratch[16416]);   //[2*4096];  [2*(SEGMSIZE+EXTDSIZE)];
S16 *CorrIn; // = (S16 *) &(PrachScratch[24608]);    //[4*NZC2];
S16 *CorrInExp; // = (S16 *) &(PrachScratch[31328]); //[4];

PBS_PRACH pbs_prach; // = (PBS_PRACH) &(PrachScratch[31332]);  // sizeof(BS_PRACH) ~ 300*2 bytes

// pPRACH_Tables = (S16 *) &(PrachScratch[31632]);  // Table size = 6720 *2 bytes

// PrachScratch size [38352]+32 (for FIR coeff and invTab)

S16 *eVec; // = (S16 *) &(PrachScratch[16416+2*EXTDSIZE]);   //&(eVecExt[2*EXTDSIZE]);
S16 *zEst; // = (S16 *) &(PrachScratch[8224]);    //SegmBuf;
S16 *DownSamplingFIR;

//S32 *temp_buffer;

#else

S16 PrachFFTin[8192+32];
S16 SegmBuf[2*4096]; // will also be used as 4096 FFT output
S16 eVecExt[2*(SEGMSIZE+EXTDSIZE)];
S16 CorrIn[4*NZC2];
S16 CorrInExp[4];

BS_PRACH bs_prach;
PBS_PRACH pbs_prach = &bs_prach;

S16 *eVec = &(eVecExt[2*EXTDSIZE]);
S16 *zEst = SegmBuf;
S16 *DownSamplingFIR;

#endif

// Tables for PRACH
//
extern S16 PRACH_Tables[];
S16 *ZC839Base; //[3356];
S16 *ZC839DFTk0; //[1678];
S16 *ZC839MultInv; //[839];
S16 *ZC839a1; //[839];

#ifdef CEVA_INTRINSICS

extern U8 *DataBuff_Bank1;

void FindPowerPeak_A(S16 startPoint, S16 endPoint, S32 *pPkPower, S16 *pPkLoc, S32 *p) __attribute__ ((section (".CSECT cevaboot")));
void PRACH_Rx_Demod_PartOne_A(S32 j0, S32 j1, S32 i, S16 *pSegmBuf,S16 *expTable, S16 *pOut) __attribute__ ((section (".CSECT cevaboot")));
void Prach_DownSampling_A(BS_PRACH *pbs_prach, S16 *pInBuf, S16 *pOut) __attribute__ ((section (".CSECT cevaboot")));
S32 AccPowerLevel_A(BS_PRACH *pbs_prach, S32 *pPowerLevel, S16 *pIFFTout, S16 Npt) __attribute__ ((section (".CSECT cevaboot")));
void Gen_ZC839DFT_A( S16 u, S16 *xuDFT ) __attribute__ ((section (".CSECT cevaboot")));
void ComputeComplexConjMult_A(S16 Vec1[], S16 Vec2[], S16 VecOut[], S16 length_krange) __attribute__ ((section (".CSECT cevaboot")));
void cfft_16t32d( short *xtime_real_img, short m, short *xfreq_real_img, short *Exponent2 ) __attribute__ ((section (".CSECT cevaboot")));
void cifft_16t32d( short *xtime_real_img, short m, short *xfreq_real_img, short *Exponent2 ) __attribute__ ((section (".CSECT cevaboot")));
void Prach_DownSampling5MHz_A(BS_PRACH *pbs_prach, S16 *pInBuf, S16 *pOut) __attribute__ ((section (".CSECT cevaboot")));
void prach_HBFilter_A( S16 *pD3, S16 *pF_out, S16 Npt, S16 Len_HBFIR, S16 *pF ) __attribute__ ((section (".CSECT cevaboot")));
void NormCorrIn_A(S16 MO, S16 *CorrInExp, S16 *CorrIn) __attribute__ ((section (".CSECT cevaboot")));
void TotalPowerComb_A(S32 **AntPower,  S32 *pPowerLevel) __attribute__ ((section (".CSECT cevaboot")));

// to save code size
void PRACH_Rx_Demod_init(S16 *step_size, S16 *pOut) __attribute__ ((section (".CSECT cevaboot")));
void CombinePower(S16 MO, S32 **AntPower, S32 * pBestAntInfo, S32 * averagePower, S16 *powerSearchWin) __attribute__ ((section (".CSECT cevaboot")));
void PrachPeakSearching(BS_PRACH *pbs_prach, S16 preambleCnt, S16 searchWin, S16 iseq, S32 *pPowerLevel,
                              S32 * pBestAntInfo, S16 *powerSearchWin,
                              U16 prachDetThreshold) __attribute__ ((section (".CSECT cevaboot")));
void PRACH_Rx_Demod(BS_PRACH *pbs_prach, S16 *pSegmBuf, S16 *expTable, S16 *pOut) __attribute__ ((section (".CSECT cevaboot")));

void Prach_DownSampling_NF_33_A(S16 DSR, S32 n2, S16 *pInBuf, S16 *pOut, S16 *pSrc ,S16 *pDst) __attribute__ ((section (".CSECT cevaboot")));

void PRACH_Rx_1(BS_PRACH *pbs_prach, S16 *pAdcBuf) __attribute__ ((section (".CSECT cevaboot")));

#ifdef EVM
extern void MemCpyFromExt(void* dst, void* src, unsigned long size);
#endif

#else

void cfft_16t32d_case2(short *xtime_real_img, short m, short *xfreq_real_img, short *Exponent2);
void cifft_16t32d_case2_2(short *xtime_real_img, short m, short *xfreq_real_img, short *Exponent2);

//void cfft_16t32d_case2(short *xtime_real_img, short m, short *xfreq_real_img, short *Exponent2,  long * temp_buffer);
//void cifft_16t32d_case2_2(short *xtime_real_img, short m, short *xfreq_real_img, short *Exponent2, long *temp_buffer);

#endif


//-----------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
/** @brief PRACH demodulation initializion, generate exponential table for demodulation
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param step_size       (INPUT) demodulation step size for each sample. <br/>
 *                          <br/>
 *  @param pOut            (OUTPUT STREAM) pointer to an array for storing exponential table for the following demodulation. <br/>
 *                         Upon return, this is an array of complex numbers, arranged as real part and
 *                         img part interleave together, i.e pOut[0].real, pOut[0].img, pOut[1].real,
 *                         pOut[1].img, ... <br/>
 *                         The output data is in Q15 format. <br/>
 *                          <br/>
 *
 *  @return                None.
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------
 *  |Name       |TCB Type      |Size       |Data Format |Valid Range |Default |Units |
 *  ----------------------------------------------------------------------------------
 *  |step_size  |Input         |1          |S16         |            |        |      |
 *  ----------------------------------------------------------------------------------
 *  |pOut       |Output Stream |2*256      |S16*        |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          | PRACH_Rx_Demod_init             |
 *  --------------------------------------------------------
 *  |Config Parameters   | step_size                       |
 *  --------------------------------------------------------
 *  |Output Stream       | pOut                            |
 *  --------------------------------------------------------
 *  |Status Parameters   | none                            |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *   Generate exponentail table for the demodulation module.
 *
 * <5> Cycle Count Formula
 *  5.3 cycles per complex input sample <br/>
 *  <br/>
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
//-------------------------------------------------------------------------------------------
void PRACH_Rx_Demod_init(S16 *step_size, S16 *pOut)
{
    S16 step_size_short;
    S16 index;
    S16 i, a, b;

    //step_size = pbs_prach->demod_step_size;

    step_size_short = shl(*step_size,4);

    index = 0;

    for (i = 0; i < 256; i++)
    {
        index = index + step_size_short; // intend to overflow, not saturation

        index &= 0xfff;

        if(index >= 2048)
        {
            index &= 0x7ff;
            a = negate(Table_FFT16b_4096[2 * index]);  // table is exp(-jx) from 0 to pi
            b = negate(Table_FFT16b_4096[2 * index + 1]);
            index = index + 2048;
        }
        else
        {
            a = (Table_FFT16b_4096[2 * index]);
            b = (Table_FFT16b_4096[2 * index + 1]);
        }


        pOut[2 * i] = a;
        pOut[2 * i + 1] = b;

    }
}
//-------------------------------------------------------------------------------------------

#ifndef CEVA_INTRINSICS
//-------------------------------------------------------------------------------------------

void PRACH_Rx_Demod_PartOne(S32 j0, S32 j1, S32 i, S16 *pSegmBuf,S16 *expTable, S16 *pOut)
{
    S32 j;
    S32 tmp0, tmp1;
    S16 y0, y1, a, b;
    S32 index;

    for (j = j0; j < j1; j++)
    {
        for (index = 0; index < 256; index++)
        {
            // exp
            a = (expTable[2 * index]);
            b = (expTable[2 * index + 1]);

            //complex multiplication
            tmp0 = L_sub(L_mult(a, pSegmBuf[2 * i]), L_mult(b, pSegmBuf[2 * i + 1]));
            tmp1 = L_add(L_mult(a, pSegmBuf[2 * i + 1]), L_mult(b, pSegmBuf[2 * i]));
            y0 = extract_h(tmp0);
            y1 = extract_h(tmp1);
            pOut[2 * i] = y0; // input and output buffers can be the same.
            pOut[2 * i + 1] = y1;

            i++;
        }

    }
}
#endif  // CEVA_INTRINSICS
//-------------------------------------------------------------------------------------------
/** @brief PRACH demodulation, convert PRACH passband signal to baseband signal
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param pbs_prach       (INPUT) pointer to the structure BS_PRACH. <br/>
 *                          <br/>
 *  @param pSegmBuf        (INPUT STREAM) pointer to the array to be processed <br/>
 *                         This is an array of complex numbers, arranged as real part and img part
 *                         interleave together, i.e pSegmBuf[0].real, pSegmBuf[0].img,
 *                         pSegmBuf[1].real, pSegmBuf[1].img, ...
 *                         The size of this array should be 2*SEGMSIZE. Each 16-bit number in
 *                         this arrayis in Q15 format. The constant SEGMSIZE is defined in the
 *                         file bs_rx_prach.h. <br/>
 *                          <br/>
 *  @param expTable        (INPUT STREAM) pointer to the array which stores the exponential table. <br/>
 *                         This is an array of complex numbers, arranged as real part and img part
 *                         interleave together, i.e expTable[0].real, expTable[0].img,
 *                         expTable[1].real, expTable[1].img, ...
 *                         The size of this array should be 2*256. Each 16-bit number in
 *                         this arrayis in Q15 format.<br/>
 *                          <br/>
 *  @param pOut            (OUTPUT STREAM) pointer to an array for storing processed results. <br/>
 *                         Upon return, this is an array of complex numbers, arranged as real part and
 *                         img part interleave together, i.e pOut[0].real, pOut[0].img, pOut[1].real,
 *                         pOut[1].img, ... <br/>
 *                         At least 2*SEGMSIZE*sizeof(short) bytes should be allocated to
 *                         this pointer before calling this function.
 *                         The output data is in Q15 format. <br/>
 *                          <br/>
 *
 *  @return                None.
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------
 *  |Name       |TCB Type      |Size       |Data Format |Valid Range |Default |Units |
 *  ----------------------------------------------------------------------------------
 *  |pbs_prach  |Input         |1          |BS_PRACH    |            |        |      |
 *  ----------------------------------------------------------------------------------
 *  |pSegmBuf   |Input Stream  |2*SEGMSIZE |S16*        |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  |expTable   |Input Stream  |2*256      |S16*        |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  |pOut       |Output Stream |2*SEGMSIZE |S16*        |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          | PRACH_Rx_Demod                  |
 *  --------------------------------------------------------
 *  |Inputs Stream       | pSegmBuf                        |
 *  --------------------------------------------------------
 *  |Inputs Stream       | expTable                        |
 *  --------------------------------------------------------
 *  |Output Stream       | pOut                            |
 *  --------------------------------------------------------
 *  |Config Parameters   | pbs_prach                       |
 *  --------------------------------------------------------
 *  |Status Parameters   | none                            |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *   Frequency shift PRACH preamble to baseband/demod (including minus-half-subcarrier shift).
 *
 * <5> Cycle Count Formula
 *  1 cycles per complex input sample <br/>
 *  <br/>
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 * \ingroup group_lte_rx_uplink
 *
 **/

void PRACH_Rx_Demod(BS_PRACH *pbs_prach, S16 *pSegmBuf, S16 *expTable, S16 *pOut)
{
    S16 i = 0, j0 = 0, j1 = 0;

#if Debug_data_print
    {
        FILE *fp,*fp2, *fp3;
        char Prach_FileName[64];
        char seg_char[8],ant_char[8],fft_char[8];
        S16   N;
        N = pbs_prach->Nseq;

        sprintf(seg_char, "%d", pbs_prach->iSegm);
        sprintf(ant_char, "%d", pbs_prach->iAntenna);
        sprintf(fft_char, "%d", N);

        strcpy(Prach_FileName, "Prach_raw_data_in");
        strcat(Prach_FileName, "_");
        strcat(Prach_FileName, (const char *) (&fft_char));
        strcat(Prach_FileName, "_");
        strcat(Prach_FileName, (const char *) (&ant_char));
        strcat(Prach_FileName, "_");
        strcat(Prach_FileName, (const char *) (&seg_char));
        strcat(Prach_FileName, ".dat");

        //fp2 = (FILE *) fopen(Prach_FileName, "at+");
        fp2 = (FILE *) fopen("Prach_demod_input", "at+");

        for ( i=0; i < 3840; i++ )
        {
            //fprintf( fp2, "%1.15f\n %1.15f\n",  pSegmBuf[2 * i]/2^15, pSegmBuf[2 * i+1]/2^15 );
            fprintf( fp2, "%d %d\n",  pSegmBuf[2 * i], pSegmBuf[2 * i+1] );
        }

        fclose(fp2);

        fp3 = (FILE *) fopen("Prach_demod_output", "at+");

        for ( i=0; i < 3840; i++ )
        {
            fprintf( fp3, "%d %d\n",  pOut[2 * i], pOut[2 * i+1] );
        }

        fclose(fp3);

        if (pbs_prach->iAntenna == 0)
        {
            if(pbs_prach->iSegm == 0)
            {
                fp = (FILE *) fopen( "data_before_demod1", "w");
            }
            else if (pbs_prach->iSegm == 1)
            {
                fp = (FILE *) fopen( "data_before_demod2", "w");
            }
            else if (pbs_prach->iSegm == 2)
            {
                fp = (FILE *) fopen( "data_before_demod3", "w");
            }
            else if (pbs_prach->iSegm == 3)
            {
                fp = (FILE *) fopen( "data_before_demod4", "w");
            }
            else if (pbs_prach->iSegm == 4)
            {
                fp = (FILE *) fopen( "data_before_demod5", "w");
            }
            else if (pbs_prach->iSegm == 5)
            {
                fp = (FILE *) fopen( "data_before_demod6", "w");
            }
            else if (pbs_prach->iSegm == 6)
            {
                fp = (FILE *) fopen( "data_before_demod7", "w");
            }
            else if (pbs_prach->iSegm == 7)
            {
                fp = (FILE *) fopen( "data_before_demod8", "w");
            }
        }
        if (pbs_prach->iAntenna == 1)
        {
            if(pbs_prach->iSegm == 0)
            {
                fp = (FILE *) fopen( "data_before_demod11", "w");
            }
            else if (pbs_prach->iSegm == 1)
            {
                fp = (FILE *) fopen( "data_before_demod12", "w");
            }
            else if (pbs_prach->iSegm == 2)
            {
                fp = (FILE *) fopen( "data_before_demod13", "w");
            }
            else if (pbs_prach->iSegm == 3)
            {
                fp = (FILE *) fopen( "data_before_demod14", "w");
            }
            else if (pbs_prach->iSegm == 4)
            {
                fp = (FILE *) fopen( "data_before_demod15", "w");
            }
            else if (pbs_prach->iSegm == 5)
            {
                fp = (FILE *) fopen( "data_before_demod16", "w");
            }
            else if (pbs_prach->iSegm == 6)
            {
                fp = (FILE *) fopen( "data_before_demod17", "w");
            }
            else if (pbs_prach->iSegm == 7)
            {
                fp = (FILE *) fopen( "data_before_demod18", "w");
            }
        }

        for ( i=0; i < 3840; i++ )
        {
            fprintf( fp, "%d %d\n",  pSegmBuf[2 * i], pSegmBuf[2 * i+1] );
        }

        fclose(fp);

    }
#endif


        // don't do CP; starting point
    if (pbs_prach->iSegm == 0)
    {
        switch(pbs_prach->nSegm)
        {
            case 8: j0 = 12;i = 3072; break;   // 20 Mhz
            case 4: j0 = 6;i = 1536; break;   // 10 Mhz
            case 2: j0 = 3; i = 768; break;   // 5 Mhz
            case 1: j0 = 0; i = 0; break;
            case 0: j0 = 0; i = 0; break;
        }
    }
    else
    {
        j0 = 0;
        i = 0;
    }
    // don't do GT; ending point
    if ((pbs_prach->iSegm == (pbs_prach->nSegm - 1)) && (pbs_prach->iSegm > 0)) //last segment
    {
        switch(pbs_prach->nSegm)
        {
            case 8: j1 = 4; break;   // 20 Mhz
            //case 8: j1 = 15; break;   // 20 Mhz
            case 4: j1 = 10; break;   // 10 Mhz
            case 2: j1 = 13; break;   // 5 Mhz
            case 1: j1 = 15; break;
            case 0: j1 = 15; break;
        }
    }
    else
    {
        j1 = 15;
    }

    //j1 = 15;
    // apply downconvert to input data; 15*256 = 3840
#ifdef CEVA_INTRINSICS
    PRACH_Rx_Demod_PartOne_A(j0, j1, i,  pSegmBuf,  expTable, pOut);
#else
    PRACH_Rx_Demod_PartOne(j0, j1, i,  pSegmBuf,  expTable, pOut);
#endif

}

//-----------------------------------------------------------------------

//-----------------------------------------------------------------------

#ifndef CEVA_INTRINSICS

//-------------------------------------------------------------------------------------------
/** @brief PRACH down-sampling for 20 Mhz, 10 Mhz and 5 Mhz channels, to reduce the number of samples in PRACH preamble detection
 *
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param pbs_prach       (INPUT) pointer to the structure BS_PRACH. <br/>
 *                          <br/>
 *  @param pInBuf          (INPUT STREAM) pointer to the array to be processed <br/>
 *                         This is an array of complex numbers, arranged as real part and img part
 *                         interleave together, i.e pInBuf[0].real, pInBuf[0].img,
 *                         pInBuf[1].real, pInBuf[1].img, ...
 *                         The size of this array should be 2*SEGMSIZE. Each 16-bit number in
 *                         this arrayis in Q15 format. The constant SEGMSIZE is defined in the
 *                         file bs_rx_prach.h. <br/>
 *                          <br/>
 *  @param pOut            (OUTPUT STREAM) pointer to an array for storing processed results. <br/>
 *                         Upon return, this is an array of complex numbers, arranged as real part and
 *                         img part interleave together, i.e pOut[0].real, pOut[0].img, pOut[1].real,
 *                         pOut[1].img, ... <br/>
 *                         At least 2*4096*sizeof(short) bytes should be allocated to
 *                         this pointer before calling this function.
 *                         The output data is in Q15 format. <br/>
 *                          <br/>
 *
 *  @return                None.
 *
 *
 * <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------
 *  |Name       |TCB Type      |Size       |Data Format |Valid Range |Default |Units |
 *  ----------------------------------------------------------------------------------
 *  |pbs_prach  |Input         |1          |BS_PRACH    |            |        |      |
 *  ----------------------------------------------------------------------------------
 *  |pInBuf     |Input Stream  |2*SEGMSIZE |S16*        |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  |pOut       |Output Stream |2*4096     |S16*        |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  TABLE_END
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          | Prach_DownSampling              |
 *  --------------------------------------------------------
 *  |Inputs Stream       | pInBuf                          |
 *  --------------------------------------------------------
 *  |Output Stream       | pOut                            |
 *  --------------------------------------------------------
 *  |Config Parameters   | pbs_prach                       |
 *  --------------------------------------------------------
 *  |Status Parameters   | none                            |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 * <4> Detailed Description:
 *
 *   This function reduces the number of samples to be processed in PRACH preamble detection.
 *   The down-sampling factor is determined by the input data size and the targeted FFT size (Nfft).
 *   The down-sampling is accomplished by a low-pass FIR filter.
 *
 * <5> Cycle Count Formula
 *  Nfft * FIR_Length * 2 / 4 * 1.1 <br/>
 *  The factor 1.1 is for estimated 10% overhead.
 *  <br/>
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 * \ingroup group_lte_rx_uplink
 **/
//-------------------------------------------------------------------------------------------
void Prach_DownSampling(BS_PRACH *pbs_prach, S16 *pInBuf, S16 *pOut)
{
    S32 n, k, n0, n1, nsub;
    S32 L_temp1, L_temp2;
    S16 DSR; //, delay;
    nsub = pbs_prach->IdxDown;
    if (pbs_prach->iSegm == 0)
    {
        // delay = pbs_prach->lpfDelay;
        DSR = pbs_prach->DSR;
        // n0 = (delay % DSR) + (divide_floor(delay, DSR) + divide_floor(pbs_prach->Ncp, DSR)) * DSR;
        n0 = pbs_prach->nStart;
        n1 = pbs_prach->SegmSize;
    }
    else
    {
        DSR = pbs_prach->DSR;
        n0 = pbs_prach->nStart;
        n1 = pbs_prach->SegmSize;
        k = (1024 * pbs_prach->nSegm + 100 - nsub) * 3;
        if (n1 > k)
        {
            n1 = k;
        }
    }

    for (n = n0; n < n1; n += DSR)
    {
        L_temp1 = 0x00008000;
        L_temp2 = 0x00008000;
        //complex samples * real filter
        for (k = 0; k < pbs_prach->Nf; k++)
        {
            L_temp1 = L_mac(L_temp1, pInBuf[2 * (n - k)], DownSamplingFIR[k]);
            L_temp2 = L_mac(L_temp2, pInBuf[2 * (n - k) + 1], DownSamplingFIR[k]);
        }
        pOut[2 * nsub] = extract_h(L_temp1);
        pOut[2 * nsub + 1] = extract_h(L_temp2);
        nsub += 1;
    }
    pbs_prach->IdxDown = (S16) nsub;
    // pbs_prach->nStart = (S16) (n - pbs_prach->SegmSize);
    pbs_prach->nStart = (S16) (n - n1);

    // Save the last Nf samples
    //
    n0 = 2 * pbs_prach->SegmSize;
    for (k = 1; k < 2 * pbs_prach->Nf; k++)
    {
        pInBuf[-k] = pInBuf[n0 - k];
    }
}
#endif  // CEVA_INTRINSICS



void Prach_DownSampling_NF_33(BS_PRACH *pbs_prach, S16 *pInBuf, S16 *pOut)
{
    S32 n, k, n0, n1, nsub;
    S32 L_temp1, L_temp2;
    S16 DSR; //, delay;
    nsub = pbs_prach->IdxDown;
    if (pbs_prach->iSegm == 0)
    {
        // delay = pbs_prach->lpfDelay;
        DSR = pbs_prach->DSR;
        // n0 = (delay % DSR) + (divide_floor(delay, DSR) + divide_floor(pbs_prach->Ncp, DSR)) * DSR;
        n0 = pbs_prach->nStart;
        n1 = pbs_prach->SegmSize;

    }
    else
    {
        DSR = pbs_prach->DSR;
        n0 = pbs_prach->nStart;
        n1 = pbs_prach->SegmSize;
        k = (1024 * pbs_prach->nSegm + 100 - nsub) * 3;
        if (n1 > k)
        {
            n1 = k;
        }
    }

#ifdef CEVA_INTRINSICS
    {
        S32 n2;

        DownSamplingFIR = prach_fir33_D3;
        n2 = (n1 + 2 - n0) / DSR;
        pbs_prach->IdxDown = (S16) nsub + n2;
        pbs_prach->nStart = (S16) (n0 + n2 * 3 - n1);
        Prach_DownSampling_NF_33_A(DSR, n2, &pInBuf[2 * (n0 + 1 - pbs_prach->Nf)],
                            &pOut[2 * nsub], &pInBuf[2 * (pbs_prach->SegmSize - pbs_prach->Nf) + 1],
                            &pInBuf[-2 * pbs_prach->Nf + 1]);
    }
#else
    for (n = n0; n < n1; n += DSR)
    {
        L_temp1 = 0x00008000;
        L_temp2 = 0x00008000;
        //complex samples * real filter
        for (k = 0; k < pbs_prach->Nf; k++)
        {
            L_temp1 = L_mac(L_temp1, pInBuf[2 * (n - k)], DownSamplingFIR[k]);
            L_temp2 = L_mac(L_temp2, pInBuf[2 * (n - k) + 1], DownSamplingFIR[k]);
        }
        pOut[2 * nsub] = extract_h(L_temp1);
        pOut[2 * nsub + 1] = extract_h(L_temp2);
        nsub += 1;
    }
    pbs_prach->IdxDown = (S16) nsub;
    // pbs_prach->nStart = (S16) (n - pbs_prach->SegmSize);
    pbs_prach->nStart = (S16) (n - n1);

    // Save the last Nf samples
    //
    n0 = 2 * pbs_prach->SegmSize;
    for (k = 1; k < 2 * pbs_prach->Nf; k++)
    {
        pInBuf[-k] = pInBuf[n0 - k];
    }
#endif
}

#ifndef CEVA_INTRINSICS
//-------------------------------------------------------------------------------------------
/** @brief PRACH down-sampling for 3 Mhz channels, to reduce the number of samples in PRACH preamble detection
 *
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param pbs_prach       (INPUT) pointer to the structure BS_PRACH. <br/>
 *                          <br/>
 *  @param pInBuf          (INPUT STREAM) pointer to the array to be processed <br/>
 *                         This is an array of complex numbers, arranged as real part and img part
 *                         interleave together, i.e pInBuf[0].real, pInBuf[0].img,
 *                         pInBuf[1].real, pInBuf[1].img, ...
 *                         The size of this array should be 2*SEGMSIZE. Each 16-bit number in
 *                         this arrayis in Q15 format. The constant SEGMSIZE is defined in the
 *                         file bs_rx_prach.h. <br/>
 *                          <br/>
 *  @param pOut            (OUTPUT STREAM) pointer to an array for storing processed results. <br/>
 *                         Upon return, this is an array of complex numbers, arranged as real part and
 *                         img part interleave together, i.e pOut[0].real, pOut[0].img, pOut[1].real,
 *                         pOut[1].img, ... <br/>
 *                         At least 2*4096*sizeof(short) bytes should be allocated to
 *                         this pointer before calling this function.
 *                         The output data is in Q15 format. <br/>
 *                          <br/>
 *
 *  @return                None.
 *
 *
 * <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------
 *  |Name       |TCB Type      |Size       |Data Format |Valid Range |Default |Units |
 *  ----------------------------------------------------------------------------------
 *  |pbs_prach  |Input         |1          |BS_PRACH    |            |        |      |
 *  ----------------------------------------------------------------------------------
 *  |pInBuf     |Input Stream  |2*SEGMSIZE |S16*        |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  |pOut       |Output Stream |2*4096     |S16*        |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  TABLE_END
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          | Prach_DownSampling              |
 *  --------------------------------------------------------
 *  |Inputs Stream       | pInBuf                          |
 *  --------------------------------------------------------
 *  |Output Stream       | pOut                            |
 *  --------------------------------------------------------
 *  |Config Parameters   | pbs_prach                       |
 *  --------------------------------------------------------
 *  |Status Parameters   | none                            |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 * <4> Detailed Description:
 *
 *   This function reduces the number of samples to be processed in PRACH preamble detection.
 *   The down-sampling factor is determined by the input data size and the targeted FFT size (Nfft).
 *   The down-sampling is accomplished by a low-pass FIR filter.
 *
 * <5> Cycle Count Formula
 *  Nfft * FIR_Length * 2 / 4 * 1.1 <br/>
 *  The factor 1.1 is for estimated 10% overhead.
 *  <br/>
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 * \ingroup group_lte_rx_uplink
 **/
//-------------------------------------------------------------------------------------------
//void Prach_DownSampling3MHz(BS_PRACH *pbs_prach, S16 *pInBuf, S16 *pOut)
//{
//    S32 n, k, n0, n1, nsub;
//    S32 L_temp1, L_temp2;
//    S16 DSR; //, delay;
//    S32 nf;
//  S16 k1;
//
//    nsub = pbs_prach->IdxDown;
//
//        // delay = pbs_prach->lpfDelay;
//        DSR = pbs_prach->DSR;
//        // n0 = (delay % DSR) + (divide_floor(delay, DSR) + divide_floor(pbs_prach->Ncp, DSR)) * DSR;
//        n0 = pbs_prach->nStart;
//        n1 = pbs_prach->SegmSize;
//
//
//
//    // DownSamplingFIR ; for 3 MHz signal, first insert zeros to oversample by two, then pass a 1/3 decimation filter (we omit the 1/2 interpolation filter)
//    nf = pbs_prach->Nf;
//
//  // for points from non-zeros
//    for (n = n0; n < n1; n += DSR)
//    {
//        L_temp1 = 0x00008000;
//        L_temp2 = 0x00008000;
//
//        //complex samples * real filter
//      k1 = 24;  // 2*24+1 = filter length
//        for (k = 0; k < pbs_prach->Nf; k+=2)
//        {
//            L_temp1 = L_mac(L_temp1, pInBuf[2 * ((n - nf + 1) + k1)], DownSamplingFIR[k]);
//            L_temp2 = L_mac(L_temp2, pInBuf[2 * ((n - nf + 1) + k1) + 1], DownSamplingFIR[k]);
//          k1++;
//        }
//        pOut[2 * nsub] = extract_h(L_temp1);
//        pOut[2 * nsub + 1] = extract_h(L_temp2);
//        nsub += 2;
//    }
//
//      // for points starting from intersrted zeros
//  nsub = pbs_prach->IdxDown+1;
//    for (n = n0+1; n < n1; n += DSR)
//    {
//        L_temp1 = 0x00008000;
//        L_temp2 = 0x00008000;
//
//        //complex samples * real filter
//      k1 = 25;
//        for (k = 1; k < pbs_prach->Nf; k+=2)
//        {
//            L_temp1 = L_mac(L_temp1, pInBuf[2 * ((n - nf + 1) + k1)], DownSamplingFIR[k]);
//            L_temp2 = L_mac(L_temp2, pInBuf[2 * ((n - nf + 1) + k1) + 1], DownSamplingFIR[k]);
//          k1++;
//        }
//        pOut[2 * nsub] = extract_h(L_temp1);
//        pOut[2 * nsub + 1] = extract_h(L_temp2);
//        nsub += 2;
//    }
//}
#endif  // CEVA_INTRINSICS

#ifndef CEVA_INTRINSICS
//-------------------------------------------------------------------------------------------
/** @brief PRACH down-sampling for 1.4 Mhz channels, to reduce the number of samples in PRACH preamble detection
 *
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param pbs_prach       (INPUT) pointer to the structure BS_PRACH. <br/>
 *                          <br/>
 *  @param pInBuf          (INPUT STREAM) pointer to the array to be processed <br/>
 *                         This is an array of complex numbers, arranged as real part and img part
 *                         interleave together, i.e pInBuf[0].real, pInBuf[0].img,
 *                         pInBuf[1].real, pInBuf[1].img, ...
 *                         The size of this array should be 2*SEGMSIZE. Each 16-bit number in
 *                         this arrayis in Q15 format. The constant SEGMSIZE is defined in the
 *                         file bs_rx_prach.h. <br/>
 *                          <br/>
 *  @param pOut            (OUTPUT STREAM) pointer to an array for storing processed results. <br/>
 *                         Upon return, this is an array of complex numbers, arranged as real part and
 *                         img part interleave together, i.e pOut[0].real, pOut[0].img, pOut[1].real,
 *                         pOut[1].img, ... <br/>
 *                         At least 2*4096*sizeof(short) bytes should be allocated to
 *                         this pointer before calling this function.
 *                         The output data is in Q15 format. <br/>
 *                          <br/>
 *
 *  @return                None.
 *
 *
 * <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------
 *  |Name       |TCB Type      |Size       |Data Format |Valid Range |Default |Units |
 *  ----------------------------------------------------------------------------------
 *  |pbs_prach  |Input         |1          |BS_PRACH    |            |        |      |
 *  ----------------------------------------------------------------------------------
 *  |pInBuf     |Input Stream  |2*SEGMSIZE |S16*        |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  |pOut       |Output Stream |2*4096     |S16*        |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  TABLE_END
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          | Prach_DownSampling              |
 *  --------------------------------------------------------
 *  |Inputs Stream       | pInBuf                          |
 *  --------------------------------------------------------
 *  |Output Stream       | pOut                            |
 *  --------------------------------------------------------
 *  |Config Parameters   | pbs_prach                       |
 *  --------------------------------------------------------
 *  |Status Parameters   | none                            |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 * <4> Detailed Description:
 *
 *   This function reduces the number of samples to be processed in PRACH preamble detection.
 *   The down-sampling factor is determined by the input data size and the targeted FFT size (Nfft).
 *   The down-sampling is accomplished by a low-pass FIR filter.
 *
 * <5> Cycle Count Formula
 *  Nfft * FIR_Length * 2 / 4 * 1.1 <br/>
 *  The factor 1.1 is for estimated 10% overhead.
 *  <br/>
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 * \ingroup group_lte_rx_uplink
 **/
//-------------------------------------------------------------------------------------------
//void Prach_DownSampling1p4MHz(BS_PRACH *pbs_prach, S16 *pInBuf, S16 *pOut)
//{
//    S32 n, k, n0, n1, nsub;
//    S32 L_temp1, L_temp2;
//    S16 DSR; //, delay;
//    S32 nf;
//  S16 k1;
//
//    nsub = pbs_prach->IdxDown;
//
//        // delay = pbs_prach->lpfDelay;
//        DSR = pbs_prach->DSR;
//        // n0 = (delay % DSR) + (divide_floor(delay, DSR) + divide_floor(pbs_prach->Ncp, DSR)) * DSR;
//        n0 = pbs_prach->nStart;
//        n1 = pbs_prach->SegmSize;
//
//
//
//    // DownSamplingFIR ; for 1.4 MHz signal, first insert three zeros to oversample by four, should pass a 1/4 interpolation
//      // filter first and then  a 1/3 decimation filter; since, after inserting zeros, signal spectrum is repeated in frequency domain, not mixed together;
//      // and we only care about the baseband signal, hence, we don't need the interpolation filter; Only the 1/3 decimation filter is enough;
//    nf = pbs_prach->Nf;
//
//  // for points from non-zeros; filter length is 57 = 4*14+1
//    for (n = n0; n < n1; n += DSR)
//    {
//        L_temp1 = 0x00008000;
//        L_temp2 = 0x00008000;
//
//        //complex samples * real filter
//      k1 = 27;
//      //k1 = 48;
//        for (k = 0; k < pbs_prach->Nf; k+=4)
//        {
//            L_temp1 = L_mac(L_temp1, pInBuf[2 * ((n - nf + 1) + k1)], DownSamplingFIR[k]);
//            L_temp2 = L_mac(L_temp2, pInBuf[2 * ((n - nf + 1) + k1) + 1], DownSamplingFIR[k]);
//          k1++;
//        }
//        pOut[2 * nsub] = extract_h(L_temp1);
//        pOut[2 * nsub + 1] = extract_h(L_temp2);
//        nsub += 4;
//    }
//
//      // for points starting from the first intersted zeros
//  nsub = pbs_prach->IdxDown+3;
//    for (n = n0+1; n < n1; n += DSR)
//    {
//        L_temp1 = 0x00008000;
//        L_temp2 = 0x00008000;
//
//        //complex samples * real filter
//      //k1 = 50;
//      k1 = 29;
//        for (k = 3; k < pbs_prach->Nf; k+=4)
//        {
//            L_temp1 = L_mac(L_temp1, pInBuf[2 * ((n - nf + 1) + k1)], DownSamplingFIR[k]);
//            L_temp2 = L_mac(L_temp2, pInBuf[2 * ((n - nf + 1) + k1) + 1], DownSamplingFIR[k]);
//          k1++;
//        }
//        pOut[2 * nsub] = extract_h(L_temp1);
//        pOut[2 * nsub + 1] = extract_h(L_temp2);
//        nsub += 4;
//    }
//
//      // for points starting from the second intersted zeros
//  nsub = pbs_prach->IdxDown+2;
//    for (n = n0+1; n < n1; n += DSR)
//    {
//        L_temp1 = 0x00008000;
//        L_temp2 = 0x00008000;
//
//        //complex samples * real filter
//      //k1 = 49;
//      k1 = 28;
//        for (k = 2; k < pbs_prach->Nf; k+=4)
//        {
//            L_temp1 = L_mac(L_temp1, pInBuf[2 * ((n - nf + 1) + k1)], DownSamplingFIR[k]);
//            L_temp2 = L_mac(L_temp2, pInBuf[2 * ((n - nf + 1) + k1) + 1], DownSamplingFIR[k]);
//          k1++;
//        }
//        pOut[2 * nsub] = extract_h(L_temp1);
//        pOut[2 * nsub + 1] = extract_h(L_temp2);
//        nsub += 4;
//    }
//
//          // for points starting from the third intersted zeros
//  nsub = pbs_prach->IdxDown+1;
//    for (n = n0+1; n < n1; n += DSR)
//    {
//        L_temp1 = 0x00008000;
//        L_temp2 = 0x00008000;
//
//        //complex samples * real filter
//      //k1 = 48;
//      k1 = 27;
//        for (k = 1; k < pbs_prach->Nf; k+=4)
//        {
//            L_temp1 = L_mac(L_temp1, pInBuf[2 * ((n - nf + 1) + k1)], DownSamplingFIR[k]);
//            L_temp2 = L_mac(L_temp2, pInBuf[2 * ((n - nf + 1) + k1) + 1], DownSamplingFIR[k]);
//          k1++;
//        }
//        pOut[2 * nsub] = extract_h(L_temp1);
//        pOut[2 * nsub + 1] = extract_h(L_temp2);
//        nsub += 4;
//    }
//}


#endif  // CEVA_INTRINSICS
//-----------------------------------------------------------------------


#ifndef CEVA_INTRINSICS

//-------------------------------------------------------------------------------------------
/** @brief Generating a Zadoff-Chu sequence in frequency domain.
 *
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param u               (INPUT) The desired root sequence number. <br/>
 *                          <br/>
 *  @param xuDFT           (OUTPUT) pointer to an array for storing results. <br/>
 *                         Upon return, this is an array of complex numbers, arranged as real part and
 *                         img part interleave together, i.e xuDFT[0].real, xuDFT[0].img, xuDFT[1].real,
 *                         xuDFT[1].img, ... <br/>
 *                         At least 2*839*sizeof(short) bytes should be allocated to
 *                         this pointer before calling this function.
 *                         The output data is in Q15 format. <br/>
 *                          <br/>
 *
 *  @return                None.
 *
 *
 * <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------
 *  |Name       |TCB Type      |Size       |Data Format |Valid Range |Default |Units |
 *  ----------------------------------------------------------------------------------
 *  |u          |Input         |1          |S16         |[0..838]    |        |      |
 *  ----------------------------------------------------------------------------------
 *  |xuDFT      |Output        |2*839      |S16*        |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          | Gen_ZC839DFT                    |
 *  --------------------------------------------------------
 *  |Inputs Stream       | None                            |
 *  --------------------------------------------------------
 *  |Output Stream       | xuDFT                           |
 *  --------------------------------------------------------
 *  |Config Parameters   | u                               |
 *  --------------------------------------------------------
 *  |Status Parameters   | none                            |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *   This function generates a Zadoff-Chu sequence in frequency domain directly without
 *   using 839-point DFT.
 *
 *
 * <5> Cycle Count Formula
 *  2900 cycles for a Zadoff-Chu sequence of length 839<br/>
 *  <br/>
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
//-------------------------------------------------------------------------------------------
void Gen_ZC839DFT(S16 u, S16 *xuDFT)
{
    int k, a; //, u2, da;
    //int uhatk;
    S16 uhat, Xuk0r, Xuk0i;
    S16 x, y;
    long tmp1, tmp2;
    long b, c, bk;

    uhat = ZC839MultInv[u];

    //u2 = u + u;
    //da = uhat + 1;
    a = 0;
    //uhatk = 0;
    b = (2 * (long) u * (long) uhat * (long) uhat) % NZC2;
    c = ((long) u * (long) uhat * (long) (uhat + 1)) % NZC2;
    for (k = 0; k < NZC; k++)
    {
#if 1
        xuDFT[2 * k] = ZC839Base[2 * a];
        xuDFT[2 * k + 1] = ZC839Base[2 * a + 1];

        // Next a
        //
        bk = (b * k) % NZC2;
        a = (a + bk + c) % NZC2;

#else

        a = u * uhat*k * (uhat*k + 1 );
        a = a % NZC2;
        xuDFT[ 2*k ] = ZC839Base[ 2*a ];
        xuDFT[ 2*k+1 ] = ZC839Base[ 2*a+1 ];

#endif

    }

    // Xu(0) * conjg( xuDFT(k) )
    //
    Xuk0r = ZC839DFTk0[2 * u];
    Xuk0i = ZC839DFTk0[2 * u + 1];
    xuDFT[0] = Xuk0r;
    xuDFT[1] = Xuk0i;
    for (k = 1; k < NZC; k++)
    {
        x = xuDFT[2 * k];
        y = xuDFT[2 * k + 1];

        tmp1 = L_mult(x, Xuk0r);
        // xuDFT[ 2*k ] = mac_r( tmp1, y, Xuk0i );
        tmp1 = L_mac(tmp1, y, Xuk0i);
        xuDFT[2 * k] = extract_h(tmp1 + 0x00008000);

        tmp2 = L_mult(x, Xuk0i);
        // xuDFT[ 2*k+1 ] = msu_r( tmp2, y, Xuk0r );
        tmp2 = L_msu(tmp2, y, Xuk0r);
        xuDFT[2 * k + 1] = extract_h(tmp2 + 0x00008000);

#if 0 // 1: check overflow
        // Check overflow

        {
            int ovf;

            ovf = 0;
            tmp1 = L_mult( x, Xuk0r );
            tmp1 = L_shr( tmp1, 16);
            tmp2 = L_mult( y, Xuk0i );
            tmp2 = L_shr( tmp2, 16);
            tmp2 = L_add( tmp1, tmp2 );
            if ( tmp2 >= 0 )
            {
                if ( tmp2 > 32767 ) ovf = 1;
            }
            else
            {
                if ( tmp2 < -32768 ) ovf = 1;
            }
            if (ovf == 1 )
            {
                printf("overflowed: (real)  u=%d    k=%d   tmp2= %d\n", u, k, tmp2);
            }

            ovf = 0;
            tmp1 = L_mult( x, Xuk0i );
            tmp1 = L_shr( tmp1, 16);
            tmp2 = L_mult( y, Xuk0r );
            tmp2 = L_shr( tmp2, 16);
            tmp2 = L_sub( tmp1, tmp2 );
            if ( tmp2 >= 0 )
            {
                if ( tmp2 > 32767 ) ovf = 1;
            }
            else
            {
                if ( tmp2 < -32768 ) ovf = 1;
            }
            if (ovf == 1 )
            {
                printf("overflowed: (imag)  u=%d    k=%d   tmp2= %d\n", u, k, tmp2);
            }

        }
#endif
    }

#if Debug_data_print
     // Uncomment this block if saving the data to the file xuDFT.out
     //
     {
     FILE *fp;

     fp = (FILE *) fopen( "xuDFT_out.dat", "w");
    // fprintf( fp, "u = %d\n",  u );
     for ( k=0; k < NZC; k++ )
     {
     fprintf( fp, "%6d %6d\n",  xuDFT[2*k], xuDFT[2*k+1] );
     }
     fclose(fp);
     }
#endif

}


#endif  // CEVA_INTRINSICS

//-----------------------------------------------------------------------


#ifndef CEVA_INTRINSICS

//-------------------------------------------------------------------------------------------
/** @brief PRACH half-band filter down-sampling by 2.
 *
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param pD3             (INPUT STREAM) pointer to the array to be processed <br/>
 *                         This is an array of complex numbers, arranged as real part and img part
 *                         interleave together, i.e pD3[0].real, pD3[0].img,
 *                         pD3[1].real, pD3[1].img, ...
 *                         The size of this array should be 2*Npt. Each 16-bit number in
 *                         this arrayis in Q15 format.
 *                          <br/>
 *  @param pF_out          (OUTPUT STREAM) pointer to an array for storing processed results. <br/>
 *                         Upon return, this is an array of complex numbers, arranged as real part and
 *                         img part interleave together, i.e pOut[0].real, pOut[0].img, pOut[1].real,
 *                         pOut[1].img, ... <br/>
 *                         At least 2*Npt*sizeof(short) bytes should be allocated to
 *                         this pointer before calling this function.
 *                         The output data is in Q15 format. <br/>
 *                          <br/>
 *  @param Npt             (INPUT) Number of IQ samples to be processed. <br/>
 *                          <br/>
 *  @param Len_HBFIR       (INPUT) Filter length. <br/>
 *                          <br/>
 *  @param pF              (INPUT) pointer to the filter coefficients. <br/>
 *                         The number of coefficients pointed by this pointer should be
 *                         ((Len_HBFIR+3)/4) * 2 + 1
 *                          <br/>
 *
 *  @return                None.
 *
 *
 * <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------
 *  |Name       |TCB Type      |Size       |Data Format |Valid Range |Default |Units |
 *  ----------------------------------------------------------------------------------
 *  |pD3        |Input Stream  |2*Npt      |S16*        |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  |pF_out     |Output Stream |2*Npt      |S16*        |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  |Npt        |Input         |1          |S16         |[0..32767]  |        |      |
 *  ----------------------------------------------------------------------------------
 *  |Len_HBFIR  |Input         |1          |S16         |[0..32767]  |        |      |
 *  ----------------------------------------------------------------------------------
 *  |pF         |Input         |Len_HBFIR  |S16         |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  TABLE_END
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          | prach_HBFilter                  |
 *  --------------------------------------------------------
 *  |Inputs Stream       | pD3                             |
 *  --------------------------------------------------------
 *  |Output Stream       | pF_out                          |
 *  --------------------------------------------------------
 *  |Config Parameters   | Npt, Len_HBFIR, pF              |
 *  --------------------------------------------------------
 *  |Status Parameters   | none                            |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 * <4> Detailed Description:
 *
 *   This function reduces the number of samples to be processed in PRACH preamble detection.
 *   The function uses a half-band FIR filter for a factor of 2 down-sampling.
 *
 * <5> Cycle Count Formula
 *  (((Len_HBFIR+3)/4) * 2 + 1) * 2 / 4 * 1.1 <br/>
 *  The factor 1.1 is for estimated 10% overhead.
 *  <br/>
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 * \ingroup group_lte_rx_uplink
 **/
//-------------------------------------------------------------------------------------------
void prach_HBFilter(S16 *pD3, S16 *pF_out, S16 Npt, S16 Len_HBFIR, S16 *pF)
{
    S32 i, j, j2, k, InStart, L;
    S32 sReal, sImag;

    L = (Len_HBFIR + 3) >> 2;
    InStart = 0;
    k = 0;
    for (i = 0; i < Npt; i++)
    {
        sReal = (S32) 0x00008000;
        sImag = (S32) 0x00008000;
        j2 = 0;
        for (j = 0; j < L; j++, j2 += 2)
        {
            sReal = L_mac(sReal, pD3[2 * (InStart + j2)], pF[j]);
            sImag = L_mac(sImag, pD3[2 * (InStart + j2) + 1], pF[j]);
        }
        j2--;
        sReal = L_mac(sReal, pD3[2 * (InStart + j2)], pF[L]);
        sImag = L_mac(sImag, pD3[2 * (InStart + j2) + 1], pF[L]);
        j2++;
        for (j = L - 1; j >= 0; j--, j2 += 2)
        {
            sReal = L_mac(sReal, pD3[2 * (InStart + j2)], pF[j]);
            sImag = L_mac(sImag, pD3[2 * (InStart + j2) + 1], pF[j]);
        }

        pF_out[k++] = extract_h(sReal);
        pF_out[k++] = extract_h(sImag);
        InStart += 2;
    }
}

#if 0
void prach_HBFilter( S16 *pD3, S16 *pF_out, S16 Npt, S16 Len_HBFIR, S16 *pF )
{
    S32 i, j, k, InStart;
    S32 sReal, sImag;
    //S16 *pF;
    //S16 Len_HBFIR;

    // Len_HBFIR = 27;
    // pF = prach_HBFIR;
    InStart = 0;
    k = 0;
    for (i=0; i < Npt; i++)
    {
        sReal = (S32) 0x00008000;
        sImag = (S32) 0x00008000;
        for (j=0; j < Len_HBFIR; j++)
        {
            sReal = L_mac(sReal, pD3[2 * (InStart+j)], pF[j]);
            sImag = L_mac(sImag, pD3[2 * (InStart+j)+1], pF[j]);
        }
        pF_out[ k++ ] = extract_h(sReal);
        pF_out[ k++ ] = extract_h(sImag);
        InStart += 2;
    }
}
#endif

#endif  // CEVA_INTRINSICS
//-----------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
/** @brief PRACH_Rx_1, the first stage of PRACH preamble detection
 *
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param pbs_prach       (INPUT) pointer to the structure BS_PRACH. <br/>
 *                          <br/>
 *  @param pAdcBuf         (INPUT STREAM) pointer to the array to be processed <br/>
 *                         This is an array of complex numbers, arranged as real part and img part
 *                         interleave together, i.e pAdcBuf[0].real, pAdcBuf[0].img,
 *                         pAdcBuf[1].real, pAdcBuf[1].img, ...
 *                         The size of this array should be 4*2*SEGMSIZE. Each 16-bit number in
 *                         this arrayis in Q15 format. The constant SEGMSIZE is defined in the
 *                         file bs_rx_prach.h. <br/>
 *                          <br/>
 *
 *  @return                None.
 *
 *
 * <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------
 *  |Name       |TCB Type      |Size         |Data Format |Valid Range |Default |Units |
 *  ------------------------------------------------------------------------------------
 *  |pbs_prach  |Input         |1            |BS_PRACH    |            |        |      |
 *  ------------------------------------------------------------------------------------
 *  |pAdcBuf    |Input Stream  |4*2*SEGMSIZE |S16*        |Q15         |        |      |
 *  ------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          | PRACH_Rx_1                      |
 *  --------------------------------------------------------
 *  |Inputs Stream       | pAdcBuf                         |
 *  --------------------------------------------------------
 *  |Config Parameters   | pbs_prach                       |
 *  --------------------------------------------------------
 *  |Status Parameters   | CorrIn, CorrInExp               |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *   This function links together the following function blocks in the first stage of PRACH
 *   preamble detection. <br/>
 *   (a) PRACH_Rx_Demod() <br/>
 *   (b) Prach_DownSampling() <br/>
 *   (c) 4096-point FFT <br/>
 *   The above functions will extract the PRACH data and transform the data into frequency
 *   domain for later processing. The resulting frequency domain data is stored in the
 *   array CorrIn[] and CorrInExp[]. The first 2*839*sizeof(S16) bytes are the results for
 *   the first antenna. The second 2*839*sizeof(S16) bytes are the results for the second
 *   antenna, and so on. The value CorrInExp[0] is the exponent value for the result of the
 *   first antenna. The value CorrInExp[1] is the exponent value for the result of the second
 *   antenna, and so on.
 *
 *
 * <5> Cycle Count Formula
 *  170000 cycles per antenna. <br/>
 *  <br/>
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
//-------------------------------------------------------------------------------------------
void PRACH_Rx_1(BS_PRACH *pbs_prach, S16 *pAdcBuf)
{
    S16 iSegm;
    S16 i, k;
    S16 *pIn;
    S16 fft_exp;
    S16 step_size;
    S16 step_size_table[10] = {-54, -42,    -30,    -18,    -6, 6,  18, 30, 42, 54};
    S16 prachFreqOffset = 0;


    pbs_prach->AdcNextIdx = 0;

    // Download the first input data segment to internal memory (should use DMA)
    //
    pIn = &(pAdcBuf[2 * pbs_prach->AdcNextIdx]);

    if(pbs_prach->nSegm == 0) // 1.4 MHz
        pbs_prach->SegmSize = 1920;

#ifdef CEVA_INTRINSICS

    k = 4 * pbs_prach->SegmSize;
    // MemCpyFromExt((void*) SegmBuf, (void*) pIn, k);
    MemCpyFromExt((void*) eVec, (void*) pIn, k);

#else

    for (i = 0; i < pbs_prach->SegmSize; i++)
    {
        eVec[2 * i] = pIn[2 * i];
        eVec[2 * i + 1] = pIn[2 * i + 1];
    }

#endif


    pbs_prach->AdcNextIdx += pbs_prach->SegmSize; // Next starting location


    // calculate step size for demodulation module
    switch(pbs_prach->Nseq)
    {
        case 24576 : //20 Mhz
            prachFreqOffset = (pbs_prach->k0 + 600)/12;
            if(prachFreqOffset % 2) // odd
                step_size = 3* shr(sub(prachFreqOffset,47),1);
            else
            {
                if(prachFreqOffset <= 46)
                    step_size = -2 + 3* shr(sub(prachFreqOffset,46),1);
                else
                    step_size = 2 + 3* shr(sub(prachFreqOffset,48),1);
            }
            break;
        case 12288: //10 Mhz
            prachFreqOffset = (pbs_prach->k0 + 300)/12;
            step_size = 3 * (sub(prachFreqOffset,22));
            break;
        case 6144: //5 Mhz
            prachFreqOffset = (pbs_prach->k0 + 150)/12;
            if(prachFreqOffset >= 10)
                step_size = 3 + 6 * (sub(prachFreqOffset,10));
            else
                step_size = -3 + 6 * (sub(prachFreqOffset,9));
            break;
        case 3072: //3 Mhz
            prachFreqOffset = (pbs_prach->k0 + 90)/12;
            step_size = step_size_table[prachFreqOffset];
            break;
        case 1536: //1.4 Mhz
            step_size = 0;
            break;
    }
    if (step_size < 0)
    {
        if((pbs_prach->Nseq == 24576) && ((prachFreqOffset % 2) == 0))
            step_size = 257 + step_size;
        else
            step_size = 256 + step_size;
    }

    // initialize the table for demodulation
    PRACH_Rx_Demod_init(&step_size,SegmBuf);

#if Debug_data_print
    {
        FILE *fp1;

        fp1 = (FILE *) fopen("Prach_demod_exptable.dat", "w");

    if (fp1 == NULL)
    {
        printf("[Print data Error in opening the file Prach_PER_result.dat\n");
    }
    for (i=0;i<256;i++)
        fprintf(fp1,"%d %d \n",SegmBuf[2*i],SegmBuf[2*i+1]);
    fclose(fp1);
    }

#endif
        // loop for each data segment
    for (iSegm = 0; iSegm <= pbs_prach->nSegm; iSegm++)
    {
        if(((iSegm == 0) && (pbs_prach->nSegm == 0)) || (iSegm < pbs_prach->nSegm) )
        {
            pbs_prach->iSegm = iSegm;

            // demodulation
        PRACH_Rx_Demod(pbs_prach, eVec,SegmBuf,eVec);

        #if Debug_data_print
          //
         {
         FILE *fp;
         if (pbs_prach->iAntenna == 0)
         {
             if(iSegm == 0)
             {
                 fp = (FILE *) fopen( "data_before_dec1", "w");
             }
             else if (iSegm == 1)
             {
                 fp = (FILE *) fopen( "data_before_dec2", "w");
             }
             else if (iSegm == 2)
             {
                 fp = (FILE *) fopen( "data_before_dec3", "w");
             }
             else if (iSegm == 3)
             {
                 fp = (FILE *) fopen( "data_before_dec4", "w");
             }
             else if (iSegm == 4)
             {
                 fp = (FILE *) fopen( "data_before_dec5", "w");
             }
             else if (iSegm == 5)
             {
                 fp = (FILE *) fopen( "data_before_dec6", "w");
             }
             else if (iSegm == 6)
             {
                 fp = (FILE *) fopen( "data_before_dec7", "w");
             }
             else if (iSegm == 7)
             {
                 fp = (FILE *) fopen( "data_before_dec8", "w");
             }
         }
         if (pbs_prach->iAntenna == 1)
         {
             if(iSegm == 0)
             {
                 fp = (FILE *) fopen( "data_before_dec11", "w");
             }
             else if (iSegm == 1)
             {
                 fp = (FILE *) fopen( "data_before_dec12", "w");
             }
             else if (iSegm == 2)
             {
                 fp = (FILE *) fopen( "data_before_dec13", "w");
             }
             else if (iSegm == 3)
             {
                 fp = (FILE *) fopen( "data_before_dec14", "w");
             }
             else if (iSegm == 4)
             {
                 fp = (FILE *) fopen( "data_before_dec15", "w");
             }
             else if (iSegm == 5)
             {
                 fp = (FILE *) fopen( "data_before_dec16", "w");
             }
             else if (iSegm == 6)
             {
                 fp = (FILE *) fopen( "data_before_dec17", "w");
             }
             else if (iSegm == 7)
             {
                 fp = (FILE *) fopen( "data_before_dec18", "w");
             }
         }
         for ( i=0; i < pbs_prach->SegmSize; i++ )
             {
             fprintf( fp, "%d %d\n",  eVec[2 * i], eVec[2 * i+1] );
             }
         fclose(fp);

        }
        #endif

         // downsampling
        if (pbs_prach->nSegm >= 2) //  5 Mhz, 10MHz or 20MHz
        {
            #ifdef CEVA_INTRINSICS
                if(33 != pbs_prach->Nf)
                    Prach_DownSampling_A(pbs_prach, eVec, PrachFFTin);  // 20, 10 Mhz
                else
                    Prach_DownSampling_NF_33(pbs_prach, eVec, PrachFFTin); // 5 Mhz
                    //Prach_DownSampling(pbs_prach, eVec, PrachFFTin);
            #else
                if(33 != pbs_prach->Nf)
                    Prach_DownSampling(pbs_prach, eVec, PrachFFTin);
                else
                    Prach_DownSampling_NF_33(pbs_prach, eVec, PrachFFTin); // 5 Mhz
            #endif
        }
        //else if (pbs_prach->nSegm == 1) // for 3 MHz
        //  Prach_DownSampling3MHz(pbs_prach, eVec, PrachFFTin);
        //else  // 1.4 MHz
        //  Prach_DownSampling1p4MHz(pbs_prach, eVec, PrachFFTin);


        // Download input data for the next segment
        if (iSegm < (pbs_prach->nSegm - 1))
        {
            // Download the next input data segment to internal memory (use DMA)
            pIn = &(pAdcBuf[2 * pbs_prach->AdcNextIdx]);

#ifdef CEVA_INTRINSICS

            k = 4 * pbs_prach->SegmSize;
            // MemCpyFromExt((void*) SegmBuf, (void*) pIn, k);
            MemCpyFromExt((void*) eVec, (void*) pIn, k);

#else

            for (i = 0; i < pbs_prach->SegmSize; i++)
            {

                eVec[2 * i] = pIn[2 * i];
                eVec[2 * i + 1] = pIn[2 * i + 1];

            }
#endif

            pbs_prach->AdcNextIdx += pbs_prach->SegmSize; // Next starting location
        }
    }

    }

#ifdef CEVA_INTRINSICS

    if (pbs_prach->nSegm == 8) // 20MHz

    {
        prach_HBFilter_A( PrachFFTin, PrachFFTin, (S16) (4096+40), 7, prach_HBFIR_1 );
        prach_HBFilter_A( PrachFFTin, PrachFFTin, (S16) (2048+20), 27, prach_HBFIR );
    }
    if (pbs_prach->nSegm == 4) //  10MHz

    {
        prach_HBFilter_A( PrachFFTin, PrachFFTin, (S16) (2048+20), 27, prach_HBFIR );
    }

    cfft_16t32d(PrachFFTin, (S16) (11) , zEst, &fft_exp);

    fft_exp -= 5;


#else  //CEVA_INTRINSICS

#if Debug_data_print
    {
            FILE *fp1, *fp2;
        if (pbs_prach->iAntenna == 0)
            fp1 = (FILE *) fopen("Prach_data_after_1st_dec1.dat", "w");
        else
            fp1 = (FILE *) fopen("Prach_data_after_1st_dec2.dat", "w");

        if (fp1 == NULL)
        {
            printf("[Print data Error in opening the file Prach_PER_result.dat\n");
        }
        for (i=0;i<4624;i++) // for 10 Mhz
            fprintf(fp1,"%d %d \n",PrachFFTin[2*i],PrachFFTin[2*i+1]);
        fclose(fp1);


        fp2 = (FILE *) fopen("Prach_data_after_1st_dec.dat", "at+");
        for (i=0;i<8292;i++) // for 20 Mhz
            fprintf(fp2,"%d %d \n",PrachFFTin[2*i],PrachFFTin[2*i+1]);
        fclose(fp2);

    }

#endif


    if (pbs_prach->nSegm == 8) // 20MHz
    {
        prach_HBFilter(PrachFFTin, PrachFFTin, (S16) (4096 + 40), 7, prach_HBFIR_1);

    #if Debug_data_print
        {
            FILE *fp1;
            fp1 = (FILE *) fopen("Prach_data_after_sec_dec1.dat", "at+");

        if (fp1 == NULL)
        {
            printf("[Print data Error in opening the file Prach_PER_result.dat\n");
        }
        for (i=0;i<4096 + 40;i++)
            fprintf(fp1,"%d %d \n",PrachFFTin[2*i],PrachFFTin[2*i+1]);
        fclose(fp1);
        }

    #endif

        prach_HBFilter(PrachFFTin, PrachFFTin, (S16) (2048 + 20), 27, prach_HBFIR);
    }
    if (pbs_prach->nSegm == 4) //  10MHz
    {
        prach_HBFilter(PrachFFTin, PrachFFTin, (S16) (2048 + 20), 27, prach_HBFIR);
    }


#if Debug_data_print
    {
        FILE *fp1;
    if (pbs_prach->iAntenna == 0)
        fp1 = (FILE *) fopen("Prach_data_after_dec1.dat", "w");
    else
        fp1 = (FILE *) fopen("Prach_data_after_dec2.dat", "w");

    if (fp1 == NULL)
    {
        printf("[Print data Error in opening the file Prach_PER_result.dat\n");
    }
    for (i=0;i<2048+264;i++)
        fprintf(fp1,"%d %d \n",PrachFFTin[2*i],PrachFFTin[2*i+1]);
    fclose(fp1);
    }

#endif

    cfft_16t32d_case2(PrachFFTin, (S16) (11) , zEst, &fft_exp);
    fft_exp -= 5;

#endif  //CEVA_INTRINSICS



#if Debug_data_print
    {
        FILE *fp1;
    if (pbs_prach->iAntenna == 0)
        fp1 = (FILE *) fopen("Prach_data_after_fft1.dat", "w");
    else
        fp1 = (FILE *) fopen("Prach_data_after_fft2.dat", "w");

    if (fp1 == NULL)
    {
        printf("[Print data Error in opening the file Prach_PER_result.dat\n");
    }
    for (i=0;i<2048;i++)
        fprintf(fp1,"%d %d \n",zEst[2*i],zEst[2*i+1]);
    fclose(fp1);
    }
#endif

    k = pbs_prach->iAntenna * NZC2;


    // get the signal in frequency band
    if((pbs_prach->Nseq == 24576) && ((prachFreqOffset % 2) == 0))
    {
        for (i = 0; i < 838 + 96; i++)
            CorrIn[k + i] = zEst[4095 - 837 - 96 + i];
        for (i = 838 + 96; i < 839*2; i++)
            CorrIn[k + i] = zEst[i - 838 - 96];
    }
    else
    {
       for(i = 0; i < 838; i++)
           CorrIn[k + i] = zEst[4095 - 837 + i];

       for(i = 838; i < 839*2; i++)
           CorrIn[k + i] = zEst[i - 838];
    }
    CorrInExp[pbs_prach->iAntenna] = fft_exp;

}
//-----------------------------------------------------------------------

#ifndef CEVA_INTRINSICS

//-------------------------------------------------------------------------------------------
/** @brief Complex multiplications of two complex arrays, the second array is conjugated..
 *
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param Vec1            (INPUT) pointer to the first complex array. <br/>
 *                          <br/>
 *  @param Vec2            (INPUT) pointer to the second complex array. <br/>
 *                          <br/>
 *  @param Vec3            (OUTPUT) pointer to the third complex array for storing the results. <br/>
 *                          <br/>
 *  @param length          (INPUT) number of complex elements in each array <br/>
 *                          <br/>
 *
 *  @return                None.
 *
 *
 * <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------
 *  |Name       |TCB Type      |Size       |Data Format |Valid Range |Default |Units |
 *  ----------------------------------------------------------------------------------
 *  |Vec1       |Input         |2*length   |S16*        |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  |Vec2       |Input         |2*length   |S16*        |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  |Vec3       |Output        |2*length   |S16*        |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  |length     |Input         |1          |S16         |Q15         |        |      |
 *  ----------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          | ComputeCmplxConjMult            |
 *  --------------------------------------------------------
 *  |Inputs Stream       | Vec1                            |
 *  --------------------------------------------------------
 *  |Inputs Stream       | Vec2                            |
 *  --------------------------------------------------------
 *  |Output Stream       | Vec3                            |
 *  --------------------------------------------------------
 *  |Config Parameters   | length                          |
 *  --------------------------------------------------------
 *  |Status Parameters   | none                            |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 * <4> Detailed Description:
 *   Complex multiplications of two complex arrays. The elements of the second array are
 *   conjugated before the multiplications.
 *
 *
 * <5> Cycle Count Formula
 *  1 cycle per complex multiplication. <br/>
 *  <br/>
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
//-------------------------------------------------------------------------------------------
void ComputeCmplxConjMult(S16 Vec1[], S16 Vec2[], S16 VecOut[], S16 length)
{
    S32 i;
    S32 L_temp1, L_temp2;
    S32 L_temp3, L_temp4;
    S16 temp1, temp2, temp3, temp4;
    S16 temp5, temp6, temp7, temp8;

    //Apply minus-half-carrier shift
    for (i = 0; i < (S32) (length >> 1); i++)
    {
        temp1 = Vec1[4 * i];
        temp2 = Vec2[4 * i];
        temp3 = Vec2[4 * i + 1];
        temp4 = Vec1[4 * i + 1];
        temp5 = Vec1[4 * i + 2];
        temp6 = Vec2[4 * i + 2];
        temp7 = Vec2[4 * i + 3];
        temp8 = Vec1[4 * i + 3];

        L_temp1 = L_mult(temp1, temp2);
        L_temp2 = L_mult(temp4, temp2);

        L_temp3 = L_mult(temp5, temp6);
        L_temp4 = L_mult(temp8, temp6);

        VecOut[4 * i] = mac_r(L_temp1, temp4, temp3);
        VecOut[4 * i + 1] = msu_r(L_temp2, temp3, temp1);

        VecOut[4 * i + 2] = mac_r(L_temp3, temp8, temp7);
        VecOut[4 * i + 3] = msu_r(L_temp4, temp5, temp7);
    }

    // if length is an odd number, the last one was not done!!!
    //
    temp1 = Vec1[4 * i];
    temp2 = Vec2[4 * i];
    temp3 = Vec2[4 * i + 1];
    temp4 = Vec1[4 * i + 1];
    temp5 = Vec1[4 * i + 2];
    temp6 = Vec2[4 * i + 2];
    temp7 = Vec2[4 * i + 3];
    temp8 = Vec1[4 * i + 3];

    L_temp1 = L_mult(temp1, temp2);
    L_temp2 = L_mult(temp4, temp2);

    L_temp3 = L_mult(temp5, temp6);
    L_temp4 = L_mult(temp8, temp6);

    VecOut[4 * i] = mac_r(L_temp1, temp4, temp3);
    VecOut[4 * i + 1] = msu_r(L_temp2, temp3, temp1);

    return;
}

#endif

//-----------------------------------------------------------------------
    // Delete "#ifndef CEVA_INTRINSICS" by Arraycomm,2011-05-25
    // Call c function to instead of assembly function,because there is no assembly file to be modified to support PRACH format 4.
    //#ifndef CEVA_INTRINSICS
//-------------------------------------------------------------------------------------------
/** @brief Normalize the data stored in the array CorrIn[].
 *
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param MO              (INPUT) Number of antennas. <br/>
 *                          <br/>
 *  @param CorrInExp       (INPUT/OUTPUT) Exponent values of the data pointed by CorrIn. <br/>
 *                          <br/>
 *  @param CorrIn          (INPUT/OUTPUT) Data to be normalized. <br/>
 *                          <br/>
 *
 *  @return                None.
 *
 *
 * <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------
 *  |Name       |TCB Type      |Size         |Data Format |Valid Range |Default |Units |
 *  ------------------------------------------------------------------------------------
 *  |MO         |Input         |1            |S16         | [1..4]     |        |      |
 *  ------------------------------------------------------------------------------------
 *  |CorrInExp  |Input/Output  |4            |S16         | Q15        |        |      |
 *  ------------------------------------------------------------------------------------
 *  |CorrIn     |Input/Output  |4*839        |S16         | Q15        |        |      |
 *  ------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          | NormCorrIn                      |
 *  --------------------------------------------------------
 *  |Inputs Stream       | None                            |
 *  --------------------------------------------------------
 *  |Config Parameters   | MO, CirrINExp, CorrIn           |
 *  --------------------------------------------------------
 *  |Status Parameters   | CorrIn, CorrInExp               |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *   The resulting exponent values stored in the array CorrInExp[] indicate different scaling
 *   factors for different antennas. This function uses the largest exponent value to normalize
 *   the data for all antennas.
 *
 *
 * <5> Cycle Count Formula
 *  0.5 cycles per complex number. <br/>
 *  <br/>
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
//-------------------------------------------------------------------------------------------
void NormCorrIn(S16 MO, S16 *CorrInExp, S16 *CorrIn)
{
    S32 i, k;
    S16 eMax, s;
    S16 *pCorrIn;

    eMax = -1; //-999;
    for (i = 0; i < MO; i++)
    {
        if (eMax < CorrInExp[i])
        {
            eMax = CorrInExp[i];
        }
    }
    // printf("[NormCorrIn] eMax = %d\n", eMax);

    for (i = 0; i < MO; i++)
    {
        if (eMax > CorrInExp[i])
        {
            s = (S16) 0x4000;
            s = s >> (eMax - CorrInExp[i] - 1);
            pCorrIn = &(CorrIn[i * NZC2]);
            for (k = 0; k < NZC2; k++)
            {
                pCorrIn[k] = mult_r(pCorrIn[k], s);
            }
            CorrInExp[i] = eMax;
        }
    }
}

#if 0
void NormCorrIn(BS_PRACH *pbs_prach)
{
    S32 i, k;
    S16 eMax, s;
    S16 *pCorrIn;

    eMax = -1; //-999;
    for (i=0; i < pbs_prach->MO; i++)
    {
        if (eMax < CorrInExp[i])
        {
            eMax = CorrInExp[i];
        }
    }
    // printf("[NormCorrIn] eMax = %d\n", eMax);

    for (i=0; i < pbs_prach->MO; i++)
    {
        if (eMax > CorrInExp[i])
        {
            s = (S16) 0x4000;
            s = s >> (eMax-CorrInExp[i] - 1);
            pCorrIn = &(CorrIn[i*NZC2]);
            for (k=0; k < NZC2; k++)
            {
                pCorrIn[k] = mult_r(pCorrIn[k], s);
            }
            CorrInExp[i] = eMax;
        }
    }
}

#endif
    //#endif  //CEVA_INTRINSICS
    // End Delete "#ifndef CEVA_INTRINSICS" by Arraycomm.
//-----------------------------------------------------------------------

#ifndef CEVA_INTRINSICS
//-------------------------------------------------------------------------------------------
/** @brief Accumulate power levels.
 *
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param pbs_prach       (INPUT) pointer to the structure BS_PRACH. <br/>
 *                          <br/>
 *  @param pPowerLevel     (INPUT/OUTPUT) pointer to the array for storing the power levels. <br/>
 *                          <br/>
 *  @param pIFFTout        (INPUT STREAM) pointer to the array to be processed <br/>
 *                         This is an array of complex numbers, arranged as real part and img part
 *                         interleave together, i.e pIFFTout[0].real, pIFFTout[0].img,
 *                         pIFFTout[1].real, pIFFTout[1].img, ...
 *                         The size of this array should be 2*Npt. Each 16-bit number in
 *                         this arrayis in Q15 format.  <br/>
 *                          <br/>
 *  @param Npt             (INPUT) number of samples to be processed. <br/>
 *                          <br/>
 *
 *  @return                average power.
 *
 *
 * <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------
 *  |Name        |TCB Type      |Size         |Data Format |Valid Range |Default |Units |
 *  -------------------------------------------------------------------------------------
 *  |pbs_prach   |Input         |1            |BS_PRACH    |            |        |      |
 *  -------------------------------------------------------------------------------------
 *  |pPowerLevel |Input/Output  |Npt          |S32*        |Q31         |        |      |
 *  -------------------------------------------------------------------------------------
 *  |pIFFTout    |Input         |2*Npt        |S16*        |Q15         |        |      |
 *  -------------------------------------------------------------------------------------
 *  |Npt         |Input         |1            |S16         |[0..32767]  |        |      |
 *  -------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          | AccPowerLevel                   |
 *  --------------------------------------------------------
 *  |Inputs Stream       | pIFFTout                        |
 *  --------------------------------------------------------
 *  |Config Parameters   | pbs_prach                       |
 *  --------------------------------------------------------
 *  |Status Parameters   | pPowerLevel, Npt                |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *   This function accumulate the power levels at the IFFT outputs from different antennas.
 *
 *
 * <5> Cycle Count Formula
 *  Npt/2 cycles per antenna. <br/>
 *  <br/>
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
//-------------------------------------------------------------------------------------------
S32 AccPowerLevel(BS_PRACH *pbs_prach, S32 *pPowerLevel, S16 *pIFFTout, S16 Npt)
{
    S32 i;
    S32 avePower, avePowerLow, temp;


    // calculate power
    for (i = 0; i < Npt; i++)
    {
        pPowerLevel[i] = L_mult(pIFFTout[2 * i], pIFFTout[2 * i]);
       // pPowerLevel[i] = L_mac(pPowerLevel[i], pIFFTout[2 * i], pIFFTout[2 * i]);
        pPowerLevel[i] = L_mac(pPowerLevel[i], pIFFTout[2 * i + 1], pIFFTout[2 * i + 1]);
    }

    //average power of each antenna
    avePower = 0;
    avePowerLow = 0;

    // calc average power
    for (i = 0; i < PRACH_IFFT_SIZE; i++)
    {
        avePower = L_add(avePower, L_shr (pPowerLevel[i],11) );
        temp = pPowerLevel[i] & 0x000007ff;
        avePowerLow = L_add(avePowerLow,temp );
    }

    // accurate average power
    avePower = L_add(avePower, L_shr (avePowerLow,11) );

    #if Debug_data_print
     {
     FILE *fp1, *fp2;

     fp1 = (FILE *) fopen( "AccPowerLevel_in.dat", "at+");

    for (i=0;i<2048;i++)
        fprintf(fp1,"%d %d \n",pIFFTout[2*i],pIFFTout[2*i+1]);
    fclose(fp1);

    fp1 = (FILE *) fopen( "AccPowerLevel_out.dat", "at+");
    for (i=0;i<2048;i++)
        fprintf(fp1,"%d  \n",pPowerLevel[i]);
    fclose(fp1);

     fp2 = (FILE *) fopen( "AccPowerLevel_average_power.dat", "at+");
    fprintf(fp2,"%d \n",avePower);
    fclose(fp2);
     }
#endif


    return (avePower);



}

#endif  //CEVA_INTRINSICS

//-----------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
/** @brief Peak searching after correlation.
 *
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param pbs_prach       (INPUT) pointer to the structure BS_PRACH. <br/>
 *                          <br/>
 *  @param iseq            (INPUT) The sequence number. <br/>
 *                          <br/>
 *  @param powerSearchWin  (INPUT) Power peak search window. <br/>
 *                          <br/>
 *  @param pPowerLevel     (INPUT) pointer to the array for storing the power levels. <br/>
 *                          <br/>
 *  @param PwrTh           (INPUT) pointer to the array for storing the detection thresholds. <br/>
 *                          <br/>
 *  @param pBestAntInfo    (INPUT) pointer to the array for storing the best antenna's PAPR and location. <br/>
 *                          <br/>
 *
 *  @return                None.
 *
 *
 * <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------
 *  |Name           |TCB Type      |Size         |Data Format |Valid Range |Default |Units |
 *  ----------------------------------------------------------------------------------------
 *  |pbs_prach      |Input         |1            |BS_PRACH    |            |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |iseq           |Input         |1            |S16         |[0..63]     |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |pPowerLevel    |Input         |1024         |S32*        |Q31         |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |PwrTh          |Input         |2            |S16*        |[0..32767]  |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |powerSearchWin |Iutput        |2            |S16*        |[0...2047]  |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |pBestAntInfo   |Input         |3            |S32*        |[0..65535]  |        |      |
 *  ----------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------------
 *  |Block Name          | PrachPeakSearching                     |
 *  ---------------------------------------------------------------
 *  |Inputs Stream       | pPowerLevel                            |
 *  ---------------------------------------------------------------
 *  |Config Parameters   | pbs_prach, iseq, PwrTh, powerSearchWin |
 *  ---------------------------------------------------------------
 *  |Status Parameters   | pbs_prach, pBestAntInfo                |
 *  ---------------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *   Based on the power levels, this function determines whether a peak is detected or not.
 *
 *
 * <5> Cycle Count Formula
 *  TBD. <br/>
 *  <br/>
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
//-------------------------------------------------------------------------------------------
void PrachPeakSearching(BS_PRACH *pbs_prach, S16 preambleCnt, S16 searchWin, S16 iseq, S32 *pPowerLevel,
                              S32 * pBestAntInfo, S16 *powerSearchWin,
                              U16 prachDetThreshold)
{

    S16 k,  loc;
    S32 Pmax, Pav32;
    U16 prachPHR;

#ifndef CEVA_INTRINSICS
    S16 i, index;
#endif

    // average power
    Pav32 = pBestAntInfo[2];

    // find the PAPR peak
    Pmax = 0;
    loc = 0;

#ifdef CEVA_INTRINSICS
        FindPowerPeak_A(powerSearchWin[0], powerSearchWin[1], &Pmax, &loc, pPowerLevel);
#else
        for (i = powerSearchWin[0]; i < powerSearchWin[1]; i++)
        {
            index = i & 0x7ff;
            if (Pmax < pPowerLevel[index])
            {
                Pmax = pPowerLevel[index];
                //loc = index;
                loc = i;
            }
        }
#endif


    loc = loc & 0x7ff;

    if (Pav32 != 0)
        prachPHR = (U16)(Pmax / Pav32);
    else
        prachPHR = 0;

    // detected or not
 //   if (Pmax > prachDetThreshold * Pav32)
    if(prachPHR > prachDetThreshold)
    {
        k = pbs_prach->nPeakDetected;
        pbs_prach->iseqPeakDetected[k] = iseq;

        if(pBestAntInfo[0] == 1)
        //if((pBestAntInfo[0] == 1) && (Pmax < 600 * Pav32))
        {
            pbs_prach->locPeakDetected[k] = (S16) pBestAntInfo[1];
        }
        else
        {
            pbs_prach->locPeakDetected[k] = (S16) loc;
        }
        pbs_prach-> preambleCntPeakDetected[k] = preambleCnt;
        pbs_prach-> searchWinPeakDetected[k] = searchWin;
        pbs_prach->prachDetPHR[k] = prachPHR;
        pbs_prach->nPeakDetected += 1;
#if Debug_Timing_Est_print
        {
         // printf timing offset estimation
        FILE *fp_PRACH_snr;
        fp_PRACH_snr = (FILE *) fopen("PRACH_test_timing_estimation_snr.dat", "at+");
        fprintf(fp_PRACH_snr, "%d %d %d\n", Pmax, Pav32, Pmax/Pav32);
        fclose(fp_PRACH_snr);
        }
#endif
    }
    else if(prachDetThreshold <= PRACH_DETECTION_THRESHOLD_NOMINAL)
    {
        if ((pBestAntInfo[0] == 1) )
        {
            k = pbs_prach->nPeakDetected;
            pbs_prach->iseqPeakDetected[k] = iseq;
            pbs_prach->locPeakDetected[k] =  ( S16) (pBestAntInfo[1]);
            pbs_prach-> preambleCntPeakDetected[k] = preambleCnt;
            pbs_prach-> searchWinPeakDetected[k] = searchWin;
            pbs_prach->prachDetPHR[k] = prachPHR;
            pbs_prach->nPeakDetected += 1;
#if Debug_Timing_Est_print
            {
             // printf timing offset estimation
            FILE *fp_PRACH_snr;
            fp_PRACH_snr = (FILE *) fopen("PRACH_test_timing_estimation_snr.dat", "at+");
            fprintf(fp_PRACH_snr, "%d %d %d\n", Pmax, Pav32, Pmax/Pav32);
            fclose(fp_PRACH_snr);
            }
#endif
        }
        //else if ((pBestAntInfo[0] == 2)&& (Pmax > 10 * Pav32))
        else if ((pBestAntInfo[0] == 2)&& (prachPHR > 10))
        {
            k = pbs_prach->nPeakDetected;
            pbs_prach->iseqPeakDetected[k] = iseq;
            pbs_prach->locPeakDetected[k] =  ( S16) (pBestAntInfo[1]);
            pbs_prach-> preambleCntPeakDetected[k] = preambleCnt;
            pbs_prach-> searchWinPeakDetected[k] = searchWin;
            pbs_prach->prachDetPHR[k] = prachPHR;
            pbs_prach->nPeakDetected += 1;
#if Debug_Timing_Est_print
            {
             // printf timing offset estimation
            FILE *fp_PRACH_snr;
            fp_PRACH_snr = (FILE *) fopen("PRACH_test_timing_estimation_snr.dat", "at+");
            fprintf(fp_PRACH_snr, "%d %d %d\n", Pmax, Pav32, Pmax/Pav32);
            fclose(fp_PRACH_snr);
            }
#endif
        }
    }
#if Debug_PAPR_print
    {
        FILE *fp;

        fp = (FILE *) fopen( "PAPR_results.txt", "at+");
        fprintf( fp, "%d %d %d %d %d %d %d\n", Pmax , loc , Pav32,pbs_prach->nPeakDetected, pbs_prach->locPeakDetected[0], pbs_prach->locPeakDetected[1],powerSearchWin[0] );

        fclose(fp);
    }
#endif
}

//-----------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
/** @brief Initialize the structure for PRACH detection.
 *
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param pbs_prach       (OUTPUT) pointer to the structure BS_PRACH. <br/>
 *                          <br/>
 *  @param bsRxPrach       (INPUT) pointer to the structure BS_RX_PRACH. <br/>
 *                          <br/>
 *  @param bsRxPrachDet    (INPUT) pointer to the structure BS_RX_PRACHDet. <br/>
 *                          <br/>
 *  @param length_adcPrach (INPUT) number of samples to be processed, for each antenna. <br/>
 *                          <br/>
 *  @param MO              (INPUT) number of antenna. <br/>
 *                          <br/>
 *
 *  @return                None.
 *
 *
 * <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------
 *  |Name             |TCB Type      |Size    |Data Format    |Valid Range |Default |Units |
 *  ----------------------------------------------------------------------------------------
 *  |pbs_prach        |Output        |1       |BS_PRACH       |            |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |bsRxPrach        |Input         |1       |BS_RX_PRACH    |            |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |bsRxPrachDet     |Input         |1       |BS_RX_PRACHDet |            |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |length_adcPrach  |Input         |1       |S32            |[0..30720]  |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |MO               |Input         |1       |S16            |[1..4]      |        |      |
 *  ----------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------------------------
 *  |Block Name          | PRACH_Rx_Init                                  |
 *  -----------------------------------------------------------------------
 *  |Inputs Stream       | None                                           |
 *  -----------------------------------------------------------------------
 *  |Config Parameters   | bsRxPrach, bsRxPrachDet, length_adcPrach, MO   |
 *  -----------------------------------------------------------------------
 *  |Status Parameters   | pbs_prach                                      |
 *  -----------------------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *   Based on the system configuration parameters, this function initializes the structure
 *   for PRACH preamble detection.
 *
 *
 * <5> Cycle Count Formula
 *  TBD. <br/>
 *  <br/>
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
#ifndef CEVA_INTRINSICS
//-------------------------------------------------------------------------------------------
void PRACH_Rx_Init(BS_PRACH *pbs_prach, BS_RX_PRACH *bsRxPrach, BS_RX_PRACHDet *bsRxPrachDet,
                   S32 length_adcPrach, S16 MO)
{
    S16 iseq;
    S16 u, Cv, vLen, Ncs;
    //S16 DSR, delay;

    // Initialization
    //
    pbs_prach->MO = MO;
    pbs_prach->Nzc = bsRxPrach->bs_prach_preambleSeqLength;
    pbs_prach->K = bsRxPrach->bs_prach_subcSpacingRatio;
    pbs_prach->phi = bsRxPrach->bs_prach_subcOffset;
    pbs_prach->k0 = bsRxPrach->bs_prach_k0;
    pbs_prach->Nseq = bsRxPrach->bs_prach_Nseq;
    pbs_prach->Ncp = bsRxPrach->bs_prach_Ncp;
    pbs_prach->FIRLength = bsRxPrachDet->bs_prachDet_firLpf_length;
    pbs_prach->length_adcPrach = length_adcPrach;

    pbs_prach->SegmSize = SEGMSIZE;
    pbs_prach->nSegm = (S16) length_adcPrach / pbs_prach->SegmSize;
    pbs_prach->iSegm = 0;

    //pbs_prach->demod_start = -pbs_prach->Ncp;
    //pbs_prach->demod_end = (S16) (pbs_prach->length_adcPrach - pbs_prach->Ncp);
    //pbs_prach->demod_istart = pbs_prach->demod_start;
    //pbs_prach->demod_idx = 0;

    pbs_prach->DSR = bsRxPrachDet->bs_prachDet_downsampleRate;
    pbs_prach->Nf = bsRxPrachDet->bs_prachDet_firLpf_length;
    pbs_prach->lpfDelay = bsRxPrachDet->bs_prachDet_lpfDelay;
    pbs_prach->IdxDown = 0;
    // pbs_prach->nStart = 0;
    //DSR = pbs_prach->DSR;
    //delay = pbs_prach->lpfDelay;

#if PRACH_USE_4096FFT
    pbs_prach->nStart = (pbs_prach->Nf + 1)/2 + pbs_prach->Ncp; //4096 FFT  ??????????????
#endif

#if PRACH_USE_2048FFT
    pbs_prach->nStart = (pbs_prach->Nf + 1) / 2 + pbs_prach->Ncp; // default //2048 FFT
    if (pbs_prach->nSegm == 8)  //20 Mhz
    {
        pbs_prach->Nf = bsRxPrachDet->bs_prachDet_firLpf_length;
        DownSamplingFIR = prachDet_firLpfMid6;
        pbs_prach->nStart = (pbs_prach->Nf + 1) / 2 + pbs_prach->Ncp - 80; // (39+39+9) //2048 FFT
    }
    if (pbs_prach->nSegm == 4) //10Mhz
    {
        pbs_prach->Nf = bsRxPrachDet->bs_prachDet_firLpf_length;
        DownSamplingFIR = prachDet_firLpfMid6;
        //pbs_prach->Nf = 37;
        //DownSamplingFIR = prach_fir37_D3;
        //pbs_prach->nStart = (pbs_prach->Nf + 1) / 2 + pbs_prach->Ncp - 40; //2048 FFT
        pbs_prach->nStart = (pbs_prach->Nf + 1) / 2 + pbs_prach->Ncp - 36; //2048 FFT
        //pbs_prach->nStart = 1552;
    }
    if (pbs_prach->nSegm == 2) //5Mhz
    {

        pbs_prach->Nf = 33;
        DownSamplingFIR = prach_fir33_D3;
        pbs_prach->nStart = pbs_prach->Ncp + 14; //2048 FFT
    }

  //  if (pbs_prach->nSegm == 2) //5Mhz
  //  {

        //pbs_prach->Nf = 37;
  //      DownSamplingFIR = prach_fir33_D3;
        //pbs_prach->nStart = pbs_prach->Ncp + 20; //2048 FFT
  //  }
  //  if (pbs_prach->nSegm == 1) // 3MHz
  //  {
        //pbs_prach->Nf = 49;
  //      DownSamplingFIR = prach_fir49_D3;
        //pbs_prach->nStart = pbs_prach->Ncp + 13; //2048 FFT
  //  }
  //  if (pbs_prach->nSegm == 0) // 1.4MHz
  //  {
        //pbs_prach->Nf = 37;
  //     // DownSamplingFIR = prach_fir37_D4;
        // DownSamplingFIR = prach_fir33_D3;
        //pbs_prach->nStart = pbs_prach->Ncp + 5; //2048 FFT
  //  }
#endif


    pbs_prach->dummy = pbs_prach->nStart; // Save nStart for later re-init

    //for (i=1; i <= 2*pbs_prach->Nf; i++)
    //{
    //    eVec[ -i ] = 0;
    //}
    //CorrInExp[pbs_prach->iAntenna] = 9999;

    pbs_prach->numRootSeq = bsRxPrachDet->bs_prachDet_numRootSeq;
    for (iseq = 0; iseq < bsRxPrachDet->bs_prachDet_numRootSeq; iseq++)
    {
        //init pointers to u'th root sequence
        bsRxPrachDet->bs_prachDet_enable = 1;
        bsRxPrachDet->bs_prachDet_cyclicShift = 0;
        bsRxPrachDet->bs_prachDet_uOffset = (S16) iseq; //move to next logical index!
        ul_PrachPreamble_Config(bsRxPrach, bsRxPrachDet, &u, &Cv, &vLen, &Ncs);
        pbs_prach->u[iseq] = u;
        pbs_prach->Cv = Cv;
        pbs_prach->vLen = vLen;
        pbs_prach->Ncs = Ncs;
    }
    if (pbs_prach->numRootSeq > 6)
    {
        pbs_prach->numRootSeq = 6; // <<<<<< limit it to 6 root sequences at most, processing time of 10 Mhz and 20 Mhz
    }

    pbs_prach->nPeakDetected = 0;
 /*   for (i = 0; i < 64; i++)
    {
        pbs_prach->iseqPeakDetected[i] = (S16) 0x7777;
        pbs_prach->locPeakDetected[i] = (S16) 0x7777;
    }*/
}

#endif

//-----------------------------------------------------------------------




//-------------------------------------------------------------------------------------------
/** @brief Find the peak powers and their locations
 *
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param p               (INPUT) Power array to be processed. <br/>
 *                          <br/>
 *  @param powerSearchWin  (INPUT) Power peak search window. <br/>
 *                          <br/>
 *  @param PeakPower       (OUTPUT) pointer to the location for storing the first peak. <br/>
 *                          <br/>
 *  @param PeakLoc         (OUTPUT) pointer to the location for storing the index of the first peak. <br/>
 *                          <br/>
 *  @param PeakPower2      (OUTPUT) pointer to the location for storing the second peak. <br/>
 *                          <br/>
 *  @param PeakLoc2        (OUTPUT) pointer to the location for storing the index of the second peak. <br/>
 *                          <br/>
 *
 *  @return                None.
 *
 *
 * <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------
 *  |Name           |TCB Type      |Size         |Data Format |Valid Range |Default |Units |
 *  ----------------------------------------------------------------------------------------
 *  |p              |Input         |1024         |S32         |Q31         |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |powerSearchWin |Iutput        |2            |S16*        |[0...2047]  |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |PeakPower      |Output        |1            |S32*        |Q31         |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |PeakLoc        |Output        |1            |S16*        |[0..32767]  |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |PeakPower2     |Output        |1            |S32*        |Q31         |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |PeakLoc2       |Output        |1            |S16*        |[0..32767]  |        |      |
 *  ----------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          | PeakPowerLocation               |
 *  --------------------------------------------------------
 *  |Inputs Stream       | p                               |
 *  --------------------------------------------------------
 *  |Config Parameters   | powerSearchWin                  |
 *  --------------------------------------------------------
 *  |Status Parameters   | PeakPower,                      |
 *  |                    | PeakLoc, PeakPower2, PeakLoc2   |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *   This function find the peaks and their indexes for each search window.
 *
 *
 *
 * <5> Cycle Count Formula
 *  TBD. <br/>
 *  <br/>
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
//-------------------------------------------------------------------------------------------
//void PeakPowerLocation(S32 *p, S32 *PeakPower, S16 *PeakLoc, S32 *PeakPower2, S16 *PeakLoc2, S16 *powerSearchWin)
//{
//    S16 i, index;
//    S32 PkPower;
//    S16 PkLoc;
//    S32 PkPower2;
//    S16 PkLoc2;
//  S16 searchWindowMin,searchWindowMax;
//  S16 peakWindowMin,peakWindowMax;
//
//
//
//    PkPower = 0;
//    PkLoc = 0;
//  // find power  PEAK
//    for (i = powerSearchWin[0]; i < powerSearchWin[1]; i++)
//    {
//      index = i & 0x7ff;
//        if (PkPower < p[index])
//        {
//            PkPower = p[index];
//            //PkLoc = (S16) index;
//          PkLoc = (S16) i;
//        }
//    }
//
//  //PkLoc = 1014 + 2047;
//
//  // find the next peak at least 3 samples away, and at most 12 samples away from the max one; belong to the same zero correlation zone
//
//  if (PkLoc > (12 + powerSearchWin[0]))
//      searchWindowMin = PkLoc - 12;
//  else
//      searchWindowMin = powerSearchWin[0];
//
//  if (PkLoc > (powerSearchWin[1] - 12))
//      searchWindowMax = powerSearchWin[1];
//  else
//      searchWindowMax = PkLoc + 12;
//
//  if (PkLoc > (2 + powerSearchWin[0]))
//      peakWindowMin = PkLoc - 2;
//  else
//      peakWindowMin = powerSearchWin[0];
//
//  if (PkLoc > (powerSearchWin[1] - 2))
//      peakWindowMax = powerSearchWin[1];
//  else
//      peakWindowMax = PkLoc + 2;
//
//  PkLoc = PkLoc & 0x7ff;
//
//  // search max from searchWindowMin to peakWindowMin
//  PkPower2 = 0;
//    PkLoc2 = 0;
//  for (i = searchWindowMin; i < peakWindowMin; i ++)
//  {
//      index = i & 0x7ff;
//        if (PkPower2 < p[index])
//        {
//            PkPower2 = p[index];
//            PkLoc2 = (S16) index;
//        }
//  }
//
//  // search max from peakWindowMax to searchWindowMax
//  for (i = peakWindowMax; i < searchWindowMax; i ++)
//  {
//      index = i & 0x7ff;
//        if (PkPower2 < p[index])
//        {
//            PkPower2 = p[index];
//            PkLoc2 = (S16) index;
//        }
//  }
//
//    *PeakPower = PkPower;
//    *PeakLoc = PkLoc;
//    *PeakPower2 = PkPower2;
//    *PeakLoc2 = PkLoc2;
//}

//void FindPowerPeak(S16 startPoint, S16 endPoint, S32 *pPkPower, S16 *pPkLoc, S32 *p)
//{
//    S16 i, index;
//    S32 PkPower;
//    S16 PkLoc;
//
//    PkPower = 0;
//    PkLoc = 0;
//  // find power PEAK
//    for (i = startPoint; i < endPoint; i++)
//    {
//      index = i & 0x7ff;
//        if (PkPower < p[index])
//        {
//            PkPower = p[index];
//          PkLoc = (S16) i;
//        }
//    }
//
//    *pPkPower = PkPower;
//    *pPkLoc   = PkLoc;
//}


void PeakPowerLocation(S32 *p, S32 *PeakPower, S16 *PeakLoc, S32 *PeakPower2, S16 *PeakLoc2, S16 *powerSearchWin)
{
    S32 PkPower;
    S16 PkLoc;
    S32 PkPower2, PkPower2_temp;
    S16 PkLoc2;
    S16 searchWindowMin,searchWindowMax;
    S16 peakWindowMin,peakWindowMax;
    S16 PkLoc2_temp;

#ifndef CEVA_INTRINSICS
    S16 i, index;
#endif

    PkPower = 0;
    PkLoc = 0;
    // find power  PEAK
#ifdef CEVA_INTRINSICS
    FindPowerPeak_A(powerSearchWin[0], powerSearchWin[1], &PkPower, &PkLoc, p);
#else
    for (i = powerSearchWin[0]; i < powerSearchWin[1]; i++)
    {
        index = i & 0x7ff;
        if (PkPower < p[index])
        {
            PkPower = p[index];
            //PkLoc = (S16) index;
            PkLoc = (S16) i;
        }
    }
#endif
    //PkLoc = 1014 + 2047;
    // find the next peak at least 3 samples away, and at most 12 samples away from the max one; belong to the same zero correlation zone
    if (PkLoc > (12 + powerSearchWin[0]))
        searchWindowMin = PkLoc - 12;
    else
        searchWindowMin = powerSearchWin[0];

    if (PkLoc > (powerSearchWin[1] - 12))
        searchWindowMax = powerSearchWin[1];
    else
        searchWindowMax = PkLoc + 12;

    if (PkLoc > (2 + powerSearchWin[0]))
        peakWindowMin = PkLoc - 2;
    else
        peakWindowMin = powerSearchWin[0];

    if (PkLoc > (powerSearchWin[1] - 2))
        peakWindowMax = powerSearchWin[1];
    else
        peakWindowMax = PkLoc + 2;

    PkLoc = PkLoc & 0x7ff;

    // search max from searchWindowMin to peakWindowMin
#ifdef CEVA_INTRINSICS
   FindPowerPeak_A(searchWindowMin, peakWindowMin, &PkPower2, &PkLoc2, p);
#else
    PkPower2 = 0;
    PkLoc2 = 0;
    for (i = searchWindowMin; i < peakWindowMin; i ++)
    {
        index = i & 0x7ff;
        if (PkPower2 < p[index])
        {
            PkPower2 = p[index];
            PkLoc2 = (S16) i;
        }
    }
#endif

    // search max from peakWindowMax to searchWindowMax
#ifdef CEVA_INTRINSICS
    FindPowerPeak_A(peakWindowMax, searchWindowMax, &PkPower2_temp, &PkLoc2_temp, p);
#else
    PkPower2_temp = 0;
    PkLoc2_temp = 0;
    for (i = peakWindowMax; i < searchWindowMax; i ++)
    {
        index = i & 0x7ff;
        if (PkPower2_temp < p[index])
        {
            PkPower2_temp = p[index];
            PkLoc2_temp = (S16) i;
        }
    }
#endif

    if(PkPower2_temp > PkPower2)
    {
        PkPower2 = PkPower2_temp;
        PkLoc2 = PkLoc2_temp;
    }

    PkLoc2 = PkLoc2 & 0x7ff;

    *PeakPower = PkPower;
    *PeakLoc = PkLoc;
    *PeakPower2 = PkPower2;
    *PeakLoc2 = PkLoc2;
}



//-------------------------------------------------------------------------------------------
/** @brief Combining the powers from different antennas for PRACH preamble detection.
 *
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param MO              (INPUT) Number of antennas. <br/>
 *                          <br/>
 *  @param powerSearchWin  (InPUT) power peak search window. <br/>
 *                          <br/>
 *  @param averagePower    (InPUT) average power for each antenna. <br/>
 *                          <br/>
 *  @param PwrTh           (OUTPUT) pointer to the array for storing the detection thresholds. <br/>
 *                          <br/>
 *  @param AntPower        (INPUT) Array of pointers that points to the power arrays of different
 *                         antennas. <br/>
 *                          <br/>
 *  @param pBestAntInfo    (INPUT) pointer to the array for storing the best antenna's PAPR and location. <br/>
 *                          <br/>
 *  @return                None.
 *
 *
 * <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------
 *  |Name           |TCB Type      |Size         |Data Format |Valid Range |Default |Units |
 *  ----------------------------------------------------------------------------------------
 *  |MO             |Input         |1            |S16         |[1..4]      |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |powerSearchWin |Iutput        |2            |S16*        |[0...2047]  |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |averagePower   |Input         |2            |S32*        |[0..65535]  |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |PwrTh          |Output        |2            |S16*        |[0..32767]  |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |AntPower       |Input         |4            |S32**       |Q31         |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |pBestAntInfo   |Input         |3            |S32*        |[0..65535]  |        |      |
 *  ----------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          | CombinePower                    |
 *  --------------------------------------------------------
 *  |Inputs Stream       | AntPower                        |
 *  --------------------------------------------------------
 *  |Config Parameters   | MO,powerSearchWin,averagePower  |
 *  --------------------------------------------------------
 *  |Status Parameters   | PwrTh, pBestAntInfo             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *   This function combines the power levels from different antennas for the purpose of PRACH preamble
 *   detection.
 *
 *
 * <5> Cycle Count Formula
 *  TBD. <br/>
 *  <br/>
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
//-------------------------------------------------------------------------------------------
void CombinePower(S16 MO, S32 **AntPower, S32 * pBestAntInfo, S32 * averagePower, S16 *powerSearchWin)
{
    S32 pp[4];
    S16 pLoc[4];
    S32 pp2[4];
    S16 pLoc2[4];
    S16 r2[4];
    S16 r[4];
    S32 PeakPower, PeakPower2;
    S16 PeakLoc, PeakLoc2;
    S32 i, i0, i1;
//    S16 w14ant, w24ant;
    S16 temp_r1,temp_r2,temp_max;
    S16 minPeakLoc;
    S16 diffLoc, sumPeak;


    for (i = 0; i < 2; i++)
    {
        pp[i] = (S32) 1;
        pLoc[i] = (S16) 1;
        pp2[i] = (S32) 1;
        pLoc2[i] = (S16) 128;
        r[i] = 1;
        r2[i] = 1;
    }
    for (i = 0; i < MO; i++)
    {
        // ap[i] = (S32) AveragePower( AntPower[i] );
#ifdef CEVA_INTRINSICS
        PeakPowerLocation(AntPower[i], &PeakPower, &PeakLoc, &PeakPower2, &PeakLoc2, powerSearchWin);
#else
        PeakPowerLocation(AntPower[i], &PeakPower, &PeakLoc, &PeakPower2, &PeakLoc2, powerSearchWin);
#endif

        pp[i] = (S32) PeakPower;
        pLoc[i] = (S16) PeakLoc;
        pp2[i] = (S32) PeakPower2;
        pLoc2[i] = (S16) PeakLoc2;
    }


    // PAPR
    for (i = 0; i < MO; i++)
    {
        if (averagePower[i] == 0)
        {
            averagePower[i] = 1; // avoid divided by 0
        }
        r[i] = (S16) (pp[i] / (averagePower[i] ));
        r2[i] = (S16) (pp2[i] / (averagePower[i] ));

        if (r2[i] >= 8)  // combine multipath together
        {
            r[i] = add(r[i],r2[i]);
        }
        if ((r[i] < 5 * r2[i]) &&  (r2[i] >= 15) &&  (abs_s(sub(pLoc[i], pLoc2[i])) <= 15)) //choose the minimum valid peak locations for each antenna if both are strong paths
        {
            if (pLoc[i] < pLoc2[i])
                pLoc[i] = pLoc[i];
            else
                pLoc[i] = pLoc2[i];
        }
    }


    minPeakLoc = 2047;
    // find the minimum peak locations among antennas for possible valid paths, if PAPR > 8
    for (i = 0; i < MO; i++)
    {
        if ((r[i] > 8) && (minPeakLoc > pLoc[i]))
            minPeakLoc = pLoc[i];
    }

    // average peak location among two antennas
    //avePeakLoc = shr(add(pLoc[0], pLoc[1]), 1);


    // find the max Peak among two antennas
    temp_r1 = r[0];
    temp_r2 = r[1];


    if(temp_r1 > temp_r2)
    {
        temp_max = temp_r1;
    }
    else
    {
        temp_max = temp_r2;
    }

    // find the best 2 antennas among 4 antennas; or sort the 2 antennas
 //   rMax[0] = 0;
 //   iMax[0] = 0;
 //   for (i = 0; i < MO; i++)
 //   {
 //       if (rMax[0] < r[i])
 //       {
 //           rMax[0] = r[i];
 //           iMax[0] = i;
 //       }
 //   }

    //temp_r = r[iMax[0]];
 //   r[iMax[0]] = 0;

 //   rMax[1] = 0;
 //   iMax[1] = 0;
 //   for (i = 0; i < MO; i++)
 //   {
 //       if (rMax[1] < r[i])
 //       {
 //           rMax[1] = r[i];
 //           iMax[1] = i;
 //       }
 //   }


    //r[iMax[0]] = temp_r;

 //   i0 = iMax[0]; // antenna index with the max PAPR
 //   i1 = iMax[1]; // antenna index with the second max PAPR

    if(r[0] > r[1])
    {
        i0 = 0;
        i1 = 1;
    }
    else
    {
        i0 = 1;
        i1 = 0;
    }

    //average power over two antenas
//  pBestAntInfo[2] = (S32) L_shr(L_add(averagePower[i0], averagePower[i1]), 1);

    // detection criteria for multipath channels
    pBestAntInfo[0] = 0;  // best antenna meets the PAPR requirement or not
    pBestAntInfo[1] = 0;   // location of the peak

    if (pLoc[1] == 2047)
    {
        pLoc[1] = 0;
    }

    if (pLoc[0] == 2047)
    {
        pLoc[0] = 0;
    }

    diffLoc = abs_s(sub(pLoc[1], pLoc[0]));
    sumPeak = temp_r1 + temp_r2;

    if ((temp_max > 31))
    {
        pBestAntInfo[0] = 1; // indicator, in this case, choose the best antenna's peak location
        if ((r[i1] > 15) && (diffLoc < 15))
        {
            if (pLoc[1] < pLoc[0])
                pBestAntInfo[1] = pLoc[1];
            else
                pBestAntInfo[1] = pLoc[0];
        }
        else
            pBestAntInfo[1] = pLoc[i0] ; //location
        //pBestAntInfo[1] = minPeakLoc;
    }
    else if ((sumPeak >=23) && (diffLoc < 7) && (sumPeak - diffLoc >= 23) )
    {
        pBestAntInfo[0] = 2; //indicator, in this case, both antennas are good ,use the average peak location
        //pBestAntInfo[1] = shr(add( pLoc[1] , pLoc[0]), 1);
        pBestAntInfo[1] = minPeakLoc;
    }
    else if ((sumPeak >=30) && (diffLoc < 15) )
    {
        pBestAntInfo[0] = 2; //indicator
        //pBestAntInfo[1] = shr(add( pLoc[1] , pLoc[0]), 1);
        pBestAntInfo[1] = minPeakLoc;
    }
    //else if ((temp_max >= 18) )  // we need to use the combined PAPR as another criteria in the final detection
    //{
    //  pBestAntInfo[0] = 3; //indicator
    //  //pBestAntInfo[1] = pLoc[i0] ;
    //  pBestAntInfo[1] = minPeakLoc;
    //}



    //w14ant = 15; // detection threshold

    //w24ant = w14ant - 1;

#ifdef DEBUG_PRINT000
    printf("iMax[0]=%d  rMax[0]=%4d    iMax[1]=%d  rMax[1]=%4d \npDis=%d  w1=%d  w2=%d\n",
        iMax[0],rMax[0],iMax[1],rMax[1],pDis,w14ant,w24ant);
#endif



    //PwrTh[0] = 15; // detection threshold
    //PwrTh[1] = 15;




    #if Debug_PAPR_print
    {
             FILE *fp;

             fp = (FILE *) fopen( "PAPR_results_antenna.txt", "at+");
         fprintf( fp, "%d %d %d %d %d  %d %d %d %d %d %d\n", r[0] , r[1], r2[0] , r2[1], pLoc[0], pLoc[1],pLoc2[0],pLoc2[1],pBestAntInfo[0],pBestAntInfo[1],powerSearchWin[0]);

         fclose(fp);
    }
#endif

    return;
}

//-----------------------------------------------------------------------
#ifndef CEVA_INTRINSICS
//-------------------------------------------------------------------------------------------
/** @brief Averaging the powers for each sample from different antennas for PRACH preamble detection.
 *
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param AntPower        (INPUT) Array of pointers that points to the power arrays of different
 *                         antennas. <br/>
 *                          <br/>
 *  @param pPowerLevel    (INPUT) pointer to the array for storing the average power for each sample. <br/>
 *                          <br/>
 *  @return                None.
 *
 *
 * <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------
 *  |Name           |TCB Type      |Size         |Data Format |Valid Range |Default |Units |
 *  ----------------------------------------------------------------------------------------
 *  |AntPower       |Input         |4            |S32**       |Q31         |        |      |
 *  ----------------------------------------------------------------------------------------
 *  |pPowerLevel    |Output        |1            |S32*        |[0..65535]  |        |      |
 *  ----------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          | TotalPowerComb                    |
 *  --------------------------------------------------------
 *  |Inputs Stream       | AntPower                        |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *   This function averages the power for each sample from different antennas for the purpose of PRACH preamble
 *   detection.
 *
 *
 * <5> Cycle Count Formula
 *  TBD. <br/>
 *  <br/>
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
//-------------------------------------------------------------------------------------------
void TotalPowerComb(S32 **AntPower,  S32 *pPowerLevel)
{
    S16 j;

    for (j = 0; j < PRACH_IFFT_SIZE; j++)
    {
        pPowerLevel[j] = (S32) L_shr(L_add(AntPower[0][j], AntPower[1][j]), 1);
    }
}

#endif
//-------------------------------------------------------------------------------------------
/** @brief LTE base station receiver PRACH preamble detection.
 *
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param bsRx            (INPUT) pointer to the structure BS_RX. <br/>
 *                          <br/>
 *                          <br/>
 *  @param bsRxPrachDet    (INPUT) pointer to the structure BS_RX_PRACHDet. <br/>
 *                          <br/>
 *  @param MO              (INPUT) number of antenna. <br/>
 *                          <br/>
 *  @param adcbufPrach     (INPUT STREAM) pointer to the data to be processed. <br/>
 *                          <br/>
 *  @param length_adcPrach (INPUT) number of samples to be processed, for each antenna. <br/>
 *                          <br/>
 *  @param pTablesInCRAM   (INPUT STREAM) pointer to the tables to be used in PRACH preamble detection. <br/>
 *                          <br/>
 *  @param pbs_prach       (OUTPUT) pointer to the structure BS_PRACH. <br/>
 *                          <br/>
 *
 *  @return                None.
 *
 *
 * <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------
 *  |Name             |TCB Type      |Size                |Data Format    |Valid Range |Default |Units |
 *  ----------------------------------------------------------------------------------------------------
 *  |bsRx             |Input         |1                   |BS_RX          |            |        |      |
 *  ----------------------------------------------------------------------------------------------------
 *  |bsRxPrach        |Input         |1                   |BS_RX_PRACH    |            |        |      |
 *  ----------------------------------------------------------------------------------------------------
 *  |bsRxPrachDet     |Input         |1                   |BS_RX_PRACHDet |            |        |      |
 *  ----------------------------------------------------------------------------------------------------
 *  |MO               |Input         |1                   |S16            |[1..4]      |        |      |
 *  ----------------------------------------------------------------------------------------------------
 *  |adcbufPrach      |Input         |length_adcPrach*MO  |S16*           |Q15         |        |      |
 *  ----------------------------------------------------------------------------------------------------
 *  |length_adcPrach  |Input         |1                   |S32            |[0..30720]  |        |      |
 *  ----------------------------------------------------------------------------------------------------
 *  |pTablesInCRAM    |Input         |6720                |S16*           |Q15         |        |      |
 *  ----------------------------------------------------------------------------------------------------
 *  |pbs_prach_cram   |Output        |1                   |BS_PRACH       |            |        |      |
 *  ----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------------------------------------------------------
 *  |Block Name          | PRACH_Rx                                                                      |
 *  ------------------------------------------------------------------------------------------------------
 *  |Inputs Stream       | adcbufPrach                                                                   |
 *  ------------------------------------------------------------------------------------------------------
 *  |Config Parameters   | bsRx, bsRxPrach, bsRxPrachDet, length_adcPrach, MO, pTablesInCRAM             |
 *  ------------------------------------------------------------------------------------------------------
 *  |Status Parameters   | pbs_prach_cram                                                                |
 *  ------------------------------------------------------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *   Based on the system configuration parameters, this function initializes the structure
 *   for PRACH preamble detection.
 *
 *
 * <5> Cycle Count Formula
 *  TBD. <br/>
 *  <br/>
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.5.7, "Physical Channels and Modulation"<BR>
 *
 *
 * \ingroup group_lte_rx_uplink
 *
 **/
//-------------------------------------------------------------------------------------------
void PRACH_Rx(BS_RX *bsRx, BS_RX_PRACH *bsRxPrach, BS_RX_PRACHDet *bsRxPrachDet, S16 MO,
              S16 **adcbufPrach, S32 length_adcPrach, S16 *pTablesInCRAM, BS_PRACH *pbs_prach_cram, S16 *zczLUT,
              U16 prachDetThreashold)
{
    S32 i, k, iseq, mo, vLen, d = 0; // Ncsh
    S16 ifft_exp[2], ns = 0;
    S16 *pIFFTout;
    S32 *pPowerLevel;
    S16 *PrachScratch;
    S16 *pPRACH_Tables;
    S16 *p16;
    S16 *p16_cram;
    //S16 d_unit;
    //S32 Ncs;
    S32 *AntPower[4];
    S32 pBestAntInfo[3];
    S32 averagePower[4];
    S16 powerSearchWin[2];
    S16 preambleCnt;
    S32 temp_k = 0;
    S16 ifft_exp_diff;

     // MO = 1;  // Force to test one antenna
    // MO = 2;  // Force to test two antennas
    pbs_prach_cram->MO = MO;

#ifdef PRACH_SETUP_MEMORY

    // Setup memory
    //

#ifdef CEVA_INTRINSICS

    PrachScratch = (S16 *) &DataBuff_Bank1[0];
    // pPRACH_Tables = (S16 *) &(PrachScratch[31332]);  // Assign the location where the tables begin
    pPRACH_Tables = (S16 *) &(PrachScratch[31632]); // Assign the location where the tables begin

    //
    // The tables should be copied from CRAM to Ceva internal memory
    //
    MemCpyFromExt((void*) pPRACH_Tables, (void*) pTablesInCRAM, 6720*2);

#else

    PrachScratch = MemAlloc(38384 * sizeof(S16), CRAM_MEM, 0);
    // pPRACH_Tables = (S16 *) pTablesInCRAM;  //PRACH_Tables;
    // pPRACH_Tables = (S16 *) &(PrachScratch[31332]);  // Assign the location where the tables begin
    pPRACH_Tables = (S16 *) &(PrachScratch[31632]); // Assign the location where the tables begin
    for (i = 0; i < 6720; i++)
    {
        pPRACH_Tables[i] = pTablesInCRAM[i];
    }

#endif

    PrachFFTin = (S16 *) &(PrachScratch[0]); //[8192+32];
    SegmBuf = (S16 *) &(PrachScratch[24608+839*2]); //[2*4096];      // will also be used as 4096 FFT output
    eVecExt = (S16 *) &(PrachScratch[16416]); //[2*4096];  [2*(SEGMSIZE+EXTDSIZE)];
    CorrIn = (S16 *) &(PrachScratch[24608]); //[4*NZC2];
    CorrInExp = (S16 *) &(PrachScratch[31328]); //[4];

    //temp_buffer = (S32 *) &(PrachScratch[8224]); //[4];

    //BS_PRACH bs_prach;
    //PBS_PRACH pbs_prach = &bs_prach;
    // pbs_prach  = (PBS_PRACH) &(PrachScratch[31632]);
    pbs_prach = (PBS_PRACH) &(PrachScratch[31332]);

    eVec = (S16 *) &(PrachScratch[16416 + 2 * EXTDSIZE ]); //&(eVecExt[2*EXTDSIZE]);
    zEst = (S16 *) &(PrachScratch[0]); // 4K FFT output. Shared with FFT input;

    // Setup the tables
    //
    ZC839Base = (S16 *) &(pPRACH_Tables[0]);
    ZC839DFTk0 = (S16 *) &(pPRACH_Tables[3360]);
    ZC839MultInv = (S16 *) &(pPRACH_Tables[5040]);
    ZC839a1 = (S16 *) &(pPRACH_Tables[5880]);

#endif

    // Load the structure
    //
    k = sizeof(BS_PRACH) / 2;
    p16 = (S16 *) pbs_prach;
    p16_cram = (S16 *) pbs_prach_cram;
    for (i = 0; i < k; i++)
    {
        p16[i] = p16_cram[i];
    }
    for (i = 1; i <= 2 * pbs_prach->Nf; i++)
    {
        eVec[-i] = 0;
    }

    if (MO >= 1)
    {
        pbs_prach->iAntenna = 0;
        pbs_prach->iSegm = 0;
        pbs_prach->IdxDown = 0;
        pbs_prach->nStart = pbs_prach->dummy;
        CorrInExp[pbs_prach->iAntenna] = 9999;
        PRACH_Rx_1(pbs_prach, adcbufPrach[0]);
    }
    if (MO >= 2)
    {
        pbs_prach->iAntenna = 1;
        pbs_prach->iSegm = 0;
        pbs_prach->IdxDown = 0;
        pbs_prach->nStart = pbs_prach->dummy;
        CorrInExp[pbs_prach->iAntenna] = 9999;
        PRACH_Rx_1(pbs_prach, adcbufPrach[1]);
    }
    if (MO >= 3)
    {
        pbs_prach->iAntenna = 2;
        pbs_prach->iSegm = 0;
        pbs_prach->IdxDown = 0;
        pbs_prach->nStart = pbs_prach->dummy;
        CorrInExp[pbs_prach->iAntenna] = 9999;
        PRACH_Rx_1(pbs_prach, adcbufPrach[2]);
    }
    if (MO >= 4)
    {
        pbs_prach->iAntenna = 3;
        pbs_prach->iSegm = 0;
        pbs_prach->IdxDown = 0;
        pbs_prach->nStart = pbs_prach->dummy;
        CorrInExp[pbs_prach->iAntenna] = 9999;
        PRACH_Rx_1(pbs_prach, adcbufPrach[3]);
    }

    //
    // At this point, the received PRACH data are stored in the array CorrIn[].
#ifdef CEVA_INTRINSICS
    NormCorrIn_A(pbs_prach->MO, CorrInExp, CorrIn);
#else
    NormCorrIn(pbs_prach->MO, CorrInExp, CorrIn);
#endif

    pIFFTout = &(PrachFFTin[PRACH_IFFT_SIZE*2]);

    // Assign memory spaces for the pointers AntPower[]
    //
//    AntPower[0] = (S32 *) &(PrachScratch[6200]); // (S32 *) &(AntPowerBuf[0][10]);
//    AntPower[1] = (S32 *) &(PrachScratch[8300]); // (S32 *) &(AntPowerBuf[1][10]);
//    AntPower[2] = (S32 *) &(PrachScratch[10400]); // (S32 *) &(AntPowerBuf[2][10]);
//    AntPower[3] = (S32 *) &(PrachScratch[12500]); // (S32 *) &(AntPowerBuf[3][10]);

    AntPower[0] = (S32 *) &(PrachScratch[18320]); // (S32 *) &(AntPowerBuf[0][10]);
    AntPower[1] = (S32 *) &(PrachScratch[8192]); // (S32 *) &(AntPowerBuf[1][10]);
    AntPower[2] = (S32 *) &(PrachScratch[0]); // (S32 *) &(AntPowerBuf[2][10]);
    AntPower[3] = (S32 *) &(PrachScratch[0]); // (S32 *) &(AntPowerBuf[3][10]);

    // reset preamble cocunt to zero
    preambleCnt = 0;
    for (iseq = 0; iseq < pbs_prach->numRootSeq; iseq++)
    {
     // Locally generate DFTed cazac root sequence

    #ifdef CEVA_INTRINSICS
        Gen_ZC839DFT_A( pbs_prach->u[iseq], eVec );
    #else
        Gen_ZC839DFT(pbs_prach->u[iseq], eVec);
    #endif
    for (mo = 0; mo < MO; mo++)
    {

    #if Debug_data_print
      //
    {
    FILE *fp;
    S32 *pPowerLevel;
    pPowerLevel = AntPower[mo];
    if(mo == 0)
    {
        fp = (FILE *) fopen( "corr_after_norm1", "w");
    }
    else
    {
         fp = (FILE *) fopen( "corr_after_norm2", "w");
    }
    for ( i=0; i < 839; i++ )
    {
         fprintf( fp, "%d %d\n",  CorrIn[2 * i], CorrIn[2 * i+1]);
    }
    fclose(fp);
    }
    #endif
#ifdef CEVA_INTRINSICS
        ComputeComplexConjMult_A(&(CorrIn[mo*NZC2]), eVec, PrachFFTin, NZC);
#else
        ComputeCmplxConjMult(&(CorrIn[mo * NZC2]), eVec, PrachFFTin, NZC);
#endif

#if Debug_data_print
    {
        FILE *fp;
        S32 *pPowerLevel;
        pPowerLevel = AntPower[mo];
        if(mo == 0)
        {
            fp = (FILE *) fopen( "corr_freq_domain1", "w");
        }
        else
        {
             fp = (FILE *) fopen( "corr_freq_domain2", "w");
        }
        for ( i=0; i < 839; i++ )
        {
             fprintf( fp, "%d %d\n",  PrachFFTin[2 * i], PrachFFTin[2 * i+1]);
        }
        fclose(fp);
    }
#endif

    for (i = NZC; i < PRACH_IFFT_SIZE; i++)
    {
        PrachFFTin[2 * i] = 0;
        PrachFFTin[2 * i + 1] = 0;
    }
     // Do 2048-pt IFFT --> resampling the 839-pt time domain data into 2048 points.
#ifdef CEVA_INTRINSICS
    // please note, a 16k bytes temp memory is needed for assembly code, the memeory starting from physical address 8224*2 bytes
    cifft_16t32d(PrachFFTin, (S16)11 , pIFFTout, &(ifft_exp[mo]));
   //  cifft_16t32d(PrachFFTin, (S16)11 , pIFFTout, &(ifft_exp));
#else
    // 2048-pt IFFT, oversampling 839-pt time domain data
    cifft_16t32d_case2_2(PrachFFTin, (S16) 11, pIFFTout,  &(ifft_exp[mo]));
   //  cifft_16t32d_case2_2(PrachFFTin, (S16) 11, pIFFTout,  &(ifft_exp));
#endif

    //ifft_exp = add(ifft_exp,1);
    //
    for (i = 0; i < 4096; i++)
    {
    #ifndef CEVA_INTRINSICS
        pIFFTout[i] = shift_r(pIFFTout[i], -1);
    #else
        pIFFTout[i] = shr_r(pIFFTout[i], 1);
    #endif       
    }



     // calcualte power and average power of each antenna
#ifdef CEVA_INTRINSICS
    averagePower[mo] = AccPowerLevel_A(pbs_prach, AntPower[mo], pIFFTout, PRACH_IFFT_SIZE);
#else
    averagePower[mo] = AccPowerLevel(pbs_prach, AntPower[mo], pIFFTout, PRACH_IFFT_SIZE);
#endif

    } //mo

    // norminalize the power from two antennas
    
    if(MO > 1)
    {
    if(ifft_exp[0] > ifft_exp[1]) 
    {
        #ifndef CEVA_INTRINSICS
            ifft_exp_diff = shift_r(ifft_exp[0] - ifft_exp[1],1);
        #else
            ifft_exp_diff = shr_r(ifft_exp[0] - ifft_exp[1],-1);
        #endif 

       for (i = 0; i < 2048; i++)
        {
        #ifndef CEVA_INTRINSICS
            AntPower[1][i] = L_shift_r(AntPower[1][i], -ifft_exp_diff);
        #else
            AntPower[1][i] = L_shr_r(AntPower[1][i], ifft_exp_diff);
        #endif       
        }

        #ifndef CEVA_INTRINSICS
            averagePower[1] = L_shift_r(averagePower[1], -ifft_exp_diff);
        #else
            averagePower[1] = L_shr_r(averagePower[1], ifft_exp_diff);
        #endif
    }
    else if (ifft_exp[0] < ifft_exp[1])
    {
        #ifndef CEVA_INTRINSICS
            ifft_exp_diff = shift_r(ifft_exp[1] - ifft_exp[0],1);
        #else
            ifft_exp_diff = shr_r(ifft_exp[1] - ifft_exp[0],-1);
        #endif 

       for (i = 0; i < 2048; i++)
        {
        #ifndef CEVA_INTRINSICS
            AntPower[0][i] = L_shift_r(AntPower[0][i], -ifft_exp_diff);
        #else
            AntPower[0][i] = L_shr_r(AntPower[0][i], ifft_exp_diff);
        #endif       
        }

       #ifndef CEVA_INTRINSICS
            averagePower[0] = L_shift_r(averagePower[0], -ifft_exp_diff);
        #else
            averagePower[0] = L_shr_r(averagePower[0], ifft_exp_diff);
        #endif
    }
    }

    pPowerLevel = (S32 *) &(PrachFFTin[32]); // Reserve 8xS32 space infront for wrap-around

    // Combine the time-donain power levels from all antenna

    if(MO == 1)
    {
        pPowerLevel = AntPower[0];
    }
    else
    {
        #ifdef CEVA_INTRINSICS
            TotalPowerComb_A(AntPower, pPowerLevel);
        #else
            TotalPowerComb(AntPower, pPowerLevel);
        #endif
    }


    // average power among two antennas
   if(MO == 1)
       pBestAntInfo[2] = averagePower[0];
   else
       pBestAntInfo[2] = (S32) L_shr(L_add(averagePower[0], averagePower[1]), 1);

    // for each zero correlation zone

    vLen = pbs_prach->vLen;

    // LUT for different Ncs values
    // search each zero correlation zone
    for(i = 0; (i < vLen) && (preambleCnt < 64); i++)
    {
        powerSearchWin[0] = zczLUT[i+1] - 4;  // zero correlation zone left window
        powerSearchWin[1] = zczLUT[i] - 6; // zero correlation zone right window
        CombinePower(MO, AntPower, pBestAntInfo, averagePower, powerSearchWin);
        PrachPeakSearching(pbs_prach, preambleCnt, zczLUT[i+1], (S16) iseq, pPowerLevel, pBestAntInfo,
                           powerSearchWin, prachDetThreashold);

        preambleCnt ++;
    }

   } // root number

    bsRxPrachDet->bs_prachDet_msDet.num = pbs_prach->nPeakDetected;
    bsRxPrach->bs_prach_TseqMicrosec = 800; //for PRACH Preamble format 0

    for (i = 0; i < pbs_prach->nPeakDetected; i++)
    {
        iseq = pbs_prach->iseqPeakDetected[i];
        //Ncs = pbs_prach->Ncs;
        // Ncsh = pbs_prach->Ncs[iseq] / 2;
       // Ncsh = pbs_prach->Ncs / 4; // make preamble boundary to be (-3*Ncs/4 .. +Ncs/4)
        //Ncsh = 3;
        vLen = pbs_prach->vLen;
        if (pbs_prach->Ncs == 0)
        {
            //Ncs = 840;
            //Ncsh = 420;
            vLen = 1;
        }

        k = pbs_prach->locPeakDetected[i];
        temp_k = k;

   #if Debug_Timing_Est_print
        {
         // printf timing offset estimation
        FILE *fp_PRACH_tming;
        fp_PRACH_tming = (FILE *) fopen("PRACH_test_timing_estimation_raw_data.dat", "at+");
        fprintf(fp_PRACH_tming, "%d %d %d\n", temp_k, pbs_prach->searchWinPeakDetected[i], pbs_prach->searchWinPeakDetected[i] + 1 - temp_k);
        fclose(fp_PRACH_tming);
        }
   #endif



        bsRxPrachDet->bs_prachDet_msDet.cyclicShift[i] = (S16) (pbs_prach->preambleCntPeakDetected[i]);
        bsRxPrachDet->bs_prachDet_msDet.peakToAvgPowerRatio[i] = pbs_prach->prachDetPHR[i];

        // Conform to sign and upper bound per 3GPP 36.213 Section 4.2.3


        // norm to 16 Ts
        // (800/2048)/(16/30.720) = 3/4; since the symbol arrives later than the timing boundary is better than the symbol arrives earlier than the timing boundary
        // we are working on small cells, so floor(0.75*x)

        if(pbs_prach->preambleCntPeakDetected[i] == 0)
            ns = (S16) (-temp_k);
        else
            ns = (S16) (pbs_prach->searchWinPeakDetected[i] + 1 - temp_k);

        if (ns < 0)
        {
            ns = -ns;
        }
        else
        {
            ns = 0;
        }

        d = 3*ns;
        ns = shr((S16) d,2);

        if (ns > 1282)
        {
            ns = 1282;
        }


        bsRxPrachDet->bs_prachDet_msDet.tDelMicrosecEst[i] = ns; //store estimated microsec delay

   #if Debug_Timing_Est_print
        {
         // printf timing offset estimation
        FILE *fp_PRACH_tming;
        fp_PRACH_tming = (FILE *) fopen("PRACH_test_timing_estimation_final_results.dat", "at+");
        fprintf(fp_PRACH_tming, "%d %d\n", bsRxPrachDet->bs_prachDet_msDet.cyclicShift[i], bsRxPrachDet->bs_prachDet_msDet.tDelMicrosecEst[i]);
        fclose(fp_PRACH_tming);
        }
   #endif
    }

#ifdef CEVA_INTRINSICS
    //k = sizeof(BS_PRACH);
    //MemCpyToExt((void *) pbs_prach_cram, (void *) pbs_prach, k);
#else
    k = sizeof(BS_PRACH) / 2;
    p16 = (S16 *) pbs_prach;
    p16_cram = (S16 *) pbs_prach_cram;
    for (i = 0; i < k; i++)
    {
        p16_cram[i] = p16[i];
    }

#endif

    //
#ifndef CEVA_INTRINSICS
    if(pbs_prach->nPeakDetected == 1)
        printf("nPeakDetected = %d\n", pbs_prach->nPeakDetected);
    else
        printf("nPeakDetected = %d\n", pbs_prach->nPeakDetected);

    for (i = 0; i < pbs_prach->nPeakDetected; i++)
    {
        printf("%3d  iseq= %3d  loc= %3d   cyclicShift= %2d   delay (16Ts) = %d  phr = %d\n", i,
               pbs_prach->iseqPeakDetected[i], pbs_prach->locPeakDetected[i],
               bsRxPrachDet->bs_prachDet_msDet.cyclicShift[i],
               bsRxPrachDet->bs_prachDet_msDet.tDelMicrosecEst[i],
               bsRxPrachDet->bs_prachDet_msDet.peakToAvgPowerRatio[i]);
    }

#ifdef DEBUG_PRINT
#if PRACH_BER_TEST
    if (MO > 1)
    {
        SavePrachDetectionResult(0, pbs_prach->nPeakDetected, pbs_prach->iseqPeakDetected,
            pbs_prach->locPeakDetected, bsRxPrachDet->bs_prachDet_msDet.cyclicShift,
            bsRxPrachDet->bs_prachDet_msDet.tDelMicrosecEst );
    }
#endif  //PRACH_BER_TEST
#endif  //DEBUG_PRINT
#endif
    //

#ifdef PRACH_SETUP_MEMORY
#ifndef CEVA_INTRINSICS
    MemFree(PrachScratch, CRAM_MEM);
#endif
#endif
}

