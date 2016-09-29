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
 
     Name:     Common LTE
  
     Type:     C source file
  
     Desc:     C code for packing/unpacking of cm_lte data structures. 
  
     File:     cm_lte.c 
  
     Sid:      cm_lte.c@@/main/2 - Fri Nov 13 14:09:17 2009
  
     Prg:      sm
  
**********************************************************************/

/* header include files (.h) */

#include "envopt.h"             /* environment options */  
#include "envdep.h"             /* environment dependent */
#include "envind.h"             /* environment independent */

#include "gen.h"                /* general */
#include "ssi.h"                /* system services interface */
#include "cm_lte.h"             /* common lte header file */

/* header/extern include files (.x) */

#include "gen.x"                /* general */
#include "ssi.x"                /* system services interface */
#include "cm_lte.x"             /* common lte header file */
#include "cm_lib.x"             /* common library functions header file */


/***********************************************************
*
*     Func : cmPkLteRlcId
*
*
*     Desc : RLC Identifier
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkLteRlcId
(
CmLteRlcId *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkLteRlcId(param, mBuf)
CmLteRlcId *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkLteRlcId);

   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   CMCHKPK(SPkU8, param->rbType, mBuf);
   CMCHKPK(cmPkLteRbId, param->rbId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkLteRlcId
*
*
*     Desc : RLC Identifier
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkLteRlcId
(
CmLteRlcId *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLteRlcId(param, mBuf)
CmLteRlcId *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkLteRlcId);

   CMCHKUNPK(cmUnpkLteRbId, &param->rbId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->rbType, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkLteTimingInfo
*
*
*     Desc : LTE Timing Info
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkLteTimingInfo
(
CmLteTimingInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkLteTimingInfo(param, mBuf)
CmLteTimingInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkLteTimingInfo);

   CMCHKPK(SPkU8, param->subframe, mBuf);
   CMCHKPK(SPkU16, param->sfn, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkLteTimingInfo
*
*
*     Desc : LTE Timing Info
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkLteTimingInfo
(
CmLteTimingInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLteTimingInfo(param, mBuf)
CmLteTimingInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkLteTimingInfo);

   CMCHKUNPK(SUnpkU16, &param->sfn, mBuf);
   CMCHKUNPK(SUnpkU8, &param->subframe, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkLtePdcpId
*
*
*     Desc : PDCP Id
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkLtePdcpId
(
CmLtePdcpId *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkLtePdcpId(param, mBuf)
CmLtePdcpId *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkLtePdcpId);

   CMCHKPK(SPkU8, param->rbType, mBuf);
   CMCHKPK(cmPkLteRbId, param->rbId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkLtePdcpId
*
*
*     Desc : PDCP Id
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkLtePdcpId
(
CmLtePdcpId *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLtePdcpId(param, mBuf)
CmLtePdcpId *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkLtePdcpId);

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteRbId, &param->rbId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->rbType, mBuf);
   RETVALUE(ROK);
}


/**********************************************************************
 
         End of file:     cm_lte.c@@/main/2 - Fri Nov 13 14:09:17 2009
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     sm                1. Initial Release.
*********************************************************************91*/
