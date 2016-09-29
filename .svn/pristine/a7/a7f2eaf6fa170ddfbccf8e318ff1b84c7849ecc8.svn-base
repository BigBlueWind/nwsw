/**
 * @file  sm_hi_ptmi.c
 * @brief This file contains layer management functions for TUCL
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
*     a) Created initial version from sample application
 */

/*
  
Layer management provides the necessary functions to control and
monitor the condition of each protocol layer.

The following functions are provided in this file:

     SmMiLhiCfgReq      Configure Request
     SmMiLhiStaReq      Status Request
     SmMiLhiStsReq      Statistics Request
     SmMiLhiCntrlReq    Control Request
   
It is assumed that the following functions are provided in the
stack management body files:

     SmMiLhiStaInd      Status Indication
     SmMiLhiStaCfm      Status Confirm
     SmMiLhiStsCfm      Statistics Confirm
     SmMiLhiTrcInd      Trace Indication
   
*/   

/* header include files (.h) */
  
#include "ccpu_cmn.h"

/* RRM Related Files */
#include "ueh.h"
#include "log.h"


#define  MAXHIMI  2

#ifndef  LCSMHIMILHI
#define  PTSMHIMILHI
#else
#ifndef   HI
#define  PTSMHIMILHI
#endif
#endif

PRIVATE U8 uehHiCfg = 0;
PRIVATE U8 uehHiTSapCnt = 0;

#ifdef PTSMHIMILHI
PRIVATE S16 PtMiLhiCfgReq    ARGS((Pst *pst, HiMngmt *cfg));
PRIVATE S16 PtMiLhiStsReq    ARGS((Pst *pst, Action action, HiMngmt *sts));
PRIVATE S16 PtMiLhiCntrlReq  ARGS((Pst *pst, HiMngmt *cntrl));
PRIVATE S16 PtMiLhiStaReq    ARGS((Pst *pst, HiMngmt *sta));
#endif


/*
the following matrices define the mapping between the primitives
called by the layer management interface of TCP UDP Convergence Layer
and the corresponding primitives in TUCL
 
The parameter MAXHIMI defines the maximum number of layer manager
entities on top of TUCL . There is an array of functions per primitive
invoked by TCP UDP Conbvergence Layer. Every array is MAXHIMI long
(i.e. there are as many functions as the number of service users).

The dispatching is performed by the configurable variable: selector.
The selector is configured during general configuration.

The selectors are:

   0 - loosely coupled (#define LCSMHIMILHI) 2 - Lhi (#define HI)

*/


/* Configuration request primitive */
 
PRIVATE LhiCfgReq SmMiLhiCfgReqMt[MAXHIMI] =
{
#ifdef LCSMHIMILHI
   cmPkLhiCfgReq,          /* 0 - loosely coupled  */
#else
   PtMiLhiCfgReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiMiLhiCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLhiCfgReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Control request primitive */
 
PRIVATE LhiCntrlReq SmMiLhiCntrlReqMt[MAXHIMI] =
{
#ifdef LCSMHIMILHI
   cmPkLhiCntrlReq,          /* 0 - loosely coupled  */
#else
   PtMiLhiCntrlReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiMiLhiCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLhiCntrlReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Statistics request primitive */
 
PRIVATE LhiStsReq SmMiLhiStsReqMt[MAXHIMI] =
{
#ifdef LCSMHIMILHI
   cmPkLhiStsReq,          /* 0 - loosely coupled  */
#else
   PtMiLhiStsReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiMiLhiStsReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLhiStsReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Status request primitive */
 
PRIVATE LhiStaReq SmMiLhiStaReqMt[MAXHIMI] =
{
#ifdef LCSMHIMILHI
   cmPkLhiStaReq,          /* 0 - loosely coupled  */
#else
   PtMiLhiStaReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiMiLhiStaReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLhiStaReq,          /* 1 - tightly coupled, portable */
#endif
};


/*
*     layer management interface functions 
*/
 
/*
*
*       Fun:   Configuration request
*
*       Desc:  This function is used to configure  TUCL
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_hi_ptmi.c
*
*/
  
PUBLIC S16 SmMiLhiCfgReq
(
Pst *spst,                /* post structure */
HiMngmt *cfg              /* configure */
)
{
   logFunc("%s","SmMiLhiCfgReq");
   /* jump to specific primitive depending on configured selector */
   (*SmMiLhiCfgReqMt[spst->selector])(spst, cfg); 
   RETVALUE(ROK);
} /* end of SmMiLhiCfgReq */

 /*
*
*       Fun:   Statistics request
*
*       Desc:  This function is used to request statistics from 
*              TUCL
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_hi_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiStsReq
(
Pst *spst,                /* post structure */
Action action,
HiMngmt *sts              /* statistics */
)
#else
PUBLIC S16 SmMiLhiStsReq(spst, action, sts)
Pst *spst;                /* post structure */   
Action action;
HiMngmt *sts;             /* statistics */
#endif
{
   logFunc("%s","SmMiLhiStsReq");
   /* jump to specific primitive depending on configured selector */
   (*SmMiLhiStsReqMt[spst->selector])(spst, action, sts); 
   RETVALUE(ROK);
} /* end of SmMiLhiStsReq */


/*
*
*       Fun:   Control request
*
*       Desc:  This function is used to send control request to 
*              TUCL
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_hi_ptmi.c
*
*/
  
PUBLIC S16 SmMiLhiCntrlReq
(
Pst *spst,                 /* post structure */
HiMngmt *cntrl            /* control */
)
{
   logFunc("%s","SmMiLhiCntrlReq");
   /* jump to specific primitive depending on configured selector */
   (*SmMiLhiCntrlReqMt[spst->selector])(spst, cntrl); 
   RETVALUE(ROK);
} /* end of SmMiLhiCntrlReq */


/*
*
*       Fun:   Status request
*
*       Desc:  This function is used to send a status request to 
*              TUCL
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_hi_ptmi.c
*
*/
  
PUBLIC S16 SmMiLhiStaReq
(
Pst *spst,                /* post structure */
HiMngmt *sta              /* status */
)
{
   logFunc("%s","SmMiLhiStaReq");
   /* jump to specific primitive depending on configured selector */
   (*SmMiLhiStaReqMt[spst->selector])(spst, sta); 
   RETVALUE(ROK);
} /* end of SmMiLhiStaReq */

#ifdef PTSMHIMILHI

/*
 *             Portable Functions
 */

/*
*
*       Fun:   Portable configure Request- TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_hi_ptmi.c
*
*/
  
PRIVATE S16 PtMiLhiCfgReq
(
Pst *spst,                  /* post structure */
HiMngmt *cfg                /* configure */
)
{
  logFunc("%s","PtMiLhiCfgReq");

  UNUSED(spst);
  UNUSED(cfg);

  RETVALUE(ROK);
} /* end of PtMiLhiCfgReq */



/*
*
*       Fun:   Portable statistics Request TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_hi_ptmi.c
*
*/
PRIVATE S16 PtMiLhiStsReq
(
Pst *spst,                  /* post structure */
Action action,
HiMngmt *sts                /* statistics */
)
{
  logFunc("%s","PtMiLhiStsReq");

  UNUSED(spst);
  UNUSED(action);
  UNUSED(sts);

  RETVALUE(ROK);
} /* end of PtMiLhiStsReq */


/*
*
*       Fun:   Portable control Request TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_hi_ptmi.c
*
*/
PRIVATE S16 PtMiLhiCntrlReq
(
Pst *spst,                  /* post structure */
HiMngmt *cntrl              /* control */
)
{
  logFunc("%s","PtMiLhiCntrlReq");

  UNUSED(spst);
  UNUSED(cntrl);

  RETVALUE(ROK);
} /* end of PtMiLhiCntrlReq */


/*
*
*       Fun:   Portable status Request TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_hi_ptmi.c
*
*/
  
PRIVATE S16 PtMiLhiStaReq
(
Pst *spst,                  /* post structure */
HiMngmt *sta                /* status */
)
{
  logFunc("%s","PtMiLhiStaReq");

  UNUSED(spst);
  UNUSED(sta);

  RETVALUE(ROK);
} /* end of PtMiLhiStaReq */

#endif /* PTSMHIMILHI */

/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/

PUBLIC S16 SmMiLhiCfgCfm(
  Pst     *pst,          /* post structure */
  HiMngmt *cfm           /* configuration */
  )
{
   Pst  hiPst;

   logFunc("%s","SmMiLhiCfgCfm");
  
  /*-- Update Event --*/
   UNUSED(pst);

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch(cfm->hdr.elmId.elmnt)
      {
         case STGEN:
            uehHiCfg |= UEH_HI_GEN_CFG;
            break;
         case STTSAP:
            uehHiCfg |= UEH_HI_TSAP_CFG;
            uehHiTSapCnt++;
            break;
         default:
           logError("TUCL Config confirm received with invalid element:%d",cfm->hdr.elmId.elmnt);
           break;
      } /* end of switch statement */

      logInfo("TUCL Config confirm received for the element:%d",cfm->hdr.elmId.elmnt);

      /* Wait for two Tsap confirms one for SCTP provider Sap
         and the other egtp provider Sap */
      if (uehHiCfg == UEH_SM_HI_CONFIGURED && uehHiTSapCnt == 2)
      {
         logInfo("%s", "TUCL CONFIGURATION SUCCESSFULLY PERFORMED");

         /* Initiate SCTP configuration after TUCL configuration. 
            So send EVTUEHSCTPCFG */
         smCb.smState = UEH_SM_STATE_TUCL_CFG_DONE;
         UEH_FILL_PST(hiPst, SM_HI_PROC, ENTSM, ENTUEH, EVTUEHSCTPCFG, UEH_POOL, SM_SELECTOR_LC);
         uehSmSelfPstMsg(&hiPst);
      }
   }
   else
   {
     logError("Configuration Failure received from TUCL for element:%d",cfm->hdr.elmId.elmnt);
   }

   RETVALUE(ROK);
}


/*
*
*       Fun:   Control Confirm
*
*       Desc:  This function is used by to present control confirm
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/

PUBLIC S16 SmMiLhiCntrlCfm(
  Pst     *pst,          /* post structure */
  HiMngmt *cntrl           /* configuration */
  )
{
    logFunc("%s","SmMiLhiCntrlCfm");

    switch(cntrl->hdr.elmId.elmnt)
    {
      case STSZGEN:
      {
        if (cntrl->cfm.status == LCM_PRIM_OK)
        {
          switch(cntrl->hdr.transId)
          {
            case SM_TUCL_DEBUG_TRANS_ID:
            {
              logInfo("%s", "Debug is enabled for TUCL");
              //smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | UEH_SM_HI_DBG_MASK);
              smCb.cfgCb.smDbgMsk = (smCb.cfgCb.smDbgMsk | UEH_SM_HI_DBG_MASK);
              break;
            }
            default:
            {
              logError("General Control confirm received from TUCL for unknown Subaction:%d", cntrl->t.cntrl.subAction );
              break;
            }
          }
        }
        else
        {
          logError("General Control Fail received from TUCL with, Action:%d, Subaction:%d",
                       cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
        }
        break;
      }
      default:
      {
        if(cntrl->cfm.status == LCM_PRIM_OK)
        {
          logInfo("Control confirm received from TUCL for Element:%d, Action:%d, Subaction:%d",
              cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
        }
        else
        {
          logError("Control fail received from TUCL for Element:%d, Action:%d, Subaction:%d",
              cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
        }
        break;
      }
    }

    RETVALUE(ROK);
} /* end of SmMiLhiCntrlCfm */

/*
*
*       Fun:   Statistics Confirm
*
*       Desc:  This function is used by to present statistics confirm
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/

PUBLIC S16 SmMiLhiStsCfm
(
Pst     *pst,          /* post structure */
HiMngmt *cfm           /* configuration */
)
{
  logFunc("%s","SmMiLhiStsCfm");
  RETVALUE(ROK);
} /* end of SmMiLhiStsCfm */

/*
*
*       Fun:   Status Confirm
*
*       Desc:  This function is used by to present status confirm
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/

PUBLIC S16 SmMiLhiStaCfm
(
Pst     *pst,          /* post structure */
HiMngmt *cfm           /* configuration */
)
{
  logFunc("%s","SmMiLhiStaCfm");
  RETVALUE(ROK);
} /* end of SmMiLhiStaCfm */

/*
*
*       Fun:   Status Indication
*
*       Desc:  This function is used by to present status indication
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/

PUBLIC S16 SmMiLhiStaInd
(
Pst     *pst,          /* post structure */
HiMngmt *cfm           /* configuration */
)
{
  logFunc("%s","SmMiLhiStaInd");
  RETVALUE(ROK);
} /* end of SmMiLhiStaInd*/

/*
*
*       Fun:   Trace Indication
*
*       Desc:  This function is used by to present trace indication
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/

PUBLIC S16 SmMiLhiTrcInd
(
Pst     *pst,          /* post structure */
HiMngmt *cfm,           /* configuration */
Buffer  *mBuf
)
{
  logFunc("%s","SmMiLhiStaInd");
  RETVALUE(ROK);
} /* end of SmMiLhiTrcInd*/
