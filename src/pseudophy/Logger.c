/*******************************************************************************
* Copyright: (C) 2009 Polaris Networks. All Rights Reserved.                   *
*    No part of this source code may be reproduced, transmitted, transcribed   *
*    or translated into any language without the written permission of         *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this  *
*    permission write to the attention of Legal counsel, Polaris Networks,     *
*    75 Robbins Road, MA 02421 United States.                                  *
********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#ifdef WIN32
#include "pthread.h"
#else
#include <pthread.h>
#include <errno.h>
#endif

#include "Logger.h"
#include "PW_PALProtocolStackShared.h"

#ifndef WIN32
#ifndef vsnprintf
int vsnprintf(char *str, size_t size, const char *format, va_list ap);
#endif
#endif

/* Module Name needs to be set for Logging */
#define __sModuleName__ "SLIB"


#ifdef WIN32
#pragma warning( disable : 4996 )
#endif


/******************************************************************************/
/* Macro definitions.                                                         */
/******************************************************************************/
#define PN_MAXLEN              128
#define PN_MAX_LOG_MESSAGE_LEN 256


/******************************************************************************/
/* Structure declaration(s) (used in this file only).                         */
/******************************************************************************/
typedef struct
{
    char* m_pcFileName;
    FILE* m_ofFP;

} SLogFileID;


/******************************************************************************/
/* Private function prototypes (used in this file only).                      */
/******************************************************************************/
static void _LOGGEROpenLogFile( const char* kpcLogFile );
static void _LOGGERSetLogLevel( ELogLevel uiLogLevel );
static void _LOGGERSetLogTimeFormat( ELogTimeFormat uiLogTimeFormat );
static void _LOGGERCloseLogFile( void );


/******************************************************************************/
/* Global variables.                                                          */
/******************************************************************************/
static const char*     s_kpcLogString[ ELOG_LEVEL_MAX ] = {"ERROR", "CRIT", 
"WARN", "MESG", "INFO", "DEBUG", "DEVEL"};
/* 0 = not initialized, 1 = initialized. */
static int             s_iInitialized = 0;
static ELogLevel       s_eLogLevel    = ELOG_LEVEL_INFO; /* Set default log level */
static ELogTimeFormat  s_eTimeFormat  = ELOG_TIME_FORMAT_DATE_TIME;
static pthread_mutex_t s_sLoggerMutex = PTHREAD_MUTEX_INITIALIZER;

#ifndef LOG_ALL /* if LOG_ALL is defined */
static char*           s_pcLogFile    = NULL;
static FILE*           s_ofLogFP      = NULL;
#else
SLogFileID SLogFileId[ELOG_LEVEL_MAX] = 
{
    {"Error.log",       NULL},
    {"Critical.log",    NULL},
    {"Warning.log",     NULL},
    {"Message.log",     NULL},
    {"Info.log",        NULL},
    {"Debug.log",       NULL}
};


static void GetFileBase(char* pcFileName, char* pcBase, char* pcExt)
{
    char* pos = strrchr(pcFileName, '.');
    if (pos != NULL)
    {
        strncpy(pcBase, pcFileName, pos-pcFileName);
        pcBase[pos-pcFileName + 1]='\0';
        strcpy(pcExt, pos);
    }
    else
    {
        strcpy(pcBase, pcFileName);
    }   
}

static void AppendLogLevelString(
                                 char*               pcLogFile,
                                 const char*         kpcFileBase, 
                                 const char*         kpcLogLevelString, 
                                 const char*         kpcFileExt
                                 )
{

    sprintf(pcLogFile, "%s%s%s", kpcFileBase, kpcLogLevelString, kpcFileExt);

}


#endif /* LOG_ALL */


/******************************************************************************
* Log Message corresponding Log Message Id.                                  *
******************************************************************************/
const   char *arr_kpcLogMessages[]  =
{
    "\n", /* ELOG_NO_MESSAGE           */
    "%s", /* ELOG_MESSAGE       */

    /* Following messages are for API Entry and Exit*/
    "%s() - IN",                                                 /* ELOG_API_IN*/
    "%s() - OUT",                                                /* ELOG_API_OUT*/
    "Polaris Networks %s Emulator Version: %d.%d.%d-%d",         /* ELOG_EMULATOR_VERSION*/
    "This emulator is based on SVN revision: %d",                /* ELOG_EMULATOR_SVN_REVISION*/

    /* Table related */
    "%s table is not created yet",
    "Opened %s table successfully",
    "Failed to open %s table",
    "Closed %s table successfully",
    "Entry already exists in %s table",
    "Entry does not exist in %s table",
    "Failed to add entry in %s table",

    /* The following constants are for File related log messages */
    "Logger Module is initialized. Log File: %s, Log Level: %s",      /* ELOGGER_INIT_SUCCESS*/
    "Error occurred while opening file %s",                    /* ELOG_FOPEN_FAILED*/
    "Error occurred while writing to the file %s",             /* ELOG_FWRITE_FAILED*/
    "Error occurred while reading file content %s",            /* ELOG_FREAD_FAILED*/
    "System Log Level is set to: %s",                          /* ELOGGER_PRINT_LOG_LEVEL*/

    /* The following constants are for Socket related log message IDs */
    "Could not create socket for MME %d",                       /* ESOCKET_CREATION_FAILED*/
    "Failed to bind socket for MME(Index: %d). Socket Id: %d, Address: %s:%d", /* ESOCKET_BIND_FAILED            */

    /* The following constants are for Memory Allocation related log message IDs */
    "Failed to allocate(Malloc) memory",                        /* ELOG_MALLOC_FAILED*/
    "Failed to allocate(Calloc) memory",                        /* ELOG_CALLOC_FAILED*/
    "Failed to reallocate(realloc) memory",                     /* ELOG_REALLOC_FAILED*/
    "Failed to Free pointer(NULL)",                             /* ELOG_FREE_FAILED*/
    "Could not Free illegal Data",                              /* ELOG_RELEASE_RESOURCE_FAILED*/
    "Illegal log level(%d) supplied",                           /* ELOG_ILLEGAL_LOG_LEVEL*/
    "Illegal log time format(%d) supplied",                     /* ELOG_ILLEGAL_TIME_FORMAT*/
    "%s, File: %s, Line: %d",                                   /* ELOG_MEM_DEBUG_MESSAGE */

    /* The following constants are for Message Queue related log message IDs */
    "Failed to create message queue %s. Error \"%s\"", /* EMQ_CREATE_FAILED*/
    "Failed to open message queue %s. Error: %s",                      /* EMQ_OPEN_FAILED*/
    "Successfully created message queue: %s",                   /* EMQ_CREATE_SUCCESSFUL*/
    "Opened message queue %s. FD: %d", /* EMQ_OPEN_SUCCESSFUL */
    "Failed to delete message queue %s",                    /* EMQ_DELETE_FAILED*/
    "Failed to close message queue",             /* EMQ_CLOSE_FAILED*/
    "Failed to send message to message queue. Error: %s",    /* EMQ_SEND_FAILED*/
    "Failed to receive message from MsgQ (FD: %d). Error: %s (Error Code: %d)",/* EMQ_RECEIVE_FAILED*/
    "Successfully created event queue: %s",                     /* EEQ_CREATE_SUCCESSFUL*/
    "Wrote %d bytes in FD: %d", /* EMQ_SEND_SUCCESS */
    "Received %d bytes from FD: %d", /* EMQ_RECV_SUCCESS */
    "MsgQ ready with Rx data. FD: %d", /* EMQ_FD_RX_READY */

    /* Timer */
    "Added Timer: %p. TimerInfo: %p, Head: %p, Tail: %p, #Nodes: %d", /* ETM_ADDED_TIMER */
    "Removed Timer: %p. TimerInfo: %p, Head: %p, Tail: %p, #Nodes: %d", /* ETM_REMOVED_TIMER */
    "Registered Timer: %p. Interval: %d ms, MaxRptn: %d", /* ETM_REGISTERED_TIMER */
    "Deregistered Timer: %p", /* ETM_DEREGISTERED_TIMER */
    "Started Timer: %p (RunNode: %p)", /* ETM_STARTED_TIMER */
    "Stopped Timer: %p", /* ETM_STOPPED_TIMER */
    "Invoked Handler for Timer: %p", /* ETM_INVOKED_TIMER_CB */
    "Invoked Cleanup callback for Timer: %p", /* ETM_INVOKED_CLEANUP_CB */

    /* The following constants are for S1AP related log message IDs */
    "Initialized S1AP Protocol Stack successfully.", /* ES1AP_INIT_SUCCESS*/
    "Failed to initialize S1AP Protocol Stack.", /* ES1AP_INIT_FAILURE*/
    "Packet received", /* ES1AP_RX_PACKET*/
    "%s %s", /* ES1AP_INFO_MESSAGE*/
    "%s %d", /* ES1AP_DEBUG_MSG_WITH_INT_VALUE*/
    "MME(%s) sent %s message to eNodeB(%s)", /* ES1AP_MME_TX_ENB_MESSAGE_INFO*/
    "MME(%s) failed to send %s message to eNodeB(%s)", /* ES1AP_MME_TX_ENB_MESSAGE_FAIL*/
    "eNodeB(%s) sent %s message to MME(%s)", /* ES1AP_ENB_TX_MME_MESSAGE_INFO*/
    "eNodeB(%s) failed to send %s message to MME(%s)", /* ES1AP_ENB_TX_MME_MESSAGE_FAIL */
    "MME(%s) sent %s message to eNodeB(%s): MME UE-ID: %d, ENB UE-ID: %d", /* ES1AP_MME_TX_ENB_UE_ASSOC_MESSAGE_INFO */
    "MME(%s) failed to send %s message to eNodeB(%s): MME UE-ID: %d, ENB UE-ID: %d", /* ES1AP_MME_TX_ENB_UE_ASSOC_MESSAGE_FAIL */
    "eNodeB(%s) sent %s message to MME(%s): ENB UE-ID: %d, MME UE-ID: %d", /* ES1AP_ENB_TX_MME_UE_ASSOC_MESSAGE_INFO */
    "eNodeB(%s) failed to send %s message to MME(%s): ENB UE-ID: %d, MME UE-ID: %d", /* ES1AP_ENB_TX_MME_UE_ASSOC_MESSAGE_FAIL */
    "MME(%s) received %s message from eNodeB(%s)", /* ES1AP_MME_RX_ENB_MESSAGE_INFO*/
    "eNodeB(%s) received %s message from MME(%s)", /* ES1AP_ENB_RX_MME_MESSAGE_INFO*/
    "MME(%s) received %s message from eNodeB(%s): MME UE-ID: %d, ENB UE-ID: %d",/* ES1AP_MME_RX_ENB_UE_ASSOC_MESSAGE_INFO*/
    "eNodeB(%s) received %s message from MME(%s): ENB UE-ID: %d, MME UE-ID: %d",/* ES1AP_ENB_RX_MME_UE_ASSOC_MESSAGE_INFO*/
    "S1AP DEBUG Message : %s", /* ES1AP_DEBUG_MESSAGE*/
    "SCTP Bind failed: Port = %d", /* ES1AP_BIND_TO_SOCKET_FAILED*/
    "eNodeB(Id: %d, name: %s, IP: %s) received a message from RRC", /* ES1AP_HANDLE_RRC_MESSAGE*/
    "MME(Id: %d, name: %s, IP: %s) received a message from MsgQ", /* ES1AP_HANDLE_NAS_MESSAGE*/
    "Received S1AP Message: Procedure Code = %d, Message Type = %d", /* ES1AP_PACKET_INFO*/
    "Sender eNodeB(%s)", /* ES1AP_ENODEB_INFO*/
    "Sender MME(%s)", /* ES1AP_MME_INFO*/
    "Established S1 Connection between eNodeB(%s) and MME(%s)", /* ES1AP_S1_CONNECTION_ESTABLISHED*/
    "Failed to setup S1 Connection between eNodeB(%s) and MME(%s)", /* ES1AP_S1_CONNECTION_FAILURE*/
    "Got unexpected S1AP message at %s (Procedure Code: %d, Message Type: %d)", /* ES1AP_UNEXPECTED_PACKET*/
    "S1AP message cannot come before SCTP Connection is established. Connection Index : %d", /* ES1AP_CONNECTION_FAILURE*/
    "Duplicate UE arrived. UE-ID: %d", /* ES1AP_DUPLICATE_UE*/
    "S1AP Node does not exist in S1AP Node Table NodeId: %d , NodeIndex: %d", /* ES1AP_NODE_NOT_FOUND*/
    "Failed to open Database for Tracking Area", /* ES1AP_TA_DB_OPENING_FAILED*/
    "Failed to open Database for Cell Info", /* ES1AP_CELL_DB_OPENING_FAILED*/
    "Failed to open Database for UE-Id-to-UEContext Mapping", /* ES1AP_UEID_TO_UE_CONTEXT_MAPPING_TABLE_OPENING_FAILED*/
    "Failed to retrieve IP Address of Ethernet Intf: %s", /* ES1AP_INTERFACE_IP_ADDRESS_ERROR*/
    "NULL MME Pointer", /* ES1AP_GARBLED_MME_NODE*/
    "NULL eNodeB Pointer", /* ES1AP_GARBLED_ENB_NODE*/
    "NULL MME and/or eNodeB Pointer(s)", /* ES1AP_GARBLED_NODE*/
    "Socket Id of the MME does not match with the socket Id decoded from packet", /* ES1AP_ILLEGAL_SOCKET_ID*/
    "MME received a packet but its Connection state is not established", /* ES1AP_ILLEGAL_MME_CONNECTION_STATE*/
    "UE Context creation in MME failed", /* ES1AP_UE_CONTEXT_ADDITION_IN_MME_FAILED*/
    "UE Context creation in eNodeB failed", /*ES1AP_UE_CONTEXT_ADDITION_IN_ENB_FAILED*/
    "UE Context in MME is not found for UE (MME UE-ID: %d)", /* ES1AP_UE_CONTEXT_IN_MME_UNAVAILABLE*/
    "UE Context in eNodeB is not found for UE (UE Identity: %p)", /* ES1AP_UE_CONTEXT_IN_ENB_UNAVAILABLE */
    "S1AP API Called from other layer: %s", /* ES1AP_API_NAME*/
    "An Event come from eNodeB(Connection Index: %d)", /* ES1AP_MME_EVENT_INFO*/
    "Number of Served PLMNs : %d", /* ES1AP_SERVED_PLMN_INFO*/
    "Failed to parse %s Message", /* ES1AP_PARSING_FAILURE_MESSAGE*/
    "Failed to build %s Message", /* ES1AP_BUILDING_FAILURE_MESSAGE*/
    "eNodeB UE Id in the received message does not match with MME UE Context", /* ES1AP_ENB_ID_MISMATCH */
    "MME UE Id in the received message does not match with eNodeB UE Context", /* ES1AP_MME_ID_MISMATCH */
    "S1AP Handler for %s returned failure", /* ES1AP_CALLBACK_FAILED */
    "S1AP Handler for %s is not implemented yet", /* ES1AP_CALLBACK_INVALID */
    "Triggered %s Handler", /* ES1AP_TRIGGERED_HANDLER */
    "Triggered %s Handler for UE (UE-ID: %d)", /* ES1AP_TRIGGERED_UE_ASSOC_HANDLER */
    "Triggered NAS PDU Recv Handler. UE-ID: %d. NAS PDU Len: %d", /* ES1AP_TRIGGERED_NAS_PDU_RECV_HANDLER */
    "Logical S1 Connection Does Not Exist - Unexpected S1AP Message received at MME", /* ES1AP_MME_NONEXISTENT_LOGICAL_S1_CONNECTION */
    "MME S1AP UE Context removed. UE-ID: %d", /* ES1AP_MME_UE_CONTEXT_REMOVED */
    "eNodeB S1AP UE Context removed. UE-ID: %d", /* ES1AP_ENB_UE_CONTEXT_REMOVED */
    "eNodeB is unable to send S1AP message, logical S1 Connection is not established", /* ES1AP_ENB_NONEXISTENT_LOGICAL_S1_CONNECTION_TX */
    "Logical S1 Connection does not exist - Unexpected S1AP Message received at eNodeB", /*ES1AP_ENB_NONEXISTENT_LOGICAL_S1_CONNECTION_RX*/
    "Failed to process %s", /*ES1AP_MSG_PROCESSING_FAILED*/
    "Inserted UE in ENodeBEntity database. MME UE-ID: %d, ENB UE-ID: %d", /* ES1AP_UE_INSERTED_IN_ENODEB_ENTITY_DB */ 
    "Deleted UE from ENodeBEntity database. MME UE-ID: %d, ENB UE-ID: %d", /* ES1AP_UE_DELETED_FROM_ENODEB_ENTITY_DB */ 
    "No matching MME found, selected PLMN(MCC:%d, MNC:%d)", /* ES1AP_MME_SELECTION_FAILURE */
    "Applied S1AP policy \"Drop message\" on procedure %u", /* ES1AP_POLICY_MSG_DROP */
    "Applied S1AP policy \"Send Error Indication with Cause=%u:%u\" on procedure %u.", /* ES1AP_POLICY_MSG_SEND_ERR_IND */
    "eNodeB Id inside E-UTRAN CGI Id IE does not match with eNodeB Id previously received in S1 Setup Request. Expected: %d, Received: %d", /* ES1AP_GLOBAL_ENB_ID_MISMATCH */
    
    /* The following constants are for SCTP API related log message IDs   */
    /* The following constants are for Input Parameters related log message IDs*/
    "Initialized SCTP Protocol Stack successfully.",            /* ESCTP_INIT_SUCCESS*/
    "Failed initialize SCTP Protocol Stack.",            /* ESCTP_INIT_FAILURE*/
    "SCTP SCTPTxEvent",                                                                                    /* ESCTP_INFO_MESSAGE               */
    "Opening new SCTP Connection. Socket Id: %d, Connection Id: %d, Remote Node Ptr: %x", /* ESCTP_CONNECTION_INFO            */
    "Got New SCTP Connection notification. Node Ptr: %x, Socket Id: %d, Connection Id: %d",    /* ESCTP_NEW_CONNECTION_INFO        */
    "Received SCTP Data. Socket Id: %d, Connection Id: %d, Stream Id: %d, Local Node Ptr: %x, Remote Node Ptr: %x",   /* ESCTP_DATA_RECEIVE_INFO          */
    "Got SCTP Notification. Socket Id:%d, Connection Id: %d. Type: %d",               /* ESCTP_NOTIFICATION_INFO          */
    "SCTP SCTPTxEvent",                                                                                    /* ESCTP_DEBUG_MESSAGE              */
    "Could not create SCTP Socket - Socket Id = %d",                                            /* ESCTP_SOCKET_CREATION_FAILED     */
    "SCTP bind failed. Reason = %s, Socket Id = %d, Destination: %s:%d",             /* ESCTP_BIND_FAILED                 */
    "SCTP open failed",                                                                                    /* ESCTP_OPEN_FAILED                 */
    "SCTP Connect failed - SocketId = %d, Connection Id = %d, ptr = %x",                               /* ESCTP_CONNECT_FAILED             */
    "Failed to send SCTP data (Socket: %d, Connection: %d, Stream: %d). Error: %s (errno: %d)", /* ESCTP_SEND_FAILED */
    "Going to send SCTP Data Packet of %d bytes to Peer",                                                  /* ESCTP_TX_INFO                    */
    "SCTP Connection Information - Socket Id: %d, Connection Id: %d, Stream Id: %d",       /* ESCTP_TX_DEBUG                  */

    /*The following constants are for TCL Interpretor Library */
    "TCL Interpreter Result :%s",                                                                          /* ETCL_INTERPRETER_STATUS          */
    "%s called with %d arguments",                                                                         /* ETCL_CMD_MESSAGE                 */
    "Setting up new Network (Network Name: %s, MCC: %s, MNC: %s)",                                       /* ETCL_CREATE_NETWORK_MESSAGE  */
    "Deleting Network (Name: %s)",                                                                      /* ETCL_DELETE_NETWORK_MESSAGE  */
    "Setting up new Tracking Area (Network Name: %s, TAC: %d)",                                          /* ETCL_LTE_CREATE_TA_MESSAGE       */
    "Deleting Tracking Area (Network Name: %s, TAC: %d)",                                       /* ETCL_LTE_DELETE_TA_MESSAGE  */
    "Defining Cell (Serving eNodeB Id: %d, TAC: %d, Cell Id: %d)",                                       /* ETCL_ENODEB_CREATE_CELL_MESSAGE    */
    "Deleting Cell (Serving eNodeB Id: %d, TAC: %d, Cell Id: %d)",                                       /* ETCL_ENODEB_DELETE_CELL_MESSAGE  */
    "Creating eNodeB (Id: %d, Name: %s, %d Served networks: {%s}, IP: %s)",                     /* ETCL_LTE_CREATE_ENB_MESSAGE      */
    "Creating MME (Name: %s, %d Served networks: {%s}, %d MME Group ID: {%d}, %d Served MME Code: {%d}, Relative Capacity: %d, IP: %s, SCTP Port: %d)", /* ETCL_LTE_CREATE_MME_MESSAGE */
    "S1 Signaling connection is being set up between eNodeB(Id: %d, IP: %s) and MME(%s)",                  /* ETCL_LTE_ASOCIATE_ENB_TO_MME_MESSAGE  */
    "Log File Name: %s",                                                                                   /* ETCL_LTE_SET_LOG_FILE */
    "Setting current Log Level to %s",                                                                     /* ETCL_LTE_SET_LOG_LEVEL*/
    "",                                                                                                    /* ETCL_DEBUG_MESSAGE */ 

    /*The following constants are for NAS Protocol Stack : */
    "Failed to initialize NAS Protocol Stack.",                                           /* ENAS_INIT_MODULE_FAILED*/
    "Failed to start NAS Init Thread.",                                                   /* ENAS_INIT_THREAD_FAILED*/
    "Successfully Started NAS Init Thread.",                                              /* ENAS_INIT_THREAD_SUCCESS*/
    "Successfully Closed NAS Library.",                                                   /* ENAS_CLOSE_LIBRARY_SUCCESS*/
    "MME Context Parameters Population failed.",                                          /* ENAS_POPULATE_MME_PARAMS_FAILED*/
    "Successfully Created MME NAS.",                                                      /* ENAS_CREATE_MME_SUCCESS*/
    "UE Context Parameters Population failed.",                                           /* ENAS_POPULATE_UE_PARAMS_FAILED*/
    "Successfully Created UE NAS.",                                                       /* ENAS_CREATE_UE_SUCCESS*/
    "Delete MME failed.",                                                                 /* ENAS_DELETE_MME_FAILED*/
    "Successfully Deleted MME.",                                                          /* ENAS_DELETE_MME_SUCCESS*/
    "Delete UE failed.",                                                                  /* ENAS_DELETE_UE_FAILED*/
    "Successfully Deleted UE.",                                                           /* ENAS_DELETE_UE_SUCCESS*/
    "Module ( %s ) Initialization Failed.",                                               /* ENAS_MODULE_INIT_FAILED      */
    "Successfully Started Event Processing Engine.",                                      /* ENAS_EVENT_PROC_STARTED*/
    "Successfully Stopped Event Processing Engine.",                                      /* ENAS_EVENT_PROC_STOPPED*/
    "Initialize Event Queue Failed.",                                                    /* ENAS_INIT_EVT_PROC_QUEUE_FAILED*/
    "Close Event Queue Failed.",                                                          /* ENAS_CLOSE_EVT_PROC_QUEUE_FAILED*/
    "Initialized NAS Protocol Stack successfully.",             /* ENAS_INIT_SUCESS				*/
    "Received message from S1AP. Added in Rx MsgQ. UE-ID: %d, Msg Len: %d", /* ENAS_RCVD_MESSAGE */
    "Received message from MsgQ. UE-ID: %d, Msg Len: %d", /* EMMENAS_RCVD_FROM_MSGQ */
    "Received Attach Complete. UE-ID: %d", /* EMMENAS_RCVD_ATTACH_COMPLETE */
    "Integrity check failed. UE-ID: %d, Msg len: %d, Auth Algo: %d. MAC mismatch (Rx: %u, Computed: %u)", /* EMMENAS_INTEGRITY_FAILED */
    "Integrity check failed. Msg len: %d, Auth Algo: %d. MAC mismatch (Rx: %u, Computed: %u)", /* EUENAS_INTEGRITY_FAILED */
    "%s UE-ID: %d", /* ENAS_GENERIC_EVENT */
    "%s UE-ID: %d, Msg Len: %d", /* ENAS_EVENT_RCVD_MSG */
    "Retransmitting PDU. UE-ID: %d, Msg len: %d", /* EMMENAS_RETRANSMIT_PDU */
    "Processing EMM Event. UE-ID: %d, CurrState: %d, Evt: %d, Handler: %p, NextState: %d", /* EMMENAS_PROCESS_EMM_EVT */
    "Changing UE State form Timer Event. UE-ID: %d, CurrState: %d, Timer: %s, NextState: %d", /* EMMENAS_TIMER_UE_STATE_CHANGE */
    "Applied NAS policy \"Drop message\" on message type %u", /* ENAS_POLICY_MSG_DROP */
    "Applied NAS policy \"Respond with EMM/ESM Status with Cause=%u\" on message type %u.", /* ENAS_POLICY_MSG_RSP_WITH_STATUS */

    /* GTP common messages*/
    "Library already initialized.",      /* EGTP_LIB_ALREADY_INIT*/
    "Library initialized successfully.", /* EGTP_LIB_INIT_SUCESS*/
    "Library initialization failed.",    /* EGTP_LIB_INIT_FAIL*/
    "Received %d bytes ( %s <-- %s ), Msg Type = %d, Sequence No. = %d", /* EGTP_RCVD_DATA */
    "Sent %d bytes ( %s --> %s )", /* EGTP_SENT_DATA */
    "Failed to send data (%d bytes) to (%s:%d). Error: %s", /* EGTP_DATA_SEND_FAILED */
    "Applied GTPv2C policy \"Drop message\" on message type %u", /* EGTPV2C_POLICY_MSG_DROP */
    "Applied GTPv2C policy \"Controlled Response with Cause=%u\" on message type %u.", /* EGTPV2C_POLICY_MSG_CTRL_RSP */

    /* GTP-U related messages*/
    "Received DL user data from kernel. Dst IP: %s, Data Length: %d bytes", /* EGTPU_DL_DATA_INFO */
    "%s DL user data over S1-U from SGW (%s) to UE (IMSI: %s), Bearer-Id: %d, Local TEID: %d, Remote TEID: %d.", /* EGTPU_DL_DATA_SENT */

    /* GTP-C related messages*/

    /* SGW Emulator */
    "Invalid GTP-C Session Handle: %p. Session already released.", /*ESGW_SGW_INVALID_SESSION_HANDLE*/
    "Invalid SGW Handle: %p. SGW already destroyed.",/* ESGW_SGW_INVALID_SGW_HANDLE */
    "GTPv1U tunnel created with EPS Bearer ID %u", /* ESGW_GTPV1U_TUNNEL_CREATE_SUCCESS */
    "GTPv2C session created with %u EPS Bearers", /* ESGW_GTPV2C_SESSION_CREATE_SUCCESS */
    "Modified EPS Bearer ID %u, Remote IP %s, Remote TEID %d", /* ESGW_GTPV1U_TUNNEL_MODIFY_SUCCESS */

    "Invoked Create Bearer Request to MME-C. #Default Bearer ID: %d, #Bearers: %d", /* ESGW_GTPV2C_INVOKED_GTPC_CREATE_BEARER_REQ */
    "Invoked Update Bearer Request to MME-C. #Bearer Num: %d", /* ESGW_GTPV2C_INVOKED_GTPC_UPDATE_BEARER_REQ */
    "Invoked Delete Bearer Request to MME-C. #Default Bearer ID: %d #Bearer Num: %d", /* ESGW_GTPV2C_INVOKED_GTPC_DELETE_BEARER_REQ */

    "Received Create Bearer Response from MME-C. #Created %d dedicated bearers.", /* ESGW_GTPV2C_RCVD_CREATE_BEARER_RESPONSE */
    "Successfully Created dedicated bearer. #Bearers ID: %d", /* ESGW_GTPV2C_BEARER_CREATED */
    "Received Update Bearer Response from MME-C. #Updated Bearers: %d", /* ESGW_GTPV2C_RCVD_UPDATE_BEARER_RESPONSE */
    "Successfully Updated dedicated bearer. #Bearers ID: %d", /* ESGW_GTPV2C_BEARER_UPDATED */
    "Received Delete Bearer Response from MME-C. #Default bearer ID: %d, #Deleted %d dedicated bearers.", /* ESGW_GTPV2C_RCVD_DELETE_BEARER_RESPONSE */
    "Successfully Deleted dedicated bearer. #Bearers ID: %d", /* ESGW_GTPV2C_BEARER_DELETED */

    "SGW Creation. Own IP address: %s", /* ESGW_SGW_CREATE */
    "S11 Session Creation. SGW-IP: %s, IMSI: %s, Default EBI: %d", /* ESGW_SGW_S11_SESSION_CREATE */
    "S11 Session Deletion. SGW-IP: %s, IMSI: %s", /* ESGW_SGW_S11_SESSION_DELETE */
    "%s S1-U Bearer Creation. SGW-IP: %s, IMSI: %s, EBI: %d, QoS: (%s), Result: (%s)", /* ESGW_SGW_S1U_BEARER_CREATE */
    "Request %s S1u Bearer Creation. SGW-IP: %s, IMSI: %s, EBI: Unknown, QoS: (%s), Result: (%s)", /* ESGW_SGW_S1U_BEARER_CREATE_REQUEST */
    "%s S1-U Bearer Deletion. SGW-IP: %s, IMSI: %s, EBI: %d, Result: (%s)", /* ESGW_SGW_S1U_BEARER_DELETE */
    "%s S1-U Bearer Modification. SGW-IP: %s, IMSI: %s, EBI: %d, Modified QoS: (%s), Result: (%s)", /* ESGW_SGW_S1U_BEARER_MODIFY */
    "%s S1-U Bearer Updated. SGW-IP: %s, IMSI: %s, EBI: %d, Updated QoS: (%s), Result: (%s)", /* ESGW_SGW_S1U_BEARER_UPDATE */
    "S5-C Session Creation: SGW-IP: %s, IMSI: %s, Default EBI %d", /* ESGW_SGW_S5C_SESSION_CREATE */
    "S5-C Session Deletion: SGW-IP: %s, IMSI: %s", /* ESGW_SGW_S5C_SESSION_DELETE */
    "%s S5-U Bearer Creation. SGW-IP: %s, IMSI: %s, EBI: %d, QoS: (%s), Result: (%s)", /* ESGW_SGW_S5U_BEARER_CREATE */
    "Request %s S5-U Bearer Creation. SGW-IP: %s, IMSI: %s, EBI: Unknown, QoS: (%s), Result: (%s)", /* ESGW_SGW_S5U_BEARER_CREATE_REQUEST */
    "%s S5-U Bearer Deletion. SGW-IP: %s, IMSI: %s, EBI: %d, Result: (%s)", /* ESGW_SGW_S5U_BEARER_DELETE */
    "%s S5-U Bearer Modification. SGW-IP: %s, IMSI: %s, EBI: %d, Result: (%s)", /* ESGW_SGW_S5U_BEARER_MODIFY */
    "%s S5-U Bearer Updated. SGW-IP: %s, IMSI: %s, EBI: %d, Updated QoS: (%s), Result: (%s)", /* ESGW_SGW_S5U_BEARER_UPDATE */
    "Downlink Data Notification. IMSI: %s, Result (%s)", /* ESGW_SGW_DOWNLINK_DATA_NOTIFY */
    "Path Failure. SGW-IP: %s, Peer IP: %s", /* ESGW_SGW_PATH_FAILURE */

    /* MME Controller */
    "Failed to create network with PLMN: %s, MCC: %lu, MNC: %lu", /* EMMEC_CREATE_NETWORK_FAILED */
    "Failed to delete network with PLMN: %s", /* EMMEC_DELETE_NETWORK_FAILED */
    "Failed to retrieve UE data from HSS. IMSI: %s", /* EMMEC_HSS_UE_RETREIVE_FAILED */
    "Failed to Purge UE data in HSS. IMSI: %s", /* EMMEC_HSS_UE_PURGE_FAILED */
    "Failed to retrieve UE data from Context Table. IMSI: %s", /* EMMEC_UE_IMSI_CONTEXT_RETREIVE_FAILED */
    "Failed to retrieve UE data from Context Table. ID: %d", /* EMMEC_UE_ID_CONTEXT_RETRIEVE_FAILED */
    "Received Duplicate UE Event from NAS. Current UE-ID: %d, Existing UE-ID: %d", /* EMMEC_RCVD_NAS_DUPLICATE_UE */
    "Received %s message with invalid(%s) EBI(%d)", /* EMMEC_RECVD_INVALID_EBI */
    "Received Create PDN Connection Event from NAS. UE-ID: %d", /* EMMEC_RECEIVED_NAS_CREATE_PDN_CONN_EVENT */
    "Invoked GTPv2-C to send Create Session Request. UE-ID: %d, EBI: %d, Local TEID: %u", /* EMMEC_INVOKED_GTPC_CREATE_SESSION_REQ */
    "Received Create Session Response from GTPv2-C. UE-ID: %d, #Bearers: %d, EBI: %d", /* EMMEC_RCVD_GTPC_CREATE_SESSION_RSP */
    "Triggered Create PDN Connection Response to NAS. UE-ID: %d, #Context(s): %d, EBI: %d", /* EMMEC_TRIGGERED_NAS_CREATE_SESSION_RSP */
    "Received Initial UE Arrival Event from S1AP. MME UE-ID: %d, ENB UE-ID: %d", /* EMMEC_RCVD_S1AP_INITIAL_UE */
    "Added Initial UE in UE-ID Context Table. MME UE-ID: %d, ENB UE-ID: %d", /* EMMEC_RCVD_INIT_UE_IN_CONTEXT_TABLE */
    "Added UE in UE-IMSI Context Table. IMSI: %s, MME UE-ID: %d, SGW IP: %s", /* EMMEC_ADDED_UE_IN_CONTEXT_TABLE */
    "Duplicate UE Context (MME UE-ID: %d, IMSI: %s)", /* EMMEC_DUPLICATE_UE_CONTEXT_ID */
    "Duplicate UE Context (IMSI: %s, MME UE-ID: %d)", /* EMMEC_DUPLICATE_UE_CONTEXT_IMSI */
    "Existing UE Context (IMSI: %s, MME UE-ID: %d)", /* EMMEC_EXISTING_UE_CONTEXT_IMSI */
    "Failed to retrieve bearer context. MME UE-ID: %d, EBI: %d", /* EMMEC_EBI_RETRIEVE_FAILED */
    "Received NAS MM State Change Event. UE-ID: %d, State: %s", /* EMMEC_UE_MM_STATE_CHANGED */
    "Ignored NAS MM State Change Event. UE-ID: %d, State: %s", /* EMMEC_UE_MM_STATE_UNCHANGED */
    "Registered S-TMSI: %s in S-TMSI table. UE-ID: %d", /* EMMEC_REGISTERED_STMSI */
    "Registered GUTI: %s, Length: %d for UE-ID: %d in GUTI table", /* EMMEC_REGISTERED_GUTI */
    "Deregistered GUTI: %s, Length: %d for UE-ID: %d from GUTI table", /* EMMEC_DEREGISTERED_GUTI */
    "Failed to retrieve UE ID from GUTI. GUTI: %d", /* EMMEC_GUTI_RETRIEVE_FAILED */
    "Mismatch in NAS #Bearers and MMEC to-be-setup #Bearers: UE-ID: %d, #NAS-Bearers: %d, #MMEC-Bearers: %d", /* EMMEC_UE_CONTEXT_NAS_BEARER_MISMATCH */
    "#Bearers exceed in eNodeB UE Context Create Handler. UE-ID: %d, #Bearers: %d", /* EMMEC_UE_CONTEXT_NAS_BEARER_EXCEEDS_MAX */
    "Received NAS UE Context Create Event. UE-ID: %d", /* EMMEC_RCVD_NAS_UE_CONTEXT_CREATE */
    "Received NAS UE Initiated Service Request Event. UE-ID: %d", /* EMMEC_RCVD_NAS_UE_INITIATED_SERVICE_REQ */
    "Triggered S1AP eNodeB UE Context Create. UE-ID: %d, #Bearers: %d", /* EMMEC_TRIGGERED_S1AP_ENB_UE_CONTEXT_CREATE */
    "Triggered NAS-UE-Signaling-Complete Event to NAS. UE-ID: %d", /* EMMEC_TRIGGERED_NAS_UE_SIG_CREATE_COMPLETE */
    "Invoked GTPv2-C to send Modify Bearer Request. UE-ID: %d, #Modified: %d, #Removed: %d", /* EMMEC_INVOKED_GTPC_MODIFY_BEARER_REQ */
    "Received Modify Bearer Response from GTPv2-C. UE-ID: %d, #Modified: %d, #Removed: %d", /* EMMEC_RCVD_GTPC_MODIFY_BEARER_RSP */    
    "Invalid Create Bearer Request from GTPv2-C.UE-ID: %d, #Bearers: %d", /* EMMEC_GTPC_INVALID_CREATE_BEARER_REQ */
    "Function call PNMMENASActivateDedicatedEPSBearerContext failed.",/*EMMEC_INVOKED_CREATE_BEARER_SETUP_TO_NAS_FAILED*/
    "Triggered Create Bearer Setup Response to SGW. UE-ID: %d, #Bearers: %d", /* EMMEC_INVOKED_GTPC_CREATE_BEARER_REQ_RESPONSE */
    "Triggered Update Bearer Response to SGW. UE-ID: %d, #Bearers: %d", /* EMMEC_INVOKED_GTPC_UPDATE_BEARER_REQ_RESPONSE */
    "Triggered Delete Bearer Response to SGW. UE-ID: %d, #Bearers: %d", /* EMMEC_INVOKED_GTPC_DELETE_BEARER_REQ_RESPONSE */

    "Received Create Bearer Response from S1AP. UE-ID: %d, #SucceededERAB: %d", /* EMMEC_RCVD_S1AP_CREATE_BEARER_RESPONSE */
    "Received Update Bearer Response from S1AP. UE-ID: %d, #SucceededERAB: %d", /* EMMEC_RCVD_S1AP_UPDTAE_BEARER_RESPONSE */
    "Received Release Bearer Response from S1AP. UE-ID: %d, #SucceededERAB: %d", /* EMMEC_RCVD_S1AP_RELEASE_BEARER_RESPONSE */

    "Received Create Bearer Request from GTPv2-C.UE-ID: %d, #Bearers: %d", /* EMMEC_RCVD_GTPC_CREATE_BEARER_REQ */
    "Received Update Bearer Request from GTPv2-C.UE-ID: %d, #Bearers: %d", /* EMMEC_RCVD_GTPC_UPDATE_BEARER_REQ */
    "Received Delete Bearer Request from GTPv2-C.UE-ID: %d, #Bearers: %d", /* EMMEC_RCVD_GTPC_DELETE_BEARER_REQ */

    "Invoked Create Bearer Request to MME-C. Default Bearer ID: %d, #Bearers: %d", /* EMMEC_INVOKED_GTPC_CREATE_BEARER_REQ */
    "Invoked Update Bearer Request to MME-C.", /* EMMEC_INVOKED_GTPC_UPDATE_BEARER_REQ */
    "Invoked Delete Bearer Request to MME-C. #Default Bearer ID: %d", /* EMMEC_INVOKED_GTPC_DELETE_BEARER_REQ */

    "Received DL Data Notification from GTPv2-C. UE-ID: %d", /* EMMEC_RCVD_GTPC_DL_DATA_NOTIFICATION */
    "Invoked GTPv2-C to send Indirect Data Forwarding Tunnel Request. UE-ID: %d", /* EMMEC_INVOKED_GTPC_INDIRECT_DATA_FWD_TUNNEL_REQ */
    "Received Indirect Data Forwarding Tunnel Response from GTPv2-C. UE-ID: %d", /* EMMEC_RCVD_GTPC_INDIRECT_DATA_FWD_TUNNEL_RESP */
    "Received Delete Session Request Event from NAS. UE-ID: %d, EBI: %d", /* EMMEC_RECEIVED_NAS_DELETE_SESSION_REQ_EVENT */
    "Invoked GTPv2-C to send Delete Session Request. UE-ID: %d, EBI: %d", /* EMMEC_INVOKED_GTPC_DELETE_SESSION_REQ */
    "Received Delete Session Response from GTPv2-C. UE-ID: %d", /* EMMEC_RCVD_GTPC_DELETE_SESSION_RSP */
    "Triggered NAS Delete Session Response Handler. UE-ID: %d, EBI: %d", /* EMMEC_TRIGGERED_NAS_DELETE_SESSION_RSP */
    "Failed to process GTPv2-C Delete Session Response. UE-ID: %d", /* EMMEC_FAILED_TO_PROC_DELETE_SESSION_RSP */
    "Received Create eNodeB UE Context Response Event from S1AP. UE-ID: %d", /* EMMEC_RCVD_S1AP_CREATE_ENB_UE_CONTEXT_RSP */
    "Received Release eNodeB UE Context Release Event from S1AP. UE-ID: %d", /* EMMEC_RCVD_S1AP_RELEASE_ENB_UE_CONTEXT_RSP */
    "Received Release eNodeB UE Context Response Event from S1AP. UE-ID: %d", /* EMMEC_RCVD_S1AP_RELEASE_ENB_UE_CONTEXT_RSP */
    "Received Release UE Context Event from NAS. UE-ID: %d", /* EMMEC_RCVD_NAS_RELEASE_UE_CONTEXT */
    "Triggered S1AP Release eNodeB UE Context. UE-ID: %d", /* EMMEC_TRIGGERED_S1AP_RELEASE_UE_CONTEXT */
    "Triggered NAS Delete UE Context. UE-ID: %d", /* EMMEC_TRIGGERED_NAS_DELETE_UE_CONTEXT */
    "Triggered S1AP Delete UE Context. UE-ID: %d", /* EMMEC_TRIGGERED_S1AP_DELETE_UE_CONTEXT */
    "Triggered NAS Start Paging. UE-ID: %d", /* EMMEC_TRIGGERED_NAS_START_PAGING */
    "Received NAS Paging Ack. UE-ID: %d", /* EMMEC_RCVD_NAS_PAGING_ACK */
    "Default Bearer is not set for UE-ID: %d. %s", /* EMMEC_DEFAULT_BEARER_NOT_SET */
    "Invoked Create Bearer Request to NAS. UE-ID: %d, #Bearers: %d", /* EMMEC_INVOKED_NAS_CREATE_BEARER_REQ */
    "Invoked GTPv2-C to send Release Access Bearer Request. UE-ID: %d", /* EMMEC_INVOKED_GTPC_RELEASE_ACCESS_BEARERS_REQ */
    "Received Release Access Bearer Response from GTPv2-C. UE-ID: %d", /* EMMEC_RCVD_GTPC_RELEASE_ACCESS_BEARERS_RSP */
    "Failed to process GTPv2-C Release Access Bearer Response. UE-ID: %d", /* EMMEC_FAILED_TO_PROC_RELEASE_ACCESS_BEARERS_RSP */
    "Modified DL Transport Info for UE-ID: %d, EBI: %d to IP: %s, TE-ID: %d", /* EMMEC_MODIFIED_DL_TNL_EBI */
    "Added Bearer in \"Bearers-Already-Setup\" table. UE-ID: %d, EBI: %d", /* EMMEC_ADDED_DONE_EBI */
    "Removed Bearer from \"Bearers-Already-Setup\" table. UE-ID: %d, EBI: %d", /* EMMEC_REMOVED_DONE_EBI */
    "Added Bearer in \"Bearers-To-Be-Setup\" table. UE-ID: %d, EBI: %d", /* EMMEC_ADDED_TO_BE_DONE_EBI */
    "Removed Bearer from \"Bearers-To-Be-Setup\" table. UE-ID: %d, EBI: %d", /* EMMEC_REMOVED_TO_BE_DONE_EBI */
    "UE ECM State changed from %s to %s. UE-ID: %d", /* EMMEC_CHANGED_UE_ECM_STATE */

    "Freed UE Context. UE-ID: %d", /* EMMEC_FREED_UE_CONTEXT */ 
    "Received UE Context Release Event from NAS. UE-ID: %d", /* EMMEC_RECEIVED_NAS_UE_CONTEXT_RELEASE_EVENT */ 
    "Received S1AP Reset Event. #UE: %d", /* EMMEC_RCVD_S1AP_RESET */
    "Received Current Procedure (%s, %s) from NAS. UE-ID: %d", /* EMMEC_RCVD_NAS_CURRENT_PROC */
    "Triggered S1AP Set UE Home PLMN. UE-ID: %d, Home PLMN: %s", /* EMMEC_TRIGGERED_S1AP_SET_UE_HOME_PLMN */
    "Received \"Registered TAC List\" Event from S1AP. UE-ID: %d, #TACs: %d", /* EMMEC_RCVD_S1AP_REGISTERED_TA_LIST */
    "Received \"Registered TAC List\" Event from S1AP for all UE(s). #TACs: %d", /* EMMEC_RCVD_S1AP_REGISTERED_TA_LIST_FOR_ALL_UE */
    "Received Handover Nofity Event from S1AP. UE-ID: %d", /* EMMEC_RCVD_S1AP_HO_NOTIFY */
    "Received Handover Preparation Event from S1AP. UE-ID: %d", /* EMMEC_RCVD_S1AP_HO_PREP */
    "Triggered S1AP Handover Request. UE-ID: %d. Target eNodeB: %s", /* EMMEC_TRIGGERED_S1AP_HO_REQ */
    "Received Handover Request Ack Event from S1AP. UE-ID: %d", /* EMMEC_RCVD_S1AP_HO_REQ_ACK */
    "Triggered S1AP Handover Command. UE-ID: %d.", /* EMMEC_TRIGGERED_S1AP_HO_CMD */
    "Received Location Report Control Event from S1AP. UE-ID: %d, ECM State: %s", /* EMMEC_RCVD_S1AP_LOC_REP_CNTRL */
    "Triggered S1AP Location Report. UE-ID: %d. Loc Rep event: %s, area: %s.", /* EMMEC_TRIGGERED_S1AP_LOC_REP */
    "Received Fwd PDU Event from NAS. UE-ID: %d, PDU Len: %d", /* EMMEC_RCVD_NAS_FWD_PDU */
    "Received Fwd NAS PDU Event from S1AP. UE-ID: %d, PDU Len: %d", /* EMMEC_RCVD_S1AP_FWD_NAS_PDU */
    "Session already deleted for this UE. UE-ID: %d", /* EMMEC_SESSION_ALREADY_DELETED */
    "Reset procedure started for this UE. UE-ID: %d , hence it will stop forwarding NAS PDU", /* EMMEC_RESET_ALREADY_STARTED */
    "Handover Failed received for UE-ID: %d, Cause group: %d , Cause value : %d", /* EMMEC_HANDOVER_FAILED */
    "Received Path Failure Notification for GTPv2C. UE-ID: %d. Message type: %d", /* EMMEC_GTPC_PACKET_SENDING_FAILED */
    "Failed to retrieve UE data from EIR. IMSI: %s", /*EMMEC_EIR_UE_RETRIEVE_FAILED*/
    "Validated the IMEI from EIR, IMSI: %s", /*EMMEC_EIR_VALIDATE_IMEI*/
    "Received Bearer Resource Failure Indication from GTPv2-C. UE-ID: %d, EBI: %d, PTI: %d", /* EMMEC_RCVD_GTPC_BEARER_RES_FAIL_IND */

    /* HSS Simulator */
    "HSS Simulating Node with PLMN %s, MCC: %u, MCC: %u", /* EHSS_NODE_CREATING */
    "HSS Registered UE with IMSI: %s", /* EHSS_UE_REGISTERED */

    /* SGSN Emulator */
    "Failed to create network with PLMN: %s, MCC: %lu, MNC: %lu", /* ESGSN_CREATE_NETWORK_FAILED */
    "Failed to delete network with PLMN: %s", /* ESGSN_DELETE_NETWORK_FAILED */
    "SGSN created for IP address %s, handle = %p", /* ESGSN_SGSN_CREATE_SUCCESS */
    "SGSN with handle = %p destroyed", /* ESGSN_SGSN_DESTROY_SUCCESS */
    "Invalid GTP-C Session Handle: %p. Session already released.", /*ESGSN_SGSN_INVALID_SESSION_HANDLE*/
    "Invalid SGSN Handle: %p. SGSN already destroyed.",/* ESGSN_SGSN_INVALID_SGW_HANDLE */
    
    /* HSS Emulator */
    "Initialized S6a Protocol Stack successfully.", /*ES6A_INIT_THREAD_SUCCESS*/
    "Failed to initialize S6a Protocol Stack.", /*ES6A_INIT_MODULE_FAILED*/
    "Successfully closed S6a Library.", /*ES6A_CLOSE_LIBRARY_SUCCESS*/
    "Received Null Packet.", /*ES6A_RECVD_NULL_PKT*/
    "Error in memory allocation.", /*ES6A_ERROR_MEM_ALLOCATION*/
    "Command Code : %d answer packet neither has Result Code nor Experimental Result.", /*ES6A_INVALID_ANSWER_PACKET*/
    "Invalid packet command code : %d received.", /*ES6A_INVALID_PKT_CMD*/
    "Received packet command code : %d command flag : %d.", /*ES6A_RECVD_PKT*/
    "Mandatory AVP missing in Packet (Command Code : %d, Command Flag : %d) Missing AVP Code : %d.", /*ES6A_PKT_MANDATORY_AVP_MISSING*/
    "Received UserName AVP within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_USERNAME_AVP*/
    "Received Visited Plmn Id AVP within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_VPLMNID_AVP*/
    "Received Result Code: %d within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_RESULT_CODE*/
    "Received Auth Session State: %d within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_AUTH_SESSION_STATE*/
    "Received RAND AVP within Packet (cmd code: %d cmd flag: %d).",/*ES6A_RECVD_RAND_CHALLANGE*/
    "Received XRES AVP within Packet (cmd code: %d cmd flag: %d).",/*ES6A_RECVD_XRES*/
    "Received AUTN AVP within Packet (cmd code: %d cmd flag: %d).",/*ES6A_RECVD_AUTN*/
    "Received KASME AVP within Packet (cmd code: %d cmd flag: %d).",/*ES6A_RECVD_KASME*/
    "Received ULR FLAG: %d within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_ULRFLAG*/
    "Received RAT Type: %d within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_RAT_TYPE*/
    "Received ULA FLAG: %d within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_ULAFLAG*/
    "Received MSISDN AVP within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_MSISDN*/
    "Received Subscription Data AVP within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_SUBS_DATA*/
    "Received APN Configuration Profile AVP within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_APN_CONFIG_PROFILE*/
    "Received APN Configuration AVP within Packet (cmd code: %d cmd flag: %d).",/*ES6A_RECVD_APN_CONFIG*/
    "Received AMBR AVP within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_AMBR*/
    "Received MAX UL AMBR AVP within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_MAX_UL_AMBR*/
    "Received Authentication Info AVP within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_AUTH_INFO*/
    "Received EUTRAN Vector AVP within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_EUTRAN_VECTOR*/
    "Received PDN TYPE AVP within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_PDN_TYPE*/
    "Received MAX DL AMBR AVP within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_MAX_DL_AMBR*/
    "Received Service Selection AVP within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_SERVICE_SEL*/
    "Received Served Party IP Address AVP within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_UE_IP*/
    "Received MIP6 Agent Info AVP within Packet (cmd code: %d cmd flag: %d).",/*ES6A_RECVD_MIP6_AGENT_INFO*/
    "Received MIP Home Agent Address AVP within Packet (cmd code: %d cmd flag: %d).",/*ES6A_RECVD_MIP_HOME_AGENT_ADDRESS*/
    "Received Cancellation Type AVP within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_CANCELLATION_TYPE*/
    "Received Requested EUTRAN Authentication Info AVP within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_REQUESTED_EUTRAN_AUTH_INFO*/
    "Initialize DIAMETER Event Queue Failed.", /*EDIAMETER_INIT_EVT_PROC_QUEUE_FAILED*/
    "Unknown Application: %s", /*ES6A_UNKNOWN_APPLICATION*/
    "Unknown Command: %s", /*ES6A_UNKNOWN_COMMAND*/
    "Unknown Avp: %s", /*ES6A_UNKNOWN_AVP*/
    "Unknown enum: %s", /*ES6A_UNKNOWN_ENUM*/
    "AVP registration Conflict: %s (%u)", /*ES6A_AVP_CONFLICT*/
    "Command already exists: %s" , /*ES6A_CMD_EXISTS*/
    "Error reading : %s", /*ES6A_READ_ERROR*/
    "Could not open :%s", /*ES6A_OPEN_ERROR*/
    "Context for UE: %s is successfully updated.", /*ES6A_UE_CONTEXT_UPDATED*/
    "UE: %s is successfully authenticated.", /*ES6A_UE_AUTHENTICATED*/
    "Received message from MsgQ. Msg Len: %d", /* ES6A_RCVD_FROM_MSGQ */
    "Received All-APN-Configurations-Included-Indicator AVP within Packet (cmd code: %d cmd flag: %d).",/* ES6A_RECVD_ALLAPNCONFIG */
    "Received IDA FLAG: %d within Packet (cmd code: %d cmd flag: %d)." , /*ES6A_RECVD_IDAFLAG*/
    "Received IDR FLAG: %d within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_IDRFLAG*/
    "Received DSA FLAG: %d within Packet (cmd code: %d cmd flag: %d)." , /*ES6A_RECVD_DSAFLAG*/
    "Received DSR FLAG: %d within Packet (cmd code: %d cmd flag: %d).", /*ES6A_RECVD_DSRFLAG*/
    "Received Context Id: %d within Packet (cmd code: %d cmd flag: %d)." , /*ES6A_RECVD_CONTEXTID*/
    "Failed to send Authentication Information Request to HSS, IMSI: %s", /*ES6A_FAILED_AUTH_REQUEST*/
    "Successfully sent Authentication Information Request to HSS, IMSI: %s",  /*ES6A_SUCCESS_AUTH_REQUEST*/
    "This UE is already Authenticated, IMSI: %s",  /*ES6A_UE_ALREADY_AUTHENTICATED*/
    "Failed to send Update Location Request to HSS, IMSI: %s", /*ES6A_FAILED_UPDATE_REQUEST*/
    "Successfully sent Update Location Request to HSS, IMSI: %s", /*ES6A_SUCCESS_UPDATE_REQUEST*/
    "Context for this UE is already updated, IMSI: %s", /*ES6A_UE_ALREADY_UPDATED*/
    "Failed to send Notify Request to HSS, IMSI: %s", /*ES6A_FAILED_NOTIFY_HSS*/
    "Successfully sent Notify Request to HSS, IMSI: %s", /*ES6A_SUCCESS_NOTIFY_HSS*/
    "HSS already notified of UE Activity, IMSI: %s", /*ES6A_UE_ALREADY_NOTIFIED*/
    "Successfully sent Mobile Equipment Identity Check Request to EIR, IMSI: %s", /*ES13_SUCCESS_MEIDENTITY_CHECK_EIR*/
    "Failed to send Mobile Equipment Identity Check Request to EIR, IMSI: %s", /*ES13_FAILED_MEIDENTITY_CHECK_EIR*/
    "Successfully sent Purge UE Request to HSS, IMSI: %s", /*ES6A_PURGE_UE_SUCCESS*/
    "Failed to send Purge UE Request Request to HSS, IMSI: %s", /*ES6A_PURGE_UE_FAILED*/
    "Successfully sent Cancel Location Answer to HSS, IMSI: %s", /*ES6A_CANCEL_LOCATION_ANSWER_SUCCESS*/
    "Failed to send Cancel Location Answer to HSS, IMSI: %s", /*ES6A_CANCEL_LOCATION_ANSWER_FAILED*/
    "Successfully sent Insert Subscriber Data Answer to HSS, IMSI: %s", /*ES6A_INSERT_SUBSCRIBER_DATA_ANSWER_SUCCESS*/
    "Failed to send Insert Subscriber Data Answer to HSS, IMSI: %s", /*ES6A_INSERT_SUBSCRIBER_DATA_ANSWER_FAILED*/
    "Successfully sent Delete Subscriber Data Answer to HSS, IMSI: %s", /*ES6A_DELETE_SUBSCRIBER_DATA_ANSWER_SUCCESS*/
    "Failed to send Delete Subscriber Data Answer to HSS, IMSI: %s", /*ES6A_DELETE_SUBSCRIBER_DATA_ANSWER_FAILED*/
    "Received Update Location Answer from HSS for IMSI: %s, Result Code: %d", /*ES6A_UPDATE_LOCATION_ANSWER_RECEIVED*/
    "Received Authentication Information Answer from HSS for IMSI: %s, Result Code: %d", /*ES6A_AUTHENTICATION_INFORMATION_ANSWER_RECEIVED*/
    "Received Cancel Location Request from HSS for IMSI: %s", /*ES6A_CANCEL_LOCATION_REQUEST_RECEIVED*/
    "Retrieving UE-Context from Internal HSS, IMSI: %s", /*ES6A_INTERNAL_HSS_UPDATE_LOCATION_REQUEST*/
    "Retrieving Authentication Vector from Internal HSS, IMSI: %s", /*ES6A_INTERNAL_HSS_AUTHENTICATION_INFORMATION_REQUEST*/
    "Received Purge UE Answer from HSS for IMSI: %s, Result Code: %d" , /*ES6A_PURGE_UE_ANSWER_RECEIVED*/
    "Received Insert Subscriber Data Request from HSS for IMSI: %s", /*ES6A_INSERT_SUBSCRIBER_DATA_REQUEST_RECEIVED*/
    "Received Delete Subscriber Data Request from HSS for IMSI: %s", /*ES6A_DELETE_SUBSCRIBER_DATA_REQUEST_RECEIVED*/
    "Received Notify Answer from HSS for IMSI: %s, Result Code: %d", /*ES6A_NOTIFY_ANSWER_RECEIVED*/
    "Received Mobile Equipment Identity Check Answer from EIR for IMSI: %s, Result Code: %d", /*ES13_MOBILE_EQUIPMENT_IDENTITY_CHECK_ANSWER_RECEIVED*/
    "Null username received, packet: %s", /*EDIAMETER_INVALID_USERNAME_RECEIVED*/
    "Null packet received, packet: %s", /*EDIAMETER_INVALID_PACKET_RECEIVED*/
    "Sending %s Answer for UserName: %s with Result Code: %d", /*EDIAMETER_SENDING_ANSWER*/

    "Network Creation. PLMN: %s, MCC: %lu, MNC: %lu", /*EMMEC_IMP_NETWORK_CREATION*/
    "MME Creation. Own IP address: %s, Primary PLMN: %s, MME Code: %d, MME Group ID: %d, S1AP Port: %d", /*EMMEC_IMP_MME_CREATION*/
    "MME Destroy. MME Id: %s", /*EMMEC_IMP_MME_DESTROY*/
    "S1-MME Connection Setup. MME-Id: %s, eNodeB IP: %s, Global eNodeB Id: %s" , /*EMMEC_IMP_MME_S1_MME_CONNECTION_SETUP*/
    "S1-MME Connection Close. MME-Id: %s, Global eNodeB Id: %d, Initiated By: %s", /*EMMEC_IMP_MME_S1_MME_CONNECTION_CLOSE*/
    "S1-MME Connection Reset. MME-Id: %s, Global eNodeB Id: %d, Reset Type: %s, Initiated By %s", /*EMMEC_IMP_MME_S1_MME_CONNECTION_RESET*/
    "UE Connection State change. MME-Id: %s, UE Id-pair (MME: %d eNodeB: %d), Changed ECM State: (%s)", /*EMMEC_IMP_MME_UE_CONNECTION_STATE_CHANGED*/
    "UE Initial Attach Request. MME-Id: %s, UE Id-pair (MME: %d eNodeB: %d), IMSI/GUTI (IMSI: %s GUTI: %s)", /*EMMEC_IMP_MME_UE_INITIAL_ATTACH_REQUST*/
    "UE Attach Procedure Complete. MME-Id: %s, UE Id-pair (MME: %d eNodeB: %d), IMSI/GUTI (IMSI: %s GUTI: %s), IMEI: %s", /*EMMEC_IMP_MME_UE_ATTACH_COMPLETE*/
    "Tracking Area Update. MME-Id: %s, UE Id-pair (MME: %d eNodeB: %d)", /*EMMEC_IMP_MME_TRACKING_AREA_UPDATE*/
    "UE HandOver Initiation. MME-Id: %s, UE Id-pair (MME: %d eNodeB: %d), Source Global eNodeB Id: %s, Target Global eNodeB Id: %s", /*EMMEC_IMP_MME_UE_HANDOVER_INITIATED*/
    "UE HandOver Completion. MME-Id: %s, UE Id-pair (MME: %d eNodeB: %d), Target SGW IP address: %s", /*EMMEC_IMP_MME_UE_HANDOVER_COMPLETED*/
    "Paging. MME-Id: %s, UE Id-pair (MME: %d eNodeB: N/A)", /*EMMEC_IMP_MME_PAGING*/
    "Dedicated S1u Bearer Modification. MME-Id: %s, UE Id-pair (MME: %d eNodeB: %d), QOS (%s), Result: (%s)", /*EMMEC_IMP_MME_DEDICATED_S1U_BEARER_MODIFICATION*/
    "Dedicated S1u Bearer Deletion. MME-Id: %s, UE Id-pair (MME: %d eNodeB: %d), QOS (%s), Result: (%s)", /*EMMEC_IMP_MME_DEDICATED_S1U_BEARER_DELETION*/
    "Dedicated S1u Bearer Creation. MME-Id: %s, UE Id-pair (MME: %d eNodeB: %d), QOS: (%s), Result: (%s)", /*EMMEC_IMP_MME_DEDICATED_S1U_BEARER_CREATION*/
    "S11 Session Deletion. MME-Id: %s, UE Id-pair (MME: %d eNodeB: %d), Default EBI: %d, Result: (%s)", /*EMMEC_IMP_MME_S11_SESSION_DELETION*/
    "S11 Session Creation. MME-Id: %s, UE Id-pair (MME: %d eNodeB: %d), Default EBI: %s, Result: (%s)", /*EMMEC_IMP_MME_S11_SESSION_CREATION*/
    "UE Detach. MME-Id: %s, UE Id-pair: (MME: %d eNodeB: %d], Detach type: (%s), Initiated By: (%s)", /*EMMEC_IMP_MME_UE_DETACH*/
    "UE Mobility State change: MME-Id: %s,  UE Id-pair: (MME: %d eNodeB: %d),  Changed EMM State: (%s)", /*EMMEC_IMP_MME_UE_MOBILITY_STATE_CHANGE*/
    "UE Authentication status update: MME-Id: %s, UE Id-pair (MME: %d eNodeB: %d), IMSI: %s, result: (%s)", /*EMMEC_IMP_MME_UE_AUTHENTICATION_STATUS_UPDATE*/ 
    "UE Authentication vector update: MME-Id: %s, UE Id-pair (MME: %d eNodeB: %d), IMSI: %s, Auth-Vector (%s), Result: (%s)", /*EMMEC_IMP_MME_UE_AUTHENTICATION_VECTOR_UPDATE*/ 
    "UE Mobile Equipment Identity validation status. MME-Id: %s, UE Id-pair (MME: %d eNodeB: %d), IMSI: %s, IMEI: %s, result: (%s)", /*EMMEC_IMP_MME_UE_MOBILE_EQUIPMENT_IDENTITY_VALIDATION_STATUS*/
    "UE Susbcription validation status update. MME-Id: %s, UE Id-pair (MME: %d eNodeB: %d), IMSI: %s, result: (%s)", /*EMMEC_IMP_MME_UE_SUSBCRIPTION_VALIDATION_STATUS_UPDATE*/
    "Subscriber Profile. Name: %s, PGW IP Address: %s, APN: %s", /*EHSSSiM_SUBSCRIBER_PROFILE*/
    "UE Register. Profile name: %s, MSIN: %s, Shared Key: %s", /*EHSSSiM_UE_REGISTER*/
	
	/* ENB Improved Loging */
	"Network Creation. Network Name (%s)",/*EENBC_IMP_NETWORK_CREATION*/
	"eNodeB Creation. IP address (%s), Primary PLMN (%s), eNodeB ID (%d), Supported PLMNs (%s)", /*EENBC_IMP_ENB_CREATION*/
	"Cell Creation. PLMN (%s), eNodeB ID (%d), Cell ID (%d), TAC (%d), Advertised PLMNs (%s)", /*EENBC_IMP_CELL_CREATION*/
	"eNodeB Destroy. %s", /*EENBC_IMP_ENB_DESTROY*/
	"S1-MME Connection Setup. %s, MME IP (%s), MME Port (%d)", /*EENBC_IMP_ENB_S1_MME_CONNECTION_SETUP*/
	"S1-MME Connection Close. PLMN (%s), eNodeB ID (%d), MME IP (%s), Initiated By (%s)	", /*EENBC_IMP_ENB_S1_MME_CONNECTION_CLOSE*/
	"S1-MME Connection Reset. PLMN (%s), eNodeB ID (%d), MME IP (%s), Reset Type (%s) ,Initiated By (%s)",/*EENBC_IMP_ENB_S1_MME_CONNECTION_RESET*/
	"eNodeB Configuration Update. PLMN (%s), eNodeB ID (%d), Cell ID (%d), Updated Advertised PLMN List (%s)", /*EENBC_IMP_ENB_CONFIGURATION_UPDATE*/
	"UE Connection State change. %s, UE RRC ID(%s), Changed ECM State (%s)", /*EENBC_IMP_ENB_UE_CONNECTION_STATE_CHANGE*/
	"UE Context Creation. PLMN (%s), eNodeB ID (%d), UE S1AP ID-Pair (ENb (%d), MME (%d))", /*EENBC_IMP_UE_CONTEXT_CREATION*/
	"UE Context Deletion. PLMN (%s), eNodeB ID (%d), UE S1AP ID-Pair (ENb (%d), MME (%d))", /*EENBC_IMP_UE_CONTEXT_DELETION*/
	"UE Handover Initiation. Source eNodeB (%s), Target eNodeB ID(%s), UE S1AP ID-Pair (ENb (%d), MME (%d))",  /*EENBC_IMP_UE_HANDOVER_INITIATED*/
    "UE Handover Completion. Target eNodeB (%s), UE S1AP ID-Pair (ENb (%d), MME (%d))",  /*EENBC_IMP_UE_HANDOVER_COMPLETE*/
	"UE Paging. %s, Network identifier for UE (%s)", /*EENBC_IMP_UE_PAGING*/
	"ERAB Creation. PLMN (%s), eNodeB ID (%d), UE S1AP ID-Pair (ENb (%d), MME (%d)), EBI (%d), QoS Param (%s), result (%s)", /*EENBC_IMP_ERAB_CREATION*/
	"ERAB Deletion. PLMN (%s), eNodeB ID (%d), UE S1AP ID-Pair (ENb (%d), MME (%d)), EBI (%d), result (%s)", /*EENBC_IMP_ERAB_DELETION*/
	"ERAB Modification. PLMN (%s), eNodeB ID (%d),  UE S1AP ID-Pair (ENb (%d), MME (%d)), EBI (%d), Modified QoS (%s), result (%s)", /*EENBC_IMP_ERAB_MODIFICATION*/
	"UE Creation. Home PLMN (%s), MSIN (%s), Shared Key (%s)", /*EUESIM_IMP_UE_CREATION*/
	"UE Attach. IMSI (%s), GUTI (%s), selected PLMN (%s), %s, Cell ID (%d), TAC (%d), result (%s)", /*EUESIM_IMP_UE_ATTACH*/
	"UE Connection State change. %s, IMSI (%s), Changed ECM State (%s)", /*EUESIM_IMP_UE_CONNECTION_STATE_CHANGE*/
	"UE Mobility State change. IMSI (%s), Changed EMM state (%s)", /*EUESIM_IMP_UE_MOBILITY_STATE_CHANGE*/
	"UE Detach. IMSI (%s), GUTI (%s), Detach Type (%s), Initiated by (%s)", /*EUESIM_IMP_UE_DETACH*/
	"UE Paging Indication. %s, Network identifier for UE (%s)", /*EUESIM_IMP_UE_PAGING_INDICATION*/
	"UE Service Request. %s, selected PLMN (%s), %s, Cell ID (%d), TAC (%d), result (%s)", /*EUESIM_IMP_UE_SERVICE_REQUEST*/
	"Tracking Area Update. GUTI (%s), Current TAC (%d), TAU Type (%s)", /*EUESIM_IMP_UE_TRACKING_AREA_UPDATE*/
	"Cell Change. Current Cell ID (%d), Current TAC (%d)", /*EUESIM_IMP_UE_CELL_CHANGE*/
	"Dedicated Bearer Creation. IMSI (%s), EBI (%d), QoS (%s), result (%s)", /*EUESIM_IMP_UE_DEDICATED_BEARER_CREATION*/
	"Dedicated Bearer Deletion. IMSI (%s), EBI (%d), result (%s)", /*EUESIM_IMP_UE_DEDICATED_BEARER_DELETION*/
	"Dedicated Bearer Modification. IMSI (%s), EBI (%d), Modified QoS (%s), result (%s)", /*EUESIM_IMP_UE_DEDICATED_BEARER_MODIFICATION*/
        "S5-C Session Creation. IMSI (%s), EBI (%d)", /*EPGW_IMP_S5C_SESSION_CREATION*/
        "S5-C Session Deletion. IMSI (%s)", /*EPGW_IMP_S5C_SESSION_DELETION*/
        "S5-U Default Bearer Creation. IMSI (%s), EBI (%d), result (%s)", /*EPGW_IMP_S5U_DEFAULT_BEARER_CREATION*/
        "S5-U Default Bearer Deletion. IMSI (%s), EBI (%d), result (%s)", /*EPGW_IMP_S5U_DEFAULT_BEARER_DELETION*/
        "S5U Dedicated Bearer Creation. IMSI (%s), EBI (%d), QoS (%s), result (%s)", /*EPGW_IMP_S5U_DEDICATED_BEARER_CREATION*/
        "S5U Dedicated Bearer Updation. IMSI (%s), EBI (%d), Modified QoS (%s), result (%s)", /*EPGW_IMP_S5U_DEDICATED_BEARER_UPDATION*/
        "S5U Dedicated Bearer Deletion. IMSI (%s), EBI (%d), result (%s)", /*EPGW_IMP_S5U_DEDICATED_BEARER_DELETION*/
        "S5U Path Failure. Peer IP (%s)", /*EPGW_IMP_S5U_PATH_FAILURE*/
        "S5C Path Failure. Peer IP (%s)", /*EPGW_IMP_S5C_PATH_FAILURE*/

        "Initializing Module: %s, Service Port: %d", /* EPAL_INITIALIZE */
        "%s Registerd Peer %s@%s:%d", /* EPAL_REGISTERED_PEER */
        "%s Sending %d bytes to peer %s@%s:%d", /* EPAL_SEND_DATA_TO_PEER */
        "%s Failed to send %d bytes of data to peer %s(Unregistered peer)", /* EPAL_FAILED_SEND_DATA_UNKNOWN_PEER */
        "%s %s", /* EPAL_LOG_MESSAGE */

        /* RLC messages */
        "Initializing MAC. Cell Id: %d, #Antennas: %d, UL BW: %d, DL BW: %d, PHY: %d", /* ERLC_INIT_MAC */
        "Received CCCH message. RNTI: 0x%x. Data length: %d", /* ERLC_RX_CCCH */

    /* *** This should be the last line *** */
	"NULL",/* ELOG_MAX_MSGID*/


};

#ifndef PN_LOGGING_DISABLED
/**********************************************************************************
* Initialize the Logger module.                                                  *
**********************************************************************************/
PN_LOGGER_API
void LOGGERInitializeModule
(
 const char*    in_kpszLogFileName,
 ELogLevel      in_eLogLevel,
 ELogTimeFormat in_eLogTimeFormat
 )
{
    int iInitializationStatus = 0;
    char* szFileName          = NULL;

    if( in_kpszLogFileName != NULL )
    {
        pthread_mutex_lock( &s_sLoggerMutex );
        {
            if( s_iInitialized == 0 ) /* Not already initialized */
            {
                if( strlen( in_kpszLogFileName ) > 0 )
                {
                    szFileName = ( char* )in_kpszLogFileName;
                }
                else
                {
                    in_kpszLogFileName = "stdout";
                }

                _LOGGEROpenLogFile( in_kpszLogFileName );
                _LOGGERSetLogLevel( in_eLogLevel );
                _LOGGERSetLogTimeFormat( in_eLogTimeFormat );

                s_iInitialized        = 1;
                iInitializationStatus = 1;
            }
        }
        pthread_mutex_unlock( &s_sLoggerMutex );

        if( iInitializationStatus == 1 )
        {
            LOG_DEVEL(ELOGGER_INIT_SUCCESS, 
                in_kpszLogFileName, 
                s_kpcLogString[in_eLogLevel]);
        }
    }
}


/**********************************************************************************
* Closes the Logger module.                                                      *
**********************************************************************************/
PN_LOGGER_API
void LOGGERCloseModule( void )
{
    pthread_mutex_lock( &s_sLoggerMutex );
    {
        if( s_iInitialized == 1 ) /* Already initialized */
        {
            _LOGGERCloseLogFile();

            s_iInitialized = 0;
        }
    }
    pthread_mutex_unlock( &s_sLoggerMutex );
}


/**********************************************************************************
* Sets log level.                                                                *
**********************************************************************************/
PN_LOGGER_API
void LOGGERSetLogLevel(ELogLevel in_eLogLevel)
{
    pthread_mutex_lock( &s_sLoggerMutex );
    {
        if( s_iInitialized == 1 ) /* Already initialized */
        {
            _LOGGERSetLogLevel(in_eLogLevel);
        }
    }
    pthread_mutex_unlock( &s_sLoggerMutex );
}

/**********************************************************************************
* Gets log level.                                                                *
**********************************************************************************/
PN_LOGGER_API
ELogLevel LOGGERGetLogLevel()
{
    return s_eLogLevel;
}


/**********************************************************************************
* Sets log time format.                                                          *
**********************************************************************************/
PN_LOGGER_API
void LOGGERSetLogTimeFormat( ELogTimeFormat in_eLogTimeFormat )
{
    pthread_mutex_lock( &s_sLoggerMutex );
    {
        if( s_iInitialized == 1 ) /* Already initialized */
        {
            _LOGGERSetLogTimeFormat( in_eLogTimeFormat );
        }
    }
    pthread_mutex_unlock( &s_sLoggerMutex );
}


/**********************************************************************************
* sets logging file name.                                                        *
**********************************************************************************/
PN_LOGGER_API
void LOGGERSetLogFileName(const char* in_kpcLogFileName)
{
    pthread_mutex_lock( &s_sLoggerMutex );
    {
        if( s_iInitialized == 1 ) /* Already initialized */
        {
            _LOGGEROpenLogFile( in_kpcLogFileName );
        }
    }
    pthread_mutex_unlock( &s_sLoggerMutex );
}


/*******************************************************************************
Function Name: LOGGERWriteLog

Prototype    : void LOGGERWriteLog
(
byte4           in_iLogLevel,
const char*     in_kpcFile,
const byte4     in_kiLine,
EMessageID      in_eMsgID,
const char*     in_kcfmt
);

Parameters   :
in_iLogLevel - Level of criticality of messages.
in_kpcFile   - File name from where LOGGERWriteLog is called.
in_kiLine    - Line number in the file in_kpcFile.
in_eMsgID    - Message ID indicates which msg to log.
in_kcfmt     - String format for variable list argument to log.

Return Value : None

Description  : Writes messages to log files.
*******************************************************************************/
PN_LOGGER_API
void LOGGERWriteLog
(
 ELogLevel     in_eLogLevel,
 const char*   in_kpszModuleName,
 const char*   in_kpcFile,
 int           in_iLine,
 ELogMessageID in_eMsgID,
 ...
 )
{
    char message[PN_MAX_LOG_MESSAGE_LEN];
    char time_buf[PN_MAXLEN], arrcFileInfo[PN_MAXLEN];
    time_t t;
    struct tm *ptm;
    FILE *pFP = stdout;
    va_list args;
    byte4 lUSec;;

    /* If this log bebyte4s to a higher level, don't log it */
    if (in_eLogLevel > s_eLogLevel)
    {
        return;
    }

    /* Get the current time, if required */
    time(&t);
    ptm = localtime(&t);
    UTILGetCurrentTimeOfDay(NULL, (size_t*)&lUSec);
    switch( s_eTimeFormat )
    {
    case ELOG_TIME_FORMAT_NONE:
        strcpy(time_buf, "" );
        break;
    case ELOG_TIME_FORMAT_TIME_ONLY:
        strftime(time_buf, PN_MAXLEN, "%H:%M:%S", ptm);
        sprintf(time_buf + strlen(time_buf), ".%06d", lUSec);
        break;
    default:
        strftime(time_buf, PN_MAXLEN, "%d-%b-%Y %H:%M:%S", ptm);
        sprintf(time_buf + strlen(time_buf), ".%06d", lUSec);
        break;
    }

    /* Create the log message format */
    va_start(args, in_eMsgID);
    vsnprintf(message, sizeof(message), arr_kpcLogMessages[in_eMsgID], args);
    va_end(args);

    /* Print the file name and line number in another string */
    if (s_eLogLevel >= ELOG_LEVEL_DEBUG)
    {
        sprintf(arrcFileInfo, "<%s, %d>", in_kpcFile, in_iLine);
    }
    else
    {
        strcpy( arrcFileInfo, "" );
    }

    if (s_ofLogFP != NULL)
    {
        pthread_mutex_lock(&s_sLoggerMutex);
    }

#ifndef LOG_ALL
    if (s_ofLogFP != NULL)
    {
        pFP = s_ofLogFP;
    }
    fprintf(pFP, "%s %6s | %5s | %s %s\n",
        time_buf,
        s_kpcLogString[in_eLogLevel],
        in_kpszModuleName,
        message, 
        arrcFileInfo);
#else
    if (SLogFileId[in_eLogLevel].m_ofFP)
    {
        pFP = SLogFileId[in_eLogLevel].m_ofFP;
    }
    fprintf(pFP, "%s [%d.%ld] | %6s | %5s | %s %s\n", 
        time_buf, 
        getpid(),
        pthread_self(), 
        s_kpcLogString[in_eLogLevel],
        in_kpszModuleName,
        message, 
        arrcFileInfo);
#endif

    if (pFP != stdout)
    {
        fflush(pFP);
    }

    if (s_ofLogFP != NULL)
    {
        pthread_mutex_unlock(&s_sLoggerMutex);
    }
}


/*******************************************************************************************************
Function Name: _LOGGERSetLogLevel                                                                          
Prototype    : void _LOGGERSetLogLevel();                                                                    
Parameters   : uiLogLevel                                                                                 
Return Value : None                                                                                 
Description  : Sets loglevel dynamically each time its called. 
*******************************************************************************************************/
static void _LOGGERSetLogLevel( ELogLevel uiLogLevel)
{
    if ((uiLogLevel < 0) || (uiLogLevel >=ELOG_LEVEL_MAX))
    {
        LOG_ERROR(ELOG_ILLEGAL_LOG_LEVEL, uiLogLevel);
        return;
    }
    else 
    {
        s_eLogLevel = uiLogLevel;
    }
#ifdef LOG_LEVEL
    s_eLogLevel = LOG_LEVEL;
#endif
    return;
}


static void _LOGGERSetLogTimeFormat( ELogTimeFormat uiLogTimeFormat )
{
    if( ( uiLogTimeFormat < ELOG_TIME_FORMAT_NONE ) || ( uiLogTimeFormat >= ELOG_TIME_FORMAT_MAX ) )
    {
        LOG_ERROR(ELOG_ILLEGAL_TIME_FORMAT, uiLogTimeFormat );
    }
    else
    {
        s_eTimeFormat = uiLogTimeFormat;
    }
}


/*******************************************************************************************************
Function Name: _LOGGEROpenLogFile                                                                           
Prototype    : void _LOGGEROpenLogFile();                                                                    
Parameters   : None                                                                                 
Return Value : None                                                                                 
Description  : Open log files. It operates on two modes. If LOG_ALL is defined then it opens separate
log file for each log level. If LOG_ALL is not defined then it opens a default log file 
to log for all log levels.
*******************************************************************************************************/
static void _LOGGEROpenLogFile(const char* kpcLogFile)
{

    _LOGGERCloseLogFile(); /* make sure we initialise variables and close */

#ifndef LOG_ALL
    if ((kpcLogFile != NULL))
    {
        if (!strcmp("stdout", kpcLogFile))
        {
            s_ofLogFP = NULL;
        }
        else if (!(s_ofLogFP = fopen(kpcLogFile, "w+")))
        {
            LOG_ERROR(ELOG_FOPEN_FAILED, kpcLogFile);
            return;
        }
        else
        {
        }
    }
    else if (!(s_ofLogFP = fopen(s_pcLogFile, "w+")))
    {
        LOG_ERROR(ELOG_FOPEN_FAILED, s_pcLogFile);
        return;
    } 

#else /* if LOG_ALL is defined */
    {
        int i = 0;
        char arr_cFileBase[PN_MAXLEN];
        char arr_cFileExt[PN_MAXLEN];
        char arr_pcNewLogFile[PN_MAXLEN];
        if (kpcLogFile!=NULL)
        {
            GetFileBase( ( char* )kpcLogFile,arr_cFileBase,arr_cFileExt);
        }

        for (i = 0; i < ELOG_LEVEL_MAX; i++)
        {
            memset(arr_pcNewLogFile, '\0', PN_MAXLEN);
            AppendLogLevelString(arr_pcNewLogFile, arr_cFileBase, s_kpcLogString[i],arr_cFileExt);
            if (kpcLogFile!=NULL)
            {
                if (!(SLogFileId[i].m_ofFP = fopen(arr_pcNewLogFile, "w+")))
                {
                    LOGGERWriteLog(ELOG_LEVEL_CRITICAL, __FILE__, __LINE__, ELOG_FOPEN_FAILED, arr_pcNewLogFile);
                }  
            }
            else if (!(SLogFileId[i].m_ofFP= fopen(SLogFileId[i].m_pcFileName, "w+")))
            {
                LOGGERWriteLog(ELOG_LEVEL_CRITICAL, __FILE__, __LINE__, ELOG_FOPEN_FAILED, SLogFileId[i].m_pcFileName);
            } 
        }
    }
#endif /* LOG_ALL */        
}


/*******************************************************************************************************
Function Name: _LOGGERCloseLogFile                                                                         
Prototype    : void _LOGGERCloseLogFile();                                                                    
Parameters   : None                                                                                 
Return Value : None                                                                                 
Description  : Closes log files and set log files pointer to NULL.
********************************************************************************************************/
static void _LOGGERCloseLogFile(void)
{
#ifndef LOG_ALL
    if (s_ofLogFP)
    { 
        fclose(s_ofLogFP);
        s_ofLogFP = NULL;
    } 

#else
    int i = 0;
    for (i = 0; i < ELOG_LEVEL_MAX; i++)
    {
        if (SLogFileId[i].m_ofFP)
        {
            fclose(SLogFileId[i].m_ofFP);
            SLogFileId[i].m_ofFP = NULL;
        }
    }
#endif
}
#endif
