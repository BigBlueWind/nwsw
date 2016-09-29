/********************************************************************16**

                         (c) COPYRIGHT 2011 by 
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

                    Email: support@ccpu.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_mux.c 
  
     Sid:      gk_mux.c@@/main/3 - Sat Jul 30 02:21:31 2011
  
     Prg:      ps
  
**********************************************************************/

/** @file rg_mux.c
@brief MAC Multiplexing API.
*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
  
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */

#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_lte.h"        /* Common LTE Defines */

#include "rg_env.h"        /* MAC Environment Defines */
#include "tfu.h"           /* TFU Interface defines */
#include "crg.h"           /* CRG Interface defines */
#include "rg_sch_inf.h"           /* RGR Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "lrg.h"           /* LRG Interface defines */

#include "rg_err.h"        /* MAC error defines */
#include "rg.h"            /* MAC defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* memory management */
#include "cm_lte.x"        /* Common LTE Definitions */

#include "tfu.x"           /* CRG Interface includes */
#include "crg.x"           /* CRG Interface includes */
#include "rg_sch_inf.x"           /* RGR Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "lrg.x"           /* LRG Interface includes */

#include "rg.x"            /* MAC includes */
/* local defines */
PRIVATE U8 padArr[RG_MAX_PAD_ARR_SZ];
/* local typedefs */
typedef struct RgMUXSubHdr
{
   U8 shLen;  
   U8 shData[RG_MAX_SDU_SUB_HDR_LEN]; /* RRELCID + F-L (O) + L (O) */
} RgMUXSubHdr;

/* local externs */

PRIVATE Void rgMUXGet20bitRarGrnt ARGS((U8 ulBw,
                                        RgInfRarUlGrnt *msg3Grnt,
                                        U8 *grnt));
PRIVATE U16 rgMUXCalcRiv ARGS((U8 bw,
                                U8 rbStart,
                                U8 numRb));
 
/* forward references */

#define RG_PACK_CE_SHDR(_subHdr, _lcId, _eBit, _mBuf, _ret) {\
   _subHdr.shLen = RG_CE_SHDR_LEN;\
   _subHdr.shData[0] = (0x3F & ((_eBit << 5) | _lcId));\
   _ret = SAddPstMsgMult(&_subHdr.shData[0], _subHdr.shLen, _mBuf);\
}

#define RG_PACK_LAST_CE_SHDR(_subHdr, _lcId, _mBuf, _ret) {\
   _subHdr.shLen = RG_CE_SHDR_LEN;\
   _subHdr.shData[0] = (0x1F & _lcId);\
   _ret = SAddPstMsgMult(&_subHdr.shData[0], _subHdr.shLen, _mBuf);\
}

#define RG_PACK_CE(_ce, _len, _ceBuf, _ret) {\
   _ret = SAddPstMsgMult((U8 *)(&(_ce)), _len, _ceBuf);\
}

#define RG_MUX_CALC_LEN(_len,_lenBytes,_elmTotLen) {\
   U8 _hdrLen;\
   _lenBytes    = (_len <= 127) ? 1 : 2;\
   _hdrLen      = _lenBytes + RG_SDU_SHDR_LEN;\
   _elmTotLen   = _hdrLen + _len;\
}

#define RG_PACK_SDU_SHDR(_subHdr, _lcId, _len, _eBit, _mBuf, _ret) {\
   _ret = ROK;\
   _subHdr.shData[0] = (0x3F & ((_eBit << 5) | _lcId));\
   if(_len <= 127)\
   {\
      _subHdr.shLen = 2;\
      _subHdr.shData[1] = (0x7F & _len);\
      _subHdr.shData[2] = 0;\
   }\
   else\
   {\
      _subHdr.shLen = 3;\
      _subHdr.shData[1] = (0x80 | (0x7F & (_len >> 8)));\
      _subHdr.shData[2] = (0xFF & _len);\
   }\
   _ret = SAddPstMsgMult(&_subHdr.shData[0], _subHdr.shLen, _mBuf);\
}

#define RG_PACK_LAST_SDU_SHDR(_subHdr, _lcId, _mBuf, _ret) {\
   _ret = ROK;\
   _subHdr.shLen = 1;\
   _subHdr.shData[0] = (0x1F & _lcId);\
   _ret = SAddPstMsgMult(&_subHdr.shData[0], _subHdr.shLen, _mBuf);\
}

#define RG_PACK_SDU(_sdusBuf, _sduBuf, _ret) {\
   _ret = SCatMsg(_sdusBuf, _sduBuf, M1M2);\
}

#define RG_PACK_PAD_SHDR(_eBit, _mBuf, _ret) {\
   if(_eBit)\
   {\
      _ret = SAddPreMsg(0x3F, _mBuf);\
   }\
   else\
   {\
      _ret = SAddPstMsg(0x1F, _mBuf);\
   }\
}

#define RG_PACK_PAD(_padArr, _len, _sdusBuf, _ret) {\
   _ret = SAddPstMsgMult(&_padArr[0], _len, _sdusBuf);\
}

#define RG_PACK_RAR_SHDR(_byte, _mBuf, _ret) {\
   _ret = SAddPstMsg(_byte, _mBuf);\
}

#define RG_MAX_NUM_PAD_HDRS 2

/**
 * @brief Function to add ces along with subhdrs. 
 *
 * @details
 *
 *     Function : rgMUXAddCes
 *     
 *  @param[in] RgBldPduInfo   *pdu
 *  @param[in] Buffer        *mBuf 
 *  @param[in] Buffer        *ceBuf 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgMUXAddCes
(
RgBldPduInfo   *pdu,
Buffer         *ceShdrBuf,
Buffer         *ceBuf,
RgErrInfo      *err
)
#else
PRIVATE S16 rgMUXAddCes(pdu, ceShdrBuf, ceBuf, err)
RgBldPduInfo   *pdu;
Buffer         *ceShdrBuf;
Buffer         *ceBuf;
RgErrInfo      *err;
#endif
{
   S16            ret;
   RgMUXSubHdr    subHdr;
   /* Moved up the initialization*/
   U8             ceSubHdr = RG_HDR_TYPE_CRES; 

   /*rg003.201:Changed to correct function name in TRC */
   TRC2(rgMUXAddCes)

  /* rg005.201 Removing the while loop, since the cases in switch are sequential 
     for optimization */
   switch(ceSubHdr)
   {
      case RG_HDR_TYPE_CRES:
      {
         if (NULLP != pdu->contResId)
         {
            if(pdu->schdTbSz >= RG_CRES_ELM_LEN)
            {
               if(pdu->schdTbSz > RG_CRES_ELM_LEN)
               {
                  RG_PACK_CE_SHDR(subHdr, RG_CRES_LCID_IDX, TRUE, ceShdrBuf, ret);
               }
               else
               {
                  RG_PACK_LAST_CE_SHDR(subHdr, RG_CRES_LCID_IDX, ceShdrBuf, ret);
               }

               if(ret != ROK)
               {
                  err->errCause = RGERR_MUX_BLD_CEHDR_FAIL;
                  MSPD_ERR("RGERR_MUX_BLD_CEHDR_FAIL \n");
                  RETVALUE(RFAILED);
               }

               RG_PACK_CE(pdu->contResId->resId[0], RG_CRES_LEN, ceBuf, ret);

               if(ret != ROK)
               {
                  err->errCause = RGERR_MUX_BLD_CE_FAIL;
                  RETVALUE(RFAILED);
               }
               pdu->schdTbSz -= RG_CRES_ELM_LEN;
            }
         }
      }
      case RG_HDR_TYPE_TA:
      {  
         if (TRUE == pdu->ta.pres)
         {
            if(pdu->schdTbSz >= RG_TA_ELM_LEN)
            {
               U8 taVal; /* Moving from outer scope to available scope */
               if(pdu->schdTbSz > RG_TA_ELM_LEN)
               {
                  RG_PACK_CE_SHDR(subHdr, RG_TA_LCID_IDX, TRUE, ceShdrBuf, ret);
               }
               else
               {
                  RG_PACK_LAST_CE_SHDR(subHdr, RG_TA_LCID_IDX, ceShdrBuf, ret);
               }

               if(ret != ROK)
               {
                  err->errCause = RGERR_MUX_BLD_CEHDR_FAIL;
                  MSPD_ERR("RGERR_MUX_BLD_CEHDR_FAIL \n");
                  RETVALUE(RFAILED);
               }

               taVal = pdu->ta.val;
               RG_PACK_CE(taVal, RG_TA_LEN, ceBuf, ret);

               if(ret != ROK)
               {
                  err->errCause = RGERR_MUX_BLD_CE_FAIL;
                  MSPD_ERR("RGERR_MUX_BLD_CE_FAIL \n");
                  RETVALUE(RFAILED);
               }
               pdu->schdTbSz -= RG_TA_ELM_LEN;
               MSPD_DBG("*********** TA muxed by MAC: %u \n",pdu->ta.val);
            }
         }
         break;
      }
      default: 
         RETVALUE(RFAILED);
   }
   
   RETVALUE(ROK);
} /* rgMUXAddCes */

/**
 * @brief Function to insert SDU along with sub headers.
 *
 * @details
 *
 *     Function : rgMUXInsSdu
 *     
 *  @param[in]       RgBldPduInfo   *pdu
 *  @param[out]      Buffer        *mBuf 
 *  @param[out]      Buffer        *sduBuf 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgMUXInsSdu
(
MsgLen         *schdTbSz,
U8             lcId,
Buffer         *sdu,
Buffer         *sHdrBuf,
Buffer         *sduBuf,
RgErrInfo      *err
)
#else
PRIVATE S16 rgMUXInsSdu(schdTbSz, lcId, sdu, sHdrBuf, sduBuf, err)
MsgLen         *schdTbSz;
U8             lcId;
Buffer         *sdu;
Buffer         *sHdrBuf;
Buffer         *sduBuf;
RgErrInfo      *err;
#endif
{
   S16            ret;
   MsgLen         msgLen = 0;
   U8             lenBytes;
   MsgLen         elmTotLen;

   TRC2(rgMUXInsSdu)

   SFndLenMsg(sdu, &msgLen);

   RG_MUX_CALC_LEN(msgLen,lenBytes,elmTotLen);
#ifdef MSPD
   {
      if (lcId == 3) {
         extern U32 dlrate_rgu;
         dlrate_rgu += msgLen;
		 /* MSPD_DBG(" MAC MsgLen %d\n", msgLen);*/
      }
   }
#endif
   if (*schdTbSz > elmTotLen)
   {
      RgMUXSubHdr    subHdr;
      RG_PACK_SDU_SHDR(subHdr, lcId, msgLen, TRUE, sHdrBuf, ret);
      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_SDUHDR_FAIL;
#ifdef MSPD
	 MSPD_ERR("RGERR_MUX_BLD_SDUHDR_FAIL\n");
#endif
         RETVALUE(RFAILED);
      }

      RG_PACK_SDU(sduBuf, sdu, ret);

      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_SDU_FAIL;
#ifdef MSPD
	 MSPD_ERR("RGERR_MUX_BLD_SDU_FAIL\n");
#endif
         RETVALUE(RFAILED);
      }

      *schdTbSz -= elmTotLen;
   }
   else if ((*schdTbSz <= elmTotLen) && 
                     (*schdTbSz >= (elmTotLen - lenBytes)))
   {
      RgMUXSubHdr    subHdr;
      if (*schdTbSz != (elmTotLen - lenBytes))
      {
         U16 numPdHdrs = *schdTbSz - (elmTotLen - lenBytes);
         if(numPdHdrs)
         {
            /* rg005.201 Packing multiple subheaders at one 
             * shot for optimization */   
            U8 sHdrArr[RG_MAX_NUM_PAD_HDRS] = {0x3F, 0x3F};
            if(numPdHdrs > RG_MAX_NUM_PAD_HDRS)
            {
               err->errCause = RGERR_MUX_BLD_PADHDR_FAIL;
#ifdef MSPD
	       MSPD_ERR("RGERR_MUX_BLD_PADHDR_FAIL\n");
#endif
               RETVALUE(RFAILED);
            }
            ret = SAddPreMsgMult(&sHdrArr[0], numPdHdrs, sHdrBuf);
            if(ret != ROK)
            {
               err->errCause = RGERR_MUX_BLD_PADHDR_FAIL;
#ifdef MSPD
	       MSPD_ERR("RGERR_MUX_BLD_PADHDR_FAIL\n");
#endif
               RETVALUE(RFAILED);
            }
            *schdTbSz-=numPdHdrs;
         } 
      }

      RG_PACK_LAST_SDU_SHDR(subHdr, lcId, sHdrBuf, ret);
      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_SDUHDR_FAIL;
#ifdef MSPD
	 MSPD_ERR("RGERR_MUX_BLD_SDUHDR_FAIL\n");
#endif
         RETVALUE(RFAILED);
      }

      RG_PACK_SDU(sduBuf, sdu, ret);

      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_SDU_FAIL;
#ifdef MSPD
	 MSPD_ERR("RGERR_MUX_BLD_SDU_FAIL\n");
#endif
         RETVALUE(RFAILED);
      }
        
      /*UE_MAC_FREE_BUF(sdu);*/

      *schdTbSz -= (msgLen + RG_SDU_SHDR_LEN);
   }
   else
   {
      /* This PDU can not be accodmodated at all */
      RGDBGINFO((rgPBuf, "TB Size is not enough to accomodate"
                                          " the element\n"));
#ifdef MSPD      
      MSPD_ERR("Failed lcId %u, elmTotLen %d lenBytes %d msglen %u schdTbSz %u\n",
	  	                    lcId, elmTotLen, lenBytes, msgLen, *schdTbSz);
#endif
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 * @brief Function to insert SDU along with sub headers.
 *
 * @details
 *
 *     Function : rgMUXAddPadd
 *     
 *  @param[in]       RgBldPduInfo   *pdu
 *  @param[out]      Buffer        *mBuf 
 *  @param[out]      Buffer        *sduBuf 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgMUXAddPadd
(
MsgLen         *schdTbSz,
Buffer         *sHdrBuf,
Buffer         *sduBuf,
Bool           isRar,
RgErrInfo      *err
)
#else
PRIVATE S16 rgMUXAddPadd(schdTbSz, sHdrBuf, sduBuf, isRar, err)
MsgLen         *schdTbSz;
Buffer         *sHdrBuf;
Buffer         *sduBuf;
Bool           isRar;
RgErrInfo      *err;
#endif
{
   S16     ret;
   TRC2(rgMUXAddPadd)

   if(*schdTbSz)
   {
      if(FALSE == isRar)
      {
         RG_PACK_PAD_SHDR(FALSE, sHdrBuf, ret);

         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_PADHDR_FAIL;
#ifdef MSPD
            MSPD_ERR("RGERR_MUX_BLD_PADHDR_FAIL\n");
#endif
            RETVALUE(RFAILED);
         }

         *schdTbSz -= 1;
      }

      if (*schdTbSz)
      {
         if (*schdTbSz <= RG_MAX_PAD_ARR_SZ)
         {
            RG_PACK_PAD(padArr, *schdTbSz, sduBuf, ret);

            if(ret != ROK)
            {
               err->errCause = RGERR_MUX_BLD_PAD_FAIL;
#ifdef MSPD
               MSPD_ERR("RGERR_MUX_BLD_PAD_FAIL\n");
#endif
               RETVALUE(RFAILED);
            }
            *schdTbSz = 0;
         }
         else
         {
            while (*schdTbSz)
            {
               if (*schdTbSz > RG_MAX_PAD_ARR_SZ)
               {
                  RG_PACK_PAD(padArr, RG_MAX_PAD_ARR_SZ, sduBuf, ret);

                  if(ret != ROK)
                  {
                     err->errCause = RGERR_MUX_BLD_PAD_FAIL;
#ifdef MSPD
                     MSPD_ERR("RGERR_MUX_BLD_PAD_FAIL\n");
#endif
                     RETVALUE(RFAILED);
                  }

                  *schdTbSz -= RG_MAX_PAD_ARR_SZ;
               }
               else
               {
                  RG_PACK_PAD(padArr, *schdTbSz, sduBuf, ret);

                  if(ret != ROK)
                  {
                     err->errCause = RGERR_MUX_BLD_PAD_FAIL;
#ifdef MSPD
                     MSPD_ERR("RGERR_MUX_BLD_PAD_FAIL\n");
#endif
                     RETVALUE(RFAILED);

                  }
                  *schdTbSz = 0;
               }
            }
         }
      }
   }

   RETVALUE(ROK);
} /* rgMUXAddPadd */

/**
 * @brief Function to add SDU along with sub headers.
 *
 * @details
 *
 *     Function : rgMUXAddSdus
 *     
 *  @param[in]       RgBldPduInfo   *pdu
 *  @param[out]      Buffer        *mBuf 
 *  @param[out]      Buffer        *sduBuf 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgMUXAddSdus
(
RgBldPduInfo   *pdu,
Buffer         *sHdrBuf,
Buffer         *sduBuf,
RgErrInfo      *err
)
#else
PRIVATE S16 rgMUXAddSdus(pdu, sHdrBuf, sduBuf, err)
RgBldPduInfo   *pdu;
Buffer         *sHdrBuf;
Buffer         *sduBuf;
RgErrInfo      *err;
#endif
{
   RgRguDedDatReq *dDatReq;
   RgRguCmnDatReq *cDatReq;

   TRC2(rgMUXAddSdus)

   switch(pdu->reqType)
   {
      case EVTRGUCDATREQ:
         cDatReq = (RgRguCmnDatReq *)(pdu->datReq);
         /* Add sdu(s) to the Message Buffer */
         if (NULLP != cDatReq)
         {
            if(rgMUXInsSdu(&pdu->schdTbSz, 
                     RG_CCCH_LCID, cDatReq->pdu, 
                     sHdrBuf, sduBuf, err) != ROK)
            {
               RETVALUE(RFAILED);
            }
            RG_FREE_MSG(cDatReq->pdu);
         }
         break;

      case EVTRGUDDATREQ:
         dDatReq = (RgRguDedDatReq *)(pdu->datReq);
         /* Add sdu(s) to the Message Buffer */
         if (NULLP != dDatReq)
         {
            if(pdu->tbIndex == 1)
            {
               U16 idx1, idx2;
               /* rg005.201 Adding this temporary variable for optimization */
               RguDatReqTb *datReqTb = &dDatReq->datReqTb[0];

               for (idx1=0; (idx1 < datReqTb->nmbLch); idx1++)
               {
                  for(idx2=0;
                     (idx2 < datReqTb->lchData[idx1].pdu.numPdu);
                     idx2++)
                  {
                     if(pdu->schdTbSz)
                     {
					 #ifdef PWAV
					 if(datReqTb->lchData[idx1].lcId == 1)
					 {
					 MSPD_DBG("Muxing for signalling message\n");
					 }
					 #endif
                        if(rgMUXInsSdu(&pdu->schdTbSz,
                           datReqTb->lchData[idx1].lcId, 
                           datReqTb->lchData[idx1].pdu.mBuf[idx2],
                           sHdrBuf, sduBuf, err) != ROK)
                        {
#ifdef MSPD
                           MSPD_ERR("Failed\n");
#endif
                           RETVALUE(RFAILED);
                        }
                     }
                     RG_FREE_MSG(datReqTb->lchData[idx1].pdu.mBuf[idx2]);
                  }
               }
            }
            else if(pdu->tbIndex == 2)
            {
               U16 idx1, idx2;
               /* rg005.201 Adding this temporary variable for optimization */
               RguDatReqTb *datReqTb = &dDatReq->datReqTb[1];
               for (idx1=0; (idx1 < datReqTb->nmbLch); idx1++)
               {
                  for(idx2=0;
                     (idx2 < datReqTb->lchData[idx1].pdu.numPdu);
                     idx2++)
                  {
                     if(pdu->schdTbSz)
                     {
                        if(rgMUXInsSdu(&pdu->schdTbSz,
                           datReqTb->lchData[idx1].lcId, 
                           datReqTb->lchData[idx1].pdu.mBuf[idx2],
                           sHdrBuf, sduBuf, err) != ROK)
                        {
#ifdef MSPD
                           MSPD_ERR("Failed TB Size %d\n", pdu->schdTbSz);
#endif
                           RETVALUE(RFAILED);
                        }
                     }
                     RG_FREE_MSG(datReqTb->lchData[idx1].pdu.mBuf[idx2]);
                  }
               }
            }
         }
         break;

      case EVTTFUTTIIND:
         break;
      default:
         break;
   } /* End of switch(reqType) */
   if(rgMUXAddPadd(&pdu->schdTbSz, sHdrBuf, sduBuf, FALSE, err) != ROK)
   {
#ifdef MSPD
      MSPD_ERR("Failed\n");
#endif
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 * @brief Function to create MAC PDU from RLC SDUs and control elements, if any. 
 *
 * @details
 *
 *     Function : rgMUXBldPdu
 *     
 *     -# This function shall be invoked by Downlink Harq Module as soon as a
 *        Data request is received from RLC for a UE along with its stored 
 *        control elements to create a MAC PDU.
 *     -# It shall create subheaders for the control elements (timing advance
 *        and contention resolution ID), if given, and then shall run through
 *        all the logical channels and create subheader for each of the SDUs
 *        given on that logical channel.
 *     -# It will concatenate the created subheaders, control elements and 
 *        the SDUs to form the entire MAC PDU.
 *     -# It shall invoke rgMUXPadPdu if the total length of the created 
 *        buffer is less than the scheduled TB size. 
 *     
 *           
 *  @param[in]  RgBldPduInfo   *bldPdu
 *  @param[in]  Buffer         **txPdu
 *  @param[out] RgErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgMUXBldPdu
(
RgBldPduInfo   *pdu,
Buffer         **txPdu,
RgErrInfo      *err
)
#else
PUBLIC S16 rgMUXBldPdu(pdu, txPdu, err)
RgBldPduInfo   *pdu;
Buffer         **txPdu;
RgErrInfo      *err;
#endif
{
   Buffer         *mBuf1=NULLP;
   Buffer         *mBuf2=NULLP;

   TRC2(rgMUXBldPdu)

   /* rg005.201 Moving the error type assignment under error cases only 
    * for optimization */
 
   if (rgGetMsg(&mBuf1) != ROK)
   {
      /* Buffer couldnt get allocated. Return a failure */
      err->errCause = RGERR_MUX_MEM_ALLOC_FAIL;
      err->errType = RGERR_MUX_BLD_PDU;
#ifdef MSPD
      MSPD_ERR("Failed\n");
#endif
      RETVALUE(RFAILED);
   }

   if (rgGetMsg(&mBuf2) != ROK)
   {
      /* Buffer couldnt get allocated. Return a failure */
      err->errCause = RGERR_MUX_MEM_ALLOC_FAIL;
      err->errType = RGERR_MUX_BLD_PDU;
#ifdef MSPD
      MSPD_ERR("Failed\n");
#endif
      RG_FREE_MSG(mBuf1); 
      RETVALUE(RFAILED);
   }

   if(rgMUXAddCes(pdu, mBuf1, mBuf2, err) != ROK)
   {
      RG_FREE_MSG(mBuf1);
      RG_FREE_MSG(mBuf2);
      err->errType = RGERR_MUX_BLD_PDU;
#ifdef MSPD
      MSPD_ERR("Failed\n");
#endif
      RETVALUE(RFAILED);
   }

   if(rgMUXAddSdus(pdu, mBuf1, mBuf2, err) != ROK)
   {
      RG_FREE_MSG(mBuf1);
      RG_FREE_MSG(mBuf2);
      err->errType = RGERR_MUX_BLD_PDU;
#ifdef MSPD
      MSPD_ERR("Failed\n");
#endif
      RETVALUE(RFAILED);
   }

   if(SCatMsg(mBuf1, mBuf2, M1M2) != ROK)
   {
      RG_FREE_MSG(mBuf1);
      RG_FREE_MSG(mBuf2);
      err->errType = RGERR_MUX_BLD_PDU;
#ifdef MSPD
      MSPD_ERR("Failed\n");
#endif
      RETVALUE(RFAILED);
   }
   RG_FREE_MSG(mBuf2); 

   *txPdu = mBuf1;

   RETVALUE(ROK);

}  /* rgMUXBldPdu */

/**
 * @brief Function to create RAR PDU. 
 *
 * @details
 *
 *     Function : rgMUXBldRarPdu
 *                This function is used to build RAR PDUs and is being 
 *                invoked by the scheduler.
 *     
 *  @param[out]RgCellCb       *cellCb
 *  @param[in] RgRaRspAlloc   *bldPdu
 *  @param[in] Buffer         **txPdu 
 *  @param[out] RgErrInfo     *err
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgMUXBldRarPdu
(
RgCellCb        *cell,
RgInfRaRntiInfo *alloc,
Buffer          **txPdu,
RgErrInfo       *err
)
#else
PUBLIC S16 rgMUXBldRarPdu(cell, alloc, txPdu, err)
RgCellCb        *cell;
RgInfRaRntiInfo *alloc;
Buffer          **txPdu;
RgErrInfo       *err;
#endif
{
   Buffer      *hdrBuf = NULLP;
   Buffer      *datBuf = NULLP;
   S16         ret; 
   U8          data[RG_RAR_ELEM_LEN];
   U8          hdrByte;
   TknU8       phdrByte;
   MsgLen      schdTbSz;
   U8          idx;

   TRC2(rgMUXBldRarPdu)

   schdTbSz = alloc->schdTbSz;
   /* RAR PDU Requirements */
   /*
   1. SubHeader - E/T/RAPID.
   2. TA ( if applicable)
   3. Ul Grant:
         a. Hopping Flag - 1 Bit.
         b. Fixed Size RB Assignment. - 10 Bits.
         c. Truncated Modulation and coding scheme - 4 Bits.
         d. TPC command for scheduled PUSCH. - 3 Bits.
         e. UL Delay - 1 Bit.
         f. CQI Request - 1 Bit.
   4. Temporary Crnti.
   */

   /* Initialize the error type */
   err->errType = RGERR_MUX_BLD_RAR_PDU;
  
   if ((ret = rgGetMsg(&hdrBuf)) != ROK)
   {
      /* Buffer couldnt get allocated. Return a failure */
      err->errCause = RGERR_MUX_MEM_ALLOC_FAIL;
      MSPD_ERR("Failed\n");
      RETVALUE(RFAILED);
   }
   if ((ret = rgGetMsg(&datBuf)) != ROK)
   {
      /* Buffer couldnt get allocated. Return a failure */
      err->errCause = RGERR_MUX_MEM_ALLOC_FAIL;
      RG_FREE_MSG(hdrBuf); 
      MSPD_ERR("Failed\n");
      RETVALUE(RFAILED);
   }

   if (TRUE == alloc->backOffInd.pres)
   {
      /*Set E and T*/
      if((schdTbSz > (RG_RAR_ELEM_LEN+RG_RAR_SHDR_LEN)) 
                           && (alloc->numCrnti))
      {
         hdrByte = 0x80;
      }
      else
      {
         hdrByte = 0x00;
      }
      /* Add the bi */
      hdrByte |= (0x0F & (alloc->backOffInd.val));

      /* Add the header */
      RG_PACK_RAR_SHDR(hdrByte, hdrBuf, ret);
      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_BI_FAIL;
         MSPD_ERR("RGERR_MUX_BLD_BI_FAIL\n");
         RG_FREE_MSG(hdrBuf);
         RG_FREE_MSG(datBuf);
         RETVALUE(RFAILED);
      }
      schdTbSz--;
   }

   phdrByte.pres = FALSE;

   for (idx=0; idx < (alloc->numCrnti) && 
         (schdTbSz >= RG_RAR_ELEM_LEN+RG_RAR_SHDR_LEN); idx++)
   {
      /* Add the tBit */
      hdrByte = 0x40; 
      /* Add the rapId */
      hdrByte |= (0x3F & (alloc->crntiInfo[idx].rapId));

      if(phdrByte.pres == TRUE)
      {
         /* Add the E bit*/
         phdrByte.val |= 0x80;
         /* Add the header */
         RG_PACK_RAR_SHDR(phdrByte.val, hdrBuf, ret);
         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_RAPIDHDR_FAIL;
            MSPD_ERR("RGERR_MUX_BLD_RAPIDHDR_FAIL\n");
            RG_FREE_MSG(hdrBuf);
            RG_FREE_MSG(datBuf);
            RETVALUE(RFAILED);
         }
      }

      phdrByte.pres = TRUE;
      phdrByte.val = hdrByte;
      /* Prepare the data */
      data[0]  =  0x7F & ((alloc->crntiInfo[idx].ta.val) >> 4);
      rgMUXGet20bitRarGrnt(cell->bwCfg.ulTotalBw, &(alloc->crntiInfo[idx].grnt), &data[1]);
      data[1] |=  ((U8)((alloc->crntiInfo[idx].ta.val) << 4));
      data[4]  =  (alloc->crntiInfo[idx].tmpCrnti) >> 8;
      data[5]  =  (U8) (alloc->crntiInfo[idx].tmpCrnti);

      if(SAddPstMsgMult(&data[0], RG_RAR_ELEM_LEN, datBuf) != ROK)
      {
         err->errCause = RGERR_MUX_BLD_RAPID_FAIL;
         MSPD_ERR("RGERR_MUX_BLD_RAPID_FAIL\n");
         RG_FREE_MSG(hdrBuf);
         RG_FREE_MSG(datBuf);
         RETVALUE(RFAILED);
      }
      schdTbSz -= RG_RAR_ELEM_LEN+RG_RAR_SHDR_LEN;
   }

   if(phdrByte.pres == TRUE)
   {
      /* Add the header */
      RG_PACK_RAR_SHDR(phdrByte.val, hdrBuf, ret);
      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_RAPIDHDR_FAIL;
	 MSPD_ERR("RGERR_MUX_BLD_RAPIDHDR_FAIL\n");
         RG_FREE_MSG(hdrBuf);
         RG_FREE_MSG(datBuf);
         RETVALUE(RFAILED);
      }
   }

   if(rgMUXAddPadd(&schdTbSz, hdrBuf, datBuf, TRUE, err) != ROK)
   {
      RG_FREE_MSG(hdrBuf);
      RG_FREE_MSG(datBuf);
      MSPD_ERR("Failed \n");
      RETVALUE(RFAILED);
   }

   /* Header and data Bufs have been created. Concatenate them. */
   if(SCatMsg(hdrBuf, datBuf, M1M2) != ROK)
   {
      RG_FREE_MSG(hdrBuf);
      RG_FREE_MSG(datBuf);
      MSPD_ERR("Failed \n");
      RETVALUE(RFAILED);
   }
   RG_FREE_MSG(datBuf);

   *txPdu = hdrBuf;

   RETVALUE(ROK);
} /* rgMUXBldRarPdu */

/***********************************************************
 *
 *     Func : rgMUXGet20bitRarGrnt
 *
 *     Desc : This function fills up the 20-bit grant
 *            for RA response.
 *
 *     Ret  : None.
 *
 *     Notes: None.
 *
 *     File : rg_mux.c
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgMUXGet20bitRarGrnt
(
U8             ulBw,
RgInfRarUlGrnt *msg3Grnt,
U8             *grnt
)
#else
PRIVATE Void rgMUXGet20bitRarGrnt(ulBw, msg3Grnt, grnt)
U8             ulBw;
RgInfRarUlGrnt *msg3Grnt;
U8             *grnt;
#endif
{
   U16       riv = rgMUXCalcRiv(ulBw, msg3Grnt->rbStart, msg3Grnt->numRb);

   TRC2(rgMUXGet20bitRarGrnt);

   grnt[2]  = msg3Grnt->cqiBit;   /* cqi bit is 0, output from sched */
   grnt[2] |= (msg3Grnt->delayBit << 1);
   grnt[2] |= (msg3Grnt->tpc << 2);
   grnt[2] |= (msg3Grnt->iMcsCrnt << 5);

   grnt[1]  = (msg3Grnt->iMcsCrnt >> 3);
   /* Forcing right shift to insert 0 as the LSB: 
    * since this is assumed in the computation */
   grnt[1] |= (U8)((riv << 1) & 0xFE);

   grnt[0]  = (U8)((riv >> 7) & 0x07);
   grnt[0] |= ((msg3Grnt->hop & 0x01) << 3);

   RETVOID;
} /* rgMUXGet20bitRarGrnt */

/***********************************************************
 *
 *     Func : rgMUXCalcRiv
 *
 *     Desc : This function calculates RIV.
 *
 *     Ret  : None.
 *
 *     Notes: None.
 *
 *     File : rg_mux.c
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U16 rgMUXCalcRiv
(
U8           bw,
U8           rbStart,
U8           numRb
)
#else
PRIVATE U16 rgMUXCalcRiv(bw, rbStart, numRb)
U8           bw;
U8           rbStart;
U8           numRb;
#endif
{
   U8           numRbMinus1 = numRb - 1;
   U16          riv;

   TRC2(rgMUXCalcRiv);

   if (numRbMinus1 <= bw/2)
   {
      riv = bw * numRbMinus1 + rbStart;
   }
   else
   {
      riv = bw * (bw - numRbMinus1) + (bw - rbStart - 1);
   }
   RETVALUE(riv);
} /* rgMUXCalcRiv */



/**********************************************************************
 
         End of file:     gk_mux.c@@/main/3 - Sat Jul 30 02:21:31 2011
 
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
/main/1      ---     ps                  1. Initial Release.
/main/2      ---     ps                  1. LTE MAC 2.1 release
           rg003.201 ns                  1. Trace added for functions.
           rg005.201 sm                  1. Changes for optimation.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
*********************************************************************91*/
