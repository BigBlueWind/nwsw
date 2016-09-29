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

     Name:     multi-threaded

     Type:     C include file

     Desc:     Structures, variables and typedefs required by the
               multi-threaded

     File:     rb.x

     Sid:      rb.x@@/main/1 - Mon Aug 29 22:31:34 2011

     Prg:      ada

*********************************************************************21*/

#ifndef __RBX__
#define __RBX__


/* typedefs */

/*
 * dynamic pool
 */
typedef struct rbDPool
{
   RbMtxId  mtx;                /* mutex for this pool */
   Queue    bufQ;               /* Queue of messages */
   S16      size;               /* Pool size - absolute value */
   U16      status;             /* Status indicator */
} RbDPool;

/*
 * static pool
 */
typedef struct rbSPool
{
   RbMtxId mtx;                 /* mutext */
   Size  memResv;               /* memory reservation (SGetSMem) */
   Data *memStrt;               /* memory start */
   Data *memEnd;                /* memory end */
   SBuf *avlStrt;               /* available start */
   Data *avlEnd;                /* available end */
   Size crntSize;               /* current size */
   Size bufSize;                /* buffer size */
   Size minSize;                /* minimum size */
   Size maxSize;                /* maximum size */
} RbSPool;

typedef struct rbTmrCp
{
   CmTqCp   tmrTqCp;
   CmTqType tmrTq[RB_TQNUMENT];
   RbMtxId  mtx;            /* mutex id */
   RbCondId cond;           /* condition id  */
   RbThrdId tmrThrd;        /* interrupt handler thread (tmr interrupts) */
}RbTmrCp;

/*
 * each pool of dBufs has the following structure:
 */
typedef struct rbDq             /* demand queue */
{
   RbMtxId  mtx;                /* mutex for this queue */
   RbCondId cond;               /* condition variable */
   Queue    bufQ;               /* Queue of events (messages) */
} RbDq;

typedef struct rbTsk
{
   Ent        ent;              /* Entity */
   Inst       inst;             /* Instance */
   ActvTsk    actvTsk;          /* activation function */
}RbTsk;

typedef struct rbDrvr
{
   Inst       inst;             /* instance */
   ProcId     low;              /* low processor id */
   ProcId     high;             /* high processor id */
   ActvTsk    actvTsk;          /* activation function */
}RbDrvr;

typedef struct rbDrvrEntry
{
   U32        index;              /* task index */
   RbDq       dq;                 /* demand queue */
   RbThrdId   tId;                /* task id */
   U32        count;              /* activity counter */
   Inst       inst;             /* instance */
   ProcId     low;              /* low processor id */
   ProcId     high;             /* high processor id */
   ActvTsk    actvTsk;          /* activation function */
} RbDrvrEntry;


typedef struct rbTskEntry
{
   U32        index;              /* task index */
   U32        type;               /* task type */
   RbDq       dq;                 /* demand queue */
   RbThrdId   tId;                /* task id */
   U32        count;              /* activity counter */
   U32        tskCnt;             /* # of tasks supported by this thread */
   RbTsk      tsks[RB_MAX_TSKCNT];/* maximum tasks */
} RbTskEntry;

typedef struct rbTmrPlace
{
   RbDq       dq;                 /* demand queue pointer */
   RbThrdId   tId;                /* task id */
} RbTmrPlace;

typedef struct rbTmrEntry
{
   S32        timerId;          /* timer id */
   Ent        ent;              /* timer pst */
   Inst       inst;             /* instance */
   PFS16      tmrFnct;          /* timer function */
   RbDq       *dq;              /* demand queue pointer */
   U16        delay;            /* delay - timer period */
   U32        sent;             /* # of msgs sent */
   U32        drop;             /* # of time-outs dropped */
   U32        received;         /* # of msgs received */
   CmTimer    timers[TMR_DEF_MAX]; /* timers */
} RbTmrEntry;

typedef struct rbOsCp
{
   RbDPool dPool[RB_NMB_MSG_POOL];    /* dynamic pools */
   RbSPool sPool;                     /* statis pool */
   RbMtxId tskTblMtx;                 /* task table mutex */
   RbTskEntry *tskTbl[RB_MAX_TASK];   /* task table */
   RbMtxId drvrTblMtx;                /* driver table mutex */
   RbDrvrEntry *drvrTbl[RB_MAX_DRVR]; /* driver table */
   RbMtxId tmrTblMtx;                 /* timer table mutex */
   RbTmrEntry *tmrTbl[RB_MAX_TIMER];  /* timer table */
   RbTmrCp tmrCp;                     /* control point for timers */
   RbTmrPlace tmrPlace;               /* timer placement on expiry */
   RbThrdId intHndlrThrd;             /* thread to handle all interrupts */
   U32 tskTblIdx;                     /* task table index */
   U32 drvrTblIdx;                    /* driver table index */
   U32 tmrTblIdx;                     /* timer table index */
   ProcId ownProcId;                  /* own proc id */
   Bool permTask;                     /* permanent task threads */
   Ticks sysTicks;                    /* system ticks */
   RbThrdId rdConThrdId;              /* read console task */
} RbOsCp;                             /* operating system control point */


typedef struct rbPoolInfo
{
   MsgLen strtIdx;              /* start index */
   MsgLen endIdx;
   S32 dSize;
   S32 nmbBufs;
}RbPoolInfo;



/* partition */

/* message */

/* queue */

/* tasks */


#endif


/********************************************************************30**

         End of file:     rb.x@@/main/1 - Mon Aug 29 22:31:34 2011

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
