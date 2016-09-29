/** @file arbiter.h
 *
 * @brief AHB Arbiter definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef ARBITER_H_
#define ARBITER_H_

#include "basetypes.h"
#include "resultcodes.h"
#include "portio.h"
#include "apb.h"

//
// Masks for TC & Configuration register
//
#define ASA_TC_REQTDMAEN        (1<<10)
#define ASA_TC_REQARM0EN        (1<<11)
#define ASA_TC_REQARM1EN        (1<<12)
#define ASA_TC_REQIDMAEN        (1<<13)
#define ASA_TC_REQSMCEN         (1<<14)

#ifdef M823XX
// TODO: Enable for FPGA-EVM
static inline void AhbPriority(U32 tdm, U32 arm0, U32 arm1, U32 idma, U32 smc)
{
    IOWrite32A(ASA_PRI,
               ((tdm)        & 0x0000000F) |
               ((arm0 <<  4) & 0x000000F0) |
               ((arm1 <<  8) & 0x00000F00) |
               ((idma << 12) & 0x0000F000) |
               ((smc  << 16) & 0x000F0000)
               );
}
#endif /* M823XX */

RESULTCODE AhbInit(void);

#endif /*ARBITER_H_*/
