//-------------------------------------------------------------------------------------------
/** @file bs_tx_PhysicalChannels.c
 *
 * @brief LTE BS Transmitter, Physical Channel processing from input
 *        codewords to output resource elements
 * @author Mindspeed Technologies
 * @version $Revision: 1.107 $
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
#include "gputil_fx.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#include "global.h"

extern U8 *DataBuff_Bank1a;
extern U8 *DataBuff_Bank1;
extern U8 *DataBuff_Bank2;
extern U8 *DataBuff_Bank3;

#else   // CEVA_INTRINSICS
#include "basic_op_cc.h"
#endif  //CEVA_INTRINSICS

U16 divide_fast(U32 num, U16 denom, U16 div);
U16 divide_fast1(U16 num, U16 denom);

U16 divide_fast(U32 num, U16 denom, U16 div)
{
    S32 Lacc;

    Lacc = num * denom;
    Lacc += 0x4000;
    if(Lacc < 0)
        Lacc = 0x7fffffff;
    Lacc >>= 15;
    if((U32)(Lacc * div) > num)
    {
        Lacc -= 1;
    }
    return (U16)Lacc;
}

U16 divide_fast1(U16 num, U16 denom)
{
    return (mult(num, denom));
}


//-------------------------------------------------------------------------------------------
/** @brief Base Station transmitter, Physical Downlink Control Channels processing, including
 *         Reference pilot generation
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *
 *  @param   a[]                          Pointer to array containing the resource elements for this transmitted symbol
 *  @param   numTxAntennas                number of base station transmit antennas
 *  @param   numPDCCHSymbols              number of symbols used for PDCCH at the start of the subframe
 *  @param   number_of_phich_channels     total number of PHICH Channels assigned in this subframe
 *  @param   CPType                       Prefix type
 *  @param   Ncell_id                     Cell ID
 *  @param   controlIndicatorScaleFactor  CFI scale factor
 *  @param   N_PHICH_Grp                  number of PHICH groups for this subframe
 *  @param   PHICH_Bit                    Pointer to array containing PHICH bit info for PHICH Channels
 *  @param   PHICH_SequenceIndex          Pointer to array containing Sequence Index corresponding to each assigned PHICH Channel
 *  @param   sizePDCCH                    size of all PDCCHs in bytes
 *  @param   startPDCCH                   Pointer to array containing PDCCH information
 *  @param   Grp                          Pointer to array containing assigned PHICH groups
 *  @param   scalefactorPDCCH             PDCCH scale factor
 *  @param   scalefactorPHICH             PHICH scale factor
 *  @param   pilotScaleFactor             pilot scale factor
 *  @param   numGroups                    maximum assigned PHICH group number for this subframe
 *  @param   Ndl_rb                       number of resource blocks in DL
 *  @param   Nrb_sc                       number of resource elements in a resource block
 *  @param   col_a                        number of symbols in resource grid
 *  @param   row_a                        number of sub-carriers/symbol in resource grid
 *  @param   slotNumber                   slot number
 *  @param   stepsize_a                   rounded number of sub-carriers/symbol to next multiple of 32
 *  @param   transmode                    transmission mode
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------------------------------
 *  |Name                       |Name in |Direction   |Length                  |Type          |Data Format |Valid Range      |Units |
 *  |                           |Standard|            |                        |              |            |                 |      |
 *  ---------------------------------------------------------------------------------------------------------------------------------
 *  |a[]                        |        |input/output|pPhy->bsTx.Ndl_rb       |pComplex Array|(1:0:15)    |[-4:+4)          |      |
 *  |                           |        |            |* pPhy->bs.Nrb_sc       |              |            |                 |      |
 *  |                           |        |            |* Ntx_ant               |              |            |                 |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |numTxAntennas              |        |input       |    1                   |Real          |(1:15:0)    |[1:4]            |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |numPDCCHSymbols            |        |input       |    1                   |Real          |(1:15:0)    |[0:3]            |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |number_of_phich_channels   |        |input       |    1                   |Real          |(1:15:0)    |[0:200]          |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |CPType                     |        |input       |    1                   |Real          |(1:15:0)    |[0:1]            |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |Ncell_id                   |        |input       |    1                   |Real          |(1:15:0)    |[0:504]          |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |controlIndicatorScaleFactor|        |input       |    1                   |Real          |(1:1:14)    |[0:32767]        |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |N_PHICH_Grp                |        |input       |    1                   |Real          |(1:15:0)    |[1:25]           |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |PHICH_Bit[]                |        |input       |number_of_phich_channels|pReal         |(1:15:0)    |[0:1]            |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |PHICH_SequenceIndex[]      |        |input       |number_of_phich_channels|pReal         |(1:15:0)    |[0:7]            |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |sizePDCCH                  |        |input       | 1                      |Real          |(1:15:0)    |[0:800]          |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |startPDCCH[]               |        |input       | 800                    |pReal         |(1:15:0)    |[0:255]          |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |Grp[]                      |        |input       |number_of_phich_channels|pReal         |(1:15:0)    |[0:24]           |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |scalefactorPDCCH           |        |input       |  1                     |Real          |(1:1:14)    |[0:32767]        |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |scalefactorPHICH           |        |input       |  1                     |Real          |(1:1:14)    |[0:32767]        |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |pilotScaleFactor           |        |input       |  1                     |Real          |(1:1:14)    |[0:32767]        |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |numGroups                  |        |input       |  1                     |Real          |(1:15:0)    |[0:25]           |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |Ndl_rb                     |        |input       |  1                     |Real          |(1:15:0)    |[6,15,25,50,100] |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |Nrb_sc                     |        |input       |  1                     |Real          |(1:15:0)    |12               |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |col_a                      |        |input       |  1                     |Real          |(1:15:0)    |12, 14           |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |row_a                      |        |input       |  1                     |Real          |(1:15:0)    |12*Ndl_rb        |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |slotNumber                 |        |input       |  1                     |Real          |(1:15:0)    |[0:19]           |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |stepsize_a                 |        |input       |  1                     |Real          |(1:15:0)    |32*ceil(row_a/32)|      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |transmode                  |        |input       |  1                     |Real          |(1:15:0)    |[1:7]            |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |phy_bs_tx_control                 |
 *  ---------------------------------------------------------
 *  |Input Stream        |a, Grp, PHICH_Bit,startPDCCH      |
 *  |                    |PHICH_SequenceIndex               |
 *  --------------------------------------------------------
 *  |Output Stream       |a                                 |
 *  ---------------------------------------------------------
 *  |Config Parameters   |numTxAntennas, numPDCCHSymbols,   |
 *  |                    |number_of_phich_channels, CPType, |
 *  |                    |Ncell_id,  sizePDCCH, N_PHICH_Grp,|
 *  |                    |controlIndicatorScaleFactor,      |
 *  |                    |scalefactorPDCCH, scalefactorPHICH|
 *  |                    |pilotScaleFactor,  numGroups,     |
 *  |                    |Ndl_rb, Nrb_sc, col_a, row_a,     |
 *  |                    |SlotNumber, stepsize_a, transmode |
 *  ---------------------------------------------------------
 *  |Status Parameters   |none                              |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *   Base Station transmitter, physical downlink control channels (PCFI, PHICH and PDCCH) processing.
 *   This function computes resource element assignment for all above three types of channels
 *   It performs modulation, layer mapping/precoding, and mapping to resource elements for each of these
 *   channels. This function also, calls a sub function to generate reference DL pilots and mapped them to
 *   the resource grid
 *
 *  This performs the processing of these channel data streams as described in [1], from the input
 *  bitstream through to the IQ samples mapped to the resource grid.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *   [1] 3GPP TS 36.211 Rel.8, Sec.6.7, 6.8 and 6.9, "Physical Channels and Modulation"<BR>
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void phy_bs_tx_control1(S16* a[], S16 numTxAntennas, S16 numcols_a, U32 number_of_phich_channels,
                       S16 CPType, U32 Ncell_id, S16 controlIndicatorScaleFactor, S16 N_PHICH_Grp,
                       S16* PHICH_Bit, S16* PHICH_SequenceIndex, S16 sizePDCCH, U8* startPDCCH,
                       S16* Grp, S16 scalefactorPDCCH, S16 scalefactorPHICH, S16 pilotScaleFactor,
                       S16 numGroups, S16 Ndl_rb, S16 Nrb_sc, S16 col_a, S16 row_a, S16 slotNumber,
                       S16 stepsize_a, S16 numCtrlSymbols, U32 transmode, S16 *StartCCECtl,
                       S16 *NumCCECtl, U32 number_of_pdcch_channels, S16 *y1,  U16 *locBytesSymb, U16 *numbytes, S16 *l_k_Isc)
{
    S32 Temp1;
    S16 *scalePhichGroup;
    S16 scaleFactor;
    S16 *y_tildePHICH;
    S32 i, j, k, m, n, mi;
    U32 ichan;
    S16 phich_present_flag;
    S16 n1_phich;
    S16 *d0_IQ_wz, *d0_IQ_wz_cur;
    S16 *cvecPhich_pos, *cvecPhich_neg;
    S16* chanNum;
    S16 offsetGrp;
    S16 N_PHICH_Grp_Prime;
    S16 numPHICH, Nsf_PHICH;
    S16 m_c;
    S16 numPDCCHQuadruplets;

    S16* k_l_row0;
    S16* k_l_row1;

    //Add PCFICH into the ResourceGrid
    //Does not need external bitstreams
    U8 *b_0;
    S16 *d0_IQ;
    U32 M0symb;
    U16 numcols_k_l;
    U32 stc_alloc_length = 0;
    U8 *cvec, *cvecByte;
    S16 ncol_y1;
    U32 temp, temp1;
    S16 temp16, sizePhichIQ;
    S16 *y, *y_cur, *a_cur, *y_bar;
    U32 cinit;
    U16 utemp16;
    S16 k_bar, kk_bar, kk_bar_mod;
    U32 *dwPtr1, *dwPtr2;

    S16 antenna_index;
    S16 NUM = 0;
    S16 n_bar, n_bar_mod;
    S16 temp2, temp3;
    S16 *RG_Available_PHICH, *EffectiveNumPHICH;
    S16 *RG_Assigned_Tx, *RG_Assigned_PCFIH, *RG_Assignment;
    U8 *bPtr1;
    S16 *wPtr1, *wPtr2, *wPtr3;
    S16 *d0_IQ_wz0;

    // PDCCH
    S16 *k0_sym; // to track filling within each symbol
    S16 *PTR; // pointer within RB
    S16 *PTR_RB; // pointer within RG
    S16 *numRePerSymbol; // simce max numPDDCHSymbols is 3
    S16 isym, Ndl_symb;
    S16 max_available_reg = 0;
    S16 *ni, *ll;


    U32 length_b0;
    U32 index;
    S16 RowTCSubblock, Length;
    U32 _size;
    S16 *y1p;
    U8* in_bytes;
    S32 irow, icol, iv;
    volatile U32 _size11;
    S16 ND;
    S16 *tab_ptrPdcch;

#ifndef CEVA_INTRINSICS
    S16 reg_1, reg_2, cond;
    S16 offset;
#endif

#ifdef DEBUG_PRINT
    S16 temp_R = 0, temp_I = 0;
    S16 NilBytes;
#endif // DEBUG_PRINT



#ifdef WIN32
    if(numCtrlSymbols <= 0)
    {
        return;
    }
#endif // _WIN32

    if (numTxAntennas != 4) // 16 is from PCFICH
    {
        numPDCCHQuadruplets = ((numCtrlSymbols * Ndl_rb * Nrb_sc) >> 2) - Ndl_rb - 4;
    }
    else
    {
        numPDCCHQuadruplets = ((numCtrlSymbols * Ndl_rb * Nrb_sc) >> 2) - (Ndl_rb << 1) - 4;
    }

    tab_ptrPdcch = NULL;
    switch (Ncell_id % 3)
    {
        case 0:
            tab_ptrPdcch = REinSubfrmSym_PDCCH_ROT0;
            break;
        case 1:
            tab_ptrPdcch = REinSubfrmSym_PDCCH_ROT1;
            break;
        case 2:
            tab_ptrPdcch = REinSubfrmSym_PDCCH_ROT2;
            break;
    }


    /////////////////////////////////////////////////////////////////////////////////////////
    // Mallocs
    /////////////////////////////////////////////////////////////////////////////////////////
    temp = 2 * Ndl_rb - 4; //to account for RG_Available_PHICH
    _size = 3 * MAX_PHICH * 4 * 4 * sizeof(short) + 6 * MAX_PHICH * 4 * 4 * sizeof(short) + 9
        * MAX_PHICH * 4 * 4 * sizeof(short) + 26 * sizeof(short) + temp * sizeof(short) + 24
        * sizeof(short) + MAX_PHICH * sizeof(short) + //!!! size of cvecPhich
        26 * sizeof(short) + 32 + 3 * 4 + 16 * 2 * 4 + 64 * 2 + (4 * 3 * Ndl_rb + 7 * 4 + 12)
        * sizeof(short);

    utemp16 = ((numPDCCHQuadruplets >> 5) + 1) << 5;
    temp1 = RUP32B(utemp16 + 2 * utemp16 * sizeof(short) +
        32 * utemp16 * sizeof(short) + 4 * 3 * Ndl_rb * sizeof(short));

    if (_size < temp1)
        _size = temp1;

    _size += 6 * row_a * sizeof(short);

#ifndef CEVA_INTRINSICS
    d0_IQ_wz0 = MemAlloc(_size, DDR_MEM, 0);
    k_l_row0 = d0_IQ_wz0 + (_size - 6 * row_a * sizeof(short)) / 2;
    k_l_row1 = k_l_row0 + 3 * row_a;
#else // CEVA_INTRINSICS

    index = stepsize_a * numTxAntennas * 2 * 2 + 3 * 2 * number_of_phich_channels + 800;

    _size11 = index + (4 * number_of_pdcch_channels) +
              2*(800 + 3 + 3 + 3*row_a);

    _size11 >>= 5;
    _size11 += 1;
    _size11 <<= 5;

    temp1 = (U32)&DataBuff_Bank1a[0];
    temp1 += _size11;

    d0_IQ_wz0 = (S16*)temp1;
    k_l_row0 = (S16*)(d0_IQ_wz0 + (_size - 6 * row_a * sizeof(short))/2);
    k_l_row1 = (S16*)(k_l_row0 + 3 * row_a);
#endif // CEVA_INTRINSICS

    memset(d0_IQ_wz0, 0, (9 * MAX_PHICH * 4 * 4 + 26 + temp) * sizeof(short));

    EffectiveNumPHICH = d0_IQ_wz0 + 9 * MAX_PHICH * 4 * 4;
    RG_Available_PHICH = EffectiveNumPHICH + 26;
    y_tildePHICH = RG_Available_PHICH + temp;
    d0_IQ_wz = y_tildePHICH + 6 * MAX_PHICH * 4 * 4;
    cvecPhich_pos = d0_IQ_wz + 3 * MAX_PHICH * 4 * 4;
    cvecPhich_neg = cvecPhich_pos + 12;
    chanNum = cvecPhich_neg + 12;
    scalePhichGroup = chanNum + MAX_PHICH;
    b_0 = (U8*) (scalePhichGroup + 26);

    cvec = b_0 + 8;
    cvecByte = b_0 + 40;
    y = (S16*) (b_0 + 44);
    d0_IQ = y + 64;

    RG_Assigned_Tx = d0_IQ + 64;
    RG_Assigned_PCFIH = RG_Assigned_Tx + 4 * 3 * Ndl_rb;
    RG_Assignment = RG_Assigned_PCFIH + 4;
    ni = RG_Assignment + 12;
    ll = ni + 4;
    k0_sym = ll + 4;
    PTR = k0_sym + 4;
    PTR_RB = PTR + 4;
    numRePerSymbol = PTR_RB + 4;

    memset(RG_Assigned_Tx, 0, (4 * 3 * Ndl_rb + 7 * 4 + 12) * sizeof(short));

    if (Ndl_rb <= 10)
    {
        Temp1 = CFI_DW[numCtrlSymbols - 2];
    }
    else
    {
        Temp1 = CFI_DW[numCtrlSymbols - 1];
    }

    // For each input codeword... {0,1} (for each user and channel...%!!!!!! for now, assume DSCH=downlink shared channel only)
    // scrambler (bits)
    // modulation mapper (bits to constellation points)
    // layer mapper (constellation points to layers)
    // precoder (layers to antenna ports)
    // For each tx antenna port... {1,2,4}
    //  resource element mapper (antenna ports to resource blocks)
    // end //antenna port
    // end //codewords for each user and channel
    //
    //
    //Modulate Data
    //

    //scramble PCFIH bits
    // cinit = floor(ns/2)*2^9 + Ncell_id;
    // initial values depends on user, cell, codeword, slot number
    utemp16 = (slotNumber >> 1) + 1;
    cinit = utemp16 * (((Ncell_id << 1) + 1) << 9) + Ncell_id;

    //    scrambler_byte(cinit, (S8*) b0, (S8*) b_0, 4);
    //run the pseudo random sequence generator for length_bx bits and scramble
    // the input sequence
    PseudoRandomSeqGen(32, cinit, cvec);
    bPtr1 = cvec;
    for (i = 0; i < 4; i++)
    {
        cvecByte[i] = 0;
        for (j = 0; j < 8; j++)
        {
            cvecByte[i] |= (1 & *bPtr1++) << (7 - j);
        }
    }
    dwPtr1 = (U32*) cvecByte;
    dwPtr2 = (U32*) b_0;

    *dwPtr2 = *dwPtr1 ^ Temp1;

#ifdef DEBUG_PRINT
    fprintf(f_data_cfi_mod0_b0, "%d\n", Temp1);
    for (i = 0; i < 4; i++)
    {
        //        fprintf(f_data_cfi_mod0_b0, "%d\n", b0[i]);
        fprintf(f_data_cfi_mod1_b_0, "%d\n", b_0[i]);
    }
#endif // DEBUG_PRINT

    Ndl_symb = (numcols_a >> 1);
    temp2= numCtrlSymbols;
    if(numCtrlSymbols != 0)
        temp2 = 1;
    for (isym = 0; isym < temp2; isym++)
    {

        for (i = 0; i < numTxAntennas; i++)
        {
            memset(a[i + isym * numTxAntennas], 0, stepsize_a * 4);
        }
    }

    //  map scrambled codeword to constellation points
    tx_Mapper(b_0, 16, 0, 1, d0_IQ);

    ncol_y1 = 16;
    if (numTxAntennas == 1) // No layer Mapping needed for STC
    {
        memcpy((S8*) y, (S8*) d0_IQ, 32 * 2);
    }
    else
    {
        STC(d0_IQ, 16, numTxAntennas, y);
    }

    //==================================================================================
    // (2) Get k_L mapping of these control elements in the resource grid
    //
    k_bar = (Nrb_sc >> 1) * ((U16) Ncell_id % (Ndl_rb << 1));

    antenna_index = 0; // Ntx_ant =<2;
    if (numTxAntennas == 4) // symbol 1
    {
        antenna_index = 1; // Ntx_ant =4;
    }
    temp = 0;
    temp1 = Ndl_rb * Nrb_sc;
    for (j = 0; j < 4; j++)
    {
        temp16 = (S16) (temp >> 1);
        temp += Ndl_rb;
        kk_bar = k_bar + (S16) temp16 * Nrb_sc / 2;
        kk_bar_mod = kk_bar % (S16) temp1;
        for (k = 0; k < 4; k++)
        {
            k_l_row0[k + (j << 2)] = tab_ptrPdcch[3 * 12 * antenna_index + k] + kk_bar_mod;
        }
//        temp16 = divide_floor(kk_bar_mod, 6); // (4 REs +2 pilots)
        temp16 = divide_fast((U32)kk_bar_mod, (32768/6), 6);

        RG_Assigned_Tx[4 * temp16] = 1;
        RG_Assigned_PCFIH[j] = temp16;
    }

    memset(k_l_row1, 0, 32);

    //map precoded symbol values to resource elements
    scaleFactor = controlIndicatorScaleFactor;
    y_cur = y;
    temp = 2 * ncol_y1;
    for (mi = 0; mi < numTxAntennas; mi++)
    {
        //loop to copy precoded data symbol values to resource elements, at
        //locations defined by the k_l matrix, first symbol only
        a_cur = a[mi];
        for (i = 0; i < ncol_y1; i++)
        {
            //get frequency index, k
            k = k_l_row0[i];
            //write to resource element (Real)
            a_cur[2 * k] = mult(y_cur[2 * i], scaleFactor);
            //write to resource element (Imag)
            a_cur[2 * k + 1] = mult(y_cur[2 * i + 1], scaleFactor);
        }
        y_cur += temp;
    }
    m_c = 1;
    numPHICH = 8;
    Nsf_PHICH = 4;
    if (CPType == 1) // extended
    {
        N_PHICH_Grp = N_PHICH_Grp << 1;
        numPHICH = 4;
        Nsf_PHICH = 2;
        m_c = 2;
    }

    N_PHICH_Grp_Prime = N_PHICH_Grp;
    if (CPType == 1)
    {
        // N_PHICH_Grp cannot be odd here since shifted left by 1 already
        N_PHICH_Grp_Prime = N_PHICH_Grp >> 1;

    }

    M0symb = m_c * 3 * Nsf_PHICH;
    stc_alloc_length = 2 * number_of_phich_channels * M0symb * numTxAntennas;

    // Symbol 0
    // 2 because there are 2 REG(Quadruplet) per RB
    // due to pilots
    // 4, REG taken by PCFICH
    memset(ll, 0, 6);

    if (CPType == 1) // consider only FDD and non-MBSFN for now
    {
        for (j = 0; j < 3; j++)
        {
            ll[j] = (S16) j;
        }
    }

    //Precompute number of REG not assigned to PCFICH in each of the first 3 symbols
    ni[0] = 2 * Ndl_rb - 4; // Symbol 0

#ifdef CEVA_INTRINSICS
    phy_bs_tx_control1_part4_A(Ndl_rb, RG_Assigned_PCFIH, RG_Available_PHICH);
#else // CEVA_INTRINSICS

    i = 0;
    for (j = 0; j < 2 * Ndl_rb; j++)
    {
        temp = 0;
        for (k = 0; k < 4; k++)
        {
            if (RG_Assigned_PCFIH[k] == j)
            {
                temp = 1;
            }
        }
        if (temp == 0)
        {

            RG_Available_PHICH[i++] = (S16) j;
        }
    }

#ifndef CEVA_INTRINSICS
    if (i != ni[0])
    {
        printf("Error in RG_Available_PHICH, i = %hd, ni[0] = %hd\n", i, ni[0]);
        exit(-1);
    }
#endif // CEVA_INTRINSICS

#endif // CEVA_INTRINSICS

    ni[1] = 3 * Ndl_rb; // no pilots
    if (numTxAntennas == 4) // symbol 1
    {
        ni[1] = 2 * Ndl_rb; // pilots
    }

    ni[2] = 3 * Ndl_rb; // symbol 3, no pilots
    NUM = 0;

    // Revisit later
    for (i = 0; i < N_PHICH_Grp_Prime; i++)
    {
        for (j = 0; j < 3; j++) // 3 REGs (12/4)
        {
            temp1 = divide_floor((S32)(Ncell_id * ni[ll[j]]), ni[0]);
//            temp2 = divide_floor((S16)(j * ni[ll[j]]), 3);
            temp2 = divide_fast((U32)(j * ni[ll[j]]), (32768/3), 3);
            n_bar = (S16) ((temp1 + i + temp2) % ni[ll[j]]);
            if (ll[j] == 0)// symbol 0
            {

                n_bar = RG_Available_PHICH[n_bar];
                n_bar_mod = n_bar * Nrb_sc / 2; // convert to REs
            }
            else if (ll[j] == 1) // symbol 1
            {
                if (numTxAntennas != 4)
                {
                    n_bar_mod = n_bar_mod = n_bar * Nrb_sc / 3; // convert to REs
                }
                else
                {
                    n_bar_mod = n_bar * Nrb_sc / 2; // convert to REs
                }
            }
            else
            {
                n_bar_mod = n_bar * Nrb_sc / 3; // convert to REs
            }

            for (k = 0; k < 4; k++)
            {
                k_l_row0[NUM] = tab_ptrPdcch[3 * 12 * antenna_index + 12 * ll[j] + k] + n_bar_mod;
                k_l_row1[NUM] = ll[j];
                NUM++;
            }
            RG_Assigned_Tx[4 * n_bar + ll[j]] = 1;
        }
    }
    numcols_k_l = NUM / N_PHICH_Grp_Prime; // Per group

    memset(EffectiveNumPHICH, 0, 26 * 2);

    for (ichan = 0; ichan < number_of_phich_channels; ichan++)
    {
        EffectiveNumPHICH[Grp[ichan]] += 1;
    }

    offsetGrp = 0;
    for (i = 0; i < numGroups; i++)
    {
        k = 0;
        for (ichan = 0; ichan < number_of_phich_channels; ichan++)
        {
            if (Grp[ichan] == i)
            {
                chanNum[offsetGrp + k] = (S16) ichan;
                k += 1;
            }
        }
        //scale_factor = pChan->phyChanScale_factor; All PHICH have same scale
        if (k != 0)
        {
            scalePhichGroup[i] = scalefactorPHICH;
        }
        else
        {
            scalePhichGroup[i] = 0;
        }
        offsetGrp += EffectiveNumPHICH[i];
    }

    phich_present_flag = 0;
    if (offsetGrp != 0)
    {
        //Pseudo Random Sequence Generation for PHICH, called once
        phich_present_flag = 1;
        for (i = 0; i < 3 * Nsf_PHICH; i++)
        {
            temp16 = 1 - 2 * cvec[i];
            cvecPhich_pos[i] = temp16 * 11585;
            cvecPhich_neg[i] = -cvecPhich_pos[i];
        }
    }

    if (phich_present_flag == 1)
    {
        sizePhichIQ = 2 * 3 * Nsf_PHICH;
        offsetGrp = 0;
        temp1 = numPHICH >> 1;
        //        y_bar = d0_IQ_wz0 + stc_alloc_length;
        M0symb = m_c * Nsf_PHICH * 3;
        ncol_y1 = (S16) M0symb;
        n1_phich = (S16) (M0symb * numTxAntennas);

        wPtr2 = y_tildePHICH;

        for (m = 0; m < numGroups; m++)
        {
            d0_IQ_wz_cur = d0_IQ_wz;
            for (n = 0; n < EffectiveNumPHICH[m]; n++)
            {

                //Repitition of bits/IQ (BPSK modulation)
                if (PHICH_Bit[chanNum[n + offsetGrp]] == 0)
                {
                    wPtr1 = cvecPhich_pos;
                }
                else
                {
                    wPtr1 = cvecPhich_neg;
                }

                //CDMA Spreading and Randomization
                i = 0;
                temp3 = PHICH_SequenceIndex[chanNum[n + offsetGrp]];
                for (k = 0; k < 3; k++)
                {
                    for (j = 0; j < Nsf_PHICH; j++)// multiply by +/- 1
                    {
                        temp2 = mult(*wPtr1++, W_PHICH[temp3][j]);
                        d0_IQ_wz_cur[i << 1] = temp2;
                        if (temp3 >= (U16) temp1)
                        {
                            d0_IQ_wz_cur[i << 1] = negate(temp2);
                        }
                        d0_IQ_wz_cur[(i << 1) + 1] = temp2;
                        i += 1;
                    }
                }
                d0_IQ_wz_cur += sizePhichIQ;
            }
            if (EffectiveNumPHICH[m] != 0)
            {
                if (numTxAntennas == 1 && CPType == 0)
                {
                    wPtr3 = d0_IQ_wz;
                    memset(wPtr2, 0, 2 * M0symb * 2);
                    for (k = 0; k < (S32) M0symb; k++)
                    {
                        wPtr1 = wPtr3;
                        wPtr3 += 2;
                        for (j = 0; j < EffectiveNumPHICH[m]; j++)
                        {
                            // Divide by 8 to avoid saturation
#ifdef CEVA_INTRINSICS
                            wPtr2[2*k] = add(wPtr2[2*k], mult(wPtr1[0], (short)4096));
                            wPtr2[2*k+1] = add(wPtr2[2*k+1], mult(wPtr1[1], (short)4096));
#else // CEVA_INTRINSICS
                            wPtr2[2 * k] += wPtr1[0] >> 3;
                            wPtr2[2 * k + 1] += wPtr1[1] >> 3;
#endif // CEVA_INTRINSICS
                            wPtr1 += 2 * M0symb;
                        }
                    }
                    for (k = 0; k < (S32) (2 * M0symb); k++)
                    {
                        // Scale by Group
                        temp16 = mult(wPtr2[k], scalePhichGroup[m]);
                        // Compensate for division by 8 above
                        wPtr2[k] = shl(temp16, 3);
                    }
                    wPtr2 += 2 * M0symb;
                }
            }
            offsetGrp += EffectiveNumPHICH[m];
        }

        if ((numTxAntennas != 1) || (CPType != 0))
        {
            y = d0_IQ_wz0 + stc_alloc_length;
            y_bar = d0_IQ_wz0 + 2 * stc_alloc_length;

            BsTxPHICH(numTxAntennas, d0_IQ_wz, Nsf_PHICH, numGroups, y, y_bar, CPType, m_c, M0symb,
                      stc_alloc_length, scalePhichGroup, d0_IQ_wz0, EffectiveNumPHICH, y_tildePHICH);
        }

        wPtr1 = y_tildePHICH;
        temp = 2 * ncol_y1;
        Temp1 = 2 * n1_phich;
        j = 0;
        for (i = 0; i < N_PHICH_Grp_Prime; i++)
        {
            y_cur = wPtr1;
            if (EffectiveNumPHICH[i] != 0)
            {
                //map precoded symbol values to resource elements
                for (mi = 0; mi < numTxAntennas; mi++)
                {
                    a_cur = a[mi];
                    for (m = 0; m < ncol_y1; m++)
                    {
                        //get frequency index, k
                        //get time index, j
                        k = k_l_row1[j + m] * col_a + k_l_row0[j + m];
                        k <<= 1;

                        //write to resource element (Real)
                        a_cur[k] = y_cur[2 * m];
                        //write to resource element (Imag)
                        a_cur[k + 1] = y_cur[2 * m + 1];
                    }
                    y_cur += temp;
                }
                wPtr1 += Temp1;
                j += numcols_k_l;
            }
            else
            {
                //zero resource elements
                for (mi = 0; mi < numTxAntennas; mi++)
                {
                    a_cur = a[mi];
                    for (m = 0; m < ncol_y1; m++)
                    {
                        //get frequency index, k
                        //get time index, j
                        k = k_l_row1[j + m] * col_a + k_l_row0[j + m];
                        k <<= 1;
                        //write to resource element (Real)
                        a_cur[k] = 0;
                        //write to resource element (Imag)
                        a_cur[k + 1] = 0;
                    }
                }
                //wPtr1 += Temp1;
                j += numcols_k_l;
            }
        }
    }

    //Add PDCCHs into the ResourceGrid
    numPDCCHQuadruplets = numPDCCHQuadruplets - 3 * N_PHICH_Grp;

    scaleFactor = scalefactorPDCCH;
    length_b0 = sizePDCCH;

    // Check if Nil bytes are needed to meet requirements of Quadruplets
#ifdef DEBUG_PRINT
    NilBytes = 0;
    if (length_b0 < (U32) (numPDCCHQuadruplets))
    {
        NilBytes = (S16) (numPDCCHQuadruplets - length_b0);
    }
#endif // DEBUG_PRINT
    length_b0 = numPDCCHQuadruplets;

    RowTCSubblock = (S16) ((length_b0 >> 5) + 1);
    Length = (S16) (RowTCSubblock << 5);

    y1p = d0_IQ_wz0;

    // No need to loop over channels since PDCCHs are concatenated (bitstream) inside the function
    // First PDCCH
    //==================================================================================
    // (2) Get k_L mapping of these control elements in the resource grid
    //

    if (numTxAntennas != 4) // 1 or 2
    {
        switch (numCtrlSymbols)
        {
            case 1:
                //For 1 Control symbol, two REGs in symbol 0
                RG_Assignment[0] = assignment_1_2_antennas[1];
                RG_Assignment[1] = assignment_1_2_antennas[1];
                max_available_reg = 2;
                break;

            case 2:
                for (n = 0; n < assignment_1_2_antennas[0]; n++)
                {
                    RG_Assignment[n] = assignment_1_2_antennas[1 + n];
                }
                max_available_reg = assignment_1_2_antennas[0];
                break;

            case 3:
                for (n = 0; n < assignment_1_2_antennas[6]; n++)
                {
                    RG_Assignment[n] = assignment_1_2_antennas[7 + n];
                }
                max_available_reg = assignment_1_2_antennas[6];
                break;

            case 4:
                for (n = 0; n < assignment_1_2_antennas[15]; n++)
                {
                    RG_Assignment[n] = assignment_1_2_antennas[16 + n];
                }
                max_available_reg = assignment_1_2_antennas[15];
                break;
        }
    }
    else
    {
        switch (numCtrlSymbols)
        {
            case 1:
                //For 1 Control symbol, two REGs in symbol 0
                RG_Assignment[0] = assignment_4_antennas[1];
                RG_Assignment[1] = assignment_4_antennas[1];
                max_available_reg = 2;
                break;

            case 2:
                for (n = 0; n < assignment_4_antennas[0]; n++)
                {
                    RG_Assignment[n] = assignment_4_antennas[1 + n];
                }
                max_available_reg = assignment_4_antennas[0];
                break;

            case 3:
                for (n = 0; n < assignment_4_antennas[5]; n++)
                {
                    RG_Assignment[n] = assignment_4_antennas[6 + n];
                }
                max_available_reg = assignment_4_antennas[5];
                break;

            case 4:
                for (n = 0; n < assignment_4_antennas[13]; n++)
                {
                    RG_Assignment[n] = assignment_4_antennas[14 + n];
                }
                max_available_reg = assignment_4_antennas[13];
                break;
        }
    } // Ant

#ifdef CEVA_INTRINSICS
    phy_bs_tx_control1_part12_A(Ndl_rb, Nrb_sc, PTR, k0_sym, tab_ptrPdcch, row_a, l_k_Isc, k_l_row1, max_available_reg, RG_Assigned_Tx, RG_Assignment, antenna_index, PTR_RB, numRePerSymbol);
#else // CEVA_INTRINSICS

    m = 0;
    //    for( mi=0; mi < numTxAntennas; mi++)
    for (mi = 0; mi < 1; mi++) //!!!!!! do 1 antenna only for now...
    {
        for (j = 0; j < Ndl_rb; j++)
        {
            //4 is max number of symbols control can take
            for (n = 0; n < 4; n++) // reset at each RB,
            {
                PTR[n] = 0;
            }
            for (n = 0; n < max_available_reg; n++)
            {
                reg_2 = RG_Assignment[n];
                for (i = PTR[reg_2]; i < Nrb_sc; i++)
                {
                    // reg is REG index
                    // RG_Assignment[n] is symbol index
                    if (reg_2 == 0 || (antenna_index == 1 && reg_2 == 1))
                    {
//                        reg_1 = divide_floor(PTR_RB[reg_2], 6);
                        reg_1 = divide_fast((U32)(PTR_RB[reg_2]), (32768/6), 6);
                        cond = RG_Assigned_Tx[4 * reg_1 + reg_2];
                        offset = 6;
                    }
                    else
                    {
                        //                        reg_1 = divide_floor(PTR_RB[reg_2], 4);
                        reg_1 = PTR_RB[reg_2] >> 2;
                        cond = RG_Assigned_Tx[4 * reg_1 + reg_2];
                        offset = 4;
                    }

                    //Pre-increment because of the break statement
                    //would allow to do it at the end of loop
                    PTR[reg_2] += offset;
                    PTR_RB[reg_2] += offset;

                    if (cond == 0) // REG is available
                    {
                        isym = reg_2;
                        numRePerSymbol[isym] = numRePerSymbol[isym] + 4;
                        for (k = 0; k < 4; k++)
                        {
                            k_l_row1[k + m] = isym;
                            //Fill in l_k_Isc, through parsing
                            if (isym < 3)
                            {
                                l_k_Isc[isym * row_a + k + k0_sym[isym]] = tab_ptrPdcch[3 * 12
                                    * antenna_index + 12 * isym + k] + PTR_RB[reg_2] - offset;
                            }
                        } // cond
                        m += 4;
                        if (isym < 3)
                        {
                            k0_sym[isym] += 4;
                        }
                        break;
                    }
                    else
                    {
                        break;
                    } // if reg2
                } //for i
            } // for n
        } //for Nrb
    } // for mi
#endif // CEVA_INTRINSICS

    numcols_k_l = numPDCCHQuadruplets << 2;

    // Since we assume all of the PDCCHs have the same power(as of now, we have not found anything
    // in the specs otherwise), we need to get the scale factor for any PDCCH
    //    BsTxPDCCH(pPhy, a, numTxAntennas, (S16)pPhy->txVector->numCtrlSymbols,
    //              slotNumber, stepsize_a, pChan->phyChanScale_factor, numcols_k_l, numRePerSymbol);

    //void BsTxPDCCH(PLTEPHYDESC pPhy, S16 a[], S16 numTxAntennas, S16 numCtrlSymbols, S16 slotNumber,
    //               S16 stepsize_a, S16 scaleFactor, S16 numcols_k_l, S16* numRePerSymbol)

#ifdef DEBUG_PRINT
    for ( i=0; i< (S32)(length_b0 - NilBytes); i++)
    {
        fprintf(f_data_ctl_mod1_b_0,"%d\n", startPDCCH[i]);
    }
#endif // DEBUG_PRINT

    in_bytes = startPDCCH;

    // Init by -1
    memset(y1, 0xff, 2 * Length);

    ND = (S16) (Length - length_b0);

    // first, copy to vector (length Row x Col), starting with ND
    // dummy <NULL> bits if necessary to pad to this length
    for (j = 0; j < ND; j++)
    {
        y1[j] = -2;
    }

    for (k = 0; k < (S32) number_of_pdcch_channels; k++)
    {
        for (j = 0; j < (S32) (NumCCECtl[k]); j++)
        {
            y1[j + ND + StartCCECtl[k]] = (U16) in_bytes[j + StartCCECtl[k]];

        }
    }

    //Step (1.4) do 1st kind of inter-column permutation
#ifdef CEVA_INTRINSICS
    phy_bs_tx_control1_part15_A(y1, y1p, RowTCSubblock);
#else // CEVA_INTRINSICS

    for (irow = 0; irow < RowTCSubblock; irow++)
    {
        j = irow << 5;
        for (icol = 0; icol < 32; icol++)
        {
            y1p[j + icol] = y1[j + CA_RMCTC_InterColPermConv[icol]];
        }
    }
#endif // CEVA_INTRINSICS


    //Step (1.5) read sub-block interleaver output bits from columns
    //vector of bits, v, = consecutive columns of bits from yp (transposed
    //for row vector)
#ifdef CEVA_INTRINSICS
    if(Length == 32)
        phy_bs_tx_control1_part16_32_A(y1, y1p, Length);
    else if(Length == 64)
        phy_bs_tx_control1_part16_64_A(y1, y1p, Length);
    else if(Length == 96)
        phy_bs_tx_control1_part16_96_A(y1, y1p, Length);
    else if(Length >= 128)
        phy_bs_tx_control1_part16_A(y1, y1p, Length);
    else
    {
        iv = 0; //initial index to output bit
        for (icol = 0; icol < 32; icol++)
        {
            for (irow = 0; irow < Length; irow += 32)
            {
                i = irow + icol;
                // Keep only non Null
                if (y1p[i] != -2)
                {
                    y1[iv++] = y1p[i];

                }

            }
        }
    }

#else // CEVA_INTRINSICS

    iv = 0; //initial index to output bit
    for (icol = 0; icol < 32; icol++)
    {
        for (irow = 0; irow < Length; irow += 32)
        {
            i = irow + icol;
            // Keep only non Null
            if (y1p[i] != -2)
            {
                y1[iv++] = y1p[i];

            }

        }
    }
#endif // CEVA_INTRINSICS

    //  Cyclic shift
    index = Ncell_id % length_b0;
    memcpy(y1p, &y1[index], (length_b0 - index) * sizeof(S16));
    memcpy(&y1p[length_b0 - index], y1, index * sizeof(S16));

    // Parsing the bits per symbol
    // For example if numCtrlSymbols = 3
    // First numRePerSymbol[0]*Qm bits will belong to symb 0
    // Then, next numRePerSymbol[1]*Qm bits will belong to symb 1
    // Then, next numRePerSymbol[2]*Qm bits will belong to symb 2
    // These bytes will be stored in sby_0
    locBytesSymb[0] = 0;
    numbytes[0] = 0;
    for (isym = 1; isym < numCtrlSymbols; isym++)
    {
        numbytes[isym] = 0; // to track # of bits per symbol
        locBytesSymb[isym] = (numRePerSymbol[isym - 1] >> 2) + locBytesSymb[isym - 1];
    }


#ifdef CEVA_INTRINSICS
    phy_bs_tx_control1_part18_A(numcols_k_l, k_l_row1, numbytes, y1p, y1, locBytesSymb);
#else // CEVA_INTRINSICS
    for (i = 0; i < (S32) (numcols_k_l >> 2); i++)
    {
        _size11 = locBytesSymb[k_l_row1[i << 2]] + numbytes[k_l_row1[i << 2]];
        y1[_size11] = y1p[i];
        numbytes[k_l_row1[i << 2]] += 1;
    }
#endif // CEVA_INTRINSICS

    {
        U8 *ib_0;
        S16 *d0IQ, *y_precoder;
        U8 *bPtr1, *bPtr2;
        U8* uu_loc;
        U32 temp1;

        temp1 = ((length_b0 >> 5) + 1) << 5;

#ifndef CEVA_INTRINSICS
        uu_loc = MemAlloc(68*800, DDR_MEM, 0);
#else // CEVA_INTRINSICS
        uu_loc = (U8*)&DataBuff_Bank2[0];
#endif // CEVA_INTRINSICS

        ib_0 = (U8*) uu_loc;
        y1p = (S16*) (ib_0 + temp1);
        d0IQ = y1p + temp1;
        y_precoder = d0IQ + 16 * temp1;

        bPtr1 = ib_0;
        bPtr2 = (U8*) y1;
        for (i = 0; i < (S32) length_b0; i++)
            bPtr1[i] = bPtr2[i*2];

        DlPilot((S16) (Ncell_id), CPType, Ndl_rb, Ndl_symb, Nrb_sc, numTxAntennas, pilotScaleFactor, 0, slotNumber, a);

        ControlProcessing(0, transmode, row_a, scaleFactor, numTxAntennas, stepsize_a, numbytes,
                            ib_0, locBytesSymb, d0IQ, y1, y_precoder, y1p, a, l_k_Isc);

#ifndef CEVA_INTRINSICS
        MemFree(uu_loc, DDR_MEM);
#endif // CEVA_INTRINSICS
    }

#ifndef CEVA_INTRINSICS
    MemFree(d0_IQ_wz0, DDR_MEM);
#endif // CEVA_INTRINSICS

    return;
}


//-------------------------------------------------------------------------------------------
/** @brief Base Station transmitter, Physical Downlink Control Channels processing, including
 *         Reference pilot generation
 *
 *  <1> Hardware Resource:
 *  CEVA
 *
 *
 *  @param   a[]                          Pointer to array containing the resource elements for this transmitted symbol
 *  @param   numTxAntennas                number of base station transmit antennas
 *  @param   numPDCCHSymbols              number of symbols used for PDCCH at the start of the subframe
 *  @param   number_of_phich_channels     total number of PHICH Channels assigned in this subframe
 *  @param   CPType                       Prefix type
 *  @param   Ncell_id                     Cell ID
 *  @param   controlIndicatorScaleFactor  CFI scale factor
 *  @param   N_PHICH_Grp                  number of PHICH groups for this subframe
 *  @param   PHICH_Bit                    Pointer to array containing PHICH bit info for PHICH Channels
 *  @param   PHICH_SequenceIndex          Pointer to array containing Sequence Index corresponding to each assigned PHICH Channel
 *  @param   sizePDCCH                    size of all PDCCHs in bytes
 *  @param   startPDCCH                   Pointer to array containing PDCCH information
 *  @param   Grp                          Pointer to array containing assigned PHICH groups
 *  @param   scalefactorPDCCH             PDCCH scale factor
 *  @param   scalefactorPHICH             PHICH scale factor
 *  @param   pilotScaleFactor             pilot scale factor
 *  @param   numGroups                    maximum assigned PHICH group number for this subframe
 *  @param   Ndl_rb                       number of resource blocks in DL
 *  @param   Nrb_sc                       number of resource elements in a resource block
 *  @param   col_a                        number of symbols in resource grid
 *  @param   row_a                        number of sub-carriers/symbol in resource grid
 *  @param   slotNumber                   slot number
 *  @param   stepsize_a                   rounded number of sub-carriers/symbol to next multiple of 32
 *  @param   transmode                    transmission mode
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------------------------------
 *  |Name                       |Name in |Direction   |Length                  |Type          |Data Format |Valid Range      |Units |
 *  |                           |Standard|            |                        |              |            |                 |      |
 *  ---------------------------------------------------------------------------------------------------------------------------------
 *  |a[]                        |        |input/output|pPhy->bsTx.Ndl_rb       |pComplex Array|(1:0:15)    |[-4:+4)          |      |
 *  |                           |        |            |* pPhy->bs.Nrb_sc       |              |            |                 |      |
 *  |                           |        |            |* Ntx_ant               |              |            |                 |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |numTxAntennas              |        |input       |    1                   |Real          |(1:15:0)    |[1:4]            |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |numPDCCHSymbols            |        |input       |    1                   |Real          |(1:15:0)    |[0:3]            |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |number_of_phich_channels   |        |input       |    1                   |Real          |(1:15:0)    |[0:200]          |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |CPType                     |        |input       |    1                   |Real          |(1:15:0)    |[0:1]            |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |Ncell_id                   |        |input       |    1                   |Real          |(1:15:0)    |[0:504]          |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |controlIndicatorScaleFactor|        |input       |    1                   |Real          |(1:1:14)    |[0:32767]        |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |N_PHICH_Grp                |        |input       |    1                   |Real          |(1:15:0)    |[1:25]           |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |PHICH_Bit[]                |        |input       |number_of_phich_channels|pReal         |(1:15:0)    |[0:1]            |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |PHICH_SequenceIndex[]      |        |input       |number_of_phich_channels|pReal         |(1:15:0)    |[0:7]            |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |sizePDCCH                  |        |input       | 1                      |Real          |(1:15:0)    |[0:800]          |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |startPDCCH[]               |        |input       | 800                    |pReal         |(1:15:0)    |[0:255]          |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |Grp[]                      |        |input       |number_of_phich_channels|pReal         |(1:15:0)    |[0:24]           |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |scalefactorPDCCH           |        |input       |  1                     |Real          |(1:1:14)    |[0:32767]        |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |scalefactorPHICH           |        |input       |  1                     |Real          |(1:1:14)    |[0:32767]        |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |pilotScaleFactor           |        |input       |  1                     |Real          |(1:1:14)    |[0:32767]        |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |numGroups                  |        |input       |  1                     |Real          |(1:15:0)    |[0:25]           |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |Ndl_rb                     |        |input       |  1                     |Real          |(1:15:0)    |[6,15,25,50,100] |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |Nrb_sc                     |        |input       |  1                     |Real          |(1:15:0)    |12               |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |col_a                      |        |input       |  1                     |Real          |(1:15:0)    |12, 14           |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |row_a                      |        |input       |  1                     |Real          |(1:15:0)    |12*Ndl_rb        |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |slotNumber                 |        |input       |  1                     |Real          |(1:15:0)    |[0:19]           |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |stepsize_a                 |        |input       |  1                     |Real          |(1:15:0)    |32*ceil(row_a/32)|      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |transmode                  |        |input       |  1                     |Real          |(1:15:0)    |[1:7]            |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |phy_bs_tx_control2                 |
 *  ---------------------------------------------------------
 *  |Input Stream        |a, Grp, PHICH_Bit,startPDCCH      |
 *  |                    |PHICH_SequenceIndex               |
 *  --------------------------------------------------------
 *  |Output Stream       |a                                 |
 *  ---------------------------------------------------------
 *  |Config Parameters   |numTxAntennas, numPDCCHSymbols,   |
 *  |                    |number_of_phich_channels, CPType, |
 *  |                    |Ncell_id,  sizePDCCH, N_PHICH_Grp,|
 *  |                    |controlIndicatorScaleFactor,      |
 *  |                    |scalefactorPDCCH, scalefactorPHICH|
 *  |                    |pilotScaleFactor,  numGroups,     |
 *  |                    |Ndl_rb, Nrb_sc, col_a, row_a,     |
 *  |                    |SlotNumber, stepsize_a, transmode |
 *  ---------------------------------------------------------
 *  |Status Parameters   |none                              |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *   Base Station transmitter, physical downlink control channels (PCFI, PHICH and PDCCH) processing.
 *   This function computes resource element assignment for all above three types of channels
 *   It performs modulation, layer mapping/precoding, and mapping to resource elements for each of these
 *   channels. This function also, calls a sub function to generate reference DL pilots and mapped them to
 *   the resource grid
 *
 *  This performs the processing of these channel data streams as described in [1], from the input
 *  bitstream through to the IQ samples mapped to the resource grid.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *   [1] 3GPP TS 36.211 Rel.8, Sec.6.7, 6.8 and 6.9, "Physical Channels and Modulation"<BR>
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void phy_bs_tx_control2(S16 isym, S16* a[], S16 numTxAntennas, S16 numcols_a,
                       S16 CPType, U32 Ncell_id, S16 N_PHICH_Grp, S16 sizePDCCH,  S16 scalefactorPDCCH,  S16 pilotScaleFactor,
                       S16 Ndl_rb, S16 Nrb_sc, S16 col_a, S16 row_a, S16 slotNumber,
                       S16 stepsize_a, U32 transmode,  S16 numCtrlSymbols, U32 number_of_pdcch_channels,  S16 *y1,
                       U16 *locBytesSymb, U16 *numbytes, S16 *l_k_Isc)
{
    S16 scaleFactor;
    S16 numPDCCHQuadruplets;

    S32 i;
    S16 Ndl_symb = (numcols_a >> 1);
    U16 utemp16;

    S16 *uu_loc;
    U8 *ib_0;

    U32 length_b0;
    U32 _size;
    S16 *y1p;
    S16 RowTCSubblock, Length;

    S16 *d0IQ, *y_precoder;
    U8 *bPtr1, *bPtr2;

#ifdef CEVA_INTRINSICS
    U32 temp1 = 0;
#endif // CEVA_INTRINSICS

#ifdef DEBUG_PRINT
    S16 temp_R = 0, temp_I = 0;
#endif // DEBUG_PRINT

#ifdef WIN32
    if(numCtrlSymbols <= 0)
    {
        return;
    }
#endif // _WIN32

    if (numTxAntennas != 4) // 16 is from PCFICH
    {
        numPDCCHQuadruplets = ((numCtrlSymbols * Ndl_rb * Nrb_sc) >> 2) - Ndl_rb - 4;
    }
    else
    {
        numPDCCHQuadruplets = ((numCtrlSymbols * Ndl_rb * Nrb_sc) >> 2) - (Ndl_rb << 1) - 4;
    }


    utemp16 = ((numPDCCHQuadruplets >> 5) + 1) << 5;

    _size = RUP32B(utemp16 + 2 * utemp16 * sizeof(short) +
        32 * utemp16 * sizeof(short) + 4 * 3 * Ndl_rb * sizeof(short));


    _size += 6 * row_a * sizeof(short);

#ifndef CEVA_INTRINSICS
    uu_loc = MemAlloc(_size, DDR_MEM, 0);
#else // CEVA_INTRINSICS

    temp1 = (U32)(DataBuff_Bank2);
    uu_loc = (S16*)temp1;
#endif // CEVA_INTRINSICS

    //Add PDCCHs into the ResourceGrid
    if (CPType == 1) // extended
    {
        N_PHICH_Grp = N_PHICH_Grp << 1;
    }

    numPDCCHQuadruplets = numPDCCHQuadruplets - 3 * N_PHICH_Grp;

    scaleFactor = scalefactorPDCCH;
    length_b0 = sizePDCCH;
    length_b0 = numPDCCHQuadruplets;

    RowTCSubblock = (S16) ((length_b0 >> 5) + 1);
    Length = (S16) (RowTCSubblock << 5);



    ib_0 = (U8*) uu_loc;
    y1p = (S16*) (ib_0 + Length);
    d0IQ = y1p + Length;
    y_precoder = d0IQ + 16 * Length;



    bPtr1 = ib_0;
    bPtr2 = (U8*) y1;
    for (i = 0; i < (S32) length_b0; i++)
    {
        *bPtr1++ = *bPtr2;
        bPtr2 += 2;
    }



    if(isym > 0)
    {
        for (i = 0; i < numTxAntennas; i++)
        {
            memset(a[i + isym * numTxAntennas], 0, stepsize_a * 4);
        }
    }

    DlPilot((S16) (Ncell_id), CPType, Ndl_rb, Ndl_symb, Nrb_sc, numTxAntennas,
             pilotScaleFactor, isym, slotNumber, &a[isym * numTxAntennas]);

    ControlProcessing(isym, transmode, row_a, scaleFactor, numTxAntennas, stepsize_a, numbytes, ib_0, locBytesSymb, d0IQ, y1, y_precoder, y1p,
                      a, l_k_Isc);

#ifndef CEVA_INTRINSICS
    MemFree(uu_loc, DDR_MEM);
#endif // CEVA_INTRINSICS

    return;
}

void ControlProcessing(S16 isym, U32 transmode, S16 row_a, S16 scaleFactor, S16 numTxAntennas, S16 stepsize_a, U16 *numbytes, U8 *ib_0, U16 *locBytesSymb,
                       S16 *d0IQ, S16 *y1, S16 *y_precoder, S16 *y1p, S16* a[], S16 *l_k_Isc)
{


    S32 i, j;
    U32 M0symb;
    U32 alloc_length = 0;

#ifdef DEBUG_PRINT
    S16 temp_R, temp_I;
#endif // DEBUG_PRINT

        if (numbytes[isym] != 0)
        {
            M0symb = numbytes[isym] << 2;

            tx_Mapper(&ib_0[locBytesSymb[isym]], M0symb, 0, 1, d0IQ);

#ifdef CEVA_INTRINSICS
            ControlProcessing_part1_A(M0symb, &y1[locBytesSymb[isym]], d0IQ);
#else // CEVA_INTRINSICS

            for (i = 0; i < (S32) M0symb; i++)
            {
                if (y1[locBytesSymb[isym] + (i >> 2)] == -1)
                {
                    d0IQ[i << 1] = 0;
                    d0IQ[(i << 1) + 1] = 0;
                }
            }
#endif // CEVA_INTRINSICS

#ifdef DEBUG_PRINT
            for ( i=0; i< (S32)M0symb; i++)
            {
                fprintf(f_data_mod2_d0,"%f %f\n", d0IQ[2*i]/16384.0, d0IQ[2*i+1]/16384.0);
            }
#endif // DEBUG_PRINT

            if (transmode != TXDIVERSITY) // SINGLE ANTENNA
            {
                alloc_length = M0symb << 1;
                memcpy((S8*) y_precoder, (S8*) d0IQ, alloc_length * sizeof(short));

#ifdef DEBUG_PRINT
                for ( j = 0; j < 1; j++)
                {
                    for ( i=0; i< (S32)M0symb; i++)
                    {
                        fprintf(f_data_mod4_y[j],"%f %f\n",
                            y_precoder[2*(i+j*M0symb)]/16384.0,
                            y_precoder[2*(i+j*M0symb)+1]/16384.0);
                    }
                }
#endif // DEBUG_PRINT
            }
            else // STC
            {
                if (numTxAntennas == 2)
                {
                    if (M0symb % numTxAntennas != 0)
                    {
                        // Add extra zero IQ sample to make it even
                        d0IQ[M0symb << 1] = 0;
                        d0IQ[(M0symb << 1) + 1] = 0;
                    }

                }
                else // 4 antennas
                {
                    i = M0symb % numTxAntennas;
                    if (i != 0)
                    {
                        // Add extra zeros IQ sample to make it multiple of 4
                        memset(&d0IQ[M0symb << 1], 0, i * (sizeof(short) << 1));
                    }
                }

                //stc_alloc_length = numTxAntennas * M0symb << 1;
                {
                    STC(d0IQ, M0symb, numTxAntennas, y_precoder);
                }
            }

            for (j = 0; j < numTxAntennas; j++)
            {

                y1p = y_precoder + (j << 1) * M0symb;

                DLResourceElementMapper(a[j + isym * numTxAntennas], y1p, &l_k_Isc[isym * row_a],
                                        (S16) M0symb, scaleFactor);

#ifdef DEBUG_PRINT
                for (i = 0; i < stepsize_a; i++)
                {
                    // Real
                    temp_R = a[j + isym*numTxAntennas][2*i];
                    // Imag
                    temp_I = a[j + isym*numTxAntennas][2*i+1];
                    fprintf(f_data_mod5_unscaled_a[j],"%f %f\n", temp_R/16384.0, temp_I/16384.0);
                }
#endif // DEBUG_PRINT
            }
        }
}


