/**
 * @file    ueh_sib.h
 * @brief This file contains the Special values of SIB
 */
#ifndef   UEH_SIB_H
#define   UEH_SIB_H

#include "ccpu_cmn.h"

/* RRM related includes */
#include "ueh_eut.h"
#include "lueh.h"
#include "lueh.x"
#include "ueh_eut_x.h"
#include "ueh.h"
#include "s1ap_cmn_proc.h"



#define SCHEDULING_INFO_PERIOD_ARRAY_LEN                7
extern U16     schInfpPeriod[SCHEDULING_INFO_PERIOD_ARRAY_LEN];

#define SPECIAL_VALUE_SIB1_PMAX                         65
#define SPECIAL_VALUE_SIB1_QRXLEVEL_MIN_OFFSET           0
#define SPECIAL_VALUE_SIB3_S_INTRA_SEARCH               32
#define SPECIAL_VALUE_SIB3_PMAX                         34

#define SPECAIL_VALUE_SIB2_UL_EARFCN                    65536


EnumNhuMasterInfoBlockdl_Bandwidth uehGetDlBwInDrbEnumForOamVal(
     DlBandwidth dbBw
    );
/**     SIB2 **/
PUBLIC S16 uehFillSib2FreqInfo(
    NhuSysInfoBlockTyp2 *sib2
  );
PUBLIC S16 uehFillSib2TimersAndConst(
    NhuSysInfoBlockTyp2 *sib2
  );
PUBLIC S16 uehFillSib2AcBarringInfo(
  Ptr                 *evnt,
  NhuSysInfoBlockTyp2 *sib2
  );
EXTERN S16 uehFillMibMsg(
  Ptr                *evnt,
  NhuMasterInfoBlock *mib,
  NhuTimingInfo      *timingInfo
  );
EXTERN S16 uehFillCmnSib1(
  NhuDatReqSdus          *nhuDatReqEvntSdu
  );
EXTERN S16 uehFillSib1Msg(
  Ptr                 *evnt,
  NhuSysInfoBlockTyp1 *sib1
  );
EXTERN S16 uehFillCmnSib2(
    Ptr                      *evnt,
    NhuBCCH_DL_SCH_Msg       *bcchSchMsg
  );
EXTERN S16 uehFillSib2Msg(
  Ptr                     *evnt,
  NhuSysInfoBlockTyp2     *sib2
  );
EXTERN S16 uehFillSib3Msg(NhuDatReqSdus *nhDatReqEvntSdu);
EXTERN S16 uehFillSib4Msg(NhuDatReqSdus *nhDatReqEvntSdu);
EXTERN S16 veSibBldSIB5Msg(NhuDatReqSdus *nhDatReqEvntSdu);

/* Paging */
PUBLIC S16 uehPagHdlTmrInd(RgrTtiIndInfo *tmngInfo);

PUBLIC S16 uehFillSib1SchdInfoLst(Ptr *evnt,NhuSysInfoBlockTyp1 *sibTyp1);
void uehFillSib1CellSelectionInfo(NhuSysInfoBlockTyp1 *sibTyp1);
PUBLIC S16 uehFillSib1CellAccRelInfo(Ptr *evnt,NhuSysInfoBlockTyp1 *sibTyp1);
PUBLIC S16 uehFillSib2RachCfg(NhuSysInfoBlockTyp2 *sib2);
PUBLIC S16 uehFillSib2BcchCfg(NhuSysInfoBlockTyp2 *sib2);
PUBLIC S16 uehFillSib2PcchCfg(NhuSysInfoBlockTyp2 *sib2);
PUBLIC S16 uehFillSib2PrachCfg(NhuSysInfoBlockTyp2 *sib2);
PUBLIC S16 uehFillSib2PdschCfg(NhuSysInfoBlockTyp2 *sib2);
PUBLIC S16 uehFillSib2PuschCfg(NhuSysInfoBlockTyp2 *sib2);
PUBLIC S16 uehFillSib2PucchCfg(NhuSysInfoBlockTyp2 *sib2);
PUBLIC S16 uehFillSib2UpLinkPwrCtrlCfg(NhuSysInfoBlockTyp2 *sib2);
PUBLIC S16 uehFillSib2SoundingRefUpLnkCfg(NhuSysInfoBlockTyp2 *sib2);
  

  
  
  




#endif /* UEH_SIB_H */
