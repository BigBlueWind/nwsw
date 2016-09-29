//-------------------------------------------------------------------------------------------
/** @file lteCommonControl.c
 *
 * @brief LTE CommonControl funtions,
 *        Functions to code/decode Control channel
 * @author Mindspeed Technologies
 * @version $Revision: 1.55 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------


#ifndef CEVA_INTRINSICS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "ext_phy.h"
#include "ltephydef.h"
#include "basic_op_cc.h"

#else
#include <basic_op.h>
#endif

#include "typedef.h"
#include "prototypes.h"

extern S16 NumREinSubfrmSym_PDSCH[3][14];

#ifdef CEVA_INTRINSICS
S16 CA_NumDLDataCarriersPerResourceBlock[4] =
{ 80, 76, 0, 72 };
#endif

void PdschMCSIndexProcedures(PDLTXINPUTPARAMS pTxInputParams, U32 i)
{
    S16 row;
    S16 nPRB, NumRBs;
    S16 n, nn;
    S16 antenna_index;
    S16 numDataSubcarriers = 0;
    S16 NumREinRBPairs_AllControl = 0;
    S16 numREinRBPairs_PdschMax;
    S16 *NumREinRBPairsMid6_Sync;
    S16 *NumREinRBPairsMid6_Bcst;
    S32 Mlayer_symb;
    S32 M0symb = 0, M1symb = 0;
    S32 G0 = 0, G1 = 0;
    S16 Qm0 = pTxInputParams->modType0[i] << 1;
    S16 Qm1 = pTxInputParams->modType1[i] << 1;
    S16 numLayers = pTxInputParams->numTxLayers[i];

    S16 nPRB_tilde;
    S16 *tabPtrVRB, *tabPtrVRB1, *tabPtrVRB2 = NULL;
    S16 isym, isymstart, isymstop, nDlSymb;
    S16 cond1, cond2, cond3, cond4, cond;
    S16 temp;

    if (pTxInputParams->Ntx_antennas == 1)
    {
        antenna_index = 0;
    }
    else if (pTxInputParams->Ntx_antennas == 2)
    {
        antenna_index = 1;
    }
    else
    {
        antenna_index = 2;
    }

    NumREinRBPairsMid6_Sync = pTxInputParams->CA_NumREinRBPairsMid6_Sync;

    NumREinRBPairsMid6_Bcst = pTxInputParams->CA_NumREinRBPairsMid6_Bcst;
    if(pTxInputParams->DistributedFlag[i] == 1)
        NumREinRBPairsMid6_Bcst = pTxInputParams->CA_NumREinRBPairsMid6_Bcst_all;


    if(pTxInputParams->DistributedFlag[i] == 0)
    {
        for (n = 0; n < pTxInputParams->numCtrlSymbols; n++)
        {
        // if control information is sent in this subframe, it is sent in the first
        // numPDCCHSymbols of all ResourceBlockPairs
            NumREinRBPairs_AllControl += NumREinSubfrmSym_PDSCH[antenna_index][n];
        }

        // substract this from the number of non-pilot RE's in a RBPair, to give the maximum number of PDSCH RE in any RB.
        // The actual number of PDSCH RE in any RB depends further on whether there are Broadcast or Sync channels in this RB.
        numREinRBPairs_PdschMax = 2 * CA_NumDLDataCarriersPerResourceBlock[pTxInputParams->Ntx_antennas - 1]
                                    - NumREinRBPairs_AllControl;

        if (pTxInputParams->subframeNumber == 0 || pTxInputParams->subframeNumber == 5)
        {
            for (row = 0; row < pTxInputParams->numberofEntries[i]; row++)
            {
                nPRB = pTxInputParams->reselmInfo[i][row].startRes; //resource block number
                NumRBs = pTxInputParams->reselmInfo[i][row].numRes; //number of consecutive resource blocks in this assignment

                for (n = 0; n < NumRBs; n++)
                {
                // Allow for Control, Broadcast and Synch Physical Channels before counting ResourceElements assigned to PDSCH
                    numDataSubcarriers += numREinRBPairs_PdschMax;
                    if ((nPRB + n >= pTxInputParams->RBIndexMid6Lo) && (nPRB + n <= pTxInputParams->RBIndexMid6Hi))
                    {
                        nn = nPRB + n - pTxInputParams->RBIndexMid6Lo;
                        // Subtract RE's to allow for Sync channels 2* accounts for both SSCH and PSCH
                        // Subtract RE's to allow for BCST channel
                        numDataSubcarriers -= 2 * NumREinRBPairsMid6_Sync[nn];

                        if (pTxInputParams->subframeNumber == 0)
                            numDataSubcarriers -= NumREinRBPairsMid6_Bcst[nn];
                    }
                }
            }
        }
        else
        {
            for (row = 0; row < pTxInputParams->numberofEntries[i]; row++)
            {
                NumRBs = pTxInputParams->reselmInfo[i][row].numRes; //number of consecutive resource blocks in this assignment
                // Allow for Control, Broadcast and Synch Physical Channels before counting ResourceElements assigned to PDSCH
                numDataSubcarriers += numREinRBPairs_PdschMax * NumRBs;
            }
        }
    }
    else // Distributed
    {
        tabPtrVRB1 = pTxInputParams->nVRB_Table;
        switch (pTxInputParams->Ndl_rb)
        {
            case 6:
                tabPtrVRB2 = &tabPtrVRB1[6]; // Slot1
            break;

            case 15:
                tabPtrVRB2 = &tabPtrVRB1[14];
            break;

            case 25:
                tabPtrVRB2 = &tabPtrVRB1[24];
            break;

            case 50:
                tabPtrVRB2 = &tabPtrVRB1[92];

                if(pTxInputParams->NgapFlag[i] == 1) // Ngap = 2
                {
                    tabPtrVRB2 = &tabPtrVRB1[92+46];
                    tabPtrVRB1 = &tabPtrVRB1[46];
                }
            break;

            case 75:
                tabPtrVRB2 = &tabPtrVRB1[128];

                if(pTxInputParams->NgapFlag[i] == 1) // Ngap = 2
                {
                    tabPtrVRB2 = &tabPtrVRB1[128+64];
                    tabPtrVRB1 = &tabPtrVRB1[64];
                }
            break;

            case 100:
                tabPtrVRB2 = &tabPtrVRB1[192];

                if(pTxInputParams->NgapFlag[i] == 1) // Ngap = 2
                {
                    tabPtrVRB2 = &tabPtrVRB1[192+96];
                    tabPtrVRB1 = &tabPtrVRB1[96];
                }
            break;
        }
        isymstart = pTxInputParams->numCtrlSymbols;
        isymstop = 2 * pTxInputParams->Ndl_symb;
        nDlSymb = pTxInputParams->Ndl_symb;
        cond3 = ((pTxInputParams->RBIndexMid6Hi - pTxInputParams->RBIndexMid6Lo) == 6);

        if (pTxInputParams->subframeNumber == 0)
        {
            for (isym = isymstart; isym < isymstop; isym++)
            {
                cond1 = (isym == 5) || (isym == 6) ;// SSS/PSS
                cond2 = (isym == 7) || (isym == 8) || (isym == 9) || (isym == 10);// BCST
                cond = cond1 || cond2;
                tabPtrVRB = tabPtrVRB1;
                if (isym >= nDlSymb)
                    tabPtrVRB = tabPtrVRB2;
                for (row = 0; row < pTxInputParams->numberofEntries[i]; row++)
                {
                    nPRB = pTxInputParams->reselmInfo[i][row].startRes; //resource block number
                    nPRB_tilde = nPRB;
                    nPRB = tabPtrVRB[nPRB];
                    NumRBs = pTxInputParams->reselmInfo[i][row].numRes; //number of consecutive resource blocks in this assignment
                    for (n = 0; n < NumRBs; n++)
                    {
                    // Allow for Control, Broadcast and Synch Physical Channels before counting ResourceElements assigned to PDSCH
                        numDataSubcarriers += NumREinSubfrmSym_PDSCH[antenna_index][isym]; //numREinRBPairs_PdschMax;
                        if ((nPRB  >= pTxInputParams->RBIndexMid6Lo) && (nPRB  <= pTxInputParams->RBIndexMid6Hi) && cond)
                        {
                            nn = nPRB - pTxInputParams->RBIndexMid6Lo;
                            cond4 = cond3 && ((nPRB == pTxInputParams->RBIndexMid6Lo) || (nPRB == pTxInputParams->RBIndexMid6Hi)); // 1st block and last block
                            temp = NumREinRBPairsMid6_Bcst[isym - nDlSymb];
                            if(cond4)
                                temp = temp >> 1; // only half is occupied

                            // Subtract RE's to allow for Sync channels
                            // Subtract RE's to allow for BCST channel
                            numDataSubcarriers -= NumREinRBPairsMid6_Sync[nn] * cond1 + temp * cond2;
                        }
                        nPRB_tilde += 1; //increment resource block number
                        nPRB = tabPtrVRB[nPRB_tilde];
                    }// n loop
                }// row loop
            }// isym
        }
        else if (pTxInputParams->subframeNumber == 5)
        {
            for (isym = isymstart; isym < isymstop; isym++)
            {
                cond = (isym == 5) || (isym == 6) ;// SSS/PSS
                tabPtrVRB = tabPtrVRB1;
                if (isym >= nDlSymb)
                    tabPtrVRB = tabPtrVRB2;
                for (row = 0; row < pTxInputParams->numberofEntries[i]; row++)
                {
                    nPRB = pTxInputParams->reselmInfo[i][row].startRes; //resource block number
                    nPRB_tilde = nPRB;
                    nPRB = tabPtrVRB[nPRB];
                    NumRBs = pTxInputParams->reselmInfo[i][row].numRes; //number of consecutive resource blocks in this assignment
                    for (n = 0; n < NumRBs; n++)
                    {
                    // Allow for Control, Broadcast and Synch Physical Channels before counting ResourceElements assigned to PDSCH
                        numDataSubcarriers += NumREinSubfrmSym_PDSCH[antenna_index][isym]; //numREinRBPairs_PdschMax;
                        if ((nPRB  >= pTxInputParams->RBIndexMid6Lo) && (nPRB  <= pTxInputParams->RBIndexMid6Hi) && cond)
                        {
                            nn = nPRB - pTxInputParams->RBIndexMid6Lo;
                            // Subtract RE's to allow for Sync channels
                            numDataSubcarriers -= NumREinRBPairsMid6_Sync[nn] * cond;
                        }
                        nPRB_tilde += 1; //increment resource block number
                        nPRB = tabPtrVRB[nPRB_tilde];
                    }// n loop
                }// row loop
            }// isym
        }
        else
        {
            for (isym = isymstart; isym < isymstop; isym++)
            {
                tabPtrVRB = tabPtrVRB1;
                if (isym >= nDlSymb)
                    tabPtrVRB = tabPtrVRB2;
                for (row = 0; row < pTxInputParams->numberofEntries[i]; row++)
                {
                    nPRB = pTxInputParams->reselmInfo[i][row].startRes; //resource block number
                    nPRB_tilde = nPRB;
                    nPRB = tabPtrVRB[nPRB];
                    NumRBs = pTxInputParams->reselmInfo[i][row].numRes; //number of consecutive resource blocks in this assignment
                    for (n = 0; n < NumRBs; n++)
                    {
                    // Allow for Control, Broadcast and Synch Physical Channels before counting ResourceElements assigned to PDSCH
                        numDataSubcarriers += NumREinSubfrmSym_PDSCH[antenna_index][isym]; //numREinRBPairs_PdschMax;
                        nPRB_tilde += 1; //increment resource block number
                        nPRB = tabPtrVRB[nPRB_tilde];
                    }// n loop
                }// row loop
            }// isym
        }
    } //Distributed

    numDataSubcarriers -= pTxInputParams->numRE_Prs; // Subtract Positioning Pilots

    //  (3) Adjust Mlayer_symb for the last time, based on numRB. Then calculate M0symb, M1symb, G0, G1 to match.
    //  number of data symbol values that will be written to resource blocks for this transportBlock
    Mlayer_symb = numDataSubcarriers;

    // Fix M0symb and M1symb to match Mlayer_symb
    if (pTxInputParams->numCodeWords[i] == 1)
    {
        M1symb = 0;
        M0symb = Mlayer_symb;
    }
    else if (pTxInputParams->numCodeWords[i] == 2)
    {
        if (numLayers == 2)
        {
            // spatial multiplexing, 2 layers
            M0symb = Mlayer_symb;
            M1symb = M0symb;
        }
        else if (numLayers == 3)
        {
            // spatial multiplexing, 3 layers
            M0symb = Mlayer_symb;
            M1symb = 2 * M0symb;
        }
        else
        {
            // numLayers == 4
            // spatial multiplexing, 4 layers
            M0symb = 2 * Mlayer_symb;
            M1symb = M0symb;
        }
    }

    G0 = M0symb * Qm0; // final value of "the available bits for the transmission of the transport block0"
    G1 = M1symb * Qm1; // final value of "the available bits for the transmission of the transport block1"

    pTxInputParams->G0[i] = G0;
    pTxInputParams->G1[i] = G1;

    return;
}

S16 ComputePRSAssignment(PDLTXINPUTPARAMS pTxInputParams)
{
    S16 vshift;
    S16 cond, cond1, cond2;
    S32 const1;

    S16 row_a, isym, Isym,  SlotNumber, startIndex;
    S16 cond11, cond22;
    S16 count_RE_PRS;

    S32 kIndex;
    S32 m;

    S16 Ncell_id    = pTxInputParams->Ncell_id;
    S16 Ndl_rb      = pTxInputParams->Ndl_rb;
    S16 Ndl_rbPrs   = pTxInputParams->Ndl_rbPrs;
    S16 Ndl_symb    = pTxInputParams->Ndl_symb;
    S16 Nrb_sc      = pTxInputParams->Nrb_sc;
    S16 slotNumber  = pTxInputParams->slotNumber;

    //init array size variables
    vshift = Ncell_id % 6;
    row_a = Ndl_rb * Nrb_sc;
    SlotNumber = slotNumber; //saving true value

    count_RE_PRS = 0;
    for(Isym = 0; Isym < 2*Ndl_symb; Isym++)
    {
        isym = Isym;
        if (Isym >= Ndl_symb)
        {
           isym = isym - Ndl_symb;
        }
        if (Isym == Ndl_symb)
        {
            slotNumber++;
        }

        cond1 = ( (isym == 3) || (isym == 5) || (isym == 6)) && ( (slotNumber & 0x1) == 0); //even
        cond2 = ( (isym == 1) || (isym == 2) || (isym == 3) || (isym == 5) || (isym == 6)) && ( (slotNumber & 0x1) == 1); //odd

        startIndex = -32768; // Indicates no PSS/SSS or PBCH
        cond11 = (Isym == 5 || Isym == 6) && (SlotNumber == 0 || SlotNumber == 10); // PSS/SSS
        cond22 = (Isym == 8 || Isym == 9 || Isym == 10) && (SlotNumber == 0); // PBCH
        if (cond11 || cond22)
           startIndex = ((row_a >> 1) - 36); // Where PSS/SSS or PBCH start

        if (cond1 || cond2)
        {


            //map to resource elements
            const1 = (6- isym + vshift) % 6;

            for (m = 0; m < 2 * Ndl_rbPrs; m++)
            {

                kIndex = (6 * (m + Ndl_rb - Ndl_rbPrs) + const1);
                //cond = ((kIndex/12 >= pTxInputParams->RBIndexMid6Lo) && (kIndex/12 <= pTxInputParams->RBIndexMid6Hi));
                //cond = ((m/2 >= pTxInputParams->RBIndexMid6Lo) && (m/2 <= pTxInputParams->RBIndexMid6Hi));
                //if( startIndex  < 0 || (cond == 0 && startIndex > 0) )

                cond = ((kIndex < startIndex) || ((kIndex > startIndex + 71) && startIndex > 0) );
                if( startIndex  < 0 || (cond == 1) )
                {
                  // startIndex++;
                   count_RE_PRS++;

                }
            }//m

        } // cond1 || cond2

    }// Isym

    return(count_RE_PRS);

}
