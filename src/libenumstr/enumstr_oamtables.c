/**
 * @file    enumstr_oamtables.c
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This is auto generated file from Excel
 * @section This file contains functions to print ENUM Information into strings
 *
 * Author: Madhusudana Rao Kata
 * Date: 13-09-2010
 * Description  : Draft version 
 *              : Modified Header files and renamed file name to prepare enum print library -- 27/09/2010 --Madhu 
 * Generated on : 13-02-2013 16:46:23
 */

#include  "enumstr_oamtables.h"
#include  "enumstr.h"

/*****************************************************************************/
/**Fill Table with Function pointers , table Ids and column Ids  **/
/*****************************************************************************/
IN EnumStrInfo c_EnumStrInfoTab[] =
{ /* Action Handler            tableNum colNum  */
  { eNbCommandStr,                     1,    2   },
  { fileUploadCommandStr,              3,    2   },
  { protocolUsedStr,                   3,    7   },
  { maxUploadSpeedStr,                 3,    9   },
  { fileDownloadCommandStr,            2,    2   },
  { protocolUsedStr,                   2,    7   },
  { maxDownloadSpeedStr,               2,    9   },
  { swTypeStr,                         4,    2   },
  { installationTypeStr,               5,    2   },
  { traceCommandStr,                   6,    2   },
  { traceDurationStr,                  6,    3   },
  { cellCommandStr,                    7,    3   },
  { profileIdStr,                      8,    2   },
  { counterCommandStr,                 8,    3   },
  { measurementDurationStr,            8,    4   },
  { reportingDurationStr,              8,    5   },
  { timeAndLocationSourceToUseStr,     51,   12  },
  { cellTypeStr,                       53,   2   },
  { dlBandwidthStr,                    53,   7   },
  { ulBandwidthStr,                    53,   9   },
  { cellReservedForOperatorUseStr,     53,   11  },
  { acBarrMoSigFactorStr,              53,   15  },
  { acBarrMoSigTimeStr,                53,   16  },
  { acBarrMoDataFactorStr,             53,   19  },
  { acBarrMoDataTimeStr,               53,   20  },
  { rsrpFilterCoefficientStr,          53,   23  },
  { ueResponseTimerStr,                53,   29  },
  { antInfoTxModeStr,                  53,   36  },
  { cqiPeriodicityIdxStr,              53,   39  },
  { sriPeriodicityIdxStr,              53,   40  },
  { ueCqiBasedRlfDetectionTimerStr,    53,   41  },
  { schedulingAlgorithmStr,            56,   2   },
  { maxUlHarqTxStr,                    56,   4   },
  { periodicPhrTimerStr,               56,   7   },
  { prohibitPhrTimerStr,               56,   8   },
  { dlPathlossChangeStr,               56,   9   },
  { periodicBsrTimerStr,               56,   10  },
  { retxBsrTimerStr,                   56,   11  },
  { phichResourceStr,                  57,   2   },
  { alphaStr,                          57,   7   },
  { cellPhyPaStr,                      57,   9   },
  { phichDurationStr,                  57,   16  },
  { intraFreqReselectionStr,           54,   2   },
  { sib2PeriodicityStr,                54,   5   },
  { sib3PeriodicityStr,                54,   6   },
  { sib4PeriodicityStr,                54,   7   },
  { siWindowLengthStr,                 54,   8   },
  { numRaPreamblesStr,                 55,   2   },
  { sizeRAPreamblesGroupAStr,          55,   3   },
  { messageSizeGroupAStr,              55,   4   },
  { messagePowerOffsetGroupBStr,       55,   5   },
  { powerRampingStepStr,               55,   6   },
  { preambleInitRcvdTgtPowerStr,       55,   7   },
  { preambleTransMaxStr,               55,   8   },
  { raRspWindowSizeStr,                55,   9   },
  { macContResolutionTimerStr,         55,   10  },
  { modificationPeriodCoeffStr,        55,   12  },
  { defaultPagCycleStr,                55,   13  },
  { nBStr,                             55,   14  },
  { puschFreqHoppingModeStr,           55,   20  },
  { deltaPucchShiftStr,                55,   26  },
  { srsBandwidthConfigStr,             55,   28  },
  { srsSubfrmConfigStr,                55,   29  },
  { deltaFPucchFormat1Str,             55,   32  },
  { deltaFPucchFormat1bStr,            55,   33  },
  { deltaFPucchFormat2Str,             55,   34  },
  { deltaFPucchFormat2aStr,            55,   35  },
  { deltaFPucchFormat2bStr,            55,   36  },
  { t300Str,                           55,   38  },
  { t301Str,                           55,   39  },
  { t310Str,                           55,   40  },
  { n310Str,                           55,   41  },
  { t311Str,                           55,   42  },
  { n311Str,                           55,   43  },
  { timeAlignmentTimerStr,             55,   45  },
  { rlcModeStr,                        59,   4   },
  { amRlcTPollRetransmitStr,           59,   5   },
  { amRlcPollPDUStr,                   59,   6   },
  { amRlcPollByteStr,                  59,   7   },
  { amRlcMaxRetxThresholdStr,          59,   8   },
  { amRlcTReorderingStr,               59,   9   },
  { amRlcTStatusProhibitStr,           59,   10  },
  { discardTimerStr,                   59,   11  },
  { drbPrioritisedBitRateStr,          59,   14  },
  { drbBucketSizeDurationStr,          59,   15  },
  { sampleIntervalStr,                 63,   2   },
  { pmFileCreationIntervalStr,         63,   3   },
  { timeToTriggerStr,                  60,   7   },
  { triggerQuantityStr,                60,   8   },
  { reportQuantityStr,                 60,   9   },
  { filterCoefficientRsrqStr,          60,   10  },
  { filterCoefficientRsrpStr,          60,   11  },
  { reportIntervalStr,                 60,   12  },
  { reportAmountStr,                   60,   13  },
  { supportedEventsStr,                60,   15  },
  { neighCellMeasBandwidthStr,         61,   9   },
  { cellIndividualOffsetStr,           61,   10  },
  { intraFreqNeighCellQOffsetCellStr,  61,   11  },
  { qHystStr,                          65,   2   },
  { allowMeasBwStr,                    65,   8   },
  { srsBandwidthStr,                   68,   1   },
  { srsHoppingBandWidthStr,            68,   2   },
  { uuCipheringStr,                    69,   2   },
  { uuIntegrityProtectionStr,          69,   3   },
  { ueEventMeasuresStr,                70,   4   },
  { profileIdStr,                      71,   3   },
  { traceDestinationStr,               62,   8   },
  { fileUploadCommandReceivedStr,      102,  2   },
  { uploadStatusStr,                   102,  4   },
  { fileDownloadCommandReceivedStr,    101,  2   },
  { downloadStatusStr,                 101,  4   },
  { swStatusStr,                       103,  3   },
  { swTypeStr,                         104,  2   },
  { componentTypeStr,                  105,  2   },
  { componentOperationalStateStr,      105,  5   },
  { connectedToMMEStr,                 106,  5   },
  { adminStateStr,                     106,  6   },
  { operationalStateStr,               106,  7   },
  { loadStateStr,                      106,  11  },
  { dateAndTimeSourceStr,              106,  18  },
  { restartReasonStr,                  124,  10  },
  { c1IQMapStr,                        112,  2   },
  { c2IQMapStr,                        112,  3   },
  { c3IQMapStr,                        112,  4   },
  { c4IQMapStr,                        112,  5   },
  { mappingOf1FAStatusStr,             112,  8   },
  { mappingOf2FAStatusStr,             112,  9   },
  { mappingOf3FAStatusStr,             112,  10  },
  { mappingOf4FAStatusStr,             112,  11  },
  { fa1ChannelBandwidthStatusStr,      112,  24  },
  { fa2ChannelBandwidthStatusStr,      112,  25  },
  { fa3ChannelBandwidthStatusStr,      112,  26  },
  { fa4ChannelBandwidthStatusStr,      112,  27  },
  { fileStatusStr,                     114,  3   },
  { ueCategoryStr,                     116,  11  },
  { ueAccessStratumReleaseStr,         116,  12  },
  { activeIntProtAlgoStr,              116,  13  },
  { activeCiphAlgoStr,                 116,  14  },
  { eventTypeStr,                      119,  5   },
  { rabReleaseCauseStr,                118,  8   },
  { alarmTypeStr,                      120,  4   },
  { alarmSeverityStr,                  120,  6   },
  { alarmCauseStr,                     120,  7   },
  { alarmNotificationTypeStr,          121,  2   },
  { alarmTypeStr,                      121,  5   },
  { alarmSeverityStr,                  121,  7   },
  { alarmCauseStr,                     121,  8   },
  { locationSourceStr,                 126,  2   },
  { ifTypeStr,                         125,  3   },
  { ifSpeedStr,                        125,  5   },
  { ifAdminStatusStr,                  125,  7   },
  { ifOperStatusStr,                   125,  8   },
  { ipSystemStatsIPVersionStr,         135,  1   },
  { priviledgeLevelStr,                132,  3   },
  { oamInterfaceTypeStr,               132,  4   },
  { ledIdStr,                          136,  2   },
  { ledColorStr,                       136,  3   },
  { profileIdStr,                      140,  2   },
  { measurementDurationStr,            140,  4   },
  { reportingDurationStr,              140,  5   },
  { traceDestinationStr,               141,  6   },
  { traceEndCauseStr,                  141,  8   },
  { traceStateStr,                     113,  3   },
  { traceDestinationStr,               113,  7   },
  { msgTraceLevelStr,                  204,  2   },
  { msgTraceDirectionStr,              204,  3   },
  { timerNameStr,                      201,  1   },
  { timeUnitStr,                       201,  2   },
  { scSpacingStr,                      208,  3   },
  { cycPfxTypeStr,                     208,  4   },
  { srbPollRetransStr,                 208,  6   },
  { srbPollPduStr,                     208,  7   },
  { srbPollByteStr,                    208,  8   },
  { srbMaxReTxThreshStr,               208,  9   },
  { srbTreordrngStr,                   208,  10  },
  { srbTStatProhibitStr,               208,  11  },
  { srsPeriodicityStr,                 208,  23  },
  { dedCyclicShiftStr,                 208,  25  },
  { dsrTransMaxStr,                    208,  28  },
  { repFactStr,                        208,  31  },
  { aPeriodicModeStr,                  208,  35  },
  { maxUlHqTxStr,                      208,  40  },
  { ulUmSnFieldLenStr,                 208,  90  },
  { dlUmSnFieldLenStr,                 208,  91  },
  { umRlcTReorderingStr,               208,  92  },
  { internalUeConfigTimerStr,          208,  95  },
  { NULL,                              0,    0   }
};
/*****************************************************************************/
/**Generate the Print  functions For Enums **/
/*****************************************************************************/

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* eNbCommandStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "ENBCOMMAND_LOCKENB";
  }
  if( enumNum EQ 2)
  {
    return "ENBCOMMAND_UNLOCKENB";
  }
  if( enumNum EQ 3)
  {
    return "ENBCOMMAND_RESTARTENB";
  }
  if( enumNum EQ 4)
  {
    return "ENBCOMMAND_SAVECONFIG";
  }
  if( enumNum EQ 5)
  {
    return "ENBCOMMAND_ROLLBACKCONFIG";
  }
  if( enumNum EQ 6)
  {
    return "ENBCOMMAND_RESETRADIOHEAD";
  }
  if( enumNum EQ 7)
  {
    return "ENBCOMMAND_RESETGPS";
  }
  if( enumNum EQ 8)
  {
    return "ENBCOMMAND_RUNBUILTINSELFTEST";
  }
  if( enumNum EQ 9)
  {
    return "ENBCOMMAND_SETLONGPOST";
  }
  if( enumNum EQ 10)
  {
    return "ENBCOMMAND_REBOOTENB";
  }
  if( enumNum EQ 11)
  {
    return "ENBCOMMAND_RESETCOUNTERS";
  }
  if( enumNum EQ 12)
  {
    return "ENBCOMMAND_STARTDIAG";
  }
  if( enumNum EQ 13)
  {
    return "ENBCOMMAND_STOPDIAG";
  }
  if( enumNum EQ 14)
  {
    return "ENBCOMMAND_DELETEALARMHISTORY";
  }
  if( enumNum EQ 15)
  {
    return "ENBCOMMAND_DELETEUEEVENTHISTORY";
  }
  if( enumNum EQ 16)
  {
    return "ENBCOMMAND_DELETERABHISTORY";
  }
  if( enumNum EQ 17)
  {
    return "ENBCOMMAND_DELETEFILEEVENTHISTORY";
  }
  if( enumNum EQ 18)
  {
    return "ENBCOMMAND_DELETESWINSTALLHISTORY";
  }
  if( enumNum EQ 19)
  {
    return "ENBCOMMAND_DELETETRACEHISTORY";
  }
  if( enumNum EQ 20)
  {
    return "ENBCOMMAND_RESERVEDFORFUTUREUSE1";
  }
  if( enumNum EQ 21)
  {
    return "ENBCOMMAND_RESERVEDFORFUTUREUSE2";
  }
  if( enumNum EQ 22)
  {
    return "ENBCOMMAND_RESERVEDFORFUTUREUSE3";
  }
  if( enumNum EQ 23)
  {
    return "ENBCOMMAND_RESERVEDFORFUTUREUSE4";
  }
  if( enumNum EQ 24)
  {
    return "ENBCOMMAND_RESERVEDFORFUTUREUSE5";
  }
  if( enumNum EQ 25)
  {
    return "ENBCOMMAND_RESERVEDFORFUTUREUSE6";
  }
  if( enumNum EQ 26)
  {
    return "ENBCOMMAND_RESERVEDFORFUTUREUSE7";
  }
  if( enumNum EQ 27)
  {
    return "ENBCOMMAND_RESERVEDFORFUTUREUSE8";
  }
  if( enumNum EQ 28)
  {
    return "ENBCOMMAND_RESERVEDFORFUTUREUSE9";
  }
  if( enumNum EQ 29)
  {
    return "ENBCOMMAND_RESERVEDFORFUTUREUSE10";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* fileUploadCommandStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "FILEUPLOADCOMMAND_UPLOAD";
  }
  if( enumNum EQ 2)
  {
    return "FILEUPLOADCOMMAND_UPLOADCANCEL";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* protocolUsedStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "PROTOCOLUSED_FTP";
  }
  if( enumNum EQ 2)
  {
    return "PROTOCOLUSED_SCP";
  }
  if( enumNum EQ 3)
  {
    return "PROTOCOLUSED_SFTP";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* maxUploadSpeedStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "MAXUPLOADSPEED_50KBPS";
  }
  if( enumNum EQ 2)
  {
    return "MAXUPLOADSPEED_100KBPS";
  }
  if( enumNum EQ 3)
  {
    return "MAXUPLOADSPEED_500KBPS";
  }
  if( enumNum EQ 4)
  {
    return "MAXUPLOADSPEED_1MBPS";
  }
  if( enumNum EQ 5)
  {
    return "MAXUPLOADSPEED_10MBPS";
  }
  if( enumNum EQ 6)
  {
    return "MAXUPLOADSPEED_UNLIMITED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* fileDownloadCommandStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "FILEDOWNLOADCOMMAND_INSTANTDOWNLOAD";
  }
  if( enumNum EQ 2)
  {
    return "FILEDOWNLOADCOMMAND_SCHEDULEADOWNLOAD";
  }
  if( enumNum EQ 3)
  {
    return "FILEDOWNLOADCOMMAND_MODIFYSCHEDULEDDOWNLOAD";
  }
  if( enumNum EQ 4)
  {
    return "FILEDOWNLOADCOMMAND_CANCELDOWNLOAD";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* maxDownloadSpeedStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "MAXDOWNLOADSPEED_50KBPS";
  }
  if( enumNum EQ 2)
  {
    return "MAXDOWNLOADSPEED_100KBPS";
  }
  if( enumNum EQ 3)
  {
    return "MAXDOWNLOADSPEED_500KBPS";
  }
  if( enumNum EQ 4)
  {
    return "MAXDOWNLOADSPEED_1MBPS";
  }
  if( enumNum EQ 5)
  {
    return "MAXDOWNLOADSPEED_10MBPS";
  }
  if( enumNum EQ 6)
  {
    return "MAXDOWNLOADSPEED_UNLIMITED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* swTypeStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "SWTYPE_ENB_L2L3_SW";
  }
  if( enumNum EQ 2)
  {
    return "SWTYPE_RH_SW";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* installationTypeStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "INSTALLATIONTYPE_L2L3RELEASE";
  }
  if( enumNum EQ 2)
  {
    return "INSTALLATIONTYPE_L2L3CONFIGURATION";
  }
  if( enumNum EQ 3)
  {
    return "INSTALLATIONTYPE_L2L3PATCH";
  }
  if( enumNum EQ 4)
  {
    return "INSTALLATIONTYPE_RHRELEASE";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* traceCommandStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "TRACECOMMAND_STARTTRACE";
  }
  if( enumNum EQ 2)
  {
    return "TRACECOMMAND_STOPTRACE";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* traceDurationStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "TRACEDURATION_5MIN";
  }
  if( enumNum EQ 2)
  {
    return "TRACEDURATION_15MIN";
  }
  if( enumNum EQ 3)
  {
    return "TRACEDURATION_30MIN";
  }
  if( enumNum EQ 4)
  {
    return "TRACEDURATION_45MIN";
  }
  if( enumNum EQ 5)
  {
    return "TRACEDURATION_60MIN";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* cellCommandStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "CELLCOMMAND_BAR";
  }
  if( enumNum EQ 2)
  {
    return "CELLCOMMAND_UNBAR";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* profileIdStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "PROFILEID_PROFILE1";
  }
  if( enumNum EQ 2)
  {
    return "PROFILEID_PROFILE2";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* counterCommandStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "COUNTERCOMMAND_START";
  }
  if( enumNum EQ 2)
  {
    return "COUNTERCOMMAND_STOP";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* measurementDurationStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "MEASUREMENTDURATION_2MINS";
  }
  if( enumNum EQ 2)
  {
    return "MEASUREMENTDURATION_15MINS";
  }
  if( enumNum EQ 3)
  {
    return "MEASUREMENTDURATION_30MINS";
  }
  if( enumNum EQ 4)
  {
    return "MEASUREMENTDURATION_1HOUR";
  }
  if( enumNum EQ 5)
  {
    return "MEASUREMENTDURATION_2HOUR";
  }
  if( enumNum EQ 6)
  {
    return "MEASUREMENTDURATION_INFINITE";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* reportingDurationStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "REPORTINGDURATION_2MINS";
  }
  if( enumNum EQ 2)
  {
    return "REPORTINGDURATION_15MINS";
  }
  if( enumNum EQ 3)
  {
    return "REPORTINGDURATION_30MINS";
  }
  if( enumNum EQ 4)
  {
    return "REPORTINGDURATION_1HOUR";
  }
  if( enumNum EQ 5)
  {
    return "REPORTINGDURATION_2HOUR";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* timeAndLocationSourceToUseStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "TIMEANDLOCATIONSOURCETOUSE_GPS";
  }
  if( enumNum EQ 2)
  {
    return "TIMEANDLOCATIONSOURCETOUSE_OPERCONFIGURED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* cellTypeStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "CELLTYPE_VERYSMALL";
  }
  if( enumNum EQ 1)
  {
    return "CELLTYPE_SMALL";
  }
  if( enumNum EQ 2)
  {
    return "CELLTYPE_MEDIUM";
  }
  if( enumNum EQ 3)
  {
    return "CELLTYPE_LARGE";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* dlBandwidthStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "DLBANDWIDTH_1DOT4MHZ";
  }
  if( enumNum EQ 1)
  {
    return "DLBANDWIDTH_3MHZ";
  }
  if( enumNum EQ 2)
  {
    return "DLBANDWIDTH_5MHZ";
  }
  if( enumNum EQ 3)
  {
    return "DLBANDWIDTH_10MHZ";
  }
  if( enumNum EQ 4)
  {
    return "DLBANDWIDTH_15MHZ";
  }
  if( enumNum EQ 5)
  {
    return "DLBANDWIDTH_20MHZ";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* ulBandwidthStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "ULBANDWIDTH_1DOT4MHZ";
  }
  if( enumNum EQ 1)
  {
    return "ULBANDWIDTH_3MHZ";
  }
  if( enumNum EQ 2)
  {
    return "ULBANDWIDTH_5MHZ";
  }
  if( enumNum EQ 3)
  {
    return "ULBANDWIDTH_10MHZ";
  }
  if( enumNum EQ 4)
  {
    return "ULBANDWIDTH_15MHZ";
  }
  if( enumNum EQ 5)
  {
    return "ULBANDWIDTH_20MHZ";
  }
  if( enumNum EQ 6)
  {
    return "ULBANDWIDTH_NOTDEFINED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* cellReservedForOperatorUseStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "CELLRESERVEDFOROPERATORUSE_RESERVED";
  }
  if( enumNum EQ 1)
  {
    return "CELLRESERVEDFOROPERATORUSE_NOTRESERVED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* acBarrMoSigFactorStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "ACBARRMOSIGFACTOR_P00";
  }
  if( enumNum EQ 1)
  {
    return "ACBARRMOSIGFACTOR_P05";
  }
  if( enumNum EQ 2)
  {
    return "ACBARRMOSIGFACTOR_P10";
  }
  if( enumNum EQ 3)
  {
    return "ACBARRMOSIGFACTOR_P15";
  }
  if( enumNum EQ 4)
  {
    return "ACBARRMOSIGFACTOR_P20";
  }
  if( enumNum EQ 5)
  {
    return "ACBARRMOSIGFACTOR_P25";
  }
  if( enumNum EQ 6)
  {
    return "ACBARRMOSIGFACTOR_P30";
  }
  if( enumNum EQ 7)
  {
    return "ACBARRMOSIGFACTOR_P40";
  }
  if( enumNum EQ 8)
  {
    return "ACBARRMOSIGFACTOR_P50";
  }
  if( enumNum EQ 9)
  {
    return "ACBARRMOSIGFACTOR_P60";
  }
  if( enumNum EQ 10)
  {
    return "ACBARRMOSIGFACTOR_P70";
  }
  if( enumNum EQ 11)
  {
    return "ACBARRMOSIGFACTOR_P75";
  }
  if( enumNum EQ 12)
  {
    return "ACBARRMOSIGFACTOR_P80";
  }
  if( enumNum EQ 13)
  {
    return "ACBARRMOSIGFACTOR_P85";
  }
  if( enumNum EQ 14)
  {
    return "ACBARRMOSIGFACTOR_P90";
  }
  if( enumNum EQ 15)
  {
    return "ACBARRMOSIGFACTOR_P95";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* acBarrMoSigTimeStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "ACBARRMOSIGTIME_S4";
  }
  if( enumNum EQ 1)
  {
    return "ACBARRMOSIGTIME_S8";
  }
  if( enumNum EQ 2)
  {
    return "ACBARRMOSIGTIME_S16";
  }
  if( enumNum EQ 3)
  {
    return "ACBARRMOSIGTIME_S32";
  }
  if( enumNum EQ 4)
  {
    return "ACBARRMOSIGTIME_S64";
  }
  if( enumNum EQ 5)
  {
    return "ACBARRMOSIGTIME_S128";
  }
  if( enumNum EQ 6)
  {
    return "ACBARRMOSIGTIME_S256";
  }
  if( enumNum EQ 7)
  {
    return "ACBARRMOSIGTIME_S512";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* acBarrMoDataFactorStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "ACBARRMODATAFACTOR_P00";
  }
  if( enumNum EQ 1)
  {
    return "ACBARRMODATAFACTOR_P05";
  }
  if( enumNum EQ 2)
  {
    return "ACBARRMODATAFACTOR_P10";
  }
  if( enumNum EQ 3)
  {
    return "ACBARRMODATAFACTOR_P15";
  }
  if( enumNum EQ 4)
  {
    return "ACBARRMODATAFACTOR_P20";
  }
  if( enumNum EQ 5)
  {
    return "ACBARRMODATAFACTOR_P25";
  }
  if( enumNum EQ 6)
  {
    return "ACBARRMODATAFACTOR_P30";
  }
  if( enumNum EQ 7)
  {
    return "ACBARRMODATAFACTOR_P40";
  }
  if( enumNum EQ 8)
  {
    return "ACBARRMODATAFACTOR_P50";
  }
  if( enumNum EQ 9)
  {
    return "ACBARRMODATAFACTOR_P60";
  }
  if( enumNum EQ 10)
  {
    return "ACBARRMODATAFACTOR_P70";
  }
  if( enumNum EQ 11)
  {
    return "ACBARRMODATAFACTOR_P75";
  }
  if( enumNum EQ 12)
  {
    return "ACBARRMODATAFACTOR_P80";
  }
  if( enumNum EQ 13)
  {
    return "ACBARRMODATAFACTOR_P85";
  }
  if( enumNum EQ 14)
  {
    return "ACBARRMODATAFACTOR_P90";
  }
  if( enumNum EQ 15)
  {
    return "ACBARRMODATAFACTOR_P95";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* acBarrMoDataTimeStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "ACBARRMODATATIME_S4";
  }
  if( enumNum EQ 1)
  {
    return "ACBARRMODATATIME_S8";
  }
  if( enumNum EQ 2)
  {
    return "ACBARRMODATATIME_S16";
  }
  if( enumNum EQ 3)
  {
    return "ACBARRMODATATIME_S32";
  }
  if( enumNum EQ 4)
  {
    return "ACBARRMODATATIME_S64";
  }
  if( enumNum EQ 5)
  {
    return "ACBARRMODATATIME_S128";
  }
  if( enumNum EQ 6)
  {
    return "ACBARRMODATATIME_S256";
  }
  if( enumNum EQ 7)
  {
    return "ACBARRMODATATIME_S512";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* rsrpFilterCoefficientStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "RSRPFILTERCOEFFICIENT_FC0";
  }
  if( enumNum EQ 2)
  {
    return "RSRPFILTERCOEFFICIENT_FC1";
  }
  if( enumNum EQ 3)
  {
    return "RSRPFILTERCOEFFICIENT_FC2";
  }
  if( enumNum EQ 4)
  {
    return "RSRPFILTERCOEFFICIENT_FC3";
  }
  if( enumNum EQ 5)
  {
    return "RSRPFILTERCOEFFICIENT_FC4";
  }
  if( enumNum EQ 6)
  {
    return "RSRPFILTERCOEFFICIENT_FC5";
  }
  if( enumNum EQ 7)
  {
    return "RSRPFILTERCOEFFICIENT_FC6";
  }
  if( enumNum EQ 8)
  {
    return "RSRPFILTERCOEFFICIENT_FC7";
  }
  if( enumNum EQ 9)
  {
    return "RSRPFILTERCOEFFICIENT_FC8";
  }
  if( enumNum EQ 10)
  {
    return "RSRPFILTERCOEFFICIENT_FC9";
  }
  if( enumNum EQ 11)
  {
    return "RSRPFILTERCOEFFICIENT_FC11";
  }
  if( enumNum EQ 12)
  {
    return "RSRPFILTERCOEFFICIENT_FC13";
  }
  if( enumNum EQ 13)
  {
    return "RSRPFILTERCOEFFICIENT_FC15";
  }
  if( enumNum EQ 14)
  {
    return "RSRPFILTERCOEFFICIENT_FC17";
  }
  if( enumNum EQ 15)
  {
    return "RSRPFILTERCOEFFICIENT_FC19";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* ueResponseTimerStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "UERESPONSETIMER_MS50";
  }
  if( enumNum EQ 1)
  {
    return "UERESPONSETIMER_MS100";
  }
  if( enumNum EQ 2)
  {
    return "UERESPONSETIMER_MS200";
  }
  if( enumNum EQ 3)
  {
    return "UERESPONSETIMER_MS500";
  }
  if( enumNum EQ 4)
  {
    return "UERESPONSETIMER_MS1000";
  }
  if( enumNum EQ 5)
  {
    return "UERESPONSETIMER_MS2000";
  }
  if( enumNum EQ 6)
  {
    return "UERESPONSETIMER_MS5000";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* antInfoTxModeStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "ANTINFOTXMODE_TM1";
  }
  if( enumNum EQ 1)
  {
    return "ANTINFOTXMODE_TM2";
  }
  if( enumNum EQ 2)
  {
    return "ANTINFOTXMODE_TM3";
  }
  if( enumNum EQ 3)
  {
    return "ANTINFOTXMODE_TM4";
  }
  if( enumNum EQ 4)
  {
    return "ANTINFOTXMODE_TM5";
  }
  if( enumNum EQ 5)
  {
    return "ANTINFOTXMODE_TM6";
  }
  if( enumNum EQ 6)
  {
    return "ANTINFOTXMODE_TM7";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* cqiPeriodicityIdxStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "CQIPERIODICITYIDX_CP_2";
  }
  if( enumNum EQ 1)
  {
    return "CQIPERIODICITYIDX_CP_5";
  }
  if( enumNum EQ 2)
  {
    return "CQIPERIODICITYIDX_CP_10";
  }
  if( enumNum EQ 3)
  {
    return "CQIPERIODICITYIDX_CP_20";
  }
  if( enumNum EQ 4)
  {
    return "CQIPERIODICITYIDX_CP_40";
  }
  if( enumNum EQ 5)
  {
    return "CQIPERIODICITYIDX_CP_80";
  }
  if( enumNum EQ 6)
  {
    return "CQIPERIODICITYIDX_CP_160";
  }
  if( enumNum EQ 7)
  {
    return "CQIPERIODICITYIDX_CP_32";
  }
  if( enumNum EQ 8)
  {
    return "CQIPERIODICITYIDX_CP_64";
  }
  if( enumNum EQ 9)
  {
    return "CQIPERIODICITYIDX_CP_128";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* sriPeriodicityIdxStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SRIPERIODICITYIDX_SP_5";
  }
  if( enumNum EQ 1)
  {
    return "SRIPERIODICITYIDX_SP_10";
  }
  if( enumNum EQ 2)
  {
    return "SRIPERIODICITYIDX_SP_20";
  }
  if( enumNum EQ 3)
  {
    return "SRIPERIODICITYIDX_SP_40";
  }
  if( enumNum EQ 4)
  {
    return "SRIPERIODICITYIDX_SP_80";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* ueCqiBasedRlfDetectionTimerStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "UECQIBASEDRLFDETECTIONTIMER_MS0";
  }
  if( enumNum EQ 1)
  {
    return "UECQIBASEDRLFDETECTIONTIMER_MS500";
  }
  if( enumNum EQ 2)
  {
    return "UECQIBASEDRLFDETECTIONTIMER_MS1000";
  }
  if( enumNum EQ 3)
  {
    return "UECQIBASEDRLFDETECTIONTIMER_MS2000";
  }
  if( enumNum EQ 4)
  {
    return "UECQIBASEDRLFDETECTIONTIMER_MS5000";
  }
  if( enumNum EQ 5)
  {
    return "UECQIBASEDRLFDETECTIONTIMER_MS10000";
  }
  if( enumNum EQ 6)
  {
    return "UECQIBASEDRLFDETECTIONTIMER_MS20000";
  }
  if( enumNum EQ 7)
  {
    return "UECQIBASEDRLFDETECTIONTIMER_MS30000";
  }
  if( enumNum EQ 8)
  {
    return "UECQIBASEDRLFDETECTIONTIMER_MS40000";
  }
  if( enumNum EQ 9)
  {
    return "UECQIBASEDRLFDETECTIONTIMER_MS50000";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* schedulingAlgorithmStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SCHEDULINGALGORITHM_BASICSC1";
  }
  if( enumNum EQ 1)
  {
    return "SCHEDULINGALGORITHM_PROFFAIR";
  }
  if( enumNum EQ 2)
  {
    return "SCHEDULINGALGORITHM_ROUNDROBIN";
  }
  if( enumNum EQ 3)
  {
    return "SCHEDULINGALGORITHM_MAXCQI";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* maxUlHarqTxStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "MAXULHARQTX_N1";
  }
  if( enumNum EQ 1)
  {
    return "MAXULHARQTX_N2";
  }
  if( enumNum EQ 2)
  {
    return "MAXULHARQTX_N3";
  }
  if( enumNum EQ 3)
  {
    return "MAXULHARQTX_N4";
  }
  if( enumNum EQ 4)
  {
    return "MAXULHARQTX_N5";
  }
  if( enumNum EQ 5)
  {
    return "MAXULHARQTX_N6";
  }
  if( enumNum EQ 6)
  {
    return "MAXULHARQTX_N7";
  }
  if( enumNum EQ 7)
  {
    return "MAXULHARQTX_N8";
  }
  if( enumNum EQ 8)
  {
    return "MAXULHARQTX_N10";
  }
  if( enumNum EQ 9)
  {
    return "MAXULHARQTX_N12";
  }
  if( enumNum EQ 10)
  {
    return "MAXULHARQTX_N16";
  }
  if( enumNum EQ 11)
  {
    return "MAXULHARQTX_N20";
  }
  if( enumNum EQ 12)
  {
    return "MAXULHARQTX_N24";
  }
  if( enumNum EQ 13)
  {
    return "MAXULHARQTX_N28";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* periodicPhrTimerStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "PERIODICPHRTIMER_SF10";
  }
  if( enumNum EQ 1)
  {
    return "PERIODICPHRTIMER_SF20";
  }
  if( enumNum EQ 2)
  {
    return "PERIODICPHRTIMER_SF50";
  }
  if( enumNum EQ 3)
  {
    return "PERIODICPHRTIMER_SF100";
  }
  if( enumNum EQ 4)
  {
    return "PERIODICPHRTIMER_SF200";
  }
  if( enumNum EQ 5)
  {
    return "PERIODICPHRTIMER_SF500";
  }
  if( enumNum EQ 6)
  {
    return "PERIODICPHRTIMER_SF1000";
  }
  if( enumNum EQ 7)
  {
    return "PERIODICPHRTIMER_INFINITY";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* prohibitPhrTimerStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "PROHIBITPHRTIMER_SF0";
  }
  if( enumNum EQ 1)
  {
    return "PROHIBITPHRTIMER_SF10";
  }
  if( enumNum EQ 2)
  {
    return "PROHIBITPHRTIMER_SF20";
  }
  if( enumNum EQ 3)
  {
    return "PROHIBITPHRTIMER_SF50";
  }
  if( enumNum EQ 4)
  {
    return "PROHIBITPHRTIMER_SF100";
  }
  if( enumNum EQ 5)
  {
    return "PROHIBITPHRTIMER_SF200";
  }
  if( enumNum EQ 6)
  {
    return "PROHIBITPHRTIMER_SF500";
  }
  if( enumNum EQ 7)
  {
    return "PROHIBITPHRTIMER_SF1000";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* dlPathlossChangeStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "DLPATHLOSSCHANGE_DB1";
  }
  if( enumNum EQ 1)
  {
    return "DLPATHLOSSCHANGE_DB3";
  }
  if( enumNum EQ 2)
  {
    return "DLPATHLOSSCHANGE_DB6";
  }
  if( enumNum EQ 3)
  {
    return "DLPATHLOSSCHANGE_INFINITY";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* periodicBsrTimerStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "PERIODICBSRTIMER_SF5";
  }
  if( enumNum EQ 1)
  {
    return "PERIODICBSRTIMER_SF10";
  }
  if( enumNum EQ 2)
  {
    return "PERIODICBSRTIMER_SF16";
  }
  if( enumNum EQ 3)
  {
    return "PERIODICBSRTIMER_SF20";
  }
  if( enumNum EQ 4)
  {
    return "PERIODICBSRTIMER_SF32";
  }
  if( enumNum EQ 5)
  {
    return "PERIODICBSRTIMER_SF40";
  }
  if( enumNum EQ 6)
  {
    return "PERIODICBSRTIMER_SF64";
  }
  if( enumNum EQ 7)
  {
    return "PERIODICBSRTIMER_SF80";
  }
  if( enumNum EQ 8)
  {
    return "PERIODICBSRTIMER_SF128";
  }
  if( enumNum EQ 9)
  {
    return "PERIODICBSRTIMER_SF160";
  }
  if( enumNum EQ 10)
  {
    return "PERIODICBSRTIMER_SF320";
  }
  if( enumNum EQ 11)
  {
    return "PERIODICBSRTIMER_SF640";
  }
  if( enumNum EQ 12)
  {
    return "PERIODICBSRTIMER_SF1280";
  }
  if( enumNum EQ 13)
  {
    return "PERIODICBSRTIMER_SF2560";
  }
  if( enumNum EQ 14)
  {
    return "PERIODICBSRTIMER_INFINITY";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* retxBsrTimerStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "RETXBSRTIMER_SF320";
  }
  if( enumNum EQ 1)
  {
    return "RETXBSRTIMER_SF640";
  }
  if( enumNum EQ 2)
  {
    return "RETXBSRTIMER_SF1280";
  }
  if( enumNum EQ 3)
  {
    return "RETXBSRTIMER_SF2560";
  }
  if( enumNum EQ 4)
  {
    return "RETXBSRTIMER_SF5120";
  }
  if( enumNum EQ 5)
  {
    return "RETXBSRTIMER_SF10240";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* phichResourceStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "PHICHRESOURCE_ONESIXTH";
  }
  if( enumNum EQ 1)
  {
    return "PHICHRESOURCE_HALF";
  }
  if( enumNum EQ 2)
  {
    return "PHICHRESOURCE_ONE";
  }
  if( enumNum EQ 3)
  {
    return "PHICHRESOURCE_TWO";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* alphaStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "ALPHA_AL0";
  }
  if( enumNum EQ 1)
  {
    return "ALPHA_AL04";
  }
  if( enumNum EQ 2)
  {
    return "ALPHA_AL05";
  }
  if( enumNum EQ 3)
  {
    return "ALPHA_AL06";
  }
  if( enumNum EQ 4)
  {
    return "ALPHA_AL07";
  }
  if( enumNum EQ 5)
  {
    return "ALPHA_AL08";
  }
  if( enumNum EQ 6)
  {
    return "ALPHA_AL09";
  }
  if( enumNum EQ 7)
  {
    return "ALPHA_AL1";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* cellPhyPaStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "CELLPHYPA_DBMINUS6";
  }
  if( enumNum EQ 1)
  {
    return "CELLPHYPA_DBMINUS4DOT77";
  }
  if( enumNum EQ 2)
  {
    return "CELLPHYPA_DBMINUS3";
  }
  if( enumNum EQ 3)
  {
    return "CELLPHYPA_DBMINUS1DOT77";
  }
  if( enumNum EQ 4)
  {
    return "CELLPHYPA_DB0";
  }
  if( enumNum EQ 5)
  {
    return "CELLPHYPA_DB1";
  }
  if( enumNum EQ 6)
  {
    return "CELLPHYPA_DB2";
  }
  if( enumNum EQ 7)
  {
    return "CELLPHYPA_DB3";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* phichDurationStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "PHICHDURATION_NORMAL";
  }
  if( enumNum EQ 1)
  {
    return "PHICHDURATION_EXTENDED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* intraFreqReselectionStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "INTRAFREQRESELECTION_ALLOWED";
  }
  if( enumNum EQ 1)
  {
    return "INTRAFREQRESELECTION_NOTALLOWED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* sib2PeriodicityStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SIB2PERIODICITY_80MS";
  }
  if( enumNum EQ 1)
  {
    return "SIB2PERIODICITY_160MS";
  }
  if( enumNum EQ 2)
  {
    return "SIB2PERIODICITY_320MS";
  }
  if( enumNum EQ 3)
  {
    return "SIB2PERIODICITY_640MS";
  }
  if( enumNum EQ 4)
  {
    return "SIB2PERIODICITY_1280MS";
  }
  if( enumNum EQ 5)
  {
    return "SIB2PERIODICITY_2560MS";
  }
  if( enumNum EQ 6)
  {
    return "SIB2PERIODICITY_5120MS";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* sib3PeriodicityStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SIB3PERIODICITY_80MS";
  }
  if( enumNum EQ 1)
  {
    return "SIB3PERIODICITY_160MS";
  }
  if( enumNum EQ 2)
  {
    return "SIB3PERIODICITY_320MS";
  }
  if( enumNum EQ 3)
  {
    return "SIB3PERIODICITY_640MS";
  }
  if( enumNum EQ 4)
  {
    return "SIB3PERIODICITY_1280MS";
  }
  if( enumNum EQ 5)
  {
    return "SIB3PERIODICITY_2560MS";
  }
  if( enumNum EQ 6)
  {
    return "SIB3PERIODICITY_5120MS";
  }
  if( enumNum EQ 7)
  {
    return "SIB3PERIODICITY_DISABLED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* sib4PeriodicityStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SIB4PERIODICITY_80MS";
  }
  if( enumNum EQ 1)
  {
    return "SIB4PERIODICITY_160MS";
  }
  if( enumNum EQ 2)
  {
    return "SIB4PERIODICITY_320MS";
  }
  if( enumNum EQ 3)
  {
    return "SIB4PERIODICITY_640MS";
  }
  if( enumNum EQ 4)
  {
    return "SIB4PERIODICITY_1280MS";
  }
  if( enumNum EQ 5)
  {
    return "SIB4PERIODICITY_2560MS";
  }
  if( enumNum EQ 6)
  {
    return "SIB4PERIODICITY_5120MS";
  }
  if( enumNum EQ 7)
  {
    return "SIB4PERIODICITY_DISABLED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* siWindowLengthStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SIWINDOWLENGTH_1MS";
  }
  if( enumNum EQ 1)
  {
    return "SIWINDOWLENGTH_2MS";
  }
  if( enumNum EQ 2)
  {
    return "SIWINDOWLENGTH_5MS";
  }
  if( enumNum EQ 3)
  {
    return "SIWINDOWLENGTH_10MS";
  }
  if( enumNum EQ 4)
  {
    return "SIWINDOWLENGTH_15MS";
  }
  if( enumNum EQ 5)
  {
    return "SIWINDOWLENGTH_20MS";
  }
  if( enumNum EQ 6)
  {
    return "SIWINDOWLENGTH_40MS";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* numRaPreamblesStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "NUMRAPREAMBLES_N4";
  }
  if( enumNum EQ 1)
  {
    return "NUMRAPREAMBLES_N8";
  }
  if( enumNum EQ 2)
  {
    return "NUMRAPREAMBLES_N12";
  }
  if( enumNum EQ 3)
  {
    return "NUMRAPREAMBLES_N16";
  }
  if( enumNum EQ 4)
  {
    return "NUMRAPREAMBLES_N20";
  }
  if( enumNum EQ 5)
  {
    return "NUMRAPREAMBLES_N24";
  }
  if( enumNum EQ 6)
  {
    return "NUMRAPREAMBLES_N28";
  }
  if( enumNum EQ 7)
  {
    return "NUMRAPREAMBLES_N32";
  }
  if( enumNum EQ 8)
  {
    return "NUMRAPREAMBLES_N36";
  }
  if( enumNum EQ 9)
  {
    return "NUMRAPREAMBLES_N40";
  }
  if( enumNum EQ 10)
  {
    return "NUMRAPREAMBLES_N44";
  }
  if( enumNum EQ 11)
  {
    return "NUMRAPREAMBLES_N48";
  }
  if( enumNum EQ 12)
  {
    return "NUMRAPREAMBLES_N52";
  }
  if( enumNum EQ 13)
  {
    return "NUMRAPREAMBLES_N56";
  }
  if( enumNum EQ 14)
  {
    return "NUMRAPREAMBLES_N60";
  }
  if( enumNum EQ 15)
  {
    return "NUMRAPREAMBLES_N64";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* sizeRAPreamblesGroupAStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SIZERAPREAMBLESGROUPA_N4";
  }
  if( enumNum EQ 1)
  {
    return "SIZERAPREAMBLESGROUPA_N8";
  }
  if( enumNum EQ 2)
  {
    return "SIZERAPREAMBLESGROUPA_N12";
  }
  if( enumNum EQ 3)
  {
    return "SIZERAPREAMBLESGROUPA_N16";
  }
  if( enumNum EQ 4)
  {
    return "SIZERAPREAMBLESGROUPA_N20";
  }
  if( enumNum EQ 5)
  {
    return "SIZERAPREAMBLESGROUPA_N24";
  }
  if( enumNum EQ 6)
  {
    return "SIZERAPREAMBLESGROUPA_N28";
  }
  if( enumNum EQ 7)
  {
    return "SIZERAPREAMBLESGROUPA_N32";
  }
  if( enumNum EQ 8)
  {
    return "SIZERAPREAMBLESGROUPA_N36";
  }
  if( enumNum EQ 9)
  {
    return "SIZERAPREAMBLESGROUPA_N40";
  }
  if( enumNum EQ 10)
  {
    return "SIZERAPREAMBLESGROUPA_N44";
  }
  if( enumNum EQ 11)
  {
    return "SIZERAPREAMBLESGROUPA_N48";
  }
  if( enumNum EQ 12)
  {
    return "SIZERAPREAMBLESGROUPA_N52";
  }
  if( enumNum EQ 13)
  {
    return "SIZERAPREAMBLESGROUPA_N56";
  }
  if( enumNum EQ 14)
  {
    return "SIZERAPREAMBLESGROUPA_N60";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* messageSizeGroupAStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "MESSAGESIZEGROUPA_B56";
  }
  if( enumNum EQ 1)
  {
    return "MESSAGESIZEGROUPA_B144";
  }
  if( enumNum EQ 2)
  {
    return "MESSAGESIZEGROUPA_B208";
  }
  if( enumNum EQ 3)
  {
    return "MESSAGESIZEGROUPA_B256";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* messagePowerOffsetGroupBStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "MESSAGEPOWEROFFSETGROUPB_MINUSINFINITY";
  }
  if( enumNum EQ 1)
  {
    return "MESSAGEPOWEROFFSETGROUPB_DB0";
  }
  if( enumNum EQ 2)
  {
    return "MESSAGEPOWEROFFSETGROUPB_DB5";
  }
  if( enumNum EQ 3)
  {
    return "MESSAGEPOWEROFFSETGROUPB_DB8";
  }
  if( enumNum EQ 4)
  {
    return "MESSAGEPOWEROFFSETGROUPB_DB10";
  }
  if( enumNum EQ 5)
  {
    return "MESSAGEPOWEROFFSETGROUPB_DB12";
  }
  if( enumNum EQ 6)
  {
    return "MESSAGEPOWEROFFSETGROUPB_DB15";
  }
  if( enumNum EQ 7)
  {
    return "MESSAGEPOWEROFFSETGROUPB_DB18";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* powerRampingStepStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "POWERRAMPINGSTEP_DB0";
  }
  if( enumNum EQ 1)
  {
    return "POWERRAMPINGSTEP_DB2";
  }
  if( enumNum EQ 2)
  {
    return "POWERRAMPINGSTEP_DB4";
  }
  if( enumNum EQ 3)
  {
    return "POWERRAMPINGSTEP_DB6";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* preambleInitRcvdTgtPowerStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "PREAMBLEINITRCVDTGTPOWER_MINUS120DBM";
  }
  if( enumNum EQ 1)
  {
    return "PREAMBLEINITRCVDTGTPOWER_MINUS118DBM";
  }
  if( enumNum EQ 2)
  {
    return "PREAMBLEINITRCVDTGTPOWER_MINUS116DBM";
  }
  if( enumNum EQ 3)
  {
    return "PREAMBLEINITRCVDTGTPOWER_MINUS114DBM";
  }
  if( enumNum EQ 4)
  {
    return "PREAMBLEINITRCVDTGTPOWER_MINUS112DBM";
  }
  if( enumNum EQ 5)
  {
    return "PREAMBLEINITRCVDTGTPOWER_MINUS110DBM";
  }
  if( enumNum EQ 6)
  {
    return "PREAMBLEINITRCVDTGTPOWER_MINUS108DBM";
  }
  if( enumNum EQ 7)
  {
    return "PREAMBLEINITRCVDTGTPOWER_MINUS106DBM";
  }
  if( enumNum EQ 8)
  {
    return "PREAMBLEINITRCVDTGTPOWER_MINUS104DBM";
  }
  if( enumNum EQ 9)
  {
    return "PREAMBLEINITRCVDTGTPOWER_MINUS102DBM";
  }
  if( enumNum EQ 10)
  {
    return "PREAMBLEINITRCVDTGTPOWER_MINUS100DBM";
  }
  if( enumNum EQ 11)
  {
    return "PREAMBLEINITRCVDTGTPOWER_MINUS98DBM";
  }
  if( enumNum EQ 12)
  {
    return "PREAMBLEINITRCVDTGTPOWER_MINUS96DBM";
  }
  if( enumNum EQ 13)
  {
    return "PREAMBLEINITRCVDTGTPOWER_MINUS94DBM";
  }
  if( enumNum EQ 14)
  {
    return "PREAMBLEINITRCVDTGTPOWER_MINUS92DBM";
  }
  if( enumNum EQ 15)
  {
    return "PREAMBLEINITRCVDTGTPOWER_MINUS90DBM";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* preambleTransMaxStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "PREAMBLETRANSMAX_N3";
  }
  if( enumNum EQ 1)
  {
    return "PREAMBLETRANSMAX_N4";
  }
  if( enumNum EQ 2)
  {
    return "PREAMBLETRANSMAX_N5";
  }
  if( enumNum EQ 3)
  {
    return "PREAMBLETRANSMAX_N6";
  }
  if( enumNum EQ 4)
  {
    return "PREAMBLETRANSMAX_N7";
  }
  if( enumNum EQ 5)
  {
    return "PREAMBLETRANSMAX_N8";
  }
  if( enumNum EQ 6)
  {
    return "PREAMBLETRANSMAX_N10";
  }
  if( enumNum EQ 7)
  {
    return "PREAMBLETRANSMAX_N20";
  }
  if( enumNum EQ 8)
  {
    return "PREAMBLETRANSMAX_N50";
  }
  if( enumNum EQ 9)
  {
    return "PREAMBLETRANSMAX_N100";
  }
  if( enumNum EQ 10)
  {
    return "PREAMBLETRANSMAX_N200";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* raRspWindowSizeStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "RARSPWINDOWSIZE_SF2";
  }
  if( enumNum EQ 1)
  {
    return "RARSPWINDOWSIZE_SF3";
  }
  if( enumNum EQ 2)
  {
    return "RARSPWINDOWSIZE_SF4";
  }
  if( enumNum EQ 3)
  {
    return "RARSPWINDOWSIZE_SF5";
  }
  if( enumNum EQ 4)
  {
    return "RARSPWINDOWSIZE_SF6";
  }
  if( enumNum EQ 5)
  {
    return "RARSPWINDOWSIZE_SF7";
  }
  if( enumNum EQ 6)
  {
    return "RARSPWINDOWSIZE_SF8";
  }
  if( enumNum EQ 7)
  {
    return "RARSPWINDOWSIZE_SF10";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* macContResolutionTimerStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "MACCONTRESOLUTIONTIMER_SF8";
  }
  if( enumNum EQ 1)
  {
    return "MACCONTRESOLUTIONTIMER_SF16";
  }
  if( enumNum EQ 2)
  {
    return "MACCONTRESOLUTIONTIMER_SF24";
  }
  if( enumNum EQ 3)
  {
    return "MACCONTRESOLUTIONTIMER_SF32";
  }
  if( enumNum EQ 4)
  {
    return "MACCONTRESOLUTIONTIMER_SF40";
  }
  if( enumNum EQ 5)
  {
    return "MACCONTRESOLUTIONTIMER_SF48";
  }
  if( enumNum EQ 6)
  {
    return "MACCONTRESOLUTIONTIMER_SF56";
  }
  if( enumNum EQ 7)
  {
    return "MACCONTRESOLUTIONTIMER_SF64";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* modificationPeriodCoeffStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "MODIFICATIONPERIODCOEFF_N2";
  }
  if( enumNum EQ 1)
  {
    return "MODIFICATIONPERIODCOEFF_N4";
  }
  if( enumNum EQ 2)
  {
    return "MODIFICATIONPERIODCOEFF_N8";
  }
  if( enumNum EQ 3)
  {
    return "MODIFICATIONPERIODCOEFF_N16";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* defaultPagCycleStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "DEFAULTPAGCYCLE_RF32";
  }
  if( enumNum EQ 1)
  {
    return "DEFAULTPAGCYCLE_RF64";
  }
  if( enumNum EQ 2)
  {
    return "DEFAULTPAGCYCLE_RF128";
  }
  if( enumNum EQ 3)
  {
    return "DEFAULTPAGCYCLE_RF256";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* nBStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "NB_FOURT";
  }
  if( enumNum EQ 1)
  {
    return "NB_TWOT";
  }
  if( enumNum EQ 2)
  {
    return "NB_ONET";
  }
  if( enumNum EQ 3)
  {
    return "NB_HALFT";
  }
  if( enumNum EQ 4)
  {
    return "NB_QUARTERT";
  }
  if( enumNum EQ 5)
  {
    return "NB_ONEEIGHTHT";
  }
  if( enumNum EQ 6)
  {
    return "NB_ONESIXTEENTHT";
  }
  if( enumNum EQ 7)
  {
    return "NB_ONETHIRTYSECONDT";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* puschFreqHoppingModeStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "PUSCHFREQHOPPINGMODE_INTERSUBFRAME";
  }
  if( enumNum EQ 1)
  {
    return "PUSCHFREQHOPPINGMODE_INTRAANDINTERSUBFRAME";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* deltaPucchShiftStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "DELTAPUCCHSHIFT_DS1";
  }
  if( enumNum EQ 1)
  {
    return "DELTAPUCCHSHIFT_DS2";
  }
  if( enumNum EQ 2)
  {
    return "DELTAPUCCHSHIFT_DS3";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* srsBandwidthConfigStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SRSBANDWIDTHCONFIG_BW0";
  }
  if( enumNum EQ 1)
  {
    return "SRSBANDWIDTHCONFIG_BW1";
  }
  if( enumNum EQ 2)
  {
    return "SRSBANDWIDTHCONFIG_BW2";
  }
  if( enumNum EQ 3)
  {
    return "SRSBANDWIDTHCONFIG_BW3";
  }
  if( enumNum EQ 4)
  {
    return "SRSBANDWIDTHCONFIG_BW4";
  }
  if( enumNum EQ 5)
  {
    return "SRSBANDWIDTHCONFIG_BW5";
  }
  if( enumNum EQ 6)
  {
    return "SRSBANDWIDTHCONFIG_BW6";
  }
  if( enumNum EQ 7)
  {
    return "SRSBANDWIDTHCONFIG_BW7";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* srsSubfrmConfigStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SRSSUBFRMCONFIG_SC0";
  }
  if( enumNum EQ 1)
  {
    return "SRSSUBFRMCONFIG_SC1";
  }
  if( enumNum EQ 2)
  {
    return "SRSSUBFRMCONFIG_SC2";
  }
  if( enumNum EQ 3)
  {
    return "SRSSUBFRMCONFIG_SC3";
  }
  if( enumNum EQ 4)
  {
    return "SRSSUBFRMCONFIG_SC4";
  }
  if( enumNum EQ 5)
  {
    return "SRSSUBFRMCONFIG_SC5";
  }
  if( enumNum EQ 6)
  {
    return "SRSSUBFRMCONFIG_SC6";
  }
  if( enumNum EQ 7)
  {
    return "SRSSUBFRMCONFIG_SC7";
  }
  if( enumNum EQ 8)
  {
    return "SRSSUBFRMCONFIG_SC8";
  }
  if( enumNum EQ 9)
  {
    return "SRSSUBFRMCONFIG_SC9";
  }
  if( enumNum EQ 10)
  {
    return "SRSSUBFRMCONFIG_SC10";
  }
  if( enumNum EQ 11)
  {
    return "SRSSUBFRMCONFIG_SC11";
  }
  if( enumNum EQ 12)
  {
    return "SRSSUBFRMCONFIG_SC12";
  }
  if( enumNum EQ 13)
  {
    return "SRSSUBFRMCONFIG_SC13";
  }
  if( enumNum EQ 14)
  {
    return "SRSSUBFRMCONFIG_SC14";
  }
  if( enumNum EQ 15)
  {
    return "SRSSUBFRMCONFIG_SC15";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* deltaFPucchFormat1Str(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "DELTAFPUCCHFORMAT1_MINUS2DB";
  }
  if( enumNum EQ 1)
  {
    return "DELTAFPUCCHFORMAT1_0DB";
  }
  if( enumNum EQ 2)
  {
    return "DELTAFPUCCHFORMAT1_2DB";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* deltaFPucchFormat1bStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "DELTAFPUCCHFORMAT1B_1DB";
  }
  if( enumNum EQ 1)
  {
    return "DELTAFPUCCHFORMAT1B_3DB";
  }
  if( enumNum EQ 2)
  {
    return "DELTAFPUCCHFORMAT1B_5DB";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* deltaFPucchFormat2Str(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "DELTAFPUCCHFORMAT2_MINUS2DB";
  }
  if( enumNum EQ 1)
  {
    return "DELTAFPUCCHFORMAT2_0DB";
  }
  if( enumNum EQ 2)
  {
    return "DELTAFPUCCHFORMAT2_1DB";
  }
  if( enumNum EQ 3)
  {
    return "DELTAFPUCCHFORMAT2_2DB";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* deltaFPucchFormat2aStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "DELTAFPUCCHFORMAT2A_MINUS2DB";
  }
  if( enumNum EQ 1)
  {
    return "DELTAFPUCCHFORMAT2A_0DB";
  }
  if( enumNum EQ 2)
  {
    return "DELTAFPUCCHFORMAT2A_2DB";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* deltaFPucchFormat2bStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "DELTAFPUCCHFORMAT2B_MINUS2DB";
  }
  if( enumNum EQ 1)
  {
    return "DELTAFPUCCHFORMAT2B_0DB";
  }
  if( enumNum EQ 2)
  {
    return "DELTAFPUCCHFORMAT2B_2DB";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* t300Str(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "T300_MS100";
  }
  if( enumNum EQ 1)
  {
    return "T300_MS200";
  }
  if( enumNum EQ 2)
  {
    return "T300_MS300";
  }
  if( enumNum EQ 3)
  {
    return "T300_MS400";
  }
  if( enumNum EQ 4)
  {
    return "T300_MS600";
  }
  if( enumNum EQ 5)
  {
    return "T300_MS1000";
  }
  if( enumNum EQ 6)
  {
    return "T300_MS1500";
  }
  if( enumNum EQ 7)
  {
    return "T300_MS2000";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* t301Str(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "T301_MS100";
  }
  if( enumNum EQ 1)
  {
    return "T301_MS200";
  }
  if( enumNum EQ 2)
  {
    return "T301_MS300";
  }
  if( enumNum EQ 3)
  {
    return "T301_MS400";
  }
  if( enumNum EQ 4)
  {
    return "T301_MS600";
  }
  if( enumNum EQ 5)
  {
    return "T301_MS1000";
  }
  if( enumNum EQ 6)
  {
    return "T301_MS1500";
  }
  if( enumNum EQ 7)
  {
    return "T301_MS2000";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* t310Str(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "T310_MS0";
  }
  if( enumNum EQ 1)
  {
    return "T310_MS50";
  }
  if( enumNum EQ 2)
  {
    return "T310_MS100";
  }
  if( enumNum EQ 3)
  {
    return "T310_MS200";
  }
  if( enumNum EQ 4)
  {
    return "T310_MS500";
  }
  if( enumNum EQ 5)
  {
    return "T310_MS1000";
  }
  if( enumNum EQ 6)
  {
    return "T310_MS2000";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* n310Str(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "N310_N1";
  }
  if( enumNum EQ 1)
  {
    return "N310_N2";
  }
  if( enumNum EQ 2)
  {
    return "N310_N3";
  }
  if( enumNum EQ 3)
  {
    return "N310_N4";
  }
  if( enumNum EQ 4)
  {
    return "N310_N6";
  }
  if( enumNum EQ 5)
  {
    return "N310_N8";
  }
  if( enumNum EQ 6)
  {
    return "N310_N10";
  }
  if( enumNum EQ 7)
  {
    return "N310_N20";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* t311Str(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "T311_MS1000";
  }
  if( enumNum EQ 1)
  {
    return "T311_MS3000";
  }
  if( enumNum EQ 2)
  {
    return "T311_MS5000";
  }
  if( enumNum EQ 3)
  {
    return "T311_MS10000";
  }
  if( enumNum EQ 4)
  {
    return "T311_MS15000";
  }
  if( enumNum EQ 5)
  {
    return "T311_MS20000";
  }
  if( enumNum EQ 6)
  {
    return "T311_MS30000";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* n311Str(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "N311_N1";
  }
  if( enumNum EQ 1)
  {
    return "N311_N2";
  }
  if( enumNum EQ 2)
  {
    return "N311_N3";
  }
  if( enumNum EQ 3)
  {
    return "N311_N4";
  }
  if( enumNum EQ 4)
  {
    return "N311_N5";
  }
  if( enumNum EQ 5)
  {
    return "N311_N6";
  }
  if( enumNum EQ 6)
  {
    return "N311_N8";
  }
  if( enumNum EQ 7)
  {
    return "N311_N10";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* timeAlignmentTimerStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "TIMEALIGNMENTTIMER_SF500";
  }
  if( enumNum EQ 1)
  {
    return "TIMEALIGNMENTTIMER_SF750";
  }
  if( enumNum EQ 2)
  {
    return "TIMEALIGNMENTTIMER_SF1280";
  }
  if( enumNum EQ 3)
  {
    return "TIMEALIGNMENTTIMER_SF1920";
  }
  if( enumNum EQ 4)
  {
    return "TIMEALIGNMENTTIMER_SF2560";
  }
  if( enumNum EQ 5)
  {
    return "TIMEALIGNMENTTIMER_SF5120";
  }
  if( enumNum EQ 6)
  {
    return "TIMEALIGNMENTTIMER_SF10240";
  }
  if( enumNum EQ 7)
  {
    return "TIMEALIGNMENTTIMER_INFINITY";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* rlcModeStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "RLCMODE_AM";
  }
  if( enumNum EQ 1)
  {
    return "RLCMODE_UM";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* amRlcTPollRetransmitStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "AMRLCTPOLLRETRANSMIT_MS5";
  }
  if( enumNum EQ 1)
  {
    return "AMRLCTPOLLRETRANSMIT_MS10";
  }
  if( enumNum EQ 2)
  {
    return "AMRLCTPOLLRETRANSMIT_MS15";
  }
  if( enumNum EQ 3)
  {
    return "AMRLCTPOLLRETRANSMIT_MS20";
  }
  if( enumNum EQ 4)
  {
    return "AMRLCTPOLLRETRANSMIT_MS25";
  }
  if( enumNum EQ 5)
  {
    return "AMRLCTPOLLRETRANSMIT_MS30";
  }
  if( enumNum EQ 6)
  {
    return "AMRLCTPOLLRETRANSMIT_MS35";
  }
  if( enumNum EQ 7)
  {
    return "AMRLCTPOLLRETRANSMIT_MS40";
  }
  if( enumNum EQ 8)
  {
    return "AMRLCTPOLLRETRANSMIT_MS45";
  }
  if( enumNum EQ 9)
  {
    return "AMRLCTPOLLRETRANSMIT_MS50";
  }
  if( enumNum EQ 10)
  {
    return "AMRLCTPOLLRETRANSMIT_MS55";
  }
  if( enumNum EQ 11)
  {
    return "AMRLCTPOLLRETRANSMIT_MS60";
  }
  if( enumNum EQ 12)
  {
    return "AMRLCTPOLLRETRANSMIT_MS65";
  }
  if( enumNum EQ 13)
  {
    return "AMRLCTPOLLRETRANSMIT_MS70";
  }
  if( enumNum EQ 14)
  {
    return "AMRLCTPOLLRETRANSMIT_MS75";
  }
  if( enumNum EQ 15)
  {
    return "AMRLCTPOLLRETRANSMIT_MS80";
  }
  if( enumNum EQ 16)
  {
    return "AMRLCTPOLLRETRANSMIT_MS85";
  }
  if( enumNum EQ 17)
  {
    return "AMRLCTPOLLRETRANSMIT_MS90";
  }
  if( enumNum EQ 18)
  {
    return "AMRLCTPOLLRETRANSMIT_MS95";
  }
  if( enumNum EQ 19)
  {
    return "AMRLCTPOLLRETRANSMIT_MS100";
  }
  if( enumNum EQ 20)
  {
    return "AMRLCTPOLLRETRANSMIT_MS105";
  }
  if( enumNum EQ 21)
  {
    return "AMRLCTPOLLRETRANSMIT_MS110";
  }
  if( enumNum EQ 22)
  {
    return "AMRLCTPOLLRETRANSMIT_MS115";
  }
  if( enumNum EQ 23)
  {
    return "AMRLCTPOLLRETRANSMIT_MS120";
  }
  if( enumNum EQ 24)
  {
    return "AMRLCTPOLLRETRANSMIT_MS125";
  }
  if( enumNum EQ 25)
  {
    return "AMRLCTPOLLRETRANSMIT_MS130";
  }
  if( enumNum EQ 26)
  {
    return "AMRLCTPOLLRETRANSMIT_MS135";
  }
  if( enumNum EQ 27)
  {
    return "AMRLCTPOLLRETRANSMIT_MS140";
  }
  if( enumNum EQ 28)
  {
    return "AMRLCTPOLLRETRANSMIT_MS145";
  }
  if( enumNum EQ 29)
  {
    return "AMRLCTPOLLRETRANSMIT_MS150";
  }
  if( enumNum EQ 30)
  {
    return "AMRLCTPOLLRETRANSMIT_MS155";
  }
  if( enumNum EQ 31)
  {
    return "AMRLCTPOLLRETRANSMIT_MS160";
  }
  if( enumNum EQ 32)
  {
    return "AMRLCTPOLLRETRANSMIT_MS165";
  }
  if( enumNum EQ 33)
  {
    return "AMRLCTPOLLRETRANSMIT_MS170";
  }
  if( enumNum EQ 34)
  {
    return "AMRLCTPOLLRETRANSMIT_MS175";
  }
  if( enumNum EQ 35)
  {
    return "AMRLCTPOLLRETRANSMIT_MS180";
  }
  if( enumNum EQ 36)
  {
    return "AMRLCTPOLLRETRANSMIT_MS185";
  }
  if( enumNum EQ 37)
  {
    return "AMRLCTPOLLRETRANSMIT_MS190";
  }
  if( enumNum EQ 38)
  {
    return "AMRLCTPOLLRETRANSMIT_MS195";
  }
  if( enumNum EQ 39)
  {
    return "AMRLCTPOLLRETRANSMIT_MS200";
  }
  if( enumNum EQ 40)
  {
    return "AMRLCTPOLLRETRANSMIT_MS205";
  }
  if( enumNum EQ 41)
  {
    return "AMRLCTPOLLRETRANSMIT_MS210";
  }
  if( enumNum EQ 42)
  {
    return "AMRLCTPOLLRETRANSMIT_MS215";
  }
  if( enumNum EQ 43)
  {
    return "AMRLCTPOLLRETRANSMIT_MS220";
  }
  if( enumNum EQ 44)
  {
    return "AMRLCTPOLLRETRANSMIT_MS225";
  }
  if( enumNum EQ 45)
  {
    return "AMRLCTPOLLRETRANSMIT_MS230";
  }
  if( enumNum EQ 46)
  {
    return "AMRLCTPOLLRETRANSMIT_MS235";
  }
  if( enumNum EQ 47)
  {
    return "AMRLCTPOLLRETRANSMIT_MS240";
  }
  if( enumNum EQ 48)
  {
    return "AMRLCTPOLLRETRANSMIT_MS245";
  }
  if( enumNum EQ 49)
  {
    return "AMRLCTPOLLRETRANSMIT_MS250";
  }
  if( enumNum EQ 50)
  {
    return "AMRLCTPOLLRETRANSMIT_MS300";
  }
  if( enumNum EQ 51)
  {
    return "AMRLCTPOLLRETRANSMIT_MS350";
  }
  if( enumNum EQ 52)
  {
    return "AMRLCTPOLLRETRANSMIT_MS400";
  }
  if( enumNum EQ 53)
  {
    return "AMRLCTPOLLRETRANSMIT_MS450";
  }
  if( enumNum EQ 54)
  {
    return "AMRLCTPOLLRETRANSMIT_MS500";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* amRlcPollPDUStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "AMRLCPOLLPDU_P4";
  }
  if( enumNum EQ 1)
  {
    return "AMRLCPOLLPDU_P8";
  }
  if( enumNum EQ 2)
  {
    return "AMRLCPOLLPDU_P16";
  }
  if( enumNum EQ 3)
  {
    return "AMRLCPOLLPDU_P32";
  }
  if( enumNum EQ 4)
  {
    return "AMRLCPOLLPDU_P64";
  }
  if( enumNum EQ 5)
  {
    return "AMRLCPOLLPDU_P128";
  }
  if( enumNum EQ 6)
  {
    return "AMRLCPOLLPDU_P256";
  }
  if( enumNum EQ 7)
  {
    return "AMRLCPOLLPDU_PINFINITY";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* amRlcPollByteStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "AMRLCPOLLBYTE_KB25";
  }
  if( enumNum EQ 1)
  {
    return "AMRLCPOLLBYTE_KB50";
  }
  if( enumNum EQ 2)
  {
    return "AMRLCPOLLBYTE_KB75";
  }
  if( enumNum EQ 3)
  {
    return "AMRLCPOLLBYTE_KB100";
  }
  if( enumNum EQ 4)
  {
    return "AMRLCPOLLBYTE_KB125";
  }
  if( enumNum EQ 5)
  {
    return "AMRLCPOLLBYTE_KB250";
  }
  if( enumNum EQ 6)
  {
    return "AMRLCPOLLBYTE_KB375";
  }
  if( enumNum EQ 7)
  {
    return "AMRLCPOLLBYTE_KB500";
  }
  if( enumNum EQ 8)
  {
    return "AMRLCPOLLBYTE_KB750";
  }
  if( enumNum EQ 9)
  {
    return "AMRLCPOLLBYTE_KB1000";
  }
  if( enumNum EQ 10)
  {
    return "AMRLCPOLLBYTE_KB1250";
  }
  if( enumNum EQ 11)
  {
    return "AMRLCPOLLBYTE_KB1500";
  }
  if( enumNum EQ 12)
  {
    return "AMRLCPOLLBYTE_KB2000";
  }
  if( enumNum EQ 13)
  {
    return "AMRLCPOLLBYTE_KB3000";
  }
  if( enumNum EQ 14)
  {
    return "AMRLCPOLLBYTE_KBINFINITY";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* amRlcMaxRetxThresholdStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "AMRLCMAXRETXTHRESHOLD_T1";
  }
  if( enumNum EQ 1)
  {
    return "AMRLCMAXRETXTHRESHOLD_T2";
  }
  if( enumNum EQ 2)
  {
    return "AMRLCMAXRETXTHRESHOLD_T3";
  }
  if( enumNum EQ 3)
  {
    return "AMRLCMAXRETXTHRESHOLD_T4";
  }
  if( enumNum EQ 4)
  {
    return "AMRLCMAXRETXTHRESHOLD_T6";
  }
  if( enumNum EQ 5)
  {
    return "AMRLCMAXRETXTHRESHOLD_T8";
  }
  if( enumNum EQ 6)
  {
    return "AMRLCMAXRETXTHRESHOLD_T16";
  }
  if( enumNum EQ 7)
  {
    return "AMRLCMAXRETXTHRESHOLD_T32";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* amRlcTReorderingStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "AMRLCTREORDERING_MS0";
  }
  if( enumNum EQ 1)
  {
    return "AMRLCTREORDERING_MS5";
  }
  if( enumNum EQ 2)
  {
    return "AMRLCTREORDERING_MS10";
  }
  if( enumNum EQ 3)
  {
    return "AMRLCTREORDERING_MS15";
  }
  if( enumNum EQ 4)
  {
    return "AMRLCTREORDERING_MS20";
  }
  if( enumNum EQ 5)
  {
    return "AMRLCTREORDERING_MS25";
  }
  if( enumNum EQ 6)
  {
    return "AMRLCTREORDERING_MS30";
  }
  if( enumNum EQ 7)
  {
    return "AMRLCTREORDERING_MS35";
  }
  if( enumNum EQ 8)
  {
    return "AMRLCTREORDERING_MS40";
  }
  if( enumNum EQ 9)
  {
    return "AMRLCTREORDERING_MS45";
  }
  if( enumNum EQ 10)
  {
    return "AMRLCTREORDERING_MS50";
  }
  if( enumNum EQ 11)
  {
    return "AMRLCTREORDERING_MS55";
  }
  if( enumNum EQ 12)
  {
    return "AMRLCTREORDERING_MS60";
  }
  if( enumNum EQ 13)
  {
    return "AMRLCTREORDERING_MS65";
  }
  if( enumNum EQ 14)
  {
    return "AMRLCTREORDERING_MS70";
  }
  if( enumNum EQ 15)
  {
    return "AMRLCTREORDERING_MS75";
  }
  if( enumNum EQ 16)
  {
    return "AMRLCTREORDERING_MS80";
  }
  if( enumNum EQ 17)
  {
    return "AMRLCTREORDERING_MS85";
  }
  if( enumNum EQ 18)
  {
    return "AMRLCTREORDERING_MS90";
  }
  if( enumNum EQ 19)
  {
    return "AMRLCTREORDERING_MS95";
  }
  if( enumNum EQ 20)
  {
    return "AMRLCTREORDERING_MS100";
  }
  if( enumNum EQ 21)
  {
    return "AMRLCTREORDERING_MS110";
  }
  if( enumNum EQ 22)
  {
    return "AMRLCTREORDERING_MS120";
  }
  if( enumNum EQ 23)
  {
    return "AMRLCTREORDERING_MS130";
  }
  if( enumNum EQ 24)
  {
    return "AMRLCTREORDERING_MS140";
  }
  if( enumNum EQ 25)
  {
    return "AMRLCTREORDERING_MS150";
  }
  if( enumNum EQ 26)
  {
    return "AMRLCTREORDERING_MS160";
  }
  if( enumNum EQ 27)
  {
    return "AMRLCTREORDERING_MS170";
  }
  if( enumNum EQ 28)
  {
    return "AMRLCTREORDERING_MS180";
  }
  if( enumNum EQ 29)
  {
    return "AMRLCTREORDERING_MS190";
  }
  if( enumNum EQ 30)
  {
    return "AMRLCTREORDERING_MS200";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* amRlcTStatusProhibitStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "AMRLCTSTATUSPROHIBIT_MS0";
  }
  if( enumNum EQ 1)
  {
    return "AMRLCTSTATUSPROHIBIT_MS5";
  }
  if( enumNum EQ 2)
  {
    return "AMRLCTSTATUSPROHIBIT_MS10";
  }
  if( enumNum EQ 3)
  {
    return "AMRLCTSTATUSPROHIBIT_MS15";
  }
  if( enumNum EQ 4)
  {
    return "AMRLCTSTATUSPROHIBIT_MS20";
  }
  if( enumNum EQ 5)
  {
    return "AMRLCTSTATUSPROHIBIT_MS25";
  }
  if( enumNum EQ 6)
  {
    return "AMRLCTSTATUSPROHIBIT_MS30";
  }
  if( enumNum EQ 7)
  {
    return "AMRLCTSTATUSPROHIBIT_MS35";
  }
  if( enumNum EQ 8)
  {
    return "AMRLCTSTATUSPROHIBIT_MS40";
  }
  if( enumNum EQ 9)
  {
    return "AMRLCTSTATUSPROHIBIT_MS45";
  }
  if( enumNum EQ 10)
  {
    return "AMRLCTSTATUSPROHIBIT_MS50";
  }
  if( enumNum EQ 11)
  {
    return "AMRLCTSTATUSPROHIBIT_MS55";
  }
  if( enumNum EQ 12)
  {
    return "AMRLCTSTATUSPROHIBIT_MS60";
  }
  if( enumNum EQ 13)
  {
    return "AMRLCTSTATUSPROHIBIT_MS65";
  }
  if( enumNum EQ 14)
  {
    return "AMRLCTSTATUSPROHIBIT_MS70";
  }
  if( enumNum EQ 15)
  {
    return "AMRLCTSTATUSPROHIBIT_MS75";
  }
  if( enumNum EQ 16)
  {
    return "AMRLCTSTATUSPROHIBIT_MS80";
  }
  if( enumNum EQ 17)
  {
    return "AMRLCTSTATUSPROHIBIT_MS85";
  }
  if( enumNum EQ 18)
  {
    return "AMRLCTSTATUSPROHIBIT_MS90";
  }
  if( enumNum EQ 19)
  {
    return "AMRLCTSTATUSPROHIBIT_MS95";
  }
  if( enumNum EQ 20)
  {
    return "AMRLCTSTATUSPROHIBIT_MS100";
  }
  if( enumNum EQ 21)
  {
    return "AMRLCTSTATUSPROHIBIT_MS105";
  }
  if( enumNum EQ 22)
  {
    return "AMRLCTSTATUSPROHIBIT_MS110";
  }
  if( enumNum EQ 23)
  {
    return "AMRLCTSTATUSPROHIBIT_MS115";
  }
  if( enumNum EQ 24)
  {
    return "AMRLCTSTATUSPROHIBIT_MS120";
  }
  if( enumNum EQ 25)
  {
    return "AMRLCTSTATUSPROHIBIT_MS125";
  }
  if( enumNum EQ 26)
  {
    return "AMRLCTSTATUSPROHIBIT_MS130";
  }
  if( enumNum EQ 27)
  {
    return "AMRLCTSTATUSPROHIBIT_MS135";
  }
  if( enumNum EQ 28)
  {
    return "AMRLCTSTATUSPROHIBIT_MS140";
  }
  if( enumNum EQ 29)
  {
    return "AMRLCTSTATUSPROHIBIT_MS145";
  }
  if( enumNum EQ 30)
  {
    return "AMRLCTSTATUSPROHIBIT_MS150";
  }
  if( enumNum EQ 31)
  {
    return "AMRLCTSTATUSPROHIBIT_MS155";
  }
  if( enumNum EQ 32)
  {
    return "AMRLCTSTATUSPROHIBIT_MS160";
  }
  if( enumNum EQ 33)
  {
    return "AMRLCTSTATUSPROHIBIT_MS165";
  }
  if( enumNum EQ 34)
  {
    return "AMRLCTSTATUSPROHIBIT_MS170";
  }
  if( enumNum EQ 35)
  {
    return "AMRLCTSTATUSPROHIBIT_MS175";
  }
  if( enumNum EQ 36)
  {
    return "AMRLCTSTATUSPROHIBIT_MS180";
  }
  if( enumNum EQ 37)
  {
    return "AMRLCTSTATUSPROHIBIT_MS185";
  }
  if( enumNum EQ 38)
  {
    return "AMRLCTSTATUSPROHIBIT_MS190";
  }
  if( enumNum EQ 39)
  {
    return "AMRLCTSTATUSPROHIBIT_MS195";
  }
  if( enumNum EQ 40)
  {
    return "AMRLCTSTATUSPROHIBIT_MS200";
  }
  if( enumNum EQ 41)
  {
    return "AMRLCTSTATUSPROHIBIT_MS205";
  }
  if( enumNum EQ 42)
  {
    return "AMRLCTSTATUSPROHIBIT_MS210";
  }
  if( enumNum EQ 43)
  {
    return "AMRLCTSTATUSPROHIBIT_MS215";
  }
  if( enumNum EQ 44)
  {
    return "AMRLCTSTATUSPROHIBIT_MS220";
  }
  if( enumNum EQ 45)
  {
    return "AMRLCTSTATUSPROHIBIT_MS225";
  }
  if( enumNum EQ 46)
  {
    return "AMRLCTSTATUSPROHIBIT_MS230";
  }
  if( enumNum EQ 47)
  {
    return "AMRLCTSTATUSPROHIBIT_MS235";
  }
  if( enumNum EQ 48)
  {
    return "AMRLCTSTATUSPROHIBIT_MS240";
  }
  if( enumNum EQ 49)
  {
    return "AMRLCTSTATUSPROHIBIT_MS245";
  }
  if( enumNum EQ 50)
  {
    return "AMRLCTSTATUSPROHIBIT_MS250";
  }
  if( enumNum EQ 51)
  {
    return "AMRLCTSTATUSPROHIBIT_MS300";
  }
  if( enumNum EQ 52)
  {
    return "AMRLCTSTATUSPROHIBIT_MS350";
  }
  if( enumNum EQ 53)
  {
    return "AMRLCTSTATUSPROHIBIT_MS400";
  }
  if( enumNum EQ 54)
  {
    return "AMRLCTSTATUSPROHIBIT_MS450";
  }
  if( enumNum EQ 55)
  {
    return "AMRLCTSTATUSPROHIBIT_MS500";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* discardTimerStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "DISCARDTIMER_MS50";
  }
  if( enumNum EQ 1)
  {
    return "DISCARDTIMER_MS100";
  }
  if( enumNum EQ 2)
  {
    return "DISCARDTIMER_MS150";
  }
  if( enumNum EQ 3)
  {
    return "DISCARDTIMER_MS300";
  }
  if( enumNum EQ 4)
  {
    return "DISCARDTIMER_MS500";
  }
  if( enumNum EQ 5)
  {
    return "DISCARDTIMER_MS750";
  }
  if( enumNum EQ 6)
  {
    return "DISCARDTIMER_MS1500";
  }
  if( enumNum EQ 7)
  {
    return "DISCARDTIMER_INFINITY";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* drbPrioritisedBitRateStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "DRBPRIORITISEDBITRATE_KBPS0";
  }
  if( enumNum EQ 1)
  {
    return "DRBPRIORITISEDBITRATE_KBPS8";
  }
  if( enumNum EQ 2)
  {
    return "DRBPRIORITISEDBITRATE_KBPS16";
  }
  if( enumNum EQ 3)
  {
    return "DRBPRIORITISEDBITRATE_KBPS32";
  }
  if( enumNum EQ 4)
  {
    return "DRBPRIORITISEDBITRATE_KBPS64";
  }
  if( enumNum EQ 5)
  {
    return "DRBPRIORITISEDBITRATE_KBPS128";
  }
  if( enumNum EQ 6)
  {
    return "DRBPRIORITISEDBITRATE_KBPS256";
  }
  if( enumNum EQ 7)
  {
    return "DRBPRIORITISEDBITRATE_INFINITY";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* drbBucketSizeDurationStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "DRBBUCKETSIZEDURATION_MS50";
  }
  if( enumNum EQ 1)
  {
    return "DRBBUCKETSIZEDURATION_MS100";
  }
  if( enumNum EQ 2)
  {
    return "DRBBUCKETSIZEDURATION_MS150";
  }
  if( enumNum EQ 3)
  {
    return "DRBBUCKETSIZEDURATION_MS300";
  }
  if( enumNum EQ 4)
  {
    return "DRBBUCKETSIZEDURATION_MS500";
  }
  if( enumNum EQ 5)
  {
    return "DRBBUCKETSIZEDURATION_MS1000";
  }
  if( enumNum EQ 6)
  {
    return "DRBBUCKETSIZEDURATION_SPARE2";
  }
  if( enumNum EQ 7)
  {
    return "DRBBUCKETSIZEDURATION_SPARE1";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* sampleIntervalStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "SAMPLEINTERVAL_1MIN";
  }
  if( enumNum EQ 2)
  {
    return "SAMPLEINTERVAL_5MIN";
  }
  if( enumNum EQ 3)
  {
    return "SAMPLEINTERVAL_10MIN";
  }
  if( enumNum EQ 4)
  {
    return "SAMPLEINTERVAL_15MIN";
  }
  if( enumNum EQ 5)
  {
    return "SAMPLEINTERVAL_30MIN";
  }
  if( enumNum EQ 6)
  {
    return "SAMPLEINTERVAL_1H";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* pmFileCreationIntervalStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "PMFILECREATIONINTERVAL_1H";
  }
  if( enumNum EQ 2)
  {
    return "PMFILECREATIONINTERVAL_2H";
  }
  if( enumNum EQ 3)
  {
    return "PMFILECREATIONINTERVAL_4H";
  }
  if( enumNum EQ 4)
  {
    return "PMFILECREATIONINTERVAL_8H";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* timeToTriggerStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "TIMETOTRIGGER_MS0";
  }
  if( enumNum EQ 1)
  {
    return "TIMETOTRIGGER_MS40";
  }
  if( enumNum EQ 2)
  {
    return "TIMETOTRIGGER_MS64";
  }
  if( enumNum EQ 3)
  {
    return "TIMETOTRIGGER_MS80";
  }
  if( enumNum EQ 4)
  {
    return "TIMETOTRIGGER_MS100";
  }
  if( enumNum EQ 5)
  {
    return "TIMETOTRIGGER_MS128";
  }
  if( enumNum EQ 6)
  {
    return "TIMETOTRIGGER_MS160";
  }
  if( enumNum EQ 7)
  {
    return "TIMETOTRIGGER_MS256";
  }
  if( enumNum EQ 8)
  {
    return "TIMETOTRIGGER_MS320";
  }
  if( enumNum EQ 9)
  {
    return "TIMETOTRIGGER_MS480";
  }
  if( enumNum EQ 10)
  {
    return "TIMETOTRIGGER_MS512";
  }
  if( enumNum EQ 11)
  {
    return "TIMETOTRIGGER_MS640";
  }
  if( enumNum EQ 12)
  {
    return "TIMETOTRIGGER_MS1024";
  }
  if( enumNum EQ 13)
  {
    return "TIMETOTRIGGER_MS1280";
  }
  if( enumNum EQ 14)
  {
    return "TIMETOTRIGGER_MS2560";
  }
  if( enumNum EQ 15)
  {
    return "TIMETOTRIGGER_MS5120";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* triggerQuantityStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "TRIGGERQUANTITY_RSRP";
  }
  if( enumNum EQ 1)
  {
    return "TRIGGERQUANTITY_RSRQ";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* reportQuantityStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "REPORTQUANTITY_SAMEASTRIGGERQUANTITY";
  }
  if( enumNum EQ 1)
  {
    return "REPORTQUANTITY_BOTH";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* filterCoefficientRsrqStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "FILTERCOEFFICIENTRSRQ_FC0";
  }
  if( enumNum EQ 1)
  {
    return "FILTERCOEFFICIENTRSRQ_FC1";
  }
  if( enumNum EQ 2)
  {
    return "FILTERCOEFFICIENTRSRQ_FC2";
  }
  if( enumNum EQ 3)
  {
    return "FILTERCOEFFICIENTRSRQ_FC3";
  }
  if( enumNum EQ 4)
  {
    return "FILTERCOEFFICIENTRSRQ_FC4";
  }
  if( enumNum EQ 5)
  {
    return "FILTERCOEFFICIENTRSRQ_FC5";
  }
  if( enumNum EQ 6)
  {
    return "FILTERCOEFFICIENTRSRQ_FC6";
  }
  if( enumNum EQ 7)
  {
    return "FILTERCOEFFICIENTRSRQ_FC7";
  }
  if( enumNum EQ 8)
  {
    return "FILTERCOEFFICIENTRSRQ_FC8";
  }
  if( enumNum EQ 9)
  {
    return "FILTERCOEFFICIENTRSRQ_FC9";
  }
  if( enumNum EQ 10)
  {
    return "FILTERCOEFFICIENTRSRQ_FC11";
  }
  if( enumNum EQ 11)
  {
    return "FILTERCOEFFICIENTRSRQ_FC13";
  }
  if( enumNum EQ 12)
  {
    return "FILTERCOEFFICIENTRSRQ_FC15";
  }
  if( enumNum EQ 13)
  {
    return "FILTERCOEFFICIENTRSRQ_FC17";
  }
  if( enumNum EQ 14)
  {
    return "FILTERCOEFFICIENTRSRQ_FC19";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* filterCoefficientRsrpStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "FILTERCOEFFICIENTRSRP_FC0";
  }
  if( enumNum EQ 1)
  {
    return "FILTERCOEFFICIENTRSRP_FC1";
  }
  if( enumNum EQ 2)
  {
    return "FILTERCOEFFICIENTRSRP_FC2";
  }
  if( enumNum EQ 3)
  {
    return "FILTERCOEFFICIENTRSRP_FC3";
  }
  if( enumNum EQ 4)
  {
    return "FILTERCOEFFICIENTRSRP_FC4";
  }
  if( enumNum EQ 5)
  {
    return "FILTERCOEFFICIENTRSRP_FC5";
  }
  if( enumNum EQ 6)
  {
    return "FILTERCOEFFICIENTRSRP_FC6";
  }
  if( enumNum EQ 7)
  {
    return "FILTERCOEFFICIENTRSRP_FC7";
  }
  if( enumNum EQ 8)
  {
    return "FILTERCOEFFICIENTRSRP_FC8";
  }
  if( enumNum EQ 9)
  {
    return "FILTERCOEFFICIENTRSRP_FC9";
  }
  if( enumNum EQ 10)
  {
    return "FILTERCOEFFICIENTRSRP_FC11";
  }
  if( enumNum EQ 11)
  {
    return "FILTERCOEFFICIENTRSRP_FC13";
  }
  if( enumNum EQ 12)
  {
    return "FILTERCOEFFICIENTRSRP_FC15";
  }
  if( enumNum EQ 13)
  {
    return "FILTERCOEFFICIENTRSRP_FC17";
  }
  if( enumNum EQ 14)
  {
    return "FILTERCOEFFICIENTRSRP_FC19";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* reportIntervalStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "REPORTINTERVAL_MS120";
  }
  if( enumNum EQ 1)
  {
    return "REPORTINTERVAL_MS240";
  }
  if( enumNum EQ 2)
  {
    return "REPORTINTERVAL_MS480";
  }
  if( enumNum EQ 3)
  {
    return "REPORTINTERVAL_MS640";
  }
  if( enumNum EQ 4)
  {
    return "REPORTINTERVAL_MS1024";
  }
  if( enumNum EQ 5)
  {
    return "REPORTINTERVAL_MS2048";
  }
  if( enumNum EQ 6)
  {
    return "REPORTINTERVAL_MS5120";
  }
  if( enumNum EQ 7)
  {
    return "REPORTINTERVAL_MS10240";
  }
  if( enumNum EQ 8)
  {
    return "REPORTINTERVAL_MIN1";
  }
  if( enumNum EQ 9)
  {
    return "REPORTINTERVAL_MIN6";
  }
  if( enumNum EQ 10)
  {
    return "REPORTINTERVAL_MIN12";
  }
  if( enumNum EQ 11)
  {
    return "REPORTINTERVAL_MIN30";
  }
  if( enumNum EQ 12)
  {
    return "REPORTINTERVAL_MIN60";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* reportAmountStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "REPORTAMOUNT_R1";
  }
  if( enumNum EQ 1)
  {
    return "REPORTAMOUNT_R2";
  }
  if( enumNum EQ 2)
  {
    return "REPORTAMOUNT_R4";
  }
  if( enumNum EQ 3)
  {
    return "REPORTAMOUNT_R8";
  }
  if( enumNum EQ 4)
  {
    return "REPORTAMOUNT_R16";
  }
  if( enumNum EQ 5)
  {
    return "REPORTAMOUNT_R32";
  }
  if( enumNum EQ 6)
  {
    return "REPORTAMOUNT_R64";
  }
  if( enumNum EQ 7)
  {
    return "REPORTAMOUNT_INFINITY";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* supportedEventsStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SUPPORTEDEVENTS_A3";
  }
  if( enumNum EQ 1)
  {
    return "SUPPORTEDEVENTS_A5";
  }
  if( enumNum EQ 2)
  {
    return "SUPPORTEDEVENTS_A3A5";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* neighCellMeasBandwidthStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "NEIGHCELLMEASBANDWIDTH_5MHZ";
  }
  if( enumNum EQ 1)
  {
    return "NEIGHCELLMEASBANDWIDTH_10MHZ";
  }
  if( enumNum EQ 2)
  {
    return "NEIGHCELLMEASBANDWIDTH_20MHZ";
  }
  if( enumNum EQ 3)
  {
    return "NEIGHCELLMEASBANDWIDTH_SAMEASOWNCELL";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* cellIndividualOffsetStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "CELLINDIVIDUALOFFSET_DBMINUS24";
  }
  if( enumNum EQ 1)
  {
    return "CELLINDIVIDUALOFFSET_DBMINUS22";
  }
  if( enumNum EQ 2)
  {
    return "CELLINDIVIDUALOFFSET_DBMINUS20";
  }
  if( enumNum EQ 3)
  {
    return "CELLINDIVIDUALOFFSET_DBMINUS18";
  }
  if( enumNum EQ 4)
  {
    return "CELLINDIVIDUALOFFSET_DBMINUS16";
  }
  if( enumNum EQ 5)
  {
    return "CELLINDIVIDUALOFFSET_DBMINUS14";
  }
  if( enumNum EQ 6)
  {
    return "CELLINDIVIDUALOFFSET_DBMINUS12";
  }
  if( enumNum EQ 7)
  {
    return "CELLINDIVIDUALOFFSET_DBMINUS10";
  }
  if( enumNum EQ 8)
  {
    return "CELLINDIVIDUALOFFSET_DBMINUS8";
  }
  if( enumNum EQ 9)
  {
    return "CELLINDIVIDUALOFFSET_DBMINUS6";
  }
  if( enumNum EQ 10)
  {
    return "CELLINDIVIDUALOFFSET_DBMINUS5";
  }
  if( enumNum EQ 11)
  {
    return "CELLINDIVIDUALOFFSET_DBMINUS4";
  }
  if( enumNum EQ 12)
  {
    return "CELLINDIVIDUALOFFSET_DBMINUS3";
  }
  if( enumNum EQ 13)
  {
    return "CELLINDIVIDUALOFFSET_DBMINUS2";
  }
  if( enumNum EQ 14)
  {
    return "CELLINDIVIDUALOFFSET_DBMINUS1";
  }
  if( enumNum EQ 15)
  {
    return "CELLINDIVIDUALOFFSET_DB0";
  }
  if( enumNum EQ 16)
  {
    return "CELLINDIVIDUALOFFSET_DB1";
  }
  if( enumNum EQ 17)
  {
    return "CELLINDIVIDUALOFFSET_DB2";
  }
  if( enumNum EQ 18)
  {
    return "CELLINDIVIDUALOFFSET_DB3";
  }
  if( enumNum EQ 19)
  {
    return "CELLINDIVIDUALOFFSET_DB4";
  }
  if( enumNum EQ 20)
  {
    return "CELLINDIVIDUALOFFSET_DB5";
  }
  if( enumNum EQ 21)
  {
    return "CELLINDIVIDUALOFFSET_DB6";
  }
  if( enumNum EQ 22)
  {
    return "CELLINDIVIDUALOFFSET_DB8";
  }
  if( enumNum EQ 23)
  {
    return "CELLINDIVIDUALOFFSET_DB10";
  }
  if( enumNum EQ 24)
  {
    return "CELLINDIVIDUALOFFSET_DB12";
  }
  if( enumNum EQ 25)
  {
    return "CELLINDIVIDUALOFFSET_DB14";
  }
  if( enumNum EQ 26)
  {
    return "CELLINDIVIDUALOFFSET_DB16";
  }
  if( enumNum EQ 27)
  {
    return "CELLINDIVIDUALOFFSET_DB18";
  }
  if( enumNum EQ 28)
  {
    return "CELLINDIVIDUALOFFSET_DB20";
  }
  if( enumNum EQ 29)
  {
    return "CELLINDIVIDUALOFFSET_DB22";
  }
  if( enumNum EQ 30)
  {
    return "CELLINDIVIDUALOFFSET_DB24";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* intraFreqNeighCellQOffsetCellStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DBMINUS24";
  }
  if( enumNum EQ 1)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DBMINUS22";
  }
  if( enumNum EQ 2)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DBMINUS20";
  }
  if( enumNum EQ 3)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DBMINUS18";
  }
  if( enumNum EQ 4)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DBMINUS16";
  }
  if( enumNum EQ 5)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DBMINUS14";
  }
  if( enumNum EQ 6)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DBMINUS12";
  }
  if( enumNum EQ 7)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DBMINUS10";
  }
  if( enumNum EQ 8)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DBMINUS8";
  }
  if( enumNum EQ 9)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DBMINUS6";
  }
  if( enumNum EQ 10)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DBMINUS5";
  }
  if( enumNum EQ 11)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DBMINUS4";
  }
  if( enumNum EQ 12)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DBMINUS3";
  }
  if( enumNum EQ 13)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DBMINUS2";
  }
  if( enumNum EQ 14)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DBMINUS1";
  }
  if( enumNum EQ 15)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DB0";
  }
  if( enumNum EQ 16)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DB1";
  }
  if( enumNum EQ 17)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DB2";
  }
  if( enumNum EQ 18)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DB3";
  }
  if( enumNum EQ 19)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DB4";
  }
  if( enumNum EQ 20)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DB5";
  }
  if( enumNum EQ 21)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DB6";
  }
  if( enumNum EQ 22)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DB8";
  }
  if( enumNum EQ 23)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DB10";
  }
  if( enumNum EQ 24)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DB12";
  }
  if( enumNum EQ 25)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DB14";
  }
  if( enumNum EQ 26)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DB16";
  }
  if( enumNum EQ 27)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DB18";
  }
  if( enumNum EQ 28)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DB20";
  }
  if( enumNum EQ 29)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DB22";
  }
  if( enumNum EQ 30)
  {
    return "INTRAFREQNEIGHCELLQOFFSETCELL_DB24";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* qHystStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "QHYST_DB0";
  }
  if( enumNum EQ 1)
  {
    return "QHYST_DB1";
  }
  if( enumNum EQ 2)
  {
    return "QHYST_DB2";
  }
  if( enumNum EQ 3)
  {
    return "QHYST_DB3";
  }
  if( enumNum EQ 4)
  {
    return "QHYST_DB4";
  }
  if( enumNum EQ 5)
  {
    return "QHYST_DB5";
  }
  if( enumNum EQ 6)
  {
    return "QHYST_DB6";
  }
  if( enumNum EQ 7)
  {
    return "QHYST_DB8";
  }
  if( enumNum EQ 8)
  {
    return "QHYST_DB10";
  }
  if( enumNum EQ 9)
  {
    return "QHYST_DB12";
  }
  if( enumNum EQ 10)
  {
    return "QHYST_DB14";
  }
  if( enumNum EQ 11)
  {
    return "QHYST_DB16";
  }
  if( enumNum EQ 12)
  {
    return "QHYST_DB18";
  }
  if( enumNum EQ 13)
  {
    return "QHYST_DB20";
  }
  if( enumNum EQ 14)
  {
    return "QHYST_DB22";
  }
  if( enumNum EQ 15)
  {
    return "QHYST_DB24";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* allowMeasBwStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "ALLOWMEASBW_MBW6";
  }
  if( enumNum EQ 1)
  {
    return "ALLOWMEASBW_MBW15";
  }
  if( enumNum EQ 2)
  {
    return "ALLOWMEASBW_MBW25";
  }
  if( enumNum EQ 3)
  {
    return "ALLOWMEASBW_MBW50";
  }
  if( enumNum EQ 4)
  {
    return "ALLOWMEASBW_MBW75";
  }
  if( enumNum EQ 5)
  {
    return "ALLOWMEASBW_MBW100";
  }
  if( enumNum EQ 6)
  {
    return "ALLOWMEASBW_SAMEASOWNCELLDLBW";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* srsBandwidthStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SRSBANDWIDTH_BW0";
  }
  if( enumNum EQ 1)
  {
    return "SRSBANDWIDTH_BW1";
  }
  if( enumNum EQ 2)
  {
    return "SRSBANDWIDTH_BW2";
  }
  if( enumNum EQ 3)
  {
    return "SRSBANDWIDTH_BW3";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* srsHoppingBandWidthStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SRSHOPPINGBANDWIDTH_HBW0";
  }
  if( enumNum EQ 1)
  {
    return "SRSHOPPINGBANDWIDTH_HBW1";
  }
  if( enumNum EQ 2)
  {
    return "SRSHOPPINGBANDWIDTH_HBW2";
  }
  if( enumNum EQ 3)
  {
    return "SRSHOPPINGBANDWIDTH_HBW3";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* uuCipheringStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "UUCIPHERING_EEA0";
  }
  if( enumNum EQ 1)
  {
    return "UUCIPHERING_EEA1";
  }
  if( enumNum EQ 2)
  {
    return "UUCIPHERING_EEA2";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* uuIntegrityProtectionStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "UUINTEGRITYPROTECTION_RESERVED";
  }
  if( enumNum EQ 1)
  {
    return "UUINTEGRITYPROTECTION_EIA1";
  }
  if( enumNum EQ 2)
  {
    return "UUINTEGRITYPROTECTION_EIA2";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* ueEventMeasuresStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "UEEVENTMEASURES_LOGALLEVENTS";
  }
  if( enumNum EQ 2)
  {
    return "UEEVENTMEASURES_LOGFAILURE";
  }
  if( enumNum EQ 3)
  {
    return "UEEVENTMEASURES_LOGONLYPROCEDURE";
  }
  if( enumNum EQ 4)
  {
    return "UEEVENTMEASURES_LOGUES";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* traceDestinationStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "TRACEDESTINATION_COLLECTTOFILE";
  }
  if( enumNum EQ 2)
  {
    return "TRACEDESTINATION_RELAYTOREMOTEHOST";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* fileUploadCommandReceivedStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "FILEUPLOADCOMMANDRECEIVED_NONE";
  }
  if( enumNum EQ 2)
  {
    return "FILEUPLOADCOMMANDRECEIVED_UPLOAD";
  }
  if( enumNum EQ 3)
  {
    return "FILEUPLOADCOMMANDRECEIVED_UPLOADCANCEL";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* uploadStatusStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "UPLOADSTATUS_NOTINITIATED";
  }
  if( enumNum EQ 2)
  {
    return "UPLOADSTATUS_ONGOING";
  }
  if( enumNum EQ 3)
  {
    return "UPLOADSTATUS_SUCCESSFUL";
  }
  if( enumNum EQ 4)
  {
    return "UPLOADSTATUS_FAILED";
  }
  if( enumNum EQ 5)
  {
    return "UPLOADSTATUS_CANCELLED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* fileDownloadCommandReceivedStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "FILEDOWNLOADCOMMANDRECEIVED_INSTANTDOWNLOAD";
  }
  if( enumNum EQ 2)
  {
    return "FILEDOWNLOADCOMMANDRECEIVED_SCHEDULEADOWNLOAD";
  }
  if( enumNum EQ 3)
  {
    return "FILEDOWNLOADCOMMANDRECEIVED_MODIFYSCHEDULEDDOWNLOAD";
  }
  if( enumNum EQ 4)
  {
    return "FILEDOWNLOADCOMMANDRECEIVED_CANCELDOWNLOAD";
  }
  if( enumNum EQ 5)
  {
    return "FILEDOWNLOADCOMMANDRECEIVED_NONE";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* downloadStatusStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "DOWNLOADSTATUS_NOTINITIATED";
  }
  if( enumNum EQ 2)
  {
    return "DOWNLOADSTATUS_ONGOING";
  }
  if( enumNum EQ 3)
  {
    return "DOWNLOADSTATUS_SUCCESSFUL";
  }
  if( enumNum EQ 4)
  {
    return "DOWNLOADSTATUS_FAILED";
  }
  if( enumNum EQ 5)
  {
    return "DOWNLOADSTATUS_CANCELLED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* swStatusStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "SWSTATUS_NOTINITIATED";
  }
  if( enumNum EQ 2)
  {
    return "SWSTATUS_INPROGRESS";
  }
  if( enumNum EQ 3)
  {
    return "SWSTATUS_SUCCESSFUL";
  }
  if( enumNum EQ 4)
  {
    return "SWSTATUS_FAILED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* componentTypeStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "COMPONENTTYPE_BASEBAND";
  }
  if( enumNum EQ 2)
  {
    return "COMPONENTTYPE_RADIOHEAD";
  }
  if( enumNum EQ 3)
  {
    return "COMPONENTTYPE_ETHERNET";
  }
  if( enumNum EQ 4)
  {
    return "COMPONENTTYPE_TEMPSENSOR";
  }
  if( enumNum EQ 5)
  {
    return "COMPONENTTYPE_CPRI";
  }
  if( enumNum EQ 6)
  {
    return "COMPONENTTYPE_GPS";
  }
  if( enumNum EQ 7)
  {
    return "COMPONENTTYPE_CLOCK";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* componentOperationalStateStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "COMPONENTOPERATIONALSTATE_UP";
  }
  if( enumNum EQ 2)
  {
    return "COMPONENTOPERATIONALSTATE_DOWN";
  }
  if( enumNum EQ 3)
  {
    return "COMPONENTOPERATIONALSTATE_UNKNOWN";
  }
  if( enumNum EQ 4)
  {
    return "COMPONENTOPERATIONALSTATE_NOTMONITORED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* connectedToMMEStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "CONNECTEDTOMME_NOTCONNECTED";
  }
  if( enumNum EQ 2)
  {
    return "CONNECTEDTOMME_CONNECTED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* adminStateStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "ADMINSTATE_LOCKED";
  }
  if( enumNum EQ 2)
  {
    return "ADMINSTATE_UNLOCKED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* operationalStateStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "OPERATIONALSTATE_DISABLED";
  }
  if( enumNum EQ 2)
  {
    return "OPERATIONALSTATE_ENABLED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* loadStateStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "LOADSTATE_LOWLOAD";
  }
  if( enumNum EQ 2)
  {
    return "LOADSTATE_NORMALLOAD";
  }
  if( enumNum EQ 3)
  {
    return "LOADSTATE_OVERLOADED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* dateAndTimeSourceStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "DATEANDTIMESOURCE_NOSOURCEAVAILABLE";
  }
  if( enumNum EQ 2)
  {
    return "DATEANDTIMESOURCE_GPS";
  }
  if( enumNum EQ 3)
  {
    return "DATEANDTIMESOURCE_NTP";
  }
  if( enumNum EQ 4)
  {
    return "DATEANDTIMESOURCE_CONFIGURED";
  }
  if( enumNum EQ 5)
  {
    return "DATEANDTIMESOURCE_OTHER";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* restartReasonStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "RESTARTREASON_OPERATORCMD";
  }
  if( enumNum EQ 2)
  {
    return "RESTARTREASON_CRASH";
  }
  if( enumNum EQ 3)
  {
    return "RESTARTREASON_NORESPONSETOHEALTHCHECK";
  }
  if( enumNum EQ 4)
  {
    return "RESTARTREASON_UNKOWN";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* c1IQMapStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "C1IQMAP_0";
  }
  if( enumNum EQ 2)
  {
    return "C1IQMAP_1";
  }
  if( enumNum EQ 3)
  {
    return "C1IQMAP_3";
  }
  if( enumNum EQ 4)
  {
    return "C1IQMAP_5";
  }
  if( enumNum EQ 5)
  {
    return "C1IQMAP_10";
  }
  if( enumNum EQ 6)
  {
    return "C1IQMAP_15";
  }
  if( enumNum EQ 7)
  {
    return "C1IQMAP_20";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* c2IQMapStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "C2IQMAP_0";
  }
  if( enumNum EQ 2)
  {
    return "C2IQMAP_1";
  }
  if( enumNum EQ 3)
  {
    return "C2IQMAP_3";
  }
  if( enumNum EQ 4)
  {
    return "C2IQMAP_5";
  }
  if( enumNum EQ 5)
  {
    return "C2IQMAP_10";
  }
  if( enumNum EQ 6)
  {
    return "C2IQMAP_15";
  }
  if( enumNum EQ 7)
  {
    return "C2IQMAP_20";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* c3IQMapStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "C3IQMAP_0";
  }
  if( enumNum EQ 2)
  {
    return "C3IQMAP_1";
  }
  if( enumNum EQ 3)
  {
    return "C3IQMAP_3";
  }
  if( enumNum EQ 4)
  {
    return "C3IQMAP_5";
  }
  if( enumNum EQ 5)
  {
    return "C3IQMAP_10";
  }
  if( enumNum EQ 6)
  {
    return "C3IQMAP_15";
  }
  if( enumNum EQ 7)
  {
    return "C3IQMAP_20";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* c4IQMapStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "C4IQMAP_0";
  }
  if( enumNum EQ 2)
  {
    return "C4IQMAP_1";
  }
  if( enumNum EQ 3)
  {
    return "C4IQMAP_3";
  }
  if( enumNum EQ 4)
  {
    return "C4IQMAP_5";
  }
  if( enumNum EQ 5)
  {
    return "C4IQMAP_10";
  }
  if( enumNum EQ 6)
  {
    return "C4IQMAP_15";
  }
  if( enumNum EQ 7)
  {
    return "C4IQMAP_20";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* mappingOf1FAStatusStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "MAPPINGOF1FASTATUS_0";
  }
  if( enumNum EQ 1)
  {
    return "MAPPINGOF1FASTATUS_C1";
  }
  if( enumNum EQ 2)
  {
    return "MAPPINGOF1FASTATUS_C2";
  }
  if( enumNum EQ 3)
  {
    return "MAPPINGOF1FASTATUS_C3";
  }
  if( enumNum EQ 4)
  {
    return "MAPPINGOF1FASTATUS_C4";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* mappingOf2FAStatusStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "MAPPINGOF2FASTATUS_0";
  }
  if( enumNum EQ 1)
  {
    return "MAPPINGOF2FASTATUS_C1";
  }
  if( enumNum EQ 2)
  {
    return "MAPPINGOF2FASTATUS_C2";
  }
  if( enumNum EQ 3)
  {
    return "MAPPINGOF2FASTATUS_C3";
  }
  if( enumNum EQ 4)
  {
    return "MAPPINGOF2FASTATUS_C4";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* mappingOf3FAStatusStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "MAPPINGOF3FASTATUS_0";
  }
  if( enumNum EQ 1)
  {
    return "MAPPINGOF3FASTATUS_C1";
  }
  if( enumNum EQ 2)
  {
    return "MAPPINGOF3FASTATUS_C2";
  }
  if( enumNum EQ 3)
  {
    return "MAPPINGOF3FASTATUS_C3";
  }
  if( enumNum EQ 4)
  {
    return "MAPPINGOF3FASTATUS_C4";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* mappingOf4FAStatusStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "MAPPINGOF4FASTATUS_0";
  }
  if( enumNum EQ 1)
  {
    return "MAPPINGOF4FASTATUS_C1";
  }
  if( enumNum EQ 2)
  {
    return "MAPPINGOF4FASTATUS_C2";
  }
  if( enumNum EQ 3)
  {
    return "MAPPINGOF4FASTATUS_C3";
  }
  if( enumNum EQ 4)
  {
    return "MAPPINGOF4FASTATUS_C4";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* fa1ChannelBandwidthStatusStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "FA1CHANNELBANDWIDTHSTATUS_0";
  }
  if( enumNum EQ 2)
  {
    return "FA1CHANNELBANDWIDTHSTATUS_1";
  }
  if( enumNum EQ 3)
  {
    return "FA1CHANNELBANDWIDTHSTATUS_3";
  }
  if( enumNum EQ 4)
  {
    return "FA1CHANNELBANDWIDTHSTATUS_5";
  }
  if( enumNum EQ 5)
  {
    return "FA1CHANNELBANDWIDTHSTATUS_10";
  }
  if( enumNum EQ 6)
  {
    return "FA1CHANNELBANDWIDTHSTATUS_15";
  }
  if( enumNum EQ 7)
  {
    return "FA1CHANNELBANDWIDTHSTATUS_20";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* fa2ChannelBandwidthStatusStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "FA2CHANNELBANDWIDTHSTATUS_0";
  }
  if( enumNum EQ 2)
  {
    return "FA2CHANNELBANDWIDTHSTATUS_1";
  }
  if( enumNum EQ 3)
  {
    return "FA2CHANNELBANDWIDTHSTATUS_3";
  }
  if( enumNum EQ 4)
  {
    return "FA2CHANNELBANDWIDTHSTATUS_5";
  }
  if( enumNum EQ 5)
  {
    return "FA2CHANNELBANDWIDTHSTATUS_10";
  }
  if( enumNum EQ 6)
  {
    return "FA2CHANNELBANDWIDTHSTATUS_15";
  }
  if( enumNum EQ 7)
  {
    return "FA2CHANNELBANDWIDTHSTATUS_20";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* fa3ChannelBandwidthStatusStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "FA3CHANNELBANDWIDTHSTATUS_0";
  }
  if( enumNum EQ 2)
  {
    return "FA3CHANNELBANDWIDTHSTATUS_1";
  }
  if( enumNum EQ 3)
  {
    return "FA3CHANNELBANDWIDTHSTATUS_3";
  }
  if( enumNum EQ 4)
  {
    return "FA3CHANNELBANDWIDTHSTATUS_5";
  }
  if( enumNum EQ 5)
  {
    return "FA3CHANNELBANDWIDTHSTATUS_10";
  }
  if( enumNum EQ 6)
  {
    return "FA3CHANNELBANDWIDTHSTATUS_15";
  }
  if( enumNum EQ 7)
  {
    return "FA3CHANNELBANDWIDTHSTATUS_20";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* fa4ChannelBandwidthStatusStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "FA4CHANNELBANDWIDTHSTATUS_0";
  }
  if( enumNum EQ 2)
  {
    return "FA4CHANNELBANDWIDTHSTATUS_1";
  }
  if( enumNum EQ 3)
  {
    return "FA4CHANNELBANDWIDTHSTATUS_3";
  }
  if( enumNum EQ 4)
  {
    return "FA4CHANNELBANDWIDTHSTATUS_5";
  }
  if( enumNum EQ 5)
  {
    return "FA4CHANNELBANDWIDTHSTATUS_10";
  }
  if( enumNum EQ 6)
  {
    return "FA4CHANNELBANDWIDTHSTATUS_15";
  }
  if( enumNum EQ 7)
  {
    return "FA4CHANNELBANDWIDTHSTATUS_20";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* fileStatusStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "FILESTATUS_NOTAPPLICABLE";
  }
  if( enumNum EQ 2)
  {
    return "FILESTATUS_FILEOPENED";
  }
  if( enumNum EQ 3)
  {
    return "FILESTATUS_FILECLOSED";
  }
  if( enumNum EQ 4)
  {
    return "FILESTATUS_FILEDELETED";
  }
  if( enumNum EQ 5)
  {
    return "FILESTATUS_FILEDOWNLOAD";
  }
  if( enumNum EQ 6)
  {
    return "FILESTATUS_FILEUPLOAD";
  }
  if( enumNum EQ 7)
  {
    return "FILESTATUS_FILEUPLOADANDDELETE";
  }
  if( enumNum EQ 8)
  {
    return "FILESTATUS_FILEAUTOUPLOADANDDELETE";
  }
  if( enumNum EQ 9)
  {
    return "FILESTATUS_FILESAVED2PERSISTENTMEMORY";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* ueCategoryStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "UECATEGORY_CAT1";
  }
  if( enumNum EQ 2)
  {
    return "UECATEGORY_CAT2";
  }
  if( enumNum EQ 3)
  {
    return "UECATEGORY_CAT3";
  }
  if( enumNum EQ 4)
  {
    return "UECATEGORY_CAT4";
  }
  if( enumNum EQ 5)
  {
    return "UECATEGORY_CAT5";
  }
  if( enumNum EQ 6)
  {
    return "UECATEGORY_UNKNOWN";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* ueAccessStratumReleaseStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "UEACCESSSTRATUMRELEASE_REL8";
  }
  if( enumNum EQ 1)
  {
    return "UEACCESSSTRATUMRELEASE_SPARE7";
  }
  if( enumNum EQ 2)
  {
    return "UEACCESSSTRATUMRELEASE_SPARE6";
  }
  if( enumNum EQ 3)
  {
    return "UEACCESSSTRATUMRELEASE_SPARE5";
  }
  if( enumNum EQ 4)
  {
    return "UEACCESSSTRATUMRELEASE_SPARE4";
  }
  if( enumNum EQ 5)
  {
    return "UEACCESSSTRATUMRELEASE_SPARE3";
  }
  if( enumNum EQ 6)
  {
    return "UEACCESSSTRATUMRELEASE_SPARE2";
  }
  if( enumNum EQ 7)
  {
    return "UEACCESSSTRATUMRELEASE_SPARE1";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* activeIntProtAlgoStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "ACTIVEINTPROTALGO_128EIA1";
  }
  if( enumNum EQ 1)
  {
    return "ACTIVEINTPROTALGO_128EIA2";
  }
  if( enumNum EQ 2)
  {
    return "ACTIVEINTPROTALGO_DISABLE";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* activeCiphAlgoStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "ACTIVECIPHALGO_EEA0";
  }
  if( enumNum EQ 1)
  {
    return "ACTIVECIPHALGO_128EEA1";
  }
  if( enumNum EQ 2)
  {
    return "ACTIVECIPHALGO_128EEA2";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* eventTypeStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "EVENTTYPE_UNKNOWN";
  }
  if( enumNum EQ 1)
  {
    return "EVENTTYPE_RRCCONNREQ";
  }
  if( enumNum EQ 2)
  {
    return "EVENTTYPE_RRCCONNSETUP";
  }
  if( enumNum EQ 3)
  {
    return "EVENTTYPE_RRCCONNREJ";
  }
  if( enumNum EQ 4)
  {
    return "EVENTTYPE_RRCCONNSETUPCOMPL";
  }
  if( enumNum EQ 5)
  {
    return "EVENTTYPE_INITIALUEMSG";
  }
  if( enumNum EQ 6)
  {
    return "EVENTTYPE_UECAPABILITYENQ";
  }
  if( enumNum EQ 7)
  {
    return "EVENTTYPE_UECAPABILITYINFO";
  }
  if( enumNum EQ 8)
  {
    return "EVENTTYPE_UECAPABILITYINFOIND";
  }
  if( enumNum EQ 9)
  {
    return "EVENTTYPE_RRCCONNRECONFIG";
  }
  if( enumNum EQ 10)
  {
    return "EVENTTYPE_RRCCONNRECONFCOMPL";
  }
  if( enumNum EQ 11)
  {
    return "EVENTTYPE_SECMODECOMMAND";
  }
  if( enumNum EQ 12)
  {
    return "EVENTTYPE_SECMODECOMPL";
  }
  if( enumNum EQ 13)
  {
    return "EVENTTYPE_SECMODEFAIL";
  }
  if( enumNum EQ 14)
  {
    return "EVENTTYPE_INITCONTEXTSETUPREQ";
  }
  if( enumNum EQ 15)
  {
    return "EVENTTYPE_INITCONTEXTSETUPRESP";
  }
  if( enumNum EQ 16)
  {
    return "EVENTTYPE_INITCONTEXTSETUPFAIL";
  }
  if( enumNum EQ 17)
  {
    return "EVENTTYPE_RRCCONNRELEASE";
  }
  if( enumNum EQ 18)
  {
    return "EVENTTYPE_RRCREESTABREQ";
  }
  if( enumNum EQ 19)
  {
    return "EVENTTYPE_RRCREESTABRES";
  }
  if( enumNum EQ 20)
  {
    return "EVENTTYPE_RRCREESTABREJ";
  }
  if( enumNum EQ 21)
  {
    return "EVENTTYPE_RRCREESTABCOM";
  }
  if( enumNum EQ 22)
  {
    return "EVENTTYPE_RRCCOUNTERCHECKREQ";
  }
  if( enumNum EQ 23)
  {
    return "EVENTTYPE_RRCCOUNTERCHECKRES";
  }
  if( enumNum EQ 24)
  {
    return "EVENTTYPE_ULINFTRANSFER";
  }
  if( enumNum EQ 25)
  {
    return "EVENTTYPE_ULNASTRANSPORTMSG";
  }
  if( enumNum EQ 26)
  {
    return "EVENTTYPE_DLINFTRANSFER";
  }
  if( enumNum EQ 27)
  {
    return "EVENTTYPE_DLNASTRANSPORTMSG";
  }
  if( enumNum EQ 28)
  {
    return "EVENTTYPE_LOCATIONREPORTCTRL";
  }
  if( enumNum EQ 29)
  {
    return "EVENTTYPE_LOCATIONREPORTFAILUREIND";
  }
  if( enumNum EQ 30)
  {
    return "EVENTTYPE_LOCATIONREPORTRES";
  }
  if( enumNum EQ 31)
  {
    return "EVENTTYPE_TRACESTART";
  }
  if( enumNum EQ 32)
  {
    return "EVENTTYPE_TRACEFAILUREIND";
  }
  if( enumNum EQ 33)
  {
    return "EVENTTYPE_DEACTIVATETRACE";
  }
  if( enumNum EQ 34)
  {
    return "EVENTTYPE_DEACTIVATETRACEFAILUREIND";
  }
  if( enumNum EQ 35)
  {
    return "EVENTTYPE_OUTHOTRIGGERED";
  }
  if( enumNum EQ 36)
  {
    return "EVENTTYPE_OUTHOCOMPLETE";
  }
  if( enumNum EQ 37)
  {
    return "EVENTTYPE_OUTHOFAIL";
  }
  if( enumNum EQ 38)
  {
    return "EVENTTYPE_INHOREQUEST";
  }
  if( enumNum EQ 39)
  {
    return "EVENTTYPE_INHOCOMPLETE";
  }
  if( enumNum EQ 40)
  {
    return "EVENTTYPE_INHOFAIL";
  }
  if( enumNum EQ 41)
  {
    return "EVENTTYPE_HONOTIFY";
  }
  if( enumNum EQ 42)
  {
    return "EVENTTYPE_PATHSWITCHREQ";
  }
  if( enumNum EQ 43)
  {
    return "EVENTTYPE_PATHSWITCHRES";
  }
  if( enumNum EQ 44)
  {
    return "EVENTTYPE_PATHSWITCHFAIL";
  }
  if( enumNum EQ 45)
  {
    return "EVENTTYPE_HOCANCEL";
  }
  if( enumNum EQ 46)
  {
    return "EVENTTYPE_HOCANCELACK";
  }
  if( enumNum EQ 47)
  {
    return "EVENTTYPE_RABSETUPREQ";
  }
  if( enumNum EQ 48)
  {
    return "EVENTTYPE_RABSETUPCOMPL";
  }
  if( enumNum EQ 49)
  {
    return "EVENTTYPE_RABSETUPFAIL";
  }
  if( enumNum EQ 50)
  {
    return "EVENTTYPE_RABMODREQ";
  }
  if( enumNum EQ 51)
  {
    return "EVENTTYPE_RABMODRES";
  }
  if( enumNum EQ 52)
  {
    return "EVENTTYPE_RABMODFAIL";
  }
  if( enumNum EQ 53)
  {
    return "EVENTTYPE_RABRELREQ";
  }
  if( enumNum EQ 54)
  {
    return "EVENTTYPE_RABRELCOMPL";
  }
  if( enumNum EQ 55)
  {
    return "EVENTTYPE_RABRELIND";
  }
  if( enumNum EQ 56)
  {
    return "EVENTTYPE_UECONTEXTRELREQ";
  }
  if( enumNum EQ 57)
  {
    return "EVENTTYPE_UECONTEXTRELCMD";
  }
  if( enumNum EQ 58)
  {
    return "EVENTTYPE_UECONTEXTRELCOM";
  }
  if( enumNum EQ 59)
  {
    return "EVENTTYPE_UECONTEXTMODREQ";
  }
  if( enumNum EQ 60)
  {
    return "EVENTTYPE_UECONTEXTMODRESP";
  }
  if( enumNum EQ 61)
  {
    return "EVENTTYPE_UECONTEXTMODFAIL";
  }
  if( enumNum EQ 62)
  {
    return "EVENTTYPE_RLFAILURE";
  }
  if( enumNum EQ 63)
  {
    return "EVENTTYPE_RLRECOVERY";
  }
  if( enumNum EQ 64)
  {
    return "EVENTTYPE_RRCBNDREQ";
  }
  if( enumNum EQ 65)
  {
    return "EVENTTYPE_RRCCELLCFG";
  }
  if( enumNum EQ 66)
  {
    return "EVENTTYPE_SCHBNDREQ";
  }
  if( enumNum EQ 67)
  {
    return "EVENTTYPE_SCHCELLCONFIG";
  }
  if( enumNum EQ 68)
  {
    return "EVENTTYPE_SCHLCHCONFIG";
  }
  if( enumNum EQ 69)
  {
    return "EVENTTYPE_SCHLCGCONFIG";
  }
  if( enumNum EQ 70)
  {
    return "EVENTTYPE_SCHUECONFIG";
  }
  if( enumNum EQ 71)
  {
    return "EVENTTYPE_SCHUERECONFIG";
  }
  if( enumNum EQ 72)
  {
    return "EVENTTYPE_SCHUEDELETE";
  }
  if( enumNum EQ 73)
  {
    return "EVENTTYPE_SCHSICONFIG";
  }
  if( enumNum EQ 74)
  {
    return "EVENTTYPE_PHYBNDREQ";
  }
  if( enumNum EQ 75)
  {
    return "EVENTTYPE_PHYUECONFIG";
  }
  if( enumNum EQ 76)
  {
    return "EVENTTYPE_PHYCELLCONFIG";
  }
  if( enumNum EQ 77)
  {
    return "EVENTTYPE_PHYUEDELETE";
  }
  if( enumNum EQ 78)
  {
    return "EVENTTYPE_UDHBNDREQ";
  }
  if( enumNum EQ 79)
  {
    return "EVENTTYPE_UDHADDRABREQ";
  }
  if( enumNum EQ 80)
  {
    return "EVENTTYPE_UDHDELRABREQ";
  }
  if( enumNum EQ 81)
  {
    return "EVENTTYPE_UDHDELUEREQ";
  }
  if( enumNum EQ 82)
  {
    return "EVENTTYPE_RRCUECAPDECODEREQ";
  }
  if( enumNum EQ 83)
  {
    return "EVENTTYPE_S1BNDREQ";
  }
  if( enumNum EQ 84)
  {
    return "EVENTTYPE_DLNASNONDELIVERYIND";
  }
  if( enumNum EQ 85)
  {
    return "EVENTTYPE_UEDELETED";
  }
  if( enumNum EQ 86)
  {
    return "EVENTTYPE_INTERNAL";
  }
  if( enumNum EQ 87)
  {
    return "EVENTTYPE_OPSTOPRELEASE";
  }
  if( enumNum EQ 88)
  {
    return "EVENTTYPE_RRCLICFGCFM";
  }
  if( enumNum EQ 89)
  {
    return "EVENTTYPE_RRCERRIND";
  }
  if( enumNum EQ 90)
  {
    return "EVENTTYPE_UERESTMREXP";
  }
  if( enumNum EQ 91)
  {
    return "EVENTTYPE_SRB1TMREXP";
  }
  if( enumNum EQ 92)
  {
    return "EVENTTYPE_RRCCONNRELL2ACKTMREXP";
  }
  if( enumNum EQ 93)
  {
    return "EVENTTYPE_UECTXRELTMREXP";
  }
  if( enumNum EQ 94)
  {
    return "EVENTTYPE_UECRNTIRETENTMREXP";
  }
  if( enumNum EQ 95)
  {
    return "EVENTTYPE_RRCENCCFM";
  }
  if( enumNum EQ 96)
  {
    return "EVENTTYPE_RRCDECCFM";
  }
  if( enumNum EQ 97)
  {
    return "EVENTTYPE_EGTERRIND";
  }
  if( enumNum EQ 98)
  {
    return "EVENTTYPE_UDHERABADDCFM";
  }
  if( enumNum EQ 99)
  {
    return "EVENTTYPE_UDHUEINACTVIND";
  }
  if( enumNum EQ 100)
  {
    return "EVENTTYPE_UDHERABDELCFM";
  }
  if( enumNum EQ 101)
  {
    return "EVENTTYPE_UECTFCFGCFM";
  }
  if( enumNum EQ 102)
  {
    return "EVENTTYPE_UDHULINACTVIND";
  }
  if( enumNum EQ 103)
  {
    return "EVENTTYPE_RRCCOUNTCFM";
  }
  if( enumNum EQ 104)
  {
    return "EVENTTYPE_UECOUNTERCHECKRSP";
  }
  if( enumNum EQ 105)
  {
    return "EVENTTYPE_S1APHOCMD";
  }
  if( enumNum EQ 106)
  {
    return "EVENTTYPE_S1APHOPREPFAILED";
  }
  if( enumNum EQ 107)
  {
    return "EVENTTYPE_S1APENCDECCFM";
  }
  if( enumNum EQ 108)
  {
    return "EVENTTYPE_MMERELINDTMREXP";
  }
  if( enumNum EQ 109)
  {
    return "EVENTTYPE_UESCHCFGCFM";
  }
  if( enumNum EQ 110)
  {
    return "EVENTTYPE_UENHUDATACFM";
  }
  if( enumNum EQ 111)
  {
    return "EVENTTYPE_S1CLEAR";
  }
  if( enumNum EQ 112)
  {
    return "EVENTTYPE_S1APERRIND";
  }
  if( enumNum EQ 113)
  {
    return "EVENTTYPE_EGTPTUNNELMGMTCFM";
  }
  if( enumNum EQ 114)
  {
    return "EVENTTYPE_UDHDELUECFM";
  }
  if( enumNum EQ 115)
  {
    return "EVENTTYPE_PAGING";
  }
  if( enumNum EQ 116)
  {
    return "EVENTTYPE_INTCONFTMREXP";
  }
  if( enumNum EQ 117)
  {
    return "EVENTTYPE_RRCCONNESTASUCC";
  }
  if( enumNum EQ 118)
  {
    return "EVENTTYPE_INITIALCTXSETUPSUCC";
  }
  if( enumNum EQ 119)
  {
    return "EVENTTYPE_SECURITYSUCC";
  }
  if( enumNum EQ 120)
  {
    return "EVENTTYPE_UECAPABILITYSUCC";
  }
  if( enumNum EQ 121)
  {
    return "EVENTTYPE_RRCRECONFIGSUCC";
  }
  if( enumNum EQ 122)
  {
    return "EVENTTYPE_UECTXRELEASESUCC";
  }
  if( enumNum EQ 123)
  {
    return "EVENTTYPE_UEENTERED";
  }
  if( enumNum EQ 124)
  {
    return "EVENTTYPE_MACL2MEAS";
  }
  if( enumNum EQ 125)
  {
    return "EVENTTYPE_RLCL2MEAS";
  }
  if( enumNum EQ 126)
  {
    return "EVENTTYPE_PDCPL2MEAS";
  }
  if( enumNum EQ 127)
  {
    return "EVENTTYPE_S1APLOCALERRIND";
  }
  if( enumNum EQ 128)
  {
    return "EVENTTYPE_RRCLOCALERRIND";
  }
  if( enumNum EQ 129)
  {
    return "EVENTTYPE_PHYKDFREQ";
  }
  if( enumNum EQ 130)
  {
    return "EVENTTYPE_PHYKDFRSP";
  }
  if( enumNum EQ 131)
  {
    return "EVENTTYPE_RESERVED1";
  }
  if( enumNum EQ 132)
  {
    return "EVENTTYPE_RESERVED2";
  }
  if( enumNum EQ 133)
  {
    return "EVENTTYPE_RESERVED3";
  }
  if( enumNum EQ 134)
  {
    return "EVENTTYPE_RESERVED4";
  }
  if( enumNum EQ 135)
  {
    return "EVENTTYPE_RESERVED5";
  }
  if( enumNum EQ 136)
  {
    return "EVENTTYPE_OTHER";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* rabReleaseCauseStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "RABRELEASECAUSE_NOTAPPLICABLE";
  }
  if( enumNum EQ 1)
  {
    return "RABRELEASECAUSE_UNSPECIFIED";
  }
  if( enumNum EQ 2)
  {
    return "RABRELEASECAUSE_UECONTEXTRELEASED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* alarmTypeStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "ALARMTYPE_OTHER";
  }
  if( enumNum EQ 2)
  {
    return "ALARMTYPE_COMMUNICATIONSALARM";
  }
  if( enumNum EQ 3)
  {
    return "ALARMTYPE_QUALITYOFSERVICEALARM";
  }
  if( enumNum EQ 4)
  {
    return "ALARMTYPE_PROCESSINGERRORALARM";
  }
  if( enumNum EQ 5)
  {
    return "ALARMTYPE_EQUIPMENTALARM";
  }
  if( enumNum EQ 6)
  {
    return "ALARMTYPE_ENVIRONMENTALALARM";
  }
  if( enumNum EQ 7)
  {
    return "ALARMTYPE_INTEGRITYVIOLATION";
  }
  if( enumNum EQ 8)
  {
    return "ALARMTYPE_OPERATIONVIOLATION";
  }
  if( enumNum EQ 9)
  {
    return "ALARMTYPE_PHYSICALVIOLATION";
  }
  if( enumNum EQ 10)
  {
    return "ALARMTYPE_SECURITYSERVICEORMECHANISMVIOLATION";
  }
  if( enumNum EQ 11)
  {
    return "ALARMTYPE_TIMEDOMAINVIOLATION";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* alarmSeverityStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "ALARMSEVERITY_CLEARED";
  }
  if( enumNum EQ 2)
  {
    return "ALARMSEVERITY_INDETERMINATE";
  }
  if( enumNum EQ 3)
  {
    return "ALARMSEVERITY_CRITICAL";
  }
  if( enumNum EQ 4)
  {
    return "ALARMSEVERITY_MAJOR";
  }
  if( enumNum EQ 5)
  {
    return "ALARMSEVERITY_MINOR";
  }
  if( enumNum EQ 6)
  {
    return "ALARMSEVERITY_WARNING";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* alarmNotificationTypeStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "ALARMNOTIFICATIONTYPE_OCCURRED";
  }
  if( enumNum EQ 2)
  {
    return "ALARMNOTIFICATIONTYPE_CLEARED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* locationSourceStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "LOCATIONSOURCE_CONFIGUREDBYOAM";
  }
  if( enumNum EQ 2)
  {
    return "LOCATIONSOURCE_GPS";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* ifTypeStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "IFTYPE_OTHER";
  }
  if( enumNum EQ 2)
  {
    return "IFTYPE_REGULAR1822";
  }
  if( enumNum EQ 3)
  {
    return "IFTYPE_HDH1822";
  }
  if( enumNum EQ 4)
  {
    return "IFTYPE_DDNX25";
  }
  if( enumNum EQ 5)
  {
    return "IFTYPE_RFC877X25";
  }
  if( enumNum EQ 6)
  {
    return "IFTYPE_ETHERNETCSMACD";
  }
  if( enumNum EQ 7)
  {
    return "IFTYPE_ISO88023CSMACD";
  }
  if( enumNum EQ 8)
  {
    return "IFTYPE_ISO88024TOKENBUS";
  }
  if( enumNum EQ 9)
  {
    return "IFTYPE_ISO88025TOKENRING";
  }
  if( enumNum EQ 10)
  {
    return "IFTYPE_ISO88026MAN";
  }
  if( enumNum EQ 11)
  {
    return "IFTYPE_STARLAN";
  }
  if( enumNum EQ 12)
  {
    return "IFTYPE_PROTEON10MBIT";
  }
  if( enumNum EQ 13)
  {
    return "IFTYPE_PROTEON80MBIT";
  }
  if( enumNum EQ 14)
  {
    return "IFTYPE_HYPERCHANNEL";
  }
  if( enumNum EQ 15)
  {
    return "IFTYPE_FDDI";
  }
  if( enumNum EQ 16)
  {
    return "IFTYPE_LAPB";
  }
  if( enumNum EQ 17)
  {
    return "IFTYPE_SDLC";
  }
  if( enumNum EQ 18)
  {
    return "IFTYPE_DS1";
  }
  if( enumNum EQ 19)
  {
    return "IFTYPE_E1";
  }
  if( enumNum EQ 20)
  {
    return "IFTYPE_BASICISDN";
  }
  if( enumNum EQ 21)
  {
    return "IFTYPE_PRIMARYISDN";
  }
  if( enumNum EQ 22)
  {
    return "IFTYPE_PROPPOINTTOPOINTSERIAL";
  }
  if( enumNum EQ 23)
  {
    return "IFTYPE_PPP";
  }
  if( enumNum EQ 24)
  {
    return "IFTYPE_SOFTWARELOOPBACK";
  }
  if( enumNum EQ 25)
  {
    return "IFTYPE_EON";
  }
  if( enumNum EQ 26)
  {
    return "IFTYPE_ETHERNET3MBIT";
  }
  if( enumNum EQ 27)
  {
    return "IFTYPE_NSIP";
  }
  if( enumNum EQ 28)
  {
    return "IFTYPE_SLIP";
  }
  if( enumNum EQ 29)
  {
    return "IFTYPE_ULTRA";
  }
  if( enumNum EQ 30)
  {
    return "IFTYPE_DS3";
  }
  if( enumNum EQ 31)
  {
    return "IFTYPE_SIP";
  }
  if( enumNum EQ 32)
  {
    return "IFTYPE_FRAMERELAY";
  }
  if( enumNum EQ 33)
  {
    return "IFTYPE_RS232";
  }
  if( enumNum EQ 34)
  {
    return "IFTYPE_PARA";
  }
  if( enumNum EQ 35)
  {
    return "IFTYPE_ARCNET";
  }
  if( enumNum EQ 36)
  {
    return "IFTYPE_ARCNETPLUS";
  }
  if( enumNum EQ 37)
  {
    return "IFTYPE_ATM";
  }
  if( enumNum EQ 38)
  {
    return "IFTYPE_MIOX25";
  }
  if( enumNum EQ 39)
  {
    return "IFTYPE_SONET";
  }
  if( enumNum EQ 40)
  {
    return "IFTYPE_X25PLE";
  }
  if( enumNum EQ 41)
  {
    return "IFTYPE_ISO88022LLC";
  }
  if( enumNum EQ 42)
  {
    return "IFTYPE_LOCALTALK";
  }
  if( enumNum EQ 43)
  {
    return "IFTYPE_SMDSDXI";
  }
  if( enumNum EQ 44)
  {
    return "IFTYPE_FRAMERELAYSERVICE";
  }
  if( enumNum EQ 45)
  {
    return "IFTYPE_V35";
  }
  if( enumNum EQ 46)
  {
    return "IFTYPE_HSSI";
  }
  if( enumNum EQ 47)
  {
    return "IFTYPE_HIPPI";
  }
  if( enumNum EQ 48)
  {
    return "IFTYPE_MODEM";
  }
  if( enumNum EQ 49)
  {
    return "IFTYPE_AAL5";
  }
  if( enumNum EQ 50)
  {
    return "IFTYPE_SONETPATH";
  }
  if( enumNum EQ 51)
  {
    return "IFTYPE_SONETVT";
  }
  if( enumNum EQ 52)
  {
    return "IFTYPE_SMDSICIP";
  }
  if( enumNum EQ 53)
  {
    return "IFTYPE_PROPVIRTUAL";
  }
  if( enumNum EQ 54)
  {
    return "IFTYPE_PROPMULTIPLEXOR";
  }
  if( enumNum EQ 55)
  {
    return "IFTYPE_IEEE80212";
  }
  if( enumNum EQ 56)
  {
    return "IFTYPE_FIBRECHANNEL";
  }
  if( enumNum EQ 57)
  {
    return "IFTYPE_HIPPIINTERFACE";
  }
  if( enumNum EQ 58)
  {
    return "IFTYPE_FRAMERELAYINTERCONNECT";
  }
  if( enumNum EQ 59)
  {
    return "IFTYPE_AFLANE8023";
  }
  if( enumNum EQ 60)
  {
    return "IFTYPE_AFLANE8025";
  }
  if( enumNum EQ 61)
  {
    return "IFTYPE_CCTEMUL";
  }
  if( enumNum EQ 62)
  {
    return "IFTYPE_FASTETHER";
  }
  if( enumNum EQ 63)
  {
    return "IFTYPE_ISDN";
  }
  if( enumNum EQ 64)
  {
    return "IFTYPE_V11";
  }
  if( enumNum EQ 65)
  {
    return "IFTYPE_V36";
  }
  if( enumNum EQ 66)
  {
    return "IFTYPE_G703AT64K";
  }
  if( enumNum EQ 67)
  {
    return "IFTYPE_G703AT2MB";
  }
  if( enumNum EQ 68)
  {
    return "IFTYPE_QLLC";
  }
  if( enumNum EQ 69)
  {
    return "IFTYPE_FASTETHERFX";
  }
  if( enumNum EQ 70)
  {
    return "IFTYPE_CHANNEL";
  }
  if( enumNum EQ 71)
  {
    return "IFTYPE_IEEE80211";
  }
  if( enumNum EQ 72)
  {
    return "IFTYPE_IBM370PARCHAN";
  }
  if( enumNum EQ 73)
  {
    return "IFTYPE_ESCON";
  }
  if( enumNum EQ 74)
  {
    return "IFTYPE_DLSW";
  }
  if( enumNum EQ 75)
  {
    return "IFTYPE_ISDNS";
  }
  if( enumNum EQ 76)
  {
    return "IFTYPE_ISDNU";
  }
  if( enumNum EQ 77)
  {
    return "IFTYPE_LAPD";
  }
  if( enumNum EQ 78)
  {
    return "IFTYPE_IPSWITCH";
  }
  if( enumNum EQ 79)
  {
    return "IFTYPE_RSRB";
  }
  if( enumNum EQ 80)
  {
    return "IFTYPE_ATMLOGICAL";
  }
  if( enumNum EQ 81)
  {
    return "IFTYPE_DS0";
  }
  if( enumNum EQ 82)
  {
    return "IFTYPE_DS0BUNDLE";
  }
  if( enumNum EQ 83)
  {
    return "IFTYPE_BSC";
  }
  if( enumNum EQ 84)
  {
    return "IFTYPE_ASYNC";
  }
  if( enumNum EQ 85)
  {
    return "IFTYPE_CNR";
  }
  if( enumNum EQ 86)
  {
    return "IFTYPE_ISO88025DTR";
  }
  if( enumNum EQ 87)
  {
    return "IFTYPE_EPLRS";
  }
  if( enumNum EQ 88)
  {
    return "IFTYPE_ARAP";
  }
  if( enumNum EQ 89)
  {
    return "IFTYPE_PROPCNLS";
  }
  if( enumNum EQ 90)
  {
    return "IFTYPE_HOSTPAD";
  }
  if( enumNum EQ 91)
  {
    return "IFTYPE_TERMPAD";
  }
  if( enumNum EQ 92)
  {
    return "IFTYPE_FRAMERELAYMPI";
  }
  if( enumNum EQ 93)
  {
    return "IFTYPE_X213";
  }
  if( enumNum EQ 94)
  {
    return "IFTYPE_ADSL";
  }
  if( enumNum EQ 95)
  {
    return "IFTYPE_RADSL";
  }
  if( enumNum EQ 96)
  {
    return "IFTYPE_SDSL";
  }
  if( enumNum EQ 97)
  {
    return "IFTYPE_VDSL";
  }
  if( enumNum EQ 98)
  {
    return "IFTYPE_ISO88025CRFPINT";
  }
  if( enumNum EQ 99)
  {
    return "IFTYPE_MYRINET";
  }
  if( enumNum EQ 100)
  {
    return "IFTYPE_VOICEEM";
  }
  if( enumNum EQ 101)
  {
    return "IFTYPE_VOICEFXO";
  }
  if( enumNum EQ 102)
  {
    return "IFTYPE_VOICEFXS";
  }
  if( enumNum EQ 103)
  {
    return "IFTYPE_VOICEENCAP";
  }
  if( enumNum EQ 104)
  {
    return "IFTYPE_VOICEOVERIP";
  }
  if( enumNum EQ 105)
  {
    return "IFTYPE_ATMDXI";
  }
  if( enumNum EQ 106)
  {
    return "IFTYPE_ATMFUNI";
  }
  if( enumNum EQ 107)
  {
    return "IFTYPE_ATMIMA";
  }
  if( enumNum EQ 108)
  {
    return "IFTYPE_PPPMULTILINKBUNDLE";
  }
  if( enumNum EQ 109)
  {
    return "IFTYPE_IPOVERCDLC";
  }
  if( enumNum EQ 110)
  {
    return "IFTYPE_IPOVERCLAW";
  }
  if( enumNum EQ 111)
  {
    return "IFTYPE_STACKTOSTACK";
  }
  if( enumNum EQ 112)
  {
    return "IFTYPE_VIRTUALIPADDRESS";
  }
  if( enumNum EQ 113)
  {
    return "IFTYPE_MPC";
  }
  if( enumNum EQ 114)
  {
    return "IFTYPE_IPOVERATM";
  }
  if( enumNum EQ 115)
  {
    return "IFTYPE_ISO88025FIBER";
  }
  if( enumNum EQ 116)
  {
    return "IFTYPE_TDLC";
  }
  if( enumNum EQ 117)
  {
    return "IFTYPE_GIGABITETHERNET";
  }
  if( enumNum EQ 118)
  {
    return "IFTYPE_HDLC";
  }
  if( enumNum EQ 119)
  {
    return "IFTYPE_LAPF";
  }
  if( enumNum EQ 120)
  {
    return "IFTYPE_V37";
  }
  if( enumNum EQ 121)
  {
    return "IFTYPE_X25MLP";
  }
  if( enumNum EQ 122)
  {
    return "IFTYPE_X25HUNTGROUP";
  }
  if( enumNum EQ 123)
  {
    return "IFTYPE_TRANSPHDLC";
  }
  if( enumNum EQ 124)
  {
    return "IFTYPE_INTERLEAVE";
  }
  if( enumNum EQ 125)
  {
    return "IFTYPE_FAST";
  }
  if( enumNum EQ 126)
  {
    return "IFTYPE_IP";
  }
  if( enumNum EQ 127)
  {
    return "IFTYPE_DOCSCABLEMACLAYER";
  }
  if( enumNum EQ 128)
  {
    return "IFTYPE_DOCSCABLEDOWNSTREAM";
  }
  if( enumNum EQ 129)
  {
    return "IFTYPE_DOCSCABLEUPSTREAM";
  }
  if( enumNum EQ 130)
  {
    return "IFTYPE_A12MPPSWITCH";
  }
  if( enumNum EQ 131)
  {
    return "IFTYPE_TUNNEL";
  }
  if( enumNum EQ 132)
  {
    return "IFTYPE_COFFEE";
  }
  if( enumNum EQ 133)
  {
    return "IFTYPE_CES";
  }
  if( enumNum EQ 134)
  {
    return "IFTYPE_ATMSUBINTERFACE";
  }
  if( enumNum EQ 135)
  {
    return "IFTYPE_L2VLAN";
  }
  if( enumNum EQ 136)
  {
    return "IFTYPE_L3IPVLAN";
  }
  if( enumNum EQ 137)
  {
    return "IFTYPE_L3IPXVLAN";
  }
  if( enumNum EQ 138)
  {
    return "IFTYPE_DIGITALPOWERLINE";
  }
  if( enumNum EQ 139)
  {
    return "IFTYPE_MEDIAMAILOVERIP";
  }
  if( enumNum EQ 140)
  {
    return "IFTYPE_DTM";
  }
  if( enumNum EQ 141)
  {
    return "IFTYPE_DCN";
  }
  if( enumNum EQ 142)
  {
    return "IFTYPE_IPFORWARD";
  }
  if( enumNum EQ 143)
  {
    return "IFTYPE_MSDSL";
  }
  if( enumNum EQ 144)
  {
    return "IFTYPE_IEEE1394";
  }
  if( enumNum EQ 145)
  {
    return "IFTYPE_IF_GSN";
  }
  if( enumNum EQ 146)
  {
    return "IFTYPE_DVBRCCMACLAYER";
  }
  if( enumNum EQ 147)
  {
    return "IFTYPE_DVBRCCDOWNSTREAM";
  }
  if( enumNum EQ 148)
  {
    return "IFTYPE_DVBRCCUPSTREAM";
  }
  if( enumNum EQ 149)
  {
    return "IFTYPE_ATMVIRTUAL";
  }
  if( enumNum EQ 150)
  {
    return "IFTYPE_MPLSTUNNEL";
  }
  if( enumNum EQ 151)
  {
    return "IFTYPE_SRP";
  }
  if( enumNum EQ 152)
  {
    return "IFTYPE_VOICEOVERATM";
  }
  if( enumNum EQ 153)
  {
    return "IFTYPE_VOICEOVERFRAMERELAY";
  }
  if( enumNum EQ 154)
  {
    return "IFTYPE_IDSL";
  }
  if( enumNum EQ 155)
  {
    return "IFTYPE_COMPOSITELINK";
  }
  if( enumNum EQ 156)
  {
    return "IFTYPE_SS7SIGLINK";
  }
  if( enumNum EQ 157)
  {
    return "IFTYPE_PROPWIRELESSP2P";
  }
  if( enumNum EQ 158)
  {
    return "IFTYPE_FRFORWARD";
  }
  if( enumNum EQ 159)
  {
    return "IFTYPE_RFC1483";
  }
  if( enumNum EQ 160)
  {
    return "IFTYPE_USB";
  }
  if( enumNum EQ 161)
  {
    return "IFTYPE_IEEE8023ADLAG";
  }
  if( enumNum EQ 162)
  {
    return "IFTYPE_BGPPOLICYACCOUNTING";
  }
  if( enumNum EQ 163)
  {
    return "IFTYPE_FRF16MFRBUNDLE";
  }
  if( enumNum EQ 164)
  {
    return "IFTYPE_H323GATEKEEPER";
  }
  if( enumNum EQ 165)
  {
    return "IFTYPE_H323PROXY";
  }
  if( enumNum EQ 166)
  {
    return "IFTYPE_MPLS";
  }
  if( enumNum EQ 167)
  {
    return "IFTYPE_MFSIGLINK";
  }
  if( enumNum EQ 168)
  {
    return "IFTYPE_HDSL2";
  }
  if( enumNum EQ 169)
  {
    return "IFTYPE_SHDSL";
  }
  if( enumNum EQ 170)
  {
    return "IFTYPE_DS1FDL";
  }
  if( enumNum EQ 171)
  {
    return "IFTYPE_POS";
  }
  if( enumNum EQ 172)
  {
    return "IFTYPE_DVBASIIN";
  }
  if( enumNum EQ 173)
  {
    return "IFTYPE_DVBASIOUT";
  }
  if( enumNum EQ 174)
  {
    return "IFTYPE_PLC";
  }
  if( enumNum EQ 175)
  {
    return "IFTYPE_NFAS";
  }
  if( enumNum EQ 176)
  {
    return "IFTYPE_TR008";
  }
  if( enumNum EQ 177)
  {
    return "IFTYPE_GR303RDT";
  }
  if( enumNum EQ 178)
  {
    return "IFTYPE_GR303IDT";
  }
  if( enumNum EQ 179)
  {
    return "IFTYPE_ISUP";
  }
  if( enumNum EQ 180)
  {
    return "IFTYPE_PROPDOCSWIRELESSMACLAYER";
  }
  if( enumNum EQ 181)
  {
    return "IFTYPE_PROPDOCSWIRELESSDOWNSTREAM";
  }
  if( enumNum EQ 182)
  {
    return "IFTYPE_PROPDOCSWIRELESSUPSTREAM";
  }
  if( enumNum EQ 183)
  {
    return "IFTYPE_HIPERLAN2";
  }
  if( enumNum EQ 184)
  {
    return "IFTYPE_PROPBWAP2MP";
  }
  if( enumNum EQ 185)
  {
    return "IFTYPE_SONETOVERHEADCHANNEL";
  }
  if( enumNum EQ 186)
  {
    return "IFTYPE_DIGITALWRAPPEROVERHEADCHANNEL";
  }
  if( enumNum EQ 187)
  {
    return "IFTYPE_AAL2";
  }
  if( enumNum EQ 188)
  {
    return "IFTYPE_RADIOMAC";
  }
  if( enumNum EQ 189)
  {
    return "IFTYPE_ATMRADIO";
  }
  if( enumNum EQ 190)
  {
    return "IFTYPE_IMT";
  }
  if( enumNum EQ 191)
  {
    return "IFTYPE_MVL";
  }
  if( enumNum EQ 192)
  {
    return "IFTYPE_REACHDSL";
  }
  if( enumNum EQ 193)
  {
    return "IFTYPE_FRDLCIENDPT";
  }
  if( enumNum EQ 194)
  {
    return "IFTYPE_ATMVCIENDPT";
  }
  if( enumNum EQ 195)
  {
    return "IFTYPE_OPTICALCHANNEL";
  }
  if( enumNum EQ 196)
  {
    return "IFTYPE_OPTICALTRANSPORT";
  }
  if( enumNum EQ 197)
  {
    return "IFTYPE_PROPATM";
  }
  if( enumNum EQ 198)
  {
    return "IFTYPE_VOICEOVERCABLE";
  }
  if( enumNum EQ 199)
  {
    return "IFTYPE_INFINIBAND";
  }
  if( enumNum EQ 200)
  {
    return "IFTYPE_TELINK";
  }
  if( enumNum EQ 201)
  {
    return "IFTYPE_Q2931";
  }
  if( enumNum EQ 202)
  {
    return "IFTYPE_VIRTUALTG";
  }
  if( enumNum EQ 203)
  {
    return "IFTYPE_SIPTG";
  }
  if( enumNum EQ 204)
  {
    return "IFTYPE_SIPSIG";
  }
  if( enumNum EQ 205)
  {
    return "IFTYPE_DOCSCABLEUPSTREAMCHANNEL";
  }
  if( enumNum EQ 206)
  {
    return "IFTYPE_ECONET";
  }
  if( enumNum EQ 207)
  {
    return "IFTYPE_PON155";
  }
  if( enumNum EQ 208)
  {
    return "IFTYPE_PON622";
  }
  if( enumNum EQ 209)
  {
    return "IFTYPE_BRIDGE";
  }
  if( enumNum EQ 210)
  {
    return "IFTYPE_LINEGROUP";
  }
  if( enumNum EQ 211)
  {
    return "IFTYPE_VOICEEMFGD";
  }
  if( enumNum EQ 212)
  {
    return "IFTYPE_VOICEFGDEANA";
  }
  if( enumNum EQ 213)
  {
    return "IFTYPE_VOICEDID";
  }
  if( enumNum EQ 214)
  {
    return "IFTYPE_MPEGTRANSPORT";
  }
  if( enumNum EQ 215)
  {
    return "IFTYPE_SIXTOFOUR";
  }
  if( enumNum EQ 216)
  {
    return "IFTYPE_GTP";
  }
  if( enumNum EQ 217)
  {
    return "IFTYPE_PDNETHERLOOP1";
  }
  if( enumNum EQ 218)
  {
    return "IFTYPE_PDNETHERLOOP2";
  }
  if( enumNum EQ 219)
  {
    return "IFTYPE_OPTICALCHANNELGROUP";
  }
  if( enumNum EQ 220)
  {
    return "IFTYPE_HOMEPNA";
  }
  if( enumNum EQ 221)
  {
    return "IFTYPE_GFP";
  }
  if( enumNum EQ 222)
  {
    return "IFTYPE_CISCOISLVLAN";
  }
  if( enumNum EQ 223)
  {
    return "IFTYPE_ACTELISMETALOOP";
  }
  if( enumNum EQ 224)
  {
    return "IFTYPE_FCIPLINK";
  }
  if( enumNum EQ 225)
  {
    return "IFTYPE_RPR";
  }
  if( enumNum EQ 226)
  {
    return "IFTYPE_QAM";
  }
  if( enumNum EQ 227)
  {
    return "IFTYPE_LMP";
  }
  if( enumNum EQ 228)
  {
    return "IFTYPE_CBLVECTASTAR";
  }
  if( enumNum EQ 229)
  {
    return "IFTYPE_DOCSCABLEMCMTSDOWNSTREAM";
  }
  if( enumNum EQ 230)
  {
    return "IFTYPE_ADSL2";
  }
  if( enumNum EQ 231)
  {
    return "IFTYPE_MACSECCONTROLLEDIF";
  }
  if( enumNum EQ 232)
  {
    return "IFTYPE_MACSECUNCONTROLLEDIF";
  }
  if( enumNum EQ 233)
  {
    return "IFTYPE_AVICIOPTICALETHER";
  }
  if( enumNum EQ 234)
  {
    return "IFTYPE_ATMBOND";
  }
  if( enumNum EQ 235)
  {
    return "IFTYPE_VOICEFGDOS";
  }
  if( enumNum EQ 236)
  {
    return "IFTYPE_MOCAVERSION1";
  }
  if( enumNum EQ 237)
  {
    return "IFTYPE_IEEE80216WMAN";
  }
  if( enumNum EQ 238)
  {
    return "IFTYPE_ADSL2PLUS";
  }
  if( enumNum EQ 239)
  {
    return "IFTYPE_DVBRCSMACLAYER";
  }
  if( enumNum EQ 240)
  {
    return "IFTYPE_DVBTDM";
  }
  if( enumNum EQ 241)
  {
    return "IFTYPE_DVBRCSTDMA";
  }
  if( enumNum EQ 242)
  {
    return "IFTYPE_X86LAPS";
  }
  if( enumNum EQ 243)
  {
    return "IFTYPE_WWANPP";
  }
  if( enumNum EQ 244)
  {
    return "IFTYPE_WWANPP2";
  }
  if( enumNum EQ 245)
  {
    return "IFTYPE_VOICEEBS";
  }
  if( enumNum EQ 246)
  {
    return "IFTYPE_IFPWTYPE";
  }
  if( enumNum EQ 247)
  {
    return "IFTYPE_ILAN";
  }
  if( enumNum EQ 248)
  {
    return "IFTYPE_PIP";
  }
  if( enumNum EQ 249)
  {
    return "IFTYPE_ALUELP";
  }
  if( enumNum EQ 250)
  {
    return "IFTYPE_GPON";
  }
  if( enumNum EQ 251)
  {
    return "IFTYPE_VDSL2";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* ifSpeedStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "IFSPEED_UNDETECTABLE";
  }
  if( enumNum EQ 2)
  {
    return "IFSPEED_MBPS10";
  }
  if( enumNum EQ 3)
  {
    return "IFSPEED_MBPS100";
  }
  if( enumNum EQ 4)
  {
    return "IFSPEED_GBPS1";
  }
  if( enumNum EQ 5)
  {
    return "IFSPEED_GBPS2ANDHALF";
  }
  if( enumNum EQ 6)
  {
    return "IFSPEED_GBPS10";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* ifAdminStatusStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "IFADMINSTATUS_ISUP";
  }
  if( enumNum EQ 2)
  {
    return "IFADMINSTATUS_ISDOWN";
  }
  if( enumNum EQ 3)
  {
    return "IFADMINSTATUS_ISTESTING";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* ifOperStatusStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "IFOPERSTATUS_ISUP";
  }
  if( enumNum EQ 2)
  {
    return "IFOPERSTATUS_ISDOWN";
  }
  if( enumNum EQ 3)
  {
    return "IFOPERSTATUS_ISTESTING";
  }
  if( enumNum EQ 4)
  {
    return "IFOPERSTATUS_ISUNKNOWN";
  }
  if( enumNum EQ 5)
  {
    return "IFOPERSTATUS_ISDORMANT";
  }
  if( enumNum EQ 6)
  {
    return "IFOPERSTATUS_ISNOTPRESENT";
  }
  if( enumNum EQ 7)
  {
    return "IFOPERSTATUS_ISLOWERLAYERDOWN";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* ipSystemStatsIPVersionStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "IPSYSTEMSTATSIPVERSION_UNKNOWN";
  }
  if( enumNum EQ 1)
  {
    return "IPSYSTEMSTATSIPVERSION_IPV4";
  }
  if( enumNum EQ 2)
  {
    return "IPSYSTEMSTATSIPVERSION_IPV6";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* priviledgeLevelStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "PRIVILEDGELEVEL_LEVEL1";
  }
  if( enumNum EQ 2)
  {
    return "PRIVILEDGELEVEL_LEVEL2";
  }
  if( enumNum EQ 3)
  {
    return "PRIVILEDGELEVEL_LEVEL3";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* oamInterfaceTypeStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "OAMINTERFACETYPE_CLI";
  }
  if( enumNum EQ 2)
  {
    return "OAMINTERFACETYPE_WEB";
  }
  if( enumNum EQ 3)
  {
    return "OAMINTERFACETYPE_EMS";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* ledIdStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "LEDID_BBPOWER";
  }
  if( enumNum EQ 2)
  {
    return "LEDID_GPS";
  }
  if( enumNum EQ 3)
  {
    return "LEDID_EMSREGISTRATION";
  }
  if( enumNum EQ 4)
  {
    return "LEDID_ENBOPERATIONAL";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* ledColorStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "LEDCOLOR_AMBER";
  }
  if( enumNum EQ 2)
  {
    return "LEDCOLOR_RED";
  }
  if( enumNum EQ 3)
  {
    return "LEDCOLOR_GREEN";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* traceEndCauseStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "TRACEENDCAUSE_STOPPEDBYOPERATOR";
  }
  if( enumNum EQ 2)
  {
    return "TRACEENDCAUSE_TIMEDURATIONELAPSED";
  }
  if( enumNum EQ 3)
  {
    return "TRACEENDCAUSE_MAXTRCFILESIZEREACHED";
  }
  if( enumNum EQ 4)
  {
    return "TRACEENDCAUSE_ENBERROR";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* traceStateStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "TRACESTATE_TRACEON";
  }
  if( enumNum EQ 2)
  {
    return "TRACESTATE_TRACEOFF";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* msgTraceLevelStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "MSGTRACELEVEL_DISABLE";
  }
  if( enumNum EQ 2)
  {
    return "MSGTRACELEVEL_MSGIDONLY";
  }
  if( enumNum EQ 3)
  {
    return "MSGTRACELEVEL_MSGWITHPARAMS";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* msgTraceDirectionStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "MSGTRACEDIRECTION_MSGIN";
  }
  if( enumNum EQ 2)
  {
    return "MSGTRACEDIRECTION_MSGOUT";
  }
  if( enumNum EQ 3)
  {
    return "MSGTRACEDIRECTION_MSGINBOTHDIREC";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* timerNameStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "TIMERNAME_WAITINITCOMPLETE";
  }
  if( enumNum EQ 2)
  {
    return "TIMERNAME_HEALTHCHECKINTERVAL";
  }
  if( enumNum EQ 3)
  {
    return "TIMERNAME_WAITHELLOACK";
  }
  if( enumNum EQ 4)
  {
    return "TIMERNAME_WAITEXITCOMPLETE";
  }
  if( enumNum EQ 5)
  {
    return "TIMERNAME_WAITBOOTLOWERARMCOMPLETE";
  }
  if( enumNum EQ 6)
  {
    return "TIMERNAME_WAITRHSWINSTALLRSP";
  }
  if( enumNum EQ 7)
  {
    return "TIMERNAME_WAITRHACTIVATERSP";
  }
  if( enumNum EQ 8)
  {
    return "TIMERNAME_WAITDEHSTATUSREPORTPERIOD";
  }
  if( enumNum EQ 9)
  {
    return "TIMERNAME_WAITALLDEVICERSP";
  }
  if( enumNum EQ 10)
  {
    return "TIMERNAME_WAITRHINIT";
  }
  if( enumNum EQ 11)
  {
    return "TIMERNAME_WAITRHDISABLERSP";
  }
  if( enumNum EQ 12)
  {
    return "TIMERNAME_WAITRHENABLERSP";
  }
  if( enumNum EQ 13)
  {
    return "TIMERNAME_WAITRHCONFIGRSP";
  }
  if( enumNum EQ 14)
  {
    return "TIMERNAME_WAITRHSTATUSRSPFORCIH";
  }
  if( enumNum EQ 15)
  {
    return "TIMERNAME_WAITRHSTATUSRSPFORSIH";
  }
  if( enumNum EQ 16)
  {
    return "TIMERNAME_WAITRHSTATUSRSPFORWIH";
  }
  if( enumNum EQ 17)
  {
    return "TIMERNAME_WAITBISTCOMPLETE";
  }
  if( enumNum EQ 18)
  {
    return "TIMERNAME_WAITCPRILINKUP";
  }
  if( enumNum EQ 19)
  {
    return "TIMERNAME_WAITTRACECONFIRM";
  }
  if( enumNum EQ 20)
  {
    return "TIMERNAME_WAITTRACEDURATION";
  }
  if( enumNum EQ 21)
  {
    return "TIMERNAME_WAITENABLECOMPLETE";
  }
  if( enumNum EQ 22)
  {
    return "TIMERNAME_WAITDISABLECOMPLETE";
  }
  if( enumNum EQ 23)
  {
    return "TIMERNAME_WAITCONFIGCOMPLETE";
  }
  if( enumNum EQ 24)
  {
    return "TIMERNAME_WAITGPSPARAMETERSRSP";
  }
  if( enumNum EQ 25)
  {
    return "TIMERNAME_WAITEMSREGRSP";
  }
  if( enumNum EQ 26)
  {
    return "TIMERNAME_WAITMMELINKESTABLISH";
  }
  if( enumNum EQ 27)
  {
    return "TIMERNAME_WAITDEVICEINITCOMPLETE";
  }
  if( enumNum EQ 28)
  {
    return "TIMERNAME_WAITINITIPADDRCOMPLETE";
  }
  if( enumNum EQ 29)
  {
    return "TIMERNAME_WAITCMDRSP";
  }
  if( enumNum EQ 30)
  {
    return "TIMERNAME_WAITCOUNTERCOLLECTION";
  }
  if( enumNum EQ 31)
  {
    return "TIMERNAME_WAITSNMPCMDRSP";
  }
  if( enumNum EQ 32)
  {
    return "TIMERNAME_WAITCOUNTERRESET";
  }
  if( enumNum EQ 33)
  {
    return "TIMERNAME_WAITL2PMCOUNTERCONTROLPROFILE1";
  }
  if( enumNum EQ 34)
  {
    return "TIMERNAME_WAITL2PMCOUNTERCONTROLPROFILE2";
  }
  if( enumNum EQ 35)
  {
    return "TIMERNAME_WAITL2PMREPORTPROFILE1";
  }
  if( enumNum EQ 36)
  {
    return "TIMERNAME_WAITL2PMREPORTPROFILE2";
  }
  if( enumNum EQ 37)
  {
    return "TIMERNAME_WAITSAMPLEINTERVAL";
  }
  if( enumNum EQ 38)
  {
    return "TIMERNAME_WAITPMFILECLOSE";
  }
  if( enumNum EQ 39)
  {
    return "TIMERNAME_WAITSCHEDULEDFILETRANSFERTOSTART";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* timeUnitStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "TIMEUNIT_USEC";
  }
  if( enumNum EQ 2)
  {
    return "TIMEUNIT_MSEC";
  }
  if( enumNum EQ 3)
  {
    return "TIMEUNIT_SEC";
  }
  if( enumNum EQ 4)
  {
    return "TIMEUNIT_MINUTE";
  }
  if( enumNum EQ 5)
  {
    return "TIMEUNIT_MAX";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* scSpacingStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SCSPACING_15KHZ";
  }
  if( enumNum EQ 1)
  {
    return "SCSPACING_7DOT5KHZ";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* cycPfxTypeStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "CYCPFXTYPE_CYCPFXNORMAL";
  }
  if( enumNum EQ 1)
  {
    return "CYCPFXTYPE_CYCPFXEXTENDED";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* srbPollRetransStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SRBPOLLRETRANS_MS5";
  }
  if( enumNum EQ 1)
  {
    return "SRBPOLLRETRANS_MS10";
  }
  if( enumNum EQ 2)
  {
    return "SRBPOLLRETRANS_MS15";
  }
  if( enumNum EQ 3)
  {
    return "SRBPOLLRETRANS_MS20";
  }
  if( enumNum EQ 4)
  {
    return "SRBPOLLRETRANS_MS25";
  }
  if( enumNum EQ 5)
  {
    return "SRBPOLLRETRANS_MS30";
  }
  if( enumNum EQ 6)
  {
    return "SRBPOLLRETRANS_MS35";
  }
  if( enumNum EQ 7)
  {
    return "SRBPOLLRETRANS_MS40";
  }
  if( enumNum EQ 8)
  {
    return "SRBPOLLRETRANS_MS45";
  }
  if( enumNum EQ 9)
  {
    return "SRBPOLLRETRANS_MS50";
  }
  if( enumNum EQ 10)
  {
    return "SRBPOLLRETRANS_MS55";
  }
  if( enumNum EQ 11)
  {
    return "SRBPOLLRETRANS_MS60";
  }
  if( enumNum EQ 12)
  {
    return "SRBPOLLRETRANS_MS65";
  }
  if( enumNum EQ 13)
  {
    return "SRBPOLLRETRANS_MS70";
  }
  if( enumNum EQ 14)
  {
    return "SRBPOLLRETRANS_MS75";
  }
  if( enumNum EQ 15)
  {
    return "SRBPOLLRETRANS_MS80";
  }
  if( enumNum EQ 16)
  {
    return "SRBPOLLRETRANS_MS85";
  }
  if( enumNum EQ 17)
  {
    return "SRBPOLLRETRANS_MS90";
  }
  if( enumNum EQ 18)
  {
    return "SRBPOLLRETRANS_MS95";
  }
  if( enumNum EQ 19)
  {
    return "SRBPOLLRETRANS_MS100";
  }
  if( enumNum EQ 20)
  {
    return "SRBPOLLRETRANS_MS105";
  }
  if( enumNum EQ 21)
  {
    return "SRBPOLLRETRANS_MS110";
  }
  if( enumNum EQ 22)
  {
    return "SRBPOLLRETRANS_MS115";
  }
  if( enumNum EQ 23)
  {
    return "SRBPOLLRETRANS_MS120";
  }
  if( enumNum EQ 24)
  {
    return "SRBPOLLRETRANS_MS125";
  }
  if( enumNum EQ 25)
  {
    return "SRBPOLLRETRANS_MS130";
  }
  if( enumNum EQ 26)
  {
    return "SRBPOLLRETRANS_MS135";
  }
  if( enumNum EQ 27)
  {
    return "SRBPOLLRETRANS_MS140";
  }
  if( enumNum EQ 28)
  {
    return "SRBPOLLRETRANS_MS145";
  }
  if( enumNum EQ 29)
  {
    return "SRBPOLLRETRANS_MS150";
  }
  if( enumNum EQ 30)
  {
    return "SRBPOLLRETRANS_MS155";
  }
  if( enumNum EQ 31)
  {
    return "SRBPOLLRETRANS_MS160";
  }
  if( enumNum EQ 32)
  {
    return "SRBPOLLRETRANS_MS165";
  }
  if( enumNum EQ 33)
  {
    return "SRBPOLLRETRANS_MS170";
  }
  if( enumNum EQ 34)
  {
    return "SRBPOLLRETRANS_MS175";
  }
  if( enumNum EQ 35)
  {
    return "SRBPOLLRETRANS_MS180";
  }
  if( enumNum EQ 36)
  {
    return "SRBPOLLRETRANS_MS185";
  }
  if( enumNum EQ 37)
  {
    return "SRBPOLLRETRANS_MS190";
  }
  if( enumNum EQ 38)
  {
    return "SRBPOLLRETRANS_MS195";
  }
  if( enumNum EQ 39)
  {
    return "SRBPOLLRETRANS_MS200";
  }
  if( enumNum EQ 40)
  {
    return "SRBPOLLRETRANS_MS205";
  }
  if( enumNum EQ 41)
  {
    return "SRBPOLLRETRANS_MS210";
  }
  if( enumNum EQ 42)
  {
    return "SRBPOLLRETRANS_MS215";
  }
  if( enumNum EQ 43)
  {
    return "SRBPOLLRETRANS_MS220";
  }
  if( enumNum EQ 44)
  {
    return "SRBPOLLRETRANS_MS225";
  }
  if( enumNum EQ 45)
  {
    return "SRBPOLLRETRANS_MS230";
  }
  if( enumNum EQ 46)
  {
    return "SRBPOLLRETRANS_MS235";
  }
  if( enumNum EQ 47)
  {
    return "SRBPOLLRETRANS_MS240";
  }
  if( enumNum EQ 48)
  {
    return "SRBPOLLRETRANS_MS245";
  }
  if( enumNum EQ 49)
  {
    return "SRBPOLLRETRANS_MS250";
  }
  if( enumNum EQ 50)
  {
    return "SRBPOLLRETRANS_MS300";
  }
  if( enumNum EQ 51)
  {
    return "SRBPOLLRETRANS_MS350";
  }
  if( enumNum EQ 52)
  {
    return "SRBPOLLRETRANS_MS400";
  }
  if( enumNum EQ 53)
  {
    return "SRBPOLLRETRANS_MS450";
  }
  if( enumNum EQ 54)
  {
    return "SRBPOLLRETRANS_MS500";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* srbPollPduStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SRBPOLLPDU_P4";
  }
  if( enumNum EQ 1)
  {
    return "SRBPOLLPDU_P8";
  }
  if( enumNum EQ 2)
  {
    return "SRBPOLLPDU_P16";
  }
  if( enumNum EQ 3)
  {
    return "SRBPOLLPDU_P32";
  }
  if( enumNum EQ 4)
  {
    return "SRBPOLLPDU_P64";
  }
  if( enumNum EQ 5)
  {
    return "SRBPOLLPDU_P128";
  }
  if( enumNum EQ 6)
  {
    return "SRBPOLLPDU_P256";
  }
  if( enumNum EQ 7)
  {
    return "SRBPOLLPDU_PINFINITY";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* srbPollByteStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SRBPOLLBYTE_KB25";
  }
  if( enumNum EQ 1)
  {
    return "SRBPOLLBYTE_KB50";
  }
  if( enumNum EQ 2)
  {
    return "SRBPOLLBYTE_KB75";
  }
  if( enumNum EQ 3)
  {
    return "SRBPOLLBYTE_KB100";
  }
  if( enumNum EQ 4)
  {
    return "SRBPOLLBYTE_KB125";
  }
  if( enumNum EQ 5)
  {
    return "SRBPOLLBYTE_KB250";
  }
  if( enumNum EQ 6)
  {
    return "SRBPOLLBYTE_KB375";
  }
  if( enumNum EQ 7)
  {
    return "SRBPOLLBYTE_KB500";
  }
  if( enumNum EQ 8)
  {
    return "SRBPOLLBYTE_KB750";
  }
  if( enumNum EQ 9)
  {
    return "SRBPOLLBYTE_KB1000";
  }
  if( enumNum EQ 10)
  {
    return "SRBPOLLBYTE_KB1250";
  }
  if( enumNum EQ 11)
  {
    return "SRBPOLLBYTE_KB1500";
  }
  if( enumNum EQ 12)
  {
    return "SRBPOLLBYTE_KB2000";
  }
  if( enumNum EQ 13)
  {
    return "SRBPOLLBYTE_KB3000";
  }
  if( enumNum EQ 14)
  {
    return "SRBPOLLBYTE_KBINFINITY";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* srbMaxReTxThreshStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SRBMAXRETXTHRESH_T1";
  }
  if( enumNum EQ 1)
  {
    return "SRBMAXRETXTHRESH_T2";
  }
  if( enumNum EQ 2)
  {
    return "SRBMAXRETXTHRESH_T3";
  }
  if( enumNum EQ 3)
  {
    return "SRBMAXRETXTHRESH_T4";
  }
  if( enumNum EQ 4)
  {
    return "SRBMAXRETXTHRESH_T6";
  }
  if( enumNum EQ 5)
  {
    return "SRBMAXRETXTHRESH_T8";
  }
  if( enumNum EQ 6)
  {
    return "SRBMAXRETXTHRESH_T16";
  }
  if( enumNum EQ 7)
  {
    return "SRBMAXRETXTHRESH_T32";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* srbTreordrngStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SRBTREORDRNG_MS0";
  }
  if( enumNum EQ 1)
  {
    return "SRBTREORDRNG_MS5";
  }
  if( enumNum EQ 2)
  {
    return "SRBTREORDRNG_MS10";
  }
  if( enumNum EQ 3)
  {
    return "SRBTREORDRNG_MS15";
  }
  if( enumNum EQ 4)
  {
    return "SRBTREORDRNG_MS20";
  }
  if( enumNum EQ 5)
  {
    return "SRBTREORDRNG_MS25";
  }
  if( enumNum EQ 6)
  {
    return "SRBTREORDRNG_MS30";
  }
  if( enumNum EQ 7)
  {
    return "SRBTREORDRNG_MS35";
  }
  if( enumNum EQ 8)
  {
    return "SRBTREORDRNG_MS40";
  }
  if( enumNum EQ 9)
  {
    return "SRBTREORDRNG_MS45";
  }
  if( enumNum EQ 10)
  {
    return "SRBTREORDRNG_MS50";
  }
  if( enumNum EQ 11)
  {
    return "SRBTREORDRNG_MS55";
  }
  if( enumNum EQ 12)
  {
    return "SRBTREORDRNG_MS60";
  }
  if( enumNum EQ 13)
  {
    return "SRBTREORDRNG_MS65";
  }
  if( enumNum EQ 14)
  {
    return "SRBTREORDRNG_MS70";
  }
  if( enumNum EQ 15)
  {
    return "SRBTREORDRNG_MS75";
  }
  if( enumNum EQ 16)
  {
    return "SRBTREORDRNG_MS80";
  }
  if( enumNum EQ 17)
  {
    return "SRBTREORDRNG_MS85";
  }
  if( enumNum EQ 18)
  {
    return "SRBTREORDRNG_MS90";
  }
  if( enumNum EQ 19)
  {
    return "SRBTREORDRNG_MS95";
  }
  if( enumNum EQ 20)
  {
    return "SRBTREORDRNG_MS100";
  }
  if( enumNum EQ 21)
  {
    return "SRBTREORDRNG_MS110";
  }
  if( enumNum EQ 22)
  {
    return "SRBTREORDRNG_MS120";
  }
  if( enumNum EQ 23)
  {
    return "SRBTREORDRNG_MS130";
  }
  if( enumNum EQ 24)
  {
    return "SRBTREORDRNG_MS140";
  }
  if( enumNum EQ 25)
  {
    return "SRBTREORDRNG_MS150";
  }
  if( enumNum EQ 26)
  {
    return "SRBTREORDRNG_MS160";
  }
  if( enumNum EQ 27)
  {
    return "SRBTREORDRNG_MS170";
  }
  if( enumNum EQ 28)
  {
    return "SRBTREORDRNG_MS180";
  }
  if( enumNum EQ 29)
  {
    return "SRBTREORDRNG_MS190";
  }
  if( enumNum EQ 30)
  {
    return "SRBTREORDRNG_MS200";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* srbTStatProhibitStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SRBTSTATPROHIBIT_MS0";
  }
  if( enumNum EQ 1)
  {
    return "SRBTSTATPROHIBIT_MS5";
  }
  if( enumNum EQ 2)
  {
    return "SRBTSTATPROHIBIT_MS10";
  }
  if( enumNum EQ 3)
  {
    return "SRBTSTATPROHIBIT_MS15";
  }
  if( enumNum EQ 4)
  {
    return "SRBTSTATPROHIBIT_MS20";
  }
  if( enumNum EQ 5)
  {
    return "SRBTSTATPROHIBIT_MS25";
  }
  if( enumNum EQ 6)
  {
    return "SRBTSTATPROHIBIT_MS30";
  }
  if( enumNum EQ 7)
  {
    return "SRBTSTATPROHIBIT_MS35";
  }
  if( enumNum EQ 8)
  {
    return "SRBTSTATPROHIBIT_MS40";
  }
  if( enumNum EQ 9)
  {
    return "SRBTSTATPROHIBIT_MS45";
  }
  if( enumNum EQ 10)
  {
    return "SRBTSTATPROHIBIT_MS50";
  }
  if( enumNum EQ 11)
  {
    return "SRBTSTATPROHIBIT_MS55";
  }
  if( enumNum EQ 12)
  {
    return "SRBTSTATPROHIBIT_MS60";
  }
  if( enumNum EQ 13)
  {
    return "SRBTSTATPROHIBIT_MS65";
  }
  if( enumNum EQ 14)
  {
    return "SRBTSTATPROHIBIT_MS70";
  }
  if( enumNum EQ 15)
  {
    return "SRBTSTATPROHIBIT_MS75";
  }
  if( enumNum EQ 16)
  {
    return "SRBTSTATPROHIBIT_MS80";
  }
  if( enumNum EQ 17)
  {
    return "SRBTSTATPROHIBIT_MS85";
  }
  if( enumNum EQ 18)
  {
    return "SRBTSTATPROHIBIT_MS90";
  }
  if( enumNum EQ 19)
  {
    return "SRBTSTATPROHIBIT_MS95";
  }
  if( enumNum EQ 20)
  {
    return "SRBTSTATPROHIBIT_MS100";
  }
  if( enumNum EQ 21)
  {
    return "SRBTSTATPROHIBIT_MS105";
  }
  if( enumNum EQ 22)
  {
    return "SRBTSTATPROHIBIT_MS110";
  }
  if( enumNum EQ 23)
  {
    return "SRBTSTATPROHIBIT_MS115";
  }
  if( enumNum EQ 24)
  {
    return "SRBTSTATPROHIBIT_MS120";
  }
  if( enumNum EQ 25)
  {
    return "SRBTSTATPROHIBIT_MS125";
  }
  if( enumNum EQ 26)
  {
    return "SRBTSTATPROHIBIT_MS130";
  }
  if( enumNum EQ 27)
  {
    return "SRBTSTATPROHIBIT_MS135";
  }
  if( enumNum EQ 28)
  {
    return "SRBTSTATPROHIBIT_MS140";
  }
  if( enumNum EQ 29)
  {
    return "SRBTSTATPROHIBIT_MS145";
  }
  if( enumNum EQ 30)
  {
    return "SRBTSTATPROHIBIT_MS150";
  }
  if( enumNum EQ 31)
  {
    return "SRBTSTATPROHIBIT_MS155";
  }
  if( enumNum EQ 32)
  {
    return "SRBTSTATPROHIBIT_MS160";
  }
  if( enumNum EQ 33)
  {
    return "SRBTSTATPROHIBIT_MS165";
  }
  if( enumNum EQ 34)
  {
    return "SRBTSTATPROHIBIT_MS170";
  }
  if( enumNum EQ 35)
  {
    return "SRBTSTATPROHIBIT_MS175";
  }
  if( enumNum EQ 36)
  {
    return "SRBTSTATPROHIBIT_MS180";
  }
  if( enumNum EQ 37)
  {
    return "SRBTSTATPROHIBIT_MS185";
  }
  if( enumNum EQ 38)
  {
    return "SRBTSTATPROHIBIT_MS190";
  }
  if( enumNum EQ 39)
  {
    return "SRBTSTATPROHIBIT_MS195";
  }
  if( enumNum EQ 40)
  {
    return "SRBTSTATPROHIBIT_MS200";
  }
  if( enumNum EQ 41)
  {
    return "SRBTSTATPROHIBIT_MS205";
  }
  if( enumNum EQ 42)
  {
    return "SRBTSTATPROHIBIT_MS210";
  }
  if( enumNum EQ 43)
  {
    return "SRBTSTATPROHIBIT_MS215";
  }
  if( enumNum EQ 44)
  {
    return "SRBTSTATPROHIBIT_MS220";
  }
  if( enumNum EQ 45)
  {
    return "SRBTSTATPROHIBIT_MS225";
  }
  if( enumNum EQ 46)
  {
    return "SRBTSTATPROHIBIT_MS230";
  }
  if( enumNum EQ 47)
  {
    return "SRBTSTATPROHIBIT_MS235";
  }
  if( enumNum EQ 48)
  {
    return "SRBTSTATPROHIBIT_MS240";
  }
  if( enumNum EQ 49)
  {
    return "SRBTSTATPROHIBIT_MS245";
  }
  if( enumNum EQ 50)
  {
    return "SRBTSTATPROHIBIT_MS250";
  }
  if( enumNum EQ 51)
  {
    return "SRBTSTATPROHIBIT_MS300";
  }
  if( enumNum EQ 52)
  {
    return "SRBTSTATPROHIBIT_MS350";
  }
  if( enumNum EQ 53)
  {
    return "SRBTSTATPROHIBIT_MS400";
  }
  if( enumNum EQ 54)
  {
    return "SRBTSTATPROHIBIT_MS450";
  }
  if( enumNum EQ 55)
  {
    return "SRBTSTATPROHIBIT_MS500";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* srsPeriodicityStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "SRSPERIODICITY_SRS2";
  }
  if( enumNum EQ 1)
  {
    return "SRSPERIODICITY_SRS5";
  }
  if( enumNum EQ 2)
  {
    return "SRSPERIODICITY_SRS10";
  }
  if( enumNum EQ 3)
  {
    return "SRSPERIODICITY_SRS20";
  }
  if( enumNum EQ 4)
  {
    return "SRSPERIODICITY_SRS40";
  }
  if( enumNum EQ 5)
  {
    return "SRSPERIODICITY_SRS80";
  }
  if( enumNum EQ 6)
  {
    return "SRSPERIODICITY_SRS160";
  }
  if( enumNum EQ 7)
  {
    return "SRSPERIODICITY_SRS320";
  }
  if( enumNum EQ 8)
  {
    return "SRSPERIODICITY_SRSINV";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* dedCyclicShiftStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "DEDCYCLICSHIFT_CS0";
  }
  if( enumNum EQ 1)
  {
    return "DEDCYCLICSHIFT_CS1";
  }
  if( enumNum EQ 2)
  {
    return "DEDCYCLICSHIFT_CS2";
  }
  if( enumNum EQ 3)
  {
    return "DEDCYCLICSHIFT_CS3";
  }
  if( enumNum EQ 4)
  {
    return "DEDCYCLICSHIFT_CS4";
  }
  if( enumNum EQ 5)
  {
    return "DEDCYCLICSHIFT_CS5";
  }
  if( enumNum EQ 6)
  {
    return "DEDCYCLICSHIFT_CS6";
  }
  if( enumNum EQ 7)
  {
    return "DEDCYCLICSHIFT_CS7";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* dsrTransMaxStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "DSRTRANSMAX_N4";
  }
  if( enumNum EQ 1)
  {
    return "DSRTRANSMAX_N8";
  }
  if( enumNum EQ 2)
  {
    return "DSRTRANSMAX_N16";
  }
  if( enumNum EQ 3)
  {
    return "DSRTRANSMAX_N32";
  }
  if( enumNum EQ 4)
  {
    return "DSRTRANSMAX_N64";
  }
  if( enumNum EQ 5)
  {
    return "DSRTRANSMAX_SPARE3";
  }
  if( enumNum EQ 6)
  {
    return "DSRTRANSMAX_SPARE2";
  }
  if( enumNum EQ 7)
  {
    return "DSRTRANSMAX_SPARE1";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* repFactStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "REPFACT_N2";
  }
  if( enumNum EQ 1)
  {
    return "REPFACT_N4";
  }
  if( enumNum EQ 2)
  {
    return "REPFACT_N6";
  }
  if( enumNum EQ 3)
  {
    return "REPFACT_SPARE1";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* aPeriodicModeStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "APERIODICMODE_RM12";
  }
  if( enumNum EQ 1)
  {
    return "APERIODICMODE_RM20";
  }
  if( enumNum EQ 2)
  {
    return "APERIODICMODE_RM22";
  }
  if( enumNum EQ 3)
  {
    return "APERIODICMODE_RM30";
  }
  if( enumNum EQ 4)
  {
    return "APERIODICMODE_RM31";
  }
  if( enumNum EQ 5)
  {
    return "APERIODICMODE_SPARE3";
  }
  if( enumNum EQ 6)
  {
    return "APERIODICMODE_SPARE2";
  }
  if( enumNum EQ 7)
  {
    return "APERIODICMODE_SPARE1";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* maxUlHqTxStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "MAXULHQTX_N1";
  }
  if( enumNum EQ 1)
  {
    return "MAXULHQTX_N2";
  }
  if( enumNum EQ 2)
  {
    return "MAXULHQTX_N3";
  }
  if( enumNum EQ 3)
  {
    return "MAXULHQTX_N4";
  }
  if( enumNum EQ 4)
  {
    return "MAXULHQTX_N5";
  }
  if( enumNum EQ 5)
  {
    return "MAXULHQTX_N6";
  }
  if( enumNum EQ 6)
  {
    return "MAXULHQTX_N7";
  }
  if( enumNum EQ 7)
  {
    return "MAXULHQTX_N8";
  }
  if( enumNum EQ 8)
  {
    return "MAXULHQTX_N10";
  }
  if( enumNum EQ 9)
  {
    return "MAXULHQTX_N12";
  }
  if( enumNum EQ 10)
  {
    return "MAXULHQTX_N16";
  }
  if( enumNum EQ 11)
  {
    return "MAXULHQTX_N20";
  }
  if( enumNum EQ 12)
  {
    return "MAXULHQTX_N24";
  }
  if( enumNum EQ 13)
  {
    return "MAXULHQTX_N28";
  }
  if( enumNum EQ 14)
  {
    return "MAXULHQTX_SPARE2";
  }
  if( enumNum EQ 15)
  {
    return "MAXULHQTX_SPARE1";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* ulUmSnFieldLenStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "ULUMSNFIELDLEN_SN5";
  }
  if( enumNum EQ 1)
  {
    return "ULUMSNFIELDLEN_SN10";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* dlUmSnFieldLenStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "DLUMSNFIELDLEN_SN5";
  }
  if( enumNum EQ 1)
  {
    return "DLUMSNFIELDLEN_SN10";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* umRlcTReorderingStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "UMRLCTREORDERING_MS0";
  }
  if( enumNum EQ 1)
  {
    return "UMRLCTREORDERING_MS5";
  }
  if( enumNum EQ 2)
  {
    return "UMRLCTREORDERING_MS10";
  }
  if( enumNum EQ 3)
  {
    return "UMRLCTREORDERING_MS15";
  }
  if( enumNum EQ 4)
  {
    return "UMRLCTREORDERING_MS20";
  }
  if( enumNum EQ 5)
  {
    return "UMRLCTREORDERING_MS25";
  }
  if( enumNum EQ 6)
  {
    return "UMRLCTREORDERING_MS30";
  }
  if( enumNum EQ 7)
  {
    return "UMRLCTREORDERING_MS35";
  }
  if( enumNum EQ 8)
  {
    return "UMRLCTREORDERING_MS40";
  }
  if( enumNum EQ 9)
  {
    return "UMRLCTREORDERING_MS45";
  }
  if( enumNum EQ 10)
  {
    return "UMRLCTREORDERING_MS50";
  }
  if( enumNum EQ 11)
  {
    return "UMRLCTREORDERING_MS55";
  }
  if( enumNum EQ 12)
  {
    return "UMRLCTREORDERING_MS60";
  }
  if( enumNum EQ 13)
  {
    return "UMRLCTREORDERING_MS65";
  }
  if( enumNum EQ 14)
  {
    return "UMRLCTREORDERING_MS70";
  }
  if( enumNum EQ 15)
  {
    return "UMRLCTREORDERING_MS75";
  }
  if( enumNum EQ 16)
  {
    return "UMRLCTREORDERING_MS80";
  }
  if( enumNum EQ 17)
  {
    return "UMRLCTREORDERING_MS85";
  }
  if( enumNum EQ 18)
  {
    return "UMRLCTREORDERING_MS90";
  }
  if( enumNum EQ 19)
  {
    return "UMRLCTREORDERING_MS95";
  }
  if( enumNum EQ 20)
  {
    return "UMRLCTREORDERING_MS100";
  }
  if( enumNum EQ 21)
  {
    return "UMRLCTREORDERING_MS110";
  }
  if( enumNum EQ 22)
  {
    return "UMRLCTREORDERING_MS120";
  }
  if( enumNum EQ 23)
  {
    return "UMRLCTREORDERING_MS130";
  }
  if( enumNum EQ 24)
  {
    return "UMRLCTREORDERING_MS140";
  }
  if( enumNum EQ 25)
  {
    return "UMRLCTREORDERING_MS150";
  }
  if( enumNum EQ 26)
  {
    return "UMRLCTREORDERING_MS160";
  }
  if( enumNum EQ 27)
  {
    return "UMRLCTREORDERING_MS170";
  }
  if( enumNum EQ 28)
  {
    return "UMRLCTREORDERING_MS180";
  }
  if( enumNum EQ 29)
  {
    return "UMRLCTREORDERING_MS190";
  }
  if( enumNum EQ 30)
  {
    return "UMRLCTREORDERING_MS200";
  }
 return "UNDEFINED_ENUM";
}

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */
UINT8* internalUeConfigTimerStr(UINT32 enumNum)
{
  if( enumNum EQ 0)
  {
    return "INTERNALUECONFIGTIMER_MS50";
  }
  if( enumNum EQ 1)
  {
    return "INTERNALUECONFIGTIMER_MS100";
  }
  if( enumNum EQ 2)
  {
    return "INTERNALUECONFIGTIMER_MS200";
  }
  if( enumNum EQ 3)
  {
    return "INTERNALUECONFIGTIMER_MS300";
  }
  if( enumNum EQ 4)
  {
    return "INTERNALUECONFIGTIMER_MS500";
  }
  if( enumNum EQ 5)
  {
    return "INTERNALUECONFIGTIMER_MS1000";
  }
  if( enumNum EQ 6)
  {
    return "INTERNALUECONFIGTIMER_MS2000";
  }
  if( enumNum EQ 7)
  {
    return "INTERNALUECONFIGTIMER_MS3000";
  }
  if( enumNum EQ 8)
  {
    return "INTERNALUECONFIGTIMER_MS5000";
  }
  if( enumNum EQ 9)
  {
    return "INTERNALUECONFIGTIMER_MS10000";
  }
  if( enumNum EQ 10)
  {
    return "INTERNALUECONFIGTIMER_MS30000";
  }
  if( enumNum EQ 11)
  {
    return "INTERNALUECONFIGTIMER_MS60000";
  }
  if( enumNum EQ 12)
  {
    return "INTERNALUECONFIGTIMER_MS120000";
  }
 return "UNDEFINED_ENUM";
}

UINT8* alarmCauseStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "MMELINKDOWN";
  }
  if( enumNum EQ 2)
  {
    return "ALLMMELINKSDOWN";
  }
  if( enumNum EQ 3)
  {
    return "EMSREGISTRATIONCONFIRMNOTRECEIVED";
  }
  if( enumNum EQ 21)
  {
    return "REQUESTTIMEDOUT";
  }
  if( enumNum EQ 22)
  {
    return "UNKNOWNDESTINATIONADDRESS";
  }
  if( enumNum EQ 23)
  {
    return "NOROUTETOTARGET";
  }
  if( enumNum EQ 24)
  {
    return "INTERFACEINACTIVETOTARGET";
  }
  if( enumNum EQ 25)
  {
    return "ARPFAILURE";
  }
  if( enumNum EQ 26)
  {
    return "UNABLETORESOLVEDNSNAME";
  }
  if( enumNum EQ 27)
  {
    return "SCTPASSOCIATIONFAILED";
  }
  if( enumNum EQ 28)
  {
    return "SETIPADDRFAILED";
  }
  if( enumNum EQ 41)
  {
    return "RHCLOCKFAIL";
  }
  if( enumNum EQ 42)
  {
    return "RHHIGHTEMPERATURE";
  }
  if( enumNum EQ 43)
  {
    return "RHLOWTEMPERATURE";
  }
  if( enumNum EQ 44)
  {
    return "RHHIGHVOLTAGE";
  }
  if( enumNum EQ 45)
  {
    return "RHLOWVOLTAGE";
  }
  if( enumNum EQ 46)
  {
    return "RHCOMMONINTERNALERROR";
  }
  if( enumNum EQ 47)
  {
    return "RHOVERPOWER";
  }
  if( enumNum EQ 48)
  {
    return "VSWRFAIL";
  }
  if( enumNum EQ 49)
  {
    return "LOWPOWER";
  }
  if( enumNum EQ 50)
  {
    return "LOWGAIN";
  }
  if( enumNum EQ 51)
  {
    return "RHDISABLED";
  }
  if( enumNum EQ 52)
  {
    return "RHSHUTDOWN";
  }
  if( enumNum EQ 53)
  {
    return "LNAFAIL";
  }
  if( enumNum EQ 54)
  {
    return "RHPATHINTERNALERROR";
  }
  if( enumNum EQ 61)
  {
    return "CLOCKFAIL";
  }
  if( enumNum EQ 62)
  {
    return "OVERPOWER";
  }
  if( enumNum EQ 63)
  {
    return "TEMPERATURESENSORINACTIVE";
  }
  if( enumNum EQ 64)
  {
    return "CPRIINTERFACEDOWN";
  }
  if( enumNum EQ 65)
  {
    return "GPSRECVNOTRESPONDING";
  }
  if( enumNum EQ 66)
  {
    return "BACKHAULETHERNETLINKDOWN";
  }
  if( enumNum EQ 67)
  {
    return "DEBUGETHERNETLINKDOWN";
  }
  if( enumNum EQ 68)
  {
    return "SELFTESTFAIL";
  }
  if( enumNum EQ 81)
  {
    return "SWCRASH";
  }
  if( enumNum EQ 82)
  {
    return "NORESPONSETOHEALTHCHECK";
  }
  if( enumNum EQ 83)
  {
    return "QUEUESIZEEXCEEDED";
  }
  if( enumNum EQ 84)
  {
    return "CONFIGURATIONORCUSTOMIZATIONERROR";
  }
  if( enumNum EQ 85)
  {
    return "S1APGUARDTIMEEXPIRED";
  }
  if( enumNum EQ 86)
  {
    return "S1APRADIONWUNSPECIFIED";
  }
  if( enumNum EQ 87)
  {
    return "S1APRADIONWTX2RELOCOVERALLEXPIRY";
  }
  if( enumNum EQ 88)
  {
    return "S1APRADIONWSUCCESSFULHANDOVER";
  }
  if( enumNum EQ 89)
  {
    return "S1APRADIONWRELEASEDUETOEUTRANGENERATEDREASON";
  }
  if( enumNum EQ 90)
  {
    return "S1APRADIONWHANDOVERCANCELLED";
  }
  if( enumNum EQ 91)
  {
    return "S1APRADIONWPARTIALHANDOVER";
  }
  if( enumNum EQ 92)
  {
    return "S1APRADIONWHOFAILUREINTARGETEPCORTARGETSYSTEM";
  }
  if( enumNum EQ 93)
  {
    return "S1APRADIONWHOTARGETNOTALLOWED";
  }
  if( enumNum EQ 94)
  {
    return "S1APRADIONWTS1RELOCOVERALLEXPIRY";
  }
  if( enumNum EQ 95)
  {
    return "S1APRADIONWTS1RELOCPREPEXPIRY";
  }
  if( enumNum EQ 96)
  {
    return "S1APRADIONWCELLNOTAVAILABLE";
  }
  if( enumNum EQ 97)
  {
    return "S1APRADIONWUNKNOWNTARGETID";
  }
  if( enumNum EQ 98)
  {
    return "S1APRADIONWNORADIORESOURCESAVAILABLEINTARGETCELL";
  }
  if( enumNum EQ 99)
  {
    return "S1APRADIONWUNKNOWNMMEUES1APID";
  }
  if( enumNum EQ 100)
  {
    return "S1APRADIONWUNKNOWNENBUES1APID";
  }
  if( enumNum EQ 101)
  {
    return "S1APRADIONWUNKNOWNPAIRUES1APID";
  }
  if( enumNum EQ 102)
  {
    return "S1APRADIONWHANDOVERDESIRABLEFORRADIOREASON";
  }
  if( enumNum EQ 103)
  {
    return "S1APRADIONWTIMECRITICALHANDOVER";
  }
  if( enumNum EQ 104)
  {
    return "S1APRADIONWRESOURCEOPTIMISATIONHANDOVER";
  }
  if( enumNum EQ 105)
  {
    return "S1APRADIONWREDUCELOADINSERVINGCELL";
  }
  if( enumNum EQ 106)
  {
    return "S1APRADIONWUSERINACTIVITY";
  }
  if( enumNum EQ 107)
  {
    return "S1APRADIONWRADIOCONNECTIONWITHUELOST";
  }
  if( enumNum EQ 108)
  {
    return "S1APRADIONWLOADBALANCINGTAUREQUIRED";
  }
  if( enumNum EQ 109)
  {
    return "S1APRADIONWCSFALLBACKTRIGGERED";
  }
  if( enumNum EQ 110)
  {
    return "S1APRADIONWUENOTAVAILABLEFORPSSERVICE";
  }
  if( enumNum EQ 111)
  {
    return "S1APRADIONWRADIORESOURCESNOTAVAILABLE";
  }
  if( enumNum EQ 112)
  {
    return "S1APRADIONWFAILUREINRADIOINTERFACEPROCEDURE";
  }
  if( enumNum EQ 113)
  {
    return "S1APRADIONWINVALIDQOSCOMBINATION";
  }
  if( enumNum EQ 114)
  {
    return "S1APRADIONWINTERRATREDIRECTION";
  }
  if( enumNum EQ 115)
  {
    return "S1APRADIONWINTERACTIONWITHOTHERPROCEDURE";
  }
  if( enumNum EQ 116)
  {
    return "S1APRADIONWUNKNOWNERABID";
  }
  if( enumNum EQ 117)
  {
    return "S1APRADIONWMULTIPLEERABIDINSTANCES";
  }
  if( enumNum EQ 118)
  {
    return "S1APRADIONWENCRYPTIONANDORINTEGRITYPROTECTIONALGORITHMSNOTSUPPORTED";
  }
  if( enumNum EQ 119)
  {
    return "S1APRADIONWS1INTRASYSTEMHANDOVERTRIGGERED";
  }
  if( enumNum EQ 120)
  {
    return "S1APRADIONWS1INTERSYSTEMHANDOVERTRIGGERED";
  }
  if( enumNum EQ 121)
  {
    return "S1APRADIONWX2HANDOVERTRIGGERED";
  }
  if( enumNum EQ 122)
  {
    return "S1APRADIONWREDIRECTIONTOWARDS1XRTT";
  }
  if( enumNum EQ 123)
  {
    return "S1APRADIONWNOTSUPPORTEDQCIVALUE";
  }
  if( enumNum EQ 124)
  {
    return "S1APRADIONWINVALIDCSGID";
  }
  if( enumNum EQ 125)
  {
    return "S1APNASNORMALRELEASE";
  }
  if( enumNum EQ 126)
  {
    return "S1APNASAUTHENTICATIONFAILURE";
  }
  if( enumNum EQ 127)
  {
    return "S1APNASDETACH";
  }
  if( enumNum EQ 128)
  {
    return "S1APNASUNSPECIFIED";
  }
  if( enumNum EQ 129)
  {
    return "S1APNASCSGSUBCRIPTIONEXPIRY";
  }
  if( enumNum EQ 130)
  {
    return "S1APTPORTTRANSPORTRESOURCEUNAVAILABLE";
  }
  if( enumNum EQ 131)
  {
    return "S1APTPORTUNSPECIFIED";
  }
  if( enumNum EQ 132)
  {
    return "S1APPROTTRANSFERSYNTAXERROR";
  }
  if( enumNum EQ 133)
  {
    return "S1APPROTABSTRACTSYNTAXERRORREJECT";
  }
  if( enumNum EQ 134)
  {
    return "S1APPROTABSTRACTSYNTAXERRORIGNOREANDNOTIFY";
  }
  if( enumNum EQ 135)
  {
    return "S1APPROTMESSAGENOTCOMPATIBLEWITHRECEIVERSTATE";
  }
  if( enumNum EQ 136)
  {
    return "S1APPROTSEMANTICERROR";
  }
  if( enumNum EQ 137)
  {
    return "S1APPROTABSTRACTSYNTAXERRORFALSELYCONSTRUCTEDMESSAGE";
  }
  if( enumNum EQ 138)
  {
    return "S1APPROTUNSPECIFIED";
  }
  if( enumNum EQ 139)
  {
    return "S1APMISCCONTROLPROCESSINGOVERLOAD";
  }
  if( enumNum EQ 140)
  {
    return "S1APMISCNOTENOUGHUSERPLANEPROCESSINGRESOURCES";
  }
  if( enumNum EQ 141)
  {
    return "S1APMISCHARDWAREFAILURE";
  }
  if( enumNum EQ 142)
  {
    return "S1APMISCOMINTERVENTION";
  }
  if( enumNum EQ 143)
  {
    return "S1APMISCUNSPECIFIED";
  }
  if( enumNum EQ 144)
  {
    return "S1APMISCUNKNOWNPLMN";
  }
  if( enumNum EQ 145)
  {
    return "S1APMAXRETRYEXPIRED";
  }
  if( enumNum EQ 201)
  {
    return "MAXCONCURRENTLIMITREACHED";
  }
  if( enumNum EQ 202)
  {
    return "CPUOVERLOAD";
  }
  if( enumNum EQ 203)
  {
    return "MEMORYOVERLOAD";
  }
  if( enumNum EQ 500)
  {
    return "OTHER";
  }
 return "UNDEFINED_ENUM";
}

AlarmType strAlarmTypeToEnum(UINT8* alarmTypeStr)
{
  if( 0 EQ strncmp( alarmTypeStr, "ALARMTYPE_OTHER", strlen(alarmTypeStr)) )
  {
    return  ALARMTYPE_OTHER;
  }
  if( 0 EQ strncmp( alarmTypeStr, "ALARMTYPE_COMMUNICATIONSALARM", strlen(alarmTypeStr)) )
  {
    return  ALARMTYPE_COMMUNICATIONSALARM;
  }
  if( 0 EQ strncmp( alarmTypeStr, "ALARMTYPE_QUALITYOFSERVICEALARM", strlen(alarmTypeStr)) )
  {
    return  ALARMTYPE_QUALITYOFSERVICEALARM;
  }
  if( 0 EQ strncmp( alarmTypeStr, "ALARMTYPE_PROCESSINGERRORALARM", strlen(alarmTypeStr)) )
  {
    return  ALARMTYPE_PROCESSINGERRORALARM;
  }
  if( 0 EQ strncmp( alarmTypeStr, "ALARMTYPE_EQUIPMENTALARM", strlen(alarmTypeStr)) )
  {
    return  ALARMTYPE_EQUIPMENTALARM;
  }
  if( 0 EQ strncmp( alarmTypeStr, "ALARMTYPE_ENVIRONMENTALALARM", strlen(alarmTypeStr)) )
  {
    return  ALARMTYPE_ENVIRONMENTALALARM;
  }
  if( 0 EQ strncmp( alarmTypeStr, "ALARMTYPE_INTEGRITYVIOLATION", strlen(alarmTypeStr)) )
  {
    return  ALARMTYPE_INTEGRITYVIOLATION;
  }
  if( 0 EQ strncmp( alarmTypeStr, "ALARMTYPE_OPERATIONVIOLATION", strlen(alarmTypeStr)) )
  {
    return  ALARMTYPE_OPERATIONVIOLATION;
  }
  if( 0 EQ strncmp( alarmTypeStr, "ALARMTYPE_PHYSICALVIOLATION", strlen(alarmTypeStr)) )
  {
    return  ALARMTYPE_PHYSICALVIOLATION;
  }
  if( 0 EQ strncmp( alarmTypeStr, "ALARMTYPE_SECURITYSERVICEORMECHANISMVIOLATION", strlen(alarmTypeStr)) )
  {
    return  ALARMTYPE_SECURITYSERVICEORMECHANISMVIOLATION;
  }
  if( 0 EQ strncmp( alarmTypeStr, "ALARMTYPE_TIMEDOMAINVIOLATION", strlen(alarmTypeStr)) )
  {
    return  ALARMTYPE_TIMEDOMAINVIOLATION;
  }
  return  ALARMTYPE_OTHER; 
}

AlarmSeverity strAlarmSeverityToEnum(UINT8* alarmSeverityStr)
{
  if( 0 EQ strncmp( alarmSeverityStr, "ALARMSEVERITY_CLEARED", strlen(alarmSeverityStr)) )
  {
    return  ALARMSEVERITY_CLEARED;
  }
  if( 0 EQ strncmp( alarmSeverityStr, "ALARMSEVERITY_INDETERMINATE", strlen(alarmSeverityStr)) )
  {
    return  ALARMSEVERITY_INDETERMINATE;
  }
  if( 0 EQ strncmp( alarmSeverityStr, "ALARMSEVERITY_CRITICAL", strlen(alarmSeverityStr)) )
  {
    return  ALARMSEVERITY_CRITICAL;
  }
  if( 0 EQ strncmp( alarmSeverityStr, "ALARMSEVERITY_MAJOR", strlen(alarmSeverityStr)) )
  {
    return  ALARMSEVERITY_MAJOR;
  }
  if( 0 EQ strncmp( alarmSeverityStr, "ALARMSEVERITY_MINOR", strlen(alarmSeverityStr)) )
  {
    return  ALARMSEVERITY_MINOR;
  }
  if( 0 EQ strncmp( alarmSeverityStr, "ALARMSEVERITY_WARNING", strlen(alarmSeverityStr)) )
  {
    return  ALARMSEVERITY_WARNING;
  }
  return  ALARMSEVERITY_INDETERMINATE;
}
