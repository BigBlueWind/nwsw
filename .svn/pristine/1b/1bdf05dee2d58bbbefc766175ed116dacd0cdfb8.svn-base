/** @file apidefs.h
 *
 * @brief API definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.97 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef APIDEFS_H_
#define APIDEFS_H_

#include "basetypes.h"

//
// Max message size
//
#define MSG_MAXSIZE             16384

//
// Control field bitmask
//
#define CTRL_ACKSUPPRESS        0x00000001  // ACK suppression bit
#define CTRL_EXTHEADER          0x00000002  // Extended header present
#define CTRL_BYPASS             0x00000004  // Foreign protocol encapsulation
#define CTRL_USETID             0x00000010  // Use Transaction ID
#define CTRL_TIDMASK            0x0000FF00  // Transaction ID's position
#define CTRL_VERSMASK           0xF0000000  // Version is encoded in 4 MSBs
#define CTRL_VERSION            0x10000000  // Version 1
#define CTRL_DEFAULT            0x10000000  // Default value (version 1)
#define CTRL_TIDSHIFT           8           // Shift by number of bits (Transaction ID)

//
// Message types
//
#define MSGT_CONFIG             0x0001      // Configuration
#define MSGT_ANSWER             0x0002      // Answer to configuration message
#define MSGT_QUERY              0x0003      // Request data
#define MSGT_RESPONSE           0x0004      // Response to data request
#define MSGT_INDICATION         0x0005      // Indication of some event
#define MSGT_DATA               0x0006      // Data message
#define MSGT_LAST               0x0006      // DO NOT FORGET TO REDEFINE!

//
// Message source/destination
//
// Statically assigned IDs
#define MSGS_SVSR               0x0001      // Supervisor (U-ARM)
#define MSGS_HOST               0x0002      // Host
#define MSGS_MAC                0x0003      // Default BS MAC / eNodeB
#define MSGS_MSMAC              0x0004      // Default MS MAC / UE
#define MSGS_AUX                0x0004      // External Interface (== MS MAC)
#define MSGS_RRC                0x0005
#define MSGS_RLC                0x0006
#define MSGS_PDCP               0x0007
#define MSGS_OAM                (MSGS_HOST)

// R*** U-ARM Instances
#define MSGS_ENBSM              0x0008  	// Stack Manager
#define MSGS_ENBAPP             0x0009  	// Application

#define MSGS_LAST               0x0009      // DO NOT FORGET TO REDEFINE!

//
// L-ARM Instance IDs
//
#define MSGS_WIMAXPHY           0x0018
#define MSGS_LTEPHY             0x0019
#define MSGS_LTEMAC             0x001C
#define MSGS_LTEPHY_EX          0x0026
#define MSGS_WMXMSPHY           0x0088      // MS PHY

#define MSGS_LARM_MAC           0x0033      // R-MAC on L-ARM 
#define MSGS_LARM_RLC           0x0034
#define MSGS_LARM_PDCP          0x0035

//
// Message routing
//
#define MSGS_ROUTE              0xF000      // Routing mask
#define MSGS_UARM               0x0000      // U-ARM
#define MSGS_LOCAL              0x4000      // Local side
#define MSGS_LARM               0x8000      // L-ARM

//
// Maximum number of dynamic instances
//
#define MSGS_DYNAMIC            64

//
// Message IDs
//
//Message ID ranges:
//
// 0x0000 - 0x0FFF : framework related messages
//
// 0x4000 - 0x4FFF : MAC related messages
// 0x5000 - 0x5FFF : PHY related messages
// 0x6000 - 0x6FFF : VoIP related messages
//
// 0x8000 - 0x8FFF : Data passing messages
//
// 0xF000 - 0xFFFF : Diagnostic related messages
//  - with 0xF000 - 0xF7FF being for software diags
//  - and  0xF800 - 0xF8FF being for hardware diags

// System
#define SYS_FIRST                   0x0001
#define SVSR_LOADLARM               0x0001
#define SVSR_LOADDSP                0x0002
#define SVSR_LOADUARM               0x0003
#define SVSR_RESET                  0x0004
#define SVSR_IOPARAMS               0x0005
#define SVSR_UNLOAD                 0x0006
#define SVSR_APIMSGSTATS        0x0007
#define SYS_LAST                    0x0007      // DO NOT FORGET TO REDEFINE!

// MAC-PHY
#define MAC_FIRST                   0x4000
#define SVSR_CREATEMAC              0x4000
#define SVSR_DESTROYMAC             0x4001
#define SVSR_CREATEPHY              0x4002
#define SVSR_DESTROYPHY             0x4003
#define SVSR_PHYTICK			    0x4004
#define MAC_LAST                    0x4004      // DO NOT FORGET TO REDEFINE!
// TODO: Split ranges
#define MAC_CREATECHAN              0x4005
#define MAC_TXREADY_IND             0x4006
#define MAC_RXBURST_IND             0x4007
#define MAC_RXMACPDU_IND            0x4008
#define MAC_RXREADY_IND             0x4009
#define MAC_RXSDU_IND               0x400A
#define MAC_PHYREADY_IND            0x400B
#define MAC_CONFIGURATION           0x400C
#define MAC_TEST_CONFIG             0x400D
#define MAC_STATUS                  0x400E
#define MAC_PACKETGEN               0x400F

// RRC-MAC APIs
#define MAC_CONFIG_CELL             0x4010
#define MAC_RECONFIG_CELL           0x4011
#define MAC_CREATE_UE_ENTITY        0x4017
#define MAC_RECONFIGURE_UE_ENTITY   0x4018

#define MAC_MACPHYIFMSG_IND         0x4019
#define PHY_LTE_START               0x401A
#define PHY_LTE_STOP                0x401B
#define MAC_TEST_MAP_CONF           0x401C
#define MAC_RXSTATUS_IND			0x401D

#define MAC_TEST_TXVECTOR_CFG        0x4020
#define MAC_TEST_RXVECTOR_CFG        0x4021

// MAC-RRC parameters (API INFORMATION ELEMENTS)
#define PAR_RECONFIGURE_LC          0x4019
#define PAR_DL_LC_RECONFIG_TEST     0x401a
#define PAR_UL_LC_RECONFIG_TEST     0x401b
#define PAR_CREATE_LC               0x4012
#define PAR_UL_LC_CREATE            0x4013
#define PAR_DL_LC_CREATE            0x4014
#define PAR_DL_LC_RECONFIG          0x4015
#define PAR_UL_LC_RECONFIG          0x4016

// PHY
#define PHY_FIRST                   0x5000
#define PHY_TICK                    0x5000
#define PHY_WIMAX_API               0x5005
#define PHY_WIMAX_PHY_CONTEXT       0x5006
#define PHY_WIMAX_DLMAP             0x5007
#define PHY_WIMAX_ULMAP             0x5008
#define PHY_WIMAX_TX_TASK_LIST      0x500D
#define PHY_WIMAX_RX_TASK_LIST      0x500E
#define PHY_WIMAX_DEBUG_MSG         0x500F
#define PHY_WIMAX_RXSDU_DATA        0x5009
#define PHY_WIMAX_RX_IQ             0x500B
#define PHY_WIMAX_TX_IQ             0x500C
#define PHY_WIMAX_RX_BUF_IQ         0x5010
#define PHY_WIMAX_TX_BUF_IQ         0x5011
#define PHY_LAST                    0x5011      // DO NOT FORGET TO REDEFINE!

// PHY LTE specific
#define PHY_LTE_MSG                 0x5105
#define PHY_LTE_MSG_EX              0x5107
#define PHY_LTE_MSG_LIST            0x5108
#define PHY_LTE_DEBUG_MSG           0x510E
#define PHY_LTE_RX_IQ               0x5110
#define PHY_LTE_TX_IQ               0x5111
#define PHY_LTE_PHYTICK_MSG         0x5112
#define PHY_LTE_CPTIBILITY_MSG      0x5113
#define PHY_LTE_SEND_IQ_ENA         0x5114
#define PHY_LTE_SEND_IQ_DIS         0x5115
#define PHY_LTE_RCV_IQ_ENA          0x5116
#define PHY_LTE_RCV_IQ_DIS          0x5117

#define PHY_LTE_GET_SAMPLES         0x5119
#define PHY_LTE_TIMER_STOP          0x511A
#define PHY_LTE_FD_MSG              0x511B
//#define PHY_LTE_START               0x511C
//#define PHY_LTE_STOP                0x511D

#define PAR_COMMIQ_PARAM_GROUP      20
#define PAR_COMMIQ_MODE             10
#define PAR_COMMIQ_L1HDR            11
#define PAR_COMMIQ_L2MODE           12
#define PAR_COMMIQ_L2HDR            13
#define PAR_COMMIQ_DATACHUNKSIZE    14
#define PAR_COMMIQ_NUMANT           15
#define PAR_COMMIQ_CHANBW           16

// VoIP
#define VOIP_FIRST                  0x6000
#define VOIP_LAST                   0x6000      // DO NOT FORGET TO REDEFINE!

// LTE RRC
#define RRC_FIRST                   0x7000
#define SVSR_CREATERRC              0x7000
#define SVSR_DESTROYRRC             0x7001

//RRC API
#define RRC_CONNECT_REQUEST         0x7001
#define RRC_CONNECT_SETUP           0x7002
#define RRC_CONNECT_RECONFIGURATION 0x7003
#define RRC_CONNECT_COMPLETE        0x7004
#define RRC_CONNECT_RECONFIGURATION_COMPLETE 0x7005
#define RRC_CONNECT_RELEASE         0x7006
#define RRC_DEBUG_DATA              0x7007
#define RRC_CONNECT_REJECT          0x7008
#define RRC_DL_INFORMATION_TRANSFER 0x7009
#define RRC_UL_INFORMATION_TRANSFER 0x700A
#define RRC_UECAPABILITY_ENQUIRY    0x700B
#define RRC_UECAPABILITY_INFORMATION 0x700C

#define RRC_LAST                    0x7111      // DO NOT FORGET TO REDEFINE!

#define MMI_FIRST				    0x8000
#define	MMI_UT_SYSTEM			    0x8000
#define MMI_LAST				    0x8000

#define	MMI_SWITCH_ON			    0x8001
#define	MMI_SWITCH_OFF			    0x8002
#define	MMI_POWER_ON			    0x8003
#define	MMI_POWER_OFF			    0x8004
#define	MMI_PLMN_MANUAL			    0x8005
#define	MMI_PLMN_AUTOMATIC		    0x8006
#define SYSTEM_CTRL_REQ             0xF007
#define DRB_COMMON_REQ              0xF278

// Internal diagnostics (debug)
#define DBG_FIRST                   0xD000
#define DIAG_CREATETASK             0xD000
#define DIAG_DESTROYTASK            0xD001
#define DIAG_CREATESEMA             0xD002
#define DIAG_DESTROYSEMA            0xD003
#define DIAG_CREATEMUTEX            0xD004
#define DIAG_DESTROYMUTEX           0xD005
#define DIAG_ECHO                   0xD00E
#define DBG_LAST                    0xD00E      // DO NOT FORGET TO REDEFINE!

// Software Diagnostic
#define DIAG_FIRST                  0xF000
#define DIAG_CONFDIAG               0xF000
#define DIAG_DIAGENASTRM            0xF001
#define DIAG_DIAGDISSTRM            0xF002
#define DIAG_DIAGBUFFER             0xF003
#define DIAG_DIAGIND                0xF004
#define DIAG_DIAGREDIR              0xF005
#define DIAG_ALARMIND               0xF006
#define DIAG_FORCEALARM             0xF007
#define DIAG_SYSSHUTDOWN            0xF008
#define DIAG_LOOPBACKENA            0xF009
#define DIAG_LOOPBACKDIS            0xF00A
// 0xF080 - 0xF0FF: Device level stats
#define DIAG_DISPSTATS              0xF080
#define DIAG_ETHSTATS               0xF081      // Ethernet statistics
#define DIAG_PCIESTATS              0xF082      // PCIe statistics
#define DIAG_SRIOSTATS              0xF083      // SRIO statistics
#define DIAG_CPRISTATS              0xF084      // CPRI statistics
#define DIAG_PROCOCCSTATS           0xF085
#define DIAG_LAST                   0xF085      // DO NOT FORGET TO REDEFINE

// Application software diagnostics
#define APP_DIAG_PROFENA    0xF201
#define APP_DIAG_PROFDIS    0xF202
#define APP_DIAG_PROFSTAT   0xF203

// L-ARM Diagnostic
#define LARM_SVSR_READY             0xF800
#define DIAG_LOOPBACK               0xF802
#define DIAG_CHECK_API_LAT          0xF804
#define LARM_HDIAG_FIRST                  0xF900
#define DIAG_OUTPUT                 0xF903
#define LARM_HDIAG_CEVADUMP             0xF908
#define LARM_HDIAG_QCEVADUMP             0xF909
#define LARM_HDIAG_ICPUTEST              0xF90A
#define LARM_HDIAG_LAST                  0xF90A      // DO NOT FORGET TO REDEFINE

// L-ARM Api & error codes
#define SVSR_READ_MEMORY            0xF807
#define SVSR_WRITE_MEMORY           0xF808

#define SCTP_WRITE_ERROR            0x02211E
#define SCTP_READ_ERROR             0x02211F
#define ICPU_WRITE_ERROR            0x022120
#define ICPU_READ_ERROR             0x022121


//
// Parameter definitions
//

// Common
#define PAR_RESULT                  0x0001      // Result code of operation
#define PAR_RESULTSTR               0x0002      // Result string
#define PAR_QUEUEPTR                0x0002      // Pointer to a MAC2PHY_QUEUE_EL structure
// LOADLARM, LOADDSP
#define PAR_PATH                    0x0003      // Path to executable
// CREATE[...]
#define PAR_INSTID                  0x0004      // Instance ID
#define PAR_DOMAIN                  0x0005      // Domain
#define PAR_TYPE                    0x0006      // Instance type
#define PAR_ASSOCID                 0x0007      // Associated ID
#define PAR_BIN_DATA                0x0008      // Binary data
#define PAR_TXBURST_DATA            0x0009      // won't be encasulated by MAC layer
#define PAR_TXMACSDU_DATA           0x000A      // will be encapsulated by MAC layer to get correct PDU
#define PAR_CREATE_CONFIG           0x000B      // will be encapsulated by MAC layer to get correct PDU
#define PAR_CONFIG_ID               0x000C
#define PAR_MODE                    0x000C
#define PAR_NUMRETRIES              0x000D
#define PAR_DRAIN_RX_SDU            0x000E      // Drain Rx SDU mode on/off

// CreateMAC
#define PAR_CHANNELBANDWIDTH        0x0010      // will be encapsulated by MAC layer to get correct PDU
#define PAR_FRTYPEDUPLEXMODE        0x0011
#define PAR_RADIOACCESSMODE         0x0012
#define PAR_PHYSICALRESOURCEBANDWIDTH 0x0013
#define PAR_NUMBERRESOURCEBLOCKSPERSLOT 0x0014
#define PAR_PHYLAYERCELLIDGROUP     0x0015
#define PAR_PHYLAYERID              0x0016
#define PAR_TXANTENNAPORTCOUNT      0x0017
#define PAR_FASTFWDORPRECLOCKINGNUMBER 0x0018
#define PAR_FFTSIZE                 0x0019
#define PAR_NUMBERUSEDSUBCARRIERS   0x001A
#define PAR_NMAXDLRB                0x001B
#define PAR_NMAXULRB                0x001C
#define PAR_REFERENCESIGNALPOWER    0x001D
#define PAR_PRIMARYSYNCSIGNALPOWER  0x001E
#define PAR_SECONDARYSYNCSIGNALPOWER 0x001F
#define PAR_PRIMARYSYNCSIGNALSTATE  0x0020
#define PAR_SECONDARYSYNCSIGNALSTATE 0x0021
#define PAR_NUMDATAREPERPRB         0x0022
#define PAR_CYCLICPREFIXTYPE        0x0023
#define PAR_RXANTENNAPORTCOUNT      0x0024
#define PAR_TXSDUCONFENABLE         0x0025
#define PAR_RADIOFRAMENUMBER        0x0026
#define PAR_SUBFRAMENUMBER          0x0027
#define PAR_SLOTNUMBER              0x0028

/* SRS */
#define PAR_SRSENABLE               0x0029
#define PAR_SRSBANDWIDTHCONFIG      0x002A
#define PAR_SRSSUBFRAMECONFIG       0x002B
#define PAR_SRSSIMULTANEOUSANANDSRS 0x002C
/* PRACH */
#define PAR_PRACHCONFIGURATIONINDEX 0x002D
#define PAR_PRACHFREQUENCYOFFSET    0x002E
#define PAR_PRACHHIGHSPEEDFLAG      0x002F
#define PAR_PRACHCYCLICSHIFTCONFIG  0x0030
#define PAR_PRACHROOTSEQUENCEINDEX  0x0031

#define PAR_RXSDULINEMONINFOENA 0x0040
#define PAR_PHICHDURATION       0x0041
#define PAR_PHICHRESOURCE       0x0042

#define PAR_ULDLCFG             0x0043
#define PAR_SPECIAL_SF_CFG      0x0044

/* MAC set test config */
#define PAR_TEST_MODEL              0x0070
#define PAR_N_CW                    0x0072
#define PAR_N_LAYERS                0x0073
#define PAR_PRECODING_MODE          0x0074
#define PAR_DURATION                0x0075
#define PAR_RNTI_BOOSTED            0x0076
#define PAR_RNTI_DEBOOSTED          0x0077
#define PAR_RS_BOOSTING             0x0078
#define PAR_EPRE_SINCHRO            0x0079
#define PAR_RESERVED_EPRE           0x007A
#define PAR_PBCH_EPRE               0x007B
#define PAR_PBCH_RESERVED_EPRE      0x007C
#define PAR_N_CTRL_SYMBOLS          0x007D
#define PAR_PCFICH_EPRE             0x007E
#define PAR_N_PHICH_GROUP           0x007F
#define PAR_N_PHICH_PER_GROUP       0x0080
#define PAR_PHICH_SYM_POWER         0x0081
#define PAR_PHICH_GROUP_EPRE        0x0082
#define PAR_N_AVALABLE_REGS         0x0083
#define PAR_N_PDCCH                 0x0084
#define PAR_N_CCE_PER_PDCCH         0x0085
#define PAR_N_REG_PER_CCE           0x0086
#define PAR_N_REG_ALL_TO_PDCCH      0x0087
#define PAR_N_NIL_REG_PADDING       0x0088
#define PAR_PDCCH_REG_EPRE          0x0089
#define PAR_NIL_REG_EPRE            0x008A
#define PAR_N_PDSCH_BOOSTED_PRB     0x008B
#define PAR_PDSCH_BOOSTED_PRB_PA    0x008C
#define PAR_N_PDSCH_DEBOOSTED_PRB   0x008D
#define PAR_PDSCH_DEBOOSTED_PRB_PA  0x008E
#define PAR_PDSCH_BOOSTED_MODULATION 0x008F
#define PAR_PDSCH_DEBOOSTED_MODULATION 0x0090
#define PAR_PUSCH_BOOSTED_PRB       0x0091
#define PAR_PUSCH_BOOSTED_PRB_PA    0x0092
#define PAR_PUSCH_BOOSTED_MODULATION    0x0093
#define PAR_PUSCH_DEBOOSTED_PRB_PB  0x0094
#define PAR_PUSCH_DEBOOSTED_PRB_PA  0x0095
#define PAR_PUSCH_DEBOOSTED_MODULATION  0x0096
#define PAR_RNTI_PUSCH           0x0097
#define PAR_PUSCH_SCRINITVAL           0x0097
#define PAR_DCI_SET                 0x0098
#define PAR_RXNUMBYTES        0x0099
#define PAR_DIRMODE        0x009A

#define PAR_PUSCH_DED_N_ACK  0x9B
#define PAR_PUSCH_DED_ACKIDX  0x9C

#define PAR_PDSCH_MCS_ARRAY_CW1 0x9D
#define PAR_PDSCH_MCS_ARRAY_CW2 0x9E
#define PAR_PUSCH_MCS_ARRAY 0x9F

#define PAR_SFN                     0x00A0
#define PAR_PHY_UL_MAP              0x00A1

#define PAR_CDDTYPE                 0x00A2
#define PAR_CODEBOOKIDX                 0x00A3
#define PAR_RV_DL                 0x00A4

#define PAR_PHICH_RESOURCE      0x00A5
#define PAR_PHICH_DURATION      0x00A6
#define PAR_PHICH_IDX           0x00A7
#define PAR_PDSCH_PAYLOAD               0x00A8


#define PAR_BCH_DATA_SET            0x00B1
#define PAR_MCS_DL                     0x00B2
#define PAR_RV                      0x00B3
#define PAR_MCS_UL                     0x00B4
#define PAR_TXMODE                     0x00B5

//PUCCH parameters
#define PAR_PUCCH_DELTA_SHIFT            0x00B6
#define PAR_PUCCH_nRBCQI                 0x00B7
#define PAR_PUCCH_nCSAN                  0x00B8
#define PAR_PUCCH_n1AN                   0x00B9
#define PAR_PUCCH_FORMAT                 0x00BA
#define PAR_PUCCH_CYCLIC_SHIFT		 0x00BB
#define PAR_UL_QAM64_ENA                 0x00BC
#define PAR_PUCCH_DLCQI_SIZE_BITS      0x00BD

#define PAR_TIMADVERR_DFLT_PUCCH      0xC0
#define PAR_TIMADVERR_DFLT_PUSCH      0xC1

#define PAR_PUSCH_NUM               0x00C9
#define PAR_PUCCH_NUM               0x00CB

#define PAR_SWEEP_DL_MAP_PDSCH1 0x00D0
#define PAR_SWEEP_DL_MAP_PDSCH2 0x00D1

#define PAR_PDSCH_SCRINITVAL    0xE1
#define PAR_PDSCH_MAP    0xE2

#define PAR_PDCCH_MAP    0xE5
#define PAR_PDSCH    0xE6
#define PAR_PUSCH_MAP    0xE8

#define PAR_PUCCH_SCRINITVAL    0xEA
#define PAR_PUCCH_MAP    0xEB
#define PAR_PUCCH_HARQ_SIZE_BITS    0xEC
#define PAR_PUCCH_HARQ_IDX    0xED
#define PAR_PUCCH_CQI_RSRC_IDX    0xEE

#define PAR_PUCCH_IDX0  0xEF
#define PAR_PUCCH_IDX1  0xF0
#define PAR_PUCCH_IDX2  0xF1
#define PAR_PUCCH_IDX3  0xF2
#define PAR_ACK_NACK_REPORT_MODE 0xF3
#define PAR_NBUNDLED  0xF4

/* LTE MAC result codes */
#define RC_LTEMAC_OK                0x0000
#define RC_LTEMAC_UNSPEC_ERROR      0x0001
#define RC_LTEMAC_UNKNOWN_PARAM     0x0002
#define RC_LTEMAC_PARAM_ERROR       0x0003
#define RC_LTEMAC_ALLOC_ERROR       0x0004
#define RC_LTEMAC_UNKNOWN_COMMAND   0x0005

#define PAR_MAC_STATUS_IRNGREADY    0x00A0
#define PAR_MAC_STATUS_IRNGDONE     0x00A1
#define PAR_MAC_STATUS_PRNGREADY    0x00A2
#define PAR_MAC_STATUS_SBCDONE      0x00A3
#define PAR_MAC_STATUS_PKMDONE      0x00A4
#define PAR_MAC_STATUS_REGDONE      0x00A5
#define PAR_MAC_STATUS_DHCPDONE     0x00A6
#define PAR_MAC_STATUS_TIMEOFDAYDONE 0x00A7
#define PAR_MAC_STATUS_TFTPDONE     0x0A80
#define PAR_MAC_STATUS_DSADONE      0x00A9
#define PAR_MAC_STATUS_DSADONEUL    0x0A90
#define PAR_MAC_STATUS_DSADONEDL    0x0A91
#define PAR_MAC_STATUS_NETWENTRYDONE 0x000AA
#define PAR_PACKETGEN_FINISHED      0x000AC
// CREATETASK, DESTROYTASK, ...
#define PAR_TASKPARAM               0xD005
#define PAR_TASKNAME                0xD006
#define PAR_TASKPRIORITY            0xD007
#define PAR_STACKBASE               0xD008
#define PAR_STACKSIZE               0xD009
// ECHO
#define PAR_ECHO                    0xD00E

// parameter ID
#define PHY_QUE_HDR_PARAM_ID        2

// PHY_WIMAX_TX_IQ
#define PAR_PHY_SECTOR              2
#define PAR_PHY_BUF_SIZE            3
#define PAR_PHY_BUF_OFFSET          4
#define PAR_PHY_DATA                5
#define PAR_PHY_ANT_NUM             6
// PHY_WIMAX_LTE_API
#define DIAG_BUF_SIZE_PARAM_ID      7
#define DIAG_BUF_OFFS_PARAM_ID      8
#define DIAG_DATA_PARAM_ID          9
#define DIAG_DSP_CYCLES             12
// DIAG_OUTPUT
#define UNCODED_OUT                 1
#define FEC_DEC_OUT                 2
#define PROFILE_INFO                3
#define IQ_SAMPLES                  4
#define DIAG_DATA_TYPE              50

// Throughput
#define PAR_DIAG_COUNTERS           5
#define PAR_THR_FULLEXEC            6
#define PAR_THR_TX                  7
#define PAR_THR_RX                  8
#define PAR_THR_TXVECT              9
#define PAR_THR_RXVECT              10
#define PAR_THR_TXSDU               11 

#define PAR_DIAG_FULLEXETIME        6
#define PAR_DIAG_TXEXETIME          7
#define PAR_DIAG_RXEXETIME          8
#define PAR_DIAG_TXVECT             9
#define PAR_DIAG_RXVECT             10
#define PAR_DIAG_TXSDU              11
#define PAR_DIAG_TX1                12
#define PAR_DIAG_TX2                13
#define PAR_DIAG_TX3                14
#define PAR_DIAG_TX4                15

// Loopback
#define PAR_LPB_DATA                4

//
// Parameter values
//

// PAR_DOMAIN
#define PAR_WIMAX                   1
#define PAR_LTE                     2
#define PAR_3G                      3

// PAR_TYPE
#define PAR_BSMAC                   1
#define PAR_MSMAC                   2
#define PAR_STUB                    3
#define PAR_CUSTOM                  4

#define PAR_HCS_CORRECT             0x481
#define PAR_HDR_TYPE                0x482
#define PAR_CRC_CORRECT             0x483

#define PAR_RXPDU_STATS             0x480   // to report all
#define PAR_NUMRXPDU_HCS_CORRECT    0x481
#define PAR_NUMRXPDU_CRC_CORRECT    0x483
#define PAR_NUMRXPDU_HCS_WRONG      0x484
#define PAR_NUMRXPDU_CRC_WRONG      0x485
#define PAR_NUMRXPDU_NOCRC          0x486
#define PAR_NUMRXPDU_LENGTH_WRONG   0x487
#define PAR_NUMRXPDU_EMPTY_DATA     0x488
#define PAR_NUMRXPDU_BCID           0x489
#define PAR_NUMRXPDU_PCID           0x48A
#define PAR_NUMRXPDU_SCID           0x48B
#define PAR_NUMRXPDU_TRAFFICCID     0x48C
#define PAR_NUMRXPDU_OTHERCID       0x48D

#define PAR_PHY_INSTANCE_ID         0x500
#define PAR_PHY_SFCOUNT             0x501
#define PAR_PHY_TIME_RESOLUTION     0x502
#define PAR_PHY_DUPLEX_MASK         0x503
#define PAR_PHY_TICK_MODE           0x504
#define PAR_PHY_SFPROCESSED         0x505
#define PAR_PHY_TIME_EXECUTED       0x506

#define PAR_PHY_SFN                0x507      
#define PAR_PHY_CH_TYPE            0x508 
#define PAR_GET_RX_SDU             0x509

/* PRACH */
#define PAR_PRACH_ENA              0x50A
#define PAR_PRACH_ROOT_SEQ_IDX     0x50B
#define PAR_PRACH_CFG_IDX          0x50C
#define PAR_PRACH_HIGH_SPEED_FL    0x50D
#define PAR_PRACH_ZERO_CORR_ZONE_CFG     0x50E
#define PAR_PRACH_FREQ_OFFSET      0x50F

#define PAR_PHY_CH_IDX    0x518

#define PAR_M_ERR_AVG_CH_0    0x519
#define PAR_M_ERR_AVG_CH_1    0x51A
#define PAR_M_ERR_EXPO    0x51B

#define PAR_TIMING_ADV    0x51C

/* SRS */
#define PAR_SRS_BW_CFG      0x520
#define PAR_SRS_SF_CFG      0x521

#define PAR_SRSINFO_NUM     0x525
#define PAR_SRSINFO_BW      0x526
#define PAR_SRSINFO_HOPBW       0x527
#define PAR_SRSINFO_FREQPOS     0x528
#define PAR_SRSINFO_CFGIDX      0x529
#define PAR_SRSINFO_CYCLSHFT    0x52A
#define PAR_SRSINFO_DUR     0x52B
#define PAR_SRSINFO_TXCOMB     0x52C

#define PAR_LAST_RXSDU         0x555

#define PAR_SVSR_APIMSGSTATS_START         0xABA0

typedef struct ParamHeader
{
    U16 paramID;
    U16 length;             // Block length without header and without padding
    U32 data[1];            // Parameter data (placeholder) - always present
} PARAMHEADER, *PPARAMHEADER;

typedef struct MsgHeader
{
    U32 control;            // Control bit field
    U32 length;             // Message payload length, no padding
    U16 type;               // Type of message (command, query etc...)
    U16 srcID;              // Instance ID of message source
    U16 dstID;              // Instance ID of message sink
    U16 msgID;              // Message ID
#if defined(_MSC_VER) || defined(__CC_ARM)
    U32 param[];            // Message parameters (placeholder)
#else /* _MSC_VER */
    U32 param[0];           // Message parameters (placeholder)
#endif /* _MSC_VER */
} MSGHEADER, *PMSGHEADER;

#endif /* APIDEFS_H_ */
