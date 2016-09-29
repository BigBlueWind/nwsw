//-------------------------------------------------------------------------------------------
/** @file LteBsRxvectorproc.c
 *
 * @brief This file implements the rxVector processing for the LTE BS
 * @author Mindspeed Technologies
 * @version $Revision: 1.152 $
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
#include "mlog.h"

#if defined(RT_API_PROC)
#include "lte_entry.h"
#include "ceva_proc.h"
#include "LteBsRxConfig.h"
#endif

#ifndef _WIN32
#define CEVA_RX_VEC_PROC    1
#endif

extern S16 LteDeterminePrachSubcSpacing(S16 prachConfigurationIndex);

extern S16 LteDeterminePrachSubcSpacing(S16 prachConfigurationIndex);

#ifdef _WIN32
U32 ceva_rxvectorproc_params[16];
#else
U32 CRAMVAR ceva_rxvectorproc_params[16];
#endif


#ifndef WIN32

static U32 tUlDescDescr;
void DescrGenCB(LPVOID ptr)
{
    MLogTask(PID_RXSDU_CTX0_DATA_DESCRAMBLER_SEQ, RESOURCE_SYS_AXI, tUlDescDescr, GET_TICKS);
    return;
};
#endif

#ifndef _WIN32
void LteBsRxVectorProcessingRT_Done(void* ptr, UINT32 ceva_ret)
{
    PULRXINPUTPARAMS pRxInputParams = (PULRXINPUTPARAMS)ptr;

    if (ceva_ret != 0)
    {
        stop_printf("Something is wrong with Ceva function LteBsRxVectorProcessingRT retCode: 0x%x\r\n", ceva_ret);
        return;
    }

    if (pRxInputParams->pMdmaDescr)
    {
        tUlDescDescr = GET_TICKS;
        MdmaRunExQ (MDMA_HW_SYS, (PMDMA_FDESC)pRxInputParams->pMdmaDescr, DescrGenCB, NULL, 200);
        pRxInputParams->pMdmaDescr = NULL;
    }

    rxVectorProcDone = 1;

    return;
}
#else   // _WIN32
#define LteBsRxVectorProcessingRT_Done(ptr, ceva_ret)
#endif  // _WIN32


//-------------------------------------------------------------------------------------------
/** @brief Implements the rxVector processing for the LTE BS
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Defines the particular PHY that is going to perform the
 *                     processing of its rxVector
 *  @param ptr
 *                     Pointer to a SUBFRDESC structure that contains the
 *                     rxVector information required to execute the LTE BS Rx
 *                     subFrame
 *  @param len         Length in bytes of the RxVector
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
 *  |Name         |Name in   |Direction     |Length  |Type                 |Data Format |Valid Range |Units  |
 *  |             |Standard  |              |        |                     |            |            |       |
 *  ----------------------------------------------------------------------------------------------------------
 *  |phyInstance  |          |pInput        |1       |unsigned int         |(1:31:0)    |[0:2]       |       |
 *  ----------------------------------------------------------------------------------------------------------
 *  |ptr          |          |pInput        |1       |pStruct(SUBFRDESC)   |(1:31:0)    |(0,2^32)    |       |
 *  ----------------------------------------------------------------------------------------------------------
 *  |len          |          |Input         |1       |unsigned int         |(1:31:0)    |(0,2^32)    |bytes  |
 *  ----------------------------------------------------------------------------------------------------------
 *  |status       |          |Output        |1       |unsigned int         |(1:31:0)    |[0,1]       |       |
 *  ----------------------------------------------------------------------------------------------------------
 *  |pPhy         |          |pOutput       |1       |pStruct(LTEPHYDESC)  |(1:31:0)    |(0,2^32)    |       |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteBsPhyRxVectorProcessing  |
 *  ---------------------------------------------------
 *  |Input Streams       |ptr                         |
 *  ---------------------------------------------------
 *  |Output Streams      |pPhy                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |phyInstance                 |
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function implements the LTE BS PHY rxVector processing according to
 *  the information contained in the SUBFRDESC structure that the second
 *  parameter indicates and for the phyInstance stated in the first parameter.
 *  Once the PHY_RXSTART completes a PHY_RXSTART.indication message is issued.
 *  This message needs to be embedded into the Mindspeed generic message
 *  format described in MSA-81479 using MessageType 0x0005 (Indication Message)
 *  and MessageID 0x7000 Foreign API Message
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * MSA 81613
 *
 * \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
U32 LteBsPhyRxVectorProcessing(U32 phyInstance, PULSUBFRDESC pRxVector, U32 len)
{
    U32 status = 0, t = 0;
    PLTEPHYDESC pPhy;
    PBS pBs;
    PBS_RX pBsRx;
    PBS_RX_PRACH pBsRxPrach[2];
    PBS_RX_PRACHDet pBsRxPrachDet[2];
    PULCHANDESC pInt = NULL;
    PULCTRLCHDESC pInt1 = NULL;
    PSRSDED psrsInfo = NULL;
    PRACHCTRL prachCtrl = NULL;
    PBS_RX_CHAN_SRS pBsRxChanSrs;
    BS_SRS *pBsSrs;
    U32 harq_prc;
    ULCHANDESC * pulCh;

    ////////////////////////////////////////////////////////
    ULRXINPUTPARAMS *pRxInputParams;
    ULRXFECPARAMS   *pRxFecParams;
    ULRXSDUPARAMS   *pUlRxSduParams;

    S16 num_Pilots_slot_pucch;
    S16 startRB;

    U32 rnti_id;
    PBS_RX_RNTI_MAPPING pRntiMapping;
    ////////////////////////////////////////////////////////

    U32 i;
    U32 upperBound;
    U32 num_sym;
    S16 countUsedPUCCH_RB;
    S16 hist[100];

    pPhy = getPhyCtxApiProc(phyInstance);

    pBs = pPhy->pBs;
    pBsRx = pPhy->pBsRx;
    pBsSrs = pBs->pBsSrs;

    harq_prc = (pPhy->pharq->harq_prc + 1) % P_HARQ_PROC;

#ifndef _WIN32
    // Conext Num for sending SDUs
    pBsRx->rxSduCtx = gRxSduCtx;
    if (pRxVector->numberofChannelDescriptors)
    {
        gRxSduCtx = IncrementSduCtx(gRxSduCtx);
    }
#endif

    pBsRxPrach[0] = pBs->pBsRxPrach[0];
    pBsRxPrach[1] = pBs->pBsRxPrach[1];
    pBsRxPrachDet[0] = pBs->pBsRxPrachDet[0];
    pBsRxPrachDet[1] = pBs->pBsRxPrachDet[1];


    ///////////////////////////////////////////////////////////////////////////////
    //New Rx Modulation Input Structure
    ///////////////////////////////////////////////////////////////////////////////
    pRxInputParams = pBsRx->pUlRxInputParams;
    pUlRxSduParams = pBsRx->pUlRxSduParams;

    pRxInputParams->rxFrameNumber  = pRxVector->frameNumber;
    pRxInputParams->subframeNumber = pRxVector->subframeNumber;

    pPhy->rxFrameNumber   = pRxVector->frameNumber;
    pBsRx->subframeNumber = pRxVector->subframeNumber;

    i = pBsRx->RxFecParam_Context;
    pRxFecParams   = pBsRx->pUlRxFecParams[i];

    pBsRx->slotNumber     = 2 * pRxInputParams->subframeNumber;
    pRxInputParams->unusedRB = pBsRx->Nul_rb; // All RBs
    pRxInputParams->currTti = (pRxInputParams->rxFrameNumber * 10) + pBsRx->subframeNumber;

    pRxInputParams->rxIntMeasFlag    = 0;//pRxVector->ulSfrCtrl.rxIntMeasEn;      // Set to 0 till API support and optimizations are done
    pRxInputParams->rssiMeasFlag     = 0;//pRxVector->ulSfrCtrl.rssiMeasEn;       // Set to 0 till API support and optimizations are done
    pRxInputParams->rxTherNoiseFlag  = 0;//pRxVector->ulSfrCtrl.rxTherNoiseEn;    // Set to 0 till API support and optimizations are done
    //pRxInputParams->Temperature[0]   = 288;
    //pRxInputParams->Temperature[1]   = 288;
    //pRxInputParams->Temperature[2]   = 288;
    //pRxInputParams->Temperature[3]   = 288;

    //Set SRS parameters
    pBsRx->numSrsChan = pRxVector->numberSrsinSf;

    LogRxVector_Main(pRxVector);

    // Define pointer to PHY user mapping by RNTI.
    pRntiMapping = pPhy->pRntiMapping;

    // If new radio frame (10 subframes), then check PHY user mapping by RNTI.
    if ((pRxInputParams->subframeNumber % 10) == 0)
        bs_rx_rnti_flush_id(pRxInputParams->rxFrameNumber, pRntiMapping);

    //Set SRS parameters
    pBsSrs->srsEnable               = ((pRxVector->ulSfrCtrl.srsSubframeConfig != 15) ? 1 : 0);
    pBsSrs->srsBandwidthConfig      = pRxVector->ulSfrCtrl.srsBandwitdhConfig;
    pBsSrs->srsSubframeConfig       = pRxVector->ulSfrCtrl.srsSubframeConfig;
    pBsSrs->srsSimultaneousANandSRS = pRxVector->ulSfrCtrl.ackNackSRSSimultaneousTransmission;

    //Common variables for all channel types and users
    pRxInputParams->groupHoppingEnabled = pRxVector->ulSfrCtrl.groupHoppingEnabled;
    pRxInputParams->sequenceHoppingEnabled = pRxVector->ulSfrCtrl.sequenceHoppingEnabled;
    pRxInputParams->n1_DMRS  = CyclicShiftTab[pRxVector->ulSfrCtrl.cyclicShift];
    pRxInputParams->delta_ss = pRxVector->ulSfrCtrl.groupAssignmentPUSCH;
    pRxInputParams->slotNum = pBsRx->slotNumber;
    pRxInputParams->numChan  = pRxVector->numberofChannelDescriptors;

    //For all PUCCH
    pRxInputParams->delta_shift_PUCCH = (S8)pRxVector->ulSfrCtrl.deltaPUCCHShift;
    pRxInputParams->N2_rb = (S8)pRxVector->ulSfrCtrl.nRBCQI;
    pRxInputParams->N1_cs = (S8)pRxVector->ulSfrCtrl.nCSAn;

    pRxInputParams->Ncp0 =  pPhy->pBs->Ncp0;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    // Then perform the processing of the RxVector and the setting of
    // the Phy instance parameters per the RxVector contents.
    // Keep track of number of resource elements, modulation index and
    // number of expected bits at the transport block on a per channel
    // basis

    upperBound = (U32) (pRxVector->numberofChannelDescriptors
        - pRxVector->numberOfCtrlChannelDescriptors);

    ul_ConfigFrame_Chan(pPhy->pBs, pBsRx);

    pRxInputParams->numPUSCH = (S16)upperBound;
    pRxInputParams->numPUCCH = (S16)pRxVector->numberOfCtrlChannelDescriptors;
    pRxFecParams->numChan    = (U16)pRxVector->numberofChannelDescriptors;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    // Need to split loop between data and control channels
    // Do Data/Shared Channel here
    pRxFecParams->muxPUSCHAckNack = 0;
    pRxFecParams->muxPUSCHCqiRi = 0;
    for (i = 0; i < upperBound; i++)
    {
        pInt  = &pRxVector->ulCh[i];
        pulCh = &pRxVector->ulCh[i];

        // Process the persistent channels present in the RxVector
        if (pInt->persistEnable)
        {
            pRxFecParams->refSubFrame[i] = pRxInputParams->subframeNumber;
            pRxFecParams->refFrame[i]    = pRxInputParams->rxFrameNumber;
            // For a new allocation reset running counter
            pRxFecParams->runSubFrCtr[i] = 0;
        }

        pRxFecParams->channelType[i] = PUSCH;
        pRxFecParams->nDi[i]         = (gMissedApiTable[harq_prc] ? 1 : pInt->ulSubChInfo.harqInfo.nDi);
        pRxFecParams->rvidx[i]       = pulCh->ulSubChInfo.harqInfo.rV;
        pRxFecParams->Qm[i]          = 2 * pulCh->ulSubChInfo.mcinfo.modulationType;
        pRxFecParams->nRNTI[i]       = pulCh->ulSubChInfo.scrInfo.scrinitValueinput;
        pRxFecParams->HalfIterations = pulCh->halfIterations;
        pRxFecParams->Mpusch_rb[i]   = pInt->ulSubChInfo.mapInfo.reselmInfo[0].numRes;

        //Look up FEC parameters for this particular MCS Index
        PuschMCSbasedCalculations(pInt, pRxFecParams, (S16) (2 * pBsRx->Nul_symb), pRxInputParams->Nrb_sc, i);

        // For Multiplex Control and Data
        pRxFecParams->nACK[i]    = pInt->ulSubChInfo.puschDed.nACK;
        pRxFecParams->nRI[i]     = pInt->ulSubChInfo.puschDed.nRI;
        pRxFecParams->nr1CQI[i]  = pInt->ulSubChInfo.puschDed.nr1CQI;
        pRxFecParams->nrg1CQI[i] = pInt->ulSubChInfo.puschDed.nrg1CQI;
        pRxFecParams->nsymi[i]   = pInt->ulSubChInfo.puschDed.nsymi;
        pRxFecParams->nrbi[i]    = pInt->ulSubChInfo.puschDed.nrbi;

        pRxFecParams->betaOffsetACKIndex[i] = pInt->ulSubChInfo.puschDed.betaOffsetACKIndex;
        pRxFecParams->betaOffsetRIIndex[i]  = pInt->ulSubChInfo.puschDed.betaOffsetRIIndex;
        pRxFecParams->betaOffsetCQIIndex[i] = pInt->ulSubChInfo.puschDed.betaOffsetCQIIndex;

        if ((pRxFecParams->nsymi[i] != 0) && (pRxFecParams->nrbi[i] != 0) )
        {
            if (pRxFecParams->nACK[i])
            {
                pRxFecParams->muxPUSCHAckNack = 1;
            }
            if ((pRxFecParams->nRI[i] + pRxFecParams->nr1CQI[i] + pRxFecParams->nrg1CQI[i]) != 0)
            {
                pRxFecParams->muxPUSCHCqiRi = 1;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        //New Rx Modulation Input Structure
        ///////////////////////////////////////////////////////////////////////////////
        // Define rnti id, i.e. active channel pointer
        rnti_id = bs_rx_rnti_get_id(pRxFecParams->nRNTI[i],
                                    pRntiMapping, pRxInputParams->rxFrameNumber);

        pRxInputParams->harqRntiIndex[i]        = rnti_id;
        pRxInputParams->ChanId[i]               = (U16)i;
        pRxInputParams->usedIndexVars[i]        = &pRntiMapping->used_index[rnti_id];

        pRxInputParams->RBStart[i]              = (S8)pInt->ulSubChInfo.mapInfo.reselmInfo[0].startRes;
        pRxInputParams->NumRBs[i]               = (S8)pInt->ulSubChInfo.mapInfo.reselmInfo[0].numRes;
        pRxInputParams->unusedRB               -= pRxInputParams->NumRBs[i]; // substract PUSCH occupied RBs
        pRxInputParams->Qm[i]                   = (S8)(2*pInt->ulSubChInfo.mcinfo.modulationType);

        pRxInputParams->subcStart[i]            = pRxInputParams->RBStart[i] * pRxInputParams->Nrb_sc;
        pRxInputParams->Mpusch_sc[i]            = (pRxInputParams->NumRBs[i] * pRxInputParams->Nrb_sc);
        pRxFecParams->Mpusch_sc[i]              = pRxInputParams->Mpusch_sc[i];
        pRxInputParams->n2_DMRS[i]              = (S8)pInt->ulSubChInfo.puschDed.nDMRS2;
        pRxInputParams->physChanType[i]         = PUSCH;
        pRxInputParams->rssiChanMeasFlag[i]     = 0;//pInt->rssiChanMeasEn;      // Set to 0 till API support and optimizations are done

        //pull in timing offset, time averaged values from the corresponding active channel
        pRxInputParams->mErrAvg[2*i]            = pRntiMapping->used_index[rnti_id].mErrAvgCh[0];
        pRxInputParams->mErrAvg[2*i + 1]        = pRntiMapping->used_index[rnti_id].mErrAvgCh[1];
        pRxInputParams->mErrExpo[i]             = pRntiMapping->used_index[rnti_id].mErrExpo;
        pRxInputParams->usedIndexVars[i]->mSetDefaults = 1;//(U8)(pulCh->timAdvErrInfo.mSetDefaults);

        if (pRxInputParams->usedIndexVars[i]->NumRbAvg <= MAXRESOURCEBLOCKS)
            pRxInputParams->usedIndexVars[i]->NumRbAvg += pRxInputParams->NumRBs[i];
        pRxInputParams->NumRbAvg[i] = (U8)pRxInputParams->usedIndexVars[i]->NumRbAvg;

        if (1)//pulCh->timAdvErrInfo.mSetDefaults)
        {
            pRxInputParams->mErrAvg[2*i]     = 1;
            pRxInputParams->mErrAvg[2*i + 1] = 0;
            pRxInputParams->mErrExpo[i]      = -32768;
        }

        //pull in frequency offset, time averaged values from the corresponding active channel
        pRxInputParams->timeAveragedFreqErr[2*i +0] = pRntiMapping->used_index[rnti_id].timeAveragedFreqErr[0];
        pRxInputParams->timeAveragedFreqErr[2*i +1] = pRntiMapping->used_index[rnti_id].timeAveragedFreqErr[1];

        pRxInputParams->G[i] = pRxFecParams->G[i];
        pRxInputParams->pucchFormat[i] = -1;

        pPhy->demapOutSize[i] = pRxInputParams->G[i];

        LogRxVector_Pusch(pRxVector, i, pRxFecParams);

        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////
    }

    //Create RxChan for Control Channels
    //upperBound is the number of Shared Channels
    if (pRxVector->numberOfCtrlChannelDescriptors != 0)
    {
        pInt1 = (PULCTRLCHDESC) ((U32) pRxVector + pRxVector->offsetULCtrlChannels);
    }

    for (i = upperBound; i < (U32) (pRxVector->numberOfCtrlChannelDescriptors + upperBound); i++)
    {
        S8 format_type;

        format_type = pInt1->pucchDedCtl.formatType;
        pRxFecParams->channelType[i]   = PUCCH;
        pRxInputParams->pucchFormat[i] = format_type;
        pRxFecParams->pucchFormat[i]   = format_type;

        pUlRxSduParams->nRNTI[i]         = pInt1->scrmblerInitValue;
        pRxFecParams->nRNTI[i]          = pInt1->scrmblerInitValue;
        pUlRxSduParams->dlCqiPmiSizeBits[i] = pInt1->pucchDedCtl.dlCqiPmiSizeBits;

        rnti_id = bs_rx_rnti_get_id(pRxFecParams->nRNTI[i],
                                    pRntiMapping, pRxInputParams->rxFrameNumber);

        pRxInputParams->usedIndexVars[i] = &pRntiMapping->used_index[rnti_id];

        ///////////////////////////////////////////////////////////////////////////////
        //New Rx Modulation Input Structure
        ///////////////////////////////////////////////////////////////////////////////
        //Variables specific to PUCCH
        num_Pilots_slot_pucch = Num_Pilots_Slot[pRxInputParams->pucchFormat[i]];

        pRxInputParams->ChanId[i]               = (U16)i;
        pRxInputParams->physChanType[i]         = PUCCH;
        pRxInputParams->NumRBs[i]               = 1;
        pRxInputParams->betaPUCCH[i]            = pInt1->txpowerControl;
        pRxInputParams->num_Pilots_slot[i]      = num_Pilots_slot_pucch;
        pRxInputParams->Mpusch_sc[i]            = pRxInputParams->Nrb_sc;
        pRxInputParams->rssiChanMeasFlag[i]     = 0;//pInt1->rssiChanMeasEn;      // Set to 0 till API support and optimizations are done

        pRxInputParams->mErrAvg[2*i]     = pInt1->timAdvErrInfo.mErrAvgCh[0];
        pRxInputParams->mErrAvg[2*i + 1] = pInt1->timAdvErrInfo.mErrAvgCh[1];
        pRxInputParams->mErrExpo[i]      = pInt1->timAdvErrInfo.mErrExpo;
        if (1)//pInt1->timAdvErrInfo.mSetDefaults)
        {
            pRxInputParams->mErrAvg[2*i]     = 1;
            pRxInputParams->mErrAvg[2*i + 1] = 0;
            pRxInputParams->mErrExpo[i]      = -32768;
        }

        pRxInputParams->simSRHarq[i]            = pInt1->pucchDedCtl.simSRHarq;
        pRxInputParams->srPUCCHResourceIndex[i] = pInt1->pucchDedCtl.srPUCCHResourceIndex;
        pRxInputParams->n2[i]                   = pInt1->pucchDedCtl.cqiPUCCHResourceIndex;
        pRxInputParams->n1[i]                   = pInt1->pucchDedCtl.harqPucchIndex;

        bs_rx_RBassignment_pucch(pRxInputParams, i, &startRB, pRxInputParams->n1[i]);
        pRxInputParams->RBStart[i]  = (S8) (startRB);
        pRxInputParams->subcStart[i] =  pRxInputParams->RBStart[i] * pRxInputParams->Nrb_sc;

        LogRxVector_Pucch(pRxVector, (i-upperBound), startRB);

        pRxInputParams->Qm[i] = 2; // QPSK
        pRxFecParams->Qm[i]   = 2;
        if (format_type == FORMAT1A)
        {
            pRxInputParams->Qm[i] = 1;//BPSK
            pRxFecParams->Qm[i]   = 1;
        }
        else if (format_type == FORMAT1)
        {
            pRxInputParams->Qm[i] = 0; //no modulation
            pRxFecParams->Qm[i]   = 0;
        }

        pRxFecParams->mac_sdu[i] = NULL;   //not used for PUCCH. Null, so don't attempt to 'free' buffer after generating API indication msg in LteBsRx()

        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////

        pInt1 = (pInt1 + 1);
    }


    //Since PUCCH can overlap we need not to count duplicate

    countUsedPUCCH_RB = 0;
    for (i = upperBound; i < (U32) (pRxVector->numberOfCtrlChannelDescriptors + upperBound); i++)
    {
        hist[pRxInputParams->RBStart[i]] = 1;
    }
    for (i = upperBound; i < (U32) (pRxVector->numberOfCtrlChannelDescriptors + upperBound); i++)
    {
        if(hist[pRxInputParams->RBStart[i]] == 1)
        {
            hist[pRxInputParams->RBStart[i]] = 0;
            countUsedPUCCH_RB++;
        }
    }

    pRxInputParams->unusedRB -= countUsedPUCCH_RB; // substract PUCCH occupied RBs

#ifdef _WIN32
    i = 0;
#else
    i = pBsRx->RxPrach_Context;
#endif

    if (pRxVector->offsetRachCtrlStruct != 0)
    {
        prachCtrl = (PRACHCTRL) ((U32) pRxVector + pRxVector->offsetRachCtrlStruct);

        LogRxVector_Prach(pRxVector, 0);

        // Update Prach Detection Inforamtion
        pBsRxPrach[i]->bs_prach_enable = prachCtrl->prachEnable;
        if (pBsRxPrach[i]->bs_prach_enable == 1)
        {
            pBsRxPrachDet[i]->bs_prachDet_firLpf_length = 8;
            pBsRxPrachDet[i]->bs_prachDet_downsampleRate = 3;
            pBsRxPrachDet[i]->bs_prachDet_lpfDelay = 5;
            pBsRxPrach[i]->bs_prach_configurationIndex = prachCtrl->prachConfigIndex;
            pBsRxPrach[i]->bs_prach_subcSpacing
                    = LteDeterminePrachSubcSpacing((S16) prachCtrl->prachConfigIndex);
            pBsRxPrach[i]->bs_prach_rootSequenceIndex = prachCtrl->rootSequenceIndex;
            pBsRxPrach[i]->bs_prach_highSpeedFlag = prachCtrl->highSpeedFlag;
            pBsRxPrach[i]->bs_prach_cyclicShiftConfig = prachCtrl->zeroCorrelationZoneConfig;
            pBsRxPrach[i]->bs_prach_frequencyOffset = prachCtrl->prachFreqOffset;

            ul_ConfigFrame_PRACH_Globals(pPhy->pBs, pBsRx, pBs->pBsRxPrach[i], pRxInputParams->subframeNumber);
            phy_bs_rx_ConfigFrame(pBs->pBsRxPrach[i], pBs->pBsRxPrachDet[i]);
            pRxInputParams->unusedRB -= 6; // substract occupied RBs
        }
    }
    else
    {
        pBsRxPrach[i]->bs_prach_enable = 0;
    }


    //Create SRS structures
    if (pBsRx->numSrsChan != 0)
    {
        psrsInfo = (PSRSDED) ((U32) pRxVector + pRxVector->offsetsrsInfo);
        ul_ConfigFrame_SRS_Globals(pPhy->pBs, pBsRx, pBsSrs, pRxInputParams->subframeNumber);
    }

    for (i = 0; i < (U32) pBsRx->numSrsChan; i++)
    {
        LogRxVector_Srs(pRxVector, i);
        pBsRxChanSrs = pBsRx->pBsRxChanSrs;

        pBsRxChanSrs->enable[i] = (psrsInfo + i)->enableDisable;
        if (pBsRxChanSrs->enable[i] == 1)
        {
            pBsRxChanSrs->cdmEnable[i] = 0;    //1;  Changed to 0 since we are not supporting overlapping SRS yet
            pBsRxChanSrs->msIndex[i] = 0;
            pBsRxChanSrs->cdmGroupIndex[i] = 0;
            pBsRxChanSrs->cyclicShift[i] = (psrsInfo + i)->cyclicShift;
            pBsRxChanSrs->bandwidth[i] = (psrsInfo + i)->srsBandwidth;
            pBsRxChanSrs->hoppingBandwidth[i] = (psrsInfo + i)->srsHoppingBandwidth;
            pBsRxChanSrs->freqDomainPosition[i] = (psrsInfo + i)->freqDomainPosition;
            pBsRxChanSrs->duration[i] = (psrsInfo + i)->duration;
            pBsRxChanSrs->configIndex[i] = (psrsInfo + i)->srsConfigIndex;
            pBsRxChanSrs->transmissionComb[i] = (psrsInfo + i)->transmissionComb;
        }
    }

    num_sym = 2 * pBsRx->Nul_symb - 2;
    if (pBsSrs->srsSubframeEnable[pRxInputParams->subframeNumber] == 1 && pBsRx->numSrsChan > 0)
        num_sym--;

    ceva_rxvectorproc_params[0] = (U32) pRxInputParams;
    ceva_rxvectorproc_params[1] = (U32) pRxFecParams;
    ceva_rxvectorproc_params[2] = (U32) pPhy->pBsRx->pBsRxPointers->pRbUsedFlag;
    ceva_rxvectorproc_params[3] = (U32) pPhy->pharq;
    ceva_rxvectorproc_params[4] = (U32) pPhy->descrIndex;
    ceva_rxvectorproc_params[5] = (U32) pPhy->descrInBufs;
    ceva_rxvectorproc_params[6] = (U32) num_sym;
    ceva_rxvectorproc_params[7] = (U32) pBsRx->slotNumber;

#ifndef CEVA_RX_VEC_PROC
    LteBsRxVectorProcessingRT((UINT32)ceva_rxvectorproc_params);

    LteBsRxVectorProcessingRT_Done(pRxInputParams, 0);
#else
    {
        U32 hRxVecProc;
        hRxVecProc = ceva_call1_cb(LteBsRxVectorProcessingRT_Done, pRxInputParams,
                                   ceva_LteBsRxVectorProcessingRT, (UINT32)ceva_rxvectorproc_params);
        if(!hRxVecProc)
        {
            stop_printf("Failed to start UL VectorProc function on Ceva\r\n");
        }
    }
#endif //CEVA_RX_VEC_PROC

#ifndef _WIN32
    t = GET_TICKS;
    UpdateUlRxTaskListRT(pPhy);
    MLogTask(PID_RXLIST_UPDATE_TASKLIST, RESOURCE_LARM, t, GET_TICKS);
#endif

    return status;
}


