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

     Name:    RRC App

     Type:    C Include file

     Desc:    This file contains the acc source code

     File:    ve_ex_ms.c

     Sid:      ve_ex_ms.c@@/main/2 - Wed Dec  1 08:44:25 2010

     Prg:     an
*********************************************************************21*/

/* header include files (.h) */
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
#include "ve_eut.h"           /* MAC RRM control Interface */

/* RRM related includes */
#include "ve.h" 
#include "lve.h"
#ifdef VE_LNX_PCAP_PERF
#include <pcap.h>
#include "ve_rout.h"
#endif

#ifdef VE_PERF_MEAS 
#include "ve_perf.h"
#endif

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
#include "ve_eut.x"           /* MAC RRM control Interface */

#ifdef VE_PERF_MEAS 
#include "ve_perf.h"
#endif

/* RRM related includes */
#include "lve.x" 
#include "ve.x" 
/* MSPD changes */
#ifndef MSPD
#include "ve_dflt.h"
#else
#include "ve_dflt_ms.x"
#endif
#ifdef VE_LNX_PCAP_PERF
#include "ve_rout.x"
#endif


#ifdef VE_RELAY


/*
 *
 *       Fun:    veActvTsk - RRC user activation
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_ex_ms.c
 *
 */

#ifdef ANSI
PUBLIC S16 veActvTsk
(
Pst      *pst,
Buffer   *mBuf
)
#else
PUBLIC S16 veActvTsk(pst, mBuf)
Pst      *pst;
Buffer   *mBuf;
#endif /* ANSI */
{
   Mem sMem;
   /* Rrc Encode/Decode confirm Handling */
   Size     maxBlkSize;
   maxBlkSize = VE_MEM_PDU_SIZE;

   TRC3(veActvTsk);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (pst == (Pst *)NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veActvTsk: Invalid Pst \n"));
#endif

      if (mBuf != (Buffer *)NULLP)
      {
         (Void)SPutMsg(mBuf);
      }

      RETVALUE(RFAILED);
   }

   if (mBuf == (Buffer *)NULLP)
   {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veActvTsk: Invalid MBuf \n"));
#endif
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   /* Assigning the proper region */
   sMem.region = veCb.init.region;
   sMem.pool   = VE_MEM_POOL;

   switch (pst->srcEnt)
   {
      case ENTSM:
         switch (pst->event)
         {
            case EVTLVECFGREQ:
               cmUnpkLveCfgReq(VeMiLveCfgReq, pst, mBuf);
               break;

            case EVTLVECNTRLREQ:
               cmUnpkLveCntrlReq(VeMiLveCntrlReq, pst, mBuf);
               break;

            default:
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veActvTsk: Unknown Event from SM\n"));
#endif
               (Void)SPutMsg(mBuf);
               break;
         }
         break;

      case ENTNH: 
         switch(pst->event)
         {
            case EVTNHUBNDCFM:
               cmUnpkNhuBndCfm(NxLiNhuBndCfm, pst, mBuf);
               break;

            case EVTNHUERRIND:
               cmUnpkNhuErrInd(NxLiNhuErrInd, pst, mBuf);
               break;

            case EVTNHUDATIND:
               cmUnpkNhuDatInd(NxLiNhuDatInd, pst, mBuf,&sMem, VE_MEM_PDU_SIZE);
               break;

            case EVTNHUCFGCFM:
               cmUnpkNhuCfgCfm(NxLiNhuCfgCfm, pst, mBuf);
               break;

            case  EVTNHUCELLCFGCFM :
               cmUnpkNhuCellCfgCfm(NxLiNhuCellCfgCfm, pst, mBuf);
               break;
            /* Rrc Encode/Decode confirm Handling */
            case  EVTNHUENCODECFM:
               cmUnpkNhuEncodeCfm(NxLiNhuEncodeCfm, pst, mBuf);
               break;
            case  EVTNHUDCODECFM:
               cmUnpkNhuDecodeCfm(NxLiNhuDecodeCfm, pst, mBuf, &sMem, maxBlkSize);
               break;
            default:
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veActvTsk: Unknown Evnt from NHU.\n"));
#endif
               (Void)SPutMsg(mBuf);
               break;
         }
         break;

      case ENTRG:
         switch (pst->event)
         {
            case EVTRGRBNDCFM:
               cmUnpkRgrBndCfm(NxLiRgrBndCfm, pst, mBuf);
               break;

            case EVTRGRCFGCFM:
               cmUnpkRgrCfgCfm(NxLiRgrCfgCfm, pst, mBuf);
               break;

#ifdef RGR_RRM_TICK
            case EVTRGRTTIIND: /* Indication every 10ms */
               cmUnpkRgrTtiInd(NxLiRgrTtiInd, pst, mBuf);
               break;
#endif
            default:
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veActvTsk: Unknown event from RGR\n"));
#endif
               (Void)SPutMsg(mBuf);
               break;
         } 
         break;

      case ENTTF:
         switch (pst->event)
         {
            case EVTCTFBNDCFM:
               cmUnpkCtfBndCfm (VeLiCtfBndCfm, pst, mBuf);
               break;

            case EVTCTFCFGCFM:
               cmUnpkCtfCfgCfm (VeLiCtfCfgCfm, pst, mBuf);
               break;

#ifdef WIRESHARK_LOG
            case EVTCTFWIRESHARK:
               veHandleWireSharkMsg(mBuf);
               break;
#endif

            default:
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veActvTsk: Unknown event from CTF \n"));
#endif
               (Void)SPutMsg(mBuf);
               break;
         } 
         break;
#ifndef LTE_ENB_PERF
      case ENTSZ:
         switch (pst->event)
         {
            case EVTSZTBNDCFM:
               (Void)cmUnpkSztBndCfm(UzLiSztBndCfm, pst, mBuf);
               break;

            case EVTSZTSTAIND:
               (Void)cmUnpkSztStaInd(UzLiSztStaInd, pst, mBuf);
               break;

            case EVTSZTERRIND:
               (Void)cmUnpkSztLclErrInd(UzLiSztErrInd, pst, mBuf);
               break;

            case EVTSZTUDATIND:
               (Void)cmUnpkSztUDatInd(UzLiSztUDatInd, pst, mBuf, &sMem);
               break;

            case EVTSZTCONIND:
               (Void)cmUnpkSztConInd(UzLiSztConInd, pst, mBuf, &sMem);
               break;

            case EVTSZTCONCFM:
               (Void)cmUnpkSztConCfm(UzLiSztConCfm, pst, mBuf, &sMem);
               break;

            case EVTSZTRELIND:
               (Void)cmUnpkSztRelInd(UzLiSztRelInd, pst, mBuf, &sMem);
               break;

            case EVTSZTRELCFM:
               (Void)cmUnpkSztRelCfm(UzLiSztRelCfm, pst, mBuf, &sMem);
               break;

            case EVTSZTDATIND:
               (Void)cmUnpkSztDatInd(UzLiSztDatInd, pst, mBuf, &sMem);
               break;

            default:
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veActvTsk: Unknown event from SZT \n"));
#endif
               (Void)SPutMsg(mBuf);
               break;
         }
         break;
#ifdef LTE_HO_SUPPORT
      case ENTCZ:
         switch (pst->event)
         {
            case EVTCZTBNDCFM:
            {
               (Void)cmUnpkCztBndCfm(RsLiCztBndCfm, pst, mBuf);
               break;
            }

            case EVTCZTGPIND:
            {
               (Void)cmUnpkCztGpInd(RsLiCztGpInd, pst, mBuf); 
               break;
            }

            case EVTCZTGPCFM:
            {
               (Void)cmUnpkCztGpCfm(RsLiCztGpCfm, pst, mBuf); 
               break;
            }

            case EVTCZTBMPIND:
            {
               (Void)cmUnpkCztBmpInd(RsLiCztBmpInd, pst, mBuf); 
               break;
            }
            case EVTCZTBMPCFM:
            {
               (Void)cmUnpkCztBmpCfm(RsLiCztBmpCfm, pst, mBuf); 
               break;
            }
            case EVTCZTGPERRIND:
            {
               (Void)cmUnpkCztGpErrInd(RsLiCztGpErrInd, pst, mBuf); 
                break;
            }
            case EVTCZTERRIND:
            {
               (Void)cmUnpkCztLclErrorInd(RsLiCztLclErrorInd, pst, mBuf); 
               break;
            }

            case EVTCZTSTAIND:
            {
               (Void)cmUnpkCztStatusInd(RsLiCztStatusInd, pst, mBuf);
               break;
            }
            case EVTCZTPEERASSOCIND:
            {
               (Void)cmUnpkCztDynamPeerAssocInd(RsLiCztDynamPeerAssocInd, pst, mBuf);
               break;
            }

            case EVTCZTLOCALRELCFM:
            {
               (Void)cmUnpkCztRelCfm(RsLiCztRelCfm, pst, mBuf);
               break;
            }

            case EVTCZTAUDCFM:
            {
               (Void)cmUnpkCztAudCfm(RsLiCztAudCfm, pst, mBuf);
               break;
            }

            default:
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veActvTsk: Unknown event from CZT \n"));
#endif
               (Void)SPutMsg(mBuf);
               break;
            }
         }
         break;
#endif /* End of LTE_HO_SUPPORT */
#endif
#ifdef EU_DAT_APP
#ifndef LTE_ENB_PERF
      case ENTEG:
         switch (pst->event)
         {
            case EVTEGTBNDCFM:
               (Void)cmUnpkEgtBndCfm(EuLiEgtBndCfm, pst, mBuf);
               break;

            case EVTEGTUDATIND:
               (Void)cmUnpkEgtEguDatInd(VeLiEgtDatInd, pst, mBuf);
               break;

            case EVTEGTULCLTNLMGMTCFM:
               (Void)cmUnpkEgtEguLclTnlMgmtCfm(EuLiEgtEguLclTnlMgmtCfm, pst, mBuf);
               break;

            case EVTEGTUSTACFM:
               (Void)cmUnpkEgtEguStaCfm(EuLiEgtEguStaCfm, pst, mBuf);
               break;

            case EVTEGTUUSTAIND:
               (Void)cmUnpkEgtEguUStaInd(EuLiEgtEguUStaInd, pst, mBuf);
               break;

            case EVTEGTUERRIND:
               (Void)cmUnpkEgtEguErrInd(EuLiEgtEguErrInd, pst, mBuf);
               break;

            default:
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veActvTsk: Unknown event from EGT\n"));
#endif
               (Void)SPutMsg(mBuf);
               break;
         }
         break;
#endif
      case ENTVE:
         switch (pst->event)
         {
#ifndef LTE_ENB_PERF
            case  EVTVECREATEEGTPTUNELCFM:
               cmUnpkEuCreattLocTunCfm(VeHdlCreatEgtpTunelCfm, pst, mBuf,&sMem);
               break;

            case  EVTVEDELEGTPTUNELCFM:
               cmUnpkEuDelLocTunCfm(VeHdlDelEgtpTunelCfm, pst, mBuf,&sMem);
               break;

            case  EVTVECREATEEGTPTUNEL:
               cmUnpkEuCreattLocTunReq(VeDatCreatEgtpTunel, pst, mBuf,&sMem);
               break;

            case  EVTVEMODIFYEGTPTUNEL:
               (Void)SPutMsg(mBuf);
               break;

            case EVTVEDELEGTPTUNEL:
               cmUnpkEuDelLocTunReq(VeDatDelEgtpTunel, pst, mBuf,&sMem);
               break;

            case EVTVERELREQ:
               cmUnpkEuRelReq(VeSndRelReq, pst, mBuf,&sMem);
               break;

#ifdef LTE_HO_SUPPORT
            case EVTVECHANGESTATE:
               cmUnpkEuChngStReq(veChngTnlState, pst, mBuf,&sMem);
               break;

            case EVTVECREATEGTPTUNELSET:
               cmUnpkEuCreattLocTunSetReq(VeDatCreateEgtpTunelSet, pst, mBuf,&sMem);
               break;
#endif

#endif

#ifdef VE_PERF_MEAS
            case VE_NOTIFY_SATRT_DATA:
#ifdef VE_LNX_PCAP_PERF
               /* libpcap receive init */
               veAppRoutPerfStartPcap(pst, mBuf, &sMem);
#else
               vePerfStartData(pst, mBuf, &sMem); 
#endif
               break;
#endif

            default:
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veActvTsk: Unknown event from internal\
                        EU Interface\n"));
#endif
               (Void)SPutMsg(mBuf);
               break;
         }
         break;

      case ENTPJ:
      case ENTKW:
         switch (pst->event)
         {
            case  EVTPJUBNDCFM:
               cmUnpkPjuBndCfm(PxLiPjuBndCfm, pst, mBuf);
               break;

            case  EVTPJUDATIND:
               cmUnpkPjuDatInd(PxLiPjuDatInd, pst, mBuf);
               break;

            case EVTPJUDATFWDIND:
               cmUnpkPjuDatFwdInd(PxLiPjuDatFwdInd, pst, mBuf);
               break;

            default:
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veActvTsk: Unknown event from PJU \n"));
#endif
               (Void)SPutMsg(mBuf);
               break;
         }

         break;

      default:
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veActvTsk: Received Msg From Unknow Src.\n"));
#endif
         (Void)SPutMsg(mBuf);
         break;
   }

#endif
   (Void)SExitTsk();
   RETVALUE(ROK);
} /* end of veActvTsk() */

#endif

/********************************************************************30**
  
         End of file:     ve_ex_ms.c@@/main/2 - Wed Dec  1 08:44:25 2010
  
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
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
