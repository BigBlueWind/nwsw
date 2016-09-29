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

     Name:     multi-thread (unix)

     Type:     C include file

     Desc:     Defines required by mt ss implementation

     File:     rb.h

     Sid:      rb.h@@/main/1 - Mon Aug 29 22:31:34 2011

     Prg:      

*********************************************************************21*/

#ifndef __RBH__
#define __RBH__


/* defines */

/*
 * TUNE THESE TO YOUR ENVIRONMENT 
 */
#define RB_MAX_TSKCNT       5   /* maximum # of tasks per thread */
#define RB_MAX_TASK         40  /* maximum # of activation tasks */
#define RB_MAX_DRVR         5   /* maximum # of drivers */
#define RB_MAX_TIMER        40  /* maxumum # of timers */
#define RB_MAX_SIG_HNDLRS   5   /* number of signal handlers (Vects) */
#define RB_MAX_EXIT_FNCTS   5   /* number of "AtExit" functions */
#define RB_MAX_MUTEX        40  /* maximum # of mutexes */
#define RB_MAX_COND         40  /* maximum # of conditions */
#define RB_MAX_THREAD       60  /* maximum # of threads */

#define RB_NMB_MSG_POOL     4

/* pool three used for message buffers (mBufs) */
#define RB_POOL_3_ID        RB_MBUF_POOL
#define RB_POOL_3_NMB_BUFS  1000
#define RB_POOL_3_DSIZE     0
#define RB_POOL_3_STRTIDX   0
#define RB_POOL_3_ENDIDX    0

/* pool two used for timer buffers (dBufs) */
#define RB_POOL_2_ID        RB_TMR_POOL
#define RB_POOL_2_NMB_BUFS  200
#define RB_POOL_2_DSIZE     32
#define RB_POOL_2_STRTIDX   16
#define RB_POOL_2_ENDIDX    (RB_POOL_2_DSIZE-1)

/* pool one used for wrapper protocol traffic (dBufs) */
#define RB_POOL_1_ID        RB_WRAP_POOL
#define RB_POOL_1_NMB_BUFS  300
#define RB_POOL_1_DSIZE     1600 /* 1576 max ethernet packet */
#define RB_POOL_1_STRTIDX   100
#define RB_POOL_1_ENDIDX    (RB_POOL_1_DSIZE-1)

/* pool zero used for normal protocol traffic (dBufs) */
#define RB_POOL_0_ID        RB_PROT_POOL
#define RB_POOL_0_NMB_BUFS  1000
#define RB_POOL_0_DSIZE     1000
#define RB_POOL_0_STRTIDX   300
#define RB_POOL_0_ENDIDX    (RB_POOL_0_DSIZE-1)

#define RB_SPOOL_SIZE       0x80000 /* static pool size (512K) */

#define RB_MSG_POOL_UNINITIALIZED     0
#define RB_MSG_POOL_INITIALIZED       1

#define RB_TMR_EVNT            1
#define RB_MSG_EVNT            2

#define RB_TSK_NULL            0
#define RB_TSK_NORM            1
#define RB_TSK_PERM            2

#define RB_THRDID_NULL         -1
#define RB_TMRID_NULL          -1

#define RB_TICK_CNT         0x186A0      /* how many micro sec per 
                                            .1 seconds */

#define RB_TQNUMENT         64           /* # of buckets */

#ifndef NOCONAVL
#define CONAVL                           /* console available? */
#endif  /* NOCONAVL */

#endif /* __RBH__ */


/********************************************************************30**

         End of file:     rb.h@@/main/1 - Mon Aug 29 22:31:34 2011

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
