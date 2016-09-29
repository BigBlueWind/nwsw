/********************************************************************16**

                         (c) COPYRIGHT 2011 by 
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

                    Email: support@ccpu.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_lim.c 
  
     Sid:      gk_lim.c@@/main/3 - Sat Jul 30 02:21:30 2011
  
     Prg:     sd 
  
**********************************************************************/

/** @file rg_lim.c.
@brief It has APIs exposed by Lower Interface Modulue of MAC. It acts as an 
Interface handler for lower interface APIs.
*/
/* header include files -- defines (.h) */
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
#include "cm_lte.h"       /* common tokens */
#include "rgu.h"           /* RGU defines */
#include "tfu.h"           /* RGU defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "crg.h"           /* layer management defines for LTE-MAC */
#include "rg_sch_inf.h"           /* layer management defines for LTE-MAC */
#include "rg_env.h"        /* customisable defines and macros for MAC */
#include "rg.h"            /* defines and macros for MAC */


/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"       /* common tokens */
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"           /* layer management typedefs for MAC */
#include "rg.x"            /* typedefs for MAC */

/* local defines */

/* local typedefs */
 
/* local externs */
PRIVATE S16  rgLIMValidateSap ARGS((SuId suId));
PRIVATE Void rgLIMUtlFreeDatIndEvnt ARGS((TfuDatIndInfo *datInd,
                                          Bool error));
#ifdef RG_UNUSED
PRIVATE Void rgLIMUtlFreeDatReqEvnt ARGS((TfuDatReqInfo *datReq,
                                          Bool error));
#endif
/* forward references */

/**
 * @brief This API is invoked to send TFU SAP bind request to PHY.
 *
 * @details
 *
 *     Function : rgLIMTfuBndReq
 *      
 *      This API is invoked to send TFU SAP bind request to PHY. It fills in 
 *      the Pst structure, spId and suId values and invokes bind request
 *      primitive at TFU.
 *           
 *  @param[in]  SuId            suId 
 *  @param[in]  SpId            spId
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgLIMTfuBndReq
(
SuId    suId, 
SpId    spId
)
#else
PUBLIC S16 rgLIMTfuBndReq(suId, spId)
SuId    suId; 
SpId    spId;
#endif
{
   S16         ret;
   RgLowSapCb  *tfuSap;
   Pst         pst;

   TRC2(rgLIMTfuBndReq);

   /* Get the lower SAP control block from the layer control block. */
   tfuSap = &(rgCb.tfuSap);
   (Void)cmMemcpy ((U8*)&pst, (U8*)&(tfuSap->sapCfg.sapPst), sizeof(Pst));
   if((ret = RgLiTfuBndReq (&pst, suId, spId)) != ROK)
   {
      RGDBGERR((rgPBuf,"rgLIMTfuBndReq() Call to RgLiTfuBndReq() failed\n"));
   }
   RETVALUE(ret);
}  /* rgLIMTfuBndReq */


/**
 * @brief This API is invoked to send TFU SAP unbind request to PHY.
 *
 * @details
 *
 *     Function : rgLIMTfuBndReq
 *      
 *      This API is invoked to send TFU SAP unbind request to PHY. It fills in 
 *      the Pst structure and spId value and invokes unbind request
 *      primitive at TFU.
 *           
 *  @param[in]  SpId            spId
 *  @param[in]  Reason          reason 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgLIMTfuUbndReq
(
SpId    spId, 
Reason  reason
)
#else
PUBLIC S16 rgLIMTfuUbndReq(spId, reason)
SpId    spId; 
Reason  reason;
#endif
{
   S16         ret;
   RgLowSapCb  *tfuSap;
   Pst         pst;

   TRC2(rgLIMTfuUbndReq);

   /* Get the lower SAP control block from the layer control block. */
   tfuSap = &(rgCb.tfuSap);
   cmMemcpy ((U8*)&pst, (U8*)&(tfuSap->sapCfg.sapPst), sizeof(Pst));
   if((ret = RgLiTfuUbndReq (&pst, tfuSap->sapCfg.spId, reason)) != ROK)
   {
      RGDBGERR((rgPBuf,"rgLIMTfuUbndReq() Call to RgLiTfuUbndReq() failed\n"));
   }
   RETVALUE(ret);

}  /* rgLIMTfuUbndReq */


/**
 * @brief Bind confirm API for TFU SAP 
 *
 * @details
 *
 *     Function : RgLiTfuBndCfm
 *      
 *      This API is invoked by PHY to confirm TFU SAP bind. 
 *     
 *           
 *  @param[in]  Pst   *pst 
 *  @param[in]  SuId  suId 
 *  @param[in]  U8    status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuBndCfm 
(
Pst     *pst,
SuId    suId, 
U8      status
)
#else
PUBLIC S16 RgLiTfuBndCfm(pst, suId, status)
Pst     *pst; 
SuId    suId; 
U8      status;
#endif
{
   S16 ret;
   RgLowSapCb  *tfuSap;

   TRC3(RgLiTfuBndCfm);

   RGDBGPRM((rgPBuf,"RgLiTfuBndCfm SuId(%d), Status(%d)\n", 
         suId, status));
   /* Lets validate suId first */
   tfuSap = &(rgCb.tfuSap);

   if (suId != tfuSap->sapCfg.suId)
   {
      RGDBGERR((rgPBuf,"Incorrect SuId. Configured (%d) Recieved (%d)\n",
            tfuSap->sapCfg.suId, suId));
      RETVALUE(RFAILED);
   }
   ret = rgLMMBndCfm (pst, suId, status);
   RETVALUE(ret);
}  /* RgLiTfuBndCfm */

 /** @brief This function Validates the SAP information received along with the
  * primitive from the lower layer. 
  * Function:
  *   Validates SAP information.
  * @param  suId The SAP Id
  * @return 
  *   -# ROK
  *   -# RFAILED
  */
#ifdef ANSI
PRIVATE S16 rgLIMValidateSap
(
 SuId    suId
)
#else
PRIVATE S16 rgLIMValidateSap(suId)
 SuId    suId;
#endif
{
   RgLowSapCb  *tfuSap;

   TRC2(rgLIMValidateSap)

   tfuSap = &(rgCb.tfuSap);

   /* First lets check the suId */
   if( suId != tfuSap->sapCfg.suId)
   {
      RGDBGERR((rgPBuf,"Incorrect SuId. Configured (%d) Recieved (%d)\n",
            tfuSap->sapCfg.suId, suId));
      RETVALUE(RFAILED);
   }
   if (tfuSap->sapSta.sapState != LRG_BND)
   {
      RGDBGERR((rgPBuf,"Lower SAP not enabled SuId (%d)\n",
            tfuSap->sapCfg.suId));
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of rgLIMValidateSap */

#if 0
/** @brief This function frees up the TfuDatIndInfo structure
 *
 * @details
 *
 *     Function: rgLIMUtlFreeDatIndEvnt 
 *       - Function frees up the TfuDatIndInfo structure, in case of error it shall
 *       free up the buffer's present in the datIndLst.
 *
 *         Processing steps:
 * @param  [in] TfuDatIndInfo *datInd
 * @param  [in] Bool          *error
 * @return 
 */
#ifdef ANSI
PRIVATE Void rgLIMUtlFreeDatIndEvnt 
(
 TfuDatIndInfo *datInd,
 Bool          error
 )
#else
PRIVATE Void rgLIMUtlFreeDatIndEvnt(datInd, error)
 TfuDatIndInfo *datInd;
 Bool          error;
#endif
{

   TfuDatInfo     *datInfo;
   CmLList        *node;

   TRC2(rgLIMUtlFreeDatIndEvnt);
   /* Steps of freeing up the TfuDatInd.
    * 1. loop through the datIndLst and free up all the buffers.
    * 2. free up the whole event
    */
   if ((error == TRUE) && (datInd->datIndLst.count > 0))
   {
      node =  datInd->datIndLst.first;
      while (node)
      {
         datInfo = (TfuDatInfo*)node->node;
         if (datInfo->mBuf != NULLP)
         {
            SPutMsg(datInfo->mBuf);
         }
         node = node->next;
      }
   }
   cmFreeMem((Ptr)datInd);
   RETVOID;
} /* end of rgLIMUtlFreeDatIndEvnt*/

/**
 * @brief Downlink data indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuDatInd
 *      
 *      This API is invoked by PHY to send data indication to MAC on 
 *      recieving data from UEs.
 *           
 *  @param[in]  Pst              *pst
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuDatIndInfo    *datInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuDatInd
(
Pst                *pst, 
SuId               suId, 
TfuDatIndInfo    *datInd
)
#else
PUBLIC S16 RgLiTfuDatInd(pst, suId, datInd)
Pst                *pst; 
SuId               suId; 
TfuDatIndInfo    *datInd;
#endif
{
   S16         ret;

   TRC3(RgLiTfuDatInd);

   /* rg005.201 Removing the validation - may not be required for
      data performance path */
#ifndef NO_ERRCLS 
   if ((ret = rgLIMValidateSap (suId)) != ROK)
   {
      RGDBGERR((rgPBuf,"RgLiTfuDatInd() SAP Validation failed.\n"));
      rgLIMUtlFreeDatIndEvnt(datInd, TRUE);
      RETVALUE(ret);
   }
#endif
   /* Now call the TOM (Tfu ownership module) primitive to process further */
   rgCb.tfuSap.sapSts.numPduRcvd += datInd->datIndLst.count;
   ret = rgTOMDatInd(datInd);
   /* Free up the memory for the request structure */
   rgLIMUtlFreeDatIndEvnt(datInd, FALSE);
   RETVALUE(ret);
}  /* RgLiTfuDatInd*/

#ifdef RG_UNUSED
/** @brief This function frees up the TfuDatReqInfo structure.
 *
 * @details
 *
 *     Function: rgLIMUtlFreeDatReqEvnt
 *       - Function frees up the TfuDatReqInfo structure, in case of error it shall
 *       free up the buffer's present in the PDUs list.
 *
 *         Processing steps:
 * @param  [in] TfuDatReqInfo *datReq
 * @param  [in] Bool          *error
 * @return 
 */
#ifdef ANSI
PRIVATE Void rgLIMUtlFreeDatReqEvnt
(
 TfuDatReqInfo *datReq,
 Bool          error
 )
#else
PRIVATE Void rgLIMUtlFreeDatReqEvnt(datReq, error)
 TfuDatReqInfo *datReq;
 Bool          error;
#endif
{

   TfuDatReqPduInfo *datInfo;
   CmLList          *node;
   U8               i;

   TRC2(rgLIMUtlFreeDatReqEvnt);
   /* Steps of freeing up the TfuDatReq.
    * 1. Free the bch buffer.
    * 2. loop through the pdus list and free up all the buffers.
    * 3. free up the whole event
    */
   if (error)
   {
      if (datReq->bchDat.pres == PRSNT_NODEF)
      {
         SPutMsg(datReq->bchDat.val);
      }
      if (datReq->pdus.count > 0)
      {
         node =  datReq->pdus.first;
         while (node)
         {
            datInfo = (TfuDatReqPduInfo*)node->node;
            for (i=0; i<datInfo->nmbOfTBs; i++)
            {
               if (datInfo->mBuf[i] != NULLP)
               {
                  SPutMsg(datInfo->mBuf[i]);
               }
            }
            node = node->next;
         }
      }
   }
   cmFreeMem((Ptr)datReq);
   RETVOID;
} /* end of rgLIMUtlFreeDatReqEvnt*/
#endif
/**
 * @brief This API is invoked to send Data to PHY.
 *
 * @details
 *
 *     Function : rgLIMTfuDatReq
 *      
 *      This API is invoked to send Data to PHY. It 
 *      fills in the Pst structure, spId value and invokes Data
 *      request primitive at TFU.
 *           
 *  @param[in]  TfuDatReqInfo *datReq
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgLIMTfuDatReq 
(
TfuDatReqInfo *datReq
)
#else
PUBLIC S16 rgLIMTfuDatReq(datReq)
TfuDatReqInfo *datReq;
#endif
{
   S16         ret;
   RgLowSapCb  *tfuSap;

   TRC2(rgLIMTfuDatReq)

   /* Get the lower SAP control block from the layer control block. */
   tfuSap = &(rgCb.tfuSap);

   /* rg005.201 Removing the validation - may not be required for
      data performance path */
#ifndef NO_ERRCLS
   if (tfuSap->sapSta.sapState != LRG_BND)
   {
      RGDBGERR((rgPBuf,"rgLIMTfuDatReq() Lower SAP not bound (%d) \n",
            tfuSap->sapSta.sapState));
#ifdef RG_UNUSED
      /* This case will never be hit if sap is not bound then we dont get TTI */
      rgLIMUtlFreeDatReqEvnt(datReq, TRUE);
#endif
      RETVALUE(RFAILED);
   }
#endif

   /* rg003.301:ccpu00122591-ADD-Updating the numPduTxmit 
    * statistics variable.
    * */
   tfuSap->sapSts.numPduTxmit += datReq->pdus.count; 

   /* Using existing pst - for optimization */
   if((ret = RgLiTfuDatReq (&tfuSap->sapCfg.sapPst, tfuSap->sapCfg.spId, 
                            datReq)) != ROK)
   {
      RGDBGERR((rgPBuf,"rgLIMTfuDatReq() Call to RgLiTfuDatReq() failed\n"));
   }
   RETVALUE(ret);
}  /* rgLIMTfuDatReq*/

/**
 * @brief Transmission time interval indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuTtiInd 
 *      
 *      This API is invoked by PHY to indicate TTI indication to MAC for a cell.
 *           
 *  @param[in]  Pst            *pst
 *  @param[in]  SuId           suId 
 *  @param[in]  TfuTtiIndInfo  *ttiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuTtiInd 
(
Pst                 *pst, 
SuId                suId, 
TfuTtiIndInfo       *ttiInd
)
#else
PUBLIC S16 RgLiTfuTtiInd(pst, suId, ttiInd)
Pst                 *pst; 
SuId                suId; 
TfuTtiIndInfo       *ttiInd;
#endif
{
   S16   ret;
#ifdef MSPD
   volatile int t=GetTIMETICK();
#endif
   
   /*rg003.201:Changed from TRC2 to TRC3*/
   TRC3(RgLiTfuTtiInd);

/* rg005.201 Removing the validation - may not be required for
      data performance path */
/* Combine compilation fixes */
   
#ifdef NO_ERRCLS
   if ((ret = rgLIMValidateSap (suId)) != ROK)
   {
      RGDBGERR((rgPBuf,"RgLiTfuTtiInd() SAP Validation failed.\n"));
      SPutSBuf (pst->region, pst->pool, (Data *)ttiInd,
            sizeof(TfuTtiIndInfo));
      RETVALUE(ret);
   }
#endif
   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = rgTOMTtiInd(ttiInd);
   /* Free up the memory for the request structure */
   SPutSBuf (pst->region, pst->pool, (Data *)ttiInd,
         sizeof(TfuTtiIndInfo));
 #ifdef MSPD_MLOG_NEW
   MLogTask(PID_MAC_TTI_IND, RESOURCE_LARM, t, GetTIMETICK());
 #endif
   RETVALUE(ret);
}  /* RgLiTfuTtiInd */

#endif
/**********************************************************************
 
         End of file:     gk_lim.c@@/main/3 - Sat Jul 30 02:21:30 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     sd   1. Initial Release.
/main/2      ---     sd   1. LTE MAC 2.1 release
           rg003.201 ns   1. Trace added for functions.
           rg005.201 sm   1. Removed checks in the data path for optimization.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
*********************************************************************91*/
