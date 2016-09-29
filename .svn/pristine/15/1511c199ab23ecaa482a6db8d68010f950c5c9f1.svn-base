/**
 * @file    module_stop_cfg_table.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains functions which will read the default configuration
 *          for module Stop table - Sequence in which Modules will be stopped.
 *
 * @author          : Shweta Polepally
 * Creation Date   : 06th July 2010
 * Change History  :
 *
 */

#include "all_cfg.h"

/*
 * @param     fileToRead : This is the full file path of the configuration file which will be read
 * @details   This function will read process configuration file and return the filled process
 *            information about which Exe's to start.
 *            If error, will report and exit from the function.
 * */
NumEntry readFromFileModuleStopTable(
  IN UINT8                                       *fileToRead,
  IO ModuleStop                                  *ptrModuleStop,
  IO UINT8                                       *error
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
     printf("\n Please enter valid Path for process Configuration file \n");
     *error = TRUE;
     return FALSE;
   }

   setting = config_lookup(&cfg, "module_stop_table");
   if(setting NOTEQ NULL)
   {
     int count = config_setting_length(setting);
     int        i;

     for(i = 0; i < count; ++i)
     {
       config_setting_t *table = config_setting_get_elem(setting, i);
       /* Only output the record if all of the expected fields are present. */
       const char *moduleName = NULL;
       const char *moduleDesc = NULL;
       config_setting_lookup_string(table, "moduleName", &moduleName );
       ptrModuleStop->moduleIdString = strModIdToEnum((UINT8*)moduleName);
       config_setting_lookup_string(table, "moduleDesc", &moduleDesc );
       strncpy( ptrModuleStop->moduleDesc, moduleDesc, strlen(moduleDesc));
       config_setting_lookup_int(table, "exitPriority", (int*)&ptrModuleStop->exitPriority);
       ptrModuleStop++;
     }
     config_destroy(&cfg);
     return count;
   }
   config_destroy(&cfg);
   return FALSE;
}
