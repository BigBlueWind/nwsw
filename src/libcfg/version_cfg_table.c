/**
 * @file    version_cfg_table.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains functions which will read version information of the
 *          package provided.
 *
 * @author          : Shweta Polepally
 * Creation Date   : Nov 2010
 * Change History  :
 *
 */

#include "all_cfg.h"


/* @param     fileToRead : This is the full file path of the configuration file which will be read
 * @param     cfgTable   : Output structure.
 * @details   This function will read Version Information and return the filled structure.
 *            If error, will report and exit from the function.
 *
 * */
NumEntry readFromFileVersionCfgContentsTable(
  IN UINT8                 *fileToRead,
  IO VersionCfgContents    *cfgTable,
  IO UINT8                 *error
  )
{
  config_t         cfg;
  config_setting_t *table;

  config_init(&cfg); /* init LibConfig */

   /* Read the file. If there is an error, report it and exit. */
   if(! config_read_file(&cfg, fileToRead))
   {
     fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
             config_error_line(&cfg), config_error_text(&cfg));
     config_destroy(&cfg);
     printf("\n Please enter valid Path for Version Information file \n");
     *error = TRUE;
     return FALSE;
   }

   table = config_lookup(&cfg, "version_info_table");
   if(table != NULL)
   {
     const char *builtOn = NULL;
     int swType=0,releaseVersion =0, featureVersion=0, bugFixVersion=0, customerId=0,svnVersion=0;

     config_setting_lookup_int(table, "swType", &swType );
     config_setting_lookup_int(table, "releaseVersion",  &releaseVersion );
     config_setting_lookup_int(table, "featureVersion",  &featureVersion );
     config_setting_lookup_int(table, "bugFixVersion",   &bugFixVersion );
     config_setting_lookup_int(table, "customerId",      &customerId );
     config_setting_lookup_int(table, "svnVersion",      &svnVersion );
     config_setting_lookup_string(table, "builtOn",  &builtOn );

     cfgTable->swType         = swType;
     cfgTable->releaseVersion = releaseVersion;
     cfgTable->featureVersion = featureVersion;
     cfgTable->bugFixVersion  = bugFixVersion;
     cfgTable->customerId     = customerId;
     cfgTable->svnVersion     = svnVersion;
     strncpy( cfgTable->builtOn, builtOn, strlen(builtOn) );
   }
   config_destroy(&cfg);
   return TRUE;
}
