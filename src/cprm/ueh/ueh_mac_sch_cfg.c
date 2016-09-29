/**
 * @file    ueh_mac_sch_cfg.c
 * @brief This file contains functions to initiate mac scheduler configuration
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 25Oct2010 :
*     a) Created initial version
 */
 

#include "ccpu_cmn.h"
 
 /* RRM related includes */
#include "ueh_eut.h"
#include "lueh.h"
#include"ueh_eut_x.h"
#include "lueh.x" 
#include "ueh.h" 
#include "ueh_crm.h"
#include "ueh_pucch_res_mgmt.h"


/* This function prepares Schedular Cofiguration 
 * It should be called during cell setup procedure.
 * Returns: ROK in case of success RFAILED in case of error
 */

void UehPrepRgrCfgReq(UehUeCb * ueCb,	 RgrCfgReqInfo *cfgInfo)
{
  RgrUeCfg *ueCfg = NULLP;
  logFunc("%s", "UehPrepRgrCfgReq");

  cfgInfo->action = RGR_CONFIG;
  cfgInfo->u.cfgInfo.cfgType = RGR_UE_CFG;
  ueCb->ueRgrCfgState = RGR_UE_CFG;
  ueCfg = &(cfgInfo->u.cfgInfo.u.ueCfg);

  ueCfg->cellId = ueCb->cellId;
  ueCfg->crnti = ueCb->crnti;
  /* UE Transmission mode */
  uehFillRgrUeTxModeCfg(&ueCfg->txMode);
  /* UE DL CQI config */
  uehFillRgrUeDlCqiCfg(&ueCfg->ueDlCqiCfg,ueCb,ueCfg->txMode.txModeEnum);
  /* UE related UL Harq Config */
  uehFillRgrUeUlHqCfg(&ueCfg->ueUlHqCfg);
  /* UE UL power config */
  uehFillRgrUeUlPwrCfg(&ueCfg->ueUlPwrCfg);
  /* UE related Dl/UL QoS config: AMBR */
  uehFillRgrUeQosCfg(&ueCfg->ueQosCfg);
  /* UE TA timer config */
  uehFillRgrUeTaTmrCfg(&ueCfg->ueTaTmrCfg);
#ifdef RGR_V1
  /* UE BSR timer config */
  uehFillRgrUeBsrTmrCfg(&ueCfg->ueBsrTmrCfg);
#endif

#ifdef RGR_CQI_REPT
  /*Number of CQI report */
  uehFillRgrUeNumCqiRept(&ueCfg->ueCqiReptCfg);
#endif

  /* UE category */
  ueCfg->ueCatEnum = smCb.cfgCb.uehConfig.ueCategory; //uehDfltUECategory;
#ifdef LTEMAC_DRX
  /* UE specific DRX configuration */
  uehFillRgrUeDrxCfg(&ueCfg->ueDrxCfg);
#endif
  /* TtiBundling Enable/Disabled for UE */
  ueCfg->isTtiBundlEnabled = FALSE;

  /* Ack/Nack Configuration for UE */
  uehFillRgrUeAckNackRepCfg(&ueCfg->ueAckNackCfg);

  /* measurement gap configuration for UE */
  uehFillRgrUeMeasGapCfg(&ueCfg->ueMesGapCfg);

  /* Ue Capabilty configuration */
  uehFillRgrUeCapCfg(&ueCfg->ueCapCfg);


  /* Number of bits in codebook for Transmission modes */
  uehFillRgrCodeBookRstCfg(&ueCfg->ueCodeBookRstCfg);

  /* If present then mapping exists at RGR user with crnti */
  ueCfg->dedPreambleId.pres = NOTPRSNT;
  ueCfg->dedPreambleId.val = NOTPRSNT;

#ifdef LTE_TDD
  uehFillRgrTddAckNackMode(&ueCfg->ackNackModeEnum);
#endif
  /* ueSpsCfg - TODO */
  
#ifdef TFU_UPGRADE
  ueCfg->srsCfg.type = 0;
  uehFillRgrUeSrCfg(&ueCfg->srCfg,ueCb);
#endif
  ueCfg->puschDedCfg.bCQIIdx = uehCb.uehCfgCb.phyCfg.betaOffsetCqiIndex;
  uehFillRgrPuschDedCfg(&ueCfg->puschDedCfg);
  ueCfg->ulTxAntSel.pres = FALSE;
#ifdef TFU_UPGRADE
  ueCfg->uePdschDedCfg.uepACfg.pAPrsnt = TRUE; 
  ueCfg->uePdschDedCfg.uepACfg.pA = uehCb.uehCfgCb.phyCfg.cellPhyPa;
#endif
}


 /* This function prepares Schedular Reconfiguration 
	* Returns: ROK in case of success RFAILED in case of error
	*/

PUBLIC S16 UehPrepRgrReCfgReq(
    UehUeCb * ueCb,
    RgrCfgReqInfo *cfgInfo
    )
{
  RgrUeRecfg *ueRecfg = NULLP;
  logFunc("%s", "UehPrepRgrReCfgReq");

  ueCb->ueRgrCfgState = RGR_UE_CFG;

  cfgInfo->action = RGR_RECONFIG;
  cfgInfo->u.recfgInfo.recfgType = RGR_UE_CFG;

  ueRecfg = &(cfgInfo->u.recfgInfo.u.ueRecfg);
  ueRecfg->cellId = ueCb->cellId;
  ueRecfg->oldCrnti = ueCb->crnti;
  ueRecfg->newCrnti = ueCb->crnti;

#ifdef LTEMAC_MIMO
  /* Rgr Recfg for UE Category */
  if(ueRecfg->ueRecfgTypes & RGR_UE_UECAT_RECFG)
  {
    ueRecfg->ueCatEnum = ueCb->ueEutraCap.ueCategory;
  }
  /* TODO: Add the code for other RGR recfg parameters */
#ifdef LTEMAC_SPS
  if(ueRecfg->ueRecfgTypes & RGR_DL_LC_QOS_RECFG)
  {
    ueRecfg->ueQosRecfg.ambrPres = TRUE;
    ueRecfg->ueQosRecfg.dlAmbr= ueCb->aggMaxBr.dl;
    ueRecfg->ueQosRecfg.ueBr= ueCb->aggMaxBr.ul;
  }
#endif
#endif
	if (uehCb.uehCfgCb.cellCfg.ueCfgMacCollatedCqiCount)
	{
		ueRecfg->ueRecfgTypes |=  RGR_UE_CQIREPT_RECFG;
		ueRecfg->ueCqiReptCfg.numColltdCqiRept = uehCb.uehCfgCb.cellCfg.ueCfgMacCollatedCqiCount;
	}

  RETVALUE(ROK);
}
