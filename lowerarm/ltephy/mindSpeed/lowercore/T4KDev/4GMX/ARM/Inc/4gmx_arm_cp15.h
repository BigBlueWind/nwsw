//-------------------------------------------------------------------------------------------
/** @file 4gmx_arm_cp15.h
 *
 * @brief ARM dependent API definition
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_ARM_CP15_H_
#define _4GMX_ARM_CP15_H_

#include "4gmx_config.h"
#include "4gmx_types.h"
#include "4gmx_const.h"

#ifdef __CC_ARM

#else /* __CC_ARM */

#define CP15_RD_API INLINE unsigned long
#define CP15_WR_API INLINE void

#define CP15_GET(Opcode1,CRn,CRm,Opcode2) \
    register unsigned int value; \
    asm volatile ("mrc p15," #Opcode1 ", %0," #CRn ", " #CRm ", " #Opcode2 "" : "=r" (value)); \
    return value;

#define CP15_PUT(Opcode1,value,CRn,CRm,Opcode2) \
    asm volatile ("mcr p15," #Opcode1 ",%0," #CRn "," #CRm "," #Opcode2 "" : : "r" (value));

#define CP15_PUT2(Opcode1,end_addr,start_addr,CRn) \
    asm volatile ("mcrr p15," #Opcode1 ", %0, %1," #CRn "" :: "r" (end_addr), "r" (start_addr));

#define CP15_SBZ(Opcode1,CRn,CRm,Opcode2) \
    register unsigned int SBZ = 0; \
    asm volatile ("mcr p15," #Opcode1 ",%0," #CRn "," #CRm "," #Opcode2"" : : "r" (SBZ));

#endif /* __CC_ARM */

/****************************************
 *  Control Register Bit Fields         *
 ****************************************/
#define CR_AFE    (1 << 29)    /**< Access Flag Enable bit*/
#define CR_V      0x00002000   /**<Location of exception vectors: 0 = Normal exception vectors selected, address range = 0x00000000-0x0000001C*/
#define CR_I      0x00001000   /**<Level one Instruction Cache enable/disable*/
#define CR_Z      0x00000800   /**<Program flow prediction*/
#define CR_C      0x00000004   /**<Level one Data Cache enable/disable*/
#define CR_A      0x00000002   /**<Strict data address alignment fault enable/disable*/
#define CR_M      0x00000001   /**<MMU enabled */

/******************************************
 *  Auxiliary Control Register Bit fields *
 ******************************************/

#define ACR_CZ    0x00000040
#define ACR_RV    0x00000020
#define ACR_RA    0x00000010
#define ACR_TR    0x00000008
#define ACR_SB    0x00000004
#define ACR_DB    0x00000002
#define ACR_RS    0x00000001

/************************************************************************
 *    System control and configuraion
 ************************************************************************/

void    sys_write_control(UINT32 val);
UINT32  sys_read_control(void);

void    MxSetAuxCtrl(UINT32 val);
UINT32  MxGetAuxCtrl(void);

#ifdef MX_ARM_MPCORE_ENABLED
UINT32 sys_read_coreid(void);
#else

#endif /*#ifdef MX_ARM_MPCORE_ENABLED */

/************************************************************************
 *    MMU control and configuration
 ************************************************************************/

void    mmu_write_ttb0(UINT32 val);
UINT32  mmu_read_ttb0(void);
void    mmu_write_domain_access(UINT32 val);
UINT32  mmu_read_domain_access(void);
UINT32  mmu_read_fault_address(void);

#endif /*_4GMX_ARM_CP15_H_*/
