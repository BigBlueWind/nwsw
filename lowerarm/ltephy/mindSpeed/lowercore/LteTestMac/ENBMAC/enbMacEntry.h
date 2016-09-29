/** @file enbMacEntry.h
 *
 * @brief LTE MAC startup functions for 4GMX integration
 * @author Mindspeed Technologies
 *
 * COPYRIGHT (c) 2009-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/

#ifndef _ENB_MAC_ENTRY_H_
#define _ENB_MAC_ENTRY_H_

#include "4gmx.h"
#include "appids.h"
#include "cmgr.h"

/*
 * LTE MAC Module Error Codes
 */

/*
#define RC_LTE_OK                  MX_OK

#define RC_LTE_PARAM_ERROR         DEF_USRRC(APP_MODULE, LTEMACAPP, 1)
#define RC_LTE_ALLOC_ERROR         DEF_USRRC(APP_MODULE, LTEMACAPP, 2)
#define RC_LTE_UNKNOWN_COMMAND     DEF_USRRC(APP_MODULE, LTEMACAPP, 3)
#define RC_PHY_UNKNOWN_API         DEF_USRRC(APP_MODULE, LTEMACAPP, 4)
#define RC_PHY_UNKNOWN_TYPE        DEF_USRRC(APP_MODULE, LTEMACAPP, 5)
#define RC_PHY_UNKNOWN_PARAM       DEF_USRRC(APP_MODULE, LTEMACAPP, 6)
#define RC_PHY_CONTEXT_ALLOC_ERROR DEF_USRRC(APP_MODULE, LTEMACAPP, 7)
#define RC_PHY_INCORRECT_PARAM     DEF_USRRC(APP_MODULE, LTEMACAPP, 8)
#define RC_PHY_NO_VALID_DATA_ERROR DEF_USRRC(APP_MODULE, LTEMACAPP, 9)
#define RC_PHY_HEAP_ALLOC_ERROR    DEF_USRRC(APP_MODULE, LTEMACAPP, 10)
#define RC_LTE_TIMEOUT             DEF_USRRC(APP_MODULE, LTEMACAPP, 11)
#define RC_LTE_INVALID_MODE        DEF_USRRC(APP_MODULE, LTEMACAPP, 12)
*/

/*
 * 4GMX Thread Scheduler
 */
#define MAC_PROCESSING_THREAD_ARM_ID ((MxGetARMID () == 0) ? 3 : 1)

/*
 * Global Application Callback
 */
extern MXRC (*LteMacAppInitCallback)(void);

/** @brief DOX_INTERNAL This function is called by the function of the L-ARM application init.
           (the function AppInitCpu0 of appinit.c file).

    @return [MXRC] the initialization error code (0 (or MX_OK) - OK)
*/
MXRC LteMacInitHandler(void);

/** @brief DOX_INTERNAL This function will be called by the console manager in case of
            running <lte> console command.

    @NOTE!!!  This function is specially designed for debug purpose.

    @param pCtx     [in] - the console manager context (it can be skipped )
    @param pParams  [in] - the command parameters specified for console command
                           for example: "lte myparam1 myparam2 ... mypaamN"

    @return [UINT32] it should return 0 (or) MX_OK

 */
UINT32 LteMacConsoleHandler(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);

#endif /* _ENB_MAC_ENTRY_H_ */

