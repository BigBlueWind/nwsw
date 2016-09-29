/********************************************************************16**

                         (c) COPYRIGHT 2010 by 
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

     Name:     LTE RRC

     Type:     C source file

     Desc:     EMM Decoder file

     File:     ve_emm.c

     Sid:      

     Prg:     mraj 

*********************************************************************21*/

/* Added emm functionality */

#ifndef EMM_UNIT_TEST
#include "cvmx-config.h"
#include "cvmx.h"
#include "cvmx-pow.h"
#include "cvmx-fpa.h"
#include "cvmx-helper-fpa.h"

#include "ve_debug.h"
#endif

/**********ve_emm.h**********************/
#define VE_EMM_MAX_MSG_VALUE 0x25
#define CM_MAX_EMM_MSG_NAME  35
#ifdef EMM_UNIT_TEST
#define DISPLAY(a) printf a
#else
#define DISPLAY(a) ENB_DEBUG_MSG(ENB_DBG_SUMMARY, a)
#endif

/* type of coding */
#define VE_EMM_ENC 0
#define VE_EMM_DEC 1

/* Direction */
#define VE_UPLINK 0
#define VE_DOWNLINK 1


/* EMM Message Index values */
#define VE_EMM_IDX_ATTACH_ACC    0x00
#define VE_EMM_IDX_ATTACH_CMP    0x01
#define VE_EMM_IDX_ATTACH_REJ    0x02
#define VE_EMM_IDX_ATTACH_REQ    0x03
#define VE_EMM_IDX_AUTH_FAIL    0x04
#define VE_EMM_IDX_AUTH_REJ      0x05
#define VE_EMM_IDX_AUTH_REQ      0x06
#define VE_EMM_IDX_AUTH_RSP      0x07
#define VE_EMM_IDX_CS_SVC_NTF    0x08
#define VE_EMM_IDX_DETACH_ACC_ENC    0x09
#define VE_EMM_IDX_DETACH_ACC_DEC    0x0a
#define VE_EMM_IDX_DETACH_REQ_ENC    0x0b
#define VE_EMM_IDX_DETACH_REQ_DEC    0x0c
#define VE_EMM_IDX_DL_NAS_TPT    0x0d
#define VE_EMM_IDX_EMM_INFO      0x0e
#define VE_EMM_IDX_EMM_STS       0x0f
#define VE_EMM_IDX_EXT_SVC_REQ   0x10
#define VE_EMM_IDX_GUTI_RLC_CMD  0x11
#define VE_EMM_IDX_GUTI_RLC_CMP  0x12
#define VE_EMM_IDX_ID_REQ        0x13
#define VE_EMM_IDX_ID_RSP        0x14
#define VE_EMM_IDX_SEC_MODE_CMD  0x15
#define VE_EMM_IDX_SEC_MODE_CMP  0x16
#define VE_EMM_IDX_SEC_MODE_REJ  0x17
#define VE_EMM_IDX_SEC_PROT_MSG  0x18
#define VE_EMM_IDX_SVC_REJ       0x19
#define VE_EMM_IDX_SVC_REQ       0x1a
#define VE_EMM_IDX_TAU_ACC       0x1b
#define VE_EMM_IDX_TAU_CMP       0x1c
#define VE_EMM_IDX_TAU_REJ       0x1d
#define VE_EMM_IDX_TAU_REQ       0x1e
#define VE_EMM_IDX_UL_NAS_TPT    0x1f

#define VE_EMM_MAX_MSG           0x20
#define VE_EMM_MAX_MSG_VALUE     0x25

#define VE_EMM_IDX_INVALID       0xff



/* EMM Message Types */
#define VE_EMM_MSG_ATTACH_REQ    0x41
#define VE_EMM_MSG_ATTACH_ACC    0x42
#define VE_EMM_MSG_ATTACH_CMP    0x43
#define VE_EMM_MSG_ATTACH_REJ    0x44
#define VE_EMM_MSG_DETACH_REQ    0x45
#define VE_EMM_MSG_DETACH_ACC    0x46

#define VE_EMM_MSG_TAU_REQ       0x48
#define VE_EMM_MSG_TAU_ACC       0x49
#define VE_EMM_MSG_TAU_CMP       0x4A
#define VE_EMM_MSG_TAU_REJ       0x4B

#define VE_EMM_MSG_EXT_SVC_REQ   0x4C
#define VE_EMM_MSG_SVC_REJ       0x4E
   
#define VE_EMM_MSG_GUTI_RLC_CMD  0x50
#define VE_EMM_MSG_GUTI_RLC_CMP  0x51
#define VE_EMM_MSG_AUTH_REQ      0x52
#define VE_EMM_MSG_AUTH_RSP      0x53
#define VE_EMM_MSG_AUTH_REJ      0x54
#define VE_EMM_MSG_AUTH_FAIL     0x5C
#define VE_EMM_MSG_ID_REQ        0x55
#define VE_EMM_MSG_ID_RSP        0x56
#define VE_EMM_MSG_SEC_MODE_CMD  0x5D
#define VE_EMM_MSG_SEC_MODE_CMP  0x5E
#define VE_EMM_MSG_SEC_MODE_REJ  0x5F

#define VE_EMM_MSG_EMM_STS       0x60
#define VE_EMM_MSG_EMM_INFO      0x61
#define VE_EMM_MSG_DL_NAS_TPT    0x62
#define VE_EMM_MSG_UL_NAS_TPT    0x63
#define VE_EMM_MSG_CS_SVC_NTF    0x64
/* Need to Clarify */
#define VE_EMM_MSG_SERVICE_REQ   0x65

/* Message to EDM index map */
#define VE_EMM_MSGIDXMAP(_msgType, _msgIdx, _coding) \
   do \
   { \
      if ((_msgType & 0x3f) > VE_EMM_MAX_MSG_VALUE) \
      { \
                     _msgIdx = VE_EMM_MAX_MSG; \
                     break; \
      } \
      _msgIdx = VeEmmMsgToIdxMap[_msgType & 0x3f]; \
      if (_msgType == VE_EMM_MSG_DETACH_REQ && \
                         _coding == VE_EMM_ENC) \
      { \
         _msgIdx = VE_EMM_IDX_DETACH_REQ_ENC; \
      } \
      else if (_msgType == VE_EMM_MSG_DETACH_ACC && \
                         _coding == VE_EMM_DEC) \
      { \
         _msgIdx = VE_EMM_IDX_DETACH_ACC_DEC; \
      } \
      if (_msgType == VE_EMM_MSG_SERVICE_REQ \
                         ) \
      { \
         _msgIdx = VE_EMM_IDX_SVC_REQ; \
      } \
   } while(0)
/**********ve_emm.h**********************/

/**********ve_emm.x**********************/
typedef struct veEmmMsg
{
    unsigned char protDisc;
    unsigned char secHdrType;
    unsigned char msgId;
    union
    {
       /*********At this point, not going for complete decode as it is
        * redundant */
       VeEmmAttachRequest  atchReq;
       VeEmmAttachReject   atchRej;
       VeEmmAttachComplete atchCmp;
       VeEmmAttachAccept   atchAcc;
       VeEmmAuthFailure    authFlr;
       VeEmmAuthReject     authRej;
       VeEmmAuthReq        authReq;
       VeEmmAuthRsp        authRsp;
       VeEmmDetachReq      dtchReq;
       VeEmmIdReq          idReq;
       VeEmmIdRsp          idRsp;
       VeEmmSecModeCmd     secModCmd;
       VeEmmSecModeCmp     secModCmp;
       VeEmmSecModeRej     secModRej;
       VeEmmStatus         emmSts;
       VeEmmServiceReq     srvReq;
    }u;
} VeEmmMsg;

static unsigned char VeEmmMsgToIdxMap[VE_EMM_MAX_MSG_VALUE] =
{
   VE_EMM_IDX_INVALID,
   VE_EMM_IDX_ATTACH_REQ,
   VE_EMM_IDX_ATTACH_ACC,
   VE_EMM_IDX_ATTACH_CMP,
   VE_EMM_IDX_ATTACH_REJ,
   VE_EMM_IDX_DETACH_REQ_DEC, /*-- VE_EMM_IDX_DETACH_REQ_ENC, --*/
   VE_EMM_IDX_DETACH_ACC_ENC, /*-- VE_EMM_IDX_DETACH_ACC_DEC, --*/

   VE_EMM_IDX_INVALID,

   VE_EMM_IDX_TAU_REQ,
   VE_EMM_IDX_TAU_ACC,
   VE_EMM_IDX_TAU_CMP,
   VE_EMM_IDX_TAU_REJ,

   VE_EMM_IDX_EXT_SVC_REQ,
   VE_EMM_IDX_SVC_REQ,
   VE_EMM_IDX_SVC_REJ,

   VE_EMM_IDX_INVALID,

   VE_EMM_IDX_GUTI_RLC_CMD,
   VE_EMM_IDX_GUTI_RLC_CMP,
   VE_EMM_IDX_AUTH_REQ,
   VE_EMM_IDX_AUTH_RSP,
   VE_EMM_IDX_AUTH_REJ,
   VE_EMM_IDX_ID_REQ,
   VE_EMM_IDX_ID_RSP,
   VE_EMM_IDX_INVALID,
   VE_EMM_IDX_INVALID,
   VE_EMM_IDX_INVALID,
   VE_EMM_IDX_INVALID,
   VE_EMM_IDX_INVALID,
   VE_EMM_IDX_AUTH_FAIL,
   VE_EMM_IDX_SEC_MODE_CMD,
   VE_EMM_IDX_SEC_MODE_CMP,
   VE_EMM_IDX_SEC_MODE_REJ,


   VE_EMM_IDX_EMM_STS,
   VE_EMM_IDX_EMM_INFO,
   VE_EMM_IDX_DL_NAS_TPT,
   VE_EMM_IDX_UL_NAS_TPT,
   VE_EMM_IDX_CS_SVC_NTF
};

/* Message Names */
static signed char msgName[VE_EMM_MAX_MSG][CM_MAX_EMM_MSG_NAME] =
{
   "Attach Accept",
   "Attach Complete",
   "Attach Reject",
   "Attach Request",
   "Authentication Failure",
   "Authentication Reject",
   "Authentication Request",
   "Authentication Response",
   "CS Service Notification",
   "UE Initiated Detach Accept",
   "Network Initiated Detach Accept",
   "UE Initiated Detach Request",
   "Network Iniated Detach Request",
   "Downlink NAS Transport",
   "EMM Information",
   "EMM Status",
   "Exteded Service Request",
   "GUTI Reallocation Command",
   "GUTI Reallocation Complete",
   "Identity Request",
   "Identity Response",
   "Security Mode Command",
   "Security Mode Complete",
   "Securiry Mode Reject",
   "Security Protected NAS Message",
   "Service Reject",
   "Service Request",
   "Tracking Area Update Accept",
   "Tracking Area Update Complete",
   "Tracking Area Update Reject",
   "Tracking Area Update Request",
   "Uplink NAS Transport"
};
signed char veDecodeNasMsg(char *buf, char dir); /* g++ compilation */
/**********ve_emm.x**********************/

/**********ve_emm.c**********************/
#ifdef ANSI
static signed char veEmmDecHdr
(
unsigned char  *buf,
VeEmmMsg *msg
)
#else
static signed char veEmmDecHdr (buf, msg)
unsigned char  *buf;
VeEmmMsg *msg;
#endif
{
   /*TRC2(veEmmDecHdr)*/

   if (!buf)
   {
      DISPLAY(("*************Invalid Buffer*************\n"));
      return (0);
   }

   msg->protDisc = buf[0] & 0x0f;
   msg->secHdrType = (buf[0] >> 4) & 0x0f;
   if(msg->secHdrType == 0)
   {
       msg->msgId = buf[1];
   }
   else if(msg->secHdrType == 12)
   { /* case of Service Request message      */
      msg->msgId = VE_EMM_MSG_SERVICE_REQ;
   }
   else
   { /* case of security protected NAS message */
      DISPLAY(("Received Security protected NAS msg.\n"));
      msg->msgId = 0xff;
   }

   return (0);

} /* veEmmDecHdr */



signed char veDecodeNasMsg(char *buf, char dir)
{
   unsigned char indx = 0;
   signed char   ret;
   VeEmmMsg      msg;
   signed char   msgIdx;

   if (!buf)
   {
      DISPLAY(("NAS Message with invalid length or buffer\n"));
      return(0);
   }

   /******* Decoding Header ******************/
   ret = veEmmDecHdr((unsigned char *)buf, &msg);/* g++ compilation fix  */
   indx+=2;
   /**********End of Header decoding *********/

   /* Extract Message index */
   VE_EMM_MSGIDXMAP(msg.msgId, msgIdx, VE_EMM_DEC);

   if (msgIdx == VE_EMM_MAX_MSG)
   {
      DISPLAY(("Error: Unknown Msg Type: (%d)\n", (const char*)msg.msgId));/* g++ compilation fix */
      return (0);
   }

   if (dir == VE_UPLINK)
   {
#ifdef PICOCCPU_TEST
     DISPLAY(("[NAS]: UE -> CoreNetwork : %s\n", msgName[msgIdx]));
#else
     ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "[NAS]: UE -> CoreNetwork : %s\n", msgName[msgIdx]);
#endif
   }
   else
   {
#ifdef PICOCCPU_TEST
     DISPLAY(("[NAS]: CoreNetwork -> UE : %s\n", msgName[msgIdx]));
#else
     ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "[NAS]: CoreNetwork -> UE : %s\n", msgName[msgIdx]);
#endif
   }
   return(1); /* g++ compilation fix */
}

#ifdef EMM_UNIT_TEST
unsigned char attachNasMsg[]={0x07, 0x41, 0x71, 0x08, 0x09, 0x10, 0x01, 0x10,
   0x32, 0x54, 0x16, 0x64, 0x05, 0xc0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x01, 0xd0, 0x11};
unsigned char attachAccept[] = {0x07, 0x42, 01, 0xff, 06};
unsigned char attachComplete[]={0x07, 0x43, 00, 0x03, 0x52, 0x00, 0xc2};
unsigned char junk[]={0x11, 0x44, 0x44};
unsigned char junk2[]={0x00, 0xff, 0xff};

int main()
{
   char *input;
   /***************************/
   /* Test 1 : Attach Request */
   /***************************/
   DISPLAY(("*****************TEST 1*****************\n"));
   input = attachNasMsg;
   veDecodeNasMsg(input, VE_UPLINK);
   /*********END***************/
   /***************************/
   /* Test 2 : Attach Accept */
   /***************************/
   DISPLAY(("*****************TEST 2*****************\n"));
   input = attachAccept;
   veDecodeNasMsg(input, VE_DOWNLINK);
   /********END****************/
   /***************************/
   /* Test 3 : Attach Complete */
   /***************************/
   DISPLAY(("*****************TEST 3*****************\n"));
   input = attachComplete;
   veDecodeNasMsg(input, VE_UPLINK);
   /**********END**************/
   /***************************/
   /* Test 4 : Junk */
   /***************************/
   DISPLAY(("*****************TEST 4*****************\n"));
   input = junk;
   veDecodeNasMsg(input, VE_UPLINK);
   /**********END**************/
   /***************************/
   /* Test 5 : Junk 2*/
   /***************************/
   DISPLAY(("*****************TEST 5*****************\n"));
   input = junk2;
   veDecodeNasMsg(input, VE_UPLINK);
   /**********END**************/
   /***************************/
   /* Test 6 : Junk 2*/
   /***************************/
   DISPLAY(("*****************TEST 6*****************\n"));
   input = 0x0;
   veDecodeNasMsg(input, VE_UPLINK);
   /**********END**************/
   return;
}
#endif
/**********ve_emm.c**********************/

/********************************************************************30**

         End of file:     ve_emm.c@@/main/1 - Wed Dec  1 08:44:25 2010

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
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
