/********************************************************************16**

                         (c) COPYRIGHT 2009 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License 
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  CCPU also warrants 
     that has the corporate authority to enter into and perform under the   
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
     nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the use set
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9450, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/

/**********************************************************************

     Name:     LTE eNodeB Stack Manager 
  
     Type:     C Source file
  
     Desc:     Stack Manager Initialization Functions.

     File:     ve_sm_ve_cntrl.c

     Sid:      ve_sm_ve_cntrl.c@@/main/2 - Wed Dec  1 08:44:39 2010

     Prg:      an 

**********************************************************************/

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_inet.h"       /* common INET defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport defines */

#include "sct.h"           /* SB (SCTP) layer */
#include "lve.h"           /* layer management defines for APP */

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#include "hi.h"
#endif

#ifdef    SB
#include "lsb.h"
#endif    /* SB */

#include "ve_sm_init.h"
#include "ve.h"

/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_inet.x"       /* common INET */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_tpt.x"        /* common transport types */

#include "sct.x"           /* SCTP Interface Structures */
#include "lve.x"           /* APP types */

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#include "hi.x"
#endif

#ifdef    SB
#include "lsb.x"
#endif    /* SB */

#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif

/*
*
*       Fun:   Initialize RRC App task.
*
*       Desc:  Invoked to create RRC App TAPA task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_ve_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smVeInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smVeInit(sysTskId)
SSTskId    sysTskId;
#endif
{

   TRC2(smVeInit)

   /* Register RRC APP TAPA Task */
   if (SRegTTsk((Ent)ENTVE, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
                 veActvInit, (ActvTsk)veActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Register RRC APP TAPA Task */
   if (SRegTTsk((Ent)ENTNX, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
                 NULLP, (ActvTsk)veActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

    /* Register Data APP TAPA Task */
   if (SRegTTsk((Ent)ENTEU, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
                 NULLP , (ActvTsk)veActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

    /* Register Data APP TAPA Task */
   if (SRegTTsk((Ent)ENTPX, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
                 NULLP , (ActvTsk)veActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Attach RRC APP TAPA Task */
   if (SAttachTTsk((Ent)ENTVE, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Attach RRC APP TAPA Task */
   if (SAttachTTsk((Ent)ENTNX, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   }


   /* Attach RRC APP TAPA Task */
   if (SAttachTTsk((Ent)ENTEU, (Inst)0, sysTskId)!= ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Attach RRC APP TAPA Task */
   if (SAttachTTsk((Ent)ENTPX, (Inst)0, sysTskId)!= ROK)
   {
      RETVALUE(RFAILED);
   }

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC App Tapa Task successfully \
registered and attached to %d\n",sysTskId));
#endif

   RETVALUE(ROK);
} /* end of smVeInit */

#ifdef EU_DAT_APP

/*
*
*       Fun:   Initialize Data App task.
*
*       Desc:  Invoked to create Data App TAPA task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_ve_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smEuInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smEuInit(sysTskId)
SSTskId    sysTskId;
#endif
{

   TRC2(smEuInit)

   RETVALUE(ROK);
} /* end of smEuInit */

#endif /* EU_DAT_APP */

/*
 *
 *       Fun:    veSmVeCfg - configure VE 
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ve_sm_sz_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void veSmVeCfg
(
Void
)
#else
PUBLIC Void veSmVeCfg(Void)
#endif /* ANSI */
{
   TRC2(veSmVeCfg)

   smBuildVeGenCfg();
   smBuildVeCtfLSapCfg(VE_YS_SUID, VE_YS_SPID);
   smBuildVeNhuLSapCfg(VE_NH_SUID, VE_NH_SPID);
#ifdef EU_DAT_APP
   smBuildVePjuLSapCfg(VE_PJ_SUID, VE_PJ_SPID);
   smBuildVeEgtLSapCfg(VE_EG_SUID, VE_EG_SPID);
#endif
   smBuildVeSztLSapCfg(VE_SZ_SUID, VE_SZ_SPID);

#ifdef LTE_HO_SUPPORT
   smBuildVeCztLSapCfg(VE_CZ_SUID, VE_CZ_SPID);
   smBuildVeNghCfg(); 
#endif
   smBuildVeRgrLSapCfg(VE_RG_SUID, VE_RG_SPID);

   RETVOID;
} /* end of veSmVeCfg */



/*
*
*       Fun:   smBuildVeGenCfg
*
*       Desc:  fill in genCfg for VE
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_ve_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildVeGenCfg
(
Void
)
#else
PUBLIC S16 smBuildVeGenCfg(Void)
#endif
{
   VeMngmt  veMgt;
   VeGenCfg    *cfg = NULLP;
   Pst       pst;
#ifdef LTE_HO_SUPPORT
   U16 loopCnt = 0;
#endif

   TRC2(smBuildVeGenCfg)

   SM_SET_ZERO(&veMgt, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(veMgt.u.cfg.u.genCfg);

   /*----------- Fill General Configuration Parameters ---------*/
#ifdef VE_RELAY
   /* CELL Configuration  */
   cfg->cellCfg.cellId       =  smCfgCb.cellId;
   cfg->cellCfg.modType      =  smCfgCb.modType;
   cfg->cellCfg.duplexMode   =  smCfgCb.duplexMode;
   cfg->cellCfg.maxUeSupp    =  smCfgCb.maxUeSupp;
   cfg->cellCfg.mcc[0].pres  =  smCfgCb.mcc[0].pres;
   cfg->cellCfg.mcc[0].val   =  smCfgCb.mcc[0].val;
   cfg->cellCfg.mcc[1].pres  =  smCfgCb.mcc[1].pres;
   cfg->cellCfg.mcc[1].val   =  smCfgCb.mcc[1].val;
   cfg->cellCfg.mcc[2].pres  =  smCfgCb.mcc[2].pres;
   cfg->cellCfg.mcc[2].val   =  smCfgCb.mcc[2].val;
   cfg->cellCfg.mnc[0].pres  =  smCfgCb.mnc[0].pres;
   /* CR ID- ccpu00116764-IOT changes*/ 
   cfg->cellCfg.mnc[0].val   =  smCfgCb.mnc[0].val;
   cfg->cellCfg.mnc[1].pres  =  smCfgCb.mnc[1].pres;
   /* CR ID- ccpu00116764-IOT changes*/ 
   cfg->cellCfg.mnc[1].val   =  smCfgCb.mnc[1].val;
   cfg->cellCfg.mnc[2].pres  =  smCfgCb.mnc[2].pres;
   cfg->cellCfg.mnc[2].val   =  smCfgCb.mnc[2].val;
   cfg->cellCfg.trackAreaCode=  smCfgCb.trackAreaCode;
   cfg->cellCfg.freqBandInd  =  smCfgCb.freqBandInd;
   cfg->cellCfg.macRntiStart = smCfgCb.macRntiStart;
   cfg->cellCfg.maxMacRntis = smCfgCb.maxMacRntis;
   cfg->cellCfg.numOfMacPrm = smCfgCb.numOfMacPrm;
   cfg->cellCfg.maxRrmRntis = smCfgCb.maxRrmRntis;
   cfg->cellCfg.rrmPrmblStart = smCfgCb.rrmDedPrmStart;
   cfg->cellCfg.maxRrmPrmbls = smCfgCb.rrmNumDedPrm;
   cfg->eNBType = smCfgCb.eNBType;
#ifdef LTE_HO_SUPPORT
   cfg->cellCfg.rsrpTrshldRange = smCfgCb.rsrpTrshldRange;
   /* Filling FDD configurations */
   cfg->cellCfg.fddCfg.ulEarfcn = smCfgCb.fddCb.ulEarfcn;
   cfg->cellCfg.fddCfg.dlEarfcn = smCfgCb.fddCb.dlEarfcn;
   cfg->cellCfg.fddCfg.ulTxBndWdth = smCfgCb.fddCb.ulTxBndWdth;
   cfg->cellCfg.fddCfg.dlTxBndWdth = smCfgCb.fddCb.dlTxBndWdth;
   /* Filling TDD configurations */
   cfg->cellCfg.tddCfg.earfcn = smCfgCb.tddCb.earfcn;
   cfg->cellCfg.tddCfg.txBndWdth = smCfgCb.tddCb.txBndWdth;
   cfg->cellCfg.tddCfg.sfAssgnmnt = smCfgCb.tddCb.sfAssgnmnt;
   cfg->cellCfg.tddCfg.splSfPatrn = smCfgCb.tddCb.splSfPatrn;
   cfg->cellCfg.tddCfg.dlCycPrefix = smCfgCb.tddCb.dlCycPrefix;
   cfg->cellCfg.tddCfg.ulCycPrefix = smCfgCb.tddCb.ulCycPrefix;
   /* Filling PLMN configuration */
   cfg->cellCfg.noOfBrdCstPlmn = smCfgCb.noOfBrdCstPlmn;
   for (loopCnt = 0; loopCnt < smCfgCb.noOfBrdCstPlmn; loopCnt++)
   {
      strcpy((S8 *)cfg->cellCfg.plmnId[loopCnt], (const S8 *)smCfgCb.plmnId[loopCnt]);
   }
   /* Filling the ENB Type */
   cfg->noOfNghInfo = smCfgCb.noOfNghInfo;
   cfg->nghInfo = (VeNghInfo **)smCfgCb.nghInfo;
   cfg->noOfguGrp = smCfgCb.noOfguGrp;
   cfg->guGrpId = (VeGuGrpId**) smCfgCb.guGrpId;

   /* Filling S1 Handover Timers */
   cfg->s1PrepTimerVal = smCfgCb.s1PrepTimerVal;
   cfg->s1OvrAllTimerVal = smCfgCb.s1OvrAllTimerVal;

   /* Filling UE HO Attach Timer */
   cfg->ueHoAttachTimerVal = smCfgCb.ueHoAttachTimerVal;

   /* Filling Physical Cell Id */
   cfg->cellCfg.physCellId = smCfgCb.pci;
#endif

   /* Copying PHY configuration parameters from cfg file to sm */
   cfg->cellCfg.opMode  =  smCfgCb.opMode;
   cfg->cellCfg.counter =  smCfgCb.counter;
   cfg->cellCfg.period  =  smCfgCb.period;
#endif /* end of VE_RELAY */

   cfg->dataAppAddr.type                     = CM_TPTADDR_IPV4;
   cfg->dataAppAddr.u.ipv4TptAddr.port       = VE_DFLT_EGTP_PORT;
   cfg->dataAppAddr.u.ipv4TptAddr.address = smCfgCb.enbIpAddr;

   cfg->lmPst.srcProcId = SM_SM_PROC;
   cfg->lmPst.dstProcId = SM_VE_PROC;
   cfg->lmPst.srcEnt = (Ent)ENTVE;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;
   cfg->lmPst.prior = (Prior)VESM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = smCb.init.region;
   cfg->lmPst.pool   =  VE_POOL;
   cfg->lmPst.selector = (Selector)VESM_VESMSEL;

   /* Fill Header */
   veMgt.hdr.msgType             = TCFG;
   veMgt.hdr.msgLen              = 0;
   veMgt.hdr.entId.ent           = ENTVE;
   veMgt.hdr.entId.inst          = SM_INST_ZERO;
   veMgt.hdr.elmId.elmnt         = STVEGEN;
   veMgt.hdr.elmId.elmntInst1    = 0;
   veMgt.hdr.elmId.elmntInst2    = 0;
   veMgt.hdr.elmId.elmntInst3    = 0;
   veMgt.hdr.seqNmb              = 0;
   veMgt.hdr.version             = 0;
   veMgt.hdr.transId             = 0;

   veMgt.hdr.response.prior      = PRIOR0;
   veMgt.hdr.response.route      = RTESPEC;
   veMgt.hdr.response.mem.region = smCb.init.region;
   veMgt.hdr.response.mem.pool   = VE_POOL;
   veMgt.hdr.response.selector   = SM_SELECTOR_LC;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Gen Cfg sent\n"));
#endif
   /* Send the request to the LM */
   (Void) SmMiLveCfgReq(&pst, &veMgt);
 
   RETVALUE(ROK);
}/* smBuildVeGenCfg */

/*
*
*       Fun:   smBuildVeCtfLSapCfg
*
*       Desc:  Call handler for CTF LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_ve_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildVeCtfLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildVeCtfLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   VeMngmt    veMgt;
   VeLSapCfg  *cfg = NULLP;
   Pst        pst;
 
   TRC2(smBuildVeCtfLSapCfg)

   SM_SET_ZERO(&veMgt, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(veMgt.u.cfg.u.lSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_YS_PROC;
#ifdef LTE_ENB_PAL
   cfg->dstEnt       = ENTTF;
#else   
   cfg->dstEnt       = ENTYS;
#endif /* LTE_ENB_PAL */   
   cfg->dstInst      = (Inst)0;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = VESM_VEYSSEL;
   cfg->srcEnt       = ENTVE;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = smCb.init.region;
   cfg->mem.pool     = VE_POOL;

   cfg->maxBndRetry = VESM_LVE_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = VESM_LVE_VAL_10;

   /* Fill Header */
   veMgt.hdr.msgType             = TCFG;
   veMgt.hdr.msgLen              = 0;
   veMgt.hdr.entId.ent           = ENTVE;
   veMgt.hdr.entId.inst          = SM_INST_ZERO;
   veMgt.hdr.elmId.elmnt         = STVECTFSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Ctf LSap Cfg sent\n"));
#endif

   /* Send the request to the LM */
   (Void) SmMiLveCfgReq(&pst, &veMgt);
 
   RETVALUE(ROK);
}/* smBuildVeCtfLSapCfg */

/*
*
*       Fun:   smBuildVeNhuLSapCfg
*
*       Desc:  Call handler for NHU LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_ve_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildVeNhuLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildVeNhuLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   VeMngmt    veMgt;
   VeLSapCfg  *cfg = NULLP;
   Pst        pst;
 
   TRC2(smBuildVeNhuLSapCfg)

   SM_SET_ZERO(&veMgt, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(veMgt.u.cfg.u.lSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_NH_PROC;
   cfg->dstEnt       = ENTNH;
   cfg->dstInst      = (Inst)0;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = VESM_VENHSEL;
   cfg->srcEnt       = ENTNX;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = smCb.init.region;
   cfg->mem.pool     = VE_POOL;

   cfg->maxBndRetry = VESM_LVE_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = VESM_LVE_VAL_10;

   /* Fill Header */
   veMgt.hdr.msgType             = TCFG;
   veMgt.hdr.msgLen              = 0;
   veMgt.hdr.entId.ent           = ENTVE;
   veMgt.hdr.entId.inst          = SM_INST_ZERO;
   veMgt.hdr.elmId.elmnt         = STVENHUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Nhu LSap Cfg sent \n"));
#endif
   /* Send the request to the LM */
   (Void) SmMiLveCfgReq(&pst, &veMgt);
 
   RETVALUE(ROK);
}/* smBuildVeNhuLSapCfg */

#ifdef EU_DAT_APP

/*
*
*       Fun:   smBuildVePjuLSapCfg
*
*       Desc:  Call handler for PJU LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_ve_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildVePjuLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildVePjuLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   VeMngmt    veMgt;
   VeLSapCfg  *cfg = NULLP;
   Pst        pst;
 
   TRC2(smBuildVePjuLSapCfg)

   SM_SET_ZERO(&veMgt, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(veMgt.u.cfg.u.lSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_PJ_PROC;
   /* Shwetha - adding PDCP's entity Id instead of RLC's for data application's
      lower SAP */
   /* cfg->dstEnt       = ENTKW; */
   cfg->dstEnt       = ENTPJ;
   cfg->dstInst      = (Inst)0;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = VESM_VEPJSEL;
   cfg->srcEnt       = ENTPX;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = smCb.init.region;
   cfg->mem.pool     = VE_POOL;

   cfg->maxBndRetry = VESM_LVE_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = VESM_LVE_VAL_10;

   /* Fill Header */
   veMgt.hdr.msgType             = TCFG;
   veMgt.hdr.msgLen              = 0;
   veMgt.hdr.entId.ent           = ENTVE;
   veMgt.hdr.entId.inst          = SM_INST_ZERO;
   veMgt.hdr.elmId.elmnt         = STVEPJUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Nhu LSap Cfg sent\n"));
#endif
   /* Send the request to the LM */
   (Void) SmMiLveCfgReq(&pst, &veMgt);
 
   RETVALUE(ROK);
}/* smBuildVePjuLSapCfg */

#endif  /* EU_DAT_APP */

/*
*
*       Fun:   smBuildVeRgrLSapCfg
*
*       Desc:  Call handler for RGR LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_ve_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildVeRgrLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildVeRgrLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   VeMngmt    veMgt;
   VeLSapCfg  *cfg = NULLP;
   Pst        pst;
 
   TRC2(smBuildVeRgrLSapCfg)

   SM_SET_ZERO(&veMgt, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(veMgt.u.cfg.u.lSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_RG_PROC;
   cfg->dstEnt       = ENTRG;
   cfg->dstInst      = (Inst)SCH_INST_ID;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = VESM_VERGSEL;
   cfg->srcEnt       = ENTNX;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = smCb.init.region;
   cfg->mem.pool     = VE_POOL;

   cfg->maxBndRetry = VESM_LVE_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = VESM_LVE_VAL_10;

   /* Fill Header */
   veMgt.hdr.msgType             = TCFG;
   veMgt.hdr.msgLen              = 0;
   veMgt.hdr.entId.ent           = ENTVE;
   veMgt.hdr.entId.inst          = SM_INST_ZERO;
   veMgt.hdr.elmId.elmnt         = STVERGRSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Rgr LSap Cfg sent\n"));
#endif
   /* Send the request to the LM */
   (Void) SmMiLveCfgReq(&pst, &veMgt);
 
   RETVALUE(ROK);
}/* smBuildVeRgrLSapCfg */


/*
*
*       Fun:   smBuildVeSztLSapCfg
*
*       Desc:  Call handler for SZT LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_ve_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildVeSztLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildVeSztLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   VeMngmt    veMgt;
   VeLSapCfg  *cfg = NULLP;
   Pst        pst;
 
   TRC2(smBuildVeSztLSapCfg)

   SM_SET_ZERO(&veMgt, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(veMgt.u.cfg.u.lSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_SZ_PROC;
   cfg->dstEnt       = ENTSZ;
   cfg->dstInst      = (Inst)0;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = VESM_VESZSEL;
   cfg->srcEnt       = ENTVE;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = smCb.init.region;
   cfg->mem.pool     = VE_POOL;

   cfg->maxBndRetry = VESM_LVE_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = VESM_LVE_VAL_10;

   /* Fill Header */
   veMgt.hdr.msgType             = TCFG;
   veMgt.hdr.msgLen              = 0;
   veMgt.hdr.entId.ent           = ENTVE;
   veMgt.hdr.entId.inst          = SM_INST_ZERO;
   veMgt.hdr.elmId.elmnt         = STVESZTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Szt LSap Cfg sent\n"));
#endif
   /* Send the request to the LM */
   (Void) SmMiLveCfgReq(&pst, &veMgt);
 
   RETVALUE(ROK);
}/* smBuildVeSztLSapCfg */

#ifdef LTE_HO_SUPPORT
/*
*
*       Fun:   smBuildVeCztLSapCfg
*
*       Desc:  Call handler for CZT LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_ve_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildVeCztLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildVeCztLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   VeMngmt    veMgt;
   VeLSapCfg  *cfg = NULLP;
   Pst        pst;

   TRC2(smBuildVeCztLSapCfg)

   SM_SET_ZERO(&veMgt, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(veMgt.u.cfg.u.lSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_CZ_PROC;
   cfg->dstEnt       = ENTCZ;
   cfg->dstInst      = (Inst)0;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = VESM_VECZSEL;
   cfg->srcEnt       = ENTVE;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = smCb.init.region;
   cfg->mem.pool     = VE_POOL;

   cfg->maxBndRetry = VESM_LVE_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = VESM_LVE_VAL_10;

   /* Fill Header */
   veMgt.hdr.msgType             = TCFG;
   veMgt.hdr.msgLen              = 0;
   veMgt.hdr.entId.ent           = ENTVE;
   veMgt.hdr.entId.inst          = SM_INST_ZERO;
   veMgt.hdr.elmId.elmnt         = STVECZTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Czt LSap Cfg sent\n"));
#endif
   /* Send the request to the LM */
   (Void) SmMiLveCfgReq(&pst, &veMgt);

   RETVALUE(ROK);
}/* smBuildVeCztLSapCfg */

/*
*
*       Fun:   smBuildVeNghCfg
*
*       Desc:  fill in NghCfg for VE
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_ve_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildVeNghCfg
(
Void
)
#else
PUBLIC S16 smBuildVeNghCfg(Void)
#endif
{
   VeMngmt  veMgt;
   VeNghCellCfgLst  *cfg = NULLP;
   Pst       pst;

   TRC2(smBuildVeNghCfg)

   SM_SET_ZERO(&veMgt, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(veMgt.u.cfg.u.NghCellCfgLst);

   /* Neighbor Cell Info  Configuration  */
   cfg->noOfCfg = smCfgCb.noOfNghCfg;
   /* Assign the neighbor cell Config list which is already made 
      while reading the config file and being kept in smCfgCb*/
   cfg->NghCellCfg = smCfgCb.veNghCellCfg; 

   cfg->lmPst.srcProcId = SM_SM_PROC;
   cfg->lmPst.dstProcId = SM_VE_PROC;
   cfg->lmPst.srcEnt = (Ent)ENTVE;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;
   cfg->lmPst.prior = (Prior)VESM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = smCb.init.region;
   cfg->lmPst.pool   =  VE_POOL;
   cfg->lmPst.selector = (Selector)VESM_VESMSEL;

   /* Fill Header */
   veMgt.hdr.msgType             = TCFG;
   veMgt.hdr.msgLen              = 0;
   veMgt.hdr.entId.ent           = ENTVE;
   veMgt.hdr.entId.inst          = SM_INST_ZERO;
   veMgt.hdr.elmId.elmnt         = STVENGH;
   veMgt.hdr.elmId.elmntInst1    = 0;
   veMgt.hdr.elmId.elmntInst2    = 0;
   veMgt.hdr.elmId.elmntInst3    = 0;
   veMgt.hdr.seqNmb              = 0;
   veMgt.hdr.version             = 0;
   veMgt.hdr.transId             = 0;

   veMgt.hdr.response.prior      = PRIOR0;
   veMgt.hdr.response.route      = RTESPEC;
   veMgt.hdr.response.mem.region = smCb.init.region;
   veMgt.hdr.response.mem.pool   = VE_POOL;
   veMgt.hdr.response.selector   = SM_SELECTOR_LC;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM NghCell Cfg sent\n"));
#endif
   /* Send the request to the LM */
   (Void) SmMiLveCfgReq(&pst, &veMgt);
 
   RETVALUE(ROK);
}

#endif

#ifdef EU_DAT_APP

/*
*
*       Fun:   smBuildVeEgtLSapCfg
*
*       Desc:  Call handler for EGT LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_ve_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildVeEgtLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildVeEgtLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   VeMngmt    veMgt;
   VeLSapCfg  *cfg = NULLP;
   Pst        pst;
 
   TRC2(smBuildVeEgtLSapCfg)

   SM_SET_ZERO(&veMgt, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(veMgt.u.cfg.u.lSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_EG_PROC;
   cfg->dstEnt       = ENTEG;
   cfg->dstInst      = (Inst)0;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = VESM_VEEGSEL;
   cfg->srcEnt       = ENTVE;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = smCb.init.region;
   cfg->mem.pool     = VE_POOL;

   cfg->maxBndRetry = VESM_LVE_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = VESM_LVE_VAL_10;

   /* Fill Header */
   veMgt.hdr.msgType             = TCFG;
   veMgt.hdr.msgLen              = 0;
   veMgt.hdr.entId.ent           = ENTVE;
   veMgt.hdr.entId.inst          = SM_INST_ZERO;
   veMgt.hdr.elmId.elmnt         = STVEEGTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Egt LSap Cfg sent\n"));
#endif
   /* Send the request to the LM */
   (Void) SmMiLveCfgReq(&pst, &veMgt);
 
   RETVALUE(ROK);
}/* smBuildVeEgtLSapCfg */

#endif  /* EU_DAT_APP  */

/*
 *      FUN:   smBindVeToSztSap
 *
 *      Desc:  Bind VE with SCT LSAP. 
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  ve_sm_ve_cntrl.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindVeToSztSap
(
Void
)
#else
PUBLIC S16 smBindVeToSztSap(Void)
#endif /* ANSI */
{
   VeMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;
   
   TRC2(smBindVeToSztSap)

   SM_SET_ZERO(&cntrl, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cntrl.u.cntrl.action        = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;
   cntrl.u.cntrl.u.sapId       = VE_SZ_SUID; 

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTVE;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STVESZTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Bnd Cntrl Req to ve for S1AP\n"));
#endif
    /* Send the control request to the APP */
   (Void)SmMiLveCntrlReq(&pst, &cntrl);

   RETVALUE(ret);
} /* smBindVeToSztSap */

#ifdef LTE_HO_SUPPORT
/*
 *      FUN:   smBindVeToCztSap
 *
 *      Desc:  Bind VE with SCT LSAP. 
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  ve_sm_ve_cntrl.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindVeToCztSap
(
Void
)
#else
PUBLIC S16 smBindVeToCztSap(Void)
#endif /* ANSI */
{
   VeMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;

   TRC2(smBindVeToCztSap)

   SM_SET_ZERO(&cntrl, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl.u.cntrl.action        = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;
   cntrl.u.cntrl.u.sapId       = VE_CZ_SUID;

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTVE;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STVECZTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Bnd Cntrl Req to ve for X2AP\n"));
#endif
    /* Send the control request to the APP */
   (Void)SmMiLveCntrlReq(&pst, &cntrl);

   RETVALUE(ret);
} /* smBindVeToCztSap */
#endif

#ifdef EU_DAT_APP

/*
 *      FUN:   smBindVeToEgtSap
 *
 *      Desc:  Bind VE with EGT LSAP. 
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  ve_sm_ve_cntrl.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindVeToEgtSap
(
Void
)
#else
PUBLIC S16 smBindVeToEgtSap(Void)
#endif /* ANSI */
{
   VeMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;
   
   TRC2(smBindVeToEgtSap)

   SM_SET_ZERO(&cntrl, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cntrl.u.cntrl.action        = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;
   cntrl.u.cntrl.u.sapId       = VE_EG_SUID; 

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTVE;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STVEEGTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Bnd Cntrl Req to ve for EGTP\n"));
#endif
    /* Send the control request to the VE*/
   (Void)SmMiLveCntrlReq(&pst, &cntrl);

   RETVALUE(ret);
} /* smBindVeToEgtSap */

#endif /* EU_DAT_APP */

/*
 *      FUN:   smBindVeToNhuSap
 *
 *      Desc:  Bind VE with NHU LSAP. 
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  ve_sm_ve_cntrl.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindVeToNhuSap
(
Void
)
#else
PUBLIC S16 smBindVeToNhuSap(Void)
#endif /* ANSI */
{
   VeMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;
   
   TRC2(smBindVeToNhuSap)

   SM_SET_ZERO(&cntrl, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cntrl.u.cntrl.action        = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;
   cntrl.u.cntrl.u.sapId       = VE_NH_SUID; 

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTVE;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STVENHUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Bnd Cntrl Req to ve for RRC\n"));
#endif
    /* Send the control request to the VE*/
   (Void)SmMiLveCntrlReq(&pst, &cntrl);

   RETVALUE(ret);
} /* smBindVeToNhuSap */

/*
 *      FUN:   smBindVeToCtfSap
 *
 *      Desc:  Bind VE with CTF LSAP. 
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  ve_sm_ve_cntrl.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindVeToCtfSap
(
Void
)
#else
PUBLIC S16 smBindVeToCtfSap(Void)
#endif /* ANSI */
{
   VeMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;
   
   TRC2(smBindVeToCtfSap)

   SM_SET_ZERO(&cntrl, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cntrl.u.cntrl.action        = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;
   cntrl.u.cntrl.u.sapId       = VE_YS_SUID; 

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTVE;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STVECTFSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Bnd Cntrl Req to ve for Ctf\n"));
#endif
    /* Send the control request to the VE*/
   (Void)SmMiLveCntrlReq(&pst, &cntrl);

   RETVALUE(ret);
} /* smBindVeToCtfSap */

#ifdef EU_DAT_APP

/*
 *      FUN:   smBindVeToPjuSap
 *
 *      Desc:  Bind VE with PJU LSAP. 
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  ve_sm_ve_cntrl.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindVeToPjuSap
(
Void
)
#else
PUBLIC S16 smBindVeToPjuSap(Void)
#endif /* ANSI */
{
   VeMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;
   
   TRC2(smBindVeToPjuSap)

   SM_SET_ZERO(&cntrl, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cntrl.u.cntrl.action        = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;
   cntrl.u.cntrl.u.sapId       = VE_PJ_SUID; 

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTVE;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STVEPJUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Bnd Cntrl Req to ve for RLC/PDCP\n"));
#endif
    /* Send the control request to the VE*/
   (Void)SmMiLveCntrlReq(&pst, &cntrl);

   RETVALUE(ret);
} /* smBindVeToNhuSap */

#endif /* EU_DAT_APP */
/*
 *      FUN:   smBindVeToRgrSap
 *
 *      Desc:  Bind VE with RGR LSAP. 
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  ve_sm_ve_cntrl.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindVeToRgrSap
(
Void
)
#else
PUBLIC S16 smBindVeToRgrSap(Void)
#endif /* ANSI */
{
   VeMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;
   
   TRC2(smBindVeToRgrSap)

   SM_SET_ZERO(&cntrl, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cntrl.u.cntrl.action        = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;
   cntrl.u.cntrl.u.sapId       = VE_RG_SUID; 

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTVE;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STVERGRSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Bnd Cntrl Req to ve for SCH\n"));
#endif
    /* Send the control request to the VE*/
   (Void)SmMiLveCntrlReq(&pst, &cntrl);

   RETVALUE(ret);
} /* smBindVeToRgrSap */



/*
*
*       Fun:   smBuildVeGenCntrl
*
*       Desc:  Invoked to Enable Alarms at APP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_ve_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildVeGenCntrl
(
)
#else
PUBLIC S16 smBuildVeGenCntrl(Void)
#endif
{
   VeMngmt cntrl;
   Pst      pst;
   
   /* Corrected function name in trace macro */
   TRC2(smBuildVeGenCntrl)

   SM_SET_ZERO(&cntrl, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl.u.cntrl.action        =  AENA;
   cntrl.u.cntrl.u.sapId       =  0;
   cntrl.u.cntrl.subAction     =  SAUSTA;

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTVE;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STVEGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Gen Cntrl Req to ve sent\n"));
#endif
   /* Send the request to the APP */
   SmMiLveCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildVeGenCntrl */

/*
*
*       Fun:   smBuildVeCellCntrl
*
*       Desc:  Invoked to intimate the APP that Binding is DONE
*              and initiate cell control
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_ve_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildVeCellCntrl
(
)
#else
PUBLIC S16 smBuildVeCellCntrl(Void)
#endif
{
   VeMngmt cntrl;
   Pst      pst;
   
   /* Corrected function name in trace macro */
   TRC2(smBuildVeCellCntrl)

   SM_SET_ZERO(&cntrl, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTVE;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STVECELLCFG;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Cell Cntrl Req to ve\n"));
#endif
   /* Send the request to the APP */
   SmMiLveCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildVeCellCntrl */

/*
*
*       Fun:   smBuildVeS1InitCntrl
*
*       Desc:  Invoked to intimate the APP that Binding is DONE.
*              APP should initiate the S1AP association
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_ve_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildVeS1InitCntrl
(
)
#else
PUBLIC S16 smBuildVeS1InitCntrl(Void)
#endif
{
   VeMngmt cntrl;
   Pst      pst;
   U16     cnt;
   
   TRC2(smBuildVeS1InitCntrl)

#if (defined(VE_PERF_MEAS) || defined(VE_WITHOUT_CNE))
   smBuildVeCellCntrl();
   smCb.smState = VE_SM_STATE_AWAIT_CELL_UP;
   RETVALUE(ROK);
#endif


   SM_SET_ZERO(&cntrl, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTVE;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STVES1CONSETUP;

   cntrl.u.cntrl.u.s1Setup.numPeers = smCfgCb.numMmes;

   VESM_ALLOC(&cntrl.u.cntrl.u.s1Setup.peerId,
              (sizeof(U32) * cntrl.u.cntrl.u.s1Setup.numPeers));
   if(cntrl.u.cntrl.u.s1Setup.peerId == NULLP)
   {
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Failure in the allocation of configuration info\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */



   for(cnt = 0; cnt < smCfgCb.numMmes; cnt++)
   {
      cntrl.u.cntrl.u.s1Setup.peerId[cnt] = smCfgCb.mmeIdx[cnt];
   }

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Cntrl Req to ve for S1 Init\n"));
#endif
   /* Send the request to the APP */
   SmMiLveCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildVeInitCntrl */

#ifdef LTE_HO_SUPPORT
/*
*
*       Fun:   smBuildVeX2InitCntrl
*
*       Desc:  Invoked to intimate the APP that Binding is DONE.
*              APP should initiate the X2AP association
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_ve_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildVeX2InitCntrl
(
)
#else
PUBLIC S16 smBuildVeX2InitCntrl(Void)
#endif
{
   VeMngmt cntrl;
   Pst      pst;
   
   TRC2(smBuildVeX2InitCntrl)


   SM_SET_ZERO(&cntrl, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTVE;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STVEX2CONSETUP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Cntrl Req to ve for X2 Init\n"));
#endif
   /* Send the request to the APP */
   SmMiLveCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildVeX2InitCntrl */
#endif /* End of smBuildVeX2InitCntrl */


/*
*
*       Fun:   smBuildVeDbgCntrl
*
*       Desc:  Invoked to enable debug prints
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_ve_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildVeDbgCntrl
(
)
#else
PUBLIC S16 smBuildVeDbgCntrl(Void)
#endif
{
   VeMngmt cntrl;
   Pst      pst;
   
   TRC2(smBuildVeDbgCntrl)

   SM_SET_ZERO(&cntrl, sizeof(VeMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTVE;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STVEGEN;

   cntrl.u.cntrl.action        =  AENA;
   cntrl.u.cntrl.subAction     =  SADBG;

   /* Fix for CRId- 115717 */
/* Enable debug mask */
#ifdef DEBUGP
   cntrl.u.cntrl.u.dbgMask = 0xffffffff;
#endif

/* Initializing log file informations */
#if (VE_SM_LOG_TO_FILE && DEBUGP)
   cntrl.u.cntrl.u.dbgMask     = 0xFFFFFFFF;
   cntrl.u.cntrl.u.dbg.nmbDbgLines = 1000;
   cmMemset(cntrl.u.cntrl.u.dbg.filePath, 0, LVE_MAX_FILE_PATH );
#endif

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRM Debug Cntrl Req to ve\n"));
#endif
   /* Send the request to the APP */
   SmMiLveCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildVeInitCntrl */

/********************************************************************30**
  
         End of file:     ve_sm_ve_cntrl.c@@/main/2 - Wed Dec  1 08:44:39 2010
  
*********************************************************************31*/


/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
