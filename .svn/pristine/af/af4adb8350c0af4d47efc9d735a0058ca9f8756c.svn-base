/**
 * @file  sm_sb_exms.c
 * @brief This file contains external activation functions for SCTP
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
*       File:  smsbexms.c
*
*/


PUBLIC S16 smSbActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
{
	logFunc("%s", "smSbActvInit");

   UNUSED(ent);
   UNUSED(inst);
   UNUSED(region);
   UNUSED(reason);
   RETVALUE(ROK);
} /* end of smSbActvInit */



/* local defines */

/* local typedefs */

/* forward references */
EXTERN  S16 smSbActvTsk     ARGS((Pst *pst, Buffer *mBuf));


/* public variable declarations */

/* public variable definitions */

/* private variable definitions */

/* public routines */

/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from SCTP layer
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:     ueh_sm_sb_exms.c
*
*/


PUBLIC S16 smSbActvTsk(
    Pst *pst,                   /* post */
    Buffer *mBuf                /* message buffer */
  )
 {
   S16 ret;
   logFunc("%s", "smSbActvTsk");
   ret = ROK;
  
   switch(pst->event)
   {
#ifdef LCSMSBMILSB
    
      case LSB_EVTCFGCFM:             /* Config confirm */
         ret = cmUnpkLsbCfgCfm(SmMiLsbCfgCfm, pst, mBuf);
         break;
      case LSB_EVTCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLsbCntrlCfm(SmMiLsbCntrlCfm, pst, mBuf);
         break;
      case LSB_EVTSTACFM:             /* Status Confirm */
         ret = cmUnpkLsbStaCfm(SmMiLsbStaCfm, pst, mBuf);
         break;
      case LSB_EVTSTSCFM:             /* Statistics Confirm */
         ret = cmUnpkLsbStsCfm(SmMiLsbStsCfm, pst, mBuf);
         break;
      case LSB_EVTSTAIND:             /* Status Indication */
            ret = cmUnpkLsbStaInd(SmMiLsbStaInd, pst, mBuf);
         break;
      case LSB_EVTTRCIND:             /* Trace Indication */
         ret = cmUnpkLsbTrcInd(SmMiLsbTrcInd, pst, mBuf);
         break;
#endif
      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         logError("SM Received invalid event:%d from SCTP",pst->event );
         break;
   }
   SExitTsk();
   RETVALUE(ret);
}
