//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL ddrmondrv.h
 *
 * @brief T4K + EVM DDR monitor driver
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#include "config.h"


#ifndef _DDR_MON_DRV_H_
#define _DDR_MON_DRV_H_

#include "hal.h"


#define DDR_MON_CTRL_ENABLED        (1 << 0)

typedef struct _DDR_MON_CTX_
{
    UINT32  Ctrl;           // See: DDR_MON_CTRL_xxx
    PUINT8  DDRBase [2];

    UINT32  WrProtectErrCount[2];
    UINT32  WrOperAddr[2];
    UINT32  WrOperLen[2];
    
}DDRMonCtx;

typedef struct _DDR_MON_REGION_
{
    UINT32  DddrID;
    UINT32  RegNum;
    UINT32  Addr;
    UINT32  Len;

}DDRMonRegion;

/** @brief This function enables/disables DDR monitor driver 

    @param ctrl [in] - see DDR_MON_CTRL_xxx
    @param pList[in] - the pointer to the list of protected areas, or NULL

    @return [MXRC] an error code */

MXRC DDRMonDrvCtrl(UINT32 ctrl, DDRMonRegion * pList);

MXRC DDRMonDrv_init(UINT32 nInit, LPVOID * pDrvCtx);

INLINE MXRC DDRMonDrv_config(LPVOID pDrvCtx, UINT32 nCfg, LPVOID pCfgData,
                    UINT32 nCfgDataSize){return 0;};

INLINE MXRC DDRMonDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                   UINT32 nCmdDataSize) {return 0;}

INLINE MXRC DDRMonDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                  UINT32 * pnCmdDataSize) {return 0;}

#endif // _DDR_MON_DRV_H_


#ifdef __cplusplus
}
#endif
