/**
 * @file    oper_config_read_write.c
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This is auto generated file from Excel
 * @section This file contains reader and writer functions for all Tables configurations
 *
 * Author: Lalit Chhabra
 * Date: 12/30/2010 
 * Description: Draft version 
 *              Changed Function Names for read and write -- madhu 11/10/2010  
 * Generated on : 11-02-2013 14:41:19
 **/

#include  "oper_config_read_write.h"


/**
 * @param     fileToRead : This is the full file path of the configuration file which will be read
 * @section   This function will read input configuration file and return the filled default
 *            configuration details to the caller of this function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *            Return 1 if ERROR is there otherwise 0
 **/
void readFromFileEnbRegistrationConfigTable(
  IN UINT8                   *fileToRead, 
  IO EnbRegistrationConfig   *ptrEnbRegistrationConfig,
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
  table = config_lookup(&cfg, "enbRegistrationConfig");
  if(table NOTEQ NULL)
  {
    const char *publicDNSServer1;
    const char *publicDNSServer2;
    const char *privateDNSServer1;
    const char *privateDNSServer2;
    const char *securityGW1;
    const char *securityGW2;
    const char *emsAddr;
    const char *configuredLatitude;
    const char *configuredLongitude;
    const char *snmpCommunityString;
    const char *initialLocalTime;
    const char *initialLocalTimeOffsetFromUTC;

    config_setting_lookup_int(table, "bsId", (int*)&ptrEnbRegistrationConfig->bsId); 
    config_setting_lookup_string(table, "publicDNSServer1", &publicDNSServer1); 
    strncpy(ptrEnbRegistrationConfig->publicDNSServer1,publicDNSServer1, MAX_SIZE_ENBREGISTRATIONCONFIG_PUBLICDNSSERVER1 );
    config_setting_lookup_string(table, "publicDNSServer2", &publicDNSServer2); 
    strncpy(ptrEnbRegistrationConfig->publicDNSServer2,publicDNSServer2, MAX_SIZE_ENBREGISTRATIONCONFIG_PUBLICDNSSERVER2 );
    config_setting_lookup_string(table, "privateDNSServer1", &privateDNSServer1); 
    strncpy(ptrEnbRegistrationConfig->privateDNSServer1,privateDNSServer1, MAX_SIZE_ENBREGISTRATIONCONFIG_PRIVATEDNSSERVER1 );
    config_setting_lookup_string(table, "privateDNSServer2", &privateDNSServer2); 
    strncpy(ptrEnbRegistrationConfig->privateDNSServer2,privateDNSServer2, MAX_SIZE_ENBREGISTRATIONCONFIG_PRIVATEDNSSERVER2 );
    config_setting_lookup_string(table, "securityGW1", &securityGW1); 
    strncpy(ptrEnbRegistrationConfig->securityGW1,securityGW1, MAX_SIZE_ENBREGISTRATIONCONFIG_SECURITYGW1 );
    config_setting_lookup_string(table, "securityGW2", &securityGW2); 
    strncpy(ptrEnbRegistrationConfig->securityGW2,securityGW2, MAX_SIZE_ENBREGISTRATIONCONFIG_SECURITYGW2 );
    config_setting_lookup_string(table, "emsAddr", &emsAddr); 
    strncpy(ptrEnbRegistrationConfig->emsAddr,emsAddr, MAX_SIZE_ENBREGISTRATIONCONFIG_EMSADDR );
    config_setting_lookup_string(table, "configuredLatitude", &configuredLatitude); 
    strncpy(ptrEnbRegistrationConfig->configuredLatitude,configuredLatitude, MAX_SIZE_ENBREGISTRATIONCONFIG_CONFIGUREDLATITUDE );
    config_setting_lookup_string(table, "configuredLongitude", &configuredLongitude); 
    strncpy(ptrEnbRegistrationConfig->configuredLongitude,configuredLongitude, MAX_SIZE_ENBREGISTRATIONCONFIG_CONFIGUREDLONGITUDE );
    config_setting_lookup_string(table, "snmpCommunityString", &snmpCommunityString); 
    strncpy(ptrEnbRegistrationConfig->snmpCommunityString,snmpCommunityString, MAX_SIZE_ENBREGISTRATIONCONFIG_SNMPCOMMUNITYSTRING );
    config_setting_lookup_int(table, "timeAndLocationSourceToUse", (int*)&ptrEnbRegistrationConfig->timeAndLocationSourceToUse); 
    config_setting_lookup_string(table, "initialLocalTime", &initialLocalTime); 
    strncpy(ptrEnbRegistrationConfig->initialLocalTime,initialLocalTime, MAX_SIZE_ENBREGISTRATIONCONFIG_INITIALLOCALTIME );
    config_setting_lookup_string(table, "initialLocalTimeOffsetFromUTC", &initialLocalTimeOffsetFromUTC); 
    strncpy(ptrEnbRegistrationConfig->initialLocalTimeOffsetFromUTC,initialLocalTimeOffsetFromUTC, MAX_SIZE_ENBREGISTRATIONCONFIG_INITIALLOCALTIMEOFFSETFROMUTC );
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
TBOOL writeToFileEnbRegistrationConfigTable(
  IN EnbRegistrationConfig   *ptrEnbRegistrationConfig,
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
  readTable = config_lookup(&readCfg, "enbRegistrationConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table enbRegistrationConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "enbRegistrationConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "bsId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEnbRegistrationConfig->bsId);
  setting = config_setting_add(group, "publicDNSServer1", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbRegistrationConfig->publicDNSServer1);
  setting = config_setting_add(group, "publicDNSServer2", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbRegistrationConfig->publicDNSServer2);
  setting = config_setting_add(group, "privateDNSServer1", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbRegistrationConfig->privateDNSServer1);
  setting = config_setting_add(group, "privateDNSServer2", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbRegistrationConfig->privateDNSServer2);
  setting = config_setting_add(group, "securityGW1", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbRegistrationConfig->securityGW1);
  setting = config_setting_add(group, "securityGW2", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbRegistrationConfig->securityGW2);
  setting = config_setting_add(group, "emsAddr", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbRegistrationConfig->emsAddr);
  setting = config_setting_add(group, "configuredLatitude", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbRegistrationConfig->configuredLatitude);
  setting = config_setting_add(group, "configuredLongitude", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbRegistrationConfig->configuredLongitude);
  setting = config_setting_add(group, "snmpCommunityString", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbRegistrationConfig->snmpCommunityString);
  setting = config_setting_add(group, "timeAndLocationSourceToUse", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEnbRegistrationConfig->timeAndLocationSourceToUse);
  setting = config_setting_add(group, "initialLocalTime", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbRegistrationConfig->initialLocalTime);
  setting = config_setting_add(group, "initialLocalTimeOffsetFromUTC", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnbRegistrationConfig->initialLocalTimeOffsetFromUTC);

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
void readFromFileEnodebConfigTable(
  IN UINT8                   *fileToRead, 
  IO EnodebConfig            *ptrEnodebConfig,
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
  table = config_lookup(&cfg, "enodebConfig");
  if(table NOTEQ NULL)
  {
    const char *enodebName;
    int         useStaticIpAddress;
    const char *enodebStaticIpAddr;
    const char *enodebNetmaskAddr;
    int         enodebRegistrationAccepted;
    int         configChangeAckByEms;

    config_setting_lookup_int(table, "enodebId", (int*)&ptrEnodebConfig->enodebId); 
    config_setting_lookup_string(table, "enodebName", &enodebName); 
    strncpy(ptrEnodebConfig->enodebName,enodebName, MAX_SIZE_ENODEBCONFIG_ENODEBNAME );
    config_setting_lookup_int(table, "useStaticIpAddress", &useStaticIpAddress); 
    ptrEnodebConfig->useStaticIpAddress = useStaticIpAddress;
    config_setting_lookup_string(table, "enodebStaticIpAddr", &enodebStaticIpAddr); 
    strncpy(ptrEnodebConfig->enodebStaticIpAddr,enodebStaticIpAddr, MAX_SIZE_ENODEBCONFIG_ENODEBSTATICIPADDR );
    config_setting_lookup_string(table, "enodebNetmaskAddr", &enodebNetmaskAddr); 
    strncpy(ptrEnodebConfig->enodebNetmaskAddr,enodebNetmaskAddr, MAX_SIZE_ENODEBCONFIG_ENODEBNETMASKADDR );
    config_setting_lookup_int(table, "enodebRegistrationAccepted", &enodebRegistrationAccepted); 
    ptrEnodebConfig->enodebRegistrationAccepted = enodebRegistrationAccepted;
    config_setting_lookup_int(table, "configChangeAckByEms", &configChangeAckByEms); 
    ptrEnodebConfig->configChangeAckByEms = configChangeAckByEms;
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
TBOOL writeToFileEnodebConfigTable(
  IN EnodebConfig            *ptrEnodebConfig,
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
  readTable = config_lookup(&readCfg, "enodebConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table enodebConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "enodebConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "enodebId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEnodebConfig->enodebId);
  setting = config_setting_add(group, "enodebName", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnodebConfig->enodebName);
  setting = config_setting_add(group, "useStaticIpAddress", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEnodebConfig->useStaticIpAddress);
  setting = config_setting_add(group, "enodebStaticIpAddr", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnodebConfig->enodebStaticIpAddr);
  setting = config_setting_add(group, "enodebNetmaskAddr", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEnodebConfig->enodebNetmaskAddr);
  setting = config_setting_add(group, "enodebRegistrationAccepted", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEnodebConfig->enodebRegistrationAccepted);
  setting = config_setting_add(group, "configChangeAckByEms", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEnodebConfig->configChangeAckByEms);

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
void readFromFileMmeConfigTable(
  IN UINT8                   *fileToRead, 
  IO MmeConfig               *ptrMmeConfig,
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
  table = config_lookup(&cfg, "mmeConfig");
  if(table NOTEQ NULL)
  {
    const char *mmeIpAddr;
    int         enableSecureMmeLink;
    int         enableSecureGtpTunnels;
    int         numSctpStreams;

    config_setting_lookup_int(table, "index", (int*)&ptrMmeConfig->index); 
    config_setting_lookup_string(table, "mmeIpAddr", &mmeIpAddr); 
    strncpy(ptrMmeConfig->mmeIpAddr,mmeIpAddr, MAX_SIZE_MMECONFIG_MMEIPADDR );
    config_setting_lookup_int(table, "enableSecureMmeLink", &enableSecureMmeLink); 
    ptrMmeConfig->enableSecureMmeLink = enableSecureMmeLink;
    config_setting_lookup_int(table, "enableSecureGtpTunnels", &enableSecureGtpTunnels); 
    ptrMmeConfig->enableSecureGtpTunnels = enableSecureGtpTunnels;
    config_setting_lookup_int(table, "numSctpStreams", &numSctpStreams); 
    ptrMmeConfig->numSctpStreams = numSctpStreams;
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
TBOOL writeToFileMmeConfigTable(
  IN MmeConfig               *ptrMmeConfig,
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
  readTable = config_lookup(&readCfg, "mmeConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table mmeConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "mmeConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "index", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrMmeConfig->index);
  setting = config_setting_add(group, "mmeIpAddr", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrMmeConfig->mmeIpAddr);
  setting = config_setting_add(group, "enableSecureMmeLink", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrMmeConfig->enableSecureMmeLink);
  setting = config_setting_add(group, "enableSecureGtpTunnels", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrMmeConfig->enableSecureGtpTunnels);
  setting = config_setting_add(group, "numSctpStreams", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrMmeConfig->numSctpStreams);

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
void readFromFileCellConfigTable(
  IN UINT8                   *fileToRead, 
  IO CellConfig              *ptrCellConfig,
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
  table = config_lookup(&cfg, "cellConfig");
  if(table NOTEQ NULL)
  {
    int         cellId;
    const char *plmnMCC;
    const char *plmnMNC;
    int         tac;
    int         freqBandInd;
    int         maxConnectedUes;
    int         acBarringEmergency;
    int         acBarrMoSigEnabled;
    const char *acBarrMoSigSpecialAC;
    int         acBarrMoDataEnabled;
    const char *acBarrMoDataSpecialAC;
    int         startRnti;
    int         schedulerProfileId;
    int         phyProfileId;
    int         phyLayerId;
    int         phyCellIdentityGroup;
    int         pMax;
    int         rrcConnRejWaitTime;
    int         sysInfo1ProfileId;
    int         sysInfo2ProfileId;
    int         sysInfo3ProfileId;
    int         siReTxCnt;
    int         initialUlCqi;
    int         pucchResourceSize;
    int         ueCfgMacCollatedCqiCount;

    config_setting_lookup_int(table, "cellId", &cellId); 
    ptrCellConfig->cellId = cellId;
    config_setting_lookup_int(table, "cellType", (int*)&ptrCellConfig->cellType); 
    config_setting_lookup_string(table, "plmnMCC", &plmnMCC); 
    strncpy(ptrCellConfig->plmnMCC,plmnMCC, MAX_SIZE_CELLCONFIG_PLMNMCC );
    config_setting_lookup_string(table, "plmnMNC", &plmnMNC); 
    strncpy(ptrCellConfig->plmnMNC,plmnMNC, MAX_SIZE_CELLCONFIG_PLMNMNC );
    config_setting_lookup_int(table, "tac", &tac); 
    ptrCellConfig->tac = tac;
    config_setting_lookup_int(table, "freqBandInd", &freqBandInd); 
    ptrCellConfig->freqBandInd = freqBandInd;
    config_setting_lookup_int(table, "dlBandwidth", (int*)&ptrCellConfig->dlBandwidth); 
    config_setting_lookup_int(table, "dlEarfcn", (int*)&ptrCellConfig->dlEarfcn); 
    config_setting_lookup_int(table, "ulBandwidth", (int*)&ptrCellConfig->ulBandwidth); 
    config_setting_lookup_int(table, "ulEarfcn", (int*)&ptrCellConfig->ulEarfcn); 
    config_setting_lookup_int(table, "cellReservedForOperatorUse", (int*)&ptrCellConfig->cellReservedForOperatorUse); 
    config_setting_lookup_int(table, "maxConnectedUes", &maxConnectedUes); 
    ptrCellConfig->maxConnectedUes = maxConnectedUes;
    config_setting_lookup_int(table, "acBarringEmergency", &acBarringEmergency); 
    ptrCellConfig->acBarringEmergency = acBarringEmergency;
    config_setting_lookup_int(table, "acBarrMoSigEnabled", &acBarrMoSigEnabled); 
    ptrCellConfig->acBarrMoSigEnabled = acBarrMoSigEnabled;
    config_setting_lookup_int(table, "acBarrMoSigFactor", (int*)&ptrCellConfig->acBarrMoSigFactor); 
    config_setting_lookup_int(table, "acBarrMoSigTime", (int*)&ptrCellConfig->acBarrMoSigTime); 
    config_setting_lookup_string(table, "acBarrMoSigSpecialAC", &acBarrMoSigSpecialAC); 
    strncpy(ptrCellConfig->acBarrMoSigSpecialAC,acBarrMoSigSpecialAC, MAX_SIZE_CELLCONFIG_ACBARRMOSIGSPECIALAC );
    config_setting_lookup_int(table, "acBarrMoDataEnabled", &acBarrMoDataEnabled); 
    ptrCellConfig->acBarrMoDataEnabled = acBarrMoDataEnabled;
    config_setting_lookup_int(table, "acBarrMoDataFactor", (int*)&ptrCellConfig->acBarrMoDataFactor); 
    config_setting_lookup_int(table, "acBarrMoDataTime", (int*)&ptrCellConfig->acBarrMoDataTime); 
    config_setting_lookup_string(table, "acBarrMoDataSpecialAC", &acBarrMoDataSpecialAC); 
    strncpy(ptrCellConfig->acBarrMoDataSpecialAC,acBarrMoDataSpecialAC, MAX_SIZE_CELLCONFIG_ACBARRMODATASPECIALAC );
    config_setting_lookup_int(table, "startRnti", &startRnti); 
    ptrCellConfig->startRnti = startRnti;
    config_setting_lookup_int(table, "rsrpFilterCoefficient", (int*)&ptrCellConfig->rsrpFilterCoefficient); 
    config_setting_lookup_int(table, "schedulerProfileId", &schedulerProfileId); 
    ptrCellConfig->schedulerProfileId = schedulerProfileId;
    config_setting_lookup_int(table, "phyProfileId", &phyProfileId); 
    ptrCellConfig->phyProfileId = phyProfileId;
    config_setting_lookup_int(table, "phyLayerId", &phyLayerId); 
    ptrCellConfig->phyLayerId = phyLayerId;
    config_setting_lookup_int(table, "phyCellIdentityGroup", &phyCellIdentityGroup); 
    ptrCellConfig->phyCellIdentityGroup = phyCellIdentityGroup;
    config_setting_lookup_int(table, "pMax", &pMax); 
    ptrCellConfig->pMax = pMax;
    config_setting_lookup_int(table, "ueResponseTimer", (int*)&ptrCellConfig->ueResponseTimer); 
    config_setting_lookup_int(table, "rrcConnRejWaitTime", &rrcConnRejWaitTime); 
    ptrCellConfig->rrcConnRejWaitTime = rrcConnRejWaitTime;
    config_setting_lookup_int(table, "ueInactivityCheckIntervalSecs", (int*)&ptrCellConfig->ueInactivityCheckIntervalSecs); 
    config_setting_lookup_int(table, "sysInfo1ProfileId", &sysInfo1ProfileId); 
    ptrCellConfig->sysInfo1ProfileId = sysInfo1ProfileId;
    config_setting_lookup_int(table, "sysInfo2ProfileId", &sysInfo2ProfileId); 
    ptrCellConfig->sysInfo2ProfileId = sysInfo2ProfileId;
    config_setting_lookup_int(table, "sysInfo3ProfileId", &sysInfo3ProfileId); 
    ptrCellConfig->sysInfo3ProfileId = sysInfo3ProfileId;
    config_setting_lookup_int(table, "siReTxCnt", &siReTxCnt); 
    ptrCellConfig->siReTxCnt = siReTxCnt;
    config_setting_lookup_int(table, "antInfoTxMode", (int*)&ptrCellConfig->antInfoTxMode); 
    config_setting_lookup_int(table, "initialUlCqi", &initialUlCqi); 
    ptrCellConfig->initialUlCqi = initialUlCqi;
    config_setting_lookup_int(table, "pucchResourceSize", &pucchResourceSize); 
    ptrCellConfig->pucchResourceSize = pucchResourceSize;
    config_setting_lookup_int(table, "cqiPeriodicityIdx", (int*)&ptrCellConfig->cqiPeriodicityIdx); 
    config_setting_lookup_int(table, "sriPeriodicityIdx", (int*)&ptrCellConfig->sriPeriodicityIdx); 
    config_setting_lookup_int(table, "ueCqiBasedRlfDetectionTimer", (int*)&ptrCellConfig->ueCqiBasedRlfDetectionTimer); 
    config_setting_lookup_int(table, "ueCfgMacCollatedCqiCount", &ueCfgMacCollatedCqiCount); 
    ptrCellConfig->ueCfgMacCollatedCqiCount = ueCfgMacCollatedCqiCount;
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
TBOOL writeToFileCellConfigTable(
  IN CellConfig              *ptrCellConfig,
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
  readTable = config_lookup(&readCfg, "cellConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table cellConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "cellConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "cellId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->cellId);
  setting = config_setting_add(group, "cellType", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->cellType);
  setting = config_setting_add(group, "plmnMCC", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrCellConfig->plmnMCC);
  setting = config_setting_add(group, "plmnMNC", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrCellConfig->plmnMNC);
  setting = config_setting_add(group, "tac", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->tac);
  setting = config_setting_add(group, "freqBandInd", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->freqBandInd);
  setting = config_setting_add(group, "dlBandwidth", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->dlBandwidth);
  setting = config_setting_add(group, "dlEarfcn", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->dlEarfcn);
  setting = config_setting_add(group, "ulBandwidth", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->ulBandwidth);
  setting = config_setting_add(group, "ulEarfcn", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->ulEarfcn);
  setting = config_setting_add(group, "cellReservedForOperatorUse", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->cellReservedForOperatorUse);
  setting = config_setting_add(group, "maxConnectedUes", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->maxConnectedUes);
  setting = config_setting_add(group, "acBarringEmergency", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->acBarringEmergency);
  setting = config_setting_add(group, "acBarrMoSigEnabled", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->acBarrMoSigEnabled);
  setting = config_setting_add(group, "acBarrMoSigFactor", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->acBarrMoSigFactor);
  setting = config_setting_add(group, "acBarrMoSigTime", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->acBarrMoSigTime);
  setting = config_setting_add(group, "acBarrMoSigSpecialAC", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrCellConfig->acBarrMoSigSpecialAC);
  setting = config_setting_add(group, "acBarrMoDataEnabled", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->acBarrMoDataEnabled);
  setting = config_setting_add(group, "acBarrMoDataFactor", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->acBarrMoDataFactor);
  setting = config_setting_add(group, "acBarrMoDataTime", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->acBarrMoDataTime);
  setting = config_setting_add(group, "acBarrMoDataSpecialAC", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrCellConfig->acBarrMoDataSpecialAC);
  setting = config_setting_add(group, "startRnti", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->startRnti);
  setting = config_setting_add(group, "rsrpFilterCoefficient", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->rsrpFilterCoefficient);
  setting = config_setting_add(group, "schedulerProfileId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->schedulerProfileId);
  setting = config_setting_add(group, "phyProfileId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->phyProfileId);
  setting = config_setting_add(group, "phyLayerId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->phyLayerId);
  setting = config_setting_add(group, "phyCellIdentityGroup", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->phyCellIdentityGroup);
  setting = config_setting_add(group, "pMax", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->pMax);
  setting = config_setting_add(group, "ueResponseTimer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->ueResponseTimer);
  setting = config_setting_add(group, "rrcConnRejWaitTime", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->rrcConnRejWaitTime);
  setting = config_setting_add(group, "ueInactivityCheckIntervalSecs", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->ueInactivityCheckIntervalSecs);
  setting = config_setting_add(group, "sysInfo1ProfileId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->sysInfo1ProfileId);
  setting = config_setting_add(group, "sysInfo2ProfileId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->sysInfo2ProfileId);
  setting = config_setting_add(group, "sysInfo3ProfileId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->sysInfo3ProfileId);
  setting = config_setting_add(group, "siReTxCnt", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->siReTxCnt);
  setting = config_setting_add(group, "antInfoTxMode", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->antInfoTxMode);
  setting = config_setting_add(group, "initialUlCqi", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->initialUlCqi);
  setting = config_setting_add(group, "pucchResourceSize", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->pucchResourceSize);
  setting = config_setting_add(group, "cqiPeriodicityIdx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->cqiPeriodicityIdx);
  setting = config_setting_add(group, "sriPeriodicityIdx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->sriPeriodicityIdx);
  setting = config_setting_add(group, "ueCqiBasedRlfDetectionTimer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->ueCqiBasedRlfDetectionTimer);
  setting = config_setting_add(group, "ueCfgMacCollatedCqiCount", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrCellConfig->ueCfgMacCollatedCqiCount);

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
void readFromFileSchedulerConfigTable(
  IN UINT8                   *fileToRead, 
  IO SchedulerConfig         *ptrSchedulerConfig,
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
  table = config_lookup(&cfg, "schedulerConfig");
  if(table NOTEQ NULL)
  {
    int         schedulerProfileId;
    int         maxDlHarqTx;
    int         maxMsg4HarqTx;
    int         pcfichCfi;

    config_setting_lookup_int(table, "schedulerProfileId", &schedulerProfileId); 
    ptrSchedulerConfig->schedulerProfileId = schedulerProfileId;
    config_setting_lookup_int(table, "schedulingAlgorithm", (int*)&ptrSchedulerConfig->schedulingAlgorithm); 
    config_setting_lookup_int(table, "maxDlHarqTx", &maxDlHarqTx); 
    ptrSchedulerConfig->maxDlHarqTx = maxDlHarqTx;
    config_setting_lookup_int(table, "maxUlHarqTx", (int*)&ptrSchedulerConfig->maxUlHarqTx); 
    config_setting_lookup_int(table, "maxMsg4HarqTx", &maxMsg4HarqTx); 
    ptrSchedulerConfig->maxMsg4HarqTx = maxMsg4HarqTx;
    config_setting_lookup_int(table, "pcfichCfi", &pcfichCfi); 
    ptrSchedulerConfig->pcfichCfi = pcfichCfi;
    config_setting_lookup_int(table, "periodicPhrTimer", (int*)&ptrSchedulerConfig->periodicPhrTimer); 
    config_setting_lookup_int(table, "prohibitPhrTimer", (int*)&ptrSchedulerConfig->prohibitPhrTimer); 
    config_setting_lookup_int(table, "dlPathlossChange", (int*)&ptrSchedulerConfig->dlPathlossChange); 
    config_setting_lookup_int(table, "periodicBsrTimer", (int*)&ptrSchedulerConfig->periodicBsrTimer); 
    config_setting_lookup_int(table, "retxBsrTimer", (int*)&ptrSchedulerConfig->retxBsrTimer); 
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
TBOOL writeToFileSchedulerConfigTable(
  IN SchedulerConfig         *ptrSchedulerConfig,
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
  readTable = config_lookup(&readCfg, "schedulerConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table schedulerConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "schedulerConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "schedulerProfileId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSchedulerConfig->schedulerProfileId);
  setting = config_setting_add(group, "schedulingAlgorithm", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSchedulerConfig->schedulingAlgorithm);
  setting = config_setting_add(group, "maxDlHarqTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSchedulerConfig->maxDlHarqTx);
  setting = config_setting_add(group, "maxUlHarqTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSchedulerConfig->maxUlHarqTx);
  setting = config_setting_add(group, "maxMsg4HarqTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSchedulerConfig->maxMsg4HarqTx);
  setting = config_setting_add(group, "pcfichCfi", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSchedulerConfig->pcfichCfi);
  setting = config_setting_add(group, "periodicPhrTimer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSchedulerConfig->periodicPhrTimer);
  setting = config_setting_add(group, "prohibitPhrTimer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSchedulerConfig->prohibitPhrTimer);
  setting = config_setting_add(group, "dlPathlossChange", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSchedulerConfig->dlPathlossChange);
  setting = config_setting_add(group, "periodicBsrTimer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSchedulerConfig->periodicBsrTimer);
  setting = config_setting_add(group, "retxBsrTimer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSchedulerConfig->retxBsrTimer);

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
void readFromFilePhyConfigTable(
  IN UINT8                   *fileToRead, 
  IO PhyConfig               *ptrPhyConfig,
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
  table = config_lookup(&cfg, "phyConfig");
  if(table NOTEQ NULL)
  {
    int         phyProfileId;
    int         prachFreqOffset;
    int         referenceSignalPower;
    int         cellPhyPb;
    int         srsEnabled;
    int         poNominalPucch;
    int         betaOffsetAckIndex;
    int         betaOffsetRiIndex;
    int         betaOffsetCqiIndex;
    int         poUePusch;
    int         poUePucch;
    int         pSrsOffset;
    int         totalTxPower;

    config_setting_lookup_int(table, "phyProfileId", &phyProfileId); 
    ptrPhyConfig->phyProfileId = phyProfileId;
    config_setting_lookup_int(table, "phichResource", (int*)&ptrPhyConfig->phichResource); 
    config_setting_lookup_int(table, "prachFreqOffset", &prachFreqOffset); 
    ptrPhyConfig->prachFreqOffset = prachFreqOffset;
    config_setting_lookup_int(table, "referenceSignalPower", &referenceSignalPower); 
    ptrPhyConfig->referenceSignalPower = referenceSignalPower;
    config_setting_lookup_int(table, "cellPhyPb", &cellPhyPb); 
    ptrPhyConfig->cellPhyPb = cellPhyPb;
    config_setting_lookup_int(table, "srsEnabled", &srsEnabled); 
    ptrPhyConfig->srsEnabled = srsEnabled;
    config_setting_lookup_int(table, "alpha", (int*)&ptrPhyConfig->alpha); 
    config_setting_lookup_int(table, "poNominalPucch", &poNominalPucch); 
    ptrPhyConfig->poNominalPucch = poNominalPucch;
    config_setting_lookup_int(table, "cellPhyPa", (int*)&ptrPhyConfig->cellPhyPa); 
    config_setting_lookup_int(table, "betaOffsetAckIndex", &betaOffsetAckIndex); 
    ptrPhyConfig->betaOffsetAckIndex = betaOffsetAckIndex;
    config_setting_lookup_int(table, "betaOffsetRiIndex", &betaOffsetRiIndex); 
    ptrPhyConfig->betaOffsetRiIndex = betaOffsetRiIndex;
    config_setting_lookup_int(table, "betaOffsetCqiIndex", &betaOffsetCqiIndex); 
    ptrPhyConfig->betaOffsetCqiIndex = betaOffsetCqiIndex;
    config_setting_lookup_int(table, "poUePusch", &poUePusch); 
    ptrPhyConfig->poUePusch = poUePusch;
    config_setting_lookup_int(table, "poUePucch", &poUePucch); 
    ptrPhyConfig->poUePucch = poUePucch;
    config_setting_lookup_int(table, "pSrsOffset", &pSrsOffset); 
    ptrPhyConfig->pSrsOffset = pSrsOffset;
    config_setting_lookup_int(table, "phichDuration", (int*)&ptrPhyConfig->phichDuration); 
    config_setting_lookup_int(table, "totalTxPower", &totalTxPower); 
    ptrPhyConfig->totalTxPower = totalTxPower;
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
TBOOL writeToFilePhyConfigTable(
  IN PhyConfig               *ptrPhyConfig,
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
  readTable = config_lookup(&readCfg, "phyConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table phyConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "phyConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "phyProfileId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->phyProfileId);
  setting = config_setting_add(group, "phichResource", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->phichResource);
  setting = config_setting_add(group, "prachFreqOffset", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->prachFreqOffset);
  setting = config_setting_add(group, "referenceSignalPower", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->referenceSignalPower);
  setting = config_setting_add(group, "cellPhyPb", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->cellPhyPb);
  setting = config_setting_add(group, "srsEnabled", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->srsEnabled);
  setting = config_setting_add(group, "alpha", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->alpha);
  setting = config_setting_add(group, "poNominalPucch", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->poNominalPucch);
  setting = config_setting_add(group, "cellPhyPa", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->cellPhyPa);
  setting = config_setting_add(group, "betaOffsetAckIndex", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->betaOffsetAckIndex);
  setting = config_setting_add(group, "betaOffsetRiIndex", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->betaOffsetRiIndex);
  setting = config_setting_add(group, "betaOffsetCqiIndex", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->betaOffsetCqiIndex);
  setting = config_setting_add(group, "poUePusch", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->poUePusch);
  setting = config_setting_add(group, "poUePucch", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->poUePucch);
  setting = config_setting_add(group, "pSrsOffset", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->pSrsOffset);
  setting = config_setting_add(group, "phichDuration", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->phichDuration);
  setting = config_setting_add(group, "totalTxPower", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPhyConfig->totalTxPower);

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
void readFromFileSysInfo1ConfigTable(
  IN UINT8                   *fileToRead, 
  IO SysInfo1Config          *ptrSysInfo1Config,
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
  table = config_lookup(&cfg, "sysInfo1Config");
  if(table NOTEQ NULL)
  {
    int         sysInfo1ProfileId;
    int         qRxLevMin;
    int         qRxLevMinOffset;

    config_setting_lookup_int(table, "sysInfo1ProfileId", &sysInfo1ProfileId); 
    ptrSysInfo1Config->sysInfo1ProfileId = sysInfo1ProfileId;
    config_setting_lookup_int(table, "intraFreqReselection", (int*)&ptrSysInfo1Config->intraFreqReselection); 
    config_setting_lookup_int(table, "qRxLevMin", &qRxLevMin); 
    ptrSysInfo1Config->qRxLevMin = qRxLevMin;
    config_setting_lookup_int(table, "qRxLevMinOffset", &qRxLevMinOffset); 
    ptrSysInfo1Config->qRxLevMinOffset = qRxLevMinOffset;
    config_setting_lookup_int(table, "sib2Periodicity", (int*)&ptrSysInfo1Config->sib2Periodicity); 
    config_setting_lookup_int(table, "sib3Periodicity", (int*)&ptrSysInfo1Config->sib3Periodicity); 
    config_setting_lookup_int(table, "sib4Periodicity", (int*)&ptrSysInfo1Config->sib4Periodicity); 
    config_setting_lookup_int(table, "siWindowLength", (int*)&ptrSysInfo1Config->siWindowLength); 
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
TBOOL writeToFileSysInfo1ConfigTable(
  IN SysInfo1Config          *ptrSysInfo1Config,
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
  readTable = config_lookup(&readCfg, "sysInfo1Config");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table sysInfo1Config doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "sysInfo1Config", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "sysInfo1ProfileId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo1Config->sysInfo1ProfileId);
  setting = config_setting_add(group, "intraFreqReselection", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo1Config->intraFreqReselection);
  setting = config_setting_add(group, "qRxLevMin", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo1Config->qRxLevMin);
  setting = config_setting_add(group, "qRxLevMinOffset", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo1Config->qRxLevMinOffset);
  setting = config_setting_add(group, "sib2Periodicity", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo1Config->sib2Periodicity);
  setting = config_setting_add(group, "sib3Periodicity", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo1Config->sib3Periodicity);
  setting = config_setting_add(group, "sib4Periodicity", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo1Config->sib4Periodicity);
  setting = config_setting_add(group, "siWindowLength", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo1Config->siWindowLength);

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
void readFromFileSysInfo2ConfigTable(
  IN UINT8                   *fileToRead, 
  IO SysInfo2Config          *ptrSysInfo2Config,
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
  table = config_lookup(&cfg, "sysInfo2Config");
  if(table NOTEQ NULL)
  {
    int         sysInfo2ProfileId;
    int         maxHarqMsg3Tx;
    int         rootSequenceIndex;
    int         prachConfigIndex;
    int         highSpeedFlag;
    int         zeroCorrelationZoneConfig;
    int         puschFreqHoppingSubBands;
    int         puschFreqHoppingOffset;
    int         ulRsGroupHoppingEnabled;
    int         ulRsGroupPusch;
    int         ulRsSeqHoppingEnabled;
    int         ulRsCyclicShiftPusch;
    int         n1PucchAn;
    int         srsSimultaneousAnSrsTx;
    int         poNominalPusch;
    int         deltaPreambleMsg3;
    int         additionalSpectrumEmission;
    int         nRbCqi;
    int         enable64QAM;
    int         nCsAn;

    config_setting_lookup_int(table, "sysInfo2ProfileId", &sysInfo2ProfileId); 
    ptrSysInfo2Config->sysInfo2ProfileId = sysInfo2ProfileId;
    config_setting_lookup_int(table, "numRaPreambles", (int*)&ptrSysInfo2Config->numRaPreambles); 
    config_setting_lookup_int(table, "sizeRAPreamblesGroupA", (int*)&ptrSysInfo2Config->sizeRAPreamblesGroupA); 
    config_setting_lookup_int(table, "messageSizeGroupA", (int*)&ptrSysInfo2Config->messageSizeGroupA); 
    config_setting_lookup_int(table, "messagePowerOffsetGroupB", (int*)&ptrSysInfo2Config->messagePowerOffsetGroupB); 
    config_setting_lookup_int(table, "powerRampingStep", (int*)&ptrSysInfo2Config->powerRampingStep); 
    config_setting_lookup_int(table, "preambleInitRcvdTgtPower", (int*)&ptrSysInfo2Config->preambleInitRcvdTgtPower); 
    config_setting_lookup_int(table, "preambleTransMax", (int*)&ptrSysInfo2Config->preambleTransMax); 
    config_setting_lookup_int(table, "raRspWindowSize", (int*)&ptrSysInfo2Config->raRspWindowSize); 
    config_setting_lookup_int(table, "macContResolutionTimer", (int*)&ptrSysInfo2Config->macContResolutionTimer); 
    config_setting_lookup_int(table, "maxHarqMsg3Tx", &maxHarqMsg3Tx); 
    ptrSysInfo2Config->maxHarqMsg3Tx = maxHarqMsg3Tx;
    config_setting_lookup_int(table, "modificationPeriodCoeff", (int*)&ptrSysInfo2Config->modificationPeriodCoeff); 
    config_setting_lookup_int(table, "defaultPagCycle", (int*)&ptrSysInfo2Config->defaultPagCycle); 
    config_setting_lookup_int(table, "nB", (int*)&ptrSysInfo2Config->nB); 
    config_setting_lookup_int(table, "rootSequenceIndex", &rootSequenceIndex); 
    ptrSysInfo2Config->rootSequenceIndex = rootSequenceIndex;
    config_setting_lookup_int(table, "prachConfigIndex", &prachConfigIndex); 
    ptrSysInfo2Config->prachConfigIndex = prachConfigIndex;
    config_setting_lookup_int(table, "highSpeedFlag", &highSpeedFlag); 
    ptrSysInfo2Config->highSpeedFlag = highSpeedFlag;
    config_setting_lookup_int(table, "zeroCorrelationZoneConfig", &zeroCorrelationZoneConfig); 
    ptrSysInfo2Config->zeroCorrelationZoneConfig = zeroCorrelationZoneConfig;
    config_setting_lookup_int(table, "puschFreqHoppingSubBands", &puschFreqHoppingSubBands); 
    ptrSysInfo2Config->puschFreqHoppingSubBands = puschFreqHoppingSubBands;
    config_setting_lookup_int(table, "puschFreqHoppingMode", (int*)&ptrSysInfo2Config->puschFreqHoppingMode); 
    config_setting_lookup_int(table, "puschFreqHoppingOffset", &puschFreqHoppingOffset); 
    ptrSysInfo2Config->puschFreqHoppingOffset = puschFreqHoppingOffset;
    config_setting_lookup_int(table, "ulRsGroupHoppingEnabled", &ulRsGroupHoppingEnabled); 
    ptrSysInfo2Config->ulRsGroupHoppingEnabled = ulRsGroupHoppingEnabled;
    config_setting_lookup_int(table, "ulRsGroupPusch", &ulRsGroupPusch); 
    ptrSysInfo2Config->ulRsGroupPusch = ulRsGroupPusch;
    config_setting_lookup_int(table, "ulRsSeqHoppingEnabled", &ulRsSeqHoppingEnabled); 
    ptrSysInfo2Config->ulRsSeqHoppingEnabled = ulRsSeqHoppingEnabled;
    config_setting_lookup_int(table, "ulRsCyclicShiftPusch", &ulRsCyclicShiftPusch); 
    ptrSysInfo2Config->ulRsCyclicShiftPusch = ulRsCyclicShiftPusch;
    config_setting_lookup_int(table, "deltaPucchShift", (int*)&ptrSysInfo2Config->deltaPucchShift); 
    config_setting_lookup_int(table, "n1PucchAn", &n1PucchAn); 
    ptrSysInfo2Config->n1PucchAn = n1PucchAn;
    config_setting_lookup_int(table, "srsBandwidthConfig", (int*)&ptrSysInfo2Config->srsBandwidthConfig); 
    config_setting_lookup_int(table, "srsSubfrmConfig", (int*)&ptrSysInfo2Config->srsSubfrmConfig); 
    config_setting_lookup_int(table, "srsSimultaneousAnSrsTx", &srsSimultaneousAnSrsTx); 
    ptrSysInfo2Config->srsSimultaneousAnSrsTx = srsSimultaneousAnSrsTx;
    config_setting_lookup_int(table, "poNominalPusch", &poNominalPusch); 
    ptrSysInfo2Config->poNominalPusch = poNominalPusch;
    config_setting_lookup_int(table, "deltaFPucchFormat1", (int*)&ptrSysInfo2Config->deltaFPucchFormat1); 
    config_setting_lookup_int(table, "deltaFPucchFormat1b", (int*)&ptrSysInfo2Config->deltaFPucchFormat1b); 
    config_setting_lookup_int(table, "deltaFPucchFormat2", (int*)&ptrSysInfo2Config->deltaFPucchFormat2); 
    config_setting_lookup_int(table, "deltaFPucchFormat2a", (int*)&ptrSysInfo2Config->deltaFPucchFormat2a); 
    config_setting_lookup_int(table, "deltaFPucchFormat2b", (int*)&ptrSysInfo2Config->deltaFPucchFormat2b); 
    config_setting_lookup_int(table, "deltaPreambleMsg3", &deltaPreambleMsg3); 
    ptrSysInfo2Config->deltaPreambleMsg3 = deltaPreambleMsg3;
    config_setting_lookup_int(table, "t300", (int*)&ptrSysInfo2Config->t300); 
    config_setting_lookup_int(table, "t301", (int*)&ptrSysInfo2Config->t301); 
    config_setting_lookup_int(table, "t310", (int*)&ptrSysInfo2Config->t310); 
    config_setting_lookup_int(table, "n310", (int*)&ptrSysInfo2Config->n310); 
    config_setting_lookup_int(table, "t311", (int*)&ptrSysInfo2Config->t311); 
    config_setting_lookup_int(table, "n311", (int*)&ptrSysInfo2Config->n311); 
    config_setting_lookup_int(table, "additionalSpectrumEmission", &additionalSpectrumEmission); 
    ptrSysInfo2Config->additionalSpectrumEmission = additionalSpectrumEmission;
    config_setting_lookup_int(table, "timeAlignmentTimer", (int*)&ptrSysInfo2Config->timeAlignmentTimer); 
    config_setting_lookup_int(table, "nRbCqi", &nRbCqi); 
    ptrSysInfo2Config->nRbCqi = nRbCqi;
    config_setting_lookup_int(table, "enable64QAM", &enable64QAM); 
    ptrSysInfo2Config->enable64QAM = enable64QAM;
    config_setting_lookup_int(table, "nCsAn", &nCsAn); 
    ptrSysInfo2Config->nCsAn = nCsAn;
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
TBOOL writeToFileSysInfo2ConfigTable(
  IN SysInfo2Config          *ptrSysInfo2Config,
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
  readTable = config_lookup(&readCfg, "sysInfo2Config");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table sysInfo2Config doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "sysInfo2Config", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "sysInfo2ProfileId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->sysInfo2ProfileId);
  setting = config_setting_add(group, "numRaPreambles", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->numRaPreambles);
  setting = config_setting_add(group, "sizeRAPreamblesGroupA", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->sizeRAPreamblesGroupA);
  setting = config_setting_add(group, "messageSizeGroupA", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->messageSizeGroupA);
  setting = config_setting_add(group, "messagePowerOffsetGroupB", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->messagePowerOffsetGroupB);
  setting = config_setting_add(group, "powerRampingStep", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->powerRampingStep);
  setting = config_setting_add(group, "preambleInitRcvdTgtPower", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->preambleInitRcvdTgtPower);
  setting = config_setting_add(group, "preambleTransMax", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->preambleTransMax);
  setting = config_setting_add(group, "raRspWindowSize", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->raRspWindowSize);
  setting = config_setting_add(group, "macContResolutionTimer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->macContResolutionTimer);
  setting = config_setting_add(group, "maxHarqMsg3Tx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->maxHarqMsg3Tx);
  setting = config_setting_add(group, "modificationPeriodCoeff", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->modificationPeriodCoeff);
  setting = config_setting_add(group, "defaultPagCycle", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->defaultPagCycle);
  setting = config_setting_add(group, "nB", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->nB);
  setting = config_setting_add(group, "rootSequenceIndex", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->rootSequenceIndex);
  setting = config_setting_add(group, "prachConfigIndex", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->prachConfigIndex);
  setting = config_setting_add(group, "highSpeedFlag", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->highSpeedFlag);
  setting = config_setting_add(group, "zeroCorrelationZoneConfig", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->zeroCorrelationZoneConfig);
  setting = config_setting_add(group, "puschFreqHoppingSubBands", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->puschFreqHoppingSubBands);
  setting = config_setting_add(group, "puschFreqHoppingMode", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->puschFreqHoppingMode);
  setting = config_setting_add(group, "puschFreqHoppingOffset", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->puschFreqHoppingOffset);
  setting = config_setting_add(group, "ulRsGroupHoppingEnabled", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->ulRsGroupHoppingEnabled);
  setting = config_setting_add(group, "ulRsGroupPusch", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->ulRsGroupPusch);
  setting = config_setting_add(group, "ulRsSeqHoppingEnabled", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->ulRsSeqHoppingEnabled);
  setting = config_setting_add(group, "ulRsCyclicShiftPusch", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->ulRsCyclicShiftPusch);
  setting = config_setting_add(group, "deltaPucchShift", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->deltaPucchShift);
  setting = config_setting_add(group, "n1PucchAn", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->n1PucchAn);
  setting = config_setting_add(group, "srsBandwidthConfig", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->srsBandwidthConfig);
  setting = config_setting_add(group, "srsSubfrmConfig", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->srsSubfrmConfig);
  setting = config_setting_add(group, "srsSimultaneousAnSrsTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->srsSimultaneousAnSrsTx);
  setting = config_setting_add(group, "poNominalPusch", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->poNominalPusch);
  setting = config_setting_add(group, "deltaFPucchFormat1", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->deltaFPucchFormat1);
  setting = config_setting_add(group, "deltaFPucchFormat1b", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->deltaFPucchFormat1b);
  setting = config_setting_add(group, "deltaFPucchFormat2", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->deltaFPucchFormat2);
  setting = config_setting_add(group, "deltaFPucchFormat2a", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->deltaFPucchFormat2a);
  setting = config_setting_add(group, "deltaFPucchFormat2b", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->deltaFPucchFormat2b);
  setting = config_setting_add(group, "deltaPreambleMsg3", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->deltaPreambleMsg3);
  setting = config_setting_add(group, "t300", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->t300);
  setting = config_setting_add(group, "t301", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->t301);
  setting = config_setting_add(group, "t310", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->t310);
  setting = config_setting_add(group, "n310", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->n310);
  setting = config_setting_add(group, "t311", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->t311);
  setting = config_setting_add(group, "n311", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->n311);
  setting = config_setting_add(group, "additionalSpectrumEmission", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->additionalSpectrumEmission);
  setting = config_setting_add(group, "timeAlignmentTimer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->timeAlignmentTimer);
  setting = config_setting_add(group, "nRbCqi", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->nRbCqi);
  setting = config_setting_add(group, "enable64QAM", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->enable64QAM);
  setting = config_setting_add(group, "nCsAn", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo2Config->nCsAn);

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
NumEntry readFromFileLogConfigTable(
  IN UINT8                   *fileToRead, 
  IO LogConfig               *ptrLogConfig,
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
  setting = config_lookup(&cfg, "logConfig");
  if(setting NOTEQ NULL)
  {
    int count = config_setting_length(setting);
    int       numElem;
    for(numElem = 0;numElem < count; ++numElem)
    {

      config_setting_t *table = config_setting_get_elem(setting, numElem);
      /* only output the record if all of the expected fields are present */

      config_setting_lookup_int(table, "moduleId", (int*)&ptrLogConfig->moduleId); 
      config_setting_lookup_int(table, "logMask1", (int*)&ptrLogConfig->logMask1); 
      ptrLogConfig++;
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
TBOOL writeToFileLogConfigTable(
  IN LogConfig               *ptrLogConfig,
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
  readTable = config_lookup(&readCfg, "logConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table logConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "logConfig", CONFIG_TYPE_LIST);

  int count = config_setting_length(readTable);
  int numElem = 0;
  for(numElem = 0; numElem < count; ++numElem)
  {
    /* Create the new entry */
    cfgEntry = config_setting_add(group, NULL, CONFIG_TYPE_GROUP);

    setting = config_setting_add(cfgEntry, "moduleId", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrLogConfig->moduleId);
    setting = config_setting_add(cfgEntry, "logMask1", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrLogConfig->logMask1);
    ptrLogConfig++;
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
void readFromFileRadioHeadConfigTable(
  IN UINT8                   *fileToRead, 
  IO RadioHeadConfig         *ptrRadioHeadConfig,
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
  table = config_lookup(&cfg, "radioHeadConfig");
  if(table NOTEQ NULL)
  {
    int         txAttenPathA;
    int         txAttenPathB;
    int         vswrShutdownAlarmThreshold;

    config_setting_lookup_int(table, "radioHeadProfileId", (int*)&ptrRadioHeadConfig->radioHeadProfileId); 
    config_setting_lookup_int(table, "txAttenPathA", &txAttenPathA); 
    ptrRadioHeadConfig->txAttenPathA = txAttenPathA;
    config_setting_lookup_int(table, "txAttenPathB", &txAttenPathB); 
    ptrRadioHeadConfig->txAttenPathB = txAttenPathB;
    config_setting_lookup_int(table, "txDelay", (int*)&ptrRadioHeadConfig->txDelay); 
    config_setting_lookup_int(table, "rxDelay", (int*)&ptrRadioHeadConfig->rxDelay); 
    config_setting_lookup_int(table, "vswrShutdownAlarmThreshold", &vswrShutdownAlarmThreshold); 
    ptrRadioHeadConfig->vswrShutdownAlarmThreshold = vswrShutdownAlarmThreshold;
    config_setting_lookup_int(table, "overpowerAlarmThreshold", (int*)&ptrRadioHeadConfig->overpowerAlarmThreshold); 
    config_setting_lookup_int(table, "lowpowerAlarmThreshold", (int*)&ptrRadioHeadConfig->lowpowerAlarmThreshold); 
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
TBOOL writeToFileRadioHeadConfigTable(
  IN RadioHeadConfig         *ptrRadioHeadConfig,
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
  readTable = config_lookup(&readCfg, "radioHeadConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table radioHeadConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "radioHeadConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "radioHeadProfileId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrRadioHeadConfig->radioHeadProfileId);
  setting = config_setting_add(group, "txAttenPathA", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrRadioHeadConfig->txAttenPathA);
  setting = config_setting_add(group, "txAttenPathB", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrRadioHeadConfig->txAttenPathB);
  setting = config_setting_add(group, "txDelay", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrRadioHeadConfig->txDelay);
  setting = config_setting_add(group, "rxDelay", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrRadioHeadConfig->rxDelay);
  setting = config_setting_add(group, "vswrShutdownAlarmThreshold", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrRadioHeadConfig->vswrShutdownAlarmThreshold);
  setting = config_setting_add(group, "overpowerAlarmThreshold", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrRadioHeadConfig->overpowerAlarmThreshold);
  setting = config_setting_add(group, "lowpowerAlarmThreshold", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrRadioHeadConfig->lowpowerAlarmThreshold);

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
void readFromFileQosConfigTable(
  IN UINT8                   *fileToRead, 
  IO QosConfig               *ptrQosConfig,
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
  table = config_lookup(&cfg, "qosConfig");
  if(table NOTEQ NULL)
  {
    const char *qosProfileName;
    int         qci;
    int         statusReportRequired;
    int         drbPriority;
    int         logicalChannelGroup;

    config_setting_lookup_int(table, "qosProfileId", (int*)&ptrQosConfig->qosProfileId); 
    config_setting_lookup_string(table, "qosProfileName", &qosProfileName); 
    strncpy(ptrQosConfig->qosProfileName,qosProfileName, MAX_SIZE_QOSCONFIG_QOSPROFILENAME );
    config_setting_lookup_int(table, "qci", &qci); 
    ptrQosConfig->qci = qci;
    config_setting_lookup_int(table, "rlcMode", (int*)&ptrQosConfig->rlcMode); 
    config_setting_lookup_int(table, "amRlcTPollRetransmit", (int*)&ptrQosConfig->amRlcTPollRetransmit); 
    config_setting_lookup_int(table, "amRlcPollPDU", (int*)&ptrQosConfig->amRlcPollPDU); 
    config_setting_lookup_int(table, "amRlcPollByte", (int*)&ptrQosConfig->amRlcPollByte); 
    config_setting_lookup_int(table, "amRlcMaxRetxThreshold", (int*)&ptrQosConfig->amRlcMaxRetxThreshold); 
    config_setting_lookup_int(table, "amRlcTReordering", (int*)&ptrQosConfig->amRlcTReordering); 
    config_setting_lookup_int(table, "amRlcTStatusProhibit", (int*)&ptrQosConfig->amRlcTStatusProhibit); 
    config_setting_lookup_int(table, "discardTimer", (int*)&ptrQosConfig->discardTimer); 
    config_setting_lookup_int(table, "statusReportRequired", &statusReportRequired); 
    ptrQosConfig->statusReportRequired = statusReportRequired;
    config_setting_lookup_int(table, "drbPriority", &drbPriority); 
    ptrQosConfig->drbPriority = drbPriority;
    config_setting_lookup_int(table, "drbPrioritisedBitRate", (int*)&ptrQosConfig->drbPrioritisedBitRate); 
    config_setting_lookup_int(table, "drbBucketSizeDuration", (int*)&ptrQosConfig->drbBucketSizeDuration); 
    config_setting_lookup_int(table, "logicalChannelGroup", &logicalChannelGroup); 
    ptrQosConfig->logicalChannelGroup = logicalChannelGroup;
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
TBOOL writeToFileQosConfigTable(
  IN QosConfig               *ptrQosConfig,
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
  readTable = config_lookup(&readCfg, "qosConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table qosConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "qosConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "qosProfileId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrQosConfig->qosProfileId);
  setting = config_setting_add(group, "qosProfileName", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrQosConfig->qosProfileName);
  setting = config_setting_add(group, "qci", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrQosConfig->qci);
  setting = config_setting_add(group, "rlcMode", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrQosConfig->rlcMode);
  setting = config_setting_add(group, "amRlcTPollRetransmit", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrQosConfig->amRlcTPollRetransmit);
  setting = config_setting_add(group, "amRlcPollPDU", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrQosConfig->amRlcPollPDU);
  setting = config_setting_add(group, "amRlcPollByte", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrQosConfig->amRlcPollByte);
  setting = config_setting_add(group, "amRlcMaxRetxThreshold", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrQosConfig->amRlcMaxRetxThreshold);
  setting = config_setting_add(group, "amRlcTReordering", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrQosConfig->amRlcTReordering);
  setting = config_setting_add(group, "amRlcTStatusProhibit", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrQosConfig->amRlcTStatusProhibit);
  setting = config_setting_add(group, "discardTimer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrQosConfig->discardTimer);
  setting = config_setting_add(group, "statusReportRequired", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrQosConfig->statusReportRequired);
  setting = config_setting_add(group, "drbPriority", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrQosConfig->drbPriority);
  setting = config_setting_add(group, "drbPrioritisedBitRate", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrQosConfig->drbPrioritisedBitRate);
  setting = config_setting_add(group, "drbBucketSizeDuration", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrQosConfig->drbBucketSizeDuration);
  setting = config_setting_add(group, "logicalChannelGroup", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrQosConfig->logicalChannelGroup);

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
void readFromFilePmCounterLogConfigTable(
  IN UINT8                   *fileToRead, 
  IO PmCounterLogConfig      *ptrPmCounterLogConfig,
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
  table = config_lookup(&cfg, "pmCounterLogConfig");
  if(table NOTEQ NULL)
  {
    int         collectMacGenCounters;
    int         collectMacUplaneCounters;
    int         collectRlcGenCounters;
    int         collectPdcpGenCounters;
    int         collectPdcpUplaneCounters;
    int         collectGtpGenCounters;
    int         collectSctpGenCounters;
    int         collectS1apGenCounters;
    int         collectS1apPeerNodeCounters;
    int         collectRrcGenCounters;
    int         collectRrcCellCounters;
    int         collectRrmCounters;
    int         collectIPCounters;
    int         collectEthernetCounters;

    config_setting_lookup_int(table, "pmProfileId", (int*)&ptrPmCounterLogConfig->pmProfileId); 
    config_setting_lookup_int(table, "sampleInterval", (int*)&ptrPmCounterLogConfig->sampleInterval); 
    config_setting_lookup_int(table, "pmFileCreationInterval", (int*)&ptrPmCounterLogConfig->pmFileCreationInterval); 
    config_setting_lookup_int(table, "collectMacGenCounters", &collectMacGenCounters); 
    ptrPmCounterLogConfig->collectMacGenCounters = collectMacGenCounters;
    config_setting_lookup_int(table, "collectMacUplaneCounters", &collectMacUplaneCounters); 
    ptrPmCounterLogConfig->collectMacUplaneCounters = collectMacUplaneCounters;
    config_setting_lookup_int(table, "collectRlcGenCounters", &collectRlcGenCounters); 
    ptrPmCounterLogConfig->collectRlcGenCounters = collectRlcGenCounters;
    config_setting_lookup_int(table, "collectPdcpGenCounters", &collectPdcpGenCounters); 
    ptrPmCounterLogConfig->collectPdcpGenCounters = collectPdcpGenCounters;
    config_setting_lookup_int(table, "collectPdcpUplaneCounters", &collectPdcpUplaneCounters); 
    ptrPmCounterLogConfig->collectPdcpUplaneCounters = collectPdcpUplaneCounters;
    config_setting_lookup_int(table, "collectGtpGenCounters", &collectGtpGenCounters); 
    ptrPmCounterLogConfig->collectGtpGenCounters = collectGtpGenCounters;
    config_setting_lookup_int(table, "collectSctpGenCounters", &collectSctpGenCounters); 
    ptrPmCounterLogConfig->collectSctpGenCounters = collectSctpGenCounters;
    config_setting_lookup_int(table, "collectS1apGenCounters", &collectS1apGenCounters); 
    ptrPmCounterLogConfig->collectS1apGenCounters = collectS1apGenCounters;
    config_setting_lookup_int(table, "collectS1apPeerNodeCounters", &collectS1apPeerNodeCounters); 
    ptrPmCounterLogConfig->collectS1apPeerNodeCounters = collectS1apPeerNodeCounters;
    config_setting_lookup_int(table, "collectRrcGenCounters", &collectRrcGenCounters); 
    ptrPmCounterLogConfig->collectRrcGenCounters = collectRrcGenCounters;
    config_setting_lookup_int(table, "collectRrcCellCounters", &collectRrcCellCounters); 
    ptrPmCounterLogConfig->collectRrcCellCounters = collectRrcCellCounters;
    config_setting_lookup_int(table, "collectRrmCounters", &collectRrmCounters); 
    ptrPmCounterLogConfig->collectRrmCounters = collectRrmCounters;
    config_setting_lookup_int(table, "collectIPCounters", &collectIPCounters); 
    ptrPmCounterLogConfig->collectIPCounters = collectIPCounters;
    config_setting_lookup_int(table, "collectEthernetCounters", &collectEthernetCounters); 
    ptrPmCounterLogConfig->collectEthernetCounters = collectEthernetCounters;
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
TBOOL writeToFilePmCounterLogConfigTable(
  IN PmCounterLogConfig      *ptrPmCounterLogConfig,
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
  readTable = config_lookup(&readCfg, "pmCounterLogConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table pmCounterLogConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "pmCounterLogConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "pmProfileId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->pmProfileId);
  setting = config_setting_add(group, "sampleInterval", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->sampleInterval);
  setting = config_setting_add(group, "pmFileCreationInterval", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->pmFileCreationInterval);
  setting = config_setting_add(group, "collectMacGenCounters", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->collectMacGenCounters);
  setting = config_setting_add(group, "collectMacUplaneCounters", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->collectMacUplaneCounters);
  setting = config_setting_add(group, "collectRlcGenCounters", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->collectRlcGenCounters);
  setting = config_setting_add(group, "collectPdcpGenCounters", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->collectPdcpGenCounters);
  setting = config_setting_add(group, "collectPdcpUplaneCounters", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->collectPdcpUplaneCounters);
  setting = config_setting_add(group, "collectGtpGenCounters", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->collectGtpGenCounters);
  setting = config_setting_add(group, "collectSctpGenCounters", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->collectSctpGenCounters);
  setting = config_setting_add(group, "collectS1apGenCounters", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->collectS1apGenCounters);
  setting = config_setting_add(group, "collectS1apPeerNodeCounters", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->collectS1apPeerNodeCounters);
  setting = config_setting_add(group, "collectRrcGenCounters", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->collectRrcGenCounters);
  setting = config_setting_add(group, "collectRrcCellCounters", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->collectRrcCellCounters);
  setting = config_setting_add(group, "collectRrmCounters", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->collectRrmCounters);
  setting = config_setting_add(group, "collectIPCounters", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->collectIPCounters);
  setting = config_setting_add(group, "collectEthernetCounters", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrPmCounterLogConfig->collectEthernetCounters);

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
void readFromFileEutraHoProfileConfigTable(
  IN UINT8                   *fileToRead, 
  IO EutraHoProfileConfig    *ptrEutraHoProfileConfig,
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
  table = config_lookup(&cfg, "eutraHoProfileConfig");
  if(table NOTEQ NULL)
  {
    int         a3Offset;
    int         reportOnLeave;
    int         a5Threshold1;
    int         a5Threshold2;
    int         hysteresis;
    int         maxReportCells;
    int         sMeasure;

    config_setting_lookup_int(table, "hoProfileId", (int*)&ptrEutraHoProfileConfig->hoProfileId); 
    config_setting_lookup_int(table, "a3Offset", &a3Offset); 
    ptrEutraHoProfileConfig->a3Offset = a3Offset;
    config_setting_lookup_int(table, "reportOnLeave", &reportOnLeave); 
    ptrEutraHoProfileConfig->reportOnLeave = reportOnLeave;
    config_setting_lookup_int(table, "a5Threshold1", &a5Threshold1); 
    ptrEutraHoProfileConfig->a5Threshold1 = a5Threshold1;
    config_setting_lookup_int(table, "a5Threshold2", &a5Threshold2); 
    ptrEutraHoProfileConfig->a5Threshold2 = a5Threshold2;
    config_setting_lookup_int(table, "hysteresis", &hysteresis); 
    ptrEutraHoProfileConfig->hysteresis = hysteresis;
    config_setting_lookup_int(table, "timeToTrigger", (int*)&ptrEutraHoProfileConfig->timeToTrigger); 
    config_setting_lookup_int(table, "triggerQuantity", (int*)&ptrEutraHoProfileConfig->triggerQuantity); 
    config_setting_lookup_int(table, "reportQuantity", (int*)&ptrEutraHoProfileConfig->reportQuantity); 
    config_setting_lookup_int(table, "filterCoefficientRsrq", (int*)&ptrEutraHoProfileConfig->filterCoefficientRsrq); 
    config_setting_lookup_int(table, "filterCoefficientRsrp", (int*)&ptrEutraHoProfileConfig->filterCoefficientRsrp); 
    config_setting_lookup_int(table, "reportInterval", (int*)&ptrEutraHoProfileConfig->reportInterval); 
    config_setting_lookup_int(table, "reportAmount", (int*)&ptrEutraHoProfileConfig->reportAmount); 
    config_setting_lookup_int(table, "maxReportCells", &maxReportCells); 
    ptrEutraHoProfileConfig->maxReportCells = maxReportCells;
    config_setting_lookup_int(table, "supportedEvents", (int*)&ptrEutraHoProfileConfig->supportedEvents); 
    config_setting_lookup_int(table, "sMeasure", &sMeasure); 
    ptrEutraHoProfileConfig->sMeasure = sMeasure;
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
TBOOL writeToFileEutraHoProfileConfigTable(
  IN EutraHoProfileConfig    *ptrEutraHoProfileConfig,
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
  readTable = config_lookup(&readCfg, "eutraHoProfileConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table eutraHoProfileConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "eutraHoProfileConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "hoProfileId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEutraHoProfileConfig->hoProfileId);
  setting = config_setting_add(group, "a3Offset", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEutraHoProfileConfig->a3Offset);
  setting = config_setting_add(group, "reportOnLeave", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEutraHoProfileConfig->reportOnLeave);
  setting = config_setting_add(group, "a5Threshold1", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEutraHoProfileConfig->a5Threshold1);
  setting = config_setting_add(group, "a5Threshold2", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEutraHoProfileConfig->a5Threshold2);
  setting = config_setting_add(group, "hysteresis", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEutraHoProfileConfig->hysteresis);
  setting = config_setting_add(group, "timeToTrigger", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEutraHoProfileConfig->timeToTrigger);
  setting = config_setting_add(group, "triggerQuantity", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEutraHoProfileConfig->triggerQuantity);
  setting = config_setting_add(group, "reportQuantity", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEutraHoProfileConfig->reportQuantity);
  setting = config_setting_add(group, "filterCoefficientRsrq", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEutraHoProfileConfig->filterCoefficientRsrq);
  setting = config_setting_add(group, "filterCoefficientRsrp", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEutraHoProfileConfig->filterCoefficientRsrp);
  setting = config_setting_add(group, "reportInterval", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEutraHoProfileConfig->reportInterval);
  setting = config_setting_add(group, "reportAmount", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEutraHoProfileConfig->reportAmount);
  setting = config_setting_add(group, "maxReportCells", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEutraHoProfileConfig->maxReportCells);
  setting = config_setting_add(group, "supportedEvents", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEutraHoProfileConfig->supportedEvents);
  setting = config_setting_add(group, "sMeasure", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrEutraHoProfileConfig->sMeasure);

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
NumEntry readFromFileEutraNeighCellConfigTable(
  IN UINT8                   *fileToRead, 
  IO EutraNeighCellConfig    *ptrEutraNeighCellConfig,
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
  setting = config_lookup(&cfg, "eutraNeighCellConfig");
  if(setting NOTEQ NULL)
  {
    int count = config_setting_length(setting);
    int       numElem;
    for(numElem = 0;numElem < count; ++numElem)
    {
      int         isUsed;
      int         neighEutraCellId;
      int         neighEutraPhyCellId;
      const char *neighEutraPlmnMcc;
      const char *neighEutraPlmnMnc;
      int         neighEutraTac;

      config_setting_t *table = config_setting_get_elem(setting, numElem);
      /* only output the record if all of the expected fields are present */

      config_setting_lookup_int(table, "neibourIndex", (int*)&ptrEutraNeighCellConfig->neibourIndex); 
      config_setting_lookup_int(table, "isUsed", &isUsed); 
      ptrEutraNeighCellConfig->isUsed = isUsed;
      config_setting_lookup_int(table, "neighEutraEnbId", (int*)&ptrEutraNeighCellConfig->neighEutraEnbId); 
      config_setting_lookup_int(table, "neighEutraCellId", &neighEutraCellId); 
      ptrEutraNeighCellConfig->neighEutraCellId = neighEutraCellId;
      config_setting_lookup_int(table, "neighEutraPhyCellId", &neighEutraPhyCellId); 
      ptrEutraNeighCellConfig->neighEutraPhyCellId = neighEutraPhyCellId;
      config_setting_lookup_string(table, "neighEutraPlmnMcc", &neighEutraPlmnMcc); 
      strncpy(ptrEutraNeighCellConfig->neighEutraPlmnMcc,neighEutraPlmnMcc, MAX_SIZE_EUTRANEIGHCELLCONFIG_NEIGHEUTRAPLMNMCC );
      config_setting_lookup_string(table, "neighEutraPlmnMnc", &neighEutraPlmnMnc); 
      strncpy(ptrEutraNeighCellConfig->neighEutraPlmnMnc,neighEutraPlmnMnc, MAX_SIZE_EUTRANEIGHCELLCONFIG_NEIGHEUTRAPLMNMNC );
      config_setting_lookup_int(table, "neighEutraTac", &neighEutraTac); 
      ptrEutraNeighCellConfig->neighEutraTac = neighEutraTac;
      config_setting_lookup_int(table, "neighCellMeasBandwidth", (int*)&ptrEutraNeighCellConfig->neighCellMeasBandwidth); 
      config_setting_lookup_int(table, "cellIndividualOffset", (int*)&ptrEutraNeighCellConfig->cellIndividualOffset); 
      config_setting_lookup_int(table, "intraFreqNeighCellQOffsetCell", (int*)&ptrEutraNeighCellConfig->intraFreqNeighCellQOffsetCell); 
      ptrEutraNeighCellConfig++;
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
TBOOL writeToFileEutraNeighCellConfigTable(
  IN EutraNeighCellConfig    *ptrEutraNeighCellConfig,
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
  readTable = config_lookup(&readCfg, "eutraNeighCellConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table eutraNeighCellConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "eutraNeighCellConfig", CONFIG_TYPE_LIST);

  int count = config_setting_length(readTable);
  int numElem = 0;
  for(numElem = 0; numElem < count; ++numElem)
  {
    /* Create the new entry */
    cfgEntry = config_setting_add(group, NULL, CONFIG_TYPE_GROUP);

    setting = config_setting_add(cfgEntry, "neibourIndex", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrEutraNeighCellConfig->neibourIndex);
    setting = config_setting_add(cfgEntry, "isUsed", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrEutraNeighCellConfig->isUsed);
    setting = config_setting_add(cfgEntry, "neighEutraEnbId", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrEutraNeighCellConfig->neighEutraEnbId);
    setting = config_setting_add(cfgEntry, "neighEutraCellId", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrEutraNeighCellConfig->neighEutraCellId);
    setting = config_setting_add(cfgEntry, "neighEutraPhyCellId", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrEutraNeighCellConfig->neighEutraPhyCellId);
    setting = config_setting_add(cfgEntry, "neighEutraPlmnMcc", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEutraNeighCellConfig->neighEutraPlmnMcc);
    setting = config_setting_add(cfgEntry, "neighEutraPlmnMnc", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrEutraNeighCellConfig->neighEutraPlmnMnc);
    setting = config_setting_add(cfgEntry, "neighEutraTac", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrEutraNeighCellConfig->neighEutraTac);
    setting = config_setting_add(cfgEntry, "neighCellMeasBandwidth", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrEutraNeighCellConfig->neighCellMeasBandwidth);
    setting = config_setting_add(cfgEntry, "cellIndividualOffset", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrEutraNeighCellConfig->cellIndividualOffset);
    setting = config_setting_add(cfgEntry, "intraFreqNeighCellQOffsetCell", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrEutraNeighCellConfig->intraFreqNeighCellQOffsetCell);
    ptrEutraNeighCellConfig++;
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
void readFromFileSysInfo3ConfigTable(
  IN UINT8                   *fileToRead, 
  IO SysInfo3Config          *ptrSysInfo3Config,
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
  table = config_lookup(&cfg, "sysInfo3Config");
  if(table NOTEQ NULL)
  {
    int         threshServingLow;
    int         cellReselectionPriority;
    int         qRxLevMin;
    int         pMax;
    int         sIntraSearch;
    int         neighCellConfig;
    int         tReselectionEutra;
    int         presenceAntennaPort1;

    config_setting_lookup_int(table, "sysInfo3ProfileId", (int*)&ptrSysInfo3Config->sysInfo3ProfileId); 
    config_setting_lookup_int(table, "qHyst", (int*)&ptrSysInfo3Config->qHyst); 
    config_setting_lookup_int(table, "threshServingLow", &threshServingLow); 
    ptrSysInfo3Config->threshServingLow = threshServingLow;
    config_setting_lookup_int(table, "cellReselectionPriority", &cellReselectionPriority); 
    ptrSysInfo3Config->cellReselectionPriority = cellReselectionPriority;
    config_setting_lookup_int(table, "qRxLevMin", &qRxLevMin); 
    ptrSysInfo3Config->qRxLevMin = qRxLevMin;
    config_setting_lookup_int(table, "pMax", &pMax); 
    ptrSysInfo3Config->pMax = pMax;
    config_setting_lookup_int(table, "sIntraSearch", &sIntraSearch); 
    ptrSysInfo3Config->sIntraSearch = sIntraSearch;
    config_setting_lookup_int(table, "allowMeasBw", (int*)&ptrSysInfo3Config->allowMeasBw); 
    config_setting_lookup_int(table, "neighCellConfig", &neighCellConfig); 
    ptrSysInfo3Config->neighCellConfig = neighCellConfig;
    config_setting_lookup_int(table, "tReselectionEutra", &tReselectionEutra); 
    ptrSysInfo3Config->tReselectionEutra = tReselectionEutra;
    config_setting_lookup_int(table, "presenceAntennaPort1", &presenceAntennaPort1); 
    ptrSysInfo3Config->presenceAntennaPort1 = presenceAntennaPort1;
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
TBOOL writeToFileSysInfo3ConfigTable(
  IN SysInfo3Config          *ptrSysInfo3Config,
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
  readTable = config_lookup(&readCfg, "sysInfo3Config");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table sysInfo3Config doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "sysInfo3Config", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "sysInfo3ProfileId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo3Config->sysInfo3ProfileId);
  setting = config_setting_add(group, "qHyst", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo3Config->qHyst);
  setting = config_setting_add(group, "threshServingLow", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo3Config->threshServingLow);
  setting = config_setting_add(group, "cellReselectionPriority", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo3Config->cellReselectionPriority);
  setting = config_setting_add(group, "qRxLevMin", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo3Config->qRxLevMin);
  setting = config_setting_add(group, "pMax", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo3Config->pMax);
  setting = config_setting_add(group, "sIntraSearch", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo3Config->sIntraSearch);
  setting = config_setting_add(group, "allowMeasBw", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo3Config->allowMeasBw);
  setting = config_setting_add(group, "neighCellConfig", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo3Config->neighCellConfig);
  setting = config_setting_add(group, "tReselectionEutra", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo3Config->tReselectionEutra);
  setting = config_setting_add(group, "presenceAntennaPort1", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSysInfo3Config->presenceAntennaPort1);

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
void readFromFileSonConfigTable(
  IN UINT8                   *fileToRead, 
  IO SonConfig               *ptrSonConfig,
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
  table = config_lookup(&cfg, "sonConfig");
  if(table NOTEQ NULL)
  {
    int         intraFreqHoThreshold;
    int         icicThreshold;
    int         ueLoadChangeThreshold;

    config_setting_lookup_int(table, "sonProfileId", (int*)&ptrSonConfig->sonProfileId); 
    config_setting_lookup_int(table, "intraFreqHoThreshold", &intraFreqHoThreshold); 
    ptrSonConfig->intraFreqHoThreshold = intraFreqHoThreshold;
    config_setting_lookup_int(table, "icicThreshold", &icicThreshold); 
    ptrSonConfig->icicThreshold = icicThreshold;
    config_setting_lookup_int(table, "ueLoadChangeThreshold", &ueLoadChangeThreshold); 
    ptrSonConfig->ueLoadChangeThreshold = ueLoadChangeThreshold;
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
TBOOL writeToFileSonConfigTable(
  IN SonConfig               *ptrSonConfig,
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
  readTable = config_lookup(&readCfg, "sonConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table sonConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "sonConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "sonProfileId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSonConfig->sonProfileId);
  setting = config_setting_add(group, "intraFreqHoThreshold", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSonConfig->intraFreqHoThreshold);
  setting = config_setting_add(group, "icicThreshold", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSonConfig->icicThreshold);
  setting = config_setting_add(group, "ueLoadChangeThreshold", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSonConfig->ueLoadChangeThreshold);

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
void readFromFileSrsConfigDedicatedTable(
  IN UINT8                   *fileToRead, 
  IO SrsConfigDedicated      *ptrSrsConfigDedicated,
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
  table = config_lookup(&cfg, "srsConfigDedicated");
  if(table NOTEQ NULL)
  {
    int         frequecyDomainPosition;
    int         duration;

    config_setting_lookup_int(table, "srsBandwidth", (int*)&ptrSrsConfigDedicated->srsBandwidth); 
    config_setting_lookup_int(table, "srsHoppingBandWidth", (int*)&ptrSrsConfigDedicated->srsHoppingBandWidth); 
    config_setting_lookup_int(table, "frequecyDomainPosition", &frequecyDomainPosition); 
    ptrSrsConfigDedicated->frequecyDomainPosition = frequecyDomainPosition;
    config_setting_lookup_int(table, "duration", &duration); 
    ptrSrsConfigDedicated->duration = duration;
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
TBOOL writeToFileSrsConfigDedicatedTable(
  IN SrsConfigDedicated      *ptrSrsConfigDedicated,
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
  readTable = config_lookup(&readCfg, "srsConfigDedicated");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table srsConfigDedicated doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "srsConfigDedicated", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "srsBandwidth", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSrsConfigDedicated->srsBandwidth);
  setting = config_setting_add(group, "srsHoppingBandWidth", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSrsConfigDedicated->srsHoppingBandWidth);
  setting = config_setting_add(group, "frequecyDomainPosition", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSrsConfigDedicated->frequecyDomainPosition);
  setting = config_setting_add(group, "duration", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSrsConfigDedicated->duration);

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
void readFromFileSecurityConfigTable(
  IN UINT8                   *fileToRead, 
  IO SecurityConfig          *ptrSecurityConfig,
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
  table = config_lookup(&cfg, "securityConfig");
  if(table NOTEQ NULL)
  {

    config_setting_lookup_int(table, "index", (int*)&ptrSecurityConfig->index); 
    config_setting_lookup_int(table, "uuCiphering", (int*)&ptrSecurityConfig->uuCiphering); 
    config_setting_lookup_int(table, "uuIntegrityProtection", (int*)&ptrSecurityConfig->uuIntegrityProtection); 
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
TBOOL writeToFileSecurityConfigTable(
  IN SecurityConfig          *ptrSecurityConfig,
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
  readTable = config_lookup(&readCfg, "securityConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table securityConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "securityConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "index", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSecurityConfig->index);
  setting = config_setting_add(group, "uuCiphering", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSecurityConfig->uuCiphering);
  setting = config_setting_add(group, "uuIntegrityProtection", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrSecurityConfig->uuIntegrityProtection);

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
void readFromFileUeEventHistoryLogConfigTable(
  IN UINT8                   *fileToRead, 
  IO UeEventHistoryLogConfig *ptrUeEventHistoryLogConfig,
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
  table = config_lookup(&cfg, "ueEventHistoryLogConfig");
  if(table NOTEQ NULL)
  {
    int         cRnti;
    int         ueProcedureBitMap;

    config_setting_lookup_int(table, "ueEventHistoryIndex", (int*)&ptrUeEventHistoryLogConfig->ueEventHistoryIndex); 
    config_setting_lookup_int(table, "cRnti", &cRnti); 
    ptrUeEventHistoryLogConfig->cRnti = cRnti;
    config_setting_lookup_int(table, "ueProcedureBitMap", &ueProcedureBitMap); 
    ptrUeEventHistoryLogConfig->ueProcedureBitMap = ueProcedureBitMap;
    config_setting_lookup_int(table, "ueEventMeasures", (int*)&ptrUeEventHistoryLogConfig->ueEventMeasures); 
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
TBOOL writeToFileUeEventHistoryLogConfigTable(
  IN UeEventHistoryLogConfig *ptrUeEventHistoryLogConfig,
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
  readTable = config_lookup(&readCfg, "ueEventHistoryLogConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table ueEventHistoryLogConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "ueEventHistoryLogConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "ueEventHistoryIndex", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUeEventHistoryLogConfig->ueEventHistoryIndex);
  setting = config_setting_add(group, "cRnti", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUeEventHistoryLogConfig->cRnti);
  setting = config_setting_add(group, "ueProcedureBitMap", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUeEventHistoryLogConfig->ueProcedureBitMap);
  setting = config_setting_add(group, "ueEventMeasures", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrUeEventHistoryLogConfig->ueEventMeasures);

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
NumEntry readFromFileCounterConfigTable(
  IN UINT8                   *fileToRead, 
  IO CounterConfig           *ptrCounterConfig,
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
  setting = config_lookup(&cfg, "counterConfig");
  if(setting NOTEQ NULL)
  {
    int count = config_setting_length(setting);
    int       numElem;
    for(numElem = 0;numElem < count; ++numElem)
    {
      int         isEnabled;

      config_setting_t *table = config_setting_get_elem(setting, numElem);
      /* only output the record if all of the expected fields are present */

      config_setting_lookup_int(table, "index", (int*)&ptrCounterConfig->index); 
      config_setting_lookup_int(table, "counterId", (int*)&ptrCounterConfig->counterId); 
      config_setting_lookup_int(table, "profileId", (int*)&ptrCounterConfig->profileId); 
      config_setting_lookup_int(table, "isEnabled", &isEnabled); 
      ptrCounterConfig->isEnabled = isEnabled;
      ptrCounterConfig++;
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
TBOOL writeToFileCounterConfigTable(
  IN CounterConfig           *ptrCounterConfig,
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
  readTable = config_lookup(&readCfg, "counterConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table counterConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "counterConfig", CONFIG_TYPE_LIST);

  int count = config_setting_length(readTable);
  int numElem = 0;
  for(numElem = 0; numElem < count; ++numElem)
  {
    /* Create the new entry */
    cfgEntry = config_setting_add(group, NULL, CONFIG_TYPE_GROUP);

    setting = config_setting_add(cfgEntry, "index", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrCounterConfig->index);
    setting = config_setting_add(cfgEntry, "counterId", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrCounterConfig->counterId);
    setting = config_setting_add(cfgEntry, "profileId", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrCounterConfig->profileId);
    setting = config_setting_add(cfgEntry, "isEnabled", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrCounterConfig->isEnabled);
    ptrCounterConfig++;
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
void readFromFileTraceConfigTable(
  IN UINT8                   *fileToRead, 
  IO TraceConfig             *ptrTraceConfig,
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
  table = config_lookup(&cfg, "traceConfig");
  if(table NOTEQ NULL)
  {
    int         traceRRC;
    int         traceS1AP;
    int         traceX2AP;
    int         traceReserved1;
    int         traceReserved2;
    int         traceReserved3;
    int         traceDestUdpPort;
    const char *traceDestIPAddress;

    config_setting_lookup_int(table, "traceProfileId", (int*)&ptrTraceConfig->traceProfileId); 
    config_setting_lookup_int(table, "traceRRC", &traceRRC); 
    ptrTraceConfig->traceRRC = traceRRC;
    config_setting_lookup_int(table, "traceS1AP", &traceS1AP); 
    ptrTraceConfig->traceS1AP = traceS1AP;
    config_setting_lookup_int(table, "traceX2AP", &traceX2AP); 
    ptrTraceConfig->traceX2AP = traceX2AP;
    config_setting_lookup_int(table, "traceReserved1", &traceReserved1); 
    ptrTraceConfig->traceReserved1 = traceReserved1;
    config_setting_lookup_int(table, "traceReserved2", &traceReserved2); 
    ptrTraceConfig->traceReserved2 = traceReserved2;
    config_setting_lookup_int(table, "traceReserved3", &traceReserved3); 
    ptrTraceConfig->traceReserved3 = traceReserved3;
    config_setting_lookup_int(table, "traceDestination", (int*)&ptrTraceConfig->traceDestination); 
    config_setting_lookup_int(table, "traceDestUdpPort", &traceDestUdpPort); 
    ptrTraceConfig->traceDestUdpPort = traceDestUdpPort;
    config_setting_lookup_string(table, "traceDestIPAddress", &traceDestIPAddress); 
    strncpy(ptrTraceConfig->traceDestIPAddress,traceDestIPAddress, MAX_SIZE_TRACECONFIG_TRACEDESTIPADDRESS );
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
TBOOL writeToFileTraceConfigTable(
  IN TraceConfig             *ptrTraceConfig,
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
  readTable = config_lookup(&readCfg, "traceConfig");
  if(readTable EQ NULL)
  {
    *error = TRUE;
    config_destroy(&writeCfg);
    config_destroy(&readCfg);
    printf("Table traceConfig doesn't exist");
    return FALSE;
  }

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "traceConfig", CONFIG_TYPE_GROUP);


  setting = config_setting_add(group, "traceProfileId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrTraceConfig->traceProfileId);
  setting = config_setting_add(group, "traceRRC", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrTraceConfig->traceRRC);
  setting = config_setting_add(group, "traceS1AP", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrTraceConfig->traceS1AP);
  setting = config_setting_add(group, "traceX2AP", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrTraceConfig->traceX2AP);
  setting = config_setting_add(group, "traceReserved1", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrTraceConfig->traceReserved1);
  setting = config_setting_add(group, "traceReserved2", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrTraceConfig->traceReserved2);
  setting = config_setting_add(group, "traceReserved3", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrTraceConfig->traceReserved3);
  setting = config_setting_add(group, "traceDestination", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrTraceConfig->traceDestination);
  setting = config_setting_add(group, "traceDestUdpPort", CONFIG_TYPE_INT);
  config_setting_set_int(setting, ptrTraceConfig->traceDestUdpPort);
  setting = config_setting_add(group, "traceDestIPAddress", CONFIG_TYPE_STRING);
  config_setting_set_string(setting,  (char*)&ptrTraceConfig->traceDestIPAddress);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);

  config_destroy(&writeCfg);
  config_destroy(&readCfg);
  return TRUE;
}
