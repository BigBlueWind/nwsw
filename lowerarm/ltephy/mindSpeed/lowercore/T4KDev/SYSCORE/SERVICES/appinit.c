//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL appinit.c
 *
 * @brief This file contains 4GMX contexts, drivers table for every one CPU, and table of
*       application initialization handlers used by the system startup code
 * @author Mindspeed Technologies
 * @version $Revision: 1.97 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "config.h"
#include "appinit.h"

#include "uartdrv.h"
#include "icdrv.h"
#include "timerdrv.h"
#include "mdmadrv.h"

#include "appprintf.h"
#include "intercpu.h"
#include "supervisor.h"
#include "heaps.h"
#include "diagnostics.h"
#include "cline.h"
#include "lcorecfg.h"
#include "cline.h"
#include "hbidrv.h"
#include "schtaskdrv.h"
#include "gemdrv.h"
#include "ncnbdata.h"
#include "systimers.h"
#include "clkrst.h"
#include "sysshutdown.h"
#include "sysclkctrl.h"
#include "FECdrv.h"
#include "mlog.h"
#include "version.h"
#include "spidrv.h"
#ifdef RDMEM_CACHE_CONTROL_ENABLED
#include "rdmemdrv.h"
#endif
#include "iqphone.h"
#include "servmgr.h"
#include "globalstoragearrays.h"
#include "pm.h"
#ifdef UNIDIAG_ENABLED
#include "unified_diag.h"
#endif
#include "sysddrctrl.h"
#include "ddrmondrv.h"
#include "swdiag.h"
#include "conicpu.h"


extern const LCoreCfg CoreCfg;

MXCONTEXT MxCtx[CPU_NUM];

#ifdef MX_INTERCPU_ENABLED
__align(32) MXICCTX   MxICCtx [CPU_NUM];          // The inter-cpu contextes of the 4GMX systems
__align(32) MXICCTX * MxICTable [CPU_NUM] = {0};  // The table of the pointers to the IC contextes
__align(32) MXSMPCTX  MxSmpCtx;
#endif //MX_INTERCPU_ENABLED

__align(1024) VUINT32 NCNBVAR MxStarted [CPU_NUM];          // The inter-cpu contextes of the 4GMX systems
__align(1024) VUINT32 NCNBVAR MxCoreAllowed[CPU_NUM];

// Place string constants on aligned adresses
__align(32) const char LTmrName[] = "LTmr";
__align(32) const char UARTName0[] = "UART0";
__align(32) const char UARTName1[] = "UART1";
__align(32) const char UARTName2[] = "UART2";
__align(32) const char* UARTName[3] = {UARTName0, UARTName1, UARTName2};
__align(32) const char HBIName[] = "HBI";
__align(32) const char STDrvName[] = "STDrv";
__align(32) const char GEM1Name[] = "GEM1";
__align(32) const char STmrName[] = "STmr";
__align(32) const char SPIDrvName[]="SPI";
#ifdef RDMEM_CACHE_CONTROL_ENABLED
__align(32) const char RdMemDrvName[]="RdMem";
#endif
__align(32) const char DDRMonDrvName[]="DDRMON";

#ifdef SYS_INFO_ENABLED
SysStayInfo __align(32) NCNBVAR syscoreinfo [CPU_NUM] = {0};
#endif

/******************************************************************************/
/*                                                                            */
/*                   The ARMx2:core0 driver table                             */
/*                                                                            */
/******************************************************************************/
BEGIN_DRV_TABLE(SysDrvTableCpu0)

#ifdef GIC_INIT
    DRIVER(ICDrv, 0x0)                             /**< The GIC driver          */
#endif

#ifdef LOCAL_TIMER_INIT
    DRIVER_NAME(TimerDrv, LOCAL_TIMER_ADDR, LTmrName)    /**< The CPU core local timer and watchdog. Attention: don't move this driver */
#endif //LOCAL_TIMER_INIT

#ifdef SYS_SHUTDOWN_DRV_ENABLED
    DRIVER(SysShdwnDrv, 0)    			    /**< The shutdown driver */
#endif

#ifdef PRINTF_INIT0
    DRIVER_NAME(UartDrv,  UART0_BASEADDR, UARTName0)      /**< The UART0 driver, it might be used by the printf function */
#endif //PRINTF_INIT0

#ifdef PRINTF_INIT1
    DRIVER_NAME(UartDrv,  UART1_BASEADDR, UARTName1)      /**< The UART1 driver, it might be used by the printf function */
#endif //PRINTF_INIT1

#ifdef PRINTF_INIT2
    DRIVER_NAME(UartDrv,  UART2_BASEADDR, UARTName2)      /**< The UART2 driver, it might be used by the printf function */
#endif //PRINTF_INIT2

    DRIVER_NAME(HBIDrv, HBI_APB_BASEADDR, HBIName)    /**< The HBI driver */

    DRIVER_NAME(STDrv,  0, STDrvName)    /**< The Scheduler Task Driver */

#if defined(PHY_GEM_ENABLED) && defined (PHY_GEM_FORCED_CORE0)
    DRIVER_NAME(GemDrv, GEMAC1_BASEADDR, GEM1Name)
#endif

#ifdef SYS_TIMER_INIT
    DRIVER_NAME(SysTimerDrv, 0, STmrName)
#endif

#ifdef SPI_DRV_INIT
    DRIVER_NAME(SPIDrv, 10, SPIDrvName)
#endif

#ifdef RDMEM_CACHE_CONTROL_ENABLED
    DRIVER_NAME(RdMemDrv, 0, RdMemDrvName)
#endif

    DRIVER_NAME(DDRMonDrv, 0, DDRMonDrvName)

END_DRV_TABLE()


/******************************************************************************/
/*                   The core1 driver table                                   */
/******************************************************************************/
/** @brief The drivers table for the second ARM core */

#ifdef CPU1_ENABLED

BEGIN_DRV_TABLE(SysDrvTableCpu1)

#ifdef GIC_INIT
    DRIVER(ICDrv, 0x1)
#endif

#ifdef LOCAL_TIMER_INIT
    DRIVER_NAME(TimerDrv, LOCAL_TIMER_ADDR, LTmrName)    /**< The CPU core local timer and watchdog. Attention: don't move this driver */
#endif //LOCAL_TIMER_INIT

#ifdef SYS_SHUTDOWN_DRV_ENABLED
    DRIVER(SysShdwnDrv, 0)    			    /**< The shutdown driver */
#endif

#ifdef PRINTF_INIT0
    DRIVER_NAME(UartDrv,  UART0_BASEADDR, UARTName0)      /**< The UART0 driver, it might be used by the printf function */
#endif //PRINTF_INIT0

#ifdef PRINTF_INIT1
    DRIVER_NAME(UartDrv,  UART1_BASEADDR, UARTName1)      /**< The UART1 driver, it might be used by the printf function */
#endif //PRINTF_INIT1

#ifdef PRINTF_INIT2
    DRIVER_NAME(UartDrv,  UART2_BASEADDR, UARTName2)      /**< The UART2 driver, it might be used by the printf function */
#endif //PRINTF_INIT2

    DRIVER_NAME(STDrv,  1, STDrvName)    /**< The Scheduler Task Driver */

#if defined(PHY_GEM_ENABLED) && defined (PHY_GEM_FORCED_CORE1)
    DRIVER_NAME(GemDrv, GEMAC1_BASEADDR, GEM1Name)
#endif

#ifdef SYS_TIMER_INIT
    DRIVER_NAME(SysTimerDrv, 0, STmrName)
#endif

END_DRV_TABLE()

#endif /*CPU1_ENABLED */

/******************************************************************************/
/*                   The core2 driver table                                   */
/******************************************************************************/

/** @brief The drivers table for the second ARM core */

#ifdef CPU2_ENABLED

BEGIN_DRV_TABLE(SysDrvTableCpu2)

#ifdef GIC_INIT
    DRIVER(ICDrv, 0x2)
#endif

#ifdef LOCAL_TIMER_INIT
    DRIVER_NAME(TimerDrv, LOCAL_TIMER_ADDR, LTmrName)    /**< The CPU core local timer and watchdog. Attention: don't move this driver */
#endif //LOCAL_TIMER_INIT

#ifdef SYS_SHUTDOWN_DRV_ENABLED
    DRIVER(SysShdwnDrv, 0)    			    /**< The shutdown driver */
#endif

#ifdef PRINTF_INIT0
    DRIVER_NAME(UartDrv,  UART0_BASEADDR, UARTName0)      /**< The UART0 driver, it might be used by the printf function */
#endif //PRINTF_INIT0

#ifdef PRINTF_INIT1
    DRIVER_NAME(UartDrv,  UART1_BASEADDR, UARTName1)      /**< The UART1 driver, it might be used by the printf function */
#endif //PRINTF_INIT1

#ifdef PRINTF_INIT2
    DRIVER_NAME(UartDrv,  UART2_BASEADDR, UARTName2)      /**< The UART2 driver, it might be used by the printf function */
#endif //PRINTF_INIT2

    DRIVER_NAME(STDrv,  2, STDrvName)    /**< The Scheduler Task Driver */

#if defined(PHY_GEM_ENABLED) && defined (PHY_GEM_FORCED_CORE2)
    DRIVER_NAME(GemDrv, GEMAC1_BASEADDR, GEM1Name)
#endif

#ifdef SYS_TIMER_INIT
    DRIVER_NAME(SysTimerDrv, 0, STmrName)
#endif

END_DRV_TABLE()

#endif /*CPU2_ENABLED */

/******************************************************************************/
/*                   The core3 driver table                                   */
/******************************************************************************/

/** @brief The drivers table for the second ARM core */

#ifdef CPU3_ENABLED

BEGIN_DRV_TABLE(SysDrvTableCpu3)

#ifdef GIC_INIT
    DRIVER(ICDrv, 0x2)
#endif

#ifdef LOCAL_TIMER_INIT
    DRIVER_NAME(TimerDrv, LOCAL_TIMER_ADDR, LTmrName)    /**< The CPU core local timer and watchdog. Attention: don't move this driver */
#endif //LOCAL_TIMER_INIT

#ifdef SYS_SHUTDOWN_DRV_ENABLED
    DRIVER(SysShdwnDrv, 0)    			    /**< The shutdown driver */
#endif

#ifdef PRINTF_INIT0
    DRIVER_NAME(UartDrv,  UART0_BASEADDR, UARTName0)      /**< The UART0 driver, it might be used by the printf function */
#endif //PRINTF_INIT0

#ifdef PRINTF_INIT1
    DRIVER_NAME(UartDrv,  UART1_BASEADDR, UARTName1)      /**< The UART1 driver, it might be used by the printf function */
#endif //PRINTF_INIT1

#ifdef PRINTF_INIT2
    DRIVER_NAME(UartDrv,  UART2_BASEADDR, UARTName2)      /**< The UART2 driver, it might be used by the printf function */
#endif //PRINTF_INIT2

    DRIVER_NAME(STDrv,  3, STDrvName)    /**< The Scheduler Task Driver */

#if defined(PHY_GEM_ENABLED) && defined (PHY_GEM_FORCED_CORE3)
    DRIVER_NAME(GemDrv, GEMAC1_BASEADDR, GEM1Name)
#endif

#ifdef SYS_TIMER_INIT
    DRIVER_NAME(SysTimerDrv, 0, STmrName)
#endif

END_DRV_TABLE()

#endif /*CPU3_ENABLED */

/** @brief The table of routins for application initialization used by
           the block of ARM initialization */

AppInitCtx appinit[CPU_NUM] =
{
#ifdef GIC_INIT
    {SysDrvTableCpu0, 0x00000000, IdleCpu0, NULL, AppInitCpu0, EXT_THR_TRACER_PROC}
#else
    {SysDrvTableCpu0, 0xFFFFFFFF, IdleCpu0, NULL, AppInitCpu0, EXT_THR_TRACER_PROC}
#endif

#ifdef CPU1_ENABLED

     #ifdef GIC_INIT
         ,{SysDrvTableCpu1, 0x00000000, IdleCpu1, NULL, AppInitCpu1, EXT_THR_TRACER_PROC}
     #else
         ,{SysDrvTableCpu1, 0xFFFFFFFF, IdleCpu1, NULL, AppInitCpu1, EXT_THR_TRACER_PROC}
     #endif

#endif /*CPU1_ENABLED */

#ifdef CPU2_ENABLED

     #ifdef GIC_INIT
         ,{SysDrvTableCpu2, 0x00000000, IdleCpu2, NULL, AppInitCpu2, EXT_THR_TRACER_PROC}
     #else
         ,{SysDrvTableCpu2, 0xFFFFFFFF, IdleCpu2, NULL, AppInitCpu2, EXT_THR_TRACER_PROC}
     #endif

#endif /*CPU2_ENABLED */

#ifdef CPU3_ENABLED

     #ifdef GIC_INIT
         ,{SysDrvTableCpu3, 0x00000000, IdleCpu3, NULL, AppInitCpu3, EXT_THR_TRACER_PROC}
     #else
         ,{SysDrvTableCpu3, 0xFFFFFFFF, IdleCpu3, NULL, AppInitCpu3, EXT_THR_TRACER_PROC}
     #endif

#endif /*CPU3_ENABLED */

};

__align(32) CpuTraceType CpuTraceMode [CPU_NUM];

/********************************************************************
*   The entry point of application initialization for CPU-0         *
*********************************************************************/

UINT32 _GetCpuTraceMode (void)
{
    return CpuTraceMode [MxGetCpuID ()].nCurMode;
}

void _SetCpuTraceMode (UINT32 nMode)
{
    UINT32 nCpuID = MxGetCpuID ();
    IRQTYPE irq;

    irq = ARM_INT_disable ();

    if (CpuTraceMode [nCpuID].nCurMode != nMode)
    {
        CpuTraceMode [nCpuID].nCurMode = nMode;

        MLogResourceCondition (nMode, nCpuID + RESOURCE_UARM0, 0);
    }

    ARM_INT_restore (irq);
}


#ifdef TIMER_TEST
UINT32 TestTimer (void * p);
#endif

#if defined(PHY_GEM_ENABLED)

#if 0
char buf [600];
#endif

MXRC AppInitPhyGem (UINT32 nCpuID)
{
    MXRC rc = MX_OK;

    GemCfg    cfg;
    GemDMACfg dmacfg;
    LPVOID    pGemDrv;

#if 0
    UINT32    val;
#endif

    pGemDrv = GemDrvGetCtx (1);

    _ASSERT_PTR (pGemDrv);

    cfg.Phy     = RGMII;
    cfg.Speed   = MAC_SPEED_1000Mbps;

     // 00-11-22-33-44-55 - temporary MAC address

    cfg.MacAddr[0] = 0x00;
    cfg.MacAddr[1] = 0x11;
    cfg.MacAddr[2] = 0x22;
    cfg.MacAddr[3] = 0x33;
    cfg.MacAddr[4] = 0x44;
    cfg.MacAddr[5] = 0x55;

    if (FAILED(rc = GemDrvCfg(pGemDrv, &cfg)))
        return rc;

    // GEM DMA configuration

    dmacfg.RxDescrCount = ETH_NUM_RX_BUFS;
    dmacfg.TxDescrCount = ETH_NUM_TX_BUFS;

    dmacfg.RxDescr   = (HGRxFrm)gem_rx_frm;
    dmacfg.TxDescr   = (HGTxFrm)gem_tx_frm;
    dmacfg.RxBuffers = gem_rx_buffers;

    dmacfg.DMAConfig = GEM_DMA_DEF_RX_SIZE(2048) | GEM_DMA_BURST_INCR4;
    //dmacfg.DMAConfig = GEM_DMA_DEF_RX_SIZE(GEM_0_RX_BUF_SIZE) | GEM_DMA_BURST_INCR16;
    dmacfg.RxBufOpt    = GEM_DMA_RX_IRQ_ENB;

    if (FAILED(rc = GemDrvConfigDMA(pGemDrv, &dmacfg)))
        return rc;

    GemDrvDisableRX(pGemDrv);
    GemDrvEnableTX(pGemDrv);

    if (FAILED(rc = GemDrvCopyAll (pGemDrv, 1)))
       return rc;

    if (FAILED(rc = GemDrvEnBroadcast (pGemDrv, 1)))
       return rc;

    // RX/TX bits setup
#if 0

    val = GemDrvReadPhy (pGemDrv, 0, 20);
    val |= (1 << 7) | (1 << 1);
    rc = GemDrvWritePhy (pGemDrv, 0, 20, val);

    // SW reset
    val = GemDrvReadPhy (pGemDrv, 0, 0);
    val |= (1 << 15);
    rc = GemDrvWritePhy (pGemDrv, 0, 0, val);

#endif

    // Just an example of GEM driver usage

#if 0
    buf [0] = 0x00;
    buf [1] = 0x07;
    buf [2] = 0xE9;
    buf [3] = 0xB7;
    buf [4] = 0x83;
    buf [5] = 0x31;

    buf [6] = 0x00;
    buf [7] = 0x11;
    buf [8] = 0x22;
    buf [9] = 0x33;
    buf [10]= 0x44;
    buf [11]= 0x55;

    buf [12] = 0x08;
    buf [13] = 0x00;

    printf ("Sending test packet ... \r\n");
    rc = GemDrvTxFrm (pGemDrv, GEM_DMA_TX_CRC, buf, 100);

    printf ("Sending test packet ... \r\n");
    rc = GemDrvTxFrm (pGemDrv, GEM_DMA_TX_CRC, buf, 150);

    printf ("Sending test packet ... \r\n");
    rc = GemDrvTxFrm (pGemDrv, GEM_DMA_TX_CRC, buf, 200);

    printf ("OK\r\n");

#endif

    return MX_OK;
}

#endif

MXRC EnableLCore (UINT32 nID)
{
    UINT32 core_id = (1 << (nID+4));
    UINT32 rc;
    IRQTYPE irq;

    while (TRUE)
    {
    	REG32(SYS_A9DP_RESET_0) |= core_id;

        __nop ();

        if ((REG32(SYS_A9DP_RESET_0) & core_id) != 0)
           break;
    }

    uart_printf ("~~~~~~~~~~~~~~~~~~~~~\r\n", nID);
    uart_printf ("Run L-ARM:core-%d ...\r\n", nID);
    uart_printf ("~~~~~~~~~~~~~~~~~~~~~\r\n", nID);

    // To wait the core initialization
    irq = ARM_INT_disable ();

	MxFlushDCache ();

    REG32(SYS_A9DP_RESET_0) &= ~core_id;

   	rc = MxICSysWaitInit (&MxStarted[nID]);
    ARM_INT_restore (irq);

    uart_printf ("%s\r\n", SUCCESSFUL (rc) ? "OK" : "FAILED");

    _ASSERT_RC(rc);

    return rc;

}

MXRC EnableUCore (UINT32 nID)
{
    IRQTYPE irq;
    UINT32 rc, core_id = (0x1 << (nID + 4));
    UINT32 addr = (UINT32)&MxStarted[nID];

    REG32(SYS_A9QP_RESET_0) |= core_id;

    if ((MX_MMU_INIT_MSG_MASK & (1 << nID)) || (MX_APP_INIT_MSG_MASK & (1 << nID)))
    {
        uart_printf ("~~~~~~~~~~~~~~~~~~~~~\r\n", nID);
        uart_printf ("Run U-ARM:core-%d ...\r\n", nID);
        uart_printf ("~~~~~~~~~~~~~~~~~~~~~\r\n", nID);
    }
    else
    {
        uart_printf ("Enable U-ARM:core-%d", nID);
        uart_printf_reset_num();
        uart_printf_set_header(":\r\n");
        uart_printf_set_align(2);
    }

    // To wait the core initialization
    irq = ARM_INT_disable ();

	MxFlushDCache ();

    REG32(SYS_A9QP_RESET_0) ^= core_id;

   	rc = MxICSysWaitInit (addr);

    uart_printf_set_header(NULL);
    uart_printf_set_align(0);
    
    if (uart_printf_get_num() == 0)
        uart_printf ("-{%s}\r\n", SUCCESSFUL (rc) ? "OK" : "FAILED");
    else
        uart_printf ("----------{U-ARM:core-%d is %s}----------\r\n", nID, SUCCESSFUL (rc) ? "OK" : "FAILED");

    ARM_INT_restore (irq);

    while (rc != 0) _NOP();

    _ASSERT_RC(rc);

    return rc;
}

void EnableCores(void)
{
    IRQTYPE irq;

    uart_printf ("----- ARM cores initialization ---------\r\n");

    WaitPrint ();

    irq = ARM_INT_disable ();

    memset ((void*)MxStarted, 0, sizeof (MxStarted));
    memset ((void*)MxCoreAllowed, 0, sizeof (MxCoreAllowed));

    MxFlushDCache ();

    if (MxGetARMID () == 0)
    {
#ifdef CPU1_ENABLED
       EnableUCore (1);
#endif

#ifdef CPU2_ENABLED
       EnableUCore (2);
#endif

#ifdef CPU3_ENABLED
       EnableUCore (3);
#endif

    MxDelayTicks (TICKS_TO_MS(2));

    // to allow the cores

#ifdef CPU1_ENABLED
    MxCoreAllowed [1] = 1;
    MxDSB ();
#endif

#ifdef CPU2_ENABLED

    MxDelayTicks (TICKS_TO_MS(2));

    MxCoreAllowed [2] = 1;
    MxDSB ();
#endif

#ifdef CPU3_ENABLED

    MxDelayTicks (TICKS_TO_MS(2));

    MxCoreAllowed [3] = 1;
    MxDSB ();
#endif

    }
    else
    {
#ifdef CPU1_ENABLED
        EnableLCore (1);
#endif

#ifdef CPU1_ENABLED

        MxDelayTicks (TICKS_TO_MS(2));

        MxCoreAllowed [1] = 1;
        MxDSB ();
#endif
    }

    MxDelayTicks (TICKS_TO_MS(2));
    uart_printf ("----------------------------------------\r\n");
    ARM_INT_restore (irq);
}

extern UINT32 CLineCmdFreq(CMgrCmdCtx * a, CMgrCmdParamCtx * b);
#ifdef PTM_ENABLED
extern MXRC  PtmDiagInit (void);
#endif
MXRC AppInitCpu0(UINT32 cpuid, MXRC (*ExtAppInit) (void))
{
    MXRC rc = MX_OK;
    //UINT32 nArmFrq;

    memset (&CpuTraceMode[MxGetCpuID ()], 0, sizeof (CpuTraceType));
    CpuTraceMode[MxGetCpuID ()].nCurMode = MLOG_MODE_IDLE;

    SysClocksSetup();

#if 0
    uart_printf("Enable DDR controller protection 0x%08x - 0x%08x\n", 0x30000, 0x0420000);
    SysDDREnableWriteProtect(0, 0, 0x30000, 0x0420000);
    rc = DDRMonDrvCtrl (DDR_MON_CTRL_ENABLED, NULL);
    uart_printf ("DDR-MONINOR is activated rc=%x\r\n", rc);
#endif

    if(((PENABLEDEVICE)&CoreCfg.EnableDevice)->cmgrCICPU)
    {		
        ConICPUInit(NULL);
        uart_printf_select_dev(PRINT_DEV_CICPU, 0, 0);
    }

    rc = ServInit (0);  // System services
    _ASSERT_RC(rc);

    rc = DiagInit ();
    _ASSERT_RC(rc);

    uart_printf ("\r\n+-----------------+-SYS-FREQUENCIES-+------------------+\r\n");
    uart_printf ("|  ARMx4: %03d Mhz ", ClkRstGetFreq (ARMx4_CORE0) / 1000000);
    uart_printf ("|  DDR-0: %03d Mhz ", SysDDRGetFreq(0, ClkRstGetFreq (CR_DDR0)) / 1000000);
    uart_printf ("| SYS-AXI: %03d Mhz |\r\n", ClkRstGetFreq (SYS_AXI) / 1000000);

    uart_printf ("|  ARMx2: %03d Mhz ", ClkRstGetFreq (ARMx2_CORE0) / 1000000);
    uart_printf ("|  DDR-1: %03d Mhz ", SysDDRGetFreq(1, ClkRstGetFreq (CR_DDR0)) / 1000000);
    uart_printf ("| SPU-AXI: %03d Mhz |\r\n", ClkRstGetFreq (SPU_AXI) / 1000000);
    
    uart_printf ("|   CEVA: %03d Mhz ", ClkRstGetFreq (CR_DEV_CEVA_0) / 1000000);
    uart_printf ("| FEC-DL: %03d Mhz ", ClkRstGetFreq (CR_DEV_FEC_DL) / 1000000);
    uart_printf ("| RAD-AXI: %03d Mhz |\r\n", ClkRstGetFreq (RAD_AXI) / 1000000);

    uart_printf ("|     FP: %03d Mhz ", ClkRstGetFreq (CR_DEV_FP_0) / 1000000);
    uart_printf ("| FEC-UL: %03d Mhz ", ClkRstGetFreq (CR_DEV_FEC_UL) / 1000000);
    uart_printf ("|  XP-AXI: %03d Mhz |\r\n", ClkRstGetFreq (XP_AXI ) / 1000000);
    uart_printf ("+-----------------+-----------------+------------------+\r\n");

    if (ExtAppInit != NULL)
    {
        uart_printf_reset_num();
        uart_printf_set_header("\r\n----------- Ext application ------------\r\n");
        uart_printf_set_align(2);
        
        if (FAILED(rc = ExtAppInit()))
        {
           uart_printf ("L-ARM application failed to start, error=0x%08x\r\n", rc);
           _ASSERT_RC(rc);
           return rc;
        }
        
        uart_printf_set_align(0);
        if (uart_printf_get_num())
            uart_printf ("----------------------------------------\r\n");
    }

    EnableCores ();

    // To store the number and size of system resources

    SvsrStoreSysObjs ();
    
    if (FAILED(rc = SvsrSendReady (rc)))
    {
        uart_printf("L-ARM SVSR - ERROR{0x%08x}\r\n", rc);
        rc = MX_OK; 
    }

    //uart_printf("----------------------------\r\n");
    uart_printf("Initializing Storage Arrays:\r\n");
    uart_printf("----------------------------\r\n");
    uart_printf("Location of gDlIqBuf:       0x%08x\r\n", gDlIqBuf);
    uart_printf("Location of gUlIqBuf:       0x%08x\r\n", gUlIqBuf);
    uart_printf("Location of gDlSduBuf:      0x%08x\r\n", gDlSduBuf);
    uart_printf("Location of gUlSduBuf:      0x%08x\r\n", gUlSduBuf);
    uart_printf("Location of gSvsrLoggerBuf: 0x%08x\r\n", gSvsrLoggerBuf);
    uart_printf("Location of gMlogBuf:       0x%08x\r\n", gMlogBuf);
    uart_printf("-----------------------------------------");

    CLUpdate ();

#if defined(TIMER_TEST) && defined(GIC_INIT)
{
    HANDLE hTestTimer;
    MxCreateThread (TestTimer, NULL, NULL, 0, &hTestTimer);
    MxScheduleThread(hTestTimer);
}
#endif

	SysTimerInitWD();
    MxStarted [0] = 1;
    return rc;
}

#if defined(TIMER_TEST) && defined(GIC_INIT)

UINT32 TestTimer (void * p)
{
    UINT32 nSec;
    UINT32 nMin;
    UINT32 nHour;

    VUINT32 nTicks   = 0xFFFFFFFF, tmp;

    uart_printf ("Begin timer tests:\r\n");
    uart_printf ("Period is %d\r\n", LOCAL_TIMER_PERIOD);

    uart_printf ("\r\n\r\n");

    while (TRUE)
    {
        if ((MxGetTime () - nTicks) >= 1000)
        {
            nTicks = MxGetTime ();

            tmp = nTicks / 1000;

            nHour = (tmp / (60*60)) % 24;
            nMin  = (tmp / (60   )) % 60;
            nSec  = (tmp          ) % 60;

            uart_printf ("%02d:%02d:%02d\r\n", nHour, nMin, nSec);
        }
    }
}

#endif //TIMER_TEST


UINT32 IdleCpu0(LPVOID p)
{
    UINT32 cmgrUART = ((PENABLEDEVICE)&CoreCfg.EnableDevice)->cmgrUART;
    if (cmgrUART == CMGR_TURNED_OFF)
        return MX_OK;
    

#ifdef CMD_LINE_INIT
    if (HaveToBeUpdated())
    {
        SetCpuTraceMode (MLOG_MODE_SUPERVISOR);

        CLUpdate();
    }
    else
    {
        //SetCpuTraceMode (MLOG_MODE_IDLE);
    }
#else
    __nop();
#endif

    return MX_OK;
}


extern __weak void App4gmxTracer(UINT32 id, UINT32 proc_address);

/** @brief This function is called by 4gmx like
           an external threads tracer

    @param id [in] - EXT_TRACER_START_THREAD - start thread
                     EXT_TRACER_STOP_THREAD  - stop thread */

void AppThrTracer (ExtTracer id)
{
    if (id == EXT_TRACER_START_THREADS)
    {
#ifdef PM_ENABLED
        PMSetARMMode (PM_ARM_MODE_SVSR);
#endif
    }
    else if (id == EXT_TRACER_START_IDLE)
    {
#ifdef PM_ENABLED
        PMSetARMMode (PM_ARM_MODE_IDLE);
#endif
    }

#ifdef APP_4GMX_TRACER_ENABLED
    {
        PTHREAD pThr;
        HANDLE hThr;
        PMXCONTEXT pCtx;

        if (id == EXT_TRACER_START_THREAD || id == EXT_TRACER_STOP_THREAD)
        {
            hThr = MxGetCurrentThread();
            _ASSERT_HANDLE(hThr);
            
            pCtx = MxInqContext();
            _ASSERT_PTR(pCtx);
            
            pThr = MxiGetThreadDescr(pCtx, hThr);
            _ASSERT_PTR(pThr);
            
            App4gmxTracer(id, (UINT32)pThr->Proc);
        }
    }

#endif // APP_4GMX_TRACER_ENABLED

#ifdef SYS_INFO_ENABLED

    if (id == EXT_TRACER_START_THREAD)
    {
        syscoreinfo [MxGetCpuID ()].hThread = MxGetCurrentThreadProc();//MxGetCurrentThread ();
    }
    else
    {
        syscoreinfo [MxGetCpuID ()].hThread = 0; //HNULL;
    }

#else
    if (id == EXT_TRACER_START_THREAD)
    {
        SetCpuTraceMode (MLOG_MODE_SUPERVISOR);
    }
#endif
}

UINT32 IsAppInitMsgEnabled (void)
{
    return (1<<MxGetCpuID()) & MX_APP_INIT_MSG_MASK;
}

/********************************************************************
*   The entry point of application initialization for CPU-1         *
*********************************************************************/

#ifdef CPU1_ENABLED

MXRC AppInitCpu1(UINT32 cpuid, MXRC (*ExtAppInit) (void))
{
    MXRC rc = MX_OK;
    UINT32 nArmFrq = 0;
    IRQTYPE irq;

    memset (&CpuTraceMode[MxGetCpuID ()], 0, sizeof (CpuTraceType));
    CpuTraceMode[MxGetCpuID ()].nCurMode = MLOG_MODE_IDLE;

    if (MxGetARMID () == 0)
    {
        nArmFrq = ClkRstGetFreq (ARMx4_CORE0) / 1000000;
        if (IsAppInitMsgEnabled())
            uart_printf ("\r\nU-ARM:core-%d freq=%dMhz\r\n", MxGetCpuID(), nArmFrq);
    }
    else
    {
        nArmFrq = ClkRstGetFreq (ARMx2_CORE0) / 1000000;
        if (IsAppInitMsgEnabled())
            uart_printf ("\r\nL-ARM:core-%d freq=%dMhz\r\n", MxGetCpuID(), nArmFrq);
    }

    // this function opens created by the core0 heaps and makes possible
    // to use the heaps on core1

    if (FAILED(rc = CreateSystemHeaps (MxGetCpuID())))
    {
        _ASSERT(0);
    }

#if defined(SVSR_ON_ARM_1_ENABLED)

    if (IsAppInitMsgEnabled())
        uart_printf ("(ARM-%d) Supervisor init... ", MxGetCpuID ());

    if (IS_ICPU())
    {
        if (IsAppInitMsgEnabled())
        {
            uart_printf ("I-CPU address is: 0x%08x\r\n", CoreCfg.ICPUBase);
            uart_printf ("[%s interface]...", "U-ARM regular I-CPU");
        }
    }
    else if (IS_EXT_ICPU())
    {
        if (IsAppInitMsgEnabled())
            uart_printf ("[%s interface]...", "U-ARM extended I-CPU");
    }
    else
    {
        if (IsAppInitMsgEnabled())
            uart_printf ("[%s interface]...", "HBI");
    }

    rc = SvsrInit();

    if (rc != MX_OK && IsAppInitMsgEnabled())
        uart_printf ("Supervisor RC = %x\n", rc);

    _ASSERT_RC(rc);

#endif

//    TmrDrvInitLocalTimer ();

	// System services
    rc = ServInit (0);
    _ASSERT_RC(rc);

    if (ExtAppInit != NULL)
    {
    	if (FAILED(rc = ExtAppInit()))
        {
            if (IsAppInitMsgEnabled())
    		    uart_printf ("L-ARM application failed to start, error=0x%08x\r\n", rc);

    		_ASSERT_RC(rc);

    		return rc;
        }
    }

    irq = ARM_INT_disable ();

    MxFlushDCache ();

    MxStarted[1] = 1;
    MxDelayTicks (TICKS_TO_MS(2));
    while (MxCoreAllowed[1] == 0)
    {
        __nop();
    }

    ARM_INT_restore (irq);

	SysTimerInitWD();

    return MX_OK;
}

UINT32 IdleCpu1(LPVOID p)
{
    //SetCpuTraceMode (MLOG_MODE_IDLE);
    return MX_OK;
}

#endif /*CPU1_ENABLED */


/********************************************************************
*   The entry point of application initialization for CPU-2         *
*********************************************************************/

#ifdef CPU2_ENABLED

MXRC AppInitCpu2(UINT32 cpuid, MXRC (*ExtAppInit) (void))
{
    MXRC rc = MX_OK;
    UINT32 nArmFrq = 0;
    IRQTYPE irq;

    memset (&CpuTraceMode[MxGetCpuID ()], 0, sizeof (CpuTraceType));
    CpuTraceMode[MxGetCpuID ()].nCurMode = MLOG_MODE_IDLE;

    if (MxGetARMID () == 0)
    {
        nArmFrq = ClkRstGetFreq (ARMx4_CORE0) / 1000000;
        
        if (IsAppInitMsgEnabled())
            uart_printf ("\r\nU-ARM:core-%d freq=%dMhz\r\n", MxGetCpuID(), nArmFrq);
    }
    else
    {
        nArmFrq = ClkRstGetFreq (ARMx2_CORE0) / 1000000;
        
        if (IsAppInitMsgEnabled())
            uart_printf ("\r\nL-ARM:core-%d freq=%dMhz\r\n", MxGetCpuID(), nArmFrq);
    }

    // this function opens created by the core0 heaps and makes possible
    // to use the heaps on core2

    if (FAILED(rc = CreateSystemHeaps (MxGetCpuID())))
    {
        _ASSERT(0);
    }

#if defined(SVSR_ON_ARM_2_ENABLED)

    if (IsAppInitMsgEnabled())
        uart_printf ("(ARM-%d) Supervisor init... ", MxGetCpuID ());

    if (IS_ICPU())
    {
        if (IsAppInitMsgEnabled())
        {
            uart_printf ("I-CPU address is: 0x%08x\r\n", CoreCfg.ICPUBase);
            uart_printf ("[%s interface]...", "U-ARM regular I-CPU");
        }
    }
    else if (IS_EXT_ICPU())
    {
        if (IsAppInitMsgEnabled())
            uart_printf ("[%s interface]...", "U-ARM extended I-CPU");
    }
    else
    {
        if (IsAppInitMsgEnabled())
            uart_printf ("[%s interface]...", "HBI");
    }

    rc = SvsrInit();

    if (rc != MX_OK && IsAppInitMsgEnabled())
        uart_printf ("Supervisor RC = %x\n", rc);

    _ASSERT_RC(rc);

#endif

//    TmrDrvInitLocalTimer ();

    // System services
    rc = ServInit (0);
    _ASSERT_RC(rc);

    if (ExtAppInit != NULL)
    {
    	if (FAILED(rc = ExtAppInit()))
        {
            if (IsAppInitMsgEnabled())
    		    uart_printf ("L-ARM application failed to start, error=0x%08x\r\n", rc);

    		_ASSERT_RC(rc);

    		return rc;
        }
    }

   	irq = ARM_INT_disable ();
   	MxFlushDCache ();

    MxStarted[2] = 1;
    MxDelayTicks (TICKS_TO_MS(2));
    while (MxCoreAllowed[2] == 0)
    {
        __nop();
    }

    ARM_INT_restore (irq);

	SysTimerInitWD();

     return MX_OK;
}

UINT32 IdleCpu2(LPVOID p)
{
    //SetCpuTraceMode (MLOG_MODE_IDLE);
    return MX_OK;
}

#endif /*CPU2_ENABLED */

/********************************************************************
*   The entry point of application initialization for CPU-3         *
*********************************************************************/

#ifdef CPU3_ENABLED

MXRC AppInitCpu3(UINT32 cpuid, MXRC (*ExtAppInit) (void))
{
    MXRC rc = MX_OK;
    UINT32 nArmFrq = 0;
    IRQTYPE irq;

    memset (&CpuTraceMode[MxGetCpuID ()], 0, sizeof (CpuTraceType));
    CpuTraceMode[MxGetCpuID ()].nCurMode = MLOG_MODE_IDLE;

    if (MxGetARMID () == 0)
    {
        nArmFrq = ClkRstGetFreq (ARMx4_CORE0) / 1000000;

        if (IsAppInitMsgEnabled())
            uart_printf ("\r\nU-ARM:core-%d freq=%dMhz\r\n", MxGetCpuID(), nArmFrq);
    }
    else
    {
        nArmFrq = ClkRstGetFreq (ARMx2_CORE0) / 1000000;

        if (IsAppInitMsgEnabled())
            uart_printf ("\r\nL-ARM:core-%d freq=%dMhz\r\n", MxGetCpuID(), nArmFrq);
    }

    // this function opens created by the core0 heaps and makes possible
    // to use the heaps on core2

    if (FAILED(rc = CreateSystemHeaps (MxGetCpuID())))
    {
        _ASSERT(0);
    }

#if defined(SVSR_ON_ARM_3_ENABLED)

    if (IsAppInitMsgEnabled())
        uart_printf ("(ARM-%d) Supervisor init... ", MxGetCpuID ());

    if (IS_ICPU())
    {
        if (IsAppInitMsgEnabled())
        {
            uart_printf ("I-CPU address is: 0x%08x\r\n", CoreCfg.ICPUBase);
            uart_printf ("[%s interface]...", "U-ARM regular I-CPU");
        }
    }
    else if (IS_EXT_ICPU())
    {
        if (IsAppInitMsgEnabled())
            uart_printf ("[%s interface]...", "U-ARM extended I-CPU");
    }
    else
    {
        if (IsAppInitMsgEnabled())
            uart_printf ("[%s interface]...", "HBI");
    }

    rc = SvsrInit();

    if (rc != MX_OK && IsAppInitMsgEnabled())
        uart_printf ("Supervisor RC = %x\n", rc);

    _ASSERT_RC(rc);

#endif

//    TmrDrvInitLocalTimer ();

    // System services
    rc = ServInit (0);
    _ASSERT_RC(rc);

    if (ExtAppInit != NULL)
    {
    	if (FAILED(rc = ExtAppInit()))
        {
            if (IsAppInitMsgEnabled())
    		    uart_printf ("L-ARM application failed to start, error=0x%08x\r\n", rc);

    		_ASSERT_RC(rc);

    		return rc;
        }
    }

   	irq = ARM_INT_disable ();
   	MxFlushDCache ();

    MxStarted[3] = 1;
    MxDelayTicks (TICKS_TO_MS(2));
    while (MxCoreAllowed[3] == 0)
    {
        __nop();
    }

    ARM_INT_restore (irq);

	SysTimerInitWD();

     return MX_OK;
}

UINT32 IdleCpu3(LPVOID p)
{
    //SetCpuTraceMode (MLOG_MODE_IDLE);
    return MX_OK;
}

#endif /*CPU3_ENABLED */

