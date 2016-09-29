//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL ltemacmsfunctions.h
 *
 * @brief This file defines the prototypes for functions that are used
 *        in the implementation of an LTE MAC
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT&copy; 2009-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/

//-------------------------------------------------------------------------------------------



#ifndef LTEMACMSFUNC_H
#define LTEMACMSFUNC_H

//#include "ltypes.h"
#include "LtePhyL2Api.h"
#include "ltephy.h"
#include "ltephydef.h"
#include "ltemac.h"
#include "LTE_test_config.h"

extern void LteMacInit(PLTEMACDESC pMac, PINITPARM pInit, U16 nextAvailableRB);
extern void LteSubFrameMacInit(PLTEMACDESC pMac, PINITPARM pInit, U16 nextAvailableRB);
extern void MacMsReAllocCtl(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id,
                            U8 *pData, U32 dataSize);
extern void MacMsReallocData(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id,
                             U8 *pData, U32 dataSize, U32 Direction,
                             double CodingRate,  U16 srsSubframeEnable);
extern void MacMsSendCtl(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id, U8 *pData,
                         U32 dataSize, PCHCFG pTestCfg);
extern void MacMsSendData(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id, U8 *pData,
                          U32 dataSize, U32 cwId, U32 maxBitsperCw, PCHCFG pTestCfg);
extern void MacMsSendDataLowArm(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id,
                                U8 *pData, U32 dataSize, U32 cwId,
                                U32 maxBitsperCw, PCHCFG pTestCfg);
extern void MacMsRx(PDLSUBFRDESC pSfr, PLTEMACDESC pMac);
extern void MacMsReallocBCH(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id,
                            U8 *pData, U32 dataSize);
extern void MacMsSendCtlLowArm(PDLSUBFRDESC pSfr, PLTEMACDESC pMac, U32 id,
                               U8 *pData, U32 dataSize, PCHCFG pTestCfg);

#endif /* LTEMACFUNCTIONS_H */
