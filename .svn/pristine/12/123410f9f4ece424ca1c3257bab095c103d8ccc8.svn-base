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

/********************************************************************20**

     Name:    

     Type:    C Include file

     Desc:    This file contains the acc source code

     File:    ve_cz_ptli.c

     Sid:      ve_cz_ptli.c@@/main/eNB_1.3_With_LatestGpr3/2 - Tue Jul 19 14:09:52 2011

     Prg:      
*********************************************************************21*/
#ifdef LTE_HO_SUPPORT

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_inet.h"       /* common INET defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport defines */
#include "cm_pasn.h"       /* Common Asn.1 library           */
#include "cm_dns.h"         /* common DNS libraru defines */
#include "nhu_asn.h"
#include "nhu.h"
#include "pju.h"           /* PJU interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "ctf.h"
#include "sct.h"           /* SB (SCTP) layer */
#include "lcz.h"           /* layer management defines for CZCP */
#include "cz.h"            /* defines and macros for CZ */
#include "szt.h"
#include "czt.h"            /* defines and macros for CZ */
#include "egt.h"           /* S1AP RRM control Interface */
#include "cz_err.h"        /* CZ error defines */

#include "ve.h"
#include "lve.h"
#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#include "hi.h"
#endif

#ifdef    SB
#include "lsb.h"
#include "sb_mtu.h"
#include "sb.h"
#endif    /* SB */


/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_inet.x"       /* common INET */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_tpt.x"        /* common transport types */
#include "cm_pasn.x"       /* Common Asn.1 library           */
#include "cm_dns.x"         /* common DNS libraru defines */

#include "nhu_asn.x"
#include "nhu.x"           /* NHU Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "sct.x"           /* SCTP Interface Structures */
#include "szt.x"
#include "ctf.x"
#ifndef MSPD
#include "ve_dflt.h"
#else
#include "ve_dflt_ms.x"
#endif
#include "czt.x"           /* CZT Interface Structures */
#include "lcz.x"           /* layer management typedefs for CZCP */

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#include "hi.x"
#endif

#ifdef    SB
#include "lsb.x"
#include "sct.x"           /* SB (SCTP) layer */
#include "sb_mtu.x"
#include "sb.x"
#endif    /* SB */
#ifdef EU_DAT_APP
#include "egt.x"           /* S1AP RRM control Interface */
#endif

#include "lve.x"
#include "ve_eut.x"
#include "cz.x"            /* typedefs for CZCP */
#include "ve.x"
#include "szt_asn.x"
/* ---- X2AP service user lower interface ---- */
/*
 *     this software may be combined with the following TRILLIUM
 *     software:
 *
 *     part no.                      description
 *     --------    ----------------------------------------------
 *     1000XXX                      X2AP v 1.1
 */

/* CR Fixes */

/************************************************************************
                              Prototypes  
************************************************************************/
#if (!defined(LWLCRSLICZT) || !defined(LCRSLICZT))
#define PTRSLICZT
#else
#ifndef CZ
#define PTRSLICZT
#else
#endif
#endif

#ifdef PTRSLICZT

PRIVATE S16 PtLiCztBndReq       ARGS((Pst *pst, SuId suId, SpId spId));
PRIVATE S16 PtLiCztUbndReq      ARGS((Pst *pst, SpId spId, Reason reason));
PRIVATE S16 PtLiCztGpReq       ARGS((Pst *pst, SpId spId, CztEvnt *cztEvnt));
PRIVATE S16 PtLiCztRelReq       ARGS((Pst *pst, SpId spId, CztRlsReq *cztRlsReq));
PRIVATE S16 PtLiCztGpRsp       ARGS((Pst *pst, SpId spId, CztEvnt *conRsp));
PRIVATE S16 PtLiCztGpErrReq    ARGS((Pst *pst, SpId spId, CztEvnt *cztEvnt));
PRIVATE S16 PtLiCztBmpReq       ARGS((Pst *pst, SpId spId, CztEvnt *cztEvnt));
PRIVATE S16 PtLiCztBmpRsp       ARGS((Pst *pst, SpId spId, CztEvnt *cztEvnt));
PRIVATE S16 PtLiCztPeerAssocRsp ARGS((Pst *pst, SpId spId, CztPeerAssocRsp *peerAssocRsp));
PRIVATE S16 PtLiCztGpRetxReq    ARGS((Pst *pst, SpId spId, CztReTxInfo *retxInfo));
PRIVATE S16 PtLiCztAudReq       ARGS((Pst *pst, SpId spId, CztAudEvnt  *audEvnt));


#endif


/* Following matrices define the mapping between the primitives called by the
 * X2AP user (as RsLiCztXXXReq) and the corresponding primitives of the 
 * X2AP layer (service provider) (as CzUiCztXXXReq).
 * The mapping is based on pst->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCRSLICZT    loose         cmPkCztXXXReq
 * 1                 CZ          tight         CzUiCztXXXReq
 * 2                LWLCRSLICZT  LW loose      cmPkCztXXXReq
 */


PUBLIC CztBndReq VeLiCztBndReqMt [] =
{
#ifdef LCRSLICZT
   cmPkCztBndReq,        /* 0 - loosely coupled */
#else
   PtLiCztBndReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCRSLICZT
   cmPkCztBndReq,        /* 1 - light weight loosely coupled */
#else
   PtLiCztBndReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef CZ
   CzUiCztBndReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiCztBndReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC CztUbndReq VeLiCztUbndReqMt [] =
{
#ifdef LCRSLICZT
   cmPkCztUbndReq,        /* 0 - loosely coupled */
#else
   PtLiCztUbndReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCRSLICZT
   cmPkCztUbndReq,        /* 1 - light weight loosely coupled */
#else
   PtLiCztUbndReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef CZ
   CzUiCztUbndReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiCztUbndReq,        /* 2 - tightly coupled, portable */
#endif
};



PUBLIC CztGpReq VeLiCztGpReqMt [] =
{
#ifdef LCRSLICZT
   cmPkCztGpReq,        /* 0 - loosely coupled */
#else
   PtLiCztGpReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCRSLICZT
   cmPkCztGpReq,        /* 1 - light weight loosely coupled */
#else
   PtLiCztGpReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef CZ
   CzUiCztGpReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiCztGpReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC CztGpRsp VeLiCztGpRspMt [] =
{
#ifdef LCRSLICZT
   cmPkCztGpRsp,        /* 0 - loosely coupled */
#else
   PtLiCztGpRsp,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCRSLICZT
   cmPkCztGpRsp,        /* 1 - light weight loosely coupled */
#else
   PtLiCztGpRsp,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef CZ
   CzUiCztGpRsp,        /* 2 - tightly coupled, stub layer */
#else
   PtLiCztGpRsp,        /* 2 - tightly coupled, portable */
#endif
};

PUBLIC CztGpErrReq VeLiCztGpErrReqMt [] =
{
#ifdef LCRSLICZT
   cmPkCztGpErrReq,        /* 0 - loosely coupled */
#else
   PtLiCztGpErrReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCRSLICZT
   cmPkCztGpErrReq,        /* 1 - light weight loosely coupled */
#else
   PtLiCztGpErrReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef CZ
   CzUiCztGpErrReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiCztGpErrReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC CztGpRetxReq VeLiCztGpRetxReqMt [] =
{
#ifdef LCRSLICZT
   cmPkCztGpRetxReq,        /* 0 - loosely coupled */
#else
   PtLiCztGpRetxReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCRSLICZT
   cmPkCztGpRetxReq,        /* 1 - light weight loosely coupled */
#else
   PtLiCztGpRetxReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef CZ
   CzUiCztGpRetxReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiCztGpRetxReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC CztRelReq VeLiCztRelReqMt [] =
{
#ifdef LCRSLICZT
   cmPkCztRelReq,        /* 0 - loosely coupled */
#else
   PtLiCztRelReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCRSLICZT
   cmPkCztRelReq,        /* 1 - light weight loosely coupled */
#else
   PtLiCztRelReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef CZ
   CzUiCztRelReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiCztRelReq,        /* 2 - tightly coupled, portable */
#endif
};




PUBLIC CztBmpReq VeLiCztBmpReqMt [] =
{
#ifdef LCRSLICZT
   cmPkCztBmpReq,        /* 0 - loosely coupled */
#else
   PtLiCztBmpReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCRSLICZT
   cmPkCztBmpReq,        /* 1 - light weight loosely coupled */
#else
   PtLiCztBmpReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef CZ
   CzUiCztBmpReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiCztBmpReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC CztBmpRsp VeLiCztBmpRspMt [] =
{
#ifdef LCRSLICZT
   cmPkCztBmpRsp,        /* 0 - loosely coupled */
#else
   PtLiCztBmpRsp,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCRSLICZT
   cmPkCztBmpRsp,        /* 1 - light weight loosely coupled */
#else
   PtLiCztBmpRsp,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef CZ
   CzUiCztBmpRsp,        /* 2 - tightly coupled, stub layer */
#else
   PtLiCztBmpRsp,        /* 2 - tightly coupled, portable */
#endif
};



PUBLIC CztDynamPeerAssocRsp VeLiCztDynamPeerAssocRspMt [] =
{
#ifdef LCRSLICZT
   cmPkCztDynamPeerAssocRsp,   /* 0 - loosely coupled */
#else
   PtLiCztPeerAssocRsp,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCRSLICZT
   cmPkCztDynamPeerAssocRsp,   /* 1 - light weight loosely coupled */
#else
   PtLiCztPeerAssocRsp,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef CZ
   CzUiCztDynamPeerAssocRsp,        /* 2 - tightly coupled, stub layer */
#else
   PtLiCztPeerAssocRsp,        /* 2 - tightly coupled, portable */
#endif
};

PUBLIC CztAudReq VeLiCztAudReqMt [] =
{
#ifdef LCRSLICZT
   cmPkCztAudReq,        /* 0 - loosely coupled */
#else
   PtLiCztAudReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCRSLICZT
   cmPkCztAudReq,        /* 1 - light weight loosely coupled */
#else
   PtLiCztAudReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef CZ
   CzUiCztAudReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiCztAudReq,        /* 2 - tightly coupled, portable */
#endif
};

/************************************************************************
                       CZT Interface Functions
************************************************************************/

/*
 *
 *       Fun:   VeLiCztBndReq
 *
 *       Desc:  This function resolves the CztBndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiCztBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else 
PUBLIC S16 VeLiCztBndReq (pst, suId, spId)
Pst *pst;
SuId suId; 
SpId spId;
#endif
{
   TRC3(VeLiCztBndReq)

   (*VeLiCztBndReqMt[pst->selector])
      (pst, suId, spId);

   RETVALUE(ROK);
} /* VeLiCztBndReq */

/*
 *
 *       Fun:   VeLiCztUbndReq
 *
 *       Desc:  This function resolves the CztUbndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiCztUbndReq
(
Pst *pst,
SpId spId,
Reason reason
)
#else 
PUBLIC S16 VeLiCztUbndReq (pst, spId, reason)
Pst *pst;
SpId spId;
Reason reason;
#endif
{
   TRC3(VeLiCztUbndReq)

   (*VeLiCztUbndReqMt[pst->selector])
      (pst, spId, reason);

   RETVALUE(ROK);
} /* VeLiCztUbndReq */


/*
 *
 *       Fun:   VeLiCztGpRetxReq
 *
 *       Desc:  This function resolves the CztGpRetxReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiCztGpRetxReq
(
Pst *pst,
SpId spId,
CztReTxInfo *cztReTxInfo
)
#else 
PUBLIC S16 VeLiCztGpRetxReq (pst, spId, cztReTxInfo)
Pst *pst;
SpId spId;
CztReTxInfo *cztReTxInfo;
#endif
{
   TRC3(VeLiCztGpRetxReq)

   (*VeLiCztGpRetxReqMt[pst->selector])
      (pst, spId, cztReTxInfo);

   RETVALUE(ROK);
} /* VeLiCztGpRetxReq */


/*
 *
 *       Fun:   VeLiCztGpReq
 *
 *       Desc:  This function resolves the CztGpReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiCztGpReq
(
Pst *pst,
SpId spId,
CztEvnt *cztEvnt
)
#else 
PUBLIC S16 VeLiCztGpReq (pst, spId, cztEvnt)
Pst *pst;
SpId spId;
CztEvnt *cztEvnt;
#endif
{
   TRC3(VeLiCztGpReq)

   (*VeLiCztGpReqMt[pst->selector])
      (pst, spId, cztEvnt);

   RETVALUE(ROK);
} /* VeLiCztGpReq */

/*
 *
 *       Fun:   VeLiCztGpRsp
 *
 *       Desc:  This function resolves the CztGpRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiCztGpRsp
(
Pst *pst,
SpId spId,
CztEvnt *cztEvnt
)
#else 
PUBLIC S16 VeLiCztGpRsp (pst, spId, cztEvnt)
Pst *pst;
SpId spId;
CztEvnt *cztEvnt;
#endif
{
   TRC3(VeLiCztGpRsp)

   (*VeLiCztGpRspMt[pst->selector])
      (pst, spId, cztEvnt);

   RETVALUE(ROK);
} /* VeLiCztGpRsp */
/*
 *
 *       Fun:   VeLiCztGpErrReq
 *
 *       Desc:  This function resolves the CztGpErrReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiCztGpErrReq
(
Pst *pst,
SpId spId,
CztEvnt *cztEvnt
)
#else 
PUBLIC S16 VeLiCztGpErrReq (pst, spId, cztEvnt)
Pst *pst;
SpId spId;
CztEvnt *cztEvnt;
#endif
{
   TRC3(VeLiCztGpErrReq)

   (*VeLiCztGpErrReqMt[pst->selector])
      (pst, spId, cztEvnt);

   RETVALUE(ROK);
} /* VeLiCztGpReq */


/*
 *
 *       Fun:   VeLiCztRelReq
 *
 *       Desc:  This function resolves the CztRlsReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiCztRelReq
(
Pst *pst,
SpId spId,
CztRlsReq *relReq
)
#else 
PUBLIC S16 VeLiCztRelReq (pst, spId, relReq)
Pst *pst;
SpId spId;
CztRlsReq *relReq;
#endif
{
   TRC3(VeLiCztRelReq)

   (*VeLiCztRelReqMt[pst->selector])
      (pst, spId, relReq);

   RETVALUE(ROK);
} /* VeLiCztRelReq */

/*
 *
 *       Fun:   VeLiCztBmpReq
 *
 *       Desc:  This function resolves the CztBmpReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiCztBmpReq
(
Pst *pst,
SpId spId,
CztEvnt *cztEvnt
)
#else 
PUBLIC S16 VeLiCztBmpReq (pst, spId, cztEvnt)
Pst *pst;
SpId spId;
CztEvnt *cztEvnt;
#endif
{
   TRC3(VeLiCztBmpReq);

  if(NULLP == pst)
  {
     CZLOGERROR(ERRCLS_DEBUG, ECZXXX, (ErrVal) 0,
              "pst is NULL.");
     RETVALUE(RFAILED);
  } 

   (*VeLiCztBmpReqMt[pst->selector])
      (pst, spId, cztEvnt);

   RETVALUE(ROK);
} /* VeLiCztBmpReq */

/*
 *
 *       Fun:   VeLiCztBmpRsp
 *
 *       Desc:  This function resolves the CztBmpRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiCztBmpRsp
(
Pst *pst,
SpId spId,
CztEvnt *cztEvnt
)
#else 
PUBLIC S16 VeLiCztBmpRsp (pst, spId, cztEvnt)
Pst *pst;
SpId spId;
CztEvnt *cztEvnt;
#endif
{
   TRC3(VeLiCztBmpRsp)

   (*VeLiCztBmpRspMt[pst->selector])
      (pst, spId, cztEvnt);

   RETVALUE(ROK);
} /* VeLiCztBmpRsp */

/*
 *
 *       Fun:   VeLiCztPeerAssocRsp
 *
 *       Desc:  This function resolves the CztPeerAssocRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiCztDynamPeerAssocRsp
(
Pst *pst,
SpId spId,
CztPeerAssocRsp *peerAssocRsp
)
#else 
PUBLIC S16 VeLiCztDynamPeerAssocRsp (pst, spId, peerAssocRsp)
Pst *pst;
SpId spId;
CztPeerAssocRsp *peerAssocRsp;
#endif
{
   TRC3(VeLiCztPeerAssocRsp)

   (*VeLiCztDynamPeerAssocRspMt[pst->selector])
      (pst, spId, peerAssocRsp);

   RETVALUE(ROK);
} /* VeLiCztDynamPeerAssocRsp */


/*
 *
 *       Fun:   VeLiCztAudReq
 *
 *       Desc:  This function resolves the CztAudReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiCztAudReq
(
Pst *pst,
SpId spId,
CztAudEvnt          *audEvnt
)
#else 
PUBLIC S16 VeLiCztAudReq (pst, spId, audEvnt)
Pst *pst;
SpId spId;
CztAudEvnt          *audEvnt;
#endif
{
   TRC3(VeLiCztAudReq)

   (*VeLiCztAudReqMt[pst->selector]) (pst, spId, audEvnt);

   RETVALUE(ROK);
} /* VeLiCztAudReq */


/************************************************************************
                    Indications/Confirms from X2AP
************************************************************************/

/*
 *
 *       Fun:   RsLiCztBndCfm
 *
 *       Desc:  This function resolves the CztBndCfm primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 RsLiCztBndCfm
(
Pst   *pst,
SpId  spId,
U8    status
)
#else 
PUBLIC S16 RsLiCztBndCfm (pst, spId, status)
Pst   *pst;
SpId  spId;
U8    status;
#endif
{
   TRC3(RsLiCztBndCfm);

   VeLiCztBndCfm (pst, spId, status);

   RETVALUE(ROK);
} /* RsLiCztBndCfm */

/*
 *
 *       Fun:   RsLiCztLclErrorInd
 *
 *       Desc:  This function resolves the CztLclErrInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 RsLiCztLclErrorInd
(
Pst   *pst,
SpId  spId,
CztLclErrInd      *cztLclErrInd
)
#else 
PUBLIC S16 RsLiCztLclErrorInd(pst, spId, cztLclErrInd)
Pst   *pst;
SpId  spId;
CztLclErrInd      *cztLclErrInd;
#endif
{
   TRC3(RsLiCztLclErrorInd);

   VeLiCztLclErrorInd(pst, spId, cztLclErrInd);

   RETVALUE(ROK);
} /* RsLiCztLclErrorInd  */


/************************************************************************
                       Portable Functions
************************************************************************/

#ifdef PTRSLICZT
/*
 *
 *       Fun:   PtLiCztBndReq
 *
 *       Desc:  Portable version of CztBndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiCztBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else 
PRIVATE S16 PtLiCztBndReq (pst, suId, spId)
Pst *pst;
SuId suId; 
SpId spId;
#endif
{
   TRC3(PtLiCztBndReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   CZLOGERROR(ERRCLS_DEBUG, ECZXXX, (ErrVal) 0, 
              "PtLiCztBndReq() called");
#endif
   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);

   RETVALUE(RFAILED);
} /* PtLiCztBndReq */

/*
 *
 *       Fun:   PtLiCztUbndReq
 *
 *       Desc:  Portable version of CztUbndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiCztUbndReq
(
Pst *pst,
SpId spId,
Reason reason
)
#else 
PRIVATE S16 PtLiCztUbndReq (pst, spId, reason)
Pst *pst;
SpId spId;
Reason reason;
#endif
{
   TRC3(PtLiCztUbndReq)

   UNUSED(pst);
   UNUSED(spId);

   RETVALUE(RFAILED);
} /* PtLiCztUbndReq */


/*
 *
 *       Fun:   PtLiCztGpErrReq
 *
 *       Desc:  Portable version of CztGpErrReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiCztGpErrReq
(
Pst *pst,
SpId spId,
CztEvnt *cztEvnt
)
#else 
PRIVATE S16 PtLiCztGpErrReq (pst, spId, cztEvnt)
Pst *pst;
SpId spId;
CztEvnt *cztEvnt;
#endif
{
   TRC3(PtLiCztGpErrReq)


   UNUSED(pst);
   UNUSED(spId);
   UNUSED(cztEvnt);

   RETVALUE(RFAILED);
} /* PtLiCztGpReq */


/*
 *
 *       Fun:   PtLiCztGpReq
 *
 *       Desc:  Portable version of CztGpReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiCztGpReq
(
Pst *pst,
SpId spId,
CztEvnt *cztEvnt
)
#else 
PRIVATE S16 PtLiCztGpReq (pst, spId, cztEvnt)
Pst *pst;
SpId spId;
CztEvnt *cztEvnt;
#endif
{
   TRC3(PtLiCztGpReq)


   UNUSED(pst);
   UNUSED(spId);
   UNUSED(cztEvnt);

   RETVALUE(RFAILED);
} /* PtLiCztGpReq */

/*
 *
 *       Fun:   PtLiCztGpRsp
 *
 *       Desc:  Portable version of CztGpRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiCztGpRsp
(
Pst *pst,
SpId spId,
CztEvnt *conRsp
)
#else 
PRIVATE S16 PtLiCztGpRsp (pst, spId, conRsp)
Pst *pst;
SpId spId;
CztEvnt *conRsp;
#endif
{
   TRC3(PtLiCztGpRsp)


   UNUSED(pst);
   UNUSED(spId);
   UNUSED(conRsp);

   RETVALUE(RFAILED);
} /* PtLiCztGpRsp */

/*
 *
 *       Fun:   PtLiCztRelReq
 *
 *       Desc:  Portable version of CztRlsReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiCztRelReq
(
Pst *pst,
SpId spId,
CztRlsReq *relReq
)
#else 
PRIVATE S16 PtLiCztRelReq (pst, spId, relReq)
Pst *pst;
SpId spId;
CztRlsReq *relReq;
#endif
{
   TRC3(PtLiCztRelReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(relReq);

   RETVALUE(RFAILED);
} /* PtLiCztRelReq */


/*
 *
 *       Fun:   PtLiCztBmpReq
 *
 *       Desc:  Portable version of CztBmpReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiCztBmpReq
(
Pst *pst,
SpId spId,
CztEvnt *cztEvnt
)
#else 
PRIVATE S16 PtLiCztBmpReq (pst, spId, cztEvnt)
Pst *pst;
SpId spId;
CztEvnt *cztEvnt;
#endif
{
   TRC3(PtLiCztBmpReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(cztEvnt);

   RETVALUE(RFAILED);
} /* PtLiCztBmpReq */

/*
 *
 *       Fun:   PtLiCztBmpRsp
 *
 *       Desc:  Portable version of CztBmpRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiCztBmpRsp
(
Pst *pst,
SpId spId,
CztEvnt *cztEvnt
)
#else 
PRIVATE S16 PtLiCztBmpRsp (pst, spId, cztEvnt)
Pst *pst;
SpId spId;
CztEvnt *cztEvnt;
#endif
{
   TRC3(PtLiCztBmpRsp)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(cztEvnt);

   RETVALUE(RFAILED);
} /* PtLiCztBmpRsp */

/*
 *
 *       Fun:   PtLiCztPeerAssocRsp
 *
 *       Desc:  Portable version of CztPeerAssocRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiCztPeerAssocRsp
(
Pst *pst,
SpId spId,
CztPeerAssocRsp *peerAssocRsp
)
#else 
PRIVATE S16 PtLiCztPeerAssocRsp (pst, spId, peerAssocRsp)
Pst *pst;
SpId spId;
CztPeerAssocRsp *peerAssocRsp;
#endif
{
   TRC3(PtLiCztPeerAssocRsp)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(peerAssocRsp);

   RETVALUE(RFAILED);
} /* PtLiCztPeerAssocRsp */

/*
 *
 *       Fun:   PtLiCztGpRetxReq
 *
 *       Desc:  This function resolves the CztGpRetxReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 PtLiCztGpRetxReq
(
Pst *pst,
SpId spId,
CztReTxInfo *cztReTxInfo
)
#else 
PUBLIC S16 PtLiCztGpRetxReq (pst, spId, cztReTxInfo)
Pst *pst;
SpId spId;
CztReTxInfo *cztReTxInfo;
#endif
{
   TRC3(PtLiCztGpRetxReq)


   RETVALUE(ROK);
} /* PtLiCztGpRetxReq */


/*
 *
 *       Fun:   PtLiCztAudReq
 *
 *       Desc:  Portable version of CztAudReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiCztAudReq
(
Pst *pst,
SpId spId,
CztAudEvnt *cztEvnt
)
#else 
PRIVATE S16 PtLiCztAudReq (pst, spId, cztEvnt)
Pst *pst;
SpId spId;
CztAudEvnt *cztEvnt;
#endif
{
   TRC3(PtLiCztAudReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(cztEvnt);

   RETVALUE(RFAILED);
} /* PtLiCztAudReq */

#endif /* PTRSLICZT */
#endif /* LTE_HO_SUPPORT */


/********************************************************************30**

         End of file:     ve_cz_ptli.c@@/main/eNB_1.3_With_LatestGpr3/2 - Tue Jul 19 14:09:52 2011

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
------------ -------- ---- ----------------------------------------------
$SID$     ----       mnawas        1. eNodeb 3.1 Release
*********************************************************************91*/
