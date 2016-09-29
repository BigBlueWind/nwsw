#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>

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
   struct ether_addr *pInputMacAddr;
   UINT32  offSet; 
   SINT16  fileDescriptor; 

//   pInputMacAddr = (struct ether_addr *)malloc(sizeof(pMacAddress));


   if(pMacAddress == NULL)
   {
      return SETGETMACADDRESSRETURNCODE_INVALID_INPUT; 
   }

   /* convert ether address from ascii to network order */
   pInputMacAddr = ether_aton((char *)pMacAddress);

   if(pInputMacAddr == NULL)
   {
      return SETGETMACADDRESSRETURNCODE_INVALID_INPUT; 
   }

   if ((fileDescriptor = open(LTE_BB_NOR_PART,O_RDWR)) < 0 )
   {
      return SETGETMACADDRESSRETURNCODE_FAILURE; 
   }
   if(portNum == MACADDRESSPORT_ETH1)
   {
      /* caluculate the offset */
      offSet = STRUCT_OFFSET(NorData,etherAddressEth0);
   }
   else if(portNum == MACADDRESSPORT_ETH2)
   {
      /* caluculate the offset */
      offSet = STRUCT_OFFSET(NorData,etherAddressEth1);
   }

   if (-1 == lseek(fileDescriptor,offSet,SEEK_SET)) 
   {
      close(fileDescriptor);
      return SETGETMACADDRESSRETURNCODE_FAILURE; 
   }

   if ((write(fileDescriptor,pInputMacAddr,sizeof(struct ether_addr))) < 0 )
   {
      close(fileDescriptor);
      return SETGETMACADDRESSRETURNCODE_FAILURE; 
   }

   close(fileDescriptor);
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
   UINT32  offSet;  
   SINT16  fileDescriptor;
   struct ether_addr macRead;
   //SINT8  * pdata =NULL; 
   char  * pdata =NULL; 

   if(pMacAddr == NULL)
   {
      return SETGETMACADDRESSRETURNCODE_INVALID_INPUT; 
   }

   if ((fileDescriptor = open(LTE_BB_NOR_PART,O_RDWR)) < 0 )
   {
      return SETGETMACADDRESSRETURNCODE_FAILURE; 
   }
   if(portNum == MACADDRESSPORT_ETH1)
   {
      /* caluculate the offset */
      offSet = STRUCT_OFFSET(NorData,etherAddressEth0);
   }
   else if(portNum == MACADDRESSPORT_ETH2)
   {
      /* caluculate the offset */
      offSet = STRUCT_OFFSET(NorData,etherAddressEth1);
   }

   if (-1 == lseek(fileDescriptor,offSet,SEEK_SET)) 
   {
      close(fileDescriptor);
      return SETGETMACADDRESSRETURNCODE_FAILURE; 
   }

   if ((read(fileDescriptor,&macRead,sizeof(macRead))) < 0 )
   {
      close(fileDescriptor);
      return SETGETMACADDRESSRETURNCODE_FAILURE; 
   }
  

   pdata = ether_ntoa((struct ether_addr *)&macRead);
   if(pdata == NULL)
   {
      close(fileDescriptor);
      return SETGETMACADDRESSRETURNCODE_INVALID_INPUT; 
   }
   else
   {
      memcpy(pMacAddr,&macRead,sizeof(macRead));
   }

   close(fileDescriptor);
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
 
   UINT32  offSet;  
   SINT16  fileDescriptor;

   if(pConfData == NULL)
   {
      return RWNORFLASHRETURNCODE_INVALID_INPUT; 
   }
   if ((fileDescriptor = open(LTE_BB_NOR_PART,O_RDWR)) < 0 )
   {
      return RWNORFLASHRETURNCODE_FAILURE; 
   }
     
   /* caluculate the offset */
   offSet = STRUCT_OFFSET(NorData,confdata);

   if (-1 == lseek(fileDescriptor,offSet,SEEK_SET)) 
   {
      close(fileDescriptor);
      return RWNORFLASHRETURNCODE_FAILURE; 
   }

   if ((write(fileDescriptor,pConfData,size)) < 0 )
   {
      close(fileDescriptor);
      return RWNORFLASHRETURNCODE_FAILURE; 
   }

   close(fileDescriptor);
   return RWNORFLASHRETURNCODE_SUCCESS;
}
/****************************************************************************
*  Function:   ReadConfDatafromNorFlash(UINT8 *pConfData,UINT16 size)
*
*  Description:
*
*  Prototype:   RwNorFlashReturnCode  ReadConfDatafromNorFlash(UINT8 *pConfData,UINT16 size)
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
RwNorFlashReturnCode ReadConfDatafromNorFlash(UINT8 *pConfData,UINT8 size)
{
   UINT16  offSet;  
   SINT16  fileDescriptor;

   if(pConfData == NULL)
   {
      return RWNORFLASHRETURNCODE_INVALID_INPUT; 
   }
   if ((fileDescriptor = open(LTE_BB_NOR_PART,O_RDWR)) < 0 )
   {
      return RWNORFLASHRETURNCODE_FAILURE; 
   }

   /* caluculate the offset */
   offSet = STRUCT_OFFSET(NorData,confdata);

   if (-1 == lseek(fileDescriptor,offSet,SEEK_SET)) 
   {
      close(fileDescriptor);
      return RWNORFLASHRETURNCODE_FAILURE; 
   }

   if ((read(fileDescriptor,pConfData,size)) < 0 )
   {
      close(fileDescriptor);
      return RWNORFLASHRETURNCODE_FAILURE; 
   }
  
 
   close(fileDescriptor);
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

        UINT32  offSet;
        SINT16  fileDescriptor;
        if(aSideData==NULL)
        {
                return RWNORFLASHRETURNCODE_INVALID_INPUT;
        }
        fileDescriptor = open(LTE_BB_NOR_PART,O_RDWR);
        if(fileDescriptor <0)
        {
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        /* calculate the offset of activeSide */
        offSet=STRUCT_OFFSET(NorData,activeSide);
        if (-1 == lseek(fileDescriptor,offSet,SEEK_SET))
        {
      		close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        if((write(fileDescriptor, aSideData,1))<0)
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        close(fileDescriptor);
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

        UINT32  offSet;
        SINT16  fileDescriptor;
        if(aSideData==NULL)
        {
                return RWNORFLASHRETURNCODE_INVALID_INPUT;
        }
        if((fileDescriptor = open(LTE_BB_NOR_PART,O_RDWR))<0)
        {
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        /* calculate the offset of activeSide */
        offSet=STRUCT_OFFSET(NorData,activeSide);
        if (-1 == lseek(fileDescriptor,offSet,SEEK_SET))
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        if((read(fileDescriptor, aSideData,1))<0)
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        close(fileDescriptor);
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
        UINT8 sTargetData[2];
        UINT32  offSet1;
        UINT32  offSet2;
        SINT16  fileDescriptor;
        if((fileDescriptor = open(LTE_BB_NOR_PART,O_RDWR))<0)
        {
                return RWNORFLASHRETURNCODE_FAILURE;
        }
	/* calculate the offset of targetSide */
        offSet1=STRUCT_OFFSET(NorData,targetSide);
        if (-1 == lseek(fileDescriptor,offSet1,SEEK_SET))
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        if((read(fileDescriptor,sTargetData,1))<0)
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
	/* calculate the offset of actualBootSide */
        offSet2=STRUCT_OFFSET(NorData,actualBootSide);
        if(-1 == lseek(fileDescriptor,offSet2,SEEK_SET))
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        if((write(fileDescriptor,sTargetData,1))<0)
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        close(fileDescriptor);
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
        UINT32  offSet;
        SINT16  fileDescriptor;
        if((fileDescriptor = open(LTE_BB_NOR_PART,O_RDWR))<0)
        {
                return RWNORFLASHRETURNCODE_FAILURE;
        }
	/* calculate the offset of actualBootSide */
        offSet=STRUCT_OFFSET(NorData,actualBootSide);
        if(-1 == lseek(fileDescriptor,offSet,SEEK_SET))
        {
        	close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        if((read(fileDescriptor,gBootData,1))<0)
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        close(fileDescriptor);
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
   UINT16  offSet;
   SINT16  fileDescriptor;

   UINT8   lPostDDRTest,lPostFlashTest,lPostDDRFlashTest;

   lPostDDRTest = 0x01;
   lPostFlashTest = 0x02;
   lPostDDRFlashTest = 0x03;

   if ((fileDescriptor = open(LTE_BB_NOR_PART,O_RDWR)) < 0 )
   {
      return RWNORFLASHRETURNCODE_FAILURE;
   }

   /* caluculate the offset */
   offSet = STRUCT_OFFSET(NorData,long_post_addr);

   if (-1 == lseek(fileDescriptor,offSet,SEEK_SET))
   {
      close(fileDescriptor);
      return RWNORFLASHRETURNCODE_FAILURE;
   }

   if(Post_DDR_FLASH == POST_DDR)
   {
        if ((write(fileDescriptor,&lPostDDRTest,1)) < 0 )
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
   }
   else if(Post_DDR_FLASH == POST_FLASH)
   {
        if ((write(fileDescriptor,&lPostFlashTest,1)) < 0 )
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
   }
   else if(Post_DDR_FLASH == POST_DDR_FLASH)
   {
       if ((write(fileDescriptor,&lPostDDRFlashTest,1)) < 0 )
       {
               close(fileDescriptor);
               return RWNORFLASHRETURNCODE_FAILURE;
       }
   }

   close(fileDescriptor);
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

RwNorFlashReturnCode ReadPostResults( unsigned char postlog[5])
{
   UINT16  offSet;
   SINT16  fileDescriptor;
   UINT8 uPostData[5];
   if ((fileDescriptor = open(LTE_BB_NOR_PART,O_RDWR)) < 0 )
   {
      return RWNORFLASHRETURNCODE_FAILURE;
   }
   /* caluculate the offset */
   offSet = STRUCT_OFFSET(NorData,postResults);
   if (-1 == lseek(fileDescriptor,offSet,SEEK_SET))
   {
      close(fileDescriptor);
      return RWNORFLASHRETURNCODE_FAILURE;
   }
   // read long post address
  // check long post applied or not
  // publish long post results if yes
  // reset to short post
   if ((read(fileDescriptor,uPostData,POST_LOG_SIZE)) < 0 )
   {
      close(fileDescriptor);
      return RWNORFLASHRETURNCODE_FAILURE;
   }
   close(fileDescriptor);

     if(uPostData[0] != 0xff)
     postlog[0]=0x1;//long post
   else
      postlog[0]=0x0; //short post

   if(uPostData[1] == 0x0)
   {
        postlog[1] = POST_TEST_PASS; //"Short POST of DDR passed" ;
   }
   else if(uPostData[1] == 0x1)
   {
        postlog[1] = POST_TEST_FAIL; //"Short POST of DDR failed" ;
   }
   else
   {
        postlog[1] = POST_TEST_NOT_IMPLEMENTED; //"Short POST of DDR not implemented yet";
   }


   if(uPostData[2] == 0x0)
   {
        postlog[2] = POST_TEST_PASS; //"Long POST of DDR passed" ;
   }
   else if(uPostData[1] == 0x1)
   {
        postlog[2] = POST_TEST_FAIL;   //"Long POST of DDR failed" ;
   }
   else
   {
        postlog[2] = POST_TEST_NOT_IMPLEMENTED;   //"Long POST of DDR not implemented yet" ;
   }

   if(uPostData[3] == 0x0)
   {
        postlog[3] = POST_TEST_PASS;   //"Short POST of NAND flash passed";
   }
   else if(uPostData[3] == 0x1)
   {
        postlog[3] = POST_TEST_FAIL;  //"Short POST of NAND flash failed";
   }
   else
   {
        postlog[3] = POST_TEST_NOT_IMPLEMENTED;   //"Short POST of NAND flash not implemented yet";
   }

   if(uPostData[4] == 0x0)
   {
        postlog[4] = POST_TEST_PASS; //"Long POST of NAND Flash passed";
   }
   else if(uPostData[4] == 0x1)
   {
        postlog[4] = POST_TEST_FAIL; //"Long POST of NAND flash failed";
   }
   else
   {
        postlog[4] = POST_TEST_NOT_IMPLEMENTED; //"Long POST of NAND Flash not implemented yet";
   }
 /*  if(uPostData[4] == 0x0)
   {
        postlog[4] = POST_TEST_PASS;   //"No bad blocks in the NAND flash";
   }
   else if(uPostData[4] == 0xFF)
   {
        postlog[4] = POST_TEST_NOT_IMPLEMENTED;  //"Long POST of NAND Flash not implemented yet";
   }
   else
   {
        //snprintf(postlog[4],2,"The no of bad blocks are %d",uPostData[4]);
        postlog[4] = uPostData[4];  
   }
*/
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
	UINT32  offSet;
        SINT16  fileDescriptor;
        UINT8 tempbuf[INVENTORYSIZE];
        if ((fileDescriptor = open(LTE_BB_NOR_PART,O_RDWR)) < 0 )
        {
                return RWNORFLASHRETURNCODE_FAILURE;
        }

        offSet=STRUCT_OFFSET(NorData,inventoryInfo);
        if(-1==lseek(fileDescriptor,offSet,SEEK_SET))
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        if((read(fileDescriptor, &tempbuf, sizeof(InventoryInfo)))<0)
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        memcpy((unsigned char *)inventoryData,&tempbuf,sizeof(InventoryInfo));
 	close(fileDescriptor);
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
	UINT32  offSet;
        SINT16  fileDescriptor;
        UINT8 tempbuf[INVENTORYSIZE];
        if(inventoryData == NULL)
        {
                return RWNORFLASHRETURNCODE_INVALID_INPUT;
        }
        memcpy(&tempbuf,(unsigned char *)inventoryData,sizeof(InventoryInfo));
        if((fileDescriptor = open(LTE_BB_NOR_PART,O_RDWR))<0)
        {
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        offSet=STRUCT_OFFSET(NorData,inventoryInfo);
        if(-1==lseek(fileDescriptor,offSet,SEEK_SET))
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        if((write(fileDescriptor, &tempbuf, sizeof(InventoryInfo)))<0)
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        close(fileDescriptor);
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
        SINT16 fileDescriptor;
        UINT32 offSet;
	if(version == NULL)
	{
		return RWNORFLASHRETURNCODE_INVALID_INPUT;	
	}
        if((fileDescriptor=open(LTE_BB_NOR_PART, O_RDWR))<0)
        {
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        offSet=STRUCT_OFFSET(NorData, uBootversion);
        if(-1 == lseek(fileDescriptor,offSet,SEEK_SET))
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        if((write(fileDescriptor,version,9))<0)
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        close(fileDescriptor);
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

RwNorFlashReturnCode uBootVersionRead (char *version)
{
        SINT16 fileDescriptor;
        UINT32 offSet;
        if((fileDescriptor=open(LTE_BB_NOR_PART, O_RDWR))<0)
        {
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        offSet=STRUCT_OFFSET(NorData, uBootversion);
        if(-1 == lseek(fileDescriptor,offSet,SEEK_SET))
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
        if((read(fileDescriptor,version,9))<0)
        {
                close(fileDescriptor);
                return RWNORFLASHRETURNCODE_FAILURE;
        }
	snprintf(version,9,"%d%d%c%d%d%c%d%d",version[1],version[2],version[3],version[4],version[5],version[6],version[7],version[8]);
        close(fileDescriptor);
        return RWNORFLASHRETURNCODE_SUCCESS;
}

