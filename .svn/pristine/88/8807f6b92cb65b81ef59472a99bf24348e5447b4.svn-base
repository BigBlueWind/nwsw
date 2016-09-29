/**
 * @file    imc_shm.c
 * @brief   This file contains common functions for attaching to shared memory
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @author  Thirumal Ravula
 *
 * @details Initial version
 */

#include "shm.h"

UINT8* exeShmkeyMap_g[MAX_EXE+1][NUM_COLS] = {
	/* K=Kilo bytes, M=Megabytes */
	/*{exeType, exename,   errkey,   errsz(K),  cfgkey(K),  cfgsz(K),  ctrkey(K), ctrsz(K),  infokey(K), infosize(M)}*/
	{"0",   "bsm.exe",     "1001",    "1000",   "1002",     "1000",    "1003",     "1",       "1004",      "10"},
	{"1",   "cih1.exe",    "1005",    "1",      "1006",     "1",       "1007",     "1",       "1008",       "1"},
	{"2",   "cih2.exe",    "1009",    "1",      "1010",     "1",       "1011",     "1",       "1012",       "1"},
	{"3",   "cih3.exe",    "1013",    "1",      "1014",     "1",       "1015",     "1",       "1016",       "1"},
	{"4",   "deh.exe",     "1017",    "1",      "1018",     "1",       "1019",     "1",       "1020",       "2"},
	{"5",   "gtp.exe",     "1021",    "1",      "1022",     "1",       "1023",     "10",      "1024",       "1"},
	{"6",   "ems_sim.exe", "1025",    "1",      "1026",     "1",       "1027",     "1",       "1028",       "1"},
	{"7",   "fth.exe",     "1029",    "1",      "1030",     "1",       "1031",     "1",       "1032",       "1"},
	{"8",   "lth.exe",     "1033",    "1",      "1034",     "1",       "1035",     "1",       "1036",       "1"},
	{"9",   "pmh.exe",     "1037",    "1",      "1038",     "1",       "1039",     "50",      "1040",       "1"},
	{"10",  "prm.exe",     "1041",    "1",      "1042",     "1",       "1043",     "1",       "1044",       "1"},
	{"11",  "rh_sim.exe",  "1045",    "1",      "1046",     "1",       "1047",     "1",       "1048",       "1"},
	{"12",  "cp.exe",      "1049",    "1000",   "1050",     "2000",    "1051",     "10",      "1052",      "10"},
	{"13",  "sih.exe",     "1053",    "1",      "1054",     "1",       "1055",     "1",       "1056",       "1"},
	{"14",  "wih.exe",     "1057",    "1",      "1058",     "1",       "1059",     "1",       "1060",       "1"},
	{"15",  "lk_sim.exe",  "1061",    "1",      "1062",     "1",       "1063",     "1",       "1064",       "1"},
#ifdef PWAV_L2SIM
  {"16", "l2sim.exe",    "1065",    "1",      "1066",     "1",       "1067",     "1",       "1068",       "1"},
	{"17",  "NONE",        "9999",    "9999",   "9999",     "9999",    "9999",     "9999",    "9999",  "999999"},
#else
  {"16",  "NONE",        "9999",    "9999",   "9999",     "9999",    "9999",     "9999",    "9999",  "999999"},
#endif
};

/**
 * @param exeName Executable name
 * @details This functions returns executable type based on executable string
 */
ExeType getExeTypeByExeName(
		IO UINT8 exeName[]
		)
{
	ExeType exeType;
	for(exeType = BSM_EXE; exeType < MAX_EXE; exeType++)
	{
		if(!strncmp(exeShmkeyMap_g[exeType][COL_EXENAME], exeName, strlen(exeShmkeyMap_g[exeType][COL_EXENAME])))
		{
			return exeType; 
		}
	}
	fprintf(stderr, "%s", "Incorrect exe name\n");
	exit(EXIT_SUCCESS);
}

/**
 * @param exeType Executable Type
 * @param	sectionType SectionType 
 * @details This function returns shared memory key based on executable type & section type
 */
key_t getShmKeyByExeAndSectionTypes(
		IN ExeType exeType,
		IN SectionType sectionType
		)
{
	key_t shmKey = 0;
	SINT32 keyColumn = -1;

	switch(sectionType)
	{
		case ERR_SECTION:
			keyColumn = COL_ERRKEY;
			break;
		case CFG_SECTION:
			keyColumn = COL_CFGKEY;
			break;
		case CTR_SECTION:
			keyColumn = COL_CTRKEY;
			break;
		case INFO_SECTION:
			keyColumn = COL_INFOKEY;
			break;
		default:
			fprintf(stderr, "F(getShmKeyByExeAndSectionTypes): Wrong section Type = %d\n", sectionType);
			break;
	}

	if (((SINT32)exeType >= BSM_EXE && exeType < MAX_EXE) && 
			keyColumn NOTEQ -1)
	{
		shmKey = atoi(exeShmkeyMap_g[exeType][keyColumn]);
	}
	else
	{
		fprintf(stderr, "F(getShmKeyByExeAndSectionTypes): This exe %d doesn't exist or columnType %d is wrong\n",exeType, keyColumn);
	}
	return shmKey;
}

/**
 * @param exeType Executable Type
 * @param	sectionType SectionType 
 * @details This function returns shared memory size based on executable type & sectiontype
 */
SINT32 getSizeByExeAndSectionTypes(
		IN ExeType exeType,
		IN SectionType sectionType
		)
{
	SINT32 size = 0;
	SINT32 multiplier = 0;
	SINT32 keyColumn = -1;

	switch(sectionType)
	{
		case ERR_SECTION:
			keyColumn = COL_ERRSZ;
			multiplier = 1024;
			break;
		case CFG_SECTION:
			keyColumn = COL_CFGSZ;
			multiplier = 1024;
			break;
		case CTR_SECTION:
			keyColumn = COL_CTRSZ;
			multiplier = 1024;
			break;
		case INFO_SECTION:
			keyColumn = COL_INFOSZ;
			multiplier = 1024*1024;
			break;
		default:
			fprintf(stderr, "F(getSizeByExeAndSectionTypes): Wrong section Type = %d\n", sectionType);
			break;
	}

	if (((SINT32)exeType >= BSM_EXE && exeType < MAX_EXE) && 
			keyColumn NOTEQ -1)
	{
		size = atoi(exeShmkeyMap_g[exeType][keyColumn]);
		if (size EQ 1000)
		{
			size = 1024;
		}
		size = size * multiplier;
	}
	else
	{
		fprintf(stderr, "F(getSizeByExeAndSectionTypes): This exe %d doesn't exist or columnType %d is wrong\n",exeType, keyColumn);
	}
	return size;
}

/**
 * @param exeType Executable Type
 * @details This function returns exe name based on executable type
 */
UINT8* getExeNameByExeType(
		IN ExeType exeType
		)
{
	UINT8* exeName = NULL;
	if ((SINT32)exeType >= BSM_EXE && exeType < MAX_EXE)
	{
		exeName = exeShmkeyMap_g[exeType][COL_EXENAME];
	}
	else
	{
		fprintf(stderr, "This exe %d doesn't exist\n",exeType);
	}
	return exeName;
}

/**
 * @param size SINT32 Type
 @param shmKey key_t type
 * @details This function returns either TRUE or FALSE based on the success of allocation of the shated memory
 */
void *invokeShmSystemFunc(
		IN key_t  shmKey,
		IN SINT32 size 
		)
{
	SINT32 shmId;

	if((shmId = shmget(shmKey, size, IPC_CREAT | 0660)) NOTEQ -1)
	{
		return shmat(shmId, NULL, 0);
	}
	else
	{
		fprintf(stderr, "shmget() failed for shmKey=%d ... Exiting\n", shmKey);
		return (void *)-1;
	}
}

/**
 * @param exeType ExeType Type
 * @param cBufs   CircularBufs* Type
 * @details This function returns either TRUE or FALSE based on the success of allocation of the shated memory
 */
TBOOL attachToSharedMem (
		IN ExeType exeType,
		IO CircularBuffers* cBufs
		)
{

	TBOOL retVal = TRUE;
	int size = 0;
	CircularBuffer* holder = NULL;
	key_t shmKey = 0;

	/*
	 * Allocate shared memory for each section to be used for logging for a process 
	 */
	if (cBufs NOTEQ NULL)
	{
		if ( (shmKey = getShmKeyByExeAndSectionTypes(MAX_EXE-1, MAX_SECTIONS-1)) EQ 0)
		{
			retVal = FALSE;
		}
		shmKey += 1; // This key is one greater than last key in the table exeShmkeyMap_g
		if (retVal && ((cBufs->partition = (UINT8*)invokeShmSystemFunc(shmKey, MAX_LOG_FILENAME)) EQ ((void*)-1)) ) 
		{
			fprintf(stderr, "shmat() failed for Partition with shmKey=%d ... Exiting\n", shmKey);
			retVal = FALSE;
		}

		SectionType sectionType = ERR_SECTION;
		while (sectionType < MAX_SECTIONS && retVal)
		{
			if ( (shmKey = getShmKeyByExeAndSectionTypes(exeType, sectionType)) EQ 0)
			{
				retVal = FALSE;
				break;
			}
			size = sizeof(CircularBuffer) + getSizeByExeAndSectionTypes(exeType, sectionType);
			if((holder = (CircularBuffer*)invokeShmSystemFunc(shmKey, size)) EQ ((void*)-1))
			{
				fprintf(stderr, "shmat() failed for shmKey=%d and section = %d ... Exiting\n", shmKey, sectionType);
				retVal = FALSE;
				break;
			}
			else
			{
				pthread_mutexattr_t pshared; 
				pthread_mutexattr_init(&pshared);
				pthread_mutexattr_setpshared(&pshared, PTHREAD_PROCESS_SHARED);
				pthread_mutex_init(&holder->lock, &pshared);
			}
			switch(sectionType)
			{
				case ERR_SECTION:
					cBufs->errBuffer = holder;
					break;
				case CFG_SECTION:
					cBufs->cfgBuffer = holder;
					break;
				case CTR_SECTION:
					cBufs->ctrBuffer = holder;
					break;
				case INFO_SECTION:
					cBufs->infoBuffer = holder;
					break;
				default:
					fprintf(stderr, "No section to attach to: sectionType = %d\n", sectionType);
					retVal = FALSE;
					break;
			}
			sectionType++;
		}
	}
	return retVal;
}

/**
 * @param logMsg       Message to be written
 * @param logLen       length of Message
 * @param cBufs*       CircularBuffers
 * @param exeType      Executable Type
 * @param	sectionType  SectionType 
 * @details This function will set up the pointer for the buffer to which the logmsg will be written.
 */
SINT32 writeToSharedMem(
		IN UINT8* logMsg, 
		IN SINT32 logLen, 
		IO CircularBuffers* cBufs,
		IN ExeType exeType,
		IN SectionType sectionType
		)
{
	CircularBuffer* cBuf = NULL;
	SINT32 bufLen = 0;
	SINT32 retVal = 0;

	if (cBufs NOTEQ NULL)
	{
		switch(sectionType)
		{
			case ERR_SECTION:
				cBuf = cBufs->errBuffer;
				break;
			case CTR_SECTION:
				cBuf = cBufs->ctrBuffer;
				break;
			case CFG_SECTION:
				cBuf = cBufs->cfgBuffer;
				break;
			case INFO_SECTION:
				cBuf = cBufs->infoBuffer;
				break;
			default:
				return 0;
		}
		bufLen = getSizeByExeAndSectionTypes(exeType, sectionType);
		retVal = insertToCircularBuffer(logMsg, logLen, cBuf, bufLen);
	}
	return retVal;
}

/*
	 insertToCircularBuffer implementation

	 writeIndex = the point after the last write.
	 readIndex  = the point after the last read.
	 logLen     = size of the source message.
	 logMsg     = source buffer.

	 Assumptions;
	 (i) When readIndex and writeIndex are equal, there is no more data to be read.
	 (ii)Payload size cannot be greater than total circular buffer size.

	 Normal Cases:
	 Case(i): writeIndex > readIndex is true before AND after the write
	 AND no looping occurs. (T T F)
	 Case(ii): writeIndex > readIndex is true before write AND false after write 
	 AND looping occurs. (T F T)
	 Case(iii): writeIndex < readIndex is true before AND after the write 
	 AND no looping occurs. (F F F)

	 OverWrite cases:
	 Case(i): writeIndex > readIndex is true before AND after the write 
	 AND looping occurs. (T T T)
	 Case(ii): writeIndex < readIndex is true before the write AND false after the 
	 write AND no looping occurs. (F T F)
	 Case(iii): writeIndex < readIndex is true before AND after the write 
	 AND looping occurs. (F F T)

	 The other two cases (F T T) -- payload greater than buffer size and (T F F) -- reading beyond valid data -- will never occur.

	 For the above overwrite conditions, 'OVERWRITE' is written to the buffer after
	 the payload is written and readIndex is set to writeIndex.
 */

/**
 * @param logMsg       Message to be written
 * @param logLen       length of Message
 * @param cBuf         CircularBuffer
 * @details This function will determine whether overwrite will occur and
 * based on which it will invoke another function to write toi the 
 * circular buffer 
 */
SINT32 insertToCircularBuffer(
		IN UINT8* logMsg, 
		IN SINT32 logLen, 
		IO CircularBuffer* cBuf,
		IN SINT32 CIRC_BUFFER_CAPACITY
		)
{
	TBOOL writeGreaterThanReadBefore = FALSE;
	TBOOL writeGreaterThanReadAfter  = FALSE;
	TBOOL alreadyLooped = FALSE;
	TBOOL looped = FALSE;
	TBOOL overWritten = FALSE;
	SINT32 newWriteIndex = 0; 
	SINT32 retVal = 0;

	if (cBuf NOTEQ NULL)
	{
		if(logLen > CIRC_BUFFER_CAPACITY)
		{
			fprintf(stderr, "F(insertToCircularBuffer): The length %d of the payload is greater than capacity %d of CIRCULAR BUFFer\n",logLen, CIRC_BUFFER_CAPACITY);
		}
		else
		{
			if (pthread_mutex_trylock(&(cBuf->lock)) EQ 0)
			{
				cBuf->buffer = (UINT8 *)(cBuf + sizeof(CircularBuffer));

				alreadyLooped = cBuf->looped;
				if (cBuf->writeIndex >= cBuf->readIndex) writeGreaterThanReadBefore = TRUE;
				if (logLen >= (CIRC_BUFFER_CAPACITY - cBuf->writeIndex)) looped = TRUE;
				if (looped) 
				{
					newWriteIndex = (logLen - (CIRC_BUFFER_CAPACITY - cBuf->writeIndex));
					cBuf->looped = TRUE;
				}
				else
				{
					newWriteIndex = cBuf->writeIndex + logLen;
				}
				if (newWriteIndex > cBuf->readIndex) writeGreaterThanReadAfter = TRUE;
				if ((writeGreaterThanReadBefore  &&  writeGreaterThanReadAfter &&  looped) || /* ( T T T) */ 
						(!writeGreaterThanReadBefore &&  writeGreaterThanReadAfter && !looped) || /* ( F T F) */
						(!writeGreaterThanReadBefore && !writeGreaterThanReadAfter &&  looped) || /* ( F F T) */
						(!writeGreaterThanReadBefore &&  writeGreaterThanReadAfter &&  looped) || /* Case (F T T), when payload is greater that buffer size */
						(cBuf->writeIndex EQ cBuf->readIndex && alreadyLooped)) /* handle special case */ 
				{
					cBuf->overWritten = TRUE;
					/* A dynamic variable is also required because cBuf->overwritten is cleared after a read */
					overWritten = TRUE; 
				}
				/* Now write the data to the curcular buffer */ 
				retVal = circularBufImpl(logMsg, logLen, cBuf, CIRC_BUFFER_CAPACITY);
				if (overWritten)
				{
					retVal += circularBufImpl("OVERWRITTEN", strlen("OVERWRITTEN"), cBuf, CIRC_BUFFER_CAPACITY);
				}
				pthread_mutex_unlock(&cBuf->lock);
			} 
			else
			{
				//fprintf(stderr, "F(insertToCircularBuffer):Could not get lock\n");
			}
		}
	}
	else
	{
		fprintf(stderr, "F(insertToCircularBuffer):Undefined circularBuffer\n");
	}
	return retVal;
}

/**
 * @param logMsg       Message to be written
 * @param logLen       length of Message
 * @param cBuf         CircularBuffer
 * @details This function will write data to the circular buffer.
 */
SINT32 circularBufImpl(
		IN UINT8* logMsg,
		IN SINT32 logLen,
		IO CircularBuffer* cBuf,
		IN SINT32 CIRC_BUFFER_CAPACITY
		)
{
	SINT32 remainingBytes = logLen;
	SINT32 writtenBytes  = 0;

	if (cBuf NOTEQ NULL)
	{
		/* Check if the payload fits till the end and if true write it */
		if ((logLen <= (CIRC_BUFFER_CAPACITY - cBuf->writeIndex)))
		{
			memcpy(&(cBuf->buffer[cBuf->writeIndex]),
					logMsg,
					logLen);
			writtenBytes += logLen;
			remainingBytes -= writtenBytes;
			if(logLen EQ (CIRC_BUFFER_CAPACITY - cBuf->writeIndex)) /* exact fit in he buffer */
			{
				cBuf->writeIndex = 0;
			}
			else
			{
				cBuf->writeIndex += writtenBytes;
			}
		}
		/* else write the part at the end and the remaining part at the front */
		else
		{
			memcpy(&(cBuf->buffer[cBuf->writeIndex]),
					logMsg,
					CIRC_BUFFER_CAPACITY - cBuf->writeIndex);

			writtenBytes = (CIRC_BUFFER_CAPACITY - cBuf->writeIndex); 
			remainingBytes -= writtenBytes;

			memcpy(&(cBuf->buffer[0]),
					&logMsg[writtenBytes],
					remainingBytes);

			cBuf->writeIndex = remainingBytes; /* set the writeIndex */
			writtenBytes   += remainingBytes;
			remainingBytes -= writtenBytes;
		}
	}
	return writtenBytes;
}

/**
 * @param cBuf         CircularBuffer
 * @param fp           file pointer 
 * @details This function will read data from the circular buffer.
 */
SINT32 readFromCircularBuffer (
		IO CircularBuffer* cBuf,
		IO FILE* fp,
		IN SINT32 CIRC_BUFFER_CAPACITY
		)
{
	SINT32 retVal = 0;
	UINT16 startIndex = 0;

	if (cBuf NOTEQ NULL)
	{
		if (pthread_mutex_lock(&(cBuf->lock)) EQ 0)
		{
			/* The pointer for buffer where the log is written/read can only be set after acquiring the lock
			 * otherwise, it gives a wrong address if a read/write operation is taking place at this address
			 */
			cBuf->buffer = (UINT8 *)(cBuf + sizeof(CircularBuffer));
			if (cBuf->overWritten) /* Where data is overwritten */
			{
				if (cBuf->writeIndex > cBuf->readIndex)
					startIndex = cBuf->writeIndex;
				else
					startIndex = cBuf->readIndex;
				
				retVal = fwrite(&cBuf->buffer[startIndex],
						1,
						CIRC_BUFFER_CAPACITY - startIndex,
						fp);
				retVal += fwrite(&cBuf->buffer[0],
						1,
						cBuf->writeIndex,
						fp);
			}
			/* Special case where writeIndex and readIndex are equal but there is data to be read */
			else if (!cBuf->overWritten && (cBuf->readIndex EQ cBuf->writeIndex) && cBuf->looped)
			{
				retVal = fwrite(&cBuf->buffer[cBuf->readIndex],
						1,
						CIRC_BUFFER_CAPACITY - cBuf->readIndex,
						fp);
				retVal += fwrite(&cBuf->buffer[0],
						1,
						cBuf->readIndex,
						fp);
			}
			else /* NORMAL CASES */
			{
				/* Case(i), read Index is less than write Index */
				if (cBuf->readIndex < cBuf->writeIndex) {
					retVal = fwrite(&cBuf->buffer[cBuf->readIndex],
							1,
							cBuf->writeIndex - cBuf->readIndex,
							fp);
				} else if (cBuf->readIndex > cBuf->writeIndex) {
					/* Case(ii) */
					/* read from readIndex to the end */
					retVal = fwrite(&cBuf->buffer[cBuf->readIndex],
							1,
							CIRC_BUFFER_CAPACITY - cBuf->readIndex,
							fp);
					/* read from start to writeIndex */
					retVal += fwrite(&cBuf->buffer[0],
							1,
							cBuf->writeIndex,
							fp);
				}
			}
			/* All data written was read*/
			cBuf->readIndex   = cBuf->writeIndex;
			cBuf->overWritten = FALSE;
			cBuf->looped      = FALSE;
			if ( pthread_mutex_unlock(&(cBuf->lock)) NOTEQ 0)
			{
				//fprintf(stderr, "F(readFromCircularBuffer): Cannot unlock\n");
			}
		}
		else
		{
			//fprintf(stderr, "F(readFromCircularBuffer): Cannot acquire lock\n");
		}
	}
	return retVal;
}

/**
 * @param cBufs         CircularBuffers
 * @param exeType       ExeType
 * @details: This function will initialize all the buffers to 0. 
 */
void initCircularBuffers(
		IN ExeType exeType, 
		IO CircularBuffers* cBufs)
{
	SINT32 size = 0;
	CircularBuffer* holder = NULL;

	if (cBufs NOTEQ NULL)
	{
		clearBytes(cBufs->partition, MAX_LOG_FILENAME);
		SectionType sectionType = 0;
		while (sectionType < MAX_SECTIONS)
		{
			size = getSizeByExeAndSectionTypes(exeType, sectionType);
			switch(sectionType)
			{
				case ERR_SECTION:
					holder = cBufs->errBuffer;
					break;
				case CFG_SECTION:
					holder = cBufs->cfgBuffer;
					break;
				case CTR_SECTION:
					holder = cBufs->ctrBuffer;
					break;
				case INFO_SECTION:
					holder = cBufs->infoBuffer;
					break;
				default:
					holder = NULL;
					break;
			}
			if (holder NOTEQ NULL)
			{
				clearBytes(holder, sizeof(CircularBuffer));
				holder->buffer = (UINT8 *)(holder + sizeof(CircularBuffer));
				clearBytes(holder->buffer,size); 
			}
			sectionType++;
		}
	}
}

/**
 * @param exeType       ExeType
 * @param cBufs         CircularBuffers
 * @details This function will set the pointers to the buffers that hold the logs.
 */
void setBufferPointers(
		IO CircularBuffers* cBufs)
{
	SectionType sectionType = ERR_SECTION;
	while (sectionType < MAX_SECTIONS)
	{
		switch(sectionType)
		{
			case ERR_SECTION:
				cBufs->errBuffer->buffer  = (UINT8*)(cBufs->errBuffer + sizeof(CircularBuffer));
				break;
			case CFG_SECTION:
				cBufs->cfgBuffer->buffer  = (UINT8*)(cBufs->cfgBuffer + sizeof(CircularBuffer));
				break;
			case CTR_SECTION:
				cBufs->ctrBuffer->buffer  = (UINT8*)(cBufs->ctrBuffer + sizeof(CircularBuffer));
				break;
			case INFO_SECTION:
				cBufs->infoBuffer->buffer = (UINT8*)(cBufs->infoBuffer + sizeof(CircularBuffer));
				break;
			default:
				break;
		}
		sectionType++;
	}
}

/**
 * @param exeType ExeType Type
 * @param cBufs   CircularBufs* Type
 * @details: This function will detach the shared memory.. 
 */
SINT32 detachFromShm(
		IN ExeType exeType,
		IO CircularBuffers* cBufs)
{
	void *ptr;
	key_t shmKey = 0;
	SINT32 size = 0;
	SINT32 retVal = -1;
	SectionType sectionType;

	sectionType = ERR_SECTION;
	while (sectionType < MAX_SECTIONS && retVal)
	{
		if ( (shmKey = getShmKeyByExeAndSectionTypes(exeType, sectionType)) EQ 0)
		{
			retVal = FALSE;
			break;
		}
		size = sizeof(CircularBuffer) + getSizeByExeAndSectionTypes(exeType, sectionType);
		ptr  = invokeShmSystemFunc(shmKey, size) ;
		if ( ptr NOTEQ (void *)-1)
		{
			retVal = shmdt(ptr);
		}
		else
		{
			fprintf(stderr, "shmat() failed for shmKey=%d\n", shmKey);
			retVal = -1;
		}
		sectionType++;
	}
	clearBytes(cBufs, sizeof(CircularBuffers));
	return retVal;
}
