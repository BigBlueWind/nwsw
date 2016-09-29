/** @file gemac_hal.h
 *
 * @brief GEMAC HAL definitions and interfaces
 * @author Cadence Design Foundry
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * COPYRIGHT(c) 2003 Cadence Design Foundry
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef GEMAC_HAL_H_
#define GEMAC_HAL_H_

#include "resultcodes.h"

/* An enumerated type for loopback values.  This can be one of three values, no
 * loopback -normal operation, local loopback with internal loopback module of
 * MAC or PHY loopback which is through the external PHY.
 */
#ifndef __MAC_LOOP_ENUM__
#define __MAC_LOOP_ENUM__
typedef enum
{
    LB_NONE, LB_EXT, LB_LOCAL
} MAC_LOOP;
#endif

/* The possible operating speeds of the MAC, currently supporting 10, 100 and
 * 1000Mb modes.
 */
#ifndef __MAC_SPEED_ENUM__
#define __MAC_SPEED_ENUM__
typedef enum
{
    SPEED_10M, SPEED_100M, SPEED_1000M, SPEED_1000M_PCS
} MAC_SPEED;
#endif

/* The possible AMBA AHB bus width of the MAC, currently supporting 32, 64 and
 * 128 bits.
 */
#ifndef __MAC_AHB_ENUM__
#define __MAC_AHB_ENUM__
typedef enum
{
    AMBA_AHB_32, AMBA_AHB_64, AMBA_AHB_128
} MAC_AHB_WIDTH;
#endif

/* The possible MDC clock division of the MAC, currently supporting 8, 16 , 32,
 * 48, 64, 96, 128, 224.
 */
#ifndef __MAC_DMCDIV_ENUM__
#define __MAC_DMCDIV_ENUM__
typedef enum
{
    MDC_DIV_8,
    MDC_DIV_16,
    MDC_DIV_32,
    MDC_DIV_48,
    MDC_DIV_64,
    MDC_DIV_96,
    MDC_DIV_128,
    MDC_DIV_224
} MAC_MDC_DIV;
#endif

/* the following PHY related thing should be standard no need to change */
#define PHY_SPEED_1000M             ((1<<9) | (1<<8))
#define PHY_DUPLEX_FULL_1000M       (1<<9)

#define PHY_SPEED_100M              ((1<<8) | (1<<7))
#define PHY_SPEED_10M               ((1<<6) | (1<<5))
#define PHY_DUPLEX_FULL_100M        (1<<8)
#define PHY_DUPLEX_FULL_10M         (1<<6)

/* Specify whether the MDIO should be available, this is set so that for reset
 * function, appropriate options are setup.  To disable, use 0.
 */
#define GEM_MDIO_ENABLED            (GEM_MDIO_EN)

/* Specify the default PCLK divisor for generating MDC.  This simply defines
 * which of the divisor control bits are active.  By default, the clock
 * division is 32.
 */
#define GEM_DEF_PCLK_DIV            (MDC_DIV_32)

/* Specify the default AMBA AHB bus width.  This simply defines
 * which of the bus width control bits are active.  By default, the bus
 * width is 32.
 */
#define GEM_DEF_AHB_WIDTH           (AMBA_AHB_32)

/* Specify default duplex mode, for half duplex - specify 0. */
#ifndef GEM_DEF_DUPLEX
/* Allow the value to be specified as compiler option
 0 - half-duplex,
 GEM_FULL_DUPLEX - full-duplex
 */
#define GEM_DEF_DUPLEX              (GEM_FULL_DUPLEX)
#endif

/* Specify default operating speed, 1 for 100Mb.  Note that this is left
 * shifted.  Also note that this simply asserts a signal to the PHY and has no
 * effect on the operation of the MAC.
 * For 10Mb/s mode, specify         SPEED_10M
 * For 100Mb/s mode, specify        SPEED_100M
 * For 1Gb/s mode, specify          SPEED_1000M
 * For 1Gb/s with PCS mode, specify SPEED_1000M_PCS
 */
#ifndef GEM_DEF_SPEED
/* Allow the value to be specified as compiler option */
#define GEM_DEF_SPEED               (SPEED_1000M)
#endif

/* Specify default loopback mode.  LB_NONE for no loopback, other values are LB_MAC
 * and LB_PHY
 */
#define GEM_DEF_LOOP (LB_NONE)

//
// GEMAC IP wrapper
//

#define	GEM_FIFO_CTRL_TXFF_EN		(1 << 0)
#define	GEM_FIFO_CTRL_HBTXRQ_EN		(1 << 1)
#define	GEM_FIFO_CTRL_RXFF_EN		(1 << 3)
#define	GEM_FIFO_CTRL_HBRXRQ_EN		(1 << 4)
#define	GEM_FIFO_CTRL_TXCP_INH		(1 << 5)
#define	GEM_FIFO_CTRL_RXCMPLTIE		(1 << 11)

#define GEM_CONF_MODE_SEL_PIN		(0 << 0)
#define GEM_CONF_MODE_SEL_GEM		(1 << 0)
#define GEM_CONF_MODE_GEM_MASK		(7 << 1)
#define GEM_CONF_MODE_GEM_MII		(0 << 1)
#define GEM_CONF_MODE_GEM_GMII		(1 << 1)
#define GEM_CONF_MODE_GEM_RMII		(2 << 1)
#define GEM_CONF_MODE_GEM_RGMII		(3 << 1)
#define GEM_CONF_MODE_GEM_SMII		(6 << 1)
#define GEM_CONF_MODE_PIN_MASK		(7 << 4)
#define GEM_CONF_MODE_PIN_MII		(0 << 4)
#define GEM_CONF_MODE_PIN_GMII		(1 << 4)
#define GEM_CONF_MODE_PIN_RMII		(2 << 4)
#define GEM_CONF_MODE_PIN_RGMII		(3 << 4)
#define GEM_CONF_MODE_PIN_SMII		(6 << 4)
#define GEM_CONF_DUPLEX_SEL_PHY		(0 << 8)
#define GEM_CONF_DUPLEX_SEL_GEM		(1 << 8)
#define GEM_CONF_DUPLEX_GEM_HALF	(0 << 9)
#define GEM_CONF_DUPLEX_GEM_FULL	(1 << 9)
#define GEM_CONF_DUPLEX_PHY_HALF	(0 << 10)
#define GEM_CONF_DUPLEX_PHY_FULL	(1 << 10)
#define GEM_CONF_SPEED_SEL_PHY		(0 << 11)
#define GEM_CONF_SPEED_SEL_GEM		(1 << 11)
#define GEM_CONF_SPEED_MASK			(3 << 12)
#define GEM_CONF_SPEED_GEM_10M		(0 << 12)
#define GEM_CONF_SPEED_GEM_100M		(1 << 12)
#define GEM_CONF_SPEED_GEM_1G		(2 << 12)
#define GEM_CONF_SPEED_PHY_10M		(0 << 14)
#define GEM_CONF_SPEED_PHY_100M		(1 << 14)
#define GEM_CONF_SPEED_PHY_1G		(2 << 14)
#define GEM_CONF_PHY_LINK_DOWN		(0 << 16)
#define GEM_CONF_PHY_LINK_UP		(1 << 16)
#define GEM_CONF_GEM_LOOPBACK		(1 << 17)

#define GEM_TXCTRL_DMAIF_EN			(1 << 0)
#define GEM_TXCTRL_CRC_EN			(1 << 1)
#define GEM_TXCTRL_RETR_EN			(1 << 2)
#define GEM_TXCTRL_TX_STATE			0xF0000

#define GEM_RXCTRL_DMAIF_EN			(1 << 0)
#define GEM_RXCTRL_RX_STATE			0xF0000

// Host fifo control bits
#define ARM_FIFO_RXDREQWE			(1 << 2)
#define ARM_FIFO_TXDREQRE			(1 << 3)
#define ARM_FIFO_TXFF_RES			(1 << 12)
#define ARM_FIFO_RXFF_RES			(1 << 13)
#define ARM_FIFO_RXCP_INH			(1 << 15)

//
// Bit positions for network control register
//
#define GEM_READ_SNAP       (1<<14)     /* Read snapshot register */
#define GEM_TAKE_SNAP       (1<<13)     /* Take a snapshot */
#define GEM_TX_0Q_PAUSE     (1<<12)     /* Transmit zero quantum pause frame */
#define GEM_TX_PAUSE        (1<<11)     /* Transmit pause frame */
#define GEM_TX_HALT         (1<<10)     /* Halt transmission after curr frame */
#define GEM_TX_START        (1<<9)      /* Start tx (tx_go) */
#define GEM_STATS_WR_EN     (1<<7)      /* Enable writing to stat registers */
#define GEM_STATS_INC       (1<<6)      /* Increment statistic registers */
#define GEM_STATS_CLR       (1<<5)      /* Clear statistic registers */
#define GEM_MDIO_EN         (1<<4)      /* Enable MDIO port */
#define GEM_TX_EN           (1<<3)      /* Enable transmit circuits */
#define GEM_RX_EN           (1<<2)      /* Enable receive circuits */
#define GEM_LB_MAC          (1<<1)      /* Perform local loopback at MAC */
#define GEM_LB_PHY          (1<<0)      /* Perform ext loopback through PHY */

//
// Bit positions for network configuration register
//
#define GEM_RX_NO_PAUSE     (1<<23)     /* Do not copy pause frames to memory */
#define GEM_AHB_WIDTH1      (1<<22)     /* Bit 1 for defining AHB width */
#define GEM_AHB_WIDTH0      (1<<21)     /* Bit 0 for defining AHB width */
#define GEM_MDC_DIV2        (1<<20)     /* PCLK divisor for MDC, bit 2 */
#define GEM_MDC_DIV1        (1<<19)     /* PCLK divisor for MDC, bit 1 */
#define GEM_MDC_DIV0        (1<<18)     /* PCLK divisor for MDC, bit 0 */
#define GEM_RX_NO_FCS       (1<<17)     /* Discard FCS from received frames. */
#define GEM_RX_LEN_CHK      (1<<16)     /* Receive length check. */
#define GEM_RX_OFFSET_BASE  14          /* Pos of LSB for rx buffer offsets. */
#define GEM_RX_OFFSET1      (1<<(GEM_RX_OFFSET_BASE + 1)) /* RX offset bit 1 */
#define GEM_RX_OFFSET0      (1<<GEM_RX_OFFSET_BASE)       /* RX offset bit 0 */
#define GEM_RX_PAUSE_EN     (1<<13)     /* Enable pause reception */
#define GEM_RETRY_TEST      (1<<12)     /* Retry test for speeding up debug */
#define GEM_PCS_SEL         (1<<11)     /* Select PCS */
#define GEM_GIG_MODE        (1<<10)     /* Gigabit mode enable */
#define GEM_EAM_EN          (1<<9)      /* External address match enable */
#define GEM_FRAME_1536      (1<<8)      /* Enable 1536 byte frames reception */
#define GEM_UNICAST_EN      (1<<7)      /* Receive unicast hash frames */
#define GEM_MULTICAST_EN    (1<<6)      /* Receive multicast hash frames */
#define GEM_NO_BROADCAST    (1<<5)      /* Do not receive broadcast frames */
#define GEM_COPY_ALL        (1<<4)      /* Copy all frames */
#define GEM_RX_JUMBO        (1<<3)      /* Allow jumbo frame reception */
#define GEM_VLAN_ONLY       (1<<2)      /* Receive only VLAN frames */
#define GEM_FULL_DUPLEX     (1<<1)      /* Enable full duplex */
#define GEM_SPEED_100       (1<<0)      /* Set to 100Mb mode */

//
// Bit positions for network status register
//
#define GEM_PHY_IDLE        (1<<2)      /* PHY management is idle */
#define GEM_MDIO_IN         (1<<1)      /* Status of mdio_in pin */
#define GEM_LINK_STATUS     (1<<0)      /* Status of link pin */

//
// Bit positions for transmit status register
//
#define GEM_TX_HRESP        (1<<8)      /* Transmit hresp not OK */
#define GEM_LATE_COL        (1<<7)      /* Late collision */
#define GEM_TX_URUN         (1<<6)      /* Transmit underrun occurred */
#define GEM_TX_COMPLETE     (1<<5)      /* Transmit completed OK */
#define GEM_TX_BUF_EXH      (1<<4)      /* Transmit buffs exhausted mid frame */
#define GEM_TX_GO           (1<<3)      /* Status of tx_go internal variable */
#define GEM_TX_RETRY_EXC    (1<<2)      /* Retry limit exceeded */
#define GEM_TX_COL          (1<<1)      /* Collision occurred during frame tx */
#define GEM_TX_USED         (1<<0)      /* Used bit read in tx buffer */

//
// Bit positions for receive status register
//
#define GEM_RX_HRESP        (1<<3)      /* Receive hresp not OK */
#define GEM_RX_ORUN         (1<<2)      /* Receive overrun occurred */
#define GEM_RX_DONE         (1<<1)      /* Frame successfully received */
#define GEM_RX_BUF_USED     (1<<0)      /* Receive buffer used bit read */

//
// Bit positions for interrupts
//
#define GEM_IRQ_PCS_AN      (1<<16)     /* PCS autonegotiation complete */
#define GEM_IRQ_EXT_INT     (1<<15)     /* External interrupt pin triggered */
#define GEM_IRQ_PAUSE_TX    (1<<14)     /* Pause frame transmitted */
#define GEM_IRQ_PAUSE_0     (1<<13)     /* Pause time has reached zero */
#define GEM_IRQ_PAUSE_RX    (1<<12)     /* Pause frame received */
#define GEM_IRQ_HRESP       (1<<11)     /* hresp not ok */
#define GEM_IRQ_RX_ORUN     (1<<10)     /* Receive overrun occurred */
#define GEM_IRQ_PCS_LINK    (1<<9)      /* Status of PCS link changed */
#define GEM_IRQ_TX_DONE     (1<<7)      /* Frame transmitted ok */
#define GEM_IRQ_TX_ERROR    (1<<6)      /* Transmit err occurred or no buffers*/
#define GEM_IRQ_RETRY_EXC   (1<<5)      /* Retry limit exceeded */
#define GEM_IRQ_TX_URUN     (1<<4)      /* Transmit underrun occurred */
#define GEM_IRQ_TX_USED     (1<<3)      /* Tx buffer used bit read */
#define GEM_IRQ_RX_USED     (1<<2)      /* Rx buffer used bit read */
#define GEM_IRQ_RX_DONE     (1<<1)      /* Frame received ok */
#define GEM_IRQ_MAN_DONE    (1<<0)      /* PHY management operation complete */
#define GEM_IRQ_ALL         (0xFFFFFFFF)/* Everything! */

//
// Transmit buffer descriptor status words bit positions
//
#define GEM_TBQE_USED       (1<<31)     /* Used bit. */
#define GEM_TBQE_WRAP       (1<<30)     /* Wrap bit */
#define GEM_TBQE_RETRY_EXC  (1<<29)     /* Retry limit exceeded. */
#define GEM_TBQE_URUN       (1<<28)     /* Transmit underrun occurred. */
#define GEM_TBQE_BUF_EXH    (1<<27)     /* Buffers exhausted mid frame. */
#define GEM_TBQE_LATE_COL   (1<<26)     /* Late collision. */
#define GEM_TBQE_NO_CRC     (1<<16)     /* No CRC */
#define GEM_TBQE_LAST_BUF   (1<<15)     /* Last buffer */
#define GEM_TBQE_LEN_MASK   (0x3FFF)    /* Mask for length field */
#define GEM_TX_MAX_LEN      (0x3FFF)    /* Maximum transmit length value */
#define GEM_TBQE_DUMMY      (0x8000BFFF)/* Dummy value to check for free buffer*/

//
// Receive buffer descriptor status words bit positions
//
#define GEM_RBQE_BROADCAST  (1<<31)     /* Broadcast frame */
#define GEM_RBQE_MULTICAST  (1<<30)     /* Multicast hashed frame */
#define GEM_RBQE_UNICAST    (1<<29)     /* Unicast hashed frame */
#define GEM_RBQE_EXT_ADDR   (1<<28)     /* External address match */
#define GEM_RBQE_SPEC_MATCH (1<<27)     /* Specific address matched */
#define GEM_RBQE_SPEC_BASE  (25)        /* Pos for base of specific match */
#define GEM_RBQE_SPEC_MAT1  (1<<(RBQE_SPEC_BASE + 1))
#define GEM_RBQE_SPEC_MAT0  (1<<RBQE_SPEC_BASE)
#define GEM_RBQE_TYPE_MATCH (1<<24)     /* Type ID matched */
#define GEM_RBQE_TYPE_BASE  (22)        /* Position for base of type id match */
#define GEM_RBQE_TYPE_MAT1  (1<<(RBQE_TYPE_BASE + 1))
#define GEM_RBQE_TYPE_MAT0  (1<<RBQE_TYPE_BASE)
#define GEM_RBQE_VLAN       (1<<21)     /* VLAN tagged */
#define GEM_RBQE_PRIORITY   (1<<20)     /* Priority tagged */
#define GEM_RBQE_VLAN_BASE  (17)        /* Position for base of VLAN priority */
#define GEM_RBQE_VLAN_P2    (1<<(RBQE_VLAN_BASE+2))
#define GEM_RBQE_VLAN_P1    (1<<(RBQE_VLAN_BASE+1))
#define GEM_RBQE_VLAN_P0    (1<<RBQE_VLAN_BASE)
#define GEM_RBQE_CFI        (1<<16)     /* CFI frame */
#define GEM_RBQE_EOF        (1<<15)     /* End of frame. */
#define GEM_RBQE_SOF        (1<<14)     /* Start of frame. */
#define GEM_RBQE_LEN_MASK   (0x3FFF)    /* Mask for the length field. */
#define GEM_RBQE_WRAP       (1<<1)      /* Wrap bit.. */
#define GEM_RBQE_USED       (1<<0)      /* Used bit.. */
#define GEM_RBQE_ADD_MASK   (0xFFFFFFFC)/* Mask for address */

//
// Revision ID Register
//
#define GEM_REV_ID_MODEL_MASK   (0x000F0000)    /* Model ID */
#define GEM_REV_ID_MODEL_BASE   (16)            /* For Shifting */
#define GEM_REV_ID_REG_MODEL    (0x00020000)    /* GEM module ID */
#define GEM_REV_ID_REV_MASK     (0x0000FFFF)    /* Revision ID */

/* Define some types for using with the HAL.  These types correspond to the
 * memory map and programming structure of the MAC device.
 * All structures are 'volatile' to indicate they can be changed by some non-
 * programming means - i.e. by the hardware itself.  This prevents the compiler
 * from making false assumptions on how to optimise the code.  Some elements
 * are also defined as 'const' to enforce some checks on the programmer.  These
 * are only for register fields that can only be changed by the hardware and are
 * not writable.
 */

/* The Address organisation for the MAC device.  All addresses are split into
 * two 32-bit register fields.  The first one (bottom) is the lower 32-bits of
 * the address and the other field are the high order bits - this may be 16-bits
 * in the case of MAC addresses, or 32-bits for the hash address.
 * In terms of memory storage, the first item (bottom) is assumed to be at a
 * lower address location than 'top'. i.e. top should be at address location of
 * 'bottom' + 4 bytes.
 */
#ifndef __MAC_ADDR_DEF__
#define __MAC_ADDR_DEF__
typedef struct
{
    U32 bottom; /* Lower 32-bits of address. */
    U32 top; /* Upper 32-bits of address. */
} MAC_ADDR;
#endif

/* The following is the organisation of the address filters section of the MAC
 * registers.  The Cadence MAC contains four possible specific address match
 * addresses, if an incoming frame corresponds to any one of these four
 * addresses then the frame will be copied to memory.
 * It is not necessary for all four of the address match registers to be
 * programmed, this is application dependant.
 */
#ifndef __SPEC_ADDR_DEF__
#define __SPEC_ADDR_DEF__
typedef struct
{
    MAC_ADDR one; /* Specific address register 1. */
    MAC_ADDR two; /* Specific address register 2. */
    MAC_ADDR three; /* Specific address register 3. */
    MAC_ADDR four; /* Specific address register 4. */
} SPEC_ADDR;
#endif

/* The set of statistics registers implemented in the Cadence MAC.
 * The statistics registers implemented are a subset of all the statistics
 * available, but contains all the compulsory ones.
 * For full descriptions on the registers, refer to the Cadence MAC programmers
 * guide or the IEEE 802.3 specifications.
 */
typedef struct
{
    U32 octets_tx_bot; /* Lower 32-bits for number of octets tx'd */
    U32 octets_tx_top; /* Upper 16-bits for number of octets tx'd */
    U32 frames_tx; /* Number of frames transmitted OK */
    U32 broadcast_tx; /* Number of broadcast frames transmitted */
    U32 multicast_tx; /* Number of multicast frames transmitted */
    U32 pause_tx; /* Number of pause frames transmitted. */
    U32 frame64_tx; /* Number of 64byte frames transmitted */
    U32 frame65_127_tx; /* Number of 65-127 byte frames transmitted */
    U32 frame128_255_tx; /* Number of 128-255 byte frames transmitted */
    U32 frame256_511_tx; /* Number of 256-511 byte frames transmitted */
    U32 frame512_1023_tx; /* Number of 512-1023 byte frames transmitted */
    U32 frame1024_1518_tx; /* Number of 1024-1518 byte frames transmitted*/
    U32 frame1519_tx; /* Number of frames greater than 1518 bytes tx*/
    U32 tx_urun; /* Transmit underrun errors due to DMA */
    U32 single_col; /* Number of single collision frames */
    U32 multi_col; /* Number of multi collision frames */
    U32 excess_col; /* Number of excessive collision frames. */
    U32 late_col; /* Collisions occuring after slot time */
    U32 def_tx; /* Frames deferred due to crs */
    U32 crs_errors; /* Errors caused by crs not being asserted. */

    U32 octets_rx_bot; /* Lower 32-bits for number of octets rx'd */
    U32 octets_rx_top; /* Upper 16-bits for number of octets rx'd */
    U32 frames_rx; /* Number of frames received OK */
    U32 broadcast_rx; /* Number of broadcast frames received */
    U32 multicast_rx; /* Number of multicast frames received */
    U32 pause_rx; /* Number of pause frames received. */
    U32 frame64_rx; /* Number of 64byte frames received */
    U32 frame65_127_rx; /* Number of 65-127 byte frames received */
    U32 frame128_255_rx; /* Number of 128-255 byte frames received */
    U32 frame256_511_rx; /* Number of 256-511 byte frames received */
    U32 frame512_1023_rx; /* Number of 512-1023 byte frames received */
    U32 frame1024_1518_rx; /* Number of 1024-1518 byte frames received*/
    U32 frame1519_rx; /* Number of frames greater than 1518 bytes rx*/
    U32 usize_frames; /* Frames received less than min of 64 bytes */
    U32 excess_length; /* Number of excessive length frames rx */
    U32 jabbers; /* Excessive length + crc or align errors. */
    U32 fcs_errors; /* Number of frames received with crc errors */
    U32 length_check_errors;/* Number of frames with incorrect length */
    U32 rx_symbol_errors; /* Number of times rx_er asserted during rx */
    U32 align_errors; /* Frames received without integer no. bytes */
    U32 rx_res_errors; /* Number of times buffers ran out during rx */
    U32 rx_orun; /* Receive overrun errors due to DMA */
} GEM_STATS;

/* This is the memory map for the Cadence Enhanced MAC device.
 * For full descriptions on the registers, refer to the Cadence MAC programmers
 * guide or the IEEE 802.3 specifications.
 */
typedef struct GemRegisters
{
    V32 net_control;            /* Network control 0x00 */
    V32 net_config;             /* Network config 0x04 */
    const V32 net_status;       /* Network status, RO, 0x08 */
    V32 user_io;                /* User IO 0x0C*/
    const V32 rsvd1;            /* reserved 0x10*/
    V32 tx_status;              /* Transmit status 0x14 */
    V32 rx_qptr;                /* Receive queue pointer 0x18 */
    V32 tx_qptr;                /* Transmit queue pointer 0x1C */
    V32 rx_status;              /* Receive status 0x20 */
    V32 irq_status;             /* Interrupt status 0x24 */
    V32 irq_enable;             /* Interrupt enable 0x28 */
    V32 irq_disable;            /* Interrupt disable 0x2C */
    const V32 irq_mask;         /* Interrupt mask, RO, 0x30 */
    V32 phy_man;                /* PHY management 0x34 */
    const V32 pause_time;       /* Pause time register 0x38 */
    V32 tx_pause_quant;         /* Transmit pause quantum 0x3C */
    U32 reserved[16];           /* Reserved */
    MAC_ADDR hash_addr;         /* Hash address 0x80 - 0x84 */
    SPEC_ADDR address;          /* Specific addresses 0x88 - 0xA4 */
    V32 id_check1;              /* Type ID check1 0xA8 */
    V32 id_check2;              /* Type ID check2 0xAC */
    V32 id_check3;              /* Type ID check3 0xB0 */
    V32 id_check4;              /* Type ID check4 0xB4 */
    U32 rsvd2[17];              /* Reserved */
    V32 rev_id;                 /* Device Revision ID 0xFC*/
    GEM_STATS stats;            /* MAC statistics 0x100 - 0x1A4 */
} GEM_REG;

// flags to be used for PHY config
#define GEMAC_PHY_1000M		0x00000001
#define GEMAC_PHY_AUTONEG 	0x00000002

/* This is a structure that will be passed and used for all HAL operations, it
 * consists of pointers to the various MAC structures such as the MAC register
 * block and the first descriptor element for the rx and tx buffer queues.
 * Other internal variables declared for use in function calls and to keep track
 * of where things are.
 */
typedef struct GemDevice
{
    GEM_REG *registers;         /* Pointer to the MAC address space. */
    V32 *config;
    MAC_ADDR macAddr;
} GEM_DEVICE;

//
// Prototypes for functions of HAL
//

//
// Re-initialise device and check reset values
//
RESULTCODE GemReset(GEM_DEVICE *mac);

//
// Device setup
//
void GemSetLoop(GEM_DEVICE *mac, MAC_LOOP gem_loop);
MAC_LOOP GemGetLoop(GEM_DEVICE *mac);

void GemEnableEam(GEM_DEVICE *mac);
void GemDisableEam(GEM_DEVICE *mac);

void GemEnableFcsRx(GEM_DEVICE *mac);
void GemDisableFcsRx(GEM_DEVICE *mac);

void GemEnable1536Rx(GEM_DEVICE *mac);
void GemDisable1536Rx(GEM_DEVICE *mac);

void GemFullDuplex(GEM_DEVICE *mac);
void GemHalfDuplex(GEM_DEVICE *mac);

void GemSetSpeed(GEM_DEVICE *mac, MAC_SPEED gem_speed);
MAC_SPEED GemGetSpeed(GEM_DEVICE *mac);

void GemSetAhbWidth(GEM_DEVICE *mac, MAC_AHB_WIDTH gem_buswidth);
MAC_AHB_WIDTH GemGetAhbWidth(GEM_DEVICE *mac);

void GemSetMdcDiv(GEM_DEVICE *mac, MAC_MDC_DIV gem_mdcdiv);
MAC_MDC_DIV GemGetMdcDiv(GEM_DEVICE *mac);

//
// Pause control
//
void GemEnablePauseRx(GEM_DEVICE *mac);
void GemDisablePauseRx(GEM_DEVICE *mac);

U32 GemPauseTime(GEM_DEVICE *mac);

void GemEnablePauseCpy(GEM_DEVICE *mac);
void GemDisablePauseCpy(GEM_DEVICE *mac);

void GemSend0qPause(GEM_DEVICE *mac);
void GemSendPause(GEM_DEVICE *mac);

void GemSetTxPauseQ(GEM_DEVICE *mac, U32 gem_pause);
U32 GemGetTxPauseQ(GEM_DEVICE *mac);

//
// PHY management control
//
void GemEnableMdio(GEM_DEVICE *mac);
void GemDisableMdio(GEM_DEVICE *mac);

RESULTCODE GemPhyManWr(GEM_DEVICE *mac, U8 phy_addr, U8 reg_addr, U32 data);
RESULTCODE GemPhyManRd(GEM_DEVICE *mac, U8 phy_addr, U8 reg_addr);
U32 GemPhyManData(GEM_DEVICE *mac);

int GemPhyManIdle(GEM_DEVICE *mac);

int GemLinkStatus(GEM_DEVICE *mac);

//
// Address setup and control
//
void GemEnableUnicast(GEM_DEVICE *mac);
void GemDisableUnicast(GEM_DEVICE *mac);

void GemEnableMulticast(GEM_DEVICE *mac);
void GemDisableMulticast(GEM_DEVICE *mac);

void GemAllowBroadcast(GEM_DEVICE *mac);
void GemNoBroadcast(GEM_DEVICE *mac);

void GemEnableCopyAll(GEM_DEVICE *mac);
void GemDisableCopyAll(GEM_DEVICE *mac);

void GemSetHash(GEM_DEVICE *mac, MAC_ADDR *hash_addr);
MAC_ADDR GemGetHash(GEM_DEVICE *mac);

void GemSetAddress(GEM_DEVICE *mac, SPEC_ADDR *spec_addr);
SPEC_ADDR GemGetAddress(GEM_DEVICE *mac);

//
// Functions to convert between address formats
//
RESULTCODE GemEnetAddrByteMac(U8 * enet_byte_addr, MAC_ADDR *enet_addr);
RESULTCODE GemEnetAddrMacByte(U8 * enet_byte_addr, MAC_ADDR *enet_addr);

void GemSetLAddr1(GEM_DEVICE *mac, MAC_ADDR *address);
void GemSetLAddr2(GEM_DEVICE *mac, MAC_ADDR *address);
void GemSetLAddr3(GEM_DEVICE *mac, MAC_ADDR *address);
void GemSetLAddr4(GEM_DEVICE *mac, MAC_ADDR *address);
MAC_ADDR GemGetLAddr1(GEM_DEVICE *mac);
MAC_ADDR GemGetLAddr2(GEM_DEVICE *mac);
MAC_ADDR GemGetLAddr3(GEM_DEVICE *mac);
MAC_ADDR GemGetLAddr4(GEM_DEVICE *mac);

void GemSetIdCheck1(GEM_DEVICE *mac, U32 id_check);
void GemSetIdCheck2(GEM_DEVICE *mac, U32 id_check);
void GemSetIdCheck3(GEM_DEVICE *mac, U32 id_check);
void GemSetIdCheck4(GEM_DEVICE *mac, U32 id_check);
U32 GemGetIdCheck1(GEM_DEVICE *mac);
U32 GemGetIdCheck2(GEM_DEVICE *mac);
U32 GemGetIdCheck3(GEM_DEVICE *mac);
U32 GemGetIdCheck4(GEM_DEVICE *mac);

void GemEnableLenCheck(GEM_DEVICE *mac);
void GemDisableLenCheck(GEM_DEVICE *mac);

//
// Interrupt handling and masking
//
void GemSetIrqStat(GEM_DEVICE *mac, U32 irq_status);
U32 GemGetIrqStat(GEM_DEVICE *mac);

void GemEnableIrq(GEM_DEVICE *mac, U32 irq_en);
void GemMaskIrq(GEM_DEVICE *mac, U32 irq_mask);
U32 GemGetIrqMask(GEM_DEVICE *mac);

//
// Transmit control
//
RESULTCODE GemStartTx(GEM_DEVICE *mac);
void GemStopTx(GEM_DEVICE *mac);
void GemAbortTx(GEM_DEVICE *mac);
int GemTransmitting(GEM_DEVICE *mac);
U32 GemGetTxStat(GEM_DEVICE *mac);
void GemResetTxStat(GEM_DEVICE *mac, U32 rst_status);

//
// Receive control
//
RESULTCODE GemEnableRx(GEM_DEVICE *mac);
void GemDisableRx(GEM_DEVICE *mac);

int GemReceiveOn(GEM_DEVICE *mac);
U32 GemGetRxStat(GEM_DEVICE *mac);
void GemResetRxStat(GEM_DEVICE *mac, U32 rst_status);

void GemEnableRxJmb(GEM_DEVICE *mac);
void GemDisableRxJmb(GEM_DEVICE *mac);

void GemEnableVlanOnly(GEM_DEVICE *mac);
void GemDisableVlanOnly(GEM_DEVICE *mac);

//
// Snapshot of statistic registers
//
void GemEnableRdSnap(GEM_DEVICE *mac);
void GemDisableRdSnap(GEM_DEVICE *mac);
void GemTakeSnap(GEM_DEVICE *mac);

//
// Debug options
//
void GemStatsWrEn(GEM_DEVICE *mac);
void GemStatsWrOff(GEM_DEVICE *mac);
void GemStatsInc(GEM_DEVICE *mac);
void GemStatsClr(GEM_DEVICE *mac);

void gem_enable_bp(GEM_DEVICE *mac);
void gem_disable_bp(GEM_DEVICE *mac);

void GemEnRetryTest(GEM_DEVICE *mac);
void GemDisRetryTest(GEM_DEVICE *mac);

GEM_STATS GemGetStats(GEM_DEVICE *mac);
void GemSetStats(GEM_DEVICE *mac, GEM_STATS *stats);

#endif /*GEMAC_HAL_H_*/
