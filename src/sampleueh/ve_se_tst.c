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

     Name:     LTE Uu Stack Manager 
  
     Type:     C Source file
  
     Desc:     Stack Manager Initialization Functions.

     File:     ve_se_tst.c

     Sid:      ve_se_tst.c@@/main/2 - Wed Dec  1 08:44:30 2010

     Prg:      an 

**********************************************************************/


#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */

#include "cm5.h"           /* common timers */
#include "cm_hash.h"       /* common hash */

/* header/extern include files (.x) */

#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash */
#define SCH_INST_ID 1

/* g++ compilation fix,name mangling  */
#ifdef __cplusplus
EXTERN "C" {
#endif  /*__cplusplus*/

EXTERN S16 kwActvInit ARGS ((
Ent    ent,                 /* entity */
Inst   inst,                /* instance */
Region region,              /* region */
Reason reason               /* reason */
));

EXTERN S16 kwActvTsk ARGS ((
Pst *pst,              /* pst structure */
Buffer *mBuf            /* message buffer */
));

EXTERN S16 pjActvInit ARGS ((
Ent    ent,                 /* entity */
Inst   inst,                /* instance */
Region region,              /* region */
Reason reason               /* reason */
));

EXTERN S16 pjActvTsk ARGS ((
Pst *pst,              /* pst structure */
Buffer *mBuf            /* message buffer */
));

EXTERN S16 rgActvInit ARGS((Ent entity, Inst inst, Region 
       region, Reason reason));

EXTERN S16 rgActvTsk ARGS((Pst *pst, Buffer *mBuf));

EXTERN S16 schActvInit ARGS((Ent entity, Inst inst, Region 
       region, Reason reason));

EXTERN S16 schActvTsk ARGS((Pst *pst, Buffer *mBuf));

EXTERN S16 ysActvInit ARGS((Ent entity, Inst inst, Region 
       region, Reason reason));

EXTERN S16 ysActvTsk ARGS((Pst *pst, Buffer *mBuf));

EXTERN S16 smKwInit ARGS ((SSTskId    sysTskId));

EXTERN S16 smYsInit ARGS ((SSTskId    sysTskId));

EXTERN S16 smRgInit ARGS ((SSTskId    sysTskId));

#ifdef __cplusplus
      }
#endif  /*__cplusplus*/

/*
*
*       Fun:   Initialize RLC/PDCP task.
*
*       Desc:  Invoked to create RLC/PDCP TAPA task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_se_tst.c
*
*/
#ifdef ANSI
PUBLIC S16 smKwInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smKwInit(sysTskId)
SSTskId    sysTskId;
#endif
{
   TRC2(smKwInit);

   /* Register RRC TAPA Task */
   if (SRegTTsk((Ent)ENTKW, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
            kwActvInit, (ActvTsk)kwActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Register RRC TAPA Task */
   if (SRegTTsk((Ent)ENTPJ, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
            pjActvInit, (ActvTsk)pjActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Attach RRC TAPA Task */
   if (SAttachTTsk((Ent)ENTKW, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */
   if (SAttachTTsk((Ent)ENTPJ, (Inst)0, sysTskId)!= ROK)
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

   RETVALUE(ROK);
} /* end of smKwInit */

/*
*
*       Fun:   Initialize MAC task.
*
*       Desc:  Invoked to create MAC TAPA task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_se_tst.c
*
*/
#ifdef ANSI
PUBLIC S16 smRgInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smRgInit(sysTskId)
SSTskId    sysTskId;
#endif
{
   TRC2(smRgInit);

   /* Register RRC TAPA Task */
   if (SRegTTsk((Ent)ENTRG, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
            rgActvInit, (ActvTsk)rgActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Attach RRC TAPA Task */
   if (SAttachTTsk((Ent)ENTRG, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Register MAC SCHEDULER Task */
   if (SRegTTsk((Ent)ENTRG, (Inst)SCH_INST_ID, (Ttype)TTNORM, (Prior)PRIOR0,
            schActvInit, (ActvTsk)schActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Attach RRC TAPA Task */
   if (SAttachTTsk((Ent)ENTRG, (Inst)SCH_INST_ID, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

   RETVALUE(ROK);
} /* end of smRgInit */

/*
*
*       Fun:   smYsInit
*
*       Desc:  Invoked to create LTE CL TAPA task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_se_tst.c
*
*/
#ifdef ANSI
PUBLIC S16 smYsInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smYsInit(sysTskId)
SSTskId    sysTskId;
#endif
{
   TRC2(smYsInit);

   /* Register CL TAPA Task */
   if (SRegTTsk((Ent)ENTYS, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
            ysActvInit, (ActvTsk)ysActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

   if (SRegTTsk((Ent)ENTTF, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
            NULLP, (ActvTsk)ysActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Attach RRC TAPA Task */
   if (SAttachTTsk((Ent)ENTTF, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Attach RRC TAPA Task */
   if (SAttachTTsk((Ent)ENTYS, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

   RETVALUE(ROK);
} /* end of smYsInit */


/*
*
*       Fun:   tst
*
*       Desc:  tst entry point. First entry point.
*
*       Ret:   ROK on success, RFAILED on error
*
*       Notes: None
*
*       File:  ve_se_tst.c
*
*/
#ifdef ANSI
PUBLIC S16 tst
(
Void
)
#else
PUBLIC S16 tst(Void)
#endif
{
   
   /* System tasks that are needed as per the architecture */
   SSTskId rlc_pdcp_mac_cl_taskId;

   TRC2(tst);

   /* All the System tasks(threads)  are created with same priority*/
   SCreateSTsk(PRIOR0, &rlc_pdcp_mac_cl_taskId); 

   /* Create RLC/PDCP tasks and attach them 
      to a single system thread */
   if(smKwInit(rlc_pdcp_mac_cl_taskId) != ROK)
   {
      /* Fix for CID- 1622-02-01 DefectId- 115851 */
      /* Removed debug print */
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Create LTE MAC tapa task and attach it
      to a single system thread */
   if(smRgInit(rlc_pdcp_mac_cl_taskId) != ROK )
   {
      /* Fix for CID- 1622-02-01 DefectId- 115851 */
      /* Removed debug print */
      RETVALUE(RFAILED);
   } /* end of if statement */


   /* Create Convergence layer TAPA tasks and attach them 
      to a single system thread */
   if(smYsInit(rlc_pdcp_mac_cl_taskId) != ROK)
   {
      /* Fix for CID- 1622-02-01 DefectId- 115851 */
      /* Removed debug print */
      RETVALUE(RFAILED);
   } /* end of if statement */

   RETVALUE(ROK);
} /* end of tst function */

/*
*
*       Fun:   rdConQ
*
*       Desc:  
*
*       Ret:   ROK on success, RFAILED on error
*
*       Notes: None
*
*       File:  ve_se_tst.c
*
*/
#ifdef ANSI
PUBLIC S16 rdConQ
(
Data data
)
#else
PUBLIC S16 rdConQ(data)
Data data;
#endif
{
   TRC2(rdConQ);
   RETVALUE(ROK);
}



/**********************************************************************
  
         End of file:     ve_se_tst.c@@/main/2 - Wed Dec  1 08:44:30 2010
   
**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
