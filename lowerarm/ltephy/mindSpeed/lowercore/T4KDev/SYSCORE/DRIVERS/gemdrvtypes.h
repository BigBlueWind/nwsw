//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL gemdrv.h
 *
 * @brief The Cadence GEM driver
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
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

#ifndef _GEM_DRV_TYPES_H_
#define _GEM_DRV_TYPES_H_

#include "4gmx.h"
#include "appids.h"
#include "hal.h"

/******************************************************************************
*       Definition of the Cadence GEM registers map                           *
*******************************************************************************/

// Network Control Register

#define GEM_NETCTRL_TX_LB               (1 << 0)
#define GEM_NETCTRL_RX_LB               (1 << 1)
#define GEM_NETCTRL_RX_ENB              (1 << 2)
#define GEM_NETCTRL_TX_ENB              (1 << 3)
#define GEM_NETCTRL_MDIO_ENB            (1 << 4)
#define GEM_NETCTRL_CLR_STAT            (1 << 5)
#define GEM_NETCTRL_INC_STAT            (1 << 6)
#define GEM_NETCTRL_STAT_WR_ENB         (1 << 7)
#define GEM_NETCTRL_START_TX            (1 << 9)
#define GEM_NETCTRL_HALT_TX             (1 << 10)
#define GEM_NETCTRL_TX_PAUSE            (1 << 11)
#define GEM_NETCTRL_TX_ZERO_PAUSE       (1 << 12)
#define GEM_NETCTRL_SNAPSHOT            (1 << 13)
#define GEM_NETCTRL_READ_SNAPSHOT       (1 << 14)
#define GEM_NETCTRL_STORE_TIMESTAMP     (1 << 15)

// Network Configuration Register

#define GEM_NETCFG_100BM                (1 << 0)    // Speed - set to logic one to indicate 100Mbps
#define GEM_NETCFG_FULL_DUPLEX          (1 << 1)    // Full duplex
#define GEM_NETCFG_DISCARD_NON_VLAN     (1 << 2)    // Discard non-VLAN frames
#define GEM_NETCFG_JUMBO_ENB            (1 << 3)    // Jumbo frames
#define GEM_NETCFG_COPY_ALL             (1 << 4)    // Copy all frames
#define GEM_NETCFG_NO_BROADCAST         (1 << 5)    // No broadcast
#define GEM_NETCFG_MULTICAST_ENB        (1 << 6)    // Multicast hash enable
#define GEM_NETCFG_UNICAST_ENB          (1 << 7)    // Unicast hash enable
#define GEM_NETCFG_1536_ENB             (1 << 8)    // Receive 1536 byte frames
#define GEM_NETCFG_EXT_ADDR_ENB         (1 << 9)    // External address match enable
#define GEM_NETCFG_1GB_MODE             (1 << 10)   // setting this bit configures the GEM for 1000 Mbps operation
#define GEM_NETCFG_TBI_ENB              (1 << 11)   // 1: TBI enabled, GMII/MII disabled
#define GEM_NETCFG_PAUSE_ENB            (1 << 13)   // Pause enable
#define GEM_NETCFG_RX_OFFS_MASK         (3 << 14)   // Receive buffer offset
#define GEM_NETCFG_VALID_L3_CHKS        (1 << 14)   // Validate L3 checksum
#define GEM_NETCFG_DROP_BAD_L3          (1 << 15)   // Drop packet in case of bad check
#define GEM_NETCFG_NO_LEN_ERR           (1 << 16)   // Length field error frame discard
#define GEM_NETCFG_FCS_REMOVE           (1 << 17)   // FCS remove
#define GEM_NETCFG_VALID_L4_CHKS        (1 << 21)   // Validate L4 checksum
#define GEM_NETCFG_DROP_BAD_L4          (1 << 22)   // Drop packet in case of bad check
#define GEM_NETCFG_DONOT_COPY_PAUSE     (1 << 23)
#define GEM_NETCFG_CALC_CHKS_ENB        (1 << 24)   // Receive checksum offload enable
#define GEM_NETCFG_RX_HALF_ENB          (1 << 25)   // Enable frames to be received in half-duplex mode while transmitting
#define GEM_NETCFG_IGNORE_RX_FCS        (1 << 26)   // Ignore RX FCS - when set frames with FCS/CRC errors will not be rejected
#define GEM_NETCFG_SGMII_MODE           (1 << 27)   // SGMII mode enable
#define GEM_NETCFG_IPG                  (1 << 28)   // IPG stretch enable
#define GEM_NETCFG_BAD_PREAMBLE         (1 << 29)   // Receive bad preamble
#define GEM_NETCFG_IGNORE_RX_ERR        (1 << 30)   // Ignore RX_ER when RX_DV is low
#define GEM_NETCFG_UNIDIR               (1 << 31)   // Uni-direction-enable

// DMA Configuration Register

#define GEM_DMA_BURST_SINGLE            (1 << 0)    // 00001: Always use SINGLE AHB bursts
#define GEM_DMA_BURST_INCR4             (1 << 2)    // 001xx: Attempt to use INCR4 AHB bursts
#define GEM_DMA_BURST_INCR8             (1 << 3)    // 01xxx: Attempt to use INCR8 AHB bursts
#define GEM_DMA_BURST_INCR16            (1 << 4)    // 1xxxx: Attempt to use INCR16 AHB bursts
#define GEM_DMA_BIG_ENDIAN              (1 << 7)    // AHB endian swap mode enable
#define GEM_DMA_RX_PB_1K                (0 << 8)    // 00: Do not use top three address bits (1 Kb)
#define GEM_DMA_RX_PB_2K                (1 << 8)    // 01: Do not use top two address bits (2 Kb)
#define GEM_DMA_RX_PB_4K                (2 << 8)    // 10: Do not use top address bit (4 Kb)
#define GEM_DMA_RX_PB_8K                (3 << 8)    // 11: Use full configured addressable space (8 Kb)
#define GEM_DMA_TX_PB_2K                (0 << 10)   // 0: Do not use top address bit (2 Kb)
#define GEM_DMA_TX_PB_4K                (1 << 10)   // 1: Use full configured addressable space (4 Kb)
#define GEM_DMA_CHK_SUM_ENB             (1 << 11)   // Transmitter IP, TCP and UDP checksum generation offload enable
#define GEM_DMA_ADM_MGR_ENB             (1 << 12)   // Enable Admittance Manager
#define GEM_DMA_RX_64                   (1 << 16)   // 64 bytes
#define GEM_DMA_RX_128                  (2 << 16)   //
#define GEM_DMA_RX_192                  (3 << 16)   //
#define GEM_DMA_RX_256                  (4 << 16)   //
#define GEM_DMA_RX_320                  (5 << 16)   //
#define GEM_DMA_RX_384                  (6 << 16)   //
#define GEM_DMA_RX_448                  (7 << 16)   //
#define GEM_DMA_RX_512                  (8 << 16)   //
#define GEM_DMA_DEF_RX_SIZE(size)       ((((size) >> 6) & 0xFF) << 16)
#define GEM_DMA_GET_RX_SIZE(dmareg)     ((((dmareg) >> 16) & 0xFF) * 64)
#define GEM_DMA_BREAK_BURST_EARLY       (1 << 24)   // Break Bursts Early on EOP access and when about to cross 1k boundary
#define GEM_DMA_RX_SW_ALLOC             (1 << 25)   // Enable RX DMA Software Buffer Allocation Mode
#define GEM_DMA_TX_SW_ALLOC             (1 << 26)   // Enable TX DMA Software Buffer Allocation Mode
#define GEM_DMA_START_BURST_16B         (1 << 27)   // Start Bursts on 16 byte boundary enable
#define GEM_DMA_SCHEDULER_ENB           ((UINT32)1L << 31)

// Transmit Status Register

#define GEM_TX_STAT_USED_WAS_READ       (1 << 0)    // Used bit read
#define GEM_TX_STAT_COLLISION           (1 << 1)    // Collision occurred
#define GEM_TX_STAT_RETRY_EXCEEDED      (1 << 2)    // Retry limit exceeded
#define GEM_TX_STAT_GO                  (1 << 3)    // Transmit go, if high transmit is active
#define GEM_TX_STAT_AHB_ERR             (1 << 4)    // Transmit frame corruption due to AHB error
#define GEM_TX_STAT_COMPLETE            (1 << 5)    // Transmit complete - set when a frame has been transmitted
#define GEM_TX_STAT_UNDERRUN            (1 << 6)    // Transmit under run
#define GEM_TX_STAT_LATE_COLLISION      (1 << 7)    // Late collision occurred
#define GEM_TX_STAT_HRESP_ERR           (1 << 8)    // Hresp not OK - set when the DMA block sees hresp not OK

#define GEM_TX_ERROR_MASK               (0                          |\
                                        GEM_TX_STAT_COLLISION       |\
                                        GEM_TX_STAT_RETRY_EXCEEDED  |\
                                        GEM_TX_STAT_AHB_ERR         |\
                                        GEM_TX_STAT_UNDERRUN        |\
                                        GEM_TX_STAT_LATE_COLLISION  |\
                                        GEM_TX_STAT_HRESP_ERR       |\
                                        0)


// Receive Status Register

#define GEM_RX_STAT_NO_BUF              (1 << 0)    // Buffer not available
#define GEM_RX_STAT_FRM_RECV            (1 << 1)    // Frame received
#define GEM_RX_STAT_OVERRUN             (1 << 2)    // Receive overrun
#define GEM_RX_STAT_HRESP_ERR           (1 << 3)    // Hresp not OK

#define GEM_RX_ERROR_MASK               (0                          |\
                                        GEM_RX_STAT_NO_BUF          |\
                                        GEM_RX_STAT_OVERRUN         |\
                                        GEM_RX_STAT_HRESP_ERR       |\
                                        0)

// Interrupt Status  Register
// Interrupt Enable  Register
// Interrupt Disable Register
// Interrupt Mask    Register

#define GEM_INT_PHY_COMPLETE            (1 << 0)    // Management frame sent - the PHY maintenance register has completed its operation
#define GEM_INT_RX_COMPLETE             (1 << 1)    // Receive complete - a frame has been stored in memory
#define GEM_INT_RX_USED_BIT_READ        (1 << 2)    // RX used bit read - set when a receive buffer descriptor is read with its used bit set
#define GEM_INT_TX_USED_BIT_READ        (1 << 3)    // TX used bit read - set when a transmit buffer descriptor is read with its used bit set
#define GEM_INT_TX_UNDERRUN             (1 << 4)    // Transmit under run
#define GEM_INT_RETRY_EXCEEDED          (1 << 5)    // Retry limit exceeded or late collision - transmit error
#define GEM_INT_AHB_ERR                 (1 << 6)    // Transmit frame corruption due to AHB error
#define GEM_INT_TX_COMPLETE             (1 << 7)    // Transmit complete
#define GEM_INT_LINK_CHANGE             (1 << 9)    // Link change
#define GEM_INT_RX_OVERRUN              (1 << 10)   // Receive overrun
#define GEM_INT_HRESP_ERR               (1 << 11)   // Hresp not OK
#define GEM_INT_RX_PAUSE_FRM            (1 << 12)   // Pause frame with non-zero pause quantum received
#define GEM_INT_PAUSE_ZERO              (1 << 13)   // Pause time zero
#define GEM_INT_TX_PAUSE_FRM            (1 << 14)   // Pause frame transmitted
#define GEM_INT_EXTERNAL                (1 << 15)   // External interrupt
#define GEM_INT_PCS_COPLETE             (1 << 16)   // PCS auto-negotiation complete
#define GEM_INT_PCS_LINK_RECEIVED       (1 << 17)   // PCS link partner page received
#define GEM_INT_TX_QUEUE_ID_MASK        (0xFF << 20)// TX QUEUE Identification


#define GEM_INT_TSU_RX_PTP_DLY_REQ      (1 << 18)   // PTP delay_req frame received
#define GEM_INT_TSU_RX_PTP_SYNC_FRM     (1 << 19)   // PTP sync frame received
#define GEM_INT_TSU_RX_PTP_PDLY_REQ     (1 << 20)   // PTP pdelay_req frame received
#define GEM_INT_TSU_RX_PTP_PDLY_RESP    (1 << 21)   // PTP pdelay_resp frame received
#define GEM_INT_TSU_TX_PTP_DLY_REQ      (1 << 22)   // PTP delay_req frame transmitted
#define GEM_INT_TSU_TX_SYNC_FRM         (1 << 23)   // PTP sync frame transmitted
#define GEM_INT_TSU_TX_PDLY_REQ         (1 << 24)   // PTP pdelay_req frame transmitted
#define GEM_INT_TSU_TX_PDLY_RESP        (1 << 25)   // PTP pdelay_resp frame transmitted



/** @brief This structure describes the registers map of Cadence GEM

    based on the "GEM_UserGuide_d12.pdf" document */

typedef struct tagGEMRegs
{
    // The offset is 0x00

    VUINT32     NetCtrl;            // Network control
    VUINT32     NetCfg;             // Network configuration
    VUINT32     NetStat;            // Network status
    VUINT32     UserIO;             // User input/output
    VUINT32     DMACfg;             // DMA configuration
    VUINT32     TxStatus;           // Transmit status
    VUINT32     RxQueue;            // Receive buffer queue base address

    // Changed from TxQueue
    VUINT32     RxGlobalOffs;       // RX Global Offset

    VUINT32     RxStatus;           // Receive status
    VUINT32     IntStat;            // Interrupt status
    VUINT32     IntEnable;          // Interrupt enable
    VUINT32     IntDisable;         // Interrupt disable
    VUINT32     IntMask;            // Interrupt mask

    // Offset is 0x34
    VUINT32     PHY;                // PHY maintenance

    VUINT32     RxPauseQuantum;     // Received pause quantum
    VUINT32     TxPauseQuantum;     // Transmit pause quantum

    VUINT32     Res1 [16];

    // The offset is 0x80

    VUINT32     HashRegBottom;      // Hash register bottom [31:0]
    VUINT32     HashRegTop;         // Hash register top [63:32]
    VUINT32     SpecAddr1Bottom;    // Specific address 1 bottom [31:0]
    VUINT32     SpecAddr1Top;       // Specific address 1 top [47:32]
    VUINT32     SpecAddr2Bottom;    // Specific address 2 bottom [31:0]
    VUINT32     SpecAddr2Top;       // Specific address 2 top [47:32]
    VUINT32     SpecAddr3Bottom;    // Specific address 3 bottom [31:0]
    VUINT32     SpecAddr3Top;       // Specific address 3 top [47:32]
    VUINT32     SpecAddr4Bottom;    // Specific address 4 bottom [31:0]
    VUINT32     SpecAddr4Top;       // Specific address 4 top [47:32]

    // The offset is 0xA8
    VUINT32     TypeID [4];

    // The offset is 0xB8
    VUINT32     WakeOnLAN;          // Wake on LAN

    // The offset is 0xBC
    VUINT32     IPGStretch;         // IPG stretch
    VUINT32     StackedVLAN;        // Stacked VLAN

    VUINT32     Res2 [14];

    // The offset is 0xFC
    VUINT32     ModuleID;           // Module ID

    // The offset is 0x100

    // -----------------------------------------------
    // TX statistics  (TxS)
    // -----------------------------------------------
    VUINT32     TxSOctetsLow;       // Octets transmitted [31:0] (in frames without error)
    VUINT32     TxSOctetsHigh;      // Octets transmitted [47:32] (in frames without error)
    VUINT32     TxSFrmNum;          // Frames transmitted without error, excluding pause frames
    VUINT32     TxSBrdcastNum;      // Broadcast frames transmitted without error, excluding pause frames
    VUINT32     TxSMulcastNum;      // Multicast frames transmitted without error,excluding pause frames
    VUINT32     TxSPauseNum;        // Transmitted pause frames (unless sent through external FIFO interface)
    VUINT32     TxSFrm_64Num;       // 64 byte frames transmitted without error, excluding pause frames
    VUINT32     TxSFrm_65_127Num;   // 65 to 127 byte frames transmitted without error
    VUINT32     TxSFrm_128_255Num;  // 128 to 255 byte frames transmitted without error
    VUINT32     TxSFrm_256_511Num;  // 256 to 511 byte frames transmitted without error
    VUINT32     TxSFrm_512_1023Num; // 512 to 1023 byte frames transmitted without error
    VUINT32     TxSFrm_1024_1518Num;// 1024 to 1518 byte frames transmitted without error
    VUINT32     TxSFrm_over1518Num; // Greater than 1518 byte frames transmitted without error
    VUINT32     TxSUndrNum;         // Transmit under run errors
    VUINT32     TxSSingleCollNum;   // Single collision frames
    VUINT32     TxSMulCollNum;      // Multiple collision frames
    VUINT32     TxSExcCollNum;      // Excessive collisions
    VUINT32     TxSLateCollNum;     // Late collisions
    VUINT32     TxSDeferredNum;     // Deferred transmission frames
    VUINT32     TxSCrSenseNum;      // Carrier sense errors

    // -----------------------------------------------
    // RX statistics  (RxS)
    // -----------------------------------------------
    VUINT32     RxSOctetsLow;       // Octets received [31:0] (in frames without error)
    VUINT32     RxSOctetsHigh;      // Octets received [47:32] (in frames without error)
    VUINT32     RxSFrmNum;          // Frames received without error, excluding pause frames
    VUINT32     RxSBrdcastNum;      // Broadcast frames received without error, excluding pause frames
    VUINT32     RxSMulcastNum;      // Multicast frames received without error,excluding pause frames
    VUINT32     RxSPauseNum;        // received pause frames (unless sent through external FIFO interface)
    VUINT32     RxSFrm_64Num;       // 64 byte frames transmitted without error, excluding pause frames
    VUINT32     RxSFrm_65_127Num;   // 65 to 127 byte frames received without error
    VUINT32     RxSFrm_128_255Num;  // 128 to 255 byte frames received without error
    VUINT32     RxSFrm_256_511Num;  // 256 to 511 byte frames received without error
    VUINT32     RxSFrm_512_1023Num; // 512 to 1023 byte frames received without error
    VUINT32     RxSFrm_1024_1518Num;// 1024 to 1518 byte frames received without error
    VUINT32     RxSFrm_over1518Num; // Greater than 1518 byte frames received without error
    VUINT32     RxSUndersizeNum;    // Undersize frames received
    VUINT32     RxSOversizeNum;     // Oversize frames received
    VUINT32     RxSJabbersNum;      // Jabbers received
    VUINT32     RxSFrmChkSeqErr;    // Frame check sequence errors
    VUINT32     RxSLenErr;          // Length field frame errors
    VUINT32     RxSRecvSymErr;      // Receive symbol errors
    VUINT32     RxSAlignErr;        // Alignment errors
    VUINT32     RxSRecvResErr;      // Receive resource errors
    VUINT32     RxSRecvOvrErr;      // Receive overrun errors
    VUINT32     RxSIPChkSumErr;     // IP header checksum errors
    VUINT32     RxSTCPChkSumErr;    // TCP checksum errors
    VUINT32     RxSUDPChkSumErr;    // UDP checksum errors

	VUINT32     Res3[7];

    // The offset is 0x1D0
    VUINT32     TimerSec;           // 1588 timer seconds
    VUINT32     TimerNanosec;       // 1588 timer nanoseconds
    VUINT32     TimerAdjust;        // 1588 timer adjust
    VUINT32     TimerIncr;          // 1588 timer increment

    // The offset is 0x1E0
    VUINT32     PTPTxSec;           // PTP event frame transmitted seconds
    VUINT32     PTPTxNanoSec;       // PTP event frame transmitted nanoseconds
    VUINT32     PTPRxSec;           // PTP event frame received seconds
    VUINT32     PTPRxNanoSec;       // PTP event frame received nanoseconds
    VUINT32     PTPPeerTxSec;       // PTP peer event frame transmitted seconds
    VUINT32     PTPPeerTxNanoSec;   // PTP peer event frame transmitted nanoseconds
    VUINT32     PTPPeerRxSec;       // PTP peer event frame received seconds
    VUINT32     PTPPeerRxNanoSec;   // PTP peer event frame received nanoseconds

    // The offset is 0x200
    VUINT32     PCSCtrl;            // PCS control
    VUINT32     PCSStat;            // PCS status
    VUINT32     PCSHighPhyID;       // PCS upper PHY identifier
    VUINT32     PCSLowPhyID;        // PCS lower PHY identifier
    VUINT32     PCSAdv;             // PCS auto-negotiation advertisement
    VUINT32     PCSLink;            // PCS auto-negotiation link partner ability
    VUINT32     PCSExp;             // PCS auto-negotiation expansion
    VUINT32     PCSNextPage;        // PCS auto-negotiation next page
    VUINT32     PCSLinkNextPage;    // PCS auto-negotiation link partner next page

    VUINT32     Res4 [6];

    // Offset 0x23C
    VUINT32     PCSExtStat;         // PCS extended status

    // Offset 0x240
    VUINT32     Res5 [48];

    // Offset 0x300
    VUINT32     TxQueue[8];         // Transmit buffer queue base address

    // Additional TypeID,
	// offset is 0x320
    VUINT32     ExtTypeID [4];      // Type ID match 5 ... Type ID match 8

    // Offset 0x330
    VUINT32     TXPBUF;             // TX PBUF watermark

    // Offset 0x334
    VUINT32     TSUStatus;
    VUINT32     TSUEnable;
    VUINT32     TSUDisable;
    VUINT32     TSUMask;

}GEMRegs, *PGEMRegs;


#define GEM_CFG_MODE_SEL_PIN                (0 << 0)
#define GEM_CFG_MODE_SEL_GEM                (1 << 0)
#define GEM_CFG_MODE_GEM_MASK               (7 << 1)
#define GEM_CFG_MODE_GEM_MII                (0 << 1)
#define GEM_CFG_MODE_GEM_GMII               (1 << 1)
#define GEM_CFG_MODE_GEM_RMII               (2 << 1)
#define GEM_CFG_MODE_GEM_RGMII              (3 << 1)
#define GEM_CFG_MODE_GEM_SGMII              (4 << 1)
#define GEM_CFG_MODE_GEM_SMII               (6 << 1)
#define GEM_CFG_MODE_GEM_TBI                (7 << 1)

#define GEM_CFG_MODE_PIN_MASK               (7 << 4)
#define GEM_CFG_MODE_PIN_MII                (0 << 4)
#define GEM_CFG_MODE_PIN_GMII               (1 << 4)
#define GEM_CFG_MODE_PIN_RMII               (2 << 4)
#define GEM_CFG_MODE_PIN_RGMII              (3 << 4)
#define GEM_CFG_MODE_PIN_SGMII              (4 << 4)
#define GEM_CFG_MODE_PIN_SMII               (6 << 4)
#define GEM_CFG_MODE_PIN_TBI                (7 << 4)

#define GEM_CFG_DUPLEX_SEL_PHY              (0 << 8)
#define GEM_CFG_DUPLEX_SEL_GEM              (1 << 8)
#define GEM_CFG_DUPLEX_GEM_HALF             (0 << 9)
#define GEM_CFG_DUPLEX_GEM_FULL             (1 << 9)
#define GEM_CFG_DUPLEX_PHY_HALF             (0 << 10)
#define GEM_CFG_DUPLEX_PHY_FULL             (1 << 10)
#define GEM_CFG_SPEED_SEL_PHY               (0 << 11)
#define GEM_CFG_SPEED_SEL_GEM               (1 << 11)
#define GEM_CFG_SPEED_MASK                  (3 << 12)
#define GEM_CFG_SPEED_GEM_10M               (0 << 12)
#define GEM_CFG_SPEED_GEM_100M              (1 << 12)
#define GEM_CFG_SPEED_GEM_1G                (2 << 12)
#define GEM_CFG_SPEED_PHY_10M               (0 << 14)
#define GEM_CFG_SPEED_PHY_100M              (1 << 14)
#define GEM_CFG_SPEED_PHY_1G                (2 << 14)
#define GEM_CFG_PHY_LINK_DOWN               (0 << 16)
#define GEM_CFG_PHY_LINK_UP                 (1 << 16)
#define GEM_CFG_GEM_LOOPBACK                (1 << 17)

typedef struct tagGEMCoreRegs
{
    VUINT32     Cfg;        // GEMCORE Configuration Register
    VUINT32     MBIST;      // GEMCORE MBIST Register
    VUINT32     Sense;      // GEMCORE Sense Amp Delay Adjustment Register
    VUINT32     Delay;      // GEMCORE Delay Element Control Register

}GEMCoreRegs, *PGEMCoreRegs;


/******************************************************************************
*       Definition of the GEM DMA buffer descriptor and macros                *
*******************************************************************************/




// RX status flags, WORD-1 of the buffer descriptor

#define GEM_DMA_RX_BROADCAST_BIT            ((UINT32)1L << 31)
#define GEM_DMA_RX_MULTICAST_BIT            (1 << 30)
#define GEM_DMA_RX_UNICAST_BIT              (1 << 29)
#define GEM_DMA_RX_WRAP_BIT                 (1 << 28)
#define GEM_DMA_RX_SPEC_ADDR_MASK           (7 << 25)
#define GEM_DMA_RX_IRQ_ENB                  (1 << 24) // Generate an interrupt after this descriptor is populated
#define GEM_DMA_RX_VLAN_BIT                 (1 << 21)
#define GEM_DMA_RX_PRIOR_BIT                (1 << 20)
#define GEM_DMA_RX_VLAN_PRIOR_MASK          (7 << 17)
#define GEM_DMA_RX_CFI_BIT                  (1 << 16)
#define GEM_DMA_RX_EOF_BIT                  (1 << 15)   // end of frame bit
#define GEM_DMA_RX_SOF_BIT                  (1 << 14)   // start of frame bit
#define GEM_DMA_RX_CRC_STAT                 (1 << 13)
#define GEM_DMA_RX_LEN_MASK                 (0x01FFF)

// RX status flags, WORD-2 of the buffer descriptor

#define GEM_DMA_RX_L4_OFFS_MASK             ((UINT32)0xFFL << 24)
#define GEM_DMA_RX_L3_OFFS_MASK             ((UINT32)0xFFL << 16)
#define GEM_DMA_RX_READY_BIT                (1 << 15)
#define GEM_DMA_RX_L4_TCP                   (1 << 9)
#define GEM_DMA_RX_L4_UDP                   (1 << 8)
#define GEM_DMA_RX_L3_IPV6                  (1 << 7)
#define GEM_DMA_RX_L3_IPV4                  (1 << 6)
#define GEM_DMA_RX_PPPoE                    (1 << 5)
#define GEM_DMA_RX_STACKED_VLAN             (1 << 4)
#define GEM_DMA_RX_TYPE_MASK                (0xF  )


// access to the WORD-1 bits of the RX buffer descriptor

#define GEM_DMA_RX_SET_ADDR(d, addr)        ((d)->AddrOpt = (addr))

#define GEM_DMA_RX_SET_WRAP(d)              ((d)->Descr |= GEM_DMA_RX_WRAP_BIT)
#define GEM_DMA_RX_IS_WRAP(d)               ((d)->Descr & GEM_DMA_RX_WRAP_BIT)
#define GEM_DMA_RX_IS_READY(d)              ((d)->ExDescr & GEM_DMA_RX_READY_BIT)
#define GEM_DMA_RX_SET_READY(d)             ((d)->ExDescr &= ~GEM_DMA_RX_READY_BIT)

#define GEM_DMA_RX_GET_SPEC_ADDR(d)         (((d)->Descr & GEM_DMA_RX_SPEC_ADDR_MASK) >> 25)
#define GEM_DMA_RX_IS_EOF(d)                (((d)->Descr & GEM_DMA_RX_EOF_BIT))
#define GEM_DMA_RX_IS_SOF(d)                (((d)->Descr & GEM_DMA_RX_SOF_BIT))
#define GEM_DMA_RX_CLR_EOF(d)               (((d)->Descr &= ~GEM_DMA_RX_EOF_BIT))
#define GEM_DMA_RX_CLR_SOF(d)               (((d)->Descr &= ~GEM_DMA_RX_SOF_BIT))
#define GEM_DMA_RX_CLR_EOF_SOF(d)           (((d)->Descr &= ~(GEM_DMA_RX_SOF_BIT | GEM_DMA_RX_EOF_BIT)))
#define GEM_DMA_RX_GET_LEN(d)               (((d)->Descr & GEM_DMA_RX_LEN_MASK))
#define GEM_DMA_RX_CLR_STATUS(d)            (d)->Descr = ((d)->Descr & (GEM_DMA_RX_WRAP_BIT | GEM_DMA_RX_IRQ_ENB)); (d)->ExDescr = 0;
#define GEM_DMA_RX_GET_MAC_MATCH(d)         (((d)->Descr & GEM_DMA_RX_SPEC_ADDR_MASK) >> 25)
#define GEM_DMA_RX_IS_UC(d)                 ((d)->Descr & (GEM_DMA_RX_UNICAST_BIT))
#define GEM_DMA_RX_IS_MC(d)                 ((d)->Descr & (GEM_DMA_RX_MULTICAST_BIT))
#define GEM_DMA_RX_IS_BC(d)                 ((d)->Descr & (GEM_DMA_RX_BROADCAST_BIT))



// access to the WORD-2 bits of the RX buffer descriptor

#define GEM_DMA_RX_GET_TYPE_ID(d)           (((d)->ExDescr & GEM_DMA_RX_TYPE_MASK))

// TX status flags, WORD-1 of the buffer descriptor

#define GEM_DMA_TX_USED_BIT                 ((UINT32)1L << 31)   // Used - must be zero for the GEM to read data to the transmit buffer
#define GEM_DMA_TX_WRAP                     (1 << 30)   // Wrap - marks last descriptor in transmit buffer descriptor list
#define GEM_DMA_TX_RETRY_EXCEEDED           (1 << 29)   // Retry limit exceeded, transmit error detected
#define GEM_DMA_TX_IRQ_ENB                  (1 << 29)   // Global TX Interrupts Enable
#define GEM_DMA_TX_FRM_CORRUPT              (1 << 28)   // Transmit Frame Corruption
#define GEM_DMA_TX_LATE_COLLISION           (1 << 27)   // Late collision, transmit error detected
#define GEM_DMA_TX_NO_L3_L4                 (0 << 25)   // Do Not Generate L3 or L4 header checksums
#define GEM_DMA_TX_L3_ADDED                 (2 << 25)   // Generate L3 checksum
#define GEM_DMA_TX_L4_ADDED                 (1 << 25)   // Generate L4 checksum
#define GEM_DMA_TX_L3_L4_ADDED              (3 << 25)   // Generate both L3 and L4 checksums
#define GEM_DMA_TX_CRC                      (1 << 24)   // Generate CRC for frame
#define GEM_DMA_TX_OFFS_MASK                (0xFF << 16)// Buffer Offset OR TCP/IP checksum generation status
#define GEM_DMA_TX_LAST_BUF                 (1 << 15)   // Last buffer
#define GEM_DMA_TX_LEN_MASK                 (0x01FFF)   // Length of buffer [12:0]

// access to the WORD-1 bits of the TX buffer descriptor

#define GEM_DMA_TX_IS_USED(d)               ((d)->Descr   & GEM_DMA_TX_USED_BIT)
#define GEM_DMA_TX_SET_USED(d)              ((d)->Descr   |= GEM_DMA_TX_USED_BIT)
#define GEM_DMA_TX_IS_SCHEDULED(d)          (!((d)->Descr   & GEM_DMA_TX_USED_BIT))
#define GEM_DMA_TX_CLR_USED(d)              ((d)->Descr   &= ~GEM_DMA_TX_USED_BIT)
#define GEM_DMA_TX_SET_WRAP(d)              ((d)->Descr   |= GEM_DMA_TX_WRAP)
#define GEM_DMA_TX_IS_WRAP(d)               ((d)->Descr   & GEM_DMA_TX_WRAP)
#define GEM_DMA_TX_CHKS_RES(d)              (((d)->Descr >> 16) & 0xFF)
#define GEM_DMA_TX_SET_LAST(d)              ((d)->Descr   |= GEM_DMA_TX_LAST_BUF)
#define GEM_DMA_TX_IS_LAST(d)               ((d)->Descr   & GEM_DMA_TX_LAST_BUF)
#define GEM_DMA_TX_CLR_LAST(d)              ((d)->Descr   &= ~GEM_DMA_TX_LAST_BUF)
#define GEM_DMA_TX_SET_LEN(d, l)            ((d)->AddrOpt = ((l) & GEM_DMA_TX_LEN_MASK))

#define GEM_DMA_TX_IS_ERR(d)                ((d)->Descr & (0                    |\
                                            GEM_DMA_TX_RETRY_EXCEEDED           |\
                                            GEM_DMA_TX_LATE_COLLISION           |\
                                            GEM_DMA_TX_FRM_CORRUPT              |\
                                            GEM_DMA_TX_LATE_COLLISION           |\
                                            0))


/** @brief This type defines GEM DMA TX buffer descriptor,
            the TX descriptors have two 32Bits WORDs:

    - WORD-0:  the address of buffer
    - WORD-1:  the status, configuration bits, other flags */

typedef struct tagGemTxBDescr
{
    VUINT32     AddrOpt;            // The address + some options bits used by the DMA
    VUINT32     Descr;              // The status, flags and parameters of DMA buffer descriptor

}GEMTxBDescr, *PGEMTxBDescr;

/** @brief This type defines GEM DMA RX buffer descriptor,
            the RX buffer descriptors have four 32Bits WORDs:

    - WORD-0:  the address of buffer
    - WORD-1:  the status, configuration bits, other flags
    - WORD-2:  the extended status, configuration bits, other flags
    - WORD-3:  PAD  */

typedef struct tagGemRxBDescr
{
    VUINT32     AddrOpt;            // The address DMA
    VUINT32     Descr;              // The status, flags and parameters of DMA buffer descriptor
    VUINT32     ExDescr;            // The extended status, flags and parameters of DMA buffer descriptor
    VUINT32     Pad;

}GEMRxBDescr, *PGEMRxBDescr;

typedef PGEMRxBDescr HGRxFrm;
typedef PGEMTxBDescr HGTxFrm;


#endif /*_GEM_DRV_TYPES_H_*/


#ifdef __cplusplus
}
#endif

