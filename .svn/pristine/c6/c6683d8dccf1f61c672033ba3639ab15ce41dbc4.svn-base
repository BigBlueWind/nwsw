/**
 * @file    alarm_db.c
 * @details Copyright Powerwave Technologies, Inc., 2011
 * @brief This file contains functions which reads alarm database config file
 *
 * @author          : Shweta Polepally
 * Creation Date   : Feb 2011
 * Change History  :
 *
 */

#include "all_cfg.h"


/* @param     fileToRead : This is the full file path of the configuration file which will be read
 * @param     cfgTable   : Output structure.
 * @details   This function will read alarm database config file and return the filled structure.
 *            If error, will report and exit from the function.
 *
 * */
NumEntry readFromFileAlarmDb(
  IN UINT8              *fileToRead,
  IO AlarmDb            *cfgTable,
  IO UINT8              *error
  )
{
  config_t         cfg;
  config_setting_t *setting;

  config_init(&cfg); /* init LibConfig */

   /* Read the file. If there is an error, report it and exit. */
   if(! config_read_file(&cfg, fileToRead))
   {
     fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
             config_error_line(&cfg), config_error_text(&cfg));
     config_destroy(&cfg);
     logInfo("%s","Please enter valid Path for Alarm DB Configuration file \n");
     *error = TRUE;
     return FALSE;
   }

   setting = config_lookup(&cfg, "alarm_db");
   if(setting NOTEQ NULL)
   {
     int  count = config_setting_length(setting);
     int  numElem = 0;
     for(numElem = 0; numElem < count; ++numElem)
     {
       config_setting_t *table = config_setting_get_elem(setting, numElem);
       const char *alarmType = NULL, *alarmLocation = NULL, *alarmSeverity = NULL;
       int alarmCause = 0, alarmId = 0, clearingMsgId = 0, clearUponLockCmd = 0;
       int bsErrorValue = 0;

       config_setting_lookup_int(table, "alarmCause", &alarmCause );
       config_setting_lookup_string(table, "alarmType",  &alarmType );
       config_setting_lookup_int(table, "alarmId", &alarmId );
       config_setting_lookup_string(table, "alarmLocation",  &alarmLocation );
       config_setting_lookup_int(table, "clearUponLockCmd", &clearUponLockCmd );
       config_setting_lookup_string(table, "alarmSeverity",  &alarmSeverity );
       config_setting_lookup_int(table, "bsError", &bsErrorValue );
       config_setting_lookup_int(table, "clearingMsgId", &clearingMsgId );

       cfgTable->alarmCause         = alarmCause;
       cfgTable->alarmId            = alarmId;
       cfgTable->bsErrorValue       = bsErrorValue;
       cfgTable->clearingMsgId      = clearingMsgId;
       cfgTable->alarmType          = strAlarmTypeToEnum((UINT8*)alarmType);
       cfgTable->clearUponLockCmd   = clearUponLockCmd;
       cfgTable->alarmSeverity      = strAlarmSeverityToEnum((UINT8*)alarmSeverity);
       snprintf( cfgTable->alarmLocation, MAX_SIZE_ACTIVEALARMSTATUS_ALARMLOCATION+1, "%s", alarmLocation );
       cfgTable++;
     }
     config_destroy(&cfg);
     return count;
   }
   config_destroy(&cfg);
   return FALSE;
}
