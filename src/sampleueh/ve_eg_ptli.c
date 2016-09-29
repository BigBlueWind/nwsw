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

     Name:    Sample App.

     Type:    C Include file

     Desc:    This file contains the acc source code

     File:    ve_eg_ptli.c

     Sid:      ve_eg_ptli.c@@/main/2 - Wed Dec  1 08:44:24 2010

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
#include "cm_tpt.h"        /* common transport defines */
#include "cm_tpt.h"        /* common LTE defines */
#include "pju.h"           /* PJU defines */
#include "egt.h"            /* defines and macros for EG */
#include "lve.h"         /* common DNS libraru defines */
#include "szt.h"
#include "szt_asn.h"
#include "nhu.h"
#include "egt.h"
#include "rgr.h"
#include "ctf.h"
#include "ve_eut.h"          
#include "ve.h"         /* common DNS libraru defines */

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
#include "cm_lte.x"        /* common LTE defines */
#include "pju.x"           /* PJU */
#include "egt.x"           /* PJU defines */
#include "ve_eut.x"           /* PJU defines */
#include "szt.x"
#include "czt.x"
#include "szt_asn.x"
#include "lve.x"
#include "nhu.x"
#include "egt.x"
#include "rgr.x"
#include "ctf.x"
#include "ve.x"

#if (!defined(LWLCEULIEGT) || !defined(LCEULIEGT))
#define PTEULIEGT
#else
#ifndef EG
#define PTEULIEGT
#else
#endif
#endif

/* ---- eGTP service user lower interface ---- */
/************************************************************************
                              Prototypes  
************************************************************************/

#ifdef PTEULIEGT

PRIVATE S16 PtLiEgtBndReq       ARGS((Pst *pst, SuId suId, SpId spId));
PRIVATE S16 PtLiEgtUbndReq      ARGS((Pst *pst, SpId spId, Reason reason));
#ifdef EGTP_C
PRIVATE S16 PtLiEgtSigReq       ARGS((Pst *pst, SpId spId,                
               CmTptAddr   *localAddr, /* Local node address */
               CmTptAddr   *remAddr,   /* Remote node address */
               TknU32      lclTeid,    /* Local Tunnel Identifier */
               TknU32      transId,    /* Transaction Identifier */
               TknU8       crtFlag,    /* Create tunnel or not */
               TmrCfg      T3,         /* Request timer expiry */
               EgMsg    *egMsg));
PRIVATE S16 PtLiEgtSigRsp       ARGS((Pst *pst, SpId spId, 
               TknU32      lclTeid,   /* Local TEID */
               EgMsg    *egMsg));

PRIVATE S16 PtLiEgtStaReq       ARGS((Pst *pst, SpId spId, 
               U8      eventType,   /* Local TEID */
               EgPathInfo   *pathInfo));
#endif /* end of EGTP_C */

PUBLIC S16 PtLiEgtLclDelReq       ARGS((Pst *pst, SpId spId, 
               U32      transId,   /* Local TEID */
               U32   teid));


#endif


/* Following matrices define the mapping between the primitives called by the
 * eGTP user (as VeLiEgtXXXReq) and the corresponding primitives of the 
 * eGTP layer (service provider) (as EgUiEgtXXXReq).
 * The mapping is based on pst->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCEULIEGT    loose         cmPkEgtXXXReq
 * 1                 EG          tight         EgUiEgtXXXReq
 * 2                LWLCEULIEGT  LW loose      cmPkEgtXXXReq
 */


PUBLIC EgtBndReq VeLiEgtBndReqMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtBndReq,        /* 0 - loosely coupled */
#else
   PtLiEgtBndReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtBndReq,        /* 1 - light weight loosely coupled */
#else
   PtLiEgtBndReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtBndReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtBndReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC EgtUbndReq VeLiEgtUbndReqMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtUbndReq,        /* 0 - loosely coupled */
#else
   PtLiEgtUbndReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtUbndReq,        /* 1 - light weight loosely coupled */
#else
   PtLiEgtUbndReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtUbndReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtUbndReq,        /* 2 - tightly coupled, portable */
#endif
};


/* Delete Local Tunnel primitive */
PUBLIC EgtEguLclTnlMgmtReq VeLiEgtEguLclTnlMgmtReqMt[] =
{
#ifdef LCEGUIEGT
   cmPkEgtEguLclTnlMgmtReq,          /* 0 - loosely coupled */
#else
   PtLiEgtEguLclTnlMgmtReq,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtEguLclTnlMgmtReq,          /* 1 - loosely coupled */
#else
   PtLiEgtEguLclTnlMgmtReq,          /* 1 - loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtEguLclTnlMgmtReq,          /* 1 - tightly coupled, service user EU */
#else
   PtLiEgtEguLclTnlMgmtReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Delete Local Tunnel primitive */
PRIVATE CONSTANT EgtEguStaReq VeLiEgtEguStaReqMt[] =
{
#ifdef LCEGUIEGT
   cmPkEgtEguStaReq,          /* 0 - loosely coupled */
#else
   PtLiEgtEguStaReq,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtEguStaReq,          /* 1 - loosely coupled */
#else
   PtLiEgtEguStaReq,          /* 1 - loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtEguStaReq,          /* 1 - tightly coupled, service user EU */
#else
   PtLiEgtEguStaReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Delete Local Tunnel primitive */
PRIVATE CONSTANT EgtEguDatReq VeLiEgtEguDatReqMt[] =
{
#ifdef LCEGUIEGT
   cmPkEgtEguDatReq,          /* 0 - loosely coupled */
#else
   PtLiEgtEguDatReq,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtEguDatReq,          /* 1 - loosely coupled */
#else
   PtLiEgtEguDatReq,          /* 1 - loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtEguDatReq,          /* 1 - tightly coupled, service user EU */
#else
   PtLiEgtEguDatReq,          /* 1 - tightly coupled, portable */
#endif
};



/************************************************************************
                       EGT Interface Functions
************************************************************************/

/*
 *
 *       Fun:   VeLiEgtBndReq
 *
 *       Desc:  This function resolves the EgtBndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_eg_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiEgtBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else 
PUBLIC S16 VeLiEgtBndReq (pst, suId, spId)
Pst *pst;
SuId suId; 
SpId spId;
#endif
{
   TRC3(VeLiEgtBndReq)

   (*VeLiEgtBndReqMt[pst->selector]) (pst, suId, spId);

   RETVALUE(ROK);
} /* VeLiEgtBndReq */

/*
 *
 *       Fun:   VeLiEgtUbndReq
 *
 *       Desc:  This function resolves the EgtUbndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_eg_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeLiEgtUbndReq
(
Pst *pst,
SpId spId,
Reason reason
)
#else 
PUBLIC S16 VeLiEgtUbndReq (pst, spId, reason)
Pst *pst;
SpId spId;
Reason reason;
#endif
{
   TRC3(VeLiEgtUbndReq)

   (*VeLiEgtUbndReqMt[pst->selector]) (pst, spId, reason);

   RETVALUE(ROK);
} /* VeLiEgtUbndReq */



/*
*
*       Fun: VeLiEgtEguLclTnlMgmtReq 
*
*       Desc:  This function resolves the EgtStaCfm primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  ve_eg_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiEgtEguLclTnlMgmtReq 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 VeLiEgtEguLclTnlMgmtReq(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   TRC3(VeLiEgtEguLclTnlMgmtReq);

   (*VeLiEgtEguLclTnlMgmtReqMt[pst->selector])(pst, suId, egtUEvnt);

   RETVALUE(ROK);
} /* end of VeLiEgtEguLclTnlMgmtReq() */

/*
*
*       Fun: VeLiEgtEguStaReq 
*
*       Desc:  
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  ve_eg_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiEgtEguStaReq 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 VeLiEgtEguStaReq(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   TRC3(VeLiEgtEguStaReq);

   (*VeLiEgtEguStaReqMt[pst->selector])(pst, suId, egtUEvnt);

   RETVALUE(ROK);
} /* end of VeLiEgtEguStaReq() */

/*
*
*       Fun:  VeLiEgtEguDatReq
*
*       Desc: Egt Data Request
*
*       Ret:   ROK -      ok
*
*       Notes: none
*
*       File:  ve_eg_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiEgtEguDatReq 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 VeLiEgtEguDatReq(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   TRC3(VeLiEgtEguDatReq);

   (*VeLiEgtEguDatReqMt[pst->selector])(pst, suId, egtUEvnt);

   RETVALUE(ROK);
} /* end of VeLiEgtEguDatReq() */


/************************************************************************
                       Portable Functions
************************************************************************/

#ifdef PTEULIEGT
/*
 *
 *       Fun:   PtLiEgtBndReq
 *
 *       Desc:  Portable version of EgtBndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_eg_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiEgtBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else 
PRIVATE S16 PtLiEgtBndReq (pst, suId, spId)
Pst *pst;
SuId suId; 
SpId spId;
#endif
{
   TRC3(PtLiEgtBndReq)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);

   RETVALUE(RFAILED);
} /* PtLiEgtBndReq */

/*
 *
 *       Fun:   PtLiEgtUbndReq
 *
 *       Desc:  Portable version of EgtUbndReq primitive
 *
 *       Ret:   ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_eg_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiEgtUbndReq
(
Pst *pst,
SpId spId,
Reason reason
)
#else 
PRIVATE S16 PtLiEgtUbndReq (pst, spId, reason)
Pst *pst;
SpId spId;
Reason reason;
#endif
{
   TRC3(PtLiEgtUbndReq)

   UNUSED(pst);
   UNUSED(spId);

   RETVALUE(RFAILED);
} /* PtLiEgtUbndReq */


/*
*
*       Fun:   PtLiEgtEguLclTnlMgmtReq 
*
*       Desc:  Local Tunnel Mgmt. Req.
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  ve_eg_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 PtLiEgtEguLclTnlMgmtReq 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 PtLiEgtEguLclTnlMgmtReq(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   TRC3(PtLiEgtEguLclTnlMgmtReq);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(egtUEvnt);

   RETVALUE(ROK);
} /* end of PtLiEgtEguLclTnlMgmtReq() */

/*
*
*       Fun: PtLiEgtEguStaReq 
*
*       Desc: Portable Status Req 
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  ve_eg_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 PtLiEgtEguStaReq 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 PtLiEgtEguStaReq(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   TRC3(PtLiEgtEguStaReq);


   UNUSED(pst);
   UNUSED(suId);
   UNUSED(egtUEvnt);

   RETVALUE(ROK);
} /* end of EgUiEgtEguTnlMgmtCfm() */

/*
*
*       Fun:  PtLiEgtEguDatReq
*
*       Desc: Portable Data Req 
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  ve_eg_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 PtLiEgtEguDatReq 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 PtLiEgtEguDatReq(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   TRC3(PtLiEgtEguDatReq);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(egtUEvnt);

   RETVALUE(ROK);
} /* end of PtLiEgtEguDatReq() */


/*
*
*       Fun:  PtLiEgtLclDelReq
*
*       Desc: Portable Del Req 
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  ve_eg_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 PtLiEgtLclDelReq 
(
Pst *pst,
SpId spId, 
U32      transId,   /* Local TEID */
U32   teid
)
#else
PUBLIC S16 PtLiEgtLclDelReq(pst, spId, transId, teid)
Pst *pst;
SpId spId;
U32  transId;
U32  teid;
#endif
{
   TRC3(PtLiEgtLclDelReq);

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(transId);
   UNUSED(teid);

   RETVALUE(ROK);
} /* end of PtLiEgtLclDelReq() */

#endif /* PTEULIEGT */

/********************************************************************30**

         End of file:     ve_eg_ptli.c@@/main/2 - Wed Dec  1 08:44:24 2010

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
