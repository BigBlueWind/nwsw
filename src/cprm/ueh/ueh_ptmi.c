/**
 * @file    ueh_ptmi.c
 * @brief This file contains the UEH APIs implemented for the 
 *               layer manager, viz. LUEH confirms and indications
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 16Sep2010 :
*     a) Created initial version from sample application
 */

#include "ccpu_cmn.h"

/* RRM related includes */
#include"ueh_eut_x.h"
#include"ueh_eut.h"
#include "lueh.h"

#include "lueh.x" 
#include "ueh.h" 

#if (!defined(LCUEHMILUEH) || !defined(SM))
#define PTUEHMILUEH
#endif

#define UEH_LUEH_MAX_SEL       2      /* maximum number of selectors */

/* forward references */
#ifdef PTUEHMILUEH
PRIVATE S16 PtMiLuehCfgCfm    ARGS((Pst *pst, UehMngmt *cfg));
PRIVATE S16 PtMiLuehCntrlCfm  ARGS((Pst *pst, UehMngmt *cntrl));
PRIVATE S16 PtMiLuehStaInd  ARGS((Pst *pst, UehMngmt *usta));
#endif
/* Status Indication primitive */
PRIVATE LuehStaInd UehMiLuehStaIndMt[UEH_LUEH_MAX_SEL] =
{
#ifdef LCUEHMILUEH
   cmPkLuehStaInd,          /* 0 - loosely coupled  */
#else
   PtMiLuehStaInd,          /* 0 - tightly coupled, portable */
#endif /* LCUEHMILUEH */
#ifdef SM
   SmMiLuehStaInd,          /* 1 - tightly coupled, layer management */
#else
   PtMiLuehStaInd,          /* 1 - tightly coupled, portable */
#endif /* SM */
};

/* private variable definitions */
PRIVATE LuehCfgCfm uehMiLuehCfgCfmMt[UEH_LUEH_MAX_SEL] =
{
#ifdef LCUEHMILUEH
   cmPkLuehCfgCfm,
#else
   PtMiLuehCfgCfm,
#endif 
#ifdef SM
   SmMiLuehCfgCfm,
#else
   PtMiLuehCfgCfm,
#endif /* SM */
};

PRIVATE LuehCntrlCfm uehMiLuehCntrlCfmMt[UEH_LUEH_MAX_SEL] =
{
#ifdef LCUEHMILUEH
   cmPkLuehCntrlCfm,
#else
   PtMiLuehCntrlCfm,
#endif
#ifdef SM
   SmMiLuehCntrlCfm,
#else
   PtMiLuehCntrlCfm,
#endif
};

/*
*     layer management interface functions
*/
/*
*
*       Fun:    UehMiLuehCfgCfm
*
*       Desc:   configuration confirm
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   ueh_ptmi.c
*
*/

PUBLIC S16 UehMiLuehCfgCfm(
  Pst         *pst,
  UehMngmt     *cfg
  )
{
   logFunc("%s","UehMiLuehCfgCfm");
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst == (Pst *)NULLP || pst->selector >= UEH_LUEH_MAX_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
   RETVALUE((*uehMiLuehCfgCfmMt[pst->selector])(pst, cfg));
}

/*
*
*       Fun:    UehMiLuehCntrlCfm
*
*       Desc:   control confirm
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   ueh_ptmi.c
*
*/
PUBLIC S16 UehMiLuehCntrlCfm
(
Pst         *pst,             /* post structure */         
UehMngmt      *cntrl            /* management structure */   
)
{
   logFunc("%s","UehMiLuehCntrlCfm");
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst == (Pst *)NULLP || pst->selector >= UEH_LUEH_MAX_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
   RETVALUE((*uehMiLuehCntrlCfmMt[pst->selector])(pst, cntrl));
}


/**************************************************************************
 *
 *      Fun:  UehMiLuehStaInd  
 *
 *      Desc: Used to indicate the status of the RRC layer to the layer manager. 
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  ueh_ptmi.c
 *
 *************************************************************************/

PUBLIC S16 UehMiLuehStaInd
(
Pst *pst,                 /* Post structure */
UehMngmt *usta             /* Unsolicited status */
)
{
   logFunc("%s","UehMiLuehStaInd");

   /* jump to specific primitive depending on configured selector */
   (*UehMiLuehStaIndMt[pst->selector])(pst, usta); 

   RETVALUE(ROK);
} /* end of function UehMiLuehStaInd */


/* Portable functions */
#ifdef PTUEHMILUEH
/*
*
*       Fun:    PtMiLuehCfgCfm
*
*       Desc:   Dummy configuration confirm, customize if necessary
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   ueh_ptmi.c
*
*/

PUBLIC S16 PtMiLuehCfgCfm
(
Pst                     *pst,
UehMngmt                 *cfg
)
{
   logFunc("%s","PtMiLuehCfgCfm");
   UNUSED(pst);
   UNUSED(cfg);
   RETVALUE(RFAILED);
}

/*
*
*       Fun:    PtMiLuehCntrlCfm
*
*       Desc:   Dummy control confirm, customize if necessary
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   ueh_ptmi.c
*
*/

PUBLIC S16 PtMiLuehCntrlCfm
(
Pst                     *pst,
UehMngmt                 *cntrl
)
{
   logFunc("%s","PtMiLuehCntrlCfm");
   UNUSED(pst);
   UNUSED(cntrl);
   RETVALUE(RFAILED);
}

/**************************************************************************
 *
 *      Fun:  PtMiLuehStaInd (Portable) 
 *
 *      Desc: Used to indicate the status of the RRC layer to the layer manager. 
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  ueh_ptmi.c
 *
 *************************************************************************/

PUBLIC S16 PtMiLuehStaInd
(
Pst *pst,                 /* Post structure */
UehMngmt *usta             /* Unsolicited status */
)
{
   logFunc("%s","PtMiLuehStaInd");

   UNUSED(pst);
   UNUSED(usta);


   RETVALUE(ROK);
} /* end of PtMiLuehStaInd */

#endif

/**********************************************************************
  
        Revision history:
  
**********************************************************************/
