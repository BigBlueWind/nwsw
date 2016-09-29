/** @file uexp.h
 *
 * @brief Universal Expansion Bus definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef UEXP_H_
#define UEXP_H_

#include "resultcodes.h"
#include "portio.h"

// UEXP_DIRECT_ENABLE
#define UEXP_EXP_BUS_ACCESS_STATUS  (1<<16)
#define UEXP_EXP_BUS_REQUEST        (1<<15)
#define UEXP_DA_WDATA_OEN           (1<<13)
#define UEXP_DA_LOWER_ADDR_ENABLE   (1<<11)
#define UEXP_DA_WE_N_ENABLE         (1<<6)
#define UEXP_DA_CS0_N_ENABLE        (1<<0)

// UEXP_DIRECT_CONTROL

#define UEXP_DA_UEXP_ALE            (1<<15)
#define UEXP_DA_UEXP_CLE            (1<<14)
#define UEXP_DA_BE3_N               (1<<13)
#define UEXP_DA_BE2_N               (1<<12)
#define UEXP_DA_BE1_N               (1<<11)
#define UEXP_DA_BE0_N               (1<<10)
#define UEXP_DA_XRE_N               (1<<9)
#define UEXP_DA_XWE_N               (1<<8)
#define UEXP_DA_RE_N                (1<<7)
#define UEXP_DA_WE_N                (1<<6)
#define UEXP_DA_CS5_N               (1<<5)
#define UEXP_DA_CS4_N               (1<<4)
#define UEXP_DA_CS3_N               (1<<3)
#define UEXP_DA_CS2_N               (1<<2)
#define UEXP_DA_CS1_N               (1<<1)
#define UEXP_DA_CS0_N               (1<<0)

#define UEXP_CS_NOR                 1
#define UEXP_CS_BOOT                2
#define UEXP_CS_TSI                 3
#define UEXP_CS_PLD                 4
#define UEXP_CS_NAND                5

// Chip Select Enable Register
#define EXP_CSSD0                   0
#define EXP_CSSD1                   1
#define EXP_CSBOOT                  2
#define UEXP_CS3                    3
#define UEXP_CS4                    5
#define UEXP_CS5                    6

// TODO: Rewrite

#if 0

__inline void HalUexpChipSelectCfg (int nCS, U32 nCfgA, U32 nCfgB, U32 nCfgC)
{
    IOWrite32A(UEXP_CS_CFGA(nCS), nCfgA);
    IOWrite32A(UEXP_CS_CFGB(nCS), nCfgB);
    IOWrite32A(UEXP_CS_CFGC(nCS), nCfgC);
}

#endif

RESULTCODE UexpInit(void);

#endif  // UEXP_H_
