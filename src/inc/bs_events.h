/**
 * @file    bs_events.h
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This is auto generated file from Excel
 * @section This file contains structures for Events
 *
 * Author: Shweta Polepally
 * Date: 28-12-2010
 * Description: Draft version 
 * Generated on : 2/15/2013 2:49:12 PM
 */

#ifndef BS_EVENTS_H
#define BS_EVENTS_H
 
 
#include "cmn_types.h"
#include "bs_alarms.h"


/* Maximum Columns in All Structures */
#define  MAX_COLS_IN_ALARMNOTIFICATION_STRUCT               9 
#define  MAX_COLS_IN_MMERESET_STRUCT                        4 
#define  MAX_COLS_IN_FILEUPLOADCOMPLETED_STRUCT             4 
#define  MAX_COLS_IN_FILEUPLOADFAILED_STRUCT                5 
#define  MAX_COLS_IN_FILEDOWNLOADCOMPLETED_STRUCT           4 
#define  MAX_COLS_IN_FILEDOWNLOADFAILED_STRUCT              5 
#define  MAX_COLS_IN_MESSAGETRACECOMPLETED_STRUCT           4 
#define  MAX_COLS_IN_MESSAGETRACEFAILED_STRUCT              4 
#define  MAX_COLS_IN_COUNTERFILEAVAILABLE_STRUCT            4 
#define  MAX_COLS_IN_COUNTERCOLLECTIONFAILED_STRUCT         4 
#define  MAX_COLS_IN_CONFIGURATIONCHANGED_STRUCT            4 
#define  MAX_COLS_IN_CONFIGURATIONSAVED_STRUCT              4 
#define  MAX_COLS_IN_CONFIGURATIONROLLBACK_STRUCT           4 
#define  MAX_COLS_IN_SWACTIVATIONFAILED_STRUCT              6 
#define  MAX_COLS_IN_SWACTIVATIONSUCCESS_STRUCT             5 
#define  MAX_COLS_IN_SWINSTALLFAILED_STRUCT                 5 
#define  MAX_COLS_IN_SWINSTALLSUCCESS_STRUCT                5 
#define  MAX_COLS_IN_ENBREGISTRATIONREQ_STRUCT              6 
#define  MAX_COLS_IN_ENBOPERATIONALSTATECHANGED_STRUCT      4 
#define  MAX_COLS_IN_ENBADMINSTATECHANGED_STRUCT            4 
#define  MAX_COLS_IN_ENBLOCATIONCHANGED_STRUCT              5 
#define  MAX_COLS_IN_ENBREBOOTIND_STRUCT                    4 
#define  MAX_COLS_IN_NONEIGHBORCONFIGURED_STRUCT            3 
#define  MAX_COLS_IN_CELLBARREDIND_STRUCT                   4 
#define  MAX_COLS_IN_CELLUNBARREDIND_STRUCT                 4 
#define  MAX_COLS_IN_MEASUREMENTCOMPLETED_STRUCT            5 

/* Define macros to indicate column numbers for ALARMNOTIFICATION*/
#define COL_ALARMNOTIFICATION_ENODEBID                                1 
#define COL_ALARMNOTIFICATION_ALARMSEQUENCENUMBER                     2 
#define COL_ALARMNOTIFICATION_DATEANDTIMESTAMP                        3 
#define COL_ALARMNOTIFICATION_ALARMTYPE                               4 
#define COL_ALARMNOTIFICATION_ALARMID                                 5 
#define COL_ALARMNOTIFICATION_ALARMSEVERITY                           6 
#define COL_ALARMNOTIFICATION_ALARMCAUSE                              7 
#define COL_ALARMNOTIFICATION_ALARMLOCATION                           8 
#define COL_ALARMNOTIFICATION_ALARMADDITIONALTEXT                     9 

/* Define macros to indicate column numbers for MMERESET*/
#define COL_MMERESET_ENODEBID                                         1 
#define COL_MMERESET_NOTIFICATIONSEQUENCENUMBER                       2 
#define COL_MMERESET_DATEANDTIMESTAMP                                 3 
#define COL_MMERESET_MMEIPADDR                                        4 

/* Define macros to indicate column numbers for FILEUPLOADCOMPLETED*/
#define COL_FILEUPLOADCOMPLETED_ENODEBID                              1 
#define COL_FILEUPLOADCOMPLETED_NOTIFICATIONSEQUENCENUMBER            2 
#define COL_FILEUPLOADCOMPLETED_DATEANDTIMESTAMP                      3 
#define COL_FILEUPLOADCOMPLETED_FILENAME                              4 

/* Define macros to indicate column numbers for FILEUPLOADFAILED*/
#define COL_FILEUPLOADFAILED_ENODEBID                                 1 
#define COL_FILEUPLOADFAILED_NOTIFICATIONSEQUENCENUMBER               2 
#define COL_FILEUPLOADFAILED_DATEANDTIMESTAMP                         3 
#define COL_FILEUPLOADFAILED_FILENAME                                 4 
#define COL_FILEUPLOADFAILED_ERRORINFO                                5 

/* Define macros to indicate column numbers for FILEDOWNLOADCOMPLETED*/
#define COL_FILEDOWNLOADCOMPLETED_ENODEBID                            1 
#define COL_FILEDOWNLOADCOMPLETED_NOTIFICATIONSEQUENCENUMBER          2 
#define COL_FILEDOWNLOADCOMPLETED_DATEANDTIMESTAMP                    3 
#define COL_FILEDOWNLOADCOMPLETED_FILENAME                            4 

/* Define macros to indicate column numbers for FILEDOWNLOADFAILED*/
#define COL_FILEDOWNLOADFAILED_ENODEBID                               1 
#define COL_FILEDOWNLOADFAILED_NOTIFICATIONSEQUENCENUMBER             2 
#define COL_FILEDOWNLOADFAILED_DATEANDTIMESTAMP                       3 
#define COL_FILEDOWNLOADFAILED_FILENAME                               4 
#define COL_FILEDOWNLOADFAILED_ERRORINFO                              5 

/* Define macros to indicate column numbers for MESSAGETRACECOMPLETED*/
#define COL_MESSAGETRACECOMPLETED_ENODEBID                            1 
#define COL_MESSAGETRACECOMPLETED_NOTIFICATIONSEQUENCENUMBER          2 
#define COL_MESSAGETRACECOMPLETED_DATEANDTIMESTAMP                    3 
#define COL_MESSAGETRACECOMPLETED_FILENAME                            4 

/* Define macros to indicate column numbers for MESSAGETRACEFAILED*/
#define COL_MESSAGETRACEFAILED_ENODEBID                               1 
#define COL_MESSAGETRACEFAILED_NOTIFICATIONSEQUENCENUMBER             2 
#define COL_MESSAGETRACEFAILED_DATEANDTIMESTAMP                       3 
#define COL_MESSAGETRACEFAILED_ERRORINFO                              4 

/* Define macros to indicate column numbers for COUNTERFILEAVAILABLE*/
#define COL_COUNTERFILEAVAILABLE_ENODEBID                             1 
#define COL_COUNTERFILEAVAILABLE_NOTIFICATIONSEQUENCENUMBER           2 
#define COL_COUNTERFILEAVAILABLE_DATEANDTIMESTAMP                     3 
#define COL_COUNTERFILEAVAILABLE_FILENAME                             4 

/* Define macros to indicate column numbers for COUNTERCOLLECTIONFAILED*/
#define COL_COUNTERCOLLECTIONFAILED_ENODEBID                          1 
#define COL_COUNTERCOLLECTIONFAILED_NOTIFICATIONSEQUENCENUMBER        2 
#define COL_COUNTERCOLLECTIONFAILED_DATEANDTIMESTAMP                  3 
#define COL_COUNTERCOLLECTIONFAILED_ERRORINFO                         4 

/* Define macros to indicate column numbers for CONFIGURATIONCHANGED*/
#define COL_CONFIGURATIONCHANGED_ENODEBID                             1 
#define COL_CONFIGURATIONCHANGED_NOTIFICATIONSEQUENCENUMBER           2 
#define COL_CONFIGURATIONCHANGED_DATEANDTIMESTAMP                     3 
#define COL_CONFIGURATIONCHANGED_OAMIFTYPE                            4 

/* Define macros to indicate column numbers for CONFIGURATIONSAVED*/
#define COL_CONFIGURATIONSAVED_ENODEBID                               1 
#define COL_CONFIGURATIONSAVED_NOTIFICATIONSEQUENCENUMBER             2 
#define COL_CONFIGURATIONSAVED_DATEANDTIMESTAMP                       3 
#define COL_CONFIGURATIONSAVED_OAMIFTYPE                              4 

/* Define macros to indicate column numbers for CONFIGURATIONROLLBACK*/
#define COL_CONFIGURATIONROLLBACK_ENODEBID                            1 
#define COL_CONFIGURATIONROLLBACK_NOTIFICATIONSEQUENCENUMBER          2 
#define COL_CONFIGURATIONROLLBACK_DATEANDTIMESTAMP                    3 
#define COL_CONFIGURATIONROLLBACK_OAMIFTYPE                           4 

/* Define macros to indicate column numbers for SWACTIVATIONFAILED*/
#define COL_SWACTIVATIONFAILED_ENODEBID                               1 
#define COL_SWACTIVATIONFAILED_NOTIFICATIONSEQUENCENUMBER             2 
#define COL_SWACTIVATIONFAILED_DATEANDTIMESTAMP                       3 
#define COL_SWACTIVATIONFAILED_SWTYPE                                 4 
#define COL_SWACTIVATIONFAILED_VERSION                                5 
#define COL_SWACTIVATIONFAILED_ERRORINFO                              6 

/* Define macros to indicate column numbers for SWACTIVATIONSUCCESS*/
#define COL_SWACTIVATIONSUCCESS_ENODEBID                              1 
#define COL_SWACTIVATIONSUCCESS_NOTIFICATIONSEQUENCENUMBER            2 
#define COL_SWACTIVATIONSUCCESS_DATEANDTIMESTAMP                      3 
#define COL_SWACTIVATIONSUCCESS_SWTYPE                                4 
#define COL_SWACTIVATIONSUCCESS_VERSION                               5 

/* Define macros to indicate column numbers for SWINSTALLFAILED*/
#define COL_SWINSTALLFAILED_ENODEBID                                  1 
#define COL_SWINSTALLFAILED_NOTIFICATIONSEQUENCENUMBER                2 
#define COL_SWINSTALLFAILED_DATEANDTIMESTAMP                          3 
#define COL_SWINSTALLFAILED_FILENAME                                  4 
#define COL_SWINSTALLFAILED_ERRORINFO                                 5 

/* Define macros to indicate column numbers for SWINSTALLSUCCESS*/
#define COL_SWINSTALLSUCCESS_ENODEBID                                 1 
#define COL_SWINSTALLSUCCESS_NOTIFICATIONSEQUENCENUMBER               2 
#define COL_SWINSTALLSUCCESS_DATEANDTIMESTAMP                         3 
#define COL_SWINSTALLSUCCESS_FILENAME                                 4 
#define COL_SWINSTALLSUCCESS_VERSION                                  5 

/* Define macros to indicate column numbers for ENBREGISTRATIONREQ*/
#define COL_ENBREGISTRATIONREQ_ENODEBID                               1 
#define COL_ENBREGISTRATIONREQ_NOTIFICATIONSEQUENCENUMBER             2 
#define COL_ENBREGISTRATIONREQ_DATEANDTIMESTAMP                       3 
#define COL_ENBREGISTRATIONREQ_PRODUCTID                              4 
#define COL_ENBREGISTRATIONREQ_LONGITUDE                              5 
#define COL_ENBREGISTRATIONREQ_LATITUDE                               6 

/* Define macros to indicate column numbers for ENBOPERATIONALSTATECHANGED*/
#define COL_ENBOPERATIONALSTATECHANGED_ENODEBID                       1 
#define COL_ENBOPERATIONALSTATECHANGED_NOTIFICATIONSEQUENCENUMBER     2 
#define COL_ENBOPERATIONALSTATECHANGED_DATEANDTIMESTAMP               3 
#define COL_ENBOPERATIONALSTATECHANGED_OPERATIONALSTATE               4 

/* Define macros to indicate column numbers for ENBADMINSTATECHANGED*/
#define COL_ENBADMINSTATECHANGED_ENODEBID                             1 
#define COL_ENBADMINSTATECHANGED_NOTIFICATIONSEQUENCENUMBER           2 
#define COL_ENBADMINSTATECHANGED_DATEANDTIMESTAMP                     3 
#define COL_ENBADMINSTATECHANGED_ADMINSTATE                           4 

/* Define macros to indicate column numbers for ENBLOCATIONCHANGED*/
#define COL_ENBLOCATIONCHANGED_ENODEBID                               1 
#define COL_ENBLOCATIONCHANGED_NOTIFICATIONSEQUENCENUMBER             2 
#define COL_ENBLOCATIONCHANGED_DATEANDTIMESTAMP                       3 
#define COL_ENBLOCATIONCHANGED_LONGITUDE                              4 
#define COL_ENBLOCATIONCHANGED_LATITUDE                               5 

/* Define macros to indicate column numbers for ENBREBOOTIND*/
#define COL_ENBREBOOTIND_ENODEBID                                     1 
#define COL_ENBREBOOTIND_NOTIFICATIONSEQUENCENUMBER                   2 
#define COL_ENBREBOOTIND_DATEANDTIMESTAMP                             3 
#define COL_ENBREBOOTIND_REBOOTREASON                                 4 

/* Define macros to indicate column numbers for NONEIGHBORCONFIGURED*/
#define COL_NONEIGHBORCONFIGURED_ENODEBID                             1 
#define COL_NONEIGHBORCONFIGURED_NOTIFICATIONSEQUENCENUMBER           2 
#define COL_NONEIGHBORCONFIGURED_DATEANDTIMESTAMP                     3 

/* Define macros to indicate column numbers for CELLBARREDIND*/
#define COL_CELLBARREDIND_ENODEBID                                    1 
#define COL_CELLBARREDIND_NOTIFICATIONSEQUENCENUMBER                  2 
#define COL_CELLBARREDIND_CELLID                                      3 
#define COL_CELLBARREDIND_DATEANDTIMESTAMP                            4 

/* Define macros to indicate column numbers for CELLUNBARREDIND*/
#define COL_CELLUNBARREDIND_ENODEBID                                  1 
#define COL_CELLUNBARREDIND_NOTIFICATIONSEQUENCENUMBER                2 
#define COL_CELLUNBARREDIND_CELLID                                    3 
#define COL_CELLUNBARREDIND_DATEANDTIMESTAMP                          4 

/* Define macros to indicate column numbers for MEASUREMENTCOMPLETED*/
#define COL_MEASUREMENTCOMPLETED_ENODEBID                             1 
#define COL_MEASUREMENTCOMPLETED_NOTIFICATIONSEQUENCENUMBER           2 
#define COL_MEASUREMENTCOMPLETED_DATEANDTIMESTAMP                     3 
#define COL_MEASUREMENTCOMPLETED_PROFILEID                            4 
#define COL_MEASUREMENTCOMPLETED_STOPPEDREASON                        5 

#define  STRUCT_ALARMNOTIFICATION_ID                        1001 
#define  STRUCT_MMERESET_ID                                 1002 
#define  STRUCT_FILEUPLOADCOMPLETED_ID                      1003 
#define  STRUCT_FILEUPLOADFAILED_ID                         1004 
#define  STRUCT_FILEDOWNLOADCOMPLETED_ID                    1005 
#define  STRUCT_FILEDOWNLOADFAILED_ID                       1006 
#define  STRUCT_MESSAGETRACECOMPLETED_ID                    1007 
#define  STRUCT_MESSAGETRACEFAILED_ID                       1008 
#define  STRUCT_COUNTERFILEAVAILABLE_ID                     1009 
#define  STRUCT_COUNTERCOLLECTIONFAILED_ID                  1010 
#define  STRUCT_CONFIGURATIONCHANGED_ID                     1011 
#define  STRUCT_CONFIGURATIONSAVED_ID                       1012 
#define  STRUCT_CONFIGURATIONROLLBACK_ID                    1013 
#define  STRUCT_SWACTIVATIONFAILED_ID                       1014 
#define  STRUCT_SWACTIVATIONSUCCESS_ID                      1015 
#define  STRUCT_SWINSTALLFAILED_ID                          1016 
#define  STRUCT_SWINSTALLSUCCESS_ID                         1017 
#define  STRUCT_ENBREGISTRATIONREQ_ID                       1018 
#define  STRUCT_ENBOPERATIONALSTATECHANGED_ID               1019 
#define  STRUCT_ENBADMINSTATECHANGED_ID                     1020 
#define  STRUCT_ENBLOCATIONCHANGED_ID                       1021 
#define  STRUCT_ENBREBOOTIND_ID                             1022 
#define  STRUCT_NONEIGHBORCONFIGURED_ID                     1023 
#define  STRUCT_CELLBARREDIND_ID                            1024 
#define  STRUCT_CELLUNBARREDIND_ID                          1025 
#define  STRUCT_MEASUREMENTCOMPLETED_ID                     1026 

#define MAX_SIZE_ALARMNOTIFICATION_DATEANDTIMESTAMP                   17 
#define MAX_SIZE_ALARMNOTIFICATION_ALARMLOCATION                      256 
#define MAX_SIZE_ALARMNOTIFICATION_ALARMADDITIONALTEXT                256 
#define MAX_SIZE_MMERESET_DATEANDTIMESTAMP                            17 
#define MAX_SIZE_MMERESET_MMEIPADDR                                   15 
#define MAX_SIZE_FILEUPLOADCOMPLETED_DATEANDTIMESTAMP                 17 
#define MAX_SIZE_FILEUPLOADCOMPLETED_FILENAME                         256 
#define MAX_SIZE_FILEUPLOADFAILED_DATEANDTIMESTAMP                    17 
#define MAX_SIZE_FILEUPLOADFAILED_FILENAME                            256 
#define MAX_SIZE_FILEUPLOADFAILED_ERRORINFO                           256 
#define MAX_SIZE_FILEDOWNLOADCOMPLETED_DATEANDTIMESTAMP               17 
#define MAX_SIZE_FILEDOWNLOADCOMPLETED_FILENAME                       256 
#define MAX_SIZE_FILEDOWNLOADFAILED_DATEANDTIMESTAMP                  17 
#define MAX_SIZE_FILEDOWNLOADFAILED_FILENAME                          256 
#define MAX_SIZE_FILEDOWNLOADFAILED_ERRORINFO                         256 
#define MAX_SIZE_MESSAGETRACECOMPLETED_DATEANDTIMESTAMP               17 
#define MAX_SIZE_MESSAGETRACECOMPLETED_FILENAME                       256 
#define MAX_SIZE_MESSAGETRACEFAILED_DATEANDTIMESTAMP                  17 
#define MAX_SIZE_MESSAGETRACEFAILED_ERRORINFO                         256 
#define MAX_SIZE_COUNTERFILEAVAILABLE_DATEANDTIMESTAMP                17 
#define MAX_SIZE_COUNTERFILEAVAILABLE_FILENAME                        256 
#define MAX_SIZE_COUNTERCOLLECTIONFAILED_DATEANDTIMESTAMP             17 
#define MAX_SIZE_COUNTERCOLLECTIONFAILED_ERRORINFO                    256 
#define MAX_SIZE_CONFIGURATIONCHANGED_DATEANDTIMESTAMP                17 
#define MAX_SIZE_CONFIGURATIONSAVED_DATEANDTIMESTAMP                  17 
#define MAX_SIZE_CONFIGURATIONROLLBACK_DATEANDTIMESTAMP               17 
#define MAX_SIZE_SWACTIVATIONFAILED_DATEANDTIMESTAMP                  17 
#define MAX_SIZE_SWACTIVATIONFAILED_VERSION                           256 
#define MAX_SIZE_SWACTIVATIONFAILED_ERRORINFO                         256 
#define MAX_SIZE_SWACTIVATIONSUCCESS_DATEANDTIMESTAMP                 17 
#define MAX_SIZE_SWACTIVATIONSUCCESS_VERSION                          256 
#define MAX_SIZE_SWINSTALLFAILED_DATEANDTIMESTAMP                     17 
#define MAX_SIZE_SWINSTALLFAILED_FILENAME                             256 
#define MAX_SIZE_SWINSTALLFAILED_ERRORINFO                            256 
#define MAX_SIZE_SWINSTALLSUCCESS_DATEANDTIMESTAMP                    17 
#define MAX_SIZE_SWINSTALLSUCCESS_FILENAME                            256 
#define MAX_SIZE_SWINSTALLSUCCESS_VERSION                             256 
#define MAX_SIZE_ENBREGISTRATIONREQ_DATEANDTIMESTAMP                  17 
#define MAX_SIZE_ENBREGISTRATIONREQ_PRODUCTID                         50 
#define MAX_SIZE_ENBREGISTRATIONREQ_LONGITUDE                         13 
#define MAX_SIZE_ENBREGISTRATIONREQ_LATITUDE                          12 
#define MAX_SIZE_ENBOPERATIONALSTATECHANGED_DATEANDTIMESTAMP          17 
#define MAX_SIZE_ENBADMINSTATECHANGED_DATEANDTIMESTAMP                17 
#define MAX_SIZE_ENBLOCATIONCHANGED_DATEANDTIMESTAMP                  17 
#define MAX_SIZE_ENBLOCATIONCHANGED_LONGITUDE                         13 
#define MAX_SIZE_ENBLOCATIONCHANGED_LATITUDE                          12 
#define MAX_SIZE_ENBREBOOTIND_DATEANDTIMESTAMP                        17 
#define MAX_SIZE_ENBREBOOTIND_REBOOTREASON                            50 
#define MAX_SIZE_NONEIGHBORCONFIGURED_DATEANDTIMESTAMP                17 
#define MAX_SIZE_CELLBARREDIND_DATEANDTIMESTAMP                       17 
#define MAX_SIZE_CELLUNBARREDIND_DATEANDTIMESTAMP                     17 
#define MAX_SIZE_MEASUREMENTCOMPLETED_DATEANDTIMESTAMP                17 
#define MAX_SIZE_MEASUREMENTCOMPLETED_STOPPEDREASON                   50 

/***********************************************/
/*                  STRUCTURES                 */
/***********************************************/
typedef struct s_MsgBsmSihAlarmNotification
{
 UINT32                      enodebId;
 UINT32                      alarmSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_ALARMNOTIFICATION_DATEANDTIMESTAMP + 1 ]; 
 AlarmType                   alarmType;
 UINT32                      alarmId;
 AlarmSeverity               alarmSeverity;
 AlarmCause                  alarmCause;
 UINT8                       alarmLocation[MAX_SIZE_ALARMNOTIFICATION_ALARMLOCATION + 1 ]; 
 UINT8                       alarmAdditionalText[MAX_SIZE_ALARMNOTIFICATION_ALARMADDITIONALTEXT + 1 ]; 
} MsgBsmSihAlarmNotification;
 
typedef struct s_MsgBsmSihMmeReset
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_MMERESET_DATEANDTIMESTAMP + 1 ]; 
 UINT8                       mmeIpAddr[MAX_SIZE_MMERESET_MMEIPADDR + 1 ]; 
} MsgBsmSihMmeReset;
 
typedef struct s_MsgBsmSihFileUploadCompleted
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_FILEUPLOADCOMPLETED_DATEANDTIMESTAMP + 1 ]; 
 UINT8                       fileName[MAX_SIZE_FILEUPLOADCOMPLETED_FILENAME + 1 ]; 
} MsgBsmSihFileUploadCompleted;
 
typedef struct s_MsgBsmSihFileUploadFailed
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_FILEUPLOADFAILED_DATEANDTIMESTAMP + 1 ]; 
 UINT8                       fileName[MAX_SIZE_FILEUPLOADFAILED_FILENAME + 1 ]; 
 UINT8                       errorInfo[MAX_SIZE_FILEUPLOADFAILED_ERRORINFO + 1 ]; 
} MsgBsmSihFileUploadFailed;
 
typedef struct s_MsgBsmSihFileDownloadCompleted
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_FILEDOWNLOADCOMPLETED_DATEANDTIMESTAMP + 1 ]; 
 UINT8                       fileName[MAX_SIZE_FILEDOWNLOADCOMPLETED_FILENAME + 1 ]; 
} MsgBsmSihFileDownloadCompleted;
 
typedef struct s_MsgBsmSihFileDownloadFailed
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_FILEDOWNLOADFAILED_DATEANDTIMESTAMP + 1 ]; 
 UINT8                       fileName[MAX_SIZE_FILEDOWNLOADFAILED_FILENAME + 1 ]; 
 UINT8                       errorInfo[MAX_SIZE_FILEDOWNLOADFAILED_ERRORINFO + 1 ]; 
} MsgBsmSihFileDownloadFailed;
 
typedef struct s_MsgBsmSihMessageTraceCompleted
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_MESSAGETRACECOMPLETED_DATEANDTIMESTAMP + 1 ]; 
 UINT8                       fileName[MAX_SIZE_MESSAGETRACECOMPLETED_FILENAME + 1 ]; 
} MsgBsmSihMessageTraceCompleted;
 
typedef struct s_MsgBsmSihMessageTraceFailed
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_MESSAGETRACEFAILED_DATEANDTIMESTAMP + 1 ]; 
 UINT8                       errorInfo[MAX_SIZE_MESSAGETRACEFAILED_ERRORINFO + 1 ]; 
} MsgBsmSihMessageTraceFailed;
 
typedef struct s_MsgBsmSihCounterFileAvailable
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_COUNTERFILEAVAILABLE_DATEANDTIMESTAMP + 1 ]; 
 UINT8                       fileName[MAX_SIZE_COUNTERFILEAVAILABLE_FILENAME + 1 ]; 
} MsgBsmSihCounterFileAvailable;
 
typedef struct s_MsgBsmSihCounterCollectionFailed
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_COUNTERCOLLECTIONFAILED_DATEANDTIMESTAMP + 1 ]; 
 UINT8                       errorInfo[MAX_SIZE_COUNTERCOLLECTIONFAILED_ERRORINFO + 1 ]; 
} MsgBsmSihCounterCollectionFailed;
 
typedef struct s_MsgBsmSihConfigurationChanged
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_CONFIGURATIONCHANGED_DATEANDTIMESTAMP + 1 ]; 
 OamIfType                   oamIfType;
} MsgBsmSihConfigurationChanged;
 
typedef struct s_MsgBsmSihConfigurationSaved
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_CONFIGURATIONSAVED_DATEANDTIMESTAMP + 1 ]; 
 OamIfType                   oamIfType;
} MsgBsmSihConfigurationSaved;
 
typedef struct s_MsgBsmSihConfigurationRollBack
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_CONFIGURATIONROLLBACK_DATEANDTIMESTAMP + 1 ]; 
 OamIfType                   oamIfType;
} MsgBsmSihConfigurationRollBack;
 
typedef struct s_MsgBsmSihSwActivationFailed
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_SWACTIVATIONFAILED_DATEANDTIMESTAMP + 1 ]; 
 SwType                      swType;
 UINT8                       version[MAX_SIZE_SWACTIVATIONFAILED_VERSION + 1 ]; 
 UINT8                       errorInfo[MAX_SIZE_SWACTIVATIONFAILED_ERRORINFO + 1 ]; 
} MsgBsmSihSwActivationFailed;
 
typedef struct s_MsgBsmSihSwActivationSuccess
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_SWACTIVATIONSUCCESS_DATEANDTIMESTAMP + 1 ]; 
 SwType                      swType;
 UINT8                       version[MAX_SIZE_SWACTIVATIONSUCCESS_VERSION + 1 ]; 
} MsgBsmSihSwActivationSuccess;
 
typedef struct s_MsgBsmSihSwInstallFailed
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_SWINSTALLFAILED_DATEANDTIMESTAMP + 1 ]; 
 UINT8                       fileName[MAX_SIZE_SWINSTALLFAILED_FILENAME + 1 ]; 
 UINT8                       errorInfo[MAX_SIZE_SWINSTALLFAILED_ERRORINFO + 1 ]; 
} MsgBsmSihSwInstallFailed;
 
typedef struct s_MsgBsmSihSwInstallSuccess
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_SWINSTALLSUCCESS_DATEANDTIMESTAMP + 1 ]; 
 UINT8                       fileName[MAX_SIZE_SWINSTALLSUCCESS_FILENAME + 1 ]; 
 UINT8                       version[MAX_SIZE_SWINSTALLSUCCESS_VERSION + 1 ]; 
} MsgBsmSihSwInstallSuccess;
 
typedef struct s_MsgBsmSihEnbRegistrationReq
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_ENBREGISTRATIONREQ_DATEANDTIMESTAMP + 1 ]; 
 UINT8                       productId[MAX_SIZE_ENBREGISTRATIONREQ_PRODUCTID + 1 ]; 
 UINT8                       longitude[MAX_SIZE_ENBREGISTRATIONREQ_LONGITUDE + 1 ]; 
 UINT8                       latitude[MAX_SIZE_ENBREGISTRATIONREQ_LATITUDE + 1 ]; 
} MsgBsmSihEnbRegistrationReq;
 
typedef struct s_MsgBsmSihEnbOperationalStateChanged
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_ENBOPERATIONALSTATECHANGED_DATEANDTIMESTAMP + 1 ]; 
 OperationalState            operationalState;
} MsgBsmSihEnbOperationalStateChanged;
 
typedef struct s_MsgBsmSihEnbAdminStateChanged
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_ENBADMINSTATECHANGED_DATEANDTIMESTAMP + 1 ]; 
 AdminState                  adminState;
} MsgBsmSihEnbAdminStateChanged;
 
typedef struct s_MsgBsmSihEnbLocationChanged
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_ENBLOCATIONCHANGED_DATEANDTIMESTAMP + 1 ]; 
 UINT8                       longitude[MAX_SIZE_ENBLOCATIONCHANGED_LONGITUDE + 1 ]; 
 UINT8                       latitude[MAX_SIZE_ENBLOCATIONCHANGED_LATITUDE + 1 ]; 
} MsgBsmSihEnbLocationChanged;
 
typedef struct s_MsgBsmSihEnbRebootInd
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_ENBREBOOTIND_DATEANDTIMESTAMP + 1 ]; 
 UINT8                       rebootReason[MAX_SIZE_ENBREBOOTIND_REBOOTREASON + 1 ]; 
} MsgBsmSihEnbRebootInd;
 
typedef struct s_MsgBsmSihNoNeighborConfigured
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_NONEIGHBORCONFIGURED_DATEANDTIMESTAMP + 1 ]; 
} MsgBsmSihNoNeighborConfigured;
 
typedef struct s_MsgBsmSihCellBarredInd
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       cellId;
 UINT8                       dateAndTimeStamp[MAX_SIZE_CELLBARREDIND_DATEANDTIMESTAMP + 1 ]; 
} MsgBsmSihCellBarredInd;
 
typedef struct s_MsgBsmSihCellUnBarredInd
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       cellId;
 UINT8                       dateAndTimeStamp[MAX_SIZE_CELLUNBARREDIND_DATEANDTIMESTAMP + 1 ]; 
} MsgBsmSihCellUnBarredInd;
 
typedef struct s_MsgBsmSihMeasurementCompleted
{
 UINT32                      enodebId;
 UINT32                      notificationSequenceNumber;
 UINT8                       dateAndTimeStamp[MAX_SIZE_MEASUREMENTCOMPLETED_DATEANDTIMESTAMP + 1 ]; 
 ProfileId                   profileId;
 UINT8                       stoppedReason[MAX_SIZE_MEASUREMENTCOMPLETED_STOPPEDREASON + 1 ]; 
} MsgBsmSihMeasurementCompleted;
 

#endif /* BS_EVENTS_H */
