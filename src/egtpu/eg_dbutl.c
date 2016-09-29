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


/***********************************************************************

    Name: Source file for filling LM configuration

    Type: C Source

    Desc: 

    File: eg_dbutl.c

    Sid:      eg_dbutl.c@@/main/3 - Fri Feb  4 18:18:09 2011

    Prg : an

************************************************************************/

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
#include "eg_edm.h"           /* EDM MODULE */
#include "eg_dbutl.h"         /* Application MODULE */
#include "eg_util.h"          /* Application MODULE */

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
#include "eg_edm.x"        /* EDM MODULE */
#include "eg_dbutl.x"      /* Application MODULE */
#include "eg_util.x"       /* Application MODULE */


#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

#ifdef EGTP_C
/************************************************************************
*
*  Func  : egMakeIeEntry
*
*  Desc  : This function populates the IE Properties in IE
           Dictionary.
*
*  Return: No Return value
*
*  File  : eg_dbutl.c
*
*************************************************************************/
#ifdef ANSI
PUBLIC Void egMakeIeEntry
(
LegIeEntry      *ieEntry,
U8             ieCode,
Bool           valid,
U8             dataType,
Ptr            fncPtr,
U16             minLen,
U16             maxLen
)
#else
PUBLIC Void egMakeIeEntry(ieEntry,ieCode,valid, dataType, fncPtr,minLen, maxLen)
LegIeEntry    *ieEntry;
U8             ieCode;
Bool           valid;
U8             dataType;
Ptr            fncPtr;
U16             minLen;
U16             maxLen;
#endif
{
   TRC2(egMakeIeEntry)

   /* Populate the IE Entry */
   ieEntry->ieType  = ieCode;
   ieEntry->dataType = dataType;
   ieEntry->minLen = minLen;
   ieEntry->maxLen = maxLen;
   /*-- eg003.201: changes from eg006.102: Added RETVOID--*/
   RETVOID;

} /* End of egMakeIeEntry */

/************************************************************************
*
*  Func  : egMakeAppChildIeEntry
*
*  Desc  : This function populates the Application IEs Properties
*          of a grouped IE
*
*  Return: No Return value
*
*  File  : eg_dbutl.c
*
*************************************************************************/

#ifdef ANSI
PUBLIC Void egMakeAppChildIeEntry
(
LegGroupedIe *grpIe,
U8             count,
U8            ieCode,
U8            ieClass,
U8            ieInst,
U8            ieList,
U8             nmbGrpIe
)
#else
PUBLIC Void egMakeAppChildIeEntry(grpIe,count,ieCode,ieClass,
                               ieInst, ieList, nmbGrpIe)
LegGroupedIe *grpIe;
U8             count;
U8            ieCode;
U8            ieClass;
U8            ieInst;
U8            ieList;
U8             nmbGrpIe;
#endif
{
   TRC2(egMakeAppChildIeEntry)

   /* If it is a grouped IE the value of nmbGrpIe is greater than 0 */
   if(nmbGrpIe > 0)
   {
      grpIe->nmbGrpIe   =   nmbGrpIe;
      cmMemset((U8* )grpIe->ieProperties, 0,nmbGrpIe*sizeof(LegIeProperties)); 
   } /* End of if statement */

   /* Populate the IE Entry */
   if( count < grpIe->nmbGrpIe )
   {
      grpIe->ieProperties[count].ieType  = ieCode;
      grpIe->ieProperties[count].ieClass  = ieClass;
      grpIe->ieProperties[count].ieInst  = ieInst;
      grpIe->ieProperties[count].ieList   = ieList;
      grpIe->posIndex[ieInst][ieCode] = count+1;
   }
   else
   {
      /* Trying to access array out of bounds */
   }
   /*-- eg003.201: changes from eg006.102: Added RETVOID--*/
   RETVOID;

} /* End of egMakeAppChildIeEntry */



/**********************************************************************************
*
*     Func  : egMakeAppIeProp
*
*     Desc  : This function populates the IE Properties within the eGTP message
*
*     Return: None
*
*     File  : eg_dbutl.c
*
**************************************************************************/


#ifdef ANSI
PUBLIC Void egMakeAppIeProp
(
LegIeProperties *ieProp,     /* Ie Property to be filled */
U8             ieCode,      /* Ie Code */
U8             ieClass,     /* Mandatory/Conditional/Optional */
U8             instance,     /* Instance  */
U8             ieList     /* Comprehension Required */
)
#else
PUBLIC Void egMakeAppIeProp(ieProp, ieCode, ieClass , instance, ieList)
LegIeProperties *ieProp;
U8             ieCode;         /*  IE Code  */
U8             ieClass;        /* Mandatory/Conditional/Optional */
U8             instance;       /* Instance  */
U8           ieList;             /* Comprehension Required */
#endif
{
   TRC2(egMakeAppIeProp)

   /* Populate the IE Properties */
   ieProp->ieType   =  ieCode;
   ieProp->ieClass  =  ieClass;
   ieProp->ieList   =  ieList;
   ieProp->ieInst =  instance;
   /*-- eg003.201: changes from eg006.102: Added RETVOID--*/
   RETVOID;

} /* End of aqMakeAppIeProp */

/***********************************************************************

   Func  : egMakeIeDict

   Desc  : This function populates the IEs in IE Dictionary

   Ret   : EG_ROK/EG_RFAILED

   File  : smegsm.c

************************************************************************/


#ifdef ANSI
PUBLIC S16 egMakeIeDict
(
LegIeEntry *legIeEntry,
U8      *nmbIe,
U16      *posIdx
)
#else
PUBLIC S16 egMakeIeDict(legIeEntry,nmbIe, posIdx)
LegIeEntry *legIeEntry;
U8      *nmbIe;
U16      *posIdx;
#endif
{
   U8  ieCount;                  /* Number of IEs inserted */
   U8  prevIndex;                            /* Previous Index  */

   ieCount = 0;
   prevIndex = 0;
   TRC2(egMakeIeDict); 
   /*-- eg009.201: Corrected Min IE length value for IE to 7
    *              EG_IMSI_IETYPE --*/
   /* Populating EG_IMSI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_IMSI_IETYPE,
                    TRUE ,EG_STRING_16 ,(NULL) ,7,8);
   posIdx[EG_IMSI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PROCEDURE_TID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PROCEDURE_TID_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_PROCEDURE_TID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_DRX_PARAM_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_DRX_PARAM_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_DRX_PARAM_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_UE_NTWRK_CAPB_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UE_NTWRK_CAPB_IETYPE,
                    TRUE ,EG_STRING_16 ,(NULL) ,1,16);
   posIdx[EG_UE_NTWRK_CAPB_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_GSM_KEY_TRIP_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_GSM_KEY_TRIP_IETYPE,
                    TRUE ,EG_GSM_KEY_TRIP ,(NULL) ,78,219);
   posIdx[EG_GSM_KEY_TRIP_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_UMTS_KEY_CIPH_QUIN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UMTS_KEY_CIPH_QUIN_IETYPE,
                    TRUE ,EG_UMTS_KEY_CIPH_QUIN ,(NULL) ,40,153);
   posIdx[EG_UMTS_KEY_CIPH_QUIN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_GSM_KEY_CIPH_QUIN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_GSM_KEY_CIPH_QUIN_IETYPE,
                    TRUE ,EG_GSM_KEY_CIPH_QUIN ,(NULL) ,16,129);
   posIdx[EG_GSM_KEY_CIPH_QUIN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_UMTS_KEY_QUIN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UMTS_KEY_QUIN_IETYPE,
                    TRUE ,EG_UMTS_KEY_QUIN ,(NULL) ,37,224);
   posIdx[EG_UMTS_KEY_QUIN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_EPS_SEC_QUAD_QUIN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_EPS_SEC_QUAD_QUIN_IETYPE,
                    TRUE ,EG_EPS_SEC_QUAD_QUIN ,(NULL) ,45,350);
   posIdx[EG_EPS_SEC_QUAD_QUIN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_UMTS_KEY_QUAD_QUIN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UMTS_KEY_QUAD_QUIN_IETYPE,
                    TRUE ,EG_UMTS_KEY_QUAD_QUIN ,(NULL) ,40,219);
   posIdx[EG_UMTS_KEY_QUAD_QUIN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PDN_CONN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PDN_CONN_IETYPE,
                    TRUE ,EG_GROUPED ,(NULL) ,2,2000);
   posIdx[EG_PDN_CONN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PDU_NUMBER_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PDU_NUMBER_IETYPE,
                    TRUE ,EG_PDUNUM ,(NULL) ,1,16);
   posIdx[EG_PDU_NUMBER_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PTMSI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PTMSI_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,3,3);
   posIdx[EG_PTMSI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PTMSI_SIG_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PTMSI_SIG_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,3,3);
   posIdx[EG_PTMSI_SIG_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_HOP_CNTR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_HOP_CNTR_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_HOP_CNTR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_UE_TIME_ZONE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UE_TIME_ZONE_IETYPE,
                    TRUE ,EG_UE_TIME_ZONE,(NULL) ,2,7);
   posIdx[EG_UE_TIME_ZONE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TRACE_REF_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TRACE_REF_IETYPE,
                    TRUE ,EG_TRACE_REF,(NULL) ,6,6);
   posIdx[EG_TRACE_REF_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_COMP_REQ_MSG_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_COMP_REQ_MSG_IETYPE,
                    TRUE ,EG_CMP_REQ ,(NULL) ,1,1000);
   posIdx[EG_COMP_REQ_MSG_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_GUTI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_GUTI_IETYPE,
                    TRUE ,EG_GUTI ,(NULL) ,6,10);
   posIdx[EG_GUTI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_F_CONTAINER_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_F_CONTAINER_IETYPE,
                    TRUE ,EG_F_CONT ,(NULL) ,1,1000);
   posIdx[EG_F_CONTAINER_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_F_CAUSE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_F_CAUSE_IETYPE,
                    TRUE ,EG_F_CAUSE ,(NULL) ,1,1000);
   posIdx[EG_F_CAUSE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SEL_PLMN_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SEL_PLMN_ID_IETYPE,
                    TRUE ,EG_STRING_8 ,(NULL) ,5,6);
   posIdx[EG_SEL_PLMN_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TARGET_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TARGET_ID_IETYPE,
                    TRUE ,EG_TRGTID ,(NULL) ,8,9);
   posIdx[EG_TARGET_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PKT_FLOW_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PKT_FLOW_ID_IETYPE,
                    TRUE ,EG_PKTFLOWID ,(NULL) ,9,19);
   posIdx[EG_PKT_FLOW_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_RAB_CONTEXT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_RAB_CONTEXT_IETYPE,
                    TRUE ,EG_RAB_CONTEXT ,(NULL) ,9,19);
   posIdx[EG_RAB_CONTEXT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SRC_RNC_PDCP_CTX_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SRC_RNC_PDCP_CTX_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,1,2000);
   posIdx[EG_SRC_RNC_PDCP_CTX_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PORT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PORT_IETYPE,
                    TRUE ,EG_UNSIGNED_16 ,(NULL) ,2,2);
   posIdx[EG_PORT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_APN_RESTRICT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_APN_RESTRICT_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_APN_RESTRICT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SEL_MODE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SEL_MODE_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_SEL_MODE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SRC_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SRC_ID_IETYPE,
                    TRUE ,EG_SRCID ,(NULL) ,14,14);
   posIdx[EG_SRC_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CNG_RPT_ACT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CNG_RPT_ACT_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_CNG_RPT_ACT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CSID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CSID_IETYPE,
                    TRUE ,EG_PDN_CONN_SET_ID ,(NULL) ,16,22);
   posIdx[EG_CSID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CHNL_NEEDED_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CHNL_NEEDED_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_CHNL_NEEDED_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_EMLPP_PRIOR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_EMLPP_PRIOR_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_EMLPP_PRIOR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_NODE_TYPE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_NODE_TYPE_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_NODE_TYPE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_FQDN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_FQDN_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,1,256);
   posIdx[EG_FQDN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TRANS_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TRANS_ID_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_TRANS_ID_IETYPE] = ieCount+1; 
   ieCount++;
#ifdef EG_REL_930 
   /* Populating EG_MBMS_SESS_DURATION_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MBMS_SESS_DURATION_IETYPE,
                    TRUE ,EG_UNSIGNED_32,(NULL) ,3,4);
   posIdx[EG_MBMS_SESS_DURATION_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MBMS_SERV_AREA_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MBMS_SERV_AREA_IETYPE,
                    TRUE ,EG_STRING_MAX,(NULL) ,1,513);
   posIdx[EG_MBMS_SERV_AREA_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MBMS_SESS_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MBMS_SESS_ID_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_MBMS_SESS_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MBMS_FLOW_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MBMS_FLOW_ID_IETYPE,
                    TRUE ,EG_UNSIGNED_16 ,(NULL) ,2,2);
   posIdx[EG_MBMS_FLOW_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MBMS_IP_MULT_DIST_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MBMS_IP_MULT_DIST_IETYPE,
                    TRUE ,EG_MBMS_IPMCAST_DIST ,(NULL) ,12,256);
   posIdx[EG_MBMS_IP_MULT_DIST_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MBMS_DIST_ACK_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MBMS_DIST_ACK_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_MBMS_DIST_ACK_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_UCI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UCI_IETYPE,
                    TRUE ,EG_UCI ,(NULL) ,12,12);
   posIdx[EG_UCI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CSG_INFO_REP_ACT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CSG_INFO_REP_ACT_IETYPE,
                    TRUE ,EG_CSG_INFO_REP_ACT,(NULL) ,1,1);
   posIdx[EG_CSG_INFO_REP_ACT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TMGI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TMGI_IETYPE,
                    TRUE ,EG_STRING_32 ,(NULL) ,3,4);
   posIdx[EG_TMGI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CSG_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CSG_ID_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_CSG_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CMI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CMI_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_CMI_IETYPE] = ieCount+1; 
   ieCount++;
#endif  /* EG_REL_930 */
   /* Populating EG_RFSP_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_RFSP_ID_IETYPE,
                    TRUE ,EG_UNSIGNED_16 ,(NULL) ,2,2);
   posIdx[EG_RFSP_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SERV_IND_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SERV_IND_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_SERV_IND_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CAUSE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CAUSE_IETYPE,
                    TRUE ,EG_CAUSE ,(NULL) ,6,6);
   posIdx[EG_CAUSE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PRIVATE_EXT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PRIVATE_EXT_IETYPE,
                    TRUE ,EG_PVT_EXTN ,(NULL) ,3,134);
   posIdx[EG_PRIVATE_EXT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_RECOVERY_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_RECOVERY_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_RECOVERY_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_APN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_APN_IETYPE,
                    TRUE ,EG_STRING_132 ,(NULL) ,1,100);
   posIdx[EG_APN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_AMBR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_AMBR_IETYPE,
                    TRUE ,EG_AMBR ,(NULL) ,8,8);
   posIdx[EG_AMBR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_EBI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_EBI_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_EBI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_IPADDR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_IPADDR_IETYPE,
                    TRUE ,EG_IP_ADDR ,(NULL) ,4,16);
   posIdx[EG_IPADDR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MEI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MEI_IETYPE,
                    TRUE ,EG_STRING_16 ,(NULL) ,15,16);
   posIdx[EG_MEI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MSISDN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MSISDN_IETYPE,
                    TRUE ,EG_STRING_32 ,(NULL) ,1,32);
   posIdx[EG_MSISDN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_INDICATION_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_INDICATION_IETYPE,
                    TRUE ,EG_INDICATION ,(NULL) ,2,2);
   posIdx[EG_INDICATION_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PCO_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PCO_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,3,256);
   posIdx[EG_PCO_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PAA_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PAA_IETYPE,
                    TRUE ,EG_PAA_IEDATA ,(NULL) ,5,22);
   posIdx[EG_PAA_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_BEARER_QOS_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_BEARER_QOS_IETYPE,
                    TRUE ,EG_BQOS_IEDATA ,(NULL) ,22,22);
   posIdx[EG_BEARER_QOS_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_FLOW_QOS_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_FLOW_QOS_IETYPE,
                    TRUE ,EG_FQOS ,(NULL) ,21,21);
   posIdx[EG_FLOW_QOS_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_RAT_TYPE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_RAT_TYPE_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_RAT_TYPE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SERVING_NWK_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SERVING_NWK_IETYPE,
                    TRUE ,EG_SNETWORK ,(NULL) ,3,3);
   posIdx[EG_SERVING_NWK_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_BEARER_TFT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_BEARER_TFT_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,3,257);
   posIdx[EG_BEARER_TFT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TAD_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TAD_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,3,257);
   posIdx[EG_TAD_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_ULI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_ULI_IETYPE,
                    TRUE ,EG_ULI_DATA ,(NULL) ,8,34);
   posIdx[EG_ULI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_FTEID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_FTEID_IETYPE,
                    TRUE ,EG_FTEID ,(NULL) ,9,25);
   posIdx[EG_FTEID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TMSI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TMSI_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_TMSI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_GBL_CN_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_GBL_CN_ID_IETYPE,
                    TRUE ,EG_GLOBAL_CN_ID ,(NULL) ,7,7);
   posIdx[EG_GBL_CN_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_S103_PDN_FW_INFO_IETYPE */
   /* ***********************************************
    * There are two variables in this IE            *
    * IPV4/IPV6 + EPS Bearer Ids ( maximum of 10 )  *
    *************************************************/
#ifndef IPV6_SUPPORTED
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_S103_PDN_FW_INFO_IETYPE,
                    TRUE ,EG_S103_PDN_FW_INFO ,(NULL) ,15,25);
#else
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_S103_PDN_FW_INFO_IETYPE,
                    TRUE ,EG_S103_PDN_FW_INFO ,(NULL) ,27,37);
#endif /* IPV6_SUPPORTED */
   posIdx[EG_S103_PDN_FW_INFO_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_S1U_DATA_FW_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_S1U_DATA_FW_IETYPE,
                    TRUE ,EG_S1U_DATA_FW ,(NULL) ,10,22);
   posIdx[EG_S1U_DATA_FW_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_DELAY_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_DELAY_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_DELAY_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_BEARER_CNTX_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_BEARER_CNTX_IETYPE,
                    TRUE ,EG_GROUPED ,(NULL) ,2,2000);
   posIdx[EG_BEARER_CNTX_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CHARGING_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CHARGING_ID_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_CHARGING_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CHARGING_CHAR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CHARGING_CHAR_IETYPE,
                    TRUE ,EG_UNSIGNED_16 ,(NULL) ,2,2);
   posIdx[EG_CHARGING_CHAR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TRACE_INFO_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TRACE_INFO_IETYPE,
                    TRUE ,EG_TRC_INFO ,(NULL) ,34,86);
   posIdx[EG_TRACE_INFO_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_BEARER_FLAGS_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_BEARER_FLAGS_IETYPE,
                    TRUE ,EG_BEARER_FLAG ,(NULL) ,1,1);
   posIdx[EG_BEARER_FLAGS_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PDN_TYPE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PDN_TYPE_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_PDN_TYPE_IETYPE] = ieCount+1; 
   ieCount++;
/* eg009.201:Added for S2B and S2A */
#if defined(EG_S2B_SUPP) || defined(EG_S2A_SUPP)
   /* Populating EG_LDN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_LDN_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,1,400);
   posIdx[EG_LDN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_APCO_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_APCO_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,3,256);
   posIdx[EG_APCO_IETYPE] = ieCount+1; 
   ieCount++;
#endif/* end of EG_S2B_SUPP*/
   /* Return the number of IEs inserted  */
   *nmbIe = ieCount;
   RETVALUE(ROK);
}  /* End of egMakeIeDeict() */

#endif /* End of EGTP_C */

#ifdef EGTP_U
/************************************************************************
*
*  Func  : egUMakeIeEntry
*
*  Desc  : This function populates the IE Properties in IE
           Dictionary for GTP-U
*
*  Return: No Return value
*
*  File  : eg_dbutl.c
*
*************************************************************************/
#ifdef ANSI
PUBLIC Void egUMakeIeEntry
(
EgIeEntry      *ieEntry,
U8             ieCode,
Bool           tvType,
U8             dataType,
U16             minLen,
U16             maxLen
)
#else
PUBLIC Void egUMakeIeEntry(ieEntry, ieCode, tvType, dataType, minLen, maxLen)
EgIeEntry    *ieEntry;
U8             ieCode;
Bool           tvType;
U8             dataType;
U16             minLen;
U16             maxLen;
#endif
{
   TRC2(egUMakeIeEntry)

   /* Populate the IE Entry */
   ieEntry->ieCode  = ieCode;
   ieEntry->tvType  = tvType;
   ieEntry->dataType = dataType;
   ieEntry->minLen = minLen;
   ieEntry->maxLen = maxLen;
   /*-- eg003.201: changes from eg006.102: Added RETVOID--*/
   RETVOID;

} /* End of egUMakeIeEntry */

/**********************************************************************************
*
*     Func  : egUMakeAppIeProp
*
*     Desc  : This function populates the IE Properties within the eGTPi-U message
*
*     Return: None
*
*     File  : eg_dbutl.c
*
**************************************************************************/


#ifdef ANSI
PUBLIC Void egUMakeAppIeProp
(
EgUIeProperties *ieProp,     /* Ie Property to be filled */
U8             ieCode,      /* Ie Code */
U8             ieClass     /* Mandatory/Conditional/Optional */
)
#else
PUBLIC Void egUMakeAppIeProp(ieProp, ieCode, ieClass)
EgUIeProperties *ieProp;
U8             ieCode;         /*  IE Code  */
U8             ieClass;        /* Mandatory/Conditional/Optional */
#endif
{
   TRC2(egUMakeAppIeProp)

   /* Populate the IE Properties */
   ieProp->ieType   =  ieCode;
   ieProp->ieClass  =  ieClass;
   /*-- eg003.201: changes from eg006.102: Added RETVOID--*/
   RETVOID;

} /* End of egUMakeAppIeProp */





#endif /* End of EGTP_U */



#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */


/**********************************************************************
         End of file:     eg_dbutl.c@@/main/3 - Fri Feb  4 18:18:09 2011

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
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/3     eg003.201 psingh            1. Merged code from eg006.102 to Fix 
                                           TRACE5 macro related issue
/main/3     eg005.201 psingh            1.Corrected Max, Min IE length value
                                          for ie EG_IMSI_IETYPE
/main/3     eg009.201 asaurabh          1. For EG_IMSI_IETYPE updated Min IE 
                                           length value to 7
                                        2. Added for S2B and S2A .
*********************************************************************91*/

