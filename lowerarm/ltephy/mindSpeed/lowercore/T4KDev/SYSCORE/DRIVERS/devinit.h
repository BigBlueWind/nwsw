//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL devinit.h
 *
 * @brief T4K + EVM additional devices initialization
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
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

#ifndef _DEV_INIT_H_
#define _DEV_INIT_H_

#include "4gmx.h"
#include "hal.h"
#include "lcorecfg.h"
#include "appids.h"
#include "heaps.h"
/******************************************************************************
*                The module error codes                                       *
*******************************************************************************/
#define RC_DEVINIT_OK		MX_OK
#define RC_DEVINIT_ERROR	DEF_USRRC(APP_DRIVER, DEV_INIT, 1)

MXRC DevInit (void);

#endif //_DEV_INIT_H_

#ifdef __cplusplus
}
#endif

