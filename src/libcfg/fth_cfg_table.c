/**
 * @file    fth_cfg_table.c
 * @brief This file is contains the code common across all features of FTH
 *          It also functions will calls config reading function.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author        : Ankur Agrawal
 * Date          : 14-Dec-2010
 * Change History:
 *
 */
#include "all_cfg.h"

/* @param     fileToRead : This is the full file path of the configuration file which will be read
 * @details   This function will read alarm configuration file and return the filled structure.
 *            If error, will report and exit from the function.
 *
 * */
void readFthConfigTable(
  IN UINT8         *fileToRead,
  IO FthCfgTable   *cfgTable,
  IO UINT8         *error
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
     return;
   }
   table = config_lookup(&cfg, "fth_cfg_table");
   if(table NOTEQ NULL)
   {
      const char *downloadDir = NULL;
      int connTimeOutInterval = 0, downloadTimeOutInterval = 0 , uploadTimeOutInterval = 0, maxFileSize = 0, maxDownloadDirSize = 0;

      config_setting_lookup_string(table, "downloadDir",         &downloadDir );
      config_setting_lookup_int(table, "connTimeOutInterval",    &connTimeOutInterval );
      config_setting_lookup_int(table, "downloadTimeOutInterval",&downloadTimeOutInterval );
      config_setting_lookup_int(table, "uploadTimeOutInterval",  &uploadTimeOutInterval );
      config_setting_lookup_int(table, "maxFileSize",            &maxFileSize );
      config_setting_lookup_int(table, "maxDownloadDirSize",     &maxDownloadDirSize );

      strncpy( cfgTable->downloadDir, downloadDir, strlen(downloadDir));
      cfgTable->connTimeOutInterval     = connTimeOutInterval;
      cfgTable->downloadTimeOutInterval = downloadTimeOutInterval;
      cfgTable->uploadTimeOutInterval   = uploadTimeOutInterval;
      cfgTable->maxFileSize             = maxFileSize;
      cfgTable->maxDownloadDirSize      = maxDownloadDirSize;
   }
}
