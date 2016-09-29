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

/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     CTF Portable
  
     File:     ve_ct_ptli.c 
  
     Sid:      ve_ct_ptli.c@@/main/2 - Wed Dec  1 08:44:22 2010
  
     Prg:       
  
**********************************************************************/
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

#include "nhu.h"           /* NHU interface defines */
#include "lnh.h"           /* Layer Manager interface defines */
#include "pju.h"           /* PDCP data interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "ctf.h"           /* PHY RRM control Interface */
#include "szt.h"           /* S1AP RRM control Interface */
#ifdef EU_DAT_APP
#include "egt.h"           /* S1AP RRM control Interface */
#endif 
#include "lrg.h"           /* MAC RRM control Interface */


/* RRM related includes */
#include "ve_eut.h"
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

#include "nhu.x"           /* NHU Interface includes */
#include "lnh.x"           /* LNH Interface includes */
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
#include "nhu_asn.x"
#include "czt_asn.x"

/* RRM related includes */
#include "lve.x" 
#include "ve_eut.x" 
#include "ve.x" 

#define MAXVELI 3

/* CTF Interface flags */
#ifndef LCVELICTF
#define PTVELICTF   1      /* Portable CTF Interface */
#else
#if(!(defined TF)  || !(defined DM))
#define PTVELICTF   1      /* Portable CTF Interface */
#endif /* TF && DM */
#endif /* PTVELICTF */

#ifdef PTVELICTF
/** Request from RRC to PHY to bind the CTF interface sap. */
EXTERN S16 PtLiCtfBndReq ARGS((Pst* pst, SuId suId, SpId spId));
/** Request from RRC to PHY to Unbind the CTF interface sap. */
EXTERN S16 PtLiCtfUbndReq ARGS((Pst* pst, SpId spId, Reason reason));
/** Configuration Request from RRC to PHY for 
 * cell or dedicated configuration. 
*/
EXTERN S16 PtLiCtfCfgReq ARGS((Pst* pst, SpId spId, CtfCfgTransId transId, CtfCfgReqInfo* cfgReqInfo));
/** UE ID change Request from RRC to PHY. */
EXTERN S16 PtLiCtfUeIdChgReq ARGS((Pst* pst, SpId spId, CtfCfgTransId transId, CtfUeInfo * ueInfo, CtfUeInfo * newUeInfo));
#endif /*--#ifdef PTVELICTF--*/



/* Request from RRC to PHY to bind the CTF interface sap. */
PRIVATE CONSTANT CtfBndReq VeLiCtfBndReqMt [MAXVELI] =
{
#ifdef LCVELICTF
   cmPkCtfBndReq,          /* 0 - loosely coupled */
#else  
   PtLiCtfBndReq,          /* 0 - loosely coupled, portable */
#endif /* LCVELICTF */
#ifdef TF 
   TfUiCtfBndReq,          /* 1 - tightly coupled, CTF */
#else
   PtLiCtfBndReq,          /* 1 - tightly coupled, portable */
#endif /* TF */
#ifdef DM 
   DmUiCtfBndReq,          /* 2 - tightly coupled, CTF */
#else
   PtLiCtfBndReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* Request from RRC to PHY to Unbind the CTF interface sap. */
PRIVATE CONSTANT CtfUbndReq VeLiCtfUbndReqMt [MAXVELI] =
{
#ifdef LCVELICTF
 cmPkCtfUbndReq,          /* 0 - loosely coupled */
#else  
 PtLiCtfUbndReq,          /* 0 - loosely coupled, portable */
#endif /* LCVELICTF */
#ifdef TF 
 TfUiCtfUbndReq,          /* 1 - tightly coupled, CTF */
#else
 PtLiCtfUbndReq,          /* 1 - tightly coupled, portable */
#endif /* TF */
#ifdef DM 
 DmUiCtfUbndReq,          /* 2 - tightly coupled, CTF */
#else
 PtLiCtfUbndReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* Configuration Request from RRC to PHY for 
 * cell or dedicated configuration. 
*/
PRIVATE CONSTANT CtfCfgReq VeLiCtfCfgReqMt[MAXVELI] =
{
#ifdef LCVELICTF
   cmPkCtfCfgReq,          /* 0 - loosely coupled */
#else  
   PtLiCtfCfgReq,          /* 0 - loosely coupled, portable */
#endif
#ifdef TF
   TfUiCtfCfgReq,          /* 1 - tightly coupled, CTF */
#else
   PtLiCtfCfgReq,          /* 1 - tightly coupled, portable */
#endif
#ifdef DM 
   DmUiCtfCfgReq,          /* 2 - tightly coupled, CTF */
#else
   PtLiCtfCfgReq,          /* 2 - tightly coupled, portable */
#endif
};

/** UE ID change Request from RRC to PHY. */
PRIVATE CONSTANT CtfUeIdChgReq VeLiCtfUeIdChgReqMt[MAXVELI] =
{
#ifdef LCVELICTF
   cmPkCtfUeIdChgReq,
#else
   PtLiCtfUeIdChgReq,
#endif
#ifdef TF
   TfUiCtfUeIdChgReq,
#else
   PtLiCtfUeIdChgReq,
#endif
#ifdef DM
   DmUiCtfUeIdChgReq,
#else
   PtLiCtfUeIdChgReq,
#endif
};

/**************************************************************************
*
*      Fun:   VeLiCtfBndReq
*
*      Desc:  Bind Req
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  ve_ct_ptli.c
*
*************************************************************************/
#ifdef ANSI
PUBLIC S16 VeLiCtfBndReq
(
Pst* pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 VeLiCtfBndReq(pst, suId, spId)
Pst* pst;
SuId suId;
SpId spId;
#endif
{
   TRC3(VeLiCtfBndReq);
   RETVALUE((*VeLiCtfBndReqMt[pst->selector])(pst, suId, spId));
} /* end of function VeLiCtfBndReq */

/**************************************************************************
*
*      Fun:   VeLiCtfUbndReq
*
*      Desc:  Unbind Request
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  ve_ct_ptli.c
*
*************************************************************************/
#ifdef ANSI
PUBLIC S16 VeLiCtfUbndReq
(
Pst* pst,
SpId spId,
Reason reason
)
#else
PUBLIC S16 VeLiCtfUbndReq(pst, spId, reason)
Pst* pst;
SpId spId;
Reason reason;
#endif
{
   TRC3(VeLiCtfUbndReq);

   RETVALUE((*VeLiCtfUbndReqMt[pst->selector])(pst, spId, reason));
} /* end of function VeLiCtfUbndReq */

/**************************************************************************
*
*      Fun:   VeLiCtfCfgReq
*
*      Desc:  CtfCfgReq
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  ve_ct_ptli.c
*
*************************************************************************/
#ifdef ANSI
PUBLIC S16 VeLiCtfCfgReq
(
Pst *pst,                       /* Post structure */
SpId spId,                      /* Service provider ID */
CtfCfgTransId transId,          /* cfg trans ID */
CtfCfgReqInfo *cfgReqInfo       /* MAC Configuration information */
)
#else
PUBLIC S16 VeLiCtfCfgReq(pst, spId, transId, cfgReqInfo)
Pst *pst;                       /* Post structure */
SpId spId;                      /* Service provider ID */
CtfCfgTransId transId;          /* cfg trans ID */
CtfCfgReqInfo *cfgReqInfo;      /* PHY Configuration information */
#endif
{
   TRC3(VeLiCtfCfgReq);

   RETVALUE((*VeLiCtfCfgReqMt[pst->selector])(pst, spId, transId, cfgReqInfo));
} /* end of function VeLiCtfCfgReq */

/**************************************************************************
*
*      Fun:   VeLiCtfUeIdChgReq
*
*      Desc:  Ue Id Change Req
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  ve_ct_ptli.c
*
*************************************************************************/
#ifdef ANSI
PUBLIC S16 VeLiCtfUeIdChgReq
(
Pst* pst,
SpId spId,
CtfCfgTransId transId,
CtfUeInfo * ueInfo,
CtfUeInfo * newUeInfo
)
#else
PUBLIC S16 VeLiCtfUeIdChgReq(pst, spId, transId, ueInfo, newUeInfo)
Pst* pst;
SpId spId;
CtfCfgTransId transId;
CtfUeInfo * ueInfo;
CtfUeInfo * newUeInfo;
#endif
{
   TRC3(VeLiCtfUeIdChgReq)

   RETVALUE((*VeLiCtfUeIdChgReqMt[pst->selector])(pst, spId, transId, ueInfo, newUeInfo));
} /* end of VeLiCtfUeIdChgReq */


#ifdef PTVELICTF
/**************************************************************************
 * The CTF (Portable) Interface
 *************************************************************************/
/**************************************************************************
 *
 *      Fun:   PtLiCtfBndReq
 *
 *      Desc:  Portable CtfBndReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  ve_ct_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtLiCtfBndReq
(
Pst *pst,                       /* Post structure */
SuId suId,                      /* Service user ID */
SpId spId                       /* Service provider ID */
)
#else
PUBLIC S16 PtLiCtfBndReq(pst, suId, spId)
Pst *pst;                       /* Post structure */
SuId suId;                      /* Service user ID */
SpId spId;                      /* Service provider ID */
#endif
{
   TRC3(PtLiCtfBndReq);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);

   VE_DBG_ERROR((VE_PRNT_BUF, "PtLiCtfBndReq() called"));

   RETVALUE(ROK);
} /* end of function PtLiCtfBndReq */

/**************************************************************************
*
*      Fun:   PtLiCtfUbndReq
*
*      Desc:  Portable CtfUbndReq primitive
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  ve_ct_ptli.c
*
*************************************************************************/
#ifdef ANSI
PUBLIC S16 PtLiCtfUbndReq
(
Pst *pst,                       /* Post structure */
SpId spId,                      /* Service provider ID */
Reason reason                   /* Reason */
)
#else
PUBLIC S16 PtLiCtfUbndReq(pst, spId, reason)
Pst *pst;                       /* Post structure */
SpId spId;                      /* Service provider ID */
Reason reason;                  /* Reason */
#endif
{
   TRC3(PtLiCtfUbndReq);

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(reason);

   VE_DBG_ERROR((VE_PRNT_BUF, "PtLiCtfUbndReq() called"));

   RETVALUE(ROK);
} /* end of function PtLiCtfUbndReq */

/**************************************************************************
*
*      Fun:   PtLiCtfCfgReq
*
*      Desc:  Portable CtfCfgReq
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  ve_ct_ptli.c
*
*************************************************************************/
#ifdef ANSI
PUBLIC S16 PtLiCtfCfgReq
(
Pst *pst,
SpId spId,
CtfCfgTransId transId,          /* cfg trans ID */
CtfCfgReqInfo *cfgReqInfo       /* PHY Configuration information */
)
#else
PUBLIC S16 PtLiCtfCfgReq(pst, spId, transId, cfgReqInfo)
Pst *pst;
SpId spId;
CtfCfgTransId transId;          /* cfg trans ID */
CtfCfgReqInfo *cfgReqInfo;      /* PHY Configuration information */
#endif
{
  TRC3(PtLiCtfCfgReq);

  UNUSED(pst);
  UNUSED(spId);
  UNUSED(transId);
  UNUSED(cfgReqInfo);

  VE_DBG_ERROR((VE_PRNT_BUF, "PtLiCtfCfgReq() called"));

  RETVALUE(ROK);
} /* end of function PtLiCtfCfgReq */

#ifdef ANSI
PUBLIC S16 PtLiCtfUeIdChgReq
(
Pst* pst,
SpId spId,
CtfCfgTransId transId,
CtfUeInfo * ueInfo,
CtfUeInfo * newUeInfo
)
#else
PUBLIC S16 PtLiCtfUeIdChgReq(pst, spId, transId, ueInfo, newUeInfo)
Pst* pst;
SpId spId;
CtfCfgTransId transId;
CtfUeInfo * ueInfo;
CtfUeInfo * newUeInfo;
#endif
{
   TRC3(PtLiCtfUeIdChgReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(transId);
   UNUSED(ueInfo);
   UNUSED(newUeInfo);

   VE_DBG_ERROR((VE_PRNT_BUF, "PtLiCtfUeIdChgReq() called"));

   RETVALUE(ROK);
} /* end of PtLiCtfUeIdChgReq */

#endif /* PTVELICTF */
/********************************************************************30**

         End of file:     ve_ct_ptli.c@@/main/2 - Wed Dec  1 08:44:22 2010

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
