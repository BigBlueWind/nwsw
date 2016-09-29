#ifndef LTE_BB_NOR_UTIL_H
#define LTE_BB_NOR_UTIL_H

/* EEPROM Defines */

#include "basic_types.h"
#include <netinet/ether.h>

#define  EE_COM_INV_DATA_SIZE    100
#define  EE_MOD_TYPE_SIZE        9
#define  EE_FACTORY_CODE_SIZE    3
#define  EE_PART_NUM_SIZE        14
#define  EE_HW_REV_SIZE          4
#define  EE_MOD_SER_NUM_SIZE     6
#define  EE_MANUF_DATE_SIZE      5
#define  EE_LOT_CODE_SIZE        11
#define  EE_COM_INV_SPARE_SIZE   19
#define  BIST_LOG_SIZE           1024*10
#define  POST_LOG_SIZE           5

#define  NOR_CONF_DATA_MAX_SIZE  100
#define  LEN_MAC_ADDR            6
#define THIS_ADDR                7

#define INVENTORYSIZE		76
// This MACRO provides the 0 based address of a structure member 
#define STRUCT_OFFSET(a, member) ((int)&(((a *)0)->member))

#define  LTE_BB_NOR_PART "/dev/mtdblock1"

#define MACADDRESSPORT_ETH1 MACADDRESS_DEBUG_PORT
#define MACADDRESSPORT_ETH2 MACADDRESS_BACKHAUL_PORT
/* set struct alignment to 1 byte boundary */
#pragma pack(push, 1)

/* Macros used for BIST TESTS */
#define BIST_TEST_FAIL 		  0
#define BIST_TEST_PASS 		  1
#define BIST_TEST_NOT_IMPLEMENTED 2

/* Macros used for POST TESTS */
#define POST_TEST_FAIL 		  0
#define POST_TEST_PASS 		  1
#define POST_TEST_NOT_IMPLEMENTED 2


typedef enum e_longPostType
{
    POST_DDR = 0x01,
    POST_FLASH = 0x02 ,
    POST_DDR_FLASH = 0x03 ,
}longPostType;

 
typedef struct s_InventoryInfo
{
   UINT8            blockId;
   UINT8            tableFormat;
   UINT16           blockSize;
   SINT8            modType[EE_MOD_TYPE_SIZE];
   SINT8            factoryCode[EE_FACTORY_CODE_SIZE];
   SINT8            partNum[EE_PART_NUM_SIZE];
   SINT8            hwRev[EE_HW_REV_SIZE];
   SINT8            modSerNum[EE_MOD_SER_NUM_SIZE];
   SINT8            manufDate[EE_MANUF_DATE_SIZE];
   SINT8            lotCode[EE_LOT_CODE_SIZE];
   SINT8            comInvSpare[EE_COM_INV_SPARE_SIZE];
   UINT16           checksum;
}InventoryInfo;

typedef struct s_NorData
{
  InventoryInfo    inventoryInfo;
  UINT8            etherAddressEth0[LEN_MAC_ADDR];
  UINT8            etherAddressEth1[LEN_MAC_ADDR];
  UINT8            bistStatus;
  SINT8            confdata[NOR_CONF_DATA_MAX_SIZE];
  SINT8            bistResults[BIST_LOG_SIZE];
  UINT8            activeSide;
  UINT8            long_post_addr;
  UINT8            postResults[POST_LOG_SIZE];
  UINT8            actualBootSide;
  UINT8            countAddr;
  UINT8            targetSide;
  UINT8            uBootversion;
}NorData;

typedef enum e_SetGetMacAddressReturnCode
{
    SETGETMACADDRESSRETURNCODE_SUCCESS = 70,
    SETGETMACADDRESSRETURNCODE_FAILURE ,
    SETGETMACADDRESSRETURNCODE_INVALID_INPUT ,
}SetGetMacAddressReturnCode;

typedef enum e_RwNorFlashReturnCode
{
    RWNORFLASHRETURNCODE_SUCCESS = 75,
    RWNORFLASHRETURNCODE_FAILURE ,
    RWNORFLASHRETURNCODE_INVALID_INPUT ,
}RwNorFlashReturnCode;

typedef enum e_MacAddressPort
{
    MACADDRESS_DEBUG_PORT=1,
    MACADDRESS_BACKHAUL_PORT ,
}MacAdressPort;

/* restore original alignment from stack */
#pragma pack(pop)   

/* extern declarations*/
SetGetMacAddressReturnCode setMacAddress(UINT8 *pMacAddress,MacAdressPort portNum);
SetGetMacAddressReturnCode getMacAddress(struct ether_addr * pMacAddr,MacAdressPort portNum );
RwNorFlashReturnCode       writeConfDatatoNorFlash(UINT8 *pConfData,UINT8 size);
RwNorFlashReturnCode       ReadConfDatafromNorFlash(UINT8 *pConfData,UINT8 size);
RwNorFlashReturnCode       setActiveSide(UINT8 *aSideData);
RwNorFlashReturnCode       getActiveSide(UINT8 *aSideData);
RwNorFlashReturnCode       setBootSide();
RwNorFlashReturnCode       getBootSide(UINT8 *aSideData);
RwNorFlashReturnCode       ApplyLongPost(UINT8 Post_DDR_FLASH);
RwNorFlashReturnCode       ReadPostResults(unsigned char postlog[5]);
RwNorFlashReturnCode       readInventoryInfo(InventoryInfo *inventoryData);
RwNorFlashReturnCode       writeInventoryInfo(InventoryInfo *inventoryData);
RwNorFlashReturnCode       uBootVersionRead(char *version);
RwNorFlashReturnCode       uBootVersionWrite(unsigned char *version);
#endif /*  LTE_BB_NOR_UTIL_H */

