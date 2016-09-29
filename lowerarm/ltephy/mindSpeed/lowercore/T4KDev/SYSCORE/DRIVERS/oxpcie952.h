//-------------------------------------------------------------------------------------------
/** @file oxpcie952.h
 *
 * @brief System Timers implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef __OXPCIE952_H__
#define __OXPCIE952_H__

#define PCI_OX_MSIX_CTRL	0x2C
#define PCI_OX_MSIX_TABLE	0x2D
#define PCI_OX_MSIX_PEND	0x2E

#define OXPCIE952_VENDOR_ID	0x1415
#define OXPCIE952_DEVICE_ID 0xC158

// UART Standard 550 Compatible Registers
#define OX_REG_THR			0x00	/**< @brief Transmitter Holding Register (Data to be transmitted) */
#define OX_REG_RHR			0x00	/**< @brief Receiver Holding Register (Data received) */
#define OX_REG_IER			0x01	/**< @brief Interrupt Enable Register */
#define OX_REG_FiCR			0x02	/**< @brief FIFO Control Register */
#define OX_REG_ISR			0x02	/**< @brief Interrupt Status Register */
#define OX_REG_LCR			0x03	/**< @brief Line Control Register */
#define OX_REG_MCR			0x04	/**< @brief Modem Control Register */
#define OX_REG_LSR			0x05	/**< @brief Line Status Register */
#define OX_REG_MSR			0x06	/**< @brief Modem Status Register */
#define OX_REG_SPR			0x07
// Additional Standard Registers
#define OX_REG_DLL			0x00
#define OX_REG_DLM			0x01

// Extended Registers
// UART Device Driver Speed-up Registers
#define OX_SREG_THR			0x80
#define OX_SREG_RHR			0x80
#define OX_SREG_LSR			0x84
#define OX_SREG_ISR			0x85
#define OX_SREG_RFL			0x86
#define OX_SREG_TFL			0x87
// UART 650 Compatible Registers
#define OX_REG_EFR			0x92
#define OX_REG_XON1			0x94
#define OX_REG_XON2			0x95
#define OX_REG_XOFF1		0x96
#define OX_REG_XOFF2		0x97
// UART 950 Specific Registers
#define OX_REG_ASR			0xA1
#define OX_REG_RFL			0xA3
#define OX_REG_TFL			0xA4
#define OX_REG_ICR			0xA5
// UART Indexed Control Register Set
#define OX_REG_ACR			0xC0
#define OX_REG_CPR			0xC1
#define OX_REG_TCR			0xC2
#define OX_REG_CPR2			0xC3
#define OX_REG_TTL			0xC4
#define OX_REG_RTL			0xC5
#define OX_REG_FCL			0xC6
#define OX_REG_FCH			0xC7
#define OX_REG_ID1			0xC8
#define OX_REG_ID2			0xC9
#define OX_REG_ID3			0xCA
#define OX_REG_REV			0xCB
#define OX_REG_CSR			0xCC
#define OX_REG_NMR			0xCD
#define OX_REG_MDM			0xCE
#define OX_REG_RFC			0xCF
#define OX_REG_GDS			0xD0
#define OX_REG_DMS			0xD1
#define OX_REG_PIDX			0xD2
#define OX_REG_RS486_DLYEN	0xD4
#define OX_REG_RS486_DLYCNT	0xD5
#define OX_REG_WDM			0xD7
#define OX_REG_A_LATCH		0xD8
#define OX_REG_A_ENABLE		0xD9
#define OX_REG_A_DLL		0xDA
#define OX_REG_A_DLM		0xDB
#define OX_REG_A_CPR		0xDC
#define OX_REG_A_TCR		0xDD
#define OX_REG_A_CPR2		0xDE
// DMA 32-bit registers
#define OX_REG_DMA_ADDR_LO	0x100
#define OX_REG_DMA_ADDR_HI	0x104
#define OX_REG_DMA_LENGTH	0x108
#define OX_REG_DMA_STATUS	0x10C

#define LCR_MAGIC			0xBF

// interrupt sources
#define OX_IRQ_NONE			0x01	/**< @brief No interrupt pending */
#define OX_IRQ_RX_ERROR	0x06	/**< @brief Receiver status error */
#define OX_IRQ_RX			0x04	/**< @brief Receiver data available */
#define OX_IRQ_RX_TIMEOUT	0x0C	/**< @brief Receiver time-out */
#define OX_IRQ_TX			0x02	/**< @brief Transmitter holding register empty (OK to TX) */
#define OX_IRQ_MDM_STATUS	0x00	/**< @brief Modem control signal status change */
#define OX_IRQ_FLOW_CTRL	0x10	/**< @brief In band flow control (XON, XOFF) */
#define OX_IRQ_CTS_RTS		0x20	/**< @brief Change in state of CTS or RTS */ 
// DMA direction
#define DMA_RX				(1 << 31)
#define DMA_TX				(0 << 31)

typedef struct tOXPCIe952Stats 
{
	UINT32 total;
	UINT32 rx;
	UINT32 rx_chars;
	UINT32 rx_timeouts;
	UINT32 rx_errors;
	UINT32 tx;
	UINT32 tx_chars;
	UINT32 mdm_status;
	UINT32 flow_control;
	UINT32 cts_rts;
	UINT32 none;
	// Misc. data
	UINT32 error_lsr;

	UINT32 loop_count;
	UINT32 idle_count;
	UINT32 tx_count;
	UINT32 tx_char_count;
} OXPCIe952Stats, *POXPCIe952Stats;

typedef struct tOXDmaBuf
{
	UINT32   size;
	UINT8    *buf;
	UINT8    *end;
	UINT8    *head;
	UINT8    *tail;
} OXDmaBuf;

typedef struct tOXPCIe952
{
	UINT32 id;
	void     *mmr;
	OXDmaBuf rx;
	OXDmaBuf tx;

	OXPCIe952Stats 	OXStats;
} OXPCIe952, *POXPCIe952;

void OXPCIe952RxIntTest(void);
UINT32 OXPCIe952Init(UINT32 id, UINT32 IntMode);

#endif /* __OXPCIE952_H__ */
