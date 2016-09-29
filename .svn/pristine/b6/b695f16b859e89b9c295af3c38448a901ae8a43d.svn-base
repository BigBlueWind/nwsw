/**
 * @file    ueh_ct_ptli.c
 * @brief This file contains CTF SAP interface functions
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 16Sep2010 :
*     a) Created initial version from sample application
 */

#include "ccpu_cmn.h"

/* RRM related includes */
#include "ueh.h" 
#include "lueh.h" 
#include "lueh.x" 
#include "ueh_eut_x.h" 
#include "ctf.x"
#define MAXUEHLI 3

/* CTF Interface flags */
#ifndef LCUEHLICTF
#define PTUEHLICTF   1      /* Portable CTF Interface */
#else
#if(!(defined TF)  || !(defined DM))
#define PTUEHLICTF   1      /* Portable CTF Interface */
#endif /* TF && DM */
#endif /* PTUEHLICTF */

#ifdef PTUEHLICTF
/** Request from RRC to PHY to bind the CTF interface sap. */
EXTERN S16 PtLiCtfBndReq ARGS((Pst* pst, SuId suId, SpId spId));
/** Request from RRC to PHY to Unbind the CTF interface sap. */
EXTERN S16 PtLiCtfUbndReq ARGS((Pst* pst, SpId spId, Reason reason));
/** Configuration Request from RRC to PHY for 
 * cell or dedicated configuration. 
*/
EXTERN S16 PtLiCtfCfgReq ARGS((Pst* pst, SpId spId, CtfCfgTransId transId, CtfCfgReqInfo* cfgReqInfo));
/** UE ID change Request from RRC to PHY. */
EXTERN S16 PtLiCtfDatReq ARGS((Pst *pst,SpId spId,CtfKdfReqInfo *DatReqSdu));
EXTERN S16 PtLiCtfDatRsp ARGS((Pst* pst, SpId spId, CtfKdfCfmInfo *DatRspSdu));
EXTERN S16 PtLiCtfUeIdChgReq ARGS((Pst* pst, SpId spId, CtfCfgTransId transId, CtfUeInfo * ueInfo, CtfUeInfo * newUeInfo));
#endif /*--#ifdef PTUEHLICTF--*/



/* Request from RRC to PHY to bind the CTF interface sap. */
PRIVATE CONSTANT CtfBndReq UehLiCtfBndReqMt [MAXUEHLI] =
{
#ifdef LCUEHLICTF
   cmPkCtfBndReq,          /* 0 - loosely coupled */
#else  
   PtLiCtfBndReq,          /* 0 - loosely coupled, portable */
#endif /* LCUEHLICTF */
#ifdef TF 
   TfUiCtfBndReq,          /* 1 - tightly coupled, CTF */
#else
   PtLiCtfBndReq,          /* 1 - tightly coupled, portable */
#endif /* TF */
#ifdef DM 
   DmUiCtfBndReq,          /* 2 - tightly coupled, CTF */
#else
   PtLiCtfBndReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* Request from RRC to PHY to Unbind the CTF interface sap. */
PRIVATE CONSTANT CtfUbndReq UehLiCtfUbndReqMt [MAXUEHLI] =
{
#ifdef LCUEHLICTF
 cmPkCtfUbndReq,          /* 0 - loosely coupled */
#else  
 PtLiCtfUbndReq,          /* 0 - loosely coupled, portable */
#endif /* LCUEHLICTF */
#ifdef TF 
 TfUiCtfUbndReq,          /* 1 - tightly coupled, CTF */
#else
 PtLiCtfUbndReq,          /* 1 - tightly coupled, portable */
#endif /* TF */
#ifdef DM 
 DmUiCtfUbndReq,          /* 2 - tightly coupled, CTF */
#else
 PtLiCtfUbndReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* Configuration Request from RRC to PHY for 
 * cell or dedicated configuration. 
*/
PRIVATE CONSTANT CtfCfgReq UehLiCtfCfgReqMt[MAXUEHLI] =
{
#ifdef LCUEHLICTF
   cmPkCtfCfgReq,          /* 0 - loosely coupled */
#else  
   PtLiCtfCfgReq,          /* 0 - loosely coupled, portable */
#endif
#ifdef TF
   TfUiCtfCfgReq,          /* 1 - tightly coupled, CTF */
#else
   PtLiCtfCfgReq,          /* 1 - tightly coupled, portable */
#endif
#ifdef DM 
   DmUiCtfCfgReq,          /* 2 - tightly coupled, CTF */
#else
   PtLiCtfCfgReq,          /* 2 - tightly coupled, portable */
#endif
};

/* Data Request Primitive */
/***************SEC_CHANGE_START*****************/
PRIVATE CONSTANT CtfKdfReq uehLiCtfDatReqMt [MAXUEHLI] =
{
#ifdef LCUEHLICTF
   cmPkCtfKdfReq,            /* 0 - loosely coupled */
#endif
};

/***************SEC_CHANGE_END*****************/
/** UE ID change Request from RRC to PHY. */
PRIVATE CONSTANT CtfUeIdChgReq UehLiCtfUeIdChgReqMt[MAXUEHLI] =
{
#ifdef LCUEHLICTF
   cmPkCtfUeIdChgReq,
#else
   PtLiCtfUeIdChgReq,
#endif
#ifdef TF
   TfUiCtfUeIdChgReq,
#else
   PtLiCtfUeIdChgReq,
#endif
#ifdef DM
   DmUiCtfUeIdChgReq,
#else
   PtLiCtfUeIdChgReq,
#endif
};

/**************************************************************************
*
*      Fun:   UehLiCtfBndReq
*
*      Desc:  Bind Req
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  ueh_ct_ptli.c
*
*************************************************************************/

#ifndef RRC_PHY_CFG_SUPP
PUBLIC S16 UehLiCtfBndReq
(
Pst* pst,
SuId suId,
SpId spId
)
{
   logFunc("%s","UehLiCtfBndReq");
   RETVALUE((*UehLiCtfBndReqMt[pst->selector])(pst, suId, spId));
} /* end of function UehLiCtfBndReq */

/**************************************************************************
*
*      Fun:   UehLiCtfUbndReq
*
*      Desc:  Unbind Request
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  ueh_ct_ptli.c
*
*************************************************************************/

PUBLIC S16 UehLiCtfUbndReq
(
Pst* pst,
SpId spId,
Reason reason
)
{
   logFunc("%s","UehLiCtfUbndReq");

   RETVALUE((*UehLiCtfUbndReqMt[pst->selector])(pst, spId, reason));
} /* end of function UehLiCtfUbndReq */

/**************************************************************************
*
*      Fun:   UehLiCtfCfgReq
*
*      Desc:  CtfCfgReq
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  ueh_ct_ptli.c
*
*************************************************************************/
PUBLIC S16 UehLiCtfCfgReq(
    Pst                 *pst,                       /* Post structure */
    SpId                spId,                      /* Service provider ID */
    CtfCfgTransId       transId,          /* cfg trans ID */
    CtfCfgReqInfo       *cfgReqInfo       /* MAC Configuration information */
  )
{
  int ueId=-1;
  String50 details;
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif

  if(cfgReqInfo->u.cfg.cfgElem == CTF_UE_CFG)
    ueId = cfgReqInfo->u.cfg.u.dedCfg.ueId;
  snprintf((S8 *)details, sizeof(String50), "cfgType = %d", cfgReqInfo->cfgType);

   logFunc("%s","UehLiCtfCfgReq");
   UECM_OUTPUT(cfgReqInfo->u.cfg.u.dedCfg.ueId,UehLiCtfCfgReq,PHY,details);

#ifdef RRM_PRL_FLAG
   if(islogApiEnabled())
    {
      buff = GET_UEHCB_PRNT_LIB_BUFF();
      printCtfCfgReqInfo(cfgReqInfo, buff);
      logAPI("RRM - PHY : %s", buff->startAddr);
      buff->numBytesWritten = 0;
    }
#endif

   RETVALUE((*UehLiCtfCfgReqMt[pst->selector])(pst, spId, transId, cfgReqInfo));
} /* end of function UehLiCtfCfgReq */

/**************************************************************************
*
*      Fun:   UehLiCtfUeIdChgReq
*
*      Desc:  Ue Id Change Req
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  ueh_ct_ptli.c
*
*************************************************************************/

PUBLIC S16 UehLiCtfUeIdChgReq
(
Pst* pst,
SpId spId,
CtfCfgTransId transId,
CtfUeInfo * ueInfo,
CtfUeInfo * newUeInfo
)
{
  String50 details;
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif

  snprintf((S8 *)details, sizeof(String50), "New UE-id=%d",newUeInfo->ueId);
  logFunc("%s","UehLiCtfUeIdChgReq");
  UECM_OUTPUT(ueInfo->ueId,UehLiCtfUeIdChgReq,PHY,details);
#ifdef RRM_PRL_FLAG
  if(islogApiEnabled())
  {
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printCtfUeIdChgReq(transId, ueInfo, newUeInfo, buff);
     logAPI("RRM - PHY : %s", buff->startAddr);
     buff->numBytesWritten = 0;
  } 
#endif
  RETVALUE((*UehLiCtfUeIdChgReqMt[pst->selector])(pst, spId, transId, ueInfo, newUeInfo));
} /* end of UehLiCtfUeIdChgReq */

PUBLIC S16 uehLiCtfDatReq
(
Pst             *pst,            /* post structure */
SpId             spId,            /* RRC Service provider SAP ID */
U16               crnti,
CtfKdfReqInfo   *ctfKdfReqInfo   /* SDU */
)
{
   CtfCfgTransId transId;
   copyBytes((U8 *)&crnti,sizeof(U16),&transId);
   RETVALUE((*uehLiCtfDatReqMt[pst->selector])(pst, spId, transId,ctfKdfReqInfo));
}

#ifdef PTUEHLICTF
/**************************************************************************
 * The CTF (Portable) Interface
 *************************************************************************/
/**************************************************************************
 *
 *      Fun:   PtLiCtfBndReq
 *
 *      Desc:  Portable CtfBndReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  ueh_ct_ptli.c
 *
 *************************************************************************/

PUBLIC S16 PtLiCtfBndReq
(
Pst *pst,                       /* Post structure */
SuId suId,                      /* Service user ID */
SpId spId                       /* Service provider ID */
)
{
   logFunc("%s","PtLiCtfBndReq");

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);

   RETVALUE(ROK);
} /* end of function PtLiCtfBndReq */

/**************************************************************************
*
*      Fun:   PtLiCtfUbndReq
*
*      Desc:  Portable CtfUbndReq primitive
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  ueh_ct_ptli.c
*
*************************************************************************/
PUBLIC S16 PtLiCtfUbndReq
(
Pst *pst,                       /* Post structure */
SpId spId,                      /* Service provider ID */
Reason reason                   /* Reason */
)
{
   logFunc("%s","PtLiCtfUbndReq");

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(reason);

   RETVALUE(ROK);
} /* end of function PtLiCtfUbndReq */

/**************************************************************************
*
*      Fun:   PtLiCtfCfgReq
*
*      Desc:  Portable CtfCfgReq
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  ueh_ct_ptli.c
*
*************************************************************************/
PUBLIC S16 PtLiCtfCfgReq
(
Pst *pst,
SpId spId,
CtfCfgTransId transId,          /* cfg trans ID */
CtfCfgReqInfo *cfgReqInfo       /* PHY Configuration information */
)
{
  logFunc("%s","PtLiCtfCfgReq");

  UNUSED(pst);
  UNUSED(spId);
  UNUSED(transId);
  UNUSED(cfgReqInfo);

  RETVALUE(ROK);
} /* end of function PtLiCtfCfgReq */


PUBLIC S16 PtLiCtfUeIdChgReq
(
Pst* pst,
SpId spId,
CtfCfgTransId transId,
CtfUeInfo * ueInfo,
CtfUeInfo * newUeInfo
)
{
   logFunc("%s","PtLiCtfUeIdChgReq");

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(transId);
   UNUSED(ueInfo);
   UNUSED(newUeInfo);

   RETVALUE(ROK);
} /* end of PtLiCtfUeIdChgReq */
/***************SEC_CHANGE_START*****************/
PUBLIC S16 PtLiCtfDatReq
(
Pst *pst,
SpId spId,
CtfKdfReqInfo *DatReqSdu       
)
{

  UNUSED(pst);
  UNUSED(spId);
  UNUSED(DatReqSdu);


  RETVALUE(ROK);
} /* end of function PtLiCtfDatReq */

PUBLIC S16 PtLiCtfDatRsp
(
Pst* pst,
SpId spId,
CtfKdfCfmInfo *DatRspSdu
)
{

  UNUSED(pst);
  UNUSED(spId);
  UNUSED(DatRspSdu);

   RETVALUE(ROK);

}
/***************SEC_CHANGE_END*****************/

#endif /* PTUEHLICTF */
#endif

/**********************************************************************

        Revision history:

**********************************************************************/
