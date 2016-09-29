/**
 * @file    deh_init.c
 * @brief This file will handle Initlisation of DEH
 *          in any DEH state.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author  mkata, spolepally, aagrawal, kgummad, rchinta, tmaji
 * @date
 * @details 1) DEH OP-START, OP-STOP, SW Start, Health Check,@n
 * 			   RH Connects to BS,Sw Exit Procedures added.
 *          2) Print functions added for Radio Head Messages
 *          3) LK messages updated to latest C & M specification
 *          4) LED color is supported in DEH.
 */

#include "deh.h"

/**
 * @details This function will fill the RH Configuration with default values.
 */

void fillRhDefaultCfgWithDefaults()
{
  RhDefaultCfg    *rhDefaultCfg = getRhDefaultCfg();
  rhDefaultCfg->radioHeadProfileId = 1;
  rhDefaultCfg->rxDelay = 0;
  rhDefaultCfg->txDelay = 0;
  rhDefaultCfg->c1IQMap = 0;
  rhDefaultCfg->rxEarfcn  = 0;
  rhDefaultCfg->txAttenPathA = 0;
  rhDefaultCfg->txAttenPathB = 0;
  rhDefaultCfg->txEarfcn  = 0;
  rhDefaultCfg->rxEarfcn  = 0;
  rhDefaultCfg->lowpowerAlarmThreshold = 0;
  rhDefaultCfg->overpowerAlarmThreshold = 0;
  rhDefaultCfg->vswrShutdownAlarmThreshold = 0;

  /* Updating the values in Table Context */
  dehUpdateRhConfigTable();
}

/**
 * @details This function will fill the RH Alarms Status with default values.
 */

void fillDefaultRhAlarmsStatus()
{
  LkAlarmTable *rhDefaultAlarms = getRhAlarmsInfo();

  rhDefaultAlarms->clockFail       = FALSE;
  rhDefaultAlarms->highTemperature = FALSE;
  rhDefaultAlarms->lowTemperature  = FALSE;
  rhDefaultAlarms->overPowerSideA  = FALSE;
  rhDefaultAlarms->vswrFailsideA   = FALSE;
  rhDefaultAlarms->lowGainsideA    = FALSE;
  rhDefaultAlarms->lnaFailSideA    = FALSE;
  rhDefaultAlarms->disabledSideA   = FALSE;
  rhDefaultAlarms->shutdownSideA   = FALSE;
  rhDefaultAlarms->overPowerSideB  = FALSE;
  rhDefaultAlarms->vswrFailSideB   = FALSE;
  rhDefaultAlarms->lowGainSideB    = FALSE;
  rhDefaultAlarms->lnaFailSideB    = FALSE;
  rhDefaultAlarms->disabledSideB   = FALSE;
  rhDefaultAlarms->shutdownSideB   = FALSE;
}

void fillDefaultDeviceAlarmStatus()
{
  DeviceAlarmsInfo  *devDefaultAlarms = getDeviceAlarmsInfo();

  devDefaultAlarms->baseBand      = FALSE;
  devDefaultAlarms->baseBandClock = FALSE;
  devDefaultAlarms->cpri          = FALSE;
  devDefaultAlarms->ethBackhaul   = FALSE;
  devDefaultAlarms->ethDebug      = FALSE;
  devDefaultAlarms->radioHead     = FALSE;
  devDefaultAlarms->tempSensor    = FALSE;
  devDefaultAlarms->gps           = FALSE;
}
/**
 * @param configFilePath path of the configuratio file.
 * @details This function will read RH IP address from configuration file.
 */

static void readRhIpFromFactorySettingTable(
  IO UINT8       *configFilePath
  )
{
  TBOOL          isError;
  EnbAddresses   settings;
  UINT8       fullFilePath[MAX_BUFFER_SIZE];

  clearBytes( &fullFilePath, sizeof(fullFilePath));
  clearBytes( &settings, sizeof(EnbAddresses));

  constructFullPath ( configFilePath, "enb_pre_config.cfg", fullFilePath );

  readFromFileEnbAddressesTable( &fullFilePath[0], &settings, &isError );
  if ( isError EQ TRUE )
  {
    BS_ERROR_CONFIG_READ_FAILED()
    exit(EXIT_FAILURE);
  }
  /* Get IP address and store it in DEH context */
  DehContext   *ctx = getDehContext();

  copyBytes( settings.radioHeadIpAddress, MAX_SIZE_ENBADDRESSES_RADIOHEADIPADDRESS, ctx->rhIpAddr );

}

/**
 * @param configDir  This is the configuration path, either will be NULL / or some valid path
 * @details This function will read the  Devices Motniored from configuration file.
 */
void readDehDevicesMonitoredFromCfg(
  IN UINT8       *configFilePath
  )
{
  UINT8       isError = FALSE;
  UINT8       fullFilePath[MAX_BUFFER_SIZE];

  GenericTable  *tableFunc       = getTableFunctions( DEVICEMONITORCONFIG);
  DeviceMonitorConfig  *rowOfDeviceMonitorTable = (DeviceMonitorConfig*)getFirstRowOfTable( DEVICEMONITORCONFIG);
  clearBytes( &fullFilePath, sizeof(fullFilePath));
  constructFullPath ( configFilePath, "enb_pre_config.cfg", fullFilePath );

  if ( NULL EQ tableFunc->getReadFromFileFunc )
  {
    BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("DEVICEMONITORCONFIG");
    exit(EXIT_FAILURE);
  }
  else
  {
    tableFunc->getReadFromFileFunc( fullFilePath, rowOfDeviceMonitorTable, &isError );

    if ( isError EQ TRUE )
    {
      BS_ERROR_CONFIG_READ_FAILED()
      exit(EXIT_FAILURE);
    }
    /* Copy DEH Devices monitored in DEH context */
    rowOfDeviceMonitorTable = (DeviceMonitorConfig*)getFirstRowOfTable( DEVICEMONITORCONFIG);
    dehStoreDeviceMonitoredInCtx( rowOfDeviceMonitorTable );
  }
}


/**
 * @param configFilePath path of the configuration file.
 * @details This function initialises DEH with default configuration.
 */


void dehInit(
  IO UINT8       *configFilePath
  )
{

  /* for Global Variables used for all OAM Tables */
  fillGenericFunctionTable();
  emptyAllTables();

  /* Fill Radio Head Default Config with Default Values at INIT */
  fillRhDefaultCfgWithDefaults();

  /*Fill Alarms status default to FALSE*/
  fillDefaultRhAlarmsStatus();

  /*Fill Alarms status default to FALSE*/
   fillDefaultDeviceAlarmStatus();

  /* This will read all the timers related to DEH module and store them in the DEH context */
  readDehTimersFromCfg( configFilePath );

  /* Read Factory settings Config file for Radio Head IP address */
  readRhIpFromFactorySettingTable( configFilePath );

  /*Read default values of Devices motiored */
  readDehDevicesMonitoredFromCfg( configFilePath );

  /* Initialise Global GPS Context */
  initDehGpsCtx();
  
  /* Initialise POST Context with POST Results */
  initDehPostCtx();

  /* Initialise status context */
  initDehStatusCtx();

  /* Fill Baseband Hardware information calling platform API */
  getBaseBandHardwareInfo();

  /* Initialize Temperature Sensor */
  dehInitTempSensor();

  /* Initialise Component status table */
  initComponentStatusTable();

  SINT32 *sockFd = getSocketFd();
  /* Open UDP Socket */
  openUdpSocket( BS_MAILBOX_ID, TRUE, sockFd);
}

/**
 * @param configFilePath path of the configuration file.
 * @details This function initialises Component status table.
 */

void initComponentStatusTable()
{
  ComponentStatus   *rowOfTable = (ComponentStatus*)getFirstRowOfTable(COMPONENTSTATUS);

  rowOfTable->index 			         = DEV_BASEBAND;
  rowOfTable->componentType              = COMPONENTTYPE_BASEBAND;
  rowOfTable->componentInstance          = 1;
  snprintf(rowOfTable->componentVersion, MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1, "%s", dehGetDeviceVersion(DEV_BASEBAND));
  snprintf(rowOfTable->infoString,  MAX_SIZE_COMPONENTSTATUS_INFOSTRING + 1, "%s", dehGetDeviceInfo(DEV_BASEBAND));
  snprintf(rowOfTable->errorString, MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1, "%s", "");
  rowOfTable->componentOperationalState  = COMPONENTOPERATIONALSTATE_DOWN;
  getNextRowOfTable(rowOfTable);

  rowOfTable->index 			         = DEV_RADIOHEAD;
  rowOfTable->componentType              = COMPONENTTYPE_RADIOHEAD;
  rowOfTable->componentInstance          = 1;
  snprintf(rowOfTable->componentVersion, MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1, dehGetDeviceVersion(DEV_RADIOHEAD));
  snprintf(rowOfTable->infoString,  MAX_SIZE_COMPONENTSTATUS_INFOSTRING + 1, "%s", dehGetDeviceInfo(DEV_RADIOHEAD));
  snprintf(rowOfTable->errorString, MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1, "%s", "");
  rowOfTable->componentOperationalState  = COMPONENTOPERATIONALSTATE_DOWN;
  getNextRowOfTable(rowOfTable);

  rowOfTable->index 			 = DEV_BACKHAULPORT;
  rowOfTable->componentType              = COMPONENTTYPE_ETHERNET;
  rowOfTable->componentInstance          = ETHERNET_BACKHAUL;
  snprintf(rowOfTable->componentVersion, MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1, dehGetDeviceVersion(DEV_BACKHAULPORT));
  snprintf(rowOfTable->infoString,  MAX_SIZE_COMPONENTSTATUS_INFOSTRING + 1, "%s", dehGetDeviceInfo(DEV_BACKHAULPORT));
  snprintf(rowOfTable->errorString, MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1, "%s", "");
  rowOfTable->componentOperationalState  = COMPONENTOPERATIONALSTATE_DOWN;
  getNextRowOfTable(rowOfTable);

  rowOfTable->index 			 = DEV_DEBUGPORT;
  rowOfTable->componentType              = COMPONENTTYPE_ETHERNET;
  rowOfTable->componentInstance          = ETHERNET_DEBUG;
  snprintf(rowOfTable->componentVersion, MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1, dehGetDeviceVersion(DEV_DEBUGPORT));
  snprintf(rowOfTable->infoString,  MAX_SIZE_COMPONENTSTATUS_INFOSTRING + 1, "%s", dehGetDeviceInfo(DEV_DEBUGPORT));
  snprintf(rowOfTable->errorString, MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1, "%s", "");
  rowOfTable->componentOperationalState  = COMPONENTOPERATIONALSTATE_DOWN;
  getNextRowOfTable(rowOfTable);

  rowOfTable->index 			         = DEV_TEMPSENSOR;
  rowOfTable->componentType              = COMPONENTTYPE_TEMPSENSOR;
  rowOfTable->componentInstance          = 1;
  snprintf(rowOfTable->componentVersion, MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1, dehGetDeviceVersion(DEV_TEMPSENSOR));
  snprintf(rowOfTable->infoString,  MAX_SIZE_COMPONENTSTATUS_INFOSTRING + 1, "%s%s", dehGetDeviceInfo(DEV_TEMPSENSOR),"[ Temp = UNKNOWN ]");
  snprintf(rowOfTable->errorString, MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1, "%s", "");
  rowOfTable->componentOperationalState  = COMPONENTOPERATIONALSTATE_DOWN;
  getNextRowOfTable(rowOfTable);

  rowOfTable->index 			         = DEV_CPRI;
  rowOfTable->componentType              = COMPONENTTYPE_CPRI;
  rowOfTable->componentInstance          = 1;
  snprintf(rowOfTable->componentVersion, MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1, dehGetDeviceVersion(DEV_CPRI));
  snprintf(rowOfTable->infoString,  MAX_SIZE_COMPONENTSTATUS_INFOSTRING + 1, "%s", dehGetDeviceInfo(DEV_CPRI));
  snprintf(rowOfTable->errorString, MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1, "%s", "");
  rowOfTable->componentOperationalState  = COMPONENTOPERATIONALSTATE_DOWN;
  getNextRowOfTable(rowOfTable);

  rowOfTable->index 			         = DEV_GPS;
  rowOfTable->componentType              = COMPONENTTYPE_GPS;
  rowOfTable->componentInstance          = 1;
  snprintf(rowOfTable->componentVersion, MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1, dehGetDeviceVersion(DEV_GPS));
  snprintf(rowOfTable->infoString,  MAX_SIZE_COMPONENTSTATUS_INFOSTRING + 1, "%s", dehGetDeviceInfo(DEV_GPS));
  snprintf(rowOfTable->errorString, MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1, "%s", "");
  rowOfTable->componentOperationalState  = COMPONENTOPERATIONALSTATE_DOWN;
  getNextRowOfTable(rowOfTable);

  rowOfTable->index                      = DEV_CLOCK;
  rowOfTable->componentType              = COMPONENTTYPE_CLOCK;
  rowOfTable->componentInstance          = 1;
  snprintf(rowOfTable->componentVersion, MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1, dehGetDeviceVersion(DEV_CLOCK));
  snprintf(rowOfTable->infoString,  MAX_SIZE_COMPONENTSTATUS_INFOSTRING + 1, "%s", dehGetDeviceInfo(DEV_CLOCK));
  snprintf(rowOfTable->errorString, MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1, "%s", "");
  rowOfTable->componentOperationalState  = COMPONENTOPERATIONALSTATE_DOWN;
  getNextRowOfTable(rowOfTable);
  
}

/**
 * @param configDir  This is the configuration path, either will be NULL / or some valid path
 * @details This function will store the default values of devices Monitored in DEH context.
 */
void dehStoreDeviceMonitoredInCtx(
  IN DeviceMonitorConfig *config
  )
{
  DehContext *dehContext = getDehContext();
  
  dehContext->deviceFunctionalStatus.deviceMonitoredState[DEV_BASEBAND]     = config->baseBand;
  dehContext->deviceFunctionalStatus.deviceMonitoredState[DEV_RADIOHEAD]    = config->radioHead;
  dehContext->deviceFunctionalStatus.deviceMonitoredState[DEV_BACKHAULPORT] = config->backHaulPort;
  dehContext->deviceFunctionalStatus.deviceMonitoredState[DEV_DEBUGPORT]    = config->debugPort;
  dehContext->deviceFunctionalStatus.deviceMonitoredState[DEV_TEMPSENSOR]   = config->tempSensor;
  dehContext->deviceFunctionalStatus.deviceMonitoredState[DEV_GPS]          = config->gps;
  dehContext->deviceFunctionalStatus.deviceMonitoredState[DEV_CPRI]         = config->cpri;
  dehContext->deviceFunctionalStatus.deviceMonitoredState[DEV_CLOCK]        = config->clock;

}


/********************************************************************/
/**************                STATUS  Functions            *********/
/********************************************************************/
/**
 * @details  This functions is used to initiate global variables used in DEH
 */
void initDehStatusCtx()
{
  DeviceStatusContext *deviceStatusContext = getDeviceStatusContext();

  /* Initialising Device Status Info */
  clearBytes(&deviceStatusContext->lastSavedDevStatus, sizeof(DevStatus));
  clearBytes(&deviceStatusContext->presentDevStatus, sizeof(DevStatus));
  /* Initialising Present and Last saved pointers  */
  deviceStatusContext->presentStatus = &deviceStatusContext->presentDevStatus;
  deviceStatusContext->lastSaved = &deviceStatusContext->lastSavedDevStatus;
}

/**
 * @details  This functions is used to get Baseband hardware information and store in global context
 */
void getBaseBandHardwareInfo()
{
  DehContext *dehContext = getDehContext();
  dehUpdateBasebandHardwareVersion( &(dehContext->baseBandversionInfo) );
}

/**
 * @param   dehContext DEH Context
 * @details This function get IP of machine and update that.
 **/
void  getAndUpdateEnodebIpAddr(
  IO DehContext   *dehContext
  )
{
  IpAddrStr   ipAddr;
  IpAddrStr   netMask;

  clearBytes(ipAddr, sizeof(IpAddrStr));
  clearBytes(netMask, sizeof(IpAddrStr));

  dehContext->isBackHaulIpRecieved = getIpAddressOfEth(BACKHAUL_PORT, ipAddr,netMask);
  /* Reading BackHaul Port Address from enb_user_config.cfg */
  if((strncmp(ipAddr, "169", 3 )) EQ 0)
  {
    logError("DHCP failed to configure eth=eth2", NULL);
  }
  else
  {
	logInfo("DHCP IP Address=%s assigned to Enodeb backhaul Eth=%s", ipAddr, BACKHAUL_PORT);
    strncpy(dehContext->backhaulIpAddr,ipAddr,sizeof(IpAddrStr));
    strncpy(dehContext->backhaulNetMaskAddr,netMask,sizeof(IpAddrStr));
  }

}
