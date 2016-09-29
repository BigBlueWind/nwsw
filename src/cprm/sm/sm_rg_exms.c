/**
 * @file   sm_rg_exms.c
 * @brief This file contains external activation functions for MAC
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
*       Fun:   Activate Task - initialize
*
*       Desc:  Invoked by system services to initialize a task.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File: ueh_sm_rg_exms.c
*
*/

PUBLIC S16 smRgActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
{
	logFunc("%s", "smRgActvInit");

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
*       Desc:   Processes received event
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   ueh_sm_rg_exms.c
*
*/

PUBLIC S16 smRgActvTsk(
    Pst *pst,                   /* post */
    Buffer *mBuf                /* message buffer */
  )
{
   S16 ret;

   logFunc("%s", "smRgActvTsk");

   ret = ROK;

   switch(pst->event)
   {
#ifdef LCLRG
      case EVTLRGCFGCFM:             /* Config confirm */
         ret = cmUnpkLrgCfgCfm(SmMiLrgCfgCfm, pst, mBuf);
         break;
      case EVTLRGSCHCFGCFM:             /* Config confirm */
         ret = cmUnpkLrgSchCfgCfm(SmMiLrgSchCfgCfm, pst, mBuf);
         break;
      case EVTLRGCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLrgCntrlCfm(SmMiLrgCntrlCfm, pst, mBuf);
         break;
      case EVTLRGSCHCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLrgCntrlCfm(SmMiLrgSchCntrlCfm, pst, mBuf);
         break;
      case EVTLRGTRCIND:           /* Trace confirm */
         ret = cmUnpkLrgTrcInd(SmMiLrgTrcInd, pst, mBuf);
         break;
      case EVTLRGSTSCFM:           /* statistics confirm */
         ret = cmUnpkLrgStsCfm(SmMiLrgStsCfm, pst, mBuf);
         break;
#ifdef LTE_L2_MEAS
      case EVTLRGSCHL2MEASCFM:   
         logInfo("%s","SM Received L2M meas cfm from MAC");
         ret = smMacFwdL2MeasCfm(pst, mBuf); 
         break;
#endif /*LTE_L2_MEAS*/

#endif
      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         logError("SM Received invalid event:%d from MAC",pst->event );
         break;
   }

   RETVALUE(ret);

} /* end of smRgActvTsk */
