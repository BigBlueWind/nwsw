//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL spidiag.h
 *
 * @brief The SPI diagnostics module
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef SPIDIAG_H_
#define SPIDIAG_H_

#include "4gmx.h"

//#define _SPIDIAGTESTSPI_

#define SPI_DEV_NUM 16
#define SPI_DUMMY_BYTE 0xFF

// The initialization structure for SPI controller
typedef struct
{
	UINT32 CTRL0_CFS_NSMicrowire;
	UINT32 CTRL0_SRL_LoopMode;
	UINT32 CTRL0_SLV_OE_Broadcast;
	UINT32 CTRL0_TMOD_TransferMode;
	UINT32 CTRL0_SCPOL_MOPolarity;
	UINT32 CTRL0_SCPH_MOPhase;
	UINT32 CTRL0_FRF_Format;
	UINT32 CTRL0_DFS_DataSize;

	UINT32 CTRL1_NDF_QuantityReceivedData;

	UINT32 MWCR_MDD;
	UINT32 MWCR_MWMOD;

	UINT32 BAUDR_BaudRate;
	UINT32 TXFTLR_TransFIFOThr;
	UINT32 RXFTLR_RecFIFOThr;
	UINT32 IMR;
} SPI_INIT_PARAMS, *P_SPI_INIT_PARAMS;

// The structure for access to SPI controller CTRL0 register bits
typedef struct
{
	unsigned int CTRL0_DFS_DataSize       : 4; // bits 0-3
	unsigned int CTRL0_FRF_Format         : 2; // bit 4
	unsigned int CTRL0_SCPH_MOPhase       : 1; // bit 5
	unsigned int CTRL0_SCPOL_MOPolarity   : 1; // bit 6
	unsigned int CTRL0_TMOD_TransferMode  : 2; // bit 7
	unsigned int CTRL0_SLV_OE_Broadcast   : 1; // bit 8
	unsigned int CTRL0_SRL_LoopMode       : 1; // bit 9
	unsigned int CTRL0_CFS_NSMicrowire    : 4; // bits 12-15
	unsigned int reserved                 : 16; // bits 16-31 reserved
} *P_SPI_CTRL0_REG;

// The structure for access to SR (Status register) of SPI controller
typedef struct
{
	unsigned char BSY  :1; // bit 0
	unsigned char TFNF :1;
	unsigned char TFE  :1;
	unsigned char RFNE :1;
	unsigned char RFF  :1;
	unsigned char TXE  :1;
	unsigned char DCOL :1; // bit 6
	unsigned char res  :1; // bit 7 reserved
} *P_SPI_SR_REG;

// The structure for access to Status Register of EEPROM device
typedef struct
{
	unsigned int RDYBSY2   :1; // bit 0
	unsigned int ES        :1;
	unsigned int PS        :1;
	unsigned int SLE       :1;
	unsigned int RSTE      :1;
	unsigned int reserved1 :3;
	unsigned int RDYBSY1   :1;
	unsigned int WELbit    :1;
	unsigned int SWP       :2;
	unsigned int WPP       :1;
	unsigned int EPE       :1;
	unsigned int reserved2 :1;
	unsigned int SPRL      :1;
	unsigned int reserved3 :16;
}SPI_EEPROM_STAT_REG, *P_SPI_EEPROM_STAT_REG;

MXRC SPIDiagInit(UINT32* pReportData, UINT32* pReportSize);
void SPIDiagEnableDev(void);
void SPIDiagDisableDev(void);
MXRC SPIDiagWaitBusyFlag(void);
void SPIDiagSetCS(UINT32 DevCS);
MXRC SPIDiagGetStatus(UINT32* statusReg);
MXRC SPIDiagWaitEEPROMbusy(void);
MXRC SPIDiagWriteEnable(UINT32 DevCS);
MXRC SPIDiagWriteOpcode(UINT32 opcode);
MXRC SPIDiagWriteByte(UINT32 data);
MXRC SPIDiagReadByte03h(UINT8* pData);
MXRC SPIDiagWriteAddrEEPROM(UINT32 addr);
MXRC SPIDiagGetSectorProtectReg(UINT32 addr, UINT8* protectReg);
MXRC SPIDiagProtectSector(UINT32 DevCS, UINT32 addr);
MXRC SPIDiagUnprotectSector(UINT32 DevCS, UINT32 addr);
MXRC SPIDiagSectorErase4K(UINT32 DevCS, UINT32 addr);
MXRC SPIDiagRunProgramAlgorithm(UINT32 DevCS);
MXRC SPIDiagInitDevice(void);
MXRC SPIDiagInitDeviceEx(SPI_INIT_PARAMS initparams);
MXRC SPIDiagConfig(UINT32 cmd, UINT32 param);
MXRC SPIDiagLookForDevice(UINT32 DevID, UINT32* DevCS);
MXRC SPIDiagWriteEEPROM(UINT32 DevCS, UINT32 addr, UINT8* pData, UINT32 datasize);
MXRC SPIDiagReadEEPROM(UINT32 DevCS, UINT32 addr, UINT8* pData, UINT32 datasize);


MXRC SPIDiagTestSPI(void);


#endif /*SPIDIAG_H_*/

#ifdef __cplusplus
}
#endif

