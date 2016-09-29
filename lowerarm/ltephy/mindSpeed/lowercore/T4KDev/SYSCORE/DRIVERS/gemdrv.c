//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL gemdrv.c
 *
 * @brief The Cadence GEM driver
 * @author Mindspeed Technologies
 * @version $Revision: 1.16 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include "gemdrv.h"
#include "hal.h"
#include "appprintf.h"
#include "mmumgr.h"

GemCtx * GemDrvCtx [2] = {0,0};
GemCtx GlobalGemCtx[2];
extern UINT32 net_prof[];
//#define NET_PROFILE(a)  net_prof[a] = MxGetTicks()
#define NET_PROFILE(a)

volatile HGTxFrm gFrm = NULL;
volatile HGTxFrm gLastFrm = NULL;


/** @brief This function processes various GEM interrupts taken from
            GEM INT status register

 @param pClear [in] - the pointer to the GIC interrupt clear register
 @param nIrq   [in] - the interrupt ID
 @param pParam [in] - the ISR parameter */

void GemDrvIrqHandler(volatile UINT32 * pClear, UINT32 nIrq, LPVOID pParam)
{
    GemCtx * pCtx = (GemCtx*)pParam;
    UINT32 nIrqStat = 0;

    _ASSERT_PTR(pCtx);

    nIrqStat = pCtx->IPRegs->IntStat;

    if (pCtx->IrqProc != NULL)
    {
        pCtx->IrqProc (pCtx->ProcCtx, pCtx, nIrqStat);
    }
    else
    {
        // the packets should be free by the application
        // the application enables/disables RX packets,
        // so it responsible for operations of data free
    }

    // To clean the IRQ bits
    pCtx->IPRegs->IntStat = nIrqStat;

    *pClear = nIrq;
}

/** @brief This function sets a source MAC address of GEM, it's used
           like a pattern for comparation

    @param pDrvCtx [in] - the driver context
    @param pMAC    [in] - the array of MAC address

    @return [MXRC] an error code */

MXRC GemDrvSetMAC (LPVOID pDrvCtx, UINT8 * pMAC)
{
    GemCtx * pCtx = (GemCtx *)pDrvCtx;
    UINT32 nVal;

    _ASSERT_PTR(pDrvCtx);
    _ASSERT_PTR(pMAC);

    GemDrvLock (pCtx);

    nVal = (UINT32)*pMAC++;
	nVal |= (UINT32)*pMAC++ << 8;
	nVal |= (UINT32)*pMAC++ << 16;
	nVal |= (UINT32)*pMAC++ << 24;

    pCtx->IPRegs->SpecAddr1Bottom = nVal;
 
	nVal = (UINT32)*pMAC++;
	nVal |= (UINT32)*pMAC++ << 8;

    pCtx->IPRegs->SpecAddr1Top = nVal;

    GemDrvUnlock (pCtx);

    return GEM_RC_OK;
}

/** @brief This function sets a source MAC address of GEM, it's used
           like a pattern for comparation

    @param pDrvCtx [in] - the driver context
    @param pMAC    [in] - the array of MAC address

    @return [MXRC] an error code */

MXRC GemDrvSetMACEx (LPVOID pDrvCtx, UINT32 ID, UINT8 * pMAC)
{
    GemCtx * pCtx = (GemCtx *)pDrvCtx;
    UINT32 nVal, nVal2;

    _ASSERT_PTR(pDrvCtx);
    _ASSERT_PTR(pMAC);

    GemDrvLock (pCtx);

    nVal =  (UINT32)*pMAC++;
    nVal |= (UINT32)*pMAC++ << 8;
    nVal |= (UINT32)*pMAC++ << 16;
    nVal |= (UINT32)*pMAC++ << 24;

    nVal2=  (UINT32)*pMAC++;
    nVal2|= (UINT32)*pMAC++ << 8;

    switch (ID)
    {
        case 0:
            pCtx->IPRegs->SpecAddr1Bottom = nVal;
            pCtx->IPRegs->SpecAddr1Top = nVal2;
            break;

        case 1:
            pCtx->IPRegs->SpecAddr2Bottom = nVal;
            pCtx->IPRegs->SpecAddr2Top = nVal2;
            break;

        case 2:
            pCtx->IPRegs->SpecAddr3Bottom = nVal;
            pCtx->IPRegs->SpecAddr3Top = nVal2;
            break;

        case 3:
            pCtx->IPRegs->SpecAddr4Bottom = nVal;
            pCtx->IPRegs->SpecAddr4Top = nVal2;
            break;

        default:
            return GEM_RC_MAC_ID_ERROR;

    }

    GemDrvUnlock (pCtx);

    return GEM_RC_OK;
}

/** @brief This function enables/disables the unicast/multicast hash bit

    @param pDrvCtx [in] - the driver context
    @param nHashID [in] - the bit index [0...63]
    @param nEnable [in] - enable/disable bit

    @return [MXRC] error code */

MXRC GemDrvHashCtrl (LPVOID pDrvCtx, UINT32 nHashID, UINT32 nEnable)
{
    GemCtx * pCtx = (GemCtx *)pDrvCtx;
    VUINT32 * pReg;

    _ASSERT_PTR(pDrvCtx);

    if (nHashID < 32)
    {
        pReg = &pCtx->IPRegs->HashRegBottom;
    }
    else
    {
        pReg = &pCtx->IPRegs->HashRegTop;
        nHashID -= 32;
    }

    if (nEnable)
    {
        *pReg |= ((UINT32)1L << nHashID);
    }
    else
    {
        *pReg &= ~((UINT32)1L << nHashID);
    }

    return GEM_RC_OK;
}

/** @brief This function returns an index of the hash bit based on the provided MAC

    @param pMac [in] - the MAC address

    @return [UINT32] an index */

UINT32 GemDrvGetMacHash (PUINT8 pMac)
{
    UINT32 i, tmp = 0;

    for (i = 0; i < 48; i++)
	{
		tmp ^= ((pMac [ (i / 8) ] >> (i%8)) & 1) << (i % 6);
	}

    return tmp;
}

/** @brief This function sets TypeID, it's used
           as a pattern to detect is it "possible" or "not possible" to copy this frame

    @param pDrvCtx [in] - the driver context
    @param nTypeID [in] - the TypeID
    @param nTypeVal[in] - the Type value

    @return [MXRC] an error code */

MXRC GemDrvSetType (LPVOID pDrvCtx, UINT32 nTypeID, UINT32 nTypeVal)
{
    GemCtx * pCtx = (GemCtx *)pDrvCtx;

    _ASSERT_PTR(pCtx);
    _ASSERT(nTypeID < 8);

    if (nTypeID < 4)
    {
        pCtx->IPRegs->TypeID [nTypeID] = (nTypeVal & 0xFFFF) | ((UINT32)1 << 31);
    }
    else
    {
        pCtx->IPRegs->ExtTypeID [nTypeID - 4] = (nTypeVal & 0xFFFF) | ((UINT32)1 << 31);
    }

    return GEM_RC_OK;
}

/** @brief This function resets TypeID, this filter will be disabled

    @param pDrvCtx [in] - the driver context
    @param nTypeID [in] - the TypeID
    @param nTypeVal[in] - the Type value

    @return [MXRC] an error code */

MXRC GemDrvResetType (LPVOID pDrvCtx, UINT32 nTypeID)
{
    GemCtx * pCtx = (GemCtx *)pDrvCtx;

    _ASSERT_PTR(pCtx);
    _ASSERT(nTypeID < 8);

    if (nTypeID < 4)
    {
        pCtx->IPRegs->TypeID [nTypeID] &= ~((UINT32)1L << 31);
    }
    else
    {
        pCtx->IPRegs->ExtTypeID [nTypeID - 4] &= ~((UINT32)1L << 31);
    }

    return GEM_RC_OK;
}



#ifdef GEM_DRV_STATISTICS_ENABLE

/** @brief This function returns GEM statistics

    @param pCtx [in]    -   the driver context
    @param pStat[out]   -   a storage for statictics

    @return [MXRC] an error code */

MXRC GemDrvGetStat (LPVOID pDrvCtx, GemDrvStat * pStat)
{
    GemCtx * pCtx = (GemCtx*)pDrvCtx;

    GemDrvStat * pCtxStat;
    GEMRegs *pRegs;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pStat);

    // These registers reset to zero on a read and stick at all ones when they count to their
    // maximum value. They should be read frequently enough to prevent loss of data

    pCtxStat = &pCtx->Stat;
    pRegs    = pCtx->IPRegs;

    // -----------------------------------------------
    // TX statistics  (TxS)
    // -----------------------------------------------
    pCtxStat->TxSOctetsLow          += pRegs->TxSOctetsLow;
    pCtxStat->TxSOctetsHigh         += pRegs->TxSOctetsHigh;
    pCtxStat->TxSFrmNum             += pRegs->TxSFrmNum;
    pCtxStat->TxSBrdcastNum         += pRegs->TxSBrdcastNum;
    pCtxStat->TxSMulcastNum         += pRegs->TxSMulcastNum;
    pCtxStat->TxSPauseNum           += pRegs->TxSPauseNum;
    pCtxStat->TxSFrm_64Num          += pRegs->TxSFrm_64Num;
    pCtxStat->TxSFrm_65_127Num      += pRegs->TxSFrm_65_127Num;
    pCtxStat->TxSFrm_128_255Num     += pRegs->TxSFrm_128_255Num;
    pCtxStat->TxSFrm_256_511Num     += pRegs->TxSFrm_256_511Num;
    pCtxStat->TxSFrm_512_1023Num    += pRegs->TxSFrm_512_1023Num;
    pCtxStat->TxSFrm_1024_1518Num   += pRegs->TxSFrm_1024_1518Num;
    pCtxStat->TxSFrm_over1518Num    += pRegs->TxSFrm_over1518Num;
    pCtxStat->TxSUndrNum            += pRegs->TxSUndrNum;
    pCtxStat->TxSSingleCollNum      += pRegs->TxSSingleCollNum;
    pCtxStat->TxSMulCollNum         += pRegs->TxSMulCollNum;
    pCtxStat->TxSExcCollNum         += pRegs->TxSExcCollNum;
    pCtxStat->TxSLateCollNum        += pRegs->TxSLateCollNum;
    pCtxStat->TxSDeferredNum        += pRegs->TxSDeferredNum;
    pCtxStat->TxSCrSenseNum         += pRegs->TxSCrSenseNum;

    // -----------------------------------------------
    // RX statistics  (RxS)
    // -----------------------------------------------
    pCtxStat->RxSOctetsLow          += pRegs->RxSOctetsLow;
    pCtxStat->RxSOctetsHigh         += pRegs->RxSOctetsHigh;
    pCtxStat->RxSFrmNum             += pRegs->RxSFrmNum;
    pCtxStat->RxSBrdcastNum         += pRegs->RxSBrdcastNum;
    pCtxStat->RxSMulcastNum         += pRegs->RxSMulcastNum;
    pCtxStat->RxSPauseNum           += pRegs->RxSPauseNum;
    pCtxStat->RxSFrm_64Num          += pRegs->RxSFrm_64Num;
    pCtxStat->RxSFrm_65_127Num      += pRegs->RxSFrm_65_127Num;
    pCtxStat->RxSFrm_128_255Num     += pRegs->RxSFrm_128_255Num;
    pCtxStat->RxSFrm_256_511Num     += pRegs->RxSFrm_256_511Num;
    pCtxStat->RxSFrm_512_1023Num    += pRegs->RxSFrm_512_1023Num;
    pCtxStat->RxSFrm_1024_1518Num   += pRegs->RxSFrm_1024_1518Num;
    pCtxStat->RxSFrm_over1518Num    += pRegs->RxSFrm_over1518Num;
    pCtxStat->RxSUndersizeNum       += pRegs->RxSUndersizeNum;
    pCtxStat->RxSOversizeNum        += pRegs->RxSOversizeNum;
    pCtxStat->RxSJabbersNum         += pRegs->RxSJabbersNum;
    pCtxStat->RxSFrmChkSeqErr       += pRegs->RxSFrmChkSeqErr;
    pCtxStat->RxSLenErr             += pRegs->RxSLenErr;
    pCtxStat->RxSRecvSymErr         += pRegs->RxSRecvSymErr;
    pCtxStat->RxSAlignErr           += pRegs->RxSAlignErr;
    pCtxStat->RxSRecvResErr         += pRegs->RxSRecvResErr;
    pCtxStat->RxSRecvOvrErr         += pRegs->RxSRecvOvrErr;
    pCtxStat->RxSIPChkSumErr        += pRegs->RxSIPChkSumErr;
    pCtxStat->RxSTCPChkSumErr       += pRegs->RxSTCPChkSumErr;
    pCtxStat->RxSUDPChkSumErr       += pRegs->RxSUDPChkSumErr;

    if (pStat != NULL)
        memcpy (pStat, &pCtx->Stat, sizeof (pCtx->Stat));

    return GEM_RC_OK;
}

#endif //GEM_DRV_STATISTICS_ENABLE

/** @brief This function configures Cadence GEM DMA engine

    @param pDrvCtx [in] - the pointer to the driver context
    @param pCfg    [in] - the pointer to DMA configuration parameters

    @return [MXRC] an error code */

MXRC GemDrvConfigDMA (LPVOID pDrvCtx, GemDMACfg * pCfg)
{
    UINT32 nNetCtrl;
    GemCtx * pCtx = (GemCtx*)pDrvCtx;
    UINT32 i, addr;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pCfg);

    // it should be aligned to 4 bytes
    _ASSERT(((UINT32)pCfg->RxBuffers) != 0);

    GemDrvLock (pCtx);

    nNetCtrl = pCtx->IPRegs->NetCtrl;

    GemDrvDisableRX(pDrvCtx);
    GemDrvDisableTX(pDrvCtx);

    // to initialize the TX/RX buffer descriptors

    addr = (UINT32)pCfg->RxBuffers;

    for (i = 0; i < pCfg->RxDescrCount; i++)
    {
        // BIT#0/BIT#1 is set to zero
        pCfg->RxDescr [i].AddrOpt = addr;
        pCfg->RxDescr [i].Descr   = pCfg->RxBufOpt;

        // to shift an address for the next RX buffer
        addr += GEM_DMA_GET_RX_SIZE(pCfg->DMAConfig);
    }
    if (pCfg->RxDescrCount > 0)
        GEM_DMA_RX_SET_WRAP(&pCfg->RxDescr [pCfg->RxDescrCount-1]);

    // to prepare TX buffers

    for (i = 0; i < pCfg->TxDescrCount; i++)
    {
        pCfg->TxDescr [i].AddrOpt = NULL;

        // the used bit is specially set to prevent possible incorrect sending
        pCfg->TxDescr [i].Descr   = GEM_DMA_TX_LAST_BUF | GEM_DMA_TX_USED_BIT;
    }
    if (pCfg->TxDescrCount > 0)
        GEM_DMA_TX_SET_WRAP(&pCfg->TxDescr [pCfg->TxDescrCount-1]);

    // set DMA RX/TX queues
    pCtx->IPRegs->RxQueue    = (UINT32)pCfg->RxDescr;
    pCtx->IPRegs->TxQueue[0] = (UINT32)pCfg->TxDescr;

    pCtx->RxD = (UINT32)pCfg->RxDescr;
    pCtx->TxD = (UINT32)pCfg->TxDescr;

    pCtx->CurRxDP = pCtx->RxD;
    pCtx->CurTxDP = pCtx->TxD;
    pCtx->FreeTxDP= pCtx->TxD;

    pCfg->DMAConfig &= ~(GEM_DMA_BIG_ENDIAN | GEM_DMA_SCHEDULER_ENB | GEM_DMA_ADM_MGR_ENB | 0x1F);

    pCfg->DMAConfig |= GEM_DMA_RX_PB_8K;
    pCfg->DMAConfig |= GEM_DMA_TX_PB_4K;
    pCfg->DMAConfig |= GEM_DMA_BURST_INCR16;
    pCfg->DMAConfig |= GEM_DMA_RX_SW_ALLOC;
    pCfg->DMAConfig |= GEM_DMA_TX_SW_ALLOC;
	pCfg->DMAConfig |= GEM_DMA_CHK_SUM_ENB;

    // set DMA configuration
    pCtx->IPRegs->DMACfg        = pCfg->DMAConfig;
    pCtx->IPRegs->RxGlobalOffs  = 0;

    // to restore the NETWORK CONTROL REGISTER
    pCtx->IPRegs->NetCtrl = nNetCtrl;

    GemDrvUnlock (pCtx);

    return GEM_RC_OK;
}


/** @brief This function resets TX and RX pointers of GEM DMA and
           re-initializes TX and RX buffer descriptors.
           In this way all possible received frames will be discarded

    @param pDrvCtx [in] - the driver context

    @return [MXRC] error code of operation */

MXRC GemDrvResetDMA (LPVOID pDrvCtx)
{
    GemCtx * pCtx= (GemCtx*)pDrvCtx;
    UINT32 nNetCtrl;

    HGRxFrm rxfrm;
    HGTxFrm txfrm;

    _ASSERT_PTR(pCtx);

    if (pCtx->RxD == NULL || pCtx->TxD == NULL)
        return GEM_RC_DMA_IS_NOT_CONFIG;

    GemDrvLock (pCtx);

    nNetCtrl = pCtx->IPRegs->NetCtrl;

    pCtx->IPRegs->NetCtrl &= ~(GEM_NETCTRL_RX_ENB | GEM_NETCTRL_TX_ENB);

    // to wait stop of TX operation

    while (pCtx->IPRegs->TxStatus & GEM_TX_STAT_GO)
    {
        CPU_IDLE();
    }

    // to re-initialize the RX buffer descriptors

    rxfrm = (HGRxFrm)pCtx->RxD;

    while(TRUE)
    {
        rxfrm->Descr = (rxfrm->Descr & (GEM_DMA_RX_WRAP_BIT | GEM_DMA_RX_IRQ_ENB));
        rxfrm->ExDescr = 0;

        if (GEM_DMA_RX_IS_WRAP(rxfrm))
        {
            break;
        }

        rxfrm ++;
    }

    // to prepare TX buffers

    txfrm = (HGTxFrm)pCtx->TxD;

    while(TRUE)
    {
        txfrm->Descr = (txfrm->Descr & GEM_DMA_TX_WRAP) | GEM_DMA_TX_USED_BIT;

        if (GEM_DMA_TX_IS_WRAP(txfrm))
            break;

        txfrm ++;
    }

    // set DMA RX/TX queues
    pCtx->IPRegs->RxQueue    = (UINT32)pCtx->RxD;
    pCtx->IPRegs->TxQueue[0] = (UINT32)pCtx->TxD;

    pCtx->CurRxDP = pCtx->RxD;
    pCtx->CurTxDP = pCtx->TxD;
    pCtx->FreeTxDP= pCtx->TxD;

    // to restore the NETWORK CONTROL REGISTER
    pCtx->IPRegs->NetCtrl = nNetCtrl;

    GemDrvUnlock (pCtx);

    return GEM_RC_OK;

}

int gem_phy_man_idle(void)
{

	GEMRegs* IPRegs;

	IPRegs    = (GEMRegs*)(GEMAC0_BASEADDR + 0xE000);
	
	return ((IPRegs->NetStat) & (1 << 2));
}


UINT16 GemDrvReadPhy (LPVOID pDrvCtx, UINT32 nPhyID, UINT32 nRegID)
{
	volatile UINT32 write_data;
	volatile int timeout = 50000;
	volatile GEMRegs* IPRegs;

	IPRegs    = (GEMRegs*)(GEMAC0_BASEADDR + 0xE000);

    IPRegs->NetCtrl |= GEM_NETCTRL_MDIO_ENB;	// Manager port enable
    IPRegs->NetCfg  &= ~(7 << 18);			// Remove clock divider
    IPRegs->NetCfg  |= GMAC_MDC_DIVIDER << 18;			// 4 = b100 = divider is 64 (sys clock up to 160 Mhz)

	write_data = 0x60020000;
	write_data |= ((nPhyID << 23) | (nRegID << 18));
	IPRegs->PHY = write_data;
	
	while (!gem_phy_man_idle()) 
	{
		if (timeout-- <= 0) 
		{
			uart_printf("PHY MDIO read/write timeout\r\n");
			return 0;
		}
	}
	
	return IPRegs->PHY & 0xFFFF;
}



MXRC GemDrvWritePhy (LPVOID pDrvCtx, UINT32 nPhyID, UINT32 nRegID, UINT16 nData)
{
	volatile int timeout = 50000;
    volatile UINT32 write_data;
	volatile GEMRegs* IPRegs;
	
	IPRegs    = (GEMRegs*)(GEMAC0_BASEADDR + 0xE000);

    IPRegs->NetCtrl |= GEM_NETCTRL_MDIO_ENB;	// Manager port enable
    IPRegs->NetCfg  &= ~(7 << 18);			// Remove clock divider
    IPRegs->NetCfg  |= GMAC_MDC_DIVIDER << 18;			// 4 = b100 = divider is 64 (sys clock up to 160 Mhz)

	write_data = 0x50020000;
	write_data |= ((nPhyID << 23) | (nRegID << 18) | nData);
	IPRegs->PHY  = write_data;

	while (!gem_phy_man_idle()) 
	{
		if (timeout-- <= 0) 
		{
			uart_printf("PHY MDIO read/write timeout\r\n");
			return GEM_RC_PHY_RW_TO;
		}
	}
	
    return GEM_RC_OK;
}


BOOL GemDrvIsLinkUp(GemCtx * pCtx)
{
	UINT16 nVal;
	UINT32 nPhy = pCtx->nDevID;

	nVal = GemDrvReadPhy (pCtx, nPhy, 1);

	if (nVal & (1 << 2))
			return TRUE;
	
	return FALSE;
}

MXRC GemDrvSelectPhy (GemCtx * pCtx, UINT32 nPhyID)
{
    UINT16 	nVal;
	UINT32 	nPhy = pCtx->nDevID;
	MXRC	rc;

    switch (nPhyID)
    {
        case RGMII:
			
	        REG32(GENERIC_PAD_CTRL_REG)     |= 1;
			            
	        if((rc = GemDrvWritePhy (pCtx, nPhy, 27, 0x848B)) != GEM_RC_OK)			
				return rc;
			
			nVal = GemDrvReadPhy (pCtx, nPhy, 20);
            nVal |= (1 << 7) | (1 << 1);
            if((rc = GemDrvWritePhy (pCtx, nPhy, 20, nVal)) != GEM_RC_OK)			
				return rc;

			nVal = GemDrvReadPhy (pCtx, nPhy, 0);
            nVal |= (1 << 15);
            if((rc = GemDrvWritePhy (pCtx, nPhy, 0, nVal)) != GEM_RC_OK)			
				return rc;
			
			break;

        case SGMII:

            // XP cluster control

            REG32(XP_SRDS_CNTRL) = 1;

            REG32(0xfe070064) = 0x11E;
            REG32(0xfe070068) = 0x11;
            REG32(0xfe0c0030) = 0xE2;

            REG32(XP_CLK_RSVD)          = 0;    // undocumented

            REG32(XP_AXI_RESET_0)       = 0;    // 7: NA 6: GEMAC1 5: GEMAC0 4: AMBA
            REG32(XP_AXI_RESET_1)       = 0;    // 1: SERDES Pclk 0: Expansion Bus

            REG32(XP_PHY_CLK_CNTRL)     = 0x33; // Changed to 0x33, the VLSI team recommendation
            REG32(XP_PHY_RESET)         = 0;
            REG32(XP_TDM_NTG_CNTRL)     |= 1;
            REG32(XP_SRDS_CNTRL)        = 0;

            // GPIO control RGMII PHY Refclk Output

            REG32(GENERIC_PAD_CTRL_REG)     |= 1;
			
            break;


        default:
            return GEM_RC_UNKNOWN_PHY;
    }

    return GEM_RC_OK;
}

/** @brief This function initializes GEM and should be called first before
           usage of drivers functions

    @param pDrvCtx [in] - the driver context
    @param pCfg    [in] - parameters

    @return [MXRC] an error code */

MXRC GemDrvCfg (LPVOID pDrvCtx, GemCfg * pCfg)
{
    //UINT32 nSpeed;
    GemCtx * pCtx = (GemCtx *)pDrvCtx;
    MXRC rc = GEM_RC_OK;

    GemDrvLock (pCtx);

    REG32(XP_PHY_RESET) &= ~1;
    REG32(GENERIC_PAD_CTRL_REG) |= 3 << 7;  // power up gemac1
    REG32(GENERIC_PAD_CTRL_REG) |= 1 << 0;

    // to select PHY interface bits
    switch (pCfg->Phy)
    {
        case RGMII:
        {
            // RGMII_TxClk_Sel
            // Selects the source of the RGMII Tx clock
            // 1 - Source Clock is external (input to device)
            // 0 - Source Clock is system PLL Output

            REG32(GP_CONFIG) |= (1 << 6);

			// GEM mode | SRGMII/RGMII | GEM Dup | FULL Dup | Speed Mux |


            pCtx->CoreRegs->Cfg =  GEM_CFG_MODE_SEL_GEM | GEM_CFG_MODE_PIN_SGMII | GEM_CFG_DUPLEX_SEL_GEM 
		   						| GEM_CFG_DUPLEX_GEM_FULL | GEM_CFG_DUPLEX_PHY_FULL | GEM_CFG_SPEED_SEL_GEM
                                | GEM_CFG_SPEED_GEM_1G | GEM_CFG_SPEED_PHY_1G | GEM_CFG_PHY_LINK_UP;
	        
            if((rc = GemDrvSelectPhy(pCtx, RGMII)) != GEM_RC_OK)
				return rc;

            break;
        }

        case SGMII:
        {
            // GEM mode | SGMII | GEM Dup | FULL Dup | Speed Mux |
            
            break;
        }
        default:
        {
            GemDrvUnlock (pCtx);

			return GEM_RC_UNKNOWN_PHY;
        }
    }

    // initialization of the GMAC
    pCtx->IPRegs->NetCtrl = GEM_NETCTRL_CLR_STAT;

    // NET configuration register
	pCtx->IPRegs->NetCfg  = GEM_NETCFG_PAUSE_ENB | GEM_NETCFG_DONOT_COPY_PAUSE | GEM_NETCFG_FULL_DUPLEX;
	pCtx->IPRegs->NetCfg  &= ~(7 << 18);			// Remove clock divider
    pCtx->IPRegs->NetCfg  |= GMAC_MDC_DIVIDER << 18;			// 4 = b100 = divider is 64 (sys clock up to 160 Mhz)

    // to select speed bits
    switch (pCfg->Speed)
    {
        case MAC_SPEED_100Mbps:
        {
            pCtx->IPRegs->NetCfg &= ~GEM_NETCFG_1GB_MODE;
            pCtx->IPRegs->NetCfg &= ~GEM_NETCFG_TBI_ENB;
            break;
        }
        case MAC_SPEED_1000Mbps:
        {
            pCtx->IPRegs->NetCfg |= GEM_NETCFG_1GB_MODE;
            pCtx->IPRegs->NetCfg &= ~GEM_NETCFG_TBI_ENB;

            break;
        }
        default:
        {
            GemDrvUnlock (pCtx);
            return GEM_RC_UNKNOWN_SPEED;
        }
    }



    pCtx->IPRegs->IntDisable= GEM_INT_RX_COMPLETE       |
                              GEM_INT_TX_UNDERRUN       |
                              GEM_INT_RETRY_EXCEEDED    |
                              GEM_INT_AHB_ERR           |
                              GEM_INT_TX_COMPLETE       |
                              GEM_INT_RX_OVERRUN        |
                              GEM_INT_HRESP_ERR         |
                              0;

    rc = MxSetIntHandler(pCtx->nDevID ? HAL_IRQ_GEM_1 : HAL_IRQ_GEM_0, (ITYPE)(INT_TYPE_IRQ | INT_TYPE_1N), (TPRIORITY)PRI_GEM, GemDrvIrqHandler, pCtx);

    if (FAILED(rc))
    {
        GemDrvUnlock (pCtx);

        return rc;
    }

    rc = MxEnableInt (pCtx->nDevID ? HAL_IRQ_GEM_1 : HAL_IRQ_GEM_0);

    if (FAILED(rc))
    {
        GemDrvUnlock (pCtx);

        return rc;
    }

    pCtx->Inited = TRUE;
    pCtx->MasterCpu = MxGetCpuID ();

    GemDrvUnlock (pCtx);

    return GEM_RC_OK;
}

/** @brief The function schedules and sends a frame.

    @NOTE: This function may be used conjointly with GemDrvSchTxFrm
           function for packets sending in a burst mode

    @param pDrvCtx  [in] - the driver context
    @param nTxFrmOpt[in] - see: GEM_DMA_TX_xxx macros
    @param pBuf     [in] - the pointer to the buffer (it can be NULL)
    @param nSize    [in] - the size of buffer (> 0 if pBuf != NULL )

    @return [MXRC] the error code of sending */

MXRC GemDrvTxFrm (LPVOID pDrvCtx, UINT32 nTxFrmOpt, LPVOID pHBuf, UINT32 nHSize, LPVOID pPBuf, UINT32 nPSize)
{
	UINT32 start_ticks;
	UINT32 cur_ticks;
	GemCtx * pCtx = (GemCtx *)pDrvCtx;
    MXRC rc = MX_OK;
    UINT32 TxResult;

    _ASSERT_PTR(pCtx);

	NET_PROFILE(10);

	nPSize = MAX(nPSize, 64);

    if (!(pCtx->IPRegs->NetCtrl & GEM_NETCTRL_TX_ENB))
        return GEM_RC_TX_DISABLED;

    if (pHBuf != NULL)
    {
        rc = GemDrvSchTxFrm (pDrvCtx, nTxFrmOpt, pHBuf, nHSize, pPBuf, nPSize);

        if (FAILED(rc))
            return rc;
    }
	
    GemDrvLock (pCtx);

    pCtx->IPRegs->TxStatus |= GEM_TX_ERROR_MASK;

    pCtx->IPRegs->NetCtrl |= GEM_NETCTRL_START_TX;

    // to wait of TX ending
    //uart_printf("GemDrvTxFrm: to wait\n");

	NET_PROFILE(11);

    //uart_printf("GemDrvTxFrm: ready\n");
   	NET_PROFILE(12);

    TxResult = pCtx->IPRegs->TxStatus;

    pCtx->IPRegs->TxStatus = 0xFFFFFFFF;

    //uart_printf("GemDrvTxFrm: whait while end transmit\n");

	cur_ticks = start_ticks = MxGetTicks();

    while ((gFrm->Descr & GEM_DMA_TX_USED_BIT) == 0  && cur_ticks - start_ticks < 150*1000*1000)
    {
        CPU_IDLE();
		cur_ticks = MxGetTicks();	
    }

    //uart_printf("GemDrvTxFrm: ok\n");

	if(gLastFrm)
	{
		GEM_DMA_TX_SET_USED(gLastFrm);
		gLastFrm = NULL;
	}
		
    GemDrvUnlock (pCtx);

	if (TxResult & GEM_TX_ERROR_MASK)
    {
        if (TxResult & GEM_TX_STAT_COLLISION)
            return GEM_RC_COLLISION;

        if (TxResult & GEM_TX_STAT_RETRY_EXCEEDED)
            return GEM_RC_RETRY_EXCEEDED;

        if (TxResult & GEM_TX_STAT_AHB_ERR)
            return GEM_RC_AHB_ERR;

        if (TxResult & GEM_TX_STAT_UNDERRUN)
            return GEM_RC_UNDERRUN;

        if (TxResult & GEM_TX_STAT_LATE_COLLISION)
            return GEM_RC_LATE_COLLISION;

        if (TxResult & GEM_TX_STAT_HRESP_ERR)
            return GEM_RC_HRESP_ERR;

        return GEM_RC_TX_ERROR;
    }
	
	if(cur_ticks - start_ticks > 150*1000*1000)
			return GEM_RC_TX_ERROR;

    return GEM_RC_OK;
}


/** @brief It returns a pointer to the current TX frame (to the first buffer)

    @Note: If TX is active, the function returns pointer to the first frame that
           was used for TX operation, in othe case it will return pointer to the
           first frame that will be used for TX

    @param pDrvCtx [in] - the driver context

    @return [HGTxFrm] */

HGTxFrm GemDrvGetFirstTxFrm (LPVOID pDrvCtx)
{
    GemCtx * pCtx = (GemCtx *)pDrvCtx;

	    _ASSERT_PTR(pCtx);

    if (pCtx->IPRegs->TxStatus & GEM_TX_STAT_GO)
        return (HGTxFrm)pCtx->CurTxDP;

    return (HGTxFrm)pCtx->IPRegs->TxQueue[0];
}

/** @brief This function removes a frame from the TX buffer descriptors list

    @NOTE: It can be useful to discard TX frame with error of sending

    @Warning: The TX operation will be halted!!!

    @param pDrvCtx [in] - the driver context
    @param pTxFrame[in] - the TX frame descriptor

    @return [MXRC] an error code */

MXRC GemDrvRemTxFrm (LPVOID pDrvCtx, HGTxFrm pTxFrame)
{
    GemCtx * pCtx = (GemCtx *)pDrvCtx;
    HGTxFrm pNextFrame = NULL, pBegin;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTxFrame);

    GemDrvLock (pCtx);

    pBegin = pTxFrame;

    pCtx->IPRegs->NetCtrl |= GEM_NETCTRL_HALT_TX;

    while (pCtx->IPRegs->TxStatus & GEM_TX_STAT_GO)
    {
        CPU_IDLE();
    }

    // to find out the next frame after current frame

    while (TRUE)
    {
        if (GEM_DMA_TX_IS_LAST(pTxFrame))
        {
            if (GEM_DMA_TX_IS_WRAP(pTxFrame))
                pNextFrame = (HGTxFrm)pCtx->TxD;
            else
                pNextFrame = (pTxFrame + 1);

            break;
        }

        if (GEM_DMA_TX_IS_WRAP(pTxFrame))
            pTxFrame = (HGTxFrm)pCtx->TxD;
        else
            pTxFrame ++;
    }

    if (pNextFrame != NULL)
    {
        while (GEM_DMA_TX_IS_SCHEDULED(pNextFrame))
        {
            pBegin->AddrOpt = pNextFrame->AddrOpt;
            pBegin->Descr   = pNextFrame->Descr & ~GEM_DMA_TX_WRAP;

            pNextFrame->Descr |= GEM_DMA_TX_USED_BIT;

            if (GEM_DMA_TX_IS_WRAP(pBegin))
                pBegin = (HGTxFrm)pCtx->TxD;
            else
                pBegin ++;

            if (GEM_DMA_TX_IS_WRAP(pNextFrame))
                pNextFrame = (HGTxFrm)pCtx->TxD;
            else
                pNextFrame ++;
        }
    }

    return GEM_RC_OK;
}


/** @breif This function schedules a packet for sending but does not send packet
           immediatelly, TX operation should be started by GemDrvTxFrm function.

    @param pDrvCtx [in] - the driver context
    @param nOpt    [in] - additional options
    @param pBuf    [in] - the pointer to buffer
    @param nSize   [in] - the size of buffer

    @return [MXRC] the error code of packet scheduling */

MXRC GemDrvSchTxFrm (LPVOID pDrvCtx, UINT32 nOpt, LPVOID pHBuf, UINT32 nHSize, LPVOID pPBuf, UINT32 nPSize)
{
    HGTxFrm hFrm;
	MXRC rc = GEM_RC_OK;

    _ASSERT_PTR (pHBuf);

    if (pHBuf != NULL)
        MxCacheClean (pHBuf, nPSize);

    hFrm = GemDrvAllocTxFrm(pDrvCtx, nOpt, pHBuf, nHSize);
	if(!hFrm)
	{
		uart_printf("some error while allocating frame\r\n");
		return GEM_RC_TX_ERROR;
	}	
	
	gFrm = hFrm;
	
    if (pPBuf != NULL)
    {
        MxCacheClean (pPBuf, nPSize);
		if ((rc = GemDrvAddTxBuf(pDrvCtx, hFrm, pPBuf, nPSize)) != GEM_RC_OK)
			return rc;
    }

    

    if (hFrm == NULL)
        return GEM_RC_TX_OVERLOAD;

    return rc;
}


/** @brief This function allocates a TX frame and sets frame parameters.
           The function can be used conjunctly with GemDrvAddTxBuf function
           to have a possibility to specify more than one buffer per frame.

    @param pDrvCtx [in] - the driver context
    @param opt     [in] - the additional TX options (see: GEM_DMA_TX_xxx bits)
    @param pBuf    [in] - the first buffer of frame
    @param nSize   [in] - the size of buffer

    @return [HGTxFrm] the allocated frame or NULL */

HGTxFrm GemDrvAllocTxFrm (LPVOID pDrvCtx, UINT32 opt, LPVOID pBuf, UINT32 nSize)
{
    GemCtx * pCtx = (GemCtx *)pDrvCtx;
    HGTxFrm pFrame, ret = NULL;

#ifdef _MX_DEBUG
    MXRC rc;
#endif

    _ASSERT_PTR(pCtx);

    GemDrvLock (pCtx);

    // to check a DMA initialization

    if (pCtx->FreeTxDP == NULL)
    {
#ifdef _MX_DEBUG
        rc = GEM_RC_DMA_TX_IS_NOT_INIT;
        _ASSERT_RC(rc);
#endif

        GemDrvUnlock (pCtx);
        return NULL;
    }

    pFrame = (HGTxFrm)pCtx->FreeTxDP;

    // if a frame is scheduled to transmit, this is
    // a situation of mistiming or TX list overloading
	
    //if (!GEM_DMA_TX_IS_USED(pFrame))
    if (GEM_DMA_TX_IS_SCHEDULED(pFrame))
    {
#ifdef _MX_DEBUG		
        rc = GEM_RC_TX_OVERLOAD;
#endif

        GemDrvUnlock (pCtx);

        return NULL;
    }

    ret = pFrame;

    if (GEM_DMA_TX_IS_WRAP(pFrame))
    {
        pFrame = (HGTxFrm)pCtx->TxD;
    }
    else
    {
        pFrame ++;
    }

    pCtx->FreeTxDP = (UINT32)pFrame;

    ret->AddrOpt = (UINT32)pBuf;

    // to set a status WORD
    // ~~~~~~~~~~~~~~~~~~~~~
    // RESTORE WRAP BIT
    // SET LAST BUFFER BIT
    // SET APP FRAME ADDITIONAL OPTIONS
    // SET BUFFER SIZE

    ret->Descr   = (ret->Descr & GEM_DMA_TX_WRAP) | GEM_DMA_TX_LAST_BUF |
                    (opt & (~GEM_DMA_TX_LEN_MASK)) | (nSize & GEM_DMA_TX_LEN_MASK);

     gFrm = ret;

    GemDrvUnlock (pCtx);

    return ret;
}



/** @brief This functon adds an additional buffer to the allocated frame,
           it should be used conjunctly with the GemDrvAllocTxFrm function

    @param pDrvCtx [in] - the driver context
    @param pTxFrame[in] - the allocated frame
    @param pBuf    [in] - the additional buffer for frame
    @param nSize   [in] - the size of buffer

    @return [MXRC] an error code */

MXRC GemDrvAddTxBuf (LPVOID pDrvCtx, HGTxFrm pTxFrame, LPVOID pBuf, UINT32 nSize)
{
    GemCtx * pCtx = (GemCtx *)pDrvCtx;
    HGTxFrm tmp = NULL;

    _ASSERT_PTR(pCtx);

    // to check a DMA initialization

    if (pCtx->FreeTxDP == NULL)
        return GEM_RC_DMA_TX_IS_NOT_INIT;

    GemDrvLock (pCtx);

    // to find out the end of TX frame

    while (TRUE)
    {
        if (GEM_DMA_TX_IS_LAST(pTxFrame))
            break;

        // to clear USED BIT (just to be sure)
        GEM_DMA_TX_CLR_USED(pTxFrame);

        // jump to the next buffer descriptor of this frame
        if (GEM_DMA_TX_IS_WRAP(pTxFrame))
        {
            pTxFrame = (HGTxFrm)pCtx->TxD;
        }
        else
        {
            pTxFrame ++;
        }
    }

    tmp = pTxFrame;

    // go to the next free space
    if (GEM_DMA_TX_IS_WRAP(pTxFrame))
    {
        pTxFrame = (HGTxFrm)pCtx->TxD;
    }
    else
    {
        pTxFrame ++;
    }

    // the first free block should be pointed by
    // pCtx->FreeTxDP pointer

    if ((UINT32)pTxFrame != pCtx->FreeTxDP)
    {
        GemDrvUnlock (pCtx);
        return GEM_RC_TX_MISTIMING;
    }

    // to check the possibility to add buffer
    if (GEM_DMA_TX_IS_SCHEDULED(pTxFrame))
    {
        GemDrvUnlock (pCtx);		
        return GEM_RC_TX_OVERLOAD;
    }

    // to clean the last bit in a prev. descr
    GEM_DMA_TX_CLR_LAST(tmp);

    // to set a status WORD
    // ~~~~~~~~~~~~~~~~~~~~~
    // RESTORE WRAP BIT
    // SET LAST BUFFER BIT
    // SET BUFFER SIZE

    pTxFrame->Descr = (pTxFrame->Descr & GEM_DMA_TX_WRAP) |
                        GEM_DMA_TX_LAST_BUF | (nSize & GEM_DMA_TX_LEN_MASK);

    pTxFrame->AddrOpt = (UINT32)pBuf;	

	gLastFrm = pTxFrame;

    // to change pCtx->FreeTxDP pointer

    if (GEM_DMA_TX_IS_WRAP(pTxFrame))
    {
        pCtx->FreeTxDP = pCtx->TxD;
    }
    else
    {
        pCtx->FreeTxDP = (UINT32)(pTxFrame + 1);
    }

    GemDrvUnlock (pCtx);

	

    return GEM_RC_OK;
}

/** @brief The function returns a descriptor of the received frame, this
           descriptor can be used for getting frame buffer(s),
           various status and for free frame operation

    @param pDrvCtx  [in] - the driver context
    @param pRxFrame [out]- the frame descriptor or NULL
    @param pnBufNum [out]- the number of buffers per received frame (the size of one buffer is DMA configured)
    @param pnFrmSize[out]- the total size of received frame in bytes

    @return [MXRC] an error code */

MXRC GemDrvRxFrm (LPVOID pDrvCtx, HGRxFrm * pRxFrame, UINT32 * pnBufNum, UINT32 * pnFrmSize)
{
    GemCtx * pCtx = (GemCtx *)pDrvCtx;
    HGRxFrm pFrame, begin;
    UINT32 nBufNum = 0;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pRxFrame);

    // to start scanning from the beginning of the RX list

    begin = (pFrame = (HGRxFrm)pCtx->CurRxDP);

    if (pFrame == NULL)
        return GEM_RC_DMA_IS_NOT_CONFIG;

    GemDrvLock (pCtx);

    do
    {
        // to load the frame parameters

        if (GEM_DMA_RX_IS_SOF(pFrame) && GEM_DMA_RX_IS_READY(pFrame))
        {
            // to find the end of frame

            nBufNum = 1;

            while (TRUE)
            {
                if (GEM_DMA_RX_IS_EOF(pFrame))
                {
                    if (pnBufNum != NULL)
                        *pnBufNum = nBufNum;

                    if (pnFrmSize != NULL)
                        *pnFrmSize = GEM_DMA_RX_GET_LEN(pFrame);

                    *pRxFrame = pFrame;

                    GemDrvUnlock (pCtx);

                    return GEM_RC_OK;
                }

                if (GEM_DMA_RX_IS_WRAP(pFrame))
                {
                    // jump to the beginning of the RX list
                    pFrame = (HGRxFrm)pCtx->RxD;
                }
                else
                {
                    // go to the next frame
                    pFrame++;
                }

                nBufNum++;
            }

        }

        if (GEM_DMA_RX_IS_WRAP(pFrame))
        {
            pFrame = (HGRxFrm)pCtx->RxD;
        }
        else
        {
            pFrame++;
        }

    }while (pFrame != begin);

    GemDrvUnlock (pCtx);

    return GEM_RC_RX_EMPTY;
}

/** @brief This function frees a received frame (it marks buffer descriptors like are not used)

    @param pDrvCtx [in] - the driver context
    @param pFrame  [in] - the frame descriptor (the first buffer descriptor of the frame)

    @return [MXRC] the error code of operation */

MXRC GemDrvFreeRxFrm (LPVOID pDrvCtx, HGRxFrm pFrame)
{
    GemCtx * pCtx = (GemCtx *)pDrvCtx;

#ifdef _MX_DEBUG
    HGRxFrm begin = NULL;

    // to start scanning from the beginning of the RX list

    begin = (HGRxFrm)pCtx->CurRxDP;

    // we need to free frames in series
    _ASSERT(begin == pFrame);

#endif

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pFrame);

    if (!GEM_DMA_RX_IS_READY(pFrame))
        return GEM_RC_FRM_IS_NOT_USED;

    if (!GEM_DMA_RX_IS_SOF(pFrame))
        return GEM_RC_FRM_IS_NOT_SOF;

    GemDrvLock (pCtx);

    // to process all buffers of the received frame

    while (TRUE)
    {
        GEM_DMA_RX_SET_READY(pFrame);

        if (GEM_DMA_RX_IS_EOF(pFrame))
        {
            // to clean end of frame and start of frame bits
            GEM_DMA_RX_CLR_STATUS(pFrame);

            break;
        }

        // to clean the end of frame and the start of frame bits
        GEM_DMA_RX_CLR_STATUS(pFrame);
    }

    // jump to the next buffer descriptor
    if (GEM_DMA_RX_IS_WRAP(pFrame))
    {
        pFrame = (HGRxFrm)pCtx->RxD;
    }
    else
    {
        pFrame ++;
    }

    pCtx->CurRxDP = (UINT32)pFrame;

    GemDrvUnlock (pCtx);

    return GEM_RC_OK;
}

/** @brief The function returns a pointer to the RX frame's buffer (one of)

    @param pDrvCtx [in] - the driver context
    @param pFrame  [in] - the frame descriptor
    @param nIdx    [in] - the index of buffer (see: GemDrvRxFrm function)

    @return [LPVOID] a pointer to the frame buffer */

LPVOID GemDrvGetRxFrmBuf (LPVOID pDrvCtx, HGRxFrm pFrame, UINT32 nIdx)
{
    GemCtx * pCtx = (GemCtx *)pDrvCtx;
    UINT32 i = 0;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pFrame);

    while (i < nIdx)
    {
        if (GEM_DMA_RX_IS_EOF(pFrame))
            return NULL;

        if (GEM_DMA_RX_IS_WRAP(pFrame))
        {
            pFrame = (HGRxFrm)pCtx->RxD;
        }
        else
        {
            pFrame++;
        }

        if (!GEM_DMA_RX_IS_READY(pFrame))
            return NULL;

        i++;
    }

    MxCacheInvalidate ((LPVOID)(pFrame->AddrOpt), GEM_DMA_RX_GET_LEN(pFrame));

    return (LPVOID)(pFrame->AddrOpt);
}

/** @brief This function returns a pointer to the driver context and provides
           a possibility to use extended driver interface

    @param nDevID [in] - the device ID [0, 1]

    @return [LPVOID] a pointer to the driver interface */

LPVOID GemDrvGetCtx (UINT32 nDevID)
{
    return GemDrvCtx [nDevID ? 1 : 0];
}

/** @brief This function is called to stop GEM driver and
           disable GEM HW block to send and receive packets

    @param nDevID [in] - the device ID

    @return [MXRC] an error code */

MXRC GemDrvShutdown (UINT32 nDevID)
{
    GemCtx * pCtx;
    IRQTYPE irq;

    if (nDevID >= 2)
        return GEM_RC_MAC_ID_ERROR;

    if ((pCtx = GemDrvGetCtx(nDevID)) == NULL)
        return GEM_RC_OK;

    if (pCtx->Inited == 0)
        return GEM_RC_OK;

    GemDrvDisableRX (pCtx);

    irq = ARM_INT_disable ();

    GemDrvLock (pCtx);

    GemDrvDisableTX (pCtx);
    MxDisableInt (nDevID ? HAL_IRQ_GEM_1 : HAL_IRQ_GEM_0);

    pCtx->Inited = FALSE;

    GemDrvUnlock (pCtx);
    ARM_INT_restore (irq);

    return GEM_RC_OK;
}

void GemDrvAckIrq (UINT32 irq)
{
    
}

/** @brief This function initializes GEM driver and should be called first
           before usage of any driver API, the next functions are:

             * GemDrvCfg        - to configure GEM driver
             * GemDrvConfigDMA  - to configure GEM HW DMA block

    @param nBaseAddr[in] - the GEM HW base address (see: GEMAC0_BASEADDR and GEMAC1_BASEADDR)

    @return [MXRC] an error code */

MXRC GemDrvInit (UINT32 nBaseAddr)
{
    GemCtx * pCtx;
    VUINT32 dummy;
    UINT32 nDevID;

    // To enable the GEMAC devices
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // GEMAC-1 - bit#6
    // GEMAC-0 - bit#5
    // AMBA    - bit#4

    if (nBaseAddr == GEMAC0_BASEADDR)
        nDevID = 0;
    else if (nBaseAddr == GEMAC1_BASEADDR)
        nDevID = 1;
    else
        return GEM_RC_MAC_ID_ERROR;

    dummy = 1000;
    while (dummy--);

    REG32(XP_AXI_RESET_0) |= (1 << (nDevID + 5));

    dummy = 1000;
    while (dummy--);

    REG32(XP_AXI_RESET_0) &= ~((1 << ((nDevID + 5)))| (1 << 4));

    pCtx = &GlobalGemCtx [nDevID];

    memset (pCtx, 0, sizeof (GemCtx));

    pCtx->nDevID    = nDevID;
    pCtx->IPRegs    = (GEMRegs*)(nBaseAddr + 0xE000);
    pCtx->CoreRegs  = (GEMCoreRegs*)(nBaseAddr + 0xF000);

    GemDrvCtx [nDevID] = pCtx;

    dummy = 1000;
    while (dummy--);

    REG32 (XP_PHY_RESET) |= (1 << (nDevID + 4));

    dummy = 1000;
    while (dummy--);

    REG32 (XP_PHY_RESET) &= ~(1 << (nDevID + 4));

    return GEM_RC_OK;
}

MXRC GemDrv_init(UINT32 nInit, LPVOID * pCtxStorage)
{
#if 1
    MXRC rc = GemDrvInit (nInit);

    if (SUCCESSFUL(rc) && (pCtxStorage != NULL))
        *pCtxStorage = GemDrvGetCtx ((nInit == GEMAC0_BASEADDR) ? 0 : 1);

    return rc;
#else
    GemCtx * pCtx;

    // To enable the GEMAC devices
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // GEMAC-1 - bit#6
    // GEMAC-0 - bit#5
    // AMBA    - bit#4

    REG32(XP_AXI_RESET_0) &= ~((1 << 6) | (1 << 5) | (1 << 4));

    pCtx = &GlobalGemCtx [(nInit == GEMAC0_BASEADDR) ? 0 : 1];

    if (pCtx == NULL)
        return GEM_RC_NOT_MEMORY;

    memset (pCtx, 0, sizeof (GemCtx));

    pCtx->nDevID    = (nInit == GEMAC0_BASEADDR) ? 0 : 1;
    pCtx->IPRegs    = (GEMRegs*)(nInit + 0xE000);
    pCtx->CoreRegs  = (GEMCoreRegs*)(nInit + 0xF000);

    GemDrvCtx [(nInit == GEMAC0_BASEADDR) ? 0 : 1] = pCtx;

    *pCtxStorage = pCtx;

    return MX_OK;
#endif
}

MXRC GemDrv_config(LPVOID pDrvCtx, UINT32 nCfg, LPVOID pCfgData, UINT32 nCfgDataSize)
{
    return MX_OK;
}

MXRC GemDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData, UINT32 nCmdDataSize)
{
    return MX_OK;
}

MXRC GemDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData, UINT32 * pnCmdDataSize)
{
    return MX_OK;
}

void GemDrvMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    // GEM-0 is needed to control PHY
    //-------------------------------
    MmuMgrCfgMemAP(GEMAC0_BASEADDR+0xE000, 10*1024, AP_MGR_READ_WRITE, AP_MGR_READ_ONLY);
    //MmuMgrCfgMemAP(GEMAC0_BASEADDR+0xF000, 10*1024, AP_MGR_READ_WRITE, AP_MGR_READ_ONLY);

    MmuMgrCfgMemAP(GEMAC1_BASEADDR+0xE000, 10*1024, AP_MGR_READ_WRITE, AP_MGR_READ_ONLY);
    MmuMgrCfgMemAP(GEMAC1_BASEADDR+0xF000, 10*1024, AP_MGR_READ_WRITE, AP_MGR_READ_ONLY);
    
    MmuMgrCfgMemAP(XPCLKRST_BASEADDR,      4*1024,  AP_MGR_READ_WRITE, AP_MGR_READ_ONLY);
}

