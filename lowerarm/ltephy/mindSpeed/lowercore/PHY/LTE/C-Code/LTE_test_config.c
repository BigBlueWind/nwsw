//-------------------------------------------------------------------------------------------
/** @file LTE_test_config.c
 *
 * @brief Contains functions to configure PHY for pre-defined test options
 * @author Mindspeed Technologies
 * @version $Revision: 1.103 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#if !defined _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "typedef.h"
#include "ltephydef.h"
#ifndef ENODEB
#include "prototypes.h"
#include "ext_phy.h"
#endif

#include "LTE_test_config.h"

//Global variable needed for SVSR logger for C-code
//to simulate pPhyDiCtx->PhyStartCurrCount in Real-Time Project
U32 PhyStartCurrCount = 0;

extern U32 test_case; // For mobile station visibility to Bs Number of Tx Antennas
CHCFG chancfg[100], chancfgms[100];
#ifdef FD_TEST
CHCFG chancfg1[100];
PCHCFG pchan1[100];
#endif
PCHCFG pchan[100], pchanms[100];
PSRSCHCFG pSrschan[100];
SRSCHCFG Srschancfg[100];

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL ReadConfigFile: Read Test Config file for Pre-defined Test Options
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param config_file   = INPUT, test config file for pre-defined test options
 *  @param tstpbs        = OUTPUT, Test Structure for Base Station
 *  @param tstpms        = OUTPUT, Test Structure for Mobile Station
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type          |Data Format |Valid Range |Units  |
 *  |            |Standard |              |        |              |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |config_file |         |Input         |1       |file pointer  |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |tstpbs      |         |pOutput       |1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |tstpms      |         |pOutput       |1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |ReadConfigFile              |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |config_file                 |
 *  ---------------------------------------------------
 *  |Status Parameters   |tstpbs,tstpms               |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Read Test configuration file for pre-defined Test Options.  Begin construction
 *  of Initialization Message, RxVector or TxVector.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------

#ifdef FD_TEST
void ReadConfigFile(FILE *config_file, TSTPDESC *tstpbs, TSTPDESC *tstpms, U32 rbAssignMode, PCHCFG *pchan, PCHCFG chancfg)
#else
void ReadConfigFile(FILE *config_file, TSTPDESC *tstpbs, TSTPDESC *tstpms, U32 rbAssignMode)
#endif
{
    int i, tmp1, tmp2, tmp3, j;
    S16 number_fields_read, UL_nDl, number_of_channels;

    // Read Test configuration file
    number_fields_read = fscanf(config_file, "%d", &tmp1);
    UL_nDl = (U8) tmp1;

    tstpbs->mapEnable = 0;

    if (UL_nDl == 1)
    {
        number_fields_read = fscanf(config_file, "%d", &tmp2);
        number_of_channels = (S16) tmp2;
        tstpbs->number_of_channels = number_of_channels;

        for (i = 0; i < number_of_channels; i++)
        {
            pchan[i] = &chancfg[i];
            tstpbs->pchan[i] = pchan[i];
            number_fields_read = fscanf(config_file, "%d", &tmp2);
            pchan[i]->ChanType = (U8) tmp2;
            number_fields_read = fscanf(config_file, "%d", &tmp2);
            pchan[i]->N2_RB = (U8) tmp2;
            number_fields_read = fscanf(config_file, "%d", &tmp2);
            pchan[i]->N1_CS = (U8) tmp2;
            number_fields_read = fscanf(config_file, "%d", &tmp2);
            pchan[i]->delta_shift_PUCCH = (U8) tmp2;
            number_fields_read = fscanf(config_file, "%d", &tmp2);
            pchan[i]->n1_pucch = (U16) tmp2;
            number_fields_read = fscanf(config_file, "%d", &tmp2);
            pchan[i]->n2_pucch = (U16) tmp2;
            number_fields_read = fscanf(config_file, "%d", &tmp2);
            pchan[i]->chPower = (U16) tmp2;
            number_fields_read = fscanf(config_file, "%d", &tmp2);
            pchan[i]->format_type = (U8) tmp2;
            number_fields_read = fscanf(config_file, "%d", &tmp2);
            pchan[i]->cyclicShift = (U8) tmp2;
            number_fields_read = fscanf(config_file, "%d", &tmp2);
            pchan[i]->n2_DMRS = (U8) tmp2;
            number_fields_read = fscanf(config_file, "%d", &tmp2);
            pchan[i]->DeltaSS = (U8) tmp2;
            number_fields_read = fscanf(config_file, "%d", &tmp2);
            pchan[i]->groupHoppingEnabled = (U8) tmp2;
            number_fields_read = fscanf(config_file, "%d", &tmp2);
            pchan[i]->sequenceHoppingEnabled = (U8) tmp2;

            if (((tstpbs->pchan[i]->ChanType == PUSCH) || (tstpbs->pchan[i]->ChanType == PUCCH))
                && (rbAssignMode))
            {

                tstpbs->mapEnable = 1;
                tstpbs->pchan[i]->mapMode = 1;
                number_fields_read = fscanf(config_file, "%d", &tmp3);
                tstpbs->pchan[i]->numBytes = (U32) tmp3;
                number_fields_read = fscanf(config_file, "%d", &tmp3);
                tstpbs->pchan[i]->numbits = (U32) tmp3;
                number_fields_read = fscanf(config_file, "%d", &tmp3);
                tstpbs->pchan[i]->map.numberofEntries = (U16) tmp3;

#ifdef DEBUG_PRINT
                printf("RB Assignment Mapping Mode  %hd\n", tstpbs->pchan[i]->mapMode);
                printf("Number of Bytes in PUSCH  %hd\n", tstpbs->pchan[i]->numBytes);
                printf("Number of Entries  %hd\n", tstpbs->pchan[i]->map.numberofEntries);
#endif

                for (j = 0; j < tstpbs->pchan[i]->map.numberofEntries; j++)
                {
                    number_fields_read = fscanf(config_file, "%d", &tmp3);
                    tstpbs->pchan[i]->map.reselmInfo[j].startRes = (U8) tmp3;
                    number_fields_read = fscanf(config_file, "%d", &tmp3);
                    tstpbs->pchan[i]->map.reselmInfo[j].numRes = (U8) tmp3;
#ifdef DEBUG_PRINT
                    printf("Starting resource block %hd\n", tstpbs->pchan[i]->map.reselmInfo[j].startRes);
                    printf("Number of resource blocks %hd\n", tstpbs->pchan[i]->map.reselmInfo[j].numRes);
#endif
                }
            }

            number_fields_read = fscanf(config_file, "%d", &tmp2);
            tstpbs->srsEnable = (S16) tmp2;

            if (tstpbs->srsEnable == 1)
            {
                number_fields_read = fscanf(config_file, "%d", &tmp2);
                tstpbs->srsBandwidthConfig = (S16) tmp2;
                number_fields_read = fscanf(config_file, "%d", &tmp2);
                tstpbs->srsSubframeConfig = (S16) tmp2;
                number_fields_read = fscanf(config_file, "%d", &tmp2);
                tstpbs->srsSimultaneousANandSRS = (S16) tmp2;
                number_fields_read = fscanf(config_file, "%d", &tmp2);
                tstpbs->numSrs = (S16) tmp2;

                for (j = 0; j < tstpbs->numSrs; j++)
                {
                    tstpbs->pSrschan[j] = &Srschancfg[j];
                    number_fields_read = fscanf(config_file, "%d", &tmp2);
                    tstpbs->pSrschan[j]->srs_enable = (U8) tmp2;

                    number_fields_read = fscanf(config_file, "%d", &tmp2);
                    tstpbs->pSrschan[j]->srs_cdmEnable = (U8) tmp2;

                    number_fields_read = fscanf(config_file, "%d", &tmp2);
                    tstpbs->pSrschan[j]->srs_msIndex = (U8) tmp2;

                    number_fields_read = fscanf(config_file, "%d", &tmp2);
                    tstpbs->pSrschan[j]->srs_cdmGroupIndex = (U8) tmp2;

                    number_fields_read = fscanf(config_file, "%d", &tmp2);
                    tstpbs->pSrschan[j]->srs_cyclicShift = (U8) tmp2;

                    number_fields_read = fscanf(config_file, "%d", &tmp2);
                    tstpbs->pSrschan[j]->srs_bandwidth = (U8) tmp2;

                    number_fields_read = fscanf(config_file, "%d", &tmp2);
                    tstpbs->pSrschan[j]->srs_hoppingBandwidth = (U8) tmp2;

                    number_fields_read = fscanf(config_file, "%d", &tmp2);
                    tstpbs->pSrschan[j]->srs_freqDomainPosition = (U8) tmp2;

                    number_fields_read = fscanf(config_file, "%d", &tmp2);
                    tstpbs->pSrschan[j]->srs_duration = (U8) tmp2;

                    number_fields_read = fscanf(config_file, "%d", &tmp2);
                    tstpbs->pSrschan[j]->srs_configIndex = (U8) tmp2;

                    number_fields_read = fscanf(config_file, "%d", &tmp2);
                    tstpbs->pSrschan[j]->srs_transmissionComb = (U8) tmp2;
                }
            }

            number_fields_read = fscanf(config_file, "%d", &tmp2);
            tstpbs->prachEnable = (S16) tmp2;
            if (tstpbs->prachEnable == 1)
            {
                number_fields_read = fscanf(config_file, "%d", &tmp2);
                tstpbs->prachConfigurationIndex = (S16) tmp2;
                number_fields_read = fscanf(config_file, "%d", &tmp2);
                tstpbs->prachSubcSpacing = (S16) tmp2;
                number_fields_read = fscanf(config_file, "%d", &tmp2);
                tstpbs->prachFrequencyOffset = (S16) tmp2;
                number_fields_read = fscanf(config_file, "%d", &tmp2);
                tstpbs->prachHighSpeedFlag = (S16) tmp2;
                number_fields_read = fscanf(config_file, "%d", &tmp2);
                tstpbs->prachCyclicShiftConfig = (S16) tmp2;
                number_fields_read = fscanf(config_file, "%d", &tmp2);
                tstpbs->prachRootSequenceIndex = (S16) tmp2;
            }

            tstpms->pchan[i] = &chancfgms[i];
            tstpms->pchan[i]->ChanType = tstpbs->pchan[i]->ChanType;
            if (tstpbs->pchan[i]->mapMode == 1)
            {
                tstpms->mapEnable = 1;
                tstpms->pchan[i]->mapMode = 1;
                tstpms->pchan[i]->numBytes = tstpbs->pchan[i]->numBytes;
                tstpms->pchan[i]->numbits = tstpbs->pchan[i]->numbits;
                tstpms->pchan[i]->map.numberofEntries = tstpbs->pchan[i]->map.numberofEntries;

                for (j = 0; j < tstpms->pchan[i]->map.numberofEntries; j++)
                {
                    tstpms->pchan[i]->map.reselmInfo[j].startRes
                        = tstpbs->pchan[i]->map.reselmInfo[j].startRes;
                    tstpms->pchan[i]->map.reselmInfo[j].numRes
                        = tstpbs->pchan[i]->map.reselmInfo[j].numRes;
                }

            }

        }
        tstpms->number_of_channels = tstpbs->number_of_channels;

    }
    else
    {
        // Added parameter "phichDuration" by ArrayComm, 2011-02-23
        // We found that parameter "phichDuration" will be used during the calculation of 
        // the resource allocation of PHICH, referred to LTE_3GPP_36.211. The time-domain
        // index is given according to the value of PHICH duration.
        // End added parameter "phichDuration"
        number_fields_read = fscanf(config_file, "%d", &tmp3);
        tstpbs->number_of_channels = (U32) tmp3;
        number_fields_read = fscanf(config_file, "%d", &tmp3);
        tstpbs->rvidx = (S16) tmp3;
        number_fields_read = fscanf(config_file, "%d", &tmp3);
        tstpbs->LayerCellIdGroup = (U8) tmp3;
        number_fields_read = fscanf(config_file, "%d", &tmp3);
        tstpbs->LayerId = (U8) tmp3;
        number_fields_read = fscanf(config_file, "%d", &tmp3);
        tstpbs->number_of_phich_channels = (U16) tmp3;
        number_fields_read = fscanf(config_file, "%d", &tmp3);
        tstpbs->Ng = (S16) tmp3;
        number_fields_read = fscanf(config_file, "%d", &tmp3);
        tstpbs->pilotPower = (S16) tmp3;
        number_fields_read = fscanf(config_file, "%d", &tmp3);
        tstpbs->psyncPower = (S16) tmp3;
        number_fields_read = fscanf(config_file, "%d", &tmp3);
        tstpbs->ssyncPower = (S16) tmp3;
        number_fields_read = fscanf(config_file, "%d", &tmp3);
        tstpbs->ciPower = (S16) tmp3;
        number_fields_read = fscanf(config_file, "%d", &tmp3);
        tstpbs->paprControl = (U16) tmp3;
        number_fields_read = fscanf(config_file, "%d", &tmp3);
        tstpbs->paprThreshold = (U16) tmp3;

#ifdef DEBUG_PRINT

        printf("\n");
        printf("DL Configuration Info for Debugging\n");
        printf("Number_of_channels %ld\n", tstpbs->number_of_channels);
        printf("Rvidx %ld\n", tstpbs->rvidx);
        printf("Number_of_phich_channels %ld\n", tstpbs->number_of_phich_channels);
        printf("Ng %ld\n", tstpbs->Ng);
        printf("PilotPower %3.4f dB\n", tstpbs->pilotPower/256.0);
        printf("PsyncPower %3.4f dB\n", tstpbs->psyncPower/256.0);
        printf("SsyncPower %3.4f dB\n", tstpbs->ssyncPower/256.0);
        printf("CFI power %3.4f dB\n", tstpbs->ciPower/256.0);
        printf("PaprControl  %hd\n", tstpbs->paprControl);
        printf("PaprThreshold  %hd\n", tstpbs->paprThreshold);
#endif
        for (i = 0; i < (S32) (tstpbs->number_of_channels); i++)
        {
            tstpbs->pchan[i] = &chancfg[i];
            number_fields_read = fscanf(config_file, "%d", &tmp3);
            tstpbs->pchan[i]->ChanType = (U8) tmp3;
            number_fields_read = fscanf(config_file, "%d", &tmp3);
            tstpbs->pchan[i]->chPower = (S16) tmp3;
            tstpbs->pchan[i]->mapMode = 0;

            number_fields_read = fscanf(config_file, "%d", &tmp3);
            tstpbs->pchan[i]->Qm = (U8) tmp3;

#ifdef DEBUG_PRINT
            printf("Channel type %hd\n", tstpbs->pchan[i]->ChanType);
            printf("Channel power %3.4f dB\n", tstpbs->pchan[i]->chPower/256.0);
            printf("Qm %hd\n", tstpbs->pchan[i]->Qm);
#endif

            if (tstpbs->pchan[i]->ChanType == PDSCH)
            {
                number_fields_read = fscanf(config_file, "%d", &tmp3);
                tstpbs->pchan[i]->nRNTI = (U16) tmp3;

#ifdef DEBUG_PRINT
                printf("nRNTI value for this PDSCH %hd\n", tstpbs->pchan[i]->nRNTI);
#endif
            }
            if (tstpbs->pchan[i]->ChanType == PDCCH)
            {
                number_fields_read = fscanf(config_file, "%d", &tmp3);
                tstpbs->pchan[i]->numbitsDCI = (U16) tmp3;
                number_fields_read = fscanf(config_file, "%d", &tmp3);
                tstpbs->pchan[i]->startCCE = (U8) tmp3;
                number_fields_read = fscanf(config_file, "%d", &tmp3);
                tstpbs->pchan[i]->numCCE = (U8) tmp3;
                number_fields_read = fscanf(config_file, "%d", &tmp3);
                tstpbs->pchan[i]->nRNTI = (U16) tmp3;

#ifdef DEBUG_PRINT
                printf("numbits for this PDCCH %hd\n", tstpbs->pchan[i]->numbitsDCI);
                printf("numCCE for this PDCCH %hd\n", tstpbs->pchan[i]->numCCE);
                printf("nRNTI value for this PDCCH %hd\n", tstpbs->pchan[i]->nRNTI);
#endif
            }
            if (tstpbs->pchan[i]->ChanType == PHICH)
            {

                number_fields_read = fscanf(config_file, "%d", &tmp3);
                tstpbs->pchan[i]->PhichGrp = (S16) tmp3;

                number_fields_read = fscanf(config_file, "%d", &tmp3);
                tstpbs->pchan[i]->PhichSequence = (S16) tmp3;

                number_fields_read = fscanf(config_file, "%d", &tmp3);
                tstpbs->pchan[i]->PhichBit = (U8) tmp3;
#ifdef DEBUG_PRINT
                printf("Phich Group %hd\n", tstpbs->pchan[i]->PhichGrp);
                printf("Phich Sequence %hd\n", tstpbs->pchan[i]->PhichSequence);
                printf("Phich Bit %hd\n", tstpbs->pchan[i]->PhichBit);
#endif
            }

            else if ((tstpbs->pchan[i]->ChanType == PDSCH) && (rbAssignMode))
            {
                tstpbs->mapEnable = 1;
                tstpbs->pchan[i]->mapMode = 1;
                number_fields_read = fscanf(config_file, "%d", &tmp3);
                tstpbs->pchan[i]->numBytes = (U32) tmp3;
                number_fields_read = fscanf(config_file, "%d", &tmp3);
                tstpbs->pchan[i]->map.numberofEntries = (U16) tmp3;

#ifdef DEBUG_PRINT
                printf("RB Assignment Mapping Mode  %hd\n", tstpbs->pchan[i]->mapMode);
                printf("Number of Bytes in PDSCH  %hd\n", tstpbs->pchan[i]->numBytes);
                printf("Number of Entries  %hd\n", tstpbs->pchan[i]->map.numberofEntries);
#endif

                for (j = 0; j < tstpbs->pchan[i]->map.numberofEntries; j++)
                {
                    number_fields_read = fscanf(config_file, "%d", &tmp3);
                    tstpbs->pchan[i]->map.reselmInfo[j].startRes = (U8) tmp3;
                    number_fields_read = fscanf(config_file, "%d", &tmp3);
                    tstpbs->pchan[i]->map.reselmInfo[j].numRes = (U8) tmp3;
#ifdef DEBUG_PRINT
                    printf("Starting resource block %hd\n", tstpbs->pchan[i]->map.reselmInfo[j].startRes);
                    printf("Number of resource blocks %hd\n", tstpbs->pchan[i]->map.reselmInfo[j].numRes);
#endif
                }
            }

            tstpms->pchan[i] = &chancfgms[i];
            tstpms->pchan[i]->ChanType = tstpbs->pchan[i]->ChanType;
        }
        tstpms->number_of_channels = tstpbs->number_of_channels;
        tstpms->LayerCellIdGroup = tstpbs->LayerCellIdGroup;
        tstpms->LayerId = tstpbs->LayerId;

#ifdef DEBUG_PRINT
        printf("\n");
        printf("\n");
#endif

    }

}

//-------------------------------------------------------------------------------------------
/** @brief LtePhyInitDefault: Creates Initialization Message
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = OUTPUT, Pointer to a PHY INITPARM structure to be initialized
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type          |Data Format |Valid Range |Units  |
 *  |            |Standard |              |        |              |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |pParm       |         |pOutput       |1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LtePhyInitDefault           |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |none                        |
 *  ---------------------------------------------------
 *  |Status Parameters   |pParm                       |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Setup of Initialization Message used by PHY for power on.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void LtePhyInitDefault(PINITPARM pParm)
{
    pParm->channelBandwidth = BW_10_0_MHZ;
    pParm->frtypeDuplexmode = FDD;
    pParm->radioAccessMode = OFDMA;
    pParm->physicalResourceBandwidth = PRB_12;
    pParm->numberResourceBlocksperSlot = RB_SLOT_50;
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pParm->txAntennaPortCount = TXANT_ONE;
    pParm->rxAntennaPortCount = RXANT_ONE;
    pParm->fastfwdorPreclockingNumber = 1600;
    pParm->fftSize = FFTSZ_1024;
    pParm->numberUsedSubcarriers = NUSC_600;
    pParm->nMaxDlRb = NMAXRB_50;
    pParm->nMaxUlRb = NMAXRB_50;
    pParm->referenceSignalPower = 250;
    pParm->primarySyncSignalPower = 65;
    pParm->secondarySyncSignalPower = 65;
    pParm->numDataRePerPRB = 80;
    pParm->cyclicPrefixType = NORMAL;
    pParm->txSduConfEnable = 0;
    pParm->radioFrameNumber = 0;
    pParm->subframeNumber = 0;
    pParm->slotNumber = 0;
    pParm->srsBandwidthConfig = 0;
    pParm->srsSubframeConfig = 0;
    pParm->srsSimultaneousANandSRS = 0;

    pParm->prachConfigurationIndex = 0;
    pParm->prachFrequencyOffset = 0;
    pParm->prachHighSpeedFlag = 0;
    pParm->prachCyclicShiftConfig = 0;
    pParm->prachRootSequenceIndex = 0;
}

//-------------------------------------------------------------------------------------------
/** @brief Initialize_PUCCH_info: Setup PUCCH area of RXVECTOR
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pSch  = OUTPUT, Pointer to a PUCCH subchannel area of RXVECTOR
 *  @param pChan = INPUT, Pointer test input structure containing data for PUCCH
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type          |Data Format |Valid Range |Units  |
 *  |            |Standard |              |        |              |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |pUlch        |         |pOutput       |1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |pChan       |         |pInput        |1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |Initialize_PUCCH_info       |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |pChan                       |
 *  ---------------------------------------------------
 *  |Status Parameters   |pSch                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Setup of PUCCH area of RXVECTOR for pre-defined Test Options
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void Initialize_PUCCH_info(PULCTRLCHDESC pUlch, PCHCFG pChan)
{
    pUlch->txpowerControl = (S16) pChan->chPower;
    pUlch->pucchDedCtl.cqiPUCCHResourceIndex = pChan->n2_pucch;
}

//-------------------------------------------------------------------------------------------
/** @brief Configure20MhzBw: Setup Initialization Message for 20Mhz Bandwidth
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = OUTPUT, Pointer to a PHY INITPARM structure to be initialized
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type          |Data Format |Valid Range |Units  |
 *  |            |Standard |              |        |              |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |pParm       |         |pOutput       |1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |Configure20MhzBw            |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |none                        |
 *  ---------------------------------------------------
 *  |Status Parameters   |pParm                       |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Setup of Initialization Message for 20Mhz bandwidth
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void Configure20MhzBw(PINITPARM pParm)
{
    pParm->channelBandwidth = BW_20_0_MHZ;
    pParm->numberResourceBlocksperSlot = RB_SLOT_100;
    pParm->fftSize = FFTSZ_2048;
    pParm->numberUsedSubcarriers = NUSC_1200;
    pParm->nMaxDlRb = NMAXRB_100;
    pParm->nMaxUlRb = NMAXRB_100;
}

//-------------------------------------------------------------------------------------------
/** @brief Configure15MhzBw: Setup Initialization Message for 15Mhz Bandwidth
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = OUTPUT, Pointer to a PHY INITPARM structure to be initialized
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type          |Data Format |Valid Range |Units  |
 *  |            |Standard |              |        |              |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |pParm       |         |pOutput       |1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |Configure15MhzBw            |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |none                        |
 *  ---------------------------------------------------
 *  |Status Parameters   |pParm                       |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Setup of Initialization Message for 15Mhz bandwidth
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void Configure15MhzBw(PINITPARM pParm)
{
    pParm->channelBandwidth = BW_15_0_MHZ;
    pParm->numberResourceBlocksperSlot = RB_SLOT_75;
    pParm->fftSize = FFTSZ_1536;
    pParm->numberUsedSubcarriers = NUSC_900;
    pParm->nMaxDlRb = NMAXRB_75;
    pParm->nMaxUlRb = NMAXRB_75;
}

//-------------------------------------------------------------------------------------------
/** @brief Configure10MhzBw: Setup Initialization Message for 10Mhz Bandwidth
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = OUTPUT, Pointer to a PHY INITPARM structure to be initialized
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type          |Data Format |Valid Range |Units  |
 *  |            |Standard |              |        |              |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |pParm       |         |pOutput       |1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |Configure10MhzBw            |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |none                        |
 *  ---------------------------------------------------
 *  |Status Parameters   |pParm                       |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Setup of Initialization Message for 10Mhz bandwidth
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void Configure10MhzBw(PINITPARM pParm)
{
    pParm->channelBandwidth = BW_10_0_MHZ;
    pParm->numberResourceBlocksperSlot = RB_SLOT_50;
    pParm->fftSize = FFTSZ_1024;
    pParm->numberUsedSubcarriers = NUSC_600;
    pParm->nMaxDlRb = NMAXRB_50;
    pParm->nMaxUlRb = NMAXRB_50;
}

//-------------------------------------------------------------------------------------------
/** @brief Configure5MhzBw: Setup Initialization Message for 5Mhz Bandwidth
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = OUTPUT, Pointer to a PHY INITPARM structure to be initialized
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type          |Data Format |Valid Range |Units  |
 *  |            |Standard |              |        |              |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |pParm       |         |pOutput       |1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |Configure5MhzBw             |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |none                        |
 *  ---------------------------------------------------
 *  |Status Parameters   |pParm                       |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Setup of Initialization Message for 5Mhz bandwidth
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void Configure5MhzBw(PINITPARM pParm)
{
    pParm->channelBandwidth = BW_5_0_MHZ;
    pParm->numberResourceBlocksperSlot = RB_SLOT_25;
    pParm->fftSize = FFTSZ_512;
    pParm->numberUsedSubcarriers = NUSC_300;
    pParm->nMaxDlRb = NMAXRB_25;
    pParm->nMaxUlRb = NMAXRB_25;
}

//-------------------------------------------------------------------------------------------
/** @brief Configure3MhzBw: Setup Initialization Message for 20Mhz Bandwidth
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = OUTPUT, Pointer to a PHY INITPARM structure to be initialized
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type          |Data Format |Valid Range |Units  |
 *  |            |Standard |              |        |              |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |pParm       |         |pOutput       |1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |Configure3MhzBw             |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |none                        |
 *  ---------------------------------------------------
 *  |Status Parameters   |pParm                       |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Setup of Initialization Message for 3Mhz bandwidth
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void Configure3MhzBw(PINITPARM pParm)
{
    pParm->channelBandwidth = BW_3_0_MHZ;
    pParm->numberResourceBlocksperSlot = RB_SLOT_15;
    pParm->fftSize = FFTSZ_256;
    pParm->numberUsedSubcarriers = NUSC_180;
    pParm->nMaxDlRb = NMAXRB_15;
    pParm->nMaxUlRb = NMAXRB_15;
}

//-------------------------------------------------------------------------------------------
/** @brief Configure1_4MhzBw: Setup Initialization Message for 1.4Mhz Bandwidth
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = OUTPUT, Pointer to a PHY INITPARM structure to be initialized
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type          |Data Format |Valid Range |Units  |
 *  |            |Standard |              |        |              |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  |pParm       |         |pOutput       |1       |pStructure    |            |            |       |
 *  -------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |Configure1_4MhzBw            |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |none                        |
 *  ---------------------------------------------------
 *  |Status Parameters   |pParm                       |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Setup of Initialization Message for 1.4Mhz bandwidth
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void Configure1_4MhzBw(PINITPARM pParm)
{
    pParm->channelBandwidth = BW_1_4_MHZ;
    pParm->numberResourceBlocksperSlot = RB_SLOT_6;
    pParm->fftSize = FFTSZ_128;
    pParm->numberUsedSubcarriers = NUSC_72;
    pParm->nMaxDlRb = NMAXRB_6;
    pParm->nMaxUlRb = NMAXRB_6;
}

//-------------------------------------------------------------------------------------------
/** @brief InitializeSubchannel: Inialize SubChannel structure of RxVector or TxVector<BR>
 *                               with Pre-Defined Test Vectors.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pSch   = OUTPUT, Pointer to SubChannel structure of TxVector or RxVector
 *  @param pTst   = INPUT,  Pointer to test structured used to populate SubChannel Structure
 *  @param id     = INPUT,  Channel ID
 *  @param chType = INPUT,  Channel Type
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction   |Length  |Type          |Data Format |Valid Range |Units  |
 *  |           |Standard |            |        |              |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pSch       |         |pOutput     |1       |pStructure    |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput      |1       |pStructure    |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |id         |         |Input       |1       |Real          |(0:32:0)    |[0:99]      |       |
 *  ----------------------------------------------------------------------------------------------
 *  |chType     |         |Input       |1       |Real          |(0:32:0)    |[0:13]      |       |
 *  ----------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |InitializeSubchannel        |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |pTst,id,chType              |
 *  ---------------------------------------------------
 *  |Status Parameters   |pSch                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Setup SubChannel Structure from RxVector or TxVector based on pre-defined<BR>
 *  test vectors.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void InitializeSubchannel(PDLSUBCHINFO pSch, PTSTPDESC pTst, U32 id, U32 chType)
{

    switch (chType)
    {
        case PDSCH:
            pSch->codingDescriptor = TURBOCDR;
            pSch->blockCodeConcatenation = 1;

            pSch->mcsType = pTst->mcs;
            pSch->nDi = (pTst->rvidx == 0) ? 1 : 0;
            pSch->rV = pTst->rvidx;
            pSch->flushReq = 0;

            break;
        case PBCH:
            pSch->codingDescriptor = TBVITCC;
            pSch->blockCodeConcatenation = 0;
            pSch->modulationType = QPSK;
            pSch->mcsType = 4;
            pSch->nDi = (pTst->rvidx == 0) ? 1 : 0;
            pSch->rV = pTst->rvidx;
            pSch->flushReq = 0;

            break;
        case PHICH:
            pSch->codingDescriptor = BLKCODHARQ;
            pSch->blockCodeConcatenation = 0;
            pSch->modulationType = BPSK;
            pSch->mcsType = 4;
            pSch->nDi = (pTst->rvidx == 0) ? 1 : 0;
            pSch->rV = pTst->rvidx;
            pSch->flushReq = 0;

            break;
    }
}

//-------------------------------------------------------------------------------------------
/** @brief InitializeULSubchannel: Inialize SubChannel structure of RxVector or TxVector<BR>
 *                               with Pre-Defined Test Vectors.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pSch   = OUTPUT, Pointer to SubChannel structure of TxVector or RxVector
 *  @param pTst   = INPUT,  Pointer to test structured used to populate SubChannel Structure
 *  @param id     = INPUT,  Channel ID
 *  @param chType = INPUT,  Channel Type
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction   |Length  |Type          |Data Format |Valid Range |Units  |
 *  |           |Standard |            |        |              |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pSch       |         |pOutput     |1       |pStructure    |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput      |1       |pStructure    |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |id         |         |Input       |1       |Real          |(0:32:0)    |[0:99]      |       |
 *  ----------------------------------------------------------------------------------------------
 *  |chType     |         |Input       |1       |Real          |(0:32:0)    |[0:13]      |       |
 *  ----------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |InitializeSubchannel        |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |pTst,id,chType              |
 *  ---------------------------------------------------
 *  |Status Parameters   |pSch                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Setup SubChannel Structure from RxVector or TxVector based on pre-defined<BR>
 *  test vectors.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void InitializeULSubchannel(PULSUBCHINFO pSch, PTSTPDESC pTst, U32 id, U32 chType)
{
    U32 i;

    switch (chType)
    {

        case PUSCH:
            pSch->mcinfo.modulationType = pTst->qm / 2;
            pSch->mcinfo.codingDescriptor = TURBOCDR;
            pSch->mcinfo.blockCodeConcatenation = 1;
            pSch->mcinfo.mcsType = pTst->mcs;
            pSch->harqInfo.nDi = (pTst->rvidx == 0) ? 1 : 1;
            pSch->harqInfo.rV = pTst->rvidx;
            pSch->harqInfo.flushReq = 0;
            pSch->crcInfo.crcLength = TWTYFOUR;
            pSch->crcInfo.crcScrambling = 0;
            pSch->scrInfo.scramblerType = DLSCR;
            pSch->scrInfo.scrinitValueinput = pTst->cellID;
            // Initialize to zero here and update from the mac layer at the time of
            // reading the data
            pSch->mapInfo.numberofEntries = 1;
            for (i = 0; i < pSch->mapInfo.numberofEntries; i++)
            {
                pSch->mapInfo.reselmInfo[i].startRes = 0;
                pSch->mapInfo.reselmInfo[i].numRes = 0;
            }
            break;
        case PRACH:
            break;
    }
}

//-------------------------------------------------------------------------------------------
/** @brief InitializeDLChannels: Initialize DLChannel structure for TxVector<BR>
 *                             for Pre-Defined Test Vectors.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pCh     = OUTPUT, Pointer to DLSubChannel structure of TxVector
 *  @param pTst    = INPUT,  Pointer to test structured used to populate SubChannel Structure
 *  @param id      = INPUT,  Channel ID
 *  @param sFrType = INPUT,  Frame Type, DLTX,DLRX,ULTX,or ULRX
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction   |Length  |Type          |Data Format |Valid Range |Units  |
 *  |           |Standard |            |        |              |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pCh       |         |pOutput     |1        |pStructure    |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pTst      |         |pInput      |1        |pStructure    |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |id        |         |Input       |1        |Real          |(0:32:0)    |[0:99]      |       |
 *  ----------------------------------------------------------------------------------------------
 *  |sFrType   |         |Input       |1        |Real          |(0:32:0)    |[0,3]       |       |
 *  ----------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |InitializeDLChannels        |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |pTst,id,sFrType             |
 *  ---------------------------------------------------
 *  |Status Parameters   |pCh                         |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Setup Channel Structure from TxVector based on pre-defined<BR>
 *  test vectors.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void InitializeDLChannels(PDLCHANDESC pCh, PTSTPDESC pTst, U32 id, U32 sFrType)
{
    U32 i;

    if (pTst->ul_dl_test_indicator == 0 && sFrType == DLTX)
    {
        pCh->txpowerControl = (S16) pTst->pchan[id]->chPower;

        pCh->subChInfo[0].modulationType = (pTst->pchan[id]->Qm) / 2; //cw1
        pCh->subChInfo[1].modulationType = (pTst->pchan[id]->Qm) / 2; //cw2
    }

    // For each channel type setup default plus parameters contained
    // in the TSTPDESC structure
    pCh->channelType = pTst->pchan[id]->ChanType;

    switch (pCh->channelType)
    {
        case PDSCH:
            pCh->channelId = (U16) id;
            pCh->persistEnable = 0x1;
            pCh->repeatCycle = 1;
            pCh->numCodeWords = pTst->nTxCodewords;
            pCh->nLayers = pTst->nTxLayers;
            pCh->transmissionMode = (U8) pTst->transmissionMode;
            pCh->crcInfo.crcLength = TWTYFOUR;
            pCh->crcInfo.crcScrambling = 0;
            pCh->scrInfo.scramblerType = DLSCR;
            pCh->scrInfo.scrinitValueinput = pTst->pchan[id]->nRNTI;
            pCh->dlPrecoderInfo.cddType = pTst->cddType;
            pCh->dlPrecoderInfo.codeBookIdx = pTst->codeBookIndex;
            pCh->distributedFlag = (U8) pTst->distributedFlag;
            pCh->nGapFlag = (U8) pTst->nGapFlag;

            for (i = 0; i < 8; i++)
            {
                pCh->dlPrecoderInfo.codebookSubsetRestriction[i] = 0;
            }

            if (pTst->mapEnable)
            {
                pCh->mapInfo.numberofEntries = pTst->pchan[id]->map.numberofEntries;

                for (i = 0; i < pCh->mapInfo.numberofEntries; i++)
                {
                    pCh->mapInfo.reselmInfo[i].startRes
                        = pTst->pchan[id]->map.reselmInfo[i].startRes;
                    pCh->mapInfo.reselmInfo[i].numRes = pTst->pchan[id]->map.reselmInfo[i].numRes;
                }
            }
            else
            {
                // Initialize to zero here and update from the mac layer at the time of
                // reading the data
                pCh->mapInfo.numberofEntries = 1;
                for (i = 0; i < pCh->mapInfo.numberofEntries; i++)
                {
                    pCh->mapInfo.reselmInfo[i].startRes = 0;
                    pCh->mapInfo.reselmInfo[i].numRes = 0;
                }
            }
            // Now set the subchannels depending on the number of codewords
            for (i = 0; i < pCh->numCodeWords; i++)
            {
                // Initialize the SubChannel info
                InitializeSubchannel(&pCh->subChInfo[i], pTst, id, (U32) pCh->channelType);
            }
            break;

        case PBCH:
            pCh->channelId = (U16) id;
            pCh->persistEnable = 0x1;
            pCh->repeatCycle = 1;
            pCh->numCodeWords = 1;
            pCh->nLayers = pTst->nTxLayers;
            pCh->transmissionMode = (U8) pTst->transmissionMode;
            if (pCh->nLayers > 1) // Multiple antenna
                pCh->transmissionMode = TXDIVERSITY;

            // Initialize the SubChannel info
            InitializeSubchannel(&pCh->subChInfo[0], pTst, id, (U32) pCh->channelType);

            pCh->crcInfo.crcLength = SIXTEEN;
            pCh->crcInfo.crcScrambling = 1;
            pCh->scrInfo.scramblerType = DLSCR;
            pCh->scrInfo.scrinitValueinput = pTst->cellID;
            pCh->dlPrecoderInfo.cddType = pTst->cddType;
            pCh->dlPrecoderInfo.codeBookIdx = pTst->codeBookIndex;

            for (i = 0; i < 8; i++)
            {
                pCh->dlPrecoderInfo.codebookSubsetRestriction[i] = 0;
            }

            // Initialize to zero here and update from the mac layer at the
            // time of reading the data
            pCh->mapInfo.numberofEntries = 1;
            for (i = 0; i < pCh->mapInfo.numberofEntries; i++)
            {
                pCh->mapInfo.reselmInfo[i].startRes = 0;
                pCh->mapInfo.reselmInfo[i].numRes = 0;
            }

            break;

        case PCFICH:
            pCh->channelId = (U16) id;
            pCh->persistEnable = 0x1;
            pCh->repeatCycle = 1;
            pCh->numCodeWords = 1;
            pCh->nLayers = pTst->nTxLayers;
            pCh->transmissionMode = (U8) pTst->transmissionMode;
            if (pCh->nLayers > 1) // Multiple antenna
                pCh->transmissionMode = TXDIVERSITY;

            //InitializeSubchannel(&pCh->subChInfo[0], pTst, id,
            //                     (U32) pCh->channelType);
            break;

        case PHICH:
            pCh->channelId = (U16) id;
            pCh->persistEnable = 0x1;
            pCh->repeatCycle = 1;
            pCh->numCodeWords = 1;
            pCh->nLayers = pTst->nTxLayers;
            pCh->transmissionMode = (U8) pTst->transmissionMode;
            if (pCh->nLayers > 1) // Multiple antenna
                pCh->transmissionMode = TXDIVERSITY;
            InitializeSubchannel(&pCh->subChInfo[0], pTst, id, (U32) pCh->channelType);

            pCh->crcInfo.crcLength = NONE;
            pCh->crcInfo.crcScrambling = 0;
            pCh->scrInfo.scramblerType = DLSCR;
            pCh->scrInfo.scrinitValueinput = pTst->cellID;
            pCh->dlPrecoderInfo.cddType = pTst->cddType;
            pCh->dlPrecoderInfo.codeBookIdx = pTst->codeBookIndex;

            for (i = 0; i < 8; i++)
            {
                pCh->dlPrecoderInfo.codebookSubsetRestriction[i] = 0;
            }

            // Initialize to zero here and update from the mac layer at the
            // time of reading the data
            pCh->mapInfo.numberofEntries = 1;
            for (i = 0; i < pCh->mapInfo.numberofEntries; i++)
            {
                pCh->mapInfo.reselmInfo[i].startRes = 0;
                pCh->mapInfo.reselmInfo[i].numRes = 0;
            }

            break;
    }

}

//-------------------------------------------------------------------------------------------
/** @brief InitializeDCIChannels: Initialize DCIChannels structure of RxVector or TxVector<BR>
 *                             for Pre-Defined Test Vectors.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pCh     = OUTPUT, Pointer to DCIChannel structure of TxVector or RxVector
 *  @param pTst    = INPUT,  Pointer to test structured used to populate DCIChannel Structure
 *  @param id      = INPUT,  Channel ID
 *  @param sFrType = INPUT,  Frame Type, DLTX,DLRX,ULTX,or ULRX
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction   |Length  |Type          |Data Format |Valid Range |Units  |
 *  |           |Standard |            |        |              |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pCh       |         |pOutput     |1        |pStructure    |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pTst      |         |pInput      |1        |pStructure    |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |id        |         |Input       |1        |Real          |(0:32:0)    |[0:99]      |       |
 *  ----------------------------------------------------------------------------------------------
 *  |sFrType   |         |Input       |1        |Real          |(0:32:0)    |[0,3]       |       |
 *  ----------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |InitializeDCIChannels       |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |pTst,id,sFrType             |
 *  ---------------------------------------------------
 *  |Status Parameters   |pCh                         |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Setup DCIChannel Structure from RxVector or TxVector based on pre-defined<BR>
 *  test vectors.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void InitializeDCIChannels(PDCICHANNELDESC pCh, PTSTPDESC pTst, U32 id, U32 sFrType)
{

    if (sFrType == DLTX)
    {
        pCh->txpowerControl = (S16) pTst->pchan[id]->chPower;
    }

    pCh->channelId = (U16) id;
    pCh->nLayers = pTst->nTxLayers;
    pCh->transmissionMode = (U8) pTst->transmissionMode;
    if (pCh->nLayers > 1) // Multiple antenna
        pCh->transmissionMode = TXDIVERSITY;

    pCh->numCodeWords = 1;

    pCh->numberofEntries = 1;
    pCh->startRes = (U8) (pTst->pchan[id]->startCCE);
    pCh->numRes = (U8) (pTst->pchan[id]->numCCE); // 0
    pCh->channelType = pTst->pchan[id]->ChanType;
    pCh->scrmblerInitValue = pTst->pchan[id]->nRNTI;

}

//-------------------------------------------------------------------------------------------
/** @brief InitializeULChannels: Initialize ULChannel structure of RxVector or TxVector<BR>
 *                             for Pre-Defined Test Vectors.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pCh     = OUTPUT, Pointer to ULSubChannel structure of TxVector or RxVector
 *  @param pTst    = INPUT,  Pointer to test structured used to populate SubChannel Structure
 *  @param id      = INPUT,  Channel ID
 *  @param sFrType = INPUT,  Frame Type, DLTX,DLRX,ULTX,or ULRX
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction   |Length  |Type          |Data Format |Valid Range |Units  |
 *  |           |Standard |            |        |              |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pCh       |         |pOutput     |1        |pStructure    |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pTst      |         |pInput      |1        |pStructure    |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |id        |         |Input       |1        |Real          |(0:32:0)    |[0:99]      |       |
 *  ----------------------------------------------------------------------------------------------
 *  |sFrType   |         |Input       |1        |Real          |(0:32:0)    |[0,3]       |       |
 *  ----------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |InitializeULChannels        |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |pTst,id,sFrType             |
 *  ---------------------------------------------------
 *  |Status Parameters   |pCh                         |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Setup Channel Structure from RxVector or TxVector based on pre-defined<BR>
 *  test vectors.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void InitializeULChannels(PULCHANDESC pCh, PTSTPDESC pTst, U32 id, U32 sFrType)
{

    // For each channel type setup default plus parameters contained
    // in the TSTPDESC structure
    int i;
    pCh->channelType = pTst->pchan[id]->ChanType;

    switch (pCh->channelType)
    {
        case PUSCH:
            pCh->channelId = (U16) id;
            pCh->txpowerControl = (S16) pTst->pchan[id]->chPower; //0;

            pCh->persistEnable = 0x1;
            pCh->repeatCycle = 1;
            pCh->halfIterations = pTst->numHalfIterations;
            // Initialize the SubChannel info
            InitializeULSubchannel(&pCh->ulSubChInfo, pTst, id, (U32) pCh->channelType);

            pCh->ulSubChInfo.puschDed.nDMRS2 = pTst->pchan[id]->n2_DMRS;

            if (pTst->mapEnable)
            {
                pCh->ulSubChInfo.mapInfo.numberofEntries = pTst->pchan[id]->map.numberofEntries;

                for (i = 0; i < pCh->ulSubChInfo.mapInfo.numberofEntries; i++)
                {
                    pCh->ulSubChInfo.mapInfo.reselmInfo[i].startRes
                        = pTst->pchan[id]->map.reselmInfo[i].startRes;
                    pCh->ulSubChInfo.mapInfo.reselmInfo[i].numRes
                        = pTst->pchan[id]->map.reselmInfo[i].numRes;
                }
            }
            break;

        case PRACH:
            break;
    }
}

//-------------------------------------------------------------------------------------------
/** @brief InitializeUCIChannels: Initialize UCIChannel structure of RxVector or TxVector<BR>
 *                             for Pre-Defined Test Vectors.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pCh     = OUTPUT, Pointer to UCIChannel structure of TxVector or RxVector
 *  @param pTst    = INPUT,  Pointer to test structured used to populate SubChannel Structure
 *  @param id      = INPUT,  Channel ID
 *  @param sFrType = INPUT,  Frame Type, DLTX,DLRX,ULTX,or ULRX
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction   |Length  |Type          |Data Format |Valid Range |Units  |
 *  |           |Standard |            |        |              |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pCh       |         |pOutput     |1        |pStructure    |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pTst      |         |pInput      |1        |pStructure    |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |id        |         |Input       |1        |Real          |(0:32:0)    |[0:99]      |       |
 *  ----------------------------------------------------------------------------------------------
 *  |sFrType   |         |Input       |1        |Real          |(0:32:0)    |[0,3]       |       |
 *  ----------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |InitializeUCIChannels       |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |pTst,id,sFrType             |
 *  ---------------------------------------------------
 *  |Status Parameters   |pCh                         |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Setup Channel Structure from RxVector or TxVector based on pre-defined<BR>
 *  test vectors.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void InitializeUCIChannels(PULCTRLCHDESC pCh, PTSTPDESC pTst, U32 id, U32 sFrType)
{
    int i;
    PCHCFG pChcfg = &chancfg[id];

    pCh->channelId = (U16) id;
    pCh->channelType = pChcfg->ChanType;
    pCh->txpowerControl = (S16) pChcfg->chPower;

    pCh->pucchDedCtl.formatType = pChcfg->format_type;
    pCh->crcLength = 0;
    pCh->scrmblerInitValue = 0;
    pCh->codingDescriptor = 0;
    pCh->blockCodeConcatenation = 0;
    pCh->modulationType = BPSK;
    pCh->mcsType = 0;
    pCh->numberofEntries = 1;
    pCh->startRes = 0;
    pCh->numRes = 0;

    if (pTst->mapEnable)
    {
        pCh->numberofEntries = pTst->pchan[id]->map.numberofEntries;

        for (i = 0; i < pCh->numberofEntries; i++)
        {
            pCh->startRes = pTst->pchan[id]->map.reselmInfo[i].startRes;
            pCh->numRes = pTst->pchan[id]->map.reselmInfo[i].numRes;
        }
    }

    pCh->pucchDedCtl.cqiPUCCHResourceIndex = pChcfg->n2_pucch;

}

//-------------------------------------------------------------------------------------------
/** @brief InitializeUCICommonCtlParameters: Initializes UCI common parameters<BR>
 *                             for Pre-Defined Test Vectors.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pSfr    = OUTPUT, Pointer to ULSUBFRDESC structure of TxVector or RxVector
 *  @param pTst    = INPUT,  Pointer to test structure used to populate SubChannel Structure
 *  @param id      = INPUT,  Channel ID
 *  @param sFrType = INPUT,  Frame Type, DLTX,DLRX,ULTX,or ULRX
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------
 *  |Name      |Name in  |Direction   |Length  |Type          |Data Format |Valid Range |Units  |
 *  |          |Standard |            |        |              |            |            |       |
 *  ---------------------------------------------------------------------------------------------
 *  |pSfr      |         |pOutput     |1       |pStructure    |            |            |       |
 *  ---------------------------------------------------------------------------------------------
 *  |pTst      |         |pInput      |1       |pStructure    |            |            |       |
 *  ---------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |InitializeUCICommonCtlParameters |
 *  -------------------------------------------------------
 *  |Input Streams       |none                             |
 *  -------------------------------------------------------
 *  |Output Streams      |none                             |
 *  -------------------------------------------------------
 *  |Config Parameters   |pTst                             |
 *  -------------------------------------------------------
 *  |Status Parameters   |pSfr                             |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Setup Channel Structure from RxVector or TxVector based on pre-defined<BR>
 *  test vectors.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------

void InitializeCommonULParameters(PULSUBFRDESC pSfr, PTSTPDESC pTst)
{
    U32 i = 0;
#ifdef FD_TEST
    PCHCFG pChcfg = &chancfg1[0];
#else
    PCHCFG pChcfg = &chancfg[0];
#endif

    pSfr->ulSfrCtrl.deltaPUCCHShift = pChcfg->delta_shift_PUCCH;
    pSfr->ulSfrCtrl.nRBCQI = pChcfg->N2_RB;
    pSfr->ulSfrCtrl.nCSAn = pChcfg->N1_CS;
    pSfr->ulCtlCh[0].pucchDedCtl.harqPucchIndex = pChcfg->n1_pucch;
    pSfr->ulSfrCtrl.nSB = 0;
    pSfr->ulSfrCtrl.hoppingMode = 0;
    pSfr->ulSfrCtrl.puschhoppingOffset = 0;
    pSfr->ulSfrCtrl.enable64QAM = 1;
    pSfr->ulSfrCtrl.groupHoppingEnabled = pChcfg->groupHoppingEnabled;
    pSfr->ulSfrCtrl.groupAssignmentPUSCH = pChcfg->DeltaSS;
    pSfr->ulSfrCtrl.sequenceHoppingEnabled = pChcfg->sequenceHoppingEnabled;
    pSfr->ulSfrCtrl.cyclicShift = pChcfg->cyclicShift;

    if (pTst->srsEnable)
    {
        pSfr->ulSfrCtrl.srsBandwitdhConfig = pTst->srsBandwidthConfig;
        pSfr->ulSfrCtrl.srsSubframeConfig = pTst->srsSubframeConfig;
        pSfr->ulSfrCtrl.ackNackSRSSimultaneousTransmission = pTst->srsSimultaneousANandSRS;
        pSfr->ulSfrCtrl.cyclicShift = pTst->pSrschan[i]->srs_cyclicShift;
    }
}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTestBsDlDefault: Configure default Initialization Message, TxVector,<BR>
 *                                   Mobile RxVector in the Downlink Direction for<BR>
 *                                   pre-defined test options
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm      = OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pParm1     = OUTPUT, Pointer to Initialization Message for Mobile Station
 *  @param txVector   = OUTPUT, Pointer to TxVector Message for Base Station
 *  @param rxMsVector = OUTPUT, Pointer to RxVector Message for Mobile Station
 *  @param tstpbs     = INPUT, Pointer to test configuration structure for Base Station
 *  @param tstpbs     = INPUT, Pointer to test configuration structure for Mobile Station
 *  @param tstOption  = INPUT, Test Option
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction   |Length  |Type          |Data Format |Valid Range |Units  |
 *  |           |Standard |            |        |              |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pParm      |         |pOutput     |1        |pStructure   |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pParm1     |         |pOutput     |1        |pStructure   |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |txVector   |         |pOutput     |1        |pStructure   |            |            |       |
 *  ---------------------------------------------------------------------------------------------
 *  |rxMsVector |         |pOutput     |1        |pStructure   |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |tstpbs     |         |Input       |1        |pStructure   |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |tstpms     |         |Input       |1        |pStructure   |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |tstOption  |         |Input       |1        |Real         |(0:31:0)    |[1:11]      |       |
 *  ----------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTestBsDlDefault        |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |tstpbs,tstpms,tstOption         |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pParm1,txVector,rxMsVector|
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure default Initialization Message, TxVector,<BR>
 *  Mobile RxVector in the Downlink Direction for<BR>
 *  pre-defined test options
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTestBsDlDefault(PINITPARM pParm, PINITPARM pParm1, PDLSUBFRDESC txVector,
                              PDLSUBFRDESC rxMsVector, PTSTPDESC tstpbs, PTSTPDESC tstpms,
                              U32 tstOption)
{
    Configure10MhzBw(pParm);
    Configure10MhzBw(pParm1);

    // BS
    pParm->txAntennaPortCount = 1;
    pParm->rxAntennaPortCount = 1;
    txVector->antennaPortcount = 1;
    tstpbs->transmissionMode = SINGLEANT;

    // MS
    pParm1->txAntennaPortCount = 1;
    pParm1->rxAntennaPortCount = 2;
    rxMsVector->antennaPortcount = 1;
    tstpms->transmissionMode = SINGLEANT;

    tstpbs->test_case = tstOption;
    tstpms->test_case = tstOption;

    tstpbs->nTxCodewords = 1;
    tstpbs->nTxLayers = 1;

    tstpms->nTxCodewords = 1;
    tstpms->nTxLayers = 1;

    // Now setup the default offsets used in the test

    txVector->offsetPowerCtrl = (U32) &txVector->dlCmnPwrCtl - (U32) txVector;
    txVector->offsetDCIChannels = (U32) &txVector->dciCh - (U32) txVector;
    txVector->offsetCustomFeatures = 0;
    rxMsVector->offsetPowerCtrl = (U32) &rxMsVector->dlCmnPwrCtl - (U32) rxMsVector;
    ;
    rxMsVector->offsetDCIChannels = (U32) &rxMsVector->dciCh - (U32) rxMsVector;
    rxMsVector->offsetCustomFeatures = 0;

}

//-------------------------------------------------------------------------------------------
/** @brief InitializeSrsChannels: Initialize SRS structure of RxVector<BR>
 *                                for Pre-Defined Test Vectors.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pSrs   = OUTPUT, Pointer to SRS structure of RxVector
 *  @param pTst   = INPUT,  Pointer to test structured used to populate SRS Structure
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction   |Length  |Type          |Data Format |Valid Range |Units  |
 *  |           |Standard |            |        |              |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pSrs       |         |pOutput     |1       |pStructure    |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput      |1       |pStructure    |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |InitializeSrsChannels       |
 *  ---------------------------------------------------
 *  |Input Streams       |none                        |
 *  ---------------------------------------------------
 *  |Output Streams      |none                        |
 *  ---------------------------------------------------
 *  |Config Parameters   |pTst                        |
 *  ---------------------------------------------------
 *  |Status Parameters   |pSrs                        |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Setup SRS Structure from RxVector for pre-defined<BR>
 *  test vectors.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void InitializeSrsChannels(PSRSDED pSrs, PSRSCHCFG pTst)
{
    pSrs->enableDisable = pTst->srs_enable;
    pSrs->cyclicShift = pTst->srs_cyclicShift;
    pSrs->srsBandwidth = pTst->srs_bandwidth;
    pSrs->srsHoppingBandwidth = pTst->srs_hoppingBandwidth;
    pSrs->freqDomainPosition = pTst->srs_freqDomainPosition;
    pSrs->duration = pTst->srs_duration;
    pSrs->srsConfigIndex = pTst->srs_configIndex;
    pSrs->transmissionComb = pTst->srs_transmissionComb;
}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTestBS_DL: Configure Common parameters of Base Station TxVector<BR>
 *                             for pre-defined test options
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to TxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction   |Length  |Type          |Data Format |Valid Range |Units  |
 *  |           |Standard |            |        |              |            |            |       |
 *  ----------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput     |1        |pStructure   |            |            |        |
 *  ----------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput    |1        |pStructure   |            |            |        |
 *  ----------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput     |1        |pStructure   |            |            |        |
 *  ----------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input      |1        |Real         |(0:32:0)    |[0:3]       |        |
 *  ----------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTestBS_DL              |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pSfr                            |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Common parameters of Base Station TxVector<BR>
 *  for pre-defined test options
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTestBS_DL(PINITPARM pParm, PDLSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    U32 i, j;
    PDCICHANNELDESC pdciChan;
    PDLCMNTXPWRCTL pdlcmnTxPwrCtl;
    // Configure first the init parameters that are
    // required for this test

    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = pTst->LayerCellIdGroup; //37;
    pParm->phyLayerId = pTst->LayerId; //0; //0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;	
    pSfr->subframeNumber = 0;

    pSfr->subframeType = (U8) sFrType;
    pSfr->numCtrlSymbols = pTst->numCtlSymbols;
    pSfr->phichResource = pTst->Ng;	
    pSfr->phichDuration = 1;

    //    pSfr->numberofChannelDescriptors= (pTst->numDataCh + pTst->numCtlCh);
    pSfr->numberofChannelDescriptors = (U8) (pTst->number_of_channels);
    pSfr->numberOfPhichChannels = (U16) pTst->number_of_phich_channels;
    pSfr->numCtrlChannels = pTst->numCtlCh;

    pdlcmnTxPwrCtl = (PDLCMNTXPWRCTL) ((U32) pSfr + pSfr->offsetPowerCtrl);
    pdlcmnTxPwrCtl->pilotPower = pTst->pilotPower;
    pdlcmnTxPwrCtl->psyncPower = pTst->psyncPower;
    pdlcmnTxPwrCtl->ssyncPower = pTst->ssyncPower;
    pdlcmnTxPwrCtl->ciPower = pTst->ciPower;
    //PRS support needs to be hooked from test app
    pdlcmnTxPwrCtl->prsRa = pTst->prsRa;
//    pdlcmnTxPwrCtl->paprControl = pTst->paprControl;
//    pdlcmnTxPwrCtl->paprThreshold = pTst->paprThreshold;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    //  pTst->rvidx = 3;
	
    pTst->codeBookIndex = 0;
    pTst->ul_dl_test_indicator = 0;
    j = 0;
    pdciChan = (PDCICHANNELDESC) ((U32) pSfr + pSfr->offsetDCIChannels);

    for (i = 0; i < pSfr->numberofChannelDescriptors; i++)
    {
        if ((pTst->pchan[i]->ChanType != PDCCH) && (pTst->pchan[i]->ChanType != PHICH))
        {
            InitializeDLChannels(&pSfr->dlCh[i - pSfr->numCtrlChannels], pTst, i, sFrType);
        }
        else
        {
            InitializeDCIChannels((pdciChan + j), pTst, i, sFrType);
            j++;
        }
    }

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTestBS_UL: Configure Base Station RxVector common<BR>
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTestBS_UL              |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pTst,sFrType                    |
 *  -------------------------------------------------------
 *  |Status Parameters   |pSfr                            |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 1.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTestBS_UL(PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    U32 i;
    PULCTRLCHDESC pulCtrlCh;

    if (pSfr->offsetULCtrlChannels != 0)
    {
        pulCtrlCh = (PULCTRLCHDESC) ((U32) pSfr + pSfr->offsetULCtrlChannels);
        for (i = 0; i < pTst->numCtlCh; i++)
        {
            InitializeUCIChannels(pulCtrlCh + i, pTst, i, sFrType);
        }
    }

    InitializeCommonULParameters(pSfr, pTst);

    for (i = pTst->numCtlCh; i < pSfr->numberofChannelDescriptors; i++)
    {
        InitializeULChannels(&pSfr->ulCh[i - pTst->numCtlCh], pTst, i, sFrType);
    }

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureULOffsets: Configure Base Station RxVector internal offset fields<BR>
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pSfr    = INPUT/OUTPUT, Pointer to RxVector Message for Base Station
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pInput/pOutput1         |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureULOffsets              |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |                                |
 *  -------------------------------------------------------
 *  |Status Parameters   |pSfr                            |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector internal offsets<BR>
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureULOffsets(PULSUBFRDESC pSfr)
{

    pSfr->offsetRachCtrlStruct = (U32) &pSfr->rachCtrl - (U32) pSfr;
    pSfr->offsetULCtrlChannels = (U32) &pSfr->ulCtlCh - (U32) pSfr;
    pSfr->offsetsrsInfo = (U32) &pSfr->srsInfo - (U32) pSfr;
    pSfr->offsetCustomFeatures = 0;
}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest1_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 1.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest1_UL               |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 1.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest1_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    PRACHCTRL prachCtrl;

    // Configure first the init parameters that are
    // required for this test
    ConfigureULOffsets(pSfr);
    prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);

    Configure5MhzBw(pParm);
    // Then configure the antennas
    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 2;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    pParm->srsSubframeConfig = 15;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 3;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 1;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest2_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 2.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest2_UL               |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 2.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest2_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    PRACHCTRL prachCtrl;

    // Configure first the init parameters that are
    // required for this test

    ConfigureULOffsets(pSfr);
    prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);

    Configure10MhzBw(pParm);
    // Then configure the antennas
    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 2;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    pParm->srsSubframeConfig = 15;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 3;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 2;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest4_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 4.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest4_UL               |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 4.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest4_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    PRACHCTRL prachCtrl;

    // Configure first the init parameters that are
    // required for this test

    ConfigureULOffsets(pSfr);
    prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);

    Configure10MhzBw(pParm);
    // Then configure the antennas
    pTst->number_of_channels = pTst->numCtlCh + pTst->numDataCh;

    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 2;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    pParm->srsSubframeConfig = 15;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 3;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 4;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest5_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 5.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest5_UL               |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 5.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest5_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    PRACHCTRL prachCtrl;

    // Configure first the init parameters that are
    // required for this test

    ConfigureULOffsets(pSfr);
    prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);

    Configure10MhzBw(pParm);
    // Then configure the antennas
    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 2;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    pParm->srsSubframeConfig = 15;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 3;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 5;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest6_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 6.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest6_UL               |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 6.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest6_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    PRACHCTRL prachCtrl;

    // Configure first the init parameters that are
    // required for this test

    ConfigureULOffsets(pSfr);
    prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);

    Configure10MhzBw(pParm);
    // Then configure the antennas
    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 2;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;
    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    pParm->srsSubframeConfig = 15;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 3;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 6;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest7_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 7.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest7_UL               |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 7.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest7_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    U32 i;
    PRACHCTRL prachCtrl;

    PSRSDED psrsInfo;

    // Configure first the init parameters that are
    // required for this test

    ConfigureULOffsets(pSfr);
    prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);
    psrsInfo = (PSRSDED) ((U32) pSfr + pSfr->offsetsrsInfo);

    Configure5MhzBw(pParm);
    // Then configure the antennas
    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 2;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 1;
        prachCtrl->prachEnable = 0;

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    if (pTst->srsEnable)
    {
        pSfr->numberSrsinSf = (U8) pTst->numSrs;
        pParm->srsBandwidthConfig = pTst->srsBandwidthConfig;
        pParm->srsSubframeConfig = pTst->srsSubframeConfig;
        pParm->srsSimultaneousANandSRS = pTst->srsSimultaneousANandSRS;
        //pSfr->numberOfSrsChannels = pTst->numSrs;
        for (i = 0; i < (U32) pTst->numSrs; i++)
        {
            InitializeSrsChannels(psrsInfo + i, &Srschancfg[i]);
        }
    }

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 3;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 7;
    pParm->srsBandwidthConfig = pTst->srsBandwidthConfig;
    pParm->srsSubframeConfig = pTst->srsSubframeConfig;
    pParm->srsSimultaneousANandSRS = pTst->srsSimultaneousANandSRS;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest8_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 8.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest8_UL               |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 8.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest8_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    U32 i;
    PRACHCTRL prachCtrl;
    PSRSDED psrsInfo;

    // Configure first the init parameters that are
    // required for this test

    ConfigureULOffsets(pSfr);

    prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);
    psrsInfo = (PSRSDED) ((U32) pSfr + pSfr->offsetsrsInfo);

    Configure3MhzBw(pParm);
    // Then configure the antennas
    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 2;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 1;
        prachCtrl->prachEnable = 0;

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    if (pTst->srsEnable)
    {
        pSfr->numberSrsinSf = (U8) pTst->numSrs;
        pParm->srsBandwidthConfig = pTst->srsBandwidthConfig;
        pParm->srsSubframeConfig = pTst->srsSubframeConfig;
        pParm->srsSimultaneousANandSRS = pTst->srsSimultaneousANandSRS;
        //pSfr->numberOfSrsChannels = pTst->numSrs;
        for (i = 0; i < (U32) pTst->numSrs; i++)
        {
            InitializeSrsChannels(psrsInfo + i, &Srschancfg[i]);
        }
    }
    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 3;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 8;
    pParm->srsBandwidthConfig = pTst->srsBandwidthConfig;
    pParm->srsSubframeConfig = pTst->srsSubframeConfig;
    pParm->srsSimultaneousANandSRS = pTst->srsSimultaneousANandSRS;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest9_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 9.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest9_UL               |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 9.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest9_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    PRACHCTRL prachCtrl;

    // Configure first the init parameters that are
    // required for this test

    ConfigureULOffsets(pSfr);

    Configure10MhzBw(pParm);
    // Then configure the antennas
    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 2;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;

        if (pSfr->offsetRachCtrlStruct != 0)
        {
            prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);
            prachCtrl->prachEnable = pTst->prachEnable;

            if (pTst->prachEnable)
            {

                pParm->prachConfigurationIndex = pTst->prachConfigurationIndex;
                pParm->prachFrequencyOffset = pTst->prachFrequencyOffset;
                pParm->prachHighSpeedFlag = pTst->prachHighSpeedFlag;
                pParm->prachCyclicShiftConfig = pTst->prachCyclicShiftConfig;
                pParm->prachRootSequenceIndex = pTst->prachRootSequenceIndex;
                prachCtrl->prachConfigIndex = pTst->prachConfigurationIndex;
                prachCtrl->rootSequenceIndex = pTst->prachRootSequenceIndex;
                prachCtrl->highSpeedFlag = pTst->prachHighSpeedFlag;
                prachCtrl->zeroCorrelationZoneConfig = pTst->prachCyclicShiftConfig;
                prachCtrl->prachFreqOffset = pTst->prachFrequencyOffset;
            }
        }

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        if (pSfr->offsetRachCtrlStruct != 0)
        {
            prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);

            prachCtrl->prachEnable = pTst->prachEnable;
        }
        else
        {
            pSfr->rachCtrl.prachEnable = 0;
        }

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    pParm->srsSubframeConfig = 15;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 3;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 9;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest92_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 92.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest92_UL               |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 9.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest92_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    PRACHCTRL prachCtrl;

    // Configure first the init parameters that are
    // required for this test

    ConfigureULOffsets(pSfr);

    Configure20MhzBw(pParm);
    // Then configure the antennas
    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 2;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;

        if (pSfr->offsetRachCtrlStruct != 0)
        {
            prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);
            prachCtrl->prachEnable = pTst->prachEnable;

            if (pTst->prachEnable)
            {

                pParm->prachConfigurationIndex = pTst->prachConfigurationIndex;
                pParm->prachFrequencyOffset = pTst->prachFrequencyOffset;
                pParm->prachHighSpeedFlag = pTst->prachHighSpeedFlag;
                pParm->prachCyclicShiftConfig = pTst->prachCyclicShiftConfig;
                pParm->prachRootSequenceIndex = pTst->prachRootSequenceIndex;
                prachCtrl->prachConfigIndex = pTst->prachConfigurationIndex;
                prachCtrl->rootSequenceIndex = pTst->prachRootSequenceIndex;
                prachCtrl->highSpeedFlag = pTst->prachHighSpeedFlag;
                prachCtrl->zeroCorrelationZoneConfig = pTst->prachCyclicShiftConfig;
                prachCtrl->prachFreqOffset = pTst->prachFrequencyOffset;
            }
        }

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        if (pSfr->offsetRachCtrlStruct != 0)
        {
            prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);

            prachCtrl->prachEnable = pTst->prachEnable;
        }
        else
        {
            pSfr->rachCtrl.prachEnable = 0;
        }

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    pParm->srsSubframeConfig = 15;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 3;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 92;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest10_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 10.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest10_UL               |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 11.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest10_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    PRACHCTRL prachCtrl;

    // Configure first the init parameters that are
    // required for this test

    ConfigureULOffsets(pSfr);
    prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);

    Configure10MhzBw(pParm);
    // Then configure the antennas
    pTst->number_of_channels = pTst->numCtlCh + pTst->numDataCh;

    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 2;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    pParm->srsSubframeConfig = 15;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 3;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 10;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}
//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest11_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 11.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest11_UL               |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 11.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest11_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    PRACHCTRL prachCtrl;

    // Configure first the init parameters that are
    // required for this test

    ConfigureULOffsets(pSfr);
    prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);

    Configure10MhzBw(pParm);
    // Then configure the antennas
    pTst->number_of_channels = pTst->numCtlCh + pTst->numDataCh;

    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 2;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    pParm->srsSubframeConfig = 15;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 3;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 11;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest12_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 12.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest12_UL               |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 11.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest12_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    PRACHCTRL prachCtrl;

    // Configure first the init parameters that are
    // required for this test

    ConfigureULOffsets(pSfr);
    prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);

    Configure10MhzBw(pParm);
    // Then configure the antennas
    pTst->number_of_channels = pTst->numCtlCh + pTst->numDataCh;

    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 2;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 4;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    pParm->srsSubframeConfig = 15;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 3;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 12;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest16_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 16.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest16_UL              |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 16.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest16_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    PRACHCTRL prachCtrl;

    // Configure first the init parameters that are
    // required for this test
    ConfigureULOffsets(pSfr);
    prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);

    Configure10MhzBw(pParm);
    // Then configure the antennas
    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 2;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    pParm->srsSubframeConfig = 15;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 0;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 1;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest30_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 30.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest30_UL              |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 30.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest30_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    PRACHCTRL prachCtrl;

    // Configure first the init parameters that are
    // required for this test
    ConfigureULOffsets(pSfr);
    prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);

    Configure10MhzBw(pParm);
    // Then configure the antennas
    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    pParm->srsSubframeConfig = 15;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 0;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 1;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest31_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 31.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest31_UL              |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 31.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest31_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    PRACHCTRL prachCtrl;

    // Configure first the init parameters that are
    // required for this test
    ConfigureULOffsets(pSfr);
    prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);

    Configure10MhzBw(pParm);
    // Then configure the antennas
    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    pParm->srsSubframeConfig = 15;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 0;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 1;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest32_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 32.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest32_UL              |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 32.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest32_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    PRACHCTRL prachCtrl;

    // Configure first the init parameters that are
    // required for this test
    ConfigureULOffsets(pSfr);
    prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);

    Configure10MhzBw(pParm);
    // Then configure the antennas
    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    pParm->srsSubframeConfig = 15;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 0;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 1;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest33_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 33.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest33_UL              |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 32.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest33_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    PRACHCTRL prachCtrl;

    // Configure first the init parameters that are
    // required for this test
    ConfigureULOffsets(pSfr);
    prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);

    Configure20MhzBw(pParm);
    // Then configure the antennas
    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    pParm->srsSubframeConfig = 15;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 0;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 1;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}

//-------------------------------------------------------------------------------------------
/** @brief ConfigureTest34_UL: Configure Base Station RxVector for pre-defined<BR>
 *                            test option 34.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param pParm   = INPUT/OUTPUT, Pointer to Initialization Message for Base Station
 *  @param pSfr    = OUTPUT, Pointer to RxVector Message for Base Station
 *  @param pTst    = INPUT, Pointer to test configuration structure
 *  @param sFrType = INPUT, Frame type, DLTX,DLRX,ULTX,or ULRX

 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pParm      |         |pInput/pOutput|1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pSfr       |         |pOutput       |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |pTst       |         |pInput        |1        |pStructure    |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |sFrType    |         |Input         |1        |Real          |(0:32:0)    |[0:3]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |ConfigureTest34_UL              |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |pParm,pTst,sFrType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |pParm,pSfr                      |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Configure Base Station RxVector for pre-defined<BR>
 *  test option 32.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void ConfigureTest34_UL(PINITPARM pParm, PULSUBFRDESC pSfr, PTSTPDESC pTst, U32 sFrType)
{
    PRACHCTRL prachCtrl;

    // Configure first the init parameters that are
    // required for this test
    ConfigureULOffsets(pSfr);
    prachCtrl = (PRACHCTRL) ((U32) pSfr + pSfr->offsetRachCtrlStruct);

    Configure20MhzBw(pParm);
    // Then configure the antennas
    if (sFrType == ULRX)
    {
        // BS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 2;
        pSfr->antennaPortcount = 1;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    else
    {
        // MS
        pParm->txAntennaPortCount = 1;
        pParm->rxAntennaPortCount = 1;
        pSfr->antennaPortcount = 2;
        pTst->nTxCodewords = 1;
        pTst->nTxLayers = 1;
        pTst->transmissionMode = SINGLEANT;
        pTst->srsEnable = 0;
        prachCtrl->prachEnable = 0;

    }
    // Then set up cell group and cell sector
    // so the cell id can be determined as
    // cellId= 3*phyLayerCellIdGroup + phyLayerId
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pTst->cellID = pParm->phyLayerCellIdGroup * 3 + pParm->phyLayerId;

    // Then configure test information for the subframe processing channel
    // configuration
    pSfr->frameNumber = 0;
    pSfr->subframeNumber = 0;
    pSfr->subframeType = (U8) sFrType;
    pSfr->numberofChannelDescriptors = (pTst->numDataCh + pTst->numCtlCh);
    pParm->srsSubframeConfig = 15;

    // Before channel configuration set up tst descriptor with this particular
    // test configuration parameters
    pTst->cddType = NOCDD;
    pTst->rvidx = 0;
    pTst->codeBookIndex = 0;

    pTst->ul_dl_test_indicator = 1;
    pTst->test_case = 1;

    ConfigureTestBS_UL(pSfr, pTst, sFrType);

}
//-------------------------------------------------------------------------------------------
/** @brief CheckResults: Check if output of pre-defined test option are<BR>
 *                       bit exact to reference.
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param tstOption = INPUT, Pre-Defined test option number.
 *  @param tstType   = INPUT, Test type, 0=Downlink, 1=Uplink
 *  @param tstMode   = INPUT, Test Mode (C-Code, Ceva-sim, EVM, etc..)
 *
 *  @return test failed or passed, 0=pass, non-zero=fail
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |tstOption  |         |Input         |1        |Real          |(0:32,0)    |[1:11]      |      |
 *  ------------------------------------------------------------------------------------------------
 *  |tstType    |         |Input         |1        |Real          |(0:32,0)    |[0:1]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  |tstMode    |         |Input         |1        |Real          |(0:32,0)    |[0:1]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |CheckResults                    |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |tstOption, tstType , tstMode    |
 *  -------------------------------------------------------
 *  |Status Parameters   |none                            |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  Check if output of pre-defined test option are<BR>
 *  bit exact to reference.
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
int CheckResults(U32 tstOption, U32 tstType, U32 tstMode, PFLAG_PASS_FAIL pFlagPassFail)
{
    int ret = 0;
    int rxbitsErr, fecErr, srsErr, prachErr, sinrErr, freqErr;

    rxbitsErr = 0;
    fecErr = 0;
    srsErr = 0;
    prachErr = 0;
    if (tstMode == 4) //floating point code
    {
        //For now, these are only tested in Floating Point code.
        sinrErr = 1;  //Initialize as FAIL
        freqErr = 1;  //Initialize as FAIL
    }
    else
    {
        sinrErr = 0;
        freqErr = 0;  
    }

    if (tstType)
    {
        //
        //Uplink
        //
        switch (tstOption)
        {
            case 1:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst1.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }
                fecErr += system("fc fecDecOut.txt fecDecOut_tst1.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst1.txt");
                    pFlagPassFail->Flag_PUSCH_SINR = sinrErr;
                    freqErr = system("fc BsFreqOffset.txt BsFreqOffset_tst1.txt");
                    pFlagPassFail->Flag_PUSCH_FREQ_OFFSET = freqErr;
                }
                break;
            case 2:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst2.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }
                fecErr += system("fc fecDecOut.txt fecDecOut_tst2.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst2.txt");
                    pFlagPassFail->Flag_PUSCH_SINR = sinrErr;
                    freqErr = system("fc BsFreqOffset.txt BsFreqOffset_tst2.txt");
                    pFlagPassFail->Flag_PUSCH_FREQ_OFFSET = freqErr;
                }
                break;
            case 4:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst4.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                if ((tstMode == 2) || (tstMode == 1) || (tstMode == 4))
                {
                    fecErr += system("fc fecCtrlOut.txt fecCtrlOut_tst4.txt");
                    pFlagPassFail->Flag_PUCCH = fecErr;
                }
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst4.txt");
                    pFlagPassFail->Flag_PUCCH_SINR = sinrErr;
                }
                break;

            case 5:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst5.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }
                if ((tstMode == 2) || (tstMode == 1) || (tstMode == 4))
                {
                    fecErr += system("fc fecCtrlOut.txt fecCtrlOut_tst5.txt");
                    pFlagPassFail->Flag_PUCCH = fecErr;
                }
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst5.txt");
                    pFlagPassFail->Flag_PUCCH_SINR = sinrErr;
                }
                break;

            case 6:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst6.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }
                if ((tstMode == 2) || (tstMode == 1) || (tstMode == 4))
                {
                    fecErr += system("fc fecCtrlOut.txt fecCtrlOut_tst6.txt");
                    pFlagPassFail->Flag_PUCCH = fecErr;
                }
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst6.txt");
                    pFlagPassFail->Flag_PUCCH_SINR = sinrErr;
                }
                break;
                // End modified to pass the FDD PUCCH test cases by Arraycomm
            case 7:
                //if (tstMode == 4)
                //{
                //    fecErr += system("fc BsFecDecOut.txt fecDecOut_tst7.txt");
                //    pFlagPassFail->Flag_PUSCH = fecErr;
                //}
                pFlagPassFail->Flag_PUSCH = -1;
                srsErr += system("fc srsChanEst.dat srsChanEst_tst7.txt");
                pFlagPassFail->Flag_SRS = srsErr;
                break;
            case 8:
//                if (tstMode != 4)
//                    ret += system("fc BsFecDecOut.txt fecDecOut_tst8.txt");
                srsErr += system("fc srsChanEst.dat srsChanEst_tst8.txt");
                pFlagPassFail->Flag_SRS = srsErr;
                break;
            case 9:
                if (tstMode != 4)
                {
                    fecErr += system("fc BsFecDecOut.txt fecDecOut_tst9.txt");
                    pFlagPassFail->Flag_PUSCH = fecErr;
                }
                prachErr += system("fc prachIndexProfile.dat prachIndexProfile_tst9.txt");
                pFlagPassFail->Flag_PRACH = prachErr;
                break;
            case 10:
                if ((tstMode == 2) || (tstMode == 1) || (tstMode == 4))
                {
                    fecErr += system("fc fecCtrlOut.txt fecCtrlOut_tst10.txt");
                    pFlagPassFail->Flag_PUCCH = fecErr;
                }
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst10.txt");
                    pFlagPassFail->Flag_PUCCH_SINR = sinrErr;
                }
                break;
            case 11:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst11.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                if ((tstMode == 2) || (tstMode == 1) || (tstMode == 4))
                {
                    fecErr += system("fc fecCtrlOut.txt fecCtrlOut_tst11.txt");
                    pFlagPassFail->Flag_PUCCH = fecErr;
                }
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst11.txt");
                    pFlagPassFail->Flag_PUCCH_SINR = sinrErr;
                }
                break;
            case 12:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst12.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                if ((tstMode == 2) || (tstMode == 1) || (tstMode == 4))
                {
                    fecErr += system("fc fecCtrlOut.txt fecCtrlOut_tst12.txt");
                    pFlagPassFail->Flag_PUCCH = fecErr;
                }
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst12.txt");
                    pFlagPassFail->Flag_PUCCH_SINR = sinrErr;
                }
                break;
            case 13:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst13.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc fecDecOut.txt fecDecOut_tst13.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                break;
            case 14:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst14.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc fecDecOut.txt fecDecOut_tst14.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                break;
            case 15:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst15.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc fecDecOut.txt fecDecOut_tst15.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                break;
            case 16:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst16.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc fecDecOut.txt macbytesin_ul_tst16.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst16.txt");
                    pFlagPassFail->Flag_PUSCH_SINR = sinrErr;
                    freqErr = system("fc BsFreqOffset.txt BsFreqOffset_tst16.txt");
                    pFlagPassFail->Flag_PUSCH_FREQ_OFFSET = freqErr;
                }
                break;
            case 17:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst17.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc fecDecOut.txt fecDecOut_tst17.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                break;
            case 18:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst18.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc fecDecOut.txt fecDecOut_tst18.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                break;
            case 19:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst19.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc fecDecOut.txt fecDecOut_tst19.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                break;
            case 20:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst20.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc fecDecOut.txt fecDecOut_tst20.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                break;
            case 21:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst21.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc fecDecOut.txt fecDecOut_tst21.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                break;
            case 22:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst22.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc fecDecOut.txt fecDecOut_tst22.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst22.txt");
                    pFlagPassFail->Flag_PUSCH_SINR = sinrErr;
                    freqErr = system("fc BsFreqOffset.txt BsFreqOffset_tst22.txt");
                    pFlagPassFail->Flag_PUSCH_FREQ_OFFSET = freqErr;
                }
                break;
            case 23:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst23.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc BsFecDecOut.txt fecDecOut_tst23.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst23.txt");
                    pFlagPassFail->Flag_PUSCH_SINR = sinrErr;
                    freqErr = system("fc BsFreqOffset.txt BsFreqOffset_tst23.txt");
                    pFlagPassFail->Flag_PUSCH_FREQ_OFFSET = freqErr;
                }
                break;
            case 24:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst24.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc BsFecDecOut.txt fecDecOut_tst24.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst24.txt");
                    pFlagPassFail->Flag_PUSCH_SINR = sinrErr;
                    freqErr = system("fc BsFreqOffset.txt BsFreqOffset_tst24.txt");
                    pFlagPassFail->Flag_PUSCH_FREQ_OFFSET = freqErr;
                }
                break;
			case 25:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst25.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                if ((tstMode == 2) || (tstMode == 1) || (tstMode == 4))
                {
                    fecErr += system("fc fecCtrlOut.txt fecCtrlOut_tst25.txt");
                    pFlagPassFail->Flag_PUCCH = fecErr;
                }
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst25.txt");
                    pFlagPassFail->Flag_PUCCH_SINR = sinrErr;
                }
                break;
            case 26:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst26.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                if ((tstMode == 2) || (tstMode == 1) || (tstMode == 4))
                {
                    //fecErr += system("fc fecCtrlOut.txt fecCtrlOut_tst26.txt");
                    //pFlagPassFail->Flag_PUCCH = fecErr;
                }
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst26.txt");
                    pFlagPassFail->Flag_PUCCH_SINR = sinrErr;
                }
                break;
            case 27:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst27.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                if ((tstMode == 2) || (tstMode == 1) || (tstMode == 4))
                {
                    fecErr += system("fc fecCtrlOut.txt fecCtrlOut_tst27.txt");
                    pFlagPassFail->Flag_PUCCH = fecErr;
                }
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst27.txt");
                    pFlagPassFail->Flag_PUCCH_SINR = sinrErr;
                }
                break;
            case 28:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst28.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                if ((tstMode == 2) || (tstMode == 1) || (tstMode == 4))
                {
                    fecErr += system("fc fecCtrlOut.txt fecCtrlOut_tst28.txt");
                    pFlagPassFail->Flag_PUCCH = fecErr;
                }
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst28.txt");
                    pFlagPassFail->Flag_PUCCH_SINR = sinrErr;
                }
                break;
             case 29:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst29.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                if ((tstMode == 2) || (tstMode == 1) || (tstMode == 4))
                {
                    fecErr += system("fc fecCtrlOut.txt fecCtrlOut_tst29.txt");
                    pFlagPassFail->Flag_PUCCH = fecErr;
                }
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst29.txt");
                    pFlagPassFail->Flag_PUCCH_SINR = sinrErr;
                }
                break;
            case 30:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst30.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst30.txt");
                    pFlagPassFail->Flag_PUSCH_SINR = sinrErr;
                    freqErr = system("fc BsFreqOffset.txt BsFreqOffset_tst30.txt");
                    pFlagPassFail->Flag_PUSCH_FREQ_OFFSET = freqErr;
                }
                break;

            case 31:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst31.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc fecDecOut.txt macbytesin_ul_tst16.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst31.txt");
                    pFlagPassFail->Flag_PUSCH_SINR = sinrErr;
                    freqErr = system("fc BsFreqOffset.txt BsFreqOffset_tst31.txt");
                    pFlagPassFail->Flag_PUSCH_FREQ_OFFSET = freqErr;
                }
                break;

            case 32:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst32.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc fecDecOut.txt macbytesin_2385.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst32.txt");
                    pFlagPassFail->Flag_PUSCH_SINR = sinrErr;
                    freqErr = system("fc BsFreqOffset.txt BsFreqOffset_tst32.txt");
                    pFlagPassFail->Flag_PUSCH_FREQ_OFFSET = freqErr;
                }
                break;
            case 33:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst33.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc BsFecDecOut.txt fecDecOut_tst33.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst33.txt");
                    pFlagPassFail->Flag_PUSCH_SINR = sinrErr;
                }
                break;
            case 34:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst34.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc BsFecDecOut.txt fecDecOut_tst34.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst34.txt");
                    pFlagPassFail->Flag_PUSCH_SINR = sinrErr;
                }
                break;
            case 35:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst35.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc fecDecOut.txt macbytesin_ul_tst16.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst35.txt");
                    pFlagPassFail->Flag_PUSCH_SINR = sinrErr;
                    freqErr = system("fc BsFreqOffset.txt BsFreqOffset_tst35.txt");
                    pFlagPassFail->Flag_PUSCH_FREQ_OFFSET = freqErr;
                }
                break;

            case 40:
                fecErr += system("fc fecDecOut.txt fecDecOut_tst40.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst40.txt");
                    pFlagPassFail->Flag_PUSCH_SINR = sinrErr;
                    freqErr = system("fc BsFreqOffset.txt BsFreqOffset_tst40.txt");
                    pFlagPassFail->Flag_PUSCH_FREQ_OFFSET = freqErr;
                }
                break;
            case 48:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst48.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }
//                fecErr += system("fc fecDecOut.txt macbytesin_ul_tst16.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                break;
            case 75:
                if (tstMode != 4)
                {
                    rxbitsErr += system("fc rxbits.dat rxbits_tst75.txt");
                    pFlagPassFail->Flag_UL_RXBITS = rxbitsErr;
                }

                fecErr += system("fc fecDecOut.txt fecDecOut_tst75.txt");
                pFlagPassFail->Flag_PUSCH = fecErr;
                if (tstMode == 4)
                {
                    sinrErr = system("fc BsSinr.txt BsSinr_tst75.txt");
                    pFlagPassFail->Flag_PUSCH_SINR = sinrErr;
                    freqErr = system("fc BsFreqOffset.txt BsFreqOffset_tst75.txt");
                    pFlagPassFail->Flag_PUSCH_FREQ_OFFSET = freqErr;
                }
                break;

            case 92:
                if (tstMode != 4)
                {
                    fecErr += system("fc BsFecDecOut.txt fecdecout_tst92.txt");
                    pFlagPassFail->Flag_PUSCH = fecErr;
                }
                prachErr += system("fc prachIndexProfile.dat prachIndexProfile_tst92.txt");
                pFlagPassFail->Flag_PRACH = prachErr;
                break;
            default:
                break;
        }

        ret = rxbitsErr + 2 * fecErr + 4 * srsErr + 8 * prachErr + 16 * sinrErr + 32 * freqErr;
    }
    else
    {
        switch (tstOption)
        {
            case 3:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst3.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;

            case 4:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst4.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;

            case 5:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst5.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;

            case 6:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst6.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;

            case 7:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst7.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;

            case 8:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst8.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;

            case 9:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst9.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;

            case 10:
                ret += system("fc DL_IQ_OUT.d DL_IQ_OUT_tst10.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;

            case 11:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst11.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;

            case 12:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst12.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;

            case 13:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst13.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;

            case 14:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst14.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 15:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst15.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 16:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst16.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 18:
                ret += system("fc DL_IQ_OUT.d DL_IQ_OUT_tst18.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 20:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst20.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 22:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst22.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 23:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst23.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 24:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst24.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 25:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst25.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 30:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst30.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 31:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst31.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 34:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst34.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 35:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst35.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 110:
                ret += system("fc DL_IQ_OUT.d DL_IQ_OUT_tst110.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 111:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst111.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 112:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst112.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 113:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst113.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 115:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst115.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 11511:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst11511.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 123:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst123.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 205:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst205.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 313:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst313.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 323:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst323.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            case 333:
                ret += system("fc BsIQ_Outsamps.txt DL_IQ_OUT_tst333.d");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
           case 50:
                ret += system("fc BsIQ_Outsamps.txt BsIQ_Outsamps_tst50.txt");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
           case 51:
                ret += system("fc BsIQ_Outsamps.txt BsIQ_Outsamps_tst51.txt");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
           case 52:
                ret += system("fc BsIQ_Outsamps.txt BsIQ_Outsamps_tst52.txt");
                pFlagPassFail->Flag_PDSCH_IQ = ret;
                break;
            default:
                //ret += system("fc DL_IQ_OUT.d DL_IQ_OUT_tst4.d");
                break;
        }
    }
    return ret;
}

/** @brief InitializeFilesPerTestOption: This function initializes the <BR>
 *                                       Input IQ Samples File information <BR>
 *                                       and the Input Mac Data File information <BR>
 *                                       per TestOption and Test Type
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param tstOption = INPUT, Pre-Defined test option number.
 *  @param tstType   = INPUT, Test type, 0=Downlink, 1=Uplink
 *  @param inFile    = OUTPUT, File pointer to reference IQ Samples
 *  @param inDFile   = OUTPUT, File pointer to reference MAC Bytes input files
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |tstOption  |         |Input         |1        |Real          |(0:32,0)    |[1:11]      |      |
 *  ------------------------------------------------------------------------------------------------
 *  |tstType    |         |Input         |1        |Real          |(0:32,0)    |[0:1]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  |inFile     |         |pOutput       |1        |file pointer  |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |inDFile    |         |pOutput       |1        |file pointer  |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |InitializeFilesPerTestOption    |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |tstOption, tstType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |inFile,inDFile                  |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  This function initializes the Input IQ Samples File information <BR>
 *  and the Input Mac Data File information per TestOption and Test <BR>
 *  Type
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void InitializeFilesPerTestOption(U32 tstOption, U32 tstType, FILE **inFile, FILE **inDFile)
{
    if (tstType)
    {
        switch (tstOption)
        {
            case 1:
                *inFile = fopen("adc_bs_nonoise.txt", "rt");
                *inDFile = fopen("macbytesin_103.txt", "rt");
                break;
            default:
            case 2:
                *inFile = fopen("adc_bs_nonoise_ul_2.txt", "rt");
                *inDFile = fopen("macbytesin_1753.txt", "rt");
                break;
            case 4:
                *inFile = fopen("adc_pucch_f2_nonoise.txt", "rt");
                break;
            case 5:
                *inFile = fopen("adc_pucch_f2A_1_nonoise.txt", "rt");
                break;
            case 6:
                *inFile = fopen("adc_pucch_f2B_01_nonoise.txt", "rt");
                break;
            case 7:
                *inFile = fopen("adc_ul_tst7.txt", "rt");
                *inDFile = fopen("MacBytesin_ul_tst7.txt", "rt");
                break;
            case 8:
                *inFile = fopen("adc_ul_tst8.txt", "rt");
                *inDFile = fopen("MacBytesin_ul_tst8.txt", "rt");
                break;
            case 9:
                *inFile = fopen("adc_ul_tst9.txt", "rt");
                *inDFile = fopen("MacBytesin_ul_tst9.txt", "rt");
                break;
            case 10:
                *inFile = fopen("adc_pucch_f1_nonoise.txt", "rt");
                break;
            case 11:
                *inFile = fopen("adc_pucch_f1A_nonoise.txt", "rt");
                break;
            case 12:
                *inFile = fopen("adc_pucch_f1B_nonoise.txt", "rt");
                break;
            case 16:
                *inFile = fopen("adc_ul_tst16.txt", "rt");
                *inDFile = fopen("MacBytesin_ul_tst16.txt", "rt");
                break;

            case 30:
                *inFile = fopen("adc_ul_tst30.txt", "rt");
                *inDFile = fopen("MacBytesin_549.txt", "rt");
                break;

            case 31:
                *inFile = fopen("adc_ul_tst31.txt", "rt");
                *inDFile = fopen("MacBytesin_ul_tst16.txt", "rt");
                break;

            case 32:
                *inFile = fopen("adc_ul_tst32.txt", "rt");
                *inDFile = fopen("MacBytesin_2385.txt", "rt");
                break;

            case 33:
                *inFile = fopen("adc_ul_tst33.txt", "rt");
                *inDFile = fopen("MacBytesin_ul_tst16.txt", "rt");
                break;

            case 34:
                *inFile = fopen("adc_ul_tst34.txt", "rt");
                *inDFile = fopen("MacBytesin_ul_tst16.txt", "rt");
                break;
            case 35:
                *inFile = fopen("adc_ul_tst35.txt", "rt");
                *inDFile = fopen("MacBytesin_ul_tst16.txt", "rt");
                break;
            case 92:
                *inFile = fopen("PrachData20MHz_tst92.txt", "rt");
                *inDFile = fopen("MacBytesIn_ul_tst92.txt", "rt");
                break;

        }
    }
    else
    {
        switch (tstOption)
        {
            case 3:
                *inFile = fopen("adc_nonoise_tst3.txt", "rt");
                *inDFile = fopen("macbytes203.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BR.txt", "rt");
                break;
            case 4:
            default:
                *inFile = fopen("adc_nonoise_tst4.txt", "rt");
                *inDFile = fopen("macbytes1419.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BR.txt", "rt");
                break;
            case 5:
                *inFile = fopen("adc_nonoise_tst5.txt", "rt");
                *inDFile = fopen("macbytes1419.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BR.txt", "rt");
                break;
            case 6:
            case 10:
                *inFile = fopen("adc_nonoise_tst6.txt", "rt");
                *(inDFile + 0) = fopen("macbytesin_6_1.txt", "rt");
                *(inDFile + 1) = fopen("macbytesin_6_3.txt", "rt");
                *(inDFile + 2) = fopen("macbytesin_6_4.txt", "rt");
                *(inDFile + 3) = fopen("macbytesin_6_5.txt", "rt");
                *(inDFile + 4) = fopen("macbytesin_6_6.txt", "rt");
                *(inDFile + 5) = fopen("macbytesin_6_7.txt", "rt");
                *(inDFile + 6) = fopen("macbytes_18_BR.txt", "rt");
                break;
            case 7:
                *inFile = fopen("adc_nonoise_tst7.txt", "rt");
                break;
            case 8:
                *inFile = fopen("adc_nonoise_tst8.txt", "rt");
                *inDFile = fopen("macbytes_18_BR.txt", "rt");
                break;
            case 9:
                *inFile = fopen("adc_nonoise_tst9.txt", "rt");
                break;
            case 11:
                *inFile = fopen("adc_nonoise_tst11.txt", "rt");
                *(inDFile + 0) = fopen("macbytes1419.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BR.txt", "rt");
                break;
            case 12:
                *inFile = fopen("adc_nonoise_tst12.txt", "rt");
                break;
            case 13:
                *inFile = fopen("adc_nonoise_tst13.txt", "rt");
                break;
            case 14:
                *inFile = fopen("adc_nonoise_tst14.txt", "rt");
                break;
            case 15:
                *inFile = fopen("adc_nonoise_tst15.txt", "rt");
                *(inDFile + 0) = fopen("macbytes8910.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BR.txt", "rt");
                break;
            case 16:
                *inFile = fopen("adc_nonoise_tst16.txt", "rt");
                *(inDFile + 0) = fopen("macbytes4455.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BR.txt", "rt");
                break;
            case 18:
                *inFile = fopen("adc_nonoise_tst18.txt", "rt");
                break;
            case 30:
                *inFile = fopen("adc_nonoise_tst16.txt", "rt");
                *(inDFile + 0) = fopen("macbytesin_549.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BR.txt", "rt");
                break;
            case 31:
                *inFile = fopen("adc_nonoise_tst16.txt", "rt");
                *(inDFile + 0) = fopen("macbytes4455.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BR.txt", "rt");
                break;
            case 34:
                *inFile = fopen("adc_nonoise_tst5.txt", "rt");
                *inDFile = fopen("macbytes18722.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BR.txt", "rt");
                break;
            case 35:
                *inFile = fopen("adc_nonoise_tst5.txt", "rt");
                *inDFile = fopen("macbytes10922.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BR.txt", "rt");
                break;
            case 110:
                *inFile = fopen("adc_nonoise_tst110.txt", "rt");
                *(inDFile + 0) = fopen("macbytes117.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BRZ.txt", "rt");
                break;
            case 111:
                *inFile = fopen("adc_nonoise_tst111.txt", "rt");
                *(inDFile + 0) = fopen("macbytes293.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BRZ.txt", "rt");
                break;
            case 112:
                *inFile = fopen("adc_nonoise_tst112.txt", "rt");
                *(inDFile + 0) = fopen("macbytes501.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BRZ.txt", "rt");
                break;
            case 113:
                *inFile = fopen("adc_nonoise_tst113.txt", "rt");
                *(inDFile + 0) = fopen("macbytes999.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BRZ.txt", "rt");
                break;
            case 115:
            case 11511:
                *inFile = fopen("adc_nonoise_tst11511.txt", "rt");
                *(inDFile + 0) = fopen("macbytes1980.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BRZ.txt", "rt");
                break;
            case 123:
                *inFile = fopen("adc_nonoise_tst123.txt", "rt");
                *(inDFile + 0) = fopen("macbytes389.txt", "rt");
                *(inDFile + 1) = fopen("macbytes597.txt", "rt");
                *(inDFile + 2) = fopen("macbytes_18_BRZ.txt", "rt");
                break;
            case 205:
                *inFile = fopen("adc_nonoise_tst205.txt", "rt");
                *(inDFile + 0) = fopen("macbytes89.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BRZ.txt", "rt");
                break;
            case 313:
                *inFile = fopen("adc_nonoise_tst313.txt", "rt");
                *(inDFile + 0) = fopen("macbytes4587.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_18_BRZ.txt", "rt");
                break;
            case 323:
                *inFile = fopen("adc_nonoise_tst323.txt", "rt");
                *(inDFile + 0) = fopen("macbytes1143.txt", "rt");
                *(inDFile + 1) = fopen("macbytes389.txt", "rt");
                *(inDFile + 2) = fopen("macbytes_18_BRZ.txt", "rt");
                break;
            case 333:
                *inFile = fopen("adc_nonoise_tst333.txt", "rt");
                *(inDFile + 0) = fopen("macbytes501.txt", "rt");
                *(inDFile + 1) = fopen("macbytes967.txt", "rt");
                *(inDFile + 2) = fopen("macbytes_18_BRZ.txt", "rt");
                break;
        }
    }
}

/** @brief InitializeFilesPerTestOptionCTL:
 *  This function initializes the Input Mac Data File information <BR>
 *  per TestOption and Test Type for all Control Channels
 *
 *  <1> Hardware Resource:
 *      Host
 *
 *  @param tstOption = INPUT, Pre-Defined test option number.
 *  @param tstType   = INPUT, Test type, 0=Downlink, 1=Uplink
 *  @param inDFile   = OUTPUT, File pointer to reference MAC Bytes input files
 *
 *  @return none
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type          |Data Format |Valid Range |Units |
 *  |           |Standard |              |         |              |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  |tstOption  |         |Input         |1        |Real          |(0:32,0)    |[1:11]      |      |
 *  ------------------------------------------------------------------------------------------------
 *  |tstType    |         |Input         |1        |Real          |(0:32,0)    |[0:1]       |      |
 *  ------------------------------------------------------------------------------------------------
 *  |inDFile    |         |pOutput       |1        |file pointer  |            |            |      |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------------
 *  |Block Name          |InitializeFilesPerTestOptionCTL |
 *  -------------------------------------------------------
 *  |Input Streams       |none                            |
 *  -------------------------------------------------------
 *  |Output Streams      |none                            |
 *  -------------------------------------------------------
 *  |Config Parameters   |tstOption, tstType              |
 *  -------------------------------------------------------
 *  |Status Parameters   |inDFile                         |
 *  -------------------------------------------------------
 *  BLOCK_END
 *
 *  <4> Detailed Description:
 *  This function initializes the Input Mac Data File information
 *  per TestOption and Test Type for all Control Channels
 *
 *  <5> Cycle Count Formula:
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *  \ingroup group_lte_api
 */
//-------------------------------------------------------------------------------------------
void InitializeFilesPerTestOptionCTL(U32 tstOption, U32 tstType, FILE **inDFile)
{
    if (tstType)
    {
        switch (tstOption)
        {
            case 4:
                *inDFile = fopen("macbytesin_1753.txt", "rt");
                break;
            case 5:
                *inDFile = fopen("macbytesin_1753.txt", "rt");
                break;
            case 6:
                *inDFile = fopen("macbytesin_1753.txt", "rt");
                break;
            default:
                break;
        }
    }
    else
    {
        switch (tstOption)
        {
            case 7:
            default:
                *inDFile = fopen("macbytes_128_ctl.txt", "rt");
                break;
            case 9:
            case 11:
                *inDFile = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 2) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 3) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 4) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 5) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 6) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 7) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 8) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 9) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 10) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 11) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 12) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 13) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 14) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 15) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 16) = fopen("macbytes_128_ctl.txt", "rt");
                break;

            case 12:
                *inDFile = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 2) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 3) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 4) = fopen("macbytes_28_ctl.txt", "rt");
                break;

            case 13:
                *inDFile = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 2) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 3) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 4) = fopen("macbytes_4_ctl.txt", "rt");
                break;

            case 14:
                *inDFile = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 2) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 3) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 4) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 5) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 6) = fopen("macbytes_58_ctl.txt", "rt");
                break;
            case 110:
            case 111:
                *inDFile = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_2_phich.txt", "rt");

                *(inDFile + 2) = fopen("macbytes_4_ctl.txt", "rt");
                *(inDFile + 3) = fopen("macbytes_4_ctl.txt", "rt");
                break;
            case 112:
                *inDFile = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_2_phich.txt", "rt");

                *(inDFile + 2) = fopen("macbytes_4_ctl.txt", "rt");
                *(inDFile + 3) = fopen("macbytes_4_ctl.txt", "rt");
                break;

            case 16:
                *inDFile = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 2) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 3) = fopen("macbytes_2_phich.txt", "rt");

                *(inDFile + 4) = fopen("macbytes_4_ctl_NZ1.txt", "rt");
                *(inDFile + 5) = fopen("macbytes_4_ctl_NZ2.txt", "rt");
                *(inDFile + 6) = fopen("macbytes_4_ctl_NZ3.txt", "rt");
                *(inDFile + 7) = fopen("macbytes_4_ctl_NZ4.txt", "rt");
                *(inDFile + 8) = fopen("macbytes_4_ctl_NZ5.txt", "rt");

                break;

            case 18:
                *inDFile = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 2) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 3) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 4) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 5) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 6) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 7) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 8) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 9) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 10) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 11) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 12) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 13) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 14) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 15) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 16) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 17) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 18) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 19) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 20) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 21) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 22) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 23) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 24) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 25) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 26) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 27) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 28) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 29) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 30) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 31) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 32) = fopen("macbytes_128_ctl.txt", "rt");
                break;

            case 30:
                *inDFile = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 2) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 3) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 4) = fopen("macbytes_4_ctl_NZ1.txt", "rt");

                break;

            case 31:
                *inDFile = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 2) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 3) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 4) = fopen("macbytes_4_ctl_NZ1.txt", "rt");

                break;
            case 34:
                *inDFile = fopen("macbytes_4_ctl.txt", "rt");
                break;
            case 35:
                *inDFile = fopen("macbytes_4_ctl.txt", "rt");
                break;

            case 113:
            case 123:
            case 313:
            case 323:
            case 333:
                *inDFile = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 2) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 3) = fopen("macbytes_2_phich.txt", "rt");

                *(inDFile + 4) = fopen("macbytes_4_ctl.txt", "rt");
                *(inDFile + 5) = fopen("macbytes_4_ctl.txt", "rt");
                *(inDFile + 6) = fopen("macbytes_4_ctl.txt", "rt");
                *(inDFile + 7) = fopen("macbytes_4_ctl.txt", "rt");
                *(inDFile + 8) = fopen("macbytes_4_ctl.txt", "rt");

                break;

            case 15:
                *inDFile = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 2) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 3) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 4) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 5) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 6) = fopen("macbytes_4_ctl_NZ1.txt", "rt");
                *(inDFile + 7) = fopen("macbytes_4_ctl_NZ2.txt", "rt");
                *(inDFile + 8) = fopen("macbytes_4_ctl_NZ3.txt", "rt");
                *(inDFile + 9) = fopen("macbytes_4_ctl_NZ4.txt", "rt");
                *(inDFile + 10) = fopen("macbytes_4_ctl_NZ5.txt", "rt");
                *(inDFile + 11) = fopen("macbytes_4_ctl_NZ6.txt", "rt");
                break;

            case 115:
            case 11511:
                *inDFile = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 2) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 3) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 4) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 5) = fopen("macbytes_2_phich.txt", "rt");

                *(inDFile + 6) = fopen("macbytes_4_ctl.txt", "rt");
                *(inDFile + 7) = fopen("macbytes_4_ctl.txt", "rt");
                *(inDFile + 8) = fopen("macbytes_4_ctl.txt", "rt");
                *(inDFile + 9) = fopen("macbytes_4_ctl.txt", "rt");
                *(inDFile + 10) = fopen("macbytes_4_ctl.txt", "rt");
                *(inDFile + 11) = fopen("macbytes_4_ctl.txt", "rt");
                break;

            case 205:
                *inDFile = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 1) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 2) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 3) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 4) = fopen("macbytes_2_phich.txt", "rt");
                *(inDFile + 5) = fopen("macbytes_2_phich.txt", "rt");

                *(inDFile + 6) = fopen("macbytes_4_ctl.txt", "rt");
                break;

        }
    }
}

