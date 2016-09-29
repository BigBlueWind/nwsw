/**
 * @file    prm_read_mapping_table.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains functions which will read Process Module Mapping information of the
 *          package provided.
 *
 * @author          : Shweta Polepally
 * Creation Date   : Dec 2010
 * Change History  :
 *
 */

#include "prm.h"


/* @param     fileToRead : This is the full file path of the configuration file which will be read
 * @param     cfgTable   : Output structure.
 * @details   This function will read Version Information and return the filled structure.
 *            If error, will report and exit from the function.
 *
 * */
NumEntry readFromFileMappingCfgTable(
  IN UINT8                        *fileToRead,
  IO ProcessModuleMappingEntry    *cfgTable,
  IO UINT8                        *error
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

    setting = config_lookup(&cfg, "mapping_table");
    if(setting NOTEQ NULL)
    {
      int count = config_setting_length(setting);
      int        i;
      UINT32     modIdIndex = 0;
      NumEntry   modules = 0;

      for(i = 0; i < count; ++i)
      {
        config_setting_t *table = config_setting_get_elem(setting, i);
        /* Only output the record if all of the expected fields are present. */
        const char *moduleName = NULL;
        const char *exeName = NULL;
        config_setting_lookup_string(table, "moduleName", &moduleName );
        config_setting_lookup_string(table, "exeName", &exeName );
        for(modIdIndex = 0 ; modIdIndex < MODID_MAX ; modIdIndex++)
        {
          /* TODO: can be optimised: read ModuleId as number rather than string */
          if(strcmp(getModuleName(modIdIndex), moduleName) EQ 0)
          {
            cfgTable->moduleName  = modIdIndex;
            strncpy( cfgTable->exeName, exeName, strlen(exeName) );
            /* Pid is not read from the configuration file, although entry is there  */
            modules++;
            break;
          }
        }
        cfgTable++;
      }
      config_destroy(&cfg);
      return modules;
    }
    config_destroy(&cfg);
    return FALSE;
}


