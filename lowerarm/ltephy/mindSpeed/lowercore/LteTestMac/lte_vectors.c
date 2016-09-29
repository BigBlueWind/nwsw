/** @file lte_vectors.c
 *
 * @brief Implemenataion for create TX and RX vectors
 * @author Mindspeed Technologies
 * @version $Revision: 1.19 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef AC_MSPD_TDD
#include "LtePhyL2Api.h"
#else
#include "LtePhyL2Api_TDD.h"
#endif

#include "resultcodes.h"
#include "LteMacTestCfgMgr.h"

#ifdef MX_ARM_ENABLED
#include "enbMac2PhyIface.h"
#else
#include "timers.h"
#include "LteMac2PhyInterface.h"
#include "udp_stream.h"
#endif

#include "lte_mac_typedef.h"
#include "lte_mac_state.h"
#include "lte_pbch.h"
#include "lte_phich.h"
#include "lte_pdsch.h"
#include "lte_pusch.h"
#include "lte_pdcch.h"
#include "lte_pucch.h"
#include "lte_modulation.h"
#include "lte_vectors.h"


extern BS_CONTEXT *pBsMacDesc;
extern tTestMgrGetPdcch pTestMgrGetPdcch;
extern tTestMgrGetPdschMap pTestMgrGetBoostedPdsch;
extern tTestMgrGetPdschMap pTestMgrGetDeBoostedPdsch;


// ---------------------------------------------------------------------------------------------------------------------------
//                                                       RX 
// ---------------------------------------------------------------------------------------------------------------------------
/** Fill header for RxVector
 *
 * @param ptest_conf_config pointer to TestConfConfig
 * @param pSubFrame pointer to UlSubFrame
 * @return Standard result code
 */
RESULTCODE lte_vectors_fill_rx_header(BS_CONFOR_TEST_CONFIG *ptest_conf_config,
                                      PULSUBFRDESC pSubFrame)
{
    PULSUBFRCMNCTRL ulSfrCtrl = &pSubFrame->ulSfrCtrl;
    PRINT_FUNCTION_NAME(__FUNCTION__);

    memset(pSubFrame, 0, sizeof(ULSUBFRDESC));

    pSubFrame->frameNumber = (U16)getFrameNum() & 0x3ff;
    pSubFrame->subframeNumber = getSubFrameNum();
    pSubFrame->subframeType = ULRX;
    pSubFrame->antennaPortcount = pBsMacDesc->initParm.rxAntennaPortCount;
    pSubFrame->numberofChannelDescriptors = 0x00;

    pSubFrame->numberOfCtrlChannelDescriptors = 0x00;

    // offsets to be updated later after channels adding
    pSubFrame->offsetCustomFeatures = NOT_SPECIFIED_VALUE;

    pSubFrame->numberSrsinSf = NOT_SPECIFIED_VALUE;

    // disable RACH channel
    if(ptest_conf_config->prachEnableAllSfs)
        pSubFrame->rachCtrl.prachEnable = PRACH_ON;
    else
        pSubFrame->rachCtrl.prachEnable = PrachConfig(ptest_conf_config->prachConfigIndex,
                                                      pSubFrame->frameNumber,
                                                      pSubFrame->subframeNumber);

    ///SubFrame Control Info
    ulSfrCtrl->deltaPUCCHShift = ptest_conf_config->ulSfrCtrl.deltaPUCCHShift; //2;
    ulSfrCtrl->nRBCQI = ptest_conf_config->ulSfrCtrl.nRBCQI; //1;
    ulSfrCtrl->nCSAn = ptest_conf_config->ulSfrCtrl.nCSAn; //0;
    ulSfrCtrl->n1PucchAN = ptest_conf_config->ulSfrCtrl.n1PucchAN; //11;
#ifndef AC_MSPD_TDD
    ulSfrCtrl->pad = 0;
#endif
    ulSfrCtrl->srsBandwitdhConfig = 0;
    ulSfrCtrl->srsSubframeConfig = 0;
    ulSfrCtrl->ackNackSRSSimultaneousTransmission = ptest_conf_config->ackNackSRSSimultaneousTransmission;
    ulSfrCtrl->nSB = ptest_conf_config->ulSfrCtrl.nSB;
    ulSfrCtrl->hoppingMode = ptest_conf_config->ulSfrCtrl.hoppingMode;
    ulSfrCtrl->puschhoppingOffset = ptest_conf_config->ulSfrCtrl.puschhoppingOffset;
    ulSfrCtrl->enable64QAM = ptest_conf_config->ulSfrCtrl.enable64QAM; //1;
    ulSfrCtrl->groupHoppingEnabled = ptest_conf_config->ulSfrCtrl.groupHoppingEnabled;
    ulSfrCtrl->groupAssignmentPUSCH = ptest_conf_config->ulSfrCtrl.groupAssignmentPUSCH;
    ulSfrCtrl->sequenceHoppingEnabled = ptest_conf_config->ulSfrCtrl.sequenceHoppingEnabled;
    ulSfrCtrl->cyclicShift = ptest_conf_config->ulSfrCtrl.cyclicShift; //0;
    ulSfrCtrl->padding = 0;

    return SUCCESS;
}

/** Add Rx channel
 *
 * @param  ptest_conf_config Pointer to TestConfConfig
 * @param  pSubFrame Pointer to UlSubFrame
 * @param  channelType type of channel. Possible types: PUSCH, PRACH, PUCCH
 * @param  pmapInfo pointer to local array !!!!
 * @return Standard result code
 */
RESULTCODE lte_vectors_rx_add(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                              PULSUBFRDESC pSubFrame,
                              U8 channelType,
                              PMAPPINGINFO pmapInfo)
{
    U8 chanIdx = pSubFrame->numberofChannelDescriptors;
    MAPPINGINFO mapInfoPusch;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    switch(channelType)
    {
        case PRACH:
            {
                pSubFrame->offsetRachCtrlStruct = (U32)&pSubFrame->rachCtrl - (U32)pSubFrame;
                pSubFrame->rachCtrl.rootSequenceIndex  = pTestConfConfig->rootSequenceIndex; //22;
                pSubFrame->rachCtrl.prachConfigIndex = pTestConfConfig->prachConfigIndex;// 12; 
                pSubFrame->rachCtrl.highSpeedFlag = pTestConfConfig->highSpeedFlag;// 0
                pSubFrame->rachCtrl.zeroCorrelationZoneConfig = pTestConfConfig->zeroCorrelationZoneConfig;//1;
                pSubFrame->rachCtrl.prachFreqOffset = pTestConfConfig->prachFreqOffset; //0
            }
            break;
        case PUCCH:
            {
                U32 NumCtrl = pSubFrame->numberOfCtrlChannelDescriptors;

                lte_pucch_add(pTestConfConfig,
                               &pSubFrame->ulCtlCh[NumCtrl],
                               chanIdx,
                               pmapInfo,
                               NumCtrl);

                pSubFrame->numberOfCtrlChannelDescriptors++;
                chanIdx++;
            }
            break;
        case PUSCH:
            /* PUSCH channels with boosted PRBs */
            if((chanIdx - pSubFrame->numberOfCtrlChannelDescriptors) == 0)
            {// we're adding the first PUSCH
                pmapInfo->numberofEntries = 0;
            }

            if (pTestMgrGetPusch(pTestConfConfig, &mapInfoPusch, pmapInfo) == SUCCESS)
            {
                U8 NumCtrl = pSubFrame->numberOfCtrlChannelDescriptors;
                ULCHANDESC* ulCh = &pSubFrame->ulCh[chanIdx - NumCtrl];
                U8 Modulation;
                U8 mcsType;

                lte_pusch_add(pTestConfConfig,
                              ulCh,
                              chanIdx,
                              lte_pusch_get_power(pTestConfConfig));

                if(pTestConfConfig->puschMcs_used)
                {
                    mcsType = pTestConfConfig->puschMcs[chanIdx - NumCtrl];
                    Modulation = lte_pusch_conv_mcs_to_modulation(mcsType);
                }
                else
                {
                    Modulation = lte_pusch_get_modulation(pTestConfConfig);
                    mcsType = lte_modulation_get_mcs_index_ul(Modulation);
                }

                lte_pusch_fill_sub_channel( pTestConfConfig,
                                        &ulCh->ulSubChInfo,
                                        Modulation,
                                        mcsType,
                                        &mapInfoPusch,
                                        chanIdx - NumCtrl
                                        );

                // be aware that SRS is added before PUSCH for this working
                if(pSubFrame->numberSrsinSf)
                    ulCh->ulSubChInfo.puschDed.nsymi--;

                chanIdx++;
            }
            else
                return FAILURE;

            break;
        default:
            break;
    }

    pSubFrame->numberofChannelDescriptors = chanIdx;
    return SUCCESS;
}

/** update Rx Srs
 *
 * @param  ptest_conf_config Pointer to TestConfConfig
 * @param  pSubFrame Pointer to UlSubFrame
 * @return Standard result code
 */
RESULTCODE lte_vectors_update_rx_srs(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                     PULSUBFRDESC pSubFrame)
{
    SRSDED* SrsInfo = pSubFrame->srsInfo;
    U8 srsSFcfgTableIdx;
    S16 Tsfc;
    S16 *subframeOffset;
    S16 subframeNumber;
    S16 subframeOffsetLength;
    S16 Toffset = 0;
    U16 idx, Isrs, Tsrs, UEtableRow;
    U8 SrsNum = 0, bAllowedSrsEnodeb = 0;
    S16 index;

    pSubFrame->ulSfrCtrl.srsSubframeConfig = pTestConfConfig->srsSubframeConfig;
    pSubFrame->ulSfrCtrl.srsBandwitdhConfig = pTestConfConfig->srsBandwitdhConfig;

    if( (pTestConfConfig->srsSubframeConfig == 15) || (pTestConfConfig->srsNumber == 0) )
    {
        pSubFrame->numberSrsinSf = 0;
        return SUCCESS;
    }

    srsSFcfgTableIdx = pTestConfConfig->srsSubframeConfig * 10;
    Tsfc = CA_srsSubframeConfigTable[srsSFcfgTableIdx++];
    subframeOffsetLength = CA_srsSubframeConfigTable[srsSFcfgTableIdx++];
    subframeOffset = &CA_srsSubframeConfigTable[srsSFcfgTableIdx];

    subframeNumber = pSubFrame->subframeNumber % Tsfc; //subframe number where the SRS is enabled, if configured

    index = 0;
    while (index < subframeOffsetLength)
    {
        if (subframeNumber == *(subframeOffset + index)) //check calculated subframe number against configured subframe numbers
        {// allowed eNodeB for this SF
            bAllowedSrsEnodeb = 1;
            break;
        }
        ++index;
    }

    if(bAllowedSrsEnodeb)
    {// allowed eNodeB for this SF
        idx = 0;
        do
        {// per UE
            Isrs = pTestConfConfig->srsInfo[idx].srsConfigIndex;  // Isrs
            Tsrs = 0;
            SrsInfo->enableDisable = 0;
            UEtableRow = 0;
            do
            {
                U16* UEsrsTableRow = &UESpecificSRSPeriodicityTable[UEtableRow][0];
                if((UEsrsTableRow[0] <= Isrs) && (UEsrsTableRow[1] >= Isrs))
                {
                    Tsrs = UEsrsTableRow[2];   // Tsrs
                    Toffset = Isrs - UEsrsTableRow[0];  // Toffset = Isrs - x
                    break;
                }
            }while(++UEtableRow < 9);

            if(Tsrs == 0)
            {
                printf("TestMac ERROR: invalid SrsConfigIndex for user %d (Isrs = %d)\n",  idx, Isrs);
                return 1;
            }

            // if (((10 * Nf + ksrs - Toffset) mod Tsrs) == 0); Nf is SFN, ksrs = {0,1..9} is SF
            if (  ((pSubFrame->frameNumber * 10 + pSubFrame->subframeNumber - Toffset) %  Tsrs) == 0)
            {// allowed UE for this SF
                memcpy(SrsInfo, &pTestConfConfig->srsInfo[idx], sizeof(SRSDED));
                SrsInfo->enableDisable  = 1; //enable the SRS in this subframe
                SrsNum++;
                SrsInfo++;
            }
        }while(++idx < pTestConfConfig->srsNumber);
    }

    pSubFrame->numberSrsinSf = SrsNum;

    return SUCCESS;
}

RESULTCODE lte_vectors_update_rx_srs_tdd(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                     PULSUBFRDESC pSubFrame)
{
    SRSDED* SrsInfo = pSubFrame->srsInfo;
    S16 srsSubframeConfig;
    S16 subframePeriod;
    S16 *subframeOffset;
    S16 subframeNumber;
    S16 UePeriod;
    U8 SrsNum = 0;

// TODO:
#if 0
//#ifdef MX_ARM_ENABLED

    S16 idx, PerodIdx, Tsrs, OffsetSrs = 0;

    pSubFrame->ulSfrCtrl.srsSubframeConfig = pTestConfConfig->srsSubframeConfig;
    pSubFrame->ulSfrCtrl.srsBandwitdhConfig = pTestConfConfig->srsBandwitdhConfig;
    if(pTestConfConfig->srsSubframeConfig == 15)
    {
        pSubFrame->numberSrsinSf = 0;
        return SUCCESS;
    }
	for(idx = 0; idx < pTestConfConfig->srsNumber; idx++)
    {
        srsSubframeConfig = pTestConfConfig->srsSubframeConfig;
        subframePeriod = CA_srsSubframeConfigTable[10 * srsSubframeConfig];
        subframeOffset = &CA_srsSubframeConfigTable[10 * srsSubframeConfig + 2];

        subframeNumber = pSubFrame->subframeNumber % subframePeriod; //subframe number where the SRS is enabled, if configured
        SrsInfo->enableDisable = 0;
        if (subframeNumber == *(subframeOffset)) //check calculated subframe number against configured subframe numbers
        {
		    UePeriod = pTestConfConfig->srsInfo[idx].srsConfigIndex;
			Tsrs = 0;
		    for(PerodIdx = 0; PerodIdx < 9; PerodIdx++)
		    {
		     if(UESpecificSRSPeriodicityTable[PerodIdx][0] <= UePeriod && UESpecificSRSPeriodicityTable[PerodIdx][0] >= UePeriod)
		     	{
			     	Tsrs = UESpecificSRSPeriodicityTable[PerodIdx][2];
					OffsetSrs = UESpecificSRSPeriodicityTable[PerodIdx][3];
					break;
		     	}
		    	
		    }
			if(Tsrs == 0)
			{
				printf("TestMac ERROR: invalid SrsConfigIndex for user %d\n",idx );
				return 1;
			}
			OffsetSrs = PerodIdx - OffsetSrs;
			if( (  ((pSubFrame->subframeNumber + (pSubFrame->frameNumber << 4)) + OffsetSrs + 1 ) %  Tsrs) != 0)
			{
		                memcpy(SrsInfo, &pTestConfConfig->srsInfo[idx], sizeof(SRSDED));
		                SrsInfo->enableDisable  = 1; //enable the SRS in this subframe
                                SrsNum++;
		                break;
		         }
	    SrsInfo++;
        }

    }

#else

    S16 index;
    U8 idx, PerodIdx;
    U16 Tsrs, OffsetSrs = 0;
    S16 subframeOffsetLength;

    pSubFrame->ulSfrCtrl.srsSubframeConfig = pTestConfConfig->srsSubframeConfig;
    pSubFrame->ulSfrCtrl.srsBandwitdhConfig = pTestConfConfig->srsBandwitdhConfig;

    if(pTestConfConfig->srsSubframeConfig == 15)
    {
        pSubFrame->numberSrsinSf = 0;
        return SUCCESS;
    }

    for(idx = 0; idx < pTestConfConfig->srsNumber; idx++)
    {
        srsSubframeConfig = pTestConfConfig->srsSubframeConfig;
        subframePeriod = CA_srsSubframeConfigTable[10 * srsSubframeConfig];
        subframeOffset = &CA_srsSubframeConfigTable[10 * srsSubframeConfig + 2];
        subframeOffsetLength = CA_srsSubframeConfigTable[10 * srsSubframeConfig + 1];

        subframeNumber = pSubFrame->subframeNumber % subframePeriod; //subframe number where the SRS is enabled, if configured
        SrsInfo->enableDisable = 0;
        for (index = 0; index < subframeOffsetLength; index++)
        {
            if (subframeNumber == *(subframeOffset + index)) //check calculated subframe number against configured subframe numbers
            {
			    UePeriod = pTestConfConfig->srsInfo[idx].srsConfigIndex;
				Tsrs = 0;
			    for(PerodIdx = 0; PerodIdx < 9; PerodIdx++)
			    {
			     if(UESpecificSRSPeriodicityTable[PerodIdx][0] <= UePeriod && UESpecificSRSPeriodicityTable[PerodIdx][1] >= UePeriod)
			     	{
				     	Tsrs = UESpecificSRSPeriodicityTable[PerodIdx][2];
						OffsetSrs = UESpecificSRSPeriodicityTable[PerodIdx][0];
						break;
			     	}
			    	
			    }
				if(Tsrs == 0)
				{
					printf("TestMac ERROR: invalid SrsConfigIndex for user %d\n",idx );
					return 1;
				}
				OffsetSrs = PerodIdx - OffsetSrs;
				if( (  ((pSubFrame->subframeNumber + (pSubFrame->frameNumber << 4)) + OffsetSrs + 1) %  Tsrs) != 0)
				{
			                memcpy(SrsInfo, &pTestConfConfig->srsInfo[idx], sizeof(SRSDED));
			                SrsInfo->enableDisable  = 1; //enable the SRS in this subframe
							SrsNum++;
			                break;
			    }
            }
		SrsInfo++;
        }

    }

#endif

	pSubFrame->numberSrsinSf = SrsNum;

    return SUCCESS;
}

/** processing vectors for Rx channels( Add Ul channels)
 *
 * @param  pSubFrame Pointer to UlSubFrame
 * @return Standard result code
 */
RESULTCODE lte_vectors_processing_ul(PULSUBFRDESC pSubFrame)
{
    BS_CONFOR_TEST_CONFIG *ptest_conf_config = get_bs_conf_test_config();
    U32 i;
    MAPPINGINFO map_info;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    memset(&map_info, 0, sizeof(MAPPINGINFO));

    // common stuff
    lte_vectors_fill_rx_header(ptest_conf_config, pSubFrame);

//#ifdef MX_ARM_ENABLED
#if defined (MX_ARM_ENABLED)
        if(!testConfConfig.ULEnabled)
        {
#ifndef LOWLATENCY
            if ((getSubFrameNum() != 0) || (getFrameNum() != 0) )
#endif
                pHiDciApiSet(NULL, NULL, NULL);
        }
#elif defined (USE_LINUX)
        if(!testConfConfig.ULEnabled)
        {
            pHiDciApiSet(NULL, NULL, NULL);
        }
#endif

    if(pSubFrame->rachCtrl.prachEnable)
        lte_vectors_rx_add(ptest_conf_config, pSubFrame, PRACH, &map_info);

    lte_vectors_update_rx_srs(ptest_conf_config, pSubFrame);

    for(i=0; i<testConfConfig.numPusch[pSubFrame->subframeNumber]; i++)
        lte_vectors_rx_add(ptest_conf_config, pSubFrame, PUSCH, &map_info);

    for(i=0; i<testConfConfig.numPucch[pSubFrame->subframeNumber]; i++)
        lte_vectors_rx_add(ptest_conf_config, pSubFrame, PUCCH, &map_info);

    return SUCCESS;
}


// ---------------------------------------------------------------------------------------------------------------------------
//                                                       TX 
// ---------------------------------------------------------------------------------------------------------------------------
/** get number of control symbols
 *
 * @param ptest_conf_config pointer to TestConfConfig
 * @return number of control symbols
 */
U8 lte_vectors_get_num_ctrl_symbols(BS_CONFOR_TEST_CONFIG *pTestConfConfig)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    return pTestConfConfig->nCtrlSymbols;
}


/** get number
 *
 * @param
 * @return number of Tprs
 */
U16 lte_vectors_get_tprs(U16 i_prs)
{
    
    if (i_prs<=159)
    {
        return 160;
    }
    if ((i_prs>=160) && (i_prs<=479))
    {
        return 320;
    }
    if ((i_prs>=480) && (i_prs<=1119))
    {
        return 640;
    }
    if ((i_prs>=1120) && (i_prs<=2399))
    {
        return 1280;
    }
//  reserved
    if ((i_prs>=2400) && (i_prs<=4095))
    {
        return 0;
    }
    return 0;
}


/** get DELTAprs
 *
 * @param
 * @return delta
 */
U16 lte_vectors_get_deltaprs(U16 i_prs)
{

    if (i_prs<=159)
    {
        return i_prs;
    }
    if ((i_prs>=160) && (i_prs<=479))
    {
        return i_prs - 160;
    }
    if ((i_prs>=480) && (i_prs<=1119))
    {
        return i_prs - 480;
    }
    if ((i_prs>=1120) && (i_prs<=2399))
    {
        return i_prs-1120;
    }
//  reserved
    if ((i_prs>=2400) && (i_prs<=4095))
    {
        return 0;
    }
    return 0;
}


/** Fill header for TxVector
 *
 * @param ptest_conf_config pointer to TestConfConfig
 * @param pSubFrame pointer to UlSubFrame
 * @return Standard result code
 */
RESULTCODE lte_vectors_fill_tx_header(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                      PDLSUBFRDESC pSubFrame)
{
    PBS_STATIC_CONFIG pStaticConfig = get_static_config();

    PRINT_FUNCTION_NAME(__FUNCTION__);
    memset(pSubFrame, 0, sizeof(DLSUBFRDESC));

    pSubFrame->frameNumber = (U16)getTxFrameNum() & 0x3ff;
    pSubFrame->subframeNumber = getTxSubFrameNum();
    pSubFrame->subframeType = DLTX;
    pSubFrame->antennaPortcount = pBsMacDesc->initParm.txAntennaPortCount;
    pSubFrame->numberofChannelDescriptors = 0x00;

    if (pStaticConfig->prs_enable)
    {
// check index of tti
        U16 tprs     = lte_vectors_get_tprs(pStaticConfig->i_prs);
        U16 deltaprs = lte_vectors_get_deltaprs(pStaticConfig->i_prs);
        
        if ((globalTTITickCount - deltaprs) % tprs == 0)
        {

            pSubFrame->prsEnable = 1;
            pSubFrame->prsNumRB = pStaticConfig->prs_num_rb;
            pSubFrame->dlCmnPwrCtl.prsRa = pStaticConfig->prs_ra;
        }
// don't need to send prs
        else
        {
            pSubFrame->prsEnable = 0;
            pSubFrame->prsNumRB = 0x00;
            pSubFrame->dlCmnPwrCtl.prsRa = 0x00;
        }
    }
    else
    {
        pSubFrame->prsEnable = 0;
        pSubFrame->prsNumRB = 0x00;
        pSubFrame->dlCmnPwrCtl.prsRa = 0x00;
    }

    pSubFrame->reserved0 = NOT_SPECIFIED_VALUE;

    // offsets to be updated later after channels adding
    pSubFrame->offsetCustomFeatures = NOT_SPECIFIED_VALUE;
    pSubFrame->numCtrlSymbols = lte_vectors_get_num_ctrl_symbols(pTestConfConfig);
    pSubFrame->phichResource = pBsMacDesc->initParm.phichResource;       // 0 - Ng =1/6
    pSubFrame->phichDuration = pBsMacDesc->initParm.phichDuration;       // 1 - Normal
    pSubFrame->numberOfPhichChannels  = pTestConfConfig->phichIdx == 0xFF ? TestMgrGetNumPhich(pTestConfConfig) : pTestConfConfig->nPhichGroup;
    pSubFrame->numCtrlChannels = 0;

    memcpy(&pSubFrame->dlCmnPwrCtl, &pStaticConfig->dl_cmn_pwr_ctl, sizeof(DLCMNTXPWRCTL));
    return SUCCESS;
}

/** Add Tx channel
 *
 * @param  ptest_conf_config Pointer to TestConfConfig
 * @param  pSubFrame Pointer to UlSubFrame
 * @param  channelType type of channel. Possible types: PUSCH, PRACH, PUCCH
 * @param  pmapInfo pointer to local array !!!!
 * @return Standard result code
 */
RESULTCODE lte_vectors_tx_add(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                              PDLSUBFRDESC pDlSubFrame,
                              U8 channelType)
{
    U8 chanIdx = (pDlSubFrame->numberofChannelDescriptors)&0xFF;
    U8 Idx = 0;
    MAPPINGINFO mapInfo;
    U8 Pdsch_idx = 0;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    switch(channelType)
    {
        case PHICH:
#ifndef MX_ARM_ENABLED
            if(pTestConfConfig->phichIdx != 0xFF)
            {
                for (Idx = 0;
                     Idx < pTestConfConfig->nPhichGroup;
                     Idx++)
                {
                    lte_add_phich(&pDlSubFrame->dciCh[chanIdx],
                                  chanIdx,
                                  Idx,  /* group ID */
                                  pTestConfConfig->phichIdx); /* sequence Index */
                    pDlSubFrame->numCtrlChannels++;
                    chanIdx++;
                }
            }
            else
#endif
            {
            for (Idx = 0;
                 Idx < pTestConfConfig->nPhichGroup;
                 Idx++)
            {
                U32 i;
                for (i = 0; i < pTestConfConfig->nPhichPerGroup; i++)
                {
                    lte_add_phich(&pDlSubFrame->dciCh[chanIdx],
                                   chanIdx,
                                   Idx,  /* group ID */
#ifdef MX_ARM_ENABLED
                                   pTestConfConfig->phichIdx); /* sequence Index */
#else
                                   (U8)((pTestConfConfig->nPhichPerGroup < 3)? i*4 : i)); /* sequence Index [0,4,0,4,0,4] */
#endif
                    pDlSubFrame->numCtrlChannels++;
                    chanIdx++;
                }
            }
            }
            break;

        case PDCCH:
            if ( pTestConfConfig->dyn_period )
            {
                U32 subframe_cfg_idx = (getTxFrameNum() * 10 + getTxSubFrameNum()) % pTestConfConfig->dyn_period;
                U32 ch_cfg_idx = 0;
                while ( (Idx = pTestConfConfig->dyn_pdcch[subframe_cfg_idx][ch_cfg_idx]) != DYN_CFG_END )
                {
                    pTestMgrGetPdcch(pTestConfConfig, Idx, &mapInfo);
                    lte_add_pdcch(&pDlSubFrame->dciCh[chanIdx],
                                  chanIdx,
                                  &mapInfo,
                                  Idx);
                    pDlSubFrame->numCtrlChannels++;
                    chanIdx++;
                    ++ch_cfg_idx;
                }
            }
            else
            {
            for (Idx = 0;
                 Idx < pTestConfConfig->nPdcch;
                 Idx++)
            {
                 pTestMgrGetPdcch(pTestConfConfig, Idx, &mapInfo);
                 lte_add_pdcch(&pDlSubFrame->dciCh[chanIdx],
                               chanIdx,
                               &mapInfo,
                               Idx);
                pDlSubFrame->numCtrlChannels++;
                chanIdx++;
            }
            }
            break;
            
        case PDSCH:
            {
                Pdsch_idx = 0;
                if ( pTestConfConfig->dyn_period )
                {
                    U32 subframe_cfg_idx = (getTxFrameNum() * 10 + getTxSubFrameNum()) % pTestConfConfig->dyn_period;
                    U32 ch_cfg_idx = 0;
                    while ( (Pdsch_idx = pTestConfConfig->dyn_pdsch[subframe_cfg_idx][ch_cfg_idx]) != DYN_CFG_END )
                    {
                        lte_pdsch_create(pTestConfConfig, pDlSubFrame, &mapInfo, &chanIdx, Pdsch_idx, 0);
                        ch_cfg_idx++;
                    }
                }
                else
                {
                    /* PDSCH channels with boosted PRBs */
                    if ( lte_pdsch_create(pTestConfConfig, pDlSubFrame, &mapInfo, &chanIdx, Pdsch_idx, 1) == SUCCESS )
                        Pdsch_idx++;

                    while(Pdsch_idx && (Pdsch_idx < pTestConfConfig->nPdsch[0]))
                    {
#ifndef MX_ARM_ENABLED
                        /* check if we've got  deBoosting in this model */
                       /* if (testMgrCtx.boostingMap[0][Pdsch_idx] == NULL)
                           return FAILURE;*/
#endif
                        lte_pdsch_create(pTestConfConfig, pDlSubFrame, &mapInfo, &chanIdx, Pdsch_idx, 0);
                        Pdsch_idx++;
                    }
                }
            }
            break;
            
        case PBCH:
            {
                if(pBsMacDesc->isMIBconfigured == FALSE)
                {// fill MIB self for not configured by Host bytes
                    MIB_INFO* pMib = (MIB_INFO*)pTestConfConfig->payloadPbch;

                    // update only SFN for now
                    pMib->systemFrameNumberHi = (getTxFrameNum() >> (2+6)) & 0x3;
                    pMib->systemFrameNumberLo = (getTxFrameNum() >> 2) & 0x3F;

                    pMib->phich_resource = pDlSubFrame->phichResource;
                    pMib->phich_duration = (pDlSubFrame->phichDuration == PHICHDUR_NORMAL)? 
                        PHICHDUR_MIB_NORMAL : PHICHDUR_MIB_EXTEND;
                }
                lte_add_pbch(&pDlSubFrame->dlCh[chanIdx - pDlSubFrame->numCtrlChannels],
                              chanIdx);
            }
            chanIdx++;
            break;
        case PCFICH:
            /* PHY addes automatically this channel */
            break;
        default:
            break;
    }

    pDlSubFrame->numberofChannelDescriptors = chanIdx;
    return SUCCESS;
}


/** processing vectors for Tx channels( Add Dl channels). This function uses into ccode (windows)
 *
 * @param  pointer to DLSUBFRDESC-structure
 * @return Standard result code
 */
RESULTCODE lte_vectors_processing_dl_ccode(PDLSUBFRDESC pSubFrame)
{

/*    U32 queueCount = 0;
    PRINT_FUNCTION_NAME(__FUNCTION__);

    if (queueCount == 0)
    {
        // Common stuff
        lte_vectors_fill_tx_header(&testConfConfig, pSubFrame);

#ifdef WIN32
        // PCFICH channel is put automaticaly by PHY 
        if(pSubFrame->numberOfPhichChannels)
            lte_vectors_tx_add(&testConfConfig, pSubFrame, PHICH);
#endif

        lte_vectors_tx_add(&testConfConfig, pSubFrame, PDCCH);

        lte_vectors_tx_add(&testConfConfig, pSubFrame, PDSCH);

        if ((getSubFrameNum() == 0) && (getFrameNum() % 4 == 0) && testConfConfig.pbchEna)
            lte_vectors_tx_add(&testConfConfig, pSubFrame, PBCH);
    }
    else
        return FAILURE;
*/
    return SUCCESS;
}

/** processing vectors for Tx channels( Add Dl channels). This function uses into 4gmx & ccode (windows)
 *
 * @param  pointer to DLSUBFRDESC-structure
 * @return Standard result code
 */
RESULTCODE lte_vectors_processing_dl(PDLSUBFRDESC pSubFrame)
{
    U32 queueCount = 0;

    PRINT_FUNCTION_NAME(__FUNCTION__);
#ifdef MX_ARM_ENABLED
    MxGetQueueDataCount(dlControlTxSduQueue, (UINT32*)&queueCount);
#endif
    if (queueCount == 0)
    {
        // Common stuff
        lte_vectors_fill_tx_header(&testConfConfig, pSubFrame);

#ifdef WIN32
        // PCFICH channel is put automaticaly by PHY 
        if(pSubFrame->numberOfPhichChannels)
            lte_vectors_tx_add(&testConfConfig, pSubFrame, PHICH);
#endif
        lte_vectors_tx_add(&testConfConfig, pSubFrame, PDCCH);

        lte_vectors_tx_add(&testConfConfig, pSubFrame, PDSCH);

        if ((getSubFrameNum() == 0) && (getFrameNum() % 4 == 0) && testConfConfig.pbchEna)
            lte_vectors_tx_add(&testConfConfig, pSubFrame, PBCH);
    }
    else
    {
        return FAILURE;
    }

    return SUCCESS;
}


/* Function procces test config and files TXVECTOR */
RESULTCODE TestMgrProcessConfigDlRt(PDLSUBFRDESC pSubFrame)
{
/*
#ifndef MX_ARM_ENABLED
    static U32 count = 0;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    if (count == 0)
    {
        //count ++;
        if (OsalGetElementCount(&dlControlTxSduQueue) == 0)
        {
            // Common stuff
            lte_vectors_fill_tx_header(&testConfConfig, pSubFrame);
            // PCFICH channel is put automaticaly by PHY

            if(pSubFrame->numberOfPhichChannels)
            {
                lte_vectors_tx_add(&testConfConfig, pSubFrame, PHICH);
            }

            lte_vectors_tx_add(&testConfConfig, pSubFrame, PDCCH);

            lte_vectors_tx_add(&testConfConfig, pSubFrame, PDSCH);

            if ((getSubFrameNum() == 0) && (getFrameNum() % 4 == 0))
                lte_vectors_tx_add(&testConfConfig, pSubFrame, PBCH);
        }
        else
            return FAILURE;
    }
#endif
*/
    return SUCCESS;
}

/** processing vectors for Tx channels( Add Dl channels). This function uses into Linux
 *
 * @param  pointer to DLSUBFRDESC-structure
 * @return Standard result code
 */
RESULTCODE lte_vectors_processing_dl_rt(PDLSUBFRDESC pSubFrame)
{
#ifndef MX_ARM_ENABLED
    static U32 count = 0;
    PLTE_PDSCH_HARQ_STATE      plte_pdsch_harq_state = get_lte_pdsch_harq_state();


    PRINT_FUNCTION_NAME(__FUNCTION__);

    if (count == 0)
    {
        U32 tt = Timer0GetCntr();
       
        if (OsalGetElementCount(&dlControlTxSduQueue) == 0)
        {
// Common stuff
            lte_vectors_fill_tx_header(&testConfConfig, pSubFrame);
            if (!(plte_pdsch_harq_state->ena_udp_stream))
            {
// PDCCH
                lte_vectors_tx_add(&testConfConfig, pSubFrame, PDCCH);
// PDSCH
                lte_vectors_tx_add(&testConfConfig, pSubFrame, PDSCH);
            }
            else
            {
                PLTE_PDCCH_CHAN_STATE plte_pdcch_chan_state = get_lte_pdcch_chan_state();

// just for single user
                plte_pdcch_chan_state->cce_idx_prev[0] = plte_pdcch_chan_state->cce_idx[0];
                plte_pdcch_chan_state->cce_idx[0] = plte_pdcch_chan_state->sCCE[0][getTxSubFrameNum()];

// send udp-data at 1..8 sf
                if ((getTxSubFrameNum() != 0) && (getTxSubFrameNum() != 9))
                {
                    OSAL_QUEUE  *pqueue = get_osal_udp_rx_queue();
                    U32 put_get = OsalCyclicBuffGetFullSize_spinlock(pqueue);
                    U32 udp_tx_threshold = 3422;

                    if (put_get >= udp_tx_threshold)
                    {
                        lte_pdcch_update_dci(1, 1, (plte_pdsch_harq_state->harq_count_cnt)&7);
// PDCCH
                        lte_vectors_tx_add(&testConfConfig, pSubFrame, PDCCH);
// PDSCH
                        lte_vectors_tx_add(&testConfConfig, pSubFrame, PDSCH);
                        plte_pdsch_harq_state->harq_count_cnt++;
                    }
                }
            }
// PBCH
            if ((getTxSubFrameNum() == 0) && (getTxFrameNum() % 4 == 0) && testConfConfig.pbchEna)
                lte_vectors_tx_add(&testConfConfig, pSubFrame, PBCH);
        }
    }
    else
    {
        return FAILURE;
    }
#endif
    return SUCCESS;
}



