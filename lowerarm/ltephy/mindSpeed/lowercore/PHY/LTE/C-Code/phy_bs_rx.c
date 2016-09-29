//-------------------------------------------------------------------------------------------
/** @file phy_bs_rx.c
 *
 * @brief LTE PHY Base Station Receiver
 * @author Mindspeed Technologies
 * @version $Revision: 1.327 $
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
#include "math_ext32.h"
#include "basic_op_cc.h"
#include "mlog.h"
#include "dft_table16.h"

#include "gputil_fx.h"
#include "bs_rx_prach.h"

#include "mlog.h"

#ifndef _WIN32
#include "LteBsRxDataSdu.h"
#include "LteBsRxPrachThread.h"
#include "LteBsRxSrsThread.h"
#include "LteBsRxThread.h"
#include "rexcomp.h"
#include "devinfo.h"
#include "fpdrv.h"
#endif

#ifdef _WIN32
FILE *f_srs_chan_est_fp;
FILE *f_prach_index_profile_fp;
#endif

extern S16 PRACH_Tables[];

//-------------------------------------------------------------------------------------------
/** @brief Create the Base Station Uplink Rx Static task lists. (PUSCH, PUCCH, PRACH, SRS)
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param pPhy - INPUT/OUTPUT, Pointer to current PHY running
 *
 *  @return status,
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type        |Data Format |Valid Range |Units|
 *  |           |Standard |              |         |            |            |            |     |
 *  ---------------------------------------------------------------------------------------------
 *  |pPhy       |         |pInput/pOutput|1        |pStructure  |            |            |     |
 *  ---------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------
 *  |Block Name          |CreateUlRxTaskList |
 *  ------------------------------------------
 *  |Input Streams       |None               |
 *  ------------------------------------------
 *  |Output Streams      |None               |
 *  ------------------------------------------
 *  |Config Parameters   |pPhy               |
 *  ------------------------------------------
 *  |Status Parameters   |pPhy               |
 *  ------------------------------------------
 *  BLOCK_END
 *
 * <4> Detailed Description:
 *  LTE PHY Base Station Receiver, output of soft decisions is located <BR>
 *  array defined in bsRxChan.
 *
 *
 * phy_bs_rx(): Base Station, rx PHY, OFDM demodulator block
 *
 * Called whenever there are IQ rx samples ready to be processed. The input adc sample rate
 * is defined by the channel frequency bandwidth. (e.g. 30.72MHz for the maximum 20MHz bandwidth case).
 * Since the base station defines all frame timings, there is no need for the BS Rx to synchronization to frame and
 * symbol boundaries. But, fine adjustment to timing and frequency offset for each individual channel needs to be
 * detected and corrected for. For now, the BS Rx will demodulate data one 1/2ms slot at a time. So basically,
 * uplink pBs rx latency is 1/2ms + processing time. So, since practical real time systems will have only small
 * MIPS overhead, can assume minm 1ms uplink pBs rx phy latency!
 *
 *Note:
 *       (1) Final version of this function will be called once per subframe, to decode ALL input transport blocks
 *       for the entire subframe.
 *       (2) This function is expected to be a "PHY Scheduler" function, running in the ARM. The input parameters
 *       pBs, bsRx, bsRxChan, are pointers only. The scheduling of each task, running in parallel on different
 *       dsp processors, and the DMA'ing of memory to each processor is controlled from here for uplink.
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 CreateUlRxTaskList(PLTEPHYDESC pPhy)
{
    // PHY Structures
    PBS pBs;
    PBS_RX pBsRx;
    PBS_RX_POINTERS pRxPtr = pPhy->pBsRx->pBsRxPointers;
    PULRXINPUTPARAMS pUlRxInputParams;
    PULRXSDUPARAMS   pUlRxSduParams;
    PULRXFECPARAMS pFecParams;
    S16 *pTable_DFT[5];

    // General Variables
    S32 i;
    S16 mo, isym, skipped_isym;
    S32 length_adc;
    S16 cond = 0;
    S16 numPucchRun[NUM_USER_PUCCH_TCBS];

    S16 MI, MO;
    S16 Nul_symb;
    S16 K, L;
    S16 pilotCarrierSymbol;
    S32 step_aDet, step_reDet;
    S32 length_r_alpha_uv;

    // Task List Related
    PTASKLIST pTaskList1 = pPhy->pBsRx->pExeTaskList[0];
    PTASKLIST pTaskListSrs = pPhy->pBsRx->pExeSrsTaskList[0];
    PTCB *pTcbList1 = pPhy->pBsRx->pTaskRx1;
    PTCB *pTcbList2 = pPhy->pBsRx->pTaskRx2;
    PTCB *pTcbList4 = pPhy->pBsRx->pTaskRx4;
    PTCB *pTcbList3 = pPhy->pBsRx->pTaskRx3;
    U16 NumTasksTotalRX = 0;
    U16 NumTasksTotalRX1 = 0;
    U16 NumTasksTotalRX3 = 0;
    S16 numDepend  = 0, numDepend1 = 0, numDepend2 = 0;
    S16 numDepend4 = 0;
    S32 numMultiChanTCBs;

    PTCB pFftDependencyList[2][16];
    U32  fftDependencyListCnt[2] = {0,0};

#ifdef IDFT_CELL_DEPENDENCY
    U32 j = 0;
    PTCB pIdftDependencyList[16];
    U32  idftDependencyListCnt = 0;
#endif

#ifndef _WIN32
    U32 *pContext;
#endif

    ////////////////////////////////////////////////////////////////////////////
    // Initialize
    ////////////////////////////////////////////////////////////////////////////
    pBs = pPhy->pBs;
    pBsRx = pPhy->pBsRx;
    pUlRxInputParams = pBsRx->pUlRxInputParams;
    pUlRxSduParams = pBsRx->pUlRxSduParams;
    i                = pBsRx->RxFecParam_Context;
    pFecParams       = pBsRx->pUlRxFecParams[i];

    Nul_symb = pBsRx->Nul_symb;
    K = pBsRx->Nul_rb * pBs->Nrb_sc; //total number of frequency indices in resource grid
    L = 2 * Nul_symb; //total number of time indices in resource grid subframe
    pUlRxInputParams->L = L;
    pUlRxInputParams->TotalMsc = 0;
    length_r_alpha_uv = 7 * NUM_SLOTS_SUBFRM * 2 * PRB_12;
    length_adc = 2 * ((pBs->Ncp0 + pBs->Nfft) + 6 * (pBs->Ncp + pBs->Nfft));

    MI = pBsRx->numMS; // it is supposed each MS has one TX antenna
    MO = pBsRx->Nrx_antennas;
    pUlRxInputParams->MI = MI;
    pUlRxInputParams->MO = MO;
    pilotCarrierSymbol = CA_ULPilotCarrierSymbol[pBs->CPType];

    for (i = 0; i < MO; i++)
    {
        pRxPtr->ppSnrPerRb_Pilot[2 * i] = (S32 *) &(pRxPtr->pSnrPerRb[100 * (2 * i)]);
        pRxPtr->ppSnrPerRb_Pilot[2 * i + 1] = (S32 *) &(pRxPtr->pSnrPerRb[100 * (2 * i + 1)]);
    }

    step_aDet = 2 * RUP8B(K + 2);
    step_reDet = 2 * RUP8B(K + 4*MAXCHSUBFRAME);

    ////////////////////////////////////////////////////////////////////////////
    // Initializations END
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // CREATE TASK LIST
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // FFT
    ////////////////////////////////////////////////////////////////////////////
#ifndef _WIN32
    pBsRx->fftBlockTcbsCnt1 = 0;
    pBsRx->fftBlockTcbsCnt3 = 0;

    // These 2 needed only for MLOG. Not used for running. TO run FFT, there is direct call to TCB
    pBsRx->pFftTcb[0] = CreateTCB(TASKID_RX_FFT, RESOURCE_FLTP, NULL, 1 * sizeof(U32), NULL, NULL);
    pBsRx->pFftTcb[0]->isym = 0;
    pContext = (U32*) pBsRx->pFftTcb[0]->ContextPtr;
    pContext[0] = (U32)pPhy;
    MLogTCBAddToList(pBsRx->pFftTcb[0],  RexGetListID(pTaskList1));
    pBsRx->pFftTcb[0]->ResourceIndex = 0x3E00;

    pBsRx->pFftTcb[1] = CreateTCB(TASKID_RX_FFT, RESOURCE_FLTP, NULL, 1 * sizeof(U32), NULL, NULL);
    pBsRx->pFftTcb[1]->isym = 1;
    pContext = (U32*) pBsRx->pFftTcb[1]->ContextPtr;
    pContext[0] = (U32)pPhy;
    MLogTCBAddToList(pBsRx->pFftTcb[1], RexGetListID(pTaskList1));
    pBsRx->pFftTcb[1]->ResourceIndex = 0x3E00;
#endif // _WIN32

    ////////////////////////////////////////////////////////////////////////////
    // PRACH LIST
    ////////////////////////////////////////////////////////////////////////////
    pTcbList2[0] = PRACH_Rx_tcb(pBsRx, pBs->pBsRxPrach[0], pBs->pBsRxPrachDet[0], MO,
                                pBsRx->adcbuf, length_adc, PRACH_Tables,
                                pBs->pBsRxPrachCram[0], pPhy->pBsRx->pExePrachTaskList[0], TCBIOBUFFILL, NULL, NULL);

    pTcbList4[0] = PRACH_Rx_tcb(pBsRx, pBs->pBsRxPrach[1], pBs->pBsRxPrachDet[1], MO,
                                pBsRx->adcbuf, length_adc, PRACH_Tables,
                                pBs->pBsRxPrachCram[1], pPhy->pBsRx->pExePrachTaskList[1], TCBIOBUFFILL, NULL, NULL);

#ifndef _WIN32
    RegListDoneCB(pBsRx->pExePrachTaskList[0], LtePhyPrachListDoneCb, &pBsRx->prachReturnCtx[0]);
    pBsRx->prachListId[0] = RexGetListID(pBsRx->pExePrachTaskList[0]);
    RegListDoneCB(pBsRx->pExePrachTaskList[1], LtePhyPrachListDoneCb, &pBsRx->prachReturnCtx[1]);
    pBsRx->prachListId[1] = RexGetListID(pBsRx->pExePrachTaskList[1]);
#endif

    ////////////////////////////////////////////////////////////////////////////
    // SRS LIST
    ////////////////////////////////////////////////////////////////////////////
    pTable_DFT[0] = NULL;
    pTable_DFT[1] = NULL;
    pTable_DFT[2] = NULL;
    pTable_DFT[3] = NULL;
    pTable_DFT[4] = NULL;

    pTcbList3[NumTasksTotalRX3] = bs_rx_Pilot_srs_tcb(pTaskListSrs, pBsRx->subframeNumber,
                                      pBs->Nrb_sc, pBs->Ncell_id, pUlRxInputParams->groupHoppingEnabled,
                                      pUlRxInputParams->sequenceHoppingEnabled, pUlRxInputParams->delta_ss,
                                      NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                                      pBsRx->numSrsChan, pBs->Nused, NULL, pTcbList1,
                                      NULL, NULL, TCBIOBUFFILL, NULL);
    NumTasksTotalRX3++;


    pTcbList3[NumTasksTotalRX3] = SRSReceiverProcessing_tcb(pTaskListSrs,
                                                    pBs->df, pBs->Fs,
                                                    pBs->Nrb_sc, pBs->Nused, pBs->Nfft,
                                                    NULL, NULL, NULL,
                                                    &pBsRx->aDet[0],
                                                    pBsRx->numSrsChan, 0, MO, K, L,
                                                    pBsRx->Nul_rb, NULL, pRxPtr, NULL,
                                                    step_aDet, NULL, NULL,
                                                    pTcbList1, pRxPtr->pDependencyArray, numDepend,
                                                    pTcbList3[NumTasksTotalRX3-1], TCBIOBUFFILL, NULL,pTable_DFT,
                                                    pPhy->rxFrameNumber,pBsRx->subframeNumber);
    pBsRx->pFftBlockTcbs3[pBsRx->fftBlockTcbsCnt3++] = pTcbList3[NumTasksTotalRX3];
    pFftDependencyList[1][fftDependencyListCnt[1]++] = pTcbList3[NumTasksTotalRX3];     // Slot 1
    NumTasksTotalRX3++;


    pTcbList3[NumTasksTotalRX3] = SRSSnrReporting_tcb(pTaskListSrs, MO, 0, pBsRx->Nul_rb,
                                                    NULL, NULL, NULL, NULL,
                                                    &pTcbList3[NumTasksTotalRX3 - MO],
                                                    TCBIOBUFFILL, NULL);
    NumTasksTotalRX3++;


#ifndef _WIN32
    RegListDoneCB(pTaskListSrs, LtePhySrsListDoneCb, NULL);
    pBsRx->srsListId = RexGetListID(pTaskListSrs);
#endif


#ifdef DEBUG_POWER_RX_SIGNAL
    pTcbList1[NumTasksTotalRX] = ul_signal_power_tcb(pTaskList1, pBsRx->adcbuf[0], pBsRx->adcbuf[1],
                                                     pUlRxInputParams);
    NumTasksTotalRX++;
#endif

    /////////////////////////////////////////////////////////////////////////////////////
    //PUCCH Uplink Pilots - generate pilots for all Users
    /////////////////////////////////////////////////////////////////////////////////////
    pTcbList1[NumTasksTotalRX] = ul_Pilot_PUCCH_tcb(pTaskList1, pRxPtr->bs_aPilots_pucch,
                                                     pBsRx->rs_grid, pRxPtr->r_alpha_uv,
                                                     length_r_alpha_uv, step_aDet,
                                                     pRxPtr->nOc, pRxPtr->Sdescramb,
                                                     pUlRxInputParams, NULL,
                                                     TCBIOBUFFILL, NULL);
    NumTasksTotalRX++;
    pRxPtr->pDependencyArray[numDepend++] = NumTasksTotalRX - 1;

    //////////////////////////////////////////////////////////////////////////////
    //Channel Estimation for PUCCH
    //////////////////////////////////////////////////////////////////////////////
    numDepend2 = 0;
    for(mo = 0; mo < MO; mo++)
    {
        //////////////////////////////////////////////////////////////////////////////
        //Channel Estimation for first slot
        //////////////////////////////////////////////////////////////////////////////
        pTcbList1[NumTasksTotalRX] = bs_rx_ChEstimator_PUCCH_part0_tcb(pTaskList1,
                                          &pBsRx->aDet[mo * step_aDet * L], pBsRx->rs_grid,
                                          pRxPtr->bs_aPilots_pucch, pUlRxInputParams, 0, mo,
                                          &pRxPtr->pExpo_ch_pucch[(mo * 2 * Nul_symb)],
                                          step_aDet,
                                          pRxPtr->pErrAvg,  ScaleComputeErr_Tab, &pRxPtr->pChEst_ul_pucch[mo * 2 * 2 * PRB_12 * MAX_PUCCH_PILOTSYMB_PER_SUBF * MAXCHSUBFRAME],
                                          pRxPtr->pErrSub_pucch, pRxPtr->pErrSubExpo_pucch,
                                          pTcbList1, pRxPtr->pDependencyArray, numDepend,
                                          TCBIOBUFFILL, NULL);
        pBsRx->pFftBlockTcbs1[pBsRx->fftBlockTcbsCnt1++] = pTcbList1[NumTasksTotalRX];
        pFftDependencyList[0][fftDependencyListCnt[0]++] = pTcbList1[NumTasksTotalRX];      // Slot 0
        pRxPtr->pDependencyArray1[numDepend1++] = NumTasksTotalRX;
        NumTasksTotalRX++;

        //////////////////////////////////////////////////////////////////////////////
        //Channel Estimation for second slot
        //////////////////////////////////////////////////////////////////////////////
        pTcbList1[NumTasksTotalRX] = bs_rx_ChEstimator_PUCCH_part0_tcb(pTaskList1,
                                          &pBsRx->aDet[mo * step_aDet * L], pBsRx->rs_grid,
                                          pRxPtr->bs_aPilots_pucch, pUlRxInputParams, 1, mo,
                                          &pRxPtr->pExpo_ch_pucch[(mo * 2 * Nul_symb)],
                                          step_aDet,
                                          pRxPtr->pErrAvg,  ScaleComputeErr_Tab, &pRxPtr->pChEst_ul_pucch[mo * 2 * 2 * PRB_12 * MAX_PUCCH_PILOTSYMB_PER_SUBF * MAXCHSUBFRAME],
                                          pRxPtr->pErrSub_pucch, pRxPtr->pErrSubExpo_pucch,
                                          pTcbList1, pRxPtr->pDependencyArray, numDepend,
                                          TCBIOBUFFILL, NULL);
        pBsRx->pFftBlockTcbs1[pBsRx->fftBlockTcbsCnt1++] = pTcbList1[NumTasksTotalRX];
        pFftDependencyList[1][fftDependencyListCnt[1]++] = pTcbList1[NumTasksTotalRX];      // Slot 1
        pRxPtr->pDependencyArray1[numDepend1++] = NumTasksTotalRX;
        NumTasksTotalRX++;
        pRxPtr->pDependencyArray2[numDepend2++] = NumTasksTotalRX - 1;
        pRxPtr->pDependencyArray2[numDepend2++] = NumTasksTotalRX - 2;
    }

    //////////////////////////////////////////////////////////////////////////////////
    //Averaging of TimingErr, depends on ChanEst for PUCCH to finish before running
    //////////////////////////////////////////////////////////////////////////////////
    pTcbList1[NumTasksTotalRX] = RunningAveragePUCCH_tcb(pTaskList1, pUlRxInputParams, pRxPtr->pErrAvg, &pRxPtr->pExpo_ch_pucch[0],
                                                     pRxPtr->pErrAvgOut, pRxPtr->pErrSub_pucch, pRxPtr->pErrSubExpo_pucch, pTcbList1,
                                                     pRxPtr->pDependencyArray2, numDepend2, TCBIOBUFFILL, NULL);



    NumTasksTotalRX1 = NumTasksTotalRX;
    NumTasksTotalRX++;

    for(mo = 0; mo < MO; mo++)
    {
        //////////////////////////////////////////////////////////////////////////////
        //Channel Estimation for first slot
        //////////////////////////////////////////////////////////////////////////////
        pTcbList1[NumTasksTotalRX] = bs_rx_ChEstimator_PUCCH_tcb(pTaskList1,
                                          &pBsRx->aDet[mo * step_aDet * L], pBsRx->rs_grid,
                                          &pRxPtr->pChEst_ul_pucch[mo * 2 * 2 * PRB_12 * MAX_PUCCH_PILOTSYMB_PER_SUBF * MAXCHSUBFRAME], pUlRxInputParams, 0, mo,
                                          &pRxPtr->pExpo_ch_pucch[(mo * 2 * Nul_symb)],
                                          step_aDet, pRxPtr->r_alpha_uv, length_r_alpha_uv,
                                          pRxPtr->Sdescramb, pRxPtr->nOc,
                                          0, pRxPtr->pAvgReDet_pucchACK, pRxPtr->pAvgReDet_pucchCQI,pRxPtr->pChEst_pucch,
                                          pRxPtr->rxInPwrAvg_PerRbPilot, pRxPtr->pPwrAvg_Pilot_expo,
                                          pRxPtr->rxInPwrAvg_PerRbDataSym, pRxPtr->pPwrAvg_Data_expo,
                                          pRxPtr->pAllPucchPwrAvg_AntSlot, pRxPtr->pPucchPwrAvg, pRxPtr->pRxInRbPwrAvg_AntSlot, pRxPtr->pRbUsedFlag,
                                          pRxPtr->pRxRssi_AntSlot, pRxPtr->pErrAvgOut,
                                          pTcbList1[NumTasksTotalRX1], TCBIOBUFFILL, NULL);

        pRxPtr->pDependencyArray1[numDepend1++] = NumTasksTotalRX;
        NumTasksTotalRX++;

        //////////////////////////////////////////////////////////////////////////////
        //Channel Estimation for second slot
        //////////////////////////////////////////////////////////////////////////////
        pTcbList1[NumTasksTotalRX] = bs_rx_ChEstimator_PUCCH_tcb(pTaskList1,
                                          &pBsRx->aDet[mo * step_aDet * L], pBsRx->rs_grid,
                                          &pRxPtr->pChEst_ul_pucch[mo * 2 * 2 * PRB_12 * MAX_PUCCH_PILOTSYMB_PER_SUBF * MAXCHSUBFRAME], pUlRxInputParams, 1, mo,
                                          &pRxPtr->pExpo_ch_pucch[(mo * 2 * Nul_symb)],
                                          step_aDet, pRxPtr->r_alpha_uv, length_r_alpha_uv,
                                          pRxPtr->Sdescramb, pRxPtr->nOc,
                                          0, pRxPtr->pAvgReDet_pucchACK,pRxPtr->pAvgReDet_pucchCQI, pRxPtr->pChEst_pucch,
                                          pRxPtr->rxInPwrAvg_PerRbPilot, pRxPtr->pPwrAvg_Pilot_expo,
                                          pRxPtr->rxInPwrAvg_PerRbDataSym, pRxPtr->pPwrAvg_Data_expo,
                                          pRxPtr->pAllPucchPwrAvg_AntSlot, pRxPtr->pPucchPwrAvg, pRxPtr->pRxInRbPwrAvg_AntSlot, pRxPtr->pRbUsedFlag,
                                          pRxPtr->pRxRssi_AntSlot, pRxPtr->pErrAvgOut,
                                          pTcbList1[NumTasksTotalRX1], TCBIOBUFFILL, NULL);

        pRxPtr->pDependencyArray1[numDepend1++] = NumTasksTotalRX;
        NumTasksTotalRX++;
    }
    //////////////////////////////////////////////////////////////////////////////
    //User base MRC, FEQ, CazacAvg and DeMapper
    //////////////////////////////////////////////////////////////////////////////
    memset(numPucchRun, 0, sizeof(S16)*NUM_USER_PUCCH_TCBS);
    for(i = 0; i < NUM_USER_PUCCH_TCBS; i++)
    {
        pTcbList1[NumTasksTotalRX] = rx_Multichan_DeModulation_Pucch_tcb(pTaskList1,
                                            pBsRx->aDet, pRxPtr->pExpo_ch_pucch,
                                            pUlRxInputParams, pUlRxSduParams,
                                            pRxPtr->pucchChan[i], numPucchRun[i],
                                            pPhy->descrInBufs, pPhy->descrIndex,
                                            pRxPtr->pAvgReDet_pucchACK, pRxPtr->pAvgReDet_pucchCQI,
                                            pRxPtr->pSnrPerRb, pRxPtr->pSnrPilot, pRxPtr->pChEst_pucch,
                                            pRxPtr->rxInPwrAvg_PerRbPilot, pRxPtr->pPwrAvg_Pilot_expo,
                                            pRxPtr->rxInPwrAvg_PerRbDataSym, pRxPtr->pPwrAvg_Data_expo,
                                            pRxPtr->pAllPucchPwrAvg_AntSlot, pRxPtr->pPucchPwrAvg,
                                            rm_dectab_20_X, rm_dectab_20_Offset, pTcbList1,
                                            pRxPtr->pDependencyArray1, numDepend1, TCBIOBUFFILL, NULL);

        pRxPtr->pDependencyArray4[numDepend4++] = NumTasksTotalRX;
        NumTasksTotalRX++;
    }

    /////////////////////////////////////////////////////////////////////////////////////
    //PUSCH Uplink Pilots - generate pilots for all Users
    /////////////////////////////////////////////////////////////////////////////////////
    numDepend = 0;
    pTcbList1[NumTasksTotalRX] = ul_Pilot_fx_tcb(pTaskList1, pBsRx->bs_aPilots, pUlRxInputParams,
                                                  pBsRx->pBsPusch->cvec_u, pBsRx->pBsPusch->PRSbits,
                                                  Bit_Reversal, UlPilots_QthRootTable,
                                                  pBsRx->rs_grid, step_aDet,
                                                  NULL, TCBIOBUFFILL, NULL);
    NumTasksTotalRX++;
    pRxPtr->pDependencyArray[numDepend++] = NumTasksTotalRX - 1;

    numDepend2 = 0;
    for (mo = 0; mo < MO; mo++)
    {
        ///////////////////////////////////////////////////////////////////////////////////////////
        //Channel estimation from pilots, for every subcarrier in the resource element matrix, for this user, for the complete slot
        //Compute Channel Estimate at pilots, computation of derotation vector
        //Derotation of Channel at pilots
        //do least squares straight line fit to these pilot channel estimates to do some noise averaging
        ///////////////////////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////////////////
        //PUSCH Channel Estimator, Slot 0
        ///////////////////////////////////////////////////////////////////////////////////////////
        pTcbList1[NumTasksTotalRX] = bs_rx_ChEstimator_part0_fx_tcb(pTaskList1, &pBsRx->aDet[mo * step_aDet * L],
                                             pBsRx->rs_grid, pBsRx->bs_aPilots,
                                             pUlRxInputParams, step_aDet, &pRxPtr->pChEst_ul[mo * MI * 2 * K * 2],
                                             ScaleComputeErr_Tab, pRxPtr->pErrAvg, mo, 0,
                                             &pRxPtr->pExpo_mat[2*mo],
                                             pTcbList1, pRxPtr->pDependencyArray, numDepend, TCBIOBUFFILL, NULL);
        pBsRx->pFftBlockTcbs1[pBsRx->fftBlockTcbsCnt1++] = pTcbList1[NumTasksTotalRX];
        pFftDependencyList[0][fftDependencyListCnt[0]++] = pTcbList1[NumTasksTotalRX];      // Slot 0
        NumTasksTotalRX++;

        ///////////////////////////////////////////////////////////////////////////////////////////
        //PUSCH Channel Estimator, Slot 1
        ///////////////////////////////////////////////////////////////////////////////////////////
        pTcbList1[NumTasksTotalRX] = bs_rx_ChEstimator_part0_fx_tcb(pTaskList1, &pBsRx->aDet[mo * step_aDet * L],
                                             pBsRx->rs_grid, pBsRx->bs_aPilots,
                                             pUlRxInputParams, step_aDet, &pRxPtr->pChEst_ul[(mo * MI * 2 * K * 2)],
                                             ScaleComputeErr_Tab, pRxPtr->pErrAvg, mo, 1,
                                             &pRxPtr->pExpo_mat[2*mo],
                                             pTcbList1, pRxPtr->pDependencyArray, numDepend, TCBIOBUFFILL, NULL);
        pBsRx->pFftBlockTcbs1[pBsRx->fftBlockTcbsCnt1++] = pTcbList1[NumTasksTotalRX];
        pFftDependencyList[1][fftDependencyListCnt[1]++] = pTcbList1[NumTasksTotalRX];      // Slot 1
        NumTasksTotalRX++;
        pRxPtr->pDependencyArray2[numDepend2++] = NumTasksTotalRX - 1;
        pRxPtr->pDependencyArray2[numDepend2++] = NumTasksTotalRX - 2;
        ///////////////////////////////////////////////////////////////////////////////////////////
    } //for each antenna, for (mo = 0; mo < MO; mo++)


     //////////////////////////////////////////////////////////////////////////////////
    //Averaging of TimingErr, depends on ChanEst for PUSCH  to finish before running
    //////////////////////////////////////////////////////////////////////////////////
    pTcbList1[NumTasksTotalRX] = RunningAverage_tcb(pTaskList1, pUlRxInputParams, pRxPtr->pErrAvg, &pRxPtr->pExpo_mat[0],
                                                     pRxPtr->pErrExpo, pRxPtr->pErrAvgOut, pRxPtr->mErrExpoTemp, pRxPtr->mErrTemp, 2, pTcbList1,
                                                     pRxPtr->pDependencyArray2, numDepend2, TCBIOBUFFILL, NULL);


    NumTasksTotalRX1 = NumTasksTotalRX;
    NumTasksTotalRX++;
    numDepend2 = 0;
    for (mo = 0; mo < MO; mo++)
    {
        ///////////////////////////////////////////////////////////////////////////////////////////
        //Channel estimation from pilots, for every subcarrier in the resource element matrix, for this user, for the complete slot
        //Compute Channel Estimate at pilots, computation of derotation vector
        //Derotation of Channel at pilots
        //do least squares straight line fit to these pilot channel estimates to do some noise averaging
        ///////////////////////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////////////////
        //PUSCH Channel Estimator, Slot 0

        ///////////////////////////////////////////////////////////////////////////////////////////
        pTcbList1[NumTasksTotalRX] = bs_rx_ChEstimator_part1_fx_tcb(pTaskList1,  &pBsRx->derotation[0],
                                             pUlRxInputParams, step_aDet, &pRxPtr->pChEst_ul[mo * MI * 2 * K * 2],
                                             pRxPtr->pErrAvgOut, mo, 0,
                                             pRxPtr->ppSnrPerRb_Pilot[2 * mo], pRxPtr->pSnrPilot, pRxPtr->pnoiseVar,
                                             pRxPtr->mErrExpoTemp, pRxPtr->mErrTemp, pRxPtr->pRxInRbPwrAvg_AntSlot, pRxPtr->pRbUsedFlag,
                                             pRxPtr->pRxRssi_AntSlot, pTcbList1[NumTasksTotalRX1], TCBIOBUFFILL, NULL);
        NumTasksTotalRX++;

        ///////////////////////////////////////////////////////////////////////////////////////////
        //PUSCH Channel Estimator, Slot 1
        ///////////////////////////////////////////////////////////////////////////////////////////
        pTcbList1[NumTasksTotalRX] = bs_rx_ChEstimator_part1_fx_tcb(pTaskList1, &pBsRx->derotation[0],
                                             pUlRxInputParams, step_aDet, &pRxPtr->pChEst_ul[(mo * MI * 2 * K * 2)],
                                             pRxPtr->pErrAvgOut, mo, 1,
                                             pRxPtr->ppSnrPerRb_Pilot[2 * mo + 1], pRxPtr->pSnrPilot, pRxPtr->pnoiseVar,
                                             pRxPtr->mErrExpoTemp, pRxPtr->mErrTemp, pRxPtr->pRxInRbPwrAvg_AntSlot, pRxPtr->pRbUsedFlag,
                                             pRxPtr->pRxRssi_AntSlot, pTcbList1[NumTasksTotalRX1], TCBIOBUFFILL, NULL);
        NumTasksTotalRX++;

        pRxPtr->pDependencyArray2[numDepend2++] = NumTasksTotalRX - 2;  //For rx_Multichan_DeModulation_tcb
        pRxPtr->pDependencyArray2[numDepend2++] = NumTasksTotalRX - 1;  //For rx_Multichan_DeModulation_tcb
        pRxPtr->pDependencyArray4[numDepend4++] = NumTasksTotalRX - 2;  //For SnrSubframe_tcb
        pRxPtr->pDependencyArray4[numDepend4++] = NumTasksTotalRX - 1;  //For SnrSubframe_tcb

        ///////////////////////////////////////////////////////////////////////////////////////////
    } //for each antenna, for (mo = 0; mo < MO; mo++)

    //////////////////////////////////////////////////////////////////////////////////
    //Compute (1) average subframe SNR per antenna, (2) average subframe SNR per RB
    //////////////////////////////////////////////////////////////////////////////////
    // Do SNR related final calculation and put them into RxInput structure
    pTcbList1[NumTasksTotalRX] = SnrSubframe_tcb(pTaskList1, pUlRxInputParams, pRxPtr->pSnrPerRb, pRxPtr->pSnrPilot,
                                                   pTcbList1, pRxPtr->pDependencyArray4, numDepend4,
                                                   TCBIOBUFFILL, NULL);
    pBsRx->pSnrSubframeTask = pTcbList1[NumTasksTotalRX];
    NumTasksTotalRX++;


    //////////////////////////////////////////////////////////////////////////////////
    //Compute  (1) RIP per RB (2) RSSI for PUSCH and PUCCH (3) Thermal Noise
    //////////////////////////////////////////////////////////////////////////////////

    pTcbList1[NumTasksTotalRX] = RipRssiSubframe_tcb(pTaskList1, pUlRxInputParams,
		                                           pBsRx->aDet, pRxPtr->pRxInRbPwrAvg_AntSlot,
		                                           pRxPtr->pRbUsedFlag, pRxPtr->pRxInRip, step_aDet, pBs->pBsSrs->srsPilotSymbol, pRxPtr->pInitFlag,
		                                           pRxPtr->pExpo_mat, pRxPtr->pRxRssi_AntSlot,
                                                   pTcbList1, pRxPtr->pDependencyArray4, numDepend4,
                                                   TCBIOBUFFILL, NULL);
    pBsRx->pRipRssiSubframeTask = pTcbList1[NumTasksTotalRX];
    NumTasksTotalRX++;

    //////////////////////////////////////////////////////////////////////////////////
    // Combine all antenna signals into one signal per user
    // Note: for now, assume do one slot at a time. But, could minimize latency by waiting only for the
    //      reference symbol. So here do two passes through this loop, one for each slot...
    // All CH symbols have same exponent within a slot and per rx antenna
    // This is due to the way Ch estimation was done
    //
    // Detect nearest constellation point with maximal ratio combining of all antenna values
    //
    // Symbol-based MRC
    //////////////////////////////////////////////////////////////////////////////////
    numDepend = 0;
    numMultiChanTCBs = 0;
    skipped_isym = 0;
    for (isym = 0; isym < Nul_symb; isym++)
    {
        cond = 0;
        cond = (isym == pilotCarrierSymbol);

        if (cond == 1)
            skipped_isym++;
        else
            pBsRx->isym_eff[isym + 1 - skipped_isym] = isym;

        if (cond == 0)
        {
            pTcbList1[NumTasksTotalRX] = rx_Multichan_DeModulation_tcb(pTaskList1, pBsRx->aDet,
                                                pBsRx->rs_grid, pRxPtr->pChEst_ul, pBsRx->derotation,
                                                pRxPtr->pReDet0, pRxPtr->pExpo_norm_reEst,
                                                step_aDet, step_reDet, isym, pUlRxInputParams,
                                                -1, pRxPtr->pnoiseVar, pTcbList1,
                                                pRxPtr->pDependencyArray2, numDepend2, TCBIOBUFFILL, NULL);
            pRxPtr->pDependencyArray[numDepend++] = NumTasksTotalRX;
            NumTasksTotalRX++;
            numMultiChanTCBs++;
        }
    }

    pBsRx->isym_eff[0] = (S16)(numMultiChanTCBs);

    //////////////////////////////////////////////////////////////////////////////////
    //Run IDFT DeSpreader
    //Undo the Transform Precoding (i.e. DFT Spreading) of the complex constellation points
    //////////////////////////////////////////////////////////////////////////////////
#ifndef _WIN32

    // Single IDFT TCB
    isym = 0;
    pTcbList1[NumTasksTotalRX] = IDFT_tcb(pTaskList1, NULL,
                                        NULL, 0, (S16)(numMultiChanTCBs), pBsRx->pMapSetupDesc, 0,
                                        0, 0, 0,
                                        &pTcbList1[pRxPtr->pDependencyArray[isym]],
                                        TCBIOBUFFILL, NULL);
    pBsRx->pMapSetupDesc->pTcb         = pTcbList1[NumTasksTotalRX];
    NumTasksTotalRX++;

#else
    isym = 0;
    pTcbList1[NumTasksTotalRX] = IDFT_tcb(pTaskList1, &pRxPtr->pReDet0[isym * step_reDet],
                                          pRxPtr->pReDet0, isym, -1, NULL, step_reDet,
                                          (U16*)pUlRxInputParams->Mpusch_sc, (S16)pUlRxInputParams->numPUSCH, pUlRxInputParams->TotalMsc,
                                          &pTcbList1[pRxPtr->pDependencyArray[isym]],
                                          TCBIOBUFFILL, NULL);
    NumTasksTotalRX++;
#endif

    //////////////////////////////////////////////////////////////////////////////////
    //Rx DeMapper to generate soft decisions
    //////////////////////////////////////////////////////////////////////////////////
    numDepend = 0;
    for (isym = 0; isym < numMultiChanTCBs; isym++)
    {
        pTcbList1[NumTasksTotalRX] = rx_Demapper_fx_tcb(pTaskList1, pRxPtr->pReDet0,
                                 pRxPtr->pExpo_norm_reEst, pFecParams, pUlRxInputParams,
                                 isym, -1, step_reDet, pPhy->descrInBufs, (S32*) pPhy->descrIndex,
                                 pRxPtr->pMuxCtrl_SoftDec, pTcbList1[NumTasksTotalRX - isym - 1], //NULL,
                                 TCBIOBUFFILL, NULL);

#ifdef IDFT_CELL_DEPENDENCY
        pIdftDependencyList[idftDependencyListCnt++] = pTcbList1[NumTasksTotalRX];
#endif
        pRxPtr->pDependencyArray[numDepend++] = NumTasksTotalRX;
        NumTasksTotalRX++;
    }


    pTcbList1[NumTasksTotalRX] = muxCntlACK_Dec_tcb(pTaskList1, pFecParams, pRxPtr->pMuxCtrl_SoftDec, pTcbList1,
                                    pRxPtr->pDependencyArray, numDepend, TCBIOBUFFILL, NULL);
    NumTasksTotalRX++;

    pBsRx->pMuxCtrlTcbCtx1 = muxCntlRI_CQI_Dec_tcb(NULL, pFecParams, pRxPtr->pMuxCtrl_SoftDec, NULL,
                                    NULL, NULL, TCBIOBUFFILL, NULL);
    pBsRx->pMuxCtrlTcbCtx2 = muxCntlRI_CQI_Dec_tcb(NULL, pFecParams, pRxPtr->pMuxCtrl_SoftDec, NULL,
                                    NULL, NULL, TCBIOBUFFILL, NULL);

    pBsRx->isExeTaskListReady = FALSE;
    pBsRx->isExePrachTaskListReady[0] = FALSE;
    pBsRx->isExePrachTaskListReady[1] = FALSE;
    pBsRx->isExeSrsTaskListReady = FALSE;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // CREATE TASK LIST END
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

#ifndef WIN32
#ifndef SLOT_BASED_FFT
    // FFT Cell Depndency Slot 0
    i = (pPhy->pBsRx->Nrx_antennas * 14 * step_aDet) - (step_aDet*8);   // Last Symbol in Slot 0 for last Ant
    pPhy->pBsRx->pFftCell[0] = (U32*) &pPhy->pBsRx->aDet[i + (2 * K)];
    for(i=0; i<fftDependencyListCnt[0]; i++)
    {
        RexAddCellDep (pFftDependencyList[0][i], (UINT32) pPhy->pBsRx->pFftCell[0], (UINT32) RX_DEP_FFT_INIT_VAL);
    }
#endif

    // FFT Cell Depndency Slot 1
    i = (pPhy->pBsRx->Nrx_antennas * 14 * step_aDet) - (step_aDet*1);   // Last Symbol in Slot 1 for last Ant
    pPhy->pBsRx->pFftCell[1] = (U32*) &pPhy->pBsRx->aDet[i + (2 * K)];
    for(i=0; i<fftDependencyListCnt[1]; i++)
    {
        RexAddCellDep (pFftDependencyList[1][i], (UINT32) pPhy->pBsRx->pFftCell[1], (UINT32) RX_DEP_FFT_INIT_VAL);
    }

    // IDFT Cell Dependency
#ifdef IDFT_CELL_DEPENDENCY
    for(i=0; i<NUM_IDFT_CELL_DEPENDENCY; i++)
    {
        j = 12-NUM_IDFT_CELL_DEPENDENCY+i;
        if (j >= 11)
            j = 11;
        j *= step_reDet;   // Last Symbol as input to demapper
        pPhy->pBsRx->pIdftCell[i] = (U32*) &pRxPtr->pReDet0[j + (2 * (K + 4*(MAXCHSUBFRAME-1)))];
        RexAddCellDep (pIdftDependencyList[i], (UINT32) pPhy->pBsRx->pIdftCell[i], (UINT32) RX_DEP_IDFT_INIT_VAL);
    }
    RexReinitCells((UINT32*)pPhy->pBsRx->pIdftCell, NUM_IDFT_CELL_DEPENDENCY);
#endif

    // Register Call Back Function
    RegListDoneCB(pBsRx->pExeTaskList[0], LteRxListDoneCb, NULL);
    pBsRx->rxListId =  RexGetListID(pBsRx->pExeTaskList[0]);

#endif

    return 0;
}

//-------------------------------------------------------------------------------------------
/** @brief Update the Base Station Uplink Rx Static task lists. (PUSCH, PUCCH, PRACH, SRS)
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range |Units|
 *  |              |Standard |              |         |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  |pPhy          |         |pInput/pOutput|1        |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------------
 *  |Block Name          |CreateUlRxTaskList       |
 *  ------------------------------------------------
 *  |Input Stream        |none                     |
 *  ------------------------------------------------
 *  |Output Stream       |none                     |
 *  ------------------------------------------------
 *  |Config Parameters   |pPhy                     |
 *  ------------------------------------------------
 *  |Status Parameters   |pPhy                     |
 *  ------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Create the Base Station Uplink Rx Static task list.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 UpdateUlRxTaskList(PLTEPHYDESC pPhy)
{
    phy_bs_rx(pPhy);

    return 0;
}


//-------------------------------------------------------------------------------------------
/** @brief Update / Run the Base Station Uplink Rx Static task list. (PUSCH, PUCCH, PRACH, SRS)
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param pPhy - INPUT/OUTPUT, Pointer to current PHY running
 *
 *  @return status,
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------
 *  |Name       |Name in  |Direction     |Length   |Type        |Data Format |Valid Range |Units|
 *  |           |Standard |              |         |            |            |            |     |
 *  ---------------------------------------------------------------------------------------------
 *  |pPhy       |         |pInput/pOutput|1        |pStructure  |            |            |     |
 *  ---------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |UpdateUlRxTaskList        |
 *  --------------------------------------
 *  |Input Streams       |None           |
 *  --------------------------------------
 *  |Output Streams      |None           |
 *  --------------------------------------
 *  |Config Parameters   |pPhy           |
 *  --------------------------------------
 *  |Status Parameters   |pPhy           |
 *  --------------------------------------
 *  BLOCK_END
 *
 * <4> Detailed Description:
 * This function is used to update the tasklist for Context N+1 while list in Context N is being processed
 * in the Hardware. Used only for the RealTime Project
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 phy_bs_rx(PLTEPHYDESC pPhy)
{
    // PHY Structures
    BS *pBs;
    BS_RX *pBsRx;
    BS_SRS *pBsSrs;
    PBS_RX_POINTERS pRxPtr = pPhy->pBsRx->pBsRxPointers;
    ULRXINPUTPARAMS *pUlRxInputParams;
    PULRXSDUPARAMS   pUlRxSduParams;
    PULRXFECPARAMS   pFecParams;

    // General Variables
    S32 i, j, k;
    S16 mo, isym;
    S32 length_adc;
    S16 cond = 0;

    S16 MI, MO;
    S16 Nul_symb;
    S32 numPusch;
    S16 K, L;
    S16 pilotCarrierSymbol, srsPilot;
    S32 step_aDet, step_reDet;
    S16 numPucchRun[NUM_USER_PUCCH_TCBS];
    S32 length_r_alpha_uv;
    S32 IndPucch1, IndPucch2, numMultiChanTCBs;
    S16 *zczLUT;

    // Task List Related
    PTCB pTcbListTmp;
    PTCB *pTcbList1 = pPhy->pBsRx->pTaskRx1;
    PTCB *pTcbList2 = pPhy->pBsRx->pTaskRx2;
    U16 NumTasksTotalRX = 0;

    ////////////////////////////////////////////////////////////////////////////
    // Initialize
    ////////////////////////////////////////////////////////////////////////////
    pBs = pPhy->pBs;
    pBsRx = pPhy->pBsRx;
    pBsSrs = pBs->pBsSrs;
    pUlRxInputParams = pBsRx->pUlRxInputParams;
    pUlRxSduParams = pBsRx->pUlRxSduParams;

    i                = pBsRx->RxFecParam_Context;
    pFecParams       = pBsRx->pUlRxFecParams[i];

    Nul_symb = pBsRx->Nul_symb;
    K = pBsRx->Nul_rb * pBs->Nrb_sc; //total number of frequency indices in resource grid
    L = 2 * Nul_symb; //total number of time indices in resource grid subframe
    length_r_alpha_uv = 7 * NUM_SLOTS_SUBFRM * 2 * PRB_12;
    srsPilot = pBsSrs->srsPilotSymbol;

    length_adc = 2 * ((pBs->Ncp0 + pBs->Nfft) + 6 * (pBs->Ncp + pBs->Nfft));

    pilotCarrierSymbol = CA_ULPilotCarrierSymbol[pBs->CPType];
    MI = pUlRxInputParams->MI;
    MO = pUlRxInputParams->MO;
    pUlRxInputParams->MO = pUlRxInputParams->MO_1;
    for (i = 0; i < MO; i++)
    {
        pRxPtr->ppSnrPerRb_Pilot[2 * i] = (S32 *) &(pRxPtr->pSnrPerRb[100 * (2 * i)]);
        pRxPtr->ppSnrPerRb_Pilot[2 * i + 1] = (S32 *) &(pRxPtr->pSnrPerRb[100 * (2 * i + 1)]);
    }

    step_aDet = 2 * RUP8B(K + 2);
    step_reDet = 2 * RUP8B(K + 4*MAXCHSUBFRAME);

    ////////////////////////////////////////////////////////////////////////////
    // Initializations END
    ////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // UPDATE TASK LIST
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // FFT
    ////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
    fft_Win32(pBs->expoNfft, pBs->Nfft,
              pBsRx->adcbuf, pBsRx->aDet,
              Nul_symb, MO, step_aDet, L, K,
              pBs->Ncp, pBs->Ncp0);
#endif // ifdef _WIN32

    ////////////////////////////////////////////////////////////////////////////
    // PRACH LIST
    ////////////////////////////////////////////////////////////////////////////
    i = 0;
    pTcbListTmp = pTcbList2[0];
    if (pPhy->pBs->pBsRxPrach[i]->bs_prach_preambleSubframeEnable[pBsRx->subframeNumber] == 1)
    {
        PRACH_Rx_Init(pPhy->pBs->pBsRxPrachCram[i], pPhy->pBs->pBsRxPrach[i], pPhy->pBs->pBsRxPrachDet[i], length_adc, MO);

        zczLUT = zeroCorrZone13;
        switch(pPhy->pBs->pBsRxPrachCram[i]->Ncs)
        {
            case 13: zczLUT = zeroCorrZone13; break;
            case 15: zczLUT = zeroCorrZone15; break;
            case 18: zczLUT = zeroCorrZone18; break;
            case 22: zczLUT = zeroCorrZone22; break;
            case 26: zczLUT = zeroCorrZone26; break;
            case 32: zczLUT = zeroCorrZone32; break;
            case 38: zczLUT = zeroCorrZone38; break;
            case 46: zczLUT = zeroCorrZone46; break;
            case 59: zczLUT = zeroCorrZone59; break;
            case 76: zczLUT = zeroCorrZone76; break;
            case 93: zczLUT = zeroCorrZone93; break;
            case 119: zczLUT = zeroCorrZone119; break;
            case 167: zczLUT = zeroCorrZone167; break;
            case 279: zczLUT = zeroCorrZone279; break;
            case 419: zczLUT = zeroCorrZone419; break;
            case 0: zczLUT = zeroCorrZone0; break;
        }
#ifndef PHY_NEW_DI
        PRACH_Rx_tcb(pBsRx, pBs->pBsRxPrach[i], pBs->pBsRxPrachDet[i], MO,
                     pBsRx->adcbuf, length_adc, PRACH_Tables,
                     pBs->pBsRxPrachCram[i], NULL, TCBIOBUFREFILL, pTcbListTmp, zczLUT);
#else
        PRACH_Rx_tcb(pBsRx, pBs->pBsRxPrach[i], pBs->pBsRxPrachDet[i], MO,
                     pBsRx->adcbuf_rt, length_adc, PRACH_Tables,
                     pBs->pBsRxPrachCram[i], NULL, TCBIOBUFREFILL, pTcbListTmp, zczLUT);

#endif

        pBsRx->isExePrachTaskListReady[i] = TRUE;
    }
    else
    {
        pBsRx->isExePrachTaskListReady[i] = FALSE;
    }

    ////////////////////////////////////////////////////////////////////////////
    // SRS LIST
    ////////////////////////////////////////////////////////////////////////////
    if (pBsSrs->srsSubframeEnable[pBsRx->subframeNumber] == 1 && pBsRx->numSrsChan > 0)
    {
        UpdateUlRxSrsTaskList(pPhy);
        pBsRx->isExeSrsTaskListReady = TRUE;
    }
    else
    {
        pBsRx->isExeSrsTaskListReady = FALSE;
        srsPilot = -1;      //if no SRS, set this to -1
    }

    /////////////////////////////////////////////////////////////////////////////////////
    //PUCCH Uplink Pilots - generate pilots for all Users
    /////////////////////////////////////////////////////////////////////////////////////
    ul_Pilot_PUCCH_tcb(NULL, pRxPtr->bs_aPilots_pucch,
                       pBsRx->rs_grid, pRxPtr->r_alpha_uv,
                       length_r_alpha_uv, step_aDet,
                       pRxPtr->nOc, pRxPtr->Sdescramb,
                       pUlRxInputParams, NULL,
                       TCBIOBUFREFILL, pTcbList1[NumTasksTotalRX]);
    NumTasksTotalRX++;

    //////////////////////////////////////////////////////////////////////////////
    //Channel Estimation for PUCCH
    //////////////////////////////////////////////////////////////////////////////
    for(mo = 0; mo < MO; mo++)
    {
        //////////////////////////////////////////////////////////////////////////////
        //Channel Estimation for first slot
        //////////////////////////////////////////////////////////////////////////////
        bs_rx_ChEstimator_PUCCH_part0_tcb(NULL,
                                    &pBsRx->aDet[mo * step_aDet * L], pBsRx->rs_grid,
                                    pRxPtr->bs_aPilots_pucch, pUlRxInputParams, 0, mo,
                                    &pRxPtr->pExpo_ch_pucch[(mo * L)],
                                    step_aDet, pRxPtr->pErrAvg,
                                    ScaleComputeErr_Tab, &pRxPtr->pChEst_ul_pucch[mo * 2 * 2 * PRB_12 * MAX_PUCCH_PILOTSYMB_PER_SUBF * MAXCHSUBFRAME],
                                     pRxPtr->pErrSub_pucch, pRxPtr->pErrSubExpo_pucch,
                                    NULL, NULL, 0,
                                    TCBIOBUFREFILL, pTcbList1[NumTasksTotalRX]);
        NumTasksTotalRX++;

        //////////////////////////////////////////////////////////////////////////////
        //Channel Estimation for second slot
        //////////////////////////////////////////////////////////////////////////////
        bs_rx_ChEstimator_PUCCH_part0_tcb(NULL,
                                    &pBsRx->aDet[mo * step_aDet * L], pBsRx->rs_grid,
                                    pRxPtr->bs_aPilots_pucch, pUlRxInputParams, 1, mo,
                                    &pRxPtr->pExpo_ch_pucch[(mo * L)],
                                    step_aDet,  pRxPtr->pErrAvg,
                                     ScaleComputeErr_Tab,&pRxPtr->pChEst_ul_pucch[mo * 2 * 2 * PRB_12 * MAX_PUCCH_PILOTSYMB_PER_SUBF * MAXCHSUBFRAME],
                                    pRxPtr->pErrSub_pucch, pRxPtr->pErrSubExpo_pucch,
                                    NULL, NULL, 0,
                                    TCBIOBUFREFILL, pTcbList1[NumTasksTotalRX]);
        NumTasksTotalRX++;
    }

    //////////////////////////////////////////////////////////////////////////////////
    //Averaging of TimingErr, depends on ChanEst for PUCCH  to finish before running
    //////////////////////////////////////////////////////////////////////////////////
    pTcbList1[NumTasksTotalRX] = RunningAveragePUCCH_tcb(NULL, pUlRxInputParams, pRxPtr->pErrAvg, &pRxPtr->pExpo_ch_pucch[0],
                                                      pRxPtr->pErrAvgOut,  pRxPtr->pErrSub_pucch, pRxPtr->pErrSubExpo_pucch, NULL, NULL, 0, TCBIOBUFREFILL,
                                                     pTcbList1[NumTasksTotalRX]);
    NumTasksTotalRX++;

    for(mo = 0; mo < MO; mo++)
    {
        //////////////////////////////////////////////////////////////////////////////
        //Channel Estimation for first slot
        //////////////////////////////////////////////////////////////////////////////
        bs_rx_ChEstimator_PUCCH_tcb(NULL,
                                    &pBsRx->aDet[mo * step_aDet * L], pBsRx->rs_grid,
                                    &pRxPtr->pChEst_ul_pucch[mo * 2 * 2 * PRB_12 * MAX_PUCCH_PILOTSYMB_PER_SUBF * MAXCHSUBFRAME], pUlRxInputParams, 0, mo,
                                    &pRxPtr->pExpo_ch_pucch[(mo * L)],
                                    step_aDet, pRxPtr->r_alpha_uv, length_r_alpha_uv,
                                    pRxPtr->Sdescramb, pRxPtr->nOc,
                                    -1, pRxPtr->pAvgReDet_pucchACK, pRxPtr->pAvgReDet_pucchCQI, pRxPtr->pChEst_pucch,
                                    pRxPtr->rxInPwrAvg_PerRbPilot, pRxPtr->pPwrAvg_Pilot_expo,
                                    pRxPtr->rxInPwrAvg_PerRbDataSym, pRxPtr->pPwrAvg_Data_expo,
                                    pRxPtr->pAllPucchPwrAvg_AntSlot, pRxPtr->pPucchPwrAvg, pRxPtr->pRxInRbPwrAvg_AntSlot, pRxPtr->pRbUsedFlag,
                                    pRxPtr->pRxRssi_AntSlot, pRxPtr->pErrAvgOut,
                                    NULL, TCBIOBUFREFILL, pTcbList1[NumTasksTotalRX]);
        NumTasksTotalRX++;

        //////////////////////////////////////////////////////////////////////////////
        //Channel Estimation for second slot
        //////////////////////////////////////////////////////////////////////////////
        bs_rx_ChEstimator_PUCCH_tcb(NULL,
                                    &pBsRx->aDet[mo * step_aDet * L], pBsRx->rs_grid,
                                    &pRxPtr->pChEst_ul_pucch[mo * 2 * 2 * PRB_12 * MAX_PUCCH_PILOTSYMB_PER_SUBF * MAXCHSUBFRAME], pUlRxInputParams, 1, mo,
                                    &pRxPtr->pExpo_ch_pucch[(mo * L)],
                                    step_aDet,  pRxPtr->r_alpha_uv, length_r_alpha_uv,
                                    pRxPtr->Sdescramb, pRxPtr->nOc,
                                    srsPilot, pRxPtr->pAvgReDet_pucchACK, pRxPtr->pAvgReDet_pucchCQI,pRxPtr->pChEst_pucch,
                                    pRxPtr->rxInPwrAvg_PerRbPilot, pRxPtr->pPwrAvg_Pilot_expo,
                                    pRxPtr->rxInPwrAvg_PerRbDataSym, pRxPtr->pPwrAvg_Data_expo,
                                    pRxPtr->pAllPucchPwrAvg_AntSlot, pRxPtr->pPucchPwrAvg, pRxPtr->pRxInRbPwrAvg_AntSlot, pRxPtr->pRbUsedFlag,
                                    pRxPtr->pRxRssi_AntSlot, pRxPtr->pErrAvgOut,
                                    NULL, TCBIOBUFREFILL, pTcbList1[NumTasksTotalRX]);
        NumTasksTotalRX++;
    }

    //////////////////////////////////////////////////////////////////////////////
    //User base MRC, FEQ, CazacAvg and DeMapper
    //////////////////////////////////////////////////////////////////////////////
    IndPucch1 = pUlRxInputParams->numPUSCH;
    IndPucch2 = IndPucch1 + pUlRxInputParams->numPUCCH;
    memset(numPucchRun, 0, sizeof(S16)*NUM_USER_PUCCH_TCBS);
    k = 0;
    for(i = IndPucch1; i < IndPucch2; k++)
    {
        for(j = 0; j < NUM_USER_PUCCH_TCBS; j++, i++)
        {
            if (i >= IndPucch2)
                break;

            pRxPtr->pucchChan[j][k] = (S16)i;
            numPucchRun[j]++;
        }
    }

    for(i = 0; i < NUM_USER_PUCCH_TCBS; i++)
    {
        rx_Multichan_DeModulation_Pucch_tcb(NULL, pBsRx->aDet, pRxPtr->pExpo_ch_pucch,
                                            pUlRxInputParams, pUlRxSduParams,
                                            pRxPtr->pucchChan[i], numPucchRun[i],
                                            pPhy->descrInBufs, pPhy->descrIndex,
                                            pRxPtr->pAvgReDet_pucchACK,
                                            pRxPtr->pAvgReDet_pucchCQI, pRxPtr->pSnrPerRb,
                                            pRxPtr->pSnrPilot, pRxPtr->pChEst_pucch,
                                            pRxPtr->rxInPwrAvg_PerRbPilot, pRxPtr->pPwrAvg_Pilot_expo,
                                            pRxPtr->rxInPwrAvg_PerRbDataSym, pRxPtr->pPwrAvg_Data_expo,
                                            pRxPtr->pAllPucchPwrAvg_AntSlot, pRxPtr->pPucchPwrAvg,
                                            rm_dectab_20_X, rm_dectab_20_Offset,
                                            NULL, NULL, 0, TCBIOBUFREFILL, pTcbList1[NumTasksTotalRX]);
        NumTasksTotalRX++;
    }

    /////////////////////////////////////////////////////////////////////////////////////
    //PUSCH Uplink Pilots - generate pilots for all Users
    /////////////////////////////////////////////////////////////////////////////////////
    numPusch = pUlRxInputParams->numPUSCH;
    ul_Pilot_fx_tcb(NULL, pBsRx->bs_aPilots, pUlRxInputParams,
                    pBsRx->pBsPusch->cvec_u, pBsRx->pBsPusch->PRSbits,
                    Bit_Reversal, UlPilots_QthRootTable,
                    pBsRx->rs_grid, step_aDet,
                    NULL, TCBIOBUFREFILL, pTcbList1[NumTasksTotalRX]);
    NumTasksTotalRX++;


    for (mo = 0; mo < MO; mo++)
    {
        ///////////////////////////////////////////////////////////////////////////////////////////
        //Channel estimation from pilots, for every subcarrier in the resource element matrix, for this user, for the complete slot
        //Compute Channel Estimate at pilots, computation of derotation vector
        //Derotation of Channel at pilots
        //do least squares straight line fit to these pilot channel estimates to do some noise averaging
        ///////////////////////////////////////////////////////////////////////////////////////////


        ///////////////////////////////////////////////////////////////////////////////////////////
        //PUSCH Channel Estimator, Slot 0
        ///////////////////////////////////////////////////////////////////////////////////////////
        pTcbList1[NumTasksTotalRX] = bs_rx_ChEstimator_part0_fx_tcb(NULL, &pBsRx->aDet[mo * step_aDet * L],
                                             pBsRx->rs_grid, pBsRx->bs_aPilots,
                                             pUlRxInputParams, step_aDet, &pRxPtr->pChEst_ul[mo * MI * 2 * K * 2],
                                             ScaleComputeErr_Tab, pRxPtr->pErrAvg, mo, 0,
                                             &pRxPtr->pExpo_mat[2*mo],
                                             NULL, NULL, 0, TCBIOBUFREFILL, pTcbList1[NumTasksTotalRX]);
        NumTasksTotalRX++;

        ///////////////////////////////////////////////////////////////////////////////////////////
        //PUSCH Channel Estimator, Slot 1
        ///////////////////////////////////////////////////////////////////////////////////////////
        pTcbList1[NumTasksTotalRX] = bs_rx_ChEstimator_part0_fx_tcb(NULL, &pBsRx->aDet[mo * step_aDet * L],
                                             pBsRx->rs_grid, pBsRx->bs_aPilots,
                                             pUlRxInputParams, step_aDet, &pRxPtr->pChEst_ul[(mo * MI * 2 * K * 2)],
                                             ScaleComputeErr_Tab, pRxPtr->pErrAvg, mo, 1,
                                             &pRxPtr->pExpo_mat[2*mo],
                                             NULL, NULL, 0, TCBIOBUFREFILL, pTcbList1[NumTasksTotalRX]);
        NumTasksTotalRX++;
        ///////////////////////////////////////////////////////////////////////////////////////////
    } //for each antenna, for (mo = 0; mo < MO; mo++)


    //////////////////////////////////////////////////////////////////////////////////
    //Averaging of TimingErr, depends on ChanEst for PUSCH  to finish before running
    //////////////////////////////////////////////////////////////////////////////////
    pTcbList1[NumTasksTotalRX] = RunningAverage_tcb(NULL, pUlRxInputParams, pRxPtr->pErrAvg, &pRxPtr->pExpo_mat[0],
                                                     pRxPtr->pErrExpo, pRxPtr->pErrAvgOut, pRxPtr->mErrExpoTemp,
                                                     pRxPtr->mErrTemp, 2, NULL, NULL, 0, TCBIOBUFREFILL,
                                                     pTcbList1[NumTasksTotalRX]);
    NumTasksTotalRX++;


    ///////////////////////////////////////////////////////////////////////////////////////////
    //Channel estimation from pilots, for every subcarrier in the resource element matrix, for this user, for the complete slot
    //Compute Channel Estimate at pilots, computation of derotation vector
    //Derotation of Channel at pilots
    //do least squares straight line fit to these pilot channel estimates to do some noise averaging
    ///////////////////////////////////////////////////////////////////////////////////////////
    for (mo = 0; mo < MO; mo++)
    {
        ///////////////////////////////////////////////////////////////////////////////////////////
        //PUSCH Channel Estimator, Slot 0
        ///////////////////////////////////////////////////////////////////////////////////////////
        bs_rx_ChEstimator_part1_fx_tcb(NULL,  &pBsRx->derotation[0],
                     pUlRxInputParams, step_aDet, &pRxPtr->pChEst_ul[mo * MI * 2 * K * 2],
                      pRxPtr->pErrAvgOut, mo, 0,
                     pRxPtr->ppSnrPerRb_Pilot[2 * mo], pRxPtr->pSnrPilot, pRxPtr->pnoiseVar,
                     pRxPtr->mErrExpoTemp, pRxPtr->mErrTemp, pRxPtr->pRxInRbPwrAvg_AntSlot, pRxPtr->pRbUsedFlag,
                     pRxPtr->pRxRssi_AntSlot, NULL, TCBIOBUFREFILL, pTcbList1[NumTasksTotalRX]);
        NumTasksTotalRX++;

        ///////////////////////////////////////////////////////////////////////////////////////////
        //PUSCH Channel Estimator, Slot 1
        ///////////////////////////////////////////////////////////////////////////////////////////
        bs_rx_ChEstimator_part1_fx_tcb(NULL,  &pBsRx->derotation[0],
                     pUlRxInputParams, step_aDet, &pRxPtr->pChEst_ul[(mo * MI * 2 * K * 2)],
                     pRxPtr->pErrAvgOut, mo, 1,
                     pRxPtr->ppSnrPerRb_Pilot[2 * mo + 1], pRxPtr->pSnrPilot, pRxPtr->pnoiseVar,
                     pRxPtr->mErrExpoTemp, pRxPtr->mErrTemp, pRxPtr->pRxInRbPwrAvg_AntSlot, pRxPtr->pRbUsedFlag,
                     pRxPtr->pRxRssi_AntSlot, NULL,  TCBIOBUFREFILL, pTcbList1[NumTasksTotalRX]);
        NumTasksTotalRX++;

        ///////////////////////////////////////////////////////////////////////////////////////////
    } //for each antenna, for (mo = 0; mo < MO; mo++)

    //////////////////////////////////////////////////////////////////////////////////
    //Compute (1) average subframe SNR per antenna, (2) average subframe SNR per RB
    //////////////////////////////////////////////////////////////////////////////////
    // Do SNR related final calculation and put them into RxInput structure
    SnrSubframe_tcb(NULL, pUlRxInputParams, pRxPtr->pSnrPerRb, pRxPtr->pSnrPilot,
                    NULL, NULL, 0, TCBIOBUFREFILL, pTcbList1[NumTasksTotalRX]);
    NumTasksTotalRX++;

    //////////////////////////////////////////////////////////////////////////////////
    //Compute  (1) RIP per RB (2) RSSI for PUSCH and PUCCH (3) Thermal Noise
    //////////////////////////////////////////////////////////////////////////////////
    RipRssiSubframe_tcb(NULL, pUlRxInputParams,  pBsRx->aDet, pRxPtr->pRxInRbPwrAvg_AntSlot, pRxPtr->pRbUsedFlag, pRxPtr->pRxInRip,
		                   step_aDet, srsPilot, pRxPtr->pInitFlag, pRxPtr->pExpo_mat, pRxPtr->pRxRssi_AntSlot,
                    NULL, NULL, 0, TCBIOBUFREFILL, pTcbList1[NumTasksTotalRX]);
    NumTasksTotalRX++;

    //////////////////////////////////////////////////////////////////////////////////
    // Combine all antenna signals into one signal per user
    // Note: for now, assume do one slot at a time. But, could minimize latency by waiting only for the
    //      reference symbol. So here do two passes through this loop, one for each slot...
    // All CH symbols have same exponent within a slot and per rx antenna
    // This is due to the way Ch estimation was done
    //
    // Detect nearest constellation point with maximal ratio combining of all antenna values
    //
    // Symbol-based MRC
    //////////////////////////////////////////////////////////////////////////////////
    for (isym = 0; isym < Nul_symb; isym++)
    {
        cond = 0;
        cond = (isym == pilotCarrierSymbol);

        if (cond == 0)
        {
            rx_Multichan_DeModulation_tcb(NULL, pBsRx->aDet,
                    pBsRx->rs_grid, pRxPtr->pChEst_ul, pBsRx->derotation,
                    pRxPtr->pReDet0, pRxPtr->pExpo_norm_reEst, step_aDet,
                    step_reDet, isym, pUlRxInputParams, srsPilot,
                    pRxPtr->pnoiseVar, NULL, NULL, 0,
                    TCBIOBUFREFILL, pTcbList1[NumTasksTotalRX]);
            NumTasksTotalRX++;
        }
    }

    numMultiChanTCBs = pBsRx->isym_eff[0];
    //////////////////////////////////////////////////////////////////////////////////
    //Run IDFT DeSpreader
    //Undo the Transform Precoding (i.e. DFT Spreading) of the complex constellation points
    //////////////////////////////////////////////////////////////////////////////////
    IDFT_tcb(NULL, pRxPtr->pReDet0, pRxPtr->pReDet0, (S16)(2 * numMultiChanTCBs), srsPilot, NULL,
             step_reDet, (U16 *)pUlRxInputParams->Mpusch_sc, (S16)numPusch, pUlRxInputParams->TotalMsc,
             NULL, TCBIOBUFREFILL, pTcbList1[NumTasksTotalRX]);

    NumTasksTotalRX++;

    //////////////////////////////////////////////////////////////////////////////////
    //Rx DeMapper to generate soft decisions
    //////////////////////////////////////////////////////////////////////////////////
    for (isym = 0; isym < numMultiChanTCBs; isym++)
    {
        rx_Demapper_fx_tcb(NULL, pRxPtr->pReDet0,
                   pRxPtr->pExpo_norm_reEst, pFecParams, pUlRxInputParams,
                   isym, srsPilot, step_reDet,
                   pPhy->descrInBufs, (S32*) pPhy->descrIndex,
                   pRxPtr->pMuxCtrl_SoftDec, NULL, TCBIOBUFREFILL,
                   pTcbList1[NumTasksTotalRX]);
        NumTasksTotalRX++;
    }

    muxCntlACK_Dec_tcb(NULL, pFecParams, pRxPtr->pMuxCtrl_SoftDec, NULL, NULL, NULL,
                        TCBIOBUFREFILL, pTcbList1[NumTasksTotalRX]);

    muxCntlRI_CQI_Dec_tcb(NULL, pFecParams, pRxPtr->pMuxCtrl_SoftDec, NULL, NULL, NULL,
                        TCBIOBUFREFILL, pBsRx->pMuxCtrlTcbCtx1);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // UPDATE TASK LIST END
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    pUlRxInputParams->MO = MO;

    pPhy->pBsRx->isExeTaskListReady = TRUE;

    return 0;
}


//-------------------------------------------------------------------------------------------
/** @brief Pre-Execute BaseStation PUSCH / PUCCH Rx TaskTask
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range                 |Units|
 *  |              |Standard |              |         |            |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |pPhy          |         |pInput/pOutput|1        |pStructure  |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |PhyBsRxPreExec                   |
 *  --------------------------------------------------------
 *  |Input Stream        |none                             |
 *  --------------------------------------------------------
 *  |Output Stream       |none                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  |Status Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Use MDMA to copy incoming IQ samples for current subframe from
 *  DDR to CRAM.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsRxPreExec(PLTEPHYDESC pPhy)
{
    U32 status = 0;
    PBS pBs;
    PBS_RX pBsRx;

    S32 length_adc;
    S16 i;

    pBs = pPhy->pBs;
    pBsRx = pPhy->pBsRx;

    pPhy->pBsRx->isExeTaskListReady = FALSE;

    // Sizeof each samples per antenna in half words  (1st 2 => I and Q, 2nd 2 => 2 slots)
    length_adc = 2 * 2 * ((pBs->Nfft + pBs->Ncp0) + 6 * (pBs->Nfft + pBs->Ncp));

    for (i = 0; i < pBsRx->Nrx_antennas; i++)
    {
        memcpy(pBsRx->adcbuf[i], &pPhy->sigIn[0][length_adc*i], length_adc * sizeof(S16));
    }

    ////////////////////////////////////////////////////////////////////////////
    // C-Only Project Initialize
    ////////////////////////////////////////////////////////////////////////////
    if (pPhy->pBsRx->numMS > 1)
    {
        phy_printf("error in phy_bs_rx(): MIMO not supported yet\n");
    }

#ifdef DEBUG_PRINT
    {
        U32 isym, mo, i;
        for(isym=0; isym<14; isym++)
        {
            for(mo=0; mo<4; mo++)
            {
                for(i=0; i<10; i++)
                {
                    debugExpo[isym][mo][i] = 0;
                }
            }
        }
        numDebugExpo = 0;
    }
#endif
    ////////////////////////////////////////////////////////////////////////////
    // C-Only Project Initialize  END
    ////////////////////////////////////////////////////////////////////////////

    return status;
}



//-------------------------------------------------------------------------------------------
/** @brief Execute Call Back function BaseStation PUSCH / PUCCH Rx TaskTask
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  0 if SUCCESS
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range |Units|
 *  |              |Standard |              |         |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  |pTaskList   |         |pInput        |1        |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  |pPhy          |         |pInput/pOutput|1        |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |PhyBsRxExeTaskList               |
 *  --------------------------------------------------------
 *  |Input Stream        |none                             |
 *  --------------------------------------------------------
 *  |Output Stream       |none                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  |Status Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Top level function that is called from Scheduler after PUSCH / PUCCH list is run to completion
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsRxExeCB(PTASKLIST pTaskList, PLTEPHYDESC pPhy)
{
    pPhy->pBsRx->isExeTaskListReady = FALSE;
    return 0;
}


//-------------------------------------------------------------------------------------------
/** @brief Execute BaseStation PUSCH / PUCCH Rx TaskTask
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range |Units|
 *  |              |Standard |              |         |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  |pPhy          |         |pInput/pOutput|1        |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |PhyBsRxExeTaskList               |
 *  --------------------------------------------------------
 *  |Input Stream        |none                             |
 *  --------------------------------------------------------
 *  |Output Stream       |none                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  |Status Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Top level function to execute all BaseStation Uplink Rx task lists
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsRxExeTaskList(PLTEPHYDESC pPhy)
{
    if (pPhy->pBsRx->isExeTaskListReady != TRUE)
    {
        phy_printf("PUSCH / PUCCH List Not Ready!!\n");
        return 0;
    }

    if (pPhy->pBsRx->pUlRxFecParams[pPhy->pBsRx->RxFecParam_Context]->numChan)
    {
#ifndef _WIN32
        RegListDoneCB(pPhy->pBsRx->pExeTaskList[0], (fListDoneCB*)PhyBsRxExeCB, pPhy);
#endif
        ExeTSchedList(pPhy->pBsRx->pExeTaskList[0]);
    }
    else
    {
        pPhy->pBsRx->isExeTaskListReady = FALSE;
    }

    return 0;
}


//-------------------------------------------------------------------------------------------
/** @brief Post-Execute BaseStation PUSCH / PUCCH Rx TaskTask
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range                 |Units|
 *  |              |Standard |              |         |            |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |pPhy          |         |pInput/pOutput|1        |pStructure  |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |PhyBsRxPostExec                  |
 *  --------------------------------------------------------
 *  |Input Stream        |none                             |
 *  --------------------------------------------------------
 *  |Output Stream       |none                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  |Status Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Freeing of dynamic memory after Base Station Uplink Rx task list is
 *  completed.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsRxPostExec(PLTEPHYDESC pPhy)
{
    UINT32 t = GET_TICKS;

    _ASSERT_PTR(pPhy);
    _ASSERT_PTR(pPhy->pBsRx->pUlRxInputParams);

    ////////////////////////////////////////////////////////////////////////////
    // DEBUG FILES
    ////////////////////////////////////////////////////////////////////////////

#ifdef DEBUG_PRINT
    {
        double max = 0.0;
        U32 step_reDet, i, offset;

        PBS_RX_POINTERS pRxPtr;
        S16 TotalMsc, isym, mo, Msc;
        S16 MI, MO, numPucch;
        S16 K, L;
        S16 Nul_symb, numPilotSym;
        U32 indPucch1, indPucch2;
        ULRXINPUTPARAMS *pUlRxInputParams = NULL;

        pUlRxInputParams = pPhy->pBsRx->pUlRxInputParams;
        pRxPtr = pPhy->pBsRx->pBsRxPointers;
        TotalMsc = pUlRxInputParams->TotalMsc;
        Nul_symb = pPhy->pBsRx->Nul_symb;
        K = pPhy->pBsRx->Nul_rb * pPhy->pBs->Nrb_sc; //total number of frequency indices in resource grid
        L = 2 * Nul_symb; //total number of time indices in resource grid subframe
        MO = pUlRxInputParams->MO;
        MI = pUlRxInputParams->MI;
        step_reDet = 2 * RUP8B(K + 4*MAXCHSUBFRAME);
        numPucch = pUlRxInputParams->numPUCCH;

        ///////////////////////////////////////////////////////////////
        //Printing Uplink Pilots for all PUSCH
        ///////////////////////////////////////////////////////////////
        offset = 0;
        for(i=0; i < (U32)pUlRxInputParams->numPUSCH; i++)
        {
            Msc = pUlRxInputParams->Mpusch_sc[i];
            filePrint(fUl_Pilots, &pPhy->pBsRx->bs_aPilots[offset], 2*Msc, "S16", "Fix2Fl_Complex",
                    (double)pow(2.0, 14.0), LOG_BS_ULPILOTS_PUSCH_FX);
            offset += 4*Msc;
        }

        ///////////////////////////////////////////////////////////////
        //Printing Uplink Pilots for all PUCCH
        ///////////////////////////////////////////////////////////////
        offset = 0;
        Msc = pUlRxInputParams->Nrb_sc;
        indPucch1 = (U32)pUlRxInputParams->numPUSCH;
        indPucch2 = (U32)(indPucch1 + pUlRxInputParams->numPUCCH);
        for(i = indPucch1; i < (U32)indPucch2; i++)
        {
            numPilotSym = pUlRxInputParams->num_Pilots_slot[i];

            filePrint(fUl_PilotsPucch, &pRxPtr->bs_aPilots_pucch[offset], 4*Msc*numPilotSym, "S16", "Fix2Fl_Complex",
                        (double)pow(2.0, 14.0), LOG_BS_ULPILOTS_PUCCH_FX);
            offset += 4*Msc*numPilotSym;
        }

        ///////////////////////////////////////////////////////////////
        //Printing Channel Estimate for all Users
        ///////////////////////////////////////////////////////////////
        Msc = pUlRxInputParams->Nrb_sc;
        for(mo=0; mo < MO; mo++)
        {
            filePrint(fChanEst_p1, &pRxPtr->pChEst_ul[mo*MI*2*K*2], TotalMsc, "S16",
                        "Fix2Fl_Complex", (double)(pow(2., 12.)), LOG_BS_CHANESTP1_PUSCH_FX);
            filePrint(fChanEst_p1, &pRxPtr->pChEst_ul[mo*MI*2*K*2 + 2*TotalMsc], TotalMsc, "S16",
                        "Fix2Fl_Complex", (double)(pow(2., 12.)), LOG_BS_CHANESTP1_PUSCH_FX);
        }

        ///////////////////////////////////////////////////////////////
        //Printing DeMapper Input for all Users
        ///////////////////////////////////////////////////////////////
        for (isym = 0; isym < pPhy->pBsRx->isym_eff[0]; isym++)
        {
            max = pow(2., (double)(debugExpo[isym][0][6]+debugExpo[isym][0][2]));
            filePrint(fRxMapperIn, &pRxPtr->pReDet0[isym * step_reDet], TotalMsc,
                        "S16", "Fix2Fl_Complex", (double)(pow(2.,11.)/max), LOG_BS_DEMAPPERIN_PUSCH_FX);
        }
    }
#endif

#ifdef DEBUG_POWER_RX_SIGNAL
    {
        UINT32 mlogDebug[10], mlogDebugCnt = 0;
        PTCB pTCB = pPhy->pBsRx->pTaskRx1[0];
        U32 *pContext = (U32*) pTCB->ContextPtr;

        mlogDebug[mlogDebugCnt++] = 0xABABABAB;
        mlogDebug[mlogDebugCnt++] = pContext[3];
        mlogDebug[mlogDebugCnt++] = pContext[4];
        MLogAddVariables (mlogDebugCnt, mlogDebug, GET_TICKS);
    }
#endif

    ReinitTaskList(pPhy->pBsRx->pExeTaskList[0], NULL);

#ifdef _WIN32
    //Need to Reset this TCB so that Ceva Sim works for Multi-subframes
    ReinitTCB(pPhy->pBsRx->pMuxCtrlTcbCtx1, NULL);
#endif
    pPhy->pBsRx->isExeTaskListReady = FALSE;

    MLogTask(PID_RXLIST_POSTPROCESS ,RESOURCE_LARM, t, GET_TICKS);

    return 0;
}

void setupFpPointers(PLTEPHYDESC pPhy)
{
    return;
}



//-------------------------------------------------------------------------------------------
/** @brief Pre-Execute BaseStation PRACH Rx TaskTask
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range                 |Units|
 *  |              |Standard |              |         |            |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |pPhy          |         |pInput/pOutput|1        |pStructure  |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |PhyBsRxPrachPreExec                   |
 *  --------------------------------------------------------
 *  |Input Stream        |none                             |
 *  --------------------------------------------------------
 *  |Output Stream       |none                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  |Status Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Use MDMA to copy incoming IQ samples for current subframe from
 *  DDR to CRAM.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsRxPrachPreExec(PLTEPHYDESC pPhy)
{

#ifdef _WIN32
    if ((f_prach_index_profile_fp = fopen("prachIndexProfile.dat", "wt")) == NULL)
    {
        printf("Can't open file: %s\n", "prachIndexProfile.dat");
        return -1;
    }
#endif

    return 0;
}


//-------------------------------------------------------------------------------------------
/** @brief Execute Call Back function BaseStation PRACH Rx TaskTask
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  0 if SUCCESS
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range |Units|
 *  |              |Standard |              |         |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  |pTaskList   |         |pInput        |1        |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  |pPhy          |         |pInput/pOutput|1        |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |PhyBsRxExeTaskList               |
 *  --------------------------------------------------------
 *  |Input Stream        |none                             |
 *  --------------------------------------------------------
 *  |Output Stream       |none                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  |Status Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Top level function that is called from Scheduler after PRACH list is run to completion
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsRxPrachExeCB(PTASKLIST pTaskList, PLTEPHYDESC pPhy, U32 Num)
{
    pPhy->pBsRx->isExePrachTaskListReady[Num] = FALSE;
    return 0;
}

//-------------------------------------------------------------------------------------------
/** @brief Execute BaseStation PRACH Rx TaskTask
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range |Units|
 *  |              |Standard |              |         |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  |pPhy          |         |pInput/pOutput|1        |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |PhyBsRxPrachExeTaskList               |
 *  --------------------------------------------------------
 *  |Input Stream        |none                             |
 *  --------------------------------------------------------
 *  |Output Stream       |none                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  |Status Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Top level function to execute all BaseStation Uplink Rx task lists
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsRxPrachExeTaskList(PLTEPHYDESC pPhy, U32 Num)
{
    S16 subframeNum;

    subframeNum = pPhy->pBsRx->subframeNumber;
    if (pPhy->pBs->pBsRxPrach[Num]->bs_prach_preambleSubframeEnable[subframeNum] == 1)
    {
        if (pPhy->pBsRx->isExePrachTaskListReady[Num] != TRUE)
        {
            phy_printf("PRACH List Not Ready!!\n");
            return 0;
        }
#ifndef _WIN32
        RegListDoneCB(pPhy->pBsRx->pExePrachTaskList[Num], (fListDoneCB*)PhyBsRxPrachExeCB, pPhy);
#endif
        ExeTSchedList(pPhy->pBsRx->pExePrachTaskList[Num]);
    }

    return 0;
}

//-------------------------------------------------------------------------------------------
/** @brief Execute BaseStation Multiplex control and data Rx TaskTask
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range |Units|
 *  |              |Standard |              |         |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  |pPhy          |         |pInput/pOutput|1        |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |PhyBsRxMuxControlExeTaskList     |
 *  --------------------------------------------------------
 *  |Input Stream        |none                             |
 *  --------------------------------------------------------
 *  |Output Stream       |none                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  |Status Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Top level function to execute all BaseStation Uplink Rx task lists
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsRxMuxControlExeTaskList(PLTEPHYDESC pPhy)
{
    PULRXFECPARAMS pFecParams;

    pFecParams       = pPhy->pBsRx->pUlRxFecParams[pPhy->pBsRx->RxFecParam_Context];

    if (pFecParams->muxPUSCHCqiRi)
    {
        ExeTSchedSingle(pPhy->pBsRx->pMuxCtrlTcbCtx1);
    }

    return 0;
}


//-------------------------------------------------------------------------------------------
/** @brief Post-Execute BaseStation PRACH Rx TaskTask
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range                 |Units|
 *  |              |Standard |              |         |            |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |pPhy          |         |pInput/pOutput|1        |pStructure  |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |PhyBsRxPrachPostExec                  |
 *  --------------------------------------------------------
 *  |Input Stream        |none                             |
 *  --------------------------------------------------------
 *  |Output Stream       |none                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  |Status Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Freeing of dynamic memory after Base Station Uplink Rx task list is
 *  completed.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsRxPrachPostExec(PLTEPHYDESC pPhy, U32 Num)
{

#ifdef _WIN32
    PBS pBs = pPhy->pBs;
    PMS_DET pDetEvent;

    S32 i, cs;
    S16 ns;

    pDetEvent =  &pBs->pBsRxPrachDet[Num]->bs_prachDet_msDet;
    if ((pDetEvent->num > 0))
    {

        fprintf(f_prach_index_profile_fp,"%3d\n", pDetEvent->num);
        for (i = 0; i < pDetEvent->num; i++)
        {
            ns = pDetEvent->tDelMicrosecEst[i];
            cs = pDetEvent->cyclicShift[i] ;

            fprintf(f_prach_index_profile_fp,"%3d\n", cs);
            fprintf(f_prach_index_profile_fp,"%5d\n", ns);

        }
    }

    fclose(f_prach_index_profile_fp);
#endif

    ReinitTaskList(pPhy->pBsRx->pExePrachTaskList[Num], NULL);

    pPhy->pBsRx->isExePrachTaskListReady[Num] = FALSE;

    return 0;
}

U32 UpdateUlRxSrsTaskList(PLTEPHYDESC pPhy)
{
    // PHY Structures
    PBS pBs;
    PBS_RX pBsRx;
    PBS_SRS pBsSrs;
    PBS_RX_POINTERS pRxPtr = pPhy->pBsRx->pBsRxPointers;
    PBS_RX_CHAN_SRS pBsRxChanSrs;
    PULRXINPUTPARAMS pUlRxInputParams;
    S16 numSrs, nc, MO, K, L, Nul_symb;
    U32 step_aDet;

    S16 *m_srs_ptr, *N_ptr;

    S16 *pTable_DFT[5];


    PTCB *pTcbList3 = pPhy->pBsRx->pTaskRx3;
    U16 NumTasksTotalRX3 = 0;

    ////////////////////////////////////////////////////////////////////////////
    // Initialize
    ////////////////////////////////////////////////////////////////////////////
    pBs = pPhy->pBs;
    pBsRx = pPhy->pBsRx;
    pBsSrs = pBs->pBsSrs;

    pUlRxInputParams = pBsRx->pUlRxInputParams;
    if (pBsSrs->srsSubframeEnable[pBsRx->subframeNumber] == 1)
    {
        numSrs = pBsRx->numSrsChan;
        MO = pBsRx->Nrx_antennas;

        Nul_symb = pBsRx->Nul_symb;
        K = pBsRx->Nul_rb * pBs->Nrb_sc; //total number of frequency indices in resource grid
        L = 2 * Nul_symb; //total number of time indices in resource grid subframe

        step_aDet = 2 * RUP8B(K + 2);

        pBsRxChanSrs = pBsRx->pBsRxChanSrs;
        m_srs_ptr = &CA_srsBandwidthConfigTable_cram[64 * pBsSrs->srsBandwidthTab + 8
            * pBsSrs->srsBandwidthConfig];
        N_ptr = &CA_srsBandwidthConfigTable_cram[64 * pBsSrs->srsBandwidthTab + 8
            * pBsSrs->srsBandwidthConfig + 1];
            for (nc = 0; nc < numSrs; nc++)
            {
                // Extract local variables from the input ul_srs and ulChan_srs data structures
                //bandwidth[nc] = pBsRxChanSrs->bandwidth[nc]; //={0,1,2,3}, MS specific parameter, srs-Bandwidth

                //index_tab1 {0, 1, 2, 3} points to the part of table depending on Nul_Rb
                //index_tab2 {0, 1, 2, 3, 4, 5, 6, 7} points to row depending on Csrs
                if (1)
                {
                    pRxPtr->nCsSrs[nc] = 0; //for CDM, locally generate pilots with cyclic shift = 0
                }
                else
                {
                    pRxPtr->nCsSrs[nc] = pBsRxChanSrs->cyclicShift[nc]; //when not doing CDM SRS, locally generate pilots
                    //with user dependent cyclic shift index
                }
                pRxPtr->pLengthSrsPilots[nc] = m_srs_ptr[2 * pBsRxChanSrs->bandwidth[nc]] * pBs->Nrb_sc / 2;
            } //ichan

            bs_rx_Pilot_srs_tcb(NULL, pBsRx->subframeNumber, pBs->Nrb_sc, pBs->Ncell_id,
                            pUlRxInputParams->groupHoppingEnabled, pUlRxInputParams->sequenceHoppingEnabled, pUlRxInputParams->delta_ss,
                            pBsSrs->u, pBsRxChanSrs->v, pRxPtr->pSrsPilots, m_srs_ptr, N_ptr, pRxPtr->nCsSrs,
                            (S16 *)pBsRxChanSrs->bandwidth, numSrs, pBs->Nused, UlPilots_QthRootTable, NULL, NULL,
                            0, TCBIOBUFREFILL, pTcbList3[NumTasksTotalRX3]);
            NumTasksTotalRX3++;

            // initialize the table for DFT/IDFT in SRS
            switch ( *(pRxPtr->pLengthSrsPilots))
            {
                case   24:
                           pTable_DFT[0] = IDFT_3ptSqrN_0384;
                           pTable_DFT[1] = IDFT_Twdd4C_0768_q1;
                           pTable_DFT[2] = IDFT_Twdd4C_0768_q2;
                           pTable_DFT[3] = IDFT_Twdd4C_0768_q3;
                           pTable_DFT[4] = IDFT_ShffIdx_0024;
                           break;
                case   48:
                           pTable_DFT[0] = IDFT_3ptSqrN_0768;
                           pTable_DFT[1] = IDFT_Twdd4C_0768_q1;
                           pTable_DFT[2] = IDFT_Twdd4C_0768_q2;
                           pTable_DFT[3] = IDFT_Twdd4C_0768_q3;
                           pTable_DFT[4] = IDFT_ShffIdx_0048;
                           break;
                case   72:
                           pTable_DFT[0] = IDFT_3ptSqrN_1152;
                           pTable_DFT[1] = IDFT_Twdd4C_1152_q1;
                           pTable_DFT[2] = IDFT_Twdd4C_1152_q2;
                           pTable_DFT[3] = IDFT_Twdd4C_1152_q3;
                           pTable_DFT[4] = IDFT_ShffIdx_0072;
                           break;
                case   96:
                           pTable_DFT[0] = IDFT_3ptSqrN_0384;
                           pTable_DFT[1] = IDFT_Twdd4C_0768_q1;
                           pTable_DFT[2] = IDFT_Twdd4C_0768_q2;
                           pTable_DFT[3] = IDFT_Twdd4C_0768_q3;
                           pTable_DFT[4] = IDFT_ShffIdx_0096;
                           break;
                case  120:
                           pTable_DFT[0] = IDFT_3ptSqrN_0480;
                           pTable_DFT[1] = IDFT_Twdd4C_0960_q1;
                           pTable_DFT[2] = IDFT_Twdd4C_0960_q2;
                           pTable_DFT[3] = IDFT_Twdd4C_0960_q3;
                           pTable_DFT[4] = IDFT_ShffIdx_0120;
                           break;
                case  144:
                           pTable_DFT[0] = IDFT_3ptSqrN_0576;
                           pTable_DFT[1] = IDFT_Twdd4C_1152_q1;
                           pTable_DFT[2] = IDFT_Twdd4C_1152_q2;
                           pTable_DFT[3] = IDFT_Twdd4C_1152_q3;
                           pTable_DFT[4] = IDFT_ShffIdx_0144;
                           break;
                case  192:
                           pTable_DFT[0] = IDFT_3ptSqrN_0768;
                           pTable_DFT[1] = IDFT_Twdd4C_0768_q1;
                           pTable_DFT[2] = IDFT_Twdd4C_0768_q2;
                           pTable_DFT[3] = IDFT_Twdd4C_0768_q3;
                           pTable_DFT[4] = IDFT_ShffIdx_0192;
                           break;
                case  216:
                           pTable_DFT[0] = IDFT_3ptSqrN_0864;
                           pTable_DFT[1] = IDFT_Twdd4C_0864_q1;
                           pTable_DFT[2] = IDFT_Twdd4C_0864_q2;
                           pTable_DFT[3] = IDFT_Twdd4C_0864_q3;
                           pTable_DFT[4] = IDFT_ShffIdx_0216;
                           break;
                case  240:
                           pTable_DFT[0] = IDFT_3ptSqrN_0960;
                           pTable_DFT[1] = IDFT_Twdd4C_0960_q1;
                           pTable_DFT[2] = IDFT_Twdd4C_0960_q2;
                           pTable_DFT[3] = IDFT_Twdd4C_0960_q3;
                           pTable_DFT[4] = IDFT_ShffIdx_0240;
                           break;
                case  288:
                           pTable_DFT[0] = IDFT_3ptSqrN_1152;
                           pTable_DFT[1] = IDFT_Twdd4C_1152_q1;
                           pTable_DFT[2] = IDFT_Twdd4C_1152_q2;
                           pTable_DFT[3] = IDFT_Twdd4C_1152_q3;
                           pTable_DFT[4] = IDFT_ShffIdx_0288;
                           break;
            }




            // Do SRS Receiver Processing
                SRSReceiverProcessing_tcb(NULL, pBs->df, pBs->Fs, pBs->Nrb_sc,
                                    pBs->Nused, pBs->Nfft, pBsSrs->srsBandwidthTab,
                                    pBsSrs->srsBandwidthConfig, CA_srsBandwidthConfigTable_cram,
                                    &pBsRx->aDet[0], numSrs,
                                    0, MO, K, L, pBsRx->Nul_rb,
                                    pBsSrs->srsPilotSymbol, pRxPtr,
                                    pBsRxChanSrs, step_aDet, ScaleComputeErr_Tab,
                                    pRxPtr->pSrsSnrPerRb, NULL, NULL, 0, NULL,
                                    TCBIOBUFREFILL, pTcbList3[NumTasksTotalRX3],pTable_DFT,pPhy->rxFrameNumber,pBsRx->subframeNumber);
                NumTasksTotalRX3++;

            SRSSnrReporting_tcb(NULL, MO, numSrs, pBsRx->Nul_rb,
                               (S16 *)pBsRxChanSrs->StartRb, (S16 *)pBsRxChanSrs->EndRb,
                               pRxPtr->pSrsSnrPerRb, pRxPtr->pSrsAvgSnrPerRbdB,
                               NULL, TCBIOBUFREFILL, pTcbList3[NumTasksTotalRX3]);
    } //bsRx.srs.subframeEnable(bsRx.subframeNumber+CE_MI1)

    return 0;
}

//-------------------------------------------------------------------------------------------
/** @brief Pre-Execute BaseStation SRS Rx TaskTask
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range                 |Units|
 *  |              |Standard |              |         |            |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |pPhy          |         |pInput/pOutput|1        |pStructure  |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |PhyBsRxSrsPreExec                   |
 *  --------------------------------------------------------
 *  |Input Stream        |none                             |
 *  --------------------------------------------------------
 *  |Output Stream       |none                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  |Status Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Use MDMA to copy incoming IQ samples for current subframe from
 *  DDR to CRAM.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsRxSrsPreExec(PLTEPHYDESC pPhy)
{

#ifdef _WIN32
    if ((f_srs_chan_est_fp = fopen("srsChanEst.dat", "wt")) == NULL)
    {
        printf("Can't open file: %s\n", "srsChanEst.dat");
        return -1;
    }
#endif

    return 0;
}


//-------------------------------------------------------------------------------------------
/** @brief Execute Call Back function BaseStation SRS Rx TaskTask
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  0 if SUCCESS
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range |Units|
 *  |              |Standard |              |         |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  |pTaskList   |         |pInput        |1        |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  |pPhy          |         |pInput/pOutput|1        |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |PhyBsRxExeTaskList               |
 *  --------------------------------------------------------
 *  |Input Stream        |none                             |
 *  --------------------------------------------------------
 *  |Output Stream       |none                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  |Status Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Top level function that is called from Scheduler after SRS list is run to completion
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsRxSrsExeCB(PTASKLIST pTaskList, PLTEPHYDESC pPhy)
{
    pPhy->pBsRx->isExeSrsTaskListReady = FALSE;
    return 0;
}

//-------------------------------------------------------------------------------------------
/** @brief Execute BaseStation SRS Rx TaskTask
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range |Units|
 *  |              |Standard |              |         |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  |pPhy          |         |pInput/pOutput|1        |            |            |            |     |
 *  ------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |PhyBsRxSRSExeTaskList               |
 *  --------------------------------------------------------
 *  |Input Stream        |none                             |
 *  --------------------------------------------------------
 *  |Output Stream       |none                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  |Status Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Top level function to execute all BaseStation Uplink Rx task lists
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsRxSRSExeTaskList(PLTEPHYDESC pPhy)
{
    if ((pPhy->pBs->pBsSrs->srsSubframeEnable[pPhy->pBsRx->subframeNumber] == 1) &&
        (pPhy->pBsRx->numSrsChan > 0))
    {
        if (pPhy->pBsRx->isExeSrsTaskListReady != TRUE)
        {
            phy_printf("SRS List Not Ready!!\n");
            return 0;
        }
#ifndef _WIN32
        RegListDoneCB(pPhy->pBsRx->pExeSrsTaskList[0], (fListDoneCB*)PhyBsRxSrsExeCB, pPhy);
#endif
        ExeTSchedList(pPhy->pBsRx->pExeSrsTaskList[0]);
    }

    return 0;
}

//-------------------------------------------------------------------------------------------
/** @brief Post-Execute BaseStation SRS Rx TaskTask
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range                 |Units|
 *  |              |Standard |              |         |            |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |pPhy          |         |pInput/pOutput|1        |pStructure  |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |PhyBsRxSRSPostExec                  |
 *  --------------------------------------------------------
 *  |Input Stream        |none                             |
 *  --------------------------------------------------------
 *  |Output Stream       |none                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  |Status Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Freeing of dynamic memory after Base Station Uplink Rx task list is
 *  completed.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsRxSrsPostExec(PLTEPHYDESC pPhy)
{
#ifdef _WIN32

    // PHY Structures
    PBS pBs;
    PBS_RX pBsRx;
    PBS_SRS pBsSrs;
    PBS_RX_POINTERS pRxPtr = pPhy->pBsRx->pBsRxPointers;

    S16 *srsChEst;
    S16 numSrs;
    S16 i, mo, MO, nc, K, k, a = 0;

    pBs = pPhy->pBs;
    pBsRx = pPhy->pBsRx;
    pBsSrs = pBs->pBsSrs;

    if ((pBsSrs->srsSubframeEnable[pBsRx->subframeNumber] == 1) && (pBsRx->numSrsChan > 0))
    {
        MO = pBsRx->Nrx_antennas;
        K = pBsRx->Nul_rb * pBs->Nrb_sc; //total number of frequency indices in resource grid
        numSrs = pBsRx->numSrsChan;

        srsChEst = (S16 *) pRxPtr->pSrsChEst;
        for (mo = 0; mo < MO; mo++)
        {
            for (nc = 0, k = 0; nc < numSrs; nc++)
            {

                //phy_printf("SrsPilotsLength: %d\n", pRxPtr->pLengthSrsPilots[nc]);
                for (i = 0; i < pRxPtr->pLengthSrsPilots[nc]; i++)
                {

                    fprintf(f_srs_chan_est_fp,"%hd %hd\n", srsChEst[(mo*2*K*numSrs)+(2*i)+k],srsChEst[(mo*2*K*numSrs)+(2*i+1)+k]);
                    a+=2;
                }
                k += pRxPtr->pLengthSrsPilots[nc];
            }
        } //mo, antenna
    }

    fclose(f_srs_chan_est_fp);
#endif

    ReinitTaskList(pPhy->pBsRx->pExeSrsTaskList[0], NULL);

    pPhy->pBsRx->isExeSrsTaskListReady = FALSE;

    return 0;
}




//-------------------------------------------------------------------------------------------
/** @brief UpdateRxChanVars, Update Channel Specific parameters
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range                 |Units|
 *  |              |Standard |              |         |            |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |pPhy          |         |pInput/pOutput|1        |pStructure  |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |UpdateRxChanVars                 |
 *  --------------------------------------------------------
 *  |Input Stream        |none                             |
 *  --------------------------------------------------------
 *  |Output Stream       |none                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  |Status Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Update UE parameters saved from previous subframes
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 UpdateRxChanVars(PLTEPHYDESC pPhy)
{
    U32 nc, chanId;
    PULRXINPUTPARAMS pUlRxInputParams;

    pUlRxInputParams = pPhy->pBsRx->pUlRxInputParams;

    for (nc = 0; nc < (U32)pUlRxInputParams->numChan; nc++)
    {
        chanId = pUlRxInputParams->ChanId[nc];

        if (pUlRxInputParams->physChanType[nc] == PUSCH)
        {
            if (pUlRxInputParams->usedIndexVars[chanId]->mSetDefaults == 0)
            {
                pUlRxInputParams->mErrAvg[2*chanId]   = pUlRxInputParams->usedIndexVars[chanId]->mErrAvgCh[0];
                pUlRxInputParams->mErrAvg[2*chanId+1] = pUlRxInputParams->usedIndexVars[chanId]->mErrAvgCh[1];
                pUlRxInputParams->mErrExpo[chanId]    = pUlRxInputParams->usedIndexVars[chanId]->mErrExpo;
            }
        }
    }

    return 0;
}

//-------------------------------------------------------------------------------------------
/** @brief Top level function to call Uplink Rx FEC Functions
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range                 |Units|
 *  |              |Standard |              |         |            |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |pPhy          |         |pInput/pOutput|1        |pStructure  |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |LteBsRxSduProcessing             |
 *  --------------------------------------------------------
 *  |Input Stream        |none                             |
 *  --------------------------------------------------------
 *  |Output Stream       |none                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  |Status Parameters   |pPhy                             |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Top level function which will call Descrambler and FEC Decoder for
 *  Base Station Uplink Receiver.
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_fec_decoder
 *
 **/
//-------------------------------------------------------------------------------------------
U32 LteBsRxSduProcessing(PLTEPHYDESC pPhy)
{
    U32 status = 0;
    PBS_RX pBsRx;
    PULRXINPUTPARAMS pUlRxInputParams;
    PULRXFECPARAMS   pFecParams;
    S16 stat;
    U16 numChan, nc;
    U32 dataBits;
    S32 Cmux = 12;

    pBsRx = pPhy->pBsRx;

    pUlRxInputParams = pBsRx->pUlRxInputParams;
    pFecParams       = pBsRx->pUlRxFecParams[pBsRx->RxFecParam_Context];

    numChan = pFecParams->numChan;

    for (nc = 0; nc < numChan; nc++)
    {
        if (pUlRxInputParams->physChanType[nc] == PUSCH)
        {
            //Update TimingErr Average here
            pFecParams->mac_sdu[nc] = MemAlloc(pFecParams->numBytes[nc] + 32 + CRC24A, CRAM_MEM, 0);
            if (pFecParams->numBytes[nc] > 0)
            {
                // Compute new data bit rate matching G parameter.
                dataBits = pUlRxInputParams->G[nc] - pFecParams->numbitsCQI[nc] - pFecParams->numbitsRI[nc];

                //Run decoder for transport block data
                bs_rx_channel_coding(dataBits, pFecParams->numBytes[nc], NULL, pFecParams->mac_sdu[nc],
                        &stat, pPhy->pharq, 1, dataBits, pFecParams->Qm[nc], pFecParams->rvidx[nc], 0,
                        pFecParams->HalfIterations, pFecParams->numbitsCQI[nc], pFecParams->numbitsRI[nc], Cmux,
                        0, pFecParams->Itbs[nc], pFecParams->Nprbm1[nc], pFecParams->harq_user[nc]);
            }
            pBsRx->pUlRxSduParams->stat[nc] = (U8)stat;
        }
        else //PUCCH
        {
            if (pBsRx->pUlRxSduParams->stat[nc] != STAT_PUCCH_DEADZONE)
            {
                pFecParams->pucch_mac_sdu[nc][0] = pBsRx->pUlRxSduParams->pucch_mac_sdu[nc][0];
                pFecParams->pucch_mac_sdu[nc][1] = pBsRx->pUlRxSduParams->pucch_mac_sdu[nc][1];
            }
            else
            {
                //if deadzone, clear srdetected flag
                pUlRxInputParams->srdetected[nc] = 0;
            }
        } // else PUCCH
    }
    return status;
}

//-------------------------------------------------------------------------------------------
/** @brief rnti_assign_new_id, Assign new RNTI ID
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   RNTI - New RNTI to be configured
 *  @param   pRntiMapping - Add RNTI to structure to track
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------------
 *  |Name          |Name in  |Direction     |Length   |Type        |Data Format |Valid Range                 |Units|
 *  |              |Standard |              |         |            |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |RNTI          |         |Input         |1        |Real        |(0:32:0)    |Full Range                  |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  |pRntiMapping  |         |pOutput       |1        |pStructure  |            |                            |     |
 *  ----------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------------------------
 *  |Block Name          |rnti_assign_new_id               |
 *  --------------------------------------------------------
 *  |Input Stream        |none                             |
 *  --------------------------------------------------------
 *  |Output Stream       |none                             |
 *  --------------------------------------------------------
 *  |Config Parameters   |RNTI                             |
 *  --------------------------------------------------------
 *  |Status Parameters   |pRntiMapping                     |
 *  --------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Add new RNTI to parameter structure to track
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  none
 *
 *
 *  \ingroup group_lte_rx_uplink_demodulator
 *
 **/
//-------------------------------------------------------------------------------------------
S8 rnti_assign_new_id(U32 RNTI, PBS_RX_RNTI_MAPPING pRntiMapping)
{
    U32 i;
    PUSED_INDEX_VARS pUsedIndexVars;
    for (i = 0; i < NUM_CONNECTED_USERS; i++)
    {
        pUsedIndexVars = &pRntiMapping->used_index[i];
        if (pUsedIndexVars->index == RNTI_VARS_NOT_USED)
        {
            pUsedIndexVars->index = RNTI_VARS_USED;
            pUsedIndexVars->last_frame_used = 0;
            pUsedIndexVars->RNTI = RNTI;

            //init active channel variables when assign to new active channel id
            pUsedIndexVars->mErrAvgCh[0] = 1;
            pUsedIndexVars->mErrAvgCh[1] = 0;
            pUsedIndexVars->mErrExpo = -32768;
            pUsedIndexVars->NumRbAvg = 0;

            pUsedIndexVars->timeAveragedFreqErr[0] = 0;  //Re part
            pUsedIndexVars->timeAveragedFreqErr[1] = 0;  //Im part

            // Averaging of SINRs
            pUsedIndexVars->avgPucchSinr = 0;
            pUsedIndexVars->pucchSinrCnt = 1;
            pUsedIndexVars->pucchSinrPrevTti = SINR_INIT_VALUE;
            pUsedIndexVars->pucchSinrPrevSinr = 0;
            pUsedIndexVars->avgPuschSinr = 0;
            pUsedIndexVars->puschSinrCnt = 1;
            pUsedIndexVars->puschSinrPrevTti = SINR_INIT_VALUE;
            pUsedIndexVars->puschSinrPrevSinr = 0;

            // Averaging of PUSCH TA
            pUsedIndexVars->puschTaCnt = 1;
            pUsedIndexVars->avgPuschTa = 31;

            if (pRntiMapping->max_used_index < i+1)
                pRntiMapping->max_used_index = i+1;

            return (S8)i;
        }
    }

    stop_printf("RNTI MAPPING ERROR: All IDs Used\n");
    return -1;
}


//-------------------------------------------------------------------------------------------
/** @brief LTE USER ID Mapping Function<BR>
 *  Function bs_rx_rnti_get_id implements the logic to map RNTI (from MAC) to the Phy Index to use within internal structures
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param      RNTI = RNTI Value from MAP
 *  @param      pRntiMapping = Pointer to PHY internal structure
 *  @param      currFrameNum = Current FrameNumber
 *  @return       PHY ID
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |RNTI          |               |input        |U32            |Real   |       |[0:65535]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |pRntiMapping       |               |output       |struct ptr    |struct |      |   |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |currFrameNum   |               |input        |S32            |Real   |       |[0:1023]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------
 *  |Block Name          |bs_rx_rnti_get_id  |
 *  ------------------------------------------
 *  |Input Stream        |RNTI, pRntiMapping, currFrameNum        |
 *  ------------------------------------------
 *  |Output Stream       |PhyId                |
 *  ------------------------------------------
 *  |Config Parameters   |pRntiMapping          |
 *  ------------------------------------------
 *  |Status Parameters   |pRntiMapping               |
 *  ------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Function bs_rx_rnti_get_id implements the logic to map RNTI (from MAC) to the Phy Index to use within internal structures
 *
 *
 *  <5> Cycle Count Formula:
 *  TBC
 *
 *
 *  <6> References:
 *
 *
 *  \ingroup group_lte_rx_uplink_fec_decoder
 *
 **/
//-------------------------------------------------------------------------------------------
U32 bs_rx_rnti_get_id(U32 RNTI, PBS_RX_RNTI_MAPPING pRntiMapping, S32 currFrameNum)
{
    if (pRntiMapping->rnti_map[RNTI] == RNTI_MAP_NOT_USED)
        pRntiMapping->rnti_map[RNTI] = rnti_assign_new_id(RNTI, pRntiMapping);

    pRntiMapping->used_index[pRntiMapping->rnti_map[RNTI]].last_frame_used = currFrameNum;
    pRntiMapping->used_index[pRntiMapping->rnti_map[RNTI]].flushed = 0;

    return pRntiMapping->rnti_map[RNTI];
}



//-------------------------------------------------------------------------------------------
/** @brief LTE USER ID Flushing Function<BR>
 *  Function bs_rx_rnti_flush_id implements the logic to flush out un-used PHY ids. Function is called once in a radio frame.
 *  If RNTI is not used for NUM_FRAMES_BEFORE_CLEAR_RNTI_INDEX, then PHY ID is flushed from the used pool and can be
 *  used for another RNTI
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param      currFrameNum = current Frame Number when this was called
 *  @param      pRntiMapping = Pointer to PHY internal structure
 *  @return       none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |Name           |Name In        |Direction    |Length         |Type          |Format    |Valid Range    |Units         |
 *  |               |Standard       |             |               |              |          |               |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |currFrameNum   |               |input        |S32            |Real   |       |[0:1023]     |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  |pRntiMapping       |               |output       |struct ptr    |struct |      |      |              |
 *  ------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------
 *  |Block Name          |bs_rx_rnti_flush_id|
 *  ------------------------------------------
 *  |Input Stream        |currFrameNum, pRntiMapping                 |
 *  ------------------------------------------
 *  |Output Stream       |none                |
 *  ------------------------------------------
 *  |Config Parameters   |pRntiMapping          |
 *  ------------------------------------------
 *  |Status Parameters   |pRntiMapping               |
 *  ------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Function bs_rx_rnti_flush_id implements the logic to flush out un-used user ids. Check is done once in a radio frame
 *  If ID is not used for NUM_FRAMES_BEFORE_CLEAR_RNTI_INDEX, then it is flushed from the used pool and can be
 *  used for another RNTI
 *
 *
 *  <5> Cycle Count Formula:
 *  TBC
 *
 *
 *  <6> References:
 *
 *
 *  \ingroup group_lte_rx_uplink_fec_decoder
 *
 **/
//-------------------------------------------------------------------------------------------
void bs_rx_rnti_flush_id(S32 currFrameNum, PBS_RX_RNTI_MAPPING pRntiMapping)
{
    U32 i, flush, RNTI, currMaxUsed = 0;
    S32 prevFrameNum;
    PUSED_INDEX_VARS pUsedIndexVars;

    for (i = 0; i < pRntiMapping->max_used_index; i++)
    {
        pUsedIndexVars = &pRntiMapping->used_index[i];
        // If ID has been used
        if (pUsedIndexVars->index == RNTI_VARS_USED && !pUsedIndexVars->flushed)
        {
            flush= 0;
            // Frame Number in which ID was Previously used
            prevFrameNum = pUsedIndexVars->last_frame_used;

            // Check the threshold
            if (prevFrameNum > currFrameNum)
            {
                if ((currFrameNum+1024 - prevFrameNum) >= NUM_FRAMES_BEFORE_CLEAR_RNTI_INDEX)
                    flush = 1;
            }
            else if ((currFrameNum - prevFrameNum) > NUM_FRAMES_BEFORE_CLEAR_RNTI_INDEX)
                flush = 1;

            // No activity. Free this Index
            if (flush)
            {
                RNTI = pRntiMapping->used_index[i].RNTI;
                pRntiMapping->rnti_map[RNTI] = RNTI_MAP_NOT_USED;

                pUsedIndexVars->index = RNTI_VARS_NOT_USED;
                pUsedIndexVars->last_frame_used = 0;
                pUsedIndexVars->RNTI = 0;

                pUsedIndexVars->flushed = 1;

                pUsedIndexVars->mErrAvgCh[0] = 1;
                pUsedIndexVars->mErrAvgCh[1] = 0;
                pUsedIndexVars->mErrExpo = -32768;
                pUsedIndexVars->NumRbAvg = 0;

                pUsedIndexVars->timeAveragedFreqErr[0] = 0;  //Re part
                pUsedIndexVars->timeAveragedFreqErr[1] = 0;  //Im part

                // Averaging of SINRs
                pUsedIndexVars->avgPucchSinr = 0;
                pUsedIndexVars->pucchSinrCnt = 0;
                pUsedIndexVars->avgPuschSinr = 0;
                pUsedIndexVars->puschSinrCnt = 0;

                // Averaging of PUSCH TA
                pUsedIndexVars->puschTaCnt = 1;
                pUsedIndexVars->avgPuschTa = 31;

            }
            else
            {
                // Keep track of the MAX ID that is being used right now. Optimization for search :)
                if (currMaxUsed < i+1)
                    currMaxUsed = i+1;
            }
        }
    }

    // Optimization for search :)
    pRntiMapping->max_used_index = currMaxUsed;

    return;
}
//------------------------------------------------------------------------------------------

