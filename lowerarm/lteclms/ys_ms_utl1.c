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
  
     File:     ys_utl.c
  
     Sid:      yw_utl.c@@/main/2 - Thu Dec  2 02:27:28 2010
  
     Prg:      pk
  
**********************************************************************/

/** @file ys_utl.c
@brief This module acts as an interface handler for upper interface and 
manages Pst and Sap related information for upper interface APIs.
*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_lte.h"
#include "ctf.h"           /* CTF defines */
#include "lys.h"           /* layer management defines for LTE-CL */
#include "tfu.h"
#ifdef YS_MSPD
#include "LtePhyL2Api.h"
#include "apidefs.h"
#include "ys_ms.h"         /* defines and macros for CL */
#else
#include "ys.h"            /* defines and macros for CL */
#include <sys/time.h>
#endif /* YS_MSPD */
#include "ys_ms_err.h"        /* YS error defines */

#ifdef TENB_AS_SECURITY
#include "spaccdrv.h" /*sumanth*/
#endif


/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"
#include "ctf.x"           /* CTF types */
#include "lys.x"           /* layer management typedefs for CL */
#include "tfu.x"
#ifdef YS_MSPD
#include "ys_ms.x"         /* typedefs for CL */
#endif


/*
*
*       Fun:   ysUtlMalloc
*
*       Desc:  This function allocates a static memory buffer of 
*              requested size.
*
*       Ret:   (Data *) - on success
*              NULLP    - on failure
*
*       Notes: None
*
*       File:  ys_util.c
*
*/

#ifdef ANSI
PUBLIC Data * ysUtlMalloc
(
Size               size                /* Static Buffer Size Requested */
)
#else
PUBLIC Data * ysUtlMalloc (size)
Size               size;               /* Static Buffer Size Requested */
#endif
{
   TskInit         *ysInit;            /* CL Task Init Structure */
   Data            *buf;               /* Buffer to be allocated */

   TRC2(ysUtlMalloc)

   buf    = NULLP;
   ysInit = &(ysCb.ysInit);

   if (size <= 0)
   {
      YS_LOGERR_DBG(EYS018, size, "ysUtlMalloc(): invalid size\n");
      RETVALUE (NULLP);
   }

   /* Obtain the Static Buffer */
   if((SGetSBuf(ysInit->region, ysInit->pool, &buf, size) != ROK) ||
      (buf == NULLP))
   {
      RETVALUE(NULLP);
   }

   /* Initialise the buffer */
   cmMemset (buf, 0, size);

   RETVALUE(buf);

} /* end of ysUtlMalloc */

/*
*
*       Fun:   ysUtlDeAlloc
*
*       Desc:  This function returns a static memory buffer to 
*              the buffer pool.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ys_util.c
*
*/

#ifdef ANSI
PUBLIC Void ysUtlDeAlloc
(
Data               *buf,               /* Static Buffer to be freed */
Size               size                /* Size of Buffer */
)
#else
PUBLIC Void ysUtlDeAlloc (buf, size)
Data               *buf;               /* Static Buffer to be freed */
Size               size;               /* Size of Buffer */
#endif
{
   TskInit         *ysInit;            /* CL Task Init Structure */

   TRC2(ysUtlDeAlloc)

   if ((buf == NULLP) || (size <= 0) )
   {
      Txt   prntBuf[PRNTSZE];

      sprintf(prntBuf, "ysUtlDeAlloc: buf=%p\n", buf);

      YS_LOGERR_DBG(EYS019, size, prntBuf);
      RETVOID;
   }

   ysInit = &(ysCb.ysInit);

   /* Free the memory */
   SPutSBuf (ysInit->region, ysInit->pool, buf, size);

   RETVOID;

} /* end of ysUtlDeAlloc */

/*
*
*       Fun:   ysUtlAllocEventMem
*
*       Desc:  This function allocates event memory 
*
*       Ret:   (Data *) - on success
*              NULLP    - on failure
*
*       Notes: None
*
*       File:  ys_utl.c
*
*/

#ifdef ANSI
PUBLIC S16 ysUtlAllocEventMem
(
Ptr       *memPtr,
Size      memSize
)
#else
PUBLIC S16 ysUtlAllocEventMem(memPtr, memSize)
Ptr       *memPtr;
Size      memSize;
#endif
{
   Mem   sMem;

   TRC2(ysUtlAllocEventMem)

   sMem.region = ysCb.ysInit.region;
   sMem.pool = ysCb.ysInit.pool;

   if (memSize<= 0)
   {
      YS_LOGERR_DBG(EYS020, memSize, "ysUtlAllocEventMem(): invalid size\n");
      RETVALUE (RFAILED);
   }

   if(ROK != cmAllocEvnt(memSize, YS_MEM_SDU_SIZE, &sMem, memPtr))
   {
      YS_DBG_ERR((_ysp, "Returning RFAILED form ysUtlAllocEventMem because \
                    cmAllocEvnt returned RFAILED\n"));
      *memPtr = NULLP;
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

} /* end of ysUtlAllocEventMem*/

/*
*
*       Fun:   ysUtlFreeEventMem
*
*       Desc:  This function frees event memory 
*
*       Ret:   
*
*       Notes: None
*
*       File:  ys_utl.c
*
*/

#ifdef ANSI
PUBLIC S16 ysUtlFreeEventMem
(
Ptr       memPtr
)
#else
PUBLIC S16 ysUtlFreeEventMem(memPtr)
Ptr       memPtr;
#endif
{
   TRC2(ysUtlFreeEventMem)

   if (memPtr == NULLP)
   {
      YS_LOGERR_DBG(EYS021, 0, "ysUtlFreeEventMem(): invalid ptr\n");
      RETVALUE (RFAILED);
   }

   cmFreeMem(memPtr);

   RETVALUE(ROK);
} /* end of ysUtlFreeEventMem */

/*
*
*       Fun:    ysUtlFreeDatReq 
*
*       Desc:   This function return UE control block 
*
*       Ret:   Pointer to ue control block 
*
*       Notes: None 
*
*       File:   ys_utl.c
*/
#ifdef ANSI
PUBLIC Void ysUtlFreeDatReq
(
TfuDatReqInfo   *datReq
)
#else
PUBLIC Void ysUtlFreeDatReq(datReq)
TfuDatReqInfo   *datReq;
#endif
{
   U8                idx; 
   TfuDatReqPduInfo  *pduInfo;
   CmLList           *cmLstEnt;
   if (datReq->bchDat.pres == PRSNT_NODEF)
   {
      SPutMsg (datReq->bchDat.val);
      datReq->bchDat.val = NULLP;
   }
   cmLstEnt = datReq->pdus.first;
   while (cmLstEnt != NULLP)
   {
      pduInfo = (TfuDatReqPduInfo *)cmLstEnt->node;
      for (idx = 0; idx < TFU_MAX_TB; idx++)
      {
         if((pduInfo) && (pduInfo->mBuf[idx] != NULLP))
         {
            SPutMsg (pduInfo->mBuf[idx]);
            pduInfo->mBuf[idx] = NULLP;
         }
      }
      cmLstEnt = cmLstEnt->next;
   }
   ysUtlFreeEventMem((Ptr)datReq);
}



void postToApp(Buffer *mBuf, U8 radioType, U8 direction, U8 rntiType,
                       U16     rnti, U16 ueid,     U16       subframe,
                       U8      isPredefinedData, U8 retx, U8 crcStatus)
{
}

/************************************************************30**
  
         End of file:     yw_utl.c@@/main/2 - Thu Dec  2 02:27:28 2010
  
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
/main/2      ---     sgm    1. eNodeB 1.2 release
/main/2    ys003.102 sgm    1. Fix for CID:1622-02-01 DefectId:ccpu00115333
/main/2    ys004.102 pkd    1. Cell Delete Fix CRID:ccpu00117556 
*********************************************************************91*/
