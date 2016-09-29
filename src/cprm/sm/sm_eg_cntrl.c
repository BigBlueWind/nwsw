/**
 * @file  sm_eg_cntrl.c
 * @brief This file contains configuration functions for EGTPU
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
*     a) Created initial version from sample application
 */
  
/* header include files (.h) */

#include "ccpu_cmn.h"

/* RRM Related Files */
#include "ueh.h"
#include "bs_tables.h"
#include "log.h"
#include "ctype.h"
#include "sys/socket.h"
#include "arpa/inet.h"

/*
 *
 *       Fun:    uehSmEgCfg - configure EG
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ueh_sm_eg_cntrl.c
 *
 */

PUBLIC Void uehSmEgCfg( Void )
{
   logFunc("%s","uehSmEgCfg");

   smBuildEgGenCfg();
   smBuildEgLSapCfg(EG_HI_SUID, EG_HI_SPID);
   smBuildEgUSapCfg(UDH_EG_SPID);
   smBuildEgIpAddrCfg(UDH_EG_SPID);
   smBuildTServerCfg();

   RETVOID;
} /* end of uehSmEgCfg */



/*
*
*       Fun:   smBuildEgGenCfg
*
*       Desc:  fill in genCfg for EG
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_eg_cntrl.c 
*
*/
PUBLIC S16 smBuildEgGenCfg(
  Void
  )
{
   EgMngmt      egMgt;
   EgGenCfg     *cfg = NULLP;
   Pst          pst;
   EgtpConfig   *egtpConfig;
   //EgtpCfgTable *egtpCfgTable;

   logFunc("%s","smBuildEgGenCfg");

   SM_SET_ZERO(&egMgt, sizeof(EgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(egMgt.u.cfg.s.gen);

   egtpConfig = &(smCb.cfgCb.egtpConfig);
   /*----------- Fill General Configuration Parameters ---------*/
   cfg->nmbUSaps                    = egtpConfig->nmbUSaps;
   cfg->maxNmbTSaps                 = egtpConfig->maxNmbTSaps;
   cfg->nmbSrvs                     = egtpConfig->nmbSrvs;
   cfg->timerRes                    = egtpConfig->timerRes;
   cfg->resThreshUpper              = egtpConfig->resThreshUpper;
   cfg->resThreshLower              = egtpConfig->resThreshLower;

   cfg->lmPst.dstProcId             = SM_SM_PROC;
   cfg->lmPst.srcProcId             = SM_EG_PROC;
   cfg->lmPst.dstEnt                = ENTSM;
   cfg->lmPst.srcEnt                = ENTEG;
   cfg->lmPst.dstInst               = 0;
   cfg->lmPst.srcInst               = 0;
   cfg->lmPst.prior                 = 0;
   cfg->lmPst.route                 = 0;
   cfg->lmPst.event                 = 0;
   cfg->lmPst.region                = 0;
   cfg->lmPst.pool                  = 0;
   cfg->lmPst.selector              = SM_SELECTOR_LC;

   /* Removed Ifdef flag As we use only EGTP-U */
   cfg->eguT3Timer.enb           = egtpConfig->eguT3TimerEnabled;
   cfg->eguT3Timer.val           = egtpConfig->eguT3TimerVal;
   cfg->nmbRetries               = egtpConfig->nmbRetries;
   cfg->echoTmr.enb              = egtpConfig->echoTmrEnabled;
   cfg->echoTmr.val              = egtpConfig->echoTmrVal;
   cfg->reOrderTmr.enb           = egtpConfig->reOrderTmrEnabled;
   cfg->reOrderTmr.val           = egtpConfig->reOrderTmrVal;
   cfg->pvtExtId                 = egtpConfig->pvtExtId;
   cfg->pduWindSize              = egtpConfig->pduWindSize;
   cfg->maxNumOfIp               = egtpConfig->maxNumOfIp;
   cfg->nodeArch                 = egtpConfig->nodeArch;
#ifdef EGTP_U_MULTI_THREADED
   cfg->nmbWorkerThread          = egtpConfig->nmbWorkerThread;
   cfg->loadDistTmr.enb          = egtpConfig->loadDistTmrEnabled;
   cfg->loadDistTmr.val          = egtpConfig->loadDistTmrVal;
#endif /* EGTP_U_MULTI_THREADED */
   /* Fill Header */
   egMgt.hdr.msgType             = TCFG;
   egMgt.hdr.msgLen              = 0;
   egMgt.hdr.entId.ent           = ENTEG;
   egMgt.hdr.entId.inst          = SM_INST_ZERO;
   egMgt.hdr.elmId.elmnt         = STEGGEN;
   egMgt.hdr.elmId.elmntInst1    = 0;
   egMgt.hdr.elmId.elmntInst2    = 0;
   egMgt.hdr.elmId.elmntInst3    = 0;
   egMgt.hdr.seqNmb              = 0;
   egMgt.hdr.version             = 0;
   egMgt.hdr.transId             = 0;

   egMgt.hdr.response.prior      = PRIOR0;
   egMgt.hdr.response.route      = RTESPEC;
   egMgt.hdr.response.mem.region = smCb.init.region;
   egMgt.hdr.response.mem.pool   = EG_POOL;
   egMgt.hdr.response.selector   = SM_SELECTOR_LC;

    /* Fill Pst */
   pst.selector                  = SM_SELECTOR_LC;
   pst.srcEnt                    = ENTSM;
   pst.dstEnt                    = ENTEG;
   pst.srcProcId                 = SM_SM_PROC;
   pst.dstProcId                 = SM_EG_PROC;

   logInfo("%s", "EGTP General Config Request sent");

   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&pst, &egMgt);
 
   RETVALUE(ROK);
}/* smBuildEgGenCfg */


/*
*
*       Fun:   smBuildEgUSapCfg
*
*       Desc:  Call handler for SSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_eg_cntrl.c
*
*/
PUBLIC S16 smBuildEgUSapCfg(
  SpId        spId              /* Service Provider Sap ID */
  )
{
   EgMngmt  egMgt;
   EgUSapCfg    *cfg = NULLP;
   Pst      pst;

   logFunc("%s","smBuildEgUSapCfg");
 
   SM_SET_ZERO(&egMgt, sizeof(EgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &(egMgt.u.cfg.s.uSap);

   cfg->sapId      = spId;

#ifdef EGTP_C
   cfg->echo       = FALSE;
   cfg->intfType   = EGT_GTP_INT_S11;
#endif
   cfg->selector   = SM_SELECTOR_LC;
   cfg->mem.region = DFLT_REGION;
   cfg->mem.pool   = EG_POOL;
   cfg->priority   = PRIOR0;
   cfg->route      = RTESPEC;
   cfg->dstProcId  = SM_UDH_PROC;
   cfg->dstEnt     = ENTUDH;
   cfg->dstInst    = 0;
   cfg->flag       = LEG_EGTP_U;

   /* Fill Header */
   egMgt.hdr.msgType             = TCFG;
   egMgt.hdr.msgLen              = 0;
   egMgt.hdr.entId.ent           = ENTEG;
   egMgt.hdr.entId.inst          = SM_INST_ZERO;
   egMgt.hdr.elmId.elmnt         = STEGUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTEG;
   pst.srcProcId = SM_SM_PROC;
   pst.dstProcId = SM_EG_PROC;
   
   logInfo("EGTP upper SAP:UDH_EG_SPID Config Request sent for SAP spId:%d", spId);

   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&pst, &egMgt);
 
   RETVALUE(ROK);
}/* smBuildEgUSapCfg */

/*
*
*       Fun:   smBuildEgLSapCfg
*
*       Desc:  Call handler for TSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_eg_cntrl.c
*
*/
PUBLIC S16 smBuildEgLSapCfg(
  SuId        suId,             /* E-GTP Sap ID */
  SpId        spId              /* Service Provider Sap ID */
  )
{
   EgMngmt  egMgt;
   EgTSapCfg  *cfg = NULLP;
   EgTSapReCfg *reCfg = NULLP;
   Pst       pst;
   EgtpConfig *egtpConfig;

   logFunc("%s","smBuildEgLSapCfg");
  
   SM_SET_ZERO(&egMgt, sizeof(EgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(egMgt.u.cfg.s.tSap);
   reCfg = &(egMgt.u.cfg.r.tSapReCfg);

   cfg->tSapId  = suId;
   cfg->spId    = spId;
   cfg->tptType = LEG_TSAP_TYPE_TUCL;

   cfg->selector     = UEHSM_EGHISEL;
   cfg->mem.region   = DFLT_REGION;
   cfg->mem.pool     = EG_POOL;
   cfg->priority     = PRIOR0;
   cfg->route        = RTESPEC;
   cfg->dstProcId    = SM_HI_PROC;
   cfg->dstEnt       = ENTHI;
   cfg->dstInst      = (Inst)0;

   egtpConfig = &(smCb.cfgCb.egtpConfig);
   cfg->tInitTmr.enb = egtpConfig->tInitTmrEnabled;
   cfg->tInitTmr.val = egtpConfig->tInitTmrVal;

   cfg->tptParam.type = CM_TPTPARAM_SOCK;
   cfg->tptParam.u.sockParam.listenQSize = egtpConfig->tPar_u_sockParam_listenQSize;
   cfg->tptParam.u.sockParam.numOpts     = egtpConfig->tserv_tPar_u_sockParam_numOpts;

   reCfg->tPar.type = CM_TPTPARAM_SOCK;
   reCfg->tPar.u.sockParam.listenQSize = egtpConfig->tPar_u_sockParam_listenQSize;
   reCfg->tPar.u.sockParam.numOpts     = egtpConfig->tserv_tPar_u_sockParam_numOpts;
   reCfg->maxBndRetry                  = egtpConfig->maxBndRetry;
   reCfg->bndTmCfg.enb                 = egtpConfig->bndTmCfgEnabled;
   reCfg->bndTmCfg.val                 = egtpConfig->bndTmCfgVal;
   /* Fill Header */
   egMgt.hdr.msgType             = TCFG;
   egMgt.hdr.msgLen              = 0;
   egMgt.hdr.entId.ent           = ENTEG;
   egMgt.hdr.entId.inst          = SM_INST_ZERO;
   egMgt.hdr.elmId.elmnt         = STEGTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTEG;
   pst.srcProcId = SM_SM_PROC;
   pst.dstProcId = SM_EG_PROC;

  logInfo("EGTP: TUCL SAP Config Request sent for spId:%d, suId:%d", spId, suId);

   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&pst, &egMgt);
 
   RETVALUE(ROK);
}/* smBuildEgLSapCfg */

/*
*
*       Fun:   asciiToHex
*
*       Desc:  Converts ASCII to HEX
*
*       Ret:   U8
*
*       Notes: None
*
*       File:  sm_eg_cntrl.c
*
*/
PRIVATE U8  asciiToHex (U8 *revdString)
{
  U8 temp = 0, i = 0, len = strlen (( const S8 *)revdString);
  S8 *b;

  if (len == 3 || len == 4) /*Check is for limiting the user to enter 3 or 4 digit numbers e.g. 0x12*/
  {
    if (revdString [0] == '0' && ( revdString [1] == 'x' || revdString [1] == 'X')) /*forcing user to enter the values starting with 0x or 0X*/
    {
      b = strtok ((S8 *)revdString, "x");
      b = strtok (NULL, "x");
      for(i = 0; i < strlen (b); i++)
      {
        if (isdigit(b[i]))  /* check for given hex value is temp digit */
        {
          temp = temp * 16 + (b[i] - 48);
        }
        else
        {
          if ((b[i] >= 65 && (b[i]<= 70))||((b[i] >= 97)&&(b[i]<=102)) )  /*check is for valid hex value i.e. between a to f */
          {
            if(isupper(b[i]))    /* Checking for the given hex value is in uppercase or lowercase */
            {
              temp = temp * 16 + (b[i] - 55); /* Coverting ASCII value to HEX */
            }
            else
            {
              temp = temp * 16 + (b[i] - 87); /* Coverting ASCII value to HEX */
            }
          }
        }
      }
      return temp; /*Returning Hex value */
    }
    else
    {
       logError ("Received string is not in expected format it should start with 0x, Received string is %s", revdString);
       exit (-1);
    }
  }
  else
  {
    logError("Received string is not in the expected format it should be either 3 or 4 characters, Received string is  %s", revdString);
    exit (-1);
  }
} /*asciiToHex*/

/*
*
*       Fun:   smBuildEgIpAddrCfg
*
*       Desc:  Call handler for IP Address Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_eg_cntrl.c
*
*/

PUBLIC S16 smBuildEgIpAddrCfg(
  SpId        spId              /* Service Provider Sap ID */
  )
{
   EgMngmt  egMgt;
   EgIpAddrCfg *cfg = NULLP;
   Pst       pst;
   EgtpConfig *egtpConfig;
   struct in_addr in;

   logFunc("%s","smBuildEgIpAddrCfg");

   SM_SET_ZERO(&egMgt, sizeof(EgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   /* Fill Header */
   egMgt.hdr.msgType             = TCFG;
   egMgt.hdr.msgLen              = 0;
   egMgt.hdr.entId.ent           = ENTEG;
   egMgt.hdr.entId.inst          = SM_INST_ZERO;
   egMgt.hdr.elmId.elmnt         = STEGIPADDR;

   cfg = &(egMgt.u.cfg.s.ipAddr);

   UEHSM_ALLOC(&(cfg->ipInfo), sizeof(EgIpInfo));
   egtpConfig     = &(smCb.cfgCb.egtpConfig);
   cfg->nmbIpAddr = egtpConfig->maxNumOfIp;

   cfg->ipInfo[0].srcIpAddr.type                  = CM_TPTADDR_IPV4;
   cfg->ipInfo[0].nodeType                        = TRUE;
   cfg->ipInfo[0].cfmStatus                       = FALSE;
   cfg->ipInfo[0].spId                            = spId;

   cfg->ipInfo[0].srcIpAddr.u.ipv4TptAddr.port    = 0;
   /*BS-1290 Changes*/
   cfg->ipInfo[0].srcIpAddr.u.ipv4TptAddr.address = smCb.cfgCb.enbIpAddr;

   cfg->ipInfo[0].echoReqPvtExtVal.length         = egtpConfig->ipInfo_echoReqPvtExtValLength;
   cfg->ipInfo[0].echoReqPvtExtVal.buff[0]        = asciiToHex (egtpConfig->ipInfo_echoReqPvtExtValBuff);//0x01;
   cfg->ipInfo[0].echoRspPvtExtVal.length         = egtpConfig->ipInfo_echoRspPvtExtValLength;
   cfg->ipInfo[0].echoRspPvtExtVal.buff[0]        = asciiToHex (egtpConfig->ipInfo_echoRspPvtExtValBuff);//0x07;
   cfg->ipInfo[0].errIndPvtExtVal.length          = egtpConfig->ipInfo_errIndPvtExtValLength;
   cfg->ipInfo[0].errIndPvtExtVal.buff[0]         = asciiToHex (egtpConfig->ipInfo_errIndPvtExtValBuff);//0x05;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTEG;
   pst.srcProcId = SM_SM_PROC;
   pst.dstProcId = SM_EG_PROC;

   in.s_addr = ntohl(cfg->ipInfo[0].srcIpAddr.u.ipv4TptAddr.address);
   logInfo("EGTP IPAddr Config Request sent with IP:%s",inet_ntoa(in) );

   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&pst, &egMgt);

   RETVALUE(ROK);

}/* smBuildEgIpAddrCfg */

                                                                
/*
*
*       Fun:   smBuildTServerCfg
*
*       Desc:  Call handler for Transport server Cfg Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  egac_leghdl.c 
*
*/
PUBLIC S16 smBuildTServerCfg(
  Void
  )
{
   EgMngmt  egMgt;
   Pst      pst;
   EgTptSrvCfg  *cfg = NULLP;
   EgtpConfig *egtpConfig;
   struct in_addr in;

   logFunc("%s","smBuildTServerCfg");

   SM_SET_ZERO(&egMgt, sizeof(EgMngmt));   
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &(egMgt.u.cfg.s.tptSrv);
   egtpConfig = &(smCb.cfgCb.egtpConfig);

   cfg->tptSrvId                         = 1;
   cfg->tSapId                           = 0;
   cfg->uSapId                           = 0;
   cfg->tptAddr.type                     = CM_TPTADDR_IPV4;
   cfg->tptAddr.u.ipv4TptAddr.port       = UEH_DFLT_EGTP_PORT; 
   /*BS-1290 Changes*/
   cfg->tptAddr.u.ipv4TptAddr.address    = smCb.cfgCb.enbIpAddr;

   cfg->tptParam.u.sockParam.numOpts     = egtpConfig->tserv_tPar_u_sockParam_numOpts;
   cfg->tptParam.u.sockParam.listenQSize = egtpConfig->tserv_tPar_u_sockParam_listenQSize;
   cfg->opnSrvRetryCnt                   = egtpConfig->opnSrvRetryCnt;
   cfg->opnSrvTmr.enb                    = egtpConfig->opnSrvTmrEnabled;
   cfg->opnSrvTmr.val                    = egtpConfig->opnSrvTmrVal;
   cfg->hostName.pres                    = egtpConfig->hostNamePresent;
   cfg->hostName.len                     = egtpConfig->hostNameLen;
   cfg->gtp_C_Or_U                       = egtpConfig->gtp_C_Or_U;

  /* Fill Header */
   egMgt.hdr.msgType             = TCFG;
   egMgt.hdr.msgLen              = 0;
   egMgt.hdr.entId.ent           = ENTEG;
   egMgt.hdr.entId.inst          = SM_INST_ZERO;
   egMgt.hdr.elmId.elmnt         = STEGSRVR;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTEG;
   pst.srcProcId = SM_SM_PROC;
   pst.dstProcId = SM_EG_PROC;

   in.s_addr = ntohl(cfg->tptAddr.u.ipv4TptAddr.address);
   logInfo("EGTP: TServer Config request sent with IPAddr:%s", inet_ntoa(in));

   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&pst, &egMgt);

   RETVALUE(ROK);
} /* end of smBuildTServerCfg */ 

/*
 *
 *       Fun:    smBindEgToHitSap
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_eg_cntrl.c
 *
 */

PUBLIC Void smBindEgToHitSap(
    SpId        spId
  )
{
   EgMngmt              egMgt;
   Pst                  pst;

   logFunc("%s","smBindEgToHitSap");

   SM_SET_ZERO(&egMgt, sizeof(EgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   egMgt.hdr.msgType             = TCNTRL;
   egMgt.hdr.entId.ent           = ENTEG;
   egMgt.hdr.entId.inst          = SM_INST_ZERO;
   egMgt.hdr.elmId.elmnt         = STEGTSAP;

   egMgt.u.cntrl.action  = ABND_ENA;
   egMgt.u.cntrl.subAction  = SAELMNT;
   egMgt.u.cntrl.s.sap.sapId    = spId;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTEG;
   pst.dstProcId = SM_EG_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s", "Bind control request sent to EGTP(EG), to bind to TUCL");

   (Void)SmMiLegCntrlReq(&pst, &egMgt);

   RETVOID;
} /* end of smBindEgToHitSap */

/*
 *
 *       Fun:    smBuildEgDbgCntrl
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_eg_cntrl.c
 *
 */

PUBLIC Void smBuildEgDbgCntrl(
    LogConfig *logConfig
  )
{
   EgMngmt              egMgt;
   Pst                  pst;
   U32                  egdbgMask=0;

   logFunc("%s","smBuildEgDbgCntrl");

   SM_SET_ZERO(&egMgt, sizeof(EgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   egMgt.hdr.msgType             = TCNTRL;
   egMgt.hdr.entId.ent           = ENTEG;
   egMgt.hdr.entId.inst          = SM_INST_ZERO;
   egMgt.hdr.elmId.elmnt         = STEGGEN;
   egMgt.hdr.transId             = SM_EGTP_DEBUG_TRANS_ID;

   if((logConfig->logMask1 & LOG_MASK_MSGIN_01) || (logConfig->logMask1 & LOG_MASK_MSGIN_02) ||
      (logConfig->logMask1 & LOG_MASK_MSGOUT_01) || (logConfig->logMask1 & LOG_MASK_MSGOUT_02) )
   {
       egdbgMask = egdbgMask | LEG_DBGMASK_MSG | LEG_DBGMASK_FLOW;
   }
   if(logConfig->logMask1 & LOG_MASK_ERROR )
    {
       egdbgMask = egdbgMask | LEG_DBGMASK_ERROR | LEG_DBGMASK_FATAL | LEG_DBGMASK_WARN;
    }
   if(logConfig->logMask1 & LOG_MASK_DEBUG )
   {
       egdbgMask = egdbgMask | LEG_DBGMASK_INFO | DBGMASK_MI | DBGMASK_UI;
   }
   logInfo(":%s: DBG-Mask: %02x",__func__, egdbgMask);

   egMgt.u.cntrl.subAction       =  SADBG;
   egMgt.u.cntrl.action          =  ADISIMM;
   egMgt.u.cntrl.s.dbgCntrl.genDbgMask = 0xffffffff;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTEG;
   pst.dstProcId = SM_EG_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s", "Debug control request sent to EGTP(EG)");

   /* Sendig msg to EGTP module for resetting the log level */
   (Void)SmMiLegCntrlReq(&pst, &egMgt);

   egMgt.u.cntrl.action          =  AENA;
   egMgt.u.cntrl.s.dbgCntrl.genDbgMask = egdbgMask;
   /* Sendig msg to EGTP module for setting the actual log level */
   (Void)SmMiLegCntrlReq(&pst, &egMgt);

   RETVOID;
} /* end of smBuildEgDbgCntrl*/
