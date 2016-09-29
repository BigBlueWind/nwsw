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

     Desc:     Various macro definitions demanded by systems services.
               The contents of these are for the RBSS implementation.

     File:     rb_ss.h

     Sid:      rb_ss.h@@/main/1 - Mon Aug 29 22:31:37 2011

     Prg:

*********************************************************************21*/

#ifndef __RBSSH__
#define __RBSSH__

/* --- Interface to SS --- */

/* general */
#define SS_PROC_ID                      PID_STK(1)
#ifdef RB_ACC_TEST
#define RB_BASE_CORE                    0
#else
#define RB_BASE_CORE                    3
#endif

/* task related */
#ifdef SS_MULTIPLE_PROCS
#define SS_MAX_PROCS                    100 /* max entries for proc list */
#endif /* SS_MULTIPLE_PROCS */

#define SS_MAX_ENT                      255
#define SS_MAX_INST                     8

#ifndef SS_MULTIPLE_PROCS
#define SS_MAX_TTSKS                    10

#define SS_MAX_STSKS                    5

#else /* SS_MULTIPLE_PROCS */
#define SS_MAX_TTSKS                    1000
#define SS_MAX_STSKS                    30
#endif /* SS_MULTIPLE_PROCS */

#ifdef SS_DRVR_SUPPORT
#define SS_MAX_DRVRTSKS                 5
#endif

#ifdef SS_RTR_SUPPORT
#define SS_MAX_RTRTSKS                  255
#endif

/* timer related */
#define SS_MAX_TMRS                     5

/* memory related */
#if (defined(RY_ENBS5SHM) && defined(USE_MEMCAL))
#define SS_MAX_REGS                     3
#define RY_SHM_REGION                   (SS_MAX_REGS - 2)
#define SS_STATIC_REGION                (SS_MAX_REGS - 1)
#else

#if (defined(RY_ENBS5SHM) || defined(USE_MEMCAL))
#define SS_MAX_REGS                     2
#ifdef RY_ENBS5SHM
#define RY_SHM_REGION                   (SS_MAX_REGS - 1)
#endif
#ifdef USE_MEMCAL
#define SS_STATIC_REGION                (SS_MAX_REGS - 1)
#endif
#else
#define SS_MAX_REGS                     1
#endif
#endif

#ifdef CMM_MAX_BKT_ENT
#define SS_MAX_POOLS_PER_REG            CMM_MAX_BKT_ENT
#else
#define SS_MAX_POOLS_PER_REG            7
#endif
#ifdef SS_WATCHDOG
#define ENTDW                           0xcd
#define ENTHB                           0xce
#define INST0                           0x00
#define SS_TMR_HRTBT                    0x00
#define EVTSSHRTBTREQ                   0x00
#define EVTSSHRTBTRSP                   0x01
#define SS_LOOSE_COUPLING               0x00
#endif /* SS_WATCHDOG */

/* locks */
#define SS_STSKTBL_LOCK                 SS_LOCK_MUTEX
#define SS_STSKENTRY_LOCK               SS_LOCK_MUTEX
#define SS_TMRTBL_LOCK                  SS_LOCK_MUTEX
#define SS_DMNDQ_LOCK                   SS_LOCK_MUTEX
#define SS_DRVRENTRY_LOCK               SS_LOCK_MUTEX
#define SS_RTRENTRY_LOCK                SS_LOCK_MUTEX

/* types needed by common SSI code */
#define SsSemaId                        volatile UINT32
#define SLockId                         volatile UINT32

#ifndef RB_SINGLE_CORE
/* calls needed by common SSI code */
#define SInitLock(l, t)                 rb_lock_init(l)
#define SLock(l)                        rb_lock(l)
#define SUnlock(l)                      rb_unlock(l)
#define SDestroyLock(l)                 rb_lock_deinit(l)
#else
#define SInitLock(l, t)   ((*l) = ROK)
#define SLock(l)          ((*l) = ROK)
#define SUnlock(l)        ((*l) = ROK)
#define SDestroyLock(l)   ((*l) = ROK)
#endif

#define ssInitSema(s, c)               ((*s) = ROK)  
#define ssWaitSema(s)                  ((*s) = ROK) 
#define ssPostSema(s)                  ((*s) = ROK)
#define ssDestroySema(s)               ((*s) = ROK)

#define SS_CHECK_CUR_STSK(t)            (ROK)

#define SInitSemaphore(s, c)             ((*s) = ROK) 
#define SWaitSemaphore(s)                ((*s) = ROK) 
#define SPostSemaphore(s)                ((*s) = ROK) 
#define SDestroySemaphore(s)             ((*s) = ROK)    

#define ssdPstTsk(p, m, t)

#define RB_MSG_HEAD_ROOM                32

#ifndef SS_LOGGER_SUPPORT
#define SExit()                         exit(0)
#else
#define SExit()  \
{ \
SCleanUp(); \
exit(0); \
}
#endif /* SS_LOGGER_SUPPORT  */

/* calls needed by Message Functions */

#define SMemCpy(d,s,c)  _memcpy(d,s,c)
#define SMemSet(s,c,n)  memset(s,c,n)
/* --- internal to RBSS-4GMX --- */

#define RB_TICK_CNT             (((U32)0x0F4240)/SS_TICKS_SEC)

#define RB_RANDOM_MAX           (365247)

#define RB_MAX_TICK_CNT_VAL     35
#define RB_MIN_TICK_CNT_VAL     1

/* interrupt service flags */
#define RB_IS_SET               0
#define RB_IS_UNSET             1
#define RB_IS_RESET             2

/******************************************************************

Memory block sizes and counts for memory manager configuration
There is no restriction in the size of each block for the bucket.
However, it is recommended that the bucket size should be word aligned.
The CMM (Common Memory Manager) also create a look up table which map
the size to the bucket Index.
The number of entry in the lookup table  (CMM_MAX_MAP_ENT, defined in
cm_mem.h) = ((maximum bucket size)/(bucket quantum size)) + 1.
The CMM_MAX_MAP_ENT should be changed depending on the bucket sizes
that are configured below.
*******************************************************************/

#ifdef RB_HEAD_ROOM
/* Bucket 0 configuration */
#define RB_BKT_0_DSIZE                  (256 + (RB_MSG_HEAD_ROOM * 2))
#define RB_BKT_0_NUMBLKS                128000

/* Bucket 1 configuration */
#define RB_BKT_1_DSIZE                  (512 + (RB_MSG_HEAD_ROOM * 2))
#define RB_BKT_1_NUMBLKS                512

/* Bucket 2 configuration */
#define RB_BKT_2_DSIZE                  (768 + (RB_MSG_HEAD_ROOM * 2))
#define RB_BKT_2_NUMBLKS                10

/* Bucket 3 configuration */
#define RB_BKT_3_DSIZE                  (1024 + (RB_MSG_HEAD_ROOM * 2))      /* Fill in this value as required */
#define RB_BKT_3_NUMBLKS                2000    /* Fill in this value as required */

#define RB_MAX_BKTS                     4
#else
/* Bucket 0 configuration */
#define RB_BKT_0_DSIZE                  128
#define RB_BKT_0_NUMBLKS                125000

/* Bucket 1 configuration */
#define RB_BKT_1_DSIZE                  256
#define RB_BKT_1_NUMBLKS                125000

/* Bucket 2 configuration */
#define RB_BKT_2_DSIZE                  512
#define RB_BKT_2_NUMBLKS                8000

/* Bucket 3 configuration */
#define RB_BKT_3_DSIZE                  768
#define RB_BKT_3_NUMBLKS                400     /* Fill in this value as required */

/* Bucket 3 configuration */
#define RB_BKT_4_DSIZE                  1664
#define RB_BKT_4_NUMBLKS                600     /* Fill in this value as required */

#define RB_BKT_5_DSIZE                  4800 
#define RB_BKT_5_NUMBLKS                400     /* Fill in this value as required */

#define RB_BKT_6_DSIZE                  9920 
#define RB_BKT_6_NUMBLKS                400     /* Fill in this value as required */

#define RB_MAX_BKTS                     7
#endif

/* memory pool data size definitions for pool-to-size mapping table */
#define RB_POOL_3_DSIZE        (RB_BKT_3_DSIZE-(sizeof(SsMblk)+sizeof(SsDblk)))
#define RB_POOL_2_DSIZE        (RB_BKT_2_DSIZE-(sizeof(SsMblk)+sizeof(SsDblk)))
#define RB_POOL_1_DSIZE        (RB_BKT_1_DSIZE-(sizeof(SsMblk)+sizeof(SsDblk)))
#define RB_POOL_0_DSIZE        (RB_BKT_0_DSIZE-(sizeof(SsMblk)+sizeof(SsDblk)))

/* memory size used for heap by the memory manager (1MB) */
#define RB_HEAP_SIZE            1170272U

#ifndef USE_MEMCAL
#define STATIC_MEM_CFG
#endif

#define RB_BKTQNSIZE 128

/* Configuration for stack size (in bytes) of spawned threads
 * Size of zero gives default of 1 MB or 2 MB for 32 bit or 64 bit
 * compilers, respectively */
#define RB_TASK_STACK          NULLD    /* stack size for task handler */
#define RB_ISTASK_STACK        NULLD    /* stack size for IS task handler */
#define RB_TMRTASK_STACK       NULLD    /* stack size for timer handler */
#define RB_CONSOLE_STACK       NULLD    /* stack size for console handler */

#ifdef SS_LOGGER_SUPPORT
#ifndef SS_MAX_LOGBUF_SIZE
#define SS_MAX_LOGBUF_SIZE 8192
#endif
#ifndef SS_MAX_PATH
#define SS_MAX_PATH 1024
#endif
#endif /* SS_LOGGER_SUPPORT */

#ifdef RB_DEBUG
EXTERN U8 rbLogBuf[256];
#define RB_PRINTF(...) do { \
   sprintf((char *)rbLogBuf, "%s():%u ", __func__, __LINE__); \
   sprintf((char *)&rbLogBuf[strlen((char *)rbLogBuf)], __VA_ARGS__); \
   SPrint((Txt *) rbLogBuf); \
} while (0)
#else
#ifdef RB_ACC_TEST
#define RB_PRINTF uart_printf
#else
#define RB_PRINTF(...)
#endif
#endif

#define SsiHeap                         DDR1Heap

#endif  /*  __RBSSH__  */


/********************************************************************30**

         End of file:     rb_ss.h@@/main/1 - Mon Aug 29 22:31:37 2011

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
