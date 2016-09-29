//-------------------------------------------------------------------------------------------
/** @file cline.h
 *
 * @brief This file contains declaration of console command menu,
*       API of initialization and so on
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
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

#ifndef _CLINE_H_
#define _CLINE_H_

#include "4gmx.h"
#include "appids.h"
#include "uartdrv.h"
#include "cmgr.h"

#define RC_CL_ALLOC_ERROR           DEF_USRRC(APP_MODULE, COMMAND_LINE, 1)
#define RC_CL_NOT_INIT              DEF_USRRC(APP_MODULE, COMMAND_LINE, 2)
#define RC_CL_PARAM_ERROR           DEF_USRRC(APP_MODULE, COMMAND_LINE, 3)
#define RC_CL_GARBAGE_DETECTED      DEF_USRRC(APP_MODULE, COMMAND_LINE, 4)

#define CLINE_NEED_UPDATE         1

#if 1

#define CLINE_GET_LOCK(x)  MxGetLock (x)
#define CLINE_RELEASE_LOCK(x) MxReleaseLock(x)

#else

#define CLINE_GET_LOCK(x)
#define CLINE_RELEASE_LOCK(x)

#endif

#define CLINE_DEVICE_UART 	0
#define CLINE_DEVICE_CICPU 	1

typedef struct
{
    UINT32  Ctrl;
    HANDLE  hUart;
    LPVOID  lpUartCtx;
    HANDLE  hTask;
    UINT32  nIrqID;
    VUINT32 Sync;

    CMgrCtx MgrCtx;

    VUINT16 RxPut;
    VUINT16 RxGet;

    UINT8   rxbuf [128];

    VUINT16 TxPut;
    VUINT16 TxGet;

    UINT8   txbuf [10*1024];
    UINT32  device;

}CLCtx;

/** @brief This function sets a pointer to the external application menu table
 *
 *  @param pMenu [in] - the menu table
 *
 *  @return [MXRC] an error code */

MXRC CLSetMenu (CMgrMenuElm * pMenu);

/** @brief The function initializes console command line module
    @eturn [MXRC] error code */

MXRC CLInit (void);

/** @brief The function shows console input
    @eturn [MXRC] error code */
MXRC CLUpdate (void);
BOOL CLIsActive (void);
MXRC CLWriteStr (char * pStr);
MXRC CLWriteChar (char ch);
BOOL IsPrintActive (void);
void CLineNeedUpdateLogo(void);
BOOL HaveToBeUpdated (void);
void WaitPrint (void);

#endif /*_CLINE_H_*/

#ifdef __cplusplus
}
#endif

