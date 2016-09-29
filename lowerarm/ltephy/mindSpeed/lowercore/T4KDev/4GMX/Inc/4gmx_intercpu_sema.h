//-------------------------------------------------------------------------------------------
/** @file 4gmx_intercpu_sema.h
 *
 * @brief The file contains declaration of the semaphore's inter-cpu
 *        commands parameters
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "4gmx_config.h"

#ifdef MX_INTERCPU_ENABLED

#ifndef _4GMX_INTERCPU_SEMA_H_
#define _4GMX_INTERCPU_SEMA_H_

#include "4gmx_types.h"
#include "4gmx_const.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef MX_MTHREAD_ENABLED

#include "4gmx_sema.h"

/** @brief The type of create operation */

typedef struct
{
    UINT32      InitState;
    LPCSTR      Name;
    HANDLE *    Handle;

}MxICSemaCreate;

/** @brief The type of remove operation */

typedef struct
{
    HANDLE      Handle;

}MxICSemaRemove;

/** @brief The type of set/reset operations */

typedef struct
{
    HANDLE      Handle;
    UINT32      Value;

}MxICSemaSet;

#endif /*MX_SEMA_ENABLED*/

#ifdef __cplusplus
}
#endif

#endif /*_4GMX_INTERCPU_SEMA_H_*/

#endif /*MX_INTERCPU_ENABLED */
