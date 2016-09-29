//-------------------------------------------------------------------------------------------
/** @file phy_bs_tx.c
 *
 * @brief LTE PHY Base Station Transmitter
 * @author Mindspeed Technologies
 * @version $Revision: 1.206 $
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
#include "mlog.h"
#include "gputil_fx.h"

#ifndef _WIN32
#include "LteBsTxThread.h"
#include "rexcomp.h"
#include "mdmadrv.h"
#include "devinfo.h"
#include "fpdrv.h"
#else
void Log_Scrambler_Input_Output(PLTEPHYDESC pPhy, DLTXINPUTPARAMS *pDlTxInPars);
#endif

extern S16 ifft_exp;
extern S16 *ptr_dac_samp;

//-------------------------------------------------------------------------------------------
/** @brief Create the Base Station Downlink Tx task list.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  0
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy          |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------
 *  |Block Name          |CreateDlTxTaskList |
 *  ------------------------------------------
 *  |Input Stream        |none               |
 *  ------------------------------------------
 *  |Output Stream       |none               |
 *  ------------------------------------------
 *  |Config Parameters   |pPhy               |
 *  ------------------------------------------
 *  |Status Parameters   |pPhy               |
 *  ------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Create input/output pointers for Base Station Downlink Tx task list.
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
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 CreateDlTxTaskList(PLTEPHYDESC pPhy)
{
    U32 i;
    S16 mi;
    S32 stepsize_a;
    PBS_TX pBsTx = pPhy->pBsTx;
    S16 col_a;
    U32 isym;
    U32 status = 0;
    U16 NumTasksTotalTX;
    PDLTXINPUTPARAMS pTxInputParams;
    PIOParams_DlControl pDlCntrl_Pars;
    S16 *rs_grid[TXANT_FOUR*14];
    S16 *rs_grid_ctrl[3 * TXANT_FOUR];

    PTASKLIST pTaskListTx;

    pBsTx->NumTasksTotalTX = 0;
    pTxInputParams = pBsTx->pTxInputParams;
    pDlCntrl_Pars  = pBsTx->pDlCntrl_TxInPars;

    NumTasksTotalTX = 0;

    col_a = pBsTx->Ndl_symb << 1; //total number of time indices in resource grid subframe
    stepsize_a = pDlCntrl_Pars->stepsize_a;
    for (i = 0; i < (U32) pBsTx->Ntx_antennas; i++)
    {
        for (isym = 0; isym < (U32) (col_a); isym++)
            rs_grid[isym + 14 * i] = &pBsTx->a[stepsize_a * (isym + 14 * i) * 2];
    }
    for (isym = 0; isym < 3; isym++)
    {
        for (i = 0; i < (U32) pBsTx->Ntx_antennas; i++)
        {
            rs_grid_ctrl[i + isym * pBsTx->Ntx_antennas] = rs_grid[isym + 14 * i];
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //
    //Modulator for Physical Data Shared Channels (PDSCH)
    //
    {
        PTXSYMBUFS pTxSymBufs;

        U32 TempAl1;
        S8* pTemp;
        U32 TempAlTxSys = RUP32B(sizeof(TXSYSTEMINFO));
        U32 TempTxSymBufs = RUP32B(sizeof(TXSYMBUFS));
        U32 TempTxSymBuf = RUP32B( sizeof(TXSYMBUF)*MAXCHSUBFRAME );

        col_a = pBsTx->col_a;

        // Calculate some incrementing steps to be used
        TempAl1 = RUP32B(col_a * sizeof(PTXSYSTEMINFO *));            // pBsTx->pTxSys

        pTemp = (S8*)pBsTx->pTxSys;
        pTemp += TempAl1;
        pBsTx->pTxSymBufs = (PTXSYMBUFS *) pTemp;
        pTemp += TempAl1;

        // TxSymBuf
        for (i = 0; i < (U32) col_a; i++)
        {
            pBsTx->pTxSymBuf[i] = (PTXSYMBUF) pTemp;
            pTemp += TempTxSymBuf;
        }

        // TxSys
        for (i = 0; i < (U32) col_a; i++)
        {
            pBsTx->pTxSys[i] = (PTXSYSTEMINFO) pTemp;

            pTemp += TempAlTxSys;
            for (mi = 0; mi < pBsTx->Ntx_antennas; mi++)
            {
                pBsTx->pTxSys[i]->a.aSymbol[mi] = rs_grid[i + mi * 14];
            }
        }

        // TxSysBufs
        for (i = 0; i < (U32) col_a; i++)
        {
            memset(pTemp, 0x55, sizeof(TXSYMBUFS));

            pBsTx->pTxSymBufs[i] = (PTXSYMBUFS) pTemp;
            pTxSymBufs = (PTXSYMBUFS) pTemp;
            pTemp += TempTxSymBufs;

            pTxSymBufs->sizeLocRE = 0;
            pTxSymBufs->sizeBuf0 = 0;
            pTxSymBufs->sizeBuf1 = 0;
        }
    }
    //Position Pointers for each antenna, in order to have contiguous data for each antenna

    pTaskListTx = pBsTx->pExeTaskList;

    ////////////////////////////////////////////////////////
    //Create SetupSymbDataInpBufs_tcb Task for Ceva Sim
    ////////////////////////////////////////////////////////
#ifdef _WIN32
    pBsTx->pTaskTx[NumTasksTotalTX] =  SetupSymbDataInpBufs_tcb(pTaskListTx, pPhy,
                        pBsTx->pTxSymBufs[0], pBsTx->pTxSymBuf[0],
                        pTxInputParams, TCBIOBUFFILL, NULL, NULL);

    pBsTx->pSetupBufsTCB = pBsTx->pTaskTx[NumTasksTotalTX++];
#else
    pBsTx->pSetupBufsTCB = SetupSymbDataInpBufs_tcb(NULL, pPhy,
                        pBsTx->pTxSymBufs[0], pBsTx->pTxSymBuf[0],
                        pTxInputParams, TCBIOBUFFILL, NULL, NULL);
#endif

    ////////////////////////////////////////////////////////
    //Create Downlink Control Task
    ////////////////////////////////////////////////////////
    pBsTx->pTaskTx[NumTasksTotalTX] = phy_bs_tx_control_tcb(pTaskListTx, rs_grid_ctrl, pBsTx->pDlCntrl_TxInPars->startPDCCH,
                                                pBsTx->pBsDlInitMemBuf, pDlCntrl_Pars, TCBIOBUFFILL, NULL, NULL);
    pBsTx->pDlControlTCB[0] = pBsTx->pTaskTx[NumTasksTotalTX++];

    //
    //Add PhysicalChannels to Resource Grid - one symbol at a time, for multiple channels
    //
    // i.e. there is no loop for n-channels here. Multiple channels is handled within each
    // lowest level function inside the PhysicalChannels function
    //
    for (isym = 0; isym < (U32)col_a/2; isym++)
    {
        pBsTx->pTaskTx[NumTasksTotalTX] = tx_MultiChan_Modulation_tcb(pTaskListTx,
                                                                       pTxInputParams,
                                                                       pBsTx->pTxSys[isym],
                                                                       g_totalsizescr[gCtxDlFec],
                                                                       gScrIndex[gCtxDlFec],
                                                                       gScrOutBufs[gCtxDlFec],
                                                                       pBsTx->pTxSymBufs[isym],
                                                                       pBsTx->pTxSymBuf[isym],
                                                                       pBsTx->sync2_d, (U32)isym,
                                                                       TCBIOBUFFILL, NULL, NULL);
        pBsTx->pModTCB[isym] = pBsTx->pTaskTx[NumTasksTotalTX];
        NumTasksTotalTX += 1;
    }

#ifdef _WIN32
    /////////////////////////////////////////////////
    //Setup iFFT TCBS for Ceva Sim
    /////////////////////////////////////////////////
    {
        S16 row_a;
        PBS pBs = pPhy->pBs;

        row_a = pBsTx->Ndl_rb * pBs->Nrb_sc; //total number of frequency indices in resource grid

        pBsTx->pTaskTx[NumTasksTotalTX] = ifft_tcb_sim(pTaskListTx, pBsTx->a, pPhy->pBsTx->ifftOut, 0,
                    pBs->expoNfft, (col_a >> 1), row_a, pBsTx->Ntx_antennas, pBs->CPType, pBs->Ncp0, pBs->Ncp,
                    TCBIOBUFFILL, NULL, NULL);
        pBsTx->pIfftTCB[0] = pBsTx->pTaskTx[NumTasksTotalTX];
        NumTasksTotalTX += 1;

        pBsTx->pTaskTx[NumTasksTotalTX] = ifft_tcb_sim(pTaskListTx, pBsTx->a, pPhy->pBsTx->ifftOut, 1,
                    pBs->expoNfft, (col_a >> 1), row_a, pBsTx->Ntx_antennas, pBs->CPType, pBs->Ncp0, pBs->Ncp,
                    TCBIOBUFFILL, NULL, NULL);
        pBsTx->pIfftTCB[1] = pBsTx->pTaskTx[NumTasksTotalTX];
        NumTasksTotalTX += 1;
    }
#else   //_WIN32

    /////////////////////////////////////////////////
    //Setup iFFT TCBS for EVM
    /////////////////////////////////////////////////
    //Only 1 TCB for IFFT required in EVM
    {
        U32 *pContext;

        pBsTx->pIfftTCB[0] = CreateTCB(TASKID_TX_IFFT, RESOURCE_FLTP, NULL, 2 * sizeof(U32), NULL, NULL);
        pBsTx->pIfftTCB[0]->isym = 0;
        pContext = (U32*) pBsTx->pIfftTCB[0]->ContextPtr;
        pContext[0] = (U32)pPhy;
        MLogTCBAddToList(pBsTx->pIfftTCB[0],  RexGetListID(pTaskListTx));
        pBsTx->pIfftTCB[0]->ResourceIndex = 0x3E00;

        if (gLowLatencyFlag)
        {
            RexSetTCBControl(pBsTx->pDlControlTCB[0], REX_LOCKED_TCB | REX_BLOCKING_CALL);
            RexSetTCBControl(pBsTx->pSetupBufsTCB, REX_LOCKED_TCB | REX_BLOCKING_CALL);
            RexSetTCBControlEx(pBsTx->pModTCB, 7, REX_LOCKED_TCB | REX_BLOCKING_CALL);
        }
        else
        {
            RexSetTCBControl(pBsTx->pDlControlTCB[0], REX_LOCKED_TCB);
        }
    }
#endif  //_WIN32

    pBsTx->NumTasksTotalTX = NumTasksTotalTX;
    if (gLowLatencyFlag)
        pPhy->pBsTx->isExeTaskListReady = TRUE;
    else
        pPhy->pBsTx->isExeTaskListReady = FALSE;

    //Set dependencies
#ifdef SCHEDULER

    if (gLowLatencyFlag)
    {
        //Set dependencies for tx_MultiChan_Modulation_tcb, CevaSim
        for(isym = (U32)0; isym < (U32) (col_a/2); isym++)
        {
            AddDependency(pBsTx->pModTCB[isym], 1, pBsTx->pSetupBufsTCB);
        }
    }

#ifdef _WIN32
    AddDependency(pBsTx->pIfftTCB[0], 1, pBsTx->pDlControlTCB[0]);
    AddDependency(pBsTx->pIfftTCB[1], 1, pBsTx->pDlControlTCB[0]);
    for(isym = 0; isym < (U32) (col_a/2); isym++)
    {
        AddDependency(pBsTx->pIfftTCB[0], 1, pBsTx->pModTCB[isym]);
        AddDependency(pBsTx->pIfftTCB[1], 1, pBsTx->pModTCB[isym]);
    }
#endif
#endif

    gCtxDlFec = 1 - gCtxDlFec;

#ifndef _WIN32
    RegListDoneCB(pBsTx->pExeTaskList, LtePhyTxListDoneCb, NULL);
    pBsTx->txListId = RexGetListID(pBsTx->pExeTaskList);
    pBsTx->isListRunning = 0;
#endif

    return status;
}

#ifdef _WIN32
//-------------------------------------------------------------------------------------------
/** @brief PhyBsTx - LTE PHY Base Station Transmitter
 *
 *  <1> Hardware Resource:
 *  ARM
 *
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
 *  |Block Name          |PhyBsTx        |
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
 *
 *  <4> Detailed Description:
 *  Main function for Base Station Downlink Transmitter
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *  [1] 3GPP TS 36.211 Rel.8, Sec.6, "Physical Channels and Modulation"<BR>
 *
 *
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsTx(PLTEPHYDESC pPhy)
{
    PBS pBs = pPhy->pBs;
    PBS_TX pBsTx = pPhy->pBsTx;
    U32 upperBound;
    S16 row_a, col_a;
    U32 status = 0;
    S32 i;
    U32 numChan, isym;
    S16 slotNumber;

    S16 RunControl;
    S16 mi;
    S16 numCtrlSymbols;

    DLTXINPUTPARAMS *pDlTxInPars;
    PIOParams_DlControl pDlCntrl_Pars;

    U32 _size;

    S32 stepsize, stepsize_a;
    S16 *rs_grid[TXANT_FOUR*14];
    S16 *rs_grid_ctrl[3 * TXANT_FOUR];

    S16 Ndl_rb = pBsTx->Ndl_rb;
    S16 Ndl_symb = pBsTx->Ndl_symb;

    S16 *dac_samples;
    S16 scale_out_fx = pPhy->dacTxScale;
    U32 li;

    PTCB *pTcbList1;
    U32 NumTasksTotalTX = 0;

    //////////////////////////////////////////////////////////////////////////
    //Downlink Task List
    //////////////////////////////////////////////////////////////////////////
    pTcbList1 = pBsTx->pTaskTx;
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //total number of frequency indices in resource grid
    row_a       = Ndl_rb * pBs->Nrb_sc;

    pDlCntrl_Pars = pBsTx->pDlCntrl_TxInPars;
    pDlTxInPars = pBsTx->pTxInputParams;
    numChan     = pDlTxInPars->numChan;
    slotNumber  = pDlTxInPars->slotNumber;
    numCtrlSymbols = pDlTxInPars->numCtrlSymbols;
    stepsize_a     = RUP32B(row_a);
    stepsize       = RUP32B(4 * CE_NmaxDL_rb);

    col_a = Ndl_symb << 1; //total number of time indices in resource grid subframe
    for(i=0; i < pBsTx->Ntx_antennas; i++)
    {
        for(isym = 0; isym < (U32)(col_a); isym++)
            rs_grid[isym + 14*i] = &pBsTx->a[stepsize_a * (isym + 14*i) * 2];
    }

    //Only send in pointers to relevant symbols for control, don't need entire resource grid
    for(isym = 0; isym < (U32)(numCtrlSymbols); isym++)
    {
        for(i = 0; i < pBsTx->Ntx_antennas; i++)
            rs_grid_ctrl[i + isym*pBsTx->Ntx_antennas] = rs_grid[isym + 14*i];
    }

    //Do Data+Brodcast, this per symbol-based at start of loop.
    //Symbol based, like every other function, so memory required is bounded!
    //At start, because Mpdsch_sc is needed for mapper loop for each user
    //Though, scIndex only needed for mux.
    //Also, include pBitFirst calculations in following simple function
    //So, do before loop or inside symbol loop in PhysicalChannels?
    //Prefer outside loop. Maybe ARM can do this efficiently or with lookup
    //tables. (See MAC Sim function). Maybe can do scIndex inside loop, per symbol...
    //why again is scIndex matrix needed for precoding???
    //Prefer before loop, so can do sanity check "ResourceElements assigned
    //above match the FEC encoder output" before run through modulator!

    /////////////////////////////////////////////////////////
    //Print input and output to Scrambler to file
    /////////////////////////////////////////////////////////
    Log_Scrambler_Input_Output(pPhy, pDlTxInPars);

    //////////////////////////////////////////////////////////////////////////
    //Modulator for Physical Data Shared Channels (PDSCH)
    //////////////////////////////////////////////////////////////////////////
    for (i = 0; i < col_a; i++)
    {
        pBsTx->pTxSymBufs[i]->sizeLocRE = 0;
        pBsTx->pTxSymBufs[i]->sizeBuf0 = 0;
        pBsTx->pTxSymBufs[i]->sizeBuf1 = 0;
    }

    SetupSymbDataInpBufs_tcb(NULL, pPhy,  pBsTx->pTxSymBufs[0], pBsTx->pTxSymBuf[0],
                    pDlTxInPars, TCBIOBUFREFILL, NULL, pTcbList1[NumTasksTotalTX]);
    NumTasksTotalTX++;

    RunControl = 1;
    for (isym = 0; isym < (U32)(col_a/2); isym++)
    {
        //
        //Add PhysicalChannels to Resource Grid - one symbol at a time, for multiple channels
        //
        // i.e. there is no loop for n-channels here. Multiple channels is handled within each
        // lowest level function inside the PhysicalChannels function
        //
        if (RunControl)
        {
            RunControl = 0; //only need to run control code once per subframe
            phy_bs_tx_control_tcb(NULL, rs_grid_ctrl, pBsTx->pDlCntrl_TxInPars->startPDCCH, pBsTx->pBsDlInitMemBuf,
                                pDlCntrl_Pars, TCBIOBUFREFILL, NULL, pTcbList1[NumTasksTotalTX]);
            NumTasksTotalTX++;
        }

        tx_MultiChan_Modulation_tcb(NULL, pDlTxInPars, pBsTx->pTxSys[isym],
                    g_totalsizescr[0], gScrIndex[0], gScrOutBufs[0], pBsTx->pTxSymBufs[isym],
                    pBsTx->pTxSymBuf[isym], pBsTx->sync2_d, (U32)isym,
                    TCBIOBUFREFILL, NULL, pTcbList1[NumTasksTotalTX]);
        NumTasksTotalTX++;
    }

    //////////////////////////////////////////////////////////////////////
    // OFDM signal generator for slot 0 and slot1
    // convert all resource blocks for this 1ms subframe to IQ dac samples
    //////////////////////////////////////////////////////////////////////
    ifft_tcb_sim(NULL, pBsTx->a, pPhy->pBsTx->ifftOut, 0, pBs->expoNfft, (col_a >> 1),
                row_a, pBsTx->Ntx_antennas, pBs->CPType, pBs->Ncp0, pBs->Ncp,
                TCBIOBUFREFILL, NULL, pTcbList1[NumTasksTotalTX]);
    NumTasksTotalTX++;

    ifft_tcb_sim(NULL, pBsTx->a, pPhy->pBsTx->ifftOut, 1, pBs->expoNfft, (col_a >> 1),
                row_a, pBsTx->Ntx_antennas, pBs->CPType, pBs->Ncp0, pBs->Ncp,
                TCBIOBUFREFILL, NULL, pTcbList1[NumTasksTotalTX]);
    NumTasksTotalTX++;

    //////////////////////////////////////////////////////////////////////////
    //Position Pointers for each antenna, in order to have contiguous data for each antenna
    upperBound = ComputeUpperBound(pBs->Nfft);

    for (mi = 0; mi < pBsTx->Ntx_antennas; mi++)
    {
        dac_samples = pPhy->pBsTx->ifftOut[mi];
        for (li = 0; li< upperBound; li++)
        {
            fprintf(f_data1," %3.9f %3.9f\n", dac_samples[2*li]/pow(2.0, 11.0), dac_samples[2*li+1]/pow(2.0, 11.0));
        }
    }

#ifdef DEBUG_PRINT
    for (mi = 0; mi < pBsTx->Ntx_antennas; mi++)
    {
        dac_samples = pPhy->pBsTx->ifftOut[mi];
        for (li =0; li< upperBound; li++)
        {
            //Q13 output - 2 (from IFFT) ---> Q11
            fprintf(f_data_mod6_s," %3.9f %3.9f\n", dac_samples[2*li]/pow(2.0, 11.0), dac_samples[2*li+1]/pow(2.0, 11.0));
            fprintf(f_data_i16_mod6_s," %hd %hd\n", dac_samples[2*li], dac_samples[2*li+1]);
        }
    }
#endif

    g_totalsizescr[0][0] = g_totalsizescr[0][1] = 0; // For the next sub -frame
    _size = RUP32B(sizeof(U32) * MAXCHSUBFRAME);
    memset(gScrIndex[0][0], 0, _size);
    memset(gScrIndex[0][1], 0, _size);

    return status;
}
#endif  //_WIN32

#ifdef _WIN32
//-------------------------------------------------------------------------------------------
/** @brief Log input and output for Base Station Downlink Tx Scrambler.
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *  @param   pDlTxInPars - Pointer to Downlink Tx input parameter structure
 *
 *  @return  0
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy          |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pDlTxInPars   |          |pInput         |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -------------------------------------------------
 *  |Block Name          |Log_Scrambler_Input_Output|
 *  -------------------------------------------------
 *  |Input Stream        |none                      |
 *  -------------------------------------------------
 *  |Output Stream       |none                      |
 *  -------------------------------------------------
 *  |Config Parameters   |pPhy,pDlTxInPars          |
 *  -------------------------------------------------
 *  |Status Parameters   |pPhy                      |
 *  -------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Log input and output for Base Station Downlink Tx Scrambler.
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
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
void Log_Scrambler_Input_Output(PLTEPHYDESC pPhy, DLTXINPUTPARAMS *pDlTxInPars)
{
    U32 ichan, li = 0, numChan;
    PBS_TX pBsTx = pPhy->pBsTx;
    PIOParams_DlControl pDlCntrl_Pars;
    U32 length_codeword0 = 0, length_codeword1 = 0;
    U32 ChanId = 0, ScrInputOffset = 0;
    U8 *scrOut1_loc = NULL, *scrOut2_loc = NULL;
    U32 ScrOutSize = 0;
    U8 *codeword0 = NULL, *codeword1 = NULL;

    pDlCntrl_Pars = pBsTx->pDlCntrl_TxInPars;
    numChan = pDlTxInPars->numChan;
#ifdef DEBUG_PRINT
    for (ichan = pDlTxInPars->numCtrlChan; ichan < numChan; ichan++)
    {
        if (pDlTxInPars->channelType[ichan] == PDSCH)
        {
            printf("Length of BITSREAM for CHANNEL_TYPE %hd is %ld\n",
                pDlTxInPars->channelType[ichan], pDlTxInPars->G0[ichan]);
            if (ichan != pDlTxInPars->numCtrlChan)
                ScrInputOffset += (pDlTxInPars->G0[ichan - 1] + 7) >> 8;
            codeword0 = &gScrInBufs[0][0][ScrInputOffset];
            length_codeword0 = (pDlTxInPars->G0[ichan] + 7) >> 3;
            for (li = 0; li < length_codeword0; li++)
            {
                fprintf(f_data_codeword0,"%d\n", *(codeword0+li));
            }

            if (pDlTxInPars->numCodeWords[ichan] == 2)
            {
                codeword1 = &gScrInBufs[0][1][ScrInputOffset];
                length_codeword1 = (pDlTxInPars->G1[ichan] + 7) >> 3;
                for (li =0; li < length_codeword1; li++)
                    fprintf(f_data_codeword1,"%d\n", *(codeword1+li));
            }
        }
    }

    /////////////////////////////////////////////////////////
    //Print output from Scrambler to file
    /////////////////////////////////////////////////////////
    scrOut1_loc = gScrOutBufs[0][0];
    scrOut2_loc = gScrOutBufs[0][1];
    ScrOutSize = 0;
    for (ichan = pDlTxInPars->numCtrlChan; ichan < numChan; ichan++)
    {
        if (pDlTxInPars->channelType[ichan] == PDSCH)
        {
            ChanId = ichan - pDlTxInPars->numCtrlChan;
            fprintf(f_data_mod1_b_0,"%%///////////////////////////////////////////\n");
            fprintf(f_data_mod1_b_0,"%%ChanId = %d, FrameNumber = %d, SubFrameNum = %d\n",
                ChanId, pDlTxInPars->txFrameNumber, pDlTxInPars->subframeNumber);
            fprintf(f_data_mod1_b_0,"%%///////////////////////////////////////////\n");
            ScrOutSize = (pDlTxInPars->G0[ichan] + 7) >> 3;
            if (pDlTxInPars->Qm0[ichan] == QAM64)
                ScrOutSize = (pDlTxInPars->G0[ichan]) / 6;

            for(li = 0; li < ScrOutSize; li++)
                fprintf(f_data_mod1_b_0,"%d\n", scrOut1_loc[li]);

            scrOut1_loc = gScrOutBufs[0][0] + gScrIndex[0][0][ichan];

            if (pDlTxInPars->numCodeWords[ichan] == 2)
            {
                fprintf(f_data_mod1_b_1,"%%///////////////////////////////////////////\n");
                fprintf(f_data_mod1_b_1,"%%ChanId = %d, FrameNumber = %d, SubFrameNum = %d\n",
                    ChanId, pDlTxInPars->txFrameNumber, pDlTxInPars->subframeNumber);
                fprintf(f_data_mod1_b_1,"%%///////////////////////////////////////////\n");

                ScrOutSize = (pDlTxInPars->G1[ichan] + 7) >> 3;
                if (pDlTxInPars->Qm1[ichan] == QAM64)
                    ScrOutSize = (pDlTxInPars->G1[ichan]) / 6;

                for(li = 0; li < ScrOutSize; li++)
                    fprintf(f_data_mod1_b_1,"%d\n", scrOut2_loc[li]);

                scrOut2_loc = gScrOutBufs[0][1] + gScrIndex[0][1][ichan];
            }
        }
    }
#endif  //DEBUG_PRINT

    printf("\n");
    printf("DL Configuration Info for Debugging FrameNum = %d, Subframe %hd\n",
                pPhy->txFrameNumber, pBsTx->subframeNumber);
    printf("Number_of_channels %ld\n", numChan);

    printf("Number_of_phich_channels %ld\n", pPhy->pBsTx->pDlCntrl_TxInPars->number_of_phich_channels);
    printf("Ng %hd\n", pPhy->pBsTx->pTxInputParams->Ng);
    printf("PilotPower %3.4f dB\n", 20.0*log10(pPhy->pBsTx->pDlCntrl_TxInPars->pilotScaleFactor/16384.0));
    printf("PsyncPower %3.4f dB\n", 20.0*log10(pPhy->pBsTx->pTxInputParams->primarySyncScaleFactor/16384.0));
    printf("SsyncPower %3.4f dB\n", 20.0*log10(pPhy->pBsTx->pTxInputParams->secondarySyncScaleFactor/16384.0));
    printf("CFI power %3.4f dB\n", 20*log10(pPhy->pBsTx->pDlCntrl_TxInPars->controlIndicatorScaleFactor/16384.0));
    printf("Ncell_Id  %hd\n", pDlTxInPars->Ncell_id);
    printf("numCtrlSymbols %hd\n",pDlTxInPars->numCtrlSymbols);

    for (ichan = 0; ichan < numChan; ichan++)
    {
        printf("Channel type %hd\n", pDlTxInPars->channelType[ichan]);
        printf("Channel power %3.4f dB\n", 20.0*log10(pDlTxInPars->phyChanScale_factor[ichan]/16384.0));

        if (pDlTxInPars->channelType[ichan] == PDSCH)
        {
            printf("Rvidx %ld\n", pDlTxInPars->rV0[ichan]);
            printf("nRNTI %hd\n", pDlTxInPars->nRNTI[ichan]);
            printf("Qm %hd\n", (S16)pDlTxInPars->modType0[ichan] * 2);
        }

        if (pDlTxInPars->channelType[ichan] == PHICH)
        {
            printf("Phich Group %hd\n", pDlCntrl_Pars->PHICH_Grp[ichan]);
            printf("Phich Sequence %hd\n", pDlCntrl_Pars->PHICH_SequenceIndex1[ichan]);
            printf("Phich Bit %hd\n", pDlCntrl_Pars->PHICH_Bit1[ichan]);
        }
    }
}
#endif  //_WIN32

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------
/** @brief Check if we need to insert PBCH into task list
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  0
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy          |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------
 *  |Block Name          |InsertPBCH              |
 *  -----------------------------------------------
 *  |Input Stream        |none                    |
 *  -----------------------------------------------
 *  |Output Stream       |none                    |
 *  -----------------------------------------------
 *  |Config Parameters   |pPhy                    |
 *  -----------------------------------------------
 *  |Status Parameters   |pPhy                    |
 *  -----------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Check if we need to insert PBCH into task list
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
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 InsertPBCH(PLTEPHYDESC pPhy)
{
    PBS_TX pBsTx = pPhy->pBsTx;
    U32 numChan;
    U8* scrOut1_loc;
    PDLTXINPUTPARAMS pTxInputParams = pBsTx->pTxInputParams;
    U8* pSrc                        = (U8*) pTxInputParams->pBchDataBuffer;

    numChan = pTxInputParams->numChan-1;

    // Frame 1, 2, 3
    if((pTxInputParams->txFrameNumber & 0x3) != 0)
    {
        scrOut1_loc = gScrOutBufs[gCtxDlFec][0] + g_scrsize[gCtxDlFec][0];
        gScrIndex[gCtxDlFec][0][numChan] = g_scrsize[gCtxDlFec][0];

        g_scrsize[gCtxDlFec][0] += RUP32B(PBCHDATASIZE/8);
        g_totalsizescr[gCtxDlFec][0] += RUP32B(PBCHDATASIZE/8);

        //Append a channel at the end of the TXVECTOR with the saved PBCH info
        memcpy(scrOut1_loc, pTxInputParams->ptrPBCH, PBCHDATASIZE/8);

        pTxInputParams->ptrPBCH += PBCHDATASIZE/8;
    }
    else        // Frame 0
    {
        if (gPhyInternalBch)
        {
            pSrc += (PBCH_LEN_IN_BYTES * (pPhy->txFrameNumber>>2));

            pTxInputParams->ptrPBCH = pTxInputParams->startPBCH;

            gScrIndex[gCtxDlFec][0][numChan] = g_scrsize[gCtxDlFec][0];
            scrOut1_loc = &gScrOutBufs[gCtxDlFec][0][g_scrsize[gCtxDlFec][0]];

            memcpy(pTxInputParams->startPBCH, pSrc, PBCH_LEN_IN_BYTES);
            memcpy(scrOut1_loc, pTxInputParams->ptrPBCH, PBCHDATASIZE/8);

#ifdef DEBUG_PRINT
            {
                FILE *f_pbch;
                int i;

                f_pbch = fopen("pbch_scr_out","w");
                for (i = 0; i < (U32)(PBCHDATASIZE/8); i++)
                    fprintf(f_pbch,"%d\n", pTxInputParams->ptrPBCH[i]);
                fclose(f_pbch);
            }
#endif

            g_totalsizescr[gCtxDlFec][0] += RUP32B(PBCHDATASIZE/8);
            g_scrsize[gCtxDlFec][0] += RUP32B(PBCHDATASIZE/8);
            pTxInputParams->ptrPBCH += PBCHDATASIZE/8;
        }
    }

    return 0;
}

//-------------------------------------------------------------------------------------------
/** @brief Update Downlink Tx Task List, phy_bs_tx_control section
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  0
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy          |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------
 *  |Block Name          |UpdateDlControlTaskList |
 *  -----------------------------------------------
 *  |Input Stream        |none                    |
 *  -----------------------------------------------
 *  |Output Stream       |none                    |
 *  -----------------------------------------------
 *  |Config Parameters   |pPhy                    |
 *  -----------------------------------------------
 *  |Status Parameters   |pPhy                    |
 *  -----------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Update Downlink Tx Task List, phy_bs_tx_control section
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
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 UpdateDlControlTaskList(PLTEPHYDESC pPhy)
{
    PBS_TX pBsTx = pPhy->pBsTx;
    U32 status = 0;
    S16 numCtrlSymbols;
    PDLTXINPUTPARAMS pTxInputParams = pBsTx->pTxInputParams;
    PTCB pTCB1;

    numCtrlSymbols = pTxInputParams->numCtrlSymbols;
    pTCB1 = pBsTx->pDlControlTCB[0];

    if(numCtrlSymbols == 0)
    {
#ifdef WIN32
        MarkTCBControl(pTCB1, BYPASS_TCB, 0); //mark it as a bypass TCB
#else
        RexSetTCBControl(pTCB1, BYPASS_TCB);
#endif
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------
/** @brief Executes the Base Station Downlink Tx task list
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  0
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy          |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ------------------------------------------
 *  |Block Name          |PhyBsTxExeTaskList |
 *  ------------------------------------------
 *  |Input Stream        |none               |
 *  ------------------------------------------
 *  |Output Stream       |none               |
 *  ------------------------------------------
 *  |Config Parameters   |pPhy               |
 *  ------------------------------------------
 *  |Status Parameters   |pPhy               |
 *  ------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Executes the Base Station Downlink Tx task list
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
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsTxExeTaskList(PLTEPHYDESC pPhy)
{
    if (pPhy->pBsTx->isExeTaskListReady != TRUE)
    {
        printf("List execution underrun !!\n");
        return 0;
    }

    PhyBsTxPreExec(pPhy);

    ExeTSchedList(pPhy->pBsTx->pExeTaskList);

#ifdef _WIN32
    {
        S16* dac_samples;
        U32 upperBound, i = 0, mi = 0 ;

        upperBound = ComputeUpperBound(pPhy->pBs->Nfft);
        for (mi = 0; mi < (U32)pPhy->pBsTx->Ntx_antennas; mi++)
        {
            dac_samples = pPhy->pBsTx->ifftOut[mi];
            for (i =0; i< upperBound; i++)
            {
                //Q13 output - 2 (from IFFT) ---> Q11
                fprintf(f_data1," %3.9f %3.9f\n", dac_samples[2*i]/pow(2.0, 11.0), dac_samples[2*i+1]/pow(2.0, 11.0));
#ifdef DEBUG_PRINT
                fprintf(f_data_mod6_s," %3.9f %3.9f\n", dac_samples[2*i]/pow(2.0, 11.0), dac_samples[2*i+1]/pow(2.0, 11.0));
                fprintf(f_data_i16_mod6_s," %hd %hd\n", dac_samples[2*i], dac_samples[2*i+1]);
#endif
            }
        }
    }
#endif  //_WIN32

    // postExe
    PhyBsTxPostExec(pPhy);

    return 0;
}

//-------------------------------------------------------------------------------------------
/** @brief Function called before Base Station Downlink Tx task list begins
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  0
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy          |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |PhyBsTxPreExec |
 *  --------------------------------------
 *  |Input Stream        |none           |
 *  --------------------------------------
 *  |Output Stream       |none           |
 *  --------------------------------------
 *  |Config Parameters   |pPhy           |
 *  --------------------------------------
 *  |Status Parameters   |pPhy           |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Function is called before Task list for BaseStation Downlink Tx is executed.  Sets up input
 *  output pointers for iFFT.
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
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsTxPreExec(PLTEPHYDESC pPhy)
{
    U32 status = 0;
    return status;
}
//-------------------------------------------------------------------------------------------
/** @brief Function called after Base Station Downlink Tx task list is complete
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy - Pointer to the current active PHY structure
 *
 *  @return  0
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -----------------------------------------------------------------------------------------------------
 *  |Name          |Name in   |Direction      |Length  |Type          |Data Format |Valid Range |Units  |
 *  |              |Standard  |               |        |              |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  |pPhy          |          |pInput/pOutput |        |structure     |            |            |       |
 *  -----------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |PhyBsTxPostExec|
 *  --------------------------------------
 *  |Input Stream        |none           |
 *  --------------------------------------
 *  |Output Stream       |none           |
 *  --------------------------------------
 *  |Config Parameters   |pPhy           |
 *  --------------------------------------
 *  |Status Parameters   |pPhy           |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Function is called after Task list for BaseStation Downlink Tx is finished.  Free dynamic memory
 *  and return.
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
 *  \ingroup group_lte_tx_downlink_modulator
 *
 **/
//-------------------------------------------------------------------------------------------
U32 PhyBsTxPostExec(PLTEPHYDESC pPhy)
{
    U32 _size;

    ReinitTaskList(pPhy->pBsTx->pExeTaskList, NULL);

    _size = RUP32B(sizeof(U32) * MAXCHSUBFRAME);
    memset(gScrIndex[0], 0, _size);
    memset(gScrIndex[1], 0, _size);

    if (gLowLatencyFlag)
        pPhy->pBsTx->isExeTaskListReady = TRUE;
    else
        pPhy->pBsTx->isExeTaskListReady = FALSE;

    return 0;
}


