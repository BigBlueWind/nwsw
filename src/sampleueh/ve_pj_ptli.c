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
  
        Name:    eNodeB Sample App.
    
        Type:    C file
  
        Desc:    This file Contains the RLC service user lower interface
                  primitive implementain
 
        File:    ve_pj_ptli.c

        Sid:      ve_pj_ptli.c@@/main/2 - Wed Dec  1 08:44:28 2010
  
        Prg:     
  
*********************************************************************21*/

/* header (.h) include files */
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
#ifdef LTE_HO_SUPPORT
#include "czt.x"           /* X2AP RRM control Interface */
#endif
#ifdef EU_DAT_APP
#include "egt.x"           /* S1AP RRM control Interface */
#endif
#include "lrg.x"           /* MAC RRM control Interface */
#include"ve_eut.x"

/* RRM related includes */
#include "lve.x" 
#include "ve.x" 

#ifdef KW_PDCP

#if (!defined(LCPJUIPJU) || !defined(PJ))
#define PTNXLIPJU
#endif 

#ifdef PTNXLIPJU
PRIVATE S16 PtLiPjuBndReq ARGS (( Pst* pst, SpId spId, SuId suId));
PRIVATE S16 PtLiPjuUbndReq ARGS (( Pst* pst, SpId spId, Reason reason));
PRIVATE S16 PtLiPjuDatReq ARGS (( Pst* pst, SpId spId, CmLtePdcpId * pdcpId, PjuSduId sduId, Buffer * mBuf));
PRIVATE S16 PtLiPjuDatFwdReq ARGS (( Pst* pst, SpId spId, CmLtePdcpId * pdcpId, PjuDatFwdReqInfo * datFwdReq));
#endif 



/************************************************************************
                             PJU Interface Matrices
************************************************************************/
PRIVATE CONSTANT PjuBndReq VeLiPjuBndReqMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuBndReq,
#else
   PtLiPjuBndReq,
#endif
#ifdef PJ
   PjUiPjuBndReq,
#else
   PtLiPjuBndReq,
#endif
};


PRIVATE CONSTANT PjuUbndReq VeLiPjuUbndReqMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuUbndReq,
#else
   PtLiPjuUbndReq,
#endif
#ifdef PJ
   PjUiPjuUbndReq,
#else
   PtLiPjuUbndReq,
#endif
};


PRIVATE CONSTANT PjuDatReq VeLiPjuDatReqMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuDatReq,
#else
   PtLiPjuDatReq,
#endif
#ifdef PJ
   PjUiPjuDatReq,
#else
   PtLiPjuDatReq,
#endif
};


PRIVATE CONSTANT PjuDatFwdReq VeLiPjuDatFwdReqMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuDatFwdReq,
#else
   PtLiPjuDatFwdReq,
#endif
#ifdef PJ
   PjUiPjuDatFwdReq,
#else
   PtLiPjuDatFwdReq,
#endif
};


/****************************************************************************
 *                         PJU Interface Mt functions
 ***************************************************************************/
#ifdef VE
/*
*
*       Fun:   VeLiPjuBndReq 
*
*       Desc:  Bind Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_pj_ptli.c 
*
*/
#ifdef ANSI
PUBLIC S16 VeLiPjuBndReq
(
Pst* pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 VeLiPjuBndReq(pst, suId, spId)
Pst* pst;
SuId suId;
SpId spId;
#endif
{

   TRC3(VeLiPjuBndReq)

   (*VeLiPjuBndReqMt[pst->selector])(pst, suId, spId);

   RETVALUE(ROK);

}

/*
*
*       Fun:   VeLiPjuUbndReq 
*
*       Desc:  Unbind Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_pj_ptli.c 
*
*/
#ifdef ANSI
PUBLIC S16 VeLiPjuUbndReq
(
Pst* pst,
SpId spId,
Reason reason
)
#else
PUBLIC S16 VeLiPjuUbndReq(pst, spId, reason)
Pst* pst;
SpId spId;
Reason reason;
#endif
{

   TRC3(VeLiPjuUbndReq)

   (*VeLiPjuUbndReqMt[pst->selector])(pst, spId, reason);

   RETVALUE(ROK);

}

/*
*
*       Fun:   VeLiPjuDatReq 
*
*       Desc:  Data Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_pj_ptli.c 
*
*/
#ifdef ANSI
PUBLIC S16 VeLiPjuDatReq
(
Pst* pst,
SpId spId,
CmLtePdcpId *pdcpId,
PjuSduId sduId,
Buffer *mBuf
)
#else
PUBLIC S16 VeLiPjuDatReq(pst, spId, pdcpId, sduId, mBuf)
Pst* pst;
SpId spId;
CmLtePdcpId *pdcpId;
PjuSduId sduId;
Buffer *mBuf;
#endif
{

   TRC3(VeLiPjuDatReq)

   (*VeLiPjuDatReqMt[pst->selector])(pst, spId, pdcpId, sduId, mBuf);

   RETVALUE(ROK);

}

/*
*
*       Fun:   VeLiPjuDatFwdReq 
*
*       Desc:  Data Forward Request 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_pj_ptli.c 
*
*/
#ifdef ANSI
PUBLIC S16 VeLiPjuDatFwdReq
(
Pst* pst,
SpId spId,
CmLtePdcpId * pdcpId,
PjuDatFwdReqInfo * datFwdReq
)
#else
PUBLIC S16 VeLiPjuDatFwdReq(pst, spId, pdcpId, datFwdReq)
Pst* pst;
SpId spId;
CmLtePdcpId * pdcpId;
PjuDatFwdReqInfo * datFwdReq;
#endif
{

   TRC3(VeLiPjuDatFwdReq)

   (*VeLiPjuDatFwdReqMt[pst->selector])(pst, spId, pdcpId, datFwdReq);

   RETVALUE(ROK);

}
#endif /*--ifdef NX--*/

  
/****************************************************************************
 *                         Porting Functions
 ***************************************************************************/
#ifdef PTNXLIPJU
/*
*
*       Fun:   PtLiPjuBndReq 
*
*       Desc:  Portable Bind Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_pj_ptli.c 
*
*/
#ifdef ANSI
PUBLIC S16 PtLiPjuBndReq
(
Pst* pst,
SpId spId,
SuId suId
)
#else
PUBLIC S16 PtLiPjuBndReq(pst, spId, suId)
Pst* pst;
SpId spId;
SuId suId;
#endif
{

   TRC3(PtLiPjuBndReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(suId);

   RETVALUE(ROK);
}

/*
*
*       Fun:   PtLiPjuUbndReq 
*
*       Desc:  Portable Unbind Request 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_pj_ptli.c 
*
*/
#ifdef ANSI
PUBLIC S16 PtLiPjuUbndReq
(
Pst* pst,
SpId spId,
Reason reason
)
#else
PUBLIC S16 PtLiPjuUbndReq(pst, spId, reason)
Pst* pst;
SpId spId;
Reason reason;
#endif
{

   TRC3(PtLiPjuUbndReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(reason);

   RETVALUE(ROK);
}

/*
*
*       Fun:   PtLiPjuDatReq 
*
*       Desc:  Portable Data Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_pj_ptli.c 
*
*/
#ifdef ANSI
PUBLIC S16 PtLiPjuDatReq
(
Pst* pst,
SpId spId,
CmLtePdcpId * pdcpId,
PjuSduId sduId,
Buffer * mBuf
)
#else
PUBLIC S16 PtLiPjuDatReq(pst, spId, pdcpId, sduId, mBuf)
Pst* pst;
SpId spId;
CmLtePdcpId * pdcpId;
PjuSduId sduId;
Buffer * mBuf;
#endif
{

   TRC3(PtLiPjuDatReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(pdcpId);
   UNUSED(sduId);
   UNUSED(mBuf);

   RETVALUE(ROK);
}

/*
*
*       Fun:   PtLiPjuDatFwdReq 
*
*       Desc:  Portable Data Fwd Req.
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_pj_ptli.c 
*
*/
#ifdef ANSI
PUBLIC S16 PtLiPjuDatFwdReq
(
Pst* pst,
SpId spId,
CmLtePdcpId * pdcpId,
PjuDatFwdReqInfo * datFwdReq
)
#else
PUBLIC S16 PtLiPjuDatFwdReq(pst, spId, pdcpId, datFwdReq)
Pst* pst;
SpId spId;
CmLtePdcpId * pdcpId;
PjuDatFwdReqInfo * datFwdReq;
#endif
{

   TRC3(PtLiPjuDatFwdReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(pdcpId);
   UNUSED(datFwdReq);

   RETVALUE(ROK);
}
#endif /*--ifdef PTNXLIPJU--*/

#endif /* KW_PDCP */
  
/********************************************************************30**
         End of file:     ve_pj_ptli.c@@/main/2 - Wed Dec  1 08:44:28 2010
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
