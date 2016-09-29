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
 
     Desc:     C source code for PFS functions
 
     File:     rg_sch_pfs.c
 
     Sid:      gk_sch_pfs.c@@/main/2 - Sat Jul 30 02:21:46 2011
 
     Prg:     apany
 
**********************************************************************/

/** @file rg_sch_pfs.c
@brief This module handles the proportional fair scheduler functionality
*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm5.h"           /* common timers */
#include "cm_hash.h"       /* common hash list */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch_err.h"
#include "rg_sch.h"
#include "rg_sch_cmn.h"
#include "rg_sch_pfs.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common tokens */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"
#include "rg_sch_pfs.x"



/* Putting declarations in 'extern_ "C"' */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Declarations for common functions (specific neither
 * to downlink, no uplink) */

PRIVATE Void rgSCHPfsInitFracPrioInfo ARGS((
         RgSchPfsFracPrioInfo   *fracPrioInfo,
         U32                    cfgdRate
         ));
PRIVATE Void rgSCHPfsUpdFracPrioInfo ARGS((
         RgSchPfsFracPrioInfo   *fracPrioInfo,
         U32                    allocd 
         ));
PRIVATE Void rgSCHPfsUlUpdPfsPrio ARGS((
         RgSchPfsUlCell        *pfsCell,
         RgSchPfsUlUe          *pfsUe,
	 Bool                  *prioChngd
         ));
PRIVATE Void rgSCHPfsDlUpdPfsPrio ARGS((
         RgSchPfsDlCell        *pfsCell,
         RgSchPfsDlLc          *pfsLc,
         RgSchPfsFracPrioInfo  *fracPrioInfo,
	 Bool                  *prioChngd
         ));
PRIVATE Void rgSCHPfsFillTbl ARGS((
         U32                   src[],
         U32                   dst[],
         U16                   numEntries,
         U32                  *minVal,
         U32                  *maxVal
         ));
PRIVATE Void rgSCHPfsFillRange ARGS((
        RgSchPfsPrioRange    *range,
        U32                   minPrios[3],
        U32                   maxPrios[3],
        U8                    numQs
        ));

/* Declarations for downlink functions */

PUBLIC Void rgSCHPfsDlInit ARGS((
         RgDlSchdApis       *apis
         ));
PUBLIC S16 rgSCHPfsDlCellCfg ARGS((
         RgSchCellCb        *cell,
         RgrCellCfg         *cfg,
         RgSchErrInfo       *err
         ));
PUBLIC S16 rgSCHPfsDlCellRecfg ARGS((
         RgSchCellCb        *cell,
         RgrCellRecfg       *recfg,
         RgSchErrInfo       *err
         ));
PUBLIC Void rgSCHPfsDlCellDel ARGS((
         RgSchCellCb       *cell
         ));
PUBLIC S16 rgSCHPfsDlUeCfg ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue,
         RgrUeCfg         *cfg,
         RgSchErrInfo     *err
         ));
PUBLIC S16 rgSCHPfsDlUeRecfg ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue,
         RgrUeRecfg       *recfg,
         RgSchErrInfo     *err
         ));
PUBLIC Void rgSCHPfsDlUeDel ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue
         ));
PUBLIC Void rgSCHPfsDlUeReset ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue
         ));
PUBLIC S16 rgSCHPfsDlLcCfg ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue,
         RgSchDlLcCb      *lc,
         RgrLchCfg        *cfg,
         RgSchErrInfo     *err
         ));
PUBLIC S16 rgSCHPfsDlLcRecfg ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue,
         RgSchDlLcCb      *lc,
         RgrLchRecfg      *recfg,
         RgSchErrInfo     *err
         ));
PUBLIC Void rgSCHPfsDlLcDel ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue,
         RgSchDlLcCb      *lc
         ));
PUBLIC Void rgSCHPfsDlInactvLstInd ARGS((
         RgSchCellCb      *cell,
         CmLListCp        *lst
         ));
PRIVATE Void rgSCHPfsDlUeInactv ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue
         ));
PUBLIC Void rgSCHPfsDlActvUeInd ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue
         ));
PUBLIC Void rgSCHPfsDlCqiInd ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue,
         TfuDlCqiRpt          *cqiInfo
         ));
PUBLIC Void rgSCHPfsDlSched ARGS((
         RgSchCellCb             *cell,
         RgSchCmnDlRbAllocInfo   *alloc
         ));
PRIVATE Void rgSCHPfsDlSchedTa ARGS((
         RgSchCellCb             *cell,
         RgSchCmnDlRbAllocInfo   *alloc
         ));
PRIVATE S16 rgSCHPfsDlSchedTaForUe ARGS((
         RgSchCellCb             *cell,
         RgSchCmnDlRbAllocInfo   *alloc,
         RgSchDlSf               *sf,
         RgSchUeCb               *ue
         ));
PRIVATE Void rgSCHPfsDlSchedRetx ARGS((
         RgSchCellCb             *cell,
         RgSchCmnDlRbAllocInfo   *alloc
         ));
PRIVATE Void rgSCHPfsDlSchedNewTx ARGS((
         RgSchCellCb             *cell,
         RgSchCmnDlRbAllocInfo   *alloc
         ));
PRIVATE Void rgSCHPfsDlNewTxInQ ARGS((
         RgSchCellCb             *cell,
         RgSchCmnDlRbAllocInfo   *alloc,
         CmLListCp               *txQ,
         CmLListCp               *retxQ,
         Bool                    *isBwLeftRef
         ));
PRIVATE Void rgSCHPfsDlRetxInQ ARGS((
         RgSchCellCb             *cell,
         RgSchCmnDlRbAllocInfo   *alloc,
         CmLListCp               *retxQ
         ));
PUBLIC Void rgSCHPfsDlProcRetx ARGS((
         RgSchCellCb         *cell,
         RgSchDlHqProcCb     *proc
         ));
PRIVATE Void rgSCHPfsDlProcAddToRetx ARGS((
         RgSchCellCb         *cell,
         RgSchUeCb           *ue,
         RgSchDlHqProcCb     *proc
         ));
PRIVATE Void rgSCHPfsDlProcAddToCellRetx ARGS((
         RgSchCellCb         *cell,
         RgSchDlHqProcCb     *proc
         ));
PRIVATE Void rgSCHPfsDlProcRmvFrmCellRetx ARGS((
         RgSchCellCb         *cell,
         RgSchDlHqProcCb     *proc
         ));
PRIVATE Void rgSCHPfsDlProcRmvFrmUeRetx ARGS((
         RgSchCellCb         *cell,
         RgSchUeCb           *ue,
         RgSchDlHqProcCb     *proc
         ));
PUBLIC Void rgSCHPfsDlOnAlloc ARGS((
         RgSchCellCb             *cell,
         RgSchCmnDlRbAllocInfo   *alloc
         ));
PRIVATE Void rgSCHPfsDlOnRetxAlloc ARGS((
         RgSchCellCb             *cell,
         RgSchCmnDlRbAllocInfo   *alloc
         ));
PRIVATE Void rgSCHPfsDlOnNewTxAlloc ARGS((
         RgSchCellCb             *cell,
         RgSchCmnDlRbAllocInfo   *alloc
         ));
PRIVATE Void rgSCHPfsDlOnNewTxAllocForUe ARGS((
         RgSchCellCb             *cell,
         RgSchUeCb               *ue,
         RgSchCmnDlRbAllocInfo   *alloc
         ));
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
PRIVATE Void rgSCHPfsDlLcPartlyAllocd ARGS((
         RgSchCellCb        *cell,
         RgSchUeCb          *ue,
         RgSchDlLcCb        *lc,
         U32                 diff,
         RgSchDlHqTbCb      *tbInfo
         ));
PRIVATE Void rgSCHPfsDlLcFullyAllocd ARGS((
         RgSchCellCb        *cell,
         RgSchUeCb          *ue,
         RgSchDlLcCb        *lc,
         RgSchDlHqTbCb      *tbInfo
         ));
PRIVATE Void rgSCHPfsDlNewTxTbDstn ARGS((
         RgSchCellCb    *cell,
         RgSchUeCb      *ue,
         RgSchDlHqTbCb  *tbInfo,
         U32            *effAlloc,
         CmLList        **node
         ));
PRIVATE Void rgSCHPfsDlSprTxTbDstn ARGS((
         RgSchCellCb    *cell,
         RgSchUeCb      *ue,
         RgSchDlHqTbCb  *tbInfo,
         U32            *effAlloc,
         CmLList        *node
));
PUBLIC Void rgSCHPfsDlBoUpd ARGS((
         RgSchCellCb            *cell,
         RgSchUeCb              *ue,
         RgSchDlLcCb            *lc
         ));
PRIVATE Void rgSCHPfsDlBoUpdInt ARGS((
         RgSchCellCb            *cell,
         RgSchUeCb              *ue,
         RgSchDlLcCb            *lc,
         U32                     bo,
         U32                     allocd
         ));
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
PRIVATE Void rgSCHPfsDlAddLcAllocToProc ARGS((
         RgSchCellCb            *cell,
         RgSchUeCb              *ue,
         RgSchDlLcCb            *lc,
         U32                     bytes,
         RgSchDlHqTbCb          *tbInfo
         ));
/*rg002.301_LPR1-MOD-Modified the prototype of the below function
 * to add a new parmater allocd*/
PRIVATE Void rgSCHPfsDlPrioAmbrSvcUpd ARGS((
         RgSchPfsDlCell     *pfsCell,
         RgSchUeCb          *ue,
         U32                allocd
         ));
PRIVATE Void rgSCHPfsDlLcAddToSrbQ ARGS((
         RgSchPfsDlCell     *pfsCell,
         RgSchPfsDlUe       *pfsUe,
         RgSchPfsDlLc       *pfsLc
         ));
PRIVATE Void rgSCHPfsDlLcAddToDrb0Q ARGS((
         RgSchPfsDlCell     *pfsCell,
         RgSchPfsDlUe       *pfsUe,
         RgSchPfsDlLc       *pfsLc
         ));
PRIVATE Void rgSCHPfsDlLcAddToGbrQ ARGS((
         RgSchPfsDlCell     *pfsCell,
         RgSchPfsDlUe       *pfsUe,
         RgSchPfsDlLc       *pfsLc,
	 Bool               prioChngd,
	 U32                allocd
         ));
PRIVATE Void rgSCHPfsDlLcAddToMbrQ ARGS((
         RgSchPfsDlCell     *pfsCell,
         RgSchPfsDlUe       *pfsUe,
         RgSchPfsDlLc       *pfsLc,
	 Bool               prioChngd,
	 U32                allocd
         ));
/*rg002.301-MOD-Modified the below function prototype to add a 
 * new parameter allocd.*/
PRIVATE Void rgSCHPfsDlLcAddToAmbrQ ARGS((
         RgSchPfsDlCell     *pfsCell,
         RgSchPfsDlUe       *pfsUe,
         RgSchPfsDlLc       *pfsLc,
         U32                allocd
         ));
PRIVATE Void rgSCHPfsDlLcAddToQ ARGS((
         RgSchPfsDlUe       *pfsUe,
         CmLListCp          *cp,
         RgSchPfsDlLc       *pfsLc
         ));
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
PRIVATE Void rgSCHPfsDlLcAmbrBoUpd ARGS((
         RgSchPfsDlCell     *pfsCell,
         RgSchUeCb          *ue,
         RgSchDlLcCb        *lc,
         U32                 bo,
         U32                 allocd
         ));
PRIVATE Void rgSCHPfsDlRemAmbrZero ARGS((
         RgSchPfsDlCell     *pfsCell,
         RgSchPfsDlUe       *pfsUe
         ));
PRIVATE Void rgSCHPfsDlLcGbrBoUpd ARGS((
         RgSchPfsDlCell     *pfsCell,
         RgSchUeCb          *ue,
         RgSchDlLcCb        *lc,
         U32                 bo,
         U32                 allocd
         ));
/*rg002.301_LPR1-MOD-Modified the prototype of the below function to add
 * a new parameter allocd.*/
PRIVATE Void rgSCHPfsDlLcAmbrActv ARGS((
         RgSchPfsDlCell     *pfsCell,
         RgSchUeCb          *ue,
         RgSchDlLcCb        *lc,
         U32                 bo,
         U32                 allocd
         ));
PRIVATE Void rgSCHPfsDlLcAmbrInactv ARGS((
         RgSchPfsDlCell     *pfsCell,
         RgSchUeCb          *ue,
         RgSchDlLcCb        *lc
         ));
PRIVATE Void rgSCHPfsDlLcInsInUeAmbrLst ARGS((
         RgSchPfsDlUe       *pfsUe,
         RgSchDlLcCb        *lc
         ));
PRIVATE U8 rgSCHPfsDlGetQId ARGS((
         RgSchPfsPrioRange  *range,
         U32                 prioVal,
         U8                  maxQId
         ));
PRIVATE Void rgSCHPfsDlLcRmvFrmPrioQ ARGS((
         RgSchPfsDlCell     *pfsCell,
         RgSchPfsDlUe       *pfsUe,
         RgSchPfsDlLc       *pfsLc
         ));
PUBLIC Void rgSCHPfsDlUeRefresh ARGS((
         RgSchCellCb         *cell,
         RgSchUeCb           *ue
         ));
PRIVATE Void rgSCHPfsDlLcGbrRefresh ARGS((
         RgSchCellCb         *cell,
         RgSchUeCb           *ue,
         RgSchDlLcCb         *lc
         ));
PRIVATE U32 rgSchPfsDlGetSvcPrio ARGS((
         RgSchPfsDlCell      *pfsCell,
         U8                   prio
         ));
PRIVATE U32 rgSCHPfsDlGetCqiPrio ARGS((
         RgSchPfsDlCell      *pfsCell,
         U8                   cqi
         ));
PRIVATE S16 rgSCHPfsDlLcSched ARGS((
         RgSchCellCb         *cell,
         RgSchCmnDlRbAllocInfo *alloc,
         RgSchDlSf           *sf,
         RgSchUeCb           *ue,
         RgSchDlLcCb         *lc,
         CmLListCp           *retxQ,
         Bool                *isBwLeftRef
         ));
PRIVATE RgSchDlHqProcCb *rgSCHPfsDlGetAvlProc ARGS((
         RgSchCellCb         *cell,
         RgSchUeCb           *ue,
         CmLListCp           *retxQ
         ));
PRIVATE Void rgSCHPfsDlRlsHqProc ARGS((
         RgSchUeCb           *ue,
         RgSchDlHqProcCb     *proc
         ));
PRIVATE Void rgSCHPfsDlResetProcs ARGS((
         RgSchCellCb         *cell,
         RgSchUeCb           *ue
         ));
PRIVATE Void rgSCHPfsDlUeSfResetTemp ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue,
	     RgSchDlHqProcCb      *hqP
         ));


/* Declarations for uplink functions */

PUBLIC Void rgSCHPfsUlInit ARGS((
         RgUlSchdApis         *apis
         ));
PUBLIC S16 rgSCHPfsUlCellCfg ARGS((
         RgSchCellCb          *cell,
         RgrCellCfg           *cfg,
         RgSchErrInfo         *err
         ));
PUBLIC S16 rgSCHPfsUlCellRecfg ARGS((
         RgSchCellCb     *cell,
         RgrCellRecfg    *recfg,
         RgSchErrInfo    *err
         ));
PUBLIC Void rgSCHPfsUlCellDel ARGS((
         RgSchCellCb       *cell
         ));
PUBLIC S16 rgSCHPfsUlUeCfg ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue,
         RgrUeCfg         *cfg,
         RgSchErrInfo     *err
         ));
PUBLIC S16 rgSCHPfsUlUeRecfg ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue,
         RgrUeRecfg       *recfg,
         RgSchErrInfo     *err
         ));
PUBLIC Void rgSCHPfsUlUeDel ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue
         ));
PUBLIC Void rgSCHPfsUlUeReset ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue
         ));
PUBLIC S16 rgSCHPfsUlLcgCfg ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue,
         RgSchLcgCb       *lcg,
         RgrLcgCfg        *cfg,
         RgSchErrInfo     *err
         ));
PUBLIC S16 rgSCHPfsUlLcgRecfg ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue,
         RgSchLcgCb       *lcg,
         RgrLcgRecfg      *recfg,
         RgSchErrInfo     *err
         ));
PUBLIC Void rgSCHPfsUlLcgDel ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue,
         RgSchLcgCb       *lcg
         ));
PUBLIC Void rgSCHPfsUlUeRefresh ARGS((
         RgSchCellCb         *cell,
         RgSchUeCb           *ue
         ));
PUBLIC Void rgSCHPfsUlSched ARGS((
         RgSchCellCb             *cell,
         RgSchCmnUlRbAllocInfo   *alloc
         ));
PRIVATE Void rgSCHPfsUlSchedNewTx ARGS((
         RgSchCellCb             *cell,
         RgSchCmnUlRbAllocInfo   *alloc,
         U8                       remUe
         ));
PRIVATE Void rgSCHPfsUlSchedContRes ARGS((
         RgSchCellCb             *cell,
         RgSchCmnUlRbAllocInfo   *alloc,
         U8                      *remUeRef
         ));
PRIVATE Void rgSCHPfsUlSchedSig ARGS((
         RgSchCellCb             *cell,
         RgSchCmnUlRbAllocInfo   *alloc,
         U8                      *remUeRef
         ));
PRIVATE Void rgSCHPfsUlSchedDat ARGS((
         RgSchCellCb             *cell,
         RgSchCmnUlRbAllocInfo   *alloc,
         U8                      *remUeRef
         ));
PRIVATE Void rgSCHPfsUlSchedUeForContRes ARGS((
         RgSchCellCb             *cell,
         RgSchCmnUlRbAllocInfo   *alloc,
         RgSchUeCb               *ue
         ));
PRIVATE S16 rgSCHPfsUlSchedUeForSigDat ARGS((
         RgSchCellCb             *cell,
         RgSchCmnUlRbAllocInfo   *alloc,
         RgSchUeCb               *ue
         ));
PUBLIC Void rgSCHPfsUlUpdBsrShort ARGS((
         RgSchCellCb         *cell,
         RgSchUeCb           *ue,
         RgSchLcgCb          *lcg,
         U8                   bsr
         ));
PUBLIC Void rgSCHPfsUlUpdBsrTrunc ARGS((
         RgSchCellCb         *cell,
         RgSchUeCb           *ue,
         RgSchLcgCb          *lcg,
         U8                   bsr
         ));
PUBLIC Void rgSCHPfsUlUpdBsrLong ARGS((
         RgSchCellCb         *cell,
         RgSchUeCb           *ue,
         U8                   bsrArr[]
         ));
PRIVATE Void rgSCHPfsUlMngUeInQ ARGS((
         RgSchCellCb         *cell,
         RgSchUeCb           *ue,
    	 U32                 allocd
         ));
PUBLIC Void rgSCHPfsUlInactvLstInd ARGS((
         RgSchCellCb      *cell,
         CmLListCp        *lst
         ));
PRIVATE Void rgSCHPfsUlUeInactv ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue
         ));
PUBLIC Void rgSCHPfsUlActvUeInd ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue
         ));
PUBLIC Void rgSCHPfsUlCqiInd ARGS((
         RgSchCellCb         *cell,
         RgSchUeCb           *ue,
         TfuUlCqiRpt         *cqiInfo
         ));
PRIVATE Void rgSCHPfsUlAddUeToSigQ ARGS((
         RgSchCellCb         *cell,
         RgSchUeCb           *ue
         ));
PRIVATE Void rgSCHPfsUlAddUeToPfsQ ARGS((
         RgSchCellCb         *cell,
         RgSchUeCb           *ue,
	 Bool                prioChngd
         ));
PUBLIC Void rgSCHPfsUlSrInd ARGS((
         RgSchCellCb         *cell,
         RgSchUeCb           *ue
         ));
PRIVATE Void rgSCHPfsUlRmvUeFrmQ ARGS((
         RgSchPfsUlCell     *pfsCell,
         RgSchPfsUlUe       *pfsUe
         ));
PRIVATE Void rgSCHPfsUlAddUeToQ ARGS((
         CmLListCp           *cp,
         RgSchUeCb           *ue
         ));
PUBLIC Void rgSCHPfsUlContResReq ARGS((
         RgSchCellCb         *cell,
         RgSchUeCb           *ue
         ));
PUBLIC Void rgSCHPfsUlOnAlloc ARGS((
         RgSchCellCb           *cell,
         RgSchCmnUlRbAllocInfo *alloc
         ));
#ifdef LTEMAC_DRX
PRIVATE Void rgSCHPfsUlUeSfResetTemp ARGS((
         RgSchCellCb             *cell,
         RgSchUeCb               *ue,
         CmLListCp               *ulInactvLst
         ));
#else
PRIVATE Void rgSCHPfsUlUeSfResetTemp ARGS((
         RgSchCellCb             *cell,
         RgSchUeCb               *ue
         ));
#endif
PRIVATE U32 rgSCHPfsUlGetCqiPrio ARGS((
         RgSchPfsUlCell      *pfsCell,
         U8                   cqi
         ));
PRIVATE U32 rgSCHPfsUlGetFracPrio ARGS((
         RgSchPfsUlCell      *pfsCell,
         U8                   frac
         ));
PRIVATE U32 rgSCHPfsUlGetLcgPrio ARGS((
         RgSchPfsUlCell      *pfsCell,
         U8                   lcgId
         ));
PRIVATE U32 rgSCHPfsUlGetQId ARGS((
         RgSchPfsUlCell      *pfsCell,
         U32                  pfsPrio
         ));
PUBLIC S16 rgSCHPfsDlUeHqEntInit ARGS((
         RgSchCellCb      *cell,
         RgSchUeCb        *ue
         ));
PUBLIC S16 rgSCHPfsDlUeHqEntDeInit ARGS((
         RgSchCellCb      *cell,
         RgSchDlHqEnt     *hqE
         ));
#ifdef __cplusplus
}
#endif /* __cplusplus */




/**** DOWNLINK functions ****/


/**
 * @brief This function initializes all the data for the scheduler
 *
 * @details
 *
 *     Function: rgSCHPfsDlInit
 *
 *     Processing Steps:
 *     - Assigns the various callback APIs
 *
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsDlInit
(
RgDlSchdApis         *apis
)
#else
PUBLIC Void rgSCHPfsDlInit(apis)
RgDlSchdApis         *apis;
#endif
{
   /*rg003.201: Trace Added */
   TRC2(rgSCHPfsDlInit);

   apis->rgSCHRgrDlUeCfg        = rgSCHPfsDlUeCfg;
   apis->rgSCHRgrDlUeRecfg      = rgSCHPfsDlUeRecfg;
   apis->rgSCHFreeDlUe          = rgSCHPfsDlUeDel;
   apis->rgSCHRgrDlCellCfg      = rgSCHPfsDlCellCfg;
   apis->rgSCHRgrDlCellRecfg    = rgSCHPfsDlCellRecfg; 
   apis->rgSCHFreeDlCell        = rgSCHPfsDlCellDel;
   apis->rgSCHRgrDlLcCfg        = rgSCHPfsDlLcCfg;
   apis->rgSCHRgrDlLcRecfg      = rgSCHPfsDlLcRecfg;
   apis->rgSCHFreeDlLc          = rgSCHPfsDlLcDel;
   apis->rgSCHDlSched           = rgSCHPfsDlSched;
   apis->rgSCHDlDedBoUpd        = rgSCHPfsDlBoUpd;
   apis->rgSCHDlProcAddToRetx   = rgSCHPfsDlProcRetx;
   apis->rgSCHDlAllocFnlz       = rgSCHPfsDlOnAlloc;
   apis->rgSCHDlCqiInd          = rgSCHPfsDlCqiInd;
   apis->rgSCHDlUeRefresh       = rgSCHPfsDlUeRefresh;
   apis->rgSCHDlUeReset         = rgSCHPfsDlUeReset;
   apis->rgSCHDlActvtUe         = rgSCHPfsDlActvUeInd;
   apis->rgSCHDlInactvtUes      = rgSCHPfsDlInactvLstInd;
   apis->rgSCHDlUeHqEntInit     = rgSCHPfsDlUeHqEntInit;
   apis->rgSCHDlUeHqEntDeInit   = rgSCHPfsDlUeHqEntDeInit;

   RETVOID;
}

/**
 * @brief Cell configuration for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsDlCellCfg
 *
 *     Processing Steps:
 *      - Create the pfsCell info within passed SchCell
 *      - Update priority tables
 *      - Determine the min, max and priority ranges per
 *        PFS queue and store them
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgrCellCfg          *cfg
 *  @param[out] RgSchErrInfo        *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPfsDlCellCfg
(
RgSchCellCb          *cell,
RgrCellCfg           *cfg,
RgSchErrInfo         *err
)
#else
PUBLIC S16 rgSCHPfsDlCellCfg(cell, cfg, err)
RgSchCellCb          *cell;
RgrCellCfg           *cfg;
RgSchErrInfo         *err;
#endif
{
   RgrCellDlPfs        *pfsCfg = &cfg->dlSchInfo.cellDlPfs;
   RgSchCmnDlCell      *cmnCell = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchPfsDlCell      *pfsCell;
   U32                  minCqiPrio;
   U32                  maxCqiPrio;
   U32                  minFracPrio;
   U32                  maxFracPrio;
   U32                  maxSvcPrio;
   U32                  minGbrPrio;
   U32                  maxGbrPrio;
   U32                  minAmbrPrio;
   U32                  maxAmbrPrio;
   U32                  minPrios[3];
   U32                  maxPrios[3];

   UNUSED(err);
   TRC2(rgSCHPfsDlCellCfg);

   if (rgSCHUtlAllocSBuf(cell->instIdx,
            (Data**)&cmnCell->schSpfc, (sizeof(RgSchPfsDlCell))) != ROK)
   {
      RETVALUE(RFAILED);
   }

   pfsCell = (RgSchPfsDlCell *)(cmnCell->schSpfc);

   /* CQI priorities */
   rgSCHPfsFillTbl(pfsCfg->cqiPrios, pfsCell->tbls.cqiPrios,
         RGR_PFS_CQI_PRIO_LVLS, &minCqiPrio, &maxCqiPrio);
   /* MS_FIX: syed Consider Rank in min/maxCqiPrio determination */
   minCqiPrio = RGSCH_CEIL(minCqiPrio, cell->numTxAntPorts);

   /* Bandwidth priorities */
   rgSCHPfsFillTbl(pfsCfg->fracPrios, pfsCell->tbls.fracPrios,
         RGR_PFS_FRAC_PRIO_LVLS, &minFracPrio, &maxFracPrio);

   /* GBR/MBR service priorities */
   rgSCHPfsFillTbl(&pfsCfg->svcPrioPrios[RG_SCH_CMN_DL_GBR_PRIO_START],
         &pfsCell->tbls.svcPrioPrios[RG_SCH_CMN_DL_GBR_PRIO_START],
         RG_SCH_CMN_DL_GBR_PRIO_END+1 - RG_SCH_CMN_DL_GBR_PRIO_START,
         &minGbrPrio, &maxGbrPrio);

   /* AMBR (except priority 0) service priorities */
   rgSCHPfsFillTbl(&pfsCfg->svcPrioPrios[RG_SCH_CMN_DL_GBR_PRIO_END+1],
         &pfsCell->tbls.svcPrioPrios[RG_SCH_CMN_DL_GBR_PRIO_END+1],
         RG_SCH_CMN_DL_PRIOS - (RG_SCH_CMN_DL_GBR_PRIO_END+1),
         &minAmbrPrio, &maxAmbrPrio);

   /* We don't care about priority zero service, that is not subjected
    * to PFS priority */

   maxSvcPrio = RGSCH_MAX(maxAmbrPrio, maxGbrPrio);

   /* Make sure that there are non-zero priority values */
   if ((maxCqiPrio == 0) || (maxSvcPrio == 0) || (maxFracPrio == 0))
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),
               "rgSCHPfsDlCellCfg(): Priority values are all zero\n"));
      MSPD_ERR("rgSCHPfsDlCellCfg(): Priority values are all zero\n");
	  /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx,
         (Data**)(&(cmnCell->schSpfc)), (sizeof(RgSchPfsDlCell)));
      RETVALUE(RFAILED);
   }

   /* Make sure that the product of maximum priority values dooes
    * not overflow a U32 */
   if ((maxCqiPrio > ((U32)0xffffffff) / maxFracPrio)
         || (maxCqiPrio * maxFracPrio > ((U32)-1) / maxSvcPrio))
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),
               "rgSCHPfsDlCellCfg(): Priority values too large\n"));
      MSPD_ERR("rgSCHPfsDlCellCfg(): Priority values too large\n");
	  /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx,
         (Data**)(&(cmnCell->schSpfc)), (sizeof(RgSchPfsDlCell)));
      RETVALUE(RFAILED);
   }

   minPrios[0] = minCqiPrio;
   minPrios[1] = minFracPrio;
   maxPrios[0] = maxCqiPrio;
   maxPrios[1] = maxFracPrio;

   minPrios[2] = minGbrPrio;
   maxPrios[2] = maxGbrPrio;
   rgSCHPfsFillRange(&pfsCell->ranges.gbrRange, minPrios, maxPrios,
         RG_SCH_PFS_DL_NUM_GBR_QUEUES);
   rgSCHPfsFillRange(&pfsCell->ranges.mbrRange, minPrios, maxPrios,
         RG_SCH_PFS_DL_NUM_MBR_QUEUES);

   minPrios[2] = minAmbrPrio;
   maxPrios[2] = maxAmbrPrio;
   rgSCHPfsFillRange(&pfsCell->ranges.ambrRange, minPrios, maxPrios,
         RG_SCH_PFS_DL_NUM_AMBR_QUEUES);
   /* MS_FIX : syed MAX Initial Priority Value to be used for prio initialization. */
   pfsCell->initPrioVal = maxCqiPrio * maxFracPrio * maxSvcPrio;

   RETVALUE(ROK);
}

/**
 * @brief Cell reconfiguration for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsDlCellRecfg
 *
 *     Processing Steps:
 *      - [NONE - no reconfig for PFS]
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgrCellRecfg      *recfg
 *  @param[out] RgSchErrInfo      *err
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPfsDlCellRecfg
(
RgSchCellCb     *cell,
RgrCellRecfg    *recfg,
RgSchErrInfo    *err
)
#else
PUBLIC S16 rgSCHPfsDlCellRecfg(cell, recfg, err)
RgSchCellCb     *cell;
RgrCellRecfg    *recfg;
RgSchErrInfo    *err;
#endif
{
   /* No reconfig for PFS, so just return ROK */
   UNUSED(cell);
   UNUSED(recfg);
   UNUSED(err);
   TRC2(rgSCHPfsDlCellRecfg);

   RETVALUE(ROK);
}


/**
 * @brief Cell deletion for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsDlCellDel
 *
 *     Processing Steps:
 *      - Free PFS related information per cell.
 *      - [ Nothing else needs to be done.
 *        Common modules would take care of deleting
 *        each UE in the cell, during which time PFS
 *        shall be called for UE deletion. ]
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsDlCellDel
(
RgSchCellCb       *cell
)
#else
PUBLIC Void rgSCHPfsDlCellDel(cell)
RgSchCellCb       *cell;
#endif
{
   RgSchCmnCell *cmnCell = (RgSchCmnCell *)(cell->sc.sch);
   TRC2(rgSCHPfsDlCellDel);

   if (cmnCell->dl.schSpfc == NULLP)
   {
      RETVOID;
   }
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(cmnCell->dl.schSpfc)), (sizeof(RgSchPfsDlCell)));
   RETVOID;
}

/**
 * @brief Dl Harq Entity initialization for PFS 
 *
 * @details
 *
 *     Function : rgSCHPfsDlUeHqEntInit
 *
 *     Processing Steps:
 *      - Create PFS related information per Harq Entity
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPfsDlUeHqEntInit
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PUBLIC S16 rgSCHPfsDlUeHqEntInit(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   RgSchDlHqEnt       *hqEnt;
   S16                 ret;
   S16                 idx;

   TRC2(rgSCHPfsDlUeHqEntInit);

   /* Alloc and init sched specific info for each proc */
   hqEnt = ue->dl.hqEnt;
   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data**)&hqEnt->sch,
         (hqEnt->numHqPrcs * sizeof(RgSchPfsDlHqProc)));
   if (ret != ROK)
   {
      RETVALUE(ret);
   }
   for (idx = 0; idx < hqEnt->numHqPrcs; ++idx)
   {
      RgSchDlHqProcCb    *proc = &ue->dl.hqEnt->procs[idx];
      RgSchCmnDlHqProc   *cmnProc = (RgSchCmnDlHqProc *)proc->sch;
      cmnProc->schSpfc = &((RgSchPfsDlHqProc *)hqEnt->sch)[idx];
      /* ueRetxLnk.node and prioLnk.node indicate presence
       * in list and so are not set here */
   }
   
   RETVALUE(ROK);
}

/**
 * @brief UE configuration for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsDlUeCfg
 *
 *     Processing Steps:
 *      - Create the pfsUeCb
 *      - Validate and update configured values
 *      - [UE specific refresh timer actions to happen
 *        as part of common modules]
 *      - Allocate and initialise scheduler specific info
 *        for each HARQ proc
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgrUeCfg         *cfg
 *  @param[out] RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPfsDlUeCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrUeCfg         *cfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHPfsDlUeCfg(cell, ue, cfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgrUeCfg         *cfg;
RgSchErrInfo     *err;
#endif
{
   RgSchPfsDlCell     *pfsCell = RG_SCH_PFS_GET_PFSCELLDL(cell);
   RgSchCmnDlUe       *cmnUe = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchPfsDlUe       *pfsUe;

   UNUSED(cfg);
   UNUSED(err);

   TRC2(rgSCHPfsDlUeCfg);


   if (rgSCHUtlAllocSBuf(cell->instIdx,
            (Data**)&cmnUe->schSpfc, (sizeof(RgSchPfsDlUe))) != ROK)
   {
      RETVALUE(RFAILED);
   }

   pfsUe = (RgSchPfsDlUe *)cmnUe->schSpfc;
   /* Placing UE into timing queue done by common */
   pfsUe->remAmbr = cmnUe->ambrCfgd;
   pfsUe->cqi     = cmnUe->mimoInfo.cwInfo[0].cqi;
   pfsUe->cqiPrio = rgSCHPfsDlGetCqiPrio(pfsCell, pfsUe->cqi);
   pfsUe->retxLnk.node = (PTR)ue;
#ifdef MSPD
   MSPD_DBG("RSYS:rgSCHPfsDlUeCfg: UE rnti(%d), cqi(%d), ambrCfgd(%lu).\n",  \
          ue->ueId, pfsUe->cqi, cmnUe->ambrCfgd);
#endif

   /* MS_FIX : Updating PFS to start prio with highest possible value
               and diminishing prio val updates from there on */
   rgSCHPfsInitFracPrioInfo(&pfsUe->fracPrioInfo, cmnUe->ambrCfgd); 
   /* Since pfsUe was memset to zero at the time of memory allocation,
    * not initialising lists. */

   RETVALUE(ROK);
}

/**
 * @brief UE reconfiguration for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsDlUeRecfg
 *
 *     Processing Steps:
 *      - [Not taking updated AMBR now;
 *        allowing current refresh cycle to misbehave]
 *
 *  @param[in]   RgSchCellCb      *cell
 *  @param[in]   RgSchUeCb        *ue
 *  @param[in]   RgrUeRecfg       *recfg
 *  @param[out]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPfsDlUeRecfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrUeRecfg       *recfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHPfsDlUeRecfg(cell, ue, recfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgrUeRecfg       *recfg;
RgSchErrInfo     *err;
#endif
{
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependncy on MIMO compile-time flag */
   /* rg007.201 - Changes for MIMO feature addition */
   RgSchCmnDlUe *ueCmnDl  = RG_SCH_CMN_GET_DL_UE(ue);

   TRC2(rgSCHPfsDlUeRecfg)
   UNUSED(recfg);
   UNUSED(err);
   /* Discarding TB2's context from scheduling Queues.
    * Since TB2 transmission needs signalling using 
    * TM specific formats. And since during this transient
    * period of UE TM Recfg, SCH always uses Format 1A,
    * the TB2s are discarded. */
   if (ueCmnDl->mimoInfo.forceTD & RG_SCH_CMN_TD_TXMODE_RECFG)
   {
      /* If HqP is in retx queue only for TB2 retx scheduling
       * then remove the harp proc from retx Queue */

      /* If Hqp is in retx queue for retx allocation of 
       * both TB1 and TB2, then reset TB2's state as ACKED */
      RgSchDlHqProcCb    *hqP;
      Pst                pst;
      RgInfRlsHqInfo     *rlsHqBufs = &(cell->rlsHqArr[cell->crntHqIdx]);
      U8                 i;

      /* Prepare TB2 release information to be sent to MAC */
      rlsHqBufs->numUes = 0;
      for(i = 0; i < ue->dl.hqEnt->numHqPrcs; i++)
      {
         hqP = &ue->dl.hqEnt->procs[i];
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].rnti = ue->ueId;
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].hqProcId = hqP->procId;
         if (hqP->tbInfo[1].state == HQ_TB_NACKED)
         {
            if (hqP->tbInfo[0].state != HQ_TB_NACKED)
            {
               /* Remove the HqP from retx Queue.
                  Release HqP.*/
               rgSCHPfsDlProcRmvFrmRetx(cell, ue, hqP);
            }
            /* rg001.301- MOD- ccpu00118350 : Correcting NDI manipulation of Harq */
            rgSCHDhmRlsHqpTb(hqP, 1, TRUE);
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].tbId[0] = 2;
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs = 1;
         }
         rlsHqBufs->numUes++;
      }
      /* Send the hqProc list for MAC to clear TB1 contents */
      if (rlsHqBufs->numUes)
      {
         rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
         RgSchMacRlsHq (&pst, rlsHqBufs);
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief Dl Harq Entity deletion for Round Robin
 *
 * @details
 *
 *     Function : rgSCHPfsDlUeHqEntDeInit 
 *
 *     Processing Steps:
 *      - Free PFS related information per Harq Entity
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchDlHqEnt     *hqE 
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPfsDlUeHqEntDeInit 
(
RgSchCellCb      *cell,
RgSchDlHqEnt     *hqE
)
#else
PUBLIC S16 rgSCHPfsDlUeHqEntDeInit(cell, hqE)
RgSchCellCb      *cell;
RgSchDlHqEnt     *hqE;
#endif
{
   TRC2(rgSCHPfsDlUeHqEntDeInit);

   if (hqE->sch != NULLP)
   {
      rgSCHUtlFreeSBuf(cell->instIdx, (Data**)(&(hqE->sch)), 
            (hqE->numHqPrcs * sizeof(RgSchPfsDlHqProc)));
   }

   RETVALUE(ROK);
}

/**
 * @brief UE delete for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsDlUeDel
 *
 *     Processing Steps:
 *     - Free scheduler specific HARQ proc info for each proc
 *     - Free UE sched info
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsDlUeDel
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PUBLIC Void rgSCHPfsDlUeDel(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   RgSchCmnDlUe        *cmnUe = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchDlHqEnt        *hqEnt;
   TRC2(rgSCHPfsDlUeDel);

   if (cmnUe->schSpfc == NULLP)
   {
      RETVOID;
   }

   hqEnt = ue->dl.hqEnt;

   if ((hqEnt != NULLP) && (hqEnt->sch != NULLP))
   {
      rgSCHPfsDlResetProcs(cell, ue);
   }

   /* No need to free logical channels from queues or to delete
    * their scheduler specific info since lc del would get
    * called as part of UE del */

   rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(cmnUe->schSpfc)), (sizeof(RgSchPfsDlUe)));

   RETVOID;
}

/**
 * @brief UE reset for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsDlUeReset
 *
 *     Processing Steps:
 *      - Remove UEs from whichever queues its LCs might be
 *        present in. B0s set to zero by the common module.
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsDlUeReset
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PUBLIC Void rgSCHPfsDlUeReset(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   S16                 idx;
   TRC2(rgSCHPfsDlUeReset);

   rgSCHPfsDlResetProcs(cell, ue);

   /* Could use a list of LCs that are in queues. This one could
    * navigate now to reset their BOs and take them out
    * of queues. 
    * BUT, for LCs not in queues, need to
    * reset BOs (they may not be in queues due to effBo being
    * zero while BO is still positive). So just go through
    * all LCs. */
   for (idx = 0; idx < RGSCH_MAX_LC_PER_UE; ++idx)
   {
      RgSchDlLcCb        *lc = ue->dl.lcCb[idx];

      if (lc == NULLP)
      {
         continue;
      }
      rgSCHPfsDlBoUpdInt(cell, ue, lc, 0, 0);
   }

   RETVOID;
}

/**
 * @brief LC configuration for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsDlLcCfg
 *
 *     Processing Steps:
 *     - Create pfsDlLc info and initialise vars
 *     - Add to cfgGbrLst if this is a gbr service
 *     - Add to cfgAmbrLst if this is an AMBR (barring
 *       DRB0) service
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlLcCb      *lc
 *  @param[in]  RgrLchCfg        *cfg
 *  @param[out] RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPfsDlLcCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchDlLcCb      *lc,
RgrLchCfg        *cfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHPfsDlLcCfg(cell, ue, lc, cfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchDlLcCb      *lc;
RgrLchCfg        *cfg;
RgSchErrInfo     *err;
#endif
{
   RgSchPfsDlCell     *pfsCell = RG_SCH_PFS_GET_PFSCELLDL(cell);
   RgSchPfsDlUe       *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
   RgSchCmnDlSvc      *cmnLc = (RgSchCmnDlSvc *)lc->sch;
   RgSchPfsDlLc       *pfsLc;

   UNUSED(cfg);
   UNUSED(err);
   TRC2(rgSCHPfsDlLcCfg);

   if (rgSCHUtlAllocSBuf(cell->instIdx,
            (Data**)&cmnLc->schSpfc, (sizeof(RgSchPfsDlLc))) != ROK)
   {
      RETVALUE(RFAILED);
   }
   pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
   pfsLc->schedSvcLnk.node = (PTR)lc;
   pfsLc->prioLnk.node = (PTR)lc;
   pfsLc->svcPrioLnk.node = (PTR)lc;
   /*pfsLc->inQLnk.node = (PTR)lc;*/
   /* ambrLnk.node should be NULLP. It is set only when lc
    * is in pfs UE's AMBR list, which happens when it has
    * non-zero BO. */

   pfsLc->svcPrio    = rgSchPfsDlGetSvcPrio(pfsCell, cmnLc->prio);
   pfsLc->svccqiPrio = pfsLc->svcPrio * pfsUe->cqiPrio;
   /* MS_FIX : Updating PFS to start prio with highest possible value
      and diminishing prio val updates from there on. */
   pfsLc->pfsPrio = pfsCell->initPrioVal;

   if (RG_SCH_CMN_DL_SVC_IS_GBR(lc))
   {
#ifdef MSPD
      MSPD_DBG("RSYS:rgSCHPfsDlLcCfg: configuring GBR for UE rnti(%d), lcId(%d), qci(%d), svcPrio(%lu), svccqiPrio(%lu).\n", 
             ue->ueId, lc->lcId, cmnLc->prio,  pfsLc->svcPrio, pfsLc->svccqiPrio);
#endif
      pfsLc->gbr           = cmnLc->gbr;
      pfsLc->remGbr        = pfsLc->gbr;
      pfsLc->deltaMbr      = cmnLc->mbr - cmnLc->gbr;
      pfsLc->remDeltaMbr   = pfsLc->deltaMbr;
      pfsLc->lcType        = RG_SCH_PFS_LC_TYPE_GBR;
      pfsLc->cfgLcLnk.node = (PTR)lc;
      cmLListAdd2Tail(&pfsUe->cfgGbrLst, &pfsLc->cfgLcLnk);
      /* MS_FIX : Updating PFS to start prio with highest possible value
                  and diminishing prio val updates from there on */
      rgSCHPfsInitFracPrioInfo(&pfsLc->fracPrioInfo, pfsLc->gbr); 
   }
   else if (lc->lcId == 1 || lc->lcId == 2)
   {
#ifdef MSPD
      MSPD_DBG("RSYS:rgSCHPfsDlLcCfg: configuring SRB for UE rnti(%d), lcId(%d), qci(%d), svcPrio(%lu), svccqiPrio(%lu).\n", 
             ue->ueId, lc->lcId, cmnLc->prio,  pfsLc->svcPrio, pfsLc->svccqiPrio);
#endif
      pfsLc->lcType = RG_SCH_PFS_LC_TYPE_SRB;
   }
   else if (cmnLc->prio == 0)
   {
#ifdef MSPD
      MSPD_DBG("RSYS:rgSCHPfsDlLcCfg: configuring DRB0 for UE rnti(%d), lcId(%d), qci(%d), svcPrio(%lu), svccqiPrio(%lu).\n", 
             ue->ueId, lc->lcId, cmnLc->prio,  pfsLc->svcPrio, pfsLc->svccqiPrio);
#endif
      pfsLc->lcType = RG_SCH_PFS_LC_TYPE_DRB0;
   }
   else
   {
      pfsLc->lcType = RG_SCH_PFS_LC_TYPE_AMBR;
      pfsLc->cfgLcLnk.node = (PTR)lc;
      cmLListAdd2Tail(&pfsUe->cfgAmbrLst, &pfsLc->cfgLcLnk);
#ifdef MSPD
      MSPD_DBG("RSYS:rgSCHPfsDlLcCfg: configuring AMBR for UE rnti(%d), lcId(%d), qci(%d), svcPrio(%lu), svccqiPrio(%lu).\n", 
             ue->ueId, lc->lcId, cmnLc->prio,  pfsLc->svcPrio, pfsLc->svccqiPrio);
#endif
   }

   RETVALUE(ROK);
}

/**
 * @brief LC re-configuration for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsDlLcRecfg
 *
 *     Processing Steps:
 *      - If DL LC is re-configured
 *        - If LC type is GBR
 *          - Update gbr
 *          - Update deltaMbr
 *      - [Need not do other changes for QoS update,
 *        vars update at refresh cycle shall take
 *        care of things]
 *      - [Not supporting LC QCI which is common with other
 *        schedulers]
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlLcCb      *lc
 *  @param[in]  RgrLchRecfg      *recfg
 *  @param[out] RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPfsDlLcRecfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchDlLcCb      *lc,
RgrLchRecfg      *recfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHPfsDlLcRecfg(cell, ue, lc, recfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchDlLcCb      *lc;
RgrLchRecfg      *recfg;
RgSchErrInfo     *err;
#endif
{
   RgSchCmnDlSvc      *cmnLc = (RgSchCmnDlSvc *)lc->sch;
   RgSchPfsDlLc       *pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);

   UNUSED(cell);
   UNUSED(ue);
   UNUSED(recfg);
   UNUSED(err);
   TRC2(rgSCHPfsDlLcRecfg);

   /* Common module is not supporting QCI change, so
    * don't bother about that */

   if (pfsLc->lcType == RG_SCH_PFS_LC_TYPE_GBR)
   {
      pfsLc->gbr      = cmnLc->gbr;
      pfsLc->deltaMbr = cmnLc->mbr - cmnLc->gbr;
      /* These shall automatically be in effect in the next refresh cycle */
   }

   RETVALUE(ROK);
}

/**
 * @brief LC deletion for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsDlLcDel
 *
 *     Processing Steps:
 *     - Remove LC from queues if present
 *     - Delete pfsDlLc info
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlLcCb      *lc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsDlLcDel
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchDlLcCb      *lc
)
#else
PUBLIC Void rgSCHPfsDlLcDel(cell, ue, lc)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchDlLcCb      *lc;
#endif
{
   RgSchPfsDlCell     *pfsCell = RG_SCH_PFS_GET_PFSCELLDL(cell);
   RgSchPfsDlUe       *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
   RgSchCmnDlSvc      *cmnLc = (RgSchCmnDlSvc *)lc->sch;
   RgSchPfsDlLc       *pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
   TRC2(rgSCHPfsDlLcDel);

   if (pfsLc == NULLP)
   {
      RETVOID;
   }

   rgSCHPfsDlLcRmvFrmPrioQ(pfsCell, pfsUe, pfsLc);
   if (pfsLc->lcType == RG_SCH_PFS_LC_TYPE_AMBR)
   {
      rgSCHPfsDlLcAmbrInactv(pfsCell, ue, lc);
      cmLListDelFrm(&pfsUe->cfgAmbrLst, &pfsLc->cfgLcLnk);
      pfsLc->cfgLcLnk.node = NULLP;
   }
   else if (pfsLc->lcType == RG_SCH_PFS_LC_TYPE_GBR)
   {
      cmLListDelFrm(&pfsUe->cfgGbrLst, &pfsLc->cfgLcLnk);
      pfsLc->cfgLcLnk.node = NULLP;
   }
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(cmnLc->schSpfc)), (sizeof(RgSchPfsDlLc)));
   RETVOID;
}

/**
 * @brief Indication of list of UEs which should not be
 *        scheduled
 *
 * @details
 *
 *     Function : rgSCHPfsDlInactvLstInd
 *
 *     Processing Steps:
 *      - Indication for set of UEs for which downlink
 *        scheduling should not be done.
 *        Triggers removal of logical channels of UEs
 *        from queues.
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsDlInactvLstInd
(
RgSchCellCb      *cell,
CmLListCp        *lst
)
#else
PUBLIC Void rgSCHPfsDlInactvLstInd(cell, lst)
RgSchCellCb      *cell;
CmLListCp        *lst;
#endif
{
   CmLList         *lnk;
   TRC2(rgSCHPfsDlInactvLstInd);

   for (lnk = lst->first; lnk; lnk = lnk->next)
   {
      RgSchUeCb       *ue = (RgSchUeCb *)lnk->node;
      rgSCHPfsDlUeInactv(cell, ue);
   }
   RETVOID;
}

/**
 * @brief Indication for an inactive (not to be scheduled) UE
 *
 * @details
 *
 *     Function : rgSCHPfsDlUeInactv
 *
 *     Processing Steps:
 *      - Triggers temporary removal of logical channels of UEs
 *        from queues.
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlUeInactv
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PRIVATE Void rgSCHPfsDlUeInactv(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   RgSchPfsDlCell     *pfsCell = RG_SCH_PFS_GET_PFSCELLDL(cell);
   RgSchPfsDlUe       *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
   CmLList            *lnk;
   S16                 idx;
   TRC2(rgSCHPfsDlUeInactv);

   for (lnk = pfsUe->retxProcs.first; lnk; lnk = lnk->next)
   {
      RgSchDlHqProcCb    *proc = (RgSchDlHqProcCb *)lnk->node;

      /* Remove from cell retx list only, leaving it in UE retx list */
      rgSCHPfsDlProcRmvFrmCellRetx(cell, proc);
   }

   /*for (lnk = &pfsUe->inQLcLst; lnk; lnk = lnk->next)*/
   for (idx = 0; idx < RGSCH_MAX_LC_PER_UE; ++idx)
   {
      RgSchDlLcCb        *lc = ue->dl.lcCb[idx];
      RgSchPfsDlLc       *pfsLc;

      if (lc == NULLP)
      {
         continue;
      }
      pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
      rgSCHPfsDlLcRmvFrmPrioQ(pfsCell, pfsUe, pfsLc);
      /* Could have remembered original cell queue,
       * and put it back if BO has
       * not changed. This can be quicker, but has more
       * complexity (which might increase if reconfig
       * of certain priorities are considered).
       */
   }
   /* MS_FIX : syed Resetting ambrSvc with ambrLst.count>0 is invalid.
    * Note: rgSCHPfsDlLcRmvFrmPrioQ() does not alter ambrLst.count. It was
    * leading to a crash during activation. */
   /*pfsUe->ambrSvc = NULLP;*/

   RETVOID;
}

/**
 * @brief Indication that a UE is allowed to be scheduled
 *
 * @details
 *
 *     Function : rgSCHPfsDlActvUeInd
 *
 *     Processing Steps:
 *      - [This is an indication for UE for which downlink
 *        scheduling can now be done, since the last
 *        time it was not allowed.]
 *      - Triggers addition of HARQ procs for which retx
 *        is pending to be added back to the cell wide
 *        retx queues.
 *      - Triggers addition of logical channels to queues
 *        when needed.
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsDlActvUeInd
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PUBLIC Void rgSCHPfsDlActvUeInd(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   RgSchPfsDlUe       *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
   CmLList            *lnk;
   S16                 idx;
   TRC2(rgSCHPfsDlActvUeInd);

   for (lnk = pfsUe->retxProcs.first; lnk; lnk = lnk->next)
   {
      RgSchDlHqProcCb    *proc = (RgSchDlHqProcCb *)lnk->node;

      /* Add to cell retx list only (it is already in UE retx list) */
      rgSCHPfsDlProcAddToCellRetx(cell, proc);
   }

   /*for (lnk = &pfsUe->inQLcLst; lnk; lnk = lnk->next)*/
   for (idx = 0; idx < RGSCH_MAX_LC_PER_UE; ++idx)
   {
      RgSchDlLcCb         *lc = ue->dl.lcCb[idx];
#ifdef LTEMAC_SPS 
      RgSchCmnDlUeSpsInfo *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
#endif

      if ((lc == NULLP)
#ifdef LTEMAC_SPS 
       || (dlSpsUe->spsSvc == lc)
#endif
    )
      {
         continue;
      }
      rgSCHPfsDlBoUpdInt(cell, ue, lc, lc->bo, 0);
   }
   /* ambrSvc shall be set correctly as part of BoUpdInt for each
    * AMBR svc */

   RETVOID;
}

/**
 * @brief Downlink CQI Indication handler
 *
 * @details
 *
 *     Function : rgSCHPfsDlCqiInd
 *
 *     Processing Steps:
 *     - If CQI priority has changed due to wideband CQI change
 *       - Update CQI component of priority (specific to UE)
 *       - [Not doing proc updates] For each of UE's non SRB retx HARQ proc
 *         - Update HARQ retx PFS priority and corresponding positions
 *           in queues
 *       - For each DTCH
 *         - Update precomputed non BO component of PFS
 *           priority (svccqiPrio)
 *       - For each DTCH in cell-wide queues
 *         - Update priority
 *         - Position in different queue if needed
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  TfuDlCqiRpt      *cqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsDlCqiInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuDlCqiRpt          *cqiInfo
)
#else
PUBLIC Void rgSCHPfsDlCqiInd(cell, ue, cqiInfo)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
TfuDlCqiRpt          *cqiInfo;
#endif
{
   RgSchPfsDlCell      *pfsCell = RG_SCH_PFS_GET_PFSCELLDL(cell);
   RgSchCmnDlUe        *cmnUe = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchPfsDlUe        *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
   U8                   cqi;
   U32                  cqiPrio;
   CmLList             *lnk;
   Bool                prioChngd;

   UNUSED(cqiInfo);
   TRC2(rgSCHPfsDlCqiInd);

   /* MS_FIX : syed Considering RI and CW1's CQI */
   cqi = ((cmnUe->mimoInfo.cwInfo[0].noLyr * cmnUe->mimoInfo.cwInfo[0].cqi) + 
          (cmnUe->mimoInfo.cwInfo[1].noLyr * cmnUe->mimoInfo.cwInfo[1].cqi))/
	  (cmnUe->mimoInfo.cwInfo[0].noLyr + cmnUe->mimoInfo.cwInfo[1].noLyr);

   cqiPrio = rgSCHPfsDlGetCqiPrio(pfsCell, cqi);
   /* MS_FIX : syed Considering RI and CW1's CQI */
   cqiPrio = RGSCH_CEIL(cqiPrio, cmnUe->mimoInfo.ri);
   if (cqiPrio == pfsUe->cqiPrio)
   {
      RETVOID;
   }
   pfsUe->cqiPrio = cqiPrio;

   /* Update GBR/MBR LCs */
   for (lnk = pfsUe->cfgGbrLst.first; lnk; lnk = lnk->next)
   {
      RgSchDlLcCb         *lc = (RgSchDlLcCb *)lnk->node;
      RgSchPfsDlLc        *pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
      pfsLc->svccqiPrio = pfsLc->svcPrio * cqiPrio;
      /* MS_FIX : Updating PFS to start prio with highest possible value
                  and diminishing prio val updates from there on */
      rgSCHPfsDlUpdPfsPrio(pfsCell, pfsLc, &pfsLc->fracPrioInfo, &prioChngd);
      /* MS_FIX: Updating LC/UEs position in scheduling Queues should be
       * done only if UE is Active */
      if (RG_SCH_CMN_DL_IS_UE_ACTIVE(ue))
      {
         if (pfsLc->remGbr)
         {
            /* Takes care of updating/retaining queue */
            rgSCHPfsDlLcAddToGbrQ(pfsCell, pfsUe, pfsLc, prioChngd, 0);
         }
         else
         {
            /* Takes care of updating/retaining queue */
            rgSCHPfsDlLcAddToMbrQ(pfsCell, pfsUe, pfsLc, prioChngd, 0);
         }
      }
   }

   /* Update AMBR LCs */
   for (lnk = pfsUe->cfgAmbrLst.first; lnk; lnk = lnk->next)
   {
      RgSchDlLcCb         *lc = (RgSchDlLcCb *)lnk->node;
      RgSchPfsDlLc        *pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
      pfsLc->svccqiPrio = pfsLc->svcPrio * cqiPrio;
      /* MS_FIX : Updating PFS to start prio with highest possible value
                  and diminishing prio val updates from there on */
      rgSCHPfsDlUpdPfsPrio(pfsCell, pfsLc, &pfsLc->fracPrioInfo, &prioChngd);
   }
   if (RG_SCH_CMN_DL_IS_UE_ACTIVE(ue))
   {
      if (pfsUe->ambrSvc)
      {
         RgSchDlLcCb         *lc = pfsUe->ambrSvc;
         RgSchPfsDlLc        *pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
         /*rg002.301_LPR1-MOD-Modified the invocation of this function to
          * add a new parameter allocd.*/
         rgSCHPfsDlLcAddToAmbrQ(pfsCell, pfsUe, pfsLc, 0);
      }
   }

   RETVOID;
}

/**
 * @brief DL scheduling by PFS
 *
 * @details
 *
 *     Function : rgSCHPfsDlSched
 *
 *     Processing Steps:
 *     - Schedule TAs
 *     - Schedule retransmissions
 *     - Schedule new transmissions
 *
 *  @param[in]  RgSchCellCb              *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo    *alloc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsDlSched
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *alloc
)
#else
PUBLIC Void rgSCHPfsDlSched(cell, alloc)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *alloc;
#endif
{
   TRC2(rgSCHPfsDlSched);

   rgSCHPfsDlSchedTa(cell, alloc);
   rgSCHPfsDlSchedRetx(cell, alloc);
   /* MS_FIX: If this is considered as a dummy TTI then the scheduling for new 
    * transmissions for all the UEs are blocked to improve the performance 
    * and pull the system from L2 processing going beyond the TTI boundary 
    * Please port this fix to other schedulers as well. 
    */
   if (!cell->isDummyTti)
   {
      rgSCHPfsDlSchedNewTx(cell, alloc);
   }

   RETVOID;
}

/**
 * @brief Scheduling of UEs for TA
 *
 * @details
 *
 *     Function : rgSCHPfsDlSchedTa
 *
 *     Processing Steps:
 *     - Schedules UEs for TA
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *alloc
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlSchedTa
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *alloc
)
#else
PRIVATE Void rgSCHPfsDlSchedTa(cell, alloc)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *alloc;
#endif
{
   RgSchCmnDlCell       *cmnCell = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchDlSf            *sf        = alloc->dedAlloc.dedDlSf;
   CmLList              *lnk;
#ifdef LTEMAC_HDFDD
   Bool                 dlAllowed = FALSE;
#endif
   U8                   remUe; 

   TRC2(rgSCHPfsDlSchedTa);

   /* MS_FIX: MUE_PERTTI_DL syed Limiting number of TA scheduling. Please
    * check in remUeCnt handling in oher schedulers as well. */
   if (sf->remUeCnt < cmnCell->maxUeNewTxPerTti)
   {
      remUe = sf->remUeCnt;	   
   }
   else
   {
      remUe = cmnCell->maxUeNewTxPerTti;	   
   }
   for (lnk = cmnCell->taLst.first; (lnk && remUe); lnk = lnk->next)
   {
      RgSchUeCb            *ue = (RgSchUeCb *)lnk->node;
#ifdef LTEMAC_HDFDD
      if (RG_SCH_HDFDD_UE_ENBLD(ue))
      {
         rgSCHCmnHdFddChkDlAllow ( cell, ue, &dlAllowed);
         if (dlAllowed == FALSE)
         {
            continue;
         }
      }
#endif
      if (rgSCHPfsDlSchedTaForUe(cell, alloc, sf, ue) != ROK)
      {
         continue;	      
      }
#ifdef MSPD
      MSPD_DBG("<TA> UE(%d) RB EST from taLst(count=%d)\n",ue->ueId, cmnCell->taLst.count);
#endif
      remUe--;
   }
   /* MS_FIX: MUE_PERTTI_DL syed Limiting number of TA scheduling. 
    * This check is to handle both TA->RETX and RETX->TA sched order. */
   if (sf->remUeCnt < cmnCell->maxUeNewTxPerTti)
   {
      sf->remUeCnt = remUe;
   }
   else
   {
      sf->remUeCnt -=(cmnCell->maxUeNewTxPerTti - remUe);
   }
   RETVOID;
}

/**
 * @brief Scheduling of UE for TA
 *
 * @details
 *
 *     Function : rgSCHPfsDlSchedTaForUe
 *
 *     Processing Steps:
 *     - Schedules UE for TA.
 *     - This relies on the fact that TA is scheduled
 *       before anything else (even before retx), and
 *       hence does not perform checks such as if
 *       UE is already scheduled in the subframe.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *alloc
 *  @param[in]  RgSchDlSf              *sf
 *  @param[in]  RgSchUeCb              *ue
 *  @return  S16
 **/
#ifdef ANSI
PRIVATE S16 rgSCHPfsDlSchedTaForUe
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *alloc,
RgSchDlSf               *sf,
RgSchUeCb               *ue
)
#else
PRIVATE S16 rgSCHPfsDlSchedTaForUe(cell, alloc, sf, ue)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *alloc;
RgSchDlSf               *sf;
RgSchUeCb               *ue;
#endif
{
   /* [Later] Returning a value may not really be needed */
   RgSchPfsDlCell       *pfsCell = RG_SCH_PFS_GET_PFSCELLDL(cell);
   RgSchCmnDlUe         *cmnUe = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchPfsDlUe         *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
   RgSchDlHqProcCb      *proc;
   RgSchCmnDlHqProc     *cmnProc;
   U32                   bytes = 0;
   CmLListCp            *retxQ   = &pfsCell->retxQueues.srbLst;
#ifdef LTEMAC_SPS
   CmLteTimingInfo      schdTime;
#endif
   TRC2(rgSCHPfsDlSchedTaForUe);

#ifdef LTEMAC_SPS
   schdTime = cell->crntTime;

   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);
   if (RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, schdTime))
   {
      RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx), "\n"
               "Ue already scheduled by SPS module %d", ue->ueId));
      RETVALUE(RFAILED);
   }
#endif
#if (ERRCLASS & ERRCLS_DEBUG)
   if (RG_SCH_CMN_IS_UE_SCHDLD(ue))
   {
      proc = (RgSchDlHqProcCb *)(cmnUe->proc);
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),
               "rgSCHPfsDlSchedTaForUe(): UE=%d is already scheduled for "
               "this SF even though TA for UE is being scheduled\n",
               ue->ueId));
      MSPD_ERR("UE=%d is already scheduled for "
               "this SF even though TA for UE is being scheduled\n",
               ue->ueId);
	  RETVALUE(RFAILED);
   }
#endif

   if (!RG_SCH_CMN_DL_IS_UE_ACTIVE(ue))
   {
      RETVALUE(RFAILED);
   }

   if ((proc = rgSCHPfsDlGetAvlProc(cell, ue, retxQ)) == NULLP)
   {
      RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),
               "rgSCHPfsDlSchedTaForUe(): Could not get HARQ proc for UE %d \n",
               ue->ueId));
      RETVALUE(RFAILED);
   }

   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependncy on MIMO compile-time flag */
   rgSCHCmnDlAllocTxRb(cell, sf, ue, RGSCH_TA_SIZE, &bytes, proc, alloc);
   /* TA size is 2 bytes, corresponding to the smallest possible
    * allocation. So check for zero allocation is enough to check
    * if there isn't sufficient allocation for TA. */
   if (bytes == 0)
   {
      rgSCHPfsDlRlsHqProc(ue, proc);
      RETVALUE(RFAILED);
   }
   pfsUe->taReq = TRUE;
   cmnUe->proc = proc;
   cmnProc = RG_SCH_CMN_GET_DL_HQP(proc);
   cmnProc->totBytes += bytes;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependncy on MIMO compile-time flag */
   /* moving this call to cmn scheduler */
   /*rgSCHCmnDlRbInfoAddUeTx(allocInfo, ue);*/
   RETVALUE(ROK);
}

/**
 * @brief Scheduling of DL re-transmissions
 *
 * @details
 *
 *     Function : rgSCHPfsDlSchedRetx
 *
 *     Processing Steps:
 *     - Traverse queues in same order as for new transmissions
 *       (this time, there shall be HARQ procs instead of services)
 *       - For each proc in the current queue
 *         - Skip if proc is already scheduled
 *         - Invoke common module to make a tentative allocation
 *           for proc
 *         - Add UE to alloc retx list with appropriate variables
 *           filled in
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[out] RgSchCmnDlRbAllocInfo  *alloc
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlSchedRetx
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *alloc
)
#else
PRIVATE Void rgSCHPfsDlSchedRetx(cell, alloc)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *alloc;
#endif
{
   RgSchPfsDlCell     *pfsCell = RG_SCH_PFS_GET_PFSCELLDL(cell);
   RgSchPfsDlQueues   *retxQs  = &pfsCell->retxQueues;
   CmLListCp          *retxQ;
   S16                 idx;
   TRC2(rgSCHPfsDlSchedRetx);
   
  
   retxQ = &retxQs->srbLst;
   rgSCHPfsDlRetxInQ(cell, alloc, retxQ);

   retxQ = &retxQs->drb0Lst;
   rgSCHPfsDlRetxInQ(cell, alloc, retxQ);

   for (idx = RG_SCH_PFS_DL_NUM_GBR_QUEUES-1; idx >= 0; --idx)
   {
      retxQ = &retxQs->gbrLst[idx];
      rgSCHPfsDlRetxInQ(cell, alloc, retxQ);
   }

   for (idx = RG_SCH_PFS_DL_NUM_MBR_QUEUES-1; idx >= 0; --idx)
   {
      retxQ = &retxQs->mbrLst[idx];
      rgSCHPfsDlRetxInQ(cell, alloc, retxQ);
   }

   for (idx = RG_SCH_PFS_DL_NUM_AMBR_QUEUES-1; idx >= 0; --idx)
   {
      retxQ = &retxQs->ambrLst[idx];
      rgSCHPfsDlRetxInQ(cell, alloc, retxQ);
   }

   RETVOID;
}

/**
 * @brief Scheduling of DL new transmissions
 *
 * @details
 *
 *     Function : rgSCHPfsDlSchedNewTx
 *
 *     Processing Steps:
 *     - For each SRB in SRB queue
 *       - Skip if UE scheduled for retx
 *       - Schedule SRB
 *     - In DRB 0 queue
 *       - For each svc in queue
 *         - Skip if UE scheduled for retx
 *         - Schedule upto BO
 *     - For each GBR queue (in decreasing order of priority)
 *       - For each svc in queue
 *         - Skip if UE scheduled for retx
 *         - Schedule upto min(BO, GBR)
 *     - For each MBR queue (in decreasing order of priority)
 *       - For each svc in queue
 *         - Skip if UE scheduled for retx
 *         - Schedule upto min(BO, MBR)
 *     - For each AMBR queue (in decreasing order of priority)
 *       - For each svc in queue
 *         - If (AMBR applicable)
 *           - Schedule upto min(BO, AMBR)
 *         - Else
 *           - Schedule upto BO
 *     - While processing each svc in the above steps, add
 *       UE to list of scheduled UEs in the 'alloc' structure
 *       with appropriate info filled in.
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[out] RgSchCmnDlRbAllocInfo   *alloc
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlSchedNewTx
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *alloc
)
#else
PRIVATE Void rgSCHPfsDlSchedNewTx(cell, alloc)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *alloc;
#endif
{
   RgSchPfsDlCell        *pfsCell = RG_SCH_PFS_GET_PFSCELLDL(cell);
   RgSchPfsDlTxQueues    *txQs  = &pfsCell->txQueues;
   RgSchPfsDlRetxQueues  *retxQs  = &pfsCell->retxQueues;
   CmLListCp             *txQ;
   CmLListCp             *retxQ;
   S16                    idx;
   Bool                   isBwLeft = TRUE;
   /* MS_FIX: MUE_PERTTI_DL */
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchDlSf    *sf         = alloc->dedAlloc.dedDlSf;
   /* MS_FIX: MUE_PERTTI_DL syed UL cfg was being used instead of DL */
   sf->remUeCnt = (sf->remUeCnt < schCmnCell->dl.maxUeNewTxPerTti)? 
                   sf->remUeCnt : schCmnCell->dl.maxUeNewTxPerTti;

   TRC2(rgSCHPfsDlSchedNewTx);
   
   txQ = &txQs->srbLst;
   retxQ = &retxQs->srbLst;
   rgSCHPfsDlNewTxInQ(cell, alloc, txQ, retxQ, &isBwLeft);

   txQ = &txQs->drb0Lst;
   retxQ = &retxQs->drb0Lst;
   rgSCHPfsDlNewTxInQ(cell, alloc, txQ, retxQ, &isBwLeft);

   /* Beginning bw check once the queues with heavier requirements are
    * traversed */

   for (idx = RG_SCH_PFS_DL_NUM_GBR_QUEUES-1; isBwLeft && (idx >= 0); --idx)
   {
      txQ = &txQs->gbrLst[idx];
      retxQ = &retxQs->gbrLst[idx];
      rgSCHPfsDlNewTxInQ(cell, alloc, txQ, retxQ, &isBwLeft);
   }

   for (idx = RG_SCH_PFS_DL_NUM_MBR_QUEUES-1; isBwLeft && (idx >= 0); --idx)
   {
      txQ = &txQs->mbrLst[idx];
      retxQ = &retxQs->mbrLst[idx];
      rgSCHPfsDlNewTxInQ(cell, alloc, txQ, retxQ, &isBwLeft);
   }

   for (idx = RG_SCH_PFS_DL_NUM_AMBR_QUEUES-1; isBwLeft && (idx >= 0); --idx)
   {
      txQ = &txQs->ambrLst[idx];
      retxQ = &retxQs->ambrLst[idx];
      rgSCHPfsDlNewTxInQ(cell, alloc, txQ, retxQ, &isBwLeft);
   }

   RETVOID;
}

/**
 * @brief Scheduling of DL new transmissions in a queue
 *
 * @details
 *
 *     Function : rgSCHPfsDlNewTxInQ
 *
 *     Processing Steps:
 *     - For each LC in queue
 *       - If UE is not scheduled for retx
 *         - Schedule for new tx
 *       - Else if UE already scheduled for new tx
 *         - Add to existing allocation
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in] RgSchCmnDlRbAllocInfo  *alloc
 *  @param[in]  CmLListCp              *txQ
 *  @param[in]  CmLListCp              *retxQ
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlNewTxInQ
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *alloc,
CmLListCp               *txQ,
CmLListCp               *retxQ,
Bool                    *isBwLeftRef
)
#else
PRIVATE Void rgSCHPfsDlNewTxInQ(cell, alloc, txQ, retxQ, isBwLeftRef)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *alloc;
CmLListCp               *txQ;
CmLListCp               *retxQ;
Bool                    *isBwLeftRef;
#endif
{
   RgSchDlSf          *sf      = alloc->dedAlloc.dedDlSf;
   CmLList            *lnk;
#ifdef LTEMAC_HDFDD
   Bool               dlAllowed = FALSE;
#endif
  
   TRC2(rgSCHPfsDlNewTxInQ);

   for (lnk = txQ->first; lnk ; lnk = lnk->next)
   {
      RgSchDlLcCb      *lc = (RgSchDlLcCb *)lnk->node;
#ifdef LTEMAC_HDFDD
      if (RG_SCH_HDFDD_UE_ENBLD(lc->ue))
      {
         rgSCHCmnHdFddChkDlAllow ( cell, lc->ue, &dlAllowed);
         if (dlAllowed == FALSE)
         {
            continue;
         }
      }
#endif
      /* MS_FIX: MUE_PERTTI_DL */
      if (!sf->remUeCnt)
      {
         /* In no more UE can be accomodated in sf, 
            do not schedule LC of a new UE */
         break;
      }
      rgSCHPfsDlLcSched(cell, alloc, sf, lc->ue, lc, retxQ, isBwLeftRef);
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPfsDlLcSched
 *
 *     Desc : Schedule LC. Takes care of doing appropriate
 *            processing to check if UE was already scheduled
 *            for this subframe, and if it was, it wasn't
 *            a retx that it was scheduled for.
 *            Calls common module to figure out number of
 *            bytes tentatively allocd. [One drawback is that
 *            this may cut down allocation in some cases as
 *            it figures out an appropriate TB size match
 *            close to needed bytes. But it is good since
 *            the numbers are taken as is when DLFS is not
 *            invoked.]
 *            Also if the LC was scheduled, adds it to the
 *            scratchpad of list of svcs to be scheduled
 *            in this subframe.
 *            Also adds UE to alloc's newTx list if not
 *            already added.
 *
 *     Ret  : ROK/RFAILED
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHPfsDlLcSched
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *alloc,
RgSchDlSf             *sf,
RgSchUeCb             *ue,
RgSchDlLcCb           *lc,
CmLListCp             *retxQ,
Bool                  *isBwLeftRef
)
#else
PRIVATE S16 rgSCHPfsDlLcSched(cell, alloc, sf, ue, lc, retxQ, isBwLeftRef)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *alloc;
RgSchDlSf             *sf;
RgSchUeCb             *ue;
RgSchDlLcCb           *lc;
CmLListCp             *retxQ;
Bool                  *isBwLeftRef;
#endif
{
   RgSchCmnDlUe       *cmnUe = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchPfsDlUe       *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
   RgSchDlHqProcCb    *proc;
   RgSchCmnDlHqProc   *cmnProc;
   RgSchPfsDlLc       *pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
   U32                 bytes;
#ifdef LTEMAC_SPS
   CmLteTimingInfo      schdTime;
#endif
   TRC2(rgSCHPfsDlLcSched);

   *isBwLeftRef = TRUE;
#ifdef LTEMAC_SPS
   schdTime = cell->crntTime;

   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);
   if (RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, schdTime))
   {
      RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx), "\n"
               "Ue already scheduled by SPS module %d", ue->ueId));
      RETVALUE(RFAILED);
   }
#endif

   if (RG_SCH_CMN_IS_UE_SCHDLD(ue))
   {
      proc = (RgSchDlHqProcCb *)cmnUe->proc;
      /* Don't do anything if UE already scheduled for retx */
      if (RG_SCH_CMN_PROC_SLCTD_FOR_RETX(proc))
      {
         RETVALUE(RFAILED);
      }
      /* UE is scheduled for either other services or TA */
      /* pfsProc->retxLstCp would have been set the first
       * time proc was obtained */
   }
   else
   {
      if ((proc = rgSCHPfsDlGetAvlProc(cell, ue, retxQ)) == NULLP)
      {
         RETVALUE(RFAILED);
      }
      /* MS_FIX: MUE_PERTTI_DL syed. */
      --sf->remUeCnt;
   }
   cmnProc = (RgSchCmnDlHqProc *)proc->sch;
   RG_SCH_CMN_DL_GET_HDR_EST(lc, pfsLc->hdrSz);
   pfsLc->reqBytes = pfsLc->effBo;
   pfsLc->reqBytes += pfsLc->hdrSz;
   if (rgSCHCmnDlAllocTxRb(cell, sf, ue, pfsLc->reqBytes, &bytes, proc, alloc)
         != ROK)

   {
      *isBwLeftRef = FALSE;
      bytes = 0;
   }
   if (bytes == 0)
   {
      if (cmnProc->totBytes == 0)
      {
         rgSCHPfsDlRlsHqProc(ue, proc);
         /*rg002.301 - ADD -[ccpu00120941] Added the handling for removing
          * UE from txUeLst and resetting outStndAlloc.*/
         if(proc->reqLnk.node != (PTR)NULLP)
         {
            cmLListDelFrm(&alloc->dedAlloc.txUeLst, &proc->reqLnk);
            proc->reqLnk.node = (PTR)NULLP;
         }
         /*Re-set the outstanding alloc information.*/
         cmnUe->outStndAlloc = 0;
         /* MS_FIX : syed proc should be set to NULLP in UE's DL scratch pad info as well. */
	     cmnUe->proc = NULLP;
         /* MS_FIX: MUE_PERTTI_DL syed*/
         ++sf->remUeCnt;
      }
      RETVALUE(RFAILED);
   }
   cmLListAdd2Tail(&pfsUe->schedSvcs, &pfsLc->schedSvcLnk);
   cmnUe->proc = proc;
   
   cmnProc = RG_SCH_CMN_GET_DL_HQP(proc);
   cmnProc->totBytes += bytes;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependncy on MIMO compile-time flag */
   /* It is moved to Cmn Scheduler */
   /* rgSCHCmnDlRbInfoAddUeTx(alloc, ue); */
   RETVALUE(ROK);
}


/**
 * @brief Scheduling of DL re-transmissions corresponding to
 *        a re-transmission queue
 *
 * @details
 *
 *     Function : rgSCHPfsDlRetxInQ
 *
 *     Processing Steps:
 *     - For each proc in queue
 *       - If UE not scheduled, schedule HARQ process
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *alloc
 *  @param[in]  CmLListCp              *retxQ
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlRetxInQ
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *alloc,
CmLListCp               *retxQ
)
#else
PRIVATE Void rgSCHPfsDlRetxInQ(cell, alloc, retxQ)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *alloc;
CmLListCp               *retxQ;
#endif
{
   RgSchDlSf          *sf      = alloc->dedAlloc.dedDlSf;
   CmLList            *lnk;      
#ifdef LTEMAC_HDFDD
   Bool               dlAllowed = FALSE;
#endif
#ifdef LTEMAC_SPS
   CmLteTimingInfo      schdTime;
#endif
   /* MS_FIX: MUE_PERTTI_DL */
   U32 remUe = sf->remUeCnt;

   TRC2(rgSCHPfsDlRetxInQ);
#ifdef LTEMAC_SPS
      schdTime = cell->crntTime;

      /* Increment by DL DELTA to determine the time for which scheduling
       * is done */
      RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);
#endif

   for (lnk = retxQ->first; lnk && remUe; lnk = lnk->next)
   {
      RgSchDlHqProcCb     *proc = (RgSchDlHqProcCb *)lnk->node;
      RgSchUeCb           *ue = proc->hqE->ue;
      RgSchCmnDlUe        *cmnUe = RG_SCH_CMN_GET_DL_UE(ue);
      U32                  ret = 0;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependncy on MIMO compile-time flag */
      RgSchPfsDlUe         *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);

     
#ifdef LTEMAC_HDFDD
      if (RG_SCH_HDFDD_UE_ENBLD(ue))
      {
         rgSCHCmnHdFddChkDlAllow ( cell, ue, &dlAllowed);
         if (dlAllowed == FALSE)
         {
            continue;
         }
      }
#endif

      if ((RG_SCH_CMN_IS_UE_SCHDLD(ue))
#ifdef LTEMAC_SPS
            || (RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, schdTime)) 
#endif
         )
      {
         MSPD_DBG("FAILED (%d)\n", ue->ueId);	   
         continue;
      }
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependncy on MIMO compile-time flag */
      /* Extra check: indicate if there is furtherScope for NewTx
       * addition for a HqProc. This information will
       * be utilized by common scheduler, in case of SM
       * UEs with only one of the TBs retransmitting and the 
       * other TB can be used for clubbing new TX. */
      if (pfsUe->svcPriorLst.first != NULLP)
      {
         ue->dl.dlAllocCb.mimoAllocInfo.hasNewTxData = TRUE; 
      }
      /* 3.1 MIMO : last parameter changed */
      rgSCHCmnDlAllocRetxRb(cell, sf, ue, 0, &ret, proc, alloc);
      if (ret == 0)
      {
         MSPD_DBG("FAILED (%d:%d)\n", ue->ueId, proc->procId);	   
         continue;
      }
      cmnUe->proc = proc;
      --remUe;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependncy on MIMO compile-time flag */
      /* 3.1 MIMO moving this call in cmn scheduler */
      /*rgSCHCmnDlRbInfoAddUeRetx(allocInfo, ue);*/
   }
   sf->remUeCnt = remUe;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPfsDlGetAvlProc
 *
 *     Desc : Gets an available HARQ proc by invoking DHM,
 *            and initialises it.
 *            Returns NULL if no proc available.
 *
 *     Ret  : RgSchDlHqProcCb *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE RgSchDlHqProcCb *rgSCHPfsDlGetAvlProc
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLListCp           *retxQ
)
#else
PRIVATE RgSchDlHqProcCb *rgSCHPfsDlGetAvlProc(cell, ue, retxQ)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
CmLListCp           *retxQ;
#endif
{
   RgSchCmnDlCell       *cmnCell = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchDlHqProcCb      *proc;
   RgSchPfsDlHqProc     *pfsProc;
   RgSchCmnDlHqProc     *cmnProc;
   TRC2(rgSCHPfsDlGetAvlProc);

   if (rgSCHDhmGetAvlHqProc(ue, cmnCell->time, &proc) != ROK)
   {
      RETVALUE(NULLP);
   }
   cmnProc = (RgSchCmnDlHqProc *)proc->sch;
   cmnProc->totBytes = 0;
   pfsProc = RG_SCH_PFS_GET_PFSPROCDL(proc);
   pfsProc->retxLstCp = retxQ;
   RETVALUE(proc);
}

/***********************************************************
 *
 *     Func : rgSCHPfsDlResetProcs
 *
 *     Desc : Manages removing procs from queues and
 *            releasing them. Called during UE reset
 *            and delete.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlResetProcs
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else
PRIVATE Void rgSCHPfsDlResetProcs(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif
{
   RgSchDlHqEnt        *hqEnt = ue->dl.hqEnt;
   S16                  idx;
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependncy on MIMO compile-time flag */
   TRC2(rgSCHPfsDlResetProcs);

   for(idx = 0; idx < hqEnt->numHqPrcs; idx++)
   {
      RgSchDlHqProcCb  *proc    = &ue->dl.hqEnt->procs[idx];
      RgSchPfsDlHqProc *pfsProc = RG_SCH_PFS_GET_PFSPROCDL(proc);
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependncy on MIMO compile-time flag */
      rgSCHPfsDlProcRmvFrmRetx(cell, ue, proc);
      /*MS_WORKAROUND for ccpu00122892 */
      if ((proc->tbInfo[0].subFrm != NULLP) &&
          (proc->tbInfo[0].sfLnk.node != NULLP))
      {
         cmLListDelFrm(&proc->tbInfo[0].subFrm->pdcchInfo.pdcchs,
                  &proc->tbInfo[0].pdcch->lnk);
         cmLListAdd2Tail(&cell->pdcchLst, &proc->tbInfo[0].pdcch->lnk);
         /* MS_FIX */
         rgSCHUtlDlHqPTbRmvFrmTx(proc->tbInfo[0].subFrm, proc, 0, FALSE);
      }
      if ((proc->tbInfo[1].subFrm != NULLP) &&
               (proc->tbInfo[1].sfLnk.node != NULLP))
      {
	 if (proc->tbInfo[1].pdcch != proc->tbInfo[0].pdcch)
	 {
#ifdef MSPD
	    MSPD_DBG("<RESET> Already cleared as part of TB0\n");
#endif
            cmLListDelFrm(&proc->tbInfo[1].subFrm->pdcchInfo.pdcchs,
                  &proc->tbInfo[1].pdcch->lnk);
            cmLListAdd2Tail(&cell->pdcchLst, &proc->tbInfo[1].pdcch->lnk);
         }
         /* MS_FIX */
         rgSCHUtlDlHqPTbRmvFrmTx(proc->tbInfo[1].subFrm, proc, 1, FALSE);
      }
      pfsProc->retxLstCp = NULLP;
   }
#ifdef MSPD
   MSPD_DBG("<RESET> DONE\n");
#endif

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPfsDlRlsHqProc
 *
 *     Desc : Does a (premature) release of a HARQ process.
 *
 *     Ret  : Void
 *
 *     Notes: This is not to be used when the HARQ process
 *            has already been used at least once for
 *            transmission. (The ndi toggling is not
 *            necessary in that case.)
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlRlsHqProc
(
RgSchUeCb           *ue,
RgSchDlHqProcCb     *proc
)
#else
PRIVATE Void rgSCHPfsDlRlsHqProc(ue, proc)
RgSchUeCb           *ue;
RgSchDlHqProcCb     *proc;
#endif
{
   RgSchPfsDlHqProc   *pfsProc = RG_SCH_PFS_GET_PFSPROCDL(proc);
   UNUSED(ue);
   TRC2(rgSCHPfsDlRlsHqProc);

   pfsProc->retxLstCp = NULLP;
   rgSCHDhmRlsHqProc(proc);
   RETVOID;
}


/**
 * @brief Indication of HARQ proc pending for retransmission
 *
 * @details
 *
 *     Function : rgSCHPfsDlProcRetx
 *
 *     Processing Steps:
 *     - The corresponding HARQ proc has to be queued up
 *       for retx
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchDlHqProcCb        *proc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsDlProcRetx
(
RgSchCellCb         *cell,
RgSchDlHqProcCb     *proc
)
#else
PUBLIC Void rgSCHPfsDlProcRetx(cell, proc)
RgSchCellCb         *cell;
RgSchDlHqProcCb     *proc;
#endif
{
   RgSchUeCb          *ue  = proc->hqE->ue;
   RgSchPfsDlUe *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
   RgSchPfsDlHqProc *pfsProc = RG_SCH_PFS_GET_PFSPROCDL(proc);
   
   TRC2(rgSCHPfsDlProcRetx);
   
   cmLListAdd2Tail(&pfsUe->retxProcs, &pfsProc->ueRetxLnk);
   pfsProc->ueRetxLnk.node = (PTR)proc;

   /* Add only if the UE is active */
   if (RG_SCH_CMN_DL_IS_UE_ACTIVE(ue))
   {
      rgSCHPfsDlProcAddToRetx(cell, ue, proc);
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPfsDlProcAddToRetx
 *
 *     Desc : Add HARQ proc to cell and ue retx list.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlProcAddToRetx
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchDlHqProcCb     *proc
)
#else
PRIVATE Void rgSCHPfsDlProcAddToRetx(cell, ue, proc)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
RgSchDlHqProcCb     *proc;
#endif
{
   UNUSED(cell);

   TRC2(rgSCHPfsDlProcAddToRetx);

   rgSCHPfsDlProcAddToCellRetx(cell, proc);
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPfsDlProcAddToCellRetx
 *
 *     Desc : Add retx HARQ proc only to cell retx list.
 *            Not adding to UE list of retx procs. This is
 *            used in the case when scheduling gap for UE
 *            is over and UE can again be considered for
 *            scheduling.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlProcAddToCellRetx
(
RgSchCellCb         *cell,
RgSchDlHqProcCb     *proc
)
#else
PRIVATE Void rgSCHPfsDlProcAddToCellRetx(cell, proc)
RgSchCellCb         *cell;
RgSchDlHqProcCb     *proc;
#endif
{
   RgSchPfsDlHqProc   *pfsProc = RG_SCH_PFS_GET_PFSPROCDL(proc);

   UNUSED(cell);

   TRC2(rgSCHPfsDlProcAddToCellRetx);

   cmLListAdd2Tail(pfsProc->retxLstCp, &pfsProc->retxLnk);
   pfsProc->retxLnk.node = (PTR)proc;
   RETVOID;
}

/* MS_WORKAROUND for ccpu00122892 : Temp fix for erroeneous RETX Harq release by rgSCHCmnDlAllocRetxRb */
/***********************************************************
 *
 *     Func : rgSCHPfsDlProcRmvFrmRetx
 *
 *     Desc : Remove retx HARQ proc from cell retx list.
 *            Since retx procs are tracked in UE, remove
 *            from UE list also.
 *
 *     Ret  : Void
 *
 *     Notes: Not setting retxLstCp to NULL here. Proc
 *            may again need to be added to retx queues due
 *            to NACK.
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHPfsDlProcRmvFrmRetx
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchDlHqProcCb     *proc
)
#else
PUBLIC Void rgSCHPfsDlProcRmvFrmRetx(cell, ue, proc)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
RgSchDlHqProcCb     *proc;
#endif
{
   TRC2(rgSCHPfsDlProcRmvFrmRetx);

   rgSCHPfsDlProcRmvFrmCellRetx(cell, proc);
   rgSCHPfsDlProcRmvFrmUeRetx(cell, ue, proc);
   /* This proc may not be free yet, do nothing else.*/
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPfsDlProcRmvFrmCellRetx
 *
 *     Desc : Remove retx HARQ proc only from cell retx
 *            list (and not from UE's list of retx procs).
 *            This is done during a gap indication.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlProcRmvFrmCellRetx
(
RgSchCellCb         *cell,
RgSchDlHqProcCb     *proc
)
#else
PRIVATE Void rgSCHPfsDlProcRmvFrmCellRetx(cell, proc)
RgSchCellCb         *cell;
RgSchDlHqProcCb     *proc;
#endif
{
   RgSchPfsDlHqProc   *pfsProc = RG_SCH_PFS_GET_PFSPROCDL(proc);

   UNUSED(cell);
   TRC2(rgSCHPfsDlProcRmvFrmCellRetx);

   if (pfsProc->retxLnk.node)
   {
      cmLListDelFrm(pfsProc->retxLstCp, &pfsProc->retxLnk);
      pfsProc->retxLnk.node = NULLP;
   }

   /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;
 
}

/***********************************************************
 *
 *     Func : rgSCHPfsDlProcRmvFrmUeRetx
 *
 *     Desc : Remove retx HARQ proc only from ue retx
 *            list (and not from cell's list of retx procs).
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlProcRmvFrmUeRetx
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchDlHqProcCb     *proc
)
#else
PRIVATE Void rgSCHPfsDlProcRmvFrmUeRetx(cell, ue, proc)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
RgSchDlHqProcCb     *proc;
#endif
{
   RgSchPfsDlUe       *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
   RgSchPfsDlHqProc   *pfsProc = RG_SCH_PFS_GET_PFSPROCDL(proc);

   UNUSED(cell);
   TRC2(rgSCHPfsDlProcRmvFrmUeRetx);

   if (pfsProc->ueRetxLnk.node)
   {
      cmLListDelFrm(&pfsUe->retxProcs, &pfsProc->ueRetxLnk);
      pfsProc->ueRetxLnk.node = NULLP;
   }
   RETVOID;
}

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
/***********************************************************
 *
 *     Func : rgSCHPfsDlNewTxTbDstn 
 *        
 *     Desc : Perform Actual allocation distribution among
 *     UEs schd svcs and TA for a given TB "tbInfo" allocation.
 *     Assumption: TA is given higher priority in Alloc Distribution.
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlNewTxTbDstn 
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgSchDlHqTbCb  *tbInfo,
U32            *effAlloc,
CmLList        **node
)
#else
PRIVATE Void rgSCHPfsDlNewTxTbDstn(cell, ue, tbInfo, effAlloc, node)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgSchDlHqTbCb  *tbInfo;
U32            *effAlloc;
CmLList        **node;
#endif
{

  RgSchDlLcCb        *lc;
  RgSchPfsDlLc       *pfsLc = NULLP;
  RgSchPfsDlUe       *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);  
  CmLList            *prev = NULLP;
  U32                bytes;

   TRC2(rgSCHPfsDlNewTxTbDstn);

   if (!tbInfo->schdTa.pres)
   {
      if (pfsUe->taReq)                  
      {
#ifdef MSPD
         MSPD_DBG("<TA> UE(%d) Finalization\n",ue->ueId);
#endif
         /* This is bound to not underflow. TA requires 2 bytes, and minimum
          * possible allocation is 2 bytes. */
         *effAlloc -= RGSCH_TA_SIZE; 
         rgSCHCmnRmvFrmTaLst(cell, ue);
         rgSCHDhmSchdTa(ue, tbInfo);
	 /* MS_FIX : syed Crash due to double delete from TaLst */
         pfsUe->taReq = FALSE;
      }
   }
   
   while(*node)
   {
      prev = *node;
      lc = (RgSchDlLcCb *)(*node)->node;
      pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
      *node = (*node)->next;
      bytes = RGSCH_MIN(*effAlloc, pfsLc->reqBytes);
      if (bytes <= pfsLc->hdrSz)
      {
         break;
      }
      else if ( pfsLc->reqBytes < *effAlloc)
      {
         rgSCHPfsDlLcFullyAllocd(cell, ue, lc, tbInfo);
         *effAlloc -= bytes; 
         pfsLc->reqBytes -= bytes;
      }
      else
      {
         rgSCHPfsDlLcPartlyAllocd(cell, ue, lc, *effAlloc, tbInfo);
         pfsLc->reqBytes -= *effAlloc;
         *effAlloc = 0;
      }
   }
   /* If no more scheduled LCs for TB data distribution
    * then distribute the spare TB data among the LCs
    * of the UE with non-zero BO. This is effective for 
    * schedulers work on LC level priorities rather than
    * UE level. */
   if ((*node == NULLP) && (pfsLc) && (pfsLc->reqBytes == 0))
   {
      rgSCHPfsDlSprTxTbDstn(cell, ue, tbInfo, effAlloc,
                            pfsUe->svcPriorLst.first);
      *node = NULLP;
      RETVOID;
   }
   /* 3.1 MIMO refer the previous service to 
    * make sure if not served completely then 
    * the other TB allocation accomodates the same */
   *node = prev;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPfsDlSprTxTbDstn 
 *        
 *     Desc : Perform Actual allocation distribution among
 *     UEs schd svcs and TA for a given spare TB "tbInfo" allocation.
 *     spare TB allocation is as a result of 1 RETX TB allocation, when
 *     conditions are favourable for 2 TB spatial multiplexing. 
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlSprTxTbDstn
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgSchDlHqTbCb  *tbInfo,
U32            *effAlloc,
CmLList        *node
)
#else
PRIVATE Void rgSCHPfsDlSprTxTbDstn(cell, ue, tbInfo, effAlloc, node)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgSchDlHqTbCb  *tbInfo;
U32            *effAlloc;
CmLList        *node;
#endif
{

  RgSchDlLcCb        *lc;
  RgSchPfsDlLc       *pfsLc;
  U32                bytes;
  U32                effBo;
  U32                rlcHdrEstmt;

   TRC2(rgSCHPfsDlSprTxTbDstn);

   while (node)
   {
      lc = (RgSchDlLcCb *)node->node;
      pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
      node = node->next;
      RG_SCH_CMN_DL_GET_HDR_EST(lc, rlcHdrEstmt);
      effBo = pfsLc->effBo + rlcHdrEstmt;
      bytes = RGSCH_MIN(*effAlloc, effBo);
      if (bytes < *effAlloc)
      {
	 /* MS_REMOVE */     
#ifdef MSPD
      {
            if (bytes < rlcHdrEstmt)
	    {
               stop_printf("ERROR!! bytes(%d) < rlcHdrEstmt(%d)\n", bytes, rlcHdrEstmt);		    
	    }		    
      }		 
#endif
         rgSCHPfsDlBoUpdInt(cell, ue, lc, 0, bytes-rlcHdrEstmt);
         rgSCHPfsDlAddLcAllocToProc(cell, ue, lc, bytes, tbInfo);
         *effAlloc -= bytes;
      }
      else if (bytes <= rlcHdrEstmt)
      {
#ifdef MSPD
         MSPD_DBG("Cell Time [%d,%d] BO %d TbIdx %d TbSiz %d numLch %d\n",cell->crntTime.sfn,cell->crntTime.subframe, bytes, tbInfo->tbIdx, tbInfo->tbSz, tbInfo->numLch);
#endif
         break;
      }
      else
      {
	 /* MS_REMOVE */     
#ifdef MSPD
      {
            if ((bytes < rlcHdrEstmt) || (pfsLc->bo < (bytes-rlcHdrEstmt)))
	    {
               stop_printf("ERROR!! bytes(%d) < rlcHdrEstmt(%d) bo(%d)\n", bytes, rlcHdrEstmt, pfsLc->bo);		    
	    }		    
      }		 
#endif
         rgSCHPfsDlBoUpdInt(cell, ue, lc, pfsLc->bo - (bytes-rlcHdrEstmt), 
                            bytes-rlcHdrEstmt);
         rgSCHPfsDlAddLcAllocToProc(cell, ue, lc, bytes, tbInfo);
         *effAlloc = 0;
         break;
      }
   }
   RETVOID;
}

/* Added new function to handle TX+RETX alloc fnlz'n */
/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        RETX+New TX allocations. The NewTx TB allocation
 *        is considered for distribution among LCs.
 *
 * @details
 *
 *     Function: rgSCHPfsDlRetxNewTxAllocFnlz 
 *     Purpose : 1. Reached here due to 1 RETX TB allocation for a 
 *                  SM UE, which is capable to accomodate a newTX
 *                  in the other TB.
 *               2. Distribute NewTX TB allocation among the
 *                  SVCs present in lcsWithData list of UE.
 *
 *
 *     Invoked by: Common Scheduler 
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *alloc
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlRetxNewTxAllocFnlz 
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *alloc
)
#else
PRIVATE Void rgSCHPfsDlRetxNewTxAllocFnlz(cell, alloc)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *alloc;
#endif
{
   CmLList              *lnk;
   RgSchPfsDlUe         *pfsUe;
   RgSchDlHqTbCb        *newTxTbInfo;   
   RgSchDlRbAlloc       *ueAllocInfo;  
   U32                   effAlloc;
   U32                   remTbBytes;
   

   /* Take action for UEs that are finally scheduled for retx */
   for (lnk = alloc->dedAlloc.schdTxRetxUeLst.first; lnk; )
   {
      RgSchDlHqProcCb    *proc    = (RgSchDlHqProcCb *)lnk->node;
      RgSchUeCb          *ue      = proc->hqE->ue;
     
      lnk = lnk->next;     
      pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
      ueAllocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
      /* Index 0 of ueAllocInfo->tbInfo will always hold the 
          * RETX TB and index 1 will hold the NewTX TB in case of
          * RETX+TX allocation. */
      newTxTbInfo = ueAllocInfo->tbInfo[1].tbCb;
      effAlloc = remTbBytes  =  ueAllocInfo->tbInfo[1].bytesAlloc;     
      rgSCHPfsDlSprTxTbDstn(cell, ue, newTxTbInfo,\
                            &remTbBytes, pfsUe->svcPriorLst.first);
      /* Trying to reduce mcs of TX TB to increase reception quality at UE.
       * In case of RETX+TX allocation, TX TB allocation was irrespective
       * of actual requirement by UE, hence in case if consumption is 
       * less than allocation, consider reducing the iMcs of this TX TB. */
      rgSCHCmnRdcImcsTxTb(&ue->dl.dlAllocCb, 1, effAlloc - remTbBytes);
      rgSCHPfsDlProcRmvFrmRetx(cell, ue, proc);
      rgSCHCmnFillHqPPdcch(cell, &ue->dl.dlAllocCb, proc);
#ifdef LTEMAC_HDFDD
      if (RG_SCH_HDFDD_UE_ENBLD(ue))
      {
         rgSCHCmnHdFddUpdDLMark(cell, ue);
      }
#endif
      rgSCHPfsDlUeSfResetTemp(cell, ue, proc);
   }

   /* Take action for UEs that are finally NOT scheduled for retx */
   for (lnk = alloc->dedAlloc.nonSchdTxRetxUeLst.first; lnk; )
   {
      RgSchDlHqProcCb    *proc    = (RgSchDlHqProcCb *)lnk->node;
      RgSchUeCb          *ue      = proc->hqE->ue;
      lnk = lnk->next;
      MSPD_DBG("UE(%d) PROC(%d) FAILED\n", ue->ueId, proc->procId);
      /* MS_FIX: syed To update Retx Alloc failure counter.
       * Need to do the same in other schedulers as well. */
      rgSCHDhmDlRetxAllocFail(ue, proc);
      rgSCHPfsDlUeSfResetTemp(cell, ue, proc);
   }
   RETVOID;
}

/**
 * @brief Invoked on final RB allocation for a DL subframe
 *
 * @details
 *
 *     Function : rgSCHPfsDlOnAlloc
 *
 *     Processing Steps:
 *     - Update for retx allocations (and lack of them)
 *     - Update for new allocations (and lack of them)
 *    3.1 MIMO
 *     - Update for retx + new allocations (and lack of them)
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *alloc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsDlOnAlloc
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *alloc
)
#else
PUBLIC Void rgSCHPfsDlOnAlloc(cell, alloc)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *alloc;
#endif
{
   TRC2(rgSCHPfsDlOnAlloc);

   rgSCHPfsDlOnRetxAlloc(cell, alloc);

   rgSCHPfsDlOnNewTxAlloc(cell, alloc);

   /* New Function added to handle TX+RETX 
    * harq process scheduling finalization */
   rgSCHPfsDlRetxNewTxAllocFnlz(cell, alloc);


   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPfsDlOnRetxAlloc
 *
 *     Desc : Do relevant updations corresponding to retx
 *             allocations (and lack of them)
 *             LTEMAC_MIMO: For SM UEs consider harq state of both TBs.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlOnRetxAlloc
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *alloc
)
#else
PRIVATE Void rgSCHPfsDlOnRetxAlloc(cell, alloc)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *alloc;
#endif
{
   CmLList              *lnk;
   TRC2(rgSCHPfsDlOnRetxAlloc);

   /* Take action for UEs that are finally scheduled for retx */
   for (lnk = alloc->dedAlloc.schdRetxUeLst.first; lnk; )
   {
      RgSchDlHqProcCb    *proc    = (RgSchDlHqProcCb *)lnk->node;
      RgSchUeCb          *ue      = proc->hqE->ue;
      lnk = lnk->next;
      rgSCHCmnFillHqPPdcch(cell, &ue->dl.dlAllocCb, proc);
#ifdef LTEMAC_HDFDD
      if (RG_SCH_HDFDD_UE_ENBLD(ue))
      {
         rgSCHCmnHdFddUpdDLMark(cell, ue);
      }
#endif
      /* Extra Check: Retain the hqProc in the RETX Queue if one/more
       * TBs of the HqProc are yet to be scheduled for RETX */
      if ((proc->tbInfo[0].state != HQ_TB_NACKED) && 
            (proc->tbInfo[1].state != HQ_TB_NACKED))     
      {
         rgSCHPfsDlProcRmvFrmRetx(cell, ue, proc);
      }     
      rgSCHPfsDlUeSfResetTemp(cell, ue, proc);
   }

   /* Take action for UEs that are finally NOT scheduled for retx */
   for (lnk = alloc->dedAlloc.nonSchdRetxUeLst.first; lnk; )
   {
      RgSchDlHqProcCb    *proc    = (RgSchDlHqProcCb *)lnk->node;
      RgSchUeCb          *ue      = proc->hqE->ue;
      lnk = lnk->next;
      /* MS_FIX: syed To update Retx Alloc failure counter.
       * Need to do the same in other schedulers as well. */
      MSPD_DBG("UE(%d) PROC(%d) FAILED\n", ue->ueId, proc->procId);
      rgSCHDhmDlRetxAllocFail(ue, proc);
      rgSCHPfsDlUeSfResetTemp(cell, ue, proc);
   }

   RETVOID;
}  /* rgSCHPfsDlOnRetxAlloc */

/***********************************************************
 *
 *     Func : rgSCHPfsDlOnNewTxAlloc
 *
 *     Desc : Do relevant updations corresponding to new
 *             tx allocations and updations for those
 *             which did not receive any allocations
 *             (freeing proc etc)
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlOnNewTxAlloc
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *alloc
)
#else
PRIVATE Void rgSCHPfsDlOnNewTxAlloc(cell, alloc)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *alloc;
#endif
{
   CmLList            *lnk;
   TRC2(rgSCHPfsDlOnNewTxAlloc);

   /* Take action for UEs that are finally scheduled for retx */
   for (lnk = alloc->dedAlloc.schdTxUeLst.first; lnk; )
   {
      RgSchDlHqProcCb    *proc    = (RgSchDlHqProcCb *)lnk->node;
      RgSchUeCb          *ue      = proc->hqE->ue;
      lnk = lnk->next;
      rgSCHPfsDlOnNewTxAllocForUe(cell, ue, alloc);
#ifdef LTEMAC_HDFDD
      if (RG_SCH_HDFDD_UE_ENBLD(ue))
      {
         rgSCHCmnHdFddUpdDLMark(cell, ue);
      }
#endif
      rgSCHPfsDlUeSfResetTemp(cell, ue, proc);
   }

   /* Take action for UEs that are finally NOT scheduled for retx */
   for (lnk = alloc->dedAlloc.nonSchdTxUeLst.first; lnk; )
   {
      RgSchDlHqProcCb    *proc    = (RgSchDlHqProcCb *)lnk->node;
      RgSchUeCb          *ue      = proc->hqE->ue;
      lnk = lnk->next;
      MSPD_DBG("UE(%d) PROC(%d) FAILED\n", ue->ueId, proc->procId);
      rgSCHPfsDlRlsHqProc(ue, proc);
      rgSCHPfsDlUeSfResetTemp(cell, ue, proc);
   }

   RETVOID;
}  /* rgSCHPfsDlOnNewTxAlloc */

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
/***********************************************************
 *
 *     Func : rgSCHPfsDlOnNewTxAllocForUe
 *
 *     Desc : Do relevant updations corresponding to new
 *             tx allocation for a UE from the Scheduled TB
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlOnNewTxAllocForUe
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnDlRbAllocInfo   *alloc
)
#else
PRIVATE Void rgSCHPfsDlOnNewTxAllocForUe(cell, ue, alloc)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
RgSchCmnDlRbAllocInfo   *alloc;
#endif
{
   RgSchCmnDlUe         *cmnUe = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchPfsDlUe         *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependncy on MIMO compile-time flag */
   CmLList              *node;
   /* 3.1 MIMO Distribute data of each TB across services */
   U32                remTb1Bytes = ue->dl.dlAllocCb.tbInfo[0].bytesAlloc;
   U32                remTb2Bytes = ue->dl.dlAllocCb.tbInfo[1].bytesAlloc;
   U32                effAlloc = 0;
   /*ccpu00120365-ADD-added to code to check if second TB is utilized */
   U32                tb2Bytes = 0;

   TRC2(rgSCHPfsDlOnNewTxAllocForUe);
 

   if (!(remTb1Bytes + remTb2Bytes))
   {
      RETVOID;
   }
   /* 3.1 MIMO Consider the allocation of New TX TB for distribution */
   /* Handle schd services */
   node = pfsUe->schedSvcs.first;
   if (remTb1Bytes){
      effAlloc += remTb1Bytes;
      rgSCHPfsDlNewTxTbDstn(cell, ue, &cmnUe->proc->tbInfo[0],\
            &remTb1Bytes, &node);
      /* In the event that TB1 is not completely filled by the DL LCs
       * BO, consider the reducing the iMcs for increasing redundancy
       * and hence reception quality at UE */
      rgSCHCmnRdcImcsTxTb(&ue->dl.dlAllocCb, 0, 
            ue->dl.dlAllocCb.tbInfo[0].bytesAlloc - remTb1Bytes);
   }
   
   /*ccpu00120365-ADD-assigning value of remTb2Bytes before utilization */
   tb2Bytes = remTb2Bytes;
   
   /* Extra check for a non SM UE allocation */
   if (remTb2Bytes){
      effAlloc += remTb2Bytes;
      rgSCHPfsDlNewTxTbDstn(cell, ue, &cmnUe->proc->tbInfo[1],\
            &remTb2Bytes, &node);
      /* In the event that TB2 is not completely filled by the DL LCs
       * BO, consider the reducing the iMcs for increasing redundancy
       * and hence reception quality at UE */
      rgSCHCmnRdcImcsTxTb(&ue->dl.dlAllocCb, 1, 
            ue->dl.dlAllocCb.tbInfo[1].bytesAlloc - remTb2Bytes);
   }
   
   /* ccpu00120365-ADD-Disable the second TB as the second TB is not
    * utilized */
   if ( remTb2Bytes && ( tb2Bytes == remTb2Bytes) )
   {
      ue->dl.dlAllocCb.mimoAllocInfo.precIdxInfo = 0;
      ue->dl.dlAllocCb.mimoAllocInfo.numTxLyrs   = 1;
      ue->dl.dlAllocCb.tbInfo[1].schdlngForTb    = FALSE;
      ue->dl.dlAllocCb.tbInfo[1].isDisabled      = TRUE;
   }

   /* In the (rare) event that allocation was too small
    * and we think no LC data can fit (due to headers taking
    * up some space), we need to rollback allocation by
    * releasing PDCCH and proc. */
   if (effAlloc == (remTb1Bytes + remTb2Bytes))
   {
      rgSCHUtlPdcchPut(cell, &ue->dl.dlAllocCb.dlSf->pdcchInfo, 
            ue->dl.dlAllocCb.pdcch);
      rgSCHPfsDlRlsHqProc(ue, cmnUe->proc);
      RETVOID;
   }

   /* Rest of the LCs have nothing allocated. Need not do anything
    * about them. */

   /* Fill PDCCH and assign it to proc */
   rgSCHCmnFillHqPPdcch(cell, &ue->dl.dlAllocCb, cmnUe->proc);

   /* Reinit of schedSvcs happens later as part of temporary variables
    * reset in function rgSCHPfsDlUeSfResetTemp() */

   RETVOID;
}  /* rgSCHPfsDlOnNewTxAllocForUe */
/***********************************************************
 *
 *     Func : rgSCHPfsDlLcPartlyAllocd
 *
 *     Desc : This function is called when an LC has
 *            been partly allocated for. The 'bytes'
 *            passed is the total allocated bytes,
 *            including header.
 *            1. Update LC BO and do appropriate
 *               repositioning by calling the BO
 *               update function.
 *            2. Add the allocation details to HARQ proc
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlLcPartlyAllocd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *lc,
U32                 bytes,
RgSchDlHqTbCb      *tbInfo
)
#else
PRIVATE Void rgSCHPfsDlLcPartlyAllocd(cell, ue, lc, bytes, tbInfo)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlLcCb        *lc;
U32                 bytes;
RgSchDlHqTbCb      *tbInfo;
#endif
{
   RgSchPfsDlLc       *pfsLc  = RG_SCH_PFS_GET_PFSDLLC(lc);
   U32                 sduBytes = bytes - pfsLc->hdrSz;
	 /* MS_REMOVE */     
#ifdef MSPD
        {
            if (pfsLc->bo < sduBytes)
	    {
               stop_printf("ERROR!! bo(%d) < sduBytes(%d)\n", pfsLc->bo, sduBytes);		    
	    }		    
	 }		 
#endif
   rgSCHPfsDlBoUpdInt(cell, ue, lc, pfsLc->bo - sduBytes, sduBytes);
   rgSCHPfsDlAddLcAllocToProc(cell, ue, lc, bytes, tbInfo);
   /* Else no allocation is made to the LC. */
   RETVOID;
}  /* rgSCHPfsDlLcPartlyAllocd */

/***********************************************************
 *
 *     Func : rgSCHPfsDlLcFullyAllocd
 *
 *     Desc : 1. Update LC BO with the appropriate
 *               repositioning by calling the BO
 *               update function.
 *            2. Add the allocation details to HARQ proc
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlLcFullyAllocd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *lc,
RgSchDlHqTbCb      *tbInfo
)
#else
PRIVATE Void rgSCHPfsDlLcFullyAllocd(cell, ue, lc, tbInfo)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlLcCb        *lc;
RgSchDlHqTbCb      *tbInfo;
#endif
{
   RgSchPfsDlLc       *pfsLc  = RG_SCH_PFS_GET_PFSDLLC(lc);
   U32                 sduBytes = pfsLc->reqBytes - pfsLc->hdrSz;

   rgSCHPfsDlBoUpdInt(cell, ue, lc, 0, sduBytes);
   rgSCHPfsDlAddLcAllocToProc(cell, ue, lc, pfsLc->reqBytes, tbInfo);
   RETVOID;
}  /* rgSCHPfsDlLcFullyAllocd */
/**
 * @brief Add LC allocation HARQ Proc
 *
 * @details
 *
 *     Function : rgSCHPfsDlAddLcAllocToProc
 *
 *     Processing Steps:
 *     - Call common module API to add allocation for a
 *       logical channel to currently scheduled HARQ proc
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchUeCb              *ue
 *  @param[in]  RgSchDlLcCb            *lc
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlAddLcAllocToProc
(
RgSchCellCb            *cell,
RgSchUeCb              *ue,
RgSchDlLcCb            *lc,
U32                     bytes,
RgSchDlHqTbCb          *tbInfo
)
#else
PRIVATE Void rgSCHPfsDlAddLcAllocToProc(cell, ue, lc, bytes, tbInfo)
RgSchCellCb            *cell;
RgSchUeCb              *ue;
RgSchDlLcCb            *lc;
U32                     bytes;
RgSchDlHqTbCb          *tbInfo;
#endif
{
   RgSchLchAllocInfo   lchSchdData;

   lchSchdData.lcId = lc->lcId;
   lchSchdData.schdData = bytes;
   /*TODO: add lc data to a specific TB */
   rgSCHDhmAddLcData(cell->instIdx, &lchSchdData, tbInfo);
   RETVOID;
}
/***********************************************************
 *
 *     Func : rgSCHPfsDlUeSfResetTemp
 *
 *     Desc : Reset whatever variables where temporarily used
 *            during UE scheduling.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlUeSfResetTemp
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchDlHqProcCb         *hqP
)
#else
PRIVATE Void rgSCHPfsDlUeSfResetTemp(cell, ue, hqP)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
RgSchDlHqProcCb         *hqP;
#endif
{
   RgSchPfsDlUe       *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);

   UNUSED(cell);

   TRC2(rgSCHPfsDlUeSfResetTemp);

   /* Not explicitly removing LCs from list of
    * scheduled services; just doing a list init.
    * Redundant for retx case.
    */
   cmLListInit(&pfsUe->schedSvcs);

   /* Re-init of ue->dl.dlAllocCb and setting cmnUe->proc to NULL etc happens
    * in the common function below */
   rgSCHCmnDlUeResetTemp(ue, hqP);

   RETVOID;
}  /* rgSCHPfsDlUeSfResetTemp */

/**
 * @brief BO update
 *
 * @details
 *
 *     Function : rgSCHPfsDlBoUpd
 *
 *     Processing Steps:
 *     - Call the internal BO update function rgSCHPfsDlBoUpdInt()
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchUeCb              *ue
 *  @param[in]  RgSchDlLcCb            *lc
 *  @param[in]  RguDBoReport           *boRpt
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsDlBoUpd
(
RgSchCellCb            *cell,
RgSchUeCb              *ue,
RgSchDlLcCb            *lc
)
#else
PUBLIC Void rgSCHPfsDlBoUpd(cell, ue, lc)
RgSchCellCb            *cell;
RgSchUeCb              *ue;
RgSchDlLcCb            *lc;
#endif
{
   RgSchPfsDlLc       *pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
   TRC2(rgSCHPfsDlBoUpd);

   if ((pfsLc->bo == lc->bo) || !RG_SCH_CMN_DL_IS_UE_ACTIVE(ue))
   {
      RETVOID;
   }
   rgSCHPfsDlBoUpdInt(cell, ue, lc, lc->bo, 0);
   RETVOID;
}

/**
 * @brief Internal BO update
 *
 * @details
 *
 *     Function : rgSCHPfsDlBoUpdInt
 *
 *     Processing Steps:
 *     - Based on type of LC, do appropriate BO+alloc update
 *       - If type is AMBR
 *         - rgSCHPfsDlLcAmbrBoUpd()
 *       - Else If type is GBR
 *         - rgSCHPfsDlLcGbrBoUpd()
 *       - Else if bo is zeo
 *         - Remove from whatever queue LC might be in
 *       - Else if SRB
 *         - Add to SRB queue
 *       - Else (if DRB0)
 *         - Add to DRB0 queue
 *
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchUeCb              *ue
 *  @param[in]  RgSchDlLcCb            *lc
 *  @param[in]  U32                     bo
 *  @param[in]  U32                     allocd
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlBoUpdInt
(
RgSchCellCb            *cell,
RgSchUeCb              *ue,
RgSchDlLcCb            *lc,

U32                     bo,
U32                     allocd
)
#else
PRIVATE Void rgSCHPfsDlBoUpdInt(cell, ue, lc, bo, allocd)
RgSchCellCb            *cell;
RgSchUeCb              *ue;
RgSchDlLcCb            *lc;
U32                     bo;
U32                     allocd;
#endif
{
   RgSchPfsDlCell     *pfsCell = RG_SCH_PFS_GET_PFSCELLDL(cell);
   RgSchPfsDlUe       *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
   RgSchPfsDlLc       *pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
   TRC2(rgSCHPfsDlBoUpdInt);


   lc->bo = bo;
   pfsLc->bo = bo;

   if (pfsLc->lcType == RG_SCH_PFS_LC_TYPE_AMBR)
   {
      rgSCHPfsDlLcAmbrBoUpd(pfsCell, ue, lc, bo, allocd);
      RETVOID;
   }

   if (pfsLc->lcType == RG_SCH_PFS_LC_TYPE_GBR)
   {
      rgSCHPfsDlLcGbrBoUpd(pfsCell, ue, lc, bo, allocd);
      RETVOID;
   }

   if (bo == 0)
   {
      pfsLc->effBo   = 0;
      rgSCHPfsDlLcRmvFrmPrioQ(pfsCell, pfsUe, pfsLc);
      RETVOID;
   }

   if (pfsLc->lcType == RG_SCH_PFS_LC_TYPE_SRB)
   {
      pfsLc->effBo = bo;
      rgSCHPfsDlLcAddToSrbQ(pfsCell, pfsUe, pfsLc);
   }
   else
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      if (pfsLc->lcType != RG_SCH_PFS_LC_TYPE_DRB0)
      {
         RGSCHDBGINFO(cell->instIdx,
               (rgSchPBuf(cell->instIdx),
                "rgSCHPfsDlBoUpdInt(): Incorrect lcType\n"));
      }
#endif
      pfsLc->effBo = bo;
      rgSCHPfsDlLcAddToDrb0Q(pfsCell, pfsUe, pfsLc);
   }
   RETVOID;
}
/***********************************************************
 *
 *     Func : rgSCHPfsDlPrioAmbrSvcUpd
 *
 *     Desc : This is called when there is now a new
 *            highest priority AMBR svc with BO
 *            (i.e. pfsUe->ambrSvc has just been
 *            updated)
 *
 *     Ret  : Void
 *
 *     Notes: It is assumed that if there was
 *            an earlier 'ambrSvc', caller would
 *            have taken appropriate actions for
 *            its removal.
 *            It is also assumed that since this
 *            service is not in cell wide queues as
 *            it is just been realised to be the
 *            highest prio ambr svc. Its remRate,
 *            effBo etc need to be computed.
 *
 *     File :
 *
 **********************************************************/
/*rg002.301_LPR1-MOD-Modified the prototype of the below function
 * to add a new parmater allocd*/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlPrioAmbrSvcUpd
(
RgSchPfsDlCell     *pfsCell,
RgSchUeCb          *ue,
U32                allocd
)
#else
PRIVATE Void rgSCHPfsDlPrioAmbrSvcUpd(pfsCell, ue, allocd)
RgSchPfsDlCell     *pfsCell;
RgSchUeCb          *ue;
U32                allocd;
#endif
{
   RgSchCmnDlUe       *cmnUe;
   RgSchPfsDlUe       *pfsUe;
   RgSchDlLcCb        *lc;
   RgSchPfsDlLc       *pfsLc;
   TRC2(rgSCHPfsDlPrioAmbrSvcUpd);

   cmnUe = RG_SCH_CMN_GET_DL_UE(ue);
   pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
   lc = pfsUe->ambrSvc;
   pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);

   if (cmnUe->ambrCfgd)
   {
      /* remAmbr > 0 here. Allocation triggers reduction
       * of remAmbr where the appropriate check and action
       * on it becoming zero takes place.
       */
      /* MS_FIX : Correcting Fairness logic */	   
      pfsLc->effBo = RGSCH_MIN(pfsLc->bo, pfsUe->remAmbr);
   }
   else
   {
      pfsLc->effBo = pfsLc->bo;
   }
   /*rg002.301_LPR1-MOD-Modified the invocation of this function to
     * add a new parameter allocd.*/
   rgSCHPfsDlLcAddToAmbrQ(pfsCell, pfsUe, pfsLc, allocd);
   RETVOID;
}  /* rgSCHPfsDlPrioAmbrSvcUpd */

/***********************************************************
 *
 *     Func : rgSCHPfsDlLcAddToSrbQ
 *
 *     Desc : Add a DL LC to SRB queue, if not already
 *            present.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlLcAddToSrbQ
(
RgSchPfsDlCell     *pfsCell,
RgSchPfsDlUe       *pfsUe,
RgSchPfsDlLc       *pfsLc
)
#else
PRIVATE Void rgSCHPfsDlLcAddToSrbQ(pfsCell, pfsUe, pfsLc)
RgSchPfsDlCell     *pfsCell;
RgSchPfsDlUe       *pfsUe;
RgSchPfsDlLc       *pfsLc;
#endif
{
   TRC2(rgSCHPfsDlLcAddToSrbQ);
   if (pfsLc->prioLstCp == NULLP)
   {
      rgSCHPfsDlLcAddToQ(pfsUe, &pfsCell->txQueues.srbLst, pfsLc);
   }
   RETVOID;
}  /* rgSCHPfsDlLcAddToSrbQ */


/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
/***********************************************************
 *
 *     Func : rgSCHPfsDLLcAddToUe
 *
 *     Desc :  Purpose:  This function adds a service to the list of
 *                   services
 *               of UE based on the priority of the services.
 *               Order of Priority of services are SRB,DRB,GBR and AMBR
 *            
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDLLcAddToUe
(
CmLListCp          *cp,
RgSchPfsDlLc       *pfsLc
)
#else
PRIVATE Void rgSCHPfsDLLcAddToUe(cp, pfsLc)
CmLListCp          *cp;
RgSchPfsDlLc       *pfsLc;
#endif
{
   CmLList *lnk;
   RgSchDlLcCb        *lc;
   RgSchPfsDlLc       *pfsSvc;

   
   if(pfsLc->lcType == RG_SCH_PFS_LC_TYPE_SRB)
   {
      lnk = cp->first;
      cp->crnt = lnk;
      cmLListInsCrnt(cp, &pfsLc->svcPrioLnk);
   }
   else if(pfsLc->lcType == RG_SCH_PFS_LC_TYPE_DRB0)
   {
      lnk = cp->first;
      while(lnk)
      {
         lc = (RgSchDlLcCb *)lnk->node;
         pfsSvc = RG_SCH_PFS_GET_PFSDLLC(lc);
         if(pfsSvc->lcType == RG_SCH_PFS_LC_TYPE_SRB)
         {
            lnk = lnk->next;
            continue;
         }
         break;
      }
      if (lnk== NULLP)
      {
         cmLListAdd2Tail(cp, &pfsLc->svcPrioLnk);
      }
      else
      {
         cp->crnt = lnk;
         cmLListInsCrnt(cp, &pfsLc->svcPrioLnk);
      }
   }  
   else if(pfsLc->lcType == RG_SCH_PFS_LC_TYPE_GBR)
   {
      lnk = cp->first;
      while(lnk)
      {
         lc = (RgSchDlLcCb *)lnk->node;
         pfsSvc = RG_SCH_PFS_GET_PFSDLLC(lc);
         if((pfsSvc->lcType == RG_SCH_PFS_LC_TYPE_SRB) || (pfsSvc->lcType == RG_SCH_PFS_LC_TYPE_DRB0))
         {
             lnk = lnk->next;
             continue;
         }
         break;
      }
      if (lnk== NULLP)
      {
         cmLListAdd2Tail(cp, &pfsLc->svcPrioLnk);
      }
      else
      {
         cp->crnt = lnk;
         cmLListInsCrnt(cp, &pfsLc->svcPrioLnk);
      }
   
   }   
   else
   {
      cmLListAdd2Tail(cp, &pfsLc->svcPrioLnk);
   }
       
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHPfsDlLcAddToDrb0Q
 *
 *     Desc : Add a DL LC to DRB0 queue, if not already
 *            present.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlLcAddToDrb0Q
(
RgSchPfsDlCell     *pfsCell,
RgSchPfsDlUe       *pfsUe,
RgSchPfsDlLc       *pfsLc
)
#else
PRIVATE Void rgSCHPfsDlLcAddToDrb0Q(pfsCell, pfsUe, pfsLc)
RgSchPfsDlCell     *pfsCell;
RgSchPfsDlUe       *pfsUe;
RgSchPfsDlLc       *pfsLc;
#endif
{
   TRC2(rgSCHPfsDlLcAddToDrb0Q);

   if (pfsLc->prioLstCp == NULLP)
   {
      rgSCHPfsDlLcAddToQ(pfsUe, &pfsCell->txQueues.drb0Lst, pfsLc);
   }
   RETVOID;
}  /* rgSCHPfsDlLcAddToDrb0Q */

/***********************************************************
 *
 *     Func : rgSCHPfsDlLcAddToGbrQ
 *
 *     Desc : Add a DL LC to GBR queue, assuming that
 *            'remRate' and 'svccqiPrio' have been
 *            determined. This further updates
 *            pfsLc->pfsPrio before updating in
 *            queue. Does not affect queue positions
 *            if LC was already in correct queue.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlLcAddToGbrQ
(
RgSchPfsDlCell     *pfsCell,
RgSchPfsDlUe       *pfsUe,
RgSchPfsDlLc       *pfsLc,
Bool               prioChngd, 
U32                allocd
)
#else
PRIVATE Void rgSCHPfsDlLcAddToGbrQ(pfsCell, pfsUe, pfsLc, prioChngd, allocd)
RgSchPfsDlCell     *pfsCell;
RgSchPfsDlUe       *pfsUe;
RgSchPfsDlLc       *pfsLc;
Bool               prioChngd;
U32                allocd;
#endif
{
   U8                  qId;
   TRC2(rgSCHPfsDlLcAddToGbrQ);

   if (pfsLc->prioLstCp && (!prioChngd) && (allocd == 0))
   {
      RETVOID;
   }
   qId = rgSCHPfsDlGetQId(&pfsCell->ranges.gbrRange,
         pfsLc->pfsPrio, RG_SCH_PFS_DL_NUM_GBR_QUEUES-1);
   rgSCHPfsDlLcRmvFrmPrioQ(pfsCell, pfsUe, pfsLc);
   rgSCHPfsDlLcAddToQ(pfsUe, &pfsCell->txQueues.gbrLst[qId], pfsLc);
   pfsLc->qId = qId;

   RETVOID;
}  /* rgSCHPfsDlLcAddToGbrQ */

/***********************************************************
 *
 *     Func : rgSCHPfsDlLcAddToMbrQ
 *
 *     Desc : Add a DL LC to MBR queue, assuming that
 *            'remRate' and 'svccqiPrio' have been
 *            determined. This further updates
 *            pfsLc->pfsPrio before updating in
 *            queue. Does not affect queue positions
 *            if LC was already in correct queue.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlLcAddToMbrQ
(
RgSchPfsDlCell     *pfsCell,
RgSchPfsDlUe       *pfsUe,
RgSchPfsDlLc       *pfsLc,
Bool               prioChngd, 
U32                allocd
)
#else
PRIVATE Void rgSCHPfsDlLcAddToMbrQ(pfsCell, pfsUe, pfsLc, prioChngd, allocd)
RgSchPfsDlCell     *pfsCell;
RgSchPfsDlUe       *pfsUe;
RgSchPfsDlLc       *pfsLc;
Bool               prioChngd;
U32                allocd;
#endif
{
   U8                  qId;
   TRC2(rgSCHPfsDlLcAddToMbrQ);

   if (pfsLc->prioLstCp && (!prioChngd) && (allocd == 0))
   {
      RETVOID;
   }
   qId = rgSCHPfsDlGetQId(&pfsCell->ranges.mbrRange,
         pfsLc->pfsPrio, RG_SCH_PFS_DL_NUM_MBR_QUEUES-1);
   rgSCHPfsDlLcRmvFrmPrioQ(pfsCell, pfsUe, pfsLc);
   rgSCHPfsDlLcAddToQ(pfsUe, &pfsCell->txQueues.mbrLst[qId], pfsLc);
   pfsLc->qId = qId;

   RETVOID;
}  /* rgSCHPfsDlLcAddToMbrQ */

/***********************************************************
 *
 *     Func : rgSCHPfsDlLcAddToAmbrQ
 *
 *     Desc : Add a DL LC to AMBR queue, assuming that
 *            'remRate' and 'svccqiPrio' have been
 *            determined. This further updates
 *            pfsLc->pfsPrio before updating in
 *            queue. Does not affect queue positions
 *            if LC was already in correct queue.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
/*rg002.301-MOD-Modified the below function prototype to add a 
 * new parameter allocd.*/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlLcAddToAmbrQ
(
RgSchPfsDlCell     *pfsCell,
RgSchPfsDlUe       *pfsUe,
RgSchPfsDlLc       *pfsLc,
U32                allocd
)
#else
PRIVATE Void rgSCHPfsDlLcAddToAmbrQ(pfsCell, pfsUe, pfsLc, allocd)
RgSchPfsDlCell     *pfsCell;
RgSchPfsDlUe       *pfsUe;
RgSchPfsDlLc       *pfsLc;
U32                allocd;
#endif
{
   U8                  qId;
   Bool                prioChngd;
   TRC2(rgSCHPfsDlLcAddToAmbrQ);

   /* [Later] Can have a common func for AddTo gbr/mbr/ambr q with
    * the following passed
    *   NUM_QUEUES
    *   &pfsCell->ranges.ambrRange
    *   &pfsCell->txQueues.ambrLst
    * reuse1
    */
   /* MS_FIX : Updating PFS to start prio with highest possible value
               and diminishing prio val updates from there on */
   /* MS_FIX : Each Priority Queue should be
    * sorted in descending order of pfsPrio. */
   rgSCHPfsUpdFracPrioInfo(&pfsUe->fracPrioInfo, allocd); 
   rgSCHPfsDlUpdPfsPrio(pfsCell, pfsLc, &pfsUe->fracPrioInfo, &prioChngd);
   if (pfsLc->prioLstCp && (!prioChngd) && (allocd == 0))
   {
      RETVOID;
   }
   qId = rgSCHPfsDlGetQId(&pfsCell->ranges.ambrRange,
         pfsLc->pfsPrio, RG_SCH_PFS_DL_NUM_AMBR_QUEUES-1);
   rgSCHPfsDlLcRmvFrmPrioQ(pfsCell, pfsUe, pfsLc);
   rgSCHPfsDlLcAddToQ(pfsUe, &pfsCell->txQueues.ambrLst[qId], pfsLc);
   pfsLc->qId = qId;

   RETVOID;
}  /* rgSCHPfsDlLcAddToAmbrQ */

/***********************************************************
 *
 *     Func : rgSCHPfsDlLcAddToQ
 *
 *     Desc : Insert LC in sorted Prio Queue. 
 *            Set lc's cp to that of queue.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlLcAddToQ
(
RgSchPfsDlUe       *pfsUe,
CmLListCp          *cp,
RgSchPfsDlLc       *pfsLc
)
#else
PRIVATE Void rgSCHPfsDlLcAddToQ(pfsUe, cp, pfsLc)
RgSchPfsDlUe       *pfsUe;
CmLListCp          *cp;
RgSchPfsDlLc       *pfsLc;
#endif
{
   RgSchDlLcCb        *lLc;
   RgSchPfsDlLc       *lPfsLc;
   CmLList            *node;

   TRC2(rgSCHPfsDlLcAddToQ);

   /* Add to priority Queues only if data and data rate 
    * available to be scheduled */      
   if (pfsLc->effBo == 0)
   {
      RETVOID;	   
   }
   /* MS_FIX : syed Each Priority Queue should be
    * sorted in descending order of pfsPrio. */
   /* Insertion can be improved by not always starting
    * from first node */
   node = cp->first;
   while(node)
   {
      lLc = (RgSchDlLcCb *)(node->node);
      lPfsLc = RG_SCH_PFS_GET_PFSDLLC(lLc);
      if (lPfsLc->pfsPrio < pfsLc->pfsPrio)
      {
         break;
      }
      node = node->next;
   }
   if (node == NULLP)
   {
      /* We have come to the end of the queue, so Append */
      cmLListAdd2Tail(cp, &pfsLc->prioLnk);
   }
   else
   {
      cp->crnt = node;
      cmLListInsCrnt(cp, &pfsLc->prioLnk);
   }

   pfsLc->prioLstCp = cp;

   /* Add to LC's to UE */
   rgSCHPfsDLLcAddToUe(&pfsUe->svcPriorLst,pfsLc);
   RETVOID;
}  /* rgSCHPfsDlLcAddToQ */

/***********************************************************
 *
 *     Func : rgSCHPfsDlLcAmbrBoUpd
 *
 *     Desc : Remove AMBR Svc from PFS UE's list of
 *            AMBR services if present in it
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlLcAmbrBoUpd
(
RgSchPfsDlCell     *pfsCell,
RgSchUeCb          *ue,
RgSchDlLcCb        *lc,
U32                 bo,
U32                 allocd
)
#else
PRIVATE Void rgSCHPfsDlLcAmbrBoUpd(pfsCell, ue, lc, bo, allocd)
RgSchPfsDlCell     *pfsCell;
RgSchUeCb          *ue;
RgSchDlLcCb        *lc;
U32                 bo;
U32                 allocd;
#endif
{
   RgSchCmnDlUe       *cmnUe   = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchPfsDlUe       *pfsUe   = RG_SCH_PFS_GET_PFSUEDL(ue);

   TRC2(rgSCHPfsDlLcAmbrBoUpd);

   /* If there is no more AMBR in this refresh cycle,
    * just update BO and get out */
   if (cmnUe->ambrCfgd && !pfsUe->remAmbr)
   {
      RETVOID;
   }

   /* If there is a BO update due to allocation, there
    * is an impact on remaining AMBR which should be
    * taken care of here */
   if (allocd && cmnUe->ambrCfgd)
   {
      RgSchPfsDlLc       *pfsLc;

      /*rg002.301_LPR1-ADD-Added the handling to set the effBo for a 
       * PFS LC to 0 if remAmbr has reached 0.*/
      pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
      if (allocd >= pfsUe->remAmbr)
      {
         pfsLc->effBo   = 0;
         pfsUe->remAmbr = 0;
         rgSCHPfsDlRemAmbrZero(pfsCell, pfsUe);
         RETVOID;
      }
      else
      {
         pfsUe->remAmbr -= allocd;
      }
   }


   if (bo == 0)
   {
      rgSCHPfsDlLcAmbrInactv(pfsCell, ue, lc);
   }
   else
   {
      /*rg002.301_LPR1-MOD-Modified the invocation of the below 
       * function to add a new parameter allocd.*/
      rgSCHPfsDlLcAmbrActv(pfsCell, ue, lc, bo, allocd);
   }
   RETVOID;
}  /* rgSCHPfsDlLcAmbrBoUpd */

/***********************************************************
 *
 *     Func : rgSCHPfsDlRemAmbrZero
 *
 *     Desc : Takes care of the condition when remAmbr
 *            becomes zero (due to allocation).
 *            Essetially, it removes all 'active'
 *            AMBR services from UE's list of active
 *            AMBR services.
 *
 *     Ret  : Void
 *
 *     Notes: By the description, this is obviously
 *            applicable only for UEs for which AMBR
 *            is configured.
 *            This is called only when remAmbr becomes
 *            zero. In theory this could have been
 *            called from other places (such as UE
 *            reset, in which case this is not only
 *            applicable for the case of UE's with
 *            configured AMBR). If that is done, this
 *            function may be renamed accordingly.
 *            Also, not setting effBo and remRate
 *            of any LC to zero. They are going to
 *            come back to queues only during refresh,
 *            when these variables will be freshly set.
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlRemAmbrZero
(
RgSchPfsDlCell     *pfsCell,
RgSchPfsDlUe       *pfsUe
)
#else
PRIVATE Void rgSCHPfsDlRemAmbrZero(pfsCell, pfsUe)
RgSchPfsDlCell     *pfsCell;
RgSchPfsDlUe       *pfsUe;
#endif
{
   CmLListCp          *cp;
   RgSchDlLcCb        *lc;
   RgSchPfsDlLc       *pfsLc;

   TRC2(rgSCHPfsDlRemAmbrZero);

   if ((lc = pfsUe->ambrSvc) == NULLP)
   {
      RETVOID;
   }

   /* Remove the highest priority AMBR service from
    * cell-wide queues */
   pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
   rgSCHPfsDlLcRmvFrmPrioQ(pfsCell, pfsUe, pfsLc);

   /* Now remove all AMBR services from UE's list of active AMBR
    * services */
   cp = &pfsUe->ambrLst;
   /* Fix from syed for mulitple bearer with same qci */
   do
   {
      lc = (RgSchDlLcCb *)(cp->first->node);	   
      pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
      /* Deemed unnecessry to set effBo to zero */
      cmLListDelFrm(cp, &pfsLc->ambrLnk);
      pfsLc->ambrLnk.node = NULLP;
   } while ((cp->first) != NULLP);
   /* Reset ambrSvc to NULLP when the AMBR Lst is empty. If we dont then upon 
    * BoUpd we check for the presence of ambrSvc andi assume that LC is also 
    * present in pfsUe->ambrLst. But when we come back to this function we always 
    * assume pfsUe->ambrLst is non-empty and we crash. */
   pfsUe->ambrSvc = NULLP;

   RETVOID;
}  /* rgSCHPfsDlRemAmbrZero */


/***********************************************************
 *
 *     Func : rgSCHPfsDlLcGbrBoUpd
 *
 *     Desc : Remove AMBR Svc from PFS UE's list of
 *            AMBR services if present in it
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlLcGbrBoUpd
(
RgSchPfsDlCell     *pfsCell,
RgSchUeCb          *ue,
RgSchDlLcCb        *lc,
U32                 bo,
U32                 allocd
)
#else
PRIVATE Void rgSCHPfsDlLcGbrBoUpd(pfsCell, ue, lc, bo, allocd)
RgSchPfsDlCell     *pfsCell;
RgSchUeCb          *ue;
RgSchDlLcCb        *lc;
U32                 bo;
U32                 allocd;
#endif
{
   RgSchPfsDlUe       *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
   RgSchPfsDlLc       *pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
   U32                 effBo = bo;
   Bool                prioChngd;
   TRC2(rgSCHPfsDlLcGbrBoUpd);

   /* Do allocation related updations */
   if (allocd)
   {
      if (pfsLc->remGbr)
      {
         if (allocd >= pfsLc->remGbr)
         {
            pfsLc->remGbr = 0;
            /* Could consider calling RmvFrmPrioQ to remove from the GBR queue
             * that this LC might be in */
	    /* From this point onwards, this LC would play in MBR Queues.
	     * Hence resetting fracPrio as per MBR */
            rgSCHPfsInitFracPrioInfo(&pfsLc->fracPrioInfo, pfsLc->deltaMbr); 
            rgSCHPfsDlUpdPfsPrio(pfsCell, pfsLc, &pfsLc->fracPrioInfo, &prioChngd);
         }
         else
         {
            pfsLc->remGbr -= allocd;
            /* MS_FIX : Updating PFS to start prio with highest possible value
                        and diminishing prio val updates from there on */
            rgSCHPfsUpdFracPrioInfo(&pfsLc->fracPrioInfo, allocd); 
            rgSCHPfsDlUpdPfsPrio(pfsCell, pfsLc, &pfsLc->fracPrioInfo, &prioChngd);
         }
      }
      else if (pfsLc->remDeltaMbr)
      {
         if (allocd >= pfsLc->remDeltaMbr)
         {
            pfsLc->remDeltaMbr = 0;
            /* Could consider calling RmvFrmPrioQ to remove from the MBR queue
             * that this LC might be in */
         }
         else
         {
            pfsLc->remDeltaMbr -= allocd;
            /* MS_FIX : Updating PFS to start prio with highest possible value
                        and diminishing prio val updates from there on */
            rgSCHPfsUpdFracPrioInfo(&pfsLc->fracPrioInfo, allocd); 
            rgSCHPfsDlUpdPfsPrio(pfsCell, pfsLc, &pfsLc->fracPrioInfo, &prioChngd);
         }
      }
   }

   if (bo == 0)
   {
      pfsLc->effBo   = 0;
      rgSCHPfsDlLcRmvFrmPrioQ(pfsCell, pfsUe, pfsLc);
      RETVOID;
   }

   if (pfsLc->remGbr)
   {
      effBo = RGSCH_MIN(bo, pfsLc->remGbr);
      rgSCHPfsDlLcAddToGbrQ(pfsCell, pfsUe, pfsLc, prioChngd, allocd);
   }
   else if (pfsLc->remDeltaMbr)
   {
      effBo = RGSCH_MIN(bo, pfsLc->remDeltaMbr);
      rgSCHPfsDlLcAddToMbrQ(pfsCell, pfsUe, pfsLc, prioChngd, allocd);
   }
   else
   {
      /* No MBR left, LC is not in, and will not be in any
       * queue until expiry of current refresh cycle */
      effBo = 0;
      rgSCHPfsDlLcRmvFrmPrioQ(pfsCell, pfsUe, pfsLc);
   }
   pfsLc->effBo = effBo;

   RETVOID;
}  /* rgSCHPfsDlLcGbrBoUpd */

/***********************************************************
 *
 *     Func : rgSCHPfsDlLcAmbrActv
 *
 *     Desc : Take all necessary action when positive BO
 *            is reported for an AMBR service.
 *
 *     Ret  : Void
 *
 *     Notes: Nothing is assumed about the service (such
 *            as whether it is in the UE's AMBR list or not).
 *            It is assumed though that the case of remAmbr
 *            being zero is handled earlier and such cases
 *            therefore need not be considered here.
 *
 *     File :
 *
 **********************************************************/
/*rg002.301-MOD-Modified the prototype of the below function to
 * add a new parameter allocd.*/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlLcAmbrActv
(
RgSchPfsDlCell     *pfsCell,
RgSchUeCb          *ue,
RgSchDlLcCb        *lc,
U32                 bo,
U32                 allocd
)
#else
PRIVATE Void rgSCHPfsDlLcAmbrActv(pfsCell, ue, lc, bo, allocd)
RgSchPfsDlCell     *pfsCell;
RgSchUeCb          *ue;
RgSchDlLcCb        *lc;
U32                 bo;
U32                 allocd;
#endif
{
   RgSchPfsDlUe       *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
   RgSchPfsDlLc       *pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
   RgSchDlLcCb        *oldLc = pfsUe->ambrSvc;
   RgSchPfsDlLc       *oldPfsLc;

   UNUSED(pfsCell);

   TRC2(rgSCHPfsDlLcAmbrActv);

   pfsLc->bo = bo;


   if (lc == pfsUe->ambrSvc)
   {
      /*rg002.301_LPR1-ADD-Added the handling to update the effBo, remRate.
       * Deleted the invocation of function rgSCHPfsDlLcAddToAmbrQ as this
       * is being called from function rgSCHPfsDlPrioAmbrSvcUpd.*/
      /*rg002.301_LPR1-MOD-Modified the invocation of below function to
       *specific one more parameter.*/
      rgSCHPfsDlPrioAmbrSvcUpd(pfsCell, ue, allocd);
      RETVOID;
   }
   rgSCHPfsDlLcInsInUeAmbrLst(pfsUe, lc);
   if (oldLc == NULLP)
   {
      /*rg002.301_LPR1-MOD-Modified the invocation of below function to
       *specific one more parameter.*/
      rgSCHPfsDlPrioAmbrSvcUpd(pfsCell, ue, 0);
   }
   else if (oldLc != pfsUe->ambrSvc)
   {
      oldPfsLc = (RgSchPfsDlLc *)oldLc->sch;
      /* This lc has displaced oldLc to become the
       * UE's highest priority AMBR svc. So remove
       * oldLc from cell wide queues. */
      rgSCHPfsDlLcRmvFrmPrioQ(pfsCell, pfsUe, oldPfsLc);
      /*rg002.301_LPR1-MOD-Modified the invocation of below function to
       *specific one more parameter.*/
      rgSCHPfsDlPrioAmbrSvcUpd(pfsCell, ue, 0);
   }
   else
   {
      pfsLc->effBo   = 0;
   }
   RETVOID;
}  /* rgSCHPfsDlLcAmbrActv */

/***********************************************************
 *
 *     Func : rgSCHPfsDlLcAmbrInactv
 *
 *     Desc : Take all necessary action when BO=0 is reported
 *            for an AMBR service
 *
 *     Ret  : Void
 *
 *     Notes: It is assumed that the case of remAmbr being
 *            zero is handled earlier.
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlLcAmbrInactv
(
RgSchPfsDlCell     *pfsCell,
RgSchUeCb          *ue,
RgSchDlLcCb        *lc
)
#else
PRIVATE Void rgSCHPfsDlLcAmbrInactv(pfsCell, ue, lc)
RgSchPfsDlCell     *pfsCell;
RgSchUeCb          *ue;
RgSchDlLcCb        *lc;
#endif
{
   RgSchPfsDlUe       *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
   RgSchPfsDlLc       *pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);

   UNUSED(pfsCell);

   TRC2(rgSCHPfsDlLcAmbrInactv);

   pfsLc->effBo   = 0;
   pfsLc->bo      = 0;

   if (pfsLc->ambrLnk.node == NULLP)
   {
      RETVOID;
   }
   cmLListDelFrm(&pfsUe->ambrLst, &pfsLc->ambrLnk);
   pfsLc->ambrLnk.node = NULLP;
   if (pfsUe->ambrSvc == lc)
   {
      rgSCHPfsDlLcRmvFrmPrioQ(pfsCell, pfsUe, pfsLc);
      if (pfsUe->ambrLst.first)
      {
         pfsUe->ambrSvc = (RgSchDlLcCb *)pfsUe->ambrLst.first->node;
         /*rg002.301_LPR1-MOD-Modified the invocation of below function to
          *specific one more parameter.*/
         rgSCHPfsDlPrioAmbrSvcUpd(pfsCell, ue, 0);
      }
      else
      {
         pfsUe->ambrSvc = NULLP;
      }
   }
   RETVOID;
}  /* rgSCHPfsDlLcAmbrInactv */


/***********************************************************
 *
 *     Func : rgSCHPfsDlLcInsInUeAmbrLst
 *
 *     Desc : If LC is not already in list, 
 *            add DL AMBR LC into UE's list of AMBR LCs
 *            maintaining correct priority order.
 *            Update pfsUe->ambrSvc also if needed.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlLcInsInUeAmbrLst
(
RgSchPfsDlUe       *pfsUe,
RgSchDlLcCb        *lc
)
#else
PRIVATE Void rgSCHPfsDlLcInsInUeAmbrLst(pfsUe, lc)
RgSchPfsDlUe       *pfsUe;
RgSchDlLcCb        *lc;
#endif
{
   RgSchPfsDlLc       *pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
   CmLListCp          *cp = &pfsUe->ambrLst;
   CmLList            *lnk;
   TRC2(rgSCHPfsDlLcInsInUeAmbrLst);

   if (pfsLc->ambrLnk.node == (PTR)lc)
   {
      RETVOID;
   }
   pfsLc->ambrLnk.node = (PTR)lc;
   for (lnk = cp->first; lnk; lnk = lnk->next)
   {
      RgSchDlLcCb   *tmpLc = (RgSchDlLcCb *)lnk->node;
      RgSchPfsDlLc  *tmpPfsLc = RG_SCH_PFS_GET_PFSDLLC(tmpLc);
      if (pfsLc->svcPrio >= tmpPfsLc->svcPrio)
      {
         cp->crnt = lnk;
         cmLListInsCrnt(cp, &pfsLc->ambrLnk);
         RETVOID;
      }
   }
   cmLListAdd2Tail(cp, &pfsLc->ambrLnk);
   if (cp->first->node == (PTR)lc)
   {
      pfsUe->ambrSvc = lc;
   }
   RETVOID;
}  /* rgSCHPfsDlLcInsInUeAmbrLst */

/***********************************************************
 *
 *     Func : rgSCHPfsDlGetQId
 *
 *     Desc : Return the queue Id appropriate for the passed
 *            priority value.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U8 rgSCHPfsDlGetQId
(
RgSchPfsPrioRange  *range,
U32                 prioVal,
U8                  maxQId
)
#else
PRIVATE U8 rgSCHPfsDlGetQId(range, prioVal, maxQId)
RgSchPfsPrioRange  *range;
U32                 prioVal;
U8                  maxQId;
#endif
{
   U32      qId;
   TRC2(rgSCHPfsDlGetQId);

#if (ERRCLASS & ERRCLS_DEBUG)
   if (prioVal < range->min)
   {
      prioVal = range->min;
   }
#endif
   /* MS_FIX : syed For small range and large number of Queues
    * precalculated numElemPerQ can be highly inaccurate. It
    * was leading to a crash.  */
   qId = ((prioVal - range->min) * range->numQs)/range->maxDiff;
#if (ERRCLASS & ERRCLS_DEBUG)
   if (qId > maxQId)
   {
      qId = maxQId;
   }
#else
#ifdef MSPD
   if (qId > maxQId)
   {
      MSPD_ERR("<PFSPRIO> qId(%d) > maxQId(%d) for prioVal(%d) min(%d) maxDiff(%d)", qId, maxQId, prioVal, range->min, range->maxDiff);
   }
#else
   UNUSED(maxQId);
#endif
#endif
   RETVALUE(qId);
}  /* rgSCHPfsDlGetQId */



/***********************************************************
 *
 *     Func : rgSCHPfsDlLcRmvFrmPrioQ
 *
 *     Desc : Remove DL LC from cell wide priority queue
 *            if present.
 *            Since LCs which are in cell wide queues are
 *            kept track of in ue (pfsUe->inQLcLst), remove
 *            from this list as well.
 *            LTEMAC_MIMO:
 *            Remove DL LC from UE wide priority queue if present
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlLcRmvFrmPrioQ
(
RgSchPfsDlCell     *pfsCell,
RgSchPfsDlUe       *pfsUe,
RgSchPfsDlLc       *pfsLc
)
#else
PRIVATE Void rgSCHPfsDlLcRmvFrmPrioQ(pfsCell, pfsUe, pfsLc)
RgSchPfsDlCell     *pfsCell;
RgSchPfsDlUe       *pfsUe;
RgSchPfsDlLc       *pfsLc;
#endif
{
   UNUSED(pfsCell);
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependncy on MIMO compile-time flag */
   TRC2(rgSCHPfsDlLcRmvFrmPrioQ);

   if (pfsLc->prioLstCp)
   {
      cmLListDelFrm(pfsLc->prioLstCp, &pfsLc->prioLnk);
      /*cmLListDelFrm(&pfsUe->inQLcLst, &pfsLc->inQLnk);*/
      pfsLc->prioLstCp = NULLP;
      /* TODO: You cannot use same link "prioLnk" to manipulate 
       * LC in 2 different lists */
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependncy on MIMO compile-time flag */
      cmLListDelFrm(&pfsUe->svcPriorLst, &pfsLc->svcPrioLnk); 
   }

   /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;
 
}  /* rgSCHPfsDlLcRmvFrmPrioQ */


/**
 * @brief DL UE refresh
 *
 * @details
 *
 *     Function : rgSCHPfsDlUeRefresh
 *
 *     Processing Steps:
 *     - If AMBR configured
 *       - Set UE's remAmbr = ambr
 *       - Set effBo of highest priority AMBR svc
 *         with non zero BO to min(BO, remAmbr)
 *       - Compute PFS priority
 *       - Update position of the svc in queues
 *     - For each GBR DTCH
 *       - Refresh remGbr and remDeltaMbr
 *       - Set effBo = min(BO, remGbr)
 *       - Compute PFS priority
 *       - Update in cell-wide queues appropriately
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsDlUeRefresh
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else
PUBLIC Void rgSCHPfsDlUeRefresh(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif
{
   RgSchPfsDlCell     *pfsCell = RG_SCH_PFS_GET_PFSCELLDL(cell);
   RgSchCmnDlUe       *cmnUe = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchPfsDlUe       *pfsUe = RG_SCH_PFS_GET_PFSUEDL(ue);
   CmLListCp          *cp;
   CmLList            *lnk;
   TRC2(rgSCHPfsDlUeRefresh);

   /* GBR related updates */
   cp = &pfsUe->cfgGbrLst;
   for (lnk = cp->first; lnk; lnk = lnk->next)
   {
      RgSchDlLcCb        *lc = (RgSchDlLcCb *)lnk->node;
      RgSchPfsDlLc       *pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
      /* MS_FIX : Updating PFS to start prio with highest possible value
         and diminishing prio val updates from there on. */
      pfsLc->pfsPrio = pfsCell->initPrioVal;
      rgSCHPfsDlLcGbrRefresh(cell, ue, lc);
   }

   /* AMBR related updates */
   if (!cmnUe->ambrCfgd)
   {
      RETVOID;
   }
#ifdef SCH_STATS
   cmnUe->remAmbrForStats = pfsUe->remAmbr;
#endif
   pfsUe->remAmbr = cmnUe->ambrCfgd;
   /* MS_FIX : Updating PFS to start prio with highest possible value
               and diminishing prio val updates from there on */
   rgSCHPfsInitFracPrioInfo(&pfsUe->fracPrioInfo, cmnUe->ambrCfgd); 
   cp = &pfsUe->cfgAmbrLst;
   for (lnk = cp->first; lnk; lnk = lnk->next)
   {
      RgSchDlLcCb        *lc    = (RgSchDlLcCb *)lnk->node;
      RgSchPfsDlLc       *pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
      /* MS_FIX : Updating PFS to start prio with highest possible value
         and diminishing prio val updates from there on. */
      pfsLc->pfsPrio = pfsCell->initPrioVal;
      if (pfsLc->bo)
      {
         pfsLc->effBo = RGSCH_MIN(pfsLc->bo, pfsUe->remAmbr);
         /* MS_FIX: Updating LC/UEs position in scheduling Queues should be
          * done only if UE is Active */
         if (RG_SCH_CMN_DL_IS_UE_ACTIVE(ue))
         {
            rgSCHPfsDlLcInsInUeAmbrLst(pfsUe, lc);
            /*rg002.301_LPR1-MOD-Modified the invocation of this function to
             * add a new parameter allocd.*/
            rgSCHPfsDlLcAddToAmbrQ(pfsCell, pfsUe, pfsLc, 0);
         }
         break;
      }
   }
   for (; lnk; lnk = lnk->next)
   {
      RgSchDlLcCb        *lc    = (RgSchDlLcCb *)lnk->node;
      RgSchPfsDlLc       *pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
      /* MS_FIX : Updating PFS to start prio with highest possible value
         and diminishing prio val updates from there on. */
      pfsLc->pfsPrio = pfsCell->initPrioVal;
      if (pfsLc->bo)
      {
         pfsLc->effBo = RGSCH_MIN(pfsLc->bo, pfsUe->remAmbr);
      }
   }

   /* Updates for others (SRBs and DRB0) not required */

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPfsDlLcGbrRefresh
 *
 *     Desc : Does refresh handling for a GBR LCG.
 *            Right now, just calls the BO update
 *            function which works fine but may not
 *            be very efficient.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlLcGbrRefresh
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchDlLcCb         *lc
)
#else
PRIVATE Void rgSCHPfsDlLcGbrRefresh(cell, ue, lc)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
RgSchDlLcCb         *lc;
#endif
{
   /* Could optimise slightly later */
   RgSchPfsDlCell     *pfsCell = RG_SCH_PFS_GET_PFSCELLDL(cell);
   RgSchPfsDlLc       *pfsLc = RG_SCH_PFS_GET_PFSDLLC(lc);
   TRC2(rgSCHPfsDlLcGbrRefresh);

   pfsLc->remGbr = pfsLc->gbr;
   pfsLc->remDeltaMbr = pfsLc->deltaMbr;
   /* MS_FIX : Updating PFS to start prio with highest possible value
               and diminishing prio val updates from there on */
   rgSCHPfsInitFracPrioInfo(&pfsLc->fracPrioInfo, pfsLc->gbr); 
   /* MS_FIX: Updating LC/UEs position in scheduling Queues should be
    * done only if UE is Active */
   if (RG_SCH_CMN_DL_IS_UE_ACTIVE(ue))
   {
      rgSCHPfsDlLcGbrBoUpd(pfsCell, ue, lc, pfsLc->bo, 0);
   }
   RETVOID;
}  /* rgSCHPfsDlLcGbrRefresh */


/***********************************************************
 *
 *     Func : rgSchPfsDlGetSvcPrio
 *
 *     Desc : Obtains SVC PFS priority from SVC QCI priority
 *            using the mapping table configured for cell.
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U32 rgSchPfsDlGetSvcPrio
(
RgSchPfsDlCell      *pfsCell,
U8                   prio
)
#else
PRIVATE U32 rgSchPfsDlGetSvcPrio(pfsCell, prio)
RgSchPfsDlCell      *pfsCell;
U8                   prio;
#endif
{
   TRC2(rgSchPfsDlGetSvcPrio);

   RETVALUE(pfsCell->tbls.svcPrioPrios[prio]);
}

/***********************************************************
 *
 *     Func : rgSCHPfsDlGetCqiPrio
 *
 *     Desc : Obtains CQI PFS priority from CQI priority
 *            table.
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U32 rgSCHPfsDlGetCqiPrio
(
RgSchPfsDlCell      *pfsCell,
U8                   cqi
)
#else
PRIVATE U32 rgSCHPfsDlGetCqiPrio(pfsCell, cqi)
RgSchPfsDlCell      *pfsCell;
U8                   cqi;
#endif
{
   TRC2(rgSCHPfsDlGetCqiPrio);

   /* MS_REMOVE : syed ERROR CHECK */
   if (cqi > 15)
   {
#ifdef MSPD
      stop_printf("ERROR!! DL CQI(%d) out or range\n", cqi);	   
#endif
   }
   RETVALUE(pfsCell->tbls.cqiPrios[cqi-1]);
}




/**** UPLINK functions ****/

/**
 * @brief Uplink intiatialisation for PFS
 *
 * @details
 *
 *     Function: rgSCHPfsUlInit
 *
 *     Processing Steps:
 *     - Assigns the various callback APIs
 *
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsUlInit
(
RgUlSchdApis         *apis
)
#else
PUBLIC Void rgSCHPfsUlInit(apis)
RgUlSchdApis         *apis;
#endif
{
   TRC2(rgSCHPfsUlInit);

   apis->rgSCHRgrUlUeCfg     = rgSCHPfsUlUeCfg;
   apis->rgSCHRgrUlUeRecfg   = rgSCHPfsUlUeRecfg;
   apis->rgSCHFreeUlUe       = rgSCHPfsUlUeDel;
   apis->rgSCHRgrUlCellCfg   = rgSCHPfsUlCellCfg;
   apis->rgSCHRgrUlCellRecfg = rgSCHPfsUlCellRecfg; 
   apis->rgSCHFreeUlCell     = rgSCHPfsUlCellDel;
   apis->rgSCHRgrUlLcgCfg    = rgSCHPfsUlLcgCfg;
   apis->rgSCHRgrUlLcgRecfg  = rgSCHPfsUlLcgRecfg;
   apis->rgSCHFreeUlLcg      = rgSCHPfsUlLcgDel;
   apis->rgSCHUlSched        = rgSCHPfsUlSched;
   apis->rgSCHUpdBsrShort    = rgSCHPfsUlUpdBsrShort;
   apis->rgSCHUpdBsrTrunc    = rgSCHPfsUlUpdBsrTrunc;
   apis->rgSCHUpdBsrLong     = rgSCHPfsUlUpdBsrLong;
   apis->rgSCHContResUlGrant = rgSCHPfsUlContResReq;
   apis->rgSCHSrRcvd         = rgSCHPfsUlSrInd;
   apis->rgSCHUlCqiInd       = rgSCHPfsUlCqiInd;
   apis->rgSCHUlUeRefresh    = rgSCHPfsUlUeRefresh;
   apis->rgSCHUlAllocFnlz    = rgSCHPfsUlOnAlloc;
   apis->rgSCHUlInactvtUes   = rgSCHPfsUlInactvLstInd;
   apis->rgSCHUlActvtUe      = rgSCHPfsUlActvUeInd;
   apis->rgSCHUlUeReset      = rgSCHPfsUlUeReset;

   RETVOID;
}


/**
 * @brief Cell configuration for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsUlCellCfg
 *
 *     Processing Steps:
 *      - Create the pfsCell info within passed SchCell
 *      - Update priority tables
 *      - Determine the min, max and priority ranges per
 *        PFS queue and store them
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgrCellCfg          *cfg
 *  @param[out] RgSchErrInfo        *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPfsUlCellCfg
(
RgSchCellCb          *cell,
RgrCellCfg           *cfg,
RgSchErrInfo         *err
)
#else
PUBLIC S16 rgSCHPfsUlCellCfg(cell, cfg, err)
RgSchCellCb          *cell;
RgrCellCfg           *cfg;
RgSchErrInfo         *err;
#endif
{
   RgrCellUlPfs        *pfsCfg = &cfg->ulSchInfo.cellUlPfs;
   RgSchCmnUlCell      *cmnCell = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchPfsUlCell      *pfsCell;
   U32                  minCqiPrio;
   U32                  maxCqiPrio;
   U32                  minFracPrio;
   U32                  maxFracPrio;
   U32                  minLcgPrio;
   U32                  maxLcgPrio;
   U32                  minPrios[3];
   U32                  maxPrios[3];

   UNUSED(err);
   TRC2(rgSCHPfsUlCellCfg);

   if (rgSCHUtlAllocSBuf(cell->instIdx,
            (Data**)&cmnCell->schSpfc, (sizeof(RgSchPfsUlCell))) != ROK)
   {
      RETVALUE(RFAILED);
   }

   pfsCell = (RgSchPfsUlCell *)(cmnCell->schSpfc);

   rgSCHPfsFillTbl(pfsCfg->cqiPrios, pfsCell->tbls.cqiPrios, RGR_PFS_CQI_PRIO_LVLS,
         &minCqiPrio, &maxCqiPrio);
   rgSCHPfsFillTbl(pfsCfg->fracPrios, pfsCell->tbls.fracPrios, RGR_PFS_FRAC_PRIO_LVLS,
         &minFracPrio, &maxFracPrio);
   rgSCHPfsFillTbl(pfsCfg->lcgPrios, pfsCell->tbls.lcgPrios, RGR_PFS_LCG_PRIO_LVLS,
         &minLcgPrio, &maxLcgPrio);

   /* Make sure that there are non-zero priority values */
   if ((maxCqiPrio == 0) || (maxLcgPrio == 0) || (maxFracPrio == 0))
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),
               "rgSCHPfsDlCellCfg(): Priority values are all zero\n"));
      MSPD_ERR("Priority values are all zero\n");
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx,
         (Data**)(&(cmnCell->schSpfc)), (sizeof(RgSchPfsUlCell)));
      RETVALUE(RFAILED);
   }

   /* Make sure that the product of maximum priority values dooes
    * not overflow a U32 */
   if ((maxCqiPrio > ((U32)0xffffffff) / maxFracPrio)
         || (maxCqiPrio * maxFracPrio > ((U32)-1) / maxLcgPrio))
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),
               "rgSCHPfsUlCellCfg(): Priority values too large\n"));
      MSPD_ERR("Priority values too large\n");
	  /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx,
         (Data**)(&(cmnCell->schSpfc)), (sizeof(RgSchPfsUlCell)));
      RETVALUE(RFAILED);
   }

   minPrios[0] = minCqiPrio;
   minPrios[1] = minFracPrio;
   minPrios[2] = minLcgPrio;
   maxPrios[0] = maxCqiPrio;
   maxPrios[1] = maxFracPrio;
   maxPrios[2] = maxLcgPrio;
   rgSCHPfsFillRange(&pfsCell->range, minPrios, maxPrios,
         RG_SCH_PFS_UL_NUM_QUEUES);
   /* MS_FIX : syed MAX Initial Priority Value to be used for prio initialization. */
   pfsCell->initPrioVal = maxCqiPrio * maxFracPrio * maxLcgPrio;

   RETVALUE(ROK);
}

/**
 * @brief Cell reconfiguration for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsUlCellRecfg
 *
 *     Processing Steps:
 *      - [NONE - no reconfig for PFS]
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgrCellRecfg      *recfg
 *  @param[out] RgSchErrInfo      *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPfsUlCellRecfg
(
RgSchCellCb     *cell,
RgrCellRecfg    *recfg,
RgSchErrInfo    *err
)
#else
PUBLIC S16 rgSCHPfsUlCellRecfg(cell, recfg, err)
RgSchCellCb     *cell;
RgrCellRecfg    *recfg;
RgSchErrInfo    *err;
#endif
{
   /* No reconfig for PFS, so just return ROK */
   UNUSED(cell);
   UNUSED(recfg);
   UNUSED(err);
   TRC2(rgSCHPfsUlCellRecfg);

   RETVALUE(ROK);
}

/**
 * @brief Cell deletion for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsUlCellDel
 *
 *     Processing Steps:
 *      - Free PFS related information per cell.
 *      - [ Nothing else needs to be done.
 *        Common modules would take care of deleting
 *        each UE in the cell, during which time PFS
 *        shall be called for UE deletion. ]
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsUlCellDel
(
RgSchCellCb       *cell
)
#else
PUBLIC Void rgSCHPfsUlCellDel(cell)
RgSchCellCb       *cell;
#endif
{
   RgSchCmnCell *cmnCell = (RgSchCmnCell *)(cell->sc.sch);
   TRC2(rgSCHPfsUlCellDel);

   if (cmnCell->ul.schSpfc == NULLP)
   {
      RETVOID;
   }
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(cmnCell->ul.schSpfc)), (sizeof(RgSchPfsUlCell)));
   RETVOID;
}

/**
 * @brief UE configuration for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsUlUeCfg
 *
 *     Processing Steps:
 *      - Create the pfsUeCb
 *      - Initialise variables
 *      - [UE specific refresh timer actions to happen
 *        as part of common modules]
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgrUeCfg         *cfg
 *  @param[out] RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPfsUlUeCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrUeCfg         *cfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHPfsUlUeCfg(cell, ue, cfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgrUeCfg         *cfg;
RgSchErrInfo     *err;
#endif
{
   RgSchPfsUlCell      *pfsCell = RG_SCH_PFS_GET_PFSCELLUL(cell);
   RgSchCmnUlUe        *cmnUe = &((RgSchCmnUe *)ue->sch)->ul;
   RgSchPfsUlUe        *pfsUe;

   UNUSED(cfg);
   UNUSED(err);

   TRC2(rgSCHPfsUlUeCfg);

   if (rgSCHUtlAllocSBuf(cell->instIdx,
            (Data**)&cmnUe->schSpfc, (sizeof(RgSchPfsUlUe))) != ROK)
   {
      RETVALUE(RFAILED);
   }
   pfsUe = RG_SCH_PFS_GET_PFSUEUL(ue);
   /* Max bitrate and remaining bitrate are present in cmnUe */
#ifdef TFU_UPGRADE
   pfsUe->cqi  = cmnUe->validUlCqi;
#else
   pfsUe->cqi   = cmnUe->crntUlCqi[0];
#endif 
   pfsUe->cqiPrio = rgSCHPfsUlGetCqiPrio(pfsCell, pfsUe->cqi);
   pfsUe->prioLnk.node = (PTR)ue;
   /* MS_FIX : Updating PFS to start prio with highest possible value
      and diminishing prio val updates from there on. */
   pfsUe->pfsPrio = pfsCell->initPrioVal;
   rgSCHPfsInitFracPrioInfo(&pfsUe->fracPrioInfo, cmnUe->cfgdUeBR); 

   RETVALUE(ROK);
}

/**
 * @brief UE reconfiguration for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsUlUeRecfg
 *
 *     Processing Steps:
 *     - [NONE]
 *
 *  @param[in]   RgSchCellCb      *cell
 *  @param[in]   RgSchUeCb        *ue
 *  @param[in]   RgrUeRecfg       *recfg
 *  @param[out]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPfsUlUeRecfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrUeRecfg       *recfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHPfsUlUeRecfg(cell, ue, recfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgrUeRecfg       *recfg;
RgSchErrInfo     *err;
#endif
{
   /* Updation of max bitrate etc is done by common */
   UNUSED(cell);
   UNUSED(ue);
   UNUSED(recfg);
   UNUSED(err);
   TRC2(rgSCHPfsUlUeRecfg);
   RETVALUE(ROK);
}

/**
 * @brief UE delete for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsUlUeDel
 *
 *     Processing Steps:
 *     - [Removal of UE allocs to happen as part
 *       of common modules]
 *     - Remove UE from any queue if present
 *     - Free UE sched info
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsUlUeDel
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PUBLIC Void rgSCHPfsUlUeDel(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   RgSchPfsUlCell     *pfsCell = RG_SCH_PFS_GET_PFSCELLUL(cell);
   RgSchCmnUlUe       *cmnUe = &((RgSchCmnUe *)ue->sch)->ul;
   RgSchPfsUlUe       *pfsUe = RG_SCH_PFS_GET_PFSUEUL(ue);

   TRC2(rgSCHPfsUlUeDel);

   if (pfsUe == NULLP)
   {
      RETVOID;
   }
   rgSCHPfsUlRmvUeFrmQ(pfsCell, pfsUe);
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(cmnUe->schSpfc)), (sizeof(RgSchPfsUlUe)));
   RETVOID;
}

/**
 * @brief UE reset for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsUlUeReset
 *
 *     Processing Steps:
 *      - [Common is expected to remove retx HARQ procs and invoke API]
 *      - Remove UE from queue if present
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsUlUeReset
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PUBLIC Void rgSCHPfsUlUeReset(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   RgSchPfsUlCell      *pfsCell = RG_SCH_PFS_GET_PFSCELLUL(cell);
   RgSchPfsUlUe        *pfsUe = RG_SCH_PFS_GET_PFSUEUL(ue);

   TRC2(rgSCHPfsUlUeReset);

   rgSCHPfsUlRmvUeFrmQ(pfsCell, pfsUe);
   RETVOID;
}


/**
 * @brief LC configuration for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsUlLcgCfg
 *
 *     Processing Steps:
 *     - NONE
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchUlLcCb      *lcg
 *  @param[in]  RgrLcgCfg        *cfg
 *  @param[out] RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPfsUlLcgCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *lcg,
RgrLcgCfg        *cfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHPfsUlLcgCfg(cell, ue, lcg, cfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchLcgCb       *lcg;
RgrLcgCfg        *cfg;
RgSchErrInfo     *err;
#endif
{
   UNUSED(cell);
   UNUSED(ue);
   UNUSED(lcg);
   UNUSED(cfg);
   UNUSED(err);
   TRC2(rgSCHPfsUlLcgCfg);

   RETVALUE(ROK);
}

/**
 * @brief LC re-configuration for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsUlLcgRecfg
 *
 *     Processing Steps:
 *     - NONE
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchLcgCb       *lcg
 *  @param[in]  RgrLcgRecfg      *recfg
 *  @param[out] RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPfsUlLcgRecfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *lcg,
RgrLcgRecfg      *recfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHPfsUlLcgRecfg(cell, ue, lcg, recfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchLcgCb       *lcg;
RgrLcgRecfg      *recfg;
RgSchErrInfo     *err;
#endif
{
   /* Need not do anything */
   UNUSED(cell);
   UNUSED(ue);
   UNUSED(lcg);
   UNUSED(recfg);
   UNUSED(err);
   TRC2(rgSCHPfsUlLcgRecfg);

   RETVALUE(ROK);
}

/**
 * @brief LC deletion for PFS
 *
 * @details
 *
 *     Function : rgSCHPfsUlLcgDel
 *
 *     Processing Steps:
 *     - Update UE's position in queues if needed
 *       [Assumed that lcg's bs has been set to zero]
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchLcgCb       *lcg
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsUlLcgDel
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *lcg
)
#else
PUBLIC Void rgSCHPfsUlLcgDel(cell, ue, lcg)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchLcgCb       *lcg;
#endif
{
   UNUSED(lcg);
   TRC2(rgSCHPfsUlLcgDel);

   rgSCHPfsUlMngUeInQ(cell, ue, 0);
   RETVOID;
}


/**
 * @brief UL UE refresh
 *
 * @details
 *
 *     Function : rgSCHPfsUlUeRefresh
 *
 *     Processing Steps:
 *     - Update UE's position in queues if needed
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsUlUeRefresh
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else
PUBLIC Void rgSCHPfsUlUeRefresh(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif
{
   RgSchPfsUlCell      *pfsCell = RG_SCH_PFS_GET_PFSCELLUL(cell);
   RgSchCmnUlUe        *cmnUe = &((RgSchCmnUe *)ue->sch)->ul;
   RgSchPfsUlUe        *pfsUe = RG_SCH_PFS_GET_PFSUEUL(ue);
   TRC2(rgSCHPfsUlUeRefresh);
   /* MS_FIX : Updating PFS to start prio with highest possible value
      and diminishing prio val updates from there on. */
   pfsUe->pfsPrio = pfsCell->initPrioVal;
   rgSCHPfsInitFracPrioInfo(&pfsUe->fracPrioInfo, cmnUe->cfgdUeBR); 
   /* Max bitrate being refreshed would happen as part of common processing,
    * which would also update effBsr */
   rgSCHPfsUlMngUeInQ(cell, ue, 0);
   RETVOID;
}


/**
 * @brief Uplink scheduling function
 *
 * @details
 *
 *     Function : rgSCHPfsUlSched
 *
 *     Processing Steps:
 *     - Schedule for new transmissions
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo   *alloc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsUlSched
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *alloc
)
#else
PUBLIC Void rgSCHPfsUlSched(cell, alloc)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *alloc;
#endif
{
   RgSchCmnUlCell  *cmnCell = RG_SCH_CMN_GET_UL_CELL(cell);
   U8               remUe = cmnCell->maxUeNewTxPerTti;
   TRC2(rgSCHPfsUlSched);

   rgSCHPfsUlSchedNewTx(cell, alloc, remUe);
   RETVOID;
}

/**
 * @brief Uplink scheduling function
 *
 * @details
 *
 *     Function : rgSCHPfsUlSchedNewTx
 *
 *     Processing Steps:
 *     - Schedule new tranmissions upto the limit of
 *       maximum UEs that can be considered for new tx
 *       in a TTI. Go through cell wide uplink
 *       queues in the following order:
 *       - Schedule UEs in contResLst for contention
 *         resolution (schedule for small TB size)
 *       - Schedule UEs corresponding to appearance
 *         in SRB queue: If BO is zero (SR case)
 *         schedule the smallest TB possible
 *       - Schedule UEs corresponding to appearance
 *         in other queues
 *     - Note: During each step of scheduling a UE
 *       - Verify that UE has not been scheduled for
 *         retx already
 *       - Set HARQ process contents corresponding to
 *         new transmission and allocation
 *       - Add UE to queue in 'alloc' and update 'alloc'
 *         info
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *alloc
 *  @param[in]  U8                      remUe
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsUlSchedNewTx
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *alloc,
U8                       remUe
)
#else
PRIVATE Void rgSCHPfsUlSchedNewTx(cell, alloc, remUe)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *alloc;
U8                       remUe;
#endif
{
#ifdef MSPD
   /* MS_FIX ccpu00120610*/
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   U8 remAllocs = schCmnCell->ul.maxAllocPerUlSf - *alloc->sf->allocCountRef;
#endif
   TRC2(rgSCHPfsUlSchedNewTx);

#ifdef  MSPD
   /* MS_FIX:ccpu00120610 consider remaining allocs in a subframe, because allocs may
    * have been consumed earlier as part of msg3 allocation.
    * Note: this needs to be carried over to other schedulers. */
   if (remUe > remAllocs)
   {
      remUe = remAllocs;
   }
   /* MS_WORKAROUND : Limiting one PUSCH per UL SF */
   /* MUE_PERTTI_UL: TODO: Remove this workaround and test */
#endif
   rgSCHPfsUlSchedContRes(cell, alloc, &remUe);
   rgSCHPfsUlSchedSig(cell, alloc, &remUe);
   rgSCHPfsUlSchedDat(cell, alloc, &remUe);
#ifdef MSPD
    if (*alloc->sf->allocCountRef > 1)
   {
      stop_printf("More than 1 UE per TTI allocated. Remove this when testing MUE per TTI\n");
   }
#endif
   RETVOID;
}


/**
 * @brief Uplink scheduling function
 *
 * @details
 *
 *     Function : rgSCHPfsUlSchedContRes
 *
 *     Processing Steps:
 *     - Schedule for UEs for which contention resolution
 *       is pending. Small allocations are scheduled
 *       for such UEs (enough to hold BSR), independent
 *       of current BSR status.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *alloc
 *  @param[in,out]  U8                 *remUeRef
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsUlSchedContRes
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *alloc,
U8                      *remUeRef
)
#else
PRIVATE Void rgSCHPfsUlSchedContRes(cell, alloc, remUeRef)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *alloc;
U8                      *remUeRef;
#endif
{
   RgSchPfsUlCell      *pfsCell = RG_SCH_PFS_GET_PFSCELLUL(cell);
   CmLListCp           *q       = &pfsCell->txQueues.contResLst;
   CmLList             *lnk;
#ifdef LTEMAC_HDFDD
   Bool                ulAllowed = FALSE;
#endif
   TRC2(rgSCHPfsUlSchedContRes);

   for (lnk = q->first; lnk && *remUeRef; lnk = lnk->next, --*remUeRef)
   {
      RgSchUeCb          *ue = (RgSchUeCb *)lnk->node;

#ifdef LTEMAC_HDFDD
      if (RG_SCH_HDFDD_UE_ENBLD(ue))
      {
         rgSCHCmnHdFddChkUlAllow (cell, ue, &ulAllowed);
         if (ulAllowed == FALSE)
         {
            continue;
         }
      }
#endif
      rgSCHPfsUlSchedUeForContRes(cell, alloc, ue);
   }

   RETVOID;
}

/**
 * @brief Uplink scheduling function
 *
 * @details
 *
 *     Function : rgSCHPfsUlSchedSig
 *
 *     Processing Steps:
 *     - Schedule for UEs for which SR is pending or
 *       which have data pending on LCG 0 (expected
 *       to correspond to signaling information)
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *alloc
 *  @param[in,out]  U8                 *remUeRef
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsUlSchedSig
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *alloc,
U8                      *remUeRef
)
#else
PRIVATE Void rgSCHPfsUlSchedSig(cell, alloc, remUeRef)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *alloc;
U8                      *remUeRef;
#endif
{
   RgSchPfsUlCell      *pfsCell = RG_SCH_PFS_GET_PFSCELLUL(cell);
   CmLListCp           *q       = &pfsCell->txQueues.sigLst;
   CmLList             *lnk;
#ifdef LTEMAC_HDFDD
   Bool                ulAllowed = FALSE;
#endif
   TRC2(rgSCHPfsUlSchedSig);

   for (lnk = q->first; lnk && *remUeRef; lnk = lnk->next)
   {
      RgSchUeCb          *ue = (RgSchUeCb *)lnk->node;
#ifdef LTEMAC_HDFDD
      if (RG_SCH_HDFDD_UE_ENBLD(ue))
      {
         rgSCHCmnHdFddChkUlAllow (cell, ue, &ulAllowed);
         if (ulAllowed == FALSE)
         {
            continue;
         }
      }
#endif
      if (rgSCHPfsUlSchedUeForSigDat(cell, alloc, ue) == ROK)
      {
         --*remUeRef;
      }
   }

   RETVOID;
}

/**
 * @brief Uplink scheduling function
 *
 * @details
 *
 *     Function : rgSCHPfsUlSchedDat
 *
 *     Processing Steps:
 *     - Schedule for UEs which have data pending
 *       on LCG 1/2/3
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *alloc
 *  @param[in,out]  U8                 *remUeRef
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsUlSchedDat
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *alloc,
U8                      *remUeRef
)
#else
PRIVATE Void rgSCHPfsUlSchedDat(cell, alloc, remUeRef)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *alloc;
U8                      *remUeRef;
#endif
{
   RgSchPfsUlCell      *pfsCell = RG_SCH_PFS_GET_PFSCELLUL(cell);
   CmLList             *lnk;
   S16                   idx;
#ifdef LTEMAC_HDFDD
   Bool                ulAllowed = FALSE;
#endif
   TRC2(rgSCHPfsUlSchedDat);

   for (idx = RG_SCH_PFS_UL_NUM_QUEUES-1; idx >= 0; --idx)
   {
      CmLListCp           *q = &pfsCell->txQueues.dataLst[idx];
      for (lnk = q->first; lnk && *remUeRef; lnk = lnk->next)
      {
         RgSchUeCb          *ue = (RgSchUeCb *)lnk->node;
#ifdef LTEMAC_HDFDD
         if (RG_SCH_HDFDD_UE_ENBLD(ue))
         {
            rgSCHCmnHdFddChkUlAllow (cell, ue, &ulAllowed);
            if (ulAllowed == FALSE)
            {
               continue;
            }
         }
#endif
         if (rgSCHPfsUlSchedUeForSigDat(cell, alloc, ue) == ROK)
         {
            --*remUeRef;
         }
      }
   }

   RETVOID;
}

/**
 * @brief Uplink scheduling function
 *
 * @details
 *
 *     Function : rgSCHPfsUlSchedUeForContRes
 *
 *     Processing Steps:
 *     - Schedule a UE for contention resolution
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *alloc
 *  @param[in]  RgSchUeCb              *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsUlSchedUeForContRes
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *alloc,
RgSchUeCb               *ue
)
#else
PRIVATE Void rgSCHPfsUlSchedUeForContRes(cell, alloc, ue)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *alloc;
RgSchUeCb               *ue;
#endif
{
   RgSchCmnUlUe       *cmnUe = &((RgSchCmnUe *)ue->sch)->ul;
   UNUSED(cell);
   TRC2(rgSCHPfsUlSchedUeForContRes);

   cmnUe->alloc.reqBytes = RG_SCH_CMN_MAX_UL_CONTRES_GRNT;
   rgSCHCmnUlAdd2CntResLst(alloc, ue);
   RETVOID;
}

/**
 * @brief Uplink scheduling function
 *
 * @details
 *
 *     Function : rgSCHPfsUlSchedUeForSigDat
 *
 *     Processing Steps:
 *     - Schedule a UE for signaling+data
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *alloc
 *  @param[in]  RgSchUeCb              *ue
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHPfsUlSchedUeForSigDat
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *alloc,
RgSchUeCb               *ue
)
#else
PRIVATE S16 rgSCHPfsUlSchedUeForSigDat(cell, alloc, ue)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *alloc;
RgSchUeCb               *ue;
#endif
{
   RgSchCmnUlCell      *cmnCell = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlUe        *cmnUe = &((RgSchCmnUe *)ue->sch)->ul;
   RgSchUlHqProcCb     *proc;

   UNUSED(cell);

   TRC2(rgSCHPfsUlSchedUeForSigDat);

   proc = rgSCHUhmGetUlHqProc(ue, cmnCell->schdHqProcIdx);
#ifdef LTEMAC_SPS
   if (RG_SCH_CMN_IS_UL_UE_RETX(ue) || RG_SCH_CMN_IS_SPS_SCHD(ue))
#else
   if (RG_SCH_CMN_IS_UL_UE_RETX(ue))
#endif
   {
      /* UE already scheduled in this subframe (for retx) */
#ifdef MSPD
         MSPD_DBG("Retransmitting  %d or inactive %d @ (%d,%d) remTx %d\n",
		 	 RG_SCH_CMN_IS_UL_UE_RETX(ue), ue->ul.ulInactvMask,
		 	 cell->crntTime.sfn, cell->crntTime.subframe, proc->remTx);
#endif
      RETVALUE(RFAILED);
   }

   cmnUe->alloc.reqBytes = cmnUe->effBsr; /* cmn considers maxBytesPerUe */
   if (cmnUe->alloc.reqBytes < 4)
   {
      cmnUe->alloc.reqBytes = 4;
   }
   rgSCHCmnUlAdd2UeLst(alloc, ue);
   RETVALUE(ROK);
}

/**
 * @brief Short BSR reception handling
 *
 * @details
 *
 *     Function : rgSCHPfsUlUpdBsrShort
 *
 *     Processing Steps:
 *     - BSR updates for each LCG (and UE) would have happened
 *       as part of common processing. Now only call the
 *       function rgSCHPfsUlMngUeInQ() that looks at
 *       the updated common variables to correctly
 *       (re)position UEs.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  RgSchLcgCb         *lcg
 *  @param[in]  U8                  bsr
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsUlUpdBsrShort
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchLcgCb          *lcg,
U8                   bsr
)
#else
PUBLIC Void rgSCHPfsUlUpdBsrShort(cell, ue, lcg, bsr)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
RgSchLcgCb          *lcg;
U8                   bsr;
#endif
{
   UNUSED(lcg);
   UNUSED(bsr);

   TRC2(rgSCHPfsUlUpdBsrShort);

   rgSCHPfsUlMngUeInQ(cell, ue, 0);
   RETVOID;
}

/**
 * @brief Truncated BSR reception handling
 *
 * @details
 *
 *     Function : rgSCHPfsUlUpdBsrTrunc
 *
 *     Processing Steps:
 *     - BSR updates for each LCG (and UE) would have happened
 *       as part of common processing. Now only call the
 *       function rgSCHPfsUlMngUeInQ() that looks at
 *       the updated common variables to correctly
 *       (re)position UEs.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  RgSchLcgCb         *lcg
 *  @param[in]  U8                  bsr
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsUlUpdBsrTrunc
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchLcgCb          *lcg,
U8                   bsr
)
#else
PUBLIC Void rgSCHPfsUlUpdBsrTrunc(cell, ue, lcg, bsr)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
RgSchLcgCb          *lcg;
U8                   bsr;
#endif
{
   UNUSED(lcg);
   UNUSED(bsr);

   TRC2(rgSCHPfsUlUpdBsrTrunc);

   rgSCHPfsUlMngUeInQ(cell, ue, 0);
   RETVOID;
}

/**
 * @brief Long BSR reception handling
 *
 * @details
 *
 *     Function : rgSCHPfsUlUpdBsrLong
 *
 *     Processing Steps:
 *     - BSR updates for each LCG (and UE) would have happened
 *       as part of common processing. Now only call the
 *       function rgSCHPfsUlMngUeInQ() that looks at
 *       the updated common variables to correctly
 *       (re)position UEs.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  U8                  bsArr[]
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsUlUpdBsrLong
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
U8                  *bsArr
)
#else
PUBLIC Void rgSCHPfsUlUpdBsrLong(cell, ue, bsArr)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
U8                  *bsArr;
#endif
{
   UNUSED(bsArr);

   TRC2(rgSCHPfsUlUpdBsrLong);

   rgSCHPfsUlMngUeInQ(cell, ue, 0);
   RETVOID;
}

/* MS_FIX : Updating PFS to start prio with highest possible value
            and diminishing prio val updates from there on */
/***********************************************************
 *
 *     Func : rgSCHPfsUlMngUeInQ
 *        
 *     Desc : This function looks at the current
 *            list of LCGs with pending data,
 *            and updates UE position in uplink
 *            queues accordingly.
 *
 *     Ret  : Void 
 *
 *     Notes: It assumes that BS of each LCG is correctly
 *            updated, and UE's remaining max bit rate
 *            (cmnUe->effUeBR) is also correctly updated.
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsUlMngUeInQ
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
U32                 allocd
)
#else
PRIVATE Void rgSCHPfsUlMngUeInQ(cell, ue, allocd)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
U32                 allocd;
#endif
{
   RgSchPfsUlCell      *pfsCell = RG_SCH_PFS_GET_PFSCELLUL(cell);
   RgSchCmnUlUe        *cmnUe = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchPfsUlUe        *pfsUe = RG_SCH_PFS_GET_PFSUEUL(ue);
   RgSchLcgCb          *lcg;
   RgSchCmnLcg         *cmnLcg;
   U32                  frac;
   U32                  fracPrio;
   U32                  lcgPrio;
   U32                  pfsPrio;
   S16                  idx;
   Bool                 prioChngd;

   TRC2(rgSCHPfsUlMngUeInQ);

   if (!RG_SCH_CMN_UL_IS_UE_ACTIVE(ue))
   {
      RETVOID;
   }
   cmnLcg = (RgSchCmnLcg *)ue->ul.lcgArr[0].sch;
   if (cmnLcg->bs || pfsUe->srRcvd)
   {
      rgSCHPfsUlAddUeToSigQ(cell, ue);
      RETVOID;
   }
   else if((!cmnLcg->bs) && (!pfsUe->srRcvd))
   {
      /*rg002.301_LPR2-ADD-Added the else-if conditional to do the handling of
       *removing the UE from the sigLst (if it exists) if no SR grant is pending 
       *and BS for lcg[0] is 0.*/
      if ((pfsUe->lstCp) && (pfsUe->lstCp == &pfsCell->txQueues.sigLst))
      {
         rgSCHPfsUlRmvUeFrmQ(pfsCell, pfsUe);
      }
   }

   if (!cmnUe->effBsr)
   {
      rgSCHPfsUlRmvUeFrmQ(pfsCell, pfsUe);
#ifdef RGR_V1
      /* rg006.201: [ccpu00112398] Added periodic BSR timer */
      if(cmnUe->totalBsr != 0)
      {
         if (ue->bsrTmr.tmrEvnt != TMR_NONE)
         {
            rgSCHTmrStopTmr(cell, ue->bsrTmr.tmrEvnt, ue); 
         }
         if (ue->ul.bsrTmrCfg.isPrdBsrTmrPres)
         {
            rgSCHTmrStartTmr(cell, ue, RG_SCH_TMR_BSR, 
                  ue->ul.bsrTmrCfg.prdBsrTmr);
#ifdef MSPD
            MSPD_DBG("Starting timer for periodic BSR at [%d,%d]\n", cell->crntTime.sfn, cell->crntTime.subframe);
#endif
         }
      }
#endif
      RETVOID;
   }
   for (idx = 1; idx < 4; ++idx)
   {
      lcg = &ue->ul.lcgArr[idx];
      cmnLcg = (RgSchCmnLcg *)lcg->sch;
      if (cmnLcg->bs)
      {
         break;
      }
   }

   /* Due to non-zero cmnUe->effBsr, there should be at least one LCG
    * with non-zero BS, and this is what cmnLcg points to */
#if (ERRCLASS & ERRCLS_DEBUG)
   if (idx == 4)
   {
      RGSCHDBGERR(cell->instIdx, (rgSchPBuf(cell->instIdx),
               "PfsUlMngUeInQ(): Despite non-zero cmnUe->effBsr, "
               "no LCG has BS\n"));
      MSPD_ERR("Despite non-zero cmnUe->effBsr, "
               "no LCG has BS\n");
	  rgSCHPfsUlRmvUeFrmQ(pfsCell, pfsUe);
      RETVOID;
   }
#endif

   /* MS_FIX : Correcting Fairness logic */	   
   lcgPrio = rgSCHPfsUlGetLcgPrio(pfsCell, lcg->lcgId);
   pfsUe->lcgPrio = lcgPrio;
   /* MS_FIX : Updating PFS to start prio with highest possible value
               and diminishing prio val updates from there on */
   rgSCHPfsUpdFracPrioInfo(&pfsUe->fracPrioInfo, allocd); 
   rgSCHPfsUlUpdPfsPrio(pfsCell, pfsUe, &prioChngd);
   rgSCHPfsUlAddUeToPfsQ(cell, ue, prioChngd);

   RETVOID;
}

/**
 * @brief Indication of list of UEs which should not be
 *        scheduled
 *
 * @details
 *
 *     Function : rgSCHPfsUlInactvLstInd
 *
 *     Processing Steps:
 *      - Indication for set of UEs for which downlink
 *        scheduling should not be done.
 *        Triggers removal of logical channels of UEs
 *        from queues.
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsUlInactvLstInd
(
RgSchCellCb      *cell,
CmLListCp        *lst
)
#else
PUBLIC Void rgSCHPfsUlInactvLstInd(cell, lst)
RgSchCellCb      *cell;
CmLListCp        *lst;
#endif
{
   CmLList         *lnk;
   TRC2(rgSCHPfsUlInactvLstInd);

   for (lnk = lst->first; lnk; lnk = lnk->next)
   {
      RgSchUeCb       *ue = (RgSchUeCb *)lnk->node;
      rgSCHPfsUlUeInactv(cell, ue);
   }
   RETVOID;
}

/**
 * @brief Indication of list of UEs which should not be
 *        scheduled
 *
 * @details
 *
 *     Function : rgSCHPfsUlUeInactv
 *
 *     Processing Steps:
 *      - Triggers (temporary) removal of UEs
 *        from queues.
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsUlUeInactv
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PRIVATE Void rgSCHPfsUlUeInactv(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   RgSchPfsUlCell      *pfsCell = RG_SCH_PFS_GET_PFSCELLUL(cell);
   RgSchPfsUlUe        *pfsUe = RG_SCH_PFS_GET_PFSUEUL(ue);
   TRC2(rgSCHPfsUlUeInactv);

   /* If UE is waiting for contention resolution, it keeps
    * monitoring PDCCH irrespective of there being gaps.
    * So don't take out of contention resolution queue. */
   if (pfsUe->lstCp == &pfsCell->txQueues.contResLst)
   {
      RETVOID;
   }

   rgSCHPfsUlRmvUeFrmQ(pfsCell, pfsUe);

   /* Managing of retx HARQ procs of UE is expected to be
    * done as part of common processing */

   RETVOID;
}

/**
 * @brief Indication that a UE is allowed to be scheduled
 *
 * @details
 *
 *     Function : rgSCHPfsUlActvUeInd
 *
 *     Processing Steps:
 *      - [This is an indication for UE for which uplink
 *        scheduling can now be done, since the last
 *        time it was not allowed.]
 *      - Triggers addition of UE to queue if needed.
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsUlActvUeInd
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PUBLIC Void rgSCHPfsUlActvUeInd(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   TRC2(rgSCHPfsUlActvUeInd);
   /* It is expected that managing retx HARQ procs of UE
    * is taken care of as part of common processing */
   rgSCHPfsUlMngUeInQ(cell, ue, 0);
   RETVOID;
}


/**
 * @brief Uplink CQI report handling
 *
 * @details
 *
 *     Function : rgSCHPfsUlCqiInd
 *
 *     Processing Steps:
 *     - If wideband CQI reported is zero, consider this to be
 *       an error and return
 *     - If wideband CQI has changed
 *       - Update CQI component of priority (specific to UE)
 *       - If UE was present in a PFS queue, update its position
 *         appropriately
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  TfuUlCqiRpt        *cqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsUlCqiInd
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
TfuUlCqiRpt         *cqiInfo
)
#else
PUBLIC Void rgSCHPfsUlCqiInd(cell, ue, cqiInfo)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
TfuUlCqiRpt         *cqiInfo;
#endif
{
   RgSchPfsUlCell      *pfsCell = RG_SCH_PFS_GET_PFSCELLUL(cell);
   RgSchCmnUlUe        *cmnUe = &((RgSchCmnUe *)ue->sch)->ul;
   RgSchPfsUlUe        *pfsUe = RG_SCH_PFS_GET_PFSUEUL(ue);
#ifdef TFU_UPGRADE
   U8                   cqi   = cmnUe->validUlCqi;
#else
   U8                   cqi   = cmnUe->crntUlCqi[0];
#endif 
   U32                  cqiPrio;
   Bool                 prioChngd;

   UNUSED(cqiInfo);

   TRC2(rgSCHPfsUlCqiInd);

   cqiPrio = rgSCHPfsUlGetCqiPrio(pfsCell, cqi);
   if (cqiPrio == pfsUe->cqiPrio)
   {
      RETVOID;
   }
   pfsUe->cqiPrio = cqiPrio;
   if (pfsUe->lstCp == NULLP) /* UE is not in any queue */
   {
      RETVOID;
   }
   if (pfsUe->lstCp == &pfsCell->txQueues.sigLst)
   {
      RETVOID;
   }
   /* MS_FIX : Updating PFS to start prio with highest possible value
               and diminishing prio val updates from there on */
   rgSCHPfsUlUpdPfsPrio(pfsCell, pfsUe, &prioChngd);
   rgSCHPfsUlAddUeToPfsQ(cell, ue, prioChngd); 

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPfsUlAddUeToSigQ
 *
 *     Desc : This function is called to add UE to LCG0
 *            queue
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsUlAddUeToSigQ
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else
PRIVATE Void rgSCHPfsUlAddUeToSigQ(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif
{
   RgSchPfsUlCell      *pfsCell = RG_SCH_PFS_GET_PFSCELLUL(cell);
   RgSchPfsUlUe        *pfsUe = RG_SCH_PFS_GET_PFSUEUL(ue);

   TRC2(rgSCHPfsUlAddUeToSigQ);

   if (pfsUe->lstCp)
   {
      if ((pfsUe->lstCp == &pfsCell->txQueues.sigLst) && (!pfsUe->schedInCrntUlSf))
      {
         RETVOID;	      
      }	      
      rgSCHPfsUlRmvUeFrmQ(pfsCell, pfsUe);
   }
   rgSCHPfsUlAddUeToQ(&pfsCell->txQueues.sigLst, ue);
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPfsUlAddUeToPfsQ
 *        
 *     Desc : This function is called when PFS priority
 *            has changed. It does not update UE position
 *            if UE is in LCG0 queue. It leaves the UE
 *            in the same PFS queue if it is to map
 *            to the same queue now. Removes from old
 *            queue and adds to end of new queue if PFS
 *            queue has changed.
 *
 *     Ret  : Void 
 *
 *     Notes: There is no check for contention resolution
 *            queue because it is not expected that UE
 *            transmits anything before contention is
 *            resolved, and hence trigger for this function
 *            should not have arisen.
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsUlAddUeToPfsQ
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
Bool                prioChngd
)
#else
PRIVATE Void rgSCHPfsUlAddUeToPfsQ(cell, ue, prioChngd)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
Bool                prioChngd;
#endif
{
   RgSchPfsUlCell      *pfsCell = RG_SCH_PFS_GET_PFSCELLUL(cell);
   RgSchPfsUlUe        *pfsUe = RG_SCH_PFS_GET_PFSUEUL(ue);
   U8                   qId;

   TRC2(rgSCHPfsUlAddUeToPfsQ);

   /* MS_FIX: Updating LC/UEs position in scheduling Queues should be
    * done only if UE is Active */
   if (!RG_SCH_CMN_UL_IS_UE_ACTIVE(ue))
   {
      RETVOID;
   }
   if (pfsUe->lstCp)
   {
      if ((!prioChngd) && (!pfsUe->schedInCrntUlSf))
      {
         RETVOID;
      }
      rgSCHPfsUlRmvUeFrmQ(pfsCell, pfsUe);
   }
   qId  = rgSCHPfsUlGetQId(pfsCell, pfsUe->pfsPrio);
   pfsUe->qId = qId;
   pfsUe->lstCp = &pfsCell->txQueues.dataLst[qId];
   rgSCHPfsUlAddUeToQ(pfsUe->lstCp, ue);
   RETVOID;
}


/**
 * @brief SR reception handling
 *
 * @details
 *
 *     Function : rgSCHPfsUlSrInd
 *
 *     Processing Steps:
 *     - Add UE to LCG0 queue
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsUlSrInd
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else
PUBLIC Void rgSCHPfsUlSrInd(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif
{
   RgSchPfsUlUe        *pfsUe = RG_SCH_PFS_GET_PFSUEUL(ue);
   TRC2(rgSCHPfsUlSrInd);

   pfsUe->srRcvd = TRUE;

   rgSCHPfsUlMngUeInQ(cell, ue, 0);
   /* Could instead check for if UE is active, and
    * call rgSCHPfsUlAddUeToSigQ(). This would be
    * a little quicker, albeit less maintainable.
    */

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPfsUlRmvUeFrmQ
 *        
 *     Desc : Remove UE from cell wide uplink tx queue
 *            if present
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsUlRmvUeFrmQ
(
RgSchPfsUlCell     *pfsCell,
RgSchPfsUlUe       *pfsUe
)
#else
PRIVATE Void rgSCHPfsUlRmvUeFrmQ(pfsCell, pfsUe)
RgSchPfsUlCell     *pfsCell;
RgSchPfsUlUe       *pfsUe;
#endif
{
   UNUSED(pfsCell);
   TRC2(rgSCHPfsUlRmvUeFrmQ);

   if (pfsUe->lstCp)
   {
      cmLListDelFrm(pfsUe->lstCp, &pfsUe->prioLnk);
      pfsUe->lstCp = NULLP;
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPfsUlAddUeToQ
 *        
 *     Desc : Insert UE to queue in descending order of pfsPrio
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsUlAddUeToQ
(
CmLListCp           *cp,
RgSchUeCb           *ue
)
#else
PRIVATE Void rgSCHPfsUlAddUeToQ(cp, ue)
CmLListCp           *cp;
RgSchUeCb           *ue;
#endif
{
   RgSchUeCb           *lUe;
   RgSchPfsUlUe        *lPfsUe;
   CmLList             *node;
   RgSchPfsUlUe        *pfsUe = RG_SCH_PFS_GET_PFSUEUL(ue);

   TRC2(rgSCHPfsUlAddUeToQ);

   /* MS_FIX : syed Each Priority Queue should be
    * sorted in descending order of pfsPrio. */
   /* Insertion can be improved by not always starting
    * from first node */
   node = cp->first;
   while(node)
   {
      lUe = (RgSchUeCb *)(node->node);
      lPfsUe = RG_SCH_PFS_GET_PFSUEUL(lUe);
      if (lPfsUe->pfsPrio <= pfsUe->pfsPrio)
      {
         break;
      }
      node = node->next;
   }
   if (node == NULLP)
   {
      /* We have come to the end of the queue, so Append */
      cmLListAdd2Tail(cp, &pfsUe->prioLnk);
   }
   else
   {
      cp->crnt = node;
      cmLListInsCrnt(cp, &pfsUe->prioLnk);
   }

   pfsUe->lstCp = cp;
   RETVOID;
}

/**
 * @brief Contention Resolution allocation request
 *
 * @details
 *
 *     Function : rgSCHPfsUlContResReq
 *
 *     Processing Steps:
 *     - Add UE to cell wide contention resolution
 *       queue (contResLst) if not already present
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsUlContResReq
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else
PUBLIC Void rgSCHPfsUlContResReq(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif
{
   RgSchPfsUlCell      *pfsCell = RG_SCH_PFS_GET_PFSCELLUL(cell);
   RgSchPfsUlUe        *pfsUe = RG_SCH_PFS_GET_PFSUEUL(ue);
   TRC2(rgSCHPfsUlContResReq);

   if (pfsUe->lstCp == &pfsCell->txQueues.contResLst)
   {
      RETVOID;
   }
   rgSCHPfsUlRmvUeFrmQ(pfsCell, pfsUe);
   rgSCHPfsUlAddUeToQ(&pfsCell->txQueues.contResLst, ue);
   RETVOID;
}


/**
 * @brief Invoked on final RB allocation for a UL subframe
 *
 * @details
 *
 *     Function : rgSCHPfsUlOnAlloc
 *
 *     Processing Steps:
 *     - For each UE allocation for new transmission
 *       - Remove UE from contention resolution queue if present
 *       - For each other UE, call the rgSCHPfsUlMngUeInQ()
 *         function to keep the UEs updated in correct queues.
 *     - Need not do anything about unallocated new transmissions.
 *     - Need not do anything about allocated or unallocated
 *       retransmissions.
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo *alloc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPfsUlOnAlloc
(
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *alloc
)
#else
PUBLIC Void rgSCHPfsUlOnAlloc(cell, alloc)
RgSchCellCb           *cell;
RgSchCmnUlRbAllocInfo *alloc;
#endif
{
   RgSchPfsUlCell      *pfsCell = RG_SCH_PFS_GET_PFSCELLUL(cell);
   CmLListCp           *cp;
   CmLList             *lnk;
#ifdef LTEMAC_DRX
   CmLListCp         ulInactvLst; /* list of UE's becoming UL-inactive */
#endif

#ifdef LTEMAC_DRX
   cmLListInit(&ulInactvLst);
#endif
   TRC2(rgSCHPfsUlOnAlloc);

   /* Go through UE's scheduled for contention resolution and remove
    * them from contention resolution queue */
   for (cp = &alloc->schdContResLst, lnk = cp->first; lnk; )
   {
      RgSchUeCb           *ue = (RgSchUeCb *)lnk->node;
      RgSchPfsUlUe        *pfsUe = RG_SCH_PFS_GET_PFSUEUL(ue);
#ifdef LTEMAC_HDFDD
      if (RG_SCH_HDFDD_UE_ENBLD(ue))
      {
         rgSCHCmnHdFddUpdULMark (cell,ue);
      }
#endif
      lnk = lnk->next;
#ifdef LTEMAC_DRX
      rgSCHPfsUlUeSfResetTemp(cell, ue, &ulInactvLst);
#else
      rgSCHPfsUlUeSfResetTemp(cell, ue);
#endif
      rgSCHPfsUlRmvUeFrmQ(pfsCell, pfsUe);
   }

   /* Go through other scheduled UEs and update */
   for (cp = &alloc->schdUeLst, lnk = cp->first; lnk; )
   {
      RgSchUeCb           *ue = (RgSchUeCb *)lnk->node;
      /*rg002.301_LPR2-ADD-Added the definetion of pfsUe.*/
      RgSchPfsUlUe        *pfsUe = RG_SCH_PFS_GET_PFSUEUL(ue);
      RgSchCmnUlUe        *ueUl  = RG_SCH_CMN_GET_UL_UE(ue);
      U32                 allocd;
#ifdef LTEMAC_HDFDD
      if (RG_SCH_HDFDD_UE_ENBLD(ue))
      {
         rgSCHCmnHdFddUpdULMark (cell,ue);
      }
#endif
      lnk = lnk->next;
      allocd = ueUl->alloc.allocdBytes;
#ifdef LTEMAC_DRX
      rgSCHPfsUlUeSfResetTemp(cell, ue, &ulInactvLst);
#else
      rgSCHPfsUlUeSfResetTemp(cell, ue);
#endif
      /*rg002.301_LPR2-ADD-Added the handling to set this
       * variable before the UE gets repositioned in the Q.*/
      pfsUe->schedInCrntUlSf = TRUE;
      rgSCHPfsUlMngUeInQ(cell, ue, allocd);
      pfsUe->schedInCrntUlSf = FALSE;
   }

   /* Go through non-scheduled UEs and reset ul alloc cb. Right
    * now, don't see a need to reset anything except reqLnk.node. */
   for (cp = &alloc->nonSchdContResLst, lnk = cp->first; lnk; )
   {
      RgSchUeCb           *ue = (RgSchUeCb *)lnk->node;
      lnk = lnk->next;
#ifdef LTEMAC_DRX
      rgSCHPfsUlUeSfResetTemp(cell, ue, &ulInactvLst);
#else
      rgSCHPfsUlUeSfResetTemp(cell, ue);
#endif
   }
   for (cp = &alloc->nonSchdUeLst, lnk = cp->first; lnk; )
   {
      RgSchUeCb           *ue = (RgSchUeCb *)lnk->node;
      lnk = lnk->next;
#ifdef LTEMAC_DRX
      rgSCHPfsUlUeSfResetTemp(cell, ue, &ulInactvLst);
#else
      rgSCHPfsUlUeSfResetTemp(cell, ue);
#endif
   }

#ifdef LTEMAC_DRX
      /*PDCCH for SR scheduled, now inactivate the UE if not active for other
       * reasons.*/
   rgSCHPfsUlInactvLstInd(cell, &ulInactvLst);
#endif

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPfsUlUeSfResetTemp
 *
 *     Desc : Reset whatever variables where temporarily used
 *            during UE scheduling.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef LTEMAC_DRX
#ifdef ANSI
PRIVATE Void rgSCHPfsUlUeSfResetTemp
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
CmLListCp               *ulInactvLst /* list of UE's becoming UL-inactive */
)
#else
PRIVATE Void rgSCHPfsUlUeSfResetTemp(cell, ue, ulInactvLst)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
CmLListCp               *ulInactvLst; /* list of UE's becoming UL-inactive */
#endif
#else
#ifdef ANSI
PRIVATE Void rgSCHPfsUlUeSfResetTemp
(
RgSchCellCb             *cell,
RgSchUeCb               *ue
)
#else
PRIVATE Void rgSCHPfsUlUeSfResetTemp(cell, ue)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
#endif
#endif
{
   RgSchPfsUlUe        *pfsUe = RG_SCH_PFS_GET_PFSUEUL(ue);
#ifdef LTEMAC_DRX
   RgSchDrxUeCb      *drxUe    = NULLP;
#endif
   UNUSED(cell);

   TRC2(rgSCHPfsUlUeSfResetTemp);

   /*cmnUe->alloc.reqLnk.node = NULLP;*/
   rgSCHCmnUlUeResetTemp(ue);
#ifdef LTEMAC_DRX
    if (ue->isDrxEnabled)
    {
      if(pfsUe->srRcvd == TRUE)
      {
         drxUe = RG_SCH_DRX_GET_UE(ue);
         drxUe->drxUlInactvMask  |= RG_SCH_DRX_SR_BITMASK;

         if(!RG_SCH_DRX_UL_IS_UE_ACTIVE(drxUe))
         {
            ue->ul.ulInactvMask |= RG_DRX_INACTIVE;
            /* Add to Ul inactive List */
            ue->ulDrxInactvLnk.node  = (PTR)ue;
            cmLListAdd2Tail(ulInactvLst,&(ue->ulDrxInactvLnk));
         }
         drxUe->srRcvd = FALSE;
      }
    }
#endif
   pfsUe->srRcvd = FALSE;
   RETVOID;
}  /* rgSCHPfsUlUeSfResetTemp */

/***********************************************************
 *
 *     Func : rgSCHPfsUlGetCqiPrio
 *        
 *     Desc : Obtains CQI PFS priority from CQI value
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U32 rgSCHPfsUlGetCqiPrio
(
RgSchPfsUlCell      *pfsCell,
U8                   cqi
)
#else
PRIVATE U32 rgSCHPfsUlGetCqiPrio(pfsCell, cqi)
RgSchPfsUlCell      *pfsCell;
U8                   cqi;
#endif
{
   TRC2(rgSCHPfsUlGetCqiPrio);

   /* MS_REMOVE : syed ERROR CHECK */
   if (cqi > 15)
   {
#ifdef MSPD
      stop_printf("ERROR!! UL CQI(%d) out or range\n", cqi);	   
#endif
   }
   /* MS_FIX : cqiPrios size is now set to 15, with indexing
    * from 0 to 14. DL CQI was already handled correctly. */
   RETVALUE(pfsCell->tbls.cqiPrios[cqi-1]);
}

/***********************************************************
 *
 *     Func : rgSCHPfsUlGetFracPrio
 *        
 *     Desc : Obtains LCG PFS priority from 'Remaining
 *            fraction' value
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U32 rgSCHPfsUlGetFracPrio
(
RgSchPfsUlCell      *pfsCell,
U8                   frac
)
#else
PRIVATE U32 rgSCHPfsUlGetFracPrio(pfsCell, frac)
RgSchPfsUlCell      *pfsCell;
U8                   frac;
#endif
{
   TRC2(rgSCHPfsUlGetFracPrio);

   RETVALUE(pfsCell->tbls.fracPrios[frac]);
}

/***********************************************************
 *
 *     Func : rgSCHPfsUlGetLcgPrio
 *        
 *     Desc : Obtains LCG PFS priority from LCG priority
 *            table.
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U32 rgSCHPfsUlGetLcgPrio
(
RgSchPfsUlCell      *pfsCell,
U8                   lcgId
)
#else
PRIVATE U32 rgSCHPfsUlGetLcgPrio(pfsCell, lcgId)
RgSchPfsUlCell      *pfsCell;
U8                   lcgId;
#endif
{
   TRC2(rgSCHPfsUlGetLcgPrio);

   RETVALUE(pfsCell->tbls.lcgPrios[lcgId-1]);
}

/***********************************************************
 *
 *     Func : rgSCHPfsUlGetQId
 *        
 *     Desc : For uplink,obtains queue ID for dataLst
 *            given a PFS priority
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U32 rgSCHPfsUlGetQId
(
RgSchPfsUlCell      *pfsCell,
U32                  pfsPrio
)
#else
PRIVATE U32 rgSCHPfsUlGetQId(pfsCell, pfsPrio)
RgSchPfsUlCell      *pfsCell;
U32                  pfsPrio;
#endif
{
   U32 qId;	
   TRC2(rgSCHPfsUlGetQId);

#if (ERRCLASS & ERRCLS_DEBUG)
   if ((pfsPrio < pfsCell->range.min)
         || (pfsPrio > pfsCell->range.max))
   {
      RETVALUE(0);
   }
#endif
   /* MS_FIX : syed For small range and large number of Queues
    * precalculated numElemPerQ can be highly inaccurate. It
    * was leading to a crash.  */
   qId = ((pfsPrio - pfsCell->range.min) * pfsCell->range.numQs) / 
	 pfsCell->range.maxDiff;
#ifdef MSPD
   if (qId > (pfsCell->range.numQs-1))
   {
      MSPD_ERR("<PFSPRIO> qId(%d) > maxQId(%d) for pfsPrio(%d) min(%d) maxDiff(%d)", qId, (pfsCell->range.numQs-1), pfsPrio, pfsCell->range.min, pfsCell->range.maxDiff);
   }
#endif   
   RETVALUE(qId);
}

/**** COMMON functions (specific neither to uplink, nor downlink) ****/


/***********************************************************
 *
 *     Func : rgSCHPfsFillTbl
 *
 *     Desc : Used to fill various PFS tables during
 *            configuration. Also determines minimum
 *            and maximum values in the passed table.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsFillTbl
(
U32                  *src,
U32                  *dst,
U16                   numEntries,
U32                  *minVal,
U32                  *maxVal
)
#else
PRIVATE Void rgSCHPfsFillTbl(src, dst, numEntries, minVal, maxVal)
U32                  *src;
U32                  *dst;
U16                   numEntries;
U32                  *minVal;
U32                  *maxVal;
#endif
{
   U32          val;
   U32          min;
   U32          max;
   U16          idx;
   TRC2(rgSCHPfsFillTbl);

   val = src[0];
   min = val;
   max = val;
   dst[0] = val;
   for (idx = 1; idx < numEntries; ++idx)
   {
      val = src[idx];
      if (val < min)
      {
         min = src[idx];
      }
      if (val > max)
      {
         max = src[idx];
      }
      dst[idx] = val;
   }
   *minVal = min;
   *maxVal = max;

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPfsFillRange
 *
 *     Desc : Fill RgSchPfsPrioRange information
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPfsFillRange
(
RgSchPfsPrioRange    *range,
U32                   minPrios[3],
U32                   maxPrios[3],
U8                    numQs
)
#else
PRIVATE Void rgSCHPfsFillRange(range, minPrios, maxPrios, numQs)
RgSchPfsPrioRange    *range;
U32                   minPrios[3];
U32                   maxPrios[3];
U8                    numQs;
#endif
{
   U32                 minPfsPrio;
   U32                 maxPfsPrio;
   TRC2(rgSCHPfsFillRange);

   minPfsPrio = minPrios[0] * minPrios[1] * minPrios[2];
   maxPfsPrio = maxPrios[0] * maxPrios[1] * maxPrios[2];
   range->min = minPfsPrio;
#if (ERRCLASS & ERRCLS_DEBUG)
   range->max = maxPfsPrio;
#endif
   /* MS_FIX : syed For small range and large number of Queues
    * precalculated numElemPerQ can be highly inaccurate. It
    * was leading to a crash.  */
   range->maxDiff = maxPfsPrio + 1 - minPfsPrio;
   range->numQs = numQs;


   RETVOID;
}

/**
 * @brief Initialize Fraction Priority related Info 
 *
 * @details
 *
 *     Function : rgSCHPfsInitFracPrioInfo 
 *
 *     Processing Steps:
 *     - Initialize Fraction Priority related Info.
 *
 *  @param[in]  RgSchPfsFracPrioInfo   *fracPrioInfo,
 *  @param[in]  U32                    cfgdRate
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsInitFracPrioInfo 
(
RgSchPfsFracPrioInfo   *fracPrioInfo,
U32                    cfgdRate
)
#else
PRIVATE Void rgSCHPfsInitFracPrioInfo(fracPrioInfo, cfgdRate)
RgSchPfsFracPrioInfo   *fracPrioInfo;
U32                    cfgdRate;
#endif
{
   TRC2(rgSCHPfsInitFracPrioInfo);

   fracPrioInfo->achvdFracDataRate = 0;
   fracPrioInfo->fracPrioLvls = 0;
   fracPrioInfo->fracDataRate = RGSCH_CEIL(cfgdRate, RGR_PFS_FRAC_PRIO_LVLS);

   RETVOID;
}

/**
 * @brief Updates Fraction Priority related Info for every allocation
 *
 * @details
 *
 *     Function : rgSCHPfsUpdFracPrioInfo 
 *
 *     Processing Steps:
 *     - Updates Fraction Priority related Info for every allocation.
 *
 *  @param[in]  RgSchPfsFracPrioInfo   *fracPrioInfo,
 *  @param[in]  U32                    allocd 
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsUpdFracPrioInfo 
(
RgSchPfsFracPrioInfo   *fracPrioInfo,
U32                    allocd 
)
#else
PRIVATE Void rgSCHPfsUpdFracPrioInfo(fracPrioInfo, allocd)
RgSchPfsFracPrioInfo   *fracPrioInfo;
U32                    allocd;
#endif
{
   TRC2(rgSCHPfsUpdFracPrioInfo);

   if ((allocd == 0) || (fracPrioInfo->fracDataRate == 0))
   {
      RETVOID;	   
   }

   fracPrioInfo->fracPrioLvls += (fracPrioInfo->achvdFracDataRate +
		                  allocd) / fracPrioInfo->fracDataRate;
   fracPrioInfo->achvdFracDataRate = (fracPrioInfo->achvdFracDataRate +
		                  allocd) % fracPrioInfo->fracDataRate;
   if (fracPrioInfo->fracPrioLvls >= RGR_PFS_FRAC_PRIO_LVLS)
   {
      MSPD_ERR("<PFSPRIO> fracDataRate=%d allocd=%d achvdFracDataRate=%d\n", 
                fracPrioInfo->fracDataRate, allocd, fracPrioInfo->achvdFracDataRate);
   }
   RETVOID;

}

/**
 * @brief Compute and update UL PFS Priority 
 *
 * @details
 *
 *     Function : rgSCHPfsUlUpdPfsPrio 
 *
 *     Processing Steps:
 *     - Compute and update UL PFS Priority.
 *
 *  @param[in]  RgSchPfsUlCell        *pfsCell,
 *  @param[in]  RgSchPfsUlUe          *pfsUe,
 *  @param[out] Bool                  *prioChngd 
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsUlUpdPfsPrio 
(
RgSchPfsUlCell        *pfsCell,
RgSchPfsUlUe          *pfsUe,
Bool                  *prioChngd
)
#else
PRIVATE Void rgSCHPfsUlUpdPfsPrio(pfsCell, pfsUe, prioChngd)
RgSchPfsUlCell        *pfsCell;	
RgSchPfsUlUe          *pfsUe;
Bool                  *prioChngd;
#endif
{
   U32 prevPfsPrio = pfsUe->pfsPrio;	
   TRC2(rgSCHPfsUlUpdPfsPrio);

   pfsUe->pfsPrio = pfsCell->initPrioVal - 
	  (pfsUe->lcgPrio * pfsUe->cqiPrio * 
	   pfsCell->tbls.fracPrios[pfsUe->fracPrioInfo.fracPrioLvls]);
   if (prevPfsPrio != pfsUe->pfsPrio)
   {
      *prioChngd = TRUE;	   
   }
   if (pfsUe->pfsPrio > pfsCell->initPrioVal)
   {
      MSPD_ERR("<PFSPRIO> UL MAX=%d CQI=%d LCG=%d FRAC=%d PFSPRIO = %d\n", 
		   pfsCell->initPrioVal, pfsUe->cqiPrio, pfsUe->lcgPrio, 
		   pfsCell->tbls.fracPrios[pfsUe->fracPrioInfo.fracPrioLvls], 
		   pfsUe->pfsPrio);
   }
   RETVOID;
}

/**
 * @brief Compute and update DL PFS Priority 
 *
 * @details
 *
 *     Function : rgSCHPfsDlUpdPfsPrio 
 *
 *     Processing Steps:
 *     - Compute and update DL PFS Priority.
 *
 *  @param[in]  RgSchPfsDlCell        *pfsCell,
 *  @param[in]  RgSchPfsDlLc          *pfsLc,
 *  @param[in]  RgSchPfsFracPrioInfo  *fracPrioInfo
 *  @param[out] Bool                  *prioChngd
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPfsDlUpdPfsPrio 
(
RgSchPfsDlCell        *pfsCell,
RgSchPfsDlLc          *pfsLc,
RgSchPfsFracPrioInfo  *fracPrioInfo,
Bool                  *prioChngd
)
#else
PRIVATE Void rgSCHPfsDlUpdPfsPrio(pfsCell, pfsLc, fracPrioInfo, prioChngd)
RgSchPfsDlCell        *pfsCell;
RgSchPfsDlLc          *pfsLc;
RgSchPfsFracPrioInfo  *fracPrioInfo;
Bool                  *prioChngd;
#endif
{
   U32 prevPfsPrio = pfsLc->pfsPrio;	
   TRC2(rgSCHPfsDlUpdPfsPrio);

   pfsLc->pfsPrio = pfsCell->initPrioVal - 
	  (pfsLc->svccqiPrio * pfsCell->tbls.fracPrios[fracPrioInfo->fracPrioLvls]);
   if (prevPfsPrio != pfsLc->pfsPrio)
   {
      *prioChngd = TRUE;	   
   }
   if (pfsLc->pfsPrio > pfsCell->initPrioVal)
   {
      MSPD_ERR("<PFSPRIO> DL MAX=%d SVCCQI=%d FRAC=%d PFSPRIO = %d\n", 
		   pfsCell->initPrioVal, pfsLc->svccqiPrio, 
		   pfsCell->tbls.fracPrios[fracPrioInfo->fracPrioLvls], 
		   pfsLc->pfsPrio);
   }
   RETVOID;
}


/**********************************************************************
 
         End of file:     gk_sch_pfs.c@@/main/2 - Sat Jul 30 02:21:46 2011
 
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
/main/1      ---     apany   1. LTE MAC 2.1 release
           rg003.201 ns      1. Trace added to functions.
           rg004.201 ms      1. DL DTX Handling
           rg006.201 sd      1. ccpu00112398: Added periodic BSR timer
           rg007.201   ap  1. Added support for MIMO
           rg008.201   sd  1. Removed dependncy on MIMO compile-time flag
           rg009.201   dv  1. Added changes of TFU_UPGRADE. 
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
           rg001.301  nudupi 1. ccpu00118350:Correcting NDI manipulation of Harq.
*********************************************************************91*/
