/********************************************************************16**

                         (c) COPYRIGHT 1989-2005 by 
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
                    9380, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/

/********************************************************************20**
  
     Name:     common - linked list functions
  
     Type:     C include file
  
     Desc:     Structures, variables and typedefs required by the
               linked list management routines.
 
     File:     cm_llist.x
  
     Sid:      cm_llist.x@@/main/7 - Tue Aug 28 12:30:53 2007
    
     Prg:      ak
 
*********************************************************************21*/
  
#ifndef __CMLLISTX__
#define __CMLLISTX__

#ifdef __cplusplus
extern "C" {
#endif


typedef struct cmLList CmLList;
typedef struct cmLListCp CmLListCp;

/* doubly linked list */
struct cmLList
{
   CmLList *next;        /* next */
   CmLList *prev;        /* previous */
   PTR    node;          /* node */
};

struct cmLListCp
{
   CmLList *first;       /* first entry in list */
   CmLList *last;        /* last entry in list */
   CmLList *crnt;        /* entry last accessed */
   U32     count;        /* number of entries */
};

EXTERN Void     cmLListInit     ARGS ((CmLListCp *lList));
EXTERN Void     cmLListAdd2Tail ARGS ((CmLListCp *lList, CmLList *node));
EXTERN Void     cmLListInsCrnt  ARGS ((CmLListCp *lList, CmLList *node));
/* cm_llist_x_001.main_6 - Add function */
EXTERN Void     cmLListInsAfterCrnt  ARGS ((CmLListCp *lList, CmLList *node));
EXTERN CmLList *cmLListDelFrm   ARGS ((CmLListCp *lList, CmLList *node));

#ifdef __cplusplus
}
#endif

#endif /* __CMLLISTX__ */

 
/********************************************************************30**
  
         End of file:     cm_llist.x@@/main/7 - Tue Aug 28 12:30:53 2007
  
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
1.1          ---  ak    1. initial release

*********************************************************************81*/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
1.2          ---      bbk  1. Changed copyright header date.
1.3          ---      apr  1. Added support for c++ compiler
  
1.4          ---      mb   1. Changed copyright header date (1999).
/main/6      ---      rbabu 1. Updated for NBAP software release 1.2
/main/7      ---      cm_llist_x_001.main_6      mn   1. Add function cmLListInsAfterCrnt
*********************************************************************91*/