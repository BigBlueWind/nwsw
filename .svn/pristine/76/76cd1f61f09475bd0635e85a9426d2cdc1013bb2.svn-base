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
#include "syscore.h"
#include "lte_entry.h"

#include "ys_ms.x"
#include "lrg.h"
#include "crg.h"
#include "rgu.h"
#include "tfu.h"
#include "rg_sch_inf.h"
#include "rg_env.h"
#include "rg.h"
#include "rg_err.h"

#include "lrg.x"
#include "crg.x"
#include "rgu.x"
#include "tfu.x"
#include "rg_sch_inf.x"
#include "rg.x"

#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "lpj.h"           /* LPJ define */
#include "cpj.h"           /* RRC layer */
#include "pju.h"           /* PDCP service user */
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */

#include "lkw.x"           /* LKW */
#include "lpj.x"           /* LM(PDCP) */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "cpj.x"           /* RRC layer */
#include "pju.x"           /* PDCP service user */
#include "kw_err.h"
#include "kw.x"
#include "kw_pj_lib.x"
#include "rgr.h"
#include "rgr.x"

void printMacCellCfg(CrgCellCfg *cellCfg)
{
  MSPD_DBG("\n*******MAC CELL Configuration*******\n");
  MSPD_DBG("cellCfg->bwCfg.dlTotalBw = %d\n", cellCfg->bwCfg.dlTotalBw);
  MSPD_DBG("cellCfg->bwCfg.ulTotalBw = %d\n", cellCfg->bwCfg.ulTotalBw);
  MSPD_DBG("cellCfg->cellId = %d\n", cellCfg->cellId);
  MSPD_DBG("cellCfg->rachCfg.maxMsg3Tx = %d\n", cellCfg->rachCfg.maxMsg3Tx);
  MSPD_DBG("\n");
}

void printMacUeCfg(CrgUeCfg *ueCfg)
{
  MSPD_DBG("\n*******MAC UE Configuration*******\n");
  MSPD_DBG("ueCfg->cellId = %d\n", ueCfg->cellId);
  MSPD_DBG("ueCfg->crnti = %d\n", ueCfg->crnti);
  MSPD_DBG("ueCfg->ueUlHqCfg.maxUlHqTx = %d\n", ueCfg->ueUlHqCfg.maxUlHqTx);
  MSPD_DBG("\n");
}

void printMacLchCfg(CrgLchCfg *lchCfg)
{
  MSPD_DBG("\n*******MAC LCH Configuration*******\n");
  MSPD_DBG("lchCfg->cellId = %d\n", lchCfg->cellId);
  MSPD_DBG("lchCfg->crnti = %d\n", lchCfg->crnti);
  MSPD_DBG("lchCfg->dir = %d\n", lchCfg->dir);
  MSPD_DBG("lchCfg->dlInfo.dlTrchType = %d\n", lchCfg->dlInfo.dlTrchType);
  MSPD_DBG("lchCfg->lcId = %d\n", lchCfg->lcId);
  MSPD_DBG("lchCfg->lcType = %d\n", lchCfg->lcType);
  MSPD_DBG("lchCfg->ulInfo.lcgId = %d\n", lchCfg->ulInfo.lcgId);
  MSPD_DBG("lchCfg->ulInfo.ulTrchType = %d\n", lchCfg->ulInfo.ulTrchType);
  MSPD_DBG("\n");
}

void printCtfCellCfg(CtfCellCfgInfo *cellCfg)
{
  MSPD_DBG("\n*******CTF CELL Configuration*******\n");

  MSPD_DBG("cellCfg->cellId = %d\n", cellCfg->cellId);
  MSPD_DBG("cellCfg->cellIdGrpId = %d\n", cellCfg->cellIdGrpId);

  MSPD_DBG("cellCfg->physCellIdPres  = %d\n", cellCfg->physCellIdPres );
  MSPD_DBG("cellCfg->physCellId = %d\n", cellCfg->physCellId);

  MSPD_DBG("cellCfg->bwCfg.pres = %d\n", cellCfg->bwCfg.pres);
  MSPD_DBG("cellCfg->bwCfg.dlBw = %d\n", cellCfg->bwCfg.dlBw);
  MSPD_DBG("cellCfg->bwCfg.ulBw = %d\n", cellCfg->bwCfg.ulBw);
  MSPD_DBG("cellCfg->bwCfg.eUtraBand = %d\n", cellCfg->bwCfg.eUtraBand);


  MSPD_DBG("cellCfg->txCfg.pres = %d\n", cellCfg->txCfg.pres);
  MSPD_DBG("cellCfg->txCfg.duplexMode = %d\n", cellCfg->txCfg.duplexMode);
  MSPD_DBG("cellCfg->txCfg.scSpacing = %d\n", cellCfg->txCfg.scSpacing);
  MSPD_DBG("cellCfg->txCfg.cycPfx = %d\n", cellCfg->txCfg.cycPfx);

  MSPD_DBG("cellCfg->antennaCfg.antPortsCnt = %d\n", cellCfg->antennaCfg.antPortsCnt);

  MSPD_DBG("cellCfg->prachCfg.pres = %d\n", cellCfg->prachCfg.pres);
  MSPD_DBG("cellCfg->prachCfg.rootSequenceIndex = %d\n", cellCfg->prachCfg.rootSequenceIndex);
  MSPD_DBG("cellCfg->prachCfg.prachCfgIndex = %d\n", cellCfg->prachCfg.prachCfgIndex);
  MSPD_DBG("cellCfg->prachCfg.zeroCorrelationZoneCfg = %d\n", cellCfg->prachCfg.zeroCorrelationZoneCfg);
  MSPD_DBG("cellCfg->prachCfg.highSpeedFlag = %d\n", cellCfg->prachCfg.highSpeedFlag);
  MSPD_DBG("cellCfg->prachCfg.prachFreqOffset = %d\n", cellCfg->prachCfg.prachFreqOffset);


  MSPD_DBG("cellCfg->pdschCfg.pres = %d\n", cellCfg->pdschCfg.pres);
  MSPD_DBG("cellCfg->pdschCfg.refSigPwr = %d\n", cellCfg->pdschCfg.refSigPwr);
  MSPD_DBG("cellCfg->pdschCfg.p_b = %d\n", cellCfg->pdschCfg.p_b);

  MSPD_DBG("cellCfg->puschCfg.pres = %d\n", cellCfg->puschCfg.pres);
  
  MSPD_DBG("cellCfg->puschCfg.puschBasicCfg.noOfsubBands  = %d\n", cellCfg->puschCfg.puschBasicCfg.noOfsubBands);
  MSPD_DBG("cellCfg->puschCfg.puschBasicCfg.hoppingMode   = %d\n", cellCfg->puschCfg.puschBasicCfg.hoppingMode);
  MSPD_DBG("cellCfg->puschCfg.puschBasicCfg.hoppingOffset = %d\n", cellCfg->puschCfg.puschBasicCfg.hoppingOffset);
  MSPD_DBG("cellCfg->puschCfg.puschBasicCfg.enable64QAM   = %d\n", cellCfg->puschCfg.puschBasicCfg.enable64QAM);
  
  MSPD_DBG("cellCfg->puschCfg.puschUlRS = %d\n", cellCfg->puschCfg.puschUlRS);

  MSPD_DBG("cellCfg->phichCfg.pres = %d\n", cellCfg->phichCfg.pres);
  MSPD_DBG("cellCfg->phichCfg.duration = %d\n", cellCfg->phichCfg.duration);
  MSPD_DBG("cellCfg->phichCfg.resource = %d\n", cellCfg->phichCfg.resource);

  MSPD_DBG("cellCfg->pucchCfg.pres = %d\n", cellCfg->pucchCfg.pres);
  MSPD_DBG("cellCfg->pucchCfg.nRB = %d\n", cellCfg->pucchCfg.nRB);
  MSPD_DBG("cellCfg->pucchCfg.nCS = %d\n", cellCfg->pucchCfg.nCS);
  MSPD_DBG("cellCfg->pucchCfg.n1PUCCH = %d\n", cellCfg->pucchCfg.n1PUCCH);
  MSPD_DBG("cellCfg->pucchCfg.deltaShift = %d\n", cellCfg->pucchCfg.deltaShift);

  MSPD_DBG("cellCfg->srsUlCfg.pres = %d\n", cellCfg->srsUlCfg.pres);
  MSPD_DBG("cellCfg->srsUlCfg.srsCfgType = %d\n", cellCfg->srsUlCfg.srsCfgType);
  MSPD_DBG("cellCfg->srsUlCfg.srsSetup.srsBw = %d\n", cellCfg->srsUlCfg.srsSetup.srsBw);
  MSPD_DBG("cellCfg->srsUlCfg.srsSetup.sfCfg = %d\n", cellCfg->srsUlCfg.srsSetup.sfCfg);
  MSPD_DBG("cellCfg->srsUlCfg.srsSetup.srsANSimultTx = %d\n", cellCfg->srsUlCfg.srsSetup.srsANSimultTx);
  MSPD_DBG("cellCfg->srsUlCfg.srsSetup.srsMaxUpPts = %d\n", cellCfg->srsUlCfg.srsSetup.srsMaxUpPts);

  MSPD_DBG("cellCfg->tddSfCfg.pres = %d\n", cellCfg->tddSfCfg.pres);
  MSPD_DBG("cellCfg->tddSfCfg.sfAssignment = %d\n", cellCfg->tddSfCfg.sfAssignment);
  MSPD_DBG("cellCfg->tddSfCfg.spclSfPatterns = %d\n", cellCfg->tddSfCfg.spclSfPatterns);


  MSPD_DBG("cellCfg->syncSigPowOs = %d\n", cellCfg->syncSigPowOs);
  MSPD_DBG("cellCfg->syncSigPowOsPres = %d\n", cellCfg->syncSigPowOsPres);
  MSPD_DBG("cellCfg->cfiPowOs = %d\n", cellCfg->cfiPowOs);
  MSPD_DBG("cellCfg->cfiPowOsPres = %d\n", cellCfg->cfiPowOsPres);
  MSPD_DBG("cellCfg->dciPowOs = %d\n", cellCfg->dciPowOs);
  MSPD_DBG("cellCfg->dciPowOsPres = %d\n", cellCfg->dciPowOsPres);
  MSPD_DBG("cellCfg->extWinMargin = %d\n", cellCfg->extWinMargin);
  MSPD_DBG("cellCfg->extWinMarginPres = %d\n", cellCfg->extWinMarginPres);
  MSPD_DBG("cellCfg->pucchNoiseGamma = %d\n", cellCfg->pucchNoiseGamma);
  MSPD_DBG("cellCfg->pucchNoiseGammaPres = %d\n", cellCfg->pucchNoiseGammaPres);
  MSPD_DBG("cellCfg->prachPkRatio4 = %d\n", cellCfg->prachPkRatio4);
  MSPD_DBG("cellCfg->prachPkRatio4Pres = %d\n", cellCfg->prachPkRatio4Pres);
  MSPD_DBG("cellCfg->prachPkRatio0 = %d\n", cellCfg->prachPkRatio0);
  MSPD_DBG("cellCfg->prachPkRatio0Pres = %d\n", cellCfg->prachPkRatio0Pres);
  MSPD_DBG("cellCfg->srsDopEstFactor = %d\n", cellCfg->srsDopEstFactor);
  MSPD_DBG("cellCfg->srsDopEstFactorPres = %d\n", cellCfg->srsDopEstFactorPres);
  MSPD_DBG("cellCfg->puschProbDtxAck = %d\n", cellCfg->puschProbDtxAck);
  MSPD_DBG("cellCfg->puschProbDtxAckPres = %d\n", cellCfg->puschProbDtxAckPres);
  MSPD_DBG("cellCfg->pucchProbDtxAck = %d\n", cellCfg->pucchProbDtxAck);
  MSPD_DBG("cellCfg->pucchProbDtxAckPres = %d\n", cellCfg->pucchProbDtxAckPres);

  MSPD_DBG("cellCfg->txAntennaPorts  = %d\n", cellCfg->txAntennaPorts );
  MSPD_DBG("cellCfg->txAntennaPortsPres = %d\n", cellCfg->txAntennaPortsPres);
  MSPD_DBG("cellCfg->rxAntennaPorts  = %d\n", cellCfg->rxAntennaPorts );
  MSPD_DBG("cellCfg->rxAntennaPortsPres  = %d\n", cellCfg->rxAntennaPortsPres );

  MSPD_DBG("cellCfg->phySyncMode = %d\n", cellCfg->phySyncMode);
  MSPD_DBG("cellCfg->phySyncModePres = %d\n", cellCfg->phySyncModePres);
  MSPD_DBG("cellCfg->dataRepMode = %d\n", cellCfg->dataRepMode);
  MSPD_DBG("cellCfg->dataRepModePres  = %d\n", cellCfg->dataRepModePres );
  MSPD_DBG("cellCfg->rachSrRepMode = %d\n", cellCfg->rachSrRepMode);
  MSPD_DBG("cellCfg->rachSrRepModePres = %d\n", cellCfg->rachSrRepModePres);
  MSPD_DBG("\n");
}

void printCtfUeCfg(CtfDedCfgInfo  *dedCfg)
{
  MSPD_DBG("\n*******CTF UE Configuration*******\n");

  MSPD_DBG("dedCfg->cellId = %d\n", dedCfg->cellId);
  MSPD_DBG("dedCfg->ueId = %d\n", dedCfg->ueId);

  MSPD_DBG("dedCfg->ueCatCfg.pres = %d\n", dedCfg->ueCatCfg.pres);
  MSPD_DBG("dedCfg->ueCatCfg.ueCategory = %d\n", dedCfg->ueCatCfg.ueCategory);

  MSPD_DBG("dedCfg->pdschCfg.pres = %d\n", dedCfg->pdschCfg.pres);
  MSPD_DBG("dedCfg->pdschCfg.pA = %d\n", dedCfg->pdschCfg.pA);

  MSPD_DBG("dedCfg->pucchCfg.pres = %d\n", dedCfg->pucchCfg.pres);
  MSPD_DBG("dedCfg->pucchCfg.dedPucchCfgType = %d\n", dedCfg->pucchCfg.dedPucchCfgType);
  MSPD_DBG("dedCfg->pucchCfg.pucchSetup.repFact = %d\n", dedCfg->pucchCfg.pucchSetup.repFact);
  MSPD_DBG("dedCfg->pucchCfg.pucchSetup.n1PUCCHRep = %d\n", dedCfg->pucchCfg.pucchSetup.n1PUCCHRep);

  MSPD_DBG("dedCfg->puschCfg.pres = %d\n", dedCfg->puschCfg.pres);
  MSPD_DBG("dedCfg->puschCfg.betaOffsetAckIdx = %d\n", dedCfg->puschCfg.betaOffsetAckIdx);
  MSPD_DBG("dedCfg->puschCfg.betaOffsetRiIdx = %d\n", dedCfg->puschCfg.betaOffsetRiIdx);
  MSPD_DBG("dedCfg->puschCfg.betaOffsetCqiIdx = %d\n", dedCfg->puschCfg.betaOffsetCqiIdx);

#ifndef TFU_UPGRADE
  MSPD_DBG("dedCfg->cqiRptCfg.pres = %d\n", dedCfg->cqiRptCfg.pres);
#ifdef CTF_VER2
  MSPD_DBG("dedCfg->cqiRptCfg.reportMode.aPeriodicRpt.pres = %d\n", dedCfg->cqiRptCfg.reportMode.aPeriodicRpt.pres);
  MSPD_DBG("dedCfg->cqiRptCfg.reportMode.aPeriodicRpt.aPeriodicRptMode = %d\n", dedCfg->cqiRptCfg.reportMode.aPeriodicRpt.aPeriodicRptMode);

#ifdef CTF_VER2
  MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.pres = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.pres);
#endif
  MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiPeriodicCfgType = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiPeriodicCfgType);
  MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPUCCHRsrcIndx = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPUCCHRsrcIndx);
  MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPmiCfgIndx = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPmiCfgIndx);
  MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.formatIndicator = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.formatIndicator);
  MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.subbandCqi.k = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.subbandCqi.k);
  MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.riCfgPres = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.riCfgPres);
  MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.riCfgIndx = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.riCfgIndx);
  MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.simultaneousAckNack = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.simultaneousAckNack);
#ifdef CTF_VER1
  MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiMask = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiMask);
#endif

#else

  MSPD_DBG("dedCfg->cqiRptCfg.reportingMode = %d\n", dedCfg->cqiRptCfg.reportingMode);
  if (CTF_CQI_RPTMODE_APRDIOC == dedCfg->cqiRptCfg.reportingMode)
  {
    MSPD_DBG("dedCfg->cqiRptCfg.reportMode.aPeriodicRpt = %d\n", dedCfg->cqiRptCfg.reportMode.aPeriodicRpt);
  }
  else if (CTF_CQI_RPTMODE_PRDIOC == dedCfg->cqiRptCfg.reportingMode)
  {

    //MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.pres = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.pres);
    MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiPeriodicCfgType = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiPeriodicCfgType);
    MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPUCCHRsrcIndx = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPUCCHRsrcIndx);
    MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPmiCfgIndx = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPmiCfgIndx);
    MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.formatIndicator = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.formatIndicator);
    MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.subbandCqi.k = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.subbandCqi.k);
    MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRspt.cqiSetup.riCfgPres = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.riCfgPres);
    MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.riCfgIndx = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.riCfgIndx);
    MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.simultaneousAckNack = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.simultaneousAckNack);
#ifdef CTF_VER1
    MSPD_DBG("dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiMask = %d\n", dedCfg->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiMask);
#endif
  }
#endif
#ifdef CTF_VER1
  MSPD_DBG("dedCfg->cqiRptCfg.pmiRiRpt = %d\n", dedCfg->cqiRptCfg.pmiRiRpt);
#endif

  MSPD_DBG("dedCfg->srsUlCfg.pres = %d\n", dedCfg->srsUlCfg.pres);
  MSPD_DBG("dedCfg->srsUlCfg.dedSrsUlCfgType = %d\n", dedCfg->srsUlCfg.dedSrsUlCfgType);
  MSPD_DBG("dedCfg->srsUlCfg.dedSrsSetup.srsBw = %d\n", dedCfg->srsUlCfg.dedSrsSetup.srsBw);
  MSPD_DBG("dedCfg->srsUlCfg.dedSrsSetup.srsHopngBw = %d\n", dedCfg->srsUlCfg.dedSrsSetup.srsHopngBw);
  MSPD_DBG("dedCfg->srsUlCfg.dedSrsSetup.freqDmnPos = %d\n", dedCfg->srsUlCfg.dedSrsSetup.freqDmnPos);
  MSPD_DBG("dedCfg->srsUlCfg.dedSrsSetup.duration = %d\n", dedCfg->srsUlCfg.dedSrsSetup.duration);
  MSPD_DBG("dedCfg->srsUlCfg.dedSrsSetup.srsCfgIdx = %d\n", dedCfg->srsUlCfg.dedSrsSetup.srsCfgIdx);
  MSPD_DBG("dedCfg->srsUlCfg.dedSrsSetup.txComb = %d\n", dedCfg->srsUlCfg.dedSrsSetup.txComb);
  MSPD_DBG("dedCfg->srsUlCfg.dedSrsSetup.cyclicShift = %d\n", dedCfg->srsUlCfg.dedSrsSetup.cyclicShift);

  MSPD_DBG("dedCfg->dedSRCfg.pres = %d\n", dedCfg->dedSRCfg.pres);
  MSPD_DBG("dedCfg->dedSRCfg.dedSRCfgType = %d\n", dedCfg->dedSRCfg.dedSRCfgType);
  MSPD_DBG("dedCfg->dedSRCfg.dedSrSetup.srPUCCHRi = %d\n", dedCfg->dedSRCfg.dedSrSetup.srPUCCHRi);
  MSPD_DBG("dedCfg->dedSRCfg.dedSrSetup.srCfgIdx = %d\n", dedCfg->dedSRCfg.dedSrSetup.srCfgIdx);
#endif /*TFU_UPGRADE*/

  MSPD_DBG("dedCfg->antInfo.pres = %d\n", dedCfg->antInfo.pres);
  MSPD_DBG("dedCfg->antInfo.txMode = %d\n", dedCfg->antInfo.txMode);
  MSPD_DBG("dedCfg->antInfo.ueTxAntSelection.cfgType = %d\n", dedCfg->antInfo.ueTxAntSelection.cfgType);
  MSPD_DBG("dedCfg->antInfo.ueTxAntSelection.txAntSelect = %d\n", dedCfg->antInfo.ueTxAntSelection.txAntSelect);

#ifdef CTF_VER2
  MSPD_DBG("dedCfg->antInfo.codeBookSubsetRes = %d\n", dedCfg->antInfo.codeBookSubsetRes);
#endif
  MSPD_DBG("\n");
}

void printSchCellCfg(RgrCellCfg *rgrCellCfg)
{
  int i;
  
  MSPD_DBG("\n*******SCH CELL Configuration*******\n");
  MSPD_DBG("rgrCellCfg->cellId = %d\n", rgrCellCfg->cellId);  
  MSPD_DBG("rgrCellCfg->macInst = %d\n", rgrCellCfg->macInst);
#ifdef RGR_V1
  MSPD_DBG("rgrCellCfg->maxMsg3PerUlSf = %d\n", rgrCellCfg->maxMsg3PerUlSf); 
#endif /* RGR_V1 */
  MSPD_DBG("rgrCellCfg->maxUePerUlSf = %d\n", rgrCellCfg->maxUePerUlSf); 
  MSPD_DBG("rgrCellCfg->maxUePerDlSf = %d\n", rgrCellCfg->maxUePerDlSf); 
  MSPD_DBG("rgrCellCfg->maxUlBwPerUe = %d\n", rgrCellCfg->maxUlBwPerUe);
  MSPD_DBG("rgrCellCfg->maxDlBwPerUe = %d\n", rgrCellCfg->maxDlBwPerUe); 
  MSPD_DBG("rgrCellCfg->maxDlRetxBw = %d\n", rgrCellCfg->maxDlRetxBw);
  MSPD_DBG("rgrCellCfg->maxDlUeNewTxPerTti = %d\n", rgrCellCfg->maxDlUeNewTxPerTti); 
  MSPD_DBG("rgrCellCfg->maxUlUeNewTxPerTti = %d\n", rgrCellCfg->maxUlUeNewTxPerTti); 
  MSPD_DBG("rgrCellCfg->numTxAntPorts = %d\n", rgrCellCfg->numTxAntPorts);  
  MSPD_DBG("rgrCellCfg->isCpUlExtend = %d\n", rgrCellCfg->isCpUlExtend);   
  MSPD_DBG("rgrCellCfg->isCpDlExtend = %d\n", rgrCellCfg->isCpDlExtend);  
  MSPD_DBG("rgrCellCfg->cellModSchm = %d\n", rgrCellCfg->cellModSchm);  
  MSPD_DBG("rgrCellCfg->dlSchdType = %d\n", rgrCellCfg->dlSchdType);  
  MSPD_DBG("rgrCellCfg->pMax = %d\n", rgrCellCfg->pMax);       
  for(i=0; i<RGR_PFS_CQI_PRIO_LVLS; i++)
  {
    MSPD_DBG("rgrCellCfg->dlSchInfo.cellDlPfs.cqiPrios[%d] = %d\n", i, rgrCellCfg->dlSchInfo.cellDlPfs.cqiPrios[i]); 
  }
    for(i=0; i<RGR_PFS_FRAC_PRIO_LVLS; i++)
  {
    MSPD_DBG("rgrCellCfg->dlSchInfo.cellDlPfs.fracPrios[%d] = %d\n", i, rgrCellCfg->dlSchInfo.cellDlPfs.fracPrios[i]);  
  }
    for(i=0; i<RGR_PFS_SVC_PRIO_LVLS; i++)
  {
    MSPD_DBG("rgrCellCfg->dlSchInfo.cellDlPfs.svcPrioPrios[%d] = %d\n", i, rgrCellCfg->dlSchInfo.cellDlPfs.svcPrioPrios[i]);
  }
  MSPD_DBG("rgrCellCfg->ulSchdType = %d\n", rgrCellCfg->ulSchdType); 
  for(i=0; i<RGR_PFS_CQI_PRIO_LVLS; i++)
  {
    MSPD_DBG("rgrCellCfg->ulSchInfo.cellUlPfs.cqiPrios[%d] = %d\n", i, rgrCellCfg->ulSchInfo.cellUlPfs.cqiPrios[i]); 
  }
    for(i=0; i<RGR_PFS_FRAC_PRIO_LVLS; i++)
  {
    MSPD_DBG("rgrCellCfg->ulSchInfo.cellUlPfs.fracPrios[%d] = %d\n", i, rgrCellCfg->ulSchInfo.cellUlPfs.fracPrios[i]);
  }
  MSPD_DBG("rgrCellCfg->ulSchInfo.cellUlPfs.lcgPrios[0] = %d\n", rgrCellCfg->ulSchInfo.cellUlPfs.lcgPrios[0]);
  MSPD_DBG("rgrCellCfg->ulSchInfo.cellUlPfs.lcgPrios[1] = %d\n", rgrCellCfg->ulSchInfo.cellUlPfs.lcgPrios[1]);
  MSPD_DBG("rgrCellCfg->ulSchInfo.cellUlPfs.lcgPrios[2] = %d\n", rgrCellCfg->ulSchInfo.cellUlPfs.lcgPrios[2]);

  MSPD_DBG("rgrCellCfg->dlfsSchdType = %d\n", rgrCellCfg->dlfsSchdType); 
  MSPD_DBG("rgrCellCfg->dlHqCfg.maxDlHqTx = %d\n", rgrCellCfg->dlHqCfg.maxDlHqTx);
  MSPD_DBG("rgrCellCfg->dlHqCfg.maxMsg4HqTx = %d\n", rgrCellCfg->dlHqCfg.maxMsg4HqTx);
  MSPD_DBG("rgrCellCfg->macRnti.startRnti = %d\n", rgrCellCfg->macRnti.startRnti);
  MSPD_DBG("rgrCellCfg->macRnti.size = %d\n", rgrCellCfg->macRnti.size);
  MSPD_DBG("rgrCellCfg->cfiCfg.cfi = %d\n", rgrCellCfg->cfiCfg.cfi);
  MSPD_DBG("rgrCellCfg->trgUlCqi.trgCqi = %d\n", rgrCellCfg->trgUlCqi.trgCqi);
  MSPD_DBG("rgrCellCfg->dlCmnCodeRate.bcchPchRaCodeRate = %d\n", rgrCellCfg->dlCmnCodeRate.bcchPchRaCodeRate); 
  MSPD_DBG("rgrCellCfg->dlCmnCodeRate.pdcchCodeRate = %d\n", rgrCellCfg->dlCmnCodeRate.pdcchCodeRate); 
  MSPD_DBG("rgrCellCfg->dlCmnCodeRate.ccchCqi = %d\n", rgrCellCfg->dlCmnCodeRate.ccchCqi); 
  MSPD_DBG("rgrCellCfg->puschSubBand.subbandStart = %d\n", rgrCellCfg->puschSubBand.subbandStart);  
  MSPD_DBG("rgrCellCfg->puschSubBand.size = %d\n", rgrCellCfg->puschSubBand.size);           
  for(i=0; i< rgrCellCfg->puschSubBand.size; i++)
  {
    MSPD_DBG("rgrCellCfg->puschSubBand.dmrs[%d] = %d\n", i, rgrCellCfg->puschSubBand.dmrs[i]);       
  }
  MSPD_DBG("rgrCellCfg->ulCmnCodeRate.ccchCqi = %d\n", rgrCellCfg->ulCmnCodeRate.ccchCqi);
  MSPD_DBG("rgrCellCfg->dlfsCfg.isDlFreqSel = %d\n", rgrCellCfg->dlfsCfg.isDlFreqSel);   
  MSPD_DBG("rgrCellCfg->dlfsCfg.thresholdCqi = %d\n", rgrCellCfg->dlfsCfg.thresholdCqi); 
  MSPD_DBG("rgrCellCfg->bwCfg.dlTotalBw = %d\n", rgrCellCfg->bwCfg.dlTotalBw);
  MSPD_DBG("rgrCellCfg->bwCfg.ulTotalBw = %d\n", rgrCellCfg->bwCfg.ulTotalBw);
  MSPD_DBG("rgrCellCfg->phichCfg.ngEnum = %d\n", rgrCellCfg->phichCfg.ngEnum);       
  MSPD_DBG("rgrCellCfg->phichCfg.isDurExtend = %d\n", rgrCellCfg->phichCfg.isDurExtend);   

  MSPD_DBG("rgrCellCfg->pucchCfg.resourceSize = %d\n", rgrCellCfg->pucchCfg.resourceSize);   
  MSPD_DBG("rgrCellCfg->pucchCfg.n1PucchAn = %d\n", rgrCellCfg->pucchCfg.n1PucchAn);       
  MSPD_DBG("rgrCellCfg->pucchCfg.deltaShift = %d\n", rgrCellCfg->pucchCfg.deltaShift);       
  MSPD_DBG("rgrCellCfg->pucchCfg.cyclicShift = %d\n", rgrCellCfg->pucchCfg.cyclicShift);       
  MSPD_DBG("rgrCellCfg->srsCfg.srsCfgPrdEnum = %d\n", rgrCellCfg->srsCfg.srsCfgPrdEnum);
  MSPD_DBG("rgrCellCfg->srsCfg.srsBwEnum = %d\n", rgrCellCfg->srsCfg.srsBwEnum);
  MSPD_DBG("rgrCellCfg->srsCfg.srsSubFrameCfg = %d\n", rgrCellCfg->srsCfg.srsSubFrameCfg);

  MSPD_DBG("rgrCellCfg->rachCfg.preambleFormat = %d\n", rgrCellCfg->rachCfg.preambleFormat);
  MSPD_DBG("rgrCellCfg->rachCfg.raWinSize = %d\n", rgrCellCfg->rachCfg.raWinSize);
  MSPD_DBG("rgrCellCfg->rachCfg.raOccasion.size = %d\n", rgrCellCfg->rachCfg.raOccasion.size);
  MSPD_DBG("rgrCellCfg->rachCfg.raOccasion.sfnEnum = %d\n", rgrCellCfg->rachCfg.raOccasion.sfnEnum);

  MSPD_DBG("rgrCellCfg->rachCfg.preambleFormat = %d\n", rgrCellCfg->rachCfg.preambleFormat);  
  MSPD_DBG("rgrCellCfg->rachCfg.raWinSize = %d\n", rgrCellCfg->rachCfg.raWinSize);        
  MSPD_DBG("rgrCellCfg->rachCfg.raOccasion.size = %d\n", rgrCellCfg->rachCfg.raOccasion.size);   
  MSPD_DBG("rgrCellCfg->rachCfg.raOccasion.sfnEnum = %d\n", rgrCellCfg->rachCfg.raOccasion.sfnEnum); 
  for(i = 0; i< rgrCellCfg->rachCfg.raOccasion.size; i++)
  {
    MSPD_DBG("rgrCellCfg->rachCfg.raOccasion.subFrameNum[%d] = %d\n", i, rgrCellCfg->rachCfg.raOccasion.subFrameNum[i]); 
  }
  MSPD_DBG("rgrCellCfg->rachCfg.maxMsg3Tx = %d\n", rgrCellCfg->rachCfg.maxMsg3Tx);       
  MSPD_DBG("rgrCellCfg->rachCfg.numRaPreamble = %d\n", rgrCellCfg->rachCfg.numRaPreamble);    
  MSPD_DBG("rgrCellCfg->rachCfg.sizeRaPreambleGrpA = %d\n", rgrCellCfg->rachCfg.sizeRaPreambleGrpA);
  MSPD_DBG("rgrCellCfg->rachCfg.msgSizeGrpA = %d\n", rgrCellCfg->rachCfg.msgSizeGrpA);        
  MSPD_DBG("rgrCellCfg->rachCfg.prachResource = %d\n", rgrCellCfg->rachCfg.prachResource);       
#ifdef RGR_V1
  MSPD_DBG("rgrCellCfg->rachCfg.contResTmr = %d\n", rgrCellCfg->rachCfg.contResTmr);           
#endif
  MSPD_DBG("rgrCellCfg->siCfg.siWinSize = %d\n", rgrCellCfg->siCfg.siWinSize);
  MSPD_DBG("rgrCellCfg->siCfg.retxCnt = %d\n", rgrCellCfg->siCfg.retxCnt);  
#ifdef RGR_SI_SCH
  MSPD_DBG("rgrCellCfg->siCfg.modPrd = %d\n", rgrCellCfg->siCfg.modPrd);
  MSPD_DBG("rgrCellCfg->siCFg.numSi = %d\n", rgrCellCfg->siCfg.numSi);     
  for(i=0; i<RGR_MAX_NUM_SI; i++)
  {
    MSPD_DBG("rgrCellCfg->siCFg.siPeriodicity[%d] = %d\n", i, rgrCellCfg->siCfg.siPeriodicity[i]); 
  }
#endif/*RGR_SI_SCH*/
  MSPD_DBG("rgrCellCfg->pwrCfg.p0NominalPusch = %d\n", rgrCellCfg->pwrCfg.p0NominalPusch);  
  MSPD_DBG("rgrCellCfg->pwrCfg.alpha = %d\n", rgrCellCfg->pwrCfg.alpha);  
  MSPD_DBG("rgrCellCfg->pwrCfg.p0NominalPucch = %d\n", rgrCellCfg->pwrCfg.p0NominalPucch);
  MSPD_DBG("rgrCellCfg->pwrCfg.deltaPreambleMsg3 = %d\n", rgrCellCfg->pwrCfg.deltaPreambleMsg3);
  MSPD_DBG("rgrCellCfg->pwrCfg.pucchPwrFmt3.startTpcRnti = %d\n", rgrCellCfg->pwrCfg.pucchPwrFmt3.startTpcRnti);
  MSPD_DBG("rgrCellCfg->pwrCfg.pucchPwrFmt3.size = %d\n", rgrCellCfg->pwrCfg.pucchPwrFmt3.size);
  MSPD_DBG("rgrCellCfg->pwrCfg.pucchPwrFmt3a.startTpcRnti = %d\n", rgrCellCfg->pwrCfg.pucchPwrFmt3a.startTpcRnti);
  MSPD_DBG("rgrCellCfg->pwrCfg.pucchPwrFmt3a.size = %d\n", rgrCellCfg->pwrCfg.pucchPwrFmt3a.size);
  MSPD_DBG("rgrCellCfg->pwrCfg.puschPwrFmt3.startTpcRnti = %d\n", rgrCellCfg->pwrCfg.puschPwrFmt3.startTpcRnti);
  MSPD_DBG("rgrCellCfg->pwrCfg.puschPwrFmt3.size = %d\n", rgrCellCfg->pwrCfg.puschPwrFmt3.size);
  MSPD_DBG("rgrCellCfg->pwrCfg.puschPwrFmt3a.startTpcRnti = %d\n", rgrCellCfg->pwrCfg.puschPwrFmt3a.startTpcRnti);
  MSPD_DBG("rgrCellCfg->pwrCfg.puschPwrFmt3a.size = %d\n", rgrCellCfg->pwrCfg.puschPwrFmt3a.size);

  MSPD_DBG("rgrCellCfg->puschCfg.numSubBands = %d\n", rgrCellCfg->puschCfg.numSubBands); 
  MSPD_DBG("rgrCellCfg->puschCfg.isIntraHop = %d\n", rgrCellCfg->puschCfg.isIntraHop); 
  MSPD_DBG("rgrCellCfg->puschCfg.hopOffst = %d\n", rgrCellCfg->puschCfg.hopOffst);
  MSPD_DBG("rgrCellCfg->macPreambleSet.pres = %d\n", rgrCellCfg->macPreambleSet.pres);
  MSPD_DBG("rgrCellCfg->macPreambleSet.start = %d\n", rgrCellCfg->macPreambleSet.start);
  MSPD_DBG("rgrCellCfg->macPreambleSet.size = %d\n", rgrCellCfg->macPreambleSet.size);
  MSPD_DBG("rgrCellCfg->numCmnLcs = %d\n", rgrCellCfg->numCmnLcs);

  for(i=0; i < rgrCellCfg->numCmnLcs ; i++)
  {
    MSPD_DBG("rgrCellCfg->cmnLcCfg[%d].lcId = %d\n", i, rgrCellCfg->cmnLcCfg[i].lcId);    
    MSPD_DBG("rgrCellCfg->cmnLcCfg[%d].lcType = %d\n", i, rgrCellCfg->cmnLcCfg[i].lcType);   
    MSPD_DBG("rgrCellCfg->cmnLcCfg[%d].dir = %d\n", i, rgrCellCfg->cmnLcCfg[i].dir);
    MSPD_DBG("rgrCellCfg->cmnLcCfg[%d].dlTrchType = %d\n", i, rgrCellCfg->cmnLcCfg[i].dlTrchType);
    MSPD_DBG("rgrCellCfg->cmnLcCfg[%d].ulTrchType = %d\n", i, rgrCellCfg->cmnLcCfg[i].ulTrchType); 
  }
#ifdef LTE_TDD
  MSPD_DBG("rgrCellCfg->ulDlCfgIdx = %d\n", rgrCellCfg->ulDlCfgIdx); 
  MSPD_DBG("rgrCellCFg.spclSfCfgIdx = %d\n", rgrCellCFg.spclSfCfgIdx);
  MSPD_DBG("rgrCellCfg->prachRscInfo.numRsc = %d\n", rgrCellCfg->prachRscInfo.numRsc); 
  for(i=0; i<RGR_TDD_MAX_FREQ_RSRC; i++)
  {
    MSPD_DBG("rgrCellCfg->prachRscInfo.prachInfo[%d].freqIdx = %d\n", i, rgrCellCfg->prachRscInfo.prachInfo[i].freqIdx);
    MSPD_DBG("rgrCellCfg->prachRscInfo.prachInfo[%d].sfn = %d\n", i, rgrCellCfg->prachRscInfo.prachInfo[i].sfn);
    MSPD_DBG("rgrCellCfg->prachRscInfo.prachInfo[%d].halfFrm = %d\n", i, rgrCellCfg->prachRscInfo.prachInfo[i].halfFrm);
    MSPD_DBG("rgrCellCfg->prachRscInfo.prachInfo[%d].ulStartSfIdx = %d\n", i, rgrCellCfg->prachRscInfo.prachInfo[i].ulStartSfIdx);
  }
#endif /* LTE_TDD */
#ifdef RGR_RRM_TICK
  MSPD_DBG("rgrCellCfg->rrmTtiIndPrd = %d\n", rgrCellCfg->rrmTtiIndPrd); 
#endif
#ifdef LTEMAC_SPS
  MSPD_DBG("rgrCellCfg->dlSpsCfg.maxSpsDlBw = %d\n", rgrCellCfg->dlSpsCfg.maxSpsDlBw); 
#endif
  MSPD_DBG("\n");
}

void printSchLcgCfg(RgrLcgCfg *lcgCfg)
{
  int i;
  MSPD_DBG("\n*******SCH LCG Configuration*******\n");
  MSPD_DBG("lcgCfg->cellId = %d\n", lcgCfg->cellId);
  MSPD_DBG("lcgCfg->crnti = %d\n", lcgCfg->crnti);
  MSPD_DBG("lcgCfg->ulInfo.lcgId = %d\n", lcgCfg->ulInfo.lcgId);
#ifdef LTE_L2_MEAS
  MSPD_DBG("lcgCfg->ulInfo.numLch = %d\n", lcgCfg->ulInfo.numLch);
  for(i = 0; i < lcgCfg->ulInfo.numLch; i++)
  {
    MSPD_DBG("lcgCfg->ulInfo.lchUlCfg[%d].lcId = %d\n", i, lcgCfg->ulInfo.lchUlCfg[i].lcId);
    MSPD_DBG("lcgCfg->ulInfo.lchUlCfg[%d].qci = %d\n", i, lcgCfg->ulInfo.lchUlCfg[i].qci);
  }
#endif
  MSPD_DBG("\n");
}

void printSchLchCfg(RgrLchCfg *lchCfg)
{
  MSPD_DBG("\n*******SCH LCH Configuration*******\n");
  MSPD_DBG("lchCfg->cellId = %d\n", lchCfg->cellId);
  MSPD_DBG("lchCfg->crnti = %d\n", lchCfg->crnti);
  MSPD_DBG("lchCfg->lcId = %d\n", lchCfg->lcId);
  MSPD_DBG("lchCfg->lcType = %d\n", lchCfg->lcType);
  MSPD_DBG("lchCfg->dlInfo.dlTrchType = %d\n", lchCfg->dlInfo.dlTrchType);
  MSPD_DBG("lchCfg->dlInfo.dlQos.qci = %d\n", lchCfg->dlInfo.dlQos.qci);
  MSPD_DBG("lchCfg->dlInfo.dlQos.gbr = %d\n", lchCfg->dlInfo.dlQos.gbr);
  MSPD_DBG("lchCfg->dlInfo.dlQos.mbr = %d\n", lchCfg->dlInfo.dlQos.mbr);
#ifdef LTEMAC_SPS
  MSPD_DBG("lchCfg->dlInfo.dlSpsCfg.isSpsEnabled = %d\n", lchCfg->dlInfo.dlSpsCfg.isSpsEnabled);
  MSPD_DBG("lchCfg->dlInfo.dlSpsCfg.packetSize = %d\n", lchCfg->dlInfo.dlSpsCfg.packetSize);
  MSPD_DBG("lchCfg->dlInfo.dlSpsCfg.packetPrdcty = %d\n", lchCfg->dlInfo.dlSpsCfg.packetPrdcty);
  MSPD_DBG("lchCfg->dlInfo.dlSpsCfg.explicitRelCnt = %d\n", lchCfg->dlInfo.dlSpsCfg.explicitRelCnt);
#endif
  MSPD_DBG("\n");
}

void printSchUeCfg(RgrUeCfg *rgrUeCfg)
{
  int i;
  
  MSPD_DBG("\n*******SCH UE Configuration*******\n");
  MSPD_DBG("rgrUeCfg->cellId = %d\n", rgrUeCfg->cellId);
  MSPD_DBG("rgrUeCfg->crnti = %d\n", rgrUeCfg->crnti);
  MSPD_DBG("rgrUeCfg->txMode.pres = %d\n", rgrUeCfg->txMode.pres);
  MSPD_DBG("rgrUeCfg->txMode.tmTrnstnState = %d\n", rgrUeCfg->txMode.tmTrnstnState);
  MSPD_DBG("rgrUeCfg->txMode.txModeEnum = %d\n", rgrUeCfg->txMode.txModeEnum);


  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.aprdCqiCfg.pres = %d\n", rgrUeCfg->ueDlCqiCfg.aprdCqiCfg.pres);
  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.aprdCqiCfg.aprdModeEnum = %d\n", rgrUeCfg->ueDlCqiCfg.aprdCqiCfg.aprdModeEnum);
#ifndef TFU_UPGRADE
  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.prdCqiCfg.pres = %d\n", rgrUeCfg->ueDlCqiCfg.prdCqiCfg.pres);
  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.prdCqiCfg.prdModeEnum = %d\n", rgrUeCfg->ueDlCqiCfg.prdCqiCfg.prdModeEnum);
  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.prdCqiCfg.prdicityEnum = %d\n", rgrUeCfg->ueDlCqiCfg.prdCqiCfg.prdicityEnum);
  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.prdCqiCfg.subframeOffst = %d\n", rgrUeCfg->ueDlCqiCfg.prdCqiCfg.subframeOffst);
  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiOffst = %d\n", rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiOffst);
  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.prdCqiCfg.k = %d\n", rgrUeCfg->ueDlCqiCfg.prdCqiCfg.k);
  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiPmiCfgIdx = %d\n", rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiPmiCfgIdx);
#else /* TFU_UPGRADE*/

  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.prdCqiCfg.type = %d\n", rgrUeCfg->ueDlCqiCfg.prdCqiCfg.type);
  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.cqiPResIdx = %d\n", rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.cqiPResIdx);
  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.cqiPCfgIdx = %d\n", rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.cqiPCfgIdx);
  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.cqiRepType = %d\n", rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.cqiRepType);
  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.k = %d\n", rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.k);  
  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.riEna = %d\n", rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.riEna);
  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.riCfgIdx = %d\n", rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.riCfgIdx);
  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.sANCQI = %d\n", rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.sANCQI);
  MSPD_DBG("rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.prdModeEnum = %d\n", rgrUeCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.prdModeEnum);
#endif /*TFU_UPGRADE */

  MSPD_DBG("rgrUeCfg->ueUlHqCfg.maxUlHqTx = %d\n", rgrUeCfg->ueUlHqCfg.maxUlHqTx);
  MSPD_DBG("rgrUeCfg->ueUlHqCfg.deltaHqOffst = %d\n", rgrUeCfg->ueUlHqCfg.deltaHqOffst);
  MSPD_DBG("rgrUeCfg->ueUlPwrCfg.uePuschPwr.pres = %d\n", rgrUeCfg->ueUlPwrCfg.uePuschPwr.pres);
  MSPD_DBG("rgrUeCfg->ueUlPwrCfg.uePuschPwr.tpcRnti = %d\n", rgrUeCfg->ueUlPwrCfg.uePuschPwr.tpcRnti);
  MSPD_DBG("rgrUeCfg->ueUlPwrCfg.uePuschPwr.idx = %d\n", rgrUeCfg->ueUlPwrCfg.uePuschPwr.idx);
  MSPD_DBG("rgrUeCfg->ueUlPwrCfg.uePucchPwr.pres = %d\n", rgrUeCfg->ueUlPwrCfg.uePucchPwr.pres);
  MSPD_DBG("rgrUeCfg->ueUlPwrCfg.uePucchPwr.tpcRnti = %d\n", rgrUeCfg->ueUlPwrCfg.uePucchPwr.tpcRnti);
  MSPD_DBG("rgrUeCfg->ueUlPwrCfg.uePucchPwr.idx = %d\n", rgrUeCfg->ueUlPwrCfg.uePucchPwr.idx);
  MSPD_DBG("rgrUeCfg->ueUlPwrCfg.isAccumulated = %d\n", rgrUeCfg->ueUlPwrCfg.isAccumulated);
  MSPD_DBG("rgrUeCfg->ueUlPwrCfg.isDeltaMCSEnabled = %d\n", rgrUeCfg->ueUlPwrCfg.isDeltaMCSEnabled);
  MSPD_DBG("rgrUeCfg->ueUlPwrCfg.p0UePusch = %d\n", rgrUeCfg->ueUlPwrCfg.p0UePusch);
  MSPD_DBG("rgrUeCfg->ueUlPwrCfg.p0UePucch = %d\n", rgrUeCfg->ueUlPwrCfg.p0UePucch);
  MSPD_DBG("rgrUeCfg->ueUlPwrCfg.pSRSOffset = %d\n", rgrUeCfg->ueUlPwrCfg.pSRSOffset);
  MSPD_DBG("rgrUeCfg->ueUlPwrCfg.trgCqi = %d\n", rgrUeCfg->ueUlPwrCfg.trgCqi);

  MSPD_DBG("rgrUeCfg->ueQosCfg.ambrPres = %d\n", rgrUeCfg->ueQosCfg.ambrPres);
  MSPD_DBG("rgrUeCfg->ueQosCfg.dlAmbr = %d\n", rgrUeCfg->ueQosCfg.dlAmbr);
  MSPD_DBG("rgrUeCfg->ueQosCfg.ueBr = %d\n", rgrUeCfg->ueQosCfg.ueBr);

  MSPD_DBG("rgrUeCfg->ueTaTmrCfg.pres = %d\n", rgrUeCfg->ueTaTmrCfg.pres);
  MSPD_DBG("rgrUeCfg->ueTaTmrCfg.taTmr = %d\n", rgrUeCfg->ueTaTmrCfg.taTmr);
#ifdef RGR_V1
  MSPD_DBG("rgrUeCfg->ueBsrTmrCfg.isPrdBsrTmrPres = %d\n", rgrUeCfg->ueBsrTmrCfg.isPrdBsrTmrPres);
  MSPD_DBG("rgrUeCfg->ueBsrTmrCfg.prdBsrTmr = %d\n", rgrUeCfg->ueBsrTmrCfg.prdBsrTmr);
  MSPD_DBG("rgrUeCfg->ueBsrTmrCfg.retxBsrTmr = %d\n", rgrUeCfg->ueBsrTmrCfg.retxBsrTmr);
#endif
  MSPD_DBG("rgrUeCfg->ueCatEnum = %d\n", rgrUeCfg->ueCatEnum);
#ifdef LTEMAC_DRX
  MSPD_DBG("rgrUeCfg->ueDrxCfg.isDrxEnabled = %d\n", rgrUeCfg->ueDrxCfg.isDrxEnabled);
  if(rgrUeCfg->ueDrxCfg.isDrxEnabled)
  {
#ifdef LTEMAC_R9
    MSPD_DBG("rgrUeCfg->ueDrxCfg.cqiMask.pres = %d\n", rgrUeCfg->ueDrxCfg.cqiMask.pres);
    MSPD_DBG("rgrUeCfg->ueDrxCfg.cqiMask.val = %d\n", rgrUeCfg->ueDrxCfg.cqiMask.val);
#endif
    MSPD_DBG("rgrUeCfg->ueDrxCfg.drxOnDurTmr = %d\n", rgrUeCfg->ueDrxCfg.drxOnDurTmr);
    MSPD_DBG("rgrUeCfg->ueDrxCfg.drxInactvTmr = %d\n", rgrUeCfg->ueDrxCfg.drxInactvTmr);
    MSPD_DBG("rgrUeCfg->ueDrxCfg.drxRetxTmr = %d\n", rgrUeCfg->ueDrxCfg.drxRetxTmr);
    MSPD_DBG("rgrUeCfg->ueDrxCfg.drxLongCycleOffst.longDrxCycle = %d\n", rgrUeCfg->ueDrxCfg.drxLongCycleOffst.longDrxCycle);
    MSPD_DBG("rgrUeCfg->ueDrxCfg.drxLongCycleOffst.drxStartOffst = %d\n", rgrUeCfg->ueDrxCfg.drxLongCycleOffst.drxStartOffst);
    MSPD_DBG("rgrUeCfg->ueDrxCfg.drxShortDrx.pres = %d\n", rgrUeCfg->ueDrxCfg.drxShortDrx.pres);
    MSPD_DBG("rgrUeCfg->ueDrxCfg.drxShortDrx.shortDrxCycle = %d\n", rgrUeCfg->ueDrxCfg.drxShortDrx.shortDrxCycle);
    MSPD_DBG("rgrUeCfg->ueDrxCfg.drxShortDrx.drxShortCycleTmr = %d\n", rgrUeCfg->ueDrxCfg.drxShortDrx.drxShortCycleTmr);
  }
#endif
  MSPD_DBG("rgrUeCfg->isTtiBundlEnabled = %d\n", rgrUeCfg->isTtiBundlEnabled);

  MSPD_DBG("rgrUeCfg->ueAckNackCfg.isAckNackEnabled = %d\n", rgrUeCfg->ueAckNackCfg.isAckNackEnabled);
  if(rgrUeCfg->ueAckNackCfg.isAckNackEnabled)
  {
    MSPD_DBG("rgrUeCfg->ueAckNackCfg.pucchAckNackRep = %d\n", rgrUeCfg->ueAckNackCfg.pucchAckNackRep);
    MSPD_DBG("rgrUeCfg->ueAckNackCfg.ackNackRepFactor = %d\n", rgrUeCfg->ueAckNackCfg.ackNackRepFactor);
  }

  MSPD_DBG("rgrUeCfg->ueMesGapCfg.isMesGapEnabled = %d\n", rgrUeCfg->ueMesGapCfg.isMesGapEnabled);
  if(rgrUeCfg->ueMesGapCfg.isMesGapEnabled)
  {
    MSPD_DBG("rgrUeCfg->ueMesGapCfg.gapPrd = %d\n", rgrUeCfg->ueMesGapCfg.gapPrd);
    MSPD_DBG("rgrUeCfg->ueMesGapCfg.gapOffst = %d\n", rgrUeCfg->ueMesGapCfg.gapOffst);
  }
  MSPD_DBG("rgrUeCfg->ueCapCfg.pwrClass = %d\n", rgrUeCfg->ueCapCfg.pwrClass);
  MSPD_DBG("rgrUeCfg->ueCapCfg.intraSfFeqHop = %d\n", rgrUeCfg->ueCapCfg.intraSfFeqHop);
  MSPD_DBG("rgrUeCfg->ueCapCfg.resAloocType1 = %d\n", rgrUeCfg->ueCapCfg.resAloocType1);
  MSPD_DBG("rgrUeCfg->ueCapCfg.simCqiAckNack = %d\n", rgrUeCfg->ueCapCfg.simCqiAckNack);
#ifdef LTEMAC_MIMO
  MSPD_DBG("rgrUeCfg->ueCapCfg.txAntSel = %d\n", rgrUeCfg->ueCapCfg.txAntSel);
#endif
  MSPD_DBG("rgrUeCfg->ueCodeBookRstCfg.pres = %d\n", rgrUeCfg->ueCodeBookRstCfg.pres);
  if(rgrUeCfg->ueCodeBookRstCfg.pres)
  {
    MSPD_DBG("rgrUeCfg->ueCodeBookRstCfg.pmiBitMap[0] = %d\n", rgrUeCfg->ueCodeBookRstCfg.pmiBitMap[0]);
    MSPD_DBG("rgrUeCfg->ueCodeBookRstCfg.pmiBitMap[1] = %d\n", rgrUeCfg->ueCodeBookRstCfg.pmiBitMap[1]);
  }
  MSPD_DBG("rgrUeCfg->dedPreambleId.pres = %d\n", rgrUeCfg->dedPreambleId.pres);
  if(rgrUeCfg->dedPreambleId.pres)
  {
    MSPD_DBG("rgrUeCfg->dedPreambleId.val = %d\n", rgrUeCfg->dedPreambleId.val);
  }
#ifdef LTE_TDD
  MSPD_DBG("rgrUeCfg->ackNackModeEnum = %d\n", rgrUeCfg->ackNackModeEnum);
#endif
#ifdef LTEMAC_SPS
  MSPD_DBG("rgrUeCfg->ueSpsCfg.spsRnti = %d\n", rgrUeCfg->ueSpsCfg.spsRnti);

  MSPD_DBG("rgrUeCfg->ueSpsCfg.dlSpsCfg.isDlSpsEnabled = %d\n", rgrUeCfg->ueSpsCfg.dlSpsCfg.isDlSpsEnabled);
  if(rgrUeCfg->ueSpsCfg.dlSpsCfg.isDlSpsEnabled)
  {
    MSPD_DBG("rgrUeCfg->ueSpsCfg.dlSpsCfg.numSpsHqProc = %d\n", rgrUeCfg->ueSpsCfg.dlSpsCfg.numSpsHqProc);
    MSPD_DBG("rgrUeCfg->ueSpsCfg.dlSpsCfg.numPucchVal = %d\n", rgrUeCfg->ueSpsCfg.dlSpsCfg.numPucchVal);
    for(i=0; i<rgrUeCfg->ueSpsCfg.dlSpsCfg.numPucchVal i++)
    {
      MSPD_DBG("rgrUeCfg->ueSpsCfg.dlSpsCfg.n1PucchVal[%d] = %d\n", i, rgrUeCfg->ueSpsCfg.dlSpsCfg.n1PucchVal[i]);
    }
    MSPD_DBG("rgrUeCfg->ueSpsCfg.dlSpsCfg.dlSpsPrdctyEnum = %d\n", rgrUeCfg->ueSpsCfg.dlSpsCfg.dlSpsPrdctyEnum);
  }
  MSPD_DBG("rgrUeCfg->ueSpsCfg.ulSpsCfg.isUlSpsEnabled = %d\n", rgrUeCfg->ueSpsCfg.ulSpsCfg.isUlSpsEnabled);
  MSPD_DBG("rgrUeCfg->ueSpsCfg.ulSpsCfg.implicitRelCnt = %d\n", rgrUeCfg->ueSpsCfg.ulSpsCfg.implicitRelCnt);
#ifdef LTE_TDD
  MSPD_DBG("rgrUeCfg->ueSpsCfg.ulSpsCfg.twoIntervalCfg = %d\n", rgrUeCfg->ueSpsCfg.ulSpsCfg.twoIntervalCfg);
#endif
  MSPD_DBG("rgrUeCfg->ueSpsCfg.ulSpsCfg.pwrCfgPres = %d\n", rgrUeCfg->ueSpsCfg.ulSpsCfg.pwrCfgPres);
  if(rgrUeCfg->ueSpsCfg.ulSpsCfg.pwrCfgPres)
  {
    MSPD_DBG("rgrUeCfg->ueSpsCfg.ulSpsCfg.pwrCfg.p0NominalPuschVal = %d\n", rgrUeCfg->ueSpsCfg.ulSpsCfg.pwrCfg.p0NominalPuschVal);
    MSPD_DBG("rgrUeCfg->ueSpsCfg.ulSpsCfg.pwrCfg.p0UePuschVal = %d\n", rgrUeCfg->ueSpsCfg.ulSpsCfg.pwrCfg.p0UePuschVal);
  }
  MSPD_DBG("rgrUeCfg->ueSpsCfg.ulSpsCfg.ulSpsPrdctyEnum = %d\n", rgrUeCfg->ueSpsCfg.ulSpsCfg.ulSpsPrdctyEnum);
  MSPD_DBG("rgrUeCfg->ueSpsCfg.ulSpsCfg.packetSize = %d\n", rgrUeCfg->ueSpsCfg.ulSpsCfg.packetSize);
  MSPD_DBG("rgrUeCfg->ueSpsCfg.ulSpsCfg.lcCnt = %d\n", rgrUeCfg->ueSpsCfg.ulSpsCfg.lcCnt);
  for(i=0; i< rgrUeCfg->ueSpsCfg.ulSpsCfg.lcCnt i++)
  {
    MSPD_DBG("rgrUeCfg->ueSpsCfg.ulSpsCfg.lcId[%d] = %d\n", i, rgrUeCfg->ueSpsCfg.ulSpsCfg.lcId[i]);
  }
#endif
#ifdef TFU_UPGRADE
  MSPD_DBG("rgrUeCfg->srsCfg.type = %d\n", rgrUeCfg->srsCfg.type);
  MSPD_DBG("rgrUeCfg->srsCfg.srsSetup.srsBw = %d\n", rgrUeCfg->srsCfg.srsSetup.srsBw);
  MSPD_DBG("rgrUeCfg->srsCfg.srsSetup.srsCfgIdx = %d\n", rgrUeCfg->srsCfg.srsSetup.srsCfgIdx);
  MSPD_DBG("rgrUeCfg->srsCfg.srsSetup.srsBw = %d\n", rgrUeCfg->srsCfg.srsSetup.srsBw);
  MSPD_DBG("rgrUeCfg->srsCfg.srsSetup.srsHopBw = %d\n", rgrUeCfg->srsCfg.srsSetup.srsHopBw);
  MSPD_DBG("rgrUeCfg->srsCfg.srsSetup.cycShift = %d\n", rgrUeCfg->srsCfg.srsSetup.cycShift);
  MSPD_DBG("rgrUeCfg->srsCfg.srsSetup.duration = %d\n", rgrUeCfg->srsCfg.srsSetup.duration);
  MSPD_DBG("rgrUeCfg->srsCfg.srsSetup.sANSrs = %d\n", rgrUeCfg->srsCfg.srsSetup.sANSrs);
  MSPD_DBG("rgrUeCfg->srsCfg.srsSetup.txComb = %d\n", rgrUeCfg->srsCfg.srsSetup.txComb);
  MSPD_DBG("rgrUeCfg->srsCfg.srsSetup.fDomPosi = %d\n", rgrUeCfg->srsCfg.srsSetup.fDomPosi);

  MSPD_DBG("rgrUeCfg->srCfg.type = %d\n", rgrUeCfg->srCfg.type);
  MSPD_DBG("rgrUeCfg->srCfg.srSetup.srResIdx = %d\n", rgrUeCfg->srCfg.srSetup.srResIdx);
  MSPD_DBG("rgrUeCfg->srCfg.srSetup.srCfgIdx = %d\n", rgrUeCfg->srCfg.srSetup.srCfgIdx);
  MSPD_DBG("rgrUeCfg->srCfg.srSetup.dTMax = %d\n", rgrUeCfg->srCfg.srSetup.dTMax);
#endif
#ifdef LTEMAC_HDFDD
  MSPD_DBG("rgrUeCfg->isHdFddEnbld = %d\n", rgrUeCfg->isHdFddEnbld);
#endif

  MSPD_DBG("rgrUeCfg->puschDedCfg.pres = %d\n", rgrUeCfg->puschDedCfg.pres);
  MSPD_DBG("rgrUeCfg->puschDedCfg.bACKIdx = %d\n", rgrUeCfg->puschDedCfg.bACKIdx);
  MSPD_DBG("rgrUeCfg->puschDedCfg.bRIIdx = %d\n", rgrUeCfg->puschDedCfg.bRIIdx);
  MSPD_DBG("rgrUeCfg->puschDedCfg.bCQIIdx = %d\n", rgrUeCfg->puschDedCfg.bCQIIdx);

  MSPD_DBG("rgrUeCfg->ulTxAntSel.pres = %d\n", rgrUeCfg->ulTxAntSel.pres);
  MSPD_DBG("rgrUeCfg->ulTxAntSel.selType = %d\n", rgrUeCfg->ulTxAntSel.selType);
#ifdef RGR_CQI_REPT
  MSPD_DBG("rgrUeCfg->ueCqiReptCfg.numColltdCqiRept = %d\n", rgrUeCfg->ueCqiReptCfg.numColltdCqiRept);
#endif
  MSPD_DBG("\n");
}

void printRlcCfg(CkwCfgInfo *ckwCfgInfo)
{
  int i, j;
  
  MSPD_DBG("\n******RLC Configuration*******\n");
  MSPD_DBG("ckwCfgInfo->transId = %d\n", ckwCfgInfo->transId);
  MSPD_DBG("ckwCfgInfo->ueId = %d\n", ckwCfgInfo->ueId);
  MSPD_DBG("ckwCfgInfo->cellId = %d\n", ckwCfgInfo->cellId);
  MSPD_DBG("ckwCfgInfo->numEnt = %d\n", ckwCfgInfo->numEnt);

  for(i = 0; i < ckwCfgInfo->numEnt; i++)
  {
    MSPD_DBG("ckwCfgInfo->entCfg[%d].cfgType = %d\n", i, ckwCfgInfo->entCfg[i].cfgType);
    MSPD_DBG("ckwCfgInfo->entCfg[%d].rbId = %d\n", i, ckwCfgInfo->entCfg[i].rbId);
    MSPD_DBG("ckwCfgInfo->entCfg[%d].rbType = %d\n", i, ckwCfgInfo->entCfg[i].rbType);
#ifdef LTE_L2_MEAS
    MSPD_DBG("ckwCfgInfo->entCfg[%d].qci = %d\n", i, ckwCfgInfo->entCfg[i].qci);
#endif
    for(j = 0; j < CM_LTE_MAX_LOGCH_PER_RB; j++)
    {
      MSPD_DBG("ckwCfgInfo->entCfg[%d].lCh[%d].lChId = %d\n", i, j, ckwCfgInfo->entCfg[i].lCh[j].lChId);
      MSPD_DBG("ckwCfgInfo->entCfg[%d].lCh[%d].type = %d\n", i, j, ckwCfgInfo->entCfg[i].lCh[j].type);
    }
    MSPD_DBG("ckwCfgInfo->entCfg[%d].entMode = %d\n", i, ckwCfgInfo->entCfg[i].entMode);
    MSPD_DBG("ckwCfgInfo->entCfg[%d].dir = %d\n", i, ckwCfgInfo->entCfg[i].dir);

    if(CM_LTE_MODE_UM == ckwCfgInfo->entCfg[i].entMode)
    {
      /* UM mode */
      MSPD_DBG("ckwCfgInfo->entCfg[%d].m.umInfo.dl.snLen = %d\n", i, ckwCfgInfo->entCfg[i].m.umInfo.dl.snLen);
      MSPD_DBG("ckwCfgInfo->entCfg[%d].m.umInfo.ul.snLen = %d\n", i, ckwCfgInfo->entCfg[i].m.umInfo.ul.snLen);
      MSPD_DBG("ckwCfgInfo->entCfg[%d].m.umInfo.ul.reOrdTmr = %d\n", i, ckwCfgInfo->entCfg[i].m.umInfo.ul.reOrdTmr);
    }
    else if(CM_LTE_MODE_AM == ckwCfgInfo->entCfg[i].entMode)
    {
      /* AM mode */
      MSPD_DBG("ckwCfgInfo->entCfg[%d].m.amInfo.dl.pollRetxTmr = %d\n", i, ckwCfgInfo->entCfg[i].m.amInfo.dl.pollRetxTmr);
      MSPD_DBG("ckwCfgInfo->entCfg[%d].m.amInfo.dl.pollPdu = %d\n", i, ckwCfgInfo->entCfg[i].m.amInfo.dl.pollPdu);
      MSPD_DBG("ckwCfgInfo->entCfg[%d].m.amInfo.dl.pollByte = %d\n", i, ckwCfgInfo->entCfg[i].m.amInfo.dl.pollByte);
      MSPD_DBG("ckwCfgInfo->entCfg[%d].m.amInfo.dl.maxRetx = %d\n", i, ckwCfgInfo->entCfg[i].m.amInfo.dl.maxRetx);
      MSPD_DBG("ckwCfgInfo->entCfg[%d].m.amInfo.ul.staProhTmr = %d\n", i, ckwCfgInfo->entCfg[i].m.amInfo.ul.staProhTmr);
      MSPD_DBG("ckwCfgInfo->entCfg[%d].m.amInfo.ul.reOrdTmr = %d\n", i, ckwCfgInfo->entCfg[i].m.amInfo.ul.reOrdTmr);
    }
    else if(CM_LTE_MODE_TM == ckwCfgInfo->entCfg[i].entMode)
    {
      /* TM mode */
      MSPD_DBG("ckwCfgInfo->entCfg[%d].m.tmInfo.dl.buffSize = %d\n", i, ckwCfgInfo->entCfg[i].m.tmInfo.dl.buffSize);
    }
  }
  MSPD_DBG("\n");
}

void printPdcpCfg(CpjCfgReqInfo *cfgReqInfo)
{
  int i, j;
  
  MSPD_DBG("\n******PDCP Configuration*******\n");
  MSPD_DBG("cfgReqInfo->ueId = %d\n", cfgReqInfo->ueId);
  MSPD_DBG("cfgReqInfo->cellId = %d\n", cfgReqInfo->cellId);
  MSPD_DBG("cfgReqInfo->transId = %d\n", cfgReqInfo->transId);
  MSPD_DBG("cfgReqInfo->numEnt = %d\n", cfgReqInfo->numEnt);

  for(i = 0; i < cfgReqInfo->numEnt; i++)
  {
    MSPD_DBG("cfgReqInfo->cfgEnt[%d].rbId = %d\n", i, cfgReqInfo->cfgEnt[i].rbId);
    MSPD_DBG("cfgReqInfo->cfgEnt[%d].cfgType = %d\n", i, cfgReqInfo->cfgEnt[i].cfgType);
    MSPD_DBG("cfgReqInfo->cfgEnt[%d].rbType = %d\n", i, cfgReqInfo->cfgEnt[i].rbType);
    if(CPJ_CFG_ADD == cfgReqInfo->cfgEnt[i].cfgType)
    {
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.addCfg.discReqd = %d\n", i, cfgReqInfo->cfgEnt[i].m.addCfg.discReqd);
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.addCfg.cfmReqd = %d\n", i, cfgReqInfo->cfgEnt[i].m.addCfg.cfmReqd);
#ifdef LTE_L2_MEAS
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.addCfg.qci = %d\n", i, cfgReqInfo->cfgEnt[i].m.addCfg.qci);
#endif
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.addCfg.rlcInfo.dir = %d\n", i, cfgReqInfo->cfgEnt[i].m.addCfg.rlcInfo.dir);
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.addCfg.rlcInfo.mode = %d\n", i, cfgReqInfo->cfgEnt[i].m.addCfg.rlcInfo.mode);
      if(cfgReqInfo->cfgEnt[i].m.addCfg.rlcInfo.mode == CM_LTE_MODE_AM)
      {
        MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.addCfg.rlcInfo.m.rlcAm.statusRepReqd = %d\n", i, cfgReqInfo->cfgEnt[i].m.addCfg.rlcInfo.m.rlcAm.statusRepReqd);
      }
      if(cfgReqInfo->cfgEnt[i].m.addCfg.rlcInfo.mode == CM_LTE_MODE_UM)
      {
        MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.addCfg.rlcInfo.m.rlcUm.pdcpSNSize = %d\n", i, cfgReqInfo->cfgEnt[i].m.addCfg.rlcInfo.m.rlcUm.pdcpSNSize);
      }
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.addCfg.discardTmr = %d\n", i, cfgReqInfo->cfgEnt[i].m.addCfg.discardTmr);
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.addCfg.hdrCompCfg.hdrCompUsed = %d\n", i, cfgReqInfo->cfgEnt[i].m.addCfg.hdrCompCfg.hdrCompUsed);
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.addCfg.hdrCompCfg.rohcInfo.maxCid = %d\n", i, cfgReqInfo->cfgEnt[i].m.addCfg.hdrCompCfg.rohcInfo.maxCid);

      for(j = 0; j < CPJ_MAX_ROHC_PROF_LIST; j++)
      {
        MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.addCfg.hdrCompCfg.rohcInfo.profLst[%d] = %d\n", i, j, cfgReqInfo->cfgEnt[i].m.addCfg.hdrCompCfg.rohcInfo.profLst[j]);
      }
    }
    else if(CPJ_CFG_MODIFY == cfgReqInfo->cfgEnt[i].cfgType)
    {
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.modCfg.bitFlag = %d\n", i, cfgReqInfo->cfgEnt[i].m.modCfg.bitFlag);
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.modCfg.discReqd = %d\n", i, cfgReqInfo->cfgEnt[i].m.modCfg.discReqd);
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.modCfg.cfmReqd = %d\n", i, cfgReqInfo->cfgEnt[i].m.modCfg.cfmReqd);
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.modCfg.hdrCompCfg.hdrCompUsed = %d\n", i, cfgReqInfo->cfgEnt[i].m.modCfg.hdrCompCfg.hdrCompUsed);
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.modCfg.hdrCompCfg.rohcInfo.maxCid = %d\n", i, cfgReqInfo->cfgEnt[i].m.modCfg.hdrCompCfg.rohcInfo.maxCid);

      for(j = 0; j < CPJ_MAX_ROHC_PROF_LIST; j++)
      {
        MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.modCfg.hdrCompCfg.rohcInfo.profLst[%d] = %d\n", i, j, cfgReqInfo->cfgEnt[i].m.modCfg.hdrCompCfg.rohcInfo.profLst[j]);
      }

      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.modCfg.hoInfo.isHoPres = %d\n", i, cfgReqInfo->cfgEnt[i].m.modCfg.hoInfo.isHoPres);
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.modCfg.hoInfo.dlCount = %d\n", i, cfgReqInfo->cfgEnt[i].m.modCfg.hoInfo.dlCount);
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.modCfg.hoInfo.ulCount = %d\n", i, cfgReqInfo->cfgEnt[i].m.modCfg.hoInfo.ulCount);
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.modCfg.hoInfo.numBits = %d\n", i, cfgReqInfo->cfgEnt[i].m.modCfg.hoInfo.numBits);
      MSPD_DBG("cfgReqInfo->cfgEnt[%d].m.modCfg.hoInfo.ulBitMap = %d\n", i, cfgReqInfo->cfgEnt[i].m.modCfg.hoInfo.ulBitMap);
    }
  }
  MSPD_DBG("\n");
}
