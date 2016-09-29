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
 
     Name:     LTE MAC Convergence Layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     ys_id.c
  
     Sid:      ys_id.c@@/main/2 - Thu Dec  2 02:27:14 2010
  
     Prg:      rk
  
**********************************************************************/

/** @file ys_id.c
@brief This module acts as an interface handler for upper interface and 
manages Pst and Sap related information for upper interface APIs.
*/

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "lys.h"           /* layer management defines for LTE-CL */

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm_lib.x"        /* common library */


/* defines */
#define YSSWMV 1             /* CL- main version */
#define YSSWMR 2             /* CL - main revision */
#define YSSWBV 0             /* CL - branch version */
#define YSSWBR 4             /* CL - branch revision ys004.102 */
#define YSSWPN "1000414"     /* CL - part number */

PUBLIC Void ysGetSId ARGS((SystemId *s));

/* system id */
PRIVATE CONSTANT SystemId sId ={
   YSSWMV,              /* CL - main version */
   YSSWMR,              /* CL - main revision */
   YSSWBV,              /* CL - branch version */
   YSSWBR,              /* CL - branch revision */
   YSSWPN               /* CL - part number */
};


/**
 * @brief Retrieve system id. 
 *
 * @details
 *
 *     Function : ysGetSId 
 *    
 *     Get system id consisting of part number, main version and
 *     revision and branch version and branch.
 *     
 *     
 *  @param[out] SystemId *s  
 *  @return  void
 **/
/*
*
*       Fun:   get system id
*
*       Desc:  Get system id consisting of part number, main version and
*              revision and branch version and branch.
*
*       Ret:   TRUE      - ok
*
*       Notes: None
*
*       File:  ys_id.c
*
*/

#ifdef ANSI
PUBLIC Void ysGetSId
(
SystemId *s                 /* system id */
)
#else
PUBLIC Void ysGetSId(s)
SystemId *s;                /* system id */
#endif
{
   TRC2(ysGetSId);

   s->mVer = sId.mVer;
   s->mRev = sId.mRev;
   s->bVer = sId.bVer;
   s->bRev = sId.bRev;
   cmMemcpy((U8 *)s->ptNmb, (U8 *)sId.ptNmb, LYS_PT_NUM_SIZE); 
   RETVOID;
} /* end of ysGetSid */

/********************************************************************30**
  
         End of file:     ys_id.c@@/main/2 - Thu Dec  2 02:27:14 2010
  
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
----------- -------- ---- -----------------------------------------------
/main/2      ---     sgm                   1. eNodeB 1.2 release
/main/2     ys001.102 rk     1. Patch 1 release for LTE CL 1.2
/main/2     ys002.102 rk     1. Patch 2 release for LTE CL 1.2
/main/2     ys003.102 sgm    1. Fix for CID:1622-02-01 DefectId:ccpu00115333
                                adding Cell delete feature for PAL.
/main/2      ys004.102     pmacharla       1. Patch 4 release for LTE CL 1.2
*********************************************************************91*/
