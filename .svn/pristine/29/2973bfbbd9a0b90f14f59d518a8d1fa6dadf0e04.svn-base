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

     Name:     Multi-threaded System Services - Solaris

     Type:     C source file

     Desc:     C source code for the RBSS-Solaris implementation of
               System Services.

     File:     rb_ss.c

     Sid:      ju_ss.c@@/main/1 - Mon Aug 29 22:31:34 2011

     Prg:

*********************************************************************21*/

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "rb_include.h"
#include "cm_lib.x"
#include "ss_diag.h"        /* Commonm log file */
#ifndef RB_4GMX_OLD_LOCK
#include "4gmx_kernel.h"
#endif /* RB_4GMX_OLD_LOCK */
#include "appinit.h"


/* forward references */
UINT32 ssdInitAsync (void *pThreadParam);
volatile U16 ssStarted = FALSE;
U8 rbCpuMask = 0x00;
EXTERN volatile U16 icoreStarted[MAX_NUM_CORES];
U8  rbLogBuf[256];

EXTERN U32 kwUlSelfMsgPosted;

/*
*
*       Fun:    dump()
*
*       Desc:
*
*       Ret:    Void
*
*       Notes:  None
*
*       File:   rb_ss.c
*
*/
#ifdef ANSI
PUBLIC Void dump
(
CONSTANT U8  *data_buffer,
CONSTANT U32 length
)
#else
PUBLIC Void dump(data_buffer, length)
CONSTANT U8  *data_buffer;
CONSTANT U32 length;
#endif
{
   U8 byte;
   U32 i, j;

   for(i=0; i < length; i++)
   {
      byte = data_buffer[i];
      RB_PRINTF("%02x ", data_buffer[i]);
      if(((i%16)==15) || (i==length-1))
      {
         for(j=0; j < 15-(i%16); j++)
            RB_PRINTF("   ");
         RB_PRINTF("| ");
         for(j=(i-(i%16)); j <= i; j++)
         {
            byte = data_buffer[j];
            if((byte > 31) && (byte < 127))
            {
               RB_PRINTF("%c", byte);
            }
            else
            {
               RB_PRINTF(".");
            }
         }
         RB_PRINTF("\n");
      }
   }
}

/*
 * (size vs. numAttempts) And (allocations vs. deallocations)
 */

#ifdef SSI_DEBUG_LEVEL1
PRIVATE S16 SPrintRegMemStats ARGS((Region region));
#endif /* SSI_DEBUG_LEVEL1 */
PRIVATE Void rbInitCpuMask ARGS((void));


/* type declarations */
#ifdef SS_DRVR_SUPPORT
typedef struct rbIsFlag
{
   U16 id;
   U8 action;
} RbIsFlag;
#endif

#ifdef ENB
/* L2 doesn't use this call back */
MXRC (*LteMacAppInitCallback)(void) = NULL;
#endif


/* public variable declarations */
PUBLIC VOLATILE HANDLE hThread[MAX_NUM_CORES];
PUBLIC VOLATILE HANDLE hBaseTmrThread;
PUBLIC VOLATILE Cntr cfgNumRegs = SS_MAX_REGS;
/* Set memory configuration as FALSE.
 * Set to true if memory configuration through file is successfull.
 */
PUBLIC VOLATILE Bool memConfigured = FALSE;
PUBLIC VOLATILE SsRegCfg cfgRegInfo[SS_MAX_REGS] =
{
   {
      SS_DFLT_REGION, SS_MAX_POOLS_PER_REG - 1,
      {
         { SS_POOL_DYNAMIC, RB_POOL_0_DSIZE },
         { SS_POOL_DYNAMIC, RB_POOL_1_DSIZE },
         { SS_POOL_DYNAMIC, RB_POOL_2_DSIZE },
         { SS_POOL_DYNAMIC, RB_POOL_3_DSIZE },
         { SS_POOL_STATIC, 0 }
      }
    }
#ifdef SS_FAP
  ,{
     SS_WL_REGION, SS_MAX_POOLS_PER_REG - 1,
     {
         { SS_POOL_DYNAMIC, RB_POOL_0_DSIZE },
         { SS_POOL_DYNAMIC, RB_POOL_1_DSIZE },
         { SS_POOL_DYNAMIC, RB_POOL_2_DSIZE },
         { SS_POOL_DYNAMIC, RB_POOL_3_DSIZE },
         { SS_POOL_STATIC, 0 }
     }
  }
#endif
#ifdef  RY_ENBS5SHM
   ,{
      RY_SHM_REGION, SS_MAX_POOLS_PER_REG - 1,
      {
         { SS_POOL_DYNAMIC, RB_POOL_0_DSIZE },
         { SS_POOL_DYNAMIC, RB_POOL_1_DSIZE },
         { SS_POOL_DYNAMIC, RB_POOL_2_DSIZE },
         { SS_POOL_DYNAMIC, RB_POOL_3_DSIZE },
         { SS_POOL_STATIC, 0 }
      }
   }
#endif
#ifdef USE_MEMCAL
   ,{
      SS_STATIC_REGION, SS_MAX_POOLS_PER_REG,
      {
         {SS_POOL_DYNAMIC, 16},
         {SS_POOL_DYNAMIC, 32},
         {SS_POOL_DYNAMIC, 80},
         {SS_POOL_DYNAMIC, 256},
         {SS_POOL_DYNAMIC, 320},
      }
   }
#endif
};
/*
 * common for both MULTICORE and NON-MULTICORE
 */

PUBLIC VOLATILE RbMemCfg rbMemoCfg =
{
#ifdef  RY_ENBS5SHM
  SS_MAX_REGS - 1,                            /* number of regions */
#else
  SS_MAX_REGS,                                /* number of regions */
#endif
  {
    {
      SS_DFLT_REGION,                         /* region id */
      RB_MAX_BKTS,                            /* number of buckets */
      RB_HEAP_SIZE,                           /* heap size */
      {
        {RB_BKT_0_DSIZE, RB_BKT_0_NUMBLKS},   /* block size, no. of blocks */
        {RB_BKT_1_DSIZE, RB_BKT_1_NUMBLKS},    /* block size, no. of blocks */
        {RB_BKT_2_DSIZE, RB_BKT_2_NUMBLKS},   /* block size, no. of blocks */
        {RB_BKT_3_DSIZE, RB_BKT_3_NUMBLKS},   /* block size, no. of blocks */
        {RB_BKT_4_DSIZE, RB_BKT_4_NUMBLKS},    /* block size, no. of blocks */
        {RB_BKT_5_DSIZE, RB_BKT_5_NUMBLKS},    /* block size, no. of blocks */
        {RB_BKT_6_DSIZE, RB_BKT_6_NUMBLKS}    /* block size, no. of blocks */
      }
    },
#ifdef SS_FAP
     {
        SS_WL_REGION,                         /* region id */
        RB_MAX_BKTS,                            /* number of buckets */
        RB_HEAP_SIZE,                           /* heap size */
        {
           {RB_BKT_0_DSIZE, RB_BKT_0_NUMBLKS},   /* block size, no. of blocks */
           {RB_BKT_1_DSIZE, RB_BKT_1_NUMBLKS},    /* block size, no. of blocks */
           {RB_BKT_2_DSIZE, RB_BKT_2_NUMBLKS},   /* block size, no. of blocks */
           {RB_BKT_3_DSIZE, RB_BKT_3_NUMBLKS}    /* block size, no. of blocks */
        }
     },
#endif
    STATIC_MEM_CFG
  }
};
/* Modifications - File Based task registration made
 * common for both MULTICORE and NON-MULTICORE
 * bucket info, as different regions may request for different no.
 * of blocks
 */
PUBLIC VOLATILE RbBktCfg rbBktInfo[RB_MAX_BKTS];


/* private variable declarations */
PUBLIC VOLATILE CmMmRegCfg *rbCMMRegCfg[SS_MAX_REGS];
PUBLIC VOLATILE CmMmRegCb *rbCMMRegCb[SS_MAX_REGS];

#ifdef RB_ACC_TEST
/* Adding prototype for testMain (defined in rb_acc.c) */
EXTERN S32 testMain ARGS((S32, S8**));
#endif

#ifndef RB_SINGLE_CORE

/*
*
*       Fun:   rbMain
*
*       Desc:  Initializes communication channel, timer channel, etc. on
*              other cores.
*
*       Ret:   none on success
*              exit on failure
*
*       Notes:
*
*       File:  rb_ss.c
*
*/
#ifdef ANSI
PUBLIC int rbMain
(
int argc,                       /* argument count */
char **argv                     /* argument vector */
)
#else
PUBLIC int rbMain(argc, argv)
int argc;                       /* argument count */
char **argv;                    /* argument vector */
#endif
{
   TRC0(rbMain);

   rbICpuInit();
#ifndef RB_CHANNEL_SVSR
   rbInitTmrQueue();
   rbInitICoreQueue();
#else
   rbInitICoreSvsr();
   rbInitTmrSvsr();
#endif
#if (defined(RB_TMR_ALLCORE) && !defined(SS_TTI_TMR))
   rbTmrInitStart();
#endif

   RETVALUE(0);
}
#ifndef RB_4GMX_OLD_LOCK
#define MxGetLock(x)    MxiGetLock (x)
#endif /* RB_4GMX_OLD_LOCK */


/*
*
*       Fun:    rb_lock_init()
*
*       Desc:   Initializes locks
*
*       Ret:    ROK     : if Success
*               RFAILED : If Failure
*
*       Notes:  None.
*
*       File:   rb_ss.c
*
*/
#ifdef ANSI
PUBLIC S32 rb_lock_init
(
SLockId *lock
)
#else
PUBLIC S32 rb_lock_init(lock)
SLockId *lock;
#endif
{
#ifdef RB_4GMX_OLD_LOCK
   MxExcSet(lock,0);
#else /* RB_4GMX_OLD_LOCK */
   *lock = 0;
#endif /* RB_4GMX_OLD_LOCK */
   RETVALUE(ROK);
}


/*
*
*       Fun:    rb_lock_deinit
*
*       Desc:   DeInitializes locks
*
*       Ret:    ROK     : if Success
*               RFAILED : If Failure
*
*       Notes:  None.
*
*       File:   rb_ss.c
*
*/
#ifdef ANSI
PUBLIC S32 rb_lock_deinit
(
SLockId *lock
)
#else
PUBLIC S32 rb_lock_deinit(lock)
SLockId *lock;
#endif
{
#ifdef RB_4GMX_OLD_LOCK
   MxExcSet(lock,0);
#else /* RB_4GMX_OLD_LOCK */
   *lock = 0;
#endif /* RB_4GMX_OLD_LOCK */
   RETVALUE(ROK);
}


/*
*
*       Fun:    rb_lock
*
*       Desc:   This function will acquire lock
*
*       Ret:    ROK     : if Success
*               RFAILED : If Failure
*
*       Notes:  None.
*
*       File:   rb_ss.c
*
*/
#ifdef ANSI
PUBLIC S32 rb_lock
(
SLockId *lock
)
#else
PUBLIC S32 rb_lock(lock)
SLockId *lock;
#endif
{
#ifdef RB_4GMX_OLD_LOCK
   while((*lock == 0)?(MxExcInc(lock) != 1):1);
#else /* RB_4GMX_OLD_LOCK */
   MxGetLock (lock);
#endif /* RB_4GMX_OLD_LOCK */
   RETVALUE(ROK);
}


/*
*
*       Fun:    rb_unlock
*
*       Desc:   This function releases lock
*
*       Ret:    ROK     : if Success
*               RFAILED : If Failure
*
*       Notes:  None.
*
*       File:   rb_ss.c
*
*/
#ifdef ANSI
PUBLIC S32 rb_unlock
(
SLockId *lock
)
#else
PUBLIC S32 rb_unlock(lock)
SLockId *lock;
#endif
{
#ifdef RB_4GMX_OLD_LOCK
   MxExcSet(lock,0);
#else /* RB_4GMX_OLD_LOCK */
   MxReleaseLock(lock);
#endif /* RB_4GMX_OLD_LOCK */
   RETVALUE(ROK);
}
#endif


/*
*
*       Fun:    ssdInitAsync
*
*       Desc:   The source code of a thread
*
*       Ret:    ROK, RFAILED
*
*       Notes:  None.
*
*       File:   rb_ss.c
*
*/
#ifdef ANSI
PUBLIC UINT32 ssdInitAsync
(
void * pThreadParam
)
#else
PUBLIC UINT32 ssdInitAsync(pThreadParam)
void * pThreadParam;
#endif
{
   int cpuId = MxGetCpuID();
   S16 ret = ROK;

   if (cpuId == RB_BASE_CORE)
   {
      rbInitCpuMask();
#ifndef RB_ACC_TEST
      ret = ssMain(NULL, NULL);
#else
      ret = testMain(NULL, NULL);
#endif
   }
#ifndef RB_SINGLE_CORE
   else
   {
      while(ssStarted != TRUE);
      ret = rbMain(NULL, NULL);
      RB_PRINTF("[%s:%d] coreID= %d ret=%d\n",__func__,__LINE__, cpuId, ret);
   }
#endif
   return ret;
}


/*
*
*       Fun:    rbCpu
*
*       Desc:   Asynchronous initialization of threads
*
*       Ret:    ROK, RFAILED
*
*       Notes:  None.
*
*       File:   rb_ss.c
*
*/
#ifdef ANSI
PUBLIC MXRC rbCpu
(
void
)
#else
PUBLIC MXRC rbCpu()
#endif
{
   MXRC rc    = MX_OK;
   U16  cpuId = MxGetCpuID();

   /* Input params:    thr entry, thr name, param,   priority,   thread handle */
   rc = MxCreateThread (ssdInitAsync,  NULL, (LPVOID)cpuId, (TPRIORITY)0, &hThread[cpuId]);

   if (rc == MX_OK)
   {
      rc = MxScheduleThread (hThread[cpuId]);
   }

   return rc;
}

#ifdef ENB

/*
*
*       Fun:   MacRegMsgThread
*
*       Desc:  This function is the entry point for the PHY integrated binary.
*
*       Ret:   none on success
*              exit on failure
*
*       Notes:
*
*       File:  rb_ss.c
*
*/
#ifdef ANSI
PUBLIC UINT32 MacRegMsgThread
(
void
)
#else
PUBLIC UINT32 MacRegMsgThread(void)
#endif
{
   MXRC ret;

   TRC0(MacRegMsgThread);

   ret = rbCpu();
   if (ret != MX_OK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      RBLOGERROR(ERRCLS_DEBUG, ERB047, 0, "Failed to Initialize SSI");
#endif
      RETVALUE(ret);
   }

   RETVALUE(MX_OK);
}
#endif


/*
*
*       Fun:   rbInitCpuMask
*
*       Desc:  This function for the final binary. It
*
*       Ret:   none on success
*              exit on failure
*
*       Notes:
*
*       File:  rb_ss.c
*
*/
#ifdef ANSI
PRIVATE Void rbInitCpuMask
(
void
)
#else
PRIVATE Void rbInitCpuMask(void)
#endif
{
   U16 tsk = 0;

   SsRegIcpuCfg icpuCfg[SS_MAX_TTSKS] = ICPUINIT;

   for(tsk=0; tsk < SS_MAX_TTSKS; tsk++)
   {
      if((icpuCfg[tsk].Ent == RB_END) && (icpuCfg[tsk].Inst == RB_END)
            && (icpuCfg[tsk].CoreId == RB_END))
      {
         break;
      }
      rbCpuMask = rbCpuMask | (0x01 << icpuCfg[tsk].CoreId);
   }

   RETVOID;
}

#ifdef RB_ACC_TEST

/*
*
*       Fun:   main
*
*       Desc:  This function is the entry point for the final binary. It
*              calls SInit() in the common code. It can be replaced by a
*              user function if required (SInit() must still be called).
*
*       Ret:   none on success
*              exit on failure
*
*       Notes:
*
*       File:  rb_ss.c
*
*/
#ifdef ANSI
PUBLIC int main
(
void
)
#else
PUBLIC int main(void)
#endif
{
   SyscoreParam param;
   U16 core = 0;

   rbInitCpuMask();

   if (rbCpuMask == 0)
   {
      RB_PRINTF("\nCPU mask uninitialized\n");
      RETVALUE(RFAILED);
   }

   memset(&param, 0, sizeof(param));

   for(core = 0; core < MAX_NUM_CORES; core++)
   {
      param.app_entry[core] = (rbCpuMask & (0x01 << core)) ? rbCpu : NULL;
      param.mmu_cfg[core] = NULL;//MmuCfg_CpuX;
   }

   return SysCoreStart(&param);
}
#endif


/*
*
*       Fun:   ssMain
*
*       Desc:  This function is the entry point for the final binary. It
*              calls SInit() in the common code. It can be replaced by a
*              user function if required (SInit() must still be called).
*
*       Ret:   none on success
*              exit on failure
*
*       Notes:
*
*       File:  rb_ss.c
*
*/
#ifdef ANSI
PUBLIC int ssMain
(
int argc,                       /* argument count */
char **argv                     /* argument vector */
)
#else
PUBLIC int ssMain(argc, argv)
int argc;                       /* argument count */
char **argv;                    /* argument vector */
#endif
{
   TRC0(ssMain);

   SSetProcId(IARM_PROCESSOR_ID_LOW);

   if( ROK != SInit())
   {
      RETVALUE(0);
   }

   SFini();

   RETVALUE(0);
}

/*
 *  initialization functions
 */

/*
 *
 *       Fun:   Initialize OS control point
 *
 *       Desc:  This function initializes RBSS-specific information
 *              in the OS control point.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 ssdInitGen
(
void
)
#else
PUBLIC S16 ssdInitGen()
#endif
{
   TRC0(ssdInitGen);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   De-initialize OS control point
 *
 *       Desc:  This function reverses the initialization in ssdInitGen().
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC Void ssdDeinitGen
(
void
)
#else
PUBLIC Void ssdDeinitGen()
#endif
{
   TRC0(ssdDeinitGen);

   rbLarmStop();

   RETVOID;
}


/*
 *
 *       Fun:   Initialize region/pool tables
 *
 *       Desc:  This function initializes RBSS-specific information
 *              in the region/pool tables and configures the common
 *              memory manager for use.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 ssdInitMem
(
void
)
#else
PUBLIC S16 ssdInitMem()
#endif
{
   U8          i;
   U16         j;
   U8          k;
   RbRegCfg    *region;
   Txt        errMsg[256] = {'\0'};

   TRC0(ssdInitMem);

   {
      extern SysStayInfo syscoreinfo[4];
      uart_printf("\n\rsyscoreinfo: addr 0x%x size %u\n\r", syscoreinfo, sizeof(syscoreinfo));
   }


   for (i = 0; i < rbMemoCfg.numRegions; i++)
   {
      /* allocate space for the region control block */
      rbCMMRegCb[i] = (CmMmRegCb *)MxHeapAlloc(SsiHeap, sizeof(CmMmRegCb));

      uart_printf("rbCMMRegCb[%d] @ %08x size %d\n", i, (unsigned int)rbCMMRegCb[i], sizeof(CmMmRegCb));

      if (rbCMMRegCb[i] == NULLP)
      {
         sprintf((char *)errMsg,"\n ssdInitMem(): Could not allocated memory \
               for the Region:%d control block\n",i);
         SPrint((Txt *) errMsg);
         for (k = 0; k < i; k++)
         {
            cmMmRegDeInit(rbCMMRegCb[k]);
            MxHeapFree(SsiHeap, rbCMMRegCfg[k]->vAddr);
            MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCb[k]);
            MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCfg[k]);
         }
         RETVALUE(RFAILED);
      }
      cmMemset((U8*)rbCMMRegCb[i], 0, sizeof(CmMmRegCb));

      rbCMMRegCfg[i] = (CmMmRegCfg *)MxHeapAlloc(SsiHeap, sizeof(CmMmRegCfg));

      uart_printf("rbCMMRegCfg[%d] @ %08x size %d\n", i, (unsigned int)rbCMMRegCfg[i], sizeof(CmMmRegCfg));

      if (rbCMMRegCfg[i] == NULLP)
      {
         for (k = 0; k < i; k++)
         {
            cmMmRegDeInit(rbCMMRegCb[k]);
            MxHeapFree(SsiHeap, rbCMMRegCfg[k]->vAddr);
            MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCb[k]);
            MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCfg[k]);
         }
         MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCb[i]);
         RETVALUE(RFAILED);
      }
      cmMemset((U8*)rbCMMRegCfg[i], 0, sizeof(CmMmRegCfg));

      /* allocate space for the region */
      region = &rbMemoCfg.region[i];
      rbCMMRegCfg[i]->size = region->heapsize;
      for (j = 0; j < region->numBkts; j++)
      {
#ifdef SSI_DEBUG_LEVEL1
         rbCMMRegCfg[i]->size += (region->bkt[j].blkSize + sizeof(CmMmBlkHdr)) *\
                                 (region->bkt[j].numBlks);
#ifdef RB_HEAD_ROOM
        rbCMMRegCfg[i]->size += (RB_MSG_HEAD_ROOM * 2) * region->bkt[j].numBlks;
#endif
#else
         rbCMMRegCfg[i]->size += region->bkt[j].blkSize * region->bkt[j].numBlks;
#ifdef RB_HEAD_ROOM
         rbCMMRegCfg[i]->size += (RB_MSG_HEAD_ROOM * 2) * region->bkt[j].numBlks;
#endif
#endif /* SSI_DEBUG_LEVEL1 */
      }

      /* MS_FIX */
      rbCMMRegCfg[i]->vAddr = (Data *)MxHeapAlloc(SsiHeap, rbCMMRegCfg[i]->size * sizeof(Data));

      uart_printf("rbCMMRegCfg[%d] @ %08x size %d\n", i,
                        (unsigned int)rbCMMRegCfg[i]->vAddr,
                        rbCMMRegCfg[i]->size * sizeof(Data));

      if (rbCMMRegCfg[i]->vAddr == NULLP)
      {
         sprintf((char *)errMsg,"\n ssdInitMem(): Could not allocate memory \
               for the Region:%d \n",i);
         SPrint((Txt *) errMsg);
         for (k = 0; k < i; k++)
         {
            cmMmRegDeInit(rbCMMRegCb[k]);
            MxHeapFree(SsiHeap, rbCMMRegCfg[k]->vAddr);
            MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCb[k]);
            MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCfg[k]);
         }
         MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCb[i]);
         MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCfg[i]);
         RETVALUE(RFAILED);
      }

      cmMemset((U8*)rbCMMRegCfg[i]->vAddr, 0, rbCMMRegCfg[i]->size * sizeof(Data));
      /* set up the CMM configuration structure */
      rbCMMRegCfg[i]->lType = SS_LOCK_MUTEX;
      rbCMMRegCfg[i]->chFlag = 0;
      rbCMMRegCfg[i]->bktQnSize = RB_BKTQNSIZE;
      rbCMMRegCfg[i]->numBkts = region->numBkts;

      for (j = 0; j < region->numBkts; j++)
      {
         rbCMMRegCfg[i]->bktCfg[j].size    = region->bkt[j].blkSize;
         rbCMMRegCfg[i]->bktCfg[j].numBlks = region->bkt[j].numBlks;
      }

      /* initialize the CMM */
      if (cmMmRegInit(region->regionId, rbCMMRegCb[i], rbCMMRegCfg[i]) != ROK)
      {
         for (k = 0; k < i; k++)
         {
            cmMmRegDeInit(rbCMMRegCb[k]);
            MxHeapFree(SsiHeap, rbCMMRegCfg[k]->vAddr);
            MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCb[k]);
            MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCfg[k]);
         }
         MxHeapFree(SsiHeap, rbCMMRegCfg[i]->vAddr);
         MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCb[i]);
         MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCfg[i]);
         RETVALUE(RFAILED);
      }

      /* initialize the STREAMS module */
      if (region->regionId == 0)
      {
         if (ssStrmCfg(region->regionId, region->regionId) != ROK)
         {
            for (k = 0; k < i; k++)
            {
               cmMmRegDeInit(rbCMMRegCb[k]);
               MxHeapFree(SsiHeap, rbCMMRegCfg[k]->vAddr);
               MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCb[k]);
               MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCfg[k]);
            }
            cmMmRegDeInit(rbCMMRegCb[i]);
            MxHeapFree(SsiHeap, rbCMMRegCfg[i]->vAddr);
            MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCb[i]);
            MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCfg[i]);
            RETVALUE(RFAILED);
         }
      }
   }
#ifdef SS_MEM_LEAK_STS
   cmInitMemLeakMdl();
#endif /* SS_MEM_LEAK_STS */

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   De-initialize region/pool tables
 *
 *       Desc:  This function reverses the initialization in ssdInitMem().
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC Void ssdDeinitMem
(
void
)
#else
PUBLIC Void ssdDeinitMem()
#endif
{
   U8     i;

   TRC0(ssdDeinitMem);
#ifdef SS_MEM_LEAK_STS
   cmDeinitMemLeakMdl();
#endif /* SS_MEM_LEAK_STS */

   for (i = 0; i < rbMemoCfg.numRegions; i++)
   {
      cmMmRegDeInit(rbCMMRegCb[i]);
      MxHeapFree(SsiHeap, rbCMMRegCfg[i]->vAddr);
      MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCb[i]);
      MxHeapFree(SsiHeap, (LPVOID) rbCMMRegCfg[i]);
   }

   RETVOID;
}


/*
 *
 *       Fun:   Initialize task table
 *
 *       Desc:  This function initializes RBSS-specific information
 *              in the task table.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 ssdInitTsk
(
void
)
#else
PUBLIC S16 ssdInitTsk()
#endif
{
   TRC0(ssdInitTsk);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Deinitialize task table
 *
 *       Desc:  This function reverses the initialization perfomed in
 *              ssdInitTsk().
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC Void ssdDeinitTsk
(
void
)
#else
PUBLIC Void ssdDeinitTsk()
#endif
{
   TRC0(ssdDeinitTsk);

   RETVOID;
}


#ifdef SS_DRVR_SUPPORT
/*
 *
 *       Fun:   Initialize driver task table
 *
 *       Desc:  This function initializes RBSS-specific information
 *              in the driver task table.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 ssdInitDrvr
(
void
)
#else
PUBLIC S16 ssdInitDrvr()
#endif
{
   TRC0(ssdInitDrvr);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Deinitialize driver information
 *
 *       Desc:  This function reverses the initialization performed in
 *              ssdInitDrvr().
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC Void ssdDeinitDrvr
(
void
)
#else
PUBLIC Void ssdDeinitDrvr()
#endif
{
   TRC0(ssdDeinitDrvr);

   RETVOID;
}
#endif /* SS_DRVR_SUPPORT */


/*
 *
 *       Fun:   Initialize timer table
 *
 *       Desc:  This function initializes RBSS-specific information
 *              in the timer table.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 ssdInitTmr
(
void
)
#else
PUBLIC S16 ssdInitTmr()
#endif
{
#ifndef RB_TICK_TIMER
   S32 i;
#endif

   TRC0(ssdInitTmr);

#ifndef RB_TICK_TIMER
   osCp.dep.tmrTqCp.tmrLen = SS_MAX_TMRS;
   osCp.dep.tmrTqCp.nxtEnt = 0;
   for (i=0; i< SS_MAX_TMRS; i++)
   {
      osCp.dep.tmrTq[i].first = (CmTimer *)NULLP;
   }
#endif

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Deinitialize timer table
 *
 *       Desc:  This function reverses the initialization performed in
 *              ssdInitTmr().
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC Void ssdDeinitTmr
(
void
)
#else
PUBLIC Void ssdDeinitTmr()
#endif
{
   TRC0(ssdDeinitTmr);

   RETVOID;
}


/*
 *
 *       Fun:   ssdInitLog
 *
 *       Desc:  Pre-tst() initialization.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 ssdInitLog
(
void
)
#else
PUBLIC S16 ssdInitLog()
#endif
{
   TRC0(ssdInitLog);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   ssdDeinitLog
 *
 *       Desc:  This function reverses the initialization performed in
 *              ssdInitLog().
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC Void ssdDeinitLog
(
void
)
#else
PUBLIC Void ssdDeinitLog()
#endif
{
   TRC0(ssdDeinitLog);

   RETVOID;
}


/*
 *
 *       Fun:   SGetOpt
 *
 *       Desc:  Get options from command line
 *
 *       Ret:   option  - success
 *              '?'     - fail
 *              EOF     - end of options
 *
 *       Notes: Handles command lines like the following
 *
 *              if opts = "abc:d"
 *                 then command line should look like This...
 *                    -a foo -b foo1 -c -d foo
 *
 *              code usage:
 *
 *              while ((ret = SGetOpt(msArgc, msArgv, "ls")) != EOF )
 *              {
 *                 switch(ret)
 *                 {
 *                    case 'l':
 *                       nloops = atoi(msArgv[msOptInd]);
 *                       break;
 *                    case 's':
 *                       state1 = atoi(msArgv[msOptInd]);
 *                    case '?':
 *                    default:
 *                       break;
 *                 }
 *              }
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SGetOpt
(
int argc,                   /* argument count */
char **argv,                /* argument value */
char *opts                  /* options */
)
#else
PUBLIC S16 SGetOpt(argc, argv, opts)
int argc;                   /* argument count */
char **argv;                /* argument value */
char *opts;                 /* options */
#endif
{
   RETVALUE(ROK);
}


/*
 *
 *       Fun:   rbLarmStop
 *
 *       Desc:  This function stops system services execution
 *
 *       Ret:   Void
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC Void rbLarmStop
(
void
)
#else
PUBLIC Void rbLarmStop()
#endif
{
   U32 coreID =0;

   TRC0(rbLarmStop);

    //coreID = MxGetCpuID();

#ifndef SS_TTI_TMR
   rbTmrDeInit(coreID);
#endif

#ifndef RB_CHANNEL_SVSR
   rbDeInitICoreQueue();
#else
   rbDeInitICoreSvsr();
#endif

    rbMlogDeInit(MxGetCpuID());

   ssStarted = FALSE;

   RETVOID;
}


/*
 *
 *       Fun:   rbWaitForICoreStart
 *
 *       Desc:  This function waits for ICORE queues to be started
 *
 *       Ret:   Void
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC Void rbWaitForICoreStart
(
void
)
#else
PUBLIC Void rbWaitForICoreStart()
#endif
{
   S16 core = 0;

   TRC0(rbWaitForICoreStart);

   for(core = 0; core < MAX_NUM_CORES; core++)
   {
      if(core != MxGetCpuID())
      {
         if(rbCpuMask & (0x01 << core))
         {
            while(icoreStarted[core]!=TRUE);
         }
      }
   }

   RETVOID;
}


/*
 *
 *       Fun:   rbLarmStart
 *
 *       Desc:  This function starts system services execution
 *
 *       Ret:   Void
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC Void rbLarmStart
(
void
)
#else
PUBLIC Void rbLarmStart()
#endif
{
   U32 coreID = MxGetCpuID();
   MXRC rc = MX_OK;

   TRC0(rbLarmStart);

   rbCfgInit();
#ifdef SS_DRVR_SUPPORT
   rbDrvrInit();
#endif
   rbICpuInit();
   rbMlogInit(0);
   /*rb001.101 Modified for Self Post related Fix*/
#ifndef RB_CHANNEL_SVSR
   rbInitICoreQueue();
   rbInitTmrQueue();
#else
   rbInitICoreSvsr();
   rbInitTmrSvsr();
#endif
   ssStarted = TRUE;
   rbWaitForICoreStart();
#ifndef SS_TTI_TMR
   /* IMP: Base timer should always start from core 0.
    * A limitation of 4GMX.
    */
   if (coreID == 0)
   {
      rbTmrInitStart(NULL);
   }
   else
   {
      /* Input params: coreId, thr entry, thr name, param,   priority,   thread handle */
      rc = MxCreateThreadEx(0, (TENTRY) rbTmrInitStart, NULL, (LPVOID)coreID, (TPRIORITY)0, &hBaseTmrThread);
      if (rc == MX_OK)
      {
         rc = MxScheduleThread (hBaseTmrThread);
      }
   }
#endif

   {
      extern void (*L2StopHndlr)();
      extern void (*L2AbortHndlr)();
      extern void rsysStopHndlr();
      L2StopHndlr  = &rsysStopHndlr;
      L2AbortHndlr = &rsysStopHndlr;
   }

   RETVOID;
}


/*
 *
 *       Fun:   ssdStart
 *
 *       Desc:  This function starts system services execution; the
 *              permanent tasks are started and the system enters a
 *              ready state.
 *
 *       Ret:   Void
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC Void ssdStart
(
void
)
#else
PUBLIC Void ssdStart()
#endif
{
   TRC0(ssdStart);

#ifndef RB_ACC_TEST
   rbLarmStart();
#endif

   RETVOID;
}

/*
 *     Indirect Interface functions to system services service user
 */


/*
 *
 *       Fun:   ssdAttachTTsk
 *
 *       Desc:  This function sends the initial tick message to a TAPA
 *              task if the task is a permanent task.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 ssdAttachTTsk
(
SsTTskEntry *tTsk           /* pointer to TAPA task entry */
)
#else
PUBLIC S16 ssdAttachTTsk(tTsk)
SsTTskEntry *tTsk;          /* pointer to TAPA task entry */
#endif
{
   TRC0(ssdAttachTTsk);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   ssdDetachTTsk
 *
 *       Desc:  Does nothing.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 ssdDetachTTsk
(
SsTTskEntry *tTsk           /* pointer to TAPA task entry */
)
#else
PUBLIC S16 ssdDetachTTsk(tTsk)
SsTTskEntry *tTsk;          /* pointer to TAPA task entry */
#endif
{
   TRC0(ssdDetachTTsk);

   RETVALUE(ROK);
}


/*
*
*       Fun:   ssdCreateSTsk
*
*       Desc:  This function creates a system task. A thread is started
*              on the system task handler function defined later.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  rb_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdCreateSTsk
(
SsSTskEntry *sTsk           /* pointer to system task entry */
)
#else
PUBLIC S16 ssdCreateSTsk(sTsk)
SsSTskEntry *sTsk;          /* pointer to system task entry */
#endif
{
   TRC0(ssdCreateSTsk);

   RETVALUE(ROK);
}


/*
*
*       Fun:   ssdDestroySTsk
*
*       Desc:  This function destroys a system task. A terminate
*              event message is sent to the thread function.
*
*       Ret:   ROK      - ok
*
*       Notes:
*
*       File:  rb_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdDestroySTsk
(
SsSTskEntry *sTsk           /* pointer to system task entry */
)
#else
PUBLIC S16 ssdDestroySTsk(sTsk)
SsSTskEntry *sTsk;          /* pointer to system task entry */
#endif
{
   TRC0(ssdDestroySTsk);

   RETVALUE(ROK);
}


/*
*       Fun:   SThreadYield
*
*       Desc:  This function defers thread execution to any other ready
*              thread.
*
*       Ret:   ROK      - ok
*              RFAILED  - failure
*
*       Notes:
*
*       File:  rb_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SThreadYield
(
void
)
#else
PUBLIC S16 SThreadYield()
#endif
{

   TRC0(SThreadYield);

   RETVALUE(ROK);
}


/*
*
*       Fun:   Register timer
*
*       Desc:  This function is used to register a timer
*              function for the service user. System services
*              will invoke the timer activation function
*              passed to it at the specified intervals.
*
*       Ret:   ROK      - ok
*
*       Notes: Timing is handled by the common timers. The
*              ticks are handled by a thread that uses
*              nanosleep() And thus timing precision will Not
*              be very accurate.
*
*       File:  rb_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 ssdRegTmr
(
SsTmrEntry *tmr             /* pointer to timer entry */
)
#else
PUBLIC S16 ssdRegTmr(tmr)
SsTmrEntry *tmr;            /* pointer to timer entry */
#endif
{
#ifndef RB_TICK_TIMER
   CmTmrArg arg;
#endif

   TRC0(ssdRegTmr);

   tmr->dep.ticksToExpire = tmr->interval;

#ifndef RB_TICK_TIMER
   /* initialize common timers */
   cmInitTimers(tmr->dep.timers, TMR_DEF_MAX);

   /* start the timer */
   arg.tq     = osCp.dep.tmrTq;
   arg.tqCp   = &osCp.dep.tmrTqCp;
   arg.timers = tmr->dep.timers;
   arg.cb     = (PTR) tmr;
   arg.evnt   = TMR_DEF;
   arg.wait   = 0;
   arg.tNum   = NOTUSED;
   arg.max    = TMR_DEF_MAX;
   arg.wait   = tmr->interval;
   cmPlcCbTq(&arg);
#endif

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Deregister timer
 *
 *       Desc:  This function is used to deregister a timer function.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 ssdDeregTmr
(
SsTmrEntry *tmr             /* pointer to timer entry */
)
#else
PUBLIC S16 ssdDeregTmr(tmr)
SsTmrEntry *tmr;            /* pointer to timer entry */
#endif
{
#ifndef RB_TICK_TIMER
   CmTmrArg arg;
#endif

   TRC0(ssdDeregTmr);

   tmr->dep.ticksToExpire = 0;

#ifndef RB_TICK_TIMER
   /* stop the timer */
   arg.tq     = osCp.dep.tmrTq;
   arg.tqCp   = &osCp.dep.tmrTqCp;
   arg.timers = tmr->dep.timers;
   arg.cb     = (PTR) tmr;
   arg.evnt   = TMR_DEF;
   arg.wait   = 0;
   arg.tNum   = NOTUSED;
   arg.max    = TMR_DEF_MAX;
   arg.wait   = tmr->interval;
   cmRmvCbTq(&arg);
#endif

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Critical error
 *
 *       Desc:  This function is called when a critical error occurs.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 ssdError
(
Seq seq,                    /* sequence number */
Reason reason               /* reset reason */
)
#else
PUBLIC S16 ssdError(seq, reason)
Seq seq;                    /* sequence number */
Reason reason;              /* reset reason */
#endif
{
   Txt errBuf[256];

   TRC0(ssdError);

   /* set up the message to display */
   sprintf((char *)errBuf, "\n\nFATAL ERROR - errno = %d,"
         "reason = %d\n\n", seq, reason);
   SPrint(errBuf);

   /* won't reach here */
   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Log error
 *
 *       Desc:  This function is called to log an error.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC Void ssdLogError
(
Ent ent,                    /* Calling layer's entity id */
Inst inst,                  /* Calling layer's instance id */
ProcId procId,              /* Calling layer's processor id */
Txt *file,                  /* file name where error occured */
S32 line,                   /* line in file where error occured */
ErrCls errCls,              /* error class */
ErrCode errCode,            /* layer unique error code */
ErrVal errVal,              /* error value */
Txt *errDesc                /* description of error */
)
#else
PUBLIC Void ssdLogError(ent, inst, procId, file, line,
                        errCls, errCode, errVal, errDesc)
Ent ent;                    /* Calling layer's entity id */
Inst inst;                  /* Calling layer's instance id */
ProcId procId;              /* Calling layer's processor id */
Txt *file;                  /* file name where error occured */
S32 line;                   /* line in file where error occured */
ErrCls errCls;              /* error class */
ErrCode errCode;            /* layer unique error code */
ErrVal errVal;              /* error value */
Txt *errDesc;               /* description of error */
#endif
{
#ifndef DEBUGNOEXIT
   S16 i;
#endif
   Txt *errClsMsg;
   Txt errBuf[512];

   TRC0(ssdLogError);

   switch(errCls)
   {
      case ERRCLS_ADD_RES:
         errClsMsg = "ERRCLS_ADD_RES";
         break;

      case ERRCLS_INT_PAR:
         errClsMsg = "ERRCLS_INT_PAR";
         break;

      case ERRCLS_DEBUG:
         errClsMsg = "ERRCLS_DEBUG";
         break;

      case ERRCLS_FTHA:
         errClsMsg = "ERRCLS_FTHA";
         break;

      default:
         errClsMsg = "INVALID ERROR CLASS!";
         break;
   }

#ifdef ALIGN_64BIT
   sprintf((char *)errBuf,
         "\nrbss(posix): sw error:  ent: %03d  inst: %03d  proc id: %03d \n"
         "file: %s line: %03d errcode: %05d errcls: %s\n"
         "errval: %05d  errdesc: %s\n",
         ent, inst, procId, file, line, errCode, errClsMsg, errVal, errDesc);
#else
   sprintf((char *)errBuf,
         "\nrbss(posix): sw error:  ent: %03d  inst: %03d  proc id: %03d \n"
         "file: %s line: %03ld errcode: %05ld errcls: %s\n"
         "errval: %05ld  errdesc: %s\n",
         ent, inst, procId, file, line, errCode, errClsMsg, errVal, errDesc);
#endif
   SDisplay(0, errBuf);

   RETVOID;
}

#ifdef ENB_RELAY

/*
 *
 *       Fun:   Register driver task
 *
 *       Desc:  This function is called to register the handlers for a
 *              driver task.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 ssdRegDrvrTsk
(
SsDrvrTskEntry *drvrTsk         /* driver task entry */
)
#else
PUBLIC S16 ssdRegDrvrTsk(drvrTsk)
SsDrvrTskEntry *drvrTsk;        /* driver task entry */
#endif
{
   TRC0(ssdRegDrvrTsk);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Deregister driver task
 *
 *       Desc:  This function is called to deregister the handlers for a
 *              driver task.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 ssdDeregDrvrTsk
(
SsDrvrTskEntry *drvrTsk         /* driver task entry */
)
#else
PUBLIC S16 ssdDeregDrvrTsk(drvrTsk)
SsDrvrTskEntry *drvrTsk;        /* driver task entry */
#endif
{
   TRC0(ssdDeregDrvrTsk);

   RETVALUE(ROK);
}
#endif


/*
 *
 *       Fun:   ssdProcTTskTerm
 *
 *       Desc:
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef SS_MULTIPLE_PROCS
#ifdef ANSI
PUBLIC S16 ssdProcTTskTerm
(
ProcId procIdx,
SsTTskEntry *tTsk,
SsIdx idx
)
#else
PUBLIC S16 ssdProcTTskTerm(procIdx, tTsk, idx)
ProcId procIdx;
SsTTskEntry *tTsk;
SsIdx idx;
#endif
#else /*SS_MULTIPLE_PROCS*/
#ifdef ANSI
PUBLIC S16 ssdProcTTskTerm
(
SsTTskEntry *tTsk,
SsIdx idx
)
#else
PUBLIC S16 ssdProcTTskTerm(tTsk, idx)
SsTTskEntry *tTsk;
SsIdx idx;
#endif
#endif /*SS_MULTIPLE_PROCS*/
{
#ifdef SS_MULTIPLE_PROCS
   ProcId       proc;
#endif
   Ent          ent;
   Inst         inst;
   SsSTskEntry  *sTsk;
   S16          n;
   S16          ret;

   TRC0(ssdProcTTskTerm);

   ent  = tTsk->ent;
   inst = tTsk->inst;
   /*  We check the sTsk element; if it is not NULLP, the
    *  task is attached. So we have to detach it before
    *  deregistering the task.
    */
   ret = SLock(&osCp.sTskTblLock);
   if (ret != ROK)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB048, ERRZERO,
            "Could not lock system task table");
      RETVALUE(RFAILED);
   }
   SS_ACQUIRE_ALL_SEMA(&osCp.tTskTblSem, ret);
   if (ret != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      RBLOGERROR(ERRCLS_DEBUG, ERB049, ERRZERO,
            "Could not lock TAPA task table");
#endif
      if ( SUnlock(&osCp.sTskTblLock) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         RBLOGERROR(ERRCLS_DEBUG, ERB050, ERRZERO,
               "Could not Unlock system task table");
         RETVALUE(RFAILED);
#endif
      }

      RETVALUE(RFAILED);
   }

#ifdef SS_MULTIPLE_PROCS
   proc = tTsk->proc;
   if (tTsk->initTsk != NULLP)
   {
#ifndef USE_MEMCAL
      (Void)(*(tTsk->initTsk))(proc, ent, inst,
                               DFLT_REGION,
                               NRM_TERM,
                               &(osCp.tTskTbl[idx].xxCb));
#else
      (Void)(*(tTsk->initTsk))(proc, ent, inst,
                               SS_STATIC_REGION,
                               NRM_TERM,
                               &(osCp.tTskTbl[idx].xxCb));
#endif /* USE_MEMCAL */
   }
#endif /* SS_MULTIPLE_PROCS */

   if (tTsk->sTsk != NULLP)
   {
      sTsk = tTsk->sTsk;

      sTsk->dep.ent = ent;
      sTsk->dep.inst = inst;

      for (n = 0;  n < SS_MAX_TTSKS;  n++)
      {
         if (sTsk->tTsks[n] == idx)
         {
            sTsk->tTsks[n] = SS_INVALID_IDX;
            sTsk->numTTsks--;
            break;
         }
      }

      /* call the implementation to detach the task */
      ssdDetachTTsk(tTsk);
      /* 100178 */
      sTsk->dep.ent = ENTNC;
      sTsk->dep.inst = INSTNC;
   }

   /* Now we empty the entry for This task and update the table
    *  information
    */
#ifdef SS_MULTIPLE_PROCS
   osCp.tTskIds[procIdx][ent][inst] = SS_TSKNC;
#else /* SS_MULTIPLE_PROCS */
   osCp.tTskIds[ent][inst] = SS_TSKNC;
#endif /* SS_MULTIPLE_PROCS */

   tTsk->used    = FALSE;
#ifdef SS_MULTIPLE_PROCS
   tTsk->proc    = PROCNC;
#endif /* SS_MULTIPLE_PROCS */
   tTsk->ent     = ENTNC;
   tTsk->inst    = INSTNC;
   tTsk->tskType = TTUND;
   tTsk->initTsk = NULLP;
   tTsk->actvTsk = NULLP;
   tTsk->sTsk    = NULLP;

   tTsk->nxt = osCp.nxtTTskEntry;
   osCp.nxtTTskEntry = idx;
   osCp.numTTsks--;

#ifdef SS_MULTIPLE_PROCS
   /* mark the control block for This task as invalid */
   osCp.tTskTbl[idx].xxCb = NULLP;
#endif

   SS_RELEASE_ALL_SEMA(&osCp.tTskTblSem);
   if ( SUnlock(&osCp.sTskTblLock) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      RBLOGERROR(ERRCLS_DEBUG, ERB051, ERRZERO, "Could not Unlock system task table");
#endif
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

#ifdef SS_DRVR_SUPPORT

/*
 *
 *       Fun:   Interrupt service task handler
 *
 *       Desc:  This is the interrupt service task handler. It blocks
 *              on a pipe from which it reads an isFlag structure. The
 *              structure indicates which interrupt service task is to
 *              be executed. The thread identifies the task, calls the
 *              isTsk function and sends itself a message to repeat
 *              This operation until it receives a message to cease.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PRIVATE Void *rbIsTskHdlr
(
Ptr tskPtr                      /* pointer to task entry */
)
#else
PRIVATE Void *rbIsTskHdlr(tskPtr)
Ptr tskPtr;                     /* pointer to task entry */
#endif
{
   TRC0(rbIsTskHdlr);

   RETVALUE( (Void *) NULLP);
}
#endif /* SS_DRVR_SUPPORT */


/*
 *       Interface functions
 */

/*
 *
 *       Fun:   SDisplay
 *
 *       Desc:  This function displays a string to a given output
 *              channel.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: Buffer should be null terminated.
 *
 *              channel 0 is reserved for backwards compatibility
 *              with SPrint
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SDisplay
(
S16 chan,                   /* channel */
Txt *buf                    /* buffer */
)
#else
PUBLIC S16 SDisplay(chan, buf)
S16 chan;                   /* channel */
Txt *buf;                   /* buffer */
#endif
{
   S16       bufSz;
   SsMlogCb  *mLogCb;
   U32       coreId;

   TRC1(SDisplay);

   if (buf == NULLP)
   {
      RBLOGERROR(ERRCLS_INT_PAR, ERB052, ERRZERO, "Null pointer");
      RETVALUE(RFAILED);
   }

   //coreId = MxGetCpuID();
   coreId = 0;

   mLogCb = &osCp.dep.mLog[coreId];

   if (mLogCb->logStarted == TRUE)
   {
      bufSz = cmStrlen((CONSTANT U8 *)buf);
      rbMlogWrite(coreId, (U8 *) buf, bufSz);
   }
   else
   {
      uart_printf((char *)buf);
   }

   RETVALUE(ROK);
}

/*
 *
 *       Fun:   SFini
 *
 *       Desc:  function, shuts down.
 *
 *       Ret:   Void
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SFini
(
void
)
#else
PUBLIC S16 SFini()
#endif
{
   TRC1(SFini);

   RETVALUE(0);
}


/*
 *
 *       Fun:   Set date and time
 *
 *       Desc:  This function is used to set the calendar
 *              date and time.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: Unimplemented
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SSetDateTime
(
REG1 DateTime *dt           /* date and time */
)
#else
PUBLIC S16 SSetDateTime(dt)
REG1 DateTime *dt;          /* date and time */
#endif
{
   TRC1(SSetDateTime);

   UNUSED(dt);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Get date and time
 *
 *       Desc:  This function is used to determine the calendar
 *              date and time. This information may be used for
 *              some management functions.
 *
 *       Ret:   ROK      - ok
 *              RFAILED  - error
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SGetDateTime
(
REG1 DateTime *dt           /* date and time */
)
#else
PUBLIC S16 SGetDateTime(dt)
REG1 DateTime *dt;          /* date and time */
#endif
{
   TRC1(SGetDateTime);

   RETVALUE(ROK);
}

/*
 * Get time from epoch in milliseconds
 *
 *       Fun:   Get time from epoch in milliseconds
 *
 *       Desc:  This function is used to get the time from epoch in milli seconds.
 *              This information may be used for calculating a layer's activation function
 *              execution time used for thread profiling.
 *
 *       Ret:   ROK      - ok
 *              RFAILED  - error
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 */
#ifdef ANSI
PUBLIC S16 SGetEpcTime
(
EpcTime *et           /* date and time */
)
#else
PUBLIC S16 SGetEpcTime(et)
EpcTime *et;          /* date and time */
#endif
{
   TRC1(SEpcTime);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Get system time
 *
 *       Desc:  This function is used to determine the system time.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: osCp.dep.sysTicks is updated by the timer thread.
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SGetSysTime
(
Ticks *sysTime              /* system time */
)
#else
PUBLIC S16 SGetSysTime(sysTime)
Ticks *sysTime;             /* system time */
#endif
{
   TRC1(SGetSysTime);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (sysTime == NULLP)
   {
      RBLOGERROR(ERRCLS_INT_PAR, ERB053, ERRZERO, "Null pointer");
      RETVALUE(RFAILED);
   }
#endif

   *sysTime = osCp.dep.sysTicks;

   RETVALUE(ROK);
}

/*
 *
 *       Fun:   Get referenced time
 *
 *       Desc:  This function is used to determine the time in seconds
 *              And microseconds from a reference time.  The reference
 *              time is expressed in seconds from UTC EPOC, January 1,
 *              1970.
 *
 *       Ret:   ROK      - ok
 *              RFAILED  - fail
 *
 *       Notes: Macros are defined for reference times:
 *                 SS_REFTIME_01_01_1970
 *                 SS_REFTIME_01_01_2002
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SGetRefTime
(
U32 refTime,             /* reference time */
U32 *sec,
U32 *usec
)
#else
PUBLIC S16 SGetRefTime(refTime, sec, usec)
U32 refTime;             /* reference time */
U32 *sec;
U32 *usec;
#endif
{
   TRC1(SGetSysTime);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Get Random Number
 *
 *       Desc:  Invoked by layer when a pseudorandom number is required.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: Suggested approach uses shuffled Linear Congruential
 *              Operators as described in Byte magazine October
 *              1984; "Generating and Testing Pseudorandom Numbers"
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SRandom
(
Random *value               /* random number */
)
#else
PUBLIC S16 SRandom(value)
Random *value;              /* random number */
#endif
{
   TRC1(SRandom);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (value == NULLP)
   {
      RBLOGERROR(ERRCLS_INT_PAR, ERB054, (ErrVal)0 , "Null pointer");
      RETVALUE(RFAILED);
   }
#endif

   *value = (U16) (MxGetTicks() % RB_RANDOM_MAX);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Exit Task
 *
 *       Desc:  This function exits from a task.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: Currently does nothing.
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SExitTsk
(
void
)
#else
PUBLIC S16 SExitTsk()
#endif
{
   TRC1(SExitTsk);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Exit Interrupt
 *
 *       Desc:  This function exits from an interrupt.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: Currently does nothing.
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SExitInt
(
void
)
#else
PUBLIC S16 SExitInt()
#endif
{
   TRC1(SExitInt);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Hold Interrupt
 *
 *       Desc:  This function prohibits interrupts from being enabled until
 *              release interrupt. This function should be called when
 *              interrupts are disabled and prior to any call to system
 *              services either by entry to an interrupt service routine or
 *              by explicit call to disable interrupt.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: Currently does nothing
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SHoldInt
(
void
)
#else
PUBLIC S16 SHoldInt()
#endif
{
   TRC1(SHoldInt);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Release Interrupt
 *
 *       Desc:  This function allows interrupts to be enabled.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: Currently does nothing.
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SRelInt
(
void
)
#else
PUBLIC S16 SRelInt()
#endif
{
   TRC1(SRelInt);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   SEnbInt
 *
 *       Desc:  Enable interrupts
 *
 *       Ret:   ROK on success
 *              RFAILED on error
 *
 *       Notes: Currently does nothing.
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC INLINE S16 SEnbInt
(
void
)
#else
PUBLIC INLINE S16 SEnbInt()
#endif
{
   TRC1(SEnbInt);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   SDisInt
 *
 *       Desc:  Disable interrupts
 *
 *       Ret:   ROK on success
 *              RFAILED on error
 *
 *       Notes: Currently does nothing.
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC INLINE S16 SDisInt
(
void
)
#else
PUBLIC INLINE S16 SDisInt()
#endif
{
   TRC1(SDisInt);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Get Vector
 *
 *       Desc:  This function gets the function address stored at the
 *              specified interrupt vector.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: Currently does nothing.
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SGetVect
(
VectNmb vectNmb,                /* vector number */
PIF *vectFnct                   /* vector function */
)
#else
PUBLIC S16 SGetVect(vectNmb, vectFnct)
VectNmb vectNmb;                /* vector number */
PIF *vectFnct;                  /* vector function */
#endif
{
   TRC1(SGetVect);

   UNUSED(vectNmb);
   UNUSED(vectFnct);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Put Vector
 *
 *       Desc:  This function installs the specified function at the
 *              specified interrupt vector.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: Currently does nothing.
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SPutVect
(
VectNmb vectNmb,                /* vector number */
PIF vectFnct                    /* vector function */
)
#else
PUBLIC S16 SPutVect(vectNmb, vectFnct)
VectNmb vectNmb;                /* vector number */
PIF vectFnct;                   /* vector function */
#endif
{
   TRC1(SPutVect);

   UNUSED(vectNmb);
   UNUSED(vectFnct);

   RETVALUE(ROK);
}

#ifndef SS_MULTIPLE_PROCS

/*
 *
 *       Fun:   SGetEntInst
 *
 *       Desc:  This function gets the current entity and instance.
 *
 *       Ret:   ROK      - ok
 *              RFAILED  - failed, general (optional)
 *
 *       Notes: This function may be called by the OS or Layer 1
 *              hardware drivers.
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SGetEntInst
(
Ent *ent,                       /* entity */
Inst *inst                      /* instance */
)
#else
PUBLIC S16 SGetEntInst(ent, inst)
Ent *ent;                       /* entity */
Inst *inst;                     /* instance */
#endif
{
   TRC1(SGetEntInst);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check pointers */
   if (ent == NULLP  ||  inst == NULLP)
   {
      RBLOGERROR(ERRCLS_INT_PAR, ERB055, ERRZERO, "Null pointer");
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   SSetEntInst
 *
 *       Desc:  This function sets the current entity and instance.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SSetEntInst
(
Ent ent,                    /* entity */
Inst inst                   /* instance */
)
#else
PUBLIC S16 SSetEntInst(ent, inst)
Ent ent;                    /* entity */
Inst inst;                  /* instance */
#endif
{
   TRC1(SSetEntInst);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* check entity and instance IDs */
   if (ent >= ENTNC  ||  inst >= INSTNC)
   {
      RBLOGERROR(ERRCLS_INT_PAR, ERB056, ERRZERO, "Invalid entity/instance");
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE(ROK);
}
#endif /* SS_MULTIPLE_PROCS */

#ifdef SS_DRVR_SUPPORT

/*
 *
 *       Fun:   SSetIntPend
 *
 *       Desc:  Set interrupt pending flag
 *
 *       Ret:   ROK on success
 *              RFAILED on error
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC INLINE S16 SSetIntPend
(
U16 id,                         /* driver task identifier */
Bool flag                       /* flag */
)
#else
PUBLIC INLINE S16 SSetIntPend(id, flag)
U16 id;                         /* driver task identifier */
Bool flag;                      /* flag */
#endif
{
   TRC1(SSetIntPend);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (id >= SS_MAX_DRVRTSKS  ||  osCp.drvrTskTbl[id].used == FALSE)
   {
      RBLOGERROR(ERRCLS_INT_PAR, ERB057, id, "Invalid instance");
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE(ROK);
}
#endif  /* SS_DRVR_SUPPORT */


/*
 *
 *       Fun:   SRegInfoShow
 *
 *       Desc:  This function displays the memory usage information
 *              for the destined region. It will show the usage of
 *              each configured bucket and the heap for the specified region.
 *
 *       Ret:   ROK      OK
 *              RFAILED      Region Not registered
 *
 *       Notes: A Sample Output from the function
 *       Bucket Memory: region 1
 *       ====================================================
 *       Bucket  Number of Blks configured  Size  Allocated
 *       ====================================================
 *       0                     1             16         1
 *       1                     1             32         0
 *       2                     1             80         0
 *       3                     1            256         0
 *       4                     1            320         0
 *
 *       ---------------
 *       Heap Memory: region 1
 *       Heap Size: 0
 *       Heap Allocated: 0
 *       Heap Segmented blocks: 0
 *
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SRegInfoShow
(
Region region,
U32 *availmem
)
#else
PUBLIC S16 SRegInfoShow(region, availmem)
Region region;
U32 *availmem;
#endif
{
   U16   idx;
   Txt   prntBuf[100];

   TRC1(SRegInfoShow);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (region > (SS_MAX_REGS-1) )
   {
      RBLOGERROR(ERRCLS_INT_PAR, ERB058, ERRZERO, "Invalid Region");
      RETVALUE(RFAILED);
   }
#endif

   *availmem = 0;

#if 0
   sprintf((char *)prntBuf, "\n\nBucket Memory: region %d\n", region);
   SDisplay(0, prntBuf);
   sprintf((char *)prntBuf, "=================================================================\n");
   SDisplay(0, prntBuf);
   sprintf((char *)prntBuf, "Bucket  Number of Blks configured  Size  Allocated  Max Allocated\n");
   SDisplay(0, prntBuf);
   sprintf((char *)prntBuf, "==================================================================\n");
   SDisplay(0, prntBuf);
#endif

   for (idx = 0; idx < rbCMMRegCb[region]->numBkts; idx++)
   {
#ifdef ALIGN_64BIT
      sprintf((char *)prntBuf, "%2u              %8u          %5u  %8u  %8u\n",
            idx, rbCMMRegCb[region]->bktTbl[idx].numBlks,
            rbCMMRegCb[region]->bktTbl[idx].size,
            rbCMMRegCb[region]->bktTbl[idx].numAlloc,
            rbCMMRegCb[region]->bktTbl[idx].maxAlloc);
#else
      sprintf((char *)prntBuf, "%2u              %8lu          %5lu  %8lu  %8lu\n",
            idx, rbCMMRegCb[region]->bktTbl[idx].numBlks,
            rbCMMRegCb[region]->bktTbl[idx].size,
            rbCMMRegCb[region]->bktTbl[idx].numAlloc,
            rbCMMRegCb[region]->bktTbl[idx].maxAlloc);
#endif
      SDisplay(0, prntBuf);
      *availmem = *availmem + (rbCMMRegCb[region]->bktTbl[idx].size * \
            (rbCMMRegCb[region]->bktTbl[idx].numBlks -  \
             rbCMMRegCb[region]->bktTbl[idx].numAlloc));
   }
   sprintf((char *)prntBuf, "\n---------------\n");
   SDisplay(0, prntBuf);
   sprintf((char *)prntBuf, "Heap Memory: region %d\n", region);
   SDisplay(0, prntBuf);
#ifdef ALIGN_64BIT
   sprintf((char *)prntBuf, "Heap Size: %u\n", rbCMMRegCb[region]->heapSize);
#else
   sprintf((char *)prntBuf, "Heap Size: %lu\n", rbCMMRegCb[region]->heapSize);
#endif
   SDisplay(0, prntBuf);
#ifdef ALIGN_64BIT
   sprintf((char *)prntBuf, "Heap Allocated: %u\n",
         (rbCMMRegCb[region]->heapSize - rbCMMRegCb[region]->heapCb.avlSize));
#else
   sprintf((char *)prntBuf, "Heap Allocated: %lu\n",
         (rbCMMRegCb[region]->heapSize - rbCMMRegCb[region]->heapCb.avlSize));
#endif
   SDisplay(0, prntBuf);
   *availmem = *availmem + rbCMMRegCb[region]->heapCb.avlSize;
#if (ERRCLASS & ERRCLS_DEBUG)
   sprintf((char *)prntBuf, "Heap Segmented blocks: %d\n",
         rbCMMRegCb[region]->heapCb.numFragBlk);
   SDisplay(0, prntBuf);
#endif
   sprintf((char *)prntBuf, "UL Self Message Posted: %d\n", kwUlSelfMsgPosted);
   SDisplay(0, prntBuf);

   RETVALUE(ROK);
}
#ifdef ANSI
PUBLIC S16 SChkResUtl
(
Region region,
U8 *wSum
)
#else
PUBLIC S16 SChkResUtl(region, wSum)
Region region;
U8 *wSum;
#endif
{
   U16      idx;
   U32      bkt[RB_MAX_BKTS];
   CmMmBkt  memBkt;

   TRC1(SChkResUtl);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (region > (SS_MAX_REGS-1) )
   {
      RBLOGERROR(ERRCLS_INT_PAR, ERB058, ERRZERO, "Invalid Region");
      RETVALUE(RFAILED);
   }
#endif
   for (idx = 0; idx < rbCMMRegCb[region]->numBkts; idx++)
   {
      memBkt = rbCMMRegCb[region]->bktTbl[idx];
      bkt[idx]=((memBkt.numBlks - memBkt.numAlloc)*10)/memBkt.numBlks;
      if (bkt[idx] <= 4)
      {
         *wSum = 1;
         if(bkt[idx] <= 1)
         { 
            stop_printf("Available memory less than 10 percent in %d th bucket \n", idx);
         }
         RETVALUE(ROK);
      }
   }
   *wSum = 0;
   RETVALUE(ROK);
}

/*
 *
 *       Fun:   SGetRegInfo
 *
 *       Desc:  This function returns the memory usage information
 *              for the destined region. It will return the usage of
 *              each configured bucket and the heap for the specified region.
 *
 *       Ret:   ROK   OK
 *              RFAILED   Region not registered
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SGetRegInfo
(
Region region,
SsMemDbgInfo *dbgInfo
)
#else
PUBLIC S16 SGetRegInfo(region, dbgInfo)
Region region;
SsMemDbgInfo *dbgInfo;
#endif
{
   U32 idx;

   TRC1(SGetRegInfo);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (region >= rbMemoCfg.numRegions )
   {
      RBLOGERROR(ERRCLS_INT_PAR, ERB059, ERRZERO, "Invalid Region");
      RETVALUE(RFAILED);
   }
#endif

   dbgInfo->availmem = 0;

   if (rbCMMRegCb[region]->numBkts > SS_MAX_BKT_PER_DBGTBL)
      dbgInfo->numBkts = SS_MAX_BKT_PER_DBGTBL;
   else
      dbgInfo->numBkts = rbCMMRegCb[region]->numBkts;

   for (idx = 0; (idx < rbCMMRegCb[region]->numBkts) && (idx < SS_MAX_BKT_PER_DBGTBL); idx++)
   {
      dbgInfo->bktDbgTbl[idx].numBlks = rbCMMRegCb[region]->bktTbl[idx].numBlks;
      dbgInfo->bktDbgTbl[idx].size = rbCMMRegCb[region]->bktTbl[idx].size;
      dbgInfo->bktDbgTbl[idx].numAlloc = rbCMMRegCb[region]->bktTbl[idx].numAlloc;

      dbgInfo->availmem += (rbCMMRegCb[region]->bktTbl[idx].size * \
            (rbCMMRegCb[region]->bktTbl[idx].numBlks -  \
             rbCMMRegCb[region]->bktTbl[idx].numAlloc));
   }

   dbgInfo->region = region;

   dbgInfo->heapSize = rbCMMRegCb[region]->heapSize;

   dbgInfo->heapAlloc = (rbCMMRegCb[region]->heapSize - \
         rbCMMRegCb[region]->heapCb.avlSize);

   dbgInfo->availmem += rbCMMRegCb[region]->heapCb.avlSize;

#if (ERRCLASS & ERRCLS_DEBUG)
   dbgInfo->numFragBlk = rbCMMRegCb[region]->heapCb.numFragBlk;
#endif

   RETVALUE(ROK);
}

/*
 * as defined by SSI enhancements
 */
#ifdef SSI_DEBUG_LEVEL1

/*
 *
 *       Fun:   SPrintRegMemStatusInfo
 *
 *       Desc:  This function displays the memory usage information
 *              for the destined region. It will show the total memory
 *              used for static and dynamic memory if typeFlag is
 *              SS_MEM_BKT_ALLOC_PROFILE. It will show the number of
 *              memory block allocated for a particular size if typeFlag
 *              is SS_MEM_BLK_SIZE_PROFILE from the hash list by
 *              calling SRegPrintMemStats.
 *
 *       Ret:   ROK
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SPrintRegMemStatusInfo
(
Region region,
U8 typeFlag
)
#else
PUBLIC S16 SPrintRegMemStatusInfo(region, typeFlag)
Region region;
U8 typeFlag;
#endif
{
   Txt prntBuf[150];
   U32 idx;
   U32 statMemSize;
   U32 dynMemSize;

   TRC1(SPrintRegMemStatusInfo);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (region >= rbMemoCfg.numRegions )
   {
      RBLOGERROR(ERRCLS_INT_PAR, ERB060, ERRZERO, "Invalid Region");
      RETVALUE(RFAILED);
   }
#endif

   /* initialize the counters*/
   statMemSize = 0;
   dynMemSize = 0;

   if (typeFlag == SS_MEM_BKT_ALLOC_PROFILE)
   {
      /* total static and dynamic memory allocated from all the buckets in region requested */
      sprintf((char *)prntBuf, "\nAllocated Memory profile of Buckets from region: %d \n", region);
      SDisplay(0, prntBuf);
      sprintf((char *)prntBuf, "===========================================\n");
      SDisplay(0, prntBuf);
      sprintf((char *)prntBuf, "Bucket        Static Memory      Dynamic Memory\n");
      SDisplay(0, prntBuf);
      sprintf((char *)prntBuf, "===========================================\n");
      SDisplay(0, prntBuf);
      for (idx = 0; idx < rbCMMRegCb[region]->numBkts; idx++)
      {
#ifdef ALIGN_64BIT
         sprintf((char *)prntBuf, "%2u           %8u           %8u\n", idx,
               rbCMMRegCb[region]->bktTbl[idx].staticMemUsed,
               rbCMMRegCb[region]->bktTbl[idx].dynamicMemUsed);
#else
         sprintf((char *)prntBuf, "%2lu           %8lu           %8lu\n", idx,
               rbCMMRegCb[region]->bktTbl[idx].staticMemUsed,
               rbCMMRegCb[region]->bktTbl[idx].dynamicMemUsed);
#endif
         SDisplay(0, prntBuf);
         /* update the total count */
         statMemSize += rbCMMRegCb[region]->bktTbl[idx].staticMemUsed;
         dynMemSize += rbCMMRegCb[region]->bktTbl[idx].dynamicMemUsed;
      }
      /* from buckets */
#ifdef ALIGN_64BIT
      sprintf((char *)prntBuf, "Total Static Memory allocated from buckets: %u\n", statMemSize);
      SDisplay(0, prntBuf);
      sprintf((char *)prntBuf, "Total Dynamic Memory allocated from buckets: %u\n", dynMemSize);
#else
      sprintf((char *)prntBuf, "Total Static Memory allocated from buckets: %lu\n", statMemSize);
      SDisplay(0, prntBuf);
      sprintf((char *)prntBuf, "Total Dynamic Memory allocated from buckets: %lu\n", dynMemSize);
#endif
      SDisplay(0, prntBuf);
      /* from heap */
      sprintf((char *)prntBuf, "\n\nAllocated Memory profile from Heap of region: %d \n", region);
      SDisplay(0, prntBuf);
#ifdef ALIGN_64BIT
      sprintf((char *)prntBuf, "STATIC MEMORY: %u   DYNAMIC MEMORY:%u \n",
            rbCMMRegCb[region]->heapCb.staticHeapMemUsed,
            rbCMMRegCb[region]->heapCb.dynamicHeapMemUsed);
#else
      sprintf((char *)prntBuf, "STATIC MEMORY: %lu   DYNAMIC MEMORY:%lu \n",
            rbCMMRegCb[region]->heapCb.staticHeapMemUsed,
            rbCMMRegCb[region]->heapCb.dynamicHeapMemUsed);
#endif
      SDisplay(0, prntBuf);
   }
   else if (typeFlag == SS_MEM_BLK_SIZE_PROFILE)
   {
      /* Bucket Memory allocation Statistics */
      RETVALUE(SPrintRegMemStats(region));
   }
   else
   {
      /* error case */
      sprintf((char *)prntBuf, "\n Invalid choice \n");
      SDisplay(0, prntBuf);
   }

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   SPrintRegMemStats
 *
 *       Desc:  This function displays the memory usage information for
 *              the destined region. It will show the number of memory
 *              block allocated for a particular size from the hash list.
 *
 *       Ret:   ROK
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PRIVATE S16 SPrintRegMemStats
(
Region region
)
#else
PRIVATE S16 SPrintRegMemStats(region)
Region region;
#endif
{
   CmMmHashListCp *hashListCp;
   Txt prntBuf[150];
   U32 idx;
   U32 cntEnt;

   TRC1(SPrintRegMemStats);

   hashListCp = &rbCMMRegCb[region]->hashListCp;

   sprintf((char *)prntBuf, "\n\nSize Vs. NumAttempts and Alloc/Dealloc profile of region %d\n", region);
   SDisplay(0, prntBuf);
   sprintf((char *)prntBuf, "Maximum Entries: %u    Current Entries: %u\n",
         hashListCp->numOfbins, hashListCp->numOfEntries);
   SDisplay(0, prntBuf);
   sprintf((char *)prntBuf, "===================================\n");
   SDisplay(0, prntBuf);
   sprintf((char *)prntBuf, "Block Size      Total number of requests\n");
   SDisplay(0, prntBuf);
   sprintf((char *)prntBuf, "===================================\n");
   SDisplay(0, prntBuf);

   for (idx = 0, cntEnt=0; (cntEnt < hashListCp->numOfEntries) &&
         (idx < CMM_STAT_HASH_TBL_LEN); idx++)
   {
      if (hashListCp->hashList[idx].numAttempts)
      {
         cntEnt++;
#ifdef ALIGN_64BIT
         sprintf((char *)prntBuf, "%8u           %8u\n", hashListCp->hashList[idx].size,
               hashListCp->hashList[idx].numAttempts);
#else
         sprintf((char *)prntBuf, "%8lu           %8lu\n", hashListCp->hashList[idx].size,
               hashListCp->hashList[idx].numAttempts);
#endif
         SDisplay(0, prntBuf);
      }
   }

   sprintf((char *)prntBuf, "\nAllocation/De-allocation profile in Buckets\n");
   SDisplay(0, prntBuf);
   sprintf((char *)prntBuf, "=================================================\n");
   SDisplay(0, prntBuf);
   sprintf((char *)prntBuf, "Bucket    Num of Alloc Attempts    Num of De-alloc Attempts\n");
   SDisplay(0, prntBuf);
   sprintf((char *)prntBuf, "=================================================\n");
   SDisplay(0, prntBuf);

   /* Print the statistics of total number of alloc/de-alloc attempts in each bucket of this region */
   for (idx = 0; idx < rbCMMRegCb[region]->numBkts; idx++)
   {
#ifdef ALIGN_64BIT
      sprintf((char *)prntBuf, "%4u        %8u             %8u\n", idx,
            rbCMMRegCb[region]->bktTbl[idx].numAllocAttempts,
            rbCMMRegCb[region]->bktTbl[idx].numDeallocAttempts);
#else
      sprintf((char *)prntBuf, "%4lu        %8lu             %8lu\n", idx,
            rbCMMRegCb[region]->bktTbl[idx].numAllocAttempts,
            rbCMMRegCb[region]->bktTbl[idx].numDeallocAttempts);
#endif
      SDisplay(0, prntBuf);
   }
   sprintf((char *)prntBuf, "\nAllocation/De-allocation profile in Heap\n");
   SDisplay(0, prntBuf);
#ifdef ALIGN_64BIT
   sprintf((char *)prntBuf, "Num of Alloc Attempts: %u      Num of De-alloc Attempts: %u\n",
         rbCMMRegCb[region]->heapCb.numAllocAttempts,
         rbCMMRegCb[region]->heapCb.numDeallocAttempts);
#else
   sprintf((char *)prntBuf, "Num of Alloc Attempts: %lu      Num of De-alloc Attempts: %lu\n",
         rbCMMRegCb[region]->heapCb.numAllocAttempts,
         rbCMMRegCb[region]->heapCb.numDeallocAttempts);
#endif
   SDisplay(0, prntBuf);
   sprintf((char *)prntBuf, "\n");
   SDisplay(0, prntBuf);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   SRegMemErrHdlr
 *
 *       Desc:  This function handles the errors returned from the memory
 *              related functions. Customers are suggested to modify This
 *              API according to their specific requirement.
 *
 *       Ret:   ROK   OK
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC Void SRegMemErrHdlr
(
Region region,
Data *ptr,
S16 errCode
)
#else
PUBLIC Void SRegMemErrHdlr(region, ptr, errCode)
Region region;
Data *ptr;
S16 errCode;
#endif
{
   Txt prntBuf[150];

   TRC1(SRegMemErrHdlr);

   if (errCode == RDBLFREE)
   {
      sprintf((char *)prntBuf, "\nDouble free attempted at location:%8p  in region:%d\n", ptr, region);
      SDisplay(0, prntBuf);
   }
   else if (errCode == RTRAMPLINGNOK)
   {
      sprintf((char *)prntBuf, "\nMemory Trampling crossed Threshold in region:%d\n", region);
      SDisplay(0, prntBuf);
   }

   RETVOID;
}


/*
 *
 *       Fun:   SPrintRegMemProfile
 *
 *       Desc:  This function displays the memory profile information
 *              for the destined region. This function prints for:
 *              1) each memory bucket-Block address, size, size for which it is
 *                 allocated, free/allocated, static/dynamic
 *              2) heap - memory block address, size, requested size,
 *                 free/allocated, static/dynamic
 *
 *       Ret:   ROK   OK
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SPrintRegMemProfile
(
Region region
)
#else
PUBLIC S16 SPrintRegMemProfile(region)
Region region;
#endif
{
   CmMmHeapCb *heapCb;
   CmMmRegCb  *regCb;
   CmMmBlkHdr *curBktBlk;
   CmHEntry   *curHBlk;
   Size       offsetToNxtBlk;
   Size       hdrSize;
   Txt        prntBuf[250];
   U32        idx;
   U32        blkCnt;

   TRC1(SPrintRegMemProfile);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (region >= rbMemoCfg.numRegions )
   {
      RBLOGERROR(ERRCLS_INT_PAR, ERB061, ERRZERO, "Invalid Region");
      RETVALUE(RFAILED);
   }
#endif

   regCb = rbCMMRegCb[region];

   /* memory profile */
   sprintf((char *)prntBuf, "\n\nFull Memory Profile of region %d\n", region);
   SDisplay(0, prntBuf);

   /* bucket profile */
   sprintf((char *)prntBuf, "\nBucket Profile\n");
   SDisplay(0, prntBuf);

   for (idx = 0; idx < regCb->numBkts; idx++)
   {

#ifdef ALIGN_64BIT
      sprintf((char *)prntBuf, "\nBucket number:%4u  of Size:%u  Num of Blocks: %u\n",
            idx, regCb->bktTbl[idx].size, regCb->bktTbl[idx].numBlks);
#else
      sprintf((char *)prntBuf, "\nBucket number:%4lu  of Size:%lu  Num of Blocks: %lu\n",
            idx, regCb->bktTbl[idx].size, regCb->bktTbl[idx].numBlks);
#endif
      SDisplay(0, prntBuf);

      sprintf((char *)prntBuf, "==========================================================================\n");
      SDisplay(0, prntBuf);
      sprintf((char *)prntBuf, " Block    Location    Free/Allocated  Static/dynamic  Size requested\n");
      SDisplay(0, prntBuf);
      sprintf((char *)prntBuf, "==========================================================================\n");
      SDisplay(0, prntBuf);

      offsetToNxtBlk = regCb->bktTbl[idx].size + sizeof(CmMmBlkHdr);

      for (blkCnt=0, curBktBlk = (CmMmBlkHdr *)(regCb->bktTbl[idx].bktStartPtr);
            ((curBktBlk) && (blkCnt < regCb->bktTbl[idx].numBlks));
            curBktBlk = (CmMmBlkHdr *)((Data *)curBktBlk + offsetToNxtBlk), blkCnt++)
      {
#ifdef ALIGN_64BIT
         sprintf((char *)prntBuf, "%6u   %8p", blkCnt, (void *)curBktBlk);
#else
         sprintf((char *)prntBuf, "%6lu   %8p", blkCnt, (void *)curBktBlk);
#endif
         SDisplay(0, prntBuf);
         /* check if it is a sane block, elxe jump to next block */
         if (cmMmRegIsBlkSane(curBktBlk) != ROK)
         {
            sprintf((char *)prntBuf, "     Trampled                         \n");
            SDisplay(0, prntBuf);

            continue;
         }

         if (CMM_IS_STATIC(curBktBlk->memFlags))
         {
#ifdef ALIGN_64BIT
            sprintf((char *)prntBuf, "     Allocated     Static      %8u\n", curBktBlk->requestedSize);
#else
            sprintf((char *)prntBuf, "     Allocated     Static      %8lu\n", curBktBlk->requestedSize);
#endif
            SDisplay(0, prntBuf);
         }
         else if (CMM_IS_DYNAMIC(curBktBlk->memFlags))
         {
#ifdef ALIGN_64BIT
            sprintf((char *)prntBuf, "     Allocated       Dynamic      %8u\n", curBktBlk->requestedSize);
#else
            sprintf((char *)prntBuf, "     Allocated       Dynamic      %8lu\n", curBktBlk->requestedSize);
#endif
            SDisplay(0, prntBuf);
         }
         else if (CMM_IS_FREE(curBktBlk->memFlags))
         {
#ifdef ALIGN_64BIT
            sprintf((char *)prntBuf, "     Free                        %8u\n", curBktBlk->requestedSize);
#else
            sprintf((char *)prntBuf, "     Free                        %8lu\n", curBktBlk->requestedSize);
#endif
            SDisplay(0, prntBuf);
         }
         else
         {
            sprintf((char *)prntBuf, "     Trampled                         \n");
            SDisplay(0, prntBuf);
         }
      }
   }

   /* heap profile */
   sprintf((char *)prntBuf, "\nHeap Profile\n");
   SDisplay(0, prntBuf);

   /* point to heapCb */
   heapCb = &(regCb->heapCb);

   sprintf((char *)prntBuf, "\nHeap Start: %8p          Heap End: %8p\n", heapCb->vStart, heapCb->vEnd);
   SDisplay(0, prntBuf);
   sprintf((char *)prntBuf, "==========================================================================\n");
   SDisplay(0, prntBuf);
   sprintf((char *)prntBuf, " Block     Location      Size    Free/Allocated   Static/dynamic  Size requested\n");
   SDisplay(0, prntBuf);
   sprintf((char *)prntBuf, "==========================================================================\n");
   SDisplay(0, prntBuf);

   /* traverse the entire heap to output the heap profile */
   hdrSize = sizeof(CmHEntry);
   for (blkCnt=0, curHBlk = (CmHEntry *)heapCb->vStart;
         ((curHBlk) && (curHBlk < (CmHEntry *)heapCb->vEnd)); blkCnt++)
   {
#ifdef ALIGN_64BIT
      sprintf((char *)prntBuf, "%6u   %8p", blkCnt, (void *)curHBlk);
#else
      sprintf((char *)prntBuf, "%6lu   %8p", blkCnt, (void *)curHBlk);
#endif
      SDisplay(0, prntBuf);

      /* check if it is a sane block, elxe jump to next block */
      if (cmMmRegIsBlkSane((CmMmBlkHdr *)curHBlk) != ROK)
      {
         sprintf((char *)prntBuf, "                Trampled                         \n");
         SDisplay(0, prntBuf);

         sprintf((char *)prntBuf, "Trampled block encountered: Stopping heap profile\n");
         SDisplay(0, prntBuf);

         /*
          * To go to next block in the heap we do not have any offset value
          * other than curHBlk->size. As the block is already trampled
          * we cannot rely on this size. So it is better to stop here unless there
          * exists any other mechanism(?) to know the offset to next block.
          */
         RETVALUE(ROK);
      }

#ifdef ALIGN_64BIT
      sprintf((char *)prntBuf, "   %8u", curHBlk->size);
#else
      sprintf((char *)prntBuf, "   %8lu", curHBlk->size);
#endif
      SDisplay(0, prntBuf);

      if (CMM_IS_STATIC(curHBlk->memFlags))
      {
#ifdef ALIGN_64BIT
         sprintf((char *)prntBuf, "     Allocated       Static       %8u\n", curHBlk->requestedSize);
#else
         sprintf((char *)prntBuf, "     Allocated       Static       %8lu\n", curHBlk->requestedSize);
#endif
         SDisplay(0, prntBuf);
      }
      else if (CMM_IS_DYNAMIC(curHBlk->memFlags))
      {
#ifdef ALIGN_64BIT
         sprintf((char *)prntBuf, "     Allocated       Dynamic      %8u\n", curHBlk->requestedSize);
#else
         sprintf((char *)prntBuf, "     Allocated       Dynamic      %8lu\n", curHBlk->requestedSize);
#endif
         SDisplay(0, prntBuf);
      }
      else if (CMM_IS_FREE(curHBlk->memFlags))
      {
#ifdef ALIGN_64BIT
         sprintf((char *)prntBuf, "     Free                      %8u\n", curHBlk->requestedSize);
#else
         sprintf((char *)prntBuf, "     Free                      %8lu\n", curHBlk->requestedSize);
#endif
         SDisplay(0, prntBuf);
      }
      else
      {
         sprintf((char *)prntBuf, "     Trampled                         \n");
         SDisplay(0, prntBuf);
      }
      /* goto next block in the heap */
      curHBlk = (CmHEntry *)((Data *)curHBlk + hdrSize + curHBlk->size);

   }

   RETVALUE(ROK);
}
#endif /* SSI_DEBUG_LEVEL1 */


/*
 *
 *       Fun:   Get TimeStamp
 *
 *       Desc:  This function is used to Get TimeStamp in micro seconds
 *
 *       Ret:   ROK      - ok
 *              RFAILED  - error
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 SGetTimeStamp
(
S8    *ts
)
#else
PUBLIC S16 SGetTimeStamp(ts)
S8    *ts;
#endif
{
   TRC1(SGetTimeStamp);

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   Get SGetSystemTsk
 *
 *       Desc:  This function is used to Get sytem task id
 *
 *       Ret:   task id
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC U32 SGetSystemTsk
(
void
)
#else
PUBLIC U32 SGetSystemTsk()
#endif
{
   TRC1(SGetSystemTskS);

   RETVALUE(0);

} /* end of SGetSystemTsk */


/*
 *
 *       Fun:   rbICorePstTsk
 *
 *       Desc:  This function is used to Put Message to Message Queue
 *
 *       Ret:   ROK     : if Success
 *              RFAILED : if Failure
 *
 *       Notes:
 *
 *       File:  rb_ss.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbICorePstTsk
(
Pst *pst,                       /* post information */
Buffer *mBuf,                   /* message to post */
void *tsk
)
#else
PUBLIC S16 rbICorePstTsk(pst, mBuf, tsk)
Pst *pst;                       /* post information */
Buffer *mBuf;                   /* message to post */
void *tsk;
#endif
{
   S16 ret = 0;
   S16 dstCoreID = 0;
   S16 coreID = MxGetCpuID();
   SsTTskEntry *tTsk = (SsTTskEntry *)tsk;

   TRC1(rbICorePstTskS);

   dstCoreID = rbGetCoreId(pst->dstEnt, pst->dstInst);
   //RB_PRINTF("\n[%s:%d] sCore=%d dCore=%d sEnt=%d sInst=%d dEnt=%d dInst=%d mBuf=%p tsk=%p actvTsk=%p\n",__func__,__LINE__, coreID, dstCoreID, pst->srcEnt, pst->srcInst, pst->dstEnt, pst->dstInst, mBuf, tsk, tTsk->actvTsk);
   if(dstCoreID < 0)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB062, (ErrVal) 0,
            "Destination TAPA task is not registered");
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   /*rb001.101 Modified for Self Post related Fix*/
   ret = rbPutICoreMsg(dstCoreID, pst, mBuf);
   //RB_PRINTF("\n[%s:%d] sCore=%d dCore=%d sEnt=%d sInst=%d dEnt=%d dInst=%d mBuf=%p ret=%d\n",__func__,__LINE__, coreID, dstCoreID, pst->srcEnt, pst->srcInst, pst->dstEnt, pst->dstInst, mBuf, ret);

   RETVALUE(ret);
}


/********************************************************************30**

         End of file:     ju_ss.c@@/main/1 - Mon Aug 29 22:31:34 2011

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
/main/1      rb001.101    sasahni   1. Modified for Self Post related Fix
                                    2. Fixing calculation of size for 
                                       allocating heap for vAddr
*********************************************************************91*/
