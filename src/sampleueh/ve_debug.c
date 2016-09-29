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
#ifndef SS_4GMX_UCORE
#include "cvmx-config.h"
#include "cvmx.h"
#include "cvmx-pow.h"
#include "cvmx-fpa.h"
#include "cvmx-helper-fpa.h"
#include "ve_sm_init.h"
#include "ve_debug.h"

#ifdef ANSI
void vePstDebugMsg
(
const char* format, 
...
)
#else
void vePstDebugMsg(const char* format, ...)
const char* format; 
...;
#endif
{
   va_list arg;

   /* Allocate buffer to store debug message */
   char *debug_buf =  (char*)cvmx_fpa_alloc(ENB_DEBUG_MSG_FPA_POOL);/* g++ compilation fix */

   /* If FPA POOL is not setup yet, use printf */
      /* Prepare debug message buffer */
      va_start(arg, format);
      vsprintf(debug_buf, format, arg);
      va_end(arg);

      /* Create and Send Debug message WQE */
      cvmx_wqe_t *msg_wqe = (cvmx_wqe_t *)cvmx_fpa_alloc(CVMX_FPA_WQE_POOL);/* g++ compilation fix */

      if(msg_wqe)
      {

         msg_wqe->grp             = OCTEON_CORE_DEBUG;
         msg_wqe->len             = strlen(debug_buf);
         msg_wqe->tag_type          = CVMX_POW_TAG_TYPE_NULL;
         msg_wqe->tag             = Debug_msg_tag_val;
         msg_wqe->qos             = WORK_QUEUE_QOS_2;
         msg_wqe->packet_ptr.s.addr    = cvmx_ptr_to_phys((uint8_t *)debug_buf);
         msg_wqe->packet_ptr.s.pool    = ENB_DEBUG_MSG_FPA_POOL;
         msg_wqe->packet_data[4]      = cvmx_get_core_num();

         /* Submit message WQE */
         cvmx_pow_work_submit(msg_wqe, msg_wqe->tag, msg_wqe->tag_type, \
                        msg_wqe->qos, msg_wqe->grp);
      }
      else
      {
         printf("DEBUG_ERROR: Debug message wqe allocation failed.\n");
      }
}

#ifdef ANSI
void VePstDebugData
(
const cvmx_wqe_t *msg_wqe
)
#else
void VePstDebugData(msg_wqe)
const cvmx_wqe_t *msg_wqe;
#endif
{
   uint8_t* debug_buf =  (uint8_t*)cvmx_fpa_alloc(msg_wqe->packet_ptr.s.pool);/* g++ compilation fix */
   if(debug_buf != NULL)
   {
      memcpy(debug_buf, cvmx_phys_to_ptr(msg_wqe->packet_ptr.s.addr), msg_wqe->len);

      /* Create and Send Debug message WQE */
      cvmx_wqe_t *dbg_msg_wqe = (cvmx_wqe_t *)cvmx_fpa_alloc(CVMX_FPA_WQE_POOL);/* g++ compilation fix */

      if(dbg_msg_wqe)
      {

         dbg_msg_wqe->grp             = OCTEON_CORE_DEBUG;
         dbg_msg_wqe->len             = msg_wqe->len;
         dbg_msg_wqe->tag_type          = CVMX_POW_TAG_TYPE_NULL;
         dbg_msg_wqe->tag             = Debug_data_tag_val;
         dbg_msg_wqe->qos             = WORK_QUEUE_QOS_2;

         dbg_msg_wqe->packet_ptr.s.addr    = cvmx_ptr_to_phys(debug_buf);
         dbg_msg_wqe->packet_ptr.s.pool    = msg_wqe->packet_ptr.s.pool;

         memcpy(dbg_msg_wqe->packet_data, msg_wqe->packet_data, 4);
         dbg_msg_wqe->packet_data[4]      = cvmx_get_core_num();

         /* Submit Debug message WQE */
         cvmx_pow_work_submit(dbg_msg_wqe, dbg_msg_wqe->tag, \
            dbg_msg_wqe->tag_type, dbg_msg_wqe->qos, dbg_msg_wqe->grp);
      }
      else
      {
         printf("DEBUG_ERROR: Debug message wqe allocation failed.\n");
      }
   }
}
#endif
/********************************************************************30**

         End of file:     ve_debug.c@@/main/1 - Wed Dec  1 08:44:25 2010

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
