/********************************************************************16**

                         (c) COPYRIGHT 2009 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transnitted, disclosed or used in any form or by any means
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


/***********************************************************************

    Name: Source file for filling LM configuration

    Type: C Source

    Desc: 

    File: eg_dbsn.c

    Sid:      eg_dbsn.c@@/main/1 - Fri Feb  4 18:18:09 2011

    Prg : an

************************************************************************/


/***********************************************************************
    This file contains automatically generated code
************************************************************************/
/* Header Include Files (.h) */
#include "envopt.h"           /* Environment Options */
#include "envdep.h"           /* Environment Dependent */
#include "envind.h"           /* Environment Independent */
#include "gen.h"              /* General */
#include "ssi.h"              /* System Services */
#include "cm_tkns.h"          /* Common Tokens */
#include "cm_hash.h"          /* Common Hash List */
#include "cm_mblk.h"          /* Common memory */
#include "cm_llist.h"         /* Common Link List */
#include "cm5.h"              /* Common Structures */
#include "cm_inet.h"          /* Common IP network */
#include "cm_tpt.h"           /* Common Transport */
#include "hit.h"              /* TUCL layer */
#include "egt.h"              /* EU layer */
#include "leg.h"              /* layer manager */
#include "eg_util.h"          /* UTIL */
#include "eg_edm.h"           /* EDM MODULE */
#include "eg_dbsn.h"
#include "eg_dbutl.h"   /* Ie codes are here */

/* Header Include Files (.x) */

#include "gen.x"           /* General */
#include "ssi.x"           /* System Services */
#include "cm_tkns.x"       /* Common Tokens */
#include "cm_hash.x"       /* Common Hash List */
#include "cm_mblk.x"       /* Common memory */
#include "cm_llist.x"      /* Common Link List */
#include "cm5.x"           /* Common Structures */
#include "cm_inet.x"       /* Common IP network */
#include "cm_tpt.x"        /* Common Transport */
#include "hit.x"           /* TUCL layer */
#include "egt.x"           /* EU layer */
#include "leg.x"           /* layer manager */
#include "eg_util.x"       /* UTIL */
#include "eg_edm.x"      /* Application MODULE */
#include "eg_dbutl.x"      /* Application MODULE */
#include "eg_dbsn.x"

#ifdef EGTP_C
#ifdef EG_REL_930

/***********************************************************************

   Func  : egMakesnMBMSESNSTOPREQGmEntry

   Desc  : This function makes the GM Entry for MBMSESNSTOPREQ

   Ret   : None

   File  : smegsm.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesnMBMSESNSTOPREQGmEntry
(
LegMsgEntry               *gmMBMSESNSTOPREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesnMBMSESNSTOPREQGmEntry(gmMBMSESNSTOPREQCb)
LegMsgEntry               *gmMBMSESNSTOPREQCb;
#endif
{
   U8 ieCnt = 0;
   TRC2(egMakesnMBMSESNSTOPREQGmEntry);

   /* Populate the MBMSESNSTOPREQ GM Entry */
   gmMBMSESNSTOPREQCb->msgType = EG_SN_MBMSESNSTOPREQ_CMD_CODE;
   gmMBMSESNSTOPREQCb->nmbIeProp = EG_SN_MBMSESNSTOPREQ_NMB_IES;
   gmMBMSESNSTOPREQCb->validateIe = (EscFun)NULLP;

   /* Populate MBMSESNSTOPREQ IE Properties */

   /* Populate MBMS_FLOW_ID IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTOPREQCb->ieProperties[ieCnt],
                    EG_MBMS_FLOW_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNSTOPREQCb->posIndex[0][EG_MBMS_FLOW_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTOPREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMBMSESNSTOPREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesnMBMSESNSTOPREQGmEntry */


/***********************************************************************

   Func  : egMakesnMBMSESNSTOPRSPGmEntry

   Desc  : This function makes the GM Entry for MBMSESNSTOPRSP

   Ret   : None

   File  : smegsm.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesnMBMSESNSTOPRSPGmEntry
(
LegMsgEntry               *gmMBMSESNSTOPRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesnMBMSESNSTOPRSPGmEntry(gmMBMSESNSTOPRSPCb)
LegMsgEntry               *gmMBMSESNSTOPRSPCb;
#endif
{
   U8 ieCnt = 0;
   TRC2(egMakesnMBMSESNSTOPRSPGmEntry);

   /* Populate the MBMSESNSTOPRSP GM Entry */
   gmMBMSESNSTOPRSPCb->msgType = EG_SN_MBMSESNSTOPRSP_CMD_CODE;
   gmMBMSESNSTOPRSPCb->nmbIeProp = EG_SN_MBMSESNSTOPRSP_NMB_IES;
   gmMBMSESNSTOPRSPCb->validateIe = (EscFun)NULLP;

   /* Populate MBMSESNSTOPRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTOPRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTOPRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTOPRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMBMSESNSTOPRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesnMBMSESNSTOPRSPGmEntry */


/***********************************************************************

   Func  : egMakesnMBMSESNSTRTREQGmEntry

   Desc  : This function makes the GM Entry for MBMSESNSTRTREQ

   Ret   : None

   File  : smegsm.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesnMBMSESNSTRTREQGmEntry
(
LegMsgEntry               *gmMBMSESNSTRTREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesnMBMSESNSTRTREQGmEntry(gmMBMSESNSTRTREQCb)
LegMsgEntry               *gmMBMSESNSTRTREQCb;
#endif
{
   U8 ieCnt = 0;
   TRC2(egMakesnMBMSESNSTRTREQGmEntry);

   /* Populate the MBMSESNSTRTREQ GM Entry */
   gmMBMSESNSTRTREQCb->msgType = EG_SN_MBMSESNSTRTREQ_CMD_CODE;
   gmMBMSESNSTRTREQCb->nmbIeProp = EG_SN_MBMSESNSTRTREQ_NMB_IES;
   gmMBMSESNSTRTREQCb->validateIe = (EscFun)NULLP;

   /* Populate MBMSESNSTRTREQ IE Properties */

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TMGI IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_TMGI_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_TMGI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_SESS_DURATION IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_MBMS_SESS_DURATION_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_MBMS_SESS_DURATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_SERV_AREA IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_MBMS_SERV_AREA_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_MBMS_SERV_AREA_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_SESS_ID IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_MBMS_SESS_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_MBMS_SESS_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_FLOW_ID IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_MBMS_FLOW_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_MBMS_FLOW_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_QOS IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_BEARER_QOS_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_IP_MULT_DIST IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_MBMS_IP_MULT_DIST_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_MBMS_IP_MULT_DIST_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesnMBMSESNSTRTREQGmEntry */


/***********************************************************************

   Func  : egMakesnMBMSESNSTRTRSPGmEntry

   Desc  : This function makes the GM Entry for MBMSESNSTRTRSP

   Ret   : None

   File  : smegsm.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesnMBMSESNSTRTRSPGmEntry
(
LegMsgEntry               *gmMBMSESNSTRTRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesnMBMSESNSTRTRSPGmEntry(gmMBMSESNSTRTRSPCb)
LegMsgEntry               *gmMBMSESNSTRTRSPCb;
#endif
{
   U8 ieCnt = 0;
   TRC2(egMakesnMBMSESNSTRTRSPGmEntry);

   /* Populate the MBMSESNSTRTRSP GM Entry */
   gmMBMSESNSTRTRSPCb->msgType = EG_SN_MBMSESNSTRTRSP_CMD_CODE;
   gmMBMSESNSTRTRSPCb->nmbIeProp = EG_SN_MBMSESNSTRTRSP_NMB_IES;
   gmMBMSESNSTRTRSPCb->validateIe = (EscFun)NULLP;

   /* Populate MBMSESNSTRTRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTRSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTRSPCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_DIST_ACK IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTRSPCb->ieProperties[ieCnt],
                    EG_MBMS_DIST_ACK_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTRSPCb->posIndex[0][EG_MBMS_DIST_ACK_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTRSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmMBMSESNSTRTRSPCb->posIndex[1][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTRSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNSTRTRSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMBMSESNSTRTRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesnMBMSESNSTRTRSPGmEntry */


/***********************************************************************

   Func  : egMakesnMBMSESNUPDREQGmEntry

   Desc  : This function makes the GM Entry for MBMSESNUPDREQ

   Ret   : None

   File  : smegsm.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesnMBMSESNUPDREQGmEntry
(
LegMsgEntry               *gmMBMSESNUPDREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesnMBMSESNUPDREQGmEntry(gmMBMSESNUPDREQCb)
LegMsgEntry               *gmMBMSESNUPDREQCb;
#endif
{
   U8 ieCnt = 0;
   TRC2(egMakesnMBMSESNUPDREQGmEntry);

   /* Populate the MBMSESNUPDREQ GM Entry */
   gmMBMSESNUPDREQCb->msgType = EG_SN_MBMSESNUPDREQ_CMD_CODE;
   gmMBMSESNUPDREQCb->nmbIeProp = EG_SN_MBMSESNUPDREQ_NMB_IES;
   gmMBMSESNUPDREQCb->validateIe = (EscFun)NULLP;

   /* Populate MBMSESNUPDREQ IE Properties */

   /* Populate MBMS_SERV_AREA IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_MBMS_SERV_AREA_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_MBMS_SERV_AREA_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TMGI IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_TMGI_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_TMGI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_SESS_DURATION IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_MBMS_SESS_DURATION_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_MBMS_SESS_DURATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_QOS IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_BEARER_QOS_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_SESS_ID IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_MBMS_SESS_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_MBMS_SESS_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_FLOW_ID IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_MBMS_FLOW_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_MBMS_FLOW_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesnMBMSESNUPDREQGmEntry */


/***********************************************************************

   Func  : egMakesnMBMSESNUPDRSPGmEntry

   Desc  : This function makes the GM Entry for MBMSESNUPDRSP

   Ret   : None

   File  : smegsm.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesnMBMSESNUPDRSPGmEntry
(
LegMsgEntry               *gmMBMSESNUPDRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesnMBMSESNUPDRSPGmEntry(gmMBMSESNUPDRSPCb)
LegMsgEntry               *gmMBMSESNUPDRSPCb;
#endif
{
   U8 ieCnt = 0;
   TRC2(egMakesnMBMSESNUPDRSPGmEntry);

   /* Populate the MBMSESNUPDRSP GM Entry */
   gmMBMSESNUPDRSPCb->msgType = EG_SN_MBMSESNUPDRSP_CMD_CODE;
   gmMBMSESNUPDRSPCb->nmbIeProp = EG_SN_MBMSESNUPDRSP_NMB_IES;
   gmMBMSESNUPDRSPCb->validateIe = (EscFun)NULLP;

   /* Populate MBMSESNUPDRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNUPDRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_DIST_ACK IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDRSPCb->ieProperties[ieCnt],
                    EG_MBMS_DIST_ACK_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNUPDRSPCb->posIndex[0][EG_MBMS_DIST_ACK_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDRSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNUPDRSPCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDRSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNUPDRSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMBMSESNUPDRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesnMBMSESNUPDRSPGmEntry */
/***********************************************************************

   Func  : egMakesnGmDict

   Desc  : This functions populates eGTP Messages in GM Dictionary

   Ret   : EG_ROK

   File  : smegsm.c

************************************************************************/


#ifdef ANSI
PUBLIC S16 egMakesnGmDict
(
LegMsgEntry *gmEntry,
U8        *nmbGmMsgs
)
#else
PUBLIC S16 egMakesnGmDict(gmEntry,nmbGmMsgs)
LegMsgEntry *gmEntry;
U8        *nmbGmMsgs;
#endif
{
   U8  gmMsgCount;                  /* Number of eGTP Messages inserted */
   gmMsgCount = 0;

   TRC2(egMakesnGmDict);

   /* Populating MBMSESNSTOPREQ GM Entry */
   egMakesnMBMSESNSTOPREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MBMSESNSTOPRSP GM Entry */
   egMakesnMBMSESNSTOPRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MBMSESNSTRTREQ GM Entry */
   egMakesnMBMSESNSTRTREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MBMSESNSTRTRSP GM Entry */
   egMakesnMBMSESNSTRTRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MBMSESNUPDREQ GM Entry */
   egMakesnMBMSESNUPDREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MBMSESNUPDRSP GM Entry */
   egMakesnMBMSESNUPDRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
   *nmbGmMsgs = gmMsgCount;
   RETVALUE(ROK);
} /* End of geMakesnGmDict */

#endif /* EG_REL_930 */
#endif /* EGTP_C */

/********************************************************************30**

         End of file:     eg_dbsn.c@@/main/1 - Fri Feb  4 18:18:09 2011

*********************************************************************31*/

/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**

    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
/main/1      ---      an                1. Created for Initial Release 1.1
/main/2      ---      kchaitanya        1. Initial for eGTP 1.2 Release
            eg004.102 rrm               1. FTEID IE in create sess req/rsp made conditional
/main/1      ---      pmacharla       Initial Release of eGTP 2.1 release
*********************************************************************91*/

