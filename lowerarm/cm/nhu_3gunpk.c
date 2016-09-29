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

     Name:     LTE RRC Layer

     Type:     C file

     Desc:     Defines required by RRC

     File:     nhu_3gunpk.c 

     Sid:      nhu_3gunpk.c@@/main/1 - Fri Dec 11 18:45:15 2009

     Prg:      chebli

**********************************************************************/

#include "envopt.h"           /* environment options */
#include "envdep.h"           /* environment dependent */
#include "envind.h"           /* environment independent */
#include "gen.h"              /* general layer */
#include "ssi.h"              /* system services */
#include "cm_mblk.h"
#include "cm_tkns.h"
#include "cm_pasn.h"          /* common defines for ASN */
#include "gen.x"              /* general layer */
#include "ssi.x"              /* system services */
#include "cm_tkns.x"          /* */
#include "cm_mblk.x"
#include "cm_pasn.x"          /* PASN */
#include "nhu_3gasn.h"
#include "nhu_3gasn.x"
#include "nhu_3gunpk.x"

#define cmUnpkNhuMaxHcCtxtSpace cmUnpkTknU32

#define cmUnpkNhuPDCP_CaplosslessSRNS_RelocationSupport cmUnpkTknU8



/*
* FUN : cmUnpkNhuPDCP_CapsupportForRfc2507
*  DESC: Function to Unpack NhuPDCP_CapsupportForRfc2507 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPDCP_CapsupportForRfc2507
(
NhuPDCP_CapsupportForRfc2507 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPDCP_CapsupportForRfc2507(param, ptr, mBuf)
NhuPDCP_CapsupportForRfc2507 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPDCP_CapsupportForRfc2507) 
    
    CMCHKUNPK(cmUnpkTknU8, &param->choice, mBuf); 
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case SUPPORTFORRFC2507_NOTSUPP :
                break;
            case SUPPORTFORRFC2507_SUPPORTED :
                CMCHKUNPK(cmUnpkNhuMaxHcCtxtSpace, &param->val.supported, mBuf); 
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPDCP_CapsupportForRfc2507 */



/*
* FUN : cmUnpkNhuPDCP_Cap
*  DESC: Function to Unpack NhuPDCP_Cap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPDCP_Cap
(
NhuPDCP_Cap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPDCP_Cap(param, ptr, mBuf)
NhuPDCP_Cap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPDCP_Cap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuPDCP_CaplosslessSRNS_RelocationSupport, &param->losslessSRNS_RelocationSupport, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuPDCP_CapsupportForRfc2507, &param->supportForRfc2507, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPDCP_Cap */

#define cmUnpkNhuTotalRLC_AM_BufferSize cmUnpkTknU32

#define cmUnpkNhuMaxRLC_WindowSize cmUnpkTknU32

#define cmUnpkNhuMaxAM_EntityNumberRLC_Cap cmUnpkTknU32



/*
* FUN : cmUnpkNhuRLC_Cap
*  DESC: Function to Unpack NhuRLC_Cap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuRLC_Cap
(
NhuRLC_Cap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuRLC_Cap(param, ptr, mBuf)
NhuRLC_Cap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuRLC_Cap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuTotalRLC_AM_BufferSize, &param->totalRLC_AM_BufferSize, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxRLC_WindowSize, &param->maximumRLC_WindowSize, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxAM_EntityNumberRLC_Cap, &param->maximumAM_EntityNumber, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuRLC_Cap */

#define cmUnpkNhuMaxNoBits cmUnpkTknU32



/*
* FUN : cmUnpkNhuTurboSupport
*  DESC: Function to Unpack NhuTurboSupport structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuTurboSupport
(
NhuTurboSupport *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuTurboSupport(param, ptr, mBuf)
NhuTurboSupport *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuTurboSupport) 
    
    CMCHKUNPK(cmUnpkTknU8, &param->choice, mBuf); 
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case TURBOSUPPORT_NOTSUPP :
                break;
            case TURBOSUPPORT_SUPPORTED :
                CMCHKUNPK(cmUnpkNhuMaxNoBits, &param->val.supported, mBuf); 
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuTurboSupport */

#define cmUnpkNhuMaxSimultaneousTransChsDL cmUnpkTknU32

#define cmUnpkNhuMaxSimultaneousCCTrCH_Count cmUnpkTknU32 
#define cmUnpkNhuMaxTportBlocksDL cmUnpkTknU32

#define cmUnpkNhuMaxNumberOfTFC_DL cmUnpkTknU32

#define cmUnpkNhuMaxNumberOfTF cmUnpkTknU32



/*
* FUN : cmUnpkNhuDL_TransChCap
*  DESC: Function to Unpack NhuDL_TransChCap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuDL_TransChCap
(
NhuDL_TransChCap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuDL_TransChCap(param, ptr, mBuf)
NhuDL_TransChCap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuDL_TransChCap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuMaxNoBits, &param->maxNoBitsReceived, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxNoBits, &param->maxConvCodeBitsReceived, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuTurboSupport, &param->turboDecodingSupport, ptr, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxSimultaneousTransChsDL, &param->maxSimultaneousTransChs, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxSimultaneousCCTrCH_Count, &param->maxSimultaneousCCTrCH_Count, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxTportBlocksDL, &param->maxReceivedTportBlocks, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxNumberOfTFC_DL, &param->maxNumberOfTFC, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxNumberOfTF, &param->maxNumberOfTF, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuDL_TransChCap */

#define cmUnpkNhuMaxSimultaneousTransChsUL cmUnpkTknU32

#define cmUnpkNhuMaxTportBlocksUL cmUnpkTknU32

#define cmUnpkNhuMaxNumberOfTFC_UL cmUnpkTknU32



/*
* FUN : cmUnpkNhuUL_TransChCapmodeSpecificInfotdd
*  DESC: Function to Unpack NhuUL_TransChCapmodeSpecificInfotdd structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUL_TransChCapmodeSpecificInfotdd
(
NhuUL_TransChCapmodeSpecificInfotdd *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUL_TransChCapmodeSpecificInfotdd(param, ptr, mBuf)
NhuUL_TransChCapmodeSpecificInfotdd *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUL_TransChCapmodeSpecificInfotdd) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuMaxSimultaneousCCTrCH_Count, &param->maxSimultaneousCCTrCH_Count, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUL_TransChCapmodeSpecificInfotdd */



/*
* FUN : cmUnpkNhuUL_TransChCapmodeSpecificInfo
*  DESC: Function to Unpack NhuUL_TransChCapmodeSpecificInfo structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUL_TransChCapmodeSpecificInfo
(
NhuUL_TransChCapmodeSpecificInfo *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUL_TransChCapmodeSpecificInfo(param, ptr, mBuf)
NhuUL_TransChCapmodeSpecificInfo *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUL_TransChCapmodeSpecificInfo) 
    
    CMCHKUNPK(cmUnpkTknU8, &param->choice, mBuf); 
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case MODESPECIFICINFO_FDD :
                break;
            case MODESPECIFICINFO_TDD :
                CMCHKUNPKPTR(cmUnpkNhuUL_TransChCapmodeSpecificInfotdd, &param->val.tdd, ptr, mBuf); 
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUL_TransChCapmodeSpecificInfo */



/*
* FUN : cmUnpkNhuUL_TransChCap
*  DESC: Function to Unpack NhuUL_TransChCap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUL_TransChCap
(
NhuUL_TransChCap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUL_TransChCap(param, ptr, mBuf)
NhuUL_TransChCap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUL_TransChCap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuMaxNoBits, &param->maxNoBitsTransmitted, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxNoBits, &param->maxConvCodeBitsTransmitted, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuTurboSupport, &param->turboEncodingSupport, ptr, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxSimultaneousTransChsUL, &param->maxSimultaneousTransChs, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUL_TransChCapmodeSpecificInfo, &param->modeSpecificInfo, ptr, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxTportBlocksUL, &param->maxTransmittedBlocks, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxNumberOfTFC_UL, &param->maxNumberOfTFC, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxNumberOfTF, &param->maxNumberOfTF, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUL_TransChCap */



/*
* FUN : cmUnpkNhuTportChannelCap
*  DESC: Function to Unpack NhuTportChannelCap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuTportChannelCap
(
NhuTportChannelCap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuTportChannelCap(param, ptr, mBuf)
NhuTportChannelCap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuTportChannelCap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuDL_TransChCap, &param->dl_TransChCap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUL_TransChCap, &param->ul_TransChCap, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuTportChannelCap */

#define cmUnpkNhuUE_PowerClass cmUnpkTknU32 
#define cmUnpkNhuTxRxFrequencySeparation cmUnpkTknU32

#define cmUnpkNhuRadioFrequencyBandTDDLst cmUnpkTknU32

#define cmUnpkNhuChipRateCap cmUnpkTknU32



/*
* FUN : cmUnpkNhuRF_CapfddRF_Cap
*  DESC: Function to Unpack NhuRF_CapfddRF_Cap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuRF_CapfddRF_Cap
(
NhuRF_CapfddRF_Cap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuRF_CapfddRF_Cap(param, ptr, mBuf)
NhuRF_CapfddRF_Cap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuRF_CapfddRF_Cap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuUE_PowerClass, &param->ue_PowerClass, mBuf); 
        CMCHKUNPK(cmUnpkNhuTxRxFrequencySeparation, &param->txRxFrequencySeparation, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuRF_CapfddRF_Cap */



/*
* FUN : cmUnpkNhuRF_CaptddRF_Cap
*  DESC: Function to Unpack NhuRF_CaptddRF_Cap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuRF_CaptddRF_Cap
(
NhuRF_CaptddRF_Cap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuRF_CaptddRF_Cap(param, ptr, mBuf)
NhuRF_CaptddRF_Cap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuRF_CaptddRF_Cap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuUE_PowerClass, &param->ue_PowerClass, mBuf); 
        CMCHKUNPK(cmUnpkNhuRadioFrequencyBandTDDLst, &param->radioFrequencyTDDBandLst, mBuf); 
        CMCHKUNPK(cmUnpkNhuChipRateCap, &param->chipRateCap, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuRF_CaptddRF_Cap */



/*
* FUN : cmUnpkNhuRF_Cap
*  DESC: Function to Unpack NhuRF_Cap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuRF_Cap
(
NhuRF_Cap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuRF_Cap(param, ptr, mBuf)
NhuRF_Cap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuRF_Cap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuRF_CapfddRF_Cap, &param->fddRF_Cap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuRF_CaptddRF_Cap, &param->tddRF_Cap, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuRF_Cap */

#define cmUnpkNhuMaxNoPhysChBitsReceived cmUnpkTknU32

#define cmUnpkNhuMaxNoSCCPCH_RL cmUnpkTknU32

#define cmUnpkNhuSimultaneousSCCPCH_DPCH_ReceptionsupportedsimultaneousSCCPCH_DPCH_DPDCH_Reception cmUnpkTknU8



/*
* FUN : cmUnpkNhuSimultaneousSCCPCH_DPCH_Receptionsupported
*  DESC: Function to Unpack NhuSimultaneousSCCPCH_DPCH_Receptionsupported structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuSimultaneousSCCPCH_DPCH_Receptionsupported
(
NhuSimultaneousSCCPCH_DPCH_Receptionsupported *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuSimultaneousSCCPCH_DPCH_Receptionsupported(param, ptr, mBuf)
NhuSimultaneousSCCPCH_DPCH_Receptionsupported *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuSimultaneousSCCPCH_DPCH_Receptionsupported) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuMaxNoSCCPCH_RL, &param->maxNoSCCPCH_RL, mBuf); 
        CMCHKUNPK(cmUnpkNhuSimultaneousSCCPCH_DPCH_ReceptionsupportedsimultaneousSCCPCH_DPCH_DPDCH_Reception, &param->simultaneousSCCPCH_DPCH_DPDCH_Reception, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuSimultaneousSCCPCH_DPCH_Receptionsupported */



/*
* FUN : cmUnpkNhuSimultaneousSCCPCH_DPCH_Reception
*  DESC: Function to Unpack NhuSimultaneousSCCPCH_DPCH_Reception structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuSimultaneousSCCPCH_DPCH_Reception
(
NhuSimultaneousSCCPCH_DPCH_Reception *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuSimultaneousSCCPCH_DPCH_Reception(param, ptr, mBuf)
NhuSimultaneousSCCPCH_DPCH_Reception *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuSimultaneousSCCPCH_DPCH_Reception) 
    
    CMCHKUNPK(cmUnpkTknU8, &param->choice, mBuf); 
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case SIMULTANEOUSSCCPCH_DPCH_RECEPTION_NOTSUPP :
                break;
            case SIMULTANEOUSSCCPCH_DPCH_RECEPTION_SUPPORTED :
                CMCHKUNPKPTR(cmUnpkNhuSimultaneousSCCPCH_DPCH_Receptionsupported, &param->val.supported, ptr, mBuf); 
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuSimultaneousSCCPCH_DPCH_Reception */

#define cmUnpkNhuDL_PhysChCapFDDmaxNoDPCH_PDSCH_Codes cmUnpkTknU32 
#define cmUnpkNhuDL_PhysChCapFDDsupportForSF_512 cmUnpkTknU8

#define cmUnpkNhuDL_PhysChCapFDDdummy cmUnpkTknU8



/*
* FUN : cmUnpkNhuDL_PhysChCapFDD
*  DESC: Function to Unpack NhuDL_PhysChCapFDD structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuDL_PhysChCapFDD
(
NhuDL_PhysChCapFDD *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuDL_PhysChCapFDD(param, ptr, mBuf)
NhuDL_PhysChCapFDD *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuDL_PhysChCapFDD) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuDL_PhysChCapFDDmaxNoDPCH_PDSCH_Codes, &param->maxNoDPCH_PDSCH_Codes, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxNoPhysChBitsReceived, &param->maxNoPhysChBitsReceived, mBuf); 
        CMCHKUNPK(cmUnpkNhuDL_PhysChCapFDDsupportForSF_512, &param->supportForSF_512, mBuf); 
        CMCHKUNPK(cmUnpkNhuDL_PhysChCapFDDdummy, &param->dummy, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuSimultaneousSCCPCH_DPCH_Reception, &param->dummy2, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuDL_PhysChCapFDD */

#define cmUnpkNhuMaxNoDPDCH_BitsTransmitted cmUnpkTknU32

#define cmUnpkNhuUL_PhysChCapFDDdummy cmUnpkTknU8



/*
* FUN : cmUnpkNhuUL_PhysChCapFDD
*  DESC: Function to Unpack NhuUL_PhysChCapFDD structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUL_PhysChCapFDD
(
NhuUL_PhysChCapFDD *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUL_PhysChCapFDD(param, ptr, mBuf)
NhuUL_PhysChCapFDD *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUL_PhysChCapFDD) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuMaxNoDPDCH_BitsTransmitted, &param->maxNoDPDCH_BitsTransmitted, mBuf); 
        CMCHKUNPK(cmUnpkNhuUL_PhysChCapFDDdummy, &param->dummy, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUL_PhysChCapFDD */

#define cmUnpkNhuMaxTS_PerFrame cmUnpkTknU32 
#define cmUnpkNhuMaxPhysChPerFrame cmUnpkTknU32 
#define cmUnpkNhuMinimumSF_DL cmUnpkTknU32

#define cmUnpkNhuMaxPhysChPerTS cmUnpkTknU32 
#define cmUnpkNhuDL_PhysChCapTDDsupportOfPDSCH cmUnpkTknU8



/*
* FUN : cmUnpkNhuDL_PhysChCapTDD
*  DESC: Function to Unpack NhuDL_PhysChCapTDD structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuDL_PhysChCapTDD
(
NhuDL_PhysChCapTDD *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuDL_PhysChCapTDD(param, ptr, mBuf)
NhuDL_PhysChCapTDD *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuDL_PhysChCapTDD) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuMaxTS_PerFrame, &param->maxTS_PerFrame, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxPhysChPerFrame, &param->maxPhysChPerFrame, mBuf); 
        CMCHKUNPK(cmUnpkNhuMinimumSF_DL, &param->minimumSF, mBuf); 
        CMCHKUNPK(cmUnpkNhuDL_PhysChCapTDDsupportOfPDSCH, &param->supportOfPDSCH, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxPhysChPerTS, &param->maxPhysChPerTS, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuDL_PhysChCapTDD */

#define cmUnpkNhuMaxPhysChPerTimeslot cmUnpkTknU32

#define cmUnpkNhuMinimumSF_UL cmUnpkTknU32

#define cmUnpkNhuUL_PhysChCapTDDsupportOfPUSCH cmUnpkTknU8



/*
* FUN : cmUnpkNhuUL_PhysChCapTDD
*  DESC: Function to Unpack NhuUL_PhysChCapTDD structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUL_PhysChCapTDD
(
NhuUL_PhysChCapTDD *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUL_PhysChCapTDD(param, ptr, mBuf)
NhuUL_PhysChCapTDD *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUL_PhysChCapTDD) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuMaxTS_PerFrame, &param->maxTS_PerFrame, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxPhysChPerTimeslot, &param->maxPhysChPerTimeslot, mBuf); 
        CMCHKUNPK(cmUnpkNhuMinimumSF_UL, &param->minimumSF, mBuf); 
        CMCHKUNPK(cmUnpkNhuUL_PhysChCapTDDsupportOfPUSCH, &param->supportOfPUSCH, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUL_PhysChCapTDD */



/*
* FUN : cmUnpkNhuPhysicalChannelCapfddPhysChCap
*  DESC: Function to Unpack NhuPhysicalChannelCapfddPhysChCap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPhysicalChannelCapfddPhysChCap
(
NhuPhysicalChannelCapfddPhysChCap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPhysicalChannelCapfddPhysChCap(param, ptr, mBuf)
NhuPhysicalChannelCapfddPhysChCap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPhysicalChannelCapfddPhysChCap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuDL_PhysChCapFDD, &param->downlinkPhysChCap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUL_PhysChCapFDD, &param->uplinkPhysChCap, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPhysicalChannelCapfddPhysChCap */



/*
* FUN : cmUnpkNhuPhysicalChannelCaptddPhysChCap
*  DESC: Function to Unpack NhuPhysicalChannelCaptddPhysChCap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPhysicalChannelCaptddPhysChCap
(
NhuPhysicalChannelCaptddPhysChCap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPhysicalChannelCaptddPhysChCap(param, ptr, mBuf)
NhuPhysicalChannelCaptddPhysChCap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPhysicalChannelCaptddPhysChCap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuDL_PhysChCapTDD, &param->downlinkPhysChCap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUL_PhysChCapTDD, &param->uplinkPhysChCap, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPhysicalChannelCaptddPhysChCap */



/*
* FUN : cmUnpkNhuPhysicalChannelCap
*  DESC: Function to Unpack NhuPhysicalChannelCap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap
(
NhuPhysicalChannelCap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap(param, ptr, mBuf)
NhuPhysicalChannelCap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPhysicalChannelCap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuPhysicalChannelCapfddPhysChCap, &param->fddPhysChCap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuPhysicalChannelCaptddPhysChCap, &param->tddPhysChCap, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPhysicalChannelCap */

#define cmUnpkNhuMultiRAT_CapsupportOfGSM cmUnpkTknU8

#define cmUnpkNhuMultiRAT_CapsupportOfMulticarrier cmUnpkTknU8



/*
* FUN : cmUnpkNhuMultiRAT_Cap
*  DESC: Function to Unpack NhuMultiRAT_Cap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuMultiRAT_Cap
(
NhuMultiRAT_Cap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuMultiRAT_Cap(param, ptr, mBuf)
NhuMultiRAT_Cap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuMultiRAT_Cap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuMultiRAT_CapsupportOfGSM, &param->supportOfGSM, mBuf); 
        CMCHKUNPK(cmUnpkNhuMultiRAT_CapsupportOfMulticarrier, &param->supportOfMulticarrier, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuMultiRAT_Cap */

#define cmUnpkNhuMultiModeCap cmUnpkTknU32



/*
* FUN : cmUnpkNhuUE_MultiModeRAT_Cap
*  DESC: Function to Unpack NhuUE_MultiModeRAT_Cap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_MultiModeRAT_Cap
(
NhuUE_MultiModeRAT_Cap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_MultiModeRAT_Cap(param, ptr, mBuf)
NhuUE_MultiModeRAT_Cap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_MultiModeRAT_Cap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuMultiRAT_Cap, &param->multiRAT_CapLst, ptr, mBuf); 
        CMCHKUNPK(cmUnpkNhuMultiModeCap, &param->multiModeCap, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_MultiModeRAT_Cap */

#define cmUnpkNhuSecCapcipheringAlgorithmCap cmUnpkTknStrBSXL 
#define cmUnpkNhuSecCapintegrityProtectionAlgorithmCap cmUnpkTknStrBSXL 


/*
* FUN : cmUnpkNhuSecCap
*  DESC: Function to Unpack NhuSecCap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuSecCap
(
NhuSecCap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuSecCap(param, ptr, mBuf)
NhuSecCap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuSecCap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuSecCapcipheringAlgorithmCap, &param->cipheringAlgorithmCap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuSecCapintegrityProtectionAlgorithmCap, &param->integrityProtectionAlgorithmCap, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuSecCap */

#define cmUnpkNhuNwAssistedGPS_Supp cmUnpkTknU32

#define cmUnpkNhuUE_Positioning_CapstandaloneLocMethodsSupp cmUnpkTknU8

#define cmUnpkNhuUE_Positioning_Capue_BasedOTDOA_Supp cmUnpkTknU8

#define cmUnpkNhuUE_Positioning_CapsupportForUE_GPS_TimingOfCellFrames cmUnpkTknU8

#define cmUnpkNhuUE_Positioning_CapsupportForIPDL cmUnpkTknU8



/*
* FUN : cmUnpkNhuUE_Positioning_Cap
*  DESC: Function to Unpack NhuUE_Positioning_Cap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_Positioning_Cap
(
NhuUE_Positioning_Cap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_Positioning_Cap(param, ptr, mBuf)
NhuUE_Positioning_Cap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_Positioning_Cap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuUE_Positioning_CapstandaloneLocMethodsSupp, &param->standaloneLocMethodsSupp, mBuf); 
        CMCHKUNPK(cmUnpkNhuUE_Positioning_Capue_BasedOTDOA_Supp, &param->ue_BasedOTDOA_Supp, mBuf); 
        CMCHKUNPK(cmUnpkNhuNwAssistedGPS_Supp, &param->networkAssistedGPS_Supp, mBuf); 
        CMCHKUNPK(cmUnpkNhuUE_Positioning_CapsupportForUE_GPS_TimingOfCellFrames, &param->supportForUE_GPS_TimingOfCellFrames, mBuf); 
        CMCHKUNPK(cmUnpkNhuUE_Positioning_CapsupportForIPDL, &param->supportForIPDL, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_Positioning_Cap */

#define cmUnpkNhuGSM_Measurementsgsm900 cmUnpkTknU8

#define cmUnpkNhuGSM_Measurementsdcs1800 cmUnpkTknU8

#define cmUnpkNhuGSM_Measurementsgsm1900 cmUnpkTknU8



/*
* FUN : cmUnpkNhuGSM_Measurements
*  DESC: Function to Unpack NhuGSM_Measurements structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuGSM_Measurements
(
NhuGSM_Measurements *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuGSM_Measurements(param, ptr, mBuf)
NhuGSM_Measurements *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuGSM_Measurements) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuGSM_Measurementsgsm900, &param->gsm900, mBuf); 
        CMCHKUNPK(cmUnpkNhuGSM_Measurementsdcs1800, &param->dcs1800, mBuf); 
        CMCHKUNPK(cmUnpkNhuGSM_Measurementsgsm1900, &param->gsm1900, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuGSM_Measurements */

#define cmUnpkNhuCompressedModeMeasCapfdd_Measurements cmUnpkTknU8

#define cmUnpkNhuCompressedModeMeasCaptdd_Measurements cmUnpkTknU8

#define cmUnpkNhuCompressedModeMeasCapmultiCarrierMeasurements cmUnpkTknU8



/*
* FUN : cmUnpkNhuCompressedModeMeasCap
*  DESC: Function to Unpack NhuCompressedModeMeasCap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCap
(
NhuCompressedModeMeasCap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCap(param, ptr, mBuf)
NhuCompressedModeMeasCap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuCompressedModeMeasCap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuCompressedModeMeasCapfdd_Measurements, &param->fdd_Measurements, mBuf); 
        CMCHKUNPK(cmUnpkNhuCompressedModeMeasCaptdd_Measurements, &param->tdd_Measurements, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuGSM_Measurements, &param->gsm_Measurements, ptr, mBuf); 
        CMCHKUNPK(cmUnpkNhuCompressedModeMeasCapmultiCarrierMeasurements, &param->multiCarrierMeasurements, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuCompressedModeMeasCap */



/*
* FUN : cmUnpkNhuMeasurementCap
*  DESC: Function to Unpack NhuMeasurementCap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuMeasurementCap
(
NhuMeasurementCap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuMeasurementCap(param, ptr, mBuf)
NhuMeasurementCap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuMeasurementCap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuCompressedModeMeasCap, &param->downlinkCompressedMode, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuCompressedModeMeasCap, &param->uplinkCompressedMode, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuMeasurementCap */



/*
* FUN : cmUnpkNhuUE_RadioAccessCap
*  DESC: Function to Unpack NhuUE_RadioAccessCap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCap
(
NhuUE_RadioAccessCap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCap(param, ptr, mBuf)
NhuUE_RadioAccessCap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_RadioAccessCap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuPDCP_Cap, &param->pdcp_Cap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuRLC_Cap, &param->rlc_Cap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuTportChannelCap, &param->transportChannelCap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuRF_Cap, &param->rf_Cap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuPhysicalChannelCap, &param->physicalChannelCap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUE_MultiModeRAT_Cap, &param->ue_MultiModeRAT_Cap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuSecCap, &param->securityCap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUE_Positioning_Cap, &param->ue_positioning_Cap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuMeasurementCap, &param->measurementCap, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCap */

#define cmUnpkNhuRadioFrequencyBandFDD cmUnpkTknU32

#define cmUnpkNhuUE_PowerClassExt cmUnpkTknU32

#define cmUnpkNhuCompressedModeMeasCapabFDDdl_MeasurementsFDD cmUnpkTknU8

#define cmUnpkNhuCompressedModeMeasCapabFDDul_MeasurementsFDD cmUnpkTknU8



/*
* FUN : cmUnpkNhuCompressedModeMeasCapabFDD
*  DESC: Function to Unpack NhuCompressedModeMeasCapabFDD structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabFDD
(
NhuCompressedModeMeasCapabFDD *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabFDD(param, ptr, mBuf)
NhuCompressedModeMeasCapabFDD *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuCompressedModeMeasCapabFDD) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuRadioFrequencyBandFDD, &param->radioFrequencyBandFDD, mBuf); 
        CMCHKUNPK(cmUnpkNhuCompressedModeMeasCapabFDDdl_MeasurementsFDD, &param->dl_MeasurementsFDD, mBuf); 
        CMCHKUNPK(cmUnpkNhuCompressedModeMeasCapabFDDul_MeasurementsFDD, &param->ul_MeasurementsFDD, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuCompressedModeMeasCapabFDD */



/*
* FUN : cmUnpkNhuCompressedModeMeasCapabFDDLst
*  DESC: Function to Unpack NhuCompressedModeMeasCapabFDDLst structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabFDDLst
(
NhuCompressedModeMeasCapabFDDLst *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabFDDLst(param, ptr, mBuf)
NhuCompressedModeMeasCapabFDDLst *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    Cntr i;
    TRC3(cmUnpkNhuCompressedModeMeasCapabFDDLst) 
    
    CMCHKUNPK(cmUnpkTknU16, &param->noComp, mBuf); 
    if( param->noComp.pres != NOTPRSNT ) 
    {
        CMGETMBLK(ptr, sizeof(NhuCompressedModeMeasCapabFDD)*(param->noComp.val), (Ptr*)&(param->member) );
        for(i = 0; i < param->noComp.val; i++)
        {
            CMCHKUNPKPTR(cmUnpkNhuCompressedModeMeasCapabFDD, &(param->member[i]), ptr, mBuf); 
        }
    }
    return(ROK);
}     /* End of function cmUnpkNhuCompressedModeMeasCapabFDDLst */

#define cmUnpkNhuRadioFrequencyBandTDD cmUnpkTknU32

#define cmUnpkNhuCompressedModeMeasCapabTDDdl_MeasurementsTDD cmUnpkTknU8

#define cmUnpkNhuCompressedModeMeasCapabTDDul_MeasurementsTDD cmUnpkTknU8



/*
* FUN : cmUnpkNhuCompressedModeMeasCapabTDD
*  DESC: Function to Unpack NhuCompressedModeMeasCapabTDD structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabTDD
(
NhuCompressedModeMeasCapabTDD *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabTDD(param, ptr, mBuf)
NhuCompressedModeMeasCapabTDD *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuCompressedModeMeasCapabTDD) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuRadioFrequencyBandTDD, &param->radioFrequencyBandTDD, mBuf); 
        CMCHKUNPK(cmUnpkNhuCompressedModeMeasCapabTDDdl_MeasurementsTDD, &param->dl_MeasurementsTDD, mBuf); 
        CMCHKUNPK(cmUnpkNhuCompressedModeMeasCapabTDDul_MeasurementsTDD, &param->ul_MeasurementsTDD, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuCompressedModeMeasCapabTDD */



/*
* FUN : cmUnpkNhuCompressedModeMeasCapabTDDLst
*  DESC: Function to Unpack NhuCompressedModeMeasCapabTDDLst structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabTDDLst
(
NhuCompressedModeMeasCapabTDDLst *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabTDDLst(param, ptr, mBuf)
NhuCompressedModeMeasCapabTDDLst *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    Cntr i;
    TRC3(cmUnpkNhuCompressedModeMeasCapabTDDLst) 
    
    CMCHKUNPK(cmUnpkTknU16, &param->noComp, mBuf); 
    if( param->noComp.pres != NOTPRSNT ) 
    {
        CMGETMBLK(ptr, sizeof(NhuCompressedModeMeasCapabTDD)*(param->noComp.val), (Ptr*)&(param->member) );
        for(i = 0; i < param->noComp.val; i++)
        {
            CMCHKUNPKPTR(cmUnpkNhuCompressedModeMeasCapabTDD, &(param->member[i]), ptr, mBuf); 
        }
    }
    return(ROK);
}     /* End of function cmUnpkNhuCompressedModeMeasCapabTDDLst */

#define cmUnpkNhuRadioFrequencyBandGSM cmUnpkTknU32

#define cmUnpkNhuCompressedModeMeasCapabGSMdl_MeasurementsGSM cmUnpkTknU8

#define cmUnpkNhuCompressedModeMeasCapabGSMul_MeasurementsGSM cmUnpkTknU8



/*
* FUN : cmUnpkNhuCompressedModeMeasCapabGSM
*  DESC: Function to Unpack NhuCompressedModeMeasCapabGSM structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabGSM
(
NhuCompressedModeMeasCapabGSM *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabGSM(param, ptr, mBuf)
NhuCompressedModeMeasCapabGSM *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuCompressedModeMeasCapabGSM) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuRadioFrequencyBandGSM, &param->radioFrequencyBandGSM, mBuf); 
        CMCHKUNPK(cmUnpkNhuCompressedModeMeasCapabGSMdl_MeasurementsGSM, &param->dl_MeasurementsGSM, mBuf); 
        CMCHKUNPK(cmUnpkNhuCompressedModeMeasCapabGSMul_MeasurementsGSM, &param->ul_MeasurementsGSM, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuCompressedModeMeasCapabGSM */



/*
* FUN : cmUnpkNhuCompressedModeMeasCapabGSMLst
*  DESC: Function to Unpack NhuCompressedModeMeasCapabGSMLst structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabGSMLst
(
NhuCompressedModeMeasCapabGSMLst *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabGSMLst(param, ptr, mBuf)
NhuCompressedModeMeasCapabGSMLst *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    Cntr i;
    TRC3(cmUnpkNhuCompressedModeMeasCapabGSMLst) 
    
    CMCHKUNPK(cmUnpkTknU16, &param->noComp, mBuf); 
    if( param->noComp.pres != NOTPRSNT ) 
    {
        CMGETMBLK(ptr, sizeof(NhuCompressedModeMeasCapabGSM)*(param->noComp.val), (Ptr*)&(param->member) );
        for(i = 0; i < param->noComp.val; i++)
        {
            CMCHKUNPKPTR(cmUnpkNhuCompressedModeMeasCapabGSM, &(param->member[i]), ptr, mBuf); 
        }
    }
    return(ROK);
}     /* End of function cmUnpkNhuCompressedModeMeasCapabGSMLst */

#define cmUnpkNhuCompressedModeMeasCapabMCdl_MeasurementsMC cmUnpkTknU8

#define cmUnpkNhuCompressedModeMeasCapabMCul_MeasurementsMC cmUnpkTknU8



/*
* FUN : cmUnpkNhuCompressedModeMeasCapabMC
*  DESC: Function to Unpack NhuCompressedModeMeasCapabMC structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabMC
(
NhuCompressedModeMeasCapabMC *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabMC(param, ptr, mBuf)
NhuCompressedModeMeasCapabMC *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuCompressedModeMeasCapabMC) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuCompressedModeMeasCapabMCdl_MeasurementsMC, &param->dl_MeasurementsMC, mBuf); 
        CMCHKUNPK(cmUnpkNhuCompressedModeMeasCapabMCul_MeasurementsMC, &param->ul_MeasurementsMC, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuCompressedModeMeasCapabMC */



/*
* FUN : cmUnpkNhuMeasurementCapExt
*  DESC: Function to Unpack NhuMeasurementCapExt structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuMeasurementCapExt
(
NhuMeasurementCapExt *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuMeasurementCapExt(param, ptr, mBuf)
NhuMeasurementCapExt *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuMeasurementCapExt) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuCompressedModeMeasCapabFDDLst, &param->compressedModeMeasCapabFDDLst, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuCompressedModeMeasCapabTDDLst, &param->compressedModeMeasCapabTDDLst, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuCompressedModeMeasCapabGSMLst, &param->compressedModeMeasCapabGSMLst, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuCompressedModeMeasCapabMC, &param->compressedModeMeasCapabMC, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuMeasurementCapExt */



/*
* FUN : cmUnpkNhuUE_RadioAccessCapabBandFDDfddRF_Cap
*  DESC: Function to Unpack NhuUE_RadioAccessCapabBandFDDfddRF_Cap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapabBandFDDfddRF_Cap
(
NhuUE_RadioAccessCapabBandFDDfddRF_Cap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapabBandFDDfddRF_Cap(param, ptr, mBuf)
NhuUE_RadioAccessCapabBandFDDfddRF_Cap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_RadioAccessCapabBandFDDfddRF_Cap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuUE_PowerClassExt, &param->ue_PowerClass, mBuf); 
        CMCHKUNPK(cmUnpkNhuTxRxFrequencySeparation, &param->txRxFrequencySeparation, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCapabBandFDDfddRF_Cap */



/*
* FUN : cmUnpkNhuUE_RadioAccessCapabBandFDD
*  DESC: Function to Unpack NhuUE_RadioAccessCapabBandFDD structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapabBandFDD
(
NhuUE_RadioAccessCapabBandFDD *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapabBandFDD(param, ptr, mBuf)
NhuUE_RadioAccessCapabBandFDD *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_RadioAccessCapabBandFDD) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuRadioFrequencyBandFDD, &param->radioFrequencyBandFDD, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCapabBandFDDfddRF_Cap, &param->fddRF_Cap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuMeasurementCapExt, &param->measurementCap, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCapabBandFDD */



/*
* FUN : cmUnpkNhuUE_RadioAccessCapabBandFDDLst
*  DESC: Function to Unpack NhuUE_RadioAccessCapabBandFDDLst structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapabBandFDDLst
(
NhuUE_RadioAccessCapabBandFDDLst *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapabBandFDDLst(param, ptr, mBuf)
NhuUE_RadioAccessCapabBandFDDLst *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    Cntr i;
    TRC3(cmUnpkNhuUE_RadioAccessCapabBandFDDLst) 
    
    CMCHKUNPK(cmUnpkTknU16, &param->noComp, mBuf); 
    if( param->noComp.pres != NOTPRSNT ) 
    {
        CMGETMBLK(ptr, sizeof(NhuUE_RadioAccessCapabBandFDD)*(param->noComp.val), (Ptr*)&(param->member) );
        for(i = 0; i < param->noComp.val; i++)
        {
            CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCapabBandFDD, &(param->member[i]), ptr, mBuf); 
        }
    }
    return(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCapabBandFDDLst */



/*
* FUN : cmUnpkNhuUE_RadioAccessCap_v370ext
*  DESC: Function to Unpack NhuUE_RadioAccessCap_v370ext structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCap_v370ext
(
NhuUE_RadioAccessCap_v370ext *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCap_v370ext(param, ptr, mBuf)
NhuUE_RadioAccessCap_v370ext *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_RadioAccessCap_v370ext) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCapabBandFDDLst, &param->ue_RadioAccessCapabBandFDDLst, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCap_v370ext */



/*
* FUN : cmUnpkNhuUTRANUE_RadioAccessCapInfo
*  DESC: Function to Unpack NhuUTRANUE_RadioAccessCapInfo structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUTRANUE_RadioAccessCapInfo
(
NhuUTRANUE_RadioAccessCapInfo *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUTRANUE_RadioAccessCapInfo(param, ptr, mBuf)
NhuUTRANUE_RadioAccessCapInfo *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUTRANUE_RadioAccessCapInfo) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCap, &param->ue_RadioAccessCap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCap_v370ext, &param->ue_RadioAccessCap_v370ext, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUTRANUE_RadioAccessCapInfo */

#define cmUnpkNhuPredefinedConfigValueTag cmUnpkTknU32 


/*
* FUN : cmUnpkNhuPredefinedConfigStatusInfoother
*  DESC: Function to Unpack NhuPredefinedConfigStatusInfoother structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPredefinedConfigStatusInfoother
(
NhuPredefinedConfigStatusInfoother *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPredefinedConfigStatusInfoother(param, ptr, mBuf)
NhuPredefinedConfigStatusInfoother *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPredefinedConfigStatusInfoother) 
    
    CMCHKUNPK(cmUnpkTknU8, &param->choice, mBuf); 
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case OTHER_NOTSTORED :
                break;
            case OTHER_STOREDWITHDIFFERENTVALUETAG :
                CMCHKUNPK(cmUnpkNhuPredefinedConfigValueTag, &param->val.storedWithDifferentValueTag, mBuf); 
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPredefinedConfigStatusInfoother */



/*
* FUN : cmUnpkNhuPredefinedConfigStatusInfo
*  DESC: Function to Unpack NhuPredefinedConfigStatusInfo structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPredefinedConfigStatusInfo
(
NhuPredefinedConfigStatusInfo *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPredefinedConfigStatusInfo(param, ptr, mBuf)
NhuPredefinedConfigStatusInfo *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPredefinedConfigStatusInfo) 
    
    CMCHKUNPK(cmUnpkTknU8, &param->choice, mBuf); 
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case PREDEFINEDCONFIGSTATUSINFO_STOREDWITHVALUETAGSAMEASPREVIUS :
                break;
            case PREDEFINEDCONFIGSTATUSINFO_OTHER :
                CMCHKUNPKPTR(cmUnpkNhuPredefinedConfigStatusInfoother, &param->val.other, ptr, mBuf); 
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPredefinedConfigStatusInfo */



/*
* FUN : cmUnpkNhuPredefinedConfigStatusLst
*  DESC: Function to Unpack NhuPredefinedConfigStatusLst structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPredefinedConfigStatusLst
(
NhuPredefinedConfigStatusLst *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPredefinedConfigStatusLst(param, ptr, mBuf)
NhuPredefinedConfigStatusLst *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    Cntr i;
    TRC3(cmUnpkNhuPredefinedConfigStatusLst) 
    
    CMCHKUNPK(cmUnpkTknU16, &param->noComp, mBuf); 
    if( param->noComp.pres != NOTPRSNT ) 
    {
        CMGETMBLK(ptr, sizeof(NhuPredefinedConfigStatusInfo)*(param->noComp.val), (Ptr*)&(param->member) );
        for(i = 0; i < param->noComp.val; i++)
        {
            CMCHKUNPKPTR(cmUnpkNhuPredefinedConfigStatusInfo, &(param->member[i]), ptr, mBuf); 
        }
    }
    return(ROK);
}     /* End of function cmUnpkNhuPredefinedConfigStatusLst */

#define cmUnpkNhuSTART_Value cmUnpkTknStrBSXL 


/*
* FUN : cmUnpkNhuUE_SecInfo
*  DESC: Function to Unpack NhuUE_SecInfo structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_SecInfo
(
NhuUE_SecInfo *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_SecInfo(param, ptr, mBuf)
NhuUE_SecInfo *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_SecInfo) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuSTART_Value, &param->start_CS, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_SecInfo */

#define cmUnpkNhuUE_PositioningCapExt_v380rx_tx_TimeDifferenceTyp2Capable cmUnpkTknU8



/*
* FUN : cmUnpkNhuUE_PositioningCapExt_v380
*  DESC: Function to Unpack NhuUE_PositioningCapExt_v380 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_PositioningCapExt_v380
(
NhuUE_PositioningCapExt_v380 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_PositioningCapExt_v380(param, ptr, mBuf)
NhuUE_PositioningCapExt_v380 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_PositioningCapExt_v380) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuUE_PositioningCapExt_v380rx_tx_TimeDifferenceTyp2Capable, &param->rx_tx_TimeDifferenceTyp2Capable, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_PositioningCapExt_v380 */



/*
* FUN : cmUnpkNhuUE_RadioAccessCap_v380ext
*  DESC: Function to Unpack NhuUE_RadioAccessCap_v380ext structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCap_v380ext
(
NhuUE_RadioAccessCap_v380ext *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCap_v380ext(param, ptr, mBuf)
NhuUE_RadioAccessCap_v380ext *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_RadioAccessCap_v380ext) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuUE_PositioningCapExt_v380, &param->ue_PositioningCapExt_v380, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCap_v380ext */

#define cmUnpkNhuSupportOfDedicatedPilotsForChEstimation cmUnpkTknU32



/*
* FUN : cmUnpkNhuDL_PhysChCapFDD_v380ext
*  DESC: Function to Unpack NhuDL_PhysChCapFDD_v380ext structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuDL_PhysChCapFDD_v380ext
(
NhuDL_PhysChCapFDD_v380ext *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuDL_PhysChCapFDD_v380ext(param, ptr, mBuf)
NhuDL_PhysChCapFDD_v380ext *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuDL_PhysChCapFDD_v380ext) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuSupportOfDedicatedPilotsForChEstimation, &param->dummy, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuDL_PhysChCapFDD_v380ext */



/*
* FUN : cmUnpkNhuInterRATHovrInfo_v390ext_IEs
*  DESC: Function to Unpack NhuInterRATHovrInfo_v390ext_IEs structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v390ext_IEs
(
NhuInterRATHovrInfo_v390ext_IEs *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v390ext_IEs(param, ptr, mBuf)
NhuInterRATHovrInfo_v390ext_IEs *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuInterRATHovrInfo_v390ext_IEs) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCap_v380ext, &param->ue_RadioAccessCap_v380ext, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuDL_PhysChCapFDD_v380ext, &param->dl_PhysChCapFDD_v380ext, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuInterRATHovrInfo_v390ext_IEs */

#define cmUnpkNhuUE_PositioningCapExt_v3a0validity_CellPCH_UraPCH cmUnpkTknU32



/*
* FUN : cmUnpkNhuUE_PositioningCapExt_v3a0
*  DESC: Function to Unpack NhuUE_PositioningCapExt_v3a0 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_PositioningCapExt_v3a0
(
NhuUE_PositioningCapExt_v3a0 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_PositioningCapExt_v3a0(param, ptr, mBuf)
NhuUE_PositioningCapExt_v3a0 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_PositioningCapExt_v3a0) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuUE_PositioningCapExt_v3a0validity_CellPCH_UraPCH, &param->validity_CellPCH_UraPCH, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_PositioningCapExt_v3a0 */



/*
* FUN : cmUnpkNhuUE_RadioAccessCap_v3a0ext
*  DESC: Function to Unpack NhuUE_RadioAccessCap_v3a0ext structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCap_v3a0ext
(
NhuUE_RadioAccessCap_v3a0ext *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCap_v3a0ext(param, ptr, mBuf)
NhuUE_RadioAccessCap_v3a0ext *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_RadioAccessCap_v3a0ext) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuUE_PositioningCapExt_v3a0, &param->ue_PositioningCapExt_v3a0, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCap_v3a0ext */



/*
* FUN : cmUnpkNhuInterRATHovrInfo_v3a0ext_IEs
*  DESC: Function to Unpack NhuInterRATHovrInfo_v3a0ext_IEs structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v3a0ext_IEs
(
NhuInterRATHovrInfo_v3a0ext_IEs *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v3a0ext_IEs(param, ptr, mBuf)
NhuInterRATHovrInfo_v3a0ext_IEs *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuInterRATHovrInfo_v3a0ext_IEs) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCap_v3a0ext, &param->ue_RadioAccessCap_v3a0ext, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuInterRATHovrInfo_v3a0ext_IEs */

#define cmUnpkNhuUESpecificBehaviourInfo1interRAT cmUnpkTknStrBSXL 


/*
* FUN : cmUnpkNhuInterRATHovrInfo_v3d0ext_IEs
*  DESC: Function to Unpack NhuInterRATHovrInfo_v3d0ext_IEs structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v3d0ext_IEs
(
NhuInterRATHovrInfo_v3d0ext_IEs *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v3d0ext_IEs(param, ptr, mBuf)
NhuInterRATHovrInfo_v3d0ext_IEs *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuInterRATHovrInfo_v3d0ext_IEs) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuUESpecificBehaviourInfo1interRAT, &param->uESpecificBehaviourInfo1interRAT, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuInterRATHovrInfo_v3d0ext_IEs */

#define cmUnpkNhuUE_PositioningCapExt_v3g0sfn_sfnTyp2Cap cmUnpkTknU32



/*
* FUN : cmUnpkNhuUE_PositioningCapExt_v3g0
*  DESC: Function to Unpack NhuUE_PositioningCapExt_v3g0 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_PositioningCapExt_v3g0
(
NhuUE_PositioningCapExt_v3g0 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_PositioningCapExt_v3g0(param, ptr, mBuf)
NhuUE_PositioningCapExt_v3g0 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_PositioningCapExt_v3g0) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuUE_PositioningCapExt_v3g0sfn_sfnTyp2Cap, &param->sfn_sfnTyp2Cap, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_PositioningCapExt_v3g0 */



/*
* FUN : cmUnpkNhuUE_RadioAccessCap_v3g0ext
*  DESC: Function to Unpack NhuUE_RadioAccessCap_v3g0ext structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCap_v3g0ext
(
NhuUE_RadioAccessCap_v3g0ext *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCap_v3g0ext(param, ptr, mBuf)
NhuUE_RadioAccessCap_v3g0ext *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_RadioAccessCap_v3g0ext) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuUE_PositioningCapExt_v3g0, &param->ue_PositioningCapExt_v3g0, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCap_v3g0ext */



/*
* FUN : cmUnpkNhuInterRATHovrInfo_v3g0ext_IEs
*  DESC: Function to Unpack NhuInterRATHovrInfo_v3g0ext_IEs structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v3g0ext_IEs
(
NhuInterRATHovrInfo_v3g0ext_IEs *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v3g0ext_IEs(param, ptr, mBuf)
NhuInterRATHovrInfo_v3g0ext_IEs *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuInterRATHovrInfo_v3g0ext_IEs) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCap_v3g0ext, &param->ue_RadioAccessCap_v3g0ext, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuInterRATHovrInfo_v3g0ext_IEs */

#define cmUnpkNhuAccessStratumRlsIndicator cmUnpkTknU32



/*
* FUN : cmUnpkNhuInterRATHovrInfo_v4b0ext_IEs
*  DESC: Function to Unpack NhuInterRATHovrInfo_v4b0ext_IEs structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v4b0ext_IEs
(
NhuInterRATHovrInfo_v4b0ext_IEs *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v4b0ext_IEs(param, ptr, mBuf)
NhuInterRATHovrInfo_v4b0ext_IEs *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuInterRATHovrInfo_v4b0ext_IEs) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuAccessStratumRlsIndicator, &param->accessStratumRlsIndicator, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuInterRATHovrInfo_v4b0ext_IEs */



/*
* FUN : cmUnpkNhuInterRATHovrInfo_v4d0ext_IEs
*  DESC: Function to Unpack NhuInterRATHovrInfo_v4d0ext_IEs structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v4d0ext_IEs
(
NhuInterRATHovrInfo_v4d0ext_IEs *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v4d0ext_IEs(param, ptr, mBuf)
NhuInterRATHovrInfo_v4d0ext_IEs *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuInterRATHovrInfo_v4d0ext_IEs) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuRadioFrequencyBandTDDLst, &param->tdd128_RF_Cap, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuInterRATHovrInfo_v4d0ext_IEs */



/*
* FUN : cmUnpkNhuPredefinedConfigValueTagLst
*  DESC: Function to Unpack NhuPredefinedConfigValueTagLst structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPredefinedConfigValueTagLst
(
NhuPredefinedConfigValueTagLst *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPredefinedConfigValueTagLst(param, ptr, mBuf)
NhuPredefinedConfigValueTagLst *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    Cntr i;
    TRC3(cmUnpkNhuPredefinedConfigValueTagLst) 
    
    CMCHKUNPK(cmUnpkTknU16, &param->noComp, mBuf); 
    if( param->noComp.pres != NOTPRSNT ) 
    {
        CMGETMBLK(ptr, sizeof(NhuPredefinedConfigValueTag)*(param->noComp.val), (Ptr*)&(param->member) );
        for(i = 0; i < param->noComp.val; i++)
        {
            CMCHKUNPK(cmUnpkNhuPredefinedConfigValueTag, &(param->member[i]), mBuf); 
        }
    }
    return(ROK);
}     /* End of function cmUnpkNhuPredefinedConfigValueTagLst */

#define cmUnpkNhuPredefinedConfigSetWithDifferentValueTagstartPosition cmUnpkTknU32 


/*
* FUN : cmUnpkNhuPredefinedConfigSetWithDifferentValueTag
*  DESC: Function to Unpack NhuPredefinedConfigSetWithDifferentValueTag structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPredefinedConfigSetWithDifferentValueTag
(
NhuPredefinedConfigSetWithDifferentValueTag *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPredefinedConfigSetWithDifferentValueTag(param, ptr, mBuf)
NhuPredefinedConfigSetWithDifferentValueTag *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPredefinedConfigSetWithDifferentValueTag) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuPredefinedConfigSetWithDifferentValueTagstartPosition, &param->startPosition, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuPredefinedConfigValueTagLst, &param->valueTagLst, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPredefinedConfigSetWithDifferentValueTag */



/*
* FUN : cmUnpkNhuPredefinedConfigSetsWithDifferentValueTag
*  DESC: Function to Unpack NhuPredefinedConfigSetsWithDifferentValueTag structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPredefinedConfigSetsWithDifferentValueTag
(
NhuPredefinedConfigSetsWithDifferentValueTag *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPredefinedConfigSetsWithDifferentValueTag(param, ptr, mBuf)
NhuPredefinedConfigSetsWithDifferentValueTag *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    Cntr i;
    TRC3(cmUnpkNhuPredefinedConfigSetsWithDifferentValueTag) 
    
    CMCHKUNPK(cmUnpkTknU16, &param->noComp, mBuf); 
    if( param->noComp.pres != NOTPRSNT ) 
    {
        CMGETMBLK(ptr, sizeof(NhuPredefinedConfigSetWithDifferentValueTag)*(param->noComp.val), (Ptr*)&(param->member) );
        for(i = 0; i < param->noComp.val; i++)
        {
            CMCHKUNPKPTR(cmUnpkNhuPredefinedConfigSetWithDifferentValueTag, &(param->member[i]), ptr, mBuf); 
        }
    }
    return(ROK);
}     /* End of function cmUnpkNhuPredefinedConfigSetsWithDifferentValueTag */



/*
* FUN : cmUnpkNhuPredefinedConfigStatusLstVarSz
*  DESC: Function to Unpack NhuPredefinedConfigStatusLstVarSz structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPredefinedConfigStatusLstVarSz
(
NhuPredefinedConfigStatusLstVarSz *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPredefinedConfigStatusLstVarSz(param, ptr, mBuf)
NhuPredefinedConfigStatusLstVarSz *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    Cntr i;
    TRC3(cmUnpkNhuPredefinedConfigStatusLstVarSz) 
    
    CMCHKUNPK(cmUnpkTknU16, &param->noComp, mBuf); 
    if( param->noComp.pres != NOTPRSNT ) 
    {
        CMGETMBLK(ptr, sizeof(NhuPredefinedConfigStatusInfo)*(param->noComp.val), (Ptr*)&(param->member) );
        for(i = 0; i < param->noComp.val; i++)
        {
            CMCHKUNPKPTR(cmUnpkNhuPredefinedConfigStatusInfo, &(param->member[i]), ptr, mBuf); 
        }
    }
    return(ROK);
}     /* End of function cmUnpkNhuPredefinedConfigStatusLstVarSz */



/*
* FUN : cmUnpkNhuPredefinedConfigStatusLstComp
*  DESC: Function to Unpack NhuPredefinedConfigStatusLstComp structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPredefinedConfigStatusLstComp
(
NhuPredefinedConfigStatusLstComp *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPredefinedConfigStatusLstComp(param, ptr, mBuf)
NhuPredefinedConfigStatusLstComp *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPredefinedConfigStatusLstComp) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuPredefinedConfigSetsWithDifferentValueTag, &param->setsWithDifferentValueTag, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuPredefinedConfigStatusLstVarSz, &param->otherEntries, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPredefinedConfigStatusLstComp */

#define cmUnpkNhuRF_CapabBandFDDComp cmUnpkTknU32



/*
* FUN : cmUnpkNhuRF_CapabBandLstFDDComp
*  DESC: Function to Unpack NhuRF_CapabBandLstFDDComp structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuRF_CapabBandLstFDDComp
(
NhuRF_CapabBandLstFDDComp *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuRF_CapabBandLstFDDComp(param, ptr, mBuf)
NhuRF_CapabBandLstFDDComp *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    Cntr i;
    TRC3(cmUnpkNhuRF_CapabBandLstFDDComp) 
    
    CMCHKUNPK(cmUnpkTknU16, &param->noComp, mBuf); 
    if( param->noComp.pres != NOTPRSNT ) 
    {
        CMGETMBLK(ptr, sizeof(NhuRF_CapabBandFDDComp)*(param->noComp.val), (Ptr*)&(param->member) );
        for(i = 0; i < param->noComp.val; i++)
        {
            CMCHKUNPK(cmUnpkNhuRF_CapabBandFDDComp, &(param->member[i]), mBuf); 
        }
    }
    return(ROK);
}     /* End of function cmUnpkNhuRF_CapabBandLstFDDComp */



/*
* FUN : cmUnpkNhuRF_CapCompfdd
*  DESC: Function to Unpack NhuRF_CapCompfdd structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuRF_CapCompfdd
(
NhuRF_CapCompfdd *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuRF_CapCompfdd(param, ptr, mBuf)
NhuRF_CapCompfdd *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuRF_CapCompfdd) 
    
    CMCHKUNPK(cmUnpkTknU8, &param->choice, mBuf); 
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case FDD_NOTSUPP :
                break;
            case FDD_SUPPORTED :
                CMCHKUNPKPTR(cmUnpkNhuRF_CapabBandLstFDDComp, &param->val.supported, ptr, mBuf); 
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuRF_CapCompfdd */



/*
* FUN : cmUnpkNhuRF_CapComptdd384_RF_Cap
*  DESC: Function to Unpack NhuRF_CapComptdd384_RF_Cap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuRF_CapComptdd384_RF_Cap
(
NhuRF_CapComptdd384_RF_Cap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuRF_CapComptdd384_RF_Cap(param, ptr, mBuf)
NhuRF_CapComptdd384_RF_Cap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuRF_CapComptdd384_RF_Cap) 
    
    CMCHKUNPK(cmUnpkTknU8, &param->choice, mBuf); 
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case TDD384_RF_CAP_NOTSUPP :
                break;
            case TDD384_RF_CAP_SUPPORTED :
                CMCHKUNPK(cmUnpkNhuRadioFrequencyBandTDDLst, &param->val.supported, mBuf); 
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuRF_CapComptdd384_RF_Cap */



/*
* FUN : cmUnpkNhuRF_CapComptdd128_RF_Cap
*  DESC: Function to Unpack NhuRF_CapComptdd128_RF_Cap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuRF_CapComptdd128_RF_Cap
(
NhuRF_CapComptdd128_RF_Cap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuRF_CapComptdd128_RF_Cap(param, ptr, mBuf)
NhuRF_CapComptdd128_RF_Cap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuRF_CapComptdd128_RF_Cap) 
    
    CMCHKUNPK(cmUnpkTknU8, &param->choice, mBuf); 
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case TDD128_RF_CAP_NOTSUPP :
                break;
            case TDD128_RF_CAP_SUPPORTED :
                CMCHKUNPK(cmUnpkNhuRadioFrequencyBandTDDLst, &param->val.supported, mBuf); 
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuRF_CapComptdd128_RF_Cap */



/*
* FUN : cmUnpkNhuRF_CapComp
*  DESC: Function to Unpack NhuRF_CapComp structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuRF_CapComp
(
NhuRF_CapComp *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuRF_CapComp(param, ptr, mBuf)
NhuRF_CapComp *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuRF_CapComp) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuRF_CapCompfdd, &param->fdd, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuRF_CapComptdd384_RF_Cap, &param->tdd384_RF_Cap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuRF_CapComptdd128_RF_Cap, &param->tdd128_RF_Cap, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuRF_CapComp */

#define cmUnpkNhuUE_RadioAccessCapComptotalAM_RLCMemoryExceeds10kB cmUnpkTknU8



/*
* FUN : cmUnpkNhuUE_RadioAccessCapComp
*  DESC: Function to Unpack NhuUE_RadioAccessCapComp structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapComp
(
NhuUE_RadioAccessCapComp *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapComp(param, ptr, mBuf)
NhuUE_RadioAccessCapComp *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_RadioAccessCapComp) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuUE_RadioAccessCapComptotalAM_RLCMemoryExceeds10kB, &param->totalAM_RLCMemoryExceeds10kB, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuRF_CapComp, &param->rf_CapComp, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCapComp */



/*
* FUN : cmUnpkNhuInterRATHovrInfo_v590ext_IEs
*  DESC: Function to Unpack NhuInterRATHovrInfo_v590ext_IEs structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v590ext_IEs
(
NhuInterRATHovrInfo_v590ext_IEs *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v590ext_IEs(param, ptr, mBuf)
NhuInterRATHovrInfo_v590ext_IEs *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuInterRATHovrInfo_v590ext_IEs) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuPredefinedConfigStatusLstComp, &param->predefinedConfigStatusLstComp, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCapComp, &param->ue_RadioAccessCapComp, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuInterRATHovrInfo_v590ext_IEs */



/*
* FUN : cmUnpkNhuUE_SecInfo2
*  DESC: Function to Unpack NhuUE_SecInfo2 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_SecInfo2
(
NhuUE_SecInfo2 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_SecInfo2(param, ptr, mBuf)
NhuUE_SecInfo2 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_SecInfo2) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuSTART_Value, &param->start_PS, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_SecInfo2 */



/*
* FUN : cmUnpkNhuRF_CapabBandLstFDDComp_ext
*  DESC: Function to Unpack NhuRF_CapabBandLstFDDComp_ext structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuRF_CapabBandLstFDDComp_ext
(
NhuRF_CapabBandLstFDDComp_ext *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuRF_CapabBandLstFDDComp_ext(param, ptr, mBuf)
NhuRF_CapabBandLstFDDComp_ext *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    Cntr i;
    TRC3(cmUnpkNhuRF_CapabBandLstFDDComp_ext) 
    
    CMCHKUNPK(cmUnpkTknU16, &param->noComp, mBuf); 
    if( param->noComp.pres != NOTPRSNT ) 
    {
        CMGETMBLK(ptr, sizeof(NhuRF_CapabBandFDDComp)*(param->noComp.val), (Ptr*)&(param->member) );
        for(i = 0; i < param->noComp.val; i++)
        {
            CMCHKUNPK(cmUnpkNhuRF_CapabBandFDDComp, &(param->member[i]), mBuf); 
        }
    }
    return(ROK);
}     /* End of function cmUnpkNhuRF_CapabBandLstFDDComp_ext */



/*
* FUN : cmUnpkNhuUE_RadioAccessCapComp_ext
*  DESC: Function to Unpack NhuUE_RadioAccessCapComp_ext structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapComp_ext
(
NhuUE_RadioAccessCapComp_ext *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapComp_ext(param, ptr, mBuf)
NhuUE_RadioAccessCapComp_ext *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_RadioAccessCapComp_ext) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuRF_CapabBandLstFDDComp_ext, &param->rf_CapFDDComp, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCapComp_ext */

#define cmUnpkNhuDL_CapWithSimultaneousHS_DSCHConfig cmUnpkTknU32

#define cmUnpkNhuHSDSCH_physical_layer_category cmUnpkTknU32 
#define cmUnpkNhuPhysicalChannelCap_edch_r6fdd_edchsupportededch_PhysicalLyrCategory cmUnpkTknU32 


/*
* FUN : cmUnpkNhuPhysicalChannelCap_edch_r6fdd_edchsupported
*  DESC: Function to Unpack NhuPhysicalChannelCap_edch_r6fdd_edchsupported structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_edch_r6fdd_edchsupported
(
NhuPhysicalChannelCap_edch_r6fdd_edchsupported *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_edch_r6fdd_edchsupported(param, ptr, mBuf)
NhuPhysicalChannelCap_edch_r6fdd_edchsupported *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPhysicalChannelCap_edch_r6fdd_edchsupported) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuPhysicalChannelCap_edch_r6fdd_edchsupportededch_PhysicalLyrCategory, &param->edch_PhysicalLyrCategory, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPhysicalChannelCap_edch_r6fdd_edchsupported */



/*
* FUN : cmUnpkNhuPhysicalChannelCap_edch_r6fdd_edch
*  DESC: Function to Unpack NhuPhysicalChannelCap_edch_r6fdd_edch structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_edch_r6fdd_edch
(
NhuPhysicalChannelCap_edch_r6fdd_edch *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_edch_r6fdd_edch(param, ptr, mBuf)
NhuPhysicalChannelCap_edch_r6fdd_edch *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPhysicalChannelCap_edch_r6fdd_edch) 
    
    CMCHKUNPK(cmUnpkTknU8, &param->choice, mBuf); 
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case FDD_EDCH_SUPPORTED :
                CMCHKUNPKPTR(cmUnpkNhuPhysicalChannelCap_edch_r6fdd_edchsupported, &param->val.supported, ptr, mBuf); 
                break;
            case FDD_EDCH_UNSUPPORTED :
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPhysicalChannelCap_edch_r6fdd_edch */



/*
* FUN : cmUnpkNhuPhysicalChannelCap_edch_r6
*  DESC: Function to Unpack NhuPhysicalChannelCap_edch_r6 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_edch_r6
(
NhuPhysicalChannelCap_edch_r6 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_edch_r6(param, ptr, mBuf)
NhuPhysicalChannelCap_edch_r6 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPhysicalChannelCap_edch_r6) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuPhysicalChannelCap_edch_r6fdd_edch, &param->fdd_edch, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPhysicalChannelCap_edch_r6 */



/*
* FUN : cmUnpkNhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch
*  DESC: Function to Unpack NhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch
(
NhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch(param, ptr, mBuf)
NhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuDL_CapWithSimultaneousHS_DSCHConfig, &param->dl_CapWithSimultaneousHS_DSCHConfig, mBuf); 
        CMCHKUNPK(cmUnpkNhuHSDSCH_physical_layer_category, &param->physicalChannelCapabComp_hspdsch_r6, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuPhysicalChannelCap_edch_r6, &param->physicalChannelCap_edch_r6, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch */



/*
* FUN : cmUnpkNhuUE_RadioAccessCapComp2
*  DESC: Function to Unpack NhuUE_RadioAccessCapComp2 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapComp2
(
NhuUE_RadioAccessCapComp2 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapComp2(param, ptr, mBuf)
NhuUE_RadioAccessCapComp2 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_RadioAccessCapComp2) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch, &param->fddPhysicalChannelCapab_hspdsch_edch, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCapComp2 */



/*
* FUN : cmUnpkNhuInterRATHovrInfo_v690ext_IEs
*  DESC: Function to Unpack NhuInterRATHovrInfo_v690ext_IEs structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v690ext_IEs
(
NhuInterRATHovrInfo_v690ext_IEs *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v690ext_IEs(param, ptr, mBuf)
NhuInterRATHovrInfo_v690ext_IEs *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuInterRATHovrInfo_v690ext_IEs) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuUE_SecInfo2, &param->ue_SecInfo2, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCapComp_ext, &param->ue_RadioAccessCapComp, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCapComp2, &param->ue_RadioAccessCapComp2, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuInterRATHovrInfo_v690ext_IEs */

#define cmUnpkNhuInterRATHovrInfo_v6b0ext_IEssupportForSIB11bis cmUnpkTknU32



/*
* FUN : cmUnpkNhuInterRATHovrInfo_v6b0ext_IEs
*  DESC: Function to Unpack NhuInterRATHovrInfo_v6b0ext_IEs structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v6b0ext_IEs
(
NhuInterRATHovrInfo_v6b0ext_IEs *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v6b0ext_IEs(param, ptr, mBuf)
NhuInterRATHovrInfo_v6b0ext_IEs *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuInterRATHovrInfo_v6b0ext_IEs) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuInterRATHovrInfo_v6b0ext_IEssupportForSIB11bis, &param->supportForSIB11bis, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuInterRATHovrInfo_v6b0ext_IEs */



/*
* FUN : cmUnpkNhuUTRANInterRATHovrInfopredefinedConfigStatusLst
*  DESC: Function to Unpack NhuUTRANInterRATHovrInfopredefinedConfigStatusLst structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfopredefinedConfigStatusLst
(
NhuUTRANInterRATHovrInfopredefinedConfigStatusLst *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfopredefinedConfigStatusLst(param, ptr, mBuf)
NhuUTRANInterRATHovrInfopredefinedConfigStatusLst *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUTRANInterRATHovrInfopredefinedConfigStatusLst) 
    
    CMCHKUNPK(cmUnpkTknU8, &param->choice, mBuf); 
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case PREDEFINEDCONFIGSTATUSLST_ABSENT :
                break;
            case PREDEFINEDCONFIGSTATUSLST_PRESENT :
                CMCHKUNPKPTR(cmUnpkNhuPredefinedConfigStatusLst, &param->val.present, ptr, mBuf); 
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUTRANInterRATHovrInfopredefinedConfigStatusLst */



/*
* FUN : cmUnpkNhuUTRANInterRATHovrInfouE_SecInfo
*  DESC: Function to Unpack NhuUTRANInterRATHovrInfouE_SecInfo structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfouE_SecInfo
(
NhuUTRANInterRATHovrInfouE_SecInfo *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfouE_SecInfo(param, ptr, mBuf)
NhuUTRANInterRATHovrInfouE_SecInfo *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUTRANInterRATHovrInfouE_SecInfo) 
    
    CMCHKUNPK(cmUnpkTknU8, &param->choice, mBuf); 
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case UE_SECINFO_ABSENT :
                break;
            case UE_SECINFO_PRESENT :
                CMCHKUNPKPTR(cmUnpkNhuUE_SecInfo, &param->val.present, ptr, mBuf); 
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUTRANInterRATHovrInfouE_SecInfo */

#define cmUnpkNhuUTRANInterRATHovrInfoue_CapContpresent cmUnpkTknStrOSXL 


/*
* FUN : cmUnpkNhuUTRANInterRATHovrInfoue_CapCont
*  DESC: Function to Unpack NhuUTRANInterRATHovrInfoue_CapCont structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfoue_CapCont
(
NhuUTRANInterRATHovrInfoue_CapCont *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfoue_CapCont(param, ptr, mBuf)
NhuUTRANInterRATHovrInfoue_CapCont *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUTRANInterRATHovrInfoue_CapCont) 
    
    CMCHKUNPK(cmUnpkTknU8, &param->choice, mBuf); 
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case UE_CAPCONT_ABSENT :
                break;
            case UE_CAPCONT_PRESENT :
                NHUCHKUNPKPTR(cmUnpkNhuUTRANInterRATHovrInfoue_CapContpresent, &param->val.present, ptr, mBuf); 
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUTRANInterRATHovrInfoue_CapCont */

#define cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsinterRATHovrInfo_r3_add_ext cmUnpkTknStrBSXL 


/*
* FUN : cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns
*  DESC: Function to Unpack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns(param, ptr, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns */



/*
* FUN : cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns
*  DESC: Function to Unpack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns(param, ptr, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuInterRATHovrInfo_v6b0ext_IEs, &param->interRATHovrInfo_v6b0ext, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns, &param->nonCriticalExtns, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns */



/*
* FUN : cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns
*  DESC: Function to Unpack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns(param, ptr, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuInterRATHovrInfo_v690ext_IEs, &param->interRATHovrInfo_v690ext, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns, &param->v6b0NonCriticalExtns, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns */



/*
* FUN : cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns
*  DESC: Function to Unpack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns(param, ptr, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuInterRATHovrInfo_v590ext_IEs, &param->interRATHovrInfo_v590ext, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns, &param->v690NonCriticalExtns, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns */



/*
* FUN : cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns
*  DESC: Function to Unpack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns(param, ptr, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuInterRATHovrInfo_v4d0ext_IEs, &param->interRATHovrInfo_v4d0ext, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns, &param->v590NonCriticalExtns, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns */



/*
* FUN : cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns
*  DESC: Function to Unpack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns(param, ptr, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuInterRATHovrInfo_v4b0ext_IEs, &param->interRATHovrInfo_v4b0ext, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns, &param->v4d0NonCriticalExtns, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns */



/*
* FUN : cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns
*  DESC: Function to Unpack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns(param, ptr, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuInterRATHovrInfo_v3g0ext_IEs, &param->interRATHovrInfo_v3g0ext, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns, &param->v4b0NonCriticalExtns, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns */



/*
* FUN : cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns
*  DESC: Function to Unpack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns(param, ptr, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuInterRATHovrInfo_v3d0ext_IEs, &param->interRATHovrInfo_v3d0ext, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsinterRATHovrInfo_r3_add_ext, &param->interRATHovrInfo_r3_add_ext, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns, &param->v3g0NonCriticalExtns, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns */



/*
* FUN : cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns
*  DESC: Function to Unpack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns(param, ptr, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuInterRATHovrInfo_v3a0ext_IEs, &param->interRATHovrInfo_v3a0ext, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns, &param->laterNonCriticalExtns, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns */



/*
* FUN : cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresent
*  DESC: Function to Unpack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresent structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresent
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresent *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresent(param, ptr, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresent *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresent) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuInterRATHovrInfo_v390ext_IEs, &param->interRATHovrInfo_v390ext, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns, &param->v3a0NonCriticalExtns, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresent */



/*
* FUN : cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtns
*  DESC: Function to Unpack NhuUTRANInterRATHovrInfov390NonCriticalExtns structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtns *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtns(param, ptr, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtns *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtns) 
    
    CMCHKUNPK(cmUnpkTknU8, &param->choice, mBuf); 
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case V390NONCRITICALEXTNS_ABSENT :
                break;
            case V390NONCRITICALEXTNS_PRESENT :
                CMCHKUNPKPTR(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresent, &param->val.present, ptr, mBuf); 
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtns */



/*
* FUN : cmUnpkNhuUTRANInterRATHovrInfo
*  DESC: Function to Unpack NhuUTRANInterRATHovrInfo structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfo
(
NhuUTRANInterRATHovrInfo *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUTRANInterRATHovrInfo(param, ptr, mBuf)
NhuUTRANInterRATHovrInfo *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUTRANInterRATHovrInfo) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuUTRANInterRATHovrInfopredefinedConfigStatusLst, &param->predefinedConfigStatusLst, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUTRANInterRATHovrInfouE_SecInfo, &param->uE_SecInfo, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUTRANInterRATHovrInfoue_CapCont, &param->ue_CapCont, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUTRANInterRATHovrInfov390NonCriticalExtns, &param->v390NonCriticalExtns, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUTRANInterRATHovrInfo */

#define cmUnpkNhuRadioFrequencyBandFDD2 cmUnpkTknU32

#define cmUnpkNhuCompressedModeMeasCapabFDD2dl_MeasurementsFDD cmUnpkTknU8

#define cmUnpkNhuCompressedModeMeasCapabFDD2ul_MeasurementsFDD cmUnpkTknU8



/*
* FUN : cmUnpkNhuCompressedModeMeasCapabFDD2
*  DESC: Function to Unpack NhuCompressedModeMeasCapabFDD2 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabFDD2
(
NhuCompressedModeMeasCapabFDD2 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabFDD2(param, ptr, mBuf)
NhuCompressedModeMeasCapabFDD2 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuCompressedModeMeasCapabFDD2) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuRadioFrequencyBandFDD, &param->radioFrequencyBandFDD, mBuf); 
        CMCHKUNPK(cmUnpkNhuRadioFrequencyBandFDD2, &param->radioFrequencyBandFDD2, mBuf); 
        CMCHKUNPK(cmUnpkNhuCompressedModeMeasCapabFDD2dl_MeasurementsFDD, &param->dl_MeasurementsFDD, mBuf); 
        CMCHKUNPK(cmUnpkNhuCompressedModeMeasCapabFDD2ul_MeasurementsFDD, &param->ul_MeasurementsFDD, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuCompressedModeMeasCapabFDD2 */



/*
* FUN : cmUnpkNhuCompressedModeMeasCapabFDDLst2
*  DESC: Function to Unpack NhuCompressedModeMeasCapabFDDLst2 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabFDDLst2
(
NhuCompressedModeMeasCapabFDDLst2 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabFDDLst2(param, ptr, mBuf)
NhuCompressedModeMeasCapabFDDLst2 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    Cntr i;
    TRC3(cmUnpkNhuCompressedModeMeasCapabFDDLst2) 
    
    CMCHKUNPK(cmUnpkTknU16, &param->noComp, mBuf); 
    if( param->noComp.pres != NOTPRSNT ) 
    {
        CMGETMBLK(ptr, sizeof(NhuCompressedModeMeasCapabFDD2)*(param->noComp.val), (Ptr*)&(param->member) );
        for(i = 0; i < param->noComp.val; i++)
        {
            CMCHKUNPKPTR(cmUnpkNhuCompressedModeMeasCapabFDD2, &(param->member[i]), ptr, mBuf); 
        }
    }
    return(ROK);
}     /* End of function cmUnpkNhuCompressedModeMeasCapabFDDLst2 */



/*
* FUN : cmUnpkNhuMeasurementCapExt2
*  DESC: Function to Unpack NhuMeasurementCapExt2 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuMeasurementCapExt2
(
NhuMeasurementCapExt2 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuMeasurementCapExt2(param, ptr, mBuf)
NhuMeasurementCapExt2 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuMeasurementCapExt2) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuCompressedModeMeasCapabFDDLst2, &param->compressedModeMeasCapabFDDLst, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuCompressedModeMeasCapabTDDLst, &param->compressedModeMeasCapabTDDLst, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuCompressedModeMeasCapabGSMLst, &param->compressedModeMeasCapabGSMLst, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuCompressedModeMeasCapabMC, &param->compressedModeMeasCapabMC, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuMeasurementCapExt2 */



/*
* FUN : cmUnpkNhuUE_RadioAccessCapabBandFDD2fddRF_Cap
*  DESC: Function to Unpack NhuUE_RadioAccessCapabBandFDD2fddRF_Cap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapabBandFDD2fddRF_Cap
(
NhuUE_RadioAccessCapabBandFDD2fddRF_Cap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapabBandFDD2fddRF_Cap(param, ptr, mBuf)
NhuUE_RadioAccessCapabBandFDD2fddRF_Cap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_RadioAccessCapabBandFDD2fddRF_Cap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuUE_PowerClassExt, &param->ue_PowerClass, mBuf); 
        CMCHKUNPK(cmUnpkNhuTxRxFrequencySeparation, &param->txRxFrequencySeparation, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCapabBandFDD2fddRF_Cap */



/*
* FUN : cmUnpkNhuUE_RadioAccessCapabBandFDD2
*  DESC: Function to Unpack NhuUE_RadioAccessCapabBandFDD2 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapabBandFDD2
(
NhuUE_RadioAccessCapabBandFDD2 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapabBandFDD2(param, ptr, mBuf)
NhuUE_RadioAccessCapabBandFDD2 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_RadioAccessCapabBandFDD2) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuRadioFrequencyBandFDD2, &param->radioFrequencyBandFDD2, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCapabBandFDD2fddRF_Cap, &param->fddRF_Cap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuMeasurementCapExt2, &param->measurementCap2, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCapabBandFDD2 */



/*
* FUN : cmUnpkNhuUE_RadioAccessCapabBandFDDLst2
*  DESC: Function to Unpack NhuUE_RadioAccessCapabBandFDDLst2 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapabBandFDDLst2
(
NhuUE_RadioAccessCapabBandFDDLst2 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapabBandFDDLst2(param, ptr, mBuf)
NhuUE_RadioAccessCapabBandFDDLst2 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    Cntr i;
    TRC3(cmUnpkNhuUE_RadioAccessCapabBandFDDLst2) 
    
    CMCHKUNPK(cmUnpkTknU16, &param->noComp, mBuf); 
    if( param->noComp.pres != NOTPRSNT ) 
    {
        CMGETMBLK(ptr, sizeof(NhuUE_RadioAccessCapabBandFDD2)*(param->noComp.val), (Ptr*)&(param->member) );
        for(i = 0; i < param->noComp.val; i++)
        {
            CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCapabBandFDD2, &(param->member[i]), ptr, mBuf); 
        }
    }
    return(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCapabBandFDDLst2 */

#define cmUnpkNhuCompressedModeMeasCapabFDD_extdl_MeasurementsFDD cmUnpkTknU8

#define cmUnpkNhuCompressedModeMeasCapabFDD_extul_MeasurementsFDD cmUnpkTknU8



/*
* FUN : cmUnpkNhuCompressedModeMeasCapabFDD_ext
*  DESC: Function to Unpack NhuCompressedModeMeasCapabFDD_ext structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabFDD_ext
(
NhuCompressedModeMeasCapabFDD_ext *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabFDD_ext(param, ptr, mBuf)
NhuCompressedModeMeasCapabFDD_ext *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuCompressedModeMeasCapabFDD_ext) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuRadioFrequencyBandFDD2, &param->radioFrequencyBandFDD2, mBuf); 
        CMCHKUNPK(cmUnpkNhuCompressedModeMeasCapabFDD_extdl_MeasurementsFDD, &param->dl_MeasurementsFDD, mBuf); 
        CMCHKUNPK(cmUnpkNhuCompressedModeMeasCapabFDD_extul_MeasurementsFDD, &param->ul_MeasurementsFDD, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuCompressedModeMeasCapabFDD_ext */



/*
* FUN : cmUnpkNhuCompressedModeMeasCapabFDDLst_ext
*  DESC: Function to Unpack NhuCompressedModeMeasCapabFDDLst_ext structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabFDDLst_ext
(
NhuCompressedModeMeasCapabFDDLst_ext *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuCompressedModeMeasCapabFDDLst_ext(param, ptr, mBuf)
NhuCompressedModeMeasCapabFDDLst_ext *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    Cntr i;
    TRC3(cmUnpkNhuCompressedModeMeasCapabFDDLst_ext) 
    
    CMCHKUNPK(cmUnpkTknU16, &param->noComp, mBuf); 
    if( param->noComp.pres != NOTPRSNT ) 
    {
        CMGETMBLK(ptr, sizeof(NhuCompressedModeMeasCapabFDD_ext)*(param->noComp.val), (Ptr*)&(param->member) );
        for(i = 0; i < param->noComp.val; i++)
        {
            CMCHKUNPKPTR(cmUnpkNhuCompressedModeMeasCapabFDD_ext, &(param->member[i]), ptr, mBuf); 
        }
    }
    return(ROK);
}     /* End of function cmUnpkNhuCompressedModeMeasCapabFDDLst_ext */



/*
* FUN : cmUnpkNhuUE_RadioAccessCapabBandFDD_ext
*  DESC: Function to Unpack NhuUE_RadioAccessCapabBandFDD_ext structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapabBandFDD_ext
(
NhuUE_RadioAccessCapabBandFDD_ext *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapabBandFDD_ext(param, ptr, mBuf)
NhuUE_RadioAccessCapabBandFDD_ext *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_RadioAccessCapabBandFDD_ext) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuRadioFrequencyBandFDD, &param->radioFrequencyBandFDD, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuCompressedModeMeasCapabFDDLst_ext, &param->compressedModeMeasCapabFDDLst_ext, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCapabBandFDD_ext */



/*
* FUN : cmUnpkNhuUE_RadioAccessCapabBandFDDLst_ext
*  DESC: Function to Unpack NhuUE_RadioAccessCapabBandFDDLst_ext structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapabBandFDDLst_ext
(
NhuUE_RadioAccessCapabBandFDDLst_ext *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCapabBandFDDLst_ext(param, ptr, mBuf)
NhuUE_RadioAccessCapabBandFDDLst_ext *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    Cntr i;
    TRC3(cmUnpkNhuUE_RadioAccessCapabBandFDDLst_ext) 
    
    CMCHKUNPK(cmUnpkTknU16, &param->noComp, mBuf); 
    if( param->noComp.pres != NOTPRSNT ) 
    {
        CMGETMBLK(ptr, sizeof(NhuUE_RadioAccessCapabBandFDD_ext)*(param->noComp.val), (Ptr*)&(param->member) );
        for(i = 0; i < param->noComp.val; i++)
        {
            CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCapabBandFDD_ext, &(param->member[i]), ptr, mBuf); 
        }
    }
    return(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCapabBandFDDLst_ext */



/*
* FUN : cmUnpkNhuUE_RadioAccessCap_v650ext
*  DESC: Function to Unpack NhuUE_RadioAccessCap_v650ext structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCap_v650ext
(
NhuUE_RadioAccessCap_v650ext *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCap_v650ext(param, ptr, mBuf)
NhuUE_RadioAccessCap_v650ext *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_RadioAccessCap_v650ext) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCapabBandFDDLst2, &param->ue_RadioAccessCapabBandFDDLst2, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCapabBandFDDLst_ext, &param->ue_RadioAccessCapabBandFDDLst_ext, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCap_v650ext */



/*
* FUN : cmUnpkNhuInterRATHovrInfo_v690ext1_IEs
*  DESC: Function to Unpack NhuInterRATHovrInfo_v690ext1_IEs structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v690ext1_IEs
(
NhuInterRATHovrInfo_v690ext1_IEs *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_v690ext1_IEs(param, ptr, mBuf)
NhuInterRATHovrInfo_v690ext1_IEs *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuInterRATHovrInfo_v690ext1_IEs) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuUE_RadioAccessCap_v650ext, &param->ue_RadioAccessCap_v650ext, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuInterRATHovrInfo_v690ext1_IEs */



/*
* FUN : cmUnpkNhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns
*  DESC: Function to Unpack NhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns
(
NhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns(param, ptr, mBuf)
NhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns */



/*
* FUN : cmUnpkNhuInterRATHovrInfo_r3_add_ext_IEs
*  DESC: Function to Unpack NhuInterRATHovrInfo_r3_add_ext_IEs structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_r3_add_ext_IEs
(
NhuInterRATHovrInfo_r3_add_ext_IEs *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuInterRATHovrInfo_r3_add_ext_IEs(param, ptr, mBuf)
NhuInterRATHovrInfo_r3_add_ext_IEs *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuInterRATHovrInfo_r3_add_ext_IEs) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuInterRATHovrInfo_v690ext1_IEs, &param->interRATHovrInfo_v690ext1, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns, &param->nonCriticalExtns, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuInterRATHovrInfo_r3_add_ext_IEs */

#define cmUnpkNhuRadioFrequencyBandTDDLst_r7 cmUnpkTknU32

#define cmUnpkNhuChipRateCap_r7 cmUnpkTknU32



/*
* FUN : cmUnpkNhuRF_Cap_r7fddRF_Cap
*  DESC: Function to Unpack NhuRF_Cap_r7fddRF_Cap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuRF_Cap_r7fddRF_Cap
(
NhuRF_Cap_r7fddRF_Cap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuRF_Cap_r7fddRF_Cap(param, ptr, mBuf)
NhuRF_Cap_r7fddRF_Cap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuRF_Cap_r7fddRF_Cap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuUE_PowerClass, &param->ue_PowerClass, mBuf); 
        CMCHKUNPK(cmUnpkNhuTxRxFrequencySeparation, &param->txRxFrequencySeparation, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuRF_Cap_r7fddRF_Cap */



/*
* FUN : cmUnpkNhuRF_Cap_r7tddRF_Cap
*  DESC: Function to Unpack NhuRF_Cap_r7tddRF_Cap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuRF_Cap_r7tddRF_Cap
(
NhuRF_Cap_r7tddRF_Cap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuRF_Cap_r7tddRF_Cap(param, ptr, mBuf)
NhuRF_Cap_r7tddRF_Cap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuRF_Cap_r7tddRF_Cap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuUE_PowerClass, &param->ue_PowerClass, mBuf); 
        CMCHKUNPK(cmUnpkNhuRadioFrequencyBandTDDLst_r7, &param->radioFrequencyTDDBandLst, mBuf); 
        CMCHKUNPK(cmUnpkNhuChipRateCap_r7, &param->chipRateCap, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuRF_Cap_r7tddRF_Cap */



/*
* FUN : cmUnpkNhuRF_Cap_r7
*  DESC: Function to Unpack NhuRF_Cap_r7 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuRF_Cap_r7
(
NhuRF_Cap_r7 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuRF_Cap_r7(param, ptr, mBuf)
NhuRF_Cap_r7 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuRF_Cap_r7) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuRF_Cap_r7fddRF_Cap, &param->fddRF_Cap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuRF_Cap_r7tddRF_Cap, &param->tddRF_Cap, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuRF_Cap_r7 */

#define cmUnpkNhuMaxPhysChPerFrame_768 cmUnpkTknU32 
#define cmUnpkNhuMinimumSF_DL_768 cmUnpkTknU32

#define cmUnpkNhuMaxPhysChPerTS_768 cmUnpkTknU32 
#define cmUnpkNhuDL_PhysChCapTDD_768supportOfPDSCH cmUnpkTknU8



/*
* FUN : cmUnpkNhuDL_PhysChCapTDD_768
*  DESC: Function to Unpack NhuDL_PhysChCapTDD_768 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuDL_PhysChCapTDD_768
(
NhuDL_PhysChCapTDD_768 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuDL_PhysChCapTDD_768(param, ptr, mBuf)
NhuDL_PhysChCapTDD_768 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuDL_PhysChCapTDD_768) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuMaxTS_PerFrame, &param->maxTS_PerFrame, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxPhysChPerFrame_768, &param->maxPhysChPerFrame, mBuf); 
        CMCHKUNPK(cmUnpkNhuMinimumSF_DL_768, &param->minimumSF, mBuf); 
        CMCHKUNPK(cmUnpkNhuDL_PhysChCapTDD_768supportOfPDSCH, &param->supportOfPDSCH, mBuf); 
        CMCHKUNPK(cmUnpkNhuMaxPhysChPerTS_768, &param->maxPhysChPerTS, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuDL_PhysChCapTDD_768 */



/*
* FUN : cmUnpkNhuPhysicalChannelCap_r7fddPhysChCap
*  DESC: Function to Unpack NhuPhysicalChannelCap_r7fddPhysChCap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_r7fddPhysChCap
(
NhuPhysicalChannelCap_r7fddPhysChCap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_r7fddPhysChCap(param, ptr, mBuf)
NhuPhysicalChannelCap_r7fddPhysChCap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPhysicalChannelCap_r7fddPhysChCap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuDL_PhysChCapFDD, &param->downlinkPhysChCap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUL_PhysChCapFDD, &param->uplinkPhysChCap, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPhysicalChannelCap_r7fddPhysChCap */



/*
* FUN : cmUnpkNhuPhysicalChannelCap_r7tddPhysChCap
*  DESC: Function to Unpack NhuPhysicalChannelCap_r7tddPhysChCap structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_r7tddPhysChCap
(
NhuPhysicalChannelCap_r7tddPhysChCap *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_r7tddPhysChCap(param, ptr, mBuf)
NhuPhysicalChannelCap_r7tddPhysChCap *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPhysicalChannelCap_r7tddPhysChCap) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuDL_PhysChCapTDD, &param->downlinkPhysChCap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUL_PhysChCapTDD, &param->uplinkPhysChCap, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPhysicalChannelCap_r7tddPhysChCap */



/*
* FUN : cmUnpkNhuPhysicalChannelCap_r7tddPhysChCap_768
*  DESC: Function to Unpack NhuPhysicalChannelCap_r7tddPhysChCap_768 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_r7tddPhysChCap_768
(
NhuPhysicalChannelCap_r7tddPhysChCap_768 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_r7tddPhysChCap_768(param, ptr, mBuf)
NhuPhysicalChannelCap_r7tddPhysChCap_768 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPhysicalChannelCap_r7tddPhysChCap_768) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuDL_PhysChCapTDD_768, &param->downlinkPhysChCap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuUL_PhysChCapTDD, &param->uplinkPhysChCap, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPhysicalChannelCap_r7tddPhysChCap_768 */



/*
* FUN : cmUnpkNhuPhysicalChannelCap_r7
*  DESC: Function to Unpack NhuPhysicalChannelCap_r7 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_r7
(
NhuPhysicalChannelCap_r7 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_r7(param, ptr, mBuf)
NhuPhysicalChannelCap_r7 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPhysicalChannelCap_r7) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuPhysicalChannelCap_r7fddPhysChCap, &param->fddPhysChCap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuPhysicalChannelCap_r7tddPhysChCap, &param->tddPhysChCap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuPhysicalChannelCap_r7tddPhysChCap_768, &param->tddPhysChCap_768, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPhysicalChannelCap_r7 */

#define cmUnpkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupportedtdd_edch_PhysicalLyrCategory cmUnpkTknU32 


/*
* FUN : cmUnpkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupported
*  DESC: Function to Unpack NhuPhysicalChannelCap_tddedch_r7tdd_edchsupported structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupported
(
NhuPhysicalChannelCap_tddedch_r7tdd_edchsupported *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupported(param, ptr, mBuf)
NhuPhysicalChannelCap_tddedch_r7tdd_edchsupported *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupported) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPK(cmUnpkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupportedtdd_edch_PhysicalLyrCategory, &param->tdd_edch_PhysicalLyrCategory, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupported */



/*
* FUN : cmUnpkNhuPhysicalChannelCap_tddedch_r7tdd_edch
*  DESC: Function to Unpack NhuPhysicalChannelCap_tddedch_r7tdd_edch structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_tddedch_r7tdd_edch
(
NhuPhysicalChannelCap_tddedch_r7tdd_edch *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_tddedch_r7tdd_edch(param, ptr, mBuf)
NhuPhysicalChannelCap_tddedch_r7tdd_edch *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPhysicalChannelCap_tddedch_r7tdd_edch) 
    
    CMCHKUNPK(cmUnpkTknU8, &param->choice, mBuf); 
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case TDD_EDCH_SUPPORTED :
                CMCHKUNPKPTR(cmUnpkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupported, &param->val.supported, ptr, mBuf); 
                break;
            case TDD_EDCH_UNSUPPORTED :
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPhysicalChannelCap_tddedch_r7tdd_edch */



/*
* FUN : cmUnpkNhuPhysicalChannelCap_tddedch_r7
*  DESC: Function to Unpack NhuPhysicalChannelCap_tddedch_r7 structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_tddedch_r7
(
NhuPhysicalChannelCap_tddedch_r7 *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuPhysicalChannelCap_tddedch_r7(param, ptr, mBuf)
NhuPhysicalChannelCap_tddedch_r7 *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuPhysicalChannelCap_tddedch_r7) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuPhysicalChannelCap_tddedch_r7tdd_edch, &param->tdd_edch, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuPhysicalChannelCap_tddedch_r7 */



/*
* FUN : cmUnpkNhuUE_RadioAccessCap_v7xyext
*  DESC: Function to Unpack NhuUE_RadioAccessCap_v7xyext structure 
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCap_v7xyext
(
NhuUE_RadioAccessCap_v7xyext *param,
Ptr ptr,
Buffer *mBuf
)
#else 
PUBLIC S16 cmUnpkNhuUE_RadioAccessCap_v7xyext(param, ptr, mBuf)
NhuUE_RadioAccessCap_v7xyext *param;
Ptr ptr; 
Buffer *mBuf;
#endif
{
    
    TRC3(cmUnpkNhuUE_RadioAccessCap_v7xyext) 
    
    CMCHKUNPK( cmUnpkTknPres, &param->pres, mBuf); 
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKUNPKPTR(cmUnpkNhuRF_Cap_r7, &param->rf_Cap, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuPhysicalChannelCap_r7, &param->physicalChannelCap_r7, ptr, mBuf); 
        CMCHKUNPKPTR(cmUnpkNhuPhysicalChannelCap_tddedch_r7, &param->physicalchannelcapability_tddedch, ptr, mBuf); 
    }
    RETVALUE(ROK);
}     /* End of function cmUnpkNhuUE_RadioAccessCap_v7xyext */

/**********************************************************************
         End of file:     nhu_3gunpk.c@@/main/1 - Fri Dec 11 18:45:15 2009
**********************************************************************/
/**********************************************************************

        Revision history:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---    ch        1. RRC LTE Initial Release.
*********************************************************************91*/
