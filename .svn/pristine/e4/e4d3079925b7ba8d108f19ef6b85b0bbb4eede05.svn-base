/** @file memlimits.h
 *
 * @brief RAM limitations
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef MEMLIMITS_H_
#define MEMLIMITS_H_

#include "basetypes.h"

#ifdef M823XX

#define SDRAM_BASE              0x00000000
#define SDRAM_SIZE              (128*1024*1024)
#define ERAM_BASE               0x08000000
#define ERAM_SIZE               (640*1024)
#define IRAM_BASE               0x0A000000
#define IRAM_SIZE               (8*1024)
#define ARAM_BASE               0x0A002000
#define ARAM_SIZE               (64*1024)

#endif  /*M823XX*/

U32 MemDetectSdramSize(void);

#endif  /*MEMLIMITS_H_*/
