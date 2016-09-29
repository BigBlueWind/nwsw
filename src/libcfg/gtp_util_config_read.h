/**
 * @file    gtp_util_config_read.h
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This file contains functions prototype of read configuration function for gtputil.cfg
 * Author: Ramprasad Reddy
 * Date: 1/3/2012
 * Description: Draft version 
 **/

#ifndef  GTP_UTIL_CONFIG_READ_H
#define  GTP_UTIL_CONFIG_READ_H

#include "all_cmn.h"

#define MAX_SIZE_GTP_UTIL_DEST_IP 15

typedef struct s_GtpUtilConfig
{
 IPV4ADDRESS   destinationIP[MAX_SIZE_GTP_UTIL_DEST_IP];
 unsigned int  numPktsPerIteration;
 unsigned int  interval;
 unsigned int  numIterations;
}GtpUtilConfig;

void readFromFileGtpUtilConfigTable(
  IN UINT8                   *fileToRead, 
  IO GtpUtilConfig           *ptrGtpUtilConfig,
  IO UINT8                   *error
  );

#endif /*GTP_UTIL_CONFIG_READ_H*/
