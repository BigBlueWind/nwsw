//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Rdmemdrv.h
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
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

#ifndef _RDMEM_DRV_H_
#define _RDMEM_DRV_H_

#ifdef RDMEM_CACHE_CONTROL_ENABLED

MXRC RdMemDrv_init(UINT32 nInit, LPVOID * pCtx);


INLINE MXRC RdMemDrv_config(LPVOID pDrvCtx, UINT32 nCfg, LPVOID pCfgData,
                     UINT32 nCfgDataSize) {return MX_OK;}

INLINE MXRC RdMemDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                    UINT32 nCmdDataSize) {return MX_OK;}

INLINE MXRC RdMemDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                   UINT32 * pnCmdDataSize) {return MX_OK;}


#endif /* RDMEM_CACHE_CONTROL_ENABLED */
#endif /*_RDMEM_DRV_H_*/

#ifdef __cplusplus
}
#endif

