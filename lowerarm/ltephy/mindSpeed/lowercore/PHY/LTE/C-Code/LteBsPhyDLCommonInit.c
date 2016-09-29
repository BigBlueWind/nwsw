//-------------------------------------------------------------------------------------------
/** @file LteBsPhyDLCommonInit.c
 *
 * @brief This file implements the initialization of the LTE BS PHY for common DL channels
 * based on cell ID, number of transmit antenna port, Master Information Block (MIB)
 * information etc.
 *
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.11 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
#include "basic_op_cc.h"
#ifndef WIN32
#include "mdmadrv.h"
#endif

//-----------------------------------------------------------------------------
/** @brief Function to generate Resource Element Map Table for any channel
 * allocated to symbol 0/1 (also control symbol 0).
 * For symbol 0 - Downlink Pilot symbols are transmitted.
 * This function takes into account this fact and generates the resource element
 * mapping table.
 *
 * Utility function.
 *
 *  <1> Hardware Resource:
 *  ARM/CEVA
 *
 *  @param   cellId         cellId (0-503)
 *  @param   cellId         numREGToMap (0-199)
 *  @param   pRegIndices    Pointer to REG Indices
 *  @param   pREMapTable    Pointer to generated Resource Element (RE)
 *                          Mapping Table
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------
 *  |Name          |Direction|Length  |Type  |Data Format |Valid Range |Units|
 *  |              |         |        |      |            |            |     |
 *  --------------------------------------------------------------------------
 *  |cellId        |input    |1       |U32   |(0:31:0)    |[0:503]     |     |
 *  --------------------------------------------------------------------------
 *  |numREGToMap   |input    |1       |U32   |(0:31:0)    |[0:199]     |     |
 *  --------------------------------------------------------------------------
 *  |pRegIndices   |input    |        |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  |pREMapTable   |pOutput  |1       |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |genREMapTableForCtrlSymbol  |
 *  -----------------------------------------------------
 *  |Input Stream        |None                          |
 *  -----------------------------------------------------
 *  |Output Stream       |pREMapTable                   |
 *  -----------------------------------------------------
 *  |Config Parameters   |cellID,numREGToMap,pIndices   |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *
 * Psuedo Logic:
 *  -- based on cell ID, identify the resource elements per REG which
 *     will be used for PILOT symbols.
 *  -- for ( indx = 0; indx < numREGtoMap; indx++ )
 *       for ( rIndx = 0; rIndx < 6; rIndx++ )
 *          if RE not allocated to DL PILOT
 *             reMapTable[cntr++] = REG[indx]*6+rIndx;
 *          end
 *       end
 *     end
 *
 *  <5> Cycle Count Formula:
 *  Run at initialization only.
 *
 *
 *  <6> References:
 *  36.211
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-----------------------------------------------------------------------------
static void genREMapTableForCtrlSymbol ( U32 cellId, S16 numREGToMap,
                                         S16 *pRegIndices,S16 *pREMapTable)
{

    /* Local Variables */
    S16 vShift, v[] = { 0 , 3 }; // nomenclature from 36.211
    S16 k_pilot_loc[2] = {0};
    S16 cntr=0;
    S32 regIndx, reIndx;

    /* vShift according to the standard - refer 36.211 (DL pilot)*/
    vShift = cellId % 6;

    /* Location for DL Pilot tone - resource mapping */
    k_pilot_loc[0] = ( v[0] + vShift ) % 6;
    k_pilot_loc[1] = (k_pilot_loc[0] + 3) % 6;

    /* re-init counter to zero */
    cntr = 0;

    /* mapping */
    for ( regIndx = 0; regIndx < numREGToMap; regIndx++ )
    {
        /* resource element mapping */
        for ( reIndx = 0; reIndx < ( N_RB_SC >> 1 ); reIndx++ )
        {
            if ( (reIndx != k_pilot_loc[0]) && (reIndx != k_pilot_loc[1]) )
            {
                /* set for PCFICH allocation */
                pREMapTable[cntr] = pRegIndices[regIndx]*(N_RB_SC>>1) +
                                    (S16)reIndx;
                cntr++;
            }
        }
    }

    return;
}

//-----------------------------------------------------------------------------
/** @brief Function to generate the Resource Element Groups (REG) allocated
 *  for PCFICH channel.
 *
 *  <1> Hardware Resource:
 *  ARM/CEVA
 *
 *  @param   cellId         cellId (0-503)
 *  @param   numDLRb        number of DL RB (0-99)
 *  @param   pCfiRegLoc     Pointer to REG for PCFICH
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------
 *  |Name          |Direction|Length  |Type  |Data Format |Valid Range |Units|
 *  |              |         |        |      |            |            |     |
 *  --------------------------------------------------------------------------
 *  |cellId        |input    |1       |U32   |(0:31:0)    |[0:503]     |     |
 *  --------------------------------------------------------------------------
 *  |numDLRb       |input    |1       |U32   |(0:31:0)    |[0:99]      |     |
 *  --------------------------------------------------------------------------
 *  |pCfiRegLoc    |pOutput  |1       |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |computePcfichRegAllocation    |
 *  -----------------------------------------------------
 *  |Input Stream        |None                          |
 *  -----------------------------------------------------
 *  |Output Stream       |pCfiRegLoc                    |
 *  -----------------------------------------------------
 *  |Config Parameters   |cellID,numDLRb                |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Based on cell ID and DL_RB - compute Resource Element Group (REG)
 *  allocated to PCFICH.
 *
 *  <5> Cycle Count Formula:
 *  Run at initialization only.
 *
 *
 *  <6> References:
 *  36.211
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-----------------------------------------------------------------------------
static void computePcfichRegAllocation(U32 cellId,U32 numDLRb,S16 *pCfiRegLoc)
{
    S32 k_bar, regIndx;
    S16 nDLRB_nRBSC;
    S16 cfiReLoc[NUM_REG_ALLOC_FOR_CFI];
    S16 tempWord16;

    /* Compute REG group locations for PCFICH channel */

    /* Location of PCFICH REG and resource element allocation */
    k_bar = ( N_RB_SC >> 1 ) * ( cellId % ( 2 * numDLRb ) );

    /* for modulo addition */
    nDLRB_nRBSC = (S16)numDLRb * N_RB_SC;

    /* PCFICH REG Locations */
    for ( regIndx = 0; regIndx < NUM_REG_ALLOC_FOR_CFI; regIndx++ )
    {
        /* set REG location */
        tempWord16 = (S16)(k_bar + ((regIndx* numDLRb)>>(1))
                                    * (N_RB_SC>>1));
        /* modulo arithmetic */
        cfiReLoc[regIndx] = tempWord16 % nDLRB_nRBSC;
    }

    /* Convert the resource element location to resource element
     * group location and also find the minimum REG location */
    for ( regIndx = 0; regIndx < NUM_REG_ALLOC_FOR_CFI; regIndx++ )
    {
        /* from resource element location to resource element group */
        pCfiRegLoc[regIndx] = cfiReLoc[regIndx] / ( N_RB_SC>>1 );
    }

    return;
}


//-----------------------------------------------------------------------------
/** @brief Initialize the Physical Common Format Indicator Channel (PCFICH) for
 *  all the combinations of control symbols and for all sub-frames of the frame
 *  number.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param   pCFIMemBuf     Pointer to the memory where CFI symbol data will be
 *                          saved
 *                          (size of the buffer = 160x3*Ntx_AntPorts IQ symbols
 *                          (160 x 3 x Ntx_AntPorts bits) )
 *  @param   dlBandwidth    Downlink bandwith (value 0=6 RB, value 1=15 RB etc.)
 *  @param   cellId         cellId (0-503)
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------
 *  |Name          |Direction|Length  |Type  |Data Format |Valid Range |Units|
 *  |              |         |        |      |            |            |     |
 *  --------------------------------------------------------------------------
 *  |pCFIMemPtr    |pInOut   |        |U32   |            |            |     |
 *  --------------------------------------------------------------------------
 *  |dlBandwidth   |input    |1       |U32   |(0:31:0)    |[0:5]       |     |
 *  --------------------------------------------------------------------------
 *  |cellId        |input    |1       |U32   |(0:31:0)    |[0:503]     |     |
 *  --------------------------------------------------------------------------
 *  |NtxAnt        |input    |1       |U32   |(0:31:0)    |[0:4]       |     |
 *  --------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |LteBsPhyInitCFIData           |
 *  -----------------------------------------------------
 *  |Input Stream        |None                          |
 *  -----------------------------------------------------
 *  |Output Stream       |pCFIMemBuf                    |
 *  -----------------------------------------------------
 *  |Config Parameters   |dlBW,cellID,NtxAnt            |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 * The CFI data can be pre-generated since the CFI data is only dependent on
 * cell ID and the number of TX antenna ports. The generated data is for
 * the entire frame i.e. all the 10 sub-frames.
 * The table will contain  bits 16 x 3 controlSym x NTxAnt x 10 sub-frames.
 * The table is arranged as follows:
 *  | NumCtrlSym=1,subframe 0 || NumCtrlSym=1,subframe 1 |... till SF=10
 *  | NumCtrlSym=2,subframe 0 || NumCtrlSym=1,subframe 1 |... till SF=10
 *  | NumCtrlSym=3,subframe 0 || NumCtrlSym=1,subframe 1 |... till SF=10
 *
 *  So for example to obtain PCFICH symbol buffer for case
 *   - num control symbols = 2 and sub-frame sf (sf=0...9)
 *  curr_cfi_symbols = pCFIBuffer + (10+sf)*(NUMBER_OF_CFI_BITS*numTxAntPort)
 * and for case: num control symbols = 1, sub-frame (sf=0..9)
 *  curr_cfi_symbols = pCFIBuffer + (0+sf)*(NUMBER_OF_CFI_BITS*numTxAntPort)
 * similarly you can access table for number of control symbols = 3 as
 *  curr_cfi_symbols = pCFIBuffer + (20+sf)*(NUMBER_OF_CFI_BITS*numTxAntPort)
 * Psuedo Logic:
 *
 *
 *  <5> Cycle Count Formula:
 *  Run at initialization only.
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-----------------------------------------------------------------------------
static void LteBsPhyInitCFIData(S16 *pCFIMemBuf, U32 numDLRb, U32 cellId,
                                U32 numBsTxAntPort, S16 *pCfiREMapTable)
{
    /* Local variables */
    S32 cfiTableOffset = 1;
    S32 ctrlSymIndx, subFrmIndx;
    S32 CFIBitSequence;
    S32 numCtrlSymStart = 1, scramIndx, bitIndx;
    U32 cInit = 0, tempWord32;
    U8  scramSeq[NUMBER_OF_CFI_BITS], *pScramSeq;
    S32 numBytes = ( NUMBER_OF_CFI_BITS >> 3 );
    U8  scramSeqByte[ ( NUMBER_OF_CFI_BITS >> 3 ) ];
    S16 modSymBuffer[ ( NUMBER_OF_CFI_BITS ) ];
    S16 cfiRegLoc[4];

    /* set the offset into CFI table based on the number of DL RB */
    if ( numDLRb <= 10 )
    {
        cfiTableOffset = 2;
        numCtrlSymStart = 2;
    }

    /*
     * Loop through all the combinations of control symbols and also
     * all the sub-frames
     * Step 1: Pick the 32-bits from the table for a given number
     *         of control symbols
     * Step 2: Generate the scrambling sequence per sub-frame
     * Step 3: Modulate the bits (QPSK)
     * Step 4: Do Layer Mapping and Precoding (based on numTxAntPorts)
     * Loop through steps 2 to 4 for all sub-frames
     */
    for ( ctrlSymIndx = 0; ctrlSymIndx < MAX_DL_CONTROL_SYM; ctrlSymIndx++ )
    {
        /* Get the control information bits */
        CFIBitSequence = CFI_DW[numCtrlSymStart - cfiTableOffset + ctrlSymIndx];

        for ( subFrmIndx = 0; subFrmIndx < NUMSUBFRPERFRAME; subFrmIndx++ )
        {
            /* initialize value for scrambling sequence */
            cInit = ( subFrmIndx + 1 )*( 2*cellId + 1)*( 1 << 9 ) + cellId;
            /* Generate the scrambling sequence */
            PseudoRandomSeqGen (NUMBER_OF_CFI_BITS, cInit, scramSeq);
            /* The scrambling sequence is in bytes - convert into a bit seq */
            pScramSeq = scramSeq;
            /* create sequence */
            for (scramIndx = 0; scramIndx < numBytes; scramIndx++)
            {
                scramSeqByte[scramIndx] = 0; // initialize to zero

                for (bitIndx = 0; bitIndx < NUM_BITS_PER_BYTE; bitIndx++)
                {
                    scramSeqByte[scramIndx] |= (1 & *pScramSeq++) <<
                                               (NUM_BITS_PER_BYTE - 1 - bitIndx);
                }
            }

            /* scrambling sequence - bit packed */
            tempWord32 = *(U32 *)(&scramSeqByte[0]);
            tempWord32 = ( tempWord32 ^ CFIBitSequence );

            /* modulate the bits to symbols */
            tx_Mapper((U8 *)(&tempWord32), (NUMBER_OF_CFI_BITS>>1), 0, MOD_QPSK,
                      modSymBuffer);

            if (numBsTxAntPort == 1) // No layer Mapping needed for STC
            {
                memcpy((S8*) pCFIMemBuf, (S8*) modSymBuffer,
                        NUMBER_OF_CFI_BITS * 2);
            }
            else
            {
                /* Transmit Diversity */
                STC((S16 *)&modSymBuffer, (NUMBER_OF_CFI_BITS>>1),
                    (S16)numBsTxAntPort, (S16*)pCFIMemBuf);
            }

            /* increment the buffer */
            pCFIMemBuf = pCFIMemBuf + ( NUMBER_OF_CFI_BITS * numBsTxAntPort );
        } // subFrmIndx
    } // control symbols

    /* compute REG locations for PCFICH channel */
    computePcfichRegAllocation(cellId, numDLRb, cfiRegLoc);

    /* generate the resource mapping table based on the REG locations */
    genREMapTableForCtrlSymbol(cellId, NUM_REG_ALLOC_FOR_CFI,
                               cfiRegLoc, pCfiREMapTable );

    return;
}

//-----------------------------------------------------------------------------
/**
* Utility function to compute the number of PHICH group for a given number
* of PHICH Resource allocation based on number of DL RB
**/
//-----------------------------------------------------------------------------
static void computeNumPhichGrp ( U32 numDLRb, S16 phichResource,
                                S16 *numPHICHGrp)
{
    S32 rbTableIndex = 0;

    /* Get number of PHICH groups from table */
    switch ( numDLRb )
    {
        case 6:
            rbTableIndex = 0;
            break;
        case 15:
            rbTableIndex = 1;
            break;
        case 25:
            rbTableIndex = 2;
            break;
        case 50:
            rbTableIndex = 3;
            break;
        case 75:
            rbTableIndex = 4;
            break;
        case 100:
            rbTableIndex = 5;
            break;
        default:
            // throw an error - empty for time being
            break;
    }

    /* get number of PHICH groups for obtaining the PHICH resource
     * element mapping
     * phichResource - 0=1/6, 1=1/2, 2=1, 3=2 */
    *numPHICHGrp = (S16)(TabN_PHICH_GRP[6*phichResource + rbTableIndex]);

    return;
}

//-----------------------------------------------------------------------------
/** @brief Function to generate the Resource Element Groups (REG) allocated
 *  for PHICH channel.
 *
 *  NOTES: Currently only supports PHICH duration = NORMAL.
 *         Code to support EXTENDED mode is present but not tested.
 *
 *  <1> Hardware Resource:
 *  ARM/CEVA
 *
 *  @param   cellId         cellId (0-503)
 *  @param   numDLRb        number of DL RB (0-99)
 *  @param   numBsTxAntPort number of BS Transmit Antenna Port (1,2 or 4 )
 *  @param   numPHICHGroup  number of PHICH groups
 *  @param   phichDuration  PHICH Duration (NORMAL or EXTENDED)
 *  @param   pPhichRegLoc   Pointer to REG for PHICH
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------
 *  |Name          |Direction|Length  |Type  |Data Format |Valid Range |Units|
 *  |              |         |        |      |            |            |     |
 *  --------------------------------------------------------------------------
 *  |cellId        |input    |1       |U32   |(0:31:0)    |[0:503]     |     |
 *  --------------------------------------------------------------------------
 *  |numDLRb       |input    |1       |U32   |(0:31:0)    |[0:99]      |     |
 *  --------------------------------------------------------------------------
 *  |numBsTxAntPort|input    |1       |U32   |(0:31:0)    |[1,2 or 4]  |     |
 *  --------------------------------------------------------------------------
 *  |numPHICHGroup |input    |1       |U32   |(0:31:0)    |[0:2]       |     |
 *  --------------------------------------------------------------------------
 *  |phichDuration |input    |1       |U32   |(0:31:0)    |NORMAL/EXT  |     |
 *  --------------------------------------------------------------------------
 *  |pPhichRegLoc  |pOutput  |1       |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |computePhichRegAllocation     |
 *  -----------------------------------------------------
 *  |Input Stream        |None                          |
 *  -----------------------------------------------------
 *  |Output Stream       |pPhichRegLoc                  |
 *  -----------------------------------------------------
 *  |Config Parameters   |cellID,numDLRb                |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Based on cell ID and DL_RB - compute Resource Element Group (REG)
 *  allocated to PHICH.
 *
 *  <5> Cycle Count Formula:
 *  Run at initialization only.
 *
 *
 *  <6> References:
 *  36.211
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-----------------------------------------------------------------------------
static void computePhichRegAllocation ( U32 cellId, U32 numDLRb,
                                        U32 numBsTxAntPort, S16 numPHICHGroup,
                                        S16 phichDuration, S16 *pPhichRegLoc )
{

    /* Local variables */
    S32 regIndx;
    S16 cfiRegLoc[NUM_REG_ALLOC_FOR_CFI];
    /* variable refers to standard nomenclature (36.211, section 6.9.3) */
    S16 nli_bar[MAX_NUM_SYM_ALLOC_FOR_PHICH] = {0};
    S16 tempPHICHRegLoc[2*MAX_NUM_RB];
    S16 *pPhichRegLoc_Sym0 = &tempPHICHRegLoc[0];
    S32 indx;
    S16 tempVal;
    S32 cntr = 0;
    /* variable refers to standard nomenclature (36.211, section 6.9.3) */
    S16 *ni_bar = pPhichRegLoc;

    /* compute PCFICH REG allocation based on nDLRB and cell ID */
    computePcfichRegAllocation ( cellId, numDLRb, cfiRegLoc );

    /* compute REG allocation for PHICH for symbol 0 */
    /* From 36.211 - symbol 0 - 2 REG per RB */
    nli_bar[0] = (S16)(2*numDLRb) - NUM_REG_ALLOC_FOR_CFI;

    /* From 36.211: For symbol 1 - num REG = 3 per PRB for 2 antenna port
     * and num REG = 2 for 4 antenna port
     * For symbol 2 - always 3 REG per PRB */
    nli_bar[1] = (S16)(3*numDLRb);
    nli_bar[2] = nli_bar[1];

    /* Based on base-station transmit antenna port - REG allocation */
    if ( numBsTxAntPort == 4 )
    {
        nli_bar[1] = (S16)(2*numDLRb);
    }

    cntr = 0;

    /* for symbols with no PCFICH - no need to compute free REG
     * since no PCFICH allocation is present.
     * Compute PHICH REG locations for symbol 0 */
    for ( regIndx = 0; regIndx < (2*(S32)numDLRb); regIndx++ )
    {
        /* set offset */
        if ( (regIndx != cfiRegLoc[0] ) &&
             (regIndx != cfiRegLoc[1] ) &&
             (regIndx != cfiRegLoc[2] ) &&
             (regIndx != cfiRegLoc[3] ) )
        {
            /* set the REG index */
            pPhichRegLoc_Sym0[cntr++] = (S16)(regIndx);
        }
    }

    cntr = 0; // initialize

    /* compute REG based on whether PHICH duration is NORMAL or EXTENDED */
    if ( phichDuration == 0 ) // should be enum but 0--> NORMAL
    {
        /* For Normal PHICH duration - only one symbol is allocated
         * for PHICH. Loop through all the number of PHICH groups
         * and allocate the REG locations */
        for ( indx = 0; indx < numPHICHGroup; indx++)
        {
            /* Implementing formula for normal Duration in 36.211
               section 6.9.3 */
            /* for i = 0 */
            tempVal = ( cellId + indx ) % nli_bar[0];
            /* set the REG index */
            ni_bar[cntr++] = pPhichRegLoc_Sym0[tempVal];
            /* for i = 1*/
            tempVal = ( cellId + indx + ( nli_bar[0] / 3 ) ) % nli_bar[0];
            /* set the REG index */
            ni_bar[cntr++] = pPhichRegLoc_Sym0[tempVal];
            /* for i = 2*/
            tempVal = ( cellId + indx + ((2*nli_bar[0]) / 3 ) )
                      % nli_bar[0];
            /* set the REG index */
            ni_bar[cntr++] = pPhichRegLoc_Sym0[tempVal];
        }
    }
    else
    {
        /* Throw an error - we don't support extended PHICH as yet -
         * since the resource mapping is different.
         * NOTE: If we need to support extended PHICH duration we
         * need to modify PDCCH allocations too.
         */

        /* For Extended PHICH duration - three symbols are allocated
         * for PHICH. Loop through all the number of PHICH groups
         * and allocate the REG locations */
        for ( indx = 0; indx < numPHICHGroup; indx++)
        {
            /* Implementing formula for normal Duration in 36.211
               section 6.9.3 */
            /* for i = 0 */
            tempVal = ( cellId + indx ) % nli_bar[0];
            /* set the REG index */
            ni_bar[cntr++] = pPhichRegLoc_Sym0[tempVal];
            /* for i = 1*/
            tempVal = ( (S16)( cellId * nli_bar[1] ) / (S16)nli_bar[0] );
            tempVal = ( tempVal + indx + ( nli_bar[1] / 3 ) ) % nli_bar[1];
            /* set the REG index */
            ni_bar[cntr++] = pPhichRegLoc_Sym0[tempVal];
            /* for i = 2*/
            tempVal = ( (S16)( cellId * nli_bar[2] ) / (S16)nli_bar[0] );
            tempVal = ( tempVal + indx + ((2*nli_bar[2]) / 3 ) )
                      % nli_bar[2];
            /* set the REG index */
            ni_bar[cntr++] = pPhichRegLoc_Sym0[tempVal];
        }
    } // end phich duration

    return;
}

//-----------------------------------------------------------------------------
/** @brief Initialize the Physical Common HARQ Indicator Channel (PHICH) for
 *  all the sub-frames and for all possible sequence indices. It also computes
 *  the REG as well as resource element indices and returns the resource
 *  element indices for the number of PHICH groups configured.
 *
 *  NOTES:
 *  (1) The symbol data returned is assuming HI bit = 1. Negate the entries
 *      in table when HI bit = 0.
 *  (2) The caller of the function is tasked with allocating correct memory
 *      No memory is allocated in this function.
 *  (3) No error checking is done.
 *  (4) Currently supports NORMAL CP, FDD mode - supports only NORMAL PHICH
 *      Duration. Hooks for extended PHICH duration are present but have
 *      not been tested.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param   pHIMemBuf      Pointer to the memory where HI symbol data will be
 *                          saved.
 *                          (Size of the buffer = 12 symbols * 8 seqIndices *
 *                                                10 sub-frames * 4 bytes *
 *                                                numBsTxAntPorts )
 *  @param   numDLRb        Number of DL RB
 *  @param   cellId         cellId (0-503)
 *  @param   numBsTxAntPort Number of base station Tx Ant Port (1,2 and 4)
 *  @param   phichResource  0 = 1/6, 1=1/2 etc (refer to table in 36.211)
 *  @param   phichDuration  0 = NORMAL, 1=EXTENDED
 *  @param   pHiRELoc       pointer to the memory where PHICH Resource Element
 *                          mapping is saved. Size: 12*50 (50=Max)
 *  @param   numPHICHGrp    pointer to number of PHICH group
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------
 *  |Name          |Direction|Length  |Type  |Data Format |Valid Range |Units|
 *  |              |         |        |      |            |            |     |
 *  --------------------------------------------------------------------------
 *  |pHIMemBuf     |pInOut   |        |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  |numDlRb       |input    |1       |U32   |(0:31:0)    |[0:100]     |     |
 *  --------------------------------------------------------------------------
 *  |cellId        |input    |1       |U32   |(0:31:0)    |[0:503]     |     |
 *  --------------------------------------------------------------------------
 *  |numBsTxAntPort|input    |1       |U32   |(0:31:0)    |[0:4]       |     |
 *  --------------------------------------------------------------------------
 *  |phichResource |input    |1       |U32   |(0:31:0)    |[0:4]       |     |
 *  --------------------------------------------------------------------------
 *  |phichDuration |input    |1       |U32   |(0:31:0)    |[0:1]       |     |
 *  --------------------------------------------------------------------------
 *  |pHiReLoc      |pInOut   |1       |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  |pNumPHICHGrp  |pInOut   |1       |U32   |            |            |     |
 *  --------------------------------------------------------------------------

 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |LteBsPhyInitHIData            |
 *  -----------------------------------------------------
 *  |Input Stream        |None                          |
 *  -----------------------------------------------------
 *  |Output Stream       |pHIMemBuf, pHiRELoc,pPHICHGrp |
 *  -----------------------------------------------------
 *  |Config Parameters   |numDLRb,cellID,NtxAntPort etc |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 * As the cell ID does not change constantly the PHICH data for all sequence
 * Indices can be pre-computed and saved for all the sub-frames. Similarly
 * the Ng (PHICH-Resource) and PHICH Duration does not change that often
 * so the REG allocated for PHICH can be pre-computed and passed during the
 * run-time.
 *
 *
 * Psuedo Logic:
 * Run function would be as follows:
 * (a) Point to the correct location of buffer based on the sub-frame
 * (b) For each PHICH channel - point to the correct sequence index
 *     and if the PHICH bit is 1 - use the sequence as is
 *     if PHICH bit = 0 --> negate the sequence from the table.
 * (c) Sum all the channels for a given group and appropriate
 *     encode the streams.
 * (d) Concatenate all the streams (from all PHICH groups) and
 *     resource map using the resource mapping table computed
 *     in this function
 *
 *  <5> Cycle Count Formula:
 *  Run at initialization only.
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-----------------------------------------------------------------------------
static void LteBsPhyInitHIData(S16 *pHIMemBuf, S16 numDLRb, S16 cellId,
                                S16 numBsTxAntPort, S16 phichResource,
                                S16 phichDuration, S16 *pHiRELoc,
                                S16 *numPHICHGrp)
{
    /* Local variables */
    S16 locNumPHICHGrp;
    S32 nRegPerGroup;
    /* variable refers to standard nomenclature (36.211, section 6.9.3) */
    S16 ni_bar[MAX_PHICH*2], tempVal;

    /* compute number of groups in PHICH */
    computeNumPhichGrp ( numDLRb, phichResource, numPHICHGrp );

    /* set the output value */
    locNumPHICHGrp = *numPHICHGrp;

    /* Compute PHICH REG allocation */
    computePhichRegAllocation ( (U32)cellId, (U32)numDLRb, (U32)numBsTxAntPort,
                                (S16)locNumPHICHGrp, phichDuration, ni_bar );

    nRegPerGroup = NUM_REG_ALLOC_FOR_PHICH_GROUP;
    tempVal      = (S16)(locNumPHICHGrp*nRegPerGroup);

    /* Generate the Resource Mapping Table for PHICH when the duration
     * is Normal Duration */
    genREMapTableForCtrlSymbol (cellId, tempVal, ni_bar, pHiRELoc);


    /*-------------------------------------------------------------------------
     * Compute all sequence Indices symbols output per sub-frame
     * Step 1: For each sub-frame generate the scrambling code
     * Step 2: Use the sequence index table and compute symbols for all
     *         sub-frames
     *-------------------------------------------------------------------------
     */
    {
        /* local variables */
        S16 temp16;
        S16 cvecPhich_neg[NUMBER_OF_HI_BITS_NORMAL_CP];
        S32 subFrmIndx, symIndx;
        S16 numPhichBits = NUMBER_OF_HI_BITS_NORMAL_CP;
        U32 cInit;
        U8  scramSeq[NUMBER_OF_HI_BITS_NORMAL_CP];
        S32 numPhichSeqIndx = NUM_PHICH_SEQ_INDX_NORMAL_CP;
        S16 tempVal16;
        S16 modValue = 0x3; // for normal CP
        S32 scramIndx, seqIndx;
        S16 locSymBuf[NUMBER_OF_HI_BITS_NORMAL_CP*2];

        /* set the symbol buffer to zero */
        memset ( pHIMemBuf, 0, sizeof ( S16 ) * NUMSUBFRPERFRAME *
                 2 * numPhichBits * numPhichSeqIndx *
                 numBsTxAntPort );

        /* Loop through all the sub-frame indices */
        for ( subFrmIndx = 0; subFrmIndx < NUMSUBFRPERFRAME; subFrmIndx++ )
        {
            /* initialize value for scrambling sequence */
            cInit = ( subFrmIndx + 1 )*( 2*cellId + 1)*( 1 << 9 ) + cellId;

            /* Generate the scrambling sequence */
            PseudoRandomSeqGen (numPhichBits, cInit, scramSeq);

            /* create sequence */
            for (scramIndx = 0; scramIndx < numPhichBits; scramIndx++)
            {   /* scrambling index = 0,1 so convert to 1,-1*/
                temp16 = ( 1 - 2 * scramSeq[scramIndx] );
                temp16 = temp16 * CE_ScaleConstel_BPSK;
                cvecPhich_neg[scramIndx] = -temp16;
            }

            /* for all the sequence indices - generate the sequence */
            for ( seqIndx = 0; seqIndx < numPhichSeqIndx; seqIndx++)
            {
                for ( symIndx = 0; symIndx < numPhichBits; symIndx++ )
                {
                    /* multiply with the orthogonal sequence */
                    tempVal16 = mult ( cvecPhich_neg[symIndx],
                                       W_PHICH[seqIndx][symIndx&modValue] );

                    /* set the output */
                    locSymBuf[(symIndx<<1)+1] = tempVal16;

                    /* for seqIndices > 4 (2 for extended CP) negate */
                    if ( seqIndx >= ( numPhichSeqIndx>>1) )
                    {
                        tempVal16 = negate(tempVal16);
                    }

                    /* set the output */
                    locSymBuf[(symIndx<<1)] = tempVal16;
                }

                /* Do layer mapping, precoding according to transmit antenna ports */
                if ( numBsTxAntPort == 1 )
                {
                    /* just do a mem copy */
                    memcpy ( pHIMemBuf, locSymBuf, sizeof(S16)*numPhichBits*2 );
                }
                else if ( numBsTxAntPort == 2 )
                {
                    /* transmit diversity */
                    STC ( locSymBuf, numPhichBits, 2, pHIMemBuf );
                }
                else
                {
                    /* do only one combination - stc type 1*/
                    STC_type1 ( locSymBuf, numPhichBits, pHIMemBuf );
                }

                /* increment the output buffer to point to next sequence index */
                pHIMemBuf = pHIMemBuf + ( numPhichBits*2*numBsTxAntPort );
            } // seqIndx
        } // subFrmIndx
    } // end of sequence generation

    return;
}

//-----------------------------------------------------------------------------
/** @brief Compute the permuted table for PDCCH interleaver. This function
 *  implements the sub-block de-interleaver specified in 36.212 (and
 *  referred to in 36.211). NOTE: The permutation tables are for
 *  PDCCH Quadruplets.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param   numQuadruplets     Number of PDCCH Quadruplets
 *  @param   cellId             cellId (0-503)
 *  @param   pPermuteTable      pointer to the PDCCH permute table
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------
 *  |Name          |Direction|Length  |Type  |Data Format |Valid Range |Units|
 *  |              |         |        |      |            |            |     |
 *  --------------------------------------------------------------------------
 *  |numQuadruplets|input    |1       |S16   |(0:16:0)    |[0:300]     |     |
 *  --------------------------------------------------------------------------
 *  |cellId        |input    |1       |U32   |(0:31:0)    |[0:503]     |     |
 *  --------------------------------------------------------------------------
 *  |pPermuteTable |pOutput  |1       |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |computePdcchPermuteTable      |
 *  -----------------------------------------------------
 *  |Input Stream        |None                          |
 *  -----------------------------------------------------
 *  |Output Stream       |pPermuteTable                 |
 *  -----------------------------------------------------
 *  |Config Parameters   |numQuadruplets, cellID        |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *
 * Psuedo Logic:
 *
 *
 *  <5> Cycle Count Formula:
 *  Run at initialization only.
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-----------------------------------------------------------------------------
static void computePdcchPermuteTable ( S16 numQuadruplets, S16 cellId,
                                       S16 *pPermuteTable )
{
    /* local variables */
    S16     D = numQuadruplets; // refer to 36.212 sub-block interleaver
    S16     numRows, numCols;
    S16     ND = 0; // refer to standard 36.212
    S16     temp, startRow, cntr = 0;
    S16     tableIndex[MAX_PDCCH_BYTES];
    S32     colIndx, rowIndx, cellShift;

    /* Compute number of rows for the interleaver */
    numCols = NUM_COL_IN_PDCCH_INTERLEAVER;

    /* compute number of rows D <= (numRows x numCols)*/
    temp = ( D & (NUM_COL_IN_PDCCH_INTERLEAVER - 1 ) );

    /* get the number of rows */
    numRows = D >> ( NUM_SHIFT_PDCCH_INTERLEAVER );

    /* update the number of rows if temp is non-negative */
    if ( temp > 0 )
    {
        /* increment the number of rows */
        numRows = numRows + 1;
    }

    /* get the number of NULL locations */
    ND = (numRows << NUM_SHIFT_PDCCH_INTERLEAVER ) - D;

    /* generate the permute table */
    for ( colIndx = 0; colIndx < numCols; colIndx++ )
    {
        startRow = 0;

        if ( CA_RMCTC_InterColPermConv[colIndx] < ND )
        {
            startRow = 1;
        }

        /* loop through the rows */
        for ( rowIndx = startRow; rowIndx < numRows; rowIndx++ )
        {
            /* set the permute table */
            tableIndex[cntr++] = CA_RMCTC_InterColPermConv[colIndx] +
                                 (numCols*(S16)rowIndx) - ND;
        }
    }

    /* take care of the shift associated with cellID */
    cellShift = cellId % D;
    memcpy(pPermuteTable, &tableIndex[cellShift], (D - cellShift) * sizeof(S16));
    memcpy(&pPermuteTable[D - cellShift], &tableIndex[0], cellShift * sizeof(S16));

    return;
}

//-----------------------------------------------------------------------------
/** @brief Initialize some parameters of Physical Common PDCCH Channel (PDCCH)
 *  for all the sub-frames. The parameters that are computed at initalization
 *  are
 *  -- Number of PDCCH quadruplets for all the different control symbols
 *     (1, 2 and 3 )
 *  -- PDCCH REG allocation per symbol for all the control symbol combinations
 *     (1, 2 and 3 )
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param   numDLRb            Number of DL RB
 *  @param   cellId             cellId (0-503)
 *  @param   numBsTxAntPort     Number of base station Tx Ant Port (1,2 and 4)
 *  @param   phichResource      0 = 1/6, 1=1/2 etc (refer to table in 36.211)
 *  @param   phichDuration      0 = NORMAL, 1=EXTENDED
 *  @param   pPDCCHPermuteTable pointer to PDCCH permutation table
 *  @param   pPDCCHREGLen       pointer to PDCCH Resource Element Group Table
 *  @param   pPdcchRETable      pointer to PDCCH Resource Mapping Table
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------
 *  |Name          |Direction|Length  |Type  |Data Format |Valid Range |Units|
 *  |              |         |        |      |            |            |     |
 *  --------------------------------------------------------------------------
 *  |numDlRb       |input    |1       |U32   |(0:31:0)    |[0:100]     |     |
 *  --------------------------------------------------------------------------
 *  |cellId        |input    |1       |U32   |(0:31:0)    |[0:503]     |     |
 *  --------------------------------------------------------------------------
 *  |numBsTxAntPort|input    |1       |U32   |(0:31:0)    |[0:4]       |     |
 *  --------------------------------------------------------------------------
 *  |phichResource |input    |1       |U32   |(0:31:0)    |[0:4]       |     |
 *  --------------------------------------------------------------------------
 *  |phichDuration |input    |1       |U32   |(0:31:0)    |[0:1]       |     |
 *  --------------------------------------------------------------------------
 *  |pPDCCHPermuteTable|pOut |1      |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  |pPDCCHREGLen  |pOut     |1       |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  |pPDCCHRETable |pOut     |1       |S16   |            |            |     |
 *  --------------------------------------------------------------------------

 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |LteBsPhyInitPDCCHData         |
 *  -----------------------------------------------------
 *  |Input Stream        |None                          |
 *  -----------------------------------------------------
 *  |Output Stream       |pPDCCHPermuteTable,pPDCCHRegLen|
 *  |                    |pPDCCHRETable                 |
 *  -----------------------------------------------------
 *  |Config Parameters   |numDLRb,cellID,NtxAntPort etc |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 * As the cell ID does not change constantly the PDCCH data related to
 * number of control symbols can be pre-computed and used at run-time.
 * The resource mapping tables and the permutation tables are pre-computed
 * and saved.
 *
 * NOTE: Only NORMAL PHICH Duration is supported.
 *
 * Psuedo Logic:
 *
 *
 *  <5> Cycle Count Formula:
 *  Run at initialization only.
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-----------------------------------------------------------------------------
static void LteBsPhyInitPDCCHData(S16 numDLRb, S16 cellId, S16 numBsTxAntPort,
                                  S16 phichResource, S16 phichDuration,
                                  S16 *pPDCCHPermuteTable, S16 *pPDCCHREGLen,
                                  S16 *pPdcchRETable)
{
    /* Local Variables */
    S16     numPHICHGrp, regIndx, regCntr = 0;
    S16     numPDCCHQuadruplets[MAX_DL_CONTROL_SYM];
    S16     pcfichRegLoc[NUM_REG_ALLOC_FOR_CFI];
    S16     phichRegLoc[NUM_REG_ALLOC_FOR_PHICH_GROUP*(MAX_PHICH>>1)];
    S16     regTableSym0[NUM_REG_FOR_SYMBOL_1_TWO_ANT_PORT*MAX_NUM_RB];
    S16     pdcchREGSym0[NUM_REG_FOR_SYMBOL_1_TWO_ANT_PORT*MAX_NUM_RB];
    S32     symIndx;
    S16     permuteTable[MAX_PDCCH_BYTES];
    S16     *pPermutePtrSym0, *pPermutePtrSym1, *pPermutePtrSym2;
    S16     cntrSym0, cntrSym1, cntrSym2;
    S16     temp16;
    S16     *pPdcchRETableSym0, *pPdcchRETableSym1, *pPdcchRETableSym2;

    /* For PDCCH Resource Element mapping
     * Step 0 : Compute the total number of PDCCH quadruplets based on PHICH
                REG and PCFICH REG + number of control symbols and transmit
                antenna port
     * Step 1 : For each control symbol - compute the permuted indices based
     *          the permutation table (36.212 + 36.211 ) and also based on
     *          cell ID
     * Step 2 : For each symbol ( 0, (0,1), (0,1,2) ) combinations associate
     *          REGs per symbol to quadruplet number in PDCCH composite
     *          symbol
    */

    /* Get the number of PHICH groups */
    computeNumPhichGrp(numDLRb, phichResource, &numPHICHGrp);

    /* Compute number of PDCCH quadruplets for all different control symbols */
    /* For control symbol 1 */
    numPDCCHQuadruplets[0] = NUM_REG_FOR_SYMBOL_0*numDLRb -
                             NUM_REG_ALLOC_FOR_CFI -
                             NUM_REG_ALLOC_FOR_PHICH_GROUP*numPHICHGrp ;

    /* If number of control symbols = 2.
     * NOTE: We are assuming NORMAL PHICH Duration */
    numPDCCHQuadruplets[1] = numPDCCHQuadruplets[0] +
                             NUM_REG_FOR_SYMBOL_1_TWO_ANT_PORT * numDLRb;

    /* For greater than 2 tx antenna port */
    if ( numBsTxAntPort > 2 )
    {
        numPDCCHQuadruplets[1] = numPDCCHQuadruplets[0] +
                                 NUM_REG_FOR_SYMBOL_1_FOUR_ANT_PORT * numDLRb;
    }

    /* If number of control symbols == 3 */
    numPDCCHQuadruplets[2] = numPDCCHQuadruplets[1] +
                             NUM_REG_FOR_SYMBOL_2 * numDLRb;


    /* set the REG per symbol */
    pPDCCHREGLen[0] = numPDCCHQuadruplets[0];
    pPDCCHREGLen[1] = numPDCCHQuadruplets[1];
    pPDCCHREGLen[2] = numPDCCHQuadruplets[2];

    /* initialize REG table for symbol 0 to zero */
    memset(regTableSym0, 0, sizeof ( S16 )*NUM_REG_FOR_SYMBOL_1_TWO_ANT_PORT*MAX_NUM_RB);

    /* Identify the REG allocated for PCFICH and PHICH - symbol 0 */
    computePcfichRegAllocation(cellId, numDLRb, pcfichRegLoc);

    /* set locations for PCFICH to be 1 */
    for ( regIndx = 0; regIndx < NUM_REG_ALLOC_FOR_CFI; regIndx++ )
    {
        regTableSym0[pcfichRegLoc[regIndx]] = 1;
    }

    computePhichRegAllocation((U32)cellId, (U32)numDLRb, (U32)numBsTxAntPort,
                              numPHICHGrp, phichDuration, &phichRegLoc[0]);

    /* set locations for PCFICH to be 1 */
    for ( regIndx = 0; regIndx < (numPHICHGrp*NUM_REG_ALLOC_FOR_PHICH_GROUP);
          regIndx++ )
    {
        regTableSym0[phichRegLoc[regIndx]] = 1;
    }

    /* initialize counter */
    regCntr = 0;

    /* REG allocated for PDCCH for symbol 0 */
    for ( regIndx = 0; regIndx < ( numDLRb * NUM_REG_FOR_SYMBOL_0); regIndx++ )
    {
        /* allocate to PDCCH REG not allocated for PHICH or PCFICH */
        if ( regTableSym0[regIndx] == 0 )
        {
            pdcchREGSym0[regCntr++] = regIndx;
        }
    }

    /* pointer to symbol 0 - resource element mapping */
    pPdcchRETableSym0 = pPdcchRETable;

    /* resource mapping table for symbol 0 */
    genREMapTableForCtrlSymbol(cellId, numPDCCHQuadruplets[0], pdcchREGSym0,
                               pPdcchRETableSym0 );

    /* pointer to symbol 0 - resource element mapping */
    pPdcchRETableSym1 = pPdcchRETable + (MAX_NUM_RB * N_RB_SC);

    /* if number of transmit antenna port = 0 -- all REG available */
    if ( numBsTxAntPort <= 2 )
    {
        for ( regIndx = 0; regIndx < ( numDLRb * N_RB_SC); regIndx++ )
        {
            pPdcchRETableSym1[regIndx] = regIndx;
        }
    }
    else
    {
        /* number of REG */
        temp16 = NUM_REG_FOR_SYMBOL_1_FOUR_ANT_PORT*numDLRb;

        for ( regIndx = 0; regIndx < temp16; regIndx++ )
        {
            pdcchREGSym0[regIndx] = regIndx;
        }

        /* resource mapping table for symbol 0 */
        genREMapTableForCtrlSymbol ( cellId, temp16, pdcchREGSym0,
                                     pPdcchRETableSym1 );
    }

    /* RE mapping for symbol 2 */
    /* pointer to symbol 2 - resource element mapping */
    pPdcchRETableSym2 = pPdcchRETable + 2*(MAX_NUM_RB * N_RB_SC);
    for ( regIndx = 0; regIndx < ( numDLRb * N_RB_SC); regIndx++ )
    {
        pPdcchRETableSym2[regIndx] = regIndx;
    }


    /* Based on the number of control symbols :
     * - compute the interleaving indices
     * - for each symbol - available REG and the associated PDCCH quadruplet
     */
    for ( symIndx = 0; symIndx < MAX_DL_CONTROL_SYM; symIndx++ )
    {
        /* compute the permuted index table */
        computePdcchPermuteTable (numPDCCHQuadruplets[symIndx], cellId,
                                  permuteTable);

        /* construct permute table for all combination of control symbols */
        if ( symIndx == 0 )
        {
            /* if symbolIndx = 0 --> only 1 control symbol so permuteTable
               does not change , do mem copy */
            memcpy ( pPDCCHPermuteTable, permuteTable,
                     sizeof (S16)*numPDCCHQuadruplets[symIndx] );
        }
        else if ( symIndx == 1 )
        {
            /* For 2 control symbols - the mapping is dependent on
               number of antenna ports. If number of antenna ports
               is 2 ==> 3 REG on symbol 1 so mapping will be
               lowest symbol 0 REG index (if available), followed by
               lowest REG for symbol 1, lowest REG for symbol 1 and
               followed by symbol 0==> [0,1,1,0,1] and if
               number of antenna port is 4 - then mapping is
               [0 1 0 1] */

            S16 *pAssignment, numREGPerMap;

            /* set pointer to permute table correctly */
            pPermutePtrSym0 = pPDCCHPermuteTable + numPDCCHQuadruplets[0];
            pPermutePtrSym1 = pPDCCHPermuteTable + 2*numPDCCHQuadruplets[0];

            /* get the assignment */
            if ( numBsTxAntPort <= 2 )
            {
                pAssignment = &assignment_1_2_antennas[1];
                numREGPerMap = assignment_1_2_antennas[0];
            }
            else
            {
                pAssignment = &assignment_4_antennas[1];
                numREGPerMap = assignment_4_antennas[0];
            }

            /* initialize counters */
            cntrSym0 = 0;
            cntrSym1 = 0;
            regCntr  = 0;
            temp16   = 0;
            regIndx = 0;

            /* permute table mapping */
            do
            {
                /* get symbol information */
                if ( pAssignment[temp16] == 0 )
                {
                    /* if REG is available add it */
                    if ( regTableSym0[cntrSym0++] == 0 )
                    {
                        /* add and increment */
                        pPermutePtrSym0[regCntr++] = permuteTable[regIndx++];
                    }
                }
                else
                {
                    /* update symbol 1 - no check for PCFICH or PHICH */
                    pPermutePtrSym1[cntrSym1++] = permuteTable[regIndx++];
                }

                /* modulo addition */
                temp16 = temp16+1;

                if ( temp16 == ( numREGPerMap ) )
                {
                    temp16 = 0;
                }
            } while ( regIndx < numPDCCHQuadruplets[1] );

        }
        else if ( symIndx == 2 )
        {

            /* For 3 control symbols - the mapping is dependent on
             *  number of antenna ports. If number of antenna ports
             *  is 2 ==> 3 REG on symbol 1 so mapping will be
             *  lowest symbol 0 REG index (if available), followed by
             *  lowest REG for symbol 1, lowest REG for symbol 1 and
             *  followed by symbol 0==> 0,1,2,1,2,0,1,2 */
            S16 *pAssignment, numREGPerMap;

            /* set pointer to permute table correctly */
            temp16 = numPDCCHQuadruplets[0] + numPDCCHQuadruplets[1];
            pPermutePtrSym0 = pPDCCHPermuteTable + temp16;
            pPermutePtrSym1 = pPermutePtrSym0 + numPDCCHQuadruplets[0];
            pPermutePtrSym2 = pPermutePtrSym0 + numPDCCHQuadruplets[1];

            /* get the assignment */
            if ( numBsTxAntPort <= 2 )
            {
                pAssignment = &assignment_1_2_antennas[7];
                numREGPerMap = assignment_1_2_antennas[6];
            }
            else
            {
                pAssignment = &assignment_4_antennas[6];
                numREGPerMap = assignment_4_antennas[5];
            }

            /* initialize counters */
            cntrSym0 = 0;
            cntrSym1 = 0;
            cntrSym2 = 0;
            regCntr  = 0;
            temp16   = 0;
            regIndx = 0;

            /* permute table mapping */
            do
            {
                /* get symbol information */
                if ( pAssignment[temp16] == 0 )
                {
                    /* if REG is available add it */
                    if ( regTableSym0[cntrSym0++] == 0 )
                    {
                        /* add and increment */
                        pPermutePtrSym0[regCntr++] = permuteTable[regIndx++];
                    }
                }
                else if ( pAssignment[temp16] == 1 )
                {
                    /* update symbol 1 - no check for PCFICH or PHICH */
                    pPermutePtrSym1[cntrSym1++] = permuteTable[regIndx++];
                }
                else
                {
                    /* update symbol 1 - no check for PCFICH or PHICH */
                    pPermutePtrSym2[cntrSym2++] = permuteTable[regIndx++];
                }

                /* modulo addition */
                temp16 = temp16+1;

                if ( temp16 == ( numREGPerMap ) )
                {
                    temp16 = 0;
                }
            } while ( regIndx < numPDCCHQuadruplets[2] );
        }
        else
        {
            /* Throw an error */
        }
    }

    return;
}


//-----------------------------------------------------------------------------
/** @brief Generate the DL pilot sequence for all the sub-frames and all
 *  the associated symbols in a given sub-frame.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param   numDLRb            Number of DL RB
 *  @param   cellId             cellId (0-503)
 *  @param   cpType             (NORMAL=0, EXTENDED=1) (use enum??)
 *  @param   numBsTxAntPort     Number of base station Tx Ant Port (1,2 and 4)
 *  @param   pDLCellSpecificRefSeq  pointer to cell specific reference sequence
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------
 *  |Name          |Direction|Length  |Type  |Data Format |Valid Range |Units|
 *  |              |         |        |      |            |            |     |
 *  --------------------------------------------------------------------------
 *  |numDlRb       |input    |1       |S16   |(0:15:0)    |[0:100]     |     |
 *  --------------------------------------------------------------------------
 *  |cellId        |input    |1       |S16   |(0:15:0)    |[0:503]     |     |
 *  --------------------------------------------------------------------------
 *  |numBsTxAntPort|input    |1       |S16   |(0:15:0)    |[0:4]       |     |
 *  --------------------------------------------------------------------------
 *  |cpType        |input    |1       |S16   |(0:15:0)    |[0:1]       |     |
 *  --------------------------------------------------------------------------
 *  |pDLCellSpecificRefSeq|pOut |1      |S16   |            |            |   |
 *  --------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |LteBsPhyInitDLCellSpecificRefSeq|
 *  -----------------------------------------------------
 *  |Input Stream        |None                          |
 *  -----------------------------------------------------
 *  |Output Stream       |pDLCellSpecificRefSeq,        |
 *  -----------------------------------------------------
 *  |Config Parameters   |numDLRb,cellID,NtxAntPort etc |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 * As the cell ID does not change constantly the DL cell specific
 * sequence for all the symbols in a sub-frame and all the sub-frames
 * can be pre-computed and saved in a table.
 *
 * Psuedo Logic:
 *
 *
 *  <5> Cycle Count Formula:
 *  Run at initialization only.
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-----------------------------------------------------------------------------
static void LteBsPhyInitDLCellSpecificRefSeq(S16 numDLRb, S16 cellId,
                                             S16 cpType, S16 numBsTxAntPort,
                                             S16 *pDLCellSpecificRefSeq)
{

    /* Local Variables */
    S16 symIndx;
    S32 cinit, slotIndex, m, numSym;
    S16 NDL_SYMB = 7;
    S16 symNum[MAX_NUM_DL_CELL_SPECIFIC_SYM_IN_SLOT];  // defined as 'l'
    S32 const3, const4, const5;
    U8  scrambleSeq[8*MAX_LTE_DL_RB_STD_DEFINED], *pScrambleSeq = scrambleSeq;
    S16 mdash, seq1, seq2;
    S16 OneOverSqrt2[2] = {11585, -11585}; // should use #define (FIX)

    /* If the CP type is extended rather than NORMAL CP */
    if ( cpType == EXTENDED )
    {
        NDL_SYMB = 6; // assuming 15 KHz seperation (need another input?)
    }

    /* compute constants for computing cinit */
    const3 = CE_NmaxDL_rb - numDLRb;
    const4 = (1 << 10) * (2 * cellId + 1);
    const5 = 1 - cpType + 2 * cellId; // NOTE: NORMAL Enum type is defined = 0

    /* symbols in slot where DL cell specific Pilot symbols get transmitted */
    symNum[0] = 0; // port 0,1
    symNum[1] = NDL_SYMB-3; // port 0,1
    numSym = 2;

    /* if ports 2,3 are defined add another symbol */
    if ( numBsTxAntPort > 2 )
    {
        numSym = numSym + 1;
        symNum[2] = 1;
    }


    /* Loop for all combinations of slots, symbols */
    for ( slotIndex = 0; slotIndex < NUM_SLOTS; slotIndex++ )
    {
        /* Loop for all symbols in a slot */
        for ( symIndx = 0; symIndx < numSym; symIndx++ )
        {
            /* compute cinit - for port 0 and port 1 */
            cinit = (const4) * (7 * (slotIndex + 1) + symNum[symIndx] + 1) + const5;

            /* generate the scrambling sequence */
            PseudoRandomSeqGen(4 * (CE_NmaxDL_rb), cinit, pScrambleSeq);

            /* generate the cell specific sequence */
            for (m = 0; m < (2 * numDLRb); m++)
            {
                //input index into r() mdash    45
                mdash = (S16)((m + const3)<<1);

                seq1 = OneOverSqrt2[pScrambleSeq[mdash]];
                *pDLCellSpecificRefSeq++ = seq1;

                seq2 = OneOverSqrt2[pScrambleSeq[mdash+1]];
                *pDLCellSpecificRefSeq++ = seq2;
            }//m
        }
    }

    return;
}


//-----------------------------------------------------------------------------
/** @brief Generate the DL scrambling sequence for PDCCH for all the sub-frames
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param   cellId             cellId (0-503)
 *  @param   pDLCellSpecificScrSeq  pointer to cell specific reference sequence
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------
 *  |Name          |Direction|Length  |Type  |Data Format |Valid Range |Units|
 *  |              |         |        |      |            |            |     |
 *  --------------------------------------------------------------------------
 *  |cellId        |input    |1       |S16   |(0:15:0)    |[0:503]     |     |
 *  --------------------------------------------------------------------------
 *  |pDLCellSpecificScrSeq|pOut |1      |S8*  |            |            |   |
 *  --------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |LteBsPhyInitDLCellSpecificScrSeq|
 *  -----------------------------------------------------
 *  |Input Stream        |None                          |
 *  -----------------------------------------------------
 *  |Output Stream       |pDLCellSpecificRefSeq,        |
 *  -----------------------------------------------------
 *  |Config Parameters   |cellID                        |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 * As the cell ID does not change constantly the DL cell specific
 * sequence for all sub-frames can be pre-computed and saved in a table.
 *
 * Psuedo Logic:
 *
 *
 *  <5> Cycle Count Formula:
 *  Run at initialization only.
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-----------------------------------------------------------------------------
static void LteBsPhyInitDLCellSpecificScrSeq(S16 cellId, U8 *pDLCellSpecificScrSeq)
{
    /* Local Variables */
    U32 cinit, m;
    U8* startSubfr;
    U8* in;

#ifndef WIN32
    MDMA_FDESC *pMdma;

    in = MemAlloc(800, DDR1_NCNB_MEM, 1);
    startSubfr = pDLCellSpecificScrSeq;
    for(m=0; m<10; m++)
    {
        pMdma = MdmaAllocDesc (MDMA_HW_SYS, 1);
        _ASSERT_PTR (pMdma);

        cinit = (m << 9) + cellId;

        MdmaSet4GSCR(pMdma, cinit, 1, 0, 0);
        MdmaSetData(pMdma, (U32*) in, 800, (U32*) startSubfr, 800);
        MdmaRunEx (MDMA_HW_SYS, pMdma, NULL, NULL);
        startSubfr += 800;
    }
    MemFree(in, DDR1_NCNB_MEM);
#else
    in = MemAlloc(800, DDR1_NCNB_MEM, 1);
    startSubfr = pDLCellSpecificScrSeq;
    for(m=0; m<10; m++)
    {
        cinit = (m << 9) + cellId;
        scrambler_byte(cinit, in, startSubfr , 800);
        startSubfr += 800;
    }
    MemFree(in, DDR1_NCNB_MEM);
#endif

    return;
}

//-----------------------------------------------------------------------------
/** @brief Initialize the broadcast channel data for all the SFN
 * (system frame number)
 *
 *  <1> Hardware Resource:
 *  ARM + FEC + MDMA
 *
 *  @param   pBchMemBuf     Pointer to the memory where BCH data will be saved
 *                          (size of the buffer = 1920 x 256 bits)
 *  @param   dlBandwidth    Downlink bandwith ( 32 bits - the three lower
 *                          bits are valid) value 0=6 RB, value 1 = 15 RB etc.
 *  @param   phichDuration  phich_duration (32 bits - lower 1 bit is valid)
 *  @param   phichResource  phich resource (32 bits - lower 2 bits are valid)
 *                          bit 3 - phich-duration, bits 2-0 - phich-resource
 *  @param   cellId         cellId (0-503)
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------
 *  |Name          |Direction|Length  |Type  |Data Format |Valid Range |Units|
 *  |              |         |        |      |            |            |     |
 *  --------------------------------------------------------------------------
 *  |pBchMemPtr    |pInOut   |        |U32   |            |            |     |
 *  --------------------------------------------------------------------------
 *  |dlBandwidth   |input    |1       |U32   |(0:31:0)    |[0:5]       |     |
 *  --------------------------------------------------------------------------
 *  |phichDuration |input    |1       |U32   |(0:31:0)    |[0:1]       |     |
 *  --------------------------------------------------------------------------
 *  |phichResource |input    |1       |U32   |(0:31:0)    |[0:3]       |     |
 *  --------------------------------------------------------------------------
 *  |cellId        |input    |1       |U32   |(0:31:0)    |[0:503]     |     |
 *  --------------------------------------------------------------------------
 *  |NtxAnt        |input    |1       |U32   |(0:31:0)    |[0:4]       |     |
 *  --------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |LteBsPhyInitBchData           |
 *  -----------------------------------------------------
 *  |Input Stream        |None                          |
 *  -----------------------------------------------------
 *  |Output Stream       |pBchMemBuf                    |
 *  -----------------------------------------------------
 *  |Config Parameters   |dlBW,phichCfg,cellID,NtxAnt   |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 * The BCH data can be pre-generated since the Master Information Block (MIB)
 * data does not change except for System Frame Number (SFN).
 * The table will contain 1920 x 256 bits. Every 40 msec increment this table
 * by 1920 bits to point to the valid BCH data for this frame.
 *
 * Psuedo Logic:
 *  Generate scrambling code based on cellID with length = 1920 bits
 *  for every SFN:
 *      generate 24-bits of BCH payload with appropriate SFN
 *      generate CRC for this BCH payload
 *      generate the encoding and rate-matching output
 *      scramble the data based on cell-id
 *      save the data into the table
 *  end
 *
 *
 *  <5> Cycle Count Formula:
 *  Run at initialization only.
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-----------------------------------------------------------------------------
static void LteBsPhyInitBchData ( PBSDLINITPARAM  pBsDlInitParam,
                                   PBSDLINITMEMBUF pBsDlInitMemBuf )
{
    // Local variables
    U32     sfnIndex;
    U32     phichConfig = 0;
    U32     bchPayLoad = 0, tempPayLoad;
    S8      payLoad[3];
    U32     numSpareBits=10; // 10 spareBits
    U32     shiftVal = BCCH_PAYLOAD_IN_BYTES*8;
    U32     phichDuration = (U32)(pBsDlInitParam->phichDuration);
    U32     phichResource = (U32)(pBsDlInitParam->phichResource);
    U32     dlBandwidth   = (U32)(pBsDlInitParam->dlBandwidth);
    S16     numBsTxAntPort = pBsDlInitParam->numBsTxAntPort;
    U32     cellId         = pBsDlInitParam->cellId;
    U32      *pBchMemBuf = pBsDlInitMemBuf->pBchDataBuffer;

    // Setup the phich configuration - 3 bits
    // MSB - phichDuration, 2 LSB - phich resource
    phichConfig = ( ( phichDuration & 0x1 ) << (2) );
    phichConfig = ( ( phichResource & 0x3 ) | phichConfig );

    // setup the BCH payload
    bchPayLoad = ( bchPayLoad | (dlBandwidth & 0x7) << (shiftVal-3) );
    bchPayLoad = ( bchPayLoad | (phichConfig & 0x7) << (shiftVal-6) );


    // Loop for all SFN to create the BCH data after scrambling
    for ( sfnIndex = 0; sfnIndex < 256; sfnIndex++ )
    {
        // update bch payload with SFN
        tempPayLoad = ( bchPayLoad | ( sfnIndex << (numSpareBits) ) );

        // payLoad is in Bytes (S8)
        payLoad[0] = (tempPayLoad>>16)&0xFF;
        payLoad[1] = (tempPayLoad>>8)&0xFF;
        payLoad[2] = (tempPayLoad)&0xFF;

        // call channel encoder with rate matching
        bs_tx_broadcast_channel_coding ( (U8 *)payLoad, (U8 *)pBchMemBuf,
                                         BCCH_PAYLOAD_IN_BYTES, (S16)numBsTxAntPort,
                                         PBCH_LENGTH_IN_BITS );


        scrambler_byte ( cellId, (S8 *)pBchMemBuf, (S8 *)pBchMemBuf,
                             PBCH_LEN_IN_BYTES);

        // Increment the memory buffer for the next SFN
        pBchMemBuf = (U32 *)(pBchMemBuf) + PBCH_LEN_IN_BYTES/4;
    }

    return;
}

//-----------------------------------------------------------------------------
/** @brief Initialize all the common DL parameters based on MIB (or some SIB)
 *  parameters.These tables will be used during Downlink processing.
 *  It is assumed that the parameters in this list are static parameters and
 *  change relatively infrequenctly.
 *
 *
 *  <1> Hardware Resource:
 *  ARM + HW(?)
 *
 *  @param   pDlInitParam   Pointer to DL Initialization parameter structure
 *  @param   pDlInitMemBuf  Pointer to DL initialization buffers
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------
 *  |Name          |Direction|Length  |Type  |Data Format |Valid Range |Units|
 *  |              |         |        |      |            |            |     |
 *  --------------------------------------------------------------------------
 *  |pDlInitParam  |pInOut   |        |U32   |            |            |     |
 *  --------------------------------------------------------------------------
 *  |pDlInitMemBuf |input    |1       |U32   |(0:31:0)    |[0:5]       |     |
 *  --------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |LteBsPhyDlCommonInit          |
 *  -----------------------------------------------------
 *  |Input Stream        |None                          |
 *  -----------------------------------------------------
 *  |Output Stream       |pDlInitMemBuf                 |
 *  -----------------------------------------------------
 *  |Config Parameters   |refer to:pDlInitParam struct  |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 * Top level function to call all the underlying routines for intialization of
 * all the DL tables.
 * NOTE: We need to support a re-configuration method in future.
 *
 * Psuedo Logic:
 *
 *
 *  <5> Cycle Count Formula:
 *  Run at initialization only.
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-----------------------------------------------------------------------------
void LteBsPhyDLCommonInit ( PBSDLINITPARAM  pBsDlInitParam,
                            PBSDLINITMEMBUF pBsDlInitMemBuf )
{

    /* pointer to buffers */
    S16 *pCFIBuffer = pBsDlInitMemBuf->pCfiMemBuffer;
    S16 *pCfiREMapTable = pBsDlInitMemBuf->pCfiReMapBuffer;
    S16 *pHIMemBuf = pBsDlInitMemBuf->pHIMemBuffer;
    S16 *pHiRELoc = pBsDlInitMemBuf->pHIREMapTable;
    S16 *pPDCCHPermuteTable = pBsDlInitMemBuf->pPDCCHPermuteTable;
    S16 *pPDCCHREGLen = pBsDlInitMemBuf->pPDCCHQuadrupletsLen;
    S16 *pPdcchRETable = pBsDlInitMemBuf->pPDCCHReMapTable;
    S16 *pDLCellSpecificRefSeq = pBsDlInitMemBuf->pDLCellSpecificRefSeq;
    U8 *pDLCellSpecificScrSeq = pBsDlInitMemBuf->pDLCellSpecificScrSeq;

    /* get all the relevant parameters */
    U32 numDlRb = pBsDlInitParam->numDlRb;
    U32 cellId  = pBsDlInitParam->cellId;
    U32 numBsTxAntPort = pBsDlInitParam->numBsTxAntPort;
    S16 phichResource = pBsDlInitParam->phichResource;
    S16 phichDuration = pBsDlInitParam->phichDuration;
    S16 *pNumPHICHGrp = &pBsDlInitParam->numPHICHGrp;
    S16 cpType = pBsDlInitParam->cyclicPrefixType;

    /* Generate the BCH table */
    LteBsPhyInitBchData ( pBsDlInitParam, pBsDlInitMemBuf );

    /* Generate tables associated with CFI */
    LteBsPhyInitCFIData ( pCFIBuffer, numDlRb, cellId, numBsTxAntPort, pCfiREMapTable);

    /* Generate tables associated with PHICH */
    LteBsPhyInitHIData ( pHIMemBuf, (S16)numDlRb, (S16)cellId, (S16)numBsTxAntPort,
                        (S16)phichResource, (S16)phichDuration, pHiRELoc, pNumPHICHGrp );

    /* Generate tables associated with PDCCH */
    LteBsPhyInitPDCCHData (  (S16)numDlRb, (S16)cellId, (S16)numBsTxAntPort,
                             (S16)phichResource, (S16)phichDuration, pPDCCHPermuteTable,
                             pPDCCHREGLen, pPdcchRETable );


    /* Generate the DL cell specific reference sequence */
    LteBsPhyInitDLCellSpecificRefSeq ( (S16)numDlRb, (S16)cellId, cpType, (S16)numBsTxAntPort,
                                       pDLCellSpecificRefSeq );

    /* Generate the DL cell specific scrambling sequences */
    LteBsPhyInitDLCellSpecificScrSeq ((S16)cellId, pDLCellSpecificScrSeq );

    return;
}
