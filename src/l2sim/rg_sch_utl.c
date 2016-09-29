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

     File:     rg_sch_utl.c

     Sid:      gk_sch_utl.c@@/main/2 - Sat Jul 30 02:21:53 2011

     Prg:      SY

**********************************************************************/

/** @file rg_sch_utl.c
@brief This file implements the schedulers main access to MAC layer code.
*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_err.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"
/* rg007.201 - Changes for MIMO feature addition */
/* rg009.201 - Removed dependency on MIMO compile-time flag */
#include "rg_sch_cmn.h"

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
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
/* rg007.201 - Changes for MIMO feature addition */
/* rg009.201 - Removed dependency on MIMO compile-time flag */
#include "rg_sch_cmn.x"        /* typedefs for Scheduler */

/* local defines */

EXTERN RgSchdApis rgSchCmnApis;

PRIVATE S16 rgSCHUtlUlAllocDbInit ARGS((
         RgSchCellCb    *cell,
         RgSchUlAllocDb *db,
         U8 maxAllocs
         ));
PRIVATE Void rgSCHUtlUlAllocDbDeinit ARGS((
         RgSchCellCb    *cell,
         RgSchUlAllocDb *db
         ));
PRIVATE S16 rgSCHUtlUlHoleDbInit ARGS((
         RgSchCellCb    *cell,
         RgSchUlHoleDb *db,
         U8 maxHoles,
         U8 start,
         U8 num
         ));
PRIVATE Void rgSCHUtlUlHoleDbDeinit ARGS((
         RgSchCellCb   *cell,
         RgSchUlHoleDb *db
         ));

/* sorted in ascending order of tbSz */
CONSTANT struct rgSchUtlBcchPcchTbSz
{
   U8   rbIndex;    /* RB index {2,3} */
   U16  tbSz;       /* one of the Transport block size in bits of
                     * rbIndex 2 or 3 */
   /* rg002.201: Corrected allocation for common channels */
   U8   mcs;        /* imcs */
} rgSchUtlBcchPcchTbSzTbl[44] = {
   { 2,   32,  0 }, { 2,   56,  1 }, { 2,   72,  2 }, { 3,   88,  1 },
   { 2,  104,  3 }, { 2,  120,  4 }, { 2,  144,  5 }, { 2,  176,  6 },
   { 3,  208,  4 }, { 2,  224,  7 }, { 2,  256,  8 }, { 2,  296,  9 },
   { 2,  328, 10 }, { 2,  376, 11 }, { 3,  392,  8 }, { 2,  440, 12 },
   { 3,  456,  9 }, { 2,  488, 13 }, { 3,  504, 10 }, { 2,  552, 14 },
   { 3,  584, 11 }, { 2,  600, 15 }, { 2,  632, 16 }, { 3,  680, 12 },
   { 2,  696, 17 }, { 3,  744, 13 }, { 2,  776, 18 }, { 2,  840, 19 },
   { 2,  904, 20 }, { 3,  968, 16 }, { 2, 1000, 21 }, { 2, 1064, 22 },
   { 2, 1128, 23 }, { 3, 1160, 18 }, { 2, 1192, 24 }, { 2, 1256, 25 },
   { 3, 1288, 19 }, { 3, 1384, 20 }, { 2, 1480, 26 }, { 3, 1608, 22 },
   { 3, 1736, 23 }, { 3, 1800, 24 }, { 3, 1864, 25 }, { 3, 2216, 26 }
};

/* local typedefs */

/* local externs */

/* forward references */
#ifdef LTE_TDD
PRIVATE Void rgSCHUtlUpdPrachOcc ARGS((
RgSchCellCb *cell,
RgrTddPrachInfo *cellCfg));
#endif

#define RGSCH_NUM_PCFICH_REG 4
#define RGSCH_NUM_REG_PER_CCE 9
#define RGSCH_NUM_REG_PER_PHICH_GRP 3

#ifdef LTE_TDD
/*rg009.201:[ccpu00116309]: Added iPhich*/
#define RGSCH_INITPHICH(_phich, _hqFeedBack, _nDmrs, _rbStart, _iPhich) {\
   (_phich)->hqFeedBack = _hqFeedBack; \
   (_phich)->rbStart = _rbStart; \
   (_phich)->nDmrs = _nDmrs; \
   (_phich)->iPhich = _iPhich; \
   (_phich)->lnk.next = NULLP; \
   (_phich)->lnk.prev = NULLP; \
   (_phich)->lnk.node = (PTR)(_phich); \
}
#else
#define RGSCH_INITPHICH(_phich, _hqFeedBack, _nDmrs, _rbStart) {\
   (_phich)->hqFeedBack = _hqFeedBack; \
   (_phich)->rbStart = _rbStart; \
   (_phich)->nDmrs = _nDmrs; \
   (_phich)->lnk.next = NULLP; \
   (_phich)->lnk.prev = NULLP; \
   (_phich)->lnk.node = (PTR)(_phich); \
}
#endif

#define RGSCH_PHICH_ALLOC(_inst,_dataPtr, _size, _ret) {\
   _ret = rgSCHUtlAllocSBuf(_inst, (Data **)&_dataPtr, _size); \
}

/* ccpu00117052 - MOD - Passing double pointer
for proper NULLP assignment*/
#define RGSCH_PHICH_FREE(_inst, _dataPtr, _size) {\
      rgSCHUtlFreeSBuf(_inst, (Data **)(&(_dataPtr)), _size); \
}

#ifdef TFU_UPGRADE
#define RGSCH_GETBIT(a, b)     ((((U8*)a)[(b)>>3] >> ((b)&7)) & 1)
#if 0
/*
*
*       Fun:   rgSCHUtlPower
*
*       Desc:  This function finds of the Power of x raised to n
*
*       Ret:   value of x raised to n
*
*       Notes: None
*
*       File:  rg_sch_utl.c
*
*/
#ifdef ANSI
PUBLIC F64 rgSCHUtlPower
(
F64    x,
F64    n
)
#else
PUBLIC F64 rgSCHUtlPower(x, n)
F64    x;
F64    n;
#endif
{
 if( n==0 )
 {
   RETVALUE( 1 );
 }
 else if ( n>0 )
 {
   RETVALUE( x * rgSCHUtlPower( x, n-1 ) );
 }
 else
 {
   RETVALUE( (1/x) * rgSCHUtlPower( x, n+1 ) );
 }
} /* end of rgSCHUtlPower*/

/*
*
*       Fun:   rgSCHUtlParse
*
*       Desc:  This function parses bits x to y of an array and
*                  returns the integer value  out of it.
*
*       Ret:   integer value of z bits
*
*       Notes: None
*
*       File:  rg_sch_utl.c
*
*/
#ifdef ANSI
PUBLIC U32 rgSCHUtlParse
(
U8     *buff,
U8     startPos,
U8     endPos,
U8     buffSize
)
#else
PUBLIC U32 rgSCHUtlParse(buff, startPos, endPos, buffSize)
U8     *buff;
U8     startPos;
U8     endPos;
U8     buffSize;
#endif
{
 U8 pointToChar,pointToEnd, loop;
 U8 size =  endPos - startPos;
 F64 result = 0;
 TRC2(rgSCHUtlParse);
 pointToEnd = (startPos)%8;
   for ( loop=0; loop<size; loop++)
   {
      pointToChar = (buffSize - (((startPos)+loop)/8))-1;
      if (RGSCH_GETBIT(buff+pointToChar,pointToEnd%8)==1)
      {
         result=result+(rgSCHUtlPower(2,loop));
      }
      pointToEnd++;
   }
   RETVALUE((U32)result);
} /* end of rgSCHUtlParse*/

/*
*
*       Fun:   rgSCHUtlFindDist
*
*       Desc:  This function calculates the iterations need to cover
*                  before the valid Index can be used for next possible Reception
*
*       Ret:   integer value of z bits
*
*       Notes: None
*
*       File:  rg_sch_utl.c
*
*/
#ifdef ANSI
PUBLIC U8 rgSCHUtlFindDist
(
U16    crntTime,
U16    tempIdx
)
#else
PUBLIC U8 rgSCHUtlFindDist(crntTime, tempIdx)
U16    crntTime;
U16    tempIdx;
#endif
{
 U8  dist =0;
 while(crntTime<tempIdx)
 {
    crntTime += RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
    dist+=1;
 }
 RETVALUE(dist-1);
} /* end of rgSCHUtlFindDist*/
#endif


/**
 * @brief This function checks availability of a PDCCH
 *
 * @details
 *
 *     Function: rgSCHUtlPdcchAvail
 *     Purpose:  This function checks if a particular PDCCH is in use.
 *               map field of PDCCH is used to track the CCEs arleady
 *               allocated. Each bit of map represents one CCE and the
 *               LSBit of first byte represents CCE 0.
 *               steps:
 *               1. Locate the set of bits that represent the PDCCH for
 *               the provided location.
 *               2.  If the value of the bits is non-zero one or many CCEs
 *               for the PDCCH are in use and hence the PDCCH is not available.
 *               3. If pdcch is available, assign it to [out]pdcch.
 *               4.  Set all of the bits to one. There is no check performed
 *               to see if the PDCCH is available.
 *
 *     Invoked by: scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchPdcchInfo*  pdcchInfo
 *  @param[in]  U8            loc
 *  @param[in]  U8            aggrLvl
 *  @param[out] RgSchPdcch**     pdcch
 *  @return  Boolean
 *         -#   TRUE  if available
 *         -#   FALSE otherwise
 *
 **/
#ifdef ANSI
PUBLIC Bool rgSCHUtlPdcchAvail
(
RgSchCellCb             *cell,
RgSchPdcchInfo          *pdcchInfo,
U32                     loc,
CmLteAggrLvl            aggrLvl,
RgSchPdcch              **pdcch
)
#else
PUBLIC Bool rgSCHUtlPdcchAvail(cell, pdcchInfo, loc, aggrLvl, pdcch)
RgSchCellCb             *cell;
RgSchPdcchInfo          *pdcchInfo;
U32                     loc;
CmLteAggrLvl            aggrLvl;
RgSchPdcch              **pdcch;
#endif
{
   U8                   *byte;
   U8                   offset;
   U8                   mask;
   Inst                 inst = cell->instIdx;
   U8                   aggrLvlVal;
   TRC2(rgSCHUtlPdcchAvail);
   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "rgSCHUtlPdcchAvail\n"));

   switch (aggrLvl)
   {
      case CM_LTE_AGGR_LVL1:
         byte = &pdcchInfo->map[loc >> 3];
         offset = loc & 7;
         mask = 1 << offset;
         aggrLvlVal = 1;
         break;
      case CM_LTE_AGGR_LVL2:
         byte = &pdcchInfo->map[loc >> 2];
         offset = loc & 3;
         mask = 3 << (offset * 2);
         aggrLvlVal = 2;
         break;
      case CM_LTE_AGGR_LVL4:
         byte = &pdcchInfo->map[loc >> 1];
         offset = loc & 1;
         mask = 0xf << (offset * 4);
         aggrLvlVal = 4;
         break;
      case CM_LTE_AGGR_LVL8:
         byte = &pdcchInfo->map[loc];
         mask = 0xff;
         aggrLvlVal = 8;
         break;
      default:
         RETVALUE(FALSE);
   }
   if ((*byte & mask))
   {
      RETVALUE(FALSE);
   }
   if (cell->pdcchLst.first != NULLP)
   {
      *pdcch = (RgSchPdcch *)(cell->pdcchLst.first->node);
      cmLListDelFrm(&cell->pdcchLst, cell->pdcchLst.first);
   }
   else
   {
      rgSCHUtlAllocSBuf(inst, (Data **)pdcch, sizeof(RgSchPdcch));
   }

   if (*pdcch)
   {
      (*byte) |= mask;
      (*pdcch)->aggrLvl = aggrLvl;
      cmLListAdd2Tail(&pdcchInfo->pdcchs, &((*pdcch)->lnk));
      (*pdcch)->lnk.node = (PTR)*pdcch;
      /* rg004.201: Fix for CR ccpu00109159 */
      (*pdcch)->nCce = loc * aggrLvlVal;
   }
   RETVALUE(TRUE);
}



/**
 * @brief This function releases a PDCCH
 *
 * @details
 *
 *     Function: rgSCHUtlPdcchPut
 *     Purpose:  This function releases a PDCCH.
 *               steps:
 *               1. Locate the set of bits that represent the PDCCH for
 *               the provided location.
 *               2. Set all of the bits to zero.
 *               3. Release the memory of PDCCH to the cell free Q
 *
 *     Invoked by: scheduler
 *
 *  @param[in]  RgSchPdcchInfo*  pdcchInfo
 *  @param[in]  U8            loc
 *  @param[in]  U8            aggrLvl
 *  @return     Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlPdcchPut
(
RgSchCellCb                *cell,
RgSchPdcchInfo             *pdcchInfo,
RgSchPdcch                 *pdcch
)
#else
PUBLIC Void rgSCHUtlPdcchPut(cell, pdcchInfo, pdcch)
RgSchCellCb                *cell;
RgSchPdcchInfo             *pdcchInfo;
RgSchPdcch                 *pdcch;
#endif
{
   U8                   *byte;
   U8                   offset;
   U8                   mask;
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif

   TRC2(rgSCHUtlPdcchPut);
   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "rgSCHUtlPdcchPut\n"));

   switch(pdcch->aggrLvl)
   {
      case CM_LTE_AGGR_LVL1:
         offset = pdcch->nCce & 7;
         mask = 1 << offset;
         break;
      case CM_LTE_AGGR_LVL2:
         offset = (pdcch->nCce >> 1) & 3;
         mask = 3 << offset;
         break;
      case CM_LTE_AGGR_LVL4:
         offset = (pdcch->nCce >> 2) & 1;
         mask = 0xf << offset;
         break;
      case CM_LTE_AGGR_LVL8:
         mask = 0xff;
         break;
      default:
         RETVOID;
   }
   /* Placing common computation of byte from all the cases above here
      for optimization */
    byte = &pdcchInfo->map[pdcch->nCce >> 3];

   /* rg002.101:ccpu00101820: Fixing incorrect del/Add order */
   cmLListDelFrm(&pdcchInfo->pdcchs, &pdcch->lnk);
   cmLListAdd2Tail(&cell->pdcchLst, &pdcch->lnk);
   pdcch->lnk.node = (PTR)pdcch;
   (*byte) &= ~mask;

  /* Stack Crash problem for TRACE5 changes. added the return below . */
 RETVOID;


}


/**
 * @brief This function initializes PDCCH information for frame
 *
 * @details
 *
 *     Function: rgSCHUtlPdcchInit
 *     Purpose:  This function initializes PDCCH information for
 *               a subframe. It removes the list of PDCCHs allocated
 *               in the prior use of this subframe structure.
 *
 *     Invoked by: rgSCHUtlSubFrmPut
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSubFrm*     subFrm
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHUtlPdcchInit
(
RgSchCellCb             *cell,
RgSchDlSf               *subFrm,
U16                     nCce
)
#else
PRIVATE Void rgSCHUtlPdcchInit(cell, subFrm, nCce)
RgSchCellCb             *cell;
RgSchDlSf               *subFrm;
U16                     nCce;
#endif
{
   RgSchPdcchInfo       *pdcchInfo;
   RgSchPdcch           *pdcch;
   Inst                 inst = cell->instIdx;
   U8                   extraBits;

   TRC2(rgSCHUtlPdcchInit);
   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "rgSCHUtlPdcchInit\n"));

   pdcchInfo = &subFrm->pdcchInfo;
   while(pdcchInfo->pdcchs.first != NULLP)
   {
      pdcch = (RgSchPdcch *)pdcchInfo->pdcchs.first->node;
      cmLListDelFrm(&pdcchInfo->pdcchs, pdcchInfo->pdcchs.first);
      cmLListAdd2Tail(&cell->pdcchLst, &pdcch->lnk);
   }
   cmLListInit(&pdcchInfo->pdcchs);

   /* The bitMap array size is the number of ceiling(CCEs/8)*/
   if (pdcchInfo->nCce == nCce)
   {
      cmMemset(pdcchInfo->map, 0, (pdcchInfo->nCce + 7) >> 3);
      /* If nCce is not exactly same as the bitMap size(no of bits allocated
       * to represent the Cce's, then mark the extra bits as unavailable
       extra bits = (((pdcchInfo->nCce + 7) >> 3)*8) - pdcchInfo->nCce
       The last byte of bit map = subFrm->pdcchInfo.map[((pdcchInfo->nCce + 7) >> 3) - 1]
       NOTE : extra bits are most significant of the last byte eg. */
      extraBits = ((pdcchInfo->nCce + 7) >> 3)*8 - pdcchInfo->nCce;
      subFrm->pdcchInfo.map[((pdcchInfo->nCce + 7) >> 3) - 1] |=
       ((1 << extraBits) - 1)  << (8 - extraBits);
      RETVOID;
   }
   else
   {
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)(&(pdcchInfo->map)),
               (pdcchInfo->nCce + 7) >> 3);
   }

   pdcchInfo->nCce = nCce;
   rgSCHUtlAllocSBuf(inst, (Data **)&pdcchInfo->map, ((pdcchInfo->nCce + 7) >> 3));
   if (pdcchInfo->map == NULLP)
   {
      /* Generate log error here */
      RETVOID;
   }
   cmMemset(subFrm->pdcchInfo.map, 0, ((pdcchInfo->nCce + 7) >> 3));
   /* If nCce is not exactly same as the bitMap size(no of bits allocated
    * to represent the Cce's, then mark the extra bits as unavailable
    extra bits = (((pdcchInfo->nCce + 7) >> 3)*8) - pdcchInfo->nCce
    The last byte of bit map = subFrm->pdcchInfo.map[((pdcchInfo->nCce + 7) >> 3) - 1]
    NOTE : extra bits are most significant of the last byte eg.  */
   extraBits = ((pdcchInfo->nCce + 7) >> 3)*8 - pdcchInfo->nCce;
   subFrm->pdcchInfo.map[((pdcchInfo->nCce + 7) >> 3) - 1] |=
     ((1 << extraBits) - 1) << (8 - extraBits);
   RETVOID;
}

/**
 * @brief This function appends PHICH information for frame
 *
 * @details
 *
 *     Function: rgSCHUtlAddPhich
 *     Purpose:  This function appends PHICH information for
 *               a subframe.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSubFrm*     subFrm
 *  @param[in]  U8            hqFeedBack
 *  @param[in]  U8            nDmrs
 *  @param[in]  U8            rbStart
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef LTE_TDD
/*rg009.201:[ccpu00116309]: Changes for iPhich*/
#ifdef ANSI
PUBLIC S16 rgSCHUtlAddPhich
(
RgSchCellCb             *cell,
CmLteTimingInfo         frm,
U8                      hqFeedBack,
U8                      nDmrs,
U8                      rbStart,
U8                      iPhich
)
#else
PUBLIC S16 rgSCHUtlAddPhich(cell, frm, hqFeedBack, nDmrs, rbStart, iPhich)
RgSchCellCb             *cell;
CmLteTimingInfo         frm;
U8                      hqFeedBack;
U8                      nDmrs;
U8                      rbStart;
U8                      iPhich;
#endif
#else
#ifdef ANSI
PUBLIC S16 rgSCHUtlAddPhich
(
RgSchCellCb             *cell,
CmLteTimingInfo         frm,
U8                      hqFeedBack,
U8                      nDmrs,
U8                      rbStart
)
#else
PUBLIC S16 rgSCHUtlAddPhich(cell, frm, hqFeedBack, nDmrs, rbStart)
RgSchCellCb             *cell;
CmLteTimingInfo         frm;
U8                      hqFeedBack;
U8                      nDmrs;
U8                      rbStart;
#endif
#endif
{
   S16                ret;
   RgSchPhich         *phich;
   RgSchDlSf          *dlSf;
   Inst               inst = cell->instIdx;
   TRC2(rgSCHUtlAddPhich);

   dlSf = rgSCHUtlSubFrmGet(cell, frm);
   RGSCH_PHICH_ALLOC(inst, phich,sizeof(RgSchPhich), ret);

   if(ret != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst), " rgSCHUtlAddPhich(): "
               "Allocation of RgSchPhich failed\n"));
      RETVALUE(RFAILED);
   }
#ifdef LTE_TDD
   /*rg009.201:[ccpu00116309]:Update iPhich*/
   RGSCH_INITPHICH(phich, hqFeedBack, nDmrs, rbStart, iPhich);
#else
   RGSCH_INITPHICH(phich, hqFeedBack, nDmrs, rbStart);
#endif
   cmLListAdd2Tail(&dlSf->phichInfo.phichs, &phich->lnk);
   RETVALUE(ROK);
} /* rgSCHUtlAddPhich */

/**
 * @brief This function resets PHICH information for frame
 *
 * @details
 *
 *     Function: rgSCHUtlPhichReset
 *     Purpose:  This function initializes PHICH information for
 *               a subframe. It removes the list of PHICHs allocated
 *               in the prior use of this subframe structure.
 *
 *     Invoked by: rgSCHUtlSubFrmPut
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSubFrm*     subFrm
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHUtlPhichReset
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm
)
#else
PRIVATE Void rgSCHUtlPhichReset(cell, subFrm)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
#endif
{
   RgSchPhichInfo          *phichInfo;
   RgSchPhich              *phich;

   UNUSED(cell);

   TRC2(rgSCHUtlPhichReset);

   phichInfo = &subFrm->phichInfo;
   while(phichInfo->phichs.first != NULLP)
   {
      phich = (RgSchPhich *)phichInfo->phichs.first->node;
      cmLListDelFrm(&phichInfo->phichs, phichInfo->phichs.first);
      RGSCH_PHICH_FREE(cell->instIdx, phich, sizeof(RgSchPhich));
   }
   cmLListInit(&phichInfo->phichs);
   RETVOID;
} /* rgSCHUtlPhichReset */


/**
 * @brief This function returns subframe data structure for a cell
 *
 * @details
 *
 *     Function: rgSCHUtlSubFrmGet
 *     Purpose:  This function resets the subframe data structure
 *               when the subframe is released
 *
 *     Invoked by: scheduler
 *
 *  @param[in]  RgSubFrm  subFrm
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC RgSchDlSf* rgSCHUtlSubFrmGet
(
RgSchCellCb            *cell,
CmLteTimingInfo        frm
)
#else
PUBLIC RgSchDlSf* rgSCHUtlSubFrmGet(cell, frm)
RgSchCellCb            *cell;
CmLteTimingInfo        frm;
#endif
{
   RgSchDlSf            *sf;
#ifdef DEBUGP
   Inst                 inst = cell->instIdx;
#endif
#ifdef LTE_TDD
   U8                   dlIdx;
#endif

   TRC2(rgSCHUtlSubFrmGet);
   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "rgSCHUtlSubFrmGet\n"));

#ifdef LTE_TDD
   dlIdx = rgSCHUtlGetDlSfIdx(cell, &frm);
   sf = cell->subFrms[dlIdx];
#else
   /* rg009.201 [ccpu00115187] Changing the idexing
      so that proper subframe is selected */
   sf = cell->subFrms[((frm.sfn & 1) * RGSCH_NUM_SUB_FRAMES) + frm.subframe];
#endif
   RETVALUE(sf);
}


/**
 * @brief This function returns subframe data structure for a cell
 *
 * @details
 *
 *     Function: rgSCHUtlSubFrmPut
 *     Purpose:  This function resets the subframe data structure
 *               when the subframe is released
 *
 *     Invoked by: scheduler
 *
 *  @param[in]  RgSubFrm  subFrm
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlSubFrmPut
(
RgSchCellCb            *cell,
RgSchDlSf              *sf
)
#else
PUBLIC Void rgSCHUtlSubFrmPut(cell, sf)
RgSchCellCb            *cell;
RgSchDlSf              *sf;
#endif
{
   U8                  i;
   U8                  noRaRsps;
#ifdef DEBUGP
   Inst                inst = cell->instIdx;
#endif

   TRC2(rgSCHUtlSubFrmPut);
   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "rgSCHUtlSubFrmPut\n"));

#ifdef LTE_TDD
   /* Release all the held PDCCH information */
   rgSCHUtlPdcchInit(cell, sf, sf->nCce);
#else
   /* Release all the held PDCCH information */
   rgSCHUtlPdcchInit(cell, sf, cell->nCce);
#endif
   rgSCHUtlPhichReset(cell, sf);

   /* Reset the bw allocated. */
   sf->bwAssigned = 0;
   /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
   /* Setting allocated bandwidth to SPS bandwidth for non-SPS RB allocator */
   sf->bwAlloced = ((cell->dlSpsCfg.maxSpsDlBw +
      cell->rbgSize - 1)/cell->rbgSize) * cell->rbgSize;
   sf->spsAllocdBw = 0;
   cmMemset((U8*) &sf->dlSfAllocInfo, 0, sizeof(RgSchDlSfAllocInfo));
#else
   sf->bwAlloced = 0;
   /* MS_FIX for ccpu00123918*/
   sf->type2Start = 0;
#endif
   sf->txDone = FALSE;

   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   /* Non DLFS scheduling using Type0 RA requires the following
    * parameter's tracking */
   /* Type 2 localized allocations start from 0th RBG and onwards */
   /* Type 0 allocations start from last RBG and backwards*/
   /*rg008.201 - Added support for SPS*/
#ifndef LTEMAC_SPS
   sf->type2End   = 0;
#else
   sf->type2End   = sf->bwAlloced/cell->rbgSize;
#endif
   sf->type0End   = cell->noOfRbgs - 1;
   /* If last RBG is of incomplete size then special handling */
   (sf->bw % cell->rbgSize == 0)? (sf->lstRbgDfct = 0) :
      (sf->lstRbgDfct = cell->rbgSize - (sf->bw % cell->rbgSize));
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   /* This resets the allocation for BCCH and PDCCH */
   sf->bch.tb     = NULLP;
   sf->bch.tbSize = 0;
   sf->bcch.pdcch = NULLP;
   sf->pcch.pdcch = NULLP;
#ifdef LTE_TDD
   noRaRsps = RGSCH_MAX_TDD_RA_RSP_ALLOC;
#else
   noRaRsps = RGSCH_MAX_RA_RSP_ALLOC;
#endif
   for (i = 0; i < noRaRsps; i++)
   {
      sf->raRsp[i].pdcch = NULLP;
      cmLListInit(&(sf->raRsp[i].raRspLst));
   }
   RETVOID;
}


/**
 * @brief This function computes log N (32 bit Unsigned) to the base 2
 *
 * @details
 *
 *     Function: rgSCHUtlLog32bitNbase2
 *     Purpose:  This function computes log N (32 bit Unsigned) to the base 2.
 *               For n= 0,1 ret = 0.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  U32       n
 *  @return  U8
 *
 **/
#ifdef ANSI
PUBLIC U8 rgSCHUtlLog32bitNbase2
(
U32               n
)
#else
PUBLIC U8 rgSCHUtlLog32bitNbase2(n)
U32               n;
#endif
{
   U32            b[] = {0x2, 0xc, 0xf0, 0xff00, 0xffff0000};
   U32            s[] = {1, 2, 4, 8, 16};
   S16            i;
   U8             ret = 0;

   TRC2(rgSCHUtlLog32bitNbase2)

   for (i=4; i >= 0; i--)
   {
      if (n & b[i])
      {
         n >>= s[i];
         ret |= s[i];
      }
   }
   RETVALUE(ret);
}



/**
 * @brief This function computes the Y value for UE
 *
 * @details
 *
 *     Function: rgSCHUtlUpdY
 *     Purpose:  This function updates the value of Y stored in the
 *               second parameter. It uses the previously computed
 *               value for computing for this subframe.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  U16       rnti
 *  @param[out] U32       *y
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlUpdY
(
U16               rnti,
U32               *y
)
#else
PUBLIC Void rgSCHUtlUpdY(rnti, y)
U16               rnti;
U32               *y;
#endif
{
   U32            start;
   U8             i;
   TRC2(rgSCHUtlUpdY);

   start = rnti;
   for (i = 0; i < RGSCH_NUM_SUB_FRAMES; i++)
   {
      start = (start * 39827) % 65537;
      y[i] = start;
   }
   RETVOID;
}


/**
 * @brief This function returns the Y value of UE for a sub frame
 *
 * @details
 *
 *     Function: rgSCHUtlGetY
 *     Purpose:  This function updates the value of Y stored in the
 *               UE control block. It uses the previously computed
 *               value for computing for this subframe.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchUeCb*   ue
 *  @param[in]  U8        subFrm
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC U32 rgSCHUtlGetY
(
RgSchUeCb         *ue,
U8                subFrm
)
#else
PUBLIC U32 rgSCHUtlGetY(ue, subFrm)
RgSchUeCb         *ue;
U8                subFrm;
#endif
{
   TRC2(rgSCHUtlGetY);
   /* Must be covered under error flags */
   if (subFrm > 9)
   {
      /* Log error here */
   }
   RETVALUE(ue->y[subFrm]);
}
/*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS

/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHUtlDlRelPdcchFbk
 *     Purpose:  Calls scheduler's handler for SPS release PDCCH feedback
 *     information.
 *
 *     Invoked by: DHM
 *
 *  @param[in]   RgSchCellCb     *cell
 *  @param[in]   RgSchUeCb       *ue
 *  @param[in]   Bool            isAck
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlRelPdcchFbk
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
Bool               isAck
)
#else
PUBLIC Void rgSCHUtlDlRelPdcchFbk(cell, ue, isAck)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
Bool               isAck;
#endif
{
   TRC2(rgSCHUtlDlRelPdcchFbk);
   cell->sc.apis->rgSCHDlRelPdcchFbk(cell, ue, isAck);
   RETVOID;
}

/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHUtlDlProcDtx
 *     Purpose:  Calls scheduler's handler to process DTX
 *     information.
 *
 *     Invoked by: DHM
 *
 *  @param[in]   RgSchCellCb     *cell
 *  @param[in]   RgSchDlHqProcCb *hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlProcDtx
(
RgSchCellCb        *cell,
RgSchDlHqProcCb    *hqP
)
#else
PUBLIC Void rgSCHUtlDlProcDtx(cell, hqP)
RgSchCellCb        *cell;
RgSchDlHqProcCb    *hqP;
#endif
{
   TRC2(rgSCHUtlDlProcDtx);
   cell->sc.apis->rgSCHDlProcDtx(cell, hqP);
   RETVOID;
}
#endif /* LTEMAC_SPS */

/***********************************************************
 *
 *     Func : rgSCHUtlCalcTotalRegs
 *
 *     Desc : Calculate total REGs, given a bandwidth, CFI
 *            and number of antennas.
 *
 *     Ret  : Total REGs (U16)
 *
 *     Notes: Could optimise if bw values are limited
 *            (taken from RRC spec) by indexing values from
 *            a table.
 *            Input values are not validated. CFI is assumed
 *            to be 1/2/3/4.
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U16 rgSCHUtlCalcTotalRegs
(
U8    bw,
U8    cfi,
U8    numAntna,
Bool  isEcp
)
#else
PRIVATE U16 rgSCHUtlCalcTotalRegs(bw, cfi, numAntna, isEcp)
U8    bw;
U8    cfi;
U8    numAntna;
Bool  isEcp;
#endif
{
   U16 regs = 0;
   TRC2(rgSCHUtlCalcTotalRegs);

   /*ccpu00116757-  removed check for (ERRCLASS & ERRCLS_DEBUG)*/

   if (bw <= 10)
      ++cfi;
   switch (cfi)
   {
      /*
       * For symbols 2,3 and 4, there are 3 REGs per RB in case of normal duration single antenna
       * configuration. Symbol 4 shall have 2 REGs per RB in case of ECP.
       * For symbol 1, there are 2 REGs per RB always.
       */
      case 4:
         if (isEcp)
            regs += bw * 2;
         else
            regs += bw * 3;
      case 3:
         regs += bw * 3;
      case 2:
         regs += bw * 3;
      default: /* case 1 */
         regs += bw * 2;
   }
   RETVALUE(regs);
}

/***********************************************************
 *
 *     Func : rgSCHUtlCalcPhichRegs
 *
 *     Desc : Calculates number of PHICH REGs
 *
 *     Ret  : Number of PHICH REGs (U8)
 *
 *     Notes: ng6 is Ng multiplied by 6
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U16 rgSCHUtlCalcPhichRegs
(
U8 bw,
U8 ng6
)
#else
PRIVATE U16 rgSCHUtlCalcPhichRegs(bw, ng6)
U8 bw;
U8 ng6;
#endif
{
   TRC2(rgSCHUtlCalcPhichRegs);
   /* ccpu00115330: Corrected the calculation for number of PHICH groups*/
   RETVALUE(RGSCH_CEIL((bw * ng6) ,(8 * 6)) * RGSCH_NUM_REG_PER_PHICH_GRP);
}

#ifdef LTE_TDD
/**
 * @brief Calculates total CCEs (N_cce)
 *
 * @details
 *
 *     Function: rgSCHUtlCalcNCce
 *     Purpose:  This function calculates and returns total CCEs for a
 *               cell, given the following: bandwidth, Ng configuration
 *               (multiplied by six), cfi (actual number of control
 *               symbols), m factor for PHICH  and number of antennas.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  U8      bw
 *  @param[in]  U8      ng6
 *  @param[in]  U8      cfi
 *  @param[in]  U8      mPhich
 *  @param[in]  U8      numAntna
 *  @param[in]  Bool    isEcp
 *  @return     N_cce (U8)
 *
 **/
#ifdef ANSI
PUBLIC U8 rgSCHUtlCalcNCce
(
U8            bw,
RgrPhichNg    ng,
U8            cfi,
U8            mPhich,
U8            numAntna,
Bool          isEcp
)
#else
PUBLIC U8 rgSCHUtlCalcNCce(bw, ng, cfi, mPhich, numAntna, isEcp)
U8             bw;
RgrPhichNg     ng;
U8             cfi;
U8             mPhich;
U8             numAntna;
Bool           isEcp;
#endif
{
   U16         totalRegs;
   U16         phichRegs;
   U16         cceRegs;
   U8          ng6;
   TRC2(rgSCHUtlCalcNCce);

   /*ccpu00116757-  removed check for (ERRCLASS & ERRCLS_DEBUG)*/

   switch (ng)
   {
      case RGR_NG_ONESIXTH:
         ng6 = 1;
         break;
      case RGR_NG_HALF:
         ng6 = 3;
         break;
      case RGR_NG_ONE:
         ng6 = 6;
         break;
      case RGR_NG_TWO:
      default:
         ng6 = 12;
         break;
   }

   totalRegs = rgSCHUtlCalcTotalRegs(bw, cfi, numAntna, isEcp);
   phichRegs = rgSCHUtlCalcPhichRegs(bw, ng6);
   cceRegs   = totalRegs - mPhich*phichRegs - RGSCH_NUM_PCFICH_REG;

   RETVALUE((U8)(cceRegs/RGSCH_NUM_REG_PER_CCE));
}

#else
/**
 * @brief Calculates total CCEs (N_cce)
 *
 * @details
 *
 *     Function: rgSCHUtlCalcNCce
 *     Purpose:  This function calculates and returns total CCEs for a
 *               cell, given the following: bandwidth, Ng configuration
 *               (multiplied by six), cfi (actual number of control
 *               symbols) and number of antennas.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  U8      bw
 *  @param[in]  U8      ng6
 *  @param[in]  U8      cfi
 *  @param[in]  U8      numAntna
 *  @return     N_cce (U8)
 *
 **/
#ifdef ANSI
PUBLIC U8 rgSCHUtlCalcNCce
(
U8            bw,
RgrPhichNg    ng,
U8            cfi,
U8            numAntna,
Bool          isEcp
)
#else
PUBLIC U8 rgSCHUtlCalcNCce(bw, ng, cfi, numAntna, isEcp)
U8             bw;
RgrPhichNg     ng;
U8             cfi;
U8             numAntna;
Bool           isEcp;
#endif
{
   U16         totalRegs;
   U16         phichRegs;
   U16         cceRegs;
   U8          ng6;
   TRC2(rgSCHUtlCalcNCce);

   /*ccpu00116757-  removed check for (ERRCLASS & ERRCLS_DEBUG)*/

   switch (ng)
   {
      case RGR_NG_ONESIXTH:
         ng6 = 1;
         break;
      case RGR_NG_HALF:
         ng6 = 3;
         break;
      case RGR_NG_ONE:
         ng6 = 6;
         break;
      case RGR_NG_TWO:
      default:
         ng6 = 12;
         break;
   }

   totalRegs = rgSCHUtlCalcTotalRegs(bw, cfi, numAntna, isEcp);
   phichRegs = rgSCHUtlCalcPhichRegs(bw, ng6);
   cceRegs   = totalRegs - phichRegs - RGSCH_NUM_PCFICH_REG;

   RETVALUE((U8)(cceRegs/RGSCH_NUM_REG_PER_CCE));
}
#endif

/**
 * @brief Returns PHICH info associated with an uplink
 *        HARQ process allocation
 *
 * @details
 *
 *     Function: rgSCHUtlGetPhichInfo
 *     Purpose:  This function returns PHICH info associated with
 *               an uplink HARQ process allocation. PHICH info
 *               comprises RB start and N_dmrs.
 *
 *  @param[in]  RgSchUlHqProcCb   *hqProc
 *  @param[out] U8             *rbStartRef
 *  @param[out] U8             *nDmrsRef
 *  @return  S16
 **/
#ifdef LTE_TDD
/*rg009.201:[ccpu00116309]:Changes for storing and passing iPhich at TFU
 * interface*/
#ifdef ANSI
PUBLIC S16 rgSCHUtlGetPhichInfo
(
RgSchUlHqProcCb   *hqProc,
U8                *rbStartRef,
U8                *nDmrsRef,
U8                *iPhich
)
#else
PUBLIC S16 rgSCHUtlGetPhichInfo(hqProc, rbStartRef, nDmrsRef, iPhich)
RgSchUlHqProcCb   *hqProc;
U8                *rbStartRef;
U8                *nDmrsRef;
U8                *iPhich;
#endif
#else
#ifdef ANSI
PUBLIC S16 rgSCHUtlGetPhichInfo
(
RgSchUlHqProcCb   *hqProc,
U8                *rbStartRef,
U8                *nDmrsRef
)
#else
PUBLIC S16 rgSCHUtlGetPhichInfo(hqProc, rbStartRef, nDmrsRef)
RgSchUlHqProcCb   *hqProc;
U8                *rbStartRef;
U8                *nDmrsRef;
#endif
#endif
{
   TRC2(rgSCHUtlGetPhichInfo);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (hqProc->alloc == NULLP)
   {
      RETVALUE(RFAILED);
   }
#endif
   *rbStartRef = hqProc->alloc->grnt.rbStart;
   *nDmrsRef   = hqProc->alloc->grnt.nDmrs;
#ifdef LTE_TDD
   /*rg009.201:[ccpu00116309]: Update iPhich*/
   *iPhich     = hqProc->iPhich;
#endif
   RETVALUE(ROK);
}
#if 0
/* rg009.201. Added changes of TFU_UPGRADE */
#ifndef TFU_UPGRADE
/* rg001.201:107387: To include the length and ModOrder in DataRecp Req. */
/* rg006.201: [ccpu00111718] Updating NDI and HARQ proc Id */
/**
 * @brief Returns uplink grant information required to permit
 *        PHY to receive data
 *
 * @details
 *
 *     Function: rgSCHUtlAllocRcptInfo
 *     Purpose:  Given an uplink allocation, this function returns
 *               uplink grant information which is needed by PHY to
 *               decode data sent from UE. This information includes:
 *               - RB start
 *               - Number of RBs
 *               - RV
 *
 *  @param[in]  RgSchUlAlloc   *alloc
 *  @param[out] U8             *rbStartRef
 *  @param[out] U8             *numRbRef
 *  @param[out] U8             *rvRef
 *  @param[out] U16            *size
 *  @param[out] TfuModScheme   *modType
 *  @param[out] Bool           *isRtx
 *  @param[out] U8             *nDmrs
 *  @param[out] Bool           *ndi
 *  @param[out] U8             *hqPId
 *  @return  S16
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlAllocRcptInfo
(
RgSchUlAlloc   *alloc,
CmLteRnti      *rnti,
U8             *iMcsRef,
U8             *rbStartRef,
U8             *numRbRef,
U8             *rvRef,
U16            *size,
TfuModScheme   *modType,
Bool           *isRtx,
U8             *nDmrs,
Bool           *ndi,
U8             *hqPId
)
#else
PUBLIC S16 rgSCHUtlAllocRcptInfo(alloc, rnti, iMcsRef, rbStartRef, numRbRef,
                                 rvRef, size, modType, isRtx, nDmrs, ndi,
                                 hqPId)
RgSchUlAlloc   *alloc;
CmLteRnti      *rnti;
U8             *iMcsRef;
U8             *rbStartRef;
U8             *numRbRef;
U8             *rvRef;
U16            *size;
TfuModScheme   *modType;
Bool           *isRtx;
U8             *nDmrs;
Bool           *ndi;
U8             *hqPId;
#endif
{
   /*rg001.301 -MOD-[ccpu00117270]modulation order for 16qam UEs would be
    * min(4,modulation order in grant). Please refer to 36.213-8.6.1*/
   CmLteUeCategory ueCtgy;

   TRC2(rgSCHUtlAllocRcptInfo);
#if (ERRCLASS & ERRCLS_DEBUG)
   if ((alloc == NULLP) || (alloc->hqProc == NULLP))
   {
      RETVALUE(RFAILED);
   }
#endif

   if ( !alloc->forMsg3 )
   {
     ueCtgy =  (RG_SCH_CMN_GET_UE_CTGY(alloc->ue));
   }

   /* rg004.201: Memory Fix:*/
   *iMcsRef    = alloc->grnt.iMcs;
   *rbStartRef = alloc->grnt.rbStart;
   *numRbRef   = alloc->grnt.numRb;
   *rvRef      = rgRvTable[alloc->hqProc->rvIdx];
   *rnti       = alloc->rnti;
   /* rg001.201:107387: To include the length and ModOrder in DataRecp Req. */
   *size       = alloc->grnt.datSz;
   /*rg001.301-MOD-[ccpu00117270]modulation order min(4,mod in grant) for 16 qam UEs*/
   *modType    = (alloc->forMsg3)? alloc->grnt.modOdr:
                         ((ueCtgy == CM_LTE_UE_CAT_5)?
                         alloc->grnt.modOdr:
                         (RGSCH_MIN(RGSCH_QM_QPSK,alloc->grnt.modOdr)));
   *isRtx      = alloc->grnt.isRtx;
   *nDmrs      = alloc->grnt.nDmrs;
   /* rg006.201: [ccpu00111718] Updating NDI and HARQ proc Id */
   *ndi        = alloc->hqProc->ndi;
   *hqPId      = alloc->hqProc->procId;

   RETVALUE(ROK);
}
#else
/**
 * @brief Returns uplink grant information required to permit
 *        PHY to receive data
 *
 * @details
 *
 *     Function: rgSCHUtlAllocRcptInfo
 *     Purpose:  Given an uplink allocation, this function returns
 *               uplink grant information which is needed by PHY to
 *               decode data sent from UE. This information includes:
 *               - RB start
 *               - Number of RBs
 *               - RV
 *
 *  @param[in]  RgSchUlAlloc   *alloc
 *  @param[out] U8             *rbStartRef
 *  @param[out] U8             *numRbRef
 *  @param[out] U8             *rvRef
 *  @param[out] U16            *size
 *  @param[out] TfuModScheme   *modType
 *  @return  S16
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlAllocRcptInfo
(
RgSchCellCb             *cell,
RgSchUlAlloc            *alloc,
CmLteTimingInfo         *timeInfo,
TfuUeUlSchRecpInfo      *recpReq
)
#else
PUBLIC S16 rgSCHUtlAllocRcptInfo(cell, alloc, timeInfo, recpReq)
RgSchCellCb             *cell;
RgSchUlAlloc            *alloc;
CmLteTimingInfo         *timeInfo;
TfuUeUlSchRecpInfo      *recpReq;
#endif
{
#ifdef LTE_TDD
   U16           idx;
   U8            ulDlCfgIdx = cell->ulDlCfgIdx;
#endif

   TRC2(rgSCHUtlAllocRcptInfo);
#if (ERRCLASS & ERRCLS_DEBUG)
   if ((alloc == NULLP) || (alloc->hqProc == NULLP))
   {
      RETVALUE(RFAILED);
   }
#endif
   recpReq->size           = alloc->grnt.datSz;
   recpReq->rbStart        = alloc->grnt.rbStart;
   recpReq->numRb          = alloc->grnt.numRb;
   /*rg001.301-MOD-[ccpu00117270]modulation order min(4,mod in grant) for 16 qam UEs.
    * Please refer to 36.213-8.6.1*/
   recpReq->modType        = (alloc->forMsg3)?alloc->grnt.modOdr:
                             ((alloc->ue->ueCatEnum == CM_LTE_UE_CAT_5)?
                              alloc->grnt.modOdr:
                             (RGSCH_MIN(RGSCH_QM_QPSK,alloc->grnt.modOdr)));
   recpReq->nDmrs          = alloc->grnt.nDmrs;
   recpReq->hoppingEnbld   = FALSE;
   recpReq->hoppingBits    = 0;
   recpReq->isRtx          = alloc->grnt.isRtx;
   recpReq->rv             = rgRvTable[alloc->hqProc->rvIdx];
#ifndef LTE_TDD
   recpReq->harqProcId = (timeInfo->sfn * RGSCH_NUM_SUB_FRAMES +
         timeInfo->subframe) % RGSCH_NUM_UL_HQ_PROC;
#else
   idx = RGSCH_NUM_SUB_FRAMES - \
         rgSchTddNumDlSubfrmTbl[ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
   idx = ((idx * timeInfo->sfn) + \
         rgSchTddNumUlSubfrmTbl[ulDlCfgIdx][timeInfo->subframe]) - 1;
   recpReq->harqProcId = idx % (rgSchTddUlNumHarqProcTbl[ulDlCfgIdx]);
#endif
   /* Transmission mode is SISO till Uplink MIMO is implemented. */
   recpReq->txMode   = 0;
   /* This value needs to filled in in the case of frequency hopping. */
   recpReq->crntTxNb = 0;

#ifdef MSPD
   recpReq->mcs = alloc->grnt.iMcs;
#endif
  /* ccpu00117050 - DEL - nSrs setting at rgSCHUtlAllocRcptInfo */
   RETVALUE(ROK);
}
#endif
#endif
#ifdef LTE_TDD
/**
 * @brief This function initialises the PRACH subframe occasions
 *
 * @details
 *
 *     Function: rgSCHUtlUpdPrachOcc
 *     Purpose:  This function updates the PRACH subframes based on
 *               RGR configuration.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgrTddPrachInfo  *cellCfg
 *  @return     Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHUtlUpdPrachOcc
(
RgSchCellCb       *cell,
RgrTddPrachInfo   *cellCfg
)
#else
PRIVATE Void rgSCHUtlUpdPrachOcc(cell, cellCfg)
RgSchCellCb       *cell;
RgrTddPrachInfo   *cellCfg;
#endif
{
   U8   idx;
   U8    count = 0;
   U8    size;
   U8    startIdx;
   U8    endIdx;

   TRC2(rgSCHUtlUpdPrachOcc)

   /* In the 1st half frame */
   if(cellCfg->halfFrm == 0)
   {
      startIdx = 2;
      endIdx = 6;
   }
   /* In the 2nd half frame */
   else
   {
      startIdx = 6;
      endIdx = 10;
   }
   for(idx = startIdx; idx < endIdx; idx++)
   {
      if(rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx][idx]
            == RG_SCH_TDD_UL_SUBFRAME)
      {
         if(cellCfg->ulStartSfIdx == count)
         {
            size = cell->rachCfg.raOccasion.size;
            cell->rachCfg.raOccasion.subFrameNum[size] = idx;
            cell->rachCfg.raOccasion.size++;
            break;
         }
         count ++;
      }
   }
   RETVOID;
}

/**
 * @brief This function initialises the PRACH occasions
 *
 * @details
 *
 *     Function: rgSCHUtlPrachCfgInit
 *     Purpose:  This function initialises the PRACH occasions based on
 *               RGR configuration.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgrCellCfg        *cellCfg
 *  @return     Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlPrachCfgInit
(
RgSchCellCb       *cell,
RgrCellCfg        *cellCfg
)
#else
PUBLIC Void rgSCHUtlPrachCfgInit(cell, cellCfg)
RgSchCellCb       *cell;
RgrCellCfg        *cellCfg;
#endif
{
   U8 idx;
   U8 subfrmIdx;
   U8 splFrm;

   TRC2(rgSCHUtlPrachCfgInit)

   /* Update SFN occasions */
   cell->rachCfg.raOccasion.sfnEnum =
                     cellCfg->prachRscInfo.prachInfo[0].sfn;

   cell->rachCfg.raOccasion.size = 0;

   /* Update subframe occasions */
   for(idx = 0; idx < cellCfg->prachRscInfo.numRsc; idx++)
   {
      if(cellCfg->prachRscInfo.prachInfo[idx].freqIdx == 0)
      {
         if(cellCfg->prachRscInfo.prachInfo[idx].halfFrm == 0)
         {
            splFrm = 1;
         }
         else
         {
            splFrm = 6;
         }
         if(cellCfg->prachRscInfo.prachInfo[idx].ulStartSfIdx ==
                                                RGR_TDD_SPL_UL_IDX)
         {
            subfrmIdx = cell->rachCfg.raOccasion.size;
            cell->rachCfg.raOccasion.subFrameNum[subfrmIdx] = splFrm;
            cell->rachCfg.raOccasion.size++;
         }
         else
         {
            rgSCHUtlUpdPrachOcc(cell,
                  &cellCfg->prachRscInfo.prachInfo[idx]);
         }
      }
   }
   RETVOID;
}
#if 0
/**
 * @brief This function performs RGR cell initialization
 *
 * @details
 *
 *     Function: rgSCHUtlRgrCellCfg
 *     Purpose:  This function initialises the cell with RGR configuration
 *               and subframe related initialization.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgrCellCfg        *cellCfg
 *  @param[in]  RgSchErrInfo      *errInfo
 *  @return     Void
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrCellCfg
(
RgSchCellCb       *cell,
RgrCellCfg        *cellCfg,
RgSchErrInfo      *errInfo
)
#else
PUBLIC S16 rgSCHUtlRgrCellCfg(cell, cellCfg, errInfo)
RgSchCellCb       *cell;
RgrCellCfg        *cellCfg;
RgSchErrInfo      *errInfo;
#endif
{
   U8              i;
   U8              sfn=0;
   U8              sfNum = 0;
   RgSchDlSf       *sf;
   CmLteTimingInfo frm;
   U8              ulDlCfgIdx = cellCfg->ulDlCfgIdx;
   U8              maxSubframes = rgSchTddDlNumHarqProcTbl[ulDlCfgIdx] + TFU_DELTA + RG_UL_DELTA;
   U8              maxDlSubframes = rgSchTddNumDlSubfrmTbl[ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
   S16             ret = ROK;
   U8              sfCount;
   TRC2(rgSCHUtlRgrCellCfg);

   cmMemset((U8 *)&frm,0,sizeof(CmLteTimingInfo));

   maxSubframes = RGSCH_CEIL(maxSubframes, maxDlSubframes);
   maxSubframes = maxSubframes * maxDlSubframes;
   cell->numDlSubfrms = maxSubframes;
   cell->ulDlCfgIdx = ulDlCfgIdx;

   /* PRACH Occasions Initialization */
   rgSCHUtlPrachCfgInit(cell, cellCfg);

   rgSCHUtlAllocSBuf(cell->instIdx,
               (Data **)&cell->subFrms, sizeof(RgSchDlSf *) * maxSubframes);
   if (cell->subFrms == NULLP)
   {
      RETVALUE(RFAILED);
   }

   /* Create memory for each frame. */
   for(i = 0; i < maxSubframes; i++)
   {
      while(rgSchTddUlDlSubfrmTbl[ulDlCfgIdx][sfNum] ==
            RG_SCH_TDD_UL_SUBFRAME)
      {
         sfNum = (sfNum+1) % RGSCH_NUM_SUB_FRAMES;
      }

      rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&sf, sizeof(RgSchDlSf));
      if (sf == NULLP)
      {
         break;
      }
      cmMemset((U8 *)sf, 0, sizeof(*sf));
      sf->sfNum = sfNum;
      /* Calculate the number of CCEs per subframe in the cell */
      sf->nCce = rgSCHUtlCalcNCce(cell->bwCfg.dlTotalBw,
            cell->phichCfg.ngEnum,
            cellCfg->cfiCfg.cfi,
            rgSchTddPhichMValTbl[ulDlCfgIdx][sfNum],
            cell->numTxAntPorts,
            cell->isCpDlExtend);

      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* determine the RBG size and no of RBGs for the configured
       * DL BW */
      if (cell->bwCfg.dlTotalBw > 63)
      {
         cell->rbgSize  = 4;
      }
      else if (cell->bwCfg.dlTotalBw > 26)
      {
         cell->rbgSize  = 3;
      }
      else if (cell->bwCfg.dlTotalBw > 10)
      {
         cell->rbgSize  = 2;
      }
      else
      {
         cell->rbgSize  = 1;
      }
      cell->noOfRbgs = RGSCH_CEIL(cell->bwCfg.dlTotalBw, cell->rbgSize);
#ifdef RG_PHASE2_SCHED
      /* calculating Available bandwidth per subframe */
      if (cellCfg->dlfsCfg.isDlFreqSel)
      {
         sf->bw    = cell->bwCfg.dlTotalBw +\
                     (cell->bwCfg.dlTotalBw * RGSCH_DL_BW_PERCNT_HIKE)/100;
      }
      else
#endif /* RG_PHASE2_SCHED */
      {
         sf->bw    = cell->bwCfg.dlTotalBw;
      }
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* rg008.201 - Added support for SPS */
#ifdef LTEMAC_SPS
   /* Mark SPS bandwidth to be occupied */
   sf->bwAlloced = ((cellCfg->dlSpsCfg.maxSpsDlBw +
      cell->rbgSize - 1)/cell->rbgSize) * cell->rbgSize;
   sf->spsAllocdBw = 0;
   sf->type2End = sf->bwAlloced/cell->rbgSize;
#else
   sf->bwAlloced = 0;
   /* MS_FIX for ccpu00123918*/
   sf->type2Start = 0;
#endif /* LTEMAC_SPS */
      /* Initialize the ackNakRepQ here */
#ifdef RG_MAC_MEASGAP
      cmLListInit (&(sf->ackNakRepQ));
#endif
      cell->subFrms[i] = sf;
      sfNum = (sfNum+1) % RGSCH_NUM_SUB_FRAMES;
   }
   if (i != maxSubframes)
   {
      for (; i > 0; i--)
      {
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(cell->instIdx,
               (Data **)(&(cell->subFrms[i-1])), sizeof(RgSchDlSf));
      }
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx,
            (Data **)(&(cell->subFrms)), sizeof(RgSchDlSf *) * maxSubframes);

      RETVALUE(RFAILED);
   }

   if (cell->sc.apis == NULLP)
   {
      cell->sc.apis = &rgSchCmnApis;
   }

   sfNum=0;
   /* Release the subframes and thereby perform the initialization */
   for (i = 0; i < maxSubframes; i++)
   {
      while(rgSchTddUlDlSubfrmTbl[ulDlCfgIdx][sfNum] ==
                                 RG_SCH_TDD_UL_SUBFRAME)
      {
        sfNum = (sfNum+1) % RGSCH_NUM_SUB_FRAMES;
      }

     if((i > 0) && (i%maxDlSubframes == 0))
     {
      sfn++;
     }
      frm.sfn = sfn;
      frm.subframe = sfNum;
      rgSCHUtlDlRlsSubFrm(cell, frm);
     sfNum = (sfNum+1) % RGSCH_NUM_SUB_FRAMES;
   }


   ret = cell->sc.apis->rgSCHRgrCellCfg(cell, cellCfg, errInfo);

   if (ret != ROK)
   {
      for (i = 0; i < maxSubframes; i++)
      {
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(cell->instIdx,
               (Data **)(&(cell->subFrms[i])), sizeof(RgSchDlSf));
      }
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx,
            (Data **)(&(cell->subFrms)), sizeof(RgSchDlSf *) * maxSubframes);
      errInfo->errCause = RGSCHERR_SCH_CFG;
   }

   /* Assign the configured Cfi to the DlSf */
   for (sfCount = 0; sfCount < maxSubframes; sfCount++)
   {
      sf = cell->subFrms[sfCount];
      if(rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx][sf->sfNum] !=
                              RG_SCH_TDD_SPL_SUBFRAME)
      {
         cell->subFrms[sfCount]->pcfichInfo = cellCfg->cfiCfg.cfi;
      }
   }

   RETVALUE(ret);
}

#else

/**
 * @brief This function performs scheduler related cell creation
 *
 * @details
 *
 *     Function: rgSCHUtlRgrCellCfg
 *     Purpose:  This function creates the subframes needed for the
 *               cell. It then peforms init of the scheduler by calling
 *               scheduler specific cell init function.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb          *cell
 *  @param[in]  RgrCellCfg        *cellCfg
 *  @param[in]  RgSchErrInfo         *errInfo
 *  @return     Void
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrCellCfg
(
RgSchCellCb       *cell,
RgrCellCfg        *cellCfg,
RgSchErrInfo      *errInfo
)
#else
PUBLIC S16 rgSCHUtlRgrCellCfg(cell, cellCfg, errInfo)
RgSchCellCb       *cell;
RgrCellCfg        *cellCfg;
RgSchErrInfo      *errInfo;
#endif
{
   U8              i, sfCount;
   RgSchDlSf       *sf;
   CmLteTimingInfo frm;
   S16             ret;
   Inst            inst = cell->instIdx;
   TRC2(rgSCHUtlRgrCellCfg);

   cmMemset((U8 *)&frm,0,sizeof(CmLteTimingInfo));
   /* Calculate the number of CCEs in the cell */
   cell->nCce = rgSCHUtlCalcNCce(cell->bwCfg.dlTotalBw, cell->phichCfg.ngEnum,
         cellCfg->cfiCfg.cfi, cell->numTxAntPorts, cell->isCpDlExtend);
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   /* determine the RBG size and no of RBGs for the configured
    * DL BW */
   if (cell->bwCfg.dlTotalBw > 63)
   {
      cell->rbgSize  = 4;
   }
   else if (cell->bwCfg.dlTotalBw > 26)
   {
      cell->rbgSize  = 3;
   }
   else if (cell->bwCfg.dlTotalBw > 10)
   {
      cell->rbgSize  = 2;
   }
   else
   {
      cell->rbgSize  = 1;
   }
   cell->noOfRbgs = RGSCH_CEIL(cell->bwCfg.dlTotalBw, cell->rbgSize);
   /* Create memory for each frame. */
   /* rg009.201 [ccpu00115187]- Changing loop limit from
      RGSCH_NUM_SUB_FRAMES to RGSCH_NUM_DL_SUBFRAMES */
   for(i = 0; i < RGSCH_NUM_DL_SUBFRAMES; i++)
   {
      rgSCHUtlAllocSBuf(inst, (Data **)&sf, sizeof(RgSchDlSf));
      if (sf == NULLP)
      {
         break;
      }
      cmMemset((U8 *)sf, 0, sizeof(*sf));
      /* rg009.201 [ccpu00115187]- doing MOD operation before
         assigning value of i */
      sf->sfNum = i % RGSCH_NUM_SUB_FRAMES;
      /* calculating Available bandwidth per subframe */
#ifdef RG_PHASE2_SCHED
      if (cellCfg->dlfsCfg.isDlFreqSel)
      {
         sf->bw    = cell->bwCfg.dlTotalBw +\
                     (cell->bwCfg.dlTotalBw * RGSCH_DL_BW_PERCNT_HIKE)/100;
      }
      else
#endif /* RG_PHASE2_SCHED */
      {
         sf->bw    = cell->bwCfg.dlTotalBw;
      }
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* Initialize the ackNakRepQ here */
#ifdef RG_MAC_MEASGAP
      cmLListInit (&(sf->ackNakRepQ));
#endif
      cell->subFrms[i] = sf;
   }
   /* rg009.201 [ccpu00115187]- Changing loop limit from
      RGSCH_NUM_SUB_FRAMES to RGSCH_NUM_DL_SUBFRAMES */
   if (i != RGSCH_NUM_DL_SUBFRAMES)
   {
      for (; i > 0; i--)
      {
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst, (Data **)(&(cell->subFrms[i-1])),
               sizeof(RgSchDlSf));
      }
      RETVALUE(RFAILED);
   }

   if (cell->sc.apis == NULLP)
   {
      cell->sc.apis = &rgSchCmnApis;
   }

   /* Release the subframes and thereby perform the initialization */
   /* rg009.201 [ccpu00115187]- Changing loop limit from
      RGSCH_NUM_SUB_FRAMES to RGSCH_NUM_DL_SUBFRAMES */
   for (i = 0; i < RGSCH_NUM_DL_SUBFRAMES; i++)
   {
      if (i >= RGSCH_NUM_SUB_FRAMES)
      {
         frm.sfn +=1;
      }
      frm.subframe = i % RGSCH_NUM_SUB_FRAMES;
      rgSCHUtlDlRlsSubFrm(cell, frm);
   }

   ret = cell->sc.apis->rgSCHRgrCellCfg(cell, cellCfg, errInfo);
   if (ret != ROK)
   {
      errInfo->errCause = RGSCHERR_SCH_CFG;
      RETVALUE(RFAILED);
   }
   /* Assign the configured Cfi to the DlSf */
   /* rg009.201 [ccpu00115187]- Changing loop limit from
      RGSCH_NUM_SUB_FRAMES to RGSCH_NUM_DL_SUBFRAMES */
   for (sfCount = 0; sfCount < RGSCH_NUM_DL_SUBFRAMES; sfCount++)
   {
      cell->subFrms[sfCount]->pcfichInfo = cellCfg->cfiCfg.cfi;
   }

   RETVALUE(ret);
}
#endif
#endif

/**
 * @brief This function performs the cell reconfiguration at RGR interface
 *
 * @details
 *
 *     Function: rgSCHUtlRgrCellRecfg
 *     Purpose:  This function updates the reconfigurable parameters
 *               on the cell control block for the scheduler.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb          *cell
 *  @param[in]  RgrCellCfg        *cellCfg
 *  @param[in]  RgSchErrInfo         *errInfo
 *  @return     Void
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrCellRecfg
(
RgSchCellCb       *cell,
RgrCellRecfg      *recfg,
RgSchErrInfo      *err
)
#else
PUBLIC S16 rgSCHUtlRgrCellRecfg(cell, recfg, err)
RgSchCellCb       *cell;
RgrCellRecfg      *recfg;
RgSchErrInfo      *err;
#endif
{
   TRC2(rgSCHUtlRgrCellRecfg);
   RETVALUE(cell->sc.apis->rgSCHRgrCellRecfg(cell, recfg, err));
}



/**
 * @brief This function returns the Y value of UE for a sub frame
 *
 * @details
 *
 *     Function: rgSCHUtlFreeCell
 *     Purpose:  This function updates the value of Y stored in the
 *               UE control block. It uses the previously computed
 *               value for computing for this subframe.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb          *cell
 *  @return     Void
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlFreeCell
(
RgSchCellCb          *cell
)
#else
PUBLIC S16 rgSCHUtlFreeCell(cell)
RgSchCellCb          *cell;
#endif
{
   U8               i;
   CmLListCp        *lst;
   RgSchPdcch       *pdcch;
   RgSchPdcchInfo   *pdcchInfo;
   RgSchPhichInfo   *phichInfo;
   RgSchPhich       *phich;
   Inst             inst = cell->instIdx;
   U8               maxSubframes;
#ifdef LTE_TDD
   RgSchRaReqInfo *raReqInfo;
   U8                idx;
#endif
   TRC2(rgSCHUtlFreeCell);

#ifdef LTE_TDD
   maxSubframes = cell->numDlSubfrms;
#else
   /* rg009.201 [ccpu00115187]- Changing loop limit from
      RGSCH_NUM_SUB_FRAMES to RGSCH_NUM_DL_SUBFRAMES */
   maxSubframes = RGSCH_NUM_DL_SUBFRAMES;
#endif


   /* Invoke the index for scheduler, cell deletion */
   cell->sc.apis->rgSCHFreeCell(cell);

   /* Release the subframes allocated               */
   for (i = 0; i < maxSubframes; i++)
   {
      pdcchInfo = &cell->subFrms[i]->pdcchInfo;
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)(&(pdcchInfo->map)),
            (pdcchInfo->nCce + 7) >> 3);
      while (pdcchInfo->pdcchs.first != NULLP)
      {
         pdcch = (RgSchPdcch *)pdcchInfo->pdcchs.first->node;
         cmLListDelFrm(&pdcchInfo->pdcchs, pdcchInfo->pdcchs.first);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst, (Data **)&pdcch, sizeof(RgSchPdcch));
      }

      phichInfo = &cell->subFrms[i]->phichInfo;
      while(phichInfo->phichs.first != NULLP)
      {
         phich = (RgSchPhich *)phichInfo->phichs.first->node;
         cmLListDelFrm(&phichInfo->phichs, phichInfo->phichs.first);
         RGSCH_PHICH_FREE(inst, phich, sizeof(RgSchPhich));
      }

      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)(&(cell->subFrms[i])), sizeof(RgSchDlSf));
   }
#ifdef LTE_TDD
   /* Release the subframe pointers */
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(inst,
         (Data **) (&(cell->subFrms)), sizeof(RgSchDlSf *) * maxSubframes);

   for(idx=0; idx < cell->raInfo.lstSize; idx++)
   {
      lst = &cell->raInfo.raReqLst[idx];
      while (lst->first != NULLP)
      {
         raReqInfo = (RgSchRaReqInfo *)lst->first->node;
         cmLListDelFrm(lst, &raReqInfo->raReqLstEnt);
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst,(Data **)&raReqInfo, sizeof(RgSchRaReqInfo));
      }
   }
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(inst,
         (Data **)(&(cell->raInfo.raReqLst)),
            sizeof(CmLListCp) * (cell->raInfo.lstSize));
#endif

   /* Release allocated pdcchs */
   lst = &cell->pdcchLst;
   while (lst->first != NULLP)
   {
      pdcch = (RgSchPdcch *)lst->first->node;
      cmLListDelFrm(lst, &pdcch->lnk);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst,(Data **)&pdcch, sizeof(RgSchPdcch));
   }
   RETVALUE(ROK);
}
#if 0

/**
 * @brief This function adds the UE to scheduler
 *
 * @details
 *
 *     Function: rgSCHUtlRgrUeCfg
 *     Purpose:  This function performs addition of UE to scheduler
 *               1. First, it updates the Y table in the UE
 *               2. Then, it calls the scheduler's handler for UE addition
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb          *cell
 *  @param[in]  RgSchUeCb            *ue
 *  @param[in]  RgrUeCfg             *cfg
 *  @param[in]  RgSchErrInfo            *err
 *  @return     S16
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrUeCfg
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgrUeCfg                   *cfg,
RgSchErrInfo               *err
)
#else
PUBLIC S16 rgSCHUtlRgrUeCfg(cell, ue, cfg, err)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgrUeCfg                   *cfg;
RgSchErrInfo               *err;
#endif
{
   TRC2(rgSCHUtlRgrUeCfg);
   /* This updates the Y table maintained in UE used by */
   /* scheduler  when allocating PDCCH                  */
   rgSCHUtlUpdY(ue->ueId, ue->y);

   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   /* Assign TM 1 as UE's default TM */
   ue->mimoInfo.txMode = RGR_UE_TM_1;
   if (cfg->txMode.pres == PRSNT_NODEF)
   {
      /* DL MU-MIMO not supported */
      if (cfg->txMode.txModeEnum == RGR_UE_TM_5)
      {
         err->errCause = RGSCHERR_SCH_CFG;
         RETVALUE(RFAILED);
      }
      ue->mimoInfo.txMode = cfg->txMode.txModeEnum;
   }
   ue->ul.ulTxAntSel = cfg->ulTxAntSel;
   ue->mimoInfo.cdbkSbstRstrctn = cfg->ueCodeBookRstCfg;
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
   ue->ueCatEnum = cfg->ueCatEnum;
   ue->ul.betaHqOffst = cfg->puschDedCfg.bACKIdx;
   ue->ul.betaCqiOffst = cfg->puschDedCfg.bCQIIdx;
   ue->ul.betaRiOffst = cfg->puschDedCfg.bRIIdx;
#ifdef MSPD
   MSPD_DBG("Beta Ofsset Config: hq(%u) cqi(%u) ri(%u)\n",
   ue->ul.betaHqOffst,ue->ul.betaCqiOffst, ue->ul.betaRiOffst );
#endif
#endif
   /* Call the handler of the scheduler based on cell configuration */
   RETVALUE(cell->sc.apis->rgSCHRgrUeCfg(cell, ue, cfg, err));
}
#endif
/* Start : LTEMAC_2.1_DEV_CFG */


/**
 * @brief This function adds a service to scheduler
 *
 * @details
 *
 *     Function: rgSCHUtlRgrLcCfg
 *     Purpose:  This function performs addition of service to scheduler
 *               The addition is performed for each direction based
 *               the direction field of the configuration
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb          *cell
 *  @param[in]  RgSchUeCb            *ue
 *  @param[in]  RgSchDlLcCb          *dlLc
 *  @param[in]  RgrLchCfg            *cfg
 *  @param[in]  RgSchErrInfo         *err
 *  @return     S16
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrLcCfg
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *dlLc,
RgrLchCfg               *cfg,
RgSchErrInfo               *errInfo
)
#else
PUBLIC S16 rgSCHUtlRgrLcCfg(cell, ue, dlLc, cfg, errInfo)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *dlLc;
RgrLchCfg               *cfg;
RgSchErrInfo               *errInfo;
#endif
{
   TRC2(rgSCHUtlRgrLcCfg);
   RETVALUE(cell->sc.apis->rgSCHRgrLchCfg(cell, ue, dlLc, cfg, errInfo));
}


/**
 * @brief This function modifies a service to scheduler
 *
 * @details
 *
 *     Function: rgSCHUtlRgrLcRecfg
 *     Purpose:  This function performs modification of a service in
 *               scheduler. The modification is performed for each direction
 *               based the direction field of the configuration
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb          *cell
 *  @param[in]  RgSchUeCb            *ue
 *  @param[in]  RgSchDlLcCb          *dlLc
 *  @param[in]  RgrLchRecfg          *recfg
 *  @param[in]  RgSchErrInfo         *err
 *  @return     S16
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrLcRecfg
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *dlLc,
RgrLchRecfg                *recfg,
RgSchErrInfo               *err
)
#else
PUBLIC S16 rgSCHUtlRgrLcRecfg(cell, ue, dlLc, recfg, err)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *dlLc;
RgrLchRecfg                *recfg;
RgSchErrInfo               *err;
#endif
{
   TRC2(rgSCHUtlRgrLcRecfg);
   RETVALUE(cell->sc.apis->rgSCHRgrLchRecfg(cell, ue, dlLc, recfg, err));
}

/**
 * @brief This function adds a service to scheduler
 *
 * @details
 *
 *     Function: rgSCHUtlRgrLcgCfg
 *     Purpose:  This function performs addition of service to scheduler
 *               The addition is performed for each direction based
 *               the direction field of the configuration
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb          *cell
 *  @param[in]  RgSchUeCb            *ue
 *  @param[in]  RgrLchCfg            *cfg
 *  @param[in]  RgSchErrInfo         *err
 *  @return     S16
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrLcgCfg
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgrLcgCfg               *cfg,
RgSchErrInfo               *errInfo
)
#else
PUBLIC S16 rgSCHUtlRgrLcgCfg(cell, ue, cfg, errInfo)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgrLcgCfg               *cfg;
RgSchErrInfo               *errInfo;
#endif
{
   TRC2(rgSCHUtlRgrLcgCfg);
   RETVALUE(cell->sc.apis->rgSCHRgrLcgCfg(cell, ue, &(ue->ul.lcgArr[cfg->ulInfo.lcgId]), cfg, errInfo));
}


/**
 * @brief This function modifies a service to scheduler
 *
 * @details
 *
 *     Function: rgSCHUtlRgrLcgRecfg
 *     Purpose:  This function performs modification of a service in
 *               scheduler. The modification is performed for each direction
 *               based the direction field of the configuration
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb          *cell
 *  @param[in]  RgSchUeCb            *ue
 *  @param[in]  RgrLcgRecfg          *recfg
 *  @param[in]  RgSchErrInfo         *err
 *  @return     S16
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrLcgRecfg
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgrLcgRecfg                *recfg,
RgSchErrInfo               *err
)
#else
PUBLIC S16 rgSCHUtlRgrLcgRecfg(cell, ue, recfg, err)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgrLcgRecfg                *recfg;
RgSchErrInfo               *err;
#endif
{
   TRC2(rgSCHUtlRgrLcgRecfg);
   RETVALUE(cell->sc.apis->rgSCHRgrLcgRecfg(cell, ue, &(ue->ul.lcgArr[recfg->ulRecfg.lcgId]), recfg, err));
} /* rgSCHUtlRgrLcRecfg */

/**
 * @brief This function modifies a service to scheduler
 *
 * @details
 *
 *     Function: rgSCHUtlRgrLcgDel
 *     Purpose:  This function performs modification of a service in
 *               scheduler. The modification is performed for each direction
 *               based the direction field of the configuration
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue
 *  @param[in]  RgrDel        *lcDelInfo
 *  @return     S16
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlRgrLcgDel
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
U8                         lcgId
)
#else
PUBLIC Void rgSCHUtlRgrLcgDel(cell, ue, lcgId)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
U8                         lcgId;
#endif
{
   TRC2(rgSCHUtlRgrLcgDel);
   cell->sc.apis->rgSCHFreeLcg(cell, ue, &ue->ul.lcgArr[lcgId]);

  /* Stack Crash problem for TRACE5 changes. added the return below . */
 RETVOID;

} /* rgSCHUtlRgrLcgDel */


/* End: LTEMAC_2.1_DEV_CFG */

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHUtlDoaInd
 *     Purpose:  Updates the DOA for the UE
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  TfuDoaRpt          *doaRpt
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void  rgSCHUtlDoaInd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuDoaRpt          *doaRpt
)
#else
PUBLIC Void rgSCHUtlDoaInd(cell, ue, doaRpt)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
TfuDoaRpt          *doaRpt;
#endif
{
   TRC2(rgSCHUtlDoaInd);
   ue->mimoInfo.doa.pres = PRSNT_NODEF;
   ue->mimoInfo.doa.val = doaRpt->doa;
   RETVOID;
}

/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHUtlDlCqiInd
 *     Purpose:  Updates the DL CQI for the UE
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  TfuDlCqiRpt        *dlCqiRpt
 *  @param[in]  CmLteTimingInfo    timingInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlCqiInd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuDlCqiRpt        *dlCqiRpt,
CmLteTimingInfo    timingInfo
)
#else
PUBLIC Void rgSCHUtlDlCqiInd(cell, ue, dlCqiRpt, timingInfo)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
TfuDlCqiRpt        *dlCqiRpt;
CmLteTimingInfo    timingInfo;
#endif
{
   TRC2(rgSCHUtlDlCqiInd);
   cell->sc.apis->rgSCHDlCqiInd(cell, ue, dlCqiRpt, timingInfo);
   RETVOID;
}


#ifdef TFU_UPGRADE
/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHUtlSrsInd
 *     Purpose:  Updates the UL SRS for the UE
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  TfuSrsRpt*     srsRpt
 *  @param[in]  CmLteTimingInfo    timingInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlSrsInd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuSrsRpt          *srsRpt,
CmLteTimingInfo    timingInfo
)
#else
PUBLIC Void rgSCHUtlSrsInd(cell, ue, srsRpt, timingInfo)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
TfuSrsRpt        *srsRpt;
CmLteTimingInfo    timingInfo;
#endif
{
   TRC2(rgSCHUtlSrsInd);
   cell->sc.apis->rgSCHSrsInd(cell, ue, srsRpt, timingInfo);
   RETVOID;
}
#endif

/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHUtlDlTARpt
 *     Purpose:  Reports PHY TA for a UE.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlTARpt
(
RgSchCellCb        *cell,
RgSchUeCb          *ue
)
#else
PUBLIC Void rgSCHUtlDlTARpt(cell, ue)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
#endif
{
   TRC2(rgSCHUtlDlTARpt);
   cell->sc.apis->rgSCHDlTARpt(cell, ue);
   RETVOID;
}


/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHUtlDlRlsSubFrm
 *     Purpose:  Releases scheduler Information from DL SubFrm.
 *
 *     Invoked by: DHM
 *
 *  @param[in]   RgSchCellCb     *cell
 *  @param[out]  CmLteTimingInfo subFrm
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlRlsSubFrm
(
RgSchCellCb        *cell,
CmLteTimingInfo    subFrm
)
#else
PUBLIC Void rgSCHUtlDlRlsSubFrm(cell, subFrm)
RgSchCellCb        *cell;
CmLteTimingInfo    subFrm;
#endif
{
   TRC2(rgSCHUtlDlRlsSubFrm);
   cell->sc.apis->rgSCHDlRlsSubFrm(cell, subFrm);
   RETVOID;
}

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */

/**
 * @brief This API is invoked to indicate scheduler of a CRC indication.
 *
 * @details
 *
 *     Function : rgSCHUtlHdlUlTransInd
 *      This API is invoked to indicate scheduler of a CRC indication.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  CmLteTimingInfo timingInfo
 *
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlHdlUlTransInd
(
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo timingInfo
)
#else
PUBLIC Void rgSCHUtlHdlUlTransInd(cell, ue, timingInfo)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
CmLteTimingInfo timingInfo;
#endif
{
   TRC2(rgSCHUtlHdlUlTransInd);
   cell->sc.apis->rgSCHHdlUlTransInd(cell, ue, timingInfo);
   RETVOID;
}
/*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
/**
 * @brief This API is invoked to indicate scheduler of a CRC failure.
 *
 * @details
 *
 *     Function : rgSCHUtlHdlCrcInd
 *      This API is invoked to indicate CRC  to scheduler.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  CmLteTimingInfo timingInfo
 *
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlHdlCrcInd
(
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo timingInfo
)
#else
PUBLIC Void rgSCHUtlHdlCrcInd(cell, ue, timingInfo)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
CmLteTimingInfo timingInfo;
#endif
{
   TRC2(rgSCHUtlHdlCrcFail);
   cell->sc.apis->rgSCHUlCrcInd(cell, ue, timingInfo);
   RETVOID;
} /* end of rgSCHUtlHdlCrcFailInd */

/**
 * @brief This API is invoked to indicate scheduler of a CRC failure.
 *
 * @details
 *
 *     Function : rgSCHUtlHdlCrcFailInd
 *      This API is invoked to indicate CRC failure to scheduler.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  CmLteTimingInfo timingInfo
 *
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlHdlCrcFailInd
(
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo timingInfo
)
#else
PUBLIC Void rgSCHUtlHdlCrcFailInd(cell, ue, timingInfo)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
CmLteTimingInfo timingInfo;
#endif
{
   TRC2(rgSCHUtlHdlCrcFail);
   cell->sc.apis->rgSCHUlCrcFailInd(cell, ue, timingInfo);
   RETVOID;
} /* end of rgSCHUtlHdlCrcFailInd */
#endif /* LTEMAC_SPS */


/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHUtlDlProcAddToRetx
 *     Purpose:  This function adds a HARQ process to retransmission
 *               queue. This may be performed when a HARQ ack is
 *               unsuccessful.
 *
 *     Invoked by: HARQ feedback processing
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchDlHqProc*   hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlProcAddToRetx
(
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
)
#else
PUBLIC Void rgSCHUtlDlProcAddToRetx(cell, hqP)
RgSchCellCb                *cell;
RgSchDlHqProcCb            *hqP;
#endif
{
   TRC2(rgSCHUtlDlProcAddToRetx);
   cell->sc.apis->rgSCHDlProcAddToRetx(cell, hqP);
   RETVOID;
}

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */


/**
 * @brief This function adds a HARQ process TB to transmission
 *
 * @details
 *
 *     Function: rgSCHUtlDlHqPTbAddToTx
 *     Purpose:  This function a HarqProcess TB to the subframe
 *               list.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSubFrm*     subFrm
 *  @param[in]  RgDlHqProc*   hqP
 *  @param[in]  U8            tbIdx
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlHqPTbAddToTx
(
RgSchDlSf                  *subFrm,
RgSchDlHqProcCb            *hqP,
U8                         tbIdx
)
#else
PUBLIC Void rgSCHUtlDlHqPTbAddToTx(subFrm, hqP, tbIdx)
RgSchDlSf                  *subFrm;
RgSchDlHqProcCb            *hqP;
U8                         tbIdx;
#endif
{
   cmLListAdd2Tail(&subFrm->tbs, &hqP->tbInfo[tbIdx].sfLnk);
   hqP->tbInfo[tbIdx].sfLnk.node  = (PTR)&hqP->tbInfo[tbIdx];
   /*MS_FIX*/
   hqP->tbInfo[tbIdx].subFrm = subFrm;
}

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */


/**
 * @brief This function removes a HARQ process TB from transmission
 *
 * @details
 *
 *     Function: rgSCHUtlDlHqPTbRmvFrmTx
 *     Purpose:  This function removes a HarqProcess TB to the subframe
 *               list.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSubFrm*     subFrm
 *  @param[in]  RgDlHqProc*   hqP
 *  @param[in]  U8            tbIdx
 *  @param[in]  Bool          isRepeting
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlHqPTbRmvFrmTx
(
RgSchDlSf                  *subFrm,
RgSchDlHqProcCb            *hqP,
U8                         tbIdx,
Bool                       isRepeting
)
#else
PUBLIC Void rgSCHUtlDlHqPTbRmvFrmTx(subFrm, hqP, tbIdx, isRepeting)
RgSchDlSf                  *subFrm;
RgSchDlHqProcCb            *hqP;
U8                         tbIdx;
Bool                       isRepeting;
#endif
{
   /* Check with TDD */
   if ((isRepeting) &&
         (hqP->hqE->ue->ackNakRepCb.cfgRepCnt !=
          hqP->tbInfo[tbIdx].fbkRepCntr))
   {
      cmLListDelFrm(&subFrm->ackNakRepQ,
         &hqP->tbInfo[tbIdx].anRepLnk[hqP->tbInfo[tbIdx].fbkRepCntr]);
   }
   else
   {
      cmLListDelFrm(&subFrm->tbs, &hqP->tbInfo[tbIdx].sfLnk);
      hqP->tbInfo[tbIdx].sfLnk.node  = (PTR)NULLP;
      /* MS_FIX */
      hqP->tbInfo[tbIdx].subFrm = NULLP;
   }
}

/**
 * @brief UE reconfiguration for scheduler
 *
 * @details
 *
 *     Function : rgSCHUtlRgrUeRecfg
 *
 *     This functions updates UE specific scheduler
 *     information upon UE reconfiguration
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[int] RgrUeRecfg   *ueRecfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrUeRecfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHUtlRgrUeRecfg(cell, ue, ueRecfg, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgrUeRecfg   *ueRecfg;
RgSchErrInfo *err;
#endif
{
/* rg009.201 - Changes for UE Category Reconfiguration feature addition */
#ifdef LTEMAC_MIMO
   RgSchCmnUe    *ueSch = RG_SCH_CMN_GET_UE(ue);
#endif

   TRC2(rgSCHUtlRgrUeRecfg);

/* rg007.201 - Changes for MIMO feature addition */
#ifdef LTEMAC_MIMO

   /* DL MU-MIMO not supported */
   if (ueRecfg->ueRecfgTypes & RGR_UE_TXMODE_RECFG)
   {

      if (ueRecfg->txMode.pres == PRSNT_NODEF)
      {
         if (ueRecfg->txMode.txModeEnum == RGR_UE_TM_5)
         {
            err->errCause = RGSCHERR_SCH_CFG;
            RETVALUE(RFAILED);
         }
         ue->mimoInfo.txMode = ueRecfg->txMode.txModeEnum;
      }
   }
/* rg009.201 - Changes for UE Category Reconfiguration feature addition */
   if (ueRecfg->ueRecfgTypes & RGR_UE_UECAT_RECFG)
   {
      ueSch->cmn.ueCat = ueRecfg->ueCatEnum-1;
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
      ue->ueCatEnum = ueRecfg->ueCatEnum;
      /* MS_FIX: ccpu00124012 */
      ue->ul.betaHqOffst = ueRecfg->puschDedCfg.bACKIdx;
      ue->ul.betaCqiOffst = ueRecfg->puschDedCfg.bCQIIdx;
      ue->ul.betaRiOffst = ueRecfg->puschDedCfg.bRIIdx;
#endif
   }
   if (ueRecfg->ueRecfgTypes & RGR_UE_ULTXANTSEL_RECFG)
   {
      ue->ul.ulTxAntSel = ueRecfg->ulTxAntSel;
   }
   if (ueRecfg->ueRecfgTypes & RGR_UE_CDBKSBST_RECFG)
   {
      ue->mimoInfo.cdbkSbstRstrctn = ueRecfg->ueCodeBookRstRecfg;
   }
#endif

   RETVALUE(cell->sc.apis->rgSCHRgrUeRecfg(cell, ue, ueRecfg, err));
}  /* rgSCHUtlRgrUeRecfg */

/**
 * @brief This function deletes a service from scheduler
 *
 * @details
 *
 *     Function: rgSCHUtlFreeDlLc
 *     Purpose:  This function is made available through a FP for
 *               making scheduler aware of a service being deleted from UE
 *
 *     Invoked by: BO and Scheduler
 *
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlFreeDlLc
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
PUBLIC Void rgSCHUtlFreeDlLc(cell, ue, svc)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{
   TRC2(rgSCHUtlFreeDlLc);
   cell->sc.apis->rgSCHFreeDlLc(cell, ue, svc);

  /* Stack Crash problem for TRACE5 changes. added the return below . */
 RETVOID;

}

/**
 * @brief UE deletion for scheduler
 *
 * @details
 *
 *     Function : rgSCHUtlFreeUe
 *
 *     This functions deletes all scheduler information
 *     pertaining to a UE
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlFreeUe
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PUBLIC Void rgSCHUtlFreeUe(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   TRC2(rgSCHUtlFreeUe);
#ifdef LTE_TDD
   rgSCHUtlDelUeANFdbkInfo(ue);
#endif
   cell->sc.apis->rgSCHFreeUe(cell, ue);

  /* Stack Crash problem for TRACE5 changes. added the return below . */
 RETVOID;

}  /* rgSCHUtlFreeUe */

/**
 * @brief Scheduler invocation per TTI
 *
 * @details
 *
 *     Function: rgSCHUtlTti
 *     Purpose:  This function implements a scheduler for LTE. This is
 *               made available as a function pointer to be called
 *               at the time of TTI processing by the MAC.
 *
 *     Invoked by: TOM (TTI processing)
 *
 *  @param[in]  RgSchCellCb *cell
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlTti
(
RgSchCellCb  *cell,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHUtlTti(cell, err)
RgSchCellCb  *cell;
RgSchErrInfo *err;
#endif
{
   TRC2(rgSCHUtlTti);
   RETVALUE(cell->sc.apis->rgSCHTti(cell, err));
}

/**
 * @brief This function updates the scheduler with service for a UE
 *
 * @details
 *
 *     Function: rgSCHUtlDlDedBoUpd
 *     Purpose:  This function should be called whenever there is a
 *               change BO for a service.
 *
 *     Invoked by: BO and Scheduler
 *
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDlDedBoUpd
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *lc
)
#else
PUBLIC Void rgSCHUtlDlDedBoUpd(cell, ue, lc)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *lc;
#endif
{
   TRC2(rgSCHUtlDlDedBoUpd);
   cell->sc.apis->rgSCHDlDedBoUpd(cell, ue, lc);
   RETVOID;
}
/**
 * rg004.201: ccpu00105914: PHR handling for MSG3
 * @brief Record MSG3 allocation into the UE
 *
 * @details
 *
 *     Function : rgSCHUtlRecMsg3Alloc
 *
 *     This function is invoked to update record msg3 allocation information
 *    in the UE when UE is detected for RaCb
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in] RgSchRaCb       *raCb
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlRecMsg3Alloc
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchRaCb        *raCb
)
#else
PUBLIC Void rgSCHUtlRecMsg3Alloc(cell, ue, raCb)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchRaCb        *raCb;
#endif
{
   TRC2(rgSCHUtlRecMsg3Alloc)
   cell->sc.apis->rgSCHUlRecMsg3Alloc(cell, ue, raCb);
   RETVOID;

}  /* rgSCHRecMsg3Alloc */

#ifdef RG_UNUSED
/**
 * @brief Update harq process for allocation
 *
 * @details
 *
 *     Function : rgSCHUtlUpdUlHqProc
 *
 *     This function is invoked when harq process
 *     control block is now in a new memory location
 *     thus requiring a pointer/reference update.
 *
 *  @param[in] RgSchCellCb      *cell
 *  @param[in] RgSchUlHqProcCb  *curProc
 *  @param[in] RgSchUlHqProcCb  *oldProc
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUpdUlHqProc
(
RgSchCellCb      *cell,
RgSchUlHqProcCb  *curProc,
RgSchUlHqProcCb  *oldProc
)
#else
PUBLIC S16 rgSCHUtlUpdUlHqProc(cell, curProc, oldProc)
RgSchCellCb      *cell;
RgSchUlHqProcCb  *curProc;
RgSchUlHqProcCb  *oldProc;
#endif
{
   TRC2(rgSCHUtlUpdUlHqProc);
   RETVALUE(cell->sc.apis->rgSCHUpdUlHqProc(cell, curProc, oldProc));
}  /* rgSCHUtlUpdUlHqProc */
#endif
/**
 * @brief UL grant for contention resolution
 *
 * @details
 *
 *     Function : rgSCHUtlContResUlGrant
 *
 *     Add UE to another queue specifically for CRNTI based contention
 *     resolution
 *
 *  @param[in]  RgSchCellCb *cell
 *  @param[in]  RgSchUeCb  *ue
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlContResUlGrant
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHUtlContResUlGrant(cell, ue, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchErrInfo *err;
#endif
{
   TRC2(rgSCHUtlContResUlGrant);
   RETVALUE(cell->sc.apis->rgSCHContResUlGrant(cell, ue, err));
}  /* rgSCHUtlContResUlGrant */

/**
 * @brief SR reception handling
 *
 * @details
 *
 *     Function : rgSCHUtlSrRcvd
 *
 *     - Handles SR reception for UE
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlSrRcvd
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
CmLteTimingInfo frm,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHUtlSrRcvd(cell, ue, frm, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
CmLteTimingInfo frm;
RgSchErrInfo *err;
#endif
{
   TRC2(rgSCHUtlSrRcvd);
   RETVALUE(cell->sc.apis->rgSCHSrRcvd(cell, ue, frm, err));
}  /* rgSCHUtlSrRcvd */

/**
 * @brief Short BSR update
 *
 * @details
 *
 *     Function : rgSCHUtlUpdBsrShort
 *
 *     This functions does requisite updates to handle short BSR reporting
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  U8           lcgId
 *  @param[in]  U8           bsr
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUpdBsrShort
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
U8           lcgId,
U8           bsr,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHUtlUpdBsrShort(cell, ue, lcgId, bsr, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
U8           lcgId;
U8           bsr;
RgSchErrInfo *err;
#endif
{
   TRC2(rgSCHUtlUpdBsrShort);
   RETVALUE(cell->sc.apis->rgSCHUpdBsrShort(cell, ue, &ue->ul.lcgArr[lcgId], bsr, err));
}  /* rgSCHUtlUpdBsrShort */


/**
 * @brief Truncated BSR update
 *
 * @details
 *
 *     Function : rgSCHUtlUpdBsrTrunc
 *
 *     This functions does required updates to handle truncated BSR report
 *
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  U8           lcgId
 *  @param[in]  U8           bsr
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUpdBsrTrunc
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
U8           lcgId,
U8           bsr,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHUtlUpdBsrTrunc(cell, ue, lcgId, bsr, err)
RgSchCellCb   *cell;
RgSchUeCb     *ue;
U8            lcgId;
U8            bsr;
RgSchErrInfo *err;
#endif
{
   TRC2(rgSCHUtlUpdBsrTrunc);
   RETVALUE(cell->sc.apis->rgSCHUpdBsrTrunc(cell, ue, &ue->ul.lcgArr[lcgId], bsr, err));
}  /* rgSCHUtlUpdBsrTrunc */


/**
 * @brief Long BSR update
 *
 * @details
 *
 *     Function : rgSCHUtlUpdBsrLong
 *
 *     - Update BSRs for all configured LCGs
 *     - Update priority of LCGs if needed
 *     - Update UE's position within/across uplink scheduling queues
 *
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue
 *  @param[in]  U8            bsr0
 *  @param[in]  U8            bsr1
 *  @param[in]  U8            bsr2
 *  @param[in]  U8            bsr3
 *  @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUpdBsrLong
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
U8           bsr0,
U8           bsr1,
U8           bsr2,
U8           bsr3,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHUtlUpdBsrLong(cell, ue, bsr0, bsr1, bsr2, bsr3, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
U8           bsr0;
U8           bsr1;
U8           bsr2;
U8           bsr3;
RgSchErrInfo *err;
#endif
{
   U8 bsArr[4];
   TRC2(rgSCHUtlUpdBsrLong);

   bsArr[0] = bsr0;
   bsArr[1] = bsr1;
   bsArr[2] = bsr2;
   bsArr[3] = bsr3;
   RETVALUE(cell->sc.apis->rgSCHUpdBsrLong(cell, ue, bsArr, err));
}  /* rgSCHUtlUpdBsrLong */

/**
 * @brief PHR update
 *
 * @details
 *
 *     Function : rgSCHUtlUpdPhr
 *
 *     Updates power headroom info for a UE
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  U8           phr
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUpdPhr
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
U8           phr,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHUtlUpdPhr(cell, ue, phr, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
U8           phr;
RgSchErrInfo *err;
#endif
{
   TRC2(rgSCHUtlUpdPhr);
   RETVALUE(cell->sc.apis->rgSCHUpdPhr(cell, ue, phr, err));
}  /* rgSCHUtlUpdPhr */


/**
 * @brief Indication of UL CQI
 *
 * @details
 *
 *     Function : rgSCHUtlUlCqiInd
 *
 *     - Updates uplink CQI information for the UE. Computes and
 *       stores the lowest CQI of CQIs reported in all subbands
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  TfuUlCqiRpt         *ulCqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlCqiInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuUlCqiRpt          *ulCqiInfo
)
#else
PUBLIC Void rgSCHUtlUlCqiInd(cell, ue, ulCqiInfo)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
TfuUlCqiRpt          *ulCqiInfo;
#endif
{
   TRC2(rgSCHUtlUlCqiInd);
   cell->sc.apis->rgSCHUlCqiInd(cell, ue, ulCqiInfo);
   RETVOID;
}  /* rgSCHUtlUlCqiInd */

/**
 * @brief Indication of PUCCH power adjustment
 *
 * @details
 *
 *     Function : rgSCHUtlPucchDeltaPwrInd
 *
 *     - Updates uplink CQI information for the UE. Computes and
 *       stores the lowest CQI of CQIs reported in all subbands
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  U8                  delta
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlPucchDeltaPwrInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
S8                   delta
)
#else
PUBLIC Void rgSCHUtlPucchDeltaPwrInd(cell, ue, delta)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
S8                   delta;
#endif
{
   TRC2(rgSCHUtlPucchDeltaPwrInd);
   cell->sc.apis->rgSCHPucchDeltaPwrInd(cell, ue, delta);
   RETVOID;
}  /* rgSCHUtlPucchDeltaPwrInd */

/* Start: LTEMAC_2.1_DEV_CFG */
/**
 * @brief Ue Reset Request
 *
 * @details
 *
 *     Function : rgSCHUtlUeReset
 *
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @return  S16
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlUeReset
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else
PUBLIC Void rgSCHUtlUeReset(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif
{
   TRC2(rgSCHUtlUeReset);
   cell->sc.apis->rgSCHUeReset(cell, ue);
   RETVOID;
}  /* rgSCHUtlUeReset */
/* End: LTEMAC_2.1_DEV_CFG */

/**
 * @brief Returns HARQ proc for which data expected now
 *
 * @details
 *
 *     Function: rgSCHUtlUlHqProcForUe
 *     Purpose:  This function returns the harq process for
 *               which data is expected in the current subframe.
 *               It does not validate if the HARQ process
 *               has an allocation.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  CmLteTimingInfo     frm
 *  @param[in]  RgSchUeCb           *ue
 *  @param[out] RgSchUlHqProcCb     **procRef
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHqProcForUe
(
RgSchCellCb         *cell,
CmLteTimingInfo     frm,
RgSchUeCb           *ue,
RgSchUlHqProcCb     **procRef
)
#else
PUBLIC Void rgSCHUtlUlHqProcForUe(cell, frm, ue, procRef)
RgSchCellCb         *cell;
CmLteTimingInfo     frm;
RgSchUeCb           *ue;
RgSchUlHqProcCb     **procRef;
#endif
{
   TRC2(rgSCHUtlUlHqProcForUe);
   cell->sc.apis->rgSCHUlHqProcForUe(cell, frm, ue, procRef);

 /* Stack Crash problems for TRACE5 changes. added the return below */
 RETVOID;

}

/**
 * @brief Returns first uplink allocation to send reception
 *        request to PHY
 *
 * @details
 *
 *     Function: rgSCHUtlFirstRcptnReq(cell)
 *     Purpose:  This function returns the first uplink allocation
 *               (or NULLP if there is none) in the subframe
 *               in which is expected to prepare and send reception
 *               request to PHY.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlFirstRcptnReq
(
RgSchCellCb      *cell
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlFirstRcptnReq(cell)
RgSchCellCb      *cell;
#endif
{
   TRC2(rgSCHUtlFirstRcptnReq);
   RETVALUE(cell->sc.apis->rgSCHFirstRcptnReq(cell));
}

/**
 * @brief Returns first uplink allocation to send reception
 *        request to PHY
 *
 * @details
 *
 *     Function: rgSCHUtlNextRcptnReq(cell)
 *     Purpose:  This function returns the next uplink allocation
 *               (or NULLP if there is none) in the subframe
 *               in which is expected to prepare and send reception
 *               request to PHY.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlNextRcptnReq
(
RgSchCellCb      *cell,
RgSchUlAlloc     *alloc
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlNextRcptnReq(cell, alloc)
RgSchCellCb      *cell;
RgSchUlAlloc     *alloc;
#endif
{
   TRC2(rgSCHUtlNextRcptnReq);
   RETVALUE(cell->sc.apis->rgSCHNextRcptnReq(cell, alloc));
}

/**
 * @brief Returns first uplink allocation to send HARQ feedback
 *        request to PHY
 *
 * @details
 *
 *     Function: rgSCHUtlFirstHqFdbkAlloc(cell)
 *     Purpose:  This function returns the first uplink allocation
 *               (or NULLP if there is none) in the subframe
 *               in which it is expected to prepare and send HARQ
 *               feedback to PHY.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlFirstHqFdbkAlloc
(
RgSchCellCb      *cell
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlFirstHqFdbkAlloc(cell)
RgSchCellCb      *cell;
#endif
{
   TRC2(rgSCHUtlFirstHqFdbkAlloc);
   RETVALUE(cell->sc.apis->rgSCHFirstHqFdbkAlloc(cell));
}


/**
 * @brief Returns next allocation to send HARQ feedback for
 *
 * @details
 *
 *     Function: rgSCHUtlNextHqFdbkAlloc(cell)
 *     Purpose:  This function returns the next uplink allocation
 *               (or NULLP if there is none) in the subframe
 *               for which HARQ feedback needs to be sent.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlNextHqFdbkAlloc
(
RgSchCellCb      *cell,
RgSchUlAlloc     *alloc
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlNextHqFdbkAlloc(cell, alloc)
RgSchCellCb      *cell;
RgSchUlAlloc     *alloc;
#endif
{
   TRC2(rgSCHUtlNextHqFdbkAlloc);
   RETVALUE(cell->sc.apis->rgSCHNextHqFdbkAlloc(cell, alloc));
}
#endif
/***********************************
***********************************/
/**
 * @brief This API is invoked to send TFU SAP bind request to PHY.
 *
 * @details
 *
 *     Function : rgSCHUtlTfuBndReq
 *
 *      This API is invoked to send TFU SAP bind request to PHY from scheduler
 *      isntance. It fills in the Pst structure, spId and suId values and
 *      invokes bind request primitive at TFU.
 *
 *  @param[in]  Inst            instId
 *  @param[in]  SuId            suId
 *  @param[in]  SpId            spId
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlTfuBndReq
(
Inst    instId,
SuId    suId,
SpId    spId
)
#else
PUBLIC S16 rgSCHUtlTfuBndReq(instId, suId, spId)
Inst    instId;
SuId    suId;
SpId    spId;
#endif
{
   S16            ret;
   RgSchLowSapCb  *tfuSap;
   Pst            pst;
   Inst           inst;

   TRC2(rgSCHUtlTfuBndReq);

   /* Get the lower SAP control block from the layer control block. */
   tfuSap = &(rgSchCb[instId].tfuSap[suId]);
   (Void)cmMemcpy ((U8*)&pst, (U8*)&(tfuSap->sapCfg.sapPst), sizeof(Pst));
   inst = pst.srcInst;
   if((ret = RgLiTfuSchBndReq (&pst, suId, spId)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"rgSCHUtlTfuBndReq() Call to RgLiTfuBndReq()"
                    " failed\n"));
   }
   RETVALUE(ret);
}  /* rgSCHUtlTfuBndReq */

/**
 * @brief This API is invoked to send TFU SAP unbind request to PHY.
 *
 * @details
 *
 *     Function : rgSCHUtlTfuUBndReq
 *
 *      This API is invoked to send TFU SAP unbind request to PHY from Scheduler
 *      isntance. It fills in the Pst structure and spId value and invokes
 *      unbind request primitive at TFU.
 *
 *  @param[in]  SpId            spId
 *  @param[in]  Reason          reason
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlTfuUBndReq
(
Inst    inst,
RgSchLowSapCfgInfo sapCfg,
Reason  reason
)
#else
PUBLIC S16 rgSCHUtlTfuUBndReq(inst, sapCfg, reason)
Inst    inst;
RgSchLowSapCfgInfo sapCfg;
Reason  reason;
#endif
{
   S16            ret;
   Pst            pst;

   TRC2(rgSCHUtlTfuUBndReq);

   /* Get the lower SAP control block from the layer control block. */
   cmMemcpy ((U8*)&pst, (U8*)&(sapCfg.sapPst), sizeof(Pst));
   if((ret = RgLiTfuSchUbndReq (&pst, sapCfg.spId, reason)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"rgSCHUtlTfuUBndReq() Call to"
                                    " RgLiTfuUbndReq() failed\n"));
   }
   RETVALUE(ret);

}  /* rgSCHUtlTfuUBndReq */

/***********************************************************
 *
 *     Func : rgSCHUtlResetSfAlloc
 *
 *     Desc : Utility Function to Reset subframe allocation information.
 *
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlResetSfAlloc
(
RgInfSfAlloc *sfAlloc,
Bool         resetCmnLcInfo,
Bool         restAlloc
)
#else
PUBLIC S16 rgSCHUtlResetSfAlloc(sfAlloc,resetCmnLcInfo,restAlloc)
RgInfSfAlloc *sfAlloc;
Bool         resetCmnLcInfo;
Bool         restAlloc;
#endif
{
   TRC2(rgSCHUtlResetSfAlloc);
   if(TRUE == restAlloc)
   {
      if(sfAlloc->ueInfo.numUes)
      {
         cmMemset((U8 *)sfAlloc->ueInfo.allocInfo,0x00,
               (sizeof(RgInfUeAlloc)*RGSCH_MAX_UE_PER_DL_SF));
      }
      sfAlloc->ueInfo.numUes = 0;
      sfAlloc->rarInfo.numRaRntis = 0;
   }
   if(TRUE == resetCmnLcInfo)
   {
      sfAlloc->cmnLcInfo.bitMask = 0;
   }
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHUtlGetRlsHqAlloc
 *
 *     Desc : Utility Function to Allocate subframe allocation information.
 *
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlGetRlsHqAlloc
(
RgSchCellCb *cell
)
#else
PUBLIC S16 rgSCHUtlGetRlsHqAlloc(cell)
RgSchCellCb *cell;
#endif
{
   U8    idx = 0;
   Inst  inst = cell->instIdx;
   TRC2(rgSCHUtlGetRlsHqAlloc);

#ifndef LTE_TDD
   for(idx=0; idx < RGSCH_NUM_SUB_FRAMES; idx++)
#else
   for(idx=0; idx < 2*RGSCH_NUM_SUB_FRAMES; idx++)
#endif
   {
      cell->rlsHqArr[idx].cellId = cell->cellId;

      /* Allocating with additional location, to accommodate
         TA scheduling along with maximum no of UEs per SF */

      /* Allocate memory for "scheduled UE" Info */
      if((rgSCHUtlAllocSBuf(inst,
                     (Data**)&(cell->rlsHqArr[idx].ueHqInfo),
                      (sizeof(RgInfUeHqInfo)*RGSCH_MAX_UE_PER_DL_SF))) != ROK)
      {
         RGSCHDBGERR(inst,(rgSchPBuf(inst),"Memory allocation FAILED for UE Alloc"));
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);

}

/***********************************************************
 *
 *     Func : rgSCHUtlPutRlsHqAlloc
 *
 *     Desc : Utility Function to deallocate subframe allocation information.
 *
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlPutRlsHqAlloc
(
RgSchCellCb *cell
)
#else
PUBLIC S16 rgSCHUtlPutRlsHqAlloc(cell)
RgSchCellCb *cell;
#endif
{
   U8    idx = 0;
   Inst  inst = cell->instIdx;
   TRC2(rgSCHUtlPutRlsHqAlloc);

#ifndef LTE_TDD
   for(idx=0; idx < RGSCH_NUM_SUB_FRAMES; idx++)
#else
   for(idx=0; idx < 2*RGSCH_NUM_SUB_FRAMES; idx++)
#endif
   {
      /* Deallocate memory for "scheduled UE" Info */
      if (cell->rlsHqArr[idx].ueHqInfo != NULLP)
      {
      /* Freeing with additional location, to accommodate TA
         scheduling along with maximum no of UEs per SF */
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst,
               (Data **)(&(cell->rlsHqArr[idx].ueHqInfo)),
               (sizeof(RgInfUeHqInfo)*RGSCH_MAX_UE_PER_DL_SF));
      }
   }

   RETVALUE(ROK);

}


/***********************************************************
 *
 *     Func : rgSCHUtlGetSfAlloc
 *
 *     Desc : Utility Function to Allocate subframe allocation information.
 *
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlGetSfAlloc
(
RgSchCellCb *cell
)
#else
PUBLIC S16 rgSCHUtlGetSfAlloc(cell)
RgSchCellCb *cell;
#endif
{
   U8    idx;
   U8    indx;
   Inst  inst = cell->instIdx;
   TRC2(rgSCHUtlGetSfAlloc);

#ifdef LTE_TDD
   for(idx=0; idx < 2*RGSCH_NUM_SUB_FRAMES; idx++)
#else
   for(idx=0; idx < RGSCH_NUM_SUB_FRAMES; idx++)
#endif
   {
      cell->sfAllocArr[idx].cellId = cell->cellId;

      /* Allocating with additional location, to accommodate
         TA scheduling along with maximum no of UEs per SF */

      /* Allocate memory for "scheduled UE" Info */
      if((rgSCHUtlAllocSBuf(inst,
                     (Data**)&(cell->sfAllocArr[idx].ueInfo.allocInfo),
                      (sizeof(RgInfUeAlloc)*RGSCH_MAX_UE_PER_DL_SF))) != ROK)
      {
         RGSCHDBGERR(inst,(rgSchPBuf(inst),"Memory allocation FAILED for UE Alloc"));
         RETVALUE(RFAILED);
      }

      /* Allocate memory for "scheduled RAR" Info */
      if((rgSCHUtlAllocSBuf(inst,
                     (Data**)&(cell->sfAllocArr[idx].rarInfo.raRntiInfo),
                      (sizeof(RgInfRaRntiInfo)*RGSCH_MAX_RARNTI_PER_DL_SF))) != ROK)
      {
         RGSCHDBGERR(inst,(rgSchPBuf(inst),"Memory allocation FAILED for RARNTI"));
         RETVALUE(RFAILED);
      }
      for(indx = 0; indx < RGSCH_MAX_RARNTI_PER_DL_SF; indx++)
      {
         if((rgSCHUtlAllocSBuf(inst,
            (Data**)&(cell->sfAllocArr[idx].rarInfo.raRntiInfo[indx].crntiInfo),
                   (sizeof(RgInfCrntiInfo)*RGSCH_MAX_RNTI_PER_RARNTI))) != ROK)
         {
            RGSCHDBGERR(inst, (rgSchPBuf(inst), "Memory allocation FAILED for RNTI"));
            RETVALUE(RFAILED);
         }
      }

   }

   RETVALUE(ROK);

}

/***********************************************************
 *
 *     Func : rgSCHUtlPutSfAlloc
 *
 *     Desc : Utility Function to deallocate subframe allocation information.
 *
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlPutSfAlloc
(
RgSchCellCb *cell
)
#else
PUBLIC S16 rgSCHUtlPutSfAlloc(cell)
RgSchCellCb *cell;
#endif
{
   U8    idx;
   U8    indx;
   Inst  inst = cell->instIdx;
   TRC2(rgSCHUtlPutSfAlloc);

#ifdef LTE_TDD
   for(idx=0; idx < 2*RGSCH_NUM_SUB_FRAMES; idx++)
#else
   for(idx=0; idx < RGSCH_NUM_SUB_FRAMES; idx++)
#endif
   {
      if (cell->sfAllocArr[idx].rarInfo.raRntiInfo != NULLP)
      {
         for(indx = 0; indx < RGSCH_MAX_RARNTI_PER_DL_SF; indx++)
         {
            if (cell->sfAllocArr[idx].rarInfo.raRntiInfo[indx].crntiInfo != NULLP)
               /* ccpu00117052 - MOD - Passing double pointer
               for proper NULLP assignment*/
               rgSCHUtlFreeSBuf(inst,
                     (Data**)(&(cell->sfAllocArr[idx].rarInfo.raRntiInfo[indx].\
                                crntiInfo)),
                     (sizeof(RgInfCrntiInfo)*RGSCH_MAX_RNTI_PER_RARNTI));
         }
         /* Deallocate memory for "scheduled RAR" Info */
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst,
               (Data**)(&(cell->sfAllocArr[idx].rarInfo.raRntiInfo)),
               (sizeof(RgInfRaRntiInfo)*RGSCH_MAX_RARNTI_PER_DL_SF));
      }
      /* Deallocate memory for "scheduled UE" Info */
      if (cell->sfAllocArr[idx].ueInfo.allocInfo != NULLP)
      {
      /* Freeing with additional location, to accommodate TA
         scheduling along with maximum no of UEs per SF */
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst,
               (Data**)(&(cell->sfAllocArr[idx].ueInfo.allocInfo)),
               (sizeof(RgInfUeAlloc)*RGSCH_MAX_UE_PER_DL_SF));
      }
   }

   RETVALUE(ROK);

}

/***********************************************************
 *
 *     Func : rgSCHUtlAllocSBuf
 *
 *     Desc : Utility Function to Allocate static buffer.
 *            Memory allocated is assumed contiguous.
 *
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: Caller doesnt need to raise the alarm in case of memory
 *            allocation gets failed.
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlAllocSBuf
(
Inst    inst,               /* Instance of the invoking scheduler */
Data    **pData,            /* Pointer of the data to be returned */
Size    size                /* size */
)
#else
PUBLIC S16 rgSCHUtlAllocSBuf(inst, pData, size)
Inst    inst;              /* Instance of the invoking scheduler */
Data    **pData;            /* Pointer of the data to be returned */
Size    size;               /* size */
#endif
{
   /* Moving alarm diagnostics to available scope */

   TRC2(rgSCHUtlAllocSBuf)

   /* Initialize the param to NULLP */
   *pData = NULLP;

   /* May not be necessary for data performance path */
#ifndef NO_ERRCLS
   if (size == 0)
   {
      RETVALUE(RFAILED);
   }
#endif

   /* allocate buffer */
   if (SGetSBuf(rgSchCb[inst].rgSchInit.region, rgSchCb[inst].rgSchInit.pool,
                      pData, size) != ROK)
   {
     RgUstaDgn dgn;      /* Alarm diagnostics structure */
     dgn.type = LRG_USTA_DGNVAL_MEM;
     dgn.u.mem.region = rgSchCb[inst].rgSchInit.region;
     dgn.u.mem.pool = rgSchCb[inst].rgSchInit.pool;
     /*  Send an alarm to Layer Manager */
     rgSCHLmmStaInd(inst, LCM_CATEGORY_RESOURCE, LCM_EVENT_SMEM_ALLOC_FAIL,
                                       LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
     RGSCHLOGERROR(inst, ERRCLS_DEBUG, ERG015, 0, "Unable to Allocate Buffer");
     RETVALUE(RFAILED);
   }

#ifndef ALIGN_64BIT
   RGSCHDBGINFO(inst,(rgSchPBuf(inst), "SGetSBuf(Region (%d), Pool (%d), "
             "Size (%ld)), Data (0x%p))\n", rgSchCb[inst].rgSchInit.region,
             rgSchCb[inst].rgSchInit.pool, size, *pData));
#else
   RGSCHDBGINFO(inst,(rgSchPBuf(inst), "SGetSBuf(Region (%d), Pool (%d),"
             "Size (%d)), Data (0x%p))\n", rgSchCb[inst].rgSchInit.region,
             rgSchCb[inst].rgSchInit.pool, size, *pData));
#endif

   /* zero out the allocated memory */
   cmMemset((U8 *)*pData, 0x00, size);

   RETVALUE(ROK);

} /* end of rgSCHUtlAllocSBuf */


/*
*
*       Fun:   rgSCHUtlFreeSBuf
*
*       Desc:  The argument to rgSCHUtlFreeSBuf() is a pointer to a block
*              previously allocated by rgSCHUtlAllocSBuf() and size. It
*              deallocates the memory.
*
*       Ret:   RETVOID
*
*       Notes: None
*
*       File:  rg_utl.c
*/
#ifdef ANSI
PUBLIC Void rgSCHUtlFreeSBuf
(
Inst inst,          /* Instance of the invoking scheduler */
Data **data,         /* pointer to data */
Size size           /* size */
)
#else
PUBLIC Void rgSCHUtlFreeSBuf(inst, data, size)
Inst inst;          /* Instance of the invoking scheduler */
Data **data;         /* pointer to data */
Size size;          /* size */
#endif
{

   S16 ret;

   TRC2(rgSCHUtlFreeSBuf)

   if ((data == NULLP) || (*data == NULLP) || (size == 0))
   {
      RETVOID;
   }

#ifndef ALIGN_64BIT
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "SPutSBuf(Region (%d), Pool (%d), "
              "Size (%ld), Data (0x%p))\n", rgSchCb[inst].rgSchInit.region,
              rgSchCb[inst].rgSchInit.pool, size, (U8 *) *data));
#else
   RGSCHDBGINFO(inst,(rgSchPBuf(inst), "SPutSBuf(Region (%d), Pool (%d), "
              "Size (%d), Data (0x%p))\n", rgSchCb[inst].rgSchInit.region,
              rgSchCb[inst].rgSchInit.pool, size, (U8 *) *data));
#endif

   /* Deallocate buffer */
   ret = SPutSBuf(rgSchCb[inst].rgSchInit.region,
                  rgSchCb[inst].rgSchInit.pool, (*data), size);

   if (ret != ROK)
   {
      RGSCHLOGERROR(inst, ERRCLS_DEBUG, ERG016, (ErrVal) 0,
                 "rgSCHUtlFreeSBuf failed.\n");
      RETVOID;
   }

   /* ccpu00117052 - ADD - Assigning the pointer to NULLP */
   *data = NULLP;

   RETVOID;

} /* end of rgSCHUtlFreeSBuf */

/***********************************************************
 *
 *     Func : rgSCHUtlFillDgnParams
 *
 *     Desc : Utility Function to Fill Diagonostic params.
 *
 *     Ret  : None.
 *
 *     Notes: None.
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlFillDgnParams
(
Inst        inst,
RgUstaDgn   *dgn,
U8          dgnType
)
#else
PUBLIC Void rgSCHUtlFillDgnParams(inst, dgn, dgnType)
Inst        inst;
RgUstaDgn   *dgn;
U8          dgnType;
#endif
{

   TRC2(rgSCHUtlFillDgnParams)

   switch(dgnType)
   {
      case LRG_USTA_DGNVAL_MEM:
         dgn->type = (U8) LRG_USTA_DGNVAL_MEM;
         dgn->u.mem.region  = rgSchCb[inst].rgSchInit.region;
         dgn->u.mem.pool    = rgSchCb[inst].rgSchInit.pool;
      break;

      default:
      break;
   }

   RETVOID;
} /* end of rgSCHUtlFillDgnParams */

/***********************************************************
 *
 *     Func : rgSCHUtlGetPstToLyr
 *
 *     Desc : Utility Function to get the pst structure to post a message to MAC
 *
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called while sending a msg from
 *     scheduler instance to MAC
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlGetPstToLyr
(
Pst     *pst,
RgSchCb *schCb,
Inst    macInst
)
#else
PUBLIC Void rgSCHUtlGetPstToLyr (pst, schCb, macInst)
Pst     *pst;
RgSchCb *schCb;
Inst    macInst;
#endif
{
   TRC2(rgSCHUtlGetPstToLyr);

   /* Only the needed params are filled */
   pst->region = schCb->rgSchInit.region;
   pst->pool = schCb->rgSchInit.pool;
   pst->srcInst = schCb->rgSchInit.inst+RGSCH_INST_START;
   pst->srcProcId = schCb->rgSchInit.procId;
   pst->dstProcId = schCb->rgSchInit.procId;

   pst->dstInst = macInst;
   pst->dstEnt = ENTRG;
   pst->srcEnt = ENTRG;
   pst->selector = 0;
   pst->prior     = PRIOR0;
   pst->intfVer   = 0;
   pst->route   = RTESPEC;

   RETVOID;
} /* end of rgSCHUtlGetPstToLyr */

/** @brief This function fills in the common lc information to be sent to MAC
 *
 * @details
 *
 *     Function: rgSCHUtlFillRgInfCmnLcInfo
 *
 *
 * @param  [out] RgSchCellCb  *cell
 * @param  [in]  RgInfSfAlloc *sfAlloc
 * @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHUtlFillRgInfCmnLcInfo
(
RgSchDlSf       *sf,
RgInfSfAlloc    *sfAlloc,
CmLteLcId       lcId,
Bool            sendInd
)
#else
PUBLIC S16 rgSCHUtlFillRgInfCmnLcInfo(sf, sfAlloc, lcId, sendInd)
RgSchDlSf       *sf;
RgInfSfAlloc    *sfAlloc;
CmLteLcId       lcId;
Bool            sendInd;
#endif
{
   TRC2(rgSCHUtlFillRgInfCmnLcInfo);

   if((sf->bch.tbSize)&&
      !(sfAlloc->cmnLcInfo.bitMask & RGINF_BCH_INFO))
   {
      /* rg004.201-MOD-Modified for SI Enhancement*/
#ifndef RGR_SI_SCH
      sfAlloc->cmnLcInfo.bchInfo.lcId = lcId;
#endif
      sfAlloc->cmnLcInfo.bitMask |= RGINF_BCH_INFO;
   }
   else if((sf->bcch.pdcch != NULLP)&&
      !(sfAlloc->cmnLcInfo.bitMask & RGINF_BCCH_INFO))
   {
      sfAlloc->cmnLcInfo.bcchInfo.rnti = RGSCH_SI_RNTI;
      rgSCHUtlFillPdschDciInfo(&(sfAlloc->cmnLcInfo.bcchInfo.dciInfo),
                                       &(sf->bcch.pdcch->dci));
#ifndef RGR_SI_SCH
      sfAlloc->cmnLcInfo.bcchInfo.lcId = lcId;
      sfAlloc->cmnLcInfo.bcchInfo.sndStatInd = sendInd;
#endif
      sfAlloc->cmnLcInfo.bitMask |= RGINF_BCCH_INFO;
   }
   else if((sf->pcch.pdcch != NULLP) &&
      !(sfAlloc->cmnLcInfo.bitMask & RGINF_PCCH_INFO))
   {
      sfAlloc->cmnLcInfo.pcchInfo.rnti = RGSCH_P_RNTI;
      rgSCHUtlFillPdschDciInfo(&(sfAlloc->cmnLcInfo.pcchInfo.dciInfo),
                                         &(sf->pcch.pdcch->dci));
      sfAlloc->cmnLcInfo.pcchInfo.lcId = lcId;
      sfAlloc->cmnLcInfo.bitMask |= RGINF_PCCH_INFO;
   }
   RETVALUE(ROK);
}

/** @brief This function fills in the RAR information to be sent to MAC
 *
 * @details
 *
 *     Function: rgSCHUtlFillRgInfRarInfo
 *
 *
 * @param  [out] RgSchCellCb  *cell
 * @param  [in]  RgInfSfAlloc *sfAlloc
 * @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHUtlFillRgInfRarInfo
(
RgSchDlSf       *sf,
RgInfSfAlloc    *sfAlloc
)
#else
PUBLIC S16 rgSCHUtlFillRgInfRarInfo(sf, sfAlloc)
RgSchDlSf       *sf;
RgInfSfAlloc    *sfAlloc;
#endif
{
   U8               idx;
   CmLListCp        *lnkLst;
   CmLList          *tmp;
   RgSchRaCb        *raCb;
   RgSchUeCb        *ue;
   RgInfRaRntiInfo  *raRntiAlloc;
   U8               noRaRsps;
   TRC2(rgSCHUtlFillRgInfRarInfo);

#ifdef LTE_TDD
   noRaRsps = RGSCH_MAX_TDD_RA_RSP_ALLOC;
#else
   noRaRsps = RGSCH_MAX_RA_RSP_ALLOC;
#endif

   for(idx =0; idx < noRaRsps; idx++)
   {
      if (sf->raRsp[idx].pdcch == NULLP)
      {
         /* No further raResp Allocations. */
         break;
      }
      raRntiAlloc = &(sfAlloc->rarInfo.raRntiInfo[idx]);
      raRntiAlloc->raRnti = sf->raRsp[idx].raRnti;
      raRntiAlloc->schdTbSz = sf->raRsp[idx].tbSz;
      raRntiAlloc->numCrnti = 0;
      rgSCHUtlFillPdschDciInfo(&(raRntiAlloc->dciInfo),
                      &(sf->raRsp[idx].pdcch->dci));
      /* RACHO : fill backoff indicator information */
      raRntiAlloc->backOffInd = sf->raRsp[idx].backOffInd;

      /* Fill for contention free UEs*/
      lnkLst = &(sf->raRsp[idx].contFreeUeLst);
      CM_LLIST_FIRST_NODE(lnkLst, tmp);
      while (tmp)
      {
         ue = (RgSchUeCb *)tmp->node;
         tmp = tmp->next;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].tmpCrnti = ue->ueId;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].isContFree = TRUE;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].rapId = ue->ul.rarGrnt.rapId;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.hop =
                                                        ue->ul.rarGrnt.hop;
         /* SHASHAHNK ADD RIV CALC */
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.rbStart =
                                                        ue->ul.rarGrnt.rbStart;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.numRb =
                                                        ue->ul.rarGrnt.numRb;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.tpc =
                                                        ue->ul.rarGrnt.tpc;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.iMcsCrnt =
                                                        ue->ul.rarGrnt.iMcsCrnt;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].ta = ue->ul.rarGrnt.ta;
         raRntiAlloc->numCrnti++;
         cmLListDelFrm(lnkLst, &ue->ul.rarGrnt.raRspLnk);
         ue->ul.rarGrnt.raRspLnk.node = (PTR)NULLP;
      }
      /* Fill for contention based UEs*/
      lnkLst = &(sf->raRsp[idx].raRspLst);

      CM_LLIST_FIRST_NODE(lnkLst, tmp);

      while((NULLP != tmp) && ((RgSchRaCb *)tmp->node != NULLP))
      {
         raCb     = (RgSchRaCb *)tmp->node;

         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].tmpCrnti = raCb->tmpCrnti;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].isContFree = FALSE;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].rapId = raCb->rapId;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].ta.pres = TRUE;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].ta.val = raCb->ta.val;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.hop =
                                                        raCb->msg3Grnt.hop;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.rbStart =
                                                        raCb->msg3Grnt.rbStart;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.numRb =
                                                        raCb->msg3Grnt.numRb;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.tpc =
                                                        raCb->msg3Grnt.tpc;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.iMcsCrnt =
                                                        raCb->msg3Grnt.iMcsCrnt;
         raRntiAlloc->crntiInfo[raRntiAlloc->numCrnti].grnt.delayBit =
                                                        raCb->msg3Grnt.delayBit;
         raRntiAlloc->numCrnti++;
         /* Search the next node */
         CM_LLIST_NEXT_NODE(lnkLst, tmp);
      }
   }
   sfAlloc->rarInfo.numRaRntis = idx;
   RETVALUE(ROK);
} /* end of rgSCHUtlFillRgInfRarInfo */

/** @brief This function fills in the pdsch data related allocation Info
 *         from the pdcch DCI info.
 * subframe
 *
 * @details
 *
 *     Function: rgSCHUtlFillPdschDciInfo
 *
 *         Processing steps:
 *         - Depending upon the DCI Format, fill the appropriate fields.
 *
 * @param  [out] TfuPdschDciInfo *pdschDci
 * @param  [in]  TfuDciInfo      *pdcchDci
 * @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHUtlFillPdschDciInfo
(
TfuPdschDciInfo *pdsch,
TfuDciInfo      *pdcchDci
)
#else
PUBLIC S16 rgSCHUtlFillPdschDciInfo(pdsch, pdcchDci)
TfuPdschDciInfo *pdsch;
TfuDciInfo      *pdcchDci;
#endif
{
   TRC2(rgSCHUtlFillPdschDciInfo)

   pdsch->format = pdcchDci->dciFormat;
   switch(pdcchDci->dciFormat)
   {
      case TFU_DCI_FORMAT_1:
         pdsch->u.format1AllocInfo = pdcchDci->u.format1Info.allocInfo;
         break;
      case TFU_DCI_FORMAT_1A:
    /*rg008.201 - Added support for SPS*/
#ifndef LTEMAC_SPS
         if (pdcchDci->u.format1aInfo.isPdcchOrder == FALSE)
#else
         if (pdcchDci->u.format1aInfo.dciInfo != TFU_DCIFMT1A_PDCCHORDER)
#endif /* LTEMAC_SPS */
         {
            pdsch->u.format1aAllocInfo = pdcchDci->u.format1aInfo.t.pdschInfo.allocInfo;
         }
         break;
      case TFU_DCI_FORMAT_1B:
         pdsch->u.format1bAllocInfo = pdcchDci->u.format1bInfo.allocInfo;
         break;
      case TFU_DCI_FORMAT_1C:
         pdsch->u.format1cAllocInfo = pdcchDci->u.format1cInfo;
         break;
      case TFU_DCI_FORMAT_1D:
         pdsch->u.format1dAllocInfo = pdcchDci->u.format1dInfo.allocInfo;
         break;
      case TFU_DCI_FORMAT_2:
         pdsch->u.format2AllocInfo = pdcchDci->u.format2Info.allocInfo;
         break;
      case TFU_DCI_FORMAT_2A:
         pdsch->u.format2AAllocInfo = pdcchDci->u.format2AInfo.allocInfo;
         break;
      default:
         RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}
#if 0
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/***********************************************************
 *
 *     Func : rgSCHUtlFillRgInfTbInfo
 *
 *     Desc : Utility Function to fill the allocation info of each Tb
 *
 *
 *     Ret  :  RETVOID
 *
 *
 *     Notes: This function should be called while sending a msg from
 *     scheduler instance to MAC
 *
 *     File : rg_utl.c
 *
 **********************************************************/

#ifdef ANSI
PRIVATE Void rgSCHUtlFillRgInfTbInfo
(
RgSchDlSf       *sf,
CmLList         **tbLnk,
RgInfUeAlloc    *allocInfo
)
#else
PRIVATE Void rgSCHUtlFillRgInfTbInfo (sf, tbLnk, allocInfo)
RgSchDlSf       *sf;
CmLList         **tbLnk;
RgInfUeAlloc    *allocInfo;
#endif
{
   U8              idx;
   RgInfUeTbInfo   *tbInfo;
   U8              tbCnt;
   RgSchDlHqProcCb *hqP = ((RgSchDlHqTbCb *)((*tbLnk)->node))->hqP;
#ifdef LTE_L2_MEAS
   RgSchDlLcCb    *dlLcCb;
   dlLcCb   = NULLP;
#endif

   allocInfo->tbStrtIdx = -1;
   for(tbCnt = 0; tbCnt < 2; tbCnt++)
   {
      RgSchUeCb  *ue = NULLP;
      /*Changed as a result of CR timer*/
      if ((hqP->hqE->ue != NULLP)/* &&
      ((hqP->tbInfo[tbCnt].lchSchdData[0].lcId != 0) || \
       (hqP->tbInfo[tbCnt].schdTa.pres == PRSNT_NODEF))*/)
      {
         ue = hqP->hqE->ue;
         /*rg002.301 - MOD - [ccpu00119223] Assigned the ueId instead of rntiLnk->rnti */
         allocInfo->rnti = ue->ueId;
         allocInfo->doa = hqP->hqE->ue->mimoInfo.doa;
         allocInfo->txMode = (TfuTxMode)(hqP->hqE->ue->mimoInfo.txMode);
         allocInfo->puschRptUsd = hqP->hqE->ue->mimoInfo.puschFdbkVld;
         allocInfo->puschPmiInfo = hqP->hqE->ue->mimoInfo.puschPmiInfo;
         if(hqP->tbInfo[tbCnt].schdTa.pres == PRSNT_NODEF)
         {
            hqP->tbInfo[tbCnt].taSnt = TRUE;
         }
#ifdef TFU_UPGRADE
         if (RG_SCH_IS_PAPRSNT(hqP->hqE->ue))
         {
            /*update pA value */
            allocInfo->pa = hqP->hqE->ue->pA.val;
         }
#endif
      }
      else
      {
         if (hqP->hqE->raCb)
         {
       /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
         allocInfo->pdcchRnti = hqP->hqE->raCb->tmpCrnti;
#endif
            allocInfo->rnti  =  hqP->hqE->raCb->tmpCrnti;
         }
      }
      /* If TB Is scheduled for this SF */
      if(hqP->tbInfo[tbCnt].subFrm == sf)
      {
         if (allocInfo->tbStrtIdx == -1){
            allocInfo->tbStrtIdx = tbCnt;
       /*rg008.201 - Added support for SPS*/
#ifndef LTEMAC_SPS
            rgSCHUtlFillPdschDciInfo(&(allocInfo->dciInfo),
                               &(hqP->tbInfo[tbCnt].pdcch->dci));
#else
            if (hqP->tbInfo[tbCnt].pdcch)
            {
               rgSCHUtlFillPdschDciInfo(&(allocInfo->dciInfo),
                     &(hqP->tbInfo[tbCnt].pdcch->dci));
            }
            else if ((ue) && (ue->dl.spsOccPdcch.rnti == ue->spsRnti))
            {
               rgSCHUtlFillPdschDciInfo(&(allocInfo->dciInfo),
                     &(ue->dl.spsOccPdcch.dci));
            }
#endif /* ifndef LTEMAC_SPS */
         }
#ifdef LTEMAC_SPS
            if (hqP->tbInfo[tbCnt].pdcch)
            {
               allocInfo->pdcchRnti = hqP->tbInfo[tbCnt].pdcch->rnti;
            }
            else if ((ue) && (ue->dl.spsOccPdcch.rnti == ue->spsRnti))
            {
               allocInfo->pdcchRnti = ue->spsRnti;
            }
#endif
         tbInfo = &(allocInfo->tbInfo[tbCnt]);
         allocInfo->nmbOfTBs++;
         allocInfo->hqProcId = hqP->procId;
         allocInfo->tbInfo[tbCnt].schdTbSz = hqP->tbInfo[tbCnt].tbSz;

         tbInfo->disTb  = FALSE;
         if(!(hqP->tbInfo[tbCnt].txCntr))
         {
            hqP->tbInfo[tbCnt].txCntr++;
            for(idx = 0; idx < hqP->tbInfo[tbCnt].numLch; idx++)
            {
               tbInfo->schdDat[idx].lcId =\
                  hqP->tbInfo[tbCnt].lchSchdData[idx].lcId;
               tbInfo->schdDat[idx].numBytes =\
                  hqP->tbInfo[tbCnt].lchSchdData[idx].schdData;
#ifdef PWAV				  
				  if((tbInfo->schdDat[idx].lcId == 1) && (hqP->hqE->ue))
				  {
				   MSPD_DBG("Consolidating UE ID:%d, \n", ue->ueId);
				  }
#endif
#ifdef LTE_L2_MEAS
               if(hqP->hqE->ue)
               {
                    dlLcCb = hqP->hqE->ue->dl.lcCb[(hqP->tbInfo[tbCnt].
                             lchSchdData[idx].lcId) - 1];
                    if(dlLcCb != NULLP)
                    {
                       dlLcCb->qciCb->dlPrbCount += (hqP->tbInfo[tbCnt].\
                                                 lchSchdData[idx].schdData  *
                                                 hqP->tbInfo[tbCnt].dlGrnt.numRb) /
                                                 hqP->tbInfo[tbCnt].tbSz;
                    }
               }
#endif
            }
            tbInfo->numSchLch = hqP->tbInfo[tbCnt].numLch;
            if(!(hqP->tbInfo[tbCnt].numLch))
            {
               tbInfo->schdDat[tbInfo->numSchLch].numBytes= hqP->tbInfo[tbCnt].tbSz;
            }
            tbInfo->ta.pres = hqP->tbInfo[tbCnt].schdTa.pres;
            tbInfo->ta.val =  hqP->tbInfo[tbCnt].schdTa.val;
            /*rg009.201 - CR timer implementation changes*/
            tbInfo->contResCe = hqP->tbInfo[tbCnt].contResCe;
            tbInfo->isReTx = FALSE;
         }
         else
         {
            hqP->tbInfo[tbCnt].txCntr++;
            tbInfo->isReTx = TRUE;
         }
         /* Update tbLnk to point to the next TB */
         CM_LLIST_NEXT_NODE(&sf->tbs, *tbLnk);
      }
    }
   RETVOID;
}
#endif
#if 0
/***********************************************************
 *
 *     Func : rgSCHUtlFillRgInfUeInfo
 *
 *     Desc : Utility Function to fill the allocation info of Ue
 *             : MIMO : Filling 2TB's of each UE
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called while sending a msg from
 *     scheduler instance to MAC
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlFillRgInfUeInfo
(
RgSchDlSf       *sf,
RgInfSfAlloc    *sfAlloc
)
#else
PUBLIC Void rgSCHUtlFillRgInfUeInfo (sf, sfAlloc)
RgSchDlSf       *sf;
RgInfSfAlloc    *sfAlloc;
#endif
{
   CmLListCp       *lnkLst;   /* lnkLst assignment */
   CmLList         *tmp;
   U8              numUes = 0;
   RgInfUeInfo     *ueInfo = &(sfAlloc->ueInfo);
   RgInfUeAlloc    *ueAlloc = sfAlloc->ueInfo.allocInfo;

   lnkLst = &(sf->tbs);
   CM_LLIST_FIRST_NODE(lnkLst, tmp);
   while(NULLP != tmp)
   {
      rgSCHUtlFillRgInfTbInfo(sf, &tmp, &ueAlloc[numUes]);
      numUes++;
   }
   ueInfo->numUes = numUes;
   RETVOID;
} /* end of rgSCHUtlFillRgInfUeInfo */
#endif

/** @brief This function  shall update the scheduler with the CEs and data rcvd
 *
 * @details
 *
 *     Function: rgSCHUtlUpdSch
 *
 *         Processing steps:
 *         - Collate the information of all the SDUs received and inform the
 *         scheduler rgSCHDataRcvd
 *         - Send Data indication to the higher layer with the dedicated data
 *         (rgUIMSndDedDatInd)
 *         - Inform scheduler with any MAC CEs if present.
 *
 * @param  [in] RgCellCb   *cellCb
 * @param  [in] RgUeCb     *ueCb
 * @param  [in] RgMacPdu   *pdu
 * @param  [in] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHUtlUpdSch
(
RgInfSfDatInd    *subfrmInfo,
RgSchCellCb      *cellCb,
RgSchUeCb        *ueCb,
RgInfUeDatInd    *pdu,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHUtlUpdSch (subfrmInfo, cellCb, ueCb, pdu, err)
RgInfSfDatInd    *subfrmInfo;
RgSchCellCb      *cellCb;
RgSchUeCb        *ueCb;
RgInfUeDatInd    *pdu;
RgSchErrInfo     *err;
#endif
{

   S16               ret;
   TRC2(rgSCHUtlUpdSch);

   /* Update scheduler with CEs if present */
   if (RGSCH_PHR_CE_PRSNT & pdu->ceInfo.bitMask)
   {
      /* PHR present */
      RGSCHCPYTIMEINFO(subfrmInfo->timingInfo, ueCb->macCeRptTime);
      if ((ret = rgSCHUtlUpdPhr(cellCb, ueCb, pdu->ceInfo.ces.phr, err)) != ROK)
         RETVALUE(ret);
   }
   if (RGSCH_TRUNC_BSR_CE_PRSNT & pdu->ceInfo.bitMask)
   {
      RGSCHCPYTIMEINFO(subfrmInfo->timingInfo, ueCb->macCeRptTime);
      if ((ret = rgSCHUtlUpdBsrTrunc (cellCb, ueCb,
                  (U8)(pdu->ceInfo.ces.bsr.truncBsr >> 6),
                  (U8)(pdu->ceInfo.ces.bsr.truncBsr & 0x3F), err)) != ROK)
         RETVALUE(ret);
   }
   else
   {
      if (RGSCH_SHORT_BSR_CE_PRSNT & pdu->ceInfo.bitMask)
      {
         RGSCHCPYTIMEINFO(subfrmInfo->timingInfo, ueCb->macCeRptTime);
         if ((ret = rgSCHUtlUpdBsrShort (cellCb, ueCb,
                     (U8)(pdu->ceInfo.ces.bsr.shortBsr >> 6),
                     (U8)(pdu->ceInfo.ces.bsr.shortBsr & 0x3F), err)) != ROK)
            RETVALUE(ret);
      }
      else
      {
         if (RGSCH_LONG_BSR_CE_PRSNT & pdu->ceInfo.bitMask)
         {
            RGSCHCPYTIMEINFO(subfrmInfo->timingInfo, ueCb->macCeRptTime);
            if ((ret = rgSCHUtlUpdBsrLong (cellCb, ueCb,
                        pdu->ceInfo.ces.bsr.longBsr.bs1,
                        pdu->ceInfo.ces.bsr.longBsr.bs2,
                        pdu->ceInfo.ces.bsr.longBsr.bs3,
                        pdu->ceInfo.ces.bsr.longBsr.bs4,
                        err)) != ROK)
               RETVALUE(ret);
         }
      }
   }
   RETVALUE(ROK);
} /* end of rgSCHUtlUpdSch */
#ifdef RGR_V1
/*rg009.201 - Changes for CR timer*/
/**
 * @brief Handler for Updating Bo received in StaRsp
 *
 * @details
 *
 *     Function : rgSCHUtlAddUeToCcchSduLst
 *
 *     This function shall be invoked once it receives staRsp on CCCH
 *
 *  @param[in]     RgSchCellCb       *cell
 *  @param[in]     RgSchUeCb       *ueCb
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlAddUeToCcchSduLst
(
RgSchCellCb       *cell,
RgSchUeCb         *ueCb
)
#else
PUBLIC S16 rgSCHUtlAddUeToCcchSduLst(cell, ueCb)
RgSchCellCb       *cell;
RgSchUeCb         *ueCb;
#endif
{
#ifdef DEBUGP
   Inst         inst = cell->instIdx;
#endif
   TRC2(rgSCHUtlAddUeToCcchSduLst);

#ifndef ALIGN_64BIT
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), \
       "rgSCHUtlAddUeToCcchSduLst(): crnti = %u;"
                             , ueCb->ueId));
#else
   RGSCHDBGPRM(inst, (rgSchPBuf(inst),
       "rgSCHUtlAddUeToCcchSduLst(): crnti = %u; "
                              , ueCb->ueId));
#endif
   /* add this to the cell wide Ue list for CCCH SDU */
   /*ueCb->ccchSduLnk.next = NULLP;
   ueCb->ccchSduLnk.prev = NULLP;*/
   ueCb->ccchSduLnk.node = (PTR)(ueCb);
   cmLListAdd2Tail(&(cell->ccchSduUeLst), &(ueCb->ccchSduLnk));

   RETVALUE(ROK);
}
/**
 * @brief Handler for Updating Bo received in StaRsp
 *
 * @details
 *
 *     Function : rgSCHUtlUpdtBo
 *
 *     This function shall be invoked once it receives staRsp on CCCH
 *
 *  @param[in]     RgSchCellCb       *cell
 *  @param[in]     RgRguCmnStaRsp *staRsp
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUpdtBo
(
RgSchCellCb       *cell,
RgInfCmnBoRpt     *staRsp
)
#else
PUBLIC S16 rgSCHUtlUpdtBo(cell, staRsp)
RgSchCellCb       *cell;
RgInfCmnBoRpt     *staRsp;
#endif
{
   RgSchUeCb *ueCb;
   Inst         inst = cell->instIdx;
   TRC2(rgSCHUtlUpdtBo)

#ifndef ALIGN_64BIT
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHUtlUpdtBo(): crnti = %u;"
                             " bo = %ld\n", staRsp->u.rnti, (U32)staRsp->bo));
#else
   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "rgSCHUtlUpdtBo(): crnti = %u; "
                              "bo = %d\n", staRsp->u.rnti, (U32)staRsp->bo));
#endif

   if ((ueCb = rgSCHDbmGetUeCb(cell, staRsp->u.rnti)) == NULLP)
   {
      /* Handle Ue fetch failure */
      RGSCHLOGERROR(inst, ERRCLS_INT_PAR,ERG017,(ErrVal)staRsp->u.rnti,
       "rgSCHUtlUpdtBo(): Invalid ue Id");
      RETVALUE(RFAILED);
   }
   /* Update Bo in ueCb */
   ueCb->dlCcchInfo.bo = (U32)(staRsp->bo);
   rgSCHUtlAddUeToCcchSduLst(cell, ueCb);


   RETVALUE(ROK);
} /* rgSCHUtlUpdtBo */

#endif
#if 0
/**
 * @brief Handler for BO Updt received for CCCH.
 *
 * @details
 *
 *     Function : rgSCHUtlHndlCcchBoUpdt
 *
 *     This function shall fetch the raCb with the given rnti and ask RAM to
 *     update BO
 *
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgInfCmnBoRpt *boRpt
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlHndlCcchBoUpdt
(
RgSchCellCb    *cell,
RgInfCmnBoRpt  *boRpt
)
#else
PUBLIC S16 rgSCHUtlHndlCcchBoUpdt(cell, boRpt)
RgSchCellCb    *cell;
RgInfCmnBoRpt  *boRpt;
#endif
{
   RgSchRaCb       *raCb;
   Inst            inst = cell->instIdx;

   TRC2(rgSCHUtlHndlCcchBoUpdt);

   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "rgSCHUtlHndlCcchBoUpdt(): boRpt->u.rnti = %u\n",
                                             boRpt->u.rnti));
   if ((raCb = rgSCHDbmGetRaCb(cell, boRpt->u.rnti)) == NULLP)
   {
#ifdef RGR_V1
   /*rg009.201 - CR timer implementation changes*/
      /*If no raCb, schedule ueCb, ueCb is extracted in rgSCHUtlUpdtBo*/
      RETVALUE(rgSCHUtlUpdtBo(cell, boRpt));
#else
      /* Handle RaCb fetch failure */
      RGSCHLOGERROR(inst, ERRCLS_INT_PAR,ERG018,(ErrVal)boRpt->u.rnti,
       "rgSCHUtlHndlCcchBoUpdt(): Invalid rnti to fetch RaCb");
      RETVALUE(RFAILED);
#endif
   }

#ifdef RGR_V1
   /*rg009.201 - CR timer implementation changes*/

   /* MSG4 MS_FIX Start */
   if(raCb->contResTmrLnk.node != NULLP)
   {   	
      if(raCb->schdLnk.node == NULLP)
      {
         cmLListDelFrm(&cell->contResGrdTmrLst, &(raCb->contResTmrLnk));
         raCb->contResTmrLnk.node=NULLP;               
         rgSCHRamUpdtBo(cell, raCb, boRpt);
      }
	  else
      {
         /* Update Bo in RaCb */
         raCb->dlCcchInfo.bo = (U32)(boRpt->bo);      
      }
   }
   else
   {
      /*
       * Since MSG 4 is already scheduled For further ReTx.
       * UeCb Should be used
       */
       RETVALUE(rgSCHUtlUpdtBo(cell, boRpt));
   }   
   /*MSG4 MS_FIX End */   
  
#else
      rgSCHRamUpdtBo(cell, raCb, boRpt);
#endif

   RETVALUE(ROK);
} /* rgSCHUtlHndlCcchBoUpdt */
#endif
/*rg001.201:107637 Nprb indication at PHY for common Ch */
/**
 * @brief Validates BO received for BCCH or PCCH.
 *
 * @details
 *
 *     Function : rgSCHUtlGetAllwdCchTbSz
 *
 *     This function shall return the tbSz equal to or
 *     the nearest greater value for a given bo.
 *     If no such value found return -1. The nPrb value is
 *     accordingly set.
 *
 *
 *  @param[in]  U32            bo
 *  @param[out] U8             *nPrb
 *  @return  S32
 *      -# ROK
 *      -# RFAILED
 **/
/* rg002.201: Corrected allocation for common channels */
#ifdef ANSI
PUBLIC S32 rgSCHUtlGetAllwdCchTbSz
(
U32 bo,
U8  *nPrb,
U8  *mcs
)
#else
PUBLIC S32 rgSCHUtlGetAllwdCchTbSz(bo, nPrb, mcs)
U32 bo;
U8  *nPrb;
U8  *mcs;
#endif
{
   S32 lt;
   S32 cn;
   S32 rt;

   TRC2(rgSCHUtlGetAllwdCchTbSz);

   for (lt = 0, rt = 43; lt <= rt;)
   {
      cn = (lt + rt)/2;
      if (rgSchUtlBcchPcchTbSzTbl[cn].tbSz == bo)
      {
         *nPrb = rgSchUtlBcchPcchTbSzTbl[cn].rbIndex;
         *mcs  = rgSchUtlBcchPcchTbSzTbl[cn].mcs;
         RETVALUE(rgSchUtlBcchPcchTbSzTbl[cn].tbSz);
      }
      else if (rgSchUtlBcchPcchTbSzTbl[cn].tbSz < bo)
         lt = cn + 1;
      else
         rt = cn - 1;
   }
   if (lt == 44)
   {
      RETVALUE(-1);
   }
   *nPrb = rgSchUtlBcchPcchTbSzTbl[lt].rbIndex;
   *mcs  = rgSchUtlBcchPcchTbSzTbl[lt].mcs;
   RETVALUE(rgSchUtlBcchPcchTbSzTbl[lt].tbSz);
}
#if 0
/**
 * @brief Handler for BO Updt received for BCCH or PCCH.
 *
 * @details
 *
 *     Function : rgSCHUtlHndlBcchPcchBoUpdt
 *
 *     This function shall store the buffer and time to transmit in lcCb
 *
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgInfCmnBoRpt  *boRpt
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlHndlBcchPcchBoUpdt
(
RgSchCellCb    *cell,
RgInfCmnBoRpt  *boUpdt
)
#else
PUBLIC S16 rgSCHUtlHndlBcchPcchBoUpdt(cell, boUpdt)
RgSchCellCb    *cell;
RgInfCmnBoRpt  *boUpdt;
#endif
{
   RgSchClcDlLcCb *dlLc;
   RgSchClcBoRpt  *boRpt;
   Inst           inst = cell->instIdx;
   U8             nPrb=0;
   /* rg002.201: ccpu00108811: Corrected allocation for common channels */
   U8             mcs=0;

   TRC2(rgSCHUtlHndlBcchPcchBoUpdt);

#ifndef ALIGN_64BIT
   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "rgSCHUtlHndlBcchPcchBoUpdt(): lcId = %u, \
                  bo = %ld, timeToTx.sfn = %u, timeToTx.subframe = %u\n",
                  boUpdt->lcId, boUpdt->bo,
                  boUpdt->u.timeToTx.sfn, boUpdt->u.timeToTx.subframe));
#else
   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "rgSCHUtlHndlBcchPcchBoUpdt(): lcId = %u, \
                  bo = %d,timeToTx.sfn = %u, timeToTx.subframe = %u\n",
                  boUpdt->lcId, boUpdt->bo,
                  boUpdt->u.timeToTx.sfn, boUpdt->u.timeToTx.subframe));
#endif
   /* rg002.201: ccpu00108808, ccpu00108807, ccpu00108737 :
    * Removed bo size validation for MIB */
   dlLc = rgSCHDbmGetBcchOnBch(cell);
   if (dlLc == NULLP)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"rgSCHUtlHndlBcchPcchBoUpdt():\
               No Logical Channel. dlLc is NULLP \n"));
      RETVALUE(RFAILED);
   }
   if (boUpdt->lcId != dlLc->lcId)
   {
      if (rgSCHUtlGetAllwdCchTbSz(boUpdt->bo*8, &nPrb, &mcs) != (boUpdt->bo*8))
      {
         RGSCHLOGERROR(inst,ERRCLS_INT_PAR,ERG020,(ErrVal)boUpdt->bo,
               "rgSCHUtlHndlBcchPcchBoUpdt(): bo = %d does not match\
               any valid TB Size.");
         RETVALUE(RFAILED);
      }
   }
   if ((dlLc = rgSCHDbmGetCmnLcCb(cell, boUpdt->lcId)) == NULLP)
   {
      /* Handle lcCb fetch failure */
      RGSCHLOGERROR(inst,ERRCLS_INT_PAR,ERG021,(ErrVal)boUpdt->lcId,
                        "rgSCHUtlHndlBcchPcchBoUpdt(): Invalid Lc Id");
      RETVALUE(RFAILED);
   }

   if (((rgSCHUtlAllocSBuf(inst, (Data **)(&boRpt), sizeof(RgSchClcBoRpt))) ==RFAILED) ||
       (!boRpt))
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "rgSCHUtlHndlBcchPcchBoUpdt(): Allocation \
                       of common bo report failed\n"));
      RETVALUE(RFAILED);
   }

   boRpt->bo = boUpdt->bo;
   boRpt->nPrb = nPrb;
   /* rg002.201: ccpu00108811: Corrected allocation for common channels */
   boRpt->mcs = mcs;
   boRpt->timeToTx = boUpdt->u.timeToTx;
   RGSCHADDTOCRNTTIME(boRpt->timeToTx,
         boRpt->maxTimeToTx, cell->siCfg.siWinSize)
   if(dlLc->si)
   {
      boRpt->retxCnt = cell->siCfg.retxCnt;
   }
   else
   {
      boRpt->retxCnt = 0;
   }
   rgSCHDbmInsCmnLcBoRpt(dlLc, boRpt);

   RETVALUE(ROK);
} /* rgSCHUtlHndlBcchPcchBoUpdt */
#endif
/**
 * @brief API for sending bind confirm from Scheduler instance to RRM
 *
 * @details
 *
 *     Function: rgSCHUtlRgrBndCfm
 *
 *     This API is invoked to send bind confirm from Scheduler instance to RRM.
 *     This API fills in Pst structure and SAP Ids and invokes
 *     bind confirm API towards RRM.
 *
 *  @param[in]  SuId          suId
 *  @param[in]  U8            status
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrBndCfm
(
Inst          instId,
SuId          suId,
U8            status
)
#else
PUBLIC S16 rgSCHUtlRgrBndCfm(instId, suId, status)
Inst          instId;
SuId          suId;
U8            status;
#endif
{
   S16  ret = ROK;

   TRC2(rgSCHUtlRgrBndCfm)

   RGSCHDBGPRM(instId,(rgSchPBuf(instId),"rgSCHUtlRgrBndCfm: suId = %d; status = %d \n", suId, status));

   ret = RgUiRgrBndCfm(&rgSchCb[instId].rgrSap[suId].sapCfg.sapPst, rgSchCb[instId].rgrSap[suId].sapCfg.suId, status);
   if (ret != ROK)
   {
      RGSCHDBGERR(instId,(rgSchPBuf(instId),"rgSCHUtlRgrBndCfm: RgUiRgrBndCfm Failed \n"));
      RETVALUE(ret);
   }
   RETVALUE(ret);
}  /* rgSCHUtlRgrBndCfm*/
/**
 * @brief API for sending configuration confirm from Scheduler to RRM
 *
 * @details
 *
 *     Function: rgSCHUtlRgrCfgCfm
 *
 *     This API is invoked to send configuration confirm from Scheduler to RRM.
 *     This API fills in Pst structure and SAP Ids and invokes
 *     config confirm API towards RRM.
 *
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  U8            status
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrCfgCfm
(
Inst          instId,
SpId          spId,
RgrCfgTransId transId,
U8            status
)
#else
PUBLIC S16 rgSCHUtlRgrCfgCfm(instId, spId, transId, status)
Inst          instId;
SpId          spId;
RgrCfgTransId transId;
U8            status;
#endif
{
   U8        prntTrans[RGR_CFG_TRANSID_SIZE+1];

   TRC2(rgSCHUtlRgrCfgCfm)

   /* rg002.101:ccpu00101624: Ensuring transId is always Null terminated. */
   cmMemcpy((U8 *)prntTrans, (U8 *)transId.trans, RGR_CFG_TRANSID_SIZE);
   prntTrans[RGR_CFG_TRANSID_SIZE] = '\0';

   RGSCHDBGPRM(instId,(rgSchPBuf(instId),"rgSCHUtlRgrCfgCfm: transId  %s status  %d \n", prntTrans, status));

   if(RgUiRgrCfgCfm(&rgSchCb[instId].rgrSap[spId].sapCfg.sapPst,
                    rgSchCb[instId].rgrSap[spId].sapCfg.suId,
                    transId, status) != ROK)
   {
      RGSCHDBGERR(instId,(rgSchPBuf(instId),"rgSCHUtlRgrCfgCfm: RgUiRgrCfgCfm Failed \n"));
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}  /* rgSCHUtlRgrCfgCfm */
#if 0
/* rg001.201: Added for sending TTI tick to RRM */
#ifdef RGR_RRM_TICK
/**
 * @brief API for sending TTI indication from Scheduler to RRM.
 *
 * @details
 *
 *     Function: rgSCHUtlRgrTtiInd
 *
 *     This API is invoked to send TTI indication from Scheduler instance to RRM.
 *     This API fills in Pst structure and RgrTtiIndInfo
 *     and calls the TTI indication API towards RRM.
 *
 *  @param[in]  cell                   RgSchCellCb
 *  @param[in]  CmLteTimingInfo        status
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrTtiInd
(
RgSchCellCb          *cell,
RgrTtiIndInfo        *rgrTti
)
#else
PUBLIC S16 rgSCHUtlRgrTtiInd(cell, rgrTti)
RgSchCellCb          *cell;
RgrTtiIndInfo        *rgrTti;
#endif
{
   S16           ret = ROK;
   RgSchUpSapCb  *rgrSap;                    /*!< RGR SAP Control Block */

   TRC2(rgSCHUtlRgrTtiInd)

   RGSCHDBGPRM(cell->instIdx,(rgSchPBuf(cell->instIdx),"rgSCHUtlRgrTtiInd: cellId = %d \n", cell->cellId));

   rgrSap = cell->rgrSap;
   /* rg002.201 Fixed compilation problem with DEBUGP */
   if (rgrSap->sapSta.sapState != LRG_BND)
   {
      RGSCHDBGERR(cell->instIdx,(rgSchPBuf(cell->instIdx),
               "rgSCHUtlRgrTtiInd() Upper SAP not bound (%d) \n",
               rgrSap->sapSta.sapState));
      RETVALUE(RFAILED);
   }
   RgUiRgrTtiInd(&(cell->rgrSap->sapCfg.sapPst),
         cell->rgrSap->sapCfg.suId, rgrTti);
   RETVALUE(ret);
}  /* rgSCHUtlRgrTtiInd*/
#endif


/** @brief This function is called by rgMacSchSfRecpInd. This function invokes the
 * scheduler with the information of the received Data and any Control Elements
 * if present.
 *
 * @details
 *
 *     Function:
 *
 *         Processing steps:
 *         - Retrieves the RaCb with the rnti provided, if it doesnt exist
 *         return failure.
 *         - If UE exists then update the Schduler with any MAC CEs if present.
 *         - Invoke RAM module to do Msg3 related processing rgSCHRamProcMsg3
 *
 * @param  [in] RgSchCellCb   *cellCb
 * @param  [in] RgSchUeCb     *ueCb
 * @param  [in] CmLteRnti     rnti
 * @param  [in] RgMacPdu   *pdu
 * @param  [in] RgSchErrInfo  *err
 * @param
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHUtlProcMsg3
(
RgInfSfDatInd    *subfrmInfo,
RgSchCellCb      *cellCb,
RgSchUeCb        *ueCb,
CmLteRnti        rnti,
RgInfUeDatInd    *pdu,
RgSchErrInfo     *err
 )
#else
PUBLIC S16 rgSCHUtlProcMsg3 (subfrmInfo, cellCb, ueCb, rnti, pdu, err)
RgInfSfDatInd    *subfrmInfo;
RgSchCellCb      *cellCb;
RgSchUeCb        *ueCb;
CmLteRnti        rnti;
RgInfUeDatInd    *pdu;
RgSchErrInfo     *err;
#endif
{
   S16               ret;
   RgSchRaCb         *raCb;
#ifdef DEBUGP
   Inst              inst = cellCb->instIdx;
#endif
   TRC2(rgSCHUtlProcMsg3)


   /* must have an raCb for this case */
   raCb = rgSCHDbmGetRaCb (cellCb, rnti);
   if (raCb == NULLP)
   {
      RGSCHDBGERR (inst, (rgSchPBuf(inst),"\n rgSCHUtlProcMsg3() Received MSG3, unable to find raCb for rnti(%d)\n",
               rnti));
      RETVALUE(RFAILED);
   }

   if (ueCb != NULLP)
   {
      rgSCHUtlUpdSch (subfrmInfo, cellCb, ueCb, pdu, err);
   }
   /* if ueCb is present */
   ret = rgSCHRamProcMsg3 (cellCb, ueCb, raCb, pdu, err);
   if (ret != ROK)
   {
      RGSCHDBGERR (inst, (rgSchPBuf(inst),"rgSCHUtlProcMsg3() Processing failed in the RAM cellId (%d) \n",
               cellCb->cellId));
      RETVALUE(ret);
   }

   RETVALUE(ROK);
}
/*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS
/** @brief This function is called by RgMacSchSpsRelInd. This function invokes the
 * scheduler with the information of the received Data.
 *
 * @details
 *
 *     Function: rgSCHUtlSpsRelInd
 *
 *         Processing steps:
 *         TODO
 *
 * @param  [in] RgSchCellCb        *cellCb
 * @param  [in] RgSchUeCb          *ueCb
 * @param  [in] Bool               *isExplRel
 * @param
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHUtlSpsRelInd
(
RgSchCellCb        *cellCb,
RgSchUeCb          *ueCb,
Bool               isExplRel
)
#else
PUBLIC S16 rgSCHUtlSpsRelInd (cellCb, ueCb, isExplRel)
RgSchCellCb        *cellCb;
RgSchUeCb          *ueCb;
Bool               isExplRel;
#endif
{
   TRC2(rgSCHUtlSpsRelInd);
   cellCb->sc.apis->rgSCHUlSpsRelInd(cellCb, ueCb, isExplRel);
   RETVALUE(ROK);
} /* end of rgSCHUtlSpsRelInd */

#endif /* LTEMAC_SPS */
#ifdef RG_PHASE_2
/**
 * @brief This API is invoked to send uplink group power control request to PHY.
 *
 * @details
 *
 *     Function : rgSCHUtlTfuGrpPwrCntrlReq
 *
 *      This API is invoked to send uplink group power control request to PHY.
 *      It fills in the Pst structure, spId value and invokes group power
 *      control request primitive at TFU.
 *
 *  @param[in]  TfuGrpPwrCntrlReqInfo *grpPwrCntrlReq
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlTfuGrpPwrCntrlReq
(
Inst                  inst,
S16                   sapId,
TfuGrpPwrCntrlReqInfo *grpPwrCntrlReq
)
#else
PUBLIC S16 rgSCHUtlTfuGrpPwrCntrlReq(inst, sapId, grpPwrCntrlReq)
Inst                  inst;
S16                   sapId;
TfuGrpPwrCntrlReqInfo *grpPwrCntrlReq;
#endif
{
   S16             ret;
   RgSchLowSapCb  *tfuSap;
   Pst             pst;

   TRC2(rgSCHUtlTfuGrpPwrCntrlReq);

   /* Get the lower SAP control block from the layer control block. */
   tfuSap = &(rgSchCb[inst].tfuSap[sapId]);
   if (tfuSap->sapSta.sapState != LRG_BND)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"rgSCHUtlTfuGrpPwrCntrlReq() Lower SAP not bound (%d) \n",
            tfuSap->sapSta.sapState));
      RETVALUE(RFAILED);
   }
   cmMemcpy ((U8*)&pst, (U8*)&(tfuSap->sapCfg.sapPst), sizeof(Pst));
   if((ret = RgLiTfuGrpPwrCntrlReq (&pst, tfuSap->sapCfg.spId, grpPwrCntrlReq)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"rgSCHUtlTfuGrpPwrCntrlReq() Call to RgLiTfuGrpPwrCntrlReq() failed\n"));
   }
   RETVALUE(ret);
}  /* rgSCHUtlTfuGrpPwrCntrlReq */
#endif

/**
 * @brief This API is invoked to send Control Info to PHY.
 *
 * @details
 *
 *     Function : rgSCHUtlTfuCntrlReq
 *
 *      This API is invoked to send Control Info to PHY. It
 *      fills in the Pst structure, spId value and invokes Cntrl
 *      request primitive at TFU.
 *
 *  @param[in]  TfuCntrlReqInfo *cntrlReq
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlTfuCntrlReq
(
Inst            inst,
S16             sapId,
TfuCntrlReqInfo *cntrlReq
)
#else
PUBLIC S16 rgSCHUtlTfuCntrlReq(inst, sapId, cntrlReq)
Inst            inst;
S16             sapId;
TfuCntrlReqInfo *cntrlReq;
#endif
{
   S16            ret;
   RgSchLowSapCb  *tfuSap;

   TRC2(rgSCHUtlTfuCntrlReq)

   /* Get the lower SAP control block from the layer control block. */
   tfuSap = &(rgSchCb[inst].tfuSap[sapId]);

   /* rg005.201 Removing SAP validation for data performance path */
#ifndef NO_ERRCLS
   if (tfuSap->sapSta.sapState != LRG_BND)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"rgSCHUtlTfuCntrlReq() Lower SAP not bound (%d) \n",
            tfuSap->sapSta.sapState));
      cmFreeMem((Ptr)cntrlReq);
      RETVALUE(RFAILED);
   }
#endif

  /* Using local variable for pst is unnecessary - for optimization */
   if((ret = RgLiTfuCntrlReq(&tfuSap->sapCfg.sapPst, tfuSap->sapCfg.spId,
                             cntrlReq)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"rgSCHUtlTfuCntrlReq() Call to RgLiTfuCntrlReq() failed\n"));
   }
   RETVALUE(ret);
}  /* rgSCHUtlTfuCntrlReq*/


/* FOR ACK NACK REP */

/**
 * @brief This API is invoked to tell the DL Scheduler to add the UE back into
 * its scheduling queues.
 *
 * @details
 *
 *     Function : rgSCHUtlDlActvtUe
 *
 *      This API is invoked from Measurement gap moduled.
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgSchUeCb        *ueCb
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlDlActvtUe
(
RgSchCellCb *cell,
RgSchUeCb   *ue
)
#else
PUBLIC S16 rgSCHUtlDlActvtUe(cell, ue)
RgSchCellCb *cell;
RgSchUeCb   *ue;
#endif
{
   TRC2(rgSCHUtlDlActvtUe);
   cell->sc.apis->rgSCHActvtDlUe(cell, ue);
   RETVALUE(ROK);
}

/**
 * @brief This API is invoked to tell the UL Scheduler to add the UE back into
 * its scheduling queues.
 *
 * @details
 *
 *     Function : rgSCHUtlUlActvtUe
 *
 *      This API is invoked from Measurement gap moduled.
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgSchUeCb        *ueCb
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUlActvtUe
(
RgSchCellCb *cell,
RgSchUeCb   *ue
)
#else
PUBLIC S16 rgSCHUtlUlActvtUe(cell, ue)
RgSchCellCb *cell;
RgSchUeCb   *ue;
#endif
{
   TRC2(rgSCHUtlUlActvtUe);
   cell->sc.apis->rgSCHActvtUlUe(cell, ue);
   RETVALUE(ROK);
}

/**
 * @brief This API is invoked to send Reception Request Info to PHY.
 *
 * @details
 *
 *     Function : rgSCHUtlTfuRecpReq
 *
 *      This API is invoked to send Reception Request Info to PHY. It
 *      fills in the Pst structure, spId value and invokes Reception
 *      request primitive at TFU.
 *
 *  @param[in]  TfuRecpReqInfo *recpReq
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlTfuRecpReq
(
Inst            inst,
S16             sapId,
TfuRecpReqInfo *recpReq
)
#else
PUBLIC S16 rgSCHUtlTfuRecpReq(inst, sapId, recpReq)
Inst            inst;
S16             sapId;
TfuRecpReqInfo *recpReq;
#endif
{
   S16            ret;
   RgSchLowSapCb  *tfuSap;

   TRC2(rgSCHUtlTfuRecpReq)

   /* Get the lower SAP control block from the layer control block. */
   tfuSap = &(rgSchCb[inst].tfuSap[sapId]);

   /* rg005.201 Removing SAP validation for data performance path */
#ifndef NO_ERRCLS
   if (tfuSap->sapSta.sapState != LRG_BND)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"rgSCHUtlTfuRecpReq() Lower SAP not bound (%d) \n",
            tfuSap->sapSta.sapState));
      cmFreeMem((Ptr)recpReq);
      RETVALUE(RFAILED);
   }
#endif

  /* Using local variable for pst is unnecessary - for optimization */
   if((ret = RgLiTfuRecpReq(&tfuSap->sapCfg.sapPst, tfuSap->sapCfg.spId,
                            recpReq)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"rgSCHUtlTfuRecpReq() Call to RgLiTfuRecpReq() failed\n"));
   }
   RETVALUE(ret);
}  /* rgSCHUtlTfuRecpReq */

 /** @brief This function Validates the SAP information received along with the
  * primitive from the lower layer.
  *
  * Function: rgSCHUtlValidateTfuSap
  *
  *                      Validates SAP information.
  * @param  suId The SAP Id
  * @return
  *   -# ROK
  *   -# RFAILED
  */
#ifdef ANSI
PUBLIC S16 rgSCHUtlValidateTfuSap
(
Inst  inst,
SuId  suId
)
#else
PUBLIC S16 rgSCHUtlValidateTfuSap(inst, suId)
Inst  inst;
SuId  suId;
#endif
{
   RgSchLowSapCb  *tfuSap;

   TRC2(rgSCHUtlValidateTfuSap)

   if(suId >= rgSchCb[inst].numSaps)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst),"Incorrect SuId"));
      RETVALUE(RFAILED);
   }
   tfuSap = &(rgSchCb[inst].tfuSap[suId]);

   /* First lets check the suId */
   if( suId != tfuSap->sapCfg.suId)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"Incorrect SuId. Configured (%d) Recieved (%d)\n",
            tfuSap->sapCfg.suId, suId));
      RETVALUE(RFAILED);
   }
   if (tfuSap->sapSta.sapState != LRG_BND)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"Lower SAP not enabled SuId (%d)\n",
            tfuSap->sapCfg.suId));
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of rgSCHUtlValidateTfuSap */

/*
*
*       Fun:   rgSCHUtlAllocEventMem
*
*       Desc:  This function allocates event memory
*
*       Ret:   ROK      - on success
*              RFAILED  - on failure
*
*       Notes: None
*
*       File:  rg_utl.c
*
*/
#ifdef ANSI
PUBLIC S16 rgSCHUtlAllocEventMem
(
Inst      inst,
Ptr       *memPtr,
Size      memSize
)
#else
PUBLIC S16 rgSCHUtlAllocEventMem(inst, memPtr, memSize)
Inst      inst;
Ptr       *memPtr;
Size      memSize;
#endif
{
   Mem   sMem;
#ifdef MSPD_MLOG_NEW
   VOLATILE U32 t=0, t2=0;
#endif

   TRC2(rgSCHUtlAllocEventMem)

   sMem.region = rgSchCb[inst].rgSchInit.region;
   sMem.pool = rgSchCb[inst].rgSchInit.pool;

#if (ERRCLASS & ERRCLS_DEBUG)
   if (memSize<= 0)
   {
      RGSCHLOGERROR(inst, ERRCLS_INT_PAR, ERG022, memSize,
                   "rgAllocEventMem(): memSize invalid\n");
      RETVALUE (RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */
#ifdef MSPD_MLOG_NEW
   t=GetTIMETICK();
#endif
   if(ROK != cmAllocEvnt(memSize, TFU_MAX_MEMBLK_SIZE, &sMem, memPtr))
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"rgSCHUtlAllocEventMem(): cmAllocEvnt Failed.\n"))
      RETVALUE(RFAILED);
   }
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_SCHUTL_CMALLCEVT, RESOURCE_LARM, t, t2=GetTIMETICK());
#endif
   RETVALUE(ROK);
} /* end of rgSCHUtlAllocEventMem*/

/*
*
*       Fun:   rgGetEventMem
*
*       Desc:  This function allocates event memory
*
*       Ret:   ROK      - on success
*              RFAILED  - on failure
*
*       Notes: None
*
*       File:  rg_utl.c
*
*/
#ifdef ANSI
PUBLIC S16 rgSCHUtlGetEventMem
(
Ptr       *ptr,
Size      len,
Ptr       memCp
)
#else
PUBLIC S16 rgSCHUtlGetEventMem(ptr, len, memCp)
Ptr       *ptr;
Size      len;
Ptr       memCp;
#endif
{
   S16   ret;

   TRC2(rgSCHUtlGetEventMem)
   ret = cmGetMem(memCp, len, (Ptr *)ptr);
   RETVALUE(ret);
} /* end of rgSCHUtlGetEventMem*/

#ifdef LTE_TDD


/**
 * @brief Handler to allocate memory for ACK/NACk feedback information
 *
 * @details
 *
 *     Function : rgSCHUtlAllocUeANFdbkInfo
 *
 *   It allocates memory for the UE related ACK NACK information.
 *
 *  @param[in]  RgSchUeCb           *ue
 *  @return     S16
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlAllocUeANFdbkInfo
(
RgSchUeCb            *ue
)
#else
PUBLIC S16 rgSCHUtlAllocUeANFdbkInfo(ue)
RgSchUeCb            *ue;
#endif
{
   U8 idx;

   TRC2(rgSCHUtlAllocUeANFdbkInfo);

   if (rgSCHUtlAllocSBuf(ue->cell->instIdx,
               (Data **) &(ue->dl.anInfo), sizeof(RgSchTddANInfo) * \
               rgSchTddANFdbkMapTbl[ue->cell->ulDlCfgIdx]) != ROK)
   {
      RETVALUE(RFAILED);
   }

   for(idx=0; idx < rgSchTddANFdbkMapTbl[ue->cell->ulDlCfgIdx]; idx++)
   {
      rgSCHUtlInitUeANFdbkInfo(&ue->dl.anInfo[idx]);
   }

   /* Set it to the first index */
   ue->dl.nextFreeANIdx = 0;
   RETVALUE(ROK);
} /* rgSCHUtlAllocUeANFdbkInfo */

/**
 * @brief Handler to release memory for ACK/NACk feedback information
 *
 * @details
 *
 *     Function : rgSCHUtlDelUeANFdbkInfo
 *
 *   It releases memory for the UE related ACK NACK information.
 *
 *  @param[in]  RgSchUeCb           *ue
 *  @return     Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlDelUeANFdbkInfo
(
RgSchUeCb            *ue
)
#else
PUBLIC Void rgSCHUtlDelUeANFdbkInfo(ue)
RgSchUeCb            *ue;
#endif
{
   TRC2(rgSCHUtlDelUeANFdbkInfo);

   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(ue->cell->instIdx,
         (Data **)(&( ue->dl.anInfo)), sizeof(RgSchTddANInfo) * \
                        rgSchTddANFdbkMapTbl[ue->cell->ulDlCfgIdx]);

   RETVOID;
} /* rgSCHUtlDelUeANFdbkInfo */

/**
 * @brief Handler to initialise UE ACK/NACk feedback information
 *
 * @details
 *
 *     Function : rgSCHUtlInitUeANFdbkInfo
 *
 *   It initialises UE related ACK NACK information.
 *
 *  @param[in]  RgSchTddANInfo   *anFdInfo
 *  @return     S16
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlInitUeANFdbkInfo
(
RgSchTddANInfo       *anFdInfo
)
#else
PUBLIC S16 rgSCHUtlInitUeANFdbkInfo(anFdInfo)
RgSchTddANInfo       *anFdInfo;
#endif
{
   TRC2(rgSCHUtlInitUeANFdbkInfo);

   anFdInfo->sfn = RGSCH_MAX_SFN+1; /* defensively setting invalid sfn */
   anFdInfo->subframe = 0;
   anFdInfo->ulDai = RG_SCH_MAX_DAI_IDX;
   anFdInfo->dlDai = RG_SCH_MAX_DAI_IDX;
   anFdInfo->latestMIdx = 0;

   RETVALUE(ROK);
} /* rgSCHUtlInitUeANFdbkInfo */

/**
 * @brief Handler to get UE related ACK NACK feedback information
 *
 * @details
 *
 *     Function : rgSCHUtlGetUeANFdbkInfo
 *
 *   It gets the UE related ACK NACK information based on
 *   SFN and subframe number.
 *
 *  @param[in]  RgSchUeCb        *ueCb
 *  @param[in]  CmLteTimingInfo  *time
 *  @return     RgSchTddANInfo*
 **/
#ifdef ANSI
PUBLIC RgSchTddANInfo* rgSCHUtlGetUeANFdbkInfo
(
RgSchUeCb         *ueCb,
CmLteTimingInfo   *timeInfo
)
#else
PUBLIC RgSchTddANInfo* rgSCHUtlGetUeANFdbkInfo(ueCb, timeInfo)
RgSchUeCb         *ueCb;
CmLteTimingInfo   *timeInfo;
#endif
{
   U8 idx;

   TRC2(rgSCHUtlGetUeANFdbkInfo);

   for (idx = 0; idx < rgSchTddANFdbkMapTbl[ueCb->cell->ulDlCfgIdx]; ++idx)
   {
       if( (timeInfo->sfn == ueCb->dl.anInfo[idx].sfn) &&
               (timeInfo->subframe == ueCb->dl.anInfo[idx].subframe))
       {
            RETVALUE(&ueCb->dl.anInfo[idx]);
       }
   }

   RETVALUE(NULLP);
} /* rgSCHUtlGetUeANFdbkInfo */

/**
 * @brief To get downlink subframe index
 *
 * @details
 *
 *     Function: rgSCHUtlGetDlSfIdx
 *     Purpose:  Gets downlink subframe index based on SFN and subframe no
 *
 *  @param[in]  CmLteTimingInfo  *timeInfo
 *  @param[in]  RgSchCellCb         *cell
 *  @return U8
 *
 **/
#ifdef ANSI
PUBLIC U8  rgSCHUtlGetDlSfIdx
(
RgSchCellCb        *cell,
CmLteTimingInfo *timeInfo
)
#else
PUBLIC U8  rgSCHUtlGetDlSfIdx(cell, timeInfo)
RgSchCellCb        *cell;
CmLteTimingInfo *timeInfo;
#endif
{
   /* rg002.201 Fixed wrap around condition for idx for TDD configuration #5 */
   U16       idx = 0;
   TRC2(rgSCHUtlGetDlSfIdx);

   idx = RGSCH_NUM_SUB_FRAMES - \
         rgSchTddNumUlSubfrmTbl[cell->ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
   idx = ((idx * timeInfo->sfn) + \
         rgSchTddNumDlSubfrmTbl[cell->ulDlCfgIdx][timeInfo->subframe]) - 1;
   idx = idx % cell->numDlSubfrms;

   RETVALUE((U8)idx);
}

/**
 * @brief To get the next downlink subframe
 *
 * @details
 *
 *     Function: rgSCHUtlGetNxtDlSfInfo
 *     Purpose:  Gets next downlink subframe based on current DL subframe
 *
 *  @param[in]  CmLteTimingInfo  curDlTime
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchDlSf        *dlSf
 *  @param[in]  RgSchDlSf        **nxtDlsf
 *  @param[in]  CmLteTimingInfo  *nxtDlTime
 *  @return U8
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlGetNxtDlSfInfo
(
CmLteTimingInfo   curDlTime,
RgSchCellCb       *cell,
RgSchDlSf         *dlSf,
RgSchDlSf         **nxtDlsf,
CmLteTimingInfo   *nxtDlTime
)
#else
PUBLIC Void rgSCHUtlGetNxtDlSfInfo(curDlTime, cell, dlSf, nxtDlsf, nxtDlTime)
CmLteTimingInfo   curDlTime;
RgSchCellCb       *cell;
RgSchDlSf         *dlSf;
RgSchDlSf         **nxtDlsf;
CmLteTimingInfo   *nxtDlTime;
#endif
{
   U16  idx = curDlTime.subframe;
   U8   count = 0;
   TRC2(rgSCHUtlGetNxtDlSfInfo);

   while(TRUE)
   {
      do
      {
         idx = (idx + 1) % RGSCH_NUM_SUB_FRAMES;
         count++;
      }while(rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx][idx]
                                       != RG_SCH_TDD_DL_SUBFRAME);
      RGSCHADDTOCRNTTIME(curDlTime, (*nxtDlTime), count);
      *nxtDlsf = rgSCHUtlSubFrmGet(cell, *nxtDlTime);
      if(dlSf->dlFdbkInfo.subframe != (*nxtDlsf)->dlFdbkInfo.subframe)
      {
         break;
      }
   }
   RETVOID;
}

/**
 * @brief To get the previous downlink subframe
 *
 * @details
 *
 *     Function: rgSCHUtlGetPrevDlSfInfo
 *     Purpose:  Gets previous downlink subframe based on current DL subframe
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  CmLteTimingInfo  curDlTime
 *  @param[in]  CmLteTimingInfo  *prevDlTime
 *  @param[in]  U8               *numSubfrm
 *  @return U8
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHUtlGetPrevDlSfInfo
(
RgSchCellCb       *cell,
CmLteTimingInfo   curDlTime,
CmLteTimingInfo   *prevDlTime,
U8                *numSubfrm
)
#else
PUBLIC Void rgSCHUtlGetPrevDlSfInfo(cell, curDlTime, prevDlTime, numSubfrm)
RgSchCellCb       *cell;
CmLteTimingInfo   curDlTime;
CmLteTimingInfo   *prevDlTime;
U8                *numSubfrm;
#endif
{
   S16 idx = curDlTime.subframe;
   U8  count = 0;
   TRC2(rgSCHUtlGetPrevDlSfInfo);

   do
   {
      idx--;
      if(idx < 0)
      {
         idx = RGSCH_NUM_SUB_FRAMES-1;
      }
      count++;
   }while(rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx][idx]
         !=  RG_SCH_TDD_DL_SUBFRAME);
   *numSubfrm = count;
   RGSCHDECRFRMCRNTTIME(curDlTime, (*prevDlTime), count);
   RETVOID;
}

#endif
/*rg003.201: Added Holes Management functions for Adaptive Re transmission */
/******* </AllocHolesMemMgmnt>: START *****/
/***********************************************************
 *
 *     Func : rgSCHUtlUlSfInit
 *
 *     Desc : UL subframe init.
 *
 *     Ret  : S16
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUlSfInit
(
RgSchCellCb  *cell,
RgSchUlSf    *sf,
U8           idx,
U8           maxUePerSf,
U8           sbStartIdx,
U8           numSb
)
#else
PUBLIC S16 rgSCHUtlUlSfInit(cell, sf, idx, maxUePerSf, sbStartIdx, numSb)
RgSchCellCb  *cell;
RgSchUlSf    *sf;
U8           idx;
U8           maxUePerSf;
U8           sbStartIdx;
U8           numSb;
#endif
{
   S16             ret=ROK;
   TRC2(rgSCHUtlUlSfInit);

   sf->idx = idx;
   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&sf->allocDb,
                           sizeof(RgSchUlAllocDb));
   if (ret != ROK)
   {
      RETVALUE(ret);
   }
   ret = rgSCHUtlUlAllocDbInit(cell, sf->allocDb, maxUePerSf);
   if (ret != ROK)
   {
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(sf->allocDb)),
                       sizeof(RgSchUlAllocDb));
      RETVALUE(ret);
   }
   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&sf->holeDb,
         sizeof(RgSchUlHoleDb));
   if (ret != ROK)
   {
      rgSCHUtlUlAllocDbDeinit(cell, sf->allocDb);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(sf->allocDb)),
                       sizeof(RgSchUlAllocDb));
      RETVALUE(ret);
   }
   ret = rgSCHUtlUlHoleDbInit(cell, sf->holeDb, (U8)(maxUePerSf + 1), \
                              sbStartIdx, numSb);

   if (ret != ROK)
   {
      rgSCHUtlUlAllocDbDeinit(cell, sf->allocDb);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(sf->allocDb)),
                       sizeof(RgSchUlAllocDb));
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(sf->holeDb)),
                       sizeof(RgSchUlHoleDb));
      RETVALUE(ret);
   }
   cmLListInit(&sf->reTxLst);
#ifdef MSPD
   /* MS_FIX ccpu00120610*/
   sf->allocCountRef = &sf->allocDb->count;
#endif
   RETVALUE(ret);
}


/***********************************************************
 *
 *     Func : rgSCHUtlUlSfDeinit
 *
 *     Desc : Deinitialises a subframe
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlSfDeinit
(
RgSchCellCb  *cell,
RgSchUlSf    *sf
)
#else
PUBLIC Void rgSCHUtlUlSfDeinit(cell, sf)
RgSchCellCb  *cell;
RgSchUlSf    *sf;
#endif
{
   TRC2(rgSCHUtlUlSfDeinit);
   if (sf->allocDb)
   {
      rgSCHUtlUlAllocDbDeinit(cell, sf->allocDb);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(sf->allocDb)),
                       sizeof(RgSchUlAllocDb));
   }
   if (sf->holeDb)
   {
      rgSCHUtlUlHoleDbDeinit(cell, sf->holeDb);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(sf->holeDb)),
                       sizeof(RgSchUlHoleDb));
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocDbInit
 *
 *     Desc : Initialise allocation DB
 *
 *     Ret  : S16 (ROK/RFAILED)
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHUtlUlAllocDbInit
(
RgSchCellCb    *cell,
RgSchUlAllocDb *allocDb,
U8             maxAllocs
)
#else
PRIVATE S16 rgSCHUtlUlAllocDbInit(cell, allocDb, maxAllocs)
RgSchCellCb    *cell;
RgSchUlAllocDb *allocDb;
U8             maxAllocs;
#endif
{
   S16 ret = rgSCHUtlUlAllocMemInit(cell, &allocDb->mem, maxAllocs);
   TRC2(rgSCHUtlUlAllocDbInit);
   if (ret != ROK)
   {
      RETVALUE(ret);
   }
   allocDb->count = 0;
   allocDb->first = NULLP;
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocDbDeinit
 *
 *     Desc : Deinitialises allocation DB
 *            sent to UE, for a UE with accumulation disabled
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHUtlUlAllocDbDeinit
(
RgSchCellCb     *cell,
RgSchUlAllocDb *allocDb
)
#else
PRIVATE Void rgSCHUtlUlAllocDbDeinit(cell, allocDb)
RgSchCellCb     *cell;
RgSchUlAllocDb *allocDb;
#endif
{
   TRC2(rgSCHUtlUlAllocDbDeinit);
   rgSCHUtlUlAllocMemDeinit(cell, &allocDb->mem);
   allocDb->count = 0;
   allocDb->first = NULLP;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleDbInit
 *
 *     Desc : Initialise hole DB
 *
 *     Ret  : S16 (ROK/RFAILED)
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHUtlUlHoleDbInit
(
RgSchCellCb   *cell,
RgSchUlHoleDb *holeDb,
U8            maxHoles,
U8            start,
U8            num
)
#else
PRIVATE S16 rgSCHUtlUlHoleDbInit(cell, holeDb, maxHoles, start, num)
RgSchCellCb   *cell;
RgSchUlHoleDb *holeDb;
U8            maxHoles;
U8            start;
U8            num;
#endif
{
   S16 ret;
   RgSchUlHole *hole;
   TRC2(rgSCHUtlUlHoleDbInit);

   ret = rgSCHUtlUlHoleMemInit(cell, &holeDb->mem, maxHoles, &hole);
   if (ret != ROK)
   {
      RETVALUE(ret);
   }
   holeDb->count = 1;
   holeDb->first = hole;
   hole->start = start;
   hole->num = num;
   hole->prv = hole->nxt = NULLP;
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleDbDeinit
 *
 *     Desc : Deinitialises hole DB
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHUtlUlHoleDbDeinit
(
RgSchCellCb   *cell,
RgSchUlHoleDb *holeDb
)
#else
PRIVATE Void rgSCHUtlUlHoleDbDeinit(cell, holeDb)
RgSchCellCb   *cell;
RgSchUlHoleDb *holeDb;
#endif
{
   TRC2(rgSCHUtlUlHoleDbDeinit);
   rgSCHUtlUlHoleMemDeinit(cell, &holeDb->mem);
   holeDb->count = 0;
   holeDb->first = NULLP;
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocGetHole
 *
 *     Desc : Get allocation from hole
 *
 *     Ret  : RgSchUlAlloc *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetHole
(
RgSchUlSf   *sf,
U8       numSb,
RgSchUlHole *hole
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetHole(sf, numSb, hole)
RgSchUlSf   *sf;
U8       numSb;
RgSchUlHole *hole;
#endif
{
   TRC2(rgSCHUtlUlAllocGetHole);
   if (numSb < hole->num)
   {
      RETVALUE(rgSCHUtlUlAllocGetPartHole(sf, numSb, hole));
   }
   else
   {
      RETVALUE(rgSCHUtlUlAllocGetCompHole(sf, hole));
   }
}


/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocGetCompHole
 *
 *     Desc : Get an allocation corresponding to an entire hole
 *
 *     Ret  : RgSchUlAlloc *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetCompHole
(
RgSchUlSf   *sf,
RgSchUlHole *hole
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetCompHole(sf, hole)
RgSchUlSf   *sf;
RgSchUlHole *hole;
#endif
{
   RgSchUlAlloc *alloc;
      /* alloc = rgSCHUtlUlAllocGetAndIns(sf->allocDb, hole->prvAlloc, hole->nxtAlloc); */
   /* Calling rgSchCmnUlAllocGetAndIns is ok, but prv alloc needs to have nxtHole
    * updated, causing another check for prv */
   RgSchUlAlloc *prv = hole->prvAlloc;
   RgSchUlAlloc *nxt = hole->nxtAlloc;
   TRC2(rgSCHUtlUlAllocGetCompHole);

   if (prv)
   {
      prv->nxtHole = NULLP;
      alloc = rgSCHUtlUlAllocGetAdjNxt(sf->allocDb, prv);
   }
   else
   {
      alloc = rgSCHUtlUlAllocGetFirst(sf->allocDb);
   }

   alloc->prvHole = NULLP;
   alloc->nxtHole = NULLP;

   alloc->sbStart = hole->start;
   alloc->numSb = hole->num;

   if (nxt)
   {
      nxt->prvHole = NULLP;
   }

   rgSCHUtlUlHoleRls(sf->holeDb, hole);

   RETVALUE(alloc);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocGetPartHole
 *
 *     Desc : Get an allocation corresponding to a part of a hole.
 *            The initial 'numSb' part of the hole shall be taken
 *            away for this alloc.
 *
 *     Ret  : RgSchUlAlloc *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetPartHole
(
RgSchUlSf   *sf,
U8       numSb,
RgSchUlHole *hole
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetPartHole(sf, numSb, hole)
RgSchUlSf   *sf;
U8       numSb;
RgSchUlHole *hole;
#endif
{
   RgSchUlAlloc *alloc;
      /* alloc = rgSCHUtlUlAllocGetAndIns(sf->allocDb, hole->prvAlloc, hole->nxtAlloc); */
   /* Calling rgSchCmnUlAllocGetAndIns is ok, but prv alloc needs to have nxtHole
    * updated, causing another check for prv */
   RgSchUlAlloc *prv = hole->prvAlloc;
   TRC2(rgSCHUtlUlAllocGetPartHole);

   if (prv)
   {
      prv->nxtHole = NULLP;
      alloc = rgSCHUtlUlAllocGetAdjNxt(sf->allocDb, prv);
   }
   else
   {
      alloc = rgSCHUtlUlAllocGetFirst(sf->allocDb);
   }

   alloc->prvHole = NULLP;
   alloc->nxtHole = hole;
   hole->prvAlloc = alloc;

   alloc->sbStart = hole->start;
   alloc->numSb = numSb;
   hole->start += numSb;
   hole->num -= numSb;

   rgSCHUtlUlHoleDecr(sf->holeDb, hole);

   RETVALUE(alloc);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocFirst
 *
 *     Desc : Get first alloc in subframe
 *
 *     Ret  : RgSchUlAlloc *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocFirst
(
RgSchUlSf *sf
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocFirst(sf)
RgSchUlSf *sf;
#endif
{
   TRC2(rgSCHUtlUlAllocFirst);
   RETVALUE(sf->allocDb->first);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocNxt
 *
 *     Desc : Get next alloc
 *
 *     Ret  : RgSchUlAlloc *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocNxt
(
RgSchUlSf    *sf,
RgSchUlAlloc *alloc
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocNxt(sf, alloc)
RgSchUlSf    *sf;
RgSchUlAlloc *alloc;
#endif
{
   TRC2(rgSCHUtlUlAllocNxt);
   UNUSED(sf);
   RETVALUE(alloc->nxt);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocGetAdjNxt
 *
 *     Desc : Get alloc which is immediately after the passed one.
 *            1. Gets alloc from mem.
 *            2. Inserts alloc into list (between prv and
 *                prv->nxt, prv is not NULLP).
 *            3. Increments alloc count.
 *            Note 1: Holes are not dealt with here.
 *            Note 2: Assumes prv to be NULL.
 *
 *     Ret  : RgSchUlAlloc *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetAdjNxt
(
RgSchUlAllocDb *db,
RgSchUlAlloc   *prv
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetAdjNxt(db, prv)
RgSchUlAllocDb *db;
RgSchUlAlloc   *prv;
#endif
{
   RgSchUlAlloc *alloc = rgSCHUtlUlAllocMemGet(&db->mem);
   RgSchUlAlloc *nxt = prv->nxt;
   TRC2(rgSCHUtlUlAllocGetAdjNxt);

#if (ERRCLASS & ERRCLS_DEBUG)
   if ( alloc == NULLP )
   {
       RETVALUE ( NULLP );
   }
#endif
   alloc->prv = prv;
   alloc->nxt = nxt;
   prv->nxt = alloc;
   if (nxt)
   {
      nxt->prv = alloc;
   }

   ++db->count;

   RETVALUE(alloc);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocGetFirst
 *
 *     Desc : Get alloc which is to be the first one in the alloc list
 *            1. Gets alloc from mem.
 *            2. Inserts alloc as first element into list.
 *            3. Increments alloc count.
 *            Note 1: Holes are not dealt with here.
 *            Note 2: prv to necessarily NULLP.
 *
 *     Ret  : RgSchUlAlloc *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetFirst
(
RgSchUlAllocDb *db
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocGetFirst(db)
RgSchUlAllocDb *db;
#endif
{
   RgSchUlAlloc *alloc = rgSCHUtlUlAllocMemGet(&db->mem);
   RgSchUlAlloc *nxt = db->first;
   TRC2(rgSCHUtlUlAllocGetFirst);

#if (ERRCLASS & ERRCLS_DEBUG)
    if ( alloc == NULLP )
    {
       RETVALUE ( NULLP );
    }
#endif

   alloc->prv = NULLP;
   alloc->nxt = nxt;
   if (nxt)
   {
      nxt->prv = alloc;
   }
   db->first = alloc;

   ++db->count;

   RETVALUE(alloc);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocRls
 *
 *     Desc : Releases an uplink allocation
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlAllocRls
(
RgSchUlSf    *sf,
RgSchUlAlloc *alloc
)
#else
PUBLIC Void rgSCHUtlUlAllocRls(sf, alloc)
RgSchUlSf    *sf;
RgSchUlAlloc *alloc;
#endif
{
   RgSchUlAllocDb *allocDb = sf->allocDb;
   RgSchUlAlloc   *prv = alloc->prv;
   RgSchUlAlloc   *nxt = alloc->nxt;
   TRC2(rgSCHUtlUlAllocRls);

   if (prv)
   {
      prv->nxt = nxt;
      if (nxt)           /* general case: this allocation lies btw two */
      {
         nxt->prv = prv;
      }
   }
   else
   {
      allocDb->first = nxt;
      if (nxt)
      {
         nxt->prv = NULLP;
      }
   }
   --allocDb->count;
   rgSCHUtlUlHoleAddAlloc(sf, alloc);
   rgSCHUtlUlAllocMemRls(&allocDb->mem, alloc);

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleFirst
 *
 *     Desc : Get first (largest) hole
 *
 *     Ret  : RgSchUlHole *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlHole *rgSCHUtlUlHoleFirst
(
RgSchUlSf *sf
)
#else
PUBLIC RgSchUlHole *rgSCHUtlUlHoleFirst(sf)
RgSchUlSf *sf;
#endif
{
   TRC2(rgSCHUtlUlHoleFirst);
   RETVALUE(sf->holeDb->first);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleNxt
 *
 *     Desc : Get next largest hole
 *
 *     Ret  : RgSchUlHole *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlHole *rgSCHUtlUlHoleNxt
(
RgSchUlSf   *sf,
RgSchUlHole *hole
)
#else
PUBLIC RgSchUlHole *rgSCHUtlUlHoleNxt(sf, hole)
RgSchUlSf   *sf;
RgSchUlHole *hole;
#endif
{
   TRC2(rgSCHUtlUlHoleNxt);
   UNUSED(sf);
   RETVALUE(hole->nxt);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleAddAlloc
 *
 *     Desc : On freeing an alloc, add to hole
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleAddAlloc
(
RgSchUlSf    *sf,
RgSchUlAlloc *alloc
)
#else
PUBLIC Void rgSCHUtlUlHoleAddAlloc(sf, alloc)
RgSchUlSf    *sf;
RgSchUlAlloc *alloc;
#endif
{
   /* Note: rgSchCmnUlHoleUpdAllocLnks function that is used should not exist as
    * one, if such excessive branching is done (AllocNone, AllocNoPrv etc).
    * The excessive branching is meant to utilise the knowledge of whether prv
    * and nxt allocs exist or not. Hence for each kind (none, noprv, nonxt,
    * both), there should be a rgSchCmnUlHoleUpdAllocLnks... function (such as
    * rgSchCmnUlHoleUpdAllocLnksNone/NoPrv etc. */
   RgSchUlHoleDb *db = sf->holeDb;
   RgSchUlHole *prv = alloc->prvHole;
   RgSchUlHole *nxt = alloc->nxtHole;
   TRC2(rgSCHUtlUlHoleAddAlloc);

   if (prv)
   {
      if (nxt)
      {
         rgSCHUtlUlHoleJoin(db, prv, nxt, alloc);
      }
      else
         rgSCHUtlUlHoleExtndRight(db, prv, alloc);
   }
   else
   {
      if (nxt)
      {
         rgSCHUtlUlHoleExtndLeft(db, nxt, alloc);
      }
      else
         rgSCHUtlUlHoleNew(db, alloc);
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleJoin
 *
 *     Desc : Join two holes (due to alloc being deleted)
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleJoin
(
RgSchUlHoleDb *db,
RgSchUlHole   *prv,
RgSchUlHole   *nxt,
RgSchUlAlloc  *alloc
)
#else
PUBLIC Void rgSCHUtlUlHoleJoin(db, prv, nxt, alloc)
RgSchUlHoleDb *db;
RgSchUlHole   *prv;
RgSchUlHole   *nxt;
RgSchUlAlloc  *alloc;
#endif
{
   TRC2(rgSCHUtlUlHoleJoin);
   prv->num += alloc->numSb + nxt->num;
   rgSCHUtlUlHoleRls(db, nxt);
   rgSCHUtlUlHoleIncr(db, prv);
   rgSCHUtlUlHoleUpdAllocLnks(prv, alloc->prv, alloc->nxt);

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleExtndRight
 *
 *     Desc : Extend hole due to alloc coming 'after' the hole
 *            being deleted
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleExtndRight
(
RgSchUlHoleDb *db,
RgSchUlHole   *prv,
RgSchUlAlloc  *alloc
)
#else
PUBLIC Void rgSCHUtlUlHoleExtndRight(db, prv, alloc)
RgSchUlHoleDb *db;
RgSchUlHole   *prv;
RgSchUlAlloc  *alloc;
#endif
{
   TRC2(rgSCHUtlUlHoleExtndRight);
   prv->num += alloc->numSb;
   rgSCHUtlUlHoleIncr(db, prv);
   rgSCHUtlUlHoleUpdAllocLnks(prv, alloc->prv, alloc->nxt);
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleExtndLeft
 *
 *     Desc : Extend hole due to alloc coming 'before' the hole
 *            being deleted
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleExtndLeft
(
RgSchUlHoleDb *db,
RgSchUlHole   *nxt,
RgSchUlAlloc  *alloc
)
#else
PUBLIC Void rgSCHUtlUlHoleExtndLeft(db, nxt, alloc)
RgSchUlHoleDb *db;
RgSchUlHole   *nxt;
RgSchUlAlloc  *alloc;
#endif
{
   TRC2(rgSCHUtlUlHoleExtndLeft);
   nxt->num += alloc->numSb;
   nxt->start = alloc->sbStart;
   rgSCHUtlUlHoleIncr(db, nxt);
   rgSCHUtlUlHoleUpdAllocLnks(nxt, alloc->prv, alloc->nxt);
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleNew
 *
 *     Desc : Create new hole due to alloc being deleted
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleNew
(
RgSchUlHoleDb *db,
RgSchUlAlloc  *alloc
)
#else
PUBLIC Void rgSCHUtlUlHoleNew(db, alloc)
RgSchUlHoleDb *db;
RgSchUlAlloc  *alloc;
#endif
{
   RgSchUlHole *hole = rgSCHUtlUlHoleMemGet(&db->mem);
#if (ERRCLASS & ERRCLS_DEBUG)
   if ( hole == NULLP )
   {
      RETVOID;
   }
#endif
   TRC2(rgSCHUtlUlHoleNew);
   hole->start = alloc->sbStart;
   hole->num = alloc->numSb;
   ++db->count;
   rgSCHUtlUlHoleIns(db, hole);
   rgSCHUtlUlHoleUpdAllocLnks(hole, alloc->prv, alloc->nxt);
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleUpdAllocLnks
 *
 *     Desc : Update alloc links in hole
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleUpdAllocLnks
(
RgSchUlHole  *hole,
RgSchUlAlloc *prvAlloc,
RgSchUlAlloc *nxtAlloc
)
#else
PUBLIC Void rgSCHUtlUlHoleUpdAllocLnks(hole, prvAlloc, nxtAlloc)
RgSchUlHole  *hole;
RgSchUlAlloc *prvAlloc;
RgSchUlAlloc *nxtAlloc;
#endif
{
   TRC2(rgSCHUtlUlHoleUpdAllocLnks);
   if (prvAlloc)
   {
      prvAlloc->nxtHole = hole;
   }
   if (nxtAlloc)
   {
      nxtAlloc->prvHole = hole;
   }
   hole->prvAlloc = prvAlloc;
   hole->nxtAlloc = nxtAlloc;
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleIns
 *
 *     Desc : Insert (newly created) hole in sorted list of holes.
 *            Searches linearly, beginning with the largest hole.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleIns
(
RgSchUlHoleDb *db,
RgSchUlHole   *hole
)
#else
PUBLIC Void rgSCHUtlUlHoleIns(db, hole)
RgSchUlHoleDb *db;
RgSchUlHole   *hole;
#endif
{
   RgSchUlHole *cur;
   TRC2(rgSCHUtlUlHoleIns);

   if ((cur = db->first))
   {
      RgSchUlHole *nxt;
      if (cur->num < hole->num)
      {
         /* Add at front */
         hole->nxt = cur;
         cur->prv = hole;
         db->first = hole;
         hole->prv = NULLP;
         RETVOID;
      }

      for (nxt = cur->nxt; nxt; cur = nxt, nxt = nxt->nxt)
      {
         if (nxt->num < hole->num)
         {
            /* Insert hole:  cur <-> hole <-> nxt */
            cur->nxt = hole;
            hole->prv = cur;
            hole->nxt = nxt;
            nxt->prv = hole;
            RETVOID;
         }
      }

      /* Add at end */
      cur->nxt = hole;
      hole->prv = cur;
      hole->nxt = NULLP;
      RETVOID;
   }

   /* This is the first hole */
   db->first = hole;
   hole->prv = NULLP; /* may not be needed */
   hole->nxt = NULLP;
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleIncr
 *
 *     Desc : hole->num has increeased, reposition in sorted
 *            list if needed
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleIncr
(
RgSchUlHoleDb *db,
RgSchUlHole   *hole
)
#else
PUBLIC Void rgSCHUtlUlHoleIncr(db, hole)
RgSchUlHoleDb *db;
RgSchUlHole   *hole;
#endif
{
   RgSchUlHole *cur;
   TRC2(rgSCHUtlUlHoleIncr);

   if ((cur = hole->prv))
   {
      RgSchUlHole *prv;

      if (cur->num > hole->num)
      {
         RETVOID;
      }

      /* Remove hole from current position */
      cur->nxt = hole->nxt;
      if (hole->nxt)
      {
         hole->nxt->prv = cur;
      }

      for (prv = cur->prv; prv; cur = prv, prv = prv->prv)
      {
         if (prv->num > hole->num)
         {
            /* Insert hole:  prv <-> hole <-> cur */
            prv->nxt = hole;
            hole->prv = prv;
            hole->nxt = cur;
            cur->prv = hole;
            RETVOID;
         }
      }

      /* Add at front */
      hole->nxt = cur;
      cur->prv = hole;
      db->first = hole;
      hole->prv = NULLP;
      RETVOID;
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleDecr
 *
 *     Desc : hole->num has decreeased, reposition in sorted
 *            list if needed
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleDecr
(
RgSchUlHoleDb *db,
RgSchUlHole   *hole
)
#else
PUBLIC Void rgSCHUtlUlHoleDecr(db, hole)
RgSchUlHoleDb *db;
RgSchUlHole   *hole;
#endif
{
   RgSchUlHole *cur;
   TRC2(rgSCHUtlUlHoleDecr);

   if ((cur = hole->nxt))
   {
      RgSchUlHole *nxt;

      if (cur->num < hole->num)
      {
         RETVOID;
      }

      /* Remove hole from current position */
      cur->prv = hole->prv;
      if (hole->prv)
      {
         hole->prv->nxt = cur;
      }
      else /* no prv, so cur to replace hole as first in list */
      {
         db->first = cur;
      }

      for (nxt = cur->nxt; nxt; cur = nxt, nxt = nxt->nxt)
      {
         if (nxt->num < hole->num)
         {
            /* Insert hole:  cur <-> hole <-> nxt */
            cur->nxt = hole;
            hole->prv = cur;
            hole->nxt = nxt;
            nxt->prv = hole;
            RETVOID;
         }
      }

      /* Add at end */
      cur->nxt = hole;
      hole->prv = cur;
      hole->nxt = NULLP;
      RETVOID;
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleRls
 *
 *     Desc : Releases hole.
 *            1. Decrements hole count.
 *            2. Deletes hole from list.
 *            3. Frees hole (hole memory release).
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleRls
(
RgSchUlHoleDb *db,
RgSchUlHole   *hole
)
#else
PUBLIC Void rgSCHUtlUlHoleRls(db, hole)
RgSchUlHoleDb *db;
RgSchUlHole   *hole;
#endif
{
   RgSchUlHole *prv = hole->prv;
   RgSchUlHole *nxt = hole->nxt;
   TRC2(rgSCHUtlUlHoleRls);

   --db->count;
   if (prv)
   {
      prv->nxt = nxt;
      if (nxt)
      {
         nxt->prv = prv;
      }
   }
   else
   {
      db->first = nxt;
      if (nxt)
      {
         nxt->prv = NULLP;
      }
   }

   rgSCHUtlUlHoleMemRls(&db->mem, hole);
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocMemInit
 *
 *     Desc : Initialises alloc free pool
 *
 *     Ret  : S16 (ROK/RFAILED)
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUlAllocMemInit
(
RgSchCellCb     *cell,
RgSchUlAllocMem *mem,
U8           maxAllocs
)
#else
PUBLIC S16 rgSCHUtlUlAllocMemInit(cell, mem, maxAllocs)
RgSchCellCb     *cell;
RgSchUlAllocMem *mem;
U8           maxAllocs;
#endif
{
   S16 ret;
   RgSchUlAlloc *allocs;
   TRC2(rgSCHUtlUlAllocMemInit);

   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&allocs,
                           maxAllocs * sizeof(*allocs));
   if (ret != ROK)
   {
      RETVALUE(ret);
   }
   mem->allocs = allocs;
   mem->maxAllocs = maxAllocs;
   if (mem->maxAllocs == 1)
   {
      allocs[0].prv = NULLP;
      allocs[0].nxt = NULLP;
   }
   else
   {
      U8 i;
      allocs[0].prv = NULLP;
      allocs[0].nxt = &allocs[1];
      for (i = 1; i < mem->maxAllocs - 1; ++i)
      {
         allocs[i].prv = &allocs[i-1];
         allocs[i].nxt = &allocs[i+1];
      }
      allocs[i].prv = &allocs[i-1];
      allocs[i].nxt = NULLP;
   }
   mem->firstFree = &allocs[0];
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocMemDeinit
 *
 *     Desc : Deinitialises alloc free pool
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlAllocMemDeinit
(
RgSchCellCb     *cell,
RgSchUlAllocMem *mem
)
#else
PUBLIC Void rgSCHUtlUlAllocMemDeinit(cell, mem)
RgSchCellCb     *cell;
RgSchUlAllocMem *mem;
#endif
{
   TRC2(rgSCHUtlUlAllocMemDeinit);
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(mem->allocs)),
                    mem->maxAllocs * sizeof(*mem->allocs));
   mem->maxAllocs = 0;
   mem->firstFree = NULLP;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleMemInit
 *
 *     Desc : Initialises hole free pool. Assumes maxHoles
 *            to be at least 2.
 *
 *     Ret  : S16 (ROK/RFAILED)
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlUlHoleMemInit
(
RgSchCellCb    *cell,
RgSchUlHoleMem *mem,
U8          maxHoles,
RgSchUlHole    **holeRef
)
#else
PUBLIC S16 rgSCHUtlUlHoleMemInit(cell, mem, maxHoles, holeRef)
RgSchCellCb     *cell;
RgSchUlHoleMem *mem;
U8          maxHoles;
RgSchUlHole    **holeRef;
#endif
{
   S16 ret;
   RgSchUlHole *holes;
   TRC2(rgSCHUtlUlHoleMemInit);

   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&holes,
                           maxHoles * sizeof(*holes));
   if (ret != ROK)
   {
      RETVALUE(ret);
   }

   mem->holes = holes;
   mem->maxHoles = maxHoles;

   /* first hole is taken up */
   holes[0].prv = NULLP; /* not needed */
   holes[0].nxt = NULLP; /* not needed */
   *holeRef = &holes[0];

   if (mem->maxHoles == 2)
   {
      holes[1].prv = NULLP; /* may not be needed */
      holes[1].nxt = NULLP; /* may not be needed */
   }
   else
   {
      U8 i;
      holes[1].prv = NULLP;
      holes[0].nxt = &holes[1];
      for (i = 1; i < mem->maxHoles - 1; ++i)
      {
         holes[i].prv = &holes[i-1];
         holes[i].nxt = &holes[i+1];
      }
      holes[i].prv = &holes[i-1];
      holes[i].nxt = NULLP;
   }
   mem->firstFree = &holes[1];

   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleMemDeinit
 *
 *     Desc : Deinitialises hole free pool
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleMemDeinit
(
RgSchCellCb    *cell,
RgSchUlHoleMem *mem
)
#else
PUBLIC Void rgSCHUtlUlHoleMemDeinit(cell, mem)
RgSchCellCb    *cell;
RgSchUlHoleMem *mem;
#endif
{
   TRC2(rgSCHUtlUlHoleMemDeinit);
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(mem->holes)),
                    mem->maxHoles * sizeof(*mem->holes));
   mem->maxHoles = 0;
   mem->firstFree = NULLP;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocMemGet
 *
 *     Desc : Gets an 'alloc' from the free pool
 *
 *     Ret  : RgSchUlAlloc *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocMemGet
(
RgSchUlAllocMem *mem
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlAllocMemGet(mem)
RgSchUlAllocMem *mem;
#endif
{
   RgSchUlAlloc *alloc;
   TRC2(rgSCHUtlUlAllocMemGet);

#if (ERRCLASS & ERRCLS_DEBUG)
   if (mem->firstFree == NULLP)
   {
      RETVALUE(NULLP);
   }
#endif

   alloc = mem->firstFree;
#ifdef MSPD
if (!alloc)
{
stop_printf("No free allocation available in UL %0x mem %0x allocs %u firstFree %0x maxAllocs %u\n",
   alloc, mem, mem->allocs, mem->firstFree, mem->maxAllocs);
}
#endif
   mem->firstFree = alloc->nxt;
   /* rg001.101: Removed unnecessary assignment */
   alloc->nxt = NULLP; /* probably not needed */
   /* alloc->prv might already be NULLP, in case was needed to set it to NULLP */

   RETVALUE(alloc);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlAllocMemRls
 *
 *     Desc : Returns an 'alloc' to the free pool
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlAllocMemRls
(
RgSchUlAllocMem *mem,
RgSchUlAlloc    *alloc
)
#else
PUBLIC Void rgSCHUtlUlAllocMemRls(mem, alloc)
RgSchUlAllocMem *mem;
RgSchUlAlloc    *alloc;
#endif
{
   TRC2(rgSCHUtlUlAllocMemRls);
   alloc->prv = NULLP;

   alloc->nxt = mem->firstFree;
   if (mem->firstFree != NULLP)
   {
      mem->firstFree->prv = alloc;
   }
   mem->firstFree = alloc;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleMemGet
 *
 *     Desc : Gets a 'hole' from the free pool
 *
 *     Ret  : RgSchUlHole *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlHole *rgSCHUtlUlHoleMemGet
(
RgSchUlHoleMem *mem
)
#else
PUBLIC RgSchUlHole *rgSCHUtlUlHoleMemGet(mem)
RgSchUlHoleMem *mem;
#endif
{
   RgSchUlHole *hole;
   TRC2(rgSCHUtlUlHoleMemGet);

#if (ERRCLASS & ERRCLS_DEBUG)
   if (mem->firstFree == NULLP)
   {
      RETVALUE(NULLP);
   }
#endif

   hole = mem->firstFree;
   mem->firstFree = hole->nxt;
   mem->firstFree->prv = NULLP; /* may not be needed, under error class */
   hole->nxt = NULLP; /* probably not needed */
   /* hole->prv is might already be NULLP, in case was needed to set it to NULLP */

   RETVALUE(hole);
}

/***********************************************************
 *
 *     Func : rgSCHUtlUlHoleMemRls
 *
 *     Desc : Returns a 'hole' to the free pool
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlUlHoleMemRls
(
RgSchUlHoleMem *mem,
RgSchUlHole    *hole
)
#else
PUBLIC Void rgSCHUtlUlHoleMemRls(mem, hole)
RgSchUlHoleMem *mem;
RgSchUlHole    *hole;
#endif
{
   TRC2(rgSCHUtlUlHoleMemRls);
   hole->prv = NULLP;

   hole->nxt = mem->firstFree;
   if (mem->firstFree != NULLP)
   {
      mem->firstFree->prv = hole;
   }
   mem->firstFree = hole;
   RETVOID;
}

/**
 * @brief Get an alloc from the specified position in the BW.
 *
 * @details
 *
 *     Function : rgSCHUtlUlGetSpfcAlloc
 *
 *      - Return an alloc from the specified position in the BW.
 *        Note: This function assumes there is always a hole
 *              Existing which completely has the specified
 *              allocation. The reason for such an assumption is
 *              the function's usage as of now guarantees that there
 *              will always be such hole. And also for efficiency.
 *
 *  @param[in]  RgSchUlSf     *sf
 *  @param[in]  U8            startSb
 *  @param[in]  U8            numSb
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHUtlUlGetSpfcAlloc
(
RgSchUlSf       *sf,
U8              startSb,
U8              numSb
)
#else
PUBLIC RgSchUlAlloc *rgSCHUtlUlGetSpfcAlloc(sf, startSb, numSb)
RgSchUlSf       *sf;
U8              startSb;
U8              numSb;
#endif
{
   RgSchUlHole     *hole, *nxtHole;
   RgSchUlAlloc    *alloc = NULLP;
   TRC2(rgSCHUtlUlGetSpfcAlloc);

   if (!(hole = rgSCHUtlUlHoleFirst(sf)))
   {
      RETVALUE(NULLP);
   }
   do
   {
      nxtHole = rgSCHUtlUlHoleNxt(sf, hole);
      if ((startSb >= hole->start) &&
          (startSb+numSb <= hole->start+hole->num))
      {
         if (startSb != hole->start)
         {
            /* Create a new hole to accomodate Subbands between
             * hole start and req alloc start */
            RgSchUlHole *newHole = rgSCHUtlUlHoleMemGet(&(sf->holeDb->mem));

#if (ERRCLASS & ERRCLS_DEBUG)
            if ( newHole == NULLP )
            {
                RETVALUE( NULLP );
            }
#endif
            newHole->start = hole->start;
            newHole->num = startSb - hole->start;
            hole->start = startSb;
            hole->num -= (startSb - hole->start);
            ++(sf->holeDb->count);
            rgSCHUtlUlHoleIns(sf->holeDb, newHole);
            newHole->prvAlloc = hole->prvAlloc;
            hole->prvAlloc = NULLP;
            if (newHole->prvAlloc)
            {
               newHole->prvAlloc->nxtHole = newHole;
            }
            if (numSb == hole->num)
            {
               alloc = rgSCHUtlUlAllocGetCompHole(sf, hole);
            }
            else
            {
               alloc = rgSCHUtlUlAllocGetPartHole(sf, numSb, hole);
            }
            alloc->prvHole = newHole;
            newHole->nxtAlloc = alloc;
         }
         else /* Hole start and req alloc start are same */
         {
            if (numSb == hole->num)
            {
               alloc = rgSCHUtlUlAllocGetCompHole(sf, hole);
            }
            else
            {
               alloc = rgSCHUtlUlAllocGetPartHole(sf, numSb, hole);
            }
         }
         break;
      }
   } while ((hole = nxtHole));
   RETVALUE(alloc);
}
#ifdef LTE_L2_MEAS
/**
 * @brief  Validates the qci values
 *
 * @details
 *
 *     Function :rgSCHUtlValidateQci
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  U8              numQci
 *  @param[out] U8              *qci
 *  @return  S16
 *           ROK
 *           RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHUtlValidateQci
(
RgSchCellCb  *cellCb,
U8           numQci,
U8           *qci
)
#else
PRIVATE S16 rgSCHUtlValidateQci(cellCb, numQci, qci)
RgSchCellCb  *cellCb;
U8           numQci;
U8           *qci;
#endif
{
   U8        qciIdx;
   U8        qciVal;

   TRC3(rgSCHUtlValidateQci)

   for(qciIdx = 0; qciIdx < numQci; qciIdx++)
   {
      qciVal = qci[qciIdx];
      if(qciVal != cellCb->qciArray[qciVal].qci)
      {
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}/* rgSCHUtlValidateQci */
/**
 * @brief  Validates the measurement request parameters.
 *
 * @details
 *
 *     Function :rgSCHUtlValidateMeasReq
 *
 *  @param[in]  RgSchCellCb        *cellCb
 *  @param[in]  LrgSchMeasReqInfo  *schL2MeasInfo
 *  @param[out] RgSchErrInfo       *err
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlValidateMeasReq
(
RgSchCellCb       *cellCb,
LrgSchMeasReqInfo *schL2MeasInfo,
RgSchErrInfo      *err
)
#else
PUBLIC S16 rgSCHUtlValidateMeasReq(cellCb, schL2MeasInfo, err)
RgSchCellCb       *cellCb;
LrgSchMeasReqInfo *schL2MeasInfo;
RgSchErrInfo      *err;
#endif
{
   U8    measType;
   S16   ret;

   TRC3(rgSCHUtlValidateMeasReq)

   measType = schL2MeasInfo->measType;

   if((measType == 0) ||
       measType > 127)
   {
      err->errType = RGSCHERR_SCH_INVALID_MEAS_TYPE;
      err->errCause = RGSCHERR_SCH_L2MEAS;
      RETVALUE(RFAILED);
   }
   if((measType & LRG_L2MEAS_AVG_PRB_PER_QCI_DL) &&
      ((schL2MeasInfo->avgPrbQciDl.numQci > LRG_MAX_QCI_PER_REQ) ||
       (schL2MeasInfo->avgPrbQciDl.numQci == 0)))
   {
      err->errType = RGSCHERR_SCH_INVALID_PARAM_RANGE;
      err->errCause = RGSCHERR_SCH_L2MEAS;
      RETVALUE(RFAILED);
   }
   if((measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL) &&
      ((schL2MeasInfo->avgPrbQciUl.numQci > LRG_MAX_QCI_PER_REQ) ||
       (schL2MeasInfo->avgPrbQciUl.numQci == 0)))
   {
      err->errType = RGSCHERR_SCH_INVALID_PARAM_RANGE;
      err->errCause = RGSCHERR_SCH_L2MEAS;
      RETVALUE(RFAILED);
   }
   if((measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL) &&
      ((schL2MeasInfo->nmbActvUeQciDl.numQci > LRG_MAX_QCI_PER_REQ) ||
       (schL2MeasInfo->nmbActvUeQciDl.numQci == 0)))
   {
      err->errType = RGSCHERR_SCH_INVALID_PARAM_RANGE;
      err->errCause = RGSCHERR_SCH_L2MEAS;
      RETVALUE(RFAILED);
   }
   if((measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL) &&
      ((schL2MeasInfo->nmbActvUeQciUl.numQci > LRG_MAX_QCI_PER_REQ) ||
       (schL2MeasInfo->nmbActvUeQciUl.numQci == 0) ||
       (schL2MeasInfo->nmbActvUeQciUl.sampPrd == 0)||
       (schL2MeasInfo->timePrd < schL2MeasInfo->nmbActvUeQciUl.sampPrd)))
   {
      err->errType = RGSCHERR_SCH_INVALID_PARAM_RANGE;
      err->errCause = RGSCHERR_SCH_L2MEAS;
      RETVALUE(RFAILED);
   }
   if(measType & LRG_L2MEAS_AVG_PRB_PER_QCI_DL)
   {
      ret  = rgSCHUtlValidateQci(cellCb, schL2MeasInfo->avgPrbQciDl.numQci,
                                 schL2MeasInfo->avgPrbQciDl.qci);
      if(ret != ROK)
      {
          err->errType = RGSCHERR_SCH_INVALID_QCI_VAL;
          err->errCause = RGSCHERR_SCH_L2MEAS;
          RETVALUE(RFAILED);
      }
   }
   if(measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL)
   {
      ret  = rgSCHUtlValidateQci(cellCb, schL2MeasInfo->nmbActvUeQciDl.numQci,
                                 schL2MeasInfo->nmbActvUeQciDl.qci);
      if(ret != ROK)
      {
          err->errType = RGSCHERR_SCH_INVALID_QCI_VAL;
          err->errCause = RGSCHERR_SCH_L2MEAS;
          RETVALUE(RFAILED);
      }
   }
   if(measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL)
   {
      ret  = rgSCHUtlValidateQci(cellCb, schL2MeasInfo->nmbActvUeQciUl.numQci,
                                 schL2MeasInfo->nmbActvUeQciUl.qci);
      if(ret != ROK)
      {
          err->errType = RGSCHERR_SCH_INVALID_QCI_VAL;
          err->errCause = RGSCHERR_SCH_L2MEAS;
          RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}/* rgSCHUtlValidateMeasReq */
#endif /* LTE_L2_MEAS */
/******* </AllocHolesMemMgmnt>: END *****/
/* rg004.201-ADD-Added for SI Enhancement*/
#ifdef RGR_SI_SCH
/**
 * @brief API for sending SI configuration confirm from Scheduler to RRM
 *
 * @details
 *
 *     Function: rgSCHUtlRgrSiCfgCfm
 *
 *     This API is invoked to send SI configuration confirm from Scheduler
 *     to RRM.
 *     This API fills in Pst structure and SAP Ids and invokes
 *     config confirm API towards RRM.
 *
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  U8            status
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrSiCfgCfm
(
Inst          instId,
SpId          spId,
RgrCfgTransId transId,
U8            status
)
#else
PUBLIC S16 rgSCHUtlRgrSiCfgCfm(instId, spId, transId, status)
Inst          instId;
SpId          spId;
RgrCfgTransId transId;
U8            status;
#endif
{
   U8        prntTrans[RGR_CFG_TRANSID_SIZE+1];

   TRC2(rgSCHUtlRgrSiCfgCfm)

   cmMemcpy((U8 *)prntTrans, (U8 *)transId.trans, RGR_CFG_TRANSID_SIZE);
   prntTrans[RGR_CFG_TRANSID_SIZE] = '\0';

   RGSCHDBGPRM(instId,(rgSchPBuf(instId),"rgSCHUtlRgrSiCfgCfm: "
               "transId  %s status  %d \n", prntTrans, status));

   if(RgUiRgrSiCfgCfm(&rgSchCb[instId].rgrSap[spId].sapCfg.sapPst,
                    rgSchCb[instId].rgrSap[spId].sapCfg.suId,
                    transId, status) != ROK)
   {
      RGSCHDBGERR(instId,(rgSchPBuf(instId),"rgSCHUtlRgrSiCfgCfm: "
                "RgUiRgrSiCfgCfm Failed \n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgSCHUtlRgrSiCfgCfm */

/***********************************************************
 *
 *     Func : rgSCHUtlPutSiInfo
 *
 *     Desc : Utility Function to deallocate SI information
 *
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHUtlPutSiInfo
(
RgSchCellCb *cell
)
#else
PUBLIC Void rgSCHUtlPutSiInfo(cell)
RgSchCellCb *cell;
#endif
{
   U8    idx = 0;
   TRC2(rgSCHUtlPutSiInfo)
   /*Free the buffers in crntSiInfo*/
   RGSCH_FREE_MSG(cell->siCb.crntSiInfo.mib)
   RGSCH_FREE_MSG(cell->siCb.crntSiInfo.sib1)
   for(idx=0; idx < sizeof(cell->siCb.crntSiInfo.si)/
                     sizeof(cell->siCb.crntSiInfo.si[0]); idx++)
   {
      RGSCH_FREE_MSG(cell->siCb.crntSiInfo.si[idx])
   }

   /*Free the buffers in newSiInfo */
   RGSCH_FREE_MSG(cell->siCb.newSiInfo.mib)
   RGSCH_FREE_MSG(cell->siCb.newSiInfo.sib1)
   for(idx=0; idx < sizeof(cell->siCb.newSiInfo.si)/
                     sizeof(cell->siCb.newSiInfo.si[0]); idx++)
   {
      RGSCH_FREE_MSG(cell->siCb.newSiInfo.si[idx])
   }

   RETVOID;
}
#endif /*RGR_SI_SCH */
#ifdef LTEMAC_DRX
/***********************************************************
 *
 *     Func : rgSCHUtlGetDrxSchdUesInDl
 *
 *     Desc : Utility Function to fill the get the list of
 *            scheduled UEs. On these UE's, drx-inactivity
 *            timer will be started/restarted.
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes:
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHUtlGetDrxSchdUesInDl
(
RgSchCellCb     *cellCb,
RgInfSfAlloc    *sfAlloc,
CmLListCp       *dlDrxInactvTmrLst
)
#else
PUBLIC S16 rgSCHUtlGetDrxSchdUesInDl(cellCb, sfAlloc,dlDrxInactvTmrLst)
RgSchCellCb     *cellCb;
RgInfSfAlloc    *sfAlloc;
CmLListCp       *dlDrxInactvTmrLst;
#endif
{
   Bool             isNewTx = FALSE;
   RgInfUeInfo      *ueInfo  = &(sfAlloc->ueInfo);
   U8                itr,idx;
   CmLListCp         ulInActvLst; /* UE's on which inactivity timer
                                     is not to be started and therefore
                                     have to be inactivted for UL */

   CmLListCp         dlInActvLst; /* UE's on which inactivity timer
                                     is not to be started and therefore
                                     have to be inactivated for DL*/
   RgSchCmnCell      *cellSch = NULLP;
   RgSchDrxDlHqProcCb  *drxHq;
   RgSchDlHqProcCb     *dlHq;
   RgSchDRXCellCb      *drxCell = cellCb->drxCb;
   RgSchDrxUeCb        *drxUe;

   cmLListInit(&ulInActvLst);
   cmLListInit(&dlInActvLst);
   cmLListInit(dlDrxInactvTmrLst);


   RGSCHDBGERR(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx),
            "rgSCHUtlGetDrxSchdUesInDl()\n"));

   for(itr=0; itr < ueInfo->numUes; itr++)
   {
      RgSchUeCb     *ueCb;

      /* find ueCb fom RNTI */
      ueCb = rgSCHDbmGetUeCb(cellCb,(ueInfo->allocInfo[itr].rnti));

      if ( ueCb == NULLP)
      {
#ifdef LTEMAC_SPS
         if((ueCb = rgSCHDbmGetSpsUeCb(cellCb,(ueInfo->allocInfo[itr].rnti)))
               == NULLP)
#endif /*LTEMAC_SPS*/
         {
            RGSCHDBGERR(cellCb->instIdx,(rgSchPBuf(cellCb->instIdx),
                     "rgSCHUtlGetSchdUes(): "
                     "No ueCb found with rnti = (%d)\n",
                     ueInfo->allocInfo[itr].rnti));
            RETVALUE(RFAILED);
         }
      }

      if ( ueCb->isDrxEnabled == FALSE )
      {
         continue;
      }



      for ( idx = 0; idx < RGINF_MAX_TB_PER_UE; idx++ )
      {
         if(ueInfo->allocInfo[itr].tbInfo[idx].isReTx == FALSE)
         {
            isNewTx = TRUE;
            break;
         }
         else
         {
            /*Stop the DRX retransmission timer as UE scheduled for retx. Here
             * we stop the timer and inactivate the UE for both UL and DL.
             * This may result in loss of one subframe for UL but this trade
             * off is taken to avoid the overhead of maintaining a list of UEs
             * to be inactivated in the next subframe.*/
            dlHq = &ueCb->dl.hqEnt->procs[ueInfo->allocInfo[itr].hqProcId];
            drxHq = RG_SCH_DRX_GET_DL_HQ(dlHq);
            drxUe = RG_SCH_DRX_GET_UE(ueCb);
            if(drxHq->reTxIndx != DRX_INVALID)
            {
               drxUe->drxDlInactvMask  |=
                     (RG_SCH_DRX_DLHQ_BITMASK << dlHq->procId);
               drxUe->drxUlInactvMask  |=
                     (RG_SCH_DRX_DLHQ_BITMASK << dlHq->procId);

               /* if no other condition is keeping ue active,
                * inactivate the Ue
                */
               if ( !RG_SCH_DRX_DL_IS_UE_ACTIVE(drxUe))
               {
                  /* BUG 2 : HARQ_RTT, changed for consistency */
                  ueCb->dl.dlInactvMask |= (RG_DRX_INACTIVE);

                  /* Add to DL inactive list */
                  cmLListAdd2Tail(&dlInActvLst,&(ueCb->dlDrxInactvLnk));
                  ueCb->dlDrxInactvLnk.node = (PTR)ueCb;
               }

               if(!RG_SCH_DRX_UL_IS_UE_ACTIVE(drxUe))
               {
                  /*BUG 2: HARQ_RTT changed for consistency */
                  ueCb->ul.ulInactvMask |= (RG_DRX_INACTIVE);

                  cmLListAdd2Tail(&ulInActvLst,&(ueCb->ulDrxInactvLnk));
                  ueCb->ulDrxInactvLnk.node  = (PTR)ueCb;
               }

               cmLListDelFrm (&(drxCell->drxQ[drxHq->reTxIndx].harqRetxQ),
                     &(drxHq->harqRetxEnt));
               drxHq->reTxIndx = DRX_INVALID;
            }
         }
      }

      if ( isNewTx == TRUE )
      {
         if ( ueCb->drxCb->raRcvd == TRUE )
         {
            ueCb->drxCb->raRcvd = FALSE;

            /* mark the ra bit */
            ueCb->drxCb->drxUlInactvMask |= RG_SCH_DRX_RA_BITMASK;
            ueCb->drxCb->drxDlInactvMask |= RG_SCH_DRX_RA_BITMASK;


            /*don't start inactivity timer if the UE
             * was active only because dedicated ra was received*/
            if ( !RG_SCH_DRX_DL_IS_UE_ONDUR_INACTIVE(ueCb->drxCb)     ||
                 !RG_SCH_DRX_UL_IS_UE_ONDUR_INACTIVE(ueCb->drxCb)     ||
                 !RG_SCH_DRX_DL_IS_UE_INACTVTMR_INACTIVE(ueCb->drxCb) ||
                 !RG_SCH_DRX_UL_IS_UE_INACTVTMR_INACTIVE(ueCb->drxCb)
               )
            {
               /*neither is Onduration timer running, not inactivity timer.
                * Dont add this UE in the list of UE's on which inactivity
                * timer has to be started */

               /* mark them as inactive if no ther condition
                * is keeping the UE as active */
               if ( !RG_SCH_DRX_UL_IS_UE_ACTIVE(ueCb->drxCb))
               {
                  ueCb->ul.ulInactvMask |= RG_DRX_INACTIVE;

                  /* UE is inactive according to the mask.
                   * Make it inactive as we had made it
                   * active irrespective of the mask
                   * when we received a RAR
                   */
                  if ( !RG_SCH_CMN_UL_IS_UE_ACTIVE(ueCb) )
                  {
                     cmLListAdd2Tail(&ulInActvLst,
                            &(ueCb->ulDrxInactvLnk));
                     ueCb->ulDrxInactvLnk.node = (PTR)ueCb;
                  }
               }/*UL_IS_UE_ACTIVE*/

               /* UE is inactive according to the mask.
                * Make it inactive as we had made it
                * active irrespective of the mask
                * when we received a RAR
                */
               if ( !RG_SCH_DRX_DL_IS_UE_ACTIVE(ueCb->drxCb) )
               {
                  ueCb->dl.dlInactvMask |= RG_DRX_INACTIVE;

                  if( !RG_SCH_CMN_DL_IS_UE_ACTIVE(ueCb) )
                  {
                     cmLListAdd2Tail(&dlInActvLst,
                           &(ueCb->dlDrxInactvLnk));
                     ueCb->dlDrxInactvLnk.node = (PTR)ueCb;
                  }
               }/*DL_IS_UE_ACTIVE*/

               continue;

            }/*!onDur && !inActivity*/

         }/*if(ra->rcvd) == TRUE */

         cmLListAdd2Tail(dlDrxInactvTmrLst,&(ueCb->dlDrxInactvTmrLnk));
         ueCb->dlDrxInactvTmrLnk.node = (PTR)ueCb;

         /* Resetting flag */
         isNewTx = FALSE;
      }/*if(isNewTx == TRUE) */

   }/*for(itr....)*/

   /* Mark the following Ues inactive for UL*/
   cellSch = RG_SCH_CMN_GET_CELL(cellCb);

   cellSch->apisUl->rgSCHUlInactvtUes(cellCb, &ulInActvLst);
   cellSch->apisDl->rgSCHDlInactvtUes(cellCb, &dlInActvLst);

   RETVALUE(ROK);
}/* rgSCHUtlGetSchdUes*/
#endif /*LTEMAC_DRX*/


/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
/**
 * @brief This function fills StaInd struct
 *
 * @details
 *
 *     Function: rgSCHUtlFillSndStaInd
 *     Purpose:  Fills StaInd struct and sends the
 *               StaInd to RRM
 *
 *  @param[in]  RgSchCellCb        *cell  pointer to Cell Control block
 *  @param[in]  RgSchUeCb          *ue  pointer to Ue Control block
 *  @param[in]  RgrStaIndInfo      *staInfo Sta Ind struct to be filled
 *  @param[in]  U8                 numCqiRept NUmber of reports to be filled
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlFillSndStaInd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrStaIndInfo      *staInfo,
U8                 numCqiRept
)
#else
PUBLIC S16 rgSCHUtlFillSndStaInd(cell, ue, staInfo, numCqiRept)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrStaIndInfo      *staInfo;
U8                 numCqiRept;
#endif
{
   U8 idxStart;

   /* Fill StaInd for sending collated Latest N CQI rpeorts */
   /* Find index in the array from where Latest N
      reports needs to be fetched. Use this value to index in the array
      and copy the reports into staInfo */

   staInfo->cellId = cell->cellId;
   staInfo->crnti = ue->ueId;

   idxStart = ue->schCqiInfo.cqiCount - numCqiRept;

   cmMemcpy ((U8*)&(staInfo->ueCqiInfo.cqiRept),
            (U8*)&(ue->schCqiInfo.cqiRept[idxStart]),
            numCqiRept * sizeof(RgrUeCqiRept));

   staInfo->ueCqiInfo.numCqiRept = numCqiRept;

   ue->schCqiInfo.cqiCount = 0;

   /* Call utility function (rgSCHUtlRgrStaInd) to send rpts to RRM */
   if(rgSCHUtlRgrStaInd(cell, staInfo) != ROK)
   {
      RGSCHDBGERR (cell->instIdx,(rgSchPBuf(cell->instIdx),\
            "Could not send CQI reports\n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

}/* End of rgSCHUtlFillSndStaInd */



/**
 * @brief API for sending STA indication from Scheduler to RRM.
 *
 * @details
 *
 *     Function: rgSCHUtlRgrStaInd
 *
 *     This API is invoked to send STA indication from Scheduler instance to RRM.
 *     This API fills in Pst structure and RgrStaIndInfo
 *     and calls the Sta primitive API towards RRM.
 *
 *  @param[in]  cell                   RgSchCellCb
 *  @param[in]  RgrStsIndInfo          *rgrSta
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUtlRgrStaInd
(
RgSchCellCb          *cell,
RgrStaIndInfo        *rgrSta
)
#else
PUBLIC S16 rgSCHUtlRgrStaInd(cell, rgrSta)
RgSchCellCb          *cell;
RgrStaIndInfo        *rgrSta;
#endif
{
   S16           ret = ROK;
   RgSchUpSapCb  *rgrSap;                    /*!< RGR SAP Control Block */

   TRC2(rgSCHUtlRgrStaInd)

   RGSCHDBGPRM(cell->instIdx,
         (rgSchPBuf(cell->instIdx),"rgSCHUtlRgrStaInd: cellId = %d \n",
          cell->cellId));

   rgrSap = cell->rgrSap;
   if (rgrSap->sapSta.sapState != LRG_BND)
   {
      RGSCHDBGERR(cell->instIdx,(rgSchPBuf(cell->instIdx),
               "rgSCHUtlRgrStaInd() Upper SAP not bound (%d) \n",
               rgrSap->sapSta.sapState));
      RETVALUE(RFAILED);
   }
   RgUiRgrStaInd(&(cell->rgrSap->sapCfg.sapPst),
         cell->rgrSap->sapCfg.suId, rgrSta);
   RETVALUE(ret);
}  /* rgSCHUtlRgrStaInd*/
#endif /* End of RGR_CQI_REPT */


/* MS_FIX */
/**
 * @brief API for sending STA indication from Scheduler to RRM.
 *
 * @details
 *
 *     Function: rgSCHUtlRlsRnti
 *
 *     This API is invoked to release rnti at Schedular and to indicate MAC to release rnti
 *
 *  @param[in]  RgSchCellCb     *cellCb                   
 *  @param[in]  RgSchUeCb            *ue
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/

#ifdef ANSI
PUBLIC Void rgSCHUtlRlsRnti
(
RgSchCellCb    *cellCb,
RgSchUeCb         *ue
)
#else
PUBLIC Void rgSCHUtlRlsRnti(cellCb, ue)
RgSchCellCb    *cellCb;
RgSchUeCb         *ue
#endif
{

   TRC2(rgSCHUtlRlsRnti)
   rgSCHDbmRlsRnti(cellCb,ue->rntiLnk);

   RETVOID;
}
#endif
/**********************************************************************

         End of file:     gk_sch_utl.c@@/main/2 - Sat Jul 30 02:21:53 2011

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
/main/1      ---       apany 1. LTE MAC 2.1 release
           rg001.201   sm    1. ccpu00107387: To include the length
                                and ModOrder in DataRecp Req.
                             2. ccpu00107637: Nprb indication at
                                PHY for common Channel.
                             3. Removed extra comments.
           rg002.201   sd    1. ccpu00108808, ccpu00108807, ccpu00108737 :
                                Removed bo size validation for MIB
                             2. ccpu00108811: Corrected allocation
                                for common channels.
                             3. ccpu00101820: Fixing incorrect del/Add order.
                             4. ccpu00101624: Ensuring transId is always Null
                                terminated.
                             5. Fixed compilation problem with DEBUGP.
                             6. Fixed wrap around condition for idx for TDD
                                configuration.
           rg003.201   ns    1. Trace added to functions.
                             2. Added Holes Management functions for Adaptive
                                Re transmission.
           rg004.201   rbabu 1. ccpu00109159: Storing nCce for pdcch.
                             2. ccpu00105914: PHR handling for MSG3
           rg004.201   rnt   1. Modified for SI Enhancement
           rg005.201   rnt   1. Removed SAP validation for optimization
           rg006.201   sd    1. ccpu00111718: Updating NDI and HARQ proc Id
                             2. Changed prototype for function
                               'rgSCHUtlAllocRcptInfo'.
           rg007.201   ap  1. Added support for MIMO
           rg008.201   sd  1. Removed dependency on MIMO compile-time flag
                rsharon 2. Added support for SPS.
           rg009.201  rsharon  1. [ccpu00112372]:Changes for CR timer.
                               2. [ccpu00115330]: Corrected the calculation for
                                  number of PHICH groups.
                      gj       3. Added support for maintaining 20 subframes in
                                  rgSchCellCb
                      dv       4. Added changes of TFU_UPGRADE.
                               5. ccpu00116309:Changes for iPhich.
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1
           rg001.301   asehgal   1. Added changes for ticket ccpu00117270.
           rg002.301   nudupi    1. [ccpu00119223] Assigned with ue->ueId
                                            rntiLnk->rnti.
*********************************************************************91*/
