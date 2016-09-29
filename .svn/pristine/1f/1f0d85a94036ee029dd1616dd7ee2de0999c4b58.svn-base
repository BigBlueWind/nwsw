//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL dspdiag.h
 *
 * @brief The DspDiag API module
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
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

#ifndef _DSP_DIAG_H_
#define _DSP_DIAG_H_

#include "4gmx.h"
#include "supervisor.h"
#include "appids.h"
#include "larmmsgid.h"
#include "diagnostics.h"
#include "DSPdrv.h"
#include "hal.h"

/******************************************************************************
*                The module error codes                                       *
*******************************************************************************/
#define RC_DSPDIAG_OK               MX_OK
#define RC_DSPDIAG_CTX_ALLOC_ERROR  DEF_USRRC(APP_MODULE, DSPDIAG, 1)
#define RC_DSPDIAG_ALLOC_BUF_ERROR  DEF_USRRC(APP_MODULE, DSPDIAG, 2)
#define RC_DSPDIAG_UNKNOWN_API      DEF_USRRC(APP_MODULE, DSPDIAG, 3)
#define RC_DSPDIAG_INPUT_ERROR      DEF_USRRC(APP_MODULE, DSPDIAG, 4)
#define RC_DSPDIAG_OUTPUT_ERROR     DEF_USRRC(APP_MODULE, DSPDIAG, 5)
#define RC_DSPDIAG_RUN_TIMEOUT      DEF_USRRC(APP_MODULE, DSPDIAG, 6)
#define RC_DSPDIAG_UNSUP_TYPE       DEF_USRRC(APP_MODULE, DSPDIAG, 7)
#define RC_DSPDIAG_NOT_PARAM        DEF_USRRC(APP_MODULE, DSPDIAG, 8)
#define RC_DSPDIAG_PARAM_ERROR      DEF_USRRC(APP_MODULE, DSPDIAG, 9)
#define RC_DSPDIAG_INDESCR_ERROR    DEF_USRRC(APP_MODULE, DSPDIAG,10)
#define RC_DSPDIAG_OUTDESCR_ERROR   DEF_USRRC(APP_MODULE, DSPDIAG,11)
#define RC_DSPDIAG_INOUTDESCR_ERROR DEF_USRRC(APP_MODULE, DSPDIAG,12)
#define RC_DSPDIAG_BUF_EMPTY        DEF_USRRC(APP_MODULE, DSPDIAG,14)
#define RC_DSPDIAG_PARAM_OFFS_ERROR DEF_USRRC(APP_MODULE, DSPDIAG,15)
#define RC_DSPDIAG_PARAM_SIZE_ERROR DEF_USRRC(APP_MODULE, DSPDIAG,16)

#define DSPDIAG_NUM_CEVA            10

#define DSPDIAG_PARAM_MASK          2
#define DSPDIAG_PARAM_DEVID         2 // the same to DSPDIAG_PARAM_MASK
#define DSPDIAG_PARAM_BUFINFO       3
#define DSPDIAG_PARAM_BUF_OFFS      3 // the same to DSPDIAG_PARAM_BUFINFO
#define DSPDIAG_PARAM_DUMP          4
#define DSPDIAG_PARAM_BUF_SIZE      4 // the same id

// This macro is used for running DSP in the async mode
// for the sync RUN_DSP API command, the communication with
// the host is equal to the sync RUN_DSP command
// This macro does not influent to the async run DSP API command

//#define EMULATE_SYNC_DSP_RUN

/******************************************************************************
*                The module types definition                                  *
*******************************************************************************/

typedef struct _DSPDIAG_CTX_
{
    volatile PDSP_CTRL_REGS DspCtrl;

    TCB         Tcb;
    HANDLE      hDspHeap;
    UINT32      ParamSizeLimit;
    HANDLE      AThrComplete;   // The thread processes DSP task compliteness
    HANDLE      AThrTimeout;    // This thread monitors DSP timeout
    HANDLE      AsyncTimer;     // The timer for AsyncThrTimeout object
    HANDLE      AsyncEvent;     // The event for AsyncThrTimeout & AsyncThrTimeout
    UINT16      nDInstance;     // The destination instance (for the indication)
    UINT16      nSInstance;
    BOOL        bProcIO;
}DspDiagCtx, *PDspDiagCtx;

/******************************************************************************
*                The API declaration                                          *
*******************************************************************************/

MXRC  DspDiagInit (UINT32* pReportData, UINT32* pReportSize);

APIHEC DspDiagApiHandler (LPVOID pData, ApiHeader * pApi, ApiHeader * pResp);

#endif /*_DSP_DIAG_H_*/

#ifdef __cplusplus
}
#endif

