#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"
#include "ssi.h"           /* System services */
#include "cm_tkns.h"       /* Common Token defines */
#include "cm_lte.h"        /* Common LTE Defines */

#include "nhu.h"           /* NHU Interface defines */
#include "nhu_3gasn.h"     /* 3G RRC UE Capability/Handover Message ASN Definitions */
#include "rgr.h"
#include "lnh.h"           /* LNH Interface includes */
#include "ctf.h"
#include "trace.h"
#include "tr_common.h"

#include "gen.x"
#include "ssi.x" 
#include "cm5.x"           /* Timer services */
#include "cm_lib.x"        /* Common library definitions */
#include "cm_tkns.x"
#include "cm_lte.x"
#include "cm_mblk.x"
#include "rgr.x"
#include "cm_llist.x"      /* Common Link List definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "lnh.x"           /* LNH Interface includes */
#include "ctf.x"
#include "nhu.x"           /* NHU Interface includes */

#define LOG_CtfBwCfgInfo(var, buf, flag)\
{\
   BeginStruct("CtfBwCfgInfo", buf);\
\
   LOG_ELEM("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_ELEM("dlBw", var.dlBw, TR_ELEM_CONT);\
\
   LOG_ELEM("ulBw", var.ulBw, TR_ELEM_CONT);\
\
   LOG_ELEM("eUtraBand", var.eUtraBand, TR_ELEM_LAST);\
\
   EndStruct(buf);\
}

#define LOG_CtfTxSchemeCfg(var, buf, flag)\
{\
   BeginStruct("CtfTxSchemeCfg", buf);\
\
   LOG_ELEM("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_ELEM("duplexMode", var.duplexMode, TR_ELEM_CONT);\
\
   LOG_ELEM("scSpacing", var.scSpacing, TR_ELEM_CONT);\
\
   LOG_ELEM("cycPfx", var.cycPfx, TR_ELEM_CONT);\
\
   EndStruct(buf);\
}

#define LOG_CtfAntennaCfgInfo(var, buf, flag)\
{\
   BeginStruct("CtfAntennaCfgInfo", buf);\
\
   LOG_ELEM("antPortsCnt", var.antPortsCnt, TR_ELEM_CONT);\
\
   EndStruct(buf);\
}

#define LOG_CtfCfgTransId(var, buf, flag)\
{\
   BeginStruct("CtfCfgTransId", buf);\
\
   LOG_ARRAY("trans", var.trans, CTF_CFG_TRANSID_SIZE, TR_ELEM_LAST);\
\
   EndStruct(buf);\
}

#define LOG_CtfPrachCfgInfo(var, buf, flag)\
{\
   BeginStruct("CtfPrachCfgInfo", buf);\
\
   LOG_ELEM("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_ELEM("rootSequenceIndex", var.rootSequenceIndex, TR_ELEM_CONT);\
\
   LOG_ELEM("prachCfgIndex", var.prachCfgIndex, TR_ELEM_CONT);\
\
   LOG_ELEM("zeroCorrelationZoneCfg", var.zeroCorrelationZoneCfg, TR_ELEM_CONT);\
\
   LOG_ELEM("highSpeedFlag", var.highSpeedFlag, TR_ELEM_CONT);\
\
   LOG_ELEM("prachFreqOffset", var.prachFreqOffset, TR_ELEM_LAST);\
\
   EndStruct(buf);\
}

#define LOG_CtfPdschCfgInfo(var, buf, flag)\
{\
   BeginStruct("CtfPdschCfgInfo", buf);\
\
   LOG_ELEM("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_ELEM("refSigPwr", var.refSigPwr, TR_ELEM_CONT);\
\
   LOG_ELEM("p_b", var.p_b, TR_ELEM_LAST);\
\
   EndStruct(buf);\
}

#define LOG_CtfPuschCfgBasic(var, buf, flag)\
{\
   BeginStruct("CtfPuschCfgBasic", buf);\
\
   LOG_ELEM("noOfsubBands", var.noOfsubBands, TR_ELEM_CONT);\
\
   LOG_ELEM("hoppingMode", var.hoppingMode, TR_ELEM_CONT);\
\
   LOG_ELEM("hoppingOffset", var.hoppingOffset, TR_ELEM_CONT);\
\
   LOG_ELEM("enable64QAM", var.enable64QAM, TR_ELEM_LAST);\
\
   EndStruct(buf);\
}

#define LOG_CtfPuschUlRS(var, buf, flag)\
{\
   BeginStruct("CtfPuschUlRS", buf);\
\
   LOG_ELEM("grpHopEnabled", var.grpHopEnabled, TR_ELEM_CONT);\
\
   LOG_ELEM("seqHopEnabled", var.seqHopEnabled, TR_ELEM_CONT);\
\
   LOG_ELEM("grpNum", var.grpNum, TR_ELEM_CONT);\
\
   LOG_ELEM("cycShift", var.cycShift, TR_ELEM_LAST);\
\
   EndStruct(buf);\
}

#define LOG_CtfPuschCfgInfo(var, buf, flag)\
{\
   BeginStruct("CtfPuschCfgInfo", buf);\
\
   LOG_ELEM("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_CtfPuschCfgBasic(var.puschBasicCfg, buf, flag);\
\
   LOG_CtfPuschUlRS(var.puschUlRS, buf, flag);\
\
   EndStruct(buf);\
}

#define LOG_CtfPhichCfgInfo(var, buf, flag)\
{\
   BeginStruct("CtfPhichCfgInfo", buf);\
\
   LOG_ELEM("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_ELEM("duration", var.duration, TR_ELEM_CONT);\
\
   LOG_ELEM("resource", var.resource, TR_ELEM_LAST);\
\
   EndStruct(buf);\
}

#define LOG_CtfPucchCfgInfo(var, buf, flag)\
{\
   BeginStruct("CtfPucchCfgInfo", buf);\
\
   LOG_ELEM("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_ELEM("nRB", var.nRB, TR_ELEM_CONT);\
\
   LOG_ELEM("nCS", var.nCS, TR_ELEM_CONT);\
\
   LOG_ELEM("n1PUCCH", var.n1PUCCH, TR_ELEM_CONT);\
\
   EndStruct(buf);\
}

#define LOG_CtfSrsUlCfgInfo(var, buf, flag)\
{\
   BeginStruct("CtfSrsUlCfgInfo", buf);\
\
   LOG_ELEM("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_ELEM("srsCfgType", var.srsCfgType, TR_ELEM_CONT);\
\
   BeginStruct("srsSetupS", buf);\
\
   LOG_ELEM("srsBw", var.srsSetup.srsBw, TR_ELEM_CONT);\
\
   LOG_ELEM("sfCfg", var.srsSetup.sfCfg, TR_ELEM_CONT);\
\
   LOG_ELEM("srsANSimultTx", var.srsSetup.srsANSimultTx, TR_ELEM_CONT);\
\
   LOG_ELEM("srsMaxUpPts", var.srsSetup.srsMaxUpPts, TR_ELEM_CONT);\
\
   EndStruct(buf);\
\
   EndStruct(buf);\
}

#define LOG_CtfTddSfCfgInfo(var, buf, flag)\
{\
   BeginStruct("CtfTddSfCfgInfo", buf);\
\
   LOG_ELEM("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_ELEM("sfAssignment", var.sfAssignment, TR_ELEM_CONT);\
\
   LOG_ELEM("spclSfPatterns", var.spclSfPatterns, TR_ELEM_LAST);\
\
   EndStruct(buf);\
}

#define LOG_CtfUeCatCfgInfo(var, buf, flag)\
{\
   BeginStruct("CtfTddSfCfgInfo", buf);\
\
   LOG_ELEM("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_CmLteUeCategory(var.ueCategory, buf, TR_ELEM_LAST);\
\
   EndStruct(buf);\
}

#define LOG_CtfDedPdschCfgInfo(var, buf, flag)\
{\
   BeginStruct("CtfDedPdschCfgInfo", buf);\
\
   LOG_ELEM("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_ELEM("pA", var.pA, TR_ELEM_LAST);\
\
   EndStruct(buf);\
}

#define LOG_CtfDedPucchCfgInfo(var, buf, flag)\
{\
   BeginStruct("CtfDedPucchCfgInfo", buf);\
\
   LOG_ELEM("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_ELEM("dedPucchCfgType", var.dedPucchCfgType, TR_ELEM_CONT);\
\
   BeginStruct("pucchSetupS", buf);\
\
   LOG_ELEM("repFact", var.pucchSetup.repFact, TR_ELEM_CONT);\
\
   LOG_ELEM("n1PUCCHRep", var.pucchSetup.n1PUCCHRep, TR_ELEM_CONT);\
\
   EndStruct(buf);\
\
   EndStruct(buf);\
}

#define LOG_CtfDedPuschCfgInfo(var, buf, flag)\
{\
   BeginStruct("CtfDedPuschCfgInfo", buf);\
\
   LOG_ELEM("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_ELEM("betaOffsetAckIdx", var.betaOffsetAckIdx, TR_ELEM_CONT);\
\
   LOG_ELEM("betaOffsetRiIdx", var.betaOffsetRiIdx, TR_ELEM_CONT);\
\
   LOG_ELEM("betaOffsetCqiIdx", var.betaOffsetCqiIdx, TR_ELEM_CONT);\
\
   EndStruct(buf);\
}

#define LOG_CtfCqiRptModeAprd(var, buf, flag)\
{\
   BeginStruct("CtfCqiRptModeAprd", buf);\
\
   LOG_ELEM("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_ELEM("aPeriodicRptMode", var.aPeriodicRptMode, TR_ELEM_CONT);\
\
   EndStruct(buf);\
}

#ifdef CTF_AS_SEC_ENB
#define LOG_CtfHdr(var, buf, flag)\
{\
   BeginStruct("CtfHdr", buf);\
\
   LOG_CmLteCellId(var.cellId, buf, TR_ELEM_CONT);\
\
   LOG_CmLteRnti(var.ueId, buf, TR_ELEM_LAST);\
\
   EndStruct(buf);\
}
#endif 

#define LOG_CtfUeInfo(var, buf, flag)\
{\
  BeginStruct("CtfUeInfo", buf);\
\
  LOG_CmLteCellId(var->cellId, buf, TR_ELEM_CONT);\
\
  LOG_CmLteRnti(var->ueId, buf, TR_ELEM_LAST);\
\
  EndStruct(buf);\
}
 
void printCtfCfgInfo(CtfCfgInfo *ptr, Buff *buf);
void printCtfReCfgInfo(CtfReCfgInfo *ptr, Buff *buf);
void printCtfCellCfgInfo(CtfCellCfgInfo *ptr, Buff *buf);
void printCtfDedCfgInfo(CtfDedCfgInfo *ptr, Buff *buf);
void printCtfCellRecfgInfo(CtfCellRecfgInfo *ptr, Buff *buf);
void printCtfDedRecfgInfo(CtfDedRecfgInfo *ptr, Buff *buf);
#ifndef TFU_UPGRADE
void printCtfCqiReportCfgInfo(CtfCqiReportCfgInfo *ptr, Buff *buf);
void printCtfDedSrsUlCfgInfo(CtfDedSrsUlCfgInfo *ptr, Buff *buf);
void printCtfDedSRCfgInfo(CtfDedSRCfgInfo *ptr, Buff *buf);
#endif 
void printDedAntCfgInfo(CtfDedAntCfgInfo *ptr, Buff *buf);
void printCtfReleaseInfo(CtfReleaseInfo *ptr, Buff *buf);

void printCtfCfgReqInfo(CtfCfgReqInfo *ptr, Buff *buf)
{
   BeginStruct("CtfCfgReqInfo", buf);

   LOG_ELEM("cfgType", ptr->cfgType, TR_ELEM_CONT);

   BeginStruct("u", buf);

   if ( ptr->cfgType == CTF_CONFIG ) 
   {
     printCtfCfgInfo(&ptr->u.cfg, buf);
   }
   else if ( ptr->cfgType == CTF_RECONFIG) 
   {
     printCtfReCfgInfo(&ptr->u.reCfg, buf);
   }
   else if ( ptr->cfgType == CTF_DELETE ) 
   {
     printCtfReleaseInfo(&ptr->u.release, buf);
   }

   EndStruct(buf);

   EndStruct(buf);

   return ;
}

void printCtfCfgInfo(CtfCfgInfo *ptr, Buff *buf)
{
   BeginStruct("CtfCfgInfo", buf);

   LOG_ELEM("cfgElem", ptr->cfgElem, TR_ELEM_CONT);

   BeginStruct("u", buf);

   if (ptr->cfgElem == CTF_CELL_CFG )
   {
     printCtfCellCfgInfo(&ptr->u.cellCfg, buf);
   }
   else if (ptr->cfgElem == CTF_UE_CFG )
   {
     printCtfDedCfgInfo(&ptr->u.dedCfg, buf);
   }

   EndStruct(buf);

   EndStruct(buf);

   return ;
}

void printCtfReCfgInfo(CtfReCfgInfo *ptr, Buff *buf)
{
   BeginStruct("CtfReCfgInfo", buf);

   LOG_ELEM("cfgElem", ptr->cfgElem, TR_ELEM_CONT);

   BeginStruct("u", buf);

   if ( ptr->cfgElem == CTF_CELL_CFG)
   {
      printCtfCellRecfgInfo(&ptr->u.cellRecfg, buf);
   }
   else if (ptr->cfgElem == CTF_UE_CFG)
   {
      printCtfDedRecfgInfo(&ptr->u.dedRecfg, buf);
   }

   EndStruct(buf);

   EndStruct(buf);

   return ;
}

void printCtfCellCfgInfo(CtfCellCfgInfo *ptr, Buff *buf)
{
   BeginStruct("CtfCellCfgInfo", buf);

   LOG_CmLteCellId(ptr->cellId, buf, TR_ELEM_CONT);

   LOG_ELEM("cellIdGrpId", ptr->cellIdGrpId, TR_ELEM_CONT);

   LOG_ELEM("physCellIdPres", ptr->physCellIdPres, TR_ELEM_CONT);

   LOG_ELEM("physCellId", ptr->physCellId, TR_ELEM_CONT);

   LOG_CtfBwCfgInfo(ptr->bwCfg, buf, TR_ELEM_CONT);

   LOG_CtfTxSchemeCfg(ptr->txCfg, buf, TR_ELEM_CONT);

   LOG_CtfAntennaCfgInfo(ptr->antennaCfg, buf, TR_ELEM_CONT);

   LOG_CtfPrachCfgInfo(ptr->prachCfg, buf, TR_ELEM_CONT);

   LOG_CtfPdschCfgInfo(ptr->pdschCfg, buf, TR_ELEM_CONT);

   LOG_CtfPuschCfgInfo(ptr->puschCfg, buf, TR_ELEM_CONT);

   LOG_CtfPhichCfgInfo(ptr->phichCfg, buf, TR_ELEM_CONT);

   LOG_CtfPucchCfgInfo(ptr->pucchCfg, buf, TR_ELEM_CONT);

   LOG_CtfSrsUlCfgInfo(ptr->srsUlCfg, buf, TR_ELEM_CONT);

   LOG_CtfTddSfCfgInfo(ptr->tddSfCfg, buf, TR_ELEM_CONT);

   LOG_ELEM("syncSigPowOs", ptr->syncSigPowOs, TR_ELEM_CONT);

   LOG_ELEM("syncSigPowOsPres", ptr->syncSigPowOsPres, TR_ELEM_CONT);

   LOG_ELEM("cfiPowOs", ptr->cfiPowOs, TR_ELEM_CONT);

   LOG_ELEM("cfiPowOsPres", ptr->cfiPowOsPres, TR_ELEM_CONT);

   LOG_ELEM("dciPowOs", ptr->dciPowOs, TR_ELEM_CONT);

   LOG_ELEM("dciPowOsPres", ptr->dciPowOsPres, TR_ELEM_CONT);

   LOG_ELEM("extWinMargin", ptr->extWinMargin, TR_ELEM_CONT);

   LOG_ELEM("extWinMarginPres", ptr->extWinMarginPres, TR_ELEM_CONT);

   LOG_ELEM("pucchNoiseGamma", ptr->pucchNoiseGamma, TR_ELEM_CONT);

   LOG_ELEM("pucchNoiseGammaPres", ptr->pucchNoiseGammaPres, TR_ELEM_CONT);

   LOG_ELEM("prachPkRatio4", ptr->prachPkRatio4, TR_ELEM_CONT);

   LOG_ELEM("prachPkRatio4Pres", ptr->prachPkRatio4Pres, TR_ELEM_CONT);

   LOG_ELEM("prachPkRatio0", ptr->prachPkRatio0, TR_ELEM_CONT);

   LOG_ELEM("prachPkRatio0Pres", ptr->prachPkRatio0Pres, TR_ELEM_CONT);

   LOG_ELEM("srsDopEstFactor", ptr->srsDopEstFactor, TR_ELEM_CONT);

   LOG_ELEM("srsDopEstFactorPres", ptr->srsDopEstFactorPres, TR_ELEM_CONT);

   LOG_ELEM("puschProbDtxAck", ptr->puschProbDtxAck, TR_ELEM_CONT);

   LOG_ELEM("puschProbDtxAckPres", ptr->puschProbDtxAckPres, TR_ELEM_CONT);

   LOG_ELEM("pucchProbDtxAck", ptr->pucchProbDtxAck, TR_ELEM_CONT);

   LOG_ELEM("pucchProbDtxAckPres", ptr->pucchProbDtxAckPres, TR_ELEM_CONT);

   LOG_ELEM("txAntennaPorts", ptr->txAntennaPorts, TR_ELEM_CONT);

   LOG_ELEM("txAntennaPortsPres", ptr->txAntennaPortsPres, TR_ELEM_CONT);

   LOG_ELEM("rxAntennaPorts", ptr->rxAntennaPorts, TR_ELEM_CONT);

   LOG_ELEM("rxAntennaPortsPres", ptr->rxAntennaPortsPres, TR_ELEM_CONT);

   LOG_ELEM("phySyncMode", ptr->phySyncMode, TR_ELEM_CONT);

   LOG_ELEM("phySyncModePres", ptr->phySyncModePres, TR_ELEM_CONT);

   LOG_ELEM("dataRepMode", ptr->dataRepMode, TR_ELEM_CONT);

   LOG_ELEM("dataRepModePres", ptr->dataRepModePres, TR_ELEM_CONT);

   LOG_ELEM("rachSrRepMode", ptr->rachSrRepMode, TR_ELEM_CONT);

   LOG_ELEM("rachSrRepModePres", ptr->rachSrRepModePres, TR_ELEM_CONT);

   LOG_ELEM("opMode", ptr->opMode, TR_ELEM_CONT);

   LOG_ELEM("counter", ptr->counter, TR_ELEM_CONT);

   LOG_ELEM("period", ptr->period, TR_ELEM_CONT);

   LOG_ELEM("priSigPwr", ptr->priSigPwr, TR_ELEM_CONT);

   LOG_ELEM("secSigPwr", ptr->secSigPwr, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printCtfDedCfgInfo(CtfDedCfgInfo *ptr, Buff *buf)
{
   BeginStruct("CtfDedCfgInfo", buf);

   LOG_CmLteCellId(ptr->cellId, buf, TR_ELEM_CONT);

   LOG_CmLteRnti(ptr->ueId, buf, TR_ELEM_CONT);

   LOG_CtfUeCatCfgInfo(ptr->ueCatCfg, buf, TR_ELEM_CONT);

   LOG_CtfDedPdschCfgInfo(ptr->pdschCfg, buf, TR_ELEM_CONT);

   LOG_CtfDedPucchCfgInfo(ptr->pucchCfg, buf, TR_ELEM_CONT);

   LOG_CtfDedPuschCfgInfo(ptr->puschCfg, buf, TR_ELEM_CONT);

#ifndef TFU_UPGRADE
   printCtfCqiReportCfgInfo(&ptr->cqiRptCfg, buf);

   printCtfDedSrsUlCfgInfo(&ptr->srsUlCfg, buf);

   printCtfDedSRCfgInfo(&ptr->dedSRCfg, buf);
#endif

   printDedAntCfgInfo(&ptr->antInfo, buf);

   EndStruct(buf);

   return ;
}

void printCtfCellRecfgInfo(CtfCellRecfgInfo *ptr, Buff *buf)
{
   BeginStruct("CtfCellRecfgInfo", buf);

   LOG_ELEM("ctfReCfgType", ptr->ctfReCfgType, TR_ELEM_CONT);

   LOG_CmLteCellId(ptr->cellId, buf, TR_ELEM_CONT);

   LOG_ELEM("physCellIdPres", ptr->physCellIdPres, TR_ELEM_CONT);

   LOG_ELEM("physCellId", ptr->physCellId, TR_ELEM_CONT);

   LOG_CtfBwCfgInfo(ptr->bwCfg, buf, TR_ELEM_CONT);

   LOG_CtfTxSchemeCfg(ptr->txCfg, buf, TR_ELEM_CONT);

   LOG_CtfAntennaCfgInfo(ptr->antennaCfg, buf, TR_ELEM_CONT);

   LOG_CmLteTimingInfo(ptr->actvnTime, buf); 

   LOG_CtfPrachCfgInfo(ptr->prachCfg, buf, TR_ELEM_CONT);

   LOG_CtfPdschCfgInfo(ptr->pdschCfg, buf, TR_ELEM_CONT);

   LOG_CtfPuschCfgInfo(ptr->puschCfg, buf, TR_ELEM_CONT);

   LOG_CtfPhichCfgInfo(ptr->phichCfg, buf, TR_ELEM_CONT);

   LOG_CtfPucchCfgInfo(ptr->pucchCfg, buf, TR_ELEM_CONT);

   LOG_CtfSrsUlCfgInfo(ptr->srsUlCfg, buf, TR_ELEM_CONT);

   LOG_CtfTddSfCfgInfo(ptr->tddSfCfg, buf, TR_ELEM_CONT);

   LOG_ELEM("syncSigPowOs", ptr->syncSigPowOs, TR_ELEM_CONT);

   LOG_ELEM("syncSigPowOsPres", ptr->syncSigPowOsPres, TR_ELEM_CONT);

   LOG_ELEM("cfiPowOs", ptr->cfiPowOs, TR_ELEM_CONT);

   LOG_ELEM("cfiPowOsPres", ptr->cfiPowOsPres, TR_ELEM_CONT);

   LOG_ELEM("dciPowOs", ptr->dciPowOs, TR_ELEM_CONT);

   LOG_ELEM("dciPowOsPres", ptr->dciPowOsPres, TR_ELEM_CONT);

   LOG_ELEM("extWinMargin", ptr->extWinMargin, TR_ELEM_CONT);

   LOG_ELEM("extWinMarginPres", ptr->extWinMarginPres, TR_ELEM_CONT);

   LOG_ELEM("pucchNoiseGamma", ptr->pucchNoiseGamma, TR_ELEM_CONT);

   LOG_ELEM("pucchNoiseGammaPres", ptr->pucchNoiseGammaPres, TR_ELEM_CONT);

   LOG_ELEM("prachPkRatio4", ptr->prachPkRatio4, TR_ELEM_CONT);

   LOG_ELEM("prachPkRatio4Pres", ptr->prachPkRatio4Pres, TR_ELEM_CONT);

   LOG_ELEM("prachPkRatio0", ptr->prachPkRatio0, TR_ELEM_CONT);

   LOG_ELEM("prachPkRatio0Pres", ptr->prachPkRatio0Pres, TR_ELEM_CONT);

   LOG_ELEM("srsDopEstFactor", ptr->srsDopEstFactor, TR_ELEM_CONT);

   LOG_ELEM("srsDopEstFactorPres", ptr->srsDopEstFactorPres, TR_ELEM_CONT);

   LOG_ELEM("puschProbDtxAck", ptr->puschProbDtxAck, TR_ELEM_CONT);

   LOG_ELEM("puschProbDtxAckPres", ptr->puschProbDtxAckPres, TR_ELEM_CONT);

   LOG_ELEM("pucchProbDtxAck", ptr->pucchProbDtxAck, TR_ELEM_CONT);

   LOG_ELEM("pucchProbDtxAckPres", ptr->pucchProbDtxAckPres, TR_ELEM_CONT);

   LOG_ELEM("txAntennaPorts", ptr->txAntennaPorts, TR_ELEM_CONT);

   LOG_ELEM("txAntennaPortsPres", ptr->txAntennaPortsPres, TR_ELEM_CONT);

   LOG_ELEM("rxAntennaPorts", ptr->rxAntennaPorts, TR_ELEM_CONT);

   LOG_ELEM("rxAntennaPortsPres", ptr->rxAntennaPortsPres, TR_ELEM_CONT);

   LOG_ELEM("phySyncMode", ptr->phySyncMode, TR_ELEM_CONT);

   LOG_ELEM("phySyncModePres", ptr->phySyncModePres, TR_ELEM_CONT);

   LOG_ELEM("dataRepMode", ptr->dataRepMode, TR_ELEM_CONT);

   LOG_ELEM("dataRepModePres", ptr->dataRepModePres, TR_ELEM_CONT);

   LOG_ELEM("rachSrRepMode", ptr->rachSrRepMode, TR_ELEM_CONT);

   LOG_ELEM("rachSrRepModePres", ptr->rachSrRepModePres, TR_ELEM_CONT);

   EndStruct(buf);

   return ;
}

void printCtfDedRecfgInfo(CtfDedRecfgInfo *ptr, Buff *buf)
{
   BeginStruct("CtfDedRecfgInfo", buf);

   LOG_CmLteCellId(ptr->cellId, buf, TR_ELEM_CONT);

   LOG_CmLteRnti(ptr->ueId, buf, TR_ELEM_CONT);

   LOG_CmLteRnti(ptr->newUeId, buf, TR_ELEM_CONT);

   LOG_CtfUeCatCfgInfo(ptr->ueCatCfg, buf, TR_ELEM_CONT);

   LOG_CtfDedPdschCfgInfo(ptr->pdschCfg, buf, TR_ELEM_CONT);

   LOG_CtfDedPucchCfgInfo(ptr->pucchCfg, buf, TR_ELEM_CONT);

   LOG_CtfDedPuschCfgInfo(ptr->puschCfg, buf, TR_ELEM_CONT);

#ifndef TFU_UPGRADE
   printCtfDedSrsUlCfgInfo(&ptr->srsUlCfg, buf);

   printCtfDedSRCfgInfo(&ptr->dedSRCfg, buf);

   printCtfCqiReportCfgInfo(&ptr->cqiRptCfg, buf);
#endif

   printDedAntCfgInfo(&ptr->antInfo, buf);

   EndStruct(buf);

   return ;
}

#if 0
#ifdef CTF_AS_SEC_ENB   
void printCtfDatReqSdus(CtfDatReqSdus *ptr, Buff *buf)
{
   BeginStruct("CtfDatReqSdus", buf);

   /* CmMemListCp wasn't printing */

   LOG_CtfHdr(ptr->hdr, buf, TR_ELEM_CONT);

   BeginStruct("sdu", buf);

   LOG_ELEM("algoType", ptr->sdu.algoType, TR_ELEM_CONT);

   LOG_ARRAY("SecKey", ptr->sdu.SecKey, CTF_SEC_KEY_LEN, TR_ELEM_CONT);

   LOG_ARRAY("strS", ptr->sdu.strS, CTF_STRING_S_LEN, TR_ELEM_CONT);

   /* DerivedKey is not print */

   LOG_ELEM("SecKeyTyp", ptr->sdu.SecKeyTyp, TR_ELEM_LAST);

   EndStruct(buf);

   EndStruct(buf);

   return ;
}

void printCtfDatRspSdus(CtfDatRspSdus *ptr, Buff *buf)
{
   BeginStruct("CtfDatRspSdus", buf);

   /* CmMemListCp wasn't printing */

   LOG_CtfHdr(ptr->Hdr, buf, TR_ELEM_CONT);

   BeginStruct("sdu", buf);

   LOG_ARRAY("SecKey", ptr->sdu.SecKey, CTF_SEC_KEY_LEN, TR_ELEM_CONT);

   LOG_ARRAY("strS", ptr->sdu.strS, CTF_STRING_S_LEN, TR_ELEM_CONT);

   LOG_ARRAY("DerivedKey", ptr->sdu.DerivedKey, CTF_STRING_S_LEN, TR_ELEM_CONT);

   LOG_ELEM("SecKeyTyp", ptr->sdu.SecKeyTyp, TR_ELEM_LAST);

   EndStruct(buf);

   EndStruct(buf);

   return ;

}
#endif
#endif

#ifndef TFU_UPGRADE
void printCtfCqiRptModePeriodic(CtfCqiRptModePeriodic *ptr, Buff *buf)
{
   BeginStruct("CtfCqiRptModePeriodic", buf);

#ifdef CTF_VER2 
   LOG_ELEM("pres", ptr->pres, TR_ELEM_CONT);
#endif

   LOG_ELEM("cqiPeriodicCfgType", ptr->cqiPeriodicCfgType, TR_ELEM_CONT);

   BeginStruct("cqiSetup", buf);

   LOG_ELEM("cqiPUCCHRsrcIndx", ptr->cqiSetup.cqiPUCCHRsrcIndx, TR_ELEM_CONT);

   LOG_ELEM("cqiPmiCfgIndx", ptr->cqiSetup.cqiPmiCfgIndx, TR_ELEM_CONT);

   LOG_ELEM("formatIndicator", ptr->cqiSetup.formatIndicator, TR_ELEM_CONT);

   BeginStruct("subbandCqiS", buf);

   LOG_ELEM("k", ptr->cqiSetup.subbandCqi.k, TR_ELEM_LAST);

   EndStruct(buf);

   LOG_ELEM("riCfgPres", ptr->cqiSetup.riCfgPres, TR_ELEM_CONT);

   LOG_ELEM("riCfgIndx", ptr->cqiSetup.riCfgIndx, TR_ELEM_CONT);


#ifdef CTF_VER1 
   LOG_ELEM("simultaneousAckNack", ptr->cqiSetup.simultaneousAckNack, TR_ELEM_CONT);

   BeginStruct("CtfCqiMask", buf);

   LOG_ELEM("pres", ptr->cqiSetup.cqiMask.pres, TR_ELEM_LAST);

   LOG_ELEM("cqiMaskSetup", ptr->cqiSetup.cqiMask.cqiMaskSetup, TR_ELEM_LAST);

   EndStruct(buf);
#else 
   LOG_ELEM("simultaneousAckNack", ptr->cqiSetup.simultaneousAckNack, TR_ELEM_LAST);

#endif

   return ;
}

void printCtfCqiReportCfgInfo(CtfCqiReportCfgInfo *ptr, Buff *buf)
{
   BeginStruct("CtfCqiReportCfgInfo", buf);

   LOG_ELEM("pres", ptr->pres, TR_ELEM_CONT);

#ifdef CTF_VER2
   BeginStruct("reportMode", buf);

   LOG_CtfCqiRptModeAprd(ptr->reportMode.aPeriodicRpt, buf, TR_ELEM_CONT);

   printCtfCqiRptModePeriodic(&ptr->reportMode.periodicRpt, buf);

   EndStruct(buf);
#else 
   LOG_ELEM("reportingMode", ptr->reportingMode, TR_ELEM_CONT);

   BeginStruct("reportMode", buf);

   if (ptr->reportingMode == CTF_CQI_RPTMODE_APRDIOC)
   {
      LOG_ELEM("aPeriodicRpt", ptr->reportMode.aPeriodicRpt, TR_ELEM_LAST);
   }
   else if (ptr->reportingMode == CTF_CQI_RPTMODE_PRDIOC)
   {
      printCtfCqiRptModePeriodic(&ptr->reportMode.periodicRpt, buf);
   }

   EndStruct(buf);
#endif 

#ifdef CTF_VER1 
   BeginStruct("CtfPmiRiRpt", buf);

   LOG_ELEM("pres", ptr->pmiRiRpt.pres, TR_ELEM_CONT);

   LOG_ELEM("pmiRiRptSetup", ptr->pmiRiRpt.pmiRiRptSetup, TR_ELEM_LAST);

   EndStruct(buf);
#endif 

   return ;
}

void printCtfDedSrsUlCfgInfo(CtfDedSrsUlCfgInfo *ptr, Buff *buf) 
{
   BeginStruct("CtfDedSrsUlCfgInfo", buf);

   LOG_ELEM("pres", ptr->pres, TR_ELEM_CONT);

   LOG_ELEM("dedSrsUlCfgType", ptr->dedSrsUlCfgType, TR_ELEM_CONT);

   BeginStruct("dedSrsSetup", buf);

   LOG_ELEM("srsBw", ptr->dedSrsSetup.srsBw, TR_ELEM_CONT);

   LOG_ELEM("srsHopngBw", ptr->dedSrsSetup.srsHopngBw, TR_ELEM_CONT);

   LOG_ELEM("freqDmnPos", ptr->dedSrsSetup.freqDmnPos, TR_ELEM_CONT);

   LOG_ELEM("duration", ptr->dedSrsSetup.duration, TR_ELEM_CONT);

   LOG_ELEM("srsCfgIdx", ptr->dedSrsSetup.srsCfgIdx, TR_ELEM_CONT);

   LOG_ELEM("txComb", ptr->dedSrsSetup.txComb, TR_ELEM_CONT);

   LOG_ELEM("cyclicShift", ptr->dedSrsSetup.cyclicShift, TR_ELEM_LAST);

   EndStruct(buf);

   EndStruct(buf);

   return ;
}

void printCtfDedSRCfgInfo(CtfDedSRCfgInfo *ptr, Buff *buf)
{
   BeginStruct("CtfDedSRCfgInfo", buf);

   LOG_ELEM("pres", ptr->pres, TR_ELEM_CONT);

   LOG_ELEM("dedSRCfgType", ptr->dedSRCfgType, TR_ELEM_CONT);

   BeginStruct("dedSrSetupS", buf);

   LOG_ELEM("srPUCCHRi", ptr->dedSrSetup.srPUCCHRi, TR_ELEM_CONT);

   LOG_ELEM("srCfgIdx", ptr->dedSrSetup.srCfgIdx, TR_ELEM_LAST);

   EndStruct(buf);

   EndStruct(buf);
   
   return ;
}
#endif 

void printDedAntCfgInfo(CtfDedAntCfgInfo *ptr, Buff *buf)
{
   BeginStruct("CtfDedAntCfgInfo", buf);

   LOG_ELEM("pres", ptr->pres, TR_ELEM_CONT);

   LOG_ELEM("txMode", ptr->txMode, TR_ELEM_CONT);

   BeginStruct("ueTxAntSelection", buf);

   LOG_ELEM("cfgType", ptr->ueTxAntSelection.cfgType, TR_ELEM_CONT);

   LOG_ELEM("txAntSelect", ptr->ueTxAntSelection.txAntSelect, TR_ELEM_LAST);

   EndStruct(buf);

#ifdef CTF_VER2 

   BeginStruct("CtfCdBkSubsetRestnType", buf);

   LOG_ELEM("ctfCdBkRestType", ptr->codeBookSubsetRes.ctfCdBkRestType, TR_ELEM_CONT);

   BeginStruct("codeBookSubsetRestn", buf);

   switch( ptr->codeBookSubsetRes.ctfCdBkRestType)
   {
      case N2TX_ANT_TM3:
        LOG_ELEM("n2TxAntTm3", ptr->codeBookSubsetRes.codeBookSubsetRestn.n2TxAntTm3, TR_ELEM_CONT);
        break;

      case N4TX_ANT_TM3:
        LOG_ELEM("n4TxAntTm3", ptr->codeBookSubsetRes.codeBookSubsetRestn.n4TxAntTm3, TR_ELEM_CONT);
       break;

      case N2TX_ANT_TM4:
        LOG_ELEM("n2TxAntTm4", ptr->codeBookSubsetRes.codeBookSubsetRestn.n2TxAntTm4, TR_ELEM_CONT);
       break;

      case N4TX_ANT_TM4:
        LOG_ARRAY("n4TxAntTm4", ptr->codeBookSubsetRes.codeBookSubsetRestn.n4TxAntTm4, 8, 
		   TR_ELEM_CONT);
       break;

      case N2TX_ANT_TM5:
        LOG_ELEM("n2TxAntTm5", ptr->codeBookSubsetRes.codeBookSubsetRestn.n2TxAntTm5, TR_ELEM_CONT);
       break;

      case N4TX_ANT_TM5:
        LOG_ELEM("n4TxAntTm5", ptr->codeBookSubsetRes.codeBookSubsetRestn.n4TxAntTm5, TR_ELEM_CONT);
       break;

      case N2TX_ANT_TM6:
        LOG_ELEM("n2TxAntTm6", ptr->codeBookSubsetRes.codeBookSubsetRestn.n2TxAntTm6, TR_ELEM_CONT);
       break;

      case N4TX_ANT_TM6:
        LOG_ELEM("n4TxAntTm6", ptr->codeBookSubsetRes.codeBookSubsetRestn.n4TxAntTm6, TR_ELEM_CONT);
       break;

     default :
       break;
   }

   EndStruct(buf);

   EndStruct(buf);

#endif 

   EndStruct(buf);
   
   return ;
}

void printCtfReleaseInfo(CtfReleaseInfo *ptr, Buff *buf)
{
   BeginStruct("CtfReleaseInfo", buf);

   LOG_ELEM("cfgElem", ptr->cfgElem, TR_ELEM_CONT);

   BeginStruct("u", buf);

   if (ptr->cfgElem == CTF_CELL_CFG) 
   {
      BeginStruct("CtfCellReleaseInfo", buf);

      LOG_ELEM("cellRel", ptr->u.cellRel.cellId, TR_ELEM_LAST);

      EndStruct(buf);
   }
   else if (ptr->cfgElem == CTF_UE_CFG) 
   {
      BeginStruct("CtfDedReleaseInfo", buf);

      LOG_ELEM("cellId", ptr->u.dedRel.cellId, TR_ELEM_CONT);

      LOG_ELEM("ueId", ptr->u.dedRel.ueId, TR_ELEM_LAST);

      EndStruct(buf);
   }

   EndStruct(buf);

   EndStruct(buf);

   return ;
}

void printCtfCfgCfm(CtfCfgTransId transId, U8 status, Buff *buf)
{
   LOG_CtfCfgTransId(transId, buf, TR_ELEM_CONT);

   LOG_ELEM("status", status, TR_ELEM_LAST);

   return ;
}

void printCtfUeIdChgReq(CtfCfgTransId transId, CtfUeInfo *ueInfo, CtfUeInfo *newUeInfo, Buff *buf)
{

   LOG_CtfCfgTransId(transId, buf, TR_ELEM_CONT);

   LOG_CtfUeInfo(ueInfo, buf, flag);

   LOG_CtfUeInfo(newUeInfo, buf, flag);

   return ;
}

void printCtfUeIdChgCfm(CtfCfgTransId transId, CtfUeInfo *ueInfo, CmStatus status, Buff *buf)
{
   LOG_CtfCfgTransId(transId, buf, TR_ELEM_CONT);

   LOG_CtfUeInfo(ueInfo, buf, flag);
   
   BeginStruct("CmStatus", buf);

   LOG_ELEM("status", status.status, TR_ELEM_CONT);

   LOG_ELEM("status", status.reason, TR_ELEM_CONT);

   EndStruct(buf);

   return ;
}

