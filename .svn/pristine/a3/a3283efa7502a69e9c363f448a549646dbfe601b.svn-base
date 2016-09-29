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
  
        Name:    LTE-RLC Layer - Upper Interface Functions
    
        Type:    C file
  
     Desc:     Source code for RLC Upper Interface Module
               This file contains following functions
      
        --KwUiCkwBndReq
        --KwUiCkwUbndReq
        --KwUiCkwCfgReq

        --KwUiKwuBndReq
        --KwUiKwuUbndReq
        --KwUiKwuDatReq
        --KwUiKwuDiscSduReq 

     File:     kw_uim.c

     Sid:      gp_uim.c@@/main/2 - Fri Nov 13 14:14:02 2009

     Prg:      gk 

**********************************************************************/
/** @file gp_uim.c
@brief RLC Upper Interface Module 
*/


/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000241     LTE-MAC
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
#ifdef KW_PDCP
#include "cpj.h"           /* CPJ defines */
#include "pju.h"           /* PJU defines */
#include "lpj.h"           /* LPJ defines */
#endif
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
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */
#ifdef KW_PDCP
#include "cpj.x"           /* CPJ defines */
#include "pju.x"           /* PJU defines */
#include "lpj.x"           /* LPJ defines */
#endif
#include "kw_err.h"
#include "kw.x"

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */


/* Global variable */
KwCb  kwCb;

/*****************************************************************************
 *                          CKW INTERFACE
 ****************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for binding the RLC upper layer service user with 
 *        the RLC layer.
 *
 * @b Description:
 *
 *        This function is used by RLC user to request for binding to 
 *        RLC. This function is called by the CKW interfacei to bind 
 *        RLC's SAP (identified by spId) with the service user's
 *        SAP (identified by suId). 
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
PUBLIC S16 KwUiCkwBndReq 
(
Pst  *pst,   /* Post structure */ 
SuId suId,   /* service user SAP ID */ 
SpId spId   /* service provider identifier */
)
#else
PUBLIC S16 KwUiCkwBndReq (pst, suId, spId)
Pst *pst;    /* Post structure */
SuId suId;    /* service user SAP ID */
SpId spId;    /* service provider identifier */
#endif
{
   KwCkwSapCb      *ckwSap;            /* pointer to session SAP */
   S16             ret;                /* return value */

   TRC3(KwUiCkwBndReq);

   ret = ROK;

#ifdef DEBUGP
   /* DEBUG_PRINT */
   KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
            "KwUiCkwBndReq(pst, spId(%d), suId(%d))\n", spId, suId));
#endif

   ckwSap = &(kwCb.ckwSap);

   /* Verify CKW SAP State */
   switch(ckwSap->state)
   {
      /* SAP is configured but not bound */
      case KW_SAP_CFG:
      case KW_SAP_UBND:
      {
         /* copy bind configuration parameters in SSAP sap */
         ckwSap->suId = suId;
         ckwSap->pst.dstProcId = pst->srcProcId;
         ckwSap->pst.dstEnt = pst->srcEnt;
         ckwSap->pst.dstInst = pst->srcInst;

         /* Update the State */
         ckwSap->state = KW_SAP_BND;

#ifdef DEBUGP
         /* DEBUG_PRINT */
         KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_DETAIL), (_kwPBuf,
                  "KwUiCkwBndReq: state (%d)\n", ckwSap->state));
#endif
         break;
      }

      /* SAP is already bound */
      case KW_SAP_BND:
      {
         /* 
          * Sap is already bound check source, destination Entity and 
          * Proc Id
          */
         if (ckwSap->pst.dstProcId != pst->srcProcId 
               || ckwSap->pst.dstEnt != pst->srcEnt
               || ckwSap->pst.dstInst != pst->srcInst
               || ckwSap->suId != suId)
         {
            KW_SEND_SAPID_ALARM(spId, LKW_EVENT_CKW_BND_REQ, LCM_CAUSE_INV_PAR_VAL);

#ifdef DEBUGP
            KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                     "KwUiCkwBndReq: SAP already bound -- state(%d)\n", ckwSap->state));
#endif
            ret = RFAILED;
         }
         break;
      }

     default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         /* LOG ERROR */
         KWLOGERROR(ERRCLS_INT_PAR, EKW207, (ErrVal) spId,
               "KwUiCkwBndReq: CKW SAP State is Invalid during Bind Req");

         KW_SEND_SAPID_ALARM(spId, LKW_EVENT_CKW_BND_REQ, LCM_CAUSE_INV_STATE);
#endif /* ERRCLASS & ERRCLS_INT_PAR */

         /* DEBUG_PRINT */
#ifdef DEBUGP
         KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                  "KwUiCkwBndReq: CKW SAP State is Invalid during Bind Req"));
#endif

         ret = RFAILED;
         break;
      }
   }

   if (ret == ROK)
   {
      KwUiCkwBndCfm(&(ckwSap->pst), ckwSap->suId, CM_BND_OK);
   }
   else
   {
      KwUiCkwBndCfm(&(ckwSap->pst), ckwSap->suId, CM_BND_NOK);
   }

   RETVALUE(ROK);
} /* KwUiCkwBndReq */


/**
 *
 * @brief 
 *
 *        Handler for unbinding the RLC upper layer service user CKW with 
 *        the RLC layer.
 *
 * @b Description:
 *
 *        This function is used by RLC user to request for unbinding  
 *        with RLC.This function is called by the CKW interface to 
 *        unbind with RLC. 
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
PUBLIC S16 KwUiCkwUbndReq
(
Pst      *pst,         /* Post structure */
SpId     spId,       /* service provider identifier */
Reason   reason      /* Reason for Unbinding */
)
#else
PUBLIC S16 KwUiCkwUbndReq(pst, spId, reason)
Pst      *pst;       /* Post structure */
SpId     spId;       /* service provider identifier */
Reason   reason;       /* Reason for Unbinding */
#endif
{
   KwCkwSapCb  *ckwSap;            /* CKW SAP control block */

   TRC3(KwUiCkwUbndReq)

#ifdef DEBUGP
   /* DEBUG_PRINT */
   KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
            "KwUiCkwUbndReq(pst, spId(%d), reason(%d))\n", spId, reason));
#endif

   UNUSED(pst);
   UNUSED(reason);

   /* disable upper sap (CKW) */
   ckwSap = &(kwCb.ckwSap);

#if (ERRCLASS & ERRCLS_INT_PAR)
   KW_GET_AND_VALIDATE_CKWSAP(ckwSap, EKW208, "KwUiCkwUbndReq");
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   ckwSap->state = KW_SAP_CFG;

   RETVALUE(ROK);
} /* KwUiCkwUbndReq */
#if 0

/**
 *
 * @brief 
 *
 *        Handler for configuring RLC entities.
 *
 * @b Description:
 *
 *        This function is used by RRC to configure(add/delete/modify)
 *        one or more RLC entities. 
 *
 *        - CKW_CFG_ADD          => kwCfgAddRb
 *        - CKW_CFG_MODIFY       => kwCfgReCfgRb
 *        - CKW_CFG_DELETE       => kwCfgDelRb
 *        - CKW_CFG_REESTABLISH  => kwCfgReEstRb
 *        - CKW_CFG_DELETE_UE    => kwCfgDelUe
 *
 *  @param[in] pst   -  Post structure  
 *  @param[in] spId  -  Serive Provider ID 
 *  @param[in] cfg   -  Configuration information for one or more RLC entities. 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 KwUiCkwCfgReq
(
Pst         *pst,
SpId        spId,
CkwCfgInfo  *cfg
)
#else
PUBLIC S16 KwUiCkwCfgReq(pst, spId, cfg)
Pst         *pst;
SpId        spId;
CkwCfgInfo  *cfg;
#endif
{
   CkwCfgCfmInfo        *cfgCfm;       /* Configuraiton Confirm */
   U8                   idx;           /* Index */
   KwCkwSapCb           *ckwSap;       /* CKW SAP Control Block */
   S16                  ret;
   Bool                 ueDelWait;     /* Add present */

#ifdef MSPD_MLOG_NEW
   U32                  t1, t2, t_reEst;
#endif
   TRC3(KwUiCkwCfgReq)

   /* DEBUG_PRINT */
   KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,
            "KwUiCkwCfgReq(pst, spId(%d))\n", spId));

   ret       = ROK;
   ueDelWait = FALSE;

   /* Get the ckwSap */
   ckwSap = &(kwCb.ckwSap);

   /* Validate SAP ID under ERRORCLS */
   KW_VALDATE_SAP(spId, ckwSap, ret);
   if (ret != ROK)
   {
      /* kw002.201 Freeing from proper region */
      KW_PST_FREE(pst->region, pst->pool, cfg, sizeof(CkwCfgInfo));
      RETVALUE(RFAILED);
   }

#ifdef MSPD_MLOG_NEW
   t1 = GetTIMETICK();
#endif
   /* Allocate memory and memset to 0 for cfmInfo */
   KW_ALLOC(cfgCfm, sizeof(CkwCfgCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (cfgCfm == NULLP)
   {
      KWLOGERROR(ERRCLS_ADD_RES, EKW209, (ErrVal) RFAILED,
            "KwUiCkwCfgReq: cfgCfm allocation Failed.");
      /* kw002.201 Freeing from proper region */
      KW_PST_FREE(pst->region, pst->pool, cfg, sizeof(CkwCfgInfo));
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

#ifdef PWAV_LARM_LOGS 
         printRlcCfg(cfg);
#endif
   /* For every entity configuration process by cfgType */
   for (idx = 0; idx < cfg->numEnt; idx++)
   {
      CkwEntCfgCfmInfo  *entCfgCfm;
      CkwEntCfgInfo     *entCfg;

      entCfg  = &(cfg->entCfg[idx]);
      entCfgCfm   = &(cfgCfm->entCfgCfm[idx]);

      switch (entCfg->cfgType)
      {
         case CKW_CFG_ADD:
            {
               /* Add a new RB entity configuration */
               if (kwCfgAddRb(cfg->ueId, cfg->cellId,\
                        entCfg, entCfgCfm) != ROK)
               {
#ifdef DEBUGP
                  KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
                        (_kwPBuf, "KwUiCkwCfgReq(pst, cfg(ueId(%d),\
                     cellId(%d), cfgType(%d), rbId(%d))\n kwCfgAddRb Failed: reason (%d)\n",
                         cfg->ueId, cfg->cellId, entCfg->cfgType,
                         cfg->entCfg[idx].rbId, entCfgCfm->status.reason));
#endif
               }
               break;
            }

         case CKW_CFG_MODIFY:
            {
               /* Re-configure the existing RB entity configuration */
               if (kwCfgReCfgRb(cfg->ueId, cfg->cellId,\
                        entCfg, entCfgCfm) != ROK)
               {
#ifdef DEBUGP
                  KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
                        (_kwPBuf, "KwUiCkwCfgReq(pst, cfg(ueId(%d),\
                     cellId(%d), cfgType(%d), rbId(%d))\n kwCfgReCfgRb Failed: reason (%d)\n",
                         cfg->ueId, cfg->cellId, entCfg->cfgType,
                         cfg->entCfg[idx].rbId, entCfgCfm->status.reason));
#endif
               }
               break;
            }

         case CKW_CFG_DELETE:
            {
               /* Delete the existing RB entity configuration */
               if (kwCfgDelRb(cfg->ueId, cfg->cellId,\
                        entCfg, entCfgCfm) != ROK)
               {
#ifdef DEBUGP
                  KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
                        (_kwPBuf, "KwUiCkwCfgReq(pst, cfg(ueId(%d),\
                     cellId(%d), cfgType(%d), rbId(%d))\n kwCfgDelRb Failed: reason (%d)\n",
                         cfg->ueId, cfg->cellId, entCfg->cfgType,
                         cfg->entCfg[idx].rbId, entCfgCfm->status.reason));
#endif
               }
               break;
            }

         case CKW_CFG_REESTABLISH:
            {
#ifdef MSPD_MLOG_NEW
               t_reEst = GetTIMETICK();
#endif
               //MSPD_DBG("<Rahul>Received RE-EST request at RLC\n");
               /* Re-establish the existing RB entity configuration */
               if (kwCfgReEstRb(cfg->ueId, cfg->cellId,\
                        entCfg, entCfgCfm) != ROK)
               {
#ifdef DEBUGP
                  KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
                        (_kwPBuf, "KwUiCkwCfgReq(pst, cfg(ueId(%d),\
                     cellId(%d), cfgType(%d), rbId(%d))\n kwCfgReEstRb Failed: reason (%d)\n",
                         cfg->ueId, cfg->cellId, entCfg->cfgType,
                         cfg->entCfg[idx].rbId, entCfgCfm->status.reason));
#endif
               }
#ifdef MSPD_MLOG_NEW
               MLogTask(PID_RLC_CFG_REQ_REEST, RESOURCE_LARM, t_reEst, GetTIMETICK());
#endif
               break;
            }

         case CKW_CFG_DELETE_UE:
            {
               /* Delete all RB entity configuration under UE */
               if (kwCfgDelUe(cfg->ueId, cfg->cellId,\
                        entCfg, entCfgCfm) != ROK)
               {

#ifdef DEBUGP
                  KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
                        (_kwPBuf, "KwUiCkwCfgReq(pst, cfg(ueId(%d),\
                     cellId(%d), cfgType(%d), rbId(%d))\n kwCfgDelUe Failed: reason (%d)\n",
                         cfg->ueId, cfg->cellId, entCfg->cfgType,
                         cfg->entCfg[idx].rbId, entCfgCfm->status.reason));
#endif
               }
#ifdef KW_PDCP
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
               else
               {
                  /* Wait for any messages in the Demand q to be processed */
                  ueDelWait   =  TRUE;
               }
#endif /* (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)) */
#endif /* KW_PDCP */
               break;
            }
         case CKW_CFG_DELETE_CELL:
            {
               if ( kwCfgDelCell(cfg->cellId,entCfg,entCfgCfm) != ROK )
               {
#ifdef DEBUGP
                  KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
                        (_kwPBuf, "KwUiCkwCfgReq(pst, cfg(ueId(%d),\
                     cellId(%d), cfgType(%d), rbId(%d))\n kwCfgDelCell Failed: reason (%d)\n",
                         cfg->ueId, cfg->cellId, entCfg->cfgType,
                         cfg->entCfg[idx].rbId, entCfgCfm->status.reason));
#endif
               } 
               break;
            }

         default:
            {
               KW_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType,\
                  CKW_CFG_CFM_NOK, CKW_CFG_REAS_INVALID_CFG);

#ifdef DEBUGP
               KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
                     (_kwPBuf, "KwUiCkwCfgReq(pst, cfg(ueId(%d),\
                  cellId(%d), cfgType(%d), rbId(%d)) Failed: reason (%d)\n",
                     cfg->ueId, cfg->cellId, entCfg->cfgType,
                     cfg->entCfg[idx].rbId, CKW_CFG_REAS_INVALID_CFG));
#endif
            }
      }
   }
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_RLC_CFG_REQ, RESOURCE_LARM, t1, GetTIMETICK());
   t2 = GetTIMETICK();
#endif

   /* Assign number of entity configuraitons and suId */
   cfgCfm->transId = cfg->transId;
   cfgCfm->ueId = cfg->ueId;
   cfgCfm->cellId = cfg->cellId;
   cfgCfm->numEnt = cfg->numEnt;


   if(ueDelWait == FALSE)
   {
      /* kw002.201 Freeing from proper region */
      KW_PST_FREE(pst->region, pst->pool, cfg, sizeof(CkwCfgInfo));
      /* Send Configuration confirm primitive */
      KwUiCkwCfgCfm(&(ckwSap->pst), ckwSap->suId, cfgCfm);
   }
#ifdef KW_PDCP
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   else
   {
      KwUeCb        *ueCb;

      /* Send Configuration confirm primitive */
      ret = kwDbmFetchUeCb(cfg->ueId, cfg->cellId, &ueCb);
      if(ueCb != NULLP)
      {
         pjUtlSaveCfmInfo(ueCb, PJ_CFG_UEDEL_ASYNC_CFM, TRUE,  ENTKW, 
                              (PTR)cfgCfm, (PTR)cfg);
      }
      /* kw002.201 Freeing from proper region */
      KW_PST_FREE(pst->region, pst->pool, cfg, sizeof(CkwCfgInfo));
      KW_FREE(cfgCfm, sizeof(CkwCfgCfmInfo));
   }
#endif /* (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)) */
#endif /* KW_PDCP */

#ifdef MSPD_MLOG_NEW
   MLogTask(PID_RLC_CFG_REQ_REM, RESOURCE_LARM, t2, GetTIMETICK());
#endif

   RETVALUE(ROK);
} /* KwUiCkwCfgReq */

/**
 *@details This primitive is used by RRC to change the UeId for the existing UE
           context.

 * @param pst     -  Point to the pst structure
 * @param spId    -  The ID of the service provider SAP in the RLC layer 
 * @param transId -  Transaction ID. This field uniquily identifies
 *                   transaction between RRC and RLC
 * @param ueInfo    -  Old UE Id Info for which the change request has come 
 * @param newUeInfo -  New UE Id Info for existing UE context 
 * @return ROK
 */
#ifdef ANSI
PUBLIC S16 KwUiCkwUeIdChgReq
(
Pst *pst, 
SpId spId, 
U32 transId, 
CkwUeInfo *ueInfo,
CkwUeInfo *newUeInfo
)
#else
PUBLIC S16 KwUiCkwUeIdChgReq(pst,spId,transId,ueInfo,newUeInfo)
Pst        *pst;
SpId       spId;
U32        transId;
CkwUeInfo *ueInfo;
CkwUeInfo *newUeInfo;
#endif
{
   /* kw006.201, ccpu00120574, fixed the warning */
   CmStatus    status;
   KwCkwSapCb  *ckwSap;       /* CKW SAP Control Block */
   CkwUeInfo   *tUeInfo;

   TRC3(KwUiCkwUeIdChgReq)

#ifdef DEBUGP
   /* DEBUG_PRINT */
#ifndef ALIGN_64BIT
   KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,
            "KwUiCkwUeIdChgReq(pst, spId(%d), transId(%ld))\n", spId, transId));
#else
   KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_BRIEF), (_kwPBuf,
            "KwUiCkwUeIdChgReq(pst, spId(%d), transId(%d))\n", spId, transId));
#endif
#endif
   
   /* kw006.201, ccpu00120574, fixed the warning */
   ckwSap = &(kwCb.ckwSap);
   status.reason = CKW_CFG_REAS_NONE;
   status.status = CKW_CFG_CFM_OK;
   
   if (kwCfgUeIdChng(ueInfo, newUeInfo,\
                        &status) != ROK)
   {
#ifdef DEBUGP
      KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_CFG | KW_DBGMASK_ERROR),
            (_kwPBuf, "KwUiCkwUeIdChgReq(pst, oldUe(ueId(%d),\
         cellId(%d)), newUe(ueId(%d), cellId(%d))\n kwCfgUeIdChng Failed: reason (%d)\n",
             ueInfo->ueId, ueInfo->cellId, newUeInfo->ueId, newUeInfo->cellId,
             status.reason));
#endif
   }

   /* Allocate memory for CkwUeInfo */
   KW_ALLOC(tUeInfo, sizeof(CkwUeInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (tUeInfo == NULLP)
   {
      /* Free memory for CkwUeInfo */
      /* Freeing from proper region */
      KW_PST_FREE(pst->region, pst->pool, newUeInfo, sizeof(CkwUeInfo));
      KW_PST_FREE(pst->region, pst->pool, ueInfo, sizeof(CkwUeInfo));

      KWLOGERROR(ERRCLS_ADD_RES, EKW210, (ErrVal) RFAILED,
            "KwUiCkwUeIdChgReq: tUeInfo allocation Failed.");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   tUeInfo->ueId = ueInfo->ueId;
   tUeInfo->cellId = ueInfo->cellId;

   /* kw002.201 Freeing from proper region */
   KW_PST_FREE(pst->region, pst->pool, newUeInfo, sizeof(CkwUeInfo));
   KW_PST_FREE(pst->region, pst->pool, ueInfo, sizeof(CkwUeInfo));

   KwUiCkwUeIdChgCfm(&(ckwSap->pst), ckwSap->suId, transId, tUeInfo, status);

   RETVALUE(ROK);
} /* KwUiCkwUeIdChgReq */
#endif

/*****************************************************************************
 *                          KWU INTERFACE
 ****************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for binding the RLC upper layer service user with 
 *        the RLC layer.
 *
 * @b Description:
 *
 *        This function is used by RLC user to request for binding to 
 *        RLC.This function is called by the KWU interfacei to bind 
 *        RLC's SAP (identified by spId) with the service user's
 *        SAP (identified by suId). 
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
PUBLIC S16 KwUiKwuBndReq 
(
Pst  *pst,   /* Post structure */ 
SuId suId,   /* service user SAP ID */ 
SpId spId   /* service provider identifier */
)
#else
PUBLIC S16 KwUiKwuBndReq (pst, suId, spId)
Pst *pst;    /* Post structure */
SuId suId;    /* service user SAP ID */
SpId spId;    /* service provider identifier */
#endif
{
   S16                  ret;           /* Return Value */
   KwKwuSapCb           *kwuSap;       /* SAP Config Block */
   
   TRC3(KwUiKwuBndReq)

   /* DEBUG_PRINT */
   KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
            "KwUiKwuBndReq(pst, spId(%d), suId(%d))\n", spId, suId));

   ret = ROK;
   kwuSap = NULLP;

   /* Get Sap control block */
   kwuSap = (KwKwuSapCb*)(kwCb.kwuSap + spId);

   /* Validation of input parameters */
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(!((spId < (S16) kwCb.genCfg.maxKwuSaps) && (spId >=0))) 
   {
      if(kwuSap != NULLP) 
      {
         KWLOGERROR(ERRCLS_INT_PAR, EKW211, (ErrVal) spId,
               "KwUiKwuBndReq: KWU SAP State is Invalid during Bind Req");

         KW_SEND_SAPID_ALARM(spId, LKW_EVENT_KWU_BND_REQ, LCM_CAUSE_INV_SAP);

         RETVALUE(RFAILED); 
      }
   }
#endif

   /* Verify KWU SAP State */
   switch(kwuSap->state)
   {
      /* SAP is configured but not bound */
      case KW_SAP_CFG:
      case KW_SAP_UBND:
      {
         /* copy bind configuration parameters in SSAP sap */
         kwuSap->suId          = suId;
         kwuSap->pst.dstProcId = pst->srcProcId;
         kwuSap->pst.dstEnt    = pst->srcEnt;
         kwuSap->pst.dstInst   = pst->srcInst;

         /* Update the State */
         kwuSap->state = KW_SAP_BND;

         /* DEBUG_PRINT */
         KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_DETAIL), (_kwPBuf,
                  "KwUiKwuBndReq: state (%d)\n", kwuSap->state));
         break;
      }

      /* SAP is already bound */
      case KW_SAP_BND:
      {
         /* 
          * Sap is already bound check source, destination Entity and 
          * Proc Id
          */
         if (kwuSap->pst.dstProcId != pst->srcProcId 
               || kwuSap->pst.dstEnt != pst->srcEnt
               || kwuSap->pst.dstInst != pst->srcInst
               || kwuSap->suId != suId)
         {
            KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                     "KwUiKwuBndReq(pst, suId(%d)<>suId(%d),\n\
               pst->srcProcId(%d)<>kwuSap->pst.dstProcId(%d),\n\
                     pst->srcEnt(%d)<>kwuSap->pst.dstEnt(%d),\n\
                     pst->srcInst(%d)<>kwuSap->pst.dstInst(%d))\n",\
                  suId, kwuSap->suId, pst->srcProcId, kwuSap->pst.dstProcId,\
                  pst->srcEnt, kwuSap->pst.dstEnt, pst->srcInst,\
                  kwuSap->pst.dstInst));

            KW_SEND_SAPID_ALARM(spId, LKW_EVENT_KWU_BND_REQ, LCM_CAUSE_INV_PAR_VAL);
            ret = RFAILED;
         }
         break;
      }

     default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         /* LOG ERROR */
         KWLOGERROR(ERRCLS_INT_PAR, EKW212, (ErrVal) spId,
               "KwUiKwuBndReq: KWU SAP State is Invalid during Bind Req");

         KW_SEND_SAPID_ALARM(spId, LKW_EVENT_KWU_BND_REQ, LCM_CAUSE_INV_STATE);
#endif /* ERRCLASS & ERRCLS_INT_PAR */

         KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                  "KwUiKwuBndReq: KWU SAP State is Invalid during Bind Req"));

         ret = RFAILED;
         break;
      }
   }

   if (ret == ROK)
   {
      KwUiKwuBndCfm(&(kwuSap->pst), kwuSap->suId, CM_BND_OK);
   }
   else
   {
      KwUiKwuBndCfm(&(kwuSap->pst), kwuSap->suId, CM_BND_NOK);
   }

   RETVALUE(ROK);

} /* KwUiKwuBndReq */


/**
 *
 * @brief 
 *
 *        Handler for unbinding the RLC upper layer service user with 
 *        the RLC layer.
 *
 * @b Description:
 *
 *        This function is used by RLC user to request for unbinding  
 *        with RLC.This function is called by the KWU interface to 
 *        unbind with RLC. 
 *
 *  @param[in] pst     Post structure  
 *  @param[in] suId    Service user SAP ID 
 *  @param[in] reason  Reason for Unbinding 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuUbndReq
(
Pst      *pst,       /* Post structure */
SpId     spId,       /* service provider identifier */
Reason   reason      /* Reason for Unbinding */
)
#else
PUBLIC S16 KwUiKwuUbndReq(pst, spId, reason)
Pst      *pst;       /* Post structure */
SpId     spId;       /* service provider identifier */
Reason   reason;       /* Reason for Unbinding */
#endif
{
   KwKwuSapCb  *kwuSap;            /* CKW SAP control block */

   TRC3(KwUiKwuUbndReq)

   /* DEBUG_PRINT */
   KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
            "KwUiKwuUbndReq(pst, spId(%d), reason(%d))\n", spId, reason));

   kwuSap = NULLP;

   UNUSED(pst);
   UNUSED(reason);

   /* Get Sap control block */
   kwuSap = (KwKwuSapCb*)(kwCb.kwuSap + spId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   KW_GET_AND_VALIDATE_KWUSAP(spId, kwuSap, EKW213, "KwUiKwuUbndReq");
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   kwuSap->state = KW_SAP_CFG;

   RETVALUE(ROK);
} /* KwUiKwuUbndReq */

#if 0
/**
 *
 * @brief 
 *
 *        Handler for receiving the data(SDU) from upper layer. 
 *
 * @b Description:
 *
 *        This function is used by RLC service user (PDCP/RRC) to 
 *        transfer data (SDU) to RLC.
 *
 *  @param[in] pst           Post structure  
 *  @param[in] spId        Service Provider SAP ID 
 *  @param[in] datreqInfo  Data Request Information
 *  @param[in] mBuf      Data Buffer (SDU) 
 *
 *  @return  S16
 *      -# ROK 
 *   -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuDatReq
(
Pst            *pst,            /* Post structure */
SpId           spId,          /* Service Provider SAP ID */   
KwuDatReqInfo  *datReq,       /* Data Request Information */
Buffer         *mBuf            /* Data Buffer (SDU) */
)
#else
PUBLIC S16 KwUiKwuDatReq(pst, spId, datReq, mBuf)
Pst            *pst;          /* Post structure */
SpId           spId;          /* Service Provider SAP ID */ 
KwuDatReqInfo  *datReq;       /* Data Request Information */
Buffer         *mBuf;          /* Data Buffer (SDU) */
#endif
{
   S16         ret;           /* Return Value */
   KwRbCb      *rbCb;         /* RB Control Block */
   KwKwuSapCb  *kwuSap;       /* SAP Config Block */

   TRC3(KwUiKwuDatReq)

   ret = ROK;
   rbCb = NULLP;
   kwuSap = NULLP;

   /* DEBUG_PRINT */
   KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
            "KwUiKwuDatReq(pst, spId(%d))\n", spId));

   /* Get Sap control block */
   kwuSap = (KwKwuSapCb*)(kwCb.kwuSap + spId);

   /* Validate SAP ID under ERRORCLS */
   KW_VALDATE_SAP(spId, kwuSap, ret);
   if (ret != ROK)
   {
      /* kw002.201 Freeing from proper region */
      KW_PST_FREE(pst->region, pst->pool, datReq, sizeof(KwuDatReqInfo));
      KW_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* Fetch the RbCb */
   if (kwDbmFetchRbCbForUi(datReq->rlcId, &rbCb) != ROK)
   {
      KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
               "KwUiKwuDatReq: RLCID not found ueId(%d), cellId(%d), rbId(%d)",
            datReq->rlcId.ueId, datReq->rlcId.cellId, datReq->rlcId.rbId));
      /* kw002.201 Freeing from proper region */
      KW_PST_FREE(pst->region, pst->pool, datReq, sizeof(KwuDatReqInfo));
      KW_FREE_BUF(mBuf);

      RETVALUE(RFAILED);
   }

   /* Check direction */
   KW_VALIDATE_DIR_DL(rbCb->dir);

   /* kw005.201 update the spId received in datReq to update statistics */
   rbCb->kwuSapId = spId;
   /* Dispatch according to mode of the rbCb */
   switch (rbCb->mode)
   {
      case CM_LTE_MODE_TM:
         {
            /* Verify the mode and user */
            if ( (rbCb->mode == CM_LTE_MODE_TM) && (pst->srcEnt != ENTNH) )
            {
               /* kw002.201 Freeing from proper region */
               KW_PST_FREE(pst->region, pst->pool, datReq, 
                           sizeof(KwuDatReqInfo));
               KW_FREE_BUF(mBuf);
                
               RETVALUE(RFAILED);
            }

            ret = kwTmmQSdu(rbCb, datReq, mBuf);
            if ( ret != ROK )
            {
               KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                        "KwUiKwuDatReq: kwTmmQSdu failed ueId(%d), cellId(%d), rbId(%d)",
                        datReq->rlcId.ueId, datReq->rlcId.cellId, datReq->rlcId.rbId));
            }
            /* kw005.201 ccpu00117318, updated statistics */
            kwuSap->sts.sduRx++;
            break;
         }
      case CM_LTE_MODE_UM:
         {
            ret = kwUmmQSdu(rbCb, datReq->sduId, mBuf);
            if ( ret != ROK )
            {
               KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                        "KwUiKwuDatReq: kwUmmQSdu failed ueId(%d), cellId(%d), rbId(%d)",
                        datReq->rlcId.ueId, datReq->rlcId.cellId, datReq->rlcId.rbId));
            }
            /* kw005.201 ccpu00117318, updated statistics */
            kwuSap->sts.sduRx++;
            /* Freeing from the proper region */
            /* KW_FREE(datReq, sizeof(KwuDatReqInfo)); */
            break;
         }
      case CM_LTE_MODE_AM:
         {
            ret = kwAmmQSdu(rbCb, mBuf, datReq->sduId);
            if ( ret != ROK )
            {
               KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                        "KwUiKwuDatReq: kwAmmQSdu failed ueId(%d), cellId(%d), rbId(%d)",
                        datReq->rlcId.ueId, datReq->rlcId.cellId, datReq->rlcId.rbId));
            }
            /* kw005.201 ccpu00117318, updated statistics */
            kwuSap->sts.sduRx++;
            /* Freeing from the proper region */
            /* KW_FREE(datReq, sizeof(KwuDatReqInfo)); */
            break;
         }
      default:
         {
            KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
            "KwUiKwuDatReq: wrong mode ueId(%d), cellId(%d), rbId(%d) mode(%d)",
            datReq->rlcId.ueId, datReq->rlcId.cellId, datReq->rlcId.rbId, rbCb->mode));
            break;
         }
   }
   /* kw002.201 Freeing from proper region */
   KW_PST_FREE(pst->region, pst->pool, datReq, sizeof(KwuDatReqInfo));

   RETVALUE(ret);
} /* KwUiKwuDatReq */


/**
 *
 * @brief 
 *
 *        Handler for discarding a SDU. 
 *
 * @b Description:
 *
 *        This function is used by RLC AM  and RLC UM entities.It send status 
 *        This function is called by the service user to discard a particular
 *        RLC SDU if it is present in the SDU queue of the RB control block 
 *        and if it is not mapped to any PDU. 
 *
 *  @param[in] pst            Post structure  
 *  @param[in] spId           Service Provider SAP ID 
 *  @param[in] discSduInfo    SDU discard Information 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuDiscSduReq 
(
Pst                *pst,            /* Post structure */ 
SpId                spId,            /* Service Provider ID */ 
KwuDiscSduInfo    *discSdu       /* SDU discard Information */      
)
#else
PUBLIC S16 KwUiKwuDiscSduReq(pst, spId, discSdu)
Pst               *pst;           /* Post structure */
SpId              spId;           /* Service Provider ID */ 
KwuDiscSduInfo    *discSdu;       /* SDU discard Information */           
#endif
{
   S16         ret;              /* Return Value */
   KwRbCb      *rbCb;            /* RB Control Block */
   KwKwuSapCb  *kwuSap;       /* SAP Config Block */

   TRC3(KwUiKwuDiscSduReq)

   ret = ROK;
   rbCb = NULLP;
   kwuSap = NULLP;

   /* DEBUG_PRINT */
   KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_BRIEF), (_kwPBuf,
            "KwUiKwuDiscSduReq(pst, spId(%d))\n", spId));

   /* Get Sap control block */
   kwuSap = (KwKwuSapCb*)(kwCb.kwuSap + spId);

   /* Validate SAP ID under ERRORCLS */
   KW_VALDATE_SAP(spId, kwuSap, ret);
   if (ret != ROK)
   {
      /* kw002.201 Freeing from proper region */
      KW_PST_FREE(pst->region, pst->pool, discSdu, sizeof(KwuDiscSduInfo));
      RETVALUE(RFAILED);
   }

   /* Fetch the rbCb */
   if (kwDbmFetchRbCbForUi(discSdu->rlcId, &rbCb) != ROK)
   {
      KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
               "KwUiKwuDiscSduReq: RLCID not found ueId(%d), cellId(%d), rbId(%d)",
            discSdu->rlcId.ueId, discSdu->rlcId.cellId, discSdu->rlcId.rbId));

      /* kw002.201 Freeing from proper region */
      KW_PST_FREE(pst->region, pst->pool, discSdu, sizeof(KwuDiscSduInfo));
      RETVALUE(RFAILED);
   }

   /* Check direction */
   KW_VALIDATE_DIR_DL(rbCb->dir);

   /* DIspatch according to mode */
   switch (rbCb->mode)
   {
      case CM_LTE_MODE_UM:
         {
            ret = kwUmmDiscSdu(rbCb, discSdu->sduId);
            if ( ret != ROK )
            {
               KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                "KwUiKwuDiscSduReq: kwUmmDiscSdu failed ueId(%d), cellId(%d), rbId(%d)",
                discSdu->rlcId.ueId, discSdu->rlcId.cellId, discSdu->rlcId.rbId));
            }
            break;
         }
      case CM_LTE_MODE_AM:
         {
            ret = kwAmmDiscSdu(rbCb, discSdu->sduId);
            if ( ret != ROK )
            {
               KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                "KwUiKwuDiscSduReq: kwAmmDiscSdu failed ueId(%d), cellId(%d), rbId(%d)",
                discSdu->rlcId.ueId, discSdu->rlcId.cellId, discSdu->rlcId.rbId));
            }
            break;
         }
      case CM_LTE_MODE_TM:
      default:
         {
            KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
             "KwUiKwuDiscSduReq: mode wrong ueId(%d), cellId(%d), rbId(%d), mode(%d)",
             discSdu->rlcId.ueId, discSdu->rlcId.cellId, discSdu->rlcId.rbId, rbCb->mode));
            break;
         }
   }
   /* Freeing from proper region */
   KW_PST_FREE(pst->region, pst->pool, discSdu, sizeof(KwuDiscSduInfo));

   RETVALUE(ret);
} /* KwUiKwuDiscSduReq */

#ifdef KW_PDCP

#ifdef ANSI
PUBLIC S16 kwUimDiscSdu
(
PjRbCb     *pjRbCb,
U32        sduId
)
#else
PUBLIC S16 kwUimDiscSdu(pjRbCb,sduId)
PjRbCb     *pjRbCb;
U32        sduId;
#endif
{
   KwRbCb      *rbCb;
   S16         ret;

   TRC2(kwUimDiscSdu)

   ret     = ROK;
   rbCb    = pjRbCb->kwRbCb;

   /* Check direction */
   KW_VALIDATE_DIR_DL(rbCb->dir);

   /* DIspatch according to mode */
   switch (rbCb->mode)
   {
      case CM_LTE_MODE_UM:
         {
            ret = kwUmmDiscSdu(rbCb, sduId);
            if ( ret != ROK )
            {
               KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                "KwUiKwuDiscSduReq: kwUmmDiscSdu failed ueId(%d), cellId(%d), rbId(%d)",
                rbCb->rlcId.ueId, rbCb->rlcId.cellId, rbCb->rlcId.rbId));
            }
            break;
         }
      case CM_LTE_MODE_AM:
         {
            ret = kwAmmDiscSdu(rbCb, sduId);
            if ( ret != ROK )
            {
               KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                "KwUiKwuDiscSduReq: kwAmmDiscSdu failed ueId(%d), cellId(%d), rbId(%d)",
                rbCb->rlcId.ueId, rbCb->rlcId.cellId, rbCb->rlcId.rbId));
            }
            break;
         }
      default:
         {
            ret = RFAILED;
            KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
             "KwUiKwuDiscSduReq: mode wrong ueId(%d), cellId(%d), rbId(%d), mode(%d)",
             rbCb->rlcId.ueId, rbCb->rlcId.cellId, rbCb->rlcId.rbId, rbCb->mode));
            break;
         }
   }

   RETVALUE(ret);
}

#ifdef ANSI
PUBLIC S16 kwUimDatReq
(
PjRbCb      *pjRbCb,
U32         sduId,
Buffer      *mBuf
)
#else
PUBLIC S16 kwUimDatReq(pjRbCb,sduId,mBuf)
PjRbCb      *pjRbCb;
U32         sduId;
Buffer      *mBuf;
#endif
{
   KwRbCb   *rbCb;
   S16      ret;
/*kw004.201 Adding of Missing Trace in LTE RLC PDCP*/
   TRC3(kwUimDatReq)
   rbCb     = pjRbCb->kwRbCb;
   ret      = ROK;

   switch (rbCb->mode)
   {
      case CM_LTE_MODE_UM:
         {
            ret = kwUmmQSdu(rbCb, sduId, mBuf);
            if ( ret != ROK )
            {
               KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                        "KwUiKwuDatReq: kwUmmQSdu failed ueId(%d), cellId(%d), rbId(%d)",
                        rbCb->rlcId.ueId, rbCb->rlcId.cellId, rbCb->rlcId.rbId));
            }
            break;
         }
      case CM_LTE_MODE_AM:
         {
            ret = kwAmmQSdu(rbCb, mBuf, sduId);
            if ( ret != ROK )
            {
               KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
                        "KwUiKwuDatReq: kwAmmQSdu failed ueId(%d), cellId(%d), rbId(%d)",
                        rbCb->rlcId.ueId, rbCb->rlcId.cellId, rbCb->rlcId.rbId));
            }
            break;
         }
      default:
         {
            KWDBGP((KW_DBGMASK_INF | KW_DBGMASK_ERROR), (_kwPBuf,
            "KwUiKwuDatReq: wrong mode ueId(%d), cellId(%d), rbId(%d) mode(%d)",
            rbCb->rlcId.ueId, rbCb->rlcId.cellId, rbCb->rlcId.rbId, rbCb->mode));
            break;
         }
   }

   RETVALUE(ret);
} /* kwUimDatReq */

#endif
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**
         End of file:     gp_uim.c@@/main/2 - Fri Nov 13 14:14:02 2009
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
/main/4      kw004.201 av   1. Adding of missing trace
/main/5      kw005.201 ap   1. Updated for statistics.
/main/6      kw006.201 ap   1. ccpu00120574, fixed the warning.
*********************************************************************91*/
