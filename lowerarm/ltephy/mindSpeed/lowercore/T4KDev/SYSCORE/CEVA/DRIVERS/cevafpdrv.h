//-------------------------------------------------------------------------------------------
/** @file cevafpdrv.h
 *
 * @brief The file contains CEVA FP driver used by the REX-CS to run FP TCBs
 * @author Mindspeed Technologies
 * @version  $Revision: 1.1 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
*/
//-------------------------------------------------------------------------------------------

#ifndef _CEVA_FP_H_
#define _CEVA_FP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef CEVA_PROJ

#include "4gmx.h"
#include "hal.h"
#include "config.h"
#include "appprintf.h"

#else
#include "global.h"
#endif

#include "prototypes.h"
#include "cevaservapi.h"
#include "mdmafectypes.h"
#include "tcb.h"
#include "fptypes.h"
#include "mlogceva.h"

typedef struct _CEVA_FP_DRV_CTX_
{
    UINT32                      Inited;
    UINT16                      IdftStart;
    UINT16                      IdftStop;
    VUINT32*                    IdftRunPtr;
    UINT32                      FpDevNum;
    volatile PMAP_DMA_EXP_REGS  pDmaExpRegs[2];

}CEVAFPDRVCTX;

void    FpDrvInit(void) CEVAEXTERN;
UINT32  FpDrvOpen  (void) CEVAEXTERN;
UINT32  FpDrvClose (void) CEVAEXTERN;
UINT32  FpDrvRunTcb (TCB * pTcb, UINT32 start_time) CEVAEXTERN;
UINT32  FpDrvIsIdftDone (void) CEVAEXTERN;

#ifdef __cplusplus
}
#endif


#endif /*_CEVA_FP_H_*/
