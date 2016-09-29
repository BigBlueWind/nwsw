/* 
 * File:   trace.h
 * Author: Raghavendra 
 * 
 * Created on 26 June 2012, 13:06
 */

#ifndef _TRACE_
#define _TRACE_

//#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"
#include "ssi.h"           /* System services */
#include "cm_tkns.h"       /* Common Token defines */

#include "gen.x"
#include "ssi.x" 
#include "cm_lte.x"
#include "cm_lib.x"        /* Common library definitions */
#include "cm_tkns.x"
#include "cm_mblk.x"

#include "rgr.h"
#include "nhu.h"
#include "szt.h"
#include "szt_asn.h"

#include "rgr.x"
#include "nhu_asn.x"
#include "nhu_3gasn.x"
#include "nhu.x"
#include "ctf.h"
#include "ctf.x"
#include "pju.h"
#include "pju.x"
#include "szt_asn.x"
#include "szt.x"

#if 0
#include "cm5.x"           /* Timer services */
#endif
#include <stdio.h>

typedef enum 
{
  TR_ELEM_LAST, 
  TR_ELEM_CONT
}trailing;


#define RRM_PRL_MAX_BUFF  5000
typedef struct _buff
{
  int   buffCapacity;
  int   numBytesWritten;
  char  data[RRM_PRL_MAX_BUFF];
  char *startAddr;
}Buff;

#define BeginStruct(name, buf) \
{\
  int ret = snprintf((char *)((char *)buf->startAddr + buf->numBytesWritten),(size_t)(buf->buffCapacity - buf->numBytesWritten), "%s={", name);\
  if(ret != -1 )\
  {\
    buf->numBytesWritten += ret; \
  }\
}

#define EndStruct(buf) \
{\
  int ret = snprintf((char *)((char *)buf->startAddr + buf->numBytesWritten), (size_t)(buf->buffCapacity - buf->numBytesWritten), "%s", "}");\
  if(ret != -1 )\
  {\
    buf->numBytesWritten += ret; \
  }\
}

#define printElem(buf, format, ...) \
{\
  int ret = snprintf((char *)((char *)buf->startAddr + buf->numBytesWritten), (buf->buffCapacity - buf->numBytesWritten), format, __VA_ARGS__);\
  if(ret != -1 )\
  {\
    buf->numBytesWritten += ret; \
  }\
}

#define LOG_MSG(msg) printElem(buf, "%s \n", msg)

#define LOG_ELEM(str, var, flag)\
{\
 printElem(buf, "%s %d %s", str,(int)var, ((flag == TR_ELEM_LAST)?"":"|"));\
}
      
#define LOG_ARRAY(string, var, length, flag)\
{\
   int idx = 0;\
   printElem(buf, "%s :", string);\
   while( idx < length) { \
     printElem(buf, "%d ", (int)var[idx++]);\
   }\
   printElem(buf, "%s", ((flag == TR_ELEM_LAST)?"":"|"));\
}

/* APIs between RRM - SCH */
void printRgrCfgReq(RgrCfgReqInfo *ptr, Buff *buf);
void printRgrCfgCfm(RgrCfgTransId transId, U8 status, Buff *buf);
#ifdef RGR_SI_SCH
void printRgrSiCfgReqInfo(RgrSiCfgReqInfo *ptr, Buff *buf);
void printRgrSiCfgCfm(RgrCfgTransId transId, U8 status, Buff *buf);
#endif 
#ifdef RGR_CQI_REPT
void printRgrStaIndInfo(RgrStaIndInfo *ptr, Buff *buf);
#endif

/* APIs between RRM _ PHY*/
void printCtfCfgReqInfo(CtfCfgReqInfo *ptr, Buff *buf);
#ifdef CTF_AS_SEC_ENB   
#if 0
void printCtfDatReqSdus(CtfDatReqSdus *ptr, Buff *buf);
void printCtfDatRspSdus(CtfDatRspSdus *ptr, Buff *buf);
#endif
#endif 
void printCtfCfgCfm(CtfCfgTransId transId, U8 status, Buff *buf);
void printCtfUeIdChgReq(CtfCfgTransId transId, CtfUeInfo *ueInfo, CtfUeInfo *newUeInfo, Buff *buf);
void printCtfUeIdChgCfm(CtfCfgTransId transId, CtfUeInfo *ueInfo, CmStatus status, Buff *buf);

/* APIs between RRM - RRC */
void printNhUiNhuCfgReq(NhuCfgReqSdus *CfgReq, Buff *buf);
void printNhuCellCfgReqSdus(NhuCellCfgReqSdus *cfgReq, Buff *buf);
void printNhUiNhuDatReq(NhuDatReqSdus *nhuDatReq, Buff *buf);
void printNhuUiNhuDatRsp(NhuDatRspSdus *ptr, Buff *buf);
void printNhUiNhuCnclUeReq ( NhuCellId cellId, NhuCrnti crnti, Buff *buf);
void printNhuUiNhuDeleteTransReq(NhuCellId cellId, NhuCrnti crnti, U32 transId, Buff *buf);
void printNhUiNhuDatCfm(NhuDatCfmSdus *ptr, Buff *buf);
void printNhuErrorInfo(NhuErrorInfo *ptr, Buff *buf);
void printNhuStaIndSdus(NhuStaIndSdus *ptr, Buff *buf);
void printNhuEncReqSdus(NhuEncReqSdus *ptr, Buff *buf);
void printNhuEncCfmSdus(NhuEncCfmSdus *ptr, Buff *buf);
void printNhuPdcpSduStaReq(NhuHdr *ptr, Buff *buf);
void printNhuPdcpSduStaCfm(NhuPdcpSduStaCfm *ptr, Buff *buf);
void printNhuDecodeReq(NhuDecodeSduReq *ptr, Buff *buf);
void printNhuDecodeCfm(NhuDecodeSduCfm *ptr, Buff *buf);
void printNhuCountReq(NhuHdr *ptr, Buff *buf);
void printNhuCountCfm(NhuDRBCountInfoLst *ptr, Buff *buf);
void printNhuDatInd(NhuDatIndSdus *ptr, Buff *buf);
void printNhuErrIndSdus(NhuErrIndSdus *ptr, Buff *buf);
void printNhuCfgCfmSdus(NhuCfgCfmSdus *ptr, Buff *buf);
void printNhuCellCfgCfmSdus(NhuCellCfgCfmSdus *ptr, Buff *buf);
void printNhuCmnMacCfg(NhuCmnMacCfg *macCfg, Buff *buf);
/* APIS between RRM and S1AP */
void printUiSztStaInd(SztStaInd *ptr, Buff *buf);
void printUiSztFlcInd(SztFlcInd *ptr, Buff *buf);
void printUiSztErrInd(SztLclErrInd *ptr, Buff *buf);
void printUiSztUDatReq(SztUDatEvnt *ptr, Buff *buf);
void printUiSztConReq(SztConReq *ptr, Buff *buf); 
void printUiSztConRsp(SztConRsp *ptr, Buff *buf);
void printUiSztRelReq(SztRelReq *ptr, Buff *buf);
void printUiSztRelInd(SztRelInd *ptr, Buff *buf);
void printUiSztRelCfm(SztRelCfm *ptr, Buff *buf);
void printUiSztDatReq(SztDatEvntReq *ptr, Buff *buf);
void printUiSztDatInd(SztDatEvntInd *ptr, Buff *buf);
void printUiSztEncDecReq(SztEncDecEvnt *ptr, Buff *buf); 
void printUiSztEncDecCfm(SztEncDecEvnt *ptr, SztReqStatus *status, Buff *buf);
void printUiSztAudEvnt(SztAudEvnt *ptr, Buff *buf);
#ifdef SZTV2 
void printSztAbortS1(SztAbortS1 *ptr, Buff *buf);
#endif 
#ifdef SZ_MME
void  printSztPeerAssocInd(SztPeerAssocInd *ptr, Buff *buf);
void printSztPeerAssocRsp(SztPeerAssocRsp *ptr, Buff *buf); 
#endif
#ifdef SZ_ENC_DEC 
void printSztEncDecStatus(SztEncDecStatus *ptr, Buff *buf);
void printSztReqStatus(SztReqStatus *ptr, Buff *buf);
#endif

#endif

