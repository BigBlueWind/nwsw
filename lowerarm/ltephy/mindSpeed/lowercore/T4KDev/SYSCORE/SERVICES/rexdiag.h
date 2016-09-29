//-------------------------------------------------------------------------------------------
/** @file rexdiag.h
 *
 * @brief The declaration of the syscore RapidEXecutive (REX)
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef _REX_DIAG_H_
#define _REX_DIAG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "config.h"
#include "rex.h"

#define REX_DIAG_RC_NOT_SUPPORTED       DEF_USRRC(APP_MODULE, REX_DIAG, 1);
#define REX_DIAG_RC_LIST_CREATE_ERROR   DEF_USRRC(APP_MODULE, REX_DIAG, 2);
#define REX_DIAG_RC_TCB_CREATE_ERROR    DEF_USRRC(APP_MODULE, REX_DIAG, 3);

/** @brief This function is designed to check TCB priorities supported by REX

    @return [MXRC] an error code */

MXRC RexDiagCheckPriors (void);

MXRC RexDiagCheckSimpleList (void);

MXRC RexDiagCheckSuperTCB (void);

#ifdef __cplusplus
}
#endif


#endif // _REX_DIAG_H_

