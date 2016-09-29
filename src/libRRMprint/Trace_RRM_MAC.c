
//#include "envopt.h"        /* Environment options */
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
#include "nhu.x"           /* NHU Interface includes */

#define LOG_RGRUlPwrCfg(var, buf, flag) \
{\
   BeginStruct("RgrTpcRntiCfg", buf);\
\
   LOG_CmLteCellId(var.startTpcRnti, buf, TR_ELEM_CONT);\
\
   LOG_ELEM("Size", \
                  var.size, \
                   flag);\
\
   EndStruct(buf);\
}

#define LOG_RgrUeAprdDlCqiCfg(var, buf)\
{\
   LOG_ELEM ("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_ELEM ("aprdModeEnum", var.aprdModeEnum, TR_ELEM_LAST);\
}

#ifndef TFU_UPGRADE
#define LOG_RgrUePrdDlCqiCfg(var, buf)\
{\
   LOG_ELEM ("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_ELEM ("prdModeEnum", var.prdModeEnum, TR_ELEM_CONT);\
\
   LOG_ELEM ("prdicityEnum", var.prdicityEnum, TR_ELEM_CONT);\
\
   LOG_ELEM ("subframeOffst", var.subframeOffst, TR_ELEM_CONT);\
\
   LOG_ELEM ("cqiOffst", var.cqiOffst, TR_ELEM_CONT);\
\
   LOG_ELEM ("k", var.k, TR_ELEM_CONT);\
\
   LOG_ELEM ("cqiPmiCfgIdx", var.cqiPmiCfgIdx, TR_ELEM_CONT);\
}
#else 
#define LOG_RgrUePrdDlCqiCfg(var, buf)\
{\
   LOG_ELEM ("type", var.type, TR_ELEM_CONT);\
\
   BeginStruct("RgrUeDlPCqiSetup", buf);\
\
   LOG_ELEM ("cqiPResIdx", var.cqiSetup.cqiPResIdx, TR_ELEM_CONT);\
\
   LOG_ELEM ("cqiPCfgIdx", var.cqiSetup.cqiPCfgIdx, TR_ELEM_CONT);\
\
   LOG_ELEM ("cqiRepType", var.cqiSetup.cqiRepType, TR_ELEM_CONT);\
\
   LOG_ELEM ("k", var.cqiSetup.k, TR_ELEM_CONT);\
\
   LOG_ELEM ("riEna", var.cqiSetup.riEna, TR_ELEM_CONT);\
\
   LOG_ELEM ("riCfgIdx", var.cqiSetup.riCfgIdx, TR_ELEM_CONT);\
\
   LOG_ELEM ("sANCQI", var.cqiSetup.sANCQI, TR_ELEM_CONT);\
\
   LOG_ELEM ("prdModeEnum", var.cqiSetup.prdModeEnum, TR_ELEM_CONT);\
\
   EndStruct(buf);\
}
#endif 

#define LOG_RgrUeGrpPwrCfg(var, buf)\
{\
   LOG_ELEM ("pres", var.pres, TR_ELEM_CONT);\
\
   LOG_CmLteRnti(var.tpcRnti, buf, TR_ELEM_CONT);\
\
   LOG_ELEM("idx", var.idx, TR_ELEM_LAST);\
}


#define LOG_RgrUeTaTmrCfg(var, buf, flag)\
{\
   LOG_ELEM("pres", var.pres, TR_ELEM_CONT);\
\
  LOG_ELEM("taTmr", var.taTmr, TR_ELEM_LAST);\
}

#ifdef RGR_V1 
#define LOG_RgrUeBsrTmrCfg(var, buf)\
{\
   LOG_ELEM("isPrdBsrTmrPres", var.isPrdBsrTmrPres, TR_ELEM_CONT);\
\
   LOG_ELEM("prdBsrTmr", var.prdBsrTmr, TR_ELEM_CONT);\
\
   LOG_ELEM("retxBsrTmr", var.retxBsrTmr, TR_ELEM_LAST);\
}        
#endif

#ifdef RGR_CQI_REPT
#define LOG_RgrUeCqiReptCfg(var, buf, flag)\
{\
   BeginStruct("RgrUeCqiReptCfg", buf);\
\
   LOG_ELEM("numColltdCqiRept", var.numColltdCqiRept, flag);\
\
   EndStruct(buf);\
}
#endif

void printRgrCfg(RgrCfg *ptr, Buff *buf);
void printRgrCellCfg(RgrCellCfg *ptr, Buff *buf);
void printRgrDlHqCfg(RgrDlHqCfg *ptr, Buff *buf);
void printRgrCfiCfg(RgrCfiCfg *ptr, Buff *buf);
void printRgrUlTrgCqiCfg(RgrUlTrgCqiCfg *ptr, Buff *buf);
void printRgrReCfg(RgrRecfg *ptr, Buff *buf);
void printRgrDel(RgrDel *ptr, Buff *buf);
void printRgrRst(RgrRst *ptr, Buff *buf);
void printRgrLcgCfg(RgrLcgCfg *ptr, Buff *buf);
void printRgrLchCfg(RgrLchCfg *ptr, Buff *buf);
void printRgrLchQosCfg(RgrLchQosCfg *ptr, Buff *buf);
void printRgrUeRecfg(RgrUeRecfg *ptr, Buff *buf);
void printLchRecfg(RgrLchRecfg *ptr, Buff *buf);
void printRgrRntiCfg(RgrRntiCfg *ptr, Buff *buf);
void printLcgRecfg(RgrLcgRecfg *ptr, Buff *buf);
void printRgrUeCfg(RgrUeCfg *ptr, Buff *buf);
void printRgrDlCmnCodeRateCfg(RgrDlCmnCodeRateCfg *ptr, Buff *buf);
void printRgrPreambleSetCfg(RgrPreambleSetCfg *ptr, Buff *buf);
void printRgrPuschCfg(RgrPuschCfg *ptr, Buff *buf);
void printRgrUlPwrCfg(RgrUlPwrCfg *ptr, Buff *buf);
void printRgrSrsCfg(RgrSrsCfg *ptr, Buff *buf);
void printRgrSiCfg(RgrSiCfg *ptr, Buff *buf);
void printRgrRachCfg(RgrRachCfg *ptr, Buff *buf);
void printRgrUlCmnCodeRateCfg(RgrUlCmnCodeRateCfg *ptr, Buff *buf);
void printRgrPuschSubBandCfg(RgrPuschSubBandCfg *ptr, Buff *buf);
void printRgrPucchCfg(RgrPucchCfg *ptr, Buff *buf);
void printRgrBwCfg(RgrBwCfg *ptr, Buff *buf);
void printRgrCodeBookRstCfg(RgrCodeBookRstCfg *ptr, Buff *buf);
void printRgrPhichCfg(RgrPhichCfg *ptr, Buff *buf);
void printRgrDlfsCfg(RgrDlfsCfg *ptr, Buff *buf);
void printRgrUeTxAntSelCfg(RgrUeTxAntSelCfg *ptr, Buff *buf);
void printRgrCellRecfg(RgrCellRecfg *ptr, Buff *buf);
void printRgrUeDlCqiCfg(RgrUeDlCqiCfg *ptr, Buff *buf);
void printRgrUePuschDedCfg(RgrUePuschDedCfg *ptr, Buff *buf);
void printRgrUeQosCfg(RgrUeQosCfg *ptr, Buff *buf);
void printRgrTxModeCfg(RgrUeTxModeCfg *ptr, Buff *buf);
void printRgrUeUlHqCfg(RgrUeUlHqCfg *ptr, Buff *buf);
#ifdef TFU_UPGRADE
void printRgrUePdschDedCfg(RgrUePdschDedCfg *ptr, Buff *buf);
void printRgrUeSrCfg(RgrUeSrCfg *ptr, Buff *buf);
void printRgrUeUlSrsCfg(RgrUeUlSrsCfg *ptr, Buff *buf);
#endif
void printRgrUeAckNackRepCfg(RgrUeAckNackRepCfg *ptr, Buff *buf);
void printRgrUeUlPwrCfg(RgrUeUlPwrCfg *ptr, Buff *buf);
void printRgrUeMeasGapCfg(RgrUeMeasGapCfg *ptr, Buff *buf);
void printRgrUeCapCfg(RgrUeCapCfg *ptr, Buff *buf);
#ifdef RGR_SI_SCH
void printRgrSiCfgReqInfo(RgrSiCfgReqInfo *ptr, Buff *buf);
#endif

/* 
 * Func : printRgrCfgReq
 *
 * Desc : Displays the configuration request details shared between RRM and
 * MAC.
 *
 * Ret  : void
 *
 */
void printRgrCfgReq(RgrCfgReqInfo *ptr, Buff *buf)
{
   BeginStruct("RgrCfgReqInfo", buf);
   LOG_ELEM("action",
                   ptr->action, 
                   TR_ELEM_CONT);

   BeginStruct("u", buf);

   if(ptr->action == RGR_CONFIG)
   {
     printRgrCfg(&ptr->u.cfgInfo, buf);
   }

   else if(ptr->action == RGR_RECONFIG)
   {
     printRgrReCfg(&ptr->u.recfgInfo, buf);
   }

   else if(ptr->action == RGR_DELETE)
   {
     printRgrDel(&ptr->u.delInfo, buf);
   }

   else if(ptr->action == RGR_RESET)
   {
     printRgrRst(&ptr->u.rstInfo, buf);
   }

   EndStruct(buf);

   EndStruct(buf);
   
   return ;
}

/* 
 * Func : printRgrCfgCfm 
 *
 * Desc : Displays the configuration confirmation details shared between RRM and
 * MAC.
 *
 * Ret  : void
 *
 */
void printRgrCfgCfm(RgrCfgTransId transId, U8 status, Buff *buf)
{
   BeginStruct("RgrCfgTransId", buf);
      
   LOG_ARRAY("trans", transId.trans, RGR_CFG_TRANSID_SIZE, TR_ELEM_LAST);

   EndStruct(buf);

   LOG_ELEM("status", status, TR_ELEM_LAST);

   return ;
}

#ifdef RGR_SI_SCH
/* 
 * Func : printRgrSiCfgReqInfo
 *
 * Desc : Displays the SI configuration request details shared between RRM and
 * MAC.
 *
 * Ret  : void
 *
 */
void printRgrSiCfgReqInfo(RgrSiCfgReqInfo *ptr, Buff *buf)
{
   BeginStruct("RgrSiCfgReqInfo", buf);

   LOG_CmLteCellId(ptr->cellId, buf, TR_ELEM_CONT);

   LOG_ELEM("cfgType", ptr->cfgType, TR_ELEM_CONT);

   LOG_ELEM("siId", ptr->siId, TR_ELEM_CONT);

   /* pdu not printed */

   EndStruct(buf);

   return ;
}

/* 
 * Func : printRgrSiCfgCfm
 *
 * Desc : Displays the SI configuration confirm details shared between RRM and
 * MAC.
 *
 * Ret  : void
 *
 */
void printRgrSiCfgCfm(RgrCfgTransId transId, U8 status, Buff *buf)
{
   BeginStruct("RgrCfgTransId", buf);
      
   LOG_ARRAY("trans", transId.trans, RGR_CFG_TRANSID_SIZE, TR_ELEM_LAST);

   EndStruct(buf);

   LOG_ELEM("status", status, TR_ELEM_CONT);

   return ;
}
#endif

#ifdef RGR_CQI_REPT
/* 
 * Func : printRgrStaIndInfo
 *
 * Desc : Displays the status indication details shared between RRM and
 * MAC.
 *
 * Ret  : void
 *
 */
void printRgrStaIndInfo(RgrStaIndInfo *ptr, Buff *buf)
{
   int i = 0; 

   BeginStruct("RgrStaIndInfo", buf);

   LOG_CmLteCellId(ptr->cellId, buf, TR_ELEM_CONT);

   LOG_CmLteRnti(ptr->crnti, buf, TR_ELEM_CONT);

   BeginStruct("RgrUeCqiInfo", buf);

   while ( i < RGR_CQIRPTS_MAXN)
   {
      LOG_ARRAY("cqi", ptr->ueCqiInfo.cqiRept[i].cqi, 2, TR_ELEM_CONT);

      LOG_ELEM("cqiMode", ptr->ueCqiInfo.cqiRept[i].cqiMode, TR_ELEM_CONT);

      i++;
   }

   LOG_ELEM("numCqiRept", ptr->ueCqiInfo.numCqiRept, TR_ELEM_LAST);

   EndStruct(buf);

   EndStruct(buf);
   
   return ;
}
#endif

void printRgrCfg(RgrCfg *ptr, Buff *buf)
{
   BeginStruct("RgrCfg", buf);

   LOG_ELEM("cfgType", 
                   ptr->cfgType, 
                   TR_ELEM_CONT);

   BeginStruct("u", buf);

   if(ptr->cfgType == RGR_CELL_CFG)
   {
      printRgrCellCfg(&ptr->u.cellCfg, buf);
   }

   else if(ptr->cfgType == RGR_UE_CFG)
   {
      printRgrUeCfg(&ptr->u.ueCfg, buf);
   }

   else if(ptr->cfgType == RGR_LCH_CFG)
   {
      printRgrLchCfg(&ptr->u.lchCfg, buf);
   }

   else if(ptr->cfgType == RGR_LCG_CFG)
   {
      printRgrLcgCfg(&ptr->u.lcgCfg, buf);
   }

   EndStruct(buf);

   EndStruct(buf);

   return ;
}

void printRgrReCfg(RgrRecfg *ptr, Buff *buf)
{
   BeginStruct("RgrRecfg", buf);

   LOG_ELEM("recfgType", 
                   ptr->recfgType, 
                   TR_ELEM_CONT);

   if ( ptr->recfgType == RGR_CELL_CFG)
   {
     printRgrCellRecfg(&ptr->u.cellRecfg, buf);
   }

   else if ( ptr->recfgType == RGR_UE_CFG)
   {
     printRgrUeRecfg(&ptr->u.ueRecfg, buf);
   }

   else if ( ptr->recfgType == RGR_LCH_CFG)
   {
     printLchRecfg(&ptr->u.lchRecfg, buf);
   }

   else if ( ptr->recfgType == RGR_LCG_CFG)
   {
     printLcgRecfg(&ptr->u.lcgRecfg, buf);
   }

   EndStruct(buf);
   
   return ;
}

void printRgrDel(RgrDel *ptr, Buff *buf)
{
   BeginStruct("RgrDel", buf);

   LOG_ELEM("delType", 
                   ptr->delType, 
                   TR_ELEM_CONT);

   BeginStruct("u", buf);

   if ( ptr->delType == RGR_CELL_CFG)
   {
      BeginStruct("rgrCellDelS", buf);

      LOG_CmLteCellId(ptr->u.cellDel.cellId, buf, TR_ELEM_LAST);

      EndStruct(buf);
   }

   else if ( ptr->delType == RGR_UE_CFG)
   {
      BeginStruct("rgrUeDelS", buf);

      LOG_CmLteCellId(ptr->u.ueDel.cellId, buf, TR_ELEM_CONT);

      LOG_CmLteRnti(ptr->u.ueDel.crnti, buf, TR_ELEM_LAST);

      EndStruct(buf);
   }

   else if ( ptr->delType == RGR_LCH_CFG)
   {
      BeginStruct("rgrLchDelS", buf);

      LOG_CmLteCellId(ptr->u.lchDel.cellId, buf, TR_ELEM_CONT);

      LOG_CmLteRnti(ptr->u.lchDel.crnti, buf, TR_ELEM_CONT);
      
      LOG_CmLteLcId(ptr->u.lchDel.lcId, buf, TR_ELEM_LAST);
   
      EndStruct(buf);
   }

   else if ( ptr->delType == RGR_LCG_CFG)
   {
      BeginStruct("rgrLcgDelS", buf);

      LOG_CmLteCellId(ptr->u.lcgDel.cellId, buf, TR_ELEM_CONT);

      LOG_CmLteRnti(ptr->u.lcgDel.crnti, buf, TR_ELEM_CONT);
      
      LOG_ELEM("lcgId", ptr->u.lcgDel.lcgId, TR_ELEM_LAST);
   
      EndStruct(buf);
   }

   EndStruct(buf);

   EndStruct(buf); 

   return ;
}

void printRgrRst(RgrRst *ptr, Buff *buf)
{
   BeginStruct("RgrRst", buf);

   LOG_CmLteCellId(ptr->cellId, buf, TR_ELEM_CONT);

   LOG_CmLteRnti(ptr->crnti, buf, TR_ELEM_LAST);

   EndStruct(buf);     

   return ;
}

void printRgrLcgCfg(RgrLcgCfg *ptr, Buff *buf)
{
   BeginStruct("RgrLcgCfg", buf);

   LOG_CmLteCellId(ptr->cellId, buf, TR_ELEM_CONT);

   LOG_CmLteRnti(ptr->crnti, buf, TR_ELEM_CONT);

   BeginStruct("RgrUlLcgCfg", buf);


#ifdef LTE_L2_MEAS /* TODO: check below code */
#if 0 
   LOG_ELEM("lcgId", ptr->ulInfo.lcgId, TR_ELEM_CONT);

   LOG_ELEM("numLch", ptr->ulInfo.numLch, TR_ELEM_CONT);

   while( i < RGR_MAX_LC_PER_LCG )
   {
      BeginStruct("RgrUlLchCfg", buf);

      LOG_CmLteLcId(ptr->ulInfo.lchUlCfg[i].lcId, buf, TR_ELEM_CONT);
      
      LOG_ELEM("qci", ptr->ulInfo.lchUlCfg[i].qci, TR_ELEM_LAST);

      EndStruct(buf);
   }
#endif

#else 

   LOG_ELEM("lcgId", ptr->ulInfo.lcgId, TR_ELEM_LAST);

#endif 
   EndStruct(buf);

   return ;
}

void printRgrLchCfg(RgrLchCfg *ptr, Buff *buf)
{
   BeginStruct("RgrLchCfg", buf);

   LOG_CmLteCellId(ptr->cellId, buf, TR_ELEM_CONT);

   LOG_CmLteRnti(ptr->crnti, buf, TR_ELEM_CONT);

   LOG_CmLteLcId(ptr->lcId, buf, TR_ELEM_CONT);

   LOG_CmLteLcType(ptr->lcType, buf, TR_ELEM_CONT);

   BeginStruct("RgrDlLchCfg", buf);

   LOG_ELEM("dlTrchType", ptr->dlInfo.dlTrchType, TR_ELEM_CONT);

   printRgrLchQosCfg(&ptr->dlInfo.dlQos, buf);

#ifdef LTEMAC_SPS
   printRgrLchSpsCfg(&ptr->dlInfo.dlSpsCfg, buf);
#endif

   EndStruct(buf);

   EndStruct(buf);

   return ;
}

void printRgrLchQosCfg(RgrLchQosCfg *ptr, Buff *buf)
{
   BeginStruct("RgrLchQosCfg", buf);
 
   LOG_ELEM("qci", ptr->qci, TR_ELEM_CONT);
 
   LOG_ELEM("gbr", ptr->gbr, TR_ELEM_CONT);
 
   LOG_ELEM("mbr", ptr->mbr, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

#ifdef LTEMAC_SPS
void printRgrLchSpsCfg(RgrLchSpsCfg *ptr, Buff *buf)
{
   BeginStruct("RgrLchSpsCfg", buf);

   LOG_ELEM("isSpsEnabled", ptr->isSpsEnabled, TR_ELEM_CONT);

   LOG_ELEM("packetSize", ptr->packetSize, TR_ELEM_CONT);

   LOG_ELEM("packetPrdcty", ptr->packetPrdcty, TR_ELEM_CONT);

   LOG_ELEM("explicitRelCnt", ptr->explicitRelCnt, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}
#endif

void printRgrUeRecfg(RgrUeRecfg *ptr, Buff *buf)
{
   BeginStruct("RgrUeRecfg", buf);

   LOG_CmLteCellId(ptr->cellId, buf, TR_ELEM_CONT);

   LOG_CmLteRnti(ptr->oldCrnti, buf, TR_ELEM_CONT);

   LOG_CmLteRnti(ptr->newCrnti, buf, TR_ELEM_CONT);

#ifdef RGR_V2 
   LOG_ELEM("ueRecfgTypes", ptr->ueRecfgTypes, TR_ELEM_CONT);
#else
   LOG_ELEM("ueRecfgTypes", ptr->ueRecfgTypes, TR_ELEM_CONT);
#endif
   
   printRgrTxModeCfg(&ptr->txMode, buf); 

   LOG_RgrUeAprdDlCqiCfg(ptr->aprdDlCqiRecfg, buf);

#ifndef TFU_UPGRADE
   LOG_RgrUePrdDlCqiCfg(ptr->prdDlCqiRecfg, buf);
#endif 

   printRgrUeUlHqCfg(&ptr->ueUlHqRecfg, buf);

   printRgrUeQosCfg(&ptr->ueQosRecfg, buf); 

   LOG_RgrUeTaTmrCfg(ptr->ueTaTmrRecfg, buf, TR_ELEM_CONT);
#ifdef RGR_V1 
   LOG_RgrUeBsrTmrCfg(ptr->ueBsrTmrRecfg, buf);
#endif

   printRgrUeUlPwrCfg(&ptr->ueUlPwrRecfg, buf);

#ifdef LTEMAC_DRX
   printRgrUeDrxCfg(&ptr->ueDrxRecfg, buf);
#endif 

   LOG_ELEM("isTtiBundlEnabled", ptr->isTtiBundlEnabled, TR_ELEM_CONT);

   printRgrUeAckNackRepCfg(&ptr->ueAckNackRecfg, buf); 

   printRgrUeCapCfg(&ptr->ueCapRecfg, buf); 

   printRgrUeMeasGapCfg(&ptr->ueMeasGapRecfg, buf); 

   printRgrCodeBookRstCfg(&ptr->ueCodeBookRstRecfg, buf);

   LOG_CmLteUeCategory(ptr->ueCatEnum, buf, TR_ELEM_CONT);

#ifdef LTEMAC_MIMO
   printRgrUeTxAntSelCfg(&ptr->ulTxAntSel, buf);
#endif 

#ifdef LTEMAC_SPS
   printRgrUeSpsCfg(&ptr->ueSpsRecfg, buf);
#endif


#ifdef TFU_UPGRADE
   LOG_RgrUePrdDlCqiCfg(ptr->cqiCfg, buf);

   printRgrUeUlSrsCfg(&ptr->srsCfg, buf);

   printRgrUeSrCfg(&ptr->srCfg, buf);
#endif

#ifdef LTEMAC_HDFDD
   LOG_ELEM("isHdFddEnbld", ptr->isHdFddEnbld, TR_ELEM_CONT);
#endif

   printRgrUePuschDedCfg(&ptr->puschDedCfg, buf);

#ifdef RGR_CQI_REPT
   LOG_RgrUeCqiReptCfg(ptr->ueCqiReptCfg, buf, TR_ELEM_CONT);
#endif

#ifdef TFU_UPGRADE
   printRgrUePdschDedCfg(&ptr->uePdschDedCfg, buf); 
#endif

   EndStruct(buf);

   return ;
}

void printLchRecfg(RgrLchRecfg *ptr, Buff *buf)
{
   BeginStruct("RgrLchRecfg", buf);

   LOG_CmLteCellId(ptr->cellId, buf, TR_ELEM_CONT);

   LOG_CmLteRnti(ptr->crnti, buf, TR_ELEM_CONT);

   LOG_CmLteLcId(ptr->lcId, buf, TR_ELEM_CONT);

#ifdef LTEMAC_SPS
   LOG_ELEM("recfgTypes", ptr->recfgTypes, TR_ELEM_CONT);
#endif 

   BeginStruct("dlRecfg", buf);

   printRgrLchQosCfg(&ptr->dlRecfg.dlQos, buf);

#ifdef LTEMAC_SPS
   printRgrLchSpsCfg(&ptr->dlRecfg.dlSpsCfg, buf);
#endif

   EndStruct(buf);

   EndStruct(buf);

   return ;
}

void printLcgRecfg(RgrLcgRecfg *ptr, Buff  *buf)
{
   BeginStruct("RgrLcgRecfg", buf);

   LOG_CmLteCellId(ptr->cellId, buf, TR_ELEM_CONT);

   LOG_CmLteRnti(ptr->crnti, buf, TR_ELEM_CONT);

   LOG_ELEM("lcgId", ptr->ulRecfg.lcgId, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrUeCfg(RgrUeCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUeCfg", buf);

   LOG_CmLteCellId(ptr->cellId, buf, TR_ELEM_CONT);

   LOG_CmLteRnti(ptr->crnti, buf, TR_ELEM_CONT);
   
   printRgrTxModeCfg(&ptr->txMode, buf); 

   printRgrUeDlCqiCfg(&ptr->ueDlCqiCfg, buf);

   printRgrUeUlHqCfg(&ptr->ueUlHqCfg, buf);

   printRgrUeUlPwrCfg(&ptr->ueUlPwrCfg, buf);

   printRgrUeQosCfg(&ptr->ueQosCfg, buf); 

   LOG_RgrUeTaTmrCfg(ptr->ueTaTmrCfg, buf, TR_ELEM_CONT);

#ifdef RGR_V1    
   LOG_RgrUeBsrTmrCfg(ptr->ueBsrTmrCfg, buf);
#endif

   LOG_CmLteUeCategory(ptr->ueCatEnum, buf, TR_ELEM_CONT);

#ifdef LTEMAC_DRX
   printRgrUeDrxCfg(&ptr->ueDrxCfg, buf);
#endif

   LOG_ELEM("isTtiBundlEnabled", ptr->isTtiBundlEnabled, TR_ELEM_CONT);

   printRgrUeAckNackRepCfg(&ptr->ueAckNackCfg, buf); 

   printRgrUeMeasGapCfg(&ptr->ueMesGapCfg, buf); 

   printRgrUeCapCfg(&ptr->ueCapCfg, buf); 

   printRgrCodeBookRstCfg(&ptr->ueCodeBookRstCfg, buf);

   LOG_ELEM("dedPreambleId", ptr->dedPreambleId.val, TR_ELEM_CONT);

#ifdef LTE_TDD

   LOG_ELEM("ackNackModeEnum", ptr->ackNackModeEnum, TR_ELEM_CONT);

#endif 

#ifdef LTEMAC_SPS
   printRgrUeSpsCfg(&ptr->ueSpsCfg, buf);
#endif 

#ifdef TFU_UPGRADE
   printRgrUeUlSrsCfg(&ptr->srsCfg, buf);

   printRgrUeSrCfg(&ptr->srCfg, buf);
#endif

#ifdef LTEMAC_HDFDD
   LOG_ELEM("isHdFddEnbld", ptr->isHdFddEnbld, TR_ELEM_CONT);
#endif  

   printRgrUePuschDedCfg(&ptr->puschDedCfg, buf);

   printRgrUeTxAntSelCfg(&ptr->ulTxAntSel, buf);

#ifdef RGR_CQI_REPT

   LOG_RgrUeCqiReptCfg(ptr->ueCqiReptCfg, buf, TR_ELEM_CONT);

#endif 

#ifdef TFU_UPGRADE

   printRgrUePdschDedCfg(&ptr->uePdschDedCfg, buf); 

#endif 

   EndStruct(buf);

   return ;
}

#ifdef TFU_UPGRADE
void printRgrUePdschDedCfg(RgrUePdschDedCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUePdschDedCfg", buf);

   BeginStruct("RgrUepACfg", buf);

   LOG_ELEM("pA", ptr->uepACfg.pA, TR_ELEM_CONT);

   LOG_ELEM("pAPrst", ptr->uepACfg.pAPrsnt, TR_ELEM_LAST);

   EndStruct(buf);

   EndStruct(buf);

   return ;
}
#endif 

void printRgrUeTxAntSelCfg(RgrUeTxAntSelCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUeTxAntSelCfg", buf);

   LOG_ELEM("pres", ptr->pres, TR_ELEM_CONT);

   LOG_ELEM("selType", ptr->selType, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrUePuschDedCfg(RgrUePuschDedCfg *ptr, Buff *buf) 
{
   BeginStruct("RgrUePuschDedCfg", buf);

   LOG_ELEM("pres", ptr->pres, TR_ELEM_CONT);

   LOG_ELEM("bACKIdx", ptr->bACKIdx, TR_ELEM_CONT);

   LOG_ELEM("bRIIdx", ptr->bRIIdx, TR_ELEM_CONT);

   LOG_ELEM("bCQIIdx", ptr->bCQIIdx, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

#ifdef TFU_UPGRADE

void printRgrUeSrCfg(RgrUeSrCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUeSrCfg", buf);

   LOG_ELEM("type", ptr->type, TR_ELEM_CONT);

   BeginStruct("RgrUeSrSetupCfg", buf);

   LOG_ELEM("srResIdx", ptr->srSetup.srResIdx, TR_ELEM_CONT);

   LOG_ELEM("srCfgIdx", ptr->srSetup.srCfgIdx, TR_ELEM_CONT);

   LOG_ELEM("dTMax", ptr->srSetup.dTMax, TR_ELEM_LAST);

   EndStruct(buf);

   EndStruct(buf);

   return ;
}

void printRgrUeUlSrsCfg(RgrUeUlSrsCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUeUlSrsCfg", buf);

   LOG_ELEM("type", ptr->type, TR_ELEM_CONT);

   BeginStruct("RgrUeUlSrsSetupCfg", buf);

   LOG_ELEM("srsCfgIdx", ptr->srsSetup.srsCfgIdx, TR_ELEM_CONT);

   LOG_ELEM("srsBw", ptr->srsSetup.srsBw, TR_ELEM_CONT);

   LOG_ELEM("srsHopBw", ptr->srsSetup.srsHopBw, TR_ELEM_CONT);

   LOG_ELEM("cycShift", ptr->srsSetup.cycShift, TR_ELEM_CONT);

   LOG_ELEM("duration", ptr->srsSetup.duration, TR_ELEM_CONT);

   LOG_ELEM("sANSrs", ptr->srsSetup.sANSrs, TR_ELEM_CONT);

   LOG_ELEM("txComb", ptr->srsSetup.txComb, TR_ELEM_CONT);

   LOG_ELEM("fDomPosi", ptr->srsSetup.fDomPosi, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}
#endif

#ifdef LTEMAC_SPS
void printRgrUeSpsCfg(RgrUeSpsCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUeSpsCfg", buf);

   LOG_CmLteRnti("spsRnti", ptr->spsRnti, TR_ELEM_CONT);

   BeginStruct("RgrUeSpsDlCfg", buf);

   LOG_ELEM("isDlSpsEnabled", ptr->dlSpsCfg.isDlSpsEnabled, TR_ELEM_CONT);

   LOG_ELEM("numSpsHqProc", ptr->dlSpsCfg.numSpsHqProc, TR_ELEM_CONT);

   LOG_ELEM("numPucchVal", ptr->dlSpsCfg.numPucchVal, TR_ELEM_CONT);

   LOG_ARRAY("n1PucchVal", ptr->dlSpsCfg.n1PucchVal, 4, TR_ELEM_CONT);

   LOG_ELEM("dlSpsPrdctyEnum", ptr->dlSpsCfg.dlSpsPrdctyEnum, TR_ELEM_LAST);

   LOG_ELEM("isUlSpsEnabled", ptr->ulSpsCfg.isUlSpsEnabled, TR_ELEM_CONT);

   LOG_ELEM("implicitRelCnt", ptr->ulSpsCfg.implicitRelCnt, TR_ELEM_CONT);

#ifdef LTE_TDD
   LOG_ELEM("twoIntervalCfg", ptr->ulSpsCfg.twoIntervalCfg, TR_ELEM_CONT);
#endif

   LOG_ELEM("pwrCfgPres", ptr->ulSpsCfg.pwrCfgPres, TR_ELEM_CONT);

   BeginStruct("pwrCfg", buf);

   LOG_ELEM("p0NominalPuschVal", ptr->ulSpsCfg.pwrCfg.p0NominalPuschVal, TR_ELEM_CONT);

   LOG_ELEM("p0UePuschVal", ptr->ulSpsCfg.pwrCfg.p0UePuschVal, TR_ELEM_LAST);

   EndStruct(buf);

   LOG_ELEM("ulSpsPrdctyEnum", ptr->ulSpsCfg.ulSpsPrdctyEnum, TR_ELEM_CONT);

   LOG_ELEM("packetSize", ptr->ulSpsCfg.packetSize, TR_ELEM_CONT);

   LOG_ELEM("lcCnt", ptr->ulSpsCfg.lcCnt, TR_ELEM_CONT);

   LOG_ARRAY("lcId", ptr->dlSpsCfg.n1PucchVal, RGR_MAX_SPS_LC, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}
#endif 

void printRgrCodeBookRstCfg(RgrCodeBookRstCfg *ptr, Buff *buf)
{
   BeginStruct("RgrCodeBookRstCfg", buf);

   LOG_ELEM("pres", ptr->pres, TR_ELEM_CONT);

   LOG_ARRAY("pmiBitMap", ptr->pmiBitMap, 2, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrUeCapCfg(RgrUeCapCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUeCapCfg", buf);

   LOG_ELEM("pwrClass", ptr->pwrClass, TR_ELEM_CONT);

   LOG_ELEM("intraSfFeqHop", ptr->intraSfFeqHop, TR_ELEM_CONT);

   LOG_ELEM("resAloocType1", ptr->resAloocType1, TR_ELEM_CONT);

#ifdef LTEMAC_MIMO
   LOG_ELEM("simCqiAckNack", ptr->simCqiAckNack, TR_ELEM_CONT);

   LOG_ELEM("txAntSel", ptr->txAntSel, TR_ELEM_LAST);
#else 
   LOG_ELEM("simCqiAckNack", ptr->simCqiAckNack, TR_ELEM_LAST);
#endif 

   EndStruct(buf);

   return ;
}

void printRgrUeMeasGapCfg(RgrUeMeasGapCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUeMeasGapCfg", buf);

   LOG_ELEM("isMesGapEnabled", ptr->isMesGapEnabled, TR_ELEM_CONT);

   LOG_ELEM("gapPrd", ptr->gapPrd, TR_ELEM_CONT);

   LOG_ELEM("gapOffst", ptr->gapOffst, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}


void printRgrUeAckNackRepCfg(RgrUeAckNackRepCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUeAckNackRepCfg", buf);

   LOG_ELEM("isAckNackEnabled", ptr->isAckNackEnabled, TR_ELEM_CONT);

   LOG_ELEM("pucchAckNackRep", ptr->pucchAckNackRep, TR_ELEM_CONT);

   LOG_ELEM("ackNackRepFactor", ptr->ackNackRepFactor, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}


void printRgrUeDrxCfg(RgrUeDrxCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUeQosCfg", buf);

   LOG_ELEM("isDrxEnabled", ptr->isDrxEnabled, TR_ELEM_CONT);

#ifdef LTEMAC_R9 

   LOG_ELEM("cqiMask", ptr->cqiMask, TR_ELEM_CONT);

#endif    

   LOG_ELEM("drxOnDurTmr", ptr->drxOnDurTmr, TR_ELEM_CONT);

   LOG_ELEM("drxInactvTmr", ptr->drxInactvTmr, TR_ELEM_CONT);

   LOG_ELEM("drxRetxTmr", ptr->drxRetxTmr, TR_ELEM_CONT);

   LOG_ELEM("drxRetxTmr", ptr->drxRetxTmr, TR_ELEM_CONT);

   BeginStruct("RgrDrxLongCycleOffst", buf);
   
   LOG_ELEM("longDrxCycle", ptr->drxLongCycleOffst.longDrxCycle, TR_ELEM_CONT);
   
   LOG_ELEM("drxStartOffst", ptr->drxLongCycleOffst.drxStartOffst, TR_ELEM_LAST);

   EndStruct(buf);

   BeginStruct("RgrDrxShortDrx", buf);

   LOG_ELEM("pres", ptr->drxShortDrx.pres, TR_ELEM_CONT);
   
   LOG_ELEM("shortDrxCycle", ptr->drxShortDrx.shortDrxCycle, TR_ELEM_LAST);

   LOG_ELEM("drxShortCycleTmr", ptr->drxShortDrx.drxShortCycleTmr, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrUeQosCfg(RgrUeQosCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUeQosCfg", buf);

   LOG_ELEM("ambrPres", ptr->ambrPres, TR_ELEM_CONT);

   LOG_ELEM("dlAmbr", ptr->dlAmbr, TR_ELEM_CONT);

   LOG_ELEM("ueBr", ptr->ueBr, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrUeUlPwrCfg(RgrUeUlPwrCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUeUlPwrCfg", buf);

   LOG_RgrUeGrpPwrCfg(ptr->uePuschPwr, buf);

   LOG_RgrUeGrpPwrCfg(ptr->uePucchPwr, buf);

   LOG_ELEM("isAccumulated", 
                   ptr->isAccumulated, 
                   TR_ELEM_CONT);

   LOG_ELEM("isDeltaMCSEnabled", 
                   ptr->isDeltaMCSEnabled, 
                   TR_ELEM_CONT);

   LOG_ELEM("p0UePusch", 
                   ptr->p0UePusch, 
                   TR_ELEM_CONT);

   LOG_ELEM("p0UePucch", 
                   ptr->p0UePucch, 
                   TR_ELEM_CONT);

   LOG_ELEM("pSRSOffset", 
                   ptr->pSRSOffset, 
                   TR_ELEM_CONT);

   LOG_ELEM("trgCqi", 
                   ptr->trgCqi, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrUeUlHqCfg(RgrUeUlHqCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUeUlHqCfg", buf);

   LOG_ELEM("maxUlHqTx", 
                   ptr->maxUlHqTx, 
                   TR_ELEM_CONT);

   LOG_ELEM("deltaHqOffst", 
                   ptr->deltaHqOffst, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrUeDlCqiCfg(RgrUeDlCqiCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUeDlCqiCfg", buf);

   LOG_RgrUeAprdDlCqiCfg(ptr->aprdCqiCfg, buf);

   LOG_RgrUePrdDlCqiCfg(ptr->prdCqiCfg, buf);

   EndStruct(buf);

   return ;
}

void printRgrTxModeCfg(RgrUeTxModeCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUeTxModeCfg", buf);

   LOG_ELEM("pres", 
                   ptr->pres, 
                   TR_ELEM_CONT);

   LOG_ELEM("tmTrnstnState", 
                   ptr->tmTrnstnState, 
                   TR_ELEM_CONT);

   LOG_ELEM("txModeEnum", 
                   ptr->txModeEnum, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrCellRecfg(RgrCellRecfg *ptr, Buff *buf)
{
   BeginStruct("RgrCellRecfg", buf);

   LOG_CmLteCellId(ptr->cellId, buf, TR_ELEM_CONT);

   LOG_ELEM("recfgTypes", 
                   ptr->recfgTypes, 
                   TR_ELEM_CONT);

   BeginStruct("RgrActvTime", buf);

   LOG_ELEM("pres", 
                   ptr->recfgActvTime.pres, 
                   TR_ELEM_CONT);

   LOG_CmLteTimingInfo(ptr->recfgActvTime.actvTime, buf);

   EndStruct(buf);

   printRgrDlHqCfg(&ptr->dlHqRecfg, buf);

   printRgrCfiCfg(&ptr->cfiRecfg, buf);

   printRgrUlTrgCqiCfg(&ptr->trgUlCqi, buf);

   printRgrDlCmnCodeRateCfg(&ptr->dlCmnCodeRate, buf);

   printRgrPuschSubBandCfg(&ptr->puschSubBand, buf);

   printRgrUlCmnCodeRateCfg(&ptr->ulCmnCodeRate, buf);

   printRgrPucchCfg(&ptr->pucchRecfg, buf);

   printRgrSrsCfg(&ptr->srsRecfg, buf);

   printRgrRachCfg(&ptr->rachRecfg, buf);

   printRgrDlfsCfg(&ptr->dlfsRecfg, buf);

#ifdef RGR_SI_SCH
   printRgrSiCfg(&ptr->siReCfg, buf);
#endif

   EndStruct(buf);

   return ;
}

void printRgrCellCfg(RgrCellCfg *ptr, Buff *buf)
{
   int indx = 0; 

   BeginStruct("RgrCellCfg", buf);

   LOG_CmLteCellId(ptr->cellId, buf, TR_ELEM_CONT);

   LOG_Inst(ptr->macInst, buf, TR_ELEM_CONT);

#ifdef RGR_V1
   LOG_ELEM("macMsg3PerUlSf", 
                   ptr->maxMsg3PerUlSf, 
                   TR_ELEM_CONT);
#endif 
   
   LOG_ELEM("maxUePerUlSf", 
                   ptr->maxUePerUlSf, 
                   TR_ELEM_CONT);
   
   LOG_ELEM("maxUePerDlSf", 
                   ptr->maxUePerDlSf, 
                   TR_ELEM_CONT);
   
   LOG_ELEM("maxUlBwPerUe", 
                   ptr->maxUlBwPerUe, 
                   TR_ELEM_CONT);
   
   LOG_ELEM("maxDlBwPerUe", 
                   ptr->maxDlBwPerUe, 
                   TR_ELEM_CONT);
   
   LOG_ELEM("maxDlRetxBw", 
                   ptr->maxDlRetxBw, 
                   TR_ELEM_CONT);
   
   LOG_ELEM("maxDlUeNewTxPerTti", 
                   ptr->maxDlUeNewTxPerTti, 
                   TR_ELEM_CONT);
   
   LOG_ELEM("maxUlUeNewTxPerTti", 
                   ptr->maxUlUeNewTxPerTti, 
                   TR_ELEM_CONT);
   
   LOG_ELEM("numTxAntPorts", 
                   ptr->numTxAntPorts, 
                   TR_ELEM_CONT);
   
   LOG_ELEM("isCpUlExtend", 
                   ptr->isCpUlExtend, 
                   TR_ELEM_CONT);
   
   LOG_ELEM("isCpDlExtend", 
                   ptr->isCpDlExtend, 
                   TR_ELEM_CONT);
   
   LOG_ELEM("cellModSchm", 
                   ptr->cellModSchm, 
                   TR_ELEM_CONT);
   
   LOG_ELEM("dlSchdType", 
                   ptr->dlSchdType, 
                   TR_ELEM_CONT);
   
   LOG_ELEM("pMax", 
                   ptr->pMax, 
                   TR_ELEM_CONT);

   if ( ptr->dlSchdType == RGR_SCH_TYPE_PFS) 
   {
      BeginStruct("rgrDlSchInfoS", buf);

      BeginStruct("RgrCellDlPfs", buf);

      LOG_ARRAY("cqiPrios", 
                   ptr->dlSchInfo.cellDlPfs.cqiPrios, 
                   RGR_PFS_SVC_PRIO_LVLS, TR_ELEM_CONT);

      LOG_ARRAY("fracPrios", 
                   ptr->dlSchInfo.cellDlPfs.fracPrios, 
                   RGR_PFS_SVC_PRIO_LVLS, TR_ELEM_CONT);

      LOG_ARRAY("svcPrioPrios", 
                   ptr->dlSchInfo.cellDlPfs.svcPrioPrios, 
                   RGR_PFS_SVC_PRIO_LVLS, TR_ELEM_LAST);

      EndStruct(buf);

      EndStruct(buf);
   }
   
   LOG_ELEM("ulSchdType", 
                   ptr->ulSchdType, 
                   TR_ELEM_CONT);
   
   if ( ptr->ulSchdType == RGR_SCH_TYPE_PFS) 
   {
      BeginStruct("rgrUlSchInfoS", buf);

      BeginStruct("RgrCellUlPfs", buf);

      LOG_ARRAY("cqiPrios", 
                   ptr->ulSchInfo.cellUlPfs.cqiPrios, 
                   RGR_PFS_SVC_PRIO_LVLS, TR_ELEM_CONT);

      LOG_ARRAY("fracPrios", 
                   ptr->ulSchInfo.cellUlPfs.fracPrios, 
                   RGR_PFS_SVC_PRIO_LVLS, TR_ELEM_CONT);

      LOG_ARRAY("lcgPrios", 
                   ptr->ulSchInfo.cellUlPfs.lcgPrios, 
                   3, TR_ELEM_LAST);

      EndStruct(buf);

      EndStruct(buf);
   }

   LOG_ELEM("dlfsSchdType", 
                   ptr->dlfsSchdType, 
                   TR_ELEM_CONT);

   printRgrDlHqCfg(&ptr->dlHqCfg, buf);

   printRgrRntiCfg(&ptr->macRnti, buf);

   printRgrCfiCfg(&ptr->cfiCfg, buf);

   printRgrUlTrgCqiCfg(&ptr->trgUlCqi, buf);

   printRgrDlCmnCodeRateCfg(&ptr->dlCmnCodeRate, buf);

   printRgrPuschSubBandCfg(&ptr->puschSubBand, buf);

   printRgrUlCmnCodeRateCfg(&ptr->ulCmnCodeRate, buf);

   printRgrDlfsCfg(&ptr->dlfsCfg, buf);

   printRgrBwCfg(&ptr->bwCfg, buf);

   printRgrPhichCfg(&ptr->phichCfg, buf);

   printRgrPucchCfg(&ptr->pucchCfg, buf);

   printRgrSrsCfg(&ptr->srsCfg, buf);

   printRgrRachCfg(&ptr->rachCfg, buf);

   printRgrSiCfg(&ptr->siCfg, buf);

   printRgrUlPwrCfg(&ptr->pwrCfg, buf);

   printRgrPuschCfg(&ptr->puschCfg, buf);

   printRgrPreambleSetCfg(&ptr->macPreambleSet, buf);

   LOG_ELEM("numCmnLcs", 
                   ptr->numCmnLcs, 
                   TR_ELEM_CONT);

   while ( indx < RGR_MAX_CMN_LC_PER_CELL)
   {
       BeginStruct("RgrCmnLchCfg", buf);
       
       LOG_ELEM("lcId", 
                   ptr->cmnLcCfg[indx].lcId, 
                   TR_ELEM_CONT);

       LOG_ELEM("lcType", 
                   ptr->cmnLcCfg[indx].lcType, 
                   TR_ELEM_CONT);

       LOG_ELEM("dir", 
                   ptr->cmnLcCfg[indx].dir, 
                   TR_ELEM_CONT);

       LOG_ELEM("dlTrchType", 
                   ptr->cmnLcCfg[indx].dlTrchType, 
                   TR_ELEM_CONT);

       LOG_ELEM("ulTrchType", 
                   ptr->cmnLcCfg[indx].ulTrchType, 
                   TR_ELEM_LAST);

       indx++;

       EndStruct(buf);

   }
#ifdef LTE_TDD 

       LOG_ELEM("ulDlCfgIdx", 
                   ptr->ulDlCfgIdx,
                   TR_ELEM_CONT);

       LOG_ELEM("spclSfCfgIdx", 
                   ptr->spclSfCfgIdx,
                   TR_ELEM_CONT);

       BeginStruct("RgrTddPrachRscInfo", buf);

       LOG_ELEM("numRsc", 
                   ptr->prachRscInfo.numRsc,
                   TR_ELEM_CONT);

       i = 0; 

       while ( i < ptr->prachRscInfo.numRsc)
       {
           BeginStruct("RgrTddPrachInfo", buf);

           LOG_ELEM("freqIdx", 
                   ptr->prachRscInfo.prachInfo[i].freqIdx,
                   TR_ELEM_CONT);

           LOG_ELEM("sfn", 
                   ptr->prachRscInfo.prachInfo[i].sfn,
                   TR_ELEM_CONT);

           LOG_ELEM("halfFrm", 
                   ptr->prachRscInfo.prachInfo[i].halfFrm,
                   TR_ELEM_CONT);

           LOG_ELEM("ulStartSfIdx", 
                   ptr->prachRscInfo.prachInfo[i].ulStartSfIdx,
                   TR_ELEM_LAST);
       
           EndStruct(buf);

           i++;
       }

       EndStruct(buf);
#endif 

#ifdef RGR_RRM_TICK 

   LOG_ELEM("rrmTtiIndPrd", 
              ptr->rrmTtiIndPrd,
              TR_ELEM_CONT);
#endif

#ifdef LTEMAC_SPS
  BeginStruct("RgrSpsDlCellCfg", buf);

  LOG_ELEM("dlSpsCfg", 
              ptr->dlSpsCfg,
              TR_ELEM_LAST);

  EndStruct(buf);
#endif 

  EndStruct(buf);

  return ;
}


void printRgrDlHqCfg(RgrDlHqCfg *ptr, Buff *buf)
{
   BeginStruct("RgrDlHqCfg", buf);

   LOG_ELEM("maxDlHqTx", 
                   ptr->maxDlHqTx, 
                   TR_ELEM_CONT);

   LOG_ELEM("maxMsg4HqTx", 
                   ptr->maxMsg4HqTx, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrRntiCfg(RgrRntiCfg *ptr, Buff *buf)
{
   BeginStruct("RgrRntiCfg", buf);

   LOG_ELEM("startRnti", 
                   ptr->startRnti, 
                   TR_ELEM_CONT);

   LOG_ELEM("size", 
                   ptr->size, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrCfiCfg(RgrCfiCfg *ptr, Buff *buf)
{
   BeginStruct("RgrCfiCfg", buf);

   LOG_ELEM("cfi", 
                   ptr->cfi, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrUlTrgCqiCfg(RgrUlTrgCqiCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUlTrgCqiCfg", buf);

   LOG_ELEM("trgCqi", 
                   ptr->trgCqi, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrDlCmnCodeRateCfg(RgrDlCmnCodeRateCfg *ptr, Buff *buf)
{
   BeginStruct("RgrDlCmnCodeRateCfg", buf);

   LOG_ELEM("bcchPchRaCodeRate", 
                   ptr->bcchPchRaCodeRate, 
                   TR_ELEM_CONT);

   LOG_ELEM("pdcchCodeRate", 
                   ptr->pdcchCodeRate, 
                   TR_ELEM_CONT);

   LOG_ELEM("ccchCqi", 
                   ptr->ccchCqi, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrPuschSubBandCfg(RgrPuschSubBandCfg *ptr, Buff *buf)
{
   BeginStruct("RgrPuschSubBandCfg", buf);

   LOG_ELEM("subbandStart", 
                   ptr->subbandStart, 
                   TR_ELEM_CONT);

   LOG_ELEM("numSubbands", 
                   ptr->numSubbands, 
                   TR_ELEM_CONT);

   LOG_ELEM("size", 
                   ptr->size, 
                   TR_ELEM_CONT);

   LOG_ARRAY("dmrs", 
                   ptr->dmrs, RGR_MAX_SUBBANDS,  
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrUlCmnCodeRateCfg(RgrUlCmnCodeRateCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUlCmnCodeRateCfg", buf);

   LOG_ELEM("ccchCqi", 
                   ptr->ccchCqi, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrDlfsCfg(RgrDlfsCfg *ptr, Buff *buf)
{
   BeginStruct("RgrDlfsCfg", buf);

   LOG_ELEM("isDlFreqSel", 
                   ptr->isDlFreqSel, 
                   TR_ELEM_CONT);

   LOG_ELEM("thresholdCqi", 
                   ptr->thresholdCqi, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrBwCfg(RgrBwCfg *ptr, Buff *buf)
{
   BeginStruct("RgrBwCfg", buf);

   LOG_ELEM("dlTotalBw", 
                   ptr->dlTotalBw, 
                   TR_ELEM_CONT);

   LOG_ELEM("ulTotalBw", 
                   ptr->ulTotalBw, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrPhichCfg(RgrPhichCfg *ptr, Buff *buf)
{
   BeginStruct("RgrPhichCfg", buf);

   LOG_ELEM("ngEnum", 
                   ptr->ngEnum, 
                   TR_ELEM_CONT);

   LOG_ELEM("isDurExtend", 
                   ptr->isDurExtend, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrPucchCfg(RgrPucchCfg *ptr, Buff *buf)
{
   BeginStruct("RgrPucchCfg", buf);

   LOG_ELEM("resourceSize", 
                   ptr->resourceSize, 
                   TR_ELEM_CONT);

   LOG_ELEM("n1PucchAn", 
                   ptr->n1PucchAn, 
                   TR_ELEM_CONT);

   LOG_ELEM("deltaShift", 
                   ptr->deltaShift, 
                   TR_ELEM_CONT);

   LOG_ELEM("cyclicShift", 
                   ptr->cyclicShift, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrSrsCfg(RgrSrsCfg *ptr, Buff *buf)
{
   BeginStruct("RgrSrsCfg", buf);

   LOG_ELEM("srsCfgPrdEnum", 
                   ptr->srsCfgPrdEnum, 
                   TR_ELEM_CONT);

   LOG_ELEM("srsBwEnum", 
                   ptr->srsBwEnum, 
                   TR_ELEM_CONT);

   LOG_ELEM("srsSubFrameCfg", 
                   ptr->srsSubFrameCfg, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrRachCfg(RgrRachCfg *ptr, Buff *buf)
{
   BeginStruct("RgrRachCfg", buf);

   LOG_ELEM("preambleFormat", 
                   ptr->preambleFormat, 
                   TR_ELEM_CONT);

   LOG_ELEM("raWinSize", 
                   ptr->raWinSize, 
                   TR_ELEM_CONT);

   BeginStruct("raOccasion", buf);

   LOG_ELEM("size", 
                   ptr->raOccasion.size, 
                   TR_ELEM_CONT);

   LOG_ELEM("sfnEnum", 
                   ptr->raOccasion.sfnEnum, 
                   TR_ELEM_CONT);

   LOG_ARRAY("subFrameNum", 
                   ptr->raOccasion.subFrameNum, 
                   RGR_MAX_SUBFRAME_NUM,
                   TR_ELEM_LAST);

   EndStruct(buf);

   LOG_ELEM("maxMsg3Tx", 
                   ptr->maxMsg3Tx, 
                   TR_ELEM_CONT);

   LOG_ELEM("numRaPreamble", 
                   ptr->numRaPreamble, 
                   TR_ELEM_CONT);

   LOG_ELEM("sizeRaPreambleGrpA", 
                   ptr->sizeRaPreambleGrpA, 
                   TR_ELEM_CONT);

   LOG_ELEM("msgSizeGrpA", 
                   ptr->msgSizeGrpA, 
                   TR_ELEM_CONT);

#ifdef RGR_V1 

   LOG_ELEM("prachResource", 
                   ptr->prachResource, 
                   TR_ELEM_CONT);

   LOG_ELEM("contResTmr", 
                   ptr->contResTmr, 
                   TR_ELEM_LAST);
#else 
   LOG_ELEM("prachResource", 
                   ptr->prachResource, 
                   TR_ELEM_LAST);

#endif 

   EndStruct(buf);

   return ;
}

void printRgrSiCfg(RgrSiCfg *ptr, Buff *buf)
{
   BeginStruct("RgrSiCfg", buf);

   LOG_ELEM("siWinSize", 
                   ptr->siWinSize, 
                   TR_ELEM_CONT);
#ifdef RFR_SI_SCH

   LOG_ELEM("retxCnt", 
                   ptr->retxCnt, 
                   TR_ELEM_CONT);

   LOG_ELEM("modPrd", 
                   ptr->modPrd, 
                   TR_ELEM_CONT);

   LOG_ELEM("numSi", 
                   ptr->numSi, 
                   TR_ELEM_CONT);

   LOG_ARRAY("siPeriodicity", 
                   ptr->siPeriodicity, 
                   RGR_MAX_NUM_SI, 
                   TR_ELEM_LAST);
#else 
   LOG_ELEM("retxCnt", 
                   ptr->retxCnt, 
                   TR_ELEM_LAST);

#endif 

   EndStruct(buf);

   return ;
}

void printRgrUlPwrCfg(RgrUlPwrCfg *ptr, Buff *buf)
{
   BeginStruct("RgrUlPwrCfg", buf);

   LOG_ELEM("p0NominalPusch", 
                   ptr->p0NominalPusch, 
                   TR_ELEM_CONT);

   LOG_ELEM("alpha", 
                   ptr->alpha, 
                   TR_ELEM_CONT);

   LOG_ELEM("p0NominalPucch", 
                   ptr->p0NominalPucch, 
                   TR_ELEM_CONT);

   LOG_ELEM("deltaPreambleMsg3", 
                   ptr->deltaPreambleMsg3, 
                   TR_ELEM_CONT);

   LOG_RGRUlPwrCfg(ptr->pucchPwrFmt3, buf, TR_ELEM_CONT);
   
   LOG_RGRUlPwrCfg(ptr->pucchPwrFmt3a, buf, TR_ELEM_CONT);
   
   LOG_RGRUlPwrCfg(ptr->puschPwrFmt3, buf, TR_ELEM_CONT);
   
   LOG_RGRUlPwrCfg(ptr->puschPwrFmt3a, buf, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrPuschCfg(RgrPuschCfg *ptr, Buff *buf)
{
   BeginStruct("RgrPuschCfg", buf);

   LOG_ELEM("numSubBands", 
                   ptr->numSubBands, 
                   TR_ELEM_CONT);

   LOG_ELEM("isIntraHop", 
                   ptr->isIntraHop, 
                   TR_ELEM_CONT);

   LOG_ELEM("hopOffst", 
                   ptr->hopOffst, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printRgrPreambleSetCfg(RgrPreambleSetCfg *ptr, Buff *buf)
{
   BeginStruct("RgrPreambleSetCfg", buf);

   LOG_ELEM("pres", 
                   ptr->pres, 
                   TR_ELEM_CONT);

   LOG_ELEM("start", 
                   ptr->start, 
                   TR_ELEM_CONT);

   LOG_ELEM("size", 
                   ptr->size, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

