//-------------------------------------------------------------------------------------------
/** @file LteBsTxDataSduThread.c
 *
 * @brief This file implements the DL SDU processing for the LTE BS
 * @author Mindspeed Technologies
 * @version $Revision: 1.37 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

/******************************************************************************
**********             CRITICAL HARDWARE FEC INFORMATION             **********
*******************************************************************************

X7 Device Information:
----------------------
1) For all previous devices, there is a problem with the CTC encoder rate
matching function.  Whenever the specified Ncb < Kw and k0 > 0 and a circular
buffer wrap-around occurs, the next read position after the wrap-around is set
to k0 instead of 0.  For all other conditions the rate matching works properly.
This problem has been fixed in the X7 device.  However, the fix requires that
the PHY code when programming the FEC's CTC rate matching buffer length uses
the value Ncb-32 instead of Ncb.

2) Unfortunately, the X7 fix described above has inadvertently broken the CC
encoder's rate matching.  This occurred because for the CC encoder the rate
matching buffer length is computed internally by the FEC and it was not
correctly modified to compute the now required Ncb-32 instead of Ncb.
Consequently, any CC encoder rate matching that results in a circular buffer
wrap-around must be performed externally by the ARM.

There is a new function DevInfoIsT4Kx7() that returns true if and only if
the device is a X7.  This function is used by the PHY code to conditionally
execute the required code needed to support the X7 device.

The code segments needed to support the X7 device are surrounded by:

    //X7X7X7X7X7X7

The code segments needed to support the X7 device are enabled by defining the
control parameter X7_DEV_ENABLED.

******************************************************************************/

//#define X7_DEV_ENABLED      1       // Define to enable X7 specific code

//*****************************************************************************

#include "ltephy.h"
#include "ltephydef.h"
#include "lte_entry.h"
#include "ext_phy.h"
#include "4gmx_serv.h"
#include "Debug.h"
#include "prototypes.h"
#include "mdmadrv.h"
#include "LteBsTxDataSdu.h"
#include "supervisor.h"
#include "FECdrv.h"
#include "mlog.h"
#include "ltertconfig.h"
#include "appprintf.h"
#include "ext_phy.h"

//#define TEST_DLFEC_PROTECTION 1
#ifdef TEST_DLFEC_PROTECTION
static U32 fec_reset_counter = 0;
#endif



void UnlockModTcb(U32 mask, PLTE_PHY_SDU_DESC pSduDesc)
{
    if (gLowLatencyFlag)
    {
        pSduDesc->unlockPdschTcb |= mask;
        if ((pSduDesc->unlockPdschTcb & UNLOCK_DLSCH_TCB) == UNLOCK_DLSCH_TCB)
        {
            RexResetTCBControlEx(pSduDesc->modTask, 7, REX_LOCKED_TCB);
            pSduDesc->unlockPdschTcb = UNLOCK_DLSCH_TCB_INIT;
        }
    }
}

void DispatchDlFec(PLTE_PHY_SDU_DESC pSduDesc)
{
    if(pSduDesc->dlCrcDone && pSduDesc->dlFecDescriptorDone)
    {
        pSduDesc->t_fec = GET_TICKS;

        FecDlRunQ(pSduDesc->pFecDlStart, TIME_TX_FEC);

        pSduDesc->dlCrcDone = 0;
        pSduDesc->dlFecDescriptorDone = 0;
    }
}

void PBCHScramblerDone(void* pParam)
{
    PLTE_PHY_SDU_DESC pSduDesc = (PLTE_PHY_SDU_DESC)pParam;

    UnlockModTcb(UNLOCK_DLSCH_TCB_BCH_COMPLETE, pSduDesc);

    MLogTask(PID_TXSDU_BCH_SCRAMBLER_DONE, RESOURCE_SYS_AXI, pSduDesc->tBchStart, GET_TICKS);
    return;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// X6, X7
///////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------
/** @brief Tx MDMA / FEC Callback function. This is called by 4GMX once the hardware resource interrupts the ARM after
 *              finishing a task
 *
 *  @param   pCtx  Pointer to TX SDU Encoder Thread Context
 *
 *  @return   none
 *
 *  \ingroup group_lte_phy_control_rt_tx_sdu
 *
**/
//-------------------------------------------------------------------------------------------
void LtePhySduEncMdmaFecCbDone1(LPVOID pCtx)
{
    PLTE_PHY_SDU_DESC pSduDesc;
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    U32 tt;

    _ASSERT_PTR(pCtx);
    pSduDesc = (PLTE_PHY_SDU_DESC) pCtx;

    if (pSduDesc->sduState == SDU_MDMA_DONE)
    {
        U8 tmp;
        U32 i;
        for(i=0; i<pSduDesc->numSDU; i++)
        {
            tmp = pSduDesc->CRC24A_output_array[i][pSduDesc->num_in_bytes[i]];
            pSduDesc->CRC24A_output_array[i][pSduDesc->num_in_bytes[i]]
                 = pSduDesc->CRC24A_output_array[i][pSduDesc->num_in_bytes[i] + 2];
            pSduDesc->CRC24A_output_array[i][pSduDesc->num_in_bytes[i] + 2] = tmp;
        }

        pSduDesc->sduState = SDU_FEC_DONE;

        pSduDesc->dlCrcDone = 1;
        DispatchDlFec(pSduDesc);

        MLogTask(PID_TXSDU_DATA_CRC, RESOURCE_SYS_AXI, pSduDesc->t_mdma, GET_TICKS);
    }
    else if (pSduDesc->sduState == SDU_FEC_DONE)
    {
        tt = GET_TICKS;
        MLogTask(PID_TXSDU_DATA_FEC, RESOURCE_FECDL, pSduDesc->t_fec, tt);

        pSduDesc->sduState = SDU_SCRAMBLER_DONE;
        pSduDesc->t_mdma_scr = tt;

        MdmaRunExQ (MDMA_HW_SYS, pSduDesc->pMdmaScrStart, LtePhySduEncMdmaFecCbDone1, pSduDesc, 200);
    }
    else if (pSduDesc->sduState == SDU_SCRAMBLER_DONE)
    {
        tt = GET_TICKS;
        MLogTask(PID_TXSDU_DATA_SCRAMBLER, RESOURCE_SYS_AXI, pSduDesc->t_mdma_scr, tt);

        UnlockModTcb(UNLOCK_DLSCH_TCB_PDSCH_DONE, pSduDesc);

        LteBsPhyStatsTimer(pLteBsPhyStats->timerStats.DLSDUProcessing, pSduDesc->startTime, GET_TICKS);

        MLogTask(PID_TXSDU_DATA_COMPLETE, RESOURCE_LARM, tt, GET_TICKS);

        pSduDesc->sduState = SDU_NEW;
    }
    return;
}

U32 LteBsTxSduProcessingRT(U32 pSduDescIn)
{
    PLTE_PHY_SDU_DESC pSduDesc = (PLTE_PHY_SDU_DESC) pSduDescIn;
    PDLTXINPUTPARAMS pTxInputParams = pSduDesc->pTxInputParams;
    U32 numPdschSdu, devX7, scrInit, armCB;
    U32 modType, rvIdx;
    U32 numBytes[MAXCHSUBFRAME*2], cwId[MAXCHSUBFRAME*2], chanId[MAXCHSUBFRAME*2];
    U32 countSdu, NL;
    U32 chanNum;
    PFEC_DL_FDESC pFecDl = NULL;
    PFEC_DL_FDESC q = NULL;
    PMDMA_FDESC pMdmaScr;
    PFEC_DL_FDESC pFecDlPrev = NULL;
    U32 k, n;
    U8 *pInp, *pOut;
    U32 ninbytes[MAXCHSUBFRAME*2];
    U32 noutbits, noutbits1, noutbytes;
    S16 numBits[MAXCHSUBFRAME*2];
    U8* pFecTxOut = NULL;
    S16 K = 0, crcsel, Qm;
    DLHWCTX hw_ctx = { 0 };
    RMParameters *prm, prm_obj;
    S32 NIR;
    S16 C_num_code_blocks, Cplus, Kplus, Cminus, Kminus, FillerBits;
    U8 *scrOut_local;
    U32 *scrIndex_local;
    S32 length_s, length_s1;
    U32 _totalsizescr_local;
    U32 packMode;
    U16 nRNTI;
    U32 fecInOutBits;
    U8 *scrInPtr[2];

#ifdef TEST_DLFEC_PROTECTION
    fec_reset_counter++;
#endif

    numPdschSdu = pSduDesc->numSDU;
    devX7 = pSduDesc->devX7;
    scrInit = pSduDesc->scrInit;
    armCB = pSduDesc->armCB;

    scrInPtr[0] = pSduDesc->ScrInBufs[0];
    scrInPtr[1] = pSduDesc->ScrInBufs[1];

    pSduDesc->t_arm = GET_TICKS;

    pMdmaScr = MdmaAllocDesc(MDMA_HW_SYS, numPdschSdu);
    pSduDesc->pMdmaScrStart = pMdmaScr;
    _ASSERT_PTR (pMdmaScr);

    for(countSdu = 0; countSdu < numPdschSdu; countSdu++)
    {
        numBytes[countSdu] = pSduDesc->num_in_bytes[countSdu];
        cwId[countSdu]     = pSduDesc->cwId[countSdu];
        chanId[countSdu]   = pSduDesc->chanId[countSdu];
    }

    for(countSdu = 0; countSdu < numPdschSdu; countSdu++)
    {
        if (pTxInputParams->txChanIdMappingTable[chanId[countSdu]] != -1)
        {
            chanNum = pTxInputParams->txChanIdMappingTable[chanId[countSdu]];

            if(cwId[countSdu] == 0)
                fecInOutBits = pTxInputParams->G0[chanNum];
            else
                fecInOutBits = pTxInputParams->G1[chanNum];

            //////////////////////////////////////////////////////////////////////////
            if(numBytes[countSdu] > 0)
            {
                // Allocate output array
                if(fecInOutBits > 0)
                {
                    pFecTxOut = scrInPtr[cwId[countSdu]];
                    scrInPtr[cwId[countSdu]] += (fecInOutBits + 7) >> 3;
                }
                else
                {
                    stop_printf("Wrong SDU size: %d\n", fecInOutBits);
                        return 0;
                }
            }

            //This is a subtle understanding of how NL supposed to be set.
            //Originaly it what thought that it is 1 if number of layers is 1 and 2 otherwise
            //In reality, it should be look at the number of layers per codeword
            NL = 2;
            if( (pTxInputParams->numCodeWords[chanNum] == (S16) pTxInputParams->numTxLayers[chanNum]) ||
                ( (S16) pTxInputParams->numTxLayers[chanNum] == 3 && cwId[countSdu] == 0) )
            {
                NL = 1;
            }

            Qm = (S16) (2 * pTxInputParams->modType0[chanNum]);
            if(cwId[countSdu] != 0)
                Qm = (S16) (2 * pTxInputParams->modType1[chanNum]);

            nRNTI = (U16) pTxInputParams->nRNTI[chanNum];

            // Read code block segmentation table using number of bytes -1 as index.
            // The number of code blocks (C_num_code_blocks = Cplus) are in the lower 16 bits.
            // The code block size (Kplus) is in the upper 16 bits.
            // For all valid LTE transport block sizes Cminus=0, Kminus=0 and FillerBits=0.
            // Invalid transport block sizes are indicated by a 0 table entry.
            k = Table_DL_FEC_CTC_CB[pSduDesc->num_in_bytes[countSdu]-1];

            // If table indicates transport block size is legal, then.
            if(k != 0)
            {
                // Use the lower 16-bits as number of total code blocks and Cplus.
                C_num_code_blocks = k & 65535;

                // Use the upper 16 bits as code block size K = Kplus.
                K = k >> 16;
            }
            // Else this is an invalid LTE transport block size.
            else
            {
                // Segment bitstream block into small sub-block streams
                // according to LTE Standard rules
                lte_code_blk_seg(pSduDesc->num_in_bytes[countSdu] * 8 + NUM_PARITY,
                    &C_num_code_blocks, &Cplus, &Cminus, &Kplus, &Kminus, &FillerBits, BlkSizTab);
            }

            prm = &prm_obj;

            _ASSERT_PTR(prm);

            // Initialize number of output bytes
            noutbits = 0;

            crcsel = 0;
            if (C_num_code_blocks > 1)
            {
                crcsel = 2;
            }

            pFecDl = FecDlAllocDesc(C_num_code_blocks);
            _ASSERT_PTR(pFecDl);

            // set hardware context
            memset(&hw_ctx, 0, sizeof(hw_ctx));

            hw_ctx.IRQEN = 0;
            hw_ctx.CTC_CCn = 1;
            hw_ctx.CRC_Sel = crcsel;
            hw_ctx.Modulation = Qm >> 1;
            hw_ctx.Rate_Index = 0;
            hw_ctx.Bypass_Encoder = 0;
            hw_ctx.Harq_req = 0;
            hw_ctx.Bypass_Interleaver = 0;
            hw_ctx.StatusComp = 0;

            q = pFecDl;

            if(countSdu == 0)
            {
                pSduDesc->pFecDlStart = pFecDl;
            }
            else
            {
                pFecDlPrev->Next = pFecDl;
            }

            pInp = pSduDesc->CRC24A_output_array[countSdu];
            pOut = pFecTxOut;

            // NIR = Nsoft/8 for UE category
            NIR = UE_NIR_Table[5-1];

            // If UE is configured for modes 3 or 4, then NIR = NIR/2
            if(pTxInputParams->numCodeWords[chanNum] > 1)
            {
                NIR = NIR >> 1;
            }

            rvIdx = pTxInputParams->rV0[chanNum];
            if(cwId[countSdu] != 0)
                rvIdx = pTxInputParams->rV1[chanNum];

            //*********************************************************************
            // If illegal transport block size, then
            if(k == 0)
            {
                // Allocate temporary space for rate matching parameters
                for (k = 0; k < C_num_code_blocks; k++)
                {
                    K = Kplus;
                    if (k < Cminus)
                    {
                        K = Kminus;
                    }

                    // Compute rate matching parameters E, k0 and Ncb.
                    Compute_Rate_Matching_Parameters(K, C_num_code_blocks, NL, fecInOutBits, Qm, k, NIR, prm);


                    ninbytes[k] = (K - FillerBits) >> 3;

                    // If X7 device, then use Ncb-32 for rate matching buffer length
                    if(devX7)
                    {
                        prm->Ncb = prm->Ncb - 32;
                    }

                    hw_ctx.BlockSize = K;
                    hw_ctx.Filler = FillerBits;
                    hw_ctx.E_Param = prm->E & 0x1FFFF;
                    hw_ctx.LTERateMaching_K0 = prm->k0[rvIdx] >> 1;
                    hw_ctx.Ncb_14_13 = (prm->Ncb >> 13) & 3;
                    hw_ctx.Multi_FEC_blk = 0;

                    noutbits += prm->E;
                    noutbits1 = prm->E;

                    // x4 specific code
                    noutbytes = prm->E >> 3;

                    numBits[k] = noutbits1 - (noutbytes << 3);

                    if(k > 0)
                    {
                        if(numBits[k-1] != 0)
                        {
                            hw_ctx.Multi_FEC_blk = 1;
                            numBits[k] += numBits[k-1];
                            if(numBits[k] >= 8)
                            {
                                numBits[k] -= 8;
                                noutbytes += 1;
                            }
                        }
                    }

                    if( (k == (C_num_code_blocks - 1)) && (numBits[k] != 0) )
                    {
                        noutbytes += 1;
                    }

                    FecDlSetHwCtx(q, hw_ctx);
#ifndef TEST_DLFEC_PROTECTION
                    FecDlSetData(q, pInp, ninbytes[k], pOut, noutbytes);
#else
                    if(fec_reset_counter % 15 == 14)
                    {
                        FecDlSetData(q, pInp, 0, pOut, noutbytes);
                    }
                    else
                        FecDlSetData(q, pInp, ninbytes[k], pOut, noutbytes);
#endif
                    q->IoBdesc[1].BCtrl |= (prm->Ncb & 0x1FFF) << 18;

                    q->CbDone = NULL;

                    pOut += noutbytes;

                    pInp += ninbytes[k] - NUM_PARITY / 8;

                    FillerBits = 0;

                    pFecDlPrev = q;

                    q = (PFEC_DL_FDESC)q->Next;

                }
            }
            //*********************************************************************
            // Else valid transport block size
            else
            {
                // Compute rate matching parameters E, k0 and Ncb.
                Compute_Rate_Matching_Parameters(K, C_num_code_blocks, NL, fecInOutBits, Qm, 0, NIR, prm);


                // If X7 device, then use Ncb-32 for rate matching buffer length
                if(devX7)
                {
                    prm->Ncb = prm->Ncb - 32;
                }

                // Compute number of input bytes (n)
                n = K >> 3;

                hw_ctx.BlockSize = K;
                hw_ctx.Filler = 0;
                hw_ctx.LTERateMaching_K0 = prm->k0[rvIdx] >> 1;
                hw_ctx.Ncb_14_13 = (prm->Ncb >> 13) & 3;

                // Allocate temporary space for rate matching parameters
                for (k = 0; k < C_num_code_blocks; k++)
                {
                    // If code block index exceeds limit, then start using alternate (larger) E
                    if(k > prm->NumCodeBlockUseE) prm->E = prm->E2;

                    hw_ctx.E_Param = prm->E & 0x1FFFF;

                    noutbits += prm->E;
                    noutbits1 = prm->E;

                    // x4 specific code
                    noutbytes = prm->E >> 3;

                    numBits[k] = noutbits1 - (noutbytes << 3);

                    hw_ctx.Multi_FEC_blk = 0;
                    if(k > 0)
                    {
                        if(numBits[k-1] != 0)
                        {
                            hw_ctx.Multi_FEC_blk = 1;
                            numBits[k] += numBits[k-1];
                            if(numBits[k] >= 8)
                            {
                                numBits[k] -= 8;
                                noutbytes += 1;
                            }
                        }
                    }

                    if( (k == (C_num_code_blocks - 1)) && (numBits[k] != 0) )
                    {
                        noutbytes += 1;
                    }

                    FecDlSetHwCtx(q, hw_ctx);

#ifndef TEST_DLFEC_PROTECTION
                    FecDlSetData(q, pInp, n, pOut, noutbytes);
#else
                    if(fec_reset_counter % 15 == 14)
                    {
                        FecDlSetData(q, pInp, 0, pOut, noutbytes);
                    }
                    else
                        FecDlSetData(q, pInp, n, pOut, noutbytes);
#endif
                    q->IoBdesc[1].BCtrl |= (prm->Ncb & 0x1FFF) << 18;

                    q->CbDone = NULL;

                    pOut += noutbytes;

                    pInp += n - NUM_PARITY / 8;

                    pFecDlPrev = q;

                    q = (PFEC_DL_FDESC)q->Next;
                }
            }
            //*********************************************************************

            // Scramble the output
            if (cwId[countSdu] == 0)
            {
                scrOut_local = pSduDesc->ScrOutBufs[0];
                scrIndex_local = pSduDesc->ScrIndex[0];
                _totalsizescr_local = pSduDesc->totalsizescr[0];
            }
            else
            {
                scrOut_local = pSduDesc->ScrOutBufs[1];
                scrIndex_local = pSduDesc->ScrIndex[1];
                _totalsizescr_local = pSduDesc->totalsizescr[1];
            }

            scrOut_local += _totalsizescr_local;

            packMode = 0; // Packing mode
            length_s = (noutbits + 7) >> 3;
            length_s1 = length_s;
            if ((Qm >> 1) == QAM64)
            {
                packMode = 1;
                length_s1 = divide_ceil(noutbits, 6);
            }

            scrIndex_local[chanNum] = _totalsizescr_local;
            _totalsizescr_local += RUP32B(length_s1);

            if (cwId[countSdu] == 0)
            {
                pSduDesc->totalsizescr[0] = _totalsizescr_local;
            }
            else
            {
                pSduDesc->totalsizescr[1] = _totalsizescr_local;
            }

            n = (nRNTI << 14) + scrInit;
            if (cwId[countSdu] != 0)
            {
                 n = n + (1 << 13);
            }

            pMdmaScr->IoBdesc[1].BPtr = (UINT32)pFecTxOut;
            pMdmaScr->IoBdesc[1].BCtrl = BCTRL_BLAST_MASK | length_s;
            pMdmaScr->IoBdesc[0].BPtr = (UINT32)scrOut_local;
            pMdmaScr->IoBdesc[0].BCtrl = BCTRL_BLAST_MASK | length_s1;

            MdmaSet4GSCR(pMdmaScr, n, 1, packMode, 0);

            modType = pTxInputParams->modType0[chanNum];
            if(cwId[countSdu] != 0)
                modType = pTxInputParams->modType1[chanNum];
            if(modType != QAM64)
            {
                numBytes[countSdu] = (noutbits + 7) >> 3;
            }
            else
            {
                numBytes[countSdu] = divide_ceil(noutbits, 6);
            }
            pSduDesc->scrsize[cwId[countSdu]] += RUP32B(numBytes[countSdu]);

            pMdmaScr = pMdmaScr->Next;
        }
    }

    pFecDlPrev->HwContext.IRQEN = 1; // enable IRQ
    pFecDlPrev->HwContext.StatusComp = 0;
    pFecDlPrev->CbDone = (void (*)(LPVOID))armCB;
    pFecDlPrev->CbContext = (LPVOID) pSduDesc;

    if (pSduDesc->dlCrcDone)
    {
        pSduDesc->sduState = SDU_FEC_DONE;
        pSduDesc->t_fec = GET_TICKS;
        FecDlRunQ(pSduDesc->pFecDlStart, TIME_TX_FEC);

        pSduDesc->dlCrcDone = 0;
    }

    MLogTask(PID_TXSDU_DATA_FEC_BUILD, RESOURCE_LARM, pSduDesc->t_arm, GET_TICKS);
    return 0;
}

