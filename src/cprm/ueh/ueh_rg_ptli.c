/**
 * @file    ueh_rg_ptli.c
 * @brief This file contains RGR SAP interface functions
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 16Sep2010 :
*     a) Created initial version from sample application
 */

#include "ccpu_cmn.h"

/* RRM related includes */
#include"ueh_eut.h"
#include "ueh.h" 
#include "lueh.h"
#include"ueh_eut_x.h"
#include "lueh.x"  
#include "log.h"

/* MAX Number of Service Providers of UEH */
#define UEH_MAX_RGR_PROV   3

#if (!defined(LCUEHLIRGR) || !defined(LWLCUEHLIRGR) || !defined(RG))
#define PTUEHLIRGR
#endif

#ifdef PTUEHLIRGR
/** @brief Request from RRM to MAC to bind the interface saps */
PRIVATE S16 PtLiRgrBndReq ARGS((Pst* pst, SuId suId, SpId spId));
/** @brief Request from RRM to MAC to Unbind the interface saps */
PRIVATE S16 PtLiRgrUbndReq ARGS((Pst* pst, SpId spId, Reason reason));
/** @brief Configuration Request from RRM to MAC for 
 * configuring Cell/Ue/Lc */
PRIVATE S16 PtLiRgrCfgReq ARGS((Pst* pst, SpId spId, RgrCfgTransId transId, RgrCfgReqInfo * cfgReqInfo));
#ifdef SI_NEW
PRIVATE S16 PtLiRgrSiCfgReq ARGS((Pst* pst, SpId spId, RgrCfgTransId transId, RgrSiCfgReqInfo * cfgReqInfo));
#endif
#endif /*--#ifdef PTUEHLIRGR--*/

/** @brief Request from RRM to MAC to bind the interface saps */
PRIVATE  RgrBndReq UehLiRgrBndReqMt[UEH_MAX_RGR_PROV] =
{
#ifdef LCUEHLIRGR
   cmPkRgrBndReq,
#else
   PtLiRgrBndReq,
#endif
#ifdef RG
   RgUiRgrBndReq,
#else
   PtLiRgrBndReq,
#endif
#ifdef LWLCUEHLIRGR
   cmPkRgrBndReq
#else
   PtLiRgrBndReq
#endif
};

/** @brief Request from RRM to MAC to Unbind the interface saps */
PRIVATE  RgrUbndReq UehLiRgrUbndReqMt[UEH_MAX_RGR_PROV] =
{
#ifdef LCUEHLIRGR
   cmPkRgrUbndReq,
#else
   PtLiRgrUbndReq,
#endif
#ifdef RG
   RgUiRgrUbndReq,
#else
   PtLiRgrUbndReq,
#endif
#ifdef LWLCUEHLIRGR
   cmPkRgrUbndReq
#else
   PtLiRgrUbndReq
#endif
};

/** @brief Configuration Request from RRM to MAC for 
 * configuring Cell/Ue/Lc */
PRIVATE  RgrCfgReq UehLiRgrCfgReqMt[UEH_MAX_RGR_PROV] =
{
#ifdef LCUEHLIRGR
   cmPkRgrCfgReq,
#else
   PtLiRgrCfgReq,
#endif
#ifdef RG
   RgUiRgrCfgReq,
#else
   PtLiRgrCfgReq,
#endif
#ifdef LWLCUEHLIRGR
   cmPkRgrCfgReq
#else
   PtLiRgrCfgReq
#endif
};

#ifdef SI_NEW
/** @brief Configuration Request from RRM to MAC for
 * configuring SI */
PRIVATE  RgrSiCfgReq uehLiRgrSiCfgReqMt[UEH_MAX_RGR_PROV] =
{
#ifdef LCUEHLIRGR
   cmPkRgrSiCfgReq,
#else
   PtLiRgrSiCfgReq,
#endif
#ifdef RG
   RgUiRgrSiCfgReq,
#else
   PtLiRgrSiCfgReq,
#endif
#ifdef LWLCUEHLIRGR
   cmPkRgrSiCfgReq
#else
   PtLiRgrSiCfgReq
#endif
};
#endif

#ifdef UEH
PUBLIC S16 UehLiRgrUbndReq
(
Pst* pst,
SpId spId,
Reason reason
);
/*
*
*       Fun:   UehLiRgrBndReq 
*
*       Desc:  Bnd Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_rg_ptli.c  
*
*/
PUBLIC S16 UehLiRgrBndReq
(
Pst* pst,
SuId suId,
SpId spId
)
{
   logFunc("%s","UehLiRgrBndReq");

   (*UehLiRgrBndReqMt[pst->selector])(pst, suId, spId);

   RETVALUE(ROK);

}

/*
*
*       Fun:   UehLiRgrUbndReq 
*
*       Desc:  Unbind Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_rg_ptli.c  
*
*/
PUBLIC S16 UehLiRgrUbndReq
(
Pst* pst,
SpId spId,
Reason reason
)
{

   logFunc("%s","UehLiRgrUbndReq");

   (*UehLiRgrUbndReqMt[pst->selector])(pst, spId, reason);

   RETVALUE(ROK);

}

/*
*
*       Fun:   UehLiRgrCfgReq 
*
*       Desc:  Cfg Req.
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_rg_ptli.c  
*
*/
PUBLIC S16 UehLiRgrCfgReq
(
Pst* pst,
SpId spId,
RgrCfgTransId transId,
RgrCfgReqInfo * cfgReqInfo
)
{
   int ueId=-1;
   char details[50] = "-";
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif

   logFunc("%s","UehLiRgrCfgReq");

   switch (cfgReqInfo->action)  {
     case RGR_CONFIG:
     case RGR_RECONFIG:
      if( RGR_UE_CFG   EQ cfgReqInfo->u.cfgInfo.cfgType || 
          RGR_LCH_CFG  EQ cfgReqInfo->u.cfgInfo.cfgType ||
          RGR_LCG_CFG  EQ cfgReqInfo->u.cfgInfo.cfgType )
        ueId = cfgReqInfo->u.cfgInfo.u.ueCfg.crnti;
   }
   sprintf(details,"cfgType = %d", cfgReqInfo->u.cfgInfo.cfgType);

   UECM_OUTPUT(cfgReqInfo->u.cfgInfo.u.ueCfg.crnti,UehLiRgrCfgReq,SCHED ,details);

#ifdef RRM_PRL_FLAG
 if(islogApiEnabled())
 {
   buff = GET_UEHCB_PRNT_LIB_BUFF(); 
   printRgrCfgReq(cfgReqInfo,buff);
   logAPI("RRM - SCH : %s", buff->startAddr);
   buff->numBytesWritten = 0;
 } 
#endif   
   (*UehLiRgrCfgReqMt[pst->selector])(pst, spId, transId, cfgReqInfo);

   RETVALUE(ROK);

}

#ifdef SI_NEW
/*
*
*       Fun:   uehLiRgrSiCfgReq
*
*       Desc:  SI Cfg Req.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_rg_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 uehLiRgrSiCfgReq
(
Pst* pst,
SpId spId,
RgrCfgTransId transId,
RgrSiCfgReqInfo * cfgReqInfo
)
#else
PUBLIC S16 uehLiRgrSiCfgReq(pst, spId, transId, cfgReqInfo)
Pst* pst;
SpId spId;
RgrCfgTransId transId;
RgrSiCfgReqInfo * cfgReqInfo;
#endif
{

#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
  logFunc("%s","uehLiRgrSiCfgReq");

#ifdef RRM_PRL_FLAG
 if(islogApiEnabled())
  { 
   buff = GET_UEHCB_PRNT_LIB_BUFF(); 
   printRgrSiCfgReqInfo(cfgReqInfo, buff);
   logAPI("RRM - SCH : %s", buff->startAddr);
   buff->numBytesWritten = 0;
  }
#endif   
  (*uehLiRgrSiCfgReqMt[pst->selector])(pst, spId, transId, cfgReqInfo);

  RETVALUE(ROK);

}
#endif


#endif /*--ifdef UEH--*/

#ifdef PTUEHLIRGR

/*
*
*       Fun:   PtLiRgrBndReq 
*
*       Desc:  Portable Bind Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_rg_ptli.c  
*
*/
PUBLIC S16 PtLiRgrBndReq
(
Pst* pst,
SuId suId,
SpId spId
)
{

   logFunc("%s","PtLiRgrBndReq");

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);

   RETVALUE(ROK);

}
/*
*
*       Fun:   PtLiRgrUbndReq 
*
*       Desc:  Portable Unbind Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_rg_ptli.c  
*
*/
PUBLIC S16 PtLiRgrUbndReq
(
Pst* pst,
SpId spId,
Reason reason
)
{

   logFunc("%s","PtLiRgrUbndReq");

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(reason);

   RETVALUE(ROK);

}

/*
*
*       Fun:   PtLiRgrCfgReq 
*
*       Desc:  Portable Cfg Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_rg_ptli.c  
*
*/

PUBLIC S16 PtLiRgrCfgReq
(
Pst* pst,
SpId spId,
RgrCfgTransId transId,
RgrCfgReqInfo * cfgReqInfo
)
{

   logFunc("%s","PtLiRgrCfgReq");
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(transId);
   UNUSED(cfgReqInfo);

   RETVALUE(ROK);

}

#ifdef SI_NEW
/*
*
*       Fun:   PtLiRgrSiCfgReq
*
*       Desc:  Portable Si Cfg Req
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_rg_ptli.c
*
*/
#ifdef ANSI
PRIVATE S16 PtLiRgrSiCfgReq
(
Pst* pst,
SpId spId,
RgrCfgTransId transId,
RgrSiCfgReqInfo * cfgReqInfo
)
#else
PRIVATE S16 PtLiRgrSiCfgReq(pst, spId, transId, cfgReqInfo)
Pst* pst;
SpId spId;
RgrCfgTransId transId;
RgrSiCfgReqInfo * cfgReqInfo;
#endif
{

   logFunc("%s","PtLiRgrSiCfgReq");

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(transId);
   UNUSED(cfgReqInfo);

   RETVALUE(ROK);

}
#endif


#endif /*--ifdef PTUEHLIRGR--*/

/**********************************************************************
  
        Revision history:
  
**********************************************************************/
