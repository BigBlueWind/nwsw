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

    Name:   LTE MAC Convergence layer

    Type:   C include file

    Desc:   Defines required by LTE MAC Convergence Layer for MSPD PHY

    File:   ys_ms.h

    Sid:

    Prg:    hs

**********************************************************************/

#ifndef __YS_MS_H__
#define __YS_MS_H__

#define YS_TTI_START_MAX_DELAY 0.25
#define YS_MAX_DAT_REQ_DELAY   0.75
#define YS_TTI                   1

#define YS_SI_RNTI        0xffff
#define YS_P_RNTI         0xfffe

#define YS_MS_TTI_DIFF(_cur, _prev) ((((_cur - _prev) + (0xffffffff + 1)) % (0xffffffff + 1))/ 150000.0)

#ifdef TTI_PROC_FIRST
/* This is the maximum RXSDU_IND count from the PHY per subframe. 
 * As per the discussion with MSPD we should receive only one 
 * RXSDU_IND(PUCCH or PUSCH) per UE in a particular subframe. 
 * And this should be changed based on the number of UEs.*/ 
#define MAX_UL_PHY_MSG_PER_TTI  16
#endif

#ifdef YS_PHY_STOP_AUTO
/*Can be changed as required  '0' when this flag is disabled*/
#define YS_MS_SF_COUNT     0  
#else
/*Do not change as required  '0' when this flag is disabled*/
#define YS_MS_SF_COUNT     0 
#endif

/*ys004.102 :  Merged MSPD code with phy 1.7 */

#define YS_FREE_SDU(_sdu) \
{\
   if (_sdu != NULLP) \
   {\
      cmFreeMem ((Ptr)(_sdu));\
      _sdu = NULLP;\
   }\
}

/* added , phy instance id should always be 0*/
#define YS_PHY_INST_ID             0
/*#define YS_PHY_CELL_ID             1*/
#define YS_MAX_CELL_HASH_BINS      2

#define YS_MS_ACCESS_MODE_OFDMA     0
#define YS_MS_ACCESS_MODE_SCFDMA    1
#define YS_MS_FAST_FW_PRE_LOCK_NUM  1600

#define YS_MS_GET_CH_BW(_cfg)        \
((_cfg.dlBw == CTF_BW_RB_6) ?  0 :   \
 (_cfg.dlBw == CTF_BW_RB_15) ? 1 :   \
 (_cfg.dlBw == CTF_BW_RB_25) ? 2 :   \
 (_cfg.dlBw == CTF_BW_RB_50) ? 3 :   \
 (_cfg.dlBw == CTF_BW_RB_75) ? 4 : 5 )

#define YS_MS_GET_DUP_MODE(_cfg) \
((_cfg.duplexMode == CTF_DUPMODE_FDD) ? 0 : 1)

#define YS_MS_GET_RSRC_BW(_cfg)      \
((_cfg.scSpacing == CTF_SC_SPACING_15KHZ) ? 12 : 24)

#define YS_MS_GET_NUM_TX_ANT(_cfg)         \
((_cfg.antPortsCnt == CTF_AP_CNT_1) ? 1 :    \
 (_cfg.antPortsCnt == CTF_AP_CNT_2) ? 2 : 4)

/* As per the specification 36.101
   Table F.5.3-1
*/
#define YS_MS_GET_FFT_SIZE(_cfg)        \
((_cfg.dlBw == CTF_BW_RB_6)  ? 128  :   \
 (_cfg.dlBw == CTF_BW_RB_15) ? 256  :   \
 (_cfg.dlBw == CTF_BW_RB_25) ? 512  :   \
 (_cfg.dlBw == CTF_BW_RB_50) ? 1024 :   \
 (_cfg.dlBw == CTF_BW_RB_75) ? 1536 : 2048 )

#define YS_MS_GET_DL_BW(_cfg)         \
((_cfg.dlBw == CTF_BW_RB_6) ?  6 :    \
 (_cfg.dlBw == CTF_BW_RB_15) ? 15 :   \
 (_cfg.dlBw == CTF_BW_RB_25) ? 25 :   \
 (_cfg.dlBw == CTF_BW_RB_50) ? 50 :   \
 (_cfg.dlBw == CTF_BW_RB_75) ? 75 : 100 )

#define YS_MS_GET_UL_BW(_cfg)         \
((_cfg.ulBw == CTF_BW_RB_6) ?  6 :    \
 (_cfg.ulBw == CTF_BW_RB_15) ? 15 :   \
 (_cfg.ulBw == CTF_BW_RB_25) ? 25 :   \
 (_cfg.ulBw == CTF_BW_RB_50) ? 50 :   \
 (_cfg.ulBw == CTF_BW_RB_75) ? 75 : 100 )

#define YS_MS_GET_REF_SIG_POW(_cfg) (_cfg.refSigPwr + 30)

#define YS_MS_GET_RE_PER_PRB(_cfg)            \
((_cfg.antPortsCnt == CTF_AP_CNT_1) ? 80 :    \
 (_cfg.antPortsCnt == CTF_AP_CNT_2) ? 76 : 72)

#define YS_MS_GET_CP_TYPE(_cfg)      \
((_cfg.cycPfx == CTF_CP_NORMAL) ? 0 : 1)

#define YS_RACH_REPORT_MODE_INDV   0
#define YS_RACH_REPORT_MODE_GLOBAL 1

#define YS_TX_SDU_CFM_DIS          0
#define YS_TX_SDU_CFM_ENB          1

#define YS_SDU_CFG_NO_SHARED_MEM   0
#define YS_SDU_CFG_PTR             1

#define YS_PHY_INIT_SFN            0
#define YS_PHY_INIT_SF             0
#define YS_PHY_INIT_SLOT           0

#define YS_NUM_PHY_CELLS           3
/* This value is CGI , can be 20bit length for MACRO eNB and 28Bit length 
 * for HOME eNB 
 * CCPU eNodeB is of type MACRO, so limiting this to 2 power 20 
 */
#define YS_NUM_LOG_CELLS           1048576

#define YS_NUM_SFN                 1024
#define YS_NUM_SUB_FRAMES          10
#define YS_TOTL_SUB_FRAMES_IN_SFN  10240


#define YS_MEM_SDU_SIZE            512

#define YS_NUM_PRACH_FDD_CFG       64
#define YS_NUM_PRACH_PRES_ARR      20

#define YS_NUM_SR_CFG              157

#define YS_NUM_SRS_CFG              637

#define YS_SR_PERIOD_5             5
#define YS_SR_PERIOD_10            10
#define YS_SR_PERIOD_20            20
#define YS_SR_PERIOD_40            40
#define YS_SR_PERIOD_80            80

#define YS_SRS_PERIOD_2             2
#define YS_SRS_PERIOD_5             5
#define YS_SRS_PERIOD_10            10
#define YS_SRS_PERIOD_20            20
#define YS_SRS_PERIOD_40            40
#define YS_SRS_PERIOD_80            80
#define YS_SRS_PERIOD_160           160
#define YS_SRS_PERIOD_320           320

#define YS_NUM_CQI_CFG              541

#define YS_CQI_PERIOD_1             1
#define YS_CQI_PERIOD_5             5
#define YS_CQI_PERIOD_10            10
#define YS_CQI_PERIOD_20            20
#define YS_CQI_PERIOD_40            40
#define YS_CQI_PERIOD_80            80
#define YS_CQI_PERIOD_160           160
#define YS_CQI_PERIOD_32            32
#define YS_CQI_PERIOD_64            64
#define YS_CQI_PERIOD_128           128

#define YS_PREAM_FRMT_NA           -1
#define YS_PREAM_FRMT_0            0
#define YS_PREAM_FRMT_1            1
#define YS_PREAM_FRMT_2            2
#define YS_PREAM_FRMT_3            3
#define YS_PREAM_FRMT_4            4

#define YS_MS_MAX_STATES           3
#define YS_MS_MAX_EVNT             6

#define YS_MS_INV_EVENT            0
#define YS_MS_EVENT_CFG            1
#define YS_MS_EVENT_CFG_RSP        2
#define YS_MS_EVENT_START          4
#define YS_MS_EVENT_START_RSP      3
#define YS_MS_EVENT_STOP           5
#define YS_MS_EVENT_STOP_RSP       6

#define YS_ENB_UL_SCH              5
#define YS_ENB_RACH                6
#define YS_ENB_UCI                 7

#define YS_BCH_RNTI                0xffff

#define YS_NUM_OF_RA_RNTIS         1
#define YS_NUM_OF_TBS              1
#define YS_NUM_OF_PREAMBLES        1
#define YS_NUM_OF_RACH_OCCASIONS   1


#define YS_NUM_UES                 100

#define YS_NUM_MODULATION          8

#define YS_REPEAT_CYCLE      1
#define YS_PERSIST_ENABLE    0

#define ACKNACKREPT          0

#define NOMPDSCHRSEPROFF     0

#define YS_CODING_DESC       TURBOCDR
#define YS_BLK_CODE_CAT      1
#define YS_BLK_CODE_CAT_DIS  0
#define YS_CRC_SCRAMBLING    0
#define YS_SINGLE_MAPPING    1
#define YS_TX_PWR_CNTRL      0
#define YS_HALF_ITERATIONS  16

/* PUCCH Definitions */
#define YS_PUCCH_CRC_LEN     0
#define YS_PUCCH_COD_DES     0
#define YS_PUCCH_BLK_CAT     0
#define YS_PUCCH_MCS_TYP     0

/* PUCCH Message Types */
#define YS_MS_SR_REQ       0x0001    /* SR type */
#define YS_MS_SRS_REQ      0x0002    /* SRS type */
#define YS_MS_CQI_REQ      0x0004    /* CQI type */
#define YS_MS_HARQ_REQ     0x0008    /* HARQ type */

/* OFFSET BETWEEN PHY AND MAC */
#define YS_MS_PHYMAC_SF_OFFSET   0x01
/* Message allocation types */
#define YS_MS_MSG_COMMON   0		/* Common area */
#define YS_MS_MSG_FAST     1		/* I-CPU with direct access */

#define YS_MS_MAX_SDU_SZ 8192 /* Maximum size of the SDU in bytes */

/* Free of message header */
#define YS_FREE_MSG(_msg)           \
{                                   \
   if (_msg != NULLP)               \
   {                                \
      MsgFreeBuffercmFreeMem (_msg);\
      _msg = NULLP;                 \
   }                                \
}

/* Memory allocation/free macros */
#define YS_MS_ALLOC(_buf, _size)\
{\
   if (SGetSBuf(ysCb.ysInit.region, ysCb.ysInit.pool, (Data **)&_buf,   \
                (Size) _size) == ROK)                                   \
   {                                                                    \
      cmMemset((U8 *)(_buf), 0, _size);                                 \
   }                                                                    \
   else                                                                 \
   {                                                                    \
      (_buf) = NULLP;                                                   \
   }                                                                    \
}

#define YS_MS_FREE(_buf, _size)\
{\
    if (_buf != NULLP)                                               \
   {                                                                 \
      (Void) SPutSBuf(ysCb.ysInit.region, ysCb.ysInit.pool,          \
            (Data *) _buf, (Size) _size);                            \
      _buf = NULLP;                                                  \
   }                                                                 \
}

#define YS_MS_ALLOC_BUF(_buf)\
{\
   if (SGetMsg(ysCb.ysInit.region, ysCb.ysInit.pool,       \
   (Buffer **)&_buf) != ROK)                               \
   {                                                       \
      (_buf) = NULLP;                                      \
   }                                                       \
}

#define YS_MS_FREE_BUF(_buf)\
{\
   if (_buf != NULLP)                                      \
   {                                                       \
      SPutMsg(_buf);                                       \
   }                                                       \
   _buf = NULLP;                                           \
}

#define   YS_MS_LAST_CHAN_OFFSET   4
#define   YS_MS_CI_POWER           -32768
#define   PAD                      0x00000000

#define YS_MS_PRIM_TMG_DIFF        2
/* Increment sfn and subframe */
#define YS_INCR_TIMING_INFO(_timingInfo, _delta)                             \
{                                                                            \
   if ((_timingInfo.subframe + _delta) > (YS_NUM_SUB_FRAMES - 1))            \
      _timingInfo.sfn = ((_timingInfo.sfn + 1) % YS_NUM_SFN);                \
   _timingInfo.subframe = ((_timingInfo.subframe + _delta) %                 \
                              YS_NUM_SUB_FRAMES);                            \
}

#define  YsUiTfuBndReq TfUiTfuBndReq
#define  YsUiTfuUbndReq TfUiTfuUbndReq
#define  YsUiTfuSchBndReq TfUiTfuSchBndReq
#define  YsUiTfuSchUbndReq TfUiTfuSchUbndReq
#define  YsUiTfuRecpReq TfUiTfuRecpReq
#define  YsUiTfuCntrlReq TfUiTfuCntrlReq
#define  YsUiTfuDatReq TfUiTfuDatReq

#define YS_MS_SETBITRANGE(_buf, _start, _len, _val) \
   ysMsSetBitRange (_buf, _start, _len, _val)

#ifndef CCPU_MLOG
#define YS_DELYD_PRINTF(_ARG_STR, _ARG_VAL) \
{\
	uart_printf(_ARG_STR, _ARG_VAL);\
}
#else
#define YS_DELYD_PRINTF(_ARG_STR, _ARG_VAL)
#endif

/* HARQ */
#define YS_TIMING_INFO_SAME(_t1, _t2) (((_t1).sfn == (_t2).sfn) && \
      ((_t1).subframe == (_t2).subframe))

#define YS_MS_FILL_PHY_LIST_ELEM(_pElem, _frameNum, _sfNum, _msgPtr, _size, _type)\
{\
(_pElem)->frameNumber = (_frameNum);\
(_pElem)->subframeNumber = (_sfNum);\
(_pElem)->MessagePtr = (U8*)(_msgPtr);\
(_pElem)->MessageLen = (_size);\
(_pElem)->MessageType = (_type);\
(_pElem)->Next = NULLP;\
}

#define YS_MS_CEIL(_a, _b) (((_a) + (_b) - 1)/(_b))

#define YS_MS_DECR_TIME(crntTime, toFill, dcr)  do  \
{                                                  \
   if (crntTime.sfn == 0)                          \
{                                                  \
   if ((crntTime.subframe - (dcr)) < 0)              \
   {                                               \
      toFill.sfn = YS_NUM_SFN - 1;                 \
   }                                               \
   else                                            \
   {                                               \
   toFill.sfn = crntTime.sfn;                      \
   }                                               \
}                                                  \
else                                               \
{                                                  \
   if ((crntTime.subframe - (dcr)) < 0)              \
   {                                               \
   toFill.sfn = crntTime.sfn - 1;                  \
   }                                               \
   else                                            \
   toFill.sfn = crntTime.sfn;                      \
}                                                  \
   toFill.subframe = \
   (YS_NUM_SUB_FRAMES + crntTime.subframe - (dcr)) % (YS_NUM_SUB_FRAMES);   \
}while(0)

#ifndef YS_MS_NO_TA
#define YS_MS_TA_THROTTLED(_ue)  ((_ue)->tarptInfo.lnk.node != NULL)
#endif

#endif /* __YS_MS_H__ */
/********************************************************************30**

         End of file:     ys_ms.h@@/main/1 - Sun Mar 28 21:19:19 2010

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
----------- -------- ---- -----------------------------------------------
/main/1      ---      pk   1. initial release.
/main/1    ys004.102  vr   1. Merged MSPD code with phy 1.7
*********************************************************************91*/
