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

     Name:     RBSS

     Type:     C source file

     Desc:     Version information

     File:     mt_err.h

     Sid:      rb_err.h@@/main/1 - Mon Aug 29 22:31:35 2011

     Prg:      ada

*********************************************************************21*/

#ifndef __RBERRH__
#define __RBERRH__

/* log error macro */
#define RBLOGERROR(errCls, errCode, errVal, errDesc) \
   SLogError(ENTNC, INSTNC, 0, __FILE__, __LINE__, \
             (ErrCls)errCls, (ErrVal)errCode, (ErrVal)errVal, errDesc )

/* defines */

#define   ERRRB       0
#define   ERBBASE     (ERRRB + 0)    /* reserved */

#define   ERB001      (ERRRB +    1)    /*     rb_acc.c: 792 */
#define   ERB002      (ERRRB +    2)    /*     rb_acc.c: 999 */

#define   ERB003      (ERRRB +    3)    /*   rb_icore.c: 196 */
#define   ERB004      (ERRRB +    4)    /*   rb_icore.c: 204 */
#define   ERB005      (ERRRB +    5)    /*   rb_icore.c: 212 */
#define   ERB006      (ERRRB +    6)    /*   rb_icore.c: 221 */
#define   ERB007      (ERRRB +    7)    /*   rb_icore.c: 228 */
#define   ERB008      (ERRRB +    8)    /*   rb_icore.c: 233 */
#define   ERB009      (ERRRB +    9)    /*   rb_icore.c: 283 */
#define   ERB010      (ERRRB +   10)    /*   rb_icore.c: 290 */
#define   ERB011      (ERRRB +   11)    /*   rb_icore.c: 297 */
#define   ERB012      (ERRRB +   12)    /*   rb_icore.c: 304 */
#define   ERB013      (ERRRB +   13)    /*   rb_icore.c: 355 */
#define   ERB014      (ERRRB +   14)    /*   rb_icore.c: 361 */
#define   ERB015      (ERRRB +   15)    /*   rb_icore.c: 373 */
#define   ERB016      (ERRRB +   16)    /*   rb_icore.c: 600 */
#define   ERB017      (ERRRB +   17)    /*   rb_icore.c: 614 */
#define   ERB018      (ERRRB +   18)    /*   rb_icore.c: 620 */
#define   ERB019      (ERRRB +   19)    /*   rb_icore.c: 669 */
#define   ERB020      (ERRRB +   20)    /*   rb_icore.c: 746 */
#define   ERB021      (ERRRB +   21)    /*   rb_icore.c: 752 */
#define   ERB022      (ERRRB +   22)    /*   rb_icore.c: 758 */
#define   ERB023      (ERRRB +   23)    /*   rb_icore.c: 769 */
#define   ERB024      (ERRRB +   24)    /*   rb_icore.c: 810 */
#define   ERB025      (ERRRB +   25)    /*   rb_icore.c: 816 */
#define   ERB026      (ERRRB +   26)    /*   rb_icore.c: 872 */
#define   ERB027      (ERRRB +   27)    /*   rb_icore.c: 883 */
#define   ERB028      (ERRRB +   28)    /*   rb_icore.c: 891 */
#define   ERB029      (ERRRB +   29)    /*   rb_icore.c: 933 */

#define   ERB030      (ERRRB +   30)    /*    rb_icpu.c: 134 */
#define   ERB031      (ERRRB +   31)    /*    rb_icpu.c: 178 */
#define   ERB032      (ERRRB +   32)    /*    rb_icpu.c: 228 */
#define   ERB033      (ERRRB +   33)    /*    rb_icpu.c: 321 */
#define   ERB034      (ERRRB +   34)    /*    rb_icpu.c: 332 */
#define   ERB035      (ERRRB +   35)    /*    rb_icpu.c: 340 */
#define   ERB036      (ERRRB +   36)    /*    rb_icpu.c: 357 */
#define   ERB037      (ERRRB +   37)    /*    rb_icpu.c: 369 */
#define   ERB038      (ERRRB +   38)    /*    rb_icpu.c: 419 */
#define   ERB039      (ERRRB +   39)    /*    rb_icpu.c: 427 */
#define   ERB040      (ERRRB +   40)    /*    rb_icpu.c: 442 */
#define   ERB041      (ERRRB +   41)    /*    rb_icpu.c: 449 */
#define   ERB042      (ERRRB +   42)    /*    rb_icpu.c: 459 */

#define   ERB043      (ERRRB +   43)    /*     rb_log.c: 128 */
#define   ERB044      (ERRRB +   44)    /*     rb_log.c: 187 */
#define   ERB045      (ERRRB +   45)    /*     rb_log.c: 194 */
#define   ERB046      (ERRRB +   46)    /*     rb_log.c: 267 */

#define   ERB047      (ERRRB +   47)    /*      rb_ss.c: 607 */
#define   ERB048      (ERRRB +   48)    /*      rb_ss.c:2011 */
#define   ERB049      (ERRRB +   49)    /*      rb_ss.c:2019 */
#define   ERB050      (ERRRB +   50)    /*      rb_ss.c:2025 */
#define   ERB051      (ERRRB +   51)    /*      rb_ss.c:2109 */
#define   ERB052      (ERRRB +   52)    /*      rb_ss.c:2194 */
#define   ERB053      (ERRRB +   53)    /*      rb_ss.c:2367 */
#define   ERB054      (ERRRB +   54)    /*      rb_ss.c:2446 */
#define   ERB055      (ERRRB +   55)    /*      rb_ss.c:2735 */
#define   ERB056      (ERRRB +   56)    /*      rb_ss.c:2775 */
#define   ERB057      (ERRRB +   57)    /*      rb_ss.c:2819 */
#define   ERB058      (ERRRB +   58)    /*      rb_ss.c:2884 */
#define   ERB059      (ERRRB +   59)    /*      rb_ss.c:2982 */
#define   ERB060      (ERRRB +   60)    /*      rb_ss.c:3067 */
#define   ERB061      (ERRRB +   61)    /*      rb_ss.c:3340 */
#define   ERB062      (ERRRB +   62)    /*      rb_ss.c:3624 */

#define   ERB063      (ERRRB +   63)    /*   rb_timer.c: 165 */
#define   ERB064      (ERRRB +   64)    /*   rb_timer.c: 171 */
#define   ERB065      (ERRRB +   65)    /*   rb_timer.c: 178 */
#define   ERB066      (ERRRB +   66)    /*   rb_timer.c: 185 */
#define   ERB067      (ERRRB +   67)    /*   rb_timer.c: 221 */
#define   ERB068      (ERRRB +   68)    /*   rb_timer.c: 228 */
#define   ERB069      (ERRRB +   69)    /*   rb_timer.c: 235 */
#define   ERB070      (ERRRB +   70)    /*   rb_timer.c: 242 */
#define   ERB071      (ERRRB +   71)    /*   rb_timer.c: 284 */
#define   ERB072      (ERRRB +   72)    /*   rb_timer.c: 322 */
#define   ERB073      (ERRRB +   73)    /*   rb_timer.c: 329 */
#define   ERB074      (ERRRB +   74)    /*   rb_timer.c: 372 */
#define   ERB075      (ERRRB +   75)    /*   rb_timer.c: 453 */
#define   ERB076      (ERRRB +   76)    /*   rb_timer.c: 459 */
#define   ERB077      (ERRRB +   77)    /*   rb_timer.c: 469 */
#define   ERB078      (ERRRB +   78)    /*   rb_timer.c: 508 */
#define   ERB079      (ERRRB +   79)    /*   rb_timer.c: 514 */
#define   ERB080      (ERRRB +   80)    /*   rb_timer.c: 520 */
#define   ERB081      (ERRRB +   81)    /*   rb_timer.c: 561 */
#define   ERB082      (ERRRB +   82)    /*   rb_timer.c: 568 */
#define   ERB083      (ERRRB +   83)    /*   rb_timer.c: 575 */
#define   ERB084      (ERRRB +   84)    /*   rb_timer.c: 582 */
#define   ERB085      (ERRRB +   85)    /*   rb_timer.c: 588 */
#define   ERB086      (ERRRB +   86)    /*   rb_timer.c: 594 */
#define   ERB087      (ERRRB +   87)    /*   rb_timer.c: 635 */
#define   ERB088      (ERRRB +   88)    /*   rb_timer.c: 642 */
#define   ERB089      (ERRRB +   89)    /*   rb_timer.c: 649 */
#define   ERB090      (ERRRB +   90)    /*   rb_timer.c: 656 */
#define   ERB091      (ERRRB +   91)    /*   rb_timer.c: 697 */
#define   ERB092      (ERRRB +   92)    /*   rb_timer.c: 703 */
#define   ERB093      (ERRRB +   93)    /*   rb_timer.c: 709 */
#define   ERB094      (ERRRB +   94)    /*   rb_timer.c: 715 */
#define   ERB095      (ERRRB +   95)    /*   rb_timer.c: 721 */
#define   ERB096      (ERRRB +   96)    /*   rb_timer.c: 763 */
#define   ERB097      (ERRRB +   97)    /*   rb_timer.c: 776 */
#define   ERB098      (ERRRB +   98)    /*   rb_timer.c: 782 */
#define   ERB099      (ERRRB +   99)    /*   rb_timer.c: 788 */
#define   ERB100      (ERRRB +  100)    /*   rb_timer.c: 830 */
#define   ERB101      (ERRRB +  101)    /*   rb_timer.c: 836 */
#define   ERB102      (ERRRB +  102)    /*   rb_timer.c: 843 */
#define   ERB103      (ERRRB +  103)    /*   rb_timer.c: 849 */
#define   ERB104      (ERRRB +  104)    /*   rb_timer.c: 888 */
#define   ERB105      (ERRRB +  105)    /*   rb_timer.c: 952 */
#define   ERB106      (ERRRB +  106)    /*   rb_timer.c: 963 */
#define   ERB107      (ERRRB +  107)    /*   rb_timer.c: 973 */

#endif


/********************************************************************30**

         End of file:     rb_err.h@@/main/1 - Mon Aug 29 22:31:35 2011

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
