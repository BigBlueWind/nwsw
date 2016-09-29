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

     Desc:     Hash defines 

     File:     ve.h

     Sid:      ve.h@@/main/2 - Wed Dec  1 08:44:20 2010

     Prg:      rbhat

*********************************************************************21*/


#ifndef __VEH__
#define __VEH__

#define VE_NUM_TQENTRY 100
#define VE_MAX_SAPS 10
#define VE_MEM_REGION 0
#define VE_MEM_POOL 0
/* ReEst */
#define INVALID_CAUSE                  0
#define ICS_OTHER_FAIL_CSFALBK_SET     1
#define ERAB_REQ_RECON_FAIL            2
#define ERAB_REQ_FAIL_ANY_CAUSE        3
#define ERAB_REL_RECON_FAIL            4
#define ERAB_REL_FAIL_ANY_CAUSE        5
#define RLF_CAUSE                      6


#if EU_DAT_APP

#define VE_EGUAC_DFLT_REORDER              FALSE
#define VE_EGUAC_DFLT_ERRCAUSE             1
#define VE_EGUAC_DFLT_DIAGINFO             0
#define VE_EGUAC_DFLT_IETYPE               0
#define VE_EGU_DFLT_INTFTYPE             0
#define VE_EGU_DFLT_EXPSEQNMB            1000
/* Changing the buffer size to accumalate huge buffers */
#define VE_EGU_DFLT_MAXTPDUSZ            5000

/* state of the tunnel */
#define VE_EU_INITIAL            0
#define VE_EU_CONNECTED          1

#ifdef LTE_HO_SUPPORT
#define VE_EU_START_DATBUF       2

/* indication of which packets to forward */
#define VE_EU_NO_FWD             3
#define VE_EU_FWD_NEW_PKTS       4
#define VE_EU_FWD_OLD_PKTS       5

/* to indicate the reason for changing state of a tunnel */
#define VE_EU_END_MARKER_RECVD   6
#define VE_EU_UE_FOUND_AT_CELL   7

/* to indicate which tunnels to delete on reception of END MARKER */
#define VE_EU_DEL_TUNNELS_NONE       1
#define VE_EU_DEL_FWD_TUNNELS    2
#define VE_EU_DEL_ALL_TUNNELS    3

#define VE_MAX_UE_TUNNELS        (8 * 3) /* 8 - max DRB for UE
                                          * 3 - 1 - actual tunnel
                                          *     2 - forwarding tunnel to receive DL pkts 
                                          *     3 - forwarding tunnel to receive UL pkts 
                                          */

/* lower layer configuration status */
#define VE_LOW_LYR_CFG_NOT_STRTD  0
#define VE_LOW_LYR_CFG_IN_PRGS    1
#define VE_LOW_LYR_CFG_COMPL      2
#define VE_LOW_LYR_CFG_REM        3

/* Number of packets that can be buffered during buffering state */
#define VE_EU_PKT_LIMIT           (1024 * 1024)
#endif

#define VE_EGTP_TUN_CREATE       1
#define VE_EGTP_TUN_DEL          2
#define VE_EGTP_FWD_TUN_CREATE   3

#define VE_EG_UTIL_MEM_SIZE                 1024

#define VE_S1AP_CON_NA        0
#define VE_S1AP_CONNECTING    1
#define VE_S1AP_CONNECTED     2

#endif

/* Added DL and UL Sn length */
#define VE_RLC_UM_DL_DFLT_SN_LEN 10
#define VE_RLC_UM_UL_DFLT_SN_LEN 10

#ifdef VE_RELAY



/* Corrected eNodeB layer name */
#define VELAYERNAME "EnodeB App"
#define VE_TMR_NHU_SAP_BND 1
#define VE_TMR_RGR_SAP_BND 2
#define VE_TMR_CTF_SAP_BND 3
#define VE_TMR_SZT_SAP_BND 4
#ifdef LTE_HO_SUPPORT
#define VE_TMR_CZT_SAP_BND 5
#endif
#define VE_TMR_EGT_SAP_BND 6
#define VE_TMR_PJU_SAP_BND 7
#define VE_TMR_INACTIVITY  8
#ifdef LTE_HO_SUPPORT
#define VE_TMR_S1_RELOC_TMR 10
#define VE_TMR_S1_OVRL_TMR 11
#define VE_TMR_UE_HO_ATTACH_TMR 12
#define VE_TMR_X2_RESET_TMR     13  /* X2-Reset Retranmission timer */
#define VE_TMR_X2_SETUP_TMR     14  /* X2-Setup Retranmission timer */
/* X2_CFG_UPD_CHANGES*/
#define VE_TMR_X2_CFGUPD_TMR     21  /* Configuration update Retranmission timer */

#define VE_MAX_X2_CFGUPD_RTX     5   /* Maximum Config UPdate Request Retries */
#define VE_X2_CFGUPD_TMR_VAL     500 /* eNodeb cofig update  Retransmission timer value */

#define VE_MAX_X2_RESET_RTX     5   /* Maximum Reset Retries */
#define VE_X2_RESET_TMR_VAL     500 /* X2 Reset Retransmission timer value */ 

#define VE_MAX_X2_SETUP_RTX     5   /* Maximum SetupRequest Retries */
#define VE_X2_SETUP_TMR_VAL     500 /* X2 Setup Retransmission timer value */ 

#define S1AP_CAUSE_PROTOCOL         4
#define S1AP_CAUSE_MISC             5
#endif

#define VE_TMR_S1SETUP_TMR 15
#define VE_ENB_CFG_UPD_TMR 16
#define VE_S1_RESET_TMR 17
#define VE_TMR_ENB_OVERLOAD_TMR 18
/* RRC CON timer*/
#define VE_TMR_RRC_CON     8
#define VE_TMR_INITUE_CON  9
#define VE_MAX_TMR_PER_UE  1
#define VE_INITTMR_VAL     10000

#ifdef VE_TEST_CODE
#define VE_TMR_TEST_ENB_UPD  19
#define VE_TMR_TEST_ENB_RST  20
#endif /* VE_TEST_CODE */
#define VE_TMR_RES            1
#define VE_APP_TQSIZE        64


#define VE_DFLT_EGTP_PORT 2152

#define VE_INIT_STATE   0

#define VE_VAL_ZERO 0
#define VE_VAL_ONE 1
#define VE_VAL_TWO 2
#define VE_VAL_THREE 3
#define VE_VAL_FOURE 4
#define VE_VAL_FIVE  5
#define VE_SI_CNT  4
/* RRC States */
#define VE_RRC_INIT        0
#define VE_RRC_CONNECTING  1  
#define VE_RRC_CONNECTED   2
#define VE_SRB2_ESTABLISHED   3
/* RRC-ReEstab */
#define VE_SRB2_DRB_SUSPENDED 4
/* CR ID- ccpu00116764- IOT changes */
#define VE_RRC_RELEASING  4

#ifdef LTE_HO_SUPPORT

/* Maximum number of targets that can be prepared,
   at a time for a UE */
#define VE_MAX_TARGET_PREP 3

#define VE_MAX_NO_OF_PLMNS 6

#define VE_MAX_X2AP_CON 10


/* Encode Request TransIds */
#define VE_ENC_TRANSID_HO_REQ      11
#define VE_ENC_TRANSID_HO_CMD  12
#define VE_DEC_TRANSID_HO_CMD      13
#define VE_ENC_TRANSID_HO_DL_DCCH_MSG 14

#define VE_TRANSID_GET_PDCP_SDUSTA  13
#define VE_TRANSID_UE_CFG           14
#define VE_TRANSID_SN_CFG           15

#endif

/* UE related info */
#define VE_PDCP_ID_SRB1 1
#define VE_PDCP_ID_SRB2 2
#define VE_PDCP_ID_DRB1 3
#define VE_PDCP_ID_DRB2 4
#define VE_PDCP_ID_DRB3 5

/* DRB State */
#define VE_DRB_NOT_IN_USE 0
#define VE_DRB_CONNECTING 1
#define VE_DRB_CONNECTED 2
#define VE_DRB_DELETING  3

/* Cell related info */
#define VE_CELL_CFG_DFLT_RBS 6

#define VE_MCC_SIZE 3
/* CR ID- ccpu00116764-   IOT changes*/
/* This eNUM VE_MNC_SIZE has not been used anymore ID:ccpu00116398*/
#define VE_CELL_ID_LEN 28
#define VE_RNTI_LEN 16

#define VE_BYTE_LEN      8
#define VE_MIB_SPARE_LEN 10
#define VE_SIB1_SUBFRM_NO 5 
#define VE_TRAC_LEN 16
/* System Info Message Types */
#define VE_SYS_MIB 0
#define VE_SYS_SIB1 1
#define VE_SYS_SIB2 2
#ifdef SI_NEW
#define VE_SYS_SIB3 3
#define VE_SYS_SIB4 4
#define VE_SYS_SIB5 5 
#endif

#define VE_MAX_SFN 1024
#define VE_SFN_INCREMENT  4
#define VE_MIB_PRESENT    0x1
#define VE_MIB_PERIOD     4
#define VE_MIB_SUBFRAME   0
#define VE_SIB1_PERIOD    2
#define VE_SIB1_SUBFRAME  5
#define VE_SIB1_PRESENT   0x2
#define VE_SIB2_PERIOD    16
#define VE_SIB2_SUBFRAME  0
#define VE_SIB2_PRESENT   0x4
#ifdef SI_NEW
#define VE_SIB3_PERIOD    32 
#define VE_SIB4_PERIOD    64
#define VE_SIB5_PERIOD    64
#endif


#define VE_MAX_TUNNEL_ID  200
#define VE_MAX_PDCP_ID    300
#define VE_MAX_RABS_IN_UE 8
#define VE_MEM_PDU_SIZE  4096

#define VE_MAX_SRBS      2

#define VE_RGR_SRB_CFG_LCH   0x1
#define VE_RGR_SRB_CFG_LCG   0x2
#define VE_RGR_DRB_CFG_LCH   0x4
#define VE_RGR_DRB_CFG_LCG   0x8

/* Security related info */
#define VE_INTG_KEY_LEN 16
#define VE_CIPH_KEY_LEN 16

#define VE_STRING_S_LEN 7

#define VE_CP_CIPH_KEY 0
#define VE_UP_CIPH_KEY 1
#define VE_INTG_KEY    2

#define VE_S1_SETUP_DONE 1
#define VE_S1_CONNECTING 2
#define VE_S1_CONNECTED  3
#define VE_MAX_S1AP_CON 100

#define VE_MAX_CELL 1
/*X2_CFG_UPD_CHANGES */
#define VE_MAX_GROUP 1

/* Updated Maxiumum UEs */
#ifndef VE_PERF_MEAS
#define VE_MAX_UE   (veCb.cellCb[0]->veDfltMacRntisize + veCb.cellCb[0]->rntiDb.maxRrmRntis)
#else
#define VE_MAX_UE   1
#endif


/* Duplex Mode */
#define VE_MODE_FDD 1
#define VE_MODE_TDD 0

/* antz - Changed names to reflect use, and using for both RLC and MAC */
#define VE_BCCH_BCH_ID    1
#define VE_BCCH_SIB1_ID   2
#define VE_BCCH_SIB_OTHERS_ID   3
#define VE_CCCH_DL_ID   4
#define VE_CCCH_UL_ID   5
#define VE_PCCH_ID      6

#ifdef WIRESHARK_LOG /* Madhur Including for wireshark logging */
#define VE_MAC_LTE_START_STRING        "mac-lte"
#define VE_MAC_LTE_RNTI_TAG            0x02
#define VE_MAC_LTE_UEID_TAG            0x03
#define VE_MAC_LTE_SUBFRAME_TAG        0x04
#define VE_MAC_LTE_PREDFINED_DATA_TAG  0x05
#define VE_MAC_LTE_RETX_TAG            0x06
#define VE_MAC_LTE_CRC_STATUS_TAG      0x07
#define VE_MAC_LTE_PAYLOAD_TAG         0x01

static int                     g_sockfd, g_sockfd1;
static struct sockaddr_in      g_serv_addr, g_serv_addr1;
#endif


#define VE_NH_MAX_MSG_TYPE   11
#define VE_NH_MAX_MSG_CLASS  5

#define SZ_MAX_MSG_ID 63
#define SZ_MEM_SDU_SIZE     2048
#define VE_CZ_MAX_MSG_ID 25
#define VE_SZ_MEM_SDU_SIZE     2048
#define VE_CZ_MEM_SDU_SIZE     2048


#define VE_INIT_CNTXT_SETUP_REQ  VE_VAL_ZERO
#define VE_ERAB_SETUP_REQ             VE_VAL_ONE
#define VE_ERAB_MODIFY_REQ           VE_VAL_TWO
#define VE_ERAB_RLS_COMM               VE_VAL_THREE

#ifdef LTE_HO_SUPPORT
#define VE_HO_CONFIRM                VE_VAL_FOURE
#endif

#define VE_UE_CNTXT_MOD_REQ     VE_VAL_FIVE
#define SRC_ENODEB 1
#define TGT_ENODEB 2

#define VE_PRNT_BUF veCb.init.prntBuf
#define DBGMASK_SM  1
#define VE_EVEBASE     0
#define VE_ERRVE       (VE_EVEBASE + 0)  /* reserved */
#define VE_EVE001      (VE_ERRVE +    1) /* ve_sm_hi_exms.c: 241 */

#ifdef VE_PICO
#define VE_CTF_MINOR_RECFG 0
#define VE_CTF_MAJOR_RECFG 1
#endif


#define VE_MAX_PLMNS_PER_MME      6
#define VE_MAX_GRPS_PER_MME       6
#define VE_MAX_CODES_PER_MME      6
#define VE_MAX_MMES               5
#define VE_DFLT_CFG_UPD_TMR_VAL   60
#define VE_MAX_ENB_CFG_UPD_RTX    5

#define VE_S1AP_ENB_ID     1
#define VE_S1AP_MME_ID     2

/* EnodeB Overload Flag Indication */
#define  VE_OVLDNORMAL  0
#define  VE_MINOR       1
#define  VE_MAJOR       2
#define  VE_CRITICAL    3

/*************************************************************************
* Debug print                                                            *
*************************************************************************/
/* Defined VEDBGP for VE_SM_LOG_TO_FILE enabled */
#if (VE_SM_LOG_TO_FILE && DEBUGP)
#define VEDBGP(_msgClass, _args)                               \
do                                                             \
{                                                              \
   if(smCb.init.dbgMask)                                       \
   {                                                           \
      if((smCb.dbgFp == NULLP) ||                              \
            (smCb.nmbDbgLines == smCb.cfgDbgLines))            \
      {                                                        \
         veSmCreateDbgFile();                                  \
      }                                                        \
      sprintf(SM_PRNT_BUF, "[%s 0x%x:%x] %s:%d ",              \
            SMLAYERNAME,smCb.init.ent, smCb.init.inst,         \
            __FILE__, __LINE__);                               \
      fwrite((SM_PRNT_BUF),cmStrlen((U8*)SM_PRNT_BUF), 1,      \
            (smCb.dbgFp));                                     \
      sprintf _args;                                           \
      fwrite((SM_PRNT_BUF),                                    \
            cmStrlen((U8*)SM_PRNT_BUF), 1,                     \
            (smCb.dbgFp));                                     \
      smCb.nmbDbgLines++;                                      \
   }                                                           \
}                                                              \
while(0)

#else /* (VE_SM_LOG_TO_FILE && DEBUGP) */
#define VEDBGP(_msgClass, _arg) \
{\
   DBGP(&smCb.init, "EnodeB App: ", _msgClass, _arg); \
}
#endif /* (VE_SM_LOG_TO_FILE && DEBUGP) */

/* Defined VE_LYR_DBGP for VE_SM_LOG_TO_FILE enabled */
#if (VE_SM_LOG_TO_FILE && DEBUGP)
#define VE_LYR_DBGP(_msgClass, _arg)                           \
do                                                             \
{                                                              \
   if(veCb.init.dbgMask)                                       \
   {                                                           \
      if((veCb.dbgFp == NULLP) ||                              \
            (veCb.nmbDbgLines == veCb.cfgDbgLines))            \
      {                                                        \
         veUtlCreateDbgFile();                                 \
      }                                                        \
      sprintf(VE_PRNT_BUF, "[%s 0x%x:%x] %s:%d ",              \
            VELAYERNAME,veCb.init.ent, veCb.init.inst,         \
            __FILE__, __LINE__);                               \
      fwrite((VE_PRNT_BUF),                                    \
            cmStrlen((U8*)VE_PRNT_BUF), 1,                     \
            (veCb.dbgFp));                                     \
      sprintf _arg;                                            \
      fwrite((VE_PRNT_BUF),                                    \
            cmStrlen((U8*)VE_PRNT_BUF), 1,                     \
            (veCb.dbgFp));                                     \
      veCb.nmbDbgLines++;                                      \
   }                                                           \
}                                                              \
while(0)
#else /* (VE_SM_LOG_TO_FILE && DEBUGP) */
#define VE_LYR_DBGP(_msgClass, _arg) \
{\
   DBGP(&veCb.init, "EnodeB App: ", _msgClass, _arg); \
}
#endif /* (VE_SM_LOG_TO_FILE && DEBUGP) */


/* 
 * Message building related macros 
 * */

/* Extract the bits from SFN */
#define VE_GET_SFN_MIB(_var, _sfnInt)  \
{\
   U8 _byte = 0;    \
   /* antz - corrected the shifting */ \
   _byte = (U8)((_sfnInt & 0x0003FC) >> 2);    \
   _var = _byte; \
}

/**  
 * @details Macro for SGetMsg.
 */
#define VE_ALLOCMBUF(_region, _pool, _buf)            \
do                                                    \
{                                                     \
   if (SGetMsg((_region), (_pool),                    \
                        (Buffer **)(_buf)) != ROK)    \
   {                                                  \
      (*(_buf)) = NULLP;                              \
   }                                                  \
}                                                     \
while(0)

/**  
 * @details Macro for SPutMsg.
 */
#define VE_FREEMBUF(_mBuf)                            \
do{                                                     \
   if (_mBuf != NULLP)                                               \
   {                                                  \
      (Void)SPutMsg((_mBuf));                            \
      (_mBuf) = NULLP;                                   \
   }                                                  \
}while(0)

/* initialize the memCp & allocate memory for the event structure */
#define VE_ALLOCEVNT(_evntPtr, _size)\
{\
   (*_evntPtr) = NULLP; \
   if(cmAllocEvnt((Size) _size, VE_MEM_PDU_SIZE, &veCb.mem,   \
                     (Ptr *)(_evntPtr)) != ROK)  \
   {                                            \
      RETVALUE(RFAILED);                         \
   }                         \
}

/* allocate memory for members in the event structure */
#define VE_GET_MEM(_memPtr, _size, _allocPtr)\
{\
   (*_allocPtr) = NULLP;    \
   if(cmGetMem((U8 *)_memPtr, _size, (Ptr *)_allocPtr) != ROK)    \
   {                                                              \
       RETVALUE(RFAILED);                                         \
   }                                                              \
}
/* free allocated memory for an event structure */
#define VE_FREE_EVNT(_evntPtr)  \
{   \
   cmFreeMem((_evntPtr));   \
   (_evntPtr) = NULLP;  \
}
/* macro to fill TknUInt */ 
#define VE_FILL_TKN_UINT(_ptr, _val) {\
   _ptr.pres = PRSNT_NODEF;\
   _ptr.val = _val;\
}
/* macro to fill String Token Contents */
/* macro to fill TknStrOSXL */ 
#define VE_FILL_TKN_STR_UINT(_ptr, _val, _len) {\
   _ptr->pres = PRSNT_NODEF;\
   _ptr->val = _val;\
   _ptr->len = _len;\
}

#define VE_GET_S1AP_CON_ID(_suConId) {\
   for(_suConId=0;_suConId<VE_MAX_S1AP_CON;_suConId++)\
   if(veCb.s1apConLst[_suConId] == NULLP)\
   break;\
}

#define VE_MEM_COPY(_dst, _src, _len) \
   cmMemcpy((U8*) (_dst), (U8*) (_src), _len);


#define VE_FREEEVNT(_evntPtr)       \
{                                      \
   cmFreeMem((_evntPtr));              \
   (_evntPtr) = NULLP;                 \
}

#define VE_DBG_ERROR(_args) \
{ \
   DBGP(&(veCb.init), VELAYERNAME, VE_ERROR, _args); \
}

/* NAS NON Delivery Indication start */
/* get the offset of a field in a structure */
#define VE_GET_OFFSET(_type, _member)                           \
      ((PTR)(&(((_type *)0)->_member)))
#endif
/* NAS NON Delivery Indication end */

#define VE_ERROR 1
/* NAS NON Delivery Indication */
#define NUM_PDU_BINS 16

#define VE_ALLOC(_buf, _size)         \
{\
   if (SGetSBuf(VE_MEM_REGION, VE_MEM_POOL, (Data **)(_buf), _size) == ROK)\
   {                                                  \
      VE_SET_ZERO((*_buf), _size);                    \
   }                                                  \
   else                                               \
   {                                                  \
      (*_buf) = NULLP;                                \
   }                                                  \
}

#define VE_SET_ZERO(_buf, _size) \
{\
   cmMemset((U8 *)(_buf), 0, _size);\
}


#define VE_FREE(_buf, _size)          \
{\
   (Void) SPutSBuf(VE_MEM_REGION, VE_MEM_POOL, (Data *) _buf, (Size) _size); \
   (_buf) = NULLP;\
}


#define VE_SET_RGR_TRANS(_arr, _cfgTyp, _lcId, _crnti)          \
{\
   U16 _tempByte = 0;\
   _arr[0] = (U8) _cfgTyp;\
   _arr[1] = (U8) _lcId;\
   _tempByte = (_crnti & 0xff);\
   _arr[2] = (U8) _tempByte;\
   _tempByte = (_crnti >> 8);\
   _arr[3] = (U8) _tempByte;\
}

#define VE_GET_VALS_RGR_TRANS(_arr, _cfgTyp, _lcId, _crnti)          \
{\
   U16 _tempByte = 0;\
   _cfgTyp = _arr[0];\
   _lcId = _arr[1];\
   _tempByte = _arr[3];\
   _tempByte = ((_tempByte << 8) | _arr[2]);\
   _crnti = _tempByte;\
}



#define VE_GET_U32_FRM_OSXL(_var, _tkn) \
{ \
   U16 _len = 0;\
   U16 _idx = 0;\
   U32 _finalVal = 0;\
   _len = _tkn.len;\
   if(_len > 4)\
   _len = 4;\
   for(_idx = 0; _idx < _len; _idx++) \
   {\
      _finalVal = ((_finalVal << 8) | (_tkn.val[_idx])); \
   }\
   _var = _finalVal; \
}

#define VE_GET_LOC_TUNNEL_ID(_idx)\
do{\
   _idx = (++(_idx)) % VE_MAX_TUNNEL_ID; \
   if ((_idx) == 0) ++(_idx); \
}while(0);

#define VE_GET_UE_IDX(_ueId) (_ueId - veCb.cellCb[0]->veDfltStartRnti)

#define VE_GET_RB_ID_FRM_ERAB(_erabId) (_erabId - 2)

#define VE_SWAP_BYTES_U32(num)\
{                               \
   num = ((num>>24)&0xff) |      \
        ((num<<8)&0xff0000) |   \
        ((num>>8)&0xff00) |     \
        ((num<<24)&0xff000000); \
}

#define VE_SWAP_BYTES_U16(num)\
{                              \
   num = (num>>8) | (num<<8);  \
}


#ifdef LTE_HO_SUPPORT

#define VE_GET_UE_X2_IDX(_ueX2apId) (_ueX2apId % VE_MAX_X2AP_CON)
#define VE_CALC_COUNT(_snLen, _sn, _txHfn, _count)         \
{                                                          \
   _count = _sn;                                           \
   _txHfn = _txHfn << _snLen;                               \
   _count |= _txHfn;                                       \
} 

#endif

#define VE_LOGERROR(errCls, errCode, errVal, errDesc) \
do \
{ \
   { \
    SLogError(smCb.init.ent, smCb.init.inst, smCb.init.procId,     \
          /* Fixed warnings for g++ compilation on Linux */ \
            __FILE__, __LINE__, errCls, errCode, (ErrVal)errVal, \
            errDesc); \
          /* Fixed warnings for g++ compilation on Linux */ \
   } \
}while(0)

#define VE_GET_UECB(_ueCb ,_crnti)\
{\
   U8 _idx = 0; \
   _ueCb = NULLP;\
   for(_idx = 0; _idx < VE_MAX_UE; _idx++)\
   {\
      if((veCb.cellCb[0])->ueCbLst[_idx] != NULLP)\
      {\
         if((veCb.cellCb[0])->ueCbLst[_idx]->crnti == _crnti)\
         {\
            _ueCb = (veCb.cellCb[0])->ueCbLst[_idx];\
               break;\
         }\
      }\
      else\
         _ueCb = NULLP;\
   }\
}



/* X2_CFG_UPD_CHANGE*/
#define VE_GET_CELLCB(_cellCb, _cellId) \
{       \
   U32 idx = 0; \
   _cellCb = NULLP;     \
   for(idx = 0; idx < VE_MAX_CELL; idx++) {     \
      if (veCb.cellCb[idx] != NULLP) {  \
         if (veCb.cellCb[idx]->cellId == _cellId) {     \
            _cellCb = veCb.cellCb[idx]; \
         }      \
      } \
   }    \
}



/* U8 send_buf[500]; ccpu00115603 fix */
/* ccpu00118847 MOD: bit string filling at MSB */                                                                          
#define VE_FILL_BIT_STR(_ptr, _len, _val, _pdu) {\
   U16     _tmpLen;                              \
   _ptr.pres = PRSNT_NODEF;                     \
   _ptr.len =  _len;                            \
   \
   _tmpLen = _len;   \
   if((_len%8) > 0)         \
   {            \
      _tmpLen = (_len/8)+1;   \
   }   \
   else         \
   {            \
      _tmpLen  =  _len/8; \
   }   \
   VE_GET_MEM(_pdu, _tmpLen  * sizeof(U8), (Ptr *)&(_ptr.val));    \
   if(_ptr.val != NULLP)         \
   {            \
      *_ptr.val  =  _val;           \
      *_ptr.val    =   (*_ptr.val)  <<    (_tmpLen*8 - _len);  \
   }   \
}
/* TIC_ID :ccpu00118847 MOD_END: bit string filling at MSB */                                                                          
/*  CR ID- ccpu00116764- IOT changes*/
 /* typecasting warning resolved */
#define VE_FILL_BIT_STR_VAL(_ptr, _len, _val, _pdu) {\
   /* Fixed warning for gcc compilation on CentOS*/  \
   S16      i; \
   /* Fixed warning for gcc compilation on CentOS*/  \
   U16     _tmpLen;                             \
   U32    _val2= _val; \
   _ptr.pres = PRSNT_NODEF;                     \
   _ptr.len =  _len;                            \
   _tmpLen = _len;                              \
   if((_len%8) > 0)                              \
   {                                             \
      _tmpLen = (_len/8)+1;                         \
   }                                             \
   else                                          \
   {                                             \
      _tmpLen = _len/8;                             \
   }                                             \
   VE_GET_MEM(_pdu, _tmpLen * sizeof(U8), (Ptr *)&(_ptr.val));  \
                      \
   if(_ptr.val != NULLP)                         \
   {                                             \
      for(i = (S8)(_tmpLen - 1); i >= 0; i--)                  \
      {                                          \
         _ptr.val[i] = (U8)(0x000000ff & _val2);                  \
         _val2 = _val2>>8;\
      }                                          \
   }                                             \
}



#define VE_GET_PAGING_CYCLE(enmValue, T) {\
   if (enmValue == 0) T = 32;\
   else if (enmValue == 1) T = 64;\
   else if (enmValue == 2) T = 128;\
   else T = 256;\
}

#define VE_GET_NB_VALUE(enmValue, T, value) {\
   if (enmValue == 0) value = T * 4;\
   else if (enmValue == 1) value = T * 2;\
   else if (enmValue == 2) value = T * 1;\
   else if (enmValue == 3) value = (T / 2);\
   else if (enmValue == 4) value = (T / 4);\
   else if (enmValue == 5) value = (T / 8);\
   else if (enmValue == 6) value = (T / 16);\
}

#define VE_GET_TIME_TO_WAIT_VAL(_rcvdVal, _tmrVal)     \
{                                                      \
   switch(_rcvdVal)                                    \
   {                                                   \
      /* 1Sec */                                       \
      case 0:                                          \
         _tmrVal = (1 * SS_TICKS_SEC);                 \
         break;                                        \
      /* 2Sec */                                       \
      case 1:                                          \
         _tmrVal = (2 * SS_TICKS_SEC);                 \
         break;                                        \
      /* 5Sec */                                       \
      case 2:                                          \
         _tmrVal = (5 * SS_TICKS_SEC);                 \
         break;                                        \
      /* 10Sec */                                      \
      case 3:                                          \
         _tmrVal = (10 * SS_TICKS_SEC);                \
         break;                                        \
      /* 20Sec */                                      \
      case 4:                                          \
         _tmrVal = (20 * SS_TICKS_SEC);                \
         break;                                        \
      /* 60Sec */                                      \
      case 5:                                          \
         _tmrVal = (60 * SS_TICKS_SEC);                \
         break;                                        \
      default:                                         \
         break;                                        \
   }                                                   \
}

#define VE_GET_MME_CB(_peerId, _mmeCb)                     \
{                                                          \
   U16       _idx;                                         \
   VeMmeCb   *tmpCb;                                       \
   for(_idx = 0; _idx < VE_MAX_MMES; _idx++)               \
   {                                                       \
      tmpCb = veCb.mmeCont.mmes[_idx];                     \
      if((tmpCb != NULLP) && (tmpCb->mmeId == _peerId))    \
      {                                                    \
         _mmeCb = tmpCb;                                   \
         break;                                            \
      }                                                    \
   }                                                       \
}

#define VE_GET_NEXT_MME_CB(_mmeCb)                           \
{                                                            \
   if(veCb.mmeCont.numMmes < VE_MAX_MMES)                   \
   {                                                         \
      _mmeCb = veCb.mmeCont.mmes[veCb.mmeCont.numMmes++];   \
   }                                                         \
}                                                            \
/* NAS NON Delivery Indication start */
#define VE_ALLOC_TRANS_ID(nhuHdr, _crnti, _msgType, _modifier){\
   U32 _TransId;                                               \
   _TransId = (U32) _crnti;                                    \
   ++_modifier;                                                \
   _TransId = ((_TransId << 8) | _msgType);                    \
   _TransId = ((_TransId << 8) | _modifier);                   \
   nhuHdr.transId = _TransId;                                  \
}
/* NAS NON Delivery Indication end */

/* E-RAB Modify */
#define VE_CMPTL_RABMDFY_UE_INFO(ueCb) {                                              \
   U8 rbIdx;                                                                          \
   U8 idx = 0;                                                                            \
   for(rbIdx = 0; rbIdx < VE_MAX_RABS_IN_UE; rbIdx++)                                 \
   {                                                                                  \
      if(ueCb->rbInfo.rab[rbIdx].rabMdfyInfo.rabInfoModified)                         \
      {                                                                               \
         ueCb->rbInfo.rab[rbIdx].qci     = ueCb->rbInfo.rab[rbIdx].rabMdfyInfo.qci;   \
         ueCb->rbInfo.rab[rbIdx].mbr.dl  = ueCb->rbInfo.rab[rbIdx].rabMdfyInfo.mbr.dl;\
         ueCb->rbInfo.rab[rbIdx].mbr.ul  = ueCb->rbInfo.rab[rbIdx].rabMdfyInfo.mbr.ul;\
         ueCb->rbInfo.rab[rbIdx].gbr.dl  = ueCb->rbInfo.rab[rbIdx].rabMdfyInfo.gbr.dl;\
         ueCb->rbInfo.rab[rbIdx].gbr.ul  = ueCb->rbInfo.rab[rbIdx].rabMdfyInfo.gbr.ul;\
         ueCb->rbInfo.rab[rbIdx].rabMdfyInfo.rabInfoModified = FALSE;                 \
         ueCb->rbInfo.rabLst[idx++] = rbIdx;                                          \
      }                                                                               \
   }                                                                                  \
   ueCb->rbInfo.aggBr.dl = ueCb->rbInfo.tempAggBr.dl;                                 \
   ueCb->rbInfo.aggBr.ul = ueCb->rbInfo.tempAggBr.ul;                                 \
}


#define VE_FIND_QCI_TYPE(_qci, _gbr) {                           \
   _gbr = 0;                                                    \
   if ( _qci < 5)                                               \
   {                                                           \
      _gbr = 1;                                                 \
   }                                                           \
   else if(_qci < 10)                                           \
   {                                                           \
      _gbr = 2;                                                 \
   }                                                           \
   else                                                        \
   {                                                           \
      VE_DBG_ERROR((VE_PRNT_BUF,"%s:QCI Received is invalid, neither GBR nor NON-GBR\n"\
               ,__FUNCTION__));\
      _gbr = 0;                                                 \
   }                                                           \
}


/* E-RAB Modify End */
#endif /* __VEH__ */


/********************************************************************30**

         End of file:     ve.h@@/main/2 - Wed Dec  1 08:44:20 2010

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
