/*
 * imc_cfg_read.c
 *
 *  Created on: Aug 17, 2011
 *      Author: hgayam
 */

#include "imc_cfg.h"


/**
 * @param   picoBsHomePath   Configuration file path given by user
 * @details This function populates the framework context.
 *           Module identifier and the destination port address of a module
 *           are taken from configuration file.
 */
void readImcCfgFile(
  IN UINT8       *fileToRead,
  IO ImcContext  *imcContext,
  IO UINT8       *error
  )
{
  config_t          cfg;
  config_setting_t *setting;
  UINT32            modIdIndex = 0;

  config_init(&cfg); /* init LibConfig */

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&cfg, fileToRead))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
    config_destroy(&cfg);
    printf("\n Please enter valid Path for Config file \n");
    return;
  }

  setting = config_lookup(&cfg, "module_cfg_table");
  if(setting NOTEQ NULL)
  {
    int count = config_setting_length(setting);
    int i;

    for(i = 0; i < count; ++i)
    {
      config_setting_t *table = config_setting_get_elem(setting, i);

      /* Only output the record if all of the expected fields are present. */
      const char  *moduleName, *ipAddress;
      int          udpPort;
      int          imcMethod;
      const char  *unixSockFilePath;

      config_setting_lookup_string(table, "unixSockFilePath", &unixSockFilePath);
      config_setting_lookup_int(table, "imcMethod", &imcMethod);
      config_setting_lookup_string(table, "moduleName", &moduleName );
      config_setting_lookup_string(table, "ipAddress", &ipAddress);
      config_setting_lookup_int(table, "udpPort", &udpPort);

      for(modIdIndex = 0 ; modIdIndex < MODID_MAX ; modIdIndex++)
      {
        if(strcmp(getModuleName(modIdIndex), moduleName) EQ 0)
        {
          imcContext->modAddrTab[modIdIndex].modId     = modIdIndex;
          imcContext->modAddrTab[modIdIndex].udpPort   = udpPort;
          strncpy(&(imcContext->modAddrTab[modIdIndex].ipv4[0]), ipAddress, 16);
          imcContext->modAddrTab[modIdIndex].imcMethod = imcMethod;
          strncpy(&(imcContext->modAddrTab[modIdIndex].unixSockFilePath[0]), unixSockFilePath, UNIX_SOCK_PATH_MAX);
          break;
        }
        if(modIdIndex EQ (MODID_MAX -1))
        {
          logFatalErrorAndExit(EXIT_CONFIG_READ_FAIL, "Unrecognized moduleName= %s in %s",moduleName, fileToRead);
        }
      }
    }
  }
  else
  {
    fprintf(stderr, "Table Not present : %s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
  }
  config_destroy(&cfg);
}
