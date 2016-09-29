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

     Name:     LTE-RRC Layer 
  
     Type:     C Source file
  
     Desc:     Code for ASN.1 Encode Decode functions.

     File:     nh_edm.c

     Sid:      ni_edm.c@@/main/3 - Fri Jul  1 01:13:17 2011

     Prg:      dsahoo

**********************************************************************/

/** @defgroup edm_group Encode/Decode Module
 *  @{
 */
/* Header include files */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */

#include "cm_tkns.h"       /* Common Token defines */
#include "cm_mblk.h"       /* Common Mem Allocation defines */
#include "cm_llist.h"      /* Common Link list defines */
#include "cm_hash.h"       /* Common Hashlist defines */
#include "cm_lte.h"        /* Common Umts defines */
#include "cm_pasn.h"       /* Common Pasn defines */
#include "cm_lte.h"       /* Common LTE defines */

#include "nhu.h"           /* NHU Interface defines */
#include "lnh.h"           /* LNH Interface defines */

#include "nh.h"            /* RRC defines */


/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"           /* Timer services */
#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_mblk.x"       /* Common Memory Allocation */
#include "cm_llist.x"      /* Common Linklist definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_pasn.x"       /* Common Pasn definitions */
#include "cm_lte.x"        /* Common LTE definitions */
#include "cm_lib.x"        /* Common Library */

#include "nhu.x"           /* NHU Interface includes */
#include "lnh.x"           /* LNH Interface includes */

#include "nh.x"            /* RRC includes */
#include "nh_db.x"
#include "nh_3gdb.x"
#include "ss_diag.h"

/**
 * @brief Decodes the RRC message using the cm_pasn library.
 * 
 * @details 
 *   
 *
 * This function decodes the buffer received from the lower layer
 * into RRC message to be sent RRC user. 
 
 * @param[in] msgCategory  Message category(Ex:CCCH/DCCH)
 * @param[in] mBuf         Buffer to be decoded
 * @param[in] memCp        Memory control pointer
 * @param[in] pdu          Decoded PDU
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhEDMDecReq
(
U8      msgCategory,
Buffer  *mBuf,
Void    *memCp,
Void    *pdu
)
#else /* ANSI */
PUBLIC S16 nhEDMDecReq(msgCategory,mBuf,memCp,pdu)
U8        msgCategory;
Buffer    *mBuf;
Void      *memCp;
Void      *pdu;
#endif /* ANSI */
{
   S16            ret=ROK;
   U32            numDecOct; /* Number of octets decoded */
   U8             dbgFlag=NH_PASN_DBG_FLAG; /* Debug flag encode/decode library */
   CmPAsnErr      err;      /* PASN error code */
   Mem            mem;      /* Memory to allocate event structure*/
   CmPAsnElmDef   **elmDef=NULLP;

   TRC2(nhEDMDecReq)

   /* Initialize memCp */
   mem.region = nhCb.init.region;
   mem.pool   = nhCb.init.pool;

   /* Initialize PASN error code */
   err.errCode = 0;

   switch(msgCategory)
   {
      case NHU_MSG_ULDCCH:  /* UL DCCH Message */
         elmDef = nhUL_DCCH_Pdu;
         break;

      case NHU_MSG_ULCCCH:   /* UL CCCH Message */
         elmDef = nhUL_CCCH_Pdu;
         break;

      case NHU_DEC_SDUTYPE_EUTRA_UECAP_IE:
         elmDef = nhUE_EUTRA_Cap_Pdu;
         break;

      case NHU_DEC_SDUTYPE_UTRA_INTR_HOVR_MESSGE:
         elmDef = nhUTRANInterRATHovrInfo_Pdu;
         break;

      case NHU_DEC_SDUTYPE_UTRA_UECAP_IE:
         elmDef = nhUTRANUE_RadioAccessCapInfo_Pdu;
         break;

      case NHU_MSG_HOCMD:
         elmDef = nhHovrCmd_Pdu;
         break;

      case NHU_MSG_UERACAPINFO:
         elmDef = nhUERadioAccessCapInfo_Pdu;
         break;

      case NHU_MSG_UECAPINFO:
         elmDef = nhUECapInfo_Pdu;
         break;

      case NHU_MSG_HOPREPINFO:
         elmDef = nhHovrPrepInfo_Pdu;
         break;

      default:
         NH_DBG_ERROR((NH_PRNT_BUF,
                       "Invalid message category \n"));
         RETVALUE(RFAILED);
   }


   NH_DIAG_MSG(mBuf, "Received RRC Message :");
   /* Calling PASN library decode function */
   ret = cmPAsnDecMsgUalign ((Void*)pdu, CM_PASN_USE_MBUF, (Void **)&mBuf,
                            &numDecOct, 0, elmDef, CM_PASN_GEN_ERR, &err,
                            NH_PASN_MAX_BLOCK_SIZE, (CmMemListCp *)memCp,
                            dbgFlag);

   if (ROK != ret)
   {
      NH_DBG_ERROR((NH_PRNT_BUF,
                    "PASN Decoding Failed for msg category %d \n", 
                    msgCategory));
      nhUTLUpdGenSts((CntrType)NH_STS_CAUSE_DEC_ERR);
      RETVALUE(ret);
   }

   RETVALUE(ROK);

} /* nhEDMDecReq */

/**
 * @brief Encodes the RRC message in ASN.1 format.
 * 
 * @details 
 * 
 *
 * This function encodes the RRC message into the buffer 
 * to be returned to the caller. Uses CM_PASN library for encoding.
 *
 * @param[in]  pdu          PDU to be encoded
 * @param[in]  msgCategory  Message category
 * @param[out] mBuf         Encoded buffer
 * @param[out] length       Length of encoded buffer
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhEDMEncReq
(
Void     *pdu,
U8       msgCategory,
Buffer   **mBuf,
U32      *length
)
#else /* ANSI */
PUBLIC S16 nhEDMEncReq(pdu,msgCategory,mBuf,length)
Void     *pdu;
U8       msgCategory;
Buffer   **mBuf;
U32      *length;
#endif /* ANSI */
{
   S16       ret = ROK;
   Bool      dbgFlag = NH_PASN_DBG_FLAG;
   CmPAsnErr err;

   NhuDL_CCCH_Msg     *dlCcchPdu = NULLP;
   NhuDL_DCCH_Msg     *dlDcchPdu = NULLP;
   NhuPCCH_Msg        *pcchPdu = NULLP;
   NhuBCCH_BCH_Msg    *bcchBchPdu = NULLP;
   NhuBCCH_DL_SCH_Msg *bcchDlschPdu = NULLP;
   NhuHovrCmd         *hovrCmd = NULLP;
   NhuHovrPrepInfo    *hovrPrepInfo = NULLP;
   NhuUERadioAccessCapInfo *ueRadioAccCapInfo = NULLP;
   NhuUECapInfo       *ueCapInfo = NULLP;
#ifdef NHU_VER2
   NhuVarShortMAC_Input   *varShortMacI = NULLP;
#endif /* NHU_VER2 */

   TRC2(nhEDMEncReq)

   /* Allocate buffer */
   NH_ALLOCMBUF(NH_REG, NH_POOL, mBuf);

   if (NULLP == mBuf)
   {
      NH_DBG_FATAL((NH_PRNT_BUF, "Unable to Allocate Buffer \n"));
      RETVALUE(RFAILED);
   }

   switch(msgCategory)
   {
      case NHU_MSG_DLCCCH:  /* defined category in nhu.h */

         /* Get the DL CCCH PDU from PDU */
         dlCcchPdu = (NhuDL_CCCH_Msg *)pdu;
         ret = cmPAsnEncMsgUalign ((TknU8*)dlCcchPdu, CM_PASN_USE_MBUF, 
               (Void*)(*mBuf), 0, nhDL_CCCH_Pdu, &err, length, dbgFlag, FALSE);
         break;

      case NHU_MSG_DLDCCH:

         /* Get the DL DCCH PDU from PDU */
         dlDcchPdu = (NhuDL_DCCH_Msg *)pdu;
         ret = cmPAsnEncMsgUalign ((TknU8*)dlDcchPdu, CM_PASN_USE_MBUF, 
              (Void*)(*mBuf), 0, nhDL_DCCH_Pdu, &err, length, dbgFlag, FALSE);
         break;

      case NHU_MSG_DLPCCH:

         /* Get the DL PCCH PDU from PDU */
         pcchPdu = (NhuPCCH_Msg *)pdu;
         ret = cmPAsnEncMsgUalign ((TknU8*)pcchPdu, CM_PASN_USE_MBUF, 
             (Void*)(*mBuf), 0, nhPCCH_Pdu, &err, length, dbgFlag, FALSE);
         break;

      case NHU_MSG_DLBCCHBCH:

         /* Get the DL BCCH-BCH PDU from PDU */
         bcchBchPdu = (NhuBCCH_BCH_Msg *)pdu;
         ret = cmPAsnEncMsgUalign ((TknU8*)bcchBchPdu, CM_PASN_USE_MBUF, 
             (Void*)(*mBuf), 0, nhBCCH_BCH_Pdu, &err, length, dbgFlag, FALSE);
         break;

      case NHU_MSG_DLBCCHDLSCH:

         /* Get the DL BCCH-DLSCH PDU from PDU */
         bcchDlschPdu = (NhuBCCH_DL_SCH_Msg *)pdu;
         ret = cmPAsnEncMsgUalign ((TknU8*)bcchDlschPdu, CM_PASN_USE_MBUF,
            (Void*)(*mBuf), 0, nhBCCH_DL_SCH_Pdu, &err, length, dbgFlag, FALSE);
         break;

      case NHU_MSG_HOCMD:

         /* Get the Handover Command PDU from pdu */
         hovrCmd = (NhuHovrCmd *)pdu;
         ret = cmPAsnEncMsgUalign ((TknU8*)hovrCmd, CM_PASN_USE_MBUF,
            (Void*)(*mBuf), 0, nhHovrCmd_Pdu, &err, length, dbgFlag, FALSE);
         break;

      case NHU_MSG_UERACAPINFO:

         /* Get the UE Radio Access Capability Information PDU from pdu */
         ueRadioAccCapInfo = (NhuUERadioAccessCapInfo *)pdu;
         ret = cmPAsnEncMsgUalign ((TknU8*)ueRadioAccCapInfo, CM_PASN_USE_MBUF,
            (Void*)(*mBuf), 0, nhUERadioAccessCapInfo_Pdu, &err, length, 
            dbgFlag, FALSE);
         break;

      case NHU_MSG_UECAPINFO:

         /* Get the UE Capability Information PDU from pdu */
         ueCapInfo = (NhuUECapInfo *)pdu;
         ret = cmPAsnEncMsgUalign ((TknU8*)ueCapInfo, CM_PASN_USE_MBUF,
            (Void*)(*mBuf), 0, nhUECapInfo_Pdu, &err, length, dbgFlag, FALSE);
         break;

      case NHU_MSG_HOPREPINFO:

         /* Get the Handover Preparatin Information PDU from pdu */
         hovrPrepInfo = (NhuHovrPrepInfo *)pdu;
         ret = cmPAsnEncMsgUalign ((TknU8*)hovrPrepInfo, CM_PASN_USE_MBUF,
                                  (Void*)(*mBuf), 0, nhHovrPrepInfo_Pdu, 
                                  &err, length, dbgFlag, FALSE);
         break;

#ifdef NHU_VER2
      case NHU_MSG_VARSHORTMACI:

         varShortMacI = (NhuVarShortMAC_Input *)pdu;
         ret = cmPAsnEncMsgUalign ((TknU8*)varShortMacI, CM_PASN_USE_MBUF,
            (Void*)(*mBuf), 0, nhVarShortMAC_Input_Pdu, &err, length, dbgFlag, FALSE);
         break;
#endif /* LTERRC_REL9 */

      default:
         NH_DBG_ERROR((NH_PRNT_BUF,
                       "Invalid Message Category \n"));
         RETVALUE(RFAILED);
   }
   NH_DIAG_MSG(*mBuf, "Sending RRC Message :");

   if (ret != ROK)
   {
      NH_DBG_ERROR((NH_PRNT_BUF,
                    "PASN Encoding Failed for Message category %d \n",
                    msgCategory));

      NH_DIAG_LVL0(0x060d0001, NH_DIAG_NA, 0,
            "ASN Decoding failed, msgCategory = %d", msgCategory, 0,0,0);
      nhUTLUpdGenSts((CntrType)NH_STS_CAUSE_ENC_ERR);
      /* Free buffer, if any */
      NH_FREEMBUF(*mBuf);
      RETVALUE(ret);
   }

   RETVALUE(ROK);

} /* nhEDMEncReq */

/** @} */
/**********************************************************************
  
         End of file:     ni_edm.c@@/main/3 - Fri Jul  1 01:13:17 2011
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      rer              1. LTE-RRC Initial Release.
/main/2      ---      ch               1. RRC Release 2.1.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
*********************************************************************91*/

