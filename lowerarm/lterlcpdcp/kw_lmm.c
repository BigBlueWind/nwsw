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


/********************************************************************20**

        Name:    LTE - RLC - Layer Manager file

        Type:    C source file

        Desc:    It contains the following common  functions for processing
                 the all the external interfaces.
                    -- KwMiLkwCfgReq
                    -- KwMiLkwCfgCfm 
                    -- KwMiLkwCntrlReq
                    -- KwMiLkwCntrlCfm
                    -- KwMiLkwStaReq
                    -- KwMiLkwStaCfm
                    -- KwMiLkwStaInd
                    -- KwMiLkwStsReq
                    -- KwMiLkwStsCfm
                    -- KwMiLkwTrcInd 
                    -- KwMiLkwL2MeasReq

        File:     gp_lmm.c

        Sid:      gp_lmm.c@@/main/2 - Fri Nov 13 14:13:42 2009

        Prg:     cp 

*********************************************************************21*/
/** @file gp_lmm.c
@brief RLC Layer Manager Module
**/


/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_err.h"        /* Err defines */
#include "kw_env.h"        /* RLC environment options */
#ifdef KW_PDCP
#include "lpj.h"           /* LPJ defines */
#include "cpj.h"           /* CPJ defines */
#include "pju.h"           /* PJU defines */
#endif /* KW_PDCP */

#include "kw.h"            /* RLC defines */

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */
#ifdef KW_PDCP
#include "lpj.x"           /* LPJ defines */
#include "cpj.x"           /* CPJ defines */
#include "pju.x"           /* PJU defines */
#endif /* KW_PDCP */

#include "kw.x"
#include "ss_diag.h"        /* Common log file */
#ifdef PWAV_HC
#include "pst_events.h"
#include "cmn_larm_uarm.h"
#include "version.x"
#endif
/* Global variable */
/* kw006.201 optimization, fix for ccpu00111716 */
Pst selfPstDl;
/* kw006.201 optimization, fix for ccpu00117290 */
Pst selfPstUl;
Pst selfPstReest;
Pst selfPstDelete;

/*********************************************************************
 *             Forward Declaration of LKW Porting Functions
 ********************************************************************/
EXTERN S16 kwActvTmr ARGS ((Void));
EXTERN S16 pjActvTmr ARGS ((Void));

PRIVATE S16 kwLmmSendCfm ARGS ((Pst *pst,KwMngmt *cfm,U8 type,Header *hdr));
PRIVATE S16 kwLmmGenCfg ARGS ((KwGenCfg *cfg));
PRIVATE S16 kwLmmCfgKwuSap ARGS ((KwSapCfg *cfg));
PRIVATE S16 kwLmmCfgCkwSap ARGS ((KwSapCfg   *cfg));
PRIVATE S16 kwLmmCfgRguSap ARGS ((KwSapCfg   *cfg));
PRIVATE S16 kwLmmGenCntrl ARGS ((KwMngmt *cntrl));
PRIVATE S16 kwLmmLSapCntrl ARGS ((KwMngmt *cntrl));
PRIVATE S16 kwLmmFormLmCfm ARGS ((Pst *pst, Header *hdr, Reason reason));
PRIVATE S16 kwLmmGetKwuSapSta ARGS ((KwKwuSapSta *sta));
PRIVATE S16 kwLmmGetRguSapSta ARGS ((KwRguSapSta *sta));
PRIVATE S16 kwLmmGetCkwCntSapSta ARGS ((KwCkwCntSapSta *sta));
PRIVATE S16 kwLmmGetGenSts ARGS ((KwGenSts *sts,Action action));
PRIVATE S16 kwLmmGetSapSts ARGS ((KwMngmt *sts,Elmnt elmnt,Action action));


/**
 * @brief This funtion initializes RLC/PDCP for background processing
 *
 *  @param[in] Void        none
 *  @return    Void        none
 */
#ifdef ANSI
PRIVATE S16 kwLmmInitUlForBG
(
Void
)
#else
PRIVATE S16 kwLmmInitUlForBG(Void)
#endif
{
#ifdef MSPD_RLC_MULTICORE
   /*Initialize the UL Lock*/
   if (SInitLock(&kwCb.ulLock, SS_LOCK_MUTEX) != ROK)
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKWXXX, (ErrVal)0,
                                    "Lock initialization failed\n");
      RETVALUE(LCM_REASON_ULLOCK_INIT_FAILED);
   }
   /*Initialize the UL Lock*/
   if (SInitLock(&kwCb.selfPstUlLock, SS_LOCK_MUTEX) != ROK)
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKWXXX, (ErrVal)0,
                                    "Lock initialization failed\n");
      RETVALUE(LCM_REASON_ULLOCK_INIT_FAILED);
   }
#endif
#ifdef KW_BG_UL_PROC
   /* Initialize selfPstUl  */
   KW_MEM_SET(&(selfPstUl), 0, sizeof(Pst));
   selfPstUl.dstProcId = SFndProcId();
   selfPstUl.dstEnt = ENTKW;
   selfPstUl.dstInst = 0;
   selfPstUl.srcEnt = ENTKW;
   selfPstUl.srcInst = 0;
   selfPstUl.prior  = PRIOR3;
   selfPstUl.event  = EVTKWSELFMSGUL;
   /* Inititalize rbLst in kwCb */
   cmLListInit(&kwCb.reassemblyRbLst);
   /*One time allocation for the mBufUl used for UL reassembly*/
   SGetMsg(kwCb.init.region, kwCb.init.pool, &kwCb.mBufUl);
#endif
   RETVALUE(ROK);
}

/**
 * @brief This funtion initializes RLC/PDCP for background processing
 *
 *  @param[in] Void        none
 *  @return    Void        none
 */
#ifdef ANSI
PRIVATE S16 kwLmmInitDlForBG
(
Void
)
#else
PRIVATE S16 kwLmmInitDlForBG(Void)
#endif
{
#ifdef MSPD_RLC_MULTICORE
   /* Initialize the DL lock for PDU release */
   if (SInitLock(&kwCb.dlLock, SS_LOCK_MUTEX) != ROK)
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKWXXX, (ErrVal)0,
                                    "Lock initialization failed\n");
      RETVALUE(LCM_REASON_ULLOCK_INIT_FAILED);
   }
   /* Initialize the DL lock for SDU addition */
   if (SInitLock(&kwCb.sduLock, SS_LOCK_MUTEX) != ROK)
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKWXXX, (ErrVal)0,
                                    "Lock initialization failed\n");
      RETVALUE(LCM_REASON_ULLOCK_INIT_FAILED);
   }
   if (SInitLock(&kwCb.selfPstDlLock, SS_LOCK_MUTEX) != ROK)
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKWXXX, (ErrVal)0,
                                    "Lock initialization failed\n");
      RETVALUE(LCM_REASON_ULLOCK_INIT_FAILED);
   }
#endif
#ifdef KW_BG_DL_PROC
   /* Initialize selfPst  */
   KW_MEM_SET(&(selfPstDl), 0, sizeof(Pst));
   selfPstDl.dstProcId = SFndProcId();
   selfPstDl.dstEnt  = ENTKW;
   selfPstDl.srcEnt  = ENTKW;
#ifdef MSPD
   selfPstDl.dstInst = 1;
#else   
   selfPstDl.dstInst = 0;
#endif   
   selfPstDl.srcInst = 0;
   selfPstDl.prior   = PRIOR3;
   selfPstDl.event   = EVTKWSELFMSGDL;
   /* Inititalize statRbLst in kwCb */
   cmLListInit(&kwCb.pduRlsRbLst);
   /*One time allocation for the mBufDl used for DL Ack deletion*/
   SGetMsg(kwCb.init.region, kwCb.init.pool, &kwCb.mBufDl);
#endif
   RETVALUE(ROK);
}
/**
 * @brief This funtion initializes RLC/PDCP for background processing
 *
 *  @param[in] Void        none
 *  @return    Void        none
 */
#ifdef ANSI
PRIVATE S16 kwLmmInitReestForBG
(
Void
)
#else
PRIVATE S16 kwLmmInitReestForBG(Void)
#endif
{
#ifdef MSPD_RLC_MULTICORE
   /* Initialize the DL lock for PDU release */
   if (SInitLock(&kwCb.reestRbLstLock, SS_LOCK_MUTEX) != ROK)
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKWXXX, (ErrVal)0,
                                    "Lock initialization failed\n");
      RETVALUE(LCM_REASON_ULLOCK_INIT_FAILED);
   }
#endif
   /* Initialize selfPst  */
   KW_MEM_SET(&(selfPstReest), 0, sizeof(Pst));
   selfPstReest.dstProcId = SFndProcId();
   selfPstReest.dstEnt  = ENTKW;
   selfPstReest.srcEnt  = ENTKW;
#ifdef MSPD
   selfPstReest.dstInst = 1;
#else
   selfPstReest.dstInst = 0;
#endif
   selfPstReest.srcInst = 0;
   selfPstReest.prior   = PRIOR3;
   selfPstReest.event   = EVTKWSELFMSGREEST;
   /* Inititalize reestablishRbList in kwCb */
   cmLListInit(&kwCb.reestablishRbLst);
   /*One time allocation for the mBufReest used for Re-establishment procedure*/
   SGetMsg(kwCb.init.region, kwCb.init.pool, &kwCb.mBufReest);
   RETVALUE(ROK);
}
/**
 * @brief This funtion initializes RLC/PDCP for background processing
 *
 *  @param[in] Void        none
 *  @return    Void        none
 */
#ifdef ANSI
PRIVATE S16 kwLmmInitDeleteForBG
(
Void
)
#else
PRIVATE S16 kwLmmInitDeleteForBG(Void)
#endif
{
#ifdef MSPD_RLC_MULTICORE
   /* Initialize the DL lock for PDU release */
   if (SInitLock(&kwCb.deleteRbLstLock, SS_LOCK_MUTEX) != ROK)
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKWXXX, (ErrVal)0,
                                    "Lock initialization failed\n");
      RETVALUE(LCM_REASON_ULLOCK_INIT_FAILED);
   }
#endif
   /* Initialize selfPst  */
   KW_MEM_SET(&(selfPstDelete), 0, sizeof(Pst));
   selfPstDelete.dstProcId = SFndProcId();
   selfPstDelete.dstEnt  = ENTKW;
   selfPstDelete.srcEnt  = ENTKW;
#ifdef MSPD
   selfPstDelete.dstInst = 1;
#else
   selfPstDelete.dstInst = 0;
#endif
   selfPstDelete.srcInst = 0;
   selfPstDelete.prior   = PRIOR3;
   selfPstDelete.event   = EVTKWSELFMSGDELETE;
   /* Inititalize reestablishRbList in kwCb */
   cmLListInit(&kwCb.deleteRbLst);
   /*One time allocation for the mBufReest used for Re-establishment procedure*/
   SGetMsg(kwCb.init.region, kwCb.init.pool, &kwCb.mBufDelete);
   RETVALUE(ROK);
}
/*********************************************************************
 *             Primitives for RGU interface 
 ********************************************************************/
/**
 * @brief
      Stores the general RLC configuration. It calculates the
      memory requirement and reserves it. It also initializes the
      control blocks, the hash lists and registers the timers. In
      case of error it rolls back and reports error in the confirm.
      If the general configuration is already done it is treated as
      reconfiguration, but right now reconfiguration is not supported.
 
 *  @param[in] cfm         RLC LM general config structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL 
 *      -# Failure : LCM_REASON_MEM_NOAVAIL
 *                   LCM_REASON_REGTMR_FAIL
 *                   LCM_REASON_INVALID_PAR_VAL
 *                   LCM_REASON_HASHING_FAILED
 */
#ifdef ANSI
PRIVATE S16 kwLmmGenCfg
(
KwGenCfg    *cfg
)
#else
PRIVATE S16 kwLmmGenCfg(cfg)
KwGenCfg    *cfg;
#endif
{
   Size       kwMemSize;
   Size       kwSapSize;
   KwKwuSapCb *kwSapCb;
   U16        count;
   S16        ret;
   CmTqCp     *kwTqCp;
   U8         cnt;

   TRC2(kwLmmGenCfg)

   ret       =   ROK;
   count     =   0;
   kwMemSize =   0;
   kwSapSize =   0;
   kwSapCb   =   NULLP;

   if((cfg->maxUe > KW_MAX_UE) || (cfg->maxKwuSaps > KW_MAX_KWUSAPS))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      KWLOGERROR(ERRCLS_INT_PAR, EKW042, (ErrVal) cfg->maxUe,
               "kwLmmGenCfg: Invalid maxUe ");

#endif /* ERRCLASS & ERRCLS_INT_PAR */

      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

#ifdef KW_PDCP
   if(kwCb.init.cfgDone & KW_LMM_GENCFG_DONE)
#else /* KW_PDCP */
   if(kwCb.init.cfgDone == TRUE)
#endif /* KW_PDCP */
   {
      /* reconfiguration not supported */
      RETVALUE(LCM_REASON_RECONFIG_FAIL);
   }

   KW_MEM_SET(&(kwCb.genCfg), 0, sizeof(KwGenCfg));
   KW_MEM_CPY(&(kwCb.genCfg), cfg, sizeof(KwGenCfg));

   /* Timer Queue Control point initialization */
   kwTqCp         = &(kwCb.kwTqCp);
   kwTqCp->tmrLen = KW_TMR_LEN;
   kwTqCp->nxtEnt = 0;

   /* calculate the memory requirements */
   /* Size of kwuSaps */
   kwMemSize = ((Size) kwCb.genCfg.maxKwuSaps * \
                    (Size)SBUFSIZE((Size)sizeof(KwKwuSapCb)) );
   /* Ue Hash List Size */
   kwMemSize = kwMemSize + \
               (Size)SBUFSIZE(KW_UE_LIST_BUCKET_SIZE * sizeof(CmListEnt));
   /* Cell Hash List Size */
   kwMemSize = kwMemSize + (Size)SBUFSIZE(KW_CELL_LIST_BUCKET_SIZE * \
                             sizeof(CmListEnt));

   /* Reserve the memory */
   if(SGetSMem(KW_GET_MEM_REGION(kwCb), kwMemSize,
              KW_GET_MEM_POOL_ADDRESS(kwCb)) != ROK)
   {
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }

   /* allocate memory to the KWU sap's */
   kwSapSize = (Size)((Size) kwCb.genCfg.maxKwuSaps *
                    (Size)sizeof(KwKwuSapCb));

   KW_ALLOC(kwCb.kwuSap, kwSapSize);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(kwCb.kwuSap == NULLP)
   {
      (Void) SPutSMem(KW_GET_MEM_REGION(kwCb), KW_GET_MEM_POOL(kwCb));
      
      KWLOGERROR(ERRCLS_INT_PAR, EKW043, (ErrVal) cfg->maxUe,
               "kwLmmGenCfg: SgetSBuf Failed for kwuSap...!");
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }
#endif

   /* Register the timer */
   if(SRegTmr(kwCb.init.ent, kwCb.init.inst, (U16)cfg->timeRes,
              kwActvTmr) != ROK)
   {
      if (kwCb.kwuSap != NULLP)
      {
         KW_FREE(kwCb.kwuSap, kwSapSize);
      }
      (Void) SPutSMem(KW_GET_MEM_REGION(kwCb), KW_GET_MEM_POOL(kwCb));

      RETVALUE(LCM_REASON_REGTMR_FAIL);
   }

   /* Timer Initialization  */
   kwCb.kwTqCp.tmrLen = KW_TMR_LEN;
   for(cnt = 0; cnt < KW_TMR_LEN; cnt++)
   {
      kwCb.kwTq[cnt].first = NULLP;
      kwCb.kwTq[cnt].tail = NULLP;
   }

   /* Initialize the Ue and Cell hash list */
   ret = kwDbmInit();
   if (ret != ROK)
   {
      RETVALUE(LCM_REASON_HASHING_FAILED);
   }
    

   /* Initialize the KWU SAP control blocks */
   for(count = 0; count < (kwCb.genCfg.maxKwuSaps); count++)
   {
      kwSapCb = (KwKwuSapCb*)(kwCb.kwuSap + count);
      KW_MEM_SET(kwSapCb, 0, sizeof(KwKwuSapCb));
   }

   /* Initialize CKW SAP control block */
   KW_MEM_SET(&(kwCb.ckwSap), 0, sizeof(KwCkwSapCb));

   /* Initialize RGU SAP control block */
   KW_MEM_SET(&(kwCb.rguSap), 0, sizeof(KwRguSapCb));

   KW_MEM_CPY(&(kwCb.init.lmPst), &cfg->lmPst,
                   sizeof(Pst));

   kwCb.init.lmPst.srcProcId = kwCb.init.procId;
   kwCb.init.lmPst.srcEnt = kwCb.init.ent;
   kwCb.init.lmPst.srcInst = kwCb.init.inst;
   kwCb.init.lmPst.event = EVTNONE;

#if !defined(SS_MULTICORE_SUPPORT) && !defined(SS_M_PROTO_REGION)
   kwCb.init.region = cfg->lmPst.region;
#endif /* !defined(SS_MULTICORE_SUPPORT) && !defined(SS_M_PROTO_REGION) */
   kwCb.init.pool   = cfg->lmPst.pool;

#ifdef KW_PDCP
   kwCb.init.cfgDone |= KW_LMM_GENCFG_DONE;
#else
   kwCb.init.cfgDone = TRUE;
#endif

   kwLmmInitUlForBG();

   kwLmmInitDlForBG();

   kwLmmInitReestForBG();

   kwLmmInitDeleteForBG();

#ifdef MSPD_RLC_MULTICORE
   /* Initialize the DBM lock for UeCb/RbCb creation */
   if (SInitLock(&kwCb.dbmLock, SS_LOCK_MUTEX) != ROK)
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKWXXX, (ErrVal)0,
                                    "Lock initialization failed\n");
      RETVALUE(LCM_REASON_ULLOCK_INIT_FAILED);
   }
#endif
   RETVALUE(LCM_REASON_NOT_APPL);
} /* kwLmmGenCfg */

/**
 *
 * @brief
 *     Sends confirmation message to LM Called by KwMiLkwCfgReq function
 *  @param[in] pst         Post structure
 *  @param[in] cfm         RLC LM structure 
 *  @param[in] type        type of message
 *  @param[in] hdr         message header
 *  @return  S16
 *      -# ROK
 */
#ifdef ANSI
PRIVATE S16 kwLmmSendCfm
(
Pst *pst,                    /* Post structure */
KwMngmt *cfm,                /* RLC LM structure */
U8 type,                     /* type of message */
Header *hdr                  /* message header */
)
#else
PRIVATE S16 kwLmmSendCfm(pst, cfm, type, hdr)
Pst *pst;                    /* Post structure */
KwMngmt *cfm;                /* RLC LM structure */
U8 type;                     /* type of message */
Header *hdr;                 /* message header */
#endif
{
   Pst rPst;                 /* Reply post structure */

   TRC2(kwLmmSendCfm);

   KW_MEM_SET(&rPst, 0, sizeof(Pst));   
 
   /* reply post structure for confirmation */
   rPst.srcEnt = kwCb.init.ent;
   rPst.srcInst = kwCb.init.inst;
   rPst.srcProcId = SFndProcId();
   rPst.dstEnt = pst->srcEnt;
   rPst.dstInst = pst->srcInst;
   rPst.dstProcId = pst->srcProcId;
   rPst.selector = hdr->response.selector;
   rPst.prior = hdr->response.prior;
   rPst.route = hdr->response.route;
   rPst.region = hdr->response.mem.region;
   rPst.pool= hdr->response.mem.pool;

   /* Fill the reply header */
   KW_MEM_SET (&cfm->hdr, '\0', sizeof(cfm->hdr));
   cfm->hdr.elmId.elmnt = hdr->elmId.elmnt;
   cfm->hdr.entId.ent = kwCb.init.ent;
   cfm->hdr.entId.inst = kwCb.init.inst;
   cfm->hdr.transId = hdr->transId;

   switch(type)
   {
      case TCFG:
         KwMiLkwCfgCfm(&rPst,cfm);
         break;
      case TCNTRL:
         SGetDateTime(&(cfm->t.cntrl.dt));
         KwMiLkwCntrlCfm(&rPst,cfm);
         break;
      case TSTS:
         SGetDateTime(&(cfm->t.sts.dt));
         KwMiLkwStsCfm(&rPst,0,cfm);
         break;
      case TSSTA:
         SGetDateTime(&(cfm->t.ssta.dt));
         KwMiLkwStaCfm(&rPst,cfm);
         break;
      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         KWLOGERROR(ERRCLS_INT_PAR, EKW044, (ErrVal) type,
               "Invalid message type");
          
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         break;
   }

   RETVALUE(ROK);
} /* kwLmmSendCfm */


/**
@brief
This function processes configuration requests received from the layer manager.The configuration requests can be for general configuration, or configuration of RGU, KWU, and CKW SAPs. The type of the configuration request can be determined from the elmId field in the header of the layer manager message.This function is called by the layer manager to configure RLC. 

This function implements the following logic: \n

- switch (cfg->hdr.elmId.elmnt)
   - case STGEN 
      - update the genCfg field in RlCb; 
      - allocate the maximum static memory required by the RLC product; 
      - register the timer service by calling SReqTmr; 
      - set CfgDone for not configuring again; 
   - case STKWUSAP 
      - if genCfg is not done, send negative Cfm to the layer manager; 
      - if all the parameters are valid, then allocate the RlSap control block; 
      - update the SAP control block with the information provided in the 
      - configuration request; 
      - send configuration confirm with the status, success; 
   - case STCKWSAP 
      - if genCfg is not done, send negative Cfm to the layer manager; 
      - if all the parametes are valid, then allocate the CrSap control block; 
      - update the SAP control block with the information provided in the configuration request; 
      - send configuration confirm with the status, success; 
   - case STRGUSAP 
      - if genCfg is not done, send negative Cfm to the layer manager; 
      - if all the parametes are valid, then allocate the MkSap control block; 
      - update the SAP control block with the information provided in the configuration request; 
      - send configuration confirm with the status, success; 
- end switch;
 
 *  @param[in] pst      post structure 
 *  @param[in] cfg      LM management structure 
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/

#ifdef ANSI
PUBLIC S16 KwMiLkwCfgReq 
(
Pst *pst, 
KwMngmt *cfg
)
#else
PUBLIC S16 KwMiLkwCfgReq (pst, cfg)
Pst  *pst;
KwMngmt *cfg;
#endif
{
   S16   ret;
   Elmnt elmnt;          /* header element */
   KwMngmt rCfm;         /* Reply cfm */
   Reason reason;        /* failure reason */

   TRC3(KwMiLkwCfgReq);

   KWDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF), (_kwPBuf,
            "KwMiLkwCfgReq(post, elmId(%d))\n", cfg->hdr.elmId.elmnt));

  KW_DIAG_LVL0(0x0b070001, KW_DIAG_NA, SS_DIAG_INV_ARG,
               "Received CfgReq from LM, element = %d",
                cfg->hdr.elmId.elmnt, 0, 0, 0)

   ret = ROK;

   /* check configuration is done or not */
   if (
#ifdef KW_PDCP
         !(kwCb.init.cfgDone & KW_LMM_GENCFG_DONE)
#else /* KW_PDCP */
         (kwCb.init.cfgDone != TRUE)
#endif /* KW_PDCP */
         && (cfg->hdr.elmId.elmnt != STGEN))
   {
      /*
       * if general config is not over then use pst structure
       * in primitive to communicate to stack manager
       */

      rCfm.cfm.status = LCM_PRIM_NOK;
      rCfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      kwLmmSendCfm(pst, &rCfm, TCFG, &cfg->hdr);     
      RETVALUE(RFAILED);
   }

   elmnt = cfg->hdr.elmId.elmnt;

   switch(elmnt)
   {
      case STGEN:
      {
         reason = kwLmmGenCfg(&cfg->t.cfg.s.gen);
         break;
      }
      case STKWUSAP:
      {
         reason = kwLmmCfgKwuSap(&cfg->t.cfg.s.sap);
         break;
      }
      case STCKWSAP:
      {
         reason = kwLmmCfgCkwSap(&cfg->t.cfg.s.sap);
         break;
      }
      case STRGUSAP:
      {
         reason = kwLmmCfgRguSap(&cfg->t.cfg.s.sap);
         break;
      }
      default:
      {
         reason = LCM_REASON_INVALID_ELMNT;
         break;
      }   
   }

   if (reason == LCM_REASON_NOT_APPL)
   {
      rCfm.cfm.status = LCM_PRIM_OK;
      rCfm.cfm.reason = LCM_REASON_NOT_APPL;

      ret = ROK;
   }
   else
   {
      rCfm.cfm.status = LCM_PRIM_NOK;
      rCfm.cfm.reason = reason;

      ret = RFAILED;
   }

   ret = kwLmmSendCfm(pst, &rCfm, TCFG, &cfg->hdr);

   RETVALUE(ret);
} /* KwMiLkwCfgReq */

/**
@brief 
The layer manager initiates and uses the management-control procedure to control RLC elements.The RLC control request primitive (KwMiLkwCntrlReq) can be called more than once and at any time after the management-configuration procedure.The control request primitive is confirmed by a KwMiLkwCntrlCfm primitive.

This function implements the following logic: \n

- if(cfgDone = FALSE) 
   - send negative confirmation to the layer manager; 
   - return; 
- else 
- switch(cntrl->hdr.elmId.elmnt) 
   -  case STGEN 
        - switch (action) 
        - case AENA 
             - switch(subAction) 
             - case SAUSTA 
                  - enable the unsolicited status flag; 
                  - send the control Cfm with success; 
             - case SADBG 
                  - set the debug mask; 
                  - send the control Cfm with success; 
             - case SATRC 
                  - enable the trace flag; 
                  - send the control Cfm with success; 
        - case ADISIMM 
             - switch(subAction) 
             - case SAUSTA 
                  - disable the unsolicited status flag; 
                  - send the control Cfm with success; 
             - case SATRC 
                  - disable the trace flag; 
                  - send the control Cfm with success; 
        - case ASHUTDOWN 
   - case STMKSAP 
        - switch (action) 
        - case ABND 
        - case AUBND 
   - default  
- end else  
 *  @param[in] pst      post structure
 *  @param[in] cntrl    LM management structure
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/
#ifdef ANSI
PUBLIC S16 KwMiLkwCntrlReq
(
Pst *pst, 
KwMngmt *cntrl
)
#else
PUBLIC S16 KwMiLkwCntrlReq(pst, cntrl)
Pst     *pst;
KwMngmt *cntrl;
#endif
{
   Elmnt elmnt;          /* header element */
   KwMngmt rCfm;         /* Reply cfm */
   Reason reason;        /* failure reason */

   S16 ret;

   TRC3(KwMiLkwCntrlReq)

   KWDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF), (_kwPBuf,
            "KwMiLkwCntrlReq(post, elmId(%d))\n", cntrl->hdr.elmId.elmnt));

  KW_DIAG_LVL0(0x0b070002, KW_DIAG_NA, SS_DIAG_INV_ARG,
               "Received CntrlReq from LM, element = %d",
                cntrl->hdr.elmId.elmnt, 0, 0, 0)
   ret = ROK;

   if (!(kwCb.init.cfgDone))
   {
      rCfm.cfm.status = LCM_PRIM_NOK;
      rCfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;

      kwLmmSendCfm(pst, &rCfm, TCNTRL, &cntrl->hdr);
      RETVALUE(RFAILED);
   }

   /* In normal cases, LCM_REASON_NOT_APPL is returned in cfm.
    * In all error cases appropriate reason is returned
    * by the functions below
    */

   elmnt = cntrl->hdr.elmId.elmnt;

   reason = LCM_REASON_NOT_APPL;


   switch (elmnt)
   {
      case STGEN:
      {
         /* general control */
         reason = kwLmmGenCntrl(cntrl);
         break;
      }

      /* Configure RLC SAP control */
      case STRGUSAP:
      {
         /* Lower SAP control */
         reason = kwLmmLSapCntrl(cntrl);
         break;
      }

      default:
      {
         reason = LCM_REASON_INVALID_ELMNT;
         break;
      }
   } /* end switch */

   ret = kwLmmFormLmCfm(pst, &cntrl->hdr, reason);
   RETVALUE(ret);
} /* KwMiLkwCntrlReq */

/**
@brief 
This function processes solicited status requests received from the layer
manager. The layer manager can request status information regarding the
system ID, RGSAP,KWSAP, or CKSAP.

 *  @param[in] pst      post structure
 *  @param[in] cfm    LM management structure
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/
#ifdef ANSI
PUBLIC S16 KwMiLkwStaReq
(
Pst *pst, 
KwMngmt *sta
)
#else
PUBLIC S16 KwMiLkwStaReq(pst, sta)
Pst     *pst;
KwMngmt *sta;
#endif
{
   S16      ret;
   Elmnt    elmnt;            /* Header element */
   KwMngmt  rSta;             /* Status */
   /* kw006.201, ccpu00120574, fixed the warning */

   TRC3(KwMiLkwStaReq);


   KWDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF), (_kwPBuf,
            "KwMiLkwStaReq(post, elmId(%d))\n", sta->hdr.elmId.elmnt));

  KW_DIAG_LVL0(0x0b070003, KW_DIAG_NA, SS_DIAG_INV_ARG,
               "Received StaReq from LM, element = %d",
                sta->hdr.elmId.elmnt, 0, 0, 0)

   KW_MEM_SET(&rSta, 0, sizeof(KwMngmt));

   elmnt = sta->hdr.elmId.elmnt;

   switch (elmnt)
   {
      case STSID:
      {
         /* kw006.201, ccpu00120574, fixed the warning */
         kwGetSId(&rSta.t.ssta.s.sysId);
         break;
      }
      case STGEN:
      {
         break;
      }
      case STKWUSAP:
      {
         KW_MEM_CPY (&rSta.t.ssta.s.kwuSap, &sta->t.ssta.s.kwuSap,
               sizeof (KwKwuSapSta));
         /* kw006.201, ccpu00120574, fixed the warning */
         kwLmmGetKwuSapSta (&rSta.t.ssta.s.kwuSap);
         break;
      }
      case STRGUSAP:
      {
         KW_MEM_CPY (&rSta.t.ssta.s.rguSap, &sta->t.ssta.s.rguSap,
               sizeof (KwRguSapSta));
         /* kw006.201, ccpu00120574, fixed the warning */
         kwLmmGetRguSapSta (&rSta.t.ssta.s.rguSap);
         break;
      }
      case STCKWSAP:
      {
         KW_MEM_CPY (&rSta.t.ssta.s.ckwSap, &sta->t.ssta.s.ckwSap,
               sizeof (KwCkwCntSapSta));
         /* kw006.201, ccpu00120574, fixed the warning */
         kwLmmGetCkwCntSapSta (&rSta.t.ssta.s.ckwSap);
         break;
      }
      default:
      {
         /* kw006.201, ccpu00120574, fixed the warning */
         break;
      }
   } /* end of switch */

   ret = kwLmmSendCfm(pst, &rSta, TSSTA, &sta->hdr);

   RETVALUE(ret);
} /* KwMiLkwStaReq */

/**
@brief 
This function processes statistics requests received from the layer manager. After collecting the statistics, this function calls the statistics confirm function to send the statistics to the layer manager.

- switch(sts->hdr.elmId.elmnt) 
  - case STGEN 
     - get the general statistics from the KW control block; 
     - if (action = RESET) 
        - reset the general statistic field in the RlCb; 
        - call KwMiLkwStsCfm to send statistics back to layer manager; 
  - case STCKWSAP 
  - case STKWUSAP 
  - case STRGUSAP 
      - get the SAP specific statistics from KW control block; 
      - if (action = RESET) 
      - reset the general statistic field in the RlCb; 
      - call KwMiLkwStsCfm to send statistics to the layer manager; 
-end switch;

 *  @param[in] pst      post structure
 *  @param[in] action   action 
 *  @param[in] cfg      LM management structure
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/
#ifdef ANSI
PUBLIC S16 KwMiLkwStsReq 
(
Pst *pst, 
Action action, 
KwMngmt *sts
)
#else
PUBLIC S16 KwMiLkwStsReq (pst, action, sts)
Pst     *pst;
Action  action;
KwMngmt *sts;
#endif
{
   S16      ret;
   KwMngmt  rSts;             /* Statistics */
   /* kw006.201, ccpu00120574, fixed the warning */
   Elmnt    elmnt;            /* Element Id */

   TRC3(KwMiLkwStsReq);

   KWDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF), (_kwPBuf,
           "KwMiLkwStsReq(post, elmId(%d),action(%d))\n", action, 
           sts->hdr.elmId.elmnt));

  KW_DIAG_LVL0(0x0b070004, KW_DIAG_NA, SS_DIAG_INV_ARG,
               "Received StsReq from LM, element = %d",
                sts->hdr.elmId.elmnt, 0, 0, 0)

   KW_MEM_SET(&rSts, 0, sizeof(KwMngmt));
   elmnt = sts->hdr.elmId.elmnt;

   switch (elmnt)
   {
      case STGEN:
      {
         /* kw006.201, ccpu00120574, fixed the warning */
         kwLmmGetGenSts(&rSts.t.sts.s.gen, action);
         break;
      }
      case STKWUSAP:
      case STRGUSAP:
      case STCKWSAP:
      {
         if(elmnt == STKWUSAP)
         {
            /* kw005.201, modified the element of kwuSap from suId to spId */
            rSts.t.sts.s.kwuSap.spId = sts->t.sts.s.kwuSap.spId;
         }
         /* kw006.201, ccpu00120574, fixed the warning */
         kwLmmGetSapSts(&rSts, elmnt, action);
         break;
      }
      default:
      {
         /* kw006.201, ccpu00120574, fixed the warning */
         break;
      }
   } /* end switch */

   ret = kwLmmSendCfm(pst, &rSts, TSTS, &sts->hdr);

   RETVALUE(ret);
} /* KwMiLkwStsReq */

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
/**
@brief 
This function processes L2 Measurement requests received from the layer manager.
The L2 Measurement is start for a time period and after the timer expiry, the 
measurement confirm is sent.

-  Accept only one set of measurements.
-  Allocate and initialise KwL2MeasEvtCb.
-  Validate if the measurement is already in progress.
   -  Loop through the existing measEvtCb with matching measType
      -  Check if measurement is running for any qci present in the measReq
      -  If present, move the qCi to list of invalid qCIs.
-  Set KwL2MeasEvtCb.measOn to TRUE for QCIs to be measured.
-  For the number of measurements requested.
   -  For all Ues with RBs matching qCI.
      -  Add RbCb to the RB linked list.
      -  Set kwRbCb.measOn to measurement type.
      -  If Meas type is DL_DELAY
         -  Update COUNT to startCount.
-  Start l2Timer

 *  @param[in] pst      post structure
 *  @param[in] action   action 
 *  @param[in] cfg      LM management structure
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/
#ifdef ANSI
PUBLIC S16 KwMiLkwL2MeasReq 
(
Pst            *pst, 
KwL2MeasReqEvt *measReqEvt 
)
#else
PUBLIC S16 KwMiLkwL2MeasReq (pst, measReqEvt)
Pst            *pst; 
KwL2MeasReqEvt *measReqEvt;
#endif
{
   S16            ret = ROK;
   KwL2MeasEvtCb *measEvtCb = NULLP;
   KwL2MeasCb    *measCb    = NULLP;
   U16            cntr;
   U8             measType;
   KwL2MeasCfmEvt measCfmEvt;
#ifdef LTE_RLC_R9
   CmLteLcId               lChId[KW_MAX_LCH_PER_UE];  /*List of LchIds for this UE*/
   U8                      numLCh = 0;                 /*Number of LCH for this UE*/
#endif /* LTE_RLC_R9 */

   TRC3(KwMiLkwL2MeasReq);
  
   /* Initialize measCfmEvt */
   KW_MEM_ZERO(&measCfmEvt, sizeof(KwL2MeasCfmEvt));
  /* validate the received measReqEvt */
#ifdef LTE_RLC_R9
   ret = kwUtlValidateL2Meas(measReqEvt, &measCfmEvt, lChId, &numLCh);
#else /* LTE_RLC_R9 */
   ret = kwUtlValidateL2Meas(measReqEvt, &measCfmEvt);
#endif /* LTE_RLC_R9 */

   if(ret != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      KWLOGERROR(ERRCLS_ADD_RES, EKWXXX, (ErrVal) RFAILED,
            "KwMiLkwL2MeasReq: Validation of measReq failed.");
#endif /* ERRCLASS & ERRCLS_ADD_RES */
      kwUtlSndL2MeasNCfm(measReqEvt, &measCfmEvt);
      RETVALUE(RFAILED);

   }
   measType = measReqEvt->measReq.measType;
   /* Allocate and intialise the Event Control Block */
   KW_ALLOC(measEvtCb, sizeof(KwL2MeasEvtCb));
   if(measEvtCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      KWLOGERROR(ERRCLS_ADD_RES, EKWXXX, (ErrVal) RFAILED,
            "KwMiLkwL2MeasReq: KW_ALLOC Failed for measCfmEvt.");
#endif /* ERRCLASS & ERRCLS_ADD_RES */
      measCfmEvt.numCfm  = 1;
      measCfmEvt.transId = measReqEvt->transId;
      measCfmEvt.measType = measType;
      measCfmEvt.status.status = LCM_PRIM_NOK;
      measCfmEvt.status.reason = LCM_REASON_MEM_NOAVAIL;
      kwUtlSndL2MeasNCfm(measReqEvt, &measCfmEvt);
      RETVALUE(RFAILED);
   }

   measEvtCb->transId      = measReqEvt->transId;

   measCb           = &measEvtCb->measCb;
   measCb->measType = measType;
   if(measCb->measType &  LKW_L2MEAS_ACT_UE)
   {
      measCb->numSamples = measReqEvt->measReq.val.nonIpThMeas.numSamples;
      measReqEvt->measPeriod /= measReqEvt->measReq.val.nonIpThMeas.numSamples;
   }
#ifdef LTE_RLC_R9
   if((measCb->measType & LKW_L2MEAS_DL_IP) ||
      (measCb->measType & LKW_L2MEAS_UL_IP))
   {
      /* Store valid qci in the control block */
      for(cntr = 0; cntr < measReqEvt->measReq.val.ipThMeas.numQci; cntr++)
      {
         measCb->qci[measCb->numQci++]  = 
                        measReqEvt->measReq.val.ipThMeas.qci[cntr];
         kwCb.kwL2Cb.measOn[measReqEvt->measReq.val.ipThMeas.qci[cntr]] |= 
                        measType;
      }
      measCb->ueId = measReqEvt->measReq.val.ipThMeas.ueId;
      measCb->cellId = measReqEvt->measReq.val.ipThMeas.cellId;
   }
   else
#endif /* LTE_RLC_R9 */
   {
      /* Store valid qci in the control block */
      for(cntr = 0; cntr < measReqEvt->measReq.val.nonIpThMeas.numQci; cntr++)
      {
         measCb->qci[measCb->numQci++]  = 
                        measReqEvt->measReq.val.nonIpThMeas.qci[cntr];
         kwCb.kwL2Cb.measOn[measReqEvt->measReq.val.nonIpThMeas.qci[cntr]] |= 
                        measType;
      }
   }
   measEvtCb->l2TmrCfg.enb = TRUE;
   measEvtCb->l2TmrCfg.val = measReqEvt->measPeriod;
   cmInitTimers(&measEvtCb->l2Tmr,KW_L2_MAX_TIMERS);

   /* update the RBCBs of the measurement */
   /* kw006.201 ccpu00120058, reduced code complexity by adding new function */
   kwUtlProcInitialActvUe(measCb);

   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      if(kwCb.kwL2Cb.kwL2EvtCb[cntr] == NULLP)
      {
         measEvtCb->cbIdx = cntr;
         kwCb.kwL2Cb.kwL2EvtCb[cntr]  = measEvtCb;
         kwCb.kwL2Cb.kwNumMeas++;
         break;
      }
   }

#ifdef LTE_RLC_R9
   /*Send notification to MAC to start Ul Ip measurement for given LChs*/
   /* kw006.201 ccpu00120058, reduced code complexity by adding new function */
   if(ROK != kwUtlNotifyMacUlIp(measCb, TRUE, lChId, &numLCh))
   {
      RETVALUE(RFAILED);
   }
#endif /* LTE_RLC_R9 */

   kwStartTmr((PTR)measEvtCb, KW_EVT_L2_TMR); 
   KW_FREE(measReqEvt, sizeof(KwL2MeasReqEvt))
   RETVALUE(ret);
} /* KwMiLkwL2MeasReq */

#endif /* LTE_L2_MEAS */

/**
 * @brief
      This function configures the RLC data sap
 *
 *  @param[in] cfm         RLC LM Sap structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SAP 
 *                   LCM_REASON_MEM_NOAVAIL 
 *                   LCM_REASON_RECONFIG_FAIL 
 */
#ifdef ANSI
PRIVATE S16 kwLmmCfgKwuSap
(
KwSapCfg   *cfg                /* SAP configuration structure */
)
#else
PRIVATE S16 kwLmmCfgKwuSap(cfg)
KwSapCfg   *cfg;               /* SAP configuration structure */
#endif
{
   KwKwuSapCb   *kwuSapCb;
   TRC2(kwLmmCfgKwuSap)

   /* Validate the protocol parameters */
   if (cfg->sapId >= (S16)kwCb.genCfg.maxKwuSaps)
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   /* Get Sap control block */
   kwuSapCb = (KwKwuSapCb*)(kwCb.kwuSap + cfg->sapId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check for reconfiguration */
   if (kwuSapCb->state != KW_SAP_NOT_CFG)
   {
      KWLOGERROR(ERRCLS_INT_PAR, EKW045, (ErrVal) kwuSapCb->state,
               "kwLmmCfgKwuSap: Invalid kwuSap State");
      /* reconfiguration not allowed */
      RETVALUE(LCM_REASON_RECONFIG_FAIL);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   /* Fill the parameters */
   kwuSapCb->pst.selector = cfg->selector;
   kwuSapCb->pst.route = cfg->route;
   kwuSapCb->pst.prior =  cfg->priority;
   /* kw002.201 With multicore support layer shall use the assigned region
      and pool from SSI */
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
   kwuSapCb->pst.region = kwCb.init.region; 
   kwuSapCb->pst.pool = kwCb.init.pool; 
#else /* defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION) */
   kwuSapCb->pst.region = cfg->mem.region; 
   kwuSapCb->pst.pool = cfg->mem.pool;
#endif /* defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION) */
   kwuSapCb->pst.dstProcId = cfg->procId;
   kwuSapCb->pst.dstEnt = cfg->ent;
   kwuSapCb->pst.dstInst = cfg->inst;
   kwuSapCb->pst.srcProcId = kwCb.init.procId;
   kwuSapCb->pst.srcEnt = kwCb.init.ent;
   kwuSapCb->pst.srcInst = kwCb.init.inst;
   kwuSapCb->pst.event = EVTNONE;
   kwuSapCb->spId = cfg->sapId;
   kwuSapCb->state = KW_SAP_CFG;

   /* update KWU sap counter */
   kwCb.maxKwuSaps++;

   RETVALUE(LCM_REASON_NOT_APPL);
} /* kwLmmCfgKwuSap */

/**
 * @brief
      This function configures the RLC control sap
 *
 *  @param[in] cfm         RLC LM Sap structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SAP
 *                   LCM_REASON_INVALID_SAP 
 *                   LCM_REASON_RECONFIG_FAIL 
 */
#ifdef ANSI
PRIVATE S16 kwLmmCfgCkwSap 
(
KwSapCfg   *cfg               /* SAP control block */
)
#else
PRIVATE S16 kwLmmCfgCkwSap(cfg)
KwSapCfg   *cfg;              /* SAP control block */
#endif
{
   KwCkwSapCb  *ckwSap;
   
   TRC2(kwLmmCfgCkwSap)

   /* Validate config parameters */
   if (cfg->sapId >= KW_MAX_CKWSAPS )
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   ckwSap = &(kwCb.ckwSap);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check for reconfiguration */
   if(ckwSap->state  != KW_SAP_NOT_CFG)
   {
      KWLOGERROR(ERRCLS_INT_PAR, EKW046, (ErrVal) ckwSap->state,
               "kwLmmCfgCkwSap: Invalid ckwSap State");
      RETVALUE(LCM_REASON_RECONFIG_FAIL);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   
   /* Fill the parameters */
   ckwSap->pst.selector = cfg->selector;
   ckwSap->pst.route = cfg->route;
   ckwSap->pst.prior =  cfg->priority;
   /* kw002.201 With multicore support layer shall use the assigned region
      and pool from SSI */
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
   ckwSap->pst.region = kwCb.init.region; 
   ckwSap->pst.pool = kwCb.init.pool; 
#else /* defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION) */
   ckwSap->pst.region = cfg->mem.region;
   ckwSap->pst.pool = cfg->mem.pool;
#endif /* defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION) */
   ckwSap->pst.dstProcId = cfg->procId;
   ckwSap->pst.dstEnt = cfg->ent;
   ckwSap->pst.dstInst = cfg->inst;
   ckwSap->pst.srcProcId = kwCb.init.procId;
   ckwSap->pst.srcEnt = kwCb.init.ent;
   ckwSap->pst.srcInst = kwCb.init.inst;
   ckwSap->pst.event = EVTNONE;
   ckwSap->spId = cfg->sapId;

   /* make sap unbound initially */
   ckwSap->state = KW_SAP_CFG;

   RETVALUE(LCM_REASON_NOT_APPL);
} /* kwLmmCfgCkwSap */

/**
 * @brief
      This function configures the RGU sap
 *
 *  @param[in] cfm         RLC LM Sap structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SAP
 *                   LCM_REASON_INVALID_SAP
 *                   LCM_REASON_RECONFIG_FAIL
 */
#ifdef ANSI
PRIVATE S16 kwLmmCfgRguSap 
(
KwSapCfg   *cfg               /* pointer to MKU SAP config struct */
)
#else
PRIVATE S16 kwLmmCfgRguSap(cfg)
KwSapCfg   *cfg;              /* pointer to MKU SAP config struct */
#endif
{
   KwRguSapCb *rguSap;

   TRC2(kwLmmCfgRguSap);
   
   /* Validate the protocol parameters */
   if((cfg->sapId >= KW_MAX_RGUSAPS) || (cfg->sapId < 0))
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   rguSap = &(kwCb.rguSap);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check for reconfiguration */
   if(rguSap->state != KW_SAP_NOT_CFG)
   {
      KWLOGERROR(ERRCLS_INT_PAR, EKW047, (ErrVal) rguSap->state,
               "kwLmmCfgRguSap: Invalid rguSap State");
      RETVALUE(LCM_REASON_RECONFIG_FAIL);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   /* Fill the parameters */
   rguSap->pst.selector = cfg->selector;
   rguSap->pst.route = cfg->route;
   rguSap->pst.prior =  cfg->priority;

   /* kw002.201 With multicore support layer shall use the assigned region
      and pool from SSI */
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
   rguSap->pst.region = kwCb.init.region; 
   rguSap->pst.pool = kwCb.init.pool; 
#else /* defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION) */
   rguSap->pst.region = cfg->mem.region;
   rguSap->pst.pool = cfg->mem.pool;
#endif /* defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION) */
   rguSap->pst.dstProcId = cfg->procId;
   rguSap->pst.dstEnt = cfg->ent;
   rguSap->pst.dstInst = cfg->inst;
   rguSap->pst.srcProcId = kwCb.init.procId;
   rguSap->pst.srcEnt = kwCb.init.ent;
   rguSap->pst.srcInst = kwCb.init.inst;
   rguSap->pst.event = EVTNONE;
   rguSap->spId = cfg->sapId;
   rguSap->bndTmrInt = cfg->bndTmrIntvl;
   rguSap->retryCnt = 0;
   rguSap->state = KW_SAP_CFG;

   cmInitTimers(&(rguSap->bndTmr), 1);

   RETVALUE(LCM_REASON_NOT_APPL);
} /* kwLmmCfgRguSap */

/**
 * @brief
     Validates the RLC general control parameters 
 *
 *  @param[in] cntrl RLC LM structure 
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SUBACTION
 *                   LCM_REASON_INVALID_ACTION
 */
#ifdef ANSI
PRIVATE S16 kwLmmValidateGenCntrl
(
KwMngmt *cntrl                 /* RLC LM structure */
)
#else
PRIVATE S16 kwLmmValidateGenCntrl(cntrl)
KwMngmt *cntrl;                /* RLC LM structure */
#endif
{
   S16 reason;                 /* reason for failure */
   U8 action;                  /* action parameter */
   U8 sAction;                 /* subaction field */


   reason = LCM_REASON_NOT_APPL;

   TRC2(kwLmmValidateGenCntrl);

   action = cntrl->t.cntrl.action;
   sAction = cntrl->t.cntrl.subAction;

   switch (action)
   {
      case AENA:
      case ADISIMM:
         if ((sAction != SAUSTA)
            && (sAction != SADBG)
            && (sAction != SATRC)
            && (sAction != SALOG))
         {
            reason = LCM_REASON_INVALID_SUBACTION;
         }
         break;
      case ASHUTDOWN:
         break;
      default:
         reason = LCM_REASON_INVALID_ACTION;
         break;
   }
   
   RETVALUE(reason);
} /* kwLmmValidateGenCntrl */

/**
 * @brief
     it deregisters the timers and deregisters the kwuSap Control blocks 
 *
 *  @return  Void
 */
#ifdef ANSI
PRIVATE Void kwLmmCleanGblRsrcs
(
Void
)
#else
PRIVATE Void kwLmmCleanGblRsrcs(Void)
#endif
{
   Size    kwSapSize;
#ifdef KW_PDCP
   Ent     ent;
#endif /* KW_PDCP */

   TRC2(kwLmmCleanGblRsrcs)   
 
   /* Deregister the timers */
   (Void) SDeregTmr(kwCb.init.ent, kwCb.init.inst,
                     (S16)kwCb.genCfg.timeRes, (PFS16)kwActvTmr);

   //(Void) SDeregTmr(kwCb.pjInit.ent, kwCb.pjInit.inst,
   //                  (S16)kwCb.genCfg.timeRes, (PFS16)pjActvTmr);

   kwSapSize = (Size)((Size)kwCb.genCfg.maxKwuSaps * (Size)sizeof(KwKwuSapCb));
   if (kwCb.kwuSap != (KwKwuSapCb *)NULLP)
   {
      KW_FREE(kwCb.kwuSap, kwSapSize);
   }

   (Void) SPutSMem(kwCb.init.region, kwCb.init.pool);
   kwCb.init.cfgDone = FALSE;
   kwCb.init.acnt = FALSE;
   kwCb.init.trc = FALSE;

   kwCb.init.usta = FALSE;

#ifdef KW_PDCP
   ent = kwCb.pjInit.ent;
#endif /* KW_PDCP */

   kwActvInit (kwCb.init.ent, kwCb.init.inst,
                        kwCb.init.region, kwCb.init.reason);

#ifdef KW_PDCP
   pjActvInit (ent, kwCb.init.inst,
                        kwCb.init.region, kwCb.init.reason);
#endif /* KW_PDCP */
   /*kw004.201 Adding of Missing Trace in LTE RLC PDCP*/
   RETVOID;
} /* kwLmmCleanGblRsrcs */

/**
 * @brief
      Delete all SAPs and Control Blocks
 *
 * @b Description
 *       Shutdown of RLC happens at modules level, tm, um and am modules will be
 *       shutdown using utility functin and dbm shutdown will clean up the SAPs,
 *       control blocks and lists. 
 *
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_HASHING_FAILED 
 */
#ifdef ANSI
PRIVATE S16 kwLmmShutdown
(
Void
)
#else
PRIVATE S16 kwLmmShutdown()
#endif
{
   KwRguSapCb *rguSap;

   TRC2(kwLmmShutdown)

   rguSap = &(kwCb.rguSap); 
   KwLiRguUbndReq(&(rguSap->pst), rguSap->spId, 0);

   kwDbmShutdown();
   kwLmmCleanGblRsrcs();

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of kwLmmShutdown */

/**
 * @brief
     Function processes the general control request 
 *
 *  @param[in] cntrl RLC LM structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SUBACTION
 *                   LCM_REASON_INVALID_ACTION
 */
#ifdef ANSI
PRIVATE S16 kwLmmGenCntrl
(
KwMngmt *cntrl                 /* RLC LM structure */
)
#else
PRIVATE S16 kwLmmGenCntrl(cntrl)
KwMngmt *cntrl;                /* RLC LM structure */
#endif
{
   U8 action;            /* action parameter */
   U8 sAction;           /* subaction field */
   KwTrcCntrl *trcCntrl; /* trace */

#ifdef DEBUGP
   KwDbgCntrl *dbgCntrl; /* debug */
#endif /* DEBUGP */

   S16   reason;         /* reason for failure */
   
   TRC2(kwLmmGenCntrl);

   reason = LCM_REASON_NOT_APPL;

   /* Validate control parameters */
   reason = kwLmmValidateGenCntrl (cntrl);

   action = cntrl->t.cntrl.action;
   sAction = cntrl->t.cntrl.subAction;

   if (reason != LCM_REASON_NOT_APPL)
   {
      RETVALUE(reason);
   }

   switch(action)
   {
      case AENA:
      {
         switch(sAction)
         {
            case SAUSTA:
            {
               KW_SET_USTA_FLAG(kwCb, TRUE);
               break;
            }
            case SADBG:
            {
#ifdef DEBUGP
               dbgCntrl = &cntrl->t.cntrl.s.dbgCntrl;
               KW_GET_DBG_MASK(kwCb) |= dbgCntrl->dbgMask;
#endif /* DEBUGP */
               break;
            }
            case SATRC:
            {
               trcCntrl = &cntrl->t.cntrl.s.trcCntrl;
               kwCb.init.trc = TRUE;
               kwCb.trcLen = trcCntrl->trcLen;
               (kwCb.trcMask) |= trcCntrl->trcMask;
                break;
            }
            case SALOG:
            {
               kwCb.init.logMask = cntrl->t.cntrl.s.logMask; 
               break;
            }
         }
         break;
      }
      case ADISIMM:
      {
         switch(sAction)
         {
            case SAUSTA:
            {
               kwCb.init.usta = FALSE;
            }
            break;
            case SADBG:
            {
#ifdef DEBUGP
               dbgCntrl = &cntrl->t.cntrl.s.dbgCntrl;
               KW_GET_DBG_MASK(kwCb) &= ~(dbgCntrl->dbgMask);
#endif /* DEBUGP */
            }
            break;
            case SATRC:
            {
               trcCntrl = &cntrl->t.cntrl.s.trcCntrl;
               kwCb.init.trc = FALSE;
               kwCb.trcMask &= ~(trcCntrl->trcMask);
            }
            break;
            case SALOG:
            {
               kwCb.init.logMask = cntrl->t.cntrl.s.logMask; 
            }
            break;

         }
         break;
      }
      case ASHUTDOWN:
      {
         reason = kwLmmShutdown();
         break;
      }
   }
   RETVALUE(reason);
} /* kwLmmGenCntrl */

/**
 * @brief
     Bind/Unbind RLC lower sap 
 *
 *  @param[in] cntrl RLC LM structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SAP 
 *                   LCM_REASON_INVALID_STATE
 *                   LCM_REASON_INVALID_ACTION 
 */
#ifdef ANSI
PRIVATE S16 kwLmmLSapCntrl
(
KwMngmt *cntrl                 /* RLC LM structure */
)
#else
PRIVATE S16 kwLmmLSapCntrl(cntrl)
KwMngmt *cntrl;                /* RLC LM structure */
#endif
{
   U8         action;                  /* action parameter */
   S16        reason;                 /* reason for failure */
   KwRguSapCb *rguSap;                /* rgu sap pointer */

   TRC2(kwLmmLSapCntrl)

   reason = LCM_REASON_NOT_APPL;
   action = cntrl->t.cntrl.action;
   rguSap = &(kwCb.rguSap);

   /* validate SuId */
   if((cntrl->t.cntrl.s.sapCntrl.suId < 0)
   || (cntrl->t.cntrl.s.sapCntrl.suId >= KW_MAX_RGUSAPS))
   {
      reason = LCM_REASON_INVALID_SAP;
      RETVALUE(reason);
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* validate SAP */
   if(kwCb.rguSap.state == KW_SAP_NOT_CFG)
   {
      KWLOGERROR(ERRCLS_INT_PAR, EKW048, (ErrVal) kwCb.rguSap.state,
               "kwLmmLSapCntrl: Invalid rguSap State");
      reason = LCM_REASON_INVALID_STATE;
      RETVALUE(reason);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   switch(action)
   {
      case ABND:
      {
         if(kwCb.rguSap.state != KW_SAP_BND)
         {
            /* start timer to wait for bind confirm */
            kwStartTmr((PTR)&(kwCb.rguSap), KW_EVT_WAIT_BNDCFM);
            kwCb.rguSap.state = KW_SAP_BINDING;
            KwLiRguBndReq(&(rguSap->pst), rguSap->suId, rguSap->spId);
         }
         else
         {
            /* control request received for an already bound SAP */
            reason = LCM_REASON_INVALID_STATE;
         }
      }
      break;
      case AUBND:
      {
         /* make the state of RGUSAP is configure but not bound */
         kwCb.rguSap.state = KW_SAP_CFG;
         KwLiRguUbndReq(&(rguSap->pst), rguSap->spId, 0);
      }
      break;
      default:
         reason = LCM_REASON_INVALID_ACTION;
         break;
   } /* end of switch */

   RETVALUE(reason);
} /* kwLmmLSapCntrl */

/**
 * @brief
     forms Lm Cfm based on the return values 
 *
 *  @param[in] cntrl RLC LM structure
 *  @return  S16
 *      -# Success : ROK 
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 kwLmmFormLmCfm
(
Pst      *pst,
Header   *hdr,
Reason   reason
)
#else
PRIVATE S16 kwLmmFormLmCfm(pst, hdr, reason)
Pst      *pst;
Header   *hdr;
Reason   reason;
#endif
{
   KwMngmt  rCfm;
   S16      ret;
   TRC3(kwLmmFormLmCfm)

   ret = ROK;

   if (reason == LCM_REASON_NOT_APPL)
   {
      rCfm.cfm.status = LCM_PRIM_OK;
      rCfm.cfm.reason = LCM_REASON_NOT_APPL;

      ret = ROK;
   }
   else
   {
      rCfm.cfm.status = LCM_PRIM_NOK;
      rCfm.cfm.reason = reason;

      ret = RFAILED;
   }

   kwLmmSendCfm(pst, &rCfm, TCNTRL, hdr);

   RETVALUE(ret);
} /* kwLmmFormLmCfm */

/**
 * @brief
     Function gather the general KWU SAP status 
 *
 *  @param[in] sta LM KWU Sap Status structure 
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_PAR_VAL
 */
#ifdef ANSI
PRIVATE S16 kwLmmGetKwuSapSta 
(
KwKwuSapSta *sta                /* RLU SAP status */
)
#else
PRIVATE S16 kwLmmGetKwuSapSta(sta)
KwKwuSapSta *sta;               /* RLU SAP status */
#endif
{
   KwKwuSapCb *kwSapCb;
   
   TRC2(kwLmmGetKwuSapSta);

   /* Validate the protocol parameters */
   if (sta->spId >= (S16)kwCb.genCfg.maxKwuSaps)
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   /* Get Sap control block */
   kwSapCb = (KwKwuSapCb*)(kwCb.kwuSap + sta->spId);

   sta->state = kwSapCb->state;

   RETVALUE(LCM_REASON_NOT_APPL);
} /* kwLmmGetKwuSapSta */

/**
 * @brief
     Function gather the general RGU SAP status
 *
 *  @param[in] sta LM RGU Sap Status structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_PAR_VAL
 */
#ifdef ANSI
PRIVATE S16 kwLmmGetRguSapSta 
(
KwRguSapSta *sta                /* RLU SAP status */
)
#else
PRIVATE S16 kwLmmGetRguSapSta(sta)
KwRguSapSta *sta;               /* RLU SAP status */
#endif
{
   TRC2(kwLmmGetRguSapSta);

   /* Validate the protocol parameters */
   if((sta->suId >= KW_MAX_RGUSAPS) || (sta->suId < 0))
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   sta->state = kwCb.rguSap.state;

   RETVALUE(LCM_REASON_NOT_APPL);
} /* kwLmmGetRguSapSta */

/**
 * @brief
     Function gather the general CKW SAP status
 *
 *  @param[in] sta LM CKW Sap Status structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_PAR_VAL
 */
#ifdef ANSI
PRIVATE S16 kwLmmGetCkwCntSapSta
(
KwCkwCntSapSta *sta              /* RLU SAP status */
)
#else
PRIVATE S16 kwLmmGetCkwCntSapSta(sta)
KwCkwCntSapSta *sta;             /* RLU SAP status */
#endif
{
   TRC2(kwLmmGetCkwCntSapSta);

   /* Validate config parameters */
   if (sta->spId >= KW_MAX_CKWSAPS )
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   sta->state = kwCb.ckwSap.state;

   RETVALUE(LCM_REASON_NOT_APPL);

} /* kwLmmGetCkwCntSapSta */

/**
 * @brief
     Gather the general statistics 
 *
 *  @param[in] sts       LM general statistics structure
 *  @param[in] action    action
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_PAR_VAL
 */
#ifdef ANSI
PRIVATE S16 kwLmmGetGenSts
(
KwGenSts *sts,              /* general statistics structure */
Action action               /* Action */
)
#else
PRIVATE S16 kwLmmGetGenSts(sts, action)
KwGenSts *sts;              /* general statistics structure */
Action action;              /* Action */
#endif
{
   KwGenSts *genSts;        /* General statistics */

   TRC2(kwLmmGetGenSts);

   /* Validate protocol parameters */
   if ((action != LKW_ZEROSTS) && (action != LKW_NOZEROSTS))
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   KW_MEM_CPY (sts, &(kwCb.genSts), sizeof (KwGenSts));
   
   if(action == LKW_ZEROSTS)
   {
      genSts = &(kwCb.genSts);
      KW_MEM_SET (genSts, 0, sizeof (KwGenSts));
   }

   RETVALUE(LCM_REASON_NOT_APPL);
} /* kwLmmGetGenSts */

/**
 * @brief
     Gather the SAP statistics
 *
 *  @param[in] sts     LM general statistics structure
 *  @param[in] elmnt   element 
 *  @param[in] action  action 
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_PAR_VAL
 */
#ifdef ANSI
PRIVATE S16 kwLmmGetSapSts
(
KwMngmt *sts,               /* RLC layer management */
Elmnt elmnt,                /* element */
Action action               /* Action */
)
#else
PRIVATE S16 kwLmmGetSapSts(sts, elmnt, action)
KwMngmt *sts;               /* RLC layer management */
Elmnt elmnt;                /* element */
Action action;              /* Action */
#endif
{
   KwKwuSapCb   *kwuSapCb;      /* RLU SAP control block */
   KwKwuSapSts  *kwuSap;       /* place holder for SAP statistics */
   KwCkwCntSts  *ckwSap;       /* place holder for SAP statistics */

   TRC2(kwLmmGetSapSts);

   /* Validate protocol parameters */
   if (action != LKW_ZEROSTS && action != LKW_NOZEROSTS)
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   switch(elmnt)
   {
      case STKWUSAP:
      {
         kwuSap = &sts->t.sts.s.kwuSap;

         /* Get Sap control block */
         /* kw005.201, modified the element of kwuSap from suId to spId */
         kwuSapCb = (KwKwuSapCb*)(kwCb.kwuSap + kwuSap->spId);

         kwuSap->sduRx = kwuSapCb->sts.sduRx;
         kwuSap->sduTx = kwuSapCb->sts.sduTx;
         /* kw006.201 fixed the spId assignement error. Fixed the defect ccpu00120260 */
         kwuSap->spId  = kwuSapCb->spId;

         if (action == LKW_ZEROSTS)
         {
            KW_MEM_SET (&kwuSapCb->sts, 0, sizeof (KwKwuSapSts));
         }
         break;
      }
      /* kw005.201 removed RGUSAP statistics support */
      case STCKWSAP:
      {
         ckwSap = &sts->t.sts.s.ckwSap;

         ckwSap->statMsgs = kwCb.ckwSap.sts.statMsgs;

         if (action == LKW_ZEROSTS)
         {
            KW_MEM_SET (&(kwCb.ckwSap.sts), 0, sizeof (KwCkwCntSts));
         }
         break;
      }
      default:
         RETVALUE(LCM_REASON_INVALID_ELMNT);
   }

   SGetDateTime(&sts->t.sts.dt);

   RETVALUE(LCM_REASON_NOT_APPL);
} /* kwLmmGetSapSts */


/**
 * @brief
    This function sends Unsolicited Status Indication to the Layer Management Entity. 
 *
 *  @param[in] category  Category 
 *  @param[in] event     event
 *  @param[in] cause     cause
 *  @param[in] UeId      ueId 
 *  @param[in] Qci        qci 
 *  @return  Void
 */
#ifdef LTE_L2_MEAS
#ifdef ANSI
PUBLIC Void kwLmmSendAlarm
(
U16      category,  /* Category */
U16      event,     /* Alarm event */
U16      cause,     /* Alarm cause */
SuId     suId,      /* suId */
U32      ueId,      /* ueId */
U8       qci        /* qci */
)
#else
PUBLIC Void kwLmmSendAlarm(category, event, cause, suId, ueId, qci)
U16      category;  /* Category */
U16      event;     /* Alarm event */
U16      cause;     /* Alarm cause */
SuId     suId;      /* suId */
U32      ueId;      /* ueId */
U8       qci;       /* qci */
#endif
#else /* LTE_L2_MEAS */
#ifdef ANSI
PUBLIC Void kwLmmSendAlarm
(
U16      category,  /* Category */
U16      event,     /* Alarm event */
U16      cause,     /* Alarm cause */
SuId     suId,      /* suId */
U32      ueId       /* ueId */
)
#else
PUBLIC Void kwLmmSendAlarm(category, event, cause, suId, ueId)
U16      category;  /* Category */
U16      event;     /* Alarm event */
U16      cause;     /* Alarm cause */
SuId     suId;      /* suId */
U32      ueId;      /* ueId */
#endif
#endif /* LTE_L2_MEAS */
{

   KwMngmt usta;    /* Rlc Management Structure */

   TRC2(kwLmmSendAlarm);

   if(kwCb.init.usta == FALSE)
   {
      RETVOID;
   }

   /* initialize the management structure */
   KW_MEM_SET(&usta, 0, sizeof(KwMngmt));

   usta.hdr.elmId.elmnt = STGEN;
   usta.hdr.entId.ent = kwCb.init.ent;
   usta.hdr.entId.inst = kwCb.init.inst;

   /* fill in the event and category */
   usta.t.usta.alarm.category = category;
   usta.t.usta.alarm.event = event;
   usta.t.usta.alarm.cause = cause;

   /* set the suId and ueId */
   usta.t.usta.ueId = ueId;
   usta.t.usta.suId = suId;
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   if(event == LKW_EVT_MEAS_HALT)
   {
      usta.t.usta.qci = qci;
   }
#endif /* LTE_L2_MEAS */
   /* update the date and time */
   (Void) SGetDateTime(&usta.t.usta.alarm.dt);

   KwMiLkwStaInd(&(kwCb.init.lmPst), &usta);

   RETVOID;

} /* kwLmmSendAlarm */

/**
 * @brief
    This function sends trace indication to LM 
 *
 *  @param[in] event    event
 *  @param[in] mBuf     meessage buffer
 *  @return S16
 *      -# Success : ROK 
 *      -# Failure : RFAILED 
 */
#ifdef ANSI
PUBLIC S16 kwLmmSendTrc
(
Event event,                 /* event */
Buffer *mBuf                 /* message buffer */
)
#else
PUBLIC S16 kwLmmSendTrc(event, mBuf)
Event event;                 /* event */
Buffer *mBuf;                /* message buffer */
#endif
{
   /* patch kw004.201 Corrected kwLmmSendTrc function definition */
   KwMngmt trc;              /* RLC management control block */
   Buffer   *dstMbuf;   
   MsgLen   bufLen;
   Data     *tempBuf;
   MsgLen   tempCnt;
   Pst      pst;

   TRC2(kwLmmSendTrc)

   KWDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF), (_kwPBuf,
         "\nkwLmmSendTrc(): Trace for event=%d, kwCb.trcLen=%d",event,
   kwCb.trcLen));

   cmMemset((U8 *)&trc, 0, sizeof(KwMngmt));

   pst = kwCb.init.lmPst;

   trc.t.trc.event = event;
   SGetDateTime(&trc.t.trc.dt);
   trc.cfm.status = LCM_PRIM_OK;
   trc.cfm.reason = LCM_REASON_NOT_APPL;
         
   if(mBuf != NULLP)
   {
     /* Check if the whole buffer is to be sent in Trace indication */
     if(kwCb.trcLen == LKW_FULL_TRACE)
     {
        if (SCpyMsgMsg(mBuf, pst.region, pst.pool, &dstMbuf)
            != ROK)
        {
           /* rg005.201 removed SPutSBuf on error */
           KWDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF),(_kwPBuf,
                 "\nkwLmmSendTrc(): SCpyMsgMsg Failed."));
           RETVALUE(RFAILED);
        }
        /* Send Trace Indication to Layer manager */
        KwMiLkwTrcInd(&pst, &trc, dstMbuf);
     }
     /* check if only a specified number of bytes are to be sent */
     else if(kwCb.trcLen > 0)
     {
        /* Get the length of the recvd message buffer */
        if (SFndLenMsg(mBuf, &bufLen) != ROK)
        {
           KWDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF),(_kwPBuf, 
                    "\nkwLmmSendTrc(): SFndLenMsg Failed."));
           RETVALUE(RFAILED);
        }
        /* Check if the recvd buffer size is less than request trace len */
        if(bufLen < kwCb.trcLen)
        {
           /* Copy the whole of the recvd buffer in trace indication */
      
           if (SCpyMsgMsg(mBuf, pst.region, pst.pool, &dstMbuf)
              != ROK)
           {
              KWDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF),(_kwPBuf,
                      "\nkwLmmSendTrc(): SCpyMsgMsg Failed."));
              RETVALUE(RFAILED);
           }
         
           /* Send Trace Indication to Layer manager */
           KwMiLkwTrcInd(&pst, &trc, dstMbuf);
        }
        /* if the recvd buffer size is greater than request trace len */
        if(bufLen >= kwCb.trcLen)
        {
           /* Get a temporary buffer to store the msg */
           KW_ALLOC(tempBuf, kwCb.trcLen);

#if (ERRCLASS & ERRCLS_INT_PAR)
           if(tempBuf == NULLP)
           {
              (Void) SPutSMem(KW_GET_MEM_REGION(kwCb), KW_GET_MEM_POOL(kwCb));
      
               KWLOGERROR(ERRCLS_INT_PAR, EKWXXX, (ErrVal)0 ,
                   "kwLmmSendTrc: SGetSBuf Failed for tempBuf...!");
               RETVALUE(LCM_REASON_MEM_NOAVAIL);
           }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         
           /* Copy trcLen nos of bytes from the recvd message */
           if (SCpyMsgFix(mBuf,0,kwCb.trcLen,tempBuf,&tempCnt) != ROK)   
           {
              KWDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF), (_kwPBuf,
                      "\nkwLmmSendTrc(): SCpyMsgFix Failed."));
              RETVALUE(RFAILED);
           }

           if (SGetMsg(pst.region, pst.pool, &dstMbuf) != ROK)
           {
              KWDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF), (_kwPBuf,
                      "\nkwLmmSendTrc(): dstMbuf Allocation Failed."));
              RETVALUE(RFAILED);
           }
           /* Copy the tempBuf data to dst mBuf */
           if (SCpyFixMsg(tempBuf,dstMbuf,0,kwCb.trcLen,&tempCnt) != ROK)
           {
              KWDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF), (_kwPBuf,
                     "\nkwLmmSendTrc(): SCpyFixMsg Failed."));
              RETVALUE(RFAILED);
           }
  
           /* Free the memory allocated for tempBuf */
           KW_FREE(tempBuf, kwCb.trcLen);
           /* Send Trace Indication to Layer manager */
           KwMiLkwTrcInd(&pst, &trc, dstMbuf);
        }
     }
   }
   else
   {
      KwMiLkwTrcInd(&pst, &trc, mBuf);
   }

   RETVALUE(ROK);
} /* end of kwLmmSendTrc */

  
/*
*
*       Fun:   Activate Task - timer
*
*       Desc:  Invoked by system services to activate a task with
*              a timer tick.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  gp_lmm.c
*
*/
 
#ifdef ANSI
PUBLIC S16 kwActvTmr
(
Void
)
#else
PUBLIC S16 kwActvTmr()
#endif
{
   
   
   TRC2(kwActvTmr)

   cmPrcTmr(&(kwCb.kwTqCp), kwCb.kwTq, (PFV) kwTmrExpiry);

   RETVALUE(ROK);

} /* end of kwActvTmr */

/* LOG_COUNT 0x60700004 */
#ifdef PWAV_HC
PUBLIC S16 sendKwMsgToFrm(
    Ent     dstEnt,
    Event   event,
    U16     len,
    U32     *data
    )
{
  Pst   pst;
  Buffer *mBuf = NULLP;

  if (SGetMsg(DFLT_REGION, DFLT_POOL, &mBuf) != ROK)
  {
    MSPD_DBG("Error in Getting Message buffer \n");
  }
  cmMemset((U8*)&pst, '\0', sizeof(Pst));

  pst.selector  = 0;
  pst.srcEnt    = ENTKW;
  pst.srcProcId = 1; /* RADIO_PROC_ID */
  pst.dstEnt    = dstEnt;
  pst.dstProcId = 4; /* OAM_PROC_ID */
  pst.event     = event;

  /* Add data to mBuf here.
   */
  if( (len > 0) && (NULL != data) )
  {
    SAddPstMsgMult((Data *)data, len, mBuf);
  }

  if(SPstTsk(&pst, mBuf) !=ROK)
  {
    return RFAILED;
  }
  else
  {
    return ROK;
  }
}


PUBLIC S16 sendKwMsgToPrm(
    Event   event,
    U16     len,
    U32     *data
    )
{
  /* Add data to mBuf here.
   * There is no data in these functions.
   * So mBuf is allocated but empty.
   */
  if( RFAILED == sendKwMsgToFrm( ENT_PRM, event, len, data ))
  {
    return RFAILED;
  }
  else
  {
    return ROK;
  }
}

void sendKwStartIndToPrm(void)
{
 
  U32 version = VERSION_MODID_RLC;
  /*send start indication to PRM indicating successful startup*/
  if( sendKwMsgToPrm(EVT_START_IND, sizeof(U32), &version ) == ROK)
  {
    MSPD_DBG("Sent EVT_START_IND from KW to PRM \n");
  }
}

void sendKwHelloAckToPrm(Buffer   *mBuf)
{
  MsgLen              cCnt;
  MsgPrmModHello      hello;
  MsgModPrmHelloAck   helloAck;

  if (SCpyMsgFix( mBuf, 0, sizeof(MsgPrmModHello), (Data*)&hello , &cCnt ) != ROK)   
  {
    MSPD_DBG( "sendKwHelloAckToPrm(): SCpyMsgFix Failed\n" );
    RETVOID;
  }
  helloAck.helloAckSeqNum = hello.helloSeqNum;

  if(sendKwMsgToPrm(EVT_HELLO_ACK, sizeof(MsgModPrmHelloAck), (U32*)&helloAck) == ROK)
  {
    MSPD_DBG("Sent EVT_HELLO_ACK from KW to PRM for sequence number:%d \n", helloAck.helloAckSeqNum );
  }
  else
  {
    MSPD_DBG("Sent EVT_HELLO_ACK from KW to PRM failed for sequence number:%d \n", helloAck.helloAckSeqNum );
  }
}

void procRcvdKwExitMsg(void)
{
  MSPD_DBG("Exiting the KW \n" );
  exit(0);
}
#endif

/********************************************************************30**

         End of file:     gp_lmm.c@@/main/2 - Fri Nov 13 14:13:42 2009

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
/main/1      ---       gk   1. initial release.
/main/2      ---       nm   1. LTERLC Release 2.1
/main/3      kw002.201 vp   1. Patch for optimization,fixes for multi-region
                               and retransmission.
/main/4      kw004.201 ap   1. Corrected kwLmmSendTrc function definition
/main/5      kw005.201 ap   1. Modified the element of kwuSap from suId to spId.
                       rd   2. Added code for populating ueId,cellId in measCb
                               from measReqEvt in KwMiLkwL2MeasReq.
                            3. Modified KwMiLkwL2MeasReq after moving measType
                               to KwL2MeasCfmEvt.
/main/6      kw006.201 rd   1. Moved out few portions of KwMiLkwL2MeasReq into
                               2 functions:kwUtlProcInitialActvUe and 
                               kwUtlNotifyMacUlIp.
                            2. Optimization change, fix for ccpu00111716
                       ap   3. Fixed the spId assignement error. 
                               Fixed the defect ccpu00120260.
                            4. ccpu00120574, fixed the warning.
                       rd   5. Added initialization of selfPstUl for 
                               ccpu00117290.
                            6. Allocation of mBuf is done in kwLmmGenCfg.
*********************************************************************91*/
