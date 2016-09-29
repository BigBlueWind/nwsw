#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include <time.h>

#include "lte_bb_platform.h"

/****************************************************************************
*  Function:   imageVersion(UINT8 *version, UINT32 side)
*
*  Description: read image version
*
*  Prototype:  Version imageVersion(UINT8 *version, UINT32 side)
*
*  Inputs:      None
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     03/11/11
*
*  Originator:  PIPAD
****************************************************************************/
Version imageVersion(UINT8 *version, UINT32 aSide)
{
        char line[VERSION_SIZE];
        FILE *file;
        UINT8 side;
	    RwNorFlashReturnCode err;
        err = getBootSide(&side);
	    if(err == RWNORFLASHRETURNCODE_FAILURE)
		  return VERSION_ERR_INVALID_INPUT;
        if(aSide == ACTIVE_SIDE)
        {
                file = fopen(ACTIVE_VERSION_FILENAME, "r");
        }
        else if(aSide == PASSIVE_SIDE)
        {
                system("mkdir -p /mnt/filesystem");
                if(side == 'B' || side == 'b')
                {
                        system(MOUNT_JFFS2_SIDEB);
                }
                else if(side == 'A' || side == 'a')
                {
                        system(MOUNT_JFFS2_SIDEA);
                }
                file = fopen(PASSIVE_VERSION_FILENAME, "r");
        }
        else
        {
                return VERSION_ERR_INVALID_INPUT;
        }
        if(file!= NULL)
        {
                while(fgets(line, sizeof(line), file) != NULL)
                {
                        strcpy((char *)version,line);
                }
                fclose(file);
        }
        else
        {
                return VERSION_ERR_FILE_OPEN;
        }
  	if(aSide == PASSIVE_SIDE)
        {
                if(side == 'B' || side == 'b')
                {
                        system(UMOUNT_JFFS2_SIDEB);
                }
                else if(side == 'A' || side == 'a')
                {
                        system(UMOUNT_JFFS2_SIDEA);
                }
          system("rm -rf /mnt/filesystem");
        }
        return VERSION_SIDE_SUCCESS;

}

