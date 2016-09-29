//-------------------------------------------------------------------------------------------
/** @file ltemacfunctions.h
 *
 * @brief This file defines the prototypes for functions that are used
 *        in the implementation of an LTE MAC
 * @author Mindspeed Technologies
 * @version $Revision: 1.13 $
 *
 * COPYRIGHT&copy; 2009-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/

//-------------------------------------------------------------------------------------------



#ifndef LTEMACFUNC_H
#define LTEMACFUNC_H

//#include "ltypes.h"
#include "LtePhyL2Api.h"
#include "ltephy.h"
#include "ltephydef.h"
#include "ltemac.h"
#include "LTE_test_config.h"

extern void LteMacInit(PLTEMACDESC pMac, PINITPARM pInit, U16 nextAvailableRB);
extern void LteSubFrameMacInit(PLTEMACDESC pMac, PINITPARM pInit, U16 nextAvailableRB);
extern void MacBsReAllocCtl(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id,
                            U8 *pData, U32 dataSize);
extern void MacBsReallocData(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id,
                             U8 *pData, U32 dataSize, U32 Direction,
                             double CodingRate,  U16 srsSubframeEnable);
extern void MacBsSendCtl(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id, U8 *pData,
                         U32 dataSize, PCHCFG pTestCfg);
extern void MacBsSendData(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id, U8 *pData,
                          U32 dataSize, U32 cwId, U32 maxBitsperCw, PCHCFG pTestCfg);
extern void MacBsSendDataLowArm(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id,
                                U8 *pData, U32 dataSize, U32 cwId,
                                U32 maxBitsperCw, PCHCFG pTestCfg);
extern void MacBsTx(PDLSUBFRDESC pSfr, PLTEMACDESC pMac);
extern void MacBsReallocBCH(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id,
                            U8 *pData, U32 dataSize);
extern void MacBsSendCtlLowArm(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id,
                               U8 *pData, U32 dataSize, PCHCFG pTestCfg);

#endif /* LTEMACFUNCTIONS_H */
