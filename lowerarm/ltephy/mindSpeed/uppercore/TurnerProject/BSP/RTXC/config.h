#ifndef __CTL_CONFIG_H_
#define __CTL_CONFIG_H_

/*
 * SDRAM
 */
#undef  CFG_SDRAM
#define CFG_SDRAM_BASE          0x0         /* SDRAM Base 	*/
#define CFG_SDRAM_SIZE          0x8000000   /* 128 Meg 	*/
#define CFG_SDRAM_CFG1          0xD2DE      /* 2.5-3-3-9-6 	*/
#define CFG_SDRAM_CFG2          0x92CA      /* tWR=TRRD=tMRD=2,set mode,DLL,x8, */
#define CFG_SDRAM_REFRESH       0x0820      /* 15.6 us refresh */
#define CFG_SDRAM_POWERON       0x682B      /* 200 us power on */

/*
 * System clocks (PLL)
 */
#undef CFG_PLL
#define CFG_SPU_CLOCK           375000000   /* 375   MHz */

/*
 * MMU and cache control
 */
#define CFG_IC_ENABLE                       /* Enable ICache */
#define CFG_DC_ENABLE                       /* Enable DCache */

/*
 * Memory Map
 */
#define CFG_STACK_SIZE		2048		/* ARM0 stack size */
#define CFG_ABORT_SIZE		128		/* Abort stack size */
#define CFG_UNDEF_SIZE		128		/* Undefined stack size */
#define CFG_FIQ_SIZE		256		/* FIQ stack size */
#define CFG_IRQ_SIZE		512		/* IRQ stack size */
#define CFG_SVC_SIZE		1024		/* SVC stack size */

/*
 * Console uart
 */
#define CONFIG_STDIO_UARTID	0
#define CONFIG_BAUD_RATE	115200

/*
 * 	Quadnet
 */
#define CONFIG_ETHADDR		00:11:22:33:44:55
#define CONFIG_ETH1ADDR         00:11:22:33:44:56
#define CONFIG_IPADDR 		192.168.2.2
#define CONFIG_NETMASK 		255.255.255.0
#define CONFIG_GW_IP   		192.168.2.1

/*
 * 	Gemac
 */
#define GEM_MDIO_ENABLED 	(0)
#define GEM_DEF_PCLK_DIV 	(MDC_DIV_32)
#define GEM_DEF_AHB_WIDTH 	(AMBA_AHB_32)
#define GEM_DEF_DUPLEX 		(GEM_FULL_DUPLEX)
#define GEM_DEF_SPEED 		(SPEED_1000M)
#define GEM_DEF_LOOP 		(LB_NONE)

#define GEMAC_SW_CONF		(1 << 8) | (1 << 11)	// GEMAC configured by SW
#define GEMAC_PHY_CONF		0			// GEMAC configured by phy lines (not for MII/GMII)
#define GEMAC_SW_FULL_DUPLEX	(1 << 9)
#define GEMAC_SW_SPEED_10M	(0 << 12)
#define GEMAC_SW_SPEED_100M	(1 << 12)
#define GEMAC_SW_SPEED_1G	(2 << 12)

#define CONFIG_COMCERTO_USE_MII		1
#define CONFIG_COMCERTO_USE_RMII	2
#define CONFIG_COMCERTO_USE_GMII	4
#define CONFIG_COMCERTO_USE_RGMII	8

#define GEMAC0_PHY_AUTONEG	0	// No Phy autoneg
#define GEMAC0_PHY_1000		1	// GIGE phy
#define GEMAC0_PHY_ADDR		0
#define GEMAC0_CONFIG		CONFIG_COMCERTO_USE_GMII
#define GEMAC0_MODE		GEMAC_SW_CONF | GEMAC_SW_FULL_DUPLEX | GEMAC_SW_SPEED_1G
#define GEMAC0_PHYIDX		0

#define GEMAC1_PHY_AUTONEG	0	// No Phy autoneg
#define GEMAC1_PHY_1000		1	// GIGE phy
#define GEMAC1_PHY_ADDR		0
#define GEMAC1_CONFIG		CONFIG_COMCERTO_USE_GMII
#define GEMAC1_MODE		GEMAC_SW_CONF | GEMAC_SW_FULL_DUPLEX | GEMAC_SW_SPEED_1G
#define GEMAC1_PHYIDX		0

#endif /* __CTL_CONFIG_H_ */
