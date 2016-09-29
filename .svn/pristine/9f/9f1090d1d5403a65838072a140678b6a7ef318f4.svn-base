//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL LTE_entry.h
 *
 * @brief The entry point of the LTE application:
 *           - The API of application initialization
 *           - The console handler (for debug purpose)
 * @author Mindspeed Technologies
 * @version $Revision: 1.30 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef _LTE_ENTRY_H_
#define _LTE_ENTRY_H_

#include "4gmx.h"
#include "config.h"
#include "appids.h"
#include "cmgr.h"

// parameters ID
#ifndef APIDEFS_H_
#define PHY_QUE_HDR_PARAM_ID      (2)
#endif
#define PHY_SECTOR_PARAM_ID       (2)
#define PHY_BUF_SIZE_PARAM_ID     (3)
#define PHY_BUF_OFFS_PARAM_ID     (4)
#define PHY_DATA_PARAM_ID         (5)
#define PHY_ANTENNA_NUM_PARAM_ID  (6)
#define PHY_RUNNING_INFO_US       (7)
#define PHY_SF_NUMBER       (8)
#define PHY_PAR_FFT_NUM       (9)
#define PHY_PAR_ANT_TOTALNUM       (10)

/*****************************************************************************
 *                   The list of error codes                                  *
 ******************************************************************************/

#define RC_LTE_OK                  MX_OK
#define RC_LTE_PARAM_ERROR         DEF_USRRC(APP_MODULE, LTEAPP, 1)
#define RC_LTE_ALLOC_ERROR         DEF_USRRC(APP_MODULE, LTEAPP, 2)
#define RC_LTE_UNKNOWN_COMMAND     DEF_USRRC(APP_MODULE, LTEAPP, 3)
#define RC_PHY_UNKNOWN_API         DEF_USRRC(APP_MODULE, LTEAPP, 4)
#define RC_PHY_UNKNOWN_TYPE        DEF_USRRC(APP_MODULE, LTEAPP, 5)
#define RC_PHY_UNKNOWN_PARAM       DEF_USRRC(APP_MODULE, LTEAPP, 6)
#define RC_PHY_CONTEXT_ALLOC_ERROR DEF_USRRC(APP_MODULE, LTEAPP, 7)
#define RC_PHY_INCORRECT_PARAM     DEF_USRRC(APP_MODULE, LTEAPP, 8)
#define RC_PHY_NO_VALID_DATA_ERROR DEF_USRRC(APP_MODULE, LTEAPP, 9)
#define RC_PHY_HEAP_ALLOC_ERROR    DEF_USRRC(APP_MODULE, LTEAPP, 10)
#define RC_LTE_TIMEOUT             DEF_USRRC(APP_MODULE, LTEAPP, 11)
#define RC_LTE_INVALID_MODE        DEF_USRRC(APP_MODULE, LTEAPP, 12)

/*****************************************************************************
 *                   Debug Messages for Host Validation                       *
 *****************************************************************************/

#define UNCODED_OUT    1
#define FEC_DEC_OUT    2
#define PROFILE_INFO   3
#define IQ_SAMPLES     4
#define SRS_CHEST_OUT  5
#define PRACH_OUT      6
#define MLOG_OUT       7
#define MLOG_SIZE      8

#define LteConsoleFunction(a) UINT32 (*a)(*CMgrCmdCtx, *CMgrCmdParamCtx)

/*****************************************************************************
 *                   The LTE types declaration                                *
 ******************************************************************************/
typedef struct
{
    UINT32 nInstanceID;
    UINT32 nMsgHandlerID; // LTE Msg handler
    UINT32 nMacInstanceId;
    UINT32 nDummy;
    // buffer for output messages to MAC
    UINT32 msgBuf[16 * 1024 / sizeof(UINT32)];

} LTEAppCtx;


/*****************************************************************************
 *                   The declaration of the interface API                     *
 ******************************************************************************/

/** @brief DOX_INTERNAL This function is called by the function of the L-ARM application init.
 (the function AppInitCpu0 of appinit.c file).

 @return [MXRC] the initialization error code (0 (or MX_OK) - OK)
 */

MXRC LTEInitHandler(void);

MXRC LTERegisterPhyConsoleFunctions(TCMgrCommand handler, UINT32 CommandType);

UINT32 LteCreatePhy(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 LtePhyStart(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 LtePhyPause(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 LtePhyResume(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 LtePhyStop(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
UINT32 LtePhyShutdown(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);


/** @brief DOX_INTERNAL This function will be called by the console manager in case of
 running <lte> console command.

 @NOTE!!!  This function is specially designed for debug purpose.

 @param pCtx     [in] - the console manager context (it can be skipped )
 @param pParams  [in] - the command parameters specified for console command
 for example: "lte myparam1 myparam2 ... mypaamN"

 @return [UINT32] it should return 0 (or) MX_OK

 */

extern LTEAppCtx *pLteCtx;
extern short MacCpuID;

INLINE void SetMacInstanceId(UINT32 MacId)
{
    pLteCtx->nMacInstanceId = MacId;
}

INLINE UINT32 GetMacInstanceId(void)
{
    return pLteCtx->nMacInstanceId;
}

void LteSendListToRmtMac(UINT32 localHead);



#endif /*_LTE_ENTRY_H_*/

