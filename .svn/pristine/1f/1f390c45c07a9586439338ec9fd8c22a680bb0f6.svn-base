//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL sriodrv.c
 *
 * @brief SRIO driver (API definition)
 * @author Mindspeed Technologies
 * @version $Revision: 1.5 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef SRIO_DRV_ENABLED

#include "sriodrv.h"
#include "hal_srio.h"
#include "appprintf.h"

//#define SRIO_DEBUG

SrioDrvCtx  SrioCtx [2] = {0,0};

/** @brief  This function returns a pointer to the SRIO device driver context
    @param nDevID [in] - the driver ID : 0 or 1
    @return [SrioDrvCtx *] the pointer to the context driver */

SrioDrvCtx * SrioDrvGetCtx (UINT32 nDevID)
{
    return &SrioCtx [(!nDevID) ? 0 : 1];
}

/** @brief The SRIO IRQ handler

    @param pAckReg [in] - interrupt ACK register
    @param nIrqID  [in] - the IRQ ID
    @param pParam  [in] - parameter (SRIO driver context) */

void SrioIrqHandler (volatile UINT32 * pAckReg, UINT32 nIrqID, LPVOID pParam)
{
    SrioDrvCtx * pCtx = (SrioDrvCtx *)pParam;
    UINT32 nIntStat, status, status2;
    UINT32 nID;

    pCtx->SrioTotalIrqNum ++;

    nID = pCtx->nID;

    nIntStat = REG32(RAD_SRIO_RAB_INTR_STAT_GNRL(nID));

#ifdef SRIO_DEBUG
    uart_printf ("SRIO IRQ: %x\r\n", nIntStat);
#endif

    if (nIntStat & SRIO_INT_STAT_AXI_PIO)
    {
        status = REG32(RAD_SRIO_RAB_INTR_ENAB_APIO(nID));

        if ((status >> 8) & 0xFF)
        {
            pCtx->Srio_err_axi ++;
        }

        REG32(RAD_SRIO_RAB_INTR_ENAB_APIO(nID)) = status;
    }

    if (nIntStat & SRIO_INT_STAT_RIO_PIO)
    {
        status = REG32(RAD_SRIO_RAB_INTR_STAT_RPIO(nID));

        if ((status >> 8) & 0xFF)
        {
            pCtx->Srio_err_rio ++;
        }
#ifdef SRIO_DEBUG
        uart_printf (" RPIO: 0x%x\r\n", status);
#endif

        REG32(RAD_SRIO_RAB_INTR_STAT_RPIO(nID)) = status;
    }

    if (nIntStat & SRIO_INT_STAT_WDMA)
    {
        status = REG32(RAD_SRIO_RAB_INTR_STAT_WDMA(nID));

        if ((status >> 16) & 0xFF)
        {
            pCtx->Srio_err_wdma ++;
        }

        if ((status >> 24) & 0xFF)
        {
            pCtx->Srio_err_wdma ++;
        }

        if (status & 0xFF)
        {
            pCtx->Srio_wdma_irq ++;
        }
#ifdef SRIO_DEBUG
        uart_printf (" WDMA: 0x%x\r\n", status);
#endif

        REG32(RAD_SRIO_RAB_INTR_STAT_WDMA(nID)) = status;
    }

    if (nIntStat & SRIO_INT_STAT_RDMA)
    {
        status = REG32(RAD_SRIO_RAB_INTR_STAT_RDMA(nID));

        if ((status >> 16) & 0xFF)
        {
            pCtx->Srio_err_rdma ++;
        }

        if (status & 0xFF)
        {
            pCtx->Srio_rdma_irq ++;
        }
#ifdef SRIO_DEBUG
        uart_printf (" RDMA: 0x%x\r\n", status);
#endif

        REG32(RAD_SRIO_RAB_INTR_STAT_RDMA(nID)) = status;
    }

    if (nIntStat & SRIO_INT_IDME)
    {
        status = REG32(RAD_SRIO_RAB_INTR_STAT_IDME(nID));
        status2 = REG32(RAD_SRIO_RAB_IB_DME0_STAT(nID));
        REG32(RAD_SRIO_RAB_IB_DME0_STAT(nID)) = status2 & (~3);
        REG32(RAD_SRIO_RAB_INTR_STAT_IDME(nID)) = status;
    }
    if (nIntStat & SRIO_INT_ODME)
    {
        status = REG32(RAD_SRIO_RAB_INTR_STAT_ODME(nID));
        status2 = REG32(RAD_SRIO_RAB_OB_DME0_STAT(nID));
        REG32(RAD_SRIO_RAB_OB_DME0_STAT(nID)) = status2 & (~3);
        REG32(RAD_SRIO_RAB_INTR_STAT_ODME(nID)) = status;
    }

    // this interrupt is generated like RX event (DMA has complited RX operation)
    // the data was written to system memory from remote SRIO client
    if (pCtx->RxCb != NULL && nIntStat & SRIO_INT_STAT_RDMA)
    {
        pCtx->RxCb(pCtx->RxCbPtr, pCtx->RxBufPtr,pCtx->RxBufSize);
    }

    // this interrupt is generated like TX event (DMA has complited TX operation)
    // the data was written from system memory to remote SRIO client
    if (pCtx->TxCb != NULL && nIntStat & SRIO_INT_STAT_WDMA)
    {
        pCtx->TxCb(pCtx->TxCbPtr, pCtx->TxBufPtr,pCtx->TxBufSize);
    }

    *pAckReg = nIrqID;
}

/** @brief This function configures serdes-2 rate and serdes loopback mode

    @param SrdsRate     [in] - SerDes rate
    @param SrdsMode     [in] - SerDes mode, SRIO, PCIE ...
    @param nInvPcsRxClk [in] - 1 : PCS RX clock will be inverted (used for SRIO)

    @return [MXRC] an error code */

extern MXRC Serdes0Init(UINT32 SrdsRate, UINT32 SrdsMode, UINT32 nInvPcsRxClk);

/** @brief This function configures serdes-2 rate and serdes loopback mode

    @param SrdsRate     [in] - SerDes rate
    @param SrdsMode     [in] - SerDes mode, SRIO, PCIE ...
    @param nInvPcsRxClk [in] - 1 : PCS RX clock will be inverted (used for SRIO)

    @return [MXRC] an error code */

MXRC Serdes2Init(UINT32 SrdsRate, UINT32 SrdsMode, UINT32 nInvPcsRxClk)
{
    // REG32(GP_CONFIG) |= 1<<4; // set SRDS2 reference clock to system PLL

    // reset Serdes 2
    REG32(RAD_SRDS2_CNTRL) = 0xF1;

    // quarter rate
    REG32(RAD_SRDS2_INIT_CFG_R0) = 0x000204DF;
    REG32(RAD_SRDS2_INIT_CFG_R1) = 0xAA800000;
    REG32(RAD_SRDS2_INIT_CFG_R2) = (SrdsMode << 18) + 0x2a;  // old settings for PCIe - 0x0010002A;

//    SysDelay(3);

    // Program SRDS2 clocks
    REG32(RAD_SRDS2_CONT_CLK_DIV_CNTRL) = 0x82;

    //!!! TxIf Clock derived from SERDES
    REG32(RAD_SRDS2_TXIF_CLK_DIV_CNTRL) = 0x82;

    MxDelayTicks(TICKS_TO_MS(10));

    REG32(RAD_SRDS2_CNTRL) = 0; // Release serdes2 reset

    MxDelayTicks(TICKS_TO_MS(100));

// Yogish PLL clock ready assertion - section 3.2 of bringup doc

    REG32(RAD_CFG_SRDS_MMD) = 0x1;
    REG32(RAD_CFG_SRDS_LANE) = 0xF;

    // PLL clock ready assertion - section 3.2 of bringup doc
    REG32(RAD_APB_SERDES2_4CH_BASEADDR+0xC000) = 0x70DF;
    MxDelayTicks(TICKS_TO_MS(3));
    REG32(RAD_APB_SERDES2_4CH_BASEADDR+0xC004) = 0xC440;
    MxDelayTicks(TICKS_TO_MS(3));
    REG32(RAD_APB_SERDES2_4CH_BASEADDR+0xC008) = 0x0362;
    MxDelayTicks(TICKS_TO_MS(3));
    REG32(RAD_APB_SERDES2_4CH_BASEADDR+0xC004) = 0xC640;
    MxDelayTicks(TICKS_TO_MS(3));
    REG32(RAD_APB_SERDES2_4CH_BASEADDR+0x8034) = 0x0066;//0x007E; // RX CTLE Control
    MxDelayTicks(TICKS_TO_MS(3));
    REG32(RAD_APB_SERDES2_4CH_BASEADDR+0x8038) = 0x0C83; // TX Config 1
    MxDelayTicks(TICKS_TO_MS(3));
//    REG32(RAD_APB_SERDES2_4CH_BASEADDR+0x803C) = 0x0021; // TX Config 2
//    SysDelay(3);

    // Preemp 0xc1 default, 0x9b works better, also 0xac
    REG32(RAD_APB_SERDES2_4CH_BASEADDR+0x8040) = 0x00ac; // TX Pre-emphasis
    MxDelayTicks(TICKS_TO_MS(3));

    // CTLE peak-pole
    REG32(RAD_APB_SERDES2_4CH_BASEADDR+0x8030) = 0x00ff;
    MxDelayTicks(TICKS_TO_MS(3));

    // Serdes DLL settings - Need this for 3.125GHz
    REG32(RAD_APB_SERDES2_4CH_BASEADDR+0xC008) = 0x262;
    MxDelayTicks(TICKS_TO_MS(3));

    // Phase Interpolator
    REG32(RAD_APB_SERDES2_4CH_BASEADDR+0xC078) = 0xC2F0;
    MxDelayTicks(TICKS_TO_MS(3));

    REG32(RAD_APB_SERDES2_4CH_BASEADDR+0x8048) = 0x0000;

    REG32(RAD_CFG_SRDS_MMD) = 0x3;
    REG32(RAD_CFG_SRDS_LANE) = 0xF;

    if (nInvPcsRxClk)
    {
        REG32(RAD_SRDS2_PCS_MISC_CFG) = 0xB117; // Invert PCS RX CLK
    }

    MxDelayTicks(TICKS_TO_MS(3));

    return MX_OK;
}

MXRC SrioDrvSerdesCfg(UINT32 nID, UINT32 LBMode, UINT32 rate)
{
    MXRC rc;

    if (nID == 0)
    {
        // Serdes initialization
    	rc = Serdes0Init(rate, SRDSMODE_SRIO, 1);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }
    }

    if (nID == 1)
    {
    	rc = Serdes2Init(rate, SRDSMODE_SRIO, 1);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }
    }

    switch (LBMode)
    {
        case SRIO_LB_INTERNAL:
	    {
            if (nID == 0)
            {
                REG32(RAD_CFG_SRIO0_MODE) = 0;
                REG32(RAD_CFG_SRIO_LOOPBACK) &= ~(1 << 0);
                REG32(RAD_CFG_SRIO_LOOPBACK) |=  (1 << 0);
            }
            else
            {
                REG32(RAD_CFG_SRIO1_MODE) = 0;
                REG32(RAD_CFG_SRIO_LOOPBACK) &= ~(1 << 1);
                REG32(RAD_CFG_SRIO_LOOPBACK) |=  (1 << 1);
            }

            break;
	    }
        case SRIO_LB_PCS:
        {
            REG32(RAD_CFG_SRDS_MMD) = 0x3;  //mmd
		    REG32(RAD_CFG_SRDS_LANE) = 0xF; //lanes

		    // set serdes internal loopback
		    if (nID == 0)
		        REG32(RAD_SRDS0_PCS_MODE) |= (1 << 14);
            else
		        REG32(RAD_SRDS2_PCS_MODE) |= (1 << 14);

            break;
        }
        case SRIO_LB_PREDRIVER:
	    {
	        REG32(RAD_CFG_SRDS_MMD) = 0x1;  //mmd
		    REG32(RAD_CFG_SRDS_LANE) = 0xF; //lanes

	        // Pre-driver loopback
	        if (nID == 0)
            {
    	        REG32(RAD_APB_SERDES0_4CH_BASEADDR+0x8050) = 0x2;
    	        REG32(RAD_APB_SERDES0_4CH_BASEADDR+0x8000) = 0x0B01;
            }
            else
            {
    	        REG32(RAD_APB_SERDES2_4CH_BASEADDR+0x8050) = 0x2;
    	        REG32(RAD_APB_SERDES2_4CH_BASEADDR+0x8000) = 0x0B01;
            }

	        MxDelayTicks(TICKS_TO_MS(10));
            break;
	    }
        default:
            break;
    }

    return MX_OK;
}

/** @brief This function initializes SERDES HW component used
           for SRIO communication

    @param pCfg [in] - the pointer to the driver configuration

    @return [MXRC] an error code */

MXRC SrioDrvInitSerdes (SRIODRVCFG * pCfg)
{
    MXRC rc;
    UINT32 id, PWO = 0;
    UINT32 Force1Lane = pCfg->Lanes;
    UINT32 ts;

    if (pCfg->nID == 0)
        REG32(RAD_AXI_RESET_1) &= ~((1 << 1) | (1 << 2) | (1 << 3) | (1 << 4)); // release SRIOs
    else
        REG32(RAD_AXI_RESET_1) &= ~((1 << 1) | (1 << 5) | (1 << 6) | (1 << 7)); // release SRIOs

    // set clock for RAB (SRIO AXI) - it is not initialized by Linux
	REG32(SRIO_AXI_CLK_CNTRL) = 0x10 | (2 <<1 ) | 1; // SRIO AXI clock
	REG32(AXI_CLK_DIV_CNTRL_2)|= (1 << 4);  // bypass SRIO AXI clock divider

    // Program Clock reset block and SRDS config register
    REG32(RAD_CLUSTER_CNTRL) = 0x1;
    MxDelayTicks(TICKS_TO_MS(10));

    // Configure SERDES MUX to 0x0 to enable both SRIOs

    // SRIO-0 is mapped to SERDES-0 (x4)
    // SRIO-1 is mapped to SERDES-2 (x4)

    REG32(RAD_CFG_SRDS_MUX) =  0x0;

    // init serdes
    // SRDSRATE_2500
    if (FAILED(rc = SrioDrvSerdesCfg(pCfg->nID, pCfg->LBMode, pCfg->Speed)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    //  Force SRIO to Host mode with ID 2'b00 (0 port, small 8bit ID)
    if(Force1Lane)
    {
        //  force  1 lane (lane 0 by default)
        if (pCfg->nID == 0)
            REG32(RAD_CFG_SRIO0_MODE) = 0x80;
        else
            REG32(RAD_CFG_SRIO1_MODE) = 0x80;

        if(Force1Lane == 2)
            PWO = 0x3;  //single-lane port, lane 2
        else
            PWO = 0x2;  //single-lane port, lane 0
    }
    else
    {
        //  use 4 lanes
        if (pCfg->nID == 0)
            REG32(RAD_CFG_SRIO0_MODE) = 0x00;
        else
            REG32(RAD_CFG_SRIO1_MODE) = 0x00;

        PWO = 0x0; //not override, 4lane
    }

    // Reset all SRIO engines
    // 7: SRIO RX 3
    // 6: SRIO RX 2
    // 5: SRIO RX 1
    // 4: SRIO RX 0
    // 3: SRIO Controller
    // 0: Global Reset

    if (pCfg->nID == 0)
        REG32(RAD_SRIO0_RESET) = 0xFF;
    else
        REG32(RAD_SRIO1_RESET) = 0xFF;
    MxDelayTicks(TICKS_TO_MS(2));

    if (pCfg->nID == 0)
        REG32(RAD_SRIO0_RESET) = 0x00;
    else
        REG32(RAD_SRIO1_RESET) = 0x00;

    MxDelayTicks(TICKS_TO_MS(2));

    // Do SRIO 0&1 reset (exept RAD)
    if (pCfg->nID == 0)
        REG32(RAD_AXI_RESET_1) |= ((1 << 1) | (1 << 2) | (1 << 3) | (1 << 4));
    else
        REG32(RAD_AXI_RESET_1) |= ((1 << 1) | (1 << 5) | (1 << 6) | (1 << 7));

    MxDelayTicks(TICKS_TO_MS(5));

    if (pCfg->nID == 0)
        REG32(RAD_AXI_RESET_1) &= ~((1 << 1) | (1 << 2) | (1 << 3) | (1 << 4));
    else
        REG32(RAD_AXI_RESET_1) &= ~((1 << 1) | (1 << 5) | (1 << 6) | (1 << 7));

    MxDelayTicks(TICKS_TO_MS(5));

 /*
    //reseting all RAD engines
    REG32(RAD_SRIO_RAB_RST_CTRL(0)) = 0xffffffff;
    REG32(RAD_SRIO_RAB_RST_CTRL(1)) = 0xffffffff;
    MxDelayTicks(TICKS_TO_MS(2));
    REG32(RAD_SRIO_RAB_RST_CTRL(0)) = 0;
    REG32(RAD_SRIO_RAB_RST_CTRL(1)) = 0;
    MxDelayTicks(TICKS_TO_MS(2));
 */

    // initialize SRIOs
    id = pCfg->nID;
    {
        REG32(RAD_SRIO_RAB_APB_CSR(id)) = 0x0 << 16;  //page #0

        // Programm GRIO BDIDCSR register to set the Device ID to 0x00/0x01
        REG32(RAD_SRIO_GRIO_BDIDCSR(id)) = (id << 16) | (id << 8);

        // Set GRIO port configuration - lanes x1 (lane 0 or lane 2) or x4
        REG32(RAD_SRIO_RAB_APB_CSR(id)) = 0x0;  //page 0
        REG32(RAD_SRIO_GRIO_P0CCSR(id)) |= 0x8000; //port disable
        MxDelayTicks(TICKS_TO_MS(1));
        REG32(RAD_SRIO_GRIO_P0CCSR(id)) = (REG32(RAD_SRIO_GRIO_P0CCSR(id)) & 0xfffffff8) | PWO; //set PWO
        REG32(RAD_SRIO_GRIO_P0CCSR(id)) &= (~0x8000); //port enable
        MxDelayTicks(TICKS_TO_MS(10));

        //set RIO-PIO LUT offset to use bits[3-0] of  DeviceID
        REG32(RAD_SRIO_RAB_RIO_AMAP_IDSL(id)) = 1;
        //disable bypass
        REG32(RAD_SRIO_RAB_RIO_AMAP_BYPS(id)) = 0;

        // error capture configuration - logical errors
        REG32(RAD_SRIO_GRIO_LTLEECSR(id)) = 0xffffffff;     //enable all
        REG32(RAD_SRIO_GRIO_P0ERECSR(id)) = 0xffffffff;     //enable all

        // clear lane sync. state - just to get actual sync. state later in the code
        //REG32(RAD_SRIO_RAB_APB_CSR(id)) = 0x20 << 16;  //page #32
        //REG32(RAD_SRIO_GRIO_P0SLCSR(id)) = 0xf0;       //clear lane0 - lane3 sync. state
    }

    if (0)
    {
	    // Reinitialize serdes - we need to detect active lanes
	    // If no reinitialization done - first time lanes can be detected in wrong way (depends on chip)
	    rc = SrioDrvSerdesCfg(pCfg->nID, pCfg->LBMode, pCfg->Speed);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        if (pCfg->nID == 0)
	        REG32(RAD_SRIO0_RESET) = 0xFF;
        else
	        REG32(RAD_SRIO1_RESET) = 0xFF;

	    MxDelayTicks(TICKS_TO_MS(2));

        if (pCfg->nID == 0)
	        REG32(RAD_SRIO0_RESET) = 0x00;
        else
	        REG32(RAD_SRIO1_RESET) = 0x00;

        MxDelayTicks(TICKS_TO_MS(20));
    }

    // clear lane sync. state - just to get actual sync. state later in the code
    if (pCfg->nID == 0)
    {
        REG32(RAD_SRIO_RAB_APB_CSR(0)) = 0x20 << 16;  //page #32
        REG32(RAD_SRIO_GRIO_P0SLCSR(0)) = 0xf0;       //clear lane0 - lane3 sync. state
    }
    else
    {
        REG32(RAD_SRIO_RAB_APB_CSR(1)) = 0x20 << 16;  //page #32
        REG32(RAD_SRIO_GRIO_P0SLCSR(1)) = 0xf0;       //clear lane0 - lane3 sync. state
    }

    //wait for GRIO to be initialized
    ts = MxGetTicks();

    REG32(RAD_SRIO_RAB_APB_CSR(pCfg->nID)) = 0;

    rc = RC_SRIODRV_SERDES_TIMEOUT;

    if (pCfg->LBMode != SRIO_LB_INTERNAL)
    {
        while((MxGetTicks() - ts) < TICKS_TO_MS(200))
        {
            //if((REG32(RAD_SRIO_GRIO_P0ESCSR(0)) & 0x02000000) && (REG32(RAD_SRIO_GRIO_P0ESCSR(1)) & 0x02000000) )
            if((REG32(RAD_SRIO_GRIO_P0ESCSR(pCfg->nID)) & 0x02000000))
            {
                rc = MX_OK;
                break;
            }
        }
#ifdef SRIO_DEBUG
        uart_printf ("\r\nP0ESCSR=0x%08x\r\n", REG32(RAD_SRIO_GRIO_P0ESCSR(pCfg->nID)));
#endif
    }
    else
    {
        rc = MX_OK;
    }

    _ASSERT_RC(rc);

    if (SUCCESSFUL(rc))
    {
        rc = RC_SRIODRV_NO_LINK;

        // Port link maintanance request
        REG32(RAD_SRIO_RAB_APB_CSR(pCfg->nID)) = 0x0;
        REG32(RAD_SRIO_GRIO_LMREQCSR(pCfg->nID)) = 0x04000000;     //LINK_REQUEST

        ts = MxGetTicks();

        while(MxGetTicks() - ts < TICKS_TO_MS(1000))
        {
            UINT32 r = REG32(RAD_SRIO_GRIO_LMRESPCSR(pCfg->nID));

            if(r & 0x00000080)
            {
                rc = MX_OK;
                break;
            }
        }
    }

    _ASSERT_RC (rc);

    return rc;
}

/** @brief This function allocated and sets pointer to the driver buffers

    @param pCtx [in] - the driver context
    @return [MXRC] an error code */

MXRC SrioDrvAllocData(SrioDrvCtx * pCtx)
{
    PUINT8 pData;
    UINT32 nSize = 0;

    pCtx->TxDmaNum = pCtx->TxBufNum;
    pCtx->RxDmaNum = pCtx->RxBufNum;

    nSize += pCtx->RxBufNum * ROUND(pCtx->RxBufSize, 32);
    nSize += pCtx->TxBufNum * ROUND(pCtx->TxBufSize, 32);
    nSize += ROUND(sizeof (PSDMAD), 32) * pCtx->TxDmaNum;
    nSize += ROUND(sizeof (PSDMAD), 32) * pCtx->RxDmaNum;

    pData = (PUINT8)MxHeapAlloc (DspHeap, nSize);

    if (pData == NULL)
        return RC_SRIODRV_ALLOC_ERROR;

    memset (pData, 0, nSize);

    pCtx->DriverBuf = pData;

    pCtx->TxDmaHead = (PSDMAD)pData;
    pData += ROUND(sizeof (PSDMAD), 32) * pCtx->TxDmaNum;

    pCtx->RxDmaHead = (PSDMAD)pData;
    pData += ROUND(sizeof (PSDMAD), 32) * pCtx->RxDmaNum;

    pCtx->TxBufPtr = pData;
    pData += pCtx->TxBufNum * ROUND(pCtx->TxBufSize, 32);

    pCtx->RxBufPtr = pData;
    pData += pCtx->RxBufNum * ROUND(pCtx->RxBufSize, 32);

#ifdef SRIO_DEBUG
    uart_printf ("\r\nTX ptr: 0x%x\r\n", pCtx->TxBufPtr);
    uart_printf ("RX ptr: 0x%x\r\n", pCtx->RxBufPtr);
#endif

    return MX_OK;
}

/** @brief This function initializes SRIO DMA descriptors (TX and RX)
    @param pCtx [in] - the driver context
    @return [MXRC] an error code */

MXRC SrioDrvInitDma (SrioDrvCtx * pCtx)
{
    SDMAD * pDma;
    UINT32 i, j;
    PUINT8 pTx, pRx;
    UINT32 wait;

    // to initialize the WDMA (used for TX operations)

    pDma = pCtx->TxDmaHead;

    for (i = 0; i < pCtx->TxDmaNum; i++)
    {
        // 16 bytes for the burst: (1 << 6)
        // Next pointer is valie : (1 << 1)
        // The descriptor is valid:(1 << 1)

        pDma [i].CtrlStat = SRIO_DMA_DEF_LEN(pCtx->TxBufSize) | (1 << 1) | (1 << 0);
        pDma [i].SrcAddr  = (UINT32)(pCtx->TxBufPtr + i * ROUND(pCtx->TxBufSize, 32)) >> 3;

        if (pCtx->LBMode == SRIO_LB_OFF)
        {
            pDma [i].DstAddr  = pCtx->RemoteTxAddr >> 3;
        }
        else
        {
            pDma [i].DstAddr  = (UINT32)(pCtx->RxBufPtr + i * ROUND(pCtx->RxBufSize, 32)) >> 3;
        }

#if 0
        if ((i + 1) < pCtx->TxDmaNum)
            pDma [i].Next = ((UINT32)(pDma + i + 1) >> 3);
        else
            //pDma [i].Next = ((UINT32)pDma >> 3);
            pDma [i].Next = NULL;
#else
        pDma [i].Next = NULL;
#endif

        for (j = 0; j < pCtx->TxBufSize / 2; j++)
        {
            ((PUINT16)(pDma [i].SrcAddr << 3))[j] = (UINT16)((i << 12) | (j & 0xFFF));
        }

        //memset ((LPVOID)(pDma [i].SrcAddr << 3), i + 1, pCtx->TxBufSize);
    }

    // to initialize the RDMA (used for RX operations)

    pDma = pCtx->RxDmaHead;

    for (i = 0; i < pCtx->RxDmaNum; i++)
    {
        // Next pointer is valid : (1 << 1)
        // The descriptor is valid:(1 << 1)

        pDma [i].CtrlStat = SRIO_DMA_DEF_LEN(pCtx->RxBufSize) | (1 << 1) | (1 << 0);

        if (pCtx->LBMode == SRIO_LB_OFF)
        {
            pDma [i].SrcAddr  = pCtx->RemoteRxAddr >> 3;
        }
        else
        {
            pDma [i].SrcAddr  = (UINT32)(pCtx->RxBufPtr + i * ROUND(pCtx->RxBufSize, 32)) >> 3;
        }

        pDma [i].DstAddr  = (UINT32)(pCtx->RxBufPtr + i * ROUND(pCtx->RxBufSize, 32)) >> 3;

#if 0
        if ((i + 1) < pCtx->RxDmaNum)
            pDma [i].Next = ((UINT32)(pDma + i + 1) >> 3);
        else
            //pDma [i].Next = ((UINT32)pDma >> 3);
            pDma [i].Next = NULL;
#else
        pDma [i].Next = NULL;
#endif
    }

    // Put GRIO0 and GRIO1 in loopback mode ; Program the GRIO BDIDCSR register to set the Device ID to 0x00
    //RAD_CFG_SRIO_LOOPBACK = 0x3;

    // Program Page #32
    REG32(RAD_SRIO_RAB_APB_CSR(pCtx->nID)) = 0x0;

    REG32(RAD_SRIO_GRIO_BDIDCSR(pCtx->nID)) = 0x0;

    // Program SRIO0/SRIO1 RPIO LUT
    // bit[4:1]=1000 - 256M, Lower 28 bits from RIO will form lower 28 bits of AXI address
    REG32(RAD_SRIO_RAB_RIO_AMAP_LUT0(pCtx->nID)) = ((UINT32)pCtx->DriverBuf & 0xFF000000) | (1 << 4) | (1 << 0);

    // Step 1 : Enable WDMA, RPIO enable bits and select depth of descriptor array in RAB_CTRL
    // 1 pref descriptor
    // RIO PIO Enable
    // Write DMA Enable
    // Read DMA Enable
    REG32(RAD_SRIO_RAB_CTRL(pCtx->nID)) |= 0x00E; // 0x106

    // Step 2 : Enable WDMA and RDMA interrupt bits in  RAB_INTR_ENAB_GNRL
    // RIO PIO Interrupt Enable
    // Write DMA Interrupt Enable
    // Read DMA Interrupt Enable
    REG32(RAD_SRIO_RAB_INTR_ENAB_GNRL(pCtx->nID)) |= 0x0E;

    // Step 3 : Enable the WDMA descriptor transfer completed bits and chain descriptor
    // Write DMA Chain Descriptor Transfer Completed
    // Write DMA Descriptor Transfer Completed
    REG32(RAD_SRIO_RAB_INTR_ENAB_WDMA(pCtx->nID)) = 0x0000FFFF;

    // Step 4 : Enable the RDMA descriptor transfer completed bits and chain descriptor
    // Read DMA Chain Descriptor Transfer Completed
    // Read DMA Descriptor Transfer Completed
    REG32(RAD_SRIO_RAB_INTR_ENAB_RDMA (pCtx->nID))= 0x0000FFFF;

    // Program page #65 (0x41)
    REG32(RAD_SRIO_RAB_APB_CSR(pCtx->nID)) = 0x00410000;

    //  Step 8 : Program the RAB_WDMA/RDMA_0_ADDR registers to sepcify the Descriptor
    REG32(RAD_SRIO_RAB_WDMA_0_ADDR(pCtx->nID)) = (UINT32)pCtx->TxDmaHead;
    REG32(RAD_SRIO_RAB_RDMA_0_ADDR(pCtx->nID)) = (UINT32)pCtx->RxDmaHead;

#ifdef SRIO_DEBUG
    uart_printf ("WDMA ADDR: %x\r\n", REG32(RAD_SRIO_RAB_WDMA_0_ADDR(pCtx->nID)));
    uart_printf ("RDMA ADDR: %x\r\n", REG32(RAD_SRIO_RAB_RDMA_0_ADDR(pCtx->nID)));
#endif

    // Step 8.1 : Program RPIO
    REG32(RAD_SRIO_RAB_INTR_ENAB_RPIO(pCtx->nID)) = 0xFFFF;
    REG32(RAD_SRIO_RAB_RPIO0_CTRL(pCtx->nID))= 0x0003;

#if 0
    // Step 10 : Specify priority, dest id and set start bit in RAB_WDMA_0_CTRL
    REG32(RAD_SRIO_RAB_WDMA_0_CTRL(pCtx->nID)) = 0x1;

    // Step 11 : Wait for Inbound data - check RPIO Interrupt Status
    // Program page #0
    REG32(RAD_SRIO_RAB_APB_CSR(pCtx->nID)) = 0x0;

    uart_printf("Waiting for SRIO0 status to be set ... ");

    wait = 0;

    while ((REG32(RAD_SRIO_RAB_INTR_STAT_RPIO(pCtx->nID)) & 0x1) == 0x0)
    {
        _NOP();

        wait ++;

        if (wait > 1000000)
            break;
    }

    uart_printf("OK (wait=%d)\n", wait);

    for (i = 0; i < pCtx->TxDmaNum; i++)
    {
        pTx = pCtx->TxBufPtr + i * ROUND(pCtx->TxBufSize, 32);
        pRx = pCtx->RxBufPtr + i * ROUND(pCtx->RxBufSize, 32);

        uart_printf ("WDMA: %x   RDMA: %x\r\n", pCtx->TxDmaHead[i].CtrlStat, pCtx->RxDmaHead[i].CtrlStat);

        uart_printf ("FRM#%d: %02x %02x %02x => %02x %02x %02x\r\n", i,
            pTx[0], pTx[1], pTx[2],
            pRx[0], pRx[1], pRx[2]
        );
    }

    uart_printf ("\r\nFINISHED\r\n");
    uart_printf (" * WDMA CTRL     : %x\r\n", REG32(RAD_SRIO_RAB_WDMA_0_CTRL(pCtx->nID)));
    uart_printf (" * WDMA ADDR     : %x\r\n", REG32(RAD_SRIO_RAB_WDMA_0_ADDR(pCtx->nID)));
    uart_printf (" * RDMA ADDR     : %x\r\n", REG32(RAD_SRIO_RAB_RDMA_0_ADDR(pCtx->nID)));
    uart_printf (" * INTR_STAT_RPIO: %x\r\n", REG32(RAD_SRIO_RAB_INTR_STAT_RPIO(pCtx->nID)));
    uart_printf (" * RAB_STAT_STAT : %x\r\n", REG32(RAD_SRIO_RAB_STAT_STAT(pCtx->nID)));

#endif
    return MX_OK;
}


/** @brief This function is used to send already prepared buffer, the
            address of buffer can be taken by using of SrioDrvGetTxBuffer function

    @param pCtx [in] - the pointer to the driver context
    @return [MXRC] an error code */

MXRC SrioDrvSendData (SrioDrvCtx * pCtx)
{
    UINT32 wait, i, tick;
    PUINT16 pTx, pRx;
    PSDMAD pTxDma;
    UINT32 rc = MX_OK;

    _ASSERT_PTR(pCtx);

    pTxDma = &pCtx->TxDmaHead [pCtx->TxDmaCurIdx++];

    // to log TX data
    if ((pCtx->LogCtrl & SRIO_CTRL_TX_LOG) && ((pCtx->TxLogPos + pCtx->TxBufSize) <= pCtx->TxLogSize))
    {
        rc = MdmaCopy (MDMA_HW_RAD, (LPVOID)(pCtx->TxLogPtr + pCtx->TxLogPos),
                          (LPVOID)(pTxDma->SrcAddr << 3),
                          pCtx->TxBufSize);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        pCtx->TxLogPos += pCtx->TxBufSize;

        if (pCtx->TxLogPos >= pCtx->TxLogSize)
        {
            if (pCtx->LogCtrl & SRIO_CTRL_TX_LOG_RR)
                pCtx->TxLogPos = 0;
        }
    }

    if (pCtx->TxDmaCurIdx >= pCtx->TxDmaNum)
        pCtx->TxDmaCurIdx = 0;

    // Program page #65 (0x41)
    REG32(RAD_SRIO_RAB_APB_CSR(pCtx->nID)) = 0x00410000;

    pTxDma->CtrlStat = SRIO_DMA_DEF_LEN(pCtx->TxBufSize) | (1 << 1) | (1 << 0);
    REG32(RAD_SRIO_RAB_WDMA_0_ADDR(pCtx->nID)) = (UINT32)pTxDma;

#ifdef SRIO_DEBUG
    uart_printf ("SRIO TX> WDMA ADDR:%x TxDmaCur:%x\r\n", REG32(RAD_SRIO_RAB_WDMA_0_ADDR(pCtx->nID)), pTxDma);
#endif

    REG32(RAD_SRIO_RAB_WDMA_0_CTRL(pCtx->nID)) = 0x1;

    // Step 11 : Wait for Inbound data - check RPIO Interrupt Status
    // Program page #0
    REG32(RAD_SRIO_RAB_APB_CSR(pCtx->nID)) = 0x0;

#ifdef SRIO_DEBUG
    uart_printf("Waiting for SRIO0 status to be set ... ");
#endif

    wait = 0;

    tick = MxGetTicks ();
    while ((pTxDma->CtrlStat & 0xFF000000) == 0)
    {
        wait ++;

        if (wait > 10000000)
        {
            rc = RC_SRIODRV_SEND_TIMEOUT;
            break;
        }
    }
    tick = MxGetTicks () - tick;

#ifdef SRIO_DEBUG
    uart_printf("OK (wait=%d)(tick=%d)(status=0x%x)\n", wait, tick, pTxDma->CtrlStat >> 24);


    for (i = 0; i < pCtx->TxDmaNum; i++)
    {
        pTx = (PUINT16)(pCtx->TxBufPtr + i * ROUND(pCtx->TxBufSize, 32));
        pRx = (PUINT16)(pCtx->RxBufPtr + i * ROUND(pCtx->RxBufSize, 32));

        uart_printf ("WDMA:%x   RDMA:%x\r\n", pCtx->TxDmaHead[i].CtrlStat, pCtx->RxDmaHead[i].CtrlStat);
        uart_printf ("FRM#%d: %04x %04x %04x => %04x %04x %04x\r\n", i,
            pTx[0], pTx[1], pTx[2],
            pRx[0], pRx[1], pRx[2]
        );

        // to erase the buffers
        memset (pRx, 0, pCtx->RxBufSize);
    }

    uart_printf ("\r\n * WDMA CTRL     : %x\r\n", REG32(RAD_SRIO_RAB_WDMA_0_CTRL(pCtx->nID)));
    uart_printf (" * WDMA ADDR     : %x\r\n", REG32(RAD_SRIO_RAB_WDMA_0_ADDR(pCtx->nID)));
    uart_printf (" * RDMA ADDR     : %x\r\n", REG32(RAD_SRIO_RAB_RDMA_0_ADDR(pCtx->nID)));
    uart_printf (" * INTR_STAT_RPIO: %x\r\n", REG32(RAD_SRIO_RAB_INTR_STAT_RPIO(pCtx->nID)));
    uart_printf (" * RAB_STAT_STAT : %x\r\n", REG32(RAD_SRIO_RAB_STAT_STAT(pCtx->nID)));
    uart_printf ("\r\n");
#endif

    return rc;
}

/** @brief This function is used to receive data in free RX buffer, the
            address of buffer can be taken by using of SrioDrvGetRxBuffer function

    @param pCtx [in] - the pointer to the driver context
    @return [MXRC] an error code */

MXRC SrioDrvRecvData (SrioDrvCtx * pCtx)
{
    PSDMAD pRxDma;
    MXRC rc = MX_OK;
    UINT32 wait, tick;

    _ASSERT_PTR(pCtx);

    pRxDma = &pCtx->RxDmaHead [pCtx->RxDmaCurIdx++];

    // to log RX data
    if ((pCtx->LogCtrl & SRIO_CTRL_RX_LOG) && ((pCtx->RxLogPos + pCtx->RxBufSize) <= pCtx->RxLogSize))
    {
        rc = MdmaCopy (MDMA_HW_RAD, (LPVOID)(pCtx->RxLogPtr + pCtx->RxLogPos),
                          (LPVOID)(pRxDma->DstAddr << 3),
                          pCtx->RxBufSize);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        pCtx->RxLogPos += pCtx->RxBufSize;

        if (pCtx->RxLogPos >= pCtx->RxLogSize)
        {
            if (pCtx->LogCtrl & SRIO_CTRL_RX_LOG_RR)
                pCtx->RxLogPos = 0;
        }
    }

    if (pCtx->RxDmaCurIdx >= pCtx->RxDmaNum)
        pCtx->RxDmaCurIdx = 0;

    // Program page #65 (0x41)
    REG32(RAD_SRIO_RAB_APB_CSR(pCtx->nID)) = 0x00410000;

    pRxDma->CtrlStat = SRIO_DMA_DEF_LEN(pCtx->RxBufSize) | (1 << 1) | (1 << 0);
    REG32(RAD_SRIO_RAB_WDMA_0_ADDR(pCtx->nID)) = (UINT32)pRxDma;

#ifdef SRIO_DEBUG
    uart_printf ("SRIO RX> RDMA ADDR:%x RxDmaCur:%x\r\n", REG32(RAD_SRIO_RAB_RDMA_0_ADDR(pCtx->nID)), pRxDma);
#endif

    REG32(RAD_SRIO_RAB_RDMA_0_CTRL(pCtx->nID)) = 0x1;

    // Step 11 : Wait for Inbound data - check RPIO Interrupt Status
    // Program page #0
    REG32(RAD_SRIO_RAB_APB_CSR(pCtx->nID)) = 0x0;

    wait = 0;

    tick = MxGetTicks ();
    while ((pRxDma->CtrlStat & 0xFF000000) == 0)
    {
        wait ++;

        if (wait > 10000000)
        {
            rc = RC_SRIODRV_RECV_TIMEOUT;
            break;
        }
    }
    tick = MxGetTicks () - tick;

    return rc;
}

/** @brief This function is designed to enable SRIO device
           with specified parameters

    @param pCfg [in] - the pointer to the configuration parameters
    @return [MXRC] an error code */

MXRC SrioDrvEnabledDevice (SRIODRVCFG * pCfg)
{
    MXRC rc = MX_OK;
    SrioDrvCtx * pCtx = NULL;

    _ASSERT_PTR (pCfg);

    if (pCfg == NULL)
        return RC_SRIODRV_PARAM_ERROR;

    pCtx = SrioDrvGetCtx (pCfg->nID);

    _ASSERT_PTR (pCtx);

    if (pCtx == NULL)
        return RC_SRIODRV_CTX_ERROR;

    memset (pCtx, 0, sizeof (*pCtx));

    // Context initialization
    pCtx->nID       = pCfg->nID;
    pCtx->LBMode    = pCfg->LBMode;
    pCtx->LogCtrl   = pCfg->LogCtrl;
    pCtx->Speed     = pCfg->Speed;
    pCtx->TxBufSize = pCfg->TxBufSize;
    pCtx->TxBufNum  = pCfg->TxBufNum;
    pCtx->RxBufSize = pCfg->RxBufSize;
    pCtx->RxBufNum  = pCfg->RxBufNum;
    pCtx->TxLogPtr  = pCfg->TxLogPtr;
    pCtx->TxLogSize = pCfg->TxLogSize;
    pCtx->RxLogPtr  = pCfg->RxLogPtr;
    pCtx->RxLogSize = pCfg->RxLogSize;
    pCtx->RxCb      = pCfg->RxCb;
    pCtx->RxCbPtr   = pCfg->RxCbPtr;
    pCtx->TxCb      = pCfg->TxCb;
    pCtx->TxCbPtr   = pCfg->TxCbPtr;

    pCtx->RemoteTxAddr = pCfg->RemoteTxAddr;
    pCtx->RemoteRxAddr = pCfg->RemoteRxAddr;

    rc = MxSetIntHandler((pCfg->nID == 0) ? HAL_IRQ_SRIO_0 : HAL_IRQ_SRIO_1, INT_TYPE_IRQ, PRI_SRIO, SrioIrqHandler, pCtx);

    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    if (FAILED(rc = MxEnableInt((pCfg->nID == 0) ? HAL_IRQ_SRIO_0 : HAL_IRQ_SRIO_1)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    if (FAILED(rc = SrioDrvAllocData (pCtx)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    if (FAILED(rc = SrioDrvInitSerdes (pCfg)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    if (FAILED(rc = SrioDrvInitDma (pCtx)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    pCtx->Ctrl |= SRIO_CTRL_INITED;

    return MX_OK;
}


/** @brief This function returns a pointer to the current buffer that can be used
            by the application to put data and it is used by SrioDrvSendData () function,
            the pointer is automatically changed by the system after callin of SrioDrvSendData ()

    @param pCtx  [in] - the device driver context
    @param nRes  [in] - reserved, SBZ

    @return [LPVOID] the pointer to the TX buffer */

LPVOID SrioDrvGetTxBuffer (SrioDrvCtx * pCtx, UINT32 nRes)
{
    _ASSERT_PTR (pCtx);

    if (pCtx->TxDmaHead == NULL)
        return NULL;

    return (LPVOID)(pCtx->TxDmaHead[pCtx->TxDmaCurIdx].SrcAddr << 3);
}

/** @brief This function returns a pointer to the current rx buffer that can be used
            by the system in operation of data reading and it is used by SrioDrvRecvData() function,
            the pointer is automatically changed by the system after callin of SrioDrvRecvData()

    @param pCtx  [in] - the device driver context
    @param nRes  [in] - reserved, SBZ

    @return [LPVOID] the pointer to the RX buffer */

LPVOID SrioDrvGetRxBuffer (SrioDrvCtx * pCtx, UINT32 nRes)
{
    _ASSERT_PTR (pCtx);

    if (pCtx->RxDmaHead == NULL)
        return NULL;

    return (LPVOID)(pCtx->RxDmaHead[pCtx->RxDmaCurIdx].DstAddr << 3);
}

/** @brief This function shutdowns SRIO devices
    @return [MXRC] an error code */

MXRC SrioDrvShutdown (void)
{
    UINT32 i;
    SrioDrvCtx * pCtx;

    for (i = 0; i < 2; i++)
    {
        pCtx = SrioDrvGetCtx (i);

        _ASSERT_PTR (pCtx);

        if (pCtx == NULL)
            continue;

        if ((pCtx->Ctrl & SRIO_CTRL_INITED) == 0)
            continue;
#ifdef SRIO_DEBUG
        uart_printf ("SRIO-%d shutdown ... ", i);
#endif

        // Program page #65 (0x41)
        REG32(RAD_SRIO_RAB_APB_CSR(i)) = 0x00410000;
        REG32(RAD_SRIO_RAB_WDMA_0_CTRL(i)) = (1 << 1); // suspend the DMA

        // Program page #0 (0x0)
        REG32(RAD_SRIO_RAB_APB_CSR(i)) = 0;

        // To reset the AXI PIO Reset/RIO PIO Reset/Write DMA Reset/Read DMA Reset
        REG32(RAD_SRIO_RAB_RST_CTRL(i)) = 0xFFFFFFFF;
        MxDelayTicks (TICKS_TO_MS(2));
        REG32(RAD_SRIO_RAB_RST_CTRL(i)) = 0x0;

        // To deallocate driver buffer
        if (pCtx->DriverBuf != NULL)
            MxHeapFree (DspHeap, pCtx->DriverBuf);

        pCtx->DriverBuf = NULL;

        if (i == 0)
            REG32(RAD_AXI_RESET_1) |= ((1 << 1) | (1 << 2) | (1 << 3) | (1 << 4)); // reset SRIOs
        else
            REG32(RAD_AXI_RESET_1) |= ((1 << 1) | (1 << 5) | (1 << 6) | (1 << 7)); // reset SRIOs

        REG32(RAD_CLUSTER_CNTRL) = 0x0;
        MxDelayTicks(TICKS_TO_MS(10));

        // reset Serdes 0
        if (i == 0)
            REG32(RAD_SRDS0_CNTRL) = 0x01;
        else
            REG32(RAD_SRDS2_CNTRL) = 0x01;

        MxDisableInt((i == 0) ? HAL_IRQ_SRIO_0 : HAL_IRQ_SRIO_1);

        memset (pCtx, 0, sizeof (*pCtx));
#ifdef SRIO_DEBUG
        uart_printf ("OK\r\n");
#endif

    }

    return MX_OK;
}

MXRC SrioDrv_init(UINT32 nInit, LPVOID * pCtxStorage)
{
    SrioDrvCtx * pCtx;
    UINT32 nID;
    MXRC rc;

    nID = (nInit == SRIO_0_BASEADDR) ? 0 : 1;

    pCtx = &SrioCtx [nID];

    memset (pCtx, 0, sizeof (SrioDrvCtx));

#if 0
    pCtx->nID = nID;

    rc = MxSetIntHandler((nID == 0) ? HAL_IRQ_SRIO_0 : HAL_IRQ_SRIO_1, INT_TYPE_IRQ, PRI_SRIO, SrioIrqHandler, pCtx);

    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    if (FAILED(rc = MxEnableInt((nID == 0) ? HAL_IRQ_SRIO_0 : HAL_IRQ_SRIO_1)))
    {
        _ASSERT_RC(rc);
        return rc;
    }
#endif

    return rc;
}

MXRC SrioDrv_config(LPVOID pDrvCtx, UINT32 nCfg, LPVOID pCfgData, UINT32 nCfgDataSize)
{
    return MX_OK;
}

MXRC SrioDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData, UINT32 nCmdDataSize)
{
    return MX_OK;
}

MXRC SrioDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData, UINT32 * pnCmdDataSize)
{
    return MX_OK;
}

#endif // SRIO_DRV_ENABLED

