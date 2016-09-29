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

     Name:    S1AP - acc test file

     Type:    C Include file

     Desc:    This file contains the acc source code

     File:    szac_lszhdl.c

     Sid:      ve_sz_ptli.c@@/main/2 - Wed Dec  1 08:44:41 2010

     Prg:      
*********************************************************************21*/

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
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_pasn.h"       /* Common Pasn includes */
#include "cm_tpt.h"        /* common transport defines */
#include "nhu.h"            /* defines and macros for SZ */
#include "szt.h"            /* defines and macros for SZ */
#include "egt.h"
#include "ve_eut.h"
#include "rgr.h"
#include "ctf.h"
#include "pju.h"
#include "lve.h"
#include "ve.h"            /* defines and macros for SZ */

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
#include "cm_lte.x"       /*  Common UMTS includes */
#include "cm_lib.x"
#include "cm_pasn.x"       /* Common Pasn includes */

#include "nhu.x"           /* SZT Interface Structures */
#include "szt.x"           /* SZT Interface Structures */
#include "czt.x"
#include "egt.x"
#include "ve_eut.x"
#include "rgr.x"
#include "ctf.x"
#include "pju.x"
#include "lve.x"
#include "ve.x"


/* ---- S1AP service user lower interface ---- */
/*
 *     this software may be combined with the following TRILLIUM
 *     software:
 *
 *     part no.                      description
 *     --------    ----------------------------------------------
 *     1000XXX                      S1AP v 1.1
 */


/************************************************************************
                              Prototypes  
************************************************************************/

#ifdef PTUZLISZT

PRIVATE S16 PtLiSztBndReq       ARGS((Pst *pst, SuId suId, SpId spId));
PRIVATE S16 PtLiSztUbndReq      ARGS((Pst *pst, SpId spId, Reason reason));
PRIVATE S16 PtLiSztUDatReq      ARGS((Pst *pst, SpId spId, SztUDatEvnt *uDatEvnt));
PRIVATE S16 PtLiSztConReq       ARGS((Pst *pst, SpId spId, SztConReq *conReq));
PRIVATE S16 PtLiSztConRsp       ARGS((Pst *pst, SpId spId, SztConRsp *conRsp));
PRIVATE S16 PtLiSztRelReq       ARGS((Pst *pst, SpId spId, SztRelReq *relReq));
PRIVATE S16 PtLiSztRelRsp       ARGS((Pst *pst, SpId spId, SztRelRsp *relRsp));
PRIVATE S16 PtLiSztDatReq       ARGS((Pst *pst, SpId spId, SztDatEvntReq *datEvnt));
PRIVATE S16 PtLiSztEncDecReq    ARGS((Pst *pst, SpId spId, SztEncDecEvnt *sztEncDecEvnt));
PRIVATE S16 PtLiSztAudReq       ARGS((Pst *pst, SpId spId, SztAudEvnt *audEvnt));
#if (defined(SZ_MME) && defined(SZ_DYNAMIC_PEER))
PRIVATE S16 PtLiSztPeerAssocRsp ARGS((Pst *pst, SpId spId, SztPeerAssocRsp *peerAssocRsp));
#endif

#endif


/* Following matrices define the mapping between the primitives called by the
 * S1AP user (as VeLiSztXXXReq) and the corresponding primitives of the 
 * S1AP layer (service provider) (as SzUiSztXXXReq).
 * The mapping is based on pst->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCUZLISZT    loose         cmPkSztXXXReq
 * 1                 SZ          tight         SzUiSztXXXReq
 * 2                LWLCUZLISZT  LW loose      cmPkSztXXXReq
 */


PUBLIC SztBndReq VeLiSztBndReqMt [] =
{
#ifdef LCUZLISZT
   cmPkSztBndReq,        /* 0 - loosely coupled */
#else
   PtLiSztBndReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztBndReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztBndReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztBndReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztBndReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztUbndReq VeLiSztUbndReqMt [] =
{
#ifdef LCUZLISZT
   cmPkSztUbndReq,        /* 0 - loosely coupled */
#else
   PtLiSztUbndReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztUbndReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztUbndReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztUbndReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztUbndReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztUDatReq VeLiSztUDatReqMt [] =
{
#ifdef LCUZLISZT
   cmPkSztUDatReq,        /* 0 - loosely coupled */
#else
   PtLiSztUDatReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztUDatReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztUDatReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztUDatReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztUDatReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztConnReq VeLiSztConReqMt [] =
{
#ifdef LCUZLISZT
   cmPkSztConReq,        /* 0 - loosely coupled */
#else
   PtLiSztConReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztConReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztConReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztConReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztConReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztConnRsp VeLiSztConRspMt [] =
{
#ifdef LCUZLISZT
   cmPkSztConRsp,        /* 0 - loosely coupled */
#else
   PtLiSztConRsp,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztConRsp,        /* 1 - light weight loosely coupled */
#else
   PtLiSztConRsp,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztConRsp,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztConRsp,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztRelsReq VeLiSztRelReqMt [] =
{
#ifdef LCUZLISZT
   cmPkSztRelReq,        /* 0 - loosely coupled */
#else
   PtLiSztRelReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztRelReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztRelReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztRelReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztRelReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztRelsRsp VeLiSztRelRspMt [] =
{
#ifdef LCUZLISZT
   cmPkSztRelRsp,        /* 0 - loosely coupled */
#else
   PtLiSztRelRsp,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztRelRsp,        /* 1 - light weight loosely coupled */
#else
   PtLiSztRelRsp,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztRelRsp,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztRelRsp,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztDatReq VeLiSztDatReqMt [] =
{
#ifdef LCUZLISZT
   cmPkSztDatReq,        /* 0 - loosely coupled */
#else
   PtLiSztDatReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztDatReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztDatReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztDatReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztDatReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztEncDecReq VeLiSztEncDecReqMt [] =
{
#ifdef LCUZLISZT
   cmPkSztEncDecReq,        /* 0 - loosely coupled */
#else
   PtLiSztEncDecReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztEncDecReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztEncDecReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztEncDecReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztEncDecReq,        /* 2 - tightly coupled, portable */
#endif
};

PUBLIC SztAudReq VeLiSztAudReqMt [] =
{
#ifdef LCUZLISZT
   cmPkSztAudReq,        /* 0 - loosely coupled */
#else
   PtLiSztAudReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztAudReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztAudReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztAudReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztAudReq,        /* 2 - tightly coupled, portable */
#endif
};

#if (defined(SZ_MME) && defined(SZ_DYNAMIC_PEER))
PUBLIC SztDynamPeerAssocRsp VeLiSztPeerAssocRspMt [] =
{
#ifdef LCUZLISZT
   cmPkSztPeerAssocRsp,        /* 0 - loosely coupled */
#else
   PtLiSztPeerAssocRsp,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCUZLISZT
   cmPkSztPeerAssocRsp,        /* 1 - light weight loosely coupled */
#else
   PtLiSztPeerAssocRsp,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztPeerAssocRsp,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztPeerAssocRsp,        /* 2 - tightly coupled, portable */
#endif
};
#endif

/************************************************************************
                       SZT Interface Functions
************************************************************************/

/*
 *
 *       Fun:   VeLiSztBndReq
 *
 *       Desc:  This function resolves the SztBndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiSztBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else 
PUBLIC S16 VeLiSztBndReq (pst, suId, spId)
Pst *pst;
SuId suId; 
SpId spId;
#endif
{
   TRC3(VeLiSztBndReq)

   (*VeLiSztBndReqMt[pst->selector])
      (pst, suId, spId);

   RETVALUE(ROK);
} /* VeLiSztBndReq */

/*
 *
 *       Fun:   VeLiSztUbndReq
 *
 *       Desc:  This function resolves the SztUbndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiSztUbndReq
(
Pst *pst,
SpId spId,
Reason reason
)
#else 
PUBLIC S16 VeLiSztUbndReq (pst, spId, reason)
Pst *pst;
SpId spId;
Reason reason;
#endif
{
   TRC3(VeLiSztUbndReq)

   (*VeLiSztUbndReqMt[pst->selector])
      (pst, spId, reason);

   RETVALUE(ROK);
} /* VeLiSztUbndReq */

/*
 *
 *       Fun:   VeLiSztUDatReq
 *
 *       Desc:  This function resolves the SztUDatReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiSztUDatReq
(
Pst *pst,
SpId spId,
SztUDatEvnt *uDatEvnt
)
#else 
PUBLIC S16 VeLiSztUDatReq (pst, spId, uDatEvnt)
Pst *pst;
SpId spId;
SztUDatEvnt *uDatEvnt;
#endif
{
   TRC3(VeLiSztUDatReq)

   (*VeLiSztUDatReqMt[pst->selector])
      (pst, spId, uDatEvnt);

   RETVALUE(ROK);
} /* VeLiSztUDatReq */

/*
 *
 *       Fun:   VeLiSztConReq
 *
 *       Desc:  This function resolves the SztConReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiSztConReq
(
Pst *pst,
SpId spId,
SztConReq *conReq
)
#else 
PUBLIC S16 VeLiSztConReq (pst, spId, conReq)
Pst *pst;
SpId spId;
SztConReq *conReq;
#endif
{
   TRC3(VeLiSztConReq)

   (*VeLiSztConReqMt[pst->selector])
      (pst, spId, conReq);

   RETVALUE(ROK);
} /* VeLiSztConReq */

/*
 *
 *       Fun:   VeLiSztConRsp
 *
 *       Desc:  This function resolves the SztConRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiSztConRsp
(
Pst *pst,
SpId spId,
SztConRsp *conRsp
)
#else 
PUBLIC S16 VeLiSztConRsp (pst, spId, conRsp)
Pst *pst;
SpId spId;
SztConRsp *conRsp;
#endif
{
   TRC3(VeLiSztConRsp)

   (*VeLiSztConRspMt[pst->selector])
      (pst, spId, conRsp);

   RETVALUE(ROK);
} /* VeLiSztConRsp */

/*
 *
 *       Fun:   VeLiSztRelReq
 *
 *       Desc:  This function resolves the SztRelReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiSztRelReq
(
Pst *pst,
SpId spId,
SztRelReq *relReq
)
#else 
PUBLIC S16 VeLiSztRelReq (pst, spId, relReq)
Pst *pst;
SpId spId;
SztRelReq *relReq;
#endif
{
   TRC3(VeLiSztRelReq)

   (*VeLiSztRelReqMt[pst->selector])
      (pst, spId, relReq);

   RETVALUE(ROK);
} /* VeLiSztRelReq */

/*
 *
 *       Fun:   VeLiSztRelRsp
 *
 *       Desc:  This function resolves the SztRelRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiSztRelRsp
(
Pst *pst,
SpId spId,
SztRelRsp *relRsp
)
#else 
PUBLIC S16 VeLiSztRelRsp (pst, spId, relRsp)
Pst *pst;
SpId spId;
SztRelRsp *relRsp;
#endif
{
   TRC3(VeLiSztRelRsp)

   (*VeLiSztRelRspMt[pst->selector])
      (pst, spId, relRsp);

   RETVALUE(ROK);
} /* VeLiSztRelRsp */

/*
 *
 *       Fun:   VeLiSztDatReq
 *
 *       Desc:  This function resolves the SztDatReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiSztDatReq
(
Pst *pst,
SpId spId,
SztDatEvntReq *datEvnt
)
#else 
PUBLIC S16 VeLiSztDatReq (pst, spId, datEvnt)
Pst *pst;
SpId spId;
SztDatEvntReq *datEvnt;
#endif
{
   TRC3(VeLiSztDatReq);

   if(NULLP == pst)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0,
              "pst is NULL.");
#endif
      RETVALUE(RFAILED);
   }

   (*VeLiSztDatReqMt[pst->selector])
      (pst, spId, datEvnt);

   RETVALUE(ROK);
} /* VeLiSztDatReq */

/*
 *
 *       Fun:   VeLiSztEncDecReq
 *
 *       Desc:  This function resolves the SztEncDecReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiSztEncDecReq
(
Pst *pst,
SpId spId,
SztEncDecEvnt *sztEncDecEvnt
)
#else 
PUBLIC S16 VeLiSztEncDecReq (pst, spId, sztEncDecEvnt)
Pst *pst;
SpId spId;
SztEncDecEvnt *sztEncDecEvnt;
#endif
{
   TRC3(VeLiSztEncDecReq)

   (*VeLiSztEncDecReqMt[pst->selector])
      (pst, spId, sztEncDecEvnt);

   RETVALUE(ROK);
} /* VeLiSztEncDecReq */

/*
 *
 *       Fun:   VeLiSztAudReq
 *
 *       Desc:  This function resolves the SztAudReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiSztAudReq
(
Pst *pst,
SpId spId,
SztAudEvnt *audEvnt
)
#else 
PUBLIC S16 VeLiSztAudReq (pst, spId, audEvnt)
Pst *pst;
SpId spId;
SztAudEvnt *audEvnt;
#endif
{
   TRC3(VeLiSztAudReq)

   (*VeLiSztAudReqMt[pst->selector])
      (pst, spId, audEvnt);

   RETVALUE(ROK);
} /* VeLiSztAudReq */

#if (defined(SZ_MME) && defined(SZ_DYNAMIC_PEER))
/*
 *
 *       Fun:   VeLiSztPeerAssocRsp
 *
 *       Desc:  This function resolves the SztPeerAssocRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiSztPeerAssocRsp
(
Pst *pst,
SpId spId,
SztPeerAssocRsp *peerAssocRsp
)
#else 
PUBLIC S16 VeLiSztPeerAssocRsp (pst, spId, peerAssocRsp)
Pst *pst;
SpId spId;
SztPeerAssocRsp *peerAssocRsp;
#endif
{
   TRC3(VeLiSztPeerAssocRsp)

   (*VeLiSztPeerAssocRspMt[pst->selector])
      (pst, spId, peerAssocRsp);

   RETVALUE(ROK);
} /* VeLiSztPeerAssocRsp */
#endif



/************************************************************************
                       Portable Functions
************************************************************************/

#ifdef PTUZLISZT
/*
 *
 *       Fun:   PtLiSztBndReq
 *
 *       Desc:  Portable version of SztBndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else 
PRIVATE S16 PtLiSztBndReq (pst, suId, spId)
Pst *pst;
SuId suId; 
SpId spId;
#endif
{
   TRC3(PtLiSztBndReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztBndReq() called");
#endif
   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);

   RETVALUE(RFAILED);
} /* PtLiSztBndReq */

/*
 *
 *       Fun:   PtLiSztUbndReq
 *
 *       Desc:  Portable version of SztUbndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztUbndReq
(
Pst *pst,
SpId spId,
Reason reason
)
#else 
PRIVATE S16 PtLiSztUbndReq (pst, spId, reason)
Pst *pst;
SpId spId;
Reason reason;
#endif
{
   TRC3(PtLiSztUbndReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztUbndReq() called");
#endif
   UNUSED(pst);
   UNUSED(spId);

   RETVALUE(RFAILED);
} /* PtLiSztUbndReq */

/*
 *
 *       Fun:   PtLiSztUDatReq
 *
 *       Desc:  Portable version of SztUDatReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztUDatReq
(
Pst *pst,
SpId spId,
SztUDatEvnt *uDatEvnt
)
#else 
PRIVATE S16 PtLiSztUDatReq (pst, spId, uDatEvnt)
Pst *pst;
SpId spId;
SztUDatEvnt *uDatEvnt;
#endif
{
   TRC3(PtLiSztUDatReq)


#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztUDatReq() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(uDatEvnt);

   RETVALUE(RFAILED);
} /* PtLiSztUDatReq */

/*
 *
 *       Fun:   PtLiSztConReq
 *
 *       Desc:  Portable version of SztConReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztConReq
(
Pst *pst,
SpId spId,
SztConReq *conReq
)
#else 
PRIVATE S16 PtLiSztConReq (pst, spId, conReq)
Pst *pst;
SpId spId;
SztConReq *conReq;
#endif
{
   TRC3(PtLiSztConReq)


#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztConReq() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(conReq);

   RETVALUE(RFAILED);
} /* PtLiSztConReq */

/*
 *
 *       Fun:   PtLiSztConRsp
 *
 *       Desc:  Portable version of SztConRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztConRsp
(
Pst *pst,
SpId spId,
SztConRsp *conRsp
)
#else 
PRIVATE S16 PtLiSztConRsp (pst, spId, conRsp)
Pst *pst;
SpId spId;
SztConRsp *conRsp;
#endif
{
   TRC3(PtLiSztConRsp)


#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztConRsp() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(conRsp);

   RETVALUE(RFAILED);
} /* PtLiSztConRsp */

/*
 *
 *       Fun:   PtLiSztRelReq
 *
 *       Desc:  Portable version of SztRelReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztRelReq
(
Pst *pst,
SpId spId,
SztRelReq *relReq
)
#else 
PRIVATE S16 PtLiSztRelReq (pst, spId, relReq)
Pst *pst;
SpId spId;
SztRelReq *relReq;
#endif
{
   TRC3(PtLiSztRelReq)


#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztRelReq() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(relReq);

   RETVALUE(RFAILED);
} /* PtLiSztRelReq */

/*
 *
 *       Fun:   PtLiSztRelRsp
 *
 *       Desc:  Portable version of SztRelRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztRelRsp
(
Pst *pst,
SpId spId,
SztRelRsp *relRsp
)
#else 
PRIVATE S16 PtLiSztRelRsp (pst, spId, relRsp)
Pst *pst;
SpId spId;
SztRelRsp *relRsp;
#endif
{
   TRC3(PtLiSztRelRsp)


#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztRelRsp() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(relRsp);

   RETVALUE(RFAILED);
} /* PtLiSztRelRsp */

/*
 *
 *       Fun:   PtLiSztDatReq
 *
 *       Desc:  Portable version of SztDatReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztDatReq
(
Pst *pst,
SpId spId,
SztDatEvntReq *datEvnt
)
#else 
PRIVATE S16 PtLiSztDatReq (pst, spId, datEvnt)
Pst *pst;
SpId spId;
SztDatEvntReq *datEvnt;
#endif
{
   TRC3(PtLiSztDatReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztDatReq() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(datEvnt);

   RETVALUE(RFAILED);
} /* PtLiSztDatReq */

/*
 *
 *       Fun:   PtLiSztEncDecReq
 *
 *       Desc:  Portable version of SztEncDecReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztEncDecReq
(
Pst *pst,
SpId spId,
SztEncDecEvnt *sztEncDecEvnt
)
#else 
PRIVATE S16 PtLiSztEncDecReq (pst, spId, sztEncDecEvnt)
Pst *pst;
SpId spId;
SztEncDecEvnt *sztEncDecEvnt;
#endif
{
   TRC3(PtLiSztEncDecReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztEncDecReq() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(sztEncDecEvnt);

   RETVALUE(RFAILED);
} /* PtLiSztEncDecReq */

/*
 *
 *       Fun:   PtLiSztAudReq
 *
 *       Desc:  Portable version of SztAudReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztAudReq
(
Pst *pst,
SpId spId,
SztAudEvnt *audEvnt
)
#else 
PRIVATE S16 PtLiSztAudReq (pst, spId, audEvnt)
Pst *pst;
SpId spId;
SztAudEvnt *audEvnt;
#endif
{
   TRC3(PtLiSztAudReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztAudReq() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(audEvnt);

   RETVALUE(RFAILED);
} /* PtLiSztAudReq */

#if (defined(SZ_MME) && defined(SZ_DYNAMIC_PEER))
/*
 *
 *       Fun:   PtLiSztPeerAssocRsp
 *
 *       Desc:  Portable version of SztPeerAssocRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_sz_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztPeerAssocRsp
(
Pst *pst,
SpId spId,
SztPeerAssocRsp *peerAssocRsp
)
#else 
PRIVATE S16 PtLiSztPeerAssocRsp (pst, spId, peerAssocRsp)
Pst *pst;
SpId spId;
SztPeerAssocRsp *peerAssocRsp;
#endif
{
   TRC3(PtLiSztPeerAssocRsp)


#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZXXX, (ErrVal) 0, 
              "PtLiSztPeerAssocRsp() called");
#endif
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(peerAssocRsp);

   RETVALUE(RFAILED);
} /* PtLiSztPeerAssocRsp */
#endif

#endif /* PTUZLISZT */

/*S1AP mapping functions*/


/*
*
*       Fun:   UzLiSztBndCfm
*       
*       Desc:  Call handler for Bind Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_sz_ptli.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztBndCfm
(
Pst            *pst,
SuId           suId,
U8             status
)
#else
PUBLIC S16 UzLiSztBndCfm(pst, suId, status)
Pst            *pst;
SuId           suId;
U8             status;
#endif
{
   TRC2(UzLiSztBndCfm);
 
   VeLiSztBndCfm(pst, suId, status);

   RETVALUE(ROK);
} /* End of UzLiSztBndCfm */

/*
*
*       Fun:   UzLiSztAudCfm
*
*       Desc:  Call handler for Aduit Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_sz_ptli.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztAudCfm
(
Pst            *pst,
SuId           suId,
SztAudEvnt     *audEvnt
)
#else
PUBLIC S16 UzLiSztAudCfm(pst, suId, audEvnt)
Pst            *pst;
SuId           suId;
SztAudEvnt     *audEvnt;
#endif /* ANSI */
{
   TRC2(UzLiSztAudCfm);
   
   RETVALUE(ROK);
} /* End of UzLiSztAudCfm */

/*
*
*       Fun:   UzLiSztStaInd
*
*       Desc:  Call handler for Status Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_sz_ptli.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztStaInd
(
Pst            *post,
SuId           suId,
SztStaInd      *sztSta
)
#else
PUBLIC S16 UzLiSztStaInd(post, suId, sztSta)
Pst            *post;
SuId           suId;
SztStaInd      *sztSta;
#endif /* ANSI */
{
   TRC2(UzLiSztStaInd);

   VeLiSztStaInd(post, suId, sztSta);
   
   RETVALUE(ROK);
} /* End of UzLiSztStaInd */

/*
*
*       Fun:   UzLiSztFlcInd
*
*       Desc:  Call handler for Status Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_sz_ptli.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztFlcInd
(
Pst            *post,
SuId           suId,
SztFlcInd      *flcInd
)
#else
PUBLIC S16 UzLiSztFlcInd(post, suId, flcInd)
Pst            *post;
SuId           suId;
SztFlcInd      *flcInd;
#endif /* ANSI */
{
   TRC2(UzLiSztFlcInd);
 
   RETVALUE(ROK);
} /* End of UzLiSztFlcInd */

/*
*
*       Fun:   UzLiSztErrInd
*
*       Desc:  Call handler for Error Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_sz_ptli.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztErrInd
(
Pst            *post,
SuId           suId,
SztLclErrInd      *sztLclErrInd
)
#else
PUBLIC S16 UzLiSztErrInd(post, suId, sztLclErrInd)
Pst            *post;
SuId           suId;
SztLclErrInd      *sztLclErrInd;
#endif /* ANSI */
{
   TRC2(UzLiSztErrInd);
 
   RETVALUE(ROK);
} /* End of UzLiSztErrInd */

/*
*
*       Fun:   UzLiSztUDatInd
*
*       Desc:  Call handler for Connection-less data indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_sz_ptli.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztUDatInd
(
Pst            *pst,
SuId           suId,
SztUDatEvnt    *uDatEvnt
)
#else
PUBLIC S16 UzLiSztUDatInd(pst, suId, uDatEvnt)
Pst            *pst;
SuId           suId;
SztUDatEvnt    *uDatEvnt;
#endif /* ANSI */
{
   TRC2(UzLiSztUDatInd);

   VeLiSztUDatInd(pst, suId, uDatEvnt);
   
   RETVALUE(ROK);
} /* End of UzLiSztUDatInd */

/*
*
*       Fun:   UzLiSztConInd
*
*       Desc:  Call handler for Connection Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_sz_ptli.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztConInd
(
Pst            *pst,
SuId           suId,
SztConInd      *conInd
)
#else
PUBLIC S16 UzLiSztConInd(pst, suId, conInd)
Pst            *pst;
SuId           suId;
SztConInd      *conInd;
#endif /* ANSI */
{
   TRC2(UzLiSztConInd);
   
   VeLiSztConInd(pst, suId, conInd);

   RETVALUE(ROK);
} /* End of UzLiSztConInd */

/*
*
*       Fun:   UzLiSztConCfm
*
*       Desc:  Call handler for Connection Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_sz_ptli.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztConCfm
(
Pst            *pst,
SuId           suId,
SztConCfm      *conCfm
)
#else
PUBLIC S16 UzLiSztConCfm(pst, suId, conCfm)
Pst            *pst;
SuId           suId;
SztConCfm      *conCfm;
#endif /* ANSI */
{
   TRC2(UzLiSztConCfm);

   VeLiSztConCfm(pst, suId, conCfm);

   RETVALUE(ROK);
} /* End of UzLiSztConCfm */

/*
*
*       Fun:   UzLiSztRelInd
*
*       Desc:  Call handler for Release Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_sz_ptli.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztRelInd
(
Pst            *pst,
SuId           suId,
SztRelInd      *relInd
)
#else
PUBLIC S16 UzLiSztRelInd(pst, suId, relInd)
Pst            *pst;
SuId           suId;
SztRelInd      *relInd;
#endif /* ANSI */
{
   TRC2(UzLiSztRelInd);
   
   VeLiSztRelInd(pst, suId, relInd);

   RETVALUE(ROK);
} /* End of UzLiSztRelInd */

/*
*
*       Fun:   UzLiSztRelCfm
*
*       Desc:  Call handler for Release Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_sz_ptli.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztRelCfm
(
Pst            *pst,
SuId           suId,
SztRelCfm      *relCfm
)
#else
PUBLIC S16 UzLiSztRelCfm(pst, suId, relCfm)
Pst            *pst;
SuId           suId;
SztRelCfm      *relCfm;
#endif /* ANSI */
{
   TRC2(UzLiSztRelCfm);
   
   VeLiSztRelCfm(pst, suId, relCfm);

   RETVALUE(ROK);
} /* End of UzLiSztRelCfm */

/*
*
*       Fun:   UzLiSztDatInd
*
*       Desc:  Call handler for Connection based data indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_sz_ptli.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztDatInd
(
Pst            *pst,
SuId           suId,
SztDatEvntInd  *datEvnt
)
#else
PUBLIC S16 UzLiSztDatInd(pst, suId, datEvnt)
Pst            *pst;
SuId           suId;
SztDatEvntInd  *datEvnt;
#endif /* ANSI */
{
   TRC2(UzLiSztDatInd);
   
   VeLiSztDatInd(pst, suId, datEvnt);

   RETVALUE(ROK);
} /* End of UzLiSztDatInd */

/*
*
*       Fun:   UzLiSztEncDecCfm
*
*       Desc:  Call handler for Encode/Decode Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_sz_ptli.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztEncDecCfm
(
Pst            *pst,
SuId           suId,
SztEncDecEvnt  *sztEncDecEvnt,
SztReqStatus   *status
)
#else
PUBLIC S16 UzLiSztEncDecCfm(pst, suId, sztEncDecEvnt,status)
Pst            *pst;
SuId           suId;
SztEncDecEvnt  *sztEncDecEvnt;
SztReqStatus   *status;
#endif /* ANSI */
{
   TRC2(UzLiSztEncDecCfm);
   
   RETVALUE(ROK);
} /* End of UzLiSztEncDecCfm */

#if (defined(SZ_MME) && defined(SZ_DYNAMIC_PEER))
/*
*
*       Fun:   UzLiSztPeerAssocInd
*
*       Desc:  Call handler for Peer Assoc Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_sz_ptli.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztPeerAssocInd
(
Pst              *pst,
SuId             suId,
SztPeerAssocInd  *peerAssocInd
)
#else
PUBLIC S16 UzLiSztPeerAssocInd(pst, suId, peerAssocInd)
Pst              *pst;
SuId             suId;
SztPeerAssocInd  *peerAssocInd;
#endif /* ANSI */
{
   TRC2(UzLiSztPeerAssocInd);
   
   RETVALUE(ROK);
} /* End of UzLiSztPeerAssocInd */
#endif


/********************************************************************30**

         End of file:     ve_sz_ptli.c@@/main/2 - Wed Dec  1 08:44:41 2010

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
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
