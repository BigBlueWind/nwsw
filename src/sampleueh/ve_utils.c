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

     Name:     S1AP Layer

     Type:     C source file

     Desc:     C Source code for Utility functions 

     File:     ve_utils.c

     Sid:      ve_utils.c@@/main/2 - Wed Dec  1 08:44:41 2010

     Prg:      

**********************************************************************/
/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_inet.h"
#include "cm_tpt.h"

#include "nhu_asn.h"
#include "nhu.h"           /* NHU interface defines */
#include "pju.h"           /* PDCP data interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "ctf.h"           /* PHY RRM control Interface */
#include "szt.h"           /* S1AP RRM control Interface */
#ifdef EU_DAT_APP
#include "egt.h"           /* S1AP RRM control Interface */
#endif 
#include "lrg.h"           /* MAC RRM control Interface */



/* RRM related includes */
#include "ve_eut.h" 
#include "ve.h" 
#include "ve_sm_init.h" 
#include "lve.h"
#include "cz.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"       /*  Common UMTS includes */
#include "cm_lib.x"
#include "cm_inet.x"
#include "cm_tpt.x"

#include "nhu_asn.x"
#include "nhu.x"           /* NHU Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "ctf.x"           /* PHY RRM control Interface */
#include "szt.x"           /* S1AP RRM control Interface */
#include "czt.x"
#ifdef EU_DAT_APP
#include "egt.x"           /* S1AP RRM control Interface */
#endif
#include "lrg.x"           /* MAC RRM control Interface */

/* RRM related includes */
#include "lve.x" 
#include "ve_sm_init.x" 
#include "ve_eut.x" 
#include "ve.x" 
/* MSPD changes */
#ifndef MSPD
#include "ve_dflt.h"
/* include of pal related header file */
#include "ve_dflt_pal.x"
#else
#include "ve_dflt_ms.x"
#endif


#ifdef __cplusplus
EXTERN "C" {
#endif

EXTERN S16 veFillMeasInfo ARGS((PTR mem, NhuMeasConfig *measCfg));
EXTERN S16 veFillUeCapInfo ARGS((PTR mem, NhuUE_CapRAT_ContLst *ue_RadioAccessCapInfo, VeUeCb *ueCb));
EXTERN S16 veFillSrcRadRscDed ARGS((PTR mem, VeUeRABInfo *rbInfo, 
                                    NhuRadioResourceConfigDedicated *srcRadRsrcConfig));
EXTERN S16 veFillSecAlg ARGS((NhuSecAlgorithmConfig *sourceSecAlgorithmConfig));
EXTERN S16 veFillUeId ARGS((PTR mem, NhuC_RNTI *sourceUE_Identity));
EXTERN S16 veFillMIBInfo ARGS((PTR mem, NhuMasterInfoBlock *mibInfo));
EXTERN S16 veFillSIB1Info ARGS((PTR mem, NhuSysInfoBlockTyp1 *sibTyp1));
EXTERN S16 veFillSIB2Info ARGS((PTR mem, NhuSysInfoBlockTyp2 *sib2));
EXTERN S16 veFillAntennaInfo ARGS((NhuAntennaInfoCmn *antennaInfoCmn));
EXTERN S16 veFillDlCarrFreq ARGS((NhuARFCN_ValueEUTRA *sourceDl_CarrierFreq));
EXTERN S16 veMonEnbOvldTmrExpiry ARGS((void));
#ifdef __cplusplus
}
#endif

EXTERN U16 szElmSize[][SZT_MAX_PROC_ID];

PRIVATE U8  SzMsgId[SZT_MAX_PROC_ID][SZT_MAX_MSG_TYPE] =
{
   /* Handover preparation : Procedure Code : 0  */
   {  
      SZT_MSG_ID_HO_REQD,      /* Initiating Message */
      SZT_MSG_ID_HO_CMD,       /* Successful Outcome */
      SZT_MSG_ID_HO_PREP_FAIL, /* UnSuccessful Outcome */
   },
   /* Handover resource allocation : Procedure Code : 1 */
   {  
      SZT_MSG_ID_HO_REQ,     /* Initiating Message */
      SZT_MSG_ID_HO_REQ_ACK, /* Successful Outcome */
      SZT_MSG_ID_HO_FAIL,    /* UnSuccessful Outcome */
   },
   /* Handover notification : Procedure Code : 2 */
   {  
      SZT_MSG_ID_HO_NTFY, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,  /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,  /* UnSuccessful Outcome */
   },
   /* Path switch request : Procedure Code : 3 */
   {  
      SZT_MSG_ID_PATH_SWTCH_REQ,      /* Initiating Message */
      SZT_MSG_ID_PATH_SWTCH_REQ_ACK,  /* Successful Outcome */
      SZT_MSG_ID_PATH_SWTCH_REQ_FAIL, /* UnSuccessful Outcome */
   },
   /* Handover cancel : Procedure Code : 4 */
   {  
      SZT_MSG_ID_HO_CANCEL,     /* Initiating Message */
      SZT_MSG_ID_HO_CANCEL_ACK, /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,        /* UnSuccessful Outcome */
   },
   /* E-RAB Assignment : Procedure Code : 5  */
   {  
      SZT_MSG_ID_E_RAB_BRR_SETUP_REQ, /* Initiating Message */
      SZT_MSG_ID_E_RAB_BRR_SETUP_RSP, /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,            /* UnSuccessful Outcome */
   },
   /* E-RAB modification : Procedure Code : 6  */
   {  
      SZT_MSG_ID_E_RAB_BRR_MOD_REQ, /* Initiating Message */
      SZT_MSG_ID_E_RAB_BRR_MOD_RSP, /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,          /* UnSuccessful Outcome */
   },
   /* E-RAB release : Procedure Code : 7  */
   {
      SZT_MSG_ID_E_RAB_BRR_REL_CMD,  /* Initiating Message */
      SZT_MSG_ID_E_RAB_BRR_REL_CMPLT,/* Successful Outcome */
      SZT_MSG_ID_NO_MSG,           /* UnSuccessful Outcome */
   },
   /* E-RAB release request : Procedure Code : 8  */
   {
      SZT_MSG_ID_E_RAB_BRR_REL_REQ, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,          /* Successful Outcome */
      SZT_MSG_ID_NO_MSG           /* UnSuccessful Outcome */
   },
   /* Initial Context setup : Procedure Code : 9  */
   {
      SZT_MSG_ID_CXT_SETUP_REQ,      /* Initiating Message */
      SZT_MSG_ID_CXT_SETUP_RSP,      /* Successful Outcome */
      SZT_MSG_ID_INIT_CXT_SETUP_FAIL,/* UnSuccessful Outcome */
   },
   /* Paging : Procedure Code : 10  */
   {
      SZT_MSG_ID_PAGING, /* Initiating Message */
      SZT_MSG_ID_NO_MSG, /* Successful Outcome */
      SZT_MSG_ID_NO_MSG  /* UnSuccessful Outcome */
   },
   /* DL NAS transport  : Procedure Code : 11 */
   {
      SZT_MSG_ID_DL_NAS_TPT, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,     /* Successful Outcome */
      SZT_MSG_ID_NO_MSG      /* UnSuccessful Outcome */
   },
   /* Initial UE message : Procedure Code : 12  */
   {
      SZT_MSG_ID_INIT_UE_MSG, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,      /* Successful Outcome */
      SZT_MSG_ID_NO_MSG       /* UnSuccessful Outcome */
   },
   /* UL NAS transport  : Procedure Code : 13 */
   {
      SZT_MSG_ID_UL_NAS_TPT, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,     /* Successful Outcome */
      SZT_MSG_ID_NO_MSG      /* UnSuccessful Outcome */
   },
   /* Reset : Procedure Code : 14  */
   {
      SZT_MSG_ID_RESET,      /* Initiating Message */
      SZT_MSG_ID_RESET_ACK,  /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,     /* UnSuccessful Outcome */
   },
   /* Error indication : Procedure Code : 15 */
   {
      SZT_MSG_ID_ERR_IND, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,  /* Successful Outcome */
      SZT_MSG_ID_NO_MSG   /* UnSuccessful Outcome */
   },
   /* NAS non delivery indication : Procedure Code : 16 */
   {
      SZT_MSG_ID_NAS_NON_DEL_IND, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,          /* Successful Outcome */
      SZT_MSG_ID_NO_MSG           /* UnSuccessful Outcome */
   },
   /* Setup : Procedure Code : 17  */
   {
      SZT_MSG_ID_SETUP_REQ,  /* Initiating Message */
      SZT_MSG_ID_SETUP_RSP,  /* Successful Outcome */
      SZT_MSG_ID_SETUP_FAIL  /* UnSuccessful Outcome */
   },
   /* UE Context release request : Procedure Code : 18 */
   {
      SZT_MSG_ID_UE_CXT_REL_REQ, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,         /* Successful Outcome */
      SZT_MSG_ID_NO_MSG          /* UnSuccessful Outcome */
   },
   /* DL CDMA2000 Tunnelling : Procedure Code : 19  */
   {  
      SZT_MSG_ID_DNLNK_CDMA_TUNNL, /* Initiating Message */
      SZT_MSG_ID_NO_MSG, /* Successful Outcome */
      SZT_MSG_ID_NO_MSG, /* UnSuccessful Outcome */
   },
   /* UL CDMA2000 Tunnelling : Procedure Code : 20  */
   {  
      SZT_MSG_ID_UPLNK_CDMA_TUNNL, /* Initiating Message */
      SZT_MSG_ID_NO_MSG, /* Successful Outcome */
      SZT_MSG_ID_NO_MSG, /* UnSuccessful Outcome */
   },
   /* UE context modification : Procedure Code : 21  */
   {
      SZT_MSG_ID_UE_CXT_MOD_REQ, /* Initiating Message */
      SZT_MSG_ID_UE_CXT_MOD_RSP, /* Successful Outcome */
      SZT_MSG_ID_UE_CXT_MOD_FAIL /* UnSuccessful Outcome */
   },
   /* UE capability Info Indication : Procedure Code : 22  */
   {  
      SZT_MSG_ID_UE_CAP_INFO, /* Initiating Message */
      SZT_MSG_ID_NO_MSG, /* Successful Outcome */
      SZT_MSG_ID_NO_MSG, /* UnSuccessful Outcome */
   },
   /* UE context release : Procedure Code : 23  */
   {
      SZT_MSG_ID_UE_CXT_REL_CMD,  /* Initiating Message */
      SZT_MSG_ID_UE_CXT_REL_CMPL, /* Successful Outcome */
      SZT_MSG_ID_NO_MSG           /* UnSuccessful Outcome */
   },
   /* ENB status transfer : Procedure Code : 24  */
   {  
      SZT_MSG_ID_ENB_STA_TRFR, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,       /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,       /* UnSuccessful Outcome */
   },
   /* MME status transfer : Procedure Code : 25  */
   {  
      SZT_MSG_ID_MME_STA_TRFR, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,       /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,       /* UnSuccessful Outcome */
   },
   /* Deactivate trace : Procedure Code : 26  */
   {  
      SZT_MSG_ID_DEACTV_TRACE, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,       /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,       /* UnSuccessful Outcome */
   },
   /* Trace start : Procedure Code : 27  */
   {  
      SZT_MSG_ID_TRACE_START, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,      /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,      /* UnSuccessful Outcome */
   },
   /* Trace failure indication : Procedure Code : 28  */
   {  
      SZT_MSG_ID_TRACE_FAIL_IND, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,         /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,         /* UnSuccessful Outcome */
   },
   /* ENB configuration update : Procedure Code : 29  */
   {  
      SZT_MSG_ID_ENB_CFG_UPD,      /* Initiating Message */
      SZT_MSG_ID_ENB_CFG_UPD_ACK,  /* Successful Outcome */
      SZT_MSG_ID_ENB_CFG_UPD_FAIL, /* UnSuccessful Outcome */
   },
   /* MME configuration update : Procedure Code : 30  */
   {  
      SZT_MSG_ID_MME_CFG_UPD,      /* Initiating Message */
      SZT_MSG_ID_MME_CFG_UPD_ACK,  /* Successful Outcome */
      SZT_MSG_ID_MME_CFG_UPD_FAIL, /* UnSuccessful Outcome */
   },
   /* Location reporting control : Procedure Code : 31  */
   {  
      SZT_MSG_ID_LOC_REPORT_CNTRL, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,           /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,           /* UnSuccessful Outcome */
   },
   /* Location report failure indication : Procedure Code : 32  */
   {  
      SZT_MSG_ID_LOC_REPORT_FAIL_IND, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,              /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,              /* UnSuccessful Outcome */
   },
   /* Location report : Procedure Code : 33  */
   {  
      SZT_MSG_ID_LOC_REPORT, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,     /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,     /* UnSuccessful Outcome */
   },
   /* Overload Start : Procedure Code : 34  */
   {  
      SZT_MSG_ID_OVLD_START, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,     /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,     /* UnSuccessful Outcome */
   },
   /* Overload Stop : Procedure Code : 35  */
   {  
      SZT_MSG_ID_OVLD_STOP, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,     /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,     /* UnSuccessful Outcome */
   },
   /* Write Replace Warning : Procedure Code : 36  */
   {  
      SZT_MSG_ID_WARNING_REQ, /* Initiating Message */
      SZT_MSG_ID_WARNING_RSP,     /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,     /* UnSuccessful Outcome */
   },
   /* ENB Direct Information Transfer : Procedure Code : 37  */
   {  
      SZT_MSG_ID_ENB_DIR_TRANS, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,     /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,     /* UnSuccessful Outcome */
   },
   /* MME Direct Information Transfer : Procedure Code : 38  */
   {  
      SZT_MSG_ID_MME_DIR_TRANS, /* Initiating Message */
      SZT_MSG_ID_NO_MSG,     /* Successful Outcome */
      SZT_MSG_ID_NO_MSG,     /* UnSuccessful Outcome */
   }
};

#ifdef LTE_HO_SUPPORT
/** @var veCzX2MsgMap 
    This static table gives the mapping of X2AP message type's procedure code 
    and choice values to Trillium's X2AP message type representation.
    @note For a list of definitions of Trillium's X2AP message types refer
          to czt.h
 */
PUBLIC S16 veCzX2MsgMap[CZ_MAX_MSG_PROC_CODE][CZ_MAX_MSG_CHOICE_VAL] = 
{
  /******Message Type IE Value********************/       
  /* Message Procedure Code */  /**Message Type Choice*/   

  /* HO Preparation */
   {
   CZT_MSG_HO_REQ,           /* Initiating Message */
   CZT_MSG_HO_ACK,           /* Successful Outcome */ 
   CZT_MSG_HO_FAIL          /* Unsuccessful Outcome */  
   },

   /* HO Cancel */
   {
   CZT_MSG_HO_CANCEL,        /* Initiating Message */
   CZT_MSG_UNKNOWN,          /* Successful Outcome */ 
   CZT_MSG_UNKNOWN           /* Unsuccessful Outcome */  
   },

  /* Load Indication */
   {
   CZT_MSG_LOAD_IND,         /* Initiating Message */
   CZT_MSG_UNKNOWN,          /* Successful Outcome */ 
   CZT_MSG_UNKNOWN           /* Unsuccessful Outcome */  
   },

  /* Error Indication */
   {
   CZT_MSG_ERR_IND,          /* Initiating Message */
   CZT_MSG_UNKNOWN,          /* Successful Outcome */ 
   CZT_MSG_UNKNOWN           /* Unsuccessful Outcome */     
   },

  /* SN Status Transfer */
   {
   CZT_MSG_SN_STATUS_TRFR,  /* Initiating Message */
   CZT_MSG_UNKNOWN,         /* Successful Outcome */
   CZT_MSG_UNKNOWN          /* Unsuccessful Outcome */   
   },

  /* UE Ctxt Release */
   {
   CZT_MSG_UE_CXT_RELEASE,   /* Initiating Message */
   CZT_MSG_UNKNOWN,          /* Successful Outcome */
   CZT_MSG_UNKNOWN           /* Unsuccessful Outcome */   
   },

   /* X2 Setup */
   {
   CZT_MSG_X2_SETUP,        /* Initiating Message */
   CZT_MSG_X2_SETUP_RSP,    /* Successful Outcome */
   CZT_MSG_X2_SETUP_FAIL    /* Unsuccessful Outcome */   
   },

   /* Reset */
   {
   CZT_MSG_RESET,           /* Initiating Message */
   CZT_MSG_RESET_RSP,       /* Successful Outcome */
   CZT_MSG_UNKNOWN          /* Unsuccessful Outcome */ 
   },

   /* Config Update */
   {
   CZT_MSG_CFG_UPD_REQ,     /* Initiating Message */
   CZT_MSG_CFG_UPD_ACK,     /* Successful Outcome */
   CZT_MSG_CFG_UPD_FAIL     /* Unsuccessful Outcome */
   },

   /* Resource Status Initiation */
   {
   CZT_MSG_RSRC_ST_REQ,     /* Initiating Message */
   CZT_MSG_RSRC_ST_RSP,     /* Successful Outcome */
   CZT_MSG_RSRC_ST_FAIL     /* Unsuccessful Outcome */
   },

   /* Resource Update Reporting */
   {
   CZT_MSG_RSRC_ST_UPD,     /* Initiating Message */
   CZT_MSG_UNKNOWN,         /* Successful Outcome */
   CZT_MSG_UNKNOWN          /* Unsuccessful Outcome */
   },

   /* Private Message */
   {
   CZT_MSG_PRIV_MSG,        /* Initiating Message */
   CZT_MSG_UNKNOWN,         /* Successful Outcome */
   CZT_MSG_UNKNOWN          /* Unsuccessful Outcome */
   }
};

PUBLIC U16 veCzUtlGetElmSize[][CZ_MAX_MSG_PROC_CODE] = 
{
   {
      /* Msg Id 0 - Cztid_handoverPrep */
      sizeof(CztProtIE_Field_HovrRqst_IEs),
      /* Msg Id 1 - Cztid_handoverCancel */
      sizeof(CztProtIE_Field_HovrCancel_IEs),
      /* Msg Id 2 - Cztid_loadInd */
      sizeof(CztProtIE_Field_LoadInform_IEs),
      /* Msg Id 3 - Cztid_errorInd */
      sizeof(CztProtIE_Field_ErrInd_IEs),
      /* Msg Id 4 - Cztid_snStatusTfr */
      sizeof(CztProtIE_Field_SNStatusTfr_IEs),
      /* Msg Id 5 - Cztid_uECntxtRls */
      sizeof(CztProtIE_Field_UECntxtRls_IEs),
      /* Msg Id 6 - Cztid_x2Setup */
      sizeof(CztProtIE_Field_X2SetupRqst_IEs),
      /* Msg Id 7 - Cztid_reset */
      sizeof(CztProtIE_Field_ResetRqst_IEs),
      /* Msg Id 8 - Cztid_eNBConfigUpd */
      sizeof(CztProtIE_Field_ENBConfigUpd_IEs),
      /* Msg Id 9 - Cztid_resourceStatusRprtngInitiation */
      sizeof(CztProtIE_Field_ResStatusRqst_IEs),
      /* Msg Id 10 - Cztid_resourceStatusRprtng */
      sizeof(CztProtIE_Field_ResStatusUpd_IEs),
      /* Msg Id 11 - Cztid_privateMsg */
      sizeof(CztPrivIE_Field_PrivMsg_IEs)
   },

   {
      /* Msg Id 0 - Cztid_handoverPrep */
      sizeof(CztProtIE_Field_HovrRqstAckg_IEs),
      /* Msg Id 1 - Cztid_handoverCancel */
      0,
      /* Msg Id 2 - Cztid_loadInd */
      0,
      /* Msg Id 3 - Cztid_errorInd */
      0,
      /* Msg Id 4 - Cztid_snStatusTfr */
      0,
      /* Msg Id 5 - Cztid_uECntxtRls */
      0,
      /* Msg Id 6 - Cztid_x2Setup */
      sizeof(CztProtIE_Field_X2SetupResp_IEs),
      /* Msg Id 7 - Cztid_reset */
      sizeof(CztProtIE_Field_ResetResp_IEs),
      /* Msg Id 8 - Cztid_eNBConfigUpd */
      sizeof(CztProtIE_Field_ENBConfigUpdAckg_IEs),
      /* Msg Id 9 - Cztid_resourceStatusRprtngInitiation */
      sizeof(CztProtIE_Field_ResStatusResp_IEs),
      /* Msg Id 10 - Cztid_resourceStatusRprtng */
      0,
      /* Msg Id 11 - Cztid_privateMsg */
      0
   },

   {
      /* Msg Id 0 - Cztid_handoverPrep */
      sizeof(CztProtIE_Field_HovrPrepFail_IEs),
      /* Msg Id 1 - Cztid_handoverCancel */
      0,
      /* Msg Id 2 - Cztid_loadInd */
      0,
      /* Msg Id 3 - Cztid_errorInd */
      0,
      /* Msg Id 4 - Cztid_snStatusTfr */
      0,
      /* Msg Id 5 - Cztid_uECntxtRls */
      0,
      /* Msg Id 6 - Cztid_x2Setup */
      sizeof(CztProtIE_Field_X2SetupFail_IEs),
      /* Msg Id 7 - Cztid_reset */
      0,
      /* Msg Id 8 - Cztid_eNBConfigUpd */
      sizeof(CztProtIE_Field_ENBConfigUpdFail_IEs),
      /* Msg Id 9 - Cztid_resourceStatusRprtngInitiation */
      sizeof(CztProtIE_Field_ResStatusFail_IEs),
      /* Msg Id 10 - Cztid_resourceStatusRprtng */
      0,
      /* Msg Id 11 - Cztid_privateMsg */
      0
   }
};



#endif /* End of LTE_HO_SUPPORT */
/*
 *
 *      Fun:   veCpyCmTptAddr
 *
 *      Desc:  Copy Addr.
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veCpyCmTptAddr
(
CmTptAddr *dst,         /* pointer to the destination CmTptAddr. */
CmTptAddr *src          /* pointer to the source CmTptAddr. */
)
#else
PUBLIC S16 veCpyCmTptAddr (dst, src)
CmTptAddr *dst;         /* pointer to the destination CmTptAddr. */
CmTptAddr *src;         /* pointer to the source CmTptAddr. */
#endif
{
   TRC2(veCpyCmTptAddr);

   if((src == NULLP) || (dst == NULLP))
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veCpyCmTptAddr: Src & Dst Invalid\n"));
#endif
      RETVALUE(RFAILED);
   }

   dst->type = src->type;
   if(src->type == CM_INET_IPV4ADDR_TYPE)
   {
      dst->u.ipv4TptAddr.port = src->u.ipv4TptAddr.port;
      dst->u.ipv4TptAddr.address = src->u.ipv4TptAddr.address;
   }
   else if(src->type == CM_INET_IPV6ADDR_TYPE)
   {
      dst->u.ipv6TptAddr.port = src->u.ipv6TptAddr.port;
      cmMemcpy((U8*)dst->u.ipv6TptAddr.ipv6NetAddr, (U8*)src->u.ipv6TptAddr.ipv6NetAddr,
            sizeof(CmInetIpAddr6));
   }

   RETVALUE( ROK);
}


/*
 *
 *      Fun:   veSztUtilsGetMsgIdx
 *
 *      Desc:  Retrieve message index
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSztUtilsGetMsgIdx
(
U8 *msgIdx,
S1apPdu *pdu
)
#else
PUBLIC S16 veSztUtilsGetMsgIdx(msgIdx,pdu)
U8 *msgIdx;
S1apPdu *pdu;
#endif
{
   U8 msgType;
   U8 procCode;

   TRC2(veSztUtilsGetMsgIdx);

   if(pdu->pdu.choice.pres == TRUE)
   {
      msgType  = pdu->pdu.choice.val;
   }
   else
   {
      *msgIdx = SZT_MSG_ID_NO_DATA;
      RETVALUE(ROK);
   }
   switch(msgType)
   {
      case S1AP_PDU_INITIATINGMSG:
         if(pdu->pdu.val.initiatingMsg.pres.pres == TRUE)
         { /* klock warning fix */
            procCode = (U8)pdu->pdu.val.initiatingMsg.procedureCode.val;
            *msgIdx = SzMsgId[procCode][msgType];
         }
         else 
         { 
            *msgIdx = SZT_MSG_ID_NO_MSG;
         }
         break;
      case S1AP_PDU_SUCCESSFULOUTCOME:
         if(pdu->pdu.val.successfulOutcome.pres.pres == TRUE)
         {  /* klock warning fix */
            procCode = (U8)pdu->pdu.val.successfulOutcome.procedureCode.val;
            *msgIdx = SzMsgId[procCode][msgType];
         }
         else
         {
            *msgIdx = SZT_MSG_ID_NO_MSG;
         }
         break;
      case S1AP_PDU_UNSUCCESSFULOUTCOME:
         if(pdu->pdu.val.unsuccessfulOutcome.pres.pres == TRUE)
         {  /* klock warning fix */
            procCode = (U8)pdu->pdu.val.unsuccessfulOutcome.procedureCode.val;
            *msgIdx = SzMsgId[procCode][msgType];
         }
         else
         {
            *msgIdx = SZT_MSG_ID_NO_MSG;
         }
         break;
   }
   RETVALUE(ROK);
}
#ifdef LTE_HO_SUPPORT
/*
 *
 *      Fun:   veCztUtilsGetMsgIdx
 *
 *      Desc:  Retrieve X2AP message index
 *
 *      Ret:   czMsgVal - message value - ok
 *             CZT_MSG_UNKNOWN          - failure
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC U8 veCztUtilsGetMsgIdx
(
CztX2AP_PDU   *pdu
)
#else
PUBLIC U8 veCztUtilsGetMsgIdx(pdu)
CztX2AP_PDU   *pdu;
#endif
{
   S16 czMsgVal = CZT_MSG_UNKNOWN;
   S16 msgChoice = -1;
   U8  procCode;

   TRC2(veCztUtilsGetMsgIdx)

   if(pdu == NULLP)
      RETVALUE(RFAILED);

   if(pdu->choice.pres == TRUE)
   {
      msgChoice  = pdu->choice.val;
   }
   switch(msgChoice)
   {
      case X2AP_PDU_INITIATINGMSG:
      {
         if(TRUE == pdu->val.initiatingMsg.pres.pres)
         {
            procCode = pdu->val.initiatingMsg.procedureCode.val;
            czMsgVal = veCzX2MsgMap[procCode][msgChoice];
         }
      }
      break;
      case X2AP_PDU_SUCCESSFULOUTCOME:
      {
         if(TRUE == pdu->val.successfulOutcome.pres.pres)
         {
            procCode = pdu->val.successfulOutcome.procedureCode.val;
            czMsgVal = veCzX2MsgMap[procCode][msgChoice];
         }
         break;
      }
      case X2AP_PDU_UNSUCCESSFULOUTCOME:
      {
         if(TRUE == pdu->val.unsuccessfulOutcome.pres.pres)
         {
            procCode = pdu->val.unsuccessfulOutcome.procedureCode.val;
            czMsgVal = veCzX2MsgMap[procCode][msgChoice];
         }
         break;
      }
      default:
      break;
   }

   RETVALUE(czMsgVal);
}/* veCztUtilsGetMsgIdx */

/*
 *
 *      Fun:   veCztUtilsGetMsgIdx
 *
 *      Desc:  Retrieve X2AP message index
 *
 *      Ret:   czMsgVal - message value - ok
 *             CZT_MSG_UNKNOWN          - failure
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veUtlGetCztIE
(
CztX2AP_PDU *pdu,
U32         id,
TknU8       **ie
)
#else
PUBLIC S16 veUtlGetCztIE (pdu, id, ie)
CztX2AP_PDU *pdu;
U32         id;
TknU8       **ie;
#endif
{
   TknU8   *currStr;   /* Start definition */
   TknU8   *startStr;   /* Start event structure */
   U8      choiceType;
   U16      numComp;
   U16      recIdx;
   U32      recId;
   U32      procCode;
   U16      size;

   TRC2(veUtlGetCztIE)

   /* Initialize the return pointer Tkn to NOTPRSNT */
   *ie = NULLP;

   numComp = 0;
   recIdx = 0; 

   if(pdu->choice.pres != PRSNT_NODEF)
   {
      RETVALUE(ROK);
   }
   choiceType = pdu->choice.val;

   /* Make start string point to the message in the PDU   */
   switch(choiceType)
   {
      case X2AP_PDU_INITIATINGMSG:
         {
            if (pdu->val.initiatingMsg.pres.pres != PRSNT_NODEF)
            {
               RETVALUE(ROK);
            }
            startStr  = (TknU8 *)(&(pdu->val.initiatingMsg.value.u));
            procCode  = pdu->val.initiatingMsg.procedureCode.val;
            break;
         }
      case X2AP_PDU_SUCCESSFULOUTCOME:
         {
            if (pdu->val.successfulOutcome.pres.pres != PRSNT_NODEF)
            { 
               RETVALUE(ROK);
            }
            startStr  = (TknU8 *)(&(pdu->val.successfulOutcome.value.u));
            procCode  = pdu->val.successfulOutcome.procedureCode.val;
            break;
         } 
      default: 
         {
            if ((pdu->val.unsuccessfulOutcome.pres.pres != PRSNT_NODEF) ||
                  (choiceType != X2AP_PDU_UNSUCCESSFULOUTCOME))
            {
               RETVALUE(ROK);
            }

            startStr  = (TknU8 *)(&(pdu->val.unsuccessfulOutcome.value.u));
            procCode  = pdu->val.unsuccessfulOutcome.procedureCode.val;
            break;
         }
   }

   if (((TknU8 *)startStr)->pres == PRSNT_NODEF)
   {
      /* Increment it by TknU8 for TknPres */
      currStr  = (TknU8 *) ((PTR)startStr + sizeof(TknU8));
   }
   else
   {
      RETVALUE(ROK);
   }

   if (((TknU16 *)(currStr))->pres == PRSNT_NODEF)
   {
      /* get the Number of IEs received : noComp */
      numComp = ((TknU16 *)(currStr))->val;
   }

   /*  Increment currStr by TknU16 for skipping noComp */
   currStr = (TknU8 *)((PTR)currStr + sizeof(TknU16));

   currStr = *(TknU8**)currStr;

   while (recIdx < numComp) 
   {
      startStr = currStr;

      if (((TknPres *)currStr)->pres == PRSNT_NODEF)
      {   
         /* Increment it by TknU8 for TknPres */
         currStr  = (TknU8 *) ((PTR)currStr + sizeof(TknPres));

         /* Get the received Id  */
         recId   = ((CztProtIE_ID *)currStr)->val; 

         /* Compare the two Ids */
         if (recId == id)
         {
            startStr = (TknU8 *) ((PTR)currStr + sizeof(CztProtIE_ID) + sizeof(CztCriticality));
            *ie = startStr;
            break;
         }
      }
      size = veCzUtlGetElmSize[choiceType][procCode];
      currStr  = (TknU8 *) ((PTR)startStr + size);
      recIdx++;
   }

   RETVALUE(ROK);
} /* end of veUtlGetCztIE */
/*
 *
 *      Fun:   veUtlSelMmeCb
 *
 *      Desc:  Retrieve MME Cb depending on GUMMEID
 *
 *      Ret:   mmeCb - if successful
 *             NULLP - if failure
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veUtlSelMmeCb
(
CztGUMMEI    *ie,
VeMmeCb     **mmeCb
)
#else
PUBLIC S16 veUtlSelMmeCb (ie, mmeCb)
CztGUMMEI    *ie;
VeMmeCb     **mmeCb;
#endif
{
   VeMmeCb            *tmpMmeCb = NULLP;
   U16                 idx, idx1;
   CztPLMN_Identity   *plmn = &(ie->gU_Group_ID.pLMN_Identity);
   VePlmnId            rcvPlmn;
   S8                  ret = FALSE;

   veGetPlmnVal(&rcvPlmn, plmn);

   for(idx = 0; idx < VE_MAX_MMES; idx++)
   {
      tmpMmeCb = veCb.mmeCont.mmes[idx];

      if((tmpMmeCb != NULLP) && (tmpMmeCb->state == VE_S1_SETUP_DONE))
      {
         for(idx1 = 0; idx1 < tmpMmeCb->numPlmnIds; idx1++)
         {
            ret = veCmpPlmnId(&rcvPlmn, &(tmpMmeCb->plmnIds[idx1]));
            if(ret == TRUE)
            {
               *mmeCb = tmpMmeCb;
               break;
            }
         }
      }
   }

   RETVALUE(ROK);
}

#endif /* End of LTE_HO_SUPPORT */

/*
 *
 *      Fun:   veUtlSelMmeCbByMmec
 *
 *      Desc:  Retrieve MME Cb depending on MMEC Code value
 *
 *      Ret:   mmeCb - if successful
 *             NULLP - if failure
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veUtlSelMmeCbByMmec
(
U8          mmec,
VeMmeCb     **mmeCb
)
#else
PUBLIC S16 veUtlSelMmeCbByMmec (mmec, mmeCb)
U8          mmec;
VeMmeCb     **mmeCb;
#endif
{
   VeMmeCb            *tmpMmeCb = NULLP;
   U16                 idx, idx1;


   for(idx = 0; idx < VE_MAX_MMES; idx++)
   {
      tmpMmeCb = veCb.mmeCont.mmes[idx];

      if((tmpMmeCb != NULLP) && (tmpMmeCb->state == VE_S1_SETUP_DONE))
      {
         for(idx1 = 0; idx1 < tmpMmeCb->numCodes; idx1++)
         {
            if(tmpMmeCb->codes[idx1] == mmec)
            {
               *mmeCb = tmpMmeCb;
               break;
            }
         }
      }
   }

   RETVALUE(ROK);
}
/*                                     
*
*       Fun:   veFillTknU8
*
*       Desc:  Fills  the TknU8 structure with the specified value
*
*       Ret:   ROK       - ok
*
*       Notes: None
*
*       File:  ve_utils.c
*
*/
#ifdef ANSI
PUBLIC S16  veFillTknU8
(
TknU8  *encParam,
U8     val
)
#else
PUBLIC S16 veFillTknU8(encParam, val)
TknU8  *encParam;
U8     val;
#endif
{
   TRC2(veFillTknU8);
   encParam->pres = PRSNT_NODEF;
   encParam->val  = val;

   RETVALUE(ROK);
} /* end of veFillTknU8 */

/*                                     
*
*       Fun:   veFillTknU16
*
*       Desc:  Fill a TknU16 structure
*
*       Ret:   ROK       - ok
*
*       Notes: None
*
*       File:  sz_msgfn.c
*
*/

#ifdef ANSI
PUBLIC S16  veFillTknU16
(
TknU16  *encParam,
U16     val
)
#else
PUBLIC S16 veFillTknU16(encParam, val)
TknU16  *encParam;
U16     val;
#endif
{
   TRC2(veFillTknU16);

   encParam->pres = PRSNT_NODEF;
   encParam->val  = val;

   RETVALUE(ROK);
} /* end of veFillTknU16 */

/*                                     
*
*       Fun:   veFillTknU32
*
*       Desc:  Fills the TknU32 structure
*
*       Ret:   RA_ROK       - ok
*
*       Notes: None
*
*       File:  ve_utils.c
*
*/

#ifdef ANSI
PUBLIC S16  veFillTknU32
(
TknU32  *encParam,
U32     val
)
#else
PUBLIC S16 veFillTknU32(encParam, val)
TknU32  *encParam;
U32     val;
#endif
{
   TRC2(veFillTknU32);

   encParam->pres = PRSNT_NODEF;
   encParam->val  = val;

   RETVALUE(ROK);
} /* end of veFillTknU32 */


#ifdef ANSI
PUBLIC S16 veFillTknBStr32
(
TknBStr32  *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
U32         val      /* value to be filled in */
)
#else
PUBLIC S16 veFillTknBStr32(ptr, len, val)
TknBStr32  *ptr;  /* Pointer to the structure */
U16         len;  /* length to be filled */
U32         val;  /* value to be filled in */
#endif
{
   TRC2(veFillTknBStr32);

   if (len > 32)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillTknBStr32: Invalid length\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len = (U8) len;

   ptr->val[0] = (U8)(((val << (32 - len)) & 0xFF000000) >> 24);
   ptr->val[1] = (U8)(((val << (32 - len)) & 0x00FF0000) >> 16);
   ptr->val[2] = (U8)(((val << (32 - len)) & 0x0000FF00) >> 8);
   ptr->val[3] = ((val << (32 - len)) & 0x000000FF);

   RETVALUE(ROK);
} /* veFillTknBStr32 */

/* MACRO_ENB: Start */
/*
 *
 *      Fun:   veFillEnbId
 *
 *      Desc:  Fill eNB Id
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veFillEnbId
(
TknBStr32  *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
U32         val      /* value to be filled in */
)
#else
PUBLIC S16 veFillEnbId(ptr, len, val)
TknBStr32  *ptr;  /* Pointer to the structure */
U16         len;  /* length to be filled */
U32         val;  /* value to be filled in */
#endif
{
   U8       idx;
   U8       octets[4];

   TRC2(veFillEnbId);

   if (len > 32)
   {
      RETVALUE(RFAILED);
   }

   octets[0] = (U8)(((val << (32 - len)) & 0xff000000) >> 24);
   octets[1] = (U8)(((val << (32 - len)) & 0x00ff0000) >> 16);
   octets[2] = (U8)(((val << (32 - len)) & 0x0000ff00) >> 8);
   octets[3] = ((val << (32 - len)) & 0x000000ff);
   ptr->pres = TRUE;
   ptr->len  = (U8)len;
   for(idx = 0; idx < 4; idx++)
   {
      ptr->val[idx] = octets[idx];
   }

   RETVALUE(ROK);

} /* veFillEnbId */
/* MACRO_ENB: End */

/*
 *
 *      Fun:   veFillTknStrOSXL
 *
 *      Desc:  Fill TknStrOSXL
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veFillTknStrOSXL
(
TknStrOSXL *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
Data       *val,     /* byte to be repetitively filled in */
S1apPdu    **pdu
)
#else
PUBLIC S16 veFillTknStrOSXL(ptr, len, val, pdu)
TknStrOSXL *ptr;  /* Pointer to the structure */
U16         len;  /* lenrah to be filled */
Data       *val;  /* byte to be repetitively filled in */
S1apPdu    **pdu;
#endif
{
   U16     idx;     /* counter */

   TRC2(veFillTknStrOSXL);

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len =  len;

   if (cmGetMem(pdu[0], len * sizeof(U8), (Ptr *)&ptr->val) != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF, "veFillTknStrOSXL: \
               cmGetMem failed, line %d\n",__LINE__));
#endif
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < len; idx++)
   {
      ptr->val[idx] = val[idx];
   }

   RETVALUE(ROK);
}
/* selva MSPD FIX */
/*
 *
 *      Fun:   veFillTknStrBSXL
 *
 *      Desc:  Fill TknStrBSXL
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veFillTknStrBSXL
(
TknStrBSXL *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
Data       *val,     /* byte to be repetitively filled in */
S1apPdu    **pdu
)
#else
PUBLIC S16 veFillTknStrBSXL(ptr, len, val, pdu)
TknStrBSXL *ptr;  /* Pointer to the structure */
U16         len;  /* lenrah to be filled */
Data       *val;  /* byte to be repetitively filled in */
S1apPdu    **pdu;
#endif
{
   S16     idx = 0;     /* counter */
   U16     numbytes = 0;
   TRC2(veFillTknStrOSXL);

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len =  len;
   numbytes = (len > 8)?(len/8):1;
   if(len % 8)
   numbytes++;
   if (cmGetMem(pdu[0], numbytes, (Ptr *)&ptr->val) != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF, "veFillTknStrOSXL: \
               cmGetMem failed, line %d\n",__LINE__));
#endif
      RETVALUE(RFAILED);
   }
   for(idx = (numbytes-1); idx >= 0; idx--)
   {
      ptr->val[idx] = *val++;
   }

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 veFillTknStrOSXL1
(
TknStrOSXL *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
U32         val,     /* byte to be repetitively filled in */
S1apPdu    **pdu
)
#else
PUBLIC S16 veFillTknStrOSXL1(ptr, len, val, pdu)
TknStrOSXL *ptr;  /* Pointer to the structure */
U16         len;  /* length to be filled */
U32         val;  /* byte to be repetitively filled in */
S1apPdu    **pdu;
#endif
{
   U16     idx;     /* counter */
   U8      tmpVal;

   TRC2(veFillTknStrOSXL1);

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len =  len;

   if (cmGetMem(pdu[0], len * sizeof(U8), (Ptr *)&ptr->val) != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF, "veFillTknStrOSXL: \
               cmGetMem failed, line %d\n",__LINE__));
#endif
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < len; idx++)
   {
      tmpVal = (U8)(val >> (8 * idx));
      ptr->val[len - (idx + 1)] = tmpVal;

   }

   RETVALUE(ROK);
}
/*
*
*       Fun:   veFillTknBitStr4
*
*       Desc:  Call handler to fill Bit string of specified length 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  szac_sztutl.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillTknBitStr4
(
TknStr4 *ptr,     /* Pointer to the structure */
U8      len,      /* length to be filled */
U8      *val       /* byte to be repetitively filled in */
)
#else
PUBLIC S16 veFillTknBitStr4(ptr, len, val)
TknStr4 *ptr;     /* Pointer to the structure */
U8      len;      /* length to be filled */
U8      *val;      /* byte to be repetitively filled in */
#endif
{
   U8      idx;     /* counter */
   U8      lenInBytes = 0;

   TRC2(veFillTknBitStr4);

   /* make the element as present */
   ptr->pres = TRUE;

   if (len > 32)
   {
      RETVALUE(RFAILED);
   }
   else
   {
      ptr->len = (U8) len;
   }

   if((len % 8) == 0 )
   {
     lenInBytes = (len/8);
   }
   else
   {
     lenInBytes = (len/8) + 1;
   }
     
   for(idx = 0; idx < lenInBytes; idx++)
   {
      ptr->val[idx] = val[idx];
   }

   RETVALUE(ROK);
} /* end of veFillTknBitStr4 */



/*
*
*       Fun:   veFillTknStr4
*
*       Desc:  Call handler to fill Octet string of length less than 4
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  szac_sztutl.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillTknStr4
(
TknStr4 *ptr,     /* Pointer to the structure */
U8      len,      /* length to be filled */
U8      val       /* byte to be repetitively filled in */
)
#else
PUBLIC S16 veFillTknStr4(ptr, len, val)
TknStr4 *ptr;     /* Pointer to the structure */
U8      len;      /* length to be filled */
U8      val;      /* byte to be repetitively filled in */
#endif
{
   U8      idx;     /* counter */

   TRC2(veFillTknStr4);

   /* make the element as present */
   ptr->pres = TRUE;

   if (len > 4)
   {
      RETVALUE(RFAILED);
   }
   else
   {
      ptr->len = (U8) len;
   }

   for(idx = 0; idx < len; idx++)
   {
      ptr->val[idx] = val;
   }

   RETVALUE(ROK);
} /* end of veFillTknStr4 */
/*
*
*       Fun:   veFill2TknStr4
*
*       Desc:  Call handler to fill Octet string of length less than 4
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  szac_sztutl.c
*
*/
#ifdef ANSI
PUBLIC S16 veFill2TknStr4
(
TknStr4 *ptr,     /* Pointer to the structure */
U8      len,      /* length to be filled */
U8      *val       /* byte to be repetitively filled in */
)
#else
PUBLIC S16 veFill2TknStr4(ptr, len, val)
TknStr4 *ptr;     /* Pointer to the structure */
U8      len;      /* length to be filled */
U8      *val;      /* byte to be repetitively filled in */
#endif
{
   U8      idx;     /* counter */

   TRC2(veFill2TknStr4);

   /* make the element as present */
   ptr->pres = TRUE;

   if (len > 4)
   {
      RETVALUE(RFAILED);
   }
   else
   {
      ptr->len = (U8) len;
   }

   for(idx = 0; idx < len; idx++)
   {
      ptr->val[idx] = val[idx];
   }

   RETVALUE(ROK);
} /* end of veFillTknStr4 */
#ifdef ANSI
PUBLIC S16 veMonEnbOvldTmrExpiry
(
)
#else
PUBLIC S16 veMonEnbOvldTmrExpiry()
#endif
{
  Status  memStatus = 0;
  S16        retVal = 0;               /* Return Value */
  U8        ovldFlag = 0;
  S16       veMemCritThrldVal = 1;
  S16       veMemMajThrldVal = 4;
  S16       veMemMinThrldVal = 7;
  U32       maxCntVal = 3;
  retVal = SChkRes(veCb.init.region, veCb.init.pool, &memStatus);
  
  if (retVal != ROK)
  {
     #ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veUtils: getting memory statistics failed \n"));
     #endif
  }
 

  if (memStatus <= veMemCritThrldVal)
  {
     if (veCb.eNBOvldParams.eNBOvldFlag != VE_CRITICAL)
     {
        veCb.eNBOvldParams.ovldCntVal++;
        ovldFlag = VE_CRITICAL;
     }
  }
  else if ((memStatus <= veMemMajThrldVal) && (memStatus >= veMemCritThrldVal))
  {
     if (veCb.eNBOvldParams.eNBOvldFlag != VE_MAJOR)
     {
        veCb.eNBOvldParams.ovldCntVal++;
        ovldFlag = VE_MAJOR;
     }
  }
  else if ((memStatus <= veMemMinThrldVal) && (memStatus >= veMemMajThrldVal))
  {
     if (veCb.eNBOvldParams.eNBOvldFlag != VE_MINOR)
     {
        veCb.eNBOvldParams.ovldCntVal++;
        ovldFlag = VE_MINOR;
     }
  }
  else
  {
     if (veCb.eNBOvldParams.eNBOvldFlag != VE_OVLDNORMAL)
     {
        veCb.eNBOvldParams.ovldCntVal++;
        ovldFlag = VE_OVLDNORMAL;
     }
  }
  if ( veCb.eNBOvldParams.ovldCntVal == maxCntVal)
  {
      veCb.eNBOvldParams.ovldCntVal = 0;
      veCb.eNBOvldParams.eNBOvldFlag = ovldFlag;
  }
  
  RETVALUE(ROK);
}

#ifdef LTE_HO_SUPPORT
/*
 *
 *      Fun:   veCzFillTknStrOSXL
 *
 *      Desc:  Fill TknStrOSXL
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veCzFillTknStrOSXL
(
TknStrOSXL  *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
Data        *val,     /* byte to be repetitively filled in */
CztEvnt **pdu
)
#else
PUBLIC S16 veCzFillTknStrOSXL(ptr, len, val, pdu)
TknStrOSXL  *ptr;  /* Pointer to the structure */
U16         len;  /* lenrah to be filled */
Data        *val;  /* byte to be repetitively filled in */
CztEvnt **pdu;
#endif
{
   U16     idx;     /* counter */

   TRC2(veCzFillTknStrOSXL);

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len =  len;

   if (cmGetMem(pdu[0], len * sizeof(U8), (Ptr *)&ptr->val) != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF, "veCzFillTknStrOSXL: \
               cmGetMem failed, line %d\n",__LINE__));
#endif
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < len; idx++)
   {
      ptr->val[idx] = val[idx];
   }

   RETVALUE(ROK);
}

/*
 *
 *      Fun:   veCzFillTknStrBSXL
 *
 *      Desc:  Fill TknStrBSXL
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veCzFillTknStrBSXL
(
TknStrBSXL  *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
Data        *val,     /* byte to be repetitively filled in */
S1apPdu **pdu
)
#else
PUBLIC S16 veCzFillTknStrBSXL(ptr, len, val, pdu)
TknStrBSXL  *ptr;  /* Pointer to the structure */
U16         len;  /* lenrah to be filled */
Data        *val;  /* byte to be repetitively filled in */
S1apPdu **pdu;
#endif
{
   U16     idx;     /* counter */

   TRC2(veCzFillTknStrBSXL);

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len =  len;

   if (cmGetMem(pdu[0], len * sizeof(U8), (Ptr *)&ptr->val) != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF, "veCzFillTknStrBSXL: \
               cmGetMem failed, line %d\n",__LINE__));
#endif
      RETVALUE(RFAILED);
   }
   len = len/8;
   len = ((len%8) != 0) ? (len +1):len;
   for(idx = 0; idx < len; idx++)
   {
      ptr->val[idx] = val[idx];
   }

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 veCzFillTknStrOSXL1
(
TknStrOSXL  *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
U32         val,     /* byte to be repetitively filled in */
CztEvnt     **pdu
)
#else
PUBLIC S16 veCzFillTknStrOSXL1(ptr, len, val, pdu)
TknStrOSXL  *ptr;  /* Pointer to the structure */
U16         len;  /* length to be filled */
U32         val;  /* byte to be repetitively filled in */
CztEvnt **pdu;
#endif
{
   U16     idx;     /* counter */
   U8      tmpVal;

   TRC2(veCzFillTknStrOSXL1);

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len =  len;

   if (cmGetMem(pdu[0], len * sizeof(U8), (Ptr *)&ptr->val) != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF, "veCzFillTknStrOSXL: \
               cmGetMem failed, line %d\n",__LINE__));
#endif
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < len; idx++)
   {
      tmpVal = (U8)(val >> (8 * idx));
      ptr->val[len - (idx + 1)] = tmpVal;

   }

   RETVALUE(ROK);
}
#endif

#ifndef LTE_ENB_PERF

/*
 *
 *      Fun:   veSzGetIE
 *
 *      Desc:  Retrieve message index
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSzGetIE
(
 SztS1AP_PDU *pdu,
 U32     id,
 TknU8   **ie
)
#else
PUBLIC S16 veSzGetIE (pdu, id, ie)
 SztS1AP_PDU *pdu;
 U32     id;
 TknU8   **ie;
#endif
{
   TknU8   *currStr;    /* Start definition */
   TknU8   *startStr;   /* Start event structure */
   U8      choiceType;
   U16     numComp;
   U16     recIdx;
   U32     recId;
   U32     procCode;
   U16     size;

   TRC2(veSzGetIE);

   /* Initialize the return pointer Tkn to NOTPRSNT */
   *ie = NULLP;

   numComp = 0;
   recIdx = 0; 

   if(pdu->choice.pres != PRSNT_NODEF)
   {
      RETVALUE(ROK);
   }

   choiceType = pdu->choice.val;

   /* Make start string point to the message in the PDU   */
   switch(choiceType)
   {
      case S1AP_PDU_INITIATINGMSG:
         if (pdu->val.initiatingMsg.pres.pres != PRSNT_NODEF)
            RETVALUE(ROK);

         startStr  = (TknU8 *)(&(pdu->val.initiatingMsg.value.u));
         procCode  = pdu->val.initiatingMsg.procedureCode.val;
         break;

      case S1AP_PDU_SUCCESSFULOUTCOME:
         if (pdu->val.successfulOutcome.pres.pres != PRSNT_NODEF)
            RETVALUE(ROK);

         startStr  = (TknU8 *)(&(pdu->val.successfulOutcome.value.u));
         procCode  = pdu->val.successfulOutcome.procedureCode.val;
         break;

      default: 
         if ((pdu->val.unsuccessfulOutcome.pres.pres != PRSNT_NODEF) ||
               (choiceType != S1AP_PDU_UNSUCCESSFULOUTCOME))
            RETVALUE(ROK);

         startStr  = (TknU8 *)(&(pdu->val.unsuccessfulOutcome.value.u));
         procCode  = pdu->val.unsuccessfulOutcome.procedureCode.val;
         break;
   }

   if (((TknU8 *)startStr)->pres == PRSNT_NODEF)
   {
      /* Increment it by TknU8 for TknPres */
      currStr  = (TknU8 *) ((PTR)startStr + sizeof(TknU8));
   }
   else
      RETVALUE(ROK);

   if (((TknU16 *)(currStr))->pres == PRSNT_NODEF)
   {
      /* get the Number of IEs received : noComp */
      numComp = ((TknU16 *)(currStr))->val;
   }

   /*  Increment currStr by TknU16 for skipping noComp */
   currStr = (TknU8 *)((PTR)currStr + sizeof(TknU16));

   currStr = *(TknU8**)currStr;

   while (recIdx < numComp) 
   {
      startStr = currStr;

      if (((TknPres *)currStr)->pres == PRSNT_NODEF)
      {   
         /* Increment it by TknU8 for TknPres */
         currStr  = (TknU8 *) ((PTR)currStr + sizeof(TknPres));

         /* Get the received Id  */
         recId  = ((SztProtIE_ID *)currStr)->val; 

         /* Compare the two Ids */
         if (recId == id)
         {
            startStr = (TknU8 *) ((PTR)currStr + sizeof(SztProtIE_ID) + sizeof(SztCriticality));

            *ie = startStr;
            break;
         }
      }

      size = szElmSize[choiceType][procCode];
      currStr  = (TknU8 *) ((PTR)startStr + size);
      recIdx++;
   }

   RETVALUE(ROK);
} /* end of veSzGetIE */
#endif
/*
 *
 *      Fun:   veInitUeSrbInfo
 *
 *      Desc:  Populate VeUeRABInfo Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_utils.c 
 *
 */
#ifndef VE_SRB2_SUPPORT
#ifdef ANSI
PUBLIC S16 veInitUeSrbInfo
(
VeUeRABInfo *srbCfg
)
#else
PUBLIC S16 veInitUeSrbInfo(srbCfg)
VeUeRABInfo *srbCfg;
#endif
{
   U8 idx = 0;
   
   TRC2(veInitUeSrbInfo);

   if(NULLP == srbCfg)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF, "veInitUeSrbInfo: \
               srbCfg is NULL."));
#endif
      RETVALUE(RFAILED);
   }

   /* Cfg only SRB1 for now */
   srbCfg->numOfSrbCfgd++;
   srbCfg->srb[idx].rbType = CM_LTE_SRB;
   srbCfg->srb[idx].rbId = VE_PDCP_ID_SRB1;
   srbCfg->srb[idx].pdcpId = VE_PDCP_ID_SRB1;

   RETVALUE(ROK);

} /* veInitUeSrbInfo */


#else
#ifdef ANSI
PUBLIC S16 veInitUeSrbInfo
(
VeUeRABInfo *srbCfg,
U8           rabId
)
#else
PUBLIC S16 veInitUeSrbInfo(srbCfg, rabId)
VeUeRABInfo *srbCfg;
U8            rabId;
#endif
{
   U8 idx;
   
   TRC2(veInitUeSrbInfo);

   if(NULLP == srbCfg)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veInitUeSrbInfo:\
         srbCfg is NULL."));
#endif
      RETVALUE(RFAILED);
   }

   idx = srbCfg->numOfSrbCfgd++;
   srbCfg->srb[idx].rbType = CM_LTE_SRB;
   srbCfg->srb[idx].rbId = rabId;
   srbCfg->srb[idx].pdcpId = rabId;

   RETVALUE(ROK);

} /* veInitUeSrbInfo */
#endif
#ifdef LTE_HO_SUPPORT
/**********************************************************************
                  Dedicated CRNTI and Preambles
**********************************************************************/

/*
 *
 *      Fun:   veGetDedCrnti
 *
 *      Desc:  Get available dedicated CRNTI
 *
 *      Ret:   ROK   - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veGetDedCrnti
(
U16 *rnti
)
#else
PUBLIC S16 veGetDedCrnti (rnti)
U16 *rnti;
#endif
{
   CmLList  *tmpNode = NULLP;
  
   TRC2(veGetDedCrnti);
   
   /* Get the first node from the list */ 
   CM_LLIST_FIRST_NODE(&veCb.cellCb[0]->rntiDb.freeRntiLst, tmpNode);
   if (NULLP != tmpNode)
   {
      cmLListDelFrm(&veCb.cellCb[0]->rntiDb.freeRntiLst, tmpNode);
      /* Add this into inuse preamble List */
      tmpNode->next = NULLP;
      tmpNode->prev = NULLP;
      cmLListAdd2Tail(&veCb.cellCb[0]->rntiDb.inUseRntiLst, tmpNode); 
      *rnti = ((VeRntinode *)tmpNode->node)->rnti;
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veGetDedCrnti:\
         Dedicated RNTI is not available : %d\n",__LINE__));
#endif
      RETVALUE(RFAILED);
   }


   RETVALUE(ROK);
} /* end of veGetDedCrnti */


/*
 *
 *      Fun:   veRlsDedCrnti
 *
 *      Desc:  Get release the CRNTI and add to free list
 *
 *      Ret:   ROK   - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veRlsDedCrnti
(
U16 rnti
)
#else
PUBLIC S16 veRlsDedCrnti (rnti)
U16 rnti;
#endif
{
   CmLList *cmLstEnt = NULLP; 
   VeRntinode *rntiNode = NULLP; 
   Bool isRntiFound = FALSE;
   TRC2(veRlsDedCrnti);
   
   cmLstEnt = veCb.cellCb[0]->rntiDb.inUseRntiLst.first;
   while (NULLP != cmLstEnt)
   {
      rntiNode = (VeRntinode *)cmLstEnt->node;
      if (rnti == rntiNode->rnti)
      {
         isRntiFound = TRUE;
         break;
      }
      /* Move to next entry */
      cmLstEnt = cmLstEnt->next;
   }/* End of while */
   if (TRUE == isRntiFound)
   {
      /* Delete from the inuse RNTI List */
      cmLListDelFrm(&veCb.cellCb[0]->rntiDb.inUseRntiLst, cmLstEnt);
      /* Add this into free RNTI List */
      cmLstEnt->next = NULLP;
      cmLstEnt->prev = NULLP;
      cmLListAdd2Tail(&veCb.cellCb[0]->rntiDb.freeRntiLst, cmLstEnt); 
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veRlsDedCrnti:\
         Dedicated RNTI to be released not found : %d\n",__LINE__));
#endif
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}/* end of veRlsDedCrnti */


/*
 *
 *      Fun:   veGetDedPrmbl
 *
 *      Desc:  Get available dedicated Preamble
 *
 *      Ret:   ROK   - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veGetDedPrmbl
(
U16 *prmblId
)
#else
PUBLIC S16 veGetDedPrmbl (prmblId)
U16 *prmblId;
#endif
{
   CmLList  *tmpNode = NULLP;
  
   TRC2(veGetDedPrmbl);
   
   /* Get the first node from the list */ 
   CM_LLIST_FIRST_NODE(&veCb.cellCb[0]->prmblDb.freePrmblLst, tmpNode);
   if (NULLP != tmpNode)
   {
      cmLListDelFrm(&veCb.cellCb[0]->prmblDb.freePrmblLst, tmpNode);
      /* Add this into inuse preamble List */
      tmpNode->next = NULLP;
      tmpNode->prev = NULLP;
      cmLListAdd2Tail(&veCb.cellCb[0]->prmblDb.inUsePrmblLst, tmpNode); 
      *prmblId = ((VePrmblnode *)tmpNode->node)->prmblId;
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veGetDedPrmbl:\
         Dedicated Preamble not available : %d\n",__LINE__));
#endif
      /* As per the spec when there is no dedicated preamble is available
         and still the Target eNB wants to admit the UE then it assigns
         0 as a dedicated preamble. So that Ue can trigger contention based RACH */
      *prmblId = 0;      
   }
   RETVALUE(ROK);
} /* end of veGetDedPrmbl */


/*
 *
 *      Fun:   veRlsDedPrmbl
 *
 *      Desc:  Get release the CRNTI and add to free list
 *
 *      Ret:   ROK   - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veRlsDedPrmbl
(
U16 prmblId
)
#else
PUBLIC S16 veRlsDedPrmbl (prmblId)
U16 prmblId;
#endif
{
   CmLList *cmLstEnt = NULLP; 
   VePrmblnode *prmblNode = NULLP; 
   Bool isPrmblFound = FALSE;
   TRC2(veRlsDedPrmbl);
   
   cmLstEnt = veCb.cellCb[0]->prmblDb.inUsePrmblLst.first;
   while (NULLP != cmLstEnt)
   {
      prmblNode = (VePrmblnode *)cmLstEnt->node;
      if (prmblId == prmblNode->prmblId)
      {
         isPrmblFound = TRUE;
         break;
      }
      /* Move to next entry */
      cmLstEnt = cmLstEnt->next;
   }/* End of while */
   if (TRUE == isPrmblFound)
   {
      /* Delete from the inuse preamble List */
      cmLListDelFrm(&veCb.cellCb[0]->prmblDb.inUsePrmblLst, cmLstEnt);
      /* Add this into free preamble List */
      cmLstEnt->next = NULLP;
      cmLstEnt->prev = NULLP;
      cmLListAdd2Tail(&veCb.cellCb[0]->prmblDb.freePrmblLst, cmLstEnt); 
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veRlsDedPrmbl:\
         Dedicated Preamble to be released not found : %d\n",__LINE__));
#endif
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}/* end of veRlsDedPrmbl */

/*
 *
 *      Fun:   veUtlFillHoCmdTrnspCntr
 *
 *      Desc:  To fill the  Transprent Container structure
 *
 *      Ret:   ROK   - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */

#ifdef ANSI
PUBLIC S16 veUtlFillHoCmdTrnspCntr
(          
VeUeCb        *ueCb,
NhuEncReqSdus *hoCmdEncReq,
NhuEncCfmSdus *nhuEncCfmSdus
)          
#else
PUBLIC S16 veUtlFillHoCmdTrnspCntr (ueCb, hoCmdEncReq,nhuEncCfmSdus)
VeUeCb            *ueCb;
NhuEncReqSdus *hoCmdEncReq;
NhuEncCfmSdus *nhuEncCfmSdus;
#endif             
{

   MsgLen cnt = 0;
   S16 retVal = ROK; 
   MsgLen len = 0;

   TRC2(veUtlFillHoCmdTrnspCntr);
   
   if(NULLP == hoCmdEncReq)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlFillHoCmdTrnspCntr : hoCmdEncReq is NULL"));
      RETVALUE(RFAILED);
   }
   hoCmdEncReq->sdu.msgCategory = NHU_MSG_HOCMD;
   VE_FILL_TKN_UINT(hoCmdEncReq->sdu.u.hovrCmd.pres,PRSNT_NODEF);
   VE_FILL_TKN_UINT(hoCmdEncReq->sdu.u.hovrCmd.criticalExtns.choice,CRITICALEXTNS_C10);
   VE_FILL_TKN_UINT(hoCmdEncReq->sdu.u.hovrCmd.criticalExtns.val.c1.choice,C22_HANDOVERCMD_R8);
   VE_FILL_TKN_UINT(hoCmdEncReq->sdu.u.hovrCmd.criticalExtns.val.c1.val.handoverCmd_r8.pres,PRSNT_NODEF);
   /* Find the length of the received IP packet */
   retVal = SFndLenMsg(nhuEncCfmSdus->sdu.mBuf, &len);
   if((len == 0) || (retVal != ROK))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                     "veUtlFillHoCmdTrnspCntr: In correct buffer len %d",len));
      RETVALUE(RFAILED);
   }
   hoCmdEncReq->sdu.u.hovrCmd.criticalExtns.val.c1.val.handoverCmd_r8.handoverCmdMsg.pres = 1;
   hoCmdEncReq->sdu.u.hovrCmd.criticalExtns.val.c1.val.handoverCmd_r8.handoverCmdMsg.len = len;
   /* Allocate memory for the flat buffer */
   VE_ALLOC(&hoCmdEncReq->sdu.u.hovrCmd.criticalExtns.val.c1.val.handoverCmd_r8.handoverCmdMsg.val, len);
   SCpyMsgFix(nhuEncCfmSdus->sdu.mBuf,0,len,
            (Data *)hoCmdEncReq->sdu.u.hovrCmd.criticalExtns.val.c1.val.handoverCmd_r8.handoverCmdMsg.val,
            &cnt);
   VE_FILL_TKN_UINT(hoCmdEncReq->sdu.u.hovrCmd.criticalExtns.val.c1.val.handoverCmd_r8.nonCriticalExtn.pres,NOTPRSNT);
   RETVALUE(ROK);
}


/*
 *
 *      Fun:   veUtlFillHoCmdTrnspCntr
 *
 *      Desc:  To fill the HO Command Transprent Container structure
 *
 *      Ret:   ROK   - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veUtlFillHoCmdTrnspCntrWthMobility
(
VeUeCb        *ueCb,
NhuEncReqSdus *hoCmdEncReq,
U16            crnti,
U16            pci
)
#else
PUBLIC S16 veUtlFillHoCmdTrnspCntrWthMobility (ueCb, hoCmdEncReq, crnti, pci)
VeUeCb            *ueCb;
NhuEncReqSdus     *hoCmdEncReq;
U16               crnti;
U16               pci;
#endif
{
   U32 rrc_TransId = 0;
   U16 len = 0;
   NhuRRCConRecfgn_r8_IEs   *rrcConRecfgn_r8 = NULLP;
   NhuRACH_ConfigCmn        *rachConfigCommon = NULLP;
   NhuPRACH_Config          *prachConfig = NULLP;
   NhuPUSCH_ConfigCmn       *puschConfigCommon = NULLP;
   NhuSecConfigHOhandoverTypintraLTE  *intraLTE = NULLP;
   NhuRadioResourceConfigDedicated *radioResCfg;
   NhuSRB_ToAddMod *srbIe = NULLP;
   NhuRLC_Configam *rlcCfgAm = NULLP;
   NhuLogChannelConfig *logChCfg = NULLP;
   U8 loopCnt = 0;
   NhuDRB_ToAddMod                           *drbIe = NULLP;
   NhuLogChannelConfigul_SpecificParams      *chCfgParams = NULLP;

   TRC2(veUtlFillHoCmdTrnspCntr);
   hoCmdEncReq->sdu.msgCategory = NHU_MSG_DLDCCH;
   veFillTknU8(&(hoCmdEncReq->sdu.u.dcchMsg.pres), PRSNT_NODEF);
   veFillTknU8(&(hoCmdEncReq->sdu.u.dcchMsg.message.choice),DL_DCCH_MSGTYP_C4);
   veFillTknU8(&(hoCmdEncReq->sdu.u.dcchMsg.message.val.c1.choice),C4_RRCCONRECFGN);
   veFillTknU8(&(hoCmdEncReq->sdu.u.dcchMsg.message.val.c1.val.rrcConRecfgn.pres),PRSNT_NODEF);
   veFillTknU32(&(hoCmdEncReq->sdu.u.dcchMsg.message.val.c1.val.rrcConRecfgn.rrc_TrnsxnId),rrc_TransId);
   veFillTknU8(&(hoCmdEncReq->sdu.u.dcchMsg.message.val.c1.val.rrcConRecfgn.criticalExtns.choice), CRITICALEXTNS_C10);
   veFillTknU8(&(hoCmdEncReq->sdu.u.dcchMsg.message.val.c1.val.rrcConRecfgn.criticalExtns.val.c1.choice),C12_RRCCONRECFGN_R8);
   rrcConRecfgn_r8 = &(hoCmdEncReq->sdu.u.dcchMsg.message.val.c1.val.rrcConRecfgn.criticalExtns.val.c1.val.rrcConRecfgn_r8);
   veFillTknU8(&(rrcConRecfgn_r8->pres),PRSNT_NODEF);
   /* Fill Mobility Control Information for handover.*/
   veFillTknU8(&(rrcConRecfgn_r8->mobilityCntrlInfo.pres),PRSNT_NODEF);
   /* Fill Physical Cell Id of Target Cell*/
   veFillTknU32(&(rrcConRecfgn_r8->mobilityCntrlInfo.targetPhysCellId), pci);
   /* Fill Carrier Frequency of eNB*/
   veFillTknU8(&(rrcConRecfgn_r8->mobilityCntrlInfo.carrierFreq.pres),PRSNT_NODEF);
   /* SELVA NSN-CHANGE , All changes in this function*/
   veFillTknU32(&(rrcConRecfgn_r8->mobilityCntrlInfo.carrierFreq.dl_CarrierFreq),veDfltMeasNghCellDlFreq);
/* selva */
#ifdef LTE_TDD
   rrcConRecfgn_r8->mobilityCntrlInfo.carrierFreq.ul_CarrierFreq.pres = NOTPRSNT;
#else
   veFillTknU32(&(rrcConRecfgn_r8->mobilityCntrlInfo.carrierFreq.ul_CarrierFreq),veDfltUlCarrierFreq);
#endif
   /* Fill Carrier Bandwidth */
   veFillTknU8(&(rrcConRecfgn_r8->mobilityCntrlInfo.carrierBandwidth.pres),PRSNT_NODEF);
   veFillTknU32(&(rrcConRecfgn_r8->mobilityCntrlInfo.carrierBandwidth.dl_Bandwidth),veDfltDlSysbw);
#ifdef LTE_TDD
   rrcConRecfgn_r8->mobilityCntrlInfo.carrierBandwidth.ul_Bandwidth.pres = NOTPRSNT;
#else
   veFillTknU32(&(rrcConRecfgn_r8->mobilityCntrlInfo.carrierBandwidth.ul_Bandwidth),veDfltUlBandwidth);
#endif
   veFillTknU32(&(rrcConRecfgn_r8->mobilityCntrlInfo.additionalSpectrumEmission),veDfltAddSpectEmn);
   veFillTknU32(&(rrcConRecfgn_r8->mobilityCntrlInfo.t304),veDfltTmrsAndConstsT304);/* 1 Means that 100ms*/
   /* Fill New UE Identity.*/   
   len = 16; /* Length of the Identity*/
   VE_FILL_BIT_STR_VAL(rrcConRecfgn_r8->mobilityCntrlInfo.newUE_Identity,len,crnti,hoCmdEncReq);
   /* Fill RRC Connection Reconfiguration IE.*/
   veFillTknU8(&(rrcConRecfgn_r8->mobilityCntrlInfo.radioResourceConfigCmn.pres),PRSNT_NODEF);
#ifdef LTERRC_REL9
   rachConfigCommon =  &rrcConRecfgn_r8->mobilityCntrlInfo.radioResourceConfigCmn.rach_ConfigCmn;
   veFillTknU8(&(rachConfigCommon->pres),PRSNT_NODEF);
   veFillTknU8(&(rachConfigCommon->preambleInfo.pres),PRSNT_NODEF);
  /* Fill No of non dedicated Preamble. 9 means that 40 preambles.*/
   veFillTknU32(&(rachConfigCommon->preambleInfo.numberOfRA_Preambles),9);
   veFillTknU8(&(rachConfigCommon->preambleInfo.preamblesGroupAConfig.pres),PRSNT_NODEF);
   /* Fill sizeOfRA_PreamblesGroupA IE.*/
   veFillTknU32(&(rachConfigCommon->preambleInfo.preamblesGroupAConfig.sizeOfRA_PreamblesGroupA),6);
   veFillTknU32(&(rachConfigCommon->preambleInfo.preamblesGroupAConfig.messageSizeGroupA),1);
   veFillTknU32(&(rachConfigCommon->preambleInfo.preamblesGroupAConfig.messagePowerOffsetGroupB),2);
   veFillTknU8(&(rachConfigCommon->powerRampingParams.pres),PRSNT_NODEF);
   veFillTknU32(&(rachConfigCommon->powerRampingParams.powerRampingStep),0);
   veFillTknU32(&(rachConfigCommon->powerRampingParams.preambleInitReceivedTgtPower),1);
   /* Fill ra_SupervisionInfo IE.*/
   veFillTknU8(&(rachConfigCommon->ra_SupervisionInfo.pres),PRSNT_NODEF);
   veFillTknU32(&(rachConfigCommon->ra_SupervisionInfo.preambleTransMax),3);
   veFillTknU32(&(rachConfigCommon->ra_SupervisionInfo.ra_RspWindowSize),3);
   veFillTknU32(&(rachConfigCommon->ra_SupervisionInfo.mac_ContentionResolutionTimer),2);
   veFillTknU32(&(rachConfigCommon->maxHARQ_Msg3Tx),4);

#else
   rachConfigCommon =  &rrcConRecfgn_r8->mobilityCntrlInfo.radioResourceConfigCmn.rach_Config;
   veFillTknU8(&(rachConfigCommon->pres),PRSNT_NODEF);
   veFillTknU8(&(rachConfigCommon->preambleInfo.pres),PRSNT_NODEF);
  /* Fill No of non dedicated Preamble. 9 means that 40 preambles.*/
   veFillTknU32(&(rachConfigCommon->preambleInfo.numberOfRA_Preambles),veDfltNumberOfRaPreambles);
   veFillTknU8(&(rachConfigCommon->preambleInfo.preamblesGroupAConfig.pres),PRSNT_NODEF);
   /* Fill sizeOfRA_PreamblesGroupA IE.*/
   veFillTknU32(&(rachConfigCommon->preambleInfo.preamblesGroupAConfig.sizeOfRA_PreamblesGroupA),veDflSizeOfRaPreamblesGroupA);
   veFillTknU32(&(rachConfigCommon->preambleInfo.preamblesGroupAConfig.messageSizeGroupA),veDfltMessageSizeGroupA);
   veFillTknU32(&(rachConfigCommon->preambleInfo.preamblesGroupAConfig.messagePowerOffsetGroupB),veDfltMessagePowerOffsetGroupB);
   veFillTknU8(&(rachConfigCommon->powerRampingParams.pres),PRSNT_NODEF);
   veFillTknU32(&(rachConfigCommon->powerRampingParams.powerRampingStep),veDfltPowerRampingStep);
   veFillTknU32(&(rachConfigCommon->powerRampingParams.preambleInitReceivedTgtPower),veDfltPreambleInitRecvdTgtPwr);
   /* Fill ra_SupervisionInfo IE.*/
   veFillTknU8(&(rachConfigCommon->ra_SupervisionInfo.pres),PRSNT_NODEF);
   veFillTknU32(&(rachConfigCommon->ra_SupervisionInfo.preambleTransMax),veDfltPreambleTransMax);
   veFillTknU32(&(rachConfigCommon->ra_SupervisionInfo.ra_RspWindowSize),veDfltRaRspWindowSize);
   veFillTknU32(&(rachConfigCommon->ra_SupervisionInfo.mac_ContentionResolutionTimer),veDfltMacContentResTmr);
   veFillTknU32(&(rachConfigCommon->maxHARQ_Msg3Tx),veDfltMaxHARQMsg3Tx);
#endif
   /* Fill NhuPRACH_Config IE of Radio Resource Config Common.*/
   prachConfig = &rrcConRecfgn_r8->mobilityCntrlInfo.radioResourceConfigCmn.prach_Config;
   veFillTknU8(&(prachConfig->pres),PRSNT_NODEF);
   veFillTknU32(&(prachConfig->rootSequenceIdx),veDfltRootSequenceIdx);
   veFillTknU8(&(prachConfig->prach_ConfigInfo.pres),PRSNT_NODEF);
   veFillTknU32(&(prachConfig->prach_ConfigInfo.prach_ConfigIdx),veDfltPrachCfgIdx);
   veFillTknU8(&(prachConfig->prach_ConfigInfo.highSpeedFlag),veDfltHighSpeedFlag);
   veFillTknU32(&(prachConfig->prach_ConfigInfo.zeroCorrelationZoneConfig),veDfltZeroCorrelZoneCfg);
   veFillTknU32(&(prachConfig->prach_ConfigInfo.prach_FreqOffset),veDfltPrachFreqOffset);

   /* Fill NhuPUSCH_ConfigCmn IE which is Mandatory.*/
   puschConfigCommon = &rrcConRecfgn_r8->mobilityCntrlInfo.radioResourceConfigCmn.pusch_ConfigCmn;
   veFillTknU8(&(puschConfigCommon->pres),PRSNT_NODEF);
   veFillTknU8(&(puschConfigCommon->pusch_ConfigBasic.pres),PRSNT_NODEF);
   veFillTknU32(&(puschConfigCommon->pusch_ConfigBasic.n_SB),veDfltNSB);
   veFillTknU32(&(puschConfigCommon->pusch_ConfigBasic.hoppingMode),veDfltHoppingMode);
   veFillTknU32(&(puschConfigCommon->pusch_ConfigBasic.pusch_HoppingOffset),veDfltPuschHoppingOff);
   veFillTknU8(&(puschConfigCommon->pusch_ConfigBasic.enable64QAM),veDfltEnable64QAM);
   veFillTknU8(&(puschConfigCommon->ul_ReferenceSignalsPUSCH.pres),PRSNT_NODEF);
   veFillTknU8(&(puschConfigCommon->ul_ReferenceSignalsPUSCH.groupHoppingEnabled),veDfltGroupHoppingEnabled);
   veFillTknU32(&(puschConfigCommon->ul_ReferenceSignalsPUSCH.groupAssignmentPUSCH),veDfltGroupAssignPUSCH);
   veFillTknU8(&(puschConfigCommon->ul_ReferenceSignalsPUSCH.sequenceHoppingEnabled),veDfltSeqHoppingEnabled);
   veFillTknU32(&(puschConfigCommon->ul_ReferenceSignalsPUSCH.cyclicShift),veDfltCyclicShift);
#ifdef LTE_TDD
   NhuTDD_Config *tddCfg = NULLP;
   tddCfg = &rrcConRecfgn_r8->mobilityCntrlInfo.radioResourceConfigCmn.tdd_Config;
   VE_FILL_TKN_UINT(tddCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(tddCfg->subframeAssignment,\
         veDfltSubframeAssignment);
   VE_FILL_TKN_UINT(tddCfg->specialSubfrmPatterns,\
         veDfltSpecialSubfrmPatterns);
#endif
   VE_FILL_TKN_UINT(rrcConRecfgn_r8->mobilityCntrlInfo.radioResourceConfigCmn.p_Max,veDfltPMax);   

   /* Fill NhuUL_CyclicPrefixLength IE which is Mandatory.*/
   veFillTknU32(&(rrcConRecfgn_r8->mobilityCntrlInfo.radioResourceConfigCmn.ul_CyclicPrefixLength),veDfltUlCyclicPrefixLength);
   
   /* Fill NhuRACH_ConfigDedicated IE.*/
   veFillTknU8(&(rrcConRecfgn_r8->mobilityCntrlInfo.rach_ConfigDedicated.pres),PRSNT_NODEF);
   veFillTknU32(&(rrcConRecfgn_r8->mobilityCntrlInfo.rach_ConfigDedicated.ra_PreambleIdx),ueCb->dedicatedPrmbl);
   veFillTknU32(&(rrcConRecfgn_r8->mobilityCntrlInfo.rach_ConfigDedicated.ra_PRACH_MaskIdx),veDfltPrachMskIdx);


   radioResCfg = &(rrcConRecfgn_r8->radioResourceConfigDedicated);
   veFillTknU8(&(radioResCfg->pres),PRSNT_NODEF);
   
   /* SRB-ToAddModList */
   VE_FILL_TKN_UINT(radioResCfg->srb_ToAddModLst.noComp, VE_VAL_TWO);

   VE_GET_MEM(hoCmdEncReq, (sizeof(NhuSRB_ToAddMod) * \
            radioResCfg->srb_ToAddModLst.noComp.val), \
         &(radioResCfg->srb_ToAddModLst.member));

   for (loopCnt = 0; loopCnt < radioResCfg->srb_ToAddModLst.noComp.val; loopCnt++)
   {
      srbIe = &(radioResCfg->srb_ToAddModLst.member[loopCnt]);
      VE_FILL_TKN_UINT(srbIe->pres, PRSNT_NODEF);
      if (0 == loopCnt)
      {
         VE_FILL_TKN_UINT(srbIe->srb_Identity, VE_PDCP_ID_SRB1);
      }
      else
      {
    VE_FILL_TKN_UINT(srbIe->srb_Identity, VE_PDCP_ID_SRB2);
      }
      VE_FILL_TKN_UINT(srbIe->rlc_Config.choice, RLC_CONFIG_EXPLICITVALUE);
      VE_FILL_TKN_UINT(srbIe->rlc_Config.val.explicitValue.choice,\
         RLC_CONFIG_AM);
     
       
      /* RLC Configuration for AM Mode */
      rlcCfgAm = &(srbIe->rlc_Config.val.explicitValue.val.am);
      VE_FILL_TKN_UINT(rlcCfgAm->pres, PRSNT_NODEF);

      /* Uplink AM RLC Config */
      VE_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.t_PollRetransmit,\
         veDfltSrbPollRetrans);
      VE_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.pollPDU, veDfltSrbPollPdu);
      VE_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.pollByte, veDfltSrbPollByte);
      VE_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.maxRetxThreshold, \
            veDfltSrbMaxReTxThresh);

      /* Downlink AM RLC Config */
      VE_FILL_TKN_UINT(rlcCfgAm->dl_AM_RLC.pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(rlcCfgAm->dl_AM_RLC.t_Reordering, veDfltSrbTreordrng);
      VE_FILL_TKN_UINT(rlcCfgAm->dl_AM_RLC.t_StatusProhibit, \
            veDfltSrbTStatProhibit);
      
      /* Logical Channel Configuration for SRB */
      VE_FILL_TKN_UINT(srbIe->logicalChannelConfig.choice,\
            LOGICALCHANNELCONFIG_EXPLICITVALUE);
      logChCfg = &(srbIe->logicalChannelConfig.val.explicitValue);
      VE_FILL_TKN_UINT(logChCfg->pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(logChCfg->\
            ul_SpecificParams.pres, PRSNT_NODEF);
      if (0 == loopCnt)
      {
         VE_FILL_TKN_UINT(logChCfg->ul_SpecificParams.priority,\
         veDfltSrb1LogchcfgPrior);
      }
      else
      {
         VE_FILL_TKN_UINT(logChCfg->ul_SpecificParams.priority,\
         veDfltSrb2LogchcfgPrior);
      }
      
      VE_FILL_TKN_UINT(logChCfg->ul_SpecificParams.prioritisedBitRate, \
            veDfltSrbLogchcfgPriBitRate);
      VE_FILL_TKN_UINT(logChCfg->ul_SpecificParams.bucketSizeDuration, \
            veDfltSrbLogchcfgBktSzDur);
      VE_FILL_TKN_UINT(logChCfg->ul_SpecificParams.logicalChannelGroup, \
            veDfltSrbLogchcfgGrp);
   }/* End of for() */
  
   /* Filling DRB to Add Mod List */

   VE_FILL_TKN_UINT(radioResCfg->drb_ToAddModLst.noComp, ueCb->rbInfo.numOfRbCfgd);
   
  /* Allocate memory for each Rab */
   VE_GET_MEM(hoCmdEncReq, (sizeof(NhuDRB_ToAddMod) * radioResCfg->drb_ToAddModLst.noComp.val),\
         &(radioResCfg->drb_ToAddModLst.member));
   for (loopCnt = 0; loopCnt < radioResCfg->drb_ToAddModLst.noComp.val; loopCnt++)
   {
      drbIe = &(radioResCfg->drb_ToAddModLst.member[loopCnt]);
      VE_FILL_TKN_UINT(drbIe->pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(drbIe->eps_BrIdentity, ueCb->rbInfo.rab[loopCnt].eRABId);
      VE_FILL_TKN_UINT(drbIe->drb_Identity, ueCb->rbInfo.rab[loopCnt].rbId);

      /* PDCP-Config */ 
      VE_FILL_TKN_UINT(drbIe->pdcp_Config.pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(drbIe->pdcp_Config.discardTimer,\
            veDfltDiscardTimer);
      /* Shashank changing DRB from AM to UM */
#ifndef VE_UM_MODE
      /* rlc_AM */
      VE_FILL_TKN_UINT(drbIe->pdcp_Config.rlc_AM.pres,\
            PRSNT_NODEF);
      VE_FILL_TKN_UINT(drbIe->pdcp_Config.rlc_AM.statusReportReqd,\
            veDfltStatusReportReqd);
#else
      /* rlc_UM */
      VE_FILL_TKN_UINT(drbIe->pdcp_Config.rlc_UM.pres,\
            PRSNT_NODEF);
      VE_FILL_TKN_UINT(drbIe->pdcp_Config.rlc_UM.pdcp_SN_Size,\
            NhuPDCP_Configrlc_UMpdcp_SN_Sizelen7bitsEnum);
#endif
      /* headerCompression - NOT USED */
      VE_FILL_TKN_UINT(drbIe->pdcp_Config.headerCompression.choice,\
            HEADERCOMPRESSION_NOTUSED);


      /* Shashank -  changing DRB from AM to UM */
#ifndef VE_UM_MODE
      /* RLC-Config */
      VE_FILL_TKN_UINT(drbIe->rlc_Config.choice, RLC_CONFIG_AM);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.pres, PRSNT_NODEF);
      /* UL-AM-RLC */
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.ul_AM_RLC.pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.ul_AM_RLC.t_PollRetransmit,\
            veDfltDrbTPollRetx);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.ul_AM_RLC.pollPDU,\
            veDfltDrbTPollPDU);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.ul_AM_RLC.pollByte,\
            veDfltDrbTPollByte);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.ul_AM_RLC.maxRetxThreshold,\
            veDfltDrbTMaxRetxThres);
      /* DL-AM-RLC */
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.dl_AM_RLC.pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.dl_AM_RLC.t_Reordering,\
            veDfltDrbTReordrng);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.dl_AM_RLC.t_StatusProhibit,\
            veDfltDrbTStatProh);
#else
#ifndef  VE_UM_MODE_BI_DIR
      /* RLC-Config */
      VE_FILL_TKN_UINT(drbIe->rlc_Config.choice, RLC_CONFIG_UM_UNI_DIRECTIONAL_DL);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Uni_Directional_DL.pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Uni_Directional_DL.dl_UM_RLC.pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Uni_Directional_DL.dl_UM_RLC.sn_FieldLength, 
            NhuSN_FieldLengthsize10Enum);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Uni_Directional_DL.dl_UM_RLC.t_Reordering, 
            veDfltDrbTReordrng);
#else /* ifndef  VE_UM_MODE_BI_DIR */
      /* RLC-Config */
      VE_FILL_TKN_UINT(drbIe->rlc_Config.choice, RLC_CONFIG_UM_BI_DIRECTIONAL);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.ul_UM_RLC.pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.ul_UM_RLC.sn_FieldLength,
            NhuSN_FieldLengthsize10Enum);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.dl_UM_RLC.pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.dl_UM_RLC.sn_FieldLength,
            NhuSN_FieldLengthsize10Enum);
      VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.dl_UM_RLC.t_Reordering,
            veDfltDrbTReordrng);
#endif /* ifndef  VE_UM_MODE_BI_DIR */
#endif /* ifndef VE_UM_MODE */

      /* logicalChannelIdentity  from 3 .. 10 */
      VE_FILL_TKN_UINT(drbIe->logicalChannelIdentity,\
            ueCb->rbInfo.rab[loopCnt].rbId);
      /* LogicalChannelConfig */
      VE_FILL_TKN_UINT(drbIe->logicalChannelConfig.pres, PRSNT_NODEF);
      chCfgParams = &(drbIe->logicalChannelConfig.ul_SpecificParams);
      VE_FILL_TKN_UINT(chCfgParams->pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(chCfgParams->priority, PRSNT_NODEF);
      VE_FILL_TKN_UINT(chCfgParams->prioritisedBitRate, PRSNT_NODEF);
      VE_FILL_TKN_UINT(chCfgParams->bucketSizeDuration, PRSNT_NODEF);
      VE_FILL_TKN_UINT(chCfgParams->logicalChannelGroup, PRSNT_NODEF);

   }
   /* RRC dedicated MAC Cfg */
   /* MAC-MainConfig  */
   /* TODO */
   veFillNhuMACMainCfg(radioResCfg,hoCmdEncReq,VE_PDCP_ID_DRB1); 
   VE_FILL_TKN_UINT(radioResCfg->physicalConfigDedicated.pres,NOTPRSNT); 
   rrcConRecfgn_r8->dedicatedInfoNASLst.noComp.pres = NOTPRSNT;
    
   /* Fill NhuSecConfigHO IE which is present in RRC Connection Reconfiguration.*/
   veFillTknU8(&(rrcConRecfgn_r8->securityConfigHO.pres),PRSNT_NODEF);
   veFillTknU8(&(rrcConRecfgn_r8->securityConfigHO.handoverTyp.choice),HANDOVERTYP_INTRALTE);
   veFillTknU8(&(rrcConRecfgn_r8->securityConfigHO.handoverTyp.val.intraLTE.pres),PRSNT_NODEF);
   
   intraLTE = &(rrcConRecfgn_r8->securityConfigHO.handoverTyp.val.intraLTE);
   veFillTknU8(&(intraLTE->securityAlgorithmConfig.pres),PRSNT_NODEF);
   veFillTknU32(&(intraLTE->securityAlgorithmConfig.cipheringAlgorithm),NhuSecAlgorithmConfigcipheringAlgorithmeea0Enum);
   veFillTknU32(&(intraLTE->securityAlgorithmConfig.integrityProtAlgorithm),NhuSecAlgorithmConfigintegrityProtAlgorithmreservedEnum);
   veFillTknU8(&(intraLTE->keyChangeIndicator),1);
   veFillTknU32(&(intraLTE->nextHopChainingCount),0);
   RETVALUE(ROK);
}/* end of veUtlFillHoCmdTrnspCntr */


PUBLIC S16 veFillUeCapInfo(PTR mem, NhuUE_CapRAT_ContLst *ue_RadioAccessCapInfo, VeUeCb *ueCb)
{
   veFillTknU16(&ue_RadioAccessCapInfo->noComp, 1);
   cmGetMem((Ptr)mem, ue_RadioAccessCapInfo->noComp.val * sizeof(NhuUE_CapRAT_Cont), \
         (Ptr *)&ue_RadioAccessCapInfo->member);
   veFillTknU8(&ue_RadioAccessCapInfo->member[0].pres, PRSNT_NODEF);
   veFillTknU32(&ue_RadioAccessCapInfo->member[0].rat_Typ, NhuRAT_TypeutraEnum);
   /*MSPD_FIX for HO */
#ifndef MSPD
   veFillTknU8(&ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.pres, PRSNT_NODEF);
   ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.len = 50;
   cmGetMem(mem, 50, &ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.val);
   memset(ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.val, 0xFF, \
         ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.len);
#else   
   ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.pres = 1;
   /* This information is available in the ueCb as it got stored there when we receive a
      ueCapability information */
#ifdef UE_RAD_CAP
   ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.len = ueCb->ueCap.ueEutraOctStr.len;
   cmGetMem(mem, ueCb->ueCap.ueEutraOctStr.len, &ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.val);
   memset(ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.val, 0x00, \
         ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.len);
   cmMemcpy(ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.val,ueCb->ueCap.ueEutraOctStr.val,
         ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.len);
#endif /* UE_RAD_CAP */
#endif /* MSPD */

   RETVALUE(ROK);
}

PUBLIC S16 veFillMeasInfo(PTR mem, NhuMeasConfig *measCfg)
{
   veFillNhuMeasCfgInfo(measCfg, (Ptr) mem);
   RETVALUE(ROK);
}

PUBLIC S16 veFillSrcRadRscDed(PTR mem, VeUeRABInfo *rbInfo, NhuRadioResourceConfigDedicated *srcRadRsrcConfig)
{
   U8 rbIdx = 0;
   U8 idx = 0;
   NhuSRB_ToAddMod *srb_ToAddMod = NULLP;
   NhuDRB_ToAddMod *drbInfo = NULLP;
   NhuRLC_Configam *rlcCfgAm = NULLP;
   NhuLogChannelConfig *logChCfg = NULLP;
   veFillTknU8(&srcRadRsrcConfig->pres, PRSNT_NODEF);

   /* Fill SRB Info */
   VE_FILL_TKN_UINT(srcRadRsrcConfig->srb_ToAddModLst.noComp,2);

   VE_GET_MEM(mem, (sizeof(NhuSRB_ToAddMod) * 
            srcRadRsrcConfig->srb_ToAddModLst.noComp.val),
            &(srcRadRsrcConfig->srb_ToAddModLst.member));

   for(idx = 0; idx < srcRadRsrcConfig->srb_ToAddModLst.noComp.val; idx++)
   {
      srb_ToAddMod = &(srcRadRsrcConfig->srb_ToAddModLst.member[idx]);
      VE_FILL_TKN_UINT(srb_ToAddMod->pres,PRSNT_NODEF);
      if(VE_VAL_ZERO == idx)
      {
         VE_FILL_TKN_UINT(srb_ToAddMod->srb_Identity,VE_PDCP_ID_SRB1);
      }
      else
      {
         VE_FILL_TKN_UINT(srb_ToAddMod->srb_Identity,VE_PDCP_ID_SRB2);
      }
      VE_FILL_TKN_UINT(srb_ToAddMod->rlc_Config.choice,RLC_CONFIG_EXPLICITVALUE);
      VE_FILL_TKN_UINT(srb_ToAddMod->rlc_Config.val.explicitValue.choice, RLC_CONFIG_AM);

      /* RLC Configuration for AM Mode */
      rlcCfgAm = &(srb_ToAddMod->rlc_Config.val.explicitValue.val.am);
      VE_FILL_TKN_UINT(rlcCfgAm->pres, PRSNT_NODEF);

      /* Uplink AM RLC Config */
      VE_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.t_PollRetransmit, veDfltSrbPollRetrans);
      VE_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.pollPDU, veDfltSrbPollPdu);
      VE_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.pollByte, veDfltSrbPollByte);
      VE_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.maxRetxThreshold, veDfltSrbMaxReTxThresh);

      /* Downlink AM RLC Config */
      VE_FILL_TKN_UINT(rlcCfgAm->dl_AM_RLC.pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(rlcCfgAm->dl_AM_RLC.t_Reordering, veDfltSrbTreordrng);
      VE_FILL_TKN_UINT(rlcCfgAm->dl_AM_RLC.t_StatusProhibit, veDfltSrbTStatProhibit);
      
      /* Logical Channel Configuration for SRB */
      VE_FILL_TKN_UINT(srb_ToAddMod->logicalChannelConfig.choice, LOGICALCHANNELCONFIG_EXPLICITVALUE);
      logChCfg = &(srb_ToAddMod->logicalChannelConfig.val.explicitValue);
      VE_FILL_TKN_UINT(logChCfg->pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(logChCfg->ul_SpecificParams.pres, PRSNT_NODEF);
      if (VE_VAL_ZERO == idx)
      {
         VE_FILL_TKN_UINT(logChCfg->ul_SpecificParams.priority, veDfltSrb1LogchcfgPrior);
      }
      else
      {
         VE_FILL_TKN_UINT(logChCfg->ul_SpecificParams.priority, veDfltSrb2LogchcfgPrior);
      }
      VE_FILL_TKN_UINT(logChCfg->ul_SpecificParams.prioritisedBitRate, veDfltSrbLogchcfgPriBitRate);
      VE_FILL_TKN_UINT(logChCfg->ul_SpecificParams.bucketSizeDuration, veDfltSrbLogchcfgBktSzDur);
      VE_FILL_TKN_UINT(logChCfg->ul_SpecificParams.logicalChannelGroup, veDfltSrbLogchcfgGrp);
   }

   /* Fill DRB Info */
   veFillTknU16(&srcRadRsrcConfig->drb_ToAddModLst.noComp, rbInfo->numOfRbCfgd);
   cmGetMem((Ptr)mem, rbInfo->numOfRbCfgd * sizeof(NhuDRB_ToAddMod),\
         (Ptr *) &srcRadRsrcConfig->drb_ToAddModLst.member);
   for(rbIdx = 0; rbIdx < rbInfo->numOfRbCfgd; rbIdx++)
   {
      drbInfo = &(srcRadRsrcConfig->drb_ToAddModLst.member[rbIdx]);
      veFillTknU8(&drbInfo->pres, PRSNT_NODEF);
      veFillTknU32(&drbInfo->eps_BrIdentity, rbInfo->rab[rbIdx].eRABId);
      veFillTknU32(&drbInfo->drb_Identity, rbInfo->rab[rbIdx].rbId);
      /*veFillTknU8(&drbInfo->pdcp_Config.pres, NOTPRSNT);
      //veFillTknU8(&drbInfo->rlc_Config.choice.pres, NOTPRSNT);
      //veFillTknU8(&drbInfo->logicalChannelIdentity.pres, NOTPRSNT);
      //veFillTknU8(&drbInfo->logicalChannelConfig.pres, NOTPRSNT);*/
      drbInfo->pdcp_Config.pres.pres = NOTPRSNT;
      drbInfo->rlc_Config.choice.pres = NOTPRSNT;
      drbInfo->logicalChannelIdentity.pres = NOTPRSNT;
      drbInfo->logicalChannelConfig.pres.pres = NOTPRSNT;
   }

   RETVALUE(ROK);
}

PUBLIC S16 veFillSecAlg(NhuSecAlgorithmConfig *sourceSecAlgorithmConfig)
{
   veFillTknU8(&sourceSecAlgorithmConfig->pres, PRSNT_NODEF);
   veFillTknU32(&sourceSecAlgorithmConfig->cipheringAlgorithm, 1);
   veFillTknU32(&sourceSecAlgorithmConfig->integrityProtAlgorithm, 1);
   RETVALUE(ROK);
}

PUBLIC S16 veFillUeId(PTR mem, NhuC_RNTI *sourceUE_Identity)
{
   U8 idx = 0;
   U16 tmpLen = 16; /* len of CRNTI */
   sourceUE_Identity->pres = PRSNT_NODEF;
   sourceUE_Identity->len = tmpLen;
   if((tmpLen % 8) > 0)
   {
      tmpLen = (tmpLen/8) + 1;
   }
   else
   {
      tmpLen = tmpLen/8;
   }
   cmGetMem((Ptr) mem, tmpLen * sizeof(U8), (Ptr *) &sourceUE_Identity->val);
   for(idx = 0; idx < sourceUE_Identity->len; idx++)
   {
      sourceUE_Identity->val[idx] = 104;
   }
   RETVALUE(ROK);
}

PUBLIC S16 veFillMIBInfo (PTR mem, NhuMasterInfoBlock *mibInfo)
{
   TRC2(veFillMIBInfo);

   VE_FILL_TKN_UINT(mibInfo->pres, PRSNT_NODEF);
   /* dl-Bandwidth */
   VE_FILL_TKN_UINT(mibInfo->dl_Bandwidth, veDfltDlSysbw);
   /* PHICH-Config */
   VE_FILL_TKN_UINT(mibInfo->phich_Config.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(mibInfo->phich_Config.phich_Duration,\
         veDfltPhichDur);
   VE_FILL_TKN_UINT(mibInfo->phich_Config.phich_Resource,\
         veDfltPhichRes);
   /* systemFrameNumber */
   VE_GET_MEM(mem, sizeof(U8),\
         &(mibInfo->systemFrameNumber.val));
   mibInfo->systemFrameNumber.pres = PRSNT_NODEF;
   mibInfo->systemFrameNumber.len  = VE_BYTE_LEN;
   /* Assign the 8 most significant bits of SFN */
   VE_GET_SFN_MIB(mibInfo->systemFrameNumber.val[0], 100);

   /* Spare, since it is not optional */
   VE_GET_MEM(mem, (2 * sizeof(U8)), &(mibInfo->spare.val));
   mibInfo->spare.pres = PRSNT_NODEF;
   mibInfo->spare.len  = VE_MIB_SPARE_LEN;

   RETVALUE(ROK);
}

PUBLIC S16 veFillSIB1Info(PTR mem, NhuSysInfoBlockTyp1 *sibTyp1)
{
   NhuPLMN_IdentityLst *plmnLst = NULLP;
   NhuSchedulingInfo *schedInf = NULLP;

   U8 idx;
   U8 digit;
   U32 allocSize = 0;

   TRC2(veSibBldSIB1Msg);

   VE_FILL_TKN_UINT(sibTyp1->pres, PRSNT_NODEF);
   /* cellAccessRelatedInfo */
   VE_FILL_TKN_UINT(sibTyp1->cellAccessRelatedInfo.pres, PRSNT_NODEF);
   /* plmn-IdentityList */
   plmnLst = &(sibTyp1->cellAccessRelatedInfo.plmn_IdentityLst);
   VE_FILL_TKN_UINT(plmnLst->noComp, VE_VAL_ONE);

   allocSize = ((plmnLst->noComp.val) * (sizeof(NhuPLMN_IdentityInfo)));

   VE_GET_MEM(mem, allocSize, &(plmnLst->member));

   for(idx = 0; idx < (plmnLst->noComp.val); idx++)
   {
      VE_FILL_TKN_UINT(plmnLst->member[idx].pres, PRSNT_NODEF);
      /* PLMN-IdentityInfo */
      VE_FILL_TKN_UINT(plmnLst->member[idx].plmn_Identity.pres, PRSNT_NODEF);
      /* Allocating 3 instances of MCC digit */
      VE_FILL_TKN_UINT(plmnLst->member[idx].plmn_Identity.mcc.noComp,\
            VE_MCC_SIZE);
      VE_GET_MEM(mem,(VE_MCC_SIZE * (sizeof(NhuMCC_MNC_Digit))),
            &(plmnLst->member[idx].plmn_Identity.mcc.member));

      /* Allocating 2 instances of MNC digit */
      VE_FILL_TKN_UINT(plmnLst->member[idx].plmn_Identity.mnc.noComp,\
            veCb.cellCb[0]->sysRelInfo.mnc_nocomp);
      VE_GET_MEM(mem, (veCb.cellCb[0]->sysRelInfo.mnc_nocomp * (sizeof(NhuMCC_MNC_Digit))),
            &(plmnLst->member[idx].plmn_Identity.mnc.member));

      for(digit = 0; digit < VE_MCC_SIZE; digit++)
      {
         VE_FILL_TKN_UINT(plmnLst->member[idx].plmn_Identity.mcc.member[digit],\
               veCb.cellCb[0]->sysRelInfo.mcc[digit].val);
      }
      for(digit = 0; digit < veCb.cellCb[0]->sysRelInfo.mnc_nocomp; digit++)
      {
         VE_FILL_TKN_UINT(plmnLst->member[idx].plmn_Identity.mnc.member[digit],\
               veCb.cellCb[0]->sysRelInfo.mnc[digit].val);
      }
      /* cellReservedForOperatorUse */
      VE_FILL_TKN_UINT(plmnLst->member[idx].cellReservedForOperatorUse,\
            veDfltCellRsrvdForOperUse);
   }

   /* TrackingAreaCode */
   sibTyp1->cellAccessRelatedInfo.trackingAreaCode.pres = PRSNT_NODEF;
   sibTyp1->cellAccessRelatedInfo.trackingAreaCode.len = VE_TRAC_LEN;
   VE_GET_MEM(mem, sizeof(U16),\
         &(sibTyp1->cellAccessRelatedInfo.trackingAreaCode.val));
   /* fill tracking area code values; set to zero for now */

   /* CellIdentity  Bitstring of 28 bits len */
   sibTyp1->cellAccessRelatedInfo.cellIdentity.pres = PRSNT_NODEF;
   sibTyp1->cellAccessRelatedInfo.cellIdentity.len = VE_CELL_ID_LEN;
   VE_GET_MEM(mem, sizeof(U32),\
         &(sibTyp1->cellAccessRelatedInfo.cellIdentity.val));
   cmMemcpy((U8 *)(sibTyp1->cellAccessRelatedInfo.cellIdentity.val), \
         (U8 *)&(veCb.cellCb[0]->cellId), sizeof(U32));

   /* cellBarred */
   VE_FILL_TKN_UINT(sibTyp1->cellAccessRelatedInfo.cellBarred, veDfltCellBarred);
   /*  intraFreqReselection */
   VE_FILL_TKN_UINT(sibTyp1->cellAccessRelatedInfo.intraFreqReselection,\
         veDfltIntraFreqResel);
   /* csg-Indication */
   VE_FILL_TKN_UINT(sibTyp1->cellAccessRelatedInfo.csg_Ind, FALSE);
   /* csg-Identity left empty*/

   /* Q-RxLevMin */ 
   VE_FILL_TKN_UINT(sibTyp1->cellSelectionInfo.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(sibTyp1->cellSelectionInfo.q_RxLevMin,\
         veDfltqRxlevmin);
   VE_FILL_TKN_UINT(sibTyp1->cellSelectionInfo.q_RxLevMinOffset,\
         veDfltqRxlevminoffset);
   /* P-Max */
   VE_FILL_TKN_UINT(sibTyp1->p_Max, veDfltPMax);
   /* freqBandIndicator */
   VE_FILL_TKN_UINT(sibTyp1->freqBandIndicator,\
         veCb.cellCb[0]->sysRelInfo.freqBandInd);
   /*TODO Atul : Fill the Scheduling Info for the SIB3, SIB4 and SIB5 */
   /* SchedulingInfoList */
#ifndef SI_NEW
   VE_FILL_TKN_UINT(sibTyp1->schedulingInfoLst.noComp, VE_VAL_ONE);
#else
   VE_FILL_TKN_UINT(sibTyp1->schedulingInfoLst.noComp, VE_SI_CNT);
#endif
   VE_GET_MEM(mem, 
              (sizeof(NhuSchedulingInfo) * sibTyp1->schedulingInfoLst.noComp.val),
         &(sibTyp1->schedulingInfoLst.member));

   for(idx = 0; idx < (sibTyp1->schedulingInfoLst.noComp.val); idx++)
   {
      schedInf = &(sibTyp1->schedulingInfoLst.member[idx]);
      VE_FILL_TKN_UINT(schedInf->pres, PRSNT_NODEF);
#ifndef SI_NEW
      VE_FILL_TKN_UINT(schedInf->si_Periodicity,\
            veDfltSiPeriodicity);
      /* Zero means only SIB2 by default */
      VE_FILL_TKN_UINT(schedInf->sib_MappingInfo.noComp,\
            VE_VAL_ZERO);
#else
      VE_FILL_TKN_UINT(schedInf->si_Periodicity,\
            veDfltSiPeriodicity[idx]);
      if (idx == 0)
      {
         /* Zero means only SIB2 by default */
         VE_FILL_TKN_UINT(schedInf->sib_MappingInfo.noComp,\
               VE_VAL_ZERO);
      }
      else
      {
         VE_FILL_TKN_UINT(schedInf->sib_MappingInfo.noComp,\
               veDfltSiMapInfoCnt[idx]);
         VE_GET_MEM(mem, 
                    (sizeof(NhuSIB_Typ) * schedInf->sib_MappingInfo.noComp.val),
               &(schedInf->sib_MappingInfo.member));
         VE_FILL_TKN_UINT(schedInf->sib_MappingInfo.member[0],\
               veDfltSiMapInfoVal[idx]);
      }
#endif
   }

#ifdef LTE_TDD
   /* TDD-Config */
   VE_FILL_TKN_UINT(sibTyp1->tdd_Config.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(sibTyp1->tdd_Config.subframeAssignment,\
         veDfltSubframeAssignment);
   VE_FILL_TKN_UINT(sibTyp1->tdd_Config.specialSubfrmPatterns,\
         veDfltSpecialSubfrmPatterns);
#endif /* end of LTE_TDD */

   /* si-WindowLength */
   VE_FILL_TKN_UINT(sibTyp1->si_WindowLength,\
         veDfltSiWindowLength);
   /* systemInfoValueTag */
   VE_FILL_TKN_UINT(sibTyp1->systemInfoValueTag, veDfltSystemInfoValueTag);
   /* nonCriticalExtension */
   VE_FILL_TKN_UINT(sibTyp1->nonCriticalExtn.pres, PRSNT_NODEF);

   RETVALUE(ROK);
}

PUBLIC S16 veFillSIB2Info(PTR mem, NhuSysInfoBlockTyp2 *sib2)
{
   NhuRACH_ConfigCmn *rachCfg = NULLP;
   NhuRACH_ConfigCmnpreambleInfo *preAmbInf = NULLP;
   NhuBCCH_Config *bcchCfg = NULLP;
   NhuPCCH_Config *pcchCfg = NULLP;
   NhuPRACH_ConfigSIB *prachCfg = NULLP;
   NhuPDSCH_ConfigCmn *pdschCfg = NULLP;
   NhuPUSCH_ConfigCmn *puschCfg = NULLP;
   NhuPUCCH_ConfigCmn *pucchCfg = NULLP;
   NhuSoundingRS_UL_ConfigCmn *srsCfg = NULLP;
   NhuUlnkPowerCntrlCmn *ulPwrCntrl = NULLP;


   TRC2(veFillSIB2Info);

   VE_FILL_TKN_UINT(sib2->pres, PRSNT_NODEF);


   /*-Fill Radio Resource Config Common -*/
   VE_FILL_TKN_UINT(sib2->radioResourceConfigCmn.pres, PRSNT_NODEF);

   /* RACH cfg */
   rachCfg = &(sib2->radioResourceConfigCmn.rach_Config);
   VE_FILL_TKN_UINT(rachCfg->pres, PRSNT_NODEF);
   preAmbInf = &(rachCfg->preambleInfo);
   VE_FILL_TKN_UINT(preAmbInf->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(preAmbInf->numberOfRA_Preambles,\
         veDfltNumberOfRaPreambles);
   VE_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.sizeOfRA_PreamblesGroupA,\
         veDflSizeOfRaPreamblesGroupA);
   VE_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.messageSizeGroupA,\
         veDfltMessageSizeGroupA);
   VE_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.messagePowerOffsetGroupB,\
         veDfltMessagePowerOffsetGroupB);

   VE_FILL_TKN_UINT(rachCfg->powerRampingParams.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(rachCfg->powerRampingParams.powerRampingStep,\
         veDfltPowerRampingStep);
   VE_FILL_TKN_UINT(rachCfg->powerRampingParams.preambleInitReceivedTgtPower,\
         veDfltPreambleInitRecvdTgtPwr);

   VE_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.preambleTransMax,\
         veDfltPreambleTransMax);
   VE_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.ra_RspWindowSize,\
         veDfltRaRspWindowSize);
   VE_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.mac_ContentionResolutionTimer,\
         veDfltMacContentResTmr);

   VE_FILL_TKN_UINT(rachCfg->maxHARQ_Msg3Tx, veDfltMaxHARQMsg3Tx);

   /* BCCH CFG */
   bcchCfg = &(sib2->radioResourceConfigCmn.bcch_Config);
   VE_FILL_TKN_UINT(bcchCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(bcchCfg->modificationPeriodCoeff, veDfltModPeriodCoeff);

   /* PCCH CFG */
   pcchCfg = &(sib2->radioResourceConfigCmn.pcch_Config);
   VE_FILL_TKN_UINT(pcchCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(pcchCfg->defaultPagCycle, veDfltDefaultPagCycle);
   VE_FILL_TKN_UINT(pcchCfg->nB, veDfltPCCHCfgNB);

   /* PRACH Cfg */
   prachCfg = &(sib2->radioResourceConfigCmn.prach_Config);
   VE_FILL_TKN_UINT(prachCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(prachCfg->rootSequenceIdx, veDfltRootSequenceIdx);
   VE_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.prach_ConfigIdx,\
         veDfltPrachCfgIdx);
   VE_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.highSpeedFlag,\
         veDfltHighSpeedFlag);
   VE_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.zeroCorrelationZoneConfig,\
         veDfltZeroCorrelZoneCfg);
   VE_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.prach_FreqOffset,\
         veDfltPrachFreqOffset);


   /* PDSCH Cfg */
   pdschCfg = &(sib2->radioResourceConfigCmn.pdsch_Config);
   VE_FILL_TKN_UINT(pdschCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(pdschCfg->referenceSignalPower, veDfltRefSignalPower);
   VE_FILL_TKN_UINT(pdschCfg->p_b, veDfltPDSCHCfgPB);

   /* PUSCH CFG */
   puschCfg = &(sib2->radioResourceConfigCmn.pusch_Config);
   VE_FILL_TKN_UINT(puschCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.n_SB, veDfltNSB);
   VE_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.hoppingMode,\
         veDfltHoppingMode);
   VE_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.pusch_HoppingOffset,\
         veDfltPuschHoppingOff);
   VE_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.enable64QAM,\
         veDfltEnable64QAM);

   /* UL-ReferenceSignalsPUSCH Cfg */
   VE_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.groupHoppingEnabled,\
         veDfltGroupHoppingEnabled);
   VE_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.groupAssignmentPUSCH,\
         veDfltGroupAssignPUSCH);
   VE_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.sequenceHoppingEnabled,\
         veDfltSeqHoppingEnabled);
   VE_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.cyclicShift,\
         veDfltCyclicShift);

   /* PUCCH Cfg */
   pucchCfg = &(sib2->radioResourceConfigCmn.pucch_Config);
   VE_FILL_TKN_UINT(pucchCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(pucchCfg->deltaPUCCH_Shift, veDfltPUCCHDeltaShift);
   VE_FILL_TKN_UINT(pucchCfg->nRB_CQI, veDfltPUCCHnRBCQI);
   VE_FILL_TKN_UINT(pucchCfg->nCS_AN, veDfltPUCCHnCSAN);
   VE_FILL_TKN_UINT(pucchCfg->n1PUCCH_AN, veDfltNlPUCCHAn);

   /* SoundingsRS UL Cfg */
   srsCfg = &(sib2->radioResourceConfigCmn.soundingRS_UL_Config);
   VE_FILL_TKN_UINT(srsCfg->choice, SOUNDINGRS_UL_CONFIGCMN_SETUP);
   VE_FILL_TKN_UINT(srsCfg->val.setup.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(srsCfg->val.setup.srs_BandwidthConfig,\
         veDfltSrsBandwidthCfg);
   VE_FILL_TKN_UINT(srsCfg->val.setup.srs_SubfrmConfig,\
         veDfltSrsSubfrmCfg);
   VE_FILL_TKN_UINT(srsCfg->val.setup.ackNackSRS_SimultaneousTransmission,\
         veDfltAckNackSrsSimTrns);
   VE_FILL_TKN_UINT(srsCfg->val.setup.srs_MaxUpPts, veDfltSrsMaxUpPts);

   /* Uplink Power Control Config */
   ulPwrCntrl = &(sib2->radioResourceConfigCmn.uplinkPowerCntrl); 
   VE_FILL_TKN_UINT(ulPwrCntrl->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(ulPwrCntrl->p0_NominalPUSCH, veDfltP0NominalPUSCH);
   VE_FILL_TKN_UINT(ulPwrCntrl->alpha, veDfltULPwrCtrlAlpha);
   VE_FILL_TKN_UINT(ulPwrCntrl->p0_NominalPUCCH, veDfltP0NominalPUCCH);

   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format1,\
         veDfltDeltaFPUCCHFormat1);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format1b,\
         veDfltDeltaFPUCCHFormat1b);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format2,\
         veDfltDeltaFPUCCHFormat2);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format2a,\
         veDfltDeltaFPUCCHFormat2a);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format2b,\
         veDfltDeltaFPUCCHFormat2b);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaPreambleMsg3,\
         veDfltUlDeltaPreambleMsg3);

   VE_FILL_TKN_UINT(sib2->radioResourceConfigCmn.ul_CyclicPrefixLength,\
         veDfltUlCyclicPrefixLength);

   /*-Fill ue Timers And Constants -*/
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.t300,
         veDfltTmrsAndConstsT300);
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.t301,
         veDfltTmrsAndConstsT301);
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.t310,
         veDfltTmrsAndConstsT310);
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.n310,
         veDfltTmrsAndConstsN310);
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.t311,
         veDfltTmrsAndConstsT311);
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.n311,
         veDfltTmrsAndConstsN311);

   /*-Fill Freuency Infomration -*/
   VE_FILL_TKN_UINT(sib2->freqInfo.pres, PRSNT_NODEF);
#ifndef LTE_TDD
    /*  Uplink Frequency has been changed */
   VE_FILL_TKN_UINT(sib2->freqInfo.ul_CarrierFreq,23230); /* 36.101 5.7.3  Specific 782Mhz[23230]*/
   VE_FILL_TKN_UINT(sib2->freqInfo.ul_Bandwidth, veDfltUlBandwidth);
#endif
   VE_FILL_TKN_UINT(sib2->freqInfo.additionalSpectrumEmission,\
         veDfltAddSpectEmn);
   /* skipping MBSFN subframe config */
   /* TimeAlignmentTimer */
   VE_FILL_TKN_UINT(sib2->timeAlignmentTimerCmn, veDfltTimeAlignTmrCmn);
   RETVALUE(ROK);
} /* End of veSibBldSIB2Msg */


PUBLIC S16 veFillAntennaInfo(NhuAntennaInfoCmn *antennaInfoCmn)
{
   veFillTknU8(&antennaInfoCmn->pres, PRSNT_NODEF);
   veFillTknU32(&antennaInfoCmn->antennaPortsCount, 1);
   RETVALUE(ROK);
}

PUBLIC S16 veFillDlCarrFreq(NhuARFCN_ValueEUTRA *sourceDl_CarrierFreq)
{
   veFillTknU32(sourceDl_CarrierFreq, 1);
   RETVALUE(ROK);
}




/*
 *
 *      Fun:  veUtlFillHoPreparationMsg 
 *
 *      Desc:  To fill the Handover Preparation structure
 *
 *      Ret:   ROK   - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veUtlFillHoPreparationMsg
(
VeUeCb        *ueCb,
NhuEncReqSdus *hoCmdEncReq
)
#else
PUBLIC S16 veUtlFillHoPreparationMsg (ueCb, hoCmdEncReq)
VeUeCb            *ueCb;
NhuEncReqSdus *hoCmdEncReq;
#endif
{
   NhuHovrPrepInfo_r8_IEs       *nhuHovrPrepInfo_r8_IEs = NULLP;


   /* Fill  Handover Preparation Message.*/
   TRC2(veUtlFillHoCmdTrnspCntr);
   /* TEST_FIX */
   if(NULLP == hoCmdEncReq)                                              
  {                                                                     
#ifdef DEBUGP                                                           
      VE_DBG_ERROR((VE_PRNT_BUF, "veUtlFillHoPreparationMsg: \
               hoCmdEncReq is NULL."));                                 
#endif                                                                  
      RETVALUE(RFAILED);                                                
  }
   hoCmdEncReq->sdu.msgCategory = NHU_MSG_HOPREPINFO;

   VE_FILL_TKN_UINT(hoCmdEncReq->sdu.u.hovrPrepInfo.pres,PRSNT_NODEF);

   VE_FILL_TKN_UINT(hoCmdEncReq->sdu.u.hovrPrepInfo.criticalExtns.choice,C23_HANDOVERPREPINFO_R8);/*C23_HANDOVERPREPINFO_R8*/

   VE_FILL_TKN_UINT(hoCmdEncReq->sdu.u.hovrPrepInfo.criticalExtns.val.c1.choice,C23_HANDOVERPREPINFO_R8);

   nhuHovrPrepInfo_r8_IEs = &(hoCmdEncReq->sdu.u.hovrPrepInfo.criticalExtns.val.c1.val.handoverPrepInfo_r8);

   VE_FILL_TKN_UINT(nhuHovrPrepInfo_r8_IEs->pres,PRSNT_NODEF);

/* FOR TESTING ONLY */
   /* Fill UE Capability Information.*/
   veFillUeCapInfo((PTR)hoCmdEncReq, &nhuHovrPrepInfo_r8_IEs->ue_RadioAccessCapInfo,ueCb);
   /* Fill AS Config Information.*/
   VE_FILL_TKN_UINT(nhuHovrPrepInfo_r8_IEs->as_Config.pres,PRSNT_NODEF);
   veFillMeasInfo((PTR)hoCmdEncReq, &nhuHovrPrepInfo_r8_IEs->as_Config.sourceMeasConfig);
   veFillSrcRadRscDed((PTR)hoCmdEncReq, &ueCb->rbInfo, &nhuHovrPrepInfo_r8_IEs->as_Config.sourceRadioResourceConfig);
   veFillSecAlg(&nhuHovrPrepInfo_r8_IEs->as_Config.sourceSecAlgorithmConfig);
   veFillUeId((PTR)hoCmdEncReq, &nhuHovrPrepInfo_r8_IEs->as_Config.sourceUE_Identity);
   veFillMIBInfo((PTR)hoCmdEncReq, &nhuHovrPrepInfo_r8_IEs->as_Config.sourceMasterInfoBlock);
   veFillSIB1Info((PTR)hoCmdEncReq, &nhuHovrPrepInfo_r8_IEs->as_Config.sourceSysInfoBlockTyp1);
   veFillSIB2Info((PTR)hoCmdEncReq, &nhuHovrPrepInfo_r8_IEs->as_Config.sourceSysInfoBlockTyp2);
   veFillAntennaInfo(&nhuHovrPrepInfo_r8_IEs->as_Config.antennaInfoCmn);
   veFillDlCarrFreq(&nhuHovrPrepInfo_r8_IEs->as_Config.sourceDl_CarrierFreq);
  
   RETVALUE(ROK);
}


/*
 *
 *      Fun:  veCzFillNghInfoFrmX2SetReq
 *
 *      Desc:  To fill the neighbor info from Handover Request structure
 *
 *      Ret:   ROK   - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veCzFillNghInfoFrmX2SetReq
(
CztEvnt     *x2SetReq,
VeNghCellCb *nghCellCb
)
#else
PUBLIC S16 veCzFillNghInfoFrmX2SetReq (x2SetReq, nghCellCb)
CztEvnt     *x2SetReq;
VeNghCellCb *nghCellCb;
#endif
{
   CztGlobalENB_ID *glbEndId = NULLP;
   CztSrvdCells    *srvdCells = NULLP;
   CztNeighbour_Inform *nghMember = NULLP;
   CztSrvdCell_Inform *cellInfo = NULLP;
   U8 idx = 0;
   TRC2(veCzFillNghInfoFrmX2SetReq);

   /* Fill Global ENB ID */
   veUtlGetCztIE(&x2SetReq->pdu, Cztid_GlobalENB_ID, (TknU8 **)&glbEndId);
   if(NULLP == glbEndId)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veCzFillNghInfoFrmX2SetReq:glbEndId is NULL.\n"));

      RETVALUE(RFAILED);
   }
   nghCellCb->enbType = glbEndId->eNB_ID.choice.val;

   
   /* Fill Served Cell Information */
   veUtlGetCztIE(&x2SetReq->pdu, Cztid_SrvdCells, (TknU8 **)&srvdCells);

   if(NULLP == srvdCells)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veCzFillNghInfoFrmX2SetReq:srvdCells is NULL.\n"));

      RETVALUE(RFAILED);
   }

   /* Filling the Neighbor Information */
   nghCellCb->numNghInfo = srvdCells->member[0].neighbour_Info.noComp.val;
   VE_ALLOC(&(nghCellCb->nghInfo), (nghCellCb->numNghInfo * sizeof(VeNghInfo)));

   if(NULLP == nghCellCb->nghInfo)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veCzFillNghInfoFrmX2SetReq: Src & Dst Invalid\n"));

      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < nghCellCb->numNghInfo; idx++)
   {
      nghMember = &(srvdCells->member[0].neighbour_Info);
     nghCellCb->nghInfo[idx]->earfcn = nghMember->member[idx].eARFCN.val;
     nghCellCb->nghInfo[idx]->physCellId = nghMember->member[idx].pCI.val;
      cmMemcpy((U8 *)&nghCellCb->nghInfo[idx]->ecgi.eutranCellId, \
                  nghMember->member[idx].eCGI.eUTRANcellIdentifier.val, \
                  nghMember->member[idx].eCGI.eUTRANcellIdentifier.len);
      VE_GET_U32_FRM_OSXL(nghCellCb->nghInfo[idx]->ecgi.plmnId,\
                  nghMember->member[idx].eCGI.pLMN_Identity);
   }

   /* Filling the Served Cell Information */
   cellInfo = &(srvdCells->member[0].servedCellInfo);
   nghCellCb->veCellPrp.physCellId = cellInfo->pCI.val;
   cmMemcpy((U8*)&nghCellCb->veCellPrp.tac, cellInfo->tAC.val, cellInfo->tAC.len);
   cmMemcpy((U8*)&nghCellCb->veCellPrp.cellId, cellInfo->cellId.eUTRANcellIdentifier.val,\
      cellInfo->cellId.eUTRANcellIdentifier.len);
   nghCellCb->veCellPrp.numBcastPlmn = cellInfo->broadcastPLMNs.noComp.val;
   for(idx = 0; idx < nghCellCb->veCellPrp.numBcastPlmn; idx++)
   {
      cmMemcpy((U8*)&nghCellCb->veCellPrp.plmnId[idx], \
         cellInfo->broadcastPLMNs.member[idx].val,
         cellInfo->broadcastPLMNs.member[idx].len);
   }
   nghCellCb->veCellPrp.duplexMode = (NhuDuplexMode)cellInfo->eUTRA_Mode_Info.choice.val;
   if(EUTRA_MODE_INFO_FDD == nghCellCb->veCellPrp.duplexMode)
   {
      nghCellCb->veCellPrp.u.fddCb.dlEarfcn = cellInfo->eUTRA_Mode_Info.val.fDD.dL_EARFCN.val;
     nghCellCb->veCellPrp.u.fddCb.ulEarfcn = cellInfo->eUTRA_Mode_Info.val.fDD.uL_EARFCN.val;
     nghCellCb->veCellPrp.u.fddCb.dlTxBndWdth = \
         (VeTxBndWdth)cellInfo->eUTRA_Mode_Info.val.fDD.dL_Transmission_Bandwidth.val;
     nghCellCb->veCellPrp.u.fddCb.ulTxBndWdth = \
         (VeTxBndWdth)cellInfo->eUTRA_Mode_Info.val.fDD.uL_Transmission_Bandwidth.val;
   }
   else
   {
      nghCellCb->veCellPrp.u.tddCb.earfcn = cellInfo->eUTRA_Mode_Info.val.tDD.eARFCN.val;
     nghCellCb->veCellPrp.u.tddCb.txBndWdth = \
         (VeTxBndWdth)cellInfo->eUTRA_Mode_Info.val.tDD.transmission_Bandwidth.val;

     nghCellCb->veCellPrp.u.tddCb.splSfInfo.dlCycPrefix = \
        (VeCyclicPrefix) cellInfo->eUTRA_Mode_Info.val.tDD.specialSubframe_Info.cyclicPrefixDL.val;
      nghCellCb->veCellPrp.u.tddCb.splSfInfo.splSfPatrn = \
        (VeSplSubFrmPattern)  cellInfo->eUTRA_Mode_Info.val.tDD.specialSubframe_Info.specialSubframePatterns.val;
     nghCellCb->veCellPrp.u.tddCb.splSfInfo.ulCycPrefix = \
        (VeCyclicPrefix) cellInfo->eUTRA_Mode_Info.val.tDD.specialSubframe_Info.cyclicPrefixUL.val;

     nghCellCb->veCellPrp.u.tddCb.sfAssgnmnt = \
        (VeSubFrameAssignment) cellInfo->eUTRA_Mode_Info.val.tDD.subframeAssignment.val;
   }

   RETVALUE(ROK);
} /* end of veCzFillNghInfoFrmX2SetReq */


/*
 *
 *      Fun:  veCzFillNghInfoFrmX2SetRes
 *
 *      Desc:  To fill the neighbor info from Handover Response structure
 *
 *      Ret:   ROK   - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 veCzFillNghInfoFrmX2SetRes
(
CztEvnt     *x2SetRes,
VeNghCellCb *nghCellCb
)
#else
PUBLIC S16 veCzFillNghInfoFrmX2SetRes (x2SetRes, nghCellCb)
CztEvnt    *x2SetRes;
VeNghCellCb *nghCellCb;
#endif
{

   CztGlobalENB_ID *glbEndId = NULLP;
   CztSrvdCells     *srvdCells = NULLP;
   CztNeighbour_Inform *nghMember = NULLP;
   CztSrvdCell_Inform *cellInfo = NULLP;
   U8 idx = 0;

   TRC2(veCzFillNghInfoFrmX2SetRes);

   /* Fetch Global ENB ID */
   veUtlGetCztIE(&x2SetRes->pdu, Cztid_GlobalENB_ID, (TknU8 **)&glbEndId);
   if(NULLP == glbEndId)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veCzFillNghInfoFrmX2SetRes:glbEndId is NULL.\n"));

      RETVALUE(RFAILED);
   }
   nghCellCb->enbType = glbEndId->eNB_ID.choice.val;

   /* Fetch Served Cell Information */
   veUtlGetCztIE(&x2SetRes->pdu, Cztid_SrvdCells, (TknU8 **)&srvdCells);

   if(NULLP == srvdCells)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veCzFillNghInfoFrmX2SetReq:srvdCells is NULL.\n"));

      RETVALUE(RFAILED);
   }

   /* Filling the Neighbor Information */
   nghCellCb->numNghInfo = srvdCells->member[0].neighbour_Info.noComp.val;

   if ( nghCellCb->numNghInfo >  0 )
   {
    VE_ALLOC(&(nghCellCb->nghInfo), (nghCellCb->numNghInfo * sizeof(VeNghInfo)));

    if(NULLP == nghCellCb->nghInfo)
    {
       VE_DBG_ERROR((VE_PRNT_BUF,"veCpyCmTptAddr: Src & Dst Invalid\n"));

       RETVALUE(RFAILED); 
    }
   }

   for(idx = 0; idx < nghCellCb->numNghInfo; idx++)
   {
      nghMember = &(srvdCells->member[0].neighbour_Info);
     nghCellCb->nghInfo[idx]->earfcn = nghMember->member[idx].eARFCN.val;
     nghCellCb->nghInfo[idx]->physCellId = nghMember->member[idx].pCI.val;
      cmMemcpy((U8 *)&nghCellCb->nghInfo[idx]->ecgi.eutranCellId, \
                  nghMember->member[idx].eCGI.eUTRANcellIdentifier.val, \
                  nghMember->member[idx].eCGI.eUTRANcellIdentifier.len);
      VE_GET_U32_FRM_OSXL(nghCellCb->nghInfo[idx]->ecgi.plmnId,\
                  nghMember->member[idx].eCGI.pLMN_Identity);
   }

   /* Filling the Served Cell Information */
   cellInfo = &(srvdCells->member[0].servedCellInfo);
   cmMemcpy((U8*)&nghCellCb->veCellPrp.tac, cellInfo->tAC.val, cellInfo->tAC.len);
   cmMemcpy((U8*)&nghCellCb->veCellPrp.cellId, cellInfo->cellId.eUTRANcellIdentifier.val,\
         cellInfo->cellId.eUTRANcellIdentifier.len);
   nghCellCb->veCellPrp.numBcastPlmn = cellInfo->broadcastPLMNs.noComp.val;
   for(idx = 0; idx < nghCellCb->veCellPrp.numBcastPlmn; idx++)
   {
      cmMemcpy((U8*)&nghCellCb->veCellPrp.plmnId[idx], \
            cellInfo->broadcastPLMNs.member[idx].val,
            cellInfo->broadcastPLMNs.member[idx].len);
   }
   nghCellCb->veCellPrp.duplexMode = (NhuDuplexMode)cellInfo->eUTRA_Mode_Info.choice.val;
   if(EUTRA_MODE_INFO_FDD == nghCellCb->veCellPrp.duplexMode)
   {
      nghCellCb->veCellPrp.u.fddCb.dlEarfcn = cellInfo->eUTRA_Mode_Info.val.fDD.dL_EARFCN.val;
      nghCellCb->veCellPrp.u.fddCb.ulEarfcn = cellInfo->eUTRA_Mode_Info.val.fDD.uL_EARFCN.val;
      nghCellCb->veCellPrp.u.fddCb.dlTxBndWdth = \
                                                (VeTxBndWdth) cellInfo->eUTRA_Mode_Info.val.fDD.dL_Transmission_Bandwidth.val;
      nghCellCb->veCellPrp.u.fddCb.ulTxBndWdth = \
                                                (VeTxBndWdth) cellInfo->eUTRA_Mode_Info.val.fDD.uL_Transmission_Bandwidth.val;
   }
   else
   {
      nghCellCb->veCellPrp.u.tddCb.earfcn = cellInfo->eUTRA_Mode_Info.val.tDD.eARFCN.val;
      nghCellCb->veCellPrp.u.tddCb.txBndWdth = \
                                              (VeTxBndWdth) cellInfo->eUTRA_Mode_Info.val.tDD.transmission_Bandwidth.val;

      nghCellCb->veCellPrp.u.tddCb.splSfInfo.dlCycPrefix = \
                                                          (VeCyclicPrefix) cellInfo->eUTRA_Mode_Info.val.tDD.specialSubframe_Info.cyclicPrefixDL.val;
      nghCellCb->veCellPrp.u.tddCb.splSfInfo.splSfPatrn = \
                                                         (VeSplSubFrmPattern) cellInfo->eUTRA_Mode_Info.val.tDD.specialSubframe_Info.specialSubframePatterns.val;
      nghCellCb->veCellPrp.u.tddCb.splSfInfo.ulCycPrefix = \
                                                          (VeCyclicPrefix) cellInfo->eUTRA_Mode_Info.val.tDD.specialSubframe_Info.cyclicPrefixUL.val;

      nghCellCb->veCellPrp.u.tddCb.sfAssgnmnt = \
                                               (VeSubFrameAssignment) cellInfo->eUTRA_Mode_Info.val.tDD.subframeAssignment.val;
   }

   RETVALUE(ROK);
} /* end of veCzFillNghInfoFrmX2SetRes */

#endif /* End of LTE_HO_SUPPORT */

/* Function definitions for log file */
#ifdef DEBUGP
#if (VE_SM_LOG_TO_FILE && DEBUGP)

/*
*
*    Fun:    veUtlStrCpy
*
*    Desc:   Method is used to copy Strings
*
*    Ret:    Pointer to Target String
*
*
*    Notes:
*
*    File:   ve_utils.c
*
*/
#ifdef ANSI
PUBLIC S16  veUtlStrcpy
(
S8             *tgt,          /* dest string*/
CONSTANT S8    *src           /* src string */
)
#else
PUBLIC S16 veUtlStrcpy(tgt, src)
S8                *tgt;         /* dest string*/
CONSTANT S8       *src;         /* src string */
#endif
{
   PTR len;

   /* Added trace macro */
   TRC2(veUtlStrcpy)

   len = cmStrlen( (CONSTANT U8* )src) + 1;

   cmMemcpy( (U8* )tgt, (CONSTANT U8* )src, len);

   RETVALUE(ROK);
} /* veUtlStrcpy */

/*
*
*    Fun:    veUtlStrCat
*
*    Desc:   Method is used to concatenate Strings
*
*    Ret:    Pointer to Target String
*
*
*    Notes:
*
*    File:   ve_utils.c
*
*/
#ifdef ANSI
PUBLIC S16  veUtlStrcat
(
S8    *tgt,          /* dest string*/
S8    *src           /* src string */
)
#else
PUBLIC S16 veUtlStrcat(tgt, src)
S8    *tgt;         /* dest string*/
S8    *src;         /* src string */
#endif
{
   U16 srcIdx = 0;
   U16 destIdx = 0;

   /* Added trace macro */
   TRC2(veUtlStrcat)

   while (tgt[destIdx++]);
   for (destIdx--; src[srcIdx]; (tgt[destIdx++] = src[srcIdx++]));

   RETVALUE(ROK);
} /* end of veUtlStrcat */

/***********************************************************
 *
 *     Func : veUtlCreateDbgFile
 *
 *     Desc : Creates the file for logging ENB App debug logs
 *            in the path given via layer manager.
 *
 *     Ret  : ROK - success
 *            RFAILED - failure
 *
 *     Notes: None
 *
 *     File : ve_utils.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void veUtlCreateDbgFile
(
Void
)
#else
PUBLIC Void veUtlCreateDbgFile(Void)
#endif
{
   Txt   fName[LVE_MAX_FILE_PATH]; /* to hold the Date & Time in File name */
   Txt   outfName[LVE_MAX_FILE_PATH * 2]; /* file name with prefix, Date & Time and Suffix */
   Txt   *prefName = "ve_"; /* file prefix */
   Txt   *sufFname = ".log"; /* file suffix */
   S16   strLen;
   S16   cnt;
   SystemId sysId;
   S16   retVal;
   Txt   sysIdInfo[LVE_MAX_FILE_PATH];
   time_t tt;
   struct tm tmStmp;

   /* Added trace macro */
   TRC2(veUtlCreateDbgFile)

   strLen = 0; cnt = 0;

   cmMemset((U8*)fName, 0, LVE_MAX_FILE_PATH);
   cmMemset((U8*)outfName, 0, (LVE_MAX_FILE_PATH * 2));
   cmMemset((U8*)&sysId, 0, sizeof(SystemId));
   cmMemset((U8*)sysIdInfo, 0, LVE_MAX_FILE_PATH);

   time(&tt);
   localtime_r(&tt, &tmStmp);

#ifdef SS_LINUX
   asctime_r(&tmStmp, (S8*)fName);
#endif

   strLen = cmStrlen((U8*)fName);

   for(cnt = 0; cnt < strLen; cnt++)
   {
      if((fName[cnt] == ' ') || (fName[cnt] == ':'))
      {
         fName[cnt] = '-';
      }
   }

   /* copy the file path given by LM into the file name */
   veUtlStrcpy(outfName, veCb.filePath);

   /* copy the file prefix, into the file name */
   veUtlStrcpy(outfName, prefName);

   /* copy the file name string made out of Date & Time */
   veUtlStrcat(outfName, fName);

   /* remove the extra char at the end */
   strLen = cmStrlen((U8*)outfName);
   outfName[strLen-1] = '\0';

   /* copy the suffix, ".log"  into the file name */
   veUtlStrcat(outfName, sufFname);

   /* add the ENB Application Protocol Information in the begining of the file */
   retVal = veGetSId(&sysId);

   /* open the file in write mode */
   veCb.dbgFp = fopen(outfName, "w");
   
   /* check whether file we have the valid file handle or not */
   if(veCb.dbgFp == NULL)
       printf("[%s : %s] File doesn't exist..\n",__FUNCTION__,__FILE__); 
   
   if(veCb.dbgFp)
   {
      sprintf(sysIdInfo, "--------- ENB App Protocol Main Version    : %d ---------\n", sysId.mVer);
      fwrite(sysIdInfo, cmStrlen((U8*)sysIdInfo), 1, veCb.dbgFp);
      sprintf(sysIdInfo, "--------- ENB App Protocol Main Revision   : %d ---------\n", sysId.mRev);
      fwrite(sysIdInfo, cmStrlen((U8*)sysIdInfo), 1, veCb.dbgFp);
      sprintf(sysIdInfo, "--------- ENB App Protocol Branch Version  : %d ---------\n", sysId.bVer);
      fwrite(sysIdInfo, cmStrlen((U8*)sysIdInfo), 1, veCb.dbgFp);
      sprintf(sysIdInfo, "--------- ENB App Protocol Branch Revision : %d ---------\n", sysId.bRev);
      fwrite(sysIdInfo, cmStrlen((U8*)sysIdInfo), 1, veCb.dbgFp);
      if(sysId.ptNmb)
      {
         sprintf(sysIdInfo, "--------- ENB App Protocol Part Number     : %s ---------\n\n", sysId.ptNmb);
         fwrite(sysIdInfo, cmStrlen((U8*)sysIdInfo), 1, veCb.dbgFp);
      }
   }


   RETVOID;
} /* veUtlCreateDbgFile */

#endif /* (VE_SM_LOG_TO_FILE && DEBUGP) */
#endif /* DEBUGP */

/*
 *
 *      Fun:   veCmpPlmnId
 *
 *      Desc:  Compares the PlmnId
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S8 veCmpPlmnId
(
VePlmnId    *plmnId1,
VePlmnId    *plmnId2
)
#else
PUBLIC S8 veCmpPlmnId (plmnId1, plmnId2)
VePlmnId    *plmnId1;
VePlmnId    *plmnId2;
#endif
{
   if ((plmnId1->mcc[0] != plmnId2->mcc[0]) ||
       (plmnId1->mcc[1] != plmnId2->mcc[1]) ||
       (plmnId1->mcc[2] != plmnId2->mcc[2]))
   {
      RETVALUE(FALSE);
   }
   if (plmnId1->numMncDigits != plmnId2->numMncDigits)
   {
      RETVALUE(FALSE);
   }
   if ((plmnId1->mnc[0] != plmnId2->mnc[0]) ||
       (plmnId1->mnc[1] != plmnId2->mnc[1]))
   {
      RETVALUE(FALSE);
   }
   if (plmnId1->numMncDigits == 3)
   {
      if (plmnId1->mnc[2] != plmnId2->mnc[2])
      {
         RETVALUE(FALSE);
      }
   }
   RETVALUE(TRUE);
}

#ifdef VE_TEST_CODE
/*
 *
 *      Fun:   veTstEnbRst
 *
 *      Desc:  Sends out the eNB reset for testing
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  ve_utils.c
 *
 */
#ifdef ANSI
PUBLIC S8 veTstEnbRst
(
 VeMmeCb    *mmeCb
)
#else
PUBLIC S8 veTstEnbRst (mmeCb)
VeMmeCb     *mmeCb;
#endif
{
   /* Logic is to test all type of resets. So, using counter, each time, 
    * one one type of reset is called */
   VeS1ConId    tmpConId[10];
   PRIVATE U16  tmpCnt = 0;
   S16          ueIdx, idx, tmpMaxUeSupp;
   VeUeCb       *ueCb = NULLP;
   VeCellCb     *cellCb = NULLP;

   tmpMaxUeSupp = veCb.cellCb[0]->maxUeSupp;
   cellCb = veCb.cellCb[0];
   idx = 0;

   /* Reset will be sent for ue with slected eNB UE S1AP id */
   if(tmpCnt == 0)
   {
      ueCb = cellCb->ueCbLst[0];
      if(ueCb == NULLP)
      {
         RETVALUE(ROK);
      }
      tmpConId[idx].conId = ueCb->s1ConCb->enb_ue_s1ap_id;
      tmpConId[idx].type = VE_S1AP_ENB_ID;
      idx++;

      printf("\n Sending Reset for connection %ld\n", ueCb->s1ConCb->enb_ue_s1ap_id);
      veHdlEnbReset(mmeCb->mmeId, CAUSE_RADIONW, 1, idx,
                   RESETTYP_PARTOFS1_INTF, tmpConId);
   }

   RETVALUE(ROK);
}
#endif /* VE_TEST_CODE */

/**********************************************************************
 
         End of file:     ve_utils.c@@/main/2 - Wed Dec  1 08:44:41 2010
 
**********************************************************************/

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
------------ -------- ---- ----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
