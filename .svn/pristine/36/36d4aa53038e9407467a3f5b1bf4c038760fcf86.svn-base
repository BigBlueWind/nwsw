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
  
        Name:    LTE-RLC Layer - Upper Interface
    
        Type:    C file
  
        Desc:    C source code for the upper interface of LTE-RLC
 
        File:    kw_ptui.c

        Sid:      gp_ptui.c@@/main/2 - Fri Nov 13 14:14:00 2009
  
        Prg:     gk
  
*********************************************************************21*/

/** @file gp_ptui.c
@brief RLC Upper Interface
*/

/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000371     LTE-RLC
 */


/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#ifdef KW_PDCP
#include "cpj.h"           /* CPJ defines */
#include "pju.h"           /* PJU defines */
#include "lpj.h"           /* LPJ defines */
#endif

#include "kw_err.h"
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */
#ifdef KW_PDCP
#include "cpj.x"           /* CPJ defines */
#include "pju.x"           /* PJU defines */
#include "lpj.x"           /* LPJ defines */
#endif

#include "kw.x"

#ifndef LCKWUIKWU
#define PTKWKWU
#endif

#ifndef LCKWUICKW
#define PTKWCKW
#endif

#ifndef NH
#define PTKWKWU
#define PTKWCKW
#endif

#ifndef PJ 
#define PTKWKWU
#define PTKWCKW
#endif

#if KW_PDCP
#if !(defined(LCPJUICPJ)  && defined(NH))
#define PTPJCPJ
#endif

#if !(defined(LCPJUIPJU) && (defined(PX))) || !(defined(NH))
#define PTPJPJU
#endif
#endif /* KW_PDCP */

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

  
/*********************************************************************
 *             Forward Declartion for KWU Porting Functions
 ********************************************************************/
#ifdef PTKWKWU
PRIVATE S16 PtUiKwuBndCfm  ARGS ((
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U8 status                       /* status */
));

#endif /* PTKWKWU */

PUBLIC S16 PtUiKwuDatCfm ARGS ((
Pst               *pst,
SuId              suId,
KwuDatCfmInfo         *datCfm
));

PUBLIC S16 PtUiKwuStaInd ARGS ((
Pst               *pst,
SuId              suId,
KwuStaIndInfo     *staInd
));

PUBLIC S16 PtUiKwuReEstCmpInd ARGS ((
Pst               *pst,
SuId              suId,
CmLteRlcId        rlcId
));
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
PUBLIC S16 PtUiKwuDiscSduCfm ARGS((
Pst               *pst,
SuId              suId,
KwuDiscSduInfo    *discCfm
));
PUBLIC S16 PtUiKwuDatAckInd ARGS((
Pst               *pst,
SuId              suId,
KwuDatAckInfo     *datAckInd
));
#endif
#if (defined(PTKWKWU) || defined(KW_PDCP))
PUBLIC S16 PtUiKwuDatInd ARGS ((
Pst               *pst,
SuId              suId,
KwuDatIndInfo     *datInd,
Buffer            *mBuf
));
#endif /* PTKWKWU || KW_PDCP */


  
/*********************************************************************
 *             Forward Declartion for CKW Porting Functions
 ********************************************************************/
#ifdef PTKWCKW
PRIVATE S16 PtUiCkwBndCfm  ARGS ((
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U8 status                       /* status */
));

PUBLIC S16 PtUiCkwCfgCfm ARGS ((
Pst            *pst,                      /* post structure */
SuId           suId,                      /* Service User Id */
CkwCfgCfmInfo  *cfmInfo                    /* Configuration Confirm */
));

PUBLIC S16 PtUiCkwUeIdChgCfm ARGS((
Pst            *pst,                      /* post structure */
SuId           suId,                      /* Service User Id */
U32            transId,
CkwUeInfo      *ueInfo,
CmStatus       status
));

#endif /* PTKWCKW */

#ifdef KW_PDCP
  
/*********************************************************************
 *             Forward Declartion for CPJ Porting Functions
 ********************************************************************/
#ifdef PTPJCPJ

EXTERN S16 PtUiCpjBndCfm ARGS((Pst * pst, SuId suId, U8 status));

EXTERN S16 PtUiCpjCfgCfm ARGS((Pst * pst, SuId suId, CpjCfgCfmInfo * cfgCfm));

EXTERN S16 PtUiCpjReEstCfm ARGS((Pst * pst, SuId suId, CpjReEstCfmInfo * reEstCfm));

EXTERN S16 PtUiCpjSecCfgCfm ARGS((Pst * pst, SuId suId, CpjSecCfgCfmInfo * secCfgCfm));

EXTERN S16 PtUiCpjUeIdChgCfm ARGS((Pst * pst, SuId suId, U32 transId, CpjUeInfo  * ueInfo, CmStatus status));

EXTERN S16 PtUiCpjCountCfm ARGS((Pst * pst, SuId suId, CpjCountCfmInfo * countCfm));

EXTERN S16 PtUiCpjSduStaCfm ARGS((Pst * pst, SuId suId, CpjSduStaCfmInfo * staCfm));
#endif /* PTPJCPJ */

  
/*********************************************************************
 *             Forward Declartion for PJU Porting Functions
 ********************************************************************/
#ifdef PTPJPJU
/* kw006.201 ccpu00120534, fixed the problem of passing suId and spId as argument */
EXTERN S16 PtUiPjuBndReq ARGS((Pst* pst, SuId suId, SpId spId));

EXTERN S16 PtUiPjuBndCfm ARGS((Pst* pst, SuId suId, U8 status));

EXTERN S16 PtUiPjuDatCfm ARGS((Pst* pst, SuId suId, CmLtePdcpId * pdcpId, PjuDatCfmInfo * datCfm));

EXTERN S16 PtUiPjuDatInd ARGS((Pst* pst, SuId suId, CmLtePdcpId * pdcpId, Buffer * mBuf));

EXTERN S16 PtUiPjuStaInd ARGS((Pst* pst, SuId suId, CmLtePdcpId * pdcpId, PjuStaIndInfo * staInd, Buffer *mBuf));

EXTERN S16 PtUiPjuDatFwdInd ARGS((Pst* pst, SuId suId, CmLtePdcpId * pdcpId, PjuDatFwdIndInfo * datFwdInd));
#endif /*--#ifdef PTPJPJU--*/
#endif /* KW_PDCP */

  
/*********************************************************************
 *             Primitives for KWU interface 
 ********************************************************************/

/* KWU Bind Confirm primitive */

PUBLIC KwuBndCfm kwUiKwuBndCfmMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuBndCfm,            /* 0 - loosely coupled */
#else
   PtUiKwuBndCfm,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef NH
   NhLiKwuBndCfm,            /* 1 - tightly coupled, RRC  */
#else
   PtUiKwuBndCfm,            /* 1 - tightly coupled, portable */
#endif /* NH */
#ifndef KW_PDCP
#ifdef PJ
   PjLiKwuBndCfm,            /* 2 - tightly coupled, PDCP */
#else
   PtUiKwuBndCfm,            /* 2 - tightly coupled, portable */
#endif /* PJ */
#endif /* KW_PDCP */
};

/* KWU Data Indication primitive */

PUBLIC KwuDatInd kwUiKwuDatIndMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuDatInd,            /* 0 - loosely coupled */
#else
   PtUiKwuDatInd,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
#ifdef NH
   NhLiKwuDatInd,            /* 1 - tightly coupled, RRC  */
#else
   PtUiKwuDatInd,            /* 1 - tightly coupled, portable */
#endif /* NH */
#ifdef KW_PDCP
   PtUiKwuDatInd,            /* 2 - tightly coupled, portable */
#else
#ifdef PJ
   PjLiKwuDatInd,            /* 2 - tightly coupled, PDCP */
#else
   PtUiKwuDatInd,            /* 2 - tightly coupled, portable */
#endif /* PJ */
#endif /* KW_PDCP */
};

#ifndef KW_PDCP
PUBLIC KwuDatCfm kwUiKwuDatCfmMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuDatCfm,            /* 0 - loosely coupled */
#else
   PtUiKwuDatCfm,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
   PtUiKwuDatCfm,            /* 1 - tightly coupled, portable */
#ifdef PJ
   PjLiKwuDatCfm,            /* 2 - tightly coupled, PDCP */
#else
   PtUiKwuDatCfm,            /* 2 - tightly coupled, portable */
#endif /* PJ */
};

/* KWU AM Status Indication primitive */

PUBLIC KwuStaInd kwUiKwuStaIndMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuStaInd,            /* 0 - loosely coupled */
#else
   PtUiKwuStaInd,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
   PtUiKwuStaInd,            /* 2 - tightly coupled, portable */
#ifdef PJ
   PjLiKwuStaInd,            /* 2 - tightly coupled, PDCP */
#else
   PtUiKwuStaInd,            /* 2 - tightly coupled, portable */
#endif /* PJ */
};

PUBLIC KwuReEstCmpInd kwUiKwuReEstCmpIndMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuReEstCmpInd,            /* 0 - loosely coupled */
#else
   PtUiKwuReEstCmpInd,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
   PtUiKwuReEstCmpInd,            /* 1 - loosely coupled, portable  */
#ifdef PJ
   PjLiKwuReEstCmpInd,            /* 2 - tightly coupled, PDCP */
#else
   PtUiKwuReEstCmpInd,            /* 2 - tightly coupled, portable */
#endif /* PJ */
};

/* kw005.201 added support for L2 measurement */
#ifdef LTE_L2_MEAS
PUBLIC KwuDiscSduCfm kwUiKwuDiscSduCfmMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuDiscSduCfm,            /* 0 - loosely coupled */
#else
   PtUiKwuDiscSduCfm,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
   PtUiKwuDiscSduCfm,            /* 1 - loosely coupled portable */
#ifdef PJ
   PjLiKwuDiscSduCfm,            /* 2 - tightly coupled, PDCP */
#else
   PtUiKwuDiscSduCfm,            /* 2 - tightly coupled, portable */
#endif /* PJ */
};
PUBLIC KwuDatAckInd kwUiKwuDatAckIndMt[] =
{
#ifdef LCKWUIKWU
   cmPkKwuDatAckInd,            /* 0 - loosely coupled */
#else
   PtUiKwuDatAckInd,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUIKWU */
   PtUiKwuDatAckInd,            /* 1 - loosely coupled, portable  */
#ifdef PJ
   PjLiKwuDatAckInd,            /* 2 - tightly coupled, PDCP */
#else
   PtUiKwuDatAckInd,            /* 2 - tightly coupled, portable */
#endif /* PJ */
};
#endif /* LTE_L2_MEAS */
/* KWU AM Data confirm primitive */

#endif /* KW_PDCP */

  
/****************************************************************************
 *                         KWU Interface Mt functions
 ***************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for confirming  the bind request received from KWU
 *        interface.
 *
 * @b Description:
 *
 *        This function send the bind confirm primitive to the RLC user,
 *        when it receives a bind request from its service user.
 *
 *  @param[in] pst     Post structure  
 *  @param[in] suId    Service user SAP ID 
 *  @param[in] status  Status of Confirmation 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuBndCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U8   status                     /* Status */
)
#else
PUBLIC S16 KwUiKwuBndCfm(pst, suId, status)
Pst *pst;                       /* post structure */
SuId suId;                      /* Service User Id */
U8   status;                    /* Status */
#endif
{
   TRC3(KwUiKwuBndCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuBndCfmMt[pst->selector])(pst, suId, status);

   RETVALUE(ROK);

} /* end of KwUiKwuBndCfm */


/**
 *
 * @brief 
 *
 *        Handler for sending the data(SDU) from upper layer. 
 *
 * @b Description:
 *
 *        This function is used to transfer a SDU received from the peer
 *        RLC entity to the service user(RRC/PDCP).
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] datIndInfo  Data Request Information
 *  @param[in] mBuf         Data Buffer (SDU) 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuDatInd
(
Pst               *pst,
SuId              suId,
KwuDatIndInfo     *datInd,
Buffer            *mBuf
)
#else
PUBLIC S16 KwUiKwuDatInd(pst, suId, datInd, mBuf)
Pst               *pst;
SuId              suId;
KwuDatIndInfo     *datInd;
Buffer            *mBuf;
#endif
{
   TRC3(KwUiKwuDatInd)

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuDatIndMt[pst->selector])(pst, suId, datInd, mBuf);

   RETVALUE(ROK);

} /* end of KwUiKwuDatInd */


#ifndef KW_PDCP
/**
 *
 * @brief 
 *
 *        Handler for sending the data confirmation to upper layer. 
 *
 * @b Description:
 *
 *        This function is used to send a confirmation to the service
 *        user about the data received by the peer RLC entity. 
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] datCfmInfo  Data Confirmation Information
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuDatCfm
(
Pst               *pst,
SuId              suId,
KwuDatCfmInfo     *datCfm
)
#else
PUBLIC S16 KwUiKwuDatCfm(pst, suId, datCfm)
Pst               *pst;
SuId              suId;
KwuDatCfmInfo     *datCfm;
#endif
{
   TRC3(KwUiKwuDatCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuDatCfmMt[pst->selector])(pst, suId, datCfm);

   RETVALUE(ROK);

} /* end of KwUiKwuDatCfm */


/**
 *
 * @brief 
 *
 *        Handler for sending the Status Indication to the upper layer. 
 *
 * @b Description:
 *
 *        This function is used only by RLC AM entity.It send status 
 *        indication to the upper layer about the maximum number of 
 *        re-transmissions reached for a RLC AM entity. 
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] staInd      RLC Entity Id  
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuStaInd
(
Pst               *pst,
SuId              suId,
KwuStaIndInfo     *staInd
)
#else
PUBLIC S16 KwUiKwuStaInd(pst, suId, staInd)
Pst               *pst;
SuId              suId;
KwuStaIndInfo     *staInd;
#endif
{
   TRC3(KwUiKwuStaInd)

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuStaIndMt[pst->selector])(pst, suId, staInd);

   RETVALUE(ROK);

} /* end of KwUiKwuStaInd */


/**
 *
 * @brief 
 *
 *        Handler for sending the Status Indication to the upper layer. 
 *
 * @b Description:
 *
 *        This function is used only by RLC AM entity.It send status 
 *        indication to the upper layer about the maximum number of 
 *        re-transmissions reached for a RLC AM entity. 
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] staInd      RLC Entity Id  
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuReEstCmpInd
(
Pst               *pst,
SuId              suId,
CmLteRlcId        rlcId
)
#else
PUBLIC S16 KwUiKwuReEstCmpInd(pst, suId, rlcId)
Pst               *pst;
SuId              suId;
CmLteRlcId        rlcId;
#endif
{
   TRC3(KwUiKwuReEstCmpInd)

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuReEstCmpIndMt[pst->selector])(pst, suId, rlcId);

   RETVALUE(ROK);

} /* end of KwUiKwuReEstCmpInd */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS

/**
 *
 * @brief 
 *
 *        Handler for sending the Sdu Disc Cfm  to the upper layer. 
 *
 * @b Description:
 *       
 *       This function confirms the discard of an SDU .
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] discCfm     Disc information.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuDiscSduCfm
(
Pst               *pst,
SuId              suId,
KwuDiscSduInfo    *discCfm
)
#else
PUBLIC S16 KwUiKwuDiscSduCfm(pst, suId, discCfm)
Pst               *pst;
SuId              suId;
KwuDiscSduInfo    *discCfm;
#endif
{
   TRC3(KwUiKwuDiscSduCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuDiscSduCfmMt[pst->selector])(pst, suId, discCfm);

   RETVALUE(ROK);

} /* end of KwUiKwuDiscSduCfm */ 


/**
 *
 * @brief 
 *
 *        Handler for sending the Data ack indication  to the upper layer. 
 *
 * @b Description:
 *       
 *       This function confirms the succesfull transmission of SDU
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] datAckInd   DatAckInd
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuDatAckInd
(
Pst               *pst,
SuId              suId,
KwuDatAckInfo     *datAckInd
)
#else
PUBLIC S16 KwUiKwuDatAckInd(pst, suId, datAckInd)
Pst               *pst;
SuId              suId;
KwuDatAckInfo     *datAckInd;
#endif
{
   TRC3(KwUiKwuDatAckInd)

   /* jump to specific primitive depending on configured selector */
   (*kwUiKwuDatAckIndMt[pst->selector])(pst, suId, datAckInd);

   RETVALUE(ROK);

} /* end of KwUiKwuDatAckInd */ 
#endif /* LTE_L2_MEAS */
#endif /* KW_PDCP */


#ifdef PTKWKWU
/*************************************************************************
 *                         KWU Porting Functions
 ************************************************************************/
/**
 *
 * @brief 
 *
 *        PtUiKwuBndCfm - Portable SAP bind confirm
 *
 *  @param[in] pst      - Post structure  
 *  @param[in] suId     - Service user SAP ID 
 *  @param[in] status   - Status
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PRIVATE S16 PtUiKwuBndCfm 
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U8 status                       /* status */
)
#else
PRIVATE S16 PtUiKwuBndCfm(pst, suId, status)
Pst *pst;                       /* post structure */
SuId suId;                      /* Service User Id */
U8 status;                      /* status */
#endif
{
   TRC3(PtUiKwuBndCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

#if (ERRCLASS & ERRCLS_DEBUG)
   KWLOGERROR(ERRCLS_DEBUG, EKW192, ERRZERO, 
      "Improper selector for NhLiKwuBndCfm\n");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);
} /* end of PtUiKwuBndCfm */
#endif /* PTKWKWU */


/**
 *
 * @brief 
 *
 *        PtUiKwuDatCfm - Portable common channel data request
 *
 *  @param[in] pst    -  Post structure  
 *  @param[in] suId   -  Service user SAP ID 
 *  @param[in] datCfm -  Data Request
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PtUiKwuDatCfm
(
Pst               *pst,
SuId              suId,
KwuDatCfmInfo    *datCfm
)
#else
PUBLIC S16 PtUiKwuDatCfm(pst, suId, datCfm)
Pst               *pst;
SuId              suId;
KwuDatCfmInfo     *datCfm;
#endif
{
   TRC3(PtUiKwuDatCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(datCfm);

#if (ERRCLASS & ERRCLS_DEBUG)
   KWLOGERROR(ERRCLS_DEBUG, EKW193, ERRZERO, 
      "Improper selector for NhLiKwuDatCfm\n");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);

} /* end of PtUiKwuDatCfm */


/**
 *
 * @brief 
 *
 *        PtUiKwuStaInd - Portable common channel data request
 *
 *  @param[in] pst    -  Post structure  
 *  @param[in] suId   -  Service user SAP ID 
 *  @param[in] staInd -  Data Request
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PtUiKwuStaInd
(
Pst               *pst,
SuId              suId,
KwuStaIndInfo     *staInd
)
#else
PUBLIC S16 PtUiKwuStaInd(pst, suId, staInd)
Pst               *pst;
SuId              suId;
KwuStaIndInfo     *staInd;
#endif
{
   TRC3(PtUiKwuStaInd)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(staInd);

#if (ERRCLASS & ERRCLS_DEBUG)
   KWLOGERROR(ERRCLS_DEBUG, EKW194, ERRZERO, 
      "Improper selector for NhLiKwuStaInd\n");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);

} /* end of PtUiKwuStaInd */


/**
 *
 * @brief 
 *
 *        PtUiKwuReEstCmpInd - Portable common channel data request
 *
 *  @param[in] pst    -  Post structure  
 *  @param[in] suId   -  Service user SAP ID 
 *  @param[in] staInd -  Data Request
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PtUiKwuReEstCmpInd
(
Pst               *pst,
SuId              suId,
CmLteRlcId        rlcId
)
#else
PUBLIC S16 PtUiKwuReEstCmpInd(pst, suId, rlcId)
Pst               *pst;
SuId              suId;
CmLteRlcId        rlcId;
#endif
{
   TRC3(PtUiKwuReEstCmpInd)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(rlcId);

#if (ERRCLASS & ERRCLS_DEBUG)
   KWLOGERROR(ERRCLS_DEBUG, EKW195, ERRZERO, 
      "Improper selector for NhLiKwuStaInd\n");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);

} /* end of PtUiKwuReEstCmpInd */

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
/*
 * @brief 
 *
 *        PtUiKwuDiscSduCfm - Portable common channel data request
 *
 *  @param[in] pst    -  Post structure  
 *  @param[in] suId   -  Service user SAP ID 
 *  @param[in] staInd -  Data Request
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PtUiKwuDiscSduCfm
(
Pst               *pst,
SuId              suId,
KwuDiscSduInfo    *discCfm
)
#else
PUBLIC S16 PtUiKwuDiscSduCfm(pst, suId, discCfm)
Pst               *pst;
SuId              suId;
KwuDiscSduInfo    *discCfm;
#endif
{
   TRC3(PtUiKwuDiscSduCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(discCfm);

#if (ERRCLASS & ERRCLS_DEBUG)
   KWLOGERROR(ERRCLS_DEBUG, EKWXXX, ERRZERO, 
      "Improper selector for DiscSduCfm\n");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);

} /* end of PtUiKwuDiscSduCfm */

/**
 *
 * @brief 
 *
 *        Handler for sending the Data ack indication  to the upper layer. 
 *
 * @b Description:
 *       
 *       This function confirms the succesfull transmission of SDU
 *
 *  @param[in] pst         Post structure  
 *  @param[in] suId        Service User SAP ID 
 *  @param[in] datAckInd   DatAckInd
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 PtUiKwuDatAckInd
(
Pst               *pst,
SuId              suId,
KwuDatAckInfo     *datAckInd
)
#else
PUBLIC S16 PtUiKwuDatAckInd(pst, suId, datAckInd)
Pst               *pst;
SuId              suId;
KwuDatAckInfo     *datAckInd;
#endif
{
   TRC3(PtUiKwuDatAckInd)
   
   UNUSED(pst);
   UNUSED(suId);
   UNUSED(datAckInd);

#if (ERRCLASS & ERRCLS_DEBUG)
   KWLOGERROR(ERRCLS_DEBUG, EKWXXX, ERRZERO, 
      "Improper selector for datAckInd\n");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   RETVALUE(ROK);
}/* end of PtUiKwuDatAckInd */
#endif /* LTE_L2_MEAS */
#if (defined(PTKWKWU) || defined(KW_PDCP))

/**
 *
 * @brief 
 *
 *        PtUiKwuDatInd - Portable common channel data request
 *
 *  @param[in] pst    -  Post structure  
 *  @param[in] suId   -  Service user SAP ID 
 *  @param[in] datInd -  Data Request
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PtUiKwuDatInd
(
Pst               *pst,
SuId              suId,
KwuDatIndInfo     *datInd,
Buffer            *mBuf
)
#else
PUBLIC S16 PtUiKwuDatInd(pst, suId, datInd, mBuf)
Pst               *pst;
SuId              suId;
KwuDatIndInfo     *datInd;
Buffer            *mBuf;
#endif
{
   TRC3(PtUiKwuDatInd)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(datInd);
   UNUSED(mBuf);

#if (ERRCLASS & ERRCLS_DEBUG)
   KWLOGERROR(ERRCLS_DEBUG, EKW196, ERRZERO, 
      "Improper selector for NhLiKwuDatInd\n");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);

} /* end of PtUiKwuDatInd */
#endif /* PTKWKWU || KW_PDCP */


  
/*********************************************************************
 *             Primitives for CKW interface 
 ********************************************************************/

/* CKW Bind Confirm primitive */

PUBLIC CkwBndCfm kwUiCkwBndCfmMt[] =
{
#ifdef LCKWUICKW
   cmPkCkwBndCfm,            /* 0 - loosely coupled */
#else
   PtUiCkwBndCfm,            /* 0 - loosely coupled, portable  */
#endif /* LCCKUICKW */
#ifdef NH
   NhLiCkwBndCfm,            /* 1 - tightly coupled, RRC  */
#else
   PtUiCkwBndCfm,            /* 1 - tightly coupled, portable */
#endif /* NH */
};

/* CKW Configuration confirm primitive */

PUBLIC CkwCfgCfm kwUiCkwCfgCfmMt[] =
{
#ifdef LCKWUICKW
   cmPkCkwCfgCfm,            /* 0 - loosely coupled */
#else
   PtUiCkwCfgCfm,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUICKW */
#ifdef NH
   NhLiCkwCfgCfm,            /* 1 - tightly coupled, RRC  */
#else
   PtUiCkwCfgCfm,            /* 1 - tightly coupled, portable */
#endif /* NH */
};

PUBLIC CkwUeIdChgCfm kwUiCkwUeIdChgCfmMt[] =
{
#ifdef LCKWUICKW
   cmPkCkwUeIdChgCfm,            /* 0 - loosely coupled */
#else
   PtUiCkwUeIdChgCfm,            /* 0 - loosely coupled, portable  */
#endif /* LCKWUICKW */
#ifdef NH
   NhLiCkwUeIdChgCfm,            /* 1 - tightly coupled, RRC  */
#else
   PtUiCkwUeIdChgCfm,            /* 1 - tightly coupled, portable */
#endif /* NH */
};


  
/****************************************************************************
 *                         CKW Interface Mt functions
 ***************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for confirming  the bind request received from CKW
 *        interface.
 *
 * @b Description:
 *
 *        This function send the bind confirm primitive to the RLC user,
 *        when it receives a bind request from its service user.
 *
 *  @param[in] pst      - Post structure  
 *  @param[in] suId     - Service user SAP ID 
 *  @param[in] status   - Status
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 KwUiCkwBndCfm
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U8   status                     /* Status */
)
#else
PUBLIC S16 KwUiCkwBndCfm(pst, suId, status)
Pst *pst;                       /* post structure */
SuId suId;                      /* Service User Id */
U8   status;                    /* Status */
#endif
{
   TRC3(KwUiCkwBndCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwUiCkwBndCfmMt[pst->selector])(pst, suId, status);

   RETVALUE(ROK);

} /* end of KwUiCkwBndCfm */


/**
 *
 * @brief 
 *
 *        Handler for sending a configuration confirm to RRC.
 *
 * @b Description:
 *
 *        This function is used by RLC user to send a configuration
 *        confirmation to RRC after configuring(add/delete/modify)
 *        the RLC entities. 
 *
 *  @param[in] pst      Post structure  
 *  @param[in] transId  Transaction Id 
 *  @param[in] cfmInfo  Config Confirmation Info 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 KwUiCkwCfgCfm
(
Pst            *pst,                      /* post structure */
SuId           suId,                      /* Service User Id */
CkwCfgCfmInfo  *cfmInfo                    /* Configuration Confirm */
)
#else
PUBLIC S16 KwUiCkwCfgCfm(pst, suId, cfmInfo)
Pst            *pst;                      /* post structure */
SuId           suId;                      /* Service User Id */
CkwCfgCfmInfo  *cfmInfo;                   /* Configuration Confirm */
#endif
{
   TRC3(KwUiCkwCfgCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwUiCkwCfgCfmMt[pst->selector])(pst, suId, cfmInfo);

   RETVALUE(ROK);

} /* end of KwUiCkwCfgCfm */


/**
 *
 * @brief 
 *
 *        Handler for sending a configuration for UE ID change.
 *
 * @b Description:
 *
 *        This function is used by RLC to send a configuration
 *        confirm for UE ID change.
 *
 *  @param[in] pst      Post structure  
 *  @param[in] suId     Service User Id
 *  @param[in] transId  Transaction Id 
 *  @param[in] cfmInfo  Config Confirmation Info 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 KwUiCkwUeIdChgCfm 
(
Pst            *pst,                      /* post structure */
SuId           suId,                      /* Service User Id */
U32            transId,
CkwUeInfo      *ueInfo,
CmStatus       status
)
#else
PUBLIC S16 KwUiCkwUeIdChgCfm(pst, suId, transId,ueInfo,status)
Pst            *pst;                      /* post structure */
SuId           suId;                      /* Service User Id */
U32            transId;
CkwUeInfo      *ueInfo;
CmStatus       status;
#endif
{
   TRC3(KwUiCkwUeIdChgCfm)

   /* jump to specific primitive depending on configured selector */
   (*kwUiCkwUeIdChgCfmMt[pst->selector])(pst, suId, transId, ueInfo, status);

   RETVALUE(ROK);

} /* end of KwUiCkwCfgCfm */


#ifdef PTKWCKW
/*************************************************************************
 *                         CKW Porting Functions
 ************************************************************************/
/**
 *
 * @brief 
 *
 *        KwUiCkwBndCfm - CKW SAP bind confirm
 *
 *  @param[in] pst      - Post structure  
 *  @param[in] suId     - Service user SAP ID 
 *  @param[in] status   - Status
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PRIVATE S16 PtUiCkwBndCfm 
(
Pst *pst,                       /* post structure */
SuId suId,                      /* Service User Id */
U8 status                       /* status */
)
#else
PRIVATE S16 PtUiCkwBndCfm(pst, suId, status)
Pst *pst;                       /* post structure */
SuId suId;                      /* Service User Id */
U8 status;                      /* status */
#endif
{
   TRC3(PtUiCkwBndCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

#if (ERRCLASS & ERRCLS_DEBUG)
   KWLOGERROR(ERRCLS_DEBUG, EKW197, ERRZERO, 
      "Improper selector for NhLiCkwBndCfm\n");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);
} /* end of PtUiCkwBndCfm */


/**
 *
 * @brief 
 *
 *        PtUiCkwCfgCfm - KWU SAP bind confirm
 *
 *  @param[in] pst      - Post structure  
 *  @param[in] suId     - Service user SAP ID 
 *  @param[in] cfmInfo  - Configuration confirm
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 PtUiCkwCfgCfm
(
Pst            *pst,                      /* post structure */
SuId           suId,                      /* Service User Id */
CkwCfgCfmInfo  *cfmInfo                    /* Configuration Confirm */
)
#else
PUBLIC S16 PtUiCkwCfgCfm(pst, suId, cfmInfo)
Pst            *pst;                      /* post structure */
SuId           suId;                      /* Service User Id */
CkwCfgCfmInfo  *cfmInfo;                   /* Configuration Confirm */
#endif
{
   TRC3(PtUiCkwCfgCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(cfmInfo);

#if (ERRCLASS & ERRCLS_DEBUG)
   KWLOGERROR(ERRCLS_DEBUG, EKW198, ERRZERO, 
      "Improper selector for NhLiCkwCfgCfm\n");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);

} /* end of PtUiCkwCfgCfm */

#ifdef ANSI
PUBLIC S16 PtUiCkwUeIdChgCfm 
(
Pst            *pst,                      /* post structure */
SuId           suId,                      /* Service User Id */
U32            transId,
CkwUeInfo      *ueInfo,
CmStatus       status
)
#else
PUBLIC S16 PtUiCkwUeIdChgCfm(pst, suId,transId,ueInfo,status)
Pst            *pst;                      /* post structure */
SuId           suId;                      /* Service User Id */
U32            transId;
CkwUeInfo      *ueInfo;
CmStatus       status;
#endif
{
   TRC3(PtUiCkwUeIdChgCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transId);
   UNUSED(ueInfo);
   UNUSED(status);

#if (ERRCLASS & ERRCLS_DEBUG)
   KWLOGERROR(ERRCLS_DEBUG, EKW199, ERRZERO,
      "Improper selector for PtUiCkwUeIdChgCfm\n");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ROK);

} /* end of PtUiCkwUeIdChgCfm */
#endif /* PTKWCKW */

  
#ifdef KW_PDCP
/*********************************************************************
 *                         Primitives for CPJ interface 
 ********************************************************************/
PRIVATE CONSTANT CpjBndCfm PjUiCpjBndCfmMt[] =
{
#ifdef LCPJUICPJ
   cmPkCpjBndCfm,
#else
   PtUiCpjBndCfm,
#endif
#ifdef NH
   NhLiCpjBndCfm,
#else
   PtUiCpjBndCfm,
#endif
};


PRIVATE CONSTANT CpjCfgCfm PjUiCpjCfgCfmMt[] =
{
#ifdef LCPJUICPJ
   cmPkCpjCfgCfm,
#else
   PtUiCpjCfgCfm,
#endif
#ifdef NH
   NhLiCpjCfgCfm,
#else
   PtUiCpjCfgCfm,
#endif
};


PRIVATE CONSTANT CpjReEstCfm PjUiCpjReEstCfmMt[] =
{
#ifdef LCPJUICPJ
   cmPkCpjReEstCfm,
#else
   PtUiCpjReEstCfm,
#endif
#ifdef NH
   NhLiCpjReEstCfm,
#else
   PtUiCpjReEstCfm,
#endif
};


PRIVATE CONSTANT CpjSecCfgCfm PjUiCpjSecCfgCfmMt[] =
{
#ifdef LCPJUICPJ
   cmPkCpjSecCfgCfm,
#else
   PtUiCpjSecCfgCfm,
#endif
#ifdef NH
   NhLiCpjSecCfgCfm,
#else
   PtUiCpjSecCfgCfm,
#endif
};


PRIVATE CONSTANT CpjUeIdChgCfm PjUiCpjUeIdChgCfmMt[] =
{
#ifdef LCPJUICPJ
   cmPkCpjUeIdChgCfm,
#else
   PtUiCpjUeIdChgCfm,
#endif
#ifdef NH
   NhLiCpjUeIdChgCfm,
#else
   PtUiCpjUeIdChgCfm,
#endif
};


PRIVATE CONSTANT CpjCountCfm PjUiCpjCountCfmMt[] =
{
#ifdef LCPJUICPJ
   cmPkCpjCountCfm,
#else
   PtUiCpjCountCfm,
#endif
#ifdef NH
   NhLiCpjCountCfm,
#else
   PtUiCpjCountCfm,
#endif
};


PRIVATE CONSTANT CpjSduStaCfm PjUiCpjSduStaCfmMt[] =
{
#ifdef LCPJUICPJ
   cmPkCpjSduStaCfm,
#else
   PtUiCpjSduStaCfm,
#endif
#ifdef NH
   NhLiCpjSduStaCfm,
#else
   PtUiCpjSduStaCfm,
#endif
};

  
/****************************************************************************
 *                         CPJ Interface Mt functions
 ***************************************************************************/
#ifdef PJ
#ifdef ANSI
PUBLIC S16 PjUiCpjBndCfm
(
Pst * pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PjUiCpjBndCfm(pst, suId, status)
Pst * pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PjUiCpjBndCfm)

   (*PjUiCpjBndCfmMt[pst->selector])(pst, suId, status);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUiCpjCfgCfm
(
Pst * pst,
SuId suId,
CpjCfgCfmInfo * cfgCfm
)
#else
PUBLIC S16 PjUiCpjCfgCfm(pst, suId, cfgCfm)
Pst * pst;
SuId suId;
CpjCfgCfmInfo * cfgCfm;
#endif
{

   TRC3(PjUiCpjCfgCfm)

   (*PjUiCpjCfgCfmMt[pst->selector])(pst, suId, cfgCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUiCpjReEstCfm
(
Pst * pst,
SuId suId,
CpjReEstCfmInfo * reEstCfm
)
#else
PUBLIC S16 PjUiCpjReEstCfm(pst, suId, reEstCfm)
Pst * pst;
SuId suId;
CpjReEstCfmInfo * reEstCfm;
#endif
{

   TRC3(PjUiCpjReEstCfm)

   (*PjUiCpjReEstCfmMt[pst->selector])(pst, suId, reEstCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUiCpjSecCfgCfm
(
Pst * pst,
SuId suId,
CpjSecCfgCfmInfo * secCfgCfm
)
#else
PUBLIC S16 PjUiCpjSecCfgCfm(pst, suId, secCfgCfm)
Pst * pst;
SuId suId;
CpjSecCfgCfmInfo * secCfgCfm;
#endif
{

   TRC3(PjUiCpjSecCfgCfm)

   (*PjUiCpjSecCfgCfmMt[pst->selector])(pst, suId, secCfgCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUiCpjUeIdChgCfm
(
Pst * pst,
SuId suId,
U32 transId,
CpjUeInfo  * ueInfo,
CmStatus status
)
#else
PUBLIC S16 PjUiCpjUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst * pst;
SuId suId;
U32 transId;
CpjUeInfo  * ueInfo;
CmStatus status;
#endif
{

   TRC3(PjUiCpjUeIdChgCfm)

   (*PjUiCpjUeIdChgCfmMt[pst->selector])(pst, suId, transId, ueInfo, status);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUiCpjCountCfm
(
Pst * pst,
SuId suId,
CpjCountCfmInfo * countCfm
)
#else
PUBLIC S16 PjUiCpjCountCfm(pst, suId, countCfm)
Pst * pst;
SuId suId;
CpjCountCfmInfo * countCfm;
#endif
{

   TRC3(PjUiCpjCountCfm)

   (*PjUiCpjCountCfmMt[pst->selector])(pst, suId, countCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUiCpjSduStaCfm
(
Pst * pst,
SuId suId,
CpjSduStaCfmInfo * staCfm
)
#else
PUBLIC S16 PjUiCpjSduStaCfm(pst, suId, staCfm)
Pst * pst;
SuId suId;
CpjSduStaCfmInfo * staCfm;
#endif
{

   TRC3(PjUiCpjSduStaCfm)

   (*PjUiCpjSduStaCfmMt[pst->selector])(pst, suId, staCfm);

   RETVALUE(ROK);
}
#endif /*--ifdef PJ--*/

  
/*************************************************************************
 *                         CPJ Porting Functions
 ************************************************************************/
#ifdef PTPJCPJ

#ifdef ANSI
PUBLIC S16 PtUiCpjBndCfm
(
Pst * pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PtUiCpjBndCfm(pst, suId, status)
Pst * pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PtUiCpjBndCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiCpjCfgCfm
(
Pst * pst,
SuId suId,
CpjCfgCfmInfo * cfgCfm
)
#else
PUBLIC S16 PtUiCpjCfgCfm(pst, suId, cfgCfm)
Pst * pst;
SuId suId;
CpjCfgCfmInfo * cfgCfm;
#endif
{

   TRC3(PtUiCpjCfgCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(cfgCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiCpjReEstCfm
(
Pst * pst,
SuId suId,
CpjReEstCfmInfo * reEstCfm
)
#else
PUBLIC S16 PtUiCpjReEstCfm(pst, suId, reEstCfm)
Pst * pst;
SuId suId;
CpjReEstCfmInfo *reEstCfm;
#endif
{

   TRC3(PtUiCpjReEstCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(reEstCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiCpjSecCfgCfm
(
Pst * pst,
SuId suId,
CpjSecCfgCfmInfo * secCfgCfm
)
#else
PUBLIC S16 PtUiCpjSecCfgCfm(pst, suId, secCfgCfm)
Pst * pst;
SuId suId;
CpjSecCfgCfmInfo * secCfgCfm;
#endif
{

   TRC3(PtUiCpjSecCfgCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(secCfgCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiCpjUeIdChgCfm
(
Pst * pst,
SuId suId,
U32 transId,
CpjUeInfo  * ueInfo,
CmStatus status
)
#else
PUBLIC S16 PtUiCpjUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst * pst;
SuId suId;
U32 transId;
CpjUeInfo  * ueInfo;
CmStatus status;
#endif
{

   TRC3(PtUiCpjUeIdChgCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transId);
   UNUSED(ueInfo);
   UNUSED(status);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiCpjCountCfm
(
Pst * pst,
SuId suId,
CpjCountCfmInfo * countCfm
)
#else
PUBLIC S16 PtUiCpjCountCfm(pst, suId, countCfm)
Pst * pst;
SuId suId;
CpjCountCfmInfo * countCfm;
#endif
{

   TRC3(PtUiCpjCountCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(countCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiCpjSduStaCfm
(
Pst * pst,
SuId suId,
CpjSduStaCfmInfo * staCfm
)
#else
PUBLIC S16 PtUiCpjSduStaCfm(pst, suId, staCfm)
Pst * pst;
SuId suId;
CpjSduStaCfmInfo * staCfm;
#endif
{

   TRC3(PtUiCpjSduStaCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(staCfm);

   RETVALUE(ROK);

}

#endif /*--ifdef PTPJCPJ--*/

  
/*********************************************************************
 *                         Primitives for PJU interface 
 ********************************************************************/

PRIVATE CONSTANT PjuBndCfm PjUiPjuBndCfmMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuBndCfm,
#else
   PtUiPjuBndCfm,
#endif
#ifdef NH
   NhLiPjuBndCfm,
#else
   PtUiPjuBndCfm,
#endif
#ifdef PX
   PxLiPjuBndCfm,
#else
   PtUiPjuBndCfm,
#endif
};


PRIVATE CONSTANT PjuDatCfm PjUiPjuDatCfmMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuDatCfm,
#else
   PtUiPjuDatCfm,
#endif
#ifdef NH
   NhLiPjuDatCfm,
#else
   PtUiPjuDatCfm,
#endif
#ifdef PX
   PxLiPjuDatCfm,
#else
   PtUiPjuDatCfm,
#endif
};


PRIVATE CONSTANT PjuDatInd PjUiPjuDatIndMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuDatInd,
#else
   PtUiPjuDatInd,
#endif
#ifdef NH
   NhLiPjuDatInd,
#else
   PtUiPjuDatInd,
#endif
#ifdef PX
   PxLiPjuDatInd,
#else
   PtUiPjuDatInd,
#endif
};


PRIVATE CONSTANT PjuStaInd PjUiPjuStaIndMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuStaInd,
#else
   PtUiPjuStaInd,
#endif
#ifdef NH
   NhLiPjuStaInd,
#else
   PtUiPjuStaInd,
#endif
#ifdef PX
   PxLiPjuStaInd,
#else
   PtUiPjuStaInd,
#endif
};


PRIVATE CONSTANT PjuDatFwdInd PjUiPjuDatFwdIndMt[] =
{
#ifdef LCPJUIPJU
   cmPkPjuDatFwdInd,
#else
   PtUiPjuDatFwdInd,
#endif
#ifdef NH
   NhLiPjuDatFwdInd,
#else
   PtUiPjuDatFwdInd,
#endif
#ifdef PX
   PxLiPjuDatFwdInd,
#else
   PtUiPjuDatFwdInd,
#endif
};

  
/****************************************************************************
 *                         PJU Interface Mt functions
 ***************************************************************************/
#ifdef PJ

#ifdef ANSI
PUBLIC S16 PjUiPjuBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PjUiPjuBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PjUiPjuBndCfm)

   (*PjUiPjuBndCfmMt[pst->selector])(pst, suId, status);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjUiPjuDatCfm
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
PjuDatCfmInfo * datCfm
)
#else
PUBLIC S16 PjUiPjuDatCfm(pst, suId, pdcpId, datCfm)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
PjuDatCfmInfo * datCfm;
#endif
{

   TRC3(PjUiPjuDatCfm)

   (*PjUiPjuDatCfmMt[pst->selector])(pst, suId, pdcpId, datCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjUiPjuDatInd
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
Buffer * mBuf
)
#else
PUBLIC S16 PjUiPjuDatInd(pst, suId, pdcpId, mBuf)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
Buffer * mBuf;
#endif
{

   TRC3(PjUiPjuDatInd)

   (*PjUiPjuDatIndMt[pst->selector])(pst, suId, pdcpId, mBuf);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjUiPjuStaInd
(
Pst * pst,
SuId suId,
CmLtePdcpId * pdcpId,
PjuStaIndInfo * staInd,
Buffer    *mBuf
)
#else
PUBLIC S16 PjUiPjuStaInd(pst, suId, pdcpId, staInd, mBuf)
Pst * pst;
SuId suId;
CmLtePdcpId * pdcpId;
PjuStaIndInfo * staInd;
Buffer *mBuf;
#endif
{

   TRC3(PjUiPjuStaInd)

   (*PjUiPjuStaIndMt[pst->selector])(pst, suId, pdcpId, staInd, mBuf);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjUiPjuDatFwdInd
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
PjuDatFwdIndInfo * datFwdInd
)
#else
PUBLIC S16 PjUiPjuDatFwdInd(pst, suId, pdcpId, datFwdInd)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
PjuDatFwdIndInfo * datFwdInd;
#endif
{

   TRC3(PjUiPjuDatFwdInd)

   (*PjUiPjuDatFwdIndMt[pst->selector])(pst, suId, pdcpId, datFwdInd);

   RETVALUE(ROK);

}
#endif /*--ifdef PJ--*/

  
/*************************************************************************
 *                         PJU Porting Functions
 ************************************************************************/
#ifdef PTPJPJU

/* kw006.201 ccpu00120534, fixed the problem of passing suId and spId as argument */
#ifdef ANSI
PUBLIC S16 PtUiPjuBndReq
(
Pst* pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 PtUiPjuBndReq(pst, suId, spId)
Pst* pst;
SuId suId;
SpId spId;
#endif
{

   TRC3(PtUiPjuBndReq)

   /* kw006.201 ccpu00120534, fixed the problem of passing suId and spId as argument */
   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiPjuBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PtUiPjuBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PtUiPjuBndCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiPjuDatCfm
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
PjuDatCfmInfo * datCfm
)
#else
PUBLIC S16 PtUiPjuDatCfm(pst, suId, pdcpId, datCfm)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
PjuDatCfmInfo * datCfm;
#endif
{

   TRC3(PtUiPjuDatCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(pdcpId);
   UNUSED(datCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiPjuDatInd
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
Buffer * mBuf
)
#else
PUBLIC S16 PtUiPjuDatInd(pst, suId, pdcpId, mBuf)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
Buffer * mBuf;
#endif
{

   TRC3(PtUiPjuDatInd)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(pdcpId);
   UNUSED(mBuf);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiPjuStaInd
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
PjuStaIndInfo * staInd,
Buffer *mBuf
)
#else
PUBLIC S16 PtUiPjuStaInd(pst, suId, pdcpId, staInd, mBuf)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
PjuStaIndInfo * staInd;
Buffer *mBuf;
#endif
{

   TRC3(PtUiPjuStaInd)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(pdcpId);
   UNUSED(staInd);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtUiPjuDatFwdInd
(
Pst* pst,
SuId suId,
CmLtePdcpId * pdcpId,
PjuDatFwdIndInfo * datFwdInd
)
#else
PUBLIC S16 PtUiPjuDatFwdInd(pst, suId, pdcpId, datFwdInd)
Pst* pst;
SuId suId;
CmLtePdcpId * pdcpId;
PjuDatFwdIndInfo * datFwdInd;
#endif
{

   TRC3(PtUiPjuDatFwdInd)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(pdcpId);
   UNUSED(datFwdInd);

   RETVALUE(ROK);

}
#endif /*--ifdef PTPJPJU--*/
#endif /* KW_PDCP */

#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**
         End of file:     gp_ptui.c@@/main/2 - Fri Nov 13 14:14:00 2009
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
/main/1      ---      gk   1. initial release.
/main/2      ---      gk   1. LTERLC Release 2.1
/main/3      kw005.201 ap  1. Added support for L2 Measurement feature.
/main/4      kw006.201 ap  1. ccpu00120534, fixed the problem of passing 
                              suId and spId as argument.
*********************************************************************91*/
