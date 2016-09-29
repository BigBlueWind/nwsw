//-------------------------------------------------------------------------------------------
/** @file ul_Pilot_pucch.c
 *
 * @brief Add Uplink Reference (pilot) signals to the resource grid
 * @author Mindspeed Technologies
 * @version $Revision: 1.49 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<math.h>

#ifdef _WIN32
#define WIN_BLD 1
#else
#define WIN_BLD 0
#endif

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
#include "math_ext32.h"

#if 0
#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#define CEVA_BUILD 1
#else
#include "basic_op_cc.h"
#define CEVA_BUILD 0
#endif
#endif

#ifdef CEVA_INTRINSICS
#include <basic_op.h>

extern U8 *DataBuff_Bank2;

#else

#include "basic_op_cc.h"
#endif

//-------------------------------------------------------------------------------------------
/** @brief ul_Pilot_PUCCH: Add Uplink Reference (pilot) signals to the resource grid for PUCCH
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param a            = OUTPUT, Pointer to resource element grid for this antenna
 *  @param r_alpha_uv   = INPUT, pointer to Reference Signal Sequence
 *  @param nOcBar       = INPUT, pointer to Orthogonal Sequence Index
 *  @param ULPilotInput = INPUT, Structure containing input parameters
 *  @param ChanNum      = INPUT, channel number
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------
 *  |Name         |Name in  |Direction     |Length     |Type          |Data Format |Valid Range |Units |
 *  |             |Standard |              |           |              |            |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |a            |         |pOutput       |           |pComplex Array|(1:1:14)    |[-2,2)      |      |
 *  -------------------------------------------------------------------------------------------------
 *  |r_alpha_uv   |         |pInput        |           |Real          |(1:15:0)    |[-1,1)      |      |
 *  --------------------------------------------------------------------------------------------------
 *  |nOcBar       |         |pInput        | 2         |Real          |(1:15:0)    |[0:2]       |      |
 *  --------------------------------------------------------------------------------------------------
 *  |pULPilotInput|         |pInput        |1          |Real          |(1:15:0)    |            |      |
 *  --------------------------------------------------------------------------------------------------
 *  |ChanNum      |         |Input         |1          |long          |(1:15:0)    |[0:100]     |      |
 *  --------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------------
 *  |Block Name          |ul_Pilot_PUCCH                          |
 *  ---------------------------------------------------------------
 *  |Input Streams       |r_alpha_uv, nOcBar                      |
 *  ---------------------------------------------------------------
 *  |Output Streams      |a                                       |
 *  ---------------------------------------------------------------
 *  |Config Parameters   |pULPilotInput, chanNum                  |
 *  ---------------------------------------------------------------
 *  |Status Parameters   |none                                    |
 *  ---------------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Add Uplink Reference (pilot) signals to the resource grid [3GPP TS 36.211 v8.4.0, Sec.5.5]
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 v8.4.0, Sec.5.5, "Reference signals"
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
void ul_Pilot_PUCCH_PartOne(S16 num_Pilots_slot,
                                   S16 Nrb_sc,
                                   S16 Nul_symb,
                                   S16 indexPilot,
                                   S16 ns,
                                   S16 *r_alpha_uv,
                                   S16  a[],
                                   S16 *w_bar_ptr)
{
    S32 l, n, m;
    S16 temp1, temp2;
    S32 indexGrid = 0;
    for (l = 0; l < num_Pilots_slot; l++)
    {
        m = ns * Nul_symb + Pilot_symbol_loc[l + indexPilot];

        for (n = 0; n < Nrb_sc; n++)
        {
            temp1 = r_alpha_uv[2 * m * PRB_12 + 2 * n];
            temp2 = r_alpha_uv[2 * m * PRB_12 + 2 * n + 1];

            a[indexGrid++] = sub(mult(temp1, w_bar_ptr[2 * l]),
                                 mult(temp2, w_bar_ptr[2 * l + 1]));
            a[indexGrid++] = add(mult(temp2, w_bar_ptr[2 * l]),
                                 mult(temp1, w_bar_ptr[2 * l + 1]));
        }// n
    }// l
}

void ul_Pilot_PUCCH(S16 a[], S16 *r_alpha_uv, S16 *nOcBar, ULRXINPUTPARAMS *pULPilotInput,
                    S32 ChanNum)
{

    S16 Nrb_sc;
    S16 indexPilot;//, indexGrid;
    S32 ns;
    S16 /*slotNum, */Nul_symb, num_Pilots_slot;
    S16 pucchFormat;

    S16 *w_bar_ptr;
    S16 numrows_a;

    S16 k0;
    S16 nPRB;

    Nrb_sc = pULPilotInput->Nrb_sc;
    //slotNum = pULPilotInput->slotNum;
    Nul_symb = pULPilotInput->Nul_symb;
    num_Pilots_slot = pULPilotInput->num_Pilots_slot[ChanNum];
    pucchFormat = pULPilotInput->pucchFormat[ChanNum];
    numrows_a = pULPilotInput->K;

    nPRB = pULPilotInput->RBStart[ChanNum];

    // Demodulator Reference Signal for PUCCH
    // sequence length 2*Mrs_sc = sequence r_alpha_uv repeated on the single
    // training symbol in each slot, with different cyclic shift each slot
    // Note that  Orthog_Seq_PUCCH_fm1/2 has a scale factor of half
    // so that we don't have to divide by 2 in phy_bs_rx.c as we did for PUSCH
    //indexGrid = 0;
    for (ns = 0; ns < NUM_SLOTS_SUBFRM; ns++)
    {
        //Pilots use all Resource Elements of this symbol
        k0 = nPRB * Nrb_sc; // frequency index for first resource element
        // in the first resource block of this resource
        // assignment ordered pair
        if (ns == 1) // second slot
            k0 = numrows_a - k0 - Nrb_sc;

        if (pucchFormat >= FORMAT2)
        {
            w_bar_ptr = Orthog_Seq_PUCCH_fm2;
            indexPilot = 3;
        }
        else // Format 1s
        {
            w_bar_ptr = Orthog_Seq_PUCCH_fm1 + 3 * 2 * nOcBar[ns]; // Consider only Normal CP for now
            indexPilot = 0;
        }
#ifndef CEVA_INTRINSICS
       ul_Pilot_PUCCH_PartOne(num_Pilots_slot,
                               Nrb_sc,
                               Nul_symb,
                               indexPilot,
                               (S16)ns,
                               r_alpha_uv,
                               &a[num_Pilots_slot* Nrb_sc * ns * 2],
                               w_bar_ptr);
#else
        ul_Pilot_PUCCH_PartOne_A(num_Pilots_slot,
                               Nrb_sc,
                               Nul_symb,
                               indexPilot,
                               (S16)ns,
                               r_alpha_uv,
                               &a[num_Pilots_slot* Nrb_sc * ns * 2],
                               w_bar_ptr);
#endif
    }// ns
    return;
}

#ifndef CEVA_INTRINSICS
//-------------------------------------------------------------------------------------------
/** @brief LTE Base Station Receiver: Generate relevant pilot information for PUCCH
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param pULPilot_Pucch  - Structure containing input and output parameters
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction |Length   |Type        |Data Format |Valid Range |Units |
 *  |              |Standard |          |         |            |            |            |      |
 *  ---------------------------------------------------------------------------------------------
 *  |pULPilot_Pucch|         |pInput    |1        |pStructure  |            |            |      |
 *  ---------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------
 *  |Block Name          |gen_ul_Pilot_info     |
 *  ---------------------------------------------
 *  |Output Streams      |pULPilot_Pucch        |
 *  ---------------------------------------------
 *  |Config Parameters   |pULPilot_Pucch        |
 *  ---------------------------------------------
 *  |Status Parameters   |none                  |
 *  ---------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Generate relevant pilot information for PUCCH.
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 v8.4.0, Sec.5.5, "Reference signals"
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
///////////////////////////////////////////////////////////////////////////////////
void gen_ul_Pilot_info(ULRXINPUTPARAMS *pULPilot_Pucch)
{
    S32 length_cvec;
    S32 i, ns, isym, Nul_symb;
    S16 fPUCCH_ss, fss;
    S16 TabPwr[8] = {1, 2, 4, 8, 16, 32, 64, 128};
    S16 *fgh, *uSeqGrpNum, *uSeqGrpNum_GrHop;
    S16 *ncs_cell;
    U8  *cvec;

    fPUCCH_ss   = pULPilot_Pucch->Ncell_id % 30;
    fss         = fPUCCH_ss;
    length_cvec = 8 * 7 * NUM_SLOTS;
    Nul_symb    = pULPilot_Pucch->Nul_symb;

    fgh              = (S16*)(&pULPilot_Pucch->Pucch_fgh[0]);
    uSeqGrpNum       = (S16*)(&pULPilot_Pucch->Pucch_uSeqGrpNum[0]);
    uSeqGrpNum_GrHop = (S16*)(&pULPilot_Pucch->Pucch_uSeqGrpNum_GrHopping[0]);
    ncs_cell         = (S16*)(&pULPilot_Pucch->Pucch_ncs_cell[0]);
    cvec             = (U8* )(MemAlloc(length_cvec, DDR_MEM, 0));

    // Generate Random sequence for entire frame of length 8*7*20
    memset(fgh, 0, NUM_SLOTS * sizeof(S16));                // 20 by 2 byte location (16 bit)
    memset(uSeqGrpNum, 0, NUM_SLOTS * sizeof(S16));
    memset(uSeqGrpNum_GrHop, 0, NUM_SLOTS * sizeof(S16));
    memset(ncs_cell, 0, 7 * NUM_SLOTS * sizeof(S16));

    //Generate sequence for Group Hopping enabled subframes
    PseudoRandomSeqGen(length_cvec, pULPilot_Pucch->Ncell_id / 30, cvec);
    for (ns = 0; ns < NUM_SLOTS; ns++)
    {
        for (i = 0; i <= 7; i++)
        {
            fgh[ns] += cvec[8 * ns + i] * TabPwr[i]; //(shl(1, i));
        }
        fgh[ns]              = fgh[ns] % 30;
        uSeqGrpNum_GrHop[ns] = (fgh[ns] + fss) % 30;
    }

    // Generate Random sequence for entire frame of length 8*7*19+8*7
    PseudoRandomSeqGen(length_cvec, pULPilot_Pucch->Ncell_id, cvec);

    for (ns = 0; ns < NUM_SLOTS; ns++)
    {
        //Generate sequence for Group Hopping disabled subframes
        uSeqGrpNum[ns] = fss % 30;
        
        //Generate ncs_cell for all PUCCH users and all slots
        for (isym = 0; isym < Nul_symb; isym++)
        {
            for (i = 0; i <= 7; i++)
                ncs_cell[isym + Nul_symb * ns] += cvec[8 * Nul_symb * ns + 8 * isym + i] * TabPwr[i]; //(shl(1, i));
        }
    }

    MemFree(cvec, DDR_MEM);
    return;
}
#endif //#ifndef CEVA_INTRINSICS

//-------------------------------------------------------------------------------------------
/** @brief LTE Base Station Receiver: Generate relevant pilot information for PUCCH
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param pULPilot_Pucch  - Structure containing input parameters
 *  @param cvec            - Random Sequence
 *  @param nOc             - Orthogonal Sequence Index (DATA)
 *  @param nOcBar          - Orthogonal Sequence Index (PILOTS)
 *  @param nCs             - cyclic shift
 *  @param ncs_cell        - cyclic shift
 *  @param Sdescramb       - Scrambling flag for PUCCH 1, 1A, 1B
 *  @param ChanNum         - INPUT, channel number
 *  @param SRequest        - INPUT, Schedule request flag
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction |Length   |Type        |Data Format |Valid Range |Units |
 *  |              |Standard |          |         |            |            |            |      |
 *  ---------------------------------------------------------------------------------------------
 *  |pULPilot_Pucch|         |pInput    |1        |pStructure  |            |            |      |
 *  ---------------------------------------------------------------------------------------------
 *  |nOc           |         |pOutput   |2        |short       |(1:15:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------
 *  |nOcBar        |         |pOutput   |2        |short       |(1:15:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------
 *  |Sdescramb     |         |pOutput   |2        |short       |(1:15:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------
 *  |nCs           |         |pOutput   |14       |short       |(1:15:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------
 *  |ncs_cell      |         |pOutput   |14       |short       |(1:15:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------
 *  |ChanNum       |         |Input     |1        |long        |(1:15:0)    |[0:100]     |      |
 *  ---------------------------------------------------------------------------------------------
 *  |SRequest      |         |Input     |1       |short       |(1:15:0)    |[0:1]        |      |
 *  ---------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------------
 *  |Block Name          |gen_ul_Pilot_info                   |
 *  |Input Streams       |cvec                                |
 *  -----------------------------------------------------------
 *  |Output Streams      |nCs, ncs_cell,nOc,nOcBar,           |
 *  |                    |Sdescramb                           |
 *  -----------------------------------------------------------
 *  |Config Parameters   |pULPilot_Pucch, ChanBNum, SRequest  |
 *  -----------------------------------------------------------
 *  |Status Parameters   |none                                |
 *  -----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Generate relevant pilot information for PUCCH.
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 v8.4.0, Sec.5.5, "Reference signals"
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
///////////////////////////////////////////////////////////////////////////////////
void gen_ul_Pilot_info1(ULRXINPUTPARAMS *pULPilot_Pucch, S16* nOc, S16* nOcBar, S16 *Sdescramb,
                       S16 *nCs, S16 *ncs_cell, S32 ChanNum, S16 SRequest)
{

    S16 ns, isym;
    S16 Mrs_sc, Nul_symb;
    S16 temp1, temp2, temp3;
    S16 c_var, d_var, h_var;
    S16 var1, var2;
    S16 Nprime, ndash, ndashPrev;
    //S16 fPUCCH_ss, fss;
    S16 n1, n2, N2_rb, N1_cs, delta_shift_PUCCH;

    Mrs_sc = pULPilot_Pucch->Nrb_sc;
    Nul_symb = pULPilot_Pucch->Nul_symb; // for shortness

    n1 = pULPilot_Pucch->n1[ChanNum];
    if( SRequest == 1)
        n1 = pULPilot_Pucch->srPUCCHResourceIndex[ChanNum];
    n2 = pULPilot_Pucch->n2[ChanNum];
    N2_rb = pULPilot_Pucch->N2_rb;
    N1_cs = pULPilot_Pucch->N1_cs;
    delta_shift_PUCCH = pULPilot_Pucch->delta_shift_PUCCH;
    if (pULPilot_Pucch->pucchFormat[ChanNum] >= FORMAT2) // Format 2, 2a, 2b
    {
        for (ns = 0; ns < NUM_SLOTS_SUBFRM; ns++)
        {
            // Calculate ndash(ns) = fn(n2_pucch, N2_rb, N1_cs)
            if (ns == 0)
            {
                //even slot number
                if (n2 < Mrs_sc * N2_rb)
                {
                    ndash = n2 % Mrs_sc;
                }
                else
                {
                    ndash = (n2 + N1_cs + 1) % Mrs_sc;
                }
                ndashPrev = ndash;
            }
            else
            {
                //odd slot number
                if (n2 < Mrs_sc * N2_rb)
                {
                    ndash = ((Mrs_sc * (ndashPrev + 1)) % (Mrs_sc + 1)) - 1;
                }
                else
                {
                    ndash = (Mrs_sc - 2 - n2) % Mrs_sc;
                }
            }

            for (isym = 0; isym < Nul_symb; isym++) //symbol-in-slot index
            {
                //nCs[ns][isym]
                // Calculate nCs(ns,isymInSlot) = fn(ndash(ns),nCell_cs(ns,isymInSlot))
                nCs[isym + Nul_symb * ns] = (ncs_cell[isym + Nul_symb * ns] + ndash) % Mrs_sc;
            } //isym
        } //ns
    }// else pucch_format
    else
    { //format 1, 1a, 1b
        c_var = 3;
        d_var = 2;
        if (pULPilot_Pucch->CPType == EXTENDED) // extended
        {
            c_var = 2;
            d_var = 0;
        }
        var1 = c_var * N1_cs / delta_shift_PUCCH;
        var2 = c_var * Mrs_sc / delta_shift_PUCCH;
        if (n1 < var1)
            Nprime = N1_cs;
        else
            Nprime = Mrs_sc;
        for (ns = 0; ns < NUM_SLOTS_SUBFRM; ns++)
        {
            // Calculate ndash(ns) = fn(n2_pucch, N2_rb, N1_cs)
            if (ns == 0)
            {
                //even slot number
                if (n1 < var1)
                {
                    ndash = n1;
                }
                else
                {
                    ndash = (n1 - var1) % (var2);
                }
                ndashPrev = ndash;
            }
            else
            {
                //odd slot number
                if (n1 >= var1)
                {
                    ndash = ((c_var * (ndashPrev + 1)) % (var2 + 1)) - 1;
                }
                else
                {
                    h_var = (ndashPrev + d_var) % (c_var * Nprime
                        / delta_shift_PUCCH);
                    temp1 = divide_floor(h_var, c_var);
                    ndash = temp1 + (h_var % c_var) * Nprime
                        / delta_shift_PUCCH;
                }
            }

            Sdescramb[ns] = 1; // Indicates multiplying by exp(-j*PI/2)
            if ((ndash % 2) == 0)
                Sdescramb[ns] = 0; // Indicates multiplying by 1

            nOcBar[ns] = divide_floor(ndash*delta_shift_PUCCH, Nprime);
            nOc[ns] = nOcBar[ns];
            if (pULPilot_Pucch->CPType == NORMAL)
            {
                temp2 = nOcBar[ns] % delta_shift_PUCCH;
            }
            else // Extended
            {
                temp2 = nOcBar[ns];
                nOc[ns] = 2 * nOc[ns];
            }

            for (isym = 0; isym < Nul_symb; isym++) //symbol-in-slot index
            {
                // Calculate nCs(ns,isymInSlot) = fn(ndash(ns),nCell_cs(ns,isymInSlot))
                temp1 = ndash * delta_shift_PUCCH;
                temp3 = (temp1 + temp2) % Nprime;
                nCs[isym + Nul_symb * ns] = (ncs_cell[isym + Nul_symb * ns] + temp3) % Mrs_sc;
            } //isym
        } //ns
    }

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief LTE Base Station Receiver: Generate relevant pilot information for PUCCH
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param pULPilot_Pucch  - Structure containing input parameters
 *  @param r_alpha_uv      - Reference Signal Sequence
 *  @param u[]             - Sequence-group hopping number
 *  @param nCs[]           - Cyclic shift
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction |Length   |Type        |Data Format |Valid Range |Units |
 *  |              |Standard |          |         |            |            |            |      |
 *  ---------------------------------------------------------------------------------------------
 *  |pULPilot_Pucch|         |pInput    |1        |pStructure  |            |            |      |
 *  ---------------------------------------------------------------------------------------------
 *  |r_alpha_uv    |         |pOutput   |168      |pComplex    |(1:0:15)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------
 *  |u             |         |pInput    |20       |short       |(1:15:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------
 *  |nCs           |         |pInput    |14       |short       |(1:15:0)    |Full Range  |      |
 *  ---------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------
 *  |Block Name          |gen_ul_Pilot_info2     |
 *  ---------------------------------------------
 *  |Input Streams       |u, nCs                 |
 *  ---------------------------------------------
 *  |Output Streams      |r_alpha_uv            |
 *  ---------------------------------------------
 *  |Config Parameters   |pULPilot_Pucch        |
 *  ---------------------------------------------
 *  |Status Parameters   |none                  |
 *  ---------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Generate relevant pilot information for PUCCH.
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 v8.4.0, Sec.5.5, "Reference signals"
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
///////////////////////////////////////////////////////////////////////////////////

void gen_ul_Pilot_info2_part1(S16 Mrs_sc, S16 *rbar_uv, S32 uSlotNum)
{
    S16 n;
    S16 sign;
    S32 index, L_temp1;

    for (n = 0; n < Mrs_sc; n++)
    {
        //Phi_mat[n+u*Nrb_sc]*PI/4.0 = Phi_mat[n+u*Nrb_sc]*2*PI/8.0
        //                           = 2*PI*512*Phi_mat[n+u*Nrb_sc]/4096.0
        //Note Table_FFT16b_4096 is exp(-j*2*PI/N)
        //Need to negate Sin result, when Phi_mat[n+u*Nrb_sc] is > 0
        L_temp1 = Phi_mat[n + uSlotNum] * 512;
        sign = 1;
        if (Phi_mat[n + uSlotNum] < 0)
        {
            sign = 0;
        }

        index = (S16)L_abs(L_temp1);

        rbar_uv[2 * n] = Table_FFT16b_4096[2 * index];
        rbar_uv[2 * n + 1] = Table_FFT16b_4096[2 * index + 1];
        if (sign == 1)
        {
            rbar_uv[2 * n + 1] = negate(rbar_uv[2 * n + 1]);
        }
    }
}

void gen_ul_Pilot_info2_part2(S16 Mrs_sc, S16 Nul_symb, S16 *nCs, S16 *rbar_uv, S16* r_alpha_uv)
{
    S16 n, l;
    S16 temp1, temp2;
    S16 ctemp1, stemp1;
    S32 index1;

    for (l = 0; l < Nul_symb; l++)
    {
        index1 = l * PRB_24;
        for (n = 0; n < Mrs_sc; n++)
        {
            //  alpha[0] = 2.0 * PI * nCs[ns][l] / 12.0;
            //  alpha[1] = 2.0 * PI * nCs[ns][l] / 12.0;
            temp1 = n * nCs[l];

            //temp1 = temp1 % 12;
            if (temp1 >= 12)
            {
                temp2 = mult(shr(temp1,2),10923);
                temp2 = temp2 * 12;
                temp1 = sub(temp1, temp2);
            }
            ctemp1 = W16t0012[2 * temp1];
            stemp1 = negate(W16t0012[2 * temp1 + 1]); // since table has exp(-j*2*PI)

            r_alpha_uv[index1 + 2 * n] = sub(mult(ctemp1, rbar_uv[2 * n]),
                                             mult(stemp1, rbar_uv[2 * n + 1])); // Real

            r_alpha_uv[index1 + 2 * n + 1] = add(mult(ctemp1, rbar_uv[2 * n + 1]),
                                                 mult(stemp1, rbar_uv[2 * n])); // Imag
        }// n (Mrs_sc)
    }//l
}

void gen_ul_Pilot_info2(ULRXINPUTPARAMS *pULPilot_Pucch, S16* r_alpha_uv, S16 *nCs,
                        S16 u[])
{
    S16   ns;
    S16 Mrs_sc, Nul_symb;

#ifndef CEVA_INTRINSICS
    S16 rbar_uv[2 * MAXSUBCARRIERS];
#else
    S16 *rbar_uv;
    rbar_uv = (S16*)(DataBuff_Bank2);
#endif

    Mrs_sc = pULPilot_Pucch->Nrb_sc;
    Nul_symb = pULPilot_Pucch->Nul_symb;

    for (ns = 0; ns < NUM_SLOTS_SUBFRM; ns++)
    {
#ifdef CEVA_INTRINSICS
        gen_ul_Pilot_info2_part1_A(Mrs_sc, rbar_uv, u[ns] * Mrs_sc);
#else
        gen_ul_Pilot_info2_part1(Mrs_sc, rbar_uv, u[ns] * Mrs_sc);
#endif

        // Apply Cyclic shift
#ifdef CEVA_INTRINSICS
        gen_ul_Pilot_info2_part2_A(Mrs_sc, Nul_symb, &nCs[Nul_symb * ns], rbar_uv, &r_alpha_uv[PRB_24 * Nul_symb * ns]);
 //       gen_ul_Pilot_info2_part2(Mrs_sc, Nul_symb, &nCs[Nul_symb * ns], rbar_uv, &r_alpha_uv[PRB_24 * Nul_symb * ns]);
#else
        gen_ul_Pilot_info2_part2(Mrs_sc, Nul_symb, &nCs[Nul_symb * ns], rbar_uv, &r_alpha_uv[PRB_24 * Nul_symb * ns]);
#endif

    }//ns

    return;
}

