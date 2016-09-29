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

     Name:     eNodeB App 

     Type:     C source file

     Desc:     C code for the APIs implemented by the layer manager, viz.
               LVE confirms and indications

     File:     ve_ptmi.c

     Sid:      ve_ptmi.c@@/main/2 - Wed Dec  1 08:44:28 2010

     Prg:      rbhat 

*********************************************************************21*/

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
#include "czt.x"           /* X2AP RRM control Interface */
#ifdef EU_DAT_APP
#include "egt.x"           /* S1AP RRM control Interface */
#endif
#include "lrg.x"           /* MAC RRM control Interface */
#include"ve_eut.x"

/* RRM related includes */
#include "lve.x" 
#include "ve.x" 

#if (!defined(LCVEMILVE) || !defined(SM))
#define PTVEMILVE
#endif

#define VE_LVE_MAX_SEL       2      /* maximum number of selectors */

/* forward references */
#ifdef PTVEMILVE
PRIVATE S16 PtMiLveCfgCfm    ARGS((Pst *pst, VeMngmt *cfg));
PRIVATE S16 PtMiLveCntrlCfm  ARGS((Pst *pst, VeMngmt *cntrl));
PRIVATE S16 PtMiLveStaInd  ARGS((Pst *pst, VeMngmt *usta));
#endif
/* Status Indication primitive */
PRIVATE LveStaInd VeMiLveStaIndMt[VE_LVE_MAX_SEL] =
{
#ifdef LCVEMILVE
   cmPkLveStaInd,          /* 0 - loosely coupled  */
#else
   PtMiLveStaInd,          /* 0 - tightly coupled, portable */
#endif /* LCVEMILVE */
#ifdef SM
   SmMiLveStaInd,          /* 1 - tightly coupled, layer management */
#else
   PtMiLveStaInd,          /* 1 - tightly coupled, portable */
#endif /* SM */
};

/* private variable definitions */
PRIVATE LveCfgCfm veMiLveCfgCfmMt[VE_LVE_MAX_SEL] =
{
#ifdef LCVEMILVE
   cmPkLveCfgCfm,
#else
   PtMiLveCfgCfm,
#endif 
#ifdef SM
   SmMiLveCfgCfm,
#else
   PtMiLveCfgCfm,
#endif /* SM */
};

PRIVATE LveCntrlCfm veMiLveCntrlCfmMt[VE_LVE_MAX_SEL] =
{
#ifdef LCVEMILVE
   cmPkLveCntrlCfm,
#else
   PtMiLveCntrlCfm,
#endif
#ifdef SM
   SmMiLveCntrlCfm,
#else
   PtMiLveCntrlCfm,
#endif
};

/*
*     layer management interface functions
*/
/*
*
*       Fun:    VeMiLveCfgCfm
*
*       Desc:   configuration confirm
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   ve_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 VeMiLveCfgCfm
(
Pst         *pst,
VeMngmt     *cfg
)
#else
PUBLIC S16 VeMiLveCfgCfm(pst, cfg)
Pst         *pst;
VeMngmt     *cfg;
#endif /* ANSI */
{
   TRC3(VeMiLveCfgCfm)
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst == (Pst *)NULLP || pst->selector >= VE_LVE_MAX_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
   RETVALUE((*veMiLveCfgCfmMt[pst->selector])(pst, cfg));
}

/*
*
*       Fun:    VeMiLveCntrlCfm
*
*       Desc:   control confirm
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   rr_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 VeMiLveCntrlCfm
(
Pst         *pst,             /* post structure */         
VeMngmt      *cntrl            /* management structure */   
)
#else
PUBLIC S16 VeMiLveCntrlCfm(pst, cntrl)
Pst         *pst;             /* post structure */         
VeMngmt      *cntrl;           /* management structure */   
#endif
{
   TRC3(VeMiLveCntrlCfm)
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst == (Pst *)NULLP || pst->selector >= VE_LVE_MAX_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
   RETVALUE((*veMiLveCntrlCfmMt[pst->selector])(pst, cntrl));
}


/**************************************************************************
 *
 *      Fun:  VeMiLveStaInd  
 *
 *      Desc: Used to indicate the status of the RRC layer to the layer manager. 
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  ve_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 VeMiLveStaInd
(
Pst *pst,                 /* Post structure */
VeMngmt *usta             /* Unsolicited status */
)
#else
PUBLIC S16 VeMiLveStaInd(pst, usta)
Pst *pst;                 /* Post structure */   
VeMngmt *usta;            /* Unsolicited status */
#endif
{
   TRC3(VeMiLveStaInd);

   /* jump to specific primitive depending on configured selector */
   (*VeMiLveStaIndMt[pst->selector])(pst, usta); 

   RETVALUE(ROK);
} /* end of function VeMiLveStaInd */


/* Portable functions */
#ifdef PTVEMILVE
/*
*
*       Fun:    PtMiLveCfgCfm
*
*       Desc:   Dummy configuration confirm, customize if necessary
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   ve_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 PtMiLveCfgCfm
(
Pst                     *pst,
VeMngmt                 *cfg
)
#else
PUBLIC S16 PtMiLveCfgCfm(pst, cfg)
Pst                     *pst;
VeMngmt                 *cfg;
#endif
{
   TRC3(PtMiLveCfgCfm)
   UNUSED(pst);
   UNUSED(cfg);
   RETVALUE(RFAILED);
}

/*
*
*       Fun:    PtMiLveCntrlCfm
*
*       Desc:   Dummy control confirm, customize if necessary
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   ve_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 PtMiLveCntrlCfm
(
Pst                     *pst,
VeMngmt                 *cntrl
)
#else
PUBLIC S16 PtMiLveCntrlCfm(pst, cntrl)
Pst                     *pst;
VeMngmt                 *cntrl;
#endif
{
   TRC3(PtMiLveCntrlCfm)
   UNUSED(pst);
   UNUSED(cntrl);
   RETVALUE(RFAILED);
}

/**************************************************************************
 *
 *      Fun:  PtMiLveStaInd (Portable) 
 *
 *      Desc: Used to indicate the status of the RRC layer to the layer manager. 
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  ve_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 PtMiLveStaInd
(
Pst *pst,                 /* Post structure */
VeMngmt *usta             /* Unsolicited status */
)
#else
PUBLIC S16 PtMiLveStaInd(pst, usta)
Pst *pst;                 /* Post structure */   
VeMngmt *usta;            /* Unsolicited status */
#endif
{
   TRC3(PtMiLveStaInd);

   UNUSED(pst);
   UNUSED(usta);


   RETVALUE(ROK);
} /* end of PtMiLveStaInd */

#endif

/********************************************************************30**
  
         End of file:     ve_ptmi.c@@/main/2 - Wed Dec  1 08:44:28 2010
  
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
