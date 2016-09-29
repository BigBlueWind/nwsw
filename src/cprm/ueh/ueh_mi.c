/**
 * @file    ueh_mi.c
 * @brief This file contains the management interface functions for User Equipment Handler (UEH) module
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
*     a) Created initial version from sample application
 */

/**********************************************************************

     Name:     LTE-RRC User
  
     Type:     C Source file
  
     Desc:     RRC user Management Interface handlers

     Ret :     ROK - success
               RFAILED - failure

     File:    ueh_mi.c

**********************************************************************/

#include "ccpu_cmn.h"

/* RRM related includes */
#include"ueh_eut.h"
#include "ueh.h" 
#include "lueh.h"
#include"ueh_eut_x.h"
#include "ueh_crm_sm.h"
#include "lueh.x" 
#include <signal.h>
#include "ueh_crm.h"
#include "log.h"

/*
 *
 *       Fun:     uehSendLmAlarm
 *
 *       Desc:    Function trigger alarm
 *
 *       Ret:     ROK   - ok
 *
 *       Notes :  None
 *
 *       File:    ueh_mi.c
 *
 */
PUBLIC Void uehSendLmAlarm(U16 category, U16 event, U16 cause)
{
   UehMngmt usta;        /* Unsolicited Status Indication structure */

   logFunc("%s","uehSendLmAlarm");

   if (FALSE EQ uehCb.init.usta)
   {
      RETVOID;
   }

   /* Initialize the management structure */
   (Void)cmMemset((U8 *) &usta, '\0', (PTR)sizeof(UehMngmt));

   usta.hdr.elmId.elmnt = STUEHGEN;
   usta.hdr.entId.ent   = uehCb.init.ent;
   usta.hdr.entId.inst  = uehCb.init.inst;

   usta.u.usta.alarm.category = category;
   usta.u.usta.alarm.event    = event;
   usta.u.usta.alarm.cause    = cause;
   /* Send an unsolicited status indication */
   UehMiLuehStaInd(&uehCb.init.lmPst, &usta);

   RETVOID;

} /* uehSendLmAlarm */

/*
 *
 *       Fun:     uehActvInit
 *
 *       Desc:    
 *
 *       Ret:     ROK   - ok
 *
 *       Notes :  The task initializes its global variables.
 *
 *       File:    ueh_mi.c
 *
 */

PUBLIC S16 uehActvInit
(
Ent    entity,    /* Entity   */
Inst   inst,      /* Instance */
Region region,    /* Region   */
Reason reason     /* reason   */
)
{
   logFunc("%s","uehActvInit");

   uehCb.init.procId  = SFndProcId();
   uehCb.init.ent     = entity;
   uehCb.init.inst    = inst;
   uehCb.init.region  = region;
   uehCb.init.reason  = reason;
   uehCb.init.cfgDone = FALSE;
   uehCb.init.pool    = DFLT_POOL;
   uehCb.init.acnt    = FALSE;
   uehCb.init.trc     = FALSE; 
   uehCb.init.usta    = TRUE;

#ifdef DEBUGP
   uehCb.init.dbgMask = 0xffffffff;
#endif /* end of DEBUGP */

   uehCb.systemOperational = 0; /*False, dont send system information*/
   uehCb.mem.region = DFLT_REGION;
   uehCb.mem.pool = DFLT_POOL;
   
   cmMemset((U8*)&(uehCb.uehCfgCb), 0, sizeof(UehCfgCb));
   cmMemset((U8*)&(uehCb.modiUehCfgCb), 0, sizeof(UehCfgCb));
#ifdef RRM_PRL_FLAG
   cmMemset((U8*)&(uehCb.buff), 0, sizeof(Buff));
   uehCb.buff.buffCapacity = RRM_PRL_MAX_BUFF; /* allow 0 to buffCapacity -1 index */
   uehCb.buff.startAddr    = &(uehCb.buff.data[0]);
   //UEH_ALLOC(&uehCb.buff.startAddr, RRM_PRL_MAX_BUFF); 
#endif
   uehCb.cfgValid = 0;

#ifdef WIRESHARK_LOG
   uehInitWireShark();
#endif
   uehCb.lastRcvdSfn = -1;
   RETVALUE(ROK);
} /* uehActvInit */

/*
 *
 *       Fun:     uehGenerateCfm 
 *
 *       Desc:    To generate confirms
 *
 *       Ret:     ROK   - ok
 *
 *       Notes :  None.
 *
 *       File:    ueh_mi.c
 *
 */
#ifdef ANSI
PUBLIC S16  uehGenerateCfm(
  Pst                     *pst,
  Header                  *hdr,
  U8                      prim,
  UehMngmt                 *cfm
  )
#else
PUBLIC S16 uehGenerateCfm (pst, hdr, prim, cfm)
Pst                     *pst;
Header                  *hdr;
U8                      prim;
UehMngmt                 *cfm;
#endif
{
   Pst                  cPst;

   logFunc("%s","uehGenerateCfm");

   /* Fill the pst structure for LM i/f */
   cPst.srcEnt    = uehCb.init.ent;
   cPst.srcInst   = uehCb.init.inst;
   cPst.srcProcId = SFndProcId();

   /*
    * If genconfig is done, fill the destn identity
    * from the pst configured, else use the incoming
    * pst structure for destination identity.
    */
   if (TRUE EQ uehCb.init.cfgDone)
   {  
      cPst.dstEnt    = uehCb.init.lmPst.dstEnt;
      cPst.dstInst   = uehCb.init.lmPst.dstInst;
      cPst.dstProcId = uehCb.init.lmPst.dstProcId;
   }
   else
   {  
      cPst.dstEnt    = pst->srcEnt;
      cPst.dstInst   = pst->srcInst;
      cPst.dstProcId = pst->srcProcId;
   }

   cPst.selector  = hdr->response.selector;
   cPst.prior     = hdr->response.prior;
   cPst.route     = hdr->response.route;
   cPst.region    = hdr->response.mem.region;
   cPst.pool      = hdr->response.mem.pool;

   /* Fill the Header in cfm structure */
   cmMemset ((U8 *)&cfm->hdr, '\0', sizeof(Header));
   cfm->hdr.elmId.elmnt = hdr->elmId.elmnt;
   cfm->hdr.entId.ent   = uehCb.init.ent;
   cfm->hdr.entId.inst  = uehCb.init.inst;

   /* Fill the transaction for which the cfm is being sent */
   cfm->hdr.transId = hdr->transId;

   switch (prim)
   {  
      case TCFG:
         cmMemset ((U8 *)&cfm->u.cfg, 0, sizeof(UehCfg));
         UehMiLuehCfgCfm (&cPst, cfm);
         break;   

      case TCNTRL:
         UehMiLuehCntrlCfm (&cPst, cfm);
         break;
   }  

   RETVALUE(ROK);
}


/********************************************************************IB**
 Layer Management Incoming Primitives
*********************************************************************IB*/

/*
 *
 *       Fun:     uehUbndLSap
 *
 *       Desc:    This function is invoked to unbind & disable a SAP 
 *                A disconnect is sent and the SAP state is changed
 *                to UEH_SAP_CFG without sending a unbind request.
 *
 *       Ret:     ROK or LCM_REASON_INVALID_STATE
 *
 *       Notes :  This function provides run time support for the
 *                stack reinitialization procedure.
 *
 *       File:    ueh_mi.c
 *
 */
#ifdef ANSI
PRIVATE S16 uehUbndLSap
(
UehLiSapCb *sapCb   /* Lower SAP control block */
)
#else
PRIVATE S16 uehUbndLSap(sapCb)
UehLiSapCb *sapCb;   /* Lower SAP control block */
#endif
{
   S16       retVal;

   logFunc("%s","uehUbndLSap");

   switch(sapCb->sapState)
   {
      case LUEH_SAP_BINDING:
         sapCb->sapState = LUEH_SAP_UNBOUND;
         retVal = ROK;
         break;

      case LUEH_SAP_BOUND:
         sapCb->sapState = LUEH_SAP_UNBOUND;
         retVal = ROK;
         break;

      default:
         UEH_ERROR_ELMNT_OUT_OF_RANGE("Receiver State in call to nuUbndLSap", sapCb->sapState, LUEH_SAP_BINDING, LUEH_SAP_BOUND);
         retVal = LCM_REASON_INVALID_STATE;
         break;
   }

   RETVALUE(retVal);
} /* uehUbndLSap */

/*
 *
 *       Fun:     uehBndLSap
 *
 *       Desc:    This function binds the lower sap.
 *
 *       Ret:     ROK   - ok
 *
 *       Notes :  This function is a state transition action handler
 *                for lower SAP ABND_ENA control request. 
 *
 *       File:    ueh_mi.c
 *
 */
#ifdef ANSI
PRIVATE S16 uehBndLSap
(
UehLiSapCb               *sapCb,
CmStatus                *status,
Elmnt                   elmnt
)
#else
PRIVATE S16 uehBndLSap(sapCb, status, elmnt)
UehLiSapCb               *sapCb;
CmStatus                *status;
Elmnt                   elmnt;
#endif
{
   logFunc("%s","uehBndLSap");
   S16  ret = RFAILED;

   if (LUEH_SAP_UNBOUND EQ sapCb->sapState)
   {
      status->status = LCM_PRIM_OK_NDONE;
      sapCb->sapState = LUEH_SAP_BINDING;
      sapCb->bndRetryCnt = 0;

      switch(elmnt)
      {
         case STUEHNHUSAP:
            ret = uehSndMsgOut (ENTNH, EVENTTYPE_RRCBNDREQ, NULLP, TRANSID_NONE, NULLP);
            break;
         case STUEHRGRSAP:
            ret = uehSndMsgOut (ENTRG, EVENTTYPE_SCHBNDREQ, NULLP, TRANSID_NONE,NULLP);
            break;
 #ifndef RRC_PHY_CFG_SUPP
         case STUEHCTFSAP:
            ret = uehSndMsgOut (ENTTF, EVENTTYPE_PHYBNDREQ, NULLP,TRANSID_NONE,NULLP);
            break;
#endif
         case STUEHSZTSAP:
            ret = uehSndMsgOut (ENTSZ, EVENTTYPE_S1BNDREQ, NULLP, TRANSID_NONE, NULLP);
            break;
         case STUEHUDHSAP:
            ret = uehSndMsgOut (ENTUDH, EVENTTYPE_UDHBNDREQ, NULLP,TRANSID_NONE,NULLP);
            break;
         default:
            break;
      }
   }
   else
   {
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_INVALID_STATE;
   }

   RETVALUE (ret);
} /* uehBndLSap */

/*
 *
 *       Fun:     uehLSapCntrl
 *
 *       Desc:    This function processes control request for the
 *                Lower SAP.
 *
 *       Ret:     ROK - Ok/RFAILED - Failure 
 *
 *       Notes :  This function implements the State transition
 *                for Lower SAP control requests. The function
 *                is the entry point for all control request
 *                events defined.
 *
 *       File:    ueh_mi.c
 *
 */
#ifdef ANSI
PRIVATE S16 uehLSapCntrl(
    UehCntrl                 *sapCntrl,
    CmStatus                *status,
    Elmnt                   elmnt
  )
#else
PRIVATE S16 uehLSapCntrl(sapCntrl, status, elmnt)
UehCntrl                 *sapCntrl;
CmStatus                *status;
Elmnt                   elmnt;
#endif
{
   UehLiSapCb            *lSapCb = NULLP;
   SuId                 lSapId;
   S16                  retVal;

   retVal = ROK;

   logFunc("%s","uehLSapCntrl");

   /* Validate the parameters */
   lSapId = sapCntrl->u.sapId;

   switch (elmnt) {
      case STUEHRGRSAP:
         lSapCb = uehCb.rgrSap[0];
         break;
      case STUEHNHUSAP:
         lSapCb = uehCb.nhuSap[0];
         break;
      case STUEHCTFSAP:
         lSapCb = uehCb.ctfSap[0];
         break;
      case STUEHSZTSAP:
         lSapCb = uehCb.sztSap[0];
         break;
      case STUEHUDHSAP:
         lSapCb = uehCb.udhSap[0];
         break;

      default:
         UEH_ERROR_ELMNT_OUT_OF_RANGE("SAP Type", elmnt, 
           STUEHRGRSAP, STUEHUDHSAP);
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_INVALID_PAR_VAL;
         RETVALUE(RFAILED);
   }

   /* Initialise the status members to success */
   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   switch (sapCntrl->action)
   {
      case ABND_ENA:
         /* Bind Lower Sap  */
         uehBndLSap(lSapCb, status, elmnt);
         retVal = ROK;
         break;

      case AUBND_DIS:
         /* Disconnect PVC and DONT send Unbind Request */
         retVal = uehUbndLSap(lSapCb);
         if (ROK != retVal)
         {
            status->status = LCM_PRIM_NOK;
            status->reason = retVal;
         }
         break;

      default:
         UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE("Action Value", sapCntrl->action, "ABND_ENA(21)/AUBND_DIS(31)");
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_INVALID_ACTION;
         retVal = RFAILED;
         break;
   }

   RETVALUE(retVal);
}


/*
 *
 *       Fun:     uehShutdown
 *
 *       Desc:    
 *
 *       Ret:     None. 
 *
 *       Notes :  This function is invoked on a control request
 *                to shutdown the layer. All the data structures
 *                allocated during configuration are freed.
 *
 *       File:    ueh_mi.c
 *
 */
#ifdef ANSI
PRIVATE Void uehShutdown
(
Void
)
#else
PRIVATE Void uehShutdown()
#endif
{
   logInfo("%s", "uehShutdown: eNodeB Layer Shutdown");

#ifdef RRM_PRL_FLAG
   //UEH_FREE(uehCb.buff.startAddr, RRM_PRL_MAX_BUFF);
#endif
   /* Reintialize the layer - the reason parameter is not used */
   uehActvInit(uehCb.init.ent, uehCb.init.inst, uehCb.init.region, 
             uehCb.init.reason);

   RETVOID;
} /* uehShutdown */ 





/*
 *
 *       Fun:     uehGenCfg
 *
 *       Desc:    This function is invoked to do the general 
 *                configuration for RRC User Layer.
 *
 *       Ret:     ROK     - ok
 *                RFAILED - Failure
 *                ROUTRES - Memory Allocation Failure due to resource
 *                          crunch.
 *
 *       Notes :  The general configuration configures the lmPst
 *                to use in all calls to the Stack Manager. This
 *                function reseruehs the static memory requirements
 *                for the layer and configures the static data strs. 
 *
 *       File:    ueh_mi.c
 *
 */
PRIVATE S16 uehGenCfg(
  UehGenCfg                *gen,
  CmStatus                *status
  )
{
   UehCellCb *cellCb = uehGetCellCb();
   U8 idx = 0;

   logFunc("%s","uehGenCfg");

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   if (TRUE NOTEQ uehCb.init.cfgDone)
   {
      cmMemcpy((U8 *)&(uehCb.init.lmPst), (U8 *)&(gen->lmPst),
            (PTR)sizeof(Pst));

      uehCb.init.lmPst.srcProcId = gen->lmPst.srcProcId;
      uehCb.init.lmPst.srcEnt    = gen->lmPst.srcEnt;
      uehCb.init.lmPst.srcInst   = gen->lmPst.srcInst;
      uehCb.init.lmPst.event     = EVTNONE;
      uehCb.init.cfgDone         = TRUE;
   }

   logInfo("%s","Initialising CELL Callblock");
   /*clearBytes(cellCb, sizeof(UehCellCb));

   cellCb->isUnlocked          = FALSE;
   cellCb->isS1LinkUp          = FALSE;
   cellCb->isUnbarred          = FALSE;
   cellCb->isConfigured        = FALSE;
   cellCb->opStopTmrDur        = 1000;
   */
 
  /* Timers are initialised in timerDescTable */
   cellCb->cellId              = gen->cellCfg.cellId;
   cellCb->modType             = gen->cellCfg.modType;
   cellCb->duplexMode          = gen->cellCfg.duplexMode;
   cellCb->maxUeSupp           = gen->cellCfg.maxUeSupp;
   cellCb->numUeCfg            = 0;
   cellCb->modiPeriodTmr       = uehGetActualDefaultPagCycle[uehCb.uehCfgCb.sib2Cfg.defaultPagCycle] *
                                  uehGetActualModiPriodCoeff[uehCb.uehCfgCb.sib2Cfg.modificationPeriodCoeff]; 
 
   cellCb->isSysInfoNeedToSend = FALSE;
   cellCb->maxRntiPool = uehCb.uehCfgCb.cellCfg.maxConnectedUes + UEH_RESERVED_RNTI_POOL_SIZE;

   if(cellCb->maxRntiPool > MAX_UE_SUPP)
   {
     logError("RNTI: Configured max %d > Compile time max %d", cellCb->maxRntiPool, MAX_UE_SUPP);
     RETVALUE (RFAILED);
   } 
   /* plmn identity */
   for (idx = 0; idx < 3; idx++)
   {
      /* mcc */
      if(PRSNT_NODEF EQ gen->cellCfg.mcc[idx].pres)
      {
         cellCb->sysRelInfo.mcc[idx].pres = PRSNT_NODEF;
         cellCb->sysRelInfo.mcc[idx].val = gen->cellCfg.mcc[idx].val;
      }
      /* mnc */
      if(PRSNT_NODEF EQ gen->cellCfg.mnc[idx].pres)
      {
         cellCb->sysRelInfo.mnc[idx].pres = PRSNT_NODEF;
         cellCb->sysRelInfo.mnc[idx].val = gen->cellCfg.mnc[idx].val;
      }
   }

   cellCb->sysRelInfo.trackAreaCode =  gen->cellCfg.trackAreaCode;
   cellCb->sysRelInfo.freqBandInd = gen->cellCfg.freqBandInd;

   if(3 > uehCb.uehCfgCb.sib2Cfg.nB)
   {
     cellCb->ns = nBEnumMappingVal[uehCb.uehCfgCb.sib2Cfg.nB];
   }
   else
   {
     cellCb->ns = 1;
   }

   uehCpyCmTptAddr(&uehCb.datAppAddr,&gen->dataAppAddr);

   uehCb.init.cfgDone = TRUE;

   /* Initialize Pending Paging List */
   uehInitPendingPagingList();

   if (ROK NOTEQ uehRegInitTmr())
   {
     RETVALUE (RFAILED);
   }

   RETVALUE(ROK);
} /* uehGenCfg */

/*
 *
 *       Fun:     uehLSapCfg
 * 
 *       Desc:    This function is used to configure the Lower SAP.
 * 
 *       Ret:     ROK - ok / RFAILED - Failure 
 *                / ROUTRES - Memory out of resources.
 *
 *       Notes :  None.
 *
 *       File:    ueh_mi.c
 * 
 */
#ifdef ANSI
PRIVATE S16 uehLSapCfg(
  UehMngmt      *cfg,
  CmStatus     *status
  )
#else
PRIVATE S16 uehLSapCfg(cfg, status)
UehMngmt      *cfg;
CmStatus     *status;
#endif
{
   UehLiSapCb *sap = NULLP;
   UehLSapCfg *lSap = &cfg->u.cfg.u.lSap;

   logFunc("%s","uehLSapCfg");

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   if (NULLP EQ lSap)
   {
      UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE("SAP Config", lSap, "Valid Address");
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_INVALID_PAR_VAL;
      RETVALUE(RFAILED);
   }

   switch(cfg->hdr.elmId.elmnt)
   {
      case STUEHNHUSAP:
         logInfo("%s","Allocating memory and saving received information for RRC(NHU) SAP");
         UEH_ALLOC(&uehCb.nhuSap, sizeof(UehLiSapCb *));
         UEH_ALLOC(&uehCb.nhuSap[0], sizeof(UehLiSapCb));
         sap = uehCb.nhuSap[0];
         break;
      case STUEHRGRSAP:
         logInfo("%s","Allocating memory and saving received information for MAC(RGR) SAP");
         UEH_ALLOC(&uehCb.rgrSap, sizeof(UehLiSapCb *));
         UEH_ALLOC(&uehCb.rgrSap[0], sizeof(UehLiSapCb));
         sap = uehCb.rgrSap[0];
         break;
      case STUEHCTFSAP:
         logInfo("%s","Allocating memory and saving received information for CL(CTF) SAP");
         UEH_ALLOC(&uehCb.ctfSap, sizeof(UehLiSapCb *));
         UEH_ALLOC(&uehCb.ctfSap[0], sizeof(UehLiSapCb));
         sap = uehCb.ctfSap[0];
         break;
      case STUEHSZTSAP:
         logInfo("%s","Allocating memory and saving received information for S1AP(SZT) SAP");
         UEH_ALLOC(&uehCb.sztSap, sizeof(UehLiSapCb *));
         UEH_ALLOC(&uehCb.sztSap[0], sizeof(UehLiSapCb));
         sap = uehCb.sztSap[0];
        
         uehCb.mmeCb.s1apSap = uehCb.sztSap[0];
        
         break;
      case STUEHUDHSAP:
         logInfo("%s","Allocating memory and saving received information for UDH SAP");
         UEH_ALLOC(&uehCb.udhSap, sizeof(UehLiSapCb *));
         UEH_ALLOC(&uehCb.udhSap[0], sizeof(UehLiSapCb));
         sap = uehCb.udhSap[0];
         break;
      default:
         UEH_ERROR_ELMNT_OUT_OF_RANGE("Sap Element", cfg->hdr.elmId.elmnt, STUEHRGRSAP, STUEHUDHSAP);
         RETVALUE(RFAILED);
         break;
   }

   sap->suId = lSap->suId;
   sap->spId = lSap->spId;
   sap->pst.srcProcId = SFndProcId();
   sap->pst.srcEnt = lSap->srcEnt;
   sap->pst.srcInst = lSap->srcInst;
   sap->pst.dstProcId = lSap->dstProcId;
   sap->pst.dstEnt = lSap->dstEnt;
   sap->pst.dstInst = lSap->dstInst;
   sap->pst.prior = lSap->dstPrior;
   sap->pst.route = lSap->dstRoute;
   sap->pst.selector = lSap->dstSel;

   /* With multicore support layer shall use the assigned region from SSI */
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
   sap->mem.region      = uehCb.init.region;
#else
   sap->mem.region      = lSap->mem.region;
#endif
   sap->mem.pool        = lSap->mem.pool;
   sap->maxBndRetry     = lSap->maxBndRetry;
   sap->bndTmr.val      = lSap->bndTmr.val;
   sap->bndTmr.enb      = lSap->bndTmr.enb;
   sap->sapState        = LUEH_SAP_UNBOUND;

   cmInitTimers(&(sap->timer.cmTimer) , 1);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:     UehMiLuehCfgReq
 *
 *       Desc:    This primitive is called by LM to configure the 
 *                Framing Protocol User 
 *
 *       Ret:     ROK/RFAILED
 *
 *       Notes :  All configuration requests are handled from this
 *                function. 
 *
 *       File:    ueh_mi.c
 *
 */
#ifdef ANSI
PUBLIC S16 UehMiLuehCfgReq(
  Pst                     *pst,
  UehMngmt                *cfg
  )
#else
PUBLIC S16 UehMiLuehCfgReq(pst, cfg)
Pst                     *pst;
UehMngmt                 *cfg;
#endif
{

   UehMngmt              cfmCfg;
   S16                  retVal;

   logFunc("%s","UehMiLuehCfgReq");

   logInfo("UEH received Config request from SM element:%d", cfg->hdr.elmId.elmnt );

   if (NULLP EQ cfg)
   {
      UEH_ERROR_NULL_POINTER_IDENTIFIED();
      RETVALUE(RFAILED);
   }

   /* Check if general configuration is already done */
   if ( (!uehCb.init.cfgDone) && (STUEHGEN NOTEQ cfg->hdr.elmId.elmnt) )
   {
      UEH_ERROR_MSG_RCVD_IN_UNEXPECTED_STATE(cfg->hdr.elmId.elmnt,uehCb.init.cfgDone);

      /* Send Config Confirm to LM with NOK status */
      cfmCfg.cfm.status = LCM_PRIM_NOK;
      cfmCfg.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      uehGenerateCfm(pst, &cfg->hdr, TCFG, &cfmCfg);

      RETVALUE(RFAILED);
   }

   retVal = ROK;
   switch (cfg->hdr.elmId.elmnt)
   {
      case STUEHGEN:
         retVal = uehGenCfg(&cfg->u.cfg.u.genCfg, &cfmCfg.cfm);
      break;

      case STUEHCTFSAP:
      case STUEHNHUSAP:
      case STUEHRGRSAP:
      case STUEHSZTSAP:
      case STUEHUDHSAP:
         retVal = uehLSapCfg(cfg, &cfmCfg.cfm);
         break;

      default:
         UEH_ERROR_ELMNT_OUT_OF_RANGE("Config request from SM", cfg->hdr.elmId.elmnt, STUEHGEN, STUEHUDHSAP);
         /* Send Config Confirm to LM with NOK status */
         cfmCfg.cfm.status = LCM_PRIM_NOK;
         cfmCfg.cfm.reason = LCM_REASON_INVALID_ELMNT;
         retVal = RFAILED;
         break;
   }

   uehGenerateCfm(pst, &cfg->hdr, TCFG, &cfmCfg);
   RETVALUE(retVal);
} /* UehMiLuehCfgReq */


/*
 *
 *       Fun:     UehMiLuehCntrlReq
 *
 *       Desc:    This primitive is called by LM to issue control 
 *                requests to the RRC User layer
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 *
 *       File:    ueh_mi.c
 *
 */
#ifdef ANSI
PUBLIC S16 UehMiLuehCntrlReq(
    Pst                     *pst,
    UehMngmt                *cntrl
  )
#else
PUBLIC S16 UehMiLuehCntrlReq(pst, cntrl)
Pst                     *pst;
UehMngmt                 *cntrl;
#endif
{
   UehMngmt              cntrlCfm;
   S16                   retVal = ROK;
   static U8             tmrConfg= 0;
   logFunc("%s","UehMiLuehCntrlReq");

   cmMemset((U8 *) &cntrlCfm, (U8) 0, (S16) sizeof(UehMngmt));

   logInfo("UEH received control request from SM element:%d", cntrl->hdr.elmId.elmnt );

   /* validate input parameters */
   if (NULLP EQ cntrl)
   {
     UEH_ERROR_NULL_POINTER_IDENTIFIED();
     RETVALUE(RFAILED);
   }
   if (tmrConfg != 1)
   {
     uehInitTmrDescTable();
     tmrConfg = 1;
   }
   /* Is genconfig done  */
   if (TRUE NOTEQ uehCb.init.cfgDone) 
   {
      UEH_ERROR_MSG_RCVD_IN_UNEXPECTED_STATE(cntrl->hdr.elmId.elmnt, uehCb.init.cfgDone);
      cntrlCfm.cfm.status = LCM_PRIM_NOK;
      cntrlCfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      cntrlCfm.u.cntrl.action = cntrl->u.cntrl.action;
      cntrlCfm.u.cntrl.subAction = cntrl->u.cntrl.subAction;
      uehGenerateCfm (pst, &cntrl->hdr, TCNTRL, &cntrlCfm);
      RETVALUE(RFAILED);
   }
   cmMemcpy((U8 *)&(uehCb.lmHdr), (U8 *)(&cntrl->hdr), sizeof(Header));

   cntrlCfm.cfm.status = LCM_PRIM_OK;
   cntrlCfm.cfm.reason = LCM_REASON_NOT_APPL;

   /* Identify the element for control */
   switch (cntrl->hdr.elmId.elmnt)
   { 
      case STUEHGEN: 
         switch (cntrl->u.cntrl.action)
         {
            case ASHUTDOWN:
               uehShutdown();
               break;
            case AENA:
               switch(cntrl->u.cntrl.subAction)
               {
                 case SAUSTA:
                 {
                     uehCb.init.usta = TRUE;
                     break;
/* ve005.101 */  }
                 case SADBG:
                 {
                    uehCb.init.dbgMask = cntrl->u.cntrl.u.dbgMask;
                    break;
                 }
                case ADISIMM:
                {    /* Action DISABLE */
                   uehCb.init.dbgMask &= ~(cntrl->u.cntrl.u.dbgMask);
                   break;
                }
                default:
                {
                  UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE("Subaction field", cntrl->u.cntrl.subAction, "ADISIMM(2)/SADBG(5)/SAUSTA(3)");
                  break;
                }
               }
               break;
         }
         break;


      case STUEHRGRSAP:
      case STUEHCTFSAP:
      case STUEHNHUSAP:
      case STUEHSZTSAP:
      case STUEHUDHSAP:
         retVal = uehLSapCntrl(&cntrl->u.cntrl, &cntrlCfm.cfm,
               cntrl->hdr.elmId.elmnt);
         break;

      case STUEHCELLCFG:
       /*Perform Cell Config before S1 Setup*/
       if (!uehCb.cfgValid)
       {
         UEH_ERROR_RESOURCE_NOT_AVAILABLE("Valid Config from BSM");
         retVal = RFAILED;
       }
       else
       {
         retVal = uehSndNhuCellCfgReq(uehGetCellCb()->cellId);
       }
       break;

      default:
      {
         UEH_ERROR_ELMNT_OUT_OF_RANGE("Control Element", cntrl->hdr.elmId.elmnt, STUEHGEN, STUEHCELLCFG);
         retVal = RFAILED;
         break;
      }
   }

   if(RFAILED EQ retVal)
   {
      cntrlCfm.cfm.status = LCM_PRIM_NOK;
      cntrlCfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
   }

   cntrlCfm.u.cntrl.action = cntrl->u.cntrl.action;
   cntrlCfm.u.cntrl.subAction = cntrl->u.cntrl.subAction;

   uehGenerateCfm (pst, &cntrl->hdr, TCNTRL, &cntrlCfm);

   RETVALUE(retVal);
} /* UehMiLuehCntrlReq */

/**********************************************************************
  
        Revision history:
  
**********************************************************************/
