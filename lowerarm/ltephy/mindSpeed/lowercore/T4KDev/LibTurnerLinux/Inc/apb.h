/** @file apb.h
 *
 * @brief This is definition of APB-mapped I/O for T4xxx
 * @author Mindspeed Technologies
 * @version $Revision: 1.17 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef APB_H_
#define APB_H_

#define ROM_BASEADDR                    0xFF000000
#define ROM_SIZE                        0x00008000

/*
 * RADIO Cluster Blocks
 */
#define RAD_BASEADDR                    0xFA000000

#define JRAM_BASEADDR                   (RAD_BASEADDR + 0x000000)
#define JRAM_SIZE                       0x00004000
#define PCIE_BASEADDR                   (RAD_BASEADDR + 0x800000)
#define CPRI_BASEADDR         	        (RAD_BASEADDR + 0xC00000)
#define SRIO_0_BASEADDR        	        (RAD_BASEADDR + 0xE00000)
#define SRIO_1_BASEADDR        	        (RAD_BASEADDR + 0xF00000)

/*
 * Flash Devices (BSP)
 */

#define NOR8_BASEADDR                   0x68000000
#define NOR16_BASEADDR                  0x60000000
#define NAND_BASEADDR                   0x60000000
#define FPGA_BASEADDR                   0xB0000000
#define RADIO_BASEADDR                  0xB8000000

#define SPI_RAD0                        (1<<0)   // SPI CS for radio module 0
#define SPI_RAD1                        (1<<1)   // SPI CS for radio module 0
#define SPI_DEJITTER_CS                 (1<<2)
#define SPI_EPROM_CS                    (1<<3)



#define I2CEPROM_BASEADDR               0x0
#define I2CEPROM_BASEADDR               0x0
#define I2CPCA1_BASEADDR                0x10 //Serdes 2 SPF connection 0 1
#define I2CPCA2_BASEADDR                0x20 //Serdes 2 SPF connection 2 3
#define I2CPCA3_BASEADDR                0x30 //Serdes 1 SPF connection 0 1
#define I2CPCA4_BASEADDR                0x40 //FPGA Serdes SPF connection 0 1
#define I2CPCA5_BASEADDR                0x50 //FPGA Serdes SPF connection 2 3
/*
 * DDR0
 */

#define DDR0_BASEADDR                   0x00000000

/*
 * DDR1
 */

#define DDR1_BASEADDR                   0x80000000
#define DDR_SIZE                        0x20000000

/*
 * IRAM
 */

#define IRAM_BASEADDR                   0xFB000000
#define IRAM_SIZE                       0x00020000

/*
 * DDR Controllers
 */

#define DDR0_CONTROLLER_BASE            0xFBE00000
#define DDR1_CONTROLLER_BASE            0xFBE80000

/*
 * Hardware semaphores
 */

#define SYS_SEMAPHORE_BASE              0xFBF00000
#define SYS_SEMAPHORE_SIZE              0x100000

#define SPU_SEMAPHORE_BASE              0xFCF00000
#define SPU_SEMAPHORE_SIZE              0x100000

/*
 * DDR PHY Controllers
 */

#define DDRPHY0_BASE                    0xFE450000
#define DDRPHY1_BASE                    0xFE460000

/*
 * CRAM
 */

#define CRAM_BASEADDR                   0xFC000000
#define CRAM_SIZE                       0x00300000


#define SPU_BASEADDR                    0xFD000000
#define MDMA_SPU_BASEADDR               (SPU_BASEADDR+0x010000)
#define FP_CTRL_BASEADDR                (SPU_BASEADDR+0x20000)
#define SPUCLKRST_BASEADDR              (SPU_BASEADDR+0x30000)
#define INTC_SPU_BASEADDR               (SPU_BASEADDR+0x40000)

/*

 * APB Address Assignments

 */

#define APB_BASEADDR                    0xFE000000
#define TDM_APB_BASEADDR                (APB_BASEADDR + 0x000000)
#define HBI_APB_BASEADDR                (APB_BASEADDR + 0x010000)
#define TDMA_APB_BASEADDR               (APB_BASEADDR + 0x020000)
#define TDMAEXT_APB_BASEADDR            (APB_BASEADDR + 0x030000)
#define TIMER_APB_BASEADDR              (APB_BASEADDR + 0x050000)
#define GPIO_APB_BASEADDR               (APB_BASEADDR + 0x070000)
#define UART0_BASEADDR                  (APB_BASEADDR + 0x090000)
#define UART1_BASEADDR                  (APB_BASEADDR + 0x092000)
#define UART2_BASEADDR                  (APB_BASEADDR + 0x094000)
#define SPI_BASEADDR                    (APB_BASEADDR + 0x098000)
#define I2C_BASEADDR                    (APB_BASEADDR + 0x09C000)
#define TOPCLKRST_BASEADDR              (APB_BASEADDR + 0x0B0000)
#define XPCLKRST_BASEADDR               (APB_BASEADDR + 0x0C0000)
#define GEMAC0_BASEADDR                 (APB_BASEADDR + 0x0D0000)
#define XP_APB_BASEADDR                 (APB_BASEADDR + 0x100000)
#define GEMAC1_BASEADDR                 (APB_BASEADDR + 0x170000)
#define SERDES_BASEADDR                 (APB_BASEADDR + 0x1A0000)
#define SYS_APB_BASEADDR                (APB_BASEADDR + 0x400000)
#define RAD_APB_BASEADDR                (APB_BASEADDR + 0x800000)

/*
 * APB Radio APB Address Assignments
 */

#define RAD_APB_CLKRST_BASEADDR         (RAD_APB_BASEADDR + 0x000000)
#define RAD_APB_CONFIG_BASEADDR         (RAD_APB_BASEADDR + 0x010000)
#define RAD_APB_SRIO0_BASEADDR          (RAD_APB_BASEADDR + 0x020000)
#define RAD_APB_SRIO1_BASEADDR          (RAD_APB_BASEADDR + 0x030000)
#define RAD_APB_SRIO_PFWD_BASEADDR      (RAD_APB_BASEADDR + 0x040000)
#define RAD_APB_CPRI_SYS_BASEADDR       (RAD_APB_BASEADDR + 0x090000)
#define RAD_APB_CPDMA_BASEADDR          (RAD_APB_BASEADDR + 0x0A0000)
#define RAD_APB_PCIE_SYS_BASEADDR       (RAD_APB_BASEADDR + 0x0B0000)
#define RAD_APB_MDMA_BASEADDR           (RAD_APB_BASEADDR + 0x0C0000)
#define RAD_APB_SERDES0_4CH_BASEADDR    (RAD_APB_BASEADDR + 0x0F0000)
#define RAD_APB_SERDES1_2CH_BASEADDR    (RAD_APB_BASEADDR + 0x100000)
#define RAD_APB_SERDES2_4CH_BASEADDR    (RAD_APB_BASEADDR + 0x110000)

// Radio Cluster Clock and Reset Control Registers
#define RAD_CLUSTER_CNTRL               (RAD_APB_CLKRST_BASEADDR + (4 * 0x00))
#define RAD_AXI_CLK_DIV_CNTRL           (RAD_APB_CLKRST_BASEADDR + (4 * 0x01))
#define RAD_AXI_CLK_CNTRL_0             (RAD_APB_CLKRST_BASEADDR + (4 * 0x02))
#define RAD_AXI_CLK_CNTRL_1             (RAD_APB_CLKRST_BASEADDR + (4 * 0x03))
#define RAD_AXI_RESET_0                 (RAD_APB_CLKRST_BASEADDR + (4 * 0x04))
#define RAD_AXI_RESET_1                 (RAD_APB_CLKRST_BASEADDR + (4 * 0x05))
#define RAD_SRDS0_TXIF_CLK_DIV_CNTRL    (RAD_APB_CLKRST_BASEADDR + (4 * 0x06))
#define RAD_SRDS0_CONT_CLK_DIV_CNTRL	(RAD_APB_CLKRST_BASEADDR + (4 * 0x09))
#define RAD_SRDS1_TXIF_CLK_DIV_CNTRL    (RAD_APB_CLKRST_BASEADDR + (4 * 0x0B))
#define RAD_SRDS1_CONT_CLK_DIV_CNTRL    (RAD_APB_CLKRST_BASEADDR + (4 * 0x0E))
#define RAD_SRDS1_CALIB_CLK_DIV_CNTRL   (RAD_APB_CLKRST_BASEADDR + (4 * 0x0F))
#define RAD_SRDS2_TXIF_CLK_DIV_CNTRL    (RAD_APB_CLKRST_BASEADDR + (4 * 0x10))
#define RAD_SRDS2_CONT_CLK_DIV_CNTRL    (RAD_APB_CLKRST_BASEADDR + (4 * 0x13))
#define RAD_SRDS2_CALIB_CLK_DIV_CNTRL   (RAD_APB_CLKRST_BASEADDR + (4 * 0x14))
#define RAD_SRIO0_CLK_CNTRL             (RAD_APB_CLKRST_BASEADDR + (4 * 0x15))
#define RAD_SRIO0_RESET                 (RAD_APB_CLKRST_BASEADDR + (4 * 0x16))
#define RAD_SRIO1_CLK_CNTRL             (RAD_APB_CLKRST_BASEADDR + (4 * 0x17))
#define RAD_SRIO1_RESET                 (RAD_APB_CLKRST_BASEADDR + (4 * 0x18))
#define RAD_CPRI0_CLK_CNTRL_0           (RAD_APB_CLKRST_BASEADDR + (4 * 0x19))
#define RAD_CPRI0_CLK_CNTRL_1           (RAD_APB_CLKRST_BASEADDR + (4 * 0x1A))
#define RAD_CPRI0_RESET_0               (RAD_APB_CLKRST_BASEADDR + (4 * 0x1B))
#define RAD_CPRI0_RESET_1               (RAD_APB_CLKRST_BASEADDR + (4 * 0x1C))
#define RAD_CPRI1_CLK_CNTRL_0           (RAD_APB_CLKRST_BASEADDR + (4 * 0x1D))
#define RAD_CPRI1_CLK_CNTRL_1           (RAD_APB_CLKRST_BASEADDR + (4 * 0x1E))
#define RAD_CPRI1_RESET_0               (RAD_APB_CLKRST_BASEADDR + (4 * 0x1F))
#define RAD_CPRI1_RESET_1               (RAD_APB_CLKRST_BASEADDR + (4 * 0x20))
#define RAD_PCIE_CLK_CNTRL              (RAD_APB_CLKRST_BASEADDR + (4 * 0x23))
#define RAD_PCIE_RESET                  (RAD_APB_CLKRST_BASEADDR + (4 * 0x24))
#define RAD_SRDS0_CNTRL                 (RAD_APB_CLKRST_BASEADDR + (4 * 0x25))
#define RAD_SRDS1_CNTRL                 (RAD_APB_CLKRST_BASEADDR + (4 * 0x26))
#define RAD_SRDS2_CNTRL                 (RAD_APB_CLKRST_BASEADDR + (4 * 0x27))
#define RAD_RE_SRDS1_RX_CLK_OUT_CNTRL   (RAD_APB_CLKRST_BASEADDR + (4 * 0x28))

// CONFIG_RAD Registers
#define RAD_CFG_PSTBL                   (RAD_APB_CONFIG_BASEADDR + 0x00)
#define RAD_CFG_SRDS_MUX                (RAD_APB_CONFIG_BASEADDR + 0x04)
#define RAD_CFG_CPRI2SPU                (RAD_APB_CONFIG_BASEADDR + 0x08)
#define RAD_CFG_T_RWM0                  (RAD_APB_CONFIG_BASEADDR + 0x0C)
#define RAD_CFG_T_RWM1                  (RAD_APB_CONFIG_BASEADDR + 0x10)
#define RAD_CFG_CPRI_PRIOR              (RAD_APB_CONFIG_BASEADDR + 0x14)
#define RAD_CFG_SRIO_PRIOR              (RAD_APB_CONFIG_BASEADDR + 0x18)
#define RAD_CFG_PCIE_PRIOR              (RAD_APB_CONFIG_BASEADDR + 0x1C)
#define RAD_CFG_RAD_DMA_PRIOR           (RAD_APB_CONFIG_BASEADDR + 0x20)
#define RAD_CFG_PCIE_RAD_DMA            (RAD_APB_CONFIG_BASEADDR + 0x24)
#define RAD_CFG_SRDS_MMD                (RAD_APB_CONFIG_BASEADDR + 0x28)
#define RAD_CFG_SRDS_LANE               (RAD_APB_CONFIG_BASEADDR + 0x2C)
#define RAD_CFG_SRIO0_MODE              (RAD_APB_CONFIG_BASEADDR + 0x30)
#define RAD_CFG_SRIO1_MODE              (RAD_APB_CONFIG_BASEADDR + 0x34)
#define RAD_CFG_CPRI_RATE               (RAD_APB_CONFIG_BASEADDR + 0x38)
#define RAD_CFG_PCIE_SET_IRQ            (RAD_APB_CONFIG_BASEADDR + 0x40)
#define RAD_CFG_PCIE_PEND_IRQ           (RAD_APB_CONFIG_BASEADDR + 0x44)
#define RAD_CFG_PCIE_CFG                (RAD_APB_CONFIG_BASEADDR + 0x48)
#define RAD_CFG_PCIE_FNUM               (RAD_APB_CONFIG_BASEADDR + 0x4C)
#define RAD_CFG_PCIE_DBG                (RAD_APB_CONFIG_BASEADDR + 0x50)
#define RAD_CFG_PCI_ROOT_COMP_ID        (RAD_APB_CONFIG_BASEADDR + 0x54)
#define RAD_CFG_PCIE_MRC_WR_ID          (RAD_APB_CONFIG_BASEADDR + 0x58)
#define RAD_CFG_PCIE_MRC_WC_ID          (RAD_APB_CONFIG_BASEADDR + 0x5C)
#define RAD_CFG_PCIE_MRC_RR_ID          (RAD_APB_CONFIG_BASEADDR + 0x60)
#define RAD_CFG_PCIE_MRC_RC_ID          (RAD_APB_CONFIG_BASEADDR + 0x64)
#define RAD_CFG_PCIE_IRQ_ACK            (RAD_APB_CONFIG_BASEADDR + 0x68)
#define RAD_CFG_PCIE_STATUS             (RAD_APB_CONFIG_BASEADDR + 0x6C)
#define RAD_CFG_PCIE_WRITE_ERR          (RAD_APB_CONFIG_BASEADDR + 0x70)
#define RAD_CFG_PCIE_READ_ERR           (RAD_APB_CONFIG_BASEADDR + 0x74)
#define RAD_SRDS0_INIT_CFG_R0           (RAD_APB_CONFIG_BASEADDR + 0x78)
#define RAD_SRDS0_INIT_CFG_R1           (RAD_APB_CONFIG_BASEADDR + 0x7C)
#define RAD_SRDS0_INIT_CFG_R2           (RAD_APB_CONFIG_BASEADDR + 0x80)
#define RAD_SRDS0_INIT_CFG_R3           (RAD_APB_CONFIG_BASEADDR + 0x84)
#define RAD_SRDS1_INIT_CFG_R0           (RAD_APB_CONFIG_BASEADDR + 0x88)
#define RAD_SRDS1_INIT_CFG_R1           (RAD_APB_CONFIG_BASEADDR + 0x8C)
#define RAD_SRDS2_INIT_CFG_R0           (RAD_APB_CONFIG_BASEADDR + 0x90)
#define RAD_SRDS2_INIT_CFG_R1           (RAD_APB_CONFIG_BASEADDR + 0x94)
#define RAD_SRDS2_INIT_CFG_R2           (RAD_APB_CONFIG_BASEADDR + 0x98)
#define RAD_SRDS2_INIT_CFG_R3           (RAD_APB_CONFIG_BASEADDR + 0x9C)
#define RAD_CFG_CPRI_FSYNC_MODE         (RAD_APB_CONFIG_BASEADDR + 0xEC)
#define RAD_CFG_SRIO_LOOPBACK           (RAD_APB_CONFIG_BASEADDR + 0xF0)
#define RAD_CFG_PDEN_REG                (RAD_APB_CONFIG_BASEADDR + 0xF4)
#define RAD_CFG_CPRI_MODE_REG           (RAD_APB_CONFIG_BASEADDR + 0xF8)

// SRDS0 Registers
#define RAD_SRDS0_PCS_MODE              (RAD_APB_SERDES0_4CH_BASEADDR + 0x8000)
#define RAD_SRDS0_PCS_MISC_CFG          (RAD_APB_SERDES0_4CH_BASEADDR + 0x8004)
#define RAD_SRDS0_PCS_PRBS_ERRCNT0      (RAD_APB_SERDES0_4CH_BASEADDR + 0x8008)
#define RAD_SRDS0_PCS_PRBS_ERRCNT1      (RAD_APB_SERDES0_4CH_BASEADDR + 0x800C)
#define RAD_SRDS0_PCS_PRBS_TIMER0       (RAD_APB_SERDES0_4CH_BASEADDR + 0x8010)
#define RAD_SRDS0_PCS_PRBS_TIMER1       (RAD_APB_SERDES0_4CH_BASEADDR + 0x8014)
#define RAD_SRDS0_PCS_PRBS_TIMER2       (RAD_APB_SERDES0_4CH_BASEADDR + 0x8018)
#define RAD_SRDS0_PCS_PRBS_RESET        (RAD_APB_SERDES0_4CH_BASEADDR + 0x8024)
#define RAD_SRDS0_PCS_PRBS_CONFIG       (RAD_APB_SERDES0_4CH_BASEADDR + 0x8030)

// SRDS1 Registers
#define RAD_SRDS1_PCS_MODE              (RAD_APB_SERDES1_2CH_BASEADDR + 0x8000)
#define RAD_SRDS1_PCS_MISC_CFG          (RAD_APB_SERDES1_2CH_BASEADDR + 0x8004)
#define RAD_SRDS1_PCS_PRBS_ERRCNT0      (RAD_APB_SERDES1_2CH_BASEADDR + 0x8008)
#define RAD_SRDS1_PCS_PRBS_ERRCNT1      (RAD_APB_SERDES1_2CH_BASEADDR + 0x800C)
#define RAD_SRDS1_PCS_PRBS_TIMER0       (RAD_APB_SERDES1_2CH_BASEADDR + 0x8010)
#define RAD_SRDS1_PCS_PRBS_TIMER1       (RAD_APB_SERDES1_2CH_BASEADDR + 0x8014)
#define RAD_SRDS1_PCS_PRBS_TIMER2       (RAD_APB_SERDES1_2CH_BASEADDR + 0x8018)
#define RAD_SRDS1_PCS_PRBS_RESET        (RAD_APB_SERDES1_2CH_BASEADDR + 0x8024)
#define RAD_SRDS1_PCS_PRBS_CONFIG       (RAD_APB_SERDES1_2CH_BASEADDR + 0x8030)

// SRDS2 Registers
#define RAD_SRDS2_PCS_MODE              (RAD_APB_SERDES2_4CH_BASEADDR + 0x8000)
#define RAD_SRDS2_PCS_MISC_CFG          (RAD_APB_SERDES2_4CH_BASEADDR + 0x8004)
#define RAD_SRDS2_PCS_PRBS_ERRCNT0      (RAD_APB_SERDES2_4CH_BASEADDR + 0x8008)
#define RAD_SRDS2_PCS_PRBS_ERRCNT1      (RAD_APB_SERDES2_4CH_BASEADDR + 0x800C)
#define RAD_SRDS2_PCS_PRBS_TIMER0       (RAD_APB_SERDES2_4CH_BASEADDR + 0x8010)
#define RAD_SRDS2_PCS_PRBS_TIMER1       (RAD_APB_SERDES2_4CH_BASEADDR + 0x8014)
#define RAD_SRDS2_PCS_PRBS_TIMER2       (RAD_APB_SERDES2_4CH_BASEADDR + 0x8018)
#define RAD_SRDS2_PCS_PRBS_RESET        (RAD_APB_SERDES2_4CH_BASEADDR + 0x8024)
#define RAD_SRDS2_PCS_PRBS_CONFIG       (RAD_APB_SERDES2_4CH_BASEADDR + 0x8030)

// PCIe Registers
#define PCIE_LOS_TIMEOUT_REG            (RAD_APB_PCIE_SYS_BASEADDR + 0x24)
#define PCIE_MIRQ                       (RAD_APB_PCIE_SYS_BASEADDR + 0xFFC)
#define PCIE_INT_ACK                    (RAD_APB_PCIE_SYS_BASEADDR + 0xFF8)

/*
 * APB System APB Address Assignments
 */

#define MDMA_SYS_BASEADDR               (SYS_APB_BASEADDR + 0x000000)
#define FEC_BASEADDR                    (SYS_APB_BASEADDR + 0x010000)
#define ARM_SYS_BASEADDR                (SYS_APB_BASEADDR + 0x020000)
#define CFG_SYS_BASEADDR                (SYS_APB_BASEADDR + 0x030000)
#define SYSCLKRST_BASEADDR              (SYS_APB_BASEADDR + 0x040000)
#define DDRPHY0_BASEADDR                (SYS_APB_BASEADDR + 0x050000)
#define DDRPHY1_BASEADDR                (SYS_APB_BASEADDR + 0x060000)

/*
 * Top Level Clock and Reset Control Registers
 */

#define CLUSTER_RESETS                  (TOPCLKRST_BASEADDR + 4*0x0)
#define TOP_GEN_CNTRL                   (TOPCLKRST_BASEADDR + 4*0x1)
#define GP_CONFIG                       (TOPCLKRST_BASEADDR + 4*0x2)
#define GP_STAT                         (TOPCLKRST_BASEADDR + 4*0x3)
#define AXI_CLK_CNTRL_0                 (TOPCLKRST_BASEADDR + 4*0x4)
#define AXI_CLK_CNTRL_1                 (TOPCLKRST_BASEADDR + 4*0x5)
#define AXI_CLK_DIV_CNTRL_0             (TOPCLKRST_BASEADDR + 4*0x6)
#define AXI_CLK_DIV_CNTRL_1             (TOPCLKRST_BASEADDR + 4*0x7)
#define AXI_CLK_DIV_CNTRL_2             (TOPCLKRST_BASEADDR + 4*0x8)
#define CEVA_CLK_CNTRL                  (TOPCLKRST_BASEADDR + 4*0xC)
#define CEVA_CLK_DIV_CNTRL              (TOPCLKRST_BASEADDR + 4*0xD)
#define FFT_CLK_CNTRL                   (TOPCLKRST_BASEADDR + 4*0x10)
#define FFT_CLK_DIV_CNTRL               (TOPCLKRST_BASEADDR + 4*0x11)
#define ARMQP_CLK_CNTRL                 (TOPCLKRST_BASEADDR + 4*0x14)
#define ARMQP_CLK_DIV_CNTRL             (TOPCLKRST_BASEADDR + 4*0x15)
#define ARMDP_CLK_CNTRL                 (TOPCLKRST_BASEADDR + 4*0x18)
#define ARMDP_CLK_DIV_CNTRL             (TOPCLKRST_BASEADDR + 4*0x19)
#define DDR_CLK_CNTRL                   (TOPCLKRST_BASEADDR + 4*0x1C)
#define DDR_CLK_DIV_CNTRL               (TOPCLKRST_BASEADDR + 4*0x1D)
#define GEMPHY_CLK_CNTRL                (TOPCLKRST_BASEADDR + 4*0x20)
#define GEMPHY_CLK_DIV_CNTRL            (TOPCLKRST_BASEADDR + 4*0x21)
#define FEC_DL_CLK_CNTRL                (TOPCLKRST_BASEADDR + 4*0x24)
#define FEC_DL_CLK_DIV_CNTRL            (TOPCLKRST_BASEADDR + 4*0x25)
#define TPI_CLK_CNTRL                   (TOPCLKRST_BASEADDR + 4*0x28)
#define TPI_CLK_DIV_CNTRL               (TOPCLKRST_BASEADDR + 4*0x29)
#define TDMNTG_CLK_CNTRL                (TOPCLKRST_BASEADDR + 4*0x2C)
#define TDMNTG_CLK_DIV_CNTRL            (TOPCLKRST_BASEADDR + 4*0x2D)
#define GEMNTG_CLK_CNTRL                (TOPCLKRST_BASEADDR + 4*0x30)
#define GEMNTG_CLK_DIV_CNTRL            (TOPCLKRST_BASEADDR + 4*0x31)
#define FEC_UL_CLK_CNTRL                (TOPCLKRST_BASEADDR + 4*0x34)
#define FEC_UL_CLK_DIV_CNTRL            (TOPCLKRST_BASEADDR + 4*0x35)
#define PLL0_NR                         (TOPCLKRST_BASEADDR + 0x100)
#define PLL0_NF_0                       (TOPCLKRST_BASEADDR + 0x104)
#define PLL0_NF_1                       (TOPCLKRST_BASEADDR + 0x108)
#define PLL0_BWADJ_0                    (TOPCLKRST_BASEADDR + 0x10C)
#define PLL0_BWADJ_1                    (TOPCLKRST_BASEADDR + 0x110)
#define PLL0_OD                         (TOPCLKRST_BASEADDR + 0x114)
#define PLL0_CNTRL                      (TOPCLKRST_BASEADDR + 0x118)
#define PLL0_STATUS                     (TOPCLKRST_BASEADDR + 0x11C)
#define PLL1_NR                         (TOPCLKRST_BASEADDR + 0x140)
#define PLL1_NF_0                       (TOPCLKRST_BASEADDR + 0x144)
#define PLL1_NF_1                       (TOPCLKRST_BASEADDR + 0x148)
#define PLL1_BWADJ_0                    (TOPCLKRST_BASEADDR + 0x14C)
#define PLL1_BWADJ_1                    (TOPCLKRST_BASEADDR + 0x150)
#define PLL1_OD                         (TOPCLKRST_BASEADDR + 0x154)
#define PLL1_CNTRL                      (TOPCLKRST_BASEADDR + 0x158)
#define PLL1_STATUS                     (TOPCLKRST_BASEADDR + 0x15C)
#define PLL2_NR                         (TOPCLKRST_BASEADDR + 0x180)
#define PLL2_NF_0                       (TOPCLKRST_BASEADDR + 0x184)
#define PLL2_NF_1                       (TOPCLKRST_BASEADDR + 0x188)
#define PLL2_BWADJ_0                    (TOPCLKRST_BASEADDR + 0x18C)
#define PLL2_BWADJ_1                    (TOPCLKRST_BASEADDR + 0x190)
#define PLL2_OD                         (TOPCLKRST_BASEADDR + 0x194)
#define PLL2_CNTRL                      (TOPCLKRST_BASEADDR + 0x198)
#define PLL2_STATUS                     (TOPCLKRST_BASEADDR + 0x19C)
#define PLL3_NR                         (TOPCLKRST_BASEADDR + 0x1C0)
#define PLL3_NF_0                       (TOPCLKRST_BASEADDR + 0x1C4)
#define PLL3_NF_1                       (TOPCLKRST_BASEADDR + 0x1C8)
#define PLL3_BWADJ_0                    (TOPCLKRST_BASEADDR + 0x1CC)
#define PLL3_BWADJ_1                    (TOPCLKRST_BASEADDR + 0x1D0)
#define PLL3_OD                         (TOPCLKRST_BASEADDR + 0x1D4)
#define PLL3_CNTRL                      (TOPCLKRST_BASEADDR + 0x1D8)
#define PLL3_STATUS                     (TOPCLKRST_BASEADDR + 0x1DC)
#define PLL4_NR                         (TOPCLKRST_BASEADDR + 0x200)
#define PLL4_NF_0                       (TOPCLKRST_BASEADDR + 0x204)
#define PLL4_NF_1                       (TOPCLKRST_BASEADDR + 0x208)
#define PLL4_BWADJ_0                    (TOPCLKRST_BASEADDR + 0x20C)
#define PLL4_BWADJ_1                    (TOPCLKRST_BASEADDR + 0x210)
#define PLL4_OD                         (TOPCLKRST_BASEADDR + 0x214)
#define PLL4_CNTRL                      (TOPCLKRST_BASEADDR + 0x218)
#define PLL4_STATUS                     (TOPCLKRST_BASEADDR + 0x21C)
#define PLL5_NR                         (TOPCLKRST_BASEADDR + 0x240)
#define PLL5_NF_0                       (TOPCLKRST_BASEADDR + 0x244)
#define PLL5_NF_1                       (TOPCLKRST_BASEADDR + 0x248)
#define PLL5_BWADJ_0                    (TOPCLKRST_BASEADDR + 0x24C)
#define PLL5_BWADJ_1                    (TOPCLKRST_BASEADDR + 0x250)
#define PLL5_OD                         (TOPCLKRST_BASEADDR + 0x254)
#define PLL5_CNTRL                      (TOPCLKRST_BASEADDR + 0x258)
#define PLL5_STATUS                     (TOPCLKRST_BASEADDR + 0x25C)

#define PLL_NR(id)                      (TOPCLKRST_BASEADDR + (0x100 + ((id) * 0x40)))
#define PLL_NF_0(id)                    (TOPCLKRST_BASEADDR + (0x104 + ((id) * 0x40)))
#define PLL_NF_1(id)                    (TOPCLKRST_BASEADDR + (0x108 + ((id) * 0x40)))
#define PLL_BWADJ_0(id)                 (TOPCLKRST_BASEADDR + (0x10C + ((id) * 0x40)))
#define PLL_BWADJ_1(id)                 (TOPCLKRST_BASEADDR + (0x110 + ((id) * 0x40)))
#define PLL_OD(id)                      (TOPCLKRST_BASEADDR + (0x114 + ((id) * 0x40)))
#define PLL_CNTRL(id)                   (TOPCLKRST_BASEADDR + (0x118 + ((id) * 0x40)))
#define PLL_STATUS(id)                  (TOPCLKRST_BASEADDR + (0x11C + ((id) * 0x40)))

/*
 *  System Cluster Clocks and Resets Control Registers
 */ 

#define SYS_CLUSTER_CNTRL               (SYSCLKRST_BASEADDR + 4*0x0)
#define SYS_A9DP_CORE_CLK_DIV_CNTRL     (SYSCLKRST_BASEADDR + 4*0x1)
#define SYS_A9DP_CORE_CLK_CNTRL         (SYSCLKRST_BASEADDR + 4*0x2)
#define SYS_A9DP_AXI_CLK_DIV_CNTRL      (SYSCLKRST_BASEADDR + 4*0x3)
#define SYS_A9DP_L2_AXI_CLK_CNTRL       (SYSCLKRST_BASEADDR + 4*0x4)
#define SYS_A9DP_RESET_0                (SYSCLKRST_BASEADDR + 4*0x5)
#define SYS_A9DP_RESET_1                (SYSCLKRST_BASEADDR + 4*0x6)
#define SYS_A9DP_RESET_2                (SYSCLKRST_BASEADDR + 4*0x7)
#define SYS_A9DP_WD_CNTRL               (SYSCLKRST_BASEADDR + 4*0x8)
#define SYS_A9QP_CORE_CLK_DIV_CNTRL     (SYSCLKRST_BASEADDR + 4*0x9)
#define SYS_A9QP_CORE_CLK_CNTRL         (SYSCLKRST_BASEADDR + 4*0xA)
#define SYS_A9QP_AXI_CLK_DIV_CNTRL      (SYSCLKRST_BASEADDR + 4*0xB)
#define SYS_A9QP_L2_AXI_CLK_CNTRL       (SYSCLKRST_BASEADDR + 4*0xC)
#define	SYS_A9QP_RESET_0                (SYSCLKRST_BASEADDR + 4*0xD)
#define SYS_A9QP_RESET_1                (SYSCLKRST_BASEADDR + 4*0xE)
#define SYS_A9QP_RESET_2                (SYSCLKRST_BASEADDR + 4*0xF)
#define SYS_A9QP_WD_CNTRL               (SYSCLKRST_BASEADDR + 4*0x10)
#define SYS_AXI_CLK_DIV_CNTRL           (SYSCLKRST_BASEADDR + 4*0x11)
#define SYS_AXI_CLK_CNTRL_0             (SYSCLKRST_BASEADDR + 4*0x12)
#define SYS_AXI_CLK_CNTRL_1             (SYSCLKRST_BASEADDR + 4*0x13)
#define SYS_AXI_RESET_0                 (SYSCLKRST_BASEADDR + 4*0x14)
#define SYS_AXI_RESET_1                 (SYSCLKRST_BASEADDR + 4*0x15)
#define SYS_DDR_CLK_DIV_CNTRL           (SYSCLKRST_BASEADDR + 4*0x17)
#define SYS_DDR_CLK_CNTRL               (SYSCLKRST_BASEADDR + 4*0x18)
#define SYS_DDR_RESET                   (SYSCLKRST_BASEADDR + 4*0x19)
#define SYS_FEC_CLK_DIV_CNTRL           (SYSCLKRST_BASEADDR + 4*0x1A)
#define SYS_FEC_CLK_CNTRL               (SYSCLKRST_BASEADDR + 4*0x1B)
#define SYS_FEC_RESET                   (SYSCLKRST_BASEADDR + 4*0x1C)
#define SYS_TPI_CLK_DIV_CNTRL           (SYSCLKRST_BASEADDR + 4*0x1D)
#define SYS_CSYS_DBG_CLK_CNTRL          (SYSCLKRST_BASEADDR + 4*0x1E)
#define SYS_CSYS_DBG_RESET              (SYSCLKRST_BASEADDR + 4*0x1F)

/*
 * System configuration registers
 */

#define CFG_SYS_CFG_0                   (CFG_SYS_BASEADDR + 0x00)
#define CFG_SYS_CFG_1                   (CFG_SYS_BASEADDR + 0x04)
#define CFG_SYS_CFG_2                   (CFG_SYS_BASEADDR + 0x08)
#define CFG_SYS_CFG_3                   (CFG_SYS_BASEADDR + 0x0C)
#define CFG_SYS_STAT_0                  (CFG_SYS_BASEADDR + 0x10)
#define CFG_SYS_STAT_1                  (CFG_SYS_BASEADDR + 0x14)
#define CFG_SYS_STAT_2                  (CFG_SYS_BASEADDR + 0x18)
#define CFG_SYS_STAT_3                  (CFG_SYS_BASEADDR + 0x1C)
#define CFG_SYS_BUS_CFG                 (CFG_SYS_BASEADDR + 0x20)
#define CFG_SYS_BUS_PRIO                (CFG_SYS_BASEADDR + 0x24)
#define CFG_SYS_DDR_CFG                 (CFG_SYS_BASEADDR + 0x24)
#define CFG_SYS_DDR_LL0                 (CFG_SYS_BASEADDR + 0x2C)
#define CFG_SYS_DDR_HL0                 (CFG_SYS_BASEADDR + 0x30)
#define CFG_SYS_DDR_LL1                 (CFG_SYS_BASEADDR + 0x34)
#define CFG_SYS_DDR_HL1                 (CFG_SYS_BASEADDR + 0x38)
#define CFG_SYS_ARM_IRQ_SET             (CFG_SYS_BASEADDR + 0x3C)
#define CFG_SYS_ARM_IRQ_CLR             (CFG_SYS_BASEADDR + 0x40)
#define CFG_SYS_SEC_CONFIG              (CFG_SYS_BASEADDR + 0x50)


/*
 *  Expansion Cluster
 */  

// Expansion Cluster Clock and Reset Control Registers

#define XP_CLUSTER_CNTRL                (XPCLKRST_BASEADDR + 4*0x00)
#define XP_CLK_RSVD                     (XPCLKRST_BASEADDR + 4*0x01)
#define XP_AXI_CLK_CNTRL_0              (XPCLKRST_BASEADDR + 4*0x02)
#define XP_AXI_CLK_CNTRL_1              (XPCLKRST_BASEADDR + 4*0x03)
#define XP_AXI_RESET_0                  (XPCLKRST_BASEADDR + 4*0x04)
#define XP_AXI_RESET_1                  (XPCLKRST_BASEADDR + 4*0x05)
#define XP_PHY_CLK_DIV_CNTRL 	        (XPCLKRST_BASEADDR + 4*0x06)
#define XP_PHY_CLK_CNTRL                (XPCLKRST_BASEADDR + 4*0x07)
#define XP_PHY_RESET                    (XPCLKRST_BASEADDR + 4*0x08)
#define XP_TDMNTG_CNTRL                 (XPCLKRST_BASEADDR + 4*0x09)
#define XP_GEMNTG_CNTRL                 (XPCLKRST_BASEADDR + 4*0x0A)
#define XP_SRDS_CNTRL                   (XPCLKRST_BASEADDR + 4*0x0B)
#define XP_EXTPHY_CLK_CNTRL             (XPCLKRST_BASEADDR + 4*0x0C)
#define XP_TDMNTG_BASE                  (XPCLKRST_BASEADDR + 4*0x40)
#define XP_GEMNTG_BASE                  (XPCLKRST_BASEADDR + 4*0x80)

//TDM NTG module
#define TDMNTG_FREQ_SET_INT             (XP_TDMNTG_BASE + 0x00)
#define TDMNTG_FREQ_SET_FRA             (XP_TDMNTG_BASE + 0x04)
#define TDMNTG_PHASE_ADJ_FREQ_INT       (XP_TDMNTG_BASE + 0x10)
#define TDMNTG_PHASE_ADJ_FREQ_FRA       (XP_TDMNTG_BASE + 0x14)
#define TDMNTG_PHASE_ADJ_DUR            (XP_TDMNTG_BASE + 0x18)
#define TDMNTG_PHASE_ADJ_START          (XP_TDMNTG_BASE + 0x1C)
#define TDMNTG_MODE                     (XP_TDMNTG_BASE + 0x20)
#define TDMNTG_PULSE_WIDTH              (XP_TDMNTG_BASE + 0x30)
#define TDMNTG_FRAME_LENGTH             (XP_TDMNTG_BASE + 0x34)
#define TDMNTG_PHASE_FRAME              (XP_TDMNTG_BASE + 0x38)
#define TDMNTG_FRAME_COUNT              (XP_TDMNTG_BASE + 0x3C)

//GEM NTG module
#define GEMNTG_FREQ_SET_INT             (XP_GEMNTG_BASE + 0x00)
#define GEMNTG_FREQ_SET_FRA             (XP_GEMNTG_BASE + 0x04)
#define GEMNTG_PHASE_ADJ_FREQ_INT       (XP_GEMNTG_BASE + 0x10)
#define GEMNTG_PHASE_ADJ_FREQ_FRA       (XP_GEMNTG_BASE + 0x14)
#define GEMNTG_PHASE_ADJ_DUR            (XP_GEMNTG_BASE + 0x18)
#define GEMNTG_PHASE_ADJ_START          (XP_GEMNTG_BASE + 0x1C)
#define GEMNTG_MODE                     (XP_GEMNTG_BASE + 0x20)
#define GEMNTG_PULSE_WIDTH              (XP_GEMNTG_BASE + 0x30)
#define GEMNTG_FRAME_LENGTH             (XP_GEMNTG_BASE + 0x34)
#define GEMNTG_PHASE_FRAME              (XP_GEMNTG_BASE + 0x38)
#define GEMNTG_FRAME_COUNT              (XP_GEMNTG_BASE + 0x3C)

#define NTG_MODE_HALF_EN                (1<<0)  // Half enable
#define NTG_MODE_FRAME_CNT_RST          (1<<3)  // Frame count reset
#define NTG_MODE_CLK_BIT_INV            (1<<4)  // Clock bit inverted
#define NTG_MODE_CLK_BIT_DIR            (1<<5)  // Clock bit direction
#define NTG_MODE_PULSE_FRM_DIR          (1<<6)  // Pulse frame direction
#define NTG_MODE_REG_UPDATE             (1<<7)  // Register update

#define XP_SWRST_REG                    (XP_APB_BASEADDR + 0x00)
#define XP_CSEN_REG                     (XP_APB_BASEADDR + 0x04)
#define XP_CS0_BASE_REG                 (XP_APB_BASEADDR + 0x08)
#define XP_CS1_BASE_REG                 (XP_APB_BASEADDR + 0x0C)
#define XP_CS2_BASE_REG                 (XP_APB_BASEADDR + 0x10)
#define XP_CS3_BASE_REG                 (XP_APB_BASEADDR + 0x14)
#define XP_CS4_BASE_REG                 (XP_APB_BASEADDR + 0x18)
#define XP_CS0_SEG_REG                  (XP_APB_BASEADDR + 0x1C)
#define XP_CS1_SEG_REG                  (XP_APB_BASEADDR + 0x20)
#define XP_CS2_SEG_REG                  (XP_APB_BASEADDR + 0x24)
#define XP_CS3_SEG_REG                  (XP_APB_BASEADDR + 0x28)
#define XP_CS4_SEG_REG                  (XP_APB_BASEADDR + 0x2C)
#define XP_CS0_CFG_REG                  (XP_APB_BASEADDR + 0x30)
#define XP_CS1_CFG_REG                  (XP_APB_BASEADDR + 0x34)
#define XP_CS2_CFG_REG                  (XP_APB_BASEADDR + 0x38)
#define XP_CS3_CFG_REG                  (XP_APB_BASEADDR + 0x3C)
#define XP_CS4_CFG_REG                  (XP_APB_BASEADDR + 0x40)
#define XP_CS0_TMG1_REG                 (XP_APB_BASEADDR + 0x44)
#define XP_CS1_TMG1_REG                 (XP_APB_BASEADDR + 0x48)
#define XP_CS2_TMG1_REG                 (XP_APB_BASEADDR + 0x4C)
#define XP_CS3_TMG1_REG                 (XP_APB_BASEADDR + 0x50)
#define XP_CS4_TMG1_REG                 (XP_APB_BASEADDR + 0x54)
#define XP_CS0_TMG2_REG                 (XP_APB_BASEADDR + 0x58)
#define XP_CS1_TMG2_REG                 (XP_APB_BASEADDR + 0x5C)
#define XP_CS2_TMG2_REG                 (XP_APB_BASEADDR + 0x60)
#define XP_CS3_TMG2_REG                 (XP_APB_BASEADDR + 0x64)
#define XP_CS4_TMG2_REG                 (XP_APB_BASEADDR + 0x68)
#define XP_CS0_TMG3_REG                 (XP_APB_BASEADDR + 0x6C)
#define XP_CS1_TMG3_REG                 (XP_APB_BASEADDR + 0x70)
#define XP_CS2_TMG3_REG                 (XP_APB_BASEADDR + 0x74)
#define XP_CS3_TMG3_REG                 (XP_APB_BASEADDR + 0x78)
#define XP_CS4_TMG3_REG                 (XP_APB_BASEADDR + 0x7C)
#define XP_CLOCK_DIV_REG                (XP_APB_BASEADDR + 0x80)
#define XP_MFSM_REG                     (XP_APB_BASEADDR + 0x100)
#define XP_CSFSM_REG                    (XP_APB_BASEADDR + 0x104)
#define XP_WRFSM_REG                    (XP_APB_BASEADDR + 0x108)
#define XP_RDFSM_REG                    (XP_APB_BASEADDR + 0x10C)

#define XP_CLK_EN       0x00000001
#define XP_CSBOOT_EN    0x00000002
#define XP_CS0_EN       0x00000002
#define XP_CS1_EN       0x00000004
#define XP_CS2_EN       0x00000008
#define XP_CS3_EN       0x00000010
#define XP_CS4_EN       0x00000020
#define XP_MEM_BUS_8    0x00000000
#define XP_MEM_BUS_16   0x00000002              
#define XP_CS_HIGH      0x00000008
#define XP_WE_HIGH      0x00000010
#define XP_RE_HIGH      0x00000020
#define XP_ALE_MODE     0x00000040
#define XP_STRB_MODE    0x00000080
#define XP_DM_MODE      0x00000100
#define XP_NAND_MODE    0x00000200
#define XP_RDY_EN       0x00000400
#define XP_RDY_EDGE     0x00000800

// General Purpose I/O

#define GPIO_OUTPUT_REG                 (GPIO_APB_BASEADDR + 0x00)	
#define GPIO_OE_REG                     (GPIO_APB_BASEADDR + 0x04)	
#define GPIO_INT_CONF_REG_0             (GPIO_APB_BASEADDR + 0x08)
#define GPIO_INT_CONF_REG_1             (GPIO_APB_BASEADDR + 0x0C)
#define GPIO_INPUT_REG                  (GPIO_APB_BASEADDR + 0x10)	
#define APB_ACCESS_WS_REG               (GPIO_APB_BASEADDR + 0x14)
#define SYSCONF_STAT_REG                (GPIO_APB_BASEADDR + 0x1C)
#define XPBUS_CTRL_REG                  (GPIO_APB_BASEADDR + 0x24)
#define TDM_CTRL_REG                    (GPIO_APB_BASEADDR + 0x28)
#define GENERIC_PAD_CTRL_REG            (GPIO_APB_BASEADDR + 0x2C)
#define GPIO_INT_STAT_REG               (GPIO_APB_BASEADDR + 0x30)
#define GPIO_INT_CLEAR_REG              (GPIO_APB_BASEADDR + 0x34)
#define GPIO_INT_MASK_REG               (GPIO_APB_BASEADDR + 0x38)
#define BOOTSTRAP_STATUS_REG            (GPIO_APB_BASEADDR + 0x40)
#define BOOTSTRAP_OVERRIDE_REG          (GPIO_APB_BASEADDR + 0x44)
#define DEVICE_GENERAL_CTRL_REG         (GPIO_APB_BASEADDR + 0x4C)
#define DEVICE_BONDING_STAT_REG         (GPIO_APB_BASEADDR + 0x50)
#define GPIO_PIN_SELECT_REG             (GPIO_APB_BASEADDR + 0x58)
#define SERDES_LANE_CTRL_REG            (GPIO_APB_BASEADDR + 0x60)
#define SERDES_PLL_CONF_REG             (GPIO_APB_BASEADDR + 0x64)
#define SERDES_MGMT_REG                 (GPIO_APB_BASEADDR + 0x68)

#define DEVICE_ROM_REMAP                (1 << 2)

#define GPIO_PIN_SELECT_UART1           ((1 << 22) | (1 << 23))

#define GPIO_0          (1 << 0)
#define GPIO_1          (1 << 1)
#define GPIO_2          (1 << 2)
#define GPIO_3          (1 << 3)
#define GPIO_4          (1 << 4)
#define GPIO_5          (1 << 5)
#define GPIO_6          (1 << 6)
#define GPIO_7          (1 << 7)
#define GPIO_8          (1 << 8)
#define GPIO_9          (1 << 9)
#define GPIO_10         (1 << 10)
#define GPIO_11         (1 << 11)
#define GPIO_12         (1 << 12)
#define GPIO_13         (1 << 13)
#define GPIO_14         (1 << 14)
#define GPIO_15         (1 << 15)

#define GPIO_RISING_EDGE    1
#define GPIO_FALLING_EDGE   2
#define GPIO_BOTH_EDGES	    3


// I2C controller

#define I2C_ADDR_REG                    (I2C_BASEADDR + 0x00*4)
#define I2C_DATA_REG                    (I2C_BASEADDR + 0x01*4)
#define I2C_CNTR_REG                    (I2C_BASEADDR + 0x02*4)
#define I2C_STAT_REG                    (I2C_BASEADDR + 0x03*4)
#define I2C_CCRFS_REG                   (I2C_BASEADDR + 0x03*4)
#define I2C_XADDR_REG                   (I2C_BASEADDR + 0x04*4)
#define I2C_CCRH_REG                    (I2C_BASEADDR + 0x05*4)
#define I2C_SRST_REG                    (I2C_BASEADDR + 0x07*4)
#define I2C_CTRL                        (I2C_BASEADDR + 0x1000) // VLSI to confirm



/*
 * SPU Cluster Blocks
 */

#define SPU_CONFIG                      (SPU_BASEADDR+0x00)
#define SPU_CEVA_MEM_CONFIG             (SPU_BASEADDR+0x04)
#define SPU_AXI_MASTER_PRI_A            (SPU_BASEADDR+0x10)
#define SPU_AXI_MASTER_PRI_B            (SPU_BASEADDR+0x14)
#define SPU_CEVA_BOOT_ADDR              (SPU_BASEADDR+0x18)
#define SPU_TO_DDR1_ADDR_WINDOW         (SPU_BASEADDR+0x20)
#define SPU_TO_DDR0_ADDR_WINDOW         (SPU_BASEADDR+0x24)
#define CLUSTER_CNTRL                   (SPUCLKRST_BASEADDR + 0x4 * 0)
#define SPU_CEVA_CLK_DIV_CNTRL          (SPUCLKRST_BASEADDR + 0x4 * 1)
#define CEVA_CLK_CNTRL_0                (SPUCLKRST_BASEADDR + 0x4 * 2)
#define CEVA_CLK_CNTRL_1                (SPUCLKRST_BASEADDR + 0x4 * 3)
#define CEVA_RESET_0                    (SPUCLKRST_BASEADDR + 0x4 * 4)
#define CEVA_RESET_1                    (SPUCLKRST_BASEADDR + 0x4 * 5)
#define CV_AXI_CLK_DIV_CNTRL            (SPUCLKRST_BASEADDR + 0x4 * 6)
#define CV_AXI_CLK_CNTRL_0              (SPUCLKRST_BASEADDR + 0x4 * 7)
#define CV_AXI_CLK_CNTRL_1              (SPUCLKRST_BASEADDR + 0x4 * 8)
#define SPU_AXI_CLK_DIV_CNTRL           (SPUCLKRST_BASEADDR + 0x4 * 0xA)
#define SPU_AXI_CLK_CNTRL_0             (SPUCLKRST_BASEADDR + 0x4 * 0xB)
#define SPU_AXI_CLK_CNTRL_1             (SPUCLKRST_BASEADDR + 0x4 * 0xC)
#define SPU_AXI_RESET_0                 (SPUCLKRST_BASEADDR + 0x4 * 0xD)
#define SPU_AXI_RESET_1                 (SPUCLKRST_BASEADDR + 0x4 * 0xE)
#define SPU_FFT_CLK_DIV_CNTRL           (SPUCLKRST_BASEADDR + 0x4 * 0x10)
#define FFT_CLK_CNTRL_0                 (SPUCLKRST_BASEADDR + 0x4 * 0x11)
#define FFT_CLK_CNTRL_1                 (SPUCLKRST_BASEADDR + 0x4 * 0x12)
#define FFT_RESET_0                     (SPUCLKRST_BASEADDR + 0x4 * 0x13)
#define FFT_RESET_1                     (SPUCLKRST_BASEADDR + 0x4 * 0x14)

#define SET_IPI_REG                     (INTC_SPU_BASEADDR + 0x00)
#define IPI_STATUS_REG                  (INTC_SPU_BASEADDR + 0x04)
#define CLEAR_IPI_REG                   (INTC_SPU_BASEADDR + 0x08)
#define AUTO_CLEAR_REG                  (INTC_SPU_BASEADDR + 0x0C)
#define SET_NMI_REG                     (INTC_SPU_BASEADDR + 0x10)
#define SET_BP_REG                      (INTC_SPU_BASEADDR + 0x20)
#define BP_STATUS_REG                   (INTC_SPU_BASEADDR + 0x20)
#define CLEAR_BP_REG                    (INTC_SPU_BASEADDR + 0x24)

#define TIMER0_CNTR_REG                 (TIMER_APB_BASEADDR + 0x00)
#define TIMER0_CURR_COUNT               (TIMER_APB_BASEADDR + 0x04)
#define TIMER1_CNTR_REG                 (TIMER_APB_BASEADDR + 0x08)
#define TIMER1_CURR_COUNT               (TIMER_APB_BASEADDR + 0x0C)
#define TIMER2_LBOUND_REG               (TIMER_APB_BASEADDR + 0x10)
#define TIMER2_HBOUND_REG               (TIMER_APB_BASEADDR + 0x14)
#define TIMER2_CNTR_REG                 (TIMER_APB_BASEADDR + 0x18)
#define TIMER2_CURR_COUNT               (TIMER_APB_BASEADDR + 0x1C)
#define TIMER3_LBOUND_REG               (TIMER_APB_BASEADDR + 0x20)
#define TIMER3_HBOUND_REG               (TIMER_APB_BASEADDR + 0x24)
#define TIMER3_CNTR_REG                 (TIMER_APB_BASEADDR + 0x28)
#define TIMER3_CURR_COUNT               (TIMER_APB_BASEADDR + 0x2C)

/*
 * ARM Cortex-A9 Interrupt Distributor
 */

#define A9IC_SCU_BASEADDR               0xfff00000
#define A9IC_CPUINT_BASEADDR            0xfff00100
#define A9IC_DISTR_BASEADDR             0xfff01000

#define A9IC_NUM_INTIDS                 160

// Local to core IRQs

#define HAL_IC_IRQ(id)                  (0 + (id))   // [0 - 15]
#define HAL_IRQ_LOCAL_TIMER             29

#define HAL_IRQ_SP804_TIMER_01          33
#define HAL_IRQ_SP804_TIMER_23          34

// This macro converts interrupt line to SPI (Shared Peripheral Interrupt) ID 
#define SPI_ID(x)                       ((x) + 32)

// Peripheral Interrupt Assignments
#define HAL_IRQ_INTER_ARM_0             SPI_ID(0)
#define HAL_IRQ_INTER_ARM_1             SPI_ID(1)
#define HAL_IRQ_INTER_ARM_2             SPI_ID(2)
#define HAL_IRQ_INTER_ARM_3             SPI_ID(3)
#define HAL_IRQ_INTER_ARM_4             SPI_ID(4)
#define HAL_IRQ_INTER_ARM_5             SPI_ID(5)
#define HAL_IRQ_INTER_ARM_6             SPI_ID(6)
#define HAL_IRQ_INTER_ARM_7             SPI_ID(7)
#define HAL_IRQ_INTER_ARM(id)           (HAL_IRQ_INTER_ARM_0 + (id))
#define HAL_IRQ_ARM2_L2_CACHE           SPI_ID(8)
#define HAL_IRQ_ARM4_L2_CACHE           SPI_ID(9)
#define HAL_IRQ_ARM2_MONITOR            SPI_ID(10)
#define HAL_IRQ_ARM4_MONITOR            SPI_ID(11)
#define HAL_IRQ_FEC_DL                  SPI_ID(12)
#define HAL_IRQ_FEC_UL                  SPI_ID(13)
#define HAL_IRQ_SPAcc                   SPI_ID(14)
#define HAL_IRQ_ESPAH                   SPI_ID(15)
#define HAL_IRQ_DDR_CTRL                SPI_ID(16)
#define HAL_IRQ_MDMA                    SPI_ID(18)
#define HAL_IRQ_CEVA_0                  SPI_ID(30)
#define HAL_IRQ_CEVA_1                  SPI_ID(31)
#define HAL_IRQ_CEVA_2                  SPI_ID(32)
#define HAL_IRQ_CEVA_3                  SPI_ID(33)
#define HAL_IRQ_CEVA_4                  SPI_ID(34)
#define HAL_IRQ_CEVA_5                  SPI_ID(35)
#define HAL_IRQ_CEVA_6                  SPI_ID(36)
#define HAL_IRQ_CEVA_7                  SPI_ID(37)
#define HAL_IRQ_CEVA_8                  SPI_ID(38)
#define HAL_IRQ_CEVA_9                  SPI_ID(39)
#define HAL_IRQ_CEVA(id)                (HAL_IRQ_CEVA_0 + (id))

#define HAL_IRQ_FPDMA0_OUT              SPI_ID(44)
#define HAL_IRQ_FPDMA0_IN               SPI_ID(45)
#define HAL_IRQ_FPDMA1_OUT              SPI_ID(46)
#define HAL_IRQ_FPDMA1_IN               SPI_ID(47)
#define HAL_IRQ_FP_TO_ARM               SPI_ID(48)
#define HAL_IRQ_MDMA_SPU                SPI_ID(49)

#define HAL_IRQ_CPRI_0                  SPI_ID(50)
#define HAL_IRQ_CPRI_1                  SPI_ID(51)
#define HAL_IRQ_CPRI_2                  SPI_ID(52)
#define HAL_IRQ_CPRI_3                  SPI_ID(53)
#define HAL_IRQ_CPRI_4                  SPI_ID(54)
#define HAL_IRQ_CPRI_5                  SPI_ID(55)

#define HAL_IRQ_CPDMA_TX_0              SPI_ID(56)
#define HAL_IRQ_CPDMA_TX_1              SPI_ID(57)
#define HAL_IRQ_CPDMA_TX_2              SPI_ID(58)
#define HAL_IRQ_CPDMA_TX_3              SPI_ID(59)
#define HAL_IRQ_CPDMA_TX_4              SPI_ID(60)
#define HAL_IRQ_CPDMA_TX_5              SPI_ID(61)

#define HAL_IRQ_CPDMA_RX_0              SPI_ID(62)
#define HAL_IRQ_CPDMA_RX_1              SPI_ID(63)
#define HAL_IRQ_CPDMA_RX_2              SPI_ID(64)
#define HAL_IRQ_CPDMA_RX_3              SPI_ID(65)
#define HAL_IRQ_CPDMA_RX_4              SPI_ID(66)
#define HAL_IRQ_CPDMA_RX_5              SPI_ID(67)

#define HAL_IRQ_SRIO_0                  SPI_ID(68)
#define HAL_IRQ_SRIO_1                  SPI_ID(69)

#define HAL_IRQ_PCIE_0                  SPI_ID(70)
#define HAL_IRQ_PCIE_1                  SPI_ID(71)
#define HAL_IRQ_PCIE_2                  SPI_ID(72)
#define HAL_IRQ_PCIE_3                  SPI_ID(73)

#define HAL_IRQ_MDMA_RAD                SPI_ID(75)

#define HAL_IRQ_GEM_0                   SPI_ID(100)
#define HAL_IRQ_ADM_0                   SPI_ID(101)
#define HAL_IRQ_GEM_1                   SPI_ID(102)
#define HAL_IRQ_ADM_1                   SPI_ID(103)

#define HAL_IRQ_SGMII_SERDES            SPI_ID(104)
#define HAL_IRQ_GEM_NTG                 SPI_ID(105)

#define HAL_IRQ_GPIO_0                  SPI_ID(106)
#define HAL_IRQ_GPIO_1                  SPI_ID(107)
#define HAL_IRQ_GPIO_2                  SPI_ID(108)
#define HAL_IRQ_GPIO_3                  SPI_ID(109)
#define HAL_IRQ_GPIO(id)                (HAL_IRQ_GPIO_0 + (id))
#define HAL_IRQ_GPIO_COMB               SPI_ID(110)

#define HAL_IRQ_TIMER                   SPI_ID(112)
#define HAL_IRQ_TDM_TIMER               SPI_ID(113)

#define HAL_IRQ_UART_0                  SPI_ID(114)
#define HAL_IRQ_UART_1                  SPI_ID(115)
#define HAL_IRQ_UART_2                  SPI_ID(116)
#define HAL_IRQ_UART(id)                (HAL_IRQ_UART_0 + (id))

#define HAL_IRQ_SPI                     SPI_ID(117)
#define HAL_IRQ_I2C                     SPI_ID(118)
#define HAL_IRQ_HBI                     SPI_ID(119)    // HostBus mailbox irq

#define HAL_IRQ_TDM_NTG                 SPI_ID(121)
#define HAL_IRQ_TDM_SIG                 SPI_ID(122)
#define HAL_IRQ_TDM_ERR_RX              SPI_ID(123)
#define HAL_IRQ_TDM_ERR_TX              SPI_ID(124)
#define HAL_IRQ_TDMA                    SPI_ID(125)
#define HAL_IRQ_TDM_TX                  SPI_ID(126)
#define HAL_IRQ_TDM_RX                  SPI_ID(127)

#define HAL_IRQPRI(pri)                 (pri<<3)

/*
 * ARM L2CC
 */
#define L2CC_BASE                       0xFFF10000
#define L2CC_ID                         (L2CC_BASE + 0x000)
#define L2CC_TYPE                       (L2CC_BASE + 0x004)
#define L2CC_CONTROL                    (L2CC_BASE + 0x100)
#define L2CC_AUX_CONTROL                (L2CC_BASE + 0x104)
#define L2CC_TAG_LATE                   (L2CC_BASE + 0x108)
#define L2CC_DATA_LATE                  (L2CC_BASE + 0x10C)
#define L2CC_EVENT_CONTROL              (L2CC_BASE + 0x200)
#define L2CC_EVENT_CONFIG1              (L2CC_BASE + 0x204)
#define L2CC_EVENT_CONFIG0              (L2CC_BASE + 0x208)
#define L2CC_EVENT_VALUE1               (L2CC_BASE + 0x20C)
#define L2CC_EVENT_VALUE0               (L2CC_BASE + 0x210)
#define L2CC_INTR_MASK                  (L2CC_BASE + 0x214)
#define L2CC_MASKED_INTR_STAT           (L2CC_BASE + 0x218)
#define L2CC_RAW_INTR_STAT              (L2CC_BASE + 0x21C)
#define L2CC_INTR_CLR                   (L2CC_BASE + 0x220)
#define L2CC_CACHE_SYNC                 (L2CC_BASE + 0x730)
#define L2CC_INVL_LINE_PA               (L2CC_BASE + 0x770)
#define L2CC_INVL_WAY                   (L2CC_BASE + 0x77C)
#define L2CC_CLN_LINE_PA                (L2CC_BASE + 0x7B0)
#define L2CC_CLN_INDX_WAY               (L2CC_BASE + 0x7B8)
#define L2CC_CLN_WAY                    (L2CC_BASE + 0x7BC)
#define L2CC_CLN_INVL_LINE_PA           (L2CC_BASE + 0x7F0)
#define L2CC_CLN_INVL_LINE_WAY_INDX     (L2CC_BASE + 0x7F8)
#define L2CC_CLEAN_INV_WAY              (L2CC_BASE + 0x7FC)
#define L2CC_DEBUG_CONTROL              (L2CC_BASE + 0xF40)

/*
 * ARM Cortex-A9 Interrupt Distributor
 */

#define A9TMWD_BASEADDR                 0xfff00600

/***********************************************************************
**  The  DMA Control Mask and IRQ Status registers                     *
************************************************************************/
#define DMAIRQ_FREADY                   (1 << 0)
#define DMAIRQ_FLAST                    (1 << 1)
#define DMAIRQ_FDONE                    (1 << 2)
#define DMAIRQ_FCTRL                    (1 << 6)

#define FDESC_INTENA                    1

#define TDM_BUS_BASEADDR(bus)           (TDM_APB_BASEADDR + (bus << 12))
#define TDMA_BUS_BASEADDR(bus)          (TDMA_APB_BASEADDR + (bus << 13))

#define CPDMA_BASEADDR(n)               (RAD_APB_CPDMA_BASEADDR + (n * 0x01000))
#define RADIP_BASEADDR(n)               (CPRI_BASEADDR  + (n * 0x20000))

#endif // APB_H_
