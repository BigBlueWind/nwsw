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

     File:     nhu_3gpk.c 

     Sid:      nhu_3gpk.c@@/main/1 - Fri Dec 11 18:45:14 2009

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
#include "nhu_3gpk.x"

#define cmPkNhuMaxHcCtxtSpace cmPkTknU32
#define cmPkNhuPDCP_CaplosslessSRNS_RelocationSupport cmPkTknU8


/* 
* FUN : cmPkNhuPDCP_CapsupportForRfc2507
*  DESC: Function to Pack NhuPDCP_CapsupportForRfc2507 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPDCP_CapsupportForRfc2507
(
NhuPDCP_CapsupportForRfc2507 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPDCP_CapsupportForRfc2507(param, mBuf)
NhuPDCP_CapsupportForRfc2507 *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPDCP_CapsupportForRfc2507) 
    
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case SUPPORTFORRFC2507_SUPPORTED :
                CMCHKPK(cmPkNhuMaxHcCtxtSpace, &param->val.supported, mBuf); 
                break;
            case SUPPORTFORRFC2507_NOTSUPP :
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    CMCHKPK(cmPkTknU8, &param->choice, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPDCP_CapsupportForRfc2507 */



/* 
* FUN : cmPkNhuPDCP_Cap
*  DESC: Function to Pack NhuPDCP_Cap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPDCP_Cap
(
NhuPDCP_Cap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPDCP_Cap(param, mBuf)
NhuPDCP_Cap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPDCP_Cap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuPDCP_CapsupportForRfc2507, &param->supportForRfc2507, mBuf); 
        CMCHKPK(cmPkNhuPDCP_CaplosslessSRNS_RelocationSupport, &param->losslessSRNS_RelocationSupport, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPDCP_Cap */

#define cmPkNhuTotalRLC_AM_BufferSize cmPkTknU32
#define cmPkNhuMaxRLC_WindowSize cmPkTknU32
#define cmPkNhuMaxAM_EntityNumberRLC_Cap cmPkTknU32


/* 
* FUN : cmPkNhuRLC_Cap
*  DESC: Function to Pack NhuRLC_Cap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuRLC_Cap
(
NhuRLC_Cap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuRLC_Cap(param, mBuf)
NhuRLC_Cap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuRLC_Cap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuMaxAM_EntityNumberRLC_Cap, &param->maximumAM_EntityNumber, mBuf); 
        CMCHKPK(cmPkNhuMaxRLC_WindowSize, &param->maximumRLC_WindowSize, mBuf); 
        CMCHKPK(cmPkNhuTotalRLC_AM_BufferSize, &param->totalRLC_AM_BufferSize, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuRLC_Cap */

#define cmPkNhuMaxNoBits cmPkTknU32


/* 
* FUN : cmPkNhuTurboSupport
*  DESC: Function to Pack NhuTurboSupport structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuTurboSupport
(
NhuTurboSupport *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuTurboSupport(param, mBuf)
NhuTurboSupport *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuTurboSupport) 
    
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case TURBOSUPPORT_SUPPORTED :
                CMCHKPK(cmPkNhuMaxNoBits, &param->val.supported, mBuf); 
                break;
            case TURBOSUPPORT_NOTSUPP :
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    CMCHKPK(cmPkTknU8, &param->choice, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuTurboSupport */

#define cmPkNhuMaxSimultaneousTransChsDL cmPkTknU32
#define cmPkNhuMaxSimultaneousCCTrCH_Count cmPkTknU32 
#define cmPkNhuMaxTportBlocksDL cmPkTknU32
#define cmPkNhuMaxNumberOfTFC_DL cmPkTknU32
#define cmPkNhuMaxNumberOfTF cmPkTknU32


/* 
* FUN : cmPkNhuDL_TransChCap
*  DESC: Function to Pack NhuDL_TransChCap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuDL_TransChCap
(
NhuDL_TransChCap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuDL_TransChCap(param, mBuf)
NhuDL_TransChCap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuDL_TransChCap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuMaxNumberOfTF, &param->maxNumberOfTF, mBuf); 
        CMCHKPK(cmPkNhuMaxNumberOfTFC_DL, &param->maxNumberOfTFC, mBuf); 
        CMCHKPK(cmPkNhuMaxTportBlocksDL, &param->maxReceivedTportBlocks, mBuf); 
        CMCHKPK(cmPkNhuMaxSimultaneousCCTrCH_Count, &param->maxSimultaneousCCTrCH_Count, mBuf); 
        CMCHKPK(cmPkNhuMaxSimultaneousTransChsDL, &param->maxSimultaneousTransChs, mBuf); 
        CMCHKPK(cmPkNhuTurboSupport, &param->turboDecodingSupport, mBuf); 
        CMCHKPK(cmPkNhuMaxNoBits, &param->maxConvCodeBitsReceived, mBuf); 
        CMCHKPK(cmPkNhuMaxNoBits, &param->maxNoBitsReceived, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuDL_TransChCap */

#define cmPkNhuMaxSimultaneousTransChsUL cmPkTknU32
#define cmPkNhuMaxTportBlocksUL cmPkTknU32
#define cmPkNhuMaxNumberOfTFC_UL cmPkTknU32


/* 
* FUN : cmPkNhuUL_TransChCapmodeSpecificInfotdd
*  DESC: Function to Pack NhuUL_TransChCapmodeSpecificInfotdd structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUL_TransChCapmodeSpecificInfotdd
(
NhuUL_TransChCapmodeSpecificInfotdd *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUL_TransChCapmodeSpecificInfotdd(param, mBuf)
NhuUL_TransChCapmodeSpecificInfotdd *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUL_TransChCapmodeSpecificInfotdd) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuMaxSimultaneousCCTrCH_Count, &param->maxSimultaneousCCTrCH_Count, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUL_TransChCapmodeSpecificInfotdd */



/* 
* FUN : cmPkNhuUL_TransChCapmodeSpecificInfo
*  DESC: Function to Pack NhuUL_TransChCapmodeSpecificInfo structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUL_TransChCapmodeSpecificInfo
(
NhuUL_TransChCapmodeSpecificInfo *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUL_TransChCapmodeSpecificInfo(param, mBuf)
NhuUL_TransChCapmodeSpecificInfo *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUL_TransChCapmodeSpecificInfo) 
    
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case MODESPECIFICINFO_TDD :
                CMCHKPK(cmPkNhuUL_TransChCapmodeSpecificInfotdd, &param->val.tdd, mBuf); 
                break;
            case MODESPECIFICINFO_FDD :
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    CMCHKPK(cmPkTknU8, &param->choice, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUL_TransChCapmodeSpecificInfo */



/* 
* FUN : cmPkNhuUL_TransChCap
*  DESC: Function to Pack NhuUL_TransChCap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUL_TransChCap
(
NhuUL_TransChCap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUL_TransChCap(param, mBuf)
NhuUL_TransChCap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUL_TransChCap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuMaxNumberOfTF, &param->maxNumberOfTF, mBuf); 
        CMCHKPK(cmPkNhuMaxNumberOfTFC_UL, &param->maxNumberOfTFC, mBuf); 
        CMCHKPK(cmPkNhuMaxTportBlocksUL, &param->maxTransmittedBlocks, mBuf); 
        CMCHKPK(cmPkNhuUL_TransChCapmodeSpecificInfo, &param->modeSpecificInfo, mBuf); 
        CMCHKPK(cmPkNhuMaxSimultaneousTransChsUL, &param->maxSimultaneousTransChs, mBuf); 
        CMCHKPK(cmPkNhuTurboSupport, &param->turboEncodingSupport, mBuf); 
        CMCHKPK(cmPkNhuMaxNoBits, &param->maxConvCodeBitsTransmitted, mBuf); 
        CMCHKPK(cmPkNhuMaxNoBits, &param->maxNoBitsTransmitted, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUL_TransChCap */



/* 
* FUN : cmPkNhuTportChannelCap
*  DESC: Function to Pack NhuTportChannelCap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuTportChannelCap
(
NhuTportChannelCap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuTportChannelCap(param, mBuf)
NhuTportChannelCap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuTportChannelCap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUL_TransChCap, &param->ul_TransChCap, mBuf); 
        CMCHKPK(cmPkNhuDL_TransChCap, &param->dl_TransChCap, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuTportChannelCap */

#define cmPkNhuUE_PowerClass cmPkTknU32 
#define cmPkNhuTxRxFrequencySeparation cmPkTknU32
#define cmPkNhuRadioFrequencyBandTDDLst cmPkTknU32
#define cmPkNhuChipRateCap cmPkTknU32


/* 
* FUN : cmPkNhuRF_CapfddRF_Cap
*  DESC: Function to Pack NhuRF_CapfddRF_Cap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuRF_CapfddRF_Cap
(
NhuRF_CapfddRF_Cap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuRF_CapfddRF_Cap(param, mBuf)
NhuRF_CapfddRF_Cap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuRF_CapfddRF_Cap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuTxRxFrequencySeparation, &param->txRxFrequencySeparation, mBuf); 
        CMCHKPK(cmPkNhuUE_PowerClass, &param->ue_PowerClass, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuRF_CapfddRF_Cap */



/* 
* FUN : cmPkNhuRF_CaptddRF_Cap
*  DESC: Function to Pack NhuRF_CaptddRF_Cap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuRF_CaptddRF_Cap
(
NhuRF_CaptddRF_Cap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuRF_CaptddRF_Cap(param, mBuf)
NhuRF_CaptddRF_Cap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuRF_CaptddRF_Cap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuChipRateCap, &param->chipRateCap, mBuf); 
        CMCHKPK(cmPkNhuRadioFrequencyBandTDDLst, &param->radioFrequencyTDDBandLst, mBuf); 
        CMCHKPK(cmPkNhuUE_PowerClass, &param->ue_PowerClass, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuRF_CaptddRF_Cap */



/* 
* FUN : cmPkNhuRF_Cap
*  DESC: Function to Pack NhuRF_Cap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuRF_Cap
(
NhuRF_Cap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuRF_Cap(param, mBuf)
NhuRF_Cap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuRF_Cap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuRF_CaptddRF_Cap, &param->tddRF_Cap, mBuf); 
        CMCHKPK(cmPkNhuRF_CapfddRF_Cap, &param->fddRF_Cap, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuRF_Cap */

#define cmPkNhuMaxNoPhysChBitsReceived cmPkTknU32
#define cmPkNhuMaxNoSCCPCH_RL cmPkTknU32
#define cmPkNhuSimultaneousSCCPCH_DPCH_ReceptionsupportedsimultaneousSCCPCH_DPCH_DPDCH_Reception cmPkTknU8


/* 
* FUN : cmPkNhuSimultaneousSCCPCH_DPCH_Receptionsupported
*  DESC: Function to Pack NhuSimultaneousSCCPCH_DPCH_Receptionsupported structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuSimultaneousSCCPCH_DPCH_Receptionsupported
(
NhuSimultaneousSCCPCH_DPCH_Receptionsupported *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuSimultaneousSCCPCH_DPCH_Receptionsupported(param, mBuf)
NhuSimultaneousSCCPCH_DPCH_Receptionsupported *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuSimultaneousSCCPCH_DPCH_Receptionsupported) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuSimultaneousSCCPCH_DPCH_ReceptionsupportedsimultaneousSCCPCH_DPCH_DPDCH_Reception, &param->simultaneousSCCPCH_DPCH_DPDCH_Reception, mBuf); 
        CMCHKPK(cmPkNhuMaxNoSCCPCH_RL, &param->maxNoSCCPCH_RL, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuSimultaneousSCCPCH_DPCH_Receptionsupported */



/* 
* FUN : cmPkNhuSimultaneousSCCPCH_DPCH_Reception
*  DESC: Function to Pack NhuSimultaneousSCCPCH_DPCH_Reception structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuSimultaneousSCCPCH_DPCH_Reception
(
NhuSimultaneousSCCPCH_DPCH_Reception *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuSimultaneousSCCPCH_DPCH_Reception(param, mBuf)
NhuSimultaneousSCCPCH_DPCH_Reception *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuSimultaneousSCCPCH_DPCH_Reception) 
    
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case SIMULTANEOUSSCCPCH_DPCH_RECEPTION_SUPPORTED :
                CMCHKPK(cmPkNhuSimultaneousSCCPCH_DPCH_Receptionsupported, &param->val.supported, mBuf); 
                break;
            case SIMULTANEOUSSCCPCH_DPCH_RECEPTION_NOTSUPP :
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    CMCHKPK(cmPkTknU8, &param->choice, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuSimultaneousSCCPCH_DPCH_Reception */

#define cmPkNhuDL_PhysChCapFDDmaxNoDPCH_PDSCH_Codes cmPkTknU32 
#define cmPkNhuDL_PhysChCapFDDsupportForSF_512 cmPkTknU8
#define cmPkNhuDL_PhysChCapFDDdummy cmPkTknU8


/* 
* FUN : cmPkNhuDL_PhysChCapFDD
*  DESC: Function to Pack NhuDL_PhysChCapFDD structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuDL_PhysChCapFDD
(
NhuDL_PhysChCapFDD *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuDL_PhysChCapFDD(param, mBuf)
NhuDL_PhysChCapFDD *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuDL_PhysChCapFDD) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuSimultaneousSCCPCH_DPCH_Reception, &param->dummy2, mBuf); 
        CMCHKPK(cmPkNhuDL_PhysChCapFDDdummy, &param->dummy, mBuf); 
        CMCHKPK(cmPkNhuDL_PhysChCapFDDsupportForSF_512, &param->supportForSF_512, mBuf); 
        CMCHKPK(cmPkNhuMaxNoPhysChBitsReceived, &param->maxNoPhysChBitsReceived, mBuf); 
        CMCHKPK(cmPkNhuDL_PhysChCapFDDmaxNoDPCH_PDSCH_Codes, &param->maxNoDPCH_PDSCH_Codes, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuDL_PhysChCapFDD */

#define cmPkNhuMaxNoDPDCH_BitsTransmitted cmPkTknU32
#define cmPkNhuUL_PhysChCapFDDdummy cmPkTknU8


/* 
* FUN : cmPkNhuUL_PhysChCapFDD
*  DESC: Function to Pack NhuUL_PhysChCapFDD structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUL_PhysChCapFDD
(
NhuUL_PhysChCapFDD *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUL_PhysChCapFDD(param, mBuf)
NhuUL_PhysChCapFDD *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUL_PhysChCapFDD) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUL_PhysChCapFDDdummy, &param->dummy, mBuf); 
        CMCHKPK(cmPkNhuMaxNoDPDCH_BitsTransmitted, &param->maxNoDPDCH_BitsTransmitted, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUL_PhysChCapFDD */

#define cmPkNhuMaxTS_PerFrame cmPkTknU32 
#define cmPkNhuMaxPhysChPerFrame cmPkTknU32 
#define cmPkNhuMinimumSF_DL cmPkTknU32
#define cmPkNhuMaxPhysChPerTS cmPkTknU32 
#define cmPkNhuDL_PhysChCapTDDsupportOfPDSCH cmPkTknU8


/* 
* FUN : cmPkNhuDL_PhysChCapTDD
*  DESC: Function to Pack NhuDL_PhysChCapTDD structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuDL_PhysChCapTDD
(
NhuDL_PhysChCapTDD *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuDL_PhysChCapTDD(param, mBuf)
NhuDL_PhysChCapTDD *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuDL_PhysChCapTDD) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuMaxPhysChPerTS, &param->maxPhysChPerTS, mBuf); 
        CMCHKPK(cmPkNhuDL_PhysChCapTDDsupportOfPDSCH, &param->supportOfPDSCH, mBuf); 
        CMCHKPK(cmPkNhuMinimumSF_DL, &param->minimumSF, mBuf); 
        CMCHKPK(cmPkNhuMaxPhysChPerFrame, &param->maxPhysChPerFrame, mBuf); 
        CMCHKPK(cmPkNhuMaxTS_PerFrame, &param->maxTS_PerFrame, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuDL_PhysChCapTDD */

#define cmPkNhuMaxPhysChPerTimeslot cmPkTknU32
#define cmPkNhuMinimumSF_UL cmPkTknU32
#define cmPkNhuUL_PhysChCapTDDsupportOfPUSCH cmPkTknU8


/* 
* FUN : cmPkNhuUL_PhysChCapTDD
*  DESC: Function to Pack NhuUL_PhysChCapTDD structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUL_PhysChCapTDD
(
NhuUL_PhysChCapTDD *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUL_PhysChCapTDD(param, mBuf)
NhuUL_PhysChCapTDD *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUL_PhysChCapTDD) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUL_PhysChCapTDDsupportOfPUSCH, &param->supportOfPUSCH, mBuf); 
        CMCHKPK(cmPkNhuMinimumSF_UL, &param->minimumSF, mBuf); 
        CMCHKPK(cmPkNhuMaxPhysChPerTimeslot, &param->maxPhysChPerTimeslot, mBuf); 
        CMCHKPK(cmPkNhuMaxTS_PerFrame, &param->maxTS_PerFrame, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUL_PhysChCapTDD */



/* 
* FUN : cmPkNhuPhysicalChannelCapfddPhysChCap
*  DESC: Function to Pack NhuPhysicalChannelCapfddPhysChCap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPhysicalChannelCapfddPhysChCap
(
NhuPhysicalChannelCapfddPhysChCap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPhysicalChannelCapfddPhysChCap(param, mBuf)
NhuPhysicalChannelCapfddPhysChCap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPhysicalChannelCapfddPhysChCap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUL_PhysChCapFDD, &param->uplinkPhysChCap, mBuf); 
        CMCHKPK(cmPkNhuDL_PhysChCapFDD, &param->downlinkPhysChCap, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPhysicalChannelCapfddPhysChCap */



/* 
* FUN : cmPkNhuPhysicalChannelCaptddPhysChCap
*  DESC: Function to Pack NhuPhysicalChannelCaptddPhysChCap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPhysicalChannelCaptddPhysChCap
(
NhuPhysicalChannelCaptddPhysChCap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPhysicalChannelCaptddPhysChCap(param, mBuf)
NhuPhysicalChannelCaptddPhysChCap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPhysicalChannelCaptddPhysChCap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUL_PhysChCapTDD, &param->uplinkPhysChCap, mBuf); 
        CMCHKPK(cmPkNhuDL_PhysChCapTDD, &param->downlinkPhysChCap, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPhysicalChannelCaptddPhysChCap */



/* 
* FUN : cmPkNhuPhysicalChannelCap
*  DESC: Function to Pack NhuPhysicalChannelCap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPhysicalChannelCap
(
NhuPhysicalChannelCap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPhysicalChannelCap(param, mBuf)
NhuPhysicalChannelCap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPhysicalChannelCap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuPhysicalChannelCaptddPhysChCap, &param->tddPhysChCap, mBuf); 
        CMCHKPK(cmPkNhuPhysicalChannelCapfddPhysChCap, &param->fddPhysChCap, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPhysicalChannelCap */

#define cmPkNhuMultiRAT_CapsupportOfGSM cmPkTknU8
#define cmPkNhuMultiRAT_CapsupportOfMulticarrier cmPkTknU8


/* 
* FUN : cmPkNhuMultiRAT_Cap
*  DESC: Function to Pack NhuMultiRAT_Cap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuMultiRAT_Cap
(
NhuMultiRAT_Cap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuMultiRAT_Cap(param, mBuf)
NhuMultiRAT_Cap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuMultiRAT_Cap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuMultiRAT_CapsupportOfMulticarrier, &param->supportOfMulticarrier, mBuf); 
        CMCHKPK(cmPkNhuMultiRAT_CapsupportOfGSM, &param->supportOfGSM, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuMultiRAT_Cap */

#define cmPkNhuMultiModeCap cmPkTknU32


/* 
* FUN : cmPkNhuUE_MultiModeRAT_Cap
*  DESC: Function to Pack NhuUE_MultiModeRAT_Cap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_MultiModeRAT_Cap
(
NhuUE_MultiModeRAT_Cap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_MultiModeRAT_Cap(param, mBuf)
NhuUE_MultiModeRAT_Cap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_MultiModeRAT_Cap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuMultiModeCap, &param->multiModeCap, mBuf); 
        CMCHKPK(cmPkNhuMultiRAT_Cap, &param->multiRAT_CapLst, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_MultiModeRAT_Cap */

#define cmPkNhuSecCapcipheringAlgorithmCap cmPkTknStrBSXL 
#define cmPkNhuSecCapintegrityProtectionAlgorithmCap cmPkTknStrBSXL 


/* 
* FUN : cmPkNhuSecCap
*  DESC: Function to Pack NhuSecCap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuSecCap
(
NhuSecCap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuSecCap(param, mBuf)
NhuSecCap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuSecCap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuSecCapintegrityProtectionAlgorithmCap, &param->integrityProtectionAlgorithmCap, mBuf); 
        CMCHKPK(cmPkNhuSecCapcipheringAlgorithmCap, &param->cipheringAlgorithmCap, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuSecCap */

#define cmPkNhuNwAssistedGPS_Supp cmPkTknU32
#define cmPkNhuUE_Positioning_CapstandaloneLocMethodsSupp cmPkTknU8
#define cmPkNhuUE_Positioning_Capue_BasedOTDOA_Supp cmPkTknU8
#define cmPkNhuUE_Positioning_CapsupportForUE_GPS_TimingOfCellFrames cmPkTknU8
#define cmPkNhuUE_Positioning_CapsupportForIPDL cmPkTknU8


/* 
* FUN : cmPkNhuUE_Positioning_Cap
*  DESC: Function to Pack NhuUE_Positioning_Cap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_Positioning_Cap
(
NhuUE_Positioning_Cap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_Positioning_Cap(param, mBuf)
NhuUE_Positioning_Cap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_Positioning_Cap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUE_Positioning_CapsupportForIPDL, &param->supportForIPDL, mBuf); 
        CMCHKPK(cmPkNhuUE_Positioning_CapsupportForUE_GPS_TimingOfCellFrames, &param->supportForUE_GPS_TimingOfCellFrames, mBuf); 
        CMCHKPK(cmPkNhuNwAssistedGPS_Supp, &param->networkAssistedGPS_Supp, mBuf); 
        CMCHKPK(cmPkNhuUE_Positioning_Capue_BasedOTDOA_Supp, &param->ue_BasedOTDOA_Supp, mBuf); 
        CMCHKPK(cmPkNhuUE_Positioning_CapstandaloneLocMethodsSupp, &param->standaloneLocMethodsSupp, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_Positioning_Cap */

#define cmPkNhuGSM_Measurementsgsm900 cmPkTknU8
#define cmPkNhuGSM_Measurementsdcs1800 cmPkTknU8
#define cmPkNhuGSM_Measurementsgsm1900 cmPkTknU8


/* 
* FUN : cmPkNhuGSM_Measurements
*  DESC: Function to Pack NhuGSM_Measurements structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuGSM_Measurements
(
NhuGSM_Measurements *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuGSM_Measurements(param, mBuf)
NhuGSM_Measurements *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuGSM_Measurements) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuGSM_Measurementsgsm1900, &param->gsm1900, mBuf); 
        CMCHKPK(cmPkNhuGSM_Measurementsdcs1800, &param->dcs1800, mBuf); 
        CMCHKPK(cmPkNhuGSM_Measurementsgsm900, &param->gsm900, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuGSM_Measurements */

#define cmPkNhuCompressedModeMeasCapfdd_Measurements cmPkTknU8
#define cmPkNhuCompressedModeMeasCaptdd_Measurements cmPkTknU8
#define cmPkNhuCompressedModeMeasCapmultiCarrierMeasurements cmPkTknU8


/* 
* FUN : cmPkNhuCompressedModeMeasCap
*  DESC: Function to Pack NhuCompressedModeMeasCap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuCompressedModeMeasCap
(
NhuCompressedModeMeasCap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuCompressedModeMeasCap(param, mBuf)
NhuCompressedModeMeasCap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuCompressedModeMeasCap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuCompressedModeMeasCapmultiCarrierMeasurements, &param->multiCarrierMeasurements, mBuf); 
        CMCHKPK(cmPkNhuGSM_Measurements, &param->gsm_Measurements, mBuf); 
        CMCHKPK(cmPkNhuCompressedModeMeasCaptdd_Measurements, &param->tdd_Measurements, mBuf); 
        CMCHKPK(cmPkNhuCompressedModeMeasCapfdd_Measurements, &param->fdd_Measurements, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuCompressedModeMeasCap */



/* 
* FUN : cmPkNhuMeasurementCap
*  DESC: Function to Pack NhuMeasurementCap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuMeasurementCap
(
NhuMeasurementCap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuMeasurementCap(param, mBuf)
NhuMeasurementCap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuMeasurementCap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuCompressedModeMeasCap, &param->uplinkCompressedMode, mBuf); 
        CMCHKPK(cmPkNhuCompressedModeMeasCap, &param->downlinkCompressedMode, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuMeasurementCap */



/* 
* FUN : cmPkNhuUE_RadioAccessCap
*  DESC: Function to Pack NhuUE_RadioAccessCap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCap
(
NhuUE_RadioAccessCap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCap(param, mBuf)
NhuUE_RadioAccessCap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_RadioAccessCap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuMeasurementCap, &param->measurementCap, mBuf); 
        CMCHKPK(cmPkNhuUE_Positioning_Cap, &param->ue_positioning_Cap, mBuf); 
        CMCHKPK(cmPkNhuSecCap, &param->securityCap, mBuf); 
        CMCHKPK(cmPkNhuUE_MultiModeRAT_Cap, &param->ue_MultiModeRAT_Cap, mBuf); 
        CMCHKPK(cmPkNhuPhysicalChannelCap, &param->physicalChannelCap, mBuf); 
        CMCHKPK(cmPkNhuRF_Cap, &param->rf_Cap, mBuf); 
        CMCHKPK(cmPkNhuTportChannelCap, &param->transportChannelCap, mBuf); 
        CMCHKPK(cmPkNhuRLC_Cap, &param->rlc_Cap, mBuf); 
        CMCHKPK(cmPkNhuPDCP_Cap, &param->pdcp_Cap, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCap */

#define cmPkNhuRadioFrequencyBandFDD cmPkTknU32
#define cmPkNhuUE_PowerClassExt cmPkTknU32
#define cmPkNhuCompressedModeMeasCapabFDDdl_MeasurementsFDD cmPkTknU8
#define cmPkNhuCompressedModeMeasCapabFDDul_MeasurementsFDD cmPkTknU8


/* 
* FUN : cmPkNhuCompressedModeMeasCapabFDD
*  DESC: Function to Pack NhuCompressedModeMeasCapabFDD structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabFDD
(
NhuCompressedModeMeasCapabFDD *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabFDD(param, mBuf)
NhuCompressedModeMeasCapabFDD *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuCompressedModeMeasCapabFDD) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuCompressedModeMeasCapabFDDul_MeasurementsFDD, &param->ul_MeasurementsFDD, mBuf); 
        CMCHKPK(cmPkNhuCompressedModeMeasCapabFDDdl_MeasurementsFDD, &param->dl_MeasurementsFDD, mBuf); 
        CMCHKPK(cmPkNhuRadioFrequencyBandFDD, &param->radioFrequencyBandFDD, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuCompressedModeMeasCapabFDD */



/* 
* FUN : cmPkNhuCompressedModeMeasCapabFDDLst
*  DESC: Function to Pack NhuCompressedModeMeasCapabFDDLst structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabFDDLst
(
NhuCompressedModeMeasCapabFDDLst *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabFDDLst(param, mBuf)
NhuCompressedModeMeasCapabFDDLst *param;
Buffer *mBuf;
#endif
{ 
    Cntr i;
    TRC3(cmPkNhuCompressedModeMeasCapabFDDLst) 
    
    if( param->noComp.pres != NOTPRSNT ) 
    { 
        for(i = param->noComp.val-1; i >= 0; i--) 
        { 
            CMCHKPK(cmPkNhuCompressedModeMeasCapabFDD, (&param->member[i]), mBuf);
        } 
    } 
    CMCHKPK(cmPkTknU16, &param->noComp, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuCompressedModeMeasCapabFDDLst */

#define cmPkNhuRadioFrequencyBandTDD cmPkTknU32
#define cmPkNhuCompressedModeMeasCapabTDDdl_MeasurementsTDD cmPkTknU8
#define cmPkNhuCompressedModeMeasCapabTDDul_MeasurementsTDD cmPkTknU8


/* 
* FUN : cmPkNhuCompressedModeMeasCapabTDD
*  DESC: Function to Pack NhuCompressedModeMeasCapabTDD structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabTDD
(
NhuCompressedModeMeasCapabTDD *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabTDD(param, mBuf)
NhuCompressedModeMeasCapabTDD *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuCompressedModeMeasCapabTDD) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuCompressedModeMeasCapabTDDul_MeasurementsTDD, &param->ul_MeasurementsTDD, mBuf); 
        CMCHKPK(cmPkNhuCompressedModeMeasCapabTDDdl_MeasurementsTDD, &param->dl_MeasurementsTDD, mBuf); 
        CMCHKPK(cmPkNhuRadioFrequencyBandTDD, &param->radioFrequencyBandTDD, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuCompressedModeMeasCapabTDD */



/* 
* FUN : cmPkNhuCompressedModeMeasCapabTDDLst
*  DESC: Function to Pack NhuCompressedModeMeasCapabTDDLst structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabTDDLst
(
NhuCompressedModeMeasCapabTDDLst *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabTDDLst(param, mBuf)
NhuCompressedModeMeasCapabTDDLst *param;
Buffer *mBuf;
#endif
{ 
    Cntr i;
    TRC3(cmPkNhuCompressedModeMeasCapabTDDLst) 
    
    if( param->noComp.pres != NOTPRSNT ) 
    { 
        for(i = param->noComp.val-1; i >= 0; i--) 
        { 
            CMCHKPK(cmPkNhuCompressedModeMeasCapabTDD, (&param->member[i]), mBuf);
        } 
    } 
    CMCHKPK(cmPkTknU16, &param->noComp, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuCompressedModeMeasCapabTDDLst */

#define cmPkNhuRadioFrequencyBandGSM cmPkTknU32
#define cmPkNhuCompressedModeMeasCapabGSMdl_MeasurementsGSM cmPkTknU8
#define cmPkNhuCompressedModeMeasCapabGSMul_MeasurementsGSM cmPkTknU8


/* 
* FUN : cmPkNhuCompressedModeMeasCapabGSM
*  DESC: Function to Pack NhuCompressedModeMeasCapabGSM structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabGSM
(
NhuCompressedModeMeasCapabGSM *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabGSM(param, mBuf)
NhuCompressedModeMeasCapabGSM *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuCompressedModeMeasCapabGSM) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuCompressedModeMeasCapabGSMul_MeasurementsGSM, &param->ul_MeasurementsGSM, mBuf); 
        CMCHKPK(cmPkNhuCompressedModeMeasCapabGSMdl_MeasurementsGSM, &param->dl_MeasurementsGSM, mBuf); 
        CMCHKPK(cmPkNhuRadioFrequencyBandGSM, &param->radioFrequencyBandGSM, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuCompressedModeMeasCapabGSM */



/* 
* FUN : cmPkNhuCompressedModeMeasCapabGSMLst
*  DESC: Function to Pack NhuCompressedModeMeasCapabGSMLst structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabGSMLst
(
NhuCompressedModeMeasCapabGSMLst *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabGSMLst(param, mBuf)
NhuCompressedModeMeasCapabGSMLst *param;
Buffer *mBuf;
#endif
{ 
    Cntr i;
    TRC3(cmPkNhuCompressedModeMeasCapabGSMLst) 
    
    if( param->noComp.pres != NOTPRSNT ) 
    { 
        for(i = param->noComp.val-1; i >= 0; i--) 
        { 
            CMCHKPK(cmPkNhuCompressedModeMeasCapabGSM, (&param->member[i]), mBuf);
        } 
    } 
    CMCHKPK(cmPkTknU16, &param->noComp, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuCompressedModeMeasCapabGSMLst */

#define cmPkNhuCompressedModeMeasCapabMCdl_MeasurementsMC cmPkTknU8
#define cmPkNhuCompressedModeMeasCapabMCul_MeasurementsMC cmPkTknU8


/* 
* FUN : cmPkNhuCompressedModeMeasCapabMC
*  DESC: Function to Pack NhuCompressedModeMeasCapabMC structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabMC
(
NhuCompressedModeMeasCapabMC *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabMC(param, mBuf)
NhuCompressedModeMeasCapabMC *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuCompressedModeMeasCapabMC) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuCompressedModeMeasCapabMCul_MeasurementsMC, &param->ul_MeasurementsMC, mBuf); 
        CMCHKPK(cmPkNhuCompressedModeMeasCapabMCdl_MeasurementsMC, &param->dl_MeasurementsMC, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuCompressedModeMeasCapabMC */



/* 
* FUN : cmPkNhuMeasurementCapExt
*  DESC: Function to Pack NhuMeasurementCapExt structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuMeasurementCapExt
(
NhuMeasurementCapExt *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuMeasurementCapExt(param, mBuf)
NhuMeasurementCapExt *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuMeasurementCapExt) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuCompressedModeMeasCapabMC, &param->compressedModeMeasCapabMC, mBuf); 
        CMCHKPK(cmPkNhuCompressedModeMeasCapabGSMLst, &param->compressedModeMeasCapabGSMLst, mBuf); 
        CMCHKPK(cmPkNhuCompressedModeMeasCapabTDDLst, &param->compressedModeMeasCapabTDDLst, mBuf); 
        CMCHKPK(cmPkNhuCompressedModeMeasCapabFDDLst, &param->compressedModeMeasCapabFDDLst, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuMeasurementCapExt */



/* 
* FUN : cmPkNhuUE_RadioAccessCapabBandFDDfddRF_Cap
*  DESC: Function to Pack NhuUE_RadioAccessCapabBandFDDfddRF_Cap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCapabBandFDDfddRF_Cap
(
NhuUE_RadioAccessCapabBandFDDfddRF_Cap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCapabBandFDDfddRF_Cap(param, mBuf)
NhuUE_RadioAccessCapabBandFDDfddRF_Cap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_RadioAccessCapabBandFDDfddRF_Cap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuTxRxFrequencySeparation, &param->txRxFrequencySeparation, mBuf); 
        CMCHKPK(cmPkNhuUE_PowerClassExt, &param->ue_PowerClass, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCapabBandFDDfddRF_Cap */



/* 
* FUN : cmPkNhuUE_RadioAccessCapabBandFDD
*  DESC: Function to Pack NhuUE_RadioAccessCapabBandFDD structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCapabBandFDD
(
NhuUE_RadioAccessCapabBandFDD *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCapabBandFDD(param, mBuf)
NhuUE_RadioAccessCapabBandFDD *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_RadioAccessCapabBandFDD) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuMeasurementCapExt, &param->measurementCap, mBuf); 
        CMCHKPK(cmPkNhuUE_RadioAccessCapabBandFDDfddRF_Cap, &param->fddRF_Cap, mBuf); 
        CMCHKPK(cmPkNhuRadioFrequencyBandFDD, &param->radioFrequencyBandFDD, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCapabBandFDD */



/* 
* FUN : cmPkNhuUE_RadioAccessCapabBandFDDLst
*  DESC: Function to Pack NhuUE_RadioAccessCapabBandFDDLst structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCapabBandFDDLst
(
NhuUE_RadioAccessCapabBandFDDLst *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCapabBandFDDLst(param, mBuf)
NhuUE_RadioAccessCapabBandFDDLst *param;
Buffer *mBuf;
#endif
{ 
    Cntr i;
    TRC3(cmPkNhuUE_RadioAccessCapabBandFDDLst) 
    
    if( param->noComp.pres != NOTPRSNT ) 
    { 
        for(i = param->noComp.val-1; i >= 0; i--) 
        { 
            CMCHKPK(cmPkNhuUE_RadioAccessCapabBandFDD, (&param->member[i]), mBuf);
        } 
    } 
    CMCHKPK(cmPkTknU16, &param->noComp, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCapabBandFDDLst */



/* 
* FUN : cmPkNhuUE_RadioAccessCap_v370ext
*  DESC: Function to Pack NhuUE_RadioAccessCap_v370ext structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCap_v370ext
(
NhuUE_RadioAccessCap_v370ext *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCap_v370ext(param, mBuf)
NhuUE_RadioAccessCap_v370ext *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_RadioAccessCap_v370ext) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUE_RadioAccessCapabBandFDDLst, &param->ue_RadioAccessCapabBandFDDLst, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCap_v370ext */



/* 
* FUN : cmPkNhuUTRANUE_RadioAccessCapInfo
*  DESC: Function to Pack NhuUTRANUE_RadioAccessCapInfo structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUTRANUE_RadioAccessCapInfo
(
NhuUTRANUE_RadioAccessCapInfo *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUTRANUE_RadioAccessCapInfo(param, mBuf)
NhuUTRANUE_RadioAccessCapInfo *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUTRANUE_RadioAccessCapInfo) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUE_RadioAccessCap_v370ext, &param->ue_RadioAccessCap_v370ext, mBuf); 
        CMCHKPK(cmPkNhuUE_RadioAccessCap, &param->ue_RadioAccessCap, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUTRANUE_RadioAccessCapInfo */

#define cmPkNhuPredefinedConfigValueTag cmPkTknU32 


/* 
* FUN : cmPkNhuPredefinedConfigStatusInfoother
*  DESC: Function to Pack NhuPredefinedConfigStatusInfoother structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPredefinedConfigStatusInfoother
(
NhuPredefinedConfigStatusInfoother *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPredefinedConfigStatusInfoother(param, mBuf)
NhuPredefinedConfigStatusInfoother *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPredefinedConfigStatusInfoother) 
    
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case OTHER_STOREDWITHDIFFERENTVALUETAG :
                CMCHKPK(cmPkNhuPredefinedConfigValueTag, &param->val.storedWithDifferentValueTag, mBuf); 
                break;
            case OTHER_NOTSTORED :
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    CMCHKPK(cmPkTknU8, &param->choice, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPredefinedConfigStatusInfoother */



/* 
* FUN : cmPkNhuPredefinedConfigStatusInfo
*  DESC: Function to Pack NhuPredefinedConfigStatusInfo structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPredefinedConfigStatusInfo
(
NhuPredefinedConfigStatusInfo *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPredefinedConfigStatusInfo(param, mBuf)
NhuPredefinedConfigStatusInfo *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPredefinedConfigStatusInfo) 
    
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case PREDEFINEDCONFIGSTATUSINFO_OTHER :
                CMCHKPK(cmPkNhuPredefinedConfigStatusInfoother, &param->val.other, mBuf); 
                break;
            case PREDEFINEDCONFIGSTATUSINFO_STOREDWITHVALUETAGSAMEASPREVIUS :
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    CMCHKPK(cmPkTknU8, &param->choice, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPredefinedConfigStatusInfo */



/* 
* FUN : cmPkNhuPredefinedConfigStatusLst
*  DESC: Function to Pack NhuPredefinedConfigStatusLst structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPredefinedConfigStatusLst
(
NhuPredefinedConfigStatusLst *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPredefinedConfigStatusLst(param, mBuf)
NhuPredefinedConfigStatusLst *param;
Buffer *mBuf;
#endif
{ 
    Cntr i;
    TRC3(cmPkNhuPredefinedConfigStatusLst) 
    
    if( param->noComp.pres != NOTPRSNT ) 
    { 
        for(i = param->noComp.val-1; i >= 0; i--) 
        { 
            CMCHKPK(cmPkNhuPredefinedConfigStatusInfo, (&param->member[i]), mBuf);
        } 
    } 
    CMCHKPK(cmPkTknU16, &param->noComp, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPredefinedConfigStatusLst */

#define cmPkNhuSTART_Value cmPkTknStrBSXL 


/* 
* FUN : cmPkNhuUE_SecInfo
*  DESC: Function to Pack NhuUE_SecInfo structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_SecInfo
(
NhuUE_SecInfo *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_SecInfo(param, mBuf)
NhuUE_SecInfo *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_SecInfo) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuSTART_Value, &param->start_CS, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_SecInfo */

#define cmPkNhuUE_PositioningCapExt_v380rx_tx_TimeDifferenceTyp2Capable cmPkTknU8


/* 
* FUN : cmPkNhuUE_PositioningCapExt_v380
*  DESC: Function to Pack NhuUE_PositioningCapExt_v380 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_PositioningCapExt_v380
(
NhuUE_PositioningCapExt_v380 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_PositioningCapExt_v380(param, mBuf)
NhuUE_PositioningCapExt_v380 *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_PositioningCapExt_v380) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUE_PositioningCapExt_v380rx_tx_TimeDifferenceTyp2Capable, &param->rx_tx_TimeDifferenceTyp2Capable, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_PositioningCapExt_v380 */



/* 
* FUN : cmPkNhuUE_RadioAccessCap_v380ext
*  DESC: Function to Pack NhuUE_RadioAccessCap_v380ext structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCap_v380ext
(
NhuUE_RadioAccessCap_v380ext *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCap_v380ext(param, mBuf)
NhuUE_RadioAccessCap_v380ext *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_RadioAccessCap_v380ext) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUE_PositioningCapExt_v380, &param->ue_PositioningCapExt_v380, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCap_v380ext */

#define cmPkNhuSupportOfDedicatedPilotsForChEstimation cmPkTknU32


/* 
* FUN : cmPkNhuDL_PhysChCapFDD_v380ext
*  DESC: Function to Pack NhuDL_PhysChCapFDD_v380ext structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuDL_PhysChCapFDD_v380ext
(
NhuDL_PhysChCapFDD_v380ext *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuDL_PhysChCapFDD_v380ext(param, mBuf)
NhuDL_PhysChCapFDD_v380ext *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuDL_PhysChCapFDD_v380ext) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuSupportOfDedicatedPilotsForChEstimation, &param->dummy, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuDL_PhysChCapFDD_v380ext */



/* 
* FUN : cmPkNhuInterRATHovrInfo_v390ext_IEs
*  DESC: Function to Pack NhuInterRATHovrInfo_v390ext_IEs structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v390ext_IEs
(
NhuInterRATHovrInfo_v390ext_IEs *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v390ext_IEs(param, mBuf)
NhuInterRATHovrInfo_v390ext_IEs *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuInterRATHovrInfo_v390ext_IEs) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuDL_PhysChCapFDD_v380ext, &param->dl_PhysChCapFDD_v380ext, mBuf); 
        CMCHKPK(cmPkNhuUE_RadioAccessCap_v380ext, &param->ue_RadioAccessCap_v380ext, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuInterRATHovrInfo_v390ext_IEs */

#define cmPkNhuUE_PositioningCapExt_v3a0validity_CellPCH_UraPCH cmPkTknU32


/* 
* FUN : cmPkNhuUE_PositioningCapExt_v3a0
*  DESC: Function to Pack NhuUE_PositioningCapExt_v3a0 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_PositioningCapExt_v3a0
(
NhuUE_PositioningCapExt_v3a0 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_PositioningCapExt_v3a0(param, mBuf)
NhuUE_PositioningCapExt_v3a0 *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_PositioningCapExt_v3a0) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUE_PositioningCapExt_v3a0validity_CellPCH_UraPCH, &param->validity_CellPCH_UraPCH, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_PositioningCapExt_v3a0 */



/* 
* FUN : cmPkNhuUE_RadioAccessCap_v3a0ext
*  DESC: Function to Pack NhuUE_RadioAccessCap_v3a0ext structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCap_v3a0ext
(
NhuUE_RadioAccessCap_v3a0ext *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCap_v3a0ext(param, mBuf)
NhuUE_RadioAccessCap_v3a0ext *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_RadioAccessCap_v3a0ext) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUE_PositioningCapExt_v3a0, &param->ue_PositioningCapExt_v3a0, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCap_v3a0ext */



/* 
* FUN : cmPkNhuInterRATHovrInfo_v3a0ext_IEs
*  DESC: Function to Pack NhuInterRATHovrInfo_v3a0ext_IEs structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v3a0ext_IEs
(
NhuInterRATHovrInfo_v3a0ext_IEs *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v3a0ext_IEs(param, mBuf)
NhuInterRATHovrInfo_v3a0ext_IEs *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuInterRATHovrInfo_v3a0ext_IEs) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUE_RadioAccessCap_v3a0ext, &param->ue_RadioAccessCap_v3a0ext, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuInterRATHovrInfo_v3a0ext_IEs */

#define cmPkNhuUESpecificBehaviourInfo1interRAT cmPkTknStrBSXL 


/* 
* FUN : cmPkNhuInterRATHovrInfo_v3d0ext_IEs
*  DESC: Function to Pack NhuInterRATHovrInfo_v3d0ext_IEs structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v3d0ext_IEs
(
NhuInterRATHovrInfo_v3d0ext_IEs *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v3d0ext_IEs(param, mBuf)
NhuInterRATHovrInfo_v3d0ext_IEs *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuInterRATHovrInfo_v3d0ext_IEs) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUESpecificBehaviourInfo1interRAT, &param->uESpecificBehaviourInfo1interRAT, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuInterRATHovrInfo_v3d0ext_IEs */

#define cmPkNhuUE_PositioningCapExt_v3g0sfn_sfnTyp2Cap cmPkTknU32


/* 
* FUN : cmPkNhuUE_PositioningCapExt_v3g0
*  DESC: Function to Pack NhuUE_PositioningCapExt_v3g0 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_PositioningCapExt_v3g0
(
NhuUE_PositioningCapExt_v3g0 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_PositioningCapExt_v3g0(param, mBuf)
NhuUE_PositioningCapExt_v3g0 *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_PositioningCapExt_v3g0) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUE_PositioningCapExt_v3g0sfn_sfnTyp2Cap, &param->sfn_sfnTyp2Cap, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_PositioningCapExt_v3g0 */



/* 
* FUN : cmPkNhuUE_RadioAccessCap_v3g0ext
*  DESC: Function to Pack NhuUE_RadioAccessCap_v3g0ext structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCap_v3g0ext
(
NhuUE_RadioAccessCap_v3g0ext *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCap_v3g0ext(param, mBuf)
NhuUE_RadioAccessCap_v3g0ext *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_RadioAccessCap_v3g0ext) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUE_PositioningCapExt_v3g0, &param->ue_PositioningCapExt_v3g0, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCap_v3g0ext */



/* 
* FUN : cmPkNhuInterRATHovrInfo_v3g0ext_IEs
*  DESC: Function to Pack NhuInterRATHovrInfo_v3g0ext_IEs structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v3g0ext_IEs
(
NhuInterRATHovrInfo_v3g0ext_IEs *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v3g0ext_IEs(param, mBuf)
NhuInterRATHovrInfo_v3g0ext_IEs *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuInterRATHovrInfo_v3g0ext_IEs) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUE_RadioAccessCap_v3g0ext, &param->ue_RadioAccessCap_v3g0ext, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuInterRATHovrInfo_v3g0ext_IEs */

#define cmPkNhuAccessStratumRlsIndicator cmPkTknU32


/* 
* FUN : cmPkNhuInterRATHovrInfo_v4b0ext_IEs
*  DESC: Function to Pack NhuInterRATHovrInfo_v4b0ext_IEs structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v4b0ext_IEs
(
NhuInterRATHovrInfo_v4b0ext_IEs *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v4b0ext_IEs(param, mBuf)
NhuInterRATHovrInfo_v4b0ext_IEs *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuInterRATHovrInfo_v4b0ext_IEs) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuAccessStratumRlsIndicator, &param->accessStratumRlsIndicator, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuInterRATHovrInfo_v4b0ext_IEs */



/* 
* FUN : cmPkNhuInterRATHovrInfo_v4d0ext_IEs
*  DESC: Function to Pack NhuInterRATHovrInfo_v4d0ext_IEs structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v4d0ext_IEs
(
NhuInterRATHovrInfo_v4d0ext_IEs *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v4d0ext_IEs(param, mBuf)
NhuInterRATHovrInfo_v4d0ext_IEs *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuInterRATHovrInfo_v4d0ext_IEs) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuRadioFrequencyBandTDDLst, &param->tdd128_RF_Cap, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuInterRATHovrInfo_v4d0ext_IEs */



/* 
* FUN : cmPkNhuPredefinedConfigValueTagLst
*  DESC: Function to Pack NhuPredefinedConfigValueTagLst structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPredefinedConfigValueTagLst
(
NhuPredefinedConfigValueTagLst *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPredefinedConfigValueTagLst(param, mBuf)
NhuPredefinedConfigValueTagLst *param;
Buffer *mBuf;
#endif
{ 
    Cntr i;
    TRC3(cmPkNhuPredefinedConfigValueTagLst) 
    
    if( param->noComp.pres != NOTPRSNT ) 
    { 
        for(i = param->noComp.val-1; i >= 0; i--) 
        { 
            CMCHKPK(cmPkNhuPredefinedConfigValueTag, (&param->member[i]), mBuf);
        } 
    } 
    CMCHKPK(cmPkTknU16, &param->noComp, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPredefinedConfigValueTagLst */

#define cmPkNhuPredefinedConfigSetWithDifferentValueTagstartPosition cmPkTknU32 


/* 
* FUN : cmPkNhuPredefinedConfigSetWithDifferentValueTag
*  DESC: Function to Pack NhuPredefinedConfigSetWithDifferentValueTag structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPredefinedConfigSetWithDifferentValueTag
(
NhuPredefinedConfigSetWithDifferentValueTag *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPredefinedConfigSetWithDifferentValueTag(param, mBuf)
NhuPredefinedConfigSetWithDifferentValueTag *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPredefinedConfigSetWithDifferentValueTag) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuPredefinedConfigValueTagLst, &param->valueTagLst, mBuf); 
        CMCHKPK(cmPkNhuPredefinedConfigSetWithDifferentValueTagstartPosition, &param->startPosition, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPredefinedConfigSetWithDifferentValueTag */



/* 
* FUN : cmPkNhuPredefinedConfigSetsWithDifferentValueTag
*  DESC: Function to Pack NhuPredefinedConfigSetsWithDifferentValueTag structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPredefinedConfigSetsWithDifferentValueTag
(
NhuPredefinedConfigSetsWithDifferentValueTag *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPredefinedConfigSetsWithDifferentValueTag(param, mBuf)
NhuPredefinedConfigSetsWithDifferentValueTag *param;
Buffer *mBuf;
#endif
{ 
    Cntr i;
    TRC3(cmPkNhuPredefinedConfigSetsWithDifferentValueTag) 
    
    if( param->noComp.pres != NOTPRSNT ) 
    { 
        for(i = param->noComp.val-1; i >= 0; i--) 
        { 
            CMCHKPK(cmPkNhuPredefinedConfigSetWithDifferentValueTag, (&param->member[i]), mBuf);
        } 
    } 
    CMCHKPK(cmPkTknU16, &param->noComp, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPredefinedConfigSetsWithDifferentValueTag */



/* 
* FUN : cmPkNhuPredefinedConfigStatusLstVarSz
*  DESC: Function to Pack NhuPredefinedConfigStatusLstVarSz structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPredefinedConfigStatusLstVarSz
(
NhuPredefinedConfigStatusLstVarSz *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPredefinedConfigStatusLstVarSz(param, mBuf)
NhuPredefinedConfigStatusLstVarSz *param;
Buffer *mBuf;
#endif
{ 
    Cntr i;
    TRC3(cmPkNhuPredefinedConfigStatusLstVarSz) 
    
    if( param->noComp.pres != NOTPRSNT ) 
    { 
        for(i = param->noComp.val-1; i >= 0; i--) 
        { 
            CMCHKPK(cmPkNhuPredefinedConfigStatusInfo, (&param->member[i]), mBuf);
        } 
    } 
    CMCHKPK(cmPkTknU16, &param->noComp, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPredefinedConfigStatusLstVarSz */



/* 
* FUN : cmPkNhuPredefinedConfigStatusLstComp
*  DESC: Function to Pack NhuPredefinedConfigStatusLstComp structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPredefinedConfigStatusLstComp
(
NhuPredefinedConfigStatusLstComp *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPredefinedConfigStatusLstComp(param, mBuf)
NhuPredefinedConfigStatusLstComp *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPredefinedConfigStatusLstComp) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuPredefinedConfigStatusLstVarSz, &param->otherEntries, mBuf); 
        CMCHKPK(cmPkNhuPredefinedConfigSetsWithDifferentValueTag, &param->setsWithDifferentValueTag, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPredefinedConfigStatusLstComp */

#define cmPkNhuRF_CapabBandFDDComp cmPkTknU32


/* 
* FUN : cmPkNhuRF_CapabBandLstFDDComp
*  DESC: Function to Pack NhuRF_CapabBandLstFDDComp structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuRF_CapabBandLstFDDComp
(
NhuRF_CapabBandLstFDDComp *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuRF_CapabBandLstFDDComp(param, mBuf)
NhuRF_CapabBandLstFDDComp *param;
Buffer *mBuf;
#endif
{ 
    Cntr i;
    TRC3(cmPkNhuRF_CapabBandLstFDDComp) 
    
    if( param->noComp.pres != NOTPRSNT ) 
    { 
        for(i = param->noComp.val-1; i >= 0; i--) 
        { 
            CMCHKPK(cmPkNhuRF_CapabBandFDDComp, (&param->member[i]), mBuf);
        } 
    } 
    CMCHKPK(cmPkTknU16, &param->noComp, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuRF_CapabBandLstFDDComp */



/* 
* FUN : cmPkNhuRF_CapCompfdd
*  DESC: Function to Pack NhuRF_CapCompfdd structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuRF_CapCompfdd
(
NhuRF_CapCompfdd *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuRF_CapCompfdd(param, mBuf)
NhuRF_CapCompfdd *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuRF_CapCompfdd) 
    
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case FDD_SUPPORTED :
                CMCHKPK(cmPkNhuRF_CapabBandLstFDDComp, &param->val.supported, mBuf); 
                break;
            case FDD_NOTSUPP :
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    CMCHKPK(cmPkTknU8, &param->choice, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuRF_CapCompfdd */



/* 
* FUN : cmPkNhuRF_CapComptdd384_RF_Cap
*  DESC: Function to Pack NhuRF_CapComptdd384_RF_Cap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuRF_CapComptdd384_RF_Cap
(
NhuRF_CapComptdd384_RF_Cap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuRF_CapComptdd384_RF_Cap(param, mBuf)
NhuRF_CapComptdd384_RF_Cap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuRF_CapComptdd384_RF_Cap) 
    
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case TDD384_RF_CAP_SUPPORTED :
                CMCHKPK(cmPkNhuRadioFrequencyBandTDDLst, &param->val.supported, mBuf); 
                break;
            case TDD384_RF_CAP_NOTSUPP :
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    CMCHKPK(cmPkTknU8, &param->choice, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuRF_CapComptdd384_RF_Cap */



/* 
* FUN : cmPkNhuRF_CapComptdd128_RF_Cap
*  DESC: Function to Pack NhuRF_CapComptdd128_RF_Cap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuRF_CapComptdd128_RF_Cap
(
NhuRF_CapComptdd128_RF_Cap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuRF_CapComptdd128_RF_Cap(param, mBuf)
NhuRF_CapComptdd128_RF_Cap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuRF_CapComptdd128_RF_Cap) 
    
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case TDD128_RF_CAP_SUPPORTED :
                CMCHKPK(cmPkNhuRadioFrequencyBandTDDLst, &param->val.supported, mBuf); 
                break;
            case TDD128_RF_CAP_NOTSUPP :
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    CMCHKPK(cmPkTknU8, &param->choice, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuRF_CapComptdd128_RF_Cap */



/* 
* FUN : cmPkNhuRF_CapComp
*  DESC: Function to Pack NhuRF_CapComp structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuRF_CapComp
(
NhuRF_CapComp *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuRF_CapComp(param, mBuf)
NhuRF_CapComp *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuRF_CapComp) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuRF_CapComptdd128_RF_Cap, &param->tdd128_RF_Cap, mBuf); 
        CMCHKPK(cmPkNhuRF_CapComptdd384_RF_Cap, &param->tdd384_RF_Cap, mBuf); 
        CMCHKPK(cmPkNhuRF_CapCompfdd, &param->fdd, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuRF_CapComp */

#define cmPkNhuUE_RadioAccessCapComptotalAM_RLCMemoryExceeds10kB cmPkTknU8


/* 
* FUN : cmPkNhuUE_RadioAccessCapComp
*  DESC: Function to Pack NhuUE_RadioAccessCapComp structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCapComp
(
NhuUE_RadioAccessCapComp *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCapComp(param, mBuf)
NhuUE_RadioAccessCapComp *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_RadioAccessCapComp) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuRF_CapComp, &param->rf_CapComp, mBuf); 
        CMCHKPK(cmPkNhuUE_RadioAccessCapComptotalAM_RLCMemoryExceeds10kB, &param->totalAM_RLCMemoryExceeds10kB, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCapComp */



/* 
* FUN : cmPkNhuInterRATHovrInfo_v590ext_IEs
*  DESC: Function to Pack NhuInterRATHovrInfo_v590ext_IEs structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v590ext_IEs
(
NhuInterRATHovrInfo_v590ext_IEs *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v590ext_IEs(param, mBuf)
NhuInterRATHovrInfo_v590ext_IEs *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuInterRATHovrInfo_v590ext_IEs) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUE_RadioAccessCapComp, &param->ue_RadioAccessCapComp, mBuf); 
        CMCHKPK(cmPkNhuPredefinedConfigStatusLstComp, &param->predefinedConfigStatusLstComp, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuInterRATHovrInfo_v590ext_IEs */



/* 
* FUN : cmPkNhuUE_SecInfo2
*  DESC: Function to Pack NhuUE_SecInfo2 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_SecInfo2
(
NhuUE_SecInfo2 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_SecInfo2(param, mBuf)
NhuUE_SecInfo2 *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_SecInfo2) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuSTART_Value, &param->start_PS, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_SecInfo2 */



/* 
* FUN : cmPkNhuRF_CapabBandLstFDDComp_ext
*  DESC: Function to Pack NhuRF_CapabBandLstFDDComp_ext structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuRF_CapabBandLstFDDComp_ext
(
NhuRF_CapabBandLstFDDComp_ext *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuRF_CapabBandLstFDDComp_ext(param, mBuf)
NhuRF_CapabBandLstFDDComp_ext *param;
Buffer *mBuf;
#endif
{ 
    Cntr i;
    TRC3(cmPkNhuRF_CapabBandLstFDDComp_ext) 
    
    if( param->noComp.pres != NOTPRSNT ) 
    { 
        for(i = param->noComp.val-1; i >= 0; i--) 
        { 
            CMCHKPK(cmPkNhuRF_CapabBandFDDComp, (&param->member[i]), mBuf);
        } 
    } 
    CMCHKPK(cmPkTknU16, &param->noComp, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuRF_CapabBandLstFDDComp_ext */



/* 
* FUN : cmPkNhuUE_RadioAccessCapComp_ext
*  DESC: Function to Pack NhuUE_RadioAccessCapComp_ext structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCapComp_ext
(
NhuUE_RadioAccessCapComp_ext *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCapComp_ext(param, mBuf)
NhuUE_RadioAccessCapComp_ext *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_RadioAccessCapComp_ext) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuRF_CapabBandLstFDDComp_ext, &param->rf_CapFDDComp, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCapComp_ext */

#define cmPkNhuDL_CapWithSimultaneousHS_DSCHConfig cmPkTknU32
#define cmPkNhuHSDSCH_physical_layer_category cmPkTknU32 
#define cmPkNhuPhysicalChannelCap_edch_r6fdd_edchsupportededch_PhysicalLyrCategory cmPkTknU32 


/* 
* FUN : cmPkNhuPhysicalChannelCap_edch_r6fdd_edchsupported
*  DESC: Function to Pack NhuPhysicalChannelCap_edch_r6fdd_edchsupported structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPhysicalChannelCap_edch_r6fdd_edchsupported
(
NhuPhysicalChannelCap_edch_r6fdd_edchsupported *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPhysicalChannelCap_edch_r6fdd_edchsupported(param, mBuf)
NhuPhysicalChannelCap_edch_r6fdd_edchsupported *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPhysicalChannelCap_edch_r6fdd_edchsupported) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuPhysicalChannelCap_edch_r6fdd_edchsupportededch_PhysicalLyrCategory, &param->edch_PhysicalLyrCategory, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPhysicalChannelCap_edch_r6fdd_edchsupported */



/* 
* FUN : cmPkNhuPhysicalChannelCap_edch_r6fdd_edch
*  DESC: Function to Pack NhuPhysicalChannelCap_edch_r6fdd_edch structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPhysicalChannelCap_edch_r6fdd_edch
(
NhuPhysicalChannelCap_edch_r6fdd_edch *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPhysicalChannelCap_edch_r6fdd_edch(param, mBuf)
NhuPhysicalChannelCap_edch_r6fdd_edch *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPhysicalChannelCap_edch_r6fdd_edch) 
    
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case FDD_EDCH_UNSUPPORTED :
                break;
            case FDD_EDCH_SUPPORTED :
                CMCHKPK(cmPkNhuPhysicalChannelCap_edch_r6fdd_edchsupported, &param->val.supported, mBuf); 
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    CMCHKPK(cmPkTknU8, &param->choice, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPhysicalChannelCap_edch_r6fdd_edch */



/* 
* FUN : cmPkNhuPhysicalChannelCap_edch_r6
*  DESC: Function to Pack NhuPhysicalChannelCap_edch_r6 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPhysicalChannelCap_edch_r6
(
NhuPhysicalChannelCap_edch_r6 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPhysicalChannelCap_edch_r6(param, mBuf)
NhuPhysicalChannelCap_edch_r6 *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPhysicalChannelCap_edch_r6) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuPhysicalChannelCap_edch_r6fdd_edch, &param->fdd_edch, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPhysicalChannelCap_edch_r6 */



/* 
* FUN : cmPkNhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch
*  DESC: Function to Pack NhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch
(
NhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch(param, mBuf)
NhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuPhysicalChannelCap_edch_r6, &param->physicalChannelCap_edch_r6, mBuf); 
        CMCHKPK(cmPkNhuHSDSCH_physical_layer_category, &param->physicalChannelCapabComp_hspdsch_r6, mBuf); 
        CMCHKPK(cmPkNhuDL_CapWithSimultaneousHS_DSCHConfig, &param->dl_CapWithSimultaneousHS_DSCHConfig, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch */



/* 
* FUN : cmPkNhuUE_RadioAccessCapComp2
*  DESC: Function to Pack NhuUE_RadioAccessCapComp2 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCapComp2
(
NhuUE_RadioAccessCapComp2 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCapComp2(param, mBuf)
NhuUE_RadioAccessCapComp2 *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_RadioAccessCapComp2) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch, &param->fddPhysicalChannelCapab_hspdsch_edch, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCapComp2 */



/* 
* FUN : cmPkNhuInterRATHovrInfo_v690ext_IEs
*  DESC: Function to Pack NhuInterRATHovrInfo_v690ext_IEs structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v690ext_IEs
(
NhuInterRATHovrInfo_v690ext_IEs *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v690ext_IEs(param, mBuf)
NhuInterRATHovrInfo_v690ext_IEs *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuInterRATHovrInfo_v690ext_IEs) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUE_RadioAccessCapComp2, &param->ue_RadioAccessCapComp2, mBuf); 
        CMCHKPK(cmPkNhuUE_RadioAccessCapComp_ext, &param->ue_RadioAccessCapComp, mBuf); 
        CMCHKPK(cmPkNhuUE_SecInfo2, &param->ue_SecInfo2, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuInterRATHovrInfo_v690ext_IEs */

#define cmPkNhuInterRATHovrInfo_v6b0ext_IEssupportForSIB11bis cmPkTknU32


/* 
* FUN : cmPkNhuInterRATHovrInfo_v6b0ext_IEs
*  DESC: Function to Pack NhuInterRATHovrInfo_v6b0ext_IEs structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v6b0ext_IEs
(
NhuInterRATHovrInfo_v6b0ext_IEs *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v6b0ext_IEs(param, mBuf)
NhuInterRATHovrInfo_v6b0ext_IEs *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuInterRATHovrInfo_v6b0ext_IEs) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuInterRATHovrInfo_v6b0ext_IEssupportForSIB11bis, &param->supportForSIB11bis, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuInterRATHovrInfo_v6b0ext_IEs */



/* 
* FUN : cmPkNhuUTRANInterRATHovrInfopredefinedConfigStatusLst
*  DESC: Function to Pack NhuUTRANInterRATHovrInfopredefinedConfigStatusLst structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfopredefinedConfigStatusLst
(
NhuUTRANInterRATHovrInfopredefinedConfigStatusLst *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfopredefinedConfigStatusLst(param, mBuf)
NhuUTRANInterRATHovrInfopredefinedConfigStatusLst *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUTRANInterRATHovrInfopredefinedConfigStatusLst) 
    
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case PREDEFINEDCONFIGSTATUSLST_PRESENT :
                CMCHKPK(cmPkNhuPredefinedConfigStatusLst, &param->val.present, mBuf); 
                break;
            case PREDEFINEDCONFIGSTATUSLST_ABSENT :
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    CMCHKPK(cmPkTknU8, &param->choice, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUTRANInterRATHovrInfopredefinedConfigStatusLst */



/* 
* FUN : cmPkNhuUTRANInterRATHovrInfouE_SecInfo
*  DESC: Function to Pack NhuUTRANInterRATHovrInfouE_SecInfo structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfouE_SecInfo
(
NhuUTRANInterRATHovrInfouE_SecInfo *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfouE_SecInfo(param, mBuf)
NhuUTRANInterRATHovrInfouE_SecInfo *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUTRANInterRATHovrInfouE_SecInfo) 
    
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case UE_SECINFO_PRESENT :
                CMCHKPK(cmPkNhuUE_SecInfo, &param->val.present, mBuf); 
                break;
            case UE_SECINFO_ABSENT :
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    CMCHKPK(cmPkTknU8, &param->choice, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUTRANInterRATHovrInfouE_SecInfo */

#define cmPkNhuUTRANInterRATHovrInfoue_CapContpresent cmPkTknStrOSXL 


/* 
* FUN : cmPkNhuUTRANInterRATHovrInfoue_CapCont
*  DESC: Function to Pack NhuUTRANInterRATHovrInfoue_CapCont structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfoue_CapCont
(
NhuUTRANInterRATHovrInfoue_CapCont *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfoue_CapCont(param, mBuf)
NhuUTRANInterRATHovrInfoue_CapCont *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUTRANInterRATHovrInfoue_CapCont) 
    
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case UE_CAPCONT_PRESENT :
                CMCHKPK(cmPkNhuUTRANInterRATHovrInfoue_CapContpresent, &param->val.present, mBuf); 
                break;
            case UE_CAPCONT_ABSENT :
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    CMCHKPK(cmPkTknU8, &param->choice, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUTRANInterRATHovrInfoue_CapCont */

#define cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsinterRATHovrInfo_r3_add_ext cmPkTknStrBSXL 


/* 
* FUN : cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns
*  DESC: Function to Pack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns(param, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns */



/* 
* FUN : cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns
*  DESC: Function to Pack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns(param, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns, &param->nonCriticalExtns, mBuf); 
        CMCHKPK(cmPkNhuInterRATHovrInfo_v6b0ext_IEs, &param->interRATHovrInfo_v6b0ext, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns */



/* 
* FUN : cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns
*  DESC: Function to Pack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns(param, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns, &param->v6b0NonCriticalExtns, mBuf); 
        CMCHKPK(cmPkNhuInterRATHovrInfo_v690ext_IEs, &param->interRATHovrInfo_v690ext, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns */



/* 
* FUN : cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns
*  DESC: Function to Pack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns(param, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns, &param->v690NonCriticalExtns, mBuf); 
        CMCHKPK(cmPkNhuInterRATHovrInfo_v590ext_IEs, &param->interRATHovrInfo_v590ext, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns */



/* 
* FUN : cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns
*  DESC: Function to Pack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns(param, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns, &param->v590NonCriticalExtns, mBuf); 
        CMCHKPK(cmPkNhuInterRATHovrInfo_v4d0ext_IEs, &param->interRATHovrInfo_v4d0ext, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns */



/* 
* FUN : cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns
*  DESC: Function to Pack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns(param, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns, &param->v4d0NonCriticalExtns, mBuf); 
        CMCHKPK(cmPkNhuInterRATHovrInfo_v4b0ext_IEs, &param->interRATHovrInfo_v4b0ext, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns */



/* 
* FUN : cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns
*  DESC: Function to Pack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns(param, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns, &param->v4b0NonCriticalExtns, mBuf); 
        CMCHKPK(cmPkNhuInterRATHovrInfo_v3g0ext_IEs, &param->interRATHovrInfo_v3g0ext, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns */



/* 
* FUN : cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns
*  DESC: Function to Pack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns(param, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns, &param->v3g0NonCriticalExtns, mBuf); 
        CMCHKPK(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsinterRATHovrInfo_r3_add_ext, &param->interRATHovrInfo_r3_add_ext, mBuf); 
        CMCHKPK(cmPkNhuInterRATHovrInfo_v3d0ext_IEs, &param->interRATHovrInfo_v3d0ext, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns */



/* 
* FUN : cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns
*  DESC: Function to Pack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns(param, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns, &param->laterNonCriticalExtns, mBuf); 
        CMCHKPK(cmPkNhuInterRATHovrInfo_v3a0ext_IEs, &param->interRATHovrInfo_v3a0ext, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns */



/* 
* FUN : cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresent
*  DESC: Function to Pack NhuUTRANInterRATHovrInfov390NonCriticalExtnspresent structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresent
(
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresent *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresent(param, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtnspresent *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresent) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns, &param->v3a0NonCriticalExtns, mBuf); 
        CMCHKPK(cmPkNhuInterRATHovrInfo_v390ext_IEs, &param->interRATHovrInfo_v390ext, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresent */



/* 
* FUN : cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtns
*  DESC: Function to Pack NhuUTRANInterRATHovrInfov390NonCriticalExtns structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtns
(
NhuUTRANInterRATHovrInfov390NonCriticalExtns *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtns(param, mBuf)
NhuUTRANInterRATHovrInfov390NonCriticalExtns *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtns) 
    
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case V390NONCRITICALEXTNS_PRESENT :
                CMCHKPK(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtnspresent, &param->val.present, mBuf); 
                break;
            case V390NONCRITICALEXTNS_ABSENT :
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    CMCHKPK(cmPkTknU8, &param->choice, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtns */



/* 
* FUN : cmPkNhuUTRANInterRATHovrInfo
*  DESC: Function to Pack NhuUTRANInterRATHovrInfo structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfo
(
NhuUTRANInterRATHovrInfo *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUTRANInterRATHovrInfo(param, mBuf)
NhuUTRANInterRATHovrInfo *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUTRANInterRATHovrInfo) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUTRANInterRATHovrInfov390NonCriticalExtns, &param->v390NonCriticalExtns, mBuf); 
        CMCHKPK(cmPkNhuUTRANInterRATHovrInfoue_CapCont, &param->ue_CapCont, mBuf); 
        CMCHKPK(cmPkNhuUTRANInterRATHovrInfouE_SecInfo, &param->uE_SecInfo, mBuf); 
        CMCHKPK(cmPkNhuUTRANInterRATHovrInfopredefinedConfigStatusLst, &param->predefinedConfigStatusLst, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUTRANInterRATHovrInfo */

#define cmPkNhuRadioFrequencyBandFDD2 cmPkTknU32
#define cmPkNhuCompressedModeMeasCapabFDD2dl_MeasurementsFDD cmPkTknU8
#define cmPkNhuCompressedModeMeasCapabFDD2ul_MeasurementsFDD cmPkTknU8


/* 
* FUN : cmPkNhuCompressedModeMeasCapabFDD2
*  DESC: Function to Pack NhuCompressedModeMeasCapabFDD2 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabFDD2
(
NhuCompressedModeMeasCapabFDD2 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabFDD2(param, mBuf)
NhuCompressedModeMeasCapabFDD2 *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuCompressedModeMeasCapabFDD2) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuCompressedModeMeasCapabFDD2ul_MeasurementsFDD, &param->ul_MeasurementsFDD, mBuf); 
        CMCHKPK(cmPkNhuCompressedModeMeasCapabFDD2dl_MeasurementsFDD, &param->dl_MeasurementsFDD, mBuf); 
        CMCHKPK(cmPkNhuRadioFrequencyBandFDD2, &param->radioFrequencyBandFDD2, mBuf); 
        CMCHKPK(cmPkNhuRadioFrequencyBandFDD, &param->radioFrequencyBandFDD, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuCompressedModeMeasCapabFDD2 */



/* 
* FUN : cmPkNhuCompressedModeMeasCapabFDDLst2
*  DESC: Function to Pack NhuCompressedModeMeasCapabFDDLst2 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabFDDLst2
(
NhuCompressedModeMeasCapabFDDLst2 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabFDDLst2(param, mBuf)
NhuCompressedModeMeasCapabFDDLst2 *param;
Buffer *mBuf;
#endif
{ 
    Cntr i;
    TRC3(cmPkNhuCompressedModeMeasCapabFDDLst2) 
    
    if( param->noComp.pres != NOTPRSNT ) 
    { 
        for(i = param->noComp.val-1; i >= 0; i--) 
        { 
            CMCHKPK(cmPkNhuCompressedModeMeasCapabFDD2, (&param->member[i]), mBuf);
        } 
    } 
    CMCHKPK(cmPkTknU16, &param->noComp, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuCompressedModeMeasCapabFDDLst2 */



/* 
* FUN : cmPkNhuMeasurementCapExt2
*  DESC: Function to Pack NhuMeasurementCapExt2 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuMeasurementCapExt2
(
NhuMeasurementCapExt2 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuMeasurementCapExt2(param, mBuf)
NhuMeasurementCapExt2 *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuMeasurementCapExt2) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuCompressedModeMeasCapabMC, &param->compressedModeMeasCapabMC, mBuf); 
        CMCHKPK(cmPkNhuCompressedModeMeasCapabGSMLst, &param->compressedModeMeasCapabGSMLst, mBuf); 
        CMCHKPK(cmPkNhuCompressedModeMeasCapabTDDLst, &param->compressedModeMeasCapabTDDLst, mBuf); 
        CMCHKPK(cmPkNhuCompressedModeMeasCapabFDDLst2, &param->compressedModeMeasCapabFDDLst, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuMeasurementCapExt2 */



/* 
* FUN : cmPkNhuUE_RadioAccessCapabBandFDD2fddRF_Cap
*  DESC: Function to Pack NhuUE_RadioAccessCapabBandFDD2fddRF_Cap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCapabBandFDD2fddRF_Cap
(
NhuUE_RadioAccessCapabBandFDD2fddRF_Cap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCapabBandFDD2fddRF_Cap(param, mBuf)
NhuUE_RadioAccessCapabBandFDD2fddRF_Cap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_RadioAccessCapabBandFDD2fddRF_Cap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuTxRxFrequencySeparation, &param->txRxFrequencySeparation, mBuf); 
        CMCHKPK(cmPkNhuUE_PowerClassExt, &param->ue_PowerClass, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCapabBandFDD2fddRF_Cap */



/* 
* FUN : cmPkNhuUE_RadioAccessCapabBandFDD2
*  DESC: Function to Pack NhuUE_RadioAccessCapabBandFDD2 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCapabBandFDD2
(
NhuUE_RadioAccessCapabBandFDD2 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCapabBandFDD2(param, mBuf)
NhuUE_RadioAccessCapabBandFDD2 *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_RadioAccessCapabBandFDD2) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuMeasurementCapExt2, &param->measurementCap2, mBuf); 
        CMCHKPK(cmPkNhuUE_RadioAccessCapabBandFDD2fddRF_Cap, &param->fddRF_Cap, mBuf); 
        CMCHKPK(cmPkNhuRadioFrequencyBandFDD2, &param->radioFrequencyBandFDD2, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCapabBandFDD2 */



/* 
* FUN : cmPkNhuUE_RadioAccessCapabBandFDDLst2
*  DESC: Function to Pack NhuUE_RadioAccessCapabBandFDDLst2 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCapabBandFDDLst2
(
NhuUE_RadioAccessCapabBandFDDLst2 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCapabBandFDDLst2(param, mBuf)
NhuUE_RadioAccessCapabBandFDDLst2 *param;
Buffer *mBuf;
#endif
{ 
    Cntr i;
    TRC3(cmPkNhuUE_RadioAccessCapabBandFDDLst2) 
    
    if( param->noComp.pres != NOTPRSNT ) 
    { 
        for(i = param->noComp.val-1; i >= 0; i--) 
        { 
            CMCHKPK(cmPkNhuUE_RadioAccessCapabBandFDD2, (&param->member[i]), mBuf);
        } 
    } 
    CMCHKPK(cmPkTknU16, &param->noComp, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCapabBandFDDLst2 */

#define cmPkNhuCompressedModeMeasCapabFDD_extdl_MeasurementsFDD cmPkTknU8
#define cmPkNhuCompressedModeMeasCapabFDD_extul_MeasurementsFDD cmPkTknU8


/* 
* FUN : cmPkNhuCompressedModeMeasCapabFDD_ext
*  DESC: Function to Pack NhuCompressedModeMeasCapabFDD_ext structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabFDD_ext
(
NhuCompressedModeMeasCapabFDD_ext *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabFDD_ext(param, mBuf)
NhuCompressedModeMeasCapabFDD_ext *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuCompressedModeMeasCapabFDD_ext) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuCompressedModeMeasCapabFDD_extul_MeasurementsFDD, &param->ul_MeasurementsFDD, mBuf); 
        CMCHKPK(cmPkNhuCompressedModeMeasCapabFDD_extdl_MeasurementsFDD, &param->dl_MeasurementsFDD, mBuf); 
        CMCHKPK(cmPkNhuRadioFrequencyBandFDD2, &param->radioFrequencyBandFDD2, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuCompressedModeMeasCapabFDD_ext */



/* 
* FUN : cmPkNhuCompressedModeMeasCapabFDDLst_ext
*  DESC: Function to Pack NhuCompressedModeMeasCapabFDDLst_ext structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabFDDLst_ext
(
NhuCompressedModeMeasCapabFDDLst_ext *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuCompressedModeMeasCapabFDDLst_ext(param, mBuf)
NhuCompressedModeMeasCapabFDDLst_ext *param;
Buffer *mBuf;
#endif
{ 
    Cntr i;
    TRC3(cmPkNhuCompressedModeMeasCapabFDDLst_ext) 
    
    if( param->noComp.pres != NOTPRSNT ) 
    { 
        for(i = param->noComp.val-1; i >= 0; i--) 
        { 
            CMCHKPK(cmPkNhuCompressedModeMeasCapabFDD_ext, (&param->member[i]), mBuf);
        } 
    } 
    CMCHKPK(cmPkTknU16, &param->noComp, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuCompressedModeMeasCapabFDDLst_ext */



/* 
* FUN : cmPkNhuUE_RadioAccessCapabBandFDD_ext
*  DESC: Function to Pack NhuUE_RadioAccessCapabBandFDD_ext structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCapabBandFDD_ext
(
NhuUE_RadioAccessCapabBandFDD_ext *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCapabBandFDD_ext(param, mBuf)
NhuUE_RadioAccessCapabBandFDD_ext *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_RadioAccessCapabBandFDD_ext) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuCompressedModeMeasCapabFDDLst_ext, &param->compressedModeMeasCapabFDDLst_ext, mBuf); 
        CMCHKPK(cmPkNhuRadioFrequencyBandFDD, &param->radioFrequencyBandFDD, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCapabBandFDD_ext */



/* 
* FUN : cmPkNhuUE_RadioAccessCapabBandFDDLst_ext
*  DESC: Function to Pack NhuUE_RadioAccessCapabBandFDDLst_ext structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCapabBandFDDLst_ext
(
NhuUE_RadioAccessCapabBandFDDLst_ext *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCapabBandFDDLst_ext(param, mBuf)
NhuUE_RadioAccessCapabBandFDDLst_ext *param;
Buffer *mBuf;
#endif
{ 
    Cntr i;
    TRC3(cmPkNhuUE_RadioAccessCapabBandFDDLst_ext) 
    
    if( param->noComp.pres != NOTPRSNT ) 
    { 
        for(i = param->noComp.val-1; i >= 0; i--) 
        { 
            CMCHKPK(cmPkNhuUE_RadioAccessCapabBandFDD_ext, (&param->member[i]), mBuf);
        } 
    } 
    CMCHKPK(cmPkTknU16, &param->noComp, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCapabBandFDDLst_ext */



/* 
* FUN : cmPkNhuUE_RadioAccessCap_v650ext
*  DESC: Function to Pack NhuUE_RadioAccessCap_v650ext structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCap_v650ext
(
NhuUE_RadioAccessCap_v650ext *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCap_v650ext(param, mBuf)
NhuUE_RadioAccessCap_v650ext *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_RadioAccessCap_v650ext) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUE_RadioAccessCapabBandFDDLst_ext, &param->ue_RadioAccessCapabBandFDDLst_ext, mBuf); 
        CMCHKPK(cmPkNhuUE_RadioAccessCapabBandFDDLst2, &param->ue_RadioAccessCapabBandFDDLst2, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCap_v650ext */



/* 
* FUN : cmPkNhuInterRATHovrInfo_v690ext1_IEs
*  DESC: Function to Pack NhuInterRATHovrInfo_v690ext1_IEs structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v690ext1_IEs
(
NhuInterRATHovrInfo_v690ext1_IEs *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuInterRATHovrInfo_v690ext1_IEs(param, mBuf)
NhuInterRATHovrInfo_v690ext1_IEs *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuInterRATHovrInfo_v690ext1_IEs) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUE_RadioAccessCap_v650ext, &param->ue_RadioAccessCap_v650ext, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuInterRATHovrInfo_v690ext1_IEs */



/* 
* FUN : cmPkNhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns
*  DESC: Function to Pack NhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns
(
NhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns(param, mBuf)
NhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns */



/* 
* FUN : cmPkNhuInterRATHovrInfo_r3_add_ext_IEs
*  DESC: Function to Pack NhuInterRATHovrInfo_r3_add_ext_IEs structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuInterRATHovrInfo_r3_add_ext_IEs
(
NhuInterRATHovrInfo_r3_add_ext_IEs *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuInterRATHovrInfo_r3_add_ext_IEs(param, mBuf)
NhuInterRATHovrInfo_r3_add_ext_IEs *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuInterRATHovrInfo_r3_add_ext_IEs) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns, &param->nonCriticalExtns, mBuf); 
        CMCHKPK(cmPkNhuInterRATHovrInfo_v690ext1_IEs, &param->interRATHovrInfo_v690ext1, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuInterRATHovrInfo_r3_add_ext_IEs */

#define cmPkNhuRadioFrequencyBandTDDLst_r7 cmPkTknU32
#define cmPkNhuChipRateCap_r7 cmPkTknU32


/* 
* FUN : cmPkNhuRF_Cap_r7fddRF_Cap
*  DESC: Function to Pack NhuRF_Cap_r7fddRF_Cap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuRF_Cap_r7fddRF_Cap
(
NhuRF_Cap_r7fddRF_Cap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuRF_Cap_r7fddRF_Cap(param, mBuf)
NhuRF_Cap_r7fddRF_Cap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuRF_Cap_r7fddRF_Cap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuTxRxFrequencySeparation, &param->txRxFrequencySeparation, mBuf); 
        CMCHKPK(cmPkNhuUE_PowerClass, &param->ue_PowerClass, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuRF_Cap_r7fddRF_Cap */



/* 
* FUN : cmPkNhuRF_Cap_r7tddRF_Cap
*  DESC: Function to Pack NhuRF_Cap_r7tddRF_Cap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuRF_Cap_r7tddRF_Cap
(
NhuRF_Cap_r7tddRF_Cap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuRF_Cap_r7tddRF_Cap(param, mBuf)
NhuRF_Cap_r7tddRF_Cap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuRF_Cap_r7tddRF_Cap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuChipRateCap_r7, &param->chipRateCap, mBuf); 
        CMCHKPK(cmPkNhuRadioFrequencyBandTDDLst_r7, &param->radioFrequencyTDDBandLst, mBuf); 
        CMCHKPK(cmPkNhuUE_PowerClass, &param->ue_PowerClass, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuRF_Cap_r7tddRF_Cap */



/* 
* FUN : cmPkNhuRF_Cap_r7
*  DESC: Function to Pack NhuRF_Cap_r7 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuRF_Cap_r7
(
NhuRF_Cap_r7 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuRF_Cap_r7(param, mBuf)
NhuRF_Cap_r7 *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuRF_Cap_r7) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuRF_Cap_r7tddRF_Cap, &param->tddRF_Cap, mBuf); 
        CMCHKPK(cmPkNhuRF_Cap_r7fddRF_Cap, &param->fddRF_Cap, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuRF_Cap_r7 */

#define cmPkNhuMaxPhysChPerFrame_768 cmPkTknU32 
#define cmPkNhuMinimumSF_DL_768 cmPkTknU32
#define cmPkNhuMaxPhysChPerTS_768 cmPkTknU32 
#define cmPkNhuDL_PhysChCapTDD_768supportOfPDSCH cmPkTknU8


/* 
* FUN : cmPkNhuDL_PhysChCapTDD_768
*  DESC: Function to Pack NhuDL_PhysChCapTDD_768 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuDL_PhysChCapTDD_768
(
NhuDL_PhysChCapTDD_768 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuDL_PhysChCapTDD_768(param, mBuf)
NhuDL_PhysChCapTDD_768 *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuDL_PhysChCapTDD_768) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuMaxPhysChPerTS_768, &param->maxPhysChPerTS, mBuf); 
        CMCHKPK(cmPkNhuDL_PhysChCapTDD_768supportOfPDSCH, &param->supportOfPDSCH, mBuf); 
        CMCHKPK(cmPkNhuMinimumSF_DL_768, &param->minimumSF, mBuf); 
        CMCHKPK(cmPkNhuMaxPhysChPerFrame_768, &param->maxPhysChPerFrame, mBuf); 
        CMCHKPK(cmPkNhuMaxTS_PerFrame, &param->maxTS_PerFrame, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuDL_PhysChCapTDD_768 */



/* 
* FUN : cmPkNhuPhysicalChannelCap_r7fddPhysChCap
*  DESC: Function to Pack NhuPhysicalChannelCap_r7fddPhysChCap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPhysicalChannelCap_r7fddPhysChCap
(
NhuPhysicalChannelCap_r7fddPhysChCap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPhysicalChannelCap_r7fddPhysChCap(param, mBuf)
NhuPhysicalChannelCap_r7fddPhysChCap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPhysicalChannelCap_r7fddPhysChCap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUL_PhysChCapFDD, &param->uplinkPhysChCap, mBuf); 
        CMCHKPK(cmPkNhuDL_PhysChCapFDD, &param->downlinkPhysChCap, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPhysicalChannelCap_r7fddPhysChCap */



/* 
* FUN : cmPkNhuPhysicalChannelCap_r7tddPhysChCap
*  DESC: Function to Pack NhuPhysicalChannelCap_r7tddPhysChCap structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPhysicalChannelCap_r7tddPhysChCap
(
NhuPhysicalChannelCap_r7tddPhysChCap *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPhysicalChannelCap_r7tddPhysChCap(param, mBuf)
NhuPhysicalChannelCap_r7tddPhysChCap *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPhysicalChannelCap_r7tddPhysChCap) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUL_PhysChCapTDD, &param->uplinkPhysChCap, mBuf); 
        CMCHKPK(cmPkNhuDL_PhysChCapTDD, &param->downlinkPhysChCap, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPhysicalChannelCap_r7tddPhysChCap */



/* 
* FUN : cmPkNhuPhysicalChannelCap_r7tddPhysChCap_768
*  DESC: Function to Pack NhuPhysicalChannelCap_r7tddPhysChCap_768 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPhysicalChannelCap_r7tddPhysChCap_768
(
NhuPhysicalChannelCap_r7tddPhysChCap_768 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPhysicalChannelCap_r7tddPhysChCap_768(param, mBuf)
NhuPhysicalChannelCap_r7tddPhysChCap_768 *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPhysicalChannelCap_r7tddPhysChCap_768) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuUL_PhysChCapTDD, &param->uplinkPhysChCap, mBuf); 
        CMCHKPK(cmPkNhuDL_PhysChCapTDD_768, &param->downlinkPhysChCap, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPhysicalChannelCap_r7tddPhysChCap_768 */



/* 
* FUN : cmPkNhuPhysicalChannelCap_r7
*  DESC: Function to Pack NhuPhysicalChannelCap_r7 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPhysicalChannelCap_r7
(
NhuPhysicalChannelCap_r7 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPhysicalChannelCap_r7(param, mBuf)
NhuPhysicalChannelCap_r7 *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPhysicalChannelCap_r7) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuPhysicalChannelCap_r7tddPhysChCap_768, &param->tddPhysChCap_768, mBuf); 
        CMCHKPK(cmPkNhuPhysicalChannelCap_r7tddPhysChCap, &param->tddPhysChCap, mBuf); 
        CMCHKPK(cmPkNhuPhysicalChannelCap_r7fddPhysChCap, &param->fddPhysChCap, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPhysicalChannelCap_r7 */

#define cmPkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupportedtdd_edch_PhysicalLyrCategory cmPkTknU32 


/* 
* FUN : cmPkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupported
*  DESC: Function to Pack NhuPhysicalChannelCap_tddedch_r7tdd_edchsupported structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupported
(
NhuPhysicalChannelCap_tddedch_r7tdd_edchsupported *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupported(param, mBuf)
NhuPhysicalChannelCap_tddedch_r7tdd_edchsupported *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupported) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupportedtdd_edch_PhysicalLyrCategory, &param->tdd_edch_PhysicalLyrCategory, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupported */



/* 
* FUN : cmPkNhuPhysicalChannelCap_tddedch_r7tdd_edch
*  DESC: Function to Pack NhuPhysicalChannelCap_tddedch_r7tdd_edch structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPhysicalChannelCap_tddedch_r7tdd_edch
(
NhuPhysicalChannelCap_tddedch_r7tdd_edch *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPhysicalChannelCap_tddedch_r7tdd_edch(param, mBuf)
NhuPhysicalChannelCap_tddedch_r7tdd_edch *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPhysicalChannelCap_tddedch_r7tdd_edch) 
    
    if(param->choice.pres) 
    {
        switch( param->choice.val ) 
        {
            case TDD_EDCH_UNSUPPORTED :
                break;
            case TDD_EDCH_SUPPORTED :
                CMCHKPK(cmPkNhuPhysicalChannelCap_tddedch_r7tdd_edchsupported, &param->val.supported, mBuf); 
                break;
            default:
                RETVALUE(RFAILED); 
        } 
    } 
    CMCHKPK(cmPkTknU8, &param->choice, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPhysicalChannelCap_tddedch_r7tdd_edch */



/* 
* FUN : cmPkNhuPhysicalChannelCap_tddedch_r7
*  DESC: Function to Pack NhuPhysicalChannelCap_tddedch_r7 structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuPhysicalChannelCap_tddedch_r7
(
NhuPhysicalChannelCap_tddedch_r7 *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuPhysicalChannelCap_tddedch_r7(param, mBuf)
NhuPhysicalChannelCap_tddedch_r7 *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuPhysicalChannelCap_tddedch_r7) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuPhysicalChannelCap_tddedch_r7tdd_edch, &param->tdd_edch, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuPhysicalChannelCap_tddedch_r7 */



/* 
* FUN : cmPkNhuUE_RadioAccessCap_v7xyext
*  DESC: Function to Pack NhuUE_RadioAccessCap_v7xyext structure
* RETURN: ROK - OK
*/ 
#ifdef ANSI 
PUBLIC S16 cmPkNhuUE_RadioAccessCap_v7xyext
(
NhuUE_RadioAccessCap_v7xyext *param,
Buffer *mBuf
)
#else 
PUBLIC S16 cmPkNhuUE_RadioAccessCap_v7xyext(param, mBuf)
NhuUE_RadioAccessCap_v7xyext *param;
Buffer *mBuf;
#endif
{
    
    TRC3(cmPkNhuUE_RadioAccessCap_v7xyext) 
    
    if( param->pres.pres != NOTPRSNT ) 
    {
        CMCHKPK(cmPkNhuPhysicalChannelCap_tddedch_r7, &param->physicalchannelcapability_tddedch, mBuf); 
        CMCHKPK(cmPkNhuPhysicalChannelCap_r7, &param->physicalChannelCap_r7, mBuf); 
        CMCHKPK(cmPkNhuRF_Cap_r7, &param->rf_Cap, mBuf); 
    }
    CMCHKPK(cmPkTknPres, &param->pres, mBuf); 
    RETVALUE(ROK);
}     /* End of function cmPkNhuUE_RadioAccessCap_v7xyext */

/**********************************************************************
         End of file:     nhu_3gpk.c@@/main/1 - Fri Dec 11 18:45:14 2009
**********************************************************************/
/**********************************************************************

        Revision history:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---    ch        1. RRC LTE Initial Release.
*********************************************************************91*/
