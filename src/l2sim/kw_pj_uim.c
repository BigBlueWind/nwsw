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
  
        Name:    LTE-PDCP Layer - Upper Interface Functions
    
        Type:    C file
  
        Desc:    Source code for PDCP Upper Interface Module
                 This file contains following functions
      
        --PjUiCpjBndReq
        --PjUiCpjUbndReq
        --PjUiCpjCfgReq
        --PjUiCpjUeIdChgReq
        --PjUiCpjReEstReq
        --PjUiCpjSecCfgReq
        --PjUiCpjSduStaReq
     

        --PjUiPjuBndReq
        --PjUiPjuUbndReq
        --PjUiPjuDatReq
        --PjUiPjuSduStaReq 

     File:     kw_pj_uim.c

     Sid:      gp_pj_uim.c@@/main/1 - Fri Nov 13 14:13:49 2009

     Prg:      vp

**********************************************************************/
/** @file gp_pj_uim.c
@brief PDCP Upper Interface Module 
*/


/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000372     LTE-MAC
 *      1000369     LTE-RRC
 */


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
#include "lpj.h"           /* LPJ define */
#include "cpj.h"           /* RRC layer */
#include "pju.h"           /* PDCP service user */
#include "kw_env.h"        /* RLC environment options */
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
#include "lpj.x"           /* LM(PDCP) */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */
#include "cpj.x"           /* RRC layer */
#include "pju.x"           /* PDCP service user */
#include "kw_err.h"
#include "kw.x"

/* kw004.201 Assigned build issue in LTE RLC kw_pj_uim.c */
#include "kw_pj_lib.x"
#ifdef CTF_AS_SEC_ENB
#if 0
#include "ys_ms_spacc.h"
#include "ys_ms_spacc.x"
#endif 
#endif

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */



/*****************************************************************************
 *                          CPJ INTERFACE
 ****************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for binding the PDCP upper layer service user with 
 *        the PDCP layer.
 *
 * @b Description:
 *
 *        1. This function is used by PDCP user to request for binding to 
 *        PDCP. @n
 *        2. This function is called by the CPJ interface to bind 
 *        PDCP's SAP (identified by spId) with the service user's
 *        SAP (identified by suId). @n
 *
 *  @param[in] pst   Post structure  
 *  @param[in] suId  Service user SAP ID 
 *  @param[in] spId  Service provider ID
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjBndReq 
(
Pst  *pst,   
SuId suId,  
SpId spId   
)
#else
PUBLIC S16 PjUiCpjBndReq (pst, suId, spId)
Pst *pst;    
SuId suId;   
SpId spId;   
#endif
{
   KwCpjSapCb      *cpjSap;            /* pointer to session SAP */
   S16             ret;                /* return value */

   TRC3(PjUiCpjBndReq)

   ret = ROK;

   PJDBGP((KW_DBGMASK_DL | KW_DBGMASK_BRIEF), (_kwPBuf,
            "PjUiCpjBndReq(pst, spId(%d), suId(%d))\n", spId, suId));

   cpjSap = &(kwCb.cpjSap);

   /* Verify CPJ SAP State */
   switch(cpjSap->state)
   {
      /* SAP is configured but not bound */
      case PJ_SAP_CFG:
      case PJ_SAP_UBND:
      {
         /* copy bind configuration parameters in SSAP sap */
         cpjSap->suId = suId;
         cpjSap->pst.dstProcId = pst->srcProcId;
         cpjSap->pst.dstEnt = pst->srcEnt;
         cpjSap->pst.dstInst = pst->srcInst;

         /* Update the State */
         cpjSap->state = PJ_SAP_BND;

         PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_DETAIL), (_kwPBuf,
                  "PjUiCpjBndReq: state (%d)\n", cpjSap->state));
         break;
      }

      /* SAP is already bound */
      case PJ_SAP_BND:
      {
         /* 
          * Sap is already bound check source, destination Entity and 
          * Proc Id
          */
         if (cpjSap->pst.dstProcId != pst->srcProcId 
               || cpjSap->pst.dstEnt != pst->srcEnt
               || cpjSap->pst.dstInst != pst->srcInst
               || cpjSap->suId != suId)
         {
            PJ_SEND_SAPID_ALARM(spId, LPJ_EVENT_CPJ_BND_REQ, LCM_CAUSE_INV_PAR_VAL);

            PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                     "PjUiCpjBndReq: SAP already bound -- state(%d)\n", cpjSap->state));
            ret = RFAILED;
         }
         break;
      }

     default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         /* LOG ERROR */
         PJLOGERROR(ERRCLS_INT_PAR, EKW081, (ErrVal) spId,
               "PjUiCpjBndReq: CPJ SAP State is Invalid during Bind Req");

         PJ_SEND_SAPID_ALARM(spId, LPJ_EVENT_CPJ_BND_REQ, LCM_CAUSE_INV_STATE);
#endif /* ERRCLASS & ERRCLS_INT_PAR */

         PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                  "PjUiCpjBndReq: CPJ SAP State is Invalid during Bind Req"));

         ret = RFAILED;
         break;
      }
   }

   if (ret == ROK)
   {
      PjUiCpjBndCfm(&(cpjSap->pst), cpjSap->suId, CM_BND_OK);
   }
   else
   {
      PjUiCpjBndCfm(&(cpjSap->pst), cpjSap->suId, CM_BND_NOK);
   }

   RETVALUE(ROK);
} /* PjUiCpjBndReq */


/**
 *
 * @brief 
 *
 *        Handler for unbinding the RLC upper layer service user CPJ with 
 *        the RLC layer.
 *
 * @b Description:
 *
 *        1. This function is used by RLC user to request for unbinding  
 *        with RLC. @n
 *        2. This function is called by the CPJ interface to 
 *        unbind with RLC. @n
 *
 *  @param[in] pst     Post structure  
 *  @param[in] spId    Service provider SAP ID 
 *  @param[in] reason  Reason for Unbinding 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjUbndReq
(
Pst      *pst,            
SpId     spId,           
Reason   reason           
)
#else
PUBLIC S16 PjUiCpjUbndReq(pst, spId, reason)
Pst      *pst;       
SpId     spId;      
Reason   reason;      
#endif
{
   KwCpjSapCb  *cpjSap;            /* CPJ SAP control block */
   /* kw006.201, ccpu00120574, fixed the warning */
#if (ERRCLASS & ERRCLS_INT_PAR)
   S16         ret;                /* Return Value */
#endif

   TRC3(PjUiCpjUbndReq)

   PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
            "PjUiCpjUbndReq(pst, spId(%d), reason(%d))\n", spId, reason));

   UNUSED(pst);
   UNUSED(reason);

   /* disable upper sap (CPJ) */
   cpjSap = &(kwCb.cpjSap);
   /* kw006.201, ccpu00120574, fixed the warning */
#if (ERRCLASS & ERRCLS_INT_PAR)
   ret    = ROK;
#endif

#if (ERRCLASS & ERRCLS_INT_PAR)
   PJ_GET_AND_VALIDATE_CPJSAP(cpjSap, EKW082, "PjUiCPjUbndReq", ret);
   if ( ret != ROK )
   {
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   cpjSap->state = PJ_SAP_CFG;

   RETVALUE(ROK);
} /* CpUiCpjUbndReq */

#if 0

/**
 *
 * @brief 
 *
 *        Handler for configuring PDCP entities.
 *
 * @b Description:
 *
 *        This function is used by RRC to configure(add/delete/modify/
 *        reestalish/delete all RB in a UE) one or more PDCP entities. 
 *
 *        - CPJ_CFG_ADD          => pjCfgAddRb
 *        - CPJ_CFG_MODIFY       => pjCfgReCfgRb
 *        - CPJ_CFG_DELETE       => pjCfgDelRb
 *        - CPJ_CFG_REESTABLISH  => pjCfgReEstRb
 *        - CPJ_CFG_DELETE_UE    => pjCfgDelUe
 *
 *  @param[in] pst   -  Post structure  
 *  @param[in] spId  -  Serive Provider ID 
 *  @param[in] cfg   -  Configuration information for one or more PDCP entities. 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjCfgReq
(
Pst             *pst,
SpId            spId,
CpjCfgReqInfo   *cfg
)
#else
PUBLIC S16 PjUiCpjCfgReq(pst, spId, cfg)
Pst            *pst;
SpId           spId;
CpjCfgReqInfo  *cfg;
#endif
{
   CpjCfgCfmInfo        *cfgCfm;       /* Configuraiton Confirm */
   U8                   idx;           /* Index */
   KwCpjSapCb           *cpjSap;       /* CPJ SAP Control Block */
   Bool                 reEstPres;     /* ReEstablishment present */
   Bool                 cfmWait;       /* Add present */
   Bool                 ueDelWait;     /* Add present */
   S16                  ret;           /* Return Value */
   KwUeCb               *ueCb;         /* UE Control Block for storing the
                                          cfg structure in case of ReEstablishment */
   U8                   srb1Only;
   RbCb                 *rbCb;
#ifdef MSPD_MLOG_NEW
   U32                  t1, t2, t_reEst;
#endif

   TRC3(PjUiCpjCfgReq)

      reEstPres = FALSE;
   cfmWait   = FALSE;
   ueDelWait = FALSE;
   ret       = ROK;
   cfgCfm    = NULLP;
   idx       = 0;
   ueCb      = NULLP;
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,
            "PjUiPjuCfgReq(pst, spId(%d), TransId(%ld))\n", spId, cfg->transId));
#else
   PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,
            "PjUiPjuCfgReq(pst, spId(%d), TransId(%d))\n", spId, cfg->transId));
#endif

   /* Get the cpjSap */
   cpjSap = &(kwCb.cpjSap);

   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(spId, cpjSap, ret);
   if (ret != ROK)
   {
      /* kw002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, cfg, sizeof(CpjCfgReqInfo));
      RETVALUE(RFAILED);
   }
#ifdef MSPD_MLOG_NEW
   t1 = GetTIMETICK();
#endif

   /* Allocate memory and memset to 0 for cfmInfo */
   PJ_ALLOC(cfgCfm, sizeof(CpjCfgCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (cfgCfm == NULLP)
   {
      PJLOGERROR(ERRCLS_ADD_RES, EKW083, (ErrVal) RFAILED,
            "PjUiPjuCfgReq: cfgCfm allocation Failed.");
      /* kw002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, cfg, sizeof(CpjCfgReqInfo));
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

#ifdef PWAV_LARM_LOGS 
         printPdcpCfg(cfg);
#endif

   /* For every entity configuration process by cfgType */
   for (idx = 0; idx < cfg->numEnt; idx++)
   {
      CpjCfmEnt         *entCfgCfm;
      CpjCfgEnt         *entCfg;

      entCfg  = &(cfg->cfgEnt[idx]);
      entCfgCfm   = &(cfgCfm->cfmEnt[idx]);

      /* Fill the detault values here */
      PJ_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_OK,\
               CPJ_CFG_REAS_NONE);


      switch (entCfg->cfgType)
      {
         case CPJ_CFG_ADD:
            {
               /* Add a new RB entity configuration */
               if (pjCfgAddRb(cfg->ueId, cfg->cellId,\
                        entCfg, entCfgCfm) != ROK)
               {
                  PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
                        (_kwPBuf, "PjUiPjuCfgReq(pst, cfg(ueId(%d),\
                     cellId(%d), cfgType(%d), rbId(%d))\n kwCfgAddRb Failed: reason (%d)\n",
                         cfg->ueId, cfg->cellId, entCfg->cfgType,
                         cfg->cfgEnt[idx].rbId, entCfgCfm->reason));
               }
               /* Only In case of DRB only we need to set this flag */
#ifdef PJ_CMP_ASYNC
               else if(PJ_DRB == entCfg->rbType)
               {
                  if(entCfg->m.addCfg.hdrCompCfg.hdrCompUsed)
                  {
                     cfmWait = TRUE;
                  }
               }
#endif /* PJ_CMP_ASYNC */
               break;
            }

         case CPJ_CFG_MODIFY:
            {
               /* Re-configure the existing RB entity configuration */
               if (pjCfgReCfgRb(cfg->ueId, cfg->cellId,\
                        entCfg, entCfgCfm) != ROK)
               {
                  PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
                        (_kwPBuf, "PjUiPjuCfgReq(pst, cfg(ueId(%d),\
                     cellId(%d), cfgType(%d), rbId(%d))\n kwCfgReCfgRb Failed: reason (%d)\n",
                         cfg->ueId, cfg->cellId, entCfg->cfgType,
                         cfg->cfgEnt[idx].rbId, entCfgCfm->reason));
               }
#ifdef PJ_CMP_ASYNC
               else if(PJ_DRB == entCfg->rbType)
               {
                  if(entCfg->m.modCfg.hdrCompCfg.hdrCompUsed)
                  {
                     cfmWait = TRUE;
                  }
               }
#endif /* PJ_CMP_ASYNC */
               break;
            }

         case CPJ_CFG_DELETE:
            {
               /* Delete the existing RB entity configuration */
               if (pjCfgDelRb(cfg->ueId, cfg->cellId,\
                        entCfg, entCfgCfm) != ROK)
               {
                  PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
                        (_kwPBuf, "PjUiPjuCfgReq(pst, cfg(ueId(%d),\
                     cellId(%d), cfgType(%d), rbId(%d))\n kwCfgDelRb Failed: reason (%d)\n",
                         cfg->ueId, cfg->cellId, entCfg->cfgType,
                         cfg->cfgEnt[idx].rbId, entCfgCfm->reason));
               }
               break;
            }

         case CPJ_CFG_REESTABLISH:
            {
               /* Re-establish the existing RB entity configuration */
#ifdef MSPD_MLOG_NEW
   t_reEst = GetTIMETICK();
#endif
               if (pjCfgReEstRb(cfg->ueId, cfg->cellId,\
                        entCfg, entCfgCfm) != ROK)
               {
                  PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
                        (_kwPBuf, "PjUiPjuCfgReq(pst, cfg(ueId(%d),\
                     cellId(%d), cfgType(%d), rbId(%d))\n kwCfgReEstRb Failed: reason (%d)\n",
                         cfg->ueId, cfg->cellId, entCfg->cfgType,
                         cfg->cfgEnt[idx].rbId, entCfgCfm->reason));

               }
               else
               {
                  reEstPres= TRUE;
               }
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_PDCP_CFG_REQ_REEST, RESOURCE_LARM, t_reEst, GetTIMETICK());
#endif
               
               break;
            }

         case CPJ_CFG_DELETE_UE:
            {
               /* Delete all RB entity configuration under UE */
               if (pjCfgDelUe(cfg->ueId, cfg->cellId,\
                        entCfg, entCfgCfm) != ROK)
               {

                  PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
                        (_kwPBuf, "PjUiPjuCfgReq(pst, cfg(ueId(%d),\
                     cellId(%d), cfgType(%d), rbId(%d))\n kwCfgDelUe Failed: reason (%d)\n",
                         cfg->ueId, cfg->cellId, entCfg->cfgType,
                         cfg->cfgEnt[idx].rbId, entCfgCfm->reason));
               }
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
               else
               {
                  /* Wait for any messages in the Demand q to be processed */
                  ueDelWait   =  TRUE;
               }
#endif /* (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)) */
               break;
            }

         default:
            {
               PJ_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
                     CPJ_CFG_REAS_INVALID_CFG);

               PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
                     (_kwPBuf, "PjUiPjuCfgReq(pst, cfg(ueId(%d),\
                  cellId(%d), cfgType(%d), rbId(%d)) Failed: reason (%d)\n",
                     cfg->ueId, cfg->cellId, entCfg->cfgType,
                     cfg->cfgEnt[idx].rbId, CPJ_CFG_REAS_INVALID_CFG));
            }
      }
   }

#ifdef MSPD_MLOG_NEW
   MLogTask(PID_PDCP_CFG_REQ, RESOURCE_LARM, t1, GetTIMETICK());
   t2 = GetTIMETICK();
#endif
   /* Assign number of entity configuraitons and suId */
   cfgCfm->transId = cfg->transId;
   cfgCfm->ueId = cfg->ueId;
   cfgCfm->cellId = cfg->cellId;
   cfgCfm->numEnt = cfg->numEnt;

   ret = kwDbmFetchUeCb(cfg->ueId, cfg->cellId, &ueCb);


   if((ueCb != NULLP) && 
      ((reEstPres == TRUE)  || (ueDelWait == TRUE) || 
       (cfmWait == TRUE)))
   {
      U8 cfmType = 0;
      Bool  startTmr =  FALSE;

      if(ueDelWait   == TRUE)
      {
         cfmType  |=  PJ_CFG_UEDEL_ASYNC_CFM;
         startTmr =  TRUE;
      }
      if(reEstPres == TRUE)
      {
         cfmType  |=  PJ_CFG_REEST_ASYNC_CFM;
      }
      if(cfmWait == TRUE)
      {
         cfmType  |=  PJ_CFG_ASYNC_CFM;
         startTmr =  TRUE;
      }

      pjUtlSaveCfmInfo(ueCb, cfmType, startTmr, ENTPJ, (PTR)cfgCfm, (PTR)cfg);

      if(TRUE == reEstPres )
          pjUtlReEstStart(ueCb, cfg); 

      /* Free the cfg */
      /* kw002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, cfg, sizeof(CpjCfgReqInfo));
      PJ_FREE(cfgCfm, sizeof(CpjCfgCfmInfo));
   }
   else
   {
      /* kw002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, cfg, sizeof(CpjCfgReqInfo));
      PjUiCpjCfgCfm(&(cpjSap->pst), cpjSap->suId, cfgCfm);

   }

   /* SRB1 is in ReEstablishment state and Existing DRBs or SRB2
      have been deleted as a part of Cfg Request. In this case SRB1
      ReEstablishment confimration should be send */

   if((ROK   == ret)       && 
      (NULLP != ueCb)      &&
      (TRUE  != reEstPres) &&
      (ueCb->libInfo.state ==  PJ_STATE_REEST))
   {
      /* Search Only SRB1 is present */
      srb1Only = TRUE;
      for(idx=0; idx < KW_MAX_DRB_PER_UE; idx++)
      {
         rbCb = ueCb->drbCb[idx];
         if(rbCb && (rbCb->rbCfgStat&PJ_CFG_PDCP)) 
         {
            srb1Only=FALSE;
            break;
         }
      }
      if(srb1Only == TRUE)
      {
         /* Check SRB2 is present or not */
         rbCb = ueCb->srbCb[PJ_SRB2_ID];
         if(rbCb && (rbCb->rbCfgStat&PJ_CFG_PDCP)) 
         {
            srb1Only=FALSE;
            
         }
      }
      /* SRB1 is only present, Trigger ReEstConf if UE state is ReEst  */ 
      if(srb1Only == TRUE)
      {      
         pjUtlReEstReconfig(ueCb, PJ_SRB);
      }
   } 

#ifdef MSPD_MLOG_NEW
   MLogTask(PID_PDCP_CFG_REQ_REM, RESOURCE_LARM, t2, GetTIMETICK());
#endif


   RETVALUE(ret);
} /* PjUiPjuCfgReq */
/**
 *@details This primitive is used by RRC to change the UeId for the existing UE
           context.

 * @param pst     -  Point to the pst structure
 * @param spId    -  The ID of the service provider SAP in the PDCP layer 
 * @param transId -  Transaction ID. This field uniquily identifies
 *                   transaction between RRC and PDCP.
 * @param ueInfo    -  Old UE Id Info for which the change request has come 
 * @param newUeInfo -  New UE Id Info for existing UE context 
 * @return ROK
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjUeIdChgReq
(
Pst        *pst, 
SpId       spId, 
U32        transId, 
CpjUeInfo  *ueInfo,
CpjUeInfo  *newUeInfo
)
#else
PUBLIC S16 PjUiCpjUeIdChgReq(pst,spId,transId,ueInfo,newUeInfo)
Pst        *pst;
SpId       spId;
U32        transId;
CpjUeInfo  *ueInfo;
CpjUeInfo  *newUeInfo;
#endif
{
   S16         ret;           /* Return Value */
   CmStatus    status;        /* Status of the request */
   KwCpjSapCb  *cpjSap;       /* CPJ SAP Control Block */
   CpjUeInfo   *tUeInfo;      /* Temporary UE info */

   TRC3(PjUiCpjUeIdChgReq)
#ifndef ALIGN_64BIT
   PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,
            "PjUiCpjUeIdChgReq(pst, spId(%d), transId(%ld))\n", spId, transId));
#else
   PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,
            "PjUiCpjUeIdChgReq(pst, spId(%d), transId(%d))\n", spId, transId));
#endif

   ret = ROK;
   tUeInfo = NULLP;
   cpjSap = &(kwCb.cpjSap);
   status.reason = CPJ_CFG_REAS_NONE;
   status.status = CPJ_CFG_CFM_OK;
   
   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(spId, cpjSap, ret);
   if (ret != ROK)
   {
      /* kw002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, ueInfo, sizeof(CpjUeInfo));
      PJ_PST_FREE(pst->region, pst->pool, newUeInfo, sizeof(CpjUeInfo));
      RETVALUE(RFAILED);
   }

   if (pjCfgUeIdChng(ueInfo, newUeInfo, &status) != ROK)
   {
      PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
            (_kwPBuf, "PjUiCpjUeIdChgReq(pst, oldUe(ueId(%d),\
         cellId(%d)), newUe(ueId(%d), cellId(%d))\n kwCfgUeIdChng Failed: reason (%d)\n",
             ueInfo->ueId, ueInfo->cellId, newUeInfo->ueId, newUeInfo->cellId,
             status.reason));
   }

   /* Allocate memory for CpjUeInfo */
   PJ_ALLOC(tUeInfo, sizeof(CpjUeInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (tUeInfo == NULLP)
   {
      /* Free memory for CpjUeInfo */
      /* kw002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, ueInfo, sizeof(CpjUeInfo));
      PJ_PST_FREE(pst->region, pst->pool, newUeInfo, sizeof(CpjUeInfo));

      PJLOGERROR(ERRCLS_ADD_RES, EKW084, (ErrVal) RFAILED,
            "PjUiCpjUeIdChgReq: tUeInfo allocation Failed.");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   tUeInfo->ueId = ueInfo->ueId;
   tUeInfo->cellId = ueInfo->cellId;

   /* Free memory for CkwUeInfo */
   /* kw002.201 Freeing from proper region */
   PJ_PST_FREE(pst->region, pst->pool, ueInfo, sizeof(CpjUeInfo));
   PJ_PST_FREE(pst->region, pst->pool, newUeInfo, sizeof(CpjUeInfo));

   PjUiCpjUeIdChgCfm(&(cpjSap->pst), cpjSap->suId, transId, tUeInfo, status);

   RETVALUE(ROK);
} /* PjUiCpjUeIdChgReq */
/**
 *@details  
 * 1. RRC uses this primitive to configure PDCP security parameters.
 * Integrity protection and/or Ciphering are configured by RRC based on the
 * selSecAct flag.  @n
 * 2. Integirty Protection/Ciphering are configured per UE and applicable
 * to all pdcp entities in that UE. Both Integrity protection and ciphering are
 * re-configured during re-establishment.  @n
 *
 * @param pst     -  Point to the pst structure
 * @param spId    -  The ID of the service provider SAP in the PDCP layer 
 * @param secCfg  -  Security Configuration Info per UE.
 *
 * @return 
 *       -# ROK
 *       -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjSecCfgReq
(
Pst              *pst,
SpId             spId,
CpjSecCfgReqInfo *secCfg
)
#else
PUBLIC S16 PjUiCpjSecCfgReq(pst,spId,secCfg)
Pst              *pst;
SpId             spId;
CpjSecCfgReqInfo *secCfg;
#endif
{
   S16               ret;           /* Return Value */
   CpjSecCfgCfmInfo  *cfgCfm;       /* Security Cfg Confirm structure */
   KwCpjSapCb        *cpjSap;       /* CPJ SAP Control Block */
#ifdef PJ_SEC_ASYNC
   KwUeCb            *ueCb;         /* UE Control Block */
#endif

   TRC2(PjUiCpjSecCfgReq)
#ifndef ALIGN_64BIT 
   PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,
               "PjUiCpjSecCfgReq(pst, spId(%d), TransId(%ld))\n", spId,
               secCfg->transId));
#else
   PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,
               "PjUiCpjSecCfgReq(pst, spId(%d), TransId(%d))\n", spId,
               secCfg->transId));
#endif

   ret = ROK;
   cfgCfm = NULLP;
   cpjSap = NULLP;

   cpjSap = &(kwCb.cpjSap);

#ifdef PJ_SEC_ASYNC
   ueCb   = NULLP;
#endif

   PJ_ALLOC(cfgCfm,sizeof(CpjSecCfgCfmInfo));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (cfgCfm == NULLP)
   {
      /* Free memory for CfgReqInfo */
      /* kw002.201 Freeing from proper region */  
      PJ_PST_FREE(pst->region, pst->pool, secCfg, sizeof(CpjSecCfgReqInfo));

      PJLOGERROR(ERRCLS_ADD_RES, EKW085, (ErrVal) RFAILED,
            "PjUiCpjSecCfgReq: cfgCfm allocation Failed.");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   if ( pjCfgPrcSecCfg(secCfg,cfgCfm) != ROK )
   {
      PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
           (_kwPBuf, "PjUiCpjSecCfgReq(pst, cfg(ueId(%d),\
           cellId(%d) )\n pjCfgSecCfg Failed: reason (%d)\n",
           secCfg->ueId, secCfg->cellId, cfgCfm->reason));

   }

#ifdef PJ_SEC_ASYNC
   ret = kwDbmFetchUeCb(secCfg->ueId, secCfg->cellId, &ueCb);
   if ( ueCb != NULLP )
   {
      pjUtlSaveCfmInfo(ueCb, PJ_SEC_ASYNC_CFM, TRUE,  ENTPJ, (PTR)cfgCfm, NULLP);
      PJ_FREE(cfgCfm, sizeof(CpjSecCfgCfmInfo));
   }
   else
   {
      PjUiCpjSecCfgCfm(&(cpjSap->pst), cpjSap->suId, cfgCfm);
   }
#else
   PjUiCpjSecCfgCfm(&(cpjSap->pst), cpjSap->suId, cfgCfm);
#endif 

  /* Configuration information needs to be freed from proper region */
   PJ_PST_FREE(pst->region, pst->pool, secCfg, sizeof(CpjSecCfgReqInfo));

   RETVALUE(ret); 
}



/**
 * @details
 * 1. RRC uses this primitive to re-establish the PDCP entity. @n
 * 2. This primitive is initiation of re-establishment procedure. @n
 * 3. This prmitive and corresponding confirmation shall be followed by
 *    PjUiCpjCfgReq procedure for entire re-establishment procedure to complete.
 *
 * @param[in] pst    - Pointer to post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used by
 * PDCP to identify its SAP.
 * @param[in] reEstReq - PDCP re-establishment request structure. 
 *
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjReEstReq 
(
Pst             *pst, 
SpId            spId, 
CpjReEstReqInfo *reEstReq
)
#else 
PUBLIC S16 PjUiCpjReEstReq(pst,spId,reEstReq)
Pst             *pst;
SpId            spId;
CpjReEstReqInfo *reEstReq;
#endif
{
   S16               ret;           /* Return Value */
   CpjReEstCfmInfo   *reEstCfm;     /* Re-Establish Confirm */
   KwCpjSapCb        *cpjSap;       /* CPJ SAP Control Block */

   TRC2(PjUiCpjReEstReq);

   PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,
            "PjUiCpjReEstReq(pst, spId(%d))\n", spId));

   ret = ROK;
   reEstCfm = NULLP;
   cpjSap = &(kwCb.cpjSap);

   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(spId, cpjSap, ret);
   if (ret != ROK)
   {
      /* kw002.201 Freeing from proper region */  
      PJ_PST_FREE(pst->region, pst->pool, reEstReq, sizeof(CpjReEstReqInfo));
      RETVALUE(RFAILED);
   }

   PJ_ALLOC(reEstCfm,sizeof(CpjReEstCfmInfo));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (reEstCfm == NULLP)
   {
      /* Free memory for CfgReqInfo */
      /* kw002.201 Freeing from proper region */  
      PJ_PST_FREE(pst->region, pst->pool, reEstReq, sizeof(CpjReEstReqInfo));

      PJLOGERROR(ERRCLS_ADD_RES, EKW086, (ErrVal) RFAILED,
            "PjUiCpjReEstReq: reEstCfm allocation Failed.");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   if ( pjCfgReEstReq(reEstReq,reEstCfm) != ROK )
   {
      PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
         (_kwPBuf, "PjUiCpjReEstReq(pst, cfg(ueId(%d),\
         cellId(%d) )\n pjCfgSecCfg Failed: reason (%u)\n",
         reEstReq->pdcpId.ueId, reEstReq->pdcpId.cellId, reEstCfm->status));

      PjUiCpjReEstCfm(&(cpjSap->pst), cpjSap->suId, reEstCfm); 
   }
  
   /* kw002.201 Freeing from proper region */  
   PJ_PST_FREE(pst->region, pst->pool, reEstReq, sizeof(CpjReEstReqInfo));


   RETVALUE(ROK); 
  
}


/**
 * @details
 * 1. RRC uses this primitive to request count value for all DRBs in a UE. @n
 * 2. PDCP sends the UL and DL count values in the confirmation of this request. 
 *
 * @param[in] pst    - Pointer to post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used
 * by PDCP to identify its SAP.
 * @param[in] countReq - PDCP Count Request Information. 
 *
 * @return ROK
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjCountReq 
(
Pst             *pst, 
SpId            spId, 
CpjCountReqInfo *countReq
)
#else
PUBLIC S16 PjUiCpjCountReq(pst,spId,countReq)
Pst             *pst;
SpId            spId;
CpjCountReqInfo *countReq;
#endif
{

   /* kw006.201, ccpu00120574, fixed the warning */
   CpjCountCfmInfo   *countCfm;     /* Count Confirm structure */
   KwCpjSapCb        *cpjSap;       /* CPJ SAP Control Block */

   TRC2(PjUiCpjCountReq);

   PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,
            "PjUiCpjCountReq(pst, spId(%d))\n", spId));

   /* kw006.201, ccpu00120574, fixed the warning */
   countCfm = NULLP;
   cpjSap = &(kwCb.cpjSap);

   PJ_ALLOC(countCfm,sizeof(CpjCountCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (countCfm == NULLP)
   {
      /* Free memory for CfgReqInfo */
      /* kw002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, countReq, sizeof(CpjCountReqInfo));

      PJLOGERROR(ERRCLS_ADD_RES, EKW087, (ErrVal) RFAILED,
            "PjUiCpjCountReq: countCfm allocation Failed.");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
   if ( pjCfgPrcCount(countReq, countCfm) != ROK )
   {
      PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
        (_kwPBuf, "PjUiCpjCountReq(pst, cfg(ueId(%d),\
          cellId(%d) )\n pjCfgSecCfg Failed: reason (%d)\n",
         countReq->ueId, countReq->cellId, countCfm->reason));

   }

   /* Freeing from proper region */
   PJ_PST_FREE(pst->region, pst->pool, countReq, sizeof(CpjCountReqInfo));

   PjUiCpjCountCfm(&(cpjSap->pst), cpjSap->suId,countCfm);

   RETVALUE(ROK);

}

/**
 * 
 * @details
 * 1. RRC uses this primitive to request the status of the SDUs for DRBs in an UE. @n
 * 2. Its used as a reestablishment request during handover.  @n
 * 3. This primitive is used to forward the handover control Information to the target
 * eNodeB.
 *
 * @param[in] pst    - Pointer to post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used
 * by PDCP to identify its SAP.
 * @param[in] sduStaReq - PDCP SDU Status Request Information. 
 *
 * @return ROK
 */
#ifdef ANSI
PUBLIC S16 PjUiCpjSduStaReq 
(
Pst              *pst, 
SpId             spId, 
CpjSduStaReqInfo *staReq
)
#else
PUBLIC S16 PjUiCpjSduStaReq(pst,spId,staReq)
Pst              *pst;
SpId             spId;
CpjSduStaReqInfo *staReq;
#endif
{

   S16               ret;           /* Return Value */
   KwCpjSapCb        *cpjSap;       /* CPJ SAP Control Block */

   TRC2(PjUiCpjSduStaReq);

   PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,
            "PjUiCpjSduStaReq(pst, spId(%d))\n", spId));

   ret = ROK;
   cpjSap = NULLP;

   cpjSap = &(kwCb.cpjSap);

   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(spId, cpjSap, ret);
   if (ret != ROK)
   {
      /* kw002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, staReq, sizeof(CpjSduStaReqInfo));
      RETVALUE(RFAILED);
   }

   if ( pjCfgPrcSduSta(staReq) != ROK )
   {

      PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
         (_kwPBuf, "PjUiCpjSduStaReq(pst, cfg(ueId(%d),\
         cellId(%d) )\n pjCfgSecCfg Failed\n",
         staReq->ueId, staReq->cellId));
      ret = RFAILED;

   }
   /* kw002.201 Freeing from proper region */
   PJ_PST_FREE(pst->region, pst->pool, staReq, sizeof(CpjSduStaReqInfo));

   RETVALUE(ret);
}
#endif

/*****************************************************************************
 *                          PJU INTERFACE
 ****************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for binding the PDCP upper layer service user with 
 *        the PDCP layer.
 *
 * @b Description:
 *
 *        1. This function is used by PDCP user to request for binding to 
 *        PDCP. @n
 *        2. This function is called by the PJU interface to bind 
 *        PDCP's SAP (identified by spId) with the service user's
 *        SAP (identified by suId). 
 *
 *  @param[in] pst   Post structure  
 *  @param[in] suId  Service user SAP ID 
 *  @param[in] spId  Service provider ID
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 PjUiPjuBndReq 
(
Pst       *pst,    
SuId      suId,   
SpId      spId     
)
#else
PUBLIC S16 PjUiPjuBndReq (pst, suId, spId)
Pst       *pst;    
SuId      suId;    
SpId      spId;    
#endif
{
   S16            ret;           /* Return Value */
   KwPjuSapCb     *pjuSap;       /* SAP Config Block */
   
   TRC3(PjUiPjuBndReq)

   PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
            "PjUiPjuBndReq(pst, spId(%d), suId(%d))\n", spId, suId));

   ret = ROK;
   pjuSap = NULLP;

   /* Get Sap control block */
   pjuSap = (&kwCb.pjuSap[spId]);

   /* Validation of input parameters */
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(!((spId < (S16) kwCb.pjGenCfg.maxPjuSaps) && (spId >=0))) 
   {
      if(pjuSap != NULLP) 
      {
         KWLOGERROR(ERRCLS_INT_PAR, EKW088, (ErrVal) spId,
               "PjUiPjuBndReq: PJU SAP State is Invalid during Bind Req");

         KW_SEND_SAPID_ALARM(spId, LPJ_EVENT_PJU_BND_REQ, LCM_CAUSE_INV_SAP);

         RETVALUE(RFAILED); 
      }
   }
#endif

   /* Verify PJU SAP State */
   switch(pjuSap->state)
   {
      /* SAP is configured but not bound */
      case PJ_SAP_CFG:
      case PJ_SAP_UBND:
      {
         /* copy bind configuration parameters in SSAP sap */
         pjuSap->suId          = suId;
         pjuSap->pst.dstProcId = pst->srcProcId;
         pjuSap->pst.dstEnt    = pst->srcEnt;
         pjuSap->pst.dstInst   = pst->srcInst;

         /* Update the State */
         pjuSap->state = PJ_SAP_BND;

         /* DEBUG_PRINT */
         PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_DETAIL), (_kwPBuf,
                  "PjUiPjuBndReq: state (%d)\n", pjuSap->state));
         break;
      }

      /* SAP is already bound */
      case PJ_SAP_BND:
      {
         /* 
          * Sap is already bound check source, destination Entity and 
          * Proc Id
          */
         if (pjuSap->pst.dstProcId != pst->srcProcId 
               || pjuSap->pst.dstEnt != pst->srcEnt
               || pjuSap->pst.dstInst != pst->srcInst
               || pjuSap->suId != suId)
         {
            PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                     "PjUiPjuBndReq(pst, suId(%d)<>suId(%d),\n\
               pst->srcProcId(%d)<>pjuSap->pst.dstProcId(%d),\n\
                     pst->srcEnt(%d)<>pjuSap->pst.dstEnt(%d),\n\
                     pst->srcInst(%d)<>pjuSap->pst.dstInst(%d))\n",\
                  suId, pjuSap->suId, pst->srcProcId, pjuSap->pst.dstProcId,\
                  pst->srcEnt, pjuSap->pst.dstEnt, pst->srcInst,\
                  pjuSap->pst.dstInst));

            PJ_SEND_SAPID_ALARM(spId, LPJ_EVENT_PJU_BND_REQ, LCM_CAUSE_INV_PAR_VAL);
            ret = RFAILED;
         }
         break;
      }

     default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         /* LOG ERROR */
         PJLOGERROR(ERRCLS_INT_PAR, EKW089, (ErrVal) spId,
               "PjUiPjuBndReq: PJU SAP State is Invalid during Bind Req");

         PJ_SEND_SAPID_ALARM(spId, LPJ_EVENT_PJU_BND_REQ, LCM_CAUSE_INV_STATE);
#endif /* ERRCLASS & ERRCLS_INT_PAR */

         PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                  "PjUiPjuBndReq: PJU SAP State is Invalid during Bind Req"));

         ret = RFAILED;
         break;
      }
   }

   if (ret == ROK)
   {
      PjUiPjuBndCfm(&(pjuSap->pst), pjuSap->suId, CM_BND_OK);
   }
   else
   {
      PjUiPjuBndCfm(&(pjuSap->pst), pjuSap->suId, CM_BND_NOK);
   }

   RETVALUE(ROK);

} /* PjUiPjuBndReq */


/**
 *
 * @brief 
 *
 *        Handler for unbinding the PDCP upper layer service user with 
 *        the PDCP layer.
 *
 * @b Description:
 *
 *        1. This function is used by PDCP user to request for unbinding  
 *        with PDCP. @n
 *        2. This function is called by the PJU interface to 
 *        unbind with PDCP. 
 *
 *  @param[in] pst     Post structure  
 *  @param[in] suId    Service user SAP ID 
 *  @param[in] reason  Reason for Unbinding 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjUiPjuUbndReq
(
Pst      *pst,   
SpId     spId,    
Reason   reason     
)
#else
PUBLIC S16 PjUiPjuUbndReq(pst, spId, reason)
Pst      *pst;       
SpId     spId;     
Reason   reason;      
#endif
{
   KwPjuSapCb  *pjuSap;            /* PJU SAP control block */
   S16         ret; 

   TRC3(PjUiPjuUbndReq)

   /* DEBUG_PRINT */
   PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
            "PjUiPjuUbndReq(pst, spId(%d), reason(%d))\n", spId, reason));

   pjuSap = NULLP;

   UNUSED(pst);
   UNUSED(reason);

   /* Get Sap control block */
   pjuSap = &(kwCb.pjuSap[spId]);
   ret    = ROK;

#if (ERRCLASS & ERRCLS_INT_PAR)
   PJ_GET_AND_VALIDATE_PJUSAP(pjuSap, EKW090, "PjUiPjuUbndReq", ret);
   if (ret != ROK )
   {
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   pjuSap->state = PJ_SAP_CFG;

   RETVALUE(ROK);
} /* PjUiPjuUbndReq */

#if 0
/**
 * @details
 * This primitive is used by the PDCP service user to request PDCP
 * for data transfer.
 *
 * @param[in] pst    - Pointer to Pst structure.
 * @param[in] spId  - Service provider ID. This is the reference number
 * used by PDCP service user (Relay) to identify its SAP. This reference number
 * will be provided by PDCP in any further primitives sent by PDCP
 * to the service user.
 * @param[in] pdcpId - Pointer to CmLtePdcpId structure, PDCP id is used to 
 * uniquely identify an PDCP entity. 
 * @param[in] sduId  - SDU ID. 
 * @param[in] mBuf   - Pointer to data buffer.
 *
 * @return ROK
 */
#ifdef ANSI
PUBLIC S16 PjUiPjuDatReq
(
Pst            *pst,            
SpId           spId,               
CmLtePdcpId    *pdcpId,         
PjuSduId       sduId,           
Buffer         *mBuf          
)
#else
PUBLIC S16 PjUiPjuDatReq(pst, spId, pdcpId, sduId, mBuf)
Pst             *pst;       
SpId            spId;           
CmLtePdcpId     *pdcpId;     
PjuSduId         sduId;   
Buffer          *mBuf;          
#endif
{
   S16         ret;           /* Return Value */
   PjRbCb      *pjRbCb;       /* RB Control Block */
   KwUeCb      *ueCb;         /* UE Control Block */
   PjSn        sn;            /* Sequence Number for the PDU */
   U32         txHfn;         /* TX_HFN for the PDU */
   MsgLen      len;           /* Size of the SDU */
#ifndef NO_ERRCLS
   KwPjuSapCb  *pjuSap;       /* SAP Config Block */
#endif
#ifdef MSPD_MLOG_NEW
   U32    tick = *(volatile U32 *) 0xFE050004;
#endif

   TRC3(PjUiPjuDatReq)

   ret = ROK;
   pjRbCb = NULLP;
   sn  = 0;
   txHfn = 0;
   len = 0;
   ueCb  = NULLP;
#ifndef NO_ERRCLS
   pjuSap = NULLP;
#endif
 
   /* DEBUG_PRINT */
   PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
            "PjUiPjuDatReq(pst, spId(%d))\n", spId));

#ifndef NO_ERRCLS
   pjuSap = &(kwCb.pjuSap[spId]);
   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(spId, pjuSap, ret);
   if (ret != ROK)
   {
      PJ_PST_FREE(pst->region, pst->pool, pdcpId, sizeof(CmLtePdcpId));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
#endif
   /* Fetch the RbCb */
   if (kwDbmFetchUeCb(pdcpId->ueId, pdcpId->cellId, &ueCb) != ROK)
   {
#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
         "PjUiPjuDatReq: kwDbmFetchUeCb Failed.\n"));
#endif
      PJ_PST_FREE(pst->region, pst->pool, pdcpId, sizeof(CmLtePdcpId));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   /* Fetch the RbCb */
   kwDbmFetchPjRbCb(ueCb, pdcpId->rbId, pdcpId->rbType, &pjRbCb); 
   if (pjRbCb == NULLP)
   {     
      PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
               "PjUiPjuDatReq: PDCP ID not found ueId(%d), cellId(%d), rbId(%d)",
            pdcpId->ueId, pdcpId->cellId, pdcpId->rbId));
      PJ_PST_FREE(pst->region, pst->pool, pdcpId, sizeof(CmLtePdcpId));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
#ifdef PTPJLIB 
   /* kw003.201 Control for packet administration */
   if (pjLibPktAdmCtrl(pjRbCb, mBuf) != ROK)
   {
      PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
               "PjUiPjuDatReq: PDCP pkt dropped for ueId(%d), cellId(%d), "
               "rbId(%d)", pdcpId->ueId, pdcpId->cellId, pdcpId->rbId));
      PJ_PST_FREE(pst->region, pst->pool, pdcpId, sizeof(CmLtePdcpId));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
#endif

#ifndef CCPU_OPT
   SFndLenMsg(mBuf, &len);
   if ( len > PJ_MAX_SDU_SIZE )
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      PJLOGERROR(ERRCLS_INT_PAR, EKW091, (ErrVal) spId,
         "PjUiPjuDatReq: pjDlmProcessSdus failed, PDU size > MAX_LEN");
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      if ( pjRbCb->dlCb.cfmReqd)
      {
         PjuDatCfmInfo *datCfm;
         CmLtePdcpId *tPdcpId;
         KwPjuSapCb  *tPjuSap;
         if (pjRbCb->rbType == PJ_SRB)
            tPjuSap   = &(kwCb.pjuSap[PJ_SRB_SAP]);
         else
            tPjuSap   = &(kwCb.pjuSap[PJ_DRB_SAP]);

         PJ_ALLOC( datCfm, sizeof(PjuDatCfmInfo));
         PJ_ALLOC( tPdcpId, sizeof(CmLtePdcpId));
         if ( datCfm != NULLP && tPdcpId != NULLP )
         {
            datCfm->cfmSta[datCfm->numSdus].sduId = sduId;
            datCfm->cfmSta[datCfm->numSdus].status = PJU_MAX_SDU_SIZ_EXCEED;
            datCfm->numSdus++;
            tPdcpId->rbId   = pjRbCb->rbId;
            tPdcpId->rbType = pjRbCb->rbType;
            tPdcpId->ueId   = pjRbCb->ueCb->key.ueId;
            tPdcpId->cellId = pjRbCb->ueCb->key.cellId;
            PjUiPjuDatCfm(&(tPjuSap->pst), tPjuSap->suId, tPdcpId, datCfm);
         }
      }

      kwCb.pjGenSts.numCmpFails++;

      PJ_FREE_BUF(mBuf);
      PJ_PST_FREE(pst->region, pst->pool, pdcpId, sizeof(CmLtePdcpId));
      RETVALUE(RFAILED);
   }   
#endif
   PJ_DLM_UPD_VAR(pjRbCb, sn, txHfn);
   ret = pjDlmProcessSdus(pjRbCb, mBuf, sduId, sn, txHfn);

   if ( ret != ROK )
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      PJLOGERROR(ERRCLS_INT_PAR, EKW092, (ErrVal) spId,
         "PjUiPjuDatReq: pjDlmProcessSdus failed");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   }
   /* kw005.201 ccpu00117318, updated statistics */
   PJ_STS_INC_PJU_CNT(pjRbCb->rbType, rxSdus);
   PJ_PST_FREE(pst->region, pst->pool, pdcpId, sizeof(CmLtePdcpId));
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_PDCP_DL_PKT, RESOURCE_LARM, tick, *(volatile U32 *) 0xFE050004);
#endif

   RETVALUE(ret);
} /* PjUiPjuDatReq */


/**
 * 
 * @details
 * This primitive is used by PDCP user to forward the SDUs along with its SN
 * as part of handover (SDUs forwarded from PDCP on source eNB via X2-U 
 * Interface to the PDCP on target eNB)
 *
 * @param[in] pst    - Pointer to Pst structure.
 * @param[in] spId   - Service provider ID. This is the reference number
 * used by PDCP service user (Relay) to identify its SAP. This reference number
 * will be provided by PDCP in any further primitives sent by PDCP
 * to the service user.
 * @param[in] pdcpId - Pointer to CmLtePdcpId structure, PDCP id is used to 
 * uniquely identify a PDCP entity. 
 * @param[in] datFwdReq - Pointer to data forward information structure.
 *
 *  @return ROK.
 */
#ifdef ANSI
PUBLIC S16 PjUiPjuDatFwdReq 
(
Pst              *pst, 
SpId             spId, 
CmLtePdcpId      *pdcpId, 
PjuDatFwdReqInfo *datFwdReq
)
#else
PUBLIC S16 PjUiPjuDatFwdReq(pst,spId,pdcpId,datFwdReq)
Pst              *pst;
SpId             spId;
CmLtePdcpId      *pdcpId;
PjuDatFwdReqInfo *datFwdReq;
#endif
{

   S16         ret;           /* Return Value */
   PjRbCb      *pjRbCb;       /* RB Control Block */
   KwUeCb      *ueCb;         /* UE Control Block */
   KwPjuSapCb  *pjuSap;       /* SAP Config Block */

   TRC3(PjUiPjuDatFwdReq)

   ret = ROK;
   pjRbCb = NULLP;
   ueCb   = NULLP;
   pjuSap = NULLP;
 
   PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
            "PjUiPjuDatFwdReq(pst, spId(%d))\n", spId));

   pjuSap = &(kwCb.pjuSap[spId]);
   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(spId, pjuSap, ret);
   if (ret != ROK)
   {
      /* Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, pdcpId, sizeof(CmLtePdcpId));
      PJ_PST_FREE(pst->region, pst->pool, datFwdReq, sizeof(PjuDatFwdReqInfo));
      RETVALUE(RFAILED);
   }

   if (kwDbmFetchUeCb(pdcpId->ueId, pdcpId->cellId, &ueCb) != ROK)
   {
#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
         "PjUiPjuDatReq: kwDbmFetchUeCb Failed.\n"));
#endif
      /* Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, pdcpId, sizeof(CmLtePdcpId));
      PJ_PST_FREE(pst->region, pst->pool, datFwdReq, sizeof(PjuDatFwdReqInfo));
      RETVALUE(RFAILED);
   }
   /* Fetch the RbCb */
   kwDbmFetchPjRbCb(ueCb, pdcpId->rbId, pdcpId->rbType, &pjRbCb); 
   if (pjRbCb == NULLP)
   {
      PJDBGP((KW_PJ_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
               "PjUiPjuDatFwdReq: PDCP ID not found ueId(%d), cellId(%d), rbId(%d)",
            pdcpId->ueId, pdcpId->cellId, pdcpId->rbId));
      /* Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, pdcpId, sizeof(CmLtePdcpId));
      PJ_PST_FREE(pst->region, pst->pool, datFwdReq, sizeof(PjuDatFwdReqInfo));

      RETVALUE(RFAILED);
   }
     
   if ( datFwdReq->dir == PJ_DIR_DL ) 
   {
      ret = pjDlmHndlDatFwdReq(pjRbCb, datFwdReq);
   }
   else if ( datFwdReq->dir == PJ_DIR_UL )
   {
      ret = pjUlmHdlDatFwdReq(pjRbCb, datFwdReq);
   }
   else
   {
      ret = RFAILED;
   }
  
   /* Freeing from proper region */
   PJ_PST_FREE(pst->region, pst->pool, pdcpId, sizeof(CmLtePdcpId));
   PJ_PST_FREE(pst->region, pst->pool, datFwdReq->datFwdInfo, 
           (datFwdReq->numSdus * sizeof(PjuDatFwdReqInfo)));
   PJ_PST_FREE(pst->region, pst->pool, datFwdReq, sizeof(PjuDatFwdReqInfo));

   if ( ret != ROK )
   {
      /* Log an error */
#if (ERRCLASS & ERRCLS_INT_PAR)
      PJLOGERROR(ERRCLS_INT_PAR, EKW093, (ErrVal) spId,
       "PjUiPjuDatFwdReq: pjDlm/pjUlmHndlDatFwdReq failed");
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   }

   RETVALUE(ret);

}
/***************SEC_CHANGE_START*****************/
#ifdef CTF_AS_SEC_ENB
#ifdef ANSI
PUBLIC S16 YsUiCtfDatReq
(
Pst*                 pst,
SpId                 spId,
CtfDatReqSdus*       CtfDatReqInfo
)
#else
PUBLIC S16 YsUiCtfDatReq(pst, spId, DatReqInfo)
Pst*                 pst;
SpId                 spId;
CtfDatReqSdus*       CtfDatReqInfo;
#endif
{
   S16       ret;
   S32 idx = 0;
   Mem sMem;
   YsMsSecInfo  *pSecInfo;
   CtfDatRspSdus *CtfDatRspSdu;
   YsMsSpaccIntAlgo intAlgoType;
   YsMsSpaccCiphAlgo ciphAlgoType;
   Pst tmpPst;

   TRC2(YsUiCtfDatReq)

   ret = ROK;

   pSecInfo = MxHeapAlloc(NcnbHeap, sizeof(YsMsSecInfo));
   _ASSERT_PTR(pSecInfo);

#ifndef SPACC_DBG
   MSPD_DBG("YsUiCtfDatReq: CtfDatReqInfo->sdu.SecKey\n");
   for(idx = 0; idx < CTF_SEC_KEY_LEN; idx++)
   {
       MSPD_DBG("%x ",CtfDatReqInfo->sdu.SecKey[idx]);
   }
   MSPD_DBG("\n"); 

   MSPD_DBG("YsUiCtfDatReq: CtfDatReqInfo->sdu.strS\n");
   for(idx = 0; idx < CTF_STRING_S_LEN; idx++)
   {
       MSPD_DBG("%x ",CtfDatReqInfo->sdu.strS[idx]);
   }
   MSPD_DBG("\n"); 
#endif

   
   cmMemcpy(pSecInfo->hash_key,CtfDatReqInfo->sdu.SecKey,CTF_SEC_KEY_LEN);
   cmMemcpy(pSecInfo->strS,CtfDatReqInfo->sdu.strS,CTF_STRING_S_LEN);
   
   ret = ysMsKDF(pSecInfo);

   if(ret!=ROK)
   {
     RETVALUE(RFAILED);
   }/*End of if(ret!=ROK)*/

   if((CtfDatReqInfo->sdu.SecKeyTyp == YS_MS_SPACC_CP_CIPHER_KEY) ||
      (CtfDatReqInfo->sdu.SecKeyTyp == YS_MS_SPACC_UP_CIPHER_KEY))
   {
       switch(CtfDatReqInfo->sdu.algoType)
       {
           case YS_MS_SPACC_EEA1:
               ciphAlgoType = YS_MS_SPACC_EEA1;
               break;
           case YS_MS_SPACC_EEA2:
               ciphAlgoType = YS_MS_SPACC_EEA2;
               break;
           default:
               ciphAlgoType = YS_MS_SPACC_EEA0;
               break;
       }
   }
   else if (CtfDatReqInfo->sdu.SecKeyTyp == YS_MS_SPACC_INTEG_KEY)
   {
       switch(CtfDatReqInfo->sdu.algoType)
       {
           case YS_MS_SPACC_EIA1:
               intAlgoType = YS_MS_SPACC_EIA1;
               break;
           case YS_MS_SPACC_EIA2:
               intAlgoType = YS_MS_SPACC_EIA2;
               break;
           default:
               intAlgoType = YS_MS_SPACC_EIA0;
               break;
       }
           
   }

   
   switch(CtfDatReqInfo->sdu.SecKeyTyp)
   {
       case YS_MS_SPACC_CP_CIPHER_KEY:
	   ret = ysMsSpaccCreateUeCtxt(CtfDatReqInfo->hdr.ueId);
 
	   MSPD_DBG("YsUiCtfDatReq: Storing CP Cipher Key\n"); 
           ret = ysMsSpaccUpdUeCtxt(CtfDatReqInfo->hdr.ueId,
                      pSecInfo->pOutBuf,YS_MS_SPACC_CP_CIPHER_KEY,ciphAlgoType,0);
           break;
       case YS_MS_SPACC_UP_CIPHER_KEY:
	   MSPD_DBG("YsUiCtfDatReq: Storing UP Cipher Key\n"); 
           ret = ysMsSpaccUpdUeCtxt(CtfDatReqInfo->hdr.ueId,
                      pSecInfo->pOutBuf,YS_MS_SPACC_UP_CIPHER_KEY,ciphAlgoType,0);
           break;
       case YS_MS_SPACC_INTEG_KEY:
	   MSPD_DBG("YsUiCtfDatReq: Storing Integrity Key\n"); 
           ret = ysMsSpaccUpdUeCtxt(CtfDatReqInfo->hdr.ueId,
                     pSecInfo->pOutBuf,YS_MS_SPACC_INTEG_KEY,0,intAlgoType);
           break;
   }

   sMem.region = 0;
   sMem.pool = 0;

   cmAllocEvnt(sizeof(CtfDatRspSdus), CTF_MEM_SDU_SIZE, &sMem,(Ptr*)&CtfDatRspSdu);


   cmMemcpy(CtfDatRspSdu->sdu.DerivedKey,pSecInfo->pOutBuf,CTF_DER_KEY_LEN);
   CtfDatRspSdu->sdu.SecKeyTyp = CtfDatReqInfo->sdu.SecKeyTyp;
   CtfDatRspSdu->Hdr.cellId = CtfDatReqInfo->hdr.cellId;
   CtfDatRspSdu->Hdr.ueId= CtfDatReqInfo->hdr.ueId;

   if (pSecInfo->pOutBuf != NULLP)                                          
   {
      (Void) SPutSBuf(DFLT_REGION, DFLT_POOL,          
            (Data *)pSecInfo->pOutBuf , (Size) YS_MS_KDF_OP_LEN);                            
      pSecInfo->pOutBuf = NULLP;                                                  
   }           
   HeapFreeByPtr((void *)pSecInfo);
   MSPD_DBG("REACHED 1\n");

   tmpPst.dstEnt = 0xdf; /*ENT_UEH:This is PWAVE RRM Entity Id*/
   tmpPst.dstInst = pst->srcInst; 
   tmpPst.dstProcId = pst->srcProcId; 
   tmpPst.srcProcId = 1;
   tmpPst.srcInst = 0;
#if 0
   tmpPst.srcEnt = ENTTF; /*Keeping it as ENTTF as PWAV RRM is expecting this msg from CL currently*/
#else
   tmpPst.srcEnt = ENTPJ; /*Keeping it as ENTTF as PWAV RRM is expecting this msg from CL currently*/
#endif
   tmpPst.selector = 0;
   tmpPst.route = RTESPEC;
   tmpPst.region = DFLT_REGION; 
   tmpPst.pool = DFLT_POOL; 
   MSPD_DBG("REACHED 2\n");
   
#if 0
   ret =  ysUiCtfDatRsp(&tmpPst, 0 ,CtfDatRspSdu);
#else
   ret =  ysUiCtfDatRsp(&kwCb.pjuSap[PJ_SRB_SAP].pst, 0 ,CtfDatRspSdu);
#endif
   
   MSPD_DBG("Dest Inst:%d, Dest Proc:%d\n",tmpPst.dstInst,tmpPst.dstProcId);
   MSPD_DBG("REACHED 3\n");
   if (ret!=ROK)
   { 
   MSPD_DBG("REACHED 4\n");
     RETVALUE(RFAILED);
   }/*End of if(ret!=ROK)*/
   RETVALUE(ROK);

}/*End of function YsUiCtfDatReq*/

#endif
#endif 
/***************SEC_CHANGE_END*****************/


#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**
         End of file:     gp_pj_uim.c@@/main/1 - Fri Nov 13 14:13:49 2009
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
/main/1      ---       vp   1. LTERLC Release 2.1
/main/2      kw002.201 vp   1. Patch for optimization,fixes for multi-region
                               and retransmission.
/main/3      kw003.201 vp   1. Fixes for re-segmentation, segement offset
                               value and optimization 
/main/4      kw004.201 av   1. Fix for  build issue in LTE RLC kw_pj_uim.c
/main/5      kw005.201 ap   1. Updated missing statistics in the code. 
/main/6      kw006.201 ap   1. ccpu00120574, fixed the warning.
*********************************************************************91*/
