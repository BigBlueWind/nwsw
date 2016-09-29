/**
 ****************************************************************************
 *
 * @file dejitter.h 
 *
 * @brief Include file for Mindspeed Transcede EVM Dejitter and clock
 * routing PLL driver software.  
 *
 * @copyright
 * This software is Copyright (c) 2010-2011 Mindspeed Technologies Inc., 
 * All rights reserved.
 * <pre>
 *       Mindspeed Technologies, Inc.
 *       4000 MACARTHUR BLVD - EAST TOWER
 *       NEWPORT BEACH, CA 92660
 * </pre>
 * This software is included as part of u-boot and therefore this particular
 * source file is licensed under the terms of the GNU Public License version
 * 2.0. 
 *
 * @par NOTE
 * Different EVMs use different PLLs and have different capabilities.
 * This file is a consolodation to support Mindspeed x670, x730 and x750
 * Evaluation and demostration boards.
 * 
 ******************************************************************************/
#ifndef __DEJITTER_H__
#define __DEJITTER_H__

#include "spidrv.h"

#include "4gmx.h"
#include "config.h"
#include "appids.h"

#define RC_DEITTER_PLL_INIT_ERROR   DEF_USRRC(APP_DRIVER, DEJITTER_DRV, 1)
#define RC_DEITTER_RAM_REG_ERROR    DEF_USRRC(APP_DRIVER, DEJITTER_DRV, 2)
#define RC_DEITTER_SPI_CTX_ERROR    DEF_USRRC(APP_DRIVER, DEJITTER_DRV, 3)

#define IS_FEMTOCELL_BOARD()   (1)

#define USE_GPIO_CS

#define DEJITTER_CS_GPIO            23
#define DEJITTER_AD9548_CS_GPIO     23
#define DEJITTER_AD9553_CS_GPIO     24
#define BRCM_SWITCH_CS_GPIO         22
#define RADIO_AD9361_CS_GPIO        21
#define DEJITTER_DIV_SYNC_GPIO      10	//Active low
#define DEJITTER_PLL_LOCK           11	//device output the status,so this gpio is an input for turner

// Dejitter init options
#define DEJITTER_INIT_STARTUP       0   // Set IDLE and also burn to EEPROM if necessary
#define DEJITTER_INIT_IDLE          1
#define DEJITTER_INIT_CPRI          2
#define DEJITTER_INIT_RADIO         3

// Clock sources (currently only supported on AD9548)
#define AD9548_CLK_SRC_REFA         0
#define AD9548_CLK_SRC_REFAA        1
#define AD9548_CLK_SRC_REFB         2
#define AD9548_CLK_SRC_REFBB        3 
#define AD9548_CLK_SRC_REFC         4
#define AD9548_CLK_SRC_REFCC        5
#define AD9548_CLK_SRC_REFD         6
#define AD9548_CLK_SRC_REFDD        7 
#define AD9548_CLK_SRC_MAX_VALUE    AD9548_CLK_SRC_REFDD

// Femtocell mapping from clock sources to PLL inputs
#define DEJITTER_CLK_SRC_TRANSCEDE      AD9548_CLK_SRC_REFA  // Clock from Transcede Network timing Generator
#define DEJITTER_CLK_SRC_GPS_1PPS       AD9548_CLK_SRC_REFB  // GPS 1 PULSE PER SECOND (1 Hz)
#define DEJITTER_CLK_SRC_GPS_OSCILLATOR AD9548_CLK_SRC_REFBB // GPS MODULE DISCPLINED TIMING
#define DEJITTER_CLK_SRC_SMA_CONNECTOR  AD9548_CLK_SRC_REFD  // Clock from external source via coax cable to SMA
#define DEJITTER_CLK_SRC_RADIO          AD9548_CLK_SRC_REFDD // Clock out from Radio chipset

#define DEJITTER_CLK_SRC_FREE_RUN       (AD9548_CLK_SRC_MAX_VALUE+1)
#define DEJITTER_CLK_SRC_HOLDOVER       (AD9548_CLK_SRC_MAX_VALUE+2)
#define DEJITTER_CLK_SRC_AUTOMATIC      (AD9548_CLK_SRC_MAX_VALUE+3)

#define DEJITTER_CLK_SRC_MAX_PARAMETER  DEJITTER_CLK_SRC_AUTOMATIC

extern void SysDelay(UINT32 ms);

// Function prototypes
MXRC DejitterInit  (UINT32 InitOption, UINT32 ClockSource);
void DejitterSelectSource(UINT32 ClockSourceId);  // Setup Dejitter refernce clock input (if supported by circuitry)

UINT32 DejitterRead  (UINT32 addr, UINT32 gpio);               // Read  RAM value
void   DejitterWrite (UINT32 addr, UINT32 data, UINT32 gpio);  // Write RAM value
void   DejitterUnlock(void);                      // Copy  RAM to EEPROM

void CypressPLLInit (UINT32 InitOption);

#endif
