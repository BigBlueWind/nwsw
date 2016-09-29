//-------------------------------------------------------------------------------------------
/** @file bs_tx_Precoder.c
 *
 * @brief LTE Base Station Transmit Precoder Implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.35 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#include <asm-insn.h>
#else
#include "basic_op_cc.h"
#endif

//-------------------------------------------------------------------------------------------
/** @brief Base Station, Tx, Map blocks of data from "Layers" to "Antenna Ports"
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param InParam     = INPUT, Structure containing all config parameters for Precoder
 *  @param x           = INPUT, Input data from LayerMapper
 *  @param y           = OUTPUT, Pointer to output of Precoder
 *  @param length_y    = OUTPUT, length of output vector, y
 *  @param Mlayer_symb = INPUT, number of layers
 *
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction|Length     |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |         |           |              |            |            |      |
 *  ---------------------------------------------------------------------------------------------
 *  |InParam    |         |Input    |1          |Structure     |            |            |      |
 *  ---------------------------------------------------------------------------------------------
 *  |x          |         |pInput   |Mlayer_symb|pComplex Array|(1:1:14)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------
 *  |y          |         |pOutput  |length_y   |pComplex Array|(1:1:14)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------
 *  |length_y   |         |pOutput  |1          |Real          |(1:31:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------
 *  |Mlayer_symb|         |Input    |1          |Real          |(1:15:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------
 *  |Block Name          |precoder           |
 *  ------------------------------------------
 *  |Input Streams       |x                  |
 *  ------------------------------------------
 *  |Output Streams      |y                  |
 *  ------------------------------------------
 *  |Config Parameters   |InParam,Mlayer_symb|
 *  ------------------------------------------
 *  |Status Parameters   |length_y           |
 *  ------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 *  Map blocks of data from "Layers" to "Antenna Ports"
 *
 * <5> Cycle Count Formula:
 * For antennas=1, Layer=1:
 *   81 + Nsymbols
 * For antennas=2, Layer=2:
 *   TXDIVERSITY--> 198 + 6*Nsymbols
 *   STC        -->  12 + 0.5*Nsymbols
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel 8 Sec.6.3.4, "Downlink: Precoding"
 *
 * \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void precoder(PRECODERPARAM InParam, S16 *x, S16 *y, S32 *length_y, S32 Mlayer_symb)
{
    S32 i, j;
    S32 Map_symb;
    S16 *W_ptr;
    S16 w_dim1, w_dim2; // dim1 # of rows, dim2 # of columns
    S16 wdu_dim1, wdu_dim2;
    S16 x_dim2;
    S16 y_dim1;
    S16 CodebookIndex;
    S16 *pWPtr[4];

    CodebookIndex = (S16) (InParam.codeBookIdx);
    if (InParam.transmissionMode == SINGLEANT)
    {
        //Precoding for Transmission on a single antenna port
        if ((InParam.nLayers == 1) && (InParam.Ntx_antennas == 1))
        {
            Map_symb = Mlayer_symb;

            j = L_add(Map_symb, Map_symb);
            i = L_add(j, j); // shorts --> bytes
            memcpy(y, x, i);

            *length_y = Map_symb;
        }
        else
        {
            myprintf("error in precoder(): Invalid V~=1, For P==1");
        }
    }
    else if ((InParam.transmissionMode == OPNLPSPMUX) || (InParam.transmissionMode == CLSLPSPMUX))
    {
        //Precoding for Spatial Multiplexing
        if ((InParam.Ntx_antennas == 2) || (InParam.Ntx_antennas == 4))
        {
            if (InParam.cddType == NOCDD)
            {
                //Zero and Small Delay, Cyclic Delay Diversity
                //
                // | y(0,index)   |                        | x(0,index)   |
                // | y(1,index)   | = D(k(index)).W(index).| x(1,index)   |
                // |     .        |                        |     .        |
                // |     .        |                        | x(V-1,index) |
                // | y(P-1,index) |
                //
                //      P x 1       =    P x P   . P x V .   V x 1          = P x 1
                //
                // Cycle count:
                // -----------
                // - for raw matrix multiplies as shown,
                // - Inner loop total of P x P x V + P x V x 1 = 4x4x4 + 4x4 = 80 complex multiplies /data subcarrier, worst case for 4 antennas, 4 layers
                // - Assume 1 complex multiply per cycle in a 4-MAC unit DSP device
                // - Assume 20% overhead above inner loop MIPS for the complete function
                // - So, total 80 x 1.2 x Nused x Fsym cycles/sec, (for Nused data subcarriers, Fsym symbols/sec)
                //
                Map_symb = Mlayer_symb;

                //determine the frequency_symbol index in resource grid that each complex constellation point for this physical channel will be mapped to
                if (InParam.Ntx_antennas == 2)
                {
                    //2 antennas:-

                    //(1) Point to matrix W(index) = W, since no parameter in terms of index
                    if (InParam.nLayers == 1)
                    {
                        W_ptr = CA_PrecCodebookP2V1 + 2 * 2 * CodebookIndex;
                        w_dim1 = 2;
                        w_dim2 = 1;
                    }
                    else // pChan->chi.nLayers==2
                    {
                        W_ptr = CA_PrecCodebookP2V2 + 2 * 2 * 2 * CodebookIndex;
                        w_dim1 = 2;
                        w_dim2 = 2;
                    }
                }
                else
                {
                    // pPhy->bs_Ntx_antennas==4
                    //4 antennas:-
                    //(1) Point to matrix W(index) = W(codebookIndex,numTxLayers) = independent of sample location "index"
                    if (InParam.nLayers == 1)
                    {
                        W_ptr = CA_PrecCodebookP4V1W + 2 * 4 * CodebookIndex; // 4x1
                        w_dim1 = 4;
                        w_dim2 = 1;
                    }
                    else if (InParam.nLayers == 2)
                    {
                        W_ptr = CA_PrecCodebookP4V2W + 2 * 4 * 2 * CodebookIndex; // 4x2
                        w_dim1 = 4;
                        w_dim2 = 2;
                    }
                    else if (InParam.nLayers == 3)
                    {
                        W_ptr = CA_PrecCodebookP4V3W + 2 * 4 * 3 * CodebookIndex; // 4x3
                        w_dim1 = 4;
                        w_dim2 = 3;
                    }
                    else//pChan->chi.nLayers==4
                    {
                        W_ptr = CA_PrecCodebookP4V4W + 2 * 4 * 4 * CodebookIndex; // 4x4
                        w_dim1 = 4;
                        w_dim2 = 4;
                    }
                }

                //(3) Do Matrix Multiplication to calculate y(:,index) from x(:,index)
                //y(:,index+CE_MI1) = W*x(:,index+CE_MI1);
                x_dim2 = InParam.nLayers;
                y_dim1 = InParam.nLayers;

                Precoder_Mat_x_Vec(w_dim1, w_dim2, Map_symb, W_ptr, x, y);

                *length_y = Map_symb * InParam.nLayers;
            }
            else
            {
                // MS_CDDType == CE_LongDelay
                //Long Delay, Cyclic Delay Diversity
                //
                // | y(0,index)   |                            | x(0,index)   |
                // | y(1,index)   | =  W(index).D(index).  U  .| x(1,index)   |
                // |     .        |                            |     .        |
                // |     .        |                            | x(V-1,index) |
                // | y(P-1,index) |
                //
                //      P x 1       =    P x V .  V x V . V x V .    V x 1        = P x 1
                //
                if ( InParam.Ntx_antennas == 2 )
                {
                    if ( InParam.nLayers == 1 )
                    {
                        pWPtr[0] = CA_PrecCodebookP2V1 + 2*InParam.Ntx_antennas * CodebookIndex;
                    }
                    else if ( InParam.nLayers == 2 ) 
                    {
                        pWPtr[0] = CA_WPrecCodebookP2V2 + InParam.Ntx_antennas * 2 * 2 * 2 * CodebookIndex;
                        pWPtr[1] = pWPtr[0] + 8;
                    }
                    else if ( InParam.nLayers == 3 )
                    {
                        pWPtr[0] = CA_WPrecCodebookP4V3W + 4 * 3 * 2 * 3 * CodebookIndex; 
                        pWPtr[1] = pWPtr[0] + 24;
                        pWPtr[2] = pWPtr[1] + 24;
                    }
                    else // nLayers == 4
                    {
                        pWPtr[0] = CA_WPrecCodebookP4V4W + 4 * 4 * 2 * 4 * CodebookIndex; 
                        pWPtr[1] = pWPtr[0] + 32;
                        pWPtr[2] = pWPtr[1] + 32;
                        pWPtr[3] = pWPtr[2] + 32;
                    }
                    // should check for an error...
                }
                else // 4 antenna ports 
                {
                    if ( InParam.nLayers == 1 )
                    {
                        pWPtr[0] = CA_PrecCodebookP4V1W + 2 * InParam.Ntx_antennas * CodebookIndex;;
                    }
                    else if ( InParam.nLayers == 2 ) 
                    {
                        pWPtr[0] = CA_WPrecCodebookP4V2W + InParam.Ntx_antennas * 2 * 2 * 2 * CodebookIndex;
                        pWPtr[1] = pWPtr[0] + 16;
                    }
                    else if ( InParam.nLayers == 3 )
                    {
                        pWPtr[0] = CA_WPrecCodebookP4V3W + 4 * 3 * 2 * 3 * CodebookIndex; 
                        pWPtr[1] = pWPtr[0] + 24;
                        pWPtr[2] = pWPtr[1] + 24;
                    }
                    else // nLayers == 4
                    {
                        pWPtr[0] = CA_WPrecCodebookP4V4W + 4 * 4 * 2 * 4 * CodebookIndex; 
                        pWPtr[1] = pWPtr[0] + 32;
                        pWPtr[2] = pWPtr[1] + 32;
                        pWPtr[3] = pWPtr[2] + 32;
                    }
                    // should check for an error...
                }

                wdu_dim1 = InParam.Ntx_antennas; // set the dimensions
                wdu_dim2 = InParam.nLayers; 

                x_dim2 = InParam.nLayers;
                y_dim1 = InParam.nLayers;

                Map_symb = Mlayer_symb;

                Precoder_LargeCDD_Mat_x_Vec(x, y, pWPtr, Map_symb, wdu_dim1,
                                            wdu_dim2, x_dim2, y_dim1, CodebookIndex); 

                *length_y = Map_symb * InParam.nLayers;
            } //MS_CDDType
        } 
        else //pPhy->bs_Ntx_antennas
        {
            myprintf(("error in precoding(): Invalid number of antenna ports, for SpatMux"));
        }
    }
    else
    {
        myprintf(" error in precoder(): Invalid AntennaMappingMethod");
    }
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   w_dim1 DOXYGEN_TO_DO
 *  @param   w_dim2 DOXYGEN_TO_DO
 *  @param   Map_symb DOXYGEN_TO_DO
 *  @param   *W_ptr DOXYGEN_TO_DO
 *  @param   *x DOXYGEN_TO_DO
 *  @param   *y DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |w_dim1        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |w_dim2        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Map_symb      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*W_ptr        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*x            |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*y            |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
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
void Precoder_Mat_x_Vec(S16 w_dim1, S16 w_dim2, S32 Map_symb, S16 *W_ptr, S16 *x, S16 *y)
{

#ifdef CEVA_INTRINSICS
    if ((w_dim1 == 2) && (w_dim2 == 1))
        Precoder_Mat_x_Vec_2_1_A(Map_symb, w_dim1, w_dim2, W_ptr, x, y);

    else if ((w_dim1 == 2) && (w_dim2 == 2))
        Precoder_Mat_x_Vec_2_2_A(Map_symb, w_dim1, w_dim2, W_ptr, x, y);

    else if ((w_dim1 == 4) && (w_dim2 == 1))
        Precoder_Mat_x_Vec_4_1_A(Map_symb, w_dim1, w_dim2, W_ptr, x, y);

    else if ((w_dim1 == 4) && (w_dim2 == 2))
        Precoder_Mat_x_Vec_4_2_A(Map_symb, w_dim1, w_dim2, W_ptr, x, y);

    else if ((w_dim1 == 4) && (w_dim2 == 3))
        Precoder_Mat_x_Vec_4_3_A(Map_symb, w_dim1, w_dim2, W_ptr, x, y);

    else if ((w_dim1 == 4) && (w_dim2 == 4))
        Precoder_Mat_x_Vec_4_4_A(Map_symb, w_dim1, w_dim2, W_ptr, x, y);

#else

    S32 i, j;
    S32 index;
    S32 ii, jj, dimxii;
    S32 kk, mm;
    S32 Ltemp_RR, Ltemp_II;
    S32 Map_symb2, i2;

    // General purpose matrix and vector multiplication
    //
    kk = (S32) add(w_dim2, w_dim2);
    j = 0;
    Map_symb2 = Map_symb + Map_symb;
    for (index = 0; index < Map_symb; index++, j += 2)
    {
        // c_mat_vec_mul_fx( W_ptr, x_vec,  w_dim1, w_dim2, y_dim1, y_vec);
        dimxii = 0;
        i = j;
        for (ii = 0; ii < w_dim1; ii++, dimxii += kk, i += Map_symb2)
        {
            Ltemp_RR = 0x00008000;
            Ltemp_II = 0x00008000;
            mm = dimxii;
            i2 = j;
            for (jj = 0; jj < w_dim2; jj++, mm += 2, i2 += Map_symb2)
            {
                Ltemp_RR = L_mac(Ltemp_RR, W_ptr[mm], x[i2]);
                Ltemp_RR = L_msu(Ltemp_RR, W_ptr[mm + 1], x[i2 + 1]);

                Ltemp_II = L_mac(Ltemp_II, W_ptr[mm + 1], x[i2]);
                Ltemp_II = L_mac(Ltemp_II, W_ptr[mm], x[i2 + 1]);
            }

            y[i] = extract_h(Ltemp_RR);
            y[i + 1] = extract_h(Ltemp_II);
        }

    } // for index
#endif

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   w_dim1 DOXYGEN_TO_DO
 *  @param   w_dim2 DOXYGEN_TO_DO
 *  @param   Map_symb DOXYGEN_TO_DO
 *  @param   *W_ptr DOXYGEN_TO_DO
 *  @param   *x DOXYGEN_TO_DO
 *  @param   *y DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |w_dim1        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |w_dim2        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |Map_symb      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*W_ptr        |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*x            |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |*y            |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
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
void Precoder_LargeCDD_Mat_x_Vec(S16 *pInput, S16 *pOutput, S16 *pWPtr[], 
                                  S32 Map_symb, S16 wdu_dim1, S16 wdu_dim2, 
                                  S16 x_dim2, S16 y_dim1, S16 CodebookIndex)
{
#ifndef CEVA_INTRINSICS
    S32 index, j, modIndx;
    S16 x_vec[32];
    S16 y_vec[32];
    S16 temp_real1, temp_imag1;
    S16 temp_real2, temp_imag2;

    // for non-ceva 
    // for a special case, two antenna, large delay CDD, to match C and CEVA implementation
    if ( (wdu_dim1 == 2 ) && ( CodebookIndex == 0 ) )
    {
        for (index = 0; index < Map_symb; index++)
        {
            for (j = 0; j < x_dim2; j++)
            {
                x_vec[2 * j] = pInput[2 * (index + j * Map_symb)];
                x_vec[2 * j + 1] = pInput[2 * (index + j * Map_symb) + 1];
            }

            if((index % 2) == 0) // even index
            {
                temp_real1 = shr(x_vec[0],1);
                temp_imag1 = shr(x_vec[1],1);
                temp_real2 = shr(x_vec[2],1);
                temp_imag2 = shr(x_vec[3],1);

                y_vec[0] = add(temp_real1,temp_real2);
                y_vec[1] = add(temp_imag1,temp_imag2);

                y_vec[2] = sub(temp_real1,temp_real2);
                y_vec[3] = sub(temp_imag1,temp_imag2);
            }
            else
            {
                temp_real1 = shr(x_vec[0],1);
                temp_imag1 = shr(x_vec[1],1);
                temp_real2 = shr(x_vec[2],1);
                temp_imag2 = shr(x_vec[3],1);

                y_vec[0] = add(temp_real1,temp_real2);
                y_vec[1] = add(temp_imag1,temp_imag2);

                y_vec[2] = sub(temp_real2,temp_real1);
                y_vec[3] = sub(temp_imag2,temp_imag1);
            }

            for (j = 0; j < y_dim1; j++)
            {
                pOutput[2 * (index + j * Map_symb)] = y_vec[2 * j];
                pOutput[2 * (index + j * Map_symb) + 1] = y_vec[2 * j + 1];
            }
        }
    }
    else // general case
    {
        // For time being leave the code as is 
        for (index = 0; index < Map_symb; index++)
        {
            modIndx = ( index % y_dim1 );

            for (j = 0; j < x_dim2; j++)
            {
                x_vec[2 * j] = pInput[2 * (index + j * Map_symb)];
                x_vec[2 * j + 1] = pInput[2 * (index + j * Map_symb) + 1];
            }

            Precoder_Mat_x_Vec(wdu_dim1, wdu_dim2, 1, pWPtr[modIndx], x_vec, y_vec);
     
            for (j = 0; j < y_dim1; j++)
            {
                pOutput[2 * (index + j * Map_symb)] = y_vec[2 * j];
                pOutput[2 * (index + j * Map_symb) + 1] = y_vec[2 * j + 1];
            }
        }
    }
#else
    S32 index, j, modIndx;
    S16 x_vec[32];
    S16 y_vec[32];
    S32 *pY0, *pY1;
    S32 *pX0, *pX1;
    S32 temp1, temp2;
    S32 *xTemp32; //add
    S32 *yTemp32; //add
        
    // From 36.211 for large delay CDD and two antenna ports
    // the code-book index = 0. Hence the two matrices are
    // [0.5 0.5;0.5 -0.5] -- pWPtr[0] and 
    // [0.5 0.5; -0.5 0.5] -- pWPtr[1] 
    if ( (wdu_dim1 == 2 ) && ( CodebookIndex == 0 ) )
    {
        xTemp32 = (S32 *)pInput; //cast
        yTemp32 = (S32 *)pOutput; //cast                      

        /* Use 32-bit intrinsics */
        pX0 = xTemp32;
        pX1 = &xTemp32[Map_symb];

        pY0 = yTemp32;
        pY1 = &yTemp32[Map_symb];

        /* Loop for all symbols */
        /* NOTE: I think the subtraction might saturate?  
         * set saturation flags ON ? */
        for ( index = 0; index < Map_symb; index += 2 )
        {
            /* For even index */
            /* arithmetic right shift by 1 bit (multiply by 0.5) */
            temp1 = shift1_acW_acZ_2w_r_ar ( *pX0++ ); 
            temp2 = shift1_acW_acZ_2w_r_ar ( *pX1++ );

            /* add real, imaginary and subtract real, imaginary */
            *pY0++ = add_acW_acX_acZ_2w ( temp1, temp2 );
            *pY1++ = sub_acW_acX_acZ_2w ( temp1, temp2 ); 

            /* for the odd index */
            temp1 = shift1_acW_acZ_2w_r_ar ( *pX0++ );
            temp2 = shift1_acW_acZ_2w_r_ar ( *pX1++ );

            *pY0++ = add_acW_acX_acZ_2w ( temp1, temp2 );
            /* subtract */
            *pY1++ = sub_acW_acX_acZ_2w ( temp2, temp1 ); 
        }
    }
    else
    {
        // For time being leave the code as is 
        for (index = 0; index < Map_symb; index++)
        {
            modIndx = ( index % y_dim1 );

            for (j = 0; j < x_dim2; j++)
            {
                x_vec[2 * j]     = pInput[2 * (index + j * Map_symb)];
                x_vec[2 * j + 1] = pInput[2 * (index + j * Map_symb) + 1];
            }

            Precoder_Mat_x_Vec(wdu_dim1, wdu_dim2, 1, pWPtr[modIndx], x_vec, y_vec);
     
            for (j = 0; j < y_dim1; j++)
            {
                pOutput[2 * (index + j * Map_symb)]     = y_vec[2 * j];
                pOutput[2 * (index + j * Map_symb) + 1] = y_vec[2 * j + 1];
            }
        }
    }
#endif  //CEVA_INTRINSICS
}
