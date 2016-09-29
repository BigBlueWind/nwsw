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
  
        Name:    LTE RLC file 
    
        Type:    C include file
  
        Desc:    This file contains all the data structures and 
                 prototypes for LTE RLC.
 
        File:    kw.x

        Sid:      kw.x@@/main/2 - Fri Nov 13 14:14:10 2009
  
        Prg:     N/A 
  
*********************************************************************21*/
/** @file kw.x
@brief RLC Product Structures, prototypes
*/

#ifndef __KWX__
#define __KWX__


/*
 *     The structures and variables declared in this file
 *     correspond to structures and variables used by
 *     the following TRILLIUM software:
 *
 *     part no.             description
 *     --------     ----------------------------------------------
 *     1000371      LTE-RLC
 *
*/
 
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @brief Local typedefs */
typedef U16    KwSn;                        /*!< Sequence Number length */
typedef RguDDatIndInfo KwDatIndInfo;
typedef RguDStaIndInfo KwDStaIndInfo;
typedef RguPduInfo KwPduInfo;                 /* kw002.201 : Aligning the structure with RGU */
typedef struct kwUeCb KwUeCb;
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
typedef RguLchMapInfo KwLchMapInfo;
#ifdef LTE_RLC_R9
typedef RgL2MeasUlThrpTimInfo KwL2MeasUlThrpTimInfo;
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */
#ifdef KW_PDCP
typedef struct pjRbCb PjRbCb;               /* PDCP RB control block */
#endif /* KW_PDCP */ 

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
typedef struct kwSduSnMap KwSduSnMap;
#endif /*  LTE_L2_MEAS */

/** @struct KwPduMap
PDU Map Info for AM */
typedef struct kwPduMap
{
  KwSn         sn;                          /*!< Mapped PDU's SN */
  Bool         rcvd;                        /*!< Flag to indicate if an ACK is
                                                 received for the PDU */
}KwPduMap;

/** @struct KwSdu
SDU received from RRC/PDCP */
typedef struct kwSdu                        
{
   CmLList   lstEnt;                        /*!< List entry for SDU */
   Buffer    *mBuf;                         /*!< SDU buffer */ 
   MsgLen    sduSz;                         /*!< Buffer Size */
   MsgLen    actSz;                         /*!< Actual buffer Size */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   KwSduSnMap  *snMap;                       /*!< Sdu and the list of SN it maps to */
#endif /* LTE_L2_MEAS */

   union
   {
      struct   _t 
      {
         U16           sfn;                 /*!< Sub frame number for BCCH/PCCH */
         U8            subframe;            /*!< Subframe number */
         CmLteRnti     rnti;                /*!< RNTI for CCCH */
      }tm;                                  /*!< SDU for TM mode */

      struct   _u
      {
         U32           sduId;               /*!< SDU Identifier */
         Bool          isSegmented;         /*!< Flag to indicate if the SDU is segmented */
      }um;                                  /*!< SDU for UM mode */

      struct   _a
      {
         U32           sduId;               /*!< SDU Identifier */
         MsgLen        rcvdSz;              /*!< Received SDU Size from the peer */
         U8            numPdu;              /*!< Number of PDUs in pduMap */
         KwPduMap      pduMap[KW_MAX_PDU_MAP];   /*!< Array of PDUs mapped with SDU */
         Bool          isSegmented;         /*!< Flag to indicate if the SDU is segmented */
      }am;                                  /*!< SDU for AM mode */
   }mode;                                   /*!< RLC mode specific SDUs */

}KwSdu;

/** @addtogroup tmmode */
/*@{*/

/** @struct KwTm
Transparent mode control block */
typedef struct kwTm
{
   CmLListCp   sduQ;                        /*!< SDU queue for TM */
}KwTm;
/*@}*/

/** @struct KwBoReport
Buffer Occupancy Report */
typedef struct kwBoRep
{
   S32         bo;                          /*!< Buffer occupancy */
   U16         estHdrSz;                    /*!< Estimated header size for the bo */
   Bool        staPduPrsnt;                 /*!< Flag to indicate if the bo is control PDU */
} KwBoRep;

/** @struct KwDatReq
Data Request to be sent to MAC */
typedef struct kwDatReq
{
   MsgLen      pduSz;                       /*!< PDU(s) size indicated by MAC */ 
   KwPduInfo   pduInfo;                     /*!< PDU Information for lChs within UE */
   KwBoRep     boRep;                       /*!< BO report */ 
   U32         dataId;                      /*!< Data Id sent with PDUs to MAC */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   KwLchMapInfo lchMapInfo;                 /*!< Logical channel mapping info */
#endif /* LTE_L2_MEAS */
}KwDatReq;

/** @defgroup ummode UM Module Info 
*/

/** @addtogroup ummode */
/*@{*/
/** @struct KwUmDl
Unacknowledged mode downlink control block */
typedef struct kwUmDl
{
   CmLListCp   sduQ;                        /*!< SDU queue for UM */
   U8          snLen;                       /*!< Sequence number length */
   S32         bo;                          /*!< Buffer occupancy */
   /* kw005.201 removed the warning */
   S32         estHdrSz;                    /*!< Estimated header size for the available BO */
   KwSn        vtUs;                        /*!< VT(US) */
   U16         numLi;                       /*!< Number of LIs */
   U16         li[KW_MAX_LI];               /*!< Length Indicator array */
}KwUmDl;

/** @struct KwUmhdr 
UM Header Info */
typedef struct kwUmHdr
{
   U8          fi;                          /*!< Framing Info */
   KwSn        sn;                          /*!< Sequence number */
   U16         numLi;                       /*!< Number of LIs */
#ifdef KW_BG_UL_PROC
   U16         numSduProcessed;             /*!< Number of RLC SDUs delivered to
                                                 PDCP in UL till now for this SN.
                                                 It is not used in DL*/
#endif 
   U16         li[KW_MAX_LI];               /*!< Array of LIs */
}KwUmHdr;

/** @struct KwUmRecBuf 
UM Reception Info */
typedef struct kwUmRecBuf
{
   Buffer      *pdu;                        /*!< PDU Buffer */
   KwUmHdr     *umHdr;                      /*!< UM Header Info */
   MsgLen      pduSz;                       /*!< PDU Size */
#ifdef KW_BG_UL_PROC
   CmLList     rlsUmPduLnk;                 /*!< Link to add to 
                                                 toBeReasmbldUmPduLst */
#endif /* KW_BG_UL_PROC */
}KwUmRecBuf;

/** @struct KwUmUl
Unacknowledged mode uplink control block */
typedef struct kwUmUl
{
   U8          snLen;                       /*!< Sequence number length */
   KwUmRecBuf  **recBuf;                    /*!< Reception buffer */
   KwSn        umWinSz;                     /*!< UM window size */ 
   KwSn        sn;                          /*!< Sequence number */
   KwSn        vrUr;                        /*!< VR(UR) - Receive state variable */
   KwSn        vrUh;                        /*!< VR(UH) - Highest received state variable */
   KwSn        vrUx;                        /*!< VR(UX) - Reordering state variable */
   CmTimer     reOrdTmr;                    /*!< Reordering Timer */
   U8          reOrdTmrInt;                 /*!< Timer Interval */
   Buffer      *partialSdu;                 /*!< Partial SDU - Remains till the complete SDU is received */
#ifdef KW_BG_UL_PROC
   CmLListCp   toBeReasmbldUmPduLst;        /*!< List to hold UL PDUs to be reassembled */
#endif /* KW_BG_UL_PROC */
}KwUmUl;
/*@}*/

/** @struct KwAmhdr 
AM Header Info */
typedef struct kwAmHdr
{
   U8          dc;                          /*!< Data/Control PDU */
   U8          rf;                          /*!< Resegmentation flag */
   U8          p;                           /*!< Poll bit */
   U8          fi;                          /*!< Framing Info */
   U8          e;                           /*!< Extension bit */
   KwSn        sn;                          /*!< Sequence number */
   U8          lsf;                         /*!< Last segment flag */
   U16         so;                          /*!< Segment offset */
   U16         numLi;                       /*!< Number of LIs */
   U16         li[KW_MAX_LI];               /*!< Array of LIs */
#ifdef KW_BG_UL_PROC
   U16         numSduProcessed;
#endif /* KW_BG_UL_PROC */
}KwAmHdr;

/** @struct KwSduMap
SDU Map Info for AM */
typedef struct kwSduMap
{
   U32           sduId;                     /*!< SDU Identifier */
   MsgLen        actSz;                     /*!< SDU Size */
   MsgLen        sduSz;                     /*!< SDU Size */
   Bool          isSegmented;               /*!< Flag to indicate if the SDU is segmented */
}KwSduMap;

/** @struct KwRetx
Retransmission Buffer */
typedef struct kwRetx
{
   CmLList     lstEnt;                      /*!< List entry for Retransmission PDU */
   Buffer      *seg;                        /*!< Buffer of PDU or Segment */
   MsgLen      segSz;                       /*!< Segment Size */
   MsgLen      hdrSz;                       /*!< Header size for the segment */
   KwAmHdr     amHdr;                       /*!< AM header Info */
   U16         soEnd;                       /*!< Segment Offset End */
   KwSn        retxCnt;                     /*!< Retransmission count */
   Bool        yetToConst;                  /*!< Yet to construct seg flag */
   Bool        wtForAck;                    /*!< Flag to indicate if the node is 
                                                 waiting for an Ack */
   U8          numSdu;                      /*!< Number of PDUs in sduMap */
   KwSduMap    sduMap[KW_MAX_LI];           /*!< Array of SDUs mapped with PDU */
/* kw006.201 optimization, fix for ccpu00111716 */
#ifdef KW_BG_DL_PROC
   CmLList     rlsReTxLnk;                  /*!< Lnk to store on release ReTx lst */
   Bool        addToReTxLst;                /*!< Added to toBeRlsReTxLst of not */
#endif
}KwRetx;

/** @struct KwTx
Transmission Buffer */
typedef struct kwTx
{
   Buffer      *pdu;                        /*!< Buffer of PDU */
   MsgLen      pduSz;                       /*!< PDU Size */
   MsgLen      hdrSz;                       /*!< Header size for the PDU */
   KwAmHdr     amHdr;                       /*!< AM header Info */
   U8          numSdu;                      /*!< Number of PDUs in sduMap */
   KwSduMap    sduMap[KW_MAX_LI];           /*!< Array of SDUs for this PDU */
/* kw006.201 optimization, fix for ccpu00111716 */
#ifdef KW_BG_DL_PROC
   CmLList     rlsTxLnk;                    /*!< Lnk to store on release Tx lst */
#endif
}KwTx;

/** @addtogroup ammode */
/*@{*/

/** @struct KwAmDl
Acknowledged mode downlink control block */
typedef struct kwAmDl
{
   CmLListCp   sduQ;                        /*!< SDU queue */
   KwSdu       *nxtTx;                      /*!< Next SDU to be transmitted */
   CmTimer     pollRetxTmr;                 /*!< T_poll_retransmit Timer */
   U16         pollRetxTmrInt;              /*!< Timer Interval */
   S16         pollPdu;                     /*!< Poll_PDU */
   S32         pollByte;                    /*!< Poll_Byte */
   U8          maxRetx;                     /*!< Max_Retx_Threshold */
   S32         cntrlBo;                     /*!< BO of control pdu */ 
   S32         retxBo;                      /*!< BO of Retransmission PDUs */ 
   U32         estHdrSz;                    /*!< Estimated header size for the available BO */
   S32         bo;                          /*!< BO of new PDUs */
   KwTx        **txBuf;                     /*!< Transmission buffer */
   CmLListCp   retxLst;                     /*!< Retransmission PDU list */
   KwRetx      *nxtRetx;                    /*!< Next node to be retransmitted */
   KwSn        txSn;                        /*!< Starting sn for txBuf */
   KwSn        vtS;                         /*!< VT(S) - Send state variable */
   KwSn        vtMs;                        /*!< VT(MS) - Maximum send state variable */
   KwSn        vtA;                         /*!< VT(A) - Acknowledgement state variable */ 
   S16         pduWoPoll;                   /*!< PDU without poll bit counter */
   S32         byteWoPoll;                  /*!< Byte without poll bit counter */
   KwSn        pollSn;                      /*!< Poll SN */
   /* kw003.201 */
   S16         numSduPdu;                   /*!< Number of SDUs/PDU to be sent*/
   S16         numSeg;                      /*!< Number of segments to be sent*/
#ifdef KW_BG_DL_PROC
   CmLListCp   toBeRlsTxLst;                /*!< Stores to be released PDUs */
   CmLListCp   toBeRlsReTxLst;              /*!< Stores to be released PDUs */
#endif
/* RLC_DL_MAX_RETX fix */
   Bool        isRbstalled;                 /*!< this is set when the DL MAX Retx  exceeded */
}KwAmDl;

/*@}*/


/** @struct KwSeg
Segment List */
typedef struct kwSeg
{
   CmLList     lstEnt;                      /*!< List entry for PDU segment */
   Buffer      *seg;                        /*!< PDU segment */
   MsgLen      segSz;                       /*!< Buffer Size */
   U16         soEnd;                       /*!< Segment Offset End */
   KwAmHdr     amHdr;                       /*!< AM header */
}KwSeg;

/** @struct KwAmRecBuf
AM Reception buffer */
typedef struct kwAmRecBuf
{
   Buffer      *pdu;                        /*!< PDU buffer */
   MsgLen      pduSz;                       /*!< Buffer Size */
   KwAmHdr     amHdr;                       /*!< AM header Info */
   CmLListCp   segLst;                      /*!< PDU Segments list */
   KwSeg       *expByteSeg;                 /*!< Next expected byte segment */
   U16         expSo;                       /*!< Next expected seg offset */
   Bool        allRcvd;                     /*!< All bytes received flag for
                                                 this PDU */
#ifdef KW_BG_UL_PROC
   CmLList     rlsAmPduLnk;                 /*!< Link to add to toBeReasmbldAmPduLst */
#endif /* KW_BG_UL_PROC */
}KwAmRecBuf;

/** @addtogroup ammode */
/*@{*/

/** @struct KwAmUl
Acknowledged mode uplink control block */
typedef struct kwAmUl
{
   CmTimer     staProhTmr;                  /*!< T_status_prohibit Timer */
   U16         staProhTmrInt;               /*!< Timer Interval */
   CmTimer     reOrdTmr;                    /*!< T_reordering Timer */
   U8          reOrdTmrInt;                 /*!< Timer Interval */
   KwAmRecBuf  **recBuf;                    /*!< Reception buffer */
   KwSn        sn;                          /*!< Sequence number */
   KwSn        vrR;                         /*!< VR(R) - Receive state variable */
   KwSn        vrH;                         /*!< VR(H) - Highest received state variable */
   KwSn        vrX;                         /*!< VR(X) - reorderig state variable */
   KwSn        vrMr;                        /*!< VR(MR) - Maximum acceptable receive 
                                                 state variable */ 
   KwSn        vrMs;                        /*!< VR(MS) - Maximum STATUS transmit 
                                                 state variable */
   Bool        staTrg;                      /*!< Flag to indicate if status trigger occured */
   Buffer      *partialSdu;                 /*!< Partial SDU - Remains till the 
                                                 complete SDU is received */
   S32         cntrlBo;                     /*!< BO of control pdu */ 
   KwSn        expSn;                       /*!< Expected SN for reassembly */
   U16         expSo;                       /*!< Expected SO for reassembly */
#ifdef KW_BG_UL_PROC
   CmLListCp   toBeReasmbldAmPduLst;        /*!< List to hold UL PDUs to be reassembled */
#endif /* KW_BG_UL_PROC */
}KwAmUl;

/*@}*/

/**
 * @struct KwLchInfo
 * Logical Channel Information
 */
typedef struct kwLchInfo
{
   CmLteLcId       lChId;              /*!< Logical channel Id */
   CmLteLcType     lChType;            /*!< Logical channel type */
}KwLchInfo;

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS

/** @struct KwL2Cntr
 * RLC L2 Counter  */
typedef struct kwL2Cntr
{
   struct
   {
      U32  numActvUe;        /*!< number of active Ue */
      U32  sampOc;           /*!< Total number of sampling occasion */
   }actUe;
   struct
   {
      U32  dLoss;            /*!< Total number of lost packets */  
      U32  posPkts;          /*!< Total number of positively acknowlegded 
                                  packets */
   }uuLoss;
   struct                    /*!< For DL IP throughput */
   {
      U32 volSummation;      /*!< Sum of data in bytes */
      U32 timeSummation;     /*!< Sum of time difference in milli sec*/
   }dlIpThruput;
   struct                    /*!< For UL IP throughput */
   {
      U32 volSummation;      /*!< Sum of data in bytes */
      U32 timeSummation;     /*!< Sum of time difference in milli sec*/
   }ulIpThruput;
}KwL2Cntr;

struct kwSduSnMap
{
   CmLList     lstEnt;
   Bool        failMarked;
   Bool        fullySent;
   U32         sduId;
   U16         numSn;
   U16         snList[KW_MAX_PDU_MAP];
   U16         harqAck;
   U16         reqSent;
   U16         rspRcvd;
};

typedef struct kwSnSduMap
{
   U16         sn;
   CmLteLcId   lChId;              /*!< Logical channel Id */
   U16         numSdu;
#ifdef LTE_RLC_R9
   Bool        isBurstSplitted;    /*!< true: burst for this LCH is splitted */
#endif /* LTE_RLC_R9 */
   KwSduSnMap  *sduList[KW_MAX_LI];
}KwSnSduMap;

typedef struct kwTbSnMap
{
   CmHashListEnt  hlTbEnt;
   U32            tbId;
   U16            prevNumSn;
   U16            numSn;
   KwSnSduMap     snSduMap[RGU_MAX_PDU * RGU_MAX_LC];
}KwTbSnMap;

/** @struct KwL2MeasCb
 * RLC L2 Measurement CB */
typedef struct kwL2MeasCb
{
   U8            measType;                /*!< Bit-wise set measurement types */ 
   U16           numSamples;              /*!< Number of samples to take on numActUe */
   U16           numQci;                  /*!< number of valid qcI */             
   U8            qci[LKW_MAX_QCI];        /*!< list of valid qcI */     
   KwL2Cntr      measData[LKW_MAX_QCI];   /*!< Measurement CB */
#ifdef LTE_RLC_R9
   CmLteRnti     ueId;                    /*!< UE ID (Used only for IP Throughput
                                             in UL/DL */
   CmLteCellId   cellId;                  /*!< UE ID (Used only for IP Throughput
                                             in UL/DL */
   U8            numLcId;                 /*!< Holds the number of LCh for which Ul Ip 
                                             measurement is ON */
   U8            lcId[KW_MAX_LCH_PER_UE]; /*!< Holds the list of LCh for which Ul ip
                                             measurement is ON */
#endif /* LTE_RLC_R9 */
}KwL2MeasCb;

/** @struct KwL2MeasEvtCb
 * RLC L2 Measurement Evt CB */
typedef struct kwL2MeasEvtCb
{
   U32           transId;                /*!< TransId of Measurement Req */
   U32           cbIdx;                  /*!< TransId of Measurement Req */
   CmTimer       l2Tmr;                  /*!< L2 Timer per request */
   TmrCfg        l2TmrCfg;               /*!< Time period of measurement */
   KwL2MeasCb    measCb;                 /*!< Measurement CB */ 
}KwL2MeasEvtCb;

/** @struct KwL2MeasRbCb
 * RLC L2 Measurement Rb CB */
typedef struct kwL2MeasRbCb
{
   U8            measOn;                      /*!< Measurements that are running */ 
   KwL2Cntr      *l2Sts[KW_MAX_L2MEAS_EVT];  /*!< L2 Mesurement statistics */     
}KwL2MeasRbCb;

/** @struct KwL2Cb
 * RLC L2  CB */
typedef struct kwL2Cb
{
   CmHashListCp  qciHlCp;                     /*!< Hashlist of qci */
   CmHashListCp  tbHlCp;                      /*!< Hashlist of TBId to KwTbSnMap */
   U32           tbId;                        /*!< TbId of the tbList */
   KwTbSnMap     *curTbSnMap;                 /*!< Current tbSnMap: in U/AmmProcessSdus */
   U16           kwNumMeas;                   /*!< Number of measurements going on */
   KwL2MeasEvtCb *kwL2EvtCb[LKW_MAX_L2MEAS];  /*!< Pointers to Measurement Cb */
   U8           measOn[LKW_MAX_QCI];          /*!< Measurement on */
   U32           numActUe[LKW_MAX_QCI];       /*!< Measurement on */
}KwL2Cb;

#ifdef LTE_RLC_R9
typedef enum _dlIpThrputState
{
   KW_DL_IPTHRU_RESET = 0,
   KW_DL_IPTHRU_BURST_STARTED,
   KW_DL_IPTHRU_BURST_CONTINUE,
   KW_DL_IPTHRU_BURST_COMPLETED
}DlIpThrputState;

/* Structure to hold parameters for DL/UL ip throughput for a RB */
typedef struct _kwL2MeasIpThruput
{
   EpcTime         timeT2;                  /*!< Time stamp T2(in milli sec)*/
   EpcTime         tempTimeT1;              /*!< Temporary Time stamp T1(in milli sec)*/
   U32             dataVol;                 /*!< Holds volume of new data in bytes
                                              for DL IP throughput */
   DlIpThrputState dlIpThrputSt;            /*!< Holds the state of RbCb */
   KwTbSnMap       *prevTbSnMap;            /*!< Holds the KwTBSnMap for the 1st allocation
                                                 which splits the data burst*/
   U32             ulDataVol;               /*!< Holds volume of new data in bytes
                                              for UL IP throughput */
   U16             count;                   /*!< Holds the number of times DL Data 
                                               Burst is split across a TTI.*/
}KwL2MeasIpThruput;
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */

/** @struct KwRbCb 
RLC RB control block */
typedef struct _kwRbCb
{
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   CmHashListEnt   qciHlEnt;                /*!< QcI Hash list entry for RbCb */
   KwUeCb          *ueCb;                   /*!< Pointer to UeCb  */
   KwL2MeasRbCb    rbL2Cb;                  /*!< RB measurement L2 Cb */
   CmLListCp       sduSnMapQ;               /*!< SDU SN map queue for UM */
   U8              qci;                     /*!< qci of the RB */
#ifdef LTE_RLC_R9
   KwL2MeasIpThruput l2MeasIpThruput;       /*!< Holds related parameter for
                                                 DL/Ul ip throughput>*/
#endif /* LTE_RLC_R9 */
#endif /* LTE_L2_MEAS */
   CmLteRlcId      rlcId;                   /*!< RLC Identifier */
   KwLchInfo       lch[CM_LTE_MAX_LOGCH_PER_RB]; /*!< Logical Channel Info 
                                                      For AM mode: 
                                                      idx = 0 is for DL and
                                                      idx = 1 is for UL */
   CmLteRlcMode    mode;                    /*!< Entity Mode */
   U8              dir;                     /*!< Direction for UL/DL */
   U32             transId;                 /*!< Transaction Id for RLC */
   /* kw005.201 updated for statistics calculation */
   SpId            kwuSapId;                /*!< KWU sap Id, to get the KwuSapCb */
   union          
   {

/** @defgroup tmmode TM Module Info 
*/
      KwTm         tm;                      /*!< TM mode specific elements */

      struct       _umInfo 
      {
         KwUmDl umDl;                       /*!< UM downlink elements */
         KwUmUl umUl;                       /*!< UM uplink elements */
      }um;                                  /*!< UM specific Info */

/** @defgroup ammode AM Module Info 
*/
      struct       _amInfo
      {
         KwAmDl    amDl;                    /*!< AM downlink elements */
         KwAmUl    amUl;                    /*!< AM uplink elements */
      }am;                                  /*!< AM specific Info */
   }m;                                      /*!< RLC mode specific Info */
#ifdef KW_PDCP
   PjRbCb *pjRbCb;                          /*!< Pointer to PDCP RbCb */
#endif /* KW_PDCP */
/* kw006.201 optimization, fix for ccpu00111716 */
#ifdef KW_BG_DL_PROC
   CmLList        rlsRbLnk;                 /*!< Link to add to release Rb list */
#endif /* KW_BG_DL_PROC */
/* kw006.201 optimization, fix for ccpu00117290 */
#ifdef KW_BG_UL_PROC
   CmLList        rlsReasmblyRbLnk;         /*!< Link to add to reasmblyRbList */
#endif /* KW_BG_UL_PROC */
#if ((defined KW_BG_DL_PROC) || (defined KW_BG_UL_PROC))
   U8             rbOp;                     /*!< Multipurpose flag for both RB
                                                 deletion and Re-Establishment.
                                                 Initial value during RB creation=0.
                                                 Bit 0 is LSB and bit 7 is MSB.
                                                 Bit 0:Indicates DL ReEstablishment.
                                                 Bit 1:Indicates UL ReEstablishment.
                                                 Bit 2:Indicates DL RB deletion.
                                                 Bit 3:Indicates UL RB deletion.
                                                 When ReEstablish Req is received,
                                                 bit 0 and 1 will be set. When RB
                                                 deletion is received, bit 2 and 3
                                                 will be set. These bits will be 
                                                 reset back to 0 after all DL PDUs
                                                 are deleted and UL PDUs are reassembled.*/
   struct rbCb           *rb;                 /*!<It holds the RbCb pointer. It is
                                                 initialized during RB creation.*/
#endif /*((defined KW_BG_DL_PROC) || (defined KW_BG_UL_PROC)) */
   U8                     reestInProgress;
}KwRbCb;

#ifdef KW_PDCP

typedef KwSn   PjSn;                        /*!< Sequence Number length */

/* kw005.201 added support for L2 Measurement */
/** @defgroup l2meas L2Meas Module Info 
*/
/** @addtogroup l2meas */
/*@{*/
#ifdef LTE_L2_MEAS

/** @struct PjL2Cntr
 * PDCP L2 Counter  */
typedef struct pjL2Cntr
{
   struct 
   {
       U32      numPkts; /*!< Number of packet monitired in dlDelay */        
       EpcTime  val;     /*!< L2 Measurement value in dlDelay */
   }dlDelay;
   struct
   {
       U32      numPkts; /*!< Number of packet monitired in ulLoss */        
       U32      val;     /*!< L2 Measurement value in ulLoss */
   }ulLoss;
   struct
   {
       U32      numPkts;  /*!< Number of packet monitired in ulLoss */        
       U32      val;     /*!< L2 Measurement value in dlDisc */
   }dlDisc;
}PjL2Cntr;

/** @struct PjL2MeasCb
 * PDCP L2 Measurement CB */
typedef struct pjL2MeasCb
{
   U8            measType;               /*!< Bit-wise set measurement types */ 
   CmLListCp     rbList;                 /*!< RB List maintained during measurements */       
   U16           numQci;                 /*!< number of valid qcI */             
   U8            qci[LPJ_MAX_QCI];       /*!< list of valid qcI */     
   PjL2Cntr      measData[LPJ_MAX_QCI];  /*!< Measurement CB */             
}PjL2MeasCb;

/** @struct PjL2MeasEvtCb
 * PDCP L2 Measurement Evt CB */
typedef struct pjL2MeasEvtCb
{
   U32           transId;  /*!< TransId of Measurement Req */
   U32           cbIdx;    /*!< TransId of Measurement Req */
   CmTimer       l2Tmr;    /*!< L2 Timer per request */
   TmrCfg        l2TmrCfg; /*!< Time period of measurement */
   PjL2MeasCb    measCb;   /*!< Measurement CB */ 
}PjL2MeasEvtCb;

/** @struct PjL2Cb
 * PDCP L2  CB */
typedef struct pjL2Cb
{
   U16           pjNumMeas;                       /*!< Number of measurements going on */
   PjL2MeasEvtCb *pjMeasEvtCb[PJ_MAX_L2MEAS_EVT]; /*!< Pointers to Measurement Cb */
   U8           measOn[LPJ_MAX_QCI];              /*!< Measurement on */
}PjL2Cb;

/** @struct PjL2MeasRbCb
 * PDCP L2 Measurement Rb CB */
typedef struct pjL2MeasRbCb
{
   U8            measOn;                    /*!< Measurements that are running */ 
   U32           startCount;                /*!< start count for dl Delay */       
   CmLList       listNode[PJ_MAX_L2MEAS_INRB + 1];  /*!< list Entry for measurements */             
   PjL2Cntr      *l2Sts[PJ_MAX_L2MEAS_INRB + 1];    /*!< Dl Delay counter */     
}PjL2MeasRbCb;
#endif /* LTE_L2_MEAS */
/*@}*/

/** @addtogroup dldata */
/*@{*/

/**
 *  @brief
 *  PDCP SDU Status Information for a DRB mapped to RLC AM.
 */

typedef struct pjHoCfmInfo
{
   U8             rbId;                     /*!< RB ID */
   Bool           pres;                     /*!< Is handover info present for this RB */
   U8             dir;                      /*!< Direction of the RB */
   struct          
   {
      U8          numBits;                  /*!< Number of bits in UL bitmap */
      U8          *ulBitMap;                /*!< Status Report bitmap of PDUs */
      U32         count;                    /*!< UL Count value with FMS */
   }ul;
   struct  
   {
      U32         count;                    /*!< Count of the next SN to be assigned */
   }dl;
}PjHoCfmInfo;

/**
 *  @brief
 *  Handover information maintainned during processing of SduStaReq
 */
typedef struct pjHoInfo
{
   U32           transId;                   /*!< Transaction ID */
   PjHoCfmInfo   *hoCfmInfo;                /*!< SDU Status Confirm */
}PjHoInfo;

typedef struct pjCfmEnt
{
   U8             rbId;                  /*!< RB ID */
   U8             rbType;                /*!< RB Type */
   U8             cfgType;               /*!< CfgType for the Cfm */
   Status         status;                /*!< ROK or RFAILED */
   Reason         reason;                /*!< Reason for failure */
}PjCfmEnt;

/**
 * @brief
 * PDCP configuration confirm structure. 
 */
typedef struct pjCfgCfmInfo
{
   CmLteRnti      ueId;                   /*!< UE ID */
   CmLteCellId    cellId;                 /*!< Cell ID */
   U32            transId;                /*!< Transcation ID */
   U8             entity;                 /*!< Entity */
   U8             numEnt;                 /*!< Number of entities */
   PjCfmEnt      cfmEnt[CPJ_MAX_CFM];    /*!< Confirm parameter for
                                               PDCP entity. */
}PjCfgCfmInfo;

typedef struct pjSecCfgCfmInfo
{
   CmLteRnti      ueId;       /*!< UE ID */
   CmLteCellId    cellId;     /*!< Cell ID */
   U32            transId;    /*!< Transcation ID */
   Status         status;     /*!< ROK or RFAILED */
   Reason         reason;     /*!< Reason for failure */
}PjSecCfgCfmInfo;

/**
 *  @brief
 *  When the libraries are offboarded, the confirms are sent
 *  asynchronously. This is to store them.
 */
typedef struct pjAsyncCfm
{
   U8             cfmType;                  /*!< PJ_SEC_ASYNC_CFM, PJ_CFG_ASYNC_CFM, 
                                                 PJ_REEST_ASYNC_CFM, PJ_CFG_REEST_ASYNC_CFM
                                                 PJ_CFG_UEDEL_ASYNC_CFM */
   U8             startTmr;                 /*!< Whether to start the timer or not */
   U32            libInitBitMask;           /*!< Bitmask to maintain the cfm for Init req */
   U32            cmpInitBitMask;           /*!< Bitmask to track cfm for compression */

   CmLteRnti      ueId;                     /*!< UE ID */
   CmLteCellId    cellId;                   /*!< Cell ID */
   U32            transId;                  /*!< Transcation ID */
   U8             entity;                   /*!< Entity */
   U8             numEnt;                   /*!< Number of entities */
   PjCfmEnt       cfmEnt[CPJ_MAX_CFM];      /*!< Confirm parameter for
                                                 PDCP entity. */
}PjAsyncCfm;
/**
 *  @brief
 *  Hook information maintainned during processing
 */
typedef struct pjLibInfo
{
   U8         state;                        /*!< State of the UE- Normal or ReEst. */
   U16        numReEstDrbUl;                /*!< Number of UL RBs under reestablishment*/
   U16        numReEstDrbDl;                /*!< Number of DL RBs under reestablishment*/
   CmTimer    obdTmr;                       /*!< Timer to wait for InitCfm 
                                                 and UE Delete */
   U32        tLibInitBitMask;              /*!< Bitmask to maintain the cfm for Init req */
   U32        tCmpInitBitMask;              /*!< Bitmask to track cfm for compression */
   PjAsyncCfm *asyncCfm[PJ_MAX_ASYNC_CFM];  /*!< Confirm list to handle multiple simultaneous
                                                 config requests when cfm is sent asynchronously */
   U16        nxtAvlbTxId;                  /*!< Next available transaction Id for entry 
                                                 in the async Cfm list */
   U16        crntTmrTxId;                  /*!< Transaction id for which the timer is started */
}PjLibInfo;

/** @struct PjSecInp
Input parameters for security algorithm */
typedef struct pjSecInp
{
   U32         count;                       /*!< COUNT associated with SDU/PDU */ 
   CmLteRbId   rbId;                        /*!< RB Identifier */
   U8          dir;                         /*!< Direction UL/DL/biderectional */
}PjSecInp;

/** @struct PjCmpCxt
Context for ROHC */
typedef struct pjCmpCxt
{
   CmLteCellId cellId;                      /*!< Cell Identifier */
   CmLteRnti   ueId;                        /*!< UE Identifier */
   CmLteRbId   rbId;                        /*!< RB Identifier */
   U8          rbType;                      /*!< RB Type */
   U16         cfgTxId;                     /*!< Tx Idx of config confirm */
}PjCmpCxt;

/** @struct PjSecCxt
Context for Security Algorithm */
typedef struct pjSecCxt
{
   CmLteCellId cellId;                      /*!< Cell Identifier */
   CmLteRnti   ueId;                        /*!< UE Identifier */
   U16         cfgTxId;                     /*!< Tx Idx of config confirm */
}PjSecCxt;

/** @struct PjLibTrans
Transaction Id for Security Algorithm */
typedef struct pjLibTrans
{
   U32         count;                       /*!< COUNT associated with SDU/PDU */
   CmLteRbId   rbId;                        /*!< PDCP RbCb */
   U8          rbType;                      /*!< RB Type : 0 - PJ_SRB, 1 - PJ_DRB */
   KwUeCb      *ueCb;                       /*!< UeCb pointer */
}PjLibTrans;

/** @struct PjIntInfo
PDCP Integrity Algo Information */
typedef struct pjIntInfo
{
   U8          algoType;                    /*!< Type of the algorithm */
   U8          intKey[PJ_INTG_KEY_LEN];      /*!< Integrity key */
}PjIntInfo;

/** @struct PjCipherInfo
PDCP Ciphering Algo Information */
typedef struct pjCipherInfo
{
   U8          algoType;                    /*!< Type of the algorithm */
   U8          cpKey[PJ_CIPHER_KEY_LEN];    /*!< Control plane ciphering key */
   U8          upKey[PJ_CIPHER_KEY_LEN];    /*!< User plane ciphering key */
}PjCipherInfo;

/** @struct PjSec
PDCP Security Info */
typedef struct pjSec
{
   Bool          secAct;                    /*!< Flag to indicate if security is 
                                                 activated for this UE */ 
   PjIntInfo     intInfo;                   /*!< Integrity Algo Information */
   PjCipherInfo  cipherInfo;                /*!< Ciphering Algo Information */
   Bool          selSecAct;                 /*!< Flag to indicate if selective 
                                                 security activation is set */ 
   Bool          firstDl;                   /*!< Flag to indicate the first DL
                                                 SRB msg received */
   Bool          firstUl;                   /*!< Flag to indicate the first UL 
                                                 SRB msg received */
   PjSecCxt      secCxt;                    /*!< Context for security algorithm */
   PTR           intCxtId;                  /*!< Context Id for Integrity
                                                 protection / verification */
   PTR           cpCxtId;                   /*!< Context Id for cntrl plane 
                                                 ciphering / deciphering */
   PTR           upCxtId;                   /*!< Context Id for user plane
                                                 ciphering / deciphering */
}PjSec;

/** @struct PjBuf
PDCP Tx/Rx Buffer */
typedef struct pjBuf
{
   U8          numBins;                     /*!< Number of lists for datQ */   
   U32         numEntries;                  /*!< Total number of entries in the datQ */   
   CmLListCp   *datQ;                       /*!< Array of lists for Tx SDUs/Rx PDUs */
}PjBuf;

/** @struct PjTxEnt
PDCP Tx Entry - SDU received from Relay */
typedef struct pjTxEnt
{
   CmLList     lstEnt;                      /*!< List entry for SDU */
   U8          state;                       /*!< SDU state */
   Ticks       arrTime;                     /*!< Arrival Time of the SDU */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   EpcTime     epcTime;                     /*!< Arrival Time of the SDU for dl delay */
#endif
   U32         txHfn;                       /*!< HFN associated with this SDU 
                                                 for the tx side COUNT */
   PjSn        sn;                          /*!< PDCP SN associated with this SDU */
   U32         count;                       /*!< COUNT associated with this SDU */  
   Buffer      *pdu;                        /*!< Pdu Buffer */
   Buffer      *sdu;                        /*!< Sdu Buffer */
   U32         sduId;                       /*!< SDU ID */
}PjTxEnt;

/** @struct PjDlCb 
PDCP downlink control block */
typedef struct pjDlCb
{
   S16         discTmrInt;                  /*!< Discard timer Interval */
   Bool        cfmReqd;                     /*!< Flag to indicate if
                                                 confirmation is required for the
                                                 SDUs delivered to the peer PDCP */
   PjBuf       txBuf;                       /*!< Transmission buffer */
   U32         txHfn;                       /*!< HFN associated with the recent
                                                 SDU */
   PjSn        nxtTxSn;                     /*!< PDCP SN to be associated with 
                                                 the next SDU */
   U32         count;                       /*!< DL COUNT associated with the
                                                 recent SDU */
   U32         nxtToSub;                    /*!< COUNT of the next PDU to be
                                                 submitted to RLC */
   U32         cfmExp;                      /*!< COUNT of the next PDU for which
                                                 confirmation is expected */
   CmTimer     discTmr;                     /*!< Discard timer */
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   CmTimer     obdTmr;                      /*!< Offboard timer */
   U32         obdCount;                    /*!< OBD Count */
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */
}PjDlCb;
/*@}*/

/** @defgroup uldata UL Module Info 
*/
/** @addtogroup uldata */
/*@{*/

/** @struct PjRxEnt
PDCP Rx Entry - PDU received from RLC */
typedef struct pjRxEnt
{
   CmLList     lstEnt;                      /*!< List entry for PDU */
   U8          state;                       /*!< PDU state */
   U32         count;                       /*!< COUNT associated with this PDU */
   Bool        discFlag;                    /*!< Whether entry can be discarded 
                                                 after processing */
   Bool        dupEntry;                    /*!< Duplicate Entry, These entries
                                                 are not inserted */
   Buffer      *mBuf;                       /*!< Buffer to store the pdu during processing */
}PjRxEnt;

/** @struct PjUlCb 
PDCP uplink control block */
typedef struct pjUlCb
{

   Bool        staRepReqd;                  /*!< Flag to indicate if status report 
                                                 is required */
   Bool        discReqd;                    /*!< Flag to indicate if the PDU can
                                                 be discarded during Integrity
                                                 failure */
   PjBuf       recBuf;                      /*!< Reception buffer */
   U8          state;                       /*!< RB state for the UL entity */
   U32         nxtSubCnt;                   /*!< Count of next SDU to be submitted 
                                                 to the upper layer. Used in SRB, DRBUM */
   U32         nxtSubDeCmp;                 /*!< Count of next PDU to be submitted 
                                                 to the decompression unit */
   U32         lastSubCnt;                  /*!< Count of last SDU submitted  
                                                 to the upper layers */       
   U32         obdPdu;                      /*!< Count of the Pdu sent for offboarding  */
   U32         firstReEstCnt;               /*!< Count of the first PDU received after
                                                 reestablishment was triggered */ 
   Bool        transCmp;                    /*!< Whether transmission of PDUs from RLC
                                                 during reestablishment is complete */ 
   PjSn        nxtRxSn;                     /*!< PDCP SN to be associated with the next PDU */
   U32         rxHfn;                       /*!< PDCP HFN value to be associated with 
                                                 the PDCP SN to derive the count */
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   CmTimer     obdTmr;                      /*!< Offboard timer */
   U32         obdCnt;                    /*!< OBD Count */
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */
}PjUlCb;
/*@}*/

/** @struct PjHdrCmp
Robust header compression Information */
typedef struct pjRohc
{
   Bool        hdrCmpUsed;                  /*!< Header Compression Used/Not */
   U16         maxCid;                      /*!< Max CID */
   U16         profId;                      /*!< ROHC Profile Id */ 
}PjRohc;

/** @struct PjRbCb 
PDCP RB control block */
struct pjRbCb
{
   CmLteRbId   rbId;                        /*!< RB Id for PDCP Cb */
   U8          rbType;                      /*!< RB Type : 0 - KW_SRB, 1 - KW_DRB */
   CmLteRlcMode mode;                       /*!< Mapped RLC entity's mode */
   U8          dir;                         /*!< Direction for UL/DL/biderctional */
   U8          snLen;                       /*!< Sequence number length */
   PjRohc      rohc;                        /*!< ROHC Information */ 
   PjCmpCxt    cmpCxt;                      /*!< Context for ROHC */
   PTR         cmpCxtId;                    /*!< Context Id for ROHC */ 
   KwRbCb      *kwRbCb;                     /*!< Pointer to RLC RbCb */
   U8          state;                       /*!< RB state - Used only for DRBs*/
   Bool        firstDlMsg;                  /*!< Flag for firstDlMsg 
                                                 Set to FALSE at start */
   PjSn        firstSn;                     /*!< SN of the first message sent
                                                 for Integrity Protection */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   U8          qci;                         /*!< QCI value associated with rb */
   PjL2MeasRbCb rbL2Cb;                     /*!< L2 Meas Cb for RB */
#endif /* LTE_L2_MEAS */
/** @defgroup dldata DL Module Info 
*/
   PjDlCb      dlCb;                        /*!< Downlink PDCP RbCb */
   PjUlCb      ulCb;                        /*!< Uplink PDCP RbCb */ 
   KwUeCb      *ueCb;                       /*!< Pointer to the UE in which RB is present */
};

/**********************SEC_CHANGE_START**********************/
/** @brief This structure the security information
 *         sent from the PDCP Layer to CL;
 *   Sumanth : Added Structure from CL for Off - Board functions.
 *  */
typedef struct pjMsSecInfo
{
    U32  rbId;
    U8   rbType;
    U8  dir;
    U8  hash_key[PJ_MS_KENB_LEN];
    U8  strS[PJ_MS_STR_S_LEN];
    U8 count[4];
    U8 fresh[4];
    U8 *pOutBuf;
    
}PjMsSecInfo;
/**********************SEC_CHANGE_END**********************/

#endif /* KW_PDCP */

/** @struct rbCb 
RB control block */
typedef struct rbCb
{
   KwRbCb      kwRbCb;                      /*!< RLC RbCb */
#ifdef KW_PDCP
   PjRbCb      pjRbCb;                      /*!< PDCP RbCb */
   U8          rbCfgStat;                   /*!< RB Config state - to indicate 
                                                 if RLC/PDCP RB is configured.
                                                 1 - PJ_CFG_PDCP
                                                 2 - PJ_CFG_RLC */
#endif /* KW_PDCP */
}RbCb;

/** @struct KwLch
Logical channel mapping with RbCb */
typedef struct kwLch
{
   KwRbCb *ulRbCb;                          /*!< Pointer to Uplink RbCb */
   KwRbCb *dlRbCb;                          /*!< Pointer to Downlink RbCb */
}KwLch;                                  

/** @struct KwCellCb
Cell control block */
typedef struct kwCellCb
{
   CmHashListEnt cellHlEnt;                 /*!< Hash list entry for CellCb */
   CmLteCellId   cellId;                    /*!< Cell Id */
   RbCb          *rbCb[KW_MAX_RB_PER_CELL]; /*!< RbCbs within a Cell */
   KwLch         lCh[KW_MAX_LCH_PER_CELL];  /*!< Array of Logical channels */

}KwCellCb;

#ifdef KW_PDCP
/** @brief SDU Confirmation Structure from RLC */
typedef struct pjDatCfm
{
   CmLteRlcId        rlcId;                 /*!< RLC identifier */
   U16               numSdu;                /*!< Number of SDUs */
   U32               sduId[PJ_MAX_DAT_CFM]; /*!< SDU Id Array */
}PjDatCfm;

#endif /* KW_PDCP */

typedef struct kwUeKey
{
   CmLteRnti     ueId;                      /*!< UE Id */
   CmLteCellId   cellId;                    /*!< Cell Id */
}KwUeKey;

/** @struct KwUeCb
UE control block */
struct kwUeCb
{
   CmHashListEnt ueHlEnt;                   /*!< Hash list entry for UeCb */
   KwUeKey       key;                       /*!< Hash list key for UeCb */
   RbCb          *srbCb[KW_MAX_SRB_PER_UE]; /*!< SRB RbCbs within a UE */ 
   RbCb          *drbCb[KW_MAX_DRB_PER_UE]; /*!< DRB RbCbs within a UE */ 
   KwLch         lCh[KW_MAX_LCH_PER_UE];    /*!< Array of Logical channels */
#ifdef KW_PDCP
   PjSec         secInfo;                   /*!< Security Info */
   PjHoInfo      *hoInfo;                   /*!< SDU Status Confirm */
   PjLibInfo     libInfo;                   /*!< Hook Info - to wait for
                                                 Init Cfm */
#endif /* KW_PDCP */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   U16           numActRb[LKW_MAX_QCI];     /*!< number of RBs Active */
#endif /* LTE_L2_MEAS */
   /*Fix for ccpu00121585*/
   Bool          isUeIdChngDone;            /*!< Flag to indicate whether UeId
                                                 change has been done or not*/
};

/** @struct KwCkwSapCb
CKW Sap Conrol Block */
typedef struct kwCkwSapCb
{
   Pst           pst;                       /*!< Service user post structure */
   SpId          spId;                      /*!< Service provider Id */
   SuId          suId;                      /*!< Service user Id */
   U8            state;                     /*!< Sap Status */
   KwCkwCntSts   sts;                       /*!< Statistics */
}KwCkwSapCb;

/** @struct KwKwuSapCb
KWU Sap Conrol Block */
typedef struct kwKwuSapCb
{
   Pst           pst;                       /*!< Service user post structure */
   SpId          spId;                      /*!< Service provider Id */
   SuId          suId;                      /*!< Service user Id */
   U8            state;                     /*!< Sap Status */
   KwKwuSapSts   sts;                       /*!< Statistics */
}KwKwuSapCb;

/** @struct KwRguSapCb
RGU Sap Control Block */
typedef struct kwRguSapCb
{
   Pst           pst;                       /*!< Service user post structure */
   SpId          spId;                      /*!< Service provider Id */
   SuId          suId;                      /*!< Service user Id */
   U8            state;                     /*!< Sap Status */
   CmTimer       bndTmr;                    /*!< Bind Timer */
   U16           bndTmrInt;                 /*!< Timer Interval */
   U8            retryCnt;                  /*!< Bind Retry Count */
}KwRguSapCb;

#ifdef KW_PDCP
/** @struct KwCpjSapCb
 * CPJ Sap Control Block */
typedef struct kwCpjSapCb
{
   Pst           pst;                       /*!< Service user post structure */
   SpId          spId;                      /*!< Service provider Id */
   SuId          suId;                      /*!< Service user Id */
   U8            state;                     /*!< Sap Status */
}KwCpjSapCb;

/** @struct KwPjuSapCb
 * PJU Sap Control Block */
typedef struct kwPjuSapCb
{
   Pst           pst;                       /*!< Service user post structure */
   SpId          spId;                      /*!< Service provider Id */
   SuId          suId;                      /*!< Service user Id */
   U8            state;                     /*!< Sap Status */
   PjPjuSts      sts;                       /*!< Statistics */
}KwPjuSapCb;
#endif /* KW_PDCP */
/** @struct KwCb
RLC global control block */
typedef struct _kwCb
{
   TskInit       init;                      /*!< Task Initialization Info */
   KwGenCfg      genCfg;                    /*!< General Configuration Structure */
   KwCkwSapCb    ckwSap;                    /*!< CKW Sap Conrol Block */ 
   U8            maxKwuSaps;                /*!< Number of RLC Data Saps */
   KwKwuSapCb    *kwuSap;                   /*!< KWU Sap Control Block */
   KwRguSapCb    rguSap;                    /*!< RGU Sap Control Block */
   KwGenSts      genSts;                    /*!< General Statistics */
   S16           trcLen;                    /*!< Trace Length */
   U8            trcMask;                   /*!< Trace Mask */
   CmTqType      kwTq[KW_TMR_LEN];          /*!< Timer queue */
   CmTqCp        kwTqCp;                    /*!< Timer queue control point */
   CmHashListCp  cellLstCp;                 /*!< Hashlist of CellCb */
   CmHashListCp  ueLstCp;                   /*!< Hashlist of UeCb */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   KwL2Cb        kwL2Cb;                    /*!< Control Block for L2 Measurements in RLC */
#endif /* LTE_L2_MEAS */
#ifdef KW_PDCP
   TskInit       pjInit;                    /*!< Task Initialization Info */
   PjGenCfg      pjGenCfg;                  /*!< PDCP General Configuration Structure */
   KwCpjSapCb    cpjSap;                    /*!< CPJ Sap Conrol Block */ 
   KwPjuSapCb    pjuSap[PJ_MAX_PJUSAPS];    /*!< PJU Sap Conrol Block */ 
   PjGenSts      pjGenSts;                  /*!< PDCP General Statistics */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   PjL2Cb        pjL2Cb;                    /*!< Control block of L2 Measurements in PDCP */
#endif /* LTE_L2_MEAS */
#endif /* KW_PDCP */
/* kw006.201 optimization, fix for ccpu00111716 */
#ifdef KW_BG_DL_PROC
   CmLListCp     statRbLst;                 /*!< If rbCb contains status pdus, then RLC
                                                 will add rbCb in this list */
   Buffer        *mBufDl;                   /*!< Buffer for self post message */
#endif /* KW_BG_DL_PROC */
/* kw006.201 optimization, fix for ccpu00117290 */
#ifdef KW_BG_UL_PROC
   CmLListCp     reasmblyRbList;            /*!< RB list with to be re-assembled PDUs */
   Buffer        *mBufUl;                   /*!< Buffer for self post message */
#ifdef MSPD_RLC_MULTICORE
   SLockId       ulLock;
   SLockId       selfPstUlLock;
   SLockId       kwLock;
#else
   U8            kwLock;
#endif /* MSPD_RLC_MULTICORE */
#endif /* KW_BG_UL_PROC */
   U32       numDlUmSdus;                  /*!< Current number of DL UM SDUs */
}KwCb;

/* Extern variable declaration */
EXTERN KwCb kwCb;                           /*!< RLC global control block */
/* kw006.201 optimization, fix for ccpu00111716 */
#ifdef KW_BG_DL_PROC
EXTERN Pst selfPstDl;                      /*!< Pst structure to post the message to 
                                                rlc itself for Acked PDU deletion */
#endif /* KW_BG_DL_PROC */
/* kw006.201 optimization, fix for ccpu00117290 */
#ifdef KW_BG_UL_PROC
EXTERN Pst selfPstUl;                      /*!< Pst structure to post the message to 
                                                rlc itself for reassembly */
#endif /* KW_BG_UL_PROC */
/****************************************************************************
 *                      EXTERN Declarations
 ***************************************************************************/
EXTERN S16 kwGetSId ARGS((SystemId *s));
/* TMM Module */
EXTERN PUBLIC S16 kwTmmQSdu ARGS ((KwRbCb *rbCb,KwuDatReqInfo *datReqInfo,
                                   Buffer *mBuf));
EXTERN PUBLIC S16 kwTmmSndToLi ARGS ((KwRbCb *rbCb,RguCStaIndInfo *staInd));
#ifdef CCPU_OPT
EXTERN PUBLIC S16 kwTmmRcvFrmLi ARGS ((KwRbCb *rbCb,CmLteRnti tCrnti,
                                       Buffer *pdu));
#else /* CCPU_OPT */
EXTERN PUBLIC S16 kwTmmRcvFrmLi ARGS ((KwRbCb *rbCb,Buffer *pdu));
#endif /* CCPU_OPT */
EXTERN PUBLIC S16 kwTmmReEstablish ARGS ((KwRbCb *rbCb));

/* UMM Module */
EXTERN PUBLIC S16 kwUmmQSdu ARGS ((KwRbCb *rbCb,U32 sduId,Buffer *mBuf));
EXTERN PUBLIC S16 kwUmmProcessSdus ARGS ((KwRbCb *rbCb, KwDatReq *datReq));
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
EXTERN S16 kwUmmProcessPdus ARGS((KwRbCb  *rbCb, KwPduInfo *pduInfo,
      KwL2MeasUlThrpTimInfo *ulTimeInfo));
#else /* LTE_RLC_R9 */
EXTERN PUBLIC S16 kwUmmProcessPdus ARGS (( KwRbCb *rbCb,KwPduInfo *pduInfo));
#endif /* LTE_RLC_R9 */
#else /* LTE_L2_MEAS */
EXTERN PUBLIC S16 kwUmmProcessPdus ARGS (( KwRbCb *rbCb,KwPduInfo *pduInfo));
#endif /* LTE_L2_MEAS */
EXTERN PUBLIC S16 kwUmmReEstablish ARGS ((CmLteRlcId rlcId, KwRbCb *rbCb));
EXTERN PUBLIC S16 kwUmmDiscSdu ARGS ((KwRbCb *rbCb,U32 sduId));
EXTERN PUBLIC S16 kwUmmReOrdTmrExp ARGS((KwRbCb  *rbCb));
/* kw006.201, CR ccpu00117290 */
#ifdef KW_BG_UL_PROC
EXTERN Void kwUmmProcessSelfMsgUl ARGS ((KwRbCb *rbCb, U16 *numPdus));
EXTERN Void kwUmmChkReasmblyLstSz ARGS ((KwRbCb *rbCb));
#endif /* KW_BG_UL_PROC */

/* Utility Module */
EXTERN PUBLIC S16 kwUtlRmvSdu ARGS ((CmLListCp *sduQ,KwSdu *sdu,
                                     CmLList *firstNode));
EXTERN PUBLIC S16 kwUtlSndDatInd ARGS ((KwRbCb *rbCb, Buffer *sdu));
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
EXTERN PUBLIC S16 kwUtlEmptySduQ ARGS ((KwRbCb *rbCb, CmLListCp *sduQ));
#else /* LTE_L2_MEAS */
EXTERN PUBLIC S16 kwUtlEmptySduQ ARGS ((CmLListCp *sduQ));
#endif /* LTE_L2_MEAS */
EXTERN S16 kwUtlEmptySduQInSlowPath ARGS ((CmLListCp *sduQ, U8 *numOfTxReTxCnt));
EXTERN PUBLIC S16 kwUtlSndDStaRsp ARGS ((KwRbCb *rbCb,S32 bo,S32 estHdrSz,
                                         Bool staPduPrsnt));
EXTERN PUBLIC S16 kwUtlSndCStaRsp ARGS ((KwRbCb *rbCb,S32 bo));
EXTERN PUBLIC S16 kwUtlSndToLi ARGS ((KwDStaIndInfo *staIndInfo));
EXTERN PUBLIC S16 kwUtlRcvFrmLi ARGS ((KwDatIndInfo *datIndInfo));
EXTERN PUBLIC S16 kwUtlShutDown ARGS ((void));
EXTERN PUBLIC S16 kwUtlCalcLiForSdu ARGS ((U16 numLi,MsgLen msgLen,S16 *pduSz));
EXTERN Void kwUtlFreeRbCb ARGS (( KwRbCb *rbCb));
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
EXTERN S16 kwUtlHdlL2TmrExp   ARGS (( KwL2MeasEvtCb *measEvtCb));
EXTERN S16 kwUtlSndL2MeasCfm  ARGS (( KwL2MeasEvtCb *measEvtCb));
EXTERN S16 kwUtlSndL2MeasNCfm ARGS (( KwL2MeasReqEvt *measReqEvt, 
                                      KwL2MeasCfmEvt *measCfmEvt)) ;
EXTERN S16 kwUtlSndDiscSduCfm ARGS (( KwRbCb *kwRbCb, U32    sduId));
EXTERN S16 kwUtlUpdSduSnMap   ARGS (( KwRbCb *kwRbCb, KwSdu  *sdu, 
                                      KwDatReq *kwDatReq, Bool   lastSegSent));
EXTERN S16 kwUtlProcHarqInd  ARGS (( RguHarqStatusInd *staInd, KwUeCb *ueCb, 
                                     U8 tbIdx));
/* kw006.201 */
EXTERN Void kwUtlProcInitialActvUe ARGS ((KwL2MeasCb *measCb));
#ifdef LTE_RLC_R9
EXTERN S16 kwUtlValidateL2Meas ARGS((KwL2MeasReqEvt *measReqEvt, 
                                     KwL2MeasCfmEvt *measCfmEvt, 
                                     CmLteLcId *lChId, U8 *numLCh));
#else /* LTE_RLC_R9 */
EXTERN S16 kwUtlValidateL2Meas ARGS((KwL2MeasReqEvt *measReqEvt,
                                     KwL2MeasCfmEvt *measCfmEvt));
#endif /* LTE_RLC_R9 */
#ifdef LTE_RLC_R9
/* kw006.201 */
EXTERN Void kwUtlResetKwRbL2Meas ARGS(( KwRbCb  *rbCb ));
EXTERN S16 kwUtlNotifyMacUlIp ARGS ((KwL2MeasCb *measCb, Bool enable,
                                    CmLteLcId  *lChId, U8 *numLCh));
#endif /* LTE_RLC_R9 */
#endif /*  LTE_L2_MEAS */
/* kw006.201, CR ccpu00117290 and ccpu00111716 */
#ifdef KW_BG_UL_PROC
EXTERN Void kwUtlProcessSelfMsgUl ARGS ((Void));
EXTERN Void kwUtlPostSelfMsgUl ARGS ((U8 coreId));
#endif /* KW_BG_UL_PROC */
#if ((defined KW_BG_DL_PROC) || (defined KW_BG_UL_PROC))
EXTERN S16 kwUtlSendRbReEstCfm ARGS ((KwRbCb *rbCb));
EXTERN S16 kwUtlRbDeletion ARGS ((KwRbCb *rbCb));
#endif /* ((defined KW_BG_DL_PROC) || (defined KW_BG_UL_PROC)) */
EXTERN Void kwUtlSetReestInProgress(KwUeCb *ueCb);
/* TMR Module */
EXTERN S16 kwStartTmr       ARGS(( PTR cb, S16 tmrEvnt ));
EXTERN S16 kwStopTmr        ARGS(( PTR cb, U8 tmrType ));
EXTERN S16 kwTmrExpiry      ARGS(( PTR cb, S16 tmrEvnt ));
EXTERN Bool kwChkTmr        ARGS(( PTR cb, S16 tmrEvnt ));

/* AMM Module */
EXTERN S16 kwAmmQSdu            ARGS(( KwRbCb *rbCb, Buffer *mBuf, U32 sduId ));
EXTERN S16 kwAmmProcessSdus     ARGS(( KwRbCb *rbCb, KwDatReq *kwDatReq ));
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef LTE_RLC_R9
EXTERN S16 kwAmmProcessPdus ARGS((KwRbCb  *rbCb, KwPduInfo  *pduInfo,
KwL2MeasUlThrpTimInfo *ulTimeInfo));
#else /* LTE_RLC_R9 */
EXTERN S16 kwAmmProcessPdus     ARGS(( KwRbCb *rbCb, KwPduInfo *pduInfo));
#endif /* LTE_RLC_R9 */
#else /* LTE_L2_MEAS */
EXTERN S16 kwAmmProcessPdus     ARGS(( KwRbCb *rbCb, KwPduInfo *pduInfo));
#endif /* LTE_L2_MEAS */
EXTERN S16 kwAmmDiscSdu         ARGS(( KwRbCb *rbCb, U32 sduId));
EXTERN S16 kwAmmReEstablish     ARGS(( CmLteRlcId rlcId, KwRbCb *rbCb));
EXTERN Void kwAmmReOrdTmrExp    ARGS(( KwRbCb *rbCb));
EXTERN Void kwAmmStaProTmrExp   ARGS(( KwRbCb *rbCb));
EXTERN Void kwAmmPollRetxTmrExp ARGS(( KwRbCb *rbCb));

/* kw006.201, CR ccpu00111716 */
#ifdef KW_BG_DL_PROC
EXTERN Void kwAmmRlsPdusSet ARGS((KwRbCb *rbCb, U8 *numOfTxReTxCnt));
EXTERN Void kwAmmRlsPdusSetOnRbDel ARGS((KwRbCb *rbCb));
EXTERN Void kwAmmProcessSelfMsgDl ARGS((Void));
EXTERN Void kwUtlGetNxtRetx  ARGS((CmLListCp lst, KwRetx **retx));
EXTERN Void kwAmmChkRlsReTxLstSz ARGS ((KwRbCb *rbCb));
#endif /* KW_BG_DL_PROC */
/* kw006.201, CR ccpu00117290 */
#ifdef KW_BG_UL_PROC
EXTERN Void kwAmmProcessSelfMsgUl ARGS ((KwRbCb *rbCb, U16 *numPdus));
EXTERN Void kwAmmChkReasmblyLstSz ARGS ((KwRbCb *rbCb));
#endif /* KW_BG_UL_PROC */
/* CFG Module */
EXTERN S16 kwCfgAddRb ARGS (( CmLteRnti ueId, CmLteCellId cellId, 
                              CkwEntCfgInfo *entCfg, CkwEntCfgCfmInfo *entCfm));
EXTERN S16 kwCfgReCfgRb ARGS (( CmLteRnti ueId, CmLteCellId cellId, 
                              CkwEntCfgInfo *entCfg, CkwEntCfgCfmInfo *entCfm));
EXTERN S16 kwCfgDelRb ARGS (( CmLteRnti ueId, CmLteCellId cellId, 
                              CkwEntCfgInfo *entCfg, CkwEntCfgCfmInfo *entCfm));
EXTERN S16 kwCfgDelUe ARGS (( CmLteRnti ueId, CmLteCellId cellId, 
                              CkwEntCfgInfo *entCfg, CkwEntCfgCfmInfo *entCfm));
EXTERN S16 kwCfgReEstRb ARGS (( CmLteRnti ueId, CmLteCellId cellId, 
                              CkwEntCfgInfo *entCfg, CkwEntCfgCfmInfo *entCfm));
EXTERN S16 kwCfgDelCell ARGS (( CmLteCellId cellId, CkwEntCfgInfo *entCfg, 
                                CkwEntCfgCfmInfo *entCfm));
EXTERN S16 kwCfgUeIdChng ARGS (( CkwUeInfo *ueInfo, CkwUeInfo *newUeInfo, 
                                 CmStatus  *status));

/* LMM Module */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
EXTERN Void kwLmmSendAlarm ARGS (( U16 category, U16 event, U16 cause, 
                                   SuId suId, U32 ueId, U8 qci));
EXTERN S16 KwMiLkwL2MeasReq ARGS (( Pst *pst, KwL2MeasReqEvt *measReqEvt ));
#else /* LTE_L2_MEAS */
EXTERN Void kwLmmSendAlarm ARGS (( U16 category, U16 event, U16 cause, 
                                   SuId suId, U32 ueId));
#endif /* LTE_L2_MEAS */
EXTERN S16 kwLmmSendTrc ARGS (( Event event, Buffer *mBuf));

/* DBM Module */
EXTERN S16 kwDbmInit ARGS ((Void));

EXTERN S16 kwDbmDeInit ARGS ((Void));

EXTERN S16 kwDbmCreateUeCb ARGS ((
CmLteRnti         ueId,       /* UE Identifier */
CmLteCellId       cellId,     /* Cell Identifier */
KwUeCb            **ueCb       /* UE Control Block */
));

EXTERN S16 kwDbmFetchUeCb ARGS ((
CmLteRnti         ueId,       /* UE Identifier */
CmLteCellId       cellId,     /* Cell Identifier */
KwUeCb            **ueCb       /* UE Control Block */
));

EXTERN S16 kwDbmDelUeCb ARGS ((
KwUeCb            *ueCb,       /* UE Identifier */
Bool              abortFlag    /* Abort Flag */
));

EXTERN S16 kwDbmDelAllUe ARGS ((Void));

EXTERN S16 kwDbmCreateCellCb ARGS ((
CmLteCellId       cellId,     /* Cell Identifier */
KwCellCb          **cellCb     /* UE Control Block */
));

EXTERN S16 kwDbmFetchCellCb ARGS ((
CmLteCellId       cellId,     /* Cell Identifier */
KwCellCb          **cellCb     /* Cell Control Block */
));

EXTERN S16 kwDbmDelCellCb ARGS ((
KwCellCb          *cellCb     /* Cell Control Block */
));

EXTERN S16 kwDbmDelAllCell ARGS ((Void));

EXTERN S16 kwDbmFetchRbCbForUi ARGS ((
CmLteRlcId        rlcId,      /* RLC Identifier */
KwRbCb            **rbCb       /* RB Cb */
));

EXTERN S16 kwDbmFetchRbCbForLi ARGS ((
CmLteRnti         ueId,       /* UE Identifier */
CmLteCellId       cellId,     /* Cell Identifier */
CmLteLcId         lcId,       /* Logical channel instance */
U8                dir,        /* Direciotn DL/UL */
KwRbCb            **rbCb      /* RB Cb */
));

EXTERN S16 kwDbmDelAllRb ARGS ((
RbCb            **rbCbLst,  /* RB Cb list */
U8                numRbCb     /* Number of rbCbs */
));

EXTERN S16 kwDbmShutdown ARGS ((Void));

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
EXTERN S16 kwDbmDelMeasEvtCb ARGS((Void));
#endif /* LTE_L2_MEAS */

/* Activation functions */
EXTERN S16 kwActvInit ARGS ((
Ent    ent,                 /* entity */
Inst   inst,                /* instance */
Region region,              /* region */
Reason reason               /* reason */
));

EXTERN S16 kwActvTsk ARGS ((
Pst *pst,              /* pst structure */
Buffer *mBuf            /* message buffer */
));


#ifdef KW_PDCP
/* Activation functions */
EXTERN S16 pjActvInit ARGS ((
Ent    ent,                 /* entity */
Inst   inst,                /* instance */
Region region,              /* region */
Reason reason               /* reason */
));

EXTERN S16 pjActvTsk ARGS ((
Pst *pst,              /* pst structure */
Buffer *mBuf            /* message buffer */
));

/* CFG Module */
EXTERN S16 pjCfgAddRb ARGS ((
CmLteRnti         ueId,
CmLteCellId       cellId,
CpjCfgEnt         *entCfg,
CpjCfmEnt         *entCfm
));

EXTERN S16 pjCfgReCfgRb ARGS ((
CmLteRnti         ueId,
CmLteCellId       cellId,
CpjCfgEnt         *entCfg,
CpjCfmEnt         *entCfm
));

EXTERN S16 pjCfgDelRb ARGS ((
CmLteRnti         ueId,
CmLteCellId       cellId,
CpjCfgEnt         *entCfg,
CpjCfmEnt         *entCfm
));

EXTERN S16 pjCfgReEstRb ARGS ((
CmLteRnti         ueId,
CmLteCellId       cellId,
CpjCfgEnt         *entCfg,
CpjCfmEnt         *entCfm
));

EXTERN S16 pjCfgDelUe ARGS ((
CmLteRnti         ueId,
CmLteCellId       cellId,
CpjCfgEnt         *entCfg,
CpjCfmEnt         *entCfm
));

EXTERN S16 pjCfgPrcSecCfg ARGS ((
CpjSecCfgReqInfo *secCfg,
CpjSecCfgCfmInfo *cfgCfm
));

EXTERN S16 pjCfgUeIdChng ARGS((
CpjUeInfo *ueInfo,
CpjUeInfo *newUeInfo,
CmStatus  *status
));

EXTERN S16 pjCfgPrcCount ARGS ((
CpjCountReqInfo       *countReq,
CpjCountCfmInfo       *countCfm
));

EXTERN S16 pjCfgPrcSduSta ARGS((
CpjSduStaReqInfo *staReq
));


EXTERN S16  pjCfgDelPjRBCb ARGS ((
KwUeCb               *ueCb,
RbCb                 *rbCb
));

EXTERN  S16 pjCfgReEstReq ARGS ((
CpjReEstReqInfo *reEstReq,
CpjReEstCfmInfo   *reEstCfm
));

/* DL Module */
EXTERN S16 pjDlmProcessSdus ARGS ((
PjRbCb *pjRbCb, 
Buffer *sdu,
U32    sduId,                 
PjSn   sn,                     
U32    txHfn 
));
EXTERN S16 pjDlmProcDrb ARGS ((
PjRbCb  *pjRbCb,
PjTxEnt *txEnt
));
EXTERN S16 pjDlmProcSrb ARGS ((
PjRbCb  *pjRbCb,
PjTxEnt *txEnt
));
EXTERN Void pjDlmReEstSrb ARGS ((
PjRbCb *pjRbCb
));

EXTERN S16 pjDlmReEstDrbUm ARGS ((
PjRbCb *pjRbCb
));

EXTERN S16 pjDlmReEstDrbAm ARGS ((
PjRbCb *pjRbCb
));
EXTERN S16 pjDlmProcessCfm ARGS ((
PjRbCb        *pjRbCb,
PjDatCfm      *pjDatCfm,
U8            cfmType
));
EXTERN S16 pjDlmHndlStaRep ARGS ((
PjRbCb   *pjRbCb,
PjSn     fms,
Buffer   *staPdu
));
EXTERN S16 pjDlmReEstHoDrbAm ARGS ((
PjRbCb *pjRbCb
));
EXTERN S16 pjDlmHndlDatFwdReq ARGS ((
PjRbCb  *pjRbCb,
PjuDatFwdReqInfo *datFwdReq
));
EXTERN S16 pjDlmDeliverPdu ARGS ((
PjRbCb     *pjRbCb,
PjTxEnt    *txEnt
));
EXTERN S16 pjDlmObdTmrExp ARGS ((
PjRbCb     *pjRbCb
));
EXTERN  S16 pjDlmHdlIntProt ARGS ((
PjRbCb  *pjRbCb,
PjTxEnt *txEnt
));
EXTERN S16 pjDlmHdlCmp ARGS (( 
PjRbCb  *pjRbCb,
PjTxEnt *txEnt
));
EXTERN S16 pjDlmHdlCiph ARGS ((
PjRbCb  *pjRbCb,
PjTxEnt *txEnt
));
EXTERN S16 pjDlmDiscTmrExp ARGS ((
PjRbCb  *pjRbCb
));

/* UL Module */
EXTERN S16 pjUlmHdlSrbPkt ARGS((
PjRbCb *pjRbCb,               /* !< PDCP Control Block */  
Buffer *pdu                   /* !< PDU Buffer */ 
));
EXTERN S16 pjUlmHdlDrbPkt ARGS((
PjRbCb *pjRbCb,               /* !< PDCP Control Block */      
Buffer *pdu                   /* !< PDU Buffer */ 
));
EXTERN S16 pjUlmProcessSrb ARGS((
PjRbCb   *pjRbCb,           /* !< PDCP Control Block */   
PjSn     sn,               /* !< SN value of PDU */           
Buffer   *pdu              /* !< PDU message buffer */           
));
EXTERN S16 pjUlmProcessDrbUm ARGS((
PjRbCb   *pjRbCb,          /* !< PDCP Control Block */   
PjSn     sn,               /* !< SN value of PDU */           
Buffer   *pdu              /* !< PDU message buffer */           
));
EXTERN S16 pjUlmProcessDrbAm ARGS((
PjRbCb   *pjRbCb,           /* !< PDCP Control Block */   
PjSn     sn,               /* !< SN value of PDU */           
Buffer   *pdu              /* !< PDU message buffer */           
));
EXTERN S16 pjUlmHdlDecipher ARGS((
PjRbCb     *pjRbCb,        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt          /* !< Rx Entry */
));
EXTERN S16 pjUlmHdlIntVer ARGS((
PjRbCb     *pjRbCb,        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt          /* !< Rx Entry */
));
EXTERN S16 pjUlmHdlDeCmp ARGS((
PjRbCb     *pjRbCb,        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt          /* !< Rx Entry */
));
EXTERN S16 pjUlmDeliverSrb ARGS((
PjRbCb *pjRbCb,          /* !< PDCP Control Block */     
PjRxEnt *rxEnt           /* !< Recption Buffer Entry   */            
));
EXTERN S16 pjUlmDeliverDrbUm ARGS((
PjRbCb *pjRbCb,        /* !< PDCP Control Block */                  
PjRxEnt *rxEnt         /* !< Recption Buffer Entry   */             
));
EXTERN S16 pjUlmDeliverDrbAm ARGS((
PjRbCb *pjRbCb,           /* !< PDCP Control Block */                  
PjRxEnt *rxEnt            /* !< Recption Buffer Entry   */             
));
EXTERN S16 pjUlmReEstSrb ARGS((
PjRbCb *pjRbCb       /*!< PDCP control block  */
));
EXTERN S16 pjUlmReEstDrbAm ARGS((
PjRbCb *pjRbCb       /*!< PDCP control block  */
));
EXTERN S16 pjUlmBldStaRep ARGS((
PjRbCb   *pjRbCb,             /* !< PDCP Control Block */  
PjSn      fms,                /* !< First Missing SN */  
Buffer   *staRep              /* !< Status Report */       
));
EXTERN S16 pjUlmHdlDatFwdReq ARGS((
PjRbCb         *pjRbCb,                       /* !< PDCP ID */        
PjuDatFwdReqInfo   *datFwdReq                    /* !< DatFwdReq Info */
));                                             
EXTERN S16 pjUlmSubmitForDecmp ARGS((
PjRbCb     *pjRbCb,        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt          /* Ptr To Rx Entry */
));

/*kw006.201 replaced macro with function */
EXTERN Void pjUlmGetSubCnt ARGS((
PjRbCb *pjRbCb,    /* !< PDCP Control Block */  
U32    *subPdu      /* Count that was/will be submitted */
));
EXTERN Void pjUlmGetDrbAmSubCnt ARGS((
PjRbCb *pjRbCb,    /* !< PDCP Control Block */  
U32    *nxtSubCnt  /* !<Count that was/will be submitted */
));
EXTERN Void pjUlmDeliverPdu ARGS((
PjRbCb *pjRbCb,    /* !< PDCP Control Block */  
PjRxEnt *rxEnt    /* Ptr To Rx Entry */
));

EXTERN Void pjUlmDeliverDrb ARGS((
PjRbCb *pjRbCb,    /* !< PDCP Control Block */  
PjRxEnt *rxEnt     /* Ptr To Rx Entry */
));
EXTERN Void pjUlmDecReEstCnt ARGS((
PjRbCb *pjRbCb    /* !< PDCP Control Block */  
));
EXTERN Void pjUlmChkRdyToDeliver ARGS((
PjRbCb *pjRbCb    /* !< PDCP Control Block */  
));
EXTERN Void pjUlmSndPjuDatInd ARGS((
PjRbCb *pjRbCb,    /* !< PDCP Control Block */  
PjRxEnt  *rxEnt    /* !<Ptr to Rx Entry     */
));
EXTERN Void pjUlmSndPjuStaInd ARGS((
PjRbCb *pjRbCb,    /* !< PDCP Control Block */  
PjRxEnt  *rxEnt    /* !<Ptr to Rx Entry     */
));
EXTERN S16 pjUtlSaveCfmInfo  ARGS((
KwUeCb      *ueCb,           /* Pointer to UeCb */ 
U8          cfmType,         /* Confirm type */
Bool        startTmr,        /* Whether to start timer */
U8          entity,          /* Confirm type */
PTR         cfmPtr,          /* Pointer to the structure for confirm */
PTR         cfgPtr           /* Pointer to the structure for request */
));
EXTERN S16 pjUtlHdlObdInitCfm ARGS((
KwUeCb      *ueCb,           /* Pointer to UeCb */ 
U16         txIdx,           /* Transaction Index for UeCb */
U8          cfmType,         /* Confirm type */
U8          maskVal          /* mask value */
));
EXTERN S16 pjUtlHdlInitObdTmrExp ARGS((
KwUeCb *ueCb
));
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
EXTERN S16 pjUtlHdlL2TmrExp ARGS((
PjL2MeasEvtCb *measEvtCb
));
EXTERN S16 pjUtlSndL2MeasNCfm ARGS((
PjL2MeasReqEvt  *measReqEvt,
PjL2MeasCfmEvt  *measCfmEvt
));
EXTERN S16 pjUtlValidateL2Meas ARGS((
PjL2MeasReqEvt  *measReqEvt,
PjL2MeasCfmEvt  *measCfmEvt
));
#endif /* LTE_L2_MEAS */
/*kw006.201 replaced macro with function */
EXTERN Void pjUtlPerfErrUpd ARGS((
PjRbCb    *pjRbCb,
U32       count
));
EXTERN Void pjUtlCalNxt2Sub ARGS((
PjUlCb   *ulCb
));
EXTERN Void pjUtlFillReEstCfgCfmInfo ARGS((
CpjCfgCfmInfo *cfmInfo,
PjAsyncCfm    *asyncCfm
));
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
EXTERN Void pjUlmHdlObdTmrExpiry ARGS((
PjRbCb     *pjRbCb         /* !< PDCP Control Block */  
));
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */
EXTERN Void pjUlmHdlErrUpdates ARGS((
PjRbCb     *pjRbCb,        /* !< PDCP Control Block */  
U32         errCnt         /* !< Count of PDU that raises the error */
));

/* DB Module */
EXTERN S16 pjDbmInsTxEnt  ARGS ((
PjBuf       *buf,             
PjTxEnt     *txEnt       
));
EXTERN S16 pjDbmBufInit ARGS ((
PjBuf       *buf,          
U8          numBins          
));
EXTERN PjTxEnt* pjDbmGetTxEnt ARGS ((
PjBuf       *buf,        
U32          count      
));
EXTERN PjTxEnt* pjDbmGetTxEntSn ARGS ((
PjBuf       *buf,      
U16          sn          
));
EXTERN S16 pjDbmDelTxEnt  ARGS ((
PjBuf       *buf,    
U32         count     
));
EXTERN S16 pjDbmTxDelAll ARGS((
PjBuf *buf       
));
EXTERN S16 pjDbmTxDeInit ARGS ((
PjBuf *buf       
));
EXTERN S16 pjDbmInsRxEnt ARGS((
PjBuf       *buf,          /* !< Rx Buffer  */
PjRxEnt     *rxEnt,        /* !< Rx Entry   */              
Bool        dupFlag        /* !< Boolean to check for duplicate  entries */
));
EXTERN PjRxEnt* pjDbmGetRxEnt ARGS((
PjBuf  *buf,       /* !< Rx Buffer  */                               
U32    count       /* !< Count value of the entry to retrieve */                               
));
EXTERN S16 pjDbmDelRxEnt ARGS((
PjBuf  *buf,        /* !< Rx Buffer  */                                                          
U32    count        /* !< Count value of the entry to retrieve */                                
));
EXTERN S16 pjDbmRxDelAll ARGS((
PjBuf *buf        /* !< Rx Buffer  */                                                          
));                 
EXTERN S16 pjDbmRxDeInit ARGS((
PjBuf *buf        /* !< Rx Buffer  */                                                          
));                 

/* TMR Module */
EXTERN S16 pjStartTmr  ARGS ((
PTR       cb,           
S16       tmrEvnt   
));

EXTERN S16 pjStopTmr ARGS ((
PTR    cb,              
U8     tmrType         
));

EXTERN Bool pjChkTmr ARGS ((
PTR       cb,         
S16       tmrEvnt       
));

EXTERN S16 pjTmrExpiry ARGS ((
PTR cb,
S16 tmrEvnt
)); 

/* UTL Module */
EXTERN S16 pjUtlCmpInit ARGS((
PjRbCb   *pjRbCb                  /* PDCP RbCb */
));
EXTERN S16 pjUtlIntInit ARGS((
KwUeCb   *ueCb                   /* UE CB Ptr */
));
EXTERN S16 pjUtlCpInit ARGS((
KwUeCb   *ueCb                    /* UE CB Ptr */
));
EXTERN S16 pjUtlUpInit ARGS((
KwUeCb   *ueCb                    /* UE CB Ptr */
));
EXTERN S16 pjUtlCmpReset ARGS((
PjRbCb      *pjRbCb                 /* Context to be reset for compression */
));
EXTERN S16 pjUtlCmpReq ARGS((
PjRbCb   *pjRbCb,                 /* PDCP RbCb */
Buffer   *mBuf,                   /* SDU to be compressed */
Buffer   **opSdu,                 /* Compressed SDU */
U32      count                    /* COUNT - transaction Id */
));
EXTERN S16 pjUtlIntProtReq ARGS((
PjRbCb   *pjRbCb,                 /* PDCP RbCb */
PjSecInp secInp ,                 /* Input parameters for integrity */ 
Buffer   *mBuf,                   /* SDU to be compressed */
U32      *macI                    /* Message authentication code for the SDU */
));
EXTERN S16 pjUtlCipherReq ARGS((
PjRbCb   *pjRbCb,                 /* PDCP RbCb */
PjSecInp secInp ,                 /* Input parameters for deciphering */ 
Buffer   *mBuf,                   /* Data to be deciphered */
Buffer   **opSdu                  /* Deciphered SDU */
));
EXTERN S16 pjUtlDeCmpReq ARGS((
PjRbCb   *pjRbCb,                 /* PDCP RbCb */
U32      count,                   /* COUNT - transaction Id */
Buffer   *mBuf,                   /* SDU to be compressed */
Buffer   **opSdu                  /* Compressed SDU */
));
EXTERN S16 pjUtlIntVerReq ARGS((
PjRbCb   *pjRbCb,                 /* PDCP RbCb */
PjSecInp secInp ,                 /* Input parameters for integrity */ 
Buffer   *mBuf,                   /* SDU to be compressed */
U32      macI                    /* MAC-I to be verified with */
));                              /* SEC_CHANGE: Removed status parameter */ 
EXTERN S16 pjUtlDecipherReq ARGS((
PjRbCb   *pjRbCb,                 /* PDCP RbCb */
PjSecInp secInp,                  /* Input parameters for deciphering */ 
Buffer   *mBuf,                   /* Data to be deciphered */
Buffer   **opSdu                  /* Deciphered SDU */
));
EXTERN S16 pjUtlSndFeedback ARGS((
PjRbCb   *pjRbCb,                 /* PDCP RbCb */
Buffer   *feedback                /* ROHC Feedback */
));
EXTERN S16 pjUtlCmpClose ARGS((
PTR      cmpCxtId                 /* ROHC Context to be closed */
));
EXTERN S16 pjUtlIntClose ARGS((
PTR      intCxtId                 /* Integration CxtId to be closed */
));
EXTERN S16 pjUtlCipherClose ARGS((
PTR      cpCxtId                  /* Context Id for Ciphering to be closed */
));
EXTERN S16 pjUtlReEstSrb1 ARGS((
PjRbCb      *pjRbCb                    /* PDCP Control Block Pointer */
));
EXTERN S16 pjUtlReEstStart ARGS((
KwUeCb         *ueCb,          /* UE Control Block Pointer */
CpjCfgReqInfo  *cfgReq         /* CfgReq for Reestablishment */
));
EXTERN S16 pjUtlReEstHOStart ARGS((
KwUeCb      *ueCb,           /* Number of RBs undergoing reestablishment*/ 
U32         transId          /* Transaction id of Sdu Status cfm */
));
/*ccpu00121586: Added 2nd argument rbType*/
EXTERN S16 pjUtlReEstReconfig ARGS ((
KwUeCb       *ueCb,          /* UE Control Block */
U8           rbType          /* Rb Type:SRB/DRB*/
));

EXTERN S16 pjUtlReEstDl ARGS((
KwUeCb       *ueCb           /* UE Control Block */ 
));
EXTERN S16 pjUtlSndReEstCfgCfm ARGS((
KwUeCb       *ueCb           /* UE Control Block */ 
));
EXTERN S16 pjUtlSndSduStaCfm ARGS((
KwUeCb       *ueCb           /* UE Control Block */ 
));
EXTERN S16 pjUtlShutdown ARGS((
Void
));
EXTERN Void pjUtlFreeRb ARGS((
PjRbCb *pjRbCb
));
/* kw005.201 added function to send data forward indication mesage */
EXTERN S16 pjUtlSndDatFwdInd ARGS((
PjRbCb           *pjRbCb,
PjuDatFwdIndInfo *datFwdInd
));
/* LI Module */
EXTERN Void pjLimDatInd ARGS((
PjRbCb      *pjRbCb,
Buffer      *mBuf
));

EXTERN Void pjLimDatCfm ARGS((
PjRbCb      *pjRbCb,
PjDatCfm    *datCfm 
));

EXTERN Void pjLimStaInd  ARGS((
PjRbCb      *pjRbCb,
PjDatCfm    *staInd
));

EXTERN Void pjLimReEstCmpInd ARGS((
PjRbCb      *pjRbCb         
));

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
EXTERN S16 pjLimDatAckInd ARGS ((
PjRbCb     *pjRbCb,
U32        count
));
EXTERN S16 pjLimDiscSduCfm ARGS ((
PjRbCb     *pjRbCb,
U32        count
));
#endif /* LTE_L2_MEAS */
/* UI Module */
EXTERN S16 kwUimDatReq ARGS ((
PjRbCb      *pjRbCb,
U32         sduId,
Buffer      *mBuf
));

EXTERN S16 kwUimDiscSdu ARGS ((
PjRbCb     *pjRbCb,
U32        sduId
));

EXTERN RbCb* kwDbmCreateRbCb ARGS ((
U8      rbId,
U8      rbType,
KwUeCb  *ueCb,
U8      rb
));
EXTERN S16 kwDbmFetchPjRbCb ARGS ((
KwUeCb  *ueCb,
U8       rbId,
U8       rbType,
PjRbCb  **pjRbCb 
));
EXTERN  S16 kwDbmDelRbCb ARGS ((
U8      rbId,
U8      rbType,
RbCb   **rbCb,
U8      rb
));

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
EXTERN Void pjLmmSendAlarm ARGS (( U16 category, U16 event, 
U16 cause, SuId suId, U32 ueId, U8 qci));
#else /* LTE_L2_MEAS */
EXTERN Void pjLmmSendAlarm ARGS (( U16 category, U16 event, U16 cause, 
                                   SuId suId, U32 ueId));
#endif /* LTE_L2_MEAS */
EXTERN S16 pjLmmSendTrc ARGS (( Event event, Buffer *mBuf));

#endif /* KW_PDCP */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __KWX__ */

  
/********************************************************************30**
  
         End of file:     kw.x@@/main/2 - Fri Nov 13 14:14:10 2009
  
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
/main/1      ---       nm  1. Initial release.
/main/2      ---       nm  1. LTERLC Release 2.1
/main/3      kw002.201 vp  1. Patch for optimization,fixes for multi-region
                              and retransmission.
/main/4      kw003.201 vp  1. Fixes for re-segmentation, segement offset
                               value and optimization 
/main/5      kw005.201 ap  1. Updated KwRbCb for statistics calculation.
                           2. Removed compilation warning.
                           3. Added cellId and ueId KwL2MeasCb
                           4. Added declaration for kwUtlValidateL2Meas.
/main/6      kw006.201 rd  1. Removed declaration of kwUtlPlcMeasDatInL2Sts. 
                           2. Added the declaration of kwUtlProcInitialActvUe
                              and kwUtlNotifyMacUlIp.
                           3. Replaced macro with function
                       ap  4. Added extern declaration for selfPst for 
                              ccpu00111716.
                       rd  5. Added extern declaration for selfPstUl for
                              ccpu00117290.
                           6. Added extern declarations for 3 functions
                              kwAmmProcessSelfMsgUl, kwUmmProcessSelfMsgUl
                              and kwUtlProcessSelfMsgUl for ccpu00117290.
                           7. Added flag rbOp in KwRbCb for ccpu00117290
                              and ccpu00111716
                           8. Added RbCb* in KwRbCb.
                           9. Added mBufDl and mBufUl in KwCb for DL/UL
                              optimization.
                          10. Fix done for ccpu00121585: Flag "isUeIdChngDone"
                              has been added in kwUeCb.
                          11. ccpu00121586: Added 2nd argument in function
                              pjUtlReEstReconfig.
                          12. Added "numSduProcessed" in KwUmHdr. It was added
                              for controlling the number of LIs to be pushed to
                              PDCP in UL per self post message.
                          13. Moved selfPstDl and selfPstUl to KwCb.
*********************************************************************91*/
