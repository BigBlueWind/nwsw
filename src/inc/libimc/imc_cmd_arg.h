/**
 * @file    imc_cmd_arg.h
 * @brief This file contains function prototypes from processing command line
 *          arguments.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author :     Madhusudana Rao Kata
 * Date   :
 * Description :
 */
#ifndef IMC_CMD_ARG_H
#define IMC_CMD_ARG_H

#include  "basic_types.h"
#include  "version.h"
/* Process cmd line args */
void cmdLineParamHandler(
  IO int          argc,
  IO char         *argv[],
  IO char         *configDir,
  IO VersionInfo  *versionInfo
  );
#endif /* IMC_CMD_ARG_H */
