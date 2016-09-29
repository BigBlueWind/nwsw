//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL schdiagctrl.h
 *
 * @brief The scheduler control API
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

#ifndef _SCH_DIAG_CTRL_H_
#define _SCH_DIAG_CTRL_H_

#include "supervisor.h"
#include "4gmx.h"

#define RC_SCHDIAG_CTX_ALLOC_ERROR      DEF_USRRC(APP_MODULE, SCH_DIAG, 1)
#define RC_SCHDIAG_UNKNOW_API           DEF_USRRC(APP_MODULE, SCH_DIAG, 2)
#define RC_SCHDIAG_API_PARAM_ERR        DEF_USRRC(APP_MODULE, SCH_DIAG, 3)
#define RC_SCHDIAG_NOT_MEMORY           DEF_USRRC(APP_MODULE, SCH_DIAG, 4)

typedef struct
{
    UINT32 nDummy;

} SchDiagCtx, * PSchDiagCtx;


MXRC  SchDiagInit (UINT32* pReportData, UINT32* pReportSize);

#endif // _SCH_DIAG_CTRL_H_

#ifdef __cplusplus
}
#endif

