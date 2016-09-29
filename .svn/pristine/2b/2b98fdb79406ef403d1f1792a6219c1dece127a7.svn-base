//#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"
#include "ssi.h"           /* System services */
#include "cm_tkns.h"       /* Common Token defines */
#include "cm_lte.h"        /* Common LTE Defines */

#include "nhu.h"           /* NHU Interface defines */
#include "nhu_3gasn.h"     /* 3G RRC UE Capability/Handover Message ASN Definitions */
#include "lnh.h"           /* LNH Interface includes */
#include <stdio.h>

#include "gen.x"
#include "ssi.x" 
#include "cm5.x"           /* Timer services */
#include "cm_lib.x"        /* Common library definitions */
#include "cm_tkns.x"
#include "cm_lte.x"
#include "cm_mblk.x"
#include "cm_llist.x"      /* Common Link List definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "lnh.x"           /* LNH Interface includes */
#include "nhu_asn.x"
#include "nhu_3gasn.x"
#include "nhu.x"           /* NHU Interface includes */

#include "trace.h"
#include "tr_nh.h"
#include "tr_common.h"

void printNhuHdr(NhuHdr *hdr, Buff *buf);
void printNhuDlDcchMsg(NhuDlDcchMsg *dlDcchMsg, Buff *buf);
void printNhuDlCcchMsg(NhuDlCcchMsg *ptr, Buff *buf); 
void printNhuUlCcchMsg(NhuUlCcchMsg *ptr, Buff *buf);
void printNhuUlDcchMsg(NhuUlDcchMsg *ptr, Buff *buf);
void printNhuCellCfgInfo(NhuCellCfgInfo *ptr, Buff *buf);
void printCellReCfgInfo(NhuCellReCfgInfo *reCfgReq, Buff *buf);
void printNhuUeCfgInfo(NhuUeCfgInfo *ptr, Buff *buf);
void printNhuCellRlsInfo(NhuCellRlsInfo *ptr,Buff *buf);
void printNhuRbCfgList(NhuRbCfgList *rbCfgList, Buff *buf);
#ifdef RRC_PHY_CFG_SUPP
void printNhuCmnPhyRecfg(NhuCmnPhyRecfg *ptr,Buff *buf);
#endif 


void printNhuHdr(NhuHdr *hdr, Buff *buf)
{
  BeginStruct("hdr(NhuHdr)", buf);

  LOG_ELEM ( "CellId ", 
              hdr->cellId, 
	      TR_ELEM_CONT);
  
  LOG_ELEM ( "UeId ", 
	      hdr->ueId, 
	      TR_ELEM_CONT);

  LOG_ELEM ( "TransactionId ", 
	      hdr->transId,
	      TR_ELEM_CONT);

  EndStruct(buf);

  return ;
}

/*
 * Fun  : printNhuCellCfgReqSdus
 *
 * Desc : prints the NhuCellCfgReqSdus structure used 
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuCellCfgReqSdus(NhuCellCfgReqSdus *cfgReq, Buff *buf)
{
   BeginStruct("NhuCfgReqSdus", buf);

   printNhuHdr(&cfgReq->hdr, buf);

   BeginStruct("sdu", buf);

   LOG_ELEM("Config Type", 
		   cfgReq->sdu.cfgType, 
		   TR_ELEM_CONT);

   if(cfgReq->sdu.cfgType == NHU_CFGTYPE_SETUP)
   {
     printNhuCellCfgInfo(&cfgReq->sdu.u.cfgInfo, buf);
   }
   else if(cfgReq->sdu.cfgType == NHU_CFGTYPE_MODIFY)
   {
     printCellReCfgInfo(&cfgReq->sdu.u.reCfgInfo, buf);
   }
   else if(cfgReq->sdu.cfgType == NHU_CFGTYPE_RELEASE)
   {
     printNhuCellRlsInfo(&cfgReq->sdu.u.rlsInfo, buf);
   }

   return ;

}

/*
 * Fun  : printNhUiNhuCfgReq 
 *
 * Desc : prints the NhUiNhuCfgReq structure used 
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhUiNhuCfgReq(NhuCfgReqSdus *CfgReq, Buff *buf)
{
   BeginStruct("NhuCfgReqSdus()", buf);

   printNhuHdr(&CfgReq->hdr, buf);
  
   printNhuUeCfgInfo(&CfgReq->cfgInfo, buf);

   EndStruct(buf);

   return ;
}

/*
 * Fun  : printNhUiNhuDatReq
 *
 * Desc : prints the NhUiNhuDatReq structure used 
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhUiNhuDatReq(NhuDatReqSdus *nhuDatReq, Buff *buf)
{

  BeginStruct("NhUiNhuDatReq", buf);

  printNhuHdr(&nhuDatReq->hdr, buf);

  BeginStruct("sdu", buf);

  BeginStruct("NhuDatReqUeMsg", buf);

  LOG_ELEM ("isMsgDed",  nhuDatReq->sdu.ueMsg.isMsgDed, TR_ELEM_CONT);

  if ( nhuDatReq->sdu.ueMsg.isMsgDed == TRUE)
  {
    BeginStruct("msg", buf);

    BeginStruct("dedMsg(NhuDedMsg)", buf);

    printNhuDlDcchMsg(&nhuDatReq->sdu.ueMsg.msg.dedMsg.dlDcchMsg, buf);

    EndStruct(buf);

    EndStruct(buf);
  }
  else
  {
    BeginStruct("msg", buf);

    BeginStruct("cmnMsg(NhuCmnMsg)", buf);

    BeginStruct("cmnMsgInfo(NhuCmnMsgInfo)", buf);

    LOG_ELEM ( "rbId ",
                 nhuDatReq->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId,
                 TR_ELEM_CONT);


    BeginStruct("timingInfo(NhuTimingInfo)", buf);

    LOG_ELEM ( "sfn ",
                     nhuDatReq->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo.sfn,
                 TR_ELEM_CONT);

    LOG_ELEM ( "subframe ",
                 nhuDatReq->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo.subframe,
                 TR_ELEM_LAST);

    EndStruct(buf);

#ifndef NHU_VER2 
    LOG_ELEM ( "tbsSize ",
                 nhuDatReq->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.tbsSize,
                 TR_ELEM_LAST);
#endif 
    EndStruct(buf);
    
    LOG_ELEM ( "cmnMsgCategory ",
                 nhuDatReq->sdu.ueMsg.msg.cmnMsg.cmnMsgCategory, 
                 TR_ELEM_CONT);

    /* Message details not printing */
    EndStruct(buf);

    EndStruct(buf);
  }

  EndStruct(buf);

  LOG_ELEM( "isUeCfgPres", 
                  nhuDatReq->sdu.isUeCfgPres, 
                  TR_ELEM_CONT);

  printNhuUeCfgInfo(&nhuDatReq->sdu.ueCfg, buf);

  EndStruct(buf);

  EndStruct(buf);

  return ;
}

/*
 * Fun  : printNhuUiNhuDatRsp
 *
 * Desc : prints the NhuUiNhuDatRsp structure used 
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuUiNhuDatRsp(NhuDatRspSdus *ptr, Buff *buf)
{
  BeginStruct("NhUiNhuDatRspSdus", buf);

  printNhuHdr(&ptr->hdr, buf);

  BeginStruct("sdu", buf);

  LOG_ELEM ( "msgCategory", 
                  ptr->sdu.msgCategory,
                  TR_ELEM_CONT);

  BeginStruct("m", buf);
  
  printNhuDlCcchMsg(&(ptr->sdu.m.dlCcchMsg), buf);
  
  printNhuDlDcchMsg(&(ptr->sdu.m.dlDcchMsg), buf);

  EndStruct(buf);

  LOG_ELEM( "isUeCfgPres", 
                  ptr->sdu.isUeCfgPres, 
                  TR_ELEM_CONT);

  printNhuUeCfgInfo(&ptr->sdu.ueCfg, buf);

  EndStruct(buf);

  EndStruct(buf);

  return ;
}

void printNhuRbCfgList(NhuRbCfgList *rbCfgList, Buff *buf)
{
   int i = 0;

   LOG_ELEM ( "numRbCfgInfo ", 
	   rbCfgList->numOfRbCfgInfo,
           TR_ELEM_CONT);
   
   BeginStruct("NhRbCfgInfo(rbCfgInfoList)", buf);

   for ( i = 0; i < (rbCfgList->numOfRbCfgInfo); i++)
   {
      LOG_ELEM ( "RbId ", 
                rbCfgList->rbCfgInfoList[i].rbId.rbId, 
                TR_ELEM_CONT);

      LOG_ELEM ( "RbTyp", 
                rbCfgList->rbCfgInfoList[i].rbId.rbTyp, 
                TR_ELEM_CONT);

      LOG_ELEM ( "logChType", 
                rbCfgList->rbCfgInfoList[i].lcId.logChType, 
                TR_ELEM_CONT);

      LOG_ELEM ( "lcId", 
                rbCfgList->rbCfgInfoList[i].lcId.lcId, 
                TR_ELEM_CONT);

      LOG_ELEM ( "rbCfgBitMsk ", 
                rbCfgList->rbCfgInfoList[i].rbCfgBitMsk,
                TR_ELEM_CONT);

     BeginStruct("NhuUlRbCfg(ulRbCfg)", buf);

     if ( rbCfgList->rbCfgInfoList[i].rbCfgBitMsk & NHU_RB_CFG_UL)
     {
        LOG_ELEM ( "ulRbBitMsk ", 
                    rbCfgList->rbCfgInfoList[i].ulRbCfg.ulRbBitMsk,
                    TR_ELEM_CONT);


       BeginStruct("NhuUlLogChMap(ulLogChMap)", buf);

       if ( rbCfgList->rbCfgInfoList[i].ulRbCfg.ulRbBitMsk &  NHU_RB_CFG_LOGCH_MAP_UL)
       {
          LOG_ELEM ( "TrChType ", 
                    rbCfgList->rbCfgInfoList[i].ulRbCfg.ulLogChMap.trChType, 
                    TR_ELEM_CONT);

          LOG_ELEM ( "LcChType ",
                    rbCfgList->rbCfgInfoList[i].ulRbCfg.ulLogChMap.lcCfgType, 
                    TR_ELEM_CONT);

          BeginStruct("NhuLogChCfg(lcCfg)", buf);

          LOG_ELEM ( "LogChCfgPres ",
                    rbCfgList->rbCfgInfoList[i].ulRbCfg.ulLogChMap.lcCfg.logChCfgpres, 
                    TR_ELEM_CONT);
                    
           LOG_ELEM ( "logicalChGrp ",
                     rbCfgList->rbCfgInfoList[i].ulRbCfg.ulLogChMap.lcCfg.logChGrp, 
                    TR_ELEM_LAST);

          EndStruct(buf);

       }

       EndStruct(buf);


       if ( rbCfgList->rbCfgInfoList[i].ulRbCfg.ulRbBitMsk &  NHU_RB_CFG_RLC_MODE_UL)
       {
         BeginStruct("NhuUlRlcMode(ulRlcMode)", buf);
            
         LOG_ELEM ( "rlcCfgType ",
                    rbCfgList->rbCfgInfoList[i].ulRbCfg.ulRlcMode.rlcCfgType, 
                    TR_ELEM_CONT);
                    
         LOG_ELEM ( "mode ",
                    rbCfgList->rbCfgInfoList[i].ulRbCfg.ulRlcMode.mode,
                    TR_ELEM_CONT);

         if ( rbCfgList->rbCfgInfoList[i].ulRbCfg.ulRlcMode.mode &  NHU_RLC_MODE_AM)
         {
            BeginStruct("NhuUlAmRlcModeInfo(amRlc)", buf);
         
            LOG_ELEM (
                    "Status Prohibit ",  
                    rbCfgList->rbCfgInfoList[i].ulRbCfg.ulRlcMode.m.amRlc.tStatusProh, 
                    TR_ELEM_CONT);

            LOG_ELEM (" T-Reordering  ",
                      rbCfgList->rbCfgInfoList[i].ulRbCfg.ulRlcMode.m.amRlc.tReordring,
                    TR_ELEM_LAST);

            EndStruct(buf);
         } 
         else if ( rbCfgList->rbCfgInfoList[i].ulRbCfg.ulRlcMode.mode &  NHU_RLC_MODE_UM)
         {
            BeginStruct("NhuUlUmRlcModeInfo(umRlc)", buf);

            LOG_ELEM ( "Sequence Field length ", 
                     rbCfgList->rbCfgInfoList[i].ulRbCfg.ulRlcMode.m.umRlc.snFieldLen, 
                    TR_ELEM_CONT);

            LOG_ELEM ( "T-Reordering  ",
                     rbCfgList->rbCfgInfoList[i].ulRbCfg.ulRlcMode.m.umRlc.tReordring,
                    TR_ELEM_LAST);

            EndStruct(buf);
         }

         EndStruct(buf);
       }

     }
  
     EndStruct(buf);

     BeginStruct("NhuDlRbCfg(dlRbCfg)", buf);

     if (rbCfgList->rbCfgInfoList[i].rbCfgBitMsk & NHU_RB_CFG_DL)
     {
        LOG_ELEM ( "dlRbBitMsk ",  rbCfgList->rbCfgInfoList[i].dlRbCfg.dlRbBitMsk,
                    TR_ELEM_CONT);

        BeginStruct("NhuDlLogChMap(dlLogChMap)", buf);

        if ( rbCfgList->rbCfgInfoList[i].dlRbCfg.dlRbBitMsk &  NHU_RB_CFG_LOGCH_MAP_DL)
        {
           LOG_ELEM ( 
                    "TrChType ",
                    rbCfgList->rbCfgInfoList[i].dlRbCfg.dlLogChMap.trChType,
                    TR_ELEM_LAST);
        }

        EndStruct(buf);

        BeginStruct("NhuDlRlcMode(dlRlcMode)", buf);

        if ( rbCfgList->rbCfgInfoList[i].dlRbCfg.dlRbBitMsk &  NHU_RB_CFG_RLC_MODE_DL)
        {
           LOG_ELEM ( "rlcCfgType ",
                    rbCfgList->rbCfgInfoList[i].dlRbCfg.dlRlcMode.rlcCfgType, 
                    TR_ELEM_CONT);
                         
           LOG_ELEM ( "mode ", 
                    rbCfgList->rbCfgInfoList[i].dlRbCfg.dlRlcMode.mode,
                    TR_ELEM_CONT);

           if ( rbCfgList->rbCfgInfoList[i].dlRbCfg.dlRlcMode.mode &  NHU_RLC_MODE_AM)
           {
               BeginStruct("NhuDlAmRlcModeInfo(amRlc)", buf);

               LOG_ELEM (
                            "MAX Retransmissions thresold  ", 
                            rbCfgList->rbCfgInfoList[i].dlRbCfg.dlRlcMode.m.amRlc.maxRetxThresh, 
                            TR_ELEM_CONT);
               
               LOG_ELEM (
                            "Poll Byte ", 
                            rbCfgList->rbCfgInfoList[i].dlRbCfg.dlRlcMode.m.amRlc.pollByte, 
                            TR_ELEM_CONT);
               
               LOG_ELEM (
                            "Poll PDU",  
                            rbCfgList->rbCfgInfoList[i].dlRbCfg.dlRlcMode.m.amRlc.pollPdu, 
                            TR_ELEM_CONT);

               LOG_ELEM (
                            "Poll retransmission ",
                            rbCfgList->rbCfgInfoList[i].dlRbCfg.dlRlcMode.m.amRlc.tPollRetx,
                            TR_ELEM_LAST);
        
               EndStruct(buf);

           } 
           else if ( rbCfgList->rbCfgInfoList[i].dlRbCfg.dlRlcMode.mode &  NHU_RLC_MODE_UM)
           {
              BeginStruct("NhuDlUmRlcModeInfo(umRlc)", buf);

              LOG_ELEM (
                            "SeqFieldLen ",
                            rbCfgList->rbCfgInfoList[i].dlRbCfg.dlRlcMode.m.umRlc.snFieldLen,
                            TR_ELEM_LAST);

              EndStruct(buf);
           }
           else if ( rbCfgList->rbCfgInfoList[i].dlRbCfg.dlRlcMode.mode &  NHU_RLC_MODE_TM)
           {
              BeginStruct("NhuDlTmRlcModeInfo(tmRlc)", buf);

              LOG_ELEM (
                            "bufSize ",
                            rbCfgList->rbCfgInfoList[i].dlRbCfg.dlRlcMode.m.tmRlc.bufferSize,
                            TR_ELEM_LAST);

              EndStruct(buf);

           }
                 
        }
        EndStruct(buf);
     }

     EndStruct(buf);

     BeginStruct("NhuPdcpCfg(pdcpCfg)", buf);

     if ( rbCfgList->rbCfgInfoList[i].rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO)
     {
         LOG_ELEM (
                     "Integrity Required  ", 
                     rbCfgList->rbCfgInfoList[i].pdcpCfg.intgReqd, 
                            TR_ELEM_CONT);
         LOG_ELEM (
                     "PDCPCfgType ",
                     rbCfgList->rbCfgInfoList[i].pdcpCfg.pdcpCfgType, 
                            TR_ELEM_CONT);
         LOG_ELEM (
                     "PdcpCfgBitMsk ",
                     rbCfgList->rbCfgInfoList[i].pdcpCfg.pdcpCfgBitMsk, 
                            TR_ELEM_CONT);
         LOG_ELEM (
                     "Discard Tmr ",
                     rbCfgList->rbCfgInfoList[i].pdcpCfg.discardTmr, 
                            TR_ELEM_CONT);
         LOG_ELEM (
                     "Flush Timer ",
                     rbCfgList->rbCfgInfoList[i].pdcpCfg.flushTmr, 
                            TR_ELEM_CONT);
         LOG_ELEM (
                     "SeqNumSize ", 
                     rbCfgList->rbCfgInfoList[i].pdcpCfg.snSize,
                            TR_ELEM_CONT);
         LOG_ELEM (
                     "Status Report Required ",
                     rbCfgList->rbCfgInfoList[i].pdcpCfg.statusRptReq,
                            TR_ELEM_CONT);
     
         BeginStruct("NhuPdcpHdrComprInfo(hdrComprInfo)", buf);

         BeginStruct("NhuPdcpRohcInfo(rohcInfo)", buf);
    
         /* Profile not displayed */
     
         LOG_ELEM (
                     "ROHC MAX CID ", 
                     rbCfgList->rbCfgInfoList[i].pdcpCfg.hdrComprInfo.rohcInfo.maxCid,
                     TR_ELEM_LAST);
    
         EndStruct(buf);
     
         EndStruct(buf);
    
         BeginStruct("NhuPdcpCfg(pdcpCfg)", buf);
   
         LOG_ELEM (
                     "HoPresent ", 
                     rbCfgList->rbCfgInfoList[i].pdcpCfg.hoPdcpSduStaInfo.isHoPres, 
                     TR_ELEM_CONT);

         LOG_ELEM (
                     "HoPDCPSDUUlCnt ",  
                     rbCfgList->rbCfgInfoList[i].pdcpCfg.hoPdcpSduStaInfo.ulCountVal, 
                     TR_ELEM_CONT);

         LOG_ELEM (
                     "HoPDCPSDUDlCnt ", 
                     rbCfgList->rbCfgInfoList[i].pdcpCfg.hoPdcpSduStaInfo.dlCountVal,
                     TR_ELEM_CONT);

         LOG_ELEM (
                     "NumULBitMAPSize ", 
                     rbCfgList->rbCfgInfoList[i].pdcpCfg.hoPdcpSduStaInfo.numBits,
                     TR_ELEM_LAST);
     
         EndStruct(buf);
     }
     
     EndStruct(buf);

   } /* For loop */
   
   EndStruct(buf);

   return ;
}

/*
 * Fun  : printNhUiNhuCnclUeReq 
 *
 * Desc : prints the NhUiNhuCnclUeReq structure used 
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhUiNhuCnclUeReq ( NhuCellId cellId, NhuCrnti crnti, Buff *buf)
{
   BeginStruct("NhuCellId", buf);
   
   LOG_ELEM("CellId ", 
                   cellId, 
                   TR_ELEM_LAST);
   
   EndStruct(buf);

   BeginStruct("NhuCrnti", buf);
   
   LOG_ELEM("Crnti ", 
                   crnti, 
                   TR_ELEM_LAST);
   
   EndStruct(buf);

   return ;
}

/*
 * Fun  : printNhuUiNhuDeleteTransReq
 *
 * Desc : prints the Delete transaction request with CRNTI, transId, CellId
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuUiNhuDeleteTransReq(NhuCellId cellId, NhuCrnti crnti, U32 transId, Buff *buf)
{
   LOG_ELEM("cellId", 
                   cellId, 
                   TR_ELEM_CONT);

   LOG_ELEM("crnti", 
                   crnti, 
                   TR_ELEM_CONT);

   LOG_ELEM("transId", 
                   transId, 
                   TR_ELEM_LAST);

   return ;
}

/*
 * Fun  : printNhUiNhuDatCfm
 *
 * Desc : prints the NhuDatCfmSdus structure
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhUiNhuDatCfm(NhuDatCfmSdus *ptr, Buff *buf)
{
   BeginStruct("NhuDatCfmSdus", buf);

   printNhuHdr(&ptr->hdr, buf);

   LOG_ELEM("pdcpId", 
                   ptr->pdcpId, 
                   TR_ELEM_CONT);

   LOG_ELEM("datCfmStatus", 
                   ptr->datCfmStatus, 
                   TR_ELEM_LAST);

   EndStruct(buf);

  return ;
}

/*
 * Fun  : printNhuErrorInfo
 *
 * Desc : prints the NhuErrorInfo structure
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuErrorInfo(NhuErrorInfo *ptr, Buff *buf)
{
   BeginStruct("NhuErrorInfo", buf);

   LOG_ELEM("errPres", 
                   ptr->errPres, 
                   TR_ELEM_CONT);

   LOG_ELEM("errType", 
                   ptr->errType, 
                   TR_ELEM_CONT);

   LOG_ELEM("errCause", 
                   ptr->errCause, 
                   TR_ELEM_CONT);

   EndStruct(buf);

   return ;
}

/*
 * Fun  : printNhuStaIndSdus
 *
 * Desc : prints the NhuStaIndSdus structure
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuStaIndSdus(NhuStaIndSdus *ptr, Buff *buf)
{
   BeginStruct("NhuStaIndSdus", buf);

   printNhuHdr(&ptr->hdr, buf);

   LOG_ELEM("pdcpId", 
                   ptr->pdcpId, 
                   TR_ELEM_CONT);

   LOG_ELEM("cause", 
                   ptr->cause, 
                   TR_ELEM_LAST);

   EndStruct(buf);

  return ;
}

/*
 * Fun  : printNhuEncReqSdus
 *
 * Desc : prints the NhuEncReqSdus structure
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuEncReqSdus(NhuEncReqSdus *ptr, Buff *buf)
{
   BeginStruct("NhuEncReqSdus", buf);

   printNhuHdr(&ptr->hdr, buf);

   BeginStruct("sdu", buf);

   LOG_ELEM("msgCategory", 
                   ptr->sdu.msgCategory, 
                   TR_ELEM_CONT);

   BeginStruct("u", buf);

   if ( ptr->sdu.msgCategory == NHU_MSG_DLDCCH)
   {
      BeginStruct("NhuDL_DCCH_Msg", buf);

      LOG_ELEM("MsgType ", 
                   ptr->sdu.u.dcchMsg.message.val.c1.choice.val,
                   TR_ELEM_CONT);

      EndStruct(buf);
   }

   if ( ptr->sdu.msgCategory == NHU_MSG_HOCMD)
   {
      BeginStruct("NhuHovrCmd", buf);

      EndStruct(buf);
   }

   if ( ptr->sdu.msgCategory == NHU_MSG_UERACAPINFO)
   {
      BeginStruct("NhuUERadioAccessCapInfo", buf);

      EndStruct(buf);
   }

   if ( ptr->sdu.msgCategory == NHU_MSG_UECAPINFO)
   {
      BeginStruct("NhuUECapInfo", buf);

      EndStruct(buf);
   }
   
   if (ptr->sdu.msgCategory == NHU_MSG_HOPREPINFO)
   {
      BeginStruct("NhuHovrPrepInfo", buf);

      EndStruct(buf);
   }
   
#ifdef NHU_VER2
   if (ptr->sdu.msgCategory == NHU_MSG_VARSHORTMACI)
   {
      BeginStruct("NhuVarShortMAC_Input", buf);

      EndStruct(buf);
   }
#endif

   EndStruct(buf);

   return ;
}

/*
 * Fun  : printNhuEncCfmSdus
 *
 * Desc : prints the NhuEncCfmSdus structure
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuEncCfmSdus(NhuEncCfmSdus *ptr, Buff *buf)
{
   BeginStruct("NhuEncCfmSdus", buf);

   printNhuHdr(&ptr->hdr, buf);

   BeginStruct("sdu", buf);

   LOG_ELEM("msgCategory", 
                   ptr->sdu.msgCategory,
                   TR_ELEM_CONT);

   LOG_ELEM("msgLen", 
                   ptr->sdu.msgLen,
                   TR_ELEM_CONT);

   LOG_ELEM("status", 
                   ptr->sdu.status,
                   TR_ELEM_CONT);
   /* mBuf */
   EndStruct(buf);

   EndStruct(buf);
 
   return ;
}

/*
 * Fun  : printNhuPdcpSduStaReq
 *
 * Desc : prints the NhuHdr for the PDCP Sdu Status Request 
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuPdcpSduStaReq(NhuHdr *ptr, Buff *buf)
{
  printNhuHdr(ptr, buf);

  return ;
}

/*
 * Fun  : printNhuPdcpSduStaCfm
 *
 * Desc : prints the NhuPdcpSduStaCfm structure
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuPdcpSduStaCfm(NhuPdcpSduStaCfm *ptr, Buff *buf)
{
   int i  = 0;

   BeginStruct("NhuPdcpSduStaCfm", buf);

   printNhuHdr(&ptr->hdr, buf);

   LOG_ELEM("numRbId", 
                   ptr->numRbId, 
                   TR_ELEM_CONT);

   while ( i < ptr->numRbId )
   {
      BeginStruct("NhuPdcpSduStaInfo", buf);

      LOG_ELEM("drbId", 
                   ptr->pdcpSduStaInfo[i].drbId,
                   TR_ELEM_CONT);

      LOG_ELEM("dir", 
                   ptr->pdcpSduStaInfo[i].dir,
                   TR_ELEM_CONT);

      LOG_ELEM("ulCountVal", 
                   ptr->pdcpSduStaInfo[i].ulCountVal,
                   TR_ELEM_CONT);

      LOG_ELEM("dlCountVal", 
                   ptr->pdcpSduStaInfo[i].dlCountVal,
                   TR_ELEM_CONT);

      BeginStruct("NhuRcvStaOfUlPdcpSdus", buf);

      LOG_ELEM("numBits", 
                   ptr->pdcpSduStaInfo[i].rcvStaOfUlPdcpSdus.numBits,
                   TR_ELEM_CONT);

      EndStruct(buf);

      EndStruct(buf);

      i++;
   }

   LOG_ELEM("status", 
                   ptr->status, 
                   TR_ELEM_CONT);

   LOG_ELEM("reason", 
                   ptr->reason, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return;
}

/*
 * Fun  : printNhuDecodeReq
 *
 * Desc : prints the NhuDecodeSduReq structure
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuDecodeReq(NhuDecodeSduReq *ptr, Buff *buf)
{
   BeginStruct("NhuDecodeReq", buf);

   printNhuHdr(&ptr->hdr, buf);

   LOG_ELEM("sduType", 
                   ptr->sduType, 
                   TR_ELEM_CONT);
   /* mbufer */

   EndStruct(buf);
   
   return ;
}

/*
 * Fun  : printNhuDecodeCfm
 *
 * Desc : prints the NhuDecodeSduCfm structure
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuDecodeCfm(NhuDecodeSduCfm *ptr, Buff *buf)
{
   BeginStruct("NhuDecodeCfm", buf);

   printNhuHdr(&ptr->hdr, buf);

   LOG_ELEM("sduType", 
                   ptr->sduType, 
                   TR_ELEM_CONT);

   LOG_ELEM("status", 
                   ptr->status, 
                   TR_ELEM_CONT);

   BeginStruct("ie", buf);

   BeginStruct("NhuUe_EUTRA_Cap", buf);

   /* ASN Format */

   EndStruct(buf);

   BeginStruct("NhuDecode3GUECapOctStr", buf);

   LOG_ELEM ( "containsUECap",
                   ptr->ie.nhuUTRAUECapOctStr.containsUECap, 
                   TR_ELEM_CONT); 

   /* ASN Formats */

   EndStruct(buf);

   BeginStruct("NhuHovrCmd", buf);

   /* ASN Formats */

   EndStruct(buf);

   BeginStruct("NhuUECapInfo", buf);

   /* ASN Formats */

   EndStruct(buf);

   BeginStruct("NhuHovrPrepInfo", buf);

   /* ASN Formats */

   EndStruct(buf);

   BeginStruct("NhuUERadioAccessCapInfo", buf);

   /* ASN Formats */

   EndStruct(buf);

   EndStruct(buf);

   EndStruct(buf);
   
   return ;
}

/*
 * Fun  : printNhuCountReq
 *
 * Desc : prints the NhuHdr for the count req 
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuCountReq(NhuHdr *ptr, Buff *buf)
{
   BeginStruct("NhuCountReq", buf);

   printNhuHdr(ptr, buf);

   EndStruct(buf);
   
   return ;
}

/*
 * Fun  : printNhuCountCfm
 *
 * Desc : prints the NhuDRBCountInfoLst structure
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuCountCfm(NhuDRBCountInfoLst *ptr, Buff *buf)
{
   int i = 0; 

   BeginStruct("NhuDRBCountInfoLst", buf);

   printNhuHdr(&ptr->countHdr, buf);

   LOG_ELEM("numDrbs", 
                   ptr->numDrbs, 
                   TR_ELEM_CONT);

   LOG_ELEM("status", 
                   ptr->status, 
                   TR_ELEM_CONT);

   while( i < ptr->numDrbs )
   {
      BeginStruct("NhuDrbCountInfo", buf);

      LOG_ELEM("drbId", 
                      ptr->member[i].drbId, 
                      TR_ELEM_CONT);

      LOG_ELEM("drbDirection", 
                      ptr->member[i].drbDirection, 
                      TR_ELEM_CONT);

      LOG_ELEM("ulCount", 
                      ptr->member[i].ulCount, 
                      TR_ELEM_CONT);

      LOG_ELEM("dlCount", 
                      ptr->member[i].dlCount, 
                      TR_ELEM_LAST);

      EndStruct(buf);

      i++;
   }
      
   EndStruct(buf);

  return ;
}

/* RRC to RRC User */
/*
 * Fun  : printNhuDatInd
 *
 * Desc : prints the NhuDatIndSdus structure
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuDatInd(NhuDatIndSdus *ptr, Buff *buf)
{
   BeginStruct("NhuDatIndSdus", buf);

   printNhuHdr(&ptr->hdr, buf);

   printNhuErrorInfo(&ptr->err, buf);

   BeginStruct("sdu", buf);

   LOG_ELEM("msgCategory", 
                   ptr->sdu.msgCategory,
                   TR_ELEM_CONT);

   BeginStruct("m", buf);

   if (ptr->sdu.msgCategory == NHU_MSG_DLCCCH)
   {
      printNhuUlCcchMsg(&ptr->sdu.m.ulCcchMsg, buf);
   }

   if (ptr->sdu.msgCategory == NHU_MSG_DLDCCH)
   {
      printNhuUlDcchMsg(&ptr->sdu.m.ulDcchMsg, buf);
   }

   EndStruct(buf);
   
   EndStruct(buf);

   EndStruct(buf);

   return ;
}

/*
 * Fun  : printNhuErrIndSdus
 *
 * Desc : prints the NhuErrIndSdus structure
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuErrIndSdus(NhuErrIndSdus *ptr, Buff *buf)
{
   BeginStruct("NhuErrIndSdus", buf);

   printNhuHdr(&ptr->hdr, buf);

   printNhuErrorInfo(&ptr->sdu, buf);

   EndStruct(buf);

  return ;
}

/*
 * Fun  : printNhuCfgCfmSdus
 *
 * Desc : prints the NhuCfgCfmSdus structure
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuCfgCfmSdus(NhuCfgCfmSdus *ptr, Buff *buf)
{
   BeginStruct("NhuCfgCfmSdus", buf);

   printNhuHdr(&ptr->hdr, buf);
   
   BeginStruct("sdu", buf);

   LOG_ELEM ( "isCfgSuccess", 
                   ptr->sdu.isCfgSuccess, 
                   TR_ELEM_CONT);

   LOG_ELEM ( "cfgError", 
                   ptr->sdu.isCfgSuccess, 
                   TR_ELEM_LAST);
   EndStruct(buf);

   EndStruct(buf);

   return ;
}

/*
 * Fun  : printNhuCellCfgCfmSdus
 *
 * Desc : prints the NhuCellCfgCfmSdus structure
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuCellCfgCfmSdus(NhuCellCfgCfmSdus *ptr, Buff *buf)
{
   BeginStruct("NhuCfgCfmSdus", buf);

   printNhuHdr(&ptr->hdr, buf);
   
   BeginStruct("sdu", buf);

   LOG_ELEM ( "isCfgSuccess", 
                   ptr->sdu.isCfgSuccess, 
                   TR_ELEM_CONT);

   LOG_ELEM ( "cfgError", 
                   ptr->sdu.isCfgSuccess, 
                   TR_ELEM_LAST);
   EndStruct(buf);

   EndStruct(buf);

   return ;
}

/*
 * Fun  : printNhuCmnMacCfg
 *
 * Desc : prints the NhuCmnMacCfg structure
 *        between RRM and RRC interface.  
 *
 * Ret : void.
 */
void printNhuCmnMacCfg(NhuCmnMacCfg *macCfg, Buff *buf)
{
  LOG_NhuRachCfg(buf, (&macCfg->rachCfg), TR_ELEM_LAST);

  return ;
}

#ifdef RRC_PHY_CFG_SUPP
void printNhuCmnSrsUlCfg(NhuCmnSrsUlCfg *ptr, Buff *buf)
{
   BeginStruct("NhuCmnSrsUlCfg", buf);

   LOG_ELEM("srsCfgType", 
                   ptr->srsCfgType, 
                   TR_ELEM_CONT);

   BeginStruct("setup", buf);

   LOG_ELEM("bwCfg", 
                   ptr->setup.bwCfg, 
                   TR_ELEM_CONT);

   LOG_ELEM("subfrmCfg", 
                   ptr->setup.subfrmCfg, 
                   TR_ELEM_CONT);

   LOG_ELEM("srsANSimultTx", 
                   ptr->setup.srsANSimultTx, 
                   TR_ELEM_CONT);

   LOG_ELEM("srsMaxUpPts", 
                   ptr->setup.srsMaxUpPts, 
                   TR_ELEM_CONT);

   EndStruct(buf);

   EndStruct(buf);

   return ;

}

void printNhuCellIdCfg(NhuPhyCellIdCfg *ptr, Buff *buf)
{
   BeginStruct("NhuPhyCellIdCfg", buf);

   LOG_ELEM("cellIdGrpId", 
                   ptr->cellIdGrpId, 
                   TR_ELEM_CONT);

   LOG_ELEM("physCellId", 
                   ptr->physCellId, 
                   TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

void printNhuCmnPhyCfg(NhuCmnPhyCfg *ptr, Buff *buf)
{
   BeginStruct("NhuCmnPhyCfg", buf);

   printNhuCellIdCfg(&ptr->pcellCfg, buf); 

   LOG_ELEM("numOfTxAnt ", 
                   ptr->numOfTxAnt, 
                   TR_ELEM_CONT);

   LOG_ELEM("eUtraBand ", 
                   ptr->eUtraBand, 
                   TR_ELEM_CONT);

   LOG_NhuTxSchmCfg(buf, (&ptr->schmCfg), TR_ELEM_CONT);

   LOG_ELEM("bitmask ", 
                   ptr->bitmask, 
                   TR_ELEM_CONT);

   LOG_NhuPrachCfg(buf, (&ptr->prachCfg), TR_ELEM_CONT);

   LOG_NhuCmnPdschCfg(buf, (&ptr->cmnPdschCfg), TR_ELEM_CONT);

   LOG_NhuCmnPuschCfg(buf, (&ptr->cmnPuschCfg), TR_ELEM_CONT);

   LOG_NhuPhichCfg(buf, (&ptr->cmnPhichCfg), TR_ELEM_CONT);

   LOG_NhuPucchCfg(buf, (&ptr->cmnPucchCfg), TR_ELEM_CONT);

   printNhuCmnSrsUlCfg(&ptr->srsUlCfg, buf);

   LOG_NhuTddSfCfgInfo((&ptr->tddSfCfg), buf, flag);

   EndStruct(buf);

   return ;
}
#endif 

void printNhuDlDcchMsg(NhuDlDcchMsg *dlDcchMsg, Buff *buf)
{
    BeginStruct("dlCcchMsg(NhuDlDcchMsg)", buf);
      
    LOG_ELEM ( "PdcpId ", 
              dlDcchMsg->pdcpId, 
         		  TR_ELEM_CONT);

    LOG_ELEM ("isMsgEncoded ", 
		          dlDcchMsg->isMsgEncoded, 
              TR_ELEM_CONT);

    BeginStruct("m", buf);

    BeginStruct("dcchMsg(NhuDL_DCCH_Msg)", buf);
    
    BeginStruct("message(NhuDL_DCCH_MsgTyp)", buf);
    
    BeginStruct("val", buf);
    
    BeginStruct("c1(NhuDL_DCCH_MsgTypc1)", buf);
    
    LOG_ELEM ( "Message type ",
                  dlDcchMsg->m.dcchMsg.message.val.c1.choice.val,  TR_ELEM_LAST);
    
    EndStruct(buf);
    
    EndStruct(buf);
    
    EndStruct(buf);
    
    EndStruct(buf);
    
    EndStruct(buf);
    
    LOG_MSG( "Message is encoded ");
    /* Encoded buf not printing */ 

    EndStruct(buf);

    return ;
}

void printNhuDlCcchMsg(NhuDlCcchMsg *ptr, Buff *buf)
{
  BeginStruct("dlCcchMsg", buf);

  LOG_ELEM ( "tmpCrnti", 
                  ptr->tmpCrnti,
                  TR_ELEM_CONT);

  LOG_ELEM ( "rbId", 
                  ptr->rbId,
                  TR_ELEM_CONT);

  LOG_ELEM( "ccchMsg", 
                  ptr->ccchMsg.message.val.c1.choice.val, 
                  TR_ELEM_CONT);
 
  EndStruct(buf);
 
  return ;
}

void printNhuUlCcchMsg(NhuUlCcchMsg *ptr, Buff *buf)
{
   BeginStruct("nhuUlCcchMsg", buf);

   LOG_ELEM("rbId", 
                   ptr->rbId,
                   TR_ELEM_CONT);

   LOG_ELEM("tempCrnti", 
                   ptr->tmpCrnti,
                   TR_ELEM_CONT);

   BeginStruct("NhuUlCcchMsg", buf);

   /* ASN Format */

   EndStruct(buf);

   EndStruct(buf);
 
  return ;
}

void printNhuUlDcchMsg(NhuUlDcchMsg *ptr, Buff *buf)
{
   BeginStruct("nhuUlDcchMsg", buf);

   LOG_ELEM("pdcpId", 
                   ptr->pdcpId,
                   TR_ELEM_CONT);
   BeginStruct("NhuUl_Dcch_Msg", buf);

   /* ASN Format */

   EndStruct(buf);

   EndStruct(buf);
 
  return ;
}

#ifdef RRC_PHY_CFG_SUPP
void printNhuCmnPhyRecfg(NhuCmnPhyRecfg *ptr,Buff *buf)
{
   BeginStruct("NhuCmnPhyRecfg", buf);
  
   LOG_CmLteTimingInfo(ptr->actvTime, buf);

   LOG_ELEM("bitmask ", 
                   ptr->bitmask, 
                   TR_ELEM_CONT);

   LOG_NhuPrachCfg(buf, (&ptr->prachCfg), TR_ELEM_CONT);

   LOG_NhuCmnPdschCfg(buf, (&ptr->cmnPdschCfg), TR_ELEM_CONT);

   LOG_NhuCmnPuschCfg(buf, (&ptr->cmnPuschCfg), TR_ELEM_CONT);

   LOG_NhuPhichCfg(buf, (&ptr->cmnPhichCfg), TR_ELEM_CONT);

   LOG_NhuPucchCfg(buf, (&ptr->cmnPucchCfg), TR_ELEM_CONT);

   printNhuCmnSrsUlCfg(&ptr->srsUlCfg, buf);

   LOG_NhuTddSfCfgInfo((&ptr->tddSfCfg), buf, flag);

   EndStruct(buf);

   return ;
}
#endif 

#ifdef NHU_MULT_SAP_SUPPRT
void printNhuLLyrSapIdInfo(NhuLLyrSapIdInfo *ptr, Buff *buf)
{
   LOG_SuId(buf, ptr->kwuSapId, "kwuSapId");
   LOG_SuId(buf, ptr->ckwSapId, "ckwSapId");
   LOG_SuId(buf, ptr->crgSapId, "crgSapId");
   LOG_SuId(buf, ptr->cpjSapId, "cpjSapId");
   LOG_SuId(buf, ptr->pjuSapId, "pjuSapId");
#ifdef RRC_PHY_CFG_SUPP
   LOG_SuId(buf, ptr->ctfSapId, "ctfSapId");
#endif

   return ;
}
#endif

void printNhuCellCfgInfo(NhuCellCfgInfo *ptr, Buff *buf)
{
   BeginStruct("NhuCellCfgInfo", buf);
   
   LOG_ELEM("duplexMode" , 
                   ptr->duplexMode, 
                   TR_ELEM_CONT);
   
   BeginStruct("NhuSysBwCfg", buf);
        
   LOG_ELEM("ulSysBw", 
                   ptr->sysBwCfg.ulSysBw, TR_ELEM_CONT);
        
   LOG_ELEM("dlSysBw", 
                   ptr->sysBwCfg.dlSysBw, TR_ELEM_LAST);
        
   EndStruct(buf);
        
   LOG_ELEM("cellCfgBitMsk ", 
                   ptr->cellCfgBitMsk, TR_ELEM_LAST);
        
   BeginStruct("NhuRbCfgList", buf);
        
   printNhuRbCfgList(&ptr->rbToAddCfgList, buf);
        
   EndStruct(buf);
        
   BeginStruct("NhuCmnMacCfg", buf);

   printNhuCmnMacCfg(&ptr->macCmnCfg, buf);

   EndStruct(buf);
        
#ifdef RRC_PHY_CFG_SUPP

   printNhuCmnPhyCfg(&ptr->phyCellCfg, buf); 
   
#endif

#ifdef NHU_MULT_SAP_SUPPORT

   BeginStruct("NhuLLyrSapIdInfo", buf);

   printNhuLLyrSapIdInfo(&ptr->llyrSapIdInfo, buf); 

   EndStruct(buf);

#endif
   
   EndStruct(buf);

   return ;

}

void printCellReCfgInfo(NhuCellReCfgInfo *reCfgReq, Buff *buf)
{
   BeginStruct("NhuCellReCfgInfo", buf);
   
   LOG_CmLteTimingInfo(reCfgReq->actvnTime, buf);

   LOG_ELEM("cellCfgBitMsk", 
                   reCfgReq->cellCfgBitMsk, 
                   TR_ELEM_CONT);

   BeginStruct("NhuCmnMacCfg", buf);

   printNhuCmnMacCfg(&reCfgReq->macCmnCfg, buf);

   EndStruct(buf);

#ifdef RRC_PHY_CFG_SUPP

   printNhuCmnPhyRecfg(&reCfgReq->phyCellCfg, buf);

#endif

   EndStruct(buf);
   
   return;
}

void printNhuCellRlsInfo(NhuCellRlsInfo *ptr,Buff *buf)
{
   BeginStruct("NhuCellRlsInfo", buf);
   
   LOG_ELEM ( "CellId ", 
              ptr->cellId, 
              TR_ELEM_LAST);

   EndStruct(buf);
   
   return;
}

void printNhuUeCfgInfo(NhuUeCfgInfo *ptr, Buff *buf)
{
   int i = 0; 

   BeginStruct("NhuUeCfgInfo", buf);

   LOG_ELEM ( "ConReCfgCause ", 
                   ptr->conReCfgCause, 
                   TR_ELEM_CONT);
		   
   LOG_ELEM ( "UeCfgBitMask ", 
                   ptr->ueCfgBitMsk, 
                   TR_ELEM_CONT);

   BeginStruct("NhuRbCfgAllList", buf);

   if (ptr->ueCfgBitMsk & NHU_EXP_CFG_RB_CFG)
   {
      LOG_ELEM ( "rbCfgBitMsk ", ptr->rbCfgAllList.rbCfgTypBitMsk,
               TR_ELEM_CONT);
 
      BeginStruct("NhuRbCfgList(rbToAddCfgList)", buf);

      if ( ptr->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_ADD )
      {
         printNhuRbCfgList(&ptr->rbCfgAllList.rbToAddCfgList, buf);
      }

      EndStruct(buf);

      BeginStruct("NhuRbCfgList(rbToModCfgList)", buf);
      
      if ( ptr->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_MODIFY)
      {
         printNhuRbCfgList( &ptr->rbCfgAllList.rbToModCfgList, buf);
      }
      
      EndStruct(buf);

      BeginStruct("NhuRbRlsCfgList(rbToRlsCfgList)", buf);
      
      if ( ptr->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_RLS)
      {

         LOG_ELEM ( "numRbCfg ", 
               ptr->rbCfgAllList.rbToRlsCfgList.numOfRbCfgInfo, 
               TR_ELEM_CONT);


         for ( i = 0; i < ptr->rbCfgAllList.rbToRlsCfgList.numOfRbCfgInfo; i++) 
         {
           BeginStruct("NhuRbId(rbsToRel)", buf);
           
           LOG_NhuRbId(ptr->rbCfgAllList.rbToRlsCfgList.rbsToRel[i].rbId, 
		                   ptr->rbCfgAllList.rbToRlsCfgList.rbsToRel[i].rbTyp);
           
      	   EndStruct(buf);
         }
         
      }
      
      EndStruct(buf);
   }

   EndStruct(buf);
   
   BeginStruct("NhuTrChCfg", buf);

   if (ptr->ueCfgBitMsk & NHU_EXP_CFG_TRCH_CFG)
   {
      LOG_ELEM ( "TrChCfgType ",
                 ptr->trChCfg.trChCfgType, 
                 TR_ELEM_CONT);

      LOG_ELEM ( "TrChBitMsk ",
                 ptr->trChCfg.trChBitMsk,
                 TR_ELEM_CONT);

      BeginStruct("NhuUlHarqCfg", buf);

      LOG_ELEM ( "UlMaxHarqTx ", 
                 ptr->trChCfg.ulHarqCfg.ulMaxHarqTx, 
                 TR_ELEM_CONT);

      LOG_ELEM ( "DeltaHarqOffset ",  
                  ptr->trChCfg.ulHarqCfg.deltaHarqOffset, 
                  TR_ELEM_LAST);

      EndStruct(buf);
   }

   EndStruct(buf);


   BeginStruct("NhuSecurityCfg", buf);

   if (ptr->ueCfgBitMsk & NHU_EXP_CFG_SEC_CFG)
   {
      LOG_ELEM ( "secCfgBitMsk ", 
	             ptr->secCfg.secCfgBitMsk, 
               TR_ELEM_CONT);

      BeginStruct("NhuCiphKeyAlgoInfo", buf);

      if ( ptr->secCfg.secCfgBitMsk & NHU_SEC_CIPHER_ALGO ) 
      {

         LOG_ELEM ( "Ciphering Algorithm ", 
                    ptr->secCfg.cipherCfgInfo.ciphAlgo,
                    TR_ELEM_CONT);
         
         LOG_ARRAY ( "CipherCPKey ", 
                    ptr->secCfg.cipherCfgInfo.cipherCpKey,
		                NHU_CIPH_KEY_LEN,
                    TR_ELEM_CONT);

         LOG_ARRAY ( "CipherUPKey ", 
                    ptr->secCfg.cipherCfgInfo.cipherUpKey,
                    NHU_CIPH_KEY_LEN,
                    TR_ELEM_LAST);
      }

      EndStruct(buf);

      BeginStruct("NhuIntgKeyAlgoInfo", buf);

      if ( ptr->secCfg.secCfgBitMsk & NHU_SEC_INTG_ALGO ) 
      {

          LOG_ELEM ( "Action ",
                    ptr->secCfg.intgCfgInfo.action, 
                    TR_ELEM_CONT);

          LOG_ELEM ( "IntgAlgo", 
                    ptr->secCfg.intgCfgInfo.intgAlgo, 
                    TR_ELEM_CONT);

          LOG_ARRAY ( "IntegrityKey ", 
                    ptr->secCfg.cipherCfgInfo.cipherUpKey,
                    NHU_CIPH_KEY_LEN,
                    TR_ELEM_LAST);
      }

      EndStruct(buf);
   }

   EndStruct(buf);

   BeginStruct("NhuNewUeInfo", buf);

   if (ptr->ueCfgBitMsk & NHU_EXP_CFG_UE_CFG)
   {
      LOG_ELEM ( "Crnti ", 
                 ptr->ueInfo.newCrnti,
                 TR_ELEM_LAST);
   }

   EndStruct(buf);

#ifdef RRC_PHY_CFG_SUPP
   BeginStruct("NhuPhyCfg", buf);

   LOG_ELEM ("bitMask", ptr->phyCfg.bitMask, TR_ELEM_CONT);

   LOG_CmLteUeCategory(ptr->phyCfg.ueCategory, buf, TR_ELEM_CONT);

   BeginStruct("NhuPdschCfg", buf);

   LOG_ELEM ("cfgTyp", ptr->phyCfg.pdschCfg.cfgTyp, TR_ELEM_CONT);

   LOG_ELEM ("pa", ptr->phyCfg.pdschCfg.pa, TR_ELEM_LAST);
   
   EndStruct(buf);

   BeginStruct("NhuDedPucchCfg", buf);

   LOG_ELEM ("cfgTyp", ptr->phyCfg.pucchCfg.cfgTyp, TR_ELEM_CONT);

   LOG_ELEM ("choice", ptr->phyCfg.pucchCfg.choice, TR_ELEM_CONT);

   LOG_ELEM ("factor", ptr->phyCfg.pucchCfg.setup.factor, TR_ELEM_CONT);

   LOG_ELEM ("n1PUCCH_AN_Rep", ptr->phyCfg.pucchCfg.setup.n1PUCCH_AN_Rep, TR_ELEM_LAST);
   
   EndStruct(buf);

   BeginStruct("NhuDedPuschCfg", buf);

   LOG_ELEM ("cfgTyp", ptr->phyCfg.puschCfg.cfgTyp, TR_ELEM_CONT);

   LOG_ELEM ("betaOffsetAckIdx", ptr->phyCfg.puschCfg.betaOffsetAckIdx, TR_ELEM_CONT);

   LOG_ELEM ("betaOffsetRiIdx", ptr->phyCfg.puschCfg.betaOffsetRiIdx, TR_ELEM_CONT);

   LOG_ELEM ("betaOffsetCqiIdx", ptr->phyCfg.puschCfg.betaOffsetCqiIdx, TR_ELEM_LAST);
   
   EndStruct(buf);

   BeginStruct("NhuCqiRptCfg", buf);

   LOG_ELEM ("cfgType", ptr->phyCfg.cqiCfg.cfgType, TR_ELEM_CONT);

#ifdef NHU_VER2
   BeginStruct("NhuCqiRptModeAprd", buf);

   LOG_ELEM ("pres", ptr->phyCfg.cqiCfg.reportMode.aPeriodicRpt.pres, TR_ELEM_CONT);

   LOG_ELEM ("aPeriodicRptMode", ptr->phyCfg.cqiCfg.reportMode.aPeriodicRpt.aPeriodicRptMode, TR_ELEM_LAST);
   
   EndStruct(buf);

   printNhuCqiRptModePeriodic(&ptr->phyCfg.cqiCfg.reportMode.periodicRpt, buf);

#else
   LOG_ELEM ("reportingMode", ptr->phyCfg.cqiCfg.reportingMode, TR_ELEM_CONT);
   
   BeginStruct("reportMode", buf);

   LOG_ELEM ("aPeriodicRpt", ptr->phyCfg.cqiCfg.reportMode.aPeriodicRpt, TR_ELEM_CONT);

   printNhuCqiRptModePeriodic(&ptr->phyCfg.cqiCfg.reportMode.periodicRpt, buf);

#ifdef NHU_VER1
   BeginStruct("NhuPmiRiRpt", buf);

   LOG_ELEM ("pres", ptr->phyCfg.cqiCfg.pmiRiRpt.pres, TR_ELEM_CONT);

   LOG_ELEM ("pmiRiRptSetup", ptr->phyCfg.cqiCfg.pmiRiRpt.pmiRiRptSetup, TR_ELEM_LAST);

   EndStruct(buf);
#endif
   EndStruct(buf);
   
   EndStruct(buf);
#endif 
   BeginStruct("NhuDedSRSUlCfg", buf);

   LOG_ELEM ("cfgTyp", ptr->phyCfg.srsUlCfg.cfgTyp, TR_ELEM_CONT);

   LOG_ELEM ("choice", ptr->phyCfg.srsUlCfg.choice, TR_ELEM_CONT);

   BeginStruct("setup", buf);

   LOG_ELEM ("srsBwCfg", ptr->phyCfg.srsUlCfg.setup.srsBwCfg, TR_ELEM_CONT);

   LOG_ELEM ("srsHopBwCfg", ptr->phyCfg.srsUlCfg.setup.srsHopBwCfg, TR_ELEM_CONT);

   LOG_ELEM ("freqDomPos", ptr->phyCfg.srsUlCfg.setup.freqDomPos, TR_ELEM_CONT);

   LOG_ELEM ("duration", ptr->phyCfg.srsUlCfg.setup.duration, TR_ELEM_CONT);

   LOG_ELEM ("srsCfgIndx", ptr->phyCfg.srsUlCfg.setup.srsCfgIndx, TR_ELEM_CONT);

   LOG_ELEM ("transComb", ptr->phyCfg.srsUlCfg.setup.transComb, TR_ELEM_CONT);

   LOG_ELEM ("cycleshft", ptr->phyCfg.srsUlCfg.setup.cycleshft, TR_ELEM_CONT);
   
   EndStruct(buf);
   
   EndStruct(buf);

   BeginStruct("NhuDedSRCfg", buf);

   LOG_ELEM ("cfgTyp", ptr->phyCfg.srCfg.cfgTyp, TR_ELEM_CONT);

   LOG_ELEM ("dedSRCfgType", ptr->phyCfg.srCfg.dedSRCfgType, TR_ELEM_CONT);

   BeginStruct("setup", buf);

   LOG_ELEM ("srPUCCHRi", ptr->phyCfg.srCfg.setup.srPUCCHRi, TR_ELEM_CONT);

   LOG_ELEM ("srCfgIdx", ptr->phyCfg.srCfg.setup.srCfgIdx, TR_ELEM_LAST);
   
   EndStruct(buf);

   BeginStruct("NhuDedAntCfgInfo", buf);

   LOG_ELEM ("cfgTyp", ptr->phyCfg.antCfg.cfgTyp, TR_ELEM_CONT);

   LOG_ELEM ("txMode", ptr->phyCfg.antCfg.txMode, TR_ELEM_CONT);

   LOG_ELEM ("txAntSelectchoice", ptr->phyCfg.antCfg.txAntSelectchoice, TR_ELEM_CONT);

   LOG_ELEM ("txAntSelect", ptr->phyCfg.antCfg.txAntSelect, TR_ELEM_LAST);
   
   EndStruct(buf);

#ifdef NHU_VER2
   BeginStruct("NhuDedAntCodeBkSubsetRes", buf);

   LOG_ELEM ("nhuCdBkRestType", ptr->phyCfg.antCfg.codeBookSubsetRes.nhuCdBkRestType, TR_ELEM_CONT);

   switch(ptr->phyCfg.antCfg.codeBookSubsetRes.nhuCdBkRestType)
   {
      case N2TX_ANTENNA_TM3:
        LOG_ELEM ("n2TxAntTm3", ptr->phyCfg.antCfg.codeBookSubsetRes.codeBookSubsetRestn.n2TxAntTm3, TR_ELEM_LAST);
        break;

      case N4TX_ANTENNA_TM3:
        LOG_ELEM ("n4TxAntTm3", ptr->phyCfg.antCfg.codeBookSubsetRes.codeBookSubsetRestn.n4TxAntTm3, TR_ELEM_LAST);
        break;

      case N2TX_ANTENNA_TM4:
        LOG_ELEM ("n2TxAntTm4", ptr->phyCfg.antCfg.codeBookSubsetRes.codeBookSubsetRestn.n2TxAntTm4, TR_ELEM_LAST);
        break;

      case N4TX_ANTENNA_TM4:
        LOG_ARRAY ("n4TxAntTm4", ptr->phyCfg.antCfg.codeBookSubsetRes.codeBookSubsetRestn.n4TxAntTm4,8, TR_ELEM_LAST);
        break;

      case N2TX_ANTENNA_TM5:
        LOG_ELEM ("n2TxAntTm5", ptr->phyCfg.antCfg.codeBookSubsetRes.codeBookSubsetRestn.n2TxAntTm5, TR_ELEM_LAST);
        break;

      case N4TX_ANTENNA_TM5:
        LOG_ELEM ("n4TxAntTm5", ptr->phyCfg.antCfg.codeBookSubsetRes.codeBookSubsetRestn.n4TxAntTm5, TR_ELEM_LAST);
        break;

      case N2TX_ANTENNA_TM6:
        LOG_ELEM ("n2TxAntTm6", ptr->phyCfg.antCfg.codeBookSubsetRes.codeBookSubsetRestn.n2TxAntTm6, TR_ELEM_LAST);
        break;

      case N4TX_ANTENNA_TM6:
        LOG_ELEM ("n4TxAntTm6", ptr->phyCfg.antCfg.codeBookSubsetRes.codeBookSubsetRestn.n4TxAntTm6, TR_ELEM_LAST);
        break;

     default:
        break;
   }
   
   EndStruct(buf);
#endif

   EndStruct(buf);

   EndStruct(buf);
#endif 

   EndStruct(buf);
  
   return ;
}

#ifdef RRC_PHY_CFG_SUPP
void printNhuCqiRptModePeriodic(NhuCqiRptModePeriodic *ptr, Buff *buf)
{
   BeginStruct("NhuCqiRptModePeriodic", buf);

#ifdef NHU_VER2
   LOG_ELEM ("pres", ptr->pres, TR_ELEM_CONT);
#endif
   LOG_ELEM ("cqiPeriodicCfgType", ptr->cqiPeriodicCfgType, TR_ELEM_CONT);
  
   BeginStruct("setup", buf);

   LOG_ELEM ("cqiPUCCHRsrcIndx", ptr->setup.cqiPUCCHRsrcIndx, TR_ELEM_CONT);

   LOG_ELEM ("cqiPmiCfgIndx", ptr->setup.cqiPmiCfgIndx, TR_ELEM_CONT);

   LOG_ELEM ("formatIndicator", ptr->setup.formatIndicator, TR_ELEM_CONT);
  
   BeginStruct("subbandCqi", buf);

   LOG_ELEM ("k", ptr->setup.subbandCqi.k, TR_ELEM_LAST);

   EndStruct(buf);

   LOG_ELEM ("riCfgPres", ptr->setup.riCfgPres, TR_ELEM_CONT);

   LOG_ELEM ("riCfgIndx", ptr->setup.riCfgIndx, TR_ELEM_CONT);

   LOG_ELEM ("simultaneousAckNack", ptr->setup.simultaneousAckNack, TR_ELEM_CONT);

#ifdef NHU_VER1
   BeginStruct("NhuCqiMask", buf);

   LOG_ELEM ("pres", ptr->setup.cqiMask.pres, TR_ELEM_CONT);

   LOG_ELEM ("cqiMaskSetup", ptr->setup.cqiMask.cqiMaskSetup, TR_ELEM_LAST);

   EndStruct(buf);
#endif 

   EndStruct(buf);

   EndStruct(buf);

  return ;

}
#endif
