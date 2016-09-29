/** @file clkcore.h
 *
 * @brief CLK module definitions and interfaces
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef CLKCORE_H_
#define CLKCORE_H_

#include "basetypes.h"

#ifdef M823XX
#define CLK_REFCLK              10000000        // Base clock is 10.0 MHz
#define CLK_AMBACLK             133000000       // 133 MHz
#define CLK_ARMCLK              437500000       // 437.5 MHz
#define CLK_SPUCLK              350000000       // 350 MHz
#else /*M823XX*/
#define CLK_AMBACLK             200000000       // 200 MHz
#endif /*M823XX*/

void ClkSetAmbaClock(U32 hclk);
U32 ClkGetAmbaClock(void);

void ClkSetArmClock(U32 fclk);
U32 ClkGetArmClock(void);

void ClkSetSpuClock(U32 fclk);
U32 ClkGetSpuClock(void);

#endif /*AMBA_H_*/
