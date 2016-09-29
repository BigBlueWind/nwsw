//-------------------------------------------------------------------------------------------
/** @file LteBsPhyDLCommonInit.c
 *
 * @brief This file implements all the processing functions for DL control channels. The
 * channels that are addressed are PCFICH, PHICH and PDCCH.
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.20.8.1 $
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

#ifdef CEVA_INTRINSICS
#include <basic_op.h>
extern U8* DataBuff_Bank2;
extern U8* DataBuff_Bank3;
#else
#include "basic_op_cc.h"
#endif

//-----------------------------------------------------------------------------
/** @brief Function to map symbol data to sub-carriers based on the resource
 *  element table. The data is also scaled before being mapped to the output.
 *  This is a utility function - should be moved to utils.c file?
 *
 *  <1> Hardware Resource:
 *  ARM/CEVA
 *
 *  @param   pInput         pointer to input symbol data (complex)
 *  @param   pREMapTable    pointer to resource element map table
 *  @param   length         Length of data being written to output
 *  @param   scale          scaling of data
 *  @param   pOutput        pointer to output symbol data (complex)
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
 *  |pInput        |input    |1       |S16   |(0:15:0)    |[Q15]       |     |
 *  --------------------------------------------------------------------------
 *  |pREMapTable   |input    |1       |S16   |(0:15:0)    |[0:1199]    |     |
 *  --------------------------------------------------------------------------
 *  |length        |input    |        |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  |scale         |input    |        |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  |pOutput       |output   |1       |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |LteBsPhyDLScaleAndMap         |
 *  -----------------------------------------------------
 *  |Input Stream        |pInput                        |
 *  -----------------------------------------------------
 *  |Output Stream       |pOutput                       |
 *  -----------------------------------------------------
 *  |Config Parameters   |                              |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *
 *
 *  <5> Cycle Count Formula:
 *
 *
 *
 *  <6> References:
 *
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-----------------------------------------------------------------------------
static void LteBsPhyDLScaleAndMap( S16 *pInput, S16 *pREMapTable,
                                   S16 lShiftValue, S16 length, S16 scale,
                                   S16 *pOutput )
{

    /* Local variables */
    S32 scIndx;
    S32 scLoc;
//  S32 reIndx, imIndx;
    S16 tempRe, tempIm;

    scIndx = 0;

    /* get sub-carrier location */
    scLoc = ((*pREMapTable++)*2);
    /* for real data index scaled with antenna index */
//  reIndx = scIndx*2;
    /* for imaginary data */
//  imIndx = reIndx + 1;

    /* increment */
    scIndx++;

    /* Loop through the sub-carriers - scale and resource map */
    do
    {
        /* write - real, imag data with scaling */
        tempRe = mult(*pInput++, scale);
        /* write to imaginary subcarrier location */
        tempIm = mult(*pInput++, scale);

        pOutput[scLoc]   = shl( tempRe, lShiftValue);
        pOutput[scLoc+1] = shl( tempIm, lShiftValue);

        /* get sub-carrier location */
        scLoc = ((*pREMapTable++)*2);
        /* for real data index scaled with antenna index */
//      reIndx = scIndx*2;
        /* for imaginary data */
//      imIndx = scIndx*2 + 1;

//      scIndx++;

    } while ( scIndx++ < length );

    return;
}


//-----------------------------------------------------------------------------
/** @brief Function to modulate and map the input PDCCH byte stream. Based
 *  on the permuted table and the valid bytes information - the appropriate
 *  bytes are selected for modulation and precoding and mapping to output
 *  frequency.
 *
 *  <1> Hardware Resource:
 *  ARM/CEVA
 *
 *  @param   pInput         pointer to input symbol data (complex)
 *  @param   pREMapTable    pointer to resource element map table
 *  @param   length         Length of data being written to output
 *  @param   scale          scaling of data
 *  @param   pOutput        pointer to output symbol data (complex)
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
 *  |pInput        |input    |1       |S16   |(0:15:0)    |[Q15]       |     |
 *  --------------------------------------------------------------------------
 *  |pREMapTable   |input    |1       |S16   |(0:15:0)    |[0:1199]    |     |
 *  --------------------------------------------------------------------------
 *  |length        |input    |        |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  |scale         |input    |        |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  |pOutput       |output   |1       |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |modulateMapPDCCH              |
 *  -----------------------------------------------------
 *  |Input Stream        |PDCCH byte stream,validByteTbl|
 *  -----------------------------------------------------
 *  |Output Stream       |pAntPortOut                   |
 *  -----------------------------------------------------
 *  |Config Parameters   | symbolNum,txMode etc         |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *
 *
 *  <5> Cycle Count Formula:
 *
 *
 *
 *  <6> References:
 *
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-----------------------------------------------------------------------------
static void modulateMapPDCCH (U8 *pPdcchBytes, S16 *pValidByteTbl,
                              U32 transmissionMode, S16 numBsTxAntPort,
                              S16 *pPdcchScale, S16 *pREMapTable,
                              S16 symbolNum, S16 regLen, S16 *pPermuteTbl,
                              S16 *pBsTxAntPortOut[] )
{
    /* local variables */
    S32 indx;
    U8  *locBytes;
    S16 *validByteLoc;
    S16 *tempIQBuffer;
    S16 *precodeBuffer;
    S16 *pPrecodeBuffer;
    S32 indxVal, antIndx, offset, offset1;
    S16 numSymbols = ( regLen << 2 ), pdcchScale = *pPdcchScale;
    S16 *pTxAntPtr;
    S32 *pTempIQBuf32;

    validByteLoc = (S16*) (DataBuff_Bank2 + MAX_PDCCH_BYTES*sizeof(S16));
    tempIQBuffer = (S16*) (DataBuff_Bank2 + 2*MAX_PDCCH_BYTES*sizeof(S16));
    locBytes = (U8*) (DataBuff_Bank2 + 3*MAX_PDCCH_BYTES*sizeof(S16) + ((MAX_REG_PER_SYMBOL*MAX_NUM_RB)<<3) * sizeof(S16));
    precodeBuffer = (S16*)(DataBuff_Bank2 + 7*MAX_PDCCH_BYTES + ((MAX_REG_PER_SYMBOL*MAX_NUM_RB)<<3) * sizeof(S16));

    pPrecodeBuffer = precodeBuffer;

    /* Get the bytes to modulate */
    for ( indx = 0; indx < regLen; indx++ )
    {
        /* get the permute byte location */
        indxVal = pPermuteTbl[indx];
        /* permute table */
        locBytes[indx] = pPdcchBytes[indxVal];
        validByteLoc[indx] = pValidByteTbl[indxVal];
    }

    /* Modulate the bytes */
    tx_Mapper(locBytes, numSymbols, 0, MOD_QPSK, tempIQBuffer);

    /* Null the bytes which are unused. These are the <NIL> bytes
     * as specified by the standard 36.211 */
    pTempIQBuf32 = (S32 *)tempIQBuffer;
    for ( indx = 0; indx < numSymbols; indx++ )
    {
        if ( validByteLoc[(indx>>2)] == -1 )
        {
//          tempIQBuffer[(indx<<1)]   = 0; // real
//          tempIQBuffer[(indx<<1)+1] = 0; // imaginary
            pTempIQBuf32[indx] = 0;
        }
    }

    /* check if we need to do diversity based on tramsission mode */
    if ( transmissionMode != TXDIVERSITY )
    {
        /* for single antenna - just do mem copy */
        memcpy ( precodeBuffer, tempIQBuffer, sizeof ( S16 )*(numSymbols*2) );
    }
    else
    {
        STC ( tempIQBuffer, numSymbols, numBsTxAntPort, precodeBuffer );
    }

    /* copy to the output buffer */
    offset1 = symbolNum * numBsTxAntPort;
    for (antIndx = 0; antIndx < numBsTxAntPort; antIndx++)
    {
        /* Set the pointers accordingly */
        pTxAntPtr = pBsTxAntPortOut[antIndx + offset1];
        offset = numSymbols * antIndx;

        /* set pointer per antenna */
        pPrecodeBuffer = &precodeBuffer[offset*2];

        /* scale and map data based on Resource Map Table */
        LteBsPhyDLScaleAndMap ( pPrecodeBuffer, pREMapTable,
                                0, numSymbols, pdcchScale, pTxAntPtr);
    }

    return;
}

//-----------------------------------------------------------------------------
/** @brief Function to compute offset to PCFICH buffer in memory. A helper
 * function - to be used to get offset into the memory so we can copy
 * only the buffer we need per call to PCFICHProc function.
 *
 *  <1> Hardware Resource:
 *  ARM/CEVA
 *
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
 *  --------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |computeOffsetToPCFICHBuffer   |
 *  -----------------------------------------------------
 *  |Input Stream        |                              |
 *  -----------------------------------------------------
 *  |Output Stream       | offset                       |
 *  -----------------------------------------------------
 *  |Config Parameters   | symbolNum,txMode etc         |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *
 *
 *  <5> Cycle Count Formula:
 *
 *
 *
 *  <6> References:
 *
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-----------------------------------------------------------------------------
void computeOffsetToPCFICHBuffer ( S16 numCtrlSymbols, S16 numBsTxAntPort,
                                   S16 subFrameNum, S16 *pOffset)
{
    S16 offset;

    /* get the offset */
    offset = ((NUM_SLOTS>>1)*(numCtrlSymbols-1) + subFrameNum);
    offset = offset*(NUMBER_OF_CFI_BITS*numBsTxAntPort);

    /* get offset to the CFI buffer */
    *pOffset = offset;

    return;
}

//-----------------------------------------------------------------------------
/** @brief Function to generate the PCFICH channel and map it to the resource
 * elements.
 *
 *  <1> Hardware Resource:
 *  ARM/CEVA
 *
 *  @param   pInput         pointer to input symbol data (complex)
 *  @param   pREMapTable    pointer to resource element map table
 *  @param   length         Length of data being written to output
 *  @param   scale          scaling of data
 *  @param   pOutput        pointer to output symbol data (complex)
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
 *  |pInput        |input    |1       |S16   |(0:15:0)    |[Q15]       |     |
 *  --------------------------------------------------------------------------
 *  |pREMapTable   |input    |1       |S16   |(0:15:0)    |[0:1199]    |     |
 *  --------------------------------------------------------------------------
 *  |length        |input    |        |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  |scale         |input    |        |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  |pOutput       |output   |1       |S16   |            |            |     |
 *  --------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |LteBsPhyPCFICHProc            |
 *  -----------------------------------------------------
 *  |Input Stream        |PCFICH stream                 |
 *  -----------------------------------------------------
 *  |Output Stream       |pAntPortOut                   |
 *  -----------------------------------------------------
 *  |Config Parameters   | symbolNum,txMode etc         |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *
 *
 *  <5> Cycle Count Formula:
 *
 *
 *
 *  <6> References:
 *
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-----------------------------------------------------------------------------
void LteBsPhyPCFICHProc(S16 *pCFIMemBuf, S16 *pCfiREMapTable,
                        S16 numCtrlSymbols, S16 numBsTxAntPort,
                        S16 subFrameNum, S16 pcfichScale,
                        S16 computeOffset, S16 *pBsTxAntPortOut[])
{
    /* Local Variables */
    S16 antIndx;
    S16 *pCurrCFIBuf, *pTxAntPtr;
    S16 offset = 0;

    /* if offset was computed outside don't compute it */
    if ( computeOffset )
    {
        /* get the offset */
        offset = ((NUM_SLOTS>>1)*(numCtrlSymbols-1) + subFrameNum);
        offset = offset*(NUMBER_OF_CFI_BITS*numBsTxAntPort);
    }

    /* Get pointer to the current CFI buffer */
    pCurrCFIBuf = pCFIMemBuf + offset;

    /* copy to the output buffer */
    for (antIndx = 0; antIndx < numBsTxAntPort; antIndx++)
    {
        /* Set the pointers accordingly */
        pTxAntPtr = pBsTxAntPortOut[antIndx];
        offset = ( NUMBER_OF_CFI_BITS )*antIndx;

        /* set pointer per antenna */
        pCurrCFIBuf = pCurrCFIBuf + offset;

        /* scale and map data based on Resource Map Table */
        LteBsPhyDLScaleAndMap ( pCurrCFIBuf, pCfiREMapTable,
                                0, (NUMBER_OF_CFI_BITS>>1), pcfichScale,
                                pTxAntPtr);
    }

    return;
}


//-----------------------------------------------------------------------------
/** @brief Function to compute offset to PHICH buffer in memory. A helper
 * function - to be used to get offset into the memory so we can copy
 * only the buffer we need per call to PCFICHProc function.
 *
 *  <1> Hardware Resource:
 *  ARM/CEVA
 *
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
 *  --------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |computeOffsetToPHICHBuffer    |
 *  -----------------------------------------------------
 *  |Input Stream        |                              |
 *  -----------------------------------------------------
 *  |Output Stream       | offset                       |
 *  -----------------------------------------------------
 *  |Config Parameters   | symbolNum,txMode etc         |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *
 *
 *  <5> Cycle Count Formula:
 *
 *
 *
 *  <6> References:
 *
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-----------------------------------------------------------------------------
void computeOffsetToPHICHBuffer ( S16 numBsTxAntPort, S16 subFrameNum,
                                  S16 *pOffset)
{
    S16 offset;

    /* Set pointer to the PHICH sequence location based on subframe number */
    offset = 2 * NUMBER_OF_HI_BITS_NORMAL_CP * NUM_PHICH_SEQ_INDX_NORMAL_CP;

    /* pointer now points to all sequence indices symbols for sub-frame*/
    offset = ( subFrameNum * offset * numBsTxAntPort );

    /* get offset to the CFI buffer */
    *pOffset = offset;

    return;
}


//-----------------------------------------------------------------------------
/** @brief Function to generate the PHICH channel and map it to the resource
 * elements.
 *
 *  <1> Hardware Resource:
 *  ARM/CEVA
 *
 *  @param   numPHICHChannels        number of PHICH channels
 *  @param   pPhichHarqBits          pointer to the HARQ bits
 *  @param   pPhichSeqIndx           pointer to PHICH sequence index
 *  @param   pPhichGrpNum            pointer to PHICH group num
 *  @param   pHISymBuf               pointer to PHICH symbol buffer
 *  @param   pHiREMapTable           pointer to PHICH RE map table
 *  @param   numBsTxAntPort          number of BS transmit antenna port
 *  @param   subFrameNum             sub-frame number
 *  @param   pPHICHScale             scale value
 *  @param   pBsTxAntPortOut         pointer to the resource element grid
 *
 *
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
  *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |LteBsPhyPHICHProc             |
 *  -----------------------------------------------------
 *  |Input Stream        |                              |
 *  -----------------------------------------------------
 *  |Output Stream       |pAntPortOut                   |
 *  -----------------------------------------------------
 *  |Config Parameters   | subFrame Number              |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *
 *
 *  <5> Cycle Count Formula:
 *
 *
 *
 *  <6> References:
 *
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-----------------------------------------------------------------------------
void LteBsPhyPHICHProc( S16 numPHICHChannels, S16 *pPhichHarqBits,
                        S16 *pPhichSeqIndx, S16 *pPhichGrpNum,
                        S16 numPHICHGrp, S16 *pHISymBuf,
                        S16 *pHiREMapTable, S16 numBsTxAntPort,
                        S16 subFrameNum, S16 computeOffset,
                        S16 *pPhichScale, S16 *pBsTxAntPortOut[])
{
    /* Local variables */
    S16 *pLocPHICHPtr, *pSeq, offset, offsetMem;
    S16 seqIndexVec[MAX_PHICH];
    S16 phichBitVec[MAX_PHICH];
    S16 symbol[2] = {0};
    S16 reIndx, imIndx;
    S32 phichGrpIndx, indx, seqIndx, symIndx;
    S16 seqBuf[MAX_NUM_ANT * 2*NUMBER_OF_HI_BITS_NORMAL_CP];
    S16 *pSeqBuf;
    S16 numPhichBits = NUMBER_OF_HI_BITS_NORMAL_CP;
    S16 *pTxAntPtr, regIndx, mapIndx, antennaVec[] = {0,2};
    S32 startScIndx, antIndx;
    S16 *pLocHiREMapTable;

    /* check if offset needs to be computed or not */
    if ( computeOffset )
    {
        /* Set pointer to the PHICH sequence location based on subframe number */
        offset = 2 * numPhichBits * NUM_PHICH_SEQ_INDX_NORMAL_CP;

        /* pointer now points to all sequence indices symbols for sub-frame*/
        pLocPHICHPtr = pHISymBuf + ( subFrameNum * offset * numBsTxAntPort );
    }
    else
    {
        /* set it accordingly */
        pLocPHICHPtr = pHISymBuf;
    }

    /* identify all channels which belong to one group */
    /* initialize to zero */
    for ( phichGrpIndx = 0; phichGrpIndx < numPHICHGrp; phichGrpIndx++ )
    {
        seqIndexVec[phichGrpIndx] = 0;
        phichBitVec[phichGrpIndx] = 0;
    }

    /* Now apporpriately set the valid sequence indices per group */
    for ( indx = 0; indx < numPHICHChannels; indx++ )
    {
        /* set the sequence index */
        seqIndexVec[pPhichGrpNum[indx]] |= ( 1 << (pPhichSeqIndx[indx]) );
        phichBitVec[pPhichGrpNum[indx]] |= ( pPhichHarqBits[indx] <<
                                               (pPhichSeqIndx[indx]) );
    }

    /* set offset */
    offset = ( numPhichBits * numBsTxAntPort );

    /* set the startSC indx = 0 */
    startScIndx = 0;

    /* For each channel - obtain the symbol sequence */
    for ( indx = 0; indx < numPHICHGrp; indx++ )
    {
        /* initialize buffer to zero */
        memset ( seqBuf, 0, sizeof ( S16 )*numBsTxAntPort*2*numPhichBits );

        /* get the sequence index */
        for ( seqIndx = 0; seqIndx < NUM_PHICH_SEQ_INDX_NORMAL_CP; seqIndx++ )
        {
            /* check if the sequence index is present */
            if ( ( seqIndexVec[indx]>>seqIndx ) & 0x1 )
            {
                /* obtain the sequence and negate it if HARQ bit is zero
                 * and add to the sequence */
                pSeq = pLocPHICHPtr + ( seqIndx * offset * 2 );

                /* scale, add and negate if bit is zero */
                for ( symIndx = 0; symIndx < offset; symIndx++ )
                {
                    reIndx = ( (S16)symIndx << 1 );
                    imIndx = reIndx + 1;

                    symbol[0] = ( pSeq[reIndx]>>3  );
                    symbol[1] = ( pSeq[imIndx]>>3  );

                    /* scale and add */
                    if ( ( ( phichBitVec[indx]>>seqIndx ) & 0x1 ) == 0 )
                    {
                        /* negate */
                        symbol[0] = negate ( symbol[0] );
                        symbol[1] = negate ( symbol[1] );
                    }

                    /* adding for all indices in the group */
                    seqBuf[reIndx] = add( seqBuf[reIndx], (symbol[0]) );
                    seqBuf[imIndx] = add( seqBuf[imIndx], (symbol[1]) );
                } /* symIndx */
            } /* if seqIndx==1 */
        }/* seqIndx loop */

        /* set pointer to sequence buffer */
        pSeqBuf = seqBuf;

        /* update the resource map table */
        pLocHiREMapTable = &(pHiREMapTable[startScIndx]);

        /* Resource Mapping */
        if ( numBsTxAntPort <= 2 )
        {
            /* copy to the output buffer */
            for (antIndx = 0; antIndx < numBsTxAntPort; antIndx++)
            {
                /* Set the pointers accordingly */
                pTxAntPtr = pBsTxAntPortOut[antIndx];
                offsetMem = ( 2*numPhichBits )*(S16)(antIndx);

                /* appropriately adjust the pointer */
                pSeqBuf = pSeqBuf + offsetMem;

                /* scale and map data based on Resource Map Table */
                LteBsPhyDLScaleAndMap ( pSeqBuf, pLocHiREMapTable,
                                        3, numPhichBits, pPhichScale[0],//check
                                        pTxAntPtr);

            }

            /* update startScIndx */
            startScIndx = startScIndx + numPhichBits;
        }
        else
        {
            for ( regIndx = 0; regIndx < 3; regIndx++ )
            {
                /* break it into 3 REG for mapping */
                mapIndx = ( regIndx + indx ) & 0x1 ;

                /* when (i+nPHICH_Grp)mod2 == 0, antenna ports (0,2) */
                /* copy to the output buffer */
                for (antIndx = 0; antIndx < 2; antIndx++)
                {
                    /* Set the pointers accordingly */
                    pTxAntPtr = pBsTxAntPortOut[antennaVec[antIndx]+mapIndx];
                    offsetMem = numPhichBits*antennaVec[antIndx];

                    /* appropriately adjust the pointer */
                    pSeqBuf = pSeqBuf + offsetMem;

                    /* update the resource map table */
                    pHiREMapTable = pHiREMapTable + startScIndx;

                    /* scale and map data based on Resource Map Table */
                    LteBsPhyDLScaleAndMap ( pSeqBuf, pHiREMapTable,
                                            3, numPhichBits, pPhichScale[indx],
                                            pTxAntPtr);
                }
            }

            /* update startScIndx */
            startScIndx = startScIndx + numPhichBits;
        }
    } /* numPHICHGrp loop */

    return;
}

//-----------------------------------------------------------------------------
/** @brief Function to generate the PDCCH channel and map it to the resource
 * elements.
 *
 *  <1> Hardware Resource:
 *  ARM/CEVA
 *
 *  @param   numPDCCHChannels        number of PDCCH channels
 *  @param   pNumCCE                 pointer to number of CCE
 *  @param   pStartCCE               pointer to PHICH sequence index
 *  @param   numCtrlSymbols          number of control symbols
 *  @param   transmissionMode        transmission mode (should be enum)
 *  @param   pPdcchPermuteTable      pointer to the PDCCH permutation table
 *  @param   pPdcchREMapTable        pointer to the PDCCH resource map table
 *  @param   pPdcchREGLen            pointer to pdcch REG len
 *  @param   numBsTxAntPort          number of base station transmit antenna ports
 *  @param   pPdcchScale             pointer to pdcch scale value
 *  @param   pBsTxAntPortOut         pointer to the resource element grid
 *
 *
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
  *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |LteBsPhyPDCCHProc             |
 *  -----------------------------------------------------
 *  |Input Stream        |                              |
 *  -----------------------------------------------------
 *  |Output Stream       |pAntPortOut                   |
 *  -----------------------------------------------------
 *  |Config Parameters   | subFrame Number              |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *
 *
 *  <5> Cycle Count Formula:
 *
 *
 *
 *  <6> References:
 *
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-----------------------------------------------------------------------------
void LteBsPhyPDCCHProc(S16 numPDCCHChannels, U8 *pPdcchBytes,
                       S16 *pNumCCE, S16 *pStartCCE,
                       S16 numCtrlSymbols, U32 transmissionMode,
                       S16 *pPdcchPermuteTable, S16 *pPdcchREMapTable,
                       S16 *pPdcchREGLen, S16 numBsTxAntPort,
                       S16 *pPdcchScale, S16 *pBsTxAntPortOut[])
{
    /* Local Variables */
    S16 *validByteLoc;
    S32 indx, chanIndx, symIndx;
    S16 startVal, endVal, symbolNum;
    S16 *pPermuteSym[MAX_DL_CONTROL_SYM];
    S16 *pREMapTableSym[MAX_DL_CONTROL_SYM];
    S16 temp16;
    S16 numPDCCHBytes[MAX_DL_CONTROL_SYM];

    validByteLoc = (S16*) DataBuff_Bank2;

    /* Initially set all the valid byte locations to -ve 1 */
    memset ( validByteLoc, -1, sizeof( S16 )*MAX_PDCCH_BYTES );

    /* set valid byte locations */
    if ( numPDCCHChannels > 0 )
    {
        chanIndx = 0;

        /* set all the valid bytes for all the PDCCH channels */
        do
        {
            startVal = pStartCCE[chanIndx];
            endVal   = startVal + pNumCCE[chanIndx];

            /* get the start indx and number of CCE */
            indx = startVal;
            do
            {
                validByteLoc[indx] = 1;
                indx++;
            } while ( indx < endVal );

            chanIndx++;
        } while ( chanIndx < numPDCCHChannels );
    }

    /* Set pointers to Resource Map tables */
    pREMapTableSym[0] = pPdcchREMapTable;
    pREMapTableSym[1] = pPdcchREMapTable + ( MAX_NUM_RB * N_RB_SC);
    pREMapTableSym[2] = pPdcchREMapTable + 2*(MAX_NUM_RB * N_RB_SC);

    /* per symbol quadruplets */
    numPDCCHBytes[0] = pPdcchREGLen[0];
    numPDCCHBytes[1] = pPdcchREGLen[1] - pPdcchREGLen[0];
    numPDCCHBytes[2] = pPdcchREGLen[2] - pPdcchREGLen[1];

    /* get the appropriate permutation table based on numCtrlSymbols */
    if ( numCtrlSymbols == 1 )
    {
        /* permute table for symbol 0 */
        pPermuteSym[0] = pPdcchPermuteTable;
        /* set symbol number is zero */
        symbolNum = 0;
        /* modulate the PDCCH bytes */
        modulateMapPDCCH ( pPdcchBytes, validByteLoc, transmissionMode,
                           numBsTxAntPort, pPdcchScale, pREMapTableSym[0],
                           symbolNum, numPDCCHBytes[0], pPermuteSym[0],
                           pBsTxAntPortOut );

    }
    else if ( numCtrlSymbols == 2 )
    {
        /* permute table for symbol 0 */
        pPermuteSym[0] = pPdcchPermuteTable + pPdcchREGLen[0];
        /* permute table for symbol 1 */
        pPermuteSym[1] = pPdcchPermuteTable + pPdcchREGLen[0]*2;

        /* modulate and map PDCCH data*/
        for ( symIndx = 0; symIndx < numCtrlSymbols; symIndx++ )
        {
            /* modulate the PDCCH bytes */
            modulateMapPDCCH ( pPdcchBytes, validByteLoc, transmissionMode,
                               numBsTxAntPort, pPdcchScale,
                               pREMapTableSym[symIndx], (S16)symIndx,
                               numPDCCHBytes[symIndx], pPermuteSym[symIndx],
                               pBsTxAntPortOut );
        }
    }
    else if ( numCtrlSymbols == 3 )
    {
        /* set pointer to permute table correctly */
        temp16 = pPdcchREGLen[0] + pPdcchREGLen[1];
        pPermuteSym[0] = pPdcchPermuteTable + temp16;
        pPermuteSym[1] = pPermuteSym[0] + pPdcchREGLen[0];
        pPermuteSym[2] = pPermuteSym[0] + pPdcchREGLen[1];

        /* modulate and map PDCCH data*/
        for ( symIndx = 0; symIndx < numCtrlSymbols; symIndx++ )
        {
            /* modulate the PDCCH bytes */
            modulateMapPDCCH ( pPdcchBytes, validByteLoc, transmissionMode,
                               numBsTxAntPort, pPdcchScale,
                               pREMapTableSym[symIndx], (S16)symIndx,
                               numPDCCHBytes[symIndx], pPermuteSym[symIndx],
                               pBsTxAntPortOut );
        }
    }
    else
    {
        /* throw an error */
    }

    return;
}

//-----------------------------------------------------------------------------
/** @brief Function to compute offset to DL Reference Sequence buffer in memory.
 * A helper function - to be used to get offset into the memory so we can copy
 * only the buffer we need per call to PCFICHProc function.
 *
 *  <1> Hardware Resource:
 *  ARM/CEVA
 *
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
 *  --------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |computeOffsetToDLRefSeqBuffer |
 *  -----------------------------------------------------
 *  |Input Stream        |                              |
 *  -----------------------------------------------------
 *  |Output Stream       | offset                       |
 *  -----------------------------------------------------
 *  |Config Parameters   | symbolNum,txMode etc         |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *
 *
 *  <5> Cycle Count Formula:
 *
 *
 *
 *  <6> References:
 *
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-----------------------------------------------------------------------------
void computeOffsetToDLRefSeqBuffer ( S16 numBsTxAntPort, S16 slotNumber,
                                     S16 symbolNum, S16 numDlRb, S16 *pOffset)
{
    S16 offset;

    /* set the symbol buffer pointer correctly */
    if ( numBsTxAntPort <= 2 )
    {
        // 2 cell specific symbols in a slot
        offset = 8*numDlRb*slotNumber;
    }
    else
    {
        // 3 symbols in a slot
        offset = 12*numDlRb*slotNumber;
    }

    /* Now adjust the pointer to point to the correct symbol buffer */
    if ( symbolNum == 1 )
    {
        offset = offset + 8*numDlRb;
    }
    else if ( symbolNum > 0 )
    {
        offset = offset + 4*numDlRb;
    }

    /* set the offset */
    *pOffset = offset;

    return;
}
//-----------------------------------------------------------------------------
/** @brief Function to generate the DL Reference Seq and map it to the resource
 * elements.
 * NOTE: No error checking is done in this function
 *
 *  <1> Hardware Resource:
 *  ARM/CEVA
 *
 *  @param   pDlRefSeqSymBuffer      pointer to DL reference sequence buffer
 *  @param   cellId                  cell ID
 *  @param   symbolNum               symbol number (0,1 or NDL_symb-3)
 *  @param   numBsTxAntPort          number of transmit antenna ports
 *  @param   slotNumber              slot number
 *  @param   numDlRb                 number of DL RB
 *  @param   pilotScaleFactor        power scaling
 *  @param   pBsTxAntPortOut         pointer to the resource element grid
 *
 *
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
  *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |LteBsPhyDLRefSeqProc          |
 *  -----------------------------------------------------
 *  |Input Stream        |                              |
 *  -----------------------------------------------------
 *  |Output Stream       |pAntPortOut                   |
 *  -----------------------------------------------------
 *  |Config Parameters   | subFrame Number              |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *
 *
 *  <5> Cycle Count Formula:
 *
 *
 *
 *  <6> References:
 *
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-----------------------------------------------------------------------------
void LteBsPhyDLRefSeqProc(S16 *pDlRefSeqSymBuffer, S16 cellId, S16 symbolNum,
                          S16 numBsTxAntPort, S16 slotNumber, S16 numDlRb,
                          S16 pilotScaleFactor, S16 computeOffset,
                          S16 * pBsTxAntPortOut[])
{

    /* Local variables */
    S16 v=0, vPlusVshift[MAX_NUM_ANT_PORT];
    S16 kIndex, vshift; // refer to 36.211 for variable names
    S16 antIndx;
    S16 *pTxAntPtr;
    S16 scaleFactor = pilotScaleFactor;
//  S32 reIndx, imIndx;
    S32 scIndx;
    S16 *pLocRefSeqBufPtr, port01;

    /* we are dividing by sqrt(2) to make the data Q13 */
//  scaleFactor = mult_r(scaleFactor, -11585); // don't like using a number..add #define

    /* compute the start index of the mapping */
    /* compute vshift - refer to 36.211 */
    vshift = (cellId) % 6;

    /* compute the relevant v's and vPlusVshift based on symbol number
     * and antenna port */
    for ( antIndx = 0; antIndx < numBsTxAntPort; antIndx++ )
    {
        port01 = ( antIndx == 0 ) || (antIndx == 1) ;

        /* compute v and vPlusVshift */
        if ( (port01==1) && ( symbolNum == 0 ) ) // p=0,1, l=0 (reference:36.211)
        {
            v = 0 + 3*(antIndx);
        }
        else if ( (port01==1) && ( symbolNum > 0 ) ) // p=0, l != 0 (reference:36.211)
        {
            v = 3 - 3*(antIndx);
        }
        else if ( ( antIndx == 2 ) || (antIndx==3) )
        {
            v = 3*(slotNumber & 0x1 ) + 3*(antIndx-2);
        }
        else
        {
            /* throw an error ?? */
        }

        /* compute vPlusVshift */
        vPlusVshift[antIndx] = ( v + vshift ) % 6;
    }

    /* Check if we need to compute offset or not */
    if ( computeOffset )
    {
        /* set the symbol buffer pointer correctly */
        if ( numBsTxAntPort <= 2 )
        {
            // 2 cell specific symbols in a slot
            pLocRefSeqBufPtr = pDlRefSeqSymBuffer + 8*numDlRb*slotNumber;
        }
        else
        {
            // 3 symbols in a slot
            pLocRefSeqBufPtr = pDlRefSeqSymBuffer + 12*numDlRb*slotNumber;
        }

        /* Now adjust the pointer to point to the correct symbol buffer */
        if ( symbolNum == 1 )
        {
            pLocRefSeqBufPtr = pLocRefSeqBufPtr + 8*numDlRb;
        }
        else if ( symbolNum > 0 )
        {
            pLocRefSeqBufPtr = pLocRefSeqBufPtr + 4*numDlRb;
        }
    }
    else
    {
        pLocRefSeqBufPtr = pDlRefSeqSymBuffer;
    }


    /* Loop through all the transmit antenna ports */
    for ( antIndx = 0; antIndx < numBsTxAntPort; antIndx++ )
    {

        S16 *pTempLocRefSeq = pLocRefSeqBufPtr;

        /* Set the pointers accordingly */
        pTxAntPtr = pBsTxAntPortOut[antIndx + symbolNum*numBsTxAntPort];

        /* set kIndex = subcarrier location */
        kIndex = vPlusVshift[antIndx]*2;

        /* To accommodate the resource element not used by this
         * antenna port */
        if ( (kIndex-6) >= 0 )
        {
            /* write to real location */
            pTxAntPtr[kIndex-6]   = 0;
            /* write to imaginary subcarrier location */
            pTxAntPtr[kIndex-6+1] = 0;
        }

        scIndx = 0; // initialize

        /* for real data index scaled with antenna index */
//      reIndx = scIndx*2;
        /* for imaginary data */
//      imIndx = reIndx + 1;

        /* increment */
        scIndx++;

        /* loop for all subcarriers */
        do
        {
            /* scale and map the data */
            /* write - real, imag data with scaling */
            pTxAntPtr[kIndex]   = mult(*pTempLocRefSeq++, scaleFactor);
            /* write to imaginary subcarrier location */
            pTxAntPtr[kIndex+1] = mult(*pTempLocRefSeq++, scaleFactor);

            /* get next sub-carrier location */
            kIndex += 6;

            pTxAntPtr[kIndex]   = 0;
            /* write to imaginary subcarrier location */
            pTxAntPtr[kIndex+1] = 0;

            /* get next sub-carrier location */
            kIndex += 6;

            /* for real data index scaled with antenna index */
//          reIndx = scIndx*2;
            /* for imaginary data */
//          imIndx = scIndx*2 + 1;

//          scIndx++;

        } while ( scIndx++ < ( 2*numDlRb) );
    }

    return;
}


//-----------------------------------------------------------------------------
/** @brief Top-level function which calls the relevant control functions.
 * NOTE: No error checking is done in this function
 *
 *  <1> Hardware Resource:
 *  ARM/CEVA
 *
 *  @param   pDlRefSeqSymBuffer      pointer to DL reference sequence buffer
 *  @param   cellId                  cell ID
 *  @param   symbolNum               symbol number (0,1 or NDL_symb-3)
 *  @param   numBsTxAntPort          number of transmit antenna ports
 *  @param   slotNumber              slot number
 *  @param   numDlRb                 number of DL RB
 *  @param   pilotScaleFactor        power scaling
 *  @param   pBsTxAntPortOut         pointer to the resource element grid
 *
 *
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
  *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |LteBsPhyDLRefSeqProc          |
 *  -----------------------------------------------------
 *  |Input Stream        |                              |
 *  -----------------------------------------------------
 *  |Output Stream       |pAntPortOut                   |
 *  -----------------------------------------------------
 *  |Config Parameters   | subFrame Number              |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *
 *
 *  <5> Cycle Count Formula:
 *
 *
 *
 *  <6> References:
 *
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-----------------------------------------------------------------------------
void LteBsPhyDLControlProc(PBSDLINITMEMBUF pBsDlInitMemBuf,
                           PIOParams_DlControl pIOParamsDlControl,
                           S16 computeOffset,
                           S16 *StartCCECtl, S16 *NumCCECtl,
                           S16 *pScratchMem,
                           S16 *pBsTxAntPortOut[])
{
    /* local variables */
    S16 numCtrlSymbols, numBsTxAntPort, subFrameNum;
    S16 slotNumber;

    /* get all the associated DL parameters */
    numCtrlSymbols = pIOParamsDlControl->numCtrlSymbols,
    numBsTxAntPort = pIOParamsDlControl->numTxAntennas;
    subFrameNum = (pIOParamsDlControl->slotNumber>>1);
    slotNumber  = pIOParamsDlControl->slotNumber;

#if 0
    {
        S16 indAntPort, iSym;
        for (iSym = 0; iSym < numCtrlSymbols; iSym++)
        {
           for (indAntPort = 0; indAntPort < numBsTxAntPort; indAntPort++)
           {
                memset(pBsTxAntPortOut[indAntPort + iSym*numBsTxAntPort], 0, pIOParamsDlControl->stepsize_a);
           }
        }
    }
#endif

    /* PCFICH Process */
    {
        /* memory pointers */
        S16 *pCFIBuffer = pBsDlInitMemBuf->pCfiMemBuffer;
        S16 *pCfiREMapTable = pBsDlInitMemBuf->pCfiReMapBuffer;
        S16 pcfichScale = pIOParamsDlControl->controlIndicatorScaleFactor;
#if 0
        if ( computeOffset == 0 )
        {
            S16 offsetValue;
            computeOffsetToPCFICHBuffer ( pIOParamsDlControl->numCtrlSymbols, pIOParamsDlControl->numTxAntennas,
                ( pIOParamsDlControl->slotNumber>>1), &offsetValue);

            /* update PCFICH address */
            pCFIBuffer = pBsDlInitMemBuf->pCfiMemBuffer + offsetValue;
        }
#endif
        /* check if number of control symbols > 0 */
        StartSubTask(SUBTASKID_TX_DLCONTROL_MAIN_CFI_PROC);
        if ( numCtrlSymbols > 0 )
        {
            /* Run PCFICH Process */
            LteBsPhyPCFICHProc ( pCFIBuffer, pCfiREMapTable, numCtrlSymbols, numBsTxAntPort,
                                 subFrameNum, pcfichScale, computeOffset, pBsTxAntPortOut );
        }
        StopSubTask(SUBTASKID_TX_DLCONTROL_MAIN_CFI_PROC);
    }

    /* PHICH Process */
    {
        /* variables associated with PHICH process */
        S16 *pHIMemBuf = pBsDlInitMemBuf->pHIMemBuffer;
        S16 *pHiREMapTable = pBsDlInitMemBuf->pHIREMapTable;

        S16 numPHICHChannels = (S16) pIOParamsDlControl->number_of_phich_channels;
        S16 *pPhichHarqBits  = pIOParamsDlControl->PHICH_Bit1;
        S16 *pPhichSeqIndx   = pIOParamsDlControl->PHICH_SequenceIndex1;
        S16 *pPhichGrpNum    = pIOParamsDlControl->PHICH_Grp;
        S16 numPHICHGrp      = pIOParamsDlControl->N_PHICH_Grp;
        S16 phichScale       = pIOParamsDlControl->scalefactorPHICH;

#if 0
        if ( computeOffset == 0 )
        {
            S16 offsetValue;
            computeOffsetToPHICHBuffer ( pIOParamsDlControl->numTxAntennas, (pIOParamsDlControl->slotNumber>>1),
                                         &offsetValue );

            /* update PCFICH address */
            pHIMemBuf = pBsDlInitMemBuf->pHIMemBuffer + offsetValue;
        }
#endif

        StartSubTask(SUBTASKID_TX_DLCONTROL_MAIN_PHICH_PROC);
#if 0
        if ( numCtrlSymbols > 0 && numPHICHChannels > 0)
#else
        if ( numCtrlSymbols > 0)
#endif
        {
            /* Run PHICH Process */
            LteBsPhyPHICHProc ( numPHICHChannels, pPhichHarqBits, pPhichSeqIndx, pPhichGrpNum,
                                numPHICHGrp, pHIMemBuf, pHiREMapTable, numBsTxAntPort,
                                subFrameNum, computeOffset, &phichScale, pBsTxAntPortOut );
        }
        StopSubTask(SUBTASKID_TX_DLCONTROL_MAIN_PHICH_PROC);
    }

    /* PDCCH Process */
    {
        /* variables associated with PDCCH process */
        S16 *pPdcchPermuteTable = pBsDlInitMemBuf->pPDCCHPermuteTable;
        S16 *pPdcchREGLen       = pBsDlInitMemBuf->pPDCCHQuadrupletsLen;
        S16 *pPdcchREMapTable   = pBsDlInitMemBuf->pPDCCHReMapTable;
        S16 numPDCCHChannels    = (S16)pIOParamsDlControl->number_of_pdcch_channels;
        U8 *pPdcchBytes         = pIOParamsDlControl->startPDCCH;
        U32 transmissionMode    = pIOParamsDlControl->transmode;
        S16 pdcchScale          = pIOParamsDlControl->scalefactorPDCCH;

        StartSubTask(SUBTASKID_TX_DLCONTROL_MAIN_PDCCH_PROC);
        if ( numCtrlSymbols > 0 )
        {
            /* Run PDCCH process */
            LteBsPhyPDCCHProc ( numPDCCHChannels, pPdcchBytes, NumCCECtl, StartCCECtl,
                                numCtrlSymbols, transmissionMode, pPdcchPermuteTable,
                                pPdcchREMapTable, pPdcchREGLen, numBsTxAntPort,
                                &pdcchScale, pBsTxAntPortOut );
        }
        StopSubTask(SUBTASKID_TX_DLCONTROL_MAIN_PDCCH_PROC);
    }

    /* DL cell specific reference sequence generation */
    {
        /* Local variables */
        S16 *pDlRefSeqSymBuffer = pBsDlInitMemBuf->pDLCellSpecificRefSeq;
        S16 cellId = (S16)pIOParamsDlControl->Ncell_id;
        S16 symbolNum = 0; // for port 0 and 1
        S16 numDlRb = pIOParamsDlControl->Ndl_rb;
        S16 pilotScaleFactor = pIOParamsDlControl->pilotScaleFactor;

        StartSubTask(SUBTASKID_TX_DLCONTROL_MAIN_PILOT_PROC);
        if ( numCtrlSymbols > 0 )
        {

#if 0
        if ( computeOffset == 0 )
        {
            S16 offsetValue;
            computeOffsetToDLRefSeqBuffer ( pIOParamsDlControl->numTxAntennas, pIOParamsDlControl->slotNumber,
                                            0, pIOParamsDlControl->Ndl_rb, &offsetValue );

            /* update DL Ref sequence address */
            pDlRefSeqSymBuffer = pBsDlInitMemBuf->pDLCellSpecificRefSeq + offsetValue;
        }
#endif
            LteBsPhyDLRefSeqProc ( pDlRefSeqSymBuffer, cellId, symbolNum, numBsTxAntPort,
                                   slotNumber, numDlRb, pilotScaleFactor,
                                   computeOffset, pBsTxAntPortOut );

            /* if number of transmit antenna ports is 4 - generate sequence for
             * symbol 1*/
            if ( numBsTxAntPort > 2 )
            {
                symbolNum = symbolNum + 1;
                LteBsPhyDLRefSeqProc ( pDlRefSeqSymBuffer, cellId, 1, numBsTxAntPort,
                                       slotNumber, numDlRb, pilotScaleFactor,
                                       computeOffset, pBsTxAntPortOut );
            }
        }
        StopSubTask(SUBTASKID_TX_DLCONTROL_MAIN_PILOT_PROC);
    }

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief Subroutines for PDCCH FEC encoding. Should be reworked during further optimization
 *
 *  <1> Hardware Resource:
 *  ARM or Ceva
 *
 * \ingroup group_lte_tx_downlink_fec_encoder
 *
 **/
//-------------------------------------------------------------------------------------------

// CRC Calculation (same as in Mdmadrv.c)
U32 ProcCRC(U32 * pRemainder, U32 polynomial, U32 width, U8 message[], U32 nBytes, U32 nBits)
{
    U32 mask = 0, Byte, bit;
    U32 topbit = 1 << (width - 1);

    switch (width)
    {
        case 8:
            mask = 0xFF;
            break;

        case 12:
            mask = 0xFFF;
            break;

        case 16:
            mask = 0xFFFF;
            break;

        case 24:
            mask = 0xFFFFFF;
            break;

        case 32:
            mask = 0xFFFFFFFF;
            break;

        default:
            return 0;
    }

    /*
     * Perform modulo-2 division, a byte at a time.
     */
    for (Byte = 0; Byte < nBytes; ++Byte)
    {
        /*
         * Bring the next byte into the remainder.
         */
        *pRemainder ^= (message[Byte] << (width - 8));

        // mycode
        *pRemainder &= mask;

        /*
         * Perform modulo-2 division, a bit at a time.
         */
        if(Byte == nBytes-1)
        {
            for (bit = nBits; bit > 0; --bit)
            {
                /*
                 * Try to divide the current data bit.
                 */
                if (*pRemainder & topbit)
                {
                    *pRemainder = (((*pRemainder & mask)<< 1) ^ polynomial) & mask;
                }
                else
                {
                    *pRemainder = (((*pRemainder & mask) << 1)) & mask;
                }
            }
        }
        else
        {
            for (bit = 8; bit > 0; --bit)
            {
                /*
                 * Try to divide the current data bit.
                 */
                if (*pRemainder & topbit)
                {
                    *pRemainder = (((*pRemainder & mask)<< 1) ^ polynomial) & mask;
                }
                else
                {
                    *pRemainder = (((*pRemainder & mask) << 1)) & mask;
                }
            }
        }
    }

    /*
     * The final remainder is the CRC result.
     */
    return (*pRemainder) & mask;
}

//-------------------------------------------------------------------------------------------
/** @brief Modified LTE downlink transmit channel coding function for control information.
 *
 *  <1> Hardware Resource:
 *  ARM, FEC
 *
 *  @param     in      packed input data bit array organized as bytes.  For each
 *                     byte bit7(msb) is the first bit and bit0(lsb) is the last
 *                     bit.
 *  @param     out     output data bit array organized as 1 bit per byte (bit0 =
 *                     data bit).
 *  @param     nin     length of input array in bits.
 *  @param     scram   16-bit value used to scramble 16-bit CRC.  Bit0 is used
 to scramble the first CRC bit and bit15 is used to scramble
 the last CRC bit.
 *  @param     E       desired rate matching output length in bits.
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |in             |               |input        |nin            |pReal Array   |(0:8:0)   |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |out            |               |output       |E              |pReal Array   |(0:8:0)   |{0,1}          |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |nin            |               |input        |1              |Real          |(1:31:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |scram          |               |input        |1              |Real          |(0:16:0)  |full range     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |E              |               |input        |1              |Real          |(1:31:0)  |[1:maximum]    |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------------
 *  |Block Name          |bsTxDciChannelCoding           |
 *  ------------------------------------------------------
 *  |Input Stream(s)     |in                             |
 *  ------------------------------------------------------
 *  |Output Stream(s)    |out                            |
 *  ------------------------------------------------------
 *  |Config Parameter(s) |nin,scram,E                    |
 *  ------------------------------------------------------
 *  |Status Parameter(s) |none                           |
 *  ------------------------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * Function bs_tx_dci_channel_coding implements the channel coding function
 * for the downlink control information as defined in document 3GPP TS 36.212
 * Section 5.3.3.  Specifically, the following processing steps are performed:
 *
 *    - Unpack input data byte array into a data bit array (1 bit per byte).
 *    - Compute and append 16-bit CRC to data bit array.
 *    - Scramble CRC bit.
 *    - Encode data and CRC using 1/3 rate tail-biting convolutional encoder.
 *    - Perform rate matching (sub-block interleaving, bit collection and
 *      bit selection of convolutional encoder outputs.
 *
 *
 * <5> Cycle Count Formula:
 * TBC
 *
 *
 * <6> References:
 * [1] 3GPP TS 36.212 Section 5.3.3
 *
 *
 * \ingroup group_lte_tx_downlink_fec_encoder
 *
 *
 **/
//-------------------------------------------------------------------------------------------

U32 bsTxDciChannelCoding(U8 in[], U8 out[], S32 nin, U16 scram, S32 E)
{
    S32 nencbits, nintlbits;
    U8 *encin;
    U32 *encout;

    U32 crc32=0;
    U8* crc;
    U8 tmp8, mask;
    S32 ninbytes, nbits;

#ifdef CEVA_INTRINSICS
    U8 MaskCC[8] = { 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80 };
#endif

    // Compute length of CC encoder input array in bits (add 16 for CRC)
    nencbits = nin + 16;

    // Compute length of sub-block interleaver output arrays in bits
    nintlbits = (nencbits + 31) >> 5;
    nintlbits = nintlbits << 5;

    encin = (U8*) DataBuff_Bank2;

    // Allocate temporary space for CC encoder output array (shared)
    encout = (U32*) &encin[nencbits];

    // Create CC encoder input array by appending 16-bit CRC to input array
// Compute CRC for bytes on ARM
    crc32 = 0;
    crc = (U8*) &crc32;
    ninbytes = (nin + 7) >> 3;
    nbits = (ninbytes << 3) - nin;
    mask = MaskCC[nbits];
    in[ninbytes - 1] &= mask;
    ProcCRC((U32 *)&crc32, 0x1021, 16, in, ninbytes, 8-nbits);

// Scramble CRC16
    crc[0] ^= (scram & 0xff);
    crc[1] ^= ((scram >> 8) & 0xff);

// Attach CRC16 immediately after input bits

    if (nbits != 0)
    {
        tmp8 = crc[1] >> (8 - nbits);
        in[ninbytes - 1] |= tmp8;
        in[ninbytes] = crc[1] << nbits;

        tmp8 = crc[0] >> (8 - nbits);
        in[ninbytes] |= tmp8;
        in[ninbytes + 1] = crc[0] << nbits;
    }
    else
    {
        in[ninbytes] = crc[1];
        in[ninbytes+1] = crc[0];
    }

    CC_Encoder(in, encout, nencbits, nbits, nintlbits);

    DCI_CC_SubBlockInterleaver(encout, out, nencbits, nintlbits, E);

    return ((E+7)>>3);
}

//-----------------------------------------------------------------------------
/** @brief FEC encoding function which calls encoding for each PDCCH channel.
 * NOTE: No error checking is done in this function
 *
 *  <1> Hardware Resource:
 *  ARM/CEVA
 *
 *  @param   pDlCntrl_Pars  pointer to common DL Control Task input structure
 *
 *  @return  O'k
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------
 *  |Name          |Direction|Length  |Type  |Data Format |Valid Range |Units|
 *  |              |         |        |      |            |            |     |
 *  --------------------------------------------------------------------------
  *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------
 *  |Block Name          |LtePhyControlSduEnc           |
 *  -----------------------------------------------------
 *  |Input Stream        |inputPDCCH                    |
 *  -----------------------------------------------------
 *  |Output Stream       |startPDCCH                    |
 *  -----------------------------------------------------
 *  |Config Parameters   | subFrameNumber, numPDCCH     |
 *  -----------------------------------------------------
 *  |Status Parameters   |none                          |
 *  -----------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *
 *
 *  <5> Cycle Count Formula:
 *
 *
 *
 *  <6> References:
 *
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-----------------------------------------------------------------------------

U32 LtePhyControlSduEnc(PIOParams_DlControl pDlCntrl_Pars)
{
    U32 i, j, outSize;
    U32 numBytes, E, nin, subframeNum;
    U8 *in, *inMsg, *out, *scr;

    PTXSDUREQ pTxSduReq;
    PDCIULSDUMSG pDci;

    U8 *inBuf, *outBuf, *scrSeq;
    U32 numPDCCH, numDCI0;
    U16 *startCCE, *numCCE, *RNTI;

    subframeNum = pDlCntrl_Pars->slotNumber >> 1;
    inBuf = pDlCntrl_Pars->inputPDCCH;
    outBuf = pDlCntrl_Pars->startPDCCH;
    numPDCCH = pDlCntrl_Pars->numPDCCH;
    numDCI0 = pDlCntrl_Pars->numDCI0;
    startCCE = (U16*) pDlCntrl_Pars->StartCCECtl;
    numCCE = (U16*) pDlCntrl_Pars->NumCCECtl;
    RNTI = pDlCntrl_Pars->pdcchRNTI;
    scrSeq = pDlCntrl_Pars->scrSeq;

    inMsg = inBuf;

    for(i=0; i<numPDCCH; i++)
    {
        pTxSduReq = (PTXSDUREQ) inMsg;
        numBytes = pTxSduReq->msgLen;
        in = (U8*) ((U8*) inMsg + sizeof(TXSDUREQ));
        nin = pTxSduReq->maxBitsperCw;

        E = numCCE[i] * 8;
        out = &outBuf[800]; //&outBuf[startCCE[i]];
        scr = &scrSeq[startCCE[i]];

        outSize = bsTxDciChannelCoding(in, out, nin, RNTI[i], E);


//  Scramble output
        for(j=0; j<outSize; j++)
        {
            out[j] ^= scr[j];
        }
        memcpy ( &outBuf[startCCE[i]], out, numCCE[i]);
        inMsg += RUP8B((U32) numBytes + 2 + sizeof (TXSDUREQ));
    }

    for(i=0; i<numDCI0; i++)
    {
        pDci = (PDCIULSDUMSG) inMsg;
        numBytes = pDci->msgLength;
        in = (U8*) ((U8*) pDci + sizeof(DCIULSDUMSG));
        nin = (S32) pDci->numBitsDciUL;

        E = numCCE[i+numPDCCH] * 8;
        out = &outBuf[800]; //&outBuf[startCCE[i+numPDCCH]];
        scr = &scrSeq[startCCE[i+numPDCCH]];

        outSize = bsTxDciChannelCoding(in, out, nin, pDci->rnti, E);

//  Scramble output
        for(j=0; j<outSize; j++)
        {
            out[j] ^= scr[j];
        }
        memcpy ( &outBuf[startCCE[i+numPDCCH]], out, numCCE[i+numPDCCH]);
        inMsg += RUP8B((U32) numBytes + 2 + sizeof (DCIULSDUMSG));
    }
    return 0;
}


U32 bsTxBchChannelCoding(U8 in[], U8 out[], S32 nin, U16 nAnt, S32 E)
{
    S32 nencbits, nintlbits;
    U8 *encin;
    U8 *encout;
    U16 scram;
    U32 crc32=0;
    U8* crc;

    scram = 0x0000;
    if (nAnt == 2)
    {
        scram = 0xffff;
    }
    else if (nAnt == 4)
    {
        scram = 0xaaaa;
    }

    // Compute length of CC encoder input array in bits (add 16 for CRC)
    nencbits = nin * 8 + 16;

    // Compute length of sub-block interleaver output arrays in bits
    nintlbits = (nencbits + 31) >> 5;
    nintlbits = nintlbits << 5;

    encin = (U8*) DataBuff_Bank2;

    // Allocate temporary space for CC encoder output array (shared)
    encout = (U8*) &encin[nencbits];

    // Create CC encoder input array by appending 16-bit CRC to input array
// Compute CRC for bytes on ARM
    crc32 = 0;
    crc = (U8*) &crc32;
    ProcCRC((U32 *)&crc32, 0x1021, 16, in, nin, 8);

// Scramble CRC16
    crc[0] ^= (scram & 0xff);
    crc[1] ^= ((scram >> 8) & 0xff);

// Attach CRC16 immediately after input bits
    in[nin] = crc[1];
    in[nin+1] = crc[0];

    CC_Encoder(in, (U32*)encout, nencbits, 0, nintlbits);

    DCI_CC_SubBlockInterleaver((U32*)encout, out, nencbits, nintlbits, E);

    return 0;
}
