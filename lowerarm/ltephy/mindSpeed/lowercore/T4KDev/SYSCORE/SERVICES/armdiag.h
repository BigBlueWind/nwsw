//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL armdiag.h
 *
 * @brief The ArmDiagnostics (ARM software implementation
*       of the FPGA H/W components)
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


#ifndef _ARM_DIAG_H_
#define _ARM_DIAG_H_

#include "4gmx.h"
#include "appids.h"
#include "larmmsgid.h"
#include "diagnostics.h"
#include "supervisor.h"

/******************************************************************************
*                The module error codes                                       *
*******************************************************************************/
#define RC_ARMDIAG_OK               MX_OK
#define RC_ARMDIAG_PARAM_ERROR      DEF_USRRC(APP_MODULE, ARMDIAG, 1)
#define RC_ARMDIAG_CTX_ALLOC_ERROR  DEF_USRRC(APP_MODULE, ARMDIAG, 2)
#define RC_ARMDIAG_NOT_PARAM        DEF_USRRC(APP_MODULE, ARMDIAG, 3)
#define RC_ARMDIAG_INPUT_ERROR      DEF_USRRC(APP_MODULE, ARMDIAG, 4)
#define RC_ARMDIAG_OUTPUT_ERROR     DEF_USRRC(APP_MODULE, ARMDIAG, 5)
#define RC_ARMDIAG_UNKNOWN_API      DEF_USRRC(APP_MODULE, ARMDIAG, 6)
#define RC_ARMDIAG_UNSUP_TYPE       DEF_USRRC(APP_MODULE, ARMDIAG, 7)
#define RC_ARMDIAG_TASKID_ERROR     DEF_USRRC(APP_MODULE, ARMDIAG, 8)
#define RC_ARMDIAG_INDESCR_ERROR    DEF_USRRC(APP_MODULE, ARMDIAG, 9)
#define RC_ARMDIAG_OUTDESCR_ERROR   DEF_USRRC(APP_MODULE, ARMDIAG,10)
#define RC_ARMDIAG_INOUTDESCR_ERROR DEF_USRRC(APP_MODULE, ARMDIAG,11)

#define BEGIN_ARM_TASKS(name)   ARMTASK (name) [] = {
#define END_ARM_TASKS()         {0,     NULL}};
#define ARM_TASK(id, cb)        {(id), (cb)},
#define DEFINE_ARM_TABLE(name)  extern ARMTASK (name) [];

typedef struct
{
    UINT32      nTaskID;
    cbTCBDone   Proc;

}ARMTASK;


typedef struct _ARMDIAG_CTX_
{
    HANDLE  hHeap;
    TCB     Tcb;

}ArmDiagCtx, *PArmDiagCtx;


DEFINE_ARM_TABLE(armtasks)

/******************************************************************************
*                The API declaration                                          *
*******************************************************************************/

MXRC  ArmDiagInit (UINT32* pReportData, UINT32* pReportSize);

APIHEC ArmDiagApiHandler (LPVOID pData, ApiHeader * pApi, ApiHeader * pResp);


#endif /*_ARM_DIAG_H_*/

#ifdef __cplusplus
}
#endif

