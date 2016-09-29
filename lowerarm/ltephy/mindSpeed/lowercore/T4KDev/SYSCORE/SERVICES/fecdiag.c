//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL fecdiag.c
 *
 * @brief The Fec diagnostics module
 * @author Mindspeed Technologies
 * @version $Revision: 1.26 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include "diagnostics.h"
#include "fecdiag.h"
#include "fecdiagdata.h"
#include "FECdrv.h"
#include "mdmadrv.h"
#include "heaps.h"
#include "larmmsgid.h"
#include "appprintf.h"
#include "cevafecdrv.h"

#define FECDIAG_DEBUG 0
PFEC_DIAG_CTX FecDiagCxt = NULL;


//////////////////////////////////////////////////////////////////////////
PUINT8 MemCmp(PUINT8 p1, PUINT8 p2, UINT32 length)
{
    while (length--)
    {
        if (*p1 != *p2)
            return p1;
        p1++, p2++;
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////////
MXRC FecDiagRun(PFEC_DIAG_CTX pCtx, ApiHeader * pApi, UINT32 *nCycles)
{

	return (MX_OK);

#if 0

    PFECDRV_CTX pFecDrvCtx = FECDrv_ctx();
    volatile FEC_CTRL_REGS * pFecCtrl = pFecDrvCtx->FecCtrlRegs;
    volatile MDMA_CTRL_REGS * pMdmaCtrl = (MDMA_CTRL_REGS *)0xFE400000;
    ApiParam * pTaskID;
    ApiParam * pResID;
    MXRC rc = MX_OK;
    PUINT32 pHwContext;
    UINT32  FecDiagOptions;
    PUINT32 p_regs;
    PDMAFDESC p_dmadesc_0;
    PDMAFDESC p_dmadesc;
    PDMABDESC p_inbdesc;
    PFEC_CONFIG_REGS pFecConfigReg;
    UINT32 n_desc;
    UINT32 n_dup_desc;
    //UINT32 i;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pApi);
    _ASSERT_PTR(pCtx->Tcb.InputDataPtr);

    pTaskID = ApiGetParamByID(pApi, DIAG_TASK_PARAM_ID);
    pResID  = ApiGetParamByID(pApi, DIAG_RES_PARAM_ID);

    pCtx->Tcb.TaskID = 0;
    pCtx->Tcb.ResourceID  = 0;

    if (pCtx->Tcb.InputDataPtr == NULL)
    {
        rc = RC_FECDIAG_INPUT_ERROR;
        _ASSERT_RC(rc);

        return rc;
    }

    if (pTaskID != NULL)
    {
        pCtx->Tcb.TaskID = pTaskID->ParamData [0];
    }

    if (pResID != NULL)
    {
        pCtx->Tcb.ResourceID = pResID->ParamData [0];
    }

    if (FAILED(rc = DiagAllocOutput(pCtx->hFecHeap, &pCtx->Tcb, pApi)))
    {
        _ASSERT_RC(rc);

        return rc;
    }

    pCtx->Tcb.Status = 0;

    FecDiagOptions = pCtx->Tcb.ResourceID;

    if (DIAG_FEC_CTX_TYPE(FecDiagOptions) == DIAG_FEC_SW_CTX)
    {
        // switching FEC HW mode (WiMAX -> LTE or LTE -> WiMAX)
        if (FECDrv_query_hw_mode() != DIAG_FEC_TYPE(FecDiagOptions))
        {
            FECDrv_shutdown();
            rc = FECDrv_init(DIAG_FEC_TYPE(FecDiagOptions));
            if (rc != MX_OK)
                return rc;
        }

#ifdef FEC_NON_BLOCK_SUPPORT
        rc = FECDrv_proc(&pCtx->Tcb, TRUE);
#else
        rc = FECDrv_proc(&pCtx->Tcb);
#endif
    }
    else
    {
        FECDrv_reset(); // ?

        // FEC_HW_CONTEXT is provided by host via Context Buffer
        pHwContext = pCtx->Tcb.ContextPtr;
        pFecConfigReg = (PFEC_CONFIG_REGS)pCtx->Tcb.ContextPtr;
        p_dmadesc_0 = (PDMAFDESC)((UINT8 *)pCtx->Tcb.ContextPtr + sizeof(FEC_CONFIG_REGS));
        p_dmadesc = p_dmadesc_0;
        n_desc = 0;
        while (1)
        {
            n_desc++;
            p_dmadesc->OutBDesc.BPtr = (UINT32)pCtx->Tcb.OutputDataPtr;
            p_inbdesc = &p_dmadesc->InBDesc;
#if FECDIAG_DEBUG
            uart_printf ("p_dmadesc=0x%x OutBDesc.BPtr=0x%x\r InputDataPtr=%x\n", p_dmadesc, p_dmadesc->OutBDesc.BPtr, pCtx->Tcb.InputDataPtr);
#endif
            uart_printf ("W0=0x%x W1=0x%x W2=0x%x W3=0x%x\r\n", p_dmadesc->NextFDesc, p_dmadesc->FControl, p_dmadesc->FStatus0, p_dmadesc->FStatus1);
            do
            {
                p_inbdesc->BPtr = (UINT32)pCtx->Tcb.InputDataPtr + (UINT32)p_inbdesc->BPtr;
#if FECDIAG_DEBUG
                uart_printf ("BPtr=0x%x BCtrl=0x%x\r\n", p_inbdesc->BPtr, p_inbdesc->BCtrl);
#endif
            } while ((p_inbdesc++->BCtrl & BCTRL_BLAST_MASK) == 0);

            if (p_dmadesc->NextFDesc == 0)
                break;
            p_dmadesc->NextFDesc = (PDMAFDESC)((UINT32)p_dmadesc->NextFDesc + (UINT32)p_dmadesc_0);
            p_dmadesc = p_dmadesc->NextFDesc;
        }
        uart_printf("n_desc=%d\n", n_desc);

        n_dup_desc = 0;
/*
        if (n_desc == 1)
        {
            for (i = 0; i < 10; i++)
            {
                *(p_dmadesc + 1) = *p_dmadesc;
                p_dmadesc->NextFDesc = p_dmadesc + 1;
                p_dmadesc = p_dmadesc->NextFDesc;
                n_dup_desc++;
            }
            p_dmadesc--;
            p_dmadesc->NextFDesc = NULL;
        }
*/
        // force max burst size
        pFecConfigReg->inbound_dma_burst_len = 15;
        pFecConfigReg->outbound_dma_burst_len = 15;
        //uart_printf("Burst %d %d\n", pFecConfigReg->inbound_dma_burst_len, pFecConfigReg->outbound_dma_burst_len);

        p_regs = (UINT32 *)&pFecDrvCtx->FecCtrlRegs->FecConfigRegs;
        if (pCtx->Tcb.TaskID == TASKID_FEC_TX)
        {
#if FECDIAG_DEBUG
            uart_printf("FecConfig0=%x\r\n", pHwContext[0]);
#endif
            *p_regs++ = pHwContext[0];
            *p_regs++ = pHwContext[1];
            *p_regs++ = pHwContext[2];
            pFecCtrl->DMA0InCtrl.BurstLength = pFecConfigReg->inbound_dma_burst_len;
            pFecCtrl->DMA0OutCtrl.BurstLength = pFecConfigReg->outbound_dma_burst_len;
            pFecCtrl->DMA0InCtrl.Control = 4;

            *nCycles = Get24MHzTick();

            pFecCtrl->DMA0OutCtrl.HeadFDesc = (UINT32)p_dmadesc_0;
            pFecCtrl->DMA0InCtrl.HeadFDesc = (UINT32)p_dmadesc_0;
        }
        else if (pCtx->Tcb.TaskID == TASKID_FEC_RX)
        {
#if 1//FECDIAG_DEBUG
            uart_printf("FecConfig0=%x\r\n", pHwContext[0]);
#endif
            *p_regs++ = pHwContext[0];
            *p_regs++ = pHwContext[1];
            *p_regs++ = pHwContext[2];
            pFecCtrl->DMA1InCtrl.BurstLength = pFecConfigReg->inbound_dma_burst_len;
            pFecCtrl->DMA1OutCtrl.BurstLength = pFecConfigReg->outbound_dma_burst_len;
            pFecCtrl->DMA1InCtrl.Control = 4;

            *nCycles = Get24MHzTick();

            pFecCtrl->DMA1OutCtrl.HeadFDesc = (UINT32)p_dmadesc_0;
            pFecCtrl->DMA1InCtrl.HeadFDesc = (UINT32)p_dmadesc_0;
        }
        else
        { // MDMA
            pMdmaCtrl->DMAInCtrl.BurstLength = pFecConfigReg->inbound_dma_burst_len;
            pMdmaCtrl->DMAOutCtrl.BurstLength = pFecConfigReg->outbound_dma_burst_len;
            pMdmaCtrl->DMAInCtrl.Control = 4;
            pMdmaCtrl->DMAOutCtrl.HeadFDesc = (UINT32)p_dmadesc;
            pMdmaCtrl->DMAInCtrl.HeadFDesc = (UINT32)p_dmadesc;
        }

        pFecDrvCtx->ExecTicks = FECDrv_wait_complete(p_dmadesc);

        *nCycles = Get24MHzTick() - *nCycles;
        *nCycles = *nCycles / (1+n_dup_desc);

        uart_printf("%x %x %d\n", *nCycles, Get24MHzTick(), *nCycles);

        if (pFecDrvCtx->ExecTicks == 0)
        {
            uart_printf("FEC timeout\n");
            rc = RC_FECDIAG_RUN_TIMEOUT;
        }
        else
        {
#if FECDIAG_DEBUG
        {
            UINT32 i;
            UINT8 *p8;

            p8 = (UINT8 *)pCtx->Tcb.InputDataPtr;
            for (i = 0; i < pCtx->Tcb.InputDataLen; i++)
                uart_printf("%02x ", p8[i]);
            uart_printf("\r\n");

            p8 = (UINT8 *)pCtx->Tcb.OutputDataPtr;
            for (i = 0; i < pCtx->Tcb.OutputDataLen; i++)
                uart_printf("%02x ", p8[i]);
            uart_printf("\r\n");
        }
#endif
            if (DIAG_FEC_CTX_TYPE(FecDiagOptions) == DIAG_FEC_HW_CTX)
            {
                p_regs = (UINT32 *)&pFecDrvCtx->FecCtrlRegs->FecConfigRegs;
                pHwContext[4] = p_regs[4];
                pHwContext[5] = p_regs[5];
            }

            uart_printf ("FEC ExecTicks=%d\n", pFecDrvCtx->ExecTicks);
        }
    }

    return rc;

#endif
}

MXRC FecDiag(UINT32 taskid, BOOL bBlock)
{

	return MX_OK;
#if 0
    MXRC rc = MX_OK;
    PFECDRV_CTX pFecDrvCtx = FECDrv_ctx();
    PTCB pTcb = NULL;
    PFEC_IO_PARAMS pFecParams = NULL;
    UINT32 InDataLen = 0, OutDataLen = 0;
    PUINT8 pInData = NULL, pOutData = NULL, pRefData = NULL;
    PUINT8 pName = NULL, pOffset = NULL;

    return rc;

    pFecParams = MxHeapAlloc(DspHeap, sizeof(*pFecParams));
    _ASSERT_PTR (pFecParams);

    memset(pFecParams, 0, sizeof(*pFecParams));

    if (pFecDrvCtx->FecCtrlRegs->FecConfigRegs.wimax_lte == FEC_HWCONF_LTE)
    {
/*
        pFecParams->CodingType = 1;
        pFecParams->BlockSize = 40;
        pFecParams->CrcSelector = 0;
        pFecParams->FillerBits = 0;
        pFecParams->Modulation = 0;
        pFecParams->BpBlockInterleaving = 0;
        pFecParams->HalfIterations = 32;
        pFecParams->BpRateMatching = 1;
        pFecParams->BpChannelInterleaving = 1;
        if (taskid == TASKID_FEC_TX)
        {
            pName = "LTE DL";
            InDataLen = sizeof(LTEEncBuf);
            OutDataLen = sizeof(LTEEncRefBuf);
            pInData = (PUINT8)LTEEncBuf;
            pRefData = (PUINT8)LTEEncRefBuf;
        }
        else if (taskid == TASKID_FEC_RX)
        {
            pName = "LTE UL";
            InDataLen = sizeof(LTEDecBuf);
            OutDataLen = sizeof(LTEEncBuf);
            pInData = (PUINT8)LTEDecBuf;
            pRefData = (PUINT8)LTEEncBuf;
        }
*/
/*
        pFecParams->CodingType = 1;
        pFecParams->BlockSize = 4736;
        pFecParams->CrcSelector = 0;
        pFecParams->FillerBits = 24;
        pFecParams->Modulation = 6; // QAM64
        pFecParams->BpBlockInterleaving = 0;
        pFecParams->HalfIterations = 6;
        pFecParams->BpRateMatching = 0;
        pFecParams->RateMatchingE = 14400;
        pFecParams->RateMatchingk0 = 11026;
        pFecParams->RateMatchingNcb = 14304;
        pFecParams->BpChannelInterleaving = 1;
        if (taskid == TASKID_FEC_TX)
        {
            pName = "LTE DL";
            InDataLen = sizeof(LTEEncBufRm);
            OutDataLen = sizeof(LTEEncRefBufRm);
            pInData = (PUINT8)LTEEncBufRm;
            pRefData = (PUINT8)LTEEncRefBufRm;
        }
        else if (taskid == TASKID_FEC_RX)
        {
            pName = "LTE UL";
            InDataLen = sizeof(LTEDecBufRm);
            OutDataLen = sizeof(LTEEncBufRm);
            pInData = (PUINT8)LTEDecBufRm;
            pRefData = (PUINT8)LTEEncBufRm;
        }
*/
        pFecParams->CodingType = 1;
        pFecParams->BlockSize = 496;
        pFecParams->CrcSelector = 2; // CRC24b
        pFecParams->FillerBits = 88;
        pFecParams->Modulation = 2; // QPSK
        pFecParams->BpBlockInterleaving = 0;
        pFecParams->HalfIterations = 32;
        pFecParams->BpRateMatching = 0;
        pFecParams->RateMatchingE = 3072;
        pFecParams->RateMatchingk0 = 18;
        pFecParams->RateMatchingNcb = 1536;
        pFecParams->BpChannelInterleaving = 0;
        pFecParams->CiSymbolNumber = 14;

        if (taskid == TASKID_FEC_TX)
        {
            pName = "LTE DL";
#if 0
            // Checking of DL Broadcast channel FEC
            pFecParams->CodingType = 0;
            pFecParams->BlockSize = 160;
            pFecParams->CrcSelector = 4; // CRC16b
            pFecParams->FillerBits = 0;
            pFecParams->Modulation = 0; // BPSK
            pFecParams->CodingRate = 0;
            pFecParams->HalfIterations = 16;
            pFecParams->DynamicStopIteration = 0;
            pFecParams->DynamicStopThreshold = 0;
            pFecParams->ScrambleEnable = 1;
            pFecParams->ScrambleMask = 0;
            pFecParams->SubpacketId = 0;
            pFecParams->BpBlockInterleaving = 0;
            pFecParams->BpEncoder = 0;
            pFecParams->BpDecoder = 0;
            pFecParams->EnableCombine = 0;
            pFecParams->BpRateMatching = 0;
            pFecParams->RateMatchingE = 480;
            pFecParams->RateMatchingk0 = 0;
            //pFecParams->RateMatchingNcb = 480;
            pFecParams->BpChannelInterleaving = 1;
            pFecParams->CiSymbolNumber = 12;

            InDataLen = sizeof(LTEEncBufCi3DlBcast);
            OutDataLen = sizeof(LTEEncRefBufCi3DlBcast);
            pInData = (PUINT8)LTEEncBufCi3DlBcast;
            pRefData = (PUINT8)LTEEncRefBufCi3DlBcast;
#else
            InDataLen = sizeof(LTEEncBufCi);
            OutDataLen = sizeof(LTEEncRefBufCi);
            pInData = (PUINT8)LTEEncBufCi;
            pRefData = (PUINT8)LTEEncRefBufCi;
#endif
        }
        else if (taskid == TASKID_FEC_RX)
        {
            pName = "LTE UL";
            InDataLen = sizeof(LTEDecBufCi);
            OutDataLen = sizeof(LTEEncBufCi);
            pInData = (PUINT8)LTEDecBufCi;
            pRefData = (PUINT8)LTEEncBufCi;
        }
    }
    else
    {
        pFecParams->CodingType = 1;
        pFecParams->BlockSize = 6;
        pFecParams->Modulation = 2; // QPSK
        pFecParams->CodingRate = 2; // 1/2
        pFecParams->BpBlockInterleaving = 0;
        pFecParams->HalfIterations = 16;
        if (taskid == TASKID_FEC_TX)
        {
            pName = "WiMAX DL";
            InDataLen = sizeof(WiMAXEncBuf);
            OutDataLen = sizeof(WiMAXEncRefBuf);
            pInData = (PUINT8)WiMAXEncBuf;
            pRefData = (PUINT8)WiMAXEncRefBuf;
        }
        else if (taskid == TASKID_FEC_RX)
        {
            pName = "WiMAX UL";
            InDataLen = sizeof(WiMAXDecBuf);
            OutDataLen = sizeof(WiMAXEncBuf);
            pInData = (PUINT8)WiMAXDecBuf;
            pRefData = (PUINT8)WiMAXEncBuf;
        }
    }

    pTcb = MxHeapAlloc(DspHeap, sizeof(*pTcb) + 16*sizeof(TIOBufDesc));
    _ASSERT_PTR (pTcb);

    memset(pTcb, 0, sizeof(*pTcb) + 2*sizeof(TIOBufDesc));

    pTcb->TaskID = taskid;
    pTcb->ContextPtr = pFecParams;
    pTcb->cbDone = CbDoneFec;

    CreateTCBIOBuf(pTcb, 2);

    {
        PUINT8 pInData2 = pInData;
        pInData = MxHeapAlloc(DspHeap, InDataLen);
        memcpy(pInData, pInData2, InDataLen);
    }

    pOutData = MxHeapAlloc(DspHeap, OutDataLen);
    memset(pOutData, 0, OutDataLen);
    FillTCBIOBuf(pTcb, pOutData, OutDataLen, OUT_BUF, NULL);
    if (pFecDrvCtx->FecCtrlRegs->FecConfigRegs.wimax_lte == FEC_HWCONF_WIMAX ||
        taskid == TASKID_FEC_TX || pFecParams->BpChannelInterleaving == 1)
    {
        FillTCBIOBuf(pTcb, pInData, InDataLen, IN_BUF, NULL);
    }
    else
    {
        UINT16 blen = InDataLen/pFecParams->CiSymbolNumber, i;
        for (i = 0; i < pFecParams->CiSymbolNumber; ++i)
        {
            FillTCBIOBuf(pTcb, pInData + blen*i, blen - *(LTEDecInsBufCi + i)/*pFecParams->CiInsertions[i]*/, IN_BUF, NULL);
        }
    }
    if (!bBlock)
    {
        // to compare
        pTcb->OutputDataPtr = pRefData;
        pTcb->OutputDataLen = OutDataLen;
    }

#ifdef FEC_NON_BLOCK_SUPPORT
    rc = FECDrv_proc(pTcb, bBlock);
#else
    rc = FECDrv_proc(pTcb);
#endif

    if (bBlock)
    {
        uart_printf("FEC complete. ExecTicks=%d\n", pTcb->ExecTicks);
    	if ((pOffset = MemCmp(pOutData, pRefData, OutDataLen)) == NULL)
        {
            uart_printf("FEC %s test passed\n", pName);
        }
    	else
        {
            uart_printf("FEC %s test failed: offset 0x%X\n", pName, pOffset - pOutData);
#if 0
            {
               UINT32 i, Delay;
                uart_printf("Bytes:");
                for(i=0; i<OutDataLen; i++)
                {
                    uart_printf("%d. %2.2X\r\n", i, pOutData[i]);
                    Delay = 1000000;
                    while(Delay--);
                }
            }
#endif
        }

        MxHeapFree(DspHeap, pOutData);
        MxHeapFree(DspHeap, pInData);
        MxHeapFree(DspHeap, pFecParams);
        MxHeapFree(DspHeap, pTcb);

    }

    return rc;
#endif
}

//////////////////////////////////////////////////////////////////////////
void CbDoneMdma(PTCB pTcb)
{
#if 0
    PMDMA_IO_PARAMS pMdmaParams = (PMDMA_IO_PARAMS)pTcb->ContextPtr;
    PTIOBufDesc pBDescs = pTcb->IOControlPtr;
    PUINT8 pName = NULL, pOutData = pBDescs[0].IOBufPtr, pOffset = NULL;

    switch (pMdmaParams->CalcType)
    {
    case MDMA_OP_COPY:
        pName = "copy";
        break;
    case MDMA_OP_CRC:
        pName = "CRC";
        break;
    case MDMA_OP_4SCR:
        pName = "4G Scrambler";
        break;
    case MDMA_OP_4DSC:
        pName = "4G Descrambler";
        break;
    }

    uart_printf("MDMA complete. ExecTicks=%d (cpu-%d)\n", pTcb->ExecTicks, MxGetCpuID ());

    if ((pOffset = MemCmp(pOutData, pTcb->OutputDataPtr, pTcb->OutputDataLen)) == NULL)
        uart_printf("MDMA %s test passed\n", pName);
    else
        uart_printf("MDMA %s test failed: offset 0x%X\n", pName, pOffset - pOutData);

    MxHeapFree(DspHeap, pOutData);
    MxHeapFree(DspHeap, pMdmaParams);
    MxHeapFree(DspHeap, pTcb);
#endif
}

//////////////////////////////////////////////////////////////////////////
MXRC MdmaDiag(UINT32 taskid, BOOL bBlock)
{
#if 0
    MXRC rc = MX_OK;
    PTCB pTcb = NULL;
    PMDMA_IO_PARAMS pMdmaParams = NULL;
    UINT32 InDataLen = 0, OutDataLen = 0;
    PUINT8 pInData = NULL, pOutData = NULL, pRefData = NULL;
    PUINT8 pName = NULL, pOffset = NULL;

    pMdmaParams = (PMDMA_IO_PARAMS)MxHeapAlloc(DspHeap, sizeof(*pMdmaParams) + 1000);

    _ASSERT_PTR (pMdmaParams);

    memset(pMdmaParams, 0, sizeof(*pMdmaParams));
    pMdmaParams->CalcType = taskid;

    switch (taskid)
    {
    case MDMA_OP_COPY:
        pName = "copy";
        InDataLen = OutDataLen = sizeof(MDMAInBuf_Copy);
        pInData = pRefData = (PUINT8)MDMAInBuf_Copy;
        break;
    case MDMA_OP_CRC:
        pName = "CRC";
        InDataLen = sizeof(MDMAInBuf_Crc);
        OutDataLen = sizeof(MDMARefBuf_Crc);
        pInData = (PUINT8)MDMAInBuf_Crc;
        pRefData = (PUINT8)MDMARefBuf_Crc;
        pMdmaParams->PolyValue = 0x9b;
        pMdmaParams->PolyOrder = 3;
        pMdmaParams->CrcInit = 0;
        break;
    case MDMA_OP_4SCR:
        pName = "4G Scrambler";
/*
        InDataLen = sizeof(MDMAInBuf_4GScrambler);
        OutDataLen = sizeof(MDMARefBuf_4GScrambler);
        pInData = (PUINT8)MDMAInBuf_4GScrambler;
        pRefData = (PUINT8)MDMARefBuf_4GScrambler;
        MdmaParams.CInit0 = 0x01011100;
        MdmaParams.CInit1 = 0x00000001;
        MdmaParams.PackedMode = 0;
*/
        InDataLen = sizeof(MDMAInBuf_4GScrambler1);
        OutDataLen = sizeof(MDMARefBuf_4GScrambler1);
        pInData = (PUINT8)MDMAInBuf_4GScrambler1;
        pRefData = (PUINT8)MDMARefBuf_4GScrambler1;
        pMdmaParams->CInit0 = 0x01011100;
        pMdmaParams->CInit1 = 0x00000001;
        pMdmaParams->PackedMode = 1;
        break;
    case MDMA_OP_4DSC:
        pName = "4G Descrambler";
        InDataLen = sizeof(MDMAInBuf_4GDescrambler);
        OutDataLen = sizeof(MDMARefBuf_4GDescrambler);
        pInData = (PUINT8)MDMAInBuf_4GDescrambler;
        pRefData = (PUINT8)MDMARefBuf_4GDescrambler;
        pMdmaParams->CInit0 = 0x01011100;
        pMdmaParams->CInit1 = 0x00000001;
        break;
    default:
        MxHeapFree(DspHeap, pMdmaParams);
        return RC_MDMA_DRV_INVALID_OP_CODE;
    }

    pTcb = (PTCB)MxHeapAlloc(DspHeap, sizeof(*pTcb) + OutDataLen + 1024);

    _ASSERT_PTR (pTcb);

    memset(pTcb, 0, sizeof(*pTcb));

    pTcb->ContextPtr = pMdmaParams;
    pTcb->cbDone = CbDoneMdma;

    CreateTCBIOBuf(pTcb, 2);

    pOutData = MxHeapAlloc(DspHeap, OutDataLen);
    memset(pOutData, 0, OutDataLen); //memcpy(pOutData, pInData, InDataLen);
    FillTCBIOBuf(pTcb, pOutData, OutDataLen, OUT_BUF, NULL);
    FillTCBIOBuf(pTcb, pInData, InDataLen, IN_BUF, NULL); //FillTCBIOBuf(pTcb, pOutData, InDataLen, IN_BUF, NULL);
    if (!bBlock)
    {
        // to compare
        pTcb->OutputDataPtr = pRefData;
        pTcb->OutputDataLen = OutDataLen;
    }

    //rc = MdmaDrvProc(pTcb, bBlock);

    if (bBlock)
    {
        uart_printf("MDMA complete. ExecTicks=%d\n", pTcb->ExecTicks);
        if ((pOffset = MemCmp(pOutData, pRefData, OutDataLen)) == NULL)
        {
            uart_printf("MDMA %s test passed\n", pName);
        }
        else
            uart_printf("MDMA %s test failed: offset 0x%X\n", pName, pOffset - pOutData);

        MxHeapFree(DspHeap, pOutData);
        MxHeapFree(DspHeap, pMdmaParams);
        MxHeapFree(DspHeap, pTcb);
    }

    return rc;
#endif

    return MX_OK;

}

MXRC MdmaDiagEx(MDMA_HW_ID MdmaId, UINT32 taskid, BOOL bBlock)
{
#if 0
    MXRC rc = MX_OK;
    PTCB pTcb = NULL;
    PMDMA_IO_PARAMS pMdmaParams = NULL;
    UINT32 InDataLen = 0, OutDataLen = 0;
    PUINT8 pInData = NULL, pOutData = NULL, pRefData = NULL;
    PUINT8 pName = NULL, pOffset = NULL;

    pMdmaParams = (PMDMA_IO_PARAMS)MxHeapAlloc(DspHeap, sizeof(*pMdmaParams) + 1000);

    _ASSERT_PTR (pMdmaParams);

    memset(pMdmaParams, 0, sizeof(*pMdmaParams));
    pMdmaParams->CalcType = taskid;

    switch (taskid)
    {
    case MDMA_OP_COPY:
        pName = "copy";
        InDataLen = OutDataLen = sizeof(MDMAInBuf_Copy);
        pInData = pRefData = (PUINT8)MDMAInBuf_Copy;
        break;
    case MDMA_OP_CRC:
        pName = "CRC";
        InDataLen = sizeof(MDMAInBuf_Crc);
        OutDataLen = sizeof(MDMARefBuf_Crc);
        pInData = (PUINT8)MDMAInBuf_Crc;
        pRefData = (PUINT8)MDMARefBuf_Crc;
        pMdmaParams->PolyValue = 0x9b;
        pMdmaParams->PolyOrder = 3;
        pMdmaParams->CrcInit = 0;
        break;
    case MDMA_OP_4SCR:
        pName = "4G Scrambler";
/*
        InDataLen = sizeof(MDMAInBuf_4GScrambler);
        OutDataLen = sizeof(MDMARefBuf_4GScrambler);
        pInData = (PUINT8)MDMAInBuf_4GScrambler;
        pRefData = (PUINT8)MDMARefBuf_4GScrambler;
        MdmaParams.CInit0 = 0x01011100;
        MdmaParams.CInit1 = 0x00000001;
        MdmaParams.PackedMode = 0;
*/
        InDataLen = sizeof(MDMAInBuf_4GScrambler1);
        OutDataLen = sizeof(MDMARefBuf_4GScrambler1);
        pInData = (PUINT8)MDMAInBuf_4GScrambler1;
        pRefData = (PUINT8)MDMARefBuf_4GScrambler1;
        pMdmaParams->CInit0 = 0x01011100;
        pMdmaParams->CInit1 = 0x00000001;
        pMdmaParams->PackedMode = 1;
        break;
    case MDMA_OP_4DSC:
        pName = "4G Descrambler";
        InDataLen = sizeof(MDMAInBuf_4GDescrambler);
        OutDataLen = sizeof(MDMARefBuf_4GDescrambler);
        pInData = (PUINT8)MDMAInBuf_4GDescrambler;
        pRefData = (PUINT8)MDMARefBuf_4GDescrambler;
        pMdmaParams->CInit0 = 0x01011100;
        pMdmaParams->CInit1 = 0x00000001;
        break;
    default:
        MxHeapFree(DspHeap, pMdmaParams);
        return RC_MDMA_DRV_INVALID_OP_CODE;
    }

    pTcb = (PTCB)MxHeapAlloc(DspHeap, sizeof(*pTcb) + OutDataLen + 1024);

    _ASSERT_PTR (pTcb);

    memset(pTcb, 0, sizeof(*pTcb));

    pTcb->ContextPtr = pMdmaParams;
    pTcb->cbDone = CbDoneMdma;

    CreateTCBIOBuf(pTcb, 2);

    pOutData = MxHeapAlloc(DspHeap, OutDataLen);

    memset(pOutData, 0, OutDataLen); //memcpy(pOutData, pInData, InDataLen);
    FillTCBIOBuf(pTcb, pOutData, OutDataLen, OUT_BUF, NULL);
    FillTCBIOBuf(pTcb, pInData, InDataLen, IN_BUF, NULL); //FillTCBIOBuf(pTcb, pOutData, InDataLen, IN_BUF, NULL);
    if (!bBlock)
    {
        // to compare
        pTcb->OutputDataPtr = pRefData;
        pTcb->OutputDataLen = OutDataLen;
    }

    //rc = MdmaDrvProcEx(MdmaId, pTcb, bBlock);

    if (bBlock)
    {
        uart_printf("MDMA%d complete. ExecTicks=%d (cpu-%d)\n", MdmaId, pTcb->ExecTicks, MxGetCpuID ());
        if ((pOffset = MemCmp(pOutData, pRefData, OutDataLen)) == NULL)
        {
            uart_printf("MDMA%d %s test passed (%d InBytes, %d OutBytes)\n", MdmaId, pName, InDataLen, OutDataLen);
        }
        else
            uart_printf("MDMA%d %s test failed: offset 0x%X (%d InBytes, %d OutBytes)\n", MdmaId, pName, pOffset - pOutData,
                InDataLen, OutDataLen);

        MxHeapFree(DspHeap, pOutData);
    }

    return rc;
#endif

    return MX_OK;

}
void CbDoneDLFec(LPVOID pRes)
{
    PUINT8 pWimaxLte = "LTE", pOffset;
    PUINT8 pOutData = FecDiagCxt->pDLOutData;
    UINT32 res = RC_FECDIAG_OK;

    if ((pOffset = MemCmp(pOutData, FecDiagCxt->pDLRefData, FecDiagCxt->DLOutLen)) == NULL)
    {
		//uart_printf("%s FEC DL complete (cb). ExecTicks=%d. Test Passed\n", pWimaxLte, FecDlGetExecTicks());
    }
    else
    {
        uart_printf("%s FEC DL complete (cb). ExecTicks=%d. Test Failed [0x%x] \n", pWimaxLte, FecDlGetExecTicks(), pOffset - pOutData);
		res = RC_FECDIAG_DL_BITEXACT_ERROR;
    }

    *(UINT32*)pRes = res;
    MxHeapFree(DspHeap, pOutData);
}

void CbDoneULFec(LPVOID pRes)
{
    PUINT8 pWimaxLte = "LTE", pOffset;
    PUINT8 pOutData = FecDiagCxt->pULOutData;
    UINT32 res = RC_FECDIAG_OK;

    if ((pOffset = MemCmp(pOutData, FecDiagCxt->pULRefData, FecDiagCxt->ULOutLen)) == NULL)
    {
	    //uart_printf("%s FEC UL complete (cb). ExecTicks=%d. Test Passed\n", pWimaxLte, FecUlGetExecTicks());
    }
    else
    {
        uart_printf("%s FEC UL complete (cb). ExecTicks=%d. Test Failed [0x%x] \n", pWimaxLte, FecUlGetExecTicks(), pOffset - pOutData);
		res = RC_FECDIAG_UL_BITEXACT_ERROR;
    }
    *(UINT32*)pRes = res;
    MxHeapFree(DspHeap, pOutData);
}

static MXRC FecUlProc(PFEC_IO_PARAMS pFecParams)
{
    PFEC_UL_FDESC pFDescHead = NULL;
    ULHWCTX ULHwCtx = {0};
    PUINT8 pInData = NULL, pOutData = NULL;
    MXRC rc = MX_OK;

    pFDescHead = FecUlAllocDesc(1);
    _ASSERT_PTR(pFDescHead);
	if(!pFDescHead)
		return 0;

    pInData =  FecDiagCxt->pULInData;
    pOutData = FecDiagCxt->pULOutData;
#if 0
{
	const Ul_496_24b_BLen[14] = {0xF0, 0xF0, 0xF0, 0x8C, 0x8C, 0xC8, 0xC8, 0xEC, 0xEC, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0};

	delta = 0;
	for(i = 0; i <14; i++)
		{
			pFDescHead->IoBdesc[i + 1].BPtr = (VUINT32)((UINT32)pInData + delta);
			pFDescHead->IoBdesc[i + 1].BCtrl = 0x80000000 | Ul_496_24b_BLen[i];
			delta += Ul_496_24b_BLen[0];
			uart_printf("%d)BPtr %x BCtrl %x\n",i+1, (UINT32)((UINT32)(pFDescHead->IoBdesc[i + 1].BPtr) - (UINT32)pInData), pFDescHead->IoBdesc[i + 1].BCtrl);
		}
		pFDescHead->IoBdesc[i].BCtrl |= BCTRL_BLAST_MASK;

		pFDescHead->IoBdesc[0].BPtr = (UINT32)pOutData;
		pFDescHead->IoBdesc[0].BCtrl = 0x80000000 | BCTRL_BLAST_MASK | FecDiagCxt->ULOutLen;

		memcpy(&ULHwCtx,&LTE_UL_HwCtx, sizeof(ULHWCTX));
}
#else
    FecDlSetData((PFEC_DL_FDESC)pFDescHead, pInData, FecDiagCxt->ULInLen, pOutData, FecDiagCxt->ULOutLen);
    ULHwCtx.CTC_CCn = pFecParams->CodingType;
    ULHwCtx.CRC_Sel = pFecParams->CrcSelector;
    ULHwCtx.Modulation = pFecParams->Modulation / 2;
    ULHwCtx.Rate_Index = pFecParams->CodingRate;
    ULHwCtx.BlockSize = pFecParams->BlockSize; // block size in bits
    ULHwCtx.Dynamic_Stop_Iteration = pFecParams->DynamicStopIteration;
    ULHwCtx.Dynamic_Stop_Threshold = pFecParams->DynamicStopThreshold;
    ULHwCtx.HalfInteration = pFecParams->HalfIterations;
    ULHwCtx.Bypass_Rate_Match = pFecParams->BpRateMatching;
    ULHwCtx.Filler = pFecParams->FillerBits;
    ULHwCtx.LTERateMaching_K0 = pFecParams->RateMatchingk0 / 2;
    ULHwCtx.E_Param = pFecParams->RateMatchingE;
    ULHwCtx.Enable_Combine = pFecParams->EnableCombine;
    ULHwCtx.Bypass_Decode_Enable = pFecParams->BpDecoder;
#endif
    FecUlSetHwCtx(pFDescHead, ULHwCtx);
    // start DMA

    rc = FecUlSetLastDesc(pFDescHead, CbDoneULFec, (LPVOID)&FecDiagCxt->ULTestRes);
    rc = FecUlRun(pFDescHead);
    return rc;
}

#if FEC_DL_CEVAX_OWNER == 0
static MXRC FecDlProc(PFEC_IO_PARAMS pFecParams)
{
    PFEC_DL_FDESC pFDescHead = NULL;
    MXRC rc = MX_OK;
    PUINT8 pInData = NULL, pOutData = NULL;
    DLHWCTX DLHwCtx = {0};

    pFDescHead = FecDlAllocDesc(1);
    _ASSERT_PTR(pFDescHead);
	if(!pFDescHead)
		return 0;

    pInData =  FecDiagCxt->pDLInData;
    pOutData = FecDiagCxt->pDLOutData;

    rc = FecDlSetData(pFDescHead, pInData, FecDiagCxt->DLInLen, pOutData, FecDiagCxt->DLOutLen);

    DLHwCtx.CTC_CCn = pFecParams->CodingType;
    DLHwCtx.CRC_Sel = pFecParams->CrcSelector;
    DLHwCtx.Modulation = pFecParams->Modulation;
    DLHwCtx.Rate_Index = pFecParams->CodingRate;
    DLHwCtx.BlockSize = pFecParams->BlockSize; // block size in bits
    DLHwCtx.Bypass_Encoder = pFecParams->BpEncoder;
    DLHwCtx.Bypass_Interleaver = pFecParams->BpBlockInterleaving;
    DLHwCtx.Harq_req = pFecParams->BpRateMatching; // bit 14 for LTE is BypassRateMatching
    DLHwCtx.Filler = pFecParams->FillerBits;
    DLHwCtx.LTERateMaching_K0 = pFecParams->RateMatchingk0;
    DLHwCtx.E_Param = pFecParams->RateMatchingE;
    DLHwCtx.Ncb_14_13= (pFecParams->RateMatchingNcb >> 13) & 3;

    pFDescHead->IoBdesc[1].BCtrl |= (8 * FecDiagCxt->DLOutLen) << 18;

    FecDlSetHwCtx(pFDescHead, DLHwCtx);
    FecDlSetLastDesc(pFDescHead, CbDoneDLFec, (LPVOID)&FecDiagCxt->DLTestRes);
    FecDlRun(pFDescHead);

    return rc;
}
#endif

static MXRC FecDlCevaProc(PFEC_IO_PARAMS pFecParams)
{
    PFEC_DL_FDESC pFDescHead = NULL;
    MXRC rc = MX_OK;
    PUINT8 pInData = NULL, pOutData = NULL;
    DLHWCTX DLHwCtx = {0};

    rc = FecDlDrvOpen();

    if (FAILED(rc))
    {
        uart_printf ("FEC-DRV-CEVA failed to open device, rc=%x\r\n", rc);
        return rc;
    }

    pFDescHead = FecDlDrvAllocDesc(1);
    _ASSERT_PTR(pFDescHead);
    
	if(!pFDescHead)
    {   
        FecDlDrvClose();
        uart_printf ("FEC-DRV-CEVA failed to allocate fdesc\r\n");
		return RC_FECDIAG_ALLOC_ERROR;
    }

    pInData =  FecDiagCxt->pDLInData;
    pOutData = FecDiagCxt->pDLOutData;

    if (pOutData != NULL)
        memset (pOutData, 0x11, FecDiagCxt->DLOutLen);

    rc = FecDlDrvSetData(pFDescHead, pInData, FecDiagCxt->DLInLen, pOutData, FecDiagCxt->DLOutLen);

    DLHwCtx.CTC_CCn = pFecParams->CodingType;
    DLHwCtx.CRC_Sel = pFecParams->CrcSelector;
    DLHwCtx.Modulation = pFecParams->Modulation;
    DLHwCtx.Rate_Index = pFecParams->CodingRate;
    DLHwCtx.BlockSize = pFecParams->BlockSize; // block size in bits
    DLHwCtx.Bypass_Encoder = pFecParams->BpEncoder;
    DLHwCtx.Bypass_Interleaver = pFecParams->BpBlockInterleaving;
    DLHwCtx.Harq_req = pFecParams->BpRateMatching; // bit 14 for LTE is BypassRateMatching
    DLHwCtx.Filler = pFecParams->FillerBits;
    DLHwCtx.LTERateMaching_K0 = pFecParams->RateMatchingk0;
    DLHwCtx.E_Param = pFecParams->RateMatchingE;
    DLHwCtx.Ncb_14_13= (pFecParams->RateMatchingNcb >> 13) & 3;

    pFDescHead->IoBdesc[1].BCtrl |= (8 * FecDiagCxt->DLOutLen) << 18;

    FecDlDrvSetHwCtx(pFDescHead, DLHwCtx);
    FecDlDrvSetLastDesc(pFDescHead, CbDoneDLFec, (LPVOID)&FecDiagCxt->DLTestRes);
    
    rc = FecDlDrvRun(pFDescHead);

    if (FAILED(rc))
    {
        FecDlDrvClose();
        uart_printf ("FEC-DL-CEVA run error, rc=0x%x\r\n", rc);
        return rc;
    }

    rc = FecDlDrvWaitCompletion(NULL, ~0);

    if (FAILED(rc))
    {
        FecDlDrvClose();
        uart_printf ("FEC-DL-CEVA waiting error, rc=0x%x\r\n", rc);
        return rc;
    }

    rc = FecDlDrvClose();

    return rc;
}

#if FEC_DL_CEVAX_OWNER == 0
static MXRC FecDlDiag(void)
{
    MXRC rc = MX_OK;
    PFEC_IO_PARAMS pFecParams = NULL;
    PUINT8  pOutData = NULL;

	pFecParams = MxHeapAlloc(DspHeap, sizeof(*pFecParams));

	if(pFecParams == NULL)
		return RC_FECDIAG_ALLOC_ERROR;

	memset(pFecParams, 0, sizeof(*pFecParams));

	pFecParams->CodingType = 1;
	pFecParams->BlockSize = 0x28;
	pFecParams->CrcSelector = 0;
	pFecParams->FillerBits = 0;
	pFecParams->Modulation = 0;
	pFecParams->BpBlockInterleaving = 0;
	pFecParams->HalfIterations = 32;
	pFecParams->BpRateMatching = 1;
	pFecParams->RateMatchingE = 0;
	pFecParams->RateMatchingk0 = 0;
	pFecParams->RateMatchingNcb = 0;
	pFecParams->BpChannelInterleaving = 0;
	pFecParams->CiSymbolNumber = 14;

	FecDiagCxt->DLInLen= sizeof(LTEEncBuf);
	FecDiagCxt->DLOutLen = sizeof(LTEEncRefBuf);

	pOutData = MxHeapAlloc(DspHeap, FecDiagCxt->DLOutLen);
	if(pOutData == NULL)
		return RC_FECDIAG_ALLOC_ERROR;

	memset(pOutData, 0, FecDiagCxt->DLOutLen);

	FecDiagCxt->pDLInData = (PUINT8)LTEEncBuf;
	FecDiagCxt->pDLOutData = pOutData;
	FecDiagCxt->pDLRefData = (PUINT8)LTEEncRefBuf;

	rc = FecDlProc(pFecParams);

	MxHeapFree(DspHeap, pFecParams);
    return rc;
}
#endif

MXRC FecDlCevaDiag(void)
{
    MXRC rc = MX_OK;
    PFEC_IO_PARAMS pFecParams = NULL;
    PUINT8  pOutData = NULL;

	pFecParams = MxHeapAlloc(DspHeap, sizeof(*pFecParams));

	if(pFecParams == NULL)
		return RC_FECDIAG_ALLOC_ERROR;

	memset(pFecParams, 0, sizeof(*pFecParams));

	pFecParams->CodingType = 1;
	pFecParams->BlockSize = 0x28;
	pFecParams->CrcSelector = 0;
	pFecParams->FillerBits = 0;
	pFecParams->Modulation = 0;
	pFecParams->BpBlockInterleaving = 0;
	pFecParams->HalfIterations = 32;
	pFecParams->BpRateMatching = 1;
	pFecParams->RateMatchingE = 0;
	pFecParams->RateMatchingk0 = 0;
	pFecParams->RateMatchingNcb = 0;
	pFecParams->BpChannelInterleaving = 0;
	pFecParams->CiSymbolNumber = 14;

	FecDiagCxt->DLInLen= sizeof(LTEEncBuf);
	FecDiagCxt->DLOutLen = sizeof(LTEEncRefBuf);

	pOutData = MxHeapAlloc(DspHeap, FecDiagCxt->DLOutLen);
	if(pOutData == NULL)
		return RC_FECDIAG_ALLOC_ERROR;

	memset(pOutData, 0, FecDiagCxt->DLOutLen);

	FecDiagCxt->pDLInData = (PUINT8)LTEEncBuf;
	FecDiagCxt->pDLOutData = pOutData;
	FecDiagCxt->pDLRefData = (PUINT8)LTEEncRefBuf;

	rc = FecDlCevaProc(pFecParams);

	MxHeapFree(DspHeap, pFecParams);
    return rc;
}

static MXRC FecUlDiag(void)
{
    MXRC rc = MX_OK;
    PFEC_IO_PARAMS pFecParams = NULL;
    PUINT8  pOutData = NULL;

    pFecParams = MxHeapAlloc(DspHeap, sizeof(*pFecParams));

	if(pFecParams == NULL)
		return RC_FECDIAG_ALLOC_ERROR;

    memset(pFecParams, 0, sizeof(*pFecParams));
#if 0

	pFecParams->CodingType = 1;
	pFecParams->BlockSize = 496;
	pFecParams->CrcSelector = 2; // CRC24b
	pFecParams->FillerBits = 88;
	pFecParams->Modulation = 2; // QPSK
	pFecParams->BpBlockInterleaving = 0;
	pFecParams->HalfIterations = 32;
	pFecParams->BpRateMatching = 1;
	pFecParams->RateMatchingE = 3072;
	pFecParams->RateMatchingk0 = 18;
	pFecParams->RateMatchingNcb = 1536;
	pFecParams->BpChannelInterleaving = 0;
	pFecParams->CiSymbolNumber = 14;

	FecDiagCxt->ULInLen = sizeof(LTEDecBufCi);
	FecDiagCxt->ULOutLen = sizeof(LTEEncBufCi);
	FecDiagCxt->pULInData = (PUINT8)LTEDecBufCi;
	FecDiagCxt->pULRefData = (PUINT8)LTEEncBufCi;
#endif

    pFecParams->CodingType = 1;
    pFecParams->BlockSize = 40;
    pFecParams->BpBlockInterleaving = 0;
    pFecParams->HalfIterations = 32;
    pFecParams->BpRateMatching = 1;

    FecDiagCxt->ULInLen = sizeof(LTEDecBuf_);
    FecDiagCxt->ULOutLen = sizeof(LTEEncBuf_);
    FecDiagCxt->pULInData = (PUINT8)LTEDecBuf_;
	FecDiagCxt->pULRefData = (PUINT8)LTEEncBuf_;

    pOutData = MxHeapAlloc(DspHeap, FecDiagCxt->ULOutLen);

	if(pOutData == NULL)
		return RC_FECDIAG_ALLOC_ERROR;

    memset(pOutData, 0, FecDiagCxt->ULOutLen);
    FecDiagCxt->pULOutData = pOutData;

    rc =  FecUlProc(pFecParams);
    MxHeapFree(DspHeap, pFecParams);
    return rc;
    }

MXRC FecDiagInit(UINT32* pReportData, UINT32* pReportSize)
{
	MXRC rc = MX_OK;
	FEC_CONFIG_REGS FecConfigRegs;
	UINT32 StartTick;

	FecDiagCxt = MxHeapAlloc(AppHeap, sizeof(FEC_DIAG_CTX));

	memset(FecDiagCxt, 0, sizeof(FEC_DIAG_CTX));

	memset(&FecConfigRegs, 0 , sizeof(FEC_CONFIG_REGS));
	FecConfigRegs.wimax_lte = FEC_HWCONF_LTE;
	FecConfigRegs.bp_channel_interleaver = 0;
	FecConfigRegs.ci_symbol_number = 0;

	if(FAILED(rc = FecInit(&FecConfigRegs)))
	{
		MxHeapFree(AppHeap, FecDiagCxt);
		return RC_FECDIAG_INIT_ERROR;
	}

	FecDiagCxt->DLTestRes = RC_FECDIAG_UNDER_TEST;
	FecDiagCxt->ULTestRes = RC_FECDIAG_UNDER_TEST;

    #if FEC_DL_CEVAX_OWNER == 0
    	if(FAILED(rc = FecDlDiag()))
    	{
    		uart_printf("FEC DL diagnostic init done failure %d \n", rc);
    		FecShutdown();
    		MxHeapFree(AppHeap, FecDiagCxt);
    		return rc;
    	}
    #else
        FecDiagCxt->DLTestRes = MX_OK;
    #endif

	if(FAILED(rc = FecUlDiag()))
	{
		uart_printf("FEC UL diagnostic init done failure %d \n", &rc);
		FecShutdown();
		MxHeapFree(AppHeap, FecDiagCxt);
		return rc;
	}

	StartTick = Get24MHzTick();

    while((FecDiagCxt->DLTestRes == RC_FECDIAG_UNDER_TEST) ||
          (FecDiagCxt->ULTestRes == RC_FECDIAG_UNDER_TEST))
    {
		if (Get24MHzTick() - StartTick > 150*1000*1000)
        {
            rc = RC_FECDIAG_RUN_TIMEOUT;
            break;
        }
    }

    if (FAILED(FecDiagCxt->DLTestRes))
    {
        uart_printf ("FEC-DL IRQ handler was not called\r\n");
        rc = FecDiagCxt->DLTestRes;
    }

    if (FAILED(FecDiagCxt->ULTestRes))
    {
        uart_printf ("FEC-UL IRQ handler was not called\r\n");
        rc = FecDiagCxt->ULTestRes;
    }

    #if FEC_DL_CEVAX_FDESC_NUM > 0
    if (SUCCESSFUL(rc))
    {
        FecDiagCxt->DLTestRes = RC_FECDIAG_UNDER_TEST;
        rc = FecDlCevaDiag ();

        if (FAILED(FecDiagCxt->DLTestRes))
        {
            uart_printf ("FEC-DL-CEVA CBDONE handler was not called\r\n");
            rc = FecDiagCxt->DLTestRes;
        }
    }
    #endif

	//rc = FecDiagCxt->DLTestRes | FecDiagCxt->ULTestRes;

	FecShutdown();
	MxHeapFree(AppHeap, FecDiagCxt);

    return rc;
}


//////////////////////////////////////////////////////////////////////////
APIHEC FecDiagApiHandler (LPVOID pData, ApiHeader * pApi, ApiHeader * pResp)
{
    MXRC rc = MX_OK;
    PFEC_DIAG_CTX pCtx = (PFEC_DIAG_CTX)pData;
    UINT32 nCycles;

    if (pApi->Type == API_TYPE_CFG_PARAM)
    {
        pResp->Type = API_TYPE_CFG_RESP;

        switch (pApi->MessageID)
        {
            case DIAG_RUN:
                {
                    //PFECDRV_CTX pFecDrvCtx = FECDrv_ctx();
                    //rc = FecDiagRun(pCtx, pApi, &nCycles);
                    //nCycles = pFecDrvCtx->ExecTicks;
                    break;
                }

            case DIAG_CONTEXT:
                {
                    rc = DiagSetContext(pCtx->hFecHeap, &pCtx->Tcb, pApi);
                    break;
                }

            case DIAG_CONTROL:
                {
                    uart_printf ("FEC Control - not used in FEC diag\r\n");
                    break;
                }

            case DIAG_INPUT:
                {
                    rc = DiagSetInput(pCtx->hFecHeap, &pCtx->Tcb, pApi);
                    break;
                }

            default:
                {
                    rc = RC_FECDIAG_UNKNOWN_API;
                    break;
                }
        }
    }
    else if (pApi->Type == API_TYPE_QRY_PARAM)
    {
        pResp->Type = API_TYPE_QRY_RESP;

        switch (pApi->MessageID)
        {
            case DIAG_CONTEXT:
                {
                    rc = DiagGetContext(&pCtx->Tcb, pResp);
                    break;
                }

            case DIAG_CONTROL:
                {
                    uart_printf ("FEC Query Control\r\n");
                    break;
                }

            case DIAG_INPUT:
                {
                    rc = DiagGetInput(&pCtx->Tcb, pApi, pResp);
                    break;
                }

            case DIAG_OUTPUT:
                {
                    rc = DiagGetOutput(&pCtx->Tcb, pApi, pResp);
                    break;
                }

            default:
                {
                    rc = RC_FECDIAG_UNKNOWN_API;
                    break;
                }
        }

        if (SUCCESSFUL(rc))
            return APIHEC_OK;
    }
    else
    {
        rc = RC_FECDIAG_UNSUP_TYPE;
    }

    ApiAddErrorCode(pResp, rc);

    if (pApi->MessageID == DIAG_RUN)
    {
        ApiAddParam(pResp, NULL, DIAG_DSP_CYCLES, &nCycles, sizeof (nCycles));
    }

    return APIHEC_OK;
}
