//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Hal_srio.h
 *
 * @brief SRIO driver header file
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

#ifndef __SRIO_H__
#define __SRIO_H__

#include "hal.h"


#define SRIO_INT_AXI_PIO   (1 << 0)
#define SRIO_INT_RIO_PIO   (1 << 1)
#define SRIO_INT_WDMA      (1 << 2)
#define SRIO_INT_RDMA      (1 << 3)
#define SRIO_INT_IDME      (1 << 4)
#define SRIO_INT_ODME      (1 << 5)
#define SRIO_INT_MISC      (1 << 6)

#define SRIO_MISC_INT_IB_DB_RECVD      (1 << 0)
#define SRIO_MISC_INT_OB_DB_SENT       (1 << 1)
#define SRIO_MISC_INT_UNEXP_MSG        (1 << 2)
#define SRIO_MISC_INT_UNSUP_RIO_REQ    (1 << 3)
#define SRIO_MISC_INT_PORT_WRITE       (1 << 4)
#define SRIO_MISC_INT_GRIO_ERR         (1 << 5)
#define SRIO_MISC_INT_GRIO_LTL_ERR     (1 << 6)
#define SRIO_MISC_INT_GRIO_LYNC_RESET  (1 << 7)
#define SRIO_MISC_INT_LOCK_CLEAR       (1 << 8)
#define SRIO_MISC_INT_LFC_BL_CHANGED   (1 << 9)
#define SRIO_MISC_INT_AXI_SLAVE        (1 << 10)
#define SRIO_MISC_INT_AXI_MASTER0      (1 << 11)



#define SRIO_DMA_CTRL_VALID         (1 << 0)
#define SRIO_DMA_CTRL_NEXT_OK       (1 << 1)
#define SRIO_DMA_STAT_DONE          (1 << 24)
#define SRIO_DMA_STAT_AXI_ERR       (1 << 25)
#define SRIO_DMA_STAT_RIO_ERR       (1 << 26)
#define SRIO_DMA_STAT_FETCH_ERR     (1 << 27)
#define SRIO_DMA_STAT_UPDATE_ERR    (1 << 28)

#define SRIO_DMA_STAT_ERR_MASK      (                               \
                                    SRIO_DMA_STAT_AXI_ERR   |       \
                                    SRIO_DMA_STAT_RIO_ERR   |       \
                                    SRIO_DMA_STAT_FETCH_ERR |       \
                                    SRIO_DMA_STAT_UPDATE_ERR        \
                                    )

#define BASE_RAD_SRIO(id)	(((id) == 0) ? (RAD_APB_SRIO0_BASEADDR) : (RAD_APB_SRIO1_BASEADDR))

// SRIO0 Registers
#define RAD_SRIO_RAB_CTRL(id)               ( BASE_RAD_SRIO(id)	+ 0x20 )
#define RAD_SRIO_RAB_AXI_TIMEOUT(id)        ( BASE_RAD_SRIO(id)	+ 0x40 )
#define RAD_SRIO_AXI_TIMER_CNT(id)          ( BASE_RAD_SRIO(id)	+ 0x40 )
#define RAD_SRIO_RAB_RST_CTRL(id)           ( BASE_RAD_SRIO(id)	+ 0x60 )
#define RAD_SRIO_RAB_COOP_LOCK(id)          ( BASE_RAD_SRIO(id)	+ 0x70 )
#define RAD_SRIO_RAB_STAT_STAT(id)          ( BASE_RAD_SRIO(id)	+ 0x90 )
#define RAD_SRIO_RAB_APB_CSR(id)            ( BASE_RAD_SRIO(id)	+ 0xC0 )
#define RAD_SRIO_RAB_ARB_TIMEOUT(id)        ( BASE_RAD_SRIO(id)	+ 0xD0 )
#define RAD_SRIO_RAB_DESC_RDY_TIMEOUT(id)   ( BASE_RAD_SRIO(id)	+ 0xE0 )


#define RAD_SRIO_RAB_INTR_ENAB_GNRL(id)     ( BASE_RAD_SRIO(id)	+ 0x100 )
#define RAD_SRIO_RAB_INTR_ENAB_APIO(id)     ( BASE_RAD_SRIO(id)	+ 0x110 )
#define RAD_SRIO_RAB_INTR_ENAB_RPIO(id)     ( BASE_RAD_SRIO(id)	+ 0x120 )
#define RAD_SRIO_RAB_INTR_ENAB_WDMA(id)     ( BASE_RAD_SRIO(id)	+ 0x130 )
#define RAD_SRIO_RAB_INTR_ENAB_RDMA(id)     ( BASE_RAD_SRIO(id)	+ 0x140 )
#define RAD_SRIO_RAB_INTR_ENAB_IDME(id)     ( BASE_RAD_SRIO(id)	+ 0x150 )
#define RAD_SRIO_RAB_INTR_ENAB_ODME(id)     ( BASE_RAD_SRIO(id)	+ 0x160 )
#define RAD_SRIO_RAB_INTR_ENAB_MISC(id)     ( BASE_RAD_SRIO(id)	+ 0x170 )

#define RAD_SRIO_RAB_INTR_STAT_GNRL(id)     ( BASE_RAD_SRIO(id)	+ 0x180 )
#define RAD_SRIO_RAB_INTR_STAT_APIO(id)     ( BASE_RAD_SRIO(id)	+ 0x190 )
#define RAD_SRIO_RAB_INTR_STAT_RPIO(id)     ( BASE_RAD_SRIO(id)	+ 0x1A0 )
#define RAD_SRIO_RAB_INTR_STAT_WDMA(id)     ( BASE_RAD_SRIO(id)	+ 0x1B0 )
#define RAD_SRIO_RAB_INTR_STAT_RDMA(id)     ( BASE_RAD_SRIO(id)	+ 0x1C0 )
#define RAD_SRIO_RAB_INTR_STAT_IDME(id)     ( BASE_RAD_SRIO(id)	+ 0x1D0 )
#define RAD_SRIO_RAB_INTR_STAT_ODME(id)     ( BASE_RAD_SRIO(id)	+ 0x1E0 )
#define RAD_SRIO_RAB_INTR_STAT_MISC(id)     ( BASE_RAD_SRIO(id)	+ 0x1F0 )


#define RAD_SRIO_RAB_RPIO0_CTRL(id)          ( BASE_RAD_SRIO(id)	+ 0x200 )
#define RAD_SRIO_RAB_RPIO0_STAT(id)          ( BASE_RAD_SRIO(id)	+ 0x210 )
#define RAD_SRIO_RAB_RPIO1_CTRL(id)          ( BASE_RAD_SRIO(id)	+ 0x220 )
#define RAD_SRIO_RAB_RPIO1_STAT(id)          ( BASE_RAD_SRIO(id)	+ 0x230 )
#define RAD_SRIO_RAB_RPIO2_CTRL(id)          ( BASE_RAD_SRIO(id)	+ 0x240 )
#define RAD_SRIO_RAB_RPIO2_STAT(id)          ( BASE_RAD_SRIO(id)	+ 0x250 )


#define RAD_SRIO_RAB_RIO_AMAP_LUT0(id)      ( BASE_RAD_SRIO(id)	+ 0x400 )
#define RAD_SRIO_RAB_RIO_AMAP_LUT1(id)      ( BASE_RAD_SRIO(id)	+ 0x410 )
#define RAD_SRIO_RAB_RIO_AMAP_LUT2(id)      ( BASE_RAD_SRIO(id)	+ 0x420 )
#define RAD_SRIO_RAB_RIO_AMAP_LUT3(id)      ( BASE_RAD_SRIO(id)	+ 0x430 )
#define RAD_SRIO_RAB_RIO_AMAP_LUT4(id)      ( BASE_RAD_SRIO(id)	+ 0x440 )
#define RAD_SRIO_RAB_RIO_AMAP_LUT5(id)      ( BASE_RAD_SRIO(id)	+ 0x450 )
#define RAD_SRIO_RAB_RIO_AMAP_LUT6(id)      ( BASE_RAD_SRIO(id)	+ 0x460 )
#define RAD_SRIO_RAB_RIO_AMAP_LUT7(id)      ( BASE_RAD_SRIO(id)	+ 0x470 )
#define RAD_SRIO_RAB_RIO_AMAP_LUT8(id)      ( BASE_RAD_SRIO(id)	+ 0x480 )
#define RAD_SRIO_RAB_RIO_AMAP_LUT9(id)      ( BASE_RAD_SRIO(id)	+ 0x490 )
#define RAD_SRIO_RAB_RIO_AMAP_LUT10(id)     ( BASE_RAD_SRIO(id)	+ 0x4A0 )
#define RAD_SRIO_RAB_RIO_AMAP_LUT11(id)     ( BASE_RAD_SRIO(id)	+ 0x4B0 )
#define RAD_SRIO_RAB_RIO_AMAP_LUT12(id)     ( BASE_RAD_SRIO(id)	+ 0x4C0 )
#define RAD_SRIO_RAB_RIO_AMAP_LUT13(id)     ( BASE_RAD_SRIO(id)	+ 0x4D0 )
#define RAD_SRIO_RAB_RIO_AMAP_LUT14(id)     ( BASE_RAD_SRIO(id)	+ 0x4E0 )
#define RAD_SRIO_RAB_RIO_AMAP_LUT15(id)     ( BASE_RAD_SRIO(id)	+ 0x4F0 )

#define RAD_SRIO_RAB_RIO_AMAP_IDSL(id)      ( BASE_RAD_SRIO(id)	+ 0x500 )
#define RAD_SRIO_RAB_RIO_AMAP_BYPS(id)      ( BASE_RAD_SRIO(id)	+ 0x510 )


#define RAD_SRIO_RAB_APIO_0_CTRL(id)        ( BASE_RAD_SRIO(id)	+ 0x600 )
#define RAD_SRIO_RAB_APIO_0_STAT(id)        ( BASE_RAD_SRIO(id)	+ 0x610 )

#define RAD_SRIO_RAB_APIO_AMAP_0_CTRL(id)   ( BASE_RAD_SRIO(id)	+ 0x800 )
#define RAD_SRIO_RAB_APIO_AMAP_0_SIZE(id)   ( BASE_RAD_SRIO(id)	+ 0x810 )
#define RAD_SRIO_RAB_APIO_AMAP_0_ABAR(id)   ( BASE_RAD_SRIO(id)	+ 0x820 )
#define RAD_SRIO_RAB_APIO_AMAP_0_RBAR(id)   ( BASE_RAD_SRIO(id)	+ 0x830 )


#define RAD_SRIO_RAB_OB_DB_0_CSR(id)        ( BASE_RAD_SRIO(id)	+ 0x1000 )
#define RAD_SRIO_RAB_OB_DB_0_INFO(id)       ( BASE_RAD_SRIO(id)	+ 0x1010 )
#define RAD_SRIO_RAB_OB_DB_1_CSR(id)        ( BASE_RAD_SRIO(id)	+ 0x1020 )
#define RAD_SRIO_RAB_OB_DB_1_INFO(id)       ( BASE_RAD_SRIO(id)	+ 0x1030 )
#define RAD_SRIO_RAB_OB_DB_2_CSR(id)        ( BASE_RAD_SRIO(id)	+ 0x1040 )
#define RAD_SRIO_RAB_OB_DB_2_INFO(id)       ( BASE_RAD_SRIO(id)	+ 0x1050 )
#define RAD_SRIO_RAB_OB_DB_3_CSR(id)        ( BASE_RAD_SRIO(id)	+ 0x1060 )
#define RAD_SRIO_RAB_OB_DB_3_INFO(id)       ( BASE_RAD_SRIO(id)	+ 0x1070 )
#define RAD_SRIO_RAB_OB_DB_4_CSR(id)        ( BASE_RAD_SRIO(id)	+ 0x1080 )
#define RAD_SRIO_RAB_OB_DB_4_INFO(id)       ( BASE_RAD_SRIO(id)	+ 0x1090 )
#define RAD_SRIO_RAB_OB_DB_5_CSR(id)        ( BASE_RAD_SRIO(id)	+ 0x10A0 )
#define RAD_SRIO_RAB_OB_DB_5_INFO(id)       ( BASE_RAD_SRIO(id)	+ 0x10B0 )
#define RAD_SRIO_RAB_OB_DB_6_CSR(id)        ( BASE_RAD_SRIO(id)	+ 0x10C0 )
#define RAD_SRIO_RAB_OB_DB_6_INFO(id)       ( BASE_RAD_SRIO(id)	+ 0x10D0 )
#define RAD_SRIO_RAB_OB_DB_7_CSR(id)        ( BASE_RAD_SRIO(id)	+ 0x10E0 )
#define RAD_SRIO_RAB_OB_DB_7_INFO(id)       ( BASE_RAD_SRIO(id)	+ 0x10F0 )

#define RAD_SRIO_RAB_IB_DB_CSR(id)          ( BASE_RAD_SRIO(id)	+ 0x1200 )
#define RAD_SRIO_RAB_IB_DB_INFO(id)         ( BASE_RAD_SRIO(id)	+ 0x1210 )

#define RAD_SRIO_RAB_OB_DME0_CTRL(id)       ( BASE_RAD_SRIO(id)	+ 0x1400 )
#define RAD_SRIO_RAB_OB_DME0_ADDR(id)       ( BASE_RAD_SRIO(id)	+ 0x1410 )
#define RAD_SRIO_RAB_OB_DME0_STAT(id)       ( BASE_RAD_SRIO(id)	+ 0x1420 )
#define RAD_SRIO_RAB_OB_DME0_DESC(id)       ( BASE_RAD_SRIO(id)	+ 0x1430 )

#define RAD_SRIO_RAB_OB_DME1_CTRL(id)       ( BASE_RAD_SRIO(id)	+ 0x1440 )
#define RAD_SRIO_RAB_OB_DME1_ADDR(id)       ( BASE_RAD_SRIO(id)	+ 0x1450 )
#define RAD_SRIO_RAB_OB_DME1_STAT(id)       ( BASE_RAD_SRIO(id)	+ 0x1460 )
#define RAD_SRIO_RAB_OB_DME1_DESC(id)       ( BASE_RAD_SRIO(id)	+ 0x1470 )

#define RAD_SRIO_RAB_IB_DME0_CTRL(id)       ( BASE_RAD_SRIO(id)	+ 0x1800 )
#define RAD_SRIO_RAB_IB_DME0_ADDR(id)       ( BASE_RAD_SRIO(id)	+ 0x1810 )
#define RAD_SRIO_RAB_IB_DME0_STAT(id)       ( BASE_RAD_SRIO(id)	+ 0x1820 )
#define RAD_SRIO_RAB_IB_DME0_DESC(id)       ( BASE_RAD_SRIO(id)	+ 0x1830 )

#define RAD_SRIO_RAB_IB_DME1_CTRL(id)       ( BASE_RAD_SRIO(id)	+ 0x1840 )
#define RAD_SRIO_RAB_IB_DME1_ADDR(id)       ( BASE_RAD_SRIO(id)	+ 0x1850 )
#define RAD_SRIO_RAB_IB_DME1_STAT(id)       ( BASE_RAD_SRIO(id)	+ 0x1860 )
#define RAD_SRIO_RAB_IB_DME1_DESC(id)       ( BASE_RAD_SRIO(id)	+ 0x1870 )

#define RAD_SRIO_RAB_IB_DME2_CTRL(id)       ( BASE_RAD_SRIO(id)	+ 0x1880 )
#define RAD_SRIO_RAB_IB_DME2_ADDR(id)       ( BASE_RAD_SRIO(id)	+ 0x1890 )
#define RAD_SRIO_RAB_IB_DME2_STAT(id)       ( BASE_RAD_SRIO(id)	+ 0x18A0 )
#define RAD_SRIO_RAB_IB_DME2_DESC(id)       ( BASE_RAD_SRIO(id)	+ 0x18B0 )

#define RAD_SRIO_RAB_IB_DME3_CTRL(id)       ( BASE_RAD_SRIO(id)	+ 0x18C0 )
#define RAD_SRIO_RAB_IB_DME3_ADDR(id)       ( BASE_RAD_SRIO(id)	+ 0x18D0 )
#define RAD_SRIO_RAB_IB_DME3_STAT(id)       ( BASE_RAD_SRIO(id)	+ 0x18E0 )
#define RAD_SRIO_RAB_IB_DME3_DESC(id)       ( BASE_RAD_SRIO(id)	+ 0x18F0 )

#define RAD_SRIO_RAB_IB_DME4_CTRL(id)       ( BASE_RAD_SRIO(id)	+ 0x1900 )
#define RAD_SRIO_RAB_IB_DME4_ADDR(id)       ( BASE_RAD_SRIO(id)	+ 0x1910 )
#define RAD_SRIO_RAB_IB_DME4_STAT(id)       ( BASE_RAD_SRIO(id)	+ 0x1920 )
#define RAD_SRIO_RAB_IB_DME4_DESC(id)       ( BASE_RAD_SRIO(id)	+ 0x1930 )

#define RAD_SRIO_RAB_IB_DME5_CTRL(id)       ( BASE_RAD_SRIO(id)	+ 0x1940 )
#define RAD_SRIO_RAB_IB_DME5_ADDR(id)       ( BASE_RAD_SRIO(id)	+ 0x1950 )
#define RAD_SRIO_RAB_IB_DME5_STAT(id)       ( BASE_RAD_SRIO(id)	+ 0x1960 )
#define RAD_SRIO_RAB_IB_DME5_DESC(id)       ( BASE_RAD_SRIO(id)	+ 0x1970 )

#define RAD_SRIO_RAB_IB_DME6_CTRL(id)       ( BASE_RAD_SRIO(id)	+ 0x1980 )
#define RAD_SRIO_RAB_IB_DME6_ADDR(id)       ( BASE_RAD_SRIO(id)	+ 0x1990 )
#define RAD_SRIO_RAB_IB_DME6_STAT(id)       ( BASE_RAD_SRIO(id)	+ 0x19A0 )
#define RAD_SRIO_RAB_IB_DME6_DESC(id)       ( BASE_RAD_SRIO(id)	+ 0x19B0 )

#define RAD_SRIO_RAB_IB_DME7_CTRL(id)       ( BASE_RAD_SRIO(id)	+ 0x19C0 )
#define RAD_SRIO_RAB_IB_DME7_ADDR(id)       ( BASE_RAD_SRIO(id)	+ 0x19D0 )
#define RAD_SRIO_RAB_IB_DME7_STAT(id)       ( BASE_RAD_SRIO(id)	+ 0x19E0 )
#define RAD_SRIO_RAB_IB_DME7_DESC(id)       ( BASE_RAD_SRIO(id)	+ 0x19F0 )

#define RAD_SRIO_RAB_WDMA_0_CTRL(id)        ( BASE_RAD_SRIO(id)	+ 0x2000 )
#define RAD_SRIO_RAB_WDMA_0_ADDR(id)        ( BASE_RAD_SRIO(id)	+ 0x2010 )
#define RAD_SRIO_RAB_WDMA_0_STAT(id)        ( BASE_RAD_SRIO(id)	+ 0x2020 )
#define RAD_SRIO_RAB_RDMA_0_CTRL(id)        ( BASE_RAD_SRIO(id)	+ 0x2200 )
#define RAD_SRIO_RAB_RDMA_0_ADDR(id)        ( BASE_RAD_SRIO(id)	+ 0x2210 )
#define RAD_SRIO_RAB_RDMA_0_STAT(id)        ( BASE_RAD_SRIO(id)	+ 0x2220 )

#define RAD_SRIO_RAB_DMA_IADDR_DESC_SEL(id)         ( BASE_RAD_SRIO(id)	+ 0x2400 )
#define RAD_SRIO_RAB_DMA_IADDR_DESC_CTRL(id)        ( BASE_RAD_SRIO(id)	+ 0x2410 )
#define RAD_SRIO_RAB_DMA_IADDR_DESC_STAT(id)        ( BASE_RAD_SRIO(id)	+ 0x2420 )
#define RAD_SRIO_RAB_DMA_IADDR_DESC_SRC_ADDR(id)    ( BASE_RAD_SRIO(id)	+ 0x2430 )
#define RAD_SRIO_RAB_DMA_IADDR_DESC_DEST_ADDR(id)   ( BASE_RAD_SRIO(id)	+ 0x2440 )
#define RAD_SRIO_RAB_DMA_IADDR_DESC_NEXT_ADDR(id)   ( BASE_RAD_SRIO(id)	+ 0x2450 )

#define RAD_SRIO_RAB_LFC_0_CSR(id)         ( BASE_RAD_SRIO(id)	+ 0x2480 )
#define RAD_SRIO_RAB_LFC_TIMEOUT(id)       ( BASE_RAD_SRIO(id)	+ 0x2580 )
#define RAD_SRIO_RAB_LFC_BLOCKED(id)       ( BASE_RAD_SRIO(id)	+ 0x2590 )




#define RAD_SRIO_GRIO_DIDCAR(id)            ( BASE_RAD_SRIO(id)	+ 0x2000 )
#define RAD_SRIO_GRIO_DICAR(id)             ( BASE_RAD_SRIO(id)	+ 0x2010 )
#define RAD_SRIO_GRIO_AIDCAR(id)            ( BASE_RAD_SRIO(id)	+ 0x2020 )
#define RAD_SRIO_GRIO_AICAR(id)             ( BASE_RAD_SRIO(id)	+ 0x2030 )
#define RAD_SRIO_GRIO_PEFCAR(id)            ( BASE_RAD_SRIO(id)	+ 0x2040 )
#define RAD_SRIO_GRIO_BDIDCSR(id)           ( BASE_RAD_SRIO(id)	+ 0x2180 )
#define RAD_SRIO_GRIO_LMREQCSR(id)          ( BASE_RAD_SRIO(id)	+ 0x2500 )
#define RAD_SRIO_GRIO_LMRESPCSR(id)          ( BASE_RAD_SRIO(id)	+ 0x2510    )
#define RAD_SRIO_GRIO_P0ESCSR(id)           ( BASE_RAD_SRIO(id)	+ 0x2560 )
#define RAD_SRIO_GRIO_P0CCSR(id)            ( BASE_RAD_SRIO(id)	+ 0x2570 )

#define RAD_SRIO_GRIO_LTLEDCSR(id)          ( BASE_RAD_SRIO(id)	+ 0x3820 )
#define RAD_SRIO_GRIO_LTLEECSR(id)          ( BASE_RAD_SRIO(id)	+ 0x3830 )
#define RAD_SRIO_GRIO_LTLACCSR(id)          ( BASE_RAD_SRIO(id)	+ 0x3850 )

#define RAD_SRIO_GRIO_P0EDCSR(id)           ( BASE_RAD_SRIO(id)	+ 0x3900 )
#define RAD_SRIO_GRIO_P0ERECSR(id)          ( BASE_RAD_SRIO(id)	+ 0x3910 )
#define RAD_SRIO_GRIO_P0ECACSR(id)          ( BASE_RAD_SRIO(id)	+ 0x3920 )
#define RAD_SRIO_GRIO_P0ERCSR(id)           ( BASE_RAD_SRIO(id)	+ 0x39A0 )

#define RAD_SRIO_GRIO_P0IECSR(id)           ( BASE_RAD_SRIO(id)	+ 0x24C0 ) //page 0x20
#define RAD_SRIO_GRIO_P0PCR(id)             ( BASE_RAD_SRIO(id)	+ 0x2500 ) //page 0x20
#define RAD_SRIO_GRIO_P0SLCSR(id)           ( BASE_RAD_SRIO(id)	+ 0x2560 ) //page 0x20


   // Different page
#define RAD_SRIO_GRIO_PNPCR(id)             ( BASE_RAD_SRIO(id)	+ 0x2500 )
#define RAD_SRIO_GRIO_PNPTAACR(id)          ( BASE_RAD_SRIO(id)	+ 0x2480 )
#define RAD_SRIO_GRIO_PNSLEICR(id)          ( BASE_RAD_SRIO(id)	+ 0x2580 )

#endif

#ifdef __cplusplus
}
#endif

