/**
 * @file    ueh_nh_ptli.c
 * @brief This file contains NHU SAP interface functions
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
#include "lueh.h"
#include"ueh_eut_x.h"
#include "lueh.x" 
#include "ueh.h" 

#include "log.h"
#ifdef RRM_PRL_FLAG
#include "trace.h"
#endif

/* local defines */
#define MAXUEHLI 3     /* Size of matrix used for LC/LWLC/TC/One for Dummy */

#ifdef UEH
#if (!(defined(LWLCUEHLINHU) && defined(LCNHUINHU)))
#define PTUEHLINHU 1
#else
#ifndef NH
#define PTUEHLINHU 1
#endif
#endif
#endif

#if (PTUEHLINHU)
PRIVATE S16 PtLiNhuBndReq  ARGS(( Pst *pst, SuId suId, SpId spId ));
PRIVATE S16 PtLiNhuUbndReq ARGS(( Pst *pst, SpId spId, Reason reason ));
PRIVATE S16 PtLiNhuDatReq  ARGS(( Pst *pst, SpId spId, NhuDatReqSdus *nhuDatReqSdus ));
PRIVATE S16 PtLiNhuDatRsp  ARGS(( Pst *pst, SpId spId, NhuDatRspSdus *nhuDatRspSdus ));
PRIVATE S16 PtLiNhuCfgReq  ARGS(( Pst *pst, SpId spId, NhuCfgReqSdus *nhuCfgReqSdus ));
PRIVATE S16 PtLiNhuCnclUeReq  ARGS(( Pst *pst, SpId spId, NhuCellId cellId,
                                       NhuCrnti crnti ));
PRIVATE S16 PtLiNhuCellCfgReq  ARGS(( Pst *pst, SpId spId, NhuCellCfgReqSdus *nhuCellCfgReqSdus ));
PRIVATE S16 PtLiNhuDeleteTransReq  ARGS(( Pst *pst, SpId spId, 
                                          NhuCellId cellId, NhuCrnti ueId,      
                                          U32 transId ));
PRIVATE S16 PtLiNhuCountReq ARGS((Pst *pst, SuId suId, NhuHdr *countHdr));
PRIVATE S16 PtLiNhuEncodeReq ARGS((Pst *pst, SuId suId, NhuEncReqSdus *nhuEncReqSdus));

PRIVATE S16 PtLiNhuPdcpSduStaReq ARGS((Pst *pst, SuId    suId, NhuHdr  nhuHdr));
PRIVATE S16 PtLiNhuDecodeReq ARGS(( Pst *pst, SuId suId, NhuDecodeSduReq *nhuDecodeSduReq));

#endif /* PTUEHLINHU */


/* Bind Request Primitive */
PRIVATE NhuBndReq UehLiNhuBndReqMt [MAXUEHLI] =
{
#if (defined(LCUEHLINHU))
   cmPkNhuBndReq,            /* 0 - loosely coupled */
#else
   PtLiNhuBndReq,            /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuBndReq,            /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuBndReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCUEHLINHU))
   cmPkNhuBndReq            /* 1 - light weight loosely coupled */
#else
   PtLiNhuBndReq            /* 1 - light weight loosely coupled,portable */
#endif
};

/* Unbind Request Primitive */
PRIVATE NhuUbndReq UehLiNhuUbndReqMt [MAXUEHLI] =
{
#if (defined(LCUEHLINHU))
   cmPkNhuUbndReq,            /* 0 - loosely coupled */
#else
   PtLiNhuUbndReq,            /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuUbndReq,            /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuUbndReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCUEHLINHU))
   cmPkNhuUbndReq            /* 1 - light weight loosely coupled */
#else
   PtLiNhuUbndReq            /* 1 - light weight loosely coupled,portable */
#endif
};

/* Data Request Primitive */
PRIVATE NhuDatReq UehLiNhuDatReqMt [MAXUEHLI] =
{
#if (defined(LCUEHLINHU))
   cmPkNhuDatReq,            /* 0 - loosely coupled */
#else
   PtLiNhuDatReq,            /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuDatReq,            /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuDatReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCUEHLINHU))
   cmPkNhuDatReq            /* 1 - light weight loosely coupled */
#else
   PtLiNhuDatReq            /* 1 - light weight loosely coupled,portable */
#endif
};

/* Data Response Primitive */ 
PRIVATE NhuDatRsp UehLiNhuDatRspMt [MAXUEHLI] = 
{ 
#if (defined(LCUEHLINHU)) 
   cmPkNhuDatRsp,            /* 0 - loosely coupled */
#else 
   PtLiNhuDatRsp,            /* 0 - loosely coupled,portable */
#endif 
#ifdef NH 
   NhUiNhuDatRsp,            /* 2 - tightly coupled,stub layer */
#else 
   PtLiNhuDatRsp,            /* 2 - tightly coupled,portable */
#endif 
#if (defined(LWLCUEHLINHU)) 
   cmPkNhuDatRsp            /* 1 - light weight loosely coupled */
#else 
   PtLiNhuDatRsp            /* 1 - light weight loosely coupled,portable */
#endif 
};

/* Delete Transaction Request Primitive */
PRIVATE NhuDeleteTransReq UehLiNhuDeleteTransReqMt[MAXUEHLI] =
{
#if (defined(LCUEHLINHU))
   cmPkNhuDeleteTransReq,      /* 0 - loosely coupled */
#else
   PtLiNhuDeleteTransReq,      /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuDeleteTransReq,      /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuDeleteTransReq,      /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCUEHLINHU))
   cmPkNhuDeleteTransReq,      /* 1 - light weight loosely coupled */
#else
   PtLiNhuDeleteTransReq       /* 1 - light weight loosely coupled,portable */
#endif
};

/* Configuration Request Primitive */
PRIVATE NhuCfgReq UehLiNhuCfgReqMt [MAXUEHLI] =
{
#if (defined(LCUEHLINHU))
   cmPkNhuCfgReq,            /* 0 - loosely coupled */
#else
   PtLiNhuCfgReq,            /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuCfgReq,            /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuCfgReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCUEHLINHU))
   cmPkNhuCfgReq            /* 1 - light weight loosely coupled */
#else
   PtLiNhuCfgReq            /* 1 - light weight loosely coupled,portable */
#endif
};

/* Cancel UE Request primitive */
PRIVATE NhuCnclUeReq UehLiNhuCnclUeReqMt [MAXUEHLI] =
{ 
#if (defined(LCUEHLINHU))
   cmPkNhuCnclUeReq,            /* 0 - loosely coupled */
#else 
   PtLiNhuCnclUeReq,            /* 0 - loosely coupled,portable */
#endif 
#ifdef NH
   NhUiNhuCnclUeReq,              /* 2 - tightly coupled,stub layer */
#else 
   PtLiNhuCnclUeReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCUEHLINHU))
   cmPkNhuCnclUeReq            /* 1 - loosely coupled */
#else 
   PtLiNhuCnclUeReq            /* 1 - loosely coupled,portable */
#endif 
};

/* Cell Config Request primitive */
PRIVATE NhuCellCfgReq UehLiNhuCellCfgReqMt [MAXUEHLI] =
{ 
#if (defined(LCUEHLINHU))
   cmPkNhuCellCfgReq,            /* 0 - loosely coupled */
#else 
   PtLiNhuCellCfgReq,            /* 0 - loosely coupled,portable */
#endif 
#ifdef NH
   NhUiNhuCellCfgReq,            /* 2 - tightly coupled,stub layer */
#else 
   PtLiNhuCellCfgReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCUEHLINHU))
   cmPkNhuCellCfgReq            /* 1 - loosely coupled */
#else 
   PtLiNhuCellCfgReq            /* 1 - loosely coupled,portable */
#endif 
};

/* Decode Request Primitive */
PRIVATE NhuDecodeReq UehLiNhuDecodeReqMt [MAXUEHLI] =
{
#if (defined(LCUEHLINHU))
   cmPkNhuDecodeReq,            /* 0 - loosely coupled */
#else
   PtLiNhuDecodeReq,            /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuDecodeReq,            /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuDecodeReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCUEHLINHU))
   cmPkNhuDecodeReq            /* 1 - light weight loosely coupled */
#else
   PtLiNhuDecodeReq            /* 1 - light weight loosely coupled,portable */
#endif
};

/* Count Request Primitive */
PRIVATE NhuCountReq UehLiNhuCountReqMt [MAXUEHLI] =
{
#if (defined(LCUEHLINHU))
   cmPkNhuCountReq,            /* 0 - loosely coupled */
#else
   PtLiNhuCountReq,            /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuCountReq,            /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuCountReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCUEHLINHU))
   cmPkNhuCountReq            /* 1 - light weight loosely coupled */
#else
   PtLiNhuCountReq            /* 1 - light weight loosely coupled,portable */
#endif
};

/* Enocde Request Primitive */
PRIVATE NhuEncReq UehLiNhuEncodeReqMt [MAXUEHLI] =
{
#if (defined(LCUEHLINHU))
   cmPkNhuEncodeReq,            /* 0 - loosely coupled */
#else
   PtLiNhuEncodeReq,            /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuEncodeReq,            /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuEncodeReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCUEHLINHU))
   cmPkNhuEncodeReq            /* 1 - light weight loosely coupled */
#else
   PtLiNhuEncodeReq            /* 1 - light weight loosely coupled,portable */
#endif
};

/* SDU Status Request Primitive */
PRIVATE NhuSduStaReq UehLiNhuPdcpSduStaReqMt [MAXUEHLI] =
{
#if (defined(LCUEHLINHU))
   cmPkNhuPdcpSduStaReq,            /* 0 - loosely coupled */
#else
   PtLiNhuPdcpSduStaReq,            /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuPdcpSduStaReq,            /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuPdcpSduStaReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCUEHLINHU))
   cmPkNhuPdcpSduStaReq            /* 1 - light weight loosely coupled */
#else
   PtLiNhuPdcpSduStaReq            /* 1 - light weight loosely coupled,portable */
#endif
};


#if (PTUEHLINHU)
/**************************************************************************
 *
 *      Fun:   PtLiNhuBndReq
 *
 *      Desc:  Bind Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ueh_nh_ptli.c
 *
 *************************************************************************/

PRIVATE S16 PtLiNhuBndReq
(
Pst    *pst,     /* post structure */
SuId   suId,     /* NHU serivce user SAP ID */ 
SpId   spId      /* NHU SAP ID */
)
{
   logFunc("%s","PtLiNhuBndReq");
   RETVALUE(ROK);
} /* PtLiNhuBndReq */

/**************************************************************************
 * 
 *      Fun:   PtLiNhuUbndReq 
 * 
 *      Desc:  UnBind Request from RRC User to RRC
 * 
 *      Ret:   ROK - ok 
 * 
 *      Notes: None 
 * 
 *      File:  ueh_nh_ptli.c 
 * 
 *************************************************************************/
PRIVATE S16 PtLiNhuUbndReq
(
Pst          *pst,     /* post structure */
SpId         spId,     /* NHU SAP ID */
Reason       reason    /* Reason for Unbind */
)
{ 
   logFunc("%s","PtLiNhuUbndReq");
 
   RETVALUE(ROK); 
} /* PtLiNhuUbndReq */


/**************************************************************************
 *
 *      Fun:   PtLiNhuDatReq
 *
 *      Desc:  Data Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ueh_nh_ptli.c
 *
 *************************************************************************/
PRIVATE S16 PtLiNhuDatReq
(
Pst             *pst,            /* post structure */
SpId            spId,            /* RRC Service provider SAP ID */
NhuDatReqSdus   *nhuDatReqSdus   /* SDU */
)
{
   logFunc("%s","PtLiNhuDatReq");

    UECM_OUTPUT(nhuDatReqSdus->hdr.ueId,UehLiEgtUbndReq,RRC,"UECM PT RRC DATA REQUEST");
   RETVALUE(ROK);

} /* PtLiNhuDatReq */

/**************************************************************************
 *
 *      Fun:   PtLiNhuDatRsp
 *
 *      Desc:  Data Response from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 * 
 *      File:  ueh_nh_ptli.c
 * 
 *************************************************************************/
PRIVATE S16 PtLiNhuDatRsp
(
Pst      *pst, 
SpId     spId,  
NhuDatRspSdus *nhuDatRspSdus
)
{ 
   logFunc("%s","PtLiNhuDatRsp");
    UECM_OUTPUT(nhuDatRspSdus->sdu.m.dlCcchMsg.tmpCrnti,PtLiNhuDatRsp,RRC,"UECM PT RRC DATA RESPONSE");
   RETVALUE(ROK); 
} /* PtLiNhuDatRsp */

/**************************************************************************
 *
 *      Fun:   PtLiNhuDeleteTransReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ueh_nh_ptli.c
 *
 *************************************************************************/
PRIVATE S16 PtLiNhuDeleteTransReq
(
Pst        *pst,       /* Post Structure */
SpId       spId,       /* Service Provider Id */
NhuCellId  cellId,     /* Cell ID */
NhuCrnti   ueId,       /* Ue Id */
U32        transId     /* transaction id */
)
{
   logFunc("%s","PtLiNhuDeleteTransReq");
    UECM_OUTPUT(ueId,PtLiNhuDeleteTransReq,RRC,"UECM PT RRC DELETE TRANS REQUEST");
   RETVALUE(ROK); 
}
 
/**************************************************************************
 *
 *      Fun:   PtLiNhuCfgReq
 *
 *      Desc:  Config Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ueh_nh_ptli.c
 *
 *************************************************************************/
PRIVATE S16 PtLiNhuCfgReq
(
Pst             *pst,
SpId            spId,
NhuCfgReqSdus   *nhuCfgReqSdus
)
{
   logFunc("%s","PtLiNhuCfgReq");
   UECM_OUTPUT(nhuCfgReqSdus->hdr.ueId,PtLiNhuCfgReq,RRC,"UECM PT RRC CONFIGURATION REQUEST");
   RETVALUE(ROK); 
} /* PtLiNhuCfgReq */ 

/**************************************************************************
 *
 *      Fun:   PtLiNhuCnclUeReq
 * 
 *      Desc:  Cancel UE Request from RRC User to RRC
 * 
 *      Ret:   ROK - ok 
 * 
 *      Notes: None 
 * 
 *      File:  ueh_nh_ptli.c 
 * 
 *************************************************************************/
PRIVATE S16 PtLiNhuCnclUeReq
(
Pst         *pst,
SpId        spId,
NhuCellId   cellId,
NhuCrnti    crnti
)
{ 
   logFunc("%s","PtLiNhuCnclUeReq");
   UECM_OUTPUT(crnti,PtLiNhuCnclUeReq,RRC,"UECM PT RRC CANCEL UE REQUEST");
   RETVALUE(ROK); 
 
} /* PtLiNhuCnclUeReq */

/**************************************************************************
 *
 *      Fun:   PtLiNhuCellCfgReq
 * 
 *      Desc:  SCCPCH Config Request from RRC User to RRC
 * 
 *      Ret:   ROK - ok 
 * 
 *      Notes: None 
 * 
 *      File:  ueh_nh_ptli.c 
 * 
 *************************************************************************/
PRIVATE S16 PtLiNhuCellCfgReq
(
Pst *pst,
SpId spId,
NhuCellCfgReqSdus *nhuCellCfgReqSdus
)
{ 
   logFunc("%s","PtLiNhuCellCfgReq");
   UECM_OUTPUT(nhuCellCfgReqSdus->hdr.ueId,PtLiNhuCellCfgReq,RRC,"UECM PT RRC CELL CONFIG REQUEST");
   RETVALUE(ROK); 
} /* PtLiNhuCellCfgReq */


/**************************************************************************
 *
 *      Fun:   PtLiNhuDecodeReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ueh_nh_ptli.c
 *
 *************************************************************************/

PRIVATE S16 PtLiNhuDecodeReq
(
Pst     *pst,
SuId     suId,
NhuDecodeSduReq *nhuDecodeSduReq
)
{
   logFunc("%s","PtLiNhuDecodeReq");
   UECM_OUTPUT(nhuDecodeSduReq->hdr.ueId,PtLiNhuDecodeReq,RRC,"UECM PT RRC DECODE REQUEST");
   RETVALUE(ROK);
}

/**************************************************************************
 *
 *      Fun:   PtLiNhuCountReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ueh_nh_ptli.c
 *
 *************************************************************************/
PRIVATE S16 PtLiNhuCountReq
(
Pst     *pst,
SuId     suId,
NhuHdr *countHdr
)
{
#ifdef RRM_PRL_FLAG 
Buff *buff = NULL;
#endif
   logFunc("%s","PtLiNhuCountReq");
   UECM_OUTPUT(countHdr->ueId,PtLiNhuCountReq,RRC,"UECM PT RRC COUNT REQUEST");
#ifdef RRM_PRL_FLAG  
   if(islogApiEnabled())
    {
      buff = GET_UEHCB_PRNT_LIB_BUFF();
      printNhuCountReq(countHdr, buff);
      logAPI("RRM - RRC :PtLiNhuCountReq:countHdr STRUCTURE %s", buff->startAddr);
      buff->numBytesWritten = 0;
     } 
 #endif
   
   RETVALUE(ROK);
}

/**************************************************************************
 *
 *      Fun:   PtLiNhuEncodeReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ueh_nh_ptli.c
 *
 *************************************************************************/
PRIVATE S16 PtLiNhuEncodeReq
(
Pst     *pst,
SuId     suId,
NhuEncReqSdus *nhuEncReqSdus
)
{
   logFunc("%s","PtLiNhuEncodeReq");
   UECM_OUTPUT(nhuEncReqSdus->hdr.ueId,PtLiNhuEncodeReq,RRC,"UECM PT RRC ENCODE REQUEST");
   RETVALUE(ROK);
}

/**************************************************************************
 *
 *      Fun:   PtLiNhuPdcpSduStaReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ueh_nh_ptli.c
 *
 *************************************************************************/
PRIVATE S16 PtLiNhuPdcpSduStaReq
(
Pst     *pst,
SuId    suId,
NhuHdr  nhuHdr
)
{
   logFunc("%s","PtLiNhuPdcpSduStaReq");
   UECM_OUTPUT(nhuHdr.ueId,PtLiNhuPdcpSduStaReq,RRC,"UECM PT RRC PDCP SDU STA REQUEST");
   RETVALUE(ROK);
}

#endif /* PTUEHLINHU */

/* LI primtives for NHU  */

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

/* 
* 
*       Fun:   lower interface - Bind Request
*  
*       Desc:  This function indicates the receipt of a bind request  
*  
*       Ret:   ROK      - ok
* 
*       Notes: None 
* 
*       File:  
*        
*/
PUBLIC S16 UehLiNhuBndReq
(
Pst    *pst,     /* post structure */
SuId   suId,     /* NHU serivce user SAP ID */ 
SpId   spId      /* NHU SAP ID */
)
{
   logFunc("%s","UehLiNhuBndReq");
   UECM_OUTPUT(spId,UehLiNhuBndReq,RRC,"UECM RRC BIND REQUEST");
   RETVALUE((*UehLiNhuBndReqMt[pst->selector])(pst, suId, spId));
} /* end of UehLiNhuBndReq */


/* 
* 
*       Fun:   lower interface - Unbind Request
*  
*       Desc:  This function indicates the receipt of a bind request  
*  
*       Ret:   ROK      - ok
* 
*       Notes: None 
* 
*       File:  
*        
*/
PUBLIC S16 UehLiNhuUbndReq
(
Pst          *pst,     /* post structure */
SpId         spId,     /* NHU SAP ID */
Reason       reason    /* Reason for Unbind */
)
{
  logFunc("%s","UehLiNhuUbndReq");
  UECM_OUTPUT(spId,UehLiNhuUbndReq,RRC,"UECM RRC UNBIND REQUEST");
   RETVALUE((*UehLiNhuUbndReqMt[pst->selector])(pst, spId, reason));

} /* end of UehLiNhuUbndReq */

/* 
* 
*       Fun:   lower interface - Dat Request
*  
*       Desc:   
*  
*       Ret:   ROK      - ok
* 
*       Notes: None 
* 
*       File:  
*        
*/
PUBLIC S16 UehLiNhuDatReq
(
Pst             *pst,            /* post structure */
SpId            spId,            /* RRC Service provider SAP ID */
NhuDatReqSdus   *nhuDatReqSdus   /* SDU */
)
{
  char details[30];
#ifdef RRM_PRL_FLAG 
Buff *buff = NULL;
#endif
  logFunc("%s","UehLiNhuDatReq");
  
  if(nhuDatReqSdus->sdu.ueMsg.msg.cmnMsg.cmnMsgCategory != NHU_MSG_DLBCCHBCH &&
      nhuDatReqSdus->sdu.ueMsg.msg.cmnMsg.cmnMsgCategory != NHU_MSG_DLBCCHDLSCH) 
  {
    if(nhuDatReqSdus->sdu.ueMsg.msg.dedMsg.dlDcchMsg.isMsgEncoded)
    {
      sprintf(details,"MsgType = %d",nhuDatReqSdus->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg.message.val.c1.choice.val);
    }
    else
    {
      sprintf(details,"Msg is encoded");
    }
    UECM_OUTPUT(nhuDatReqSdus->hdr.ueId,UehLiNhuDatReq,RRC,details);
  }
 
#ifdef RRM_PRL_FLAG  
  if(islogApiEnabled())
   {  
     /* Do not print MIB and SIBs going to RRC as they clutter the log */
     TBOOL printIt = TRUE;
     if(nhuDatReqSdus->sdu.ueMsg.isMsgDed EQ FALSE)
     {
       if ( (nhuDatReqSdus->sdu.ueMsg.msg.cmnMsg.cmnMsgCategory EQ NHU_MSG_DLBCCHDLSCH) ||
            (nhuDatReqSdus->sdu.ueMsg.msg.cmnMsg.cmnMsgCategory EQ NHU_MSG_DLBCCHBCH))
         printIt = FALSE;
     }
     if(printIt) 
     {
       buff = GET_UEHCB_PRNT_LIB_BUFF();
       printNhUiNhuDatReq(nhuDatReqSdus, buff);
       logAPI("RRM - RRC :UehLiNhuDatReq: %s", buff->startAddr);
       buff->numBytesWritten = 0;
     }
   }
#endif
  RETVALUE((*UehLiNhuDatReqMt[pst->selector])(pst, spId, nhuDatReqSdus));
   
} /* end of function UehLiNhuDatReq */

/* 
* 
*       Fun:   lower interface - Dat Response
*  
*       Desc:    
*  
*       Ret:   ROK      - ok
* 
*       Notes: None 
* 
*       File:  
*        
*/
PUBLIC S16 UehLiNhuDatRsp
(
Pst      *pst, 
SpId     spId,  
NhuDatRspSdus *nhuDatRspSdus
)
{
  char details[30];
#ifdef RRM_PRL_FLAG 
 Buff *buff = NULL;
#endif
 
  logFunc("%s","UehLiNhuDatRsp");

  if(nhuDatRspSdus->sdu.msgCategory == NHU_MSG_DLCCCH)
  {
    sprintf(details,"MsgType=%d",nhuDatRspSdus->sdu.m.dlCcchMsg.ccchMsg.message.val.c1.choice.val);
  }
  else
  {
    sprintf(details,"MsgType=%d",nhuDatRspSdus->sdu.m.dlDcchMsg.m.dcchMsg.message.val.c1.choice.val);
  }
  UECM_OUTPUT(nhuDatRspSdus->sdu.m.dlCcchMsg.tmpCrnti,UehLiNhuDatRsp,RRC,details);
#ifdef RRM_PRL_FLAG  
  if(islogApiEnabled()) 
   {  
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhuUiNhuDatRsp(nhuDatRspSdus,buff);
     logAPI("RRM - RRC :UEH NhuDatRsp: %s", buff->startAddr);
     buff->numBytesWritten = 0;
   }
#endif
  RETVALUE((*UehLiNhuDatRspMt[pst->selector])(pst, spId, nhuDatRspSdus));

} /* end of function NhUiNhuDatRsp */


/* 
* 
*       Fun:   lower interface - Configuration Request
*  
*       Desc:   
*  
*       Ret:   ROK      - ok
* 
*       Notes: None 
* 
*       File:  
*        
*/
PUBLIC S16 UehLiNhuCfgReq
(
Pst             *pst,
SpId            spId,
NhuCfgReqSdus   *nhuCfgReqSdus
)
{
  logFunc("%s","UehLiNhuCfgReq");
  UECM_OUTPUT(nhuCfgReqSdus->cfgInfo.ueInfo.newCrnti,UehLiNhuCfgReq,RRC,"RRC CONFIG REQUEST");
  RETVALUE((*UehLiNhuCfgReqMt[pst->selector])(pst, spId, nhuCfgReqSdus));

} /* end of function UehLiNhuCfgReq */

/* 
* 
*       Fun:   lower interface - Cancel UE Request
*  
*       Desc:   
*  
*       Ret:   ROK      - ok
* 
*       Notes: None 
* 
*       File:  
*        
*/
PUBLIC S16 UehLiNhuCnclUeReq
(
Pst         *pst,
SpId        spId,
NhuCellId   cellId,
NhuCrnti    crnti
)
{
#ifdef RRM_PRL_FLAG 
 Buff *buff = NULL;
#endif
  UECM_OUTPUT(crnti,UehLiNhuCnclUeReq,RRC,"UECM RRC UE CANCEL REQUEST");
  logInfo("sending NHU Cancel UeId:%d",crnti);
#ifdef RRM_PRL_FLAG  
  if(islogApiEnabled())
   { 
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhUiNhuCnclUeReq ( cellId, crnti, buff);
     logAPI("RRM - RRC:UECM RRC UE CANCEL REQUEST %s", buff->startAddr); 
     buff->numBytesWritten = 0;
   }
#endif
  RETVALUE((*UehLiNhuCnclUeReqMt[pst->selector])(pst, spId, cellId, crnti));

} /* end of function UehLiNhuCnclUeReq */

/*
*
*       Fun:   lower interface - Cell Config Request
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:
*
*/
PUBLIC S16 UehLiNhuCellCfgReq
(
Pst *pst,
SpId spId,
NhuCellCfgReqSdus *nhuCellCfgReqSdus
)
{
#ifdef RRM_PRL_FLAG 
   Buff *buff = NULL;
#endif
  logFunc("%s","UehLiNhuCellCfgReq");
  UECM_OUTPUT(nhuCellCfgReqSdus->hdr.ueId,UehLiNhuCellCfgReq,RRC,"UECM RRC CELL CONFIG REQUEST");
  
#ifdef RRM_PRL_FLAG 
  if(islogApiEnabled())
   {  
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhuCellCfgReqSdus(nhuCellCfgReqSdus, buff);
     logAPI("RRM - RRC:UECM RRC CELL CONFIG REQUEST %s", buff->startAddr); 
     buff->numBytesWritten = 0;
   }
#endif
  RETVALUE((*UehLiNhuCellCfgReqMt[pst->selector])(pst, spId, nhuCellCfgReqSdus));

} /* end of function UehLiNhuCellCfgReq */

/**************************************************************************
 *
 *      Fun:   UehLiNhuDeleteTransReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ueh_nh_ptli.c
 *
 *************************************************************************/
PUBLIC S16 UehLiNhuDeleteTransReq
(
Pst        *pst,       /* Post Structure */
SpId       spId,       /* Service Provider Id */
NhuCellId  cellId,     /* Cell ID */
NhuCrnti   ueId,       /* Ue Id */
U32        transId     /* transaction id */
)
{
#ifdef RRM_PRL_FLAG 
   Buff *buff = NULL;
#endif
  logFunc("%s","UehLiNhuDeleteTransReq");
  UECM_OUTPUT(ueId,UehLiNhuDeleteTransReq,RRC,"UECM RRC DELETE TRANS REQUEST");
#ifdef RRM_PRL_FLAG 
 if(islogApiEnabled())
  {
    buff = GET_UEHCB_PRNT_LIB_BUFF();
    printNhuUiNhuDeleteTransReq(cellId, ueId, transId, buff);
    logAPI("RRM_Print_Library_RRMtoRRC:UehLiNhuPdcpSduStaReq:nhuHdr STRUCTURE: %s", buff->startAddr);
    buff->numBytesWritten = 0;
  }
#endif
  RETVALUE((*UehLiNhuDeleteTransReqMt[pst->selector])(pst, spId, cellId, ueId, transId)); 
}


/**************************************************************************
 *
 *      Fun:   UehLiNhuPdcpSduStaReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ueh_nh_ptli.c
 *
 *************************************************************************/
PUBLIC S16 UehLiNhuPdcpSduStaReq
(
Pst     *pst,
SuId    suId,
NhuHdr  nhuHdr
)
{
#ifdef RRM_PRL_FLAG 
Buff *buff = NULL;
#endif
  logFunc("%s","UehLiNhuPdcpSduStaReq");
  UECM_OUTPUT(nhuHdr.ueId,UehLiNhuPdcpSduStaReq,RRC,"UECM RRC PDCP STATUS REQUEST");
#ifdef RRM_PRL_FLAG  
  if(islogApiEnabled())
   {   
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhuPdcpSduStaReq(&nhuHdr, buff);
     logAPI("RRM - RRC :UehLiNhuPdcpSduStaReq:nhuHdr STRUCTURE: %s", buff->startAddr);
     buff->numBytesWritten = 0;
   }
#endif  
  RETVALUE((*UehLiNhuPdcpSduStaReqMt[pst->selector])(pst, suId, nhuHdr)); 
}


/**************************************************************************
 *
 *      Fun:   UehLiNhuDecodeReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ueh_nh_ptli.c
 *
 *************************************************************************/

PUBLIC S16 UehLiNhuDecodeReq
(
Pst     *pst,
SuId     suId,
NhuDecodeSduReq *nhuDecodeSduReq
)
{
#ifdef RRM_PRL_FLAG 
Buff *buff = NULL;
#endif
  logFunc("%s","UehLiNhuDecodeReq");
  UECM_OUTPUT(nhuDecodeSduReq->hdr.ueId,UehLiNhuDecodeReq,RRC,"UECM RRC DECODE REQUEST");
#ifdef RRM_PRL_FLAG  
   if(islogApiEnabled())
    {
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhuDecodeReq(nhuDecodeSduReq, buff);
     logAPI("RRM - RRC :UehLiNhuDecodeReq:nhuDecodeSduReq STRUCTURE: %s", buff->startAddr);
     buff->numBytesWritten = 0;
    }
#endif  
  RETVALUE((*UehLiNhuDecodeReqMt[pst->selector])(pst, suId, nhuDecodeSduReq));
}

/**************************************************************************
 *
 *      Fun:   UehLiNhuCountReq
 *
 *      Desc:  Count Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ueh_nh_ptli.c
 *
 *************************************************************************/
PUBLIC S16 UehLiNhuCountReq
(
Pst     *pst,
SuId     suId,
NhuHdr *countHdr
)
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif 
  logFunc("%s","UehLiNhuCountReq");
  UECM_OUTPUT(countHdr->ueId,UehLiNhuCountReq,RRC,"UECM RRC COUNT REQUEST");
#ifdef RRM_PRL_FLAG  
  if(islogApiEnabled())
   {  
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhuCountReq(countHdr, buff);
     logAPI("RRM - RRC :UehLiNhuCountReq:countHdr STRUCTURE %s", buff->startAddr);
     buff->numBytesWritten = 0;
   }
#endif
  RETVALUE((*UehLiNhuCountReqMt[pst->selector])(pst, suId, countHdr));
}

/**************************************************************************
 *
 *      Fun:   UehLiNhuEncodeReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ueh_nh_ptli.c
 *
 *************************************************************************/

PUBLIC S16 UehLiNhuEncodeReq
(
Pst     *pst,
SuId     suId,
NhuEncReqSdus *nhuEncReqSdus
)
{
#ifdef RRM_PRL_FLAG 
 Buff *buff = NULL;
#endif
  logFunc("%s","UehLiNhuEncodeReq");
  UECM_OUTPUT(nhuEncReqSdus->hdr.ueId,UehLiNhuEncodeReq,RRC,"UECM RRC ENCODE REQUEST");
#ifdef RRM_PRL_FLAG 
  if(islogApiEnabled())
   {
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhuEncReqSdus(nhuEncReqSdus, buff);
     logAPI("RRM - RRC :UehLiNhuEncodeReq:STRUCTURE %s", buff->startAddr);  
     buff->numBytesWritten = 0;
   }
#endif
  RETVALUE((*UehLiNhuEncodeReqMt[pst->selector])(pst, suId, nhuEncReqSdus));
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

/**********************************************************************

        Revision history:

**********************************************************************/
