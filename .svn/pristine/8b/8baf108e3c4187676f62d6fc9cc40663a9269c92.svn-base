//-------------------------------------------------------------------------------------------
/** @file ltephydef.h
 *
 * @brief This file defines constants and the external functions and variables
 *        used in the implementation of an LTE PHY
 * @author Mindspeed Technologies
 * @version $Revision: 1.28 $
 *
 * COPYRIGHT&copy; 2009-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/

//-------------------------------------------------------------------------------------------

#ifndef LTEPHYDEF_H
#define LTEPHYDEF_H

#include "LtePhyL2Api.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define COMMON_MEM	0
#define DDR_MEM		1
#define IRAM_MEM	2
#define CRAM_MEM	3
#define NCNB_MEM	4
#define CACHEABLE	DDR_MEM

#define MAX_PHICH   50

#define BASESTATION    0
#define MOBILESTATION  1

#define         MAX_OUTPUT_BLK_SIZE 100800 // Tracing

#define         MAX_INPUT_TRANSPORT_BYTE_SIZE 12600
#define         MAX_OUTPUT_TRANSPORT_BIT_SIZE (8*MAX_INPUT_TRANSPORT_BYTE_SIZE)
#define         MAX_NUM_CODE_TRANSPORT_SIZE 17
#define         NUM_PARITY 24
#define         MAX_SUB_CODE_SIZE 6144
#define         NUM_TC_STREAMS 3
#define         CC_MEMORY 3
#define         MAX_ROW_RBASSIGNMENT 64
#define         MAX_COL_RBASSIGNMENT  2
#define         ROW_K_L 2
#define         COL_K_L 16000
#define         MAX_NUMBER_FILL_BITS 64
#define         NUM_SLOTS            20

#define         PI_FX_BY2 12868
#define         PI_FX 25736

#define CRC24A        3      //number of bytes in CRC24A
#define CRC16         2      //number of bytes in CRC16

//For now, set maximum number of dependencies between tasks to be 64
//We can increase if it is needed.
#define MAX_DEPENDENCIES    64

#define         QUADRUPLET 4
#define         N_PER_ROW 6
#define         INV_12 2731
#define         NUM_BITS_PUCCH_F2_PER_SYM 24 // number of bits for Format2,2B
#define         NUM_BITS_PUCCH_F2A_PER_SYM 12 // number of bits for Format2A

#define			MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT 18000 // bytes for 20MHz 100 users per antenna
#define			MAX_BYTES_PER_ANTENNA_SCRAMBLER_OUT_C 90000 // bytes for 20MHz 100 users per antenna.
				// This is bigger as the PBCH is 1 byte per bit in C-Only.


// Constant definitions to be used throughout the LTE PHY
// MAXNUMPHYINSTANCES corresponds to the maximum number of supported PHY
// instances
#define MAXNUMPHYINSTANCES 4
#define MAX_DL_TASK_LISTS_PER_SF 1
#define MAX_UL_TASK_LISTS_PER_SF 1

#ifdef RT_API_PROC
#define MAX_NUM_OF_SF_CTX 2
#else
#define MAX_NUM_OF_SF_CTX 1
#endif

#define UL_TASKLIST_START_IDX 0
#define DL_TASKLIST_START_IDX 2
#define PRACH_TASKLIST_START_IDX 4


#define MAX_SDU_PER_SUBFRAME	19

// MAXNUMLAYERS corresponds to the maximum number of layers supported
// for MIMO configurations
#define MAXNUMLAYERS 4
// Maximum number of channels per subframe
#define LTEMXSFCH 40
// LTEMAXCHAN is the maximum number of channels supported per sector
#define LTEMAXCHAN 400

// PHY Types definitions
// Base Station LTE PHY
#define BS_PHY 0
// Mobile Station LTE PHY
#define MS_PHY 1
// Carrier Spacing Options
#define CS_7500 7500
#define CS_15K  15000
// Reference number of Resource Elements per PRB pair to be used for
// Shared Data Channel allocations and Transport Block Size determination
// per R1-081284 (assumes 3 symbols for control info, 2 Antennas,
// No sync signals, No Pbch, Normal CP
#define RefRePerPrbPair 120
#define RefScPerRb 12
#define RefSymPerRb 5
// Maximum Bits Available for TB Transmission need to remove and
// compute on the fly from the TX SDU size
//#define G 6048

#define MAX_PUCCH   192
#define MAX_UL_SRS  400

#define FAILURE 1

#define UNCODED_OUT 1
#define FEC_DEC_OUT 2
#define PROFILE_INFO 3
#define IQ_SAMPLES 4
#define SRS_CHEST_OUT 5
#define PRACH_OUT 6

// Bypass check for proper Transport Block Sizes from MAC Layer
#define TEST_FORCING_ANY_TBSIZE
// Function prototypes
extern U32 LteParseMsg(U32 phyInstance, U32* inMsg, U32 inMsgSize, U32* outMsg,
                       U32* outMsgSize);
extern U32 LteBsTx(U32 phyInstance, S16* sigOut, U32* outMsg, U32* outMsgSize);
extern U32 LteBsRx(U32 phyInstance, S16* sigIn, U32* outMsg, U32* outMsgSize);
#ifdef _USRDLL
__declspec(dllexport)
#else
extern
#endif /* _USRDLL */
U32 LteBsPhyMain(U32 phyInstance, S16* sigOut, S16* sigIn, U32* inMsg,
                 U32 inMsgSize, U32* outMsg, U32* outMsgSize);
#ifdef _USRDLL
__declspec(dllexport)
#else
extern
#endif /* _USRDLL */
U32 LteBsPhyDllMain(U32 phyInstance, S16* sigOut, S16* sigIn,
                 U32* inputMessage, U32 inputMessageSize,
                 U32* outputMessage, U32* outputMessageSize);
extern U32 LtePhyInitialization(U32 phyInstance, PINITPARM parmPtr);
extern U32 LteBsPhyInit(U32 phyInstance, PINITPARM parmPtr);
extern U32 LteMsPhyInit(U32 phyInstance, PINITPARM parmPtr);
extern S16 LteDetermineSymbolsPerSlot(S16 cpType, S16 nRbSc);
extern void LteGenerateAPIMsg(U32 phyInstance, U32* outMsg, U32* outMsgSize,
                              U32 msgType, U32 status);
extern void LteGenerateAPIMsg__1(PLTEPHYDESC pPhy, U32* outMsg, U32* outMsgSize, U32 msgType,
                              U32 status);
extern U32 LteBsPhyTxVectorProcessing(U32 phyInstance, PDLSUBFRDESC ptr, U32 len);
extern U32 LteBsPhyRxVectorProcessing(U32 phyInstance, PULSUBFRDESC ptr, U32 len);
extern void LteGenerateTxSduConfMsg(U32 phyInstance, U32* outMsg,
                                    U32* outMsgSize, U32 status, U32 chanId);
extern U32 LteBsTxSduProcessing(U32 phyInstance, U32* ptr, U32 chanId,
                                PTXSDUREQ ptrSDU, U32 cwId);
extern U32 LteBsTxHiSduProcessing(U32 phyInstance, PTXSDUREQ ptr, U32 chanId,
                                U32 numBytes, U32 cwId);


extern void LteGenerateRxSduIndMsg(U32 phyInstance, U32* outMsg, U32* outMsgSize,
                             U32 status, U32 chanId, U32 i);
#ifndef _WIN32
extern U32 LteSendMsgToMac(U32* outMsg, U32 outMsgSize);
#endif
U32 LteStart(PSTARTREQ pParam);
U32 LteStop(U32 phyInstance);

//*****************************************************************************
//**********                   UPLINK HARQ COMBINING                 **********
//*****************************************************************************

    // Define number of HARQ processes
    #define P_HARQ_PROC     8

    // Define maximum number of transmissions allowed to be combined for
    // each HARQ process
    #define P_HARQ_COMB     4

    // Define maximum number of soft decisions that can be generated in one
    // HARQ interval (1 subframe)
    #define P_HARQ_SFDEC     93318

    // Define maximum number of HARQ users (UEs)
    #define P_HARQ_USERS    100

//*****************************************************************************

#endif /* LTEPHYDEF_H */
