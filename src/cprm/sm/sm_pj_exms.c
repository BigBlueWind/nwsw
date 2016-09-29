/**
 * @file  sm_pj_exms.c
 * @brief This file contains external activation functions for PDCP
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
*     a) Created initial version from sample application
 */

/* header include files (.h) */
#include "ccpu_cmn.h"

/* RRM Related Files */
#include "ueh.h"
#include "log.h"


/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from PJ
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   ueh_sm_pj_exms.c
*
*/

PUBLIC S16 smPjActvTsk(
    Pst *pst,                   /* post */
    Buffer *mBuf                /* message buffer */
  )
{
   S16 ret;

   ret = ROK;

   switch(pst->event)
   {
#ifdef LCPJMILPJ
      case LPJ_EVT_CFG_CFM:             /* Config confirm */
         ret = cmUnpkLpjCfgCfm(SmMiLpjCfgCfm, pst, mBuf);
         break;
      case LPJ_EVT_CNTRL_CFM:           /* Control confirm */
         ret = cmUnpkLpjCntrlCfm(SmMiLpjCntrlCfm, pst, mBuf);
         break;
      case LPJ_EVT_TRC_IND:           /* Trace indication */
         ret = cmUnpkLpjTrcInd(SmMiLpjTrcInd, pst, mBuf);
         break;
      case LPJ_EVT_STS_CFM:           /* Statistics confirmation */
         ret = cmUnpkLpjStsCfm(SmMiLpjStsCfm, pst, mBuf);
         break;
#endif
      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         logError("SM Received invalid event:%d from PDCP",pst->event );
         break;
   }

   RETVALUE(ret);

} /* end of smPjActvTsk */
