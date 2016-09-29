/**
 * @file    imc_cmd_arg.c
 * @brief   This file Process commandline arguments.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 *
 * @author  Madhusudana Rao Kata
 * @date    07-06-2010  Command Line argument Parser is added.
 *
 */

#include "imc_cmd_arg.h"
#include "log.h"
#include "imc_util.h"
#include "all_cfg.h"

/**
 * @param argc used for finding number of arguments passed
 * @param argv arguments passed while running modules
 * @param picoBsHomePath Path of configuration file
 * @details This function is used to parse command line arguments.
 */
void cmdLineParamHandler(
  IO int          argc,
  IO char         *argv[],
  IO char         *configDir,
  IO VersionInfo  *versionInfo
  )
{
  UINT8       isError = FALSE;
  UINT8       fullFilePath[MAX_BUFFER_SIZE];
  clearBytes( &fullFilePath, MAX_BUFFER_SIZE );
  UINT8       index ; 

  if(argc > 1)
  {
    for(index = 1;(strcmp (argv[index],"--rootDir") NOTEQ 0) &&( index < argc-1); index++) ;

    if(index < (argc-1))
    {
      VersionCfgContents   versionCfgContents;
      clearBytes( &versionCfgContents, sizeof(VersionCfgContents) );

      *configDir = '\0';
      sprintf( configDir, "%s/config/", argv[index+1] );
      sprintf( fullFilePath, "%s/version.cfg", configDir );
      readFromFileVersionCfgContentsTable( fullFilePath, &versionCfgContents, &isError );
      convertVersionCfgContentsToVersionInfo( &versionCfgContents, versionInfo );
      if ( isError EQ TRUE )
      {
        BS_ERROR_CONFIG_READ_FAILED()
        /* printf is used because log has not been initialised yet */
        printf("\n version.cfg not read correctly. Read of config file failed!! ");
        exit(EXIT_CONFIG_READ_FAIL);
      }

      /* Handle other cmd line arguments here */
      if ( argc > 3 )
      {
	    for(index = 1;(strcmp (argv[index],"--version") NOTEQ 0) &&( index < argc-1); index++);
        if (strcmp(argv[index], "--version") EQ 0)
        {
          /* show the version and exit.
          * The version is to be displayed on stdout: dont change to logInfo.
          */
          printf("\n****************************** ");
          printf("\n         Version Info ");
          printf("\n****************************** ");
          printf("\nexeName        = %s", argv[0]);
          printf("\nSW Type        = %u", versionInfo->swType );
          printf("\n SWTYPE_ENB_L2L3_SW=1");
          printf("\nSW Version     = %s",  versionInfo->version );
          printf("\nBuild on       = %s",  versionInfo->builtOn );
          printf("\n\n");
          exit(EXIT_SUCCESS);
        }
      }
    }
    else
    {
       printf("\n Enter --rootDir and <PATH of root> as command line argument!! \n");
       exit(EXIT_FAILURE);
    }
  }
}

