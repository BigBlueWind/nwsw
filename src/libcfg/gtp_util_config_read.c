/**
 * @file    gtp_util_config_read.c
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This file contains read function for gtputil.cfg configuration
 * Author: Ramprasad Reddy
 * Date: 1/3/2012
 * Description: Draft version 
 **/

#include  "gtp_util_config_read.h"


/**
 * @param     fileToRead : This is the full file path of the configuration file which will be read
 * @section   This function will read input configuration file and return the filled default
 *            configuration details to the caller of this function.
 *            Return 1 if ERROR is there otherwise 0
 **/
void readFromFileGtpUtilConfigTable(
  IN UINT8                   *fileToRead, 
  IO GtpUtilConfig           *ptrGtpUtilConfig,
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
  table = config_lookup(&cfg, "gtputilDefaultCfg");
  if(table NOTEQ NULL)
  {
    const char *destIp;

    config_setting_lookup_string(table, "destinationIP", &destIp); 
    strncpy(ptrGtpUtilConfig->destinationIP, destIp, MAX_SIZE_GTP_UTIL_DEST_IP );

    config_setting_lookup_int(table, "numPktsPerIteration", (int*)&ptrGtpUtilConfig->numPktsPerIteration); 
    config_setting_lookup_int(table, "interval", (int*)&ptrGtpUtilConfig->interval); 
    config_setting_lookup_int(table, "numIterations", (int*)&ptrGtpUtilConfig->numIterations); 
  }
  else
  {
    fprintf(stderr, "Table Not present : %s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
    *error = TRUE;
  }
  config_destroy(&cfg);
}

