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
 
     Name:     RRC User

     Type:     C source file

     Desc:     C source code for upper layer(RX) across NHU interface

     File:     ve_nh_ptli.c

     Sid:      ve_nh_ptli.c@@/main/2 - Wed Dec  1 08:44:27 2010

     Prg:      rbhat
  
**********************************************************************/
/*
*     This software may be combined with the following TRILLIUM
*     software:
*
*     part no.                      description
*     --------    ----------------------------------------------
*     1000369     LTE RRC
*
*/

#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_inet.h"
#include "cm_tpt.h"

#include "nhu_asn.h"
#include "nhu.h"           /* NHU interface defines */
#include "pju.h"           /* PDCP data interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "ctf.h"           /* PHY RRM control Interface */
#include "szt.h"           /* S1AP RRM control Interface */
#ifdef EU_DAT_APP
#include "egt.h"           /* S1AP RRM control Interface */
#endif 
#include "lrg.h"           /* MAC RRM control Interface */
#include"ve_eut.h"


/* RRM related includes */
#include "ve.h" 
#include "lve.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"       /*  Common UMTS includes */
#include "cm_lib.x"
#include "cm_inet.x"
#include "cm_tpt.x"

#include "nhu_asn.x"
#include "nhu.x"           /* NHU Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "ctf.x"           /* PHY RRM control Interface */
#include "szt.x"           /* S1AP RRM control Interface */
#include "czt.x"
#ifdef EU_DAT_APP
#include "egt.x"           /* S1AP RRM control Interface */
#endif
#include "lrg.x"           /* MAC RRM control Interface */
#include"ve_eut.x"

/* RRM related includes */
#include "lve.x" 
#include "ve.x" 

/* local defines */
#define MAXVELI 3     /* Size of matrix used for LC/LWLC/TC/One for Dummy */

#ifdef VE
#if (!(defined(LWLCVELINHU) && defined(LCNHUINHU)))
#define PTVELINHU 1
#else
#ifndef NH
#define PTVELINHU 1
#endif
#endif
#endif

#if (PTVELINHU)
PRIVATE S16 PtLiNhuBndReq  ARGS(( Pst *pst, SuId suId, SpId spId ));
PRIVATE S16 PtLiNhuUbndReq ARGS(( Pst *pst, SpId spId, Reason reason ));
PRIVATE S16 PtLiNhuDatReq  ARGS(( Pst *pst, SpId spId, NhuDatReqSdus *nhuDatReqSdus ));
PRIVATE S16 PtLiNhuDatRsp  ARGS(( Pst *pst, SpId spId, NhuDatRspSdus *nhuDatRspSdus ));
PRIVATE S16 PtLiNhuCfgReq  ARGS(( Pst *pst, SpId spId, NhuCfgReqSdus *nhuCfgReqSdus ));
PRIVATE S16 PtLiNhuCnclUeReq  ARGS(( Pst *pst, SpId spId, NhuCellId cellId,
                                       NhuCrnti crnti ));
PRIVATE S16 PtLiNhuCellCfgReq  ARGS(( Pst *pst, SpId spId, NhuCellCfgReqSdus *nhuCellCfgReqSdus ));
PRIVATE S16 PtLiNhuDeleteTransReq  ARGS(( Pst *pst, SpId spId, 
                                          NhuCellId cellId, NhuCrnti ueId,      
                                          U32 transId ));
PRIVATE S16 PtLiNhuCountReq ARGS((Pst *pst, SuId suId, NhuHdr *countHdr));
PRIVATE S16 PtLiNhuEncodeReq ARGS((Pst *pst, SuId suId, NhuEncReqSdus *nhuEncReqSdus));

PRIVATE S16 PtLiNhuPdcpSduStaReq ARGS((Pst *pst, SuId    suId, NhuHdr  nhuHdr));
PRIVATE S16 PtLiNhuDecodeReq ARGS(( Pst *pst, SuId suId, NhuDecodeSduReq *nhuDecodeSduReq));

#endif /* PTVELINHU */


/* Bind Request Primitive */
PRIVATE NhuBndReq VeLiNhuBndReqMt [MAXVELI] =
{
#if (defined(LCVELINHU))
   cmPkNhuBndReq,            /* 0 - loosely coupled */
#else
   PtLiNhuBndReq,            /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuBndReq,            /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuBndReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCVELINHU))
   cmPkNhuBndReq            /* 1 - light weight loosely coupled */
#else
   PtLiNhuBndReq            /* 1 - light weight loosely coupled,portable */
#endif
};

/* Unbind Request Primitive */
PRIVATE NhuUbndReq VeLiNhuUbndReqMt [MAXVELI] =
{
#if (defined(LCVELINHU))
   cmPkNhuUbndReq,            /* 0 - loosely coupled */
#else
   PtLiNhuUbndReq,            /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuUbndReq,            /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuUbndReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCVELINHU))
   cmPkNhuUbndReq            /* 1 - light weight loosely coupled */
#else
   PtLiNhuUbndReq            /* 1 - light weight loosely coupled,portable */
#endif
};

/* Data Request Primitive */
PRIVATE NhuDatReq VeLiNhuDatReqMt [MAXVELI] =
{
#if (defined(LCVELINHU))
   cmPkNhuDatReq,            /* 0 - loosely coupled */
#else
   PtLiNhuDatReq,            /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuDatReq,            /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuDatReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCVELINHU))
   cmPkNhuDatReq            /* 1 - light weight loosely coupled */
#else
   PtLiNhuDatReq            /* 1 - light weight loosely coupled,portable */
#endif
};

/* Data Response Primitive */ 
PRIVATE NhuDatRsp VeLiNhuDatRspMt [MAXVELI] = 
{ 
#if (defined(LCVELINHU)) 
   cmPkNhuDatRsp,            /* 0 - loosely coupled */
#else 
   PtLiNhuDatRsp,            /* 0 - loosely coupled,portable */
#endif 
#ifdef NH 
   NhUiNhuDatRsp,            /* 2 - tightly coupled,stub layer */
#else 
   PtLiNhuDatRsp,            /* 2 - tightly coupled,portable */
#endif 
#if (defined(LWLCVELINHU)) 
   cmPkNhuDatRsp            /* 1 - light weight loosely coupled */
#else 
   PtLiNhuDatRsp            /* 1 - light weight loosely coupled,portable */
#endif 
};

/* Delete Transaction Request Primitive */
PRIVATE NhuDeleteTransReq VeLiNhuDeleteTransReqMt[MAXVELI] =
{
#if (defined(LCVELINHU))
   cmPkNhuDeleteTransReq,      /* 0 - loosely coupled */
#else
   PtLiNhuDeleteTransReq,      /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuDeleteTransReq,      /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuDeleteTransReq,      /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCVELINHU))
   cmPkNhuDeleteTransReq,      /* 1 - light weight loosely coupled */
#else
   PtLiNhuDeleteTransReq       /* 1 - light weight loosely coupled,portable */
#endif
};

/* Configuration Request Primitive */
PRIVATE NhuCfgReq VeLiNhuCfgReqMt [MAXVELI] =
{
#if (defined(LCVELINHU))
   cmPkNhuCfgReq,            /* 0 - loosely coupled */
#else
   PtLiNhuCfgReq,            /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuCfgReq,            /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuCfgReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCVELINHU))
   cmPkNhuCfgReq            /* 1 - light weight loosely coupled */
#else
   PtLiNhuCfgReq            /* 1 - light weight loosely coupled,portable */
#endif
};

/* Cancel UE Request primitive */
PRIVATE NhuCnclUeReq VeLiNhuCnclUeReqMt [MAXVELI] =
{ 
#if (defined(LCVELINHU))
   cmPkNhuCnclUeReq,            /* 0 - loosely coupled */
#else 
   PtLiNhuCnclUeReq,            /* 0 - loosely coupled,portable */
#endif 
#ifdef NH
   NhUiNhuCnclUeReq,              /* 2 - tightly coupled,stub layer */
#else 
   PtLiNhuCnclUeReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCVELINHU))
   cmPkNhuCnclUeReq            /* 1 - loosely coupled */
#else 
   PtLiNhuCnclUeReq            /* 1 - loosely coupled,portable */
#endif 
};

/* Cell Config Request primitive */
PRIVATE NhuCellCfgReq VeLiNhuCellCfgReqMt [MAXVELI] =
{ 
#if (defined(LCVELINHU))
   cmPkNhuCellCfgReq,            /* 0 - loosely coupled */
#else 
   PtLiNhuCellCfgReq,            /* 0 - loosely coupled,portable */
#endif 
#ifdef NH
   NhUiNhuCellCfgReq,            /* 2 - tightly coupled,stub layer */
#else 
   PtLiNhuCellCfgReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCVELINHU))
   cmPkNhuCellCfgReq            /* 1 - loosely coupled */
#else 
   PtLiNhuCellCfgReq            /* 1 - loosely coupled,portable */
#endif 
};

/* Decode Request Primitive */
PRIVATE NhuDecodeReq VeLiNhuDecodeReqMt [MAXVELI] =
{
#if (defined(LCVELINHU))
   cmPkNhuDecodeReq,            /* 0 - loosely coupled */
#else
   PtLiNhuDecodeReq,            /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuDecodeReq,            /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuDecodeReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCVELINHU))
   cmPkNhuDecodeReq            /* 1 - light weight loosely coupled */
#else
   PtLiNhuDecodeReq            /* 1 - light weight loosely coupled,portable */
#endif
};

/* Count Request Primitive */
PRIVATE NhuCountReq VeLiNhuCountReqMt [MAXVELI] =
{
#if (defined(LCVELINHU))
   cmPkNhuCountReq,            /* 0 - loosely coupled */
#else
   PtLiNhuCountReq,            /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuCountReq,            /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuCountReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCVELINHU))
   cmPkNhuCountReq            /* 1 - light weight loosely coupled */
#else
   PtLiNhuCountReq            /* 1 - light weight loosely coupled,portable */
#endif
};

/* Enocde Request Primitive */
PRIVATE NhuEncReq VeLiNhuEncodeReqMt [MAXVELI] =
{
#if (defined(LCVELINHU))
   cmPkNhuEncodeReq,            /* 0 - loosely coupled */
#else
   PtLiNhuEncodeReq,            /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuEncodeReq,            /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuEncodeReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCVELINHU))
   cmPkNhuEncodeReq            /* 1 - light weight loosely coupled */
#else
   PtLiNhuEncodeReq            /* 1 - light weight loosely coupled,portable */
#endif
};

/* SDU Status Request Primitive */
PRIVATE NhuSduStaReq VeLiNhuPdcpSduStaReqMt [MAXVELI] =
{
#if (defined(LCVELINHU))
   cmPkNhuPdcpSduStaReq,            /* 0 - loosely coupled */
#else
   PtLiNhuPdcpSduStaReq,            /* 0 - loosely coupled,portable */
#endif
#ifdef NH
   NhUiNhuPdcpSduStaReq,            /* 2 - tightly coupled,stub layer */
#else
   PtLiNhuPdcpSduStaReq,            /* 2 - tightly coupled,portable */
#endif
#if (defined(LWLCVELINHU))
   cmPkNhuPdcpSduStaReq            /* 1 - light weight loosely coupled */
#else
   PtLiNhuPdcpSduStaReq            /* 1 - light weight loosely coupled,portable */
#endif
};


#if (PTVELINHU)
/**************************************************************************
 *
 *      Fun:   PtLiNhuBndReq
 *
 *      Desc:  Bind Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ve_nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiNhuBndReq
(
Pst    *pst,     /* post structure */
SuId   suId,     /* NHU serivce user SAP ID */ 
SpId   spId      /* NHU SAP ID */
)
#else
PRIVATE S16 PtLiNhuBndReq(pst, suId, spId)
Pst    *pst;     /* post structure */
SuId   suId;     /* NHU serivce user SAP ID */ 
SpId   spId;     /* NHU SAP ID */
#endif
{
   TRC3(PtLiNhuBndReq)
   RETVALUE(ROK);
} /* PtLiNhuBndReq */

/**************************************************************************
 * 
 *      Fun:   PtLiNhuUbndReq 
 * 
 *      Desc:  UnBind Request from RRC User to RRC
 * 
 *      Ret:   ROK - ok 
 * 
 *      Notes: None 
 * 
 *      File:  ve_nh_ptli.c 
 * 
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiNhuUbndReq
(
Pst          *pst,     /* post structure */
SpId         spId,     /* NHU SAP ID */
Reason       reason    /* Reason for Unbind */
)
#else
PRIVATE S16 PtLiNhuUbndReq(pst, spId, reason)
Pst          *pst;     /* post structure */
SpId         spId;     /* NHU SAP ID */
Reason       reason;   /* Reason for Unbind */
#endif
{ 
   TRC3(PtLiNhuUbndReq) 
 
   RETVALUE(ROK); 
} /* PtLiNhuUbndReq */


/**************************************************************************
 *
 *      Fun:   PtLiNhuDatReq
 *
 *      Desc:  Data Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ve_nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiNhuDatReq
(
Pst             *pst,            /* post structure */
SpId            spId,            /* RRC Service provider SAP ID */
NhuDatReqSdus   *nhuDatReqSdus   /* SDU */
)
#else
PRIVATE S16 PtLiNhuDatReq(pst, spId, nhuDatReqSdus)
Pst             *pst;             /* post structure */
SpId            spId;             /* RRC Service provider SAP ID */
NhuDatReqSdus   *nhuDatReqSdus;   /* SDU */
#endif
{
   TRC3(PtLiNhuDatReq)

 
   RETVALUE(ROK);

} /* PtLiNhuDatReq */

/**************************************************************************
 *
 *      Fun:   PtLiNhuDatRsp
 *
 *      Desc:  Data Response from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 * 
 *      File:  ve_nh_ptli.c
 * 
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiNhuDatRsp
(
Pst      *pst, 
SpId     spId,  
NhuDatRspSdus *nhuDatRspSdus
)
#else
PRIVATE S16 PtLiNhuDatRsp(pst, spId, nhuDatRspSdus)
Pst      *pst; 
SpId     spId;  
NhuDatRspSdus *nhuDatRspSdus;
#endif
{ 
   TRC3(PtLiNhuDatRsp)
 
   RETVALUE(ROK); 
} /* PtLiNhuDatRsp */

/**************************************************************************
 *
 *      Fun:   PtLiNhuDeleteTransReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ve_nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiNhuDeleteTransReq
(
Pst        *pst,       /* Post Structure */
SpId       spId,       /* Service Provider Id */
NhuCellId  cellId,     /* Cell ID */
NhuCrnti   ueId,       /* Ue Id */
U32        transId     /* transaction id */
)
#else
PRIVATE S16 PtLiNhuDeleteTransReq(pst, spId, cellId, ueId, transId)
Pst        *pst;       /* Post Structure */
SpId       spId;       /* Service Provider Id */
NhuCellId  cellId;     /* Cell ID */
NhuCrnti   ueId;       /* Ue Id */
U32        transId;    /* Trans id */
#endif
{
   TRC3(PtLiNhuDeleteTransReq)
 
   RETVALUE(ROK); 
}
 
/**************************************************************************
 *
 *      Fun:   PtLiNhuCfgReq
 *
 *      Desc:  Config Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ve_nh_ptli.c
 *
 *************************************************************************/
 #ifdef ANSI
PRIVATE S16 PtLiNhuCfgReq
(
Pst             *pst,
SpId            spId,
NhuCfgReqSdus   *nhuCfgReqSdus
)
#else
PRIVATE S16 PtLiNhuCfgReq(pst, spId, nhuCfgReqSdus)
Pst             *pst;
SpId            spId;
NhuCfgReqSdus   *nhuCfgReqSdus;
#endif
{
   TRC3(PtLiNhuCfgReq)
   RETVALUE(ROK); 
} /* PtLiNhuCfgReq */ 

/**************************************************************************
 *
 *      Fun:   PtLiNhuCnclUeReq
 * 
 *      Desc:  Cancel UE Request from RRC User to RRC
 * 
 *      Ret:   ROK - ok 
 * 
 *      Notes: None 
 * 
 *      File:  ve_nh_ptli.c 
 * 
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiNhuCnclUeReq
(
Pst         *pst,
SpId        spId,
NhuCellId   cellId,
NhuCrnti    crnti
)
#else
PRIVATE S16 PtLiNhuCnclUeReq(pst, spId, cellId, crnti)
Pst *pst;
SpId spId;
NhuCellId   cellId;
NhuCrnti    crnti;
#endif
{ 
   TRC3(PtLiNhuCnclUeReq)
 
 
   RETVALUE(ROK); 
 
} /* PtLiNhuCnclUeReq */

/**************************************************************************
 *
 *      Fun:   PtLiNhuCellCfgReq
 * 
 *      Desc:  SCCPCH Config Request from RRC User to RRC
 * 
 *      Ret:   ROK - ok 
 * 
 *      Notes: None 
 * 
 *      File:  ve_nh_ptli.c 
 * 
 *************************************************************************/
 #ifdef ANSI
PRIVATE S16 PtLiNhuCellCfgReq
(
Pst *pst,
SpId spId,
NhuCellCfgReqSdus *nhuCellCfgReqSdus
)
#else
PRIVATE S16 PtLiNhuCellCfgReq(pst, spId, nhuCellCfgReqSdus)
Pst                 *pst;
SpId                spId;
NhuCellCfgReqSdus   *nhuCellCfgReqSdus;
#endif
{ 
   TRC3(PtLiNhuCellCfgReq)
   RETVALUE(ROK); 
} /* PtLiNhuCellCfgReq */


/**************************************************************************
 *
 *      Fun:   PtLiNhuDecodeReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ve_nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiNhuDecodeReq
(
Pst     *pst,
SuId     suId,
NhuDecodeSduReq *nhuDecodeSduReq
)
#else
PRIVATE S16 PtLiNhuDecodeReq(pst, suId, nhuDecodeSduReq)
Pst     *pst;
SuId     suId;
NhuDecodeSduReq *nhuDecodeSduReq;
#endif
{
   TRC3(PtLiNhuDecodeReq)
   RETVALUE(ROK);
}

/**************************************************************************
 *
 *      Fun:   PtLiNhuCountReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ve_nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiNhuCountReq
(
Pst     *pst,
SuId     suId,
NhuHdr *countHdr
)
#else
PRIVATE S16 PtLiNhuCountReq(pst, suId, countHdr)
Pst     *pst;
SuId     suId;
NhuHdr *countHdr;
#endif
{
   TRC3(PtLiNhuCountReq)
   RETVALUE(ROK);
}

/**************************************************************************
 *
 *      Fun:   PtLiNhuEncodeReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ve_nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiNhuEncodeReq
(
Pst     *pst,
SuId     suId,
NhuEncReqSdus *nhuEncReqSdus
)
#else
PRIVATE S16 PtLiNhuEncodeReq(pst, suId, nhuEncReqSdus)
Pst     *pst;
SuId     suId;
NhuEncReqSdus *nhuEncReqSdus;
#endif
{
   TRC3(PtLiNhuEncodeReq)
   RETVALUE(ROK);
}

/**************************************************************************
 *
 *      Fun:   PtLiNhuPdcpSduStaReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ve_nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiNhuPdcpSduStaReq
(
Pst     *pst,
SuId    suId,
NhuHdr  nhuHdr
)
#else
PRIVATE S16 PtLiNhuPdcpSduStaReq(pst, suId, nhuHdr)
Pst     *pst;
SuId    suId;
NhuHdr  nhuHdr;
#endif
{
   TRC3(PtLiNhuPdcpSduStaReq)
   RETVALUE(ROK);
}

#endif /* PTVELINHU */

/* LI primtives for NHU  */

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

/* 
* 
*       Fun:   lower interface - Bind Request
*  
*       Desc:  This function indicates the receipt of a bind request  
*  
*       Ret:   ROK      - ok
* 
*       Notes: None 
* 
*       File:  
*        
*/
#ifdef ANSI
PUBLIC S16 VeLiNhuBndReq
(
Pst    *pst,     /* post structure */
SuId   suId,     /* NHU serivce user SAP ID */ 
SpId   spId      /* NHU SAP ID */
)
#else
PUBLIC S16 VeLiNhuBndReq(pst, suId, spId)
Pst    *pst;     /* post structure */
SuId   suId;     /* NHU serivce user SAP ID */ 
SpId   spId;     /* NHU SAP ID */
#endif
{
   TRC2(VeLiNhuBndReq)

   RETVALUE((*VeLiNhuBndReqMt[pst->selector])(pst, suId, spId));
} /* end of VeLiNhuBndReq */


/* 
* 
*       Fun:   lower interface - Unbind Request
*  
*       Desc:  This function indicates the receipt of a bind request  
*  
*       Ret:   ROK      - ok
* 
*       Notes: None 
* 
*       File:  
*        
*/
#ifdef ANSI
PUBLIC S16 VeLiNhuUbndReq
(
Pst          *pst,     /* post structure */
SpId         spId,     /* NHU SAP ID */
Reason       reason    /* Reason for Unbind */
)
#else
PUBLIC S16 VeLiNhuUbndReq(pst, spId, reason)
Pst          *pst;     /* post structure */
SpId         spId;     /* NHU SAP ID */
Reason       reason;   /* Reason for Unbind */
#endif
{
   TRC2(VeLiNhuUbndReq)

   RETVALUE((*VeLiNhuUbndReqMt[pst->selector])(pst, spId, reason));

} /* end of VeLiNhuUbndReq */

/* 
* 
*       Fun:   lower interface - Dat Request
*  
*       Desc:   
*  
*       Ret:   ROK      - ok
* 
*       Notes: None 
* 
*       File:  
*        
*/
#ifdef ANSI
PUBLIC S16 VeLiNhuDatReq
(
Pst             *pst,            /* post structure */
SpId            spId,            /* RRC Service provider SAP ID */
NhuDatReqSdus   *nhuDatReqSdus   /* SDU */
)
#else
PUBLIC S16 VeLiNhuDatReq(pst, spId, nhuDatReqSdus)
Pst             *pst;             /* post structure */
SpId            spId;             /* RRC Service provider SAP ID */
NhuDatReqSdus   *nhuDatReqSdus;   /* SDU */
#endif
{
   TRC3(VeLiNhuDatReq);

   RETVALUE((*VeLiNhuDatReqMt[pst->selector])(pst, spId, nhuDatReqSdus));
   
} /* end of function VeLiNhuDatReq */

/* 
* 
*       Fun:   lower interface - Dat Response
*  
*       Desc:    
*  
*       Ret:   ROK      - ok
* 
*       Notes: None 
* 
*       File:  
*        
*/
#ifdef ANSI
PUBLIC S16 VeLiNhuDatRsp
(
Pst      *pst, 
SpId     spId,  
NhuDatRspSdus *nhuDatRspSdus
)
#else
PUBLIC S16 VeLiNhuDatRsp(pst, spId, nhuDatRspSdus)
Pst      *pst; 
SpId     spId;  
NhuDatRspSdus *nhuDatRspSdus;
#endif
{
   TRC3(VeLiNhuDatRsp);

   RETVALUE((*VeLiNhuDatRspMt[pst->selector])(pst, spId, nhuDatRspSdus));

} /* end of function NhUiNhuDatRsp */


/* 
* 
*       Fun:   lower interface - Configuration Request
*  
*       Desc:   
*  
*       Ret:   ROK      - ok
* 
*       Notes: None 
* 
*       File:  
*        
*/
#ifdef ANSI
PUBLIC S16 VeLiNhuCfgReq
(
Pst             *pst,
SpId            spId,
NhuCfgReqSdus   *nhuCfgReqSdus
)
#else
PUBLIC S16 VeLiNhuCfgReq(pst, spId, nhuCfgReqSdus)
Pst             *pst;
SpId            spId;
NhuCfgReqSdus   *nhuCfgReqSdus;
#endif
{
   TRC3(VeLiNhuCfgReq);

   RETVALUE((*VeLiNhuCfgReqMt[pst->selector])(pst, spId, nhuCfgReqSdus));

} /* end of function VeLiNhuCfgReq */

/* 
* 
*       Fun:   lower interface - Cancel UE Request
*  
*       Desc:   
*  
*       Ret:   ROK      - ok
* 
*       Notes: None 
* 
*       File:  
*        
*/
#ifdef ANSI
PUBLIC S16 VeLiNhuCnclUeReq
(
Pst         *pst,
SpId        spId,
NhuCellId   cellId,
NhuCrnti    crnti
)
#else
PUBLIC S16 VeLiNhuCnclUeReq(pst, spId, cellId, crnti)
Pst *pst;
SpId spId;
NhuCellId   cellId;
NhuCrnti    crnti;
#endif
{
   TRC3(VeLiNhuCnclUeReq);

   RETVALUE((*VeLiNhuCnclUeReqMt[pst->selector])(pst, spId, cellId, crnti));

} /* end of function VeLiNhuCnclUeReq */

/*
*
*       Fun:   lower interface - Cell Config Request
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:
*
*/
#ifdef ANSI
PUBLIC S16 VeLiNhuCellCfgReq
(
Pst *pst,
SpId spId,
NhuCellCfgReqSdus *nhuCellCfgReqSdus
)
#else
PUBLIC S16 VeLiNhuCellCfgReq(pst, spId, nhuCellCfgReqSdus)
Pst                 *pst;
SpId                spId;
NhuCellCfgReqSdus   *nhuCellCfgReqSdus;
#endif
{
   TRC3(VeLiNhuCellCfgReq);

   RETVALUE((*VeLiNhuCellCfgReqMt[pst->selector])(pst, spId, nhuCellCfgReqSdus));

} /* end of function VeLiNhuCellCfgReq */

/**************************************************************************
 *
 *      Fun:   VeLiNhuDeleteTransReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ve_nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 VeLiNhuDeleteTransReq
(
Pst        *pst,       /* Post Structure */
SpId       spId,       /* Service Provider Id */
NhuCellId  cellId,     /* Cell ID */
NhuCrnti   ueId,       /* Ue Id */
U32        transId     /* transaction id */
)
#else
PUBLIC S16 VeLiNhuDeleteTransReq(pst, spId, cellId, ueId, transId)
Pst        *pst;       /* Post Structure */
SpId       spId;       /* Service Provider Id */
NhuCellId  cellId;     /* Cell ID */
NhuCrnti   ueId;       /* Ue Id */
U32        transId;    /* Trans id */
#endif
{
   TRC3(VeLiNhuDeleteTransReq)
 
   RETVALUE((*VeLiNhuDeleteTransReqMt[pst->selector])(pst, spId, cellId, ueId, transId)); 
}


/**************************************************************************
 *
 *      Fun:   VeLiNhuPdcpSduStaReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ve_nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 VeLiNhuPdcpSduStaReq
(
Pst     *pst,
SuId    suId,
NhuHdr  nhuHdr
)
#else
PUBLIC S16 VeLiNhuPdcpSduStaReq(pst, suId, nhuHdr)
Pst     *pst;
SuId    suId;
NhuHdr  nhuHdr;
#endif
{
   TRC3(VeLiNhuPdcpSduStaReq)
   RETVALUE((*VeLiNhuPdcpSduStaReqMt[pst->selector])(pst, suId, nhuHdr)); 
}


/**************************************************************************
 *
 *      Fun:   VeLiNhuDecodeReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ve_nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 VeLiNhuDecodeReq
(
Pst     *pst,
SuId     suId,
NhuDecodeSduReq *nhuDecodeSduReq
)
#else
PUBLIC S16 VeLiNhuDecodeReq(pst, suId, nhuDecodeSduReq)
Pst     *pst;
SuId     suId;
NhuDecodeSduReq *nhuDecodeSduReq;
#endif
{
   TRC3(VeLiNhuDecodeReq)

   RETVALUE((*VeLiNhuDecodeReqMt[pst->selector])(pst, suId, nhuDecodeSduReq));
}

/**************************************************************************
 *
 *      Fun:   VeLiNhuCountReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ve_nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 VeLiNhuCountReq
(
Pst     *pst,
SuId     suId,
NhuHdr *countHdr
)
#else
PUBLIC S16 VeLiNhuCountReq(pst, suId, countHdr)
Pst     *pst;
SuId     suId;
NhuHdr *countHdr;
#endif
{
   TRC3(VeLiNhuCountReq)

   RETVALUE((*VeLiNhuCountReqMt[pst->selector])(pst, suId, countHdr));
}

/**************************************************************************
 *
 *      Fun:   VeLiNhuEncodeReq
 *
 *      Desc:  Delete Trans Request from RRC User to RRC
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  ve_nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 VeLiNhuEncodeReq
(
Pst     *pst,
SuId     suId,
NhuEncReqSdus *nhuEncReqSdus
)
#else
PUBLIC S16 VeLiNhuEncodeReq(pst, suId, nhuEncReqSdus)
Pst     *pst;
SuId     suId;
NhuEncReqSdus *nhuEncReqSdus;
#endif
{
   TRC3(VeLiNhuEncodeReq)

   RETVALUE((*VeLiNhuEncodeReqMt[pst->selector])(pst, suId, nhuEncReqSdus));
}


#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**

         End of file:     ve_nh_ptli.c@@/main/2 - Wed Dec  1 08:44:27 2010

*********************************************************************31*/


/********************************************************************40**

        Notes:  ve_nh_ptli.c

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
