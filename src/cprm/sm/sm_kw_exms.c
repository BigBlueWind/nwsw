/**
 * @file  sm_kw_exms.c
 * @brief This file contains RLC activation functions
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
*       File:  smkwexms.c
*
*/


PUBLIC S16 smKwActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
{
	logFunc("%s", "smKwActvInit");

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
*       Desc:   Processes received event from KW
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   ueh_sm_kw_exms.c
*
*/


PUBLIC S16 smKwActvTsk(
    Pst *pst,                   /* post */
    Buffer *mBuf                /* message buffer */
  )
{
   S16 ret;

   logFunc("%s", "smKwActvTsk");

   ret = ROK;

   switch(pst->event)
   {
#ifdef LCKWMILKW
      case LKW_EVT_CFG_CFM:             /* Config confirm */
         ret = cmUnpkLkwCfgCfm(SmMiLkwCfgCfm, pst, mBuf);
         break;
      case LKW_EVT_CNTRL_CFM:           /* Control confirm */
         ret = cmUnpkLkwCntrlCfm(SmMiLkwCntrlCfm, pst, mBuf);
         break;
      case LKW_EVT_STA_IND:           /* Status Indication*/
         ret = cmUnpkLkwStaInd(SmMiLkwStaInd , pst, mBuf);
         break;
      case LKW_EVT_TRC_IND:           /* Trace Indication*/
         ret = cmUnpkLkwTrcInd(SmMiLkwTrcInd , pst, mBuf);
         break;
      case LKW_EVT_STS_CFM:           /* Statistics confirm*/
         ret = cmUnpkLkwStsCfm(SmMiLkwStsCfm , pst, mBuf);
         break;
#ifdef LTE_L2_MEAS
      case LKW_EVT_L2MEAS_CFM:     /*L2 Meas confirm */ 
         logInfo("%s","l2m: LKW_EVT_L2MEAS_CFM received at SM");
         smRlcFwdL2MeasCfm(pst, mBuf);
         break;
#endif /*LTE_L2_MEAS*/
#endif
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
#ifdef LTE_L2_MEAS
      case LPJ_EVT_L2MEAS_CFM:     /*L2 Meas confirm */
         logInfo("%s","l2m: LPJ_EVT_L2MEAS_CFM received at SM");
         smPdcpFwdL2MeasCfm(pst, mBuf);
         break;
#endif /*LTE_L2_MEAS*/
#endif
 
      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         logError("SM Received invalid event:%d from RLC/PDCP",pst->event );
         break;
   } /* end of switch statement */

   RETVALUE(ret);

} /* end of smKwActvTsk */
