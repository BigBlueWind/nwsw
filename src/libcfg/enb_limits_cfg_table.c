/**
 * @file    enb_limits_cfg_table.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains functions which will read enb limits information.
 *
 * @author          : Shweta Polepally
 * Creation Date   : Nov 2010
 * Change History  :
 *
 */


#include "all_cfg.h"

/**
 * @param     fileToRead : This is the full file path of the configuration file which will be read
 * @details   This function will read input configuration file and return the filled default
 *            configuration details to the caller of this function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *            Return 1 if ERROR is there otherwise 0
 **/
TBOOL readFromFileEnbLimitsConfigTable(
  IN UINT8                   *fileToRead,
  IO EnbLimitsConfig         *enbLimitsConfig,
  IO UINT8                   *error
  )
{
  config_t         cfg;
  config_setting_t *table;
  config_init(&cfg); /* init LibConfig */
  *error = FALSE;
  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&cfg, fileToRead))
   {
     fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
     *error = TRUE;
     config_destroy(&cfg);
     return FALSE;
   }
  table = config_lookup(&cfg, "enbLimitsDefaultCfg");
  if(table NOTEQ NULL)
   {
     config_setting_lookup_int(table, "maxTraceFileSize", &enbLimitsConfig->maxTraceFileSize);
     config_setting_lookup_int(table, "maxCombinedPmLogFileSize", &enbLimitsConfig->maxCombinedPmLogFileSize);
   }
     config_destroy(&cfg);
     return TRUE;
}
