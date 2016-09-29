/********************************************************************16**

        (c) COPYRIGHT 1989-1999 by Trillium Digital Systems, Inc.
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
     52.227-19 of the FAR.

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
  
     Name:     stack manager error
  
     Type:     C include file
  
     Desc:     Error defines required by the stack manager.
   
     File:     sm_err.h
  
     Sid:      sm_err.h 1.2  -  05/17/99 12:44:42
  
     Prg:      nkk
  
*********************************************************************21*/

#ifndef __SMERRH__
#define __SMERRH__


/* defines */
  
#define  ESMBASE     (ERRSM + 0)    /* reserved */
/* 
 * ERRSM = 29000
 * Each layer that uses this file need to "reserve" a range
 * These errnos are only used in stack management packing/unpaking 
 * routines, therefore each product shouldn't need too many....I hope.
 * Remeber, errnos are S16 so our real limit here is 32767 and we're
 * starting at 29000
 */
#define  ESMXXX      (ESMBASE + 0)   /*   SM gets   0 - 49  */
#define  ESMSTX      (ESMBASE + 50)  /* TCAP gets  50 - 99  */
#define  ESMSPX      (ESMBASE + 100) /* SCCP gets 100 - 149 */
#define  ESMSNX      (ESMBASE + 150) /* MTP3 gets 150 - 199 */
#define  ESMSDX      (ESMBASE + 200) /* MTP2 gets 200 - 249 */

#endif /* __SMERRH__ */


/********************************************************************30**

         End of file: sm_err.h 1.2  -  05/17/99 12:44:42

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
1.1          ---  fmg   1. initial release

*********************************************************************81*/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
1.2          ---      mb   1. Changed copyright Header
 
*********************************************************************91*/
