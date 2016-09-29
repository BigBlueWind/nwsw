/**
 * @file   sm_udh_exms.c
 * @brief This file contains the definition of UE Handler stack manager activate task. 
 *                It handles all the events meant for UDH SM, and acts accordingly
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
#include "ueh_eut.h"           /* MAC RRM control Interface */
#include "ueh.h" 
#include "lueh.h"
#include "ueh_eut_x.h"           /* MAC RRM control Interface */

#ifdef UEH_PERF_MEAS 
#include "ueh_perf.h"
#endif
#include "ueh_udh_if.h"
#include "sm.h"

#if 0
/*
 *
 *       Fun:    smUdhActvInit - UE Activation Initialization
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ueh_sm_udh_exms.c
 *
 */


PUBLIC S16 smUdhActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
{
	logFunc("%s", "smUdhActvInit");

   UNUSED(ent);
   UNUSED(inst);
   UNUSED(region);
   UNUSED(reason);
   RETVALUE(ROK);
}

#endif

/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from UDH
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   ueh_sm_udh_exms.c
*
*/

PUBLIC S16 smUdhActvTsk
(
Pst *pst,                   /* post */
Buffer *mBuf                /* message buffer */
)
{
   S16 ret;

   logFunc("%s", "smUdhActvTsk");

   ret = ROK;
   switch(pst->event)
   {
      case EVT_UDH_SM_SAP_CFG_CFM:
         ret = udhUnpkSmUdhSapCfgCfm(SmMiLudhCfgCfm, pst, mBuf);
         break;
      case EVT_UDH_SM_SAP_BIND_CFM:
         ret = cmUnpkUdhSmBndCfm(SmMiLudhBndCfm, pst, mBuf);
         break;
      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         logError ("smUdhActvTsk : Received invalid event Id: %d", pst->event);
         break;
   }

   RETVALUE(ret);

} /* end of smUdhActvTsk */
