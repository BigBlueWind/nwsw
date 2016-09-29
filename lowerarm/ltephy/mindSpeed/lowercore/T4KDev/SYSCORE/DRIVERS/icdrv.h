//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL icdrv.c
 *
 * @brief The types and macros definition of GIC driver
 * @author Mindspeed Technologies
 * @version $Revision: 1.21 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _IC_DRV_H_
#define _IC_DRV_H_

#include "config.h"
#include "4gmx.h"
#include "appids.h"
#include "hal.h"
#include "sirqassign.h"
#include "appprintf.h"

#define RC_INTS_DRV_ALLOC_ERROR         DEF_USRRC(APP_DRIVER, INTC_DRV, 1)
#define RC_INTS_DRV_UNSUPP_CMD          DEF_USRRC(APP_DRIVER, INTC_DRV, 2)
#define RC_INTS_DRV_INIT_ERR            DEF_USRRC(APP_DRIVER, INTC_DRV, 3)
#define RC_INTS_DRV_SET_PRIOR_ERR       DEF_USRRC(APP_DRIVER, INTC_DRV, 4)
#define RC_INTS_DRV_RESET_PRIOR_ERR     DEF_USRRC(APP_DRIVER, INTC_DRV, 5)
#define RC_INTS_SAFE_READ_FAILED        DEF_USRRC(APP_DRIVER, INTC_DRV, 6)
#define RC_INTS_SAFE_WRITE_FAILED       DEF_USRRC(APP_DRIVER, INTC_DRV, 7)
#define RC_INTS_TRACE_IS_DISABLED       DEF_USRRC(APP_DRIVER, INTC_DRV, 8)
#define RC_INTS_ENUM_STOPPED            DEF_USRRC(APP_DRIVER, INTC_DRV, 9)

//#define GIC_TRACE_ENABLED
#define GIC_TRACE_SIZE                  (1024*20)
#define ICDRV_INT_STAT_ENABLED

#define GIC_CPU_BASE_ADDR               0xfff00100
#define GIC_DISTR_BASE_ADDR             0xfff01000

#define INTC_FAKE_IRQ_ID                (0x3FF)
#define INTC_FAKE_IRQ_MASK              (0x3FF)

#define CUR_CPU_ID                      (GicGetLocalCpuID ())

#define GIC_IRQ_TYPE                    (0)
#define GIC_FIQ_TYPE                    ((UINT32)1 << 31)

#define IC_ENABLE_INT_PROCESSING
#define GLOBAL_INTER_CPU_INTERRUPTS
//#define ICDRV_FIQ_ENABLED
/*****************************************************************************
*           The bits mask for the "CtrlMask" of IC drv context               *
******************************************************************************/

#define IC_CTRL_MASK_CHECK_ADDR         (1 << 0)
#define IC_CTRL_MASK_CHECK_ADDR_FAILED  (1 << 1)

// These macros will be redefined to use real function in case of
// usage the next one core

#if 1

#define GIC_DRV_GET_LOCK()	            GicGetLock()
#define GIC_DRV_RELEASE_LOCK()          GicReleaseLock()

#else

#define GIC_DRV_GET_LOCK()
#define GIC_DRV_RELEASE_LOCK()

#endif

#ifdef PROC_ABORTS_ENABLED
#define MXEHT_
#else
#define MXEHT_ MXEHT
#endif

/*****************************************************************************
*                    The driver type definitions                             *
******************************************************************************/

typedef struct _GIT_IRQ_TRACE_TYPE_
{
    UINT32 IrqID;       // The IRQ ID
    UINT32 StartTicks;
    UINT32 StopTicks;

}GIT_IRQ_TRACE_TYPE;

/** @brief This type of data describes interrupt handler enanle in the system
           please see: ICDrvEnumInt () function */

typedef struct _ICDRV_INT_INFO_
{
    UINT32 nIntID;          // The INT id
    UINT32 nIntPr;          // The INT priority
    UINT16 IntType;         // 1- IRQ, 0 - FIQ
    UINT16 IntMode;         // Level/edge
    UINT32 CpuMask;         // The bitmask of CPU cores (route)
    UINT32 Handler;         // The address of INT handler 
    UINT32 HandlerPtr;      // The handler parameter 
    UINT32 Statistics;      // The statistics how many times this handler was processed (if macro ICDRV_INT_STAT_ENABLED enabled)

}ICDRV_INT_INFO;

typedef struct _GIC_CPU_REGS_
{
    VUINT32 Ctrl;      /**< CPU Interface Control Register (ICCICR) */
    VUINT32 Mask;      /**< Interrupt Priority Mask Register (ICCPMR) */
    VUINT32 BinPoint;  /**< Binary Point Register (ICCBPR)    */
    VUINT32 IntAck;    /**< Interrupt Acknowledge Register (ICCIAR) */
    VUINT32 IntEnd;    /**< End of Interrupt Register (ICCEOIR)  */
    VUINT32 RunPrior;  /**< Running Priority Register (ICCRPR)   */
    VUINT32 HighPend;  /**< Highest Pending Interrupt Register (ICCHPIR)*/

} GicCpuRegs;

typedef struct _GIC_DIST_REGS_
{
    VUINT32 Ctrl;          /**< Distributor Control Register (ICDDCR)      */
    VUINT32 Type;          /**< Interrupt Controller Type Register (ICDICTR)    */
    VUINT32 DistID;        /**< Distributor Implementer Identification Register (ICDIIDR) */
    VUINT32 Res[29];       /**< Reserved             */
    VUINT32 Secur[0x20];   /**< Interrupt Security Registers (ICDISRn)      */
    //VUINT32 Res2[0x1F];    /**< Reserved             */
    VUINT32 SetEnable[32]; /**< Interrupt Set-Enable Registers (ICDISERn)     */
    VUINT32 ClrEnable[32]; /**< Interrupt Clear-Enable Registers (ICDICERn)    */
    VUINT32 SetPend[32];   /**< Interrupt Set-Pending Registers (ICDISPRn)     */
    VUINT32 ClrPend[32];   /**< Interrupt Clear-Pending Registers (ICDICPRn)    */
    VUINT32 ActBit[32];    /**< Active Bit Registers (ICDABRn)        */
    VUINT32 Res3[32];      /**< Reserved             */
    VUINT32 Prior[255];    /**< Interrupt Priority Registers (ICDIPRn)      */
    VUINT32 Res4;          /**< Reserved             */
    VUINT32 Target[255];   /**< Interrupt Processor Targets Registers (ICDIPTRn)   */
    VUINT32 Res5;          /**< Reserved             */
    VUINT32 Config[64];    /**< Interrupt Configuration Registers (ICDICFRn)    */
    VUINT32 PPIStatus;     /**< PPI Status Register */
    VUINT32 SPIStatus[31]; /**< SPI Status Register */
    VUINT32 Res6[32];      /**< IMPLEMENTATION DEFINED registers       */
    VUINT32 Res7[64];      /**< Reserved             */
    VUINT32 SoftGenInt;    /**< Software Generated Interrupt Register (ICDSGIR)   */
    VUINT32 Res8[51];      /**< Reserved             */
    VUINT32 ID[12];        /**< Identification registers         */

} GicDistRegs;

typedef struct _INTC_CTX_
{
    GicDistRegs *Distr;
    GicCpuRegs *Cpu;

    UINT32 nInitOpt;
    PMXCONTEXT pOSCtx;

    VUINT32 CtrlMask;   // See: IC_CTRL_MASK_xxx

} INTCCTX, *PINTCCTX;

MXRC INLINE ICDrvRaiseInterCpu (UINT32 nCpu, UINT32 nIRQ)
{
	MXRC rc = MX_OK;
    GicDistRegs* pDist = (GicDistRegs*)GIC_DISTR_BASE_ADDR;
    UINT32 secure_flag;

	if (nIRQ >= HAL_IRQ_INTER_ARM_0)
    {
		// for 4x2 only this code in this if will be used
        REG32 (CFG_SYS_ARM_IRQ_SET) = 1 << (nIRQ - HAL_IRQ_INTER_ARM_0);
    }
    else
    {
        #ifdef ICDRV_FIQ_ENABLED
            // FIQ is supported on U-ARM only
            if (MxGetARMID () == 0)
                secure_flag = (nCpu & GIC_FIQ_TYPE) ? (0) : (1 << 15);
            else
                secure_flag = 0;
        #else
            secure_flag = 0;
        #endif
        
        nCpu &= 0xFF;
    
        if (nIRQ != SOFT_ICPU_TO_UARM)
        {
            // to convert logical core id to physical core id
			rc = MxGetRemoteCoreID (nCpu, &nCpu);

            // to keep core id only, not cluster ID
			nCpu &= 0xF;

            _ASSERT_RC(rc);
        }

        // Send interrupt to the CPUs specified in the target list
		pDist->SoftGenInt = ((((1 << nCpu) & 0xFF) << 16) | (nIRQ)) | secure_flag;
    }

	return rc;
}

MXRC INLINE ICDrvRaiseInterCpuByMask (UINT32 nCpuMask, UINT32 nIRQ)
{
    GicDistRegs* pDist = (GicDistRegs*)GIC_DISTR_BASE_ADDR;
    UINT32 secure_flag;

    // to read secure/non-secure flag for STI IRQs
    #ifdef ICDRV_FIQ_ENABLED
        if (MxGetARMID () == 0)
    secure_flag = (nCpuMask & GIC_FIQ_TYPE)?(0):(1<<15);
        else
            secure_flag = 0;
    #else
        secure_flag = 0;
    #endif

    // Send interrupt to the CPUs specified in the target list
    pDist->SoftGenInt = ((((nCpuMask) & 0xFF) << 16) | (nIRQ)) | secure_flag;
    
	return MX_OK;
}


MXRC INLINE ICDrvCleanInterCpu (UINT32 nIRQ)
{
	if (nIRQ >= HAL_IRQ_INTER_ARM_0)
    {
        REG32 (CFG_SYS_ARM_IRQ_CLR) = (1 << (nIRQ - HAL_IRQ_INTER_ARM_0));
    }

	return MX_OK;
}

/** @brief This function returns the numbre of elements in the IRQ trace

    @param nCpu [in] - the CPU id (0..3)

    @return [UINT32] the number of elements */

UINT32 ICDrvGetTraceSize (UINT32 nCpu);

/** @brief This function returns a pointer to the trace elements for some processor

    @param nCpu  [in] - the CPU id (0..3)
    @param nIndex[in] - the element index, see ICDrvGetTraceSize function

    @return [GIT_IRQ_TRACE_TYPE *] the pointer to the trace element or NULL*/
    
GIT_IRQ_TRACE_TYPE * ICDrvGetTraceElm(UINT32 nCpu, UINT32 nIndex);

/** @brief This function enabled IRQ trace, please also see GIC_TRACE_ENABLED macro

    @param bEnb [in] - the flag to enable/disable IRQ trace 

    @return [MXRC] an error code */
    
MXRC ICDrvEnableIrqTrace (UINT32 bEnb);

UINT32 INLINE ICDrvGetPendStatus (UINT32 nRegIndex)
{
	return ((GicDistRegs*)GIC_DISTR_BASE_ADDR)->SetPend [nRegIndex];
}

/** @brief The function checks is INT enabled / disabled
    @param nID [in] - the INT id 
    @return [BOOL] enable/disable status */

BOOL ICDrvIsIntEnabled(UINT32 nID);

/** @brief This function enumerates enable in the system interrupts 
    @param pEnumId [in/out] - the pointer to enumerator, by initially 0
    @param pInfo   [out]    - the pointer to information structure
    @return [MXRC] an error code */
    
MXRC ICDrvEnumInt (UINT32 * pEnumId, ICDRV_INT_INFO * pInfo);

MXRC ICDrvClearIrq     (UINT32 nID);
MXRC ICDrvClearPendIrq (UINT32 nID);
BOOL ICDrvIsIrq        (UINT32 nID);

void ICDrvShutdown (void);

MXRC ICDrv_init(UINT32 nInit, LPVOID * pDrvCtx);

MXRC ICDrv_config(LPVOID pDrvCtx, UINT32 nCfg, LPVOID pCfgData,
                  UINT32 nCfgDataSize);

MXRC ICDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                 UINT32 nCmdDataSize);

MXRC ICDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                UINT32 * pnCmdDataSize);

#endif /*_INTS_DRV_H_*/

#ifdef __cplusplus
}
#endif

