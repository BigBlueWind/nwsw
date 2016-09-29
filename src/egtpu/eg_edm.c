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

     Name:     eGTP Encoder/Decoder & Msg validator

     Type:     C include file

     Desc:     Encoder/Decoder and Msg validation functions

     File:     eg_edm.c

     Sid:      pg_edm.c@@/main/3 - Fri Feb  4 18:18:15 2011

     Prg:      pchebolu

*********************************************************************21*/


/************************************************************************
 * H E A D E R     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment dependent */
  
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* common tokens */
#include "cm_hash.h"       /* common header 1 */
#include "cm_llist.h"      /* cm link list */
#include "cm5.h"           /* common header 3 */
#include "cm_err.h"        /* common error */
#include "cm_inet.h"       /* common tokens  */
#include "cm_tpt.h"        /* common transport */
#include "cm_dns.h"        /* common transport */
#include "leg.h"            /* eGTP Layer */
/*eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_psf.h"
#include "sht.h"
#endif
#include "eg.h"            /* eGTP Layer */
#include "eg_util.h"        /* eGTP layer */
#include "eg_edm.h"            /* eGTP Layer */
#include "eg_err.h"        /* eGTP Error Codes */

/************************************************************************
 * E X T E R N     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_tkns.x"       /* common tokens */
#include "cm_hash.x"       /* common structs 1 */
#include "cm_lib.x"        /* common library */
#include "cm_mblk.x"       /* common memory */
#include "cm_llist.x"      /* cm link list */
#include "cm5.x"           /* common header 3 */
#include "cm_inet.x"       /* common tokens  */
#include "cm_tpt.x"        /* common transport */
#include "cm_dns.x"        /* common transport */

#include "egt.x"           /* eGTP layer */
#include "leg.x"            /* eGTP layer */
#include "eg_edm.x"        /* eGTP layer */
/*eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#include "sht.x"
#endif
#include "eg.x"            /* eGTP layer */

#include "eg_tpt.x"            /* EGTP TPT module defines */
#include "eg_util.x"        /* eGTP layer */

#ifdef HW
#include "lhw.x"
#include "hw.x"          /* EGTPC UA                       */
#endif

#ifdef __cplusplus
EXTERN "C" {
#endif



#ifdef EGTP_U


/*******************************************************************
*        ENCODER            RELATED                  FUNCTIONS     *
*******************************************************************/
PRIVATE S16 egUEncodeGmHdr ARGS((EgUEvnt *evntMsg, Buffer *mBuf));

PRIVATE S16 egUEncIeData   ARGS((Buffer *mBuf, EgUIe *egIe));

PRIVATE S16 egUEncIe    ARGS((EgUIe *ie, Buffer *mBuf));

/*******************************************************************
*        ENCODER            RELATED                  FUNCTIONS     *
*******************************************************************/
PRIVATE S16 egUDecodeGmHdr ARGS((EgUEvnt *evntMsg, Buffer *mBuf));

PRIVATE S16 egUDecIeData ARGS((EgUEvnt *evntMsg, Buffer *mBuf, EgUIe *egIe, U16 ieLen));

PRIVATE S16 egUDecIe ARGS((EgUEvnt *evntMsg, Buffer *mBuf,Bool *iePres,U8 *ieType));

/*******************************************************************
*        VALIDATION         RELATED                  FUNCTIONS     *
*******************************************************************/

PRIVATE S16 egUValidateGmHdr ARGS((EgUEvnt *evntMsg));

PRIVATE S16 egUValidateGmMsgIEs ARGS((EgUEvnt *evntMsg, Bool *iePres));

PRIVATE S16 egUValidateIeLen ARGS((EgIeEntry *ieEntry, U8 msgType, U16 ieLen));

PRIVATE S16 egUGetCRType ARGS ((U8 extHdrType, U8 extHdrLen, Buffer *mBuf));

#endif /* End of EGTP_U */

#ifdef __cplusplus
}
#endif




#ifdef EGTP_U


/************************************************************************
 *   Fun:   egUEncodeGmHdr
 *   
 *   Desc:  This will Encode the GTP-U message Header.
 *   
 *   Ret:   ROK  - for Success
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 ***********************************************************************/
#ifdef ANSI
PRIVATE S16 egUEncodeGmHdr
(
   EgUEvnt    *evntMsg,   /* Event message having the MsgHdr to encode hdr */
   Buffer     *mBuf       /* Mbuf which holds the encoded buffer */
 )
#else
PRIVATE S16 egUEncodeGmHdr( evntMsg, mBuf)
   EgUEvnt    *evntMsg;   /* Event message having the MsgHdr to encode hdr */
   Buffer     *mBuf;      /* Mbuf which holds the encoded buffer */
#endif
{

   S16        retVal  = ROK;        /* Holds the return value */
   EgUMsgHdr  *header = NULLP;      /* Temp stores the header struct for enc */
   U8         tmpByte = 0;          /* stores one byte of data for enc */
   U8         hdr[EGU_MSG_MAX_HDR_LEN];      /* stores one byte of data for enc */
   U8         cnt     = EGU_MSG_MAX_HDR_LEN; /* Stores the position */
   MsgLen     msgLen  = 0;          /* Holds message length, in case of G-PDU */ 
   /* eg006.201: Flag indicating presence of any one Seq Num, Ext Hdr, or PDCP  --*/
   Bool       extPres = FALSE;      /* Flag for indication of S, E or P presense flag */

   EG_TRC2(egUEncodeGPduHdr)

   header = &(evntMsg->egMsg->msgHdr);

   /* Version */
   tmpByte |= EG_MASK_BIT6;

   /* Protocol Type */
   tmpByte |= EG_MASK_BIT5;

   /*************************************************
    * Encode the Extension Header Flag, if present  *
    *************************************************/
   if(header->extHdr.udpPort.pres || header->extHdr.pdcpNmb.pres)
   {
      tmpByte |=  EG_MASK_BIT3;
   }

   /**************************************************
    * Encode the Sequence Number Flag, if present    *
    **************************************************/
   if(header->seqNumber.pres)
   {
      tmpByte |=  EG_MASK_BIT2;
   }

   /**************************************************
    * Encode the N-PDU Number Flag, if present       *
    *************************************************/
   if(header->nPduNmb.pres)
   {
      tmpByte |=  EG_MASK_BIT1;
   }

   /*-- eg006.201: Set extPres Flag if either one of the Bit fields(S,P,E) is set. If this
    * flag is set, all the optional fields for Seq No, Pdcp & Ext Hdr will be encoded into mBuf */

   if((tmpByte & EG_MASK_BIT1) || (tmpByte & EG_MASK_BIT2)||(tmpByte & EG_MASK_BIT3))
   {
      extPres = TRUE;
   }

   /*****************************************************
    * Fill the header in the reversing order as finally *
    * we will be doing SAddPreMsgMult                   *
    *****************************************************/
   hdr[--cnt] = tmpByte;

   /*********************************************
    * Encode the Message Type                   *
    *********************************************/ 
   hdr[--cnt] = header->msgType;

   /*********************************************************
    * Encode the message length with zeroes                 *
    * At the end of the encoding all the IE's, the final    *
    * Length will be calculated and updated the mBuf        *
    ********************************************************/
   hdr[--cnt] = 0;
   hdr[--cnt] = 0;

   /**********************
    * Encode the Teid    *
    **********************/
   EG_GET_U32(header->teId, hdr, cnt);

   /*********************************************
    * Encode the sequence number,if present     *
    *********************************************/
   if(header->seqNumber.pres)
   {
      hdr[--cnt] = GetHiByte(header->seqNumber.val);
      hdr[--cnt] = GetLoByte(header->seqNumber.val);
   }

   /******************************************************************
    *   eg006.201 : If ExtPres is true, but Seq Num is not present, 
    * implies, either of E or P bit is set, so according to the spec 
    * the Byte fields will be padded with 0 for Seq Num */
   /****************************************************************
    * Spec says: This field is present if any of Seq Num, Ext Hdr  *
    * PN Present Flag is present, So we fill the field with Zeroes *
    * in case where Seq Num is not preset but other flags are set  *
    ****************************************************************/
   else if(extPres)
   {
      hdr[--cnt] = EG_ZERO;
      hdr[--cnt] = EG_ZERO;
   }

   /*********************************************
    * Encode the N-PDU Number, if present       *
    *********************************************/
   if(header->nPduNmb.pres)
   {
      hdr[--cnt] = header->nPduNmb.val;
   }

   /******************************************************************
    *   eg006.201 : If ExtPres is true, but Pdu Num is not present, 
    * implies, either of E or S bit is set, so according to the spec 
    * the Byte fields will be padded with 0 for Pdu Num */
   /****************************************************************
    * Spec says: This field is present if any of Seq Num, Ext Hdr  *
    * PN Present Flag is present, So we fill the field with Zeroes *
    * in case where Seq Num is not preset but other flags are set  *
    ****************************************************************/
   else  if(extPres)
   {
      hdr[--cnt] = EG_ZERO;
   }

   /*********************************************
    * Encode extension headers, if present      *
    *********************************************/
   if(header->extHdr.udpPort.pres)
   {
      hdr[--cnt] = EGU_EXT_HDR_UDP_TYPE;
      hdr[--cnt] = EG_ONE;
      hdr[--cnt] = GetHiByte(header->extHdr.udpPort.val);
      hdr[--cnt] = GetLoByte(header->extHdr.udpPort.val);
   }

   if(header->extHdr.pdcpNmb.pres)
   {
      hdr[--cnt] = EGU_EXT_HDR_PDCP_TYPE;
      hdr[--cnt] = EG_ONE;
      hdr[--cnt] = GetHiByte(header->extHdr.pdcpNmb.val);
      hdr[--cnt] = GetLoByte(header->extHdr.pdcpNmb.val);
   }

   if(tmpByte & EG_MASK_BIT3)
   {
      /****************************************************************
       * Add End of Extension Headers                                    *
       *******************************************************************/
      hdr[--cnt] = EG_ZERO;
   }

   /******************************************************************
    * eg006.201 : If ExtPres is true, but Ext hdr is not present, 
    * implies, either of P or S bit is set, so according to the spec 
    * the Byte fields will be padded with 0 for Pdu Num */
   /****************************************************************
    * Spec says: This field is present if any of Seq Num, Ext Hdr  *
    * PN Present Flag is present, So we fill the field with Zeroes *
    * in case where Seq Num is not preset but other flags are set  *
      ****************************************************************/
   else if(extPres)
   {
      hdr[--cnt] = EG_ZERO;
   }
    
   /***********************************************************
    * In case of G-PDU, calculate and update the final length *
    **********************************************************/
   if(header->msgType == EGT_GTPU_MSG_GPDU)
   {
      /***********************************************************
       * Calculate total Buffer length and decrement 8bytes, to  *
       * get the value tobe filled in the length field of the    *
       * message header                                          *
       ***********************************************************/
      msgLen = (evntMsg->tPduSize) + (EGU_MSG_MAX_HDR_LEN - cnt) - EG_LEN_EIGHT;

      /***********************************************
       * Fill the length field of the message header *
       ***********************************************/
      hdr[EGU_MSG_MAX_HDR_LEN - 3] = (U8)GetHiByte(msgLen);
      hdr[EGU_MSG_MAX_HDR_LEN - 4] = (U8)GetLoByte(msgLen);
   }

   /*****************************************
    * Add the header to the existing mBuf.  *
    *****************************************/
   /***************************************************************************
    * In case of non G-PDU messages, existing mBuf will be empty              *
    *                                                                         *
    * In case of G-PDUs existing mBuf will have T-PDU, i.e mBuf coming from   *
    * application as T-PDU                                                    *
    ***************************************************************************/
   SAddPreMsgMult(&hdr[cnt], (EGU_MSG_MAX_HDR_LEN - cnt), mBuf);
   EG_RETVALUE(retVal);

} /* End of egUEncodeGmHdr() */


/***********************************************************************
 *   Fun:  egUEncMsgReq
 *   
 *   Desc:  This will Encode the GTP-U message receieved from 
 *          the application
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c  
 **********************************************************************/
#ifdef ANSI
PUBLIC S16 egUEncMsgReq
(
   EgUEvnt *evntMsg        /* Event message having the Msg to encode msg */
)
#else
PUBLIC S16 egUEncMsgReq(evntMsg)
   EgUEvnt *evntMsg;       /* Event message having the Msg to encode msg */
#endif
{
   S16               retVal = ROK;         /* Holds the return value */
   U8                msgType = 0;          /* For storing the msg Type */
   U16               cnt    = 0;           /* For iterating thru Node Lists */
   /*eg009.201:Klockwork fix datatype modified*/
   U32               ieCnt  = 0;           /* For storing the nmb ie in list */
   U16               tmpCnt = 0;           /* For storing the nmb ie in list */
   Bool              expIe  = TRUE;        /* expected IE or NOT */
   Bool              iePres[EGU_MAX_IE]; /* Holds whether IE is present or not */
   Buffer            *mBuf  = NULLP;       /* Stores the Encoded Buffer */
   CmLList           *tmpNode = NULLP;     /* Stores the Ie Node */
   CmLList           *tmpList = NULLP;     /* Stores Ie List */
   EgUstaDgn         dgn;      /* diagnostics for sending alaram to LM */
   Mem               mem;      /* For allocating memory using reg, pool */
   MsgLen            encLen = 0;           /* Temp stores the encoded length */
   EgUIe             *tmpIe = NULLP;       /* Holds the IE NODE */
   EgIeEntry         *ieEntry = NULLP;     /* Holds IE Entry for each IE */
   EgUMsgEntry       *gmEntry = NULLP;     /* Holds the message Entry */
   MsgLen            tPduSize = 0;         /* T-PDU Size */
   U16               gmPosIdx = 0;         /* Message Poiton Idx */
   U16               iePosIdx = 0;         /* Ie Poiton Idx */
   U16               tempLen = 0;         /* tmp length */
#ifdef EG_FILE_LOG 
#ifdef _EG_PRINT_BUF_
   U16               nmbLines = 0;         /* No. of lines */
#endif
#endif

   EG_TRC2(egUEncMsgReq);

   /**************************************************
    * Check for event message being proper           *
    **************************************************/
   if (!evntMsg)
   {
      EG_DBG_INFO(0, 0, (egp,
               "egUEncMsgReq: evntMsg is NULLP \n"));
      EG_RETVALUE(EGT_ERR_MSG_UNABLE_TO_COMPLY);
   }

   /******************************************************
    * Check for eGTP message to be encoded being proper  *
    ******************************************************/ 
   if (!evntMsg->egMsg)
   {
      retVal = EGT_ERR_MSG_UNABLE_TO_COMPLY;
      EG_DBG_INFO(0, 0, (egp,
               "egUEncMsgReq: egMsg is NULLP \n"));

      /*************************************
       * Fill and send error event message *
       *************************************/ 
      EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0);
      EG_RETVALUE(retVal);
   }

   /*************************
    * Copy region and pool  *
    ************************/
   mem.region = evntMsg->region;
   mem.pool   = evntMsg->pool;

#ifdef _EG_PRINT_BUF_
   EgUUtilGmDump(evntMsg->egMsg
#ifdef EG_FILE_LOG
         , egCb.dbgFp, &nmbLines
#endif /* end of EG_FILE_LOG */
         );
#endif

   /*************************
    * Update Message Type   *
    *************************/
   msgType = evntMsg->egMsg->msgHdr.msgType;

   if(msgType == EGT_GTPU_MSG_GPDU)
   {
      /***************************** 
       * eg009.201 - Fix TEID zero *
       *****************************/
      /**************************************************************
       * Validate T-PDU size, based on configured max size of T-PDU *
       **************************************************************/
      SFndLenMsg(evntMsg->egMsg->u.mBuf, &tPduSize);
      if((evntMsg->maxTPduSize) && (tPduSize > evntMsg->maxTPduSize))
      {

         /*-- eg003.201: use format specifier according to datatype --*/
#ifdef LONG_MSG
         EG_DBG_INFO(0,0, (egp,
                  "egUEncMsgReq: T-PDU size: %ld exceeds the maximum configured value: %d \n",
                  tPduSize, evntMsg->maxTPduSize));
#else
         EG_DBG_INFO(0,0, (egp,
                  "egUEncMsgReq: T-PDU size: %hd exceeds the maximum configured value: %d \n",
                  tPduSize, evntMsg->maxTPduSize));
#endif

         retVal = EGT_ERR_MAX_TDPU_SIZE_EXCED;
         /****************************************************
          * Fill and send error event message                *
          ****************************************************/ 
         EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, msgType);
         EG_RETVALUE(retVal);
      }

      /*************************************
       * Update the T-PDU size to evntMsg  *
       *************************************/
      evntMsg->tPduSize = tPduSize;

      /*****************************
       * Encode the message header *
       ****************************/      
      egUEncodeGmHdr(evntMsg, evntMsg->egMsg->u.mBuf);

      /*****************************************
       * Update the buffer pointer to evntMsg  *
       *****************************************/
      evntMsg->mBuf = evntMsg->egMsg->u.mBuf;

      /**************************
       * Return Success         *
       **************************/
      evntMsg->resultType = EG_SUCCESS;
      evntMsg->egMsg->u.mBuf = NULLP;

      EgUUtilDeAllocGmMsg(&(evntMsg->egMsg));
      EG_RETVALUE(ROK);
   } /* End of G-PDU */

   /********************************************************
    * Echo Request / Response dont need any validation as  *
    * they are generated by GTP-U layer itself             *
    * ******************************************************/
   /*************************************************************
    * Error Indication message, Supported Extension header      *
    * Notification messages generated by GTP-U layer dont need  *
    * any validation                                            *
    * ***********************************************************/
   else if((msgType != EGT_GTPU_MSG_ECHO_REQ) && (msgType != EGT_GTPU_MSG_ECHO_RSP) &&
         !(msgType == EGT_GTPU_MSG_ERROR_IND && evntMsg->errInd == TRUE) && 
         !(msgType == EGT_GTPU_MSG_SUPP_EXT_HDR_NTF && evntMsg->suppExtHdrNtf == TRUE))
   {
      /***********************************
       * Initialize the iePres structure *
       ***********************************/
      cmMemset((Bool *)iePres, EG_ZERO, sizeof(iePres));

      evntMsg->isEnc = TRUE;

      /******************************************************
       * Validate the eGTP message header                   *
       ******************************************************/
      retVal = egUValidateGmHdr(evntMsg);   
      if(retVal != ROK)
      {
         EG_DBG_INFO(0,0, (egp,
                  "egUEncMsgReq: Message validation is failed \n"));
         /****************************************************
          * Fill and send error event message                *
          ****************************************************/ 
         EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, msgType);
         EG_RETVALUE(retVal);
      }

      /***************************************************
       *  Load the message entry to gmEntry              *
       ***************************************************/
      gmPosIdx = egCb.egUCb.edmUCb.egMsgCb.posIdx[msgType];

      gmEntry =  &(egCb.egUCb.edmUCb.egMsgCb.gmEntry[gmPosIdx]);


      CM_LLIST_FIRST_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
      ieCnt = evntMsg->egMsg->u.egIeList.count;

      /***************************************************
       * Itearate through the list of nodes in the egMsg *
       * for doing the validation of IEs                 *
       ***************************************************/
      for(cnt = 0; cnt < ieCnt; cnt++)
      {
         tmpIe = (EgUIe*)tmpNode->node;

         /********************************************
          * Get the IE entry from the IE Dictionary  *
          ********************************************/
         iePosIdx = egCb.egUCb.edmUCb.egIeCb.posIdx[tmpIe->ieType];
         if(iePosIdx)
         {
            ieEntry = &(egCb.egUCb.edmUCb.egIeCb.ieEntry[iePosIdx]);
         }
         else
         {
            /****************************************
             * Store the previous element details   *
             ****************************************/
            tmpList = tmpNode->prev;

            /***************************************
             * Delete the Unknown IE               *
             * *************************************/
            EgUUtilGmDelIe(evntMsg->egMsg, tmpIe->ieType, EG_ONE);
            EG_DBG_INFO(0,0, (egp, "egUEncIe(Deleted the Unknown IE(%d)\n",
                     tmpIe->ieType));

            /***************************
             * Resoring the crnt value *
             ***************************/
            evntMsg->egMsg->u.egIeList.crnt = tmpList;

            CM_LLIST_NEXT_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
            continue;
         }

         /***********************************************
          * Verify whether the ie Data type is matching *
          * with the expected datatype                  *
          * *********************************************/
         if ( (ieEntry) && (ieEntry->dataType != tmpIe->dataType))
         {
            /****************************************
             * Store the previous element details   *
             ****************************************/
            tmpList = tmpNode->prev;

            /***************************************
             * Delete IE with wrong DataType       *
             ***************************************/
            EgUUtilGmDelIe(evntMsg->egMsg, tmpIe->ieType, EG_ONE);
            EG_DBG_INFO(0,0, (egp, "egUEncIe(Deleted the IE(%d) with wrong datatype\n",
                     tmpIe->ieType));

            /***************************
             * Restoring the crnt value *
             ***************************/
            evntMsg->egMsg->u.egIeList.crnt = tmpList;

            CM_LLIST_NEXT_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
            continue;
         }

         /******************************************************
          * For IEs of type EG_IP_ADDR, address type should be *
          * Ipv4 or IPv6                                       *
          ******************************************************/
         if((tmpIe->dataType == EG_IP_ADDR) &&
               ((tmpIe->t.ipAddr.addrType != CM_TPTADDR_IPV4) && 
                (tmpIe->t.ipAddr.addrType != CM_TPTADDR_IPV6)))
         {
            /****************************************
             * Store the previous element details   *
             ****************************************/
            tmpList = tmpNode->prev;

            /***************************************
             * Delete IE with wrong Data           *
             ***************************************/
            EgUUtilGmDelIe(evntMsg->egMsg, tmpIe->ieType, EG_ONE);
            EG_DBG_INFO(0,0, (egp, "egUEncIe(Deleted the IE(%d) with wrong data\n",
                     tmpIe->ieType));

            /***************************
             * Restoring the crnt value *
             ***************************/
            evntMsg->egMsg->u.egIeList.crnt = tmpList;

            CM_LLIST_NEXT_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
            continue;
         }

         /******************************************************************
          * Store ie presence value in iePres, while avoiding duplicates. *
          * Duplicates are allowed for Private Extension IE                *
          ******************************************************************/
         if((iePres[tmpIe->ieType] == FALSE) || 
               (tmpIe->ieType == EGU_IE_PVT_EXTN))
         {
            iePres[tmpIe->ieType] = TRUE;
         }
         else
         {
            /****************************************
             * Store the previous element details   *
             ****************************************/
            tmpList = tmpNode->prev;

            /***************************************************
             * Case of repeated IE : Skip this IE and continue *
             * further processing                              *
             ***************************************************/
            EgUUtilGmDelIe(evntMsg->egMsg, tmpIe->ieType, EG_TWO);
            EG_DBG_INFO(0,0, (egp, "egUEncIe(Deleted the duplicate IE(%d)\n",
                     tmpIe->ieType));

            /***************************
             * Restoring the crnt value *
             ***************************/
            evntMsg->egMsg->u.egIeList.crnt = tmpList;

            CM_LLIST_NEXT_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
            continue;
         }

         /******************************************
          * Initializing expected IE to FALSE      *
          * ****************************************/
         expIe = FALSE;
         /*************************************************
          * Iterate through the message dict. to identify *
          * Unexpected IEs                                *
          *************************************************/ 
         for ( tmpCnt=0; tmpCnt < gmEntry->nmbIe; tmpCnt++)
         {
            if (gmEntry->ieProperties[tmpCnt].ieType == tmpIe->ieType)
            {
               expIe = TRUE;
               break;
            }   
         }  

         if(!expIe)
         {
            /****************************************
             * Store the previous element details   *
             ****************************************/
            tmpList = tmpNode->prev;


            /*************************************
             * Delete Unexpected IE              *
             *************************************/
            EgUUtilGmDelIe(evntMsg->egMsg, tmpIe->ieType, EG_ONE);
            EG_DBG_INFO(0,0, (egp, "egUEncIe(Deleted the Unexpected IE(%d)\n",
                     tmpIe->ieType));

            /***************************
             * Restoring the crnt value *
             ***************************/
            evntMsg->egMsg->u.egIeList.crnt = tmpList;

            CM_LLIST_NEXT_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
            continue;
         }
         CM_LLIST_NEXT_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
      }

      /*****************************************
       * Do more validation on the IE's        *
       *****************************************/
      retVal = egUValidateGmMsgIEs(evntMsg, iePres);
      if(retVal != ROK)
      {
         EG_DBG_INFO(0,0, (egp,
                  "egUEncMsgReq: IE validation is failed \n"));
         EG_RETVALUE(retVal);
      }
   }
   /*********************************************************
    * Validation is successful, do the encoding             * 
    * Allocate memory for the buffer                        *
    *********************************************************/ 
   retVal = SGetMsg(mem.region, mem.pool, &mBuf);
   if(retVal != ROK)
   {
      /********************************************************
       * Memory allocation failed, send alarm to LM and send  *
       * error event message                                  * 
       ********************************************************/ 
      cmMemset((U8*)&dgn, EG_ZERO, sizeof(EgUstaDgn));
      EGU_SND_ALARM_MEM_ALLOC_FAIL(mem.region, mem.pool, dgn);

      retVal = EGT_ERR_MEM_ALLOC_FAILED;
      EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, msgType); 
      EG_RETVALUE(retVal);
   }

   /*****************************
    * Encode the message header *
    ****************************/
   egUEncodeGmHdr(evntMsg, mBuf);

   /*********************************
    * Encode the IEs one by one     *
    *********************************/
   CM_LLIST_FIRST_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
   for(cnt = 0; cnt < evntMsg->egMsg->u.egIeList.count; cnt++)
   {
      /*****************************
       * Get the IE entry          *
       ****************************/
      tmpIe = (EgUIe*)tmpNode->node;

      /*****************************
       * Encode the IE             *
       ****************************/
      retVal = egUEncIe(tmpIe, mBuf);
      if(retVal != ROK)
      {
         EG_DBG_INFO(0,0, (egp,
                  "egUEncMsgReq: IE encoding is failed \n"));

         if(retVal == EGT_ERR_INV_IE_VAL)
         {
            EG_DBG_INFO(0,0, (egp, "egUEncIe(IE(%d), resultType(%d)) \n",
                     tmpIe->ieType,  retVal));
         }
         /**************************************
          * Fill and send error event message  * 
          * free buffer                        *
          **************************************/ 
         EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe->ieType, msgType); 
         SPutMsg(mBuf);
         EG_RETVALUE(retVal);
      }
      else
      {
         /*********************************
          * Get the next IE from the list *
          ********************************/
         CM_LLIST_NEXT_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
      }
   }

   /************************************************************
    * Calculate total Buffer length and decrement 8bytes, to   *
    * get the value tobe filled in the length field of the     *
    * message header                                           *
    ************************************************************/
   SFndLenMsg(mBuf, &encLen);
   encLen -= EG_LEN_EIGHT;

   /************************************************************
    * Fill the message length, length starts from index 2      *
    ************************************************************/
   tempLen = EG_LEN_TWO;
   EG_ENC_LEN(encLen, mBuf, tempLen);

   /**************************************
    * Free the event structure received  *       
    **************************************/
   if(msgType != EGT_GTPU_MSG_ECHO_REQ)
   {
      /* eg009.201: egMsg & memory allocated to IE inside egMsg
       * is freed with EgUUtilDeAllocGmMsg */
      EgUUtilDeAllocGmMsg(&(evntMsg->egMsg));      
   }
   /****************************************
    * Assign the mBuf in the event message *     
    ***************************************/
   evntMsg->mBuf = mBuf;

   /************************************************************
    * Send the message to main thread with SUCCESS result code *
    ************************************************************/
   evntMsg->resultType = EG_SUCCESS;

   EG_RETVALUE(retVal);
} /* End of egUEncMsgReq() */



/***********************************************************************
 *   Fun:  egUEncIeData
 *
 *   Desc:  Encode the GTP-U Msg IE Data part
 *
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *
 *   Notes: None
 *
 *   File:  eg_edm.c
 ***********************************************************************/
#ifdef ANSI
PRIVATE S16 egUEncIeData
(
   Buffer    *mBuf,                    /* Buffer to hold the Encoded msg */
   EgUIe     *egIe                     /* IE to encode the data portion */
 ) 
#else
PRIVATE S16 egUEncIeData(mBuf, egIe)
   Buffer    *mBuf;                    /* Buffer to hold the Encoded msg */
   EgUIe     *egIe;                    /* IE to encode the data portion */
#endif
{
   S16         retVal = ROK;           /* Holds the return value */

   EG_TRC2(egUEncIeData);

   EG_DBG_INFO(0,0, (egp,
            "egUEncIeData(IE Type(%d), IE Data Type(%d)) \n",
            egIe->ieType, egIe->dataType));

   /*********************************************
    * Decode the IE based on DataType of the IE *
    *********************************************/
   switch (egIe->dataType)
   {
      case EG_UNSIGNED_8:
         SAddPstMsg(egIe->t.valUnSgnd8, mBuf);
         break;

      case EG_UNSIGNED_32:
         EG_ENC_U32(egIe->t.valUnSgnd32, mBuf);
         break;

      case EG_PVT_EXTN:
         EG_ENC_U16(egIe->t.pvtExt.entId, mBuf);
         SAddPstMsgMult(egIe->t.pvtExt.propVal.val,
               egIe->t.pvtExt.propVal.length, mBuf);
         break;

      case EG_IP_ADDR:
         if(egIe->t.ipAddr.addrType == CM_TPTADDR_IPV4)
         {
            EG_ENC_U32(egIe->t.ipAddr.u.ipv4, mBuf);
         }
#ifdef EG_IPV6_SUPPORTED
         else if (egIe->t.ipAddr.addrType == CM_TPTADDR_IPV6)
         {
            SAddPstMsgMult(egIe->t.ipAddr.u.ipv6, EG_LEN_SIXTEEN, mBuf);
         }
#endif /* EG_IPV6_SUPPORTED */
         break;

      case EG_EXT_HDR_TYPE_LST:
         {
            U16 cnt = 0;
            if(egIe->t.extHdrTypeLst.noOfExtHdrs > EGT_U_MAX_EXT_TYPES)
            {
               retVal = EGT_ERR_INV_IE_VAL;
            }
            for(cnt = 0; cnt < egIe->t.extHdrTypeLst.noOfExtHdrs; cnt++)
               SAddPstMsg(egIe->t.extHdrTypeLst.extType[cnt], mBuf);
            break;
         }

#ifdef ERRCHK
         /********************************************************
          * Default is not required as datatype validation is    *
          * already done in egUEncMsgReq                         *
          ********************************************************/
      default:
         EG_DBG_INFO(0,0, (egp,
                  "egUEncIeData(Invalied IE Data Type: %d)\n", egIe->dataType));
         retVal = EGT_ERR_MSG_UNABLE_TO_COMPLY;
#endif
   }

   EG_RETVALUE(retVal);    
} /* End of egUEncIeData() */



/***********************************************************************
 *   Fun:  egUEncIe
 *   
 *   Desc:  Encodes the IE 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egUEncIe
(
   EgUIe   *ie,                          /* IE to encode */
   Buffer  *mBuf                         /* Buffer holds the encoded IE */
 )
#else
PRIVATE S16 egUEncIe(ie, mBuf)
   EgUIe   *ie;                          /* IE to encode */
   Buffer  *mBuf;                        /* Buffer holds the encoded IE */
#endif
{
   S16      retVal = ROK;                /* Holds the return value */
   MsgLen   initLen = 0;    /* Initial mBuf Length for Grp IE encoding */
   MsgLen   finLen = 0;     /* final mBuf Length for Grp IE encoding */
   MsgLen   ieLen = 0;                  /* actual length of IE being encoded */
   Data     dumBytes[EG_LEN_TWO];     /* Tmp stores the data for Enc */
   Data     dumBytes1[EG_LEN_ONE];     /* Tmp stores the data for Enc */
   EgIeEntry  *ieEntry = NULLP;     /* Holds IE Entry for the IE */
   U16         iePosIdx = 0;         /* Ie Poiton Idx */

   EG_TRC2(egUEncIe);

   cmMemset((U8*)dumBytes, EG_ZERO, EG_LEN_TWO);
   cmMemset((U8*)dumBytes1, EG_ZERO, EG_LEN_ONE);

   /*************************
    * Check for IE validity *
    ************************/
   if(ie == NULLP)
   {
      retVal = EGT_ERR_MSG_UNABLE_TO_COMPLY;
      EG_RETVALUE(retVal);
   }

   /*********************
    * Debug information *
    ********************/
   EG_DBG_INFO(0,0, (egp,
            "egUEncIe(IE Type(%d) \n",ie->ieType));

   /********************************************
    * Get the IE entry from the IE Dictionary  *
    ********************************************/
   iePosIdx = egCb.egUCb.edmUCb.egIeCb.posIdx[ie->ieType];
   ieEntry = &(egCb.egUCb.edmUCb.egIeCb.ieEntry[iePosIdx]);

   /***************************************
    * Store the index to encode IE length *
    ***************************************/
   SFndLenMsg(mBuf, &initLen);

   /**********************
    * Encode the IE Type *
    **********************/
   SAddPstMsg(ie->ieType, mBuf);

   /************************************************************
    * Encode dummy bytes in the length field, for TLV type IEs *
    ************************************************************/
   if(!ieEntry->tvType)
   {
      if(ie->ieType != EGU_IE_EXT_HDR_LST)
      {
         SAddPstMsgMult((U8*)dumBytes, EG_LEN_TWO, mBuf);
      }
      else
      {
         SAddPstMsgMult((U8*)dumBytes1, EG_LEN_ONE, mBuf);
      }
   }

   /**********************
    * Encode the IE data  *
    ***********************/ 
   retVal = egUEncIeData(mBuf, ie);
   if(retVal != ROK)
   {
      EG_DBG_INFO(0,0, (egp,
               "egUEncMsgReq: IE Data Encoding is failed \n"));
      EG_RETVALUE(retVal);
   }

   /*********************************************
    * Insert Correct IE length for TLV type IEs *
    *********************************************/
   if(!ieEntry->tvType)
   {
      /********************************************
       * Calculate final length of the buffer     *
       ********************************************/
      SFndLenMsg(mBuf, &finLen);

      /***********************************
       * Decrement 3bytes for the ie Hdr *
       ***********************************/
      if(ie->ieType != EGU_IE_EXT_HDR_LST)
      {
         ieLen = finLen - initLen - EG_LEN_THREE;
      }
      else
      {
         ieLen = finLen - initLen - EG_LEN_TWO;
      }

      /***********************************************************
       * Increment the index to point the actual length location *
       ***********************************************************/
      initLen += EG_IE_LEN_OFFSET;

      /********************************************
       * Encode the offset b/w finLen and initLen *
       ********************************************/
      if(ie->ieType != EGU_IE_EXT_HDR_LST)
      {
         EG_ENC_LEN(ieLen, mBuf, initLen);
      }
      else
      {
         SRepMsg(ieLen, mBuf, initLen);
      }
   } 

   EG_RETVALUE(retVal);
} /* End of egUEncIe() */



/***********************************************************************
 *   Fun:  egUDecMsgReq
 *   
 *   Desc:  Decodes the GTP-U Msg and then validate it.
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PUBLIC S16 egUDecMsgReq
(
   EgUEvnt  *evntMsg        /* Event message having the Msg to decode msg */
)
#else
PUBLIC S16 egUDecMsgReq( evntMsg)
   EgUEvnt  *evntMsg;       /* Event message having the Msg to decode msg */
#endif
{

   /***************************************************
    * Local variables required for non G-PDU message  *
    ***************************************************/

   S16            retVal  = ROK;       /* stores the return value */
   Buffer         *mBuf   = NULLP;     /* stores buffer present in event msg */
   Mem            mem;                 /* Memory used for allocating */
   EgUstaDgn      dgn;               /* diagnostics for sending alaram to LM */
   U16            cnt     = 0;         /* For iterating thru Node Lists */
   /*eg009.201:Klockwork fix Modified datatype*/
   U32            ieCnt   = 0;         /* Loop to iterate thru IE's */
   MsgLen         msgLen  = 0;         /* Stores Message len. after decoding */
   EgUMsgEntry    *gmEntry = NULLP;    /* Pointer to Message entry */
   U8             lastDecIeType = 0;   /* Last Decoded IE type */
   Bool           iePres[EGU_MAX_IE];    /* Holds IE Presence value */
   U8             msgType = 0;          /* Message Type */
   U8             ieType = 0;           /* Ie Type */
   U16            gmPosIdx = 0;         /* Message Position Idx */
   CmLList        *tmpNode = NULLP;     /* Stores the Ie Node */
   CmLList        *tmpList = NULLP;     /* Ie list */
   EgUIe          *tmpIe = NULLP;       /* tmp IE */
   Bool           expIe  = TRUE;        /* expected IE or NOT */
   U16            tmpCnt     = 0;         /* For iterating thru Node Lists */

#ifdef EG_FILE_LOG 
#ifdef _EG_PRINT_BUF_
   U16             nmbLines = 0;        /* number of Lines */ 
#endif
#endif 

   EG_TRC2(egUDecMsgReq);

   /* ccpu00125923: klocwork fix : removed check, NULL check already done in egUCpmHdlEvt */

   /*********************************************************
    * Initialize memory region and pool                     *
    *********************************************************/
   mem.region = evntMsg->region;
   mem.pool   = evntMsg->pool;

	/* ccpu00125923: klocwork fix : removed check, NULL check already done in EgActvTsk */

   /*********************************************************
    * Assign the buffer to the local pointer and clear the  *
    * event structure buffer                                *
    *********************************************************/
   mBuf = evntMsg->mBuf;
   evntMsg->mBuf = NULLP;

   /*********************************************************
    * Allocate the memory for the event structure           *
    *********************************************************/
   if(cmAllocEvnt(sizeof(EgUMsg), EGU_MEM_SDU_SIZE, 
            &mem, (Ptr*)&evntMsg->egMsg) != ROK)
   {
      EG_DBG_INFO(0, 0, (egp,
               "egUDecMsgReq: Memory allocation failed for egMsg \n"));

      /******************************************************
       * Fill and send error event message                  *
       * Rree memory associated with mBuf    .              * 
       ******************************************************/ 
      cmMemset((U8*)&dgn, EG_ZERO, sizeof(EgUstaDgn));
      EGU_SND_ALARM_MEM_ALLOC_FAIL(mem.region, mem.pool, dgn);
      SPutMsg(mBuf);
      /* ccpu00125923: klocwork fix : removed since returning here */
      EG_RETVALUE(EGT_ERR_MSG_UNABLE_TO_COMPLY);
   }

   /**************************************************
    * Decode the GTP-U message header                *
    **************************************************/
   retVal = egUDecodeGmHdr(evntMsg, mBuf);
   if(retVal != ROK)
   {
      EG_DBG_INFO(0,0, (egp,
               "egUDecMsgReq: Message header decoding is failed \n"));
      /*******************
       * free the memory *
       ******************/
      SPutMsg(mBuf);
      /* ccpu00125923: klocwork fix : removed since returning here */
      evntMsg->resultType = retVal;
      EG_RETVALUE(retVal);
   }

   /**************************************************
    *  Decode the IEs, if message type is not GPDU   *
    **************************************************/
   if(evntMsg->egMsg->msgHdr.msgType != EGT_GTPU_MSG_GPDU)
   {
      /**************************************************
       * Initialize iePres array                        *
       **************************************************/
      cmMemset((Bool *)iePres, EG_ZERO, sizeof(iePres));


      msgType = evntMsg->egMsg->msgHdr.msgType;

      /**********************************************
       *  Loop through to Decode the GTP IE's       *
       **********************************************/
      SFndLenMsg(mBuf, &msgLen);
      while (msgLen)
      {
         /************************
          *  Decode the IE       *
          ************************/
         retVal = egUDecIe(evntMsg, mBuf, iePres, &ieType);
         /*******************************************
          * If decoding of IE fails then free the   *
          * the message buffer and retrun from here *
          *******************************************/
         /* ccpu00125923: klocwork fix: combining both the error checks together */
			/* Compromise for performance in case of error is OK */
         if ((retVal != ROK) || (ieType < lastDecIeType))
         {
            if (retVal != ROK)
            {
               EG_DBG_INFO(0,0, (egp,
                     "egUDecMsgReq: IE decoding is failed \n"));
            }
				/****************************************************************
				* If decodded IE type is less than the last decoded IE type,    *
				* i.e IEs are not received in ascending order, free mBuf & ret  *
				****************************************************************/
				else
				{
					retVal = EGT_ERR_IE_NOT_ASCD_ORDER;
				}
            SPutMsg(mBuf);
				evntMsg->resultType = retVal;
            EG_RETVALUE(retVal);
        }
		/* ccpu00125923:klocwork fix: removed check here */

         /***************************************
          * Update the last decoded IE type     *
          ***************************************/
         lastDecIeType = ieType;

         SFndLenMsg(mBuf, &msgLen);
      } 

      gmPosIdx = egCb.egUCb.edmUCb.egMsgCb.posIdx[msgType];
      gmEntry =  &egCb.egUCb.edmUCb.egMsgCb.gmEntry[gmPosIdx];

      /********************************************************************
       * Itearate through the list of IEs in egMsg, while comparing with  *
       * the Message dict. to find out Unexpected IEs                     *
       ********************************************************************/
      CM_LLIST_FIRST_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
      ieCnt = evntMsg->egMsg->u.egIeList.count;
      for(cnt = 0; cnt < ieCnt; cnt++)
      {
         tmpIe = (EgUIe*)tmpNode->node;
         expIe = FALSE;
         for ( tmpCnt=0; tmpCnt < gmEntry->nmbIe; tmpCnt++)
         {
            if (gmEntry->ieProperties[tmpCnt].ieType == tmpIe->ieType)
            {
               /********************
                * Expected IE      *
                ********************/
               expIe = TRUE;
               break;
            }
         }
         if(!expIe)
         {
            /****************************************
             * Store the previous element details   *
             ****************************************/
            tmpList = tmpNode->prev;

            /*******************************
             * Delete the unexpected IEs   *
             *******************************/
            EgUUtilGmDelIe(evntMsg->egMsg, tmpIe->ieType, EG_ONE);
            EG_DBG_INFO(0,0, (egp, "egUDecIe(Deleted the unexpected IE(%d)\n",
                     tmpIe->ieType));

            /***************************
             * Resoring the crnt value *
             ***************************/
            evntMsg->egMsg->u.egIeList.crnt = tmpList;
         }

         CM_LLIST_NEXT_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
      }

      if(gmEntry->nmbMandIe)
      {
         /************************************************************
          * Validate the IE's, if the message has some mandatory IEs *
          ************************************************************/
         retVal = egUValidateGmMsgIEs(evntMsg, iePres);
         if(retVal != ROK)
         {
            EG_DBG_INFO(0,0, (egp,
                     "egUDecMsgReq: Message validation is failed \n"));
            /******************************************
             * Fill and send error event message and  *
             * free memory associated with mBuf.      * 
             ******************************************/ 
            SPutMsg(mBuf);   
            /* ccpu00125923: klocwork fix : removed since returning here */
            evntMsg->resultType = retVal;
            EG_RETVALUE(retVal);
         }
      }

      /****************************************************************
       * Decoding is successful, free the memory associated with mBuf *
       ****************************************************************/
      SPutMsg(mBuf);
      /* ccpu00125923: klocwork fix : removed since returning after this code */
   }
   else
   {
      /****************************************************
       * Fill egMsg with the remaining buffer, for GPDUs  *
       ***************************************************/
      evntMsg->egMsg->u.mBuf = mBuf;
   }

   /**********************
    * Return as SUCCESS  *
    **********************/
   evntMsg->resultType = EG_SUCCESS;

#ifdef _EG_PRINT_BUF_
   EgUUtilGmDump(evntMsg->egMsg
#ifdef EG_FILE_LOG
         , egCb.dbgFp, &nmbLines
#endif /* end of EG_FILE_LOG */
         );
#endif /* end of _EG_PRINT_BUF_ */

   EG_RETVALUE(retVal);
} /* End of egUDecMsgReq() */



/***********************************************************************
 *   Fun:  egUDecodeGmHdr
 *   
 *   Desc:  Decodes the GTP-U Msg Header
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egUDecodeGmHdr
(
   EgUEvnt    *evntMsg,         /* Event message stores the decoded hdr */
   Buffer     *mBuf             /* Buffer to decode the header */
 )
#else
PRIVATE S16 egUDecodeGmHdr(evntMsg, mBuf)
   EgUEvnt     *evntMsg;         /* Event message stores the decoded hdr */
   Buffer      *mBuf;            /* Buffer to decode the header */
#endif
{
   S16      retVal  = ROK;       /* Holds the return value */
   U8       tmpByte = 0;         /* Holds one byte of data after Dec */
   U8       version = 0;         /* Holds the version type, decoded */
   MsgLen   msgLen  = 0;         /* Holds the msgLen from the Hdr */
   MsgLen   bufLen  = 0;         /* Holds the total buffer length */
   U8      extHdrType = 0;       /* Holds the Extension hdr type */
   U8      extHdrLen = 0;        /* Extension hdr length */
   Bool    unKnownExtHdr = 0;    /* Is Unknown Extension Hdr? */
   /* eg006.201: Flag indicating presence of any one Seq Num, Ext Hdr, or PDCP  --*/
   Bool       extPres = FALSE;      /* Flag for indication of S, E or P presense flag */

   EG_TRC2(egUDecodeGmHdr);


   /*********************************************
    * Check the length of buffer to be decoded  *
    *********************************************/
   SFndLenMsg(mBuf, &bufLen);

#ifdef LONG_MSG   
#ifndef ALIGN_64BIT
   EG_DBG_INFO(0,0, (egp,
            "egUDecodeGmHdr(MsgLen(%ld)) \n", bufLen));
#else            
   EG_DBG_INFO(0,0, (egp,
            "egUDecodeGmHdr(MsgLen(%d)) \n", bufLen));
#endif
#endif

   /*************************************************
    * Store the first byte in tmpByte, Right shift  *
    * 5 times to get version                        *
    *************************************************/
   SRemPreMsg(&tmpByte, mBuf);
   version = tmpByte >> EG_SHIFT_5;

   /*********************************************************
    * Check if the version is 1, if not report INV version  *
    *********************************************************/
   if(version != EGU_MSG_VERSION)
   {
      retVal = EGT_ERR_MSG_INV_VERSION;
      EG_DBG_INFO(0,0, (egp,
               "egUDecodeGmHdr(Invalid Msg Version(%d), \
         resultType(%d) \n",version, retVal));

      EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0);
      EG_RETVALUE(retVal);
   }

   /**************************************************************
    * Decode the Protocol Type, and if it is not 1, return error *
    **************************************************************/
   if (!(tmpByte & EG_MASK_BIT5))
   {
      retVal = EGT_ERR_MSG_INV_PTTYPE;
      EG_DBG_INFO(0,0, (egp,
               "egUDecodeGmHdr(Invalid Protocol Type(%d), \
         resultType(%d) \n",0, retVal));

      EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0);
      EG_RETVALUE(retVal);
   }

   /******************************
    * decode the message Type    *
    ******************************/
   SRemPreMsg((Data*)&(evntMsg->egMsg->msgHdr.msgType), mBuf);

   /******************************
    * decode the message length  *
    *****************************/
   EG_DEC_U16(msgLen, mBuf);


   /*-- eg006.201: Aligning Decoder code to spec --*/
   if((tmpByte & EG_MASK_BIT1) || (tmpByte & EG_MASK_BIT2)||(tmpByte & EG_MASK_BIT3))
   {
      extPres = TRUE;
   }

   /*******************************************************************
    * The message length should not have the first 8 mandatory octets *
    * Check whether the message lenght is 8 octets less than the      *
    * total buffer length                                             *
    ******************************************************************/
   if(msgLen != bufLen - EG_LEN_EIGHT)
   {
      retVal = EGT_ERR_MSG_INV_LEN;
#ifdef LONG_MSG   
#ifndef ALIGN_64BIT
      EG_DBG_INFO(0,0, (egp,
               "egUDecodeGmHdr(Decoded MsgLen(%ld), actual MsgLen(%ld), \
         resultType(%d)) -- Invalid Length \n", 
               msgLen, bufLen, retVal));
#else            
      EG_DBG_INFO(0,0, (egp,
               "egUDecodeGmHdr(Decoded MsgLen(%d), actual MsgLen(%d), \
         resultType(%d)) -- Invalid Length \n", 
               msgLen, bufLen, retVal));
#endif            
#endif            
      EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0,
            evntMsg->egMsg->msgHdr.msgType);
      EG_RETVALUE(retVal);
   }

   EG_DEC_U32(evntMsg->egMsg->msgHdr.teId, mBuf);

   /***********************************************
    * Decode the Sequence Number, if S-bit is set *
    ***********************************************/
   if ( tmpByte & EG_MASK_BIT2 )
   {
      evntMsg->egMsg->msgHdr.seqNumber.pres = PRSNT_NODEF;
      EG_DEC_U16(evntMsg->egMsg->msgHdr.seqNumber.val, mBuf);
   }

   /******************************************************************
    * eg006.201 : If ExtPres is true, but Seq NUm bit is not set, 
    * implies, either of P or S bit was set during Encode, so accordingly
    * extract Byte fields from mBuf will be extracted with 0 for Seq Num
   ******************************************************************/
   else  if(extPres)
   {
      evntMsg->egMsg->msgHdr.seqNumber.pres = 0;
      EG_DEC_U16(evntMsg->egMsg->msgHdr.seqNumber.val, mBuf);
   }

   /*********************************************
    * Decode the N-PDU Number, if PN-bit is set *
    *********************************************/
   if ( tmpByte & EG_MASK_BIT1 )
   {
      evntMsg->egMsg->msgHdr.nPduNmb.pres = PRSNT_NODEF;
      SRemPreMsg(&(evntMsg->egMsg->msgHdr.nPduNmb.val), mBuf);
   }
   /******************************************************************
    * eg006.201 : If ExtPres is true, but Pdu NUm bit is not set, 
    * implies, either of E or S bit was set during Encode  so accordingly 
    * extract  Byte fields will be extracted form Mbuf with 0 for Pdu Num
   ******************************************************************/
   else  if(extPres)
   {
      evntMsg->egMsg->msgHdr.nPduNmb.pres = 0;
      SRemPreMsg(&(evntMsg->egMsg->msgHdr.nPduNmb.val), mBuf);
   }

   /************************************************
    * Decode the Extension Header, if E-bit is set *
    ************************************************/
   if(tmpByte & EG_MASK_BIT3)
   {
      SRemPreMsg(&extHdrType, mBuf);
      while(extHdrType)
      {
         retVal = ROK;
         unKnownExtHdr = FALSE;

         switch (extHdrType)
         {
            /*********************************************
             * Decode Extension Header of type UDP Port  *
             *********************************************/
            case EGU_EXT_HDR_UDP_TYPE:
               SRemPreMsg(&extHdrLen, mBuf);
               if(extHdrLen == EG_LEN_ONE)
               {
                  evntMsg->egMsg->msgHdr.extHdr.udpPort.pres = PRSNT_NODEF;
                  EG_DEC_U16(evntMsg->egMsg->msgHdr.extHdr.udpPort.val, mBuf);
               }
               else
               {
                  /********************************************************
                   * If the Ext. Header length is wrong, get the CR type  *
                   ********************************************************/
                  retVal = egUGetCRType(extHdrType, extHdrLen, mBuf);
                  EG_DBG_INFO(0,0, (egp,
                           "egUDecodeGmHdr(Received Ext. Hdr :%d with wrong \
                     length : %d -- Actual length :%d\n", 
                           extHdrType, extHdrLen, EG_LEN_ONE));
               }
               break;

               /****************************************************
                * Decode Extension Header of type PDCP PDU Number  *
                ****************************************************/
            case EGU_EXT_HDR_PDCP_TYPE:
               SRemPreMsg(&extHdrLen, mBuf);
               if(extHdrLen == EG_LEN_ONE)
               {
                  evntMsg->egMsg->msgHdr.extHdr.pdcpNmb.pres = PRSNT_NODEF;
                  EG_DEC_U16(evntMsg->egMsg->msgHdr.extHdr.pdcpNmb.val, mBuf);
               }
               else
               {
                  /********************************************************
                   * If the Ext. Header length is wrong, get the CR type  *
                   ********************************************************/
                  retVal = egUGetCRType(extHdrType, extHdrLen, mBuf);
                  EG_DBG_INFO(0,0, (egp,
                           "egUDecodeGmHdr(Received Ext. Hdr :%d with wrong \
                     length : %d -- Actual length :%d\n", 
                           extHdrType, extHdrLen, EG_LEN_ONE));
               }
               break;

            default:
               /**********************************************************
                * If the Ext. Header length is Unknown, get the CR type  *
                **********************************************************/
               SRemPreMsg(&extHdrLen, mBuf);
               retVal = egUGetCRType(extHdrType, extHdrLen, mBuf);
               unKnownExtHdr = TRUE;
         }

         if((retVal == ROK || retVal == EG_CH_NOT_REQ) && (unKnownExtHdr == FALSE))
         {
            /**************************
             * Get Next Ext. Hdr Type *
             **************************/
            SRemPreMsg(&extHdrType, mBuf);
            retVal = ROK;
         }

         else if(retVal == EG_CH_REQ )
         {
            if(unKnownExtHdr == TRUE)
            {
               EG_DBG_INFO(0,0, (egp,
                        "egUDecodeGmHdr(Received UnSupported Ext. Hdr :%d with \
                  Comprehension Required\n", extHdrType));

               /***************************************************************
                * If sequence number is present, update in the evntMsg,       *
                * so that same can be used for sending Supported Extension    *
                * Header Notification message                                 *
                * *************************************************************/
               if(evntMsg->egMsg->msgHdr.seqNumber.pres)
               {
                  evntMsg->seqNumber = evntMsg->egMsg->msgHdr.seqNumber.val;
               }
               else
               {
                  evntMsg->seqNumber = 0;
               }

               retVal = EGT_ERR_UNSUPP_EXT_HDR;
					/* ccpu00125923: klocwork fix, removed return */
				   break; /* while loop */
            }
            else
            {
               EG_DBG_INFO(0,0, (egp,
                        "egUDecodeGmHdr(Received Supported Ext. Hdr :%d with \
                  Comprehension Required with Wrong length field\n",
                        extHdrType));
               retVal = EGT_ERR_WRONG_EXT_HDR;/* klocwork fix: removed return */
			   break; /* while loop */
            }
         }
         else if ((retVal == EG_CH_NOT_REQ) && (unKnownExtHdr == TRUE))
         {
            /**************************
             * Get Next Ext. Hdr Type *
             **************************/
            SRemPreMsg(&extHdrType, mBuf);
            retVal = ROK;
         }
      }
   }
   /******************************************************************
    * eg006.201 : If ExtPres is true, but Pdu NUm bit is not set, 
    * implies, either of E or S bit was set during Encode  so accordingly 
    * extract  Byte fields will be extracted form Mbuf with 0 for Ext Hdr 
   ******************************************************************/
   else  if(extPres)
   {
      SRemPreMsg(&extHdrType, mBuf);
   }
   /******************************************************************
    * check for msgLen error, if no error continue, otherwise return *
    ******************************************************************/
   if((retVal == ROK) && (evntMsg->egMsg->msgHdr.msgType != EGT_GTPU_MSG_GPDU))
   {
      /***************************************
       * Call to validate the GTP Msg Header *
       **************************************/
      retVal = egUValidateGmHdr(evntMsg);
   }

   EG_RETVALUE(retVal); 
} /* End of egUDecodeGmHdr() */ 



/***********************************************************************
 *   Fun:  egUValidateGmHdr
 *   
 *   Desc:  To Validate GTP-U Msg header as per specs.
 *   
 *   Ret:   ROK  - for Success
 *          EGT_ERR_INV_HDR -   Invalid GTP Header
 *          EGT_ERR_UNSUPP_VER - VERSION not supported
 *          EGT_ERR_MSG_INV_TYPE - Invalid Message Type
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egUValidateGmHdr
(
   EgUEvnt  *evntMsg                      /* Event message to validate */
 )
#else
PRIVATE S16 egUValidateGmHdr( evntMsg)
   EgUEvnt  *evntMsg;                     /* Event message to validate */
#endif
{
   S16           retValue = ROK;          /* Holds the return value */
   EgUMsgHdr     *hdr = NULLP;            /* Holds the Message Hdr */
   U16           gmPosIdx = 0;            /* Message Position Idx */

   /*************************************************************
    *  Trace the function only if debug mask is enabled for EDM *
    *************************************************************/
   EG_TRC2(egUValidateGmHdr)

      hdr = &evntMsg->egMsg->msgHdr;

   EG_DBG_INFO(0,0, (egp,
            "egUValidateGmHdr( Message Type (%d) \n",
            hdr->msgType));

   gmPosIdx = egCb.egUCb.edmUCb.egMsgCb.posIdx[hdr->msgType];
   if(!gmPosIdx)
   {

      /*******************************************
       * Return Error for Unknown Message Type   *
       *******************************************/
      retValue = EGT_ERR_MSG_INV_TYPE;
      EG_DBG_INFO(0,0, (egp,
               "egUValidateGmHdr(Invalid Message Type(%d) resultType(%d) \n",
               hdr->msgType, retValue));
      EGU_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 
            evntMsg->egMsg->msgHdr.msgType);
      EG_RETVALUE(retValue);
   }

   /***********************************************************
    * TEID should be Zero for the following message types *
    ***********************************************************/
   if(((hdr->msgType == EGT_GTPU_MSG_ECHO_REQ) || 
            (hdr->msgType == EGT_GTPU_MSG_ECHO_RSP) || 
            (hdr->msgType == EGT_GTPU_MSG_SUPP_EXT_HDR_NTF) || 
            (hdr->msgType == EGT_GTPU_MSG_ERROR_IND)) && (hdr->teId))
   {
      /*************************************
       * Return Error if TEID is non-ZERO  *
       *************************************/
      retValue = EGT_ERR_INV_TEID;
#ifdef ALIGN_64BIT
      EG_DBG_INFO(0,0, (egp,
               "egUValidateGmHdr(Invalid teid(%d) for this message \n", hdr->teId));
#else
      EG_DBG_INFO(0,0, (egp,
               "egUValidateGmHdr(Invalid teid(%ld) for this message\n", hdr->teId));
#endif

      EGU_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 
            evntMsg->egMsg->msgHdr.msgType);
      EG_RETVALUE(retValue);

   }

   /*************************************
    * Encoding side Validation          *
    *************************************/
   if(evntMsg->isEnc)
   {
      /******************************************************************
       * TEID should be non-Zero for the following message types.       *
       * On Decoding side the same validation will be taken care by CPM *
       ******************************************************************/
      if((hdr->msgType == EGT_GTPU_MSG_END_MARKER) && (!hdr->teId))
      {
         /*********************************
          * Return Error if TEID is ZERO  *
          *********************************/
         retValue = EGT_ERR_INV_TEID;
#ifdef ALIGN_64BIT
         EG_DBG_INFO(0,0, (egp,
                  "egUValidateGmHdr(Invalid teid(%d) for this message \n", hdr->teId));
#else
         EG_DBG_INFO(0,0, (egp,
                  "egUValidateGmHdr(Invalid teid(%ld) for this message\n", hdr->teId));
#endif

         EGU_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 
               evntMsg->egMsg->msgHdr.msgType);
         EG_RETVALUE(retValue);
      }

      /*********************************************************
       * Sequence Number should be present for these messages  *
       *********************************************************/
      /********************************************************************
       * Sequence Number should be present for Echo also, but the control * 
       * never comes here for Echo, so no check is added for Echo         *
       ********************************************************************/
      if((hdr->msgType == EGT_GTPU_MSG_SUPP_EXT_HDR_NTF) || (hdr->msgType == EGT_GTPU_MSG_ERROR_IND))
      {
         if(!hdr->seqNumber.pres)
         {
            retValue = EGT_ERR_NO_SEQ; 
            EG_DBG_INFO(0,0, (egp,
                     "egUValidateGmHdr(Sequence number not present for message %d\n", hdr->msgType));
            EGU_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 
                  evntMsg->egMsg->msgHdr.msgType);
            EG_RETVALUE(retValue);
         }
      }

      /*********************************************************
       * End Marker should be sent, only from few interfaces   *
       *********************************************************/
      if(hdr->msgType == EGT_GTPU_MSG_END_MARKER )
      {
         /*-- eg003.201: S1_U interface added for END Marker msg --*/
         /* eg001.201 End marker to be Sent on X2/S1-U/S4/S12 interfaces*/
#ifndef EGTP_U_REL_9
        if((evntMsg->intfType != EGT_INT_S1_U_SGW_GTP_U)&&
           (evntMsg->intfType != EGT_INT_S1_U_ENB_GTP_U)&&
           (evntMsg->intfType != EGT_INT_X2_DL_ENB_GTP_U)&&
           (evntMsg->intfType != EGT_INT_X2_UL_ENB_GTP_U))
#else
         /********************************************************
          * Release 9 :End Marker: End marker can be received    *
          * from X2/S1-U/S4/S12 interfaces and hence checks are  *
          * added to ensure the same.                            *
          ****************************************************** */
        if((evntMsg->intfType != EGT_INT_S1_U_ENB_GTP_U)&&
           (evntMsg->intfType != EGT_INT_S1_U_SGW_GTP_U)&&
           (evntMsg->intfType != EGT_INT_S12_RNC_GTP_U)&&
           (evntMsg->intfType != EGT_INT_S12_SGW_GTP_U)&&
           (evntMsg->intfType != EGT_INT_S4_SGSN_GTP_U)&&
           (evntMsg->intfType != EGT_INT_S4_SGW_GTP_U)&&
           (evntMsg->intfType != EGT_INT_X2_DL_ENB_GTP_U)&&
           (evntMsg->intfType != EGT_INT_X2_UL_ENB_GTP_U))
#endif 
         {
            retValue = EGT_ERR_MSG_INV_TYPE;
            EG_DBG_INFO(0,0, (egp,
                     "egUValidateGmHdr(Invalid Message(%d) on this node \n",
                     hdr->msgType));
            EGU_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 
                  evntMsg->egMsg->msgHdr.msgType);
            EG_RETVALUE(retValue);
         }  
      }
   }
   /*************************************
    * Decoding side Validation          *
    *************************************/
   else
   {
      /*********************************************************
       * Sequence Number should be present for these messages  *
       *********************************************************/
      if((hdr->msgType == EGT_GTPU_MSG_ECHO_REQ) || (hdr->msgType == EGT_GTPU_MSG_ECHO_RSP))
      {
         if(!hdr->seqNumber.pres)
         {
            retValue = EGT_ERR_NO_SEQ; 
            EG_DBG_INFO(0,0, (egp,
                     "egUValidateGmHdr(Sequence number not present for message %d\n", hdr->msgType));
            EGU_FILL_ERR_EVNT_MSG(evntMsg, retValue,  0, 
                  evntMsg->egMsg->msgHdr.msgType);
            EG_RETVALUE(retValue);
         }

      }
/* eg001.201 End Marker reception not be dropped for X2 interface */
#ifndef EGTP_U_REL_9
      /**************************************************************
       * End Marker reception and Echo request reception on X2 is   *
       * not Valid in eGTP-U R8.Drop the received message.However   *
       * the same messages are valid in case of R9 hence no dropping*
       **************************************************************/
      if(((hdr->msgType == EGT_GTPU_MSG_END_MARKER) &&
          (evntMsg->intfType != EGT_INT_X2_DL_ENB_GTP_U && 
           evntMsg->intfType != EGT_INT_X2_UL_ENB_GTP_U)) ||
         ((hdr->msgType == EGT_GTPU_MSG_ECHO_REQ) && 
          (evntMsg->intfType == EGT_INT_X2_DL_ENB_GTP_U || 
           evntMsg->intfType == EGT_INT_X2_UL_ENB_GTP_U)))
      {
         retValue = EGT_ERR_MSG_INV_TYPE;
         EG_DBG_INFO(0,0, (egp,
                  "egUValidateGmHdr(Invalid Message(%d) on this node \n",
                  hdr->msgType));
         EGU_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 
               evntMsg->egMsg->msgHdr.msgType);
         EG_RETVALUE(retValue);
      }
#endif
   } 

   EG_RETVALUE (retValue);
}  /* End of egUValidateGmHdr() */


/**********************************************************************
 *   Fun:  egUDecIe
 *   
 *   Desc:  Decodes the GTP-U IE 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egUDecIe
(
   EgUEvnt         *evntMsg,   /* Event message to stores the decoded ie */
   Buffer          *mBuf,       /* Buffer which has to decode */
   Bool            *iePres,
   U8              *ieType      /* Decoded IE type */
 )
#else
PRIVATE S16 egUDecIe(evntMsg, mBuf, iePres, ieType)
   EgUEvnt        *evntMsg;       /* Event message to stores the decoded ie */
   Buffer         *mBuf;          /* Buffer which has to decode */
   Bool           *iePres;
   U8              *ieType;       /* Decoded IE type */
#endif
{
   S16              retVal   = ROK;        /* Holds return value */
   U16              tmpIeLen = 0;          /* Holds the IE Length */
   EgIeEntry        *ieEntry = NULLP;      /* Holds IE Entry */
   MsgLen           msgLen   = 0;          /* Holds Rcd Buffer length */
   EgUIe            tmpIe;                 /* tmp Holds Decoded IE */
   EgUstaDgn        dgn;                   /* Used for sending the alarms  */
   Bool             isTvType = FALSE;       /* is this IE of TV type ? */
   U8               skipIe[EG_MAX_BYTES];  /* To Skip IE from the buf */
   U16              iePosIdx = 0;          /* IE position Idx */

   EG_TRC2(egUDecIe);

   /******************************************
    * Initialization for all the structures  *
    ******************************************/
   cmMemset((U8*)&tmpIe, EG_ZERO, sizeof(EgUIe));
   cmMemset((U8*)skipIe, EG_ZERO, EG_MAX_BYTES);

   /*****************************************
    * find the length of the encoded buffer *
    *****************************************/
   SFndLenMsg(mBuf, &msgLen);

   if(msgLen == 0)
   {
      EG_RETVALUE(EGT_ERR_MSG_INV_LEN);
   }

   /*************************
    * Decode the IE Type    *
    ************************/
   SRemPreMsg(&tmpIe.ieType, mBuf);
   *ieType = tmpIe.ieType;
   msgLen--;

   isTvType = EGU_IE_IS_TV(tmpIe.ieType);
   if(!isTvType)
   {
      /************************
       * Decode the IE length *
       ***********************/
      if(tmpIe.ieType != EGU_IE_EXT_HDR_LST)
      {
         EG_DEC_U16(tmpIeLen, mBuf);
         msgLen -= EG_LEN_TWO;
      }
      else
      {
         SRemPreMsg((U8 *)&tmpIeLen, mBuf);
         msgLen--;
      }
   }

   /*******************************************
    * Get the IE entry from the IE Dictionary *
    ******************************************/
   if((iePosIdx = egCb.egUCb.edmUCb.egIeCb.posIdx[tmpIe.ieType]))
   {
      ieEntry = &egCb.egUCb.edmUCb.egIeCb.ieEntry[iePosIdx];
   }
   else
   {
      /**************************************************
       * Unknown IE, handle based on the Type (TV/TLV)  *
       **************************************************/
      if(isTvType)
      {
         /********************************************
          * Unknown IE of TV type - Return Error     *
          ********************************************/
         retVal = EGT_ERR_UNKNOWN_TV;
         EG_DBG_INFO(0,0, (egp,
                  "egUDecIe(IE(%d), resultType(%d)) \n",
                  tmpIe.ieType,  retVal));
         EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe.ieType,
               evntMsg->egMsg->msgHdr.msgType);
         /* ccpu00125923: klocwork fix */
      }
      else
      {
         /***************************************************
          * Unknown IE of TLV type -                        *
          * Skip this IE and continue further processing    *
          **************************************************/
         SRemPreMsgMult(skipIe, tmpIeLen, mBuf);
         retVal = ROK;
      }
	  EG_RETVALUE(retVal);
   }

   /**************************************************
    * Get the IE length from IE dict. for TV type IE *
    **************************************************/
   if(isTvType)
   {
      tmpIeLen = ieEntry->minLen;
   }

   if((iePres[tmpIe.ieType] == TRUE) && (tmpIe.ieType != EGU_IE_PVT_EXTN))
   {
      /*************************************************************
       * Skip this Duplicate IE and continue further processing    *
       * Duplicates are allowed for Private Extension IEs          *
       *************************************************************/
      SRemPreMsgMult(skipIe, tmpIeLen, mBuf);
      EG_RETVALUE(ROK);
   }

   /*******************************************
    * Do the IE length Validation for TLV IEs *
    *******************************************/
   if(!isTvType)
   {
      /********************************
       * Do the IE length Validation  *
       ********************************/
      retVal = egUValidateIeLen(ieEntry,evntMsg->egMsg->msgHdr.msgType,
            tmpIeLen);

      if (retVal == EGT_ERR_MAN_IE_INV_LEN)
      {
         /**************************************************************
          * Mandatory IE is received with Invalid length, report error *
          **************************************************************/
         EG_DBG_INFO(0,0, (egp,
                  "egUDecIe(IE(%d), resultType(%d)) \n",
                  tmpIe.ieType,  retVal));
         EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe.ieType, 
               evntMsg->egMsg->msgHdr.msgType);
         EG_RETVALUE(retVal);
      }
      /**********************************************************
       * Non-Mandatory IE received with Invalid length *
       **********************************************************/
      else if( retVal == EGU_SKIP_IE)
      {
         if (tmpIeLen > msgLen)
         {
            /************************************
             * Invalid length, report error     *
             ************************************/
            retVal = EGT_ERR_IE_INV_LEN;
            EG_DBG_INFO(0,0, (egp,
                     "egUDecIe(IE(%d), resultType(%d)) \n",
                     tmpIe.ieType,  retVal));
            EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe.ieType,
                  evntMsg->egMsg->msgHdr.msgType);
			/* ccpu00125923:: klocwork fix , removed return*/
         }
		 /* ccpu00125923: Klocwork fix , else added*/
		 else
		 {
		    /***************************************************
		     * skip this IE and continue further processing    *
			 **************************************************/
			 SRemPreMsgMult(skipIe, tmpIeLen, mBuf);
			 retVal = ROK;
	     }
         EG_RETVALUE(retVal);
      }
   }

   /********************************************
    * Retreive the data type from the ie entry *
    *******************************************/
   tmpIe.dataType = ieEntry->dataType;

   /**********************
    * Decode the IE Data *
    *********************/
   retVal = egUDecIeData(evntMsg, mBuf, &tmpIe, tmpIeLen );
   if( retVal != ROK)
   {
      EG_DBG_INFO(0,0, (egp,
               "egUDecIe(Unable to Decode IE: %d)\n",tmpIe.ieType));
      EG_RETVALUE(retVal);
   }

   /******************************
    * Append the IE to the egMsg *
    ******************************/
   retVal = EgUUtilGmAppendIe(evntMsg->egMsg, &tmpIe);
   if(retVal != ROK)
   {
      EG_DBG_INFO(0,0, (egp,
               "egUDecIe(Unable to Append IE :%d) \n",tmpIe.ieType));

      /**************************************************
       * If memory allocation fails send the report to   *
       * LM and then return                              *
       ****************************************************/
      if(retVal == EG_UTIL_ERR_MEM_ALLOC_FAILED)
      {
         cmMemset((U8*)&dgn, EG_ZERO, sizeof(EgUstaDgn));
         dgn.dgnVal[0].type = LEG_PAR_MEM;
         dgn.dgnVal[0].t.mem.region = evntMsg->region;
         dgn.dgnVal[0].t.mem.pool   = evntMsg->pool;
         egSendLmAlarm (LCM_CATEGORY_RESOURCE, LCM_EVENT_DMEM_ALLOC_FAIL, 
               LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
         EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe.ieType,evntMsg->egMsg->msgHdr.msgType);
         /* ccpu00125923: klocwork fix, removed return */
      }
      else
      {
         retVal = ROK;
      }
	  EG_RETVALUE(retVal);
   }

   /****************************
    * Update the iePres array  *
    ****************************/
   iePres[tmpIe.ieType] = TRUE;

   EG_RETVALUE(retVal); 
} /* End of egUDecIe() */



/**********************************************************************
 *   Fun:  egUDecIeData
 *   
 *   Desc:  Decodes the GTP-U Msg IE Data part 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egUDecIeData
(
 EgUEvnt     *evntMsg,       /* Event message to stores the decoded ie */
 Buffer      *mBuf,          /* Buffer which has to decode */
 EgUIe       *egIe,          /* Ie to store the decoded IE data */
 U16         ieLen           /* IE Length psnt in the Hdr */
 )
#else
PRIVATE S16 egUDecIeData(evntMsg, mBuf, egIe, ieLen)
   EgUEvnt    *evntMsg;       /* Event message to stores the decoded ie */
   Buffer     *mBuf;          /* Buffer which has to decode */
   EgUIe      *egIe;          /* Ie to store the decoded IE data */
   U16        ieLen;          /* Total Ie Length excluding hdr   */
#endif
{
   S16      retVal = ROK;         /* Holds the return value */
   U8       skipIe[EG_MAX_BYTES];  /* To Skip IE from the buf */

   EG_TRC2(egUDecIeData);

   EG_DBG_INFO(0,0, (egp,
            "egUDecIeData(IE Type(%d), IE Data Type(%d)) \n",
            egIe->ieType, egIe->dataType));

   /**************************************************
    * Decodes the IE based on the IE data type       *
    **************************************************/
   switch (egIe->dataType)
   {
      case EG_UNSIGNED_8:
         SRemPreMsg(&egIe->t.valUnSgnd8, mBuf);
         break;

      case EG_UNSIGNED_32:
         EG_DEC_U32(egIe->t.valUnSgnd32, mBuf);
         break;

      case EG_PVT_EXTN:
         {
            /*eg009.201:Klockwork fix,modified U8 correctly to U16*/
            U16 propValLen = 0;
            EG_DEC_U16(egIe->t.pvtExt.entId, mBuf);

            propValLen = ieLen - EG_LEN_TWO;
            egIe->t.pvtExt.propVal.length = propValLen;
            SRemPreMsgMult(egIe->t.pvtExt.propVal.val, propValLen, mBuf);
            break;
         }

      case EG_IP_ADDR:
         /***************************************
          * Ip address is either IPv4 or IPv6   *
          ***************************************/
         if(ieLen == EG_LEN_FOUR)
         {
            egIe->t.ipAddr.addrType = EG_LEN_FOUR;
            EG_DEC_U32(egIe->t.ipAddr.u.ipv4, mBuf);
         }
#ifdef EG_IPV6_SUPPORTED
         else
         {
            egIe->t.ipAddr.addrType = EG_LEN_SIXTEEN;
            SRemPreMsgMult(&egIe->t.ipAddr.u.ipv6, EG_LEN_SIXTEEN, mBuf);
         }
#endif /* EG_IPV6_SUPPORTED */
         break;

      case EG_EXT_HDR_TYPE_LST:
         {
            U16 cnt = 0;

            egIe->t.extHdrTypeLst.noOfExtHdrs = ieLen;
            for(cnt = 0; cnt < ieLen; cnt ++)
            {
               SRemPreMsg(&egIe->t.extHdrTypeLst.extType[cnt], mBuf);
            }
            break;
         }

      default:
         SRemPreMsgMult((Data*)skipIe, ieLen, mBuf);
         EG_RETVALUE(EG_IE_INV_DATA_TYPE);
   }

   EG_RETVALUE(retVal);
} /* End of egUDecIeData() */



/**********************************************************************
 *   Fun:  egUValidateGmMsgIEs
 *   
 *   Desc:  To Validate the GTP-U Msg IEs. 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egUValidateGmMsgIEs
(
   EgUEvnt     *evntMsg,              /* Contains EGMSG for validation */
   Bool        *iePres
 )
#else
PRIVATE S16 egUValidateGmMsgIEs(evntMsg, iePres)
   EgUEvnt     *evntMsg;             /* Contains the EGMSG for validation */
   Bool        *iePres;
#endif
{
   S16              retVal = ROK;           /* Holds the return value */
   EgUMsgEntry      *gmEntry = NULLP;       /* Holds the message Entry */
   S16              loopCnt = 0;                /* Used for the loops */
   U8               msgType = 0;           /* Message type */
   CmLListCp        *ieList = NULLP;      /* Ie List tobe sorted*/
   CmLList          *tmpNode = NULLP;      /* Node for outer loop of sorting */
   CmLList          *node    = NULLP;      /* Node for inner loop of sorting */
   EgUIe            *tmpEgIe = NULLP;      /* IE for outer loop of sorting */
   EgUIe            *egIe    = NULLP;      /* IE for inner loop of sorting*/
   CmLList          *lclCrnt = NULLP;      /* List used for inner loop */
   CmLList          *tmpList = NULLP;     /* Stores the crnt List */
   U16              gmPosIdx = 0;         /* Message Position Idx */

   EG_TRC2(egUValidateGmMsgIEs)

      msgType = evntMsg->egMsg->msgHdr.msgType;
   gmPosIdx = egCb.egUCb.edmUCb.egMsgCb.posIdx[msgType];
   gmEntry =  &egCb.egUCb.edmUCb.egMsgCb.gmEntry[gmPosIdx];

   /*****************************************************
    * Iterate through the message dict. to find out the *
    * Missing Mandatory IEs                             *
    *****************************************************/
   if(gmEntry->nmbMandIe)
   {
      for ( loopCnt=0; loopCnt < gmEntry->nmbIe; loopCnt++)
      {
         if((gmEntry->ieProperties[loopCnt].ieClass == EG_IE_MANDATORY) &&
               (! iePres[gmEntry->ieProperties[loopCnt].ieType]))
         {
            /************************************************
             * Send missing mandatory IE to the application *
             ************************************************/
            retVal = EGT_ERR_MSG_MISS_MAND_IE;
            EG_DBG_INFO(0,0, (egp,
                     "egValidateGmMsgIEs(IE Type(%d), Msg Type (%d) - \
               mandatory element is missing, resulType(%d)) \n",
                     gmEntry->ieProperties[loopCnt].ieType, msgType, retVal));

            EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal,
                  gmEntry->ieProperties[loopCnt].ieType, evntMsg->egMsg->msgHdr.msgType);
            EG_RETVALUE(retVal);
         }
      }
   }

   if((evntMsg->isEnc) && (evntMsg->egMsg->u.egIeList.count > 1))
   {
      ieList = &evntMsg->egMsg->u.egIeList;
      /***************************************
       * Sort the IEs in the Ascending order *
       ***************************************/
      tmpEgIe = (EgUIe* ) CM_LLIST_FIRST_NODE(ieList, tmpNode);

      while(tmpEgIe != NULLP)
      {
         lclCrnt = ieList->crnt->next;
         if(lclCrnt == NULLP)
            break;
         else
            egIe = (EgUIe* )(lclCrnt->node);

         while(egIe != NULLP)
         {
            if(tmpEgIe->ieType > egIe->ieType)
            {
               tmpList = ieList->crnt;
               node = lclCrnt;

               /*********************************
                * Remove the Node from the List *
                *********************************/
               cmLListDelFrm(ieList, node);

               /***************************************************
                * Travel till the position where this node needs  *
                * tobe inserted                                   *
                ***************************************************/
               ieList->crnt = tmpList;

               /***********************************
                * Insert the removed node         *
                ***********************************/
               cmLListInsCrnt(ieList, node);

               tmpNode = node;
               tmpEgIe = (EgUIe* ) tmpNode;
            }
            lclCrnt = lclCrnt->next;
            if(lclCrnt == NULLP)
               break;
            else   
               egIe = (EgUIe* )(lclCrnt->node);
         }

         tmpEgIe = (EgUIe* ) CM_LLIST_NEXT_NODE(ieList, tmpNode);
      }
   }

   EG_RETVALUE(retVal);
} /* End of egUValidateGmMsgIEs() */


/*************************************************************************
 *   Fun:  egUValidateIeLen
 *
 *   Desc:  Validate the Ie Len for a GTP-U IE
 *
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *
 *   Notes: None
 *
 *   File:  eg_edm.c
 ************************************************************************/
#ifdef ANSI
PRIVATE S16 egUValidateIeLen
(
 EgIeEntry    *ieEntry,   /* Holds the min and max length   */
 U8            msgType,   /* To get the msg entry           */
 U16           ieLen      /* actual IE length to validate   */
 )
#else
PRIVATE S16 egUValidateIeLen(ieEntry, msgType, ieLen)
   EgIeEntry    *ieEntry;   /* Holds the min and max length   */
   U8            msgType;   /* To get the msg entry           */
   U16           ieLen;     /* actual IE length to validate   */
#endif
{
   EgUMsgEntry       *gmEntry = NULLP;   /* Holds the msg Entry */
   U8                tmpCnt = 0;   /* Index to get the pos of IE in the list */
   U16               gmPosIdx = 0; /* Position Idx */ 

   EG_TRC2(egUValidateIeLen);

   /*********************
    * Invalid IE length *
    *********************/
   /*************************************************************************
    * For IEs of type EG_IP_ADDR the length should match with exact length  *
    *************************************************************************/
   if ((ieLen < ieEntry->minLen || ieLen > ieEntry->maxLen) ||
         ((ieEntry->dataType == EG_IP_ADDR) && 
          ((ieLen != ieEntry->minLen) && (ieLen != ieEntry->maxLen))))
   {
      /*********************************************************
       * Get the ieProp and then check whether this ie is      *
       * mandatory or not                                      *
       ********************************************************/
      gmPosIdx = egCb.egUCb.edmUCb.egMsgCb.posIdx[msgType];
      gmEntry = &egCb.egUCb.edmUCb.egMsgCb.gmEntry[gmPosIdx];

      for(tmpCnt=0; tmpCnt < gmEntry->nmbIe; tmpCnt++)
      {
         if ((gmEntry->ieProperties[tmpCnt].ieType == ieEntry->ieCode) &&
               (gmEntry->ieProperties[tmpCnt].ieClass == EG_IE_MANDATORY))
         {
            EG_DBG_INFO(0,0, (egp,
                     "egUValidateIeLen: :Length validation for Mandatory IE(%d) is failed) \n",
                     ieEntry->ieCode));
            /*****************************************
             * Invalied Mandatory IE - Return Error  *
             *****************************************/
            EG_RETVALUE (EGT_ERR_MAN_IE_INV_LEN);
         }
      }

      /******************************************
       * For non-Mandatory IEs - Skip this IE   *
       * and continue further processing        *
       ******************************************/
      EG_RETVALUE(EGU_SKIP_IE);
   } /* End of getting the idx */

   EG_RETVALUE(ROK);
} /* End of egUValidateIeLen() */


/***********************************************************************
 *   Fun:  egUGetCRType
 *   
 *   Desc: Calculates the CR value of an IE
 *   
 *   Ret:   EG_CH_REQ     - Comprehension Required
 *          EG_CH_NOT_REQ - Comprehension Not Required
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egUGetCRType
(
U8         extHdrType,             /* Ext. Hdr type */
U8         extHdrLen,               /* Ext. Hdr length */
Buffer      *mBuf                 /* Buffer */
)
#else
PRIVATE S16 egUGetCRType(extHdrType, extHdrLen, mBuf)
U8         extHdrType;            /* Ext. Hdr type */
U8         extHdrLen;             /* Ext. Hdr length */
Buffer      *mBuf;                 /* Buffer */
#endif
{
   U8 chValue = 0; /* Comprehension value */
   U8 extHdrValLen = 0; /* Length of Ext. Hdr Value field */
   U8 temp[100];   /* temp. variable */

   EG_TRC2(egUGetCRType);

   /************************************************************
    * Get Comprehension value, by shifting extHdrType 6 times, *
    * to get the the last 2bits                                *
    ************************************************************/
   chValue = extHdrType >> EG_SHIFT_6;

   /******************************
    * Comprehension Required     *
    ******************************/
   if(chValue == EG_THREE)
   {
      EG_RETVALUE(EG_CH_REQ);
   }
   else
   {
      /******************************
      * Comprehension Not Required *
      ******************************/
      /************************************************************************
       * Length field of Ext. Hdr is in terms of 4 bytes.                     *
       * Ext. Hdr Value field is of size 2 bytes (length, Next Ext. Hdr type) *
       * less than the total length of the Ext. Hdr                           *
       ************************************************************************/
      extHdrValLen = (EG_LEN_FOUR * extHdrLen) - EG_LEN_TWO;

      /***********************************************
       * Delete the Extension header Value Feild     *
       ***********************************************/
      SRemPreMsgMult(temp, extHdrValLen, mBuf);
      EG_DBG_INFO(0,0, (egp,
               "egUGetCRType Deleted Extension Header :%d", extHdrType));
      EG_RETVALUE(EG_CH_NOT_REQ);
   }

} /* end of egUGetCRType() */

#endif /* End of EGTP_U */


/********************************************************************30**

         End of file:     pg_edm.c@@/main/3 - Fri Feb  4 18:18:15 2011

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
/main/1      ---          ad             1. Created for Initial Release 1.1
/main/2      ---          kchaitanya     1. Initial for eGTP 1.2 Release
/main/2      eg001.102    rss            1. Memory leak fix.
/main/3      ---          pmacharla         Initial for eGTP release 2.1
/main/3      eg001.201    asaurabh       1. End marker can be received from 
                                            X2/S1-U/S4/S12 interfaces
					 2. Endmarker reception not be dropped 
                                            for X2 interface 
/main/3      eg003.201    psingh         1. Merged code from eg006.102 to Fix 
                                            TRACE5 macro related issue
                                         2. Merged code from eg006.102 to Fix 
                                            the encoding and decoding of optional
                                            fields(Sequence Number, N-PDU
 														  Number and Next Extension Header Type)
                                         3. Fix for IOT issue
                                         4. Updated valid interfaces S1_U &X2 for 
                                            END_MARKER msg
                                         5. Init the ptr ieProp with NULL to avoid
                                            warnings
/main/3      eg004.201   magnihotri      1.Header files added for eGTP-C PSF
/main/3      eg006.201    psingh         1.Updated EvntMsg to reflect the error
                                            return type
                                         2.Removed EGTPU_HDR_ALWAYS_PRSNT flag 
                                         3.Aligned GTP-U Encode/Decode proc with Spec
/main/3      eg007.201    psingh         1. Free Hashlist,gmDictHlCp, associated with intfCbLst
/main/3      eg008.201    asaurabh       1. Data Type of nextGmCb modified from PTR to EgGmCb
/main/3      eg009.201    asaurabh       1. Klocwork fix-Unused variable removed
                                         2. Added for S2B
                                         3. Added for S2A
                                         4. Warning fixed,unused variable grpOccCnt
                                         5. Klockwork fix-variable not usedafter assignment
                                         6. Klockwork fix -Variable not used
                                         7. Klockwork fix,check groupedIe ptr for NULL
                                         8. Magic number replaced with Macro
                                         9. Added for S2B and S2A
                                         10.Klockwork fix datatype modified
                                         11.Fix TEID zero 
                                         12.egMsg & memory allocated to IE inside egMsg 
                                            is freed with EgUUtilDeAllocGmMsg
                                         13.Klockwork fix Modified datatype
                                         14.Klockwork fix,modified U8 correctly to U16
/main/3      eg010.201    asaurabh       1. Fixed encoding piggyBack bit
                                         2. Fixed decoding piggyback bit
/main/3      eg012.201    avenugop       1. Klocwork fixes added
*********************************************************************91*/
