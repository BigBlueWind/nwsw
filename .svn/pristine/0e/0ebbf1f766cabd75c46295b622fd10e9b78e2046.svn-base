#include <asm-dsp.h>
#include "typedef.h"
#include "global.h"
#include "tcb.h"
#include "rex_serv.h"
#include "mlogceva.h"
#include "cevamdmadrv.h"

extern REX_SERV_CTX rex_serv_ctx;

void* GetArmMsg(unsigned int id)
{
	TCB * pTcb = NULL;
	
	while (ArmMsgReady == 0 && rex_serv_ctx.pInf == NULL);
	
	if (ArmMsgReady)
	{
		pTcb = (TCB*)CRAM_ARM_MSG_ADDR(id);
		ArmMsgReady = 0;
	}
	
    return pTcb;
}

void dwmemcpy(void *dst, void *src, unsigned size)
{
    unsigned *p_dst = (unsigned *) dst;
    unsigned *p_src = (unsigned *) src;

    size >>= 2;

    while (size--)
        *p_dst++ = *p_src++;
}

int main()
{
    U32 ceva_id = GetCevaID();
    volatile PTCB pTcb;
	unsigned int TaskID, Status, opt, ticks;
    
	ArmMsgReady = 0;
	
    //  system initialization
    _dsp_asm("and a0, #0x300, a0");
    _dsp_asm("or a0, #0x0080001B, a0");
    _dsp_asm("mov a0, moda");
	_dsp_asm("nop");
	_dsp_asm("nop");
	_dsp_asm("nop");
	_dsp_asm("nop");
	_dsp_asm("nop");
	_dsp_asm("nop");
	_dsp_asm("nop");
	_dsp_asm("nop");
	
	pTcb = (PTCB)CRAM_ARM_MSG_ADDR(ceva_id);
	
	if (pTcb != NULL)
		pTcb->Status = STATUS_COMPLETE;
	
	// to initialize the global context 
	MLogDevInit();
	RexServPreInit();
	
	// in a loop we process TCBs from the client and 
	// also processes Client/Server command sent by using 
	// personal communication interface 
	
	while (1)
	{
		// if the host generated an interrupt (new one TCB is ready to be processed)
		// for compatibility, processing of copy_data task is added
		
		pTcb = (volatile PTCB)GetArmMsg(ceva_id);
		
		if (pTcb != NULL)
		{
			ticks = GetBusTick ();
			
			TaskID = pTcb->TaskID;
			Status = pTcb->Status;
			opt    = Status;
			
			Status = 0;
			
			if (TaskID == TASKID_COPY_DATA)
			{
				U32 *InputDataPtr = pTcb->InputDataPtr;
				U32 *OutputDataPtr = pTcb->OutputDataPtr;
				U32 length;

				if (InputDataPtr && OutputDataPtr)
				{
					length = MIN(pTcb->InputDataLen, pTcb->OutputDataLen);
					dwmemcpy(OutputDataPtr, InputDataPtr, length);
					Status = STATUS_COMPLETE; // make this request as complete
				}
				
				//pTcb->ExecTicks = GetBusTick () - ticks;
				pTcb->ExecTicks = ceva_id;
			}
			else if (TaskID == 1)
			{
				RexServSetInterface((void*)pTcb->InputDataPtr);
				Status = STATUS_COMPLETE;
			}
			else
			{
				Status = STATUS_COMPLETE;
			}
			
			//if ((opt & ((U32)1 << TCB_CFG_STAT_DIS_INT)) != 0)
			if (opt & (1<<16))
			{
				Status |= TCB_CFG_STAT_MARK_DONE;
			}
			
			pTcb->Status = Status;
			
			if (opt & (1<<16))
			{
				GenArmInt(); // generate interrupt to arm via CUO_0
			}
		}
		else
		{
			// to run the processing of server kernel
			// this function takes processor time if 
			// communication interface was set (see: TCB::TaskID = 1)
		
			RexServKernel(1);
		}
	}
   
    return 0;
}
