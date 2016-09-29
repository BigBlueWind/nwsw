/** @file Fdxmac.c
 *
 * @brief Implemenataion of MAC from BE tests
 * @author Mindspeed Technologies
 * @version $Revision: 1.25 $
 *
 * COPYRIGHT(c) 2008-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "osal.h"
#include "math.h"
#include "basetypes.h"
#include "fdxmac.h"
#include "bsmac.h"
#include "ltemaccore.h"
#include "apidefs.h"
#include "ctrlmsg.h"
#include "msgroute.h"
#include "diags.h"

extern BS_CONTEXT* pBsMacDesc;

// As a function of cyclic prefix
static S16 CA_NumULDataCarriersPerResourceBlock[2] = {12*(7-1),  12*(6-1)};

LTEMACDESC FdxMac;

U8 NumDataRePerRB[4] =
{
  80,76,0,72
};

U8 NumDataRePerRBUL[2] =
{
  72, 60
};

U16 RBIndexMid6Lo[6]=
{
  0, 4, 9, 22, 34, 47
};

U16 RBIndexMid6Hi[6]=
{
  5, 10, 15, 27, 40, 52
};

// Arrays to be used for any Phy instance for number of symbols used for
// reference signals, synchronization signals, broadcast channels,
// control, etc as a function of the number of antennas

//  This array gives the number of available re's per symbol for a subframe
//  for 1 Tx Antenna
U32 NumREinSubfrmSym_PDSCH_1Ant [14] =
{
    10,12,12,12,10,12,12,10,12,12,12,10,12,12
};

//  This array gives the number of available re's per symbol for a subframe
//  for 2 Tx Antennas

U32 NumREinSubfrmSym_PDSCH_2Ant [14] =
{
    8,12,12,12,8,12,12,8,12,12,12,8,12,12
};

//  This array gives the number of available re's per symbol for a subframe
//  for 4 Tx Antennas

U32 NumREinSubfrmSym_PDSCH_4Ant [14] =
{
    8,8,12,12,8,12,12,8,8,12,12,8,12,12
};

//  This array gives the number of available re's per symbol for a subframe
//  for 1 Tx Antenna as a function of the number of control symbols
U32 NumREinRBPairsAll_Control_1Ant [5] =
{
   0, 10,22,34,46
};

//  This array gives the number of available re's per symbol for a subframe
//  for 2 Tx Antennas as a function of the number of control symbols

U32 NumREinRBPairsAll_Control_2Ant [5] =
{
    0,8,20,32,44
};

//  This array gives the number of available re's per symbol for a subframe
//  for 4 Tx Antennas as a function of the number of control symbols

U32 NumREinRBPairsAll_Control_4Ant [5] =
{
   0, 8,16,28,40
};

//  This array gives the number of available re's per symbol for a subframe
//  for those symbols that are used for the Broadcast channel indexed by
//  numberOfAntennaPorts-1

U32 NumREinRBPairsAll_Broadcast [4] =
{
    46,44,0,40
};

//  This array gives the number of available re's per symbol for a subframe
//  for those symbols that are used for the Secondary Synchronization
//  signals indexed by numberOfAntennaPorts-1

U32 NumREinRBPairsMid6_SSync [4] =
{
    12,12,0,12
};

//  This array gives the number of available re's per symbol for a subframe
//  for those symbols that are used for the Primary Synchronization
//  signals indexed by numberOfAntennaPorts-1

U32 NumREinRBPairsMid6_PSync [4] =
{
    12,12,0,12
};

// DFT size is N. The value N must be one of the value in following list,
// so = 2^a * 3^b * 5^c
U32 DFTSizeAllowed[35] =
    {12, 24, 36, 48, 60, 72, 96, 108, 120, 144, 180, 192, 216, 240, 288,
     300, 324, 360, 384, 432, 480, 540, 576, 600, 648, 720, 768, 864,
     900, 960, 972, 1080, 1152, 1200, 1296};

/** @brief LteFdxMacInit
 *         This function initializes the LTE Mac Layer
 *
 *  @param pMac Pointer to the LTEMACDESC structure that describes the
 *         MAC
 *
 *  @param pInit Pointer to the PINITPARM structure for the current
 *         PHY instance
 *
 *  This function initializes to default values the entries in the
 *  LTEMACDESC structure
 *
 */
void LteFdxMacInit(PLTEMACDESC pMac, PINITPARM pInit)
{
    pMac->nextAvailableCce = 0;
    pMac->nextAvailableRB = 0;
    pMac->numREinRBPairsAllControl = 0;
    pMac->numREinRBPairsMid6_Broadcast = 0;
    pMac->numREinRBPairsMid6_Ssync = 0;
    pMac->numREinRBPairsMid6_Psync = 0;
    pMac->pInit = pInit;
    pMac->rBIndexMid6Lo = RBIndexMid6Lo[pInit->channelBandwidth];
    pMac->rBIndexMid6Hi = RBIndexMid6Hi[pInit->channelBandwidth];

    memset((void *) &pMac->tCh[0], 0, MAXCHPERSF * sizeof(TRCHDESC));
}

/** @brief MacBsTx
 *         This function implements the LTE mac base station transmitter
 *         functions that assign resource elements for Data and Control
 *         Information
 *
 *  @param pSfr        Pointer to SUBFRDESC that contains the LTE BS PHY
 *                     subframe parameters for a given PHY instance under test
 *
 *  @param pMac        Pointer to the LTEMACDESC structure
 *
 *  This function implements the LTE mac base station transmitter functions
 *  that assign resource elements for Data and Control Information and matches
 *  the MatLab implementation so a bit exact test can be executed to compare the
 *  "c" reference model results versus the Matlab versus the "device" results
 */
void MacBsTx(PDLSUBFRDESC pSfr, PLTEMACDESC pMac)
{
    U32 numREinRBPairsAllControl = 0;
    U32 numREinRBPairsMid6_Broadcast = 0;
    U32 numREinRBPairsMid6_SSync = 0;
    U32 numREinRBPairsMid6_PSync = 0;
    U32 *pTbl;

    switch (pSfr->antennaPortcount)
    {
        case 1:
            pTbl = &NumREinRBPairsAll_Control_1Ant[0];
            break;
        case 2:
            pTbl = &NumREinRBPairsAll_Control_2Ant[0];
            break;
        case 4:
            pTbl = &NumREinRBPairsAll_Control_4Ant[0];
            break;
        default:
            // Not supported
            DiagPrint(DIAG_LEVEL3, "MAC", "Invalid antenna port count [%i]\n",
                      pSfr->antennaPortcount);
            return;
    }

    // (1) Determine number of resource elements reserved for Control information,
    // in ResourceBlockPairs where Control info is sent

    // Since this function for testing purposes can be called from the UL direction
    // now we check and don't use the numCtrlSymbols
    if ((pSfr->subframeType == DLTX) || (pSfr->subframeType == DLRX))
    {
        numREinRBPairsAllControl = *(pTbl + (pSfr->numCtrlSymbols));
    }
    else
    {
        numREinRBPairsAllControl = *(pTbl);
    }

    // (2) Determine number of resource elements reserved for Broadcast Channel
    // information, in ResourceBlockPairs where Broadcast info is sent
    if (pSfr->subframeNumber == 0)
    {
        numREinRBPairsMid6_Broadcast = NumREinRBPairsAll_Broadcast[pSfr->antennaPortcount-1];
    }

    // (3) Determine number of resource elements reserved for Primary and Secondary
    // Synch Channel information, in ResourceBlockPairs where Synch info is sent
    if ((pSfr->subframeNumber == 0) || (pSfr->subframeNumber == 5))
    {
        numREinRBPairsMid6_SSync = NumREinRBPairsMid6_SSync[pSfr->antennaPortcount-1];
        numREinRBPairsMid6_PSync = NumREinRBPairsMid6_PSync[pSfr->antennaPortcount-1];
    }

    // (4) Update the LTEMACDESC info so all of the channels associated with this
    //  MAC-PHY interface can use the results of this computation
    pMac->numREinRBPairsAllControl = (U16) numREinRBPairsAllControl;
    pMac->numREinRBPairsMid6_Broadcast = (U16) numREinRBPairsMid6_Broadcast;
    pMac->numREinRBPairsMid6_Ssync = (U16) numREinRBPairsMid6_SSync;
    pMac->numREinRBPairsMid6_Psync = (U16) numREinRBPairsMid6_PSync;
}

/** @brief MacBsReallocData
 *         This function allocates Scheduling Resource Blocks (Pairs of
 *         Resource Blocks)
 *
 *  @param pSfr Pointer to the SUBFRDESC structure to be used in the
 *         allocation
 *  @param pMac Pointer to the LTEMACDESC structure
 *  @param id   Id of the channel for which the allocation is being performed
 *  @param pData Pointer to the MAC PDU being conveyed in this channel
 *  @param dataSize Size in bytes of the data information
 *  @param Direction 0 DL, 1 UL
 *
 *  This function allocates SRB's to a channel based on the MAC PDU size
 *
 */
void MacBsReallocData(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id, U8 *pData,
                      U32 dataSize, U32 Direction)
{
    U32 bytes0 = 0, bytes1 = 0, numRB;
    U32 n,allowed = 0,nDft;
    U8 *mac_pdu0, *mac_pdu1;
    U8 numDataRePerRB;
    U32 numRBPairsAllowed;
    U16 numPDSCHSymbRequested,numPDSCHSymbAssigned,row_RBAssignment,baseRB;
    U16 numPUSCHSymbRequested,numPUSCHSymbAssigned;
    U16 temp, M1symb,M0symb,Qm,Mrb_symb,rndup,Mlayer_symb = 0;

    U16 NumREinRBPairsAll_SRS;
    PULSUBFRDESC pSfr1 = (PULSUBFRDESC)pSfr;

    mac_pdu0 = (U8*)malloc(dataSize);

    // Calculate Resource Element Frequency-Time assignments for the data for this
    // user in this subframe

    // Step (1) Split DL logical channel data into transport blocks at MAC-PHY
    // interface. Data input and output is in byte format
    if (Direction || (pSfr->dlCh[id - pSfr->numCtrlChannels].numCodeWords == 1))
    {
        //with 1 codeword, all txdata goes into first codeword,
        //2nd codeword=null
        bytes0= dataSize;
        bytes1 = 0;
        pMac->tCh[id].pFECTxIn[0]= (U8*)pData;
        pMac->tCh[id].pFECTxIn[1]= 0;
    }
    else if (pSfr->dlCh[id - pSfr->numCtrlChannels].numCodeWords == 2)
    {
        // In this case we need to allocate two buffers to hold the data
        mac_pdu1= (U8*)malloc(dataSize);
        pMac->tCh[id].pFECTxIn[0]= (U8*)mac_pdu0;
        pMac->tCh[id].pFECTxIn[1]= (U8*)mac_pdu1;

        if (pSfr->dlCh[id - pSfr->numCtrlChannels].nLayers == 3)
        {
            //with 3 layers, 2nd codeword length =2* first codeword length
            //=1st third of data bytes, but rounded to integer in case not
            //divisible by 3
            //ceil(length(mac_sdu)/3);
            //bytes0 = LteDivideS((U16)dataSize, 3, 1);
            bytes0 = (S16)ceil((double)dataSize/(double)3);

            //=remaining last two thirds (plus maybe 1 or 2 less than 2*bytes0)
            bytes1 = dataSize-bytes0;

            //~1st third of data bytes from logical channel input
            for (n = 0; n < bytes0; n++)
            {
                *(mac_pdu0+n) = *(pData+n);
            }
            //~last two thirds of data bytes from logical channel input
            for (n = 0; n < bytes1; n++)
            {
                *(mac_pdu1+n) = *(pData+n+bytes0);
            }
        }
        else
        {
            //bs_Ntx_layers == 2 or 4
            //with 2 or 4 layers, codewords are equal length
            //=1st half of data bytes, but rounded to integer in case not
            // divisible by 2
            //ceil(length(mac_sdu)/2);
            //bytes0 = LteDivideS((U16)dataSize, 2, 1);
            bytes0 = (S16)ceil((double)dataSize/(double)2);
            //=remaining last half (plus maybe 1 less than bytes0)
            bytes1 = dataSize-bytes0;

            //~1st half of data bytes from mac
            for (n = 0; n < bytes0; n++)
            {
                *(mac_pdu0+n) = *(pData+n);
            }

            //~2nd half of data bytes from mac
            for (n = 0; n < bytes1; n++)
            {
                *(mac_pdu1+n) = *(pData+n+bytes0);
            }
        }
    }

    // Step (2) Calculate minimum number of resource blocks needed to send
    // mac_pdu0 and mac_pdu1

    // get first estimates of G0 and G1, number of bits used to transmit tb0 and
    // tb1

    //8*length(mac_pdu0)
    pMac->tCh[id].fecInOutBits[0] = 8 * bytes0;
    //8*length(mac_pdu1)
    pMac->tCh[id].fecInOutBits[1] = 8 * bytes1;
    //x3 for turbo encoding expansion
    pMac->tCh[id].fecInOutBits[0] *= 3;
    //x3 for turbo encoding expansion
    pMac->tCh[id].fecInOutBits[1] *= 3;
    // NumSduBits = G0+G1
    // Msymb  = NumSduBits / Qm;  total number of mapped constellation points.
    // Since Qm bits are mapped to each constellation point
    if (Direction)
    {
        Qm = pSfr1->ulCh[id - pSfr->numCtrlChannels].ulSubChInfo.mcinfo.modulationType<<1;
    }
    else
    {
        Qm = pSfr->dlCh[id - pSfr->numCtrlChannels].subChInfo[0].modulationType<<1;
    }
    //ceil(G0 / Qm)
    //M0symb = LteDivideL((S32)pMac->tCh[id].fecInOutBits[0], Qm, 1);
    M0symb = (S16)ceil((double)pMac->tCh[id].fecInOutBits[0]/(double)Qm);
    // minm number of mapped constellation points from transportBlock0
    //ceil(G1 / Qm)
    M1symb= (S16)ceil((double)pMac->tCh[id].fecInOutBits[1]/(double)Qm);
    // minm number of mapped constellation points from transportBlock1

    // Find the minm possible value of Mlayer_symb, first pass
    if ( Direction ||
         (pSfr->dlCh[id - pSfr->numCtrlChannels].numCodeWords == 1))
    {
        //i.e. M1symb == 0
        if ( Direction ||
             (pSfr->dlCh[id - pSfr->numCtrlChannels].nLayers == 1))
        {
            //number of symbols in the single layer = number of symbols in
            //mac_pdu0 (mac_pdu1 does not exist)
            Mlayer_symb = M0symb;
        }
        else if (pSfr->dlCh[id - pSfr->numCtrlChannels].nLayers == 2)
        {
            //tx diversity MIMO, 2 layers
            rndup  = M0symb % 2;
            //round up to even number
            M0symb = M0symb + rndup;
            Mlayer_symb = M0symb/2;
        }
        else
        {
            //tx diversity MIMO, 4 layers
            rndup  = M0symb % 4;
            if (rndup > 0)
            {
                rndup = 4 - rndup;
            }
            M0symb = M0symb + rndup;
            //tx diversity MIMO, 4 layers
            Mlayer_symb = M0symb/4;
        }
    }
    else if (pSfr->dlCh[id - pSfr->numCtrlChannels].numCodeWords == 2)
    {
        if ( (pSfr->dlCh[id - pSfr->numCtrlChannels].nLayers == 2) ||
            (pSfr->dlCh[id - pSfr->numCtrlChannels].nLayers == 3) )
        {
            //spatial multiplexing, 2 or 3 layers
            //number of symbols in 2 or 3 layers = number of symbols in
            //mac_pdu0.
            //(M1symb does not matter at this stage)
            Mlayer_symb = M0symb;
        }
        else
        {
            //numLayers == 4
            //spatial multiplexing, 4 layers
            rndup  = M0symb % 2;
            //round up to even number
            M0symb = M0symb + rndup;

            //number of symbols in 4 layers = number of symbols in mac_pdu0.
            //(M1symb does not matter at this stage)
            Mlayer_symb = M0symb/2;
        }
    }
    //=minm number of data symbol values written to resource blocks for these
    //transport blocks
    Mrb_symb = Mlayer_symb;

    if (Direction)
    {
        numDataRePerRB =
         (U8)CA_NumULDataCarriersPerResourceBlock[pMac->pInit->cyclicPrefixType];

        pMac->pInit->numDataRePerPRB= numDataRePerRB;
    }
    else
    {
        numDataRePerRB = NumDataRePerRB[pSfr->antennaPortcount-1];
        pMac->pInit->numDataRePerPRB= numDataRePerRB;
    }
    //minimum number of resource blocks needed to send this data
    //ceil(Mrb_symb/numDataRePerRB)
    numRB = (S16)ceil((double)Mrb_symb/(double)numDataRePerRB);
    // add another RB for now, just to make sure don't run out of available
    // bits when add CRC(s).        ;
    numRB = numRB + 1;
    // Need to build CRC, turbo/cc coding tailbits expansion etc. into these
    // calculations...

    //force the number of RBs even, since allocated 2 at
    //a time in time domain, smallest resolution 1ms subframe
    numRB = numRB + numRB % 2;

    // Step (3) Now, allowing for Control, Broadcast and Synch information that
    // is also sent in this RBAssignment,
    // calculate actual number of resource blocks needed to send mac_pdu0 and
    // mac_pdu1
    // Also, Define resource blocks within the resource grid for this 1ms
    // subframe,used to send these even numbered transport blocks
    //!!!!!!! Note: Consider here 3 different RBAssignment formats;
    //!!!!!!! (1) baseRBIndex, NumConsecutiveRB
    //!!!!!!! (2) baseRBIndex
    //!!!!!!! (3) bitmap (as in Agilent ADS)
    //!!!!!!! Think now (2) probably best since;
    //!!!!!!! - Format (3) requires 4x32bit bitmap for every PDSCH, so could use
    //up to ~110 x 16bytes for 110 concurrent users max per subframe
    //!!!!!!! - Format (1) could use 2x more memory than (2) if use distributed
    //assignments everywhere.
    //!!!!!!! - Format (2) uses total 110 bytes max per subframe for 20MHz
    //channel with 110 RBs.

    //for thorough test pattern, use next 2 available RB's, then skip 1 RB

    //init pointer to first RB frequency index that is available

    baseRB = pMac->nextAvailableRB;
    n = 0;
    row_RBAssignment = 0; // Starting point to start filling RBAssignment

    if (Direction)
    {
        //if (srsSubframeEnable == 1)
          //  NumREinRBPairsAll_SRS = 12;  //12 resource elements are for SRS, when SRS is enabled in this subframe
        //else
            NumREinRBPairsAll_SRS = 0;  //0 resource elements are for SRS, when SRS is disabled in this subframe

        //init number of data symbols requested for sending this TransBlock
        numPUSCHSymbRequested = (U16)numRB*numDataRePerRB;

        //init number of data symbols in this RBAssignment
        numPUSCHSymbAssigned = 0;

        while (numPUSCHSymbAssigned < numPUSCHSymbRequested)
        {
           //See [TS.36.211 Sec.5.3.3], DFTSize must be representable by 2^a2 * 3^a3 * 5^a5
           //So, force numRB to be an allowed number
           allowed=0;
           while (allowed == 0)
           {
               for (nDft=0; nDft < 35; nDft++)
               {
                  numRBPairsAllowed = DFTSizeAllowed[nDft]/12;
                  if (numRB/2 == numRBPairsAllowed )
                  {
                      allowed = 1;
                  }
               }

               if (allowed == 0)
               {
                   numRB = numRB + 2;
               }
               else
               {
                   break;
               }
            }
            while (numRB >= n+4)
            {
               /* if (baseRB+1 > pMac->pInit->nMaxUlRb)
                {
                    printf("error in MacBsReallocData(): RBAssignment+1 is \
                            going beyond nMaxDlRb\n");
                   // exit(1);
                }*/
                //assign next two RB indices.
                //Ordered pair = base number of RB, number of consecutive RB's
                pMac->tCh[id].map.reselmInfo[row_RBAssignment].startRes=(U8)baseRB;
                pMac->tCh[id].map.reselmInfo[row_RBAssignment].numRes= 2;

                row_RBAssignment++;

                //Add PDSCH Symbols assigned in the first RBPair
                //add the total number of non-Reference ResourceElements
                numPUSCHSymbAssigned = numPUSCHSymbAssigned +
                                  2*NumDataRePerRBUL[pMac->pInit->cyclicPrefixType];
                numPUSCHSymbAssigned = numPUSCHSymbAssigned - NumREinRBPairsAll_SRS;

                //Add PDSCH Symbols assigned in the second RBPair
                //add the total number of non-Reference ResourceElements
                numPUSCHSymbAssigned = numPUSCHSymbAssigned +
                                  2*NumDataRePerRBUL[pMac->pInit->cyclicPrefixType];
                numPUSCHSymbAssigned = numPUSCHSymbAssigned - NumREinRBPairsAll_SRS;

                baseRB = baseRB + 2;
                //add 4 to RB counter after these 2 RBPair assignments
                n = n + 4;
                //since each RB assignment covers 2 RB's in 2 slots
            // inner while
            }

            //printf("N %hd %hd \n", n, baseRB);
            if (numRB == n+2)
            {
                /*if (baseRB > pMac->pInit->nMaxUlRb)
                {
                    printf(" error in ac_bs_tx(): RBAssignment is going \
                                       beyond Nul_rb");
                }*/

                //assign last RB index so last 2 RB's for 1 subframe
                pMac->tCh[id].map.reselmInfo[row_RBAssignment].startRes=(U8)baseRB;
                pMac->tCh[id].map.reselmInfo[row_RBAssignment].numRes = 1;
                row_RBAssignment++;

                //Add PUSCH Symbols assigned in this RBPair
                //add the total number of non-Reference ResourceElements
                numPUSCHSymbAssigned = numPUSCHSymbAssigned +
                                  2*NumDataRePerRBUL[pMac->pInit->cyclicPrefixType];
                numPUSCHSymbAssigned = numPUSCHSymbAssigned - NumREinRBPairsAll_SRS;

                //update pointer
                baseRB = baseRB + 1;
                //add 2 to RB counter after this RBPair assignment,
                //since each RB assignment covers 2 RB's in 2 slots
                n = n + 2;
            }

            if (numPUSCHSymbAssigned < numPUSCHSymbRequested)
            {
                 numRB = numRB + 2;
            }
            //printf("numPDSCHSymbAssigned %hd \n", numPDSCHSymbAssigned)
        // outer while
        }
        //number of data symbol values that will be written to resource blocks for
        //these transportBlocks!
        Mrb_symb = numPUSCHSymbAssigned;
    }
    else
    {
        //init number of data symbols requested for sending this TransBlock
        numPDSCHSymbRequested = (U16)numRB*numDataRePerRB;

        //init number of data symbols in this RBAssignment
        numPDSCHSymbAssigned = 0;

        while (numPDSCHSymbAssigned < numPDSCHSymbRequested)
        {
            while (numRB >= n+4)
            {
               /* if (baseRB+1 > pMac->pInit->nMaxDlRb-1)
                {
                    printf("error in MacBsReallocData(): RBAssignment+1 is \
                            going beyond nMaxDlRb\n");
                    //exit(1);
                }*/
                //assign next two RB indices.
                //Ordered pair = base number of RB, number of consecutive RB's
                pMac->tCh[id].map.reselmInfo[row_RBAssignment].startRes=(U8)baseRB;
                pMac->tCh[id].map.reselmInfo[row_RBAssignment].numRes= 2;

                row_RBAssignment++;

                //Add PDSCH Symbols assigned in the first RBPair
                //add the total number of non-Reference ResourceElements
                numPDSCHSymbAssigned = numPDSCHSymbAssigned +
                                     2*NumDataRePerRB[pSfr->antennaPortcount-1];

                //subtract the PDCCH RE allocations
                numPDSCHSymbAssigned = numPDSCHSymbAssigned -
                                       pMac->numREinRBPairsAllControl;

                if ((baseRB >= pMac->rBIndexMid6Lo) && (baseRB <=
                     pMac->rBIndexMid6Hi))
                {
                      //subtract PBCH and SCH RE allocations
                      numPDSCHSymbAssigned = numPDSCHSymbAssigned -
                                             pMac->numREinRBPairsMid6_Broadcast -
                                             pMac->numREinRBPairsMid6_Psync -
                                             pMac->numREinRBPairsMid6_Ssync;
                }

                //Add PDSCH Symbols assigned in the second RBPair
                //add the total number of non-Reference ResourceElements
                numPDSCHSymbAssigned = numPDSCHSymbAssigned +
                                      2*NumDataRePerRB[pSfr->antennaPortcount-1];
                //subtract the PDCCH RE allocations
                numPDSCHSymbAssigned = numPDSCHSymbAssigned -
                                       pMac->numREinRBPairsAllControl;

                if ((baseRB+1 >= pMac->rBIndexMid6Lo) && (baseRB+1 <=
                     pMac->rBIndexMid6Hi))
                {
                    //subtract PBCH and SCH RE allocations
                    numPDSCHSymbAssigned = numPDSCHSymbAssigned -
                                           pMac->numREinRBPairsMid6_Broadcast -
                                           pMac->numREinRBPairsMid6_Psync -
                                           pMac->numREinRBPairsMid6_Ssync;
                }

                baseRB = baseRB + 2;

                //add 4 to RB counter after these 2 RBPair assignments
                n = n + 4;
                //since each RB assignment covers 2 RB's in 2 slots
            // inner while
            }

            //printf("N %hd %hd \n", n, baseRB);
            if (numRB == n+2)
            {
               /* if (baseRB > pMac->pInit->nMaxDlRb)
                {
                    printf(" error in ac_bs_tx(): RBAssignment is going \
                                       beyond Ndl_rb");
                   // exit(1);
                }*/

                //assign last RB index so last 2 RB's for 1 subframe
                pMac->tCh[id].map.reselmInfo[row_RBAssignment].startRes=(U8)baseRB;
                pMac->tCh[id].map.reselmInfo[row_RBAssignment].numRes = 1;

                row_RBAssignment++;

                //Add PDSCH Symbols assigned in this RBPair
                //add the total number of non-Reference ResourceElements
                numPDSCHSymbAssigned = numPDSCHSymbAssigned +
                                    2*NumDataRePerRB[pSfr->antennaPortcount-1];

                //subtract the PDCCH RE allocations
                numPDSCHSymbAssigned = numPDSCHSymbAssigned -
                                       pMac->numREinRBPairsAllControl;

                if ( (baseRB >= pMac->rBIndexMid6Lo) &&
                                              (baseRB <= pMac->rBIndexMid6Hi))
                {
                    //subtract PBCH and SCH RE allocations
                    numPDSCHSymbAssigned = numPDSCHSymbAssigned -
                                           pMac->numREinRBPairsMid6_Broadcast-
                                           pMac->numREinRBPairsMid6_Psync-
                                           pMac->numREinRBPairsMid6_Ssync;
                }

                //update pointer
                baseRB = baseRB + 1;
                //add 2 to RB counter after this RBPair assignment,
                //since each RB assignment covers 2 RB's in 2 slots
                n = n + 2;
            }

            if (numPDSCHSymbAssigned < numPDSCHSymbRequested)
            {
                 numRB = numRB + 2;
            }
            //printf("numPDSCHSymbAssigned %hd \n", numPDSCHSymbAssigned)
        // outer while
        }
        //number of data symbol values that will be written to resource blocks for
        //these transportBlocks!
        Mrb_symb = numPDSCHSymbAssigned;

    }
    // Save baseRB to Mac Structure so next channel allocation starts
    // from there
    pMac->nextAvailableRB= baseRB;
    // Save numRbMaps
    pMac->tCh[id].map.numberofEntries= row_RBAssignment;
    // Step (4) Adjust Mlayer_symb for the last time, based on numRB. Then
    // calculate M0symb, M1symb, G0, G1 to match.
    // values that will be written to resource blocks for these transportBlocks!

    Mlayer_symb = Mrb_symb;

    //!!!!!! wrong!? if (Mlayer_symb*numAntennas)~=(numRB * numDataRePerRB)
    // floor(Mlayer_symb/numAntennas)
    temp = (S16)floor((double)Mlayer_symb/(double)(pSfr->antennaPortcount));
   /* if (Mlayer_symb/pSfr->antennaPortcount != temp)
    {
        printf(" error in mac_bs_tx(): Mlayer_symb invalid. \
                        Not divisible by the number of antennas - \
                        Not sure if this is necessary any more?!...");
    }*/

    //Fix M0symb and M1symb to match Mlayer_symb
    if (Direction ||
        (pSfr->dlCh[id - pSfr->numCtrlChannels].numCodeWords == 1))
    {
        //i.e. M1symb == 0
        if ( Direction || (pSfr->dlCh[id - pSfr->numCtrlChannels].nLayers == 1))
        {   //single antenna
            M0symb = Mlayer_symb;
        }
        else if (pSfr->dlCh[id - pSfr->numCtrlChannels].nLayers == 2)
        {
            //tx diversity MIMO, 2 layers
            M0symb = 2*Mlayer_symb;
        }
        else
        {
            //tx diversity MIMO, 4 layers
            M0symb = 4*Mlayer_symb;
        }
    }
    else if (pSfr->dlCh[id - pSfr->numCtrlChannels].numCodeWords == 2)
    {
        if (pSfr->dlCh[id - pSfr->numCtrlChannels].nLayers == 2)
        {
            //spatial multiplexing, 2 layers
            M0symb = Mlayer_symb;
            M1symb = M0symb;
        }
        else if (pSfr->dlCh[id - pSfr->numCtrlChannels].nLayers == 3)
        {
            //spatial multiplexing, 3 layers
            M0symb = Mlayer_symb;
            M1symb = 2*M0symb;
        }
        else
        {
            //numLayers == 4
            //spatial multiplexing, 4 layers
            M0symb = 2*Mlayer_symb;
            M1symb = M0symb;
        }
    }

    // final value of "the available bits for the
    // transmission of the transport block0"
    pMac->tCh[id].fecInOutBits[0] = M0symb * Qm;
    // final value of "the available bits for the
    // transmission of the transport block1"
    pMac->tCh[id].fecInOutBits[1] = M1symb * Qm;
    pMac->tCh[id].tbBytes[0] = bytes0;
    pMac->tCh[id].tbBytes[1] = bytes1;

    if (Direction)
    {
        // Save info to RxSubFrame
        pSfr1->ulCh[id - pSfr1->numberOfCtrlChannelDescriptors].ulSubChInfo.mapInfo= pMac->tCh[id].map;
    }
    else
    {
        for (n=0; n< pSfr->dlCh[id - pSfr->numCtrlChannels].numCodeWords; n++)
        {
            // Save info to TxSubFrame
            pSfr->dlCh[id - pSfr->numCtrlChannels].mapInfo= pMac->tCh[id].map;
        }
        memset(&pSfr->dlCh[id - pSfr->numCtrlChannels].mapInfo, 0, sizeof(MAPPINGINFO));
        pSfr->dlCh[id - pSfr->numCtrlChannels].mapInfo.numberofEntries = 1;
        pSfr->dlCh[id - pSfr->numCtrlChannels].mapInfo.reselmInfo[0].startRes = 0;
        pSfr->dlCh[id - pSfr->numCtrlChannels].mapInfo.reselmInfo[0].numRes = 50;
    }
}

/** @brief MacBsSendDataLowArm
 *         This function fills the channel parameters in the SUBFRDESC
 *         structure for a data channel identified by its id and then
 *         issues the TXSDU command to the PHY
 *
 *  @param pSfr Pointer to the SUBFRDESC structure that is used with
 *         this channel
 *
 *  @param pMac Pointer to the LTEMACDESC structure used
 *
 *  @param id   Id of the channel for which the parameters are being filled
 *
 *  @param pData Pointer to the MAC PDU being conveyed in this channel
 *
 *  @param dataSize Size in bytes of the control information
 *
 *  This function fills the SRB's allocation information for the channel
 *  specified by the id parameter in the SUBFRDESC used with this PHY and
 *  then issues the TXSDU command to the PHY
 *
 */
void MacBsSendDataLowArm(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id, U8 *pData,
                         U32 dataSize, U32 cwId, U32 maxBitsperCw, U8 chType)
{
    PMSGHEADER pHdr = (PMSGHEADER)pData;
    U32 i;
    U32 inSize;
    PTXSDUREQ pSduReq;
    U32 *input,*msg;

    if (pHdr == NULL)
    {
        return;
    }
    // First copy the Scheduling Resource Block allocations to the SUBFRDESC
    for (i=0; i< pSfr->dlCh[id - pSfr->numCtrlChannels].numCodeWords; i++)
    {
        pSfr->dlCh[id - pSfr->numCtrlChannels].mapInfo= pMac->tCh[id].map;
    }
    // Then issue the TXSDU commands to the PHY
    // Next Test the TxSdu Interface
    inSize= sizeof(TXSDUREQ)+ dataSize ;
    input = msg = MsgGetDataOffset(pHdr);
    pSduReq= (PTXSDUREQ)msg;
    pSduReq->phyEntityId = 0;
    pSduReq->msgType= PHY_TXSDU_REQ;
    pSduReq->chanId= id;
    pSduReq->cwId= cwId;
    pSduReq->maxBitsperCw= maxBitsperCw;
    pSduReq->msgLen= dataSize;

    pSduReq->pTxSdu = 0;
    pBsMacDesc->Phy.txCb(pBsMacDesc->Phy.toPhy, pHdr, inSize + sizeof(MSGHEADER));
}

/**
 *
 * @param pSfr
 * @param pMac
 * @param id
 * @param pData
 * @param dataSize
 * @param chType
 */
void MacBsSendCtlLowArm(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id, U8 *pData,
                        U32 dataSize, U8 chType)
{
    PMSGHEADER pHdr = MsgAllocBuffer(MSG_COMMON);
    U32 inSize;
    PTXSDUREQ pSduReq;
    U32 *input,*msg;

    if (pHdr == NULL)
    {
        return;
    }
    // Then issue the TXSDU commands to the PHY
    // Next Test the TxSdu Interface

    inSize = sizeof(TXSDUREQ) + dataSize;
    input = msg = MsgGetDataOffset(pHdr);
    pSduReq = (PTXSDUREQ)msg;
    pSduReq->channelType = chType;
    pSduReq->phyEntityId = 0;
    pSduReq->msgType = PHY_TXSDU_REQ;
    pSduReq->chanId = id;
    pSduReq->cwId = 0;
    pSduReq->maxBitsperCw = 8*dataSize;
    pSduReq->msgLen = dataSize;
    if (pSduReq->channelType == PHICH)
    {
        /*
        pSduReq->phichGrpNumber = pTestCfg->PhichGrp;
        pSduReq->lulRbIdx = pTestCfg->PhichSequence;
        pSduReq->uciFormat = 0; // For now single antenna mode for PHICH
        pSduReq->nackAck = pTestCfg->PhichBit; */
    }

    pSduReq->pTxSdu = 0;
    memcpy((U8*)((U8*)msg + sizeof(TXSDUREQ)), pData, dataSize);

    {
        U8 *ptr = ((U8*)msg) + sizeof(TXSDUREQ);
        unsigned int i;
        printf("Ctrl TxSdu Size %d\n", dataSize);
        for(i = 0; i < dataSize; i ++)
            printf("%d ", ptr[i]);
    }

    pBsMacDesc->Phy.txCb(pBsMacDesc->Phy.toPhy, pHdr, inSize + sizeof(MSGHEADER));
}

