//-------------------------------------------------------------------------------------------
/** @file tcb_tasks.c
 *
 * @brief TCB Tasks to run on ARM or Reference C-Code
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
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
#include "tc7000_lte.h"
#include "tc1700.h"
#include "math_ext32.h"
#include "prototypes.h"
#include "papr_rcf.h"
#include "gputil_fx.h"
#include "basic_op_cc.h"
#include "bs_rx_prach.h"

#ifndef _WIN32
#include "mdmadrv.h"
#include "LteBsApiThread.h"
#endif

#else

#include <asm-dsp.h>
#include "..\Ceva\Prj\LteApp\global.h"
#include "ltephy.h"    //This includes typedef.h
#include "prototypes.h"    //This includes typedef.h
#include <basic_op.h>

#ifdef EVM
#include "cevamdmadrv.h"
#include "cevafecdrv.h"
//#include "cevaprintf.h"
#endif

extern U8 *DataBuff_Bank1, *DataBuff_Bank2, *DataBuff_Bank3;
extern U8 *DataBuff_Bank1a, *DataBuff_Bank3a;
extern U8 *DataBuff_Bank1_Off, *DataBuff_Bank2_Off, *DataBuff_Bank3_Off;
extern S16 BlkSizTab[];
#endif



//#define TEST_CEVA_PROTECTION 1
//#define TEST_DLFEC_PROTECTION 1
//#define TEST_ULFEC_PROTECTION 1
//#define TEST_MDMA_PROTECTION 1

#ifndef CEVA_INTRINSICS


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Downlink ifftcg_c_16t24d Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTask                |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |ifftcg_c_16t24d_task               |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Downlink ifftcg_c_16t24d function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: ifftcg_c_16t24d() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void ifftcg_c_16t24d_task(PTCB pTask)
{
    PIOParams_FFT pContext;
    S16 *pRS_Grid, **pIFFTOut;
    S32 Ncpsam, i, count_s = 0;
    S16 *pIfft_In, *pIfft_Out;
    S16 expo, *ptr_dac_samp;
    S32 mi, FFTLen, stepsize_a, slotLen;
    U32 Idx1 = 0, Idx2 = 0, isym, col_a;
    S16 fftExpo, slotNum, num_sym, Ncp, Ncp0;
    S16 numSubCar, numTxAnt, CPType;

    pContext = (PIOParams_FFT) (pTask->ContextPtr);

    fftExpo = pContext->FFT_Order;
    slotNum = pContext->slotNum;
    num_sym = pContext->num_sym;
    Ncp     = pContext->Ncp;
    Ncp0    = pContext->Ncp0;
    numSubCar =  pContext->numSubCar;
    numTxAnt  =  pContext->numTxAnt;
    CPType    =  pContext->CPType;

    pIFFTOut = (S16 **)MemAlloc(numTxAnt*sizeof(S16*), DDR_MEM, 0);

    FFTLen = (1 << fftExpo);
    stepsize_a = RUP32B(numSubCar);
    slotLen = 2*((FFTLen * num_sym) + (Ncp0 + Ncp * (num_sym-1)));

    pRS_Grid = (S16 *) extractTCBIOBuf(pTask, 0);
    for(mi = 0; mi < numTxAnt; mi++)
    {
        pIFFTOut[mi] = (S16 *) extractTCBIOBuf(pTask, (1 + mi));

#ifdef DEBUG_PRINT
        fprintf(f_data_mod5a[mi],"%%SlotNum = %d\n", slotNum);
        fprintf(f_data_imod5a[mi],"%%SlotNum = %d\n", slotNum);
#endif
    }
    phy_printf("Executing Task [%3d] TASKID_TX_IFFT\n", TASKID_TX_IFFT);

    pIfft_In  = MemAlloc(FFTLen*2*sizeof(S16), DDR_MEM, 0);
    pIfft_Out = MemAlloc(FFTLen*2*sizeof(S16), DDR_MEM, 0);

    col_a = 2*num_sym;
    for(i = 0; i < (S32)num_sym; i++)
    {
        isym = slotNum*num_sym + i;

        Idx1 = stepsize_a*(isym << 1);
        Idx2 = stepsize_a*(col_a << 1);

        if (i == 0)
            Ncpsam = Ncp0;
        else
            Ncpsam = Ncp;

        for(mi = 0; mi < numTxAnt; mi++)
        {
            ptr_dac_samp = &pIFFTOut[mi][count_s];

            SetupIFFT_Bufs(pIfft_In, &pRS_Grid[Idx1 + Idx2*mi], numSubCar, (S16)col_a, (S16)isym, (S16)mi, (S16)FFTLen, (S16)Ncpsam);
#ifdef DEBUG_PRINT
            {
                S32 j;

                fprintf(f_data_imod5a[mi],"%%isym = %d, mi = %d\n", isym, mi);
                for (j = 0; j < numSubCar; j++)
                {
                    fprintf(f_data_mod5a[mi],"%f %f\n", pRS_Grid[Idx1 + Idx2*mi + 2*j]/8192.0, pRS_Grid[Idx1 + Idx2*mi + 2*j +1]/8192.0);
                    fprintf(f_data_imod5a[mi],"%hd %hd\n", pRS_Grid[Idx1 + Idx2*mi + 2*j], pRS_Grid[Idx1 + Idx2*mi + 2*j +1]);
                }
            }
#endif
            ifftcg_c_16t24d(pIfft_In, fftExpo, pIfft_Out, &expo);
            AddCP(ptr_dac_samp, pIfft_Out, (S16)FFTLen, (S16)Ncpsam);
        }
        count_s += 2*(Ncpsam + FFTLen);
    }

    MemFree(pIfft_In, DDR_MEM);
    MemFree(pIfft_Out, DDR_MEM);
    MemFree(pIFFTOut, DDR_MEM);

    return;
}
#endif //_WIN32

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Downlink fftcg_c_16t24d Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTask                |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |fftcg_c_16t24d_task                |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Downlink fftcg_c_16t24d function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: fftcg_c_16t24d() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void fftcg_c_16t24d_task(PTCB pTask)
{
    S16 *addr;
    S16 m;
    PIOParams_FFT pContext;
    S16 *ifft_in, *ifft_out;

    pContext = (PIOParams_FFT) (pTask->ContextPtr);

    m = pContext->FFT_Order;
    addr = pContext->ExpAddr;

    ifft_in = (S16 *) extractTCBIOBuf(pTask, 0);
    ifft_out = (S16 *) extractTCBIOBuf(pTask, 1);

    fftcg_c_16t24d(ifft_in, m, ifft_out, addr);

    return;
}

#endif  // CEVA_INTRINSICS


////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
// Downlink Task Functions
//----------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Downlink phy_bs_tx_control Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTask                |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |phy_bs_tx_control_task             |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Downlink phy_bs_tx_control function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: phy_bs_tx_control() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void phy_bs_tx_control_task(PTCB pTask)
{
    PIOParams_DlControl pDlCntrl_TxPars = 0;
    BS_DL_INIT_MEMBUF BsDlInitMemBuf;
    PBSDLINITMEMBUF pBsDlInitMemBuf = &BsDlInitMemBuf;
    S32 numCtrlSymbols, numTxAnt;
    S16 *StartCCECtl, *NumCCECtl;
    S16* rs_grid[3*4], *rs_grid_ceva[3*4];
    S16 numInputs = 15;
    S32 isym, j, numPDCCH;
    U8 *pBuff, *pBuff_start;
    BS_DL_INIT_MEMBUF BsDlInitMemBufCeva;
    PBSDLINITMEMBUF   pBsDlInitMemBufCeva = &BsDlInitMemBufCeva;

    S16 offsetIntoBuffer, subFrameNum, indx, computeOffset;
    U8 *inputPDCCH_save, *scrSeq_save;
    U32 _size;
    U8  *locBytes;

    StartSubTask(SUBTASKID_TX_DLCONTROL_START);

    pDlCntrl_TxPars = (PIOParams_DlControl) extractTCBIOBuf(pTask, 0);
    numCtrlSymbols  = pDlCntrl_TxPars->numCtrlSymbols;
    numTxAnt = pDlCntrl_TxPars->numTxAntennas;

    if(numCtrlSymbols > 0)
    {
        pDlCntrl_TxPars->startPDCCH = (U8*) extractTCBIOBuf(pTask, 1);
        StartCCECtl = (S16*) extractTCBIOBuf(pTask, 2);
        NumCCECtl   = (S16*) extractTCBIOBuf(pTask, 3);
        pBsDlInitMemBuf->pCfiMemBuffer   = (S16*) extractTCBIOBuf(pTask, 4);
        pBsDlInitMemBuf->pCfiReMapBuffer = (S16*) extractTCBIOBuf(pTask, 5);
        pBsDlInitMemBuf->pHIMemBuffer    = (S16*) extractTCBIOBuf(pTask, 6);
        pBsDlInitMemBuf->pHIREMapTable   = (S16*) extractTCBIOBuf(pTask, 7);
        pBsDlInitMemBuf->pPDCCHPermuteTable    = (S16*) extractTCBIOBuf(pTask, 8);
        pBsDlInitMemBuf->pPDCCHQuadrupletsLen  = (S16*) extractTCBIOBuf(pTask, 9);
        pBsDlInitMemBuf->pPDCCHReMapTable      = (S16*) extractTCBIOBuf(pTask, 10);
        pBsDlInitMemBuf->pDLCellSpecificRefSeq = (S16*) extractTCBIOBuf(pTask, 11);
        pDlCntrl_TxPars->scrSeq                = (U8*)  extractTCBIOBuf(pTask, 12);
        pDlCntrl_TxPars->inputPDCCH            = (U8*)  extractTCBIOBuf(pTask, 13);
        pDlCntrl_TxPars->pdcchRNTI             = (U16*) extractTCBIOBuf(pTask, 14);

        StopAndStartSubTask(SUBTASKID_TX_DLCONTROL_EXTRACT, SUBTASKID_TX_DLCONTROL_DMA_IN);
        pBuff_start = (U8 *)(DataBuff_Bank1a);

        pBuff = pBuff_start;
        computeOffset = 0;

        /* get subframe number */
        subFrameNum = (pDlCntrl_TxPars->slotNumber >> 1);

        /* Buffers for PCFICH symbols, PHICH, PDCCH and DL Reference
         * Sequence are in the external memory - need to copy
         * them to internal memory --- how do we make sure the
         * size doesn't overrun the limits? - CHECK */
        computeOffsetToPCFICHBuffer ( (S16)numCtrlSymbols,
                                      (S16)numTxAnt,
                                      subFrameNum,
                                      &offsetIntoBuffer);

        /* update pointer to the buffer */
        pBsDlInitMemBuf->pCfiMemBuffer += offsetIntoBuffer;

        /* copy ( NUMBER_OF_CFI_BITS * numBsTxAntPort ) size
           to local memory and set pointer accordingly */
        _size = ( NUMBER_OF_CFI_BITS * numTxAnt )*sizeof(S16);
        pBsDlInitMemBufCeva->pCfiMemBuffer = (S16 *)pBuff;
        /* update pointer to working buffer */
        pBuff += _size;

        /* copy _size buffer from CRAM to local buffer */
        MemCpyFromExt((void*) pBsDlInitMemBufCeva->pCfiMemBuffer,
                      (void*) pBsDlInitMemBuf->pCfiMemBuffer, _size);

        /* copy PCFICH_INIT_RE_BUFFER_SIZE to resource element buffer */
        _size = (PCFICH_INIT_RE_BUFFER_SIZE)*sizeof(S16);
        /* local buffer pointer to resource elements */
        pBsDlInitMemBufCeva->pCfiReMapBuffer = (S16 *)pBuff;
        /* update pointer to working buffer */
        pBuff += _size;

        /* copy to local memory */
        MemCpyFromExt((void*) pBsDlInitMemBufCeva->pCfiReMapBuffer,
                      (void*) pBsDlInitMemBuf->pCfiReMapBuffer, _size);

        /* reset address */
        pBsDlInitMemBuf->pCfiMemBuffer   = pBsDlInitMemBufCeva->pCfiMemBuffer;
        pBsDlInitMemBuf->pCfiReMapBuffer = pBsDlInitMemBufCeva->pCfiReMapBuffer;

        /* Setup working buffers for PHICH Buffer */
        computeOffsetToPHICHBuffer ( (S16)numTxAnt,
                                     subFrameNum,
                                     &offsetIntoBuffer );
        /* update pointer to the buffer */
        pBsDlInitMemBuf->pHIMemBuffer += offsetIntoBuffer;

        /* copy ( NUMBER_OF_HI_BITS * numBsTxAntPort ) size
           to local memory and set pointer accordingly */
        indx  = ( NUMBER_OF_HI_BITS_NORMAL_CP * 2 * NUM_PHICH_SEQ_INDX_NORMAL_CP);
        _size = (  indx * numTxAnt )*sizeof(S16);
        pBsDlInitMemBufCeva->pHIMemBuffer = (S16 *)pBuff;
        /* update pointer to working buffer */
        pBuff += _size;

        /* copy _size buffer from CRAM to local buffer */
        MemCpyFromExt((void*) pBsDlInitMemBufCeva->pHIMemBuffer,
                      (void*) pBsDlInitMemBuf->pHIMemBuffer, _size);

        /* copy the resource element table for PHICH */
        _size = ( pDlCntrl_TxPars->N_PHICH_Grp * NUMBER_OF_HI_BITS_NORMAL_CP  )*sizeof(S16);
        pBsDlInitMemBufCeva->pHIREMapTable = (S16 *)pBuff;
        /* update pointer to working buffer */
        pBuff += _size;

        /* copy _size buffer from CRAM to local buffer */
        MemCpyFromExt((void*) pBsDlInitMemBufCeva->pHIREMapTable,
                      (void*) pBsDlInitMemBuf->pHIREMapTable, _size);

        /* reset address */
        pBsDlInitMemBuf->pHIMemBuffer  = pBsDlInitMemBufCeva->pHIMemBuffer;
        pBsDlInitMemBuf->pHIREMapTable = pBsDlInitMemBufCeva->pHIREMapTable;

        /* For PDCCH - move to local buffer - it is inefficient
           currently - can be further optimized */
        /* copy the resource element table */
        _size = (MAX_NUM_RB * N_RB_SC) * numCtrlSymbols * sizeof(S16);
        pBsDlInitMemBufCeva->pPDCCHReMapTable = (S16 *)pBuff;
        /* update pointer to working buffer */
        pBuff += _size;
        /* copy _size buffer from CRAM to local buffer */
        MemCpyFromExt((void*) pBsDlInitMemBufCeva->pPDCCHReMapTable,
                      (void*) pBsDlInitMemBuf->pPDCCHReMapTable, _size);

        /* reset the address */
        pBsDlInitMemBuf->pPDCCHReMapTable = pBsDlInitMemBufCeva->pPDCCHReMapTable;

        /* copy the PDCCH permutation table */
        /* size of the data to be copied determined by number of
         * control symbols */
        _size = pBsDlInitMemBuf->pPDCCHQuadrupletsLen[0] +
                pBsDlInitMemBuf->pPDCCHQuadrupletsLen[1] +
                pBsDlInitMemBuf->pPDCCHQuadrupletsLen[2] ;
        _size = (_size) * sizeof(S16);

        offsetIntoBuffer = 0;

        /* increment buffer */
        pBsDlInitMemBuf->pPDCCHPermuteTable += offsetIntoBuffer;

        /* set the local buffer */
        pBsDlInitMemBufCeva->pPDCCHPermuteTable = (S16 *)pBuff;
        /* update pointer to working buffer */
        pBuff += _size;
        /* copy _size buffer from CRAM to local buffer */
        MemCpyFromExt((void*) pBsDlInitMemBufCeva->pPDCCHPermuteTable,
                      (void*) pBsDlInitMemBuf->pPDCCHPermuteTable, _size);

        /* reset the address */
        pBsDlInitMemBuf->pPDCCHPermuteTable = pBsDlInitMemBufCeva->pPDCCHPermuteTable;


        /* copy the Pilot symbols to local memory */
        computeOffsetToDLRefSeqBuffer ( (S16)numTxAnt,
                                        pDlCntrl_TxPars->slotNumber,
                                        0, pDlCntrl_TxPars->Ndl_rb,
                                        &offsetIntoBuffer );

        /* increment the buffer */
        pBsDlInitMemBuf->pDLCellSpecificRefSeq += offsetIntoBuffer;

        /* set the size of the copy */
        _size = (4*pDlCntrl_TxPars->Ndl_rb)*sizeof(S16);
        /* set the local buffer */
        pBsDlInitMemBufCeva->pDLCellSpecificRefSeq = (S16 *)pBuff;
        /* update pointer to working buffer */
        pBuff += _size;
        /* copy _size buffer from CRAM to local buffer */
        MemCpyFromExt((void*) pBsDlInitMemBufCeva->pDLCellSpecificRefSeq,
                      (void*) pBsDlInitMemBuf->pDLCellSpecificRefSeq, _size);

        /* reset the address */
        pBsDlInitMemBuf->pDLCellSpecificRefSeq =
                         pBsDlInitMemBufCeva->pDLCellSpecificRefSeq;

        /* local buffer for PDCCH bytes */
        _size = MAX_PDCCH_BYTES;

        locBytes = (U8 *)pBuff;
        pBuff += _size;
        /* copy _size buffer from CRAM to local buffer */
        pDlCntrl_TxPars->startPDCCH = locBytes;

        for (isym = 0; isym < numCtrlSymbols; isym++)
        {
           for (j = 0; j < numTxAnt; j++)
           {
                rs_grid[j + isym * numTxAnt]      = (S16*) extractTCBIOBuf(pTask, numInputs++);
                rs_grid_ceva[j + isym * numTxAnt] = (S16*) pBuff;
                pBuff += RUP32B(pDlCntrl_TxPars->stepsize_a * 4);
           }
        }

        numPDCCH = pDlCntrl_TxPars->numPDCCH + pDlCntrl_TxPars->numDCI0;
        inputPDCCH_save = pDlCntrl_TxPars->inputPDCCH;
        scrSeq_save = pDlCntrl_TxPars->scrSeq;

        locBytes = (U8 *)pBuff;
        _size = numPDCCH * 128;
        pBuff += _size;
        /* copy _size buffer from CRAM to local buffer */
        MemCpyFromExt ( (void*) locBytes, (void*)pDlCntrl_TxPars->inputPDCCH, _size );
        pDlCntrl_TxPars->inputPDCCH = locBytes;

        locBytes = (U8 *)pBuff;
        _size = 800;
        pBuff += _size;
        /* copy _size buffer from CRAM to local buffer */
        MemCpyFromExt ( (void*) (&locBytes[0]), (void*)(&pDlCntrl_TxPars->scrSeq[subFrameNum*800]), _size );
        pDlCntrl_TxPars->scrSeq = locBytes;

        //Stop timer for DMA in and start sub task timer for Sdu Enc
        StopAndStartSubTask(SUBTASKID_TX_DLCONTROL_DMA_IN, SUBTASKID_TX_DLCONTROL_SDU_ENC);

        LtePhyControlSduEnc(pDlCntrl_TxPars);

        pDlCntrl_TxPars->inputPDCCH = inputPDCCH_save;
        pDlCntrl_TxPars->scrSeq = scrSeq_save;

        //Stop timer for Sdu Enc
        StopSubTask(SUBTASKID_TX_DLCONTROL_SDU_ENC);

        LteBsPhyDLControlProc(pBsDlInitMemBuf, pDlCntrl_TxPars, computeOffset,
                              StartCCECtl, NumCCECtl, NULL, rs_grid_ceva);

        //Start sub task timer for DMA Out
        StartSubTask(SUBTASKID_TX_DLCONTROL_DMA_OUT);

        for (isym = 0; isym < numCtrlSymbols; isym++)
        {
            for (j = 0; j < numTxAnt; j++)
            {
                MemCpyToExt( (void*) rs_grid[j + isym * numTxAnt], (void*) rs_grid_ceva[j + isym * numTxAnt],
                    pDlCntrl_TxPars->stepsize_a * 4);
            }
        }

        //Stop timer for DMA Out
        StopSubTask(SUBTASKID_TX_DLCONTROL_DMA_OUT);

#ifdef DEBUG_PRINT
        for(j = 0; j < numTxAnt; j++)
        {
            U16 m;
            S16 temp_R = 0, temp_I = 0;
            // Writing a column by column (symbol at a time)
            // a is stored column by column
            for (isym = 0; isym < numCtrlSymbols; isym++)
            {
                for (m = 0; m < 600; m++)
                {
                    temp_R = rs_grid[j + isym*numTxAnt][2*m];      //real
                    temp_I = rs_grid[j + isym*numTxAnt][2*m+1];    //imag
                    fprintf(f_data_mod5c_ctl_a[j],"%f %f\n", temp_R/16384.0, temp_I/16384.0);
                }
            }
        }
#endif
    }

    SetSubTaskLoggerCount(SUBTASKID_TX_DLCONTROL_START, SUBTASKID_TX_DLCONTROL_END);
    return;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Downlink tx_MultiChan_Modulation Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |tx_MultiChan_Modulation_task       |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Downlink tx_MultiChan_Modulation function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: tx_MultiChan_Modulation() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////

#if defined(EVM) && defined(TEST_CEVA_PROTECTION)
static U32 count_ceva = 0;
#endif

void tx_MultiChan_Modulation_task(PTCB pTCB)
{
    U8* pTxSysLocal;
    U8* pTxSymBufsLocal;
    U8* pTxSymBufLocal;

    TXMODULATIONPARAMS In;
    PTXMODULATIONPARAMS pIn;
    PDLTXINPUTPARAMS pTxInVars;
    S16 *syncP, *syncS;
    U32 *pContext, isym, i, j;
    S16 *pSigOutDummy[4];

    U32 lenAlTxSys   = RUP32B(sizeof(TXSYSTEMINFO));
    U32 lenTxSymBufs = RUP32B(sizeof(TXSYMBUFS));
    U32 lenTxSymBuf  = RUP32B(sizeof(TXSYMBUF)*MAXCHSUBFRAME);

    PTXSYSTEMINFO pTxSysCeva;
    U32 numVar, size1;
    U32 length_b, lenSigOut, index;
    S16 *pSigOut[4], *pSigOutCeva[4];

    StartSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_START);

    // Parse TCB
    pIn = &In;
    pContext = pTCB->ContextPtr;
    isym     = pContext[0];
    j = 0;
    pTxInVars = (DLTXINPUTPARAMS*) extractTCBIOBuf(pTCB, j++);
    pTxSysLocal = (U8*) extractTCBIOBuf(pTCB, j++);
    syncP = (S16*) extractTCBIOBuf(pTCB, j++);
    syncS = (S16*) extractTCBIOBuf(pTCB, j++);
    pTxSymBufsLocal = (U8*) extractTCBIOBuf(pTCB, j++);
    pTxSymBufLocal = (U8*) extractTCBIOBuf(pTCB, j++);

    pIn->scrIndex1 = (U32*) extractTCBIOBuf(pTCB, j++);
    pIn->scrIndex2 = (U32*) extractTCBIOBuf(pTCB, j++);
    pIn->scrOut1 = (U8*) extractTCBIOBuf(pTCB, j++);
    pIn->scrOut2 = (U8*) extractTCBIOBuf(pTCB, j++);

#if defined(EVM) && defined(TEST_CEVA_PROTECTION)
    if( (count_ceva++ % 28 == 27) && (isym == 3) )
        while(1);
#endif

    numVar = 10;
    lenSigOut = pTxInVars->NumSubCars;
    index = lenSigOut >> 1;

    // Setup Input Local Pointers
    size1 = sizeof(DLRESELEMMAPPERMULTICHANOUT);
    size1 = (size1 + 7) & (~7);

    pIn->pIn_a = (TXMAPPERMULTICHANIN *) DataBuff_Bank1;
    pIn->pOut_a = (TXMAPPERMULTICHANOUT *) DataBuff_Bank1_Off;
    pIn->pOut_b = (TXLAYERMAPPERMULTICHANOUT *) DataBuff_Bank1;
    pIn->pOut_d = (DLRESELEMMAPPERMULTICHANOUT *) DataBuff_Bank2;
    pSigOutCeva[0] = (S16*) (&DataBuff_Bank2[size1]);
    pSigOutCeva[1] = (S16*) (&pSigOutCeva[0][index]);
    pSigOutCeva[2] = (S16*) (&pSigOutCeva[1][index]);
    pSigOutCeva[3] = (S16*) (&pSigOutCeva[2][index]);
    pIn->pOut_c = (TXPRECODERMULTICHANOUT *) (&pSigOutCeva[3][index]);
    pTxSysCeva = (TXSYSTEMINFO *) DataBuff_Bank3_Off;

    StopSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_EXTRACT);

    for (i = 0; i < 2; i++)
    {
        isym += i*7;

        for (j = 0; j < pTxInVars->Ntx_antennas; j++)
            pSigOut[j] = (S16 *) extractTCBIOBuf(pTCB, numVar++);

        if((S16)isym >= pTxInVars->numCtrlSymbols)
        {
            // DMA data in
            StartSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_DMA_IN);
            length_b = sizeof(TXSYSTEMINFO);
            MemCpyFromExt((void*) pTxSysCeva, (void*) pTxSysLocal, length_b);

            for (j = 0; j < pTxInVars->Ntx_antennas; j++)
                pTxSysCeva->a.aSymbol[j] = pSigOutCeva[j];
            StopSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_DMA_IN);

            tx_MultiChan_Modulation(pTxInVars, pIn, pTxSysCeva, (PTXSYMBUFS)pTxSymBufsLocal,
                                (PTXSYMBUF)pTxSymBufLocal, pSigOutDummy, syncP, syncS, isym);

            // DMA data out
            StartSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_DMA_OUT);
            for (j = 0; j < pTxInVars->Ntx_antennas; j++)
            {
                MemCpyToExt((void *) pSigOut[j], (void *) pSigOutCeva[j], lenSigOut);
            }
            StopSubTask(SUBTASKID_TX_MULTICHAN_MODULATION_DMA_OUT);
        }

        pTxSysLocal     += lenAlTxSys * 7;
        pTxSymBufsLocal += lenTxSymBufs * 7;
        pTxSymBufLocal  += lenTxSymBuf * 7;
    }

    SetSubTaskLoggerCount(SUBTASKID_TX_MULTICHAN_MODULATION_START, SUBTASKID_TX_MULTICHAN_MODULATION_END);
    return;
}



////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Downlink SetupSymbDataInpBufs Task Wrap Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |SetupSymbDataInpBufs_task_wrap     |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function is needed to call SetupSymbDataInpBufs from REX without invoking a TaskList.
 *  ARM is able to directly call SetupSymbDataInpBufs_task which will then call SetupSymbDataInpBufs
 *  to run on the Ceva.
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: SetupSymbDataInpBufs() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
U32 SetupSymbDataInpBufs_task_wrap(U32 pTCB)
{
    if(pTCB != 0)
        SetupSymbDataInpBufs_task((PTCB)pTCB);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Downlink SetupSymbDataInpBufs Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |SetupSymbDataInpBufs_task          |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Downlink SetupSymbDataInpBufs function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: SetupSymbDataInpBufs() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void SetupSymbDataInpBufs_task(PTCB pTCB)
{
    PTXSYMBUFS pTxSymBufs;
    PTXSYMBUF pTxSymBuf;
    U32 isymstart;
    U32 isymstop;
    U32 ichanStart;
    U32 numChan;
    U16 Ntx_antennas;
    DLTXINPUTPARAMS *pTxInPars;

    PTXSYMBUFS pTxSymBufsCeva = NULL;
    PTXSYMBUF pTxSymBufCeva = NULL;

    U8 *pCeva;

    StartSubTask(SUBTASKID_TX_SETUP_SYMB_BUFS_START);

    pTxSymBufs = (PTXSYMBUFS) extractTCBIOBuf(pTCB, 0);
    pTxSymBuf  = (PTXSYMBUF) extractTCBIOBuf(pTCB, 1);
    pTxInPars  = (PDLTXINPUTPARAMS) extractTCBIOBuf(pTCB, 2);

    isymstart = pTxInPars->numCtrlSymbols;
    isymstop = pTxInPars->Ndl_symb << 1;
    ichanStart = pTxInPars->numCtrlChan;
    numChan = pTxInPars->numChan;
    Ntx_antennas = pTxInPars->Ntx_antennas;

    pCeva = (U8*) DataBuff_Bank1;
    pTxSymBufsCeva = (PTXSYMBUFS)pCeva;
    pCeva += RUP32B(sizeof(TXSYMBUFS));
    pTxSymBufCeva = (PTXSYMBUF)pCeva;

    StopSubTask(SUBTASKID_TX_SETUP_SYMB_BUFS_EXTRACT);

    SetupSymbDataInpBufs(isymstart, isymstop, ichanStart, numChan, Ntx_antennas,
                                pTxSymBufs, pTxSymBuf, pTxSymBufsCeva, pTxSymBufCeva, pTxInPars);

    SetSubTaskLoggerCount(SUBTASKID_TX_SETUP_SYMB_BUFS_START, SUBTASKID_TX_SETUP_SYMB_BUFS_END);

    return;
}


////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
// Uplink Task Functions in Ceva
//----------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// PUSCH Tasks
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink Pilots Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |ul_Pilot_task                      |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink Pilots function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: ul_Pilot_fx() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void ul_Pilot_task(PTCB pTCB)
{
    U32* pContext = 0;
    S16 L, K;

    S16 slotNum;
    S32 i, offset_pusch, numPusch, Msc;
    S32 length;

    S16 *a;
    ULRXINPUTPARAMS *pULPilotInput;
    U8 *cvec_u, *PRSbits;
    S16 *bitRevTable, *pUlPilots_QthRootTable;
    U32 *rs_grid;
    U32 stepsize_aDet;
    S16 *ceva_ac;
    S16 *u, *v, *nPRSCv;
    U8  *cvec_uCv, *PRSbitsCv;
    S16 *bitRevCv;
    S32 index;

    StartSubTask(SUBTASKID_RX_ULPILOT_START);

    pContext = (U32*) pTCB->ContextPtr;
    stepsize_aDet = pContext[0];

    a =             (S16 *) extractTCBIOBuf(pTCB, 0);
    pULPilotInput = (ULRXINPUTPARAMS *) extractTCBIOBuf(pTCB, 1);
    cvec_u =        (U8 *) extractTCBIOBuf(pTCB, 2);
    PRSbits =       (U8 *) extractTCBIOBuf(pTCB, 3);
    bitRevTable =   (S16 *) extractTCBIOBuf(pTCB, 4);
    pUlPilots_QthRootTable = (S16 *) extractTCBIOBuf(pTCB, 5);
    rs_grid =       (U32 *) extractTCBIOBuf(pTCB, 6);

    L  = pULPilotInput->L;
    K = pULPilotInput->K;
    length = 4 * K;

    ceva_ac   = (S16*) (DataBuff_Bank1a);
    index     = RUP8B(length);
    u         = (S16*) (&ceva_ac[index]);
    index     = RUP8B(NUM_SLOTS);
    v         = (S16*) (&u[index]);
    nPRSCv    = (S16*) (&v[index]);
    cvec_uCv  = (U8*) (&nPRSCv[index]);
    index     = RUP8B(7*NUM_SLOTS);
    PRSbitsCv = (U8*) (&cvec_uCv[index]);
    bitRevCv  = (S16*) (&PRSbitsCv[index]);

    StopAndStartSubTask(SUBTASKID_RX_ULPILOT_EXTRACT, SUBTASKID_RX_ULPILOT_DMA_IN);

    //Copy from CRAM to internal Ceva Memory
    if (pULPilotInput->groupHoppingEnabled)
        MemCpyFromExt((void*) cvec_uCv, (void*) cvec_u, 7 * NUM_SLOTS);

    MemCpyFromExt((void*) PRSbitsCv, (void*) PRSbits, 7 * NUM_SLOTS);
    MemCpyFromExt((void*) bitRevCv, (void*) bitRevTable, 256 * 2);

    StopSubTask(SUBTASKID_RX_ULPILOT_DMA_IN);

    ////////////////////////////////////////////////////////////////
    numPusch  = pULPilotInput->numPUSCH;
    slotNum   = pULPilotInput->slotNum;
    ////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    //Generate local copy of Uplink Pilots for all PUSCH
    //////////////////////////////////////////////////////////////////////////////
    offset_pusch = 0;
    for(i=0; i < numPusch; i++)
    {
        StartSubTask(SUBTASKID_RX_ULPILOT_CONFIG_PILOTS);
        Msc = pULPilotInput->Mpusch_sc[i];
        ul_ConfigPilots_Pusch(pULPilotInput, cvec_uCv, u, PRSbitsCv, v, nPRSCv,
                              bitRevCv, i);

        StopAndStartSubTask(SUBTASKID_RX_ULPILOT_CONFIG_PILOTS, SUBTASKID_RX_ULPILOT_MAIN);
        ul_Pilot_fx(&ceva_ac[offset_pusch], nPRSCv + slotNum, pULPilotInput,
                    u + slotNum, v + slotNum, pUlPilots_QthRootTable, i);

        offset_pusch += 4 * Msc;

        StopAndStartSubTask(SUBTASKID_RX_ULPILOT_MAIN, SUBTASKID_RX_ULPILOT_DEMUX);
        //////////////////////////////////////////////////////////////////////////////
        //Find where each user is located in resource grid for PUSCH
        //////////////////////////////////////////////////////////////////////////////
        bs_rx_DemuxFDMAPhysChan_fx(pULPilotInput, stepsize_aDet, &rs_grid[i*L], i);
        //////////////////////////////////////////////////////////////////////////////
        StopSubTask(SUBTASKID_RX_ULPILOT_DEMUX);
    }
    //////////////////////////////////////////////////////////////////////////////

    //Copy back to CRAM
    StartSubTask(SUBTASKID_RX_ULPILOT_DMA_OUT);
    MemCpyToExt((void *) a, (void *) ceva_ac, 2*offset_pusch);
    StopSubTask(SUBTASKID_RX_ULPILOT_DMA_OUT);

    SetSubTaskLoggerCount(SUBTASKID_RX_ULPILOT_START, SUBTASKID_RX_ULPILOT_END);
    return;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink bs_rx_ChEstimator_part0_fx Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |bs_rx_ChEstimator_part0_fx_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink bs_rx_ChEstimator_part0_fx function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: bs_rx_ChEstimator_part0_fx() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void bs_rx_ChEstimator_part0_fx_task(PTCB pTCB)
{
    U32* pContext = 0;
    S32 i = 0, numPusch;
    S32 LengthInOut, LengthInOut1;
    S16 MO, K, Nrb_sc;

    S32 Msc;
    S32 pilotInd = 0, chanEstInd = 0;
    S32 gridInd = 0,  ErrInd = 0;
    S32 pilotSymbol;

    S16 antenna, slot;
    S32 stepsize_aDet;

    S16 *adet, *aPilots;
    S16 *chEst, *pScaleComputeErr_Tab, *expo_mat;
    //U32 *rs_grid;
    S32 *Err;
    ULRXINPUTPARAMS *pChanEstPar;

    S16 *aDetCeva, *aPilotsCeva;
    S16 *chEstCeva;

    StartSubTask(SUBTASKID_RX_CHANEST_P0_START);

    pContext      = (U32*) pTCB->ContextPtr;
    stepsize_aDet = (S32) pContext[0];
    antenna       = (S16) pContext[1];
    slot          = (S16) pContext[2];

    adet                 = (S16*) extractTCBIOBuf(pTCB, 0);
    //rs_grid              = (U32*) extractTCBIOBuf(pTCB, 1);
    aPilots              = (S16*) extractTCBIOBuf(pTCB, 2);
    pChanEstPar          = (ULRXINPUTPARAMS*) extractTCBIOBuf(pTCB, 3);
    chEst                = (S16*) extractTCBIOBuf(pTCB, 4);
    pScaleComputeErr_Tab = (S16*) extractTCBIOBuf(pTCB, 5);
    Err                  = (S32*) extractTCBIOBuf(pTCB, 6);
    expo_mat             = (S16*) extractTCBIOBuf(pTCB, 7);

    StopAndStartSubTask(SUBTASKID_RX_CHANEST_P0_EXTRACT, SUBTASKID_RX_CHANEST_P0_DMA_IN);

    K  = pChanEstPar->K;
    MO = pChanEstPar->MO;
    numPusch    = pChanEstPar->numPUSCH;
    pilotSymbol = pChanEstPar->pilotCarrierSymbol;
    Nrb_sc      = pChanEstPar->Nrb_sc;
    ErrInd      = 2*slot + 4*antenna;

    // Allocate Memory
    LengthInOut = 4 * K * sizeof(S16);
    LengthInOut1 = 2 * K * sizeof(S16);

    aDetCeva    = (S16 *) &DataBuff_Bank1[0];
    aPilotsCeva = &(aDetCeva[2 * K]);
    chEstCeva   = &(aPilotsCeva[4 * K]);

    gridInd = (pilotSymbol + pChanEstPar->Nul_symb*slot)*stepsize_aDet;
    MemCpyFromExt((void*) aDetCeva, (void*) &adet[gridInd], LengthInOut1);
    MemCpyFromExt((void*) aPilotsCeva, (void*) aPilots, LengthInOut);

    StopSubTask(SUBTASKID_RX_CHANEST_P0_DMA_IN);

    expo_mat[slot] = adet[2*K + (pilotSymbol + slot*pChanEstPar->Nul_symb)*stepsize_aDet];
    for(i = 0; i < numPusch; i++)
    {
        StartSubTask(SUBTASKID_RX_CHANEST_P0_MAIN);
        Msc     = pChanEstPar->Mpusch_sc[i];
        gridInd = pChanEstPar->RBStart[i]*2*Nrb_sc;

        bs_rx_ChEstimator_part0_fx(&aDetCeva[gridInd], &aPilotsCeva[pilotInd + 2*Msc*slot],
                                   pChanEstPar, &chEstCeva[chanEstInd],
                                   pScaleComputeErr_Tab, (S32*)&Err[ErrInd],
                                   antenna, i);

        pilotInd += 4*Msc;
        chanEstInd += 2*Msc;
        ErrInd += 2*4*MO;
        StopSubTask(SUBTASKID_RX_CHANEST_P0_MAIN);
    }

    StartSubTask(SUBTASKID_RX_CHANEST_P0_DMA_OUT);
    chanEstInd = 0;
    for(i=0; i < numPusch; i++)
    {
        Msc = pChanEstPar->Mpusch_sc[i];
        MemCpyToExt((void *) &chEst[2*chanEstInd + 2*Msc*slot], (void *) &chEstCeva[chanEstInd], Msc*4);
        chanEstInd += 2*Msc;
    }
    StopSubTask(SUBTASKID_RX_CHANEST_P0_DMA_OUT);

    SetSubTaskLoggerCount(SUBTASKID_RX_CHANEST_P0_START, SUBTASKID_RX_CHANEST_P0_END);
    return;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink bs_rx_ChEstimator_part1_fx Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |bs_rx_ChEstimator_part1_fx_task    |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink bs_rx_ChEstimator_part1_fx function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: bs_rx_ChEstimator_part1_fx() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void bs_rx_ChEstimator_part1_fx_task(PTCB pTCB)
{
    U32* pContext = 0;
    S32 i = 0, numPusch;

    S16 MO, K;

    S32 Msc;
    S32 chanEstInd = 0;
    S32 SnrInd = 0, ErrInd = 0, NoiseInd = 0,  rbPwrInd = 0, rssiInd = 0;
    S32 derotInd = 0;

    S16 antenna, slot;
    S16 *derotation;
    S16 *chEst;
    U32  *Err, *snrPerRb_Pilot, *snrPilot;
    S32 *noiseVar;
    S32 *rbPwrAvg_AntSlot, *rssi_AntSlot;
    U8 *rbUsedFlag;
    ULRXINPUTPARAMS *pChanEstPar;

    S16 *chEstCeva, *derotationCeva;
    S16 *VecChEstCeva;
    S16 *mErrExpoTemp;
    S32 *mErrTemp;

    StartSubTask(SUBTASKID_RX_CHANEST_P1_START);

    pContext      = (U32*) pTCB->ContextPtr;
    antenna       = (S16) pContext[1];
    slot          = (S16) pContext[2];

    derotation           = (S16*) EXTRACTTCBIOBUF(pTCB, 0);
    pChanEstPar          = (ULRXINPUTPARAMS*) EXTRACTTCBIOBUF(pTCB, 1);
    chEst                = (S16*) EXTRACTTCBIOBUF(pTCB, 2);
    Err                  = (U32*) EXTRACTTCBIOBUF(pTCB, 3);
    snrPerRb_Pilot       = (U32*) EXTRACTTCBIOBUF(pTCB, 4);
    snrPilot             = (U32*) EXTRACTTCBIOBUF(pTCB, 5);
    noiseVar             = (S32*) EXTRACTTCBIOBUF(pTCB, 6);
    mErrExpoTemp         = (S16 *) EXTRACTTCBIOBUF(pTCB, 7);
    mErrTemp             = (S32 *) EXTRACTTCBIOBUF(pTCB, 8);
    rbPwrAvg_AntSlot     = (S32 *) EXTRACTTCBIOBUF(pTCB, 9);
    rbUsedFlag           = (U8 *) EXTRACTTCBIOBUF(pTCB, 10);
    rssi_AntSlot         = (S32 *) EXTRACTTCBIOBUF(pTCB, 11);

    K = pChanEstPar->K;
    MO = pChanEstPar->MO;
    numPusch = pChanEstPar->numPUSCH;

    SnrInd = slot + 2*antenna;
    NoiseInd = slot + 2*antenna;
    rbPwrInd = slot*MAXRESOURCEBLOCKS + 2*antenna*MAXRESOURCEBLOCKS;

    VecChEstCeva = (S16 *) &DataBuff_Bank1[0];
    derotationCeva = &(VecChEstCeva[4 * K]);
    chEstCeva = &(derotationCeva[2 * K]);

    StopSubTask(SUBTASKID_RX_CHANEST_P1_EXTRACT);

    for(i=0; i < numPusch; i++)
    {
        Msc = pChanEstPar->Mpusch_sc[i];

        StartSubTask(SUBTASKID_RX_CHANEST_P1_DMA_IN);
        MemCpyFromExt((void*) &chEstCeva[chanEstInd], (void*) &chEst[2*chanEstInd + 2*Msc*slot], Msc*4);
        StopSubTask(SUBTASKID_RX_CHANEST_P1_DMA_IN);

        bs_rx_ChEstimator_part1_fx(VecChEstCeva, &derotationCeva[derotInd], pChanEstPar,
                &chEstCeva[chanEstInd],  (S32*)&Err[ErrInd],
                antenna, (S32*)snrPerRb_Pilot, (S32*)&snrPilot[SnrInd], &noiseVar[NoiseInd], i,
                mErrExpoTemp[i],  &mErrTemp[2*i],  &rbPwrAvg_AntSlot[rbPwrInd], &rbUsedFlag[rbPwrInd], &rssi_AntSlot[rssiInd]);

        derotInd += 2*Msc;
        chanEstInd += 2*Msc;
        ErrInd += 2;
        SnrInd += 4*MO;
        NoiseInd += 2*MO;
        rssiInd += 2*MO;
    }

    StartSubTask(SUBTASKID_RX_CHANEST_P1_DMA_OUT);
    chanEstInd = 0;
    for(i=0; i < numPusch; i++)
    {
        Msc = pChanEstPar->Mpusch_sc[i];
        MemCpyToExt((void *) &derotation[chanEstInd], (void *) &derotationCeva[chanEstInd], Msc*4);
        MemCpyToExt((void *) &chEst[2*chanEstInd + 2*Msc*slot], (void *) &chEstCeva[chanEstInd], Msc*4);
        chanEstInd += 2*Msc;
    }

    StopSubTask(SUBTASKID_RX_CHANEST_P1_DMA_OUT);
    SetSubTaskLoggerCount(SUBTASKID_RX_CHANEST_P1_START, SUBTASKID_RX_CHANEST_P1_END);

    return;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink RunningAverage Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |RunningAverage_task                |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink RunningAverage_fx function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: RunningAverage_fx() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void RunningAverage_task(PTCB pTCB)
{
    ULRXINPUTPARAMS *pRxInParams;
    S32 *ErrAvgIn, *ErrAvgOut;
    S16 *ErrExpoPusch, *ErrExpo;
    S16 Ns;
    S16 *mErrExpoTemp;
    S32 *mErrTemp;

    U32* pContext = 0;
    S32 i, numChan, MO;

    StartSubTask(SUBTASKID_RX_RUNNING_AVG_START);

    pContext = (U32*) pTCB->ContextPtr;
    Ns = (S16) pContext[0];

    pRxInParams  = (ULRXINPUTPARAMS *) extractTCBIOBuf(pTCB, 0);
    ErrAvgIn       = (S32 *) extractTCBIOBuf(pTCB, 1);
    ErrExpoPusch = (S16 *) extractTCBIOBuf(pTCB, 2);
    ErrExpo = (S16 *) extractTCBIOBuf(pTCB, 3);
    ErrAvgOut       = (S32 *) extractTCBIOBuf(pTCB, 4);
    mErrExpoTemp = (S16 *) extractTCBIOBuf(pTCB, 5);
    mErrTemp      = (S32 *) extractTCBIOBuf(pTCB, 6);

    StopSubTask(SUBTASKID_RX_RUNNING_AVG_EXTRACT);

    numChan = pRxInParams->numChan;
    MO = pRxInParams->MO;

    for(i=0; i < numChan; i++)
    {
        RunningAverage0(pRxInParams, &ErrAvgIn[8*MO*i], ErrExpoPusch, ErrExpo,
                        &ErrAvgOut[2*i], &mErrExpoTemp[i],  &mErrTemp[2*i], Ns, i);
    }

    SetSubTaskLoggerCount(SUBTASKID_RX_RUNNING_AVG_START, SUBTASKID_RX_RUNNING_AVG_END);
    return;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink RunningAveragePUCCH Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |RunningAveragePUCCH_task                |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink RunningAveragePUCCH_fx function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: RunningAveragePUCCH_fx() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void RunningAveragePUCCH_task(PTCB pTCB)
{
    ULRXINPUTPARAMS *pRxInParams;
    S32 *ErrAvgIn, *ErrAvgOut, *ErrSub;
    S16 *ErrExpoPucch, *ErrSubExpo;
    S16  MO;
    S32 i, numPucch;
    S16 max_expo_vec[NUM_SLOTS_SUBFRM*MAX_NUM_ANT], max_expo;
    S16 numRxAnt;
    S16 Nul_symb;
    S16 j, k, l;
    S32 indPucch1, indPucch2;

    pRxInParams  = (ULRXINPUTPARAMS *) extractTCBIOBuf(pTCB, 0);
    ErrAvgIn     = (S32 *) extractTCBIOBuf(pTCB, 1);
    ErrExpoPucch = (S16 *) extractTCBIOBuf(pTCB, 2);
    ErrAvgOut    = (S32 *) extractTCBIOBuf(pTCB, 3);
    ErrSub       = (S32 *) extractTCBIOBuf(pTCB, 4);
    ErrSubExpo   = (S16 *) extractTCBIOBuf(pTCB, 5);

    numPucch = pRxInParams->numPUCCH;
    MO = pRxInParams->MO;
    numRxAnt = pRxInParams->MO;
    Nul_symb = pRxInParams->Nul_symb;

    for (k = 0; k < NUM_SLOTS_SUBFRM*MAX_NUM_ANT; k++)
        max_expo_vec[k] =  -32768;
    max_expo = -32768;

    indPucch1 = pRxInParams->numPUSCH;
    indPucch2 = pRxInParams->numPUSCH + numPucch;

     //Find Max_expo per slot/per antenna
     for (k = 0; k < numRxAnt; k++)
     {
         for (j = 0; j < NUM_SLOTS_SUBFRM; j++)
         {
            for (l = 0; l < Nul_symb; l++)
            {
                if (max_expo_vec[k * NUM_SLOTS_SUBFRM + j] < ErrExpoPucch[k*Nul_symb*NUM_SLOTS_SUBFRM + j * Nul_symb + l])
                {
                    max_expo_vec[k * NUM_SLOTS_SUBFRM + j] = ErrExpoPucch[k*Nul_symb*NUM_SLOTS_SUBFRM + j * Nul_symb + l];
                }
            }
         }
     }

    //Find global Max_expo
    for (k = 0; k < numRxAnt; k++)
    {
        for (j = 0; j < NUM_SLOTS_SUBFRM; j++)
        {
             if (max_expo < max_expo_vec[k*NUM_SLOTS_SUBFRM + j])
                 max_expo = max_expo_vec[k*NUM_SLOTS_SUBFRM + j];
        }
    }

    for(i = indPucch1; i < indPucch2; i++)
    {
        RunningAveragePUCCH(pRxInParams, &ErrAvgIn[8*MO*i], &ErrAvgOut[4*i], i, max_expo_vec,
                            max_expo, &ErrSub[2*2*NUM_SLOTS_SUBFRM*MO*i],
                            &ErrSubExpo[2*NUM_SLOTS_SUBFRM*MO*i]);

        if(pRxInParams->simSRHarq[i] == 1)
        {
            RunningAveragePUCCH(pRxInParams, &ErrAvgIn[8*MO*i + 4*MO], &ErrAvgOut[4*i + 2], i,
                                max_expo_vec, max_expo,
                                &ErrSub[2*2*NUM_SLOTS_SUBFRM*MO*i + 2*NUM_SLOTS_PER_SUBF*MO],
                                &ErrSubExpo[2*2*NUM_SLOTS_SUBFRM*MO*i + NUM_SLOTS_PER_SUBF*MO]);
        }
    }

    return;
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink SnrSubframe Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |SnrSubframe_task                   |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink SnrSubframe function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: SnrSubframe() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void SnrSubframe_task(PTCB pTCB)
{
    S32 i, numChan, MO;
    U32 L1, L2;

    ULRXINPUTPARAMS *pRxInPar;
    S32 *snrPerRb, *snrPerRbCeva, *snrPilot, *snrPilotCeva;
#if !defined(EVM) && !defined(_WIN32)
    USED_INDEX_VARS *pTempStore[MAXCHSUBFRAME];
#endif

    StartSubTask(SUBTASKID_RX_SNRSUBFRAME_START);

    pRxInPar = (ULRXINPUTPARAMS *) extractTCBIOBuf(pTCB, 0);
    snrPerRb = (S32 *) extractTCBIOBuf(pTCB, 1);
    snrPilot = (S32 *) extractTCBIOBuf(pTCB, 2);

    MO = pRxInPar->MO;
    numChan = pRxInPar->numChan;

#if !defined(EVM) && !defined(_WIN32)
    //Need to extract these for Ceva Sim
    for(i = 0; i < numChan; i++)
    {
        pTempStore[i] = pRxInPar->usedIndexVars[i];
        pRxInPar->usedIndexVars[i] = (USED_INDEX_VARS * )extractTCBIOBuf(pTCB, (i+3));
    }
#endif

    StopAndStartSubTask(SUBTASKID_RX_SNRSUBFRAME_EXTRACT, SUBTASKID_RX_SNRSUBFRAME_DMA_IN);

    L1 = 2 * 100 * pRxInPar->MO;
    L2 = 4 * pRxInPar->MO * numChan * sizeof(S32);

    snrPerRbCeva = (S32 *) &DataBuff_Bank1[0];
    snrPilotCeva = (S32 *) &snrPerRbCeva[L1];

    MemCpyFromExt((void*) snrPerRbCeva, (void*) snrPerRb, L1 * sizeof(S32));
    MemCpyFromExt((void*) snrPilotCeva, (void*) snrPilot, L2);

    StopSubTask(SUBTASKID_RX_SNRSUBFRAME_DMA_IN);

    for(i = 0; i < numChan; i++)
        SnrSubframe(pRxInPar, snrPerRbCeva, &snrPilotCeva[4*MO*i], i);

#if !defined(EVM) && !defined(_WIN32)
    //Store back for Ceva Sim
    for(i = 0; i < numChan; i++)
    {
        pRxInPar->usedIndexVars[i] = pTempStore[i];
    }
#endif

    SetSubTaskLoggerCount(SUBTASKID_RX_SNRSUBFRAME_START, SUBTASKID_RX_SNRSUBFRAME_END);
    return;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink RipRssiSubframe_task Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |RipRssiSubframe_task            |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink RipRssiSubframe function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: RipRssiSubframe() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////

void RipRssiSubframe_task(PTCB pTCB)
{
    U32* pContext = 0;
    S32 i;

    ULRXINPUTPARAMS *pRxInPar;
    S32 step_aDet, numChan;
    S16 srsPilot, *InitFlag;
    S16 *aDet, *aDetCeva;
    S32 *rxInRbPwrAvg_AntSlot, *rxInRIP;
    U8 *rbUsedFlag;
    S16 max_expo_AntSlot[8];
    S16 *expo_mat;
    S32 *rssi_AntSlot;
    U32 rssiChanMeasEn = 0;

    StartSubTask(SUBTASKID_RX_RIPRSSISUBFRAME_START);

    pContext  = (U32*)pTCB->ContextPtr;
    step_aDet = (S32) pContext[0];
    srsPilot  = (S16) pContext[1];



    pRxInPar             = (ULRXINPUTPARAMS *) extractTCBIOBuf(pTCB, 0);
    aDet                 = (S16 *) extractTCBIOBuf(pTCB, 1);
    rxInRbPwrAvg_AntSlot = (S32 *) extractTCBIOBuf(pTCB, 2);
    rbUsedFlag           = (U8 *) extractTCBIOBuf(pTCB, 3);
    rxInRIP              = (S32 *) extractTCBIOBuf(pTCB, 4);
    InitFlag             = (S16 *) extractTCBIOBuf(pTCB, 5);
    expo_mat             = (S16 *) extractTCBIOBuf(pTCB, 6);
    rssi_AntSlot         = (S32 *) extractTCBIOBuf(pTCB, 7);

    numChan = pRxInPar->numChan;

    for(i = 0; i < numChan; i++)
    {
        if (pRxInPar->rssiChanMeasFlag[i])
        {
            rssiChanMeasEn = 1;
            break;
        }
    }

    StopAndStartSubTask(SUBTASKID_RX_RIPRSSISUBFRAME_EXTRACT, SUBTASKID_RX_RIPRSSISUBFRAME_DMA_IN);

    aDetCeva = (S16 *) &DataBuff_Bank1[0];
    MemCpyFromExt((void*) aDetCeva, (void*) aDet, pRxInPar->MO * step_aDet * pRxInPar->L * sizeof(S16));

    StopAndStartSubTask(SUBTASKID_RX_RIPRSSISUBFRAME_DMA_IN, SUBTASKID_RX_RIPRSSISUBFRAME_RB_PWR_AVG);

    if(pRxInPar->rxIntMeasFlag + rssiChanMeasEn)
       bs_rx_RbPwrAvg(pRxInPar, aDetCeva, rxInRbPwrAvg_AntSlot, rbUsedFlag, step_aDet, srsPilot, max_expo_AntSlot);

    StopAndStartSubTask(SUBTASKID_RX_RIPRSSISUBFRAME_RB_PWR_AVG, SUBTASKID_RX_RIPRSSISUBFRAME_RIP_SUBFRAME);

    if(pRxInPar->rxIntMeasFlag)
        RipSubframe(rxInRbPwrAvg_AntSlot, max_expo_AntSlot, rxInRIP, &pRxInPar->RipPerRB_dB[0],
                    pRxInPar->MO, pRxInPar->Nul_rb, pRxInPar->Nfft, InitFlag);

    StopAndStartSubTask(SUBTASKID_RX_RIPRSSISUBFRAME_RIP_SUBFRAME, SUBTASKID_RX_RIPRSSISUBFRAME_THERMAL_NOISE);

    if(pRxInPar->rxTherNoiseFlag)
        ComputeThermalNoise(pRxInPar->unusedRB, &pRxInPar->Temperature[0], pRxInPar->MO,  &pRxInPar->ThermalNoisePwr);

    StopAndStartSubTask(SUBTASKID_RX_RIPRSSISUBFRAME_THERMAL_NOISE, SUBTASKID_RX_RIPRSSISUBFRAME_RSSI_SUBFRAME);

    for(i = 0; i < numChan; i++)
        if (pRxInPar->rssiChanMeasFlag[i])
            RssiSubframe(pRxInPar, rssi_AntSlot, expo_mat, max_expo_AntSlot, i);

    StopSubTask(SUBTASKID_RX_RIPRSSISUBFRAME_RSSI_SUBFRAME);

    SetSubTaskLoggerCount(SUBTASKID_RX_RIPRSSISUBFRAME_START, SUBTASKID_RX_RIPRSSISUBFRAME_END);
    return;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink rx_Multichan_DeModulation Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |rx_Multichan_DeModulation_task     |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink rx_Multichan_DeModulation function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: rx_Multichan_DeModulation() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void rx_Multichan_DeModulation_task(PTCB pTCB)
{
    S32 i;
    U32 *pContext = 0;
    S32 numPusch, L, MO, K;

    S32 j, k, mo, Msc;
    S16 *RXINCeva, *ChEstCeva, *chEst_pilotCeva, *reDet0Ceva;
    S16 expo_norm, expo_pilot[NUM_SLOTS_PER_SUBF * MAX_NUM_ANT];
    S16 *expo_reEst1Ceva, *derotCeva, Nul_symb, MI, Qm;
    S32 *RXOUTCeva, *CHOUTCeva, ChanEstInd = 0;
    S32 ChanEstInd2 = 0;
    S32 derotInd = 0, expoInd;
    S32 offset_pusch = 0;

    S32 stepsize_aDet, isym, srsPilot, length_b;
    S16 *aDet[RXANT_FOUR], *chEst_ul, *derotation;
    S16 expo_mat[RXANT_FOUR];
    S16 *reDet0, *expo_reEst_norm;
    U32 *rs_grid;
    ULRXINPUTPARAMS *pRxInParams;
    S32 *noiseVar, pilotSym, reDet_isym, step_reDet;

    StartSubTask(SUBTASKID_RX_MULTICHAN_DEMODULATION_START);

    pContext = pTCB->ContextPtr;

    isym          = pContext[0];
    stepsize_aDet = pContext[1];
    MO            = pContext[2];
    srsPilot      = pContext[3];
    step_reDet    = pContext[4];

    j = 0;
    for (i = 0; i < MO; i++)
        aDet[i]     = (S16 *) extractTCBIOBuf(pTCB, j++);

    rs_grid         = (U32 *) extractTCBIOBuf(pTCB, j++);
    chEst_ul        = (S16 *) extractTCBIOBuf(pTCB, j++);
    derotation      = (S16 *) extractTCBIOBuf(pTCB, j++);
    reDet0          = (S16 *) extractTCBIOBuf(pTCB, j++);
    expo_reEst_norm = (S16 *) extractTCBIOBuf(pTCB, j++);
    pRxInParams     = (ULRXINPUTPARAMS *) extractTCBIOBuf(pTCB, j++);
    noiseVar        = (S32 *) extractTCBIOBuf(pTCB, j++);

    // Multichan Demodulation Task to consolidate all Ceva TCBs into 1 Single TCB
    numPusch = pRxInParams->numPUSCH;
    K = pRxInParams->K;
    L = pRxInParams->L;

    MI = pRxInParams->MI;
    Nul_symb = pRxInParams->Nul_symb;
    pilotSym = pRxInParams->pilotCarrierSymbol;

    // Setup Ceva Internal Memory pointers
    RXINCeva  = (S16 *) &(DataBuff_Bank1[0]);                                   //ChEst_p3 Input
    chEst_pilotCeva = (S16 *) &(RXINCeva[RXANT_FOUR * MAXSUBCARRIERS * 2]);     //ChEst_p4 Input
    derotCeva = (S16 *) &(chEst_pilotCeva[RXANT_FOUR * MAXSUBCARRIERS * 4]);    //ChEst_p3 Input
    RXOUTCeva = (S32 *) (chEst_pilotCeva);                                      //MRC Output
    ChEstCeva = (S16 *) &(derotCeva[MAXSUBCARRIERS * 2]);                       //ChEst_p4 Output

    i = 2 * K;
    i = (i + 3) & ~3; // In 32-bit words
    CHOUTCeva = (S32 *) &(RXOUTCeva[i]);        //MRC Output
    expo_reEst1Ceva = (S16 *) &(CHOUTCeva[K]);  //FEQ temp buffer

    reDet0Ceva = (S16 *) (DataBuff_Bank3a);     //Final Output

    StopSubTask(SUBTASKID_RX_MULTICHAN_DEMODULATION_EXTRACT);
    for(mo = 0; mo < MO; mo++)
    {
        expo_pilot[2*mo]     = aDet[mo][2*K + pilotSym*stepsize_aDet];
        expo_pilot[2*mo + 1] = aDet[mo][2*K + (pilotSym + Nul_symb)*stepsize_aDet];
    }

    for(k = 0; k < NUM_SLOTS_SUBFRM; k++)
    {
        //Find symbol in 1st or 2nd slot
        isym += (S16)(k * Nul_symb);
        reDet_isym = isym;

        //Check if we have crossed a pilot symbol
        if (reDet_isym >= pilotSym)
            reDet_isym--;
        if (reDet_isym >= (pilotSym + Nul_symb - 1))
            reDet_isym--;

        //Exit if we are at SRS pilot symbol
        if (isym == srsPilot)
            break;

        //Reset all Indexes for symbol in next slot
        ChanEstInd   = 0;
        ChanEstInd2  = 0;
        expoInd      = 0;
        derotInd     = 0;

        //Find offset into reDet
        offset_pusch = 0;

        expo_norm = -64;
        for(mo = 0; mo < MO; mo++)
        {
            expo_mat[mo] = aDet[mo][2*K + isym*stepsize_aDet];
            if (expo_norm < expo_mat[mo])
                expo_norm = expo_mat[mo];
        }
        for(i = 0; i < numPusch; i++)
        {
            Msc = pRxInParams->Mpusch_sc[i];
            Qm  = pRxInParams->Qm[i];
            length_b = 2 * Msc * sizeof(S16);
            StartSubTask(SUBTASKID_RX_MULTICHAN_DEMODULATION_DMA_IN);
            for(mo = 0; mo < MO; mo++)
            {
                ChanEstInd  = mo * MI * 2 * K * 2;      //Index for ChanEst

                MemCpyFromExt((void *)(&RXINCeva[mo * Msc * 2]),
                              (void *)(&aDet[mo][rs_grid[isym + L*i]]), length_b);
                MemCpyFromExt((void *)(&chEst_pilotCeva[mo * Msc * 4]),
                              (void *)(&chEst_ul[ChanEstInd + ChanEstInd2]), 2 * length_b);
            }

            MemCpyFromExt((void *) (&derotCeva[derotInd]),
                              &derotation[derotInd], length_b);

            StopSubTask(SUBTASKID_RX_MULTICHAN_DEMODULATION_DMA_IN);

            expoInd = i*L + isym;
            rx_Multichan_DeModulation(RXINCeva, ChEstCeva, chEst_pilotCeva, RXOUTCeva, CHOUTCeva,
                            &derotCeva[derotInd], expo_mat, &expo_pilot[0],
                            &reDet0Ceva[offset_pusch], &expo_reEst_norm[expoInd],
                            expo_reEst1Ceva, expo_norm, &noiseVar[2 * MO * i],
                            (S16)MO, (S16)Msc, (S16)L, (S16)isym, Nul_symb, Qm);

            //////////////////////////////////////////////////////////////////////////////
            //Increment pointers to point to the next user
            //////////////////////////////////////////////////////////////////////////////
            ChanEstInd2  += 4*Msc;      //Index for all ChanEst Inputs
            offset_pusch += 2*(Msc+2);  //Index for pReDet0
            derotInd     += 2*Msc;      //Index for Derotation buffer
        }   //for numPusch

        length_b = 2 * offset_pusch;

        StartSubTask(SUBTASKID_RX_MULTICHAN_DEMODULATION_DMA_OUT);
        MemCpyToExt((void *) &reDet0[reDet_isym * step_reDet], (void *) reDet0Ceva, length_b);
        StopSubTask(SUBTASKID_RX_MULTICHAN_DEMODULATION_DMA_OUT);

    }   //for NUM_SLOTS_SUBFRM

    SetSubTaskLoggerCount(SUBTASKID_RX_MULTICHAN_DEMODULATION_START, SUBTASKID_RX_MULTICHAN_DEMODULATION_END);
    return;
}

#ifndef CEVA_INTRINSICS
////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink IDFT Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |IDFT_task                          |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink IDFT function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: IDFT() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void IDFT_task(PTCB pTCB)
{
    S32 offset = 0, offset1 = 0;
    U32* pContext = 0;
    S32 srsPilot, num_sym;
    S32 i = 0, j, step_reDet;

    S16 *input, *output;
    U16 *idftSize;
    S16 numPusch;

    pContext = (U32*) pTCB->ContextPtr;
    numPusch = (S16)pContext[0];
    step_reDet = (S32) pContext[1];
    num_sym    = (S32) pContext[2];
    srsPilot   = (S32) pContext[3];

    input    = (S16*) extractTCBIOBuf(pTCB, 0);
    output   = (S16*) extractTCBIOBuf(pTCB, 1);
    idftSize = (U16*) extractTCBIOBuf(pTCB, 2);

#ifdef DEBUG_PRINT
    {
        double max = 0.0;
        S32 totalIdftSize = 0;
        for(i = 0; i < numPusch; i++)
            totalIdftSize += idftSize[i];

        for(i = 0; i < num_sym; i++)
        {
            filePrint(fIdft_In, &input[i*step_reDet], totalIdftSize, "S16", "Fix2Fl_Complex",
                        (double)(1.0), LOG_BS_IDFTIN_FX);
        }
    }
#endif

    for(j = 0; j < num_sym; j++)
    {
        offset = 0;
        offset1 = j * step_reDet;
        if ((srsPilot != -1) && (j == (num_sym-1)))
            return;

        for(i = 0; i < numPusch; i++)
        {
            IDFT(&input[offset + offset1], idftSize[i], &output[offset + offset1],
                        &output[2*idftSize[i] + offset + offset1]);

            offset += 2*(idftSize[i]+2);
        }
    }
    return;
}
#endif  //CEVA_INTRINSICS


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink setup MAP iDFT descriptors task function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |mapSetupDescriptor   |          |Input         |1         |real          |(32:0:0)|Full Range  |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |MapSetupDescriptorsTask            |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |mapSetupDescriptor                 |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |                                   |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will setup the MAP decriptors for all of the iDFT sizes needed to run
 *  in the current subframe.
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: IDFT() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
U32 MapSetupDescriptorsTask(U32 mapSetupDescriptor)
{
    PMAP_SETUP_DESCRIPTOR pMapSetupDesc = (PMAP_SETUP_DESCRIPTOR)mapSetupDescriptor;
    U32  j, i;
    U16  MaxIdftSizeInDesc[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    U16  NumIdftSizeInDesc[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    U16 *pIdftSizesInDesc[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    U16 *pIdftInpLocInDesc[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    U16 *pIdftOutLocInDesc[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    U32  ConfigTablesSize[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    U32  NumDesc;

    PMAP_DRV_IDFT_MULT_TABLE pMultTableStruct;
    U8 *pIdftMultTable[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    U32 MultTableSize[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    PDMAFDESC pIdftFDescHead, pInFDesc, pOutFDesc, pOutFDescLast, pConfigFDesc, pMultTableFDesc, pFDesc = NULL, pFDesc1 = NULL;
    U32 SymNum, CurrMask, FpId, ResourceIndex;
    U32 SymMask, IdftCurFp, IdftFpStop, IdftFpStart;
    U32 IdftOutputSize;

    U32 ArrSize = pMapSetupDesc->ArrSize;
    U16 pIdftSizeArr[FP_DRV_MAX_NUM_USERS_PER_TTI];
    U16 totalInputLoc, totalOutputLoc, IdftSizeIndex, NumIdftSizeInDescTemp;
    PMAP_DRV_IDFT_CONFIG pConfigTableStruct;
    U16 *pConfigTableSize;
    U32  offset = 0;
    U8  *pAllocPtr;

    U32 numAnt, numSym, cond;
    PDMAFDESC pFftFDescHead;
    MAP_DMA_TR_CTRL_CEVA FftInTrCtrl128, FftInTrCtrl;
    PDMAFDESC pIfftFDescHead = pMapSetupDesc->pIfftFDescHead0;
    MAP_DMA_TR_CTRL_CEVA IfftInTrCtrl0, IfftInTrCtrl1;

    // Setup FFT Descriptors
    //-----------------------
    cond = 0;

    memcpy(&FftInTrCtrl,    &pMapSetupDesc->FftInTrCtrl, sizeof(MAP_DMA_TR_CTRL_CEVA));
    memcpy(&FftInTrCtrl128, &pMapSetupDesc->FftInTrCtrl128, sizeof(MAP_DMA_TR_CTRL_CEVA));

    for (numAnt = 0; numAnt < pMapSetupDesc->numRxAnt; numAnt++)
    {
        pFftFDescHead = pMapSetupDesc->pFftFDescHead0;
        for (numSym = 0; numSym < 7; numSym++)
        {
            // Special 128 Pt FFT
            if (pMapSetupDesc->fftIfftSize == 128)
            {
                cond = 0;             // Symbols 0, 2, 4, 6, 7, 8, 11, 13
                if ((numSym<7) && ((numSym&1))) // Symbols 1, 3, 5
                    cond = 1;
                if ((numSym>7) && ((numSym&1)==0)) // Symbols 8, 10, 12
                    cond = 1;
            }

            pInFDesc = pFftFDescHead + (numSym*2) + (numAnt*14*2);
            pOutFDesc = pInFDesc+1;

            //Set Input Descriptor
            pInFDesc->FControl = 0; // disable interrupt
            if (cond)
                memcpy((void*)&pInFDesc->FStatus0, &FftInTrCtrl128, sizeof(MAP_DMA_TR_CTRL_CEVA));
            else
                memcpy((void*)&pInFDesc->FStatus0, &FftInTrCtrl, sizeof(MAP_DMA_TR_CTRL_CEVA));

            // Set Output Descriptor
            pOutFDesc->FControl = 0; // disable interrupt
            pOutFDesc->FStatus0 = 0;
            pOutFDesc->FStatus1 = 0;
        }

        pFftFDescHead = pMapSetupDesc->pFftFDescHead1;
        for (numSym = 7; numSym < 14; numSym++)
        {
            // Special 128 Pt FFT
            if (pMapSetupDesc->fftIfftSize == 128)
            {
                cond = 0;             // Symbols 0, 2, 4, 6, 7, 8, 11, 13
                if ((numSym<7) && ((numSym&1))) // Symbols 1, 3, 5
                    cond = 1;
                if ((numSym>7) && ((numSym&1)==0)) // Symbols 8, 10, 12
                    cond = 1;
            }

            pInFDesc = pFftFDescHead + (numSym*2) + (numAnt*14*2);
            pOutFDesc = pInFDesc+1;

            //Set Input Descriptor
            pInFDesc->FControl = 0; // disable interrupt
            if (cond)
                memcpy((void*)&pInFDesc->FStatus0, &FftInTrCtrl128, sizeof(MAP_DMA_TR_CTRL_CEVA));
            else
                memcpy((void*)&pInFDesc->FStatus0, &FftInTrCtrl, sizeof(MAP_DMA_TR_CTRL_CEVA));

            // Set Output Descriptor
            pOutFDesc->FControl = 0; // disable interrupt
            pOutFDesc->FStatus0 = 0;
            pOutFDesc->FStatus1 = 0;
        }
    }


    // Setup IFFT Descriptors
    //-----------------------
    memcpy(&IfftInTrCtrl0, &pMapSetupDesc->IfftInTrCtrl0, sizeof(MAP_DMA_TR_CTRL_CEVA));
    memcpy(&IfftInTrCtrl1, &pMapSetupDesc->IfftInTrCtrl1, sizeof(MAP_DMA_TR_CTRL_CEVA));

    for (numAnt = 0; numAnt < pMapSetupDesc->numTxAnt; numAnt++)
    {
        for (numSym = 0; numSym < 14; numSym++)
        {
            pInFDesc = pIfftFDescHead + (numSym*2) + (numAnt*14*2);
            pOutFDesc = pInFDesc+1;

            // Input Data: Input
            pInFDesc->FControl = 0; // disable interrupt
            if ((numSym == 0) || (numSym == 7))
                memcpy((void*)&pInFDesc->FStatus0, &IfftInTrCtrl0, sizeof(MAP_DMA_TR_CTRL_CEVA));
            else
                memcpy((void*)&pInFDesc->FStatus0, &IfftInTrCtrl1, sizeof(MAP_DMA_TR_CTRL_CEVA));

            // Output Data: Output
            pOutFDesc->FControl = 0; // disable interrupt
            pOutFDesc->FStatus0 = 0;
            pOutFDesc->FStatus1 = 0;
        }
    }



    // Setup IDFT Descriptors
    //---------------------------

    // Initialize
    if (ArrSize > FP_DRV_MAX_NUM_USERS_PER_TTI)
    {
        return 65535;
    }

    pAllocPtr = (U8*)DataBuff_Bank1;
    if (ArrSize)
    {

        // Malloc
        //------------

        for (j = 0; j < MAX_IDFT_TRANSACTIONS_PER_SYMBOL; j++)
        {
            pIdftSizesInDesc[j]    = (U16*)MemAllocFrom(pAllocPtr, &offset, sizeof(U16)*FP_DRV_MAX_NUM_USERS_PER_TTI);
            pIdftInpLocInDesc[j]   = (U16*)MemAllocFrom(pAllocPtr, &offset, sizeof(U16)*FP_DRV_MAX_NUM_USERS_PER_TTI);
            pIdftOutLocInDesc[j]   = (U16*)MemAllocFrom(pAllocPtr, &offset, sizeof(U16)*FP_DRV_MAX_NUM_USERS_PER_TTI);
            memset(pIdftSizesInDesc[j],  0, sizeof(U16)*FP_DRV_MAX_NUM_USERS_PER_TTI);
            memset(pIdftInpLocInDesc[j], 0, sizeof(U16)*FP_DRV_MAX_NUM_USERS_PER_TTI);
            memset(pIdftOutLocInDesc[j], 0, sizeof(U16)*FP_DRV_MAX_NUM_USERS_PER_TTI);
        }
        memset(MaxIdftSizeInDesc, 0, MAX_IDFT_TRANSACTIONS_PER_SYMBOL*sizeof(U16));
        memset(NumIdftSizeInDesc, 0, MAX_IDFT_TRANSACTIONS_PER_SYMBOL*sizeof(U16));



        // Generate Tables
        //------------------
        memcpy(pIdftSizeArr, pMapSetupDesc->pIdftSizeArr, sizeof(U16)*ArrSize);
        NumDesc = 0;

        // Calculate Number of Descriptors needed to perform IDFT on this symbol
        totalInputLoc = 0;
        totalOutputLoc = 0;
        for (i = 0; i < ArrSize; i++)
        {
            for (j = 0; j < MAX_IDFT_TRANSACTIONS_PER_SYMBOL; j++)
            {
                if (MaxIdftSizeInDesc[j])
                {
                    if (MaxIdftSizeInDesc[j] <= MAX_IDFT_SIZE_SMALLER_TRANSACTION)
                    {
                        if (pIdftSizeArr[i] <= MAX_IDFT_SIZE_SMALLER_TRANSACTION)
                        {
                            if (MaxIdftSizeInDesc[j] < pIdftSizeArr[i])
                                MaxIdftSizeInDesc[j] = pIdftSizeArr[i];

                            NumIdftSizeInDescTemp = NumIdftSizeInDesc[j]++;
                            pIdftSizesInDesc[j][NumIdftSizeInDescTemp] = pIdftSizeArr[i];
                            pIdftInpLocInDesc[j][NumIdftSizeInDescTemp] = totalInputLoc;
                            pIdftOutLocInDesc[j][NumIdftSizeInDescTemp] = totalOutputLoc;
                            break;
                        }
                    }
                    else
                    {
                        if (pIdftSizeArr[i] == MaxIdftSizeInDesc[j])
                        {
                            NumIdftSizeInDescTemp = NumIdftSizeInDesc[j]++;
                            pIdftSizesInDesc[j][NumIdftSizeInDescTemp] = pIdftSizeArr[i];
                            pIdftInpLocInDesc[j][NumIdftSizeInDescTemp] = totalInputLoc;
                            pIdftOutLocInDesc[j][NumIdftSizeInDescTemp] = totalOutputLoc;
                            break;
                        }
                    }
                }
                else
                {
                    MaxIdftSizeInDesc[j] = pIdftSizeArr[i];

                    NumIdftSizeInDescTemp = NumIdftSizeInDesc[j]++;
                    pIdftSizesInDesc[j][NumIdftSizeInDescTemp] = pIdftSizeArr[i];
                    pIdftInpLocInDesc[j][NumIdftSizeInDescTemp] = totalInputLoc;
                    pIdftOutLocInDesc[j][NumIdftSizeInDescTemp] = totalOutputLoc;
                    break;
                }
            }
            totalInputLoc += (pIdftSizeArr[i] + 2);     // 2 because of exponent in output will be written here
            totalOutputLoc += (pIdftSizeArr[i] + 2);    // 2 because of exponent

            if (NumDesc < j)
                NumDesc = j;
        }

        NumDesc++;

        for (i = 0; i < NumDesc; i++)
        {
            ConfigTablesSize[i] = sizeof(MAP_DRV_IDFT_CONFIG)*NumIdftSizeInDesc[i] + 8;
            memset(pMapSetupDesc->pConfigTables[i], 0, sizeof(MAP_DRV_IDFT_CONFIG)*NumIdftSizeInDesc[i] + 8);
            memset(pMapSetupDesc->pMultTables[i], 0, sizeof(MAP_DRV_IDFT_MULT_TABLE)*NumIdftSizeInDesc[i]);
        }

        // Initialize all tables to correct values
        for (i = 0; i < NumDesc; i++)
        {
            // pConfigTables
            pConfigTableSize = (U16*)pMapSetupDesc->pConfigTables[i];
            pConfigTableSize[0] = (NumIdftSizeInDesc[i] * sizeof(MAP_DRV_IDFT_CONFIG)>>2) + 1; // Number of copies to be done from Bank 0 and Bank 6
            pConfigTableSize[2] = NumIdftSizeInDesc[i]; // Number of loops to do on Bank 6
            pConfigTableStruct = (PMAP_DRV_IDFT_CONFIG)&pConfigTableSize[4];

            // ppMultTables
            pMultTableStruct = (PMAP_DRV_IDFT_MULT_TABLE)pMapSetupDesc->pMultTables[i];

            for (j = 0; j < NumIdftSizeInDesc[i]; j++)
            {
                IdftSizeIndex = pMapSetupDesc->fp_idft_size_index_map[pIdftSizesInDesc[i][j]];
                pConfigTableStruct[j].InputLoc[0] = 512 + pIdftInpLocInDesc[i][j];  // 512 because we copy input to location 512
                pConfigTableStruct[j].OutputLoc[0] = pIdftOutLocInDesc[i][j];
                pConfigTableStruct[j].ConfigLoc[0] = pMapSetupDesc->fp_idft_big_config_start[IdftSizeIndex];
                pConfigTableStruct[j].MultTableLoc[0] = pMapSetupDesc->fp_idft_mult_table_start[IdftSizeIndex];
            }

            IdftSizeIndex = pMapSetupDesc->fp_idft_size_index_map[MaxIdftSizeInDesc[i]];
            pMultTableStruct->TableDMALoc = (U8*)pMapSetupDesc->fp_idft_tables[IdftSizeIndex];
            pMultTableStruct->TableSize = pMapSetupDesc->fp_idft_table_sizes_with_offset[IdftSizeIndex];
        }



        // Queue IDFT Descriptors
        //----------------------

        SymNum = 0;
        ResourceIndex = 0;

        for (i=0; i<NumDesc; i++)     // Allocate max IOBuffers. This will be updated later
        {
            pMultTableStruct = (PMAP_DRV_IDFT_MULT_TABLE)pMapSetupDesc->pMultTables[i];
            pIdftMultTable[i] = pMultTableStruct->TableDMALoc;
            MultTableSize[i] = pMultTableStruct->TableSize << 2;
        }

        // Set Head to the correct context
        pIdftFDescHead = (PDMAFDESC)pMapSetupDesc->pIdftFDescHead;

        // Build a list if something is already runnin on the MAPs
        SymMask = pMapSetupDesc->SymMask;
        IdftFpStart = pMapSetupDesc->IdftFpStart;
        IdftFpStop = pMapSetupDesc->IdftFpStop;
        IdftCurFp = IdftFpStart;
        IdftOutputSize = pMapSetupDesc->IdftOutputSize;

        for (i = IdftFpStart; i <= IdftFpStop; i++)
        {
            pMapSetupDesc->pReadyListHead[i] = NULL;
            pMapSetupDesc->pReadyListTail[i] = NULL;
        }

        while(SymMask != 0)
        {
            CurrMask = 1<<SymNum;
            if (CurrMask & SymMask)
            {
                SymMask &= ~CurrMask;

                FpId = IdftCurFp++;
                if (IdftCurFp > IdftFpStop)
                    IdftCurFp = IdftFpStart;

                ResourceIndex |= 1<<FpId;

                // Setup Descriptors based on TCB input
                pInFDesc = pIdftFDescHead + SymNum*MAX_NUM_IDFT_DMA_DESC;           // Desc 0
                pConfigFDesc = pInFDesc + 1;                                        // Desc 1
                pMultTableFDesc = pInFDesc + 2;                                     // Desc 2
                pOutFDesc = pInFDesc + NUM_IDFT_OUTPUT_DESC_START;                  // First Output Desc

                pOutFDescLast = pOutFDesc;

                // Input Data: Input
                pInFDesc->FControl = 0;
                memcpy((void*)&pInFDesc->FStatus0, (void*)&pMapSetupDesc->IdftInTrCtrl[FpId][0], sizeof(MAP_DMA_TR_CTRL_CEVA));

                for (i = 0; i < NumDesc; i++)
                {
                    // Chain descriptors together
                    if (i > 0)
                    {
                        pFDesc->NextFDesc = pConfigFDesc;           // Mult Descriptor
                        pFDesc1->NextFDesc = pOutFDescLast;         // Output Descriptor
                        pFDesc1->OutBDesc.BCtrl = BCTRL_BLAST_MASK | (2*sizeof(short)*2);   // Dummy Output
                    }

                    // Input Data: Config
                    pConfigFDesc->FControl = 0;
                    pConfigFDesc->OutBDesc.BCtrl = BCTRL_BLAST_MASK | ConfigTablesSize[i];
                    pConfigFDesc->OutBDesc.BPtr = (U32)pMapSetupDesc->pConfigTables[i];
                    memcpy((void*)&pConfigFDesc->FStatus0, (void*)&pMapSetupDesc->IdftInTrCtrl[FpId][1], sizeof(MAP_DMA_TR_CTRL_CEVA));

                    // Input Data: Mult Tables
                    pMultTableFDesc->FControl = 0;
                    pMultTableFDesc->NextFDesc = NULL;
                    pMultTableFDesc->OutBDesc.BCtrl = BCTRL_BLAST_MASK | MultTableSize[i];
                    pMultTableFDesc->OutBDesc.BPtr = (U32)pIdftMultTable[i];
                    memcpy((void*)&pMultTableFDesc->FStatus0, (void*)&pMapSetupDesc->IdftInTrCtrl[FpId][2], sizeof(MAP_DMA_TR_CTRL_CEVA));

                    // Output Data: Output
                    pOutFDescLast->FControl = 0;
                    pOutFDescLast->NextFDesc = NULL;
                    pOutFDescLast->OutBDesc.BCtrl = BCTRL_BLAST_MASK | IdftOutputSize;
                    memcpy((void*)&pOutFDescLast->FStatus0, (void*)&pMapSetupDesc->IdftOutTrCtrl[FpId], sizeof(MAP_DMA_TR_CTRL_CEVA));

                    // Save Descriptors
                    pFDesc = pMultTableFDesc;
                    pFDesc1 = pOutFDescLast;

                    // Increment Descriptors
                    pConfigFDesc = pConfigFDesc + 2;
                    pMultTableFDesc = pMultTableFDesc + 2;
                    pOutFDescLast = pOutFDescLast + 1;
                }

                // If a list does not exist yet and this tcb is the first one in the list
                if (pMapSetupDesc->pReadyListHead[FpId] == NULL)
                {
                    pMapSetupDesc->pReadyListHead[FpId] = pMapSetupDesc->pReadyListTail[FpId] = pInFDesc;
                }

                // If list exists already and this tcb is to be appeneded
                else
                {
                    // Set Input
                    pFDesc = pMapSetupDesc->pReadyListTail[FpId];
                    while (pFDesc->NextFDesc)
                        pFDesc = pFDesc->NextFDesc;
                    pFDesc->NextFDesc = pInFDesc;

                    // Set Output
                    pFDesc = pMapSetupDesc->pReadyListTail[FpId] + NUM_IDFT_OUTPUT_DESC_START;
                    while (pFDesc->NextFDesc)
                        pFDesc = pFDesc->NextFDesc;
                    pFDesc->NextFDesc = pOutFDesc;

                    pMapSetupDesc->pReadyListTail[FpId] = pInFDesc;

                }
            }
            SymNum++;
        }

        pMapSetupDesc->pTcb->ResourceIndex = (U16)ResourceIndex;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// End PUSCH Tasks
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink rx_Demapper_fx Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |rx_Demapper_fx_task                |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink rx_Demapper_fx function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: rx_Demapper_fx() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void rx_Demapper_fx_task(PTCB pTCB)
{
    S16 expo, i;
    S32 offset = 0, offset1, expoInd;

    S16 Qm[MAXCHSUBFRAME], numBits[MAXCHSUBFRAME], Msc;
    S8  *bDetSoftCeva[MAXCHSUBFRAME];
    S32 *descrIndex, descrIndexCeva[MAXCHSUBFRAME];
    S8  *scrSeq, *scrSeqCeva[MAXCHSUBFRAME], *scrSeqCevaStart;
    U32 TotalLen = 0;

    U32* pContext = 0;
    S32 numPusch;

    S16 *reEst1, *expo_reEst;
    S16 *reEst1_Ceva;

    S8 *bDetSoft;
    ULRXINPUTPARAMS *pRxInParams;
    PULRXFECPARAMS pFecParams;

    S32 isym, isym1, L, CP, K, k;
    S32 pilotSym, srsPilot, Nul_symb;
    S32 offset_reEst, step_reEst;
    S32 length_b_in;
    S8  *pMuxSoftDec;
    S8  *bDetSoftCevaStart;

    StartSubTask(SUBTASKID_RX_DEMAPPER_START);

    pContext   = (U32*) pTCB->ContextPtr;
    isym       = pContext[0];
    srsPilot   = pContext[1];
    step_reEst = pContext[2];

    reEst1      = (S16 *) extractTCBIOBuf(pTCB, 0);
    expo_reEst  = (S16 *) extractTCBIOBuf(pTCB, 1);
    pRxInParams = (ULRXINPUTPARAMS *) extractTCBIOBuf(pTCB, 2);
    pFecParams  = (PULRXFECPARAMS) extractTCBIOBuf(pTCB, 3);
    scrSeq      = (S8*) extractTCBIOBuf(pTCB, 4);
    descrIndex  = (S32*) extractTCBIOBuf(pTCB, 5);
    pMuxSoftDec = (S8 *) extractTCBIOBuf(pTCB, 6);

    numPusch = pRxInParams->numPUSCH;
    K  = pRxInParams->K;
    L  = pRxInParams->L;
    CP = pRxInParams->CPType;
    Nul_symb = pRxInParams->Nul_symb;
    pilotSym = pRxInParams->pilotCarrierSymbol;

    // DMA data into Ceva Memory
    length_b_in = 2 * (K + 2*numPusch);

    reEst1_Ceva = (S16 *) &DataBuff_Bank1[0];
    bDetSoftCevaStart = (S8 *) &reEst1_Ceva[length_b_in];

    for(i = 0; i < numPusch; i++)
    {
        Msc   = pRxInParams->Mpusch_sc[i];
        Qm[i] = pRxInParams->Qm[i];
        numBits[i] = Msc * Qm[i];
        bDetSoftCeva[i] = &bDetSoftCevaStart[offset];
        offset += numBits[i];
        descrIndexCeva[i] = descrIndex[i];
        TotalLen += numBits[i];
    }
    scrSeqCevaStart = (S8*)(&bDetSoftCevaStart[TotalLen]);

    StopSubTask(SUBTASKID_RX_DEMAPPER_EXTRACT);
    for(k = 0; k < NUM_SLOTS_SUBFRM; k++)
    {
        StartSubTask(SUBTASKID_RX_DEMAPPER_DMA_IN);

        //Find symbol in 1st or 2nd slot
        isym += k * (Nul_symb - 1);
        isym1 = isym;

        //Need to skip over pilot exponents
        if (isym >= pilotSym)
            isym1++;
        if (isym >= (pilotSym + Nul_symb - 1))
            isym1++;

        //return if we are at pilot symbol
        if (isym1 == srsPilot)
            break;

        offset_reEst = isym * step_reEst;
        MemCpyFromExt((void*) reEst1_Ceva, (void*) &reEst1[offset_reEst], length_b_in * sizeof(S16));

        offset = 0;
        for(i = 0; i < numPusch; i++)
        {
            scrSeqCeva[i] = &scrSeqCevaStart[offset];
            offset1 = descrIndexCeva[i] + (isym * numBits[i] >> 3);
            MemCpyFromExt((void*) scrSeqCeva[i], (void*) &scrSeq[offset1], (numBits[i] >> 3));
            offset += (numBits[i] >> 3);
        }

        StopSubTask(SUBTASKID_RX_DEMAPPER_DMA_IN);

        offset = 0;
        for(i = 0; i < numPusch; i++)
        {
            StartSubTask(SUBTASKID_RX_DEMAPPER_MAIN);

            Msc   = pRxInParams->Mpusch_sc[i];
            expoInd = i*L + isym1;
            expo = expo_reEst[expoInd] + reEst1_Ceva[2*Msc + offset];

            rx_Demapper_fx(&reEst1_Ceva[offset], Qm[i], scrSeqCeva[i],
                              bDetSoftCeva[i], Msc, PUSCH, expo);

            StopAndStartSubTask(SUBTASKID_RX_DEMAPPER_MAIN, SUBTASKID_RX_DEMAPPER_MUX_SOFTBITS);

            if(pFecParams->muxPUSCH_User[i])
            {
                bs_rx_mux_control_extract_softbits_sym(pFecParams, scrSeqCeva[i], bDetSoftCeva[i],
                                    i, CP, (U32)isym, pMuxSoftDec);
            }

            StopAndStartSubTask(SUBTASKID_RX_DEMAPPER_MUX_SOFTBITS, SUBTASKID_RX_DEMAPPER_DMA_OUT);

#if defined(EVM) || defined(RT_API_PROC)
            bDetSoft = pFecParams->by[i];
#else
            offset1   = 7 + i;
            bDetSoft = (S8*) extractTCBIOBuf(pTCB, offset1);
#endif
            offset1 = isym * numBits[i];
            MemCpyToExt((void *) &bDetSoft[offset1], (void *) bDetSoftCeva[i], numBits[i]);

            StopSubTask(SUBTASKID_RX_DEMAPPER_DMA_OUT);

            offset += 2 * (Msc + 2);

        }   //for numPusch
    }   //for NUM_SLOTS_SUBFRM

    SetSubTaskLoggerCount(SUBTASKID_RX_DEMAPPER_START, SUBTASKID_RX_DEMAPPER_END);
    return;
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink muxCntlRI_CQI_Dec_task Task Wrap Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |muxCntlRI_CQI_Dec_task_wrap        |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function is needed to call muxCntlRI_CQI_Dec from REX without invoking a TaskList.
 *  ARM is able to directly call muxCntlRI_CQI_Dec_task which will then call bs_rx_mux_RI_CQI_decoding
 *  to run on the Ceva.
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: bs_rx_mux_RI_CQI_decoding() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
U32 muxCntlRI_CQI_Dec_task_wrap(U32 pTCB)
{
    if(pTCB != 0)
        muxCntlRI_CQI_Dec_task((PTCB)pTCB);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink muxCntlRI_CQI_Dec_task Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |muxCntlRI_CQI_Dec_task             |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink bs_rx_mux_RI_CQI_decoding function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: bs_rx_mux_RI_CQI_decoding() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void muxCntlRI_CQI_Dec_task(PTCB pTCB)
{
    PULRXFECPARAMS pFecParams;
    S8 *RM_decTablAddrsCeva[11], *FHT_Mask_Addrs;
    S8 *RM_decTablAddrs, *pMuxSoftDec, *pCevaMuxSofDec;
    S32 TotalLen, offACK;
    U32 memTop = NULL;

    StartSubTask(SUBTASKID_RX_FEC_MUX_RI_CQI_CNTL_START);

    memTop = (U32) DataBuff_Bank1a;
    //We have to be careful here.  The total size of the soft decision buffer
    //assuming worst case is extremely big.  There is not enough internal
    //Ceva memory to accommodate this pathalogical case where CQI will fill
    //up the entire bandwidth.  There is about 92K of free internal Ceva
    //memory so we can only safely DMA in about 30K of soft decisions since
    //we use DataBuff_Bank3(0x10000) as temp area for Reed Muller tables.
    //Viterbi decoder also needs it's own temp buffer which is the size of
    //the soft decision buffer
    pCevaMuxSofDec = (S8*)memTop;

    pFecParams  = (PULRXFECPARAMS) extractTCBIOBuf(pTCB, 0);
    pMuxSoftDec = (S8 *) extractTCBIOBuf(pTCB, 1);

    StopSubTask(SUBTASKID_RX_FEC_MUX_RI_CQI_CNTL_EXTRACT);

    if(pFecParams->muxPUSCHCqiRi)
    {
        StartSubTask(SUBTASKID_RX_FEC_MUX_RI_CQI_CNTL_DMA_IN);

        RM_decTablAddrs     = (S8*) extractTCBIOBuf(pTCB, 2);
        FHT_Mask_Addrs      = (S8*) extractTCBIOBuf(pTCB, 3);

#if defined(_WIN32) || defined(EVM) || defined(RT_API_PROC)

        //Come here if running function on PC, ARM or Ceva EVM
        MemCpyFromExt((void*) RM_decTablAddrsCeva, (void*) RM_decTablAddrs, 11 * sizeof(S8*));

#else
        {
            S32 i, numVar;

            numVar = 4;
            //Only come in here for Ceva Simulator
            for(i = 0; i < 11; i++)
                RM_decTablAddrsCeva[i]  = (S8*) extractTCBIOBuf(pTCB, (numVar + i));
        }
#endif
        TotalLen = pFecParams->TotalRI_Bits + pFecParams->TotalCQI_Bits;
        offACK = pFecParams->TotalACK_Bits;

        //Check if the total length of the soft decision buffer is bigger then available
        //Ceva internal memory.
        if (TotalLen < 30000)
        {
            MemCpyFromExt((void*) pCevaMuxSofDec, (void*) &pMuxSoftDec[offACK], TotalLen);
            memTop += RUP32B(TotalLen);
        }
        else
        {
            //if soft decision buffer is too big, there is not enough room to DMA them
            //into internal ceva memory.  To prevent corruption of tables, just read
            //soft decisions directly from pMuxSoftDec instead.
            pCevaMuxSofDec = (S8*)(&pMuxSoftDec[offACK]);
        }

        StopSubTask(SUBTASKID_RX_FEC_MUX_RI_CQI_CNTL_DMA_IN);

        bs_rx_mux_RI_CQI_decoding(pFecParams, pCevaMuxSofDec, memTop, RM_decTablAddrsCeva, FHT_Mask_Addrs);

        if (TotalLen < 30000)
            memTop -= RUP32B(TotalLen);
    }

    SetSubTaskLoggerCount(SUBTASKID_RX_FEC_MUX_RI_CQI_CNTL_START, SUBTASKID_RX_FEC_MUX_RI_CQI_CNTL_END);
    return;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink muxCntlACK_Dec_task Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |muxCntlACK_Dec_task                |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink bs_rx_mux_ACK_decoding function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: bs_rx_mux_ACK_decoding() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void muxCntlACK_Dec_task(PTCB pTCB)
{
    PULRXFECPARAMS pFecParams;
    S8  *pMuxSoftDec, *pCevaMuxSofDec;
    S32 TotalLen;
    U32 memTop = NULL;

    memTop = (U32) DataBuff_Bank1a;
    pCevaMuxSofDec = (S8*)(memTop);

    StartSubTask(SUBTASKID_RX_FEC_MUX_ACK_CNTL_START);

    pFecParams  = (PULRXFECPARAMS) extractTCBIOBuf(pTCB, 0);
    pMuxSoftDec = (S8 *) extractTCBIOBuf(pTCB, 1);

    StopSubTask(SUBTASKID_RX_FEC_MUX_ACK_CNTL_EXTRACT);

    if(pFecParams->muxPUSCHAckNack)
    {
        StartSubTask(SUBTASKID_RX_FEC_MUX_ACK_CNTL_DMA_IN);

        TotalLen = pFecParams->TotalACK_Bits;
        MemCpyFromExt((void*) pCevaMuxSofDec, (void*) pMuxSoftDec, TotalLen);

        StopAndStartSubTask(SUBTASKID_RX_FEC_MUX_ACK_CNTL_DMA_IN, SUBTASKID_RX_FEC_MUX_ACK_CNTL_MAIN);

        bs_rx_mux_ACK_decoding(pFecParams, pCevaMuxSofDec);

        StopSubTask(SUBTASKID_RX_FEC_MUX_ACK_CNTL_MAIN);
    }

    SetSubTaskLoggerCount(SUBTASKID_RX_FEC_MUX_ACK_CNTL_START, SUBTASKID_RX_FEC_MUX_ACK_CNTL_END);
    return;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// End PUSCH Tasks
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink ul_signal_power Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |ul_signal_power_task               |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function call calculate the input power to the FFT in the uplink direction
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void ul_signal_power_task(PTCB pTCB)
{
    ULRXINPUTPARAMS *pULPilotInput;
    S16 *adcBuf0;//, *adcBuf1;
    U32* pContext = 0;
    U32 sum1 = 0, sum2 = 0;

    S16 *ptr;
    U32 i, j;
    U32 nCp = 0, nCp0 = 0, shift = 1;

    pContext = (U32*) pTCB->ContextPtr;

    adcBuf0 = (S16*)pContext[0];
    //adcBuf1 = (S16*)pContext[1];
    pULPilotInput = (PULRXINPUTPARAMS)pContext[2];

    switch (pULPilotInput->Nfft)
    {
        case 128:
            nCp  = 9;
            nCp0 = 10;
            shift = 7;
        break;
        case 256:
            nCp  = 18;
            nCp0 = 20;
            shift = 8;
        break;
        case 512:
            nCp  = 36;
            nCp0 = 40;
            shift = 9;
        break;
        case 1024:
            nCp  = 72;
            nCp0 = 80;
            shift = 10;
        break;
        case 2048:
            nCp  = 144;
            nCp0 = 160;
            shift = 11;
        break;
        default:
        return;
    }

    ptr = adcBuf0;
    for (j = 0; j < (U32)pULPilotInput->L; j++)
    {
        if ((j == 0) || (j == 7))
        {
            ptr += (nCp0*2);
        }
        else
        {
            ptr += (nCp*2);
        }

        for (i = 0; i < (U32)pULPilotInput->Nfft; i+=10)
        {
            sum1 += abs_s(ptr[2*i]) + abs_s(ptr[2*i+1]);
        }

        ptr += (pULPilotInput->Nfft*2);
    }
    sum1 = sum1 >> shift;

#if 0
    if (pULPilotInput->MO > 1)
    {
        ptr = adcBuf1;
        for (j = 0; j < (U32)pULPilotInput->L; j++)
        {
            if ((j == 0) || (j == 7))
            {
                ptr += (nCp0*2);
            }
            else
            {
                ptr += (nCp*2);
            }

            for (i = 0; i < (U32)pULPilotInput->Nfft; i+=10)
            {
                sum2 += abs_s(ptr[2*i]) + abs_s(ptr[2*i+1]);
            }

            ptr += (pULPilotInput->Nfft*2);
        }

        sum2 = sum2 >> shift;
    }
#endif

    pContext[3] = sum1;
    pContext[4] = sum2;

    return;
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// PUCCH Tasks
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink ul_Pilot_PUCCH Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |ul_Pilot_PUCCH_task                |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink ul_Pilot_PUCCH function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: ul_Pilot_PUCCH() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void ul_Pilot_PUCCH_task(PTCB pTCB)
{
    ULRXINPUTPARAMS *pULPilotInput;
    U32 *rs_grid;
    S16 *r_alpha_uv, *aPilots, Msc, L;
    S16 *Sdescramb;
    S32 i, j;
    U32* pContext = 0;
    S32 indPucch1, indPucch2, offset_pucch, Nul_symb;
    S32 len_r_alpha_uv, step_aDet, slotNum, numPucch;
    S16 *ncs_cell, *nOc;
    S16 uSeqGrpNum[NUM_SLOTS_SUBFRM], nOcBar[NUM_SLOTS_SUBFRM];
    S16 SdescrambCeva[NUM_SLOTS_SUBFRM], nOcCeva[NUM_SLOTS_SUBFRM];
    S16 numPilotSym, *nCs, *r_alpha_uvCeva;
    S16 *aPilotsCeva;

    StartSubTask(SUBTASKID_RX_ULPILOT_PUCCH_START);

    pContext = (U32*) pTCB->ContextPtr;
    len_r_alpha_uv = pContext[0];
    step_aDet = pContext[1];

    pULPilotInput = (ULRXINPUTPARAMS *) extractTCBIOBuf(pTCB, 0);
    rs_grid       = (U32 *) extractTCBIOBuf(pTCB, 1);
    r_alpha_uv    = (S16 *) extractTCBIOBuf(pTCB, 2);
    aPilots       = (S16 *) extractTCBIOBuf(pTCB, 3);
    nOc           = (S16 *) extractTCBIOBuf(pTCB, 4);
    Sdescramb     = (S16 *) extractTCBIOBuf(pTCB, 5);

    numPucch = pULPilotInput->numPUCCH;
    Msc      = pULPilotInput->Nrb_sc;

    ncs_cell   = (S16*) (DataBuff_Bank1a);
    nCs        = (S16*) (&ncs_cell[7 * NUM_SLOTS_SUBFRM]);
    r_alpha_uvCeva = (S16*) (&nCs[7 * NUM_SLOTS_SUBFRM]);
    aPilotsCeva    = (S16*) (&r_alpha_uvCeva[len_r_alpha_uv]);

    indPucch1 = pULPilotInput->numPUSCH;
    indPucch2    = pULPilotInput->numPUSCH + numPucch;
    Nul_symb     = pULPilotInput->Nul_symb;
    offset_pucch = 0;
    L = pULPilotInput->L;
    slotNum      = pULPilotInput->slotNum;

    StopAndStartSubTask(SUBTASKID_RX_ULPILOT_PUCCH_EXTRACT, SUBTASKID_RX_ULPILOT_PUCCH_DMA_IN);

    //Copy from CRAM to internal Ceva Memory
    MemCpyFromExt((void*) ncs_cell, (void*)(&pULPilotInput->Pucch_ncs_cell[slotNum * Nul_symb]), 7 * NUM_SLOTS_SUBFRM * sizeof(S16));
    for(i = 0; i < NUM_SLOTS_SUBFRM; i++)
    {
        if (pULPilotInput->groupHoppingEnabled == 1)
            uSeqGrpNum[i] = pULPilotInput->Pucch_uSeqGrpNum_GrHopping[slotNum + i];
        else
            uSeqGrpNum[i] = pULPilotInput->Pucch_uSeqGrpNum[slotNum + i];
    }
    StopSubTask(SUBTASKID_RX_ULPILOT_PUCCH_DMA_IN);

    for(i = indPucch1; i < indPucch2; i++)
    {
        numPilotSym = pULPilotInput->num_Pilots_slot[i];

        gen_ul_Pilot_info1(pULPilotInput, &nOcCeva[0], &nOcBar[0], &SdescrambCeva[0],
                                   nCs, ncs_cell, i, 0);
        gen_ul_Pilot_info2(pULPilotInput, r_alpha_uvCeva, nCs, &uSeqGrpNum[0]);

            //generate pilots and add to the resource grid
        ul_Pilot_PUCCH(aPilotsCeva, r_alpha_uvCeva, &nOcBar[0], pULPilotInput, i);

        //Copy back to CRAM
        StartSubTask(SUBTASKID_RX_ULPILOT_PUCCH_DMA_OUT);
        MemCpyToExt((void*)(&r_alpha_uv[2*i*len_r_alpha_uv]), (void*)(r_alpha_uvCeva), len_r_alpha_uv * sizeof(S16));
        MemCpyToExt((void*)(&aPilots[offset_pucch]), (void*)(aPilotsCeva), 2 * 2 * Msc * numPilotSym * sizeof(S16));
        for(j = 0; j < NUM_SLOTS_SUBFRM; j++)
        {
            Sdescramb[2*i*NUM_SLOTS_SUBFRM + j] = SdescrambCeva[j];
            nOc[2*i*NUM_SLOTS_SUBFRM + j]       = nOcCeva[j];
        }
        StopSubTask(SUBTASKID_RX_ULPILOT_PUCCH_DMA_OUT);

        if(pULPilotInput->simSRHarq[i] == 1) //
        {
            gen_ul_Pilot_info1(pULPilotInput, &nOcCeva[0], &nOcBar[0], &SdescrambCeva[0],
                               nCs, ncs_cell, i, 1);
            gen_ul_Pilot_info2(pULPilotInput, r_alpha_uvCeva, nCs, &uSeqGrpNum[0]);

            // Generate pilots and add to the resource grid
            ul_Pilot_PUCCH(aPilotsCeva, r_alpha_uvCeva, &nOcBar[0], pULPilotInput, i);

            //Copy back to CRAM
            StartSubTask(SUBTASKID_RX_ULPILOT_PUCCH_DMA_OUT);
            MemCpyToExt((void*)(&r_alpha_uv[2*i*len_r_alpha_uv + len_r_alpha_uv]),
                        (void*)(r_alpha_uvCeva), len_r_alpha_uv * sizeof(S16));
            MemCpyToExt((void*)(&aPilots[offset_pucch + Msc*2*2*numPilotSym]),
                        (void*)(aPilotsCeva), 2 * 2 * Msc * numPilotSym * sizeof(S16));
            for(j = 0; j < NUM_SLOTS_SUBFRM; j++)
            {
                Sdescramb[2*i*NUM_SLOTS_SUBFRM + NUM_SLOTS_SUBFRM + j] = SdescrambCeva[j];
                nOc[2*i*NUM_SLOTS_SUBFRM + NUM_SLOTS_SUBFRM + j]       = nOcCeva[j];
            }
            StopSubTask(SUBTASKID_RX_ULPILOT_PUCCH_DMA_OUT);
        }

        offset_pucch += 2*Msc*2*2*numPilotSym;  //2 for IQ, 2 slots, 2 for SimSR
        //////////////////////////////////////////////////////////////////////////////
        //Find where each user is located in resource grid for PUSCH
        //////////////////////////////////////////////////////////////////////////////
        bs_rx_DemuxFDMAPhysChan_fx(pULPilotInput, step_aDet, &rs_grid[i*L], i);
        //////////////////////////////////////////////////////////////////////////////
    }

    SetSubTaskLoggerCount(SUBTASKID_RX_ULPILOT_PUCCH_START, SUBTASKID_RX_ULPILOT_PUCCH_END);
    return;
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink bs_rx_ChEstimator_PUCCH_part0 Task Function.
 *

 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |bs_rx_ChEstimator_PUCCH_part0_task |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink bs_rx_ChEstimator_PUCCH_part0 function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: bs_rx_ChEstimator_PUCCH_part0() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void bs_rx_ChEstimator_PUCCH_part0_task(PTCB pTCB)
{
    ULRXINPUTPARAMS *pChanEstPar;
    S16  slot, mo, *adet, *chEst, *expo_fac, *aPilots;
    S16 Nul_symb, numPucch;
    U32 *rs_grid;
    U32* pContext = 0;
    S32 indPucch1, indPucch2, i, step_aDet;
    S32 PilotInd, PilotLen, K, slotsym;
    S32 ChanEstInd;
    S16 isym, Msc, num_pilots, L, MO;
    S16 StartRB;
    S16 *aDetCeva, *aPilotsCeva, *chEstCeva;
    S32 Length2, nn;
    S16 *pScaleComputeErr_Tab;
    S32 *pErrAvg;
    S32 *ErrSub;
    S16 *ErrSubExpo;
    S32 *tempVecErr;

    pContext     = (U32*)pTCB->ContextPtr;
    slot         = (S16) pContext[0];
    mo           = (S16) pContext[1];
    step_aDet    = (S32) pContext[2];

    pChanEstPar = (ULRXINPUTPARAMS *) extractTCBIOBuf(pTCB, 0);
    adet        = (S16 *) extractTCBIOBuf(pTCB, 1);
    rs_grid     = (U32 *) extractTCBIOBuf(pTCB, 2);
    expo_fac    = (S16 *) extractTCBIOBuf(pTCB, 3);
    aPilots     = (S16 *) extractTCBIOBuf(pTCB, 4);

    pErrAvg              = (S32 *) extractTCBIOBuf(pTCB, 5);
    pScaleComputeErr_Tab = (S16 *) extractTCBIOBuf(pTCB, 6);
    chEst                = (S16 *) extractTCBIOBuf(pTCB, 7);
    ErrSub               = (S32 *) extractTCBIOBuf(pTCB, 8);
    ErrSubExpo           = (S16 *) extractTCBIOBuf(pTCB, 9);

    Msc = pChanEstPar->Nrb_sc;
    MO  = pChanEstPar->MO;
    L   = pChanEstPar->L;
    K   = pChanEstPar->K;
    Nul_symb = pChanEstPar->Nul_symb;
    numPucch = pChanEstPar->numPUCCH;

    Length2 = 3 * Msc * 2 * 2 * MAXCHSUBFRAME * 2;

    // Allocate Memory
    tempVecErr      = (S32*) (DataBuff_Bank1a);
    aDetCeva        = (S16*) (&tempVecErr[2*27]);
    aPilotsCeva     = (S16*) (&aDetCeva[2 * Msc * Nul_symb]);
    chEstCeva       = (S16*) (&aPilotsCeva[Length2]);

    indPucch1 = pChanEstPar->numPUSCH;
    indPucch2 = pChanEstPar->numPUSCH + numPucch;

    PilotInd = 0;
    slotsym = slot*Nul_symb;
    for(i = 0; i < Nul_symb; i++)
        expo_fac[i + slotsym] = adet[2*K + (i + slotsym)*step_aDet];

    ChanEstInd = 0;
    for(i = indPucch1; i < indPucch2; i++)
    {
        num_pilots   = pChanEstPar->num_Pilots_slot[i];
        PilotLen     = 2*Msc*num_pilots;
        StartRB      = pChanEstPar->RBStart[i];

        //Copy Used REs only, to internal ceva memory space
        for (isym = 0; isym < Nul_symb; isym++) // Changed MemCpyFromExt to memcpy
            memcpy((void*)&aDetCeva[2*Msc*isym], (void*)&adet[rs_grid[isym + slotsym + L*i]], 2*Msc*sizeof(S16));

        //Copy pilots into internal ceva memory space
        for (nn = 0; nn < num_pilots; nn++) // Changed MemCpyFromExt to memcpy
            memcpy((void*)&aPilotsCeva[2*Msc*nn], (void*)&aPilots[PilotInd + slot*PilotLen + 2*Msc*nn], 2*Msc*sizeof(S16));

        bs_rx_ChEstimator_PUCCH_part0(aDetCeva, aPilotsCeva, pChanEstPar,
                                &chEstCeva[ChanEstInd], &expo_fac[slotsym], slot,  i,
                                &pErrAvg[2*4*i*MO + 4*mo + 2*slot], pScaleComputeErr_Tab, &ErrSub[2*4*i*MO + 4*mo + 2*slot],
                                &ErrSubExpo[4*i*MO + 2*mo + slot], tempVecErr);

#ifdef DEBUG_PRINT
        phy_fprintf(fChanEst_pucch, LOG_BS_CHANEST_PUCCH_P0_FX, "%%UE %d, slot %d, mo %d\n", i, slot, mo);
        filePrint(fChanEst_pucch, &chEstCeva[ChanEstInd], 2*Msc*Nul_symb, "S16", "Fix2Fl_Complex",
                    (double)(pow(2., 12.)), LOG_BS_CHANEST_PUCCH_P0_FX);
#endif


        if(pChanEstPar->simSRHarq[i] == 1) //
        {
            bs_rx_RBassignment_pucch(pChanEstPar, i, &StartRB, pChanEstPar->srPUCCHResourceIndex[i]);

            if(slot == 1)
                StartRB = pChanEstPar->Nul_rb - 1 - StartRB;

            //Copy Used REs only, to internal ceva memory space
            for (isym = 0; isym < Nul_symb; isym++) // Changed MemCpyFromExt to memcpy
                memcpy((void*)&aDetCeva[2*Msc*isym], (void*)&adet[2*Msc*StartRB + step_aDet*(isym + slotsym)], 2*Msc*sizeof(S16));

            //Copy pilots into internal ceva memory space
            for (nn = 0; nn < num_pilots; nn++) // Changed MemCpyFromExt to memcpy
                memcpy((void*)&aPilotsCeva[2*Msc*nn], (void*)&aPilots[PilotInd + slot*PilotLen + 2*2*Msc*num_pilots + 2*Msc*nn], 2*Msc*sizeof(S16));

            bs_rx_ChEstimator_PUCCH_part0(aDetCeva, aPilotsCeva, pChanEstPar,
                                 &chEstCeva[ChanEstInd + MAX_PUCCH_PILOTSYMB_PER_SUBF*Msc*2], &expo_fac[slotsym], slot,  i,
                                 &pErrAvg[2*4*i*MO + 4*mo + 2*slot + 4*MO],  pScaleComputeErr_Tab, &ErrSub[2*4*i*MO + 4*mo + 2*slot + 2*NUM_SLOTS_PER_SUBF*MO],
                                 &ErrSubExpo[4*i*MO + 2*mo + slot + NUM_SLOTS_PER_SUBF*MO], tempVecErr);

#ifdef DEBUG_PRINT
            phy_fprintf(fChanEst_pucch, LOG_BS_CHANEST_PUCCH_P0_FX, "%%UE %d, slot %d, mo %d, SR\n", i, slot, mo);
            filePrint(fChanEst_pucch, &chEstCeva[ChanEstInd + MAX_PUCCH_PILOTSYMB_PER_SUBF*Msc*2], 2*Msc*Nul_symb, "S16", "Fix2Fl_Complex",
                        (double)(pow(2., 12.)), LOG_BS_CHANEST_PUCCH_P0_FX);
#endif
        }

        ChanEstInd += 2*2*Msc*MAX_PUCCH_PILOTSYMB_PER_SUBF;
        PilotInd += 2*2*2*Msc*num_pilots;
    }

    ChanEstInd = 0;
    for(i = indPucch1; i < indPucch2; i++)
    {
        MemCpyToExt((void *) &chEst[ChanEstInd + 2*Msc*MAX_PUCCH_PILOTSYMB_PER_SLOT*slot], (void *) &chEstCeva[ChanEstInd], sizeof(S16)*Msc*2*MAX_PUCCH_PILOTSYMB_PER_SLOT);
        if(pChanEstPar->simSRHarq[i] == 1)
            MemCpyToExt((void *) &chEst[ChanEstInd + 2*Msc*MAX_PUCCH_PILOTSYMB_PER_SLOT*slot + MAX_PUCCH_PILOTSYMB_PER_SUBF*Msc*2], (void *) &chEstCeva[ChanEstInd + MAX_PUCCH_PILOTSYMB_PER_SUBF*Msc*2], sizeof(S16)*Msc*2*MAX_PUCCH_PILOTSYMB_PER_SLOT);

        ChanEstInd += 2*2*Msc*MAX_PUCCH_PILOTSYMB_PER_SUBF;
    }

    return;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink bs_rx_ChEstimator_PUCCH Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |bs_rx_ChEstimator_PUCCH_task       |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink bs_rx_ChEstimator_PUCCH function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: bs_rx_ChEstimator_PUCCH() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void bs_rx_ChEstimator_PUCCH_task(PTCB pTCB)
{
    ULRXINPUTPARAMS *pChanEstPar;
    S16  slot, mo, *adet, *chEst, *expo_fac;
    S16 Nul_symb, numPucch;
    U32 *rs_grid;
    U32* pContext = 0;
    S16 *tempVecChEst;
    S32 indPucch1, indPucch2, i, step_aDet;
    S16 *r_alpha_uv;
    S32 len_alpha_uv;
    S16 *Sdescramb;
    S16 *nOc;
    S16 srsPilotSym;
    S16 *pAvgReDet_pucchACK, *pAvgReDet_pucchCQI, *pChEst_pucch;
    S32 PilotInd, K, slotsym;
    S32 ChanEstInd, ChanEstInd1, PwrInd = 0;
    S16 isym, Msc, num_pilots, L, MO;
    S16 StartRB, tempStartRB, nPRB_slot;
    S32 pRxInPwr_Pilot[2], pRxInPwr_Data[2];
    S16 pPilot_expo[2], pData_expo[2];
    S16 pChEstPucch[2], SdescrambCeva[2], nOcCeva[2];
    S16 pAvgReDet_ACKCeva[2], *pAvgReDet_CQICeva;
    S32 *pRxInPwrAvg_PerRbPilot, *pRxInPwrAvg_PerRbDataSym;
    S16 *pPwrAvg_Pilot_expo, *pPwrAvg_Data_expo;
    S16 D_CqiSlot = NUM_CQI_DATASYMB_PER_SUBF / 2;
    S32 dEstCqiInd;
    S32 *pAllPucchPwrAvg_AntSlot,  *pPucchPwrAvg;
    S32 *allPucchPwrAvg_RB;
    S16 nPRB, pucch_format;
    S16 max_expo1, max_expo2;
    S16 *aDetCeva,  *r_alpha_uvCeva, *chEstCeva;
    S32 Length1, Length3;
    S32 *Err;
    S32 *pRxInRbPwrAvg_AntSlot;
    S32 *pRxRssi_AntSlot;
    U8 *pRbUsedFlag;
    S16 srsEnable;
    S32 pucchRssiTemp;
    S32 RssiInd;

    StartSubTask(SUBTASKID_RX_CHANEST_PUCCH_START);

    pContext     = (U32*)pTCB->ContextPtr;
    slot         = (S16) pContext[0];
    mo           = (S16) pContext[1];
    step_aDet    = (S32) pContext[2];
    srsPilotSym  = (S16) pContext[3];
    len_alpha_uv = (S32) pContext[4];

    pChanEstPar = (ULRXINPUTPARAMS *) extractTCBIOBuf(pTCB, 0);
    adet        = (S16 *) extractTCBIOBuf(pTCB, 1);
    rs_grid     = (U32 *) extractTCBIOBuf(pTCB, 2);
    expo_fac    = (S16 *) extractTCBIOBuf(pTCB, 3);
    chEst     = (S16 *) extractTCBIOBuf(pTCB, 4);
    pAvgReDet_pucchCQI       = (S16 *) extractTCBIOBuf(pTCB, 5);
    pAllPucchPwrAvg_AntSlot  = (S32 *) extractTCBIOBuf(pTCB, 6);
    pPucchPwrAvg             = (S32 *) extractTCBIOBuf(pTCB, 7);
    Sdescramb                = (S16 *) extractTCBIOBuf(pTCB, 8);
    r_alpha_uv               = (S16 *) extractTCBIOBuf(pTCB, 9);
    nOc                      = (S16 *) extractTCBIOBuf(pTCB, 10);
    pAvgReDet_pucchACK       = (S16 *) extractTCBIOBuf(pTCB, 11);
    pChEst_pucch             = (S16 *) extractTCBIOBuf(pTCB, 12);
    pRxInPwrAvg_PerRbPilot   = (S32 *) extractTCBIOBuf(pTCB, 13);
    pPwrAvg_Pilot_expo       = (S16 *) extractTCBIOBuf(pTCB, 14);
    pRxInPwrAvg_PerRbDataSym = (S32 *) extractTCBIOBuf(pTCB, 15);
    pPwrAvg_Data_expo        = (S16 *) extractTCBIOBuf(pTCB, 16);
    Err     = (S32 *) extractTCBIOBuf(pTCB, 17);
    pRxInRbPwrAvg_AntSlot        = (S32 *) extractTCBIOBuf(pTCB, 18);
    pRbUsedFlag        = (U8 *) extractTCBIOBuf(pTCB, 19);
    pRxRssi_AntSlot          = (S32 *) extractTCBIOBuf(pTCB, 20);


    Msc = pChanEstPar->Nrb_sc;
    MO  = pChanEstPar->MO;
    L   = pChanEstPar->L;
    K   = pChanEstPar->K;
    Nul_symb = pChanEstPar->Nul_symb;
    numPucch = pChanEstPar->numPUCCH;

    Length1 = 2 * len_alpha_uv * MAXCHSUBFRAME;
    Length3 = 2 * D_CqiSlot;

    StopAndStartSubTask(SUBTASKID_RX_CHANEST_PUCCH_EXTRACT, SUBTASKID_RX_CHANEST_PUCCH_DMA_IN);

    // Allocate Memory
    tempVecChEst      = (S16*) (DataBuff_Bank1a);
    aDetCeva          = (S16*) (&tempVecChEst[11 * Msc]);
    r_alpha_uvCeva    = (S16*) (&aDetCeva[2 * Msc * Nul_symb]);
    chEstCeva       = (S16*) (&r_alpha_uvCeva[Length1]);
    pAvgReDet_CQICeva = (S16*) (&chEstCeva[2*2*Msc*MAX_PUCCH_PILOTSYMB_PER_SUBF*numPucch]);
    allPucchPwrAvg_RB = (S32*) (&pAvgReDet_CQICeva[Length3]);

    indPucch1 = pChanEstPar->numPUSCH;
    indPucch2 = pChanEstPar->numPUSCH + numPucch;

    //Copy to internal Ceva memory
    MemCpyFromExt((void*)&r_alpha_uvCeva[indPucch1 * 2 * len_alpha_uv],
                  (void*)&r_alpha_uv[indPucch1 * 2 * len_alpha_uv], numPucch * 2 * len_alpha_uv * sizeof(S16));
    memset(allPucchPwrAvg_RB, 0, MAX_NUM_RB * sizeof(S32)); //index for Resource Block number
    memset(tempVecChEst, 0, 11 * Msc * sizeof(S16));
    if(slot == 0 && mo == 0)
       memset(pChanEstPar->Multi_PUCCH_PerRB, 0, MAXCHSUBFRAME * sizeof(S32));

    StopSubTask(SUBTASKID_RX_CHANEST_PUCCH_DMA_IN);

    PilotInd = 0;
    slotsym = slot*Nul_symb;
    for(i = 0; i < Nul_symb; i++)
        expo_fac[i + slotsym] = adet[2*K + (i + slotsym)*step_aDet];

    ChanEstInd = 0;
    ChanEstInd1 = 0;

    PwrInd     = 2*2*indPucch1*MO + slot + 2*mo;
    dEstCqiInd = mo * NUM_CQI_DATASYMB_PER_SUBF * 2 + slot * D_CqiSlot * 2;      //index for dEstCqiAnt[chan][ant][dsym] (2* for Re & Im)

    for(i = indPucch1; i < indPucch2; i++)
    {
        StartRB = pChanEstPar->RBStart[i];
        tempVecChEst[2*Msc + StartRB]++;
        if(pChanEstPar->simSRHarq[i] == 1) //
        {
            bs_rx_RBassignment_pucch(pChanEstPar, i, &tempStartRB, pChanEstPar->srPUCCHResourceIndex[i]);
            //if(StartRB != tempStartRB)// For same UE, make sure it is not same RB
            tempVecChEst[2*Msc + tempStartRB]++;
        }
    }

    for(i = indPucch1; i < indPucch2; i++)
    {
        num_pilots   = pChanEstPar->num_Pilots_slot[i];
        StartRB      = pChanEstPar->RBStart[i];
        pucch_format = pChanEstPar->pucchFormat[i];
        if(slot == 0 && mo == 0 && tempVecChEst[2*Msc + StartRB] == 1)
           pChanEstPar->Multi_PUCCH_PerRB[i] = 1;

        //Copy to Internal Ceva Memory
        SdescrambCeva[0] = Sdescramb[2*i*NUM_SLOTS_SUBFRM];
        SdescrambCeva[1] = Sdescramb[2*i*NUM_SLOTS_SUBFRM + 1];
        nOcCeva[0]       = nOc[2*i*NUM_SLOTS_SUBFRM];
        nOcCeva[1]       = nOc[2*i*NUM_SLOTS_SUBFRM + 1];

        //Copy Used REs only, to internal ceva memory space
        StartSubTask(SUBTASKID_RX_CHANEST_PUCCH_DMA_IN);
        for (isym = 0; isym < Nul_symb; isym++) // Changed MemCpyFromExt to memcpy
            memcpy((void*)&aDetCeva[2*Msc*isym], (void*)&adet[rs_grid[isym + slotsym + L*i]], 2*Msc*sizeof(S16));

        MemCpyFromExt((void *) &chEstCeva[ChanEstInd1], (void *) &chEst[ChanEstInd1 + 2*Msc*MAX_PUCCH_PILOTSYMB_PER_SLOT*slot], sizeof(S16)*Msc*2*MAX_PUCCH_PILOTSYMB_PER_SLOT);
        bs_rx_ChEstimator_PUCCH(aDetCeva,  pChanEstPar,
                                &chEstCeva[ChanEstInd1], &expo_fac[slotsym], slot, tempVecChEst, i,
                                &r_alpha_uvCeva[2*i*len_alpha_uv], &SdescrambCeva[0],
                                &nOcCeva[0], srsPilotSym, &pAvgReDet_ACKCeva[0],
                                &pAvgReDet_CQICeva[0], &pChEstPucch[0],
                                &pRxInPwr_Pilot[0], &pPilot_expo[0], &pRxInPwr_Data[0], &pData_expo[0], &Err[4*i], StartRB);

#ifdef DEBUG_PRINT
        phy_fprintf(fChanEst_pucch, LOG_BS_CHANEST_PUCCH_FX, "%%UE %d, slot %d, mo %d\n", i, slot, mo);
        filePrint(fChanEst_pucch, &chEstCeva[ChanEstInd1], 2*Msc*Nul_symb, "S16", "Fix2Fl_Complex",
                    (double)(pow(2., 12.)), LOG_BS_CHANEST_PUCCH_FX);
#endif

        //Copy back to CRAM
        pRxInPwrAvg_PerRbPilot[PwrInd]   = pRxInPwr_Pilot[0];
        pPwrAvg_Pilot_expo[PwrInd]       = pPilot_expo[0];
        pRxInPwrAvg_PerRbDataSym[PwrInd] = pRxInPwr_Data[0];
        pPwrAvg_Data_expo[PwrInd]        = pData_expo[0];

        pChEst_pucch[4*mo + 2*slot + ChanEstInd]           = pChEstPucch[0];
        pChEst_pucch[4*mo + 2*slot + ChanEstInd + 1]       = pChEstPucch[1];
        pAvgReDet_pucchACK[4*mo + 2*slot + ChanEstInd]     = pAvgReDet_ACKCeva[0];
        pAvgReDet_pucchACK[4*mo + 2*slot + ChanEstInd + 1] = pAvgReDet_ACKCeva[1];

        //Accumulate total power of all PUCCH channels in each RB
        AccumAllPucchChanPower(pucch_format, &pChEstPucch[0],
                               &pAvgReDet_ACKCeva[0], &pAvgReDet_CQICeva[0],
                               &allPucchPwrAvg_RB[StartRB], &pPucchPwrAvg[PwrInd]);

        if(pChanEstPar->simSRHarq[i] == 1) //
        {
            bs_rx_RBassignment_pucch(pChanEstPar, i, &StartRB, pChanEstPar->srPUCCHResourceIndex[i]);
            nPRB = StartRB; // for allPucchPwrAvg_RB
            tempStartRB = StartRB; // save it
            if(slot == 0 && mo == 0 && tempVecChEst[2*Msc + StartRB] == 1)
                pChanEstPar->Multi_PUCCH_PerRB[i] = 1;

            if(slot == 1)
                StartRB = pChanEstPar->Nul_rb - 1 - StartRB;

            //Copy to Internal Ceva Memory
            SdescrambCeva[0] = Sdescramb[2*i*NUM_SLOTS_SUBFRM + NUM_SLOTS_SUBFRM];
            SdescrambCeva[1] = Sdescramb[2*i*NUM_SLOTS_SUBFRM + NUM_SLOTS_SUBFRM + 1];
            nOcCeva[0]       = nOc[2*i*NUM_SLOTS_SUBFRM + NUM_SLOTS_SUBFRM];
            nOcCeva[1]       = nOc[2*i*NUM_SLOTS_SUBFRM + NUM_SLOTS_SUBFRM + 1];

            //Copy Used REs only, to internal ceva memory space
            StartSubTask(SUBTASKID_RX_CHANEST_PUCCH_DMA_IN);
            for (isym = 0; isym < Nul_symb; isym++) // Changed MemCpyFromExt to memcpy
                memcpy((void*)&aDetCeva[2*Msc*isym], (void*)&adet[2*Msc*StartRB + step_aDet*(isym + slotsym)], 2*Msc*sizeof(S16));

            MemCpyFromExt((void *) &chEstCeva[ChanEstInd1 + MAX_PUCCH_PILOTSYMB_PER_SUBF*Msc*2], (void *) &chEst[ChanEstInd1 + 2*Msc*MAX_PUCCH_PILOTSYMB_PER_SLOT*slot + MAX_PUCCH_PILOTSYMB_PER_SUBF*Msc*2], sizeof(S16)*Msc*2*MAX_PUCCH_PILOTSYMB_PER_SLOT);

            bs_rx_ChEstimator_PUCCH(aDetCeva,  pChanEstPar,
                                 &chEstCeva[ChanEstInd1 + MAX_PUCCH_PILOTSYMB_PER_SUBF*Msc*2], &expo_fac[slotsym], slot, tempVecChEst, i,
                                 &r_alpha_uvCeva[2*i*len_alpha_uv + len_alpha_uv],
                                 &SdescrambCeva[0], &nOcCeva[0], srsPilotSym,
                                 &pAvgReDet_ACKCeva[0], &pAvgReDet_CQICeva[0], &pChEstPucch[0],
                                 &pRxInPwr_Pilot[1], &pPilot_expo[1], &pRxInPwr_Data[1], &pData_expo[1], &Err[4*i + 2], tempStartRB);

#ifdef DEBUG_PRINT
            phy_fprintf(fChanEst_pucch, LOG_BS_CHANEST_PUCCH_FX, "%%UE %d, slot %d, mo %d, SR\n", i, slot, mo);
            filePrint(fChanEst_pucch, &chEstCeva[ChanEstInd1 + MAX_PUCCH_PILOTSYMB_PER_SUBF*Msc*2], 2*Msc*Nul_symb, "S16", "Fix2Fl_Complex",
                        (double)(pow(2., 12.)), LOG_BS_CHANEST_PUCCH_FX);
#endif
            //Copy back to CRAM
            pRxInPwrAvg_PerRbPilot[PwrInd + 2*MO]   = pRxInPwr_Pilot[1];
            pPwrAvg_Pilot_expo[PwrInd + 2*MO]       = pPilot_expo[1];
            pRxInPwrAvg_PerRbDataSym[PwrInd + 2*MO] = pRxInPwr_Data[1];
            pPwrAvg_Data_expo[PwrInd + 2*MO]        = pData_expo[1];

            pChEst_pucch[4*mo + 2*slot + ChanEstInd + 2*2*MO]           = pChEstPucch[0];
            pChEst_pucch[4*mo + 2*slot + ChanEstInd + 2*2*MO + 1]       = pChEstPucch[1];
            pAvgReDet_pucchACK[4*mo + 2*slot + ChanEstInd + 2*2*MO]     = pAvgReDet_ACKCeva[0];
            pAvgReDet_pucchACK[4*mo + 2*slot + ChanEstInd + 2*2*MO + 1] = pAvgReDet_ACKCeva[1];

            //Accumulate total power of all PUCCH channels in each RB
            AccumAllPucchChanPower(pucch_format, &pChEstPucch[0],
                               &pAvgReDet_ACKCeva[0], &pAvgReDet_CQICeva[0],
                               &allPucchPwrAvg_RB[nPRB], &pPucchPwrAvg[PwrInd + 2*MO]);
           // pChanEstPar->RBStart[i] = (U8) tempStartRB;//Retreiving original value;
        }

        //Copy back to CRAM
        StartSubTask(SUBTASKID_RX_CHANEST_PUCCH_DMA_OUT);
        MemCpyToExt((void*)(&pAvgReDet_pucchCQI[dEstCqiInd]), (void*)(&pAvgReDet_CQICeva[0]), Length3 * sizeof(S16));
        StartSubTask(SUBTASKID_RX_CHANEST_PUCCH_DMA_OUT);

        PwrInd     += 2*2*MO;
        ChanEstInd1 += 2*2*Msc*MAX_PUCCH_PILOTSYMB_PER_SUBF;
        ChanEstInd += 2*2*2*MO;

        PilotInd   += 2*2*2*Msc*num_pilots;
        dEstCqiInd += MO * NUM_CQI_DATASYMB_PER_SUBF * 2; //incrememt for next user. Each user has MO*NUM_CQI_DATASYMB_PER_SUBF complex IQ samples
    }

    //Save the final "sum of all pucch channel powers", seen in this RB (slot,ant), for this channel
    PwrInd = 2*2*indPucch1*MO + mo * NUM_SLOTS_PER_SUBF  + slot;
    RssiInd = 2*indPucch1*MO + slot + 2 * mo;
    srsEnable = (srsPilotSym!=-1);

    for(i = indPucch1; i < indPucch2; i++)
    {
        StartRB = pChanEstPar->RBStart[i];
        nPRB_slot = StartRB;
        if (slot == 1)
            nPRB_slot = pChanEstPar->Nul_rb - 1 - StartRB;


        pAllPucchPwrAvg_AntSlot[PwrInd] = allPucchPwrAvg_RB[StartRB];    //store for this channel, the final value seen in this RB
        pucch_format = pChanEstPar->pucchFormat[i];

        PucchRbNoisePower(pucch_format, srsEnable, nPRB_slot,
                          &pRxInPwrAvg_PerRbPilot[PwrInd], &pRxInPwrAvg_PerRbDataSym[PwrInd],
                          &allPucchPwrAvg_RB[StartRB],  &pRxInRbPwrAvg_AntSlot[(mo*2+slot)*MAXRESOURCEBLOCKS],
                          &pRbUsedFlag[(mo*2+slot)*MAXRESOURCEBLOCKS]);
        pRxRssi_AntSlot[RssiInd] = L_add(pRxInPwrAvg_PerRbPilot[PwrInd], pRxInRbPwrAvg_AntSlot[(mo*2+slot)*MAXRESOURCEBLOCKS+nPRB_slot]);
        if(pChanEstPar->simSRHarq[i] == 1) //
        {
            bs_rx_RBassignment_pucch(pChanEstPar, i, &nPRB, pChanEstPar->srPUCCHResourceIndex[i]);
            nPRB_slot = nPRB;
            if (slot == 1)
                nPRB_slot = pChanEstPar->Nul_rb - 1 - nPRB;

            pAllPucchPwrAvg_AntSlot[PwrInd + NUM_SLOTS_PER_SUBF*MO] = allPucchPwrAvg_RB[nPRB];    //store for this channel, the final value seen in this RB

            pucchRssiTemp = L_add(pRxInPwrAvg_PerRbPilot[PwrInd+2*MO], pRxInRbPwrAvg_AntSlot[(mo*2+slot)*MAXRESOURCEBLOCKS+nPRB_slot]);
            if (pucchRssiTemp > pRxRssi_AntSlot[RssiInd])
                pRxRssi_AntSlot[RssiInd] = pucchRssiTemp;
        }

        PwrInd  += 2*NUM_SLOTS_PER_SUBF*MO; //increment for next user. Each user has MO*NUM_SLOTS_PER_SUBF complex channel estimates, extra 2 to acomodate SR storage
        RssiInd += 2*MO;
    }

    if(slot == 0)
    {
        max_expo1 = -32768;
        for (i = 0; i < Nul_symb; i++)
        {
            if (max_expo1 < expo_fac[i])
                max_expo1 = expo_fac[i];
        }

        for (i = 0; i < Nul_symb; i++) // copied here instead of bs_rx_ChEstimator_PUCCH()
            expo_fac[i] = max_expo1;
    }
    else
    {
        max_expo2 = -32768;
        for (i = 0; i < Nul_symb; i++)
        {
            if (max_expo2 < expo_fac[i + Nul_symb])
                max_expo2 = expo_fac[i + Nul_symb];
        }

        for (i = 0; i < Nul_symb; i++) // copied here instead of bs_rx_ChEstimator_PUCCH()
            expo_fac[i + Nul_symb] = max_expo2;
    }

    SetSubTaskLoggerCount(SUBTASKID_RX_CHANEST_PUCCH_START, SUBTASKID_RX_CHANEST_PUCCH_END);
    return;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink rx_Multichan_DeModulation_Pucch Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_LARM or RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------------
 *  |Block Name          |rx_Multichan_DeModulation_Pucch_task     |
 *  ----------------------------------------------------------------
 *  |Input Stream(s)     |                                         |
 *  ----------------------------------------------------------------
 *  |Output Stream(s)    |                                         |
 *  ----------------------------------------------------------------
 *  |Config Parameter(s) |pTCB                                     |
 *  ----------------------------------------------------------------
 *  |Status Parameter(s) |pTCB                                     |
 *  ----------------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink rx_Multichan_DeModulation_Pucch function.<BR><BR>
 *  Will use this if we want to move ceva or MAP functions to the ARM to test if they are causing problems. <BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: rx_Multichan_DeModulation_Pucch() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void rx_Multichan_DeModulation_Pucch_task(PTCB pTCB)
{
    ULRXINPUTPARAMS *pRxInputPars;
    ULRXSDUPARAMS *pUlRxSduParams;
    S16 *pRxIn[RXANT_FOUR], *expo_ch, *pReDet0, *expo_reEst_norm;
    S16 *pucchChan;
    S32 *snrPerRb,  *snrPilot, PwrInd;
    S16 numPucchRun;
    S32 step_aDet, i, j;
    S8  *pDectab_20_X, *bDetSoftTemp, *pFHT_MASK;
    S16 *pDectab_20_Offset;
    U32 *pContext = 0;

    S32 Msc, isym, MO;
    S32 mo, L, K, AvgReDetInd, AvgReDetCQIInd;
    S16 *pRxIn1, *pExpo_reEst1;
    S16 *expo_norm, *pExpo_temp;
    S32 *pRxOut, *pChOut, ChanNum;

    S16 *pAvgReDet_pucchACK, *pAvgReDet_pucchCQI, *pChEst_pucch;
    S16 pucch_format;

    S32 *pRxInPwrAvg_PerRbPilot, *pRxInPwrAvg_PerRbDataSym;
    S16 *pPwrAvg_Pilot_expo, *pPwrAvg_Data_expo;
    S32 *pAllPucchPwrAvg_AntSlot;
    S16 *chEstVec;
    S32 *pPucchPwrAvg;
    S8  *pucch_by;
    S8  bDetSoftBackup[2] = {0};
    S32 dist[8];
    S16 temp11, temp21, temp12, temp22;
    S32 min_index;
    S16 StartRB, StartRB_SSR;
    S32 min_dist;

    S8 tmpDescrBuf[24];
    S8 *pDescrBuf;
    S32 *pDescrIndex;

    StartSubTask(SUBTASKID_RX_MULTICHAN_PUCCH_DEMODULATION_START);

    pContext = pTCB->ContextPtr;
    numPucchRun  = (S16)pContext[0];
    MO           = (S32)pContext[1];

    bDetSoftBackup[0] = 0;
    bDetSoftBackup[1] = 0;

    j = 0;
    for(i=0; i < MO; i++)
        pRxIn[i] = (S16 *) extractTCBIOBuf(pTCB, j++);

    expo_ch  = (S16 *) extractTCBIOBuf(pTCB, j++);
    pRxInputPars = (ULRXINPUTPARAMS *) extractTCBIOBuf(pTCB, j++);
    pUlRxSduParams = (ULRXSDUPARAMS *) extractTCBIOBuf(pTCB, j++);
    pucchChan    = (S16 *) extractTCBIOBuf(pTCB, j++);
    pAvgReDet_pucchCQI = (S16 *) extractTCBIOBuf(pTCB, j++);
    pAvgReDet_pucchACK = (S16 *) extractTCBIOBuf(pTCB, j++);
    snrPerRb   = (S32 *) extractTCBIOBuf(pTCB, j++);
    snrPilot = (S32 *) extractTCBIOBuf(pTCB, j++);
    pChEst_pucch = (S16 *) extractTCBIOBuf(pTCB, j++);
    pRxInPwrAvg_PerRbPilot   = (S32 *) extractTCBIOBuf(pTCB, j++);
    pPwrAvg_Pilot_expo       = (S16 *) extractTCBIOBuf(pTCB, j++);
    pRxInPwrAvg_PerRbDataSym = (S32 *) extractTCBIOBuf(pTCB, j++);
    pPwrAvg_Data_expo        = (S16 *) extractTCBIOBuf(pTCB, j++);
    pAllPucchPwrAvg_AntSlot   = (S32 *) extractTCBIOBuf(pTCB, j++);
    pPucchPwrAvg = (S32 *) extractTCBIOBuf(pTCB, j++);
    pDectab_20_X = (S8  *) extractTCBIOBuf(pTCB, j++);
    pDectab_20_Offset = (S16 *) extractTCBIOBuf(pTCB, j++);
    pFHT_MASK = (S8  *) extractTCBIOBuf(pTCB, j++);

    pDescrBuf = (S8  *) extractTCBIOBuf(pTCB, j++);
    pDescrIndex = (S32 *) extractTCBIOBuf(pTCB, j++);

    //////////////////////////////////////////////////////////////////////////////
    //Demodulation for PUCCH entire subframe
    //////////////////////////////////////////////////////////////////////////////
    Msc = pRxInputPars->Nrb_sc;
    L = pRxInputPars->L;
    K = pRxInputPars->K;
    MO = pRxInputPars->MO;
    step_aDet = RUP32B(K);

    pRxIn1 = (S16*) (DataBuff_Bank1a);
    pExpo_reEst1 = (S16*) (&pRxIn1[2 * MO * Msc * L * numPucchRun]);
    pExpo_temp = (S16*) (&pExpo_reEst1[MO * L]);
    expo_norm  = (S16*) (&pExpo_temp[Msc]);
    expo_reEst_norm = (S16*) (&expo_norm[L]);
    bDetSoftTemp = (S8*) (&expo_reEst_norm[L]);
    pReDet0 = (S16*) (&bDetSoftTemp[2 * Msc * L]);
    pRxOut  = (S32*) (&pReDet0[2 * Msc * L]);
    pChOut  = (S32*) (&pRxOut[2*Msc]);
    chEstVec = (S16*) (&pChOut[2*Msc]);
    pucch_by = (S8*) (&chEstVec[2*Msc]);

    for(isym = 0; isym < L; isym++)
    {
        expo_norm[isym] = -32;
        for (mo = 0; mo < MO; mo++)
        {
            pExpo_reEst1[isym*MO + mo] = pRxIn[mo][2*K + isym*step_aDet];
            if (pExpo_reEst1[isym*MO + mo] > expo_norm[isym])
                expo_norm[isym] = pExpo_reEst1[isym*MO + mo];
        }
    }

    StopSubTask(SUBTASKID_RX_MULTICHAN_PUCCH_DEMODULATION_EXTRACT);

    for(i = 0; i < numPucchRun; i++)
    {
        ChanNum = pucchChan[i];

        StartSubTask(SUBTASKID_RX_MULTICHAN_PUCCH_DEMODULATION_DMA_IN);

        MemCpyFromExt((void*) tmpDescrBuf, (void*) &pDescrBuf[pDescrIndex[ChanNum]], 24);

        StopSubTask(SUBTASKID_RX_MULTICHAN_PUCCH_DEMODULATION_DMA_IN);

        pRxInputPars->srdetected[ChanNum] = 0;
        pucch_format = pRxInputPars->pucchFormat[ChanNum];
        StartRB = pRxInputPars->RBStart[ChanNum];
        AvgReDetInd = 2*(ChanNum - pRxInputPars->numPUSCH) *2*2*MO;
        AvgReDetCQIInd = (ChanNum - pRxInputPars->numPUSCH) *NUM_CQI_DATASYMB_PER_SUBF*2*MO;

        PwrInd = 2*2*ChanNum*MO;

        rx_Multichan_DeModulation_Pucch(&pAvgReDet_pucchACK[AvgReDetInd], &pAvgReDet_pucchCQI[AvgReDetCQIInd],
                                    &pChEst_pucch[AvgReDetInd], pRxOut, pChOut, pReDet0, expo_ch, pExpo_temp,
                                    expo_reEst_norm, tmpDescrBuf, bDetSoftTemp, pRxInputPars,
                                    &pRxInPwrAvg_PerRbPilot[PwrInd], &pPwrAvg_Pilot_expo[PwrInd],
                                    &pRxInPwrAvg_PerRbDataSym[PwrInd], &pPwrAvg_Data_expo[PwrInd],
                                    ChanNum, StartRB, snrPerRb, &snrPilot[2 * MO * ChanNum * 2],
                                    &pAllPucchPwrAvg_AntSlot[PwrInd], chEstVec, &pPucchPwrAvg[PwrInd]);

        if(pucch_format < FORMAT2)
        {
            bDetSoftBackup[0] = bDetSoftTemp[0];
            bDetSoftBackup[1] = bDetSoftTemp[1];
        }

        if(pRxInputPars->simSRHarq[ChanNum] == 1) //
        {
            bs_rx_RBassignment_pucch(pRxInputPars, ChanNum, &StartRB_SSR, pRxInputPars->srPUCCHResourceIndex[ChanNum]);
            rx_Multichan_DeModulation_Pucch(&pAvgReDet_pucchACK[AvgReDetInd + 2*2*MO], &pAvgReDet_pucchCQI[AvgReDetCQIInd],
                                    &pChEst_pucch[AvgReDetInd + 2*2*MO], pRxOut, pChOut, pReDet0,
                                    expo_ch, pExpo_temp, expo_reEst_norm, tmpDescrBuf, bDetSoftTemp, pRxInputPars,
                                    &pRxInPwrAvg_PerRbPilot[PwrInd + 2*MO], &pPwrAvg_Pilot_expo[PwrInd + 2*MO],
                                    &pRxInPwrAvg_PerRbDataSym[PwrInd + 2*MO], &pPwrAvg_Data_expo[PwrInd + 2*MO],
                                    ChanNum,  StartRB_SSR, snrPerRb,
                                    &snrPilot[2 * MO * (ChanNum) * 2 + 2*MO],
                                    &pAllPucchPwrAvg_AntSlot[PwrInd + NUM_SLOTS_PER_SUBF*MO], chEstVec, &pPucchPwrAvg[PwrInd + NUM_SLOTS_PER_SUBF*MO]);

            //For format1a/1b RB
            temp11 = negate((S16) bDetSoftBackup[0]);
            temp12 = negate((S16) bDetSoftBackup[1]);

            //For SR RB
            temp21 = negate((S16)bDetSoftTemp[0]);
            temp22 = negate((S16)bDetSoftTemp[1]);
            if(pucch_format == FORMAT1A)
            {
                //Compute distance to closest constellation points
                dist[0] = abs_s( temp11 + 64); // 64 is +/-1 in constellation
                if(bDetSoftBackup[0] < 0)
                    dist[0] = abs_s(temp11 - 64);

                dist[1] = abs_s(temp21+ 64);
                if(bDetSoftTemp[0] < 0)
                    dist[1] = abs_s(temp21 - 64);

                if(dist[0] < dist[1]) // pick closest to
                    bDetSoftTemp[0] = bDetSoftBackup[0];
                else
                {
                    pRxInputPars->srdetected[ChanNum] = 1;
                    for(mo = 0; mo < NUM_SLOTS_SUBFRM*MO; mo++)
                        snrPilot[2 * MO * (ChanNum) * 2 + mo] = snrPilot[2 * MO * (ChanNum) * 2 + 2*MO + mo];
                }
            }
            else if(pucch_format == FORMAT1B)
            {
                // 4 constellation points, Mean-square distance
                dist[0] =  (temp11 - 91)*(temp11 - 91) +  (temp12 - 91)*(temp12 - 91); // 91 is +/- 1/sqrt(2) in constellation
                dist[1] =  (temp11 - 91)*(temp11 - 91) +  (temp12 + 91)*(temp12 + 91);
                dist[2] =  (temp11 + 91)*(temp11 + 91) +  (temp12 - 91)*(temp12 - 91);
                dist[3] =  (temp11 + 91)*(temp11 + 91) +  (temp12 + 91)*(temp12 + 91);

                dist[4] =  (temp21 - 91)*(temp21 - 91) +  (temp22 - 91)*(temp22 - 91);
                dist[5] =  (temp21 - 91)*(temp21 - 91) +  (temp22 + 91)*(temp22 + 91);
                dist[6] =  (temp21 + 91)*(temp21 + 91) +  (temp22 - 91)*(temp22 - 91);
                dist[7] =  (temp21 + 91)*(temp21 + 91) +  (temp22 + 91)*(temp22 + 91);

                min_index = 0;
                min_dist = dist[0];
                for (j = 1; j < 8; j++)
                {
                   if(dist[j] < min_dist)
                   {
                        min_index = j;
                        min_dist = dist[j];
                   }

                }

                if(min_index < 4) // pick smallest distance to contellation
                {
                    bDetSoftTemp[0] = bDetSoftBackup[0];
                    bDetSoftTemp[1] = bDetSoftBackup[1];
                }
                else
                {
                    pRxInputPars->srdetected[ChanNum] = 1;
                    for(mo = 0; mo < NUM_SLOTS_SUBFRM*MO; mo++)
                        snrPilot[2 * MO * (ChanNum) * 2 + mo] = snrPilot[2 * MO * (ChanNum) * 2 + 2*MO + mo];
                }
            }
        }//simSRHarq

        if (pucch_format == FORMAT1)
        {
            if (bDetSoftTemp[0])
                pRxInputPars->srdetected[ChanNum] = 1;
        }

        BsRx_PucchSduProcessing(ChanNum, bDetSoftTemp, pRxInputPars, pUlRxSduParams,
                                pucch_by, pDectab_20_X, pDectab_20_Offset, pFHT_MASK);
    }//for numPucchRun

    SetSubTaskLoggerCount(SUBTASKID_RX_MULTICHAN_PUCCH_DEMODULATION_START, SUBTASKID_RX_MULTICHAN_PUCCH_DEMODULATION_END);
    return;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// END PUCCH Tasks
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// PRACH Tasks
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void bs_rx_prach_task(PTCB pTask)
{
    U32 *pContext;
    S16 MO;
    S32 length_adcPrach;
    S16 *PRACH_Tables;
    BS_RX *bsRx;
    BS_RX_PRACH *bsRxPrach;
    BS_RX_PRACHDet *bsRxPrachDet;
    U32 adcbufPrach[8], j;
    BS_PRACH *pbs_prach;
    S16 *zczLUT;
    U16 prachDetThreashold;

    StartSubTask(SUBTASKID_RX_PRACH_START);

    pContext = (U32*) pTask->ContextPtr;

    MO = (S16)(pContext[0]);
    length_adcPrach = (S32)(pContext[1]);
    prachDetThreashold = (U16)pContext[2];

    j = 0;
    bsRx = (BS_RX *) extractTCBIOBuf(pTask, j++);
    bsRxPrach = (BS_RX_PRACH *) extractTCBIOBuf(pTask, j++);
    bsRxPrachDet = (BS_RX_PRACHDet *) extractTCBIOBuf(pTask, j++);
    PRACH_Tables = (S16 *) extractTCBIOBuf(pTask, j++);
    pbs_prach = (BS_PRACH *) extractTCBIOBuf(pTask, j++);
    zczLUT = (S16 *) extractTCBIOBuf(pTask, j++);
    adcbufPrach[0] = (U32)((S16 *) extractTCBIOBuf(pTask, j++));
    adcbufPrach[1] = (U32)((S16 *) extractTCBIOBuf(pTask, j++));
    adcbufPrach[2] = (U32)((S16 *) extractTCBIOBuf(pTask, j++));
    adcbufPrach[3] = (U32)((S16 *) extractTCBIOBuf(pTask, j++));

    StopSubTask(SUBTASKID_RX_PRACH_EXTRACT);

    PRACH_Rx(bsRx, bsRxPrach, bsRxPrachDet, MO, (S16 **) adcbufPrach, length_adcPrach,
             PRACH_Tables, pbs_prach, zczLUT, prachDetThreashold);

    SetSubTaskLoggerCount(SUBTASKID_RX_PRACH_START, SUBTASKID_RX_PRACH_END);
    return;
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// END PRACH Tasks
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// SRS Tasks in Ceva
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink SRSReceiverProcessing Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |SRSReceiverProcessing_task         |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink SRSReceiverProcessing function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: SRSReceiverProcessing() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void SRSReceiverProcessing_task(PTCB pTCB)
{
    PIOParams_SRS pContext = 0;

    S16 df, Nrb_sc, numSrs, MO;
    S16 NusedE, Nfft, srsBandwidthTab, srsBandwidthConfig;
    S16 K, L, Nul_rb, srsPilotSymbol;
    U32 Fs;
    S32 stepsize_aDet, index;

    PBS_RX_CHAN_SRS pBsRxChanSrs;

    S16 *aDet, *psrsBandwidthConfigTable;
    S16 *pScaleComputeErr_Tab;
    S32 *pSnrPerRb;
    S16 *pSrsChEst, *pRawSrsChEst;
    S16 *pSrsPilots;
    PBSRXCHMeas pSrsChMeas;
    S16 *pTable_DFT[5];
    S16 frameNumber;
    S16 subframeNumber;

    S16 *length_srsPilots;
    PBS_RX_CHAN_SRS pBsRxChanSrsCv;
    S16 *length_srsPilotsCv, *cdmEnableCv, *transmissionCombCv, *freqDomainPositionCv,  *srsConfigIndexCv;
    S16 *cyclicShiftCv, *bandwidthCv, *nCs_srsCv, *hoppingBandwidthCv, *cdmGroupIndexCv;
    S16 *temp_vecSrsCv;

    StartSubTask(SUBTASKID_RX_SRS_START);

    // Extract Context
    pContext = (PIOParams_SRS) pTCB->ContextPtr;
    df = pContext->df;
    Fs = pContext->Fs;
    Nrb_sc = pContext->Nrb_sc;
    NusedE = pContext->NusedE;
    Nfft = pContext->Nfft;
    numSrs = pContext->numSrs;
    MO = pContext->mo;
    K = pContext->K;
    L = pContext->L;
    Nul_rb = pContext->Nul_rb;
    srsPilotSymbol = pContext->srsPilotSymbol;
    stepsize_aDet = pContext->stepsize_aDet;
    srsBandwidthTab = pContext->srsBandwidthTab;
    srsBandwidthConfig = pContext->srsBandwidthConfig;
    frameNumber = pContext->frameNumber;
    subframeNumber = pContext->subframeNumber;

    // Extract IO Buffers
    index = 0;
    pSrsChEst      = (S16*) extractTCBIOBuf(pTCB, index++);
    pRawSrsChEst   = (S16*) extractTCBIOBuf(pTCB, index++);

    aDet           = (S16*) extractTCBIOBuf(pTCB, index++);
    pSrsPilots     = (S16*) extractTCBIOBuf(pTCB, index++);

    pBsRxChanSrs     = (PBS_RX_CHAN_SRS) extractTCBIOBuf(pTCB, index++);
    length_srsPilots = (S16*) extractTCBIOBuf(pTCB, index++);

    pScaleComputeErr_Tab     = (S16*) extractTCBIOBuf(pTCB, index++);
    psrsBandwidthConfigTable = (S16*) extractTCBIOBuf(pTCB, index++);
    pSnrPerRb                = (S32*) extractTCBIOBuf(pTCB, index++);

    pSrsChMeas               = (PBSRXCHMeas) extractTCBIOBuf(pTCB, index++);
    pTable_DFT[0] =  ((S16*) extractTCBIOBuf(pTCB, index++));
    pTable_DFT[1] =  ((S16*) extractTCBIOBuf(pTCB, index++));
    pTable_DFT[2] =  ((S16*) extractTCBIOBuf(pTCB, index++));
    pTable_DFT[3] =  ((S16*) extractTCBIOBuf(pTCB, index++));
    pTable_DFT[4] =  ((S16*) extractTCBIOBuf(pTCB, index++));

    // Setup Memory Pointers and DMA Data if Needed
    pBsRxChanSrsCv = (PBS_RX_CHAN_SRS) (&DataBuff_Bank1[0]);
    transmissionCombCv = (S16*)pBsRxChanSrsCv->transmissionComb;
    freqDomainPositionCv = (S16*)pBsRxChanSrsCv->freqDomainPosition;
    hoppingBandwidthCv = (S16*)pBsRxChanSrsCv->hoppingBandwidth;
    cdmEnableCv = (S16*)pBsRxChanSrsCv->cdmEnable;
    cdmGroupIndexCv = (S16*)pBsRxChanSrsCv->cdmGroupIndex;
    bandwidthCv = (S16*)pBsRxChanSrsCv->bandwidth;
    srsConfigIndexCv = (S16*)pBsRxChanSrsCv->configIndex;
    cyclicShiftCv = (S16*)pBsRxChanSrsCv->cyclicShift;
    length_srsPilotsCv = (S16*)&pBsRxChanSrsCv[1];
    nCs_srsCv = (S16*)&length_srsPilotsCv[MAX_UL_SRS];
    temp_vecSrsCv = (S16*)&nCs_srsCv[MAX_UL_SRS];

    memcpy((void*) pBsRxChanSrsCv, (void*) pBsRxChanSrs, sizeof(BS_RX_CHAN_SRS));

    StopSubTask(SUBTASKID_RX_SRS_EXTRACT);

    // Run the Function
    SRSReceiverProcessing(df, Fs, Nrb_sc, NusedE, Nfft, srsBandwidthTab,  srsBandwidthConfig,
                            psrsBandwidthConfigTable, aDet, numSrs, MO, K, L, Nul_rb, srsPilotSymbol,
                            pSrsChEst, pRawSrsChEst, pSrsPilots, length_srsPilots,
                            stepsize_aDet, cyclicShiftCv, bandwidthCv, transmissionCombCv,
                            freqDomainPositionCv, hoppingBandwidthCv, cdmEnableCv, cdmGroupIndexCv,
                            temp_vecSrsCv, pScaleComputeErr_Tab, pSnrPerRb,(S16*)pBsRxChanSrsCv->StartRb,
                            (S16*)pBsRxChanSrsCv->EndRb, pSrsChMeas, (S16**) pTable_DFT, srsConfigIndexCv,
                            frameNumber, subframeNumber);

    // DMA Data out
    memcpy((void*) pBsRxChanSrs, (void*) pBsRxChanSrsCv, sizeof(BS_RX_CHAN_SRS));

    SetSubTaskLoggerCount(SUBTASKID_RX_SRS_START, SUBTASKID_RX_SRS_END);
    return;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink bs_rx_Pilot_srs Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |bs_rx_Pilot_srs_task               |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink bs_rx_Pilot_srs function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: bs_rx_Pilot_srs() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void bs_rx_Pilot_srs_task(PTCB pTCB)
{
    PIOParams_SRS_Pilots pContext;
    S16 *srsPilots, *m_srs, *N, *nCs_srs, *bandwidth;
    S16 *pUlPilots_QthRootTable;
    S16 *u, *v;

    S16 groupHoppingEnabled, sequenceHoppingEnabled;
    S16 delta_ss;
    S16 numSrs;
    S16 subframeNumber;
    S16 Nrb_sc;
    U32 Ncell_id;

    S16 subframe = 0;
    S16 *uCv, *vCv;
    S16 *srsPilotsCv, *m_srsCv, *nCs_srsCv, *bandwidthCv;
    S16 *fghCv, *NCv;
    U8 *cvecCv;
    S16 index1, index2;
    S16 Nused;

    StartSubTask(SUBTASKID_RX_SRS_PILOTS_START);

    // Extract Context
    pContext = (PIOParams_SRS_Pilots) pTCB->ContextPtr;
    subframeNumber = pContext->subframeNumber;
    Nrb_sc = pContext->Nrb_sc;
    groupHoppingEnabled = pContext->groupHoppingEnabled;
    sequenceHoppingEnabled = pContext->sequenceHoppingEnabled;
    Ncell_id = pContext->Ncell_id;
    numSrs = pContext->numSrs;
    delta_ss = pContext->delta_ss;

    // Extract IO Buffers
    srsPilots = (S16 *) extractTCBIOBuf(pTCB, 0);
    u = (S16 *) extractTCBIOBuf(pTCB, 1);
    m_srs = (S16 *) extractTCBIOBuf(pTCB, 2);
    N = (S16 *) extractTCBIOBuf(pTCB, 3);
    nCs_srs = (S16 *) extractTCBIOBuf(pTCB, 4);
    bandwidth = (S16 *) extractTCBIOBuf(pTCB, 5);
    v = (S16 *) extractTCBIOBuf(pTCB, 6);
    pUlPilots_QthRootTable = (S16 *) extractTCBIOBuf(pTCB, 7);

    Nused = pContext->Nused;

    // Setup Memory Pointers and DMA Data if Needed
    cvecCv = (U8*) (&DataBuff_Bank1[0]);
    index1 = RUP32B(8*7*NUM_SLOTS);
    fghCv = (S16*) (&cvecCv[index1]);

    index1 = RUP32B(2 * NUM_SLOTS);
    uCv = (S16*) (&fghCv[index1]);

    vCv = (S16*) (&uCv[index1]);

    index1 = RUP32B(2 * NUM_SLOTS *  2 * numSrs);

    m_srsCv = (S16*) (&vCv[index1]);
    index1 = RUP32B(2 * numSrs * 4 * 2);
    NCv = (S16*) (&m_srsCv[index1]);
    nCs_srsCv = (S16*) (&NCv[index1]);
    index1 = RUP32B(2 * numSrs);
    bandwidthCv = (S16*) (&nCs_srsCv[index1]);
    srsPilotsCv = (S16*) (&bandwidthCv[index1]);

    StopAndStartSubTask(SUBTASKID_RX_SRS_PILOTS_EXTRACT, SUBTASKID_RX_SRS_PILOTS_DMA_IN);

    MemCpyFromExt((void*) m_srsCv, (void*) m_srs, 4 *  2 * 2);
    MemCpyFromExt((void*) NCv, (void*) N, 4 *  2 * 2);
    MemCpyFromExt((void*) uCv, (void*) u, NUM_SLOTS * 2);
    MemCpyFromExt((void*) vCv, (void*) v, NUM_SLOTS * 2 * numSrs);
    MemCpyFromExt((void*) nCs_srsCv, (void*) nCs_srs, numSrs  *  2);
    MemCpyFromExt((void*) bandwidthCv, (void*) bandwidth, numSrs  *  2);

    StopSubTask(SUBTASKID_RX_SRS_PILOTS_DMA_IN);

    // Run the Function
    bs_rx_Pilot_srs(subframeNumber, Nrb_sc, Ncell_id, groupHoppingEnabled, sequenceHoppingEnabled,
                   uCv, vCv, delta_ss, srsPilotsCv, m_srsCv, NCv, nCs_srsCv, bandwidthCv,
                   numSrs, pUlPilots_QthRootTable, cvecCv, fghCv);

    // DMA Data out
    StartSubTask(SUBTASKID_RX_SRS_PILOTS_DMA_OUT);

    index1 = 2 * NUM_SLOTS;
    index2 = 2 * Nused * numSrs;
    //index2 = 2 * 48 * numSrs;
    MemCpyToExt((void *) srsPilots, (void *) srsPilotsCv, index2);

    if ((subframe % 10) == 0)
    {
       MemCpyToExt((void *) u, (void *) uCv, index1);
       MemCpyToExt((void *) v, (void *) vCv, index1);
    }

    StopSubTask(SUBTASKID_RX_SRS_PILOTS_DMA_OUT);

    SetSubTaskLoggerCount(SUBTASKID_RX_SRS_PILOTS_START, SUBTASKID_RX_SRS_PILOTS_END);
    return;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink SRSSnrReporting Task Function.
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTCB                 |          |pInput/pOutput|          |structure     |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |SRSSnrReporting_task               |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |pTCB                               |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  This function will run the TCB created in tcb_wrap.c for the LTE Uplink SRSSnrReporting function.<BR><BR>
 *
 *  For a detailed description of the input parameters to this function, look under the
 *  "called" function: SRSSnrReporting() <BR><BR>
 *
 *  For a detailed description of TCBs and how to create them, please look under the
 *  Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide <BR><BR>
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void SRSSnrReporting_task(PTCB pTCB)
{
    S16 MO, numSrs, Mrb;
    S16 *StartRb, *EndRb;
    S32 *snrPerRb;
    U8 *rxV_snrPerRB;
    U32 *pContext;

    StartSubTask(SUBTASKID_RX_SRS_START);

    // Extract Context
    pContext = (U32*) pTCB->ContextPtr;
    MO     = (S16)pContext[0];
    Mrb    = (S16)pContext[1];
    numSrs = (S16)pContext[2];

    // Extract IO Buffers
    StartRb      = (S16*) extractTCBIOBuf(pTCB, 0);
    EndRb        = (S16*) extractTCBIOBuf(pTCB, 1);
    snrPerRb     = (S32*) extractTCBIOBuf(pTCB, 2);
    rxV_snrPerRB = (U8*)  extractTCBIOBuf(pTCB, 3);

    StopSubTask(SUBTASKID_RX_SRS_EXTRACT);

    // Run the Function
    SRSSnrReporting(MO, numSrs, Mrb, StartRb, EndRb, snrPerRb, rxV_snrPerRB);

    SetSubTaskLoggerCount(SUBTASKID_RX_SRS_SNR_START, SUBTASKID_RX_SRS_SNR_END);
    return;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// End SRS Tasks
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
// Uplink Task Functions
//----------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------
// Downlink FEC setup CEVA task
//----------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////
#ifdef CEVA_INTRINSICS

#ifdef EVM
#include "..\ARM\Prj\LtePhyRt\LteBsTxDataSdu.h"

extern void ceva_uart_printf(const char *fmt, ...) CEVAEXTERN;

#if defined(EVM) && defined(TEST_DLFEC_PROTECTION)
static U32 fec_reset_counter_dl = 0;
#endif

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Downlink LteBsTxSduProcessingRT
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |numPdschSdu          |          |Input         |1         |Real          |(32:0:0)|Full Range  |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |devX7                |          |Input         |1         |Real          |(32:0:0)|Full Range  |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |scrInit              |          |Input         |1         |Real          |(32:0:0)|Full Range  |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |armCB                |          |Input         |1         |Real          |(32:0:0)|Full Range  |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |LteBsTxSduProcessingRT               |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |numPdschSdu, devX7, scrInit, armCB |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |                                   |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Ceva version of the LteBsTxSduProcessingRT.   Called directly from ARM via REX scheduler.
 *
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
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
    RMParameters *prm, prm_minus, prm_plus, prm_ref;
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
    U32 *Table_DL_FEC_CTC_CB;
    S32 *UE_NIR_Table;
    S16 *blkSizeTab;

#if defined(EVM) && defined(TEST_DLFEC_PROTECTION)
    fec_reset_counter_dl++;
#endif

    numPdschSdu = pSduDesc->numSDU;
    devX7 = pSduDesc->devX7;
    scrInit = pSduDesc->scrInit;
    armCB = pSduDesc->armCB;
    Table_DL_FEC_CTC_CB = pSduDesc->pTable_DL_FEC_CTC_CB;
    UE_NIR_Table = pSduDesc->pUE_NIR_Table;
    blkSizeTab = (S16*)BlkSizTab;

    if (InitializeDriver(MDMA_SYS))
        return 1;

    if (InitializeDriver(FEC_DL))
        return 2;

    scrInPtr[0] = pSduDesc->ScrInBufs[0];
    scrInPtr[1] = pSduDesc->ScrInBufs[1];

    pMdmaScr = MdmaDrvAllocDesc(MDMA_HW_SYS, numPdschSdu);
    pSduDesc->pMdmaScrStart = pMdmaScr;

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
            k = Table_DL_FEC_CTC_CB[numBytes[countSdu]-1];

            //ceva_uart_printf ("C: k:%x,i:%x\r\n", k, pSduDesc->num_in_bytes[countSdu]-1);

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
                lte_code_blk_seg(numBytes[countSdu] * 8 + NUM_PARITY,
                    &C_num_code_blocks, &Cplus, &Cminus, &Kplus, &Kminus, &FillerBits, blkSizeTab);

                //ceva_uart_printf ("@:{%x,%x,%x,%x,%x,%x}\r\n",
                //    C_num_code_blocks, Cplus, Cminus, Kplus, Kminus, FillerBits
                //);
            }

            // Initialize number of output bytes
            noutbits = 0;

            crcsel = 0;
            if (C_num_code_blocks > 1)
            {
                crcsel = 2;
            }

            pFecDl = FecDlDrvAllocDesc(C_num_code_blocks);


            // set hardware context
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

            //ceva_uart_printf ("Out:%x, Ch:%x, Ptr:%x", pOut, pChan, pFecTxOut);

            // NIR = Nsoft/8 for UE category
    //        NIR = UE_NIR_Table[5-1];
            NIR = 458400;

            // If UE is configured for modes 3 or 4, then NIR = NIR/2
    //        if(pChan->chi.numCodeWords > 1)
    //        {
    //            NIR = NIR >> 1;
    //        }
            rvIdx = pTxInputParams->rV0[chanNum];
            if(cwId[countSdu] != 0)
                rvIdx = pTxInputParams->rV1[chanNum];

            //*********************************************************************
            // If illegal transport block size, then
            if(k == 0)
            {
                prm = &prm_minus;
                // Compute rate matching parameters E, k0 and Ncb.
                Compute_Rate_Matching_Parameters(Kminus, C_num_code_blocks, NL, fecInOutBits, Qm, 0, NIR, prm);

                prm = &prm_plus;
                // Compute rate matching parameters E, k0 and Ncb.
                Compute_Rate_Matching_Parameters(Kplus, C_num_code_blocks, NL, fecInOutBits, Qm, 0, NIR, prm);

                // Allocate temporary space for rate matching parameters
                for (k = 0; k < C_num_code_blocks; k++)
                {
                    prm = &prm_plus;
                    K = Kplus;
                    if (k < Cminus)
                    {
                        prm = &prm_minus;
                        K = Kminus;
                    }

                    // Compute rate matching parameters E, k0 and Ncb.
                    // Compute_Rate_Matching_Parameters(K, C_num_code_blocks, NL, pChan->fecInOutBits[cwId[countSdu]], Qm, k, NIR, prm);

                    ninbytes[k] = (K - FillerBits) >> 3;

                    // If X7 device, then use Ncb-32 for rate matching buffer length
                    if(devX7)
                    {
                        prm->Ncb = prm->Ncb - 32;
                    }

                    if(k > prm->NumCodeBlockUseE) prm->E = prm->E2;

    //            ceva_uart_printf ("@1:{%x,%x,%x,%x}\r\n",
    //                prm->Ncb, prm->E, prm->E2, prm->k0[pChan->chi.subChInfo[cwId[countSdu]].rV]
    //            );


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

                    FecDlDrvSetHwCtx(q, hw_ctx);

                    //ceva_uart_printf ("k=%d(Out:%x, num=%d)\r\n", k, pOut, noutbytes);

#if !(defined(EVM) && defined(TEST_DLFEC_PROTECTION))
                    FecDlDrvSetData(q, pInp, ninbytes[k], pOut, noutbytes);
#else
                    if(fec_reset_counter_dl % 15 == 14)
                    {
                        FecDlDrvSetData(q, pInp, 0, pOut, noutbytes);
                    }
                    else
                        FecDlDrvSetData(q, pInp, ninbytes[k], pOut, noutbytes);
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
                prm = &prm_ref;
                // Compute rate matching parameters E, k0 and Ncb.
                Compute_Rate_Matching_Parameters(K, C_num_code_blocks, NL, fecInOutBits, Qm, 0, NIR, prm);

                // If X7 device, then use Ncb-32 for rate matching buffer length
                if(devX7)
                {
                    prm->Ncb = prm->Ncb - 32;
                }

    //            ceva_uart_printf ("@2:{%x,%x,%x,%x}\r\n",
    //                prm->Ncb, prm->E, prm->E2, prm->k0[pChan->chi.subChInfo[cwId[countSdu]].rV]
    //            );

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

                    FecDlDrvSetHwCtx(q, hw_ctx);

#if !(defined(EVM) && defined(TEST_DLFEC_PROTECTION))
                    FecDlDrvSetData(q, pInp, n, pOut, noutbytes);
#else
                    if(fec_reset_counter_dl % 15 == 14)
                    {
                        FecDlDrvSetData(q, pInp, 0, pOut, noutbytes);
                    }
                    else
                        FecDlDrvSetData(q, pInp, n, pOut, noutbytes);
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
                length_s1 = noutbits / 6;
                if( (noutbits - length_s1*6) > 0 )
                    length_s1 += 1;
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

            MdmaDrvSet4GSCR(pMdmaScr, n, 1, packMode, 0);

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
    pFecDlPrev->CbDone = (void(*)(void*))armCB;
    pFecDlPrev->CbContext = (LPVOID) pSduDesc;

    MdmaDrvSetLastDescArm(pSduDesc->pMdmaScrStart, (MdmaCbDoneType)armCB, pSduDesc);

    if (pSduDesc->dlCrcDone)
    {
        pSduDesc->sduState = SDU_FEC_DONE;
        pSduDesc->t_fec = MxGetTicks();

        FecDlDrvRunQ(pSduDesc->pFecDlStart, TIME_TX_FEC);

        pSduDesc->dlCrcDone = 0;
    }

    return 0;
}


#endif  //EVM
#endif  //CEVA_INTRINSICS


#if defined(EVM) || defined(RT_API_PROC)

#if defined(EVM) && defined(TEST_MDMA_PROTECTION)
static U32 mdma_count = 0;
#endif

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Downlink LteBsTxStartCRC_RT
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pSduDescIn           |          |Input         |1         |Real          |(32:0:0)|Full Range  |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |LteBsTxStartCRC_RT               |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |pSduDescIn                         |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |                                   |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Ceva version of the LteBsTxStartCRC.   Called directly from ARM via REX scheduler.  Will invoke
 *  CRC processing via MDMA from the Ceva instead of having ARM do it.
 *
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
U32 LteBsTxStartCRC_RT(U32 pSduDescIn)
{
    U32 countSdu;
    PLTE_PHY_SDU_DESC pSduDesc = (PLTE_PHY_SDU_DESC)pSduDescIn;
    PMDMA_FDESC pMdma, pMdmaStart = NULL;
    U8 *pDlCrcOutBuf = pSduDesc->CRC24A_buf;

    if (InitializeDriver(MDMA_SYS))
        return 1;

    pMdma = MdmaDrvAllocDesc (MDMA_HW_SYS, pSduDesc->numSDU);
    pMdmaStart = pMdma;
    _ASSERT_PTR (pMdma);

    for(countSdu = 0; countSdu < pSduDesc->numSDU; countSdu++)
    {
        pSduDesc->CRC24A_output_array[countSdu] = pDlCrcOutBuf;

        MdmaDrvSetCRC(pMdma, 0, 0x864CFB, 1);
        pMdma->IoBdesc[1].BPtr = pSduDesc->inDatPtr[countSdu];
        pMdma->IoBdesc[1].BCtrl = BCTRL_BLAST_MASK | pSduDesc->num_in_bytes[countSdu];
        pMdma->IoBdesc[0].BPtr = (U32)pSduDesc->CRC24A_output_array[countSdu];
        pMdma->IoBdesc[0].BCtrl = BCTRL_BLAST_MASK | (pSduDesc->num_in_bytes[countSdu] + 3);

        pDlCrcOutBuf += RUP32B(pSduDesc->num_in_bytes[countSdu] + 3);
        pMdma = pMdma->Next;
    }

    pSduDesc->t_mdma = MxGetTicks();
    pSduDesc->sduState = SDU_MDMA_DONE;

#ifdef EVM
    MdmaDrvSetLastDescArm(pMdmaStart, (MdmaCbDoneType)pSduDesc->armCB, pSduDesc);

#ifdef TEST_MDMA_PROTECTION
    if(mdma_count++ % 17 == 16)
    {
        pMdma = pMdmaStart;
        while(pMdma->Next != 0)
        {
            if(pMdma->Next == 0)
                break;
            pMdma = pMdma->Next;
        }
        pMdma->HwContext.IRQEN = 0;
    }
#endif

    MdmaDrvRunQ(MDMA_HW_SYS, pMdmaStart, 200);
#else
    MdmaRunExQ (MDMA_HW_SYS, pMdmaStart, (MdmaCbDoneType)pSduDesc->armCB, pSduDesc, 200);
#endif

    return 0;
}
#endif  // defined(EVM) || defined(RT_API_PROC)


#if defined (_WIN32) || defined (EVM) || defined (RT_MODE)

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink LteBsRxVectorProcessingRT
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                     | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                         | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |ceva_rxvectorproc_params |          |Input/Ouput   |1         |Pointer       |        |Full Range  |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |LteBsRxVectorProcessingRT          |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |ceva_rxvectorproc_params            |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |                                   |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Ceva version of the LteBsRxVectorProcessingRT.   Called directly from ARM via REX scheduler.
 *
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
U32 LteBsRxVectorProcessingRT(U32 ceva_rxvectorproc_params)
{
    U32*                        pCeva_rxvectorproc_params = (U32*)ceva_rxvectorproc_params;
    PULRXINPUTPARAMS            pRxInputParams = (PULRXINPUTPARAMS)pCeva_rxvectorproc_params[0];
    PULRXFECPARAMS              pRxFecParams = (PULRXFECPARAMS)pCeva_rxvectorproc_params[1];
    U8*                         pRbUsedFlag = (U8*)pCeva_rxvectorproc_params[2];
    PHARQS                      pharq = (PHARQS)pCeva_rxvectorproc_params[3];
    U32*                        descrIndex = (U32*) pCeva_rxvectorproc_params[4];
    S8*                         descrInBufs = (S8*) pCeva_rxvectorproc_params[5];
    U32                         num_sym = (U32) pCeva_rxvectorproc_params[6];
    U32                         slotNumber = (U32) pCeva_rxvectorproc_params[7];

    U32 prevSize, cinit = 0;
    U32 TotalACK_Bits = 0, TotalRI_Bits = 0, TotalCQI_Bits = 0;
    U32 harq_id, nc;
    S16 prc, grp, iter;
    S32 posharq;
    S8 *sftpos;
    S16 harq_cont;
    U32 prevScrSize = 0;
    S8* ptrDescrSeq;
    U32 DescrInBufSize;
    U32 muxPUSCH;
    U32 i;
    S16* blkSizTab;

#if defined(EVM) || defined(RT_API_PROC)
    PMDMA_FDESC pMdma, pMdmaFirst = NULL;
#endif

    blkSizTab = (S16*)BlkSizTab;

    if (InitializeDriver(MDMA_SYS))
        return 1;

    pRxInputParams->pMdmaDescr = NULL;


    /////////////////// Setup FEC /////////////////////////////////////////////////
    //*****************************************************************************
    //**********                        UPLINK HARQ                      **********
    //*****************************************************************************
    posharq = 0;

    // Increment HARQ process counter.
    pharq->harq_prc = (pharq->harq_prc + 1) % P_HARQ_PROC;

    // Increment HARQ group index for process.
    pharq->harq_grp[pharq->harq_prc] = (pharq->harq_grp[pharq->harq_prc] + 1) % P_HARQ_COMB;
    // prc = HARQ process.
    prc = pharq->harq_prc;
    // grp = HARQ soft decision buffer group for process this subframe
    grp = pharq->harq_grp[prc];

    pRxInputParams->TotalMsc = 0;
    prevSize = 0;
    prevScrSize = 0;

    DescrInBufSize = 0;
    muxPUSCH = 0;
    pRxFecParams->TotalACK_Bits = 0;
    pRxFecParams->TotalRI_Bits  = 0;
    pRxFecParams->TotalCQI_Bits = 0;

#if defined(EVM) || defined(RT_API_PROC)
    if (pRxInputParams->numChan)
    {
        pMdmaFirst = MdmaDrvAllocDesc (MDMA_HW_SYS, pRxInputParams->numChan);
        _ASSERT_PTR (pMdmaFirst);
    }
    pMdma = pMdmaFirst;
#endif

    memset(pRbUsedFlag, 0, 2*pRxInputParams->MO*MAXRESOURCEBLOCKS*sizeof(U8));

    for (nc = 0; nc < (U32)pRxInputParams->numChan; nc++)
    {
        if (nc == 0)
            descrIndex[nc] = 0;
        else
            descrIndex[nc] = descrIndex[nc-1] + prevScrSize;

        if (pRxInputParams->physChanType[nc] == PUSCH)
        {
            pRxInputParams->TotalMsc += pRxInputParams->Mpusch_sc[nc];
            DescrInBufSize += pRxInputParams->G[nc];
            prevSize = pRxInputParams->G[nc];
            cinit = slotNumber >> 1;
            cinit = (pRxFecParams->nRNTI[nc] << 14) + (cinit << 9) + pRxInputParams->Ncell_id;

            //*****************************************************************************
            //**********                        UPLINK HARQ                      **********
            //*****************************************************************************
            // Define harq control (0=repeating data, 1=new data)
            harq_cont = ((pRxFecParams->nDi[nc]) & (0x1));

            // Define harq user id
            harq_id = pRxInputParams->harqRntiIndex[nc];

            // If HARQ combining disabled, then
            if (harq_cont == 1)
            {
                // Reset user's HARQ iteration counter.
                pharq->harq_iter[prc][harq_id] = 0;
            }
            else
            {
                // Increment user's HARQ iteration counter.
                pharq->harq_iter[prc][harq_id] = pharq->harq_iter[prc][harq_id] + 1;

                // Check for possible HARQ iteration error
                if ((pharq->harq_iter[prc][harq_id] < 0) || (pharq->harq_iter[prc][harq_id] >= P_HARQ_COMB))
                {
                    pharq->harq_iter[prc][harq_id] = P_HARQ_COMB - 1;
                }
            }

            // iter = user's HARQ iteration modulo P_HARQ_COMB.
            // This allows for more than P_HARQ_COMB iterations, but only keeps
            // the last P_HARQ_COMB iterations.
            iter = (pharq->harq_iter[prc][harq_id]) % P_HARQ_COMB;

            // sftpos = buffer address for user's soft decisions.
            sftpos = &(pharq->harq_sft_buf[prc][grp][posharq]);

            // Increment subframe buffer index.
            posharq = posharq + prevSize;

            // Store soft decision buffer address for user.
            pharq->harq_sft_pos[prc][harq_id][iter] = sftpos;

            // Store rate matching rvidx for user.
            pharq->harq_rvidx[prc][harq_id][iter] = pRxFecParams->rvidx[nc];

            // Store user index.
            pRxFecParams->harq_user[nc] = (S16) harq_id;

            //phy_printf("HARQ receive processing:\n");
            //phy_printf("     process = %d, group = %d\n", prc, grp);
            //phy_printf("     user index = %d, harq cont = %d, iter = %d\n", pharq->harq_user, harq_cont, pharq->harq_iter[prc][harq_id]);
            //phy_printf("     sftpos = %d, G = %d, posharq = %d\n", sftpos, pChan->G, posharq);
            //phy_printf("     rvidx = %d\n", pharq->harq_rvidx[prc][harq_id][iter]);
            //*****************************************************************************
            pRxFecParams->by[nc]      = sftpos;
        }
        else if (pRxInputParams->physChanType[nc] == PUCCH)
        {
            cinit = (slotNumber >> 1) + 1;
            cinit = cinit * ((2 * pRxInputParams->Ncell_id + 1) << 16) + pRxFecParams->nRNTI[nc];
            prevSize = pRxInputParams->Nrb_sc * 2;
        }

        prevScrSize = prevSize >> 3;
        ptrDescrSeq = &descrInBufs[descrIndex[nc]];

#if defined(EVM) || defined(RT_API_PROC)
        MdmaDrvSet4GSCR(pMdma, cinit, 1, 0, 0);
        MdmaDrvSetData(pMdma, pRxFecParams->genBuf, prevScrSize, ptrDescrSeq, prevScrSize);
        pMdma = pMdma->Next;
#elif defined (_WIN32)
        {
            U8 tmp[MAX_OUTPUT_TRANSPORT_BIT_SIZE];
            PseudoRandomSeqGen2(prevSize, cinit, tmp);
            PACKBB(tmp, ptrDescrSeq, prevSize, 0);
        }
#endif

        if (pRxInputParams->physChanType[nc] == PUSCH)
        {
            // Need to place logic here to define rank info for now always rank1
            Num_Cont_Syms_PUSCH(pRxFecParams, (S16)nc, (S16)num_sym, blkSizTab);

            pRxFecParams->muxPUSCH_User[nc] = 0;
            if (pRxFecParams->numbitsCQI[nc] || pRxFecParams->numbitsACK[nc] || pRxFecParams->numbitsRI[nc])
            {
                muxPUSCH = 1;
                pRxFecParams->muxPUSCH_User[nc] = 1;
                TotalACK_Bits += RUP4B(pRxFecParams->numbitsACK[nc]);
                TotalRI_Bits  += RUP4B(pRxFecParams->numbitsRI[nc]);
                TotalCQI_Bits += RUP4B(pRxFecParams->numbitsCQI[nc]);

                //Need to determine where soft decisions will be for ACK, RI and CQI
                //in Mux Control soft decision buffer for each user.
                pRxFecParams->ACK_User_offset[nc] = 0;
                pRxFecParams->RI_User_offset[nc] = 0;
                pRxFecParams->CQI_User_offset[nc] = 0;
                if (nc)
                {
                    for(i = 0; i < nc; i++)
                    {
                        pRxFecParams->ACK_User_offset[nc] += RUP4B(pRxFecParams->numbitsACK[i]);
                        pRxFecParams->RI_User_offset[nc]  += RUP4B(pRxFecParams->numbitsRI[i]);
                        pRxFecParams->CQI_User_offset[nc] += RUP4B(pRxFecParams->numbitsCQI[i]);
                    }
                }
            }
        }
    }
    pRxInputParams->DescrInBufSizePusch = DescrInBufSize;

#if defined(EVM) || defined(RT_API_PROC)
    pRxInputParams->pMdmaDescr = (U32*) pMdmaFirst;
#endif

    if(muxPUSCH)
    {
        pRxFecParams->TotalACK_Bits = TotalACK_Bits;
        pRxFecParams->TotalRI_Bits  = TotalRI_Bits;
        pRxFecParams->TotalCQI_Bits = TotalCQI_Bits;

        bs_rx_mux_control_findpos(pRxFecParams, P_DEINT_COL, pRxInputParams->CPType);
    }

    return 0;
}
#endif  //defined (_WIN32) || defined (EVM) || defined (RT_API_PROC)


#if defined(EVM) || defined(RT_API_PROC)

/******************************************************************************
    **********          MORE CRITICAL HARDWARE FEC INFORMATION           **********
    *******************************************************************************

     It appears that all of the Turner devices have some type of FEC channel
     de-interleaver problem.  This de-interleaver is designed to de-interleave one
     code block prior to de-rate matching and CTC decoding for the PUSCH.  For some
     reason the CTC decoder generates errors after de-interleaving and de-rate
     matching for some of the PUSCH test cases.  Unfortunately, there is no way in
     the Turner device to examine the CTC decoder inputs directly.  Only the decoder
     outputs can be seen.  Therefore, it is impossible to determine exactly what is
     causing the problem.  Bugzilla #66949 has been opened to track the problem.
     The FEC's designer Chuong Vu has investigated but so far has not been able to
     explain the observations.  The following conclusions are based on several
     PUSCH test cases executed on all of the Turner EVM boards.

     1) QPSK (Qm=2) test cases never seem to fail.

     2) All of the failures occur for either QAM16 (Qm=4) or QAM64 (Qm=6).  The
        failures seem to be related to the de-interleaver columns having different
        lengths.  For PUSCH test cases where the de-interleaver buffer columns have
        equal lengths there never are any failures.  The multiplexing of CQI and/or
        RI with the CTC data causes the column lengths to be different.

     3) As an experiment, the RI soft-decisions were all set to 0.  Amazingly, the
        CTC decoder output errors were reduced.  If the de-interleaver was
        operating properly, all of these RI soft decisions should be ignored.  This
        experiment seems to indicate that for some reason some of the RI soft
        decisions are incorrectly being processed during de-interleaving.

     4) Depending on the CTC decoder's programmed number of half iterations,
        sometimes it appears that whatever errors are being generated the decoder
        is capable of correcting them.  For this reason it is important to use only
        2 half iterations to help prevent any errors from being corrected.

     5) All of the PUSCH test case failures can be corrected by modifying the
        channel de-interleaver descriptors so that all of the columns have the same
        length.  Of course, this work-around results in slightly more puncturing
        since the shortest column length must be used for all of the columns and
        therefore some of the soft decisions normally sent to the de-rate matching
        block will be ignored.  This is the only work-around that seems to work.

     The code segments needed to implement this work-around are surrounded by:

    //FECFIXFECFIXFECFIXFECFIXFECFIXDECFIXFECFIXFECFIXFECFIXFECFIXFECFIXFECFIX

******************************************************************************/

// The conditions for executing the work-around are defined below.  These
// conditions ensure that the work-around does not result in excessive
// puncturing.  Of course, if the work-around is prevented because of excessive
// puncturing, then currently there is no guarantee that the FEC hardware will
// work correctly!

//      L = (P_FEC_DEINT_SCF*E) >> 15
//      if(L < K+4) L = K+4
//      if(T > L) execute work-around

//      Where:  E = de-rate matching input size.
//              K = code block size.
//              T = size of de-interleaver after work-around.
//              P_FEC_DEINT_SCF = fractional limit (x32768)


//*****************************************************************************

// Control parameter P_ENB_FEC_DEINT_FIX is used to enable (1) or disable (0)
// the FEC channel de-interleaver work-around.

#define P_ENB_FEC_DEINT_FIX     1   // Enable Turner FEC de-interleaver fix

//*****************************************************************************


#ifdef CEVA_INTRINSICS
#define FEC_CONFIG_REGS_BASE        (0xFE410000 + 0x400)
typedef S16  MatrixS16Type     [27][110];
typedef S16  MatrixS16Type3     [4][27][110];
#endif  // CEVA_INTRINSICS

#if defined(EVM) && defined(TEST_ULFEC_PROTECTION)
static U32 fec_reset_counter_ul = 0;
#endif


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink LteBsRxSduCevaPreProcessingRT
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |ulRxInputParams      |          |Input         |1         |Real          |(32:0:0)|Full Range  |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |ulFecParams          |          |Input         |1         |Real          |(32:0:0)|Full Range  |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |ulShareParams        |          |Input         |1         |Real          |(32:0:0)|Full Range  |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |ulHarq               |          |Input         |1         |Real          |(32:0:0)|Full Range  |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |Table_C              |          |Input         |1         |Real          |(32:0:0)|Full Range  |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |Table_K              |          |Input         |1         |Real          |(32:0:0)|Full Range  |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |Table_k0             |          |Input         |1         |Real          |(32:0:0)|Full Range  |      |
 *  --------------------------------------------------------------------------------------------------------
*  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |LteBsTxSduProcessingRT             |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |ulRxInputParams, ulFecParams,      |
 *  |                    |ulShareParams, ulHarq, Table_C     |
 *  |                    |Table_K, Table_k0,                 |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |                                   |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Ceva version of the LteBsRxSduPreProcessingRT.   Called directly from ARM via REX scheduler.
 *
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
U32 LteBsRxSduCevaPreProcessingRT(U32 ulRxInputParams, U32 ulFecParams, U32 ulShareParams,
                                  U32 ulHarq, U32 Table_C, U32 Table_K, U32 Table_k0)
{
    PULRXINPUTPARAMS            pUlRxInputParams = (PULRXINPUTPARAMS)ulRxInputParams;
    PULRXFECPARAMS              pFecParams = (PULRXFECPARAMS)ulFecParams;
    PLTE_PHY_UL_SHARED_PARAMS   pUlShareParams = (PLTE_PHY_UL_SHARED_PARAMS)ulShareParams;
    PHARQS                      pharq = (PHARQS)ulHarq;

    PHARQ_DESCRIPTOR_T          pHarqDescriptor = pharq->pHarqDescriptor;

    PRXSDUIND pRxSduInd;

    PFEC_CONFIG_REGS pFecConfigRegs;
    PFEC_UL_FDESC fecUlFirst[MAXCHSUBFRAME], fecUlLast[MAXCHSUBFRAME];
    PFEC_UL_FDESC pFecUlStart = NULL, pFecUL = NULL, pFecUlPrev = NULL;
    PMDMA_FDESC pMdmaStart = NULL, pMdma, pMdmaPrev = NULL;
    PFEC_UL_FDESC q = NULL;
    ULHWCTX hw_ctx = { 0 };

    //FECFIXFECFIXFECFIXFECFIXFECFIXDECFIXFECFIXFECFIXFECFIXFECFIXFECFIXFECFIX
    // If Turner device FEC de-interleaver issue work-around enabled, then.
#if P_ENB_FEC_DEINT_FIX
    // Required only for Turner FEC de-interleaver work-around
    S32 i, minlen, ncol, len, nlen, lim;

#endif
    //FECFIXFECFIXFECFIXFECFIXFECFIXDECFIXFECFIXFECFIXFECFIXFECFIXFECFIXFECFIX

    S16 Itbs, Nprbm1, K, C;
    S32 E;
    S16 prc;
    U16 numChan, nc, nctmp;
    U16 countPUSCH;
    S16 Qm;
    S32 G;
    S16 rvidx;
    U32 halfit;
    S16 NL = 1;
    S32 Cmux = 12;
    S16 r;
    S32 j;
    S32 inp;
    S16 crctype;
    S16 user = 0, iter, citer;
    U16 harqcomb, decbypass;
    S8 *psft;
    S32 dataBits;
    U8* out;
    U32 outp;
    U32 maxSize = 0, maxInd = 0;
    U32* numBytes;
    U32 ignoreHarq;

#ifdef CEVA_INTRINSICS
    MatrixS16Type  *Table_FEC_CodeBlock_C, *Table_FEC_CodeBlock_K;
    MatrixS16Type3 *Table_FEC_RateMatching_k0;

    Table_FEC_CodeBlock_C = (MatrixS16Type*)Table_C;
    Table_FEC_CodeBlock_K = (MatrixS16Type*)Table_K;
    Table_FEC_RateMatching_k0 = (MatrixS16Type3*)Table_k0;

    if (InitializeDriver(MDMA_SYS))
        return 1;

    if (InitializeDriver(FEC_UL))
        return 20;
#endif

#if defined(EVM) && defined(TEST_ULFEC_PROTECTION)
    fec_reset_counter_ul++;
#endif

    numChan = pFecParams->numChan;
    countPUSCH = 0;
    numBytes = pFecParams->numBytes;

    for (nctmp = 0; nctmp < numChan; nctmp++)
    {
        nc = pUlShareParams->IndexMap[nctmp];

        if (pUlRxInputParams->physChanType[nc] == PUSCH)
        {
            // Prepare MDMA/FEC descriptors
            out = pUlShareParams->fecOut[nc];

            Qm = pFecParams->Qm[nc];
            dataBits = pUlRxInputParams->G[nc] - pFecParams->numbitsCQI[nc] - pFecParams->numbitsRI[nc];
            G = dataBits;
            halfit = pFecParams->HalfIterations;

            // Compute transmitter's code block segmentation input size in bits.

            // Get 3GPP TS 36.213 Table 7.1.7.2.1-1 Itbs and Nprb-1 values.
            Itbs =  pFecParams->Itbs[nc];
            Nprbm1 =  pFecParams->Nprbm1[nc];

#ifdef CEVA_INTRINSICS
            // Use table look-up to compute total number of code blocks (C).
            C = (S16)(*Table_FEC_CodeBlock_C)[Itbs][Nprbm1];

            // Use table look-up to compute code block size in bits (K);
            K = (S16)(*Table_FEC_CodeBlock_K)[Itbs][Nprbm1];
#else
            // Use table look-up to compute total number of code blocks (C).
            C = Table_FEC_CodeBlock_C[Itbs][Nprbm1];

            // Use table look-up to compute code block size in bits (K);
            K = Table_FEC_CodeBlock_K[Itbs][Nprbm1];
#endif
            // If only one code block then there is no attached CRC24B (crctype = 0).
            // Otherwise, CRC24B is appended to each code block (crctype = 2).
            crctype = 0;
            if (C > 1)
                crctype = 2;

            // Initaialize soft decision input array index.
            inp = 0;

            // Initialize output array index.
            outp = 0;

            // prc = HARQ process.
            prc = pharq->harq_prc;

            // user = HARQ user (channel number).
            user = pFecParams->harq_user[nc];

            // iter = total number of HARQ iterations - 1.
            iter = pharq->harq_iter[prc][user];

            // Limit iter to maximum allowed - 1.
            if(iter >= P_HARQ_COMB) iter = P_HARQ_COMB-1;

            // citer = current HARQ iteration (0 - P_HARQ_COMB-1).
            citer = pharq->harq_iter[prc][user] % P_HARQ_COMB;

            //  Check all HARQ history descriptors for valid size
            ignoreHarq = 0;
            for (r = 0; r < C; r++)
            {
                if(pHarqDescriptor->harq_DescLen[prc][user][0][r][0] <= 0)
                {
                    ignoreHarq = 1;
                    break;
                }
                else
                {
                    for (j = 1; j < iter; j++)
                    {
                        if(pHarqDescriptor->harq_DescLen[prc][user][j][r][0] != pHarqDescriptor->harq_DescLen[prc][user][j-1][r][0])
                            ignoreHarq = 1;
                    }
                }
            }

            // Bypass HARQ on this subframe, init new iteration
            if(ignoreHarq)
            {
                // stop_printf("HARQ API error detected!!!\n");
                pharq->harq_iter[prc][user] = 0;
                iter = 0;
                citer = 0;
            }

            // Configure FEC control registers for HARQ.
            pFecConfigRegs = (PFEC_CONFIG_REGS) FEC_CONFIG_REGS_BASE;
            pFecConfigRegs->harq_zerolen = 1;

            // Assume DescN == 12, otherwise need allocate dynamically
            pFecUL = FecUlDrvAllocDesc(C * (iter + 1));
            _ASSERT_PTR(pFecUL);

            if(maxSize < numBytes[nc])
            {
                maxSize = numBytes[nc];
                maxInd = countPUSCH;
            }
            fecUlFirst[countPUSCH] = pFecUL;

            if (pFecUL == NULL)
            {
                ceva_uart_printf("Can't allocate FEC UL descriptors, C %d, iter %d, out size %d\r\n", C, iter, numBytes[nc]);
                return 0;
            }

            if(countPUSCH == 0)
            {
                pFecUlStart = pFecUL;
            }
            else
            {
                pFecUlPrev->Next = pFecUL;
            }

            // set hardware context
            memset(&hw_ctx, 0, sizeof(hw_ctx));
            hw_ctx.CTC_CCn = 1;
            hw_ctx.CRC_Sel = crctype;
            hw_ctx.HalfInteration = halfit;
            hw_ctx.Bypass_Rate_Match = 0;
            hw_ctx.Rate_Index = 0;
            hw_ctx.Dynamic_Stop_Iteration = 0;

            // Assign 1st pointer to pFecUL.
            q = pFecUL;

            // Do for each code block.
            for (r = 0; r < C; r++)
            {
                // Compute rate matching parameter E.
                E = Compute_Rate_Matching_E(C, NL, G, Qm, r);

                // Initialize harq combining control for no combining.
                harqcomb = 0;

                // Initialize CTC decoder bypass control for bypass
                decbypass = 1;

                // Do for each HARQ iteration
                for (j = 0; j <= iter; j++)
                {
                    // If last iteration, then enable CTC decoder.
                    if (j == iter)
                        decbypass = 0;

                    // rvidx = rate matching rviex for iteration
                    rvidx = pharq->harq_rvidx[prc][user][j];

                    // psft = pointer to soft decisions for iteration
                    psft = pharq->harq_sft_pos[prc][user][j];

                    // If iteration is current HARQ iteration, then.
                    if(j == citer)
                    {
                        // Save rate matching E for code block in HARQ structure.
                        pharq->harq_E[prc][user][j][r] = E;

                        // rvidx = rate matching rviex for iteration
                        rvidx = pharq->harq_rvidx[prc][user][j];

#ifdef CEVA_INTRINSICS
                        // Save rate matching k0 for code block in HARQ structure.
                        pharq->harq_k0[prc][user][j][r] = (S16)(*Table_FEC_RateMatching_k0)[rvidx][Itbs][Nprbm1];
#else
                        // Save rate matching k0 for code block in HARQ structure.
                        pharq->harq_k0[prc][user][j][r] = Table_FEC_RateMatching_k0[rvidx][Itbs][Nprbm1];
#endif

                        // Save constellation size in HARQ structure.
                        if(r == 0) pharq->harq_Qm[prc][user][j] = Qm;

                        // Generate channel de-interleaver descriptors for code block
                        // and save in HARQ structure.
                        LTE_Chan_Deint_Gen(inp, inp - 1 + E,
                                           0, Qm, Cmux,
                                           dataBits, pFecParams->numbitsCQI[nc], pFecParams->numbitsRI[nc],
                                           psft,
                                           &pHarqDescriptor->harq_DescAdr[prc][user][j][r][0],
                                           &pHarqDescriptor->harq_DescLen[prc][user][j][r][0],
                                           &pHarqDescriptor->harq_DescN[prc][user][j][r]);

                        //FECFIXFECFIXFECFIXFECFIXFECFIXDECFIXFECFIXFECFIXFECFIXFECFIXFECFIXFECFIX
                        // If Turner device FEC de-interleaver issue work-around enabled, then.
#if P_ENB_FEC_DEINT_FIX

                        // If QAM16 (Qm=4) or QAM64 (Qm=6), then.
                        if(Qm > 2)
                        {

                            //printf("!!! FEC De-interleaver work-around: !!!\n     Qm = %d, Number of columns = %d\n",
                            //       Qm, pHarqDescriptor->harq_DescN[prc][user][j][r]);

                            // Compute limit for executing work-around.
                            lim = E*P_FEC_DEINT_SCF;
                            lim = lim >> 15;
                            if(lim < (K+4)) lim = K+4;

                            // Initialize minimum column length.
                            minlen = 1000000;

                            // Initialize number of non-zero lengths.
                            nlen = 0;

                            // Get number of columns.
                            ncol = pHarqDescriptor->harq_DescN[prc][user][j][r];

                            // For each of the other de-interleaver column lengths.
                            for(i = 0; i < ncol; i++)
                            {
                                // get column length.
                                len = pHarqDescriptor->harq_DescLen[prc][user][j][r][i];

                                // If not an empty column, then.
                                if(len != 0)
                                {
                                    // If column length is shorter, then use it as minimum length.
                                    if(len < minlen) minlen = len;

                                    // Increment non-empty column counter.
                                    nlen = nlen + 1;
                                }

                                //printf("     Column length[%d] = %d, minlen = %d\n", i, len, minlen);

                            }

                            //printf("nlen=%d, minlen=%d, nlen*minlen=%d, E=%d, K=%d, lim=%d\n", nlen, minlen, nlen*minlen, E, K, lim);

                            // If additional puncturing does not exceed limit, then.
                            if( (minlen*nlen) > lim)
                            {

                                // For each de-interleaver buffer column.
                                for(i = 0; i < ncol; i++)
                                {
                                    // Reset longer column lengths to minimum column length.
                                    if(pHarqDescriptor->harq_DescLen[prc][user][j][r][i] > minlen)
                                    {

                                        //printf("Original %dth column length=%d, new column length=%d\n",
                                        //       i, pHarqDescriptor->harq_DescLen[prc][user][j][r][i], minlen);

                                        pHarqDescriptor->harq_DescLen[prc][user][j][r][i] = minlen;
                                    }
                                }

                            }

                        }

#endif
                        //FECFIXFECFIXFECFIXFECFIXFECFIXDECFIXFECFIXFECFIXFECFIXFECFIXFECFIXFECFIX

                        if (pHarqDescriptor->harq_DescN[prc][user][j][r] != 12)
                        {
                            ceva_uart_printf("Number of symbols %d is not supported in channel deinterleaver\n",
                                              pHarqDescriptor->harq_DescN[prc][user][j][r]);
                            return 1;
                        }

                    }

                    hw_ctx.BlockSize = K;
                    hw_ctx.Filler = 0;
                    hw_ctx.E_Param = pharq->harq_E[prc][user][j][r];
                    hw_ctx.LTERateMaching_K0 = ( pharq->harq_k0[prc][user][j][r] ) >> 1;
                    hw_ctx.Enable_Combine = harqcomb;
                    hw_ctx.Bypass_Decode_Enable = decbypass;
                    hw_ctx.Modulation = ( pharq->harq_Qm[prc][user][j] ) >> 1;

                    // Create FEC CTC decoder control descriptor via driver.
                    FecUlDrvSetHwCtx(q, hw_ctx);

#if !(defined(EVM) && defined(TEST_ULFEC_PROTECTION))
                    FecUlDrvSetData(q, (LPVOID*)&pHarqDescriptor->harq_DescAdr[prc][user][j][r][0],
                                 pHarqDescriptor->harq_DescLen[prc][user][j][r][0],
                                 (U8*) (out + outp), K / 8);
#else
                    if(fec_reset_counter_ul % 15 == 11)
                        FecUlDrvSetData(q, (LPVOID*)&pHarqDescriptor->harq_DescAdr[prc][user][j][r][0],
                                 pHarqDescriptor->harq_DescLen[prc][user][j][r][0],
                                 (U8*) (out + outp), 0);
                    else
                        FecUlDrvSetData(q, (LPVOID*)&pHarqDescriptor->harq_DescAdr[prc][user][j][r][0],
                                 pHarqDescriptor->harq_DescLen[prc][user][j][r][0],
                                 (U8*) (out + outp), K / 8);
#endif
                    pFecUlPrev = q;
                    q = (PFEC_UL_FDESC)q->Next;

                    // Set HARQ combining to combine next time.
                    harqcomb = 1;
                }

                // Compute number of decoded data bits - filler bits
                j = K;

                // If more than 1 code block
                if (C > 1)
                {
                    // Subtract 24 gCRC24B CRC bits from decoded data bits total.
                    j = j - 24;
                }

                // If last code block, then subtract 24 gCRC24A CRC bits from
                // decoded data bits total.
                if (r == (C - 1))
                {
                    j = j - 24;
                }

                // Increment output array index.
                outp += j / 8;

                // Increment input array index.
                inp += E;

            }

            fecUlLast[countPUSCH] = pFecUlPrev;

            if(numBytes[nc] != outp)
            {
                ceva_uart_printf("Wrong UL FEC settings\r\nRX SDU size taken from API: %d, calculated FEC output size %d\n",
                    numBytes[nc], outp);
                return 2;
            }

            pMdma = MdmaDrvAllocDesc (MDMA_HW_SYS, 1);
            _ASSERT_PTR (pMdma);
            if(countPUSCH == 0)
            {
                pMdmaStart = pMdma;
                pMdmaStart->Next = 0;
            }
            else
            {
                pMdmaPrev->Next = pMdma;
            }
            pMdmaPrev = pMdma;

            // Derive CRC bits and append them using MDMA.
            pRxSduInd = (PRXSDUIND)pUlShareParams->mdmaOut[nc];
            MdmaDrvSetCRC(pMdma, 0, 0x864CFB, 1);
            MdmaDrvSetDataEx(pMdma, out, numBytes[nc], &pRxSduInd->pRxSdu, numBytes[nc] + 3);
            countPUSCH++;
        }
    }

    if(countPUSCH > 1 && (maxInd != countPUSCH - 1))
    {
        // Make a ring
        fecUlLast[countPUSCH-1]->Next = pFecUlStart;
        // Change start to next after Max: (maxInd+1) is valid because (maxInd != countPUSCH - 1)
        pFecUlStart = fecUlFirst[maxInd+1];
        // Break the ring
        fecUlLast[maxInd]->Next = 0;
    }


    pUlShareParams->pFecUlStart = (U32*)pFecUlStart;
    pUlShareParams->pMdmaStart  = (U32*)pMdmaStart;

    return 0;
}

#endif  // defined(EVM) || defined(RT_API_PROC)




#if defined (CEVA_INTRINSICS) && !defined (EVM)

// DUMMY declarations for CEVA Simulator Mode
U32 LteBsTxStartCRC_RT(U32 pSduDescIn)
{
    return (pSduDescIn);
}
U32 LteBsTxSduProcessingRT(U32 pSduDescIn)
{
    return (pSduDescIn);
}

U32 LteBsRxSduCevaPreProcessingRT(U32 ulRxInputParams, U32 ulFecParams, U32 ulShareParams,
                                  U32 ulHarq, U32 Table_C, U32 Table_K, U32 Table_k0)
{
    return (ulRxInputParams + ulFecParams + ulShareParams + ulHarq + Table_C + Table_K + Table_k0);
}

U32 LteBsRxVectorProcessingRT(U32 ceva_rxvectorproc_params)
{
    return ceva_rxvectorproc_params;
}

U32 LteBsApiErrorCheck(U32 LteBsApiErrorCheckCtxLocal)
{
	return LteBsApiErrorCheckCtxLocal;
}

U32 LteBsStartTtiProcessing(U32 LteBsApiErrorCheckCtxLocal, U32 BsTxSduDesc)
{
    return (LteBsApiErrorCheckCtxLocal + BsTxSduDesc);
}

U32 InsertPBCH_RT(U32 pSduDescIn)
{
    return pSduDescIn;
}

#endif  // defined (CEVA_INTRINSICS) && !defined (EVM)









// API ErrorCheck   - Only for Device Mode and Ceva in Device Mode
#ifndef _WIN32
#if defined(EVM) || defined(RT_API_PROC)
#include "../Arm/Prj/LtePhyRt/LteBsApiErrorCheck.h"

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink AddDci0InpBuf
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name                   | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                       | Standard |              |          |              |        |            |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |pdcchSdu               |          |pInput        |1         |pStructure    |        |            |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |pDlCntrl_TxInPars      |          |pOutput       |1         |pStructure    |        |            |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |pTxInputParams         |          |pOutput       |1         |pStructure    |        |            |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |numPdcchSdu            |          |Input         |1         |Real          |(32:0:0)|Full Range  |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |gUseTxSduPointer       |          |Input         |1         |pInput        |        |            |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------
 *  |Block Name          |AddPdcchInpBuf          |
 *  -----------------------------------------------
 *  |Input Stream(s)     |pdcchSdu                |
 *  -----------------------------------------------
 *  |Output Stream(s)    |pDlCntrl_TxInPars,      |
 *  |                    |pTxInputParams          |
 *  -----------------------------------------------
 *  |Config Parameter(s) |numPdcchSdu,            |
 *  |                    |gUseTxSduPointer        |
 *  -----------------------------------------------
 *  |Status Parameter(s) |                        |
 *  -----------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Called directly from ARM via REX scheduler.  Copy Pdcch TxSDU messages to
 *  Downlink Control Modulation structure for Ceva Processing.
 *
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void AddPdcchInpBuf(PDLTXINPUTPARAMS pTxInputParams, PIOParams_DlControl pDlCntrl_TxInPars, PTXSDUREQ *pdcchSdu, U32 numPdcchSdu, U32 gUseTxSduPointer)
{
    U32 i, numBytes, TotalMsgLen;
    PTXSDUREQ pTxSduReq;

    for(i=0; i<numPdcchSdu; i++)
    {
        pTxSduReq = pdcchSdu[i];
        if (pTxInputParams->txChanIdMappingTable[pTxSduReq->chanId] != -1)      // This was present in TxVectror
        {
            numBytes = pTxSduReq->msgLen;
            if(gUseTxSduPointer)
            {
                // Copy payload from pointer to end of TxSdu
                memcpy((U8*)&pTxSduReq[1], pTxSduReq->pTxSdu, numBytes);
            }

            TotalMsgLen = (U32)(numBytes + sizeof(TXSDUREQ));
            memcpy(pDlCntrl_TxInPars->ptrInputPDCCH, pTxSduReq, TotalMsgLen);

            pDlCntrl_TxInPars->ptrInputPDCCH[TotalMsgLen] = 0;
            pDlCntrl_TxInPars->ptrInputPDCCH += RUP8B((U32) numBytes + 2 + sizeof (TXSDUREQ));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink AddDci0InpBuf
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name                   | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                       | Standard |              |          |              |        |            |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |dciSdu                 |          |pInput        |1         |pStructure    |        |            |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |pDlCntrl_TxInPars      |          |pOutput       |1         |pStructure    |        |            |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |numDciSdu              |          |Input         |1         |pInput        |        |            |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------
 *  |Block Name          |AddDci0InpBuf           |
 *  -----------------------------------------------
 *  |Input Stream(s)     |dciSdu,                 |
 *  -----------------------------------------------
 *  |Output Stream(s)    |pDlCntrl_TxInPars       | 
 *  -----------------------------------------------
 *  |Config Parameter(s) |numDciSdu               |
 *  -----------------------------------------------
 *  |Status Parameter(s) |                        |
 *  -----------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Called directly from ARM via REX scheduler.  Copy Dci TxSDU messages to
 *  Downlink Control Modulation structure for Ceva Processing.
 *
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
void AddDci0InpBuf(PIOParams_DlControl pDlCntrl_TxInPars, PDCIULSDUMSG *dciSdu, U32 numDciSdu)
{
    U32 i, numBytes;
    PDCIULSDUMSG pDci;

    for(i=0; i<numDciSdu; i++)
    {
        pDci = dciSdu[i];
        numBytes = pDci->msgLength;
        memcpy(pDlCntrl_TxInPars->ptrInputPDCCH, dciSdu[i], (U32) numBytes + sizeof (DCIULSDUMSG));
        pDlCntrl_TxInPars->ptrInputPDCCH += RUP8B((U32) numBytes + 2 + sizeof (DCIULSDUMSG));
    }
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink LtePhyHiDciParse
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name                   | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                       | Standard |              |          |              |        |            |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |LteBsApiErrorCheck     |          |pInput        |1         |pStructure    |        |            |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |pDlCntrl_TxInPars      |          |pOutput       |1         |pStructure    |        |            |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |pDlCntrl_TxInPars_List |          |pOutput       |1         |pStructure    |        |            |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |pTxInputParams         |          |pOutput       |1         |pStructure    |        |            |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  |gUseTxSduPointer       |          |Input         |1         |pInput        |        |            |      |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  -----------------------------------------------------------------
 *  |Block Name          |LtePhyHiDciParse                          |
 *  -----------------------------------------------------------------
 *  |Input Stream(s)     |LteBsApiErrorCheck,                       |
 *  -----------------------------------------------------------------
 *  |Output Stream(s)    |pDlCntrl_TxInPars, pDlCntrl_TxInPars_List |
 *  |                    |pTxInputParams                            |
 *  -----------------------------------------------------------------
 *  |Config Parameter(s) |gUseTxSduPointer                          |
 *  -----------------------------------------------------------------
 *  |Status Parameter(s) |                                          |
 *  -----------------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Called directly from ARM via REX scheduler.  Parse HiDci API vector 
 *  from MAC in the Ceva
 *
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
U32 LtePhyHiDciParse(U32 LteBsApiErrorCheck, PIOParams_DlControl pDlCntrl_TxInPars, PIOParams_DlControl pDlCntrl_TxInPars_List, PDLTXINPUTPARAMS pTxInputParams, U32 gUseTxSduPointer)
{
    PLTE_BS_API_ERROR_CHECK pLteBsApiErrorCheck = (PLTE_BS_API_ERROR_CHECK)LteBsApiErrorCheck;
    PIOParams_DlControl pDlCntrl_Pars;
    PHIINFOMSGDESC pHi= NULL;
    PDCIULSDUMSG pDci = NULL;

    S16 PHICH_ScaleFactor, PDCCH_ScaleFactor;
    S16 temp16;
    U32 status = 0, i;

    //  Store PDCCH SDUs for current subframe
    pDlCntrl_TxInPars->ptrInputPDCCH = pDlCntrl_TxInPars->inputPDCCH;
    if(pLteBsApiErrorCheck->numPdcchSdu)
    {
        AddPdcchInpBuf(pTxInputParams, pDlCntrl_TxInPars, pLteBsApiErrorCheck->pPdcchSdu, pLteBsApiErrorCheck->numPdcchSdu, gUseTxSduPointer);
    }

    // Update DL Control with PHICH/DCI0 channels
    pDlCntrl_Pars = pDlCntrl_TxInPars_List;
    pDlCntrl_Pars->numDCI0 = pLteBsApiErrorCheck->numDciSdu;
    if(pLteBsApiErrorCheck->numHARQVector)
    {
        PHICH_ScaleFactor = 0;
        if(pLteBsApiErrorCheck->pHiDciMsg->numHiSdus)
        {
            pDlCntrl_Pars->number_of_phich_channels = pLteBsApiErrorCheck->numPhichSdu;

            temp16 = add((S16)pDlCntrl_Pars->pilotPowerdB_EPRE , (S16)pLteBsApiErrorCheck->pHichSdu[0]->txPowerControl);
            PHICH_ScaleFactor = Convert_dB_Linear(temp16);
            pDlCntrl_Pars->scalefactorPHICH = PHICH_ScaleFactor;

            for(i=0; i<pLteBsApiErrorCheck->numPhichSdu; i++)
            {
                pHi = pLteBsApiErrorCheck->pHichSdu[i];
                pDlCntrl_Pars->PHICH_Bit1[i] = pHi->nackAck;
                pDlCntrl_Pars->PHICH_SequenceIndex1[i] = pHi->phichSequenceIndex;
                pDlCntrl_Pars->PHICH_Grp[i]  = pHi->phichGroupNumber;
            }
        }

        // Update DL Control with DCI channels
        if(pLteBsApiErrorCheck->pHiDciMsg->numDciUlSdus)
        {
            pDlCntrl_Pars->number_of_pdcch_channels += pLteBsApiErrorCheck->numDciSdu;

            // Transmode and scale factor is the same for all PDCCH
            if ((pLteBsApiErrorCheck->numDciSdu) && (pDlCntrl_Pars->numPDCCH == 0))
            {
                pDci = pLteBsApiErrorCheck->pDciSdu[0];

                temp16 = add((S16)pDlCntrl_Pars->pilotPowerdB_EPRE , (S16)pDci->txPowerControl);
                PDCCH_ScaleFactor = Convert_dB_Linear(temp16);

                pDlCntrl_Pars->scalefactorPDCCH = PDCCH_ScaleFactor;
                pDlCntrl_Pars->transmode = pDci->transmissionMode;
            }

            for(i = 0; i < pLteBsApiErrorCheck->numDciSdu; i++)
            {
                pDci = pLteBsApiErrorCheck->pDciSdu[i];
                pDlCntrl_Pars->pdcchRNTI[pDlCntrl_Pars->numPDCCH + i] = pDci->rnti;
                pDlCntrl_Pars->NumCCECtl[pDlCntrl_Pars->numPDCCH + i]   = (pDci->numCCE*72) >> 3; // length in Bytes
                pDlCntrl_Pars->StartCCECtl[pDlCntrl_Pars->numPDCCH + i] = (pDci->startCCE*72) >> 3; //start location in Bytes
            }

            AddDci0InpBuf(pDlCntrl_TxInPars_List, pLteBsApiErrorCheck->pDciSdu, pLteBsApiErrorCheck->numDciSdu);
        }
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink LteBsApiErrorCheckSortTxSdu
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                 | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                     | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pTxSduReqStore       |          |pInput        |1         |pStructure    |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pLteBsApiErrorCheck  |          |pOutput       |1         |pStructure    |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |numTxSdu             |          |Input         |1         |Real          |(32:0:0)|Full Range  |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |LteBsApiErrorCheckSortTxSdu        |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |pTxSduReqStore                     |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |pLteBsApiErrorCheck                |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |numTxSdu                           |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |                                   |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Called directly from ARM via REX scheduler.  Api error checking for TxSDU.
 *  Will sort TxSdus in order of ChanID
 *
 *
 * \ingroup group_lte_tcb
 *
 **/
////////////////////////////////////////////////////////////////////////////////////////
UINT32 LteBsApiErrorCheckSortTxSdu(PLTE_BS_API_ERROR_CHECK pLteBsApiErrorCheck, PTXSDUREQ *pTxSduReqStore, UINT32 numTxSdu)
{
    UINT32 i, numswitch;
    PTXSDUREQ pTxSduReqLocal;

    do
    {
        numswitch = 0;
        for(i = 1; i < numTxSdu; i++)
        {
            if (pTxSduReqStore[i-1]->chanId > pTxSduReqStore[i]->chanId)
            {
                pTxSduReqLocal = pTxSduReqStore[i-1];
                pTxSduReqStore[i-1] = pTxSduReqStore[i];
                pTxSduReqStore[i] = pTxSduReqLocal;

                numswitch = 1;
            }
        }
    }while(numswitch != 0);

    for (i = 0; i < numTxSdu; i++)
    {
        // Check Channel Type
        if (pTxSduReqStore[i]->channelType == PDSCH)
        {
            pLteBsApiErrorCheck->pPdschSdu[pLteBsApiErrorCheck->numPdschSdu++] = pTxSduReqStore[i];
        }
        else if (pTxSduReqStore[i]->channelType == PDCCH)
        {
            pLteBsApiErrorCheck->pPdcchSdu[pLteBsApiErrorCheck->numPdcchSdu++] = pTxSduReqStore[i];
        }
        else if (pTxSduReqStore[i]->channelType == PBCH)
        {
            pLteBsApiErrorCheck->pBchSdu = pTxSduReqStore[i];
            pLteBsApiErrorCheck->numPbchSdu++;
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink LtePhyApiErrorCheckUseEmptyChannels
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------------------------
 *  |Name                   | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                       | Standard |              |          |              |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  |pLteBsApiErrorCheckCtx |          |pInput        |1         |pStructure    |        |            |      |
 *  --------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |LtePhyApiErrorCheckUseEmptyChannels|
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |pLteBsApiErrorCheckCtx             |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |                                   |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |                                   |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Called directly from ARM via REX scheduler.  This function is used by PHY 
 *  to use Dummy vectors and continue with processing if MAC messages are not 
 *  delivered on time.
 *
 *
 * \ingroup group_lte_phy_control_rt_mac_phy
 *
 **/
PMAC2PHY_QUEUE_EL LtePhyApiErrorCheckUseEmptyChannels(PLTE_BS_API_ERROR_CHECK_CTX pLteBsApiErrorCheckCtx)
{
    PDLSUBFRDESC pEmptyTxVector;
    PULSUBFRDESC pEmptyRxVector;

    UINT32 currTxSfn, currRxSfn;

#if defined(RT_API_PROC)
    pLteBsApiErrorCheckCtx = LteBsApiErrorCheckGetCtx();
#endif

    _ASSERT_PTR(pLteBsApiErrorCheckCtx);

    pEmptyTxVector = pLteBsApiErrorCheckCtx->pEmptyTxVector;
    pEmptyRxVector = pLteBsApiErrorCheckCtx->pEmptyRxVector;

    _ASSERT_PTR(pEmptyTxVector);
    _ASSERT_PTR(pEmptyRxVector);

    currTxSfn         = LteBsIncrementSfn(pLteBsApiErrorCheckCtx->lastTxSfn);
    currRxSfn         = LteBsIncrementSfn(pLteBsApiErrorCheckCtx->lastRxSfn);

    pEmptyTxVector->frameNumber    = LteBsGetFrameNum(currTxSfn);
    pEmptyTxVector->subframeNumber = LteBsGetSubFrameNum(currTxSfn);

    pEmptyRxVector->frameNumber    = LteBsGetFrameNum(currRxSfn);
    pEmptyRxVector->subframeNumber = LteBsGetSubFrameNum(currRxSfn);

    if (pEmptyTxVector->subframeNumber == 0)
    {
        if ((pEmptyTxVector->frameNumber & 0x3) == 0)
        {
            pEmptyTxVector->numberofChannelDescriptors = 1;
        }
        else
        {
            pEmptyTxVector->numberofChannelDescriptors = 0;
        }
    }
    else
    {
        pEmptyTxVector->numberofChannelDescriptors = 0;
    }

    return (pLteBsApiErrorCheckCtx->pTxMsg);
}

////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink LteBsStartTtiProcessing
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------------
 *  |Name                       | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                           | Standard |              |          |              |        |            |      |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |LteBsApiErrorCheckCtxLocal |          |pInput        |1         |pStructure    |        |            |      |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |BsTxSduDesc                |          |pInput        |1         |pStructure    |        |            |      |
 *  ---------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |LteBsStartTtiProcessing            |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |LteBsApiErrorCheckCtxLocal,        |
 *  |                    |BsTxSduDesc                        |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |                                   |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Called directly from ARM via REX scheduler.  This function is used by PHY 
 *  to start TTI processing from the Ceva.
 *
 *
 * \ingroup group_lte_phy_control_rt_mac_phy
 *
 **/
U32 LteBsStartTtiProcessing(U32 LteBsApiErrorCheckCtxLocal, U32 BsTxSduDesc)
{
    PLTE_BS_API_ERROR_CHECK_CTX pLteBsApiErrorCheckCtx  = (PLTE_BS_API_ERROR_CHECK_CTX )LteBsApiErrorCheckCtxLocal;
    PLTE_PHY_SDU_DESC pSduDesc                          = (PLTE_PHY_SDU_DESC)BsTxSduDesc;
    PLTE_BS_API_ERROR_CHECK pLteBsApiErrorCheck         = pLteBsApiErrorCheckCtx->pLteBsApiErrorCheck;
    PDLTXINPUTPARAMS pTxInputParams;
    PIOParams_DlControl pDlCntrl_Pars;

    // Dispatch CRC
    U32 *in_dat_ptr;
    U32 countSdu, numPdsch = 0;

    UINT32 retVal = 0;

    retVal = LteBsApiErrorCheck(LteBsApiErrorCheckCtxLocal);

    if (retVal == 0)
    {
        // DL CRC
        if (pLteBsApiErrorCheck->numPdschSdu)
        {
            for(countSdu = 0; countSdu < pLteBsApiErrorCheck->numPdschSdu; countSdu++)
            {
                if(pLteBsApiErrorCheck->pPdschSdu[countSdu]->msgLen > 0)
                {
                    in_dat_ptr = (U32*)((pSduDesc->gUseTxSduPointer)?
                                pLteBsApiErrorCheck->pPdschSdu[countSdu]->pTxSdu : (U32*) ((U8*) pLteBsApiErrorCheck->pPdschSdu[countSdu] + sizeof(TXSDUREQ))); // ptr

                    pSduDesc->num_in_bytes[numPdsch] = pLteBsApiErrorCheck->pPdschSdu[countSdu]->msgLen;
                    pSduDesc->inDatPtr[numPdsch]     = (U32)in_dat_ptr;
                    pSduDesc->cwId[numPdsch]         = pLteBsApiErrorCheck->pPdschSdu[countSdu]->cwId;
                    pSduDesc->chanId[numPdsch]       = pLteBsApiErrorCheck->pPdschSdu[countSdu]->chanId;

                    numPdsch++;
                }
            }

            if (numPdsch)
            {
                pSduDesc->numSDU = numPdsch;

                LteBsTxStartCRC_RT((U32)pSduDesc);
            }
        }

        // TxVectorProcData
        pTxInputParams = pSduDesc->pTxInputParams;
        retVal = LteBsPhyTxVectorProcData(pLteBsApiErrorCheck->pTxVector, pTxInputParams, pLteBsApiErrorCheckCtx->isInternalBch);

        // TxVectorProcControl
        pDlCntrl_Pars = pSduDesc->pDlCntrl_Pars;
        retVal |= LteBsPhyTxVectorProcControl(pLteBsApiErrorCheck->pTxVector, pDlCntrl_Pars, pTxInputParams);

        // Add the PDCCH SDUs
        if(pSduDesc->gLowLatencyFlag)
            LtePhyHiDciParse((U32)pLteBsApiErrorCheck, pDlCntrl_Pars, pDlCntrl_Pars, pTxInputParams, pSduDesc->gUseTxSduPointer);
        else
            LtePhyHiDciParse((U32)pLteBsApiErrorCheck, pDlCntrl_Pars, pSduDesc->pDlCntrl_Pars_List, pTxInputParams, pSduDesc->gUseTxSduPointer);

        // PDSCH / PBCH related
        pSduDesc->totalsizescr[0]   = 0;
        pSduDesc->totalsizescr[1]   = 0;
        pSduDesc->scrsize[0]        = 0;
        pSduDesc->scrsize[1]        = 0;
    }

    return retVal;
}


////////////////////////////////////////////////////////////////////////////////////////
/** @brief LTE Uplink LteBsApiErrorCheck
 *
 *  <1> Hardware Resource:
 *  RESOURCE_CEVA
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------------
 *  |Name                       | Name In  |Direction     |Length    |Type          |Format  |Valid Range |Units |
 *  |                           | Standard |              |          |              |        |            |      |
 *  ---------------------------------------------------------------------------------------------------------------
 *  |LteBsApiErrorCheckCtxLocal |          |pInput        |1         |pStructure    |        |            |      |
 *  ---------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------------------------
 *  |Block Name          |LteBsApiErrorCheck                 |
 *  ----------------------------------------------------------
 *  |Input Stream(s)     |                                   |
 *  ----------------------------------------------------------
 *  |Output Stream(s)    |                                   |
 *  ----------------------------------------------------------
 *  |Config Parameter(s) |LteBsApiErrorCheckCtxLocal,        |
 *  ----------------------------------------------------------
 *  |Status Parameter(s) |                                   |
 *  ----------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  Called directly from ARM via REX scheduler.  This function is used by PHY 
 *  to check if API sent by the MAC has errors.
 *
 *
 * \ingroup group_lte_phy_control_rt_mac_phy
 *
 **/
U32 LteBsApiErrorCheck(U32 LteBsApiErrorCheckCtxLocal)
{
    PLTE_BS_API_ERROR_CHECK_CTX pLteBsApiErrorCheckCtx = (PLTE_BS_API_ERROR_CHECK_CTX )LteBsApiErrorCheckCtxLocal;
    PLTE_BS_API_ERROR_CHECK pLteBsApiErrorCheck = pLteBsApiErrorCheckCtx->pLteBsApiErrorCheck;
    PMAC2PHY_QUEUE_EL pApiList                  = pLteBsApiErrorCheckCtx->pApiList;
    U32 isDummy                                 = pLteBsApiErrorCheckCtx->isDummy;

    PMAC2PHY_QUEUE_EL pCurrMsg;
    PGENMSGDESC pMsgHeader;
    PTXSDUREQ pTxSduReq;
    PTXSDUREQ pTxSduReqStore[MAXCHSUBFRAME*2];
    UINT32    numTxSdu = 0;
    UINT32 sfn, localFirstSfn = 0;

    PDLSUBFRDESC pDlSubFrDesc;
    PDLCHANDESC  pDlChanDesc;
    PDCICHANNELDESC pDlDciChanDesc;
    UINT32 offsetDCIChannels;
    UINT32 numCodeWords;
    UINT32 i, j, k, l, count;
    UINT32 retVal = 0;

    _ASSERT_PTR(pLteBsApiErrorCheck);

    // Reset Everything in Structre to 0
    pLteBsApiErrorCheck->numTxVector   = 0;
    pLteBsApiErrorCheck->numRxVector   = 0;
    pLteBsApiErrorCheck->numHARQVector = 0;

    pLteBsApiErrorCheck->numPdschSdu   = 0;
    pLteBsApiErrorCheck->numPdcchSdu   = 0;
    pLteBsApiErrorCheck->numPbchSdu    = 0;
    pLteBsApiErrorCheck->numPhichSdu   = 0;
    pLteBsApiErrorCheck->numDciSdu     = 0;

    pLteBsApiErrorCheck->pHiDciMsg     = NULL;

    pCurrMsg = pApiList;
    while (pCurrMsg)
    {
        pMsgHeader = (PGENMSGDESC) pCurrMsg->MessagePtr;

        switch (pMsgHeader->msgType)
        {
            case PHY_TXSTART_REQ:
                pLteBsApiErrorCheck->numTxVector++;

                pLteBsApiErrorCheck->lenTxVector = (U16) pMsgHeader->msgSpecific;
                pLteBsApiErrorCheck->pTxVector = (PDLSUBFRDESC) (pMsgHeader + 1);

                sfn = LteBsConstructSfn(pLteBsApiErrorCheck->pTxVector->frameNumber,
                                        pLteBsApiErrorCheck->pTxVector->subframeNumber);

                if (*pLteBsApiErrorCheckCtx->pgFirstMessageArrived == 0)
                {
                    *pLteBsApiErrorCheckCtx->pgLastTxSfn = sfn;
                    localFirstSfn = 1;
                }

                pLteBsApiErrorCheckCtx->lastTxSfn = sfn;
                (*pLteBsApiErrorCheckCtx->pgTTIPauseResumeCount)++;
            break;

            case PHY_RXSTART_REQ:
                pLteBsApiErrorCheck->numRxVector++;

                pLteBsApiErrorCheck->lenRxVector = (U16) pMsgHeader->msgSpecific;
                pLteBsApiErrorCheck->pRxVector = (PULSUBFRDESC) (pMsgHeader + 1);

                sfn = LteBsConstructSfn(pLteBsApiErrorCheck->pRxVector->frameNumber,
                                        pLteBsApiErrorCheck->pRxVector->subframeNumber);

                if (*pLteBsApiErrorCheckCtx->pgFirstMessageArrived == 0)
                {
                    *pLteBsApiErrorCheckCtx->pgLastRxSfn = sfn;
                    localFirstSfn = 1;
                }

                pLteBsApiErrorCheckCtx->lastRxSfn = sfn;
            break;

            case PHY_TXHIADCIUL_REQ:
                pLteBsApiErrorCheck->numHARQVector++;
                pLteBsApiErrorCheck->pHiDciMsg = (PHIADCIULMSGDESC)pMsgHeader;
            break;

            case PHY_TXHISDU_REQ:
                pLteBsApiErrorCheck->pHichSdu[pLteBsApiErrorCheck->numPhichSdu++] = (PHIINFOMSGDESC) pMsgHeader;
            break;

            case PHY_TXDCIULSDU_REQ:
                pLteBsApiErrorCheck->pDciSdu[pLteBsApiErrorCheck->numDciSdu++] = (PDCIULSDUMSG) pMsgHeader;
            break;

            case PHY_TXSDU_REQ:
                pTxSduReq = (PTXSDUREQ) pMsgHeader;
                pTxSduReqStore[numTxSdu++] = pTxSduReq;
            break;

            default:
            break;
        }

        pCurrMsg = pCurrMsg->Next;
    }

    if (*pLteBsApiErrorCheckCtx->pgFirstMessageArrived == 0)
        *pLteBsApiErrorCheckCtx->pgFirstMessageArrived = localFirstSfn;

    // Sort the TxSdus we got by ChanId
    if (numTxSdu)
        LteBsApiErrorCheckSortTxSdu(pLteBsApiErrorCheck, pTxSduReqStore, numTxSdu);

    if (*pLteBsApiErrorCheckCtx->pgFirstMessageArrived && pLteBsApiErrorCheckCtx->errorCheckEnabled)
    {
        // Check if this is dummy API. If it is, then no need to do Error Checks
        if (isDummy == 0)
        {
            // General stuff
            if (pLteBsApiErrorCheck->numTxVector != 1 ||
                pLteBsApiErrorCheck->numRxVector != 1 ||
                pLteBsApiErrorCheck->numHARQVector > 1 ||
                pLteBsApiErrorCheck->pTxVector == NULL ||
                pLteBsApiErrorCheck->pRxVector == NULL)
            {
                retVal |= LTE_BS_API_ERROR_CHECK_INVALID_NUM_API;
            }

            // Check All DL API issues
            //-----------------------------
            pDlSubFrDesc = pLteBsApiErrorCheck->pTxVector;

            // BCH Presence
            //--------------
            if (pLteBsApiErrorCheckCtx->isInternalBch == 0)
            {
                if ((pLteBsApiErrorCheck->numPbchSdu == 0) && ((pLteBsApiErrorCheckCtx->lastTxSfn % 40) == 0))
                    retVal |= LTE_BS_API_ERROR_CHECK_INTERNAL_BCH_ERR;
            }


            // DCI Offset
            if (pDlSubFrDesc->numCtrlChannels)
            {
                offsetDCIChannels = pDlSubFrDesc->offsetDCIChannels;
                if(offsetDCIChannels > ((U32)&pDlSubFrDesc->dciCh - (U32)pDlSubFrDesc)
                        || (offsetDCIChannels & (sizeof (U32) - 1)) != 0)
                {
                    retVal |= LTE_BS_API_ERROR_CHECK_TXVEC_DCI_OFFSET_ERR;
                }
            }

            // DL CONTROL
            if (pDlSubFrDesc->numCtrlChannels)
            {
                offsetDCIChannels = pDlSubFrDesc->offsetDCIChannels;
                pDlDciChanDesc = (PDCICHANNELDESC) ((U32) pDlSubFrDesc + (U32) offsetDCIChannels);
                for (i = 0; i < (UINT32) pDlSubFrDesc->numCtrlChannels; i++)
                {
                    // Chan ID in TxVector for DCI
                    if(pDlDciChanDesc->channelId != i)
                        retVal |= LTE_BS_API_ERROR_CHECK_TXVEC_INVALID_CH_ID;

                    // Chan ID in TxSDU
                    if (pTxSduReqStore[i]->chanId != i)
                        retVal |= LTE_BS_API_ERROR_CHECK_TXSDU_INVALID_CH_ID;

                    // Chan Type should be the same between TxSDU and TXVector for same ChanId
                    if (pTxSduReqStore[i]->channelType != pDlDciChanDesc->channelType)
                        retVal |= LTE_BS_API_ERROR_CHECK_TXSDU_INVALID_CH_TYPE;

                    pDlDciChanDesc++;
                }
            }

            // DL DATA
            if (pDlSubFrDesc->numberofChannelDescriptors - pDlSubFrDesc->numCtrlChannels)
            {
                count = (UINT32) pDlSubFrDesc->numCtrlChannels;
                for (i = count, j = 0; i < (UINT32) pDlSubFrDesc->numberofChannelDescriptors; i++, j++)
                {
                    pDlChanDesc = &pDlSubFrDesc->dlCh[j];

                    // Chan ID in TxVector for DCI
                    if(pDlChanDesc->channelId != i)
                        retVal |= LTE_BS_API_ERROR_CHECK_TXVEC_INVALID_CH_ID;

                    if(pDlChanDesc->channelType == PDSCH)
                        numCodeWords = (UINT32)pDlChanDesc->numCodeWords;
                    else
                        numCodeWords = 1;

                    for (k = 0; k < numCodeWords; k++)
                    {
                        // Chan ID in TxSDU
                        if (pTxSduReqStore[count]->chanId != i)
                            retVal |= LTE_BS_API_ERROR_CHECK_TXSDU_INVALID_CH_ID;

                        // Chan Type should be the same between TxSDU and TXVector for same ChanId
                        if (pTxSduReqStore[count]->channelType != pDlChanDesc->channelType)
                            retVal |= LTE_BS_API_ERROR_CHECK_TXSDU_INVALID_CH_TYPE;

                        // Qm
                        if ((pDlChanDesc->channelType == PDSCH) && (pDlChanDesc->subChInfo[k].modulationType == 0))
                            retVal |= LTE_BS_API_ERROR_CHECK_TXVEC_INVALID_QM;

                        count++;
                    }

                    // RBs
                    if (pDlChanDesc->channelType == PDSCH)
                    {
                        if (pDlChanDesc->mapInfo.numberofEntries == 0)
                            retVal |= LTE_BS_API_ERROR_CHECK_TXVEC_INVALID_NUM_ENTR;
                        else
                        {
                            for (k = 0, l = 0; k < pDlChanDesc->mapInfo.numberofEntries; k++)
                            {
                                if (pDlChanDesc->mapInfo.reselmInfo[k].numRes)
                                {
                                    l = 1;
                                    break;
                                }
                            }

                            if (l == 0)
                                retVal |= LTE_BS_API_ERROR_CHECK_TXVEC_ZERO_RBS;
                        }
                    }
                }
            }

            // Check HI/DCI Issues
            //------------------
            if (pLteBsApiErrorCheck->pHiDciMsg)
            {
                if(pLteBsApiErrorCheck->numPhichSdu != pLteBsApiErrorCheck->pHiDciMsg->numHiSdus)
                {
                    retVal |= LTE_BS_API_ERROR_CHECK_HISDU_NUM_ERR;
                }
            }

            if(pLteBsApiErrorCheck->pHiDciMsg)
            {
                if(pLteBsApiErrorCheck->numDciSdu != pLteBsApiErrorCheck->pHiDciMsg->numDciUlSdus)
                {
                    retVal |= LTE_BS_API_ERROR_CHECK_DCISDU_NUM_ERR;
                }
            }

            // Check All UL API issues
            //-----------------------------
            // TO DO

            if (pLteBsApiErrorCheckCtx->robustControlEnabled)
            {
                // If Robustness is enabled, then we substitute existing APIs from L2 with Dummy APIs
                if (retVal)
                {
                    // Reset Everything in Structre to 0
                    pLteBsApiErrorCheck->numTxVector   = 0;
                    pLteBsApiErrorCheck->numRxVector   = 0;
                    pLteBsApiErrorCheck->numHARQVector = 0;

                    pLteBsApiErrorCheck->numPdschSdu   = 0;
                    pLteBsApiErrorCheck->numPdcchSdu   = 0;
                    pLteBsApiErrorCheck->numPbchSdu    = 0;
                    pLteBsApiErrorCheck->numPhichSdu   = 0;
                    pLteBsApiErrorCheck->numDciSdu     = 0;

                    pCurrMsg = LtePhyApiErrorCheckUseEmptyChannels(pLteBsApiErrorCheckCtx);

                    // TxVector
                    pMsgHeader = (PGENMSGDESC) pCurrMsg->MessagePtr;
                    pLteBsApiErrorCheck->numTxVector++;
                    pLteBsApiErrorCheck->lenTxVector = (U16) pMsgHeader->msgSpecific;
                    pLteBsApiErrorCheck->pTxVector = (PDLSUBFRDESC) (pMsgHeader + 1);

                    // Next Message
                    pCurrMsg = pCurrMsg->Next;

                    // RxVector
                    pMsgHeader = (PGENMSGDESC) pCurrMsg->MessagePtr;
                    pLteBsApiErrorCheck->numRxVector++;
                    pLteBsApiErrorCheck->lenRxVector = (U16) pMsgHeader->msgSpecific;
                    pLteBsApiErrorCheck->pRxVector = (PULSUBFRDESC) (pMsgHeader + 1);
                }
            }

        }
    }

    // Clean it up for next API arrival 10 sfn later
    if (pLteBsApiErrorCheck->numTxVector)
        pLteBsApiErrorCheckCtx->apiSfn[pLteBsApiErrorCheck->pTxVector->subframeNumber] = NULL;

    return retVal;
}

#endif  // defined(EVM) || defined(RT_API_PROC)
#endif  // _WIN32


#ifndef _WIN32
#if defined(EVM) || defined(RT_API_PROC)

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
//----------------------------------------------------------------------------------------------------
U32 InsertPBCH_RT(U32 pSduDescIn)
{
    PLTE_PHY_SDU_DESC pSduDesc      = (PLTE_PHY_SDU_DESC)pSduDescIn;
    PDLTXINPUTPARAMS pTxInputParams = (PDLTXINPUTPARAMS) pSduDesc->pTxInputParams;
    U8* scrOut                      = pSduDesc->pBchScrOut;
    U8* inDataPtr                   = pSduDesc->pBchSdu;
    U32 numBytes                    = pSduDesc->bchNumBytes;
    U8* pSrc                        = (U8*) pTxInputParams->pBchDataBuffer;

    MDMA_FDESC *pMdma, *pMdmaStart;

    if (InitializeDriver(MDMA_SYS))
        return 1;

    // Frames 1, 2, 3
    if((pTxInputParams->txFrameNumber & 0x3) != 0)
    {
        //Append a channel at the end of the TXVECTOR with the saved PBCH info
        memcpy(scrOut, pTxInputParams->ptrPBCH, PBCHDATASIZE/8);
        pTxInputParams->ptrPBCH += PBCHDATASIZE/8;

    }
    else        // Frame 0
    {
        pTxInputParams->ptrPBCH = pTxInputParams->startPBCH;

        if (pSduDesc->gPhyInternalBch)
        {
            pSrc += (PBCH_LEN_IN_BYTES * (pTxInputParams->txFrameNumber>>2));

            memcpy(pTxInputParams->startPBCH, pSrc, PBCH_LEN_IN_BYTES);
            memcpy(scrOut, pTxInputParams->ptrPBCH, PBCHDATASIZE/8);

            pTxInputParams->ptrPBCH += PBCHDATASIZE/8;
        }
        else
        {
            bsTxBchChannelCoding(inDataPtr, pTxInputParams->startPBCH, (S32) numBytes,
                pTxInputParams->Ntx_antennas, PBCH_LENGTH_IN_BITS);

            pMdmaStart = MdmaDrvAllocDesc (MDMA_HW_SYS, 2);
            _ASSERT_PTR (pMdmaStart);

            MdmaDrvSet4GSCR(pMdmaStart, pTxInputParams->Ncell_id, 1, 0, 0);
            MdmaDrvSetData(pMdmaStart, (U32*) pTxInputParams->startPBCH, PBCHDATASIZE/2,
                                    (U32*) pTxInputParams->startPBCH, PBCHDATASIZE/2);
            pMdma = pMdmaStart->Next;
            MdmaDrvSetCopy(pMdma);
            MdmaDrvSetData(pMdma, (U32*) pTxInputParams->startPBCH, PBCHDATASIZE/8,
                                (U32*) scrOut, PBCHDATASIZE/8);

            pSduDesc->tBchStart = MxGetTicks();
#ifdef CEVA_INTRINSICS
            MdmaDrvSetLastDescArm(pMdmaStart, (void (*)(LPVOID)) pSduDesc->pbchCB, (void*) pSduDesc);
            MdmaDrvRunQ(MDMA_HW_SYS, pMdmaStart, 300);
#else
            MdmaRunExQ (MDMA_HW_SYS, pMdmaStart, (void (*)(LPVOID)) pSduDesc->pbchCB,
                (void*) pSduDesc, 300);
#endif
            pTxInputParams->ptrPBCH += PBCHDATASIZE/8;
        }
    }

    return 0;
}
#endif  // defined(EVM) || defined(RT_API_PROC)
#endif  // _WIN32


