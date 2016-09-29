//-------------------------------------------------------------------------------------------
/** @file bs_tx_PHICH.c
 *
 * @brief LTE BS Transmitter, Physical Channel processing from input
 *        codewords to output resource elements
 * @author Mindspeed Technologies
 * @version $Revision: 1.12 $
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
#else
#include "basic_op_cc.h"
#endif

//#ifndef _WIN32
//#include "mdmadrv.h"
//extern U32 gDepDiscovery;
//#endif

//-------------------------------------------------------------------------------------------
// BsTxPHICH()
//
/** @brief Base Station transmitter, Physical HARQ Indicator Channel processing
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param pPhy        Pointer to PLTEPHYDESC for this Phy Instance
 *  @param a           Pointer to array containing the resource elements for this transmitted symbol
 *  @param Ntx_ant     number of transmit antennas
 *  @param d0_IQ_wz  slot number of this 1/2ms slot within the 10ms radio frame
 *  @param scaleFactor signal level scale factor for the PHICH, for power level control
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------------------------
 *  |Name         |Name in   |Direction     |Length                 |Type                    |Data Format |Valid Range |Units  |
 *  |             |Standard  |              |                       |                        |            |            |       |
 *  ----------------------------------------------------------------------------------------------------------------------------
 *  |pPhy         |          |input         |1                      |pStructure(PLTEPHYDESC) |            |            |       |
 *  ----------------------------------------------------------------------------------------------------------------------------
 *  |a            |a         |input/output  |pPhy->bsTx.Ndl_rb      |pComplex Array          |(1:1:14)    |[-2:+2)     |       |
 *  |             |          |              |* pPhy->bs.Nrb_sc      |                        |            |            |       |
 *  |             |          |              |* pPhy->bsTx.Ndl_symb  |                        |            |            |       |
 *  |             |          |              |* Ntx_ant              |                        |            |            |       |
 *  ----------------------------------------------------------------------------------------------------------------------------
 *  |Ntx_ant      |P         |input         |1                      |real                    |(1:15:0)    |[1:4]       |       |
 *  ----------------------------------------------------------------------------------------------------------------------------
 *  |d0_IQ_wz   |d       |input         |1                      |pComplex Array                    |(1:1:14)    |[-2:+2)      |       |
 *  ----------------------------------------------------------------------------------------------------------------------------
 *  |scaleFactor  |beta      |input         |1                      |real                    |(1:1:14)    |[-2:+2)     |       |
 *  ----------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------
 *  |Block Name          |BsTxPHICH                 |
 *  -------------------------------------------------
 *  |Input Streams       |pPhy->bsTxChan.pFecTxOut  |
 *  -------------------------------------------------
 *  |Output Streams      |a                         |
 *  -------------------------------------------------
 *  |Config Parameters   |pPhy, Ntx_ant             |
 *  -------------------------------------------------
 *  |Status Parameters   |none                      |
 *  -------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Base Station transmitter, physical HARQ indicator channel (PHICH) processing;<BR>
 *   -- bit repetition<BR>
 *   -- CDMA scrambling and randomization<BR>
 *   -- Resource group alignment, Layer Mapping and Precoding<BR>
 *   -- summation of up to 8 PHICH into each group<BR>
 *   -- PHICHResourceElementAssignment()<BR>
 *   -- PHICHResourceElementMapper()<BR>
 *  This performs the processing of the PHICH data for this subframe as described in [1], from the input
 *  bitstream through to the IQ samples mapped to the resource grid.
 *
 *
 *  <5> Cycle Count Formula:
 * TBD
 *
 *
 *  <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.6.9, "Physical Channels and Modulation"<BR>
 *
 *
 * \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void BsTxPHICH(S16 numTxAntennas, S16 d0_IQ_wz[], S16 Nsf_PHICH, S16 N_PHICH_Grp, S16 *y,
               S16 *y_bar, S16 CPType, S16 m_c, U32 M0symb, U32 stc_alloc_length,
               S16 scaleFactor[], S16 *d0_IQ_wz0, S16 *EffectiveNumPHICH, S16 y_tilde[])
{
    U32 i, n1, n2;
    U32 length_per_call;
    S16 cond1, cond2;
    S16 index;
    S32 j, k;
    S16 expo;
    S16 Nsf_PHICH3 = Nsf_PHICH * 3;
    U32 n1i;
    S16 NumPHICH;
    U32 Idx1, Idx2;
    U32 IdxTmp;

    //Resource group alignment, Layer Mapping and Precoding
    if (CPType == 1 && (N_PHICH_Grp & 1) == 0) // extended
    {// insertion every 2 IQ, Type1
        n1 = 0;
        n2 = 0;
        for (i = 0; i < (U32) (N_PHICH_Grp); i++)
        {
            for (j = 0; j < EffectiveNumPHICH[i]; j++)
            {
                for (k = 0; k < Nsf_PHICH3; k++)
                {
                    index = (S16) ((k & ~1) + k + n2);
                    memcpy((S8*) &d0_IQ_wz0[index << 1], (S8*) &d0_IQ_wz[(k + n1) << 1],
                           sizeof(S16) << 1);
                }
                n1 = n1 + 3 * Nsf_PHICH;
                n2 = n2 + m_c * Nsf_PHICH3;
            }
        }
    }
    else if (CPType == 1 && (N_PHICH_Grp & 1) == 1) // extended
    {// insertion every 2 IQ, Type2
        n1 = 0;
        n2 = 0;
        for (i = 0; i < (U32) (N_PHICH_Grp); i++)
        {
            for (j = 0; j < EffectiveNumPHICH[i]; j++)
            {
                for (k = 0; k < 3 * Nsf_PHICH3; k++)
                {
                    index = (S16) ((k & ~1) + k + n2 + 2);
                    memcpy((S8*) &d0_IQ_wz0[index << 1], (S8*) &d0_IQ_wz[(k + n1) << 1],
                           sizeof(S16) << 1);
                }
                n1 = n1 + Nsf_PHICH3;
                n2 = n2 + m_c * Nsf_PHICH3;
            }
        }
    }
    else // Normal, Simple copy
    {
        n1 = 0;
        for (i = 0; i < (U32) (N_PHICH_Grp); i++)
        {
            for (j = 0; j < EffectiveNumPHICH[i]; j++)
            {
                for (k = 0; k < Nsf_PHICH3; k++)
                {
                    index = (S16) (k + n1);
                    memcpy((S8*) &d0_IQ_wz0[index << 1], (S8*) &d0_IQ_wz[(k + n1) << 1],
                           sizeof(S16) << 1);
                }
                n1 = n1 + Nsf_PHICH3;
            }
        }
    }

    if (numTxAntennas == 1)
    {
        memcpy(y, d0_IQ_wz0, stc_alloc_length * sizeof(y[0]));
    }
    else if (numTxAntennas == 2)
    {
        length_per_call = numTxAntennas * M0symb * 2;

        n1 = 0;
        n2 = 0;
        for (i = 0; i < (U32) (N_PHICH_Grp); i++)
        {
            for (j = 0; j < EffectiveNumPHICH[i]; j++)
            {
                STC(d0_IQ_wz0 + n1, M0symb, numTxAntennas, y + n2);
                n1 = n1 + (M0symb << 1);
                n2 = n2 + length_per_call;
            }
        }
    }
    else // pBsTx->Ntx_antennas ==  4
    {
        length_per_call = numTxAntennas * M0symb * 2;

        n1 = 0;
        n2 = 0;
        for (i = 0; i < (U32) (N_PHICH_Grp); i++)
        {
            for (j = 0; j < EffectiveNumPHICH[i]; j++)
            {
                for (k = 0; k < 3; k++)
                {
                    cond1 = (k + N_PHICH_Grp) & 1;
                    cond2 = (S16) (k + (N_PHICH_Grp >> 1));
                    if ((cond1 == 0 && CPType == 0) || (cond2 == 0 && CPType == 1))
                    {
                        STC_type1(d0_IQ_wz0 + n1, 4, y + n2);
                    }
                    else
                    {
                        STC_type2(d0_IQ_wz0 + n1, 4, y + n2);
                    }
                    n1 = n1 + 8; //2 * 4
                    n2 = n2 + length_per_call;
                }
            }
        }

    }

    // Combining(Summation over pPhy->numPHICH)

    n1 = M0symb * numTxAntennas;
    expo = 4096; //(1/8 in Q15)
    if (CPType == 1)
    {
        expo = 8192; // (1/4 in Q15)
    }

    n2 = 0;
    for (i = 0; i < (U32) (N_PHICH_Grp); i++)
    {
        n1i = n1 * i;
        NumPHICH = EffectiveNumPHICH[i];
        //expo = 4096; //invTab[pPhy->EffectiveNumPHICH[i] - 1];
#if 0
        for (k = 0; k < (S32) (n1); k++)
        {
            Idx1 = (k + n1i) << 1;
            IdxTmp = (k + n1i * NumPHICH) << 1;
            for (j = 0; j < NumPHICH; j++)
            {// divide by pPhy->numPHICH to avoid saturation
                Idx2 = IdxTmp + ((n1 * j) << 1);
                y_bar[Idx1] = add(y_bar[Idx1], mult(y[Idx2], expo));
                y_bar[Idx1 + 1] = add(y_bar[Idx1 + 1], mult(y[Idx2 + 1], expo));

            }

        }
#else
        for (k = 0; k < (S32) (n1); k++)
        {
            Idx1 = (k + n1i) << 1;
            IdxTmp = (k + n1i * n2) << 1;
            for (j = 0; j < NumPHICH; j++)
            {// divide by pPhy->numPHICH to avoid saturation
                Idx2 = IdxTmp + ((n1 * j) << 1);
                y_bar[Idx1] = add(y_bar[Idx1], mult(y[Idx2], expo));
                y_bar[Idx1 + 1] = add(y_bar[Idx1 + 1], mult(y[Idx2 + 1], expo));

            }

        }
        n2 = n2 + n1 * NumPHICH;

#endif //end of "#if 0"

        //Scale by Group
        for (k = 0; k < (S32) (n1); k++)
        {
            y_bar[2 * (k + n1 * i)] = mult(y_bar[2 * (k + n1 * i)], scaleFactor[i]);
            y_bar[2 * (k + n1 * i) + 1] = mult(y_bar[2 * (k + n1 * i) + 1], scaleFactor[i]);
        }

    }
    //Combining(Summation over every 2 groups) for extended CP
    if (CPType == 1)
    {
        expo = 2;
        for (i = 0; i < (U32) (N_PHICH_Grp >> 1); i++)
        {
            n1i = n1 * i;
            for (k = 0; k < (S32) (n1); k++)
            {
                Idx1 = (k + n1i) << 1;
                IdxTmp = (k + (n1i << 1)) << 1;
                {// compensate for division by 4 above
                    y_tilde[Idx1] = add(y_tilde[Idx1], shl(y[IdxTmp], expo));
                    y_tilde[Idx1 + 1] = add(y_tilde[Idx1 + 1], shl(y[IdxTmp + 1], expo));
                    IdxTmp += n1 << 1;
                    y_tilde[Idx1] = add(y_tilde[Idx1], shl(y[IdxTmp], expo));
                    y_tilde[Idx1 + 1] = add(y_tilde[Idx1 + 1], shl(y[IdxTmp + 1], expo));
                }
            }
        }
        if ((N_PHICH_Grp & 1) == 1) // odd
        {
            n1i = n1 * (N_PHICH_Grp >> 1);
            for (k = 0; k < (S32) (n1); k++)
            {
                Idx1 = (k + n1i) << 1;
                IdxTmp = (k + (n1i << 1)) << 1;
                y_tilde[Idx1] = shl(y[IdxTmp], 2);
                y_tilde[Idx1 + 1] = shl(y[IdxTmp + 1], 2);
            }
        }
    }
    else
    { // just copy y_bar to y_tilde
        n2 = (N_PHICH_Grp * n1) << 1;
        for (i = 0; i < n2; i++)
        {
            y_tilde[i] = shl(y_bar[i], 3); // compensate for division by 8 above
        }

    }

    return;
}

#if 0

//  Newer version (never tested)

void BsTxPHICH(PBSTXPHICHPARAM PHICHInfo, S16 d0_IQ_wz[],
    S16 scaleFactor[], S16 buffWrk[], S16 *RG_Assigned_Tx, S16 *RG_Assigned_PCFIH,
    S16 *k_l_row0, S16 *k_l_row1, S16 *EffectiveNumPHICH, S16 y_tilde[], S16 *n1out, S16 *ncol_y,
    S16 *effecPHICHChanList, S16 *effecPHICHGrpList)
{

    S16 *d0_IQ_wz0;

    S32 M0symb;
    S16 numrows_k_l, numcols_k_l;
    S16 *y;
    U32 i, n1, n2, n3;
    U32 num_blocks;
    U32 length_per_call;

    S16 cond1, cond2;

    U32 L_temp;
    S16 index;

    S32 j, k;

    S16 *y_bar, expo;
    S16 *RG_BuffWrk;
    S16 Ntx_ant = PHICHInfo->Ntx_ant;
    S16 Nsf_PHICH = PHICHInfo->Nsf_PHICH;
    S16 N_PHICH_Grp = PHICHInfo->N_PHICH_Grp;
    S16 EffectiveN_PHICH_Grp = PHICHInfo->EffectiveN_PHICH_Grp;
    S16 numPHICH = PHICHInfo->numPHICH;
    S16 Nsf_PHICH3 = PHICHInfo->Nsf_PHICH * 3;
    U32 stc_alloc_length = PHICHInfo->stc_alloc_length;

    // allocating 3 extra IQ locations for convenience
    L_temp = (3 * PHICHInfo->number_of_phich_channels * Nsf_PHICH + 3)<<2;
    d0_IQ_wz0 = buffWrk;
    y = d0_IQ_wz0 + L_temp;
    y_bar = y + stc_alloc_length;
    for(j =0; j < (S32) stc_alloc_length; j++)
    {
        y_bar[j] = 0;
        y[j] = 0;
    }

    //Resource group alignment, Layer Mapping and Precoding
    if (PHICHInfo->CPType == 1 ) // extended

    {
        n1 = 0;
        n2 = 0;
        for (i = 0; i < (U32) (EffectiveN_PHICH_Grp); i++)
        {
            if( (effecPHICHGrpList[i] & 1) == 0)
            { // insertion every 2 IQ, Type1
                for (j = 0; j < EffectiveNumPHICH[effecPHICHGrpList[i]]; j++)
                {
                    for (k = 0; k < Nsf_PHICH3; k++)
                    {
#if 0
                        index = (S16) (2 * (k / 2) + k + n2);
                        d0_IQ_wz0[2 * index] = d0_IQ_wz[2 * (k + n1)]; // Real
                        d0_IQ_wz0[2 * index + 1] = d0_IQ_wz[2 * (k + n1) + 1]; //Imag
#else
                        index = (S16) ((k & ~1) + k + n2);
                        memcpy((S8*)&d0_IQ_wz0[index << 1], (S8*)&d0_IQ_wz[(k + n1) << 1], sizeof(S16) << 1);
#endif
                    }
                    n1 = n1 + 3 * Nsf_PHICH;
                    n2 = n2 + PHICHInfo->m_c * Nsf_PHICH3;
                }
            }
            else
            {// insertion every 2 IQ, Type2


                for (j = 0; j < EffectiveNumPHICH[effecPHICHGrpList[i]]; j++)
                {
                    for (k = 0; k < 3 * Nsf_PHICH3; k++)
                    {
#if 0
                        index = (S16) (2 * (k / 2) + k + n2 + 2);
                        d0_IQ_wz0[2 * index] = d0_IQ_wz[2 * (k + n1)]; // Real
                        d0_IQ_wz0[2 * index + 1] = d0_IQ_wz[2 * (k + n1) + 1]; //Imag
#else
                        index = (S16) ((k & ~1) + k + n2 + 2);
                        memcpy((S8*)&d0_IQ_wz0[index << 1], (S8*)&d0_IQ_wz[(k + n1) << 1], sizeof(S16) << 1);
#endif
                    }
                    n1 = n1 + Nsf_PHICH3;
                    n2 = n2 + PHICHInfo->m_c * Nsf_PHICH3;
                }
            }
        }
    }
    else // Normal, Simple copy

    {
        n1 = 0;
        for (i = 0; i < (U32) (EffectiveN_PHICH_Grp); i++)
        {
            for (j = 0; j < EffectiveNumPHICH[effecPHICHGrpList[i]]; j++)
            {
                for (k = 0; k < Nsf_PHICH3; k++)
                {
                    index = (S16) (k + n1);
#if 0
                    d0_IQ_wz0[2 * index] = d0_IQ_wz[2 * (k + n1)]; // Real
                    d0_IQ_wz0[2 * index + 1] = d0_IQ_wz[2 * (k + n1) + 1]; //Imag

#else
                    memcpy((S8*)&d0_IQ_wz0[index << 1], (S8*)&d0_IQ_wz[(k + n1) << 1], sizeof(S16) << 1);

#endif
                }
                n1 = n1 + Nsf_PHICH3;
            }
        }
    }

    M0symb = PHICHInfo->m_c * Nsf_PHICH3;

    if (Ntx_ant == 1)
    {

        n1 = 0;
        n3 = 0;
        for (i = 0; i < (U32) (EffectiveN_PHICH_Grp); i++)
        {

            for (j = 0; j < EffectiveNumPHICH[effecPHICHGrpList[i]]; j++)
            {
                U32 Idx1 = (effecPHICHGrpList[i]*numPHICH + effecPHICHChanList[n3])*Nsf_PHICH3;
                for (k = 0; k < Nsf_PHICH3; k++)
                {
                    index = (S16) (k + Idx1);
                    y[2 * index] = d0_IQ_wz0[2 * (k + n1)]; // Real
                    y[2 * index + 1] = d0_IQ_wz0[2 * (k + n1) + 1]; //Imag

                }
                n3++;
                n1 = n1 + Nsf_PHICH3;

            }

        }

    }
    else if (Ntx_ant == 2)
    {
        num_blocks = M0symb / Ntx_ant;
        length_per_call = Ntx_ant * Ntx_ant * (num_blocks << 1);

        n1 = 0;
        n2 = 0;

        for (i = 0; i < (U32) (EffectiveN_PHICH_Grp); i++)
        {
            index = (S16) ((effecPHICHGrpList[i]*numPHICH + effecPHICHChanList[n2])*length_per_call);
            for (j = 0; j < EffectiveNumPHICH[effecPHICHGrpList[i]]; j++)
            {

                STC(d0_IQ_wz0 + n1, M0symb, Ntx_ant, y + index);
                n1 = n1 + (M0symb << 1);
                //n2 = n2 + length_per_call;
            }
            n2++;
        }
    }
    else // pBsTx->Ntx_antennas ==  4

    {
        //num_blocks = M0symb / Ntx_ant;
        length_per_call = Ntx_ant * (Ntx_ant << 1);

        n1 = 0;

        n3 = 0;
        for (i = 0; i < (U32) (EffectiveN_PHICH_Grp); i++)
        {
            index = (S16) ((effecPHICHGrpList[i]*numPHICH + effecPHICHChanList[n3])*length_per_call*3);
            for (j = 0; j < EffectiveNumPHICH[effecPHICHGrpList[i]]; j++)
            {
                n2 = 0;
                for (k = 0; k < 3; k++)
                {
                    cond1 = (k + effecPHICHGrpList[i]) & 1;
                    cond2 = (k + (effecPHICHGrpList[i] >> 1));
                    if ((cond1 == 0 && PHICHInfo->CPType == 0) || (cond2 == 0 && PHICHInfo->CPType == 1))
                    {
                        STC_type1(d0_IQ_wz0 + n1, 4, y + index + n2);
                    }
                    else
                    {
                        STC_type2(d0_IQ_wz0 + n1, 4, y + index + n2);
                    }
                    n1 = n1 + 8; //2 * 4
                    n2 = n2 + length_per_call;
                }
            }
            n3++;
        }

    }

    // Combining(Summation over pPhy->numPHICH)

    n1 = M0symb;
    if (Ntx_ant != 1)
    {
        n1 = (M0symb / Ntx_ant) * Ntx_ant * Ntx_ant;
    }

    expo = 4096; //(1/8 in Q15)
    if (PHICHInfo->CPType == 1)
    expo = 8192; // (1/4 in Q15)
    n3 = 0;
    for (i = 0; i < (U32) (EffectiveN_PHICH_Grp); i++)
    {
        U32 n1i = n1 * effecPHICHGrpList[i];

        //expo = 4096; //invTab[pPhy->EffectiveNumPHICH[i] - 1];
        //index = (S16) ((effecPHICHGrpList[i]*numPHICH + effecPHICHChanList[n3])*Nsf_PHICH3);
        index = (S16) ((effecPHICHGrpList[i]*numPHICH)*Nsf_PHICH3);
        for (k = 0; k < (S32) (n1); k++)
        {
            U32 Idx1 = (k + n1i) << 1;
            U32 IdxTmp = (k + index) << 1;

            for (j = 0; j < EffectiveNumPHICH[effecPHICHGrpList[i]]; j++)
            {// divide by pPhy->numPHICH to avoid saturation
                U32 Idx2 = IdxTmp + ((n1 * effecPHICHChanList[n3+j]) << 1);

#if 0

                y_bar[2 * (k + n1 * effecPHICHGrpList[i])] = add(y_bar[2 * (k + n1 * effecPHICHGrpList[i])],
                    mult(y[2 * (k + n1 * (effecPHICHChanList[n3+j]) + index)], expo));
                y_bar[2 * (k + n1 * effecPHICHGrpList[i]) + 1] = add(y_bar[2 * (k + n1 * effecPHICHGrpList[i]) + 1],
                    mult(y[2 * (k + n1 * (effecPHICHChanList[n3+j]) + index) + 1], expo));

#else
                y_bar[Idx1] = add(y_bar[Idx1], mult(y[Idx2], expo));
                y_bar[Idx1 + 1] = add(y_bar[Idx1 + 1], mult(y[Idx2 + 1], expo));

#endif

            }

            //            index = 0;


        }

        n3 = n3 + EffectiveNumPHICH[effecPHICHGrpList[i]];

        //Scale by Group
        for (k = 0; k < (S32) (n1); k++)
        {
#if 0
            y_bar[2 * (k + n1 * effecPHICHGrpList[i])] = mult(y_bar[2 * (k + n1 * effecPHICHGrpList[i])], scaleFactor[effecPHICHGrpList[i]]);
            y_bar[2 * (k + n1 * effecPHICHGrpList[i]) + 1] = mult(y_bar[2 * (k + n1 * effecPHICHGrpList[i]) + 1],scaleFactor[effecPHICHGrpList[i]]);
#else
            U32 Idx1 = (k + n1i) << 1;
            U32 Idx2 = effecPHICHGrpList[i];
            y_bar[Idx1] = mult(y_bar[Idx1], scaleFactor[Idx2]);
            y_bar[Idx1 + 1] = mult(y_bar[Idx1 + 1],scaleFactor[Idx2]);
#endif 
        }

    }

    //Combining(Summation over every 2 groups) for extended CP
    if (PHICHInfo->CPType == 1)
    {
        expo = 2;
        for (i = 0; i < (U32) (N_PHICH_Grp >> 1); i++)
        {
            U32 n1i = n1 * i;

            for (k = 0; k < (S32) (n1); k++)
            {
#if 0
                for (j = 0; j < 2; j++)
                {// compensate for division by 4 above
                    y_tilde[2 * (k + n1 * i)] = add(y_tilde[2 * (k + n1 * i)],
                        shl(y_bar[2 * (k + n1 * j + n1 * 2 * i)], expo));
                    y_tilde[2 * (k + n1 * i) + 1] = add(y_tilde[2 * (k + n1 * i) + 1],
                        shl(y_bar[2 * (k + n1 * j + n1 * 2 * i) + 1], expo));
                }
#else                
                U32 Idx1 = (k + n1i) << 1;
                U32 IdxTmp = (k + (n1i << 1)) << 1;
                {// compensate for division by 4 above
                    y_tilde[Idx1] = add(y_tilde[Idx1], shl(y_bar[IdxTmp], expo));
                    y_tilde[Idx1 + 1] = add(y_tilde[Idx1 + 1], shl(y_bar[IdxTmp + 1], expo));
                    IdxTmp += n1 << 1;
                    y_tilde[Idx1] = add(y_tilde[Idx1], shl(y_bar[IdxTmp], expo));
                    y_tilde[Idx1 + 1] = add(y_tilde[Idx1 + 1], shl(y_bar[IdxTmp + 1], expo));
                }
#endif
            }
        }
        if( (N_PHICH_Grp & 1) == 1) // odd

        {
#if 0
            for (i = (U32) (N_PHICH_Grp / 2); i < (U32) (N_PHICH_Grp / 2 + 1); i++)
            {
                for (k = 0; k < (S32) (n1); k++)
                {
                    for (j = 0; j < 1; j++)
                    {
                        y_tilde[2 * (k + n1 * i)] = shl(y_bar[2 * (k + n1 * j + n1 * 2 * i)],2);
                        y_tilde[2 * (k + n1 * i) + 1] = shl(y_bar[2* (k + n1 * j + n1 * 2 * i) + 1],2);

                    }
                }
            }
#else
            U32 n1i = n1 * (N_PHICH_Grp >> 1);
            for (k = 0; k < (S32) (n1); k++)
            {
                U32 Idx1 = (k + n1i) << 1;
                U32 IdxTmp = (k + (n1i << 1)) << 1;

                y_tilde[Idx1] = shl(y_bar[IdxTmp],2);
                y_tilde[Idx1 + 1] = shl(y_bar[IdxTmp + 1],2);

            }
#endif		   
        }

    }
    else
    { // just copy y_bar to y_tilde
        n2 = (N_PHICH_Grp * n1) << 1;
        for (i = 0; i < n2; i++)
        {

            y_tilde[i] = shl(y_bar[i],3); // compensate for division by 8 above
            fprintf(fy, "y=%d\n", y_tilde[i]);
        }

    }

    *ncol_y = (S16) M0symb;
    *n1out = (S16) n1;

    RG_BuffWrk = y_bar + stc_alloc_length;

    PHICHResourceElementAssignment(PHICHInfo, RG_Assigned_Tx, RG_Assigned_PCFIH, k_l_row0, k_l_row1, &numrows_k_l, &numcols_k_l, RG_BuffWrk);

    return;
}

#endif
