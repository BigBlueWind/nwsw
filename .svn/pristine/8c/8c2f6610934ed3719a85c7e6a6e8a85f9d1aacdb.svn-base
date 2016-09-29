//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL fecdiag.h
 *
 * @brief The Fec diagnostics module
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
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

#ifndef _FEC_DIAG_H_
#define _FEC_DIAG_H_

#include "4gmx.h"
#include "supervisor.h"
#include "appids.h"

/******************************************************************************
*                The module error codes                                       *
*******************************************************************************/
#define RC_FECDIAG_OK                   DEF_USRRC(APP_MODULE, FECDIAG, 0)
#define RC_FECDIAG_INIT_ERROR           DEF_USRRC(APP_MODULE, FECDIAG, 1)
#define RC_FECDIAG_ALLOC_ERROR 		    DEF_USRRC(APP_MODULE, FECDIAG, 2)
#define RC_FECDIAG_UNKNOWN_API          DEF_USRRC(APP_MODULE, FECDIAG, 3)
#define RC_FECDIAG_INPUT_ERROR          DEF_USRRC(APP_MODULE, FECDIAG, 4)
#define RC_FECDIAG_OUTPUT_ERROR         DEF_USRRC(APP_MODULE, FECDIAG, 5)
#define RC_FECDIAG_RUN_TIMEOUT          DEF_USRRC(APP_MODULE, FECDIAG, 6)
#define RC_FECDIAG_UNSUP_TYPE           DEF_USRRC(APP_MODULE, FECDIAG, 7)
#define RC_FECDIAG_NOT_PARAM            DEF_USRRC(APP_MODULE, FECDIAG, 8)
#define RC_FECDIAG_PARAM_ERROR          DEF_USRRC(APP_MODULE, FECDIAG, 10)
#define RC_FECDIAG_DL_BITEXACT_ERROR    DEF_USRRC(APP_MODULE, FECDIAG, 11)
#define RC_FECDIAG_UL_BITEXACT_ERROR    DEF_USRRC(APP_MODULE, FECDIAG, 12)
#define RC_FECDIAG_UNDER_TEST           DEF_USRRC(APP_MODULE, FECDIAG, 13)

/******************************************************************************
*                The FEC objects addresses                                    *
*******************************************************************************/

typedef struct tFEC_DIAG_CTX
{
    TCB        Tcb;
    HANDLE     hFecHeap;
    PUINT8     pDLInData;
    PUINT8     pDLOutData;
    PUINT8     pDLRefData;
    UINT32     DLInLen;
    UINT32     DLOutLen;
    VUINT32    DLTestRes;

    PUINT8     pULInData;
    PUINT8     pULOutData;
    PUINT8     pULRefData;
    UINT32     ULInLen;
    UINT32     ULOutLen;
    VUINT32    ULTestRes;
    UINT32     LteWiMax;
} FEC_DIAG_CTX, *PFEC_DIAG_CTX;

#define DIAG_FEC_TYPE(p)            (p & 0x00000001)

#define DIAG_FEC_CTX_TYPE(p)        (p & 0x00000002)
#define DIAG_FEC_HW_CTX             0x00000000
#define DIAG_FEC_SW_CTX             0x00000002

#define DIAG_FEC_BYPASS_MODE(p)     (p & 0x00000004)
#define DIAG_FEC_BYPASS             0x00000004
#define DIAG_FEC_OPERATION          0x00000000

/******************************************************************************
*                The API declaration                                          *
*******************************************************************************/

MXRC  FecDiagInit (UINT32* pReportData, UINT32* pReportSize);
APIHEC FecDiagApiHandler (LPVOID pData, ApiHeader * pApi, ApiHeader * pResp);

#endif /*_FEC_DIAG_H_*/

#ifdef __cplusplus
}
#endif

