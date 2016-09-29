/**
 * @file    pre_config_read_write.c
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This is auto generated file from Excel
 * @section This file contains reader and writer functions for all Tables configurations
 *
 * Author: Lalit Chhabra
 * Date: 12/30/2010 
 * Description: Draft version 
 *              Changed Function Names for read and write -- madhu 11/10/2010  
 * Generated on : 11/29/2012 12:22:21 PM
 **/

#include  "pre_config_read_write.h"


/**
 * @param     fileToRead : This is the full file path of the configuration file which will be read
 * @section   This function will read input configuration file and return the filled default
 *            configuration details to the caller of this function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *            Return 1 if ERROR is there otherwise 0
 **/
NumEntry readFromFileMsgTraceTable(
  IN UINT8                   *fileToRead, 
  IO MsgTrace                *ptrMsgTrace,
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
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
    config_destroy(&cfg);
    return FALSE;
  }
  setting = config_lookup(&cfg, "msgTrace");
  if(setting NOTEQ NULL)
  {
    int count = config_setting_length(setting);
    int       numElem;
    for(numElem = 0;numElem < count; ++numElem)
    {

      config_setting_t *table = config_setting_get_elem(setting, numElem);
      /* only output the record if all of the expected fields are present */

      config_setting_lookup_int(table, "msgId", (int*)&ptrMsgTrace->msgId); 
      config_setting_lookup_int(table, "msgTraceLevel", (int*)&ptrMsgTrace->msgTraceLevel); 
      config_setting_lookup_int(table, "msgTraceDirection", (int*)&ptrMsgTrace->msgTraceDirection); 
      ptrMsgTrace++;
    }
    config_destroy(&cfg);
    return count;
  }
  else
  {
    fprintf(stderr, "Setting Not present : %s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
  }
  config_destroy(&cfg);
  return FALSE;
}


/**
 * @param    fileToWrite : This is the full file path of the configuration file which will be written
 * @section   This function will read Cell configuration table and fill default
 *            configurations to file fileToWrite given as input by caller function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *
 **/
TBOOL writeToFileMsgTraceTable(
  IN MsgTrace                *ptrMsgTrace,
  IO UINT8                   *fileToWrite,
  IO UINT8                   *error,
  IO FILE                    *fp
  )
{
  config_t         readCfg;/* Read RO values into local structure use them while writing new cfg */
  config_setting_t *readTable;
  config_t         writeCfg;
  config_setting_t *root, *setting, *group, *cfgEntry;
  config_init(&writeCfg); /* init LibConfig */
  config_init(&readCfg); /* init LibConfig */
  *error = FALSE;

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&readCfg, fileToWrite))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&readCfg),config_error_line(&readCfg), config_error_text(&readCfg));
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    return FALSE;
  }
  readTable = config_lookup(&readCfg, "msgTrace");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table msgTrace doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "msgTrace", CONFIG_TYPE_LIST);

  int count = config_setting_length(readTable);
  int numElem = 0;
  for(numElem = 0; numElem < count; ++numElem)
  {
    /* Create the new entry */
    cfgEntry = config_setting_add(group, NULL, CONFIG_TYPE_GROUP);

    setting = config_setting_add(cfgEntry, "msgId", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrMsgTrace->msgId);
    setting = config_setting_add(cfgEntry, "msgTraceLevel", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrMsgTrace->msgTraceLevel);
    setting = config_setting_add(cfgEntry, "msgTraceDirection", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrMsgTrace->msgTraceDirection);
    ptrMsgTrace++;
  }

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);

  config_destroy(&writeCfg);
  config_destroy(&readCfg);
  return TRUE;
}

/**
 * @param     fileToRead : This is the full file path of the configuration file which will be read
 * @section   This function will read input configuration file and return the filled default
 *            configuration details to the caller of this function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *            Return 1 if ERROR is there otherwise 0
 **/
NumEntry readFromFileTimersConfigTable(
  IN UINT8                   *fileToRead, 
  IO TimersConfig            *ptrTimersConfig,
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
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
    config_destroy(&cfg);
    return FALSE;
  }
  setting = config_lookup(&cfg, "timersConfig");
  if(setting NOTEQ NULL)
  {
    int count = config_setting_length(setting);
    int       numElem;
    for(numElem = 0;numElem < count; ++numElem)
    {
      const char *description;

      config_setting_t *table = config_setting_get_elem(setting, numElem);
      /* only output the record if all of the expected fields are present */

      config_setting_lookup_int(table, "timerName", (int*)&ptrTimersConfig->timerName); 
      config_setting_lookup_int(table, "timeUnit", (int*)&ptrTimersConfig->timeUnit); 
      config_setting_lookup_int(table, "duration", (int*)&ptrTimersConfig->duration); 
      config_setting_lookup_string(table, "description", &description); 
      strncpy(ptrTimersConfig->description,description, MAX_SIZE_TIMERSCONFIG_DESCRIPTION );
      config_setting_lookup_int(table, "moduleId", (int*)&ptrTimersConfig->moduleId); 
      ptrTimersConfig++;
    }
    config_destroy(&cfg);
    return count;
  }
  else
  {
    fprintf(stderr, "Setting Not present : %s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
  }
  config_destroy(&cfg);
  return FALSE;
}


/**
 * @param    fileToWrite : This is the full file path of the configuration file which will be written
 * @section   This function will read Cell configuration table and fill default
 *            configurations to file fileToWrite given as input by caller function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *
 **/
TBOOL writeToFileTimersConfigTable(
  IN TimersConfig            *ptrTimersConfig,
  IO UINT8                   *fileToWrite,
  IO UINT8                   *error,
  IO FILE                    *fp
  )
{
  config_t         readCfg;/* Read RO values into local structure use them while writing new cfg */
  config_setting_t *readTable;
  config_t         writeCfg;
  config_setting_t *root, *setting, *group, *cfgEntry;
  config_init(&writeCfg); /* init LibConfig */
  config_init(&readCfg); /* init LibConfig */
  *error = FALSE;

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&readCfg, fileToWrite))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&readCfg),config_error_line(&readCfg), config_error_text(&readCfg));
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    return FALSE;
  }
  readTable = config_lookup(&readCfg, "timersConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table timersConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "timersConfig", CONFIG_TYPE_LIST);

  int count = config_setting_length(readTable);
  int numElem = 0;
  for(numElem = 0; numElem < count; ++numElem)
  {
    /* Create the new entry */
    cfgEntry = config_setting_add(group, NULL, CONFIG_TYPE_GROUP);

    setting = config_setting_add(cfgEntry, "timerName", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrTimersConfig->timerName);
    setting = config_setting_add(cfgEntry, "timeUnit", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrTimersConfig->timeUnit);
    setting = config_setting_add(cfgEntry, "duration", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrTimersConfig->duration);
    setting = config_setting_add(cfgEntry, "description", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrTimersConfig->description);
    setting = config_setting_add(cfgEntry, "moduleId", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrTimersConfig->moduleId);
    ptrTimersConfig++;
  }

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);

  config_destroy(&writeCfg);
  config_destroy(&readCfg);
  return TRUE;
}

/**
 * @param     fileToRead : This is the full file path of the configuration file which will be read
 * @section   This function will read input configuration file and return the filled default
 *            configuration details to the caller of this function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *            Return 1 if ERROR is there otherwise 0
 **/
void readFromFileEventThresholdsTable(
  IN UINT8                   *fileToRead, 
  IO EventThresholds         *ptrEventThresholds,
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
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
    config_destroy(&cfg);
    return;
  }
  table = config_lookup(&cfg, "eventThresholds");
  if(table NOTEQ NULL)
  {

    config_setting_lookup_int(table, "index", (int*)&ptrEventThresholds->index); 
    config_setting_lookup_int(table, "cpuUsageHigh", (int*)&ptrEventThresholds->cpuUsageHigh); 
    config_setting_lookup_int(table, "cpuUsageNormal", (int*)&ptrEventThresholds->cpuUsageNormal); 
    config_setting_lookup_int(table, "memUsageHigh", (int*)&ptrEventThresholds->memUsageHigh); 
    config_setting_lookup_int(table, "memUsageNormal", (int*)&ptrEventThresholds->memUsageNormal); 
    config_setting_lookup_int(table, "enbLoadHighMbps", (int*)&ptrEventThresholds->enbLoadHighMbps); 
    config_setting_lookup_int(table, "enbLoadNormalMbps", (int*)&ptrEventThresholds->enbLoadNormalMbps); 
  }
  else
  {
    fprintf(stderr, "Table Not present : %s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
  }
  config_destroy(&cfg);
}


/**
 * @param    fileToWrite : This is the full file path of the configuration file which will be written
 * @section   This function will read Cell configuration table and fill default
 *            configurations to file fileToWrite given as input by caller function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *
 **/
TBOOL writeToFileEventThresholdsTable(
  IN EventThresholds         *ptrEventThresholds,
  IO UINT8                   *fileToWrite,
  IO UINT8                   *error,
  IO FILE                    *fp
  )
{
  config_t         readCfg;/* Read RO values into local structure use them while writing new cfg */
  config_setting_t *readTable;
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */
  config_init(&readCfg); /* init LibConfig */
  *error = FALSE;

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&readCfg, fileToWrite))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&readCfg),config_error_line(&readCfg), config_error_text(&readCfg));
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    return FALSE;
  }
  readTable = config_lookup(&readCfg, "eventThresholds");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table eventThresholds doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "eventThresholds", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "index", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEventThresholds->index);
  setting = config_setting_add(group, "cpuUsageHigh", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEventThresholds->cpuUsageHigh);
  setting = config_setting_add(group, "cpuUsageNormal", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEventThresholds->cpuUsageNormal);
  setting = config_setting_add(group, "memUsageHigh", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEventThresholds->memUsageHigh);
  setting = config_setting_add(group, "memUsageNormal", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEventThresholds->memUsageNormal);
  setting = config_setting_add(group, "enbLoadHighMbps", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEventThresholds->enbLoadHighMbps);
  setting = config_setting_add(group, "enbLoadNormalMbps", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEventThresholds->enbLoadNormalMbps);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);

  config_destroy(&writeCfg);
  config_destroy(&readCfg);
  return TRUE;
}

/**
 * @param     fileToRead : This is the full file path of the configuration file which will be read
 * @section   This function will read input configuration file and return the filled default
 *            configuration details to the caller of this function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *            Return 1 if ERROR is there otherwise 0
 **/
void readFromFileEnbAddressesTable(
  IN UINT8                   *fileToRead, 
  IO EnbAddresses            *ptrEnbAddresses,
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
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
    config_destroy(&cfg);
    return;
  }
  table = config_lookup(&cfg, "enbAddresses");
  if(table NOTEQ NULL)
  {
    const char *backhaulPortMacAddress;
    const char *debugPortMacAddress;
    const char *uniqueProductId;
    const char *staticIPAddressUsedAtDebugPort;
    const char *netmaskForDebugPort;
    const char *radioHeadIpAddress;
    const char *radioHeadMacAddress;

    config_setting_lookup_string(table, "backhaulPortMacAddress", &backhaulPortMacAddress); 
    strncpy(ptrEnbAddresses->backhaulPortMacAddress,backhaulPortMacAddress, MAX_SIZE_ENBADDRESSES_BACKHAULPORTMACADDRESS );
    config_setting_lookup_string(table, "debugPortMacAddress", &debugPortMacAddress); 
    strncpy(ptrEnbAddresses->debugPortMacAddress,debugPortMacAddress, MAX_SIZE_ENBADDRESSES_DEBUGPORTMACADDRESS );
    config_setting_lookup_string(table, "uniqueProductId", &uniqueProductId); 
    strncpy(ptrEnbAddresses->uniqueProductId,uniqueProductId, MAX_SIZE_ENBADDRESSES_UNIQUEPRODUCTID );
    config_setting_lookup_string(table, "staticIPAddressUsedAtDebugPort", &staticIPAddressUsedAtDebugPort); 
    strncpy(ptrEnbAddresses->staticIPAddressUsedAtDebugPort,staticIPAddressUsedAtDebugPort, MAX_SIZE_ENBADDRESSES_STATICIPADDRESSUSEDATDEBUGPORT );
    config_setting_lookup_string(table, "netmaskForDebugPort", &netmaskForDebugPort); 
    strncpy(ptrEnbAddresses->netmaskForDebugPort,netmaskForDebugPort, MAX_SIZE_ENBADDRESSES_NETMASKFORDEBUGPORT );
    config_setting_lookup_string(table, "radioHeadIpAddress", &radioHeadIpAddress); 
    strncpy(ptrEnbAddresses->radioHeadIpAddress,radioHeadIpAddress, MAX_SIZE_ENBADDRESSES_RADIOHEADIPADDRESS );
    config_setting_lookup_string(table, "radioHeadMacAddress", &radioHeadMacAddress); 
    strncpy(ptrEnbAddresses->radioHeadMacAddress,radioHeadMacAddress, MAX_SIZE_ENBADDRESSES_RADIOHEADMACADDRESS );
  }
  else
  {
    fprintf(stderr, "Table Not present : %s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
  }
  config_destroy(&cfg);
}


/**
 * @param    fileToWrite : This is the full file path of the configuration file which will be written
 * @section   This function will read Cell configuration table and fill default
 *            configurations to file fileToWrite given as input by caller function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *
 **/
TBOOL writeToFileEnbAddressesTable(
  IN EnbAddresses            *ptrEnbAddresses,
  IO UINT8                   *fileToWrite,
  IO UINT8                   *error,
  IO FILE                    *fp
  )
{
  config_t         readCfg;/* Read RO values into local structure use them while writing new cfg */
  config_setting_t *readTable;
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */
  config_init(&readCfg); /* init LibConfig */
  *error = FALSE;

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&readCfg, fileToWrite))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&readCfg),config_error_line(&readCfg), config_error_text(&readCfg));
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    return FALSE;
  }
  readTable = config_lookup(&readCfg, "enbAddresses");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table enbAddresses doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "enbAddresses", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "backhaulPortMacAddress", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbAddresses->backhaulPortMacAddress);
  setting = config_setting_add(group, "debugPortMacAddress", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbAddresses->debugPortMacAddress);
  setting = config_setting_add(group, "uniqueProductId", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbAddresses->uniqueProductId);
  setting = config_setting_add(group, "staticIPAddressUsedAtDebugPort", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbAddresses->staticIPAddressUsedAtDebugPort);
  setting = config_setting_add(group, "netmaskForDebugPort", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbAddresses->netmaskForDebugPort);
  setting = config_setting_add(group, "radioHeadIpAddress", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbAddresses->radioHeadIpAddress);
  setting = config_setting_add(group, "radioHeadMacAddress", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbAddresses->radioHeadMacAddress);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);

  config_destroy(&writeCfg);
  config_destroy(&readCfg);
  return TRUE;
}

/**
 * @param     fileToRead : This is the full file path of the configuration file which will be read
 * @section   This function will read input configuration file and return the filled default
 *            configuration details to the caller of this function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *            Return 1 if ERROR is there otherwise 0
 **/
void readFromFileUdhConfigTable(
  IN UINT8                   *fileToRead, 
  IO UdhConfig               *ptrUdhConfig,
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
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
    config_destroy(&cfg);
    return;
  }
  table = config_lookup(&cfg, "udhConfig");
  if(table NOTEQ NULL)
  {
    int         ulInactivityIntervalInSecs;
    int         ueThroughtputTimerInSecs;
    int         memoryDiagCfg;

    config_setting_lookup_int(table, "ulInactivityIntervalInSecs", &ulInactivityIntervalInSecs); 
    ptrUdhConfig->ulInactivityIntervalInSecs = ulInactivityIntervalInSecs;
    config_setting_lookup_int(table, "ueThroughtputTimerInSecs", &ueThroughtputTimerInSecs); 
    ptrUdhConfig->ueThroughtputTimerInSecs = ueThroughtputTimerInSecs;
    config_setting_lookup_int(table, "memoryDiagCfg", &memoryDiagCfg); 
    ptrUdhConfig->memoryDiagCfg = memoryDiagCfg;
  }
  else
  {
    fprintf(stderr, "Table Not present : %s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
  }
  config_destroy(&cfg);
}


/**
 * @param    fileToWrite : This is the full file path of the configuration file which will be written
 * @section   This function will read Cell configuration table and fill default
 *            configurations to file fileToWrite given as input by caller function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *
 **/
TBOOL writeToFileUdhConfigTable(
  IN UdhConfig               *ptrUdhConfig,
  IO UINT8                   *fileToWrite,
  IO UINT8                   *error,
  IO FILE                    *fp
  )
{
  config_t         readCfg;/* Read RO values into local structure use them while writing new cfg */
  config_setting_t *readTable;
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */
  config_init(&readCfg); /* init LibConfig */
  *error = FALSE;

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&readCfg, fileToWrite))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&readCfg),config_error_line(&readCfg), config_error_text(&readCfg));
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    return FALSE;
  }
  readTable = config_lookup(&readCfg, "udhConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table udhConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "udhConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "ulInactivityIntervalInSecs", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUdhConfig->ulInactivityIntervalInSecs);
  setting = config_setting_add(group, "ueThroughtputTimerInSecs", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUdhConfig->ueThroughtputTimerInSecs);
  setting = config_setting_add(group, "memoryDiagCfg", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUdhConfig->memoryDiagCfg);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);

  config_destroy(&writeCfg);
  config_destroy(&readCfg);
  return TRUE;
}

/**
 * @param     fileToRead : This is the full file path of the configuration file which will be read
 * @section   This function will read input configuration file and return the filled default
 *            configuration details to the caller of this function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *            Return 1 if ERROR is there otherwise 0
 **/
void readFromFileUehConfigTable(
  IN UINT8                   *fileToRead, 
  IO UehConfig               *ptrUehConfig,
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
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
    config_destroy(&cfg);
    return;
  }
  table = config_lookup(&cfg, "uehConfig");
  if(table NOTEQ NULL)
  {
    int         prachResource;
    int         srsCfgPrd;
    int         srb1LogchcfgPrior;
    int         srb2LogchcfgPrior;
    int         macCfgTtiBundling;
    int         deltaMcsEnabled;
    int         accumulationEnabled;
    int         cqiPUCCHResIdx;
    int         cqiPmiConfigIdx;
    int         riConfigIdx;
    int         simultAckNackAndCQI;
    int         tpcRNTI;
    int         indexOfFormat3;
    int         transmissionComb;
    int         srPucchResIdx;
    int         srConfigIdx;
    int         drbGrp;
    int         n1PucchAnRep;
    int         formatInd;
    int         periodicMode;
    int         prdicityEnum;
    int         cqiOffst;
    int         cqiCfgk;
    int         ueUlPwrTrgCqi;
    int         ackNackRepFactor;
    int         macInst;
    int         maxUePerUlSf;
    int         maxUePerDlSf;
    int         maxUlBwPerUe;
    int         maxDlBwPerUe;
    int         maxDlRetxBw;
    int         maxDlUeNewTxPerTti;
    int         maxUlUeNewTxPerTti;
    int         isCyclicPrefixUlExt ;
    int         isCyclicPrefixDlExt ;
    int         cellModSchm;
    int         bcchPchRaCodeRate;
    int         pdcchCodeRate;
    int         dlCmnCodRateCcchCqi;
    int         puschSubBndStrt;
    int         puschNoSubbnds;
    int         puschSubBandSz;
    int         puschSubBandDmrs;
    int         ulCmnCodRateCcchCqi;
    int         isDlFreqSel;
    int         thresholdCqi;
    int         preambleFrmt;
    int         raWinSz;
    int         raOccSz;
    int         raOccSfnEnum;
    int         pucchPwrFmt3RntiSt;
    int         pucchPwrFmt3RntiSz;
    int         pucchPwrFmt3aRntiSt;
    int         pucchPwrFmt3aRntiSz;
    int         puschPwrFmt3RntiSt;
    int         puschPwrFmt3RntiSz;
    int         puschPwrFmt3aRntiSt;
    int         puschPwrFmt3aRntiSz;
    int         preambleSetCfgPres;
    int         preambleSetCfgStart;
    int         preambleSetCfgSz;
    int         prachNumRes;
    int         isUeSecEnble;
    int         ueCatPollByteIdx;
    int         dupMode;
    int         ulCpLength;
    int         srbPrioritisedBitRate;
    int         uePwrReconfTimerinSec;
    int         isDlLaEnabled;
    int         uehStateAuditTmrDurInSecs;
    int         uehProcedureAuditTmrDurInSecs;
    int         uehPucchAuditTmrDurInSecs;
    int         memoryDiagCfg;
    int         maxMsg3PerUlSf;

    config_setting_lookup_int(table, "systemInfoValueTag", (int*)&ptrUehConfig->systemInfoValueTag); 
    config_setting_lookup_int(table, "prachResource", &prachResource); 
    ptrUehConfig->prachResource = prachResource;
    config_setting_lookup_int(table, "scSpacing", (int*)&ptrUehConfig->scSpacing); 
    config_setting_lookup_int(table, "cycPfxType", (int*)&ptrUehConfig->cycPfxType); 
    config_setting_lookup_int(table, "srsCfgPrd", &srsCfgPrd); 
    ptrUehConfig->srsCfgPrd = srsCfgPrd;
    config_setting_lookup_int(table, "srbPollRetrans", (int*)&ptrUehConfig->srbPollRetrans); 
    config_setting_lookup_int(table, "srbPollPdu", (int*)&ptrUehConfig->srbPollPdu); 
    config_setting_lookup_int(table, "srbPollByte", (int*)&ptrUehConfig->srbPollByte); 
    config_setting_lookup_int(table, "srbMaxReTxThresh", (int*)&ptrUehConfig->srbMaxReTxThresh); 
    config_setting_lookup_int(table, "srbTreordrng", (int*)&ptrUehConfig->srbTreordrng); 
    config_setting_lookup_int(table, "srbTStatProhibit", (int*)&ptrUehConfig->srbTStatProhibit); 
    config_setting_lookup_int(table, "srb1LogchcfgPrior", &srb1LogchcfgPrior); 
    ptrUehConfig->srb1LogchcfgPrior = srb1LogchcfgPrior;
    config_setting_lookup_int(table, "srb2LogchcfgPrior", &srb2LogchcfgPrior); 
    ptrUehConfig->srb2LogchcfgPrior = srb2LogchcfgPrior;
    config_setting_lookup_int(table, "macCfgTtiBundling", &macCfgTtiBundling); 
    ptrUehConfig->macCfgTtiBundling = macCfgTtiBundling;
    config_setting_lookup_int(table, "deltaMcsEnabled", &deltaMcsEnabled); 
    ptrUehConfig->deltaMcsEnabled = deltaMcsEnabled;
    config_setting_lookup_int(table, "accumulationEnabled", &accumulationEnabled); 
    ptrUehConfig->accumulationEnabled = accumulationEnabled;
    config_setting_lookup_int(table, "cqiPUCCHResIdx", &cqiPUCCHResIdx); 
    ptrUehConfig->cqiPUCCHResIdx = cqiPUCCHResIdx;
    config_setting_lookup_int(table, "cqiPmiConfigIdx", &cqiPmiConfigIdx); 
    ptrUehConfig->cqiPmiConfigIdx = cqiPmiConfigIdx;
    config_setting_lookup_int(table, "riConfigIdx", &riConfigIdx); 
    ptrUehConfig->riConfigIdx = riConfigIdx;
    config_setting_lookup_int(table, "simultAckNackAndCQI", &simultAckNackAndCQI); 
    ptrUehConfig->simultAckNackAndCQI = simultAckNackAndCQI;
    config_setting_lookup_int(table, "tpcRNTI", &tpcRNTI); 
    ptrUehConfig->tpcRNTI = tpcRNTI;
    config_setting_lookup_int(table, "indexOfFormat3", &indexOfFormat3); 
    ptrUehConfig->indexOfFormat3 = indexOfFormat3;
    config_setting_lookup_int(table, "srsPeriodicity", (int*)&ptrUehConfig->srsPeriodicity); 
    config_setting_lookup_int(table, "transmissionComb", &transmissionComb); 
    ptrUehConfig->transmissionComb = transmissionComb;
    config_setting_lookup_int(table, "dedCyclicShift", (int*)&ptrUehConfig->dedCyclicShift); 
    config_setting_lookup_int(table, "srPucchResIdx", &srPucchResIdx); 
    ptrUehConfig->srPucchResIdx = srPucchResIdx;
    config_setting_lookup_int(table, "srConfigIdx", &srConfigIdx); 
    ptrUehConfig->srConfigIdx = srConfigIdx;
    config_setting_lookup_int(table, "dsrTransMax", (int*)&ptrUehConfig->dsrTransMax); 
    config_setting_lookup_int(table, "drbGrp", &drbGrp); 
    ptrUehConfig->drbGrp = drbGrp;
    config_setting_lookup_int(table, "ueCategory", (int*)&ptrUehConfig->ueCategory); 
    config_setting_lookup_int(table, "repFact", (int*)&ptrUehConfig->repFact); 
    config_setting_lookup_int(table, "n1PucchAnRep", &n1PucchAnRep); 
    ptrUehConfig->n1PucchAnRep = n1PucchAnRep;
    config_setting_lookup_int(table, "formatInd", &formatInd); 
    ptrUehConfig->formatInd = formatInd;
    config_setting_lookup_int(table, "antennaSel", (int*)&ptrUehConfig->antennaSel); 
    config_setting_lookup_int(table, "aPeriodicMode", (int*)&ptrUehConfig->aPeriodicMode); 
    config_setting_lookup_int(table, "periodicMode", &periodicMode); 
    ptrUehConfig->periodicMode = periodicMode;
    config_setting_lookup_int(table, "prdicityEnum", &prdicityEnum); 
    ptrUehConfig->prdicityEnum = prdicityEnum;
    config_setting_lookup_int(table, "cqiOffst", &cqiOffst); 
    ptrUehConfig->cqiOffst = cqiOffst;
    config_setting_lookup_int(table, "cqiCfgk", &cqiCfgk); 
    ptrUehConfig->cqiCfgk = cqiCfgk;
    config_setting_lookup_int(table, "maxUlHqTx", (int*)&ptrUehConfig->maxUlHqTx); 
    config_setting_lookup_int(table, "ueUlPwrTrgCqi", &ueUlPwrTrgCqi); 
    ptrUehConfig->ueUlPwrTrgCqi = ueUlPwrTrgCqi;
    config_setting_lookup_int(table, "ackNackRepFactor", &ackNackRepFactor); 
    ptrUehConfig->ackNackRepFactor = ackNackRepFactor;
    config_setting_lookup_int(table, "macInst", &macInst); 
    ptrUehConfig->macInst = macInst;
    config_setting_lookup_int(table, "maxUePerUlSf", &maxUePerUlSf); 
    ptrUehConfig->maxUePerUlSf = maxUePerUlSf;
    config_setting_lookup_int(table, "maxUePerDlSf", &maxUePerDlSf); 
    ptrUehConfig->maxUePerDlSf = maxUePerDlSf;
    config_setting_lookup_int(table, "maxUlBwPerUe", &maxUlBwPerUe); 
    ptrUehConfig->maxUlBwPerUe = maxUlBwPerUe;
    config_setting_lookup_int(table, "maxDlBwPerUe", &maxDlBwPerUe); 
    ptrUehConfig->maxDlBwPerUe = maxDlBwPerUe;
    config_setting_lookup_int(table, "maxDlRetxBw", &maxDlRetxBw); 
    ptrUehConfig->maxDlRetxBw = maxDlRetxBw;
    config_setting_lookup_int(table, "maxDlUeNewTxPerTti", &maxDlUeNewTxPerTti); 
    ptrUehConfig->maxDlUeNewTxPerTti = maxDlUeNewTxPerTti;
    config_setting_lookup_int(table, "maxUlUeNewTxPerTti", &maxUlUeNewTxPerTti); 
    ptrUehConfig->maxUlUeNewTxPerTti = maxUlUeNewTxPerTti;
    config_setting_lookup_int(table, "isCyclicPrefixUlExt ", &isCyclicPrefixUlExt ); 
    ptrUehConfig->isCyclicPrefixUlExt  = isCyclicPrefixUlExt ;
    config_setting_lookup_int(table, "isCyclicPrefixDlExt ", &isCyclicPrefixDlExt ); 
    ptrUehConfig->isCyclicPrefixDlExt  = isCyclicPrefixDlExt ;
    config_setting_lookup_int(table, "cellModSchm", &cellModSchm); 
    ptrUehConfig->cellModSchm = cellModSchm;
    config_setting_lookup_int(table, "bcchPchRaCodeRate", &bcchPchRaCodeRate); 
    ptrUehConfig->bcchPchRaCodeRate = bcchPchRaCodeRate;
    config_setting_lookup_int(table, "pdcchCodeRate", &pdcchCodeRate); 
    ptrUehConfig->pdcchCodeRate = pdcchCodeRate;
    config_setting_lookup_int(table, "dlCmnCodRateCcchCqi", &dlCmnCodRateCcchCqi); 
    ptrUehConfig->dlCmnCodRateCcchCqi = dlCmnCodRateCcchCqi;
    config_setting_lookup_int(table, "puschSubBndStrt", &puschSubBndStrt); 
    ptrUehConfig->puschSubBndStrt = puschSubBndStrt;
    config_setting_lookup_int(table, "puschNoSubbnds", &puschNoSubbnds); 
    ptrUehConfig->puschNoSubbnds = puschNoSubbnds;
    config_setting_lookup_int(table, "puschSubBandSz", &puschSubBandSz); 
    ptrUehConfig->puschSubBandSz = puschSubBandSz;
    config_setting_lookup_int(table, "puschSubBandDmrs", &puschSubBandDmrs); 
    ptrUehConfig->puschSubBandDmrs = puschSubBandDmrs;
    config_setting_lookup_int(table, "ulCmnCodRateCcchCqi", &ulCmnCodRateCcchCqi); 
    ptrUehConfig->ulCmnCodRateCcchCqi = ulCmnCodRateCcchCqi;
    config_setting_lookup_int(table, "isDlFreqSel", &isDlFreqSel); 
    ptrUehConfig->isDlFreqSel = isDlFreqSel;
    config_setting_lookup_int(table, "thresholdCqi", &thresholdCqi); 
    ptrUehConfig->thresholdCqi = thresholdCqi;
    config_setting_lookup_int(table, "preambleFrmt", &preambleFrmt); 
    ptrUehConfig->preambleFrmt = preambleFrmt;
    config_setting_lookup_int(table, "raWinSz", &raWinSz); 
    ptrUehConfig->raWinSz = raWinSz;
    config_setting_lookup_int(table, "raOccSz", &raOccSz); 
    ptrUehConfig->raOccSz = raOccSz;
    config_setting_lookup_int(table, "raOccSfnEnum", &raOccSfnEnum); 
    ptrUehConfig->raOccSfnEnum = raOccSfnEnum;
    config_setting_lookup_int(table, "pucchPwrFmt3RntiSt", &pucchPwrFmt3RntiSt); 
    ptrUehConfig->pucchPwrFmt3RntiSt = pucchPwrFmt3RntiSt;
    config_setting_lookup_int(table, "pucchPwrFmt3RntiSz", &pucchPwrFmt3RntiSz); 
    ptrUehConfig->pucchPwrFmt3RntiSz = pucchPwrFmt3RntiSz;
    config_setting_lookup_int(table, "pucchPwrFmt3aRntiSt", &pucchPwrFmt3aRntiSt); 
    ptrUehConfig->pucchPwrFmt3aRntiSt = pucchPwrFmt3aRntiSt;
    config_setting_lookup_int(table, "pucchPwrFmt3aRntiSz", &pucchPwrFmt3aRntiSz); 
    ptrUehConfig->pucchPwrFmt3aRntiSz = pucchPwrFmt3aRntiSz;
    config_setting_lookup_int(table, "puschPwrFmt3RntiSt", &puschPwrFmt3RntiSt); 
    ptrUehConfig->puschPwrFmt3RntiSt = puschPwrFmt3RntiSt;
    config_setting_lookup_int(table, "puschPwrFmt3RntiSz", &puschPwrFmt3RntiSz); 
    ptrUehConfig->puschPwrFmt3RntiSz = puschPwrFmt3RntiSz;
    config_setting_lookup_int(table, "puschPwrFmt3aRntiSt", &puschPwrFmt3aRntiSt); 
    ptrUehConfig->puschPwrFmt3aRntiSt = puschPwrFmt3aRntiSt;
    config_setting_lookup_int(table, "puschPwrFmt3aRntiSz", &puschPwrFmt3aRntiSz); 
    ptrUehConfig->puschPwrFmt3aRntiSz = puschPwrFmt3aRntiSz;
    config_setting_lookup_int(table, "ueUlByteRate", (int*)&ptrUehConfig->ueUlByteRate); 
    config_setting_lookup_int(table, "preambleSetCfgPres", &preambleSetCfgPres); 
    ptrUehConfig->preambleSetCfgPres = preambleSetCfgPres;
    config_setting_lookup_int(table, "preambleSetCfgStart", &preambleSetCfgStart); 
    ptrUehConfig->preambleSetCfgStart = preambleSetCfgStart;
    config_setting_lookup_int(table, "preambleSetCfgSz", &preambleSetCfgSz); 
    ptrUehConfig->preambleSetCfgSz = preambleSetCfgSz;
    config_setting_lookup_int(table, "prachNumRes", &prachNumRes); 
    ptrUehConfig->prachNumRes = prachNumRes;
    config_setting_lookup_int(table, "crntiRtnTimer", (int*)&ptrUehConfig->crntiRtnTimer); 
    config_setting_lookup_int(table, "numOfTxAnt", (int*)&ptrUehConfig->numOfTxAnt); 
    config_setting_lookup_int(table, "standaloneSrb1Timer", (int*)&ptrUehConfig->standaloneSrb1Timer); 
    config_setting_lookup_int(table, "contextReleaseTimer", (int*)&ptrUehConfig->contextReleaseTimer); 
    config_setting_lookup_int(table, "mmeRelIndTimer", (int*)&ptrUehConfig->mmeRelIndTimer); 
    config_setting_lookup_int(table, "rrcConnRelL2AckTimer", (int*)&ptrUehConfig->rrcConnRelL2AckTimer); 
    config_setting_lookup_int(table, "isUeSecEnble", &isUeSecEnble); 
    ptrUehConfig->isUeSecEnble = isUeSecEnble;
    config_setting_lookup_int(table, "ueCatPollByteIdx", &ueCatPollByteIdx); 
    ptrUehConfig->ueCatPollByteIdx = ueCatPollByteIdx;
    config_setting_lookup_int(table, "dupMode", &dupMode); 
    ptrUehConfig->dupMode = dupMode;
    config_setting_lookup_int(table, "ulUmSnFieldLen", (int*)&ptrUehConfig->ulUmSnFieldLen); 
    config_setting_lookup_int(table, "dlUmSnFieldLen", (int*)&ptrUehConfig->dlUmSnFieldLen); 
    config_setting_lookup_int(table, "umRlcTReordering", (int*)&ptrUehConfig->umRlcTReordering); 
    config_setting_lookup_int(table, "ulCpLength", &ulCpLength); 
    ptrUehConfig->ulCpLength = ulCpLength;
    config_setting_lookup_int(table, "srbPrioritisedBitRate", &srbPrioritisedBitRate); 
    ptrUehConfig->srbPrioritisedBitRate = srbPrioritisedBitRate;
    config_setting_lookup_int(table, "internalUeConfigTimer", (int*)&ptrUehConfig->internalUeConfigTimer); 
    config_setting_lookup_int(table, "uePwrReconfTimerinSec", &uePwrReconfTimerinSec); 
    ptrUehConfig->uePwrReconfTimerinSec = uePwrReconfTimerinSec;
    config_setting_lookup_int(table, "isDlLaEnabled", &isDlLaEnabled); 
    ptrUehConfig->isDlLaEnabled = isDlLaEnabled;
    config_setting_lookup_int(table, "uehStateAuditTmrDurInSecs", &uehStateAuditTmrDurInSecs); 
    ptrUehConfig->uehStateAuditTmrDurInSecs = uehStateAuditTmrDurInSecs;
    config_setting_lookup_int(table, "uehProcedureAuditTmrDurInSecs", &uehProcedureAuditTmrDurInSecs); 
    ptrUehConfig->uehProcedureAuditTmrDurInSecs = uehProcedureAuditTmrDurInSecs;
    config_setting_lookup_int(table, "uehPucchAuditTmrDurInSecs", &uehPucchAuditTmrDurInSecs); 
    ptrUehConfig->uehPucchAuditTmrDurInSecs = uehPucchAuditTmrDurInSecs;
    config_setting_lookup_int(table, "memoryDiagCfg", &memoryDiagCfg); 
    ptrUehConfig->memoryDiagCfg = memoryDiagCfg;
    config_setting_lookup_int(table, "maxMsg3PerUlSf", &maxMsg3PerUlSf); 
    ptrUehConfig->maxMsg3PerUlSf = maxMsg3PerUlSf;
  }
  else
  {
    fprintf(stderr, "Table Not present : %s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
  }
  config_destroy(&cfg);
}


/**
 * @param    fileToWrite : This is the full file path of the configuration file which will be written
 * @section   This function will read Cell configuration table and fill default
 *            configurations to file fileToWrite given as input by caller function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *
 **/
TBOOL writeToFileUehConfigTable(
  IN UehConfig               *ptrUehConfig,
  IO UINT8                   *fileToWrite,
  IO UINT8                   *error,
  IO FILE                    *fp
  )
{
  config_t         readCfg;/* Read RO values into local structure use them while writing new cfg */
  config_setting_t *readTable;
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */
  config_init(&readCfg); /* init LibConfig */
  *error = FALSE;

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&readCfg, fileToWrite))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&readCfg),config_error_line(&readCfg), config_error_text(&readCfg));
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    return FALSE;
  }
  readTable = config_lookup(&readCfg, "uehConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table uehConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "uehConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "systemInfoValueTag", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->systemInfoValueTag);
  setting = config_setting_add(group, "prachResource", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->prachResource);
  setting = config_setting_add(group, "scSpacing", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->scSpacing);
  setting = config_setting_add(group, "cycPfxType", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->cycPfxType);
  setting = config_setting_add(group, "srsCfgPrd", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->srsCfgPrd);
  setting = config_setting_add(group, "srbPollRetrans", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->srbPollRetrans);
  setting = config_setting_add(group, "srbPollPdu", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->srbPollPdu);
  setting = config_setting_add(group, "srbPollByte", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->srbPollByte);
  setting = config_setting_add(group, "srbMaxReTxThresh", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->srbMaxReTxThresh);
  setting = config_setting_add(group, "srbTreordrng", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->srbTreordrng);
  setting = config_setting_add(group, "srbTStatProhibit", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->srbTStatProhibit);
  setting = config_setting_add(group, "srb1LogchcfgPrior", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->srb1LogchcfgPrior);
  setting = config_setting_add(group, "srb2LogchcfgPrior", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->srb2LogchcfgPrior);
  setting = config_setting_add(group, "macCfgTtiBundling", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->macCfgTtiBundling);
  setting = config_setting_add(group, "deltaMcsEnabled", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->deltaMcsEnabled);
  setting = config_setting_add(group, "accumulationEnabled", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->accumulationEnabled);
  setting = config_setting_add(group, "cqiPUCCHResIdx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->cqiPUCCHResIdx);
  setting = config_setting_add(group, "cqiPmiConfigIdx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->cqiPmiConfigIdx);
  setting = config_setting_add(group, "riConfigIdx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->riConfigIdx);
  setting = config_setting_add(group, "simultAckNackAndCQI", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->simultAckNackAndCQI);
  setting = config_setting_add(group, "tpcRNTI", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->tpcRNTI);
  setting = config_setting_add(group, "indexOfFormat3", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->indexOfFormat3);
  setting = config_setting_add(group, "srsPeriodicity", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->srsPeriodicity);
  setting = config_setting_add(group, "transmissionComb", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->transmissionComb);
  setting = config_setting_add(group, "dedCyclicShift", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->dedCyclicShift);
  setting = config_setting_add(group, "srPucchResIdx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->srPucchResIdx);
  setting = config_setting_add(group, "srConfigIdx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->srConfigIdx);
  setting = config_setting_add(group, "dsrTransMax", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->dsrTransMax);
  setting = config_setting_add(group, "drbGrp", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->drbGrp);
  setting = config_setting_add(group, "ueCategory", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->ueCategory);
  setting = config_setting_add(group, "repFact", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->repFact);
  setting = config_setting_add(group, "n1PucchAnRep", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->n1PucchAnRep);
  setting = config_setting_add(group, "formatInd", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->formatInd);
  setting = config_setting_add(group, "antennaSel", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->antennaSel);
  setting = config_setting_add(group, "aPeriodicMode", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->aPeriodicMode);
  setting = config_setting_add(group, "periodicMode", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->periodicMode);
  setting = config_setting_add(group, "prdicityEnum", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->prdicityEnum);
  setting = config_setting_add(group, "cqiOffst", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->cqiOffst);
  setting = config_setting_add(group, "cqiCfgk", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->cqiCfgk);
  setting = config_setting_add(group, "maxUlHqTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->maxUlHqTx);
  setting = config_setting_add(group, "ueUlPwrTrgCqi", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->ueUlPwrTrgCqi);
  setting = config_setting_add(group, "ackNackRepFactor", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->ackNackRepFactor);
  setting = config_setting_add(group, "macInst", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->macInst);
  setting = config_setting_add(group, "maxUePerUlSf", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->maxUePerUlSf);
  setting = config_setting_add(group, "maxUePerDlSf", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->maxUePerDlSf);
  setting = config_setting_add(group, "maxUlBwPerUe", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->maxUlBwPerUe);
  setting = config_setting_add(group, "maxDlBwPerUe", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->maxDlBwPerUe);
  setting = config_setting_add(group, "maxDlRetxBw", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->maxDlRetxBw);
  setting = config_setting_add(group, "maxDlUeNewTxPerTti", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->maxDlUeNewTxPerTti);
  setting = config_setting_add(group, "maxUlUeNewTxPerTti", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->maxUlUeNewTxPerTti);
  setting = config_setting_add(group, "isCyclicPrefixUlExt ", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->isCyclicPrefixUlExt );
  setting = config_setting_add(group, "isCyclicPrefixDlExt ", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->isCyclicPrefixDlExt );
  setting = config_setting_add(group, "cellModSchm", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->cellModSchm);
  setting = config_setting_add(group, "bcchPchRaCodeRate", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->bcchPchRaCodeRate);
  setting = config_setting_add(group, "pdcchCodeRate", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->pdcchCodeRate);
  setting = config_setting_add(group, "dlCmnCodRateCcchCqi", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->dlCmnCodRateCcchCqi);
  setting = config_setting_add(group, "puschSubBndStrt", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->puschSubBndStrt);
  setting = config_setting_add(group, "puschNoSubbnds", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->puschNoSubbnds);
  setting = config_setting_add(group, "puschSubBandSz", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->puschSubBandSz);
  setting = config_setting_add(group, "puschSubBandDmrs", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->puschSubBandDmrs);
  setting = config_setting_add(group, "ulCmnCodRateCcchCqi", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->ulCmnCodRateCcchCqi);
  setting = config_setting_add(group, "isDlFreqSel", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->isDlFreqSel);
  setting = config_setting_add(group, "thresholdCqi", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->thresholdCqi);
  setting = config_setting_add(group, "preambleFrmt", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->preambleFrmt);
  setting = config_setting_add(group, "raWinSz", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->raWinSz);
  setting = config_setting_add(group, "raOccSz", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->raOccSz);
  setting = config_setting_add(group, "raOccSfnEnum", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->raOccSfnEnum);
  setting = config_setting_add(group, "pucchPwrFmt3RntiSt", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->pucchPwrFmt3RntiSt);
  setting = config_setting_add(group, "pucchPwrFmt3RntiSz", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->pucchPwrFmt3RntiSz);
  setting = config_setting_add(group, "pucchPwrFmt3aRntiSt", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->pucchPwrFmt3aRntiSt);
  setting = config_setting_add(group, "pucchPwrFmt3aRntiSz", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->pucchPwrFmt3aRntiSz);
  setting = config_setting_add(group, "puschPwrFmt3RntiSt", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->puschPwrFmt3RntiSt);
  setting = config_setting_add(group, "puschPwrFmt3RntiSz", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->puschPwrFmt3RntiSz);
  setting = config_setting_add(group, "puschPwrFmt3aRntiSt", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->puschPwrFmt3aRntiSt);
  setting = config_setting_add(group, "puschPwrFmt3aRntiSz", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->puschPwrFmt3aRntiSz);
  setting = config_setting_add(group, "ueUlByteRate", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->ueUlByteRate);
  setting = config_setting_add(group, "preambleSetCfgPres", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->preambleSetCfgPres);
  setting = config_setting_add(group, "preambleSetCfgStart", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->preambleSetCfgStart);
  setting = config_setting_add(group, "preambleSetCfgSz", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->preambleSetCfgSz);
  setting = config_setting_add(group, "prachNumRes", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->prachNumRes);
  setting = config_setting_add(group, "crntiRtnTimer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->crntiRtnTimer);
  setting = config_setting_add(group, "numOfTxAnt", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->numOfTxAnt);
  setting = config_setting_add(group, "standaloneSrb1Timer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->standaloneSrb1Timer);
  setting = config_setting_add(group, "contextReleaseTimer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->contextReleaseTimer);
  setting = config_setting_add(group, "mmeRelIndTimer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->mmeRelIndTimer);
  setting = config_setting_add(group, "rrcConnRelL2AckTimer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->rrcConnRelL2AckTimer);
  setting = config_setting_add(group, "isUeSecEnble", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->isUeSecEnble);
  setting = config_setting_add(group, "ueCatPollByteIdx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->ueCatPollByteIdx);
  setting = config_setting_add(group, "dupMode", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->dupMode);
  setting = config_setting_add(group, "ulUmSnFieldLen", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->ulUmSnFieldLen);
  setting = config_setting_add(group, "dlUmSnFieldLen", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->dlUmSnFieldLen);
  setting = config_setting_add(group, "umRlcTReordering", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->umRlcTReordering);
  setting = config_setting_add(group, "ulCpLength", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->ulCpLength);
  setting = config_setting_add(group, "srbPrioritisedBitRate", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->srbPrioritisedBitRate);
  setting = config_setting_add(group, "internalUeConfigTimer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->internalUeConfigTimer);
  setting = config_setting_add(group, "uePwrReconfTimerinSec", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->uePwrReconfTimerinSec);
  setting = config_setting_add(group, "isDlLaEnabled", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->isDlLaEnabled);
  setting = config_setting_add(group, "uehStateAuditTmrDurInSecs", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->uehStateAuditTmrDurInSecs);
  setting = config_setting_add(group, "uehProcedureAuditTmrDurInSecs", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->uehProcedureAuditTmrDurInSecs);
  setting = config_setting_add(group, "uehPucchAuditTmrDurInSecs", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->uehPucchAuditTmrDurInSecs);
  setting = config_setting_add(group, "memoryDiagCfg", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->memoryDiagCfg);
  setting = config_setting_add(group, "maxMsg3PerUlSf", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUehConfig->maxMsg3PerUlSf);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);

  config_destroy(&writeCfg);
  config_destroy(&readCfg);
  return TRUE;
}

/**
 * @param     fileToRead : This is the full file path of the configuration file which will be read
 * @section   This function will read input configuration file and return the filled default
 *            configuration details to the caller of this function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *            Return 1 if ERROR is there otherwise 0
 **/
void readFromFileEgtpConfigTable(
  IN UINT8                   *fileToRead, 
  IO EgtpConfig              *ptrEgtpConfig,
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
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
    config_destroy(&cfg);
    return;
  }
  table = config_lookup(&cfg, "egtpConfig");
  if(table NOTEQ NULL)
  {
    int         nmbUSaps;
    int         maxNmbTSaps;
    int         nmbSrvs;
    int         timerRes;
    int         resThreshUpper;
    int         resThreshLower;
    int         eguT3TimerEnabled;
    int         eguT3TimerVal;
    int         nmbRetries;
    int         echoTmrEnabled;
    int         echoTmrVal;
    int         reOrderTmrEnabled;
    int         reOrderTmrVal;
    int         pvtExtId;
    int         maxNumOfIp;
    int         nodeArch;
    int         nmbWorkerThread;
    int         loadDistTmrEnabled;
    int         loadDistTmrVal;
    int         tInitTmrEnabled;
    int         tInitTmrVal;
    int         maxBndRetry;
    int         bndTmCfgEnabled;
    int         bndTmCfgVal;
    int         tPar_u_sockParam_listenQSize;
    int         tPar_u_sockParam_numOpts;
    int         ipInfo_echoReqPvtExtValLength;
    const char *ipInfo_echoReqPvtExtValBuff;
    int         ipInfo_echoRspPvtExtValLength;
    const char *ipInfo_echoRspPvtExtValBuff;
    int         ipInfo_errIndPvtExtValLength;
    const char *ipInfo_errIndPvtExtValBuff;
    int         tserv_tPar_u_sockParam_numOpts;
    int         tserv_tPar_u_sockParam_listenQSize;
    int         opnSrvRetryCnt;
    int         opnSrvTmrEnabled;
    int         opnSrvTmrVal;
    int         hostNamePresent;
    int         hostNameLen;
    int         gtp_C_Or_U;

    config_setting_lookup_int(table, "nmbUSaps", &nmbUSaps); 
    ptrEgtpConfig->nmbUSaps = nmbUSaps;
    config_setting_lookup_int(table, "maxNmbTSaps", &maxNmbTSaps); 
    ptrEgtpConfig->maxNmbTSaps = maxNmbTSaps;
    config_setting_lookup_int(table, "nmbSrvs", &nmbSrvs); 
    ptrEgtpConfig->nmbSrvs = nmbSrvs;
    config_setting_lookup_int(table, "timerRes", &timerRes); 
    ptrEgtpConfig->timerRes = timerRes;
    config_setting_lookup_int(table, "resThreshUpper", &resThreshUpper); 
    ptrEgtpConfig->resThreshUpper = resThreshUpper;
    config_setting_lookup_int(table, "resThreshLower", &resThreshLower); 
    ptrEgtpConfig->resThreshLower = resThreshLower;
    config_setting_lookup_int(table, "eguT3TimerEnabled", &eguT3TimerEnabled); 
    ptrEgtpConfig->eguT3TimerEnabled = eguT3TimerEnabled;
    config_setting_lookup_int(table, "eguT3TimerVal", &eguT3TimerVal); 
    ptrEgtpConfig->eguT3TimerVal = eguT3TimerVal;
    config_setting_lookup_int(table, "nmbRetries", &nmbRetries); 
    ptrEgtpConfig->nmbRetries = nmbRetries;
    config_setting_lookup_int(table, "echoTmrEnabled", &echoTmrEnabled); 
    ptrEgtpConfig->echoTmrEnabled = echoTmrEnabled;
    config_setting_lookup_int(table, "echoTmrVal", &echoTmrVal); 
    ptrEgtpConfig->echoTmrVal = echoTmrVal;
    config_setting_lookup_int(table, "reOrderTmrEnabled", &reOrderTmrEnabled); 
    ptrEgtpConfig->reOrderTmrEnabled = reOrderTmrEnabled;
    config_setting_lookup_int(table, "reOrderTmrVal", &reOrderTmrVal); 
    ptrEgtpConfig->reOrderTmrVal = reOrderTmrVal;
    config_setting_lookup_int(table, "pvtExtId", &pvtExtId); 
    ptrEgtpConfig->pvtExtId = pvtExtId;
    config_setting_lookup_int(table, "pduWindSize", (int*)&ptrEgtpConfig->pduWindSize); 
    config_setting_lookup_int(table, "maxNumOfIp", &maxNumOfIp); 
    ptrEgtpConfig->maxNumOfIp = maxNumOfIp;
    config_setting_lookup_int(table, "nodeArch", &nodeArch); 
    ptrEgtpConfig->nodeArch = nodeArch;
    config_setting_lookup_int(table, "nmbWorkerThread", &nmbWorkerThread); 
    ptrEgtpConfig->nmbWorkerThread = nmbWorkerThread;
    config_setting_lookup_int(table, "loadDistTmrEnabled", &loadDistTmrEnabled); 
    ptrEgtpConfig->loadDistTmrEnabled = loadDistTmrEnabled;
    config_setting_lookup_int(table, "loadDistTmrVal", &loadDistTmrVal); 
    ptrEgtpConfig->loadDistTmrVal = loadDistTmrVal;
    config_setting_lookup_int(table, "tInitTmrEnabled", &tInitTmrEnabled); 
    ptrEgtpConfig->tInitTmrEnabled = tInitTmrEnabled;
    config_setting_lookup_int(table, "tInitTmrVal", &tInitTmrVal); 
    ptrEgtpConfig->tInitTmrVal = tInitTmrVal;
    config_setting_lookup_int(table, "maxBndRetry", &maxBndRetry); 
    ptrEgtpConfig->maxBndRetry = maxBndRetry;
    config_setting_lookup_int(table, "bndTmCfgEnabled", &bndTmCfgEnabled); 
    ptrEgtpConfig->bndTmCfgEnabled = bndTmCfgEnabled;
    config_setting_lookup_int(table, "bndTmCfgVal", &bndTmCfgVal); 
    ptrEgtpConfig->bndTmCfgVal = bndTmCfgVal;
    config_setting_lookup_int(table, "tPar_u_sockParam_listenQSize", &tPar_u_sockParam_listenQSize); 
    ptrEgtpConfig->tPar_u_sockParam_listenQSize = tPar_u_sockParam_listenQSize;
    config_setting_lookup_int(table, "tPar_u_sockParam_numOpts", &tPar_u_sockParam_numOpts); 
    ptrEgtpConfig->tPar_u_sockParam_numOpts = tPar_u_sockParam_numOpts;
    config_setting_lookup_int(table, "ipInfo_echoReqPvtExtValLength", &ipInfo_echoReqPvtExtValLength); 
    ptrEgtpConfig->ipInfo_echoReqPvtExtValLength = ipInfo_echoReqPvtExtValLength;
    config_setting_lookup_string(table, "ipInfo_echoReqPvtExtValBuff", &ipInfo_echoReqPvtExtValBuff); 
    strncpy(ptrEgtpConfig->ipInfo_echoReqPvtExtValBuff,ipInfo_echoReqPvtExtValBuff, MAX_SIZE_EGTPCONFIG_IPINFO_ECHOREQPVTEXTVALBUFF );
    config_setting_lookup_int(table, "ipInfo_echoRspPvtExtValLength", &ipInfo_echoRspPvtExtValLength); 
    ptrEgtpConfig->ipInfo_echoRspPvtExtValLength = ipInfo_echoRspPvtExtValLength;
    config_setting_lookup_string(table, "ipInfo_echoRspPvtExtValBuff", &ipInfo_echoRspPvtExtValBuff); 
    strncpy(ptrEgtpConfig->ipInfo_echoRspPvtExtValBuff,ipInfo_echoRspPvtExtValBuff, MAX_SIZE_EGTPCONFIG_IPINFO_ECHORSPPVTEXTVALBUFF );
    config_setting_lookup_int(table, "ipInfo_errIndPvtExtValLength", &ipInfo_errIndPvtExtValLength); 
    ptrEgtpConfig->ipInfo_errIndPvtExtValLength = ipInfo_errIndPvtExtValLength;
    config_setting_lookup_string(table, "ipInfo_errIndPvtExtValBuff", &ipInfo_errIndPvtExtValBuff); 
    strncpy(ptrEgtpConfig->ipInfo_errIndPvtExtValBuff,ipInfo_errIndPvtExtValBuff, MAX_SIZE_EGTPCONFIG_IPINFO_ERRINDPVTEXTVALBUFF );
    config_setting_lookup_int(table, "tserv_tPar_u_sockParam_numOpts", &tserv_tPar_u_sockParam_numOpts); 
    ptrEgtpConfig->tserv_tPar_u_sockParam_numOpts = tserv_tPar_u_sockParam_numOpts;
    config_setting_lookup_int(table, "tserv_tPar_u_sockParam_listenQSize", &tserv_tPar_u_sockParam_listenQSize); 
    ptrEgtpConfig->tserv_tPar_u_sockParam_listenQSize = tserv_tPar_u_sockParam_listenQSize;
    config_setting_lookup_int(table, "opnSrvRetryCnt", &opnSrvRetryCnt); 
    ptrEgtpConfig->opnSrvRetryCnt = opnSrvRetryCnt;
    config_setting_lookup_int(table, "opnSrvTmrEnabled", &opnSrvTmrEnabled); 
    ptrEgtpConfig->opnSrvTmrEnabled = opnSrvTmrEnabled;
    config_setting_lookup_int(table, "opnSrvTmrVal", &opnSrvTmrVal); 
    ptrEgtpConfig->opnSrvTmrVal = opnSrvTmrVal;
    config_setting_lookup_int(table, "hostNamePresent", &hostNamePresent); 
    ptrEgtpConfig->hostNamePresent = hostNamePresent;
    config_setting_lookup_int(table, "hostNameLen", &hostNameLen); 
    ptrEgtpConfig->hostNameLen = hostNameLen;
    config_setting_lookup_int(table, "gtp_C_Or_U", &gtp_C_Or_U); 
    ptrEgtpConfig->gtp_C_Or_U = gtp_C_Or_U;
  }
  else
  {
    fprintf(stderr, "Table Not present : %s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
  }
  config_destroy(&cfg);
}


/**
 * @param    fileToWrite : This is the full file path of the configuration file which will be written
 * @section   This function will read Cell configuration table and fill default
 *            configurations to file fileToWrite given as input by caller function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *
 **/
TBOOL writeToFileEgtpConfigTable(
  IN EgtpConfig              *ptrEgtpConfig,
  IO UINT8                   *fileToWrite,
  IO UINT8                   *error,
  IO FILE                    *fp
  )
{
  config_t         readCfg;/* Read RO values into local structure use them while writing new cfg */
  config_setting_t *readTable;
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */
  config_init(&readCfg); /* init LibConfig */
  *error = FALSE;

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&readCfg, fileToWrite))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&readCfg),config_error_line(&readCfg), config_error_text(&readCfg));
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    return FALSE;
  }
  readTable = config_lookup(&readCfg, "egtpConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table egtpConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "egtpConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "nmbUSaps", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->nmbUSaps);
  setting = config_setting_add(group, "maxNmbTSaps", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->maxNmbTSaps);
  setting = config_setting_add(group, "nmbSrvs", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->nmbSrvs);
  setting = config_setting_add(group, "timerRes", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->timerRes);
  setting = config_setting_add(group, "resThreshUpper", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->resThreshUpper);
  setting = config_setting_add(group, "resThreshLower", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->resThreshLower);
  setting = config_setting_add(group, "eguT3TimerEnabled", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->eguT3TimerEnabled);
  setting = config_setting_add(group, "eguT3TimerVal", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->eguT3TimerVal);
  setting = config_setting_add(group, "nmbRetries", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->nmbRetries);
  setting = config_setting_add(group, "echoTmrEnabled", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->echoTmrEnabled);
  setting = config_setting_add(group, "echoTmrVal", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->echoTmrVal);
  setting = config_setting_add(group, "reOrderTmrEnabled", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->reOrderTmrEnabled);
  setting = config_setting_add(group, "reOrderTmrVal", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->reOrderTmrVal);
  setting = config_setting_add(group, "pvtExtId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->pvtExtId);
  setting = config_setting_add(group, "pduWindSize", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->pduWindSize);
  setting = config_setting_add(group, "maxNumOfIp", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->maxNumOfIp);
  setting = config_setting_add(group, "nodeArch", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->nodeArch);
  setting = config_setting_add(group, "nmbWorkerThread", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->nmbWorkerThread);
  setting = config_setting_add(group, "loadDistTmrEnabled", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->loadDistTmrEnabled);
  setting = config_setting_add(group, "loadDistTmrVal", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->loadDistTmrVal);
  setting = config_setting_add(group, "tInitTmrEnabled", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->tInitTmrEnabled);
  setting = config_setting_add(group, "tInitTmrVal", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->tInitTmrVal);
  setting = config_setting_add(group, "maxBndRetry", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->maxBndRetry);
  setting = config_setting_add(group, "bndTmCfgEnabled", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->bndTmCfgEnabled);
  setting = config_setting_add(group, "bndTmCfgVal", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->bndTmCfgVal);
  setting = config_setting_add(group, "tPar_u_sockParam_listenQSize", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->tPar_u_sockParam_listenQSize);
  setting = config_setting_add(group, "tPar_u_sockParam_numOpts", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->tPar_u_sockParam_numOpts);
  setting = config_setting_add(group, "ipInfo_echoReqPvtExtValLength", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->ipInfo_echoReqPvtExtValLength);
  setting = config_setting_add(group, "ipInfo_echoReqPvtExtValBuff", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEgtpConfig->ipInfo_echoReqPvtExtValBuff);
  setting = config_setting_add(group, "ipInfo_echoRspPvtExtValLength", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->ipInfo_echoRspPvtExtValLength);
  setting = config_setting_add(group, "ipInfo_echoRspPvtExtValBuff", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEgtpConfig->ipInfo_echoRspPvtExtValBuff);
  setting = config_setting_add(group, "ipInfo_errIndPvtExtValLength", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->ipInfo_errIndPvtExtValLength);
  setting = config_setting_add(group, "ipInfo_errIndPvtExtValBuff", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEgtpConfig->ipInfo_errIndPvtExtValBuff);
  setting = config_setting_add(group, "tserv_tPar_u_sockParam_numOpts", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->tserv_tPar_u_sockParam_numOpts);
  setting = config_setting_add(group, "tserv_tPar_u_sockParam_listenQSize", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->tserv_tPar_u_sockParam_listenQSize);
  setting = config_setting_add(group, "opnSrvRetryCnt", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->opnSrvRetryCnt);
  setting = config_setting_add(group, "opnSrvTmrEnabled", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->opnSrvTmrEnabled);
  setting = config_setting_add(group, "opnSrvTmrVal", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->opnSrvTmrVal);
  setting = config_setting_add(group, "hostNamePresent", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->hostNamePresent);
  setting = config_setting_add(group, "hostNameLen", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->hostNameLen);
  setting = config_setting_add(group, "gtp_C_Or_U", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEgtpConfig->gtp_C_Or_U);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);

  config_destroy(&writeCfg);
  config_destroy(&readCfg);
  return TRUE;
}

/**
 * @param     fileToRead : This is the full file path of the configuration file which will be read
 * @section   This function will read input configuration file and return the filled default
 *            configuration details to the caller of this function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *            Return 1 if ERROR is there otherwise 0
 **/
void readFromFileSctpConfigTable(
  IN UINT8                   *fileToRead, 
  IO SctpConfig              *ptrSctpConfig,
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
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
    config_destroy(&cfg);
    return;
  }
  table = config_lookup(&cfg, "sctpConfig");
  if(table NOTEQ NULL)
  {
    int         maxNmbSctSaps;
    int         maxNmbTSaps;
    int         maxNmbAssoc;
    int         maxNmbEndp;
    int         maxNmbDstAddr;
    int         maxNmbSrcAddr;
    int         maxNmbInStrms;
    int         maxNmbOutStrms;
    int         mtuInitial;
    int         mtuMinInitial;
    int         mtuMaxInitial;
    int         performMtu;
    int         timeRes;
    int         reConfig_MaxInitReTx;
    int         reConfig_MaxAssocReTx;
    int         reConfig_MaxPathReTx;
    int         reConfig_AltAcceptFlg;
    int         reConfig_KeyTm;
    int         reConfig_Alpha;
    int         reConfig_Beta;
    int         reConfig_TIntTmr;
    int         reConfig_MaxBndRetry;
    int         reConfig_MaxAckDelayTm;
    int         reConfig_MaxAckDelayDg;
    int         reConfig_RtoInitial;
    int         reConfig_RtoMin;
    int         reConfig_RtoMax;
    int         reConfig_FreezeTm;
    int         reConfig_BundleTime;
    int         reConfig_CookieLife;
    int         reConfig_HbeatIntervalTm;
    int         reConfig_MaxBurst;
    int         reConfig_MaxHbBurst;
    int         reConfig_T5SdownGrdTm;
    int         reConfig_HandleInitFlg;
    int         reConfig_NegAbrtFlg;
    int         reConfig_HBeatEnable;

    config_setting_lookup_int(table, "maxNmbSctSaps", &maxNmbSctSaps); 
    ptrSctpConfig->maxNmbSctSaps = maxNmbSctSaps;
    config_setting_lookup_int(table, "maxNmbTSaps", &maxNmbTSaps); 
    ptrSctpConfig->maxNmbTSaps = maxNmbTSaps;
    config_setting_lookup_int(table, "maxNmbAssoc", &maxNmbAssoc); 
    ptrSctpConfig->maxNmbAssoc = maxNmbAssoc;
    config_setting_lookup_int(table, "maxNmbEndp", &maxNmbEndp); 
    ptrSctpConfig->maxNmbEndp = maxNmbEndp;
    config_setting_lookup_int(table, "maxNmbDstAddr", &maxNmbDstAddr); 
    ptrSctpConfig->maxNmbDstAddr = maxNmbDstAddr;
    config_setting_lookup_int(table, "maxNmbSrcAddr", &maxNmbSrcAddr); 
    ptrSctpConfig->maxNmbSrcAddr = maxNmbSrcAddr;
    config_setting_lookup_int(table, "maxNmbTxChunks", (int*)&ptrSctpConfig->maxNmbTxChunks); 
    config_setting_lookup_int(table, "maxNmbRxChunks", (int*)&ptrSctpConfig->maxNmbRxChunks); 
    config_setting_lookup_int(table, "maxNmbInStrms", &maxNmbInStrms); 
    ptrSctpConfig->maxNmbInStrms = maxNmbInStrms;
    config_setting_lookup_int(table, "maxNmbOutStrms", &maxNmbOutStrms); 
    ptrSctpConfig->maxNmbOutStrms = maxNmbOutStrms;
    config_setting_lookup_int(table, "initARwnd", (int*)&ptrSctpConfig->initARwnd); 
    config_setting_lookup_int(table, "mtuInitial", &mtuInitial); 
    ptrSctpConfig->mtuInitial = mtuInitial;
    config_setting_lookup_int(table, "mtuMinInitial", &mtuMinInitial); 
    ptrSctpConfig->mtuMinInitial = mtuMinInitial;
    config_setting_lookup_int(table, "mtuMaxInitial", &mtuMaxInitial); 
    ptrSctpConfig->mtuMaxInitial = mtuMaxInitial;
    config_setting_lookup_int(table, "performMtu", &performMtu); 
    ptrSctpConfig->performMtu = performMtu;
    config_setting_lookup_int(table, "timeRes", &timeRes); 
    ptrSctpConfig->timeRes = timeRes;
    config_setting_lookup_int(table, "reConfig_MaxInitReTx", &reConfig_MaxInitReTx); 
    ptrSctpConfig->reConfig_MaxInitReTx = reConfig_MaxInitReTx;
    config_setting_lookup_int(table, "reConfig_MaxAssocReTx", &reConfig_MaxAssocReTx); 
    ptrSctpConfig->reConfig_MaxAssocReTx = reConfig_MaxAssocReTx;
    config_setting_lookup_int(table, "reConfig_MaxPathReTx", &reConfig_MaxPathReTx); 
    ptrSctpConfig->reConfig_MaxPathReTx = reConfig_MaxPathReTx;
    config_setting_lookup_int(table, "reConfig_AltAcceptFlg", &reConfig_AltAcceptFlg); 
    ptrSctpConfig->reConfig_AltAcceptFlg = reConfig_AltAcceptFlg;
    config_setting_lookup_int(table, "reConfig_KeyTm", &reConfig_KeyTm); 
    ptrSctpConfig->reConfig_KeyTm = reConfig_KeyTm;
    config_setting_lookup_int(table, "reConfig_Alpha", &reConfig_Alpha); 
    ptrSctpConfig->reConfig_Alpha = reConfig_Alpha;
    config_setting_lookup_int(table, "reConfig_Beta", &reConfig_Beta); 
    ptrSctpConfig->reConfig_Beta = reConfig_Beta;
    config_setting_lookup_int(table, "reConfig_TIntTmr", &reConfig_TIntTmr); 
    ptrSctpConfig->reConfig_TIntTmr = reConfig_TIntTmr;
    config_setting_lookup_int(table, "reConfig_MaxBndRetry", &reConfig_MaxBndRetry); 
    ptrSctpConfig->reConfig_MaxBndRetry = reConfig_MaxBndRetry;
    config_setting_lookup_int(table, "reConfig_MaxAckDelayTm", &reConfig_MaxAckDelayTm); 
    ptrSctpConfig->reConfig_MaxAckDelayTm = reConfig_MaxAckDelayTm;
    config_setting_lookup_int(table, "reConfig_MaxAckDelayDg", &reConfig_MaxAckDelayDg); 
    ptrSctpConfig->reConfig_MaxAckDelayDg = reConfig_MaxAckDelayDg;
    config_setting_lookup_int(table, "reConfig_RtoInitial", &reConfig_RtoInitial); 
    ptrSctpConfig->reConfig_RtoInitial = reConfig_RtoInitial;
    config_setting_lookup_int(table, "reConfig_RtoMin", &reConfig_RtoMin); 
    ptrSctpConfig->reConfig_RtoMin = reConfig_RtoMin;
    config_setting_lookup_int(table, "reConfig_RtoMax", &reConfig_RtoMax); 
    ptrSctpConfig->reConfig_RtoMax = reConfig_RtoMax;
    config_setting_lookup_int(table, "reConfig_FreezeTm", &reConfig_FreezeTm); 
    ptrSctpConfig->reConfig_FreezeTm = reConfig_FreezeTm;
    config_setting_lookup_int(table, "reConfig_BundleTime", &reConfig_BundleTime); 
    ptrSctpConfig->reConfig_BundleTime = reConfig_BundleTime;
    config_setting_lookup_int(table, "reConfig_CookieLife", &reConfig_CookieLife); 
    ptrSctpConfig->reConfig_CookieLife = reConfig_CookieLife;
    config_setting_lookup_int(table, "reConfig_HbeatIntervalTm", &reConfig_HbeatIntervalTm); 
    ptrSctpConfig->reConfig_HbeatIntervalTm = reConfig_HbeatIntervalTm;
    config_setting_lookup_int(table, "reConfig_MaxBurst", &reConfig_MaxBurst); 
    ptrSctpConfig->reConfig_MaxBurst = reConfig_MaxBurst;
    config_setting_lookup_int(table, "reConfig_MaxHbBurst", &reConfig_MaxHbBurst); 
    ptrSctpConfig->reConfig_MaxHbBurst = reConfig_MaxHbBurst;
    config_setting_lookup_int(table, "reConfig_T5SdownGrdTm", &reConfig_T5SdownGrdTm); 
    ptrSctpConfig->reConfig_T5SdownGrdTm = reConfig_T5SdownGrdTm;
    config_setting_lookup_int(table, "reConfig_HandleInitFlg", &reConfig_HandleInitFlg); 
    ptrSctpConfig->reConfig_HandleInitFlg = reConfig_HandleInitFlg;
    config_setting_lookup_int(table, "reConfig_NegAbrtFlg", &reConfig_NegAbrtFlg); 
    ptrSctpConfig->reConfig_NegAbrtFlg = reConfig_NegAbrtFlg;
    config_setting_lookup_int(table, "reConfig_HBeatEnable", &reConfig_HBeatEnable); 
    ptrSctpConfig->reConfig_HBeatEnable = reConfig_HBeatEnable;
    config_setting_lookup_int(table, "reConfig_FlcUpThr", (int*)&ptrSctpConfig->reConfig_FlcUpThr); 
    config_setting_lookup_int(table, "reConfig_FlcLowThr", (int*)&ptrSctpConfig->reConfig_FlcLowThr); 
  }
  else
  {
    fprintf(stderr, "Table Not present : %s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
  }
  config_destroy(&cfg);
}


/**
 * @param    fileToWrite : This is the full file path of the configuration file which will be written
 * @section   This function will read Cell configuration table and fill default
 *            configurations to file fileToWrite given as input by caller function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *
 **/
TBOOL writeToFileSctpConfigTable(
  IN SctpConfig              *ptrSctpConfig,
  IO UINT8                   *fileToWrite,
  IO UINT8                   *error,
  IO FILE                    *fp
  )
{
  config_t         readCfg;/* Read RO values into local structure use them while writing new cfg */
  config_setting_t *readTable;
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */
  config_init(&readCfg); /* init LibConfig */
  *error = FALSE;

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&readCfg, fileToWrite))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&readCfg),config_error_line(&readCfg), config_error_text(&readCfg));
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    return FALSE;
  }
  readTable = config_lookup(&readCfg, "sctpConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table sctpConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "sctpConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "maxNmbSctSaps", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->maxNmbSctSaps);
  setting = config_setting_add(group, "maxNmbTSaps", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->maxNmbTSaps);
  setting = config_setting_add(group, "maxNmbAssoc", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->maxNmbAssoc);
  setting = config_setting_add(group, "maxNmbEndp", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->maxNmbEndp);
  setting = config_setting_add(group, "maxNmbDstAddr", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->maxNmbDstAddr);
  setting = config_setting_add(group, "maxNmbSrcAddr", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->maxNmbSrcAddr);
  setting = config_setting_add(group, "maxNmbTxChunks", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->maxNmbTxChunks);
  setting = config_setting_add(group, "maxNmbRxChunks", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->maxNmbRxChunks);
  setting = config_setting_add(group, "maxNmbInStrms", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->maxNmbInStrms);
  setting = config_setting_add(group, "maxNmbOutStrms", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->maxNmbOutStrms);
  setting = config_setting_add(group, "initARwnd", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->initARwnd);
  setting = config_setting_add(group, "mtuInitial", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->mtuInitial);
  setting = config_setting_add(group, "mtuMinInitial", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->mtuMinInitial);
  setting = config_setting_add(group, "mtuMaxInitial", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->mtuMaxInitial);
  setting = config_setting_add(group, "performMtu", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->performMtu);
  setting = config_setting_add(group, "timeRes", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->timeRes);
  setting = config_setting_add(group, "reConfig_MaxInitReTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_MaxInitReTx);
  setting = config_setting_add(group, "reConfig_MaxAssocReTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_MaxAssocReTx);
  setting = config_setting_add(group, "reConfig_MaxPathReTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_MaxPathReTx);
  setting = config_setting_add(group, "reConfig_AltAcceptFlg", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_AltAcceptFlg);
  setting = config_setting_add(group, "reConfig_KeyTm", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_KeyTm);
  setting = config_setting_add(group, "reConfig_Alpha", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_Alpha);
  setting = config_setting_add(group, "reConfig_Beta", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_Beta);
  setting = config_setting_add(group, "reConfig_TIntTmr", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_TIntTmr);
  setting = config_setting_add(group, "reConfig_MaxBndRetry", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_MaxBndRetry);
  setting = config_setting_add(group, "reConfig_MaxAckDelayTm", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_MaxAckDelayTm);
  setting = config_setting_add(group, "reConfig_MaxAckDelayDg", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_MaxAckDelayDg);
  setting = config_setting_add(group, "reConfig_RtoInitial", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_RtoInitial);
  setting = config_setting_add(group, "reConfig_RtoMin", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_RtoMin);
  setting = config_setting_add(group, "reConfig_RtoMax", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_RtoMax);
  setting = config_setting_add(group, "reConfig_FreezeTm", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_FreezeTm);
  setting = config_setting_add(group, "reConfig_BundleTime", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_BundleTime);
  setting = config_setting_add(group, "reConfig_CookieLife", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_CookieLife);
  setting = config_setting_add(group, "reConfig_HbeatIntervalTm", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_HbeatIntervalTm);
  setting = config_setting_add(group, "reConfig_MaxBurst", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_MaxBurst);
  setting = config_setting_add(group, "reConfig_MaxHbBurst", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_MaxHbBurst);
  setting = config_setting_add(group, "reConfig_T5SdownGrdTm", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_T5SdownGrdTm);
  setting = config_setting_add(group, "reConfig_HandleInitFlg", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_HandleInitFlg);
  setting = config_setting_add(group, "reConfig_NegAbrtFlg", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_NegAbrtFlg);
  setting = config_setting_add(group, "reConfig_HBeatEnable", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_HBeatEnable);
  setting = config_setting_add(group, "reConfig_FlcUpThr", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_FlcUpThr);
  setting = config_setting_add(group, "reConfig_FlcLowThr", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSctpConfig->reConfig_FlcLowThr);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);

  config_destroy(&writeCfg);
  config_destroy(&readCfg);
  return TRUE;
}

/**
 * @param     fileToRead : This is the full file path of the configuration file which will be read
 * @section   This function will read input configuration file and return the filled default
 *            configuration details to the caller of this function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *            Return 1 if ERROR is there otherwise 0
 **/
void readFromFileS1apConfigTable(
  IN UINT8                   *fileToRead, 
  IO S1apConfig              *ptrS1apConfig,
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
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
    config_destroy(&cfg);
    return;
  }
  table = config_lookup(&cfg, "s1apConfig");
  if(table NOTEQ NULL)
  {
    int         nmbSztSaps;
    int         nmbSctSaps;
    int         timeRes;
    int         maxBndRetry;
    int         bndTmCfgEnabled;
    int         bndTmCfgVal;
    int         s1apLocalPort;
    int         s1apGuardTimer;
    int         assocCfg_nmbRetry;
    int         assocCfg_tmrAssocRecov_enb;
    int         assocCfg_tmrAssocRecov_val;
    int         maxS1SetupAllowed;

    config_setting_lookup_int(table, "nmbSztSaps", &nmbSztSaps); 
    ptrS1apConfig->nmbSztSaps = nmbSztSaps;
    config_setting_lookup_int(table, "nmbSctSaps", &nmbSctSaps); 
    ptrS1apConfig->nmbSctSaps = nmbSctSaps;
    config_setting_lookup_int(table, "nmbConn", (int*)&ptrS1apConfig->nmbConn); 
    config_setting_lookup_int(table, "nmbPeer", (int*)&ptrS1apConfig->nmbPeer); 
    config_setting_lookup_int(table, "timeRes", &timeRes); 
    ptrS1apConfig->timeRes = timeRes;
    config_setting_lookup_int(table, "maxBndRetry", &maxBndRetry); 
    ptrS1apConfig->maxBndRetry = maxBndRetry;
    config_setting_lookup_int(table, "bndTmCfgEnabled", &bndTmCfgEnabled); 
    ptrS1apConfig->bndTmCfgEnabled = bndTmCfgEnabled;
    config_setting_lookup_int(table, "bndTmCfgVal", &bndTmCfgVal); 
    ptrS1apConfig->bndTmCfgVal = bndTmCfgVal;
    config_setting_lookup_int(table, "s1apLocalPort", &s1apLocalPort); 
    ptrS1apConfig->s1apLocalPort = s1apLocalPort;
    config_setting_lookup_int(table, "s1apGuardTimer", &s1apGuardTimer); 
    ptrS1apConfig->s1apGuardTimer = s1apGuardTimer;
    config_setting_lookup_int(table, "assocCfg_nmbRetry", &assocCfg_nmbRetry); 
    ptrS1apConfig->assocCfg_nmbRetry = assocCfg_nmbRetry;
    config_setting_lookup_int(table, "assocCfg_tmrAssocRecov_enb", &assocCfg_tmrAssocRecov_enb); 
    ptrS1apConfig->assocCfg_tmrAssocRecov_enb = assocCfg_tmrAssocRecov_enb;
    config_setting_lookup_int(table, "assocCfg_tmrAssocRecov_val", &assocCfg_tmrAssocRecov_val); 
    ptrS1apConfig->assocCfg_tmrAssocRecov_val = assocCfg_tmrAssocRecov_val;
    config_setting_lookup_int(table, "maxS1SetupAllowed", &maxS1SetupAllowed); 
    ptrS1apConfig->maxS1SetupAllowed = maxS1SetupAllowed;
  }
  else
  {
    fprintf(stderr, "Table Not present : %s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
  }
  config_destroy(&cfg);
}


/**
 * @param    fileToWrite : This is the full file path of the configuration file which will be written
 * @section   This function will read Cell configuration table and fill default
 *            configurations to file fileToWrite given as input by caller function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *
 **/
TBOOL writeToFileS1apConfigTable(
  IN S1apConfig              *ptrS1apConfig,
  IO UINT8                   *fileToWrite,
  IO UINT8                   *error,
  IO FILE                    *fp
  )
{
  config_t         readCfg;/* Read RO values into local structure use them while writing new cfg */
  config_setting_t *readTable;
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */
  config_init(&readCfg); /* init LibConfig */
  *error = FALSE;

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&readCfg, fileToWrite))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&readCfg),config_error_line(&readCfg), config_error_text(&readCfg));
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    return FALSE;
  }
  readTable = config_lookup(&readCfg, "s1apConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table s1apConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "s1apConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "nmbSztSaps", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrS1apConfig->nmbSztSaps);
  setting = config_setting_add(group, "nmbSctSaps", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrS1apConfig->nmbSctSaps);
  setting = config_setting_add(group, "nmbConn", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrS1apConfig->nmbConn);
  setting = config_setting_add(group, "nmbPeer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrS1apConfig->nmbPeer);
  setting = config_setting_add(group, "timeRes", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrS1apConfig->timeRes);
  setting = config_setting_add(group, "maxBndRetry", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrS1apConfig->maxBndRetry);
  setting = config_setting_add(group, "bndTmCfgEnabled", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrS1apConfig->bndTmCfgEnabled);
  setting = config_setting_add(group, "bndTmCfgVal", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrS1apConfig->bndTmCfgVal);
  setting = config_setting_add(group, "s1apLocalPort", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrS1apConfig->s1apLocalPort);
  setting = config_setting_add(group, "s1apGuardTimer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrS1apConfig->s1apGuardTimer);
  setting = config_setting_add(group, "assocCfg_nmbRetry", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrS1apConfig->assocCfg_nmbRetry);
  setting = config_setting_add(group, "assocCfg_tmrAssocRecov_enb", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrS1apConfig->assocCfg_tmrAssocRecov_enb);
  setting = config_setting_add(group, "assocCfg_tmrAssocRecov_val", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrS1apConfig->assocCfg_tmrAssocRecov_val);
  setting = config_setting_add(group, "maxS1SetupAllowed", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrS1apConfig->maxS1SetupAllowed);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);

  config_destroy(&writeCfg);
  config_destroy(&readCfg);
  return TRUE;
}

/**
 * @param     fileToRead : This is the full file path of the configuration file which will be read
 * @section   This function will read input configuration file and return the filled default
 *            configuration details to the caller of this function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *            Return 1 if ERROR is there otherwise 0
 **/
void readFromFileDeviceMonitorConfigTable(
  IN UINT8                   *fileToRead, 
  IO DeviceMonitorConfig     *ptrDeviceMonitorConfig,
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
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
    config_destroy(&cfg);
    return;
  }
  table = config_lookup(&cfg, "deviceMonitorConfig");
  if(table NOTEQ NULL)
  {
    int         baseBand;
    int         radioHead;
    int         backHaulPort;
    int         debugPort;
    int         tempSensor;
    int         gps;
    int         cpri;
    int         clock;

    config_setting_lookup_int(table, "index", (int*)&ptrDeviceMonitorConfig->index); 
    config_setting_lookup_int(table, "baseBand", &baseBand); 
    ptrDeviceMonitorConfig->baseBand = baseBand;
    config_setting_lookup_int(table, "radioHead", &radioHead); 
    ptrDeviceMonitorConfig->radioHead = radioHead;
    config_setting_lookup_int(table, "backHaulPort", &backHaulPort); 
    ptrDeviceMonitorConfig->backHaulPort = backHaulPort;
    config_setting_lookup_int(table, "debugPort", &debugPort); 
    ptrDeviceMonitorConfig->debugPort = debugPort;
    config_setting_lookup_int(table, "tempSensor", &tempSensor); 
    ptrDeviceMonitorConfig->tempSensor = tempSensor;
    config_setting_lookup_int(table, "gps", &gps); 
    ptrDeviceMonitorConfig->gps = gps;
    config_setting_lookup_int(table, "cpri", &cpri); 
    ptrDeviceMonitorConfig->cpri = cpri;
    config_setting_lookup_int(table, "clock", &clock); 
    ptrDeviceMonitorConfig->clock = clock;
  }
  else
  {
    fprintf(stderr, "Table Not present : %s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
  }
  config_destroy(&cfg);
}


/**
 * @param    fileToWrite : This is the full file path of the configuration file which will be written
 * @section   This function will read Cell configuration table and fill default
 *            configurations to file fileToWrite given as input by caller function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *
 **/
TBOOL writeToFileDeviceMonitorConfigTable(
  IN DeviceMonitorConfig     *ptrDeviceMonitorConfig,
  IO UINT8                   *fileToWrite,
  IO UINT8                   *error,
  IO FILE                    *fp
  )
{
  config_t         readCfg;/* Read RO values into local structure use them while writing new cfg */
  config_setting_t *readTable;
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */
  config_init(&readCfg); /* init LibConfig */
  *error = FALSE;

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&readCfg, fileToWrite))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&readCfg),config_error_line(&readCfg), config_error_text(&readCfg));
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    return FALSE;
  }
  readTable = config_lookup(&readCfg, "deviceMonitorConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table deviceMonitorConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "deviceMonitorConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "index", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrDeviceMonitorConfig->index);
  setting = config_setting_add(group, "baseBand", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrDeviceMonitorConfig->baseBand);
  setting = config_setting_add(group, "radioHead", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrDeviceMonitorConfig->radioHead);
  setting = config_setting_add(group, "backHaulPort", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrDeviceMonitorConfig->backHaulPort);
  setting = config_setting_add(group, "debugPort", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrDeviceMonitorConfig->debugPort);
  setting = config_setting_add(group, "tempSensor", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrDeviceMonitorConfig->tempSensor);
  setting = config_setting_add(group, "gps", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrDeviceMonitorConfig->gps);
  setting = config_setting_add(group, "cpri", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrDeviceMonitorConfig->cpri);
  setting = config_setting_add(group, "clock", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrDeviceMonitorConfig->clock);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);

  config_destroy(&writeCfg);
  config_destroy(&readCfg);
  return TRUE;
}
