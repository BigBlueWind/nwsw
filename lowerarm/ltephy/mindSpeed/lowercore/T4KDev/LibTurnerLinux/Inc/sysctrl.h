/** @file sysctrl.h
 *
 * @brief System Control Coprocessor
 * @author Mindspeed Technologies
 * @version $Revision: 1.20 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef SYSCTRL_H_
#define SYSCTRL_H_

#include "basetypes.h"

//
// ARM Definitions
//
#ifndef UARM
#define UARM                    0
#define LARM                    1
#endif /*UARM*/

//
//  Main ID Register Bit Fields
//
#define ID_VENDOR(x)            (((x) >> 24) & 0x000000FF)
#define ID_VENDOR_ARM           0x00000041
#define ID_VENDOR_DEC           0x00000044
#define ID_VENDOR_INTEL         0x00000069
#define ID_VAR_NUM(x)           (((x) >> 20) & 0x0000000F)
#define ID_ARCH(x)              (((x) >> 16) & 0x0000000F)
#define ID_ARCH_4               0x00000001
#define ID_ARCH_4T              0x00000002
#define ID_ARCH_5               0x00000003
#define ID_ARCH_5T              0x00000004
#define ID_ARCH_5TE             0x00000005
#define ID_ARCH_6               0x00000007
#define ID_PART_NUM(x)          (((x) >> 4)  & 0x00000FFF)
#define ID_PART_ARM920T         0x00000920
#define ID_PART_ARM1136JS       0x00000B36
#define ID_REVISION(x)          (((x) >> 0)  & 0x0000000F)

//
//  Cache Type Register Bit Fields
//
#define CT_CTYPE(x)             (((x) >> 25) & 0x0000000F)
#define CT_CTYPE_WT             0x00000000
#define CT_CTYPE_WB             0x00000001
#define CT_CTYPE_WB_1           0x00000002
#define CT_CTYPE_FMT_A          0x00000006
#define CT_CTYPE_FMT_B          0x00000007
#define CT_CTYPE_FMT_C          0x0000000E
#define CT_S                    0x01000000
#define CT_DSIZE(x)             (((x) >> 12) & 0x00000FFF)
#define CT_ISIZE(x)             (((x) >> 0)  & 0x00000FFF)
#define CT_CACHE_LINE(s)        (1 << (3 + ((x) & 0x3)))
#define CT_CACHE_M(x)           (2 + ((x) >> 2) & 0x1))
#define CT_CACHE_ASSOC(x)       (CT_CACHE_M(x) << ((((x) >> 3) & 0x7)-1)))
#define CT_CACHE_SIZE(x)        (CT_CACHE_M(x) << ((((x) >> 6) & 0x7)+8)))

//
//  Translation Table Base Register Bit Fields
//
#define TTB_BASE_MASK           0xFFFFC000
#define TTB_BASE_SHIFT          14
#define TTB_RGN_MASK            0x00000018
#define TTB_RGN_NO_CACHE        (0 << 3)
#define TTB_RGN_CACHE_WB_WA     (1 << 3)
#define TTB_RGN_CACHE_WT        (2 << 3)
#define TTB_RGN_CACHE_WB        (3 << 3)
#define TTB_P                   0x00000004
#define TTB_S                   0x00000002
#define TTB_C                   0x00000001

//
//  Translation Table Base Control Register Bit Fields
//
#define TTBCR_TTB0_16K          0x00000000
#define TTBCR_TTB0_8K           0x00000001
#define TTBCR_TTB0_4K           0x00000002
#define TTBCR_TTB0_2K           0x00000003
#define TTBCR_TTB0_1K           0x00000004
#define TTBCR_TTB0_512          0x00000005
#define TTBCR_TTB0_256          0x00000006
#define TTBCR_TTB0_128          0x00000007

//
//  Domain Access Control Register Bit Fields
//
#define DACR_Dx_SHIFT(x)        ((x) << 1)
#define DACR_Dx_MASK(x)         (0x3 << DACR_Dx_SHIFT(x))
#define DACR_NO_ACCESS          0x00000000
#define DACR_CLIENT             0x00000001
#define DACR_MANAGER            0x00000003

//
//  TLB Type Register
//
#define TLBTYPE_ILSIZE(x)       (((x) >> 16) & 0x000000FF)
#define TLBTYPE_DLSIZE(x)       (((x) >> 8)  & 0x000000FF)
#define TLBTYPE_U               0x00000001

//
//  Control Register Bit Fields
//
#define CR_ASYNC                0xC0000000      // 0
#define CR_SYNC                 0x40000000      // 0
#define CR_AFE                  0x20000000      // 1 == Gener of AF faults by AP[0] is ena
#define CR_TRE                  0x10000000      // 1 == TEX remap ena
#define CR_EE                   0x02000000      // 1 == CPSR E bit set to 1
#define CR_VE                   0x01000000      // 1 == Intr vectors defined in VIC
#define CR_XP                   0x00800000      // 1 == Subpage AP bits disa
#define CR_U                    0x00400000      // 1 == Unaligned data access ena
#define CR_FI                   0x00200000      // 1 == Low intr latency ena
#define CR_IT                   0x00040000      // Redundant in ARMv6
#define CR_DT                   0x00010000      // Redundant in ARMv6
#define CR_L4                   0x00008000      // 1 == Do not set T bit on PC load
#define CR_RR                   0x00004000      // 1 == Round robin I/D replacement
#define CR_V                    0x00002000      // 1 == High exception vectors
#define CR_I                    0x00001000      // 1 == L1 instr cache ena
#define CR_Z                    0x00000800      // 1 == Program flow prediction ena
#define CR_F                    0x00000400      // 0
#define CR_R                    0x00000200      // 1 == ROM protection ena
#define CR_S                    0x00000100      // 1 == MMU system protection ena
#define CR_B                    0x00000080      // 1 == Big-endian
#define CR_D                    0x00000020      // 1
#define CR_P                    0x00000010      // 1
#define CR_W                    0x00000008      // Not used in ARM1136
#define CR_C                    0x00000004      // 1 == L1 data cache ena
#define CR_A                    0x00000002      // 1 == Strict alignment fault checking ena
#define CR_M                    0x00000001      // 1 == MMU ena

//
//  Auxiliary Control Register Bit fields
//
#define ACR_CZ                  0x00000040
#define ACR_RV                  0x00000020
#define ACR_RA                  0x00000010
#define ACR_TR                  0x00000008
#define ACR_SB                  0x00000004
#define ACR_DB                  0x00000002
#define ACR_RS                  0x00000001

//
//  Coprocessor Access Control Register Bit Fields
//
#define CACR_CPx_SHIFT(x)       ((x) << 1)
#define CACR_CPx_MASK(x)        (0x3 << CACR_CPx_SHIFT(x))
#define CACR_NO_ACCESS          0x00000000
#define CACR_SUPERVISOR         0x00000001
#define CACR_ALL_ACCESS         0x00000003

//
//  TCM Status Register Bit Fields
//
#define TCMS_ITCM(x)            (((x) >>  0) & 0x3)
#define TCMS_DTCM(x)            (((x) >> 16) & 0x3)

//
//  Data TCM Region Register Bit Fields
//
#define DTCM_BASE_MASK          0xFFFFF000
#define DTCM_BASE_SHIFT         12
#define DTCM_SIZE(x)            (((x) >> 2) & 0x1F)
#define DTCM_SIZE_4K            0x00000003
#define DTCM_SIZE_8K            0x00000004
#define DTCM_SIZE_16K           0x00000005
#define DTCM_SIZE_32K           0x00000006
#define DTCM_SIZE_64K           0x00000007
#define DTCM_SC                 0x00000002
#define DTCM_En                 0x00000001

//
//  Instruction TCM Region Register Bit Fields
//
#define ITCM_BASE_MASK          0xFFFFF000
#define ITCM_BASE_SHIFT         12
#define ITCM_SIZE(x)            (((x) >> 2) & 0x1F)
#define ITCM_SIZE_4K            0x00000003
#define ITCM_SIZE_8K            0x00000004
#define ITCM_SIZE_16K           0x00000005
#define ITCM_SIZE_32K           0x00000006
#define ITCM_SIZE_64K           0x00000007
#define ITCM_SC                 0x00000002
#define ITCM_En                 0x00000001

//
// TLB Lockdown Register Bit Fields
//
#define TLBLOCK_P               0x00000001

//
// Cache Dirty Status Register Bit Fields
//
#define CDSR_C                  0x00000001

//
// Data Fault Status Register Bit Fields
//
#define DFSR_RW                 0x00000800
#define DFSR_DOMAIN(x)          (((x) >> 4) & 0x0F)
#define DFSR_DOMAIN_SHIFT       4
#define DFSR_STATUS(x)          (((x) & 0x0F) | (((x) >> 6) & 0x10))

//
// Instruction Fault Status Register Bit Fields
//
#define IFSR_STATUS             ((x) & 0x0F)

//
// Block Transfer Status Register Bit Fields
//
#define BTSR_R                  0x00000001

//
// Performance Monitor Control Register Bit Fields
//
#define PMNC_EVT_COUNT0(x)      (((x) >> 20) & 0xFF)
#define PMNC_EVT_COUNT1(x)      (((x) >> 12) & 0xFF)
#define PMNC_X                  0x00000400
#define PMNC_CCR                0x00000200
#define PMNC_CR1                0x00000100
#define PMNC_CR0                0x00000080
#define PMNC_EC1                0x00000020
#define PMNC_EC0                0x00000010
#define PMNC_D                  0x00000008
#define PMNC_C                  0x00000004
#define PMNC_P                  0x00000002
#define PMNC_E                  0x00000001

//
// RVDS-GNU interop
//
#ifdef __GNUC__
#ifndef __CC_ARM
static inline void __nop(void)
{
    asm volatile ("nop" : : );
}

static inline void __disable_irq(void)
{
    asm volatile ("cpsid i" : : );
}

static inline void __disable_fiq(void)
{
    asm volatile ("cpsid f" : : );
}

static inline void __enable_irq(void)
{
    asm volatile ("cpsie i" : : );
}

static inline void __enable_fiq(void)
{
    asm volatile ("cpsie f" : : );
}
#endif /* __CC_ARM */
#endif /* __GNUC__ */

//
// Internal API support macro definitions
//
#define CP15_RD_API static inline U32
#define CP15_WR_API static inline void

#ifdef __GNUC__
#ifdef __CC_ARM
#if __ARMCC_VERSION >= 400000

#define CP15_GET(Opcode_1,CRn,CRm,Opcode_2) \
    register U32 value; \
    __asm ("mrc p15," #Opcode_1 ",value," #CRn "," #CRm "," #Opcode_2 ""); \
    return value;

#define CP15_PUT(Opcode_1,value,CRn,CRm,Opcode_2) \
    __asm ("mcr p15," #Opcode_1 "," #value "," #CRn "," #CRm "," #Opcode_2 "");

#define CP15_PUT2(Opcode_1, end_addr, start_addr, CRn) \
    __asm ("mcrr p15," #Opcode_1 "," #end_addr "," #start_addr "," #CRn "");

#define CP15_SBZ(Opcode_1,CRn,CRm,Opcode_2) \
    register U32 SBZ = 0; \
    __asm ("mcr p15," #Opcode_1 ",SBZ," #CRn "," #CRm "," #Opcode_2 "");

#else /* __ARMCC_VERSION */

#define CP15_GET(Opcode_1, CRn, CRm, Opcode_2) \
    register U32 value; \
    asm {mrc p15,##Opcode_1,value,##CRn,##CRm,##Opcode_2} \
    return value;

#define CP15_PUT(Opcode_1, value, CRn, CRm, Opcode_2) \
    asm {mcr p15,##Opcode_1,value,##CRn,##CRm,##Opcode_2}

#define CP15_PUT2(Opcode_1, end_addr, start_addr, CRn) \
    asm {mcrr p15,##Opcode_1,##end_addr,##start_addr,##CRn}

#define CP15_SBZ(Opcode_1, CRn, CRm, Opcode_2) \
    register U32 SBZ = 0; \
    asm {mcr p15,##Opcode_1,SBZ,##CRn,##CRm,##Opcode_2}

#endif /* __ARMCC_VERSION */

#else /* __CC_ARM */

#define CP15_GET(Opcode1,CRn,CRm,Opcode2) \
    register U32 value; \
    asm volatile ("mrc p15," #Opcode1 ", %0," #CRn ", " #CRm ", " #Opcode2 "" : "=r" (value)); \
    return value;

#define CP15_PUT(Opcode1,value,CRn,CRm,Opcode2) \
    asm volatile ("mcr p15," #Opcode1 ",%0," #CRn "," #CRm "," #Opcode2 "" : : "r" (value));

#define CP15_PUT2(Opcode1,end_addr,start_addr,CRn) \
    asm volatile ("mcrr p15," #Opcode1 ", %0, %1," #CRn "" :: "r" (end_addr), "r" (start_addr));

#define CP15_SBZ(Opcode1,CRn,CRm,Opcode2) \
    register U32 SBZ = 0; \
    asm volatile ("mcr p15," #Opcode1 ",%0," #CRn "," #CRm "," #Opcode2"" : : "r" (SBZ));

#endif /* __CC_ARM */

#endif /* __GNUC__ */

//
// System control and configuraion
//
CP15_RD_API SysReadCpuid()                  {CP15_GET(0,c0,c0,0)}
CP15_RD_API SysReadCachetype()              {CP15_GET(0,c0,c0,1)}
CP15_WR_API SysWriteControl(U32 val)        {CP15_PUT(0,val,c1,c0,0)}
CP15_RD_API SysReadControl()                {CP15_GET(0,c1,c0,0)}
CP15_WR_API SysWriteAuxControl(U32 val)     {CP15_PUT(0,val,c1,c0,1)}
CP15_RD_API SysReadAuxControl()             {CP15_GET(0,c1,c0,1)}
CP15_WR_API SysWriteAccessControl(U32 val)  {CP15_PUT(0,val,c1,c0,2)}
CP15_RD_API SysReadAccessControl()          {CP15_GET(0,c1,c0,2)}

//
// MMU control and configuration
//
CP15_RD_API MmuReadTlbType()                {CP15_GET(0,c0,c0,3)}
CP15_WR_API MmuWriteTtb0(U32 val)           {CP15_PUT(0,val,c2,c0,0)}
CP15_RD_API MmuReadTtb0()                   {CP15_GET(0,c2,c0,0)}
CP15_WR_API MmuWriteTtb1(U32 val)           {CP15_PUT(0,val,c2,c0,1)}
CP15_RD_API MmuReadTtb1()                   {CP15_GET(0,c2,c0,1)}
CP15_WR_API MmuWriteTTControl(U32 val)      {CP15_PUT(0,val,c2,c0,2)}
CP15_RD_API MmuReadTTControl()              {CP15_GET(0,c2,c0,2)}
CP15_WR_API MmuWriteDomainAccess(U32 val)   {CP15_PUT(0,val,c3,c0,0)}
CP15_RD_API MmuReadDomainAccess()           {CP15_GET(0,c3,c0,0)}
CP15_WR_API MmuWriteDfs(U32 val)            {CP15_PUT(0,val,c5,c0,0)}
CP15_RD_API MmuReadDfs()                    {CP15_GET(0,c5,c0,0)}
CP15_WR_API MmuWriteIfs(U32 val)            {CP15_PUT(0,val,c5,c0,1)}
CP15_RD_API MmuReadIfs()                    {CP15_GET(0,c5,c0,1)}
CP15_WR_API MmuWriteFaultAddress(U32 val)   {CP15_PUT(0,val,c6,c0,0)}
CP15_RD_API MmuReadFaultAddress()           {CP15_GET(0,c6,c0,0)}
CP15_WR_API MmuTlbICInvalidate()            {CP15_SBZ(0,c8,c5,0)}
CP15_WR_API MmuTlbICInvalidateEntry(U32 val){CP15_PUT(0,val,c8,c5,1)}
CP15_WR_API MmuTlbDCInvalidate()            {CP15_SBZ(0,c8,c6,0)}
CP15_WR_API MmuTlbDCInvalidateEntry(U32 val){CP15_PUT(0,val,c8,c6,1)}
CP15_WR_API MmuTlbInvalidate()              {CP15_SBZ(0,c8,c7,0)}
CP15_WR_API MmuTlbInvalidateEntry(U32 val)  {CP15_PUT(0,val,c8,c7,1)}
CP15_WR_API MmuTlbLockdown(U32 val)         {CP15_PUT(0,val,c10,c0,0)}
CP15_WR_API MmuTlbDCLockdown(U32 val)       {CP15_PUT(0,val,c10,c0,0)}
CP15_WR_API MmuTlbICLockdown(U32 val)       {CP15_PUT(0,val,c10,c0,1)}

//
// Cache Operations
//
CP15_WR_API CacheICInvalidate()                 {CP15_SBZ(0,c7,c5,0)}
CP15_WR_API CacheInvalidate()                   {CP15_SBZ(0,c7,c7,0)}
CP15_WR_API CacheICInvalidateLine(U32 val)      {CP15_PUT(0,val,c7,c5,1)}
CP15_WR_API CacheDCInvalidate()                 {CP15_SBZ(0,c7,c6,0)}
CP15_WR_API CacheDCClean()                      {CP15_SBZ(0,c7,c10,0)}
CP15_WR_API CacheDCCleanInvalidate()            {CP15_SBZ(0,c7,c14,0)}
CP15_WR_API CacheDCInvalidateLine(U32 val)      {CP15_PUT(0,val,c7,c6,1)}
CP15_WR_API CacheDCCleanLine(U32 val)           {CP15_PUT(0,val,c7,c10,1)}
CP15_WR_API CacheDCCleanInvalidateLine(U32 val) {CP15_PUT(0,val,c7,c14,1)}
CP15_WR_API DrainWriteBuffer()                  {CP15_SBZ(0,c7,c10,4)}
CP15_WR_API DataMemoryBarrier()                 {CP15_SBZ(0,c7,c10,5)}
CP15_WR_API WaitForInterrupt()                  {CP15_SBZ(0,c7,c0,4)}
CP15_WR_API FlushPrefetchBuffer()               {CP15_SBZ(0,c7,c5,4)}
CP15_WR_API FlushBranchTarget()                 {CP15_SBZ(0,c7,c5,6)}
CP15_WR_API FlushBranchTargetEntry(U32 val)     {CP15_PUT(0,val,c7,c5,6)}
CP15_RD_API CacheDirtyStatus()                  {CP15_GET(0,c7,c10,6)}
CP15_RD_API CacheBlockStatus()                  {CP15_GET(0,c7,c12,4)}

CP15_WR_API CacheICInvalidateRange(U32 sa, U32 ea)      {CP15_PUT2(0,ea,sa,c5)}
CP15_WR_API CacheDCInvalidateRange(U32 sa, U32 ea)      {CP15_PUT2(0,ea,sa,c6)}
CP15_WR_API CacheDCCleanRange(U32 sa, U32 ea)           {CP15_PUT2(0,ea,sa,c12)}
CP15_WR_API CacheDCCleanInvalidateRange(U32 sa, U32 ea) {CP15_PUT2(0,ea,sa,c14)}

//
// TCM control and configuration (ARM1136)
//
CP15_RD_API TcmReadStatus()                 {CP15_GET(0,c0,c0,2)}
CP15_RD_API TcmReadDataRegion()             {CP15_GET(0,c9,c1,0)}
CP15_WR_API TcmWriteDataRegion(U32 val)     {CP15_PUT(0,val,c9,c1,0)}
CP15_RD_API TcmReadCodeRegion()             {CP15_GET(0,c9,c1,1)}
CP15_WR_API TcmWriteCodeRegion(U32 val)     {CP15_PUT(0,val,c9,c1,1)}

//
// DMA control (ARM1136)
//
CP15_RD_API DmaReadPresent()                {CP15_GET(0,c11,c0,0)}
CP15_RD_API DmaReadQueued()                 {CP15_GET(0,c11,c0,1)}
CP15_RD_API DmaReadRunning()                {CP15_GET(0,c11,c0,2)}
CP15_RD_API DmaReadInterrupting()           {CP15_GET(0,c11,c0,3)}
CP15_RD_API DmaReadUserAccess()             {CP15_GET(0,c11,c1,0)}
CP15_WR_API DmaWriteUserAccess(U32 val)     {CP15_PUT(0,val,c11,c1,0)}
CP15_RD_API DmaReadChannelNum()             {CP15_GET(0,c11,c2,0)}
CP15_WR_API DmaWriteChannelNum(U32 val)     {CP15_PUT(0,val,c11,c2,0)}
CP15_WR_API DmaStop(U32 val)                {CP15_PUT(0,val,c11,c3,0)}
CP15_WR_API DmaStart(U32 val)               {CP15_PUT(0,val,c11,c3,1)}
CP15_WR_API DmaClear(U32 val)               {CP15_PUT(0,val,c11,c3,2)}
CP15_RD_API DmaReadControl()                {CP15_GET(0,c11,c4,0)}
CP15_RD_API DmaReadIntStartAddr()           {CP15_GET(0,c11,c5,0)}
CP15_RD_API DmaReadExtBaseAddr()            {CP15_GET(0,c11,c6,0)}
CP15_RD_API DmaReadIntEndAddr()             {CP15_GET(0,c11,c7,0)}
CP15_RD_API DmaReadChanStatus()             {CP15_GET(0,c11,c8,0)}
CP15_WR_API DmaWriteControl(U32 val)        {CP15_PUT(0,val,c11,c4,0)}
CP15_WR_API DmaWriteIntStartAddr(U32 val)   {CP15_PUT(0,val,c11,c5,0)}
CP15_WR_API DmaWriteExtBaseAddr(U32 val)    {CP15_PUT(0,val,c11,c6,0)}
CP15_WR_API DmaWriteIntEndAddr(U32 val)     {CP15_PUT(0,val,c11,c7,0)}
CP15_WR_API DmaWriteChanStatus(U32 val)     {CP15_PUT(0,val,c11,c8,0)}

//
// System performance monitoring (ARM1136)
//
CP15_RD_API SysmonReadControl()                 {CP15_GET(0,c15,c12,0)}
CP15_WR_API SysmonWriteControl(U32 val)         {CP15_PUT(0,val,c15,c12,0)}
CP15_RD_API SysmonReadCycleCounter()            {CP15_GET(0,c15,c12,1)}
CP15_WR_API SysmonWriteCycleCounter(U32 val)    {CP15_PUT(0,val,c15,c12,1)}
CP15_RD_API SysmonReadCounter0()                {CP15_GET(0,c15,c12,2)}
CP15_WR_API SysmonWriteCounter0(U32 val)        {CP15_PUT(0,val,c15,c12,2)}
CP15_RD_API SysmonReadCounter1()                {CP15_GET(0,c15,c12,3)}
CP15_WR_API SysmonWriteCounter1(U32 val)        {CP15_PUT(0,val,c15,c12,3)}

#ifdef M823XX
/*! \brief Read CPU ID (Kandinsky)
 *         Distinguish by high/low vector area
 *         Valid only after interrupt vector allocation
 *
 *  \return Zero - Upper ARM, Non-Zero - Lower ARM
*/
static inline U32 SysReadCpuID(void)
{
    // Distinguish by high/low interrupt vector area
    // 0 == Low vectors (ARM1--lower ARM)
    // 1 == High vectors (ARM0--upper ARM)
    // TODO: Check CPU ID
    return (SysReadControl() & CR_V) == 0;
}

#define IsUpperArm(cpuid)       (cpuid==UARM)
#define IsLowerArm(cpuid)       (cpuid!=UARM)

#endif /*M823XX*/

static inline U32 SysEnableIrq(void)
{
    U32 flags;

#ifdef __GNUC__
#ifdef __CC_ARM
    __asm ("mrs flags, cpsr");
#else /* __CC_ARM */
    asm volatile ("mrs %0, cpsr" : "=r" (flags));
#endif /* __CC_ARM */
#endif /* __GNUC__ */

    __enable_irq();

    return flags;
}

static inline U32 SysDisableIrq(void)
{
    U32 flags;

#ifdef __GNUC__
#ifdef __CC_ARM
    __asm ("mrs flags, cpsr");
#else /* __CC_ARM */
    asm volatile ("mrs %0, cpsr" : "=r" (flags));
#endif /* __CC_ARM */
#endif /* __GNUC__ */

    __disable_irq();

    return flags;
}

static inline void SysRestoreIrq(U32 flags)
{
#ifdef __GNUC__
#ifdef __CC_ARM
    __asm ("msr cpsr_c, flags");
#else /* __CC_ARM */
    asm volatile ("msr cpsr, %0" : : "r" (flags));
#endif /* __CC_ARM */
#endif /* __GNUC__ */
}

#endif /* SYSCTRL_H_ */
