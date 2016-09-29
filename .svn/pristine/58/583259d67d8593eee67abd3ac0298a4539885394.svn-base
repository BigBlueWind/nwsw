/**
 * @file   sm_ys_ptmi.c
 * @brief This file contains management interface functions for CL
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


#define  MAXYSMI  2

PRIVATE U8 uehYsCfg = 0;
PRIVATE U8 uehNumTfuSaps = 0;

#ifndef  LCSMYSMILYS
#define  PTSMYSMILYS
#else
#ifndef   YS
#define  PTSMYSMILYS
#endif
#endif

#ifdef PTSMYSMILYS
PRIVATE S16 PtMiLysCfgReq    ARGS((Pst *pst, YsMngmt *cfg));
PRIVATE S16 PtMiLysCntrlReq  ARGS((Pst *pst, YsMngmt *cntrl));
PRIVATE S16 PtMiLysStaReq    ARGS((Pst *pst, YsMngmt *sta));
#endif


/*
the following matrices define the mapping between the primitives
called by the layer management interface of TCP UDP Convergence Layer
and the corresponding primitives in TUCL
 
The parameter MAXYSMI defines the maximum number of layer manager
entities on top of TUCL . There is an array of functions per primitive
invoked by TCP UDP Conbvergence Layer. Every array is MAXYSMI long
(i.e. there are as many functions as the number of service users).

The dispatcysng is performed by the configurable variable: selector.
The selector is configured during general configuration.

The selectors are:

   0 - loosely coupled (#define LCSMYSMILYS) 2 - Lys (#define YS)

*/


/* Configuration request primitive */
 
PRIVATE LysCfgReq SmMiLysCfgReqMt[MAXYSMI] =
{
#ifdef LCSMYSMILYS
   cmPkLysCfgReq,          /* 0 - loosely coupled  */
#else
   PtMiLysCfgReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef YS
   YsMiLysCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLysCfgReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Control request primitive */
 
PRIVATE LysCntrlReq SmMiLysCntrlReqMt[MAXYSMI] =
{
#ifdef LCSMYSMILYS
   cmPkLysCntrlReq,          /* 0 - loosely coupled  */
#else
   PtMiLysCntrlReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef YS
   YsMiLysCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLysCntrlReq,          /* 1 - tightly coupled, portable */
#endif
};




/* Status request primitive */
 
PRIVATE LysStaReq SmMiLysStaReqMt[MAXYSMI] =
{
#ifdef LCSMYSMILYS
   cmPkLysStaReq,          /* 0 - loosely coupled  */
#else
   PtMiLysStaReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef YS
   YsMiLysStaReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLysStaReq,          /* 1 - tightly coupled, portable */
#endif
};


/*
*     layer management interface functions 
*/
 
/*
*
*       Fun:   Configuration request
*
*       Desc:  Tyss function is used to configure  TUCL
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_ys_ptmi.c
*
*/
  
PUBLIC S16 SmMiLysCfgReq
(
Pst *spst,                /* post structure */
YsMngmt *cfg              /* configure */
)
{
   logFunc("%s","SmMiLysCfgReq");
   /* jump to specific primitive depending on configured selector */
   (*SmMiLysCfgReqMt[spst->selector])(spst, cfg); 
   RETVALUE(ROK);
} /* end of SmMiLysCfgReq */



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
*       File:     ueh_sm_ys_ptmi.c
*
*/
  
PUBLIC S16 SmMiLysCntrlReq
(
Pst *spst,                 /* post structure */
YsMngmt *cntrl            /* control */
)
{
   logFunc("%s","SmMiLysCntrlReq");
   /* jump to specific primitive depending on configured selector */
   (*SmMiLysCntrlReqMt[spst->selector])(spst, cntrl); 
   RETVALUE(ROK);
} /* end of SmMiLysCntrlReq */


/*
*
*       Fun:   Status request
*
*       Desc:  Tyss function is used to send a status request to 
*              TUCL
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_ys_ptmi.c
*
*/
  
PUBLIC S16 SmMiLysStaReq
(
Pst *spst,                /* post structure */
YsMngmt *sta              /* status */
)
{
   logFunc("%s","SmMiLysStaReq");
   /* jump to specific primitive depending on configured selector */
   (*SmMiLysStaReqMt[spst->selector])(spst, sta); 
   RETVALUE(ROK);
} /* end of SmMiLysStaReq */

#ifdef PTSMYSMILYS

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
*       File:     ueh_sm_ys_ptmi.c
*
*/
  
PRIVATE S16 PtMiLysCfgReq
(
Pst *spst,                  /* post structure */
YsMngmt *cfg                /* configure */
)
{
  logFunc("%s","PtMiLysCfgReq");

  UNUSED(spst);
  UNUSED(cfg);

  RETVALUE(ROK);
} /* end of PtMiLysCfgReq */


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
*       File:     ueh_sm_ys_ptmi.c
*
*/
PRIVATE S16 PtMiLysCntrlReq
(
Pst *spst,                  /* post structure */
YsMngmt *cntrl              /* control */
)
{
  logFunc("%s","PtMiLysCntrlReq");

  UNUSED(spst);
  UNUSED(cntrl);

 RETVALUE(ROK);
} /* end of PtMiLysCntrlReq */

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
*       File:     ueh_sm_ys_ptmi.c
*
*/
  

PRIVATE S16 PtMiLysStaReq
(
Pst *spst,                  /* post structure */
YsMngmt *sta                /* status */
)
{
  logFunc("%s","PtMiLysStaReq");

  UNUSED(spst);
  UNUSED(sta);

 RETVALUE(ROK);
} /* end of PtMiLysStaReq */

#endif /* PTSMYSMILHI */

/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_sm_ys_ptmi.c
*
*/

PUBLIC S16 SmMiLysCfgCfm(
    Pst     *pst,          /* post structure */
    YsMngmt *cfm           /* configuration */
  )
{
   Pst      ysPst;

   logFunc("%s","SmMiLysCfgCfm");

   SM_SET_ZERO(&ysPst, sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STGEN:
            uehYsCfg |= UEH_YS_GEN_CFG;
            break;      
         case STTFUSAP:
            uehNumTfuSaps++;
            if (uehNumTfuSaps == 2)
            {
               uehNumTfuSaps = 0;
               uehYsCfg |= UEH_YS_TFU_SAP_CFG;
            }
           break;
         case STCTFSAP:
            uehYsCfg |= UEH_YS_CTF_SAP_CFG;
            break;
         default:
           logError("CL Config Confirm received for invalid element:%d",cfm->hdr.elmId.elmnt);
            break;
      }

      logInfo("CL Config Confirm received for element:%d",cfm->hdr.elmId.elmnt);
      if (uehYsCfg == UEH_SM_YS_CONFIGURED)
      {
         logInfo("%s", "CL CONFIGURATION SUCCESSFULLY PERFORMED");

         smCb.smState = UEH_SM_STATE_CL_CFG_DONE;
         UEH_FILL_PST(ysPst, SM_UEH_PROC, ENTSM, ENTUEH, EVTUEHENBDEBUGS, UEH_POOL, SM_SELECTOR_LC);
         if(uehSmSelfPstMsg(&ysPst) != ROK)
         {
           logError("%s", "Error in posting the message to SM for Configuring Debug levels in Stacks");
         }
         else
         {
           logInfo("%s", "Message to SM sent for Configuring Debug levels in Stacks");
         }
      }
   }
   else
   {
     logError("Configuration Failure received from CL for element:%d",cfm->hdr.elmId.elmnt);
   }

   SM_FREE(DFLT_REGION, DFLT_POOL, cfm, sizeof(YsMngmt)); /* Manu: mem-leak-fix */
   RETVALUE(ROK);
}

/*
*
*       Fun:   Control Confirm
*
*       Desc:  This function is used by to present control confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_sm_ys_ptmi.c
*
*/
PUBLIC S16 SmMiLysCntrlCfm(
    Pst     *pst,          /* post structure */
    YsMngmt *cntrl         /* control */
  )
{
   logFunc("%s","SmMiLysCntrlCfm");

   switch(cntrl->hdr.elmId.elmnt)
   {
     case STSZGEN:
     {
       if (cntrl->cfm.status == LCM_PRIM_OK)
       {
         switch(cntrl->hdr.transId)
         {
           case SM_CL_DEBUG_TRANS_ID:
           {
             logInfo("%s", "Debug is enabled for CL");
             smCb.cfgCb.smDbgMsk = (smCb.cfgCb.smDbgMsk | UEH_SM_YS_DBG_MASK);
             break;
           }
           default:
           {
             logError("General Control confirm received from CL for unknown Subaction:%d", cntrl->t.cntrl.subAction );
             break;
           }
         }
       }
       else
       {
         logError("General Control Fail received from CL with, Action:%d, Subaction:%d",
                      cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
       }
       break;
     }
     default:
     {
       if(cntrl->cfm.status == LCM_PRIM_OK)
       {
         logInfo("Control confirm received from CL for Element:%d, Action:%d, Subaction:%d",
             cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
       }
       else
       {
         logError("Control fail received from CL for Element:%d, Action:%d, Subaction:%d",
             cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
       }
       break;
     }
   }

   SM_FREE(DFLT_REGION, DFLT_POOL, cntrl, sizeof(YsMngmt)); /* Manu: mem-leak-fix */
   RETVALUE(ROK);
}
