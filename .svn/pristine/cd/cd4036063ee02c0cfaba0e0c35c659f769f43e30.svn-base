//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL oxpcie952.c
 *
 * @brief OXPCIe952 PCIe Driver functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "pciedrv.h"
#include "syscore.h"
#include "hal.h"
#include <string.h>
#include "oxpcie952.h"

POXPCIe952 gPOXPCIe952;

static void OXDmaBufInit(OXDmaBuf *pOXDma, UINT32 size)
{
	pOXDma->size = size;
	pOXDma->buf  = MxHeapAlloc (DspHeap, size);
	pOXDma->end  = pOXDma->buf + size;
	pOXDma->head =
	pOXDma->tail = pOXDma->buf;

	memset(pOXDma->buf, 0, size);
}

INLINE UINT32 ox_reg_read(const OXPCIe952 *ctx, unsigned reg)
{
#if 0
	return *((VUINT8*)ctx->mmr + reg);
#else
	VUINT32 tmp;

	tmp = *((VUINT8*)ctx->mmr + reg);
	tmp = *((VUINT8*)ctx->mmr + reg);

	return tmp;

#endif
}

INLINE void ox_reg_write(const OXPCIe952 *ctx, unsigned reg, unsigned val)
{
	printf("wr %p<=%02x\n", (UINT8*)ctx->mmr + reg, val);
	*((VUINT8*)ctx->mmr + reg) = val;
}

INLINE unsigned ox_reg32_read(const OXPCIe952 *ctx, unsigned reg)
{
#if 0
	return *((volatile unsigned*)((char*)ctx->mmr + reg));
#else
	VUINT32 tmp;

	tmp = *((volatile unsigned*)((char*)ctx->mmr + reg));
	tmp = *((volatile unsigned*)((char*)ctx->mmr + reg));

	return tmp;
#endif
}

INLINE void ox_reg32_write(const OXPCIe952 *ctx, unsigned reg, unsigned val)
{
	*((volatile unsigned*)((char*)ctx->mmr + reg)) = val;
}

void OXPCIe952_ISR(UINT32 irq, void *pData)
{
	POXPCIe952 ctx = (POXPCIe952)pData;
	char c;
	unsigned lsr;

	ctx->OXStats.total++;

	do
	{
		// Read the interrupt status register
		// Note: reading this interrupt also clears the status bits to allow the next interrupt to occur.
	    	irq = ox_reg_read(ctx, OX_REG_ISR) & 0x3F;

		//printf("\nIrq: %x\n", irq);

		switch (irq)
		{
			case OX_IRQ_RX_ERROR:
				ctx->OXStats.rx_errors++;
				lsr = ox_reg_read(ctx, OX_REG_LSR);
				ctx->OXStats.error_lsr = lsr;
				break; // AKB added, errors processed separately from Main RX code 

			case OX_IRQ_RX:
			case OX_IRQ_RX_TIMEOUT:
				{
					unsigned i;
					unsigned nrx;
					unsigned lsr = ox_reg_read(ctx, OX_REG_LSR);

					//printf("LSR: %x\n", lsr);

					if (irq == OX_IRQ_RX)
{						ctx->OXStats.rx++;
uart_printf(".");
}
					else
{						ctx->OXStats.rx_timeouts++;
uart_printf("@");
}

					// Get receive FIFO level
					nrx = ox_reg_read(ctx, OX_REG_RFL);

					for (i = 0; i < nrx; i++)
					{
						c = ox_reg_read(ctx, OX_REG_RHR);

						if (ctx->rx.head >= ctx->rx.end)
							ctx->rx.head = ctx->rx.buf;

						*ctx->rx.head++ = c;
						ctx->OXStats.rx_chars++;
						//printf(">%x\n", c);
						//UART_Transmit(&c, 1);
					}
					//OXDmaRxStart(ctx, reg);
				}
				break;

			case OX_IRQ_TX:
				ctx->OXStats.tx++;
				break;

			case OX_IRQ_MDM_STATUS:
				ctx->OXStats.mdm_status++;
				break;

			case OX_IRQ_FLOW_CTRL:
				ctx->OXStats.flow_control++;
				break;

			case OX_IRQ_CTS_RTS:
				ctx->OXStats.cts_rts++;
				break;
			
			case OX_IRQ_NONE:
				ctx->OXStats.none++;
				break;
		}
	} while (OX_IRQ_NONE != irq);
}

#if 0
unsigned OXDmaRxStart(OXPCIe952 *ctx, unsigned size)
{
	UINT8 *head;

	// program DMA
	ox_reg32_write(ctx, OX_REG_DMA_ADDR_LO, (int)ctx->rx.head);
	//ox_reg_write(ctx, OX_REG_DMA_ADDR_HI, 0);

	// move head pointer
	head = ctx->rx.head + size;

	if (head >= ctx->rx.end) {
		size -= (head - ctx->rx.end);
		head = ctx->rx.buf;
	}

	ctx->rx.head  = head;

	// start
	ox_reg32_write(ctx, OX_REG_DMA_LENGTH, DMA_RX | size);

	return size;
}

static void OXPCIeDmaTest(OXPCIe952 *ctx)
{
	unsigned bytes = 0;
	unsigned prev_bytes = 0;
	unsigned dma_active = 0;

	for (;;)
	{
		unsigned reg, lsr;
		unsigned rfl, tfl;

		reg = ox_reg32_read(ctx, OX_SREG_LSR);
		lsr = reg & 0xff;
		rfl = (reg >> 16) & 0xff;
		tfl = (reg >> 24);

		if (rfl > 112 && !dma_active)
		{
			dma_active = OXDmaRxStart(ctx, rfl);
		}

		reg = ox_reg32_read(ctx, OX_REG_DMA_STATUS);

		/*if ((reg & 1) && rfl < 20)
			printf("DMA %x,%d\n", reg, rfl);*/

		// if DMA request has completed
		if (dma_active && (reg & 4))
		{
			bytes += dma_active;
			if ((bytes - prev_bytes) > 10000) {
				printf("bytes %u\n", bytes);
				prev_bytes = bytes;
			}
			if (bytes > 1000000000) break;
			//UART_Transmit(ctx->rx.buf, dma_active);
			dma_active = 0;
			// if DMA error
			if (reg & 2) {
				printf("DMA error %x,%d\n", reg, rfl);
				break;
			}
		}

		// fill Tx FIFO
		if (tfl < 80) {
			ox_reg32_write(ctx, OX_SREG_THR, 0x33323130);
		}
	}
}
#endif

void OXPCIe952RxIntTest(void)
{
	UINT32 reg, /*lsr, rfl, */tfl, ctr = 0;
	POXPCIe952 ctx = gPOXPCIe952;

	uart_printf("\nStarting OXPCie Interrupt test\n");
	SysDelay(10);

	for (;;)
	{
		ctx->OXStats.loop_count++;

		reg = ox_reg32_read(ctx, OX_SREG_LSR);
//		lsr = reg & 0xff;
//		rfl = (reg >> 16) & 0xff;
		tfl = (reg >> 24);

		if (tfl == 0) 
		{
			// Space in TX FIFO available to send characters
			ctx->OXStats.tx_count++;

			//
			// Use OX9952 enhanced register to send 4 character
			// at a time "0123"
			//
			ox_reg32_write(ctx, OX_SREG_THR, 0x33323130);
			ctx->OXStats.tx_char_count += 4;
			ctr = 0;
		}
		else
		{
			ctx->OXStats.idle_count++;
			if(ctr++== 500)
			{
				printf("Loop locked\n");
				break;
			}
		}
	}
}

void OXPCIe952_SetParams(OXPCIe952 *ctx)
{
	unsigned reg;

	//
	// Reset the chip, to make sure we are in a default state
	//
	ox_reg_write(ctx, OX_REG_CSR, 0);
	SysDelay(10); // Wait for 10 milliseconds to be safe

	// Make sure interrupts are disabled prior to starting
	ox_reg_write(ctx, OX_REG_IER, 0);

	// Read ISR until clear to make sure no pending interrupts
	do 
	{
		reg = ox_reg_read(ctx,OX_REG_ISR) & 0x3F;
		printf("ICR=%02x\n", reg);
	} while (reg != OX_IRQ_NONE);

	reg = ox_reg_read(ctx, OX_REG_LCR);
	printf("LCR=%02x\n", reg);

	ox_reg_write(ctx, OX_REG_LCR, 3);
	// latch access
	ox_reg_write(ctx, OX_REG_LCR, LCR_MAGIC);
	ox_reg_write(ctx, OX_REG_EFR, 0x10);	// Enhanced Mode
	// set speed 115200 bps
	ox_reg_write(ctx, OX_REG_DLL, 0x22);
	ox_reg_write(ctx, OX_REG_DLM, 0x00);
	// 8-bit, no parity, 1 stop-bit
	ox_reg_write(ctx, OX_REG_LCR, 3);

	ox_reg_write(ctx, OX_REG_TCR, 4);
//	ox_reg_write(ctx, OX_REG_CPR, 0x08);
	ox_reg_write(ctx, OX_REG_CPR, 0x20);

//	ox_reg_write(ctx, OX_REG_MCR, 0x80);	// enable M+N/8 divider
	ox_reg_write(ctx, OX_REG_MCR, 0x90);	// enable M+N/8 divider
	ox_reg_write(ctx, OX_REG_FiCR, 1);		// enable FIFO for 16C550 mode (16 byte FIFO)
	
//	ox_reg_write(ctx, OX_REG_RTL, 64); 		// set Receive Trigger Level
	ox_reg_write(ctx, OX_REG_RTL, 16); 		// set Receive Trigger Level
	ox_reg_write(ctx, OX_REG_ACR, 0x20);	// enable Trigger Level

	// Setup interrupt enable register (IER)
	// enable Rx Interrupts
	ox_reg_write(ctx, OX_REG_IER,
			     0
			     | (0<<7) // CTS
			     | (0<<6) // RTS
			     | (0<<5) // Special character detect
			     | (0<<4) // Unused
			     | (0<<3) // Modem interrupt mask
			     | (0<<2) // Rx Stat interrupt mask
			     | (0<<1) // Transmit holding register emtpy interrupt mask
			     | (1<<0) // Receive characater available interrupt mask
			     );

	ox_reg_write(ctx, OX_REG_ACR, 0xA0);	// enable Additional Status, Trigger Level
}

UINT32 OXPCIe952Init(UINT32 id, UINT32 IntMode)
{
	printf("\nInitializing OXPCIe952 (%d)\n", id);

	// Allocate local management structure from the heap
	gPOXPCIe952 = MxHeapAlloc (NcnbHeap, sizeof(*gPOXPCIe952));
	if (!gPOXPCIe952)
		return FALSE;

	memset(gPOXPCIe952, 0, sizeof(*gPOXPCIe952));

	gPOXPCIe952->id = id;
	gPOXPCIe952->mmr = (void*)(PCIE_RC_MEM + 0x1000*(id+1));

	OXDmaBufInit(&gPOXPCIe952->rx, 1024);
	OXDmaBufInit(&gPOXPCIe952->tx, 1024);

	// Setup interrupt vector prior to programming, this makes sure
	// That when the chip and bus are starting to program, you handle
	// any initial interrupts that come in during programming

#if 0
	/*Interrupt*/
	/*if (IntMode == PCIE_INT_MSIX)	// Message Signaled Interrupt Mode
	{
		UINT32 *pMsiX;
		UINT32 reg;

		pcie_read_config_dword(dev, PCIEREG(PCI_OX_MSIX_TABLE), &reg);
		pMsiX = (U32*)(PCIE_RC_MEM + (reg & ~7));
		pMsiX[0] = 0xFAA00000;
		pMsiX[2] = 0xA1234567;
		pMsiX[3] = 0;

		// Enable MSI-X interrupts
		pcie_read_config_dword(dev, PCIEREG(PCI_OX_MSIX_CTRL), &reg);
		reg |= 1 << 31;
		pcie_write_config_dword(dev, PCIEREG(PCI_OX_MSIX_CTRL), reg);

		pcie_request_irq(dev, OXPCIe952_ISR, gPOXPCIe952);
		ICEnableNonSecureInt(HAL_IRQ_PCIE_4, KSE_get_hw_cpuid());
	}
	else*/ if (IntMode == PCIE_INT_MSI)	// Message Signaled Interrupt Mode
	{
		UINT32 reg;

		// Enable MSI Interrupts in MSI Control Register
		pcie_read_config_dword(dev, PCIEREG(PCI_FUNC_MSI_CTRL), &reg);
		reg |= 1 << 16;
		pcie_write_config_dword(dev, PCIEREG(PCI_FUNC_MSI_CTRL), reg);

		pcie_request_irq(dev, OXPCIe952_ISR, gPOXPCIe952);
		ICEnableNonSecureInt(HAL_IRQ_PCIE_4, KSE_get_hw_cpuid());
	}
	else
#endif
		if (IntMode == PCIE_INT_LEGACY) 	// Legacy Interrupt Mode
	{
		PCIeDrvSetIrqCb(FALSE, OXPCIe952_ISR, (void*)gPOXPCIe952);
	}

	// Program chip after interrupt service routine settings are complete so any interrupts generated
	// due to initial programming are handled OK
	OXPCIe952_SetParams(gPOXPCIe952);

	//OXPCIeDmaTest(gPOXPCIe952);
	//OXPCIe952RxIntTest(gPOXPCIe952);

	return TRUE;
}
