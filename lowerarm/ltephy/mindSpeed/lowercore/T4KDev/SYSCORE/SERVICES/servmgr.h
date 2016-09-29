//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL servmgr.h
 *
 * @brief Services manager, it's designed to easily integrate new services
*       to the system, it's used like an entry point
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

#ifndef _SERVMGR_H_
#define _SERVMGR_H_

#include "config.h"
#include "4gmx.h"
#include "appids.h"

/** @brief This function initialize list of system services
           specified in the table inside

    @param nCtrl [in] - reserved, it should be zero

    @return [MXRC] an error code */

MXRC ServInit (UINT32 nCtrl);

#endif //_SERVMGR_H_

#ifdef __cplusplus
}
#endif

