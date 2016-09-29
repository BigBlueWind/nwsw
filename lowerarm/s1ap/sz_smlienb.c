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

     Name:     S1AP Layer

     Type:     C source file

     Desc:     C Source code for Lower interface state machine on EnodeB

     File:     sq_smlienb.c

     Sid:      sq_smlienb.c@@/main/4 - Mon Jan 10 22:17:06 2011

     Prg:      sn

*********************************************************************21*/
/* Header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"
#include "envind.h"        /* environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System Services */
#include "cm_tkns.h"       /* common tokens */
#include "cm_hash.h"       /* common structs 1 */
#include "cm_mblk.h"       /* common memory */
#include "cm_llist.h"      /* cm link list */
#include "cm5.h"           /* common structs 3 */
#include "cm_inet.h"       /* common tokens  */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport */
#include "sct.h"           /* SCTP Layer */
#include "szt.h"           /* S1AP Upper Interface */
/* sz003.301: Updted for PSF-S1AP 1.1 release. Added include of FT/HA related
 * header files */
#ifdef SZ_FTHA
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_pftha.h"
#include "cm_psf.h"
#include "sht.h"           /* System Agent */
#endif /* SZ_FTHA */
#include "lsz.h"           /* S1AP LM Interface */
#include "sz.h"            /* S1AP Layer */
#include "cm_pasn.h"
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.h"
#endif

/* Header include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System Services */
#include "cm_tkns.x"       /* common tokens */
#include "cm_hash.x"       /* common structs 1 */
#include "cm_lib.x"        /* common library */
#include "cm_mblk.x"       /* common memory */
#include "cm_llist.x"      /* cm link list */
#include "cm5.x"           /* common structs 3 */
#include "cm_inet.x"       /* common transport */
#include "cm_tpt.x"        /* common transport */
/* sz006.301: Removed un-necessary include files  */
#include"szt_asn.x"        /* common ss7 */
#ifndef SZ_ENC_DEC
#include "szt_pk.x"
#include "szt_unpk.x"
#endif /* SZ_ENC_DEC */
#include "sct.x"           /* SCTP Layer */
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.x"
#endif
#include "szt.x"           /* S1AP Upper Interface */
/* sz003.301: Updted for PSF-S1AP 1.1 release. Added include of FT/HA related
 * header files */
#ifdef SZ_FTHA
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_pftha.x"
#include "cm_psf.x"
#include "sht.x"           /* System Agent */
#endif /* SZ_FTHA */
#include "lsz.x"           /* S1AP LM Interface */
#include "cm_pasn.x"
#include "sz.x"            /* S1AP Layer */

#ifdef SZ_ENB
#ifdef __cplusplus
EXTERN "C" {
#endif
PUBLIC S16 szSmRcvUeCtxRlsCmd ARGS((SzConCb *conCb,SzPeerCb *peerCb,S1apPdu *pdu));
#ifdef __cplusplus
}
#endif

PFRAM szSmLiEnb[SZ_MAX_MSG_ID][SZ_MAX_ST] =
{
   /* E-RAB setup request */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* E-RAB setup response */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* E-RAB modify request */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* E-RAB modify response */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* E-RAB release command */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* E-RAB release complete */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* E-RAB release request */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Initial Context setup request */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmRcvConnStMsg,      /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Initial Context setup response */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Initial Context setup failure */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* UE Context release request */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* UE Context release command */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmRcvUeCtxRlsCmd,    /* Connecting  */
      szSmRcvUeCtxRlsCmd,    /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /*  UE Context release complete */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* UE Context modification request */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* UE Context modification response */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* UE Context modification failure */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Initial UE message */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Downlink NAS transport */
   {
      szSmPeerErrHdlr,      /* Idle        */
      szSmRcvConnStMsg,      /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Uplink NAS transport */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* NAS non delivery indication */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Error indication */
   {
      szSmRcvIdleStErrInd,   /* Idle        */
      szSmRcvConnStErrInd,   /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmLclErrState        /* Releasing   */
   },
   /* No Data */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Reset */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Reset Acknowledge */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Setup Request */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Setup Response */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Setup Fail */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Paging */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Handover Required */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Handover Command */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Handover Preperation Failure */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Handover Request */
   {
      szSmRcvIdleStMsg,      /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Handover Request Acknowledge */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Handover Request Failure */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Handover Notify */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Path Switch Request */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Path Switch Request Acknowledge */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmRcvConnStMsg,      /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Path Switch Request Failure */
   {
      szSmIdleStPeerErr,          /* Idle        */
      szSmRcvConnStPathSwReqFail, /* Connecting  */
      szSmPeerErrHdlr,            /* Estbalished */
      szSmPeerErrHdlr             /* Releasing   */
   },
   /* Handover Cancel */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Handover Cancel Acknowledge */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* ENB Status Transfer */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* MME Status Transfer */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Deactivate Trace */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Trace Start */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Trace Failure Indication */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Location Reporting Control */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Location Report Failure Indication */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Location Report */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* ENB Configuration Update */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* ENB Configuration Update Acknowledge */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* ENB Configuration Update Failure */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* MME Configuration Update */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* MME Configuration Update Acknowledge */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* MME Configuration Update Failure */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Uplink CDMA 2000 Tunneling */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Downlink CDMA 2000 Tunneling */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* UE Capability Info Indication */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Overload Start */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Overload Stop */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Write Replace Warning Request */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Write Replace Warning Response */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* ENB Information Direct Transfer */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* MME Information Direct Transfer */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Private Message */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* ENB Configuration Transfer */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* MME Configuration Transfer */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Cell Traffic Trace */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   }
#ifndef S1AP_REL8A0
   ,
   /* Kill Request */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Established */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Kill Response */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Established */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Downlink UE Associated LPPa Transport  */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Established */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Uplink UE Associated LPPa Transport  */
   {
      szSmSemPeerErr,        /* Idle        */
      szSmSemPeerErr,        /* Connecting  */
      szSmSemPeerErr,        /* Established */
      szSmSemPeerErr         /* Releasing   */
   },
   /* Downlink non-UE Associated LPPa Transport  */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Established */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Uplink non-UE Associated LPPa Transport  */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Established */
      szSmPeerErrHdlr        /* Releasing   */
   }
#endif /* S1AP_REL8A0 */
};

/*
*
*       Fun:  szSmRcvUeCtxRlsCmd 
*
*       Desc:  Called to handle UE release command
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_smlienb.c
*
*/

#ifdef ANSI
PUBLIC S16 szSmRcvUeCtxRlsCmd 
(
SzConCb  *conCb,
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 szSmRcvUeCtxRlsCmd(conCb, peerCb, pdu)
SzConCb  *conCb;
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif
{
   S16 ret = RFAILED;

   SztRelInd relInd;

   TRC2(szSmRcvUeCtxRlsCmd)

   SZ_ZERO(&relInd, sizeof(SztRelInd));

   relInd.spConnId = conCb->cd.spConnId;
   /* LPR provided by S1AP team for the ccpu00125615 */
   if((conCb->state == SZ_SMSZ_ESTABLISHED) ||
      (conCb->conType == SZ_OUTGNG_CONN))
   {
      relInd.suConnId.pres = PRSNT_NODEF;
      relInd.suConnId.val = conCb->cd.suConnId;
   }
   else
   {
      relInd.suConnId.pres = NOTPRSNT;
   }
   relInd.pdu = pdu;

   /* Change the state to Releasing */
   conCb->state = SZ_SMSZ_RELEASING;

   /* Send decoded data to its User */
   ret = szUiSndRelInd(conCb, &relInd);

   RETVALUE(ret);
} /* end of szSmRcvUeCtxRlsCmd */
#endif

/**********************************************************************
 
         End of file:     sq_smlienb.c@@/main/4 - Mon Jan 10 22:17:06 2011
 
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
/main/4      ---      vvashishth  1. Updated for S1AP release 3.1
/main/4   sz003.301   pka   1. Updated for PSF-S1AP 1.1 release
/main/4   sz006.301   ve    1. Removed un-necessary include files
*********************************************************************91*/
