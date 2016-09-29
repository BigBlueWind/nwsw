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
  
     Name:     LTE RRC  Software Identifier
  
     Type:     C source file
  
     Desc:     Structure for software part number, version, and
               revision.

     File:     nh_id.c
  
     Sid:      ni_id.c@@/main/3 - Fri Jul  1 01:13:18 2011
 
     Prg:      vk
  
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



/* Header include files (.h) */

#include "envopt.h"           /* Environment options */  
#include "envdep.h"           /* Environment dependent */
#include "envind.h"           /* Environment independent */

#include "gen.h"              /* General layer */
#include "ssi.h"              /* System services */


/* Header/extern include files (.x) */

#include "gen.x"              /* General layer */
#include "ssi.x"              /* System services */

/* defines */

#define NHSWMV 3             /* RRC - main version */
#define NHSWMR 1             /* RRC - main revision */
#define NHSWBV 0             /* RRC - branch version */
#define NHSWBR 2             /* RRC - branch revision nh002.301*/
#define NHSWPN "1000369"     /* RRC - part number. Caution: When this
                              * string is modified the 
                              * LNH_PART_NUM_SIZE must be modified in
                              * lnh.h
                              */

/* Public variable declarations */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

EXTERN S16 nhGetSId ARGS((SystemId *sysId));

#ifdef __cplusplus
}
#endif /* __cplusplus */
  
/* Copyright banner */
  
PUBLIC CONSTANT Txt nhBan1[] =
   {"(c) COPYRIGHT 1989-2006, Continuous Computing Corporation."};

PUBLIC CONSTANT Txt nhBan2[] =
   {"                 All rights reserved."};
  
/* System ID */
  
PRIVATE CONSTANT SystemId sId ={
   NHSWMV,              /* RRC - main version */
   NHSWMR,              /* RRC - main revision */
   NHSWBV,              /* RRC - branch version */
   NHSWBR,              /* RRC - branch revision */
   NHSWPN               /* RRC - part number */
};

   
/******************************************************************************
*
*       Fun:   nhGetSId
*
*       Desc:  Get system ID consisting of part number, main version and
*              revision, and branch version and branch.
*
*       Ret:   TRUE - OK
*
*       Notes: None
*
*       File:  nh_id.c
*
******************************************************************************/
 
#ifdef ANSI
PUBLIC S16 nhGetSId
(
SystemId *sysId                 /* System ID */
)
#else /* ANSI */
PUBLIC S16 nhGetSId(sysId)
SystemId *sysId;                /* System ID */
#endif /* ANSI */
{
   TRC2(nhGetSId);


   sysId->mVer = sId.mVer;
   sysId->mRev = sId.mRev;
   sysId->bVer = sId.bVer;
   sysId->bRev = sId.bRev;
   sysId->ptNmb = sId.ptNmb;

   RETVALUE(TRUE);

} /* end of function nhGetSid */

/**********************************************************************
         End of file:     ni_id.c@@/main/3 - Fri Jul  1 01:13:18 2011
**********************************************************************/

/**********************************************************************
        Revision history:
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      vk                1. RRC Initial Release.
/main/2      ---      vk                1. Updated for RRC release 2.1
             ---      chebli            1. Patch Release nh001.201.
             ---      vkulkarni         1. Patch Release nh002.201.
             ---      vkulkarni         1. Patch Release nh003.201.
             ---      vkulkarni         1. Patch Release nh004.201.
/main/3      ---      sbalakrishna      1. Updated for Release of 3.1.
/main/3      ---      sbalakrishna      1. Patch Release nh001.301.
             ---      mpatel            1. Patch Release nh002.301.         
*********************************************************************91*/
