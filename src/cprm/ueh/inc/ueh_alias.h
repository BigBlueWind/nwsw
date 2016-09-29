#ifndef UEH_ALIAS_H
#define UEH_ALIAS_H


#define SIZE_UEH_LOG_FILENAME	100
#define EXE_NAME		        "cpup"
#define EXE_NAME1               "uehCallTrc"
#define EXE_NAME2               "protoMsgTrc"

#define MIN_PROC_ID         1

#define OAM_PROC_ID         4
#define CP_PROC_ID          2
#define GTP_PROC_ID         3
#define RADIOUP_PROC_ID     1

#ifdef PWAV_L2SIM
#define L2SIM_PROC_ID RADIOUP_PROC_ID
#endif

#define MAX_PROC_ID         5

/* Logical Processor Ids */
#define SM_UEH_PROC   CP_PROC_ID
#define SM_SM_PROC    CP_PROC_ID /* Stack Manager */
#define SM_NH_PROC    CP_PROC_ID /* RRC */
#define SM_NX_PROC    SM_UEH_PROC /* RRC User = UEH */
#define SM_SZ_PROC    CP_PROC_ID /* S1AP */
#define SM_SB_PROC    CP_PROC_ID /* SCTP */
#define SM_S1M_PROC   CP_PROC_ID
#define SM_CEM_PROC   CP_PROC_ID

#define SM_HI_PROC    GTP_PROC_ID /* TUCL */
#define SM_EG_PROC    GTP_PROC_ID /* EGTP */
#define SM_UDH_PROC   GTP_PROC_ID /* User Data Handler */

#define SM_PJ_PROC    RADIOUP_PROC_ID /* PDCP */
#define SM_TF_PROC    RADIOUP_PROC_ID /* PHY: Not used */
#define SM_KW_PROC    RADIOUP_PROC_ID /* RLC */
#define SM_RG_PROC    RADIOUP_PROC_ID /* MAC */
#define SM_YS_PROC    RADIOUP_PROC_ID /* MAC-PHY-CL */

#define SM_BSM_PROC   OAM_PROC_ID
#define SM_PRM_PROC   OAM_PROC_ID
#define SM_LTH_PROC   OAM_PROC_ID
#define SM_PMH_PROC   OAM_PROC_ID

/* ve001.102: Assigned Pool values to 0 */
#define RG_POOL     0
#define HI_POOL     0
#define SB_POOL     0
#define SZ_POOL     0
#define UEH_NH_POOL 0
#define UEH_POOL    0
#define UDH_POOL    0
#define EG_POOL     0
#define YS_POOL     0
#define PJ_POOL     0
#define KW_POOL     0

#define SCH_INST_ID   1

#define UEH_YS_SUID                      0
#define UEH_YS_SPID                      0
#define UEH_NH_SUID                      1
#define UEH_NH_SPID                      0

#define UEH_RG_SUID                      0
#define UEH_RG_SPID                      0
#define UEH_SZ_SPID                      0

#define UEH_UDH_SUID                     6
#define UEH_UDH_SPID                     0

#define UDH_EG_SUID                      0
#define UDH_EG_SPID                      0
#define UDH_PJ_SUID                      1
#define UDH_PJ_SPID                      1

#define NH_PJ_SPID                       0 /* Vamsee */
#define NH_PJ_SUID                       0

#define RG_YS_SUID                       0
#define RG_YS_SPID                       0
#define RG_SCH_YS_SPID                   1

#define SB_HI_SUID                      0
#define SB_HI_SPID                      0

#define EG_HI_SUID                      0
#define EG_HI_SPID                      1

#define SZ_SB_SUID                      0
#define SZ_SB_SPID                      0

#define NH_KW_KWU_SUID                  0
#define NH_KW_CKW_SUID                  1
#define NH_RG_CRG_SUID                  2
#define NH_KW_CPJ_SUID                  3
#define NH_KW_PJU_SUID                  4



#define UEH_RG_GEN_CFG                   1
#define UEH_RG_RGU_SAP_CFG               2
#define UEH_RG_CRG_SAP_CFG               4
#define UEH_RG_SCH_INS_CFG               8
#define UEH_RG_TFU_SAP_CFG              16

#define UEH_KW_GEN_CFG                  1
#define UEH_KW_RGU_SAP_CFG              2
#define UEH_KW_CKW_SAP_CFG              4
#define UEH_KW_KWU_SAP_CFG              8

#define UEH_NH_GEN_CFG            1
#define UEH_NH_PROT_CFG           2
#define UEH_NH_USAP_CFG           4
#define UEH_NH_CKW_SAP_CFG        8
#define UEH_NH_KWU_SAP_CFG       16 
#define UEH_NH_CRG_SAP_CFG       32
#define UEH_NH_CPJ_SAP_CFG       64
#define UEH_NH_PJU_SAP_CFG      128
#define UEH_NH_CTF_SAP_CFG      256

#define UEH_SZ_GEN_CFG                  1 
#define UEH_SZ_PROT_CFG                 2
#define UEH_SZ_SZT_SAP_CFG              4
#define UEH_SZ_SCT_SAP_CFG              8
#define UEH_SZ_PEER_CFG                16

#define UEH_SB_GEN_CFG                  1
#define UEH_SB_TSAP_CFG                 2
#define UEH_SB_SCT_SAP_CFG              4 

#define UEH_EG_GEN_CFG                  1
#define UEH_EG_EGT_SAP_CFG              2
#define UEH_EG_TSAP_CFG                 4
#define UEH_EG_TSRV_CFG                 8
#define UEH_EG_IPADDR_CFG              16

#define UEH_UEH_GEN_CFG                  1
#define UEH_UEH_CTF_SAP_CFG              2
#define UEH_UEH_NHU_SAP_CFG              4
#define UEH_UEH_RGR_SAP_CFG              8
#define UEH_UEH_SZT_SAP_CFG             16
#define UEH_UEH_UDH_SAP_CFG             32

#define UEH_YS_GEN_CFG                  1
#define UEH_YS_CTF_SAP_CFG              2
#define UEH_YS_TFU_SAP_CFG              4

#define UEH_PJ_GEN_CFG                  1
#define UEH_PJ_CPJ_SAP_CFG              2
#define UEH_PJ_PJU_SAP_CFG              4

#define UEH_SM_PJ_CONFIGURED  (UEH_PJ_GEN_CFG | UEH_PJ_CPJ_SAP_CFG | \
                                              UEH_PJ_PJU_SAP_CFG)

#define UEH_SM_UEH_CONFIGURED  (UEH_UEH_GEN_CFG | UEH_UEH_CTF_SAP_CFG | \
                              UEH_UEH_NHU_SAP_CFG | UEH_UEH_RGR_SAP_CFG |\
                              UEH_UEH_SZT_SAP_CFG | UEH_UEH_UDH_SAP_CFG)

#define UEH_HI_GEN_CFG        1
#define UEH_HI_TSAP_CFG       2

#define UEH_SM_HI_CONFIGURED  (UEH_HI_GEN_CFG | UEH_HI_TSAP_CFG)
#define UEH_SM_SB_CONFIGURED  (UEH_SB_GEN_CFG | UEH_SB_TSAP_CFG | UEH_SB_SCT_SAP_CFG)
#define UEH_SM_SZ_CONFIGURED  (UEH_SZ_GEN_CFG | UEH_SZ_PROT_CFG | UEH_SZ_SZT_SAP_CFG | \
                              UEH_SZ_SCT_SAP_CFG )
#define UEH_SM_EG_CONFIGURED  (UEH_EG_GEN_CFG | UEH_EG_EGT_SAP_CFG | \
                              UEH_EG_TSAP_CFG | UEH_EG_TSRV_CFG | UEH_EG_IPADDR_CFG)


#define UEH_SM_YS_CONFIGURED   (UEH_YS_GEN_CFG | UEH_YS_CTF_SAP_CFG | UEH_YS_TFU_SAP_CFG)

#define UEH_SM_RG_CONFIGURED  (UEH_RG_GEN_CFG |  UEH_RG_RGU_SAP_CFG |\
                              UEH_RG_CRG_SAP_CFG | UEH_RG_SCH_INS_CFG |\
                              UEH_RG_TFU_SAP_CFG)
#define UEH_SM_KW_CONFIGURED  (UEH_KW_GEN_CFG |  UEH_KW_RGU_SAP_CFG |\
                              UEH_KW_CKW_SAP_CFG | UEH_KW_KWU_SAP_CFG)
#define UEH_SM_NH_CONFIGURED  (UEH_NH_GEN_CFG | UEH_NH_PROT_CFG | UEH_NH_USAP_CFG | \
                              UEH_NH_CKW_SAP_CFG | UEH_NH_KWU_SAP_CFG | \
                              UEH_NH_CRG_SAP_CFG | UEH_NH_CPJ_SAP_CFG | \
                              UEH_NH_PJU_SAP_CFG )

#define UEHSM_LYS_MAX_TFU_SAPS         2
#define UEHSM_LYS_NMB_UES              2


/* Transaction IDs for DEBUG */
#define SM_MAC_DEBUG_TRANS_ID             120
#define SM_RLC_DEBUG_TRANS_ID             121
#define SM_PDCP_DEBUG_TRANS_ID            122
#define SM_RRC_DEBUG_TRANS_ID             123
#define SM_S1AP_DEBUG_TRANS_ID            124
#define SM_SCTP_DEBUG_TRANS_ID            125
#define SM_EGTP_DEBUG_TRANS_ID            126
#define SM_TUCL_DEBUG_TRANS_ID            127
#define SM_CL_DEBUG_TRANS_ID              128

/* Transaction IDs for ALARM */
#define SM_RRC_ALARM_TRANS_ID             121
#define SM_S1AP_ALARM_TRANS_ID            122

/* Transaction IDs for Peer Deletion */
#define SM_RRC_PEERDEL_TRANS_ID             122
#define SM_S1AP_PEERDEL_TRANS_ID            123


/* Transaction IDs for LSAP BIND */
#define SM_RRC_LSAPBND_TRANS_ID             124
#define SM_S1AP_LSAPBND_TRANS_ID            125

#define SM_MAC_TRACE_TRANS_ID             100

#define SM_RLC_START_TRACE_TRANS_ID       100
#define SM_RLC_STOP_TRACE_TRANS_ID        110

#define SM_PDCP_START_TRACE_TRANS_ID      100
#define SM_PDCP_STOP_TRACE_TRANS_ID       110

#define SM_EGTP_START_TRACE_TRANS_ID       100
#define SM_EGTP_STOP_TRACE_TRANS_ID        110

#define SM_S1AP_SCTSAP_START_TRACE_TRANS_ID     100
#define SM_S1AP_PEER_START_TRACE_TRANS_ID       101
#define SM_S1AP_SCTSAP_STOP_TRACE_TRANS_ID      110
#define SM_S1AP_PEER_STOP_TRACE_TRANS_ID        111

#define SM_RRC_KWUSAP_START_TRACE_TRANS_ID     100
#define SM_RRC_CKWSAP_START_TRACE_TRANS_ID     101
#define SM_RRC_CRGSAP_START_TRACE_TRANS_ID     102
#define SM_RRC_CPJSAP_START_TRACE_TRANS_ID     103
#define SM_RRC_PJUSAP_START_TRACE_TRANS_ID     104
#define SM_RRC_KWUSAP_STOP_TRACE_TRANS_ID      110
#define SM_RRC_CKWSAP_STOP_TRACE_TRANS_ID      111
#define SM_RRC_CRGSAP_STOP_TRACE_TRANS_ID      112
#define SM_RRC_CPJSAP_STOP_TRACE_TRANS_ID      113
#define SM_RRC_PJUSAP_STOP_TRACE_TRANS_ID      114




#define SM_RRC_TRACE_TRANS_ID             105





#define SM_DFLT_MEM_UPPER_THR_CFG   9
#define SM_DFLT_MEM_LOWER_THR_CFG   2
#define SM_DFLT_MSEC_TMR_CFG        10

#define SM_INST_ZERO   0
#define SM_ZERO_VAL    0

#define KWU_SAP_ID 0x00
#define CKW_SAP_ID 0x01
#define CRG_SAP_ID 0x02
#define CPJ_SAP_ID 0x03
#define PJU_SAP_ID 0x04
#define RGR_SAP_ID 0x05
#define RGU_SAP_ID 0x05
#define NHU_SAP_ID 0x06


#define SM_SELECTOR_LC      0
#define SM_SELECTOR_TC      1
#define SM_SELECTOR_LWLC    2


#define UEH_SZ_SPID                      0
#define UEH_SZ_SUID                      0

#define UEHSM_LSZ_MAX_SZTSAP   10
#define UEHSM_LSZ_MAX_SCTSAP   10

#define UEHSM_LSZ_TIMERES      10
#define UEHSM_MSGPRIOR        PRIOR0    

/* SZ ---->SM posts */
#define UEHSM_SZSMSEL           0
/* SM ---->SZ posts */
#define UEHSM_SMSZSEL           0
 
/* YS ---->SM posts */
#define UEHSM_YSSMSEL           0
/* SM ---->YS posts */
#define UEHSM_SMYSSEL           0
 
/* SB ----> SM selector */
#define UEHSM_SBSMSEL           0
/* SM ----> SB selector */
#define UEHSM_SMSBSEL           0
#ifdef CL_MAC_LWLC
/* RG ----> YS selector */
#define UEHSM_RGYSSEL           2
/* YS ----> RG selector */
#define UEHSM_YSRGSEL           2
#else
/* RG ----> YS selector */
#define UEHSM_RGYSSEL           1
/* YS ----> RG selector */
#define UEHSM_YSRGSEL           1
#endif


/* HI ----> SM selector */
#define UEHSM_HISMSEL           0
/* SM ----> HI selector */
#define UEHSM_SMHISEL           0


/* UEH ----> SZ selector */
#define UEHSM_UEHSZSEL           0
/* SZ ----> UEH selector */
#define UEHSM_SZUEHSEL           2

/* SZ ----> SB selector */
#define UEHSM_SZSBSEL           0
/* SB ----> SZ selector */
#define UEHSM_SBSZSEL           0

/* SB ----> HI selector */
#define UEHSM_SBHISEL           0
/* HI ----> SB selector */
#define UEHSM_HISBSEL           0

/* EG ----> HI selector */
#define UEHSM_EGHISEL           0
/* HI ----> EG selector */
#define UEHSM_HIEGSEL           0

/* UEH ----> SM selector */
#define UEHSM_UEHSMSEL           0
/* SM ----> UEH selector */
#define UEHSM_SMUEHSEL           0

/* UEH ----> YS selector */
#define UEHSM_UEHYSSEL           0
/* YS ----> UEH selector */
#define UEHSM_YSUEHSEL           0

/* UEH ----> NH selector */
#define UEHSM_UEHNHSEL           0

/* UEH ----> UDH selector */
#define UEHSM_UEHUDHSEL          0

/* YS ----> UEH selector */
#define UEHSM_NHUEHSEL           0

/* UEH ----> PJ selector */
#define UEHSM_UEHPJSEL           0
/* PJ ----> UEH selector */
#define UEHSM_PJUEHSEL           0

/* UEH ----> RG selector */
#define UEHSM_UEHRGSEL           0
/* RG ----> UEH selector */
#define UEHSM_RGUEHSEL           0

/* UEH ----> EG selector */
#define UEHSM_UEHEGSEL           2
/* EG ----> UEH selector */
#define UEHSM_EGUEHSEL           2

#define UEHSM_LSZ_VAL_3        3
#define UEHSM_LSZ_VAL_5        5
#define UEHSM_LSZ_VAL_10       10 
#define UEHSM_LSZ_VAL_50       50

#define UEHSM_ENB_IPADDR       0x0A010101
#define UEHSM_MME_IPADDR       0x0A010102
#define UEHSM_SCTP_IP_ADDR     0x0A010102

#define UEHSM_LSZ_ENB_SCTPORT  10000
#define UEHSM_LSZ_MME_SCTPORT  36412

#define UEHSM_LSZ_ENB_PRC0     0
#define UEHSM_LSZ_MME_PRC1     1

#define UEHSM_MAX_PROFILE      50
#define UEHSM_LSZ_INV_NODE     10
#define UEHSM_LSZ_RESETRETRY   10

/* Peer Cfg */
#define UEHSM_LSZ_NUM_PEERS       1
#define UEHSM_LSZ_OUT_STRMS       3

/* Peer cntrl */
#define UEHSM_LSZ_PEERID       0

#define UEHSM_LSZ_MAX_PEER     1

#define UEHSM_HI_NUMBSAPS           10
#define UEHSM_HI_NMBCONS            10

#ifdef HI_MULTI_THREADED
#define UEHSM_HI_FDS                1024
#else
#define UEHSM_HI_FDS                64
#endif /* HI_MULTI_THREADED */
#define UEHSM_HI_FDBINS             4

#define UEHSM_HI_SELTIMEOUT         2
#define UEHSM_HI_UDP_MSGS_TOREAD    20
#define UEHSM_HI_NUMCL_TOACCEPT     5

#define UEHSM_HI_PERM_TSK_FLAG           TRUE
#define UEHSM_HI_SCHDTMR_VAL             1
/* timer resolution unused */
#define UEHSM_HI_TIME_RES                0
#define UEHSM_HI_STRT_THRESH             1
#define UEHSM_HI_DRP_THRESH              1
#define UEHSM_HI_STP_THRESH              1

#define UEHSM_HI_CONG_STRT                15000
#define UEHSM_HI_CONG_DRP                 20000
#define UEHSM_HI_CONG_STP                 10000
#define UEHSM_HI_NMB_HLBINS               16


#define UEHSM_LUEH_VAL_10                 100 
#define UEHSM_LUEH_MAX_BND_RETRY           3

#define SM_GETSBUF(_size, _datPtr, _ret)  _ret = SGetSBuf(DFLT_REGION,DFLT_POOL, (Data**)&_datPtr, _size)

#define     EVTUEHTUCLCFG       10
#define     EVTUEHSCTPCFG       11
#define     EVTUEHS1APCFG       12
#define     EVTUEHEGTPCFG       13
#define     EVTUEHAPPCFG        14
#define     EVTUEHRRCCFG        15
#define     EVTUEHPDCPCFG       16
#define     EVTUEHRLCCFG        17
#define     EVTUEHMACCFG        18
#define     EVTUEHCLCFG         19
#define     EVTUEHS1APSTKBND    20
#define     EVTUEHEGTPSTKBND    21
#define     EVTUEHRRCSTKBND     22
#define     EVTUEHENBDEBUGS     23
#define     EVTUEHENBALRMS      24
#define     EVTUEHINITS1SETUP   25
#define     EVTUEHCELLCFG       26
#define     EVTUEHUDHCFG        27
/*PWAV Events*/
#define     EVTBSMUEHCELLCFG    30
//#ifdef LTE_L2_MEAS
#define     EVTUEHMACL2MEASREQ  31
#define     EVTUEHRLCL2MEASREQ  32 
#define     EVTUEHPDCPL2MEASREQ 33
//#endif



#define UEH_SM_HI_DBG_MASK 0x01
#define UEH_SM_SB_DBG_MASK 0x02
#define UEH_SM_SZ_DBG_MASK 0x04
#define UEH_SM_EG_DBG_MASK 0x08
#define UEH_SM_UEH_DBG_MASK 0x10
#define UEH_SM_NH_DBG_MASK 0x20
#define UEH_SM_KW_DBG_MASK 0x40
#define UEH_SM_RG_DBG_MASK 0x80
#define UEH_SM_YS_DBG_MASK 0x100



#define INTERNAL_RECONF_TIMER_DUR      3000  
#define UEH_NUM_TQENTRY 100
#define UEH_MAX_SAPS 10
#define UEH_MEM_REGION 0
#define UEH_MEM_POOL 0
#define DATA_RATE_IN_MBPS               1024
#define EGU_DFLT_INTFTYPE               0
#define EGU_DFLT_EXPSEQNMB              1000
/* Changing the buffer size to accumulate huge buffers */
#define EGU_DFLT_MAXTPDUSZ              5000

#define EGTP_TUN_CREATE       1
#define EGTP_TUN_DEL          2

#define UEH_RLC_UM_DFLT_SN_LEN 10

#define UEH_RLC_AM_MODE      0
#define UEH_RLC_UM_MODE      1

#define UEHLAYERNAME "LTE-UEH"
#define TRANSID_NONE NULL
#define TRANSID_REJ_ADMIT_FAIL 0x0F

#define MAX_TIMER_NAME_LEN    50
#define TMR_UINTS_PER_SEC     1000 /*Timer units in milli secs*/
/* end timer Ids */

#define UEH_TMR_RES            1
#define UEH_APP_TQSIZE        64

#define UEH_DFLT_EGTP_PORT 2152

#define UEH_VAL_ZERO    0
#define UEH_VAL_ONE     1
#define UEH_VAL_TWO     2
#define UEH_VAL_THREE   3
#define UEH_SI_CNT      4

#define UE_SZ_MSG_CAT                   0x0F /*This will change if a new type of RRC peer msg is added. Max value is 0xE in RRC*/
#define UE_UNSUPPORTED_EVT              0xFFFF

/* RRC States TO remove */
#define UEH_RRC_CONNECTED               5
#define UEH_SRB2_ESTABLISHED            6

#define UEH_SRB_LC_GRP   0
/* UE related info */
#define UEH_PDCP_ID_SRB1 1
#define UEH_PDCP_ID_SRB2 2
#define UEH_PDCP_ID_DRB1 3
#define UEH_PDCP_ID_DRB2 4
#define UEH_PDCP_ID_DRB3 5
/* Cell related info */
#define UEH_CELL_CFG_DFLT_RBS 6

#define UEH_MCC_SIZE                    3
#define UEH_MNC_SIZE                    3
#define UEH_CELL_ID_LEN                 28
#define UEH_RNTI_LEN                    16

#define UEH_BYTE_LEN                    8
#define UEH_MIB_SPARE_LEN               10
#define UEH_SIB1_SUBFRM_NO              5
#define UEH_TRAC_LEN                    16

/* System Info Message Types */
#define UEH_SYS_MIB                       0
#define UEH_SYS_SIB1                      1
#define UEH_SYS_SIB2                      2
#define UEH_SYS_SIB3                      3
#define UEH_SYS_SIB4                      4
#define UEH_SYS_SIB5                      5

#define UEH_MAX_SIBS                      3
#define UEH_DISABLD_SIB                   7

#define UEH_MOD_CFG_SIB3_IDX              1
#define UEH_MOD_CFG_SIB4_IDX              2
#define UEH_MOD_CFG_SIB5_IDX              3

#define UEH_MAX_SFN                       1024
#define UEH_MAX_SFN_PAGING                1024
#define UEH_MAX_PO                        4
#define UEH_MAX_I_S                       4
#define MAX_PAGE_REC                      16
#define UEH_SFN_INCREMENT                 2
#define UEH_SFN_INCREMENT_TO_STORE_PAGMSG (UEH_SFN_INCREMENT + 1) /* To send paging message to RRC in next SFN */
#define UEH_MIB_PERIOD                    4
#define UEH_SIB1_PERIOD                   2
#define UEH_SIB2_PERIOD                   16

#define UEH_MIB_SUBFRAME                  0
#define UEH_SIB1_SUBFRAME                 5
#define UEH_SIB2_SUBFRAME                 0

#define UEH_MIB_PRESENT                   0x1
#define UEH_SIB1_PRESENT                  0x2
#define UEH_SIB2_PRESENT                  0x4

#ifdef SI_NEW
#define UEH_SIB3_PERIOD    32
#define UEH_SIB4_PERIOD    64
#define UEH_SIB5_PERIOD    64
#endif

#define UEH_MAX_SRBS                    2
#define UEH_MAX_RABS_IN_UE              2
//#define MAX_RAB_PER_UE                  UEH_MAX_RABS_IN_UE

#define UEH_MAX_TUNNEL_ID               50
#define UEH_MAX_PDCP_ID                 50

#define UEH_MEM_PDU_SIZE                2048

#define UEH_MTMSI_SIZE          4
#define UEH_MMEGRP_ID_SIZE      2

#define UEH_RGR_RB_CFG_LCH   0x1
#define UEH_RGR_RB_CFG_LCG   0x2

#define UEH_RB_RGRMSK_ZERO   0
#define UEH_RB_RGRMSK_DONE   3

#define UEH_UECAP_RGRMSK_NOTDONE   0
#define UEH_UECAP_RGRMSK_DONE      1
/* This macro is used to fill the transaction ID when deleting UE in RGR */
#define UEH_RGR_UE_DEL_TRANSID     0xFE
/* Security related info */
#define UEH_INTG_KEY_LEN 32 
#define UEH_CIPH_KEY_LEN 32 

#define UEH_STRING_S_LEN 7

#define UEH_CP_CIPH_KEY 0
#define UEH_UP_CIPH_KEY 1
#define UEH_INTG_KEY    2
#define MAX_S1AP_CON 100

#define NOT_APPLICABLE -1

#define UEH_MAX_CELL 1
#define BCCH_BCH_ID    1
#define BCCH_SIB1_ID   2
#define BCCH_SIB_OTHERS_ID   3
#define CCCH_DL_ID   4
#define CCCH_UL_ID   5
#define PCCH_ID      6

#ifdef WIRESHARK_LOG /* Madhur Including for wireshark logging */
#define MAC_LTE_START_STRING        "mac-lte"
#define MAC_LTE_RNTI_TAG            0x02
#define MAC_LTE_UEID_TAG            0x03
#define MAC_LTE_SUBFRAME_TAG        0x04
#define MAC_LTE_PREDFINED_DATA_TAG  0x05
#define MAC_LTE_RETX_TAG            0x06
#define MAC_LTE_CRC_STATUS_TAG      0x07
#define MAC_LTE_PAYLOAD_TAG         0x01

static int                     g_sockfd, g_sockfd1;
static struct sockaddr_in      g_serv_addr, g_serv_addr1;
#endif

#define UEH_NH_MAX_MSG_TYPE   11
#define UEH_NH_MAX_MSG_CLASS  5

//#define SZ_MAX_MSG_ID 63
//#define SZ_MEM_SDU_SIZE     2048

#define UEH_PRNT_BUF uehCb.init.prntBuf
#define DBGMASK_SM      1
#define EUEHBASE     0
#define ERRUEH       (EUEHBASE + 0)  /* reserved */
#define EUEH001      (ERRUEH +    1) /* ve_sm_hi_exms.c: 241 */

#define SIZE_OF_IPV4                    4
#define SIZE_OF_IPV6                    16

#define NH_UEID_ZERO                   0
#define NH_UEID_INVALID                0xFFFF
/* HIGH LEVEL UE procedure codes
 * This is RRC+S1 combined procedure 
#define UEH_RRC_CON_SETUP              1
#define UEH_INITIAL_CXT_SETUP          2
#define UEH_UE_CXT_REL                 3
#define UEH_ERAB_SETUP                 4
#define UEH_ERAB_REL                   5
#define UEH_MEAS_CFG                   6
#define UEH_OUT_HO_ONGOING             7
#define UEH_IN_HO_ONGOING              8
#define UEH_REEST_ONGOING              9
#define UEH_UE_CXT_REL_DUE_TO_RL_FAIL  10
#define UEH_UE_CONN_REL_ONGOING        11 
#define UEH_UE_CTXT_CLEAR_ONGOING      12  
#define UEH_INITIAL_CXT_SETUP_FAILURE  13 
#define UEH_UE_REL_REQ_SENT            14
#define UEH_CALL_CONNECTED             15
#define UEH_PUCCH_PWR_REC_ONGOING      16
#define UEH_INITIAL_UE_MSG             17
*/
#define MAX_RNTI_POOL (uehCb.uehCfgCb.cellCfg.maxConnectedUes + UEH_RESERVED_RNTI_POOL_SIZE)

#define UEH_ERROR 1


// To remove global variable uehDfltNumCmnLcs this macro is defined.
//#define   NUM_LOGICAL_CHANNELS 6
//MAC related debug logs
#define   MAC_DBG_ERROR DBGMASK_ERR
#define   MAC_DBG_INFO  DBGMASK_INFO
#define   MAC_DBG_MOD   DBGMASK_PRM


//S1AP related debug logs
#define   S1AP_DBG_INFO  (DBGMASK_LYR | LSZ_DBGMASK_MF | LSZ_DBGMASK_DB | LSZ_DBGMASK_ASN | \
                          DBGMASK_LI | DBGMASK_MI | DBGMASK_UI)
#define   S1AP_DBG_TMR   LSZ_DBGMASK_TMR

//RLC related debug logs
#define   KW_ALL_MOD     (KW_DBGMASK_DUT | KW_DBGMASK_TM | KW_DBGMASK_CFG | KW_DBGMASK_UM | KW_DBGMASK_AM | KW_DBGMASK_DL | KW_DBGMASK_UL | KW_DBGMASK_LMM)
#define   KW_PJ_ALL_MOD  (KW_PJ_DBGMASK_CFG | KW_PJ_DBGMASK_UTL | KW_PJ_DBGMASK_ULM | KW_PJ_DBGMASK_INF | KW_PJ_DBGMASK_DLM | KW_PJ_DBGMASK_LMM)

#define   KW_DBG_INFO    (KW_DBGMASK_BRIEF | KW_DBGMASK_DETAIL | KW_ALL_MOD | KW_PJ_ALL_MOD)
#define   KW_DBG_ERROR   (KW_DBGMASK_ERROR | KW_ALL_MOD | KW_PJ_ALL_MOD)

//RRC related debug logs
/*
#ifndef UEH_MAIN_H
#define   NH_DBG_INFO  LNH_DBGMASK_INFO
#define   NH_DBG_ERROR (LNH_DBGMASK_ERROR | LNH_DBGMASK_FATAL)
#define   NH_DBG_FUNC  LNH_DBGMASK_DBG
#endif
*/
//CL related debug logs

#define   YS_DBG_ERROR 2  // Value of YS_DBGMASK_ERR 
#define   YS_DBG_MOD   3  // Value of YS_DBGMASK_PARAM
#define   YS_DBG_INFO  4  //YS_DBGMASK_INFO

//SCTP related debug logs
#define   SB_DBG_INFO  (SB_DBGMASK_GEN | SB_DBGMASK_SQ | SB_DBGMASK_SG | SB_DBGMASK_DB | SB_DBGMASK_PM | \
			SB_DBGMASK_AC | SB_DBGMASK_AS | SB_DBGMASK_VA | SB_DBGMASK_CM | SB_DBGMASK_MTU)

//EGTP related debug logs
//#ifndef UEH_MAIN_H
//#define   EG_DBG_INFO   LEG_DBGMASK_INFO | DBGMASK_MI | DBGMASK_UI
//#define   EG_DBG_ERROR  (LEG_DBGMASK_ERROR | LEG_DBGMASK_FATAL | LEG_DBGMASK_WARN)
//#define   EG_DBG_MSG    (LEG_DBGMASK_MSG | LEG_DBGMASK_FLOW)
//#endif

//TUCL related debug logs
#define   HI_DBG_INFO  (DBGMASK_LYR | DBGMASK_MI | DBGMASK_UI | DBGMASK_SI)

/* Akash: SaveInput prototype - start */
#define SAVEQLEN                          5
#define MAX_SIZE_UE_CAPBILITY_RAT_INFO    100

#define CALLTRCBUFLEN       100
#define CALLTRCFLUSHPERIOD  100  /* In multiple of RRM Tick, if RRMTICK=10ms, then 100 means 10*100 ms = 1sec periodicity */



#define UEH_EVNT_TRC_MAX_LEN                            255
#define UEH_EVNT_TIME_STR_MAX_LEN                       30
#define UEH_EVNT_DETAIL_MAX_LEN                         30

#ifdef LTE_L2_MEAS

#define MAX_NUM_COUNTERS    14  /*At L2*/
#define MAX_NUM_PROFILES     2
#define MAX_NUM_QCI          9
#define L2M_NUM_SAMPLES     10
#define L2M_SAMPLE_PERIOD  100
#define PROFILEID_INVALID    0

#define UEH_L2M_MASK_CLEAR                 0x0000
#define UEH_L2M_MASK_AVG_NUM_ACTV_UE_QCI   0x0001
#define UEH_L2M_MASK_PKT_LOSS_RATE_DL_QCI  0x0002
#define UEH_L2M_MASK_PKT_DELAY_DL_QCI      0x0004
#define UEH_L2M_MASK_PKT_DISC_RATE_DL_QCI  0x0008
#define UEH_L2M_MASK_PKT_LOSS_UL_QCI       0x0010
#define UEH_L2M_MASK_TOTAL_PRB_USAGE_DL    0x0020 
#define UEH_L2M_MASK_TOTAL_PRB_USAGE_UL    0x0040
#define UEH_L2M_MASK_PRB_USAGE_DL_QCI      0x0080
#define UEH_L2M_MASK_PRB_USAGE_UL_QCI      0x0100
#define UEH_L2M_MASK_RECV_PREAMBLES        0x0200
#define UEH_L2M_MASK_NUM_ACTV_UE_QCI_DL    0x0400
#define UEH_L2M_MASK_NUM_ACTV_UE_QCI_UL    0x0800

#define UEH_L2M_INV_AVG_NUM_ACTV_UE_QCI   0xFFFE
#define UEH_L2M_INV_PKT_LOSS_RATE_DL_QCI  0xFFFD
#define UEH_L2M_INV_PKT_DELAY_DL_QCI      0xFFFB
#define UEH_L2M_INV_PKT_DISC_RATE_DL_QCI  0xFFF7
#define UEH_L2M_INV_PKT_LOSS_UL_QCI       0xFFEF
#define UEH_L2M_INV_TOTAL_PRB_USAGE_DL    0xFFDF 
#define UEH_L2M_INV_TOTAL_PRB_USAGE_UL    0xFFBF
#define UEH_L2M_INV_PRB_USAGE_DL_QCI      0xFF7F
#define UEH_L2M_INV_PRB_USAGE_UL_QCI      0xFEFF
#define UEH_L2M_INV_RECV_PREAMBLES        0xFDFF
#define UEH_L2M_INV_NUM_ACTV_UE_QCI_DL    0xFBFF
#define UEH_L2M_INV_NUM_ACTV_UE_QCI_UL    0xF7FF

#endif

#define EMERGENY_CALL 0
#define MAX_UE_SENT_TO_BSM       3
#define MAX_UE_PROC_SENT_TO_BSM  20
#define MAX_UE_PAGING_STORED     30




#define   UE_PAGING_IDX                 120
#define   LOG_ALL_UES                   65535
#define   LOG_ALL_EVENTS                1
#define   LOG_FAILURE                   2
#define   LOG_ONLY_PROCEDURE            3
#define   LOG_UES                       4 
#define   RRC_CONN_EST_SUCC             1
#define   SEC_PROC_SUCC                 2 
#define   DIR_TRANS_SUCC                4
#define   CAPAB_PROC_SUCC               8 
#define   RRC_RE_CFG_PROC_SUCC          16 
#define   INITIAL_CTX_SETUP_SUCC        32
#define   UE_CTX_RLS_SUCC               64
#define   PAGING_RNTI                   65534
#define   MAX_EVT_IN_HIS                50


#endif
