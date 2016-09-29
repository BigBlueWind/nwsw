//-------------------------------------------------------------------------------------------
/** @file cevafpdrv.h
 *
 * @brief The file contains CEVA FP driver used by the REX-CS to run FP TCBs
 * @author Mindspeed Technologies
 * @version  $Revision: 1.1 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
*/
//-------------------------------------------------------------------------------------------

#include "cevafpdrv.h"
#include "cevadrvcfg.h"

#ifdef CEVA_PROJ
#include "cevaprintf.h"
#else
#include "appprintf.h"
#endif

#include "rex.h"

CEVAFPDRVCTX ceva_fp_ctx;

UINT32 IsFpDrvOpened(void) CEVAEXTERN;
CEVAFPDRVCTX * GetFpDrvCtx (void) CEVAEXTERN;
LPVOID TcbGetIOBuf(PTCB pTCB, UINT32 nBufIndex) CEVAEXTERN;

CEVAFPDRVCTX * GetFpDrvCtx (void)
{
    return &ceva_fp_ctx;
}

UINT32 IsFpDrvOpened(void)
{
    return ceva_fp_ctx.Inited;
}

void FpDrvInit(void)
{
    memset (&ceva_fp_ctx, 0, sizeof (ceva_fp_ctx));
}

UINT32 FpDrvOpen(void)
{
    PCEVA_DRV_CFG pCfg = CEVA_GET_DRV_CFG();
    CEVAFPDRVCTX * pCtx;
    
    _ASSERT_PTR(pCfg);

    pCtx = GetFpDrvCtx();
    _ASSERT_PTR(pCtx);

    pCfg->FpCfg.IdftRunMask = 0;
    
    pCtx->IdftRunPtr = &pCfg->FpCfg.IdftRunMask;
    pCtx->IdftStart  = pCfg->FpCfg.IdftStart;
    pCtx->IdftStop   = pCfg->FpCfg.IdftStop;
    pCtx->FpDevNum   = pCfg->FpCfg.FpDevNum;
    pCtx->Inited     = 1;

    pCtx->pDmaExpRegs[FP_GRP0] = (PMAP_DMA_EXP_REGS)(0xFD000000 + 0x20000 + 0xE400); // use FPDMA0
    pCtx->pDmaExpRegs[FP_GRP1] = (PMAP_DMA_EXP_REGS)(0xFD000000 + 0x20000 + 0xEC00); // use FPDMA1

    #if 0
    uart_printf ("\r\nIDFT:START=0x%x\r\nIDFT:STOP=0x%x\r\n,IDFT:DEVNUM=%d\r\nDMA[0]=0x%x\r\nDMA[1]=0x%x\r\n",
        pCtx->IdftStart,
        pCtx->IdftStop,
        pCtx->FpDevNum,
        pCtx->pDmaExpRegs[FP_GRP0],
        pCtx->pDmaExpRegs[FP_GRP1]
    );
    #endif

    return MX_OK;
}

UINT32 FpDrvClose (void)
{
    PCEVA_DRV_CFG pCfg = CEVA_GET_DRV_CFG();
    CEVAFPDRVCTX * pCtx;
    
    _ASSERT_PTR(pCfg);

    pCtx = GetFpDrvCtx();
    _ASSERT_PTR(pCtx);

    pCfg->FpCfg.IdftRunMask = 0;

    pCtx->IdftRunPtr = NULL;
    pCtx->IdftStart  = 0;
    pCtx->IdftStop   = 0;
    pCtx->Inited     = 0;

    return 0;
}

void FpDispatchFexp(CEVAFPDRVCTX * pCtx, UINT32 fpID, PDMAFDESC pInFDesc, PDMAFDESC pOutFDesc)
{
    if (fpID < (pCtx->FpDevNum / 2))
    {
        #if 0
        uart_printf ("IDFT RUN:%d (%x %x)DMA:(%x %x)\r\n", fpID, pOutFDesc, pInFDesc, pCtx->pDmaExpRegs[FP_GRP0], pCtx->pDmaExpRegs[FP_GRP1]);
        #endif
    
        pCtx->pDmaExpRegs[FP_GRP0]->FHeadOut[fpID] = (UINT32)pOutFDesc;
        pCtx->pDmaExpRegs[FP_GRP0]->FHeadIn [fpID] = (UINT32)pInFDesc;
    }
    else
    {
        pCtx->pDmaExpRegs[FP_GRP1]->FHeadOut[fpID % (pCtx->FpDevNum / 2)] = (UINT32)pOutFDesc;
        pCtx->pDmaExpRegs[FP_GRP1]->FHeadIn [fpID % (pCtx->FpDevNum / 2)] = (UINT32)pInFDesc;
    }
}

LPVOID TcbGetIOBuf(PTCB pTCB, UINT32 nBufIndex)
{
    TIOBufDesc * pDesc;

    if (pTCB == NULL)
    {
        _ASSERT_PTR(pTCB);
        return NULL;
    }

    if ((nBufIndex * sizeof(TIOBufDesc)) >= TCB_CONTROL_LEN(pTCB))
    {
        _ASSERT((nBufIndex * sizeof(TIOBufDesc)) < TCB_CONTROL_LEN(pTCB));
        return NULL;
    }

    pDesc = (TIOBufDesc *)((UINT32)TCB_CONTROL(pTCB) + (nBufIndex * sizeof(TIOBufDesc)));

    return pDesc->IOBufPtr;
}


UINT32 FpDrvRunTcb (TCB * pTcb, UINT32 start_time)
{
    UINT32 rc = 0;
    CEVAFPDRVCTX * pCtx = GetFpDrvCtx();

    volatile PDMAFDESC pInFDesc, pOutFDesc, pFDesc;
    UINT32 CurrMask, FpId, ResourceIndex;

    PDMAFDESC *pTcbReadyListHead = NULL;
    PDMAFDESC *pTcbReadyListTail = NULL;

    _ASSERT_PTR(pCtx);

    if (!pCtx->Inited)
    {
        if ((rc = FpDrvOpen()) != 0)
            return rc;
    }

    pTcbReadyListHead = (PDMAFDESC*) TcbGetIOBuf(pTcb, 0);
    pTcbReadyListTail = (PDMAFDESC*) TcbGetIOBuf(pTcb, 1);

    #if 0
    uart_printf ("\r\nFP:start:%x stop:%x Res:%x (%x %x)\r\n",

        pCtx->IdftStart, 
        pCtx->IdftStop, 
        pTcb->ResourceIndex,
        pTcbReadyListHead,
        pTcbReadyListTail

    );
    #endif

    ResourceIndex = pTcb->ResourceIndex;

    pTcb->ResourceIndex = 0x1F0;
    
    MLogDevEXEStart(pTcb, 0, start_time);
    pTcb->ExecTicks = start_time;

    FpId = pCtx->IdftStart;
    
    while(ResourceIndex != 0)
    {
        CurrMask = (1 << FpId);
        
        if (CurrMask & ResourceIndex)
        {
            ResourceIndex &= ~CurrMask;
            (*pCtx->IdftRunPtr) |= (1 << FpId);

            pFDesc = pTcbReadyListTail[FpId] + NUM_IDFT_OUTPUT_DESC_START;

            while(1)
            {
                if (pFDesc->NextFDesc == NULL)
                    break;
                
                pFDesc = pFDesc->NextFDesc;
            }

            pFDesc->FStatus0 |= (3 << 20); // enable interrupt
            
            pInFDesc = pTcbReadyListHead[FpId];
            pOutFDesc = pInFDesc + NUM_IDFT_OUTPUT_DESC_START;

            FpDispatchFexp(pCtx, FpId, pInFDesc, pOutFDesc);
        }

        FpId++;
    }

    return 0;
}

UINT32  FpDrvIsIdftDone (void)
{
    CEVAFPDRVCTX * pCtx = GetFpDrvCtx();

    _ASSERT_PTR(pCtx);
    
    if (!IsFpDrvOpened())
        return 0;

    _ASSERT(pCtx->IdftRunPtr != NULL);

    if (*pCtx->IdftRunPtr == 0)
        return 1; 

    return 0;
}

