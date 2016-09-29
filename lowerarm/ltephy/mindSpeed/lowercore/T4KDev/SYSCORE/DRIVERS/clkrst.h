//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL clkrst.h
 *
 * @brief Clock and reset control block
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
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

#include "config.h"

#ifndef _CLK_RST_H_
#define _CLK_RST_H_

#include "4gmx.h"
#include "hal.h"
#include "appids.h"

#define CLK_RST_REF_FREQ        (25000000)  // 25 MHz
#define CLK_RST_ALT_FREQ        (156250000) // 156.25 MHz

#define CLK_RST_REF_PLL         0xFF


/*******************************************************************
*               The list of error codes                            *
********************************************************************/
#define RC_CLKRST_ID_ERROR      DEF_USRRC(APP_DRIVER, CLKRST_DRV, 1)
#define RC_CLKRST_FREQ_ERROR    DEF_USRRC(APP_DRIVER, CLKRST_DRV, 2)
#define RC_CLKRST_NO_PLL        DEF_USRRC(APP_DRIVER, CLKRST_DRV, 3)
#define RC_CLKRST_DEV_ERROR     DEF_USRRC(APP_DRIVER, CLKRST_DRV, 4)

typedef enum
{
    CR_PLL_0      = 1,
    CR_PLL_1      = 2,
    CR_PLL_2      = 3,
    CR_PLL_3      = 4,
    CR_PLL_4      = 5,
    CR_PLL_5      = 6,

    CR_DEV_FEC_DL = 7,
    CR_DEV_FEC_UL = 8,
    CR_DEV_SPACC  = 9,

    CR_DEV_CEVA_0 = 10,
    CR_DEV_CEVA_1 = 11,
    CR_DEV_CEVA_2 = 12,
    CR_DEV_CEVA_3 = 13,
    CR_DEV_CEVA_4 = 14,
    CR_DEV_CEVA_5 = 15,
    CR_DEV_CEVA_6 = 16,
    CR_DEV_CEVA_7 = 17,
    CR_DEV_CEVA_8 = 18,
    CR_DEV_CEVA_9 = 19,

    CR_DEV_FP_0   = 20,
    CR_DEV_FP_1   = 21,
    CR_DEV_FP_2   = 22,
    CR_DEV_FP_3   = 23,
    CR_DEV_FP_4   = 24,
    CR_DEV_FP_5   = 25,
    CR_DEV_FP_6   = 26,
    CR_DEV_FP_7   = 27,
    CR_DEV_FP_8   = 28,
    CR_DEV_FP_9   = 29,

    SPU_AXI       = 30,
    RAD_AXI       = 31,
    SYS_AXI       = 32,
    XP_AXI        = 33,

    ARMx4_CORE0   = 34,
    ARMx4_CORE1   = 35,
    ARMx4_CORE2   = 36,
    ARMx4_CORE3   = 37,

    ARMx2_CORE0   = 38,
    ARMx2_CORE1   = 39,
    CR_PCI        = 40,
    CR_DDR0       = 41,
}ClkRstDev;


/** @brif This function checks out of reset bit and clock enable bit

    @param nDevID [in] - the device ID

    @return [BOOL]  TRUE - if device is enabled, 0 - if device is disabled*/

BOOL ClkRstIsDevEnabled (UINT32 nDevID);


/** @brief This function returns a frequency of control object,
           if the frequency is equal to 0, this device is in reset mode

    @param nDevID [in] - the device ID, see: ClkRstDev enum

    @return [UINT32] the frequency */

UINT32 ClkRstGetFreq (UINT32 nDevID);


/** @brief This function returns TRUE is a device is in reset mode

    @param nDevID [in] - the device ID , see ClkRstDev enum type

    @return [BOOL] */

BOOL ClkRstIsInReset (UINT32 nDevID);

/** @brief This function enables device in clock and reset module

    @param nDevID [in] - the device ID , see ClkRstDev enum type

    @return [MXRC] */

MXRC ClkRstEnableDev (UINT32 nDevID);

/** @brief This function returns device frequency in ASCIIZ

    @param [nDevID] - the device ID, see ClkRstDev enum type

    @return [char *] */

char * ClkRstGetAsciizFreq (UINT32 nDevID);

/** @brief This function returns the ID of source PLL used to clock specified device

    @param nDevID [in] - the device ID, see: ClkRstDev enum

    @return [UINT32] the frequency */
UINT32 ClkGetSrcPll(UINT32 nDevID);


/** @brief This function sets PLL frequency 
    @param nPllID [in] - the PLL id [CR_PLL_0..CR_PLL_5]
    @param nFreq  [in] - the PLL frequency specified in MHz
    @return [MXRC] an error code */

MXRC ClkRstSetPll (UINT32 nPllID, UINT32 nFreq);

MXRC ClkRstSetDevFreq (UINT32 nDevID, UINT32 nFreq, UINT32 nPllID);
void SysClkMmuCfg(UINT32 nCpuID, UINT32 nParam);

#endif //_CLK_RST_H_

#ifdef __cplusplus
}
#endif

