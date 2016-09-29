//-------------------------------------------------------------------------------------------
/** @file cevamdmadrv.h
 *
 * @brief MDMA driver developed for CEVA code
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _CEVA_MDMA_DRV_H_
#define _CEVA_MDMA_DRV_H_

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

#define MDMA_CEVA_RC_OK                     0
#define MDMA_CEVA_RC_CFG_ERROR              1
#define MDMA_CEVA_RC_DEV_ERROR              2
#define MDMA_CEVA_RC_PARAM_ERROR            3

#define MDMA_CEVA_ALLOC_SYNC

typedef struct _CEVA_MDMA_DRV_CTX_
{
    UINT32          OwnerCpu;       // If owner, the ARM is not involved in usage of MDMA
    UINT32          HwSema;
    PMDMA_LIST_INF  ListInfPtr;
    PMXPOOL         PoolPtr;
    MDMA_CTRL_REGS *MdmaCtrlRegs;

}CEVA_MDMA_DRV_CTX;

UINT32      MdmaDrvOpen(UINT32 MdmaID) CEVAEXTERN;
UINT32      MdmaDrvClose(UINT32 MdmaID) CEVAEXTERN;
UINT32      MdmaDrvSetLastDesc(PMDMA_FDESC pFDesc, void (*CbDone)(LPVOID), LPVOID CbContext) CEVAEXTERN;
UINT32      MdmaDrvSetLastDescArm(PMDMA_FDESC pFDesc, void (*CbDone)(LPVOID), LPVOID CbContext) CEVAEXTERN;
UINT32      MdmaDrvRun(UINT32 MdmaID, PMDMA_FDESC pFDesc) CEVAEXTERN;
UINT32      MdmaDrvRunQ(UINT32 MdmaID, PMDMA_FDESC pFDesc, UINT32 nNumUs) CEVAEXTERN;
UINT32      MdmaDrvWaitCompletion(UINT32 MdmaID, PMDMA_FDESC pFDesc) CEVAEXTERN;
PMDMA_FDESC MdmaDrvAllocDesc (UINT32 MdmaID, UINT32 nNum) CEVAEXTERN;
void        MdmaDrvSetData(PMDMA_FDESC pFDesc, LPVOID pInData, UINT32 InDataSize, LPVOID pOutData, UINT32 OutDataSize) CEVAEXTERN;
void        MdmaDrvSetCopy (PMDMA_FDESC pMdma) CEVAEXTERN;
void        MdmaDrvSetCRC (PMDMA_FDESC pMdma, UINT32 CrcInit, UINT32 PolyValue, UINT32 PolyOrder) CEVAEXTERN;
void        MdmaDrvSet4GSCR (PMDMA_FDESC pMdma, UINT32 cInit0, UINT32 cInit1, UINT32 PackMode, UINT32 nScrOpt) CEVAEXTERN;
void        MdmaDrvSet4GDSCR (PMDMA_FDESC pMdma, UINT32 cInit0, UINT32 cInit1, UINT32 nScrOpt) CEVAEXTERN;
void        MdmaDrvSetMultiple4GDSCR (PMDMA_FDESC pMdma, UINT32 cInit0, UINT32 cInit1, UINT32 nScrOpt, UINT32 numDesc) CEVAEXTERN;

#endif //_CEVA_MDMA_DRV_H_

#ifdef __cplusplus
}
#endif

