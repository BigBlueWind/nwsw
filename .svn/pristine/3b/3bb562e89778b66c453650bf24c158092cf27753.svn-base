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


/**********************************************************************
  
     Name:     LTE eNodeB Sample Software Identifier
  
     Type:     C source file
  
     Desc:     Structure for software part number, version, and
               revision.

     File:     ve_debug.c
  
     Sid:      ve_debug.c
 
     Prg:      rkumara
  
**********************************************************************/

/* Added debug functionality */

#ifndef _VE_DEBUG_H_
#define _VE_DEBUG_H_


/* g++ warning fix */
#define ENB_DEBUG_LOG_LEVEL         DEBUG_LEVEL
#define ENB_DEBUG_MSG_FPA_POOL         (2)
#define ENB_DEBUG_MSG_FPA_POOL_BSIZE   (1024)
#define ENB_DEBUG_MSG_FPA_POOL_BLOCKS  (1500)
#define ENB_DEBUG_MSG_MAX_SIZE         (ENB_DEBUG_MSG_FPA_POOL_BSIZE)

#define OCTEON_CORE_DEBUG               (3)

/* QOS to be used for WQE priority */
#define WORK_QUEUE_QOS_0      (0) /* DL & UL_High_Prio Messages */
#define WORK_QUEUE_QOS_1      (1) /* UL Low Priority Messages */
#define WORK_QUEUE_QOS_2      (2)   /* Debug Messages */

#ifdef LTE_ENB_PAL 
enum
{
   UL_msg_tag_val       = 1002,
   DL_msg_tag_val       = 1003,
   TTI_ind_tag_val   = 1004,
   Debug_msg_tag_val    = 1005,
   Debug_data_tag_val   = 1006
};
#endif

/* g++ warning fix */
enum enb_dbg_level
{
   ENB_DBG_INFO   = 0,
   ENB_DBG_SUMMARY,
    ENB_DBG_WARNING,
    ENB_DBG_ERROR,
};

inline void vePstDebugMsg(const char* format,...);

inline void vePstDebugWqe(const cvmx_wqe_t *msg_wqe);
void VePstDebugData(const cvmx_wqe_t *msg_wqe); /* g++ */
void debug_core_main(void);


#define ENB_DEBUG_MSG(_log_lvl, _format, ...) ({ \
   if (1) {\
   if (_log_lvl >= 0/*ENB_DEBUG_LOG_LEVEL*/) \
      vePstDebugMsg(_format, ##__VA_ARGS__);\
      }\
})

/* 4GMX SSI changes */
#ifndef SS_4GMX_SSI_PORTING
#define DEBUG_WQE(_log_lvl, _msg_wqe) ({ \
   if ((_log_lvl) >= ENB_DEBUG_LOG_LEVEL ) \
   { \
      vePstDebugWqe(_msg_wqe);\
   } \
})
#endif

#ifndef SS_4GMX_SSI_PORTING
#endif

#endif /* _VE_DEBUG_H_ */

/********************************************************************30**

         End of file:     ve_debug.h@@/main/1 - Wed Dec  1 08:44:25 2010

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
------------ -------- ---- ----------------------------------------------
$SID$     ----       mnawas        1. eNodeb 3.1 Release
*********************************************************************91*/
