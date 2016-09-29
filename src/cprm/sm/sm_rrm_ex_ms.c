/**
 * @file  sm_ex_ms.c
 * @brief This file contains the definition of UE Handler stack manager activate task. 
 *                It handles all the events meant for UEH SM, and acts accordingly
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
#include "ueh_eut.h"           /* MAC RRM control Interface */
#include "lueh.h"
#include "ueh_eut_x.h"           /* MAC RRM control Interface */
#ifdef UEH_PERF_MEAS 
#include "ueh_perf.h"
#endif
#include "lueh.x" 
#include "ueh.h" 
#include "lkw.x"
#include "lrg.x"
#include "lpj.x"


/*
 *
 *       Fun:    smUehActvTsk - UE Handler Activation
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ueh_ex_ms.c
 *
 */

PUBLIC S16 smUehActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
{
	logFunc("%s", "smUehActvInit");

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
*       Desc:   Processes received event from UEH
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   ueh_sm_ex_ms.c
*
*/

PUBLIC S16 smUehActvTsk
(
Pst *pst,                   /* post */
Buffer *mBuf                /* message buffer */
)
{
   S16 ret;

   logFunc("%s", "smUehActvTsk");

   ret = ROK;
   switch(pst->event)
   {
#ifdef LCUEHMILUEH
      case EVTLUEHCFGCFM:             /* Config confirm */
         ret = cmUnpkLuehCfgCfm(SmMiLuehCfgCfm, pst, mBuf);
         break;
      case EVTLUEHCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLuehCntrlCfm(SmMiLuehCntrlCfm, pst, mBuf);
         break;
      case EVTLUEHSTAIND:
         ret = cmUnpkLuehStaInd(SmMiLuehStaInd, pst,mBuf);
         break;
#endif
      case EVTUEHSCTPCFG:
      case EVTUEHS1APCFG:
      case EVTUEHEGTPCFG:
      case EVTUEHAPPCFG:
      case EVTUEHRRCCFG:
      case EVTUEHPDCPCFG:
      case EVTUEHRLCCFG:
      case EVTUEHMACCFG:
      case EVTUEHCLCFG:
      case EVTUEHS1APSTKBND:
      case EVTUEHEGTPSTKBND:
      case EVTUEHRRCSTKBND:
      case EVTUEHENBALRMS:
      case EVTUEHENBDEBUGS:
      case EVTUEHINITS1SETUP:
      case EVTUEHCELLCFG:
      case EVTUEHUDHCFG:
      {
         smUehProcSm(pst->event);
         (Void)SPutMsg(mBuf);
         break;
      }
#ifdef LTE_L2_MEAS
      case EVTUEHMACL2MEASREQ:  
         logInfo ("%s", "MAC l2m req received from UEH");
         cmUnpkLrgSchL2MeasReq(smMacFwdL2MeasReq, pst , mBuf);
         break; 
        
      case EVTUEHRLCL2MEASREQ: 
         logInfo("%s", "RLC l2m req received from UEH");
         cmUnpkLkwL2MeasReq(smRlcFwdL2MeasReq, pst , mBuf);
         break; 
        
      case EVTUEHPDCPL2MEASREQ:
         logInfo("%s", "PDCP l2m req received from UEH");
         cmUnpkLpjL2MeasReq(smPdcpFwdL2MeasReq, pst , mBuf);
         break; 
#endif /* LTE_L2_MEAS */

      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         logError ("%s", "smUehActvTsk : Received invalid event");
         break;
   }

   RETVALUE(ret);

} /* end of smUehActvTsk */

#ifdef LTE_L2_MEAS_STUB
void l2MeasStub(
Pst *pst,                   /* post */
Buffer *mBuf                /* message buffer */
)
{
  S16 ret;

  logInfo ("l2m: l2MeasStub, received Event[%d]", pst->event);
  
  switch(pst->event)
  {
    case LKW_EVT_L2MEAS_REQ:
      ret = cmUnpkLkwL2MeasReq(sendRlcL2MeasCfm, pst, mBuf);
      break;

    case LPJ_EVT_L2MEAS_REQ:
      ret = cmUnpkLpjL2MeasReq(sendPdcpL2MeasCfm, pst, mBuf);
      break;

    case EVTLRGSCHL2MEASREQ:
      ret = cmUnpkLrgSchL2MeasReq(sendMacL2MeasCfm, pst, mBuf);
      break; 
      
      default:
      break;
  }
}
#endif
