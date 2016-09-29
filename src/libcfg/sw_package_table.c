/**
 * @file    sw_info_table.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This is auto generated file from Excel
 * @brief This file contains reader and writer functions for all Multi row Table configurations
 *
 * @author: Lalit Chhabra
 * Date: 30-11-2010
 * Description: Draft version 
 * Generated on : 12/20/2010 12:26:41 PM
 **/

#include  "all_cfg.h"


/**
 * @param     fileToRead : This is the full file path of the configuration file which will be read
 * @details   This function will read input configuration file and return the filled structure
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *            Return 1 if ERROR is there otherwise 0 in pointer to error given by caller
 *            return argument is numbers of configuration read
 **/
NumEntry readFromFileSwPackageStatusTable(
  IN UINT8                   *fileToRead, 
  IO SwPackageStatus         *ptrSwPackageStatus,
  IO UINT8                   *error
  )
{
  config_t         cfg;
  config_setting_t *setting;
  config_init(&cfg); /* init LibConfig */
  *error = FALSE;
  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&cfg, fileToRead))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    printf("\n Please enter valid Path for Software Information file \n");
    *error = TRUE;
    return FALSE;
  }
  setting = config_lookup(&cfg, "sw_package_table");
  if(setting NOTEQ NULL)
  {
    int  count = config_setting_length(setting);
    int  numElem = 0;
    for(numElem = 0; numElem < count; ++numElem)
    {
      config_setting_t *table = config_setting_get_elem(setting, numElem);
      const char *version;
      const char *builtOn;
      const char *swNotes;
      const char *packageDir;

      config_setting_lookup_int(table, "index", (int*)&ptrSwPackageStatus->index);
      config_setting_lookup_int(table, "swType", (int*)&ptrSwPackageStatus->swType);
      config_setting_lookup_string(table, "version", &version); 
      strncpy(ptrSwPackageStatus->version,version, strlen(version));
      config_setting_lookup_string(table, "builtOn", &builtOn); 
      strncpy(ptrSwPackageStatus->builtOn,builtOn, strlen(builtOn));
      config_setting_lookup_string(table, "swNotes", &swNotes);
      strncpy(ptrSwPackageStatus->swNotes,swNotes, strlen(swNotes));
      config_setting_lookup_int(table, "isRunning", (int*)&ptrSwPackageStatus->isRunning);
      config_setting_lookup_string(table, "packageDir", &packageDir); 
      strncpy(ptrSwPackageStatus->packageDir,packageDir, strlen(packageDir));
      ptrSwPackageStatus++;
    }
    config_destroy(&cfg);
    return count;
  }
  else
  {
     fprintf(stderr, "Setting  Not present : %s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
     *error = TRUE;
  }
  config_destroy(&cfg);
  return FALSE;
}


/**
 * @param    fileToWrite : This is the full file path of the configuration file which will be written
 * @details   This function will read input configuration table pinter and fill 
 *            configurations to file fileToWrite given as input by caller function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *
 **/
void writeToFileSwPackageStatusTable(
  IN SwPackageStatus         *ptrSwPackageStatus,
  IO UINT8                   *fileToWrite,
  IO UINT8                   *error
  )
{
  config_t         readCfg;/* Read RO values into local structure use them while writing new cfg */
  config_setting_t *readTable;
  config_t         writeCfg;
  config_setting_t *root, *setting, *group, *swEntry;
  config_init(&writeCfg); /* init LibConfig */
  config_init(&readCfg); /* init LibConfig */
  *error = FALSE;

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&readCfg, fileToWrite))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&readCfg),config_error_line(&readCfg), config_error_text(&readCfg));
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("\n Please enter valid Path for Software Information file \n");
    *error = TRUE;
    return;
  }
  readTable = config_lookup(&readCfg, "sw_package_table");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf(" \n Table doesn't exist\n");
    return;
  }
  else
  {
    root = config_root_setting(&writeCfg);
    group = config_setting_add(root, "sw_package_table", CONFIG_TYPE_LIST);

    int  count = config_setting_length(readTable);
    int  numElem = 0;
    for(numElem = 0; numElem < count; ++numElem)
    {
    /* Create the new entry. */
      swEntry = config_setting_add(group, NULL, CONFIG_TYPE_GROUP);

      setting = config_setting_add(swEntry, "index", CONFIG_TYPE_INT);
      config_setting_set_int(setting, ptrSwPackageStatus->index);

      setting = config_setting_add(swEntry, "swType", CONFIG_TYPE_INT);
      config_setting_set_int(setting, ptrSwPackageStatus->swType);

      setting = config_setting_add(swEntry, "version", CONFIG_TYPE_STRING);
      config_setting_set_string(setting, (char*)&ptrSwPackageStatus->version);

      setting = config_setting_add(swEntry, "builtOn", CONFIG_TYPE_STRING);
      config_setting_set_string(setting, (char*)&ptrSwPackageStatus->builtOn);

      setting = config_setting_add(swEntry, "swNotes", CONFIG_TYPE_STRING);
      config_setting_set_string(setting, (char*)&ptrSwPackageStatus->swNotes);

      setting = config_setting_add(swEntry, "isRunning", CONFIG_TYPE_INT);
      config_setting_set_int(setting, ptrSwPackageStatus->isRunning);

      setting = config_setting_add(swEntry, "packageDir", CONFIG_TYPE_STRING);
      config_setting_set_string(setting, (char*)&ptrSwPackageStatus->packageDir);

      ptrSwPackageStatus++;
    }
  }
  /* Write the file. If there is an error, report it and exit. */
  if(! config_write_file(&writeCfg, fileToWrite))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&writeCfg),config_error_line(&writeCfg), config_error_text(&writeCfg));
    *error = TRUE;
  }
  config_destroy(&writeCfg);
  config_destroy(&readCfg);
  return;
}
