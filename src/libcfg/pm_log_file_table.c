/**
 * @file    pm_log_file_table.c
 * @details Copyright Powerwave Technologies, Inc., 2011
 * @brief This file contains reader and writer functions for pm log file status table
 *
 * @author:  Shweta Polepally
 * Date:    8 April 2011
 * Description: Draft version
 **/

#include  "all_cfg.h"

/**
 * @param     fileToRead : This is the full file path of the configuration file which will be read
 * @details   This function will read input configuration file and return the filled structure
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *            Return 1 if ERROR is there otherwise 0 in pointer to error given by caller
 *            return argument is numbers of configuration read
 **/
NumEntry readFromFilePmLogFileStatusTable(
  IN UINT8                   *fileToRead,
  IO PmLogFileStatus         *ptrPmLogFileStatus,
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
    printf("\n Please enter valid Path for PM Log file \n");
    *error = TRUE;
    return FALSE;
  }
  setting = config_lookup(&cfg, "pm_log_file_table");
  if(setting NOTEQ NULL)
  {
    int  count = config_setting_length(setting);
    int  numElem = 0;
    for(numElem = 0; numElem < count; ++numElem)
    {
      config_setting_t *table = config_setting_get_elem(setting, numElem);
      const char *fileName;
      const char *openedAt;
      const char *closedAt;

      config_setting_lookup_int(table, "index", &ptrPmLogFileStatus->index);
      config_setting_lookup_string(table, "fileName", &fileName);
      strncpy(ptrPmLogFileStatus->fileName,fileName, strlen(fileName));
      config_setting_lookup_int(table, "fileSizeInBytes", &ptrPmLogFileStatus->fileSizeInBytes);
      config_setting_lookup_string(table, "openedAt", &openedAt);
      strncpy(ptrPmLogFileStatus->openedAt,openedAt, strlen(openedAt));
      config_setting_lookup_string(table, "closedAt", &closedAt);
      strncpy(ptrPmLogFileStatus->closedAt,closedAt, strlen(closedAt));
      ptrPmLogFileStatus++;
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
TBOOL writeToFilePmLogFileStatusTable(
  IN PmLogFileStatus         *ptrPmLogFileStatus,
  IO UINT8                   *fileToWrite,
  IO UINT8                   *error
  )
{
  config_t         readCfg;/* Read RO values into local structure use them while writing new cfg */
  config_setting_t *readTable;
  config_t         writeCfg;
  config_setting_t *root, *setting, *group, *pmLogEntry;
  config_init(&writeCfg); /* init LibConfig */
  config_init(&readCfg); /* init LibConfig */
  *error = FALSE;

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&readCfg, fileToWrite))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&readCfg),config_error_line(&readCfg), config_error_text(&readCfg));
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("\n Please enter valid Path for PM LOG file \n");
    *error = TRUE;
    return FALSE;
  }
  readTable = config_lookup(&readCfg, "pm_log_file_table");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf(" \n Table doesn't exist\n");
    return FALSE;
  }
  else
  {

    root = config_root_setting(&writeCfg);
    group = config_setting_add(root, "pm_log_file_table", CONFIG_TYPE_LIST);

    int  count = config_setting_length(readTable);
    int  numElem = 0;
    for(numElem = 0; numElem < count; ++numElem)
    {
    /* Create the new entry. */
      pmLogEntry = config_setting_add(group, NULL, CONFIG_TYPE_GROUP);

      setting = config_setting_add(pmLogEntry, "index", CONFIG_TYPE_INT);
      config_setting_set_int(setting, ptrPmLogFileStatus->index);

      setting = config_setting_add(pmLogEntry, "fileName", CONFIG_TYPE_STRING);
      config_setting_set_string(setting, (char*)&ptrPmLogFileStatus->fileName);

      setting = config_setting_add(pmLogEntry, "fileSizeInBytes", CONFIG_TYPE_INT);
      config_setting_set_int(setting, ptrPmLogFileStatus->fileSizeInBytes);

      setting = config_setting_add(pmLogEntry, "openedAt", CONFIG_TYPE_STRING);
      config_setting_set_string(setting, (char*)&ptrPmLogFileStatus->openedAt);

      setting = config_setting_add(pmLogEntry, "closedAt", CONFIG_TYPE_STRING);
      config_setting_set_string(setting, (char*)&ptrPmLogFileStatus->closedAt);

      ptrPmLogFileStatus++;
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
  return TRUE;
}
