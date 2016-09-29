/********************************************************************16**

                         (c) COPYRIGHT 2006 by 
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
  
     Name:     Sample Application
  
     Type:     C source file
  
     Desc:     This file implements Circular Queue
  
     File:     
  
     Sid:      
         
     Prg:      an
  
*********************************************************************21*/

#ifndef _UTIL_CIRC_Q_IMPL_H_
#define _UTIL_CIRC_Q_IMPL_H_

#define UTIL_CIRC_Q_MAXSIZE     32
#define UTIL_CIRC_Q_MAXDATASIZE 1000 /* should be the same as supported by L1 */
#define TRUE 1
#define FALSE 0

/*ys004.102 :  Merged MSPD code with phy 1.7 */

typedef struct _utilCircQData
{
   U32  len;
   U8   data[YS_MAX_RCVD_BUF_LEN];
}UtilCircQData;

typedef struct _utilCircQ
{
   U32 front;
   U32 rear;
   UtilCircQData elements[UTIL_CIRC_Q_MAXSIZE+1];
}UtilCircQ;

typedef struct _ysStCb
{
   CmTqCp        tmrTqCp;
   CmTqType      tmrTq[YS_TQ_SIZE];
   CmTimer       tmr;
}YsStCb;

EXTERN UtilCircQ      circQueue;
EXTERN UtilCircQData* utilCircQFront(UtilCircQ *queue);
EXTERN Void utilCircQDequeue(UtilCircQ *queue);
EXTERN Void utilCircQMakeNull(UtilCircQ *queue);
EXTERN U32 utilCircQAddOne(U32 idx);
EXTERN U32 utilCircQIsEmpty(UtilCircQ *queue);
EXTERN S32 utilCircQEnqueue(UtilCircQ *queue);
#endif

/********************************************************************30**
  
     End of file:     

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
/main/1      ------    an     1. For Circular Queue implementation
/main/1     ys004.102  vr     1. Merged MSPD code with phy 1.7
*********************************************************************91*/
