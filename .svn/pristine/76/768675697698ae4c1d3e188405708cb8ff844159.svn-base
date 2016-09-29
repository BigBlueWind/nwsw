//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL larmmsgid.h
 *
 * @brief The declaration of lower ARM messages IDs
 * @author Mindspeed Technologies
 * @version $Revision: 1.40 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _LARMMSGID_H_
#define _LARMMSGID_H_


#define MESSAGE_ID(larm, module, id)   ((larm) + ((module) << 8) + (id))

#define FRM_MSG_OFFS       0x0000      // framework related messages
#define MAC_MSG_OFFS       0x4000      // MAC related messages
#define PHY_MSG_OFFS       0x5000      // PHY related messages (may not be that many, but they still need a range)
#define VOIP_MSG_OFFS      0x6000      // VoIP related messages (may as well get in early - not used for Comcerto compatible product)
#define DATA_MSG_OFFS      0x8000      // Data passing messages
#define SYSINFO_MSG_OFFS   0xA000      // System info releated messages
#define SDIAG_MSG_OFFS     0xF000      // Diagnostic related messages (being for software diags)
#define HDIAG_MSG_OFFS     0xF800      // Diagnostic related messages (being for hardware diags)


/******************************************************************************
*                The list of lower ARM modules                                *
*******************************************************************************/
#define SVSR_MODULE         0
#define DIAG_MODULE         1
#define APP_DIAG_MODULE     2
#define SVSR_LOGGER         3
#define IQPHONE_MODULE      4
#define PLAYER_MODULE       5
#define INFORMER_MODULE     6

/******************************************************************************
*                The list of the supervisor API commands                      *
*******************************************************************************/
// 0xF800
#define SVSR_READY          MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_MODULE, 0)
// 0xF801
#define SVSR_QUERY_INST_ID  MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_MODULE, 1)
// 0xF802
#define SVSR_CHECK_API_WAY  MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_MODULE, 2)
// 0xF803
#define SVSR_CHECK_SYS  	MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_MODULE, 3)
// 0xF804
#define SVSR_CHECK_API_LAT 	MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_MODULE, 4)
// 0xF805
#define SVSR_STORE_SYS_OBJ	MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_MODULE, 5)
// 0xF806
#define SVSR_GET_VERSION	MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_MODULE, 6)
#ifndef APIDEFS_H_
// 0xF807
#define SVSR_READ_MEMORY	MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_MODULE, 7)
// 0xF808
#define SVSR_WRITE_MEMORY	MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_MODULE, 8)
#endif
// 0xF809
#define SVSR_API_DUMP_EN    MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_MODULE, 9)
// 0xF80A
#define SVSR_API_CRC        MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_MODULE, 10)
// 0xF80B
#define SVSR_GET_HEAP_INFO  MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_MODULE, 11)
// 0xF80C
#define SVSR_TEST_ICPU_LIST MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_MODULE, 12)
// 0xF80D
#define SVSR_TEST_GEN_API   MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_MODULE, 13)
/******************************************************************************
*                The list of diagnostics API commands                         *
*******************************************************************************/
// 0xF900
#define DIAG_CONTEXT        MESSAGE_ID(HDIAG_MSG_OFFS, DIAG_MODULE, 0)
// 0xF901
#define DIAG_CONTROL        MESSAGE_ID(HDIAG_MSG_OFFS, DIAG_MODULE, 1)
// 0xF902
#define DIAG_INPUT          MESSAGE_ID(HDIAG_MSG_OFFS, DIAG_MODULE, 2)
#ifndef APIDEFS_H_
// 0xF903
#define DIAG_OUTPUT         MESSAGE_ID(HDIAG_MSG_OFFS, DIAG_MODULE, 3)
#endif
// 0xF904
#define DIAG_RUN            MESSAGE_ID(HDIAG_MSG_OFFS, DIAG_MODULE, 4)
// 0xF905
#define DIAG_RUNASYNC       MESSAGE_ID(HDIAG_MSG_OFFS, DIAG_MODULE, 5)
// 0xF906
#define DIAG_DEBUG          MESSAGE_ID(HDIAG_MSG_OFFS, DIAG_MODULE, 6)
// 0xF907
#define DIAG_MDMA_RUN       MESSAGE_ID(HDIAG_MSG_OFFS, DIAG_MODULE, 7)
// 0xF908
#define DIAG_CEVA_DUMP      MESSAGE_ID(HDIAG_MSG_OFFS, DIAG_MODULE, 8)
// 0xF909
#define DIAG_Q_CEVA_DUMP    MESSAGE_ID(HDIAG_MSG_OFFS, DIAG_MODULE, 9)
// 0xF90A
#define DIAG_READ_DATA      MESSAGE_ID(HDIAG_MSG_OFFS, DIAG_MODULE, 0xA)
// 0xF90B
#define DIAG_WRITE_DATA     MESSAGE_ID(HDIAG_MSG_OFFS, DIAG_MODULE, 0xB)
// 0xF90C
#define GEN_LOG_CFG         MESSAGE_ID(HDIAG_MSG_OFFS, DIAG_MODULE, 0xC)
// 0xF90D
#define GEN_LOG_GET_INFO    MESSAGE_ID(HDIAG_MSG_OFFS, DIAG_MODULE, 0xD)
// 0xF90E
#define GEN_LOG_RESET       MESSAGE_ID(HDIAG_MSG_OFFS, DIAG_MODULE, 0xE)
// 0xF90F
#define GEN_LOG_FLUSH       MESSAGE_ID(HDIAG_MSG_OFFS, DIAG_MODULE, 0xF)
/******************************************************************************
*                The list of the logger API commands                          *
*******************************************************************************/
// 0xFB00
#define SVSR_LOGGER_CFG        MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_LOGGER, 0)
// 0xFB01
#define SVSR_LOGGER_GET_INFO   MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_LOGGER, 1)
// 0xFB02
#define SVSR_LOGGER_RESET      MESSAGE_ID(HDIAG_MSG_OFFS, SVSR_LOGGER, 2)

// 0xFC00
#define IQPHONE_INIT           MESSAGE_ID(HDIAG_MSG_OFFS, IQPHONE_MODULE, 0)
// 0xFC01
#define IQPHONE_GET_INFO       MESSAGE_ID(HDIAG_MSG_OFFS, IQPHONE_MODULE, 1)
// 0xFC02
#define IQPHONE_START          MESSAGE_ID(HDIAG_MSG_OFFS, IQPHONE_MODULE, 2)
// 0xFC03
#define IQPHONE_STOP           MESSAGE_ID(HDIAG_MSG_OFFS, IQPHONE_MODULE, 3)

// 0xFD00
#define APIPLAYER_LOAD           MESSAGE_ID(HDIAG_MSG_OFFS, PLAYER_MODULE, 0)
// 0xFD01
#define APIPLAYER_GET_INFO       MESSAGE_ID(HDIAG_MSG_OFFS, PLAYER_MODULE, 1)
// 0xFD02
#define APIPLAYER_START          MESSAGE_ID(HDIAG_MSG_OFFS, PLAYER_MODULE, 2)
// 0xFD03
#define APIPLAYER_STOP           MESSAGE_ID(HDIAG_MSG_OFFS, PLAYER_MODULE, 3)
// 0xFD04
#define APIPLAYER_INIT           MESSAGE_ID(HDIAG_MSG_OFFS, PLAYER_MODULE, 4)

/****************************************************************************/
                      /*Trner Software Diagnostics module*/
/****************************************************************************/

//0xF000
#define DIAG_CONFIGURE           MESSAGE_ID(SDIAG_MSG_OFFS, SVSR_MODULE, 0)       
// 0xF001
#define DIAG_STREAM_ENA        MESSAGE_ID(SDIAG_MSG_OFFS, SVSR_MODULE, 1)
// 0xF002
#define DIAG_STREAM_DIS        MESSAGE_ID(SDIAG_MSG_OFFS, SVSR_MODULE, 2)
// 0xF003
#define DIAG_BUFF_ACCESS        MESSAGE_ID(SDIAG_MSG_OFFS, SVSR_MODULE, 3)
// 0xF004
#define DIAG_IND        MESSAGE_ID(SDIAG_MSG_OFFS, SVSR_MODULE, 4)
// 0xF005
#define DIAG_REDIRECT        MESSAGE_ID(SDIAG_MSG_OFFS, SVSR_MODULE, 5)
// 0xF006
#define DIAG_ALARM_IND        MESSAGE_ID(SDIAG_MSG_OFFS, SVSR_MODULE, 6)
// 0xF007
#define DIAG_FORCE_ALARM        MESSAGE_ID(SDIAG_MSG_OFFS, SVSR_MODULE, 7)
// 0xF008
#define DIAG_SYS_SHTDWN        MESSAGE_ID(SDIAG_MSG_OFFS, SVSR_MODULE, 8)
// 0xF009
#define DIAG_LOOPBACK_ENA        MESSAGE_ID(SDIAG_MSG_OFFS, SVSR_MODULE, 9)
// 0xF00A
#define DIAG_LOOPBACK_DIS        MESSAGE_ID(SDIAG_MSG_OFFS, SVSR_MODULE, 10)
// 0xF00B
#define  DIAG_INIT                     MESSAGE_ID(SDIAG_MSG_OFFS, SVSR_MODULE, 0xB)
// 0xF00C
#define  DIAG_CLOSE                     MESSAGE_ID(SDIAG_MSG_OFFS, SVSR_MODULE, 0xC)
// 0xF00D
#define  DIAG_API_TEST_INIT                 MESSAGE_ID(SDIAG_MSG_OFFS, SVSR_MODULE, 0xD)
// 0xF00E
#define  DIAG_API_TEST_CLOSE                MESSAGE_ID(SDIAG_MSG_OFFS, SVSR_MODULE, 0xE)


// 0xF100
#define DIAG_SCH_GET_OBJ_INFO       MESSAGE_ID(SDIAG_MSG_OFFS, DIAG_MODULE, 0)
// 0xF101
#define DIAG_SCH_READ               MESSAGE_ID(SDIAG_MSG_OFFS, DIAG_MODULE, 1)
// 0xF102
#define DIAG_SCH_WRITE              MESSAGE_ID(SDIAG_MSG_OFFS, DIAG_MODULE, 2)
// 0xF103
#define DIAG_SCH_CREATE_OBJ         MESSAGE_ID(SDIAG_MSG_OFFS, DIAG_MODULE, 3)
// 0xF104
#define DIAG_SCH_DESTROY_OBJ        MESSAGE_ID(SDIAG_MSG_OFFS, DIAG_MODULE, 4)


// 0xF201
#define APP_DIAG_PROFILE_ENA        MESSAGE_ID(SDIAG_MSG_OFFS, APP_DIAG_MODULE, 1)
// 0xF202
#define APP_DIAG_PROFILE_DIS        MESSAGE_ID(SDIAG_MSG_OFFS, APP_DIAG_MODULE, 2)
// 0xF203
#define APP_DIAG_PROFILE_STAT       MESSAGE_ID(SDIAG_MSG_OFFS, APP_DIAG_MODULE, 3)

//0x0600
#define INFORMER_GET_OBJ_INFO       MESSAGE_ID(FRM_MSG_OFFS, INFORMER_MODULE, 0)

/******************************************************************************
*                The list of PHY API commands                                 *
*******************************************************************************/
#define WIMAX_MODULE        0
#define LTE_MODULE          1

/******************************************************************************/
                             /* LTE */
/******************************************************************************/

#ifndef APIDEFS_H_
// 0x5101
#define PHY_LTE_TEST             MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE,   1)
// 0x5102
#define  PHY_LTE_START_SECTOR    MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 2)
// 0x5103
#define  PHY_LTE_STOP_SECTOR     MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 3)
// 0x5104
#define  PHY_LTE_REINIT          MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 4)
// 0x5105
#define  PHY_LTE_MSG             MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 5)
// 0x5106
#define  PHY_LTE_PHY_CONTEXT     MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 6)

// 0x5108
#define  PHY_LTE_MSG_LIST        MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 8)

// SET and GET RX and TX IQ samples
// 0x510B
#define  PHY_LTE_RX_IQ           MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 11)
// 0x510C
#define  PHY_LTE_TX_IQ           MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 12)

// Task list
// 0x510D
#define  PHY_LTE_TASK_LIST       MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 13)

// User specific debug messages
// 0x510E
#define  PHY_LTE_DEBUG_MSG       MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 14)
// 0x5110
#define  PHY_LTE_RX_BUF_IQ       MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 16)
// 0x5111
#define  PHY_LTE_TX_BUF_IQ       MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 17)
// 0x5112
#define  PHY_LTE_PHYTICK_MSG     MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 18)
// 0x5113
#define  PHY_LTE_CPTIBILITY_MSG  MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 19)
// 0x5114
#define PHY_LTE_SEND_IQ_ENA      MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 20)
// 0x5115
#define PHY_LTE_SEND_IQ_DIS      MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 21)
// 0x5116
#define PHY_LTE_RCV_IQ_ENA       MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 22)
// 0x5117
#define PHY_LTE_RCV_IQ_DIS       MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 23)
// 0x5118
#define  PHY_SETRXBUF            MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 24)
// 0x5119
#define  PHY_GETTXBUF            MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 25)
// 0x511A
#define  PHY_LTE_TIMER_STOP      MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 26)
// 0x511B
#define  PHY_LTE_FD_MSG          MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 27)
// 0x511C
#define  AUTO_RUN_STAT           MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 28)

// 0x511D
#define  PHY_GETRXSDUBUF         MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 29)

// 0x511E
#define  PHY_CPRI_GPIO_CTRL      MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 30)

// 0x511F
#define  PHY_CPRI_CONFIG          MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 31)
// 0x5120
#define  PHY_CPRI_SFP_CTRL        MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 32)

// 0x5121
#define  PHY_EVMFPGA_GPIO_CTRL    MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 33)
// 0x5122
#define  PHY_LTE_TIMER_PAUSED     MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 34)
// 0x5123
#define  PHY_LTE_TIMER_RESUME     MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 35)
// 0x5150, for NMM
#define  PHY_LTE_NMM_MSG          MESSAGE_ID(PHY_MSG_OFFS, LTE_MODULE, 50)

#endif

#endif /*_LARMMSGID_H_*/

#ifdef __cplusplus
}
#endif

