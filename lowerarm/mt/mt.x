/********************************************************************16**

        (c) COPYRIGHT 1989-1997 by Trillium Digital Systems, Inc.
                          All rights reserved.

     This software is confidential and proprietary to Trillium
     Digital Systems, Inc.  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement
     between Trillium and its licensee.

     Trillium warrants that for a period, as provided by the written
     license agreement between Trillium and its licensee, this
     software will perform substantially to Trillium specifications as
     published at the time of shipment and the media used for delivery
     of this software will be free from defects in materials and
     workmanship.

     TRILLIUM MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL TRILLIUM BE LIABLE FOR ANY INDIRECT, SPECIAL,
     OR CONSEQUENTIAL DAMAGES IN CONNECTION WITH OR ARISING OUT OF
     THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
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
     States.  The Government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.Œ.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the Use set
     forth in the written License Agreement between Trillium and
     its Licensee. Among other things, the Use of this software
     may be limited to a particular type of Designated Equipment.
     Before any installation, use or transfer of this software, please
     consult the written License Agreement or contact Trillium at
     the location set forth below in order to confirm that you are
     engaging in a permissible Use of the software.

                    Trillium Digital Systems, Inc.
                  12100 Wilshire Boulevard, suite 1800
                    Los Angeles, CA 90025-7118, USA

                        Tel: +1 (310) 442-9222
                        Fax: +1 (310) 442-1162

                   Email: tech_support@trillium.com
                     Web: http://www.trillium.com

*********************************************************************17*/


/********************************************************************20**
  
     Name:     multi-threaded
  
     Type:     C include file
  
     Desc:     Structures, variables and typedefs required by the
               multi-threaded
  
     File:     mt.x
  
     Sid:      mt.x@@/main/1 - Mon Apr  5 18:19:16 2010

     Prg:      ada

*********************************************************************21*/
  
#ifndef __MTX__
#define __MTX__


/* typedefs */

/*
 * dynamic pool
 */
typedef struct mtDPool
{
   MtMtxId  mtx;                /* mutex for this pool */
   Queue    bufQ;               /* Queue of messages */
   S16      size;               /* Pool size - absolute value */
   U16      status;             /* Status indicator */
} MtDPool;

/*
 * static pool
 */
typedef struct mtSPool
{
   MtMtxId mtx;                 /* mutext */
   Size  memResv;               /* memory reservation (SGetSMem) */
   Data *memStrt;               /* memory start */
   Data *memEnd;                /* memory end */
   SBuf *avlStrt;               /* available start */
   Data *avlEnd;                /* available end */
   Size crntSize;               /* current size */
   Size bufSize;                /* buffer size */
   Size minSize;                /* minimum size */
   Size maxSize;                /* maximum size */
} MtSPool;

typedef struct mtTmrCp
{
   CmTqCp   tmrTqCp;
   CmTqType tmrTq[MT_TQNUMENT];
   MtMtxId  mtx;            /* mutex id */
   MtCondId cond;           /* condition id  */
   MtThrdId tmrThrd;        /* interrupt handler thread (tmr interrupts) */
}MtTmrCp;

/*
 * each pool of dBufs has the following structure:
 */
typedef struct mtDq             /* demand queue */
{
   MtMtxId  mtx;                /* mutex for this queue */
   MtCondId cond;               /* condition variable */
   Queue    bufQ;               /* Queue of events (messages) */
} MtDq;

typedef struct mtTsk
{
   Ent        ent;              /* Entity */
   Inst       inst;             /* Instance */
   ActvTsk    actvTsk;          /* activation function */
}MtTsk;

typedef struct mtDrvr
{
   Inst       inst;             /* instance */
   ProcId     low;              /* low processor id */
   ProcId     high;             /* high processor id */
   ActvTsk    actvTsk;          /* activation function */
}MtDrvr;

typedef struct mtDrvrEntry
{
   U32        index;              /* task index */
   MtDq       dq;                 /* demand queue */
   MtThrdId   tId;                /* task id */
   U32        count;              /* activity counter */
   Inst       inst;             /* instance */
   ProcId     low;              /* low processor id */
   ProcId     high;             /* high processor id */
   ActvTsk    actvTsk;          /* activation function */
} MtDrvrEntry;


typedef struct mtTskEntry
{
   U32        index;              /* task index */
   U32        type;               /* task type */
   MtDq       dq;                 /* demand queue */
   MtThrdId   tId;                /* task id */
   U32        count;              /* activity counter */
   U32        tskCnt;             /* # of tasks supported by this thread */
   MtTsk      tsks[MT_MAX_TSKCNT];/* maximum tasks */
} MtTskEntry;

typedef struct mtTmrPlace
{
   MtDq       dq;                 /* demand queue pointer */
   MtThrdId   tId;                /* task id */
} MtTmrPlace;   
 
typedef struct mtTmrEntry
{
   S32        timerId;          /* timer id */
   Ent        ent;              /* timer pst */
   Inst       inst;             /* instance */
   PFS16      tmrFnct;          /* timer function */
   MtDq       *dq;              /* demand queue pointer */
   U16        delay;            /* delay - timer period */
   U32        sent;             /* # of msgs sent */
   U32        drop;             /* # of time-outs dropped */
   U32        received;         /* # of msgs received */
   CmTimer    timers[TMR_DEF_MAX]; /* timers */
} MtTmrEntry;

typedef struct mtOsCp
{
   MtDPool dPool[MT_NMB_MSG_POOL];    /* dynamic pools */
   MtSPool sPool;                     /* statis pool */
   MtMtxId tskTblMtx;                 /* task table mutex */
   MtTskEntry *tskTbl[MT_MAX_TASK];   /* task table */
   MtMtxId drvrTblMtx;                /* driver table mutex */
   MtDrvrEntry *drvrTbl[MT_MAX_DRVR]; /* driver table */
   MtMtxId tmrTblMtx;                 /* timer table mutex */
   MtTmrEntry *tmrTbl[MT_MAX_TIMER];  /* timer table */
   MtTmrCp tmrCp;                     /* control point for timers */
   MtTmrPlace tmrPlace;               /* timer placement on expiry */
   MtThrdId intHndlrThrd;             /* thread to handle all interrupts */
   U32 tskTblIdx;                     /* task table index */
   U32 drvrTblIdx;                    /* driver table index */
   U32 tmrTblIdx;                     /* timer table index */
   ProcId ownProcId;                  /* own proc id */
   Bool permTask;                     /* permanent task threads */
   Ticks sysTicks;                    /* system ticks */
   MtThrdId rdConThrdId;              /* read console task */
} MtOsCp;                             /* operating system control point */


typedef struct mtPoolInfo
{
   MsgLen strtIdx;              /* start index */
   MsgLen endIdx;
   S32 dSize;
   S32 nmbBufs;
}MtPoolInfo;



/* partition */

/* message */

/* queue */

/* tasks */


#endif

  
/********************************************************************30**

         End of file:     mt.x@@/main/1 - Mon Apr  5 18:19:16 2010

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

  version    pat  init                   description
----------- ----- ----  ------------------------------------------------
1.1          ---  ada   1. initial release 

*********************************************************************81*/

