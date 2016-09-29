//#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
#include "gen.h"
#include "ssi.h"           /* System services */
#include "cm_tkns.h"       /* Common Token defines */
#include "cm_lte.h"        /* Common LTE Defines */
#include "cm5.h"
#include "szt.h"
#include "szt_asn.h"
#include "trace.h"
#include "tr_common.h"

#include "gen.x"
#include "ssi.x" 
#include "cm5.x"
#include "cm_lib.x"        /* Common library definitions */
#include "cm_tkns.x"
#include "cm_lte.x"
#include "cm_mblk.x"
#include "szt_asn.x"
#include "szt.x"

#define LOG_SztReqStatus(var, buf, flag)\
{\
   BeginStruct("SztReqStatus", buf);\
\
   LOG_ELEM("status", var.status, TR_ELEM_CONT);\
\
   LOG_ELEM("cause", var.cause, TR_ELEM_LAST);\
\
   EndStruct(buf);\
}

#define LOG_CmNetAddr(var, buf, flag)\
{\
   BeginStruct("CmNetAddr", buf);\
\
   LOG_ELEM("type", var.type, TR_ELEM_CONT);\
\
   BeginStruct("u", buf);\
\
   LOG_ELEM("ipv4NetAddr", var.u.ipv4NetAddr, TR_ELEM_CONT);\
\
   LOG_ARRAY("ipv6NetAddr", var.u.ipv6NetAddr, 16,  TR_ELEM_LAST);\
\
   EndStruct(buf);\
\
   EndStruct(buf);\
}

#define LOG_SztNetAddrLst(var, buf, flag)\
{\
   U32 i = 0;\
\
   BeginStruct("SztNetAddrLst", buf);\
\
   LOG_ELEM("nmb", var.nmb, TR_ELEM_CONT);\
\
   while ( i < SZT_MAX_NET_ADDRS){\
\
     LOG_CmNetAddr(var.nAddr[i++], buf, TR_ELEM_CONT);\
   }\
\
   EndStruct(buf);\
}	

/* 
 * SzUiSztStaInd 
 */
void printUiSztStaInd(SztStaInd *ptr, Buff *buf)
{
   BeginStruct("SztStaInd", buf);

   LOG_ELEM("peerId", ptr->peerId.val, TR_ELEM_CONT);

   LOG_ELEM("type", ptr->type, TR_ELEM_CONT);

   LOG_ELEM("status", ptr->status, TR_ELEM_CONT);

   LOG_ELEM("reason", ptr->reason, TR_ELEM_CONT);

   EndStruct(buf);

   return ;
}

/*
 * SzUiSztFlcInd 
 */
void printUiSztFlcInd(SztFlcInd *ptr, Buff *buf)
{
   BeginStruct("SztFlcInd", buf);

   LOG_ELEM("peerId", ptr->peerId.val, TR_ELEM_CONT);

   LOG_ELEM("status", ptr->status, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

/*
 * SzUiSztErrInd
 */
void printUiSztErrInd(SztLclErrInd *ptr, Buff *buf)
{
   BeginStruct("SztLclErrInd", buf);

   LOG_ELEM("suConnId", ptr->suConnId.val, TR_ELEM_CONT);

   LOG_ELEM("spConnId", ptr->spConnId.val, TR_ELEM_CONT);

   LOG_ELEM("transId", ptr->transId.val, TR_ELEM_CONT);

   LOG_ELEM("causeType", ptr->causeType, TR_ELEM_CONT);

   LOG_ELEM("causeValue", ptr->causeValue, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

/*
 * SzUiSztUDatReq and SzUiSztUDatInd
 */
void printUiSztUDatReq(SztUDatEvnt *ptr, Buff *buf)
{
   BeginStruct("SztUDatEvnt", buf);

   LOG_ELEM("transId", ptr->transId.val, TR_ELEM_CONT);

   LOG_ELEM("peerId", ptr->peerId.val, TR_ELEM_CONT);

   /* PDU Not printing */

   EndStruct(buf);

   return ;
}

/*
 * SzUiSztConReq and SzUiSztConInd
 */
void printUiSztConReq(SztConReq *ptr, Buff *buf)
{
   BeginStruct("SztConReq", buf);

   LOG_ELEM("peerId", ptr->peerId.val, TR_ELEM_CONT);

   BeginStruct("u", buf);

   LOG_ELEM("spConnId", ptr->u.spConnId, TR_ELEM_CONT);

   LOG_ELEM("suConnId", ptr->u.suConnId, TR_ELEM_LAST);

   EndStruct(buf);

   /* PDU Not printing */

   EndStruct(buf);

   return ;
}

/*
 * SzUiSztConRsp, SztRelRsp and SzUiSztConCfm 
 */
void printUiSztConRsp(SztConRsp *ptr, Buff *buf)
{
   BeginStruct("SztConReq", buf);

   LOG_ELEM("spConnId", ptr->spConnId, TR_ELEM_CONT);

   LOG_ELEM("suConnId", ptr->suConnId, TR_ELEM_LAST);

   /* PDU Not printing */

   EndStruct(buf);

   return ;
}

/*
 * SzUiSztRelReq
 */
void printUiSztRelReq(SztRelReq *ptr, Buff *buf)
{
   BeginStruct("SztRelReq", buf);

   LOG_ELEM("connIdType", ptr->connIdType, TR_ELEM_CONT);

   LOG_ELEM("relType", ptr->relType, TR_ELEM_CONT);

   LOG_ELEM("connId", ptr->connId, TR_ELEM_LAST);

   /* PDU is not printing */

   EndStruct(buf);

   return ;
}

/*
 * SzUiSztRelInd
 */
void printUiSztRelInd(SztRelInd *ptr, Buff *buf)
{
   BeginStruct("SztRelInd", buf);

   LOG_ELEM("suConnId", ptr->suConnId.val, TR_ELEM_CONT);

   LOG_ELEM("spConnId", ptr->spConnId, TR_ELEM_LAST);

   /* PDU Not printing */

   EndStruct(buf);

   return ;
}

/*
 * SzUiSztRelCfm 
 */
void printUiSztRelCfm(SztRelCfm *ptr, Buff *buf)
{
   BeginStruct("SztRelCfm", buf);

   LOG_ELEM("suConnId", ptr->suConnId, TR_ELEM_CONT);

   LOG_ELEM("spConnId", ptr->spConnId, TR_ELEM_CONT);

   LOG_ELEM("relType", ptr->relType, TR_ELEM_LAST);

   /* PDU Not printing */

   EndStruct(buf);

   return ;
}

/*
 * SzUiSztDatReq
 */
void printUiSztDatReq(SztDatEvntReq *ptr, Buff *buf)
{
   BeginStruct("SztDatEvntReq", buf);

   LOG_ELEM("spConnId", ptr->spConnId, TR_ELEM_CONT);

   /* PDU Not printing */

   EndStruct(buf);

   return ;
}

/*
 * SzUiSztDatInd
 */
void printUiSztDatInd(SztDatEvntInd *ptr, Buff *buf)
{
   BeginStruct("SztDatEvntInd", buf);

   LOG_ELEM("suConnId", ptr->suConnId, TR_ELEM_LAST);

   /* PDU Not printing */

   EndStruct(buf);

   return ;
}

/*
 * SzUiSztEncDecReq 
 */
void printUiSztEncDecReq(SztEncDecEvnt *ptr, Buff *buf)
{
   BeginStruct("SztEncDecEvnt", buf);

   LOG_ELEM("transId", ptr->transId, TR_ELEM_CONT);

   LOG_ELEM("type", ptr->type.val, TR_ELEM_CONT);

#ifdef SZTV1

   LOG_ELEM("ieType", ptr->ieType.val, TR_ELEM_CONT);

#endif 
   BeginStruct("u", buf);

   /* PDU Not printing */

   /* ie PDU Not printing */

   /* mBuf PDU Not printing */

   EndStruct(buf);

   EndStruct(buf);

   return ;
}

/*
 * SzUiSztEncDecCfm
 */
void printUiSztEncDecCfm(SztEncDecEvnt *ptr, SztReqStatus *status, Buff *buf)
{
   BeginStruct("SztEncDecEvnt", buf);

   LOG_ELEM("transId", ptr->transId, TR_ELEM_CONT);

   LOG_ELEM("type", ptr->type.val, TR_ELEM_CONT);

#ifdef SZTV1

   LOG_ELEM("ieType", ptr->ieType.val, TR_ELEM_CONT);

#endif 
   BeginStruct("u", buf);

   /* PDU Not printing */

   /* ie PDU Not printing */

   /* mBuf PDU Not printing */

   EndStruct(buf);

   EndStruct(buf);

   BeginStruct("SztReqStatus", buf);

   LOG_ELEM("status", status->status, TR_ELEM_CONT);

   LOG_ELEM("type", status->cause, TR_ELEM_LAST);

   EndStruct(buf);

   return ;
}

/*
 * SzUiSztAudReq and SzUiSztAudCfm
 */
void printUiSztAudEvnt(SztAudEvnt *ptr, Buff *buf)
{
   U32 i = 0; 

   BeginStruct("SztAudEvnt", buf);

   LOG_ELEM("type", ptr->type, TR_ELEM_CONT);

   BeginStruct("u", buf);

   if(ptr->type == SZT_AUD_CONN)
   {
     BeginStruct("SztAudConn", buf);

     LOG_ELEM("connIdType", ptr->u.audConn.connIdType, TR_ELEM_CONT);

     LOG_ELEM("nmbConn", ptr->u.audConn.nmbConn, TR_ELEM_CONT);

     while( i < SZT_MAX_AUD_CONN)
     {
     	BeginStruct("SztConnSt", buf);
     
	LOG_ELEM("connId", ptr->u.audConn.connSt[i].connId, TR_ELEM_CONT);
     
	LOG_ELEM("state", ptr->u.audConn.connSt[i].state, TR_ELEM_LAST);

        EndStruct(buf);

	i++;
     }
     EndStruct(buf);
   }
   else
   {
     BeginStruct("SztAudPeer", buf);
     
     LOG_ELEM("peerId", ptr->u.audPeer.peerId, TR_ELEM_CONT);
     
     LOG_ELEM("peerState", ptr->u.audPeer.peerState, TR_ELEM_LAST);

     EndStruct(buf);
   }

   LOG_SztReqStatus(ptr->status, buf, TR_ELEM_LAST);

   EndStruct(buf);
   
   return ;

}

#ifdef SZTV2 
/*
 * SzUiSztAbortS1Req and SzUiSztAbortS1Cfm
 */
void printSztAbortS1(SztAbortS1 *ptr, Buff *buf)
{
  BeginStruct("SztAbortS1", buf);
     
  LOG_ELEM("peerId", ptr->peerId.val, TR_ELEM_CONT);

  BeginStruct("CmStatus", buf);
     
  LOG_ELEM("status", ptr->abortS1Cfm.status, TR_ELEM_CONT);
     
  LOG_ELEM("reason", ptr->abortS1Cfm.reason, TR_ELEM_LAST);

  EndStruct(buf);

  EndStruct(buf);

  return ;
}
#endif 

#ifdef SZ_MME
/*
 * SzUiSztPeerAssocInd
 */
void  printSztPeerAssocInd(SztPeerAssocInd *ptr, Buff *buf)
{
  BeginStruct("SztPeerAssocInd", buf);

  LOG_SztNetAddrLst(ptr->dstAddrLst, buf, TR_ELEM_CONT);

  LOG_ELEM("SztPort", ptr->dstPort, TR_ELEM_LAST);

  EndStruct(buf);

  return ;
}

/*
 * SzUiSztPeerAssocRsp
 */
void printSztPeerAssocRsp(SztPeerAssocRsp *ptr, Buff *buf)
{
  BeginStruct("SztPeerAssocRsp", buf);

  LOG_ELEM("peerId", ptr->peerId, TR_ELEM_CONT);

  LOG_SztNetAddrLst(ptr->dstAddrLst, buf, TR_ELEM_CONT);

  LOG_ELEM("dstPort", ptr->dstPort, TR_ELEM_CONT);
     
  LOG_CmNetAddr(ptr->priDstAddr, buf, TR_ELEM_CONT);

  LOG_ELEM("comProcStrm", ptr->comProcStrm, TR_ELEM_CONT);

  LOG_ELEM("locOutStrms", ptr->locOutStrms, TR_ELEM_CONT);

  LOG_SztReqStatus(ptr->status, buf, TR_ELEM_LAST);

  EndStruct(buf);

  return ;
}
#endif 

#ifdef SZ_ENC_DEC 
/*
 * SzUiSztEncReq and SzUiSztDecReq
 */
void printSztEncDecStatus(SztEncDecStatus *ptr, Buff *buf)
{
  BeginStruct("SztEncDecStatus", buf);

  LOG_ELEM("status", ptr->status, TR_ELEM_CONT);

  LOG_ELEM("cause", ptr->cause, TR_ELEM_CONT);

  LOG_ELEM("nmbOct", ptr->nmbOct, TR_ELEM_CONT);

  BeginStruct("SztEncDecErr", buf);

  LOG_ELEM("ieCount", ptr->errInfo.ieCount, TR_ELEM_CONT);

  while(i < SZT_MAX_ERR_IE)
  {
     BeginStruct("SztIeErrInfo", buf);
  
     LOG_ELEM("errType", ptr->errInfo.ieInfo[i].errType, TR_ELEM_CONT);
  
     LOG_ELEM("criticality", ptr->errInfo.ieInfo[i].criticality, TR_ELEM_CONT);
  
     LOG_ELEM("ieId", ptr->errInfo.ieInfo[i].ieId, TR_ELEM_LAST);

     EndStruct(buf);
     
     i++; 
  }
     
  EndStruct(buf);
     
  EndStruct(buf);

  return ;
}

/*
 * SzUiSztStsReq and SzUiSztInitReq
 */
void printSztReqStatus(SztReqStatus *ptr, Buff *buf)
{
  LOG_SztReqStatus(ptr, buf, TR_ELEM_LAST);

  return ;
}
#endif
