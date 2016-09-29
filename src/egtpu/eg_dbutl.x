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

     Name:    EGTP - 

     Type:    C source file

     Desc:    

     File:    eg_dbutl.x

     Sid:      eg_dbutl.x@@/main/3 - Fri Feb  4 18:18:10 2011

     Prg:    ad  

*********************************************************************21*/
#ifndef __EG_DBUTL_X__
#define __EG_DBUTL_X__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef EGTP_C

PUBLIC Void egMakeIeEntry ARGS
((
LegIeEntry      *ieEntry,
U8             ieCode,
Bool           valid,
U8             dataType,
Ptr            fncPtr,
U16             minLen,
U16             maxLen
));

PUBLIC Void egMakeAppChildIeEntry ARGS
((
LegGroupedIe *grpIe,
U8             count,
U8            ieType,
U8            ieClass,
U8            ieList,
U8            ieInst,
U8             nmbGrpIe
));

PUBLIC Void egMakeAppIeProp ARGS
((
LegIeProperties *ieProp,     /* Ie Property to be filled */
U8             ieCode,      /* Ie Code */
U8             ieClass,     /* Mandatory/Conditional/Optional */
U8             instance,     /* Instance  */
U8                  cr
));

PUBLIC S16 egMakeIeDict ARGS
((
LegIeEntry *legIeEntry,
U8      *nmbIe,
U16      *posIdx
));
#endif

#ifdef EGTP_U

PUBLIC Void egUMakeIeEntry ARGS
((
EgIeEntry      *ieEntry,
U8             ieCode,
Bool           tvType,
U8             dataType,
U16             minLen,
U16             maxLen
));

PUBLIC Void egUMakeAppIeProp ARGS
((
EgUIeProperties *ieProp,     /* Ie Property to be filled */
U8             ieCode,      /* Ie Code */
U8             ieClass     /* Mandatory/Conditional/Optional */
));

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __EG_DBUTL_X__ */

/**********************************************************************
 
         End of file:     eg_dbutl.x@@/main/3 - Fri Feb  4 18:18:10 2011
 
**********************************************************************/

/********************************************************************40**
 
        Notes:
 
*********************************************************************41*/
 
/********************************************************************50**
 
*********************************************************************51*/
 
/********************************************************************60**
 
        Revision history:
 
*********************************************************************61*/

/********************************************************************80**
 
*********************************************************************81*/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      sn                1. Created for Initial Release 1.1
/main/2      ---      kchaitanya        1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
*********************************************************************91*/
