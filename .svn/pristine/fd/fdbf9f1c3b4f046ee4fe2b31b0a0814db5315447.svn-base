//#include "../../common/binarystring.h"
#pragma pack(1)


#ifdef __MINDSPEED_RCT_CODEC__
	unsigned long GetEnumStr2ID(const char EnumList[][256], int ItemCount, const char *EnumItem)
	{
		for(int i=0; i<ItemCount; i++)
		{
			if(!_stricmp(EnumList[i], EnumItem)) 
				return i;
		}
		return 0xFFFFFFFF;
	}
	const char* GetEnumID2Str(const char EnumList[][256], int ItemCount, int ItemID)
	{
		if(ItemID >= ItemCount)
			return NULL;
		return EnumList[ItemID];
	}
#endif

//RRCConnectionRequest
typedef struct {
	unsigned char mmec;
	unsigned long mTMSI;

#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		pBS.GetByte();
		mmec = pBS.GetBltInBitString<unsigned char, 8>();
		pBS.GetByte();
		mTMSI = pBS.GetBltInBitString<unsigned long, 32>();
	}

	void Pack( CBinaryString &pBS )
	{
		pBS.AppendByte(0);
		pBS.AppendBltInBitString<8>(mmec);
		pBS.AppendByte(0);
		pBS.AppendBltInBitString<32>(mTMSI);
	}
#endif
}TsTMSI;

typedef union {
	TsTMSI Params;
	unsigned char Array[5];

#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		Params.Unpack(pBS);
	}

	void Pack( CBinaryString &pBS )
	{
		Params.Pack(pBS);
	}
#endif
}uTInitialUEIdentity;

typedef struct {
	uTInitialUEIdentity ueIdentity;
	unsigned long establishmentCause;
	unsigned char spare;

#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		const char eEstablishmentCause [8][256] = {{'e', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y'},
		{'h', 'i', 'g', 'h', 'P', 'r', 'i', 'o', 'r', 'i', 't', 'y', 'A', 'c', 'c', 'e', 's', 's'},
		{'m', 't', 'A', 'c', 'c', 'e', 's', 's'},
		{'m', 'o', 'S', 'i', 'g', 'n', 'a', 'l', 'l', 'i', 'n', 'g'},
		{'m', 'o', 'D', 'a', 't', 'a'},
		{'s', 'p', 'a', 'r', 'e', '3'},
		{'s', 'p', 'a', 'r', 'e', '2'},
		{'s', 'p', 'a', 'r', 'e', '1'}};

		const char *eCause;

		pBS.GetByte();
		ueIdentity.Unpack(pBS);

		pBS.GetByte();
		eCause = pBS.GetBltInString();
		establishmentCause = GetEnumStr2ID(eEstablishmentCause, 8, eCause);

		pBS.GetByte();
		spare = pBS.GetBltInBitString<unsigned char, 1>();
	}

	void Pack( CBinaryString &pBS )
	{
		const char eEstablishmentCause [8][256] = {{'e', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y'},
		{'h', 'i', 'g', 'h', 'P', 'r', 'i', 'o', 'r', 'i', 't', 'y', 'A', 'c', 'c', 'e', 's', 's'},
		{'m', 't', 'A', 'c', 'c', 'e', 's', 's'},
		{'m', 'o', 'S', 'i', 'g', 'n', 'a', 'l', 'l', 'i', 'n', 'g'},
		{'m', 'o', 'D', 'a', 't', 'a'},
		{'s', 'p', 'a', 'r', 'e', '3'},
		{'s', 'p', 'a', 'r', 'e', '2'},
		{'s', 'p', 'a', 'r', 'e', '1'}};

		const char *eCause;

		pBS.Clear();

		unsigned long nNameLength = strlen("lte_types.TRRCConnectionRequestR8IEs");
	
		pBS.AppendLong( nNameLength );
		pBS.AppendBytes( (unsigned char*)"lte_types.RRCConnectionRequestR8IEs", nNameLength );
		
		pBS.AppendByte(0);
		ueIdentity.Pack(pBS);

		pBS.AppendByte(0);
		eCause = GetEnumID2Str(eEstablishmentCause, 8, establishmentCause);
		pBS.AppendBltInEnum(eCause);

		pBS.AppendByte(0);
		pBS.AppendBltInBitString<1>(spare);
	}
#endif
}TRRCConnectionRequestR8IEs;

//RRCConnectionSetup
typedef struct {
	unsigned long snFieldLength;
	unsigned long tReordering;
#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		const char eSFN [2][256] = {"size5", "size10"};
		const char eReordering[32][256] = {"ms0", "ms5", "ms10", "ms15", "ms20", 
										 "ms25", "ms30", "ms35", "ms40", "ms45", 
										 "ms50", "ms55", "ms60", "ms65", "ms70",
										 "ms75", "ms80", "ms85", "ms90", "ms95", 
										 "ms100", "ms110", "ms120", "ms130", "ms140", 
										 "ms150", "ms160", "ms170",	"ms180", "ms190", 
										 "ms200", "spare1"};
		const char *strTmp;

		pBS.GetByte();
		strTmp = pBS.GetBltInString();
		snFieldLength = GetEnumStr2ID(eSFN, 2, strTmp);

		pBS.GetByte();
		strTmp = pBS.GetBltInString();
		tReordering = GetEnumStr2ID(eReordering, 32, strTmp);
	}
	void Pack( CBinaryString &pBS )
	{
		const char eSFN [2][256] = {"size5", "size10"};
		const char eReordering[32][256] = {"ms0", "ms5", "ms10", "ms15", "ms20", 
										 "ms25", "ms30", "ms35", "ms40", "ms45", 
										 "ms50", "ms55", "ms60", "ms65", "ms70",
										 "ms75", "ms80", "ms85", "ms90", "ms95", 
										 "ms100", "ms110", "ms120", "ms130", "ms140", 
										 "ms150", "ms160", "ms170",	"ms180", "ms190", 
										 "ms200", "spare1"};
		const char *strTmp;

		pBS.AppendByte(0);
		strTmp = GetEnumID2Str(eSFN, 2, snFieldLength);
		pBS.AppendBltInEnum(strTmp);

		pBS.AppendByte(0);
		strTmp = GetEnumID2Str(eReordering, 32, tReordering);
		pBS.AppendBltInEnum(strTmp);
	}
#endif
}TDLUMRLC;

typedef struct {
	unsigned long ulUMRLC;
	TDLUMRLC dlUMRLC;
#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		const char eSFN [2][256] = {"size5", "size10"};
		const char *strulUMRLC;

		pBS.GetByte();
		strulUMRLC = pBS.GetBltInString();
		ulUMRLC = GetEnumStr2ID(eSFN, 2, strulUMRLC);

		pBS.GetByte();
		dlUMRLC.Unpack(pBS);
	}
	void Pack( CBinaryString &pBS )
	{
		const char eSFN [2][256] = {"size5", "size10"};
		const char *strulUMRLC;

		pBS.AppendByte(0);
		strulUMRLC = GetEnumID2Str(eSFN, 2, ulUMRLC);
		pBS.AppendBltInEnum(strulUMRLC);

		pBS.AppendByte(0);
		dlUMRLC.Pack(pBS);
	}
#endif
}TRLCConfiguration;

typedef struct {
	unsigned char priority;
	unsigned long prioritizedBitRate;
	unsigned long bucketSizeDuration;
	unsigned char logicalChannelGroup;
#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		const char ePBR [16][256] = {"kBps0", "kBps8", "kBps16", "kBps32", "kBps64", 
									 "kBps128", "kBps256", "inf", "spare8", "spare7", 
									 "spare6", "spare5", "spare4", "spare3", "spare2", 
									 "spare1"};
		const char eBSD[8][256] = {"ms50", "ms100", "ms150", "ms300", "ms500", 
								   "ms1000", "spare2", "spare1"};
		const char *strTmp;

		pBS.GetByte();
		priority = pBS.GetBltInInteger();

		pBS.GetByte();
		strTmp = pBS.GetBltInString();
		prioritizedBitRate = GetEnumStr2ID(ePBR, 16, strTmp);

		pBS.GetByte();
		strTmp = pBS.GetBltInString();
		bucketSizeDuration = GetEnumStr2ID(eBSD, 8, strTmp);

		pBS.GetByte();
		logicalChannelGroup = pBS.GetBltInInteger();
	}
	void Pack( CBinaryString &pBS )
	{
		const char ePBR [16][256] = {"kBps0", "kBps8", "kBps16", "kBps32", "kBps64", 
									 "kBps128", "kBps256", "inf", "spare8", "spare7", 
									 "spare6", "spare5", "spare4", "spare3", "spare2", 
									 "spare1"};
		const char eBSD[8][256] = {"ms50", "ms100", "ms150", "ms300", "ms500", 
								   "ms1000", "spare2", "spare1"};
		const char *strTmp;

		pBS.AppendByte(0);
		pBS.AppendBltInInteger(priority);

		pBS.AppendByte(0);
		strTmp = GetEnumID2Str(ePBR, 16, prioritizedBitRate);
		pBS.AppendBltInEnum(strTmp);

		pBS.AppendByte(0);
		strTmp = GetEnumID2Str(eBSD, 8, bucketSizeDuration);
		pBS.AppendBltInEnum(strTmp);

		pBS.AppendByte(0);
		pBS.AppendBltInInteger(logicalChannelGroup);
	}
#endif
}TLogicalChannelConfig;

typedef struct {
	unsigned char srbIdentity;
	TRLCConfiguration explicitValue;
	TLogicalChannelConfig LCConfig;
#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		pBS.GetByte();
		srbIdentity = pBS.GetBltInInteger();
		pBS.GetByte();
		explicitValue.Unpack(pBS);
		pBS.GetByte();
		expValue.Unpack(pBS);
	}
	void Pack( CBinaryString &pBS )
	{
		pBS.AppendByte(0);
		pBS.AppendBltInInteger(srbIdentity);
		pBS.AppendByte(0);
		explicitValue.Pack(pBS);
		pBS.AppendByte(0);
		expValue.Pack(pBS);
	}
#endif
}TSRBToAddModifyList;

typedef struct {
	unsigned long timeAlignmentTimerDedicated;
#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		const char eTAT[8][256] = {"sf500", "sf750", "sf1280", "sf1920", "sf2560", 
								   "sf5120", "sf10240", "inf"};
		const char *strTmp;

		pBS.GetByte();
		pBS.GetByte();

		pBS.GetByte();
		strTmp = pBS.GetBltInString();
		timeAlignmentTimerDedicated = GetEnumStr2ID(eTAT, 8, strTmp);

		pBS.GetByte();
	}
	void Pack( CBinaryString &pBS )
	{
		const char eTAT[8][256] = {"sf500", "sf750", "sf1280", "sf1920", "sf2560", 
								   "sf5120", "sf10240", "inf"};
		const char *strTmp;

		pBS.AppendByte(1);
		pBS.AppendByte(1);

		pBS.AppendByte(0);
		strTmp = GetEnumID2Str(eTAT, 8, timeAlignmentTimerDedicated);
		pBS.AppendBltInEnum(strTmp);

		pBS.AppendByte(1);
	}
#endif
}TMACMainConfiguration;

typedef struct {
	unsigned long ackNackRepetition;
	unsigned long tddAckNackFeedbackMode;
#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		const char eANR[4][256] = {"n2", "n4", "n6", "spare1"};
		const char eTANFM[2][256] = {"bundling", "multiplexing"};
		const char *strTmp;

		pBS.GetByte();
		strTmp = pBS.GetBltInString();
		ackNackRepetition = GetEnumStr2ID(eANR, 4, strTmp);

		pBS.GetByte();
		strTmp = pBS.GetBltInString();
		tddAckNackFeedbackMode = GetEnumStr2ID(eTANFM, 2, strTmp);
	}
	void Pack( CBinaryString &pBS )
	{
		const char eANR[4][256] = {"n2", "n4", "n6", "spare1"};
		const char eTANFM[2][256] = {"bundling", "multiplexing"};
		const char *strTmp;

		pBS.AppendByte(0);
		strTmp = GetEnumID2Str(eANR, 4, ackNackRepetition);
		pBS.AppendBltInEnum(strTmp);

		pBS.AppendByte(0);
		strTmp = GetEnumID2Str(eTANFM, 2, tddAckNackFeedbackMode);
		pBS.AppendBltInEnum(strTmp);
	}
#endif
}TPUCCHConfigDedicated;

typedef struct {
	unsigned char deltaOffsetACKIndex;
	unsigned char deltaOffsetRIIndex;
	unsigned char deltaOffsetCQIIndex;
#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		pBS.GetByte();
		deltaOffsetACKIndex = pBS.GetBltInInteger();

		pBS.GetByte();
		deltaOffsetRIIndex = pBS.GetBltInInteger();

		pBS.GetByte();
		deltaOffsetCQIIndex = pBS.GetBltInInteger();
	}
	void Pack( CBinaryString &pBS )
	{
		pBS.AppendByte(0);
		pBS.AppendBltInInteger(deltaOffsetACKIndex);

		pBS.AppendByte(0);
		pBS.AppendBltInInteger(deltaOffsetRIIndex);

		pBS.AppendByte(0);
		pBS.AppendBltInInteger(deltaOffsetCQIIndex);
	}
#endif
}TPUSCHConfigDedicated;

typedef struct {
	char p0UePUSCH;
	unsigned long deltaMCSEnabled;
	unsigned char accumulationEnabled;
	char p0uePUCCH;
	unsigned char pSRSOffset;
#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		const char eDMSCE[2][256] = {"en0", "en1"};
		const char *strTmp;

		pBS.GetByte();
		p0UePUSCH = pBS.GetBltInInteger();

		pBS.GetByte();
		strTmp = pBS.GetBltInString();
		deltaMCSEnabled = GetEnumStr2ID(eDMSCE, 2, strTmp);

		pBS.GetByte();
		accumulationEnabled = pBS.GetByte();

		pBS.GetByte();
		p0uePUCCH = pBS.GetBltInInteger();

		pBS.GetByte();
		pSRSOffset = pBS.GetBltInInteger();
	}
	void Pack( CBinaryString &pBS )
	{
		const char eDMSCE[2][256] = {"en0", "en1"};
		const char *strTmp;

		pBS.AppendByte(0);
		pBS.AppendBltInInteger(p0UePUSCH);

		pBS.AppendByte(0);
		strTmp = GetEnumID2Str(eDMSCE, 2, deltaMCSEnabled);
		pBS.AppendBltInEnum(strTmp);

		pBS.AppendByte(0);
		pBS.AppendBltInBoolean(accumulationEnabled ? true : false );

		pBS.AppendByte(0);
		pBS.AppendBltInInteger(p0uePUCCH);

		pBS.AppendByte(0);
		pBS.AppendBltInInteger(pSRSOffset);
	}
#endif
}TUplinkPowerControlDedicated;

typedef struct {
	unsigned long transmissionMode;
	unsigned long ueTransmitAntennaSelection;
#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		const char eTM[8][256] = {"tm1", "tm2", "tm3", "tm4", "tm5", 
								  "tm6", "tm7", "spare1"};
		const char eUTAS[2][256] = {"closedLoop", "openLoop"};
		const char *strTmp;

		pBS.GetByte();
		strTmp = pBS.GetBltInString();
		transmissionMode = GetEnumStr2ID(eTM, 8, strTmp);

		pBS.GetByte();

		pBS.GetByte();
		strTmp = pBS.GetBltInString();
		ueTransmitAntennaSelection = GetEnumStr2ID(eUTAS, 2, strTmp);
	}
	void Pack( CBinaryString &pBS )
	{
		const char eTM[8][256] = {"tm1", "tm2", "tm3", "tm4", "tm5", 
								  "tm6", "tm7", "spare1"};
		const char eUTAS[2][256] = {"closedLoop", "openLoop"};
		const char *strTmp;

		pBS.AppendByte(0);
		strTmp = GetEnumID2Str(eTM, 8, transmissionMode);
		pBS.AppendBltInEnum(strTmp);

		pBS.AppendByte(1);

		pBS.AppendByte(0);
		strTmp = GetEnumID2Str(eUTAS, 2, ueTransmitAntennaSelection);
		pBS.AppendBltInEnum(strTmp);
	}
#endif
}TAntennaInformationDedicated;

typedef struct {
	unsigned long pdschConfiguration;
	TPUCCHConfigDedicated pucchConfiguration;
	TPUSCHConfigDedicated puschConfiguration;
	TUplinkPowerControlDedicated uplinkPowerControl;
	TAntennaInformationDedicated explicitValue;
#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		const char eConf[8][256] = {"dBmin6", "dBmin4dot77", "dBmin3", "dBmin1dot77", 
								   "dB0", "dB1", "dB2", "dB3"};
		const char *strTmp;

		pBS.GetByte();
		strTmp = pBS.GetBltInString();
		pdschConfiguration = GetEnumStr2ID(eConf, 8, strTmp);

		pBS.GetByte();
		pucchConfiguration.Unpack(pBS);
		
		pBS.GetByte();
		puschConfiguration.Unpack(pBS);

		pBS.GetByte();
		uplinkPowerControl.Unpack(pBS);

		pBS.GetByte();
		pBS.GetByte();
		pBS.GetByte();
		pBS.GetByte();

		pBS.GetByte();
		explicitValue.Unpack(pBS);

		pBS.GetByte();
	}
	void Pack( CBinaryString &pBS )
	{
		const char eConf[8][256] = {"dBmin6", "dBmin4dot77", "dBmin3", "dBmin1dot77", 
								   "dB0", "dB1", "dB2", "dB3"};
		const char *strTmp;
		
		pBS.AppendByte(0);
		strTmp = GetEnumID2Str(eConf, 8, pdschConfiguration);
		pBS.AppendBltInEnum(strTmp);

		pBS.AppendByte(0);
		pucchConfiguration.Pack(pBS);

		pBS.AppendByte(0);
		puschConfiguration.Pack(pBS);

		pBS.AppendByte(0);
		uplinkPowerControl.Pack(pBS);

		pBS.AppendByte(1);
		pBS.AppendByte(1);
		pBS.AppendByte(1);
		pBS.AppendByte(1);

		pBS.AppendByte(0);
		explicitValue.Pack(pBS);

		pBS.AppendByte(1);
	}
#endif
}TPhysicalConfigDedicated;

typedef struct {
	TSRBToAddModifyList srbToAddModifyList;
	TMACMainConfiguration explicitValue;
	TPhysicalConfigDedicated physicalConfigDedicated;
#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		pBS.GetByte();
		srbToAddModifyList.Unpack(pBS);

		pBS.GetByte();
		pBS.GetByte();

		pBS.GetByte();
		explicitValue.Unpack(pBS);

		pBS.GetByte();

		pBS.GetByte();
		physicalConfigDedicated.Unpack(pBS);
	}
	void Pack( CBinaryString &pBS )
	{
		pBS.AppendByte(0);
		srbToAddModifyList.Pack(pBS);

		pBS.AppendByte(1);
		pBS.AppendByte(1);

		pBS.AppendByte(0);
		explicitValue.Pack(pBS);

		pBS.AppendByte(1);

		pBS.AppendByte(0);
		physicalConfigDedicated.Pack(pBS);
	}
#endif
}TRRCConnectionSetupR8IEs;

typedef struct {
	unsigned char rrcTransactionIdentifier;
	TRRCConnectionSetupR8IEs rrcConnectionSetupR8;
#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		pBS.GetByte();
		rrcTransactionIdentifier = pBS.GetBltInInteger();
		pBS.GetByte();
		rrcConnectionSetupR8.Unpack(pBS);
	}
	void Pack( CBinaryString &pBS )
	{
		pBS.AppendLong(strlen("lte_types.RRCConnectionSetup"));
		pBS.AppendBytes((unsigned char*)"lte_types.RRCConnectionSetup", strlen("lte_types.RRCConnectionSetup"));
		pBS.AppendByte(0);
		pBS.AppendBltInInteger(rrcTransactionIdentifier);
		pBS.AppendByte(0);
		rrcConnectionSetupR8.Pack(pBS);
	}
#endif
}TRRCConnectionSetup;

//RRCConnectionSetupComplete
typedef struct {
	unsigned char mcc[3];
	unsigned char mnc[2];
#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		pBS.GetByte();
		pBS.GetLong();
		mcc[0] = pBS.GetBltInInteger();
		mcc[1] = pBS.GetBltInInteger();
		mcc[2] = pBS.GetBltInInteger();

		pBS.GetByte();
		pBS.GetLong();
		mnc[0] = pBS.GetBltInInteger();
		mnc[1] = pBS.GetBltInInteger();
	}
	void Pack( CBinaryString &pBS )
	{
		pBS.AppendByte(0);
		pBS.AppendLong(3);
		pBS.AppendBltInInteger(mcc[0]);
		pBS.AppendBltInInteger(mcc[1]);
		pBS.AppendBltInInteger(mcc[2]);

		pBS.AppendByte(0);
		pBS.AppendLong(2);
		pBS.AppendBltInInteger(mnc[0]);
		pBS.AppendBltInInteger(mnc[1]);
	}
#endif
}TPLMNIdentity;

typedef struct {
	TPLMNIdentity plmnIdentity;
	unsigned short mmegi;
	unsigned char mmec;
#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		pBS.GetByte();
		plmnIdentity.Unpack(pBS);
		pBS.GetByte();
		mmegi = pBS.GetBltInBitString<unsigned short, 16>();
		pBS.GetByte();
		mmec = pBS.GetBltInBitString<unsigned char, 8>();
	}
	void Pack( CBinaryString &pBS )
	{
		pBS.AppendByte(0);
		plmnIdentity.Pack(pBS);
		pBS.AppendByte(0);
		pBS.AppendBltInBitString<16>(mmegi);
		pBS.AppendByte(0);
		pBS.AppendBltInBitString<8>(mmec);
	}
#endif
}TRegisteredMME;

typedef struct {
	unsigned char selectedPLMNIdentity;
	TRegisteredMME registeredMME;
	unsigned char nasDedicatedInformation[4];
#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		pBS.GetByte();
		selectedPLMNIdentity = pBS.GetBltInInteger();
		pBS.GetByte();
		registeredMME.Unpack(pBS);
		pBS.GetByte();
		const unsigned char *strTmp = pBS.GetBltInOctetString();
		memcpy_s(nasDedicatedInformation, 4, strTmp, 4);
	}
	void Pack( CBinaryString &pBS )
	{
		pBS.AppendByte(0);
		pBS.AppendBltInInteger(selectedPLMNIdentity);
		pBS.AppendByte(0);
		registeredMME.Pack(pBS);
		pBS.AppendByte(0);
		pBS.AppendBltInOctetString(nasDedicatedInformation, 4);
	}
#endif
}TRRCConnectionSetupCompleter8IEs;

typedef struct {
	unsigned char rrcTransactionIdentifier;
	TRRCConnectionSetupCompleter8IEs rrcConnectionSetupCompleter8;
#ifdef __MINDSPEED_RCT_CODEC__
	void Unpack( CBinaryString &pBS )
	{
		pBS.GetByte();
		rrcTransactionIdentifier = pBS.GetBltInInteger();
		pBS.GetByte();
		rrcConnectionSetupCompleter8.Unpack(pBS);
	}
	void Pack( CBinaryString &pBS )
	{
		pBS.AppendLong(strlen("lte_types.RRCConnectionSetupComplete"));
		pBS.AppendBytes((unsigned char*)"lte_types.RRCConnectionSetupComplete", strlen("lte_types.RRCConnectionSetupComplete"));
		pBS.AppendByte(0);
		pBS.AppendBltInInteger(rrcTransactionIdentifier);
		pBS.AppendByte(0);
		rrcConnectionSetupCompleter8.Pack(pBS);		
	}
#endif
}TRRCConnectionSetupComplete;
#pragma pack()
