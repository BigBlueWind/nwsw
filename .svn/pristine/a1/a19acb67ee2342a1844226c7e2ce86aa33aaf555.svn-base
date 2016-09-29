/**
 * @file  sm_hi_exms.c
 * @brief This file contains external activation functions for TUCL
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
#include "ueh.h"           /* stack management- TUCL */
#include "lueh.h"
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
*       File:  smhiexms.c
*
*/

PUBLIC S16 smHiActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
{
	logFunc("%s", "smHiActvInit");

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
*       Desc:   Processes received event from TUCL
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:     ueh_sm_hi_exms.c
*
*/
  
PUBLIC S16 smHiActvTsk
(
Pst    *spst,               /* post structure */
Buffer *mBuf                /* message buffer */
)
{
   S16           ret = ROK; /* return value from the function */

   switch(spst->event)
   {
#ifdef LCSMHIMILHI
      case EVTLHICFGCFM:             /* Configuration Confirm */
         if ((ret = cmUnpkLhiCfgCfm(SmMiLhiCfgCfm, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;
      
      case EVTLHISTSCFM:             /* Statistics Confirm */
         if ((ret = cmUnpkLhiStsCfm(SmMiLhiStsCfm, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;

      case EVTLHICNTRLCFM:           /* Conntrol Confirm */
         if ((ret = cmUnpkLhiCntrlCfm(SmMiLhiCntrlCfm, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;

      case EVTLHISTACFM:             /* Status Confirm */
         if ((ret = cmUnpkLhiStaCfm(SmMiLhiStaCfm, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;

     case EVTLHISTAIND:             /* Status Indication */
         if ((ret = cmUnpkLhiStaInd(SmMiLhiStaInd, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;

      case EVTLHITRCIND:             /* Trace Indication */
         if ((ret = cmUnpkLhiTrcInd(SmMiLhiTrcInd, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;
#endif
      default:
        (Void)SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
        SLogError(spst->dstEnt, spst->dstInst, spst->dstProcId, __FILE__, \
                   __LINE__, (ErrCls) ERRCLS_DEBUG, (ErrVal)EUEHXXX, 
                   (ErrVal)spst->event, "smHiActvTsk () Failed"); 
#endif
         break;
   }

   SExitTsk();

   RETVALUE(ret);
}/* end of smHiActvTsk() */
