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

     Name:     EnodeB Application

     Type:     C source file

     Desc:     EUT Internal Intf. Data structures

     File:     ve_eut.x

     Sid:      ve_eut.x@@/main/2 - Wed Dec  1 08:44:25 2010

     Prg:      kpawar

*********************************************************************21*/
#ifndef __VE_EUT_X__
#define __VE_EUT_X__


#ifdef __cplusplus
extern "C" {
#endif


typedef struct _euEGtpTunInf
{
   U32 locTeid;
   U32 remTeid;
   CmLtePdcpId pdcpId;
   CmTptAddr dstIpAddr;
   CmTptAddr frmDstIpAddr;
} EuEGtpTunInf;

typedef EuEGtpTunInf EgtpTunInf;

#ifdef LTE_HO_SUPPORT
typedef enum
{
   VE_NORMAL_TUNNEL = 0,
   VE_FORWARD_TUNNEL
} VeEuTunType;

typedef struct _euEGtpGenTunInf
{
   U8          rbDir;     /* dl /ul tunnel */
   /* workaround for ccpu00121364 */
   U8          rbMode;
   VeEuTunType tunnType;  /* forward / normal tunnel */
   EgtpTunInf  tunInf;
} EuGenEGtpTunInf;

typedef struct _euEgtpCreatTunInf
{
   U8 numTunls;
   EuGenEGtpTunInf tunLst[VE_MAX_UE_TUNNELS];
} EuCrEGtpTunInf;

/* Data Structure for Data Forwarding and Buffering at EU */
typedef struct euEgtpBuffInfo
{
   CmLList      lnk;         /* Link of Linked List */
   Buffer       *data;       /* Egt Event */
}EuEgtpBuffInfo;

#endif

typedef struct _veEuChngStInfo
{
   U32 teId;
   U8  state;
   U8  cause;
} VeEuChngTnlStInfo;


typedef S16 (*EgtpTunCreat)       ARGS((
        Pst *pst,
        SuId suId,
        EgtpTunInf *tunelInf 
        ));

typedef S16 (*EgtpTunCreatCfm)       ARGS((
        Pst *pst,
        SuId suId,
        CmLtePdcpId *pdcpId 
        ));


typedef S16 (*EgtpTunDel)      ARGS((
        Pst *pst,
        SpId spId,
        U32 locTeid
        ));

typedef S16 (*S1APRelReq) ARGS((
         Pst *pst,
         U32 ueId
        ));

#ifdef LTE_HO_SUPPORT
typedef S16 (*EgtpTunSetCreat)       ARGS((
        Pst *pst,
        SuId suId,
        EuCrEGtpTunInf *tunelInf 
        ));
#endif


typedef S16 (*EgtpChngState) ARGS((
         Pst *pst,
         SuId suId,
         VeEuChngTnlStInfo *stInfo
        ));

PUBLIC S16 cmPkEuCreattLocTunReq ARGS((Pst *pst, SuId suId, EgtpTunInf *tunelInf));
PUBLIC S16 cmPkEuDelLocTunReq ARGS((Pst *pst, SuId suId, U32  locTeid));
PUBLIC S16 cmUnpkEuCreattLocTunReq ARGS((EgtpTunCreat func, Pst *pst,Buffer *mBuf,Mem *sMem));
PUBLIC S16 cmUnpkEuDelLocTunReq ARGS((EgtpTunDel func, Pst *pst,Buffer *mBuf,Mem *sMem));

PUBLIC S16 cmPkChngStReq ARGS((Pst *pst, SuId suId, VeEuChngTnlStInfo *tunelInf));
PUBLIC S16 cmUnpkEuChngStReq ARGS((EgtpChngState func, Pst *pst,Buffer *mBuf,Mem *sMem));
#ifdef LTE_HO_SUPPORT
PUBLIC S16 cmPkEuCreattLocTunSetReq ARGS((Pst *pst, SuId suId, EuCrEGtpTunInf *tunelInf));
PUBLIC S16 cmUnpkEuCreattLocTunSetReq ARGS((EgtpTunSetCreat func, Pst *pst,Buffer *mBuf,Mem *sMem));
#endif

PUBLIC S16 cmPkEuCreattLocTunCfm ARGS((
Pst *pst,
SpId spId,
CmLtePdcpId *pdcpId
));

PUBLIC S16 cmUnpkEuCreattLocTunCfm ARGS((
EgtpTunCreatCfm func,
Pst *pst,
Buffer *mBuf,
Mem *sMem
));

PUBLIC S16 cmPkEuDelLocTunCfm ARGS((
Pst *pst,
SpId spId,
CmLtePdcpId *pdcpId
));

PUBLIC S16 cmUnpkEuDelLocTunCfm ARGS((
EgtpTunCreatCfm func,
Pst *pst,
Buffer *mBuf,
Mem *sMem
));



PUBLIC S16 cmPkEuRelReq ARGS((
 Pst *pst,
 SuId suId,
 U32  ueId
 ));

PUBLIC S16 cmUnpkEuRelReq ARGS((
 S1APRelReq func,
 Pst *pst,
 Buffer *mBuf,
 Mem *sMem
 ));



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __VEX__ */
/********************************************************************30**

         End of file:     ve_eut.x@@/main/2 - Wed Dec  1 08:44:25 2010

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
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
