/**
 * @file   sm_ys_exms.c
 * @brief This file contains external activation functions for CL
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


/** @brief  Invoked by system services to initialize a Stack Manager task.
 * @param ent 
 * @param inst 
 * @param region 
 * @param reason 
 * @return ROK/RFAILED
 */

PUBLIC S16 smYsActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
{
	logFunc("%s", "smYsActvInit");

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
*       Desc:   Processes received event from YS
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   ueh_sm_ys_exms.c
*
*/

PUBLIC S16 smYsActvTsk
(
Pst *pst,                   /* post */
Buffer *mBuf                /* message buffer */
)
{
   S16 ret;

   ret = ROK;

   switch(pst->event)
   {
#ifdef LCYSMILYS
      case EVTLYSCFGCFM:             /* Config confirm */
         ret = cmUnpkLysCfgCfm(SmMiLysCfgCfm, pst, mBuf);
         break;
      case EVTLYSCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLysCntrlCfm(SmMiLysCntrlCfm, pst, mBuf);
         break;
#endif
      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         logError ("%s", "smYsActvTsk : Received\
                           invalid event");
         break;
   }

   RETVALUE(ret);

} /* end of smYsActvTsk */
