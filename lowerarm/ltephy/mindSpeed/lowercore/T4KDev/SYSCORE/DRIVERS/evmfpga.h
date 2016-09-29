//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL evmfpga.h
 *
 * @brief PCIe EP / Radio control implemented on FPGA
 * @author Mindspeed Technologies
 * @version $Revision: 1.25 $
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

#ifndef __EVMFPGA_H__
#define __EVMFPGA_H__

#include "config.h"
#include "4gmx.h"
#include "appids.h"
#include "heaps.h"
#include "apimgr.h" 

//#define EVM_FPGA_TX_LOG_1_4_SF

#define RC_EVM_FPGA_NOT_INITED      DEF_USRRC(APP_DRIVER, EVM_FPGA_DRV, 1)
#define RC_EVM_FPGA_NO_MEMORY       DEF_USRRC(APP_DRIVER, EVM_FPGA_DRV, 2)
#define RC_EVM_FPGA_NO_OWNER        DEF_USRRC(APP_DRIVER, EVM_FPGA_DRV, 3)
#define RC_EVM_FPGA_LOG_PARAM_ERROR DEF_USRRC(APP_DRIVER, EVM_FPGA_DRV, 4) // Log parameters are provided incorrectly
#define RC_EVM_FPGA_NO_MDMA_DESCS   DEF_USRRC(APP_DRIVER, EVM_FPGA_DRV, 5)
#define RC_EVM_FPGA_NO_IRQ          DEF_USRRC(APP_DRIVER, EVM_FPGA_DRV, 6)
#define RC_EVM_FPGA_NO_PATTERN      DEF_USRRC(APP_DRIVER, EVM_FPGA_DRV, 7)
#define RC_EVM_FPGA_COMPARE_ERROR   DEF_USRRC(APP_DRIVER, EVM_FPGA_DRV, 8)
#define RC_EVM_FPGA_MIMO_IS_NOT_SUP DEF_USRRC(APP_DRIVER, EVM_FPGA_DRV, 9)
#define RC_EVM_FPGA_INT_PROF_ERR    DEF_USRRC(APP_DRIVER, EVM_FPGA_DRV, 10)
#define RC_EVM_FPGA_ALLOC_PCIE_STAT DEF_USRRC(APP_DRIVER, EVM_FPGA_DRV, 11) // Error of allocation buffer for PCIe interrupt status

#define EVM_FPGA_RX_TIME_TRACE_NUM  0 //200
#define EVM_FPGA_TX_TIME_TRACE_NUM  0 //200
#define EVM_FPGA_TIMER_TRACE_NUM    0 //200

#define EVM_FPGA_REINIT_TIMER_ON_FIRST_RX

#define RFPGA_IM_TX             (1 << 5)
#define RFPGA_IM_RX             (1 << 7)

#define FPGA_RAD_TX_FIFO_SIZE   ((15360/4) * 4)  // 1/4 IQ sample
#define FPGA_RAD_RX_FIFO_SIZE   ((15360)   * 4)  // 1 IQ sample
#define FPGA_RAD_RX_SIZE        ((1*15360) * 4)  // 2 RX buffers
#define FPGA_RAD_TX_SIZE        ((1*15360) * 4)  // 1 TX buffers
#define EVM_FPGA_FIRST_TX_SIZE  (15360)
#define EVM_FPGA_FIRST_RX_SIZE  (15360)

#ifdef EVM_FPGA_TX_LOG_1_4_SF
#define FPGA_TX_LOG_PART_SIZE   (15360 * 1)  // 1/4 TX buffers
#else
#define FPGA_TX_LOG_PART_SIZE   (15360 * 4)  // 1/1 TX buffers
#endif

#define FPGA_RAD_RX_BUF_SIZE    ((2*15360) * 4)  // 2 RX buffers
#define FPGA_RAD_TX_BUF_SIZE    ((2*15360) * 4)  // 2 TX buffers

#define EVM_FPGA_CTRL_TX0_LOG    (1 << 0)    // To enable TX#0 log
#define EVM_FPGA_CTRL_TX1_LOG    (1 << 1)    // To enable TX#1 log
#define EVM_FPGA_CTRL_RX0_LOG    (1 << 2)    // To enable RX#0 log
#define EVM_FPGA_CTRL_RX1_LOG    (1 << 3)    // To enable RX#1 log
#define EVM_FPGA_CTRL_TX_LOG_RR  (1 << 4)    // RX log is in round-robin mode
#define EVM_FPGA_CTRL_RX_LOG_RR  (1 << 5)    // TX log is in round-robin mode
#define EVM_FPGA_CTRL_CHECK      (1 << 6)    // To run some initial test to be sure that system works
#define EVM_FPGA_CTRL_AUTO_RUN   (1 << 7)    // To run TX/RX in initialization routine

#define EVM_FPGA_PCIE_CONNECTION_TIME   (150000000 * 1) // 1 second

#define DI_EVMFPGA_GPIO_CTRL 2

/******************************************************************************
*              Interrupt signal processing                                    *
******************************************************************************/
//#define EVM_FPGA_INT_HANDLER_CPU_ID         MxGetCpuID()    // To run IRQ hanler on the same CPU where code was initialized
#define EVM_FPGA_INT_HANDLER_CPU_ID         0             // To run IRQ handler on CPU-0
//#define EVM_FPGA_INT_HANDLER_CPU_ID         1             // To run IRQ handler on CPU-1
//#define EVM_FPGA_INT_HANDLER_CPU_ID         2             // To run IRQ handler on CPU-2
//#define EVM_FPGA_INT_HANDLER_CPU_ID         3             // To run IRQ handler on CPU-3

#define EVM_FPGA_INT_PROF_DEF               0   // The default profile (GPIO_6-tx and GPIO_2-rx)
#define EVM_FPGA_INT_PROG_0_2               1   // The profile (GPIO_0-tx and GPIO_2-rx)
#define EVM_FPGA_INT_PROF_PCIE_MASTER       2   // The PCIe is Master(reads/writes memory) / GPIO INTs are used
#define EVM_FPGA_INT_PROF_PCIE_MASTER_IRQ   3   // The PCIe is Master(reads/writes memory) and also generates interrupts

//#define EVM_FPGA_INT_FORCED_PROF        EVM_FPGA_INT_PROF_DEF
//#define EVM_FPGA_INT_FORCED_PROF        EVM_FPGA_INT_PROG_0_2
//#define EVM_FPGA_INT_FORCED_PROF        EVM_FPGA_INT_PROF_PCIE_MASTER
#define EVM_FPGA_INT_FORCED_PROF        EVM_FPGA_INT_PROF_PCIE_MASTER_IRQ

#define IS_PROF_DEF(ctx)                (((ctx)->IntProfID) == EVM_FPGA_INT_PROF_DEF)
#define IS_PROF_PROG_0_2(ctx)           (((ctx)->IntProfID) == EVM_FPGA_INT_PROG_0_2)
#define IS_PROF_PCIE_MASTER(ctx)        (((ctx)->IntProfID) == EVM_FPGA_INT_PROF_PCIE_MASTER)
#define IS_PROF_PCIE_MASTER_IRQ(ctx)    (((ctx)->IntProfID) == EVM_FPGA_INT_PROF_PCIE_MASTER_IRQ)

//
// FPGA radio module control register bits
//
#define RFPGA_RADIO_MODULE_ENABLE                   (1<<0)
#define RFPGA_PERFORM_FULL_DUPLEX                   (1<<1)
#define RFPGA_PERFORM_RX_STOP_TX                    (1<<2)
#define RFPGA_ENABLE_DATA_TRANSFER                  (1<<3)
#define RFPGA_ALLOW_DATA_STATE_REACHED_INTERRUPT    (1<<4)
#define RFPGA_ENABLE_AGC                            (1<<5)
#define RFPGA_HEADER_CPLD                           (1<<6)

//
// Bit mask (multiple registers) for controlling antennas
// enable/disable bits
// NOTE: In current FPGA, only single radio module with up to two antennas is
// supported.  Second radio module is not supported (coded here for possible
// future support)
//
#define RFPGA_ANTENNA_0		(1<<0) /**< @brief First antenna  of first  radio module */
#define RFPGA_ANTENNA_1		(1<<1) /**< @brief Second antenna of first  radio module */
#define RFPGA_ANTENNA_2		(1<<2) /**< @brief First antenna  of second radio module */
#define RFPGA_ANTENNA_3		(1<<3) /**< @brief Second antenna of second radio module */

//
// Dual antenna mode (bit 14 of FIFO status register)
// Set to 1 for 2 antennas (RX and TX) or 0 for one antenna
//
#define RFPGA_DUAL_ANTENNA_MODE	(1<<14)

#define FPGA_CODE_VERSION_SISO  0x00010001

/** 
 * @brief PCIE to Radio interface FPGA memory mapped register structure
 * 
 * @details 
 *  On Mindspeed EVMs, this is on the expansion bus memory space
 *  and is setup for address base of 0xB0000000
 * 
 *  All control plane (setting/reading of control registers)
 *  is done via this address space.  PCIe is used for sending
 *  and receiving of the actual I/Q data.
 * 
 * New registers for MIMO capable FPGA are:
 * <pre>
 * 
 * 0x22C ADC to PCI FIFO data path enable
 * 
 * 0x230-0x237: Capture RAM Markers, 2 x 32 bits (Data compare value to look for)
 * 0x238-0x23B: Reserved 
 * 0x23C:       Marker check, 0 to look for marker, Reserved for Debug use 
 * 0x23D-0x23F: Reserved 
 * 0x240:       Capture RAM Done Status, 1 Done, 0 not done
 * 0x241:       Capture RAM Busy Status, 1 Busy, 0 not busy 
 * 0x242:       Capture RAM Marker 0 Status 
 * 0x243-0x24F: Reserved 
 * 0x250:       Capture RAM Bank Dump Select, 0 or 1 for Bank 0 or 1 
 * 0x251-0x25F: Reserved
 * 0x260-0x263: 32 bit data from capture ram read 32 bits at a time base on length
 * 0x264:       Amount of data to read from Capture RAM Bank 
 * 
 * 0x300 Egress FIFO 1 overflow counts
 * 0x304 Egress FIFO 2 overflow counts
 * 0x308 Underflow FIFO 1
 * 0x30C Underflow FIFO 2
 * 
 * 0x318 Number of 8 bytes writes wrote to egress FIFO antenna 1
 * 0x31C Number of 8 bytes writes wrote to egress FIFO antenna 2
 * 0x320 Egress FIFO 1 to 2 toggle counts (antenna mux)
 * 0x324 Egress FIFO 2 to 1 toggle counts (antenna mux)
 * 
 * 0x370 Egress PCIe 8 byte transfer counts
 * 0x374 Egress PCIe Start of Frame  counts (PCIe write request from Transced to FPGA)
 * 0x378 Egress PCIe End of frame counts (i.e. end of DMA transfer, should match with start)
 * 
 * 0x380 BIST enable (1 enable, 0 disable)
 * 0x381 BIST status 1 (0 while runnng, non-zero when done)
 * 0x382 BIST status 2
 * 0x383 Internal generator checker mode: enabling (BIST status 3?)
 * 
 * 0x384 DLL Lock status register (0 if locked, 1 if not)
 * 0x388 DLL control 1 Set/Hold in reset, 0 Release from Reset
 * 
 * 0x38C FPGA internal loopback (TX to RX right before radio interface)
 * 0x390 Number of 8 byte reads from egress FIFO antenna 1 (from FIFO to Radio interface)
 * 0x394 Number of 8 byte reads from egress FIFO antenna 2 (from FIFO to Radio interface) 
 * 
 * Ingress FIFO(s) (from Radio to PCIe block) statistics
 * 0x3A8 Antenna 1 Ingress FIFO overflow counts
 * 0x3AC Antenna 2 Ingress FIFO overflow counts
 * 0x3B0 Antenna 1 Ingress FIFO underrun counts
 * 0x3B4 Antenna 2 Ingress FIFO underrun counts
 * 0x3B8 Number of 8 bytes writes to   ingress FIFO antenna 1
 * 0x3BC Number of 8 bytes writes to   ingress FIFO antenna 2
 * 0x3C0 Number of 8 bytes reads  from ingress FIFO antenna 1
 * 0x3C4 Number of 8 bytes writes from ingress FIFO antenna 2
 * 
 * 0x3E0 Ingress path PCIe 64 byte DMA transfers (FPGA to Transcede)
 *
 * </pre>
 */
typedef struct tagRADIOFPGA
{
//
// Common registers for older SISO only and newer MIMO capable
// FPGA
//
/*
 * 0x000-0x00F: FPGA Info
 * 
 */	
//	VUINT16  FpgaRevision;              /**< @brief 0x000-0x001: FPGA revision (minor revision number) */
//	VUINT16  FpgaBuildId;               /**< @brief 0x002-0x003: FPGA Build ID (major revision number, feature set change) */
    VUINT32  FpgaId;                    /**< @brief 0x000-0x003: FPGA ID (32 bits) */
    VUINT8   Reserved004To00F[12];      /**< @brief 0x004-0x00F: Reserved     */
    
/*
 * 0x010-0x017: PCIe control/status
 */    
    VUINT16  PcieReset;                 /**< @brief 0x010-0x011: PCIe Reset (16 bits) */
    VUINT16  PcieStatus;                /**< @brief 0x012-0x013: PCIe Status (16 bits) */
    VUINT8   Reserved014To017[4];       /**< @brief 0x014-0x017: Reserved */
/*
 * 0x018-0x2F: FIfo and interrupt control
 */    
    VUINT16  FifoStatusSel;             /**< @brief 0x018-0x019: FIFO Status Select (16 bits) */
    VUINT16  IntMask;                   /**< @brief 0x01A-0x01B: Interrupt Mask (16 bits) */
    VUINT16  IntStat;                   /**< @brief 0x01C-0x01D: Interrupt Status (16 bits) */
    VUINT8   AdcFifoCtrl;               /**< @brief 0x01E:       ADC FIFO control (8 bits) */
    VUINT8   DacFifoCtrl;               /**< @brief 0x01F:       DAC FIFO control (8 bits) */
    VUINT16  DacToAdcWriteSyncDelay;    /**< @brief 0x020-0x021: DAC to ADC Write Sync delay (16 bits) */
    VUINT8   Reserved022To02F[14];      /**< @brief 0x022-0x02F: Reserved */

/*
 * 0x030-0x0EF: Reserved
 */    
    VUINT8  Reserved030To0EF[0xC0];     /**< @brief 0x030-0x00FF: Reserved */
    
/*
 * 0x0F0-0x0F3: Internal test revision
 */
    VUINT32 InternalTestRevision;       /**< @brief 0x0F0-0x0F3: Internal Test revision (MSPD use only) */

/*
 * 0x0F3-0xFF: Reserved
 */
    
    VUINT8  Reserved0F3To0FF[12];

/*
 * 0x0100-0x11F: Radio interface control/status
 */    
    VUINT16  Rad0Ctrl;                  /**< @brief 0x100-0x101: Radio interface 0 control (16 bits) */
    VUINT16  Rad0TxSampleCnt;           /**< @brief 0x102-0x103: Radio interface 0 transmit sample count (16 bits), Reserved */
    VUINT8   Rad0Reserved[12];          /**< @brief 0x104-0x10F: Reserved */

    VUINT16  Rad1Ctrl;                  /**< @brief 0x110-0x111: Radio interface 1 control (16 bits), Reserved set to 0x000D */
    VUINT16  Rad1TxSampleCnt;           /**< @brief 0x112-0x113: Radio interface 0 transmit sample count (16 bits), Reserved */
    VUINT8   Rad1Reserved[12];          /**< @brief 0x114-0x11F: Reserved */
    
    VUINT8   Reserved120To1FF[0xE0];    /**< @brief 0x120-0x1FF: Reserved */
    
    VUINT8   PcieMaxTxPayload;          /**< @brief 0x200:       PCIe Maximum Transmit Payload (8 bits), Reserved */
    VUINT8   AxCEnable;                 /**< @brief 0x201:       Radio ADC to PCI function enable (8 bits) Bit0:Antenna 1, Bit1: Antenna2, 1 for enable, 0 for disable */
    VUINT8   PcieNumOutReq;             /**< @brief 0x202:       PCIe Number of Outstanding Requests */
    VUINT8   Reserved203[13];           /**< @brief 0x203-0x20F: Reserved */
    VUINT32  AxCPcieBaseAddr[4];        /**< @brief 0x210-0x21F: Radio ADC to PCI (RX data path) pointers     radio 0 : 2 antennas, radio 1 : 2 antennas (4 x 32 bit memory pointers, last 2 of 4 reserved */ 
    VUINT16  AxCBufSize[4];             /**< @brief 0x220-0x227: Radio ADC to PCI (RX data path) buffer sizes radio 0 : 2 antennas, radio 1 : 2 antennas (4 x 16 bits RX buffer size in number of 32 bit entries, last 2 of 4 reserved */
    VUINT8   AxCReserved228[4];         /**< @brief 0x228-0x22B: Reserved */
//
// Registers only in newer MIMO Capable FPGA
//
    VUINT8   TxRxDataPathEnable;        /**< @brief 0x22C:       Enable TX and RX Data path (0x10:TX, 0x01:RX */
    VUINT8   AxCReserved22D[3];         /**< @brief 0x22D-0x22F: Reserved */
/*
 * Loopback check marker and enable (used for FPGA internal pattern check
 * 
 * 0x230-0x237: Capture RAM Markers 2x32 bits, 1 per capture RAM bank
 * 0x23C Marker check enable                      
 */
    
    VUINT32  CaptureRamMarker[2];       /**< @brief 0x230-0x237: Capture RAM Markers (1 per antenna/path), Reserved for Debug use */
    VUINT8   MarkerReserved238[4];      /**< @brief 0x238-0x23B: Reserved */
    VUINT8   MarkerCheckControl;        /**< @brief 0x23C:       Marker check, 0 to look for marker, Reserved for Debug use */
    VUINT8   MarkerReserved23D[3];      /**< @brief 0x23D-0x23F: Reserved */
/*
 * 0x240-0x26F: FPGA internal capture RAM function registers
 */
    VUINT8   CaptureRamDoneStatus;      /**< @brief 0x240:       Capture RAM Done Status, 1 Done, 0 not done */
    VUINT8   CaptureRamBusyStatus;      /**< @brief 0x241:       Capture RAM Busy Status, 1 Busy, 0 not busy */
    VUINT8   CaptureRamMarker0Status;   /**< @brief 0x242:       Capture RAM Marker 0 Status */
    VUINT8   CaptureRamReserved243[13]; /**< @brief 0x243-0x24F: Reserved */

    VUINT8   CaptureRamBankDumpSelect;  /**< @brief 0x250:       Capture RAM Bank Dump Select, 0 or 1 for Bank 0 or 1 */
    VUINT8   CaptureRamReserved251[15]; /**< @brief 0x251-0x24F: Reserved */
    
    VUINT32  CaptureRamReadData;        /**< @brief 0x260-0x263: 32 bit data from capture ram read 32 bits at a time base on length */
    VUINT16  CaptureRamIndexSelect;     /**< @brief 0x264:       Amount of data to read from Capture RAM Bank */
    VUINT8   CaptureRamReserved265[10]; /**< @brief 0x265:       Reserved */
    VUINT32  AxCTxBaseAddr[2];          /**< @brief 0x270-0x277: Radio TX data path pointers  radio 0 : 2 antennas */
    VUINT16  AxCTxBufSize[2];           /**< @brief 0x278-0x27B: Radio TX data path (DAC) buffer sizes */
    VUINT8   TxPcieMasterEnable;        /**< @brief 0x27C:       PCIe Master Enable for Tx Path */
    VUINT8   PcieIntEnable;             /**< @brief 0x27D:       PCIe Inbound Interrupts Enable */
    VUINT8   Reserved0x27eto0x27f[2];   /**< @brief 0x27E-0x27F: Reserved */
    VUINT32  PcieIntStatAddr;           /**< @brief 0x280:       Reserved */
/*
 * 0x284-0x2FF: Reserved
 */    
    VUINT8   Reserved284to2FF[0x7c];    /**< @brief 0x284-0x2FF: Reserved */
/*  
 * Egress FIFO statistics
 *   
 * 0x300 Egress FIFO 1 overflow counts
 * 0x304 Egress FIFO 2 overflow counts
 * 0x308 Underflow FIFO 1
 * 0x30C Underflow FIFO 2
 * 0x310 Reserved
 * 0x314 Reserved
 * 0x318 Number of 8 bytes writes to egress FIFO antenna 1
 * 0x31C Number of 8 bytes writes to egress FIFO antenna 2
 * 0x320 Egress FIFO 1 to 2 toggle counts (antenna mux)
 * 0x324 Egress FIFO 2 to 1 toggle counts (antenna mux)
 *  
 */
    VUINT32  EgressFifoOverflows  [2]; /**< @brief 0x300-0x307: Egress FIFO overflow  count (for 2 antennas/paths) */
    VUINT32  EgressFifoUnderruns  [2]; /**< @brief 0x308-0x30F: Egress FIFO underflow count (for 2 antennas/paths) */
    VUINT32  ReservedStats310     [2]; /**< @brief 0x310-0x317: Reserved statistics */
    VUINT32  EgressFifo8ByteWrites[2]; /**< @brief 0x318-0x31F: Number of 8 byte writes to egress FIFO */
    VUINT32  EgressFifo1To2Toggles[2]; /**< @brief 0x320-0x327: Egress FIFO 1 to 2 toggle counts (antenna mux) */
    VUINT32  ReservedStats328     [2]; /**< @brief 0x328-0x32F: Reserved statistics */
/*
 * 0x330-0x36F: reserved
 */
    VUINT8   Reserved330To36F[0x40];   /**< @brief 0x330-0x36F: Reserved */
    
/*
 * 0x370-0x37F Egress direction (Transcede to FPGA) PCIe statistics
 * 
 * 0x370 Egress PCIe 8 byte transfer counts
 * 0x374 Egress PCIe Start of DMA counts
 * 0x378 Egress PCIe End of frame counts (i.e. end of DMA transfer, should match with start)
 * 0x37C Reserved
 */        
    VUINT32  EgressPCIeTransfers;      /**< @brief 0x370-0x373: Egress PCIe 8 byte transfer counts */
    VUINT32  EgressPCIeSOFs;           /**< @brief 0x374-0x377: Egress PCIe Start of DMA counts */
    VUINT32  EgressPCIeEOFs;           /**< @brief 0x378-0x37B: Egress PCIe End of frame counts (i.e. end of DMA transfer, should match with start)*/
    VUINT32  EgressPCIeReserved37C;    /**< @brief 0x37C-0x37F: Reserved */
    
/*
 * 0x380-0x383: Built in Self test (BIST) control/status registers
 * 0x380 BIST enable (1 enable, 0 disable)
 * 0x381 BIST status 1 (0 while runnng, non-zero when done)
 * 0x382 BIST status 2
 * 0x383 Internal generator checker mode: enabling (BIST status 3?)
 *
 */    
    VUINT8   BistTestEnable;            /**< @brief 0x380: BIST test enable (1 enable, 0 disable) */
    VUINT8   BistTestRunning;           /**< @brief 0x381: BIST test running (1 running, 0 complete) */
    VUINT8   BistTestStatus;            /**< @brief 0x382: BIST test status */
    VUINT8   BistTestStartCompare;      /**< @brief 0x383: internal generator checker mode, 1 to enable */
    
/*
 * 0x384-0x38B: DLL control/status registers
 *
 * 0x384        DLL Lock status register (0 if locked, 1 if not)
 * 0x385-0x387: Reserved
 * 0x388:       DLL control 1
 * 0x389-0x38B: Reserved
 */
    VUINT8   DllLockStatus;             /**< @brief 0x384: DLL locked status (0 for locked, non-zero for not locked) */
    VUINT8   DllReserved385[3];         /**< @brief 0x385-0x387: Reserved */
    VUINT8   DllControl;                /**< @brief 0x388: DLL control */                     
    VUINT8   DllReserved389[3];         /**< @brief 0x389-0x38B: Reserved */
    
/* 
 * 0x38C-0x38F: Internal FPGA loopback
 * 
 * 0x38C:       FPGA internal loopback control (TX to RX right before radio interface)
 * 0x38D-0x38F: Reserved
 */
    VUINT8   FpgaLoopbackControl;      /**< @brief 0x38C: FPGA loopback control, 1 to enable, 0 to disable */
    VUINT8   FpgaLoopbackReserved[3];  /**< @brief 0x38D-0x38F: Reserved */
/*
 * 0x390       Number of 8 byte reads from egress FIFO antenna 1
 * 0x394       "" antenna 2
 * 0x398-0x39F Reserved
 */    
    VUINT32  EgressFifo8ByteReads[2]; /**< @brief 0x390-0x397: Number of 8 byte reads from egress FIFO (TX to radio) */
    VUINT8   EgressFifoReserved398[8];/**< @brief 0x398-0x39F: Reserved */
    
/*
 * 0x3A0-0x3EF: Reserved
 */
    VUINT8   Reserved3A0[0x50];       /**< @brief 0x3A0-0x3EF: Reserved */        
    
    VUINT8   FpgaSoftReset;           /**< @brief 0x3F0: Reset register, 0xFF to set reset, any other value to release reset */
    VUINT8   Reserved3F1[15];         /**< @brief 0x3F1-0x3FF: Reserved */
    VUINT32  FpgaDebugRegisters[8];   /**< @brief 0x400-0x41F: Debug registers for dumping Misc FPGA debug info */

} RADIOFPGA, *PRADIOFPGA;


typedef struct _EvmCfg_
{
    UINT32 CtrlMask;    // See: EVM_FPGA_CTRL_xxx
    LPVOID pRxCbCtx;
    LPVOID pTxCbCtx;

    void  (*pRxDMACallBack)(void * pCtx, UINT32 nChID, PUINT8 pRxData, UINT32 nSize);
    void  (*pTxDMACallBack)(void * pCtx, UINT32 nChID, PUINT8 pTxData, UINT32 nSize);

    UINT32          AntennaNum; // The number of antennas used with ADI: 1, 2, 4
    UINT32          IntProfID;  // Interrupt profile : see EVM_FPGA_INT_PROF_xxx 

    PUINT8          pTxLog[4];
    PUINT8          pRxLog[4];
    UINT32          TxLogSize;
    UINT32          RxLogSize;

    UINT32          PcieLines;

    UINT32      TxIrqGpioEna;
    UINT32      TxIrqGpioPort;
    UINT32      TxIrqGpioToggleDelays;
    UINT32      RxIrqGpioEna;
    UINT32      RxIrqGpioPort;
    UINT32      RxIrqGpioToggleDelays;

}EvmCfg, *PEvmCfg;

typedef struct _EvmFPGACtx_
{
    BOOL            bInited;
    BOOL            bReinitTimerOnRx;
    UINT32          MasterCpu;
    VUINT32         MaskCtrl;
    RADIOFPGA*      Regs;
    UINT32          FpgaID;        // FPGA code id, it's stored here to prevent usage of mapped FPGA memory very often

    UINT32          AntennaNum;    // The number of antennas: 1, 2, 4
    UINT32          IntProfID;  // Interrupt profile : see EVM_FPGA_INT_PROF_xxx 

    VUINT32         IrqTxTicks;    // Number of ticks between two TX IRQs
    VUINT32         IrqTxPeriod;   // Number of ticks between two TX IRQs
    VUINT32         MaxTxPeriod;   // Number of ticks between two TX IRQs

    VUINT32         IrqTxSubTicks; // Number of ticks between two TX IRQs
    VUINT32         IrqTxSubPeriod;// Number of ticks between two TX IRQs
    VUINT32         MaxTxSubPeriod;// Number of ticks between two TX IRQs

    VUINT32         IrqSubTxPeriodSum;
    VUINT32         IrqTxPeriodSum;
    VUINT32         IrqRxPeriodSum;

    VUINT32         IrqRxTicks;    // Number of ticks between two RX IRQs
    VUINT32         IrqRxPeriod;   // Number of ticks between two RX IRQs
    VUINT32         MaxRxPeriod;

    VUINT32         TxMdmaTicks;
    VUINT32         TxMdmaPeriod;
    VUINT32         MaxTxMdmaPeriod;
    VUINT32         TxMdmaComplete;
    VUINT32         TxMdmaOverrun;

    PUINT8          PCIeBaseMem;
    VUINT32*        PCIIntStatusPtr;    // The pointer to the buffer where FPGA:PCI HW sets int status
    VUINT32         MdmaIsDone;         // Mainly used in PCIe Master mode 

    PUINT8          RxBuf;      // The pointer to the RX buffer (aligned on 4K)
    PUINT8          TxBuf;      // The pointer to the TX buffer

    UINT32          RxBufSize;  // The size of RX buffer in bytes
    UINT32          TxBufSize;  // The size of TX buffer in bytes

    UINT32          RxBufPos;   // The RX buffer possition to read alread eceived data
    UINT32          TxBufPos;   // The TX buffer possition to write data to PCIe memory

    void            (*pRxDMACallBack)(void * pCtx, UINT32 nChID, PUINT8 pRxData, UINT32 nSize);
    void            (*pTxDMACallBack)(void * pCtx, UINT32 nChID, PUINT8 pTxData, UINT32 nSize);

    LPVOID          pRxCbCtx;
    LPVOID          pTxCbCtx;

    UINT32          RxIrqNum;
    UINT32          TxIrqNum;

    PUINT8          pTxLog[4];
    UINT32          TxLogPos;
    UINT32          TxLogSize;

    PUINT8          pRxLog[4];
    UINT32          RxLogPos;
    UINT32          RxLogSize;

    VUINT32         StopIrq;

    UINT32          TxIrqGpioEna;
    UINT32          TxIrqGpioPort;
    UINT32          RxIrqGpioEna;
    UINT32          RxIrqGpioPort;
    UINT32          TxIrqGpioToggleDelays[2];
    UINT32          RxIrqGpioToggleDelays[2];
    UINT32          TxIrqGpioToggleCntr;
    UINT32          RxIrqGpioToggleCntr;
    UINT32          TxIrqGpioToggleDelayIdx;
    UINT32          RxIrqGpioToggleDelayIdx;

}EvmFpgaCtx;

/** @brief This function should be called to initialize FPGA radio module
           and start using radio board

    @param pCfg [in] - the configuration parameters
    @return [MXRC] an error code */

MXRC EvmFpgaInit(PEvmCfg pCfg);

EvmFpgaCtx * EvmFpgaGetCtx (void);

/** @brief This function just stops RX and TX operations
            and can be called when device is inited

    @return [MXRC] an error code */

MXRC EvmFpgaStopRadio(void);

/** @brief This function start TX/RX operations and has to be called
            after EVM initialization (see: EvmFpgaInit) if auto start was not
            specified

    @return [MXRC] an error code */

MXRC EvmFpgaStart (void);

/** @brief This function returns the pointer to the TX buffer used to send data
    @param nAntenna [in] - an antenna ID
    @return [void*] the pointer to the buffer

    @NOTE : The EVM module should be initializeds */

void * EvmGetTxBuffer (UINT32 nAntenna);

/** @brief This function returns the pointer to the TX buffer used to send data
    @param nAntenna [in] - an antenna ID
    @return [void*] the pointer to the buffer

    @NOTE : The EVM module should be initializeds */

void * EvmGetTxBufferEx(LPVOID pPtr, UINT32 nAntenna);

/** @brief This function returns the pointer to the RX buffer used to receive data
    @param nAntenna [in] - an antenna ID
    @return [void*] the pointer to the buffer

    @NOTE : The EVM module should be initializeds */

void * EvmGetRxBuffer (UINT32 nAntenna);

/** @brief This function just stops RX operation
            and can be called when device is inited

    @param nAntennaID [in] - the antenna ID

    @return [MXRC] an error code */

MXRC EvmFpgaStopRx(UINT32 nAntennaID);

/** @brief The fnction returns CpuID where this modle was initialized
    @return [UINT32] [0..3] */

UINT32 EvmFpgaGetMasterCpu (void);

/** @brief This function should stop the device and free used buffers
    @return [MXRC] an error code */

MXRC EvmFpgaShutdown (void);

MXRC EvmFpgaEnableCtrl (UINT32 nCtrl);

MXRC EvmFpgaDisableCtrl (UINT32 nCtrl);

UINT32 EvmFpgaGetCtrl (void);

MXRC EvmFpgaSetCtrl (UINT32 nCtrl);

UINT32 EvmFpgaGetMaxRxIrqPeriod (void);

UINT32 EvmFpgaGetMaxTxIrqPeriod (void);

UINT32 EvmFpgaGetMaxTxIrqSubPeriod (void);

UINT32 EvmFpgaGetMaxTxMdmaPeriod (void);

UINT32 EvmFpgaGetTxMdmaPeriod (void);

MXRC EvmFpgaResetTxLog(void);

MXRC EvmFpgaResetRxLog(void);

UINT32 EvmFpgaGetRxIrqNum(void);

UINT32 EvmFpgaGetTxIrqNum(void);

UINT32 EvmFpgaGetRxLogSize(void);

UINT32 EvmFpgaGetTxLogSize(void);

MXRC EvmFpgaCfgApi(ApiHeader *pApi, ApiHeader *pResp);

void EvmFpgaMmuCfg(UINT32 nCpuID, UINT32 nParam);

#if EVM_FPGA_TIMER_TRACE_NUM > 0
UINT32 EvmFpgaTraceTimerGetTime(UINT32 * pIter);
#endif

#if EVM_FPGA_TX_TIME_TRACE_NUM > 0
UINT32 EvmFpgaTxTraceGetTime (UINT32 *pIter);
#endif

#if EVM_FPGA_RX_TIME_TRACE_NUM > 0
UINT32 EvmFpgaRxTraceGetTime (UINT32 *pIter);
#endif

#endif /*__EVMFPGA_H__*/

#ifdef __cplusplus
}
#endif

