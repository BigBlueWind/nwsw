/**
* @file    alarm_ids.h
* @details Copyright Powerwave Technologies, Inc., 2009
* @brief
* @brief This file contains all possible alarm IDs in the system.
 */
#ifndef ALARM_IDS_H
#define ALARM_IDS_H

#include "bs_tables.h"

typedef struct s_AlarmDb
{
  AlarmCause                  alarmCause;
  AlarmType                   alarmType;
  UINT32                      alarmId;
   /*cleared, indeterminate, critical, major, minor, warning*/
  UINT8                       alarmLocation[MAX_SIZE_ACTIVEALARMSTATUS_ALARMLOCATION + 1];
  TBOOL                       clearUponLockCmd;
  AlarmSeverity               alarmSeverity;
  BsErrorValue                bsErrorValue;
  MsgId                       clearingMsgId;
} AlarmDb;



#endif /* ALARM_IDS_H */
