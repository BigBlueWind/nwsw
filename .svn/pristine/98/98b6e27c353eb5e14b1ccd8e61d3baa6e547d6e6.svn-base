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

     Name:

     Type:     C File.

     Desc:     4GMX memory logger.

     File:     rb_log.c

     Sid:      ju_log.c@@/main/1 - Mon Aug 29 22:31:33 2011

     Prg:

************************************************************************/

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "rb_include.h"
#include "cm_lib.x"

#ifdef SS_4GMX_LCORE

/*
 *
 *     Fun  : rbMlogInit
 *
 *     Desc : Initializes the logging stream for given core.
 *
 *     Ret:   ROK - SUCCESS
 *            RFAILED - FAILURE
 *
 *     Notes: None
 *
 *     File : rb_log.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbMlogInit
(
U8   coreId
)
#else
PUBLIC S16 rbMlogInit (coreId)
U8   coreId;
#endif
{
   SsMlogCb   *mLogCb;

   TRC2(rbMlogInit);

   mLogCb         = &osCp.dep.mLog[coreId];
   mLogCb->logBuf = (SsMLogBuf*) MxHeapAlloc(NcnbHeap, sizeof(SsMLogBuf));

   if (mLogCb->logBuf == NULLP)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB043, ERRZERO,
            "\n rbMlogInit allocation for CCPU MLOG failed\n");\
      RETVALUE(RFAILED);
   }
   cmMemset((U8*)mLogCb->logBuf->buf, ' ', sizeof(mLogCb->logBuf->buf));

   /* Initialize the following - the reader shall use these values */
   mLogCb->logBuf->rPtr    = 0;
   mLogCb->logBuf->blkSize = MLOG_MAX_BLOCK_SIZE;
   mLogCb->logBuf->nmbBlks = MLOG_MAX_BLOCKS;
   mLogCb->logStarted      = TRUE;
   mLogCb->wPtr            = 0;

   uart_printf("\n ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n");
   uart_printf("\n CCPU MLOG (%d) Address (0x0%x) Size(%d)\n",
         coreId, mLogCb->logBuf->buf, sizeof(mLogCb->logBuf->buf));
   uart_printf("\n ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n");

   RETVALUE(ROK);
}

__attribute__((section(".NCNBSECT."))) unsigned int Locked;


/*
 *
 *     Fun  : rbMlogWrite
 *
 *     Desc : Writes the input log string to logger stream of given core
 *
 *     Ret:   ROK - SUCCESS
 *            RFAILED - FAILURE
 *
 *     Notes: None
 *
 *     File : rb_log.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbMlogWrite
(
U8    coreId,
U8    *src,
U32   size
)
#else
PUBLIC S16 rbMlogWrite (coreId, src, size)
U8    coreId;
U8    *src;
U32   size;
#endif
{
   SsMlogCb   *mLogCb;
   int irq;
   S8  *endmarker = "<END>\n";
   U32  endlen = 6;
   U32  actualsize = size;
   U32  wPtr;

   TRC2(rbMlogWrite);

   mLogCb = &osCp.dep.mLog[coreId];
   size += endlen;

#ifdef ERRCLASS
   if (mLogCb->logBuf == NULLP)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB044, ERRZERO,
            "\n rbMlogWrite: logging buffer not allocated \n");
      RETVALUE(RFAILED);
   }
#endif
   if (size > 1024)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB045, ERRZERO,
            "\nERROR - rbMlogWrite: Printing more that 1024 bytes \n");
      RETVALUE(RFAILED);
   }
   irq = ARM_INT_disable();
   MxiGetLock(&Locked);
   wPtr = mLogCb->wPtr;
   /* Check if we are going over the MAX */
   if (wPtr + size > MLOG_MAX_BLOCK_SIZE * MLOG_MAX_BLOCKS)
   {
      /* We have to clean up the current read pointer block */
      cmMemset((U8*)(mLogCb->logBuf->buf + wPtr), ' ',
            MLOG_MAX_BLOCK_SIZE * MLOG_MAX_BLOCKS - wPtr);
	  wPtr = 0;
   }

   /* Write into memory */
   SMemCpy((Void *)(&mLogCb->logBuf->buf[wPtr]), (Void *)src, (size_t)actualsize);
   SMemCpy((Void *)(&mLogCb->logBuf->buf[wPtr+=actualsize]), (Void *)endmarker,
   	   (size_t)endlen);

   mLogCb->wPtr = wPtr;

   MxReleaseLock(&Locked);
   ARM_INT_restore(irq);
   RETVALUE(ROK);
}


/*
 *
 *     Fun  : rbMlogDeInit
 *
 *     Desc : Deinitializes the logger stream of given core
 *
 *     Ret:  ROK - SUCCESS
 *           RFAILED - FAILURE
 *
 *     Notes: None
 *
 *     File : rb_log.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbMlogDeInit
(
U8    coreId
)
#else
PUBLIC S16 rbMlogDeInit (coreId)
U8    coreId;
#endif
{
   SsMlogCb  *mLogCb;
   S16       ret;

   TRC2(rbMlogDeInit);

   mLogCb = &osCp.dep.mLog[coreId];

   ret = MxHeapFree(NcnbHeap, mLogCb->logBuf);

   if (ret != ROK)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB046, ERRZERO,
            "\n Unable to free CCPU MLOG buffer \n");
      RETVALUE(RFAILED);
   }

   mLogCb->logStarted = FALSE;
   mLogCb->wPtr       = 0;
   mLogCb->logBuf     = NULLP;

   RETVALUE(ROK);
}

#define RB_CALLSTACK_SIZE_16B  20
#define RB_CALLSTACK_SIZE      ((RB_CALLSTACK_SIZE_16B) * 4)

typedef struct _rbCallstack
{
   U32  top;
   U32  bad;  /* stack is bad */
   U32  res[2];
   U32  stack[RB_CALLSTACK_SIZE];
} RbCallstack;

__attribute__((section(".NCNBSECT."))) RbCallstack rbCallstacks[4];

void rbCallstackPush(U32 ret, U32 sp)
{
   U32  numPushes = 2;
   U32  coreId    = MxGetCpuID();
   U32  top       = rbCallstacks[coreId].top;
   U32 *stack     = rbCallstacks[coreId].stack;
   if (top <= RB_CALLSTACK_SIZE - numPushes)
   {
      stack[top++] = ret;
      stack[top++] = sp;
      rbCallstacks[coreId].top = top;
      return;
   }
   MSPD_DBG("Warning: Recorded stack too deep, increase RB_CALLSTACK_SIZE_16B(%u),"
         " caller 0x%x\n", RB_CALLSTACK_SIZE_16B, RB_GET_CALLER());
   rbCallstacks[coreId].bad = ~0;
}

void rbCallstackPop()
{
   U32  numPops = 2;
   U32  coreId    = MxGetCpuID();
   U32  top = rbCallstacks[coreId].top;
   if (top >= numPops - 1)
   {
      rbCallstacks[coreId].top -= numPops;
      return;
   }
   MSPD_DBG("Warning: Not enough info to pop, haven't pushed?\n");
   rbCallstacks[coreId].bad = ~0;
}

void rbCallstackShowForCore(U32 coreId)
{
   U32  numPops = 2;
   U32  top     = rbCallstacks[coreId].top;
   U32 *stack   = rbCallstacks[coreId].stack;
   if (rbCallstacks[coreId].bad)
      MSPD_DBG("Warning: Call stack is not reliable\n");
   MSPD_DBG("On core %u, call stack for core %u (showing: caller sp)\n", MxGetCpuID(), coreId);
   while (top > numPops - 1)
   {
      MSPD_DBG_RAW("    0x%x  0x%x\n", stack[top-2], stack[top-1]);
      top -= numPops;
   }
   if (top > 0)
   {
      MSPD_DBG("Warning: Call stack not correct, excess %u\n", top);
      rbCallstacks[coreId].bad = ~0;
   }
}

void rbCallstackShow()
{
   rbCallstackShowForCore(MxGetCpuID());
}

#endif


/********************************************************************30**

         End of file:     ju_log.c@@/main/1 - Mon Aug 29 22:31:33 2011

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
