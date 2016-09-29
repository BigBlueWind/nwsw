//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Sirqassign.h
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.13 $
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

#ifndef __SOFT_IRQ_ASSIGNMENT__
#define __SOFT_IRQ_ASSIGNMENT__

#include "hal.h"

//HAL_IRQ_INTER_ARM_3 

#define SOFT_ICPU_4GMX_ICPU		((MxGetARMID () == 0) ? 1 : (MxGetCpuID () ? HAL_IRQ_INTER_ARM_1 : HAL_IRQ_INTER_ARM_0))	// 4GMX inter-cpu fnctionality


// -----------------------------------------------------------------------------
// <HAL_IRQ_INTER_ARM_2> can be used by: 
//    - REX scheduler if REX_INJECT_CALL_INSTEAD_OF_IRQ is not defined in rex.h
//    - Con-ICPU service (to communicate to L-ARM through Linux by using telnet)
// NOTE!
//   Only one service can use this IRQ at the same time
//   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define SOFT_ICPU_SCHEDULER     ((MxGetARMID () == 0) ? 2 : HAL_IRQ_INTER_ARM_2)

#define CICPU_TO_UARM_IRQ       (HAL_IRQ_INTER_ARM_2)
#define CICPU_FROM_UARM_IRQ     (HAL_IRQ_INTER_ARM_3)

// -----------------------------------------------------------------------------

#define SOFT_ICPU_SYS_SHDWN		(HAL_IRQ_INTER_ARM_4)
#define SOFT_ICPU_SYS_SHDWN_EX  ((MxGetARMID () == 0) ? 4 : HAL_IRQ_INTER_ARM_4)
#define SOFT_ICPU_RDMEM         (HAL_IRQ_INTER_ARM_5)   // I-CPU from U-ARM RdMem
#define SOFT_ICPU_FROM_UARM		(HAL_IRQ_INTER_ARM_6)	// I-CPU from U-ARM message
#define SOFT_ICPU_TO_UARM		(HAL_IRQ_INTER_ARM_7)	// I-CPU to U-ARM message
#define SOFT_ICPU_PM_SYNC       (8)                     // Performance monitor synchronization
#endif //__SOFT_IRQ_ASSIGNMENT__

#ifdef __cplusplus
}
#endif

