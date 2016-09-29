/** @file LteMacTestCfgMgr.h
 *
 * @brief Interface file to 36.141 test conformance manager
 * @author Mindspeed Technologies
 * @version $Revision: 1.104 $
 *
 * COPYRIGHT(c) 2008-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _LTE_MAC_TEST_MGR_H
#define _LTE_MAC_TEST_MGR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#ifdef MX_ARM_ENABLED
#include "bs_static_config.h"
#else
#include "osal.h"
#include "resultcodes.h"
#include "basetypes.h"
#endif

#include "dci.h"
#include "ltemaccore.h"

#ifndef AC_MSPD_TDD
#include "LtePhyL2Api.h"
#else
#include "LtePhyL2Api_TDD.h"
#endif

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAX_PDCCH_CHANNELS      20
#define MAX_PDSCH_CHANNELS      20
#define MAX_SDU_CHANNELS        10
#define MAX_LAYERS              4
#define TIMERRBUFDEPTH          6
#define TIMERRINFO_DELAY_SF     3
#define MAX_DYN_PERIOD          20


#define RB_UNASSIGNED           0xffffffff
#define MAX_RBS                 110
#define DYN_CFG_END             0xff

#ifdef FUNCTION_CALLS_TRACE
#define PRINT_FUNCTION_NAME(s)  printf("%s\n", s)
#else
#define PRINT_FUNCTION_NAME(s)
#endif

/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef enum eCodingSchema
{
    CS_TURBO_CODING = 0,
    CS_TAIL_BITTING,
    CS_BLOCK_CODE_CQI_32_2,
    CS_BLOCK_CODE_DL_HARQ_3_1,
    CS_CODE_UCI_20_A
} CODING_SCHEMA;

typedef U8 (*DciFunctionPtr)(U32 *pSdu, 
                             DCI_INFO *pDciInfo,
                             BS_STATIC_CONFIG *pBsStaticConfig, 
                             U32 *index);

typedef struct
{
    U8          pucch_format;   // FORMAT1=0, FORMAT1A, FORMAT1B; FORMAT2, FORMAT2A, FORMAT2B
    U8          harqPucchIndex;
    U8          dlCqiPmiSizeBits;   
}PUCCH_CONFIG;

typedef struct
{
    U32         dummy;
}PDCCH_CONFIG;

typedef struct
{
    DCI_INFO        dci_info;

    U8              dci_format;         // no Dci palyloads=0, 
                                        // DCI_FORMAT0=1;
                                        // DCI_FORMAT1=2, 1A, 1B, 1C, 1D; 
                                        // DCI_FORMAT2, 2A, 2B; DCI_FORMAT3, 3A;
    U8              dci_length_bits;
    U32             dci_payload[MAX_DCI_SIZE_BYTES];
    DciFunctionPtr  dci_encode_function;

// --------------------------------------------------------
    U8              mcs;
}PDSCH_CONFIG;

typedef struct
{
    DCI_INFO        dci_info;

    U8              dci_format;         // no Dci palyloads=0, 
                                        // DCI_FORMAT0=1;
                                        // DCI_FORMAT1=2, 1A, 1B, 1C, 1D; 
                                        // DCI_FORMAT2, 2A, 2B; DCI_FORMAT3, 3A;
    U8              dci_length_bits;
    U32             dci_payload[MAX_DCI_SIZE_BYTES];
    DciFunctionPtr  dci_encode_function;

// --------------------------------------------------------
    U8          mcs;

// --------------------------------------------------------
    U8          ena_mux;

    U8          num_ri_bits;        // 
    U8          num_ri_symbol;      // 

    U8          num_cqi_bits;       // 
    U8          num_cqi_symbol;     // 
    U8          cqi_coding_schema;  // 

    U8          num_harq;           // 
    U8          num_harq_symbol;    // 
}PUSCH_CONFIG;

typedef struct
{
    U8          prach_index;       // 
    U8          Ncs_config;        // 1 (Ncs value = 13, 0.79 km radius)
    U8          high_speed;        // no higt speed support (= 0)
}PRACH_CONFIG;


typedef struct VUE_CONFIG        // "Virtual" UE
{
    U16                 rnti; // rnti 0 means end of array
    PDSCH_CONFIG        *pdsch;
    PUSCH_CONFIG        *pusch;
    PDCCH_CONFIG        *pdcch;
    PUCCH_CONFIG        *pucch;
}VUE_CFG;

typedef struct RBMap_tag
{
    U32 max_rbs;                    //!< total amount of RBs available
    U32 unassigned_rbs_cnt;         //!< number of unassigned RBs
    U32 ids[MAX_RBS];               //!< RB allocation map: each element is resource ID

    U32 prop_cnt;                   //!< number of channels to participate in proportional distribution
    U32 prop_ids[MAX_RBS];          //!< channel ids to participate in proportional distribution
    U32 prop_vrb[MAX_RBS];          //!< virtual compliant assignment required
    U32 prop_sizes[MAX_RBS];        //!< sizes of assigned RB regions

    U32 fixed_size_cnt;             //!< number of channels being set fixed size, but not offset
    U32 fixed_size_total;           //!< total size of all fixed RBs
    U32 fixed_size_ids[MAX_RBS];    //!< ids of channels with fixed size, but not offset
    U32 fixed_size_val[MAX_RBS];    //!< sizes of channels with fixed size

}RBMap;

typedef struct ULPUCCH_CONFIG
{
    U8 deltaPUCCHShift;
    U8 nRBCQI;
    U8 nCSAn;
    U16 n1PucchAN;
    U8 cyclicShift;
    U8 uciformat;
    U8 enable64QAM;
    U8 pucch_only;
    U8 dlCqiPmiSizeBits;
    int nSB;
    U8 hoppingMode;
    U8 puschhoppingOffset;
    U8 groupHoppingEnabled;
    U8 groupAssignmentPUSCH;
    U8 sequenceHoppingEnabled;
} UL_PUCCH_CFG, *PUL_PUCCH_CFG;

typedef struct TimingAdvErrInfoFsf
{
    U32 mErrAvgCh[2];
    S16 mErrExpo;
    U16 mSetDefaults;
    U16 Fnumber;
    U16 SFnumber;
} TIMADVERRINF_FSF, *PTIMADVERRINF_FSF;

typedef struct _stCW_CFG
{
    U8 Mcs;
    U8 Mod;
    U16 Rvi; //3 In Lte used U16
}CW_CFG, *PCW_CFG;
typedef struct _stPDSCH_CFG
{
    CW_CFG CwCfg[2];
    U16 Rnti;
    U16 ueCategory;
    U32 Pa;
    MAPPINGINFO Map; //has to be a last parameter
}PDSCH_CFG, *PPDSCH_CFG;
/* Physical parameters for E-TM */
typedef struct BsTestConformanceConfig
{
    U32 configId;
    /* Select model */
    U8 TestModel;

    /* The following general parameters are used by all E-UTRA test models: */
    U8 nAntennaPort;    /* using p = 0 */
    U8 nCw;             /* 1 code word */
    U8 nLayers;         /* 1 layer */
    U8 precodingMode;   /* not used */
    U32 duration;       /* 10 subframes */
    U8 cp;
    U8 transmissionMode;
    U8 cddType;
    U8 codeBookIdx;
    U8 arRctRvDl[2][10];
    /* Virtual resource blocks of localized type,
    no intra-subframe hopping for PDSCH */

    /* UE-specific reference signals are not used */

//    U8 bwMode;          /* 5Mhz, 10Mhz, 20Mhz */
    U32 RctMcsDl[4];
    U32 RctMcsUl[4];

    U8 arRctRv[1000];
    U32 nRctRv;
    U32 curRctRv;

    /* Referance, Syncronisation Signals */
    U32 rsBoosting;     /* RS boosting, PB = EB/EA */
    S32 epreSinchro;    /* Synchronisation signal EPRE / ERS [dB] */

    S32 reservedEpre;   /* Reserved EPRE / ERS [dB] */

//    S32 rs_epre; //pilotPower in  struct of TxVector)
//    S32 pss_ra;  //psyncPower
//    S32 sss_ra;  //ssyncPower

    /* PBCH */
    U32 pbchEna;
    U32 pbchEpre;         /* PBCH EPRE / ERS [dB] */
    S32 pbchReservedEpre; /* ReservedEPRE / ERS [dB]/ ERS [dB] */
    U8 payloadPbch[BCH_MAX_SIZE];
    U8 pbchRv;

    /* PCFICH */
    U8 nCtrlSymbols;      /* # of symbols used for control channels */


    /* PHICH */
    U8 nPhichGroup;      /* # of PHICH groups */
    U8 nPhichPerGroup;   /* # of PHICH per group */
    U8 PhichGroupSfn; //for Tdd

    S32 phichSymPower;   /* PHICH BPSK symbol power / ERS [dB] */
    S32 phichGroupEpre;  /* PHICH group EPRE / ERS */

    /* PDCCH */
    U8 nAvalableRegs;   /* # of available REGs */
    U8 nPdcch;          /* # of PDCCH */
    U8 nPdcchUl;
    U8 nCcePerPdcch;    /* # of CCEs per PDCCH */
    U8 nRegPerCce;      /* # of REGs per CCE */
    U8 nRegAllToPdcch;  /* # of REGs allocated to PDCCH */
    U8 nNilRegPadding;  /* # of <NIL> REGs added for padding */
    DCI_FORMAT DciFormat;


    U8 pdcch_map_used;
    RESELEMINFO pdcch_map[MAX_PDCCH_CHANNELS][10];
    U8 pdcch_map_unified[MAX_PDCCH_CHANNELS];

    /* DCI  */
    U8 payloadPdcch_unified;
    U8 payloadPdcch[MAX_PDCCH_CHANNELS][DCI_MAX_SIZE];
    U32 PdcchDataLen[MAX_PDCCH_CHANNELS];
    U8 payloadPdcchUl[DCI_MAX_SIZE];
    U32 PdcchDataLenUl[MAX_PDCCH_CHANNELS];
	U16 PdcchRnti[MAX_PDCCH_CHANNELS];
	U16 PdcchRntiEna;

    S32 pdcchRegEpre;   /* PDCCH REG EPRE / ERS [dB] */
    S32 nilRegEpre;     /* <NIL> REG EPRE / ERS [dB] */

    /* PDSCH */
    U16 nPdsch[MAX_LAYERS];

    U8  nPdschPrb[MAX_LAYERS][MAX_SDU_CHANNELS];   /* # of QPSK PDSCH PRBs which are boosted */
    S32 pdschPrbPa[MAX_LAYERS][MAX_SDU_CHANNELS];         /* PRB PA = EA/ERS [dB] */
    U8  pdschMod[MAX_LAYERS][MAX_SDU_CHANNELS];

    U8  nPdschBoostedPrb;   /* # of QPSK PDSCH PRBs which are boosted */
    U8  nPdschDeBoostedPrb; /* # of QPSK PDSCH PRBs which are de-boosted */

    U8 pdschMcs_used[2/*per CW*/];
    U8 pdschMcs[2/*per CW*/][MAX_PDSCH_CHANNELS/*per Ch*/];

    U8 pdschPayloadMode[2/*per CW*/][MAX_PDSCH_CHANNELS/*per Ch*/];

    RESELEMINFO pdsch_map[MAX_PDSCH_CHANNELS/*per Ch*/];
    U8 pdsch_map_used;

    U8 pdschRnti_used;
    U16 pdschRnti[MAX_PDSCH_CHANNELS/*per Ch*/];

    /* PUSCH */
    U8  nPuschBoostedPrb;   /* # of QPSK PDSCH PRBs which are boosted */
    S32 puschBoostedPrbPa;         /* PRB PA = EA/ERS [dB] */

    U8  puschBoostedMod;

    U8 puschMcs_used;
    U8 puschMcs[10/*per Ch*/];

    U8 pusch_nDMRS2[10/*per Ch*/];
    U8 pusch_nDMRS2_used;

    U8 pusch_ri[16];        // number of ri per each user
    U8 pusch_nCQI[16];      // number of CQI per each user
    U8 pusch_nack[16];      // number of HARQ(ACK) per each user

    U16 rntiPusch[10/*per Ch*/];
    U8 rntiPusch_used;

    U8 pusch_map_used;
    RESELEMINFO pusch_map[10/*per Ch*/];

    U16 rntiPucch[16/*per Ch*/];
    U8 rntiPucch_used;

    U8 pucch_map_used;
    RESELEMINFO pucch_map[16/*per Ch*/];

    U8 pucchFormat[16/*per Ch*/];
    U8 pucchFormat_used;

    U16 rntiBoosted[16/*per Ch*/];
    U16 rntiDeBoosted[16/*per Ch*/];

    U8 pucch_dlCqiPmiSizeBits[16/*per Ch*/];
    U8 pucch_dlCqiPmiSizeBits_used;

    U8 pucch_harqSizebits_used;
    U8 pucch_harqSizebits[16/*per Ch*/];

    U8 pucch_harqPucchIndex_used;
    U16 pucch_harqPucchIndex[16/*per Ch*/];

    U8 ackPUCCHResourceIndex[4];
    U16 ackPUCCHResourceIndex0[16/*per Ch*/];
    U16 ackPUCCHResourceIndex1[16/*per Ch*/];
    U16 ackPUCCHResourceIndex2[16/*per Ch*/];
    U16 ackPUCCHResourceIndex3[16/*per Ch*/];

    U16 pucch_cqirsrcidx[16/*per Ch*/];
    U8 pucch_cqirsrcidx_used;

/// Pusch dedicated channel
    U8 PuschRiIdxUsed;
    U8 PuschRiIdx[16/*per Ch*/];

    U8 PuschCqiIdx[16/*per Ch*/];
    U8 PuschCqiIdxUsed;

    U8 Pusch_nRiUsed;
    U8 Pusch_nRi[16/*per Ch*/][10];

    U8 Pusch_nrlCqi[16/*per Ch*/][10];
    U8 Pusch_nrlCqiUsed;

    U8 Pusch_nrglCqiUsed;
    U8 Pusch_nrglCqi[16/*per Ch*/];

    U8 PuschAckIdx[16/*per Ch*/];
    U8 PuschAckUsed;

    U8  Pusch_nAckUsed;
    U8 Pusch_nAck[16/*per Ch*/][10];
	
    int betaOffsetCQIIndex;
    int nsymi;

    /* PRACH */
    U8 prachEnableAllSfs;
    U32 rootSequenceIndex;
    U8 prachConfigIndex;
    U8 highSpeedFlag;
    U8 zeroCorrelationZoneConfig;
    U8 prachFreqOffset;

    U8 prachMap[10];
    U8 TimAdvCmdMap[10];
    UL_PUCCH_CFG ulSfrCtrl;
    U32 SFcount;

    U16 DirMode;
    U32 RxNumBytes[2];

#ifdef MX_ARM_ENABLED 
    TIMADVERRINF_FSF timAdvErrInfoArrFSF[TIMERRBUFDEPTH/*delay*/][16/*per Ch*/];
#else
    TIMADVERRINF timAdvErrInfoArrFSF[TIMERRBUFDEPTH/*delay*/][16/*per Ch*/];
#endif

    TIMADVERRINF timAdvErrInfo[3];

    U8 timAdvErrInfoArr_put[16/*per Ch*/];
    U8 timAdvErrInfoArr_get[16/*per Ch*/];

    PUSCHDED puschDed;

    U8 numPucch[10];
    U8 numPusch[10];
    U8 ULEnabled;

    U8 SwpTst_MapIdx_1[101];
    U8 SwpTst_MapIdx_2[101];

    U8 timAdvErrDflt_pucch[2000];
    U16 timAdvErrDflt_pucch_len;
    U16 timAdvErrDflt_pusch_len;
    U8 timAdvErrDflt_pusch[2000];

    SRSDED srsInfo[16/* per CH */];
    U8 srsBandwitdhConfig;
    U8 srsSubframeConfig;
    U8 srsNumber;
    PDSCH_CFG PdschCfg[16/* per CH */];
    U32 PdschCfgEna;
    U8 phichResource;
    U8 phichDuration;
    U8 phichIdx;
    U8 pdschPayload;
    U8 pdschPayloadEna;

    U8 ackNackSRSSimultaneousTransmission;
    U8 ackNackReportMode;
    U8 Nbundled;
    U32 dyn_period; // dynamic period: different channel configurations used every sub-frame

    U8 dyn_pdsch[MAX_DYN_PERIOD + 1][MAX_PDSCH_CHANNELS]; // +1 for trailing 0xff
    U8 dyn_pdcch[MAX_DYN_PERIOD + 1][MAX_PDCCH_CHANNELS];

    RBMap ul_rb_map, dl_rb_map;
    U8 DontUsePuschRxSduForHiAndDci0;

    U8 ena_harq;
    U8 ena_dynamic_cqi_pmi_ri;

    U8 nmmMode;
} BS_CONFOR_TEST_CONFIG;

/*  */
typedef enum
{
    TEST_MGR_NO_CONFIG, //0
    TEST_MGR_10MHZ_1ANT_RT,       // 1
    TEST_MGR_10MHZ_1ANT_RT_MAX_THROUGHPUT,   // 2

    TEST_MGR_10MHZ_1ANT_RT_NO_REPACK,     // 3

    TEST_MGR_20MHZ_1ANT_RT,                              // 4
    TEST_MGR_20MHZ_1ANT_RT_MAX_THROUGHPUT, // 5

    TEST_MGR_TM500,                                                   // 6
    TEST_MGR_1_10MHz_2ANT,                                   // 7
    TEST_MGR_1_20MHz_2ANT,                                  // 8

    TEST_MGR_10MHZ_1ANT_RT_MAX_QAM16,                  // 9
    TEST_MGR_10MHZ_1ANT_RT_QAM16_QAM64_2SDU,  // 10
    TEST_MGR_10MHZ_1ANT_RT_QAM64_25_50,            // 11

    TEST_MGR_10MHz_2ANT_1CW,

    TEST_MGR_20MHz_2ANT_BCH, //13
    TEST_MGR_20MHz_2ANT_BCH_PDCCH, //14
    TEST_MGR_20MHz_2ANT_SPMUX, //15
    TEST_MGR_20MHz_2ANT_DIVERSITY, //16

    TEST_HARQ, //17
    TEST_HARQ_OPTIMIZATION_QAM64, //18
    TEST_HARQ_OPTIMIZATION_QAM16, //19

    TEST_1TXHISDU, //20
    TEST_1TXDCIULSDU,
    TEST_4HISDU_R1TXDCIULSDU, //22
    TEST_MGR_20MHz_2ANT_SPMUX_RT39, //23
    TEST_MGR_20MHz_2ANT_SPMUX_RT43, //24
    TEST_10MHz_XDL_XUL_HARQ_RT45, //25

    TEST_6UE_1ANT_1LAYER_10MHz_MAX_THROUGHPUT, //26
    TEST_6UE_1ANT_1LAYER_20MHz_MAX_THROUGHPUT, //27
    TEST_6UE_2ANT_2LAYER_10MHz_MAX_THROUGHPUT, //28
    TEST_6UE_2ANT_2LAYER_20MHz_MAX_THROUGHPUT, //29

    TEST_MGR_TM500_CTRL, //30
    TEST_MGR_10MHz_2ANT_SPMUX, //31
    TEST_4UE_10MHz_1ANT_MAX_THROUGHPUT, //32
    
    TEST_RESERVED,
    
    TEST_4UE_10Mhz_1ANT_MAX_THROUGHPUT_1, // 34
    TEST_4UE_10Mhz_1ANT_MAX_THROUGHPUT_2, // 35
    TEST_4UE_10Mhz_1ANT_MAX_THROUGHPUT_3, // 36
    TEST_4UE_10Mhz_1ANT_MAX_THROUGHPUT_4, // 37
    
    TEST_3UE_10Mhz_1ANT_MAX_THROUGHPUT_1, // 38
    TEST_3UE_10Mhz_1ANT_MAX_THROUGHPUT_2, // 39
    TEST_3UE_10Mhz_1ANT_MAX_THROUGHPUT_3, // 40
    TEST_3UE_10Mhz_1ANT_MAX_THROUGHPUT_4, // 41
    
    TEST_2UE_10Mhz_1ANT_MAX_THROUGHPUT_1, // 42
    TEST_2UE_10Mhz_1ANT_MAX_THROUGHPUT_2, // 43
    TEST_2UE_10Mhz_1ANT_MAX_THROUGHPUT_3, // 44
    TEST_2UE_10Mhz_1ANT_MAX_THROUGHPUT_4, // 45

    TEST_8UE_10Mhz_2ANT_MAX_THROUGHPUT_1, // 46
    TEST_5UE_10Mhz_2ANT_MAX_THROUGHPUT_2, // 47

    TEST_4UE_10Mhz_2ANT_CQI_HARQ, // 48
    TEST_4UE_10Mhz_2ANT_HARQ, // 49

    TEST_1UE_10Mhz_1ANT_UDP_MPEG2, //50
    TEST_1UE_10Mhz_1ANT_UDP_MPEG2_DIRECT, //51

    TEST_xUE_10Mhz_1ANT_VAR_THROUGHPUT, //52
    
    TEST_2Pdsch2AntSpatialMuxQAM64_2Pusch2AntQAM16_10MHZ, //53
    TEST_3Pdsch2AntSpatialMuxQAM64_3Pusch2AntQAM16_10MHZ, //54
    TEST_4Pdsch2AntSpatialMuxQAM64_4Pusch2AntQAM16_10MHZ, //55

    TEST_RESERVED_56, // 56

    // configurations from v3_7_1 
    TEST_MGR_1_10MHz_2TXANT_2RXANT,     //57
    TEST_MGR_1_10MHz_2ANT_HDX,          //58
    TEST_MGR_1_10MHz_2TXANT_2RXANT_HDX, //59

    TEST_ETM_LIB, //60


    TEST_MGR_CONFIG_MAX_ID // don't forget to move it
} TEST_MGR_PREDEF_CONFIG;

typedef struct
{
   U32 configId;
   U32 ConsoleVals[20];
   U32 ConsoleValsNum;
}TESTMGRINIT, *PTESTMGRINIT;

typedef struct TestMgrCtx
{
    U8 **boostingMap[MAX_LAYERS][MAX_SDU_CHANNELS];
    U8 **deBoostingMap;
    const U8 **boostingUlMap;
}TEST_MGR_CTX;


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern BS_CONFOR_TEST_CONFIG    testConfConfig;
extern TEST_MGR_CTX             testMgrCtx;

#ifdef MX_ARM_ENABLED 
extern char TEST_MGR_PREDEF_CONFIG_STR[TEST_MGR_CONFIG_MAX_ID][64];
extern int TEST_MGR_PREDEF_CONFIG_DETAILS[TEST_MGR_CONFIG_MAX_ID][11];
#endif
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

RESULTCODE TestMgrInit(PTESTMGRINIT pTestMgrCfg);
RESULTCODE TestMgrClean(void);
RESULTCODE TestMgrReconfigure(BS_CONFOR_TEST_CONFIG *pTestConfig);
RESULTCODE TestMgrReconfigureRxVec(BS_CONFOR_TEST_CONFIG *pTestConfig);
RESULTCODE TestMgrReconfigureTxVec(BS_CONFOR_TEST_CONFIG *pTestConfig);

RESULTCODE TestMgrProcessConfigDl(PDLSUBFRDESC pSubFrame);
RESULTCODE TestMgrProcessConfigUl(PULSUBFRDESC pSubFrame);

RESULTCODE RctMacAllocationULMap(U16 sfn, U8* map, U32 len);
U8 TestMgrIsTimAdv(BS_CONFOR_TEST_CONFIG *pTestConfConfig, U8 sf);

U32 DCI0DescSet(PDLSUBFRDESC pDlSubFrame,
                PULSUBFRDESC pUlSubFrame,
                PRXSDUIND pRxSdu);

RESULTCODE TestMgrProcessConfigDl_Ex(PDLSUBFRDESC pSubFrame);

#ifdef MX_ARM_ENABLED /*Exported Functions*/

RESULTCODE LteBsMacHiDCI0Scheduler(PDLSUBFRDESC pDlSubFrame,
                                   PULSUBFRDESC pUlSubFrame,
                                   PRXSDUIND pRxSdu);
U32 rbm_get_prb(RBMap *rbm, U32 id, S32 *rb_offset, U32 *rb_size);
void rbm_init(RBMap *rbm, U32 max_rbs);
U32 rbm_alloc(RBMap *rbm, U32 id, S32 prb_offset, U32 prb_num, U8 vrb);
U32 rbm_done(RBMap *rbm);
void vue_alloc_res(VUE_CFG *vues, RBMap *dlm, RBMap *ulm);

#else /*Exported Functions*/

RESULTCODE TestMgrInitRt(PTESTMGRINIT pTestMgrCfg);
RESULTCODE TestMgrProcessConfigDlRt(PDLSUBFRDESC pSubFrame);
RESULTCODE TestMgrProcessConfigDlRt_Ex(PDLSUBFRDESC pSubFrame);
U32 TestMgrCalculateDataSduSize(PDLSUBCHINFO pSubCh, MAPPINGINFO *pMapInfo, U8 TxMode);
RESULTCODE TestFileCfgMgr(BS_CONFOR_TEST_CONFIG *pTestConfig);
RESULTCODE SetRv(U8* rv, U32 len);
#endif /*Exported Functions*/

U8 PrachConfig(U8 Index, U16 frameNumber, U16 subframeNumber);
RESULTCODE LtePhyUpdateTestRxVectorSrs(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                       PULSUBFRDESC pSubFrame);

S16 TestMgrGetPcfichPower(BS_CONFOR_TEST_CONFIG *pTestConfConfig);
U8 TestMgrGetNumConsecutiveRBs(U8 **pMap, U32* num);
S16 TestMgrGetSsyncPower(BS_CONFOR_TEST_CONFIG *pTestConfConfig);
S16 TestMgrGetPsyncPower(BS_CONFOR_TEST_CONFIG *pTestConfConfig);
S16 TestMgrGetPilotPower(BS_CONFOR_TEST_CONFIG *pTestConfConfig);
U16 TestMgrGetNumPhich(BS_CONFOR_TEST_CONFIG *pTestConfConfig);

RESULTCODE LtePhyAddPdsch(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                          PDLCHANDESC pCh,
                          U32 id,
                          U32 pdsch_idx,
                          MAPPINGINFO *pMapInfo,
                          U16 rnti,
                          S16 power);

U8 GetMcsIndexDl(U16 modulation, U16 cwId);
U8 GetMcsIndexUl(U16 modulation);

RESULTCODE LtePhyAddPusch(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                          PULCHANDESC pCh,
                          U32 id,
                          S16 power);

RESULTCODE LtePhyFillPuschSubDesc(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                  PULSUBCHINFO pSubCh,
                                  U8 modulation,
                                  U8 mcsType,
                                  MAPPINGINFO *pMapInfo,
                                  U8 idx);

U8 TestMgrPuschMcs2Mod(U8 Mcs);
U8 TestMgrGetPuschModulation(BS_CONFOR_TEST_CONFIG *pTestConfConfig);


S16 TestMgrGetPuschPower(BS_CONFOR_TEST_CONFIG *pTestConfConfig);
RESULTCODE TestMgrGetPusch(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                           MAPPINGINFO *pMapInfo, MAPPINGINFO *pMapInfoStore);

RESULTCODE LtePhyAddPucch(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                          PULCTRLCHDESC pCh,
                          U32 id,
                          MAPPINGINFO *pMapInfo,
                          U32 Idx);

U8 TestMgrGetNumConsecutiveRBs(U8 **pMap, U32* num);

BS_CONFOR_TEST_CONFIG* get_bs_conf_test_config(void);
DCI_INFO* get_dci_info(U32 ch_idx);

RESULTCODE SetDLDciCtx(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                       DCI_DATA_INFO* pDciInfo, /* TxVector */
                       U32 pdsch_idx);




#endif /* _LTE_MAC_TEST_MGR_H */

