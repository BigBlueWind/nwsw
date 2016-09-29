/**
 * file    shm.h
 * @brief   
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author   
 *
 *  */

#ifndef SHM_H
#define SHM_H

#include "basic_types.h"   /* UINT16, UINT8  */
#include <sys/shm.h>
#include <pthread.h>

/*
 * define macros 
 */
#define NUM_COLS    10
#define COL_EXETYPE  0
#define COL_EXENAME  1
#define COL_ERRKEY   2
#define COL_ERRSZ    3
#define COL_CFGKEY   4
#define COL_CFGSZ    5
#define COL_CTRKEY   6
#define COL_CTRSZ    7
#define COL_INFOKEY  8
#define COL_INFOSZ   9
#define MAX_LOG_SIZE 10485760 //10MB

#define MAX_LOG_FILENAME 100
/*
 * Define enumerator to indicate exe type
 */
typedef enum 
{
	BSM_EXE = 0,
	CIH1_EXE,
	CIH2_EXE,
	CIH3_EXE,
	DEH_EXE,
	GTP_EXE,
	EMS_SIM_EXE,
	FTH_EXE,
	LTH_EXE,
	PMH_EXE,
	PRM_EXE,
	RH_SIM_EXE,
	CP_EXE,
	SIH_EXE,
	WIH_EXE,
	LKS_SIM_EXE,
#ifdef PWAV_L2SIM
  L2SIM_EXE,
#endif
	MAX_EXE
} ExeType;

typedef enum 
{
	ERR_SECTION = 0,
	CFG_SECTION,
	CTR_SECTION,
	INFO_SECTION,
	MAX_SECTIONS
} SectionType;

/*
 * Define enum to identify logging type
 */
typedef enum e_LogType
{
	LOG_TYPE_SHARED_MEMORY = 1,
	LOG_TYPE_WRITE_TO_FILE
} LogType;

typedef struct 
{
	pthread_mutex_t lock;
	SINT32      writeIndex;
	SINT32      readIndex;
	TBOOL       looped;
	TBOOL       overWritten;
	UINT8*      buffer;
} CircularBuffer;

typedef struct
{
  UINT8*          partition;//Will hold the partion to which the logs have to read for logutil.
	CircularBuffer* errBuffer; // will hold all errors
	CircularBuffer* cfgBuffer; // will hold all configuration
	CircularBuffer* ctrBuffer; // will hold all counters
	CircularBuffer* infoBuffer; // will hold all other logs
} CircularBuffers;

ExeType getExeTypeByExeName(
		IO UINT8 exeName[]
		);

key_t getShmKeyByExeAndSectionTypes(
		IN ExeType exeType,
		IN SectionType sectionType
		);

SINT32 getSizeByExeAndSectionTypes(
		IN ExeType exeType,
		IN SectionType sectionType
		);

UINT8* getExeNameByExeType(
		IN ExeType exeType
		);

void *invokeShmSystemFunc(
		IN key_t  shmKey,
		IN SINT32 size 
		);

TBOOL attachToSharedMem(
		IN ExeType exeType,
		IO CircularBuffers* cBufs
		);

SINT32 writeToSharedMem(
		IN UINT8* logMsg, 
		IN SINT32 logLen, 
		IO CircularBuffers* cBufs,
		IN ExeType exeType,
		IN SectionType sectionType
		);

SINT32 insertToCircularBuffer(
		IN UINT8* logMsg, 
		IN SINT32 logLen, 
		IO CircularBuffer* cBuf,
		IN SINT32 CIRC_BUFFER_CAPACITY
		);

SINT32 circularBufImpl(
		IN UINT8* logMsg, 
		IN SINT32 logLen, 
		IO CircularBuffer* cBuf,
		IN SINT32 CIRC_BUFFER_CAPACITY
		);

SINT32 readFromCircularBuffer (
		IO CircularBuffer* cBuf,
		IO FILE* fp,
		IN SINT32 CIRC_BUFFER_CAPACITY
		);

void setBufferPointers(
		IO CircularBuffers* cBufs
		);

void initCircularBuffers(
		IN ExeType exeType,
		IO CircularBuffers* cBufs
		);

SINT32 detachFromShm(
		IN ExeType exeType,
		IO CircularBuffers* cBufs
		);

#endif // SHM_H
