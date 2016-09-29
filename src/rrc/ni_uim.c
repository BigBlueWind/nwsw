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

     Name:     LTE-RRC Layer 
  
     Type:     C  file
  
     Desc:     Code for Upper Interface Primitives. 
               
     File:     nh_uim.c

     Sid:      ni_uim.c@@/main/3 - Fri Jul  1 01:13:23 2011

     Prg:      rer 

**********************************************************************/
/** @defgroup uim_group Upper Interface Module
 *  @{
 */


/* header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm_tkns.h"       /* Common Token defines */
#include "cm_llist.h"      /* Common Link List defines */
#include "cm_hash.h"       /* Common Hashlist defines */
#include "cm_lte.h"        /* Common LTE Defines */
#include "lnh.h"           /* LNH Interface defines */
#include "nhu.h"           /* NHU Interface defines */
#include "nh.h"            /* RRC defines */
#include "nhu_3gasn.h"     /* 3G RRC UE Capability/Handover Message ASN Definitions */

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"           /* Timer services */
#include "cm_lib.x"        /* Common library definitions */
#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_llist.x"      /* Common Link List definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_lte.x"        /* Common LTE includes */
#include "lnh.x"           /* LNH Interface includes */
#include "nhu.x"           /* NHU Interface includes */
#include "nh.x"            /* RRC includes */


/** Upper Layer Interface State Matrix
*/

PUBLIC PCDFS16 nhUISMTbl[NH_EVNT_UI_MAX_EVNTS] =
{
   NULLP,               /*!< Not implemented RRC Messages Handler */
   nhCPMHndlDlDirTx,    /*!< DL Information Transfer Handler */
   nhCCMSecModCmdHndlr, /*!< Security Mode Command Handler*/
   nhCCMReCfgReqHndlr,  /*!< RRC Reconfiguration Handler */
   nhCCMConRlsHndlr,    /*!< RRC COnnection Release Handler */
   nhBRMSysInfoTx,      /*!< SIB and MIB Handler */
   nhCPMHndlPagingMsg,  /*!< Paging Handler */
   nhCCMConRestbHdlr,   /*!< RRC Connection Restablishment Handler */
   nhCCMConRstbRejHdlr, /*!< RRC Connnection Reestablishment Reject Handler */
   nhCCMConSetupHdlr,   /*!< RRC Connection Setup Handler */
   nhCCMConRejHdlr,     /*!< RRC Connection Reject Handler */
   nhCPMMobFrmEutraCmd, /*!< Mobility from E-UTRA Command Handler */ 
   nhCPMHoFrmEutraPrepTfr, /*!< Handover from E-UTRA Preparation Transfer Handler */ 
   nhCPMCDMACsfbParamRsp, /*!< CSFB Parameters Response CDMA2000 */
   nhCPMUeCapEnqHndlr,   /*!< UE Capability Enquiry */
   nhCCMCountChkHndlr   /*!< RRC Counter Check Handler */
#ifdef LTERRC_REL9 
   , nhCPMUEInfoReqHndlr  /*!< UE information request handler*/
#endif /* LTERRC_REL9 */
};

/**********************************************************************
 * Upper Layer Interface
 *********************************************************************/
/**
 * @brief Upper Interface Bind Request
 * @details
 *
 *  This function binds an User to the RRC upper interface (NHU). 
 *  RRC layer software registers this new User and allocates a 
 *  Service Access Point for this bind and records the identity
 *  of the service user.
 *  It also issues a NhUiNhuBndCfm to the service user after
 *  completing a successful bind.
 *
 *  @param[in] pst Post structure
 *  @param[in] suId Service user ID
 *  @param[in] spId Service provider ID
 *
 *  @return S16 
 *   -# ROK 
 *   -# RFAILED 
 *
 *
 */
#ifdef ANSI
PUBLIC S16 NhUiNhuBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else /* ANSI */
PUBLIC S16 NhUiNhuBndReq(pst, suId, spId)
Pst *pst;
SuId suId;
SpId spId;
#endif /* ANSI */
{
   S16        ret;      /* Return value */
   NhUstaDgn  dgn;      /* Unsolicited status diagnostics structure */
   U16        event;
   Pst        lmPst;

   TRC3(NhUiNhuBndReq);

   NH_DBG_INFO((NH_PRNT_BUF, 
            "Received Bind Request from RRC user(suId(%d), spId(%d)) \n", 
            suId, spId));

   event = LCM_EVENT_BND_OK;

   /* Check if general configuration is done */
   NH_CHK_LYR_CFG(NhUiNhuBndReq, ret);
   if( ret != ROK)
   {
      /* Layer is not yet configured. Ignore this
       * stray BndReq and return 
       */
      RETVALUE(ROK);
   }
    /* Validation of input parameters */
   if (nhCb.uSapCb->state == LNH_SAP_CFG)
   {
      ret = ROK;
   }
   else
      ret = RFAILED;

   if (ret == RFAILED)
   {
      NH_DBG_ERROR((NH_PRNT_BUF,
       "Invalid SAP state(%d) while processing bind request from the RRC user", 
       nhCb.uSapCb->state));
      dgn.type    = LNH_USTA_DGNVAL_SAP;
      dgn.u.sapInfo.sapId = spId;
      dgn.u.sapInfo.sapElmId = STNHUSAP;

      /* Send Alarm to the layer manager */
      nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                                                  LCM_CAUSE_INV_SPID, &dgn);
      RETVALUE(ROK);
   } /* if (ret == RFAILED) */

   /* Check if the SAP is already bound */
   if (nhCb.uSapCb->state != LNH_SAP_BOUND)
   {
      /* Copy bind configuration parameters in SAP */
      nhCb.uSapCb->suId          = suId;
      nhCb.uSapCb->pst.dstProcId = pst->srcProcId;
      nhCb.uSapCb->pst.dstEnt    = pst->srcEnt;
      nhCb.uSapCb->pst.dstInst   = pst->srcInst;

      /* State bound and enabled */
      nhCb.uSapCb->state = LNH_SAP_BOUND;
   } /* if (nhCb.uSapCb->state != LNH_SAP_BOUND) */

   /* If the SAP is already bound, a Bind confirm is given to
    * the user and no state change is made */
   lmPst = nhCb.uSapCb->pst;
   /* Send an acknowledgement back to the user */
   if (ROK != NhUiNhuBndCfm(&lmPst, suId, CM_BND_OK))
   {
      event = LCM_EVENT_BND_FAIL;
   }
    /*  Send an Alarm to the layer manager on successful Bind */
   dgn.type    = LNH_USTA_DGNVAL_SAP;
   dgn.u.sapInfo.sapId = spId;
   dgn.u.sapInfo.sapElmId = STNHUSAP;
   nhSendLmAlarm(LCM_CATEGORY_INTERFACE, event,
                 LCM_CAUSE_UNKNOWN, &dgn);

   RETVALUE(ROK);
   
} /* NhUiNhuBndReq */

/**
 * @brief Upper Interface UnBind Request
 * @details
 *
 * This function unbinds the user SAP. The service access point
 * is not de-allocated, so a new bind  request can be issued on this 
 * SAP (without reconfiguring the SAP in the service  provider).
 *
 *  @param[in] pst    Post
 *  @param[in] spId   Service provider ID
 *  @param[in] reason Reason for unbinding
 *
 *  @return S16 
 *   -# ROK 
 *   -# RFAILED 
 *
 *
 */
#ifdef ANSI
PUBLIC S16 NhUiNhuUbndReq
(
Pst    *pst,
SpId   spId,
Reason reason
)
#else /* ANSI */
PUBLIC S16 NhUiNhuUbndReq(pst, spId, reason)
Pst    *pst;
SpId   spId;
Reason reason;
#endif /* ANSI */
{
   S16         ret;     /* Return value */
   NhUstaDgn   dgn;     /* Alarm diagnostics */

   TRC3(NhUiNhuUbndReq);

   UNUSED(pst);
   NH_DBG_INFO((NH_PRNT_BUF, 
            "received Unbind Request from RRC user(spId(%d), reason(%d)) \n", 
            spId, reason));

   NH_CHK_LYR_CFG(NhUiNhuUBndReq, ret);
   if( ret != ROK)
   {
      /*  This is a stray request. Layer is not yet configured.
       *  Ignore this request
       */
      RETVALUE(ROK);
   }

   if (nhCb.uSapCb->state != LNH_SAP_BOUND)
   {
      ret = RFAILED;
   }
   if (ret == RFAILED)
   {
      NH_DBG_ERROR((NH_PRNT_BUF,
         "Invalid SAP state(%d) while processing Unbind request",
         nhCb.uSapCb->state));
      dgn.type    = LNH_USTA_DGNVAL_SAP;
      dgn.u.sapInfo.sapId = spId;
      dgn.u.sapInfo.sapElmId = STNHUSAP;

      /* Send Alarm to the layer manager */
      nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                                                  LCM_CAUSE_INV_SPID, &dgn);
      RETVALUE(ROK);
   } /* if (ret == RFAILED) */

   if( nhCb.uSapCb->state == LNH_SAP_BOUND)
   {
      /* Set the SAP state to LNH_SAP_CFG */
      nhCb.uSapCb->state = LNH_SAP_CFG;
   } /* if nhCb.uSapCb.state == LNH_SAP_BOUND */

   RETVALUE(ROK);
} /* NhUiNhuUBndReq */

/**
 * @brief Data Response Primitives
 * @details
 *
 * This function handles the data response from the RRC user to RRC. 
 * This primitive is used only for Dl-CCCH message.
 *
 *  @param[in] pst         Post
 *  @param[in] spId        Service provider ID
 *  @param[in] nhuDataRspSdus Message SDU
 *
 *  @return S16 
 *   -# ROK 
 *   -# RFAILED 
 *
 *
 */
#ifdef ANSI
PUBLIC S16 NhUiNhuDatRsp
(
Pst           *pst,
SpId          spId,
NhuDatRspSdus *nhuDataRspSdus
)
#else /* ANSI */
PUBLIC S16 NhUiNhuDatRsp(pst, spId, nhuDataRspSdus)
Pst           *pst;
SpId          spId;
NhuDatRspSdus *nhuDataRspSdus;
#endif /* ANSI */
{
   S16            ret;           /* return value */
   U8             nhEvnt;         /* event */
   NhDatReqRspSdu datReqRspSdu;
   U8             msgTransId;
   NhuErrIndSdus  errInd;
   U8             msgChoice;
   NhuErrorInfo   *errInfo;
   NhCellAndUeCb  cellAndUeCb;

   TRC3(NhUiNhuDatRsp);
   NH_DBG_INFO((NH_PRNT_BUF, 
              "received data response from RRC User (spId(%d))\n", spId));

   UNUSED(pst);
   UNUSED(spId);

   ret = ROK;
   msgTransId = 0;
   msgChoice = 0;
   cellAndUeCb.ueCb = NULLP;
   cellAndUeCb.cellCb = NULLP;
   NH_SET_ZERO(&errInd, sizeof(NhuErrIndSdus));

   errInfo = &(errInd.sdu);
   /* Checks for NULL pointers being passed to the function  and 
      *  exits the function 
      */
   if (NH_CHK_INPUT_PARAM(nhuDataRspSdus))
   {
      NH_DBG_FATAL((NH_PRNT_BUF, "GOT NULL nhuDataRspSdus\n"));
      RETVALUE(RFAILED);
   }

   /* validate if the RB on which the UE message must be transmitted
    * is configured. Also validate if the message is a valid message on the
    * RB requested. If an error is found, then error indication is dispatched
    * from inside of this function */
   ret = nhUTLValidateDatRsp(nhuDataRspSdus, &cellAndUeCb, 
                                       &msgTransId, &nhEvnt, errInfo); 
   if(ROK != ret)
   {
      NH_SEND_ERROR_IND(errInd, nhuDataRspSdus->hdr);
      NH_FREEEVNT(nhuDataRspSdus);
      
      RETVALUE(RFAILED);
   }

   /* Validate the Configuration in the data response */
   if (TRUE == nhuDataRspSdus->sdu.isUeCfgPres)
   {
      if (ROK != nhUTLValidateUeCfg(&(nhuDataRspSdus->sdu.ueCfg), 
                                    cellAndUeCb.ueCb, errInfo))
      {
         NH_DBG_ERROR((NH_PRNT_BUF, "Config in Data Response Validation \
                  Failed  (ErrorType(%d), ErrorCause(%d)) \n",
          errInfo->errType, errInfo->errCause));
         
         NH_SEND_ERROR_IND(errInd, nhuDataRspSdus->hdr);
         NH_FREEEVNT(nhuDataRspSdus);
         RETVALUE(RFAILED);
      }
   }

   datReqRspSdu.isDatReqSdu = FALSE;
   datReqRspSdu.sdu.datRspSdu = nhuDataRspSdus;

   /* Invoke the specific handlers by invoking state machine here */
   if (nhEvnt != NH_IGNORE)
   {
      ret = nhUISMTbl[nhEvnt](&datReqRspSdu, &cellAndUeCb,
                                    msgTransId, errInfo);
      if (ROK != ret)
      {
         /* handle the errIndSdus here */
         NH_SEND_ERROR_IND(errInd, nhuDataRspSdus->hdr);
         NH_FREEEVNT(nhuDataRspSdus);
         RETVALUE(ret);
      }
   }

   NH_FREEEVNT(nhuDataRspSdus);
   
   RETVALUE(ROK);
} /* end of NhUiNhuDatRsp */


/**
 * @brief Configuration Request Primitives
 * @details
 *
 * This function handles the configuration request from the RRC user to RRC.
 *
 *  @param[in] pst             Post
 *  @param[in] spId            Service provider ID
 *  @param[in] nhuCfgReqSdu     Config request SDU
 *
 *  @return S16 
 *   -# ROK 
 *   -# RFAILED 
 *
 *
 */
#ifdef ANSI
PUBLIC S16 NhUiNhuCfgReq
(
Pst           *pst,
SpId          spId,
NhuCfgReqSdus *nhuCfgReqSdu
)
#else /* ANSI */
PUBLIC S16 NhUiNhuCfgReq(pst, spId, nhuCfgReqSdu)
Pst           *pst;
SpId          spId;
NhuCfgReqSdus *nhuCfgReqSdu;
#endif /* ANSI */
{
   S16            ret;                /* return value */
   NhuCrnti       crnti;
   NhuErrIndSdus  errInd;
   NhActvUeCb     *ueCb = NULLP;
   U8             bitMskPres;
   S16            retVal;
   TransCb       *transCb = NULLP;
   U32            transCbKey;
   NhCellCb      *cellCb = NULLP;
   Bool          createUeCtxt = FALSE;
   Bool          reCfgUeCtxt = FALSE;
   NhuUeCfgInfo  *cfgInfo = NULLP;
   NhCellAndUeCb cellAndUeCb;
   NhuErrorInfo  *errInfo;

   TRC3(NhUiNhuCfgReq);
   NH_DBG_INFO((NH_PRNT_BUF, 
           "received configuration request from RRC User (spId(%d))\n", spId));

   ret = ROK;
   crnti = 0;
   cellAndUeCb.ueCb = NULLP;
   cellAndUeCb.cellCb = NULLP;
   NH_SET_ZERO(&errInd, sizeof(NhuErrIndSdus));

   UNUSED(pst);
   UNUSED(spId);

   /* CHECK FOR NULL PTR */
   if (NH_CHK_INPUT_PARAM(nhuCfgReqSdu))
   {
      NH_DBG_FATAL((NH_PRNT_BUF, "GOT NULL nhuCfgReqSdu\n"));
      RETVALUE(RFAILED);
   }

   errInfo = &(errInd.sdu); 

/* Add Validation 
    1) hdr->ueId == 0 then ueCb == NULLP (SetUp Case) => create UE context
    2) hdr->ueId != 0 then ueCb != NULLP (re-establishment case) => ueId
                                                                    Change req
    3) hdr->ueId != 0 and UeCb == NULLP Error case
   */
   if(ROK != nhUTLValidateCfgReq(nhuCfgReqSdu, &ueCb, &cellCb, errInfo))
   {
      NH_DBG_ERROR((NH_PRNT_BUF, "UE Config request Validate failed \n"));
      NH_SEND_ERROR_IND(errInd, nhuCfgReqSdu->hdr);
      NH_FREEEVNT(nhuCfgReqSdu);
      RETVALUE(RFAILED);
   }

   /* Use CGM to update the lower layer config in ueCb
      *  It in turn issues requests at the lower layers
      */
   if (nhuCfgReqSdu->cfgInfo.ueCfgBitMsk & NHU_EXP_CFG_UE_CFG)
   {
      /* R9 upgrade: kworks Error: UE Id validation is deleted since above
       * validation func is already doing the same*/
      if(ueCb == NULLP) 
      {
         ret = nhDBMCreateUeCb(cellCb,
               nhuCfgReqSdu->cfgInfo.ueInfo.newCrnti, &ueCb);
         if (ROK != ret)
       {
            NH_DBG_ERROR((NH_PRNT_BUF, 
                    "UE CB creation failed.\n"));
            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG,
                              NHU_ERRCAUSE_UE_CB_CREATE_FAILED);
            NH_SEND_ERROR_IND(errInd, nhuCfgReqSdu->hdr);
            NH_FREEEVNT(nhuCfgReqSdu);
            RETVALUE(ret);
         }
         createUeCtxt = TRUE;
         if(nhuCfgReqSdu->cfgInfo.conReCfgCause & NHU_CONN_RECFG_AT_HNDOVR)
         {
            /* If ts HO config, RRC expects RRC Recfg Complete from UE.
               To increment the ho counter for this UE when Recfg Complete is
               received, hoToEutraOrMobFrmEutra field is set. When Recfg 
               complete is received, and if this field is set, ho success  
               counter is incremented */
            ueCb->hoToEutraOrMobFrmEutra = NH_MSGTYPE_CONNRECFGCOMP_HO; 
         }
      } /*  if(ueCb == NULLP &&  !(nhuCfgReqSdu->hdr.ueId)) */
      /* R9 upgrade: kworks Error: else if replaced with else*/
      else 
      {
         reCfgUeCtxt = TRUE;
      }
   }

   cellAndUeCb.cellCb = cellCb;
   cellAndUeCb.ueCb = ueCb;

   /* user RX transId as key */
   transCbKey = nhuCfgReqSdu->hdr.transId;
   /* create transaction block for the configuration */
   retVal = nhDBMAddTransCb(ueCb, &transCb, transCbKey);
   if (ROK != retVal)
   {
      NH_DBG_ERROR((NH_PRNT_BUF,
                "nhCGMCfgReq: Could not create Trans block \n"));
      /* In case of failure, all the changes done by RRC should be reverted
       * So UE CB is deleted when it is created here */
      if(TRUE == createUeCtxt)
      {
         nhDBMDeleteUeCb(cellAndUeCb.cellCb, &(cellAndUeCb.ueCb));
      }
      NH_FREEEVNT(nhuCfgReqSdu);
      RETVALUE(retVal);
   }

   bitMskPres = nhuCfgReqSdu->cfgInfo.ueCfgBitMsk;
   if (bitMskPres & NHU_EXP_CFG_SEC_CFG)
   {
      if(nhuCfgReqSdu->cfgInfo.conReCfgCause & NHU_CONN_RECFG_AT_HNDOVR ||
               cellAndUeCb.ueCb->rbSecCfgBitMask & NH_SEC_CFG_DONE)
      {
         /* Selective Security Mode Config is disbale in this case */
         transCb->enblSelSecModeCfg = NH_SEL_SEC_MOD_CFG_DISABLE;
      }
      else
      {
         /* Enable Selective Security Mode Config is disbale if it is
            not the in this case */
         transCb->enblSelSecModeCfg = NH_SEL_SEC_MOD_CFG_ENABLE;
      }
   }

   transCb->nxTransId = transCbKey;

   cfgInfo = &(nhuCfgReqSdu->cfgInfo);

   /* Fill the transCb cause here */
   transCb->cause = NH_TRANSCB_CFG_REQ;

   if(createUeCtxt)
   {
      transCb->ordrdCfg.ueInfo.crntiCfgType = NH_UE_INFO_NEW_CRNTI;
      transCb->ordrdCfg.ueInfo.u.newCrnti = 
         nhuCfgReqSdu->cfgInfo.ueInfo.newCrnti;
   }
   else if(reCfgUeCtxt)
   {
      transCb->ordrdCfg.ueInfo.crntiCfgType = NH_UE_INFO_MOD_CRNTI;
      transCb->ordrdCfg.ueInfo.u.modCrnti = 
         nhuCfgReqSdu->cfgInfo.ueInfo.newCrnti;
   }
  
   /* perform the specific configuration function */
   retVal = nhCGMCfgReq(&cellAndUeCb, transCb, &nhuCfgReqSdu->cfgInfo);
   if(retVal != ROK)
   {
      NH_DBG_ERROR((NH_PRNT_BUF, 
         "Lower layer config failed during UE config \n"));

      /* UE and transCb CB is deleted only if lower layer config is
         not sent */
      if((!transCb->numOfMacCfms) && (!transCb->numOfRlcCfms) &&
            (!transCb->numOfPdcpCfms)) 
      {
         /* Delete the transcb just created */
         nhDBMDelTransCb(cellAndUeCb.ueCb, &transCb);

         if(TRUE == createUeCtxt)
         {
            /* Delete the UE CB */
            nhDBMDeleteUeCb(cellAndUeCb.cellCb, &(cellAndUeCb.ueCb));
         }
      }

      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG,
            NHU_ERRCAUSE_LOW_LYR_CFG_FAIL); 

      NH_SEND_ERROR_IND(errInd, nhuCfgReqSdu->hdr);
      /* Free the CfgReqSdu */
      NH_FREEEVNT(nhuCfgReqSdu);
      RETVALUE(retVal);
   }

   NH_FREEEVNT(nhuCfgReqSdu);

   RETVALUE(ROK);

} /* end of NhUiNhuCfgReq  */

/**
 * @brief Cell Configuration Request Primitives
 * @details
 *
 * This function handles the cell configuration request from the 
 * RRC user to RRC.
 *
 *  @param[in] pst             Post
 *  @param[in] spId             Service provider ID
 *  @param[in] nhuCellCfgReqSdu Config request SDU
 *
 *  @return S16 
 *   -# ROK 
 *   -# RFAILED 
 *
 *
 */
#ifdef ANSI
PUBLIC S16 NhUiNhuCellCfgReq
(
Pst               *pst,
SpId              spId,
NhuCellCfgReqSdus *nhuCellCfgReqSdu
)
#else /* ANSI */
PUBLIC S16 NhUiNhuCellCfgReq(pst, spId, nhuCellCfgReqSdu)
Pst               *pst;
SpId              spId;
NhuCellCfgReqSdus *nhuCellCfgReqSdu;
#endif /* ANSI */
{
   S16            ret;
   NhCellCb       *cellCb;     /* Cell Control block */
   NhuErrorInfo   *errInfo;
   NhuErrIndSdus  errInd;

   TRC3(NhUiNhuCellCfgReq);
   NH_DBG_INFO((NH_PRNT_BUF, 
              "received cell configuration request from RRC User (spId(%d))\n", 
              spId));

   UNUSED(pst);
   UNUSED(spId);

   ret = ROK;
   cellCb = NULLP;
   NH_SET_ZERO(&errInd, sizeof(NhuErrIndSdus));

   /* Check if input SDU is NULL */
   if (NH_CHK_INPUT_PARAM(nhuCellCfgReqSdu))
   {
      NH_DBG_FATAL((NH_PRNT_BUF, "GOT NULL nhuCellCfgReqSdu\n"));
      RETVALUE(RFAILED);
   }

   errInfo = &(errInd.sdu);

   if ( !(nhuCellCfgReqSdu->hdr.cellId)) 
   {
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                  NHU_ERRCAUSE_INV_HDR_CELLID);
      NH_SEND_ERROR_IND(errInd, nhuCellCfgReqSdu->hdr);
      NH_FREEEVNT(nhuCellCfgReqSdu);
      RETVALUE(RFAILED);
   }

   cellCb = nhDBMGetCellCb(nhuCellCfgReqSdu->hdr.cellId);

   ret = nhUTLValidateCellCfgReq(nhuCellCfgReqSdu, cellCb, errInfo);
   if (ROK != ret)
   {
      NH_SEND_ERROR_IND(errInd, nhuCellCfgReqSdu->hdr);
      NH_FREEEVNT(nhuCellCfgReqSdu);
      RETVALUE(RFAILED);
   }

   ret = nhUIMCellCfgHndlr(nhuCellCfgReqSdu, cellCb, errInfo);
   if (ROK != ret)
   {
      NH_DBG_ERROR((NH_PRNT_BUF, 
          "Cell config failed for cell:%d \n", nhuCellCfgReqSdu->hdr.cellId));
      NH_SEND_ERROR_IND(errInd, nhuCellCfgReqSdu->hdr);
      NH_FREEEVNT(nhuCellCfgReqSdu);
      RETVALUE(ROK);
   }

   NH_FREEEVNT(nhuCellCfgReqSdu);

   RETVALUE(ROK);
} /* end of NhUiNhuCellCfgReq */

/**
 * @brief Cancel UE Request Primitives
 * @details
 *
 * This function handles the cancel UE request from the RRC user to RRC. RRC 
 * releases the lower layer configuration and finally deletes the 
 * UE context at RRC.
 *
 *  @param[in] pst      Post 
 *  @param[in] spId     Service provider ID
 *  @param[in] cellId   CellId to which the UE belongs to
 *  @param[in] crnti    CRNTI for which the UECb is deleted
 *
 *  @return S16 
 *   -# ROK 
 *   -# RFAILED 
 *
 *
 */
#ifdef ANSI
PUBLIC S16 NhUiNhuCnclUeReq
(
Pst           *pst,
SpId          spId,
NhuCellId     cellId,
NhuCrnti      crnti
)
#else /* ANSI */
PUBLIC S16 NhUiNhuCnclUeReq(pst, spId, cellId, crnti)
Pst           *pst;
SpId          spId;
NhuCellId     cellId;
NhuCrnti      crnti;
#endif /* ANSI */
{
   S16           ret;                /* return value */
   NhActvUeCb    *ueCb = NULLP;
   NhCellCb      *cellCb = NULLP;
   NhuErrIndSdus errInd;
   NhuHdr        hdr;
   NhuErrorInfo  *errInfo;

   TRC3(NhUiNhuCnclUeReq);
   NH_DBG_INFO((NH_PRNT_BUF, 
         "received cancel UE  request from RRC User (spId(%d), cellId(%d))\n", 
         spId, cellId));

   UNUSED(pst);
   UNUSED(spId);

   ret = ROK;
   NH_SET_ZERO(&errInd, sizeof(NhuErrIndSdus));

   NH_SET_ZERO(&hdr, sizeof(NhuHdr));

   errInfo = &(errInd.sdu);

   if (!cellId)
   {
      NH_DBG_ERROR((NH_PRNT_BUF, "Cell Id is Zero \n"));

      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                  NHU_ERRCAUSE_INV_HDR_CELLID);
      NH_SEND_ERROR_IND(errInd, hdr);
      RETVALUE(RFAILED);
   }

   cellCb = nhDBMGetCellCb(cellId);

   if(NULLP == cellCb)
   {
      NH_DBG_ERROR((NH_PRNT_BUF, "NULL CELL CB %d \n", cellId));
      /* CellCb does not exist. Send an error indication */
      hdr.ueId = crnti;
      hdr.cellId = cellId;

      NH_FILL_NHU_ERROR(*errInfo, NUU_ERRTYPE_INV_CNCL_UE_REQ,
                                 NHU_ERRCAUSE_CELL_NOT_CONFIGURED);

      NH_SEND_ERROR_IND(errInd, hdr);
      RETVALUE(RFAILED);
   }

   /* Get UE Control Block based on URNTI */
   if (NULLP == (ueCb = nhDBMGetUeCb(cellCb, crnti)))
   {
      NH_DBG_ERROR((NH_PRNT_BUF, "NULL UE CB %d \n", crnti));
      /* This is a case where the UE context, for which cancel request
      *  has arrived, does not exist. Send an error indication */
      hdr.ueId = crnti;
      hdr.cellId = cellId;

      NH_FILL_NHU_ERROR(*errInfo, NUU_ERRTYPE_INV_CNCL_UE_REQ,
                                 NHU_ERRCAUSE_UE_NOT_CONFIGURED);

      NH_SEND_ERROR_IND(errInd, hdr);
      RETVALUE(RFAILED);
   }

   ret = nhCGMCnclUe(cellCb, ueCb);

   RETVALUE(ret);
   
} /* end of NhUiNhuCnclUeReq */

/**
 * @brief Delete Transaction Request Primitives
 * @details
 *
 * This function deletes a transaction CB from RRC. RRC user requests the 
 * transId for the transaction.
 *
 *  @param[in] pst       Post
 *  @param[in] spId       Service provider ID
 *  @param[in] cellId     CellId to which the UE belongs
 *  @param[in] crnti      CRNTI for which the transaction needs to be removed
 *  @param[in] transId   Transaction ID of the transCb
 *
 *  @return S16 
 *   -# ROK 
 *   -# RFAILED 
 *
 *
 */
#ifdef ANSI
PUBLIC S16 NhUiNhuDeleteTransReq
(
Pst           *pst,
SpId          spId,
NhuCellId     cellId,
NhuCrnti      crnti,
U32           transId
)
#else /* ANSI */
PUBLIC S16 NhUiNhuDeleteTransReq(pst, spId, cellId, crnti, transId)
Pst           *pst;
SpId          spId;
NhuCellId     cellId;
NhuCrnti      crnti;
U32           transId;
#endif /* ANSI */
{
   S16            ret;     /* return value */
   NhActvUeCb     *ueCb;
   TransCb        *transCb;
   NhuErrIndSdus  errInd;
   NhuHdr         hdr;
   NhuErrorInfo   *errInfo;
   NhCellCb       *cellCb;

   TRC3(NhUiNhuDeleteTransReq);
   NH_DBG_INFO((NH_PRNT_BUF, 
              "received Delete Transaction request from RRC User (spId(%d), \
              cellId(%d))\n", spId, cellId));

   UNUSED(pst);
   UNUSED(spId);

   ret = ROK;
   NH_SET_ZERO(&errInd, sizeof(NhuErrIndSdus));
   errInfo = &(errInd.sdu);

   if (!cellId)
   {
      NH_DBG_ERROR((NH_PRNT_BUF, "Cell Id is Zero \n"));
      hdr.cellId = cellId;
      hdr.ueId = crnti;
      hdr.transId = transId;

      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                  NHU_ERRCAUSE_INV_HDR_CELLID);
      NH_SEND_ERROR_IND(errInd, hdr);
      RETVALUE(RFAILED);
   }

   cellCb = nhDBMGetCellCb(cellId);

   if(NULLP == cellCb)
   {
      NH_DBG_ERROR((NH_PRNT_BUF, "NULL Cell CB %d \n", cellId));
      /* CellCb does not exist. Send an error indication */
      hdr.cellId = cellId;
      hdr.ueId = crnti;
      hdr.transId = transId;

      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_DEL_TRANS_REQ,
                                 NHU_ERRCAUSE_CELL_NOT_CONFIGURED);

      NH_SEND_ERROR_IND(errInd, hdr);
      RETVALUE(RFAILED);
   }
   /* Get UE Control Block based on CRNTI */
   if (NULLP == (ueCb = (NhActvUeCb *) nhDBMGetUeCb(cellCb, crnti)))
   {
      NH_DBG_ERROR((NH_PRNT_BUF, "NULL UE CB %d \n", crnti));
      /* This is a case where the UE context, for which cancel request
       *  has arrived, does not exist. Send an error indication */
      hdr.cellId = cellId;
      hdr.ueId = crnti;
      hdr.transId = transId;

      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_DEL_TRANS_REQ,
                                  NHU_ERRCAUSE_UE_NOT_CONFIGURED);
      NH_SEND_ERROR_IND(errInd, hdr);

      RETVALUE(RFAILED);
   }

   /* Get Transaction Control Block based on ueCb */
   if (NULLP == 
         (transCb = (TransCb *)nhUTLGetTransCbFrmNxTransId(ueCb, transId)))
   {
#ifndef ALIGN_64BIT      
      NH_DBG_ERROR((NH_PRNT_BUF, "NULL TRANS CB (%ld) \n", transId));
#else /* ALIGN_64BIT */
      NH_DBG_ERROR((NH_PRNT_BUF, "NULL TRANS CB (%d) \n", transId));
#endif /* ALIGN_64BIT */
      /* This is a case where the transaction, for which Delete request
      *  has arrived, does not exist. Send an error indication */
      hdr.cellId = cellId;
      hdr.ueId = crnti;
      hdr.transId = transId;

      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_DEL_TRANS_REQ,
                                  NHU_ERRCAUSE_TRANSCB_NOT_FOUND);
      NH_SEND_ERROR_IND(errInd, hdr);

      RETVALUE(RFAILED);
   }

   ret = nhCGMDeleteTrans(ueCb, transCb);
  
   RETVALUE(ret);
} /* end of NhUiNhuDeleteTransReq */


/**
 *
 * @brief Data Request Primitive from the RRC service user to RRC
 * 
 * @details 
 * This function does the following:
 * 
 * -# Receives the Data Request primitive from the RRC user. 
 * -# Calls validate functions to validate the incoming primtive and contents.
 * -# Invokes relevant module handlers using the UIM state machine.
 * @param[in]  pst Pst       Post structure
 * @param[in]  spId SpId     Service provider ID
 * @param[in]  nhuDataReqSdus NhuDatReqSdus containing the message that must be
 *                           encoded and sent to UE along with the configuration
                             details (if required)
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 NhUiNhuDatReq
(
Pst           *pst,              /* Post structure */
SpId          spId,              /* Service provider ID */
NhuDatReqSdus *nhuDataReqSdus     /* The Message SDU */
)
#else /* ANSI */
PUBLIC S16 NhUiNhuDatReq(pst, spId, nhuDataReqSdus)
Pst           *pst;              /* Post structure */
SpId           spId;             /* Service provider ID */
NhuDatReqSdus *nhuDataReqSdus;    /* The Message SDU */
#endif /* ANSI */
{
   S16               ret;
   NhuErrIndSdus     errIndSdus;
   NhuErrorInfo      *errInfo;
   U8                event;
   U8                msgTransId;
   NhCellAndUeCb     cellAndUeCb;
   NhDatReqRspSdu    datReqRspSdu;
   NhMsgPduAndType   msgPduAndType;
   NhDatReqParam     datReqParam;
   
   TRC3(NhUiNhuDatReq);
   NH_DBG_INFO((NH_PRNT_BUF, 
              "received data request from RRC User (spId(%d))\n", spId));

   UNUSED(pst);
   UNUSED(spId);

   ret = ROK;
   event = NH_EVNT_UI_INVALID;

   cellAndUeCb.ueCb = NULLP;
   cellAndUeCb.cellCb = NULLP;
   msgTransId = 0;
   NH_SET_ZERO(&errIndSdus, sizeof(NhuErrIndSdus));
   errInfo = &(errIndSdus.sdu);

   /* Check  if input SDU is NULL */
   if (NH_CHK_INPUT_PARAM(nhuDataReqSdus))
   {
      NH_DBG_FATAL((NH_PRNT_BUF, "GOT NULL nhuDataReqSdus\n"));
      RETVALUE(RFAILED);
   }

   /* validate header and other input parameters. 
    * Error info is updated in the validate function.
    * Event for DL-DCCH structure is also updated 
    * by this validate function.
    */

   if (ROK != nhUTLValidateDatReq(nhuDataReqSdus, &cellAndUeCb, 
                               &msgTransId, errInfo, &event))
   {
      NH_DBG_ERROR((NH_PRNT_BUF,
         "Data Request Validation Failed(ErrorType(%d), ErrorCause(%d)) \n", 
            errInfo->errType, errInfo->errCause));
      NH_SEND_ERROR_IND(errIndSdus, nhuDataReqSdus->hdr);
      NH_FREEEVNT(nhuDataReqSdus);
      RETVALUE(RFAILED);
   }

   /* Validate the configuration in the data request */
   if (TRUE == nhuDataReqSdus->sdu.isUeCfgPres)
   {
      if (ROK != nhUTLValidateUeCfg(&(nhuDataReqSdus->sdu.ueCfg), 
                                    cellAndUeCb.ueCb, errInfo))
      {
         NH_DBG_ERROR((NH_PRNT_BUF, "Config in Data Request Validation \
                  Failed  (ErrorType(%d), ErrorCause(%d)) \n", 
                  errInfo->errType, errInfo->errCause));

         NH_SEND_ERROR_IND(errIndSdus, nhuDataReqSdus->hdr);
         NH_FREEEVNT(nhuDataReqSdus);
         RETVALUE(RFAILED);
      }
   }/* end of if (nhuDataReqSdus->sdu.datReqBitMsk & NHU_DAT_UE_CFG) */

   /* Message specific actions */
   if (!(nhuDataReqSdus->sdu.ueMsg.isMsgDed))
   {
      switch(nhuDataReqSdus->sdu.ueMsg.msg.cmnMsg.cmnMsgCategory)
      {
         case NHU_MSG_DLPCCH:
         {
            event = (NH_EVNT_UI_PAGING_MSG - NH_EVNT_LI_MAX_EVNTS);
         }
         break;
         case NHU_MSG_DLBCCHBCH:
         case NHU_MSG_DLBCCHDLSCH:
         {
            event = (NH_EVNT_UI_SYS_INFO_TX - NH_EVNT_LI_MAX_EVNTS);
         }
         break;
         default:
         {
            NH_DBG_ERROR((NH_PRNT_BUF,
                  "Unknown common message category(%d) \n", 
                  nhuDataReqSdus->sdu.ueMsg.msg.cmnMsg.cmnMsgCategory));
            /* Fill the error info and return */
            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_SDU_PARAM,
                                        NHU_ERRCAUSE_INV_MSG_CATEGORY);
            NH_SEND_ERROR_IND(errIndSdus, nhuDataReqSdus->hdr);
            NH_FREEEVNT(nhuDataReqSdus);
            RETVALUE(RFAILED);
         }
      } /* end of switch */
   } /* end of if (!(nhuDataReqSdus->sdu.ueMsg.isMsgDed)) */

   /* Fill the structure to be passed to the state machine */
   datReqRspSdu.isDatReqSdu = TRUE;
   datReqRspSdu.sdu.datReqSdu = nhuDataReqSdus;

   if( (TRUE == nhuDataReqSdus->sdu.ueMsg.isMsgDed) && 
       (TRUE == nhuDataReqSdus->sdu.ueMsg.msg.dedMsg.dlDcchMsg.isMsgEncoded) )
   {
      msgPduAndType.encBuf = 
              nhuDataReqSdus->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.mBuf;
      msgPduAndType.type = NHU_MSG_DLDCCH;
      NH_SET_ZERO(&(datReqParam), sizeof(NhDatReqParam));

      /* Fill nhId Parameters */
      NH_FORM_NHID_FRM_DATREQ(datReqParam.nhId, nhuDataReqSdus);
      datReqParam.isPdcpDatReq = TRUE;
      datReqParam.isMsgEncoded =
                   nhuDataReqSdus->sdu.ueMsg.msg.dedMsg.dlDcchMsg.isMsgEncoded;

      /* Encodes and sends message to UE */
      ret = nhUTLEncAndSnd(&cellAndUeCb, nhuDataReqSdus->hdr.transId,
            &msgPduAndType, &datReqParam, errInfo);
      if(ROK != ret)
      {
         NH_DBG_ERROR((NH_PRNT_BUF,"nhUTLEncAndSnd Failed.\n"));
         /* handle the errIndSdus here */
         NH_SEND_ERROR_IND(errIndSdus, nhuDataReqSdus->hdr);
      }
      NH_FREEEVNT(nhuDataReqSdus);
      RETVALUE(ret);
   }

   /* Invoke the specific handlers by invoking the state machine here */
   ret = nhUISMTbl[event](&datReqRspSdu, &cellAndUeCb,
                                 msgTransId, errInfo);
   if (ROK != ret)
   {
      /* handle the errIndSdus here */
      NH_SEND_ERROR_IND(errIndSdus, nhuDataReqSdus->hdr);
      NH_FREEEVNT(nhuDataReqSdus);
      RETVALUE(ret);
   }

   NH_FREEEVNT(nhuDataReqSdus);

   RETVALUE(ROK);
} /* end of NhUiNhuDatReq */

/**
 *
 * @brief Count Request Primitive from the RRC service user to RRC
 * 
 * @details 
 * This function does the following:
 * 
 * -# Receives the Count Request premitive from RRC user. 
 * -# Calls validate functions to validate the incoming primtive and contents.
 * -# Invokes relevant module handler using UIM state machines.
 * @param[in]  pst Pst     Post structure
 * @param[in]  spId SpId      Service provider ID
 * @param[in]  countHdr NhuHdr     Header containing UE ID, Cell ID and trans ID
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 NhUiNhuCountReq
(
Pst *pst,
SpId spId,
NhuHdr *countHdr
)
#else /* ANSI */
PUBLIC S16 NhUiNhuCountReq (pst, spId, countHdr)
Pst *pst;
SpId spId;
NhuHdr *countHdr;
#endif /* ANSI */
{
   S16               ret;
   NhuErrIndSdus     errIndSdus;
   NhuErrorInfo      *errInfo;
   U8                event;
   NhActvUeCb        *ueCb = NULLP;
   NhuHdr            hdr;

   TRC3(NhUiNhuCountReq)
   NH_DBG_INFO((NH_PRNT_BUF, 
              "Received Count request from RRC User (spId(%d))\n", spId));

   UNUSED(pst);
   UNUSED(spId);

   ret = ROK;
   event = NH_EVNT_UI_INVALID;

   NH_SET_ZERO(&errIndSdus, sizeof(NhuErrIndSdus));
   errInfo = &(errIndSdus.sdu);

   /* Check if input SDU is NULL */
   if (NH_CHK_INPUT_PARAM(countHdr))
   {
      NH_DBG_FATAL((NH_PRNT_BUF, "GOT NULL countHdr\n"));
      RETVALUE(RFAILED);
   }

   /* validate header and other input parameters. 
    * Error info is updated in the validate function.
    */
   if (ROK != nhUTLValidateCountReq(countHdr, errInfo, &event))
   {
      NH_DBG_ERROR((NH_PRNT_BUF,
         "Data Request Validation Failed(ErrorType(%d), ErrorCause(%d)) \n",
            errInfo->errType, errInfo->errCause));
      /*R9 upgrade: kworks Error: Passing value of countHdr*/ 
      NH_SEND_ERROR_IND(errIndSdus, *countHdr);
      NH_FREE(pst->region, pst->pool, countHdr, sizeof(NhuHdr));
      RETVALUE(RFAILED);
   }

   /* Invoke the specific handlers by invoking the state machine here */
   /* NOTE: ueCb is unsed in LIM*/
   ret = nhLIMCountReqHndlr(countHdr, ueCb);
   if (ROK != ret)
   {
      /* Save the id information before freeing countHdr*/
      hdr.cellId = countHdr->cellId;
      hdr.ueId = countHdr->ueId;
      hdr.transId = countHdr->transId;

      /* Free the use memory -countHdr*/
      NH_FREE(pst->region, pst->pool, countHdr, sizeof(NhuHdr));

      NH_DBG_ERROR((NH_PRNT_BUF,
         "LIM Count Req handler Failed (ErrorType(%d), ErrorCause(%d)) \n",
            errInfo->errType, errInfo->errCause));
      /* Fill the error info and return */
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG,
                                  NHU_ERRCAUSE_INV_CFG_TYPE);
      /* handle the errIndSdus here */
      NH_SEND_ERROR_IND(errIndSdus, hdr);
      RETVALUE(ret);
   }

   NH_FREE(pst->region, pst->pool, countHdr, sizeof(NhuHdr));

   RETVALUE(ROK);
} /* end of NhUiNhuCountReq */

/**
 *
 * @brief UE Capability Decode Request Primitive from the RRC service 
 * user to RRC
 * 
 * @details 
 * This funtion supports decoding to UE Capability IE only if RAT type is 
 * either of E-URAN or UTRAN. This function does the following:
 * 
 * -# Receives the UE Capability Ie decode request premitive from RRC user. 
 * -# Calls validate functions to validate the incoming primtive and contents.
 * -# Invokes relevant unpack funtions to decode the mssage received in the
 *    premitive.
 * -# Calls "NhUiNhuDecUeCapCfm" to sends decoded UE Capability Ie to RRC User.
 *
 * @param[in]  pst Pst       Post structure
 * @param[in]  spId SpId     Service provider ID
 * @param[in]  nhuDecSduReq nhuDecodeSduReq containing the message that must
 *                           be decoded
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 NhUiNhuDecodeReq
(
Pst  *pst,
SpId spId,
NhuDecodeSduReq *nhuDecSduReq
)
#else /* ANSI */
PUBLIC S16 NhUiNhuDecodeReq (pst, spId, nhuDecSduReq)
Pst  *pst;
SpId spId;
NhuDecodeSduReq *nhuDecSduReq;
#endif /* ANSI */
{
   NhuDecodeSduCfm   *nhuDecodeSduCfm = NULLP;
   NhuErrIndSdus     errIndSdus;
   NhuErrorInfo      *errInfo;
   Mem               sMem;
   U16               ret;
   Pst               *pstForCfm;

   TRC3(NhUiNhuDecodeReq)
   NH_DBG_INFO((NH_PRNT_BUF,
            "Received Decode request from RRC User (spId(%d))\n", spId));

   /* Check if input SDU is NULL */
   if (NH_CHK_INPUT_PARAM(nhuDecSduReq))
   {
      NH_DBG_FATAL((NH_PRNT_BUF, "GOT NULL nhuDecSduReq\n"));
      RETVALUE(RFAILED);
   }
   
   if (NH_CHK_INPUT_PARAM(nhuDecSduReq->mbuf))
   {
      NH_DBG_FATAL((NH_PRNT_BUF, "GOT NULL mBuf in nhuDecSduReq\n"));
      NH_FREE(pst->region, pst->pool, nhuDecSduReq, sizeof(NhuDecodeSduReq));
      RETVALUE(RFAILED);
   }

   /* Allocate memory for the Decode Confirm SDU */
   sMem.region = nhCb.init.region; 
   sMem.pool = nhCb.init.pool;

   NH_ALLOCEVNT((Ptr *)&nhuDecodeSduCfm, sizeof(NhuDecodeSduCfm), &sMem);
   if(NULLP == nhuDecodeSduCfm)
   {
      NH_DBG_FATAL((NH_PRNT_BUF,
              "NhUiNhuDecodeReq: nhuDecodeSduCfm memory allocation failed.\n"));
      NH_FREEMBUF(nhuDecSduReq->mbuf);
      NH_FREE(pst->region, pst->pool, nhuDecSduReq, sizeof(NhuDecodeSduReq));
      RETVALUE(RFAILED);
   }

   NH_SET_ZERO(&errIndSdus, sizeof(NhuErrIndSdus));
   errInfo = &(errIndSdus.sdu);

   /* Fill the cfm info */
   NH_MEM_COPY(&(nhuDecodeSduCfm->hdr), &(nhuDecSduReq->hdr), sizeof(NhuHdr));
   nhuDecodeSduCfm->sduType = nhuDecSduReq->sduType;
   nhuDecodeSduCfm->status = NHU_DECODE_SUCCESS;

   switch(nhuDecSduReq->sduType)
   {
      case NHU_DEC_SDUTYPE_EUTRA_UECAP_IE:
            ret = nhEDMDecReq(NHU_DEC_SDUTYPE_EUTRA_UECAP_IE,
                                 nhuDecSduReq->mbuf,
                                 nhuDecodeSduCfm,
                                 (Void*)&nhuDecodeSduCfm->ie.nhuUE_EUTRA_Cap);

            break;

      case NHU_DEC_SDUTYPE_UTRA_INTR_HOVR_MESSGE:
            ret = nhEDMDecReq(NHU_DEC_SDUTYPE_UTRA_INTR_HOVR_MESSGE,
                                 nhuDecSduReq->mbuf,
                                 nhuDecodeSduCfm,
                                 (Void*)&nhuDecodeSduCfm->ie.nhuUTRAUECapOctStr.
                                 nhuUE_UTRAHovrInfo);
            if (ret == ROK && (nhuDecodeSduCfm->ie.nhuUTRAUECapOctStr.
                  nhuUE_UTRAHovrInfo.ue_CapCont.choice.val == 
                  UE_CAPCONT_PRESENT))
            {
               ret = nhEDMDecReq(NHU_DEC_SDUTYPE_UTRA_UECAP_IE, 
                                 nhuDecSduReq->mbuf,
                                 nhuDecodeSduCfm,
                                 (Void*)&nhuDecodeSduCfm->ie.nhuUTRAUECapOctStr.
                                 nhuUE_UTRACap);
               if (ret != ROK)
               {
                  NH_DBG_ERROR((NH_PRNT_BUF,
                          "NhUiNhuDecodeReq: Decoding UE Cap Octate string \
                          failed\n"));
               }
               nhuDecodeSduCfm->ie.nhuUTRAUECapOctStr.containsUECap = TRUE;
            }
            else
            {
               nhuDecodeSduCfm->ie.nhuUTRAUECapOctStr.containsUECap = FALSE;
            }
            break;

      case NHU_MSG_HOCMD:
            ret = nhEDMDecReq(NHU_MSG_HOCMD,
                                 nhuDecSduReq->mbuf,
                                 nhuDecodeSduCfm,
                                 (Void*)&nhuDecodeSduCfm->ie.hovrCmd);
            break;

      case NHU_MSG_UERACAPINFO:
            ret = nhEDMDecReq(NHU_MSG_UERACAPINFO,
                                 nhuDecSduReq->mbuf,
                                 nhuDecodeSduCfm,
                                 (Void*)&nhuDecodeSduCfm->ie.ueRadioCapInfo);
            break;

      case NHU_MSG_UECAPINFO:
            ret = nhEDMDecReq(NHU_MSG_UECAPINFO,
                                 nhuDecSduReq->mbuf,
                                 nhuDecodeSduCfm,
                                 (Void*)&nhuDecodeSduCfm->ie.ueCapInfo);
            break;

      case NHU_MSG_HOPREPINFO:
            ret = nhEDMDecReq(NHU_MSG_HOPREPINFO,
                                 nhuDecSduReq->mbuf,
                                 nhuDecodeSduCfm,
                                 (Void*)&nhuDecodeSduCfm->ie.nhuHovrPrepInfo);
            break;

      default:
            NH_FREEMBUF(nhuDecSduReq->mbuf);
            /*Free the memopry allcoation of the request/cfm SDU */
            NH_FREE(pst->region, pst->pool, nhuDecSduReq, 
                                 sizeof(NhuDecodeSduReq));
            NH_DBG_ERROR((NH_PRNT_BUF,
                        "NhUiNhuDecodeReq: Invalid SDU type to decode\n"));
            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_DEC_FAILED,
                                        NHU_ERRCAUSE_INV_SDU_TYP);
            NH_SEND_ERROR_IND(errIndSdus, nhuDecodeSduCfm->hdr);
            NH_FREEEVNT(nhuDecodeSduCfm);
            RETVALUE(RFAILED);
   }

   if (ret != ROK)
   {
      /* handle the errIndSdus here */
      NH_DBG_ERROR((NH_PRNT_BUF,
               "NhUiNhuDecodeReq: Deocde failed"));
      nhuDecodeSduCfm->status = NHU_DECODE_FAIL;
   }

   /*Free the memopry allcoation of the request SDU */
   NH_FREEMBUF(nhuDecSduReq->mbuf);
   NH_FREE(pst->region, pst->pool, nhuDecSduReq, sizeof(NhuDecodeSduReq));

   /* Send Decode Confirm */
   pstForCfm = &(nhCb.uSapCb->pst);

   ret = NhUiNhuDecodeCfm(pstForCfm, nhCb.uSapCb->suId, nhuDecodeSduCfm);
   if (ret != ROK)
   {
      NH_DBG_ERROR((NH_PRNT_BUF,
               "NhUiNhuDecodeReq: Decoded IE send failed\n"));
      NH_FREEEVNT(nhuDecodeSduCfm);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of NhUiNhuDecodeReq */

/**
 *
 * @brief Encode RRC SDU request from RRC user to RRC.
 * 
 * @details 
 *    This function supports encoding of the following messages:
 *    -# DL_DCCH Msg -  This has to contain RRC Connection Reconfiguration 
 *                      message. 
 *    -# Handover Command Msg - Contains the entire DL-DCCH-Message including
 *                              the RRCConnectionReconfiguration message used to
 *                              perform handover to E-UTRAN by the target eNB. 
 *                              This message is encoded by the target eNB and 
 *                              decoded at source eNB.
 *    -# UE Radio Access Capability Information - This message is used to 
 *                              transfer UE radio access capability 
 *                              information. 
 *    -# UE Capability Information - This message is used to transfer UE 
 *                              capability information message sent by UE 
 *                              including E-UTRA, GERAN, UTRA and 
 *                              CDMA2000-1xRTT Bandclass radio access 
 *                              capabilities (separated).
 *    -# Handover Preparation Message - This message is used to transfer the 
 *                                E-UTRA RRC information used by the target eNB 
 *                                during handover preparation.
 *
 * @param[in]  pst Pst       Post structure
 * @param[in]  spId SpId     Service provider ID
 * @param[in]  nhuEncReqSdus nhuEncodeReqSdus containing the message that 
 *             must be encoded
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 NhUiNhuEncodeReq
(
Pst           *pst,
SpId          spId,
NhuEncReqSdus *nhuEncodeReqSdus
)
#else /* ANSI */
PUBLIC S16 NhUiNhuEncodeReq (pst, spId, nhuEncodeReqSdus)
Pst           *pst;
SpId          spId;
NhuEncReqSdus *nhuEncodeReqSdus;
#endif /* ANSI */
{
   Pst            *cfmPst;
   U32            bufLen;             /* length of encoded message */
   Buffer         *encBuf;            /* tmp buffer for encoded msg */
   Void           *encPdu;            /* Pdu to be encoded */
   NhuEncCfmSdus  *nhuEncCfmSdus = NULLP;
   NhuErrIndSdus  errInd;
   NhuErrorInfo   *errInfo;

   TRC3(NhUiNhuEncodeReq)

   NH_DBG_INFO((NH_PRNT_BUF, 
              "Received Encode request from RRC User (spId(%d))\n", spId));

   /* Check if input SDU is NULL */
   if (NH_CHK_INPUT_PARAM(nhuEncodeReqSdus))
   {
      NH_DBG_FATAL((NH_PRNT_BUF, "GOT NULL nhuEncodeReqSdus\n"));
      RETVALUE(RFAILED);
   }

   NH_SET_ZERO(&errInd, sizeof(NhuErrIndSdus));
   errInfo = &(errInd.sdu);

   switch(nhuEncodeReqSdus->sdu.msgCategory)
   {
      case NHU_MSG_DLDCCH:
      {
/* Conditional IE validation */
#ifdef NH_ASN_COND_VALIDATE
         if(C4_RRCCONRECFGN == 
                   nhuEncodeReqSdus->sdu.u.dcchMsg.message.val.c1.choice.val)
         {
            if(ROK != nhUTLValidateAsnCondIes((void *)nhuEncodeReqSdus, 
                                 NH_MSGTYPE_CONNRECFG, errInfo, pst->event))
            {
               NH_DBG_ERROR((NH_PRNT_BUF,
                       "NhUiNhuEncodeReq: conditional IE validation \
                       failed for Reconfiguration\n"));
               NH_SEND_ERROR_IND(errInd, nhuEncodeReqSdus->hdr);
               NH_FREEEVNT(nhuEncodeReqSdus);
               RETVALUE(RFAILED);
            }
         }
#endif /* NH_ASN_COND_VALIDATE */
         encPdu = (Void *)(&(nhuEncodeReqSdus->sdu.u.dcchMsg));
      }
      break;
      case NHU_MSG_HOCMD:
      {
         encPdu = (Void *)(&(nhuEncodeReqSdus->sdu.u.hovrCmd));
      }
      break;
      case NHU_MSG_UERACAPINFO:
      {
         encPdu = (Void *)(&(nhuEncodeReqSdus->sdu.u.ueRadioCapInfo));
      }
      break;
      case NHU_MSG_UECAPINFO:
      {
         encPdu = (Void *)(&(nhuEncodeReqSdus->sdu.u.ueCapInfo));
      }
      break;
      case NHU_MSG_HOPREPINFO:
      {
/* Conditional IE validation */
#ifdef NH_ASN_COND_VALIDATE
         if(ROK != nhUTLValidateAsnCondIes((void *)nhuEncodeReqSdus, 
                               NH_MSGTYPE_HOPREPINFO, errInfo, pst->event))
         {
            NH_DBG_ERROR((NH_PRNT_BUF,
                    "NhUiNhuEncodeReq: conditional IE validation \
                    failed for HO preparation message\n"));
            NH_SEND_ERROR_IND(errInd, nhuEncodeReqSdus->hdr);
            NH_FREEEVNT(nhuEncodeReqSdus);
            RETVALUE(RFAILED);
         }
#endif /* NH_ASN_COND_VALIDATE */

         encPdu = (Void *)(&(nhuEncodeReqSdus->sdu.u.hovrPrepInfo));
      }
      break;
#ifdef NHU_VER2
      case NHU_MSG_VARSHORTMACI:
      {
         encPdu = (Void *)(&(nhuEncodeReqSdus->sdu.u.varShortMacInput));
      }
      break;
#endif /* NHU_VER2 */
      default:
      {
         NH_DBG_ERROR((NH_PRNT_BUF,"Invalid message category for encode.\n"));
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_ENC_FAILED, 
                           NHU_ERRCAUSE_INV_MSG_CATEGORY);
         NH_SEND_ERROR_IND(errInd, nhuEncodeReqSdus->hdr);
         NH_FREEEVNT(nhuEncodeReqSdus);
         RETVALUE(RFAILED);
      }
   }

   NH_ALLOC(NH_REG, NH_POOL, &nhuEncCfmSdus, sizeof(NhuEncCfmSdus));
   if(NULLP == nhuEncCfmSdus)
   {
      NH_DBG_FATAL((NH_PRNT_BUF,"NhuEncCfmSdus memory allocation failed.\n"));
      NH_FREEEVNT(nhuEncodeReqSdus);
      RETVALUE(RFAILED);
   }

   nhuEncCfmSdus->sdu.msgCategory = nhuEncodeReqSdus->sdu.msgCategory;
   nhuEncCfmSdus->sdu.status     = NHU_ENCODE_SUCCESS;

   NH_MEM_COPY(&(nhuEncCfmSdus->hdr), &(nhuEncodeReqSdus->hdr), sizeof(NhuHdr));

   if (ROK != nhEDMEncReq(encPdu, nhuEncodeReqSdus->sdu.msgCategory, &encBuf, 
                           &bufLen))
   {
      NH_DBG_ERROR((NH_PRNT_BUF, "Encoding failed\n")); 
      nhuEncCfmSdus->sdu.status  = NHU_ENCODE_FAIL;
   }
   else
   {
      nhuEncCfmSdus->sdu.mBuf        = encBuf;
      nhuEncCfmSdus->sdu.msgLen      = bufLen;
   }

   NH_FREEEVNT(nhuEncodeReqSdus);

   cfmPst = &(nhCb.uSapCb->pst);

   NhUiNhuEncodeCfm(cfmPst, spId, nhuEncCfmSdus);

   RETVALUE(ROK);
} /* end of NhUiNhuEncodeReq */

/**
 *
 * @brief PDCP SDU status request from RRC user to RRC
 * 
 * @details 
 *    This function is used to request RRC to fetch PDCP SDU status from PDCP. 
 *    This function triggers re-establishment towards RLC and PDCP, reset 
 *    towards MAC. 
 *
 * @param[in]  pst  Post structure
 * @param[in]  spId Service provider ID
 * @param[in]  nhuHeader NHU header
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 NhUiNhuPdcpSduStaReq
(
Pst           *pst,
SpId          spId,
NhuHdr        nhuHeader
)
#else /* ANSI */
PUBLIC S16 NhUiNhuPdcpSduStaReq (pst, spId, nhuHeader)
Pst           *pst;
SpId          spId;
NhuHdr        nhuHeader;
#endif /* ANSI */
{
   NhuErrIndSdus  errInd;
   NhActvUeCb     *ueCb = NULLP;
   NhCellCb       *cellCb = NULLP;
   TransCb        *transCb = NULLP;
   U32            transCbKey;
   NhuErrorInfo   *errInfo;
   S16            retVal = ROK;
   NhCellAndUeCb  cellAndUeCb;

   TRC3(NhUiNhuPdcpSduStaReq);

   NH_DBG_INFO((NH_PRNT_BUF, 
              "PDCP SDU Status request from RRC User (spId(%d))\n", spId));

   UNUSED(pst);
   UNUSED(spId);

   retVal = ROK;

   NH_SET_ZERO(&errInd, sizeof(NhuErrIndSdus));
   errInfo = &(errInd.sdu); 

   /*  CELL CB  Validation  */
   cellCb = nhDBMGetCellCb(nhuHeader.cellId);
   if (NULLP == cellCb)
   {
      NH_DBG_ERROR((NH_PRNT_BUF,
                 "Cell CB with cell Id %d not found \n",nhuHeader.cellId));
      /* Fill the error info */
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                  NHU_ERRCAUSE_CELL_NOT_CONFIGURED);
      /* Send error indication */
      NH_SEND_ERROR_IND(errInd, nhuHeader);
      RETVALUE(RFAILED);
   }

   /*  UE CB Validation */
   ueCb = nhDBMGetUeCb(cellCb, nhuHeader.ueId);
   if(NULLP == ueCb)
   {
      NH_DBG_ERROR((NH_PRNT_BUF,
                 "Invalid UE ID in the header \n"));
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                  NHU_ERRCAUSE_INV_HDR_UEID);
      /* Send error indication */
      NH_SEND_ERROR_IND(errInd, nhuHeader);
      RETVALUE(RFAILED);  
   }      

   /* user NX transId as key */
   transCbKey = nhuHeader.transId;

   /* create transaction block for the configuration */
   retVal = nhDBMAddTransCb(ueCb, &transCb, transCbKey);
   if (ROK != retVal)
   {
      NH_DBG_ERROR((NH_PRNT_BUF,
                   "NhUiNhuPdcpSduStaReq: Could not create Trans block \n"));
      RETVALUE(retVal);
   }
   
   transCb->nxTransId = transCbKey;

   /* Fill the transCb cause Restab due to PDCP SDU status. In re-estab case, *
    * there is no ordered config stored because there is no commit when       *
    * confirms are received                                                   */
   transCb->cause = NH_TRANSCB_SDU_STA_RESTAB;

   /* Setting MAC and RLC bit mask, because re-establish request to PDCP is *
    * sent  after receiving confirm from RLC.                               */
   transCb->cfgMask = NH_MAC_CONFIGURED | NH_RLC_CONFIGURED;
   transCb->cfmMask = transCb->cfgMask;

   cellAndUeCb.cellCb = cellCb;
   cellAndUeCb.ueCb = ueCb;

   /* Fill status and action for MAC. For RLC and PDCP, transCb->cause will *
    * be used                                                               */
   transCb->rstTbl[NH_RESET_MAPTBL_IDX_MAC].status = NH_STATUS_USED;
   transCb->rstTbl[NH_RESET_MAPTBL_IDX_MAC].action = NH_MAC_CFG_RESET;

   /* Call nhLIMSendCfgMsg to send requests to lower layers. */
   retVal = nhLIMSendCfgMsg(&cellAndUeCb, transCb);
   if(retVal != ROK)
   {
      NH_DBG_ERROR((NH_PRNT_BUF,"LIM Send Cfg Faied\n"));
      RETVALUE(retVal);
   }

   RETVALUE(ROK);
} /* end of NhUiNhuPdcpSduStaReq */

/**
 * @brief Handler for Cell Config Request Primitive
 * 
 * @details 
 * 
 * This function checks whether the configuration is to add, modify, or release.
 * Call the CGM module to configure the lower layers.  
 
 * @param[in]  nhCellCfgSdu  Cell configuration request SDU
 * @param[in]  cellCb        Cell control block 
 * @param[in]  errInfo       Error info to be filled, if any error occurs during
 *                           configuration. 
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUIMCellCfgHndlr
(
NhuCellCfgReqSdus *nhCellCfgSdu, 
NhCellCb          *cellCb,
NhuErrorInfo      *errInfo
)
#else /* ANSI */
PUBLIC S16 nhUIMCellCfgHndlr(nhCellCfgSdu, cellCb, errInfo)
NhuCellCfgReqSdus *nhCellCfgSdu;
NhCellCb          *cellCb;
NhuErrorInfo      *errInfo;
#endif /* ANSI */
{
   S16            ret;
   U16            cellId;

   TRC2(nhUIMCellCfgHndlr);
   NH_DBG_DEBUG((NH_PRNT_BUF, " nhUIMCellCfgHndlr"));

   ret = ROK;

   cellId = nhCellCfgSdu->hdr.cellId;

   if (NHU_CFGTYPE_SETUP == nhCellCfgSdu->sdu.cfgType)
   {
      /* Create Cell Cb */
      ret = nhDBMCreateCellCb(cellId, &cellCb);
      if (ROK != ret)
      {
         NH_DBG_ERROR((NH_PRNT_BUF,
                  "Cannot create CELL CB %d\n", cellId));
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_CELL_CFG,
                                     NHU_ERRCAUSE_CELL_NOT_CONFIGURED);
         RETVALUE(ret);
      }
#ifdef NHU_MULT_SAP_SUPPRT
      if (nhCellCfgSdu->sdu.u.cfgInfo.cellCfgBitMsk & NHU_CELL_CFG_MSAP)
      {
         NH_MEM_COPY(&(cellCb->llyrSapIdInfo), 
                     &(nhCellCfgSdu->sdu.u.cfgInfo.llyrSapIdInfo), 
                     sizeof(NhuLLyrSapIdInfo));
      }
#endif /* NHU_MULT_SAP_SUPPRT */
   }

   /* Use CGM to update the lower layer config in Cell Cb
    * It in turn issues requests at the lower layers */
   ret = nhCGMLowerLyrCellCfgReq(nhCellCfgSdu, cellCb);
   if (ROK != ret)
   {
      NH_DBG_ERROR((NH_PRNT_BUF, 
               "Cell(%d): Cannot send lower layer config \n", cellId));
      /* In case of setup No need to clean up the cellCb as the cellCb is
       * required to handle the cfgcfms from lower layers. Only if no CfgReq
       * has been sent to the lower layers then delete the cellcb in case of
       * cell setup. */
      if (NHU_CFGTYPE_SETUP == nhCellCfgSdu->sdu.cfgType && 
            !cellCb->cellTransCb.numOfRlcCfms && 
            !cellCb->cellTransCb.numOfMacCfms)
      {
         if(ROK != nhDBMDeleteCellCb(&cellCb))
         {
            NH_DBG_ERROR((NH_PRNT_BUF, "nhDBMDeleteCellCb failed \n"));
         }
      }

      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_CELL_CFG, 
            NHU_ERRCAUSE_LOW_LYR_CFG_FAIL);

      RETVALUE(ret);
   }

   RETVALUE(ROK);
} /* end of nhUIMCellCfgHndlr */

/**
 * @brief Informs the RRC user about the result of the cell config request 
 *  
 * 
 * @details 
 * 
 *
 * This function consolidates all the confirms from MAC and RLC, which are
 * configured during the cell configuration request and send the confirm 
 * status to the RRC user.
 *
 * @param[in] cfgCfmStatus  Config confirm status during Cell configuration
 * @param[in] cellCb        Cell control block 
 * @param[in] errCause      Cause for the failure 
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUIMSndCellCfgCfm
(
U8        cfgCfmStatus, 
NhCellCb *cellCb,
U8        errCause
)
#else /* ANSI */
PUBLIC S16 nhUIMSndCellCfgCfm(cfgCfmStatus,cellCb, errCause)
U8        cfgCfmStatus;
NhCellCb *cellCb;
U8        errCause;
#endif /* ANSI */
{
   NhuCellCfgCfmSdus   *cfgCfm=NULLP;
   Pst                 pst;

   TRC2(nhUIMSndCellCfgCfm)
   NH_DBG_DEBUG((NH_PRNT_BUF, " nhUIMSndCellCfgCfm"));

   NH_ALLOC(NH_REG, NH_POOL, &cfgCfm, sizeof(NhuCellCfgCfmSdus));
   if(NULLP == cfgCfm)
   {
      NH_DBG_FATAL((NH_PRNT_BUF,
                  "NhuCellCfgCfmSdus memory allocation failed.\n"));
      RETVALUE(RFAILED);
   }
   NH_SET_ZERO(cfgCfm, sizeof(NhuCellCfgCfmSdus));

   cfgCfm->hdr.cellId = cellCb->cellId;
   cfgCfm->hdr.transId = cellCb->cellTransCb.nxTransId;

   NH_FORM_CFG_CFM_RESULT(cellCb->cellTransCb.cfgMask, *cfgCfm,
                                     cfgCfmStatus,errCause);
   
   pst = nhCb.uSapCb->pst;

   if (ROK != NhUiNhuCellCfgCfm(&pst, nhCb.uSapCb->suId, cfgCfm))
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

} /* end of nhUIMSndCellCfgCfm */

/**
 * @brief Informs the RRC user about the incoming data indication from the 
 *        lower layer
 * 
 * @details 
 * 
 *
 * This function calls the NHU interface module to send the message to NX.
 
 * @param[in] nhuDatIndSdu Data indication SDU containing the decode message.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUIMDatInd
(
NhuDatIndSdus *nhuDatIndSdu
)
#else /* ANSI */
PUBLIC S16 nhUIMDatInd(nhuDatIndSdu)
NhuDatIndSdus *nhuDatIndSdu;
#endif /* ANSI */
{
   Pst  pst;
   S16  ret;

   TRC2(nhUIMDatInd)
   NH_DBG_DEBUG((NH_PRNT_BUF, " nhUIMDatInd"));
  
   pst =  nhCb.uSapCb->pst;

   ret = NhUiNhuDatInd(&pst, nhCb.uSapCb->suId, nhuDatIndSdu);
 
   RETVALUE(ret);  

} /* end of nhUIMDatInd */

/**
 * @brief Handler for Status Indication 
 * 
 * @details 
 * 
 *
 * This function is called by the RRC layer to indicate an error has 
 * occured at RRC.
 *  
 * @param[in] nhStaInd NhuStaIndSdus containg PDCP status information (cause)
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUIMSndStaInd
(
NhuStaIndSdus *nhStaInd
)
#else /* ANSI */
PUBLIC S16 nhUIMSndStaInd(nhStaInd)
NhuStaIndSdus *nhStaInd;
#endif /* ANSI */
{
   Pst pst;

   TRC2(nhUIMSndStaInd)
   NH_DBG_DEBUG((NH_PRNT_BUF, " nhUIMSndStaInd"));

   pst = nhCb.uSapCb->pst;

   NhUiNhuStaInd(&pst, nhCb.uSapCb->suId, nhStaInd);

   RETVALUE(ROK);

} /* end of nhUIMSndStaInd */

/**
 * @brief Handler for Data confirm
 * 
 * @details 
 * 
 *
 * This function is called by RRC layer to indicate PDCP data confirm
 * to RRC user.
 *  
 * @param[in] nhDatCfm NhuDatCfmSdus Data Confirm Status 
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUIMSndDatCfm
(
NhuDatCfmSdus *nhDatCfm
)
#else /* ANSI */
PUBLIC S16 nhUIMSndDatCfm(nhDatCfm)
NhuDatCfmSdus *nhDatCfm;
#endif /* ANSI */
{
   Pst pst;

   TRC2(nhUIMSndDatCfm)
   NH_DBG_DEBUG((NH_PRNT_BUF, " nhUIMSndDatCfm"));

   pst = nhCb.uSapCb->pst;

   NhUiNhuDatCfm(&pst, nhCb.uSapCb->suId, nhDatCfm);

   RETVALUE(ROK);

} /* end of nhUIMSndDatCfm */

/**
 * @brief Handler for Error Indication 
 * 
 * @details 
 * 
 *
 * This function is called by RRC layer to indicate an error has 
 * occured at RRC.
 *  
 * @param[in] nhErrInd NhuErrIndSdus error information(type, cause)
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUIMSndErrInd
(
NhuErrIndSdus *nhErrInd
)
#else /* ANSI */
PUBLIC S16 nhUIMSndErrInd(nhErrInd)
NhuErrIndSdus *nhErrInd;
#endif /* ANSI */
{
   Pst pst;

   TRC2(nhUIMSndErrInd)
   NH_DBG_DEBUG((NH_PRNT_BUF, " nhUIMSndErrInd"));

   pst = nhCb.uSapCb->pst;

   NhUiNhuErrInd(&pst, nhCb.uSapCb->suId, nhErrInd);

   RETVALUE(ROK);

} /* end of nhUIMSndErrInd */


/**
 * @brief Handler for Consolidated Config Confirm Sent to User
 * 
 * @details 
 * 
 *
 * This function handles all the confirms from the lower layers and send a
 * consolidated confirm to the RRC user after all the confirms have been
 * received .
 
 * @param[in] nhIdentity       Structure containing cellId, ueId, & pdcpId/rbId
 * @param[in] nxTransId  Transcation Id for this transaction
 * @param[in] cfgMsk     Config Mask to indicate the layer(s) from whcih cfs
 *                       have been received
 * @param[in] cfmStatus  Success or fail
 * @param[in] errCause   Cause of Config Failure
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUIMSndCfgCfm
(
NhId          nhIdentity,
NhTransId     nxTransId,
U8            cfgMsk,
U8            cfmStatus,
U8            errCause
)
#else /* ANSI */
PUBLIC S16 nhUIMSndCfgCfm(nhIdentity, nxTransId, cfgMsk, cfmStatus, errCause)
NhId          nhIdentity;
NhTransId     nxTransId;
U8            cfgMsk;
U8            cfmStatus;
U8            errCause;
#endif /* ANSI */
{
   NhuCfgCfmSdus   *cfgCfm=NULLP;
   Pst             pst;

   TRC2(nhUIMSndCfgCfm)
   NH_DBG_DEBUG((NH_PRNT_BUF, " nhUIMSndCfgCfm"));

   NH_ALLOC(NH_REG, NH_POOL, &cfgCfm, sizeof(NhuCfgCfmSdus));
   if(NULLP == cfgCfm)
   {
      NH_DBG_FATAL((NH_PRNT_BUF,"NhuCfgCfmSdus memory allocation failed.\n"));
      RETVALUE(RFAILED);
   }
   NH_SET_ZERO(cfgCfm, sizeof(NhuCfgCfmSdus));

   NH_FORM_NHU_HDR(cfgCfm->hdr, nhIdentity); 
   cfgCfm->hdr.transId = nxTransId;

   NH_FORM_CFG_CFM_RESULT(cfgMsk, *cfgCfm, cfmStatus, errCause);
   pst = nhCb.uSapCb->pst;

   if (ROK !=  NhUiNhuCfgCfm(&pst, nhCb.uSapCb->suId, cfgCfm))
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

} /* end of nhUIMSndCfgCfm */

/**
 * @brief Handler for consolidated PDCP SDU status confirm sent to user
 * 
 * @details 
 * 
 *
 * This function handles all the confirms from the lower layers and send a
 * consolidated confirm to the RRC user after all the confirms have been
 * received .
 
 * @param[in] pdcpCfgCfm  Structure containing PDCP SDU Status confirm 
 *                        information
 * @param[in] cfgMsk      Config Mask to indicate the layer(s) from whcih cfs
 *                        have been received
 * @param[in] cfmStatus   Success or fail
 * @param[in] ueCb        UE CB
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUIMSndPdcpSduStaCfm
(
NhuPdcpSduStaCfm *pdcpCfgCfm,
U8               cfgMsk,
U8               cfmStatus,
NhActvUeCb       *ueCb
)
#else /* ANSI */
PUBLIC S16 nhUIMSndPdcpSduStaCfm(pdcpCfgCfm, cfgMsk, cfmStatus, ueCb)
NhuPdcpSduStaCfm *pdcpCfgCfm;
U8               cfgMsk;
U8               cfmStatus;
NhActvUeCb       *ueCb;
#endif /* ANSI */
{
   Pst                pst;

   TRC2(nhUIMSndPdcpSduStaCfm)

   NH_DBG_DEBUG((NH_PRNT_BUF, " nhUIMSndPdcpSduStaCfm"));

   /* Fill the status from cfgMask and cfmStatus */
   /* Status will be 
    *  - zero if success from all lower layers *
    *  - NHU_CFGERRTYPE_RLC if RLC failure     *
    *  - NHU_CFGERRTYPE_PDCP if PDCP failure   *
    *  - NHU_CFGERRTYPE_MAC if MAC failure     */
   NH_FORM_PDCP_STA_CFM_RESULT(cfgMsk, pdcpCfgCfm->status, cfmStatus); 

   pst = nhCb.uSapCb->pst;

   /* Send the PDCP SDU status confirm to RRC user */
   NhUiNhuPdcpSduStaCfm(&pst, nhCb.uSapCb->suId, pdcpCfgCfm);

   RETVALUE(ROK);

} /* end of nhUIMSndPdcpSduStaCfm */

/**
 * @brief Informs the RRC user about the result of the cell config request 
 *  
 * 
 * @details 
 * 
 *
 * This function consolidates all the confirms from MAC and RLC, which are
 * configured during the cell configuration request and send the confirm status to 
 * the RRC user.
 *
 * @param[in] nhuDRBCntInfoLst  Data construct containing Count infromation of RBs
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUIMSndCountCfm
(
NhuDRBCountInfoLst  *nhuDRBCntInfoLst
)
#else /* ANSI */
PUBLIC S16 nhUIMSndCountCfm(nhuDRBCntInfoLst)
NhuDRBCountInfoLst  *nhuDRBCntInfoLst;
#endif /* ANSI */
{
   Pst   pst;
   U16   retVal = ROK;

   TRC2(nhUIMSndCountCfm)

   NH_DBG_INFO((NH_PRNT_BUF, " nhUIMSndCountCfm"));

   pst = nhCb.uSapCb->pst;

   retVal = NhUiNhuCountCfm(&pst, nhCb.uSapCb->suId, nhuDRBCntInfoLst);

   RETVALUE(retVal);
} /* end of nhUIMSndCountCfm */

/** @} */
/**********************************************************************
  
         End of file:     ni_uim.c@@/main/3 - Fri Jul  1 01:13:23 2011
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      rer              1. LTE-RRC Initial Release.
/main/2      ---      ch               1. RRC Release 2.1.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
*********************************************************************91*/

