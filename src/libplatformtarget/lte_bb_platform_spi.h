
#include "lte_bb_misc_api.h"

#define LTE_BB_AD9548_PART      "/sys/bus/spi/devices/spi0.1/ad9548"
#define LTE_BB_CDCE62005_PART   "/sys/bus/spi/devices/spi0.2/cdce62005"
#define SYS_CLK_AD9548_REGISTER      0x0D01
#define SYS_FREQ_AD9548_REGISTER     0x0D0A
#define DLL_AD9548_REGISTER          0x0D0B
#define CDCE_REG008_REGISTER         0x08

#ifndef LTE_BB_PLTM_SPI
#define LTE_BB_PLTM_SPI
extern Ad9548ReturnCode WriteAD9548(unsigned int RegAddr, unsigned char RegData);
extern Ad9548ReturnCode ReadAD9548(unsigned int RegAddr, unsigned char *RegData);
extern CdceReturnCode ReadCDCE620005(UINT8 RegAddr,UINT32 *RegData);
#endif

