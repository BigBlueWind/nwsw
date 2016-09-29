//-------------------------------------------------------------------------------------------
/** @file swdiag.h
 *
 * @brief Turner Software Diagnostics module
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------


#ifndef SWDIAG_H_
#define SWDIAG_H_

#include "4gmx.h"
#include "appids.h"
#include "supervisor.h"
#include "apimgr.h"
#include "config.h"

#define RC_SWDIAG_ALLOC_ERR     DEF_USRRC(APP_MODULE, SWDIAG_APP, 1)
#define RC_SWDIAG_ERROR         DEF_USRRC(APP_MODULE, SWDIAG_APP, 2)
#define RC_SWDIAG_NOT_FOUND     DEF_USRRC(APP_MODULE, SWDIAG_APP, 3)
#define RC_SWDIAG_NAME_ERROR    DEF_USRRC(APP_MODULE, SWDIAG_APP, 4)
#define RC_SWDIAG_REG_MOD_ERR   DEF_USRRC(APP_MODULE, SWDIAG_APP, 5)
#define RC_SWDIAG_MODULE_ERR    DEF_USRRC(APP_MODULE, SWDIAG_APP, 6)
#define RC_SWDIAG_MOD_INIT_ERR  DEF_USRRC(APP_MODULE, SWDIAG_APP, 7)
#define RC_SWDIAG_BUF_ERR       DEF_USRRC(APP_MODULE, SWDIAG_APP, 8)

typedef enum _SWDIAG_MODULE_ID_
{
    SWDIAG_MOD_ID_RES  = 0,
    SWDIAG_MOD_ID_TEST = 1,

    // 
    MEMBERS_LIST_SIZE
}SWDIAG_MODULE_ID;

// initial data for Turner Software Diagnostics module
typedef struct
{
	BOOL            bSWDiagEn;      // enables software diagnostics 
	UINT32          LogDataSize;    // the size of buffer with results
	UINT32          ModID;          // name of a module
	
} SWDIAGINITDATA, *PSWDIAGINITDATA;

// the structure contains pointers to SWDiag context and their corresponding names  
typedef struct
{
    UINT32          ModID;
	BOOL            bSwdiagInited;      // this flag indicates whether SWDiag module has been initialized or not
	BOOL            bSWDiagEn;          // enables software diagnostics

	UINT32          LogDataSize;        // the size of buffer with results
	unsigned char*  LogDataPtr;         // the pointer to start address of the buffer with results of working
	UINT32          LogPut;
    UINT32          LogGet;
    
}SWDIAGCTX, *PSWDIAGCTX;

PSWDIAGCTX SWDiagGetCtx(UINT32 nModID);

MXRC SWDiagInit (PSWDIAGCTX pSwdiagctx, PSWDIAGINITDATA pSwdiagInitdata);
MXRC SWDiagClose(PSWDIAGCTX pSwdiagctx);

MXRC SWDiagEnable(PSWDIAGCTX pSwdiagctx);
MXRC SWDiagDisable(PSWDIAGCTX pSwdiagctx);
MXRC SWDiagnosticsInit(UINT32* pReportData, UINT32* pReportSize);
MXRC SWDiagnosticsDeinit(void);

void SWDiagAddLogData(PSWDIAGCTX pSwdiagctx, LPVOID data, UINT32 data_size);
void SWDiagPrintLogData(PSWDIAGCTX pSwdiagctx);

// API Test Module
MXRC SWDiagApiTestInit(PSWDIAGINITDATA pSwdiagInitdata, PUINT8 pData, UINT32 fillSize);


#endif /*SWDIAG_H_*/
