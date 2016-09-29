/**
 * @file    sih.h SIH module.
 * @brief   Contains the SIH moudle related functions and structures.
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * Author     : Prashant Gupta
 * Date       : 31-08-2010
 * Description: 1. Created basic version.
 *
 */

#ifndef SIH_H
#define SIH_H

/* include */

/* Macros */
#define SIH_COMMANDS                        1
#define SIH_MOD_MAX_TIMERS                  3
#define SIH_INSTANCE_ID                     OAMIF_SIH  

#define TABLE_MAX_ID             MAX_PRE_CFG_TABLEID


/* IDs Table, Row, Column which are Invalid or NA */
#define CONFIG_TABLE_ID_NONE        0
#define CONFIG_ROW_ID_NONE          0
#define CONFIG_COL_ID_NONE          0

#define OK                          0
#define ERROR                      -1
#define FOREVER                     1

#define OID_ROW_POSITION	    12

/* RH Status Takes Longer Time */
#define MAX_RH_MSG_PROCESSING_DURATION 10000
#define Q_RH_POLL_INTERVAL 10000

/* externs */
extern SINT32 g_sihReqSent;
extern SINT32 g_sihRespRecd;
extern TBOOL g_sihGetRequestProcessed;
extern TBOOL g_sihSetRequestProcessed;

/* types */
/* SIH Context */

/* Structures */
typedef enum e_SihState
{
  SIH_STATE_INITIALIZING =1,
  SIH_STATE_IDLE,
  SIH_STATE_BUSY,
  SIH_STATE_MAX,
} SihState;

/* Timers  */
typedef ModTimersInfo SihTimersInfo;

typedef struct s_SihContext
{
  VersionInfo         versionInfo;
  FilePath            configDir;
  SihState            sihState;
  UINT8               totalTimers;
  SihTimersInfo       sihTimersInfo;
  UINT16	      curRecvTransId;
}SihContext;


typedef void (*sihMsgHandlerFp)(
   IN Msg           *msg,
   IO SihContext    *sihContext
   );

typedef struct s_SihStateMachine
 {
   sihMsgHandlerFp  fp;
   SihState         nextState;
 } SihStateMachine;

typedef TBOOL (*sendConfigGetResponse) (IO TlvArray  *tlvArray);

typedef struct s_SihGetResponse
 {
   sendConfigGetResponse  fp;
 } SihGetResponse;


 /* Check the state of SIH */
#define sihCheckState(context, state) \
                                  (context->sihState EQ state) ? TRUE : FALSE

 /* Macro to set the state of SIH */
#define sihSetState( context, state) \
                                  (context)->sihState = state

 /* Set the state machine */
 extern SihStateMachine      sihStateMachine[SIH_STATE_MAX][MSG_ID_MAX];
#define InitSihStateMachine( presentState, msgIn, procFunc, newState) \
			sihStateMachine[ presentState ][msgIn].fp = procFunc; \
			sihStateMachine[ presentState ][msgIn].nextState = newState;

 /* initialize get response functions */
extern SihGetResponse      sihGetResponse[TABLE_MAX_ID];
#define InitSihGetRespFuncs( tableId, getFunc) \
			do { sihGetResponse[ tableId ].fp = getFunc; } while (0)

#define printTLV(tlvArray) \
    do { int i;                \
     for (i=0; i < (tlvArray)->numTlvs; i++)        \
      syslog(LOG_INFO, "TAG: %d <=> TABLE ID=%d, ROW ID=%d, COL ID=%d, LENGTH=%d, DATA=%d",  \
             i+1, (tlvArray)->tlvInfo[i].tag.tableId, (tlvArray)->tlvInfo[i].tag.rowId, (tlvArray)->tlvInfo[i].tag.colId, \
             (tlvArray)->tlvInfo[i].len, *((tlvArray)->tlvInfo[i].data));  \
       } while(0)

#define printTraceStatusTLV(tlvArray) \
    do { int i;                \
     for (i=0; i < (tlvArray)->numTlvs; i++)        \
      logError("remove this log. TAG: %d <=> TABLE ID=%d, ROW ID=%d, COL ID=%d, LENGTH=%d, DATA=%d",  \
             i+1, (tlvArray)->tlvInfo[i].tag.tableId, (tlvArray)->tlvInfo[i].tag.rowId, (tlvArray)->tlvInfo[i].tag.colId, \
             (tlvArray)->tlvInfo[i].len, *((tlvArray)->tlvInfo[i].data));  \
       } while(0)

/* Prototypes of common functions */
SihContext  *getSihContext(void);

void initSihStateMachine(void);

void refreshAgentConfig(void);

/* Prototypes of all functions used in SIH */
void sihMain (Msg* msg);

/* externs */
extern TBOOL sendEnbAdminSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendFileDownloadSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendFileUploadSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendSwActivationSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendSwInstallSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendTraceControlSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendCellAdminSnmpGetResponse(IO TlvArray *tlvArray);

extern TBOOL sendEnbRegConfigSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendEnbConfigSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendCellConfigSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendSysInfo1ConfigSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendSysInfo2ConfigSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendSchedulerConfigSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendPhyConfigSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendTraceConfigSnmpGetResponse (IO TlvArray  *tlvArray);
extern TBOOL sendPmCounterLogConfigSnmpGetResponse (IO TlvArray  *tlvArray);
extern TBOOL sendLogConfigSnmpGetResponse (IO TlvArray  *tlvArray);
extern TBOOL sendQosConfigSnmpGetResponse (IO TlvArray  *tlvArray);
extern TBOOL sendEutraHoConfigSnmpGetResponse (IO TlvArray  *tlvArray);
extern TBOOL sendEutraNeighCellConfigSnmpGetResponse (IO TlvArray  *tlvArray);
extern TBOOL sendSysInfo3ConfigSnmpGetResponse (IO TlvArray  *tlvArray);
extern TBOOL sendRadioHeadConfigSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendMmeConfigSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendSonConfigSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendSrsConfigDedicatedSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendSecurityConfigSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendUeEventHistoryLogConfigSnmpGetResponse(IO TlvArray *tlvArray);

extern TBOOL sendFileDownloadStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendFileUploadStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendSwActivationStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendSwPackageStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendComponentStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendEnbStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendMacGenCountersSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendRlcGenCountersSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendPdcpGenCountersSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendGtpGenCountersSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendSctpGenCountersSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendRadioHeadStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendTraceStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendFileEventHistorySnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendSwInstallHistorySnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendActiveUeStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendActiveRabStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendRabHistorySnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendUeEventHistorySnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendModuleStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendEthernetCountersSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendActiveAlarmStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendPmLogFileStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendModuleLogFileStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendLocationStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendRrcGenCountersSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendRrmCountersSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendRrcCellCountersSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendS1apGenCountersSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendS1apPeerNodeCountersSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendAlarmHistorySnmpGetResponse(IO TlvArray *tlvArray);
/* extern TBOOL sendLoginStatusSnmpGetResponse(IO TlvArray *tlvArray); */
extern TBOOL sendMacUplaneCountersSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendPdcpUplaneCountersSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendIpCountersSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendLedStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendGpsStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendCellStatusSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendTraceHistorySnmpGetResponse(IO TlvArray *tlvArray);

extern TBOOL sendTimersConfigSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendMsgTraceSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendEnbAddressesSnmpGetResponse(IO TlvArray *tlvArray);
extern TBOOL sendEventThresholdsSnmpGetResponse(IO TlvArray *tlvArray);
void fillTableInfo( 
     IO TagInfo *tagInfo,
     IN TableId  tableId,
     IN RowId    rowId,
     IN ColId    colId
     );
#endif /* SIH_H */


