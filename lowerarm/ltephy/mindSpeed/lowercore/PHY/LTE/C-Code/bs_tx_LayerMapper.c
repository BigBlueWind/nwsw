//-------------------------------------------------------------------------------------------
/** @file bs_tx_LayerMapper.c
 *
 * @brief LTE Base Station Transmit Layer Mapper Implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.21 $
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
#include "ext_phy.h"
#include "prototypes.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#else
#include "basic_op_cc.h"
#endif

//-------------------------------------------------------------------------------------------
/** @brief Base Station, Map 1 or 2 codewords to 1, 2, 3 or 4 layers per 3GPP TS 36.211 Rel 8 Sec.6.3.3
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param d0        INPUT, Pointer to input bitstream d0
 *  @param d1        INPUT, Pointer to input bitstream d1
 *  @param M0symb    INPUT, number of modulation symbols in codewords 0
 *  @param M1symb    INPUT, number of modulation symbols in codewords 1
 *  @param V         INPUT, number of layers
 *  @param x         OUTPUT, Pointer to LayerMapper output
 *  @param transmode INPUT, transmission Mode
 *  @param num_row_x OUTPUT, Pointer to number of rows in the layer Mapper Out
 *  @param num_col_x OUTPUT, Pointer to number of colums in the layer Mapper Out
 *
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name     |Name in  |Direction|Length     |Type          |Data Format |Valid Range |Units |
 *  |         |Standard |         |           |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |d0       |         |pInput   |M0symb     |pComplex Array|(1:1:14)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |d1       |         |pInput   |M1symb     |pComplex Array|(1:1:14)    |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------
 *  |M0symb   |         |Input    |1          |Real          |(1:15:0)    |[0:1200]    |      |
 *  -------------------------------------------------------------------------------------------------
 *  |M1symb   |         |Input    |1          |Real          |(1:15:0)    |[0:1200]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  |V        |         |Input    |1          |Real          |(1:15:0)    |[1,2,3,4]   |      |
 *  --------------------------------------------------------------------------------------------------
 *  |x        |         |pOutput  |num_row_x* |pComplex Array|(1:1:14)    |Full Range  |      |
 *  |         |         |         |num_col_x  |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |transmode|         |Input    |1          |Real          |(1:15:0)    |[1:7]       |      |
 *  --------------------------------------------------------------------------------------------------
 *  |num_row_x|         |pOutput  |1          |Real          |(1:15:0)    |[1,2,4]     |      |
 *  --------------------------------------------------------------------------------------------------
 *  |num_col_x|         |pOutput  |1          |Real          |(1:15:0)    |[0:1200]    |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------
 *  |Block Name          |LayerMapper              |
 *  ------------------------------------------------
 *  |Input Streams       |d0,d1                    |
 *  ------------------------------------------------
 *  |Output Streams      |x                        |
 *  ------------------------------------------------
 *  |Config Parameters   |M0symb,M1symb,V,transmode|
 *  ------------------------------------------------
 *  |Status Parameters   |num_row_x,num_col_x      |
 *  ------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Map 1 or 2 codewords to 1, 2, 3 or 4 layers<BR>
 *
 *  <B>Input Parameters:</B><BR>
 *  - d0       = d0(0),d0(1),...,d0(M0symb-1)  = input bitstream, d0(0)
 *            sent first in time<BR>
 *  - d1       = d1(0),d1(1),...,d1(M1symb-1)  = input bitstream, d1(0)
 *            sent first in time<BR>
 *  - M0symb, M1symb  = number of modulation symbols in codewords 0, 1<BR>
 *  - V        = 1, 2, 3 or 4  = number of layers<BR><BR>
 *
 *  <B>Outputs:</B><BR>
 *  - x     = x(0,0),x(0,1),...,x(0,Mlayer_symb-1);<BR>
 *          x(1,0),x(1,1),...,x(1,Mlayer_symb-1);<BR>
 *          ....<BR>
 *          x(V-1,0),x(V-1,1),...,x(V-1,Mlayer_symb-1);<BR>
 *  - transmode = {CE_SingleAntenna, CE_SpatMux, CE_Diversity}
 *              = MIMO tx antenna mapping algorithm<BR>
 *
 * <5> Cycle Count Formula:
 * Worst case inner loop is 4 (antennas) copies of a complex symbol for each data subcarrier.
 * Assume worst case, only 16-bits copy per cycle, so 2 cycles for a complex value copy.
 * Assume total MIPS is 20% more than the inner loop MIPS<BR>
 *
 * <B>Estimate:</B>
 * - Cycles/symbol = 4 x 2 x Nused x 1.2<BR>
 * - Cycles/sec    = 9.6 x Nused x Fsym<BR>
 *
 * For Q=1, V=1 --> Cycles = 62 + Nsymbols
 * For Q=2, V=2 --> Cycles = 103 + 2*Nsymbols
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel 8 Sec.6.3.3, "Downlink: Layer mapping"
 *
 * \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void LayerMapper(S16 d0[], S16 d1[], S32 M0symb, S32 M1symb, S16 V, S16 *x, U8 transmode,
                 S32 *num_row_x, S32 *num_col_x)
{
#ifndef CEVA_INTRINSICS	
    int i, index, i2, i2M, i4, i6;
#endif	
    int Q = 2; //init it for 2 codewords
    int nb;
    S16 *px;
    S32 Mlayer_symb;

    if (M1symb == 0)
    {
        //only 1 codeword, since length of 2nd codeword=0
        Q = 1;
    }

    if (Q == 1 && V == 1)
    {
        //Layer Mapping for Transmission on a single antenna port
        //AND Layer Mapping for Spatial Multiplexing, where Q=V=1
        //NOTE: no difference in layer mapping code for Single Antenna
        //(Q==1, V==1) and Spatial Multiplexing with (Q==1, V==1),
        //so no need for AntennaMappingMethod={SingleAntenna, SpatMux,
        // Diversity} input parameter!
        Mlayer_symb = M0symb;

        nb = Mlayer_symb + Mlayer_symb; // 2*Mlayer_symb
        nb = nb + nb; // shorts --> bytes
        memcpy(x, d0, nb);

        *num_row_x = 1;
        *num_col_x = Mlayer_symb;
    }
    else if (transmode == OPNLPSPMUX || transmode == CLSLPSPMUX) // CE_SpatMux)
    {
        if (Q == 2 && V == 2)
        {
            //Layer Mapping for Spatial Multiplexing (with 2 input codewords)
            //2 codewords, 2 layers
#ifdef _WIN32
            if (M0symb != M1symb)
            {
                myprintf(" Error in LayerMapper(): For Spatial Multiplexing, \
                         Q==2, V==2, M0symb must equal M1symb");
            }
#endif
            Mlayer_symb = M0symb;
            //so, x is a 2 * Mlayer_symb matrix,where Mlayer_symb = M0symb = M1symb
            nb = Mlayer_symb + Mlayer_symb; // 2*Mlayer_symb
            px = (S16 *) &(x[nb]);
            nb = nb + nb; // shorts --> bytes
            memcpy(x, d0, nb);
            memcpy(px, d1, nb);

            *num_row_x = 2;
            *num_col_x = Mlayer_symb;
        }
        else if (Q == 1 && V == 2)
        {
            //1 codewords, 2 layers
            Mlayer_symb = M0symb >> 1; // Mlayer_symb = M0symb/2;
            //so, x is a 2 * Mlayer_symb matrix,where Mlayer_symb = M0symb/2
#ifdef CEVA_INTRINSICS
            LayerMapper_Q1_V2_A(x, d0, Mlayer_symb);
#else
            i2 = 0;
            i2M = Mlayer_symb + Mlayer_symb;
            i4 = 0;
            for (i = 0; i < Mlayer_symb; i++)
            {
                x[i2++] = d0[i4++]; // x(0,i) = d(0,2*i)
                x[i2++] = d0[i4++];
                x[i2M++] = d0[i4++]; // x(1,i) = d(0,2*i+1)
                x[i2M++] = d0[i4++];
            }
#endif

            *num_row_x = 2;
            *num_col_x = Mlayer_symb;
        }
        else if (Q == 2 && V == 3)
        {
            //2 codewords, 3 layers
#ifdef _WIN32
            if (M0symb != (M1symb/2))
            {
                myprintf(" Error in LayerMapper(): For Spatial Multiplexing, Q==2,\
                         V==3, M0symb must equal M1symb/2");
            }
#endif
            Mlayer_symb = M0symb;
#if 0
            for (i =0; i< 2*Mlayer_symb; i++)
            {
                //x(0,i) = d(0,i)
                x[i] = d0[i];
            }
#else
            memcpy(x, d0, (Mlayer_symb << 2)); //short->byte
#endif

#ifdef CEVA_INTRINSICS
            LayerMapper_Q2_V3_A(x, d1, Mlayer_symb);
#else
            i2 = Mlayer_symb + Mlayer_symb;
            i4 = i2 + i2;
            for (index = 0; index < Mlayer_symb; index++)
            {
                x[i2++] = d1[4 * index]; //x(1,i) = d(1,2i)
                x[i2++] = d1[4 * index + 1]; //x(1,i) = d(1,2i)

                x[i4++] = d1[4 * index + 2]; //x(2,i) = d(1,2i+1)
                x[i4++] = d1[4 * index + 3]; //x(2,i) = d(1,2i+1)
            }
#endif
            *num_row_x = 3;
            *num_col_x = Mlayer_symb;
        }
        else if (Q == 2 && V == 4)
        {
            //2 codewords, 4 layers
#ifdef _WIN32
            if (M0symb != M1symb)
            {
                myprintf(" Error in LayerMapper(): For Spatial Multiplexing, Q==2,\
                         V==4, M0symb/2 must equal M1symb/2");
            }
#endif
            Mlayer_symb = M0symb >> 1; // Mlayer_symb = M0symb/2

#ifdef CEVA_INTRINSICS
            LayerMapper_Q2_V4_A(x, d0, d1, Mlayer_symb);
#else
            i = 0;
            i2 = Mlayer_symb + Mlayer_symb;
            i4 = i2 + i2;
            i6 = i2 + i4;
            for (index = 0; index < Mlayer_symb; index++)
            {
                x[i++] = d0[4 * index]; //x(0,i) = d(0,2i)
                x[i++] = d0[4 * index + 1]; //x(0,i) = d(0,2i)

                x[i2++] = d0[4 * index + 2]; //x(1,i) = d(0,2i+1)
                x[i2++] = d0[4 * index + 3]; //x(1,i) = d(0,2i+1)

                x[i4++] = d1[4 * index]; //x(2,i) = d(1,2i)
                x[i4++] = d1[4 * index + 1]; //x(2,i) = d(1,2i)

                x[i6++] = d1[4 * index + 2]; //x(3,i) = d(1,2i+1)
                x[i6++] = d1[4 * index + 3]; //x(3,i) = d(1,2i+1)
            }
#endif
            *num_row_x = 4;
            *num_col_x = Mlayer_symb;
        }
        else
        {
#ifdef _WIN32
            myprintf(" Error inLayerMapper(): Invalid parameter V, for Q==2");
#endif
        }
    }
    else
    {
#ifdef _WIN32
        myprintf(" Error in LayerMapper(): Invalid parameter combination for Q and V, and mimoTxMode");
#endif
    }
}

