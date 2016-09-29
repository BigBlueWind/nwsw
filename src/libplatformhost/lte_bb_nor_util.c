#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include <netinet/ether.h>

#include "lte_bb_nor_util.h"

/****************************************************************************
*  Function:   setMacAddress(UINT8 *pMacAddress)
*
*  Description:
*
*  Prototype:   SetGetMacAddressReturnCode setMacAddress(UINT8 *pMacAddress)
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
SetGetMacAddressReturnCode setMacAddress(UINT8 *pMacAddress,MacAdressPort portNum)
{
  return SETGETMACADDRESSRETURNCODE_SUCCESS;
}
/****************************************************************************
*  Function:   getMacAddress(UINT8 portNum)
*
*  Description:
*
*  Prototype:   SetGetMacAddressReturnCode getMacAddress(UINT8 *pMacAddress,UINT8 portNum)
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
SetGetMacAddressReturnCode getMacAddress(struct ether_addr * pMacAddr,MacAdressPort portNum )
{
  return SETGETMACADDRESSRETURNCODE_SUCCESS;
}
/****************************************************************************
*  Function:    writeConfDatatoNorFlash(UINT8 *pConfData,UINT8 size)
*
*  Description:
*
*  Prototype:   RwNorFlashReturnCode writeConfDatatoNorFlash(UINT8 *pConfData,UINT8 size)
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
RwNorFlashReturnCode writeConfDatatoNorFlash(UINT8 *pConfData,UINT8 size)
{
  return RWNORFLASHRETURNCODE_SUCCESS;
}
/****************************************************************************
*  Function:   ReadConfDatatoNorFlash(UINT8 *pConfData,UINT16 size)
*
*  Description:
*
*  Prototype:   RwNorFlashReturnCode  ReadConfDatatoNorFlash(UINT8 *pConfData,UINT16 size)
*
*  Inputs:      None
*
*  Outputs:     
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2010
*
*  Created:     10/01/11
*
*  Originator:  PIPAD
****************************************************************************/
RwNorFlashReturnCode ReadConfDatatoNorFlash(UINT8 *pConfData,UINT8 size)
{
  return RWNORFLASHRETURNCODE_SUCCESS;
}
/****************************************************************************
*  Function:   setActiveSide(UINT8 *aSideData)
*
*  Description:
*
*  Prototype:   RwNorFlashReturnCode  setActiveSide(UINT8 *aSideData)
*
*  Inputs:      None
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2011
*
*  Created:     23/05/11
*
*  Originator:  PIPAD
****************************************************************************/
RwNorFlashReturnCode setActiveSide(UINT8 *aSideData)
{
  return RWNORFLASHRETURNCODE_SUCCESS;
}
/****************************************************************************
*  Function:   getActiveSide(UINT8 *aSideData)
*
*  Description:
*
*  Prototype:   RwNorFlashReturnCode  getActiveSide(UINT8 *aSideData)
*
*  Inputs:      None
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2011
*
*  Created:     23/05/11
*
*  Originator:  PIPAD
****************************************************************************/
RwNorFlashReturnCode getActiveSide(UINT8 *aSideData)
{
  return RWNORFLASHRETURNCODE_SUCCESS;
}

/****************************************************************************
*  Function:   setBootSide()
*
*  Description:
*
*  Prototype:   RwNorFlashReturnCode setBootSide()
*
*  Inputs:      None
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2011
*
*  Created:     23/05/11
*
*  Originator:  PIPAD
****************************************************************************/
RwNorFlashReturnCode setBootSide()
{
  return RWNORFLASHRETURNCODE_SUCCESS;
}

/****************************************************************************
*  Function:   getBootSide(UINT8 *gBootData)
*
*  Description:
*
*  Prototype:   RwNorFlashReturnCode getBootSide(UINT8 *gBootData)
*
*  Inputs:      None
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2011
*
*  Created:     23/05/11
*
*  Originator:  PIPAD
****************************************************************************/
RwNorFlashReturnCode getBootSide(UINT8 *gBootData)
{
  return RWNORFLASHRETURNCODE_SUCCESS;
}


/****************************************************************************
*  Function:   ApplyLongPostontheBoard( )
*
*  Description: writes 0x01 in the nor flash if long POST is required
*
*  Prototype:   void ApplyLongPostontheBoard(void)
*
*  Inputs:      None
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2011
*
*  Created:     13/05/11
*
*  Originator:  PIPAD
****************************************************************************/

RwNorFlashReturnCode ApplyLongPost(UINT8 Post_DDR_FLASH)
{
  return RWNORFLASHRETURNCODE_SUCCESS;
}

/****************************************************************************
*  Function:   ReadPostResults( )
*
*  Description: reading the Post results
*
*  Prototype:   void ReadPostResults(void)
*
*  Inputs:      None
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2011
*
*  Created:     13/05/11
*
*  Originator:  PIPAD
****************************************************************************/

RwNorFlashReturnCode ReadPostResults( char *postlog[5])
{
   return RWNORFLASHRETURNCODE_SUCCESS;
}

/****************************************************************************
*  Function:   readInventoryInfo()
*
*  Description: reading inventoryInfo
*
*  Prototype:   RwNorFlashReturnCode readInventoryInfo(InventoryInfo *inventoryData)
*
*  Inputs:      InventoryInfo
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2011
*
*  Created:     13/10/11
*
*  Originator:  PIPAD
****************************************************************************/

RwNorFlashReturnCode readInventoryInfo(InventoryInfo *inventoryData)
{
  return RWNORFLASHRETURNCODE_SUCCESS;
}

/****************************************************************************
*  Function:   writeInventoryInfo()
*
*  Description: write inventoryData
*
*  Prototype:   RwNorFlashReturnCode writeInventoryInfo(InventoryInfo *inventoryData)
*
*  Inputs:      InventoryInfo
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2011
*
*  Created:     13/10/11
*
*  Originator:  PIPAD
****************************************************************************/

RwNorFlashReturnCode writeInventoryInfo(InventoryInfo *inventoryData)
{
  return RWNORFLASHRETURNCODE_SUCCESS;
}
/****************************************************************************
*  Function:   u-bootVersion()
*
*  Description: read u-bootVersion
*
*  Prototype:   RwNorFlashReturnCode u-bootVersion(unsigned char *version)
*
*  Inputs:      InventoryInfo
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2011
*
*  Created:     13/10/11
*
*  Originator:  PIPAD
****************************************************************************/

RwNorFlashReturnCode uBootVersionWrite (unsigned char *version)
{
  return RWNORFLASHRETURNCODE_SUCCESS;
}

/****************************************************************************
*  Function:   u-bootVersionRead()
*
*  Description: read u-bootVersion
*
*  Prototype:   RwNorFlashReturnCode u-bootVersionRead(unsigned char *version)
*
*  Inputs:      
*
*  Outputs:
*
*  Disclosure:  Copyright, Powerwave Technologies, Inc. 2011
*
*  Created:     13/10/11
*
*  Originator:  PIPAD
****************************************************************************/

RwNorFlashReturnCode uBootVersionRead (unsigned char *version)
{
  return RWNORFLASHRETURNCODE_SUCCESS;
}

