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

/************************************************************************
 
     Name:     LTE MAC Convergence Layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     ys_ptui.c
  
     Sid:      yw_ptui.c@@/main/2 - Thu Dec  2 02:27:26 2010
  
     Prg:      rk
  
**********************************************************************/

/** @file ys_ptui.c
@brief This module acts as an interface handler for upper interface and 
manages Pst and Sap related information for upper interface APIs.
*/

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_lte.h"
#include "ctf.h"           /* CTF defines */
#include "lys.h"           /* layer management defines for LTE-CL */
#include "tfu.h"
#ifdef YS_MSPD
#include "LtePhyL2Api.h"    /* Silicon Includes */
#include "apidefs.h"
#include "ys_ms.h"          /* defines and macros for CL */
#else
#include "ys.h"
#include <sys/time.h>
#endif
#include "ys_err.h"        /* YS error defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"
#include "ctf.x"           /* CTF types */
#include "lys.x"           /* layer management typedefs for CL */
#include "tfu.x"
#ifdef YS_MSPD
#include "ys_ms.x"         /* typedefs for CL */
#else
#include "ys.x"            /* typedefs for CL */
#endif
#ifdef VE

#ifdef __cplusplus /* ys004.102: name mangling */
EXTERN "C" {
#endif  /*__cplusplus*/

   EXTERN S16 VeLiCtfBndCfm ARGS(( Pst *pst, SuId suId, U8 status));
   EXTERN S16 VeLiCtfCfgCfm ARGS(( Pst *pst, SuId suId, CtfCfgTransId transId,U8 status));
#ifdef __cplusplus
}
#endif /* __cplusplus */

EXTERN S16 VeLiCtfUeIdChgCfm ARGS(( Pst *pst, SuId suId, CtfCfgTransId transId,CtfUeInfo *ueInfo,U8 status));
#endif

#if !(defined(LCYSUITFU)  && defined(RG) && defined(LWLCYSUITFU))
#define PTYSUITFU
#endif

#if !(defined(LCYSUICTF)  && defined(YS) && defined(LWLCYSUICTF))
#define PTYSUICTF
#endif


/* MAX Number of Service Users of YS */
#define YS_MAX_TFU_USR   4

/* MAX Number of Service Users of YS */
#define YS_MAX_CTF_USR   4

#ifdef PTYSUITFU
/** @brief This API is used to receive a Bind confirm from CL to MAC.
*/
EXTERN S16 PtUiTfuBndCfm ARGS((Pst* pst, SuId suId, U8 status));
/** @brief This API is used to receive a Bind confirm from CL to scheduler.
*/
EXTERN S16 PtUiTfuSchBndCfm ARGS((Pst* pst, SuId suId, U8 status));
/** @brief This API is used to indication Random Access Request reception from
 * CL to scheduler.
 */
EXTERN S16 PtUiTfuRaReqInd ARGS((Pst * pst, SuId suId, TfuRaReqIndInfo * raReqInd));
/** @brief This API is used to indicate CQI reporting from CL to scheduler.
*/
EXTERN S16 PtUiTfuUlCqiInd ARGS((Pst * pst, SuId suId, TfuUlCqiIndInfo * ulCqiInd));
/** @brief This API is used to indicate HARQ feedback from CL to scheduler.
*/
EXTERN S16 PtUiTfuHqInd ARGS((Pst * pst, SuId suId, TfuHqIndInfo * hqInd));
/** @brief This API is used to indicate a SR reception from CL to scheduler..
*/
EXTERN S16 PtUiTfuSrInd ARGS((Pst * pst, SuId suId, TfuSrIndInfo * srInd));
/** @brief This API is used to indicate the reception of CQI report from CL to
 * scheduler..
 */
EXTERN S16 PtUiTfuDlCqiInd ARGS((Pst * pst, SuId suId, TfuDlCqiIndInfo * dlCqiInd));
/** @brief This API is used to indicate Data Reception from CL to MAC.
*/
EXTERN S16 PtUiTfuDatInd ARGS((Pst * pst, SuId suId, TfuDatIndInfo * datInd));
/** @brief This API is used to indicate Decode failure from CL to scheduler..
*/
EXTERN S16 PtUiTfuCrcInd ARGS((Pst * pst, SuId suId, TfuCrcIndInfo * crcInd));
/** @brief This API is used to indicate a Timing Advance from PAL to scheduler..
*/
EXTERN S16 PtUiTfuTimingAdvInd ARGS((Pst * pst, SuId suId, TfuTimingAdvIndInfo * timingAdvInd));
/** @brief This API is the TTI indication from CL to MAC.
*/
EXTERN S16 PtUiTfuTtiInd ARGS((Pst * pst, SuId suId, TfuTtiIndInfo * ttiInd));
/** @brief This API is the TTI indication from CL to scheduler.
*/
EXTERN S16 PtUiTfuSchTtiInd ARGS((Pst * pst, SuId suId, TfuTtiIndInfo * ttiInd));
/** @brief This API is used to indicate PUCCH power delta  from CL to scheduler..
*/
EXTERN S16 PtUiTfuPucchDeltaPwrInd ARGS((Pst * pst, SuId suId, TfuPucchDeltaPwrIndInfo* pucchDeltaPwrInd));
#endif /*--#ifdef PTYSUITFU--*/

#ifdef PTYSUICTF
/** @brief This API is used to receive a Bind confirm from CL to MAC.
*/
EXTERN S16 PtUiCtfBndCfm ARGS((Pst* pst, SuId suId, U8 status));
/** @brief This API is used to receive a Configuration confirm from CL to MAC.
*/
EXTERN S16 PtUiCtfCfgCfm ARGS((Pst* pst, SuId suId, 
         CtfCfgTransId  transId, U8 status));
/***************SEC_CHANGE_START*****************/
#if 0
#ifdef CTF_AS_SEC_ENB
/** @brief This API is used to receive a Data Response from CL to eNodeB Application.
*/		 
EXTERN S16 PtUiCtfDatRsp ARGS((Pst* pst, SpId spId, CtfDatRspSdus *DatRspSdu));
#endif
#endif
/***************SEC_CHANGE_END*****************/
#endif

/** @brief This API is used to receive a Bind confirm from CL to MAC.
*/
PRIVATE CONSTANT TfuBndCfm YsUiTfuBndCfmMt[YS_MAX_TFU_USR] =
{
#ifdef LCYSUITFU
   cmPkTfuBndCfm,
#else
   PtUiTfuBndCfm,
#endif
#ifdef RG
   RgLiTfuBndCfm,
#else
   PtUiTfuBndCfm,
#endif
#ifdef LWLCYSUITFU
   cmPkTfuBndCfm
#else
      PtUiTfuBndCfm
#endif
};

PRIVATE CONSTANT TfuSchBndCfm YsUiTfuSchBndCfmMt[YS_MAX_TFU_USR] =
{
#ifdef LCYSUITFU
   cmPkTfuSchBndCfm,
#else
   PtUiTfuSchBndCfm,
#endif
#ifdef RG
   RgLiTfuSchBndCfm,
#else
   PtUiTfuSchBndCfm,
#endif
#ifdef LWLCYSUITFU
   cmPkTfuSchBndCfm
#else
      PtUiTfuSchBndCfm
#endif
};

#if 0
/** @brief This API is used to indication Random Access Request reception from
 * CL to MAC.
 */
PRIVATE CONSTANT TfuRaReqInd YsUiTfuRaReqIndMt[YS_MAX_TFU_USR] =
{
#ifdef LCYSUITFU
   cmPkTfuRaReqInd,
#else
   PtUiTfuRaReqInd,
#endif
#ifdef RG
   RgLiTfuRaReqInd,
#else
   PtUiTfuRaReqInd,
#endif
#ifdef LWLCYSUITFU
   cmPkTfuRaReqInd
#else
      PtUiTfuRaReqInd
#endif
};

/** @brief This API is used to indicate CQI reporting from CL to MAC
*/
PRIVATE CONSTANT TfuUlCqiInd YsUiTfuUlCqiIndMt[YS_MAX_TFU_USR] =
{
#ifdef LCYSUITFU
   cmPkTfuUlCqiInd,
#else
   PtUiTfuUlCqiInd,
#endif
#ifdef RG
   RgLiTfuUlCqiInd,
#else
   PtUiTfuUlCqiInd,
#endif
#ifdef LWLCYSUITFU
   cmPkTfuUlCqiInd
#else
      PtUiTfuUlCqiInd
#endif
};

/** @brief This API is used to indicate HARQ feedback from CL to MAC
*/
PRIVATE CONSTANT TfuHqInd YsUiTfuHqIndMt[YS_MAX_TFU_USR] =
{
#ifdef LCYSUITFU
   cmPkTfuHqInd,
#else
   PtUiTfuHqInd,
#endif
#ifdef RG
   RgLiTfuHqInd,
#else
   PtUiTfuHqInd,
#endif
#ifdef LWLCYSUITFU
   cmPkTfuHqInd
#else
      PtUiTfuHqInd
#endif
};

/** @brief This API is used to indicate a SR reception from CL to MAC.
*/
PRIVATE CONSTANT TfuSrInd YsUiTfuSrIndMt[YS_MAX_TFU_USR] =
{
#ifdef LCYSUITFU
   cmPkTfuSrInd,
#else
   PtUiTfuSrInd,
#endif
#ifdef RG
   RgLiTfuSrInd,
#else
   PtUiTfuSrInd,
#endif
#ifdef LWLCYSUITFU
   cmPkTfuSrInd
#else
      PtUiTfuSrInd
#endif
};
#if 0
#ifdef TFU_UPGRADE
/** @brief This API is used to indicate the reception of CQI report from CL to
 * MAC.
 */
PRIVATE CONSTANT TfuRawCqiInd YsUiTfuRawCqiIndMt[YS_MAX_TFU_USR] =
{
#ifdef LCYSUITFU
   cmPkTfuRawCqiInd,
#else
   PtUiTfuRawCqiInd,
#endif
#ifdef RG
   RgLiTfuRawCqiInd,
#else
   PtUiTfuRawCqiInd,
#endif
#ifdef LWLCYSUITFU
   cmPkTfuRawCqiInd
#else
   PtUiTfuRawCqiInd
#endif
};
#endif
#endif
/** @brief This API is used to indicate the reception of CQI report from CL to
 * MAC.
 */
PRIVATE CONSTANT TfuDlCqiInd YsUiTfuDlCqiIndMt[YS_MAX_TFU_USR] =
{
#ifdef LCYSUITFU
   cmPkTfuDlCqiInd,
#else
   PtUiTfuDlCqiInd,
#endif
#ifdef RG
   RgLiTfuDlCqiInd,
#else
   PtUiTfuDlCqiInd,
#endif
#ifdef LWLCYSUITFU
   cmPkTfuDlCqiInd
#else
      PtUiTfuDlCqiInd
#endif
};

/** @brief This API is used to indicate Data Reception from CL to MAC.
*/
PRIVATE CONSTANT TfuDatInd YsUiTfuDatIndMt[YS_MAX_TFU_USR] =
{
#ifdef LCYSUITFU
   cmPkTfuDatInd,
#else
   PtUiTfuDatInd,
#endif
#ifdef RG
   RgLiTfuDatInd,
#else
   PtUiTfuDatInd,
#endif
#ifdef LWLCYSUITFU
   cmPkTfuDatInd
#else
      PtUiTfuDatInd
#endif
};

/** @brief This API is used to indicate Decode failure from CL to MAC.
*/
PRIVATE CONSTANT TfuCrcInd YsUiTfuCrcIndMt[YS_MAX_TFU_USR] =
{
#ifdef LCYSUITFU
   cmPkTfuCrcInd,
#else
   PtUiTfuCrcInd,
#endif
#ifdef RG
   RgLiTfuCrcInd,
#else
   PtUiTfuCrcInd,
#endif
#ifdef LWLCYSUITFU
   cmPkTfuCrcInd
#else
      PtUiTfuCrcInd
#endif
};

/** @brief This API is used to indicate a Timing Advance from CL to MAC.
*/
PRIVATE CONSTANT TfuTimingAdvInd YsUiTfuTimingAdvIndMt[YS_MAX_TFU_USR] =
{
#ifdef LCYSUITFU
   cmPkTfuTimingAdvInd,
#else
   PtUiTfuTimingAdvInd,
#endif
#ifdef RG
   RgLiTfuTimingAdvInd,
#else
   PtUiTfuTimingAdvInd,
#endif
#ifdef LWLCYSUITFU
   cmPkTfuTimingAdvInd
#else
      PtUiTfuTimingAdvInd
#endif
};

/** @brief This API is the TTI indication from CL to MAC.
*/
PRIVATE CONSTANT TfuTtiInd YsUiTfuTtiIndMt[YS_MAX_TFU_USR] =
{
#ifdef LCYSUITFU
   cmPkTfuTtiInd,
#else
   PtUiTfuTtiInd,
#endif
#ifdef RG
   RgLiTfuTtiInd,
#else
   PtUiTfuTtiInd,
#endif
#ifdef LWLCYSUITFU
   cmPkTfuTtiInd
#else
      PtUiTfuTtiInd
#endif
};

/** @brief This API is the TTI indication from CL to scheduler.
*/
PRIVATE CONSTANT TfuSchTtiInd YsUiTfuSchTtiIndMt[YS_MAX_TFU_USR] =
{
#ifdef LCYSUITFU
   cmPkTfuSchTtiInd,
#else
   PtUiTfuSchTtiInd,
#endif
#ifdef RG
   RgLiTfuSchTtiInd,
#else
   PtUiTfuSchTtiInd,
#endif
#ifdef LWLCYSUITFU
   cmPkTfuSchTtiInd
#else
      PtUiTfuSchTtiInd
#endif
};

/** @brief This API is the PUCCH delta power indication from CL to scheduler.
*/
PRIVATE CONSTANT TfuPucchDeltaPwrInd YsUiTfuPucchDeltaPwrIndMt[YS_MAX_TFU_USR] =
{
#ifdef LCYSUITFU
  cmPkTfuPucchDeltaPwrIndInfo,
#else
  PtUiTfuPucchDeltaPwrInd,
#endif
#ifdef RG
   RgLiTfuPucchDeltaPwrInd,
#else
   PtUiTfuPucchDeltaPwrInd,
#endif
#ifdef LWLCYSUITFU
   cmPkTfuPucchDeltaPwrIndInfo
#else
    PtUiTfuPucchDeltaPwrInd
#endif
};

#endif

/** @brief This API is used to receive a Bind confirm from CL to RRM.
*/
PRIVATE CONSTANT CtfBndCfm YsUiCtfBndCfmMt[YS_MAX_CTF_USR] =
{
#ifdef LCYSUICTF
   cmPkCtfBndCfm,
#else
   PtUiCtfBndCfm,
#endif
#ifdef VE
  VeLiCtfBndCfm,
#else
   PtUiCtfBndCfm,
#endif
#ifdef LWLCYSUICTF
   cmPkCtfBndCfm,
#else
   PtUiCtfBndCfm,
#endif
#if 0   
#ifdef NH
   NhLiCtfBndCfm
#else
   PtUiCtfBndCfm
#endif
#endif   
};

/** @brief This API is used to receive a Config confirm from CL to RRM.
 */
PRIVATE CONSTANT CtfCfgCfm YsUiCtfCfgCfmMt[YS_MAX_CTF_USR] =
{
#ifdef LCYSUICTF
   cmPkCtfCfgCfm,
#else
   PtUiCtfCfgCfm,
#endif
#ifdef VE
  VeLiCtfCfgCfm,
#else
   PtUiCtfCfgCfm,
#endif
#ifdef LWLCYSUICTF
   cmPkCtfCfgCfm,
#else
   PtUiCtfCfgCfm,
#endif
#if 0   
#ifdef NH
   NhLiCtfCfgCfm
#else
   PtUiCtfCfgCfm
#endif
#endif
};

/***************SEC_CHANGE_START*****************/
#if 0
#ifdef CTF_AS_SEC_ENB
PRIVATE CtfDatRsp YsUiCtfDatRspMt [YS_MAX_CTF_USR] =
{
#ifdef LCYSUICTF
   cmPkCtfDatRsp,            /* 0 - loosely coupled */
#else
   PtUiCtfDatRsp,            /* 0 - loosely coupled,portable */
#endif
#ifdef VE
   VeUiCtfDatRsp,            /* 2 - tightly coupled,stub layer */
#else
   PtUiCtfDatRsp,            /* 2 - tightly coupled,portable */
#endif
#ifdef LWLCYSUICTF
   cmPkCtfDatRsp            /* 1 - light weight loosely coupled */
#else
   PtUiCtfDatRsp            /* 1 - light weight loosely coupled,portable */
#endif
};
#endif
#endif
/***************SEC_CHANGE_END*****************/

#ifdef YS



/**
* @brief This API is used to receive a Bind confirm from CL to MAC.
*
* @details
*
*     Function : YsUiTfuBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 YsUiTfuBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 YsUiTfuBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(YsUiTfuBndCfm);

   (*YsUiTfuBndCfmMt[pst->selector])(pst, suId, status);

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 YsUiTfuSchBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 YsUiTfuSchBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(YsUiTfuSchBndCfm);

   (*YsUiTfuSchBndCfmMt[pst->selector])(pst, suId, status);

   RETVALUE(ROK);
}


/**
* @brief This API is used to indication Random Access Request reception from
 * CL to MAC.
*
* @details
*
*     Function : YsUiTfuRaReqInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuRaReqIndInfo *  raReqInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 YsUiTfuRaReqInd
(
Pst * pst,
SuId suId,
TfuRaReqIndInfo * raReqInd
)
#else
PUBLIC S16 YsUiTfuRaReqInd(pst, suId, raReqInd)
Pst * pst;
SuId suId;
TfuRaReqIndInfo * raReqInd;
#endif
{

   TRC3(YsUiTfuRaReqInd);
#if 0
   (*YsUiTfuRaReqIndMt[pst->selector])(pst, suId, raReqInd);
#endif
   RETVALUE(ROK);

}



/**
* @brief This API is used to indicate CQI reporting from CL to MAC
*
* @details
*
*     Function : YsUiTfuUlCqiInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuUlCqiIndInfo *  ulCqiInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 YsUiTfuUlCqiInd
(
Pst * pst,
SuId suId,
TfuUlCqiIndInfo * ulCqiInd
)
#else
PUBLIC S16 YsUiTfuUlCqiInd(pst, suId, ulCqiInd)
Pst * pst;
SuId suId;
TfuUlCqiIndInfo * ulCqiInd;
#endif
{
#ifdef MSPD_MLOG_NEW
   volatile int t = GetTIMETICK();
#endif

   TRC3(YsUiTfuUlCqiInd);
#if 0
   (*YsUiTfuUlCqiIndMt[pst->selector])(pst, suId, ulCqiInd);
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_MAC_UL_CQI_IND, RESOURCE_LARM, t, GetTIMETICK());
#endif
#endif
   RETVALUE(ROK);

}



/**
* @brief This API is used to indicate HARQ feedback from CL to MAC
*
* @details
*
*     Function : YsUiTfuHarqAckInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuHarqAckIndInfo *  harqAckInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 YsUiTfuHqInd
(
Pst * pst,
SuId suId,
TfuHqIndInfo      *hqInd
)
#else
PUBLIC S16 YsUiTfuHqInd(pst, suId, hqInd)
Pst * pst;
SuId suId;
TfuHqIndInfo      *hqInd;
#endif
{
#ifdef MSPD_MLOG_NEW
   volatile int t = GetTIMETICK();
#endif
   TRC3(YsUiTfuHqInd);
#if 0
   (*YsUiTfuHqIndMt[pst->selector])(pst, suId, hqInd);
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_MAC_HARQ_IND, RESOURCE_LARM, t, GetTIMETICK());
#endif
#endif   
   RETVALUE(ROK);

}



/**
* @brief This API is used to indicate a SR reception from CL to MAC.
*
* @details
*
*     Function : YsUiTfuSrInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuSrIndInfo *  srInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 YsUiTfuSrInd
(
Pst * pst,
SuId suId,
TfuSrIndInfo * srInd
)
#else
PUBLIC S16 YsUiTfuSrInd(pst, suId, srInd)
Pst * pst;
SuId suId;
TfuSrIndInfo * srInd;
#endif
{

#ifdef MSPD_MLOG_NEW
   volatile int t = GetTIMETICK();
#endif
   TRC3(YsUiTfuSrInd);

#if 0
   (*YsUiTfuSrIndMt[pst->selector])(pst, suId, srInd);
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_MAC_SR_IND, RESOURCE_LARM, t, GetTIMETICK());
#endif
#endif

   RETVALUE(ROK);

}
#if 0
#ifdef TFU_UPGRADE

/**
* @brief This API is used to indicate the reception of CQI report from CL to
 * MAC.
*
* @details
*
*     Function : YsUiTfuRawCqiInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuRawCqiIndInfo *  dlCqiInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 YsUiTfuRawCqiInd
(
Pst * pst,
SuId suId,
TfuRawCqiIndInfo * dlCqiInd
)
#else
PUBLIC S16 YsUiTfuRawCqiInd(pst, suId, dlCqiInd)
Pst * pst;
SuId suId;
TfuRawCqiIndInfo * dlCqiInd;
#endif
{
#ifdef MSPD_MLOG_NEW
   volatile int t = GetTIMETICK();
#endif
   TRC3(YsUiTfuRawCqiInd);

   (*YsUiTfuRawCqiIndMt[pst->selector])(pst, suId, dlCqiInd);
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_MAC_DL_CQI_IND, RESOURCE_LARM, t, GetTIMETICK());
#endif
   RETVALUE(ROK);

}
#endif
#endif

/**
* @brief This API is used to indicate the reception of CQI report from CL to
 * MAC.
*
* @details
*
*     Function : YsUiTfuDlCqiInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuDlCqiIndInfo *  dlCqiInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 YsUiTfuDlCqiInd
(
Pst * pst,
SuId suId,
TfuDlCqiIndInfo * dlCqiInd
)
#else
PUBLIC S16 YsUiTfuDlCqiInd(pst, suId, dlCqiInd)
Pst * pst;
SuId suId;
TfuDlCqiIndInfo * dlCqiInd;
#endif
{
#ifdef MSPD_MLOG_NEW
   volatile int t = GetTIMETICK();
#endif
   TRC3(YsUiTfuDlCqiInd);
#if 0
   (*YsUiTfuDlCqiIndMt[pst->selector])(pst, suId, dlCqiInd);
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_MAC_DL_CQI_IND, RESOURCE_LARM, t, GetTIMETICK());
#endif
#endif   
   RETVALUE(ROK);

}



/**
* @brief This API is used to indicate Data Reception from CL to MAC.
*
* @details
*
*     Function : YsUiTfuDatInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuDatIndInfo *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 YsUiTfuDatInd
(
Pst * pst,
SuId suId,
TfuDatIndInfo * datInd
)
#else
PUBLIC S16 YsUiTfuDatInd(pst, suId, datInd)
Pst * pst;
SuId suId;
TfuDatIndInfo * datInd;
#endif
{
#ifdef MSPD_MLOG_NEW
   volatile int t = GetTIMETICK();
#endif
   TRC3(YsUiTfuDatInd);

   /* No need of assigning selector as this is decided with configuration */
#if 0
   (*YsUiTfuDatIndMt[pst->selector])(pst, suId, datInd);
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_MAC_DAT_IND, RESOURCE_LARM, t, GetTIMETICK());
#endif
#endif
   RETVALUE(ROK);
}



/**
* @brief This API is used to indicate Decode failure from CL to MAC.
*
* @details
*
*     Function : YsUiTfuCrcInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuCrcIndInfo *  crcInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 YsUiTfuCrcInd
(
Pst * pst,
SuId suId,
TfuCrcIndInfo * crcInd
)
#else
PUBLIC S16 YsUiTfuCrcInd(pst, suId, crcInd)
Pst * pst;
SuId suId;
TfuCrcIndInfo * crcInd;
#endif
{
#ifdef MSPD_MLOG
   volatile int t = GetTIMETICK();
#endif

   TRC3(YsUiTfuCrcInd);

#if 0
   (*YsUiTfuCrcIndMt[pst->selector])(pst, suId, crcInd);
#ifdef MSPD_MLOG
   MLogTask(PID_UL_SCHEDULING, RESOURCE_LARM, t, GetTIMETICK());
#endif
#endif
   RETVALUE(ROK);
}



/**
* @brief This API is used to indicate a Timing Advance from CL to MAC.
*
* @details
*
*     Function : YsUiTfuTimingAdvInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuTimingAdvIndInfo *  timingAdvInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 YsUiTfuTimingAdvInd
(
Pst * pst,
SuId suId,
TfuTimingAdvIndInfo * timingAdvInd
)
#else
PUBLIC S16 YsUiTfuTimingAdvInd(pst, suId, timingAdvInd)
Pst * pst;
SuId suId;
TfuTimingAdvIndInfo * timingAdvInd;
#endif
{
#ifdef MSPD_MLOG_NEW
   volatile int t = GetTIMETICK();
#endif

   TRC3(YsUiTfuTimingAdvInd);
#if 0
   (*YsUiTfuTimingAdvIndMt[pst->selector])(pst, suId, timingAdvInd);
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_MAC_TA_IND, RESOURCE_LARM, t, GetTIMETICK());
#endif
#endif
   RETVALUE(ROK);

}



/**
* @brief This API is the TTI indication from CL to MAC.
*
* @details
*
*     Function : YsUiTfuTtiInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuTtiIndInfo *  ttiInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 YsUiTfuTtiInd
(
Pst * pst,
SuId suId,
TfuTtiIndInfo * ttiInd
)
#else
PUBLIC S16 YsUiTfuTtiInd(pst, suId, ttiInd)
Pst * pst;
SuId suId;
TfuTtiIndInfo * ttiInd;
#endif
{
#ifdef MSPD_MLOG_NEW
   volatile int t = GetTIMETICK();
#endif
   TRC3(YsUiTfuTtiInd);
#if 0
   /* No need of assigning selector as this is decided with configuration */
   (*YsUiTfuTtiIndMt[pst->selector])(pst, suId, ttiInd);
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_MAC_TTI_IND, RESOURCE_LARM, t, GetTIMETICK());
#endif
#endif
   RETVALUE(ROK);

}

/**
* @brief This API is the TTI indication from CL to scheduler.
*
* @details
*
*     Function : YsUiTfuSchTtiInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuTtiIndInfo *  ttiInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 YsUiTfuSchTtiInd
(
Pst * pst,
SuId suId,
TfuTtiIndInfo * ttiInd
)
#else
PUBLIC S16 YsUiTfuSchTtiInd(pst, suId, ttiInd)
Pst * pst;
SuId suId;
TfuTtiIndInfo * ttiInd;
#endif
{
#ifdef MSPD_MLOG_NEW
   volatile int t = GetTIMETICK();
#endif
   TRC3(YsUiTfuSchTtiInd);
#if 0
   /* No need of assigning selector as this is decided with configuration */
   (*YsUiTfuSchTtiIndMt[pst->selector])(pst, suId, ttiInd);
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_SCH_TTI_IND, RESOURCE_LARM, t, GetTIMETICK());
#endif
#endif
   RETVALUE(ROK);

}
/**
* @brief This API is the PUCCH delta power indication from CL to scheduler.
*
* @details
*
*     Function : YsUiTfuPucchDeltaPwrInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuTtiIndInfo *  ttiInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 YsUiTfuPucchDeltaPwrInd
(
Pst * pst,
SuId suId,
TfuPucchDeltaPwrIndInfo *pucchDeltaPwrInd
)
#else
PUBLIC S16 YsUiTfuPucchDeltaPwrInd(pst, suId, pucchDeltaPwrInd)
Pst * pst;
SuId suId;
TfuPucchDeltaPwrIndInfo *pucchDeltaPwrInd;
#endif
{
/* ys005.102 : Introduced new variable */
   TRC3(YsUiTfuPucchDeltaPwrInd);
#if 0
   /* No need of assigning selector as this is decided with configuration */
   (*YsUiTfuPucchDeltaPwrIndMt[pst->selector])(pst, suId, pucchDeltaPwrInd);
/* ys005.102 : Calling MLogTask function */
#endif
   RETVALUE(ROK);

}
/**
* @brief This API is used to receive a Bind confirm from CL to RRM.
*
* @details
*
*     Function : YsUiCtfBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 YsUiCtfBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 YsUiCtfBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(YsUiCtfBndCfm);

   (*YsUiCtfBndCfmMt[pst->selector])(pst, suId, status);

   RETVALUE(ROK);

}

/**
* @brief This API is used to receive a Bind confirm from CL to RRM.
*
* @details
*
*     Function : YsUiCtfCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   CtfCfgTransId  transId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 YsUiCtfCfgCfm
(
Pst* pst,
SuId suId,
CtfCfgTransId transId,
U8 status
)
#else
PUBLIC S16 YsUiCtfCfgCfm(pst, suId, transId, status)
Pst* pst;
SuId suId;
CtfCfgTransId transId;
U8 status;
#endif
{

   TRC3(YsUiCtfCfgCfm);

   (*YsUiCtfCfgCfmMt[pst->selector])(pst, suId, transId, status);

   RETVALUE(ROK);

}
/***************SEC_CHANGE_START*****************/
#if 0
#ifdef CTF_AS_SEC_ENB
#ifdef ANSI
PUBLIC S16 ysUiCtfDatRsp
(
Pst      *pst,
SpId     spId,
CtfDatRspSdus *ctfDatRspSdus
)
#else
PUBLIC S16 ysUiCtfDatRsp(pst, spId, CtfDatRspSdu)
Pst      *pst;
SpId     spId;
CtfDatRspSdus *ctfDatRspSdus;
#endif
{
   TRC3(ysUiCtfDatRsp);

   RETVALUE((*YsUiCtfDatRspMt[pst->selector])(pst, spId, ctfDatRspSdus));

} /* end of function ysUiCtfDatRsp */
#endif
#endif
/***************SEC_CHANGE_END*****************/


#endif /*--ifdef YS--*/

#ifdef PTYSUITFU



/**
* @brief This API is used to receive a Bind confirm from CL to MAC.
*
* @details
*
*     Function : PtUiTfuBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiTfuBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PtUiTfuBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PtUiTfuBndCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiTfuSchBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PtUiTfuSchBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PtUiTfuSchBndCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   RETVALUE(ROK);

}


/**
* @brief This API is used to indication Random Access Request reception from
 * CL to MAC.
*
* @details
*
*     Function : PtUiTfuRaReqInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuRaReqIndInfo *  raReqInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiTfuRaReqInd
(
Pst * pst,
SuId suId,
TfuRaReqIndInfo * raReqInd
)
#else
PUBLIC S16 PtUiTfuRaReqInd(pst, suId, raReqInd)
Pst * pst;
SuId suId;
TfuRaReqIndInfo * raReqInd;
#endif
{

   TRC3(PtUiTfuRaReqInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(raReqInd);

   RETVALUE(ROK);

}



/**
* @brief This API is used to indicate CQI reporting from CL to MAC
*
* @details
*
*     Function : PtUiTfuUlCqiInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuUlCqiIndInfo *  ulCqiInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiTfuUlCqiInd
(
Pst * pst,
SuId suId,
TfuUlCqiIndInfo * ulCqiInd
)
#else
PUBLIC S16 PtUiTfuUlCqiInd(pst, suId, ulCqiInd)
Pst * pst;
SuId suId;
TfuUlCqiIndInfo * ulCqiInd;
#endif
{

   TRC3(PtUiTfuUlCqiInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(ulCqiInd);

   RETVALUE(ROK);

}



/**
* @brief This API is used to indicate HARQ feedback from CL to MAC
*
* @details
*
*     Function : PtUiTfuHqInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuHqIndInfo  *hqInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiTfuHqInd
(
Pst * pst,
SuId suId,
TfuHqIndInfo  *hqInd
)
#else
PUBLIC S16 PtUiTfuHqInd(pst, suId, hqInd)
Pst * pst;
SuId suId;
TfuHqIndInfo  *hqInd;
#endif
{

   TRC3(PtUiTfuHqInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(hqInd);

   RETVALUE(ROK);

}



/**
* @brief This API is used to indicate a SR reception from CL to MAC.
*
* @details
*
*     Function : PtUiTfuSrInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuSrIndInfo *  srInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiTfuSrInd
(
Pst * pst,
SuId suId,
TfuSrIndInfo * srInd
)
#else
PUBLIC S16 PtUiTfuSrInd(pst, suId, srInd)
Pst * pst;
SuId suId;
TfuSrIndInfo * srInd;
#endif
{

   TRC3(PtUiTfuSrInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(srInd);

   RETVALUE(ROK);

}



/**
* @brief This API is used to indicate the reception of CQI report from CL to
 * MAC.
*
* @details
*
*     Function : PtUiTfuDlCqiInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuDlCqiIndInfo *  dlCqiInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiTfuDlCqiInd
(
Pst * pst,
SuId suId,
TfuDlCqiIndInfo * dlCqiInd
)
#else
PUBLIC S16 PtUiTfuDlCqiInd(pst, suId, dlCqiInd)
Pst * pst;
SuId suId;
TfuDlCqiIndInfo * dlCqiInd;
#endif
{

   TRC3(PtUiTfuDlCqiInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(dlCqiInd);

   RETVALUE(ROK);

}



/**
* @brief This API is used to indicate Data Reception from CL to MAC.
*
* @details
*
*     Function : PtUiTfuDatInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuDatIndInfo *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiTfuDatInd
(
Pst * pst,
SuId suId,
TfuDatIndInfo * datInd
)
#else
PUBLIC S16 PtUiTfuDatInd(pst, suId, datInd)
Pst * pst;
SuId suId;
TfuDatIndInfo * datInd;
#endif
{

   TRC3(PtUiTfuDatInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(datInd);

   RETVALUE(ROK);

}



/**
* @brief This API is used to indicate Decode failure from CL to MAC.
*
* @details
*
*     Function : PtUiTfuCrcInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuCrcIndInfo *  crcInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiTfuCrcInd
(
Pst * pst,
SuId suId,
TfuCrcIndInfo * crcInd
)
#else
PUBLIC S16 PtUiTfuCrcInd(pst, suId, crcInd)
Pst * pst;
SuId suId;
TfuCrcIndInfo * crcInd;
#endif
{

   TRC3(PtUiTfuCrcInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(crcInd);

   RETVALUE(ROK);

}



/**
* @brief This API is used to indicate a Timing Advance from CL to MAC.
*
* @details
*
*     Function : PtUiTfuTimingAdvInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuTimingAdvIndInfo *  timingAdvInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiTfuTimingAdvInd
(
Pst * pst,
SuId suId,
TfuTimingAdvIndInfo * timingAdvInd
)
#else
PUBLIC S16 PtUiTfuTimingAdvInd(pst, suId, timingAdvInd)
Pst * pst;
SuId suId;
TfuTimingAdvIndInfo * timingAdvInd;
#endif
{

   TRC3(PtUiTfuTimingAdvInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(timingAdvInd);

   RETVALUE(ROK);

}



/**
* @brief This API is the TTI indication from CL to MAC.
*
* @details
*
*     Function : PtUiTfuTtiInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuTtiIndInfo *  ttiInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiTfuTtiInd
(
Pst * pst,
SuId suId,
TfuTtiIndInfo * ttiInd
)
#else
PUBLIC S16 PtUiTfuTtiInd(pst, suId, ttiInd)
Pst * pst;
SuId suId;
TfuTtiIndInfo * ttiInd;
#endif
{

   TRC3(PtUiTfuTtiInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(ttiInd);

   RETVALUE(ROK);

}

/**
* @brief This API is the TTI indication from CL to scheduler.
*
* @details
*
*     Function : PtUiTfuSchTtiInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuTtiIndInfo *  ttiInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiTfuSchTtiInd
(
Pst * pst,
SuId suId,
TfuTtiIndInfo * ttiInd
)
#else
PUBLIC S16 PtUiTfuSchTtiInd(pst, suId, ttiInd)
Pst * pst;
SuId suId;
TfuTtiIndInfo * ttiInd;
#endif
{

   TRC3(PtUiTfuSchTtiInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(ttiInd);

   RETVALUE(ROK);

}

/*
* @brief This API is the Pucch DELTA power indication from CL to scheduler.
*
* @details
*
*     Function :PtUiTfuPucchDeltaPwrInd
*
*  @param[in]   Pst *  pst
*  @param[in]   SuId  suId
*  @param[in]   TfuPucchDeltaPwrIndInfo *pucchDeltaPwrInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiTfuPucchDeltaPwrInd
(
Pst * pst,
SuId suId,
TfuPucchDeltaPwrIndInfo *pucchDeltaPwrInd
)
#else
PUBLIC S16 PtUiTfuSchTtiInd(pst, suId, pucchDeltaPwrInd)
Pst * pst;
SuId suId;
TfuPucchDeltaPwrIndInfo *pucchDeltaPwrInd;
#endif
{

   TRC3(PtUiTfuPucchDeltaPwrInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(pucchDeltaPwrInd);

   RETVALUE(ROK);

}
#endif /*--ifdef PTYSUITFU--*/

#ifdef PTYSUICTF


/**
* @brief This API is used to receive a Bind confirm from CL to MAC.
*
* @details
*
*     Function : PtUiCtfBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiCtfBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PtUiCtfBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PtUiCtfBndCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   RETVALUE(ROK);

}

/**
* @brief This API is used to receive a Bind confirm from CL to MAC.
*
* @details
*
*     Function : PtUiCtfCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiCtfCfgCfm
(
Pst* pst,
SuId suId,
CtfCfgTransId  transId,
U8 status
)
#else
PUBLIC S16 PtUiCtfCfgCfm(pst, suId, transId, status)
Pst* pst;
SuId suId;
CtfCfgTransId  transId;
U8 status;
#endif
{

   TRC3(PtUiCtfCfgCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transId);
   UNUSED(status);

   RETVALUE(ROK);

}

/***************SEC_CHANGE_START*****************/
#if 0
#ifdef CTF_AS_SEC_ENB
#ifdef ANSI
PUBLIC S16 PtUiCtfDatRsp
(
Pst* pst,
SpId spId,
CtfDatRspSdus *DatRspSdu
)
#else
PUBLIC S16 PtUiCtfDatRsp(pst, spId, DatRspSdu)
Pst* pst;
SpId spId;
CtfDatRspSdus *DatRspSdu;
#endif
{

   TRC3(PtUiCtfDatRsp);

  UNUSED(pst);
  UNUSED(spId);
  UNUSED(DatRspSdu);


   RETVALUE(ROK);

}
#endif
#endif
/***************SEC_CHANGE_END*****************/

#endif /* PTYSUICTF */

/********************************************************************30**
  
         End of file:     yw_ptui.c@@/main/2 - Thu Dec  2 02:27:26 2010
  
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
----------- -------- ---- -----------------------------------------------
/main/2      ---     sgm                   1. eNodeB 1.2 release
/main/2      ys004.102     ms              1. g++ compilation.
*********************************************************************91*/
