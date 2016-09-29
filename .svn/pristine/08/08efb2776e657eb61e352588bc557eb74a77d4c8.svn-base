/**
 * @file   sm_nh_exms.c
 * @brief This file contains Activation functions for RRC
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
*     a) Created initial version from sample application
 */

/* header include files (.h) */
#include "ccpu_cmn.h"

#include "ueh.h"
#include "log.h"
/*
*
*       Fun:   Activate Task - initialize
*
*       Desc:  Invoked by system services to initialize a task.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smnhexms.c
*
*/

#ifdef ANSI
PUBLIC S16 smNhActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
#else
PUBLIC S16 smNhActvInit(ent, inst, region, reason)
Ent ent;                      /* entity */
Inst inst;                    /* instance */
Region region;                /* region */
Reason reason;                /* reason */
#endif
{
	logFunc("%s", "smNhActvInit");

   UNUSED(ent);
   UNUSED(inst);
   UNUSED(region);
   UNUSED(reason);
   RETVALUE(ROK);
}




/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from FP User layer
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   smrrexms.c
*
*/

PUBLIC S16 smNhActvTsk(
    Pst *pst,                   /* post */
    Buffer *mBuf                /* message buffer */
  )
{
   S16 ret;

   logFunc("%s", "smNhActvTsk");

   ret = ROK;

   switch(pst->event)
   {
#ifdef LCSMNHMILNH
      case EVTLNHCFGCFM:             /* Config confirm */
         ret = cmUnpkLnhCfgCfm(SmMiLnhCfgCfm, pst, mBuf);
         break;
      case EVTLNHCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLnhCntrlCfm(SmMiLnhCntrlCfm, pst, mBuf);
         break;
      case EVTLNHSTAIND:           /* Status Ind */
         ret = cmUnpkLnhStaInd(SmMiLnhStaInd, pst, mBuf);
         break;
      case EVTLNHSTSCFM:           /* Statistics confirm */
         ret = cmUnpkLnhStsCfm(SmMiLnhStsCfm, pst, mBuf);
         break;
#endif
#ifdef NH_TRACE_ENABLED
      case EVTLNHTRCIND:           /* Trace Ind */
         ret = cmUnpkLnhTrcInd(SmMiLnhTrcInd, pst, mBuf);
         break;
#endif
      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         logError("SM Received invalid event:%d from RRC",pst->event );
         break;
   }

   RETVALUE(ret);

} /* end of smNhActvTsk */
