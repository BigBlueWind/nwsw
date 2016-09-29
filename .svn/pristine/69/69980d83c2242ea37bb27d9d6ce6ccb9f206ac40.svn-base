/**
 * @file     ueh_sz_ptli.c
 * @details  Copyright Powerwave Technologies, Inc., 2010
 * @brief    TThis file contains SZT SAP interface functions
 *
 * @author    Amar Ramteke
 * @date     16-Sep-2010
 * @details  1. Basic version @n
 */
 
/* header include files (.h) */
#include "ccpu_cmn.h"

/* RRM Related Files */
#include "ueh_eut.h"
#include "lueh.h"
#include "ueh_eut_x.h"
#include "lueh.x"
#include "ueh.h"
#include "s1ap_mgr.h"
#include "log.h"
#include "szt.x"
#ifdef RRM_PRL_FLAG
#include "trace.h"
#endif

/* ---- S1AP service user lower interface ---- */
/*
 *     this software may be combined with the following TRILLIUM
 *     software:
 *
 *     part no.                      description
 *     --------    ----------------------------------------------
 *     1000XXX                      S1AP v 1.1
 */


/************************************************************************
                              Prototypes  
************************************************************************/

#ifdef PTUZLISZT

PRIVATE S16 PtLiSztBndReq       ARGS((Pst *pst, SuId suId, SpId spId));
PRIVATE S16 PtLiSztUbndReq      ARGS((Pst *pst, SpId spId, Reason reason));
PRIVATE S16 PtLiSztUDatReq      ARGS((Pst *pst, SpId spId, SztUDatEvnt *uDatEvnt));
PRIVATE S16 PtLiSztConReq       ARGS((Pst *pst, SpId spId, SztConReq *conReq));
PRIVATE S16 PtLiSztConRsp       ARGS((Pst *pst, SpId spId, SztConRsp *conRsp));
PRIVATE S16 PtLiSztRelReq       ARGS((Pst *pst, SpId spId, SztRelReq *relReq));
PRIVATE S16 PtLiSztRelRsp       ARGS((Pst *pst, SpId spId, SztRelRsp *relRsp));
PRIVATE S16 PtLiSztDatReq       ARGS((Pst *pst, SpId spId, SztDatEvntReq *datEvnt));
PRIVATE S16 PtLiSztEncDecReq    ARGS((Pst *pst, SpId spId, SztEncDecEvnt *sztEncDecEvnt));
PRIVATE S16 PtLiSztAudReq       ARGS((Pst *pst, SpId spId, SztAudEvnt *audEvnt));
#if (defined(SZ_MME) && defined(SZ_DYNAMIC_PEER))
PRIVATE S16 PtLiSztPeerAssocRsp ARGS((Pst *pst, SpId spId, SztPeerAssocRsp *peerAssocRsp));
#endif

#endif

PUBLIC S16 UehLiSztAbortCfm (Pst *pst,SuId suId,SztAbortS1 *abortS1);
/* Following matrices define the mapping between the primitives called by the
 * S1AP user (as UehLiSztXXXReq) and the corresponding primitives of the 
 * S1AP layer (service provider) (as SzUiSztXXXReq).
 * The mapping is based on pst->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCUZLISZT    loose         cmPkSztXXXReq
 * 1                 SZ          tight         SzUiSztXXXReq
 * 2                LWLCUZLISZT  LW loose      cmPkSztXXXReq
 */


PUBLIC SztBndReq UehLiSztBndReqMt [] =
{
#ifdef LCUZLISZT
   cmPkSztBndReq,        /* 0 - loosely coupled */
#else
   PtLiSztBndReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztBndReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztBndReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztBndReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztBndReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztUbndReq UehLiSztUbndReqMt [] =
{
#ifdef LCUZLISZT
   cmPkSztUbndReq,        /* 0 - loosely coupled */
#else
   PtLiSztUbndReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztUbndReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztUbndReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztUbndReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztUbndReq,        /* 2 - tightly coupled, portable */
#endif
};

PUBLIC SztAbortS1Req UehLiSztAbortReqMt[] = 
{
  cmPkSztAbortS1Req, /* 0 - loosely coupled */
  SzUiSztAbortS1Req,  /* 1 - tightly coupled */ 
};

PUBLIC SztUDatReq UehLiSztUDatReqMt [] =
{
#ifdef LCUZLISZT
   cmPkSztUDatReq,        /* 0 - loosely coupled */
#else
   PtLiSztUDatReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztUDatReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztUDatReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztUDatReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztUDatReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztConnReq UehLiSztConReqMt [] =
{
#ifdef LCUZLISZT
   cmPkSztConReq,        /* 0 - loosely coupled */
#else
   PtLiSztConReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztConReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztConReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztConReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztConReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztConnRsp UehLiSztConRspMt [] =
{
#ifdef LCUZLISZT
   cmPkSztConRsp,        /* 0 - loosely coupled */
#else
   PtLiSztConRsp,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztConRsp,        /* 1 - light weight loosely coupled */
#else
   PtLiSztConRsp,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztConRsp,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztConRsp,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztRelsReq UehLiSztRelReqMt [] =
{
#ifdef LCUZLISZT
   cmPkSztRelReq,        /* 0 - loosely coupled */
#else
   PtLiSztRelReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztRelReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztRelReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztRelReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztRelReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztRelsRsp UehLiSztRelRspMt [] =
{
#ifdef LCUZLISZT
   cmPkSztRelRsp,        /* 0 - loosely coupled */
#else
   PtLiSztRelRsp,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztRelRsp,        /* 1 - light weight loosely coupled */
#else
   PtLiSztRelRsp,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztRelRsp,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztRelRsp,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztDatReq UehLiSztDatReqMt [] =
{
#ifdef LCUZLISZT
   cmPkSztDatReq,        /* 0 - loosely coupled */
#else
   PtLiSztDatReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztDatReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztDatReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztDatReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztDatReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztEncDecReq UehLiSztEncDecReqMt [] =
{
#ifdef LCUZLISZT
   cmPkSztEncDecReq,        /* 0 - loosely coupled */
#else
   PtLiSztEncDecReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztEncDecReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztEncDecReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztEncDecReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztEncDecReq,        /* 2 - tightly coupled, portable */
#endif
};

PUBLIC SztAudReq UehLiSztAudReqMt [] =
{
#ifdef LCUZLISZT
   cmPkSztAudReq,        /* 0 - loosely coupled */
#else
   PtLiSztAudReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztAudReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztAudReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztAudReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztAudReq,        /* 2 - tightly coupled, portable */
#endif
};

#if (defined(SZ_MME) && defined(SZ_DYNAMIC_PEER))
PUBLIC SztDynamPeerAssocRsp UehLiSztPeerAssocRspMt [] =
{
#ifdef LCUZLISZT
   cmPkSztPeerAssocRsp,        /* 0 - loosely coupled */
#else
   PtLiSztPeerAssocRsp,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztPeerAssocRsp,        /* 1 - light weight loosely coupled */
#else
   PtLiSztPeerAssocRsp,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztPeerAssocRsp,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztPeerAssocRsp,        /* 2 - tightly coupled, portable */
#endif
};
#endif

PUBLIC S16 UehLiSztAudReq
(
Pst *pst,
SpId spId,
SztAudEvnt *audEvnt
);

/************************************************************************
                       SZT Interface Functions
************************************************************************/

/*
 *
 *       Fun:   UehLiSztBndReq
 *
 *       Desc:  This function resolves the SztBndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PUBLIC S16 UehLiSztBndReq(
  Pst *pst,
  SuId suId,
  SpId spId
)
{
  logFunc("%s","UehLiSztBndReq");

  RETVALUE((*UehLiSztBndReqMt[pst->selector])(pst, suId, spId));

} /* UehLiSztBndReq */

/*
 *
 *       Fun:   UehLiSztUAbortReq
 *
 *       Desc:  This function is used to send the abort request to the S1AP protocol 
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */

PUBLIC void UehLiSztUAbortReq(
    Pst *pst,
    SpId spId,
    SztAbortS1 *s1Abort
    )
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
  logFunc("%s","UehLiSztUAbortReq");
#ifdef RRM_PRL_FLAG
#ifdef SZTV2
  if(islogApiEnabled())
  {   
     buff = GET_UEHCB_PRNT_LIB_BUFF(); 
     printSztAbortS1(s1Abort, buff);
     logAPI("RRM - S1AP: %s", buff->startAddr);
     buff->numBytesWritten = 0;
  }
#endif   
#endif   

  (*UehLiSztAbortReqMt[pst->selector])(pst,spId,s1Abort);

}
/*
 *
 *       Fun:   UehLiSztUbndReq
 *
 *       Desc:  This function resolves the SztUbndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PUBLIC S16 UehLiSztUbndReq
(
Pst *pst,
SpId spId,
Reason reason
)
{
  logFunc("%s","UehLiSztUbndReq");
  (*UehLiSztUbndReqMt[pst->selector])(pst, spId, reason);

  RETVALUE(ROK);
} /* UehLiSztUbndReq */

/*
 *
 *       Fun:   UehLiSztUDatReq
 *
 *       Desc:  This function resolves the SztUDatReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PUBLIC S16 UehLiSztUDatReq
(
Pst *pst,
SpId spId,
SztUDatEvnt *uDatEvnt
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
  logFunc("%s","UehLiSztUDatReq");
#ifdef RRM_PRL_FLAG
  if(islogApiEnabled())
  {   
     buff = GET_UEHCB_PRNT_LIB_BUFF(); 
     printUiSztUDatReq(uDatEvnt, buff);
     logAPI("RRM - S1AP: %s", buff->startAddr);
     buff->numBytesWritten = 0;
}
#endif   
  (*UehLiSztUDatReqMt[pst->selector])(pst, spId, uDatEvnt);

  RETVALUE(ROK);
} /* UehLiSztUDatReq */

/*
 *
 *       Fun:   UehLiSztConReq
 *
 *       Desc:  This function resolves the SztConReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PUBLIC S16 UehLiSztConReq
(
Pst *pst,
SpId spId,
SztConReq *conReq
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
  logFunc("%s","UehLiSztConReq");

#ifdef RRM_PRL_FLAG
  if(islogApiEnabled())
  {   
    buff = GET_UEHCB_PRNT_LIB_BUFF(); 
    printUiSztConReq(conReq, buff);
    logAPI("RRM - S1AP: %s", buff->startAddr);
    buff->numBytesWritten = 0; 
  }
#endif   

  (*UehLiSztConReqMt[pst->selector])(pst, spId, conReq);

  RETVALUE(ROK);
} /* UehLiSztConReq */

/*
 *
 *       Fun:   UehLiSztConRsp
 *
 *       Desc:  This function resolves the SztConRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PUBLIC S16 UehLiSztConRsp
(
Pst *pst,
SpId spId,
SztConRsp *conRsp
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
  logFunc("%s","UehLiSztConRsp");

#ifdef RRM_PRL_FLAG
  if(islogApiEnabled())
  {   
    buff = GET_UEHCB_PRNT_LIB_BUFF(); 
    printUiSztConRsp(conRsp, buff);
    logAPI("RRM - S1AP: %s", buff->startAddr);
    buff->numBytesWritten = 0;
  }
#endif   
  (*UehLiSztConRspMt[pst->selector])(pst, spId, conRsp);

  RETVALUE(ROK);
} /* UehLiSztConRsp */

/*
 *
 *       Fun:   UehLiSztRelReq
 *
 *       Desc:  This function resolves the SztRelReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */

PUBLIC S16 UehLiSztRelReq(
    Pst         *pst,
    SpId         spId,
    SztRelReq   *relReq
  )
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s","UehLiSztRelReq");
#ifdef RRM_PRL_FLAG
  if(islogApiEnabled())
  {   
    buff = GET_UEHCB_PRNT_LIB_BUFF(); 
    printUiSztRelReq(relReq, buff);
    logAPI("RRM - S1AP: %s", buff->startAddr);
    buff->numBytesWritten = 0;
  }
#endif
   (*UehLiSztRelReqMt[pst->selector])(pst, spId, relReq);

   RETVALUE(ROK);
} /* UehLiSztRelReq */

/*
 *
 *       Fun:   UehLiSztRelRsp
 *
 *       Desc:  This function resolves the SztRelRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PUBLIC S16 UehLiSztRelRsp
(
Pst *pst,
SpId spId,
SztRelRsp *relRsp
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s","UehLiSztRelRsp");

#ifdef RRM_PRL_FLAG
  if(islogApiEnabled())
  {   
    buff = GET_UEHCB_PRNT_LIB_BUFF(); 
    printUiSztConRsp(relRsp, buff);    //doubtful
    logAPI("RRM - S1AP: %s", buff->startAddr);
    buff->numBytesWritten = 0;
  }
#endif
   (*UehLiSztRelRspMt[pst->selector])
      (pst, spId, relRsp);

   RETVALUE(ROK);
} /* UehLiSztRelRsp */

/*
 *
 *       Fun:   UehLiSztDatReq
 *
 *       Desc:  This function resolves the SztDatReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PUBLIC S16 UehLiSztDatReq
(
Pst *pst,
SpId spId,
SztDatEvntReq *datEvnt
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s","UehLiSztDatReq");

#ifdef RRM_PRL_FLAG
  if(islogApiEnabled())
  {   
    buff = GET_UEHCB_PRNT_LIB_BUFF(); 
    printUiSztDatReq(datEvnt, buff);
    logAPI("RRM - S1AP: %s", buff->startAddr);
    buff->numBytesWritten = 0;
  }
  
#endif
   (*UehLiSztDatReqMt[pst->selector])
      (pst, spId, datEvnt);

   RETVALUE(ROK);
} /* UehLiSztDatReq */

/*
 *
 *       Fun:   UehLiSztEncDecReq
 *
 *       Desc:  This function resolves the SztEncDecReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PUBLIC S16 UehLiSztEncDecReq
(
Pst *pst,
SpId spId,
SztEncDecEvnt *sztEncDecEvnt
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s","UehLiSztEncDecReq");

#ifdef RRM_PRL_FLAG
   if(islogApiEnabled())
{   
   buff = GET_UEHCB_PRNT_LIB_BUFF(); 
    printUiSztEncDecReq(sztEncDecEvnt, buff);
   logAPI("RRM - S1AP: %s", buff->startAddr);
   buff->numBytesWritten = 0;
   
}
   
#endif
   (*UehLiSztEncDecReqMt[pst->selector])
      (pst, spId, sztEncDecEvnt);

   RETVALUE(ROK);
} /* UehLiSztEncDecReq */

/*
 *
 *       Fun:   UehLiSztAudReq
 *
 *       Desc:  This function resolves the SztAudReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PUBLIC S16 UehLiSztAudReq
(
Pst *pst,
SpId spId,
SztAudEvnt *audEvnt
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s","UehLiSztAudReq");
#ifdef RRM_PRL_FLAG
   if(islogApiEnabled())
   {   
     buff = GET_UEHCB_PRNT_LIB_BUFF(); 
     printUiSztAudEvnt(audEvnt,buff);
     logAPI("RRM - S1AP: %s", buff->startAddr);
     buff->numBytesWritten = 0;
   
   }
  
#endif

   (*UehLiSztAudReqMt[pst->selector])
      (pst, spId, audEvnt);

   RETVALUE(ROK);
} /* UehLiSztAudReq */

#if (defined(SZ_MME) && defined(SZ_DYNAMIC_PEER))
/*
 *
 *       Fun:   UehLiSztPeerAssocRsp
 *
 *       Desc:  This function resolves the SztPeerAssocRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PUBLIC S16 UehLiSztPeerAssocRsp
(
Pst *pst,
SpId spId,
SztPeerAssocRsp *peerAssocRsp
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s","UehLiSztPeerAssocRsp");

#ifdef RRM_PRL_FLAG
    if(islogApiEnabled())
     {   
       buff = GET_UEHCB_PRNT_LIB_BUFF(); 
       printSztPeerAssocRsp(peerAssocRsp, buff);
       logAPI("RRM - S1AP: %s", buff->startAddr);
       buff->numBytesWritten = 0;
   
     }
  
#endif   
   (*UehLiSztPeerAssocRspMt[pst->selector])
      (pst, spId, peerAssocRsp);

   RETVALUE(ROK);
} /* UehLiSztPeerAssocRsp */
#endif



/************************************************************************
                       Portable Functions
************************************************************************/

#ifdef PTUZLISZT
/*
 *
 *       Fun:   PtLiSztBndReq
 *
 *       Desc:  Portable version of SztBndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PRIVATE S16 PtLiSztBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
{
   logFunc("%s","PtLiSztBndReq");

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztBndReq() called");
#endif
   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);

   RETVALUE(RFAILED);
} /* PtLiSztBndReq */

/*
 *
 *       Fun:   PtLiSztUbndReq
 *
 *       Desc:  Portable version of SztUbndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */

PRIVATE S16 PtLiSztUbndReq
(
Pst *pst,
SpId spId,
Reason reason
)
{
   logFunc("%s","PtLiSztUbndReq");

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztUbndReq() called");
#endif
   UNUSED(pst);
   UNUSED(spId);

   RETVALUE(RFAILED);
} /* PtLiSztUbndReq */

/*
 *
 *       Fun:   PtLiSztUDatReq
 *
 *       Desc:  Portable version of SztUDatReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PRIVATE S16 PtLiSztUDatReq
(
Pst *pst,
SpId spId,
SztUDatEvnt *uDatEvnt
)
{
   logFunc("%s","PtLiSztUDatReq");


#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztUDatReq() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(uDatEvnt);

   RETVALUE(RFAILED);
} /* PtLiSztUDatReq */

/*
 *
 *       Fun:   PtLiSztConReq
 *
 *       Desc:  Portable version of SztConReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PRIVATE S16 PtLiSztConReq
(
Pst *pst,
SpId spId,
SztConReq *conReq
)
{
   logFunc("%s","PtLiSztConReq");


#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztConReq() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(conReq);

   RETVALUE(RFAILED);
} /* PtLiSztConReq */

/*
 *
 *       Fun:   PtLiSztConRsp
 *
 *       Desc:  Portable version of SztConRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PRIVATE S16 PtLiSztConRsp
(
Pst *pst,
SpId spId,
SztConRsp *conRsp
)
{
   logFunc("%s","PtLiSztConRsp");


#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztConRsp() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(conRsp);

   RETVALUE(RFAILED);
} /* PtLiSztConRsp */

/*
 *
 *       Fun:   PtLiSztRelReq
 *
 *       Desc:  Portable version of SztRelReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PRIVATE S16 PtLiSztRelReq
(
Pst *pst,
SpId spId,
SztRelReq *relReq
)
{
   logFunc("%s","PtLiSztRelReq");


#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztRelReq() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(relReq);

   RETVALUE(RFAILED);
} /* PtLiSztRelReq */

/*
 *
 *       Fun:   PtLiSztRelRsp
 *
 *       Desc:  Portable version of SztRelRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PRIVATE S16 PtLiSztRelRsp
(
Pst *pst,
SpId spId,
SztRelRsp *relRsp
)
{
   logFunc("%s","PtLiSztRelRsp");


#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztRelRsp() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(relRsp);

   RETVALUE(RFAILED);
} /* PtLiSztRelRsp */

/*
 *
 *       Fun:   PtLiSztDatReq
 *
 *       Desc:  Portable version of SztDatReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PRIVATE S16 PtLiSztDatReq
(
Pst *pst,
SpId spId,
SztDatEvntReq *datEvnt
)
{
   logFunc("%s","PtLiSztDatReq");

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztDatReq() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(datEvnt);

   RETVALUE(RFAILED);
} /* PtLiSztDatReq */

/*
 *
 *       Fun:   PtLiSztEncDecReq
 *
 *       Desc:  Portable version of SztEncDecReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PRIVATE S16 PtLiSztEncDecReq
(
Pst *pst,
SpId spId,
SztEncDecEvnt *sztEncDecEvnt
)
{
   logFunc("%s","PtLiSztEncDecReq");

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztEncDecReq() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(sztEncDecEvnt);

   RETVALUE(RFAILED);
} /* PtLiSztEncDecReq */

/*
 *
 *       Fun:   PtLiSztAudReq
 *
 *       Desc:  Portable version of SztAudReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PRIVATE S16 PtLiSztAudReq
(
Pst *pst,
SpId spId,
SztAudEvnt *audEvnt
)
{
   logFunc("%s","PtLiSztAudReq");

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztAudReq() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(audEvnt);

   RETVALUE(RFAILED);
} /* PtLiSztAudReq */

#if (defined(SZ_MME) && defined(SZ_DYNAMIC_PEER))
/*
 *
 *       Fun:   PtLiSztPeerAssocRsp
 *
 *       Desc:  Portable version of SztPeerAssocRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ueh_sz_ptli.c
 *
 */
PRIVATE S16 PtLiSztPeerAssocRsp
(
Pst *pst,
SpId spId,
SztPeerAssocRsp *peerAssocRsp
)
{
   logFunc("%s","PtLiSztPeerAssocRsp");


#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztPeerAssocRsp() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(peerAssocRsp);

   RETVALUE(RFAILED);
} /* PtLiSztPeerAssocRsp */
#endif

#endif /* PTUZLISZT */

/*S1AP mapping functions*/


/*
*
*       Fun:   UzLiSztBndCfm
*       
*       Desc:  Call handler for Bind Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ueh_sz_ptli.c
*       
*/
PUBLIC S16 UzLiSztBndCfm
(
Pst            *pst,
SuId           suId,
U8             status
)
{
   logFunc("%s","UzLiSztBndCfm");
 
   UehLiSztBndCfm(pst, suId, status);

   RETVALUE(ROK);
} /* End of UzLiSztBndCfm */

/*
*
*       Fun:   UzLiSztAbortS1Cfm
*              
*       Desc:  Call handler for Abort Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ueh_sz_ptli.c
*       
*/

PUBLIC S16 UzLiSztAbortS1Cfm(    
    Pst          *pst,
    SuId         suId,
    SztAbortS1   *abortS1
 )
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
  logFunc("%s","UzLiSztAbortS1Cfm");

#ifdef RRM_PRL_FLAG
#ifdef SZTV2
  if (islogApiEnabled())
   { 
      buff = GET_UEHCB_PRNT_LIB_BUFF();
      printSztAbortS1(abortS1, buff);
      logAPI("RRM - S1AP: %s", buff->startAddr); 
      buff->numBytesWritten = 0;
   } 
 
   
#endif   
#endif   
  UehLiSztAbortCfm( pst,suId,abortS1);

  RETVALUE(ROK);
}
/*
*
*       Fun:   UzLiSztAudCfm
*
*       Desc:  Call handler for Aduit Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ueh_sz_ptli.c
*       
*/
PUBLIC S16 UzLiSztAudCfm
(
Pst            *pst,
SuId           suId,
SztAudEvnt     *audEvnt
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s","UzLiSztAudCfm");
   
#ifdef RRM_PRL_FLAG
 if (islogApiEnabled())
  { 
    buff = GET_UEHCB_PRNT_LIB_BUFF();
    printUiSztAudEvnt(audEvnt,buff);
    logAPI("RRM - S1AP: %s", buff->startAddr); 
    buff->numBytesWritten = 0;
  } 
 #endif
   RETVALUE(ROK);
} /* End of UzLiSztAudCfm */

/*
*
*       Fun:   UzLiSztStaInd
*
*       Desc:  Call handler for Status Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ueh_sz_ptli.c
*       
*/
PUBLIC S16 UzLiSztStaInd
(
Pst            *post,
SuId           suId,
SztStaInd      *sztSta
)
{
   logFunc("%s","UzLiSztStaInd");
#ifdef RRM_PRL_FLAG
   Buff *buff = GET_UEHCB_PRNT_LIB_BUFF();
   printUiSztStaInd(sztSta, buff);
   logStateChange("S1AP-RRM| %s", buff->startAddr);
   buff->numBytesWritten = 0;
#endif
   s1apProcStatInd( post, suId, sztSta );
   RETVALUE(ROK);
} /* End of UzLiSztStaInd */

/*
*
*       Fun:   UzLiSztFlcInd
*
*       Desc:  Call handler for Status Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ueh_sz_ptli.c
*       
*/
PUBLIC S16 UzLiSztFlcInd
(
Pst            *post,
SuId           suId,
SztFlcInd      *flcInd
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s","UzLiSztFlcInd");
#ifdef RRM_PRL_FLAG
    if (islogApiEnabled())
     { 
       buff = GET_UEHCB_PRNT_LIB_BUFF();
       printUiSztFlcInd(flcInd, buff);
       logAPI("RRM - S1AP: %s", buff->startAddr);
       buff->numBytesWritten = 0;
    } 
   
#endif   
 
   RETVALUE(ROK);
} /* End of UzLiSztFlcInd */

/*
*
*       Fun:   UzLiSztErrInd
*
*       Desc:  Call handler for Error Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ueh_sz_ptli.c
*       
*/
PUBLIC S16 UzLiSztErrInd (
  Pst            *post,
  SuId            suId,
  SztLclErrInd   *sztLclErrInd
 )
{

  /* Local Error Indication is an error.
   * printing it as logError instead of logApi so that it is always printed.
   */
#ifdef RRM_PRL_FLAG
  if(islogApiEnabled())
   {
     Buff *buff = GET_UEHCB_PRNT_LIB_BUFF();
     printUiSztErrInd(sztLclErrInd, buff);
     logAPI("RRM - S1AP: %s", buff->startAddr);
     buff->numBytesWritten = 0;
   }
#endif
  logError("SztLclErrInd (suId=%d) ignored. Not handled yet", suId);
  /* RETVALUE(UehLiSztErrInd( post, suId, sztLclErrInd )); */
  RETVALUE(RFAILED);
} /* End of UzLiSztErrInd */

/*
*
*       Fun:   UzLiSztUDatInd
*
*       Desc:  Call handler for Connection-less data indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ueh_sz_ptli.c
*       
*/
PUBLIC S16 UzLiSztUDatInd
(
Pst            *pst,
SuId           suId,
SztUDatEvnt    *uDatEvnt
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s","UzLiSztUDatInd");

   
#ifdef RRM_PRL_FLAG
     if (islogApiEnabled())
      { 
        buff = GET_UEHCB_PRNT_LIB_BUFF();
        printUiSztUDatReq(uDatEvnt, buff);
        logAPI("RRM - S1AP: %s", buff->startAddr);
        buff->numBytesWritten = 0;
      } 
   
#endif   
   UehLiSztUDatInd(pst, suId, uDatEvnt);
   UEH_FREEEVNT(uDatEvnt->pdu);
   RETVALUE(ROK);
} /* End of UzLiSztUDatInd */

/*
*
*       Fun:   UzLiSztConInd
*
*       Desc:  Call handler for Connection Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ueh_sz_ptli.c
*       
*/
PUBLIC S16 UzLiSztConInd
(
Pst            *pst,
SuId           suId,
SztConInd      *conInd
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s","UzLiSztConInd");
   
#ifdef RRM_PRL_FLAG
   if (islogApiEnabled())
    { 
       buff = GET_UEHCB_PRNT_LIB_BUFF();
       printUiSztConReq(conInd , buff);
       logAPI("RRM - S1AP: %s", buff->startAddr);
       buff->numBytesWritten = 0;
    } 
   
#endif  
   UehLiSztConInd(pst, suId, conInd); 
   UEH_FREEEVNT(conInd->pdu);
   RETVALUE(ROK);
} /* End of UzLiSztConInd */

/*
*
*       Fun:   UzLiSztConCfm
*
*       Desc:  Call handler for Connection Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ueh_sz_ptli.c
*       
*/
PUBLIC S16 UzLiSztConCfm
(
Pst            *pst,
SuId           suId,
SztConCfm      *conCfm
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s","UzLiSztConCfm");

   
#ifdef RRM_PRL_FLAG
   if (islogApiEnabled())
    { 
      buff = GET_UEHCB_PRNT_LIB_BUFF();
      printUiSztConRsp(conCfm , buff);
      logAPI("RRM - S1AP: %s", buff->startAddr);
      buff->numBytesWritten = 0;
    } 
 
#endif  
   UehLiSztConCfm(pst, suId, conCfm);
   UEH_FREEEVNT(conCfm->pdu);
   RETVALUE(ROK);
} /* End of UzLiSztConCfm */

/*
*
*       Fun:   UzLiSztRelInd
*
*       Desc:  Call handler for Release Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ueh_sz_ptli.c
*       
*/
PUBLIC S16 UzLiSztRelInd
(
Pst            *pst,
SuId           suId,
SztRelInd      *relInd
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s","UzLiSztRelInd");
   
   
#ifdef RRM_PRL_FLAG
  if (islogApiEnabled())
   { 
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printUiSztRelInd(relInd, buff);
     logAPI("RRM - S1AP: %s", buff->startAddr);
     buff->numBytesWritten = 0;
  } 
   
#endif  
   UehLiSztRelInd(pst, suId, relInd); 
   UEH_FREEEVNT(relInd->pdu);
   RETVALUE(ROK);
} /* End of UzLiSztRelInd */

/*
*
*       Fun:   UzLiSztRelCfm
*
*       Desc:  Call handler for Release Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ueh_sz_ptli.c
*       
*/
PUBLIC S16 UzLiSztRelCfm
(
Pst            *pst,
SuId           suId,
SztRelCfm      *relCfm
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s","UzLiSztRelCfm");
   
   
#ifdef RRM_PRL_FLAG
  if (islogApiEnabled())
   { 
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printUiSztRelCfm(relCfm, buff);
     logAPI("RRM - S1AP: %s", buff->startAddr);
     buff->numBytesWritten = 0;
   } 
   
#endif   
   UehLiSztRelCfm(pst, suId, relCfm);
   if (relCfm->relType != SZT_LOCAL_REL)
   {
   UEH_FREEEVNT(relCfm->pdu);
   }
   RETVALUE(ROK);
} /* End of UzLiSztRelCfm */

/*
*
*       Fun:   UzLiSztDatInd
*
*       Desc:  Call handler for Connection based data indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ueh_sz_ptli.c
*       
*/
PUBLIC S16 UzLiSztDatInd
(
Pst            *pst,
SuId           suId,
SztDatEvntInd  *datEvnt
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s","UzLiSztDatInd");
   
   
#ifdef RRM_PRL_FLAG
   if (islogApiEnabled())
    { 
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printUiSztDatInd(datEvnt, buff);
     logAPI("RRM - S1AP: %s", buff->startAddr);
     buff->numBytesWritten = 0;
    } 
   
#endif 
   UehLiSztDatInd(pst, suId, datEvnt);  
   UEH_FREEEVNT(datEvnt->pdu);
   RETVALUE(ROK);
} /* End of UzLiSztDatInd */

/*
*
*       Fun:   UzLiSztEncDecCfm
*
*       Desc:  Call handler for Encode/Decode Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ueh_sz_ptli.c
*       
*/
PUBLIC S16 UzLiSztEncDecCfm
(
Pst            *pst,
SuId           suId,
SztEncDecEvnt  *sztEncDecEvnt,
SztReqStatus   *status
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s","UzLiSztEncDecCfm");
  
#ifdef RRM_PRL_FLAG
    if (islogApiEnabled())
   { 
       buff = GET_UEHCB_PRNT_LIB_BUFF();
       printUiSztEncDecCfm(sztEncDecEvnt, status, buff);
       logAPI("RRM - S1AP: %s", buff->startAddr);
       buff->numBytesWritten = 0;
    } 
   
#endif
    UehLiSztEncDecCfm(pst, suId, sztEncDecEvnt, status);
   if (SZT_REQ_TYPE_DECODE EQ sztEncDecEvnt->type.val || SZT_REQ_TYPE_DECODE_IE EQ sztEncDecEvnt->type.val)
   {
    UEH_FREEEVNT(sztEncDecEvnt->u.pdu);
   } 
   else if (SZT_REQ_TYPE_ENCODE EQ sztEncDecEvnt->type.val || SZT_REQ_TYPE_ENCODE_IE EQ sztEncDecEvnt->type.val )
   {
    UEH_FREEMBUF(sztEncDecEvnt->u.mBuf);
   }
   RETVALUE(ROK);
} /* End of UzLiSztEncDecCfm */

#if (defined(SZ_MME) && defined(SZ_DYNAMIC_PEER))
/*
*
*       Fun:   UzLiSztPeerAssocInd
*
*       Desc:  Call handler for Peer Assoc Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ueh_sz_ptli.c
*       
*/
PUBLIC S16 UzLiSztPeerAssocInd
(
Pst              *pst,
SuId             suId,
SztPeerAssocInd  *peerAssocInd
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s","UzLiSztPeerAssocInd");
   
#ifdef RRM_PRL_FLAG
  if (islogApiEnabled())
  { 
    buff = GET_UEHCB_PRNT_LIB_BUFF();
    printSztPeerAssocInd(peerAssocInd, buff);
    logAPI("RRM - S1AP: %s", buff->startAddr);
    buff->numBytesWritten = 0;
  } 
#endif
   RETVALUE(ROK);
} /* End of UzLiSztPeerAssocInd */
#endif
