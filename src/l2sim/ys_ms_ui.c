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

/************************************************************************
 
     Name:     LTE MAC Convergence Layer
  
     Type:     C source file
  
     Desc:     C source code for upper interface of Convergence Layer
  
     File:     ys_ms_ui.c
  
     Sid:      
  
     Prg:      rk
  
**********************************************************************/

/* Trillium Includes */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list defines  */
#include "cm_hash.h"       /* Common hashlist defines */
#include "cm_lte.h"        /* Common LTEE defines */
#include "ys_err.h"        /* CL error header file */
#include "tfu.h"
#include "ctf.h"
#include "lys.h"
#include "ys_ms.h"

#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"        /* Common LTE includes */
#include "cm_lib.x"
#include "tfu.x"
#include "ctf.x"
#include "lys.x"

/* Silicon Includes */
#include "LtePhyL2Api.h"
#include "apidefs.h"
//#include "syscore.h"
//#include "lte_entry.h"

#include "ys_ms.x"
#include "log.h"
/*ys004.102 :  Merged MSPD code with phy 1.7 */

Bool		prntDbg = FALSE;
PRIVATE S16 ysUiChkAndGetTfuSap ARGS((
SpId       spId,
YsTfuSapType type,
YsTfuSapCb **tfuSapCb
));

void Print_Pucch_info( YsCellCb *cellCb, TfuRecpReqInfo *recpReq);
PRIVATE S16 ysUiChkAndGetCtfSap (SpId spId, YsCtfSapCb **ctfSapCb);

/* CL Control Block Structure */
PUBLIC YsCb ysCb;

#ifdef YS_MIB_WARND
/*variable introduced for temp testing */
extern Bool  acceptPrimsFrmMac;
#endif

/**
 * @brief Handler for Bind request from RRM towards CL.
 *
 * @details
 *
 *     Function : YsUiCtfBndReq
 *     
 *     This function handles the bind request from RRM.
 *     
 *           
 *  @param[in]  Pst  *pst
 *  @param[in]  SuId suId
 *  @param[in]  SpId spId
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 YsUiCtfBndReq
(
Pst  *pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 YsUiCtfBndReq(pst, suId, spId)
Pst  *pst;
SuId suId;
SpId spId;
#endif
{
   Pst       retPst;
   S16       ret;
   YsUstaDgn dgn;

   TRC2(YsUiCtfBndReq)

   YS_DBG_INFO((_ysp, "YsUiCtfBndReq(): suId = %d; spId = %d;\n", suId, spId));
   ret = ROK;

   cmMemcpy((U8 *)&retPst, (U8 *)pst, sizeof(Pst));
   retPst.dstProcId = pst->srcProcId;
   retPst.dstEnt    = pst->srcEnt;
   retPst.dstInst   = pst->srcInst;
   retPst.srcProcId = pst->dstProcId;
   retPst.srcEnt    = pst->dstEnt;
   retPst.srcInst   = pst->dstInst;

   if(spId == ysCb.ctfSap.spId)
   {
      /* Check the state of the SAP */
      switch (ysCb.ctfSap.sapState)
      {
         case LYS_NOT_CFG: /* SAP Not configured */
            YS_DBG_INFO((_ysp, "SAP Not Configured\n"));
            ret = YsUiCtfBndCfm(&retPst, suId, CM_BND_NOK);
            break;

         case LYS_UNBND: /* SAP is not bound */
            YS_DBG_INFO((_ysp, "SAP Not yet bound\n"));
            ysCb.ctfSap.sapState = LYS_BND;
            ysCb.ctfSap.suId = suId;
            ysCb.ctfSap.sapPst.dstProcId = pst->srcProcId;
            ysCb.ctfSap.sapPst.dstEnt    = pst->srcEnt;
            ysCb.ctfSap.sapPst.dstInst   = pst->srcInst;
            /* Send Bind Confirm with status as SUCCESS */
            ret = YsUiCtfBndCfm(&ysCb.ctfSap.sapPst, suId, CM_BND_OK);
            dgn.type = LYS_USTA_DGNVAL_SAP;
            ysLMMStaInd (LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK,
                  LCM_CAUSE_UNKNOWN, &dgn);
            break;

         case LYS_BND: /* SAP is already bound*/
            YS_DBG_INFO((_ysp, "SAP already bound\n"));

            ret = YsUiCtfBndCfm(&ysCb.ctfSap.sapPst, suId, CM_BND_OK);
            break;
         default: /* Should Never Enter here */
            YS_LOGERR_INTPAR(EYSXXX, (ErrVal)ysCb.ctfSap.sapState,
                  "Invalid SAP State:YsUiCtfBndReq failed\n");
            ret = YsUiCtfBndCfm(&retPst, suId, CM_BND_NOK);
            break;
      }
   }
   else
   {
      YS_LOGERR_INTPAR(EYSXXX, (ErrVal)ysCb.ctfSap.spId,
            "Invalid SAP Id:YsUiCtfBndReq failed\n");
      dgn.type = LYS_USTA_DGNVAL_SAP;
      ysLMMStaInd (LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
            LCM_CAUSE_INV_SAP, &dgn);
      ret = YsUiCtfBndCfm(&retPst, suId, CM_BND_NOK);
   }
   RETVALUE(ret);
}  /* YsUiCtfBndReq */

/**
 * @brief API for unbind request from RRM towards CL. 
 *
 * @details
 *
 *     Function: YsUiCtfUbndReq
 *     
 *     This API is invoked by RRM towards CL to unbind CTF SAP. 
 *     These API validates the Pst, spId, suId and transfers the unbind request
 *     specific information to corresponding ownership module (GOM) API.
 *
 *           
 *  @param[in]  Pst    *pst
 *  @param[in]  SuId   suId
 *  @param[in]  Reason reason
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 YsUiCtfUbndReq
(
Pst    *pst,
SpId   spId,
Reason reason
)
#else
PUBLIC S16 YsUiCtfUbndReq(pst, spId, reason)
Pst    *pst;
SpId   spId;
Reason reason;
#endif
{
   TRC2(YsUiCtfUbndReq)

   YS_DBG_INFO((_ysp, "YsUiCtfUbndReq: spId = %d; reason = %d \n", spId, reason));

   /* SAP Id validation */
   if(spId == ysCb.ctfSap.spId)
   {
      switch (ysCb.ctfSap.sapState)
      {
         case LYS_BND: /* SAP is already bound*/
            /* setting SAP state to UN BOUND */
            YS_DBG_INFO((_ysp, "SAP Is Bound\n"));
            ysCb.ctfSap.sapState = LYS_UNBND;
            break;
         default:
            YS_LOGERR_INTPAR(EYSXXX, (ErrVal)ysCb.ctfSap.sapState,
                  "Invalid SAP State: YsUiCtfUbndReq failed\n");
            RETVALUE(RFAILED);
      }
   }
   else
   {
      YS_LOGERR_INTPAR(EYSXXX, (ErrVal)ysCb.ctfSap.spId,
            "Invalid SAP Id:YsUiCtfUbndReq failed\n");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}  /* YsUiCtfUbndReq */

#if 0
/** 
 * @brief Configuration Request from RRM.
 *
 *     Function: YsUiCtfCfgReq
 *
 * @details This primitive is used by RRM for the following at PHY:
 *          - Configure Cell related parameters.
 *          - Reconfigure cell related parameters.
 *          - Release a cell context.
 *          - Configure UE specific information.
 *          - Reconfigure UE specific information.
 *          - Release UE specific configuration.
 * The cell specific configuration is identified using a cell identifier.
 * The UE specific configuration is identified using a cell identifier and 
 * UE identifier.
 * The configuration request primitive is assocated with a transaction 
 * identifier to correlate the configuration confirm returned by PHY.
 *
 * @param[in] pst Pointer to the service user task configuration structure.
 * @param[in] spId The service provider SAP ID.
 * @param[in] transId The transaction identifier.
 * @param[in] cfgReqInfo Pointer to the PHY(Cell/UE) configuration information.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
 *
*/
#ifdef ANSI
PUBLIC S16 YsUiCtfCfgReq
(
Pst*                 pst,
SpId                 spId,
CtfCfgTransId        transId,
CtfCfgReqInfo*       cfgReqInfo
)
#else
PUBLIC S16 YsUiCtfCfgReq(pst, spId, transId, cfgReqInfo)
Pst*                 pst;
SpId                 spId;
CtfCfgTransId        transId;
CtfCfgReqInfo*       cfgReqInfo;
#endif
{
   S16       ret;
   YsCtfSapCb  *ctfSapCb;

   TRC2(YsUiCtfCfgReq)
   /*uart_printf("YsUiCtfCfgReq(): spId = %d; transId = %p;\n",
            spId, transId.trans);*/

   YS_DBG_INFO((_ysp, "YsUiCtfCfgReq(): spId = %d; transId = %p;\n",
            spId, transId.trans));

   ret = ROK;

   if(ysUiChkAndGetCtfSap(spId, &ctfSapCb) != ROK)
   {
      YsUiCtfCfgCfm(&ysCb.ctfSap.sapPst, ysCb.ctfSap.suId,
            transId, CTF_CFG_CFM_NOK);
      SPutSBuf (pst->region, pst->pool, (Data *)cfgReqInfo,
         sizeof(CtfCfgReqInfo));

      RETVALUE(RFAILED);
   }

   switch(cfgReqInfo->cfgType)
   {
      case CTF_CONFIG:
         {
            switch(cfgReqInfo->u.cfg.cfgElem)
            {
               case CTF_CELL_CFG:
#ifdef PWAV_LARM_LOGS 
                  printCtfCellCfg(&cfgReqInfo->u.cfg.u.cellCfg);
#endif
		       /*uart_printf ("\n YsUiCtfCfgReq getting Cell Cfg \n");*/
                  ret = ysMsCfgAddCellCfg(&cfgReqInfo->u.cfg.u.cellCfg);
                  if( ret == ROK)
                     ctfSapCb->ctfSts.numCellCfg++;
                  break;

               case CTF_UE_CFG:
#ifdef PWAV_LARM_LOGS 
                  printCtfUeCfg(&cfgReqInfo->u.cfg.u.dedCfg);
#endif
                  ret = ysMsCfgAddUeCfg(&cfgReqInfo->u.cfg.u.dedCfg);
                  if( ret == ROK)
                     ctfSapCb->ctfSts.numUeCfg++;
                  break;

               default:
                  YS_LOGERR_INTPAR(EYSXXX, (ErrVal)cfgReqInfo->u.cfg.cfgElem,
                        "Invalid cfgElem: YsUiCtfCfgReq failed\n");
                  ret = RFAILED;

            }
         }
         break;

      case CTF_RECONFIG:
         {
            switch(cfgReqInfo->u.reCfg.cfgElem)
            {
               case CTF_CELL_CFG:
                  ret = ysMsCfgModCellCfg(&cfgReqInfo->u.reCfg.u.cellRecfg);
                  break;

               case CTF_UE_CFG:
                  ret = ysMsCfgModUeCfg(&cfgReqInfo->u.reCfg.u.dedRecfg);
                  break;
               /* Starts - Fix for CR ccpu00123185 */
               case CTF_TX_PWR_CFG:
                  /* Code to be added as part of MSPD 1.8 integration */
#ifdef MSPD                 
                  MSPD_DBG("Received CTF_TX_PWR_CFG for RefSigPwr=%d\n", 
                             cfgReqInfo->u.reCfg.u.cellRecfg.pdschCfg.refSigPwr);
                  MSPD_DBG("Received CTF_TX_PWR_CFG for pilotSigPwr=%d\n", 
                             cfgReqInfo->u.reCfg.u.cellRecfg.pilotSigPwr);
                  MSPD_DBG("Received CTF_TX_PWR_CFG for priSigPwr=%d\n", 
                             cfgReqInfo->u.reCfg.u.cellRecfg.priSigPwr);
                  MSPD_DBG("Received CTF_TX_PWR_CFG for secSigPwr=%d\n", 
                             cfgReqInfo->u.reCfg.u.cellRecfg.secSigPwr);
#endif
                  ret = ysMsCfgModTxPwrCfg(&cfgReqInfo->u.reCfg.u.cellRecfg);
                  break;
               /* Ends - Fix for CR ccpu00123185 */

               default:
                  YS_LOGERR_INTPAR(EYSXXX, (ErrVal)cfgReqInfo->u.reCfg.cfgElem,
                        "Invalid cfgElem: YsUiCtfCfgReq failed\n");
                  ret = RFAILED;

            }
         }
         break;

      case CTF_DELETE:
         {
            switch(cfgReqInfo->u.release.cfgElem)
            {
               case CTF_CELL_CFG:
                  ret = ysMsCfgDelCellCfg(cfgReqInfo->u.release.u.cellRel.cellId);
                  break;

               case CTF_UE_CFG:
                  ret = ysMsCfgDelUeCfg(&cfgReqInfo->u.release.u.dedRel);
                  break;

               default:
                  YS_LOGERR_INTPAR(EYSXXX, (ErrVal)cfgReqInfo->u.release.cfgElem,
                        "Invalid cfgElem: YsUiCtfCfgReq failed\n");
                  ret = RFAILED;

            }
         }
         break;

      default:
         YS_LOGERR_INTPAR(EYSXXX, (ErrVal)cfgReqInfo->cfgType,
               "Invalid cfgType: YsUiCtfCfgReq failed\n");
         ret = RFAILED;
   }

   /* Send Cfg confirm */
   if(ret != ROK)
   {
      YS_LOGERR_INTPAR(EYSXXX, (ErrVal)cfgReqInfo->u.reCfg.cfgElem,
            "YsUiCtfCfgReq failed\n");
	  MSPD_DBG("\n Sending UECFG Fail\n");
      YsUiCtfCfgCfm(&ysCb.ctfSap.sapPst, ysCb.ctfSap.suId,
            transId, CTF_CFG_CFM_NOK);
   }// Sudhanshu WR Change
   else
   {
      MSPD_DBG("\n Sending UECFG success\n");
      YsUiCtfCfgCfm(&ysCb.ctfSap.sapPst, ysCb.ctfSap.suId,
            transId, CTF_CFG_CFM_OK);
   }

   ysCb.ctfSap.transId = transId;

   SPutSBuf (pst->region, pst->pool, (Data *)cfgReqInfo,
      sizeof(CtfCfgReqInfo));

   RETVALUE(ret);
}  /* YsUiCtfCfgReq */

/*
 *
 *       Fun:   ysUiChkAndGetCtfSap
 *
 *       Desc:  This function checks SAP state and returns 
 *              sapCb if present
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  ys_ms_ui.c
 *
 */
/***************SEC_CHANGE_START*****************/
#if 0
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
   YsCtfSapCb  *ctfSapCb;
   YsMsSecInfo  *pSecInfo;
   CtfDatRspSdus *CtfDatRspSdu;
   YsMsSpaccIntAlgo intAlgoType;
   YsMsSpaccCiphAlgo ciphAlgoType;

   TRC2(YsUiCtfDatReq)

   YS_DBG_INFO((_ysp, "YcurrDDTsUiCtfDatReq(): spId = %d",spId));
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
     YS_DBG_ERR((_ysp, "ysMsSpaccClose failed\n"));
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
 
	   //MSPD_DBG("YsUiCtfDatReq: Storing CP Cipher Key\n"); 
           ret = ysMsSpaccUpdUeCtxt(CtfDatReqInfo->hdr.ueId,
                      pSecInfo->pOutBuf,YS_MS_SPACC_CP_CIPHER_KEY,ciphAlgoType,0);
           break;
       case YS_MS_SPACC_UP_CIPHER_KEY:
	   //MSPD_DBG("YsUiCtfDatReq: Storing UP Cipher Key\n"); 
           ret = ysMsSpaccUpdUeCtxt(CtfDatReqInfo->hdr.ueId,
                      pSecInfo->pOutBuf,YS_MS_SPACC_UP_CIPHER_KEY,ciphAlgoType,0);
           break;
       case YS_MS_SPACC_INTEG_KEY:
	   //MSPD_DBG("YsUiCtfDatReq: Storing Integrity Key\n"); 
           ret = ysMsSpaccUpdUeCtxt(CtfDatReqInfo->hdr.ueId,
                     pSecInfo->pOutBuf,YS_MS_SPACC_INTEG_KEY,0,intAlgoType);
           break;
   }

   sMem.region = ysCb.ysInit.region;
   sMem.pool = ysCb.ysInit.pool;

   cmAllocEvnt(sizeof(CtfDatRspSdus), CTF_MEM_SDU_SIZE, &sMem,(Ptr*)&CtfDatRspSdu);

   ret = ysUiChkAndGetCtfSap(spId, &ctfSapCb);

   if (ret!=ROK)
   {
     YS_DBG_ERR((_ysp, "ysUiChkAndGetCtfSap failed\n"));
     RETVALUE(RFAILED);
   }/*End of if(ret!=ROK)*/

   cmMemcpy(CtfDatRspSdu->sdu.DerivedKey,pSecInfo->pOutBuf,CTF_DER_KEY_LEN);
   CtfDatRspSdu->sdu.SecKeyTyp = CtfDatReqInfo->sdu.SecKeyTyp;
   CtfDatRspSdu->Hdr.cellId = CtfDatReqInfo->hdr.cellId;
   CtfDatRspSdu->Hdr.ueId= CtfDatReqInfo->hdr.ueId;

   YS_MS_FREE(pSecInfo->pOutBuf,YS_MS_KDF_OP_LEN);
   HeapFreeByPtr((void *)pSecInfo);
   
   ret =  ysUiCtfDatRsp(&ysCb.ctfSap.sapPst, ysCb.ctfSap.suId ,CtfDatRspSdu);
   if (ret!=ROK)
   { 
     YS_DBG_ERR((_ysp, "ysUiChkAndGetCtfSap failed\n"));
     RETVALUE(RFAILED);
   }/*End of if(ret!=ROK)*/
   RETVALUE(ROK);

}/*End of function YsUiCtfDatReq*/

/*
 *
 *       Fun:   ysUiChkAndGetCtfSap
 *
 *       Desc:  This function checks SAP state and returns 
 *              sapCb if present
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  ys_ms_ui.c
 *
 */
#endif
#endif
/***************SEC_CHANGE_END*****************/
#ifdef ANSI
PRIVATE S16 ysUiChkAndGetCtfSap
(
SpId       spId,
YsCtfSapCb **ctfSapCb
)
#else
PRIVATE S16 ysUiChkAndGetCtfSap(spId, ctfSapCb)
SpId       spId;
YsCtfSapCb **ctfSapCb;
#endif
{
   TRC3(ysUiChkAndGetCtfSap);

   *ctfSapCb = NULLP;

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(spId == ysCb.ctfSap.spId)
   {
      switch(ysCb.ctfSap.sapState)
      {
         case LYS_BND: /* SAP is already bound */
            YS_DBG_INFO((_ysp, "SAP Is already Bound\n"));
            break;
         default: /* Should never reach here */
            YS_LOGERR_INTPAR(EYSXXX, (ErrVal)ysCb.ctfSap.sapState,
                  "Invalid SAP State: ysUiChkAndGetCtfSap failed\n");
            RETVALUE(RFAILED);
      }

      *ctfSapCb = &ysCb.ctfSap;
   }
   else
   {
      YS_LOGERR_INTPAR(EYSXXX, (ErrVal)spId,
            "Invalid SAP Id:ysUiChkAndGetCtfSap failed\n");
      RETVALUE(RFAILED);
   }
#else
   *ctfSapCb = &ysCb.ctfSap;
#endif
   RETVALUE(ROK);
} /* ysUiChkAndGetCtfSap */
#endif
/**
 * @brief Handler for Bind request.
 *
 * @details
 *
 *     Function : YsUiTfuBndReq
 *     
 *     This function handles the bind request from MAC Service User.
 *     
 *           
 *  @param[in]  Pst  *pst
 *  @param[in]  SuId suId
 *  @param[in]  SpId spId
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 YsUiTfuBndReq
(
Pst  *pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 YsUiTfuBndReq(pst, suId, spId)
Pst  *pst;
SuId suId;
SpId spId;
#endif
{
   S16         ret;
   YsTfuSapCb  *tfuSap;
   Pst         retPst;
   YsUstaDgn dgn;

   TRC2(YsUiTfuBndReq)

   ret = ROK;

   logInfo( "YsUiTfuBndReq(): suId = %d; spId = %d;\n", suId, spId);

   cmMemcpy((U8 *)&retPst, (U8 *)pst, sizeof(Pst));
   retPst.dstProcId = pst->srcProcId;
   retPst.dstEnt    = pst->srcEnt;
   retPst.dstInst   = pst->srcInst;
   retPst.srcProcId = pst->dstProcId;
   retPst.srcEnt    = pst->dstEnt;
   retPst.srcInst   = pst->dstInst;

   tfuSap = ysCb.macTfuSap;
   if(tfuSap == NULLP)
   {
      logError("%s", "tfuSap is NULL:YsUiTfuBndReq failed\n");
      YsUiTfuBndCfm(&retPst, suId, CM_BND_NOK);
      RETVALUE(RFAILED);
   }

   /* Check the state of the SAP */
   switch (tfuSap->sapState)
   {
      case LYS_NOT_CFG: /* SAP Not configured */
         logInfo("%s", "SAP Not Configured\n");
         ret = YsUiTfuBndCfm(&retPst, suId, CM_BND_NOK);
         break;

      case LYS_UNBND: /* SAP is not bound */
         logInfo("%s", "SAP Not yet bound\n");
         tfuSap->suId          = suId;
         tfuSap->sapPst.dstProcId = pst->srcProcId;
         tfuSap->sapPst.dstEnt    = pst->srcEnt;
         tfuSap->sapPst.dstInst   = pst->srcInst;


         tfuSap->sapState = LYS_BND;
         /* Send Bind Confirm with status as SUCCESS */
         ret = YsUiTfuBndCfm(&tfuSap->sapPst, suId, CM_BND_OK);
            dgn.type = LYS_USTA_DGNVAL_SAP;
            ysLMMStaInd (LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK,
                  LCM_CAUSE_UNKNOWN, &dgn);
         break;

      case LYS_BND: /* SAP is already bound*/
         logInfo("%s", "SAP already bound\n");
         ret = YsUiTfuBndCfm(&tfuSap->sapPst, suId, CM_BND_OK);
         break;

      default: /* Should Never Enter here */
         logError("%s", "Invalid SAP State:YsUiTfuBndReq failed");
         dgn.type = LYS_USTA_DGNVAL_SAP;
         ysLMMStaInd (LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
               LCM_CAUSE_INV_SAP, &dgn);
         ret = YsUiTfuBndCfm(&retPst, suId, CM_BND_NOK);
         break;
   }

   RETVALUE(ret);
}  /* YsUiTfuBndReq */

/**
 * @brief API for unbind request from MAC towards CL. 
 *
 * @details
 *
 *     Function: YsUiTfuUbndReq
 *     
 *     This API is invoked by MAC towards CL to unbind TFU SAP. 
 *     These API validates the Pst, spId, suId and transfers the unbind request
 *     specific information to corresponding ownership module (GOM) API.
 *
 *           
 *  @param[in]  Pst    *pst
 *  @param[in]  SuId   suId
 *  @param[in]  Reason reason
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 YsUiTfuUbndReq
(
Pst    *pst,
SpId   spId,
Reason reason
)
#else
PUBLIC S16 YsUiTfuUbndReq(pst, spId, reason)
Pst    *pst;
SpId   spId;
Reason reason;
#endif
{
   YsTfuSapCb  *tfuSapCb;

   TRC2(YsUiTfuUbndReq)

   YS_DBG_INFO((_ysp, "YsUiTfuUbndReq: spId = %d; reason = %d \n", spId, reason));

   if(ysUiChkAndGetTfuSap(spId, LYS_TFU_USR_SAP, &tfuSapCb) != ROK)
   {
      RETVALUE(RFAILED);
   }

   tfuSapCb->sapState = LYS_UNBND;

   RETVALUE(ROK);
}  /* YsUiTfuUbndReq */

/**
 * @brief Handler for TFU sap Bind request.
 *
 * @details
 *
 *     Function : YsUiTfuSchBndReq
 *     
 *     This function handles the bind request from Scheduler. 
 *     
 *           
 *  @param[in]  Pst  *pst
 *  @param[in]  SuId suId
 *  @param[in]  SpId spId
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 YsUiTfuSchBndReq
(
Pst  *pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 YsUiTfuSchBndReq(pst, suId, spId)
Pst  *pst;
SuId suId;
SpId spId;
#endif
{
   S16         ret;
   YsTfuSapCb  *schTfuSap;
   Pst         retPst;
   YsUstaDgn dgn;

   TRC2(YsUiTfuSchBndReq)

   ret = ROK;

   logInfo("YsUiTfuSchBndReq(): suId = %d; spId = %d", suId, spId);

   cmMemcpy((U8 *)&retPst, (U8 *)pst, sizeof(Pst));
   retPst.dstProcId = pst->srcProcId;
   retPst.dstEnt    = pst->srcEnt;
   retPst.dstInst   = pst->srcInst;
   retPst.srcProcId = pst->dstProcId;
   retPst.srcEnt    = pst->dstEnt;
   retPst.srcInst   = pst->dstInst;

   if(spId >= (SpId)ysCb.genCfg.maxTfuSaps)
   {
      logError ("%s", "Invalid SAP Id:YsUiTfuSchBndReq failed");
      YsUiTfuSchBndCfm(&retPst, suId, CM_BND_NOK);
      RETVALUE(RFAILED);
   }

   schTfuSap = *(ysCb.tfuSapLst + spId);

   if(schTfuSap == NULLP)
   {
      logError("%s", "Invalid SAP Id:YsUiTfuSchBndReq failed");
      YsUiTfuSchBndCfm(&retPst, suId, CM_BND_NOK);
      RETVALUE(RFAILED);
   }

   if (schTfuSap->tfuSapCfg.type != LYS_TFU_SCH_SAP)
   {
      logError("%s", "Invalid SAP type:YsUiTfuBndReq failed");
      YsUiTfuSchBndCfm(&retPst, suId, CM_BND_NOK);
      RETVALUE(RFAILED);
   }

   /* Check the state of the SAP */
   switch (schTfuSap->sapState)
   {
      case LYS_NOT_CFG: /* SAP Not configured */
         logInfo("%s", "SAP Not Configured\n");
         ret = YsUiTfuSchBndCfm(&retPst, suId, CM_BND_NOK);
         break;

      case LYS_UNBND: /* SAP is not bound */
         logInfo("%s", "SAP Not yet bound");
         schTfuSap->suId             = suId;
         schTfuSap->sapPst.dstProcId = pst->srcProcId;
         schTfuSap->sapPst.dstEnt    = pst->srcEnt;
         schTfuSap->sapPst.dstInst   = pst->srcInst;

         schTfuSap->sapState = LYS_BND;
         /* Send Bind Confirm with status as SUCCESS */
         ret = YsUiTfuSchBndCfm(&schTfuSap->sapPst, suId, CM_BND_OK);
         dgn.type = LYS_USTA_DGNVAL_SAP;
         ysLMMStaInd (LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK,
               LCM_CAUSE_UNKNOWN, &dgn);
         break;

      case LYS_BND: /* SAP is already bound*/
         logInfo("%s", "SAP already bound\n");
         ret = YsUiTfuSchBndCfm(&schTfuSap->sapPst, suId, CM_BND_OK);
         break;

      default: /* Should Never Enter here */
         YS_LOGERR_INTPAR(EYSXXX, (ErrVal)schTfuSap->sapState,
               "Invalid SAP State:YsUiTfuSchBndReq failed\n");
         dgn.type = LYS_USTA_DGNVAL_SAP;
         ysLMMStaInd (LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
               LCM_CAUSE_INV_SAP, &dgn);
         ret = YsUiTfuSchBndCfm(&retPst, suId, CM_BND_NOK);
         break;
   }

   RETVALUE(ret);
}  /* YsUiTfuSchBndReq */

/**
 * @brief API for unbind request from MAC towards CL. 
 *
 * @details
 *
 *     Function: YsUiTfuSchUbndReq
 *     
 *     This API is invoked by MAC towards CL to unbind TFU SAP. 
 *     These API validates the Pst, spId, suId and transfers the unbind request
 *     specific information to corresponding ownership module (GOM) API.
 *
 *           
 *  @param[in]  Pst    *pst
 *  @param[in]  SuId   suId
 *  @param[in]  Reason reason
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 YsUiTfuSchUbndReq
(
Pst    *pst,
SpId   spId,
Reason reason
)
#else
PUBLIC S16 YsUiTfuSchUbndReq(pst, spId, reason)
Pst    *pst;
SpId   spId;
Reason reason;
#endif
{
   YsTfuSapCb  *tfuSapCb;

   TRC2(YsUiTfuSchUbndReq)

   YS_DBG_INFO((_ysp, "YsUiTfuSchUbndReq: spId = %d; reason = %d \n", spId, reason));

   if(ysUiChkAndGetTfuSap(spId, LYS_TFU_SCH_SAP, &tfuSapCb) != ROK)
   {
      RETVALUE(RFAILED);
   }

   tfuSapCb->sapState = LYS_UNBND;

   RETVALUE(ROK);
}  /* YsUiTfuSchUbndReq */

/**
 * @brief API for sending control information MAC to PHY
 *
 * @details
 *
 *     Function: YsUiTfuCntrlReq
 *     
 *     This API is invoked to send control information from MAC to RLC.
 *     It provides PHY with all the control information
 *       - PDCCH
 *       - PHICH
 *       - PCFICH
 *           
 *  @param[in]  pst
 *  @param[in]  spId
 *  @param[in]  cntrlReq pointer to TfuCntrlReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 YsUiTfuCntrlReq
(
Pst             *pst,
SpId            spId,
TfuCntrlReqInfo *cntrlReq
)
#else
PUBLIC S16 YsUiTfuCntrlReq(pst, spId, cntrlReq)
Pst             *pst;
SpId            spId;
TfuCntrlReqInfo *cntrlReq;
#endif
{
   S16             ret;
   YsCellCb        *cellCb;
   YsTfuSapCb      *tfuSapCb;
   volatile int    t = MacGetTick();
   U32             t2;

   TRC2(YsUiTfuCntrlReq)

   ret = ROK;

   logInfo("YsUiTfuCntrlReq(): spId = %d;\n", spId);

#ifdef YS_MIB_WARND
   /*Handling for temp testing */
   if(!acceptPrimsFrmMac)
   {
      YS_FREE_SDU(cntrlReq);
      RETVALUE(ROK);
   } 
#endif
/*{volatile trap=1; while(trap);}*/
   if(ysUiChkAndGetTfuSap(spId, LYS_TFU_SCH_SAP, &tfuSapCb) != ROK)
   {
   
      YS_FREE_SDU(cntrlReq);
      RETVALUE(RFAILED);
   }
#if 0
   if((cellCb = ysMsCfgGetCellCfg(cntrlReq->cellId)) == NULLP)
   {
   
      YS_DBG_ERR((_ysp, "ysMsCfgGetCellCfgFailed \n"));
      YS_FREE_SDU(cntrlReq);
      RETVALUE(RFAILED);
   }

   if(cellCb->phyState != LYS_PHY_STATE_RUN)
   {
      uart_printf("In TfuCntrlReq: PHY state not run\n");
      YS_DBG_ERR((_ysp, "PHY is not in running state \n"));
      YS_FREE_SDU(cntrlReq);
      RETVALUE(RFAILED);
   }
#endif   
   /* Validating sfn and subframe */
   /* This conditional needs to be modified wrt timingInfo name 
#if (ERRCLASS & ERRCLS_INT_PAR)
   if((cellCb->timingInfo.sfn != cntrlReq->timingInfo.sfn) ||
         (cellCb->timingInfo.subframe != cntrlReq->timingInfo.subframe))
   {
      YsUstaDgn dgn;
      dgn.type = LYS_USTA_DGNVAL_SFN_SF;
      cmMemcpy((U8 *)&dgn.u.timingInfo, (U8 *)&cellCb->timingInfo,
            sizeof(CmLteTimingInfo));
      ysLMMStaInd (LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
            LCM_CAUSE_INV_PAR_VAL, &dgn);
      YS_DBG_FATAL((_ysp, "Invalid Timing Info.. Expected SFN=%d Subframe=%d \
               Received SFN=%d Subframe=%d\n",
               cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
               cntrlReq->timingInfo.sfn, cntrlReq->timingInfo.subframe));

      YS_FREE_SDU(cntrlReq);
      RETVALUE(RFAILED);
   }
#endif
*/
   tfuSapCb->tfuSts.numCntrlReqRcvd++;

#if 0
   ret = ysMsDlmPrcCntrlReq(cellCb, cntrlReq);

   if(ret != ROK)
   {
      YS_DBG_ERR((_ysp, "CntrlReq processing Failed \n"));
      uart_printf("CntrlReq processing Failed \n");
      YS_FREE_SDU(cntrlReq);
      RETVALUE(RFAILED);
   }
#endif
   YS_FREE_SDU(cntrlReq);
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_CL_CNTRL_REQ, RESOURCE_LARM, t, t2 = MacGetTick());
#endif

   RETVALUE(ret);
}  /* YsUiTfuCntrlReq*/

#if 0
/**
 * @brief This Primitive carries the Data PDUs from MAC to PHY for transmission
 *
 * @details
 *
 *     Function: YsUiTfuDatReq
 *     
 * @details The data being sent in this primitive is meant to be transmitted on
 * the downlink channel PDSCH and PBCH (if present). To facilitate physical
 * layer processing, requisite control information is also sent along with the
 * data. 
 *           
 *  @param[in]  pst
 *  @param[in]  spId
 *  @param[in]  tfuDatReq pointer to TfuDatReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 YsUiTfuDatReq
(
Pst             *pst,
SpId            spId,
TfuDatReqInfo   *datReq
)
#else
PUBLIC S16 YsUiTfuDatReq(pst, spId, datReq)
Pst             *pst;
SpId            spId;
TfuDatReqInfo   *datReq;
#endif
{
   YsCellCb *cellCb;
   YsTfuSapCb *tfuSapCb;
   volatile int t = MacGetTick();
   U32 t2;


   TRC2(YsUiTfuDatReq)

   //YS_DBG_PARAM((_ysp, "YsUiTfuDatReq(): spId = %d;\n", spId));

#ifdef YS_MIB_WARND
   /*Handling for temp testing */
   if(!acceptPrimsFrmMac)
   {
      ysUtlFreeDatReq(datReq);
      RETVALUE(ROK);
   } 
#endif

   if(ysUiChkAndGetTfuSap(spId, LYS_TFU_USR_SAP, &tfuSapCb) != ROK)
   {
      ysUtlFreeDatReq(datReq);
      RETVALUE(RFAILED);
   }

   if((cellCb = ysMsCfgGetCellCfg(datReq->cellId)) == NULLP)
   {
      YS_DBG_ERR((_ysp, "ysMsCfgGetCellCfgFailed \n"));
      ysUtlFreeDatReq(datReq);
      RETVALUE(ROK);
   }

   if(cellCb->phyState != LYS_PHY_STATE_RUN)
   {
      YS_DBG_ERR((_ysp, "PHY is not in running state \n"));
      ysUtlFreeDatReq(datReq);
      RETVALUE(ROK);
   }

   tfuSapCb->tfuSts.numDatReqRcvd++;

   if (((datReq->timingInfo.sfn * YS_NUM_SUB_FRAMES) + datReq->timingInfo.subframe)
      != ((cellCb->timingInfo.sfn * YS_NUM_SUB_FRAMES) + cellCb->timingInfo.subframe
      + TFU_DLDATA_DELTA)% YS_TOTL_SUB_FRAMES_IN_SFN)
   {
      MSPD_DBG("Delayed DatReq Dropped DatReq Time(%d,%d) CL time(%d,%d)\n",
         datReq->timingInfo.sfn, datReq->timingInfo.subframe, 
         cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
   }
   else
   {
      ysMsDlmPrcDatReq(cellCb, datReq);
   }
   ysUtlFreeDatReq(datReq);

#ifdef MSPD_MLOG_NEW
   MLogTask(PID_CL_DAT_REQ, RESOURCE_LARM, t, t2 = MacGetTick());
#endif


   RETVALUE(ROK);
}  /* YsUiTfuDatReq*/
/**
 * @brief brief This primitive is sent from Scheduler to PHY.
 *
 * @details
 *
 *     Function: YsUiTfuRecpReq
 *     
 * @details This primitive provides PHY with all the information required by 
 * PHY to decode transmissions from the UE on either PUCCH or PUSCH.
 * -# On PUCCH UE can transmit the following
 *    -# SR
 *    -# HARQ feedback
 *    -# CQI report
 *    -# HARQ + CQI
 *    -# HARQ + SR
 * -# On PUSCH UE can transmit the following
 *    -# Data
 *    -# Data + CQI
 *    -# Data + HARQ Feedback
 * This primitive carries all the information for the expected subframe for all
 * the UEs that have been scheduled to transmit.
 *           
 *  @param[in]  pst
 *  @param[in]  spId
 *  @param[in]  recpReq Pointer to the TfuRecpReqInfo structure.
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 YsUiTfuRecpReq
(
Pst             *pst,
SpId            spId,
TfuRecpReqInfo  *recpReq
)
#else
PUBLIC S16 YsUiTfuRecpReq(pst, spId, recpReq)
Pst             *pst;
SpId            spId;
TfuRecpReqInfo  *recpReq;
#endif
{
   S16  ret;
   YsCellCb *cellCb;
   YsTfuSapCb *tfuSapCb;
   
   volatile int    t = MacGetTick();
   U32             t2;

   TRC2(YsUiTfuRecpReq)

   ret = ROK;

   YS_DBG_INFO((_ysp, "YsUiTfuRecpReq(): (%d) (%d);\n", 
            recpReq->timingInfo.sfn, recpReq->timingInfo.subframe));
#ifdef YS_MIB_WARND
   /*Handling for temp testing */
   if(!acceptPrimsFrmMac)
   {
      YS_FREE_SDU(recpReq);

      RETVALUE(ROK);
   } 
#endif
   if(ysUiChkAndGetTfuSap(spId, LYS_TFU_SCH_SAP, &tfuSapCb) != ROK)
   {
      YS_FREE_SDU(recpReq);
      RETVALUE(RFAILED);
   }

   if((cellCb = ysMsCfgGetCellCfg(recpReq->cellId)) == NULLP)
   {
      YS_DBG_ERR((_ysp, "ysMsCfgGetCellCfgFailed \n"));
      YS_FREE_SDU(recpReq);
      RETVALUE(RFAILED);
   }

   if(cellCb->phyState != LYS_PHY_STATE_RUN)
   {
      YS_DBG_ERR((_ysp, "PHY is not in running state \n"));
      YS_FREE_SDU(recpReq);
      RETVALUE(RFAILED);
   }
      /* Validating sfn and subframe */
#if (ERRCLASS & ERRCLS_INT_PAR)
   if((cellCb->timingInfo.sfn != recpReq->timingInfo.sfn) ||
         (cellCb->timingInfo.subframe != recpReq->timingInfo.subframe))
   {
      YsUstaDgn dgn;
      dgn.type = LYS_USTA_DGNVAL_SFN_SF;
      cmMemcpy((U8 *)&dgn.u.timingInfo, (U8 *)&cellCb->timingInfo,
            sizeof(CmLteTimingInfo));
      ysLMMStaInd (LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
            LCM_CAUSE_INV_PAR_VAL, &dgn);
      YS_DBG_FATAL((_ysp, "Invalid Timing Info.. Expected SFN=%d Subframe=%d \
               Received SFN=%d Subframe=%d\n",
               cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
               recpReq->timingInfo.sfn, recpReq->timingInfo.subframe));

      YS_FREE_SDU(recpReq);
      RETVALUE(RFAILED);
   }
#endif

   tfuSapCb->tfuSts.numRecpReqRcvd++;

   ret = ysMsUlmPrcRecpReq(cellCb, recpReq);

   if(ret != ROK)
   {
      YS_DBG_ERR((_ysp, "Failed to process Reception Request\n"));
      YS_FREE_SDU(recpReq);
      RETVALUE(RFAILED);
   }
   /* changed , not sure what will happen */
   YS_FREE_SDU(recpReq);

#ifdef MSPD_MLOG_NEW
   MLogTask(PID_CL_RECP_REQ, RESOURCE_LARM, t, t2 = MacGetTick());
#endif
   RETVALUE(ret);
}  /* YsUiTfuRecpReq*/

void Print_Pucch_info( YsCellCb *cellCb, TfuRecpReqInfo *recpReq)
{
	   CmLList          *cmLstEnt;

	   TfuUePucchRecpReq    *pucchRecpReq;
	   TfuUeRecpReqInfo   *recpReqInfo;
   
       cmLstEnt = recpReq->ueRecpReqLst.first;

       while (cmLstEnt != NULLP)
       {
           recpReqInfo = (TfuUeRecpReqInfo*)cmLstEnt->node;

		   if (recpReqInfo->type == TFU_RECP_REQ_PUCCH)
		   	{
		   	  //MSPD_DBG("TFU_RECP_REQ_PUCCH recpReqTime [%d,%d] nCce %d\n", recpReq->timingInfo.sfn, recpReq->timingInfo.subframe,
			 //	 recpReqInfo->t.pucchRecpReq.t.nCce);
		   	}

         cmLstEnt = cmLstEnt->next;
	   }

 }
#endif
/*
 *
 *       Fun:   ysUiChkAndGetTfuSap
 *
 *       Desc:  This function checks SAP state and returns 
 *              sapCb if present
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  ys_ms_ui.c
 *
 */

#ifdef ANSI
PRIVATE S16 ysUiChkAndGetTfuSap
(
SpId       spId,
YsTfuSapType type,
YsTfuSapCb **tfuSapCb
)
#else
PRIVATE S16 ysUiChkAndGetTfuSap(spId, type, tfuSapCb)
SpId       spId;
YsTfuSapType type;
YsTfuSapCb **tfuSapCb;
#endif
{
   YsTfuSapCb  *sap = NULLP;

   TRC3(ysUiChkAndGetTfuSap);


   if (type == LYS_TFU_USR_SAP)
   {
      sap = (ysCb.macTfuSap);
   }
   else if(type == LYS_TFU_SCH_SAP)
   {
      sap = *(ysCb.tfuSapLst + spId);
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(sap == NULLP)
   {
      YS_LOGERR_INTPAR(EYSXXX, (ErrVal)spId,
            "Invalid SAP Id:ysUiChkAndGetTfuSap failed\n");
      RETVALUE(RFAILED);
   }

   if (spId != sap->spId)
   {
      YS_LOGERR_INTPAR(EYSXXX, (ErrVal)spId,
            "Invalid SAP Id:ysUiChkAndGetTfuSap failed\n");
      RETVALUE(RFAILED);
   }

   switch (sap->sapState)
   {
      case LYS_BND: /* SAP is already bound */
         break;

      default: /* Should never reach here */
         YS_LOGERR_INTPAR(EYSXXX, (ErrVal)sap->sapState,
               "Invalid SAP State: YsUiTfuUbndReq failed\n");
         RETVALUE(RFAILED);
   }
#endif

   *tfuSapCb = sap;

   RETVALUE(ROK);
} /* ysUiChkAndGetTfuSap */

/********************************************************************30**
  
         End of file: 
  
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
----------- -------- ---- -----------------------------------------------
/main/1      ---      rk   1. initial release.
/main/1    ys004.102  vr   1. Merged MSPD code with phy 1.7
*********************************************************************91*/
