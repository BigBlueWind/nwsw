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
  
     Name:     multi-thread (unix)
  
     Type:     C include file
  
     Desc:     Defines required by mt ss implementation
  
     File:     mt.h
  
     Sid:      mt.h@@/main/1 - Mon Apr  5 18:19:16 2010

     Prg:      ada
  
*********************************************************************21*/

#ifndef __MTH__
#define __MTH__


/* defines */

/*
 * TUNE THESE TO YOUR ENVIRONMENT 
 */
#define MT_MAX_TSKCNT       5   /* maximum # of tasks per thread */
#define MT_MAX_TASK         40  /* maximum # of activation tasks */
#define MT_MAX_DRVR         5   /* maximum # of drivers */
#define MT_MAX_TIMER        40  /* maxumum # of timers */
#define MT_MAX_SIG_HNDLRS   5   /* number of signal handlers (Vects) */
#define MT_MAX_EXIT_FNCTS   5   /* number of "AtExit" functions */
#define MT_MAX_MUTEX        40  /* maximum # of mutexes */
#define MT_MAX_COND         40  /* maximum # of conditions */
#define MT_MAX_THREAD       60  /* maximum # of threads */

#define MT_NMB_MSG_POOL     4

/* pool three used for message buffers (mBufs) */
#define MT_POOL_3_ID        MT_MBUF_POOL
#define MT_POOL_3_NMB_BUFS  1000
#define MT_POOL_3_DSIZE     0
#define MT_POOL_3_STRTIDX   0
#define MT_POOL_3_ENDIDX    0

/* pool two used for timer buffers (dBufs) */
#define MT_POOL_2_ID        MT_TMR_POOL
#define MT_POOL_2_NMB_BUFS  200
#define MT_POOL_2_DSIZE     32
#define MT_POOL_2_STRTIDX   16
#define MT_POOL_2_ENDIDX    (MT_POOL_2_DSIZE-1)

/* pool one used for wrapper protocol traffic (dBufs) */
#define MT_POOL_1_ID        MT_WRAP_POOL
#define MT_POOL_1_NMB_BUFS  300
#define MT_POOL_1_DSIZE     1600 /* 1576 max ethernet packet */
#define MT_POOL_1_STRTIDX   100
#define MT_POOL_1_ENDIDX    (MT_POOL_1_DSIZE-1)

/* pool zero used for normal protocol traffic (dBufs) */
#define MT_POOL_0_ID        MT_PROT_POOL
#define MT_POOL_0_NMB_BUFS  1000
#define MT_POOL_0_DSIZE     1000
#define MT_POOL_0_STRTIDX   300
#define MT_POOL_0_ENDIDX    (MT_POOL_0_DSIZE-1)

#define MT_SPOOL_SIZE       0x80000 /* static pool size (512K) */

#define MT_MSG_POOL_UNINITIALIZED     0
#define MT_MSG_POOL_INITIALIZED       1

#define MT_TMR_EVNT            1
#define MT_MSG_EVNT            2

#define MT_TSK_NULL            0
#define MT_TSK_NORM            1
#define MT_TSK_PERM            2

#define MT_THRDID_NULL         -1
#define MT_TMRID_NULL          -1

#define MT_TICK_CNT         0x186A0      /* how many micro sec per 
                                            .1 seconds */

#define MT_TQNUMENT         64           /* # of buckets */

#ifndef NOCONAVL
#define CONAVL                           /* console available? */
#endif  /* NOCONAVL */

#endif /* __MTH__ */

  
/********************************************************************30**
  
         End of file:     mt.h@@/main/1 - Mon Apr  5 18:19:16 2010

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
1.1          ---  ada    1. initial release

*********************************************************************81*/

