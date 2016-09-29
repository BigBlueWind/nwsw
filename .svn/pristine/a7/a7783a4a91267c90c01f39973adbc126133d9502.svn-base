//-------------------------------------------------------------------------------------------
/** @debug.c
 *
 * @brief Print debug information
 * @author Mindspeed Technologies
 * @version $Revision: 1.27 $
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
#include <stdarg.h>

#include "typedef.h"
#include "prototypes.h"
#include "Debug.h"

/*------------------------------------------------------------------------------------*
 *
 * START - DEBUG TRACE VARIABLES
 *
 *------------------------------------------------------------------------------------*/

#ifdef DEBUG_PRINT
//  Expo [][][0] - FFT
//  Expo [][][1] - ChEst
//  Expo [][][2] - MRC
//  Expo [][][3] - FFT - norm
//  Expo [][][4] - ChEst - norm
//  Expo [][][5] - MRC ChOut 
//  Expo [][][6] - IDFT Out 
short debugExpo[14][4][10] =
{   0};
short numDebugExpo = 0;
#endif

short deci[MAX_OUTPUT_BLK_SIZE];

FILE *f_data1 = NULL; /* Decision output  */

#ifdef DEBUG_PRINT
FILE *f_data_codeword0;
FILE *f_data_codeword1;

FILE *f_data_feca0;
FILE *f_data_feca1;
FILE *f_data_fecb0;
FILE *f_data_fecb1;
FILE *f_data_fecc0;
FILE *f_data_fecc1;
FILE *f_data_fecd0;
FILE *f_data_fecd1;
FILE *f_data_fecf0;
FILE *f_data_fecf1;

FILE *f_data_mod0_b0;
FILE *f_data_mod1_b_0;

FILE *f_data_cfi_mod0_b0;
FILE *f_data_cfi_mod1_b_0;
FILE *f_data_ctl_mod0_b0;
FILE *f_data_ctl_mod1_b_0;

FILE *f_data_mod2_d0;

FILE *f_data_mod0_b1;
FILE *f_data_mod1_b_1;
FILE *f_data_mod2_d1;

FILE *f_data_mod3_x[4];
FILE *f_data_mod4_y[4];

FILE *f_data_k_l;
FILE *f_data_k_l_dec;

FILE *f_data_mod5_unscaled_a[4];
FILE *f_data_mod5a[4];
FILE *f_data_imod5a[4];
FILE *f_data_mod5c_ctl_a[4];
FILE *f_data_mod5d_bcst_a[4];
FILE *f_data_mod5e_sync_a;
FILE *f_data_mod5f_a[4];
FILE *f_data_mod_bfps_a[4];
FILE *f_data_mod6_s;
FILE *f_data_i16_mod6_s;

//DL/UL RX

FILE *f_data_demod5a_Nfft_a[4];
FILE *f_data_demod5_a[4];

FILE *f_data_demod_data_b_all;
FILE *f_data_demod_data_b_0;

//UL RX

FILE *f_data_demod4_z[4];
FILE *f_data_demod1_u0_b;

#endif

////////////////////////////////////////////////////
//Uplink Rx debug files used to for comparing
//fixed point and floating point
////////////////////////////////////////////////////
#ifdef _WIN32
FILE *fFFT_In;
FILE *fFFT_Out;
FILE *fTcbIOBuf;
FILE *fAdet;
FILE *fUl_Pilots;
FILE *fUl_PilotsPucch;
FILE *fChanEst_pucch;
FILE *fChanEst_p0;
FILE *fChanEst_p1;
FILE *fChanEst_p2;
FILE *fChanEst_p3;
FILE *fChanEst_p4;
FILE *fChanEst_p5;
FILE *fMmseNoiseVarAvg;
FILE *fChanEstIn;
FILE *fChanEstStage1;
FILE *fChanEstStage2;
FILE *fTiming;
FILE *fDerotation;
FILE *fFrequency;
FILE *fChanEst_BeforeNoiseAvg;
FILE *fNoiseAvg_p1;
FILE *fNoiseAvg_p2;
FILE *fNoiseAvg_p3;
FILE *fNoiseAvg_p4;
FILE *fSnrPerRb;
FILE *fSrs_Snr_PerRB;
FILE *fSnr;
FILE *fSnr_Calc;
FILE *fMrc_Out;
FILE *fMrc_ChanPower;
FILE *fMrc_Out_pucch;
FILE *fMrc_ChanPower_pucch;
FILE *fIdft_In;
FILE *fRxMapperIn;
FILE *fRxMapperOut;
FILE *fRxMapperIn_pucch;
FILE *fRxMapperOut_pucch;

FILE *fecout;
FILE *fCtrlOutput;
FILE *f_bits_out;
FILE *fBsSinr;
FILE *fBsFreqOffset;
#endif

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   *fInput     - Input file name
 *  @param   *inputArray - Input array to be stored to file name
 *  @param   size        - length of array to be stored    
 *  @param   *datatype   - DataType of Input array
 *  @param   *type       - Real or Complex input array
 *  @param   scale       - If we wish to convert fixed point to floating point, use this scale factor
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range   |Units         |
 *  |               |Standard       |             |               |              |          |              |              |
 *  -----------------------------------------------------------------------------------------------------------------------
 *  |*fileName      |               |pInput       |               |char          |          |              |              |
 *  -----------------------------------------------------------------------------------------------------------------------
 *  |*inputArray    |               |pInput       |               |char          |          |              |              |
 *  -----------------------------------------------------------------------------------------------------------------------
 *  |size           |               |Input        |               |Real          |          |              |              |
 *  -----------------------------------------------------------------------------------------------------------------------
 *  |*datatype      |               |pInput       |               |char          |          |              |              |
 *  -----------------------------------------------------------------------------------------------------------------------
 *  |*type          |               |pInput       |               |char          |          |              |              |
 *  -----------------------------------------------------------------------------------------------------------------------
 *  |*mode          |               |pInput       |               |char          |          |              |              |
 *  -----------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |filePrint      |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void filePrint(FILE *fInput, void *inputArray, int size, char *datatype, char *type,
               double scale, int printEnable)
{
#ifdef DEBUG_PRINT
    int i;
    S8 *pS8Input;
    U8 *pU8Input;
    S16 *pS16Input;
    U16 *pU16Input;
    S32 *pS32Input;
    U32 *pU32Input;
    double *pDoubleInput;
    float *pFloatInput;

    if (printEnable)
    {
        if (strcmp(datatype,"S8") == 0)
        {
            pS8Input = (S8*)inputArray;
            for(i = 0; i < size; i++)
            {
                if (strcmp(type,"real") == 0)
                    fprintf(fInput,"%10d\n", pS8Input[i]);
                else if (strcmp(type,"Complex") == 0)
                    fprintf(fInput,"%10d %10d\n", pS8Input[2*i], pS8Input[2*i + 1]);
                else if (strcmp(type,"Fix2Fl_real") == 0)
                    fprintf(fInput,"%10.6f\n", (double)((double)(pS8Input[i])/scale + 0.000005));
                else if (strcmp(type,"Fix2Fl_Complex") == 0)
                    fprintf(fInput,"%10.6f %10.6f\n", (double)((double)(pS8Input[2*i])/scale + 0.000005),
                                (double)((double)(pS8Input[2*i+1])/scale + 0.000005));
                else
                printf("Only input types 'real','Complex','Fix2Fl_real',Fix2Fl_Complex' supported for S8 input\n");
            }
        }
        else if (strcmp(datatype,"U8") == 0)
        {
            pU8Input = (U8*)inputArray;
            for(i = 0; i < size; i++)
            {
                if (strcmp(type,"real") == 0)
                    fprintf(fInput,"%10d\n", pU8Input[i]);
                else if (strcmp(type,"Complex") == 0)
                    fprintf(fInput,"%10d %10d\n", pU8Input[2*i], pU8Input[2*i + 1]);
                else if (strcmp(type,"Fix2Fl_real") == 0)
                    fprintf(fInput,"%10.6f\n", (double)((double)(pU8Input[i])/scale + 0.000005));
                else if (strcmp(type,"Fix2Fl_Complex") == 0)
                    fprintf(fInput,"%10.6f %10.6f\n", (double)((double)(pU8Input[2*i])/scale + 0.000005), 
                                (double)((double)(pU8Input[2*i+1])/scale + 0.000005));
                else
                printf("Only input types 'real','Complex','Fix2Fl_real',Fix2Fl_Complex' supported for U8 input\n");
            }
        }
        else if (strcmp(datatype,"S16") == 0)
        {
            pS16Input = (S16*)inputArray;
            for(i = 0; i < size; i++)
            {
                if (strcmp(type,"real") == 0)
                    fprintf(fInput,"%10d\n", pS16Input[i]);
                else if (strcmp(type,"Complex") == 0)
                    fprintf(fInput,"%10d %10d\n", pS16Input[2*i], pS16Input[2*i + 1]);
                else if (strcmp(type,"Fix2Fl_real") == 0)
                    fprintf(fInput,"%10.6f\n", (double)((double)(pS16Input[i])/scale + 0.000005));
                else if (strcmp(type,"Fix2Fl_Complex") == 0)
                    fprintf(fInput,"%10.6f %10.6f\n", (double)((double)(pS16Input[2*i])/scale + 0.000005), 
                                (double)((double)(pS16Input[2*i+1])/scale + 0.000005));
                else
                printf("Only input types 'real','Complex','Fix2Fl_real',Fix2Fl_Complex' supported for S16 input\n");
            }
        }
        else if (strcmp(datatype,"U16") == 0)
        {
            pU16Input = (U16*)inputArray;
            for(i = 0; i < size; i++)
            {
                if (strcmp(type,"real") == 0)
                    fprintf(fInput,"%10d\n", pU16Input[i]);
                else if (strcmp(type,"Complex") == 0)
                    fprintf(fInput,"%10d %10d\n", pU16Input[2*i], pU16Input[2*i + 1]);
                else if (strcmp(type,"Fix2Fl_real") == 0)
                    fprintf(fInput,"%10.6f\n", (double)((double)(pU16Input[i])/scale + 0.000005));
                else if (strcmp(type,"Fix2Fl_Complex") == 0)
                    fprintf(fInput,"%10.6f %10.6f\n", (double)((double)(pU16Input[2*i])/scale + 0.000005), 
                                (double)((double)(pU16Input[2*i+1])/scale + 0.000005));
                else
                printf("Only input types 'real','Complex','Fix2Fl_real',Fix2Fl_Complex' supported for U16 input\n");
            }
        }
        else if (strcmp(datatype,"S32") == 0)
        {
            pS32Input = (S32*)inputArray;
            for(i = 0; i < size; i++)
            {
                if (strcmp(type,"real") == 0)
                    fprintf(fInput,"%10d\n", pS32Input[i]);
                else if (strcmp(type,"Complex") == 0)
                    fprintf(fInput,"%10d %10d\n", pS32Input[2*i], pS32Input[2*i + 1]);
                else if (strcmp(type,"Fix2Fl_real") == 0)
                    fprintf(fInput,"%10.6f\n", (double)((double)(pS32Input[i])/scale + 0.000005));
                else if (strcmp(type,"Fix2Fl_Complex") == 0)
                    fprintf(fInput,"%10.6f %10.6f\n", (double)((double)(pS32Input[2*i])/scale + 0.000005), 
                                (double)((double)(pS32Input[2*i+1])/scale + 0.000005));
                else
                printf("Only input types 'real','Complex','Fix2Fl_real',Fix2Fl_Complex' supported for S32 input\n");
            }
        }
        else if (strcmp(datatype,"U32") == 0)
        {
            pU32Input = (U32*)inputArray;
            for(i = 0; i < size; i++)
            {
                if (strcmp(type,"real") == 0)
                    fprintf(fInput,"%10d\n", pU32Input[i]);
                else if (strcmp(type,"Complex") == 0)
                    fprintf(fInput,"%10d %10d\n", pU32Input[2*i], pU32Input[2*i + 1]);
                else if (strcmp(type,"Fix2Fl_real") == 0)
                    fprintf(fInput,"%10.6f\n", (double)((double)(pU32Input[i])/scale + 0.000005));
                else if (strcmp(type,"Fix2Fl_Complex") == 0)
                    fprintf(fInput,"%10.6f %10.6f\n", (double)((double)(pU32Input[2*i])/scale + 0.000005), 
                                (double)((double)(pU32Input[2*i+1])/scale + 0.000005));
                else
                printf("Only input types 'real','Complex','Fix2Fl_real',Fix2Fl_Complex' supported for U32 input\n");
            }
        }
        else if (strcmp(datatype,"double") == 0)
        {
            pDoubleInput = (double*)inputArray;
            for(i = 0; i < size; i++)
            {
                if (strcmp(type,"real") == 0)
                    fprintf(fInput,"%10.6f\n", pDoubleInput[i]);
                else if (strcmp(type,"Complex") == 0)
                    fprintf(fInput,"%10.6f %10.6f\n", pDoubleInput[2*i], pDoubleInput[2*i + 1]);
                else
                printf("Only input types 'real' and 'Complex' supported for double input\n");
            }
        }
        else if (strcmp(datatype,"float") == 0)
        {
            pFloatInput = (float*)inputArray;
            for(i = 0; i < size; i++)
            {
                if (strcmp(type,"real") == 0)
                    fprintf(fInput,"%10.6f\n", pFloatInput[i]);
                else if (strcmp(type,"Complex") == 0)
                    fprintf(fInput,"%10.6f %10.6f\n", pFloatInput[2*i], pFloatInput[2*i + 1]);
                else
                printf("Only input types 'real' and 'Complex' supported for float input\n");
            }
        }
        else
        {
            printf("Only 'S8', 'U8', 'S16', 'U16', 'S32', 'U32', 'double', 'float' data types supported\n");
        }
    }
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   *fInput     - Input file 
 *  @param   *inputArray - Input array to be stored to file name
 *  @param   size        - length of array to be stored    
 *  @param   *datatype   - DataType of Input array
 *  @param   *type       - Real or Complex input array
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range   |Units         |
 *  |               |Standard       |             |               |              |          |              |              |
 *  -----------------------------------------------------------------------------------------------------------------------
 *  |*fileName      |               |pInput       |               |char          |          |              |              |
 *  -----------------------------------------------------------------------------------------------------------------------
 *  |*inputArray    |               |pInput       |               |char          |          |              |              |
 *  -----------------------------------------------------------------------------------------------------------------------
 *  |size           |               |Input        |               |Real          |          |              |              |
 *  -----------------------------------------------------------------------------------------------------------------------
 *  |*datatype      |               |pInput       |               |char          |          |              |              |
 *  -----------------------------------------------------------------------------------------------------------------------
 *  |*type          |               |pInput       |               |char          |          |              |              |
 *  -----------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |filePrint      |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void filePrintSingle(FILE *fInput, char *datatype, char *type, double scale, int printEnable,
                     U16 numargs, ...)
{
#ifdef DEBUG_PRINT
    va_list arguments;

    S8 S8Input;
    U8 U8Input;
    S16 S16Input;
    U16 U16Input;
    S32 S32Input;
    U32 U32Input;
    double doubleInput;
    float floatInput;

    if (printEnable)
    {
        /* Initializing arguments to store all values after numargs */
        va_start(arguments, numargs);

        if (strcmp(datatype,"S8") == 0)
        {
            while ((numargs--) != 0)
            {
                S8Input = (S8)va_arg(arguments, S8);
                if (strcmp(type,"Fix2Fl_real") == 0)
                    fprintf(fInput,"%10.6f ", (double)((double)(S8Input)/scale + 0.000005));
                else
                    fprintf(fInput,"%10d ", S8Input);
            }
            fprintf(fInput,"\n");
        }
        else if (strcmp(datatype,"U8") == 0)
        {
            while ((numargs--) != 0)
            {
                U8Input = (U8)va_arg(arguments, U8);
                if (strcmp(type,"Fix2Fl_real") == 0)
                    fprintf(fInput,"%10.6f ", (double)((double)(U8Input)/scale + 0.000005));
                else
                    fprintf(fInput,"%10d ", U8Input);
            }
            fprintf(fInput,"\n");
        }
        else if (strcmp(datatype,"S16") == 0)
        {
            while ((numargs--) != 0)
            {
                S16Input = (S16)va_arg(arguments, S16);
                if (strcmp(type,"Fix2Fl_real") == 0)
                    fprintf(fInput,"%10.6f ", (double)((double)(S16Input)/scale + 0.000005));
                else
                    fprintf(fInput,"%10d ", S16Input);
            }
            fprintf(fInput,"\n");
        }
        else if (strcmp(datatype,"U16") == 0)
        {
            while ((numargs--) != 0)
            {
                U16Input = (U16)va_arg(arguments, U16);
                if (strcmp(type,"Fix2Fl_real") == 0)
                    fprintf(fInput,"%10.6f ", (double)((double)(U16Input)/scale + 0.000005));
                else
                    fprintf(fInput,"%10d ", U16Input);
            }
            fprintf(fInput,"\n");
        }
        else if (strcmp(datatype,"S32") == 0)
        {
            while ((numargs--) != 0)
            {
                S32Input = (S32)va_arg(arguments, S32);
                if (strcmp(type,"Fix2Fl_real") == 0)
                    fprintf(fInput,"%10.6f ", (double)((double)(S32Input)/scale + 0.000005));
                else
                    fprintf(fInput,"%10d ", S32Input);
            }
            fprintf(fInput,"\n");
        }
        else if (strcmp(datatype,"U32") == 0)
        {
            while ((numargs--) != 0)
            {
                U32Input = (U32)va_arg(arguments, U32);
                if (strcmp(type,"Fix2Fl_real") == 0)
                    fprintf(fInput,"%10.6f ", (double)((double)(U32Input)/scale + 0.000005));
                else
                    fprintf(fInput,"%10d ", U32Input);
            }
            fprintf(fInput,"\n");
        }
        else if (strcmp(datatype,"double") == 0)
        {
            while ((numargs--) != 0)
            {
                doubleInput = (double)va_arg(arguments, double);
                fprintf(fInput,"%10.6f ", doubleInput);
            }
            fprintf(fInput,"\n");
        }
        else if (strcmp(datatype,"float") == 0)
        {
            while ((numargs--) != 0)
            {
                floatInput = (float)va_arg(arguments, float);
                fprintf(fInput,"%10.6f ", floatInput);
            }
            fprintf(fInput,"\n");
        }
        else
        {
            printf("Only 'S8', 'U8', 'S16', 'U16', 'S32', 'U32', 'double', 'float' data types supported\n");
        }

        va_end(arguments);
    }
#endif
}

void phy_fprintf( void *fStream, int printEnable, const char *format, ...)
{
#ifdef DEBUG_PRINT
    va_list argptr;

    if (printEnable)
    {
        /* Initializing pointer to arguments */
        va_start(argptr, format);
        if (!fStream)
        {
            phy_printf("File is not open for phy_fprintf, fStream = %d\n", fStream);
        }
        else
        {
            if (vfprintf((FILE *)fStream, format, argptr) < 0)
                phy_printf("Unable to write to File for phy_fprintf\n");
        }

        //Clear memory here
        va_end(argptr);
    }
#endif
}