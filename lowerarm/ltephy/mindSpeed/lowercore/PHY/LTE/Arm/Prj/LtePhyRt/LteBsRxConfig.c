//-------------------------------------------------------------------------------------------
/** @file LteBsRxConfigThread.c
 *
 * @brief Configuration of the PHY Rx Structures from RxVector (received from MAC)
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.45 $
 *
 * COPYRIGHT&copy; 2009-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#include "ltephy.h"
#include "ltephydef.h"
#include "Debug.h"
#include "prototypes.h"
#include "supervisor.h"
#include "heaps.h"
#include "apimgr.h"
#include "larmmsgid.h"
#include "LteBsRxConfig.h"
#include "LteBsRxThread.h"
#include "ltertconfig.h"
#include "ltemac.h"
#include "ext_phy.h"
#include "lte_entry.h"
#include "fpdrv.h"
#include "globalstoragearrays.h"
#include "mlog.h"

U32 UpdateUlRxTaskListRT(PLTEPHYDESC pPhy)
{
    // PHY Structures
    BS *pBs;
    BS_RX *pBsRx;
    BS_SRS *pBsSrs;
    PBS_RX_POINTERS pRxPtr = pPhy->pBsRx->pBsRxPointers;
    ULRXINPUTPARAMS *pUlRxInputParams;
    PULRXFECPARAMS   pFecParams;

    // General Variables
    S32 i, j, k, mo, isym;
    S32 length_adc;

    S32 MO;
    S32 numPusch, numPucch;
    S16 srsPilot;
    S16 numPucchRun[NUM_USER_PUCCH_TCBS];
    S32 IndPucch1, IndPucch2, numMultiChanTCBs;

    // Task List Related
    PTCB pTcbListTmp = NULL;
    PTCB *pTcbList1 = pPhy->pBsRx->pTaskRx1;
    PTCB *pTcbList2 = pPhy->pBsRx->pTaskRx2;
    PTCB *pTcbList4 = pPhy->pBsRx->pTaskRx4;
    PTCB *pTasksRxBypass = pPhy->pBsRx->pTasksRxBypass;
    U16 NumTasksTotalRX = 0;
    U32 numTasksBypass = 0;

    PTCB pTCB;
    U32* pContext;
    volatile U32 t1, t2;
    S16 *zczLUT;

    ////////////////////////////////////////////////////////////////////////////
    // Initialize
    ////////////////////////////////////////////////////////////////////////////
    pBs = pPhy->pBs;
    pBsRx = pPhy->pBsRx;
    pBsSrs = pBs->pBsSrs;
    pUlRxInputParams = pBsRx->pUlRxInputParams;
    pFecParams   = pBsRx->pUlRxFecParams[pBsRx->RxFecParam_Context];

    srsPilot = pBsSrs->srsPilotSymbol;

    length_adc = 2 * ((pBs->Ncp0 + pBs->Nfft) + 6 * (pBs->Ncp + pBs->Nfft));

    MO = pUlRxInputParams->MO;
    for (i = 0; i < MO; i++)
    {
        pRxPtr->ppSnrPerRb_Pilot[2 * i] = (S32 *) &(pRxPtr->pSnrPerRb[100 * (2 * i)]);
        pRxPtr->ppSnrPerRb_Pilot[2 * i + 1] = (S32 *) &(pRxPtr->pSnrPerRb[100 * (2 * i + 1)]);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Initializations END
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // UPDATE TASK LIST
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // PRACH LIST
    ////////////////////////////////////////////////////////////////////////////
    i = pBsRx->RxPrach_Context;

    if(i==0)
    {
        pTcbListTmp = pTcbList2[0];
    }
    else if(i==1)
    {
        pTcbListTmp = pTcbList4[0];
    }
    if (pPhy->pBs->pBsRxPrach[i]->bs_prach_preambleSubframeEnable[pBsRx->subframeNumber] == 1)
    {
        zczLUT = zeroCorrZone0;
        PRACH_Rx_Init(pPhy->pBs->pBsRxPrachCram[i], pPhy->pBs->pBsRxPrach[i],
            pPhy->pBs->pBsRxPrachDet[i], length_adc, MO);

        zczLUT = zeroCorrZone0;
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

        PRACH_Rx_tcb(pBsRx, pBs->pBsRxPrach[i], pBs->pBsRxPrachDet[i], MO,
                     pBsRx->adcbuf, length_adc, PRACH_Tables,
                     pBs->pBsRxPrachCram[i], NULL, TCBIOBUFREFILL, pTcbListTmp, zczLUT);

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

    numPucch = pUlRxInputParams->numPUCCH;

#ifdef DEBUG_POWER_RX_SIGNAL
    pTCB = pTcbList1[NumTasksTotalRX];
    pContext = (U32*) pTCB->ContextPtr;

    pContext[0] = (U32)pPhy->pBsRx->adcbuf[0];
    pContext[1] = (U32)pPhy->pBsRx->adcbuf[1];
    NumTasksTotalRX++;
#endif


    //////////////////////////////////////////////////////////////////////////////
    // PUCCH Pilots
    //ul_Pilot_PUCCH_tcb
    //////////////////////////////////////////////////////////////////////////////
    if (numPucch == 0)
    {
        pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX];
    }
    NumTasksTotalRX++;

    //////////////////////////////////////////////////////////////////////////////
    //Channel Estimation for PUCCH
    //bs_rx_ChEstimator_PUCCH_part0_tcb
    //////////////////////////////////////////////////////////////////////////////
    for(mo = 0; mo < MO; mo++)
    {
        if (numPucch == 0)
        {
            pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX++];
            pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX++];
        }
        else
        {
            //////////////////////////////////////////////////////////////////////////////
            //ChanEst PUCCH for slot 0
            //////////////////////////////////////////////////////////////////////////////
            pTCB = pTcbList1[NumTasksTotalRX++];


            //////////////////////////////////////////////////////////////////////////////
            //ChanEst PUCCH for slot 1
            //////////////////////////////////////////////////////////////////////////////
            pTCB = pTcbList1[NumTasksTotalRX++];

        }
    }

    //////////////////////////////////////////////////////////////////////////////////
    //Averaging of TimingErr, depends on ChanEst for PUCCH to finish before running
    //RunningAveragePUCCH_tcb
    //////////////////////////////////////////////////////////////////////////////////
    if (pUlRxInputParams->numPUCCH == 0)
    {
        pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX];
    }
    NumTasksTotalRX++;

    //////////////////////////////////////////////////////////////////////////////
    //Channel Estimation for PUCCH
    //bs_rx_ChEstimator_PUCCH_tcb
    //////////////////////////////////////////////////////////////////////////////
    for(mo = 0; mo < MO; mo++)
    {
        if (numPucch == 0)
        {
            pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX++];
            pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX++];
        }
        else
        {
            //////////////////////////////////////////////////////////////////////////////
            //ChanEst PUCCH for slot 0
            //////////////////////////////////////////////////////////////////////////////
            pTCB = pTcbList1[NumTasksTotalRX++];
            pContext = pTCB->ContextPtr;
            pContext[3] = (UINT32)-1;       //must be -1 for slot 0

            //////////////////////////////////////////////////////////////////////////////
            //ChanEst PUCCH for slot 1
            //////////////////////////////////////////////////////////////////////////////
            pTCB = pTcbList1[NumTasksTotalRX++];
            pContext = pTCB->ContextPtr;
            pContext[3] = srsPilot;
        }
    }
    //////////////////////////////////////////////////////////////////////////////
    //User base MRC, FEQ, CazacAvg and DeMapper for PUCCH,
    //rx_Multichan_DeModulation_Pucch_tcb
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
        pTCB = pTcbList1[NumTasksTotalRX];

        if (numPucchRun[i] == 0)
            pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX];
        else
        {
            pContext = pTCB->ContextPtr;
            pContext[0] = numPucchRun[i];
            pContext[2] = srsPilot;
        }
        NumTasksTotalRX++;
    }

    /////////////////////////////////////////////////////////////////////////////////////
    //PUSCH Uplink Pilots - generate pilots for all Users
    //ul_Pilot_fx_tcb
    /////////////////////////////////////////////////////////////////////////////////////
    numPusch = pUlRxInputParams->numPUSCH;

    if (numPusch == 0)
    {
        pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX];
    }
    NumTasksTotalRX++;

    ///////////////////////////////////////////////////////////////////////////////////////////
    //Channel estimation from pilots, for every subcarrier in the resource element matrix, for this user, for the complete slot
    //Compute Channel Estimate at pilots, computation of Error vector
    //bs_rx_ChEstimator_part0_fx_tcb
    ///////////////////////////////////////////////////////////////////////////////////////////
    for (mo = 0; mo < MO; mo++)
    {
        if (numPusch == 0)
        {
            pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX++];
            pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX++];
        }
        else
        {
            NumTasksTotalRX++;
            NumTasksTotalRX++;
            if (mo == MO - 1)
            {
                RexSetTCBControl(pTcbList1[NumTasksTotalRX], BLOCKING_TCB); //mark it as a blocking TCB
            }
        }
    } //for each antenna, for (mo = 0; mo < MO; mo++)

    //////////////////////////////////////////////////////////////////////////////////
    //Averaging of TimingErr, depends on ChanEst for PUSCH to finish before running
    //RunningAverage0_tcb
    //////////////////////////////////////////////////////////////////////////////////
    if (numPusch == 0)
    {
        pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX];
    }
    NumTasksTotalRX++;

    ///////////////////////////////////////////////////////////////////////////////////////////
    //computation of derotation vector
    //Derotation of Channel at pilots
    //do least squares straight line fit to these pilot channel estimates to do some noise averaging
    //bs_rx_ChEstimator_part1_fx_tcb
    ///////////////////////////////////////////////////////////////////////////////////////////
    for (mo = 0; mo < MO; mo++)
    {
        if (numPusch == 0)
        {
            pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX++];
            pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX++];
        }
        else
        {
            NumTasksTotalRX++;
            NumTasksTotalRX++;
        }
    } //for each antenna, for (mo = 0; mo < MO; mo++)

    //////////////////////////////////////////////////////////////////////////////////
    //Averaging of TimingErr and SNR Calc
    //RunningAverage_tcb, SnrSubframe_tcb
    //////////////////////////////////////////////////////////////////////////////////
    if (pUlRxInputParams->numChan == 0)
    {
        pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX++];
    }
    else
    {
        NumTasksTotalRX++;
    }


    //////////////////////////////////////////////////////////////////////////////////
    //RipRssiSubframe
    //////////////////////////////////////////////////////////////////////////////////
    if (pUlRxInputParams->rxIntMeasFlag == 0 && pUlRxInputParams->rssiMeasFlag == 0 && pUlRxInputParams->rxTherNoiseFlag == 0)
    {
        pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX++];
    }
    else
    {
        pTCB = pTcbList1[NumTasksTotalRX];
        pContext = (U32*) pTCB->ContextPtr;
        pContext[4] = pBsRx->subframeNumber;
        pContext[5] = pPhy->rxFrameNumber;

        NumTasksTotalRX++;
    }

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
    // rx_Multichan_DeModulation_tcb
    //////////////////////////////////////////////////////////////////////////////////
    numMultiChanTCBs = pBsRx->isym_eff[0];
    for (isym = 0; isym < numMultiChanTCBs; isym++)
    {
        if (numPusch == 0)
        {
            pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX];
        }
        NumTasksTotalRX++;
    }

    //////////////////////////////////////////////////////////////////////////////////
    //Run IDFT DeSpreader
    //Undo the Transform Precoding (i.e. DFT Spreading) of the complex constellation points
    //IDFT_tcb
    //////////////////////////////////////////////////////////////////////////////////

    // Single IDFT TCB
    if (numPusch == 0)
    {
        pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX];
    }
    NumTasksTotalRX++;

    //////////////////////////////////////////////////////////////////////////////////
    //Rx DeMapper to generate soft decisions
    //rx_Demapper_fx_tcb
    //////////////////////////////////////////////////////////////////////////////////
    for (isym = 0; isym < numMultiChanTCBs; isym++)
    {
        if (pUlRxInputParams->numPUSCH == 0)
        {
            pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX];
        }
        else
        {
            ReFillTCBIOBuf(pTcbList1[NumTasksTotalRX], (U32*) pFecParams,  sizeof(ULRXFECPARAMS), INOUT_BUF, 3);
        }
        NumTasksTotalRX++;
    }

    /////////////////////////////////////////////////////////////////////////////////////
    //Decode Multiplex control ACK in PUSCH
    //muxCntlACK_Dec_tcb
    /////////////////////////////////////////////////////////////////////////////////////
    if(pFecParams->muxPUSCHAckNack)
    {
        ReFillTCBIOBuf(pTcbList1[NumTasksTotalRX], (U32*) pFecParams,  sizeof(ULRXFECPARAMS), INOUT_BUF, 0);
    }
    else
    {
        pTasksRxBypass[numTasksBypass++] = pTcbList1[NumTasksTotalRX];
    }
    NumTasksTotalRX++;


    /////////////////////////////////////////////////////////////////////////////////////
    //Decode Multiplex control CQI and RI
    //muxCntlRI_CQI_Dec_tcb
    /////////////////////////////////////////////////////////////////////////////////////
    if(pBsRx->MuxControlTcbCtx == 0)
    {
        ReFillTCBIOBuf(pBsRx->pMuxCtrlTcbCtx1, (U32*) pFecParams,  sizeof(ULRXFECPARAMS), INOUT_BUF, 0);
    }
    else
    {
        ReFillTCBIOBuf(pBsRx->pMuxCtrlTcbCtx2, (U32*) pFecParams,  sizeof(ULRXFECPARAMS), INOUT_BUF, 0);
    }

    // Mark all TCBs depending on FFT to be Locked
    if(numTasksBypass)
    {
        RexSetTCBControlEx(pTasksRxBypass, numTasksBypass, REX_BYPASS_TCB);
    }

    UpdateRxChanVars (pPhy);

#ifdef SLOT_BASED_FFT
    RexReinitCells((UINT32*)&pBsRx->pFftCell[1], 1);
#else
    RexReinitCells((UINT32*)pBsRx->pFftCell, 2);
#endif

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // UPDATE TASK LIST END
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    pPhy->pBsRx->isExeTaskListReady = TRUE;

    return 0;
}
