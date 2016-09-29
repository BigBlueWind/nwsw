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

     Name:     S1AP Layer

     Type:     C source file

     Desc:     C source code for DB

     File:     sq_db.c

     Sid:      sq_db.c@@/main/4 - Mon Jan 10 22:16:51 2011

     Prg:      hsingh

**********************************************************************/

#include "envopt.h"           /* environment options */
#include "envdep.h"           /* environment dependent */
#include "envind.h"           /* environment independent */
#include "gen.h"              /* general layer */
#include "ssi.h"              /* system services */
#include "cm_mblk.h"
#include "cm_tkns.h"
#include "cm_hash.h"
#include "cm_pasn.h"          /* common defines for ASN */
#include "cm5.h"

#include "cm_tpt.h"
#include "sct.h"
#include "cm_inet.h"
#include "szt.h"
/* sz003.301: Updted for PSF-S1AP 1.1 release. Added include of FT/HA related
 * header files */
#ifdef SZ_FTHA
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_pftha.h"
#include "cm_psf.h"
#include "sht.h"           /* System Agent */
#endif /* SZ_FTHA */
#ifndef SZ_ENC_DEC
#include "lsz.h"
#endif
#include "sz.h"

/* header/extern include files (.x) */
#include "cm_llist.h"         /* */
#include "cm_hash.h"
#include "cm_umts.h"
#include "gen.x"              /* general layer */
#include "ssi.x"              /* system services */
#include "cm5.x"              /* */
#include "cm_tkns.x"          /* */
#include "cm_mblk.x"
#include "cm_llist.x"
#include "cm_hash.x"
#include "cm_umts.x"
#include "cm_pasn.x"          /* PASN */
#include <szt_asn.h>
#include <szt_asn.x>

#include "cm_inet.x"
#include "cm_tpt.x"
#include "sct.x"
#ifdef SZTV3
/* sz008.301 support for source RNC to target RNC */
#include "szt_3gasn.x"
#endif
#include "szt.x"
/* sz003.301: Updted for PSF-S1AP 1.1 release. Added include of FT/HA related
 * header files */
#ifdef SZ_FTHA
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_pftha.x"
#include "cm_psf.x"
#include "sht.x"           /* System Agent */
#endif /* SZ_FTHA */
#ifndef SZ_ENC_DEC
#include "lsz.x"
#include "sz.x"
#else
#endif /* SZ_ENC_DEC */

 
#ifdef S1AP_REL8A0     /* S1ap release 8.10.0 */

#include "sz_dbr8.c"

#else               /* S1ap release 9.4.0 */

#include "sz_dbr9.c"

#endif            /* S1ap release specification flag */


/* sz004.301: Added for TC Dec/Enc */
#ifdef  SZTV1
PUBLIC CmPAsnElmDef *szMsgDbIe[][2] =
{
   {
      NULLP
   },
   {
      &szSrceNB_ToTgeteNB_TprntCont,
      NULLP
   },
   {
      &szTgeteNB_ToSrceNB_TprntCont,
      NULLP
   }
};
#endif    /* SZTV1 */



/**********************************************************************

         End of file:     sq_db.c@@/main/4 - Mon Jan 10 22:16:51 2011

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/4      ---        pkaX   1. Updated for S1AP release 3.1
/main/4   sz003.301     pka    1. Updated for PSF-S1AP 1.1 release
/main/4   sz004.301     va     1. Updated for Transparent Container
                                  Enc/dec
*********************************************************************91*/

