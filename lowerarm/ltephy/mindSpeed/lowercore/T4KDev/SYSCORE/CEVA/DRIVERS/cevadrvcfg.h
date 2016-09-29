//-------------------------------------------------------------------------------------------
/** @file cevadrvcfg.h
 *
 * @brief CEVA drivers configuration
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _CEVA_DRV_CFG_H_
#define _CEVA_DRV_CFG_H_

#define CEVA_GET_DRV_CFG()              ((PCEVA_DRV_CFG)(0xFC000000 + 22*4))

#define CEVA_HW_SEMA_BASE               0xFBF00000
#define DEF_CEVA_HW_SEMA(id)            (CEVA_HW_SEMA_BASE + (id * 4))

#define CEVA_DRV_HW_SEMA_SYS_MDMA       DEF_CEVA_HW_SEMA(10)
#define CEVA_DRV_HW_SEMA_SPU_MDMA       DEF_CEVA_HW_SEMA(11)
#define CEVA_DRV_HW_SEMA_RAD_MDMA       DEF_CEVA_HW_SEMA(12)
#define CEVA_DRV_HW_SEMA_FEC_DL         DEF_CEVA_HW_SEMA(13)
#define CEVA_DRV_HW_SEMA_FEC_UL         DEF_CEVA_HW_SEMA(14)
#define CEVA_DRV_HW_SEMA_LARM_UART      DEF_CEVA_HW_SEMA(15)

typedef struct _CEVA_MDMA_DRV_CFG_
{
    UINT32  OwnerCpu;   // If '1' the ARM is not involved in MDMA usage
	UINT32	HwSema;		// The address of HW semaphore (or NULL) to implement synchronization
	LPVOID	PoolPtr;	// The pointer to the POOL storage initializaed by master (ARM processor is master)
	LPVOID  ListIntPtr; // The pointer to the MDMA active/inactive lists provided by ARM driver 
    UINT32  UsedMask;   // The mask of CEVA processors where this driver was opened 

}CEVA_MDMA_DRV_CFG;

typedef struct _CEVA_FEC_DRV_CFG_
{
    UINT32  OwnerCpu;   // If '1' the ARM is not involved in FEC usage
	UINT32	HwSema;		// The address of HW semaphore (or NULL) to implement synchronization
	LPVOID	PoolPtr;	// The pointer to the POOL storage initializaed by master (ARM processor is master)
	LPVOID  ListIntPtr; // The pointer to the MDMA active/inactive lists provided by ARM driver 
	UINT32  UsedMask;   // The mask of CEVA processors where this driver was opened 

}CEVA_FEC_DRV_CFG;

typedef struct _CEVA_UART_DRV_CFG_
{
	VUINT32	HwSema;		// The address of HW semaphore (or NULL) to implement synchronization
    VUINT32	UartAddr;   // The base address of the UART HW provided by console manager 
    
}CEVA_UART_DRV_CFG;

typedef struct _CEVA_FP_DRV_CFG_
{
    UINT32      IdftRunMask;    // The mask used to provide ARM FP driver with FPs were used to run IDFT
    UINT16      IdftStart;      // The index of first FP IDFT processor
    UINT16      IdftStop;       // The index of last FP IDFT processor
    UINT32      FpDevNum;

}CEVA_FP_DRV_CFG;

/** @brief The CEVA drivers configuration table, this table is configured 
	   by ARMdrivers and delivered by the CEVA driver to provide parameters to CEVA code */

typedef struct _CEVA_DRV_CFG_
{
	CEVA_MDMA_DRV_CFG	SysMdmaCfg;
	CEVA_MDMA_DRV_CFG	SpuMdmaCfg;
	CEVA_MDMA_DRV_CFG	RadMdmaCfg;
	CEVA_FEC_DRV_CFG 	FecDLCfg;
	CEVA_FEC_DRV_CFG 	FecULCfg;
    CEVA_UART_DRV_CFG   UartCfg;
    CEVA_FP_DRV_CFG     FpCfg;
    
}CEVA_DRV_CFG, *PCEVA_DRV_CFG;

#endif //_CEVA_DRV_CFG_H_

#ifdef __cplusplus
}
#endif

