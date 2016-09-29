//-------------------------------------------------------------------------------------------
/** @file ResourceElementMapperAssignment.c
 *
 * @brief Maps precoded symbols to Resource Elements
 * @author Mindspeed Technologies
 * @version $Revision: 1.13 $
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
//#include "heaps.h"

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
#else
#include "basic_op_cc.h"
#endif

//-------------------------------------------------------------------------------------------
// ResourceElementMapperSymb()
//
/** @brief Resource Element Mapper for one symbol of PDSCH
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param a        Pointer to resource grid for this antenna and this symbol
 *  @param y        precoded data symbol values, for this antenna
 *  @param k_l_row0 pointer to array of frequency coordinates for mapping consecutive
 *                  complex modulation symbols to the resource grid
 *  @param k_l_row1 pointer to array of time coordinates for mapping consecutive
 *                  complex modulation symbols to the resource grid
 *  @param numrows_k_l -not used-
 *  @param numcols_k_l Number of resource elements to be mapped to the resource grid
 *  @param numrows_a Number of rows in the resource grid (Frequency axis)
 *  @param length_y  Number of resource elements to be mapped to the resource grid
 *  @param scaleFactor scale factor applied to the precoded data symbol values before mapping to the resource grid
 *
 *  @return None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Name             |Name in       |Direction     |Length    |Type             |Data Format |Valid Range |Units  |
 *  |                 |Standard      |              |          |                 |            |            |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |a                |a             |output        |numrows_a |pComplex Array   |(1:1:14)    |[-2:+2)     |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |y                |y             |input         |length_y  |pComplex Array   |(1:1:14)    |[-2:+2)     |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |k_l_row0         |k             |input         |length_y  |pReal Array      |(1:15:0)    |[0:1200]    |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |k_l_row1         |l             |input         |length_y  |pReal Array      |(1:15:0)    |[0:13]      |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |numrows_k_l      |              |input         |1         |Real             |(1:15:0)    |            |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |numcols_k_l      |              |input         |1         |Real             |(1:15:0)    |[0:1200]    |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |numrows_a        |              |input         |1         |Real             |(1:15:0)    |[0:1200]    |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |length_y         |              |input         |1         |Real             |(1:15:0)    |[0:1200]    |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |scaleFactor      |beta          |input         |1         |Real             |(1:1:14)    |[-2:+2)     |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------------------------------------------
 *  |Block Name          |ResourceElementMapperSymb                                         |
 *  -----------------------------------------------------------------------------------------
 *  |Input Streams       |y                                                                 |
 *  -----------------------------------------------------------------------------------------
 *  |Output Streams      |a                                                                 |
 *  -----------------------------------------------------------------------------------------
 *  |Config Parameters   |k_l_row0, k_l_row1, numcols_k_l, numrows_a, length_y, scaleFactor |
 *  -----------------------------------------------------------------------------------------
 *  |Status Parameters   |none                                                              |
 *  -----------------------------------------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * The vector of complex-valued data symbols y is mapped in sequence starting with y[0] to
 * resource elements in the resource grid, as defined in [1]. This function maps into a portion of the resource grid
 * for one symbol and one antenna. The pointer into the resource grid is set up by the calling function.<BR>
 * -  PDSCH mapping skips over resource elements reserved for control channels, broadcast channel,
 *    synchronizing signals and reference signals
 * -  The mapping to resource elements not reserved for other purposes is in increasing order of the frequency
 *    index over the assigned physical resource blocks.
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.6.3.5, "Physical Channels and Modulation"<BR>
 *
 * \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void ResourceElementMapperSymb(S16 *a, S16 *y, S16 *k_l_row0, S16 *k_l_row1, S16 numrows_k_l,
                               S16 numcols_k_l, S16 numrows_a, S32 length_y, S16 scaleFactor)
{
    S32 Map_symb;
    S16 k, l, m;

    //init internal constants

    //init loop counters and pointers
    Map_symb = length_y;

    //sanity check of array sizes

    if (numcols_k_l != Map_symb)
    {
#ifdef _WIN32
        myprintf("error in ResourceElementMapper(): number of elements in y  \
                     (Map_symb) does not match number of elements in k_l");
#endif
    }
    //loop to copy precoded data symbol values to resource elements, at
    //locations defined by the k_l matrix
    for (m = 0; m < Map_symb; m++)
    {
        //get frequency index, k
        k = k_l_row0[m];
        //get time index, l
        l = k_l_row1[m];
        //write to resource element (Real)
        a[2 * l * numrows_a + 2 * k] = mult(y[2 * m], scaleFactor);
        //write to resource element (Imag)
        a[2 * l * numrows_a + 2 * k + 1] = mult(y[2 * m + 1], scaleFactor);
        //        printf("RESOURCE %hd %hd %hd %hd %f %f\n", m, k,l,\
//                        2*k*numcols_a+2*l, y[2*m]/16384.0, y[2*m+1]/16384.0);
    }
    //    printf("end_RS_NEW\n");

}

//-------------------------------------------------------------------------------------------
// ResourceElementMapper_PS()
//
/** @brief Resource Element Mapper for the Physical Synchronization signals
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *
 *  @param   Ndl_rb     number of resource blocks in the downlink resource grid
 *  @param   Nrb_sc     number of subcarriers per resource block
 *  @param   lIndex     symbol index
 *  @param   d          pointer to primary synchronization input sequence
 *  @param   a          pointer to resource grid for output
 *  @param   row_a      row within resource grid to write output
 *  @param   col_a      column within resource grid to write output
 *  @param   SyncTxAntennaNum antenna number used to send synchronization signal
 *  @param   scaleFactor scale factor for synchronization signal
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Name             |Name in       |Direction     |Length    |Type             |Data Format |Valid Range |Units  |
 *  |                 |Standard      |              |          |                 |            |            |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Ndl_rb           |Ndl_rb        |input         |1         |Real             |(0:8:0)     |[6:100]     |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Nrb_sc           |Nrb_sc        |input         |1         |Real             |(0:8:0)     |12          |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |lIndex           |l             |input         |1         |Real             |(1:15:0)    |[0:13]      |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |*d               |d             |input         |72        |pComplex Array   |(1:1:14)    |[-2:+2)     |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |*a               |a             |output        |72        |pComplex Array   |(1:1:14)    |[-2:+2)     |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |row_a            |              |input         |1         |Real             |(1:15:0)    |[0:1200]    |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |col_a            |              |input         |1         |Real             |(1:15:0)    |[0:13]      |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |SyncTxAntennaNum |p             |input         |1         |Real             |(1:15:0)    |[0:3]       |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |scaleFactor      |beta          |input         |1         |Real             |(1:1:14)    |[-2:+2)     |       |
 *  ----------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------------------------------------------
 *  |Block Name          |ResourceElementMapper_PS                                            |
 *  -------------------------------------------------------------------------------------------
 *  |Input Streams       |d                                                                   |
 *  -------------------------------------------------------------------------------------------
 *  |Output Streams      |a                                                                   |
 *  -------------------------------------------------------------------------------------------
 *  |Config Parameters   |Ndl_rb, Nrb_sc, lIndex, row_a, col_a, SyncTxAntennaNum, scaleFactor |
 *  -------------------------------------------------------------------------------------------
 *  |Status Parameters   |                                                                    |
 *  -------------------------------------------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 * For the antenna ports used for transmission of the synchronization signals, the block of complex-valued symbols
 * d is mapped in sequence starting with d[0] to resource elements in the resource grid, as defined in [1].
 * -  The mapping to resource elements is in increasing order of first the frequency
 *    index over the physical resource blocks assigned to synch signals and then the time (symbol) index.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.6.3.5, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void ResourceElementMapper_PS(S16 nDlRb, S16 nRbSc, S16 lIndex, S16 *d, S16 *a, S16 row_a,
                              S16 col_a, S16 SyncTxAntennaNum, S16 scaleFactor)
{
    S16 n;
    S16 startInd;

    /*
     for ( n = 0; n < 61+5+5; n++)
     {
     //=length of sync sequence, d, including 5 reserved symbols before and
     //after
     // !!! assume FDD frame structure only here...
     kIndex = n - 31 + nDlRb * nRbSc / 2   - 5;
     // a(kIndex+CE_MI1, lIndex+CE_MI1, SyncTxAntennaNum+CE_MI1) = d(n+CE_MI1);

     a[2*row_a*col_a*SyncTxAntennaNum + 2*row_a*lIndex + 2*kIndex] = mult(d[2*n],scaleFactor);
     a[2*row_a*col_a*SyncTxAntennaNum + 2*row_a*lIndex + 2*kIndex+1] = mult(d[2*n+1],scaleFactor);

     }
     */

    startInd = 2 * row_a * col_a * SyncTxAntennaNum + 2 * row_a * lIndex + 2 * (nDlRb * nRbSc / 2
        - 36);

    for (n = 0; n < (61 + 5 + 5) * 2; n++)
    {
        a[startInd + n] = mult(d[n], scaleFactor);
    }
}

//-------------------------------------------------------------------------------------------
// DLResourceElementMapper()
//
/** @brief Base Station transmitter, Resource Element Mapper
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param a        Pointer to portion of resource element grid for this antenna and this symbol
 *  @param y        precoded data symbol values, for this antenna
 *  @param k_l      pointer to array of (k,l) coordinates for consecutive complex
 *                  modulation symbols
 *  @param scIndex  frequency index vector corresponding to each precoded data symbol
 *  @param length_y  Length of the Precoded Data Symbols
 *  @param scaleFactor scale factor applied to the precoded data symbol values before mapping to the resource grid
 *
 *  @return None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name             |Name in       |Direction     |Length           |Type             |Data Format |Valid Range  |Units  |
 *  |                 |Standard      |              |                 |                 |            |             |       |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |a                |a             |output        |length_y         |pComplex Array   |(1:1:14)    |[-2:+2)      |       |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |y                |y             |input         |length_y         |pComplex Array   |(1:1:14)    |[-2:+2)      |       |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |scIndex          |              |input         |length_y         |pReal Array      |(1:15:0)    |[0:1200]     |       |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |length_y         |              |input         |1                |Real             |(1:15:0)    |[0:1200]     |       |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |scaleFactor      |beta          |input         |1                |Real             |(1:1:14)    |[-2:+2)      |       |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |DLResourceElementMapper          |
 *  --------------------------------------------------------
 *  |Input Streams       |y                                |
 *  --------------------------------------------------------
 *  |Output Streams      |a                                |
 *  --------------------------------------------------------
 *  |Config Parameters   |scIndex, length_y, scaleFactor   |
 *  --------------------------------------------------------
 *  |Status Parameters   |none                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * The vector of complex-valued data symbols y is mapped in sequence starting with y[0] to
 * resource elements in the resource grid, as defined in [1].<BR>
 * -  PDSCH mapping skips over resource elements reserved for control channels, broadcast channel,
 *    synchronizing signals and reference signals
 * -  The mapping to resource elements not reserved for other purposes is in increasing order of the frequency
 *    index over the assigned physical resource blocks.
 *
 *
 * <5> Cycle Count Formula:
 * 52 + 5*Nsymbols
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.6.3.5, "Physical Channels and Modulation"<BR>
 *
 * \ingroup group_lte_tx_downlink_modulator
 *
 **/
void DLResourceElementMapper(S16 *a, S16 *y, S16 *scIndex, S16 length_y, S16 scale_factor)
{
    S32 Map_symb;
    S32 k, k1;
    S32 k2, k3;
    S16 sf;
    S16 tmpR, tmpI;
    S16 tmpR1, tmpI1;
    S16 tmpR2, tmpI2;
    S16 tmpR3, tmpI3;
    S32 m;

    Map_symb = (S32) (length_y >> 2);
    sf = scale_factor;
    for (m = 0; m < Map_symb; m++)
    {
        //get frequency index, k
        k = (S32) (scIndex[4 * m] << 1);
        k1 = (S32) (scIndex[4 * m + 1] << 1);
        k2 = (S32) (scIndex[4 * m + 2] << 1);
        k3 = (S32) (scIndex[4 * m + 3] << 1);
        tmpR = (S16) mult(y[8 * m], sf);
        tmpI = (S16) mult(y[8 * m + 1], sf);
        tmpR1 = (S16) mult(y[8 * m + 2], sf);
        tmpI1 = (S16) mult(y[8 * m + 3], sf);
        tmpR2 = (S16) mult(y[8 * m + 4], sf);
        tmpI2 = (S16) mult(y[8 * m + 5], sf);
        tmpR3 = (S16) mult(y[8 * m + 6], sf);
        tmpI3 = (S16) mult(y[8 * m + 7], sf);
        a[k] = tmpR;
        a[k + 1] = tmpI;
        a[k1] = tmpR1;
        a[k1 + 1] = tmpI1;
        a[k2] = tmpR2;
        a[k2 + 1] = tmpI2;
        a[k3] = tmpR3;
        a[k3 + 1] = tmpI3;
    }
    if (length_y > 4 * Map_symb)
    {
        k3 = length_y - 4 * Map_symb;
        for (m = k3; m < length_y; m++)
        {
            k = (S32) (scIndex[m] << 1);
            tmpR = (S16) mult(y[2 * m], sf);
            tmpI = (S16) mult(y[2 * m + 1], sf);
            a[k] = tmpR;
            a[k + 1] = tmpI;
        }
    }
    // Ceva cycles: 34680 cycyle for 9744 subcarriers --> 3.56 cycles/subcarrier
}

//-------------------------------------------------------------------------------------------
/** @brief DLResourceElementAssignment - LTE PHY Downlink Resource Element Assignment
 *
 *  <1> Hardware Resource:
 *  Ceva
 *
 *  @param InParam  - Pointer to input structure including all input parameters
 *  @param pResInfo - Pointer to resource block assignments
 *  @param isym     - Current symbol being processed
 *  @param num_RE   - Number of resource blocks used this symbol for this channel
 *  @param loc_RE   - Array containing subcarrier numbers used by this channel

 *
 *  @return status,
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length      |Type       |Data Format |Valid Range |Units|
 *  |            |Standard |              |            |           |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  |InParam     |         |pInput        |1           |pStructure |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  |pResInfo    |         |pInput        |1           |pStructure |(1:15:0)    |[0:99]      |     |
 *  ------------------------------------------------------------------------------------------------
 *  |isym        |         |Input         |1           |Real       |(1:15:0)    |[0:13]      |     |
 *  ------------------------------------------------------------------------------------------------
 *  |num_RE      |         |pOutput       |1           |Real       |(1:15:0)    |[0:12]      |     |
 *  ------------------------------------------------------------------------------------------------
 *  |loc_RE      |         |pOutput       |pPhy->nDlRb*|           |(1:15:0)    |[0:1199]    |     |
 *  |            |         |              |pPhy->nRbSc |Real       |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------
 *  |Block Name          |DLResourceElementAssignment|
 *  --------------------------------------------------
 *  |Input Streams       |None                       |
 *  --------------------------------------------------
 *  |Output Streams      |loc_RE,                    |
 *  --------------------------------------------------
 *  |Config Parameters   |InParam,isym,pResInfo      |
 *  --------------------------------------------------
 *  |Status Parameters   |num_RE                     |
 *  --------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  LTE PHY Downlink Resource Element Assignment
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
#if 0
void DLResourceElementAssignment(PDLRESOURCEPARAM InParam, RESELEMINFO *pResInfo, S16 isym,
                                 S16 *num_RE, S16 *loc_RE, S16 enable)
{
    S16 cond, cond1, cond1a, cond1b, cond1c, cond2, cond2a, cond2b, cond2c, cond3, cond4;
    S16 cond1x, cond2x;
    S32 k;
    S16 k0;
    S32 row;
    S16 length_kIndexRange;
    S16 nPRB, NumRBs;
    S32 n;
    S16 antenna_index;
    S16 subframeNumber = InParam->txSubFrameNumber;
    S16 numRE, physChanType;
    S16 count = 0;
    S16 RBIndexMid6Lo, RBIndexMid6Hi, nRbSc, nDlSymb;
    S32 numEntries;

    S16 *tab_ptrPdsch, *tab_ptrPbch;
    S16 moduloNum, isymSlot;

    RBIndexMid6Lo = InParam->RBIndexMid6Lo;
    RBIndexMid6Hi = InParam->RBIndexMid6Hi;
    nRbSc = InParam->nRbSc;
    nDlSymb = InParam->nDlSymb;
    antenna_index = InParam->Ntx_antennas >> 1;
    numEntries = InParam->numberofEntries;
    physChanType = InParam->channelType;

    isymSlot = isym;
    if (isym >= nDlSymb)
        isymSlot = isym - nDlSymb;

    tab_ptrPbch = NULL;
    switch (InParam->Ncell_id % 3)
    {
        case 0:
            tab_ptrPbch = REinSubfrmSym_PBCH_ROT0;
            break;
        case 1:
            tab_ptrPbch = REinSubfrmSym_PBCH_ROT1;
            break;
        case 2:
            tab_ptrPbch = REinSubfrmSym_PBCH_ROT2;
            break;
    }

    moduloNum = 6; // 1 antenna
    if (InParam->Ntx_antennas != 1) // 2 or 4 antennas
        moduloNum = 3;

    tab_ptrPdsch = NULL;
    switch (InParam->Ncell_id % moduloNum)
    {
        case 0:
            tab_ptrPdsch = REinSubfrmSym_PDSCH_ROT0;
            break;
        case 1:
            tab_ptrPdsch = REinSubfrmSym_PDSCH_ROT1;
            break;
        case 2:
            tab_ptrPdsch = REinSubfrmSym_PDSCH_ROT2;
            break;
        case 3:
            tab_ptrPdsch = REinSubfrmSym_PDSCH_ROT3;
            break;
        case 4:
            tab_ptrPdsch = REinSubfrmSym_PDSCH_ROT4;
            break;
        case 5:
            tab_ptrPdsch = REinSubfrmSym_PDSCH_ROT5;
            break;
    }

    *num_RE = 0;
    numRE = 0;

    // First Determine if we are dealing with a BW that has odd number of RB's
    cond3 = ((RBIndexMid6Hi - RBIndexMid6Lo) == 6);
    cond1a = (subframeNumber == 0);
    cond1b = (isym >= nDlSymb);
    cond1c = (isym <= nDlSymb + 3);
    cond2a = (subframeNumber == 5);
    cond2b = (isym >= nDlSymb - 2);
    cond2c = (isym <= nDlSymb - 1);

    cond1x = (cond1a) && ((cond1b) && (cond1c));
    cond2x = ((cond1a) || (cond2a)) && ((cond2b) && (cond2c));
    for (row = 0; row < numEntries; row++)
    {
        nPRB = pResInfo[row].startRes; //resource block number
        NumRBs = pResInfo[row].numRes; //number of consecutive resource blocks
        //in this assignment
        count = 0;
        for (n = 0; n < NumRBs; n++)
        {
            cond = ((nPRB >= RBIndexMid6Lo) && (nPRB <= RBIndexMid6Hi));
            cond4 = cond3 && ((nPRB == RBIndexMid6Lo) || (nPRB == RBIndexMid6Hi));

            // Allow for Control, Broadcast and Synch Physical Channels
            // before assigning ResourceElements to PDSCH
            cond1 = (cond1x && cond);
            cond2 = (cond2x && cond);

            if ((cond1 || cond2 == 1) && cond4 == 1)
            {
                count++;
            }
            else
            {
                count = 0;
            }

            if (cond1 && (count != 1 || physChanType == PBCH))
            {
                if (physChanType == PBCH)
                {
                    //
                    //PBCH (broadcast channel)
                    //
                    // Add the number of ResourceElements for this ResourceBlock and symbol to PBCH
                    //Npdsch_sc = NumREinSubfrmSym_PBCH[cond4][isym];
                    //numRE     = numRE + Npdsch_sc;
                    k0 = nPRB * nRbSc; //frequency index for first resource element in
                    // the first resource block of this resource assignment ordered pair
                    if (nPRB == RBIndexMid6Lo && cond3 == 1)
                    {
                        k0 += nRbSc >> 1;
                    }

                    length_kIndexRange = NumREinSubfrmSym_PBCH[cond4][isym];
                    numRE = numRE + length_kIndexRange;
                    if(enable)
                    {
                        for (k = 0; k < length_kIndexRange; k++)
                        {
                            *loc_RE++ = k0 + tab_ptrPbch[12 * isymSlot + k];
                        }
                    }
                }
            }
            else if (cond2 && count != 1)
            {
                //synchronization channel
                // So no k_l ResourceElement assignment to PDSCH
            }
            else
            {
                if (physChanType == PDSCH)
                {
                    // PDSCH
                    // So, assign k_l ResourceElements for this symbol to PDSCH

                    k0 = nPRB * nRbSc; //frequency index for first resource element
                    // in the first resource block of this resource assignment ordered pair
                    length_kIndexRange = NumREinSubfrmSym_PDSCH[antenna_index][isym];

                    if (cond4 && (cond1 == 1 || cond2 == 1))
                    {
                        if (nPRB == RBIndexMid6Hi)
                        {
                            k0 += nRbSc >> 1;
                        }
                        length_kIndexRange = length_kIndexRange >> 1;
                    }

                    numRE = numRE + length_kIndexRange;
                    if(enable)
                    {
                        for (k = 0; k < length_kIndexRange; k++)
                        {
                            *loc_RE++ = k0 + tab_ptrPdsch[12 * 7 * antenna_index + 12 * isymSlot + k];
                        }
                    }
                }
            }
            nPRB = nPRB + 1; //increment resource block number
        } //for n loop
    } // for row loop

    *num_RE = numRE;
}
#endif
