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

     Name:     common general functions
  
     Type:     C include file
  
     Desc:     Prototypes for common functions used to pack and unpack 
               primitives and structures. 

     File:     cm_gen.x

     Sid:      cm_gen.x@@/main/6 - Fri Sep 16 02:55:36 2005

     Prg:      nkk

*********************************************************************21*/

#ifdef CMFILE_REORG_1
#define __CMGENX__
#endif

#ifndef __CMGENX__
#define __CMGENX__

#ifdef __cplusplus
extern "C" {
#endif


/* common packing functions */

/* system services structures */
EXTERN S16 cmPkDateTime ARGS((DateTime *dateTime, Buffer *mBuf));
EXTERN S16 cmPkDuration ARGS((Duration *duration, Buffer *mBuf));
EXTERN S16 cmPkEntityId ARGS((EntityId *entityId, Buffer *mBuf));
EXTERN S16 cmPkElmntId  ARGS((ElmntId  *elmntId,  Buffer *mBuf));
EXTERN S16 cmPkMemoryId ARGS((MemoryId *memoryId, Buffer *mBuf));

/* general structures */
EXTERN S16 cmPkSystemId ARGS((SystemId *systemId, Buffer *mBuf));
EXTERN S16 cmPkAddrs    ARGS((Addrs    *addrs,    Buffer *mBuf));
EXTERN S16 cmPkShrtAddrs ARGS((ShrtAddrs    *addrs,    Buffer *mBuf));
EXTERN S16 cmPkAddrMask ARGS((U8    *mask,    Buffer *mBuf));
EXTERN S16 cmPkBndCfg   ARGS((BndCfg   *bndCfg,   Buffer *mBuf));
EXTERN S16 cmPkPst      ARGS((Pst      *pst,      Buffer *mBuf));
EXTERN S16 cmPkElmtHdr  ARGS((ElmtHdr  *m,        Buffer *mBuf));
EXTERN S16 cmPkTknU8    ARGS((TknU8    *tknU8,    Buffer *mBuf));
EXTERN S16 cmPkTknU16   ARGS((TknU16   *tknU16,   Buffer *mBuf));
EXTERN S16 cmPkTknU32   ARGS((TknU32   *tknU32,   Buffer *mBuf));
EXTERN S16 cmPkTknStr   ARGS((TknStr   *tknStr,   Buffer *mBuf));                              
EXTERN S16 cmPkTknStrM  ARGS((TknStrM  *tknStr,   Buffer *mBuf));
EXTERN S16 cmPkTknStrS  ARGS((TknStrS  *tknStr,   Buffer *mBuf));
EXTERN S16 cmPkTknStrE  ARGS((TknStrE  *tknStr,   Buffer *mBuf));
EXTERN S16 cmPkPnNodeId ARGS((PnNodeId *id,       Buffer *mBuf));

EXTERN S16 cmPkTknStr4   ARGS((TknStr4   *tknStr,   Buffer *mBuf));
EXTERN S16 cmPkTknStr12  ARGS((TknStr12  *tknStr,   Buffer *mBuf));
EXTERN S16 cmPkTknStr32  ARGS((TknStr32  *tknStr,   Buffer *mBuf));
EXTERN S16 cmPkTknStr64  ARGS((TknStr64  *tknStr,   Buffer *mBuf));
EXTERN S16 cmPkTknStr132 ARGS((TknStr132 *tknStr,   Buffer *mBuf));
EXTERN S16 cmPkTknStr256 ARGS((TknStr256 *tknStr,   Buffer *mBuf));

/* layer management structures */
EXTERN S16 cmPkHeader   ARGS((Header   *header,   Buffer *mBuf));
EXTERN S16 cmPkSmCfg    ARGS((SmCfg    *smCfg,    Buffer *mBuf));
EXTERN S16 cmPkTmrCfg   ARGS((TmrCfg   *tmrCfg,   Buffer *mBuf));


/* common unpacking functions */

/* system services structures */
EXTERN S16 cmUnpkDateTime ARGS((DateTime *dateTime, Buffer *mBuf));
EXTERN S16 cmUnpkDuration ARGS((Duration *duration, Buffer *mBuf));
EXTERN S16 cmUnpkEntityId ARGS((EntityId *entityId, Buffer *mBuf));
EXTERN S16 cmUnpkElmntId  ARGS((ElmntId  *elmntId,  Buffer *mBuf));
EXTERN S16 cmUnpkMemoryId ARGS((MemoryId *memoryId, Buffer *mBuf));

/* general structures */
EXTERN S16 cmUnpkSystemId ARGS((SystemId *systemId, Buffer *mBuf));
EXTERN S16 cmUnpkAddrs    ARGS((Addrs    *addrs,    Buffer *mBuf));
EXTERN S16 cmUnpkShrtAddrs ARGS((ShrtAddrs    *addrs,    Buffer *mBuf));
EXTERN S16 cmUnpkAddrMask ARGS((U8    *mask,    Buffer *mBuf));
EXTERN S16 cmUnpkBndCfg   ARGS((BndCfg   *bndCfg,   Buffer *mBuf));
EXTERN S16 cmUnpkPst      ARGS((Pst      *pst,      Buffer *mBuf));
EXTERN S16 cmUnpkElmtHdr  ARGS((ElmtHdr  *m,        Buffer *mBuf));
EXTERN S16 cmUnpkTknU8    ARGS((TknU8    *tknU8,    Buffer *mBuf));
EXTERN S16 cmUnpkTknU16   ARGS((TknU16   *tknU16,   Buffer *mBuf));
EXTERN S16 cmUnpkTknU32   ARGS((TknU32   *tknU32,   Buffer *mBuf));
EXTERN S16 cmUnpkTknStr   ARGS((TknStr   *tknStr,   Buffer *mBuf));                              
EXTERN S16 cmUnpkTknStrM  ARGS((TknStrM  *tknStr,   Buffer *mBuf));
EXTERN S16 cmUnpkTknStrS  ARGS((TknStrS  *tknStr,   Buffer *mBuf));
EXTERN S16 cmUnpkTknStrE  ARGS((TknStrE  *tknStr,   Buffer *mBuf));
EXTERN S16 cmUnpkPnNodeId ARGS((PnNodeId *id,       Buffer *mBuf));

EXTERN S16 cmUnpkTknStr4   ARGS((TknStr4   *tknStr,   Buffer *mBuf));
EXTERN S16 cmUnpkTknStr12  ARGS((TknStr12  *tknStr,   Buffer *mBuf));
EXTERN S16 cmUnpkTknStr32  ARGS((TknStr32  *tknStr,   Buffer *mBuf));
EXTERN S16 cmUnpkTknStr64  ARGS((TknStr64  *tknStr,   Buffer *mBuf));
EXTERN S16 cmUnpkTknStr132 ARGS((TknStr132 *tknStr,   Buffer *mBuf));
EXTERN S16 cmUnpkTknStr256 ARGS((TknStr256 *tknStr,   Buffer *mBuf));

/* layer management structures */
EXTERN S16 cmUnpkHeader   ARGS((Header   *header,   Buffer *mBuf));
EXTERN S16 cmUnpkSmCfg    ARGS((SmCfg    *smCfg,    Buffer *mBuf));
EXTERN S16 cmUnpkTmrCfg   ARGS((TmrCfg   *tmrCfg,   Buffer *mBuf));

#ifdef __cplusplus
}
#endif

#endif /* __CMGENX__ */

  
/********************************************************************30**
  
         End of file:     cm_gen.x@@/main/6 - Fri Sep 16 02:55:36 2005
  
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
1.1          ---  lc    1. initial release

*********************************************************************81*/

/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
1.2          ---      apr  1. Added support for c++ compiler
                           2. Added CMFILE_REORG_1 flag.
 
1.3          ---      tej  1. New copyright header

/main/5      ---      rk   1. Updated for Release of IUA v1.3
/main/6      ---      st   1. Update for MAP Release 2.3
*********************************************************************91*/
