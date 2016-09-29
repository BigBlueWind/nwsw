/**
 * @file    ueh_pj_ptli.c
 * @brief This file contains PJU SAP interface functions
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. @author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 16Sep2010 :
*     a) Created initial version from sample application
 */

/* header (.h) include files */

#include "ccpu_cmn.h"

/* RRM related includes */
#include"ueh_eut.h"
#include "lueh.h"
#include"ueh_eut_x.h"
#include "lueh.x" 
#include "ueh.h" 

#ifdef KW_PDCP

#if (!defined(LCPJUIPJU) || !defined(PJ))
#define PTNXLIPJU
#endif 

#ifdef PTNXLIPJU
PRIVATE S16 PtLiPjuBndReq ARGS (( Pst* pst, SpId spId, SuId suId));
PRIVATE S16 PtLiPjuUbndReq ARGS (( Pst* pst, SpId spId, Reason reason));
PRIVATE S16 PtLiPjuDatReq ARGS (( Pst* pst, SpId spId, CmLtePdcpId * pdcpId, PjuSduId sduId, Buffer * mBuf));
PRIVATE S16 PtLiPjuDatFwdReq ARGS (( Pst* pst, SpId spId, CmLtePdcpId * pdcpId, PjuDatFwdReqInfo * datFwdReq));
#endif 



/************************************************************************
                             PJU Interface Matrices
************************************************************************/
PRIVATE CONSTANT PjuBndReq UehLiPjuBndReqMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuBndReq,
#else
   PtLiPjuBndReq,
#endif
#ifdef PJ
   PjUiPjuBndReq,
#else
   PtLiPjuBndReq,
#endif
};


PRIVATE CONSTANT PjuUbndReq UehLiPjuUbndReqMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuUbndReq,
#else
   PtLiPjuUbndReq,
#endif
#ifdef PJ
   PjUiPjuUbndReq,
#else
   PtLiPjuUbndReq,
#endif
};


PRIVATE CONSTANT PjuDatReq UehLiPjuDatReqMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuDatReq,
#else
   PtLiPjuDatReq,
#endif
#ifdef PJ
   PjUiPjuDatReq,
#else
   PtLiPjuDatReq,
#endif
};


PRIVATE CONSTANT PjuDatFwdReq UehLiPjuDatFwdReqMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuDatFwdReq,
#else
   PtLiPjuDatFwdReq,
#endif
#ifdef PJ
   PjUiPjuDatFwdReq,
#else
   PtLiPjuDatFwdReq,
#endif
};

#ifdef UEH
PUBLIC S16 UehLiPjuBndReq
(
Pst* pst,
SuId suId,
SpId spId
);

PUBLIC S16 UehLiPjuUbndReq
(
Pst* pst,
SpId spId,
Reason reason
);

PUBLIC S16 UehLiPjuDatFwdReq
(
Pst* pst,
SpId spId,
CmLtePdcpId * pdcpId,
PjuDatFwdReqInfo * datFwdReq
);

PUBLIC S16 UehLiPjuDatReq
(
Pst* pst,
SpId spId,
CmLtePdcpId *pdcpId,
PjuSduId sduId,
Buffer *mBuf
);
#endif


/****************************************************************************
 *                         PJU Interface Mt functions
 ***************************************************************************/
#ifdef UEH
/*
*
*       Fun:   UehLiPjuBndReq 
*
*       Desc:  Bind Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_pj_ptli.c 
*
*/
PUBLIC S16 UehLiPjuBndReq
(
Pst* pst,
SuId suId,
SpId spId
)
{

   logFunc("%s","UehLiPjuBndReq");

   (*UehLiPjuBndReqMt[pst->selector])(pst, suId, spId);

   RETVALUE(ROK);

}

/*
*
*       Fun:   UehLiPjuUbndReq 
*
*       Desc:  Unbind Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_pj_ptli.c 
*
*/
PUBLIC S16 UehLiPjuUbndReq
(
Pst* pst,
SpId spId,
Reason reason
)
{

   logFunc("%s","UehLiPjuUbndReq");

   (*UehLiPjuUbndReqMt[pst->selector])(pst, spId, reason);

   RETVALUE(ROK);

}

/*
*
*       Fun:   UehLiPjuDatReq 
*
*       Desc:  Data Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_pj_ptli.c 
*
*/
PUBLIC S16 UehLiPjuDatReq
(
Pst* pst,
SpId spId,
CmLtePdcpId *pdcpId,
PjuSduId sduId,
Buffer *mBuf
)
{
    logFunc("%s","UehLiPjuDatReq");
#ifdef DATA_PATH_DEBUGP
    /*UECM_OUTPUT(pdcpId->ueId,UehLiPjuDatReq,PDCP ,"UECM PDCP  DATA REQUEST");*/
    logInfo ("%s","UECM PDCP  DATA REQUEST");
#endif
   (*UehLiPjuDatReqMt[pst->selector])(pst, spId, pdcpId, sduId, mBuf);

   RETVALUE(ROK);

}

/*
*
*       Fun:   UehLiPjuDatFwdReq 
*
*       Desc:  Data Forward Request 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_pj_ptli.c 
*
*/
PUBLIC S16 UehLiPjuDatFwdReq
(
Pst* pst,
SpId spId,
CmLtePdcpId * pdcpId,
PjuDatFwdReqInfo * datFwdReq
)
{

   logFunc("%s","UehLiPjuDatFwdReq");
   UECM_OUTPUT(pdcpId->ueId,UehLiPjuDatFwdReq,PDCP ,"UECM PDCP DATA FORWARD REQUEST");
   (*UehLiPjuDatFwdReqMt[pst->selector])(pst, spId, pdcpId, datFwdReq);

   RETVALUE(ROK);

}
#endif /*--ifdef NX--*/

  
/****************************************************************************
 *                         Porting Functions
 ***************************************************************************/
#ifdef PTNXLIPJU
/*
*
*       Fun:   PtLiPjuBndReq 
*
*       Desc:  Portable Bind Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_pj_ptli.c 
*
*/
PUBLIC S16 PtLiPjuBndReq
(
Pst* pst,
SpId spId,
SuId suId
)
{

   logFunc("%s","PtLiPjuBndReq");

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(suId);

   RETVALUE(ROK);
}

/*
*
*       Fun:   PtLiPjuUbndReq 
*
*       Desc:  Portable Unbind Request 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_pj_ptli.c 
*
*/
PUBLIC S16 PtLiPjuUbndReq
(
Pst* pst,
SpId spId,
Reason reason
)
{

   logFunc("%s","PtLiPjuUbndReq");

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(reason);

   RETVALUE(ROK);
}

/*
*
*       Fun:   PtLiPjuDatReq 
*
*       Desc:  Portable Data Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_pj_ptli.c 
*
*/
PUBLIC S16 PtLiPjuDatReq
(
Pst* pst,
SpId spId,
CmLtePdcpId * pdcpId,
PjuSduId sduId,
Buffer * mBuf
)
{

   logFunc("%s","PtLiPjuDatReq");
   UECM_OUTPUT(pdcpId->ueId,PtLiPjuDatReq,PDCP ,"PT PDCP DATA REQUEST");
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(pdcpId);
   UNUSED(sduId);
   UNUSED(mBuf);

   RETVALUE(ROK);
}

/*
*
*       Fun:   PtLiPjuDatFwdReq 
*
*       Desc:  Portable Data Fwd Req.
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_pj_ptli.c 
*
*/

PUBLIC S16 PtLiPjuDatFwdReq
(
Pst* pst,
SpId spId,
CmLtePdcpId * pdcpId,
PjuDatFwdReqInfo * datFwdReq
)
{

   logFunc("%s","PtLiPjuDatFwdReq");
   UECM_OUTPUT(pdcpId->ueId,PtLiPjuDatFwdReq,PDCP ,"PT PDCP DATA FORWARD REQUEST");
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(pdcpId);
   UNUSED(datFwdReq);

   RETVALUE(ROK);
}
#endif /*--ifdef PTNXLIPJU--*/

#endif /* KW_PDCP */

/**********************************************************************
  
        Revision history:
  
**********************************************************************/
