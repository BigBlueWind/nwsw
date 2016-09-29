#include <stdio.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <time.h>
#define __USE_UNIX98
#include <unistd.h>

#include "basic_types.h"
#include "lte_bb_platform_spi.h"

/****************************************************************************
*  Function:    WriteAD9548(UINT32 RegAddr, UINT8 RegData)
*
*  Description:
*
*  Prototype:   Ad9548ReturnCode WriteAD9548(UINT32 RegAddr, UINT8 RegData)
*
*  Inputs:      RegAddr & Value
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     21/10/11
*
*  Originator:  PIPAD
****************************************************************************/

Ad9548ReturnCode WriteAD9548(UINT32 RegAddr, UINT8 RegData)
{
   SINT16  fileDescriptor;
   if((fileDescriptor = open(LTE_BB_AD9548_PART, O_RDWR))<0)
   {
	return AD9548RETURNCODE_ERR_SPI_DRV_OPEN; 
   }
   if((pwrite(fileDescriptor, &RegData, 1, RegAddr ))<0)
   {
	close(fileDescriptor);
	return ERROR;
   }
   close(fileDescriptor);
   return OK; 
}
/****************************************************************************
*  Function:    ReadAD9548(UINT32 RegAddr, UINT8 *RegData)
*
*  Description:
*
*  Prototype:   Ad9548ReturnCode ReadAD9548(UINT32 RegAddr, UINT8 *RegData)
*
*  Inputs:      RegAddr
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     21/10/11
*
*  Originator:  PIPAD
****************************************************************************/
Ad9548ReturnCode ReadAD9548(UINT32 RegAddr, UINT8 *RegData)
{
   SINT16  fileDescriptor;
   if((fileDescriptor = open(LTE_BB_AD9548_PART, O_RDWR))<0)
   {
        return AD9548RETURNCODE_ERR_SPI_DRV_OPEN;
   }
   if((pread(fileDescriptor, RegData, 1, RegAddr ))<0)
   {
	close(fileDescriptor);
	return ERROR;
   }
   close(fileDescriptor);
   return OK;
}
/****************************************************************************
*  Function:    ReadCDCE620005(UINT8 RegAddr, UINT32 *RegData)
*
*  Description:
*
*  Prototype:   CdceReturnCode ReadCDCE620005(UINT8 RegAddr, UINT32 *RegData)
*
*  Inputs:      RegAddr
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     21/10/11
*
*  Originator:  PIPAD
****************************************************************************/
CdceReturnCode ReadCDCE620005(UINT8 RegAddr,UINT32 *RegData)
{
    SINT16 fileDescriptor;
    if((fileDescriptor = open (LTE_BB_CDCE62005_PART, O_RDWR))<0)
    {
	return CDCERETURNCODE_ERR_SPI_DRV_OPEN;
    }
    if((pread(fileDescriptor, RegData, 4, RegAddr * sizeof(int)))<0)
    {
	close(fileDescriptor);
	return ERROR; 
    }
    close(fileDescriptor);
    return OK;
}

