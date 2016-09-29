/**
 * @file     ueh_udh_ptli.c
 * @details  Copyright Powerwave Technologies, Inc., 2010
 * @brief    This file contains UDH Sap interface functions
 *
 * @author   Bhogeswar M
 * @date     12-May-2011
 * @details  1. Basic version @n
 */

#include "ccpu_cmn.h"

/* RRM Related Files */
#include "ueh_udh_if.h"

/* local defines */
#define MAXUEHLI 2     /* Size of matrix used for LC/LWLC/TC/One for Dummy */

/* Bind Request Primitive */
PRIVATE UdhBndReq UehLiUdhBndReqMt [MAXUEHLI] =
{
  cmPkAndSendUehUdhBndReq,      /* 0 - loosely coupled */
#ifdef UDH
  udhProcUehUdhBndReq           /* 1 - tightly coupled */
#else
  cmPkAndSendUehUdhBndReq,      /* TODO : Need to write dummy functions */
#endif
};

/* Add ERAB Request Primitive */
PRIVATE UdhAddRabReq UehLiUdhAddRabReqMt [MAXUEHLI] =
{
  cmPkAndSendUehUdhAddRabReq,   /* 0 - loosely coupled */
#ifdef UDH
  udhProcUehUdhAddRabReq        /* 1 - tightly coupled */
#else
  cmPkAndSendUehUdhAddRabReq,   /* TODO : Need to write dummy functions */
#endif
};

/* Del ERAB Request Primitive */
PRIVATE UdhDelRabReq UehLiUdhDelRabReqMt [MAXUEHLI] =
{
  cmPkAndSendUehUdhDelRabReq,   /* 0 - loosely coupled */
#ifdef UDH
  udhProcUehUdhDelRabReq        /* 1 - tightly coupled */
#else
  cmPkAndSendUehUdhDelRabReq,   /* TODO : Need to write dummy functions */
#endif
};

/* Del UE Request Primitive */
PRIVATE UdhDelUeReq UehLiUdhDelUeReqMt [MAXUEHLI] =
{
  cmPkAndSendUehUdhDelUeReq,   /* 0 - loosely coupled */
#ifdef UDH
  udhProcUehUdhDelUeReq        /* 1 - tightly coupled */
#else
  cmPkAndSendUehUdhDelUeReq,   /* TODO : Need to write dummy functions */
#endif
};

/* Delete Local Tunnel primitive */
PUBLIC EgtEguLclTnlMgmtReq UehLiEgtEguLclTnlMgmtReqMt[] =
{
   cmPkEgtEguLclTnlMgmtReq,          /* 0 - loosely coupled */
   cmPkEgtEguLclTnlMgmtReq,          /* TODO : No tight coupling between UEH and EG EgUiEgtEguLclTnlMgmtReq :1 - tightly coupled, service user EU */
};

/**
 * @param     *pst  post structure
 * @param     suId  UDH serivce user SAP ID
 * @param     spId  UDH serivce provider SAP ID
 * @details   This function indicates the receipt of a bind request
 */
PUBLIC S16 UehLiUdhBndReq
(
  Pst    *pst,
  SuId   suId,
  SpId   spId
)
{
  logFunc("%s","UehLiUdhBndReq");
  RETVALUE((*UehLiUdhBndReqMt[pst->selector])(pst, suId, spId));
}

/**
 * @param     *pst  post structure
 * @param     suId  UDH serivce user SAP ID
 * @param     *addRabReq  UDH Add RAB structure
 * @details   This function indicates the receipt of a Add Rab Request
 */
PUBLIC S16 UehLiUdhAddRabReq
(
  Pst               *pst,
  SuId              suId,
  UehUdhAddRabReq   *addRabReq   /* UDH Add RAB structure */
)
{
  logFunc("%s","UehLiUdhAddRabReq");
  RETVALUE((*UehLiUdhAddRabReqMt[pst->selector])(pst, suId, addRabReq));
}

/**
 * @param     *pst  post structure
 * @param     suId  UDH serivce user SAP ID
 * @param     *delRabReq  UDH Delete RAB structure
 * @details   lower interface - Del Rab Request ; This function indicates the receipt of a Del Rab Request
 */
PUBLIC S16 UehLiUdhDelRabReq
(
  Pst               *pst,        /* post structure */
  SuId              suId,        /* UDH serivce user SAP ID */
  UehUdhDelRabReq   *delRabReq   /* UDH Del RAB structure */
)
{
  logFunc("%s","UehLiUdhDelRabReq");
  RETVALUE((*UehLiUdhDelRabReqMt[pst->selector])(pst, suId, delRabReq));
}

/**
 * @param     *pst  post structure
 * @param     suId  UDH serivce user SAP ID
 * @param     *delUeReq  UDH Delete UE structure
 * @details   lower interface - Del Ue Request ; This function indicates the receipt of a Del Ue Request
 */
PUBLIC S16 UehLiUdhDelUeReq
(
  Pst               *pst,        /* post structure */
  SuId              suId,        /* UDH serivce user SAP ID */
  UehUdhDelUeReq    *delUeReq
)
{
  logFunc("%s","UehLiUdhDelUeReq");
  RETVALUE((*UehLiUdhDelUeReqMt[pst->selector])(pst, suId, delUeReq));
}

/**
 * @param     *pst  post structure
 * @param     suId  UDH serivce user SAP ID
 * @param     *egtUEvnt  UDH Delete UE structure
 * @details   Egtp Tunnel Management Request
 */
PUBLIC S16 UehLiEgtEguLclTnlMgmtReq 
(
  Pst          *pst,
  SuId         suId,
  EgtUEvnt     *egtUEvnt
)
{
   logFunc("%s","UehLiEgtEguLclTnlMgmtReq");

   (*UehLiEgtEguLclTnlMgmtReqMt[pst->selector])(pst, suId, egtUEvnt);

   RETVALUE(ROK);
}


