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
  
        Name:    RLC service user (PDCP) lower interface
    
        Type:    C file
  
        Desc:    This file Contains the RLC service user lower interface
                  primitive implementain
 
        File:    kw_ptlib.c

        Sid:      kw_ptlib.c@@/main/1 - Fri Nov 13 14:14:14 2009
  
        Prg:     chakrapani
  
*********************************************************************21*/

/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "pju.h"           /* PJU defines */
#include "rgu.h"           /* RGU defines */
#include "cpj.h"           /* CPJ defines */
#include "lpj.h"           /* LPJ defines */
#include "pju.h"           /* PJU defines */
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "pju.x"           /* PJU */
#include "rgu.x"           /* RGU */
#include "cpj.x"           /* CPJ */
#include "lpj.x"           /* LPJ */
#include "pju.x"           /* PJU */
#include "kw.x"
#include "kw_pj_lib.x"     /* LIB */

#ifdef TENB_AS_SECURITY
PUBLIC U32 ysMsSpaccCipherPkt (Void* *pSec,Buffer *mBuf,Buffer **opSdu);
PUBLIC U32 ysMsSpaccIntegProtPkt(Void *pSec,Buffer *mBuf,U32 macI);
#endif

#ifdef KW_PDCP
#ifdef PTPJLIB
#ifdef PJ_CMP_ASYNC
/************************************************************************
                             PJU Interface Matrices
************************************************************************/

/****************************************************************************
 *                         PJU Interface Mt functions
 ***************************************************************************/

#ifdef ANSI
PUBLIC S16 PjLibObdCmpInitReq
(
Pst * pst,
PjCmpCxt cmpCxt,
PjRohc rohc
)
#else
PUBLIC S16 PjLibObdCmpInitReq(pst, cmpCxt, rohc)
Pst * pst;
PjCmpCxt cmpCxt;
PjRohc rohc;
#endif
{

   TRC3(PjLibObdCmpInitReq)

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjLibObdCmpReq
(
Pst * pst,
PTR cmpCxtId,
PjLibTrans libTrans,
Buffer * mBuf
)
#else
PUBLIC S16 PjLibObdCmpReq(pst, cmpCxtId, libTrans, mBuf)
Pst * pst;
PTR cmpCxtId;
PjLibTrans libTrans;
Buffer * mBuf;
#endif
{

   TRC3(PjLibObdCmpReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdDecmpReq
(
Pst * pst,
PTR cmpCxtId,
PjLibTrans libTrans,
Buffer * mBuf
)
#else
PUBLIC S16 PjLibObdDecmpReq(pst, cmpCxtId, libTrans, mBuf)
Pst * pst;
PTR cmpCxtId;
PjLibTrans libTrans;
Buffer * mBuf;
#endif
{

   TRC3(PjLibObdDecmpReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdCmpCloseReq
(
Pst * pst,
PTR cmpCxtId
)
#else
PUBLIC S16 PjLibObdCmpCloseReq(pst, cmpCxtId)
Pst * pst;
PTR cmpCxtId;
#endif
{

   TRC3(PjLibObdCmpCloseReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdCmpResetReq
(
Pst * pst,
PTR cmpCxtId
)
#else
PUBLIC S16 PjLibObdCmpResetReq(pst, cmpCxtId)
Pst * pst;
PTR cmpCxtId;
#endif
{

   TRC3(PjLibObdCmpResetReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdCmpFbReq
(
Pst * pst,
PTR cmpCxtId,
PjLibTrans libTrans,
Buffer *feedback
)
#else
PUBLIC S16 PjLibObdCmpFbReq(pst, cmpCxtId, libTrans, feedback)
Pst * pst;
PTR cmpCxtId;
PjLibTrans libTrans;
Buffer *feedback;
#endif
{

   TRC3(PjLibObdCmpFbReq)

   RETVALUE(ROK);

}

#else

#ifdef ANSI
PUBLIC S16 pjLibCmpInitReq 
(
PjCmpCxt cmpCxt,                  /* Context for ROHC */
PjRohc   rohc,                    /* ROHC Config Info */
PTR      *cmpCxtId                /* Compression Context ID */
)
#else
PUBLIC S16 pjLibCmpInitReq(cmpCxt, rohc, cmpCxtId)
PjCmpCxt cmpCxt;                  /* Context for ROHC */
PjRohc   rohc;                    /* ROHC Config Info */
PTR      *cmpCxtId;               /* Compression Context ID */
#endif
{
   TRC3(pjLibCmpInitReq)

   RETVALUE(ROK);

} /* end of pjLibCmpInitReq */

#ifdef ANSI
PUBLIC S16 pjLibCmpReq 
(
PTR      cmpCxtId,                /* Context Id for compression */
Buffer   *mBuf,                   /* SDU to be compressed */
Buffer   **opSdu                  /* Compressed SDU */
)
#else
PUBLIC S16 pjLibCmpReq(cmpCxtId, mBuf, opSdu)
PTR      cmpCxtId;                /* Context Id for compression */
Buffer   *mBuf;                   /* SDU to be compressed */
Buffer   **opSdu;                 /* Compressed SDU */
#endif
{
   TRC3(pjLibCmpReq)

   RETVALUE(ROK);

} /* end of pjLibCmpReq */

#ifdef ANSI
PUBLIC S16 pjLibDecmpReq 
(
PTR      cmpCxtId,                /* Context Id for compression */
Buffer   *mBuf,                   /* SDU to be decompressed */
Buffer   **feedback,                 /* Decompressed SDU */
Buffer   **opSdu               /* ROHC feedback buffer */
)
#else
PUBLIC S16 pjLibDecmpReq(cmpCxtId, mBuf, feedback, opSdu)
PTR      cmpCxtId;                /* Context Id for compression */
Buffer   *mBuf;                   /* SDU to be decompressed */
Buffer   **feedback;                 /* Decompressed SDU */
Buffer   **opSdu;              /* ROHC feedback buffer */
#endif
{
   TRC3(pjLibDecmpReq)

   RETVALUE(ROK);

} /* end of pjLibDecmpReq */

#ifdef ANSI
PUBLIC S16 pjLibCmpCloseReq
(
PTR      cmpCxtId                 /* ROHC Context to be closed */
)
#else
PUBLIC S16 pjLibCmpCloseReq(cmpCxtId)
PTR      cmpCxtId;                /* ROHC Context to be closed */
#endif
{
   TRC3(pjLibCmpCloseReq)

   RETVALUE(ROK);

} /* end of pjLibCmpCloseReq */

#ifdef ANSI
PUBLIC S16 pjLibCmpResetReq
(
PTR      cmpCxtId                 /* ROHC Context to be closed */
)
#else
PUBLIC S16 pjLibCmpResetReq(cmpCxtId)
PTR      cmpCxtId;                /* ROHC Context to be closed */
#endif
{
   TRC3(pjLibCmpResetReq)

   RETVALUE(ROK);

} /* end of pjLibCmpResetReq */

#ifdef ANSI
PUBLIC S16 pjLibCmpFbReq
(
PTR      cmpCxtId,                /* ROHC Context to be closed */
Buffer   *feedback                /* Feed back */
)
#else
PUBLIC S16 pjLibCmpFbReq(cmpCxtId, feedback)
PTR      cmpCxtId;                /* ROHC Context to be closed */
Buffer   *feedback;               /* Feed back */
#endif
{
   TRC3(pjLibCmpFbReq)

   RETVALUE(ROK);

} /* end of pjLibCmpFbReq */

#endif /* PJ_CMP_ASYNC */

#ifdef TENB_AS_SECURITY

#ifdef ANSI
PUBLIC S16 PjLibObdIntInitReq
(
Pst * pst,
PjSecCxt secCtx,
PjIntInfo intInfo
)
#else
PUBLIC S16 PjLibObdIntInitReq(pst, secCtx, intInfo)
Pst * pst;
PjSecCxt secCtx;
PjIntInfo intInfo;
#endif
{

   TRC3(PjLibObdIntInitReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdIntProtReq
(
Pst * pst,
PTR intCxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf
)
#else
PUBLIC S16 PjLibObdIntProtReq(pst, intCxtId, secInp, libTrans, mBuf)
Pst * pst;
PTR intCxtId;
PjSecInp secInp;
PjLibTrans libTrans;
Buffer * mBuf;
#endif
{

   U32 ret = ROK;
   PjMsSecInfo pSecInfo;
   U32         dummyMacI=0;


   TRC3(PjLibObdIntProtReq)

   /*(MSPD_DBG("\nPjLibObdIntProtReq : count address = %x\n",&secInp.count);*/
   MSPD_DBG("\nPjLibObdIntProtReq : count  = %x\n",secInp.count);
   /*cmMemcpy((U8 *)&(pSecInfo.count),(U8 *)&secInp.count,sizeof(secInp.count));*/
   
   PJ_SEC_FILL_COUNT(secInp.count, pSecInfo.count);	  

   pSecInfo.rbId = libTrans.rbId;
   pSecInfo.dir = secInp.dir;

   PJ_SEC_FILL_FRESH(pSecInfo.fresh, pSecInfo.rbId);

   pSecInfo.intAlgoType = libTrans.ueCb->secInfo.intInfo.algoType - 1;

   cmMemcpy(pSecInfo.integ_key,libTrans.ueCb->secInfo.intInfo.intKey, 16);

   if(pSecInfo.intAlgoType != 0)
   {
   ret = ysMsSpaccIntegProtPkt((Void*)&pSecInfo, mBuf, dummyMacI);
   }
   else
   {
      PJ_PACK_MACI(mBuf, dummyMacI);
      ret=ROK;
   }

   RETVALUE(ret);
}

#ifdef ANSI
PUBLIC S16 PjLibObdIntVerReq
(
Pst * pst,
PTR intCxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf,
U32   macI
)
#else
PUBLIC S16 PjLibObdIntVerReq(pst, intCxtId, secInp, libTrans, mBuf, macI)
Pst * pst;
PTR intCxtId;
PjSecInp secInp;
PjLibTrans libTrans;
Buffer * mBuf;
U32   macI;
#endif
{

   U32 ret = ROK;
   PjMsSecInfo pSecInfo;


   TRC3(PjLibObdIntVerReq)

   PJ_SEC_FILL_COUNT(secInp.count, pSecInfo.count);	  

   pSecInfo.rbId = libTrans.rbId;
   pSecInfo.dir = secInp.dir;

   PJ_SEC_FILL_FRESH(pSecInfo.fresh, pSecInfo.rbId);

   pSecInfo.intAlgoType = libTrans.ueCb->secInfo.intInfo.algoType - 1;
   cmMemcpy(pSecInfo.integ_key,libTrans.ueCb->secInfo.intInfo.intKey, 16);
    
   if(pSecInfo.intAlgoType != 0)
   {
      ret = ysMsSpaccIntegProtPkt((Void*)&pSecInfo, mBuf, macI);

      ret = ROK;
   }
   else
   {
     ret = ROK;
   }

  RETVALUE(ret);
}

#ifdef ANSI
PUBLIC S16 PjLibObdIntCloseReq
(
Pst * pst,
PTR intCxtId
)
#else
PUBLIC S16 PjLibObdIntCloseReq(pst, intCxtId)
Pst * pst;
PTR intCxtId;
#endif
{

   TRC3(PjLibObdIntCloseReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdCpInitReq
(
Pst * pst,
PjSecCxt secCxt,
U8 algoType,
U8 *cpKey
)
#else
PUBLIC S16 PjLibObdCpInitReq(pst, secCxt, algoType, cpKey)
Pst * pst;
PjSecCxt secCxt;
U8 algoType;
U8 *cpKey;
#endif
{

   TRC3(PjLibObdCpInitReq)

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjLibObdUpInitReq
(
Pst * pst,
PjSecCxt secCxt,
U8 algoType,
U8 *upKey
)
#else
PUBLIC S16 PjLibObdUpInitReq(pst, secCxt, algoType, upKey)
Pst * pst;
PjSecCxt secCxt;
U8 algoType;
U8 *upKey;
#endif
{

   TRC3(PjLibObdUpInitReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdCipherReq
(
Pst * pst,
PTR cxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf,
Buffer **opSdu
)
#else
PUBLIC S16 PjLibObdCipherReq(pst, cxtId, secInp, libTrans, mBuf)
Pst * pst;
PTR cxtId;
PjSecInp secInp;
PjLibTrans libTrans;
Buffer * mBuf;
#endif
{

   U32 ret = ROK;
   PjMsSecInfo pSecInfo;

   TRC3(PjLibObdCipherReq)

   PJ_SEC_FILL_COUNT(secInp.count, pSecInfo.count);	  

   pSecInfo.rbId = libTrans.rbId;
   pSecInfo.dir = secInp.dir;
   pSecInfo.rbType = libTrans.rbType;

   cmMemcpy(pSecInfo.cpCiph_key,libTrans.ueCb->secInfo.cipherInfo.cpKey,16);
   cmMemcpy(pSecInfo.upCiph_key,libTrans.ueCb->secInfo.cipherInfo.upKey,16);
   pSecInfo.ciphAlgoType = libTrans.ueCb->secInfo.cipherInfo.algoType-1;
   
   if(pSecInfo.ciphAlgoType == 0)
   {
      SCpyMsgMsg(mBuf, kwCb.init.region, kwCb.init.pool,  
                  opSdu);
   }
   else
   {
      PJ_ALLOC_BUF(*opSdu);
      ret = ysMsSpaccCipherPkt((Void*)&pSecInfo, mBuf, opSdu);
   }

   RETVALUE(ret);
}


#ifdef ANSI
PUBLIC S16 PjLibObdDecipherReq
(
Pst * pst,
PTR cxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf,
Buffer **opSdu 
)
#else
PUBLIC S16 PjLibObdDecipherReq(pst, cxtId, secInp, libTrans, mBuf)
Pst * pst;
PTR cxtId;
PjSecInp secInp;
PjLibTrans libTrans;
Buffer * mBuf;
#endif
{

   U32 ret = ROK;
   PjMsSecInfo pSecInfo;

   TRC3(PjLibObdDecipherReq)

   PJ_SEC_FILL_COUNT(secInp.count, pSecInfo.count);	  

   pSecInfo.rbId = libTrans.rbId;
   pSecInfo.dir = secInp.dir;
   pSecInfo.rbType = libTrans.rbType;

   cmMemcpy(pSecInfo.cpCiph_key,libTrans.ueCb->secInfo.cipherInfo.cpKey,16);
   cmMemcpy(pSecInfo.upCiph_key,libTrans.ueCb->secInfo.cipherInfo.upKey,16);
   pSecInfo.ciphAlgoType = libTrans.ueCb->secInfo.cipherInfo.algoType - 1;

   if(pSecInfo.ciphAlgoType == 0)
   {
      *opSdu = mBuf;
   }
   else
   {
     PJ_ALLOC_BUF(*opSdu);
     ret = ysMsSpaccCipherPkt((Void*)&pSecInfo,mBuf, opSdu);
     PJ_FREE_BUF(mBuf);
   }

   RETVALUE(ret);

}

#ifdef ANSI
PUBLIC S16 PjLibObdCipherCloseReq
(
Pst * pst,
PTR cpCxtId
)
#else
PUBLIC S16 PjLibObdCipherCloseReq(pst, cpCxtId)
Pst * pst;
PTR cpCxtId;
#endif
{

   TRC3(PjLibObdCipherCloseReq)

   RETVALUE(ROK);

}

#else

#ifdef ANSI
PUBLIC S16 PjLibObdIntInitReq
(
Pst * pst,
PjSecCxt secCtx,
PjIntInfo intInfo
)
#else
PUBLIC S16 PjLibObdIntInitReq(pst, secCtx, intInfo)
Pst * pst;
PjSecCxt secCtx;
PjIntInfo intInfo;
#endif
{

   TRC3(PjLibObdIntInitReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdIntProtReq
(
Pst * pst,
PTR intCxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf
)
#else
PUBLIC S16 PjLibObdIntProtReq(pst, intCxtId, secInp, libTrans, mBuf)
Pst * pst;
PTR intCxtId;
PjSecInp secInp;
PjLibTrans libTrans;
Buffer * mBuf;
#endif
{

   TRC3(PjLibObdIntProtReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdIntVerReq
(
Pst * pst,
PTR intCxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf,
U32   macI
)
#else
PUBLIC S16 PjLibObdIntVerReq(pst, intCxtId, secInp, libTrans, mBuf, macI)
Pst * pst;
PTR intCxtId;
PjSecInp secInp;
PjLibTrans libTrans;
Buffer * mBuf;
U32   macI;
#endif
{

   TRC3(PjLibObdIntVerReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdIntCloseReq
(
Pst * pst,
PTR intCxtId
)
#else
PUBLIC S16 PjLibObdIntCloseReq(pst, intCxtId)
Pst * pst;
PTR intCxtId;
#endif
{

   TRC3(PjLibObdIntCloseReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdCpInitReq
(
Pst * pst,
PjSecCxt secCxt,
U8 algoType,
U8 *cpKey
)
#else
PUBLIC S16 PjLibObdCpInitReq(pst, secCxt, algoType, cpKey)
Pst * pst;
PjSecCxt secCxt;
U8 algoType;
U8 *cpKey;
#endif
{

   TRC3(PjLibObdCpInitReq)

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjLibObdUpInitReq
(
Pst * pst,
PjSecCxt secCxt,
U8 algoType,
U8 *upKey
)
#else
PUBLIC S16 PjLibObdUpInitReq(pst, secCxt, algoType, upKey)
Pst * pst;
PjSecCxt secCxt;
U8 algoType;
U8 *upKey;
#endif
{

   TRC3(PjLibObdUpInitReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdCipherReq
(
Pst * pst,
PTR cxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf
)
#else
PUBLIC S16 PjLibObdCipherReq(pst, cxtId, secInp, libTrans, mBuf)
Pst * pst;
PTR cxtId;
PjSecInp secInp;
PjLibTrans libTrans;
Buffer * mBuf;
#endif
{

   TRC3(PjLibObdCipherReq)

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjLibObdDecipherReq
(
Pst * pst,
PTR cxtId,
PjSecInp secInp,
PjLibTrans libTrans,
Buffer * mBuf
)
#else
PUBLIC S16 PjLibObdDecipherReq(pst, cxtId, secInp, libTrans, mBuf)
Pst * pst;
PTR cxtId;
PjSecInp secInp;
PjLibTrans libTrans;
Buffer * mBuf;
#endif
{

   TRC3(PjLibObdDecipherReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdCipherCloseReq
(
Pst * pst,
PTR cpCxtId
)
#else
PUBLIC S16 PjLibObdCipherCloseReq(pst, cpCxtId)
Pst * pst;
PTR cpCxtId;
#endif
{

   TRC3(PjLibObdCipherCloseReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 pjLibIntInitReq 
(
PjSecCxt secCxt,                  /* Context for Intg protection/verification */
PjIntInfo intInfo,                /* Integrity Cfg Info */
PTR       *cxtId                  /* FD */
)
#else
PUBLIC S16 pjLibIntInitReq(secCxt, intInfo, cxtId)
PjSecCxt secCxt;                  /* Context for Intg protection/verification */
PjIntInfo intInfo;                /* Integrity Cfg Info */
PTR       *cxtId;                 /* FD */
#endif
{
   S16   ret = ROK;

   TRC3(pjLibIntInitReq)

   RETVALUE(ret);

} /* end of pjLibIntInitReq */

#ifdef ANSI
PUBLIC S16 pjLibCpInitReq
(
PjSecCxt secCxt,                  /* Context for Intg protection/verificatino */
U8       algoType,                /* Type of the algorithm */
U8       *cpKey,                  /* Control plane ciphering key */
PTR      *cxtId                   /* FD */
)
#else
PUBLIC S16 pjLibCpInitReq(secCxt, algoType, cpKey, cxtId)
PjSecCxt secCxt;                  /* Context for Intg protection/verificatino */
U8       algoType;                /* Type of the algorithm */
U8       *cpKey;                  /* Control plane ciphering key */
PTR      *cxtId;                  /* FD */
#endif
{
   S16   ret = ROK;

   TRC3(pjLibCpInitReq)

   RETVALUE(ret);

} /* end of pjLibCpInitReq */

#ifdef ANSI
PUBLIC S16 pjLibUpInitReq
(
PjSecCxt secCxt,                  /* Context for Intg protection/verificatino */
U8       algoType,                /* Type of the algorithm */
U8       *upKey,                  /* User plane ciphering key */
PTR      *cxtId                   /* FD */
)
#else
PUBLIC S16 pjLibUpInitReq(secCxt, algoType, upKey, cxtId)
PjSecCxt secCxt;                  /* Context for Intg protection/verificatino */
U8       algoType;                /* Type of the algorithm */
U8       *upKey;                  /* User plane ciphering key */
PTR      *cxtId;                  /* FD */
#endif
{
   TRC3(pjLibUpInitReq)

   RETVALUE(ROK);

} /* end of pjLibUpInitReq */

#ifdef ANSI
PUBLIC S16 pjLibIntProtReq 
(
PTR        intCxtId,                /* Context Id for compression */
PjSecInp   secInp,                  /* Input for Integrity protection */                  
Buffer     *mBuf,                   /* SDU to be compressed */
U32        *macI                    /* Message authentication code for the SDU */
)
#else
PUBLIC S16 pjLibIntProtReq(intCxtId, secInp, mBuf, macI)
PTR        intCxtId;                /* Context Id for compression */
PjSecInp   secInp;                  /* Input for Integrity protection */
Buffer     *mBuf;                   /* SDU to be compressed */
U32        *macI;                   /* Message authentication code for the SDU */
#endif
{
   TRC3(pjLibIntProtReq)
#ifdef LTE_HENB
   /*  kw001.201 */
   *macI=0;
#endif

   RETVALUE(ROK);

} /* end of pjLibIntProtReq */

#ifdef ANSI
PUBLIC S16 pjLibCipherReq 
(
PTR         secCxtId,           /* Context Id for compression */
PjSecInp    secInp,             /* Input for Ciphering */                  
Buffer      *mBuf,              /* SDU to be ciphered */
Buffer      **opSdu             /* ciphered SDU */
)
#else
PUBLIC S16 pjLibCipherReq(secCxtId, secInp, mBuf, opSdu)
PTR         secCxtId;           /* Context Id for ciphering */
PjSecInp    secInp;             /* Input for ciphering */                  
Buffer      *mBuf;              /* SDU to be ciphering */
Buffer      **opSdu;            /* Ciphering SDU */
#endif
{
   TRC3(pjLibCipherReq)

#ifdef LTE_HENB
   /*  kw001.201 */
   SCpyMsgMsg(mBuf, kwCb.init.region, kwCb.init.pool,  
                  opSdu);
#endif
      
   RETVALUE(ROK);

} /* end of pjLibCmpReq */

#ifdef ANSI
PUBLIC S16 pjLibIntVerReq 
(
PTR        intCxtId,                /* Context Id for compression */
PjSecInp   secInp,                  /* Input for Integrity protection */                  
Buffer     *mBuf,                   /* SDU to be compressed */
U32        macI,                    /* Message authentication code for the SDU */
Status     *status                  /* Status of the Integrity verification */
)
#else
PUBLIC S16 pjLibIntVerReq(intCxtId, secInp, mBuf, macI, status)
PTR        intCxtId;                /* Context Id for compression */
PjSecInp   secInp;                  /* Input for Integrity protection */
Buffer     *mBuf;                   /* SDU to be compressed */
U32        macI;                    /* Message authentication code for the SDU */
Status     *status;                 /* Status of the Integrity verification */
#endif
{
   TRC3(pjLibIntVerReq)
#ifdef LTE_HENB
    /*  kw001.201 */
    *status = ROK;
#endif
   RETVALUE(ROK);

} /* end of pjLibIntVerReq */

#ifdef ANSI
PUBLIC S16 pjLibDecipherReq 
(
PTR         intCxtId,           /* Context Id for compression */
PjSecInp    secInp,             /* Input for Deciphering */                  
Buffer      *mBuf,              /* SDU to be deciphered */
Buffer      **opSdu             /* deciphered SDU */
)
#else
PUBLIC S16 pjLibDecipherReq(intCxtId, secInp, mBuf, opSdu)
PTR         intCxtId;           /* Context Id for Deciphering */
PjSecInp    secInp;             /* Input for Deciphering */                  
Buffer      *mBuf;              /* SDU to be Deciphering */
Buffer      **opSdu;            /* Deciphering SDU */
#endif
{
   TRC3(pjLibDecipherReq)
#ifdef LTE_HENB
   /*  kw001.201 :Allocate memory and Copy things back */
   /* DATAFORPAL */
   *opSdu = mBuf;
#endif
   RETVALUE(ROK);

} /* end of pjLibDecipherReq */

#ifdef ANSI
PUBLIC S16 pjLibIntCloseReq
(
U32      intCxtId                 /* Integration CxtId to be closed */
)
#else
PUBLIC S16 pjLibIntCloseReq(intCxtId)
U32      intCxtId;                /* Integration CxtId to be closed */
#endif
{
   TRC3(pjLibIntCloseReq)

   RETVALUE(ROK);

} /* end of pjLibIntCloseReq */

#ifdef ANSI
PUBLIC S16 pjLibCipherCloseReq
(
U32      cxtId                  /* Context Id for SRBs to be closed */
)
#else
PUBLIC S16 pjLibCipherCloseReq(cxtId)
U32      cxtId;                 /* Context Id for SRBs to be closed */
#endif
{
   TRC3(pjLibCipherCloseReq)

   RETVALUE(ROK);

} /* end of pjLibCipherCloseReq */

#ifdef ANSI
PUBLIC S16 pjLibUpCloseReq
(
U32      upCxtId                  /* Context Id for DRBs to be closed */
)
#else
PUBLIC S16 pjLibUpCloseReq(upCxtId)
U32      upCxtId;                 /* Context Id for DRBs to be closed */
#endif
{
   TRC3(pjLibUpCloseReq)

   RETVALUE(ROK);

} /* end of pjLibUpCloseReq */

#endif /* PJ_SEC_ASYNC */

/* kw003.201 This function is for admission control at packet level */
#ifdef ANSI
PUBLIC S16 pjLibPktAdmCtrl
(
PjRbCb        *pjRbCb,
Buffer        *pkt
) 
#else
PUBLIC S16 pjLibPktAdmCtrl(pjRbCb, pkt)
PjRbCb        *pjRbCb;
Buffer        *pkt;
#endif 
{
   extern U32 ysTotalPkts, ysPktsDropped, ysPktsDrpdMem; 
   extern U32 memAlrm;

   TRC3(pjLibPktAdmCtrl)
#ifdef MSPD
   ysTotalPkts++;
    /* RLC_DL_MAX_RETX fix */
   if((memAlrm == 1) || (pjRbCb->dlCb.txBuf.numEntries > 800)||
   	((pjRbCb->mode == CM_LTE_MODE_AM) && (pjRbCb->kwRbCb->m.am.amDl.isRbstalled == TRUE)))
   {
      ysPktsDropped++;
      MSPD_DBG("[%d][%d] (%d,%d,%d) sduQ %d\n", pjRbCb->ueCb->key.ueId,
                pjRbCb->rbId, pjRbCb->dlCb.txBuf.numEntries,
                pjRbCb->mode, pjRbCb->kwRbCb->m.am.amDl.isRbstalled, 
                pjRbCb->kwRbCb->m.am.amDl.sduQ.count);
      if(memAlrm == 1)
      {
         ysPktsDrpdMem++;
      }
      RETVALUE(RFAILED);
   }
#endif
#ifdef SS_CAVIUM
      if(pjRbCb->dlCb.txBuf.numEntries > 450)
      {
         RETVALUE(RFAILED);
      }
#endif
   RETVALUE(ROK);

} /* end of pjLibUpCloseReq */

#endif /* PTPJLIB */
#endif /* KW_PDCP */
  
/********************************************************************30**
         End of file:     kw_ptlib.c@@/main/1 - Fri Nov 13 14:14:14 2009
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
/main/1      ---         cp    1. Initial release.
/main/1      ---         gk    1. LTERLC Release 2.1
/main/2      kw001.201   vp    1. Patch  for HENB, changes for max RBs
                                    in cell and avoidance of float variables.
/main/5      kw003.201   vp    1. Fixes for re-segmentation, segement offset
                               value and optimization 
/main/6      kw004.201   ap    1. Added support for SS_CAVIUM in the function 
                                  pjLibPktAdmCtrl
*********************************************************************91*/
