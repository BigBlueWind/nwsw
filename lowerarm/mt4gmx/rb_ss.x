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

     Name:     RBSS -- implementation specific definitions

     Type:     C include file

     Desc:     Data structure definitions demanded by systems services.
               The contents of these are for the RBSS implementation.

     File:     rb_ss.x

     Sid:      rb_ss.x@@/main/1 - Mon Aug 29 22:31:38 2011

     Prg:      kp

*********************************************************************21*/

#ifndef __RBSSX__
#define __RBSSX__

#ifdef SS_4GMX_LCORE
/* TAPA task entry doesn't need anything extra for RBSS */
typedef S8              SsdTTskEntry;

/* System task entry */
typedef struct ssdSTskEntry
{
   Ent                  ent;
   Inst                 inst;
   SLockId              lock;
} SsdSTskEntry;

#ifdef SS_DRVR_SUPPORT
/* Driver task entry */
typedef struct ssdDrvrTskEntry
{
   Bool flag;
} SsdDrvrTskEntry;
#endif

/* timer entry--RBSS uses common timers */
typedef struct ssdTmrEntry
{
   CmTimer              timers[TMR_DEF_MAX];
   U32                  ticksToExpire;
} SsdTmrEntry;

/* dynamic pool doesn't need anything extra for RBSS */
typedef S8              SsdDPoolEntry;

/* static pool -- information for the memory management scheme */
typedef S8              SsdSPoolEntry;

/* generic pool doesn't need anything extra for RBSS */
typedef S8              SsdPoolEntry;

/* region doesn't need anything extra for RBSS */
typedef S8              SsdRegionEntry;

/* system services control point--tick count, timer implementation,
 *   console implementation, IS task implementation
 */
typedef struct ssdOs
{
   unsigned     randSeed;               /* random number generator seed */

   Ticks        sysTicks;               /* elapsed system ticks */

#ifndef SS_TTI_TMR
   HANDLE       tmrThrdID[MAX_NUM_CORES];         /* timer thread */
   HANDLE       tmrEvntID[MAX_NUM_CORES];         /* timer Event */
   HANDLE       tmrID[MAX_NUM_CORES];             /* timer ID  */
#endif

#ifndef RB_CHANNEL_SVSR
   HANDLE       QThrdID[MAX_NUM_CORES][MAX_NUM_CORES];           /* Inter Core Queue thread */
   HANDLE       QEvntID[MAX_NUM_CORES][MAX_NUM_CORES];           /* Inter Core Queue Event  */
   HANDLE       QHndlID[MAX_NUM_CORES][MAX_NUM_CORES];               /* Inter Core Queue ID */

   void         *pMem[MAX_NUM_CORES][MAX_NUM_CORES];

   HANDLE       TmrQThrdID[MAX_NUM_CORES];           /* Inter Core Queue thread */
   HANDLE       TmrQEvntID[MAX_NUM_CORES];           /* Inter Core Queue Event  */
   HANDLE       TmrQHndlID[MAX_NUM_CORES];           /* Inter Core Queue ID */

   void         *pMemTmr[MAX_NUM_CORES];
#else
   HANDLE       ICoreMsgPart;
   U32          pICoreCtx[MAX_NUM_CORES];
   U32          pTmrCtx[MAX_NUM_CORES];
#endif

   SsAppCtx     pICpuCtx[MAX_NUM_CORES];

#ifndef RB_TICK_TIMER
   CmTqCp       tmrTqCp;                /* common timer control point */
   CmTqType     tmrTq[SS_MAX_TMRS];     /* common timer queue */
#endif

   int          ssStarted;              /* posted when SS completes init */
#ifdef CONAVL
   FILE         *conInFp;               /* console input file pointer */
   FILE         *conOutFp;              /* console output file pointer */
#endif

#ifndef NOFILESYS
   FILE         *fileOutFp;             /* output file pointer */
#endif

   SsMlogCb     mLog[MLOG_MAX_CORES]; /* One MLOG CB per core */
} SsdOs;

typedef struct rbBktCfg
{
   Size         blkSize;                /* bucket quantum size */
   U32          numBlks;                /* the total blocks in the bucket */
} RbBktCfg;

typedef struct rbRegCfg
{
   Region       regionId;
   U16          numBkts;
   Size         heapsize;
   RbBktCfg     bkt[SS_MAX_POOLS_PER_REG];
} RbRegCfg;

typedef struct rbMemCfg
{
   U8           numRegions;
   RbRegCfg     region[SS_MAX_REGS];
} RbMemCfg;

EXTERN Void rbLarmStart ARGS((void));
EXTERN Void rbLarmStop ARGS((void));
EXTERN S16 rbICorePstTsk ARGS((Pst *pst, Buffer *mBuf, void *tsk));
#ifndef RB_SINGLE_CORE
EXTERN S32 rb_lock_init ARGS((SLockId *lock));
EXTERN S32 rb_lock_deinit ARGS((SLockId *lock));
EXTERN S32 rb_lock ARGS((SLockId *lock));
EXTERN S32 rb_unlock ARGS((SLockId *lock));
#endif
#endif  /*SS_4GMX_LCORE*/

#endif  /*  __RBSSX__  */


/********************************************************************30**

         End of file:     rb_ss.x@@/main/1 - Mon Aug 29 22:31:38 2011

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

   ver        pat        init                  description
---------  ---------  ----------  -------------------------------------
/main/1      --------     ------    1. Initial release
*********************************************************************91*/
