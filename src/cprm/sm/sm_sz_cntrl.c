/**
 * @file   sm_sz_cntrl.c
 * @brief This file contains management interface functions for S1AP
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
*     a) Created initial version from sample application
 */

#include "ccpu_cmn.h"

/* RRM Related Files */
#include "ueh.h"
#include "log.h"

/*
*
*       Fun:   Initialize S1AP task.
*
*       Desc:  Invoked to create S1AP Tapa task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_sz_cntrl.c
*
*/

PUBLIC S16 smSzInit
(
SSTskId    sysTskId
)
{
   logFunc("%s","smSzInit");
 
   /* Register S1AP TAPA Task */
   if (SRegTTsk((Ent)ENTSZ, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
                 szActvInit, (ActvTsk)szActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Attach S1AP TAPA Task */
   if (SAttachTTsk((Ent)ENTSZ, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   }

  logInfo ("S1AP Tapa Task successfully registered and attached to %d",sysTskId);

   RETVALUE(ROK);
} /* end of smSzInit */

/*
 *
 *       Fun:    uehSmSzCfg - configure SZ 
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ueh_sm_sz_cntrl.c
 *
 */

PUBLIC Void uehSmSzCfg(
  Void
  )
{
   logFunc("%s","uehSmSzCfg");

   smBuildSzGenCfg();
   smBuildSzProtCfg();
   smBuildSzLSapCfg(SZ_SB_SPID, SZ_SB_SUID);
   smBuildSzUSapCfg(UEH_SZ_SPID);
   /* PEER Addition and Deletion is done in S1AP Manager
    * When Unlock and Lock is done by operator */
   RETVOID;
}


/*
*
*       Fun:   smBuildSzGenCfg
*
*       Desc:  fill in genCfg for SZ
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_sz_cntrl.c 
*
*/
PUBLIC S16 smBuildSzGenCfg
(
Void
)
{
   SzMngmt        szMgt;
   SzGenCfg       *cfg = NULLP;
   Pst            pst;
   S1apConfig     *s1apConfig;
   
   logFunc("%s","smBuildSzGenCfg");

   SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(szMgt.u.cfg.u.gen);
   s1apConfig = &(smCb.cfgCb.s1apConfig);

   /*----------- Fill General Configuration Parameters ---------*/
   cfg->nmbSztSaps                     = (S16)s1apConfig->nmbSztSaps;
   cfg->nmbSctSaps                     = (S16)s1apConfig->nmbSctSaps;
   cfg->nmbConn                        = (U16)smCb.cfgCb.s1apCfgInfo.maxConnectedUes;//smCfgCb.s1apCfgInfo.maxConnectedUes;
   cfg->nmbPeer                        = (U32)s1apConfig->nmbPeer;
   cfg->timeRes                        = (Ticks)s1apConfig->timeRes;

   cfg->lmPst.srcProcId                = SM_SM_PROC;
   cfg->lmPst.dstProcId                = SM_SM_PROC;
   cfg->lmPst.srcEnt                   = (Ent)ENTSZ;
   cfg->lmPst.dstEnt                   = (Ent)ENTSM;
   cfg->lmPst.srcInst                  = (Inst)0;
   cfg->lmPst.dstInst                  = (Inst)0;
   cfg->lmPst.prior                    = (Prior)UEHSM_MSGPRIOR;
   cfg->lmPst.route                    = (Route)RTESPEC;
   cfg->lmPst.event                    = (Event)EVTNONE;
   cfg->lmPst.region                   = DFLT_REGION;
   cfg->lmPst.pool                     =  SZ_POOL;
   cfg->lmPst.selector                 = (Selector)UEHSM_SZSMSEL;

   /* Fill Header */
   szMgt.hdr.msgType                   = TCFG;
   szMgt.hdr.msgLen                    = 0;
   szMgt.hdr.entId.ent                 = ENTSZ;
   szMgt.hdr.entId.inst                = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt               = STSZGEN;
   szMgt.hdr.elmId.elmntInst1          = 0;
   szMgt.hdr.elmId.elmntInst2          = 0;
   szMgt.hdr.elmId.elmntInst3          = 0;
   szMgt.hdr.seqNmb                    = 0;
   szMgt.hdr.version                   = 0;
   szMgt.hdr.transId                   = 0;

   szMgt.hdr.response.prior            = PRIOR0;
   szMgt.hdr.response.route            = RTESPEC;
   szMgt.hdr.response.mem.region       = DFLT_REGION;
   szMgt.hdr.response.mem.pool         = SZ_POOL;
   szMgt.hdr.response.selector         = SM_SELECTOR_LC;

    /* Fill Pst */
   pst.selector                        = SM_SELECTOR_LC;
   pst.srcEnt                          = ENTSM;
   pst.dstEnt                          = ENTSZ;
   pst.dstProcId                       = SM_SZ_PROC;
   pst.srcProcId                       = SM_SM_PROC;

   logInfo("%s", "S1AP General Config Request sent");

   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&pst, &szMgt);
 
   RETVALUE(ROK);
}/* smBuildSzGenCfg */

/*
*
*       Fun:   smBuildSzLSapCfg
*
*       Desc:  Call handler for TSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_sz_cntrl.c
*
*/

PUBLIC S16 smBuildSzLSapCfg(
  SuId        suId,             /* S1AP Sap ID */
  SpId        spId              /* Service Provider Sap ID */
  )
{
   SzMngmt        szMgt;
   SzSctSapCfg    *cfg = NULLP;
   Pst            pst;
 
   logFunc("%s","smBuildSzLSapCfg");

   SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(szMgt.u.cfg.u.sctSap);

   cfg->suId                     = suId;
   cfg->spId                     = spId;

   cfg->selector                 = UEHSM_SZSBSEL;
   cfg->mem.region               = DFLT_REGION;
   cfg->mem.pool                 = SZ_POOL;
   cfg->priority                 = PRIOR0;
   cfg->route                    = RTESPEC;
   cfg->dstProcId                = SM_SB_PROC;
   cfg->dstEntId                 = ENTSB;
   cfg->dstInstId                = (Inst)0;

   cfg->tBndTmr.enb              = smCb.cfgCb.s1apConfig.bndTmCfgEnabled;
   cfg->tBndTmr.val              = smCb.cfgCb.s1apConfig.bndTmCfgVal;
   cfg->maxBndRetry              = smCb.cfgCb.s1apConfig.maxBndRetry;
   /* Fill Header */
   szMgt.hdr.msgType             = TCFG;
   szMgt.hdr.msgLen              = 0;
   szMgt.hdr.entId.ent           = ENTSZ;
   szMgt.hdr.entId.inst          = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt         = STSZSCTSAP;

   /* Fill Pst */
   pst.selector                  = SM_SELECTOR_LC;
   pst.srcEnt                    = ENTSM;
   pst.dstEnt                    = ENTSZ;
   pst.dstProcId                 = SM_SZ_PROC;
   pst.srcProcId                 = SM_SM_PROC;

   logInfo("S1AP: SCTP SAP Config Request sent for spId:%d suid:%d", spId, suId);

   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&pst, &szMgt);
 
   RETVALUE(ROK);
}/* smBuildSzLSapCfg */


/*
*
*       Fun:   smBuildSzUSapCfg
*
*       Desc:  Call handler for SSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_sz_cntrl.c
*
*/

PUBLIC S16 smBuildSzUSapCfg(
  SpId        spId              /* Service Provider Sap ID */
  )
{
   SzMngmt  szMgt;
   SzSztSapCfg    *cfg = NULLP;
   Pst      pst;
 
   logFunc("%s","smBuildSzUSapCfg");

   SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &(szMgt.u.cfg.u.sztSap);

   cfg->spId   = spId;
   cfg->selector                 = UEHSM_SZUEHSEL;
   cfg->mem.region               = DFLT_REGION;
   cfg->mem.pool                 = SZ_POOL;
   cfg->priority                 = PRIOR0;
   cfg->route                    = RTESPEC;

   /* Fill Header */
   szMgt.hdr.msgType             = TCFG;
   szMgt.hdr.msgLen              = 0;
   szMgt.hdr.entId.ent           = ENTSZ;
   szMgt.hdr.entId.inst          = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt         = STSZSZTSAP;

   /* Fill Pst */
   pst.selector                  = SM_SELECTOR_LC;
   pst.srcEnt                    = ENTSM;
   pst.dstEnt                    = ENTSZ;
   pst.dstProcId                 = SM_SZ_PROC;
   pst.srcProcId                 = SM_SM_PROC;
   
   logInfo("S1AP Upper SAP:UEH_SZ_SPID Config Request sent for SAP Id:%d", spId);

   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&pst, &szMgt);
 
   RETVALUE(ROK);
}/* smBuildSzUSapCfg */


/*
*
*       Fun:   smBuildSzProtCfg
*
*       Desc:  Call handler for Protocol Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_sz_cntrl.c
*
*/
PUBLIC S16 smBuildSzProtCfg(
  Void
  )
{
   SzMngmt        szMgt;
   SzProtCfg      *protCfg = NULLP;
   Pst            pst;
 
   logFunc("%s","smBuildSzProtCfg");

   SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   protCfg = &(szMgt.u.cfg.u.prot);

   protCfg->srcAddr.ipAddr.nmb                      =   1;
   protCfg->srcAddr.ipAddr.nAddr[0].type            =   CM_IPV4ADDR_TYPE;
   protCfg->srcAddr.ipAddr.nAddr[0].u.ipv4NetAddr   =   smCb.cfgCb.enbIpAddr;//smCfgCb.enbIpAddr;
   protCfg->nodeType                                =   LSZ_NODE_ENB;
   protCfg->u.eNbId                                 =   UEHSM_LSZ_ENB_PRC0;
   protCfg->srcAddr.sctPort                         =   smCb.cfgCb.s1apConfig.s1apLocalPort;

   /* Fill Header */
   szMgt.hdr.msgType             = TCFG;
   szMgt.hdr.msgLen              = 0;
   szMgt.hdr.entId.ent           = ENTSZ;
   szMgt.hdr.entId.inst          = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt         = STSZPROT;

   /* Fill Pst */
   pst.selector                  = SM_SELECTOR_LC;
   pst.srcEnt                    = ENTSM;
   pst.dstEnt                    = ENTSZ;
   pst.dstProcId                 = SM_SZ_PROC;
   pst.srcProcId                 = SM_SM_PROC;
 
   logInfo("%s", "S1AP protocol Config Request sent");

   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&pst, &szMgt);
 
   RETVALUE(ROK);
}/* smBuildSzProtCfg */

/*
*
*       Fun:   smBuildSzPeerCfg
*
*       Desc:  Call handler for Peer Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_sz_cntrl.c
*
*/
PUBLIC S16 smBuildSzPeerCfg
(
Void
)
{
   SzMngmt              szMgt;
   SzPeerCfg            *cfg = NULLP;
   U16                  cnt;
   Pst                  pst;
   MsgBsmS1apConfigReq  *s1apCfgInfo;

   logFunc("%s","smBuildSzPeerCfg");

   SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &(szMgt.u.cfg.u.peer);
  
   /* Save the number of peers to EnodeB,
    * Our PEER on S1AP is always a MME */
   cfg->nmbPeers                      = (U32)smCb.cfgCb.s1apConfig.nmbPeer;
   s1apCfgInfo                        = &( smCb.cfgCb.s1apCfgInfo);//smCfgCb.s1apCfgInfo);
   UEHSM_ALLOC(&cfg->peerInfo, sizeof(SzPeerInfo)* cfg->nmbPeers);

   for (cnt=0; cnt < cfg->nmbPeers; cnt++)
   {
      cfg->peerInfo[cnt].type         =  LSZ_NODE_MME;
      cfg->peerInfo[cnt].peerId.pres  =  TRUE;
      cfg->peerInfo[cnt].peerId.val   =  s1apCfgInfo->mmeConfig.index;
      cfg->peerInfo[cnt].uSapId       =  UEH_SZ_SPID;
      cfg->peerInfo[cnt].sapId        =  UEH_SZ_SUID;

      cfg->peerInfo[cnt].assocCfg.locOutStrms   =  s1apCfgInfo->mmeConfig.numSctpStreams;

      /* Fix Port and IP Addresses */
      cfg->peerInfo[cnt].assocCfg.dstPort                   =  UEHSM_LSZ_MME_SCTPORT;
      cfg->peerInfo[cnt].assocCfg.priDstAddr.type           =  CM_IPV4ADDR_TYPE;
      cfg->peerInfo[cnt].assocCfg.priDstAddr.u.ipv4NetAddr  =  smCb.cfgCb.mmeIpAddr;//smCfgCb.mmeIpAddr;
      cfg->peerInfo[cnt].assocCfg.dstAddrLst.nmb            =  1;
      cfg->peerInfo[cnt].assocCfg.dstAddrLst.nAddr[0].type  =  CM_IPV4ADDR_TYPE;
      cfg->peerInfo[cnt].assocCfg.dstAddrLst.nAddr[0].u.ipv4NetAddr   =  smCb.cfgCb.mmeIpAddr;//smCfgCb.mmeIpAddr;


      cfg->peerInfo[cnt].assocCfg.nmbRetry           =  smCb.cfgCb.s1apConfig.assocCfg_nmbRetry;
      cfg->peerInfo[cnt].assocCfg.tmrAssocRecov.enb  =  smCb.cfgCb.s1apConfig.assocCfg_tmrAssocRecov_enb;
      cfg->peerInfo[cnt].assocCfg.tmrAssocRecov.val  =  smCb.cfgCb.s1apConfig.assocCfg_tmrAssocRecov_val;
  }

   /* Fill Header */
   szMgt.hdr.msgType             = TCFG;
   szMgt.hdr.msgLen              = 0;
   szMgt.hdr.entId.ent           = ENTSZ;
   szMgt.hdr.entId.inst          = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt         = STSZPEER;

   /* Fill Pst */
   pst.selector                  = SM_SELECTOR_LC;
   pst.srcEnt                    = ENTSM;
   pst.dstEnt                    = ENTSZ;
   pst.dstProcId                 = SM_SZ_PROC;
   pst.srcProcId                 = SM_SM_PROC;
 
  logInfo ("%s", "S1AP Peer Cfg sent");

   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&pst, &szMgt);
 
   RETVALUE(ROK);
}/* smBuildSzPeerCfg */



/*
 *      FUN:   smBindSzToLSap
 *
 *      Desc:  Brings S1AP to the state before configuration
 *
 *      Ret:   void
 *
 *      Notes: None
 *
 *      File:  ueh_sm_sz_cntrl.c
 *
 *
 */
PUBLIC S16 smBindSzToLSap(
    void
  )
{
   SzMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;
   
   logFunc("%s","smBindSzToLSap");
   
   SM_SET_ZERO(&cntrl, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cntrl.u.cntrl.action          = ABND_ENA;
   cntrl.u.cntrl.subAction       = 0;
   cntrl.u.cntrl.u.sap.id        = UEH_SZ_SUID;

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTSZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STSZSCTSAP;
   cntrl.hdr.transId             = SM_S1AP_LSAPBND_TRANS_ID; /*SM_S1AP_PEERDEL_TRANS_ID;*/

   /* Fill Pst */
   pst.selector                  = SM_SELECTOR_LC;
   pst.srcEnt                    = ENTSM;
   pst.dstEnt                    = ENTSZ;
   pst.dstProcId                 = SM_SZ_PROC;
   pst.srcProcId                 = SM_SM_PROC;

   logInfo("%s","S1AP Lower SAP:STSZSCTSAP Bind Request sent to SCTP");

    /* Send the control request to the S1AP */
   (Void)SmMiLszCntrlReq(&pst, &cntrl);

   RETVALUE(ret);
} /* end of smBindSzToLSap */

/*
*
*       Fun:   smBuildSzGenCntrl
*
*       Desc:  Invoked to Enable Alarms at S1AP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_sz_cntrl.c
*
*/
PUBLIC S16 smBuildSzAlarmCntrl(
     void
  )
{
   SzMngmt cntrl;
   Pst      pst;
   
   logFunc("%s","smBuildSzAlarmCntrl");
   
   SM_SET_ZERO(&cntrl, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl.u.cntrl.action          =  AENA;
   cntrl.u.cntrl.u.sap.id        =  0;
   cntrl.u.cntrl.subAction       =  SAUSTA;

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTSZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STSZGEN;
   cntrl.hdr.transId             = SM_S1AP_ALARM_TRANS_ID;
   
   /* Fill Pst */
   pst.selector                  = SM_SELECTOR_LC;
   pst.srcEnt                    = ENTSM;
   pst.dstEnt                    = ENTSZ;
   pst.dstProcId                 = SM_SZ_PROC;
   pst.srcProcId                 = SM_SM_PROC;
   
   logInfo("%s", "Alarm control request sent to S1AP(SZ)");

   /* Send the request to the S1AP */
   SmMiLszCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildSzGenCntrl */

/*
*
*       Fun:   smBuildSzDbgCntrl
*
*       Desc:  Invoked to Enable Debugs at S1AP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_sz_cntrl.c
*
*/
PUBLIC S16 smBuildSzDbgCntrl(
    LogConfig *logConfig
  )
{
   SzMngmt       cntrl;
   Pst           pst;
   U32           szdbgMask = 0;
   logFunc("%s","smBuildSzDbgCntrl");
   
   SM_SET_ZERO(&cntrl, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if((logConfig->logMask1 & LOG_MASK_TIMER ) || (logConfig->logMask1 & LOG_MASK_TIMEOUT ) )
   {
      szdbgMask = szdbgMask | S1AP_DBG_TMR;
   }
   if(logConfig->logMask1 & LOG_MASK_DEBUG )
   {
      szdbgMask = szdbgMask | S1AP_DBG_INFO;
   }
    logInfo(":%s: DBG-Mask: %02x",__func__, szdbgMask);
   cntrl.u.cntrl.action        =  ADISIMM;
   cntrl.u.cntrl.subAction     =  SADBG;
   cntrl.u.cntrl.u.dbg.dbgMask =  0xffffffff;

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTSZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STSZGEN;
   cntrl.hdr.transId             = SM_S1AP_DEBUG_TRANS_ID;

   /* Fill Pst */
   pst.selector                  = SM_SELECTOR_LC;
   pst.srcEnt                    = ENTSM;
   pst.dstEnt                    = ENTSZ;
   pst.dstProcId                 = SM_SZ_PROC;
   pst.srcProcId                 = SM_SM_PROC;
   
   logInfo("%s", "Debug control request sent to S1AP(SZ)");

   /* Sendig msg to S1AP module for resetting the log level */
   SmMiLszCntrlReq(&pst, &cntrl);

   cntrl.u.cntrl.action        =  AENA;
   cntrl.u.cntrl.u.dbg.dbgMask =  szdbgMask;
   /* Sendig msg to S1AP module for setting the actual log level */
   SmMiLszCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildSzGenCntrl */
