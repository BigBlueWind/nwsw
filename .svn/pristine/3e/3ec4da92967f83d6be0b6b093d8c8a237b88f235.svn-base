//-------------------------------------------------------------------------------------------
/** @file MacBsTx.c
 *
 * @brief This file implements the LTE mac base station transmitter functions
 *        that assign resource elements for Data and Control Information
 * @author Mindspeed Technologies
 * @version $Revision: 1.17 $
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
#include "ltemac.h"
//#include "heaps.h"

// Arrays to be used for any Phy instance for number of symbols used for
// reference signals, synchronization signals, broadcast channels,
// control, etc as a function of the number of antennas

//  This array gives the number of available re's per symbol for a subframe
//  for 1 Tx Antenna
U32 NumREinSubfrmSym_PDSCH_1Ant[14] =
{ 10, 12, 12, 12, 10, 12, 12, 10, 12, 12, 12, 10, 12, 12 };

//  This array gives the number of available re's per symbol for a subframe
//  for 2 Tx Antennas

U32 NumREinSubfrmSym_PDSCH_2Ant[14] =
{ 8, 12, 12, 12, 8, 12, 12, 8, 12, 12, 12, 8, 12, 12 };

//  This array gives the number of available re's per symbol for a subframe
//  for 4 Tx Antennas

U32 NumREinSubfrmSym_PDSCH_4Ant[14] =
{ 8, 8, 12, 12, 8, 12, 12, 8, 8, 12, 12, 8, 12, 12 };

//  This array gives the number of available re's per symbol for a subframe
//  for 1 Tx Antenna as a function of the number of control symbols
U32 NumREinRBPairsAll_Control_1Ant[5] =
{ 0, 10, 22, 34, 46 };

//  This array gives the number of available re's per symbol for a subframe
//  for 2 Tx Antennas as a function of the number of control symbols

U32 NumREinRBPairsAll_Control_2Ant[5] =
{ 0, 8, 20, 32, 44 };

//  This array gives the number of available re's per symbol for a subframe
//  for 4 Tx Antennas as a function of the number of control symbols

U32 NumREinRBPairsAll_Control_4Ant[5] =
{ 0, 8, 16, 28, 40 };

//  This array gives the number of available re's per symbol for a subframe
//  for those symbols that are used for the Broadcast channel indexed by
//  numberOfAntennaPorts-1

U32 NumREinRBPairsAll_Broadcast[4] =
{ 46, 44, 0, 40 };

//  This array gives the number of available re's per symbol for a subframe
//  for those symbols that are used for the Secondary Synchronization
//  signals indexed by numberOfAntennaPorts-1

U32 NumREinRBPairsMid6_SSync[4] =
{ 12, 12, 0, 12 };

//  This array gives the number of available re's per symbol for a subframe
//  for those symbols that are used for the Primary Synchronization
//  signals indexed by numberOfAntennaPorts-1

U32 NumREinRBPairsMid6_PSync[4] =
{ 12, 12, 0, 12 };

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL This function implements the LTE mac base station transmitter<BR>
 *         functions that assign resource elements for Data and Control<BR>
 *         Information<BR>
 *
 *  <1> Hardware Resource:
 *  Host
 *
 *  @param pSfr - INPUT, Pointer to TxVector for this SubFrame
 *  @param pMac - OUTPUT, Pointer to MAC Structure
 *
 *  @return none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------
 *  |Name    |Name in  |Direction|Length    |Type        |Data Format |Valid Range |Units |
 *  |        |Standard |         |          |            |            |            |      |
 *  ---------------------------------------------------------------------------------------
 *  |pSfr    |         |pInput   |1         |pStructure  |            |            |      |
 *  ---------------------------------------------------------------------------------------
 *  |pMac    |         |pOutput  |1         |pStructure  |            |            |      |
 *  ---------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |MacBsTx                            |
 *  ----------------------------------------------------------
 *  |Input Streams       |none                               |
 *  ----------------------------------------------------------
 *  |Output Streams      |none                               |
 *  ----------------------------------------------------------
 *  |Config Parameters   |pSfr                               |
 *  ----------------------------------------------------------
 *  |Status Parameters   |pMac                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  This function implements the LTE mac base station transmitter functions<BR>
 *  that assign resource elements for Data and Control Information and matches<BR>
 *  the MatLab implementation so a bit exact test can be executed to compare the<BR>
 *  "c" reference model results versus the Matlab versus the "device" results<BR>
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *  <6> References:
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void MacBsTx(PDLSUBFRDESC pSfr, PLTEMACDESC pMac)
{
    U32 numREinRBPairsAllControl = 0;
    U32 numREinRBPairsMid6_Broadcast = 0;
    U32 numREinRBPairsMid6_SSync = 0;
    U32 numREinRBPairsMid6_PSync = 0;
    U32* pTbl;

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
        numREinRBPairsMid6_Broadcast = NumREinRBPairsAll_Broadcast[pSfr->antennaPortcount - 1];
    }

    // (3) Determine number of resource elements reserved for Primary and Secondary
    // Synch Channel information, in ResourceBlockPairs where Synch info is sent

    if ((pSfr->subframeNumber == 0) || (pSfr->subframeNumber == 5))
    {
        numREinRBPairsMid6_SSync = NumREinRBPairsMid6_SSync[pSfr->antennaPortcount - 1];
        numREinRBPairsMid6_PSync = NumREinRBPairsMid6_PSync[pSfr->antennaPortcount - 1];
    }

    // (4) Update the LTEMACDESC info so all of the channels associated with this
    //  MAC-PHY interface can use the results of this computation

    pMac->numREinRBPairsAllControl = (U16) numREinRBPairsAllControl;
    pMac-> numREinRBPairsMid6_Broadcast = (U16) numREinRBPairsMid6_Broadcast;
    pMac->numREinRBPairsMid6_Ssync = (U16) numREinRBPairsMid6_SSync;
    pMac->numREinRBPairsMid6_Psync = (U16) numREinRBPairsMid6_PSync;

}
