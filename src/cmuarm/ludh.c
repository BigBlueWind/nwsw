/*
 * ludh.c
 *
 *  Created on: 31-May-2011
 *      Author: mkata
 */

#include "ludh.h"

/* Packing function for UEH Bind Request */
PUBLIC S16 cmPkAndSendUehUdhBndReq
(
  Pst       *pst,
  SpId       spId,
  SuId       suId
)
{
  Buffer *mBuf = NULLP;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
  {
    logError("cmPkAndSendUehUdhBndReq: no mem for Packing Bind Request pst->region = %d, pst->pool = %d ",pst->region, pst->pool);
    RETVALUE(RFAILED);
  }
  CMCHKPK(SPkS16, spId,mBuf);
  CMCHKPK(SPkS16, suId,mBuf);
  pst->event = (Event) EVT_UEH_UDH_BND_REQ;
  RETVALUE(SPstTsk(pst,mBuf));
}

/*  Unpacking function for UEH Bind Request */
PUBLIC S16 cmUnpkUehUdhBndReq
(
  ProcUehUdhBndRabReq  func,
  Pst                  *pst,
  Buffer               *mBuf
)
{
  SpId    spId;
  SuId    suId;

  if (SUnpkS16(&suId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("%s","cmUnpkUehUdhBndReq: Failed while unpacking in suId \n");
    RETVALUE(RFAILED);
  }
  if (SUnpkS16(&spId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("%s","cmUnpkUehUdhBndReq: Failed while unpacking in spId \n");
    RETVALUE(RFAILED);
  }

  SPutMsg(mBuf);
  /* call the function after unpacking */
  RETVALUE( (*func)(pst, suId, spId));
}

/*
 *    Fun  :    cmPkAndSendSmUdhSapCfg
 *    Desc :    pack the structure UdhLSapCfg
 *    Ret  :    ROK  -ok
 *    File :    udh_func.c
 */
PUBLIC S16 cmPkAndSendSmUdhSapCfg
(
  Pst       *pst,
  CmSapCfg  *sapCfg
)
{
  Buffer *mBuf = NULLP;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
  {
    logError("cmPkAndSendSmUdhSapCfg: no mem for Packing Bind Confirm, pst->region = %d, pst->pool = %d ",pst->region, pst->pool);
    RETVALUE(RFAILED);
  }

  CMCHKPK(SPkU16,sapCfg->pst.intfVer,mBuf);
  CMCHKPK(SPkU8, sapCfg->pst.selector,mBuf);
  CMCHKPK(SPkU8, sapCfg->pst.pool,mBuf);
  CMCHKPK(SPkU8, sapCfg->pst.region,mBuf);
  CMCHKPK(SPkU8, sapCfg->pst.event,mBuf);
  CMCHKPK(SPkU8, sapCfg->pst.route,mBuf);

  CMCHKPK(SPkU8, sapCfg->pst.prior,mBuf);
  CMCHKPK(SPkU8, sapCfg->pst.srcInst,mBuf);
  CMCHKPK(SPkU8, sapCfg->pst.srcEnt,mBuf);
  CMCHKPK(SPkU8, sapCfg->pst.dstInst,mBuf);
  CMCHKPK(SPkU8, sapCfg->pst.dstEnt,mBuf);

  CMCHKPK(SPkU16, sapCfg->pst.srcProcId,mBuf);
  CMCHKPK(SPkU16, sapCfg->pst.dstProcId,mBuf);

  CMCHKPK(SPkS16, sapCfg->spId,mBuf);
  CMCHKPK(SPkS16, sapCfg->suId,mBuf);

  CMCHKPK(SPkU32, sapCfg->sapId,mBuf);

  pst->event = (Event) EVT_SM_UDH_SAP_CFG_REQ;
  RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkAndSendSmUdhSapCfg*/

/*
 *    Fun :    cmUnpkSmUdhSapCfg
 *    Desc:    unpack the structure CmSapCfg
 *    Ret :    ROK  -ok
 *    File:    udh_func.c
 */
PUBLIC S16 cmUnpkSmUdhSapCfg
(
  CmSapCfg  *sapCfg,
  Buffer    *mBuf
)
{
  CMCHKUNPK(SUnpkU32,(U32 *)&sapCfg->sapId,mBuf);
  CMCHKUNPK(SUnpkS16, &sapCfg->suId,mBuf);
  CMCHKUNPK(SUnpkS16, &sapCfg->spId,mBuf);
  CMCHKUNPK(SUnpkU16, &sapCfg->pst.dstProcId,mBuf);
  CMCHKUNPK(SUnpkU16, &sapCfg->pst.srcProcId,mBuf);

  CMCHKUNPK(SUnpkU8, &sapCfg->pst.dstEnt,mBuf);
  CMCHKUNPK(SUnpkU8, &sapCfg->pst.dstInst,mBuf);
  CMCHKUNPK(SUnpkU8, &sapCfg->pst.srcEnt,mBuf);
  CMCHKUNPK(SUnpkU8, &sapCfg->pst.srcInst,mBuf);
  CMCHKUNPK(SUnpkU8, &sapCfg->pst.prior,mBuf);

  CMCHKUNPK(SUnpkU8, &sapCfg->pst.route,mBuf);
  CMCHKUNPK(SUnpkU8, &sapCfg->pst.event,mBuf);
  CMCHKUNPK(SUnpkU8, &sapCfg->pst.region,mBuf);
  CMCHKUNPK(SUnpkU8, &sapCfg->pst.pool,mBuf);
  CMCHKUNPK(SUnpkU8, &sapCfg->pst.selector,mBuf);
  CMCHKUNPK(SUnpkU16,&sapCfg->pst.intfVer,mBuf);

  (Void)SPutMsg(mBuf);
  RETVALUE(ROK);
} /*end of function cmUnpkSmUdhSapCfg*/


/* Packing Function for Sap Config Confirm */
PUBLIC S16 udhPkAndSendSmUdhSapCfgCfm
(
  Pst*  pst,
  SpId  spId,
  U8    status
)
{
  Buffer *mBuf = NULLP;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
  {
    logError("udhPkAndSendSmUdhSapCfgCfm: no mem for Packing Config Confirm pst->region = %d, pst->pool = %d ",pst->region, pst->pool);
    RETVALUE(RFAILED);
  }
  CMCHKPK(SPkS16, spId, mBuf);
  if (SPkU8(status, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("udhPkAndSendSmUdhSapCfgCfm: Packing of Status = %d Failed \n", status);
    RETVALUE(RFAILED);
  }
  pst->event = (Event) EVT_UDH_SM_SAP_CFG_CFM;
  RETVALUE(SPstTsk(pst,mBuf));
}

/* Unpacking Function for Sap Config Confirm */
PUBLIC S16 udhUnpkSmUdhSapCfgCfm
(
  UdhSmSapCfgCfm  func,
  Pst             *pst,
  Buffer          *mBuf
)
{
  SpId  spId;
  U8    status;

  SUnpkU8(&status, mBuf);
  CMCHKUNPK(SUnpkS16, &spId,mBuf);

  SPutMsg(mBuf);
  /* call the function after unpacking */
  RETVALUE( (*func)(pst, spId, status));
}

/*  Unpacking function for UEH Config Confirm */
PUBLIC S16 cmUnpkUdhUehCfgCfm
(
  Pst       *pst,
  Buffer    *mBuf
)
{
  SuId suId;
  U8   status;

  if (SUnpkS16(&suId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("%s","cmUnpkUdhUehCfgCfm: Failed while unpacking in suId \n");
    RETVALUE(RFAILED);
  }
  if (SUnpkU8(&status, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("%s","cmUnpkUdhUehCfgCfm: Failed while unpacking in spId \n");
    RETVALUE(RFAILED);
  }
  SPutMsg(mBuf);
  RETVALUE(ROK);
}

/* Pack and Unpack Functions for SM-UDH Bind Request */
PUBLIC S16 cmPkAndSendSmUdhBndReq
(
  Pst           *pst,
  CmSapBindReq  *udhGtpSapBindReq
)
{
  Buffer *mBuf = NULLP;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
  {
    logError("cmPkAndSendSmUdhBndReq: no mem for Packing UDH Bind Request pst->region = %d, pst->pool = %d ",pst->region, pst->pool);
    RETVALUE(RFAILED);
  }
  CMCHKPK(SPkU32, udhGtpSapBindReq->sapId,mBuf);
  pst->event = (Event) EVT_SM_UDH_SAP_BIND_REQ;
  RETVALUE(SPstTsk(pst,mBuf));
}

/* UnPack SM - UDH Bind Request */
PUBLIC S16 cmUnpkSmUdhBndReq
(
  Buffer        *mBuf,
  CmSapBindReq  *udhGtpSapBindReq
)
{
  CMCHKUNPK(SUnpkU32,(U32 *) &udhGtpSapBindReq->sapId,mBuf);
  SPutMsg(mBuf);
  RETVALUE(ROK);
}

/* Pack and post function for UDH -UEH Bind Confirm */
PUBLIC S16 cmPkAndSendUdhUehBndCfm
(
  Pst*  pst,
  SuId  suId,
  U8    status
)
{
  Buffer *mBuf = NULLP;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
  {
    logError("cmPkAndSendUdhUehBndCfm: no mem for Packing Bind Confirm pst->region = %d, pst->pool = %d ",pst->region, pst->pool);
    RETVALUE(RFAILED);
  }
  if (SPkU8(status, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmPkAndSendUdhUehBndCfm: Packing of Status = %d Failed \n", status);
    RETVALUE(RFAILED);
  }
  if (SPkS16(suId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmPkAndSendUdhUehBndCfm: Packing of suId = %d Failed \n", suId);
    RETVALUE(RFAILED);
  }
  pst->event = (Event) EVT_UDH_UEH_BND_CFM;
  RETVALUE(SPstTsk(pst,mBuf));
}

PUBLIC S16 cmUnpkUdhUehBndCfm
(
  UehUdhBndCfm          func,
  Pst                  *pst,
  Buffer               *mBuf
)
{
  U8     status;
  SuId   suId;

  if (SUnpkS16(&suId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("%s","cmUnpkUdhUehBndCfm: Failed while unpacking in suId \n");
    RETVALUE(RFAILED);
  }
  if (SUnpkU8(&status, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("%s","cmUnpkUdhUehBndCfm: Failed while unpacking in spId \n");
    RETVALUE(RFAILED);
  }

  SPutMsg(mBuf);

  RETVALUE((*func)(pst, suId, status));
}


/* Pack and post function for UDH -SM Bind Confirm */
PUBLIC S16 cmPkAndSendUdhSmBndCfm
(
  Pst           *pst,
  CmSapBindCfm  *cmSapBindCfm,
  U8             status
)
{
  Buffer *mBuf = NULLP;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
  {
    logError("cmPkAndSendUdhSmBndCfm: no mem for Packing Bind Confirm pst->region = %d, pst->pool = %d ",pst->region, pst->pool);
    RETVALUE(RFAILED);
  }

  CMCHKPK(SPkU8, status, mBuf);
  CMCHKPK(SPkU32, cmSapBindCfm->sapId,mBuf);

  pst->event = (Event) EVT_UDH_SM_SAP_BIND_CFM;
  RETVALUE(SPstTsk(pst,mBuf));
}

/* Un Pack function for UDH - SM Bnd CFM */
PUBLIC S16 cmUnpkUdhSmBndCfm
(
  UdhSmBndCfm  func,
  Pst          *pst,
  Buffer       *mBuf
)
{
  U8           status;
  CmSapBindCfm cmSapBindCfm;

  CMCHKUNPK(SUnpkU32,(U32 *) &cmSapBindCfm.sapId, mBuf);
  CMCHKUNPK(SUnpkU8, &status, mBuf);

  SPutMsg(mBuf);
  /* call the function after unpacking */
  RETVALUE((*func)(pst, &cmSapBindCfm, &status));
}


/* Pack and Unpack functions for Add RAB Request */
/* Pack Function */
PUBLIC S16 cmPkAddRabReqMsg
(
  UehUdhAddRabReq  *uehUdhAddRabReq,
  Buffer           *mBuf
)
{
  CMCHKPK(SPkU32, uehUdhAddRabReq->locTeId, mBuf);
  CMCHKPK(SPkU32, uehUdhAddRabReq->remTeid, mBuf);
  CMCHKPK(SPkU32, uehUdhAddRabReq->qci, mBuf);
  CMCHKPK(SPkU32, uehUdhAddRabReq->rabId, mBuf);
  CMCHKPK(cmPkCmTptAddr, &uehUdhAddRabReq->srcIpAddr, mBuf);
  CMCHKPK(cmPkCmTptAddr, &uehUdhAddRabReq->dstIpAddr, mBuf);
  CMCHKPK(cmPkCmTptAddr, &uehUdhAddRabReq->frmDstIpAddr, mBuf);
  cmPkLtePdcpId(&uehUdhAddRabReq->pdcpId ,mBuf);
  RETVALUE(ROK);
}
/* Pack and send UehUdhAddRabReq */
PUBLIC S16 cmPkAndSendUehUdhAddRabReq
(
  Pst*             pst,
  SuId             suId,
  UehUdhAddRabReq  *uehUdhAddRabReq
)
{
  Buffer *mBuf = NULLP;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
  {
    logError("cmPkAndSendUehUdhAddRabReq: no mem for Packing Add Rab Request pst->region = %d, pst->pool = %d ",pst->region, pst->pool);
    RETVALUE(RFAILED);
  }
  if (SPkS16(suId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmPkAndSendUehUdhAddRabReq: Packing of suId = %d Failed \n", suId);
    RETVALUE(RFAILED);
  }
  if (cmPkAddRabReqMsg(uehUdhAddRabReq, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("%s","cmPkAndSendUehUdhAddRabReq: Packing of cmPkUehUdhAddRabReqMsg is Failed \n");
    RETVALUE(RFAILED);
  }

  pst->event = (Event) EVT_UEH_UDH_ADD_RAB_REQ;
  RETVALUE(SPstTsk(pst,mBuf));
}

PUBLIC S16 cmUnpkAddRabReqMsg
(
  UehUdhAddRabReq  *uehUdhAddRabReq,
  Buffer           *mBuf
)
{
  cmUnpkLtePdcpId(&uehUdhAddRabReq->pdcpId ,mBuf);
  CMCHKUNPK(cmUnpkCmTptAddr, &uehUdhAddRabReq->frmDstIpAddr, mBuf);
  CMCHKUNPK(cmUnpkCmTptAddr, &uehUdhAddRabReq->dstIpAddr, mBuf);
  CMCHKUNPK(cmUnpkCmTptAddr, &uehUdhAddRabReq->srcIpAddr, mBuf);
  CMCHKPK(SUnpkU32, &uehUdhAddRabReq->rabId, mBuf);
  CMCHKPK(SUnpkU32, &uehUdhAddRabReq->qci, mBuf);
  CMCHKUNPK(SUnpkU32, &uehUdhAddRabReq->remTeid, mBuf);
  CMCHKUNPK(SUnpkU32, &uehUdhAddRabReq->locTeId, mBuf);
  RETVALUE(ROK);
}

/* Un Pack function for UehUdhAddRabReq */
PUBLIC S16 cmUnpkUehUdhAddRabReq
(
  ProcUehUdhAddRabReq  func,
  Pst                  *pst,
  Buffer               *mBuf
)
{
  SuId             suId;
  UehUdhAddRabReq  uehUdhAddRabReq;

  clearBytes(&uehUdhAddRabReq , sizeof(UehUdhAddRabReq));
  if (cmUnpkAddRabReqMsg(&uehUdhAddRabReq, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("%s","cmUnpkUehUdhAddRabReq: Failed while unpacking of AddRabReq \n");
    RETVALUE(RFAILED);
  }
  if (SUnpkS16(&suId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmUnpkUehUdhAddRabReq: Failed while unpacking suId = %d\n", suId);
    RETVALUE(RFAILED);
  }
  SPutMsg(mBuf);
  logEventIn(pst, 0, "UEId:%d, rabId:%d, dstTeid:%d, srcTeid:%d", uehUdhAddRabReq.pdcpId.ueId,
             uehUdhAddRabReq.rabId, uehUdhAddRabReq.remTeid, uehUdhAddRabReq.locTeId);
  /* call the function after unpacking */
  RETVALUE( (*func)(pst, suId, &uehUdhAddRabReq));
}

/* Pack and Unpack functions for Delete RAB Request */
/* Pack Function */
PUBLIC S16 cmPkDelRabReqMsg
(
  UehUdhDelRabReq  *uehUdhDelRabReq,
  Buffer           *mBuf
)
{
  cmPkLtePdcpId(&uehUdhDelRabReq->pdcpId ,mBuf);
  CMCHKPK(SPkU32,uehUdhDelRabReq->rabReleaseCause , mBuf);
RETVALUE(ROK);
}
/* Pack and send UehUdhDelRabReq */
PUBLIC S16 cmPkAndSendUehUdhDelRabReq
(
Pst*             pst,
SuId             suId,
UehUdhDelRabReq  *uehUdhDelRabReq
)
{
  Buffer *mBuf = NULLP;

  if(SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
  {
    logError("cmPkAndSendUehUdhDelRabReq: no mem for Packing Delete Rab Request  pst->region = %d, pst->pool = %d ",pst->region, pst->pool);
    RETVALUE(RFAILED);
  }
  if (SPkS16(suId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmPkAndSendUehUdhDelRabReq: Packing of suId = %d Failed \n", suId);
    RETVALUE(RFAILED);
  }
  if (cmPkDelRabReqMsg(uehUdhDelRabReq, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("%s","cmPkAndSendUehUdhDelRabReq: Packing of cmPkUehUdhAddRabReqMsg is Failed \n");
    RETVALUE(RFAILED);
  }

  pst->event = (Event) EVT_UEH_UDH_DEL_RAB_REQ;
  RETVALUE(SPstTsk(pst,mBuf));
}

/* UehUdhDelRabReq structure Unpacking */
PUBLIC S16 cmUnpkDelRabReqMsg
(
  UehUdhDelRabReq  *uehUdhDelRabReq,
  Buffer           *mBuf
)
{
  CMCHKUNPK(SUnpkU32,(U32 *)&uehUdhDelRabReq->rabReleaseCause, mBuf);
  cmUnpkLtePdcpId(&uehUdhDelRabReq->pdcpId ,mBuf);
  RETVALUE(ROK);
}
/* Un Pack function for UehUdhDelRabReq */
PUBLIC S16 cmUnpkUehUdhDelRabReq
(
  ProcUehUdhDelRabReq  func,
  Pst                  *pst,
  Buffer               *mBuf
)
{
  SuId             suId;
  UehUdhDelRabReq  uehUdhDelRabReq;
  clearBytes(&uehUdhDelRabReq, sizeof(UehUdhDelRabReq));

  if(cmUnpkDelRabReqMsg(&uehUdhDelRabReq, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("%s","cmUnpkUehUdhDelRabReq: Failed while unpacking of DelRabReq \n");
    RETVALUE(RFAILED);
  }
  if (SUnpkS16(&suId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmUnpkUehUdhDelRabReq: Failed while unpacking suId = %d\n", suId);
    RETVALUE(RFAILED);
  }
  SPutMsg(mBuf);
  logEventIn(pst, 0, "UEId:%d, rabId:%d, dstTeid:%d, srcTeid:%d rab-rel-cause:%d",
             uehUdhDelRabReq.pdcpId.ueId,uehUdhDelRabReq.pdcpId.rbId,uehUdhDelRabReq.rabReleaseCause);
  /* call the function after unpacking */
  RETVALUE( (*func)(pst, suId, &uehUdhDelRabReq));
}

/* Pack and send UehUdhDelUeReq */
PUBLIC S16 cmPkDelUeReqMsg
(
  UehUdhDelUeReq  *uehUdhDelUeReq,
  Buffer           *mBuf
)
{
  cmPkLteRnti(uehUdhDelUeReq->ueId ,mBuf);
  RETVALUE(ROK);
}
PUBLIC S16 cmPkAndSendUehUdhDelUeReq
(
  Pst*             pst,
  SuId             suId,
  UehUdhDelUeReq  *uehUdhDelUeReq
)
{
  Buffer *mBuf = NULLP;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
  {
    logError("cmPkAndSendUehUdhDelUeReq: no mem for Packing Delete Ue Request  pst->region = %d, pst->pool = %d ",pst->region, pst->pool);
    RETVALUE(RFAILED);
  }
  if (SPkS16(suId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmPkAndSendUehUdhDelUeReq: Packing of suId = %d Failed \n", suId);
    RETVALUE(RFAILED);
  }
  if (cmPkDelUeReqMsg(uehUdhDelUeReq, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("%s","cmPkAndSendUehUdhDelUeReq: Packing Failed \n");
    RETVALUE(RFAILED);
  }

  pst->event = (Event) EVT_UEH_UDH_DEL_UE_REQ;
  RETVALUE(SPstTsk(pst,mBuf));
}


/* cmUnpkDelUeReqMsg structure Unpacking */
PUBLIC S16 cmUnpkDelUeReqMsg
(
  UehUdhDelUeReq  *uehUdhDelUeReq,
  Buffer          *mBuf
)
{
  cmUnpkLteRnti(&uehUdhDelUeReq->ueId ,mBuf);
  RETVALUE(ROK);
}

/* Un Pack function for UehUdhDelUeReq */
PUBLIC S16 cmUnpkUehUdhDelUeReq
(
  ProcUehUdhDelUeReq   func,
  Pst                  *pst,
  Buffer               *mBuf
)
{
  SuId             suId;
  UehUdhDelUeReq  uehUdhDelUeReq;
  clearBytes(&uehUdhDelUeReq, sizeof(UehUdhDelUeReq));

  if(cmUnpkDelUeReqMsg(&uehUdhDelUeReq, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("%s","cmUnpkUehUdhDelUeReq: Failed while unpacking of DelUeReq \n");
    RETVALUE(RFAILED);
  }
  if (SUnpkS16(&suId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmUnpkUehUdhDelUeReq: Failed while unpacking suId = %d\n", suId);
    RETVALUE(RFAILED);
  }
  SPutMsg(mBuf);
  logEventIn(pst, 0, "UEId:%d",uehUdhDelUeReq.ueId);
  /* call the function after unpacking */
  RETVALUE( (*func)(pst, suId, &uehUdhDelUeReq));
}

/***********************************/
/*      Add RAB Success to UEH     */
/***********************************/

PUBLIC S16 cmUnpkAddRabCfm
(
  UehAddRabCfm       func,
  Pst                *pst,
  Buffer             *mBuf
)
{
  UdhUehAddRabCfm  udhUehAddRabCfm;

  cmUnpkLtePdcpId(&udhUehAddRabCfm.pdcpId ,mBuf);
  CMCHKUNPK(SUnpkS16, &udhUehAddRabCfm.status, mBuf);

  SPutMsg(mBuf);
  RETVALUE((*func)(pst, &udhUehAddRabCfm));
}

PUBLIC S16  cmPkAndSendAddRabCfm(
  Pst              *pst,
  UdhUehAddRabCfm  *udhUehAddRabCfm
  )
{
  Buffer *mBuf = NULLP;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
  {
   logError("cmPkAndSendAddRabCfm: no mem for Packing Add RAB Succ  pst->region = %d, pst->pool = %d ",pst->region, pst->pool);
   RETVALUE(RFAILED);
  }

  if (SPkS16(udhUehAddRabCfm->status, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("%s","cmPkAndSendAddRabCfm: Packing of status Failed \n");
    RETVALUE(RFAILED);
  }

  if (cmPkLtePdcpId(&udhUehAddRabCfm->pdcpId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("%s","cmPkAndSendAddRabCfm: Packing of cmPkLtePdcpId Failed \n");
    RETVALUE(RFAILED);
  }
  pst->event = (Event) EVT_UDH_UEH_ADD_RAB_CFM;
  RETVALUE(SPstTsk(pst,mBuf));
}



PUBLIC S16  cmPkAndSendDelRabCfm(
  Pst              *pst,
  UdhUehDelRabCfm  *udhUehDelRabCfm
  )
{
  Buffer *mBuf = NULLP;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
  {
   logError("cmPkAndSendDelRabCfm: no mem for Packing Del RAB Succ  pst->region = %d, pst->pool = %d ",pst->region, pst->pool);
   RETVALUE(RFAILED);
  }
  if (SPkS16(udhUehDelRabCfm->status, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmPkAndSendDelRabCfm: Packing of status = %d Failed \n",udhUehDelRabCfm->status);
    RETVALUE(RFAILED);
  }
  if (cmPkLtePdcpId(&udhUehDelRabCfm->pdcpId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("%s","cmPkAndSendDelRabCfm: Packing of cmPkLtePdcpId Failed \n");
    RETVALUE(RFAILED);
  }
  pst->event = (Event) EVT_UDH_UEH_DEL_RAB_CFM;
  RETVALUE(SPstTsk(pst,mBuf));
}

PUBLIC S16 cmUnpkDelRabCfm
(
  UdhDelRabCfm     func,
  Pst              *pst,
  Buffer           *mBuf
)
{
  UdhUehDelRabCfm  udhUehDelRabCfm;

  cmUnpkLtePdcpId(&udhUehDelRabCfm.pdcpId ,mBuf);
  CMCHKUNPK(SUnpkS16, &udhUehDelRabCfm.status, mBuf);

  SPutMsg(mBuf);
  RETVALUE( (*func)(pst, &udhUehDelRabCfm));
}

PUBLIC S16  cmPkAndSendDelUeCfm(
  Pst              *pst,
  UdhUehDelUeCfm   *udhUehDelUeCfm
  )
{
  Buffer *mBuf = NULLP;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
  {
   logError("cmPkAndSendDelUeCfm: no mem for Packing Del UE Succ pst->region = %d, pst->pool = %d ",pst->region, pst->pool);
   RETVALUE(RFAILED);
  }
  if (SPkS16(udhUehDelUeCfm->status, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmPkAndSendDelUeCfm: Packing of status = %d Failed \n", udhUehDelUeCfm->status);
    RETVALUE(RFAILED);
  }
  if (cmPkLteRnti(udhUehDelUeCfm->ueId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmPkAndSendDelUeCfm: Packing of cmPkLtePdcpId = %d Failed \n", udhUehDelUeCfm->ueId);
    RETVALUE(RFAILED);
  }
  pst->event = (Event) EVT_UDH_UEH_DEL_UE_CFM;
  RETVALUE(SPstTsk(pst,mBuf));
}

PUBLIC S16 cmUnpkDelUeCfm
(
  UdhDelUeCfm     func,
  Pst              *pst,
  Buffer           *mBuf
)
{
  UdhUehDelUeCfm  udhUehDelUeCfm;

  cmUnpkLteRnti(&udhUehDelUeCfm.ueId ,mBuf);
  CMCHKUNPK(SUnpkS16, &udhUehDelUeCfm.status, mBuf);

  SPutMsg(mBuf);
  RETVALUE( (*func)(pst, &udhUehDelUeCfm));
}



/* Process Ueh-Udh Bind Request */



/* Un Pack  UE s Inactivity Detection Ind */
PUBLIC S16 cmUnpkUeInactiveInd
(
  UehUeInactvInd       func,
  Pst                  *pst,
  Buffer               *mBuf
)
{
  U16                   idx = 0;
  S16                   suId = 0;
  UdhUehUeInactiveInd   udhUehInactiveUeInd;

  if (SUnpkU16(&udhUehInactiveUeInd.noofInactiveUes, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmUnpkUeInactiveInd: Failed while unpacking noofInactiveUes = %d\n", udhUehInactiveUeInd.noofInactiveUes);
    RETVALUE(RFAILED);
  }
  for(idx = 0; idx < udhUehInactiveUeInd.noofInactiveUes ; idx++)
  {
    CMCHKUNPK(cmUnpkLteRnti, &udhUehInactiveUeInd.ueId[idx], mBuf);
  }

  if (SUnpkS16(&suId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmUnpkUeInactiveInd: Failed while unpacking suId = %d\n", suId);
    RETVALUE(RFAILED);
  }
  SPutMsg(mBuf);

  RETVALUE((*func)(pst, &udhUehInactiveUeInd));
}

/* Pack and Send UE s Inactivity Detection */
PUBLIC S16  cmPkAndSendInactiveUesInd(
  Pst                 *pst,
  SpId                *spId,
  UdhUehUeInactiveInd *udhUehInactiveUeInd
  )
{
  Buffer *mBuf = NULLP;
  U16 idx = 0;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
  {
   logError("cmPkAndSendInactiveUesInd: no mem for Packing Add RAB Succ pst->region = %d, pst->pool = %d",pst->region, pst->pool);
   RETVALUE(RFAILED);
  }

  if (SPkS16(*spId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmPkAndSendInactiveUesInd: Packing of spId = %d Failed \n", *spId);
    RETVALUE(RFAILED);
  }

  for(idx = 0 ; idx < udhUehInactiveUeInd->noofInactiveUes ; idx++ )
  {
    CMCHKPK(cmPkLteRnti, udhUehInactiveUeInd->ueId[idx], mBuf);
  }

  if (SPkU16(udhUehInactiveUeInd->noofInactiveUes, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmPkAndSendInactiveUesInd: Packing of noofInactiveUes = %d Failed \n", udhUehInactiveUeInd->noofInactiveUes);
    RETVALUE(RFAILED);
  }
  pst->event = (Event) EVT_UDH_UEH_INACTIVE_UES_IND;
  RETVALUE(SPstTsk(pst,mBuf));
}


/* Un Pack UL Inactivity Detection Ind */
PUBLIC S16 cmUnpkUlInactiveInd
(
  UehUlInactvInd       func,
  Pst                  *pst,
  Buffer               *mBuf
)
{
  U16                   idx = 0;
  S16                   suId = 0;
  UdhUehUlInactiveInd   udhUehUlInactiveInd;

  if (SUnpkU16(&udhUehUlInactiveInd.noofInactiveUes, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmUnpkUlInactiveInd: Failed while unpacking noofInactiveUes = %d\n", udhUehUlInactiveInd.noofInactiveUes);
    RETVALUE(RFAILED);
  }
  for(idx = 0; idx < udhUehUlInactiveInd.noofInactiveUes ; idx++)
  {
    CMCHKUNPK(cmUnpkLteRnti, &udhUehUlInactiveInd.ueId[idx], mBuf);
  }

  if (SUnpkS16(&suId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmUnpkUlInactiveInd: Failed while unpacking suId = %d\n", suId);
    RETVALUE(RFAILED);
  }
  SPutMsg(mBuf);

  RETVALUE((*func)(pst, &udhUehUlInactiveInd));
}

/* Pack and Send UE s UL Inactivity Detection */
PUBLIC S16  cmPkAndSendUlInactiveUesInd(
  Pst                 *pst,
  SpId                *spId,
  UdhUehUlInactiveInd *udhUehUlInactiveUeInd
  )
{
  Buffer *mBuf = NULLP;
  U16 idx = 0;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
  {
   logError("cmPkAndSendUlInactiveUesInd: NO Memory for Packing Add RAB Succ pst->region = %d, pst->pool = %d",pst->region, pst->pool);
   RETVALUE(RFAILED);
  }

  if (SPkS16(*spId, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmPkAndSendUlInactiveUesInd: Packing of spId = %d Failed \n", *spId);
    RETVALUE(RFAILED);
  }

  for(idx = 0 ; idx < udhUehUlInactiveUeInd->noofInactiveUes ; idx++ )
  {
    CMCHKPK(cmPkLteRnti, udhUehUlInactiveUeInd->ueId[idx], mBuf);
  }

  if (SPkU16(udhUehUlInactiveUeInd->noofInactiveUes, mBuf) != ROK)
  {
    SPutMsg(mBuf);
    logError("cmPkAndSendUlInactiveUesInd: Packing of noofInactiveUes = %d Failed \n", udhUehUlInactiveUeInd->noofInactiveUes);
    RETVALUE(RFAILED);
  }
  pst->event = (Event) EVT_UDH_UEH_UL_INACTIVE_IND;
  RETVALUE(SPstTsk(pst,mBuf));
}


/* Pack and Unpack Functions for SM-UDH Debug Control Request */
PUBLIC S16 cmPkAndSendSmUdhDbgCntrl
(
  Pst             *pst,
  SmUdhLogConfig  *smUdhLogConfig
)
{
  Buffer *mBuf = NULLP;

  if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
  {
    logError("cmPkAndSendSmUdhBndReq: no mem for Packing UDH Bind Request pst->region = %d, pst->pool = %d ",pst->region, pst->pool);
    RETVALUE(RFAILED);
  }
  CMCHKPK(SPkU32, smUdhLogConfig->logMask1, mBuf);

  pst->event = (Event) EVT_SM_UDH_DBG_CNTRL_REQ;
  RETVALUE(SPstTsk(pst, mBuf));
}

/* UnPack SM - UDH Debug Control */
PUBLIC S16 cmUnPkAndSendSmUdhDbgCntrl
(
  Buffer          *mBuf,
  SmUdhLogConfig  *smUdhLogConfig
)
{
  CMCHKUNPK(SUnpkU32, &smUdhLogConfig->logMask1,mBuf);

  SPutMsg(mBuf);
  RETVALUE(ROK);
}
