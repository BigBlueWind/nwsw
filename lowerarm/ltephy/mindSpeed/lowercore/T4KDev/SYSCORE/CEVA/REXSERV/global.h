//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL global.h
 *
 * @brief DOXYGEN_TO_DO
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

///////////////////////////////////////////////////////////////////////////////////////////////
// Defines
///////////////////////////////////////////////////////////////////////////////////////////////
#define TOTAL_CYCLES    	1           			//If we wish to capture total cycles including DMA into internal memory
#define STATUS_COMPLETE		1

#ifndef NULL
#define NULL				((void*)0)
#endif
#define MIN(a,b)			((a) < (b) ? (a) : (b))
#define MAX(a,b)			((a) > (b) ? (a) : (b))
#define MX_OK				0
#define MxGetCpuID()		0
#define MLogTask(a,b,c,d)   MLogDevTask(a,b,c,d)
#define MLogEXEFinish2(a,b) MLogDevEXEFinish2(a,b)
#define MLogEXEStart(a,b,c) MLogDevEXEStart(a,b,c)
#define MxGetTicks()		GetBusTick()

#define TRUE				1
#define FALSE				0

#define FAILED(x)	((x) != MX_OK)

#define DEF_ASSERT_LR()
#define _ASSERT(x)
#define _ASSERT_RC(x)
#define _ASSERT_PTR(x)
#define _ASSERT_HANDLE(h)
#define _ASSERT_LR(x)
#define _ASSERT_PTR_LR(x)

#define REG32(x)	(*(volatile unsigned int*)(x))
#define REG16(x)	(*(volatile unsigned short*)(x))

//CEVAs TCBs are stored starting at CRAM Location 0xFC000000 for each ceva.
//The First 8 bytes at this location are reserved (0XDEADBEEF)
#define CRAM_BASE 0xFC000000
#define CRAM_ARM_MSG_ADDR(id) (((unsigned int *)(CRAM_BASE + 8))[id])

//macro to extract iobuf descriptor, ionum:0 upto 15

#ifdef EXTRACTTCBIOBUF
#undef EXTRACTTCBIOBUF
#endif
//extract the size in bytes
#ifdef EXTRACTTCBIOBUFSZ
#undef EXTRACTTCBIOBUFSZ
#endif

#define EXTRACTTCBIOBUF(tcb, ionum)     (*((UINT32*)tcb->IOControlPtr+ (ionum*2)))
#define EXTRACTTCBIOBUFSZ(tcb, ionum)   ((*((UINT32*)tcb->IOControlPtr+ (ionum*2)+1))&(0x00ffffff))

///////////////////////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////////////////////

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef unsigned char UINT8;
typedef volatile unsigned int VUINT32;
typedef volatile unsigned short VUINT16;
typedef volatile unsigned char VUINT8;
typedef unsigned int * PUINT32;
typedef unsigned short* PUINT16;
typedef unsigned char* PUINT8;

typedef void * LPVOID;
typedef UINT32 MXRC;
typedef UINT32 HANDLE;
typedef UINT32 IRQTYPE;
typedef UINT32 BOOL;

typedef struct tDSP_DMA_DESC
{
    uint32 next;
    uint32 code;
    uint32 length;
    uint32 internal;
    uint32 external;
} DSP_DMA_DESC, *PDSP_DMA_DESC;


///////////////////////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////////////////////

extern volatile unsigned short ArmMsgReady;

///////////////////////////////////////////////////////////////////////////////////////////////
// Function Prototypes
///////////////////////////////////////////////////////////////////////////////////////////////

#if defined EVM
extern uint32 GetBusTick();
__inline uint32 GetExecBusTicks(unsigned start_tick);
#else

#define GetBusTick()        0
#define GetExecBusTicks(a)  0
#endif

uint32 GetCevaID();
uint32 GetSP();
void GenArmInt();
void* GetArmMsg(unsigned int id);

void DmaFromExtPram(void *p_ext_addr, void *p_int_addr, unsigned size);
void DmaFromExtDram(void *p_ext_addr, void *p_int_addr, unsigned size);
void DmaToExtDram(void *p_ext_addr, void *p_int_addr, unsigned size);
void MemCpy(void* dst, void* src, unsigned long size);
void MemCpyFromExt(void* dst, void* src, unsigned long size);
void MemCpyToExt(void* dst, void* src, unsigned long size);
void dwmemcpy(void *dst, void *src, unsigned size);

#endif
