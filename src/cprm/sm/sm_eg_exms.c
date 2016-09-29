/**
 * @file  sm_eg_exms.c
 * @brief This file contains external activation functions for EGTPU
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
*     a) Created initial version from sample application
 */

/* header include files (.h) */
#include "ccpu_cmn.h"

/* RRM Related Includes */
#include "ueh.h"
#include "log.h"


/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from EG
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   ueh_sm_eg_exms.c
*
*/

PUBLIC S16 smEgActvTsk(
    Pst *pst,                   /* post */
    Buffer *mBuf                /* message buffer */
  )
{
   S16 ret;

   ret = ROK;

   switch(pst->event)
   {
#ifdef LCEGMILEG
      case EVTLEGCFGCFM:             /* Config confirm */
         ret = cmUnpkLegCfgCfm(SmMiLegCfgCfm, pst, mBuf);
         break;
      case EVTLEGCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLegCntrlCfm(SmMiLegCntrlCfm, pst, mBuf);
         break;
      case EVTLEGSTSCFM:           /* Statistics confirm */
         ret = cmUnpkLegStsCfm(SmMiLegStsCfm, pst, mBuf);
         break;
      case EVTLEGTRCIND:           /* Statistics confirm */
         ret = cmUnpkLegTrcInd(SmMiLegTrcInd, pst, mBuf);
         break;
#endif
      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         logError("SM Received invalid event:%d from EGTP",pst->event );
         break;
   }

   RETVALUE(ret);

} /* end of smEgActvTsk */
