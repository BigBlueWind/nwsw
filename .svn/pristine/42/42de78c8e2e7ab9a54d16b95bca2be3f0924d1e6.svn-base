//-------------------------------------------------------------------------------------------
/** @file cevafecdrv.h
 *
 * @brief FEC-DL/UL driver developed for CEVA code 
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
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

#ifndef _CEVA_FEC_DRV_H_
#define _CEVA_FEC_DRV_H_

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

#define FEC_CEVA_RC_OK                      0
#define FEC_CEVA_RC_CFG_ERROR               1
#define FEC_CEVA_RC_DEV_ERROR               2
#define FEC_CEVA_RC_PARAM_ERROR             3
#define FEC_CEVA_RC_NOT_OPENED              4
#define FEC_CEVA_RC_TIMEOUT                 5

//#define FEC_CEVA_ALLOC_SYNC                 

typedef struct _CEVA_FECDL_DRV_CTX_
{
    UINT32          OwnerCpu;       // If owner, the ARM is not involved in usage of FEC-DL
    UINT32          HwSema;
    PFECDL_LIST_INF ListInfPtr;
    PMXPOOL         PoolPtr;
    FEC_CTRL_REGS * FecCtrlRegsPtr;

}CEVA_FECDL_DRV_CTX;

typedef struct _CEVA_FECUL_DRV_CTX_
{
    UINT32          OwnerCpu;       // If owner, the ARM is not involved in usage of MDMA
    UINT32          HwSema;
    PFECUL_LIST_INF  ListInfPtr;
    PMXPOOL         PoolPtr;
    FEC_CTRL_REGS * FecCtrlRegsPtr;

}CEVA_FECUL_DRV_CTX;

UINT32          FecDlDrvOpen(void) CEVAEXTERN;
UINT32          FecDlDrvClose(void) CEVAEXTERN;
UINT32          FecDlDrvSetHwCtx(PFEC_DL_FDESC pFDesc, DLHWCTX HwContext)  CEVAEXTERN;
UINT32          FecDlDrvSetLastDesc(PFEC_DL_FDESC pFDesc, FecCbDoneType cbDone, LPVOID CbContext) CEVAEXTERN;
UINT32          FecDlDrvSetLastDescArm(PFEC_DL_FDESC pFDesc, FecCbDoneType cbDone, LPVOID CbContext) CEVAEXTERN;
UINT32          FecDlDrvSetData(PFEC_DL_FDESC pFDesc, LPVOID pInData, UINT32 InDataSize, LPVOID pOutData, UINT32 OutDataSize) CEVAEXTERN;
UINT32          FecDlDrvRun(PFEC_DL_FDESC pFDesc) CEVAEXTERN;
UINT32          FecDlDrvRunQ(PFEC_DL_FDESC pFDesc, UINT32 nNumUs) CEVAEXTERN;
UINT32          FecDlDrvWaitCompletion(PFEC_DL_FDESC pFDesc, UINT32 nTime) CEVAEXTERN;
PFEC_DL_FDESC   FecDlDrvAllocDesc (UINT32 nNum) CEVAEXTERN;

UINT32          FecUlDrvOpen(void) CEVAEXTERN;
UINT32          FecUlDrvClose(void) CEVAEXTERN;
UINT32          FecUlDrvSetHwCtx(PFEC_UL_FDESC pFDesc, ULHWCTX HwContext) CEVAEXTERN;
UINT32          FecUlDrvSetLastDesc(PFEC_UL_FDESC pFDesc, FecCbDoneType CbDone, LPVOID CbContext) CEVAEXTERN;
UINT32          FecUlDrvSetLastDescArm(PFEC_UL_FDESC pFDesc, FecCbDoneType CbDone, LPVOID CbContext) CEVAEXTERN;
UINT32          FecUlDrvSetData(PFEC_UL_FDESC pFDesc, LPVOID pInData[], UINT32 InDataSize, LPVOID pOutData, UINT32 OutDataSize) CEVAEXTERN;
UINT32          FecUlDrvRun(PFEC_UL_FDESC pFDesc) CEVAEXTERN;
UINT32          FecUlDrvRunQ(PFEC_UL_FDESC pFDesc, UINT32 nNumUs) CEVAEXTERN;
UINT32          FecUlDrvWaitCompletion(PFEC_UL_FDESC pFDesc, UINT32 nTime) CEVAEXTERN;
PFEC_UL_FDESC   FecUlDrvAllocDesc(UINT32 NumDesc) CEVAEXTERN;
    
#endif //_CEVA_MDMA_DRV_H_

#ifdef __cplusplus
}
#endif

