//-------------------------------------------------------------------------------------------
/** @file 4gmx_arm_types.h
 *
 * @brief ARM dependent types definition
 * @author Mindspeed Technologies
 * @version $Revision: 1.5 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "4gmx_config.h"

#ifdef MX_ARM_ENABLED

#ifndef _4GMX_ARM_TYPES_H_
#define _4GMX_ARM_TYPES_H_

/************************************************************************/
/*      Types definition                                                */
/************************************************************************/

typedef enum _CPUMODE_
{
    CPU_MODE_USER       = 0x10,
    CPU_MODE_FIQ        = 0x11,
    CPU_MODE_IRQ        = 0x12,
    CPU_MODE_SUPERVISOR = 0x13,
    CPU_MODE_ABOART     = 0x17,
    CPU_MODE_UNDEF      = 0x1B,
    CPU_MODE_SYSTEM     = 0x1F,

    CPU_MODE_MASK       = 0x1F
} CPUMODE;

typedef enum _CPUMODEID_
{
    CPU_MODE_ID_USER       = 0x0,
    CPU_MODE_ID_FIQ        = 0x1,
    CPU_MODE_ID_IRQ        = 0x2,
    CPU_MODE_ID_SUPERVISOR = 0x3,
    CPU_MODE_ID_ABOART     = 0x4,
    CPU_MODE_ID_UNDEF      = 0x5,
    CPU_MODE_ID_SYSTEM     = 0x6,

    CPU_MODE_ID_MASK       = 0xF
} CPUMODEID;


#define MX_ARM_CPUMODE_NUM  7   // The number of ARM modes (see CPUMODE type)

typedef enum _ITYPE_
{
    INT_TYPE_IRQ    = 0x80,
    INT_TYPE_FIQ    = 0x40,
    INT_TYPE_EDGE   = 0x01,
    INT_TYPE_LEVEL  = 0x00,
    INT_TYPE_NN     = 0x00,
    INT_TYPE_1N     = 0x02,
} ITYPE;

typedef enum _ETYPE_
{
    EXCEPTION_TYPE_RESET    = 0x00,
    EXCEPTION_TYPE_UNDEF    = 0x04,
    EXCEPTION_TYPE_SWI      = 0x08,
    EXCEPTION_TYPE_PREFETCH = 0x0C,
    EXCEPTION_TYPE_DATA     = 0x10,
    EXCEPTION_TYPE_RESERVED = 0x14,
    EXCEPTION_TYPE_IRQ      = 0x18,
    EXCEPTION_TYPE_FIQ      = 0x1C,

    EXCEPTION_TYPE_NUM      = 8
} ETYPE;

typedef enum _ARM_INT_CTRL_
{
    INT_CTRL_IRQ_ENABLED    = (1 << 0),
    INT_CTRL_IRQ_DISABLED   = (1 << 1),
    INT_CTRL_FIQ_ENABLED    = (1 << 2),
    INT_CTRL_FIQ_DISABLED   = (1 << 3)
} ARMINTCTRL;

typedef void (*TISRENTRY)(volatile UINT32 * pAckReg, UINT32 nIrqID, LPVOID pParam);

// MXEHT = Mx exception handler type 

typedef MXEHT void (*EXPENTRY)(void);

typedef struct _INTVECTOR_
{
    TISRENTRY Isr;
    LPVOID Param;
    UINT32 Opt;
    UINT32 Stat;

} INTVECTOR, *PINTVECTOR;

typedef struct _ARM_SCU_REG_MAP_
{
    VUINT32     Ctrl;       // SCU Control Register
    VUINT32     Cfg;        // SCU Configuration Register
    VUINT32     Power;      // SCU CPU Power Status Register
    VUINT32     CpuInvCtrl; // SCU Invalidate All Registers in Secure State
    VUINT32     Res1 [12];

    // 0x40 offset 
    VUINT32     FilderStart;// Filtering Start Address Register
    VUINT32     FilderEnd;  // Filtering End Address Register
    VUINT32     Res2 [2];

    // 0x50 offset 
    VUINT32     SAC;        // SCU Access Control (SAC) Register
    VUINT32     SNSAC;      // SCU Non-secure Access Control (SNSAC) Register

}ARMSCUMAP;

typedef struct _ARMCPUCONTEXT_
{
    UINT32      r[16];
    UINT32      cpsr;
    UINT32      spsr;

}ARMCPUCONTEXT, *PARMCPUCONTEXT;

#endif /*_4GMX_ARM_TYPES_H_*/

#endif /*MX_ARM_ENABLED */
