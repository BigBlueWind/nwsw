//-------------------------------------------------------------------------------------------
/** @file LteBsRx.c
 *
 * @brief This file implements the receiver function for an LTE
 *        Base Station
 * @author Mindspeed Technologies
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
#include "prototypes.h"
#include "ext_phy.h"
#include "gputil_fx.h"
#include "lte_bs_phy_nmm_fdd.h"

#ifndef _WIN32
#include "mdmadrv.h"
#include "mlog.h"
#endif

extern U16 UnCoded_Bits[4 * 8 * MAX_INPUT_TRANSPORT_BYTE_SIZE];
extern U32 BS_OutSize1;
extern U32 BS_OutSize2;
extern U32 test_case;

#ifdef _WIN32
//-------------------------------------------------------------------------------------------
/** @brief Implements the receiver for an LTE Base Station
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Defines the particular PHY that is going to perform the
 *                     processing of the messages and input samples
 *  @param sigIn
 *                     Pointer to the Input IQ samples memory
 *  @param outMsg
 *                     Pointer to the output LTE L2-PHY API Message
 *  @param outMsgSize
 *                     Pointer to Output LTE L2-PHY message size in long words
 *
 *  @return status     A value of zero indicate success a value of 1 indicates
 *                     failure (We can expand to indicate the specific failure
 *                     reason)
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name              |Name in  |Direction |Length |Type         |Data Format|Valid Range |Units       |
 *  |                  |standard |          |       |             |           |            |            |
 *  -----------------------------------------------------------------------------------------------------
 *  |phyInstance       |         |Input     |1      |unsigned int |           |[0:2]       |            |
 *  -----------------------------------------------------------------------------------------------------
 *  |sigIn             |         |pInput    |1      |complex      |(1:1:14)   |(0,2^32)    |            |
 *  -----------------------------------------------------------------------------------------------------
 *  |outMsg            |         |pOutput   |1      |punsigned int|(1:31:0)   |(0,2^32)    |            |
 *  -----------------------------------------------------------------------------------------------------
 *  |outMsgSize        |         |Output    |1      |unsigned int |(1:31:0)   |(0,2^32)    |Bytes       |
 *  -----------------------------------------------------------------------------------------------------
 *  |status            |         |Output    |1      |unsigned int |(1:31:0)   |[0,2^32)    |            |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteBsRx                     |
 *  ---------------------------------------------------
 *  |Input Streams       |sigIn                       |
 *  ---------------------------------------------------
 *  |Output Streams      |outMsg,outMsgSize           |
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
 * This function is the top level entry to the BS receiver, it also provides acks to the L2 using
 * an outMsg
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * 3GPP 36.211
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------

extern U32             nmm_subframe_count;
extern U32             nmm_proc_subframe;
extern U8              *pGlobalNMMOutApiMsg;
extern U32             *globalNMMoutApiMsgLen;
#define MAX_N_PROC_SUBFRAME  10

extern void lte_bs_phy_nmm_update_subframe_count(U32 N_buf_subframe);
extern void lte_bs_phy_nmm_check_proc_flag(U16 *proc_flag);

U32 LteBsRx(U32 phyInstance, S16* sigIn, U32* outMsg, U32* outMsgSize)
{
    U32 status = 0;
    PLTEPHYDESC pPhy = getPhyCtxApiProc(phyInstance);
    PHARQS pharq = pPhy->pharq;
    U32 outMsgSizeLocal = 0;

    U32 i, numBytes = 0;
    PBS pBs = pPhy->pBs;
    PBS_RX pBsRx = pPhy->pBsRx;
    PBS_RX_PRACH pBsRxPrach= pBs->pBsRxPrach[0];
    U32 outMsgSizeLocal1 = 0;
    U8 *outMsgLocal = (U8*)outMsg;
    ULRXINPUTPARAMS *pUlRxInputParams;
    ULRXFECPARAMS   *pRxFecParams;
    U32 length_bUserDetHard;
    S8 *bUserDetSoft;
    U32 k, nc, prc, user, iter, currTti;
    static U16 n_subfrm_stored = 0;
    static U16 n_subfrm_omit = 0;
    U16 n_subfrm_read=0;
    S16* pSrc;
    S32 timeOffset;
    U16 cellIdx,cellFound,Nsamples;
    U32 max_N_proc_subframe = 0;
    U16 N_buf_subframe;
    U16 proc_flag = 0;

    PLISTENINGDESC pListening;

    if (gMode == NMM_MODE)
    {
        pListening = LteBsGetNmmCtx();
        N_buf_subframe = (U16)(pListening->N_buf_subframe);
        if(pGlobalNMMOutApiMsg == NULL)
        {
            pGlobalNMMOutApiMsg = outMsgLocal;
            globalNMMoutApiMsgLen = &outMsgSizeLocal;
        }
        if(pListening && pListening->state >= NMM_IDLE)
        {
//            LteGenerateAPIMsg(phyInstance, (U32*)outMsgLocal, &outMsgSizeLocal1, PHY_RXSTART_IND, status);

            Nsamples = pPhy->pBs->Nfft*15*2;
            if(n_subfrm_omit < nmm_proc_subframe)
            {
                n_subfrm_omit ++;
                nmm_subframe_count ++;
                if(nmm_subframe_count == MAX_NMM_SUBFRAME_COUNT)
                    nmm_subframe_count = 0;

            }
            else
            {
                memcpy(nmm_iq_storage+n_subfrm_stored*Nsamples,sigIn,Nsamples*sizeof(S16));
                n_subfrm_stored ++;
                nmm_subframe_count ++;
                if(nmm_subframe_count == MAX_NMM_SUBFRAME_COUNT)
                    nmm_subframe_count = 0;
            }

            if(n_subfrm_stored == N_buf_subframe)
            {
                lte_bs_phy_nmm_update_subframe_count(N_buf_subframe);
                max_N_proc_subframe = 0;

                for(n_subfrm_read=1;n_subfrm_read<n_subfrm_stored-1;n_subfrm_read++)
                {
                    lte_bs_phy_nmm_check_proc_flag(&proc_flag);

                    if(proc_flag == 1)
                    {
                        max_N_proc_subframe = MAX_N_PROC_SUBFRAME;

                        pSrc = nmm_iq_storage+pPhy->pBs->Nfft*15*n_subfrm_read*2;
                        timeOffset = 0;
                        if( (pListening->state ==NMM_SYNC_SEARCHING_DONE) || (pListening->state ==NMM_PBCH_SEARCHING) ||
                            (pListening->state ==NMM_SIB1_SEARCHING) || (pListening->state == NMM_FULL_BCCH_SEARCHING) ||
                            (pListening->state ==NMM_PBCH_SEARCHING_DONE) || (pListening->state == NMM_TRACKING))
                        {
                            if (pListening->state ==NMM_SYNC_SEARCHING_DONE)
                                timeOffset = (pListening->pcell_search->cell_param[pListening->dlrs_foe_cellIdx].cell_time_offset >> 4)*2;
                            else
                            {
                                cellFound=0;
                                for(cellIdx=0;cellIdx<pListening->pcell_search->nb_detected_cell;cellIdx++)
                                {
                                    if(pListening->pcell_search->cell_param[cellIdx].cell_pci==pListening->pbch_pci)
                                    {
                                        cellFound=1;
                                        break;
                                    }
                                }
                                if(cellFound==1)
                                {
                                   timeOffset = (pListening->pcell_search->cell_param[cellIdx].cell_time_offset >> 4)*2;
                                }
                            }
                        }
                    
                        pSrc += timeOffset;
                        memcpy(sigIn,pSrc,Nsamples*sizeof(S16));
                        printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
                        printf("\\\\    read subframe %d \n", n_subfrm_read);
                        printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");

                        //outMsgSizeLocal1 = 0;
                        lte_bs_phy_nmm_main(phyInstance, sigIn, (U32*)outMsgLocal, &outMsgSizeLocal1);
                        //outMsgLocal += outMsgSizeLocal1;
                        //outMsgSizeLocal += outMsgSizeLocal1;
                    }//proc flag
                }// n read

                nmm_proc_subframe = rand();
                nmm_proc_subframe = (U32)floor(nmm_proc_subframe*max_N_proc_subframe/((double)RAND_MAX));

                n_subfrm_stored = 0;
                n_subfrm_omit = 0;
            }// buffer full
        }// state > idle
        pGlobalNMMOutApiMsg = NULL;
    }// MODE = NMM_MODE
    // This function should be invoked after a semaphore gets updated from
    // an isr that corresponds to the 1 ms subframe period. Per 3GPP 36.211
    // section 8.1 the Uplink Subframe preceeds the Downlink subframe by a
    // number between 0 and 0.66 ms. So for example the Rx Samples clock could
    // be used as a reference for the timer event that leads to the Tx and Rx
    // sub-frame proecessing


    //*****************************************************************************
    else if (gMode == LTE_MODE)
    {

        // First indicate that the receiver process has started
        LteGenerateAPIMsg(phyInstance, (U32*)outMsgLocal, &outMsgSizeLocal1, PHY_RXSTART_IND, status);

        // Point to Incoming IQ samples, for now for all antenna
        pPhy->sigIn[0] = sigIn;

        // Pre Run
        PhyBsRxPreExec(pPhy);   // Copy the data from pPhy->sigIn[0] to pBsRx->adcbuf
        PhyBsRxPrachPreExec(pPhy);
        PhyBsRxSrsPreExec(pPhy);

        // Update List
        setupFpPointers(pPhy);
        phy_bs_rx(pPhy);

        // Run
        PhyBsRxSRSExeTaskList(pPhy);
        PhyBsRxExeTaskList(pPhy);
        PhyBsRxPrachExeTaskList(pPhy, 0);
        PhyBsRxMuxControlExeTaskList(pPhy);

        // Post Run
        PhyBsRxPostExec(pPhy);
        PhyBsRxPrachPostExec(pPhy, 0);
        PhyBsRxSrsPostExec(pPhy);

        // Run FEC
        LteBsRxSduProcessing(pPhy);

        pUlRxInputParams = pBsRx->pUlRxInputParams;
        i                = pBsRx->RxFecParam_Context;
        pRxFecParams     = pBsRx->pUlRxFecParams[i];
        pBsRx->RxFecParam_Context = 1 - pBsRx->RxFecParam_Context;

        currTti = pPhy->rxFrameNumber*10 + pBsRx->subframeNumber;

        // Printing FEC output, soft decisions, SINR and frequency offset values to file
        // And Universal Diags streaming printout
        for(nc = 0; nc < (U32)pRxFecParams->numChan; nc++)
        {
            //Find harq Process
            prc = pharq->harq_prc;
            user = pRxFecParams->harq_user[nc];

            // iter = number of total HARQ iterations - 1.
            iter = pharq->harq_iter[prc][user];
            bUserDetSoft = pharq->harq_sft_pos[prc][user][iter];

            if(pUlRxInputParams->physChanType[nc] == PUSCH)
            {
                length_bUserDetHard = pPhy->demapOutSize[nc];

                for(i = 0; i < length_bUserDetHard; i++)
                {
#ifdef DEBUG_PRINT
                    fprintf(f_data_demod1_u0_b,"%hd\n", bUserDetSoft[i]);
#endif
                    if (test_case)
                        fprintf(f_bits_out,"%hd\n", bUserDetSoft[i]);
                }

                if (test_case)
                {
                    for (k = 0; k < pRxFecParams->numBytes[nc]; k++)
                    {
                        fprintf(fecout, "%d\n", pRxFecParams->mac_sdu[nc][k]);
                    }

                    //SINR
                    fprintf(fBsSinr,"%d \t", pUlRxInputParams->slotNum);
                    fprintf(fBsSinr,"%d \t", pUlRxInputParams->ul_CQI[nc]);
                    fprintf(fBsSinr,"%d \t", pUlRxInputParams->softDecSinrdB[nc]);
                    fprintf(fBsSinr, "%%UE #%d, PUSCH\n", nc);

                    //frequency offset
                    fprintf(fBsFreqOffset,"%d \t", pUlRxInputParams->slotNum);
                    fprintf(fBsFreqOffset,"%d \t", pUlRxInputParams->freqOffset[nc]);
                    fprintf(fBsFreqOffset, "%%UE #%d, PUSCH\n", nc);


                    //
                    // Universal Diags streaming output, specific to PUSCH channels only
                    //
                    //i.e. print out internal diagnostics not visible through the API
                    //TTTTTT printf("     FreqOffset = %d Hz\n", pUlRxInputParams->freqOffset[nc]);

                }
                //Copy Descrambler Input (UnCoded Bits) to Plugin output buffer for retrieval later
                for(i = 0; i < length_bUserDetHard; i++)
                {
                    UnCoded_Bits[i + BS_OutSize2] = bUserDetSoft[i];
                }
                BS_OutSize2 += length_bUserDetHard;
            }
            else //chan = pucch
            {
                length_bUserDetHard = 0;
                if (test_case)
                {
                    for (k = 0; k < 2; k++)
                        fprintf(fCtrlOutput, "%d\n", pBsRx->pUlRxSduParams->pucch_mac_sdu[nc][k]);

                    //SINR
                    fprintf(fBsSinr,"%d \t", pUlRxInputParams->slotNum);
                    fprintf(fBsSinr,"%d \t", pUlRxInputParams->ul_CQI[nc]);
                    fprintf(fBsSinr, "%%UE #%d, PUCCH\n", nc);
                }

                for(i = 0; i < length_bUserDetHard; i++)
                    UnCoded_Bits[i + BS_OutSize2] = bUserDetSoft[i];
                BS_OutSize2 += length_bUserDetHard;
            } // else PUCCH

            if (test_case)
            {
                //
                // Universal Diags streaming output, common for all channels
                //
                //i.e. print out internal diagnostics not visible through the API
                //TTTTTT printf("     softDecSINR = (%d =) %4.1f dB\n", pUlRxInputParams->softDecSinrdB[nc], ((double)pUlRxInputParams->softDecSinrdB[nc] /2.0) -64.0);
            }

        }

        //Send PUSCH/PUCCH RxSDU messages to MAC
        for (i = 0; i < (U32) pRxFecParams->numChan; i++)
        {
            if (pUlRxInputParams->physChanType[i] == PUSCH)
            {
                pRxFecParams->numBitsRx[i] = pRxFecParams->numBytes[i] * 8;
                numBytes = pRxFecParams->numBytes[i];
            }
            else if (pUlRxInputParams->physChanType[i] == PUCCH)
            {
                pRxFecParams->numBitsRx[i] = 2 * 8; // 13 bits for Format 2
                numBytes = 2;
                if (pBsRx->pUlRxSduParams->stat[i] == STAT_PUCCH_DEADZONE)
                {
                    pRxFecParams->numBitsRx[i] = 0;
                    numBytes = 0;
                }
            }

            // RSSI
            if ((pUlRxInputParams->rssiMeasFlag == 0) || (pUlRxInputParams->physChanType[i] == PUCCH))
                pUlRxInputParams->Rssi_dB[i] = DEFAULT_RSSI;

            LteGenerateRxSduIndMsg(pPhy, (U32*)outMsgLocal, &outMsgSizeLocal1,
                                   ((pBsRx->pUlRxSduParams->stat[i] == 3) ? SUCCESS : RX_CRC_ERROR),
                                   i, pUlRxInputParams, pRxFecParams, CONTEXT_P_PHY);

            outMsgLocal += outMsgSizeLocal1;
            outMsgSizeLocal += outMsgSizeLocal1;

            if (gTwoRxMuxControlLists)
            {
                // Generate PHY_RXSTATUS for ACK/NACK
                if (pRxFecParams->numbitsACK[i])
                {
                    LteGenerateRxStatusHi(pPhy, phyInstance, (U32*)outMsgLocal, &outMsgSizeLocal1,
                                               pRxFecParams, i, (U8)pPhy->pBsRx->subframeNumber, CONTEXT_P_PHY);
                    outMsgLocal += outMsgSizeLocal1;
                    outMsgSizeLocal += outMsgSizeLocal1;
                }
                // Generate PHY_RXSTATUS for Cqi,Ri
                if (pRxFecParams->numbitsCQI[i] || pRxFecParams->numbitsRI[i])
                {
                    LteGenerateRxStatusCqiRi(pPhy, phyInstance, (U32*)outMsgLocal, &outMsgSizeLocal1,
                                               pRxFecParams, i, (U8)pPhy->pBsRx->subframeNumber, CONTEXT_P_PHY);
                    outMsgLocal += outMsgSizeLocal1;
                    outMsgSizeLocal += outMsgSizeLocal1;
                }
            }
            else
            {
                // Generate PHY_RXSTATUS for Cqi,Ri and Ack/Nack
                if (pRxFecParams->numbitsCQI[i] || pRxFecParams->numbitsACK[i] || pRxFecParams->numbitsRI[i])
                {
                    LteGenerateRxStatusCqiRiHi(pPhy, phyInstance, (U32*)outMsgLocal, &outMsgSizeLocal1,
                                               pRxFecParams, i, (U8)pPhy->pBsRx->subframeNumber, CONTEXT_P_PHY);
                    outMsgLocal += outMsgSizeLocal1;
                    outMsgSizeLocal += outMsgSizeLocal1;
                }
            }
        }

        //Send PRACH messages to MAC
        if (pBsRxPrach[0].bs_prach_preambleSubframeEnable[pBsRx->subframeNumber] == 1)
        {
            LteGenerateRxPrachStatusIndMsg(pPhy, (U32*)outMsgLocal, &outMsgSizeLocal1,
                                          &pBs->pBsRxPrachDet[0]->bs_prachDet_msDet, (U8)pBsRx->subframeNumber, pPhy->rxFrameNumber);
            outMsgLocal += outMsgSizeLocal1;
            outMsgSizeLocal += outMsgSizeLocal1;
        }

        //Send SRS messages to MAC
        if (pBs->pBsSrs->srsSubframeEnable[pBsRx->subframeNumber] == 1 && pBsRx->numSrsChan > 0)
        {
            LteGenerateRxSrsStatusIndMsg(pPhy, (U32*)outMsgLocal,
                              &outMsgSizeLocal1, (U8)pBsRx->subframeNumber, pPhy->rxFrameNumber);
            outMsgLocal += outMsgSizeLocal1;
            outMsgSizeLocal += outMsgSizeLocal1;
        }

        // Measurements
        //--------------
        // RIP
        if (pUlRxInputParams->rxIntMeasFlag)
        {
            LteBsRxGenerateRipIndMsg(pUlRxInputParams->RipPerRB_dB, pUlRxInputParams->Nul_rb,
                (U8)pBsRx->subframeNumber, pPhy->rxFrameNumber, (U32*)outMsgLocal, &outMsgSizeLocal1);
            outMsgLocal += outMsgSizeLocal1;
            outMsgSizeLocal += outMsgSizeLocal1;
        }

        // Thermal Noise
        if (pUlRxInputParams->rxTherNoiseFlag)
        {
            LteBsRxGenerateThermalNoiseIndMsg(pUlRxInputParams->ThermalNoisePwr, pUlRxInputParams->unusedRB,
                (U8)pBsRx->subframeNumber, pPhy->rxFrameNumber, (U32*)outMsgLocal, &outMsgSizeLocal1);
            outMsgLocal += outMsgSizeLocal1;
            outMsgSizeLocal += outMsgSizeLocal1;
        }

        // RX END
        LteGenerateAPIMsg(phyInstance, (U32*)outMsgLocal, &outMsgSizeLocal1, PHY_RXEND_IND, status);


        // Clean up
        for (i = 0; i < (U32) pRxFecParams->numChan; i++)
        {
            if (pRxFecParams->mac_sdu[i])
                MemFree(pRxFecParams->mac_sdu[i], CRAM_MEM);

            if (pUlRxInputParams->physChanType[i] != PUSCH)
                MemFree(pRxFecParams->by[i], CRAM_MEM);

            pRxFecParams->by[i] = NULL;
            pRxFecParams->mac_sdu[i] = NULL;
        }
    }// MODE = LTE_MODE

    *outMsgSize = outMsgSizeLocal;
    return (status);
}
#endif

