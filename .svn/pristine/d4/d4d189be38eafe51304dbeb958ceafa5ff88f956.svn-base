//-------------------------------------------------------------------------------------------
/** @file LteBsTxvectorproc.c
 *
 * @brief This file implements the txVector processing for the LTE BS
 * @author Mindspeed Technologies
 * @version $Revision: 1.83 $
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
#include "typedef.h"
#include "ltephydef.h"
#include "ext_phy.h"
#include "papr_rcf.h"
#include "gputil_fx.h"
#include "basic_op_cc.h"

#else

#include "ltephy.h"    //This includes typedef.h
#include "ext_phy.h"
#include <basic_op.h>

#endif

#include "prototypes.h"

//-------------------------------------------------------------------------------------------
/** @brief Implements the txVector processing for PDSCH and PBCH the LTE BS
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param pDlSubFrDesc - Pointer toDLSUBFRDESC structure that contains the
 *                     txVector information required to execute the LTE BS Tx
 *                     subFrame.
 *  @param pTxInputParams - Pointer to a internal PHY structure which contains all of
 *                     the information stored in txVector.  Structure will be passed to
 *                     FEC and Ceva functions.
 *
 *  @return status     A value of zero indicate success a value of 1 indicates
 *                     failure (We can expand to indicate the specific failure
 *                     reason)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction     |Length  |Type                      |Data Format |Valid Range |Units  |
 *  |              |Standard  |              |        |                          |            |            |       |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |pDlSubFrDesc  |          |pInput        |1       |pStruct(PDLSUBFRDESC)     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |pTxInputParams|          |pOutput       |1       |pStruct(PDLTXINPUTPARAMS) |            |            |       |
 *  -----------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteBsPhyTxVectorProcData    |
 *  ---------------------------------------------------
 *  |Input Streams       |                            |
 *  ---------------------------------------------------
 *  |Output Streams      |                            |
 *  ---------------------------------------------------
 *  |Config Parameters   |pDlSubFrDesc, pTxInputParams|
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function implements the LTE BS PHY txVector processing according to
 *  the information contained in the DLSUBFRDESC structure that the second
 *  parameter indicates and for the phyInstance stated in the first parameter.
 *  Once the PHY_TXSTART completes a PHY_TXSTART.indication message is issued.
 *  This message needs to be embedded into the Mindspeed generic message
 *  format described in MSA-81479 using MessageType 0x0005 (Indication Message)
 *  and MessageID 0x7000 Foreign API Message
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * MSA-81613
 *
 * \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
U32 LteBsPhyTxVectorProcData(PDLSUBFRDESC pDlSubFrDesc, PDLTXINPUTPARAMS pTxInputParams, U32 isInternalBch)
{
    PDLCHANDESC pInt = NULL, pDlChanDescBch = NULL;
    PDLCMNTXPWRCTL pdlcmnTxPwrCtl;

    U32 i, j, k, numPhich, status = 0, bchChanNumSave = 0;
    S16 tempVar, phyChanScale_factor;
    U32 chanID, chanType;

    S16 pilotPowerdB_EPRE = 0;              //pilotPower;
    S16 primarySyncPowerdB_RS = 0;          //primarySyncPower;
    S16 secondarySyncPowerdB_RS = 0;        //secondarySyncPower;

    S16 NgTable[4] = { 1365, 4096, 8192, 16384 }; //Q13

    if (pDlSubFrDesc == NULL)
        return FAILURE;

    if (pDlSubFrDesc->offsetPowerCtrl != 0)
    {
        pdlcmnTxPwrCtl                          = (PDLCMNTXPWRCTL) ((U32) pDlSubFrDesc + pDlSubFrDesc->offsetPowerCtrl);
        pilotPowerdB_EPRE                       = pdlcmnTxPwrCtl->pilotPower;
        primarySyncPowerdB_RS                   = pdlcmnTxPwrCtl->psyncPower;
        secondarySyncPowerdB_RS                 = pdlcmnTxPwrCtl->ssyncPower;

        pTxInputParams->PoSpilotScaleFactor     = pdlcmnTxPwrCtl->prsRa;
        pTxInputParams->PoSpilotScaleFactor     = 23143;
    }

    // Mapping table for chanId that comes in TxVector with Phy Chan IDs. This will be used later for TXSDU Processing. Initialize to -1
    for (i = 0; i < MAXCHSUBFRAME; i++)
        pTxInputParams->txChanIdMappingTable[i] = -1;

    pTxInputParams->pilotScaleFactor            = Convert_dB_Linear(pilotPowerdB_EPRE);
    tempVar                                     = add(pilotPowerdB_EPRE, primarySyncPowerdB_RS);
    pTxInputParams->primarySyncScaleFactor      = Convert_dB_Linear(tempVar);
    tempVar                                     = add(pilotPowerdB_EPRE, secondarySyncPowerdB_RS);
    pTxInputParams->secondarySyncScaleFactor    = Convert_dB_Linear(tempVar);

    ///////////////////////////////////////////////////////
    // Tx Input Structure
    ///////////////////////////////////////////////////////
    numPhich                        = pDlSubFrDesc->numberOfPhichChannels;
    pTxInputParams->txFrameNumber   = pDlSubFrDesc->frameNumber;
    pTxInputParams->subframeNumber  = pDlSubFrDesc->subframeNumber;
    pTxInputParams->numCtrlSymbols  = pDlSubFrDesc->numCtrlSymbols;
    pTxInputParams->numPhichChan    = numPhich;
    pTxInputParams->numCtrlChan     = pDlSubFrDesc->numCtrlChannels;
    pTxInputParams->numChan         = pDlSubFrDesc->numberofChannelDescriptors;
    pTxInputParams->slotNumber      = 2*pTxInputParams->subframeNumber;
    pTxInputParams->Ng              = NgTable[pDlSubFrDesc->phichResource];

    //Place Holder for PRS and Distributed VRB till API is finalized
    pTxInputParams->PRS_EnableFlag          = 0;
    pTxInputParams->Ndl_rbPrs               = 25;
    pTxInputParams->DistributedExistFlag    = 0;

    pTxInputParams->numRE_Prs       = 0;
    if(pTxInputParams->PRS_EnableFlag)
       pTxInputParams->numRE_Prs    = ComputePRSAssignment(pTxInputParams);

    for (i = (U32) pDlSubFrDesc->numCtrlChannels, k = (U32) pDlSubFrDesc->numCtrlChannels; i < (U32) pDlSubFrDesc->numberofChannelDescriptors; i++)
    {
        pInt     = &pDlSubFrDesc->dlCh[i - pDlSubFrDesc->numCtrlChannels];
        chanID   = pInt->channelId;
        chanType = pInt->channelType;

        // Remove BCH Processing from the chain since it is a special type of channel which will be placed at the end.
        // This is done because in frame numbers that are not multiple of 4, we need to save these off
        if (chanType == PBCH)
        {
            pTxInputParams->numChan--;
            bchChanNumSave = i;
            pDlChanDescBch = pInt;
            continue;
        }

        // Save this off for the TxSdus
        pTxInputParams->txChanIdMappingTable[chanID] = (S16)k;

        if (chanType == PDSCH)
        {
            tempVar             = add(pilotPowerdB_EPRE, (S16) pInt->txpowerControl);
            if(pTxInputParams->Ntx_antennas > 1 && chanType == PDSCH)
                tempVar         = add(tempVar, LTE_POWER_DB_BIAS);
            phyChanScale_factor = Convert_dB_Linear(tempVar);

            //////////////////////////////////////////////////////////////////////
            //Using new Tx Structure here
            //////////////////////////////////////////////////////////////////////
            pTxInputParams->chanId[k]               = (U16)k;
            pTxInputParams->channelType[k]          = (U16)pInt->channelType;
            pTxInputParams->transmissionMode[k]     = pInt->transmissionMode;
            pTxInputParams->phyChanScale_factor[k]  = phyChanScale_factor;
            pTxInputParams->numTxLayers[k]          = pInt->nLayers;
            pTxInputParams->numCodeWords[k]         = pInt->numCodeWords;
            pTxInputParams->cddType[k]              = pInt->dlPrecoderInfo.cddType;
            pTxInputParams->codeBookIdx[k]          = pInt->dlPrecoderInfo.codeBookIdx;
            pTxInputParams->nRNTI[k]                = pInt->scrInfo.scrinitValueinput;
            pTxInputParams->ueCategory[k]           = pInt->ueCategory;

            pTxInputParams->modType0[k]             = pInt->subChInfo[0].modulationType;
            pTxInputParams->modType1[k]             = pInt->subChInfo[1].modulationType;
            pTxInputParams->Qm0[k]                  = 2 * pInt->subChInfo[0].modulationType;
            pTxInputParams->Qm1[k]                  = 2 * pInt->subChInfo[1].modulationType;
            pTxInputParams->rV0[k]                  = pInt->subChInfo[0].rV;
            pTxInputParams->rV1[k]                  = pInt->subChInfo[1].rV;
            pTxInputParams->mcsType0[k]             = pInt->subChInfo[0].mcsType;
            pTxInputParams->mcsType1[k]             = pInt->subChInfo[1].mcsType;

            pTxInputParams->DistributedFlag[k] = 0;
            if(pTxInputParams->DistributedFlag[k])
            {
                pTxInputParams->DistributedExistFlag = 1;
            }
            pTxInputParams->NgapFlag[k] = 0;

            pTxInputParams->numberofEntries[k] = pInt->mapInfo.numberofEntries;
            for(j = 0; j < (U32)pInt->mapInfo.numberofEntries; j++)
            {
                pTxInputParams->reselmInfo[k][j].startRes = pInt->mapInfo.reselmInfo[j].startRes;
                pTxInputParams->reselmInfo[k][j].numRes   = pInt->mapInfo.reselmInfo[j].numRes;
            }

            PdschMCSIndexProcedures(pTxInputParams, k);

            k++;
        }
    }

    // PBCH Processing
    if (!pTxInputParams->subframeNumber)                    // Subframe 0
    {
        if ((pTxInputParams->txFrameNumber & 0x3) == 0)     // Frame 0.
        {
            if (pDlChanDescBch)
            {
                i = MAXCHSUBFRAME - 1;

                tempVar             = add(pilotPowerdB_EPRE, (S16) pDlChanDescBch->txpowerControl);
                phyChanScale_factor = Convert_dB_Linear(tempVar);

                pTxInputParams->phyChanScale_factor[i]  = phyChanScale_factor;

                if (!isInternalBch)             // USe from API. Else already set in Init
                {
                    pTxInputParams->channelType[i]          = (U16)pDlChanDescBch->channelType;
                    pTxInputParams->transmissionMode[i]     = pDlChanDescBch->transmissionMode;
                    pTxInputParams->phyChanScale_factor[i]  = phyChanScale_factor;
                    pTxInputParams->numTxLayers[i]          = pDlChanDescBch->nLayers;
                    pTxInputParams->numCodeWords[i]         = pDlChanDescBch->numCodeWords;
                    pTxInputParams->cddType[i]              = pDlChanDescBch->dlPrecoderInfo.cddType;
                    pTxInputParams->codeBookIdx[i]          = pDlChanDescBch->dlPrecoderInfo.codeBookIdx;
                    pTxInputParams->nRNTI[i]                = pDlChanDescBch->scrInfo.scrinitValueinput;
                    pTxInputParams->ueCategory[i]           = pDlChanDescBch->ueCategory;

                    pTxInputParams->modType0[i]             = pDlChanDescBch->subChInfo[0].modulationType;
                    pTxInputParams->modType1[i]             = pDlChanDescBch->subChInfo[1].modulationType;
                    pTxInputParams->Qm0[i]                  = 2 * pDlChanDescBch->subChInfo[0].modulationType;
                    pTxInputParams->Qm1[i]                  = 2 * pDlChanDescBch->subChInfo[1].modulationType;
                    pTxInputParams->rV0[i]                  = pDlChanDescBch->subChInfo[0].rV;
                    pTxInputParams->rV1[i]                  = pDlChanDescBch->subChInfo[1].rV;
                    pTxInputParams->mcsType0[i]             = pInt->subChInfo[0].mcsType;
                    pTxInputParams->mcsType1[i]             = pInt->subChInfo[1].mcsType;
                }
            }
            else
            {
                bchChanNumSave = pTxInputParams->numChan;        // Future case
            }
        }
        else
        {
            bchChanNumSave = pTxInputParams->numChan;
        }

        // Always put BCH channel at the end of the PDSCH channels
        i = pTxInputParams->numChan;
        pTxInputParams->numChan++;

        // Save this off for the TxSdus
        pTxInputParams->txChanIdMappingTable[bchChanNumSave] = (S16)i;

        //////////////////////////////////////////////////////////////////////
        //Using new Tx Structure here
        //////////////////////////////////////////////////////////////////////
        pTxInputParams->chanId[i]                 = (U16)i;
        pTxInputParams->channelType[i]            = PBCH;
        pTxInputParams->transmissionMode[i]       = pTxInputParams->transmissionMode[MAXCHSUBFRAME-1];
        pTxInputParams->phyChanScale_factor[i]    = pTxInputParams->phyChanScale_factor[MAXCHSUBFRAME-1];
        pTxInputParams->numTxLayers[i]            = pTxInputParams->numTxLayers[MAXCHSUBFRAME-1];
        pTxInputParams->numCodeWords[i]           = pTxInputParams->numCodeWords[MAXCHSUBFRAME-1];
        pTxInputParams->cddType[i]                = pTxInputParams->cddType[MAXCHSUBFRAME-1];
        pTxInputParams->codeBookIdx[i]            = pTxInputParams->codeBookIdx[MAXCHSUBFRAME-1];
        pTxInputParams->nRNTI[i]                  = pTxInputParams->codeBookIdx[MAXCHSUBFRAME-1];
        pTxInputParams->ueCategory[i]             = pTxInputParams->codeBookIdx[MAXCHSUBFRAME-1];

        pTxInputParams->modType0[i]               = pTxInputParams->modType0[MAXCHSUBFRAME-1];
        pTxInputParams->modType1[i]               = pTxInputParams->modType1[MAXCHSUBFRAME-1];
        pTxInputParams->Qm0[i]                    = pTxInputParams->Qm0[MAXCHSUBFRAME-1];
        pTxInputParams->Qm1[i]                    = pTxInputParams->Qm1[MAXCHSUBFRAME-1];
        pTxInputParams->rV0[i]                    = pTxInputParams->rV0[MAXCHSUBFRAME-1];
        pTxInputParams->rV1[i]                    = pTxInputParams->rV1[MAXCHSUBFRAME-1];
        pTxInputParams->mcsType0[i]               = pTxInputParams->mcsType0[MAXCHSUBFRAME-1];
        pTxInputParams->mcsType1[i]               = pTxInputParams->mcsType0[MAXCHSUBFRAME-1];

        pTxInputParams->numberofEntries[i]        = 1;
        pTxInputParams->reselmInfo[i][0].startRes = pTxInputParams->reselmInfo[MAXCHSUBFRAME-1][0].startRes;
        pTxInputParams->reselmInfo[i][0].numRes   = pTxInputParams->reselmInfo[MAXCHSUBFRAME-1][0].numRes;
    }

#ifdef _WIN32
    g_scrsize[gCtxDlFec][0] = 0;
    g_scrsize[gCtxDlFec][1] = 0;
    g_totalsizescr[gCtxDlFec][0] = 0;
    g_totalsizescr[gCtxDlFec][1] = 0;
#endif

    return status;
}


//-------------------------------------------------------------------------------------------
/** @brief Implements the txVector processing for PDCCH the LTE BS
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param pDlSubFrDesc - Pointer toDLSUBFRDESC structure that contains the
 *                     txVector information required to execute the LTE BS Tx
 *                     subFrame.
 *
 *  @param pDlCntrl_Pars - Pointer to a internal PHY structure which contains all of
 *                     the information stored in txVector.  Structure will be passed to
 *                     Ceva functions which will do FEC and Modulation for all Control
 *                     donwlink channels.
 *
 *  @param pTxInputParams - Pointer to a internal PHY structure which contains all of
 *                     the information stored in txVector.  Structure will be passed to
 *                     FEC and Ceva functions.
 *
 *  @return status     A value of zero indicate success a value of 1 indicates
 *                     failure (We can expand to indicate the specific failure
 *                     reason)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction     |Length  |Type                         |Data Format |Valid Range |Units  |
 *  |              |Standard  |              |        |                             |            |            |       |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |pDlSubFrDesc  |          |pInput        |1       |pStruct(PDLSUBFRDESC)        |            |            |       |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |pDlCntrl_Pars |          |pOutput       |1       |pStruct(PIOParams_DlControl) |            |            |       |
 *  -----------------------------------------------------------------------------------------------------------------
 *  |pTxInputParams|          |pOutput       |1       |pStruct(PDLTXINPUTPARAMS)    |            |            |       |
 *  -----------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteBsPhyTxVectorProcControl |
 *  ---------------------------------------------------
 *  |Input Streams       |                            |
 *  ---------------------------------------------------
 *  |Output Streams      |                            |
 *  ---------------------------------------------------
 *  |Config Parameters   |pDlSubFrDesc, pTxInputParams|
 *  |                    |pDlCntrl_Pars               |
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function implements the LTE BS PHY txVector processing according to
 *  the information contained in the DLSUBFRDESC structure that the second
 *  parameter indicates and for the phyInstance stated in the first parameter.
 *  Once the PHY_TXSTART completes a PHY_TXSTART.indication message is issued.
 *  This message needs to be embedded into the Mindspeed generic message
 *  format described in MSA-81479 using MessageType 0x0005 (Indication Message)
 *  and MessageID 0x7000 Foreign API Message
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * MSA-81613
 *
 * \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
U32 LteBsPhyTxVectorProcControl(PDLSUBFRDESC pDlSubFrDesc, PIOParams_DlControl pDlCntrl_Pars, PDLTXINPUTPARAMS pTxInputParams)
{
    PDCICHANNELDESC     pInt1;
    PDLCMNTXPWRCTL      pDlcmnTxPwrCtl;
    U32 i, j, numPhich, status = 0;
    U16 numPDCCH = 0, chanID;
    S16 tempVar;
    U32 offsetDCIChannels;
    S16 phyChanScale_factor;
    S16 pilotPowerdB_EPRE = 0;          //pilotPower;
    S16 controlIndicatorPowerdB_RS = 0; //controlIndicatorPower;

    if (pDlSubFrDesc == NULL)
        return FAILURE;

    if (pDlSubFrDesc->offsetPowerCtrl != 0)
    {
        pDlcmnTxPwrCtl                  = (PDLCMNTXPWRCTL) ((U32) pDlSubFrDesc + pDlSubFrDesc->offsetPowerCtrl);
        pilotPowerdB_EPRE               = pDlcmnTxPwrCtl->pilotPower;
        controlIndicatorPowerdB_RS      = pDlcmnTxPwrCtl->ciPower;

        pDlCntrl_Pars->pilotPowerdB_EPRE           = pilotPowerdB_EPRE;
    }

    numPhich = pDlSubFrDesc->numberOfPhichChannels;

    pDlCntrl_Pars->number_of_phich_channels     = numPhich;
    pDlCntrl_Pars->numCtrlSymbols               = pDlSubFrDesc->numCtrlSymbols;
    pDlCntrl_Pars->pilotScaleFactor             = Convert_dB_Linear(pilotPowerdB_EPRE);
    tempVar                                     = add(pilotPowerdB_EPRE, controlIndicatorPowerdB_RS);
    pDlCntrl_Pars->controlIndicatorScaleFactor  = Convert_dB_Linear(tempVar);
    pDlCntrl_Pars->slotNumber                   = 2*pDlSubFrDesc->subframeNumber;

#if defined(EVM) || defined(RT_API_PROC)
    pDlCntrl_Pars->number_of_pdcch_channels = pDlSubFrDesc->numCtrlChannels;
#else
    pDlCntrl_Pars->number_of_pdcch_channels = pDlSubFrDesc->numCtrlChannels - numPhich;
#endif

    i = pDlCntrl_Pars->PHICH_Grp_Idx1;
    j = pDlSubFrDesc->phichResource;

    pDlCntrl_Pars->N_PHICH_Grp = TabN_PHICH_GRP[j * 6 + i];
    if (pDlSubFrDesc->numCtrlChannels == 0)
    {
        pDlCntrl_Pars->scalefactorPDCCH = 0;
        pDlCntrl_Pars->transmode = SINGLEANT;
    }

    ///////////////////////////////////////////////////////
    pDlCntrl_Pars->numDCI0 = 0;
    offsetDCIChannels = pDlSubFrDesc->offsetDCIChannels;
    if (offsetDCIChannels != 0)
    {
        pInt1 = (PDCICHANNELDESC) ((U32) pDlSubFrDesc + (U32) offsetDCIChannels);
        for (i = 0; i < (U32) pDlSubFrDesc->numCtrlChannels; i++)
        {
            chanID = pInt1->channelId;

            // Save this off for the TxSdus
            pTxInputParams->txChanIdMappingTable[chanID] = (S16)i;

            pTxInputParams->chanId[i] = (U16)i;
            tempVar = add(pilotPowerdB_EPRE, (S16) pInt1->txpowerControl);
            phyChanScale_factor = Convert_dB_Linear(tempVar);

            if (pInt1->channelType == PDCCH)
            {
                if (numPDCCH == 0)
                {
                    //All PDCCH have the same scaling and transmode
                    pDlCntrl_Pars->scalefactorPDCCH = phyChanScale_factor;
                    pDlCntrl_Pars->transmode = pInt1->transmissionMode;
                }
                numPDCCH += 1;
#if defined(EVM) || defined(RT_API_PROC)
                pDlCntrl_Pars->StartCCECtl[i] = (pInt1->startRes * 72) >> 3; //start location in Bytes
                pDlCntrl_Pars->NumCCECtl[i]   = (pInt1->numRes   * 72) >> 3; //length in Bytes
                pDlCntrl_Pars->pdcchRNTI[i] = pInt1->scrmblerInitValue;
#else
                pDlCntrl_Pars->StartCCECtl[i - numPhich] = (pInt1->startRes * 72) >> 3; //start location in Bytes
                pDlCntrl_Pars->NumCCECtl[i - numPhich]   = (pInt1->numRes   * 72) >> 3; //length in Bytes
                pDlCntrl_Pars->pdcchRNTI[i - numPhich] = pInt1->scrmblerInitValue;
#endif
            }
            else if (pInt1->channelType == PHICH)
            {
                pDlCntrl_Pars->scalefactorPHICH = phyChanScale_factor;
            }
            pInt1 = (pInt1 + 1);
        }
    }
    pDlCntrl_Pars->numPDCCH = numPDCCH;

    return status;
}

