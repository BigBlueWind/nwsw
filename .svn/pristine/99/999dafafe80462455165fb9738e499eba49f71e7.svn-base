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
                    9380, Carroll Park Drive
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

    File: smegsi.c

    Sid:      eg_dbsi.c@@/main/1 - Fri Feb  4 18:18:07 2011

    Prg: an

************************************************************************/


/***********************************************************************
    This file contains automatically generated code
************************************************************************/
/* Header Include Files (.h) */
#include "envopt.h"            /* Environment Options */
#include "envdep.h"            /* Environment Dependent */
#include "envind.h"            /* Environment Independent */
#include "gen.h"            /* General */
#include "ssi.h"            /* System Services */
#include "cm_tkns.h"         /* Common Tokens */
#include "cm_hash.h"         /* Common Hash List */
#include "cm_mblk.h"         /* Common memory */
#include "cm_llist.h"           /* Common Link List */
#include "cm5.h"               /* Common Structures */
#include "cm_inet.h"           /* Common IP network */
#include "cm_tpt.h"           /* Common Transport */
#include "hit.h"            /* TUCL layer */
#include "egt.h"            /* EU layer */
#include "leg.h"      /* layer manager */
#include "eg_util.h"      /* UTIL */
#include "eg_edm.h"      /* EDM MODULE */
#include "eg_dbsi.h"
#include "eg_dbutl.h"   /* Ie codes are here */

/* Header Include Files (.x) */

#include "gen.x"            /* General */
#include "ssi.x"            /* System Services */
#include "cm_tkns.x"         /* Common Tokens */
#include "cm_hash.x"         /* Common Hash List */
#include "cm_mblk.x"         /* Common memory */
#include "cm_llist.x"           /* Common Link List */
#include "cm5.x"               /* Common Structures */
#include "cm_inet.x"           /* Common IP network */
#include "cm_tpt.x"           /* Common Transport */
#include "hit.x"            /* TUCL layer */
#include "egt.x"            /* EU layer */
#include "leg.x"      /* layer manager */
#include "eg_util.x"      /* UTIL */
/*-- eg005.201 : Update with product header file --*/
#include "eg_edm.x"      /* EDM MODULE */
#include "eg_dbutl.x"      /* Application MODULE */

#include "eg_dbsi.x"



#ifdef EGTP_C
/***********************************************************************

   Func  : egMakesiCFGTFRTNLGmEntry

   Desc  : This function makes the GM Entry for CFGTFRTNL

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiCFGTFRTNLGmEntry
(
LegMsgEntry               *gmCFGTFRTNLCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiCFGTFRTNLGmEntry(gmCFGTFRTNLCb)
LegMsgEntry               *gmCFGTFRTNLCb;
#endif
{
   U8 ieCnt = 0;
   TRC2(egMakesiCFGTFRTNLGmEntry);

   /* Populate the CFGTFRTNL GM Entry */
   gmCFGTFRTNLCb->msgType = EG_SI_CFGTFRTNL_CMD_CODE;
   gmCFGTFRTNLCb->nmbIeProp = EG_SI_CFGTFRTNL_NMB_IES;
   gmCFGTFRTNLCb->validateIe = (EscFun)NULLP;

   /* Populate CFGTFRTNL IE Properties */

   /* Populate F_CONTAINER IE Properties */
   egMakeAppIeProp(&gmCFGTFRTNLCb->ieProperties[ieCnt],
                    EG_F_CONTAINER_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCFGTFRTNLCb->posIndex[0][EG_F_CONTAINER_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TARGET_ID IE Properties */
   egMakeAppIeProp(&gmCFGTFRTNLCb->ieProperties[ieCnt],
                    EG_TARGET_ID_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCFGTFRTNLCb->posIndex[0][EG_TARGET_ID_IETYPE]  = ieCnt + 1;

} /* End of egMakesiCFGTFRTNLGmEntry */


/***********************************************************************

   Func  : egMakesiCRTINDDATFWDTUNREQGmEntry

   Desc  : This function makes the GM Entry for CRTINDDATFWDTUNREQ

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiCRTINDDATFWDTUNREQGmEntry
(
LegMsgEntry               *gmCRTINDDATFWDTUNREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiCRTINDDATFWDTUNREQGmEntry(gmCRTINDDATFWDTUNREQCb)
LegMsgEntry               *gmCRTINDDATFWDTUNREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakesiCRTINDDATFWDTUNREQGmEntry);

   /* Populate the CRTINDDATFWDTUNREQ GM Entry */
   gmCRTINDDATFWDTUNREQCb->msgType = EG_SI_CRTINDDATFWDTUNREQ_CMD_CODE;
   gmCRTINDDATFWDTUNREQCb->nmbIeProp = EG_SI_CRTINDDATFWDTUNREQ_NMB_IES;
   gmCRTINDDATFWDTUNREQCb->validateIe = (EscFun)NULLP;

   /* Populate CRTINDDATFWDTUNREQ IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTINDDATFWDTUNREQCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTINDDATFWDTUNREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,TRUE);

   gmCRTINDDATFWDTUNREQCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_CRTINDDATFWDTUNREQ_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CRTINDDATFWDTUNREQ_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,2,FALSE,
0 );

   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,3,FALSE,
0 );

   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,4,FALSE,
0 );

   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,5,FALSE,
0 );

}

}
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTINDDATFWDTUNREQCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTINDDATFWDTUNREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesiCRTINDDATFWDTUNREQGmEntry */


/***********************************************************************

   Func  : egMakesiCRTSESREQGmEntry

   Desc  : This function makes the GM Entry for CRTSESREQ

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiCRTSESREQGmEntry
(
LegMsgEntry               *gmCRTSESREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiCRTSESREQGmEntry(gmCRTSESREQCb)
LegMsgEntry               *gmCRTSESREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakesiCRTSESREQGmEntry);

   /* Populate the CRTSESREQ GM Entry */
   gmCRTSESREQCb->msgType = EG_SI_CRTSESREQ_CMD_CODE;
   gmCRTSESREQCb->nmbIeProp = EG_SI_CRTSESREQ_NMB_IES;
   gmCRTSESREQCb->validateIe = (EscFun)NULLP;

   /* Populate CRTSESREQ IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MSISDN IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_MSISDN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_MSISDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SERVING_NWK IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_SERVING_NWK_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_SERVING_NWK_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RAT_TYPE IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_RAT_TYPE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_RAT_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate APN IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_APN_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_APN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SEL_MODE IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_SEL_MODE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_SEL_MODE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PDN_TYPE IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_PDN_TYPE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_PDN_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PAA IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_PAA_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_PAA_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate APN_RESTRICT IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_APN_RESTRICT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_APN_RESTRICT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,TRUE);

   gmCRTSESREQCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmCRTSESREQCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmCRTSESREQCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_CRTSESREQ_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CRTSESREQ_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_TFT_IETYPE,EG_IE_OPTIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,2,FALSE,
0 );

   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,3,FALSE,
0 );

   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,4,FALSE,
0 );
/*eg009.201 New FTEID Instances 5,6 introduced*/
#if defined (EG_S2A_SUPP) || defined (EG_S2B_SUPP)
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,5,FALSE,
0 );
   
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,6,FALSE,
0 );
#endif
}

}
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmCRTSESREQCb->posIndex[1][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmCRTSESREQCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmCRTSESREQCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_CRTSESREQ_BEARER_CNTX1_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CRTSESREQ_BEARER_CNTX1_GRP_NMB_IES);

   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

}

}
   ieCnt++;

   /* Populate TRACE_INFO IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_TRACE_INFO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_TRACE_INFO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmCRTSESREQCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;
   /*eg009.201:Added CSID instance 2,3*/
#if defined (EG_S2A_SUPP) || defined (EG_S2B_SUPP)
   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,2,FALSE);

   gmCRTSESREQCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;
   
   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,3,FALSE);

   gmCRTSESREQCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif   
   /* Populate UE_TIME_ZONE IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_UE_TIME_ZONE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_UE_TIME_ZONE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CHARGING_CHAR IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_CHARGING_CHAR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_CHARGING_CHAR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MEI IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_MEI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_MEI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmCRTSESREQCb->posIndex[1][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesiCRTSESREQGmEntry */


/***********************************************************************

   Func  : egMakesiCTXREQGmEntry

   Desc  : This function makes the GM Entry for CTXREQ

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiCTXREQGmEntry
(
LegMsgEntry               *gmCTXREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiCTXREQGmEntry(gmCTXREQCb)
LegMsgEntry               *gmCTXREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiCTXREQGmEntry);

   /* Populate the CTXREQ GM Entry */
   gmCTXREQCb->msgType = EG_SI_CTXREQ_CMD_CODE;
   gmCTXREQCb->nmbIeProp = EG_SI_CTXREQ_NMB_IES;
   gmCTXREQCb->validateIe = (EscFun)NULLP;

   /* Populate CTXREQ IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PTMSI IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_PTMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_PTMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PTMSI_SIG IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_PTMSI_SIG_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_PTMSI_SIG_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate COMP_REQ_MSG IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_COMP_REQ_MSG_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_COMP_REQ_MSG_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PORT IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_PORT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_PORT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RAT_TYPE IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_RAT_TYPE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_RAT_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate HOP_CNTR IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_HOP_CNTR_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_HOP_CNTR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SERVING_NWK IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_SERVING_NWK_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_SERVING_NWK_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate GUTI IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_GUTI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_GUTI_IETYPE]  = ieCnt + 1;

} /* End of egMakesiCTXREQGmEntry */


/***********************************************************************

   Func  : egMakesiDELPDNCONSETREQGmEntry

   Desc  : This function makes the GM Entry for DELPDNCONSETREQ

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiDELPDNCONSETREQGmEntry
(
LegMsgEntry               *gmDELPDNCONSETREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiDELPDNCONSETREQGmEntry(gmDELPDNCONSETREQCb)
LegMsgEntry               *gmDELPDNCONSETREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiDELPDNCONSETREQGmEntry);

   /* Populate the DELPDNCONSETREQ GM Entry */
   gmDELPDNCONSETREQCb->msgType = EG_SI_DELPDNCONSETREQ_CMD_CODE;
   gmDELPDNCONSETREQCb->nmbIeProp = EG_SI_DELPDNCONSETREQ_NMB_IES;
   gmDELPDNCONSETREQCb->validateIe = (EscFun)NULLP;

   /* Populate DELPDNCONSETREQ IE Properties */

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELPDNCONSETREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmDELPDNCONSETREQCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,2,FALSE);

   gmDELPDNCONSETREQCb->posIndex[2][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELPDNCONSETREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesiDELPDNCONSETREQGmEntry */


/***********************************************************************

   Func  : egMakesiDELPDNCONSETRSPGmEntry

   Desc  : This function makes the GM Entry for DELPDNCONSETRSP

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiDELPDNCONSETRSPGmEntry
(
LegMsgEntry               *gmDELPDNCONSETRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiDELPDNCONSETRSPGmEntry(gmDELPDNCONSETRSPCb)
LegMsgEntry               *gmDELPDNCONSETRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiDELPDNCONSETRSPGmEntry);

   /* Populate the DELPDNCONSETRSP GM Entry */
   gmDELPDNCONSETRSPCb->msgType = EG_SI_DELPDNCONSETRSP_CMD_CODE;
   gmDELPDNCONSETRSPCb->nmbIeProp = EG_SI_DELPDNCONSETRSP_NMB_IES;
   gmDELPDNCONSETRSPCb->validateIe = (EscFun)NULLP;

   /* Populate DELPDNCONSETRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmDELPDNCONSETRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELPDNCONSETRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesiDELPDNCONSETRSPGmEntry */


/***********************************************************************

   Func  : egMakesiFWDRELREQGmEntry

   Desc  : This function makes the GM Entry for FWDRELREQ

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiFWDRELREQGmEntry
(
LegMsgEntry               *gmFWDRELREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiFWDRELREQGmEntry(gmFWDRELREQCb)
LegMsgEntry               *gmFWDRELREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   LegIeProperties **ieProperties1;

   TRC2(egMakesiFWDRELREQGmEntry);

   /* Populate the FWDRELREQ GM Entry */
   gmFWDRELREQCb->msgType = EG_SI_FWDRELREQ_CMD_CODE;
   gmFWDRELREQCb->nmbIeProp = EG_SI_FWDRELREQ_NMB_IES;
   gmFWDRELREQCb->validateIe = (EscFun)NULLP;

   /* Populate FWDRELREQ IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PDN_CONN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_PDN_CONN_IETYPE,EG_IE_MANDATORY,0,TRUE);

   gmFWDRELREQCb->posIndex[0][EG_PDN_CONN_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmFWDRELREQCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmFWDRELREQCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_FWDRELREQ_PDN_CONN0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_APN_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_FWDRELREQ_PDN_CONN0_GRP_NMB_IES);

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_APN_RESTRICT_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_SEL_MODE_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_IPADDR_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_IPADDR_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FQDN_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,0,TRUE,
0 );


      grpCnt1 = 0;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe) ,sizeof(LegGroupedIe));
      if( ret == ROK && (gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe)!= NULLP)
      {

         ieProperties1 = &gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe->ieProperties;
         ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties1),(sizeof(LegIeProperties)*(EG_FWDRELREQ_BEARER_CNTX00_GRP_NMB_IES)));
         if( ret == ROK && *ieProperties1 != NULLP)
         {

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_FWDRELREQ_BEARER_CNTX00_GRP_NMB_IES);

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_BEARER_TFT_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_FTEID_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_F_CONTAINER_IETYPE,EG_IE_OPTIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_TRANS_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_BEARER_FLAGS_IETYPE,EG_IE_OPTIONAL,0,FALSE,
0 );

      }
   }
   grpCnt++;

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_AMBR_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CHARGING_CHAR_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CNG_RPT_ACT_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

}

}
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,1,FALSE);

   gmFWDRELREQCb->posIndex[1][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FQDN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_FQDN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_FQDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate GSM_KEY_TRIP IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_GSM_KEY_TRIP_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_GSM_KEY_TRIP_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_CIPH_QUIN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_CIPH_QUIN_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_UMTS_KEY_CIPH_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate GSM_KEY_CIPH_QUIN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_GSM_KEY_CIPH_QUIN_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_GSM_KEY_CIPH_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_QUIN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_QUIN_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_UMTS_KEY_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EPS_SEC_QUAD_QUIN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_EPS_SEC_QUAD_QUIN_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_EPS_SEC_QUAD_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_QUAD_QUIN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_QUAD_QUIN_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_UMTS_KEY_QUAD_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CONTAINER IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_F_CONTAINER_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_F_CONTAINER_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CONTAINER IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_F_CONTAINER_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmFWDRELREQCb->posIndex[1][EG_F_CONTAINER_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TARGET_ID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_TARGET_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_TARGET_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IPADDR IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_IPADDR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_IPADDR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IPADDR IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_IPADDR_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmFWDRELREQCb->posIndex[1][EG_IPADDR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CAUSE IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_F_CAUSE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_F_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CAUSE IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_F_CAUSE_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmFWDRELREQCb->posIndex[1][EG_F_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CONTAINER IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_F_CONTAINER_IETYPE,EG_IE_CONDITIONAL,2,FALSE);

   gmFWDRELREQCb->posIndex[2][EG_F_CONTAINER_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CAUSE IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_F_CAUSE_IETYPE,EG_IE_CONDITIONAL,2,FALSE);

   gmFWDRELREQCb->posIndex[2][EG_F_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SEL_PLMN_ID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_SEL_PLMN_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_SEL_PLMN_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TRACE_INFO IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_TRACE_INFO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_TRACE_INFO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RFSP_ID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_RFSP_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_RFSP_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RFSP_ID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_RFSP_ID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmFWDRELREQCb->posIndex[1][EG_RFSP_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SRC_ID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_SRC_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_SRC_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesiFWDRELREQGmEntry */


/***********************************************************************

   Func  : egMakesiIDENTREQGmEntry

   Desc  : This function makes the GM Entry for IDENTREQ

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiIDENTREQGmEntry
(
LegMsgEntry               *gmIDENTREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiIDENTREQGmEntry(gmIDENTREQCb)
LegMsgEntry               *gmIDENTREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiIDENTREQGmEntry);

   /* Populate the IDENTREQ GM Entry */
   gmIDENTREQCb->msgType = EG_SI_IDENTREQ_CMD_CODE;
   gmIDENTREQCb->nmbIeProp = EG_SI_IDENTREQ_NMB_IES;
   gmIDENTREQCb->validateIe = (EscFun)NULLP;

   /* Populate IDENTREQ IE Properties */

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PTMSI IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_PTMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_PTMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PTMSI_SIG IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_PTMSI_SIG_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_PTMSI_SIG_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IPADDR IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_IPADDR_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_IPADDR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PORT IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_PORT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_PORT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate HOP_CNTR IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_HOP_CNTR_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_HOP_CNTR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SERVING_NWK IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_SERVING_NWK_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_SERVING_NWK_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate GUTI IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_GUTI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_GUTI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate COMP_REQ_MSG IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_COMP_REQ_MSG_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_COMP_REQ_MSG_IETYPE]  = ieCnt + 1;

} /* End of egMakesiIDENTREQGmEntry */


/***********************************************************************

   Func  : egMakesiIDENTRSPGmEntry

   Desc  : This function makes the GM Entry for IDENTRSP

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiIDENTRSPGmEntry
(
LegMsgEntry               *gmIDENTRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiIDENTRSPGmEntry(gmIDENTRSPCb)
LegMsgEntry               *gmIDENTRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiIDENTRSPGmEntry);

   /* Populate the IDENTRSP GM Entry */
   gmIDENTRSPCb->msgType = EG_SI_IDENTRSP_CMD_CODE;
   gmIDENTRSPCb->nmbIeProp = EG_SI_IDENTRSP_NMB_IES;
   gmIDENTRSPCb->validateIe = (EscFun)NULLP;

   /* Populate IDENTRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate GSM_KEY_TRIP IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_GSM_KEY_TRIP_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_GSM_KEY_TRIP_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_CIPH_QUIN IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_CIPH_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_UMTS_KEY_CIPH_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate GSM_KEY_CIPH_QUIN IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_GSM_KEY_CIPH_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_GSM_KEY_CIPH_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_QUIN IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_UMTS_KEY_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EPS_SEC_QUAD_QUIN IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_EPS_SEC_QUAD_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_EPS_SEC_QUAD_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_QUAD_QUIN IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_QUAD_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_UMTS_KEY_QUAD_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TRACE_INFO IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_TRACE_INFO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_TRACE_INFO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesiIDENTRSPGmEntry */

#ifdef EG_REL_930
/***********************************************************************

   Func  : egMakesiMBMSESNSTRTREQGmEntry

   Desc  : This function makes the GM Entry for MBMSESNSTRTREQ

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiMBMSESNSTRTREQGmEntry
(
LegMsgEntry               *gmMBMSESNSTRTREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiMBMSESNSTRTREQGmEntry(gmMBMSESNSTRTREQCb)
LegMsgEntry               *gmMBMSESNSTRTREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiMBMSESNSTRTREQGmEntry);

   /* Populate the MBMSESNSTRTREQ GM Entry */
   gmMBMSESNSTRTREQCb->msgType = EG_SI_MBMSESNSTRTREQ_CMD_CODE;
   gmMBMSESNSTRTREQCb->nmbIeProp = EG_SI_MBMSESNSTRTREQ_NMB_IES;
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

} /* End of egMakesiMBMSESNSTRTREQGmEntry */
#endif /* EG_REL_930 */

/***********************************************************************

   Func  : egMakesiRANINFORLYGmEntry

   Desc  : This function makes the GM Entry for RANINFORLY

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiRANINFORLYGmEntry
(
LegMsgEntry               *gmRANINFORLYCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiRANINFORLYGmEntry(gmRANINFORLYCb)
LegMsgEntry               *gmRANINFORLYCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiRANINFORLYGmEntry);

   /* Populate the RANINFORLY GM Entry */
   gmRANINFORLYCb->msgType = EG_SI_RANINFORLY_CMD_CODE;
   gmRANINFORLYCb->nmbIeProp = EG_SI_RANINFORLY_NMB_IES;
   gmRANINFORLYCb->validateIe = (EscFun)NULLP;

   /* Populate RANINFORLY IE Properties */

   /* Populate F_CONTAINER IE Properties */
   egMakeAppIeProp(&gmRANINFORLYCb->ieProperties[ieCnt],
                    EG_F_CONTAINER_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmRANINFORLYCb->posIndex[0][EG_F_CONTAINER_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TARGET_ID IE Properties */
   egMakeAppIeProp(&gmRANINFORLYCb->ieProperties[ieCnt],
                    EG_TARGET_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmRANINFORLYCb->posIndex[0][EG_TARGET_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmRANINFORLYCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmRANINFORLYCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesiRANINFORLYGmEntry */


/***********************************************************************

   Func  : egMakesiRELCANCELREQGmEntry

   Desc  : This function makes the GM Entry for RELCANCELREQ

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiRELCANCELREQGmEntry
(
LegMsgEntry               *gmRELCANCELREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiRELCANCELREQGmEntry(gmRELCANCELREQCb)
LegMsgEntry               *gmRELCANCELREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiRELCANCELREQGmEntry);

   /* Populate the RELCANCELREQ GM Entry */
   gmRELCANCELREQCb->msgType = EG_SI_RELCANCELREQ_CMD_CODE;
   gmRELCANCELREQCb->nmbIeProp = EG_SI_RELCANCELREQ_NMB_IES;
   gmRELCANCELREQCb->validateIe = (EscFun)NULLP;

   /* Populate RELCANCELREQ IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmRELCANCELREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmRELCANCELREQCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CAUSE IE Properties */
   egMakeAppIeProp(&gmRELCANCELREQCb->ieProperties[ieCnt],
                    EG_F_CAUSE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmRELCANCELREQCb->posIndex[0][EG_F_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmRELCANCELREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmRELCANCELREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesiRELCANCELREQGmEntry */
/***********************************************************************

   Func  : egMakesiGmDict

   Desc  : This functions populates eGTP Messages in GM Dictionary

   Ret   : EG_ROK

   File  : smegsi.c

************************************************************************/


#ifdef ANSI
PUBLIC S16 egMakesiGmDict
(
LegMsgEntry *gmEntry,
U8        *nmbGmMsgs
)
#else
PUBLIC S16 egMakesiGmDict(gmEntry,nmbGmMsgs)
LegMsgEntry *gmEntry;
U8        *nmbGmMsgs;
#endif
{
   U8  gmMsgCount;                  /* Number of eGTP Messages inserted */
   gmMsgCount = 0;

   TRC2(egMakesiGmDict);

   /* Populating CFGTFRTNL GM Entry */
   egMakesiCFGTFRTNLGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTINDDATFWDTUNREQ GM Entry */
   egMakesiCRTINDDATFWDTUNREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTSESREQ GM Entry */
   egMakesiCRTSESREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CTXREQ GM Entry */
   egMakesiCTXREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELPDNCONSETREQ GM Entry */
   egMakesiDELPDNCONSETREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELPDNCONSETRSP GM Entry */
   egMakesiDELPDNCONSETRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating FWDRELREQ GM Entry */
   egMakesiFWDRELREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating IDENTREQ GM Entry */
   egMakesiIDENTREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating IDENTRSP GM Entry */
   egMakesiIDENTRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
#ifdef EG_REL_930
   /* Populating MBMSESNSTRTREQ GM Entry */
   egMakesiMBMSESNSTRTREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
#endif /* EG_REL_930 */
   /* Populating RANINFORLY GM Entry */
   egMakesiRANINFORLYGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RELCANCELREQ GM Entry */
   egMakesiRELCANCELREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
   *nmbGmMsgs = gmMsgCount;
   RETVALUE(ROK);
} /* End of geMakesiGmDict */
#endif /* EGTP_C */

/********************************************************************30**

         End of file:     eg_dbsi.c@@/main/1 - Fri Feb  4 18:18:07 2011

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
/main/1      ---      an                1. Initial Release
/main/2      ---      kchitanya         1. initial for egtp release 1.2
/main/1      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/1     eg005.201 psingh            1. Updated headers to product file
                                           eg_edm.x & eg_dbutl.x instead of
                                           sample header file smeg_apputl.x
/main/1     eg009.201 asaurabh          1. New FTEID Instances 5,6 introduced
                                        2. Added CSID instance 2,3 
*********************************************************************91*/

