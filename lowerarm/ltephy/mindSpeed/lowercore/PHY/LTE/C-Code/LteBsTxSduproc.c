//-------------------------------------------------------------------------------------------
/** @file LteBsTxSduproc.c
 *
 * @brief This file implements the TXSDU  processing for the LTE BS
 * @author Mindspeed Technologies
 * @version $Revision: 1.99 $
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
#include "basic_op_cc.h"

//---------------------------------------------------------------------------------------------------------
/** @brief Implements the TXSDU processing for the LTE BS
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Defines the particular PHY that is going to perform the
 *                     processing of the current TXSDU
 *  @param ptr
 *                     Pointer to a TXSDU message payload that contains the
 *                     data stream to be coded in the next subframe indicated
 *                     in the TXSTART request command received prior to the
 *                     first TXSDU request
 *  @param len         Length in bytes of the TxVector
 *
 *  @return status     A value of zero indicate success a value of 1 indicates
 *                     failure (We can expand to indicate the specific failure
 *                     reason)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type               |Data Format |Valid Range   |Units |
 *  |            |standard |		      |        |                   |            |              |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |phyInstance |         |Input         |1       |unsigned int       |(1:31:0)    |[0,2]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |pPhy        |         |pInput        |1       |pStruct(LTEPHYDESC)|(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |ptr         |         |pInput        |1       |punsigned int      |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |chanId      |         |Input         |1       |unsigned int       |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |ptrSDU      |         |Input         |1       |pStruct(PTXSDUREQ) |(1:31:0)    |(0,TBD]       |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |cwId        |         |Input         |1       |unsigned int       |(1:31:0)    | [0,1]        |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |status      |         |Output        |1       |unsigned int       |(1:31:0)    | [0,1]        |      |
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteBsTxSduProcessing        |
 *  ---------------------------------------------------
 *  |Input Streams       |pPhy,ptr,chanId             |
 *  ---------------------------------------------------
 *  |Output Streams      |pChan->pFecTxOut[cwId]      |
 *  ---------------------------------------------------
 *  |Config Parameters   |phyInstance,ptrSDU          |
 *  |                    |                            |
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function implements the LTE BS TXSDU processing according to the
 *  channel configuration contained LTEPHYDESC structure for the current PHY
 *  instance<BR>
 *  This message needs to be embedded into the Mindspeed generic message
 *  format described in MSA-81479 using MessageType 0x0005 (Indication Message)
 *  and MessageID 0x7000 Foreign API Message
 *
 * <5> Cycle Count Formula:
 * DOXYGEN_TO_DO
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.5.0 (2009-12)
 *
 *  \ingroup group_lte_api
 *
 **/
//----------------------------------------------------------------------------------------------------------
U32 LteBsTxSduProcessing(U32 phyInstance, U32* ptr, U32 chanId, PTXSDUREQ ptrSDU, U32 cwId)
{
    U32 status = 0;
    U32 numBytes = ptrSDU->msgLen, ScrInputOffset = 0;
    PLTEPHYDESC pPhy = getPhyCtxApiProc(phyInstance);
    PBS_TX pBsTx = pPhy->pBsTx;
    PBS pBs = pPhy->pBs;
    PDLTXINPUTPARAMS pTxInputParams;
    S32 numBitsOut;
    S16 NL, rV, modType;
    U8  *pScrInPtr;
#ifdef DEBUG_PRINT
    FILE *f_pbch;
#endif
    U32 TotalMsgLen, numChan, cinit = (pBsTx->subframeNumber << 9) + pBs->Ncell_id;
    S32 i, chanIdx;
    S32 NIR, fecInOutBits;

    pTxInputParams = pBsTx->pTxInputParams;
    numChan = pTxInputParams->numChan;

    if (pTxInputParams->txChanIdMappingTable[chanId] != -1)             // This has been re-init in TxVectorProc to -1
    {
        chanIdx = pTxInputParams->txChanIdMappingTable[chanId];

        for(i = 0; i < (S32)chanIdx; i++)
        {
            if ((pTxInputParams->channelType[i] == PDSCH) && (i != 0))
            {
                if (cwId == 0)
                    ScrInputOffset += (pTxInputParams->G0[i-1] + 7) >> 3;
                else
                    ScrInputOffset += (pTxInputParams->G1[i-1] + 7) >> 3;
            }
        }

        // Invoke the FEC coder
        if (pTxInputParams->channelType[chanIdx] == PDSCH)
        {
            //This is a subtle understanding of how NL supposed to be set.
            //Originaly it what thought that it is 1 if number of layers is 1 and 2 otherwise
            //In reality, it should be look at the number of layers per codeword
            NL = 2;
            if( ( pTxInputParams->numCodeWords[chanIdx] == pTxInputParams->numTxLayers[chanIdx]) ||
                ( pTxInputParams->numTxLayers[chanIdx] == 3 && cwId == 0) )
                NL = 1;

            if (numBytes > 0)
            {
                // i = UE category (1 - 5)
                i = pTxInputParams->ueCategory[chanIdx];
                // If specified UE category out of range, then use 5
                if( (i < 1) || (i > 5) )
                    i = 5;

                // NIR = Nsoft/8 for UE category
                NIR = UE_NIR_Table[i-1];

                // If UE is configured for modes 3 or 4, then NIR = NIR/2
                if( (pTxInputParams->transmissionMode[chanIdx] == 3) || (pTxInputParams->transmissionMode[chanIdx] == 4) )
                {
                    NIR = NIR >> 1;
                }

                pScrInPtr = &gScrInBufs[0][cwId][ScrInputOffset];
                if (cwId == 0)
                {
                    fecInOutBits = pTxInputParams->G0[chanIdx];
                    rV = pTxInputParams->rV0[chanIdx];
                    modType = pTxInputParams->modType0[chanIdx];
                }
                else if (cwId == 1)
                {
                    fecInOutBits = pTxInputParams->G1[chanIdx];
                    rV = pTxInputParams->rV1[chanIdx];
                    modType = pTxInputParams->modType1[chanIdx];
                }
                else
                {
                    printf("cwId must be equal to 0 or 1..  cwId = %d\n", cwId);
                    return FAILURE;
                }
                BsTxChannelCoder(chanIdx, cwId, (U8*) ptr,
                           (S32) numBytes, pScrInPtr,
                           (S32) fecInOutBits, (S16) rV, (S16) NL,
                           (S16) (2 * modType),
                           (S16) pTxInputParams->nRNTI[chanIdx],
                           (S16) pTxInputParams->channelType[chanIdx],
                           (S16) (pBsTx->subframeNumber << 1),
                           (S16) pBs->Ncell_id, (U32) pTxInputParams->numCtrlChan,
                                 gScrOutBufs[0], gScrIndex[0], g_totalsizescr[0], NIR);
                g_scrsize[0][cwId] = g_totalsizescr[0][cwId];
            }
        }
        else if (pTxInputParams->channelType[chanIdx] == PDCCH)
        {
            //PDCCH FEC now done in the Ceva.  Just need to copy input into PDCCH Array
            TotalMsgLen = (U32)(numBytes + sizeof (TXSDUREQ));
            memcpy(pBsTx->pDlCntrl_TxInPars->ptrInputPDCCH, (void*)ptrSDU, TotalMsgLen);
            pBsTx->pDlCntrl_TxInPars->ptrInputPDCCH[TotalMsgLen] = 0;
            pBsTx->pDlCntrl_TxInPars->ptrInputPDCCH += RUP8B((U32) numBytes + 2 + sizeof (TXSDUREQ));
        }
        else if (pTxInputParams->channelType[chanIdx] == PBCH)
        {
            //If the Phy Maintains PBCH internally, don't have to run FEC here
            if (((pTxInputParams->txFrameNumber & 0x3) == 0) && (!pTxInputParams->subframeNumber) && (!gPhyInternalBch))
            {
                U8* scrOut_local;

                numBitsOut = 240; // bytes

                bs_tx_broadcast_channel_coding((U8*) ptr, pTxInputParams->startPBCH, (S32) numBytes,
                                               pTxInputParams->Ntx_antennas, numBitsOut * 8);

                printf("numBytes %d\n", numBytes);
                printf("Out: %d %d %d %d %d ... %d %d %d %d %d\n",
                        pTxInputParams->startPBCH[0],
                        pTxInputParams->startPBCH[1],
                        pTxInputParams->startPBCH[2],
                        pTxInputParams->startPBCH[3],
                        pTxInputParams->startPBCH[4],
                        pTxInputParams->startPBCH[235],
                        pTxInputParams->startPBCH[236],
                        pTxInputParams->startPBCH[237],
                        pTxInputParams->startPBCH[238],
                        pTxInputParams->startPBCH[239]);

//                bsTxBchChannelCoding((U8*) ptr, pTxInputParams->startPBCH, numBytes,
//                                                pTxInputParams->Ntx_antennas, numBitsOut * 8);

#ifdef DEBUG_PRINT
                {
                    U8 *pb;

                    pb = (U8*) ptr;

                    fprintf(f_data_mod0_b0,"%%///////////////////////////////////////////\n");
                    fprintf(f_data_mod0_b0,"%%ChanId = %d, FrameNumber = %d, SubFrameNum = %d\n",
                        chanIdx, pPhy->txFrameNumber , pBsTx->subframeNumber);
                    fprintf(f_data_mod0_b0,"%%///////////////////////////////////////////\n");
                    for(i=0; i< (S32)numBytes; i++)
                    {
                        fprintf(f_data_mod0_b0,"%d\n", pb[i]);
                    }
                }
#endif

                scrambler_byte(pBs->Ncell_id, pTxInputParams->startPBCH, pTxInputParams->startPBCH, numBitsOut);

#ifdef DEBUG_PRINT
                f_pbch= fopen("pbch_scr_out","w");
                for (i=0; i < numBitsOut; i++)
                    fprintf(f_pbch,"%d\n",*(pTxInputParams->startPBCH +i));
                fclose(f_pbch);
#endif

                pTxInputParams->ptrPBCH = pTxInputParams->startPBCH;

                gScrIndex[0][0][chanIdx] = g_totalsizescr[0][0];

                scrOut_local = &gScrOutBufs[0][0][g_totalsizescr[0][0]];

                //Append a channel at the end of the TXVECTOR with the saved PBCH info
                memcpy(scrOut_local, pTxInputParams->ptrPBCH, PBCHDATASIZE / 8);

                g_totalsizescr[0][0] += RUP32B(PBCHDATASIZE/8);
                g_scrsize[0][0]      += RUP32B(PBCHDATASIZE/8);

                pTxInputParams->ptrPBCH += PBCHDATASIZE / 8;
            }
        }
    }
    else
    {
        printf("Some Errror: %d %d %d\n", pTxInputParams->txChanIdMappingTable[chanId], chanId, ptrSDU->channelType);
        while(1);
        status = FAILURE;
    }

    return status;
}

//--------------------------------------------------------------------------------------------------------
/** @brief Implements the TXSDU processing for the LTE BS HI Channel
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Defines the particular PHY that is going to perform the
 *                     processing of the current TXSDU for an HI channel
 *  @param ptr
 *                     Pointer to a TXSDU message that contains the
 *                     data and control info to be coded in the next subframe indicated
 *                     in the TXSTART request command received prior to the
 *                     first TXSDU request
 *  @param len         Length in bytes of the TxSduReq
 *
 *  @return status     A value of zero indicate success a value of 1 indicates
 *                     failure (We can expand to indicate the specific failure
 *                     reason)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type               |Data Format |Valid Range   |Units |
 *  |            |standard |		      |        |                   |            |              |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |phyInstance |         |Input         |1       |unsigned int       |(1:31:0)    |[0,2]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |pPhy        |         |pInput        |1       |pStruct(LTEPHYDESC)|(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |ptr         |         |pInput        |1       |pStruct(TXSDUREQ)  |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |chanId      |         |Input         |1       |unsigned int       |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |numBytes    |         |Input         |1       |unsigned int       |(1:31:0)    |(0,TBD]       |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |cwId        |         |Input         |1       |unsigned int       |(1:31:0)    | [0,1]        |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |status      |         |Output        |1       |unsigned int       |(1:31:0)    | [0,1]        |      |
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteBsTxHiSduProcessing      |
 *  ---------------------------------------------------
 *  |Input Streams       |pPhy,ptr,chanId             |
 *  ---------------------------------------------------
 *  |Output Streams      |pChan->pFecTxOut[cwId]      |
 *  ---------------------------------------------------
 *  |Config Parameters   |phyInstance,numBytes,       |
 *  |                    |                            |
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function implements the LTE BS TXSDU processing according to the
 *  channel configuration contained LTEPHYDESC structure for the current PHY
 *  instance<BR>
 *  This message needs to be embedded into the Mindspeed generic message
 *  format described in MSA-81479 using MessageType 0x0005 (Indication Message)
 *  and MessageID 0x7000 Foreign API Message
 *
 * <5> Cycle Count Formula:
 * DOXYGEN_TO_DO
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.5.0 (2009-12)
 *
 *  \ingroup group_lte_api
 *
 **/
//---------------------------------------------------------------------------------------------------------
U32 LteBsTxHiSduProcessing(U32 phyInstance, PTXSDUREQ ptr, U32 chanId, U32 numBytes, U32 cwId)
{
    U32 status = 0, numCtrlChan;
    PLTEPHYDESC pPhy = getPhyCtxApiProc(phyInstance);
    PBS_TX pBsTx = pPhy->pBsTx;
    PIOParams_DlControl pDlCntrl_Pars;
    S32 ChanId_Found = 0, chanIdx;

    pDlCntrl_Pars = pBsTx->pDlCntrl_TxInPars;

    numCtrlChan = pDlCntrl_Pars->number_of_pdcch_channels + pDlCntrl_Pars->number_of_phich_channels;
    if (chanId < numCtrlChan)
    {
        ChanId_Found = 1;
        chanIdx = chanId;
    }

    if (ChanId_Found)
    {
        // Update subframe channel array and index for subframe processing
        pDlCntrl_Pars->PHICH_Grp[chanId]  = ptr->phichGrpNumber;
        pDlCntrl_Pars->PHICH_Bit1[chanId] = ptr->nackAck;
        pDlCntrl_Pars->PHICH_SequenceIndex1[chanId] = ptr->phichSeqIndex;
    }
    else
    {
        status = FAILURE;
    }

    return status;
}

//---------------------------------------------------------------------------------------------------------
/** @brief Implements the TXSDU processing for the LTE BS Dci messages received in PHY_TXDCIULSDU_REQ
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Defines the particular PHY that is going to perform the
 *                     processing of the current TXSDU
 *  @param ptr
 *                     Pointer to a TXSDU message payload that contains the
 *                     data stream to be coded in the next subframe indicated
 *                     in the TXSTART request command received prior to the
 *                     first TXSDU request
 *  @param len         Length in bytes of the TxVector
 *
 *  @return status     A value of zero indicate success a value of 1 indicates
 *                     failure (We can expand to indicate the specific failure
 *                     reason)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type                  |Data Format |Valid Range   |Units |
 *  |            |standard |		      |        |                      |            |              |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |phyInstance |         |Input         |1       |unsigned int          |(1:31:0)    |[0,2]         |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |chanId      |         |Input         |1       |unsigned int          |(1:31:0)    |(0,2^32)      |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |numBytes    |         |Input         |1       |unsigned int          |(1:31:0)    |(0,2^32)      |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |pDci        |         |Input         |1       |pStruct(PDCIULSDUMSG) |(1:31:0)    |(0,TBD]       |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |status      |         |Output        |1       |unsigned int          |(1:31:0)    | [0,1]        |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteBsTxDciSduReqProcessing  |
 *  ---------------------------------------------------
 *  |Input Streams       |pDci,chanId                 |
 *  ---------------------------------------------------
 *  |Output Streams      |                            |
 *  ---------------------------------------------------
 *  |Config Parameters   |phyInstance, chanId,        |
 *  |                    |numBytes                    |
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function implements the LTE BS TXSDU processing according to the
 *  channel configuration contained LTEPHYDESC structure for the current PHY
 *  instance<BR>
 *  This message needs to be embedded into the Mindspeed generic message
 *  format described in MSA-81479 using MessageType 0x0005 (Indication Message)
 *  and MessageID 0x7000 Foreign API Message
 *
 * <5> Cycle Count Formula:
 * DOXYGEN_TO_DO
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.5.0 (2009-12)
 *
 *  \ingroup group_lte_api
 *
 **/
//----------------------------------------------------------------------------------------------------------
U32 LteBsTxDciSduReqProcessing(U32 phyInstance, PDCIULSDUMSG pDci, U32 chanId, U16 numBytes)
{
    PLTEPHYDESC pPhy = getPhyCtxApiProc(phyInstance);
    PBS_TX pBsTx = pPhy->pBsTx;
    PIOParams_DlControl pDlCntrl_Pars;
    S16 temp16, PDCCH_ScaleFactor;
    U16 transmode;
    U32 TotalMsgLen;

    // Update DL Control with DCI channels
    pDlCntrl_Pars = pBsTx->pDlCntrl_TxInPars;

    //transmode and scale factor is the same for all PDCCH
    temp16 = add((S16)pDlCntrl_Pars->pilotPowerdB_EPRE , (S16)pDci->txPowerControl);
    transmode = pDci->transmissionMode;
    PDCCH_ScaleFactor = Convert_dB_Linear(temp16);
    pDlCntrl_Pars->NumCCECtl[chanId]   = (pDci->numCCE*72) >> 3; // length in Bytes
    pDlCntrl_Pars->StartCCECtl[chanId] = (pDci->startCCE*72) >> 3; //start location in Bytes

    //New phy_bs_tx_Control structure
    pDlCntrl_Pars->number_of_pdcch_channels += 1;
    pDlCntrl_Pars->scalefactorPDCCH = PDCCH_ScaleFactor;
    pDlCntrl_Pars->transmode = transmode;

    pDlCntrl_Pars->pdcchRNTI[chanId] = pDci->rnti;

    //PDCCH FEC now done in the Ceva.  Just need to copy input into PDCCH Array
    TotalMsgLen = (U32)(numBytes + sizeof (DCIULSDUMSG));
    memcpy(pBsTx->pDlCntrl_TxInPars->ptrInputPDCCH, (void*)pDci, TotalMsgLen);
    pBsTx->pDlCntrl_TxInPars->ptrInputPDCCH[TotalMsgLen] = 0;
    pBsTx->pDlCntrl_TxInPars->ptrInputPDCCH += RUP8B((U32) numBytes + 2 + sizeof (TXSDUREQ));

    return 0;
}

//--------------------------------------------------------------------------------------------------------
/** @brief Implements the TXSDU processing for the LTE BS HI Channel received in PHY_TXHISDU_REQ
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Defines the particular PHY that is going to perform the
 *                     processing of the current TXSDU for an HI channel
 *  @param ptr
 *                     Pointer to a TXSDU message that contains the
 *                     data and control info to be coded in the next subframe indicated
 *                     in the TXSTART request command received prior to the
 *                     first TXSDU request
 *  @param len         Length in bytes of the TxSduReq
 *
 *  @return status     A value of zero indicate success a value of 1 indicates
 *                     failure (We can expand to indicate the specific failure
 *                     reason)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type                    |Data Format |Valid Range   |Units |
 *  |            |standard |		      |        |                        |            |              |      |
 *  ------------------------------------------------------------------------------------------------------------
 *  |phyInstance |         |Input         |1       |unsigned int            |(1:31:0)    |[0,2]         |      |
 *  ------------------------------------------------------------------------------------------------------------
 *  |ptr         |         |pInput        |1       |pStruct(HIINFOMSGDESC)  |(1:31:0)    |(0,2^32)      |      |
 *  ------------------------------------------------------------------------------------------------------------
 *  |chanId      |         |Input         |1       |unsigned int            |(1:31:0)    |(0,2^32)      |      |
 *  ------------------------------------------------------------------------------------------------------------
 *  |status      |         |Output        |1       |unsigned int            |(1:31:0)    | [0,1]        |      |
 *  ------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteBsTxHiSduReqProcessing   |
 *  ---------------------------------------------------
 *  |Input Streams       |ptr                         |
 *  ---------------------------------------------------
 *  |Output Streams      |                            |
 *  ---------------------------------------------------
 *  |Config Parameters   |phyInstance, chanId         |
 *  |                    |                            |
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function implements the LTE BS TXSDU processing according to the
 *  channel configuration contained LTEPHYDESC structure for the current PHY
 *  instance<BR>
 *  This message needs to be embedded into the Mindspeed generic message
 *  format described in MSA-81479 using MessageType 0x0005 (Indication Message)
 *  and MessageID 0x7000 Foreign API Message
 *
 * <5> Cycle Count Formula:
 * DOXYGEN_TO_DO
 *
 * <6> References:
 * [1] 3GPP TS 36.212 V8.5.0 (2009-12)
 *
 *  \ingroup group_lte_api
 *
 **/
//---------------------------------------------------------------------------------------------------------
U32 LteBsTxHiSduReqProcessing(U32 phyInstance, PHIINFOMSGDESC ptr, U32 chanId)
{
    U32 status = 0;
    PLTEPHYDESC pPhy = getPhyCtxApiProc(phyInstance);
    PBS_TX pBsTx = pPhy->pBsTx;
    PIOParams_DlControl pDlCntrl_Pars;
    S16 PHICH_ScaleFactor, temp16;
    pDlCntrl_Pars = pBsTx->pDlCntrl_TxInPars;

    temp16 = add((S16)pDlCntrl_Pars->pilotPowerdB_EPRE , (S16)ptr->txPowerControl);
    PHICH_ScaleFactor = Convert_dB_Linear(temp16);

    // Update subframe channel array and index for subframe processing
    pDlCntrl_Pars->PHICH_Grp[chanId]  = ptr->phichGroupNumber;
    pDlCntrl_Pars->PHICH_Bit1[chanId] = ptr->nackAck;
    pDlCntrl_Pars->PHICH_SequenceIndex1[chanId] = ptr->phichSequenceIndex;
    pDlCntrl_Pars->scalefactorPHICH = PHICH_ScaleFactor;

    return status;
}
