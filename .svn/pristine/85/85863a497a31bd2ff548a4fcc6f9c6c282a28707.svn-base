/*
 * sm_udh_cntrl.c
 *
 *  Created on: 18-May-2011
 *      @author: mkata
 */

/* header include files (.h) */
#include "ccpu_cmn.h"

/* RRM Related Files */
#include "log.h"
#include "ueh.h"

#include "bs_tables.h"
#include "udh_includes.h"
#include "ludh.h"


PRIVATE U8 uehUdhCfg = 0;
/*
 *  Fun :    uehSmUdhCfg - configure EG
 *  Desc:
 *  Ret :    Void
 *  File:    ueh_sm_udh_cntrl.c
 */

PUBLIC Void uehSmUdhCfg(Void)
{
  logFunc("%s","uehSmEgCfg");

  smBuildUdhGtpSapCfg();
  smBuildUdhPdcpSapCfg();
  smBuildUdhUehSapCfg();

  RETVOID;
} /* end of uehSmUdhCfg */




PRIVATE UdhSapsCfgReq smUdhSapsCfgReqMt[] =
{
  cmPkAndSendSmUdhSapCfg,
#ifdef UDH
  udhProcSapCfgReq,
#else
  cmPkAndSendSmUdhSapCfg, /* TODO : Need to write dummy function */
#endif
};
/*
 *  Fun :   SmUdhSapsCfgReq - Configuration request
 *  Desc:
 *  Ret :   ROK     - succeeded
 *          RFAILED - failed
 *  File:   ueh_sm_udh_ctrl.c
 */

PUBLIC S16 smUdhSapsCfgReq
(
  Pst       *pst,
  CmSapCfg  *cmSapCfg
)
{
  logFunc("%s","SmUdhGtpCfgReq");
  if (pst == (Pst *) NULLP || pst->selector >= SM_MAX_UDH_SEL)
  {
    /* Add log Error -SHWETA */
    
    RETVALUE(RFAILED);
  }
   RETVALUE((*(smUdhSapsCfgReqMt[pst->selector]))(pst, cmSapCfg));
} /* end of SmUdhSapsCfgReq() */

/*
 *  Fun :  smBuildUdhGtpSapCfg
 *  Desc:  Call handler for SM-UDH-GTP Sap configuration
 *  Ret :  ROK
 *  File:  ueh_sm_udh_cntrl.c
 */
PUBLIC S16 smBuildUdhGtpSapCfg(Void)
{
  Pst       pst;
  CmSapCfg  gtpCmSapCfg;

  clearBytes(&gtpCmSapCfg, sizeof(CmSapCfg));
  clearBytes(&pst, sizeof(Pst));

  gtpCmSapCfg.suId = UDH_EG_SUID;
  gtpCmSapCfg.spId = UDH_EG_SPID;
  gtpCmSapCfg.sapId = UDH_GTP_SAP;

  gtpCmSapCfg.pst.selector  = SM_SELECTOR_LC;
  gtpCmSapCfg.pst.srcEnt    = ENTUDH;
  gtpCmSapCfg.pst.dstEnt    = ENTEG;
  gtpCmSapCfg.pst.srcProcId = SM_UDH_PROC;
  gtpCmSapCfg.pst.dstProcId = SM_EG_PROC;

  /* Pst to send */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTUDH;
  pst.srcProcId = SM_SM_PROC;
  pst.dstProcId = SM_UDH_PROC;

  logInfo("%s","smBuildUdhGtpSapCfg :sm-udh-gtp Sap Cfg Req sent");

  /* Send the request to the LM */
  (Void) smUdhSapsCfgReq(&pst, &gtpCmSapCfg);

   RETVALUE(ROK);
}/* smBuildUdhGtpSapCfg */

/*
 *  Fun :  smBuildUdhPdcpSapCfg
 *  Desc:  Call handler for SM-UDH-PDCP Sap configuration
 *  Ret :  ROK
 *  File:  ueh_sm_udh_cntrl.c
 */
PUBLIC S16 smBuildUdhPdcpSapCfg(Void)
{
  Pst       pst;
  CmSapCfg  pdcpCmSapCfg;

  SM_SET_ZERO(&pst, sizeof(Pst));
  clearBytes(&pdcpCmSapCfg, sizeof(CmSapCfg));

  pdcpCmSapCfg.suId = UDH_PJ_SUID;
  pdcpCmSapCfg.spId = UDH_PJ_SPID;
  pdcpCmSapCfg.sapId = UDH_PDCP_SAP;

  pdcpCmSapCfg.pst.selector  = SM_SELECTOR_LC;
  pdcpCmSapCfg.pst.srcEnt    = ENTUDH;
  pdcpCmSapCfg.pst.dstEnt    = ENTPJ;
  pdcpCmSapCfg.pst.srcProcId = SM_UDH_PROC;
  pdcpCmSapCfg.pst.dstProcId = SM_PJ_PROC;

  /* Pst to send */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTUDH;
  pst.srcProcId = SM_SM_PROC;
  pst.dstProcId = SM_UDH_PROC;

  logInfo ("%s", "smBuildUdhPdcpSapCfg :sm-udh-pdcp Sap Cfg Req sent");

  /* Send the request to the LM */
  (Void) smUdhSapsCfgReq(&pst, &pdcpCmSapCfg);

   RETVALUE(ROK);
}/* smBuildUdhPdcpSapCfg */

/*
 *  Fun :  smBuildUdhUehSapCfg
 *  Desc:  Call handler for SM-UDH-UEH Sap configuration
 *  Ret :  ROK
 *  File:  ueh_sm_udh_cntrl.c
 */
PUBLIC S16 smBuildUdhUehSapCfg(Void)
{
  Pst       pst;
  CmSapCfg  uehCmSapCfg;

  clearBytes(&uehCmSapCfg, sizeof(CmSapCfg));
  SM_SET_ZERO(&pst, sizeof(Pst));

  uehCmSapCfg.suId = UEH_UDH_SUID;
  uehCmSapCfg.spId = UEH_UDH_SPID;
  uehCmSapCfg.sapId = UDH_UEH_SAP;

  uehCmSapCfg.pst.selector  = SM_SELECTOR_LC;
  uehCmSapCfg.pst.srcEnt    = ENTUDH;
  uehCmSapCfg.pst.dstEnt    = ENTUEH;
  uehCmSapCfg.pst.srcProcId = SM_UDH_PROC;
  uehCmSapCfg.pst.dstProcId = SM_UEH_PROC;

  /* Pst to send */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTUDH;
  pst.srcProcId = SM_SM_PROC;
  pst.dstProcId = SM_UDH_PROC;

  logInfo ("%s", "smBuildUdhUehSapCfg :sm-udh-ueh Sap Cfg Req sent");

  /* Send the request to the LM */
  (Void) smUdhSapsCfgReq(&pst, &uehCmSapCfg);

   RETVALUE(ROK);
}/* smBuildUdhUehSapCfg */

/***********************************************************/

PRIVATE UdhSapsBindReq smUdhSapsBindReqMt[] =
{
  cmPkAndSendSmUdhBndReq,
#ifdef UDH
  udhProcSmUdhBndReq
#else
  cmPkAndSendSmUdhBndReq, /* TODO : Need to write Dummy function */
#endif
};

/*
 *  Fun :   smUdhSapsBindReq - Bind request
 *  Desc:
 *  Ret :   ROK     - succeeded
 *          RFAILED - failed
 *  File:   ueh_sm_udh_ctrl.c
 */

PUBLIC S16 smUdhSapsBindReq
(
  Pst           *pst,
  CmSapBindReq  *udhGtpSapBindReq
)
{
  logFunc("%s","SmUdhGtpCfgReq");
  if (pst == (Pst *) NULLP || pst->selector >= SM_MAX_UDH_SEL)
  {
     /* Add log Error here - SHWETA */
     RETVALUE(RFAILED);
  }
  RETVALUE((*(smUdhSapsBindReqMt[pst->selector]))(pst, udhGtpSapBindReq));
} /* end of smUdhSapsBindReq() */


/*
 *  Fun :  smUdhGtpSapBindReq
 *  Desc:  Call handler for SM-UDH-GTP Sap Bind Request
 *  Ret :  ROK
 *  File:  ueh_sm_udh_cntrl.c
 */
PUBLIC S16 smUdhGtpSapBindReq(Void)
{
  Pst           pst;
  CmSapBindReq  udhGtpSapBindReq;

  clearBytes(&udhGtpSapBindReq, sizeof(CmSapBindReq));
  SM_SET_ZERO(&pst, sizeof(Pst));

  udhGtpSapBindReq.sapId = UDH_GTP_SAP;

  /* Pst to send */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTUDH;
  pst.srcProcId = SM_SM_PROC;
  pst.dstProcId = SM_UDH_PROC;

  logInfo ("%s", "smUdhGtpSapBindReq :sm-udh-ueh Sap Bind Req sent");

  /* Send the request to the LM */
  (Void) smUdhSapsBindReq(&pst, &udhGtpSapBindReq);

   RETVALUE(ROK);
}/* smUdhGtpSapBindReq */

/*
 *  Fun :  smUdhPdcpSapBindReq
 *  Desc:  Call handler for SM-UDH-PDCP Sap Bind Request
 *  Ret :  ROK
 *  File:  ueh_sm_udh_cntrl.c
 */
PUBLIC S16 smUdhPdcpSapBindReq(Void)
{
  Pst           pst;
  CmSapBindReq  udhPdcpSapBindReq;

  clearBytes(&udhPdcpSapBindReq, sizeof(CmSapBindReq));
  SM_SET_ZERO(&pst, sizeof(Pst));

  udhPdcpSapBindReq.sapId = UDH_PDCP_SAP;

  /* Pst to send */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTUDH;
  pst.srcProcId = SM_SM_PROC;
  pst.dstProcId = SM_UDH_PROC;

  logInfo ("%s", "smUdhPdcpSapBindReq :sm-udh-ueh Sap Bind Req sent");

  /* Send the request to the LM */
  (Void) smUdhSapsBindReq(&pst, &udhPdcpSapBindReq);

   RETVALUE(ROK);
}/* smUdhPdcpSapBindReq */

/*
 *
 *       Fun:    SmMiLudhBndCfm - Bind Confirmation
 *
 *       Desc:   Receiuehd Bind Confirmation
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   udh_func.c
 *
 */

PUBLIC S16 SmMiLudhBndCfm
(
  Pst          *pst,
  CmSapBindCfm *cmSapBindCfm,
  U8           *status
)
{
  Pst  udhPst;
  MsgUehBsmEnbConfigFail  enbCfgFail;

  clearBytes(&enbCfgFail, sizeof(MsgUehBsmEnbConfigFail));
  SM_SET_ZERO(&udhPst, sizeof(Pst));

  if (CM_CFG_NOK == *status)
  {
    logError ("BIND OF APP lsap id[%d] Failed ", cmSapBindCfm->sapId);

    enbCfgFail.transId = smCb.enbTransId;
    sendCemMsgToBsm(EVT_CEM_BSM_CFG_FAIL, sizeof(enbCfgFail), (U8*)&enbCfgFail);
    RETVALUE(ROK);
  }
  if(UDH_GTP_SAP == cmSapBindCfm->sapId)
  {
    logInfo ("BIND OF UDH TO EGTP IS SUCCESSFUL SAPID= %d", cmSapBindCfm->sapId);

    if (smCb.smState == UEH_SM_STATE_S1AP_STK_BND_DONE)
    {
      smCb.smState = UEH_SM_STATE_EGTP_STK_BND_DONE;
      logInfo ("%s", "BIND OF EGTP STACK SUCCESSFUL ");

      /* Perform the bind of APP and SCTP */
      UEH_FILL_PST(udhPst, SM_UEH_PROC, ENTSM, ENTUEH, EVTUEHRRCSTKBND, UEH_POOL, SM_SELECTOR_LC);
      if(uehSmSelfPstMsg(&udhPst) != ROK)
      {
    	  logInfo ("Failure at Self Pst. %d", smCb.smState);
      }
    }
  }
  else if(UDH_PDCP_SAP == cmSapBindCfm->sapId)
  {
    logInfo ("BIND OF UDH TO PDCP IS SUCCESSFUL SAPID= %d", cmSapBindCfm->sapId);

    logInfo("%s", "Initiate control request for binding RRC to RLC");
    smBindNhToCkwSap();
  }

  RETVALUE(ROK);
} /* end of SmMiLudhBndCfm() */

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
*       File:  ueh_sm_ueh_ptmi.c
*
*/

PUBLIC S16 SmMiLudhCfgCfm(
  Pst     *pst,
  SpId    spId,
  U8      status
  )
{
   Pst      uehPst;
   SM_SET_ZERO(&uehPst, sizeof(Pst));

   if(CM_CFG_OK == status)
   {
     switch(spId)
     {
       case UDH_GTP_SAP:
         uehUdhCfg |= UEH_UDH_GTP_SAP_CFG;
         break;

       case UDH_PDCP_SAP:
         uehUdhCfg |= UEH_UDH_PDCP_SAP_CFG;
         break;

       case UDH_UEH_SAP:
         uehUdhCfg |= UEH_UDH_UEH_SAP_CFG;
         break;

       default:
         logError("UDH Config confirm received with invalid spId:%d", spId);
         break;
     }

     logInfo("UDH Sap Config confirm received for spId:%d", spId);

     if (uehUdhCfg == UEH_SM_UDH_CONFIGURED)
     {
       logInfo("%s", "UDH CONFIGURATION SUCCESSFULLY PERFORMED");

       smCb.smState = UEH_SM_STATE_UDH_CFG_DONE;
       UEH_FILL_PST(uehPst, SM_UEH_PROC, ENTSM, ENTUEH, EVTUEHAPPCFG, UEH_POOL, SM_SELECTOR_LC);
       if(uehSmSelfPstMsg(&uehPst) != ROK)
       {
         logError("%s", "Error in posting the message to SM for Configuring EVTUEHAPPCFG");
       }
       else
       {
         logInfo("%s", "Message to SM sent for Configuring EVTUEHAPPCFG");
       }
     }
   }
   else
   {
     logError("Configuration Failure received from UDH for spId:%d", spId);
   }
   
   RETVALUE(ROK);
}


/*
 *
 *       Fun:     udhSendLmAlarm
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
#ifdef ANSI
PUBLIC Void udhSendLmAlarm
(
U16 category,           
U16 event,             
U16 cause            
)
#else
PUBLIC Void udhSendLmAlarm(category, event, cause)
U16 category;       
U16 event;         
U16 cause;        
#endif
{
   UehMngmt usta;        /* Unsolicited Status Indication structure */

   logFunc("%s","udhSendLmAlarm");

   /* Initialize the management structure */
   (Void)cmMemset((U8 *) &usta, '\0', (PTR)sizeof(UehMngmt));

   usta.hdr.elmId.elmnt = STUEHGEN;
   usta.u.usta.alarm.category = category;
   usta.u.usta.alarm.event    = event;
   usta.u.usta.alarm.cause    = cause;

   /* Send an unsolicited status indication */
   UehMiLuehStaInd(&uehCb.init.lmPst, &usta);

   RETVOID;

} /* udhSendLmAlarm */


PRIVATE UdhDbgCntrl smUdhDbgCntrlMt[] =
{
  cmPkAndSendSmUdhDbgCntrl,
#ifdef UDH
  udhProcSmUdhDbgCntrl
#else
  cmPkAndSendSmUdhDbgCntrl, /* TODO : Need to write Dummy function */
#endif
};

PUBLIC Void smBuildUdhDbgCntrl(LogConfig *logConfig)
{
  Pst             pst;
  SmUdhLogConfig  smUdhLogConfig;

  clearBytes(&smUdhLogConfig, sizeof(SmUdhLogConfig));
  SM_SET_ZERO(&pst, sizeof(Pst));

  smUdhLogConfig.logMask1 = logConfig->logMask1;

  /* Pst to send */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTUDH;
  pst.srcProcId = SM_SM_PROC;
  pst.dstProcId = SM_UDH_PROC;

  logInfo ("%s", "smBuildUdhDbgCntrl :sm-udh-ueh Sap Debug config Control Message sent");

  /* Send the request to the LM */
  smUdhDbgCntrlMt[pst.selector](&pst, &smUdhLogConfig);
  RETVOID;
}
