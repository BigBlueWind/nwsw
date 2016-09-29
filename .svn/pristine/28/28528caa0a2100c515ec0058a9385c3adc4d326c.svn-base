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

     Type:     C source file

     Desc:     Contains LTE RRC ASN1.C Database

     File:     nh_db.c

     Sid:      ni_3gdb.c@@/main/2 - Fri Jul  1 01:13:12 2011

     Prg:      chebli

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
#include "cm_llist.h"         /* */
#include "cm_hash.h"
#include "nhu_3gasn.h"
/* header/extern include files (.x) */
#include "gen.x"              /* general layer */
#include "ssi.x"              /* system services */
#include "cm5.x"              /* */
#include "cm_tkns.x"          /* */
#include "cm_mblk.x"
#include "cm_llist.x"
#include "cm_pasn.x"          /* PASN */
#include "nhu_3gasn.x"


U32 flag3gNh3O = CM_PASN_ELM_OPT | CM_PASN_POST_DECODE_ESC_CALL;
U32 flag3gNh3M = CM_PASN_ELM_MAND | CM_PASN_POST_DECODE_ESC_CALL;
U32 flag3gNh3D = CM_PASN_ELM_DEF | CM_PASN_POST_DECODE_ESC_CALL;
CmPAsnElmDef nh3gConstTerm =
{
#ifdef CM_PASN_DBG
   " nh3gConstTerm (Set-Sequence Terminator) ",
#endif /* CM_PASN_DEBUG */
   CM_PASN_TET_SETSEQ_TERM,
   CM_PASN_EXT_MKR_ABS,
   0,
   &flag3gNh3M,    
   CM_PASN_ESC_FUNC_NULLP,
   NULLP 
}; 
PUBLIC CmPAsnElmDef nh3gNull =
{
#ifdef CM_PASN_DBG
   " nh3gNull ",
#endif /* CM_PASN_DEBUG */
  CM_PASN_TET_NULL,
  CM_PASN_EXT_MKR_ABS,
  0,        
  &flag3gNh3M,
  CM_PASN_ESC_FUNC_NULLP,
  NULLP
}; 
CmPAsnElmDef nh3gExtMkr =
{  
#ifdef CM_PASN_DBG
  " nh3gExtMkr(Extension Marker) ",
#endif /* CM_PASN_DEBUG */
  CM_PASN_TET_EXT_MKR,
  CM_PASN_EXT_MKR_ABS,
  0,
  &flag3gNh3M,
  CM_PASN_ESC_FUNC_NULLP,
  NULLP 
};
/* Element ----- MaxHcContextSpace ------- */
CmPAsnTknEnum nhMaxHcCtxtSpaceLst[6] =
{
    5,
    NhuMaxHcCtxtSpacedummyEnum,
    NhuMaxHcCtxtSpaceby1024Enum,
    NhuMaxHcCtxtSpaceby2048Enum,
    NhuMaxHcCtxtSpaceby4096Enum,
    NhuMaxHcCtxtSpaceby8192Enum
};
CmPAsnEnumDef nhMaxHcCtxtSpacespec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMaxHcCtxtSpaceLst,
    NULLP
};
CmPAsnElmDef nhMaxHcCtxtSpace =
{
#ifdef CM_PASN_DBG
    " nhMaxHcCtxtSpace ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxHcCtxtSpace),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxHcCtxtSpacespec),
    NULLP
};

CmPAsnElmDef nhPDCP_CaplosslessSRNS_RelocationSupport =
{
#ifdef CM_PASN_DBG
    " nhPDCP_CaplosslessSRNS_RelocationSupport ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhPDCP_CapsupportForRfc2507Tkn[] =
{
    &nh3gNull,
    &nhMaxHcCtxtSpace,
    &nh3gConstTerm
};
CmPAsnChoiceDef nhPDCP_CapsupportForRfc2507spec = 
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_CHOICE,
#endif /* CM_PASN_DBG */
    2,
    0,
    &nhPDCP_CapsupportForRfc2507Tkn[0]  /* Token List */
};
CmPAsnElmDef nhPDCP_CapsupportForRfc2507 =
{
#ifdef CM_PASN_DBG
    " nhPDCP_CapsupportForRfc2507 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_CHOICE,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPDCP_CapsupportForRfc2507),
    &flag3gNh3M,
    NULLP,
    (U8 *)(&nhPDCP_CapsupportForRfc2507spec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhPDCP_CapTkn[] =
{
    &nhPDCP_CaplosslessSRNS_RelocationSupport,
    &nhPDCP_CapsupportForRfc2507,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhPDCP_Capspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhPDCP_CapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhPDCP_Cap =
{
#ifdef CM_PASN_DBG
    " nhPDCP_Cap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPDCP_Cap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPDCP_Capspec),
    NULLP
};
/* Element ----- TotalRLC-AM-BufferSize ------- */
CmPAsnTknEnum nhTotalRLC_AM_BufferSizeLst[9] =
{
    8,
    NhuTotalRLC_AM_BufferSizedummyEnum,
    NhuTotalRLC_AM_BufferSizekb10Enum,
    NhuTotalRLC_AM_BufferSizekb50Enum,
    NhuTotalRLC_AM_BufferSizekb100Enum,
    NhuTotalRLC_AM_BufferSizekb150Enum,
    NhuTotalRLC_AM_BufferSizekb500Enum,
    NhuTotalRLC_AM_BufferSizekb1000Enum,
    NhuTotalRLC_AM_BufferSizespareEnum
};
CmPAsnEnumDef nhTotalRLC_AM_BufferSizespec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhTotalRLC_AM_BufferSizeLst,
    NULLP
};
CmPAsnElmDef nhTotalRLC_AM_BufferSize =
{
#ifdef CM_PASN_DBG
    " nhTotalRLC_AM_BufferSize ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuTotalRLC_AM_BufferSize),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhTotalRLC_AM_BufferSizespec),
    NULLP
};

/* Element ----- MaximumRLC-WindowSize ------- */
CmPAsnTknEnum nhMaxRLC_WindowSizeLst[3] =
{
    2,
    NhuMaxRLC_WindowSizemws2047Enum,
    NhuMaxRLC_WindowSizemws4095Enum
};
CmPAsnEnumDef nhMaxRLC_WindowSizespec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMaxRLC_WindowSizeLst,
    NULLP
};
CmPAsnElmDef nhMaxRLC_WindowSize =
{
#ifdef CM_PASN_DBG
    " nhMaxRLC_WindowSize ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxRLC_WindowSize),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxRLC_WindowSizespec),
    NULLP
};

/* Element ----- MaximumAM-EntityNumberRLC-Cap ------- */
CmPAsnTknEnum nhMaxAM_EntityNumberRLC_CapLst[8] =
{
    7,
    NhuMaxAM_EntityNumberRLC_CapdummyEnum,
    NhuMaxAM_EntityNumberRLC_Capam4Enum,
    NhuMaxAM_EntityNumberRLC_Capam5Enum,
    NhuMaxAM_EntityNumberRLC_Capam6Enum,
    NhuMaxAM_EntityNumberRLC_Capam8Enum,
    NhuMaxAM_EntityNumberRLC_Capam16Enum,
    NhuMaxAM_EntityNumberRLC_Capam30Enum
};
CmPAsnEnumDef nhMaxAM_EntityNumberRLC_Capspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMaxAM_EntityNumberRLC_CapLst,
    NULLP
};
CmPAsnElmDef nhMaxAM_EntityNumberRLC_Cap =
{
#ifdef CM_PASN_DBG
    " nhMaxAM_EntityNumberRLC_Cap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxAM_EntityNumberRLC_Cap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxAM_EntityNumberRLC_Capspec),
    NULLP
};

PUBLIC CmPAsnElmDef *nhRLC_CapTkn[] =
{
    &nhTotalRLC_AM_BufferSize,
    &nhMaxRLC_WindowSize,
    &nhMaxAM_EntityNumberRLC_Cap,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhRLC_Capspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhRLC_CapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhRLC_Cap =
{
#ifdef CM_PASN_DBG
    " nhRLC_Cap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRLC_Cap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRLC_Capspec),
    NULLP
};
/* Element ----- MaxNoBits ------- */
CmPAsnTknEnum nhMaxNoBitsLst[14] =
{
    13,
    NhuMaxNoBitsb640Enum,
    NhuMaxNoBitsb1280Enum,
    NhuMaxNoBitsb2560Enum,
    NhuMaxNoBitsb3840Enum,
    NhuMaxNoBitsb5120Enum,
    NhuMaxNoBitsb6400Enum,
    NhuMaxNoBitsb7680Enum,
    NhuMaxNoBitsb8960Enum,
    NhuMaxNoBitsb10240Enum,
    NhuMaxNoBitsb20480Enum,
    NhuMaxNoBitsb40960Enum,
    NhuMaxNoBitsb81920Enum,
    NhuMaxNoBitsb163840Enum
};
CmPAsnEnumDef nhMaxNoBitsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMaxNoBitsLst,
    NULLP
};
CmPAsnElmDef nhMaxNoBits =
{
#ifdef CM_PASN_DBG
    " nhMaxNoBits ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxNoBits),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxNoBitsspec),
    NULLP
};

PUBLIC CmPAsnElmDef *nhTurboSupportTkn[] =
{
    &nh3gNull,
    &nhMaxNoBits,
    &nh3gConstTerm
};
CmPAsnChoiceDef nhTurboSupportspec = 
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_CHOICE,
#endif /* CM_PASN_DEBUG */
    2,
    0,
    &nhTurboSupportTkn[0]  /* Token List */
};
CmPAsnElmDef nhTurboSupport =
{
#ifdef CM_PASN_DBG
    " nhTurboSupport ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_CHOICE,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuTurboSupport),
    &flag3gNh3M,
    NULLP,
    (U8 *)(&nhTurboSupportspec),
    NULLP
};
/* Element ----- MaxSimultaneousTransChsDL ------- */
CmPAsnTknEnum nhMaxSimultaneousTransChsDLLst[5] =
{
    4,
    NhuMaxSimultaneousTransChsDLe4Enum,
    NhuMaxSimultaneousTransChsDLe8Enum,
    NhuMaxSimultaneousTransChsDLe16Enum,
    NhuMaxSimultaneousTransChsDLe32Enum
};
CmPAsnEnumDef nhMaxSimultaneousTransChsDLspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMaxSimultaneousTransChsDLLst,
    NULLP
};
CmPAsnElmDef nhMaxSimultaneousTransChsDL =
{
#ifdef CM_PASN_DBG
    " nhMaxSimultaneousTransChsDL ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxSimultaneousTransChsDL),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxSimultaneousTransChsDLspec),
    NULLP
};

/* Element ----- MaxSimultaneousCCTrCH-Count ------- */
CmPAsnUIntDef nhMaxSimultaneousCCTrCH_Countspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
    1,
    8UL
};
CmPAsnElmDef nhMaxSimultaneousCCTrCH_Count =
{
#ifdef CM_PASN_DBG
    " nhMaxSimultaneousCCTrCH_Count ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_UINT32,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxSimultaneousCCTrCH_Count),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxSimultaneousCCTrCH_Countspec),
    NULLP
};

/* Element ----- MaxTransportBlocksDL ------- */
CmPAsnTknEnum nhMaxTportBlocksDLLst[11] =
{
    10,
    NhuMaxTportBlocksDLtb4Enum,
    NhuMaxTportBlocksDLtb8Enum,
    NhuMaxTportBlocksDLtb16Enum,
    NhuMaxTportBlocksDLtb32Enum,
    NhuMaxTportBlocksDLtb48Enum,
    NhuMaxTportBlocksDLtb64Enum,
    NhuMaxTportBlocksDLtb96Enum,
    NhuMaxTportBlocksDLtb128Enum,
    NhuMaxTportBlocksDLtb256Enum,
    NhuMaxTportBlocksDLtb512Enum
};
CmPAsnEnumDef nhMaxTportBlocksDLspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMaxTportBlocksDLLst,
    NULLP
};
CmPAsnElmDef nhMaxTportBlocksDL =
{
#ifdef CM_PASN_DBG
    " nhMaxTportBlocksDL ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxTportBlocksDL),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxTportBlocksDLspec),
    NULLP
};

/* Element ----- MaxNumberOfTFC-DL ------- */
CmPAsnTknEnum nhMaxNumberOfTFC_DLLst[10] =
{
    9,
    NhuMaxNumberOfTFC_DLtfc16Enum,
    NhuMaxNumberOfTFC_DLtfc32Enum,
    NhuMaxNumberOfTFC_DLtfc48Enum,
    NhuMaxNumberOfTFC_DLtfc64Enum,
    NhuMaxNumberOfTFC_DLtfc96Enum,
    NhuMaxNumberOfTFC_DLtfc128Enum,
    NhuMaxNumberOfTFC_DLtfc256Enum,
    NhuMaxNumberOfTFC_DLtfc512Enum,
    NhuMaxNumberOfTFC_DLtfc1024Enum
};
CmPAsnEnumDef nhMaxNumberOfTFC_DLspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMaxNumberOfTFC_DLLst,
    NULLP
};
CmPAsnElmDef nhMaxNumberOfTFC_DL =
{
#ifdef CM_PASN_DBG
    " nhMaxNumberOfTFC_DL ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxNumberOfTFC_DL),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxNumberOfTFC_DLspec),
    NULLP
};

/* Element ----- MaxNumberOfTF ------- */
CmPAsnTknEnum nhMaxNumberOfTFLst[7] =
{
    6,
    NhuMaxNumberOfTFtf32Enum,
    NhuMaxNumberOfTFtf64Enum,
    NhuMaxNumberOfTFtf128Enum,
    NhuMaxNumberOfTFtf256Enum,
    NhuMaxNumberOfTFtf512Enum,
    NhuMaxNumberOfTFtf1024Enum
};
CmPAsnEnumDef nhMaxNumberOfTFspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMaxNumberOfTFLst,
    NULLP
};
CmPAsnElmDef nhMaxNumberOfTF =
{
#ifdef CM_PASN_DBG
    " nhMaxNumberOfTF ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxNumberOfTF),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxNumberOfTFspec),
    NULLP
};

PUBLIC CmPAsnElmDef *nhDL_TransChCapTkn[] =
{
    &nhMaxNoBits,
    &nhMaxNoBits,
    &nhTurboSupport,
    &nhMaxSimultaneousTransChsDL,
    &nhMaxSimultaneousCCTrCH_Count,
    &nhMaxTportBlocksDL,
    &nhMaxNumberOfTFC_DL,
    &nhMaxNumberOfTF,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhDL_TransChCapspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhDL_TransChCapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhDL_TransChCap =
{
#ifdef CM_PASN_DBG
    " nhDL_TransChCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuDL_TransChCap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhDL_TransChCapspec),
    NULLP
};
/* Element ----- MaxSimultaneousTransChsUL ------- */
CmPAsnTknEnum nhMaxSimultaneousTransChsULLst[6] =
{
    5,
    NhuMaxSimultaneousTransChsULdummyEnum,
    NhuMaxSimultaneousTransChsULe4Enum,
    NhuMaxSimultaneousTransChsULe8Enum,
    NhuMaxSimultaneousTransChsULe16Enum,
    NhuMaxSimultaneousTransChsULe32Enum
};
CmPAsnEnumDef nhMaxSimultaneousTransChsULspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMaxSimultaneousTransChsULLst,
    NULLP
};
CmPAsnElmDef nhMaxSimultaneousTransChsUL =
{
#ifdef CM_PASN_DBG
    " nhMaxSimultaneousTransChsUL ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxSimultaneousTransChsUL),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxSimultaneousTransChsULspec),
    NULLP
};

/* Element ----- MaxTransportBlocksUL ------- */
CmPAsnTknEnum nhMaxTportBlocksULLst[12] =
{
    11,
    NhuMaxTportBlocksULdummyEnum,
    NhuMaxTportBlocksULtb4Enum,
    NhuMaxTportBlocksULtb8Enum,
    NhuMaxTportBlocksULtb16Enum,
    NhuMaxTportBlocksULtb32Enum,
    NhuMaxTportBlocksULtb48Enum,
    NhuMaxTportBlocksULtb64Enum,
    NhuMaxTportBlocksULtb96Enum,
    NhuMaxTportBlocksULtb128Enum,
    NhuMaxTportBlocksULtb256Enum,
    NhuMaxTportBlocksULtb512Enum
};
CmPAsnEnumDef nhMaxTportBlocksULspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMaxTportBlocksULLst,
    NULLP
};
CmPAsnElmDef nhMaxTportBlocksUL =
{
#ifdef CM_PASN_DBG
    " nhMaxTportBlocksUL ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxTportBlocksUL),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxTportBlocksULspec),
    NULLP
};

/* Element ----- MaxNumberOfTFC-UL ------- */
CmPAsnTknEnum nhMaxNumberOfTFC_ULLst[12] =
{
    11,
    NhuMaxNumberOfTFC_ULdummy1Enum,
    NhuMaxNumberOfTFC_ULdummy2Enum,
    NhuMaxNumberOfTFC_ULtfc16Enum,
    NhuMaxNumberOfTFC_ULtfc32Enum,
    NhuMaxNumberOfTFC_ULtfc48Enum,
    NhuMaxNumberOfTFC_ULtfc64Enum,
    NhuMaxNumberOfTFC_ULtfc96Enum,
    NhuMaxNumberOfTFC_ULtfc128Enum,
    NhuMaxNumberOfTFC_ULtfc256Enum,
    NhuMaxNumberOfTFC_ULtfc512Enum,
    NhuMaxNumberOfTFC_ULtfc1024Enum
};
CmPAsnEnumDef nhMaxNumberOfTFC_ULspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMaxNumberOfTFC_ULLst,
    NULLP
};
CmPAsnElmDef nhMaxNumberOfTFC_UL =
{
#ifdef CM_PASN_DBG
    " nhMaxNumberOfTFC_UL ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxNumberOfTFC_UL),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxNumberOfTFC_ULspec),
    NULLP
};

PUBLIC CmPAsnElmDef *nhUL_TransChCapmodeSpecificInfotddTkn[] =
{
    &nhMaxSimultaneousCCTrCH_Count,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUL_TransChCapmodeSpecificInfotddspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUL_TransChCapmodeSpecificInfotddTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUL_TransChCapmodeSpecificInfotdd =
{
#ifdef CM_PASN_DBG
    " nhUL_TransChCapmodeSpecificInfotdd ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUL_TransChCapmodeSpecificInfotdd),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUL_TransChCapmodeSpecificInfotddspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUL_TransChCapmodeSpecificInfoTkn[] =
{
    &nh3gNull,
    &nhUL_TransChCapmodeSpecificInfotdd,
    &nh3gConstTerm
};
CmPAsnChoiceDef nhUL_TransChCapmodeSpecificInfospec = 
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_CHOICE,
#endif /* CM_PASN_DEBUG */
    2,
    0,
    &nhUL_TransChCapmodeSpecificInfoTkn[0]  /* Token List */
};
CmPAsnElmDef nhUL_TransChCapmodeSpecificInfo =
{
#ifdef CM_PASN_DBG
    " nhUL_TransChCapmodeSpecificInfo ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_CHOICE,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUL_TransChCapmodeSpecificInfo),
    &flag3gNh3M,
    NULLP,
    (U8 *)(&nhUL_TransChCapmodeSpecificInfospec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUL_TransChCapTkn[] =
{
    &nhMaxNoBits,
    &nhMaxNoBits,
    &nhTurboSupport,
    &nhMaxSimultaneousTransChsUL,
    &nhUL_TransChCapmodeSpecificInfo,
    &nhMaxTportBlocksUL,
    &nhMaxNumberOfTFC_UL,
    &nhMaxNumberOfTF,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUL_TransChCapspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUL_TransChCapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUL_TransChCap =
{
#ifdef CM_PASN_DBG
    " nhUL_TransChCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUL_TransChCap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUL_TransChCapspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhTportChannelCapTkn[] =
{
    &nhDL_TransChCap,
    &nhUL_TransChCap,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhTportChannelCapspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhTportChannelCapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhTportChannelCap =
{
#ifdef CM_PASN_DBG
    " nhTportChannelCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuTportChannelCap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhTportChannelCapspec),
    NULLP
};
/* Element ----- UE-PowerClass ------- */
CmPAsnUIntDef nhUE_PowerClassspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
    1,
    4UL
};
CmPAsnElmDef nhUE_PowerClass =
{
#ifdef CM_PASN_DBG
    " nhUE_PowerClass ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_UINT32,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_PowerClass),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_PowerClassspec),
    NULLP
};

/* Element ----- TxRxFrequencySeparation ------- */
CmPAsnTknEnum nhTxRxFrequencySeparationLst[4] =
{
    3,
    NhuTxRxFrequencySeparationdefault_TxRx_separationEnum,
    NhuTxRxFrequencySeparationspare2Enum,
    NhuTxRxFrequencySeparationspare1Enum
};
CmPAsnEnumDef nhTxRxFrequencySeparationspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhTxRxFrequencySeparationLst,
    NULLP
};
CmPAsnElmDef nhTxRxFrequencySeparation =
{
#ifdef CM_PASN_DBG
    " nhTxRxFrequencySeparation ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuTxRxFrequencySeparation),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhTxRxFrequencySeparationspec),
    NULLP
};

/* Element ----- RadioFrequencyBandTDDList ------- */
CmPAsnTknEnum nhRadioFrequencyBandTDDLstLst[9] =
{
    8,
    NhuRadioFrequencyBandTDDLstaEnum,
    NhuRadioFrequencyBandTDDLstbEnum,
    NhuRadioFrequencyBandTDDLstcEnum,
    NhuRadioFrequencyBandTDDLstabEnum,
    NhuRadioFrequencyBandTDDLstacEnum,
    NhuRadioFrequencyBandTDDLstbcEnum,
    NhuRadioFrequencyBandTDDLstabcEnum,
    NhuRadioFrequencyBandTDDLstspareEnum
};
CmPAsnEnumDef nhRadioFrequencyBandTDDLstspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhRadioFrequencyBandTDDLstLst,
    NULLP
};
CmPAsnElmDef nhOptRadioFrequencyBandTDDLst =
{
#ifdef CM_PASN_DBG
    " nhRadioFrequencyBandTDDLst ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRadioFrequencyBandTDDLst),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRadioFrequencyBandTDDLstspec),
    NULLP
};
CmPAsnElmDef nhRadioFrequencyBandTDDLst =
{
#ifdef CM_PASN_DBG
    " nhRadioFrequencyBandTDDLst ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRadioFrequencyBandTDDLst),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRadioFrequencyBandTDDLstspec),
    NULLP
};

/* Element ----- ChipRateCapability ------- */
CmPAsnTknEnum nhChipRateCapLst[3] =
{
    2,
    NhuChipRateCapmcps3_84Enum,
    NhuChipRateCapmcps1_28Enum
};
CmPAsnEnumDef nhChipRateCapspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhChipRateCapLst,
    NULLP
};
CmPAsnElmDef nhChipRateCap =
{
#ifdef CM_PASN_DBG
    " nhChipRateCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuChipRateCap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhChipRateCapspec),
    NULLP
};

PUBLIC CmPAsnElmDef *nhRF_CapfddRF_CapTkn[] =
{
    &nhUE_PowerClass,
    &nhTxRxFrequencySeparation,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhRF_CapfddRF_Capspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhRF_CapfddRF_CapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptRF_CapfddRF_Cap =
{
#ifdef CM_PASN_DBG
    " nhRF_CapfddRF_Cap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRF_CapfddRF_Cap),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRF_CapfddRF_Capspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhRF_CaptddRF_CapTkn[] =
{
    &nhUE_PowerClass,
    &nhRadioFrequencyBandTDDLst,
    &nhChipRateCap,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhRF_CaptddRF_Capspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhRF_CaptddRF_CapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptRF_CaptddRF_Cap =
{
#ifdef CM_PASN_DBG
    " nhRF_CaptddRF_Cap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRF_CaptddRF_Cap),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRF_CaptddRF_Capspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhRF_CapTkn[] =
{
    &nhOptRF_CapfddRF_Cap,
    &nhOptRF_CaptddRF_Cap,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhRF_Capspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    2,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhRF_CapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhRF_Cap =
{
#ifdef CM_PASN_DBG
    " nhRF_Cap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRF_Cap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRF_Capspec),
    NULLP
};
/* Element ----- MaxNoPhysChBitsReceived ------- */
CmPAsnTknEnum nhMaxNoPhysChBitsReceivedLst[16] =
{
    15,
    NhuMaxNoPhysChBitsReceiveddummyEnum,
    NhuMaxNoPhysChBitsReceivedb1200Enum,
    NhuMaxNoPhysChBitsReceivedb2400Enum,
    NhuMaxNoPhysChBitsReceivedb3600Enum,
    NhuMaxNoPhysChBitsReceivedb4800Enum,
    NhuMaxNoPhysChBitsReceivedb7200Enum,
    NhuMaxNoPhysChBitsReceivedb9600Enum,
    NhuMaxNoPhysChBitsReceivedb14400Enum,
    NhuMaxNoPhysChBitsReceivedb19200Enum,
    NhuMaxNoPhysChBitsReceivedb28800Enum,
    NhuMaxNoPhysChBitsReceivedb38400Enum,
    NhuMaxNoPhysChBitsReceivedb48000Enum,
    NhuMaxNoPhysChBitsReceivedb57600Enum,
    NhuMaxNoPhysChBitsReceivedb67200Enum,
    NhuMaxNoPhysChBitsReceivedb76800Enum
};
CmPAsnEnumDef nhMaxNoPhysChBitsReceivedspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMaxNoPhysChBitsReceivedLst,
    NULLP
};
CmPAsnElmDef nhMaxNoPhysChBitsReceived =
{
#ifdef CM_PASN_DBG
    " nhMaxNoPhysChBitsReceived ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxNoPhysChBitsReceived),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxNoPhysChBitsReceivedspec),
    NULLP
};

/* Element ----- MaxNoSCCPCH-RL ------- */
CmPAsnTknEnum nhMaxNoSCCPCH_RLLst[2] =
{
    1,
    NhuMaxNoSCCPCH_RLrl1Enum
};
CmPAsnEnumDef nhMaxNoSCCPCH_RLspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMaxNoSCCPCH_RLLst,
    NULLP
};
CmPAsnElmDef nhMaxNoSCCPCH_RL =
{
#ifdef CM_PASN_DBG
    " nhMaxNoSCCPCH_RL ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxNoSCCPCH_RL),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxNoSCCPCH_RLspec),
    NULLP
};

CmPAsnElmDef nhSimultaneousSCCPCH_DPCH_ReceptionsupportedsimultaneousSCCPCH_DPCH_DPDCH_Reception =
{
#ifdef CM_PASN_DBG
    " nhSimultaneousSCCPCH_DPCH_ReceptionsupportedsimultaneousSCCPCH_DPCH_DPDCH_Reception ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhSimultaneousSCCPCH_DPCH_ReceptionsupportedTkn[] =
{
    &nhMaxNoSCCPCH_RL,
    &nhSimultaneousSCCPCH_DPCH_ReceptionsupportedsimultaneousSCCPCH_DPCH_DPDCH_Reception,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhSimultaneousSCCPCH_DPCH_Receptionsupportedspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhSimultaneousSCCPCH_DPCH_ReceptionsupportedTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhSimultaneousSCCPCH_DPCH_Receptionsupported =
{
#ifdef CM_PASN_DBG
    " nhSimultaneousSCCPCH_DPCH_Receptionsupported ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuSimultaneousSCCPCH_DPCH_Receptionsupported),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhSimultaneousSCCPCH_DPCH_Receptionsupportedspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhSimultaneousSCCPCH_DPCH_ReceptionTkn[] =
{
    &nh3gNull,
    &nhSimultaneousSCCPCH_DPCH_Receptionsupported,
    &nh3gConstTerm
};
CmPAsnChoiceDef nhSimultaneousSCCPCH_DPCH_Receptionspec = 
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_CHOICE,
#endif /* CM_PASN_DEBUG */
    2,
    0,
    &nhSimultaneousSCCPCH_DPCH_ReceptionTkn[0]  /* Token List */
};
CmPAsnElmDef nhSimultaneousSCCPCH_DPCH_Reception =
{
#ifdef CM_PASN_DBG
    " nhSimultaneousSCCPCH_DPCH_Reception ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_CHOICE,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuSimultaneousSCCPCH_DPCH_Reception),
    &flag3gNh3M,
    NULLP,
    (U8 *)(&nhSimultaneousSCCPCH_DPCH_Receptionspec),
    NULLP
};
/* Element ----- maxNoDPCH-PDSCH-Codes ------- */
CmPAsnUIntDef nhDL_PhysChCapFDDmaxNoDPCH_PDSCH_Codesspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
    1,
    8UL
};
CmPAsnElmDef nhDL_PhysChCapFDDmaxNoDPCH_PDSCH_Codes =
{
#ifdef CM_PASN_DBG
    " nhDL_PhysChCapFDDmaxNoDPCH_PDSCH_Codes ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_UINT32,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuDL_PhysChCapFDDmaxNoDPCH_PDSCH_Codes),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhDL_PhysChCapFDDmaxNoDPCH_PDSCH_Codesspec),
    NULLP
};

CmPAsnElmDef nhDL_PhysChCapFDDsupportForSF_512 =
{
#ifdef CM_PASN_DBG
    " nhDL_PhysChCapFDDsupportForSF_512 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

CmPAsnElmDef nhDL_PhysChCapFDDdummy =
{
#ifdef CM_PASN_DBG
    " nhDL_PhysChCapFDDdummy ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhDL_PhysChCapFDDTkn[] =
{
    &nhDL_PhysChCapFDDmaxNoDPCH_PDSCH_Codes,
    &nhMaxNoPhysChBitsReceived,
    &nhDL_PhysChCapFDDsupportForSF_512,
    &nhDL_PhysChCapFDDdummy,
    &nhSimultaneousSCCPCH_DPCH_Reception,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhDL_PhysChCapFDDspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhDL_PhysChCapFDDTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhDL_PhysChCapFDD =
{
#ifdef CM_PASN_DBG
    " nhDL_PhysChCapFDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuDL_PhysChCapFDD),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhDL_PhysChCapFDDspec),
    NULLP
};
/* Element ----- MaxNoDPDCH-BitsTransmitted ------- */
CmPAsnTknEnum nhMaxNoDPDCH_BitsTransmittedLst[11] =
{
    10,
    NhuMaxNoDPDCH_BitsTransmittedb600Enum,
    NhuMaxNoDPDCH_BitsTransmittedb1200Enum,
    NhuMaxNoDPDCH_BitsTransmittedb2400Enum,
    NhuMaxNoDPDCH_BitsTransmittedb4800Enum,
    NhuMaxNoDPDCH_BitsTransmittedb9600Enum,
    NhuMaxNoDPDCH_BitsTransmittedb19200Enum,
    NhuMaxNoDPDCH_BitsTransmittedb28800Enum,
    NhuMaxNoDPDCH_BitsTransmittedb38400Enum,
    NhuMaxNoDPDCH_BitsTransmittedb48000Enum,
    NhuMaxNoDPDCH_BitsTransmittedb57600Enum
};
CmPAsnEnumDef nhMaxNoDPDCH_BitsTransmittedspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMaxNoDPDCH_BitsTransmittedLst,
    NULLP
};
CmPAsnElmDef nhMaxNoDPDCH_BitsTransmitted =
{
#ifdef CM_PASN_DBG
    " nhMaxNoDPDCH_BitsTransmitted ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxNoDPDCH_BitsTransmitted),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxNoDPDCH_BitsTransmittedspec),
    NULLP
};

CmPAsnElmDef nhUL_PhysChCapFDDdummy =
{
#ifdef CM_PASN_DBG
    " nhUL_PhysChCapFDDdummy ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhUL_PhysChCapFDDTkn[] =
{
    &nhMaxNoDPDCH_BitsTransmitted,
    &nhUL_PhysChCapFDDdummy,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUL_PhysChCapFDDspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUL_PhysChCapFDDTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUL_PhysChCapFDD =
{
#ifdef CM_PASN_DBG
    " nhUL_PhysChCapFDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUL_PhysChCapFDD),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUL_PhysChCapFDDspec),
    NULLP
};
/* Element ----- MaxTS-PerFrame ------- */
CmPAsnUIntDef nhMaxTS_PerFramespec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
    1,
    14UL
};
CmPAsnElmDef nhMaxTS_PerFrame =
{
#ifdef CM_PASN_DBG
    " nhMaxTS_PerFrame ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_UINT32,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxTS_PerFrame),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxTS_PerFramespec),
    NULLP
};

/* Element ----- MaxPhysChPerFrame ------- */
CmPAsnUIntDef nhMaxPhysChPerFramespec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
    1,
    224UL
};
CmPAsnElmDef nhMaxPhysChPerFrame =
{
#ifdef CM_PASN_DBG
    " nhMaxPhysChPerFrame ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_UINT32,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxPhysChPerFrame),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxPhysChPerFramespec),
    NULLP
};

/* Element ----- MinimumSF-DL ------- */
CmPAsnTknEnum nhMinimumSF_DLLst[3] =
{
    2,
    NhuMinimumSF_DLsf1Enum,
    NhuMinimumSF_DLsf16Enum
};
CmPAsnEnumDef nhMinimumSF_DLspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMinimumSF_DLLst,
    NULLP
};
CmPAsnElmDef nhMinimumSF_DL =
{
#ifdef CM_PASN_DBG
    " nhMinimumSF_DL ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMinimumSF_DL),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMinimumSF_DLspec),
    NULLP
};

/* Element ----- MaxPhysChPerTS ------- */
CmPAsnUIntDef nhMaxPhysChPerTSspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
    1,
    16UL
};
CmPAsnElmDef nhMaxPhysChPerTS =
{
#ifdef CM_PASN_DBG
    " nhMaxPhysChPerTS ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_UINT32,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxPhysChPerTS),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxPhysChPerTSspec),
    NULLP
};

CmPAsnElmDef nhDL_PhysChCapTDDsupportOfPDSCH =
{
#ifdef CM_PASN_DBG
    " nhDL_PhysChCapTDDsupportOfPDSCH ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhDL_PhysChCapTDDTkn[] =
{
    &nhMaxTS_PerFrame,
    &nhMaxPhysChPerFrame,
    &nhMinimumSF_DL,
    &nhDL_PhysChCapTDDsupportOfPDSCH,
    &nhMaxPhysChPerTS,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhDL_PhysChCapTDDspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhDL_PhysChCapTDDTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhDL_PhysChCapTDD =
{
#ifdef CM_PASN_DBG
    " nhDL_PhysChCapTDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuDL_PhysChCapTDD),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhDL_PhysChCapTDDspec),
    NULLP
};
/* Element ----- MaxPhysChPerTimeslot ------- */
CmPAsnTknEnum nhMaxPhysChPerTimeslotLst[3] =
{
    2,
    NhuMaxPhysChPerTimeslotts1Enum,
    NhuMaxPhysChPerTimeslotts2Enum
};
CmPAsnEnumDef nhMaxPhysChPerTimeslotspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMaxPhysChPerTimeslotLst,
    NULLP
};
CmPAsnElmDef nhMaxPhysChPerTimeslot =
{
#ifdef CM_PASN_DBG
    " nhMaxPhysChPerTimeslot ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxPhysChPerTimeslot),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxPhysChPerTimeslotspec),
    NULLP
};

/* Element ----- MinimumSF-UL ------- */
CmPAsnTknEnum nhMinimumSF_ULLst[6] =
{
    5,
    NhuMinimumSF_ULsf1Enum,
    NhuMinimumSF_ULsf2Enum,
    NhuMinimumSF_ULsf4Enum,
    NhuMinimumSF_ULsf8Enum,
    NhuMinimumSF_ULdummyEnum
};
CmPAsnEnumDef nhMinimumSF_ULspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMinimumSF_ULLst,
    NULLP
};
CmPAsnElmDef nhMinimumSF_UL =
{
#ifdef CM_PASN_DBG
    " nhMinimumSF_UL ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMinimumSF_UL),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMinimumSF_ULspec),
    NULLP
};

CmPAsnElmDef nhUL_PhysChCapTDDsupportOfPUSCH =
{
#ifdef CM_PASN_DBG
    " nhUL_PhysChCapTDDsupportOfPUSCH ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhUL_PhysChCapTDDTkn[] =
{
    &nhMaxTS_PerFrame,
    &nhMaxPhysChPerTimeslot,
    &nhMinimumSF_UL,
    &nhUL_PhysChCapTDDsupportOfPUSCH,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUL_PhysChCapTDDspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUL_PhysChCapTDDTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUL_PhysChCapTDD =
{
#ifdef CM_PASN_DBG
    " nhUL_PhysChCapTDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUL_PhysChCapTDD),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUL_PhysChCapTDDspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhPhysicalChannelCapfddPhysChCapTkn[] =
{
    &nhDL_PhysChCapFDD,
    &nhUL_PhysChCapFDD,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhPhysicalChannelCapfddPhysChCapspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhPhysicalChannelCapfddPhysChCapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptPhysicalChannelCapfddPhysChCap =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCapfddPhysChCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCapfddPhysChCap),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPhysicalChannelCapfddPhysChCapspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhPhysicalChannelCaptddPhysChCapTkn[] =
{
    &nhDL_PhysChCapTDD,
    &nhUL_PhysChCapTDD,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhPhysicalChannelCaptddPhysChCapspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhPhysicalChannelCaptddPhysChCapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptPhysicalChannelCaptddPhysChCap =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCaptddPhysChCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCaptddPhysChCap),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPhysicalChannelCaptddPhysChCapspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhPhysicalChannelCapTkn[] =
{
    &nhOptPhysicalChannelCapfddPhysChCap,
    &nhOptPhysicalChannelCaptddPhysChCap,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhPhysicalChannelCapspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    2,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhPhysicalChannelCapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhPhysicalChannelCap =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPhysicalChannelCapspec),
    NULLP
};
CmPAsnElmDef nhMultiRAT_CapsupportOfGSM =
{
#ifdef CM_PASN_DBG
    " nhMultiRAT_CapsupportOfGSM ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

CmPAsnElmDef nhMultiRAT_CapsupportOfMulticarrier =
{
#ifdef CM_PASN_DBG
    " nhMultiRAT_CapsupportOfMulticarrier ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhMultiRAT_CapTkn[] =
{
    &nhMultiRAT_CapsupportOfGSM,
    &nhMultiRAT_CapsupportOfMulticarrier,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhMultiRAT_Capspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhMultiRAT_CapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhMultiRAT_Cap =
{
#ifdef CM_PASN_DBG
    " nhMultiRAT_Cap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMultiRAT_Cap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMultiRAT_Capspec),
    NULLP
};
/* Element ----- MultiModeCapability ------- */
CmPAsnTknEnum nhMultiModeCapLst[4] =
{
    3,
    NhuMultiModeCaptddEnum,
    NhuMultiModeCapfddEnum,
    NhuMultiModeCapfdd_tddEnum
};
CmPAsnEnumDef nhMultiModeCapspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMultiModeCapLst,
    NULLP
};
CmPAsnElmDef nhMultiModeCap =
{
#ifdef CM_PASN_DBG
    " nhMultiModeCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMultiModeCap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMultiModeCapspec),
    NULLP
};

PUBLIC CmPAsnElmDef *nhUE_MultiModeRAT_CapTkn[] =
{
    &nhMultiRAT_Cap,
    &nhMultiModeCap,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_MultiModeRAT_Capspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_MultiModeRAT_CapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_MultiModeRAT_Cap =
{
#ifdef CM_PASN_DBG
    " nhUE_MultiModeRAT_Cap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_MultiModeRAT_Cap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_MultiModeRAT_Capspec),
    NULLP
};
CmPAsnBitStrDef nhSecCapcipheringAlgorithmCapspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
    16,
    16
};
CmPAsnElmDef nhSecCapcipheringAlgorithmCap =
{
#ifdef CM_PASN_DBG
    " nhSecCapcipheringAlgorithmCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BITSTRXL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuSecCapcipheringAlgorithmCap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhSecCapcipheringAlgorithmCapspec),
    NULLP
};
CmPAsnBitStrDef nhSecCapintegrityProtectionAlgorithmCapspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
    16,
    16
};
CmPAsnElmDef nhSecCapintegrityProtectionAlgorithmCap =
{
#ifdef CM_PASN_DBG
    " nhSecCapintegrityProtectionAlgorithmCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BITSTRXL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuSecCapintegrityProtectionAlgorithmCap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhSecCapintegrityProtectionAlgorithmCapspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhSecCapTkn[] =
{
    &nhSecCapcipheringAlgorithmCap,
    &nhSecCapintegrityProtectionAlgorithmCap,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhSecCapspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhSecCapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhSecCap =
{
#ifdef CM_PASN_DBG
    " nhSecCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuSecCap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhSecCapspec),
    NULLP
};
/* Element ----- NetworkAssistedGPS-Supported ------- */
CmPAsnTknEnum nhNwAssistedGPS_SuppLst[5] =
{
    4,
    NhuNwAssistedGPS_SuppnetworkBasedEnum,
    NhuNwAssistedGPS_Suppue_BasedEnum,
    NhuNwAssistedGPS_SuppbothNwAndUE_BasedEnum,
    NhuNwAssistedGPS_SuppnoNwAssistedGPSEnum
};
CmPAsnEnumDef nhNwAssistedGPS_Suppspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhNwAssistedGPS_SuppLst,
    NULLP
};
CmPAsnElmDef nhNwAssistedGPS_Supp =
{
#ifdef CM_PASN_DBG
    " nhNwAssistedGPS_Supp ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuNwAssistedGPS_Supp),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhNwAssistedGPS_Suppspec),
    NULLP
};

CmPAsnElmDef nhUE_Positioning_CapstandaloneLocMethodsSupp =
{
#ifdef CM_PASN_DBG
    " nhUE_Positioning_CapstandaloneLocMethodsSupp ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

CmPAsnElmDef nhUE_Positioning_Capue_BasedOTDOA_Supp =
{
#ifdef CM_PASN_DBG
    " nhUE_Positioning_Capue_BasedOTDOA_Supp ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

CmPAsnElmDef nhUE_Positioning_CapsupportForUE_GPS_TimingOfCellFrames =
{
#ifdef CM_PASN_DBG
    " nhUE_Positioning_CapsupportForUE_GPS_TimingOfCellFrames ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

CmPAsnElmDef nhUE_Positioning_CapsupportForIPDL =
{
#ifdef CM_PASN_DBG
    " nhUE_Positioning_CapsupportForIPDL ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhUE_Positioning_CapTkn[] =
{
    &nhUE_Positioning_CapstandaloneLocMethodsSupp,
    &nhUE_Positioning_Capue_BasedOTDOA_Supp,
    &nhNwAssistedGPS_Supp,
    &nhUE_Positioning_CapsupportForUE_GPS_TimingOfCellFrames,
    &nhUE_Positioning_CapsupportForIPDL,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_Positioning_Capspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_Positioning_CapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_Positioning_Cap =
{
#ifdef CM_PASN_DBG
    " nhUE_Positioning_Cap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_Positioning_Cap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_Positioning_Capspec),
    NULLP
};
CmPAsnElmDef nhGSM_Measurementsgsm900 =
{
#ifdef CM_PASN_DBG
    " nhGSM_Measurementsgsm900 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

CmPAsnElmDef nhGSM_Measurementsdcs1800 =
{
#ifdef CM_PASN_DBG
    " nhGSM_Measurementsdcs1800 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

CmPAsnElmDef nhGSM_Measurementsgsm1900 =
{
#ifdef CM_PASN_DBG
    " nhGSM_Measurementsgsm1900 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhGSM_MeasurementsTkn[] =
{
    &nhGSM_Measurementsgsm900,
    &nhGSM_Measurementsdcs1800,
    &nhGSM_Measurementsgsm1900,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhGSM_Measurementsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhGSM_MeasurementsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhGSM_Measurements =
{
#ifdef CM_PASN_DBG
    " nhGSM_Measurements ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuGSM_Measurements),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhGSM_Measurementsspec),
    NULLP
};
PUBLIC CmPAsnElmDef nhOptGSM_Measurements =
{
#ifdef CM_PASN_DBG
    " nhGSM_Measurements ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuGSM_Measurements),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhGSM_Measurementsspec),
    NULLP
};
CmPAsnElmDef nhCompressedModeMeasCapfdd_Measurements =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapfdd_Measurements ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

CmPAsnElmDef nhOptCompressedModeMeasCaptdd_Measurements =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCaptdd_Measurements ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

CmPAsnElmDef nhOptCompressedModeMeasCapmultiCarrierMeasurements =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapmultiCarrierMeasurements ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhCompressedModeMeasCapTkn[] =
{
    &nhCompressedModeMeasCapfdd_Measurements,
    &nhOptCompressedModeMeasCaptdd_Measurements,
    &nhOptGSM_Measurements,
    &nhOptCompressedModeMeasCapmultiCarrierMeasurements,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhCompressedModeMeasCapspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    3,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhCompressedModeMeasCapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhCompressedModeMeasCap =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuCompressedModeMeasCap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhCompressedModeMeasCapspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhMeasurementCapTkn[] =
{
    &nhCompressedModeMeasCap,
    &nhCompressedModeMeasCap,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhMeasurementCapspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhMeasurementCapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhMeasurementCap =
{
#ifdef CM_PASN_DBG
    " nhMeasurementCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMeasurementCap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMeasurementCapspec),
    NULLP
};
PUBLIC CmPAsnElmDef nhOptMeasurementCap =
{
#ifdef CM_PASN_DBG
    " nhMeasurementCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMeasurementCap),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMeasurementCapspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_RadioAccessCapTkn[] =
{
    &nhPDCP_Cap,
    &nhRLC_Cap,
    &nhTportChannelCap,
    &nhRF_Cap,
    &nhPhysicalChannelCap,
    &nhUE_MultiModeRAT_Cap,
    &nhSecCap,
    &nhUE_Positioning_Cap,
    &nhOptMeasurementCap,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_RadioAccessCapspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_RadioAccessCapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_RadioAccessCap =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCapspec),
    NULLP
};
/* Element ----- RadioFrequencyBandFDD ------- */
CmPAsnTknEnum nhRadioFrequencyBandFDDLst[9] =
{
    8,
    NhuRadioFrequencyBandFDDfdd2100Enum,
    NhuRadioFrequencyBandFDDfdd1900Enum,
    NhuRadioFrequencyBandFDDfdd1800Enum,
    NhuRadioFrequencyBandFDDbandVIEnum,
    NhuRadioFrequencyBandFDDbandIVEnum,
    NhuRadioFrequencyBandFDDbandVEnum,
    NhuRadioFrequencyBandFDDbandVIIEnum,
    NhuRadioFrequencyBandFDDextension_indicatorEnum
};
CmPAsnEnumDef nhRadioFrequencyBandFDDspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhRadioFrequencyBandFDDLst,
    NULLP
};
CmPAsnElmDef nhOptRadioFrequencyBandFDD =
{
#ifdef CM_PASN_DBG
    " nhRadioFrequencyBandFDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRadioFrequencyBandFDD),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRadioFrequencyBandFDDspec),
    NULLP
};
CmPAsnElmDef nhRadioFrequencyBandFDD =
{
#ifdef CM_PASN_DBG
    " nhRadioFrequencyBandFDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRadioFrequencyBandFDD),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRadioFrequencyBandFDDspec),
    NULLP
};

/* Element ----- UE-PowerClassExt ------- */
CmPAsnTknEnum nhUE_PowerClassExtLst[9] =
{
    8,
    NhuUE_PowerClassExtclass1Enum,
    NhuUE_PowerClassExtclass2Enum,
    NhuUE_PowerClassExtclass3Enum,
    NhuUE_PowerClassExtclass4Enum,
    NhuUE_PowerClassExtspare4Enum,
    NhuUE_PowerClassExtspare3Enum,
    NhuUE_PowerClassExtspare2Enum,
    NhuUE_PowerClassExtspare1Enum
};
CmPAsnEnumDef nhUE_PowerClassExtspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhUE_PowerClassExtLst,
    NULLP
};
CmPAsnElmDef nhUE_PowerClassExt =
{
#ifdef CM_PASN_DBG
    " nhUE_PowerClassExt ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_PowerClassExt),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_PowerClassExtspec),
    NULLP
};

CmPAsnElmDef nhCompressedModeMeasCapabFDDdl_MeasurementsFDD =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabFDDdl_MeasurementsFDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

CmPAsnElmDef nhCompressedModeMeasCapabFDDul_MeasurementsFDD =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabFDDul_MeasurementsFDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhCompressedModeMeasCapabFDDTkn[] =
{
    &nhOptRadioFrequencyBandFDD,
    &nhCompressedModeMeasCapabFDDdl_MeasurementsFDD,
    &nhCompressedModeMeasCapabFDDul_MeasurementsFDD,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhCompressedModeMeasCapabFDDspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhCompressedModeMeasCapabFDDTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhCompressedModeMeasCapabFDD =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabFDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuCompressedModeMeasCapabFDD),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhCompressedModeMeasCapabFDDspec),
    NULLP
};
CmPAsnSetSeqOfDef nhCompressedModeMeasCapabFDDLstspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
    1,        /* start of the size */
    NhumaxFreqBandsFDD,        /* start of the size */
    3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef nhCompressedModeMeasCapabFDDLst =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabFDDLst ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQOF,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuCompressedModeMeasCapabFDDLst),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhCompressedModeMeasCapabFDDLstspec),
    NULLP
};
/* Element ----- RadioFrequencyBandTDD ------- */
CmPAsnTknEnum nhRadioFrequencyBandTDDLst1[5] =
{
    4,
    NhuRadioFrequencyBandTDDaEnum,
    NhuRadioFrequencyBandTDDbEnum,
    NhuRadioFrequencyBandTDDcEnum,
    NhuRadioFrequencyBandTDDdEnum
};
CmPAsnEnumDef nhRadioFrequencyBandTDDspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhRadioFrequencyBandTDDLst1,
    NULLP
};
CmPAsnElmDef nhRadioFrequencyBandTDD =
{
#ifdef CM_PASN_DBG
    " nhRadioFrequencyBandTDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRadioFrequencyBandTDD),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRadioFrequencyBandTDDspec),
    NULLP
};

CmPAsnElmDef nhCompressedModeMeasCapabTDDdl_MeasurementsTDD =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabTDDdl_MeasurementsTDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

CmPAsnElmDef nhCompressedModeMeasCapabTDDul_MeasurementsTDD =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabTDDul_MeasurementsTDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhCompressedModeMeasCapabTDDTkn[] =
{
    &nhRadioFrequencyBandTDD,
    &nhCompressedModeMeasCapabTDDdl_MeasurementsTDD,
    &nhCompressedModeMeasCapabTDDul_MeasurementsTDD,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhCompressedModeMeasCapabTDDspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhCompressedModeMeasCapabTDDTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhCompressedModeMeasCapabTDD =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabTDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuCompressedModeMeasCapabTDD),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhCompressedModeMeasCapabTDDspec),
    NULLP
};
CmPAsnSetSeqOfDef nhCompressedModeMeasCapabTDDLstspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
    1,        /* start of the size */
    NhumaxFreqBandsTDD,        /* start of the size */
    3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef nhOptCompressedModeMeasCapabTDDLst =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabTDDLst ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQOF,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuCompressedModeMeasCapabTDDLst),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhCompressedModeMeasCapabTDDLstspec),
    NULLP
};
/* Element ----- RadioFrequencyBandGSM ------- */
CmPAsnTknEnum nhRadioFrequencyBandGSMLst[17] =
{
    16,
    NhuRadioFrequencyBandGSMgsm450Enum,
    NhuRadioFrequencyBandGSMgsm480Enum,
    NhuRadioFrequencyBandGSMgsm850Enum,
    NhuRadioFrequencyBandGSMgsm900PEnum,
    NhuRadioFrequencyBandGSMgsm900EEnum,
    NhuRadioFrequencyBandGSMgsm1800Enum,
    NhuRadioFrequencyBandGSMgsm1900Enum,
    NhuRadioFrequencyBandGSMspare9Enum,
    NhuRadioFrequencyBandGSMspare8Enum,
    NhuRadioFrequencyBandGSMspare7Enum,
    NhuRadioFrequencyBandGSMspare6Enum,
    NhuRadioFrequencyBandGSMspare5Enum,
    NhuRadioFrequencyBandGSMspare4Enum,
    NhuRadioFrequencyBandGSMspare3Enum,
    NhuRadioFrequencyBandGSMspare2Enum,
    NhuRadioFrequencyBandGSMspare1Enum
};
CmPAsnEnumDef nhRadioFrequencyBandGSMspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhRadioFrequencyBandGSMLst,
    NULLP
};
CmPAsnElmDef nhRadioFrequencyBandGSM =
{
#ifdef CM_PASN_DBG
    " nhRadioFrequencyBandGSM ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRadioFrequencyBandGSM),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRadioFrequencyBandGSMspec),
    NULLP
};

CmPAsnElmDef nhCompressedModeMeasCapabGSMdl_MeasurementsGSM =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabGSMdl_MeasurementsGSM ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

CmPAsnElmDef nhCompressedModeMeasCapabGSMul_MeasurementsGSM =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabGSMul_MeasurementsGSM ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhCompressedModeMeasCapabGSMTkn[] =
{
    &nhRadioFrequencyBandGSM,
    &nhCompressedModeMeasCapabGSMdl_MeasurementsGSM,
    &nhCompressedModeMeasCapabGSMul_MeasurementsGSM,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhCompressedModeMeasCapabGSMspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhCompressedModeMeasCapabGSMTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhCompressedModeMeasCapabGSM =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabGSM ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuCompressedModeMeasCapabGSM),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhCompressedModeMeasCapabGSMspec),
    NULLP
};
CmPAsnSetSeqOfDef nhCompressedModeMeasCapabGSMLstspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
    1,        /* start of the size */
    NhumaxFreqBandsGSM,        /* start of the size */
    3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef nhOptCompressedModeMeasCapabGSMLst =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabGSMLst ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQOF,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuCompressedModeMeasCapabGSMLst),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhCompressedModeMeasCapabGSMLstspec),
    NULLP
};
CmPAsnElmDef nhCompressedModeMeasCapabMCdl_MeasurementsMC =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabMCdl_MeasurementsMC ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

CmPAsnElmDef nhCompressedModeMeasCapabMCul_MeasurementsMC =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabMCul_MeasurementsMC ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhCompressedModeMeasCapabMCTkn[] =
{
    &nhCompressedModeMeasCapabMCdl_MeasurementsMC,
    &nhCompressedModeMeasCapabMCul_MeasurementsMC,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhCompressedModeMeasCapabMCspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhCompressedModeMeasCapabMCTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhCompressedModeMeasCapabMC =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabMC ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuCompressedModeMeasCapabMC),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhCompressedModeMeasCapabMCspec),
    NULLP
};
PUBLIC CmPAsnElmDef nhOptCompressedModeMeasCapabMC =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabMC ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuCompressedModeMeasCapabMC),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhCompressedModeMeasCapabMCspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhMeasurementCapExtTkn[] =
{
    &nhCompressedModeMeasCapabFDDLst,
        &nhCompressedModeMeasCapabFDD,
        &nh3gConstTerm,
    &nhOptCompressedModeMeasCapabTDDLst,
        &nhCompressedModeMeasCapabTDD,
        &nh3gConstTerm,
    &nhOptCompressedModeMeasCapabGSMLst,
        &nhCompressedModeMeasCapabGSM,
        &nh3gConstTerm,
    &nhOptCompressedModeMeasCapabMC,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhMeasurementCapExtspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    3,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhMeasurementCapExtTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhMeasurementCapExt =
{
#ifdef CM_PASN_DBG
    " nhMeasurementCapExt ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMeasurementCapExt),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMeasurementCapExtspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_RadioAccessCapabBandFDDfddRF_CapTkn[] =
{
    &nhUE_PowerClassExt,
    &nhTxRxFrequencySeparation,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_RadioAccessCapabBandFDDfddRF_Capspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_RadioAccessCapabBandFDDfddRF_CapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptUE_RadioAccessCapabBandFDDfddRF_Cap =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCapabBandFDDfddRF_Cap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCapabBandFDDfddRF_Cap),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCapabBandFDDfddRF_Capspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_RadioAccessCapabBandFDDTkn[] =
{
    &nhRadioFrequencyBandFDD,
    &nhOptUE_RadioAccessCapabBandFDDfddRF_Cap,
    &nhMeasurementCapExt,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_RadioAccessCapabBandFDDspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_RadioAccessCapabBandFDDTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_RadioAccessCapabBandFDD =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCapabBandFDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCapabBandFDD),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCapabBandFDDspec),
    NULLP
};
CmPAsnSetSeqOfDef nhUE_RadioAccessCapabBandFDDLstspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
    1,        /* start of the size */
    NhumaxFreqBandsFDD,        /* start of the size */
    3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef nhUE_RadioAccessCapabBandFDDLst =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCapabBandFDDLst ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQOF,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCapabBandFDDLst),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhUE_RadioAccessCapabBandFDDLstspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_RadioAccessCap_v370extTkn[] =
{
    &nhUE_RadioAccessCapabBandFDDLst,
        &nhUE_RadioAccessCapabBandFDD,
        &nh3gConstTerm,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_RadioAccessCap_v370extspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_RadioAccessCap_v370extTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_RadioAccessCap_v370ext =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCap_v370ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCap_v370ext),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCap_v370extspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUTRANUE_RadioAccessCapInfoTkn[] =
{
    &nhUE_RadioAccessCap,
    &nhUE_RadioAccessCap_v370ext,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUTRANUE_RadioAccessCapInfospec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUTRANUE_RadioAccessCapInfoTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUTRANUE_RadioAccessCapInfo =
{
#ifdef CM_PASN_DBG
    " nhUTRANUE_RadioAccessCapInfo ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANUE_RadioAccessCapInfo),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUTRANUE_RadioAccessCapInfospec),
    NULLP
};
/* Element ----- PredefinedConfigValueTag ------- */
CmPAsnUIntDef nhPredefinedConfigValueTagspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
    0,
    15UL
};
CmPAsnElmDef nhPredefinedConfigValueTag =
{
#ifdef CM_PASN_DBG
    " nhPredefinedConfigValueTag ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_UINT32,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPredefinedConfigValueTag),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPredefinedConfigValueTagspec),
    NULLP
};

PUBLIC CmPAsnElmDef *nhPredefinedConfigStatusInfootherTkn[] =
{
    &nh3gNull,
    &nhPredefinedConfigValueTag,
    &nh3gConstTerm
};
CmPAsnChoiceDef nhPredefinedConfigStatusInfootherspec = 
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_CHOICE,
#endif /* CM_PASN_DEBUG */
    2,
    0,
    &nhPredefinedConfigStatusInfootherTkn[0]  /* Token List */
};
CmPAsnElmDef nhPredefinedConfigStatusInfoother =
{
#ifdef CM_PASN_DBG
    " nhPredefinedConfigStatusInfoother ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_CHOICE,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPredefinedConfigStatusInfoother),
    &flag3gNh3M,
    NULLP,
    (U8 *)(&nhPredefinedConfigStatusInfootherspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhPredefinedConfigStatusInfoTkn[] =
{
    &nh3gNull,
    &nhPredefinedConfigStatusInfoother,
    &nh3gConstTerm
};
CmPAsnChoiceDef nhPredefinedConfigStatusInfospec = 
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_CHOICE,
#endif /* CM_PASN_DEBUG */
    2,
    0,
    &nhPredefinedConfigStatusInfoTkn[0]  /* Token List */
};
CmPAsnElmDef nhPredefinedConfigStatusInfo =
{
#ifdef CM_PASN_DBG
    " nhPredefinedConfigStatusInfo ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_CHOICE,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPredefinedConfigStatusInfo),
    &flag3gNh3M,
    NULLP,
    (U8 *)(&nhPredefinedConfigStatusInfospec),
    NULLP
};
CmPAsnSetSeqOfDef nhPredefinedConfigStatusLstspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
    NhumaxPredefConfig,        /* start of the size */
    NhumaxPredefConfig,        /* start of the size */
    3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef nhPredefinedConfigStatusLst =
{
#ifdef CM_PASN_DBG
    " nhPredefinedConfigStatusLst ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQOF,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPredefinedConfigStatusLst),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhPredefinedConfigStatusLstspec),
    NULLP
};
CmPAsnBitStrDef nhSTART_Valuespec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
    20,
    20
};
CmPAsnElmDef nhSTART_Value =
{
#ifdef CM_PASN_DBG
    " nhSTART_Value ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BITSTRXL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuSTART_Value),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhSTART_Valuespec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_SecInfoTkn[] =
{
    &nhSTART_Value,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_SecInfospec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_SecInfoTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_SecInfo =
{
#ifdef CM_PASN_DBG
    " nhUE_SecInfo ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_SecInfo),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_SecInfospec),
    NULLP
};
CmPAsnElmDef nhUE_PositioningCapExt_v380rx_tx_TimeDifferenceTyp2Capable =
{
#ifdef CM_PASN_DBG
    " nhUE_PositioningCapExt_v380rx_tx_TimeDifferenceTyp2Capable ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhUE_PositioningCapExt_v380Tkn[] =
{
    &nhUE_PositioningCapExt_v380rx_tx_TimeDifferenceTyp2Capable,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_PositioningCapExt_v380spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_PositioningCapExt_v380Tkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_PositioningCapExt_v380 =
{
#ifdef CM_PASN_DBG
    " nhUE_PositioningCapExt_v380 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_PositioningCapExt_v380),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_PositioningCapExt_v380spec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_RadioAccessCap_v380extTkn[] =
{
    &nhUE_PositioningCapExt_v380,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_RadioAccessCap_v380extspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_RadioAccessCap_v380extTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_RadioAccessCap_v380ext =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCap_v380ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCap_v380ext),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCap_v380extspec),
    NULLP
};
PUBLIC CmPAsnElmDef nhOptUE_RadioAccessCap_v380ext =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCap_v380ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCap_v380ext),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCap_v380extspec),
    NULLP
};
/* Element ----- SupportOfDedicatedPilotsForChEstimation ------- */
CmPAsnTknEnum nhSupportOfDedicatedPilotsForChEstimationLst[2] =
{
    1,
    NhuSupportOfDedicatedPilotsForChEstimationtrueEnum
};
CmPAsnEnumDef nhSupportOfDedicatedPilotsForChEstimationspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhSupportOfDedicatedPilotsForChEstimationLst,
    NULLP
};
CmPAsnElmDef nhOptSupportOfDedicatedPilotsForChEstimation =
{
#ifdef CM_PASN_DBG
    " nhSupportOfDedicatedPilotsForChEstimation ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuSupportOfDedicatedPilotsForChEstimation),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhSupportOfDedicatedPilotsForChEstimationspec),
    NULLP
};

PUBLIC CmPAsnElmDef *nhDL_PhysChCapFDD_v380extTkn[] =
{
    &nhOptSupportOfDedicatedPilotsForChEstimation,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhDL_PhysChCapFDD_v380extspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhDL_PhysChCapFDD_v380extTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhDL_PhysChCapFDD_v380ext =
{
#ifdef CM_PASN_DBG
    " nhDL_PhysChCapFDD_v380ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuDL_PhysChCapFDD_v380ext),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhDL_PhysChCapFDD_v380extspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhInterRATHovrInfo_v390ext_IEsTkn[] =
{
    &nhOptUE_RadioAccessCap_v380ext,
    &nhDL_PhysChCapFDD_v380ext,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhInterRATHovrInfo_v390ext_IEsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhInterRATHovrInfo_v390ext_IEsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhInterRATHovrInfo_v390ext_IEs =
{
#ifdef CM_PASN_DBG
    " nhInterRATHovrInfo_v390ext_IEs ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuInterRATHovrInfo_v390ext_IEs),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhInterRATHovrInfo_v390ext_IEsspec),
    NULLP
};
/* Element ----- validity-CellPCH-UraPCH ------- */
CmPAsnTknEnum nhUE_PositioningCapExt_v3a0validity_CellPCH_UraPCHLst[2] =
{
    1,
    NhuUE_PositioningCapExt_v3a0validity_CellPCH_UraPCHtrueEnum
};
CmPAsnEnumDef nhUE_PositioningCapExt_v3a0validity_CellPCH_UraPCHspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhUE_PositioningCapExt_v3a0validity_CellPCH_UraPCHLst,
    NULLP
};
CmPAsnElmDef nhUE_PositioningCapExt_v3a0validity_CellPCH_UraPCH =
{
#ifdef CM_PASN_DBG
    " nhUE_PositioningCapExt_v3a0validity_CellPCH_UraPCH ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_PositioningCapExt_v3a0validity_CellPCH_UraPCH),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_PositioningCapExt_v3a0validity_CellPCH_UraPCHspec),
    NULLP
};

PUBLIC CmPAsnElmDef *nhUE_PositioningCapExt_v3a0Tkn[] =
{
    &nhUE_PositioningCapExt_v3a0validity_CellPCH_UraPCH,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_PositioningCapExt_v3a0spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_PositioningCapExt_v3a0Tkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_PositioningCapExt_v3a0 =
{
#ifdef CM_PASN_DBG
    " nhUE_PositioningCapExt_v3a0 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_PositioningCapExt_v3a0),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_PositioningCapExt_v3a0spec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_RadioAccessCap_v3a0extTkn[] =
{
    &nhUE_PositioningCapExt_v3a0,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_RadioAccessCap_v3a0extspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_RadioAccessCap_v3a0extTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_RadioAccessCap_v3a0ext =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCap_v3a0ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCap_v3a0ext),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCap_v3a0extspec),
    NULLP
};
PUBLIC CmPAsnElmDef nhOptUE_RadioAccessCap_v3a0ext =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCap_v3a0ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCap_v3a0ext),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCap_v3a0extspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhInterRATHovrInfo_v3a0ext_IEsTkn[] =
{
    &nhOptUE_RadioAccessCap_v3a0ext,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhInterRATHovrInfo_v3a0ext_IEsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhInterRATHovrInfo_v3a0ext_IEsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhInterRATHovrInfo_v3a0ext_IEs =
{
#ifdef CM_PASN_DBG
    " nhInterRATHovrInfo_v3a0ext_IEs ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuInterRATHovrInfo_v3a0ext_IEs),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhInterRATHovrInfo_v3a0ext_IEsspec),
    NULLP
};
CmPAsnBitStrDef nhUESpecificBehaviourInfo1interRATspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
    8,
    8
};
CmPAsnElmDef nhOptUESpecificBehaviourInfo1interRAT =
{
#ifdef CM_PASN_DBG
    " nhUESpecificBehaviourInfo1interRAT ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BITSTRXL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUESpecificBehaviourInfo1interRAT),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhUESpecificBehaviourInfo1interRATspec),
    NULLP
};
CmPAsnElmDef nhUESpecificBehaviourInfo1interRAT =
{
#ifdef CM_PASN_DBG
    " nhUESpecificBehaviourInfo1interRAT ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BITSTRXL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUESpecificBehaviourInfo1interRAT),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhUESpecificBehaviourInfo1interRATspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhInterRATHovrInfo_v3d0ext_IEsTkn[] =
{
    &nhOptUESpecificBehaviourInfo1interRAT,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhInterRATHovrInfo_v3d0ext_IEsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhInterRATHovrInfo_v3d0ext_IEsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhInterRATHovrInfo_v3d0ext_IEs =
{
#ifdef CM_PASN_DBG
    " nhInterRATHovrInfo_v3d0ext_IEs ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuInterRATHovrInfo_v3d0ext_IEs),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhInterRATHovrInfo_v3d0ext_IEsspec),
    NULLP
};
/* Element ----- sfn-sfnType2Capability ------- */
CmPAsnTknEnum nhUE_PositioningCapExt_v3g0sfn_sfnTyp2CapLst[2] =
{
    1,
    NhuUE_PositioningCapExt_v3g0sfn_sfnTyp2CaptrueEnum
};
CmPAsnEnumDef nhUE_PositioningCapExt_v3g0sfn_sfnTyp2Capspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhUE_PositioningCapExt_v3g0sfn_sfnTyp2CapLst,
    NULLP
};
CmPAsnElmDef nhUE_PositioningCapExt_v3g0sfn_sfnTyp2Cap =
{
#ifdef CM_PASN_DBG
    " nhUE_PositioningCapExt_v3g0sfn_sfnTyp2Cap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_PositioningCapExt_v3g0sfn_sfnTyp2Cap),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_PositioningCapExt_v3g0sfn_sfnTyp2Capspec),
    NULLP
};

PUBLIC CmPAsnElmDef *nhUE_PositioningCapExt_v3g0Tkn[] =
{
    &nhUE_PositioningCapExt_v3g0sfn_sfnTyp2Cap,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_PositioningCapExt_v3g0spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_PositioningCapExt_v3g0Tkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_PositioningCapExt_v3g0 =
{
#ifdef CM_PASN_DBG
    " nhUE_PositioningCapExt_v3g0 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_PositioningCapExt_v3g0),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_PositioningCapExt_v3g0spec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_RadioAccessCap_v3g0extTkn[] =
{
    &nhUE_PositioningCapExt_v3g0,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_RadioAccessCap_v3g0extspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_RadioAccessCap_v3g0extTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_RadioAccessCap_v3g0ext =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCap_v3g0ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCap_v3g0ext),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCap_v3g0extspec),
    NULLP
};
PUBLIC CmPAsnElmDef nhOptUE_RadioAccessCap_v3g0ext =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCap_v3g0ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCap_v3g0ext),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCap_v3g0extspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhInterRATHovrInfo_v3g0ext_IEsTkn[] =
{
    &nhOptUE_RadioAccessCap_v3g0ext,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhInterRATHovrInfo_v3g0ext_IEsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhInterRATHovrInfo_v3g0ext_IEsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhInterRATHovrInfo_v3g0ext_IEs =
{
#ifdef CM_PASN_DBG
    " nhInterRATHovrInfo_v3g0ext_IEs ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuInterRATHovrInfo_v3g0ext_IEs),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhInterRATHovrInfo_v3g0ext_IEsspec),
    NULLP
};
/* Element ----- AccessStratumReleaseIndicator ------- */
CmPAsnTknEnum nhAccessStratumRlsIndicatorLst[17] =
{
    16,
    NhuAccessStratumRlsIndicatorrel_4Enum,
    NhuAccessStratumRlsIndicatorrel_5Enum,
    NhuAccessStratumRlsIndicatorrel_6Enum,
    NhuAccessStratumRlsIndicatorrel_7Enum,
    NhuAccessStratumRlsIndicatorspare12Enum,
    NhuAccessStratumRlsIndicatorspare11Enum,
    NhuAccessStratumRlsIndicatorspare10Enum,
    NhuAccessStratumRlsIndicatorspare9Enum,
    NhuAccessStratumRlsIndicatorspare8Enum,
    NhuAccessStratumRlsIndicatorspare7Enum,
    NhuAccessStratumRlsIndicatorspare6Enum,
    NhuAccessStratumRlsIndicatorspare5Enum,
    NhuAccessStratumRlsIndicatorspare4Enum,
    NhuAccessStratumRlsIndicatorspare3Enum,
    NhuAccessStratumRlsIndicatorspare2Enum,
    NhuAccessStratumRlsIndicatorspare1Enum
};
CmPAsnEnumDef nhAccessStratumRlsIndicatorspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhAccessStratumRlsIndicatorLst,
    NULLP
};
CmPAsnElmDef nhAccessStratumRlsIndicator =
{
#ifdef CM_PASN_DBG
    " nhAccessStratumRlsIndicator ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuAccessStratumRlsIndicator),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhAccessStratumRlsIndicatorspec),
    NULLP
};

PUBLIC CmPAsnElmDef *nhInterRATHovrInfo_v4b0ext_IEsTkn[] =
{
    &nhAccessStratumRlsIndicator,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhInterRATHovrInfo_v4b0ext_IEsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhInterRATHovrInfo_v4b0ext_IEsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhInterRATHovrInfo_v4b0ext_IEs =
{
#ifdef CM_PASN_DBG
    " nhInterRATHovrInfo_v4b0ext_IEs ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuInterRATHovrInfo_v4b0ext_IEs),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhInterRATHovrInfo_v4b0ext_IEsspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhInterRATHovrInfo_v4d0ext_IEsTkn[] =
{
    &nhOptRadioFrequencyBandTDDLst,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhInterRATHovrInfo_v4d0ext_IEsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhInterRATHovrInfo_v4d0ext_IEsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhInterRATHovrInfo_v4d0ext_IEs =
{
#ifdef CM_PASN_DBG
    " nhInterRATHovrInfo_v4d0ext_IEs ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuInterRATHovrInfo_v4d0ext_IEs),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhInterRATHovrInfo_v4d0ext_IEsspec),
    NULLP
};
CmPAsnSetSeqOfDef nhPredefinedConfigValueTagLstspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
    1,        /* start of the size */
    NhumaxPredefConfig,        /* start of the size */
    3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef nhPredefinedConfigValueTagLst =
{
#ifdef CM_PASN_DBG
    " nhPredefinedConfigValueTagLst ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQOF,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPredefinedConfigValueTagLst),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhPredefinedConfigValueTagLstspec),
    NULLP
};
/* Element ----- startPosition ------- */
CmPAsnUIntDef nhPredefinedConfigSetWithDifferentValueTagstartPositionspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
    0,
    10UL
};
CmPAsnElmDef nhPredefinedConfigSetWithDifferentValueTagstartPosition =
{
#ifdef CM_PASN_DBG
    " nhPredefinedConfigSetWithDifferentValueTagstartPosition ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_UINT32,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPredefinedConfigSetWithDifferentValueTagstartPosition),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPredefinedConfigSetWithDifferentValueTagstartPositionspec),
    NULLP
};
CmPAsnElmDef nhDefPredefinedConfigSetWithDifferentValueTagstartPosition =
{
#ifdef CM_PASN_DBG
    " nhPredefinedConfigSetWithDifferentValueTagstartPosition ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_UINT32,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPredefinedConfigSetWithDifferentValueTagstartPosition),
    &flag3gNh3D,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPredefinedConfigSetWithDifferentValueTagstartPositionspec),
    (U32 *)0
};

PUBLIC CmPAsnElmDef *nhPredefinedConfigSetWithDifferentValueTagTkn[] =
{
    &nhDefPredefinedConfigSetWithDifferentValueTagstartPosition,
    &nhPredefinedConfigValueTagLst,
        &nhPredefinedConfigValueTag,
        &nh3gConstTerm,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhPredefinedConfigSetWithDifferentValueTagspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhPredefinedConfigSetWithDifferentValueTagTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhPredefinedConfigSetWithDifferentValueTag =
{
#ifdef CM_PASN_DBG
    " nhPredefinedConfigSetWithDifferentValueTag ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPredefinedConfigSetWithDifferentValueTag),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPredefinedConfigSetWithDifferentValueTagspec),
    NULLP
};
CmPAsnSetSeqOfDef nhPredefinedConfigSetsWithDifferentValueTagspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
    1,        /* start of the size */
    2,        /* start of the size */
    3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef nhPredefinedConfigSetsWithDifferentValueTag =
{
#ifdef CM_PASN_DBG
    " nhPredefinedConfigSetsWithDifferentValueTag ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQOF,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPredefinedConfigSetsWithDifferentValueTag),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhPredefinedConfigSetsWithDifferentValueTagspec),
    NULLP
};
CmPAsnSetSeqOfDef nhPredefinedConfigStatusLstVarSzspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
    1,        /* start of the size */
    NhumaxPredefConfig,        /* start of the size */
    3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef nhOptPredefinedConfigStatusLstVarSz =
{
#ifdef CM_PASN_DBG
    " nhPredefinedConfigStatusLstVarSz ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQOF,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPredefinedConfigStatusLstVarSz),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhPredefinedConfigStatusLstVarSzspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhPredefinedConfigStatusLstCompTkn[] =
{
    &nhPredefinedConfigSetsWithDifferentValueTag,
        &nhPredefinedConfigSetWithDifferentValueTag,
        &nh3gConstTerm,
    &nhOptPredefinedConfigStatusLstVarSz,
        &nhPredefinedConfigStatusInfo,
        &nh3gConstTerm,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhPredefinedConfigStatusLstCompspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhPredefinedConfigStatusLstCompTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhPredefinedConfigStatusLstComp =
{
#ifdef CM_PASN_DBG
    " nhPredefinedConfigStatusLstComp ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPredefinedConfigStatusLstComp),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPredefinedConfigStatusLstCompspec),
    NULLP
};
PUBLIC CmPAsnElmDef nhOptPredefinedConfigStatusLstComp =
{
#ifdef CM_PASN_DBG
    " nhPredefinedConfigStatusLstComp ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPredefinedConfigStatusLstComp),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPredefinedConfigStatusLstCompspec),
    NULLP
};
/* Element ----- RF-CapabBandFDDComp ------- */
CmPAsnTknEnum nhRF_CapabBandFDDCompLst[5] =
{
    4,
    NhuRF_CapabBandFDDCompnotSuppEnum,
    NhuRF_CapabBandFDDCompdefault_TxRx_separationEnum,
    NhuRF_CapabBandFDDCompspare2Enum,
    NhuRF_CapabBandFDDCompspare1Enum
};
CmPAsnEnumDef nhRF_CapabBandFDDCompspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhRF_CapabBandFDDCompLst,
    NULLP
};
CmPAsnElmDef nhRF_CapabBandFDDComp =
{
#ifdef CM_PASN_DBG
    " nhRF_CapabBandFDDComp ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRF_CapabBandFDDComp),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRF_CapabBandFDDCompspec),
    NULLP
};

CmPAsnSetSeqOfDef nhRF_CapabBandLstFDDCompspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
    1,        /* start of the size */
    NhumaxFreqBandsFDD,        /* start of the size */
    3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef nhRF_CapabBandLstFDDComp =
{
#ifdef CM_PASN_DBG
    " nhRF_CapabBandLstFDDComp ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQOF,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRF_CapabBandLstFDDComp),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhRF_CapabBandLstFDDCompspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhRF_CapCompfddTkn[] =
{
    &nh3gNull,
    &nhRF_CapabBandLstFDDComp,
        &nhRF_CapabBandFDDComp,
        &nh3gConstTerm,
    &nh3gConstTerm
};
CmPAsnChoiceDef nhRF_CapCompfddspec = 
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_CHOICE,
#endif /* CM_PASN_DEBUG */
    2,
    0,
    &nhRF_CapCompfddTkn[0]  /* Token List */
};
CmPAsnElmDef nhRF_CapCompfdd =
{
#ifdef CM_PASN_DBG
    " nhRF_CapCompfdd ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_CHOICE,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRF_CapCompfdd),
    &flag3gNh3M,
    NULLP,
    (U8 *)(&nhRF_CapCompfddspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhRF_CapComptdd384_RF_CapTkn[] =
{
    &nh3gNull,
    &nhRadioFrequencyBandTDDLst,
    &nh3gConstTerm
};
CmPAsnChoiceDef nhRF_CapComptdd384_RF_Capspec = 
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_CHOICE,
#endif /* CM_PASN_DEBUG */
    2,
    0,
    &nhRF_CapComptdd384_RF_CapTkn[0]  /* Token List */
};
CmPAsnElmDef nhRF_CapComptdd384_RF_Cap =
{
#ifdef CM_PASN_DBG
    " nhRF_CapComptdd384_RF_Cap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_CHOICE,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRF_CapComptdd384_RF_Cap),
    &flag3gNh3M,
    NULLP,
    (U8 *)(&nhRF_CapComptdd384_RF_Capspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhRF_CapComptdd128_RF_CapTkn[] =
{
    &nh3gNull,
    &nhRadioFrequencyBandTDDLst,
    &nh3gConstTerm
};
CmPAsnChoiceDef nhRF_CapComptdd128_RF_Capspec = 
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_CHOICE,
#endif /* CM_PASN_DEBUG */
    2,
    0,
    &nhRF_CapComptdd128_RF_CapTkn[0]  /* Token List */
};
CmPAsnElmDef nhRF_CapComptdd128_RF_Cap =
{
#ifdef CM_PASN_DBG
    " nhRF_CapComptdd128_RF_Cap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_CHOICE,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRF_CapComptdd128_RF_Cap),
    &flag3gNh3M,
    NULLP,
    (U8 *)(&nhRF_CapComptdd128_RF_Capspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhRF_CapCompTkn[] =
{
    &nhRF_CapCompfdd,
    &nhRF_CapComptdd384_RF_Cap,
    &nhRF_CapComptdd128_RF_Cap,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhRF_CapCompspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhRF_CapCompTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhRF_CapComp =
{
#ifdef CM_PASN_DBG
    " nhRF_CapComp ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRF_CapComp),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRF_CapCompspec),
    NULLP
};
CmPAsnElmDef nhUE_RadioAccessCapComptotalAM_RLCMemoryExceeds10kB =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCapComptotalAM_RLCMemoryExceeds10kB ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhUE_RadioAccessCapCompTkn[] =
{
    &nhUE_RadioAccessCapComptotalAM_RLCMemoryExceeds10kB,
    &nhRF_CapComp,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_RadioAccessCapCompspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_RadioAccessCapCompTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_RadioAccessCapComp =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCapComp ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCapComp),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCapCompspec),
    NULLP
};
PUBLIC CmPAsnElmDef nhOptUE_RadioAccessCapComp =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCapComp ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCapComp),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCapCompspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhInterRATHovrInfo_v590ext_IEsTkn[] =
{
    &nhOptPredefinedConfigStatusLstComp,
    &nhOptUE_RadioAccessCapComp,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhInterRATHovrInfo_v590ext_IEsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    2,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhInterRATHovrInfo_v590ext_IEsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhInterRATHovrInfo_v590ext_IEs =
{
#ifdef CM_PASN_DBG
    " nhInterRATHovrInfo_v590ext_IEs ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuInterRATHovrInfo_v590ext_IEs),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhInterRATHovrInfo_v590ext_IEsspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_SecInfo2Tkn[] =
{
    &nhSTART_Value,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_SecInfo2spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_SecInfo2Tkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_SecInfo2 =
{
#ifdef CM_PASN_DBG
    " nhUE_SecInfo2 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_SecInfo2),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_SecInfo2spec),
    NULLP
};
PUBLIC CmPAsnElmDef nhOptUE_SecInfo2 =
{
#ifdef CM_PASN_DBG
    " nhUE_SecInfo2 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_SecInfo2),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_SecInfo2spec),
    NULLP
};
CmPAsnSetSeqOfDef nhRF_CapabBandLstFDDComp_extspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
    1,        /* start of the size */
    NhumaxFreqBandsFDD_ext,        /* start of the size */
    3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef nhRF_CapabBandLstFDDComp_ext =
{
#ifdef CM_PASN_DBG
    " nhRF_CapabBandLstFDDComp_ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQOF,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRF_CapabBandLstFDDComp_ext),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhRF_CapabBandLstFDDComp_extspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_RadioAccessCapComp_extTkn[] =
{
    &nhRF_CapabBandLstFDDComp_ext,
        &nhRF_CapabBandFDDComp,
        &nh3gConstTerm,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_RadioAccessCapComp_extspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_RadioAccessCapComp_extTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_RadioAccessCapComp_ext =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCapComp_ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCapComp_ext),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCapComp_extspec),
    NULLP
};
PUBLIC CmPAsnElmDef nhOptUE_RadioAccessCapComp_ext =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCapComp_ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCapComp_ext),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCapComp_extspec),
    NULLP
};
/* Element ----- DL-CapabilityWithSimultaneousHS-DSCHConfig ------- */
CmPAsnTknEnum nhDL_CapWithSimultaneousHS_DSCHConfigLst[5] =
{
    4,
    NhuDL_CapWithSimultaneousHS_DSCHConfigkbps32Enum,
    NhuDL_CapWithSimultaneousHS_DSCHConfigkbps64Enum,
    NhuDL_CapWithSimultaneousHS_DSCHConfigkbps128Enum,
    NhuDL_CapWithSimultaneousHS_DSCHConfigkbps384Enum
};
CmPAsnEnumDef nhDL_CapWithSimultaneousHS_DSCHConfigspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhDL_CapWithSimultaneousHS_DSCHConfigLst,
    NULLP
};
CmPAsnElmDef nhOptDL_CapWithSimultaneousHS_DSCHConfig =
{
#ifdef CM_PASN_DBG
    " nhDL_CapWithSimultaneousHS_DSCHConfig ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuDL_CapWithSimultaneousHS_DSCHConfig),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhDL_CapWithSimultaneousHS_DSCHConfigspec),
    NULLP
};

/* Element ----- HSDSCH-physical-layer-category ------- */
CmPAsnUIntDef nhHSDSCH_physical_layer_categoryspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
    1,
    64UL
};
CmPAsnElmDef nhHSDSCH_physical_layer_category =
{
#ifdef CM_PASN_DBG
    " nhHSDSCH_physical_layer_category ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_UINT32,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuHSDSCH_physical_layer_category),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhHSDSCH_physical_layer_categoryspec),
    NULLP
};

/* Element ----- edch-PhysicalLayerCategory ------- */
CmPAsnUIntDef nhPhysicalChannelCap_edch_r6fdd_edchsupportededch_PhysicalLyrCategoryspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
    1,
    16UL
};
CmPAsnElmDef nhPhysicalChannelCap_edch_r6fdd_edchsupportededch_PhysicalLyrCategory =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCap_edch_r6fdd_edchsupportededch_PhysicalLyrCategory ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_UINT32,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCap_edch_r6fdd_edchsupportededch_PhysicalLyrCategory),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPhysicalChannelCap_edch_r6fdd_edchsupportededch_PhysicalLyrCategoryspec),
    NULLP
};

PUBLIC CmPAsnElmDef *nhPhysicalChannelCap_edch_r6fdd_edchsupportedTkn[] =
{
    &nhPhysicalChannelCap_edch_r6fdd_edchsupportededch_PhysicalLyrCategory,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhPhysicalChannelCap_edch_r6fdd_edchsupportedspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhPhysicalChannelCap_edch_r6fdd_edchsupportedTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhPhysicalChannelCap_edch_r6fdd_edchsupported =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCap_edch_r6fdd_edchsupported ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCap_edch_r6fdd_edchsupported),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPhysicalChannelCap_edch_r6fdd_edchsupportedspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhPhysicalChannelCap_edch_r6fdd_edchTkn[] =
{
    &nhPhysicalChannelCap_edch_r6fdd_edchsupported,
    &nh3gNull,
    &nh3gConstTerm
};
CmPAsnChoiceDef nhPhysicalChannelCap_edch_r6fdd_edchspec = 
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_CHOICE,
#endif /* CM_PASN_DEBUG */
    2,
    0,
    &nhPhysicalChannelCap_edch_r6fdd_edchTkn[0]  /* Token List */
};
CmPAsnElmDef nhPhysicalChannelCap_edch_r6fdd_edch =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCap_edch_r6fdd_edch ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_CHOICE,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCap_edch_r6fdd_edch),
    &flag3gNh3M,
    NULLP,
    (U8 *)(&nhPhysicalChannelCap_edch_r6fdd_edchspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhPhysicalChannelCap_edch_r6Tkn[] =
{
    &nhPhysicalChannelCap_edch_r6fdd_edch,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhPhysicalChannelCap_edch_r6spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhPhysicalChannelCap_edch_r6Tkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhPhysicalChannelCap_edch_r6 =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCap_edch_r6 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCap_edch_r6),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPhysicalChannelCap_edch_r6spec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edchTkn[] =
{
    &nhOptDL_CapWithSimultaneousHS_DSCHConfig,
    &nhHSDSCH_physical_layer_category,
    &nhPhysicalChannelCap_edch_r6,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edchspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edchTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edchspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_RadioAccessCapComp2Tkn[] =
{
    &nhOptUE_RadioAccessCapComp2fddPhysicalChannelCapab_hspdsch_edch,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_RadioAccessCapComp2spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_RadioAccessCapComp2Tkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_RadioAccessCapComp2 =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCapComp2 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCapComp2),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCapComp2spec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhInterRATHovrInfo_v690ext_IEsTkn[] =
{
    &nhOptUE_SecInfo2,
    &nhOptUE_RadioAccessCapComp_ext,
    &nhUE_RadioAccessCapComp2,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhInterRATHovrInfo_v690ext_IEsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    2,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhInterRATHovrInfo_v690ext_IEsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhInterRATHovrInfo_v690ext_IEs =
{
#ifdef CM_PASN_DBG
    " nhInterRATHovrInfo_v690ext_IEs ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuInterRATHovrInfo_v690ext_IEs),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhInterRATHovrInfo_v690ext_IEsspec),
    NULLP
};
/* Element ----- supportForSIB11bis ------- */
CmPAsnTknEnum nhInterRATHovrInfo_v6b0ext_IEssupportForSIB11bisLst[2] =
{
    1,
    NhuInterRATHovrInfo_v6b0ext_IEssupportForSIB11bistrueEnum
};
CmPAsnEnumDef nhInterRATHovrInfo_v6b0ext_IEssupportForSIB11bisspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhInterRATHovrInfo_v6b0ext_IEssupportForSIB11bisLst,
    NULLP
};
CmPAsnElmDef nhOptInterRATHovrInfo_v6b0ext_IEssupportForSIB11bis =
{
#ifdef CM_PASN_DBG
    " nhInterRATHovrInfo_v6b0ext_IEssupportForSIB11bis ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuInterRATHovrInfo_v6b0ext_IEssupportForSIB11bis),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhInterRATHovrInfo_v6b0ext_IEssupportForSIB11bisspec),
    NULLP
};

PUBLIC CmPAsnElmDef *nhInterRATHovrInfo_v6b0ext_IEsTkn[] =
{
    &nhOptInterRATHovrInfo_v6b0ext_IEssupportForSIB11bis,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhInterRATHovrInfo_v6b0ext_IEsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhInterRATHovrInfo_v6b0ext_IEsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhInterRATHovrInfo_v6b0ext_IEs =
{
#ifdef CM_PASN_DBG
    " nhInterRATHovrInfo_v6b0ext_IEs ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuInterRATHovrInfo_v6b0ext_IEs),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhInterRATHovrInfo_v6b0ext_IEsspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUTRANInterRATHovrInfopredefinedConfigStatusLstTkn[] =
{
    &nh3gNull,
    &nhPredefinedConfigStatusLst,
        &nhPredefinedConfigStatusInfo,
        &nh3gConstTerm,
    &nh3gConstTerm
};
CmPAsnChoiceDef nhUTRANInterRATHovrInfopredefinedConfigStatusLstspec = 
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_CHOICE,
#endif /* CM_PASN_DEBUG */
    2,
    0,
    &nhUTRANInterRATHovrInfopredefinedConfigStatusLstTkn[0]  /* Token List */
};
CmPAsnElmDef nhUTRANInterRATHovrInfopredefinedConfigStatusLst =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfopredefinedConfigStatusLst ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_CHOICE,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfopredefinedConfigStatusLst),
    &flag3gNh3M,
    NULLP,
    (U8 *)(&nhUTRANInterRATHovrInfopredefinedConfigStatusLstspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUTRANInterRATHovrInfouE_SecInfoTkn[] =
{
    &nh3gNull,
    &nhUE_SecInfo,
    &nh3gConstTerm
};
CmPAsnChoiceDef nhUTRANInterRATHovrInfouE_SecInfospec = 
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_CHOICE,
#endif /* CM_PASN_DEBUG */
    2,
    0,
    &nhUTRANInterRATHovrInfouE_SecInfoTkn[0]  /* Token List */
};
CmPAsnElmDef nhUTRANInterRATHovrInfouE_SecInfo =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfouE_SecInfo ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_CHOICE,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfouE_SecInfo),
    &flag3gNh3M,
    NULLP,
    (U8 *)(&nhUTRANInterRATHovrInfouE_SecInfospec),
    NULLP
};
CmPAsnOctStrDef nhUTRANInterRATHovrInfoue_CapContpresentspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_OCTSTRXL,
#endif /* CM_PASN_DEBUG */
    0,
    63
};
CmPAsnElmDef nhUTRANInterRATHovrInfoue_CapContpresent =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfoue_CapContpresent " ,
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_OCTSTRXL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfoue_CapContpresent),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhUTRANInterRATHovrInfoue_CapContpresentspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUTRANInterRATHovrInfoue_CapContTkn[] =
{
    &nh3gNull,
    &nhUTRANInterRATHovrInfoue_CapContpresent,
    &nh3gConstTerm
};
CmPAsnChoiceDef nhUTRANInterRATHovrInfoue_CapContspec = 
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_CHOICE,
#endif /* CM_PASN_DEBUG */
    2,
    0,
    &nhUTRANInterRATHovrInfoue_CapContTkn[0]  /* Token List */
};
CmPAsnElmDef nhUTRANInterRATHovrInfoue_CapCont =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfoue_CapCont ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_CHOICE,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfoue_CapCont),
    &flag3gNh3M,
    NULLP,
    (U8 *)(&nhUTRANInterRATHovrInfoue_CapContspec),
    NULLP
};
CmPAsnBitStrDef nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsinterRATHovrInfo_r3_add_extspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
    1,
    255
};
CmPAsnElmDef nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsinterRATHovrInfo_r3_add_ext =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsinterRATHovrInfo_r3_add_ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BITSTRXL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsinterRATHovrInfo_r3_add_ext),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsinterRATHovrInfo_r3_add_extspec),
    NULLP
};
CmPAsnElmDef nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsinterRATHovrInfo_r3_add_ext =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsinterRATHovrInfo_r3_add_ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BITSTRXL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsinterRATHovrInfo_r3_add_ext),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsinterRATHovrInfo_r3_add_extspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtnsTkn[] =
{
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtnsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtnsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtnsspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsTkn[] =
{
    &nhInterRATHovrInfo_v6b0ext_IEs,
    &nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsnonCriticalExtns,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtnsspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsTkn[] =
{
    &nhInterRATHovrInfo_v690ext_IEs,
    &nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsv6b0NonCriticalExtns,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtnsspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsTkn[] =
{
    &nhInterRATHovrInfo_v590ext_IEs,
    &nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsv690NonCriticalExtns,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtnsspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsTkn[] =
{
    &nhInterRATHovrInfo_v4d0ext_IEs,
    &nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsv590NonCriticalExtns,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtnsspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsTkn[] =
{
    &nhInterRATHovrInfo_v4b0ext_IEs,
    &nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsv4d0NonCriticalExtns,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtnsspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsTkn[] =
{
    &nhInterRATHovrInfo_v3g0ext_IEs,
    &nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsv4b0NonCriticalExtns,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtnsspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsTkn[] =
{
    &nhInterRATHovrInfo_v3d0ext_IEs,
    &nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsinterRATHovrInfo_r3_add_ext,
    &nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsv3g0NonCriticalExtns,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    2,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtnsspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnsTkn[] =
{
    &nhInterRATHovrInfo_v3a0ext_IEs,
    &nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnslaterNonCriticalExtns,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtnsspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUTRANInterRATHovrInfov390NonCriticalExtnspresentTkn[] =
{
    &nhInterRATHovrInfo_v390ext_IEs,
    &nhOptUTRANInterRATHovrInfov390NonCriticalExtnspresentv3a0NonCriticalExtns,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUTRANInterRATHovrInfov390NonCriticalExtnspresentspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUTRANInterRATHovrInfov390NonCriticalExtnspresentTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUTRANInterRATHovrInfov390NonCriticalExtnspresent =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfov390NonCriticalExtnspresent ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfov390NonCriticalExtnspresent),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUTRANInterRATHovrInfov390NonCriticalExtnspresentspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUTRANInterRATHovrInfov390NonCriticalExtnsTkn[] =
{
    &nh3gNull,
    &nhUTRANInterRATHovrInfov390NonCriticalExtnspresent,
    &nh3gConstTerm
};
CmPAsnChoiceDef nhUTRANInterRATHovrInfov390NonCriticalExtnsspec = 
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_CHOICE,
#endif /* CM_PASN_DEBUG */
    2,
    0,
    &nhUTRANInterRATHovrInfov390NonCriticalExtnsTkn[0]  /* Token List */
};
CmPAsnElmDef nhUTRANInterRATHovrInfov390NonCriticalExtns =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfov390NonCriticalExtns ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_CHOICE,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfov390NonCriticalExtns),
    &flag3gNh3M,
    NULLP,
    (U8 *)(&nhUTRANInterRATHovrInfov390NonCriticalExtnsspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUTRANInterRATHovrInfoTkn[] =
{
    &nhUTRANInterRATHovrInfopredefinedConfigStatusLst,
    &nhUTRANInterRATHovrInfouE_SecInfo,
    &nhUTRANInterRATHovrInfoue_CapCont,
    &nhUTRANInterRATHovrInfov390NonCriticalExtns,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUTRANInterRATHovrInfospec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUTRANInterRATHovrInfoTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUTRANInterRATHovrInfo =
{
#ifdef CM_PASN_DBG
    " nhUTRANInterRATHovrInfo ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUTRANInterRATHovrInfo),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUTRANInterRATHovrInfospec),
    NULLP
};
/* Element ----- RadioFrequencyBandFDD2 ------- */
CmPAsnTknEnum nhRadioFrequencyBandFDD2Lst[17] =
{
    16,
    NhuRadioFrequencyBandFDD2bandVIIIEnum,
    NhuRadioFrequencyBandFDD2bandIXEnum,
    NhuRadioFrequencyBandFDD2bandXEnum,
    NhuRadioFrequencyBandFDD2bandXIEnum,
    NhuRadioFrequencyBandFDD2bandXIIEnum,
    NhuRadioFrequencyBandFDD2bandXIIIEnum,
    NhuRadioFrequencyBandFDD2bandXIVEnum,
    NhuRadioFrequencyBandFDD2bandXVEnum,
    NhuRadioFrequencyBandFDD2bandXVIEnum,
    NhuRadioFrequencyBandFDD2bandXVIIEnum,
    NhuRadioFrequencyBandFDD2bandXVIIIEnum,
    NhuRadioFrequencyBandFDD2bandXIXEnum,
    NhuRadioFrequencyBandFDD2bandXXEnum,
    NhuRadioFrequencyBandFDD2bandXXIEnum,
    NhuRadioFrequencyBandFDD2bandXXIIEnum,
    NhuRadioFrequencyBandFDD2extension_indicatorEnum
};
CmPAsnEnumDef nhRadioFrequencyBandFDD2spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhRadioFrequencyBandFDD2Lst,
    NULLP
};
CmPAsnElmDef nhOptRadioFrequencyBandFDD2 =
{
#ifdef CM_PASN_DBG
    " nhRadioFrequencyBandFDD2 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRadioFrequencyBandFDD2),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRadioFrequencyBandFDD2spec),
    NULLP
};
CmPAsnElmDef nhRadioFrequencyBandFDD2 =
{
#ifdef CM_PASN_DBG
    " nhRadioFrequencyBandFDD2 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRadioFrequencyBandFDD2),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRadioFrequencyBandFDD2spec),
    NULLP
};

CmPAsnElmDef nhCompressedModeMeasCapabFDD2dl_MeasurementsFDD =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabFDD2dl_MeasurementsFDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

CmPAsnElmDef nhCompressedModeMeasCapabFDD2ul_MeasurementsFDD =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabFDD2ul_MeasurementsFDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhCompressedModeMeasCapabFDD2Tkn[] =
{
    &nhOptRadioFrequencyBandFDD,
    &nhOptRadioFrequencyBandFDD2,
    &nhCompressedModeMeasCapabFDD2dl_MeasurementsFDD,
    &nhCompressedModeMeasCapabFDD2ul_MeasurementsFDD,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhCompressedModeMeasCapabFDD2spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    2,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhCompressedModeMeasCapabFDD2Tkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhCompressedModeMeasCapabFDD2 =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabFDD2 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuCompressedModeMeasCapabFDD2),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhCompressedModeMeasCapabFDD2spec),
    NULLP
};
CmPAsnSetSeqOfDef nhCompressedModeMeasCapabFDDLst2spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
    1,        /* start of the size */
    NhumaxFreqBandsFDD,        /* start of the size */
    3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef nhCompressedModeMeasCapabFDDLst2 =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabFDDLst2 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQOF,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuCompressedModeMeasCapabFDDLst2),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhCompressedModeMeasCapabFDDLst2spec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhMeasurementCapExt2Tkn[] =
{
    &nhCompressedModeMeasCapabFDDLst2,
        &nhCompressedModeMeasCapabFDD2,
        &nh3gConstTerm,
    &nhOptCompressedModeMeasCapabTDDLst,
        &nhCompressedModeMeasCapabTDD,
        &nh3gConstTerm,
    &nhOptCompressedModeMeasCapabGSMLst,
        &nhCompressedModeMeasCapabGSM,
        &nh3gConstTerm,
    &nhOptCompressedModeMeasCapabMC,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhMeasurementCapExt2spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    3,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhMeasurementCapExt2Tkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhMeasurementCapExt2 =
{
#ifdef CM_PASN_DBG
    " nhMeasurementCapExt2 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMeasurementCapExt2),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMeasurementCapExt2spec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_RadioAccessCapabBandFDD2fddRF_CapTkn[] =
{
    &nhUE_PowerClassExt,
    &nhTxRxFrequencySeparation,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_RadioAccessCapabBandFDD2fddRF_Capspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_RadioAccessCapabBandFDD2fddRF_CapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptUE_RadioAccessCapabBandFDD2fddRF_Cap =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCapabBandFDD2fddRF_Cap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCapabBandFDD2fddRF_Cap),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCapabBandFDD2fddRF_Capspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_RadioAccessCapabBandFDD2Tkn[] =
{
    &nhRadioFrequencyBandFDD2,
    &nhOptUE_RadioAccessCapabBandFDD2fddRF_Cap,
    &nhMeasurementCapExt2,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_RadioAccessCapabBandFDD2spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_RadioAccessCapabBandFDD2Tkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_RadioAccessCapabBandFDD2 =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCapabBandFDD2 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCapabBandFDD2),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCapabBandFDD2spec),
    NULLP
};
CmPAsnSetSeqOfDef nhUE_RadioAccessCapabBandFDDLst2spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
    1,        /* start of the size */
    NhumaxFreqBandsFDD,        /* start of the size */
    3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef nhUE_RadioAccessCapabBandFDDLst2 =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCapabBandFDDLst2 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQOF,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCapabBandFDDLst2),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhUE_RadioAccessCapabBandFDDLst2spec),
    NULLP
};
CmPAsnElmDef nhCompressedModeMeasCapabFDD_extdl_MeasurementsFDD =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabFDD_extdl_MeasurementsFDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

CmPAsnElmDef nhCompressedModeMeasCapabFDD_extul_MeasurementsFDD =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabFDD_extul_MeasurementsFDD ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhCompressedModeMeasCapabFDD_extTkn[] =
{
    &nhRadioFrequencyBandFDD2,
    &nhCompressedModeMeasCapabFDD_extdl_MeasurementsFDD,
    &nhCompressedModeMeasCapabFDD_extul_MeasurementsFDD,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhCompressedModeMeasCapabFDD_extspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhCompressedModeMeasCapabFDD_extTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhCompressedModeMeasCapabFDD_ext =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabFDD_ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuCompressedModeMeasCapabFDD_ext),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhCompressedModeMeasCapabFDD_extspec),
    NULLP
};
CmPAsnSetSeqOfDef nhCompressedModeMeasCapabFDDLst_extspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
    1,        /* start of the size */
    NhumaxFreqBandsFDD,        /* start of the size */
    3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef nhCompressedModeMeasCapabFDDLst_ext =
{
#ifdef CM_PASN_DBG
    " nhCompressedModeMeasCapabFDDLst_ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQOF,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuCompressedModeMeasCapabFDDLst_ext),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhCompressedModeMeasCapabFDDLst_extspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_RadioAccessCapabBandFDD_extTkn[] =
{
    &nhRadioFrequencyBandFDD,
    &nhCompressedModeMeasCapabFDDLst_ext,
        &nhCompressedModeMeasCapabFDD_ext,
        &nh3gConstTerm,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_RadioAccessCapabBandFDD_extspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_RadioAccessCapabBandFDD_extTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_RadioAccessCapabBandFDD_ext =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCapabBandFDD_ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCapabBandFDD_ext),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCapabBandFDD_extspec),
    NULLP
};
CmPAsnSetSeqOfDef nhUE_RadioAccessCapabBandFDDLst_extspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
    1,        /* start of the size */
    NhumaxFreqBandsFDD,        /* start of the size */
    3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef nhOptUE_RadioAccessCapabBandFDDLst_ext =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCapabBandFDDLst_ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQOF,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCapabBandFDDLst_ext),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *) (&nhUE_RadioAccessCapabBandFDDLst_extspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_RadioAccessCap_v650extTkn[] =
{
    &nhUE_RadioAccessCapabBandFDDLst2,
        &nhUE_RadioAccessCapabBandFDD2,
        &nh3gConstTerm,
    &nhOptUE_RadioAccessCapabBandFDDLst_ext,
        &nhUE_RadioAccessCapabBandFDD_ext,
        &nh3gConstTerm,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_RadioAccessCap_v650extspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_RadioAccessCap_v650extTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_RadioAccessCap_v650ext =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCap_v650ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCap_v650ext),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCap_v650extspec),
    NULLP
};
PUBLIC CmPAsnElmDef nhOptUE_RadioAccessCap_v650ext =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCap_v650ext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCap_v650ext),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCap_v650extspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhInterRATHovrInfo_v690ext1_IEsTkn[] =
{
    &nhOptUE_RadioAccessCap_v650ext,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhInterRATHovrInfo_v690ext1_IEsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhInterRATHovrInfo_v690ext1_IEsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhInterRATHovrInfo_v690ext1_IEs =
{
#ifdef CM_PASN_DBG
    " nhInterRATHovrInfo_v690ext1_IEs ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuInterRATHovrInfo_v690ext1_IEs),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhInterRATHovrInfo_v690ext1_IEsspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtnsTkn[] =
{
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtnsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtnsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns =
{
#ifdef CM_PASN_DBG
    " nhInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtnsspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhInterRATHovrInfo_r3_add_ext_IEsTkn[] =
{
    &nhInterRATHovrInfo_v690ext1_IEs,
    &nhOptInterRATHovrInfo_r3_add_ext_IEsnonCriticalExtns,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhInterRATHovrInfo_r3_add_ext_IEsspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    1,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhInterRATHovrInfo_r3_add_ext_IEsTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhInterRATHovrInfo_r3_add_ext_IEs =
{
#ifdef CM_PASN_DBG
    " nhInterRATHovrInfo_r3_add_ext_IEs ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuInterRATHovrInfo_r3_add_ext_IEs),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhInterRATHovrInfo_r3_add_ext_IEsspec),
    NULLP
};
/* Element ----- RadioFrequencyBandTDDList-r7 ------- */
CmPAsnTknEnum nhRadioFrequencyBandTDDLst_r7Lst[17] =
{
    16,
    NhuRadioFrequencyBandTDDLst_r7aEnum,
    NhuRadioFrequencyBandTDDLst_r7bEnum,
    NhuRadioFrequencyBandTDDLst_r7cEnum,
    NhuRadioFrequencyBandTDDLst_r7dEnum,
    NhuRadioFrequencyBandTDDLst_r7abEnum,
    NhuRadioFrequencyBandTDDLst_r7acEnum,
    NhuRadioFrequencyBandTDDLst_r7adEnum,
    NhuRadioFrequencyBandTDDLst_r7bcEnum,
    NhuRadioFrequencyBandTDDLst_r7bdEnum,
    NhuRadioFrequencyBandTDDLst_r7cdEnum,
    NhuRadioFrequencyBandTDDLst_r7abcEnum,
    NhuRadioFrequencyBandTDDLst_r7abdEnum,
    NhuRadioFrequencyBandTDDLst_r7acdEnum,
    NhuRadioFrequencyBandTDDLst_r7bcdEnum,
    NhuRadioFrequencyBandTDDLst_r7abcdEnum,
    NhuRadioFrequencyBandTDDLst_r7spareEnum
};
CmPAsnEnumDef nhRadioFrequencyBandTDDLst_r7spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhRadioFrequencyBandTDDLst_r7Lst,
    NULLP
};
CmPAsnElmDef nhRadioFrequencyBandTDDLst_r7 =
{
#ifdef CM_PASN_DBG
    " nhRadioFrequencyBandTDDLst_r7 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRadioFrequencyBandTDDLst_r7),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRadioFrequencyBandTDDLst_r7spec),
    NULLP
};

/* Element ----- ChipRateCapability-r7 ------- */
CmPAsnTknEnum nhChipRateCap_r7Lst[4] =
{
    3,
    NhuChipRateCap_r7mcps3_84Enum,
    NhuChipRateCap_r7mcps7_68Enum,
    NhuChipRateCap_r7mcps1_28Enum
};
CmPAsnEnumDef nhChipRateCap_r7spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhChipRateCap_r7Lst,
    NULLP
};
CmPAsnElmDef nhChipRateCap_r7 =
{
#ifdef CM_PASN_DBG
    " nhChipRateCap_r7 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuChipRateCap_r7),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhChipRateCap_r7spec),
    NULLP
};

PUBLIC CmPAsnElmDef *nhRF_Cap_r7fddRF_CapTkn[] =
{
    &nhUE_PowerClass,
    &nhTxRxFrequencySeparation,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhRF_Cap_r7fddRF_Capspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhRF_Cap_r7fddRF_CapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptRF_Cap_r7fddRF_Cap =
{
#ifdef CM_PASN_DBG
    " nhRF_Cap_r7fddRF_Cap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRF_Cap_r7fddRF_Cap),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRF_Cap_r7fddRF_Capspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhRF_Cap_r7tddRF_CapTkn[] =
{
    &nhUE_PowerClass,
    &nhRadioFrequencyBandTDDLst_r7,
    &nhChipRateCap_r7,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhRF_Cap_r7tddRF_Capspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhRF_Cap_r7tddRF_CapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptRF_Cap_r7tddRF_Cap =
{
#ifdef CM_PASN_DBG
    " nhRF_Cap_r7tddRF_Cap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRF_Cap_r7tddRF_Cap),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRF_Cap_r7tddRF_Capspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhRF_Cap_r7Tkn[] =
{
    &nhOptRF_Cap_r7fddRF_Cap,
    &nhOptRF_Cap_r7tddRF_Cap,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhRF_Cap_r7spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    2,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhRF_Cap_r7Tkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhRF_Cap_r7 =
{
#ifdef CM_PASN_DBG
    " nhRF_Cap_r7 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRF_Cap_r7),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRF_Cap_r7spec),
    NULLP
};
PUBLIC CmPAsnElmDef nhOptRF_Cap_r7 =
{
#ifdef CM_PASN_DBG
    " nhRF_Cap_r7 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuRF_Cap_r7),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhRF_Cap_r7spec),
    NULLP
};
/* Element ----- MaxPhysChPerFrame-768 ------- */
CmPAsnUIntDef nhMaxPhysChPerFrame_768spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
    1,
    448UL
};
CmPAsnElmDef nhMaxPhysChPerFrame_768 =
{
#ifdef CM_PASN_DBG
    " nhMaxPhysChPerFrame_768 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_UINT32,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxPhysChPerFrame_768),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxPhysChPerFrame_768spec),
    NULLP
};

/* Element ----- MinimumSF-DL-768 ------- */
CmPAsnTknEnum nhMinimumSF_DL_768Lst[3] =
{
    2,
    NhuMinimumSF_DL_768sf1Enum,
    NhuMinimumSF_DL_768sf32Enum
};
CmPAsnEnumDef nhMinimumSF_DL_768spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
    nhMinimumSF_DL_768Lst,
    NULLP
};
CmPAsnElmDef nhMinimumSF_DL_768 =
{
#ifdef CM_PASN_DBG
    " nhMinimumSF_DL_768 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_ENUM,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMinimumSF_DL_768),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMinimumSF_DL_768spec),
    NULLP
};

/* Element ----- MaxPhysChPerTS-768 ------- */
CmPAsnUIntDef nhMaxPhysChPerTS_768spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
    1,
    32UL
};
CmPAsnElmDef nhMaxPhysChPerTS_768 =
{
#ifdef CM_PASN_DBG
    " nhMaxPhysChPerTS_768 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_UINT32,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuMaxPhysChPerTS_768),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhMaxPhysChPerTS_768spec),
    NULLP
};

CmPAsnElmDef nhDL_PhysChCapTDD_768supportOfPDSCH =
{
#ifdef CM_PASN_DBG
    " nhDL_PhysChCapTDD_768supportOfPDSCH ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_BOOL,
    CM_PASN_EXT_MKR_ABS,
    sizeof(TknU8),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    NULLP,
    (U8 *) NULLP
};

PUBLIC CmPAsnElmDef *nhDL_PhysChCapTDD_768Tkn[] =
{
    &nhMaxTS_PerFrame,
    &nhMaxPhysChPerFrame_768,
    &nhMinimumSF_DL_768,
    &nhDL_PhysChCapTDD_768supportOfPDSCH,
    &nhMaxPhysChPerTS_768,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhDL_PhysChCapTDD_768spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhDL_PhysChCapTDD_768Tkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhDL_PhysChCapTDD_768 =
{
#ifdef CM_PASN_DBG
    " nhDL_PhysChCapTDD_768 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuDL_PhysChCapTDD_768),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhDL_PhysChCapTDD_768spec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhPhysicalChannelCap_r7fddPhysChCapTkn[] =
{
    &nhDL_PhysChCapFDD,
    &nhUL_PhysChCapFDD,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhPhysicalChannelCap_r7fddPhysChCapspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhPhysicalChannelCap_r7fddPhysChCapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptPhysicalChannelCap_r7fddPhysChCap =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCap_r7fddPhysChCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCap_r7fddPhysChCap),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPhysicalChannelCap_r7fddPhysChCapspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhPhysicalChannelCap_r7tddPhysChCapTkn[] =
{
    &nhDL_PhysChCapTDD,
    &nhUL_PhysChCapTDD,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhPhysicalChannelCap_r7tddPhysChCapspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhPhysicalChannelCap_r7tddPhysChCapTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhOptPhysicalChannelCap_r7tddPhysChCap =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCap_r7tddPhysChCap ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCap_r7tddPhysChCap),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPhysicalChannelCap_r7tddPhysChCapspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhPhysicalChannelCap_r7tddPhysChCap_768Tkn[] =
{
    &nhDL_PhysChCapTDD_768,
    &nhUL_PhysChCapTDD,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhPhysicalChannelCap_r7tddPhysChCap_768spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhPhysicalChannelCap_r7tddPhysChCap_768Tkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhPhysicalChannelCap_r7tddPhysChCap_768 =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCap_r7tddPhysChCap_768 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCap_r7tddPhysChCap_768),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPhysicalChannelCap_r7tddPhysChCap_768spec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhPhysicalChannelCap_r7Tkn[] =
{
    &nhOptPhysicalChannelCap_r7fddPhysChCap,
    &nhOptPhysicalChannelCap_r7tddPhysChCap,
    &nhPhysicalChannelCap_r7tddPhysChCap_768,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhPhysicalChannelCap_r7spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    2,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhPhysicalChannelCap_r7Tkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhPhysicalChannelCap_r7 =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCap_r7 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCap_r7),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPhysicalChannelCap_r7spec),
    NULLP
};
PUBLIC CmPAsnElmDef nhOptPhysicalChannelCap_r7 =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCap_r7 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCap_r7),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPhysicalChannelCap_r7spec),
    NULLP
};
/* Element ----- tdd-edch-PhysicalLayerCategory ------- */
CmPAsnUIntDef nhPhysicalChannelCap_tddedch_r7tdd_edchsupportedtdd_edch_PhysicalLyrCategoryspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
    1,
    16UL
};
CmPAsnElmDef nhPhysicalChannelCap_tddedch_r7tdd_edchsupportedtdd_edch_PhysicalLyrCategory =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCap_tddedch_r7tdd_edchsupportedtdd_edch_PhysicalLyrCategory ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_UINT32,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCap_tddedch_r7tdd_edchsupportedtdd_edch_PhysicalLyrCategory),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPhysicalChannelCap_tddedch_r7tdd_edchsupportedtdd_edch_PhysicalLyrCategoryspec),
    NULLP
};

PUBLIC CmPAsnElmDef *nhPhysicalChannelCap_tddedch_r7tdd_edchsupportedTkn[] =
{
    &nhPhysicalChannelCap_tddedch_r7tdd_edchsupportedtdd_edch_PhysicalLyrCategory,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhPhysicalChannelCap_tddedch_r7tdd_edchsupportedspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhPhysicalChannelCap_tddedch_r7tdd_edchsupportedTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhPhysicalChannelCap_tddedch_r7tdd_edchsupported =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCap_tddedch_r7tdd_edchsupported ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCap_tddedch_r7tdd_edchsupported),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPhysicalChannelCap_tddedch_r7tdd_edchsupportedspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhPhysicalChannelCap_tddedch_r7tdd_edchTkn[] =
{
    &nhPhysicalChannelCap_tddedch_r7tdd_edchsupported,
    &nh3gNull,
    &nh3gConstTerm
};
CmPAsnChoiceDef nhPhysicalChannelCap_tddedch_r7tdd_edchspec = 
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_CHOICE,
#endif /* CM_PASN_DBG */
    2,
    0,
    &nhPhysicalChannelCap_tddedch_r7tdd_edchTkn[0]  /* Token List */
};
CmPAsnElmDef nhPhysicalChannelCap_tddedch_r7tdd_edch =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCap_tddedch_r7tdd_edch ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_CHOICE,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCap_tddedch_r7tdd_edch),
    &flag3gNh3M,
    NULLP,
    (U8 *)(&nhPhysicalChannelCap_tddedch_r7tdd_edchspec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhPhysicalChannelCap_tddedch_r7Tkn[] =
{
    &nhPhysicalChannelCap_tddedch_r7tdd_edch,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhPhysicalChannelCap_tddedch_r7spec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    0,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhPhysicalChannelCap_tddedch_r7Tkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhPhysicalChannelCap_tddedch_r7 =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCap_tddedch_r7 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCap_tddedch_r7),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPhysicalChannelCap_tddedch_r7spec),
    NULLP
};
PUBLIC CmPAsnElmDef nhOptPhysicalChannelCap_tddedch_r7 =
{
#ifdef CM_PASN_DBG
    " nhPhysicalChannelCap_tddedch_r7 ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuPhysicalChannelCap_tddedch_r7),
    &flag3gNh3O,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhPhysicalChannelCap_tddedch_r7spec),
    NULLP
};
PUBLIC CmPAsnElmDef *nhUE_RadioAccessCap_v7xyextTkn[] =
{
    &nhOptRF_Cap_r7,
    &nhOptPhysicalChannelCap_r7,
    &nhOptPhysicalChannelCap_tddedch_r7,
    &nh3gConstTerm
};
CmPAsnSetSeqDef nhUE_RadioAccessCap_v7xyextspec =
{
#ifdef CM_PASN_DBG
    CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
    3,      /* number of optional elements */
    0,      /* number of extension elements */
    NULLP, /* not supported yet */
    NULLP, /* not supported yet */
    &nhUE_RadioAccessCap_v7xyextTkn[0], /* ptr to first token*/
    FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef nhUE_RadioAccessCap_v7xyext =
{
#ifdef CM_PASN_DBG
    " nhUE_RadioAccessCap_v7xyext ",
#endif /* CM_PASN_DEBUG */
    CM_PASN_TET_SEQ,
    CM_PASN_EXT_MKR_ABS,
    sizeof(NhuUE_RadioAccessCap_v7xyext),
    &flag3gNh3M,
    CM_PASN_ESC_FUNC_NULLP,
    (U8 *)(&nhUE_RadioAccessCap_v7xyextspec),
    NULLP
};


PUBLIC CmPAsnElmDef *nhUTRANInterRATHovrInfo_Pdu[] =
{
         &nhUTRANInterRATHovrInfo,
         NULLP
};

PUBLIC CmPAsnElmDef *nhUTRANUE_RadioAccessCapInfo_Pdu[] =
{
         &nhUTRANUE_RadioAccessCapInfo,
         NULLP
};

/**********************************************************************
         End of file:     ni_3gdb.c@@/main/2 - Fri Jul  1 01:13:12 2011
**********************************************************************/
/**********************************************************************

        Revision history:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      ch                 1. RRC Initial Release.
/main/1      ---      ch                 1. RRC Release 2.1.
/main/2      ---      sbalakrishna       1. Updated for Release of 3.1
*********************************************************************91*/
