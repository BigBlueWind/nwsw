#ifndef UEH_GLOBAL_DEC_H
#define UEH_GLOBAL_DEC_H



#include <stdarg.h>

#include "ueh_alias.h"
#include "ueh_fdecl.h"
#include "ueh_types.h"


#include "bsm_ueh_if.h"
#include "bsm_udh_if.h"
#include "bs_tables.h"
#include "ueh_pucch_res_mgmt.h"
#include "ueh_srs_res_mgmt.h"
#include "ueh_cfg.h"

#include "../../s1mgr/s1ap_cm.h"

#include <sys/timeb.h>
#include "ueh_udh_if.h"
#include "egt.h"
#include "egt.x"



#include "ccpu_cmn.h"
#include "msg_ids.h"
#include "bs_modules.h"
#include "bsm_s1ap_if.h"
#include "ludh.h"

#include "kw.h"

#ifdef LTE_L2_MEAS
#include "lrg.x"
#include "ueh_pmh_if.h"
#endif 

#include "ueh_cfg.h"
//#include "s1ap_mgr.h"
//#include "s1ap_sm.h"
#ifdef RRM_PRL_FLAG
#include "trace.h"
#endif

#ifdef UEH_PICO
#include "lteenb_common.h"
#endif

#define STATICBUFFBIT 1
#define BLOCKBUFFBIT  2
#define MSGBUFFBIT    4

void configMemLeakDetection(UINT8 memoryDiagCfg);

#define getUeHistoryTable()             &(uehCb.uehUeHisTbl)
#define getUeEventCounter()                  &(uehCb.ueEventCounterTable)
#define incrementUeEventCounter(ueIdx,ueEvt)  (uehCb.ueEventCounterTable.ueEventCounter[ueIdx][ueEvt]++)
#define clearUeHistoryTable(uehHistoryTabPtr)  uehHistoryTabPtr->nElem = 0

/** constants **/
const NhuTPollReTx       pollRetxForOamVal[ AMRLCTPOLLRETRANSMIT_MS500 + 1];
const NhuPollPdu         pollPduForOamVal[ AMRLCPOLLPDU_PINFINITY + 1];
const NhuRlcPollByte     pollByteForOamVal[ AMRLCPOLLBYTE_KBINFINITY + 1];
const NhuMaxRetxThres    maxRetxThreForOamVal[ AMRLCMAXRETXTHRESHOLD_T32 + 1 ];
const NhuTReOrdering     tReordForOamVal[ AMRLCTREORDERING_MS200 + 1];
const NhuTStatusProh     statProhForOamVal[ AMRLCTSTATUSPROHIBIT_MS500 + 1];
const NhuPdcpDiscardTmr  discTmrForOamVal[ DISCARDTIMER_INFINITY + 1 ];
const NhuMaxHarqTx       maxHargTxForOamVal[MAXULHARQTX_N28 + 1];

extern struct timeval  gTv_a1;
extern struct timeval  gTv_a2;
extern U32 uehGetTimerValFrmEnum[];
extern U32 uehGetUeRespTmrInMsFromEnum[];
extern U32 uehGetUeRlfDetectionTmrInMsFromEnum[];
extern U16 uehGetActualDefaultPagCycle[];
extern U8  uehGetActualModiPriodCoeff[];

EXTERN FILE* callTrcFile;
EXTERN FILE *protoMsgTrcFile;

//UehCellCb *uehGetCellCb(void);
UehCellCb *uehGetCellCbByCellId(UINT8 cellId);

void uehInitTmrDescTable(void);

EXTERN  UINT16         pagingCycleVal[4];
EXTERN  const U8       siWinSizeFrmWinLen[SIWINDOWLENGTH_40MS + 1];
EXTERN  const U32      siPeriodicityVal[];
EXTERN  const U8       rachNumOfRaPreambles[];
EXTERN  const U8       rachSizeOfRaPreambleGrpA[];
EXTERN  const U16      rachMsgGrpSizeInBits[];
EXTERN  UINT8          nBEnumMappingVal[7];
EXTERN  UINT8          poMappingValFromAsAndNs[3][4];

EXTERN SmCb   smCb;
EXTERN  UehCb          uehCb;
EXTERN  UehEuCb      uehEuCb;
EXTERN UehFreqBand uehFreqBand[];

EXTERN Bw2RbMapping bw2RbMapping[];
//EXTERN UehCtxHang uehCtxHangList[MAX_UE_SUPP];
#endif
