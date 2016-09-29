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
  
     Desc:     C source code for tables lookup  
     File:     ys_db.c
  
     Sid:      yw_db.c@@/main/1 - Thu Dec  2 02:27:25 2010
  
     Prg:      pk
  
**********************************************************************/

/** @file ys_db.c
@brief This module acts as an interface handler for upper interface and 
manages Pst and Sap related information for upper interface APIs.
*/

/* header include files -- defines (.h) */

/* header/extern include files (.x) */
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
#include "LtePhyL2Api.h"
#include "ys_ms.h"            /* defines and macros for CL */
#else
#include "ys.h"            /* defines and macros for CL */
#endif /* YS_MSPD */
#include "ys_ms_err.h"        /* YS error defines */

#ifdef TENB_AS_SECURITY
#include "spaccdrv.h" /*sumanth*/
#endif

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
#include "ys_ms.x"            /* typedefs for CL */
#else
#include "ys.x"            /* typedefs for CL */
#endif /* YS_MSPD */




PUBLIC Bool               ysPrachPresDb[YS_NUM_PRACH_PRES_ARR];
PUBLIC YsSrCfgInfo        ysSrCfgDb[YS_NUM_SR_CFG];
PUBLIC YsSrsCfgInfo       ysSrsCfgDb[YS_NUM_SRS_CFG];
PUBLIC YsCqiPmiCfgIdxInfo ysCqiPmiCfgDb[YS_NUM_CQI_CFG];

#ifdef YS_MSPD
PUBLIC YsPrachFddCfgInfo ysPrachFddCfgDb[YS_NUM_PRACH_FDD_CFG] = 
{
   { /* PRACH Cfg Index 0 */
      0,
      YS_PREAM_FRMT_0,
      YS_EVEN_SFN,
      1,
      {
         1
      }
   },
   { /* PRACH Cfg Index 1 */
      1,
      YS_PREAM_FRMT_0,
      YS_EVEN_SFN,
      1,
      {
         4
      }
   },
   { /* PRACH Cfg Index 2 */
      2,
      YS_PREAM_FRMT_0,
      YS_EVEN_SFN,
      1,
      {
         7
      }
   },
   { /* PRACH Cfg Index 3 */
      3,
      YS_PREAM_FRMT_0,
      YS_ALL_SFN,
      1,
      {
         1
      }
   },
   { /* PRACH Cfg Index 4 */
      4,
      YS_PREAM_FRMT_0,
      YS_ALL_SFN,
      1,
      {
         4
      }
   },
   { /* PRACH Cfg Index 5 */
      5,
      YS_PREAM_FRMT_0,
      YS_ALL_SFN,
      1,
      {
         7
      }
   },
   { /* PRACH Cfg Index 6 */
      6,
      YS_PREAM_FRMT_0,
      YS_ALL_SFN,
      2,
      {
         1, 6
      }
   },
   { /* PRACH Cfg Index 7 */
      7,
      YS_PREAM_FRMT_0,
      YS_ALL_SFN,
      2,
      {
         2, 7
      }
   },
   { /* PRACH Cfg Index 8 */
      8,
      YS_PREAM_FRMT_0,
      YS_ALL_SFN,
      2,
      {
         3, 8
      }
   },
   { /* PRACH Cfg Index 9 */
      9,
      YS_PREAM_FRMT_0,
      YS_ALL_SFN,
      3,
      {
         1, 4, 7
      }
   },
   { /* PRACH Cfg Index 10 */
      10,
      YS_PREAM_FRMT_0,
      YS_ALL_SFN,
      3,
      {
         2, 5, 8
      }
   },
   { /* PRACH Cfg Index 11 */
      11,
      YS_PREAM_FRMT_0,
      YS_ALL_SFN,
      3,
      {
         3, 6, 9
      }
   },
   { /* PRACH Cfg Index 12 */
      12,
      YS_PREAM_FRMT_0,
      YS_ALL_SFN,
      5,
      {
         0, 2, 4, 6, 8
      }
   },
   { /* PRACH Cfg Index 13 */
      13,
      YS_PREAM_FRMT_0,
      YS_ALL_SFN,
      5,
      {
         1, 3, 5, 7, 9
      }
   },
   { /* PRACH Cfg Index 14 */
      14,
      YS_PREAM_FRMT_0,
      YS_ALL_SFN,
      10,
      {
         0, 1, 2, 3, 4, 5, 6, 7, 8, 9
      }
   },
   { /* PRACH Cfg Index 15 */
      15,
      YS_PREAM_FRMT_0,
      YS_EVEN_SFN,
      1,
      {
         9
      }
   },
   { /* PRACH Cfg Index 16 */
      16,
      YS_PREAM_FRMT_1,
      YS_EVEN_SFN,
      1,
      {
         1
      }
   },
   { /* PRACH Cfg Index 17 */
      17,
      YS_PREAM_FRMT_1,
      YS_EVEN_SFN,
      1,
      {
         4
      }
   },
   { /* PRACH Cfg Index 18 */
      18,
      YS_PREAM_FRMT_1,
      YS_EVEN_SFN,
      1,
      {
         7
      }
   },
   { /* PRACH Cfg Index 19 */
      19,
      YS_PREAM_FRMT_1,
      YS_ALL_SFN,
      1,
      {
         1
      }
   },
   { /* PRACH Cfg Index 20 */
      20,
      YS_PREAM_FRMT_1,
      YS_ALL_SFN,
      1,
      {
         4
      }
   },
   { /* PRACH Cfg Index 21 */
      21,
      YS_PREAM_FRMT_1,
      YS_ALL_SFN,
      1,
      {
         7
      }
   },
   { /* PRACH Cfg Index 22 */
      22,
      YS_PREAM_FRMT_1,
      YS_ALL_SFN,
      2,
      {
         1, 6
      }
   },
   { /* PRACH Cfg Index 23 */
      23,
      YS_PREAM_FRMT_1,
      YS_ALL_SFN,
      2,
      {
         2, 7
      }
   },
   { /* PRACH Cfg Index 24 */
      24,
      YS_PREAM_FRMT_1,
      YS_ALL_SFN,
      2,
      {
         3, 8
      }
   },
   { /* PRACH Cfg Index 25 */
      25,
      YS_PREAM_FRMT_1,
      YS_ALL_SFN,
      3,
      {
         1, 4, 7
      }
   },
   { /* PRACH Cfg Index 26 */
      26,
      YS_PREAM_FRMT_1,
      YS_ALL_SFN,
      3,
      {
         2, 5, 8
      }
   },
   { /* PRACH Cfg Index 27 */
      27,
      YS_PREAM_FRMT_1,
      YS_ALL_SFN,
      3,
      {
         3, 6, 9
      }
   },
   { /* PRACH Cfg Index 28 */
      28,
      YS_PREAM_FRMT_1,
      YS_ALL_SFN,
      5,
      {
         0, 2, 4, 6, 8
      }
   },
   { /* PRACH Cfg Index 29 */
      29,
      YS_PREAM_FRMT_1,
      YS_ALL_SFN,
      5,
      {
         1, 3, 5, 7, 9
      }
   },
   { /* PRACH Cfg Index 30 */
      30,
      YS_PREAM_FRMT_NA
   },
   { /* PRACH Cfg Index 31 */
      31,
      YS_PREAM_FRMT_1,
      YS_EVEN_SFN,
      1,
      {
         9
      }
   },
   { /* PRACH Cfg Index 32 */
      32,
      YS_PREAM_FRMT_2,
      YS_EVEN_SFN,
      1,
      {
         1
      }
   },
   { /* PRACH Cfg Index 33 */
      33,
      YS_PREAM_FRMT_2,
      YS_EVEN_SFN,
      1,
      {
         4
      }
   },
   { /* PRACH Cfg Index 34 */
      34,
      YS_PREAM_FRMT_2,
      YS_EVEN_SFN,
      1,
      {
         7
      }
   },
   { /* PRACH Cfg Index 35 */
      35,
      YS_PREAM_FRMT_2,
      YS_ALL_SFN,
      1,
      {
         1
      }
   },
   { /* PRACH Cfg Index 36 */
      36,
      YS_PREAM_FRMT_2,
      YS_ALL_SFN,
      1,
      {
         4
      }
   },
   { /* PRACH Cfg Index 37 */
      37,
      YS_PREAM_FRMT_2,
      YS_ALL_SFN,
      1,
      {
         7
      }
   },
   { /* PRACH Cfg Index 38 */
      38,
      YS_PREAM_FRMT_2,
      YS_ALL_SFN,
      2,
      {
         1, 6
      }
   },
   { /* PRACH Cfg Index 39 */
      39,
      YS_PREAM_FRMT_2,
      YS_ALL_SFN,
      2,
      {
         2, 7
      }
   },
   { /* PRACH Cfg Index 40 */
      40,
      YS_PREAM_FRMT_2,
      YS_ALL_SFN,
      2,
      {
         3, 8
      }
   },
   { /* PRACH Cfg Index 41 */
      41,
      YS_PREAM_FRMT_2,
      YS_ALL_SFN,
      3,
      {
         1, 4, 7
      }
   },
   { /* PRACH Cfg Index 42 */
      42,
      YS_PREAM_FRMT_2,
      YS_ALL_SFN,
      3,
      {
         2, 5, 8
      }
   },
   { /* PRACH Cfg Index 43 */
      43,
      YS_PREAM_FRMT_2,
      YS_ALL_SFN,
      3,
      {
         3, 6, 9
      }
   },
   { /* PRACH Cfg Index 44 */
      44,
      YS_PREAM_FRMT_2,
      YS_ALL_SFN,
      5,
      {
         0, 2, 4, 6, 8
      }
   },
   { /* PRACH Cfg Index 45 */
      45,
      YS_PREAM_FRMT_2,
      YS_ALL_SFN,
      5,
      {
         1, 3, 5, 7, 9
      }
   },
   { /* PRACH Cfg Index 46 */
      46,
      YS_PREAM_FRMT_NA
   },
   { /* PRACH Cfg Index 47 */
      47,
      YS_PREAM_FRMT_2,
      YS_EVEN_SFN,
      1,
      {
         9
      }
   },
   { /* PRACH Cfg Index 48 */
      48,
      YS_PREAM_FRMT_3,
      YS_EVEN_SFN,
      1,
      {
         1
      }
   },
   { /* PRACH Cfg Index 49 */
      49,
      YS_PREAM_FRMT_3,
      YS_EVEN_SFN,
      1,
      {
         4
      }
   },
   { /* PRACH Cfg Index 50 */
      50,
      YS_PREAM_FRMT_3,
      YS_EVEN_SFN,
      1,
      {
         7
      }
   },
   { /* PRACH Cfg Index 51 */
      51,
      YS_PREAM_FRMT_3,
      YS_ALL_SFN,
      1,
      {
         1
      }
   },
   { /* PRACH Cfg Index 52 */
      52,
      YS_PREAM_FRMT_3,
      YS_ALL_SFN,
      1,
      {
         4
      }
   },
   { /* PRACH Cfg Index 53 */
      53,
      YS_PREAM_FRMT_3,
      YS_ALL_SFN,
      1,
      {
         7
      }
   },
   { /* PRACH Cfg Index 54 */
      54,
      YS_PREAM_FRMT_3,
      YS_ALL_SFN,
      2,
      {
         1, 6
      }
   },
   { /* PRACH Cfg Index 55 */
      55,
      YS_PREAM_FRMT_3,
      YS_ALL_SFN,
      2,
      {
         2, 7
      }
   },
   { /* PRACH Cfg Index 56 */
      56,
      YS_PREAM_FRMT_3,
      YS_ALL_SFN,
      2,
      {
         3, 8
      }
   },
   { /* PRACH Cfg Index 57 */
      57,
      YS_PREAM_FRMT_3,
      YS_ALL_SFN,
      3,
      {
         1, 4, 7
      }
   },
   { /* PRACH Cfg Index 58 */
      58,
      YS_PREAM_FRMT_3,
      YS_ALL_SFN,
      3,
      {
         2, 5, 8
      }
   },
   { /* PRACH Cfg Index 59 */
      59,
      YS_PREAM_FRMT_3,
      YS_ALL_SFN,
      3,
      {
         3, 6, 9
      }
   },
   { /* PRACH Cfg Index 60 */
      60,
      YS_PREAM_FRMT_NA
   },
   { /* PRACH Cfg Index 61 */
      61,
      YS_PREAM_FRMT_NA
   },
   { /* PRACH Cfg Index 62 */
      62,
      YS_PREAM_FRMT_NA
   },
   { /* PRACH Cfg Index 63 */
      63,
      YS_PREAM_FRMT_3,
      YS_ALL_SFN,
      1,
      {
         9
      }
   }
};

/* Basis Sequences for (20, A) Code
   References 36-212 Section 5.2.3.3
*/
PUBLIC U8 MSeq[20][13]=
{
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0}, 
  { 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
  { 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1},
  { 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1},
  { 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1},
  { 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1},
  { 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
  { 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1},
  { 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1},
  { 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1},
  { 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1},
  { 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1},
  { 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1},
  { 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
  { 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1},
  { 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1},
  { 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1},
  { 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1},
  { 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
  { 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0}
}; 

/* 16 values 0000 to 1111. And their corresponding
   20 bit values which are pre-computed. 
b(0) = (a(0).M(0, 0) + a(1).M(0, 1) + a(2).M(0, 2) + a(3).M(0, 3))mod 2 */
PUBLIC U8 cqiTo20BitMap[16][2] =
{
   {0, 0},
   {1, 0},
   {2, 0},
   {3, 0},
   {4, 0},
   {5, 0},
   {6, 0},
   {7, 0},
   {8, 0},
   {9, 0},
   {10, 0},
   {11, 0},
   {12, 0},
   {13, 0},
   {14, 0},
   {15, 0}
};
 
/*ys004.102 MSPD Mege for PHY version 1.7*/
PUBLIC Bool               ysPrachPresDb[YS_NUM_PRACH_PRES_ARR];
PUBLIC YsSrCfgInfo        ysSrCfgDb[YS_NUM_SR_CFG];
PUBLIC YsSrsCfgInfo       ysSrsCfgDb[YS_NUM_SRS_CFG];
PUBLIC YsCqiPmiCfgIdxInfo ysCqiPmiCfgDb[YS_NUM_CQI_CFG];

#endif
/********************************************************************30**
  
         End of file:     yw_db.c@@/main/1 - Thu Dec  2 02:27:25 2010
  
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
/main/1      ---     sgm                   1. eNodeB 1.2 release
/main/1      ys004.102    vr               1. MSPD Merge for PHY ver 1.7 
*********************************************************************91*/
