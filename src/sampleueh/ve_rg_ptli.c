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
  
     Desc:     C source code for Entry point fucntions
  
     File:     
  
     Sid:      ve_rg_ptli.c@@/main/2 - Wed Dec  1 08:44:29 2010
  
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

/* MAX Number of Service Providers of VE */
#define VE_MAX_RGR_PROV   3

#if (!defined(LCVELIRGR) || !defined(LWLCVELIRGR) || !defined(RG))
#define PTVELIRGR
#endif

#ifdef PTVELIRGR
/** @brief Request from RRM to MAC to bind the interface saps */
PRIVATE S16 PtLiRgrBndReq ARGS((Pst* pst, SuId suId, SpId spId));
/** @brief Request from RRM to MAC to Unbind the interface saps */
PRIVATE S16 PtLiRgrUbndReq ARGS((Pst* pst, SpId spId, Reason reason));
/** @brief Configuration Request from RRM to MAC for 
 * configuring Cell/Ue/Lc */
PRIVATE S16 PtLiRgrCfgReq ARGS((Pst* pst, SpId spId, RgrCfgTransId transId, RgrCfgReqInfo * cfgReqInfo));
#ifdef SI_NEW
PRIVATE S16 PtLiRgrSiCfgReq ARGS((Pst* pst, SpId spId, RgrCfgTransId transId, RgrSiCfgReqInfo * cfgReqInfo));
#endif
#endif /*--#ifdef PTVELIRGR--*/

/** @brief Request from RRM to MAC to bind the interface saps */
PRIVATE  RgrBndReq VeLiRgrBndReqMt[VE_MAX_RGR_PROV] =
{
#ifdef LCVELIRGR
   cmPkRgrBndReq,
#else
   PtLiRgrBndReq,
#endif
#ifdef RG
   RgUiRgrBndReq,
#else
   PtLiRgrBndReq,
#endif
#ifdef LWLCVELIRGR
   cmPkRgrBndReq
#else
   PtLiRgrBndReq
#endif
};

/** @brief Request from RRM to MAC to Unbind the interface saps */
PRIVATE  RgrUbndReq VeLiRgrUbndReqMt[VE_MAX_RGR_PROV] =
{
#ifdef LCVELIRGR
   cmPkRgrUbndReq,
#else
   PtLiRgrUbndReq,
#endif
#ifdef RG
   RgUiRgrUbndReq,
#else
   PtLiRgrUbndReq,
#endif
#ifdef LWLCVELIRGR
   cmPkRgrUbndReq
#else
   PtLiRgrUbndReq
#endif
};

/** @brief Configuration Request from RRM to MAC for 
 * configuring Cell/Ue/Lc */
PRIVATE  RgrCfgReq VeLiRgrCfgReqMt[VE_MAX_RGR_PROV] =
{
#ifdef LCVELIRGR
   cmPkRgrCfgReq,
#else
   PtLiRgrCfgReq,
#endif
#ifdef RG
   RgUiRgrCfgReq,
#else
   PtLiRgrCfgReq,
#endif
#ifdef LWLCVELIRGR
   cmPkRgrCfgReq
#else
   PtLiRgrCfgReq
#endif
};

#ifdef SI_NEW
/** @brief Configuration Request from RRM to MAC for 
 * configuring SI */
PRIVATE  RgrSiCfgReq VeLiRgrSiCfgReqMt[VE_MAX_RGR_PROV] =
{
#ifdef LCVELIRGR
   cmPkRgrSiCfgReq,
#else
   PtLiRgrSiCfgReq,
#endif
#ifdef RG
   RgUiRgrSiCfgReq,
#else
   PtLiRgrSiCfgReq,
#endif
#ifdef LWLCVELIRGR
   cmPkRgrSiCfgReq
#else
   PtLiRgrSiCfgReq
#endif
};
#endif

#ifdef VE
/*
*
*       Fun:   VeLiRgrBndReq 
*
*       Desc:  Bnd Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_rg_ptli.c  
*
*/
#ifdef ANSI
PUBLIC S16 VeLiRgrBndReq
(
Pst* pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 VeLiRgrBndReq(pst, suId, spId)
Pst* pst;
SuId suId;
SpId spId;
#endif
{

   TRC3(VeLiRgrBndReq);

   (*VeLiRgrBndReqMt[pst->selector])(pst, suId, spId);

   RETVALUE(ROK);

}

/*
*
*       Fun:   VeLiRgrUbndReq 
*
*       Desc:  Unbind Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_rg_ptli.c  
*
*/
#ifdef ANSI
PUBLIC S16 VeLiRgrUbndReq
(
Pst* pst,
SpId spId,
Reason reason
)
#else
PUBLIC S16 VeLiRgrUbndReq(pst, spId, reason)
Pst* pst;
SpId spId;
Reason reason;
#endif
{

   TRC3(VeLiRgrUbndReq);

   (*VeLiRgrUbndReqMt[pst->selector])(pst, spId, reason);

   RETVALUE(ROK);

}

/*
*
*       Fun:   VeLiRgrCfgReq 
*
*       Desc:  Cfg Req.
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_rg_ptli.c  
*
*/
#ifdef ANSI
PUBLIC S16 VeLiRgrCfgReq
(
Pst* pst,
SpId spId,
RgrCfgTransId transId,
RgrCfgReqInfo * cfgReqInfo
)
#else
PUBLIC S16 VeLiRgrCfgReq(pst, spId, transId, cfgReqInfo)
Pst* pst;
SpId spId;
RgrCfgTransId transId;
RgrCfgReqInfo * cfgReqInfo;
#endif
{

   TRC3(VeLiRgrCfgReq);

   if(NULLP == pst)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiRgrCfgReq : pst is NULL.\n"));
      RETVALUE(RFAILED);
   }

   (*VeLiRgrCfgReqMt[pst->selector])(pst, spId, transId, cfgReqInfo);

   RETVALUE(ROK);

}

#ifdef SI_NEW
/*
*
*       Fun:   VeLiRgrSiCfgReq
*
*       Desc:  SI Cfg Req.
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_rg_ptli.c  
*
*/
#ifdef ANSI
PUBLIC S16 VeLiRgrSiCfgReq
(
Pst* pst,
SpId spId,
RgrCfgTransId transId,
RgrSiCfgReqInfo * cfgReqInfo
)
#else
PUBLIC S16 VeLiRgrSiCfgReq(pst, spId, transId, cfgReqInfo)
Pst* pst;
SpId spId;
RgrCfgTransId transId;
RgrSiCfgReqInfo * cfgReqInfo;
#endif
{

   TRC3(VeLiRgrSiCfgReq);

   (*VeLiRgrSiCfgReqMt[pst->selector])(pst, spId, transId, cfgReqInfo);

   RETVALUE(ROK);

}
#endif

#endif /*--ifdef VE--*/

#ifdef PTVELIRGR

/*
*
*       Fun:   PtLiRgrBndReq 
*
*       Desc:  Portable Bind Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_rg_ptli.c  
*
*/
#ifdef ANSI
PUBLIC S16 PtLiRgrBndReq
(
Pst* pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 PtLiRgrBndReq(pst, suId, spId)
Pst* pst;
SuId suId;
SpId spId;
#endif
{

   TRC3(PtLiRgrBndReq);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);

   RETVALUE(ROK);

}
/*
*
*       Fun:   PtLiRgrUbndReq 
*
*       Desc:  Portable Unbind Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_rg_ptli.c  
*
*/
#ifdef ANSI
PUBLIC S16 PtLiRgrUbndReq
(
Pst* pst,
SpId spId,
Reason reason
)
#else
PUBLIC S16 PtLiRgrUbndReq(pst, spId, reason)
Pst* pst;
SpId spId;
Reason reason;
#endif
{

   TRC3(PtLiRgrUbndReq);

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(reason);

   RETVALUE(ROK);

}

/*
*
*       Fun:   PtLiRgrCfgReq 
*
*       Desc:  Portable Cfg Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_rg_ptli.c  
*
*/
#ifdef ANSI
PUBLIC S16 PtLiRgrCfgReq
(
Pst* pst,
SpId spId,
RgrCfgTransId transId,
RgrCfgReqInfo * cfgReqInfo
)
#else
PUBLIC S16 PtLiRgrCfgReq(pst, spId, transId, cfgReqInfo)
Pst* pst;
SpId spId;
RgrCfgTransId transId;
RgrCfgReqInfo * cfgReqInfo;
#endif
{

   TRC3(PtLiRgrCfgReq);

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(transId);
   UNUSED(cfgReqInfo);

   RETVALUE(ROK);

}

#ifdef SI_NEW
/*
*
*       Fun:   PtLiRgrSiCfgReq 
*
*       Desc:  Portable Si Cfg Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_rg_ptli.c  
*
*/
#ifdef ANSI
PRIVATE S16 PtLiRgrSiCfgReq
(
Pst* pst,
SpId spId,
RgrCfgTransId transId,
RgrSiCfgReqInfo * cfgReqInfo
)
#else
PRIVATE S16 PtLiRgrSiCfgReq(pst, spId, transId, cfgReqInfo)
Pst* pst;
SpId spId;
RgrCfgTransId transId;
RgrSiCfgReqInfo * cfgReqInfo;
#endif
{

   TRC3(PtLiRgrSiCfgReq);

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(transId);
   UNUSED(cfgReqInfo);

   RETVALUE(ROK);

}
#endif
#endif /*--ifdef PTVELIRGR--*/
/********************************************************************30**
  
         End of file:     ve_rg_ptli.c@@/main/2 - Wed Dec  1 08:44:29 2010
  
*********************************************************************31*/

/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
