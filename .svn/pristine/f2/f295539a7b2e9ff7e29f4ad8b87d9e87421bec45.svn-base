/**
 * @file   sm_sz_exms.c
 * @brief This file contains external activation functions for S1AP
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
*       Desc:   Processes received event from SZ
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   ueh_sm_sz_exms.c
*
*/

PUBLIC S16 smSzActvTsk(
    Pst *pst,                   /* post */
    Buffer *mBuf                /* message buffer */
  )
{
   S16 ret;

   logFunc("%s", "smSzActvTsk");

   ret = ROK;

   switch(pst->event)
   {
#ifdef LCSZMILSZ
      case EVTLSZCFGCFM:             /* Config confirm */
         ret = cmUnpkLszCfgCfm(SmMiLszCfgCfm, pst, mBuf);
         break;
      case EVTLSZCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLszCntrlCfm(SmMiLszCntrlCfm, pst, mBuf);
         break;
      case EVTLSZSTAIND:             /* Status Indication */
         ret = cmUnpkLszStaInd(SmMiLszStaInd, pst, mBuf);
         break;
      case EVTLSZTRCIND:            /* Trace Indication */
        ret = cmUnpkLszTrcInd(SmMiLszTrcInd, pst, mBuf);
        break;
      case EVTLSZSTSCFM:           /* Statistics confirm */
        ret = cmUnpkLszStsCfm(SmMiLszStsCfm, pst, mBuf);
        break;
#endif
      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         logError("SM Received invalid event:%d from S1AP",pst->event );
         break;
   }

   RETVALUE(ret);

} /* end of smSzActvTsk */
