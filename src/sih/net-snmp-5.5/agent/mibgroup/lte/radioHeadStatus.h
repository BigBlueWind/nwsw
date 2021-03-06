/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 19302 2010-08-13 12:19:42Z dts12 $
 */
#ifndef RADIOHEADSTATUS_H
#define RADIOHEADSTATUS_H

/* function declarations */
void init_radioHeadStatus(void);
void initialize_table_radioHeadStatusTable(void);
Netsnmp_Node_Handler radioHeadStatusTable_handler;
Netsnmp_First_Data_Point  radioHeadStatusTable_get_first_data_point;
Netsnmp_Next_Data_Point   radioHeadStatusTable_get_next_data_point;

/* column number definitions for table radioHeadStatusTable */
       #define COLUMN_RADIO_HEAD_STATUS_INDEX		1
       #define COLUMN_C1IQMAP		2
       #define COLUMN_C2IQMAP		3
       #define COLUMN_C3IQMAP		4
       #define COLUMN_C4IQMAP		5
       #define COLUMN_FWMODE		6
       #define COLUMN_NUMBEROFFA		7
       #define COLUMN_MAPPINGOF1FASTATUS		8
       #define COLUMN_MAPPINGOF2FASTATUS		9
       #define COLUMN_MAPPINGOF3FASTATUS		10
       #define COLUMN_MAPPINGOF4FASTATUS		11
       #define COLUMN_CVC1		12
       #define COLUMN_CVC2		13
       #define COLUMN_CVC3		14
       #define COLUMN_CVC4		15
       #define COLUMN_TXFILTERBANDWIDTH		16
       #define COLUMN_RXFILTERBANDWIDTH		17
       #define COLUMN_DLRATIO		18
       #define COLUMN_ULRATIO		19
       #define COLUMN_RADIO_HEAD_STATUS_VSWRSHUTDOWNALARMTHRESHOLD		20
       #define COLUMN_OVERPOWERSHUTDOWNALARMTHRESHOLD		21
       #define COLUMN_RADIO_HEAD_STATUS_LOWPOWERALARMTHRESHOLD		22
       #define COLUMN_CENTERFREQUENCY		23
       #define COLUMN_FA1CHANNELBANDWIDTHSTATUS		24
       #define COLUMN_FA2CHANNELBANDWIDTHSTATUS		25
       #define COLUMN_FA3CHANNELBANDWIDTHSTATUS		26
       #define COLUMN_FA4CHANNELBANDWIDTHSTATUS		27
       #define COLUMN_TX1STFAFREQUENCY		28
       #define COLUMN_TX2NDFAFREQUENCY		29
       #define COLUMN_TX3RDFAFREQUENCY		30
       #define COLUMN_TX4THFAFREQUENCY		31
       #define COLUMN_RX1STFAFREQUENCY		32
       #define COLUMN_RX2NDFAFREQUENCY		33
       #define COLUMN_RX3RDFAFREQUENCY		34
       #define COLUMN_RX4THFAFREQUENCY		35
       #define COLUMN_TXDELAY		36
       #define COLUMN_RXDELAY		37
       #define COLUMN_TXPATHAFACONTROL		38
       #define COLUMN_TXPATHBFACONTROL		39
       #define COLUMN_RXPATHAFACONTROL		40
       #define COLUMN_RXPATHBFACONTROL		41
       #define COLUMN_MULTIMODE		42
       #define COLUMN_POWERBOOST		43
       #define COLUMN_FILTERSWITCHINGMODE		44
       #define COLUMN_PATHATXONOROFF		45
       #define COLUMN_PATHATXENABLEORDISABLE		46
       #define COLUMN_PATHATEMPERATURE		47
       #define COLUMN_PATHATXRFPOWER		48
       #define COLUMN_PATHARETURNLOSS		49
       #define COLUMN_PATHAFA1RSSIDIGITALIQLEVEL		50
       #define COLUMN_PATHAFA2RSSIDIGITALIQLEVEL		51
       #define COLUMN_PATHAFA3RSSIDIGITALIQLEVEL		52
       #define COLUMN_PATHAFA4RSSIDIGITALIQLEVEL		53
       #define COLUMN_PATHAFA1TSSIDIGITALIQLEVEL		54
       #define COLUMN_PATHAFA2TSSIDIGITALIQLEVEL		55
       #define COLUMN_PATHAFA3TSSIDIGITALIQLEVEL		56
       #define COLUMN_PATHAFA4TSSIDIGITALIQLEVEL		57
       #define COLUMN_PATHAFA1TXATTEN		58
       #define COLUMN_PATHAFA2TXATTEN		59
       #define COLUMN_PATHAFA3TXATTEN		60
       #define COLUMN_PATHAFA4TXATTEN		61
       #define COLUMN_PATHBTXONOROFF		62
       #define COLUMN_PATHBTXENABLEORDISABLE		63
       #define COLUMN_PATHBTEMPERATURE		64
       #define COLUMN_PATHBTXRFPOWER		65
       #define COLUMN_PATHBRETURNLOSS		66
       #define COLUMN_PATHBFA1RSSIDIGITALIQLEVEL		67
       #define COLUMN_PATHBFA2RSSIDIGITALIQLEVEL		68
       #define COLUMN_PATHBFA3RSSIDIGITALIQLEVEL		69
       #define COLUMN_PATHBFA4RSSIDIGITALIQLEVEL		70
       #define COLUMN_PATHBFA1TSSIDIGITALIQLEVEL		71
       #define COLUMN_PATHBFA2TSSIDIGITALIQLEVEL		72
       #define COLUMN_PATHBFA3TSSIDIGITALIQLEVEL		73
       #define COLUMN_PATHBFA4TSSIDIGITALIQLEVEL		74
       #define COLUMN_PATHBFA1TXATTEN		75
       #define COLUMN_PATHBFA2TXATTEN		76
       #define COLUMN_PATHBFA3TXATTEN		77
       #define COLUMN_PATHBFA4TXATTEN		78

#define RADIO_HEAD_STATUS_MAX_COL	COLUMN_PATHBFA4TXATTEN

    /* Typical data structure for a row entry */
typedef struct radioHeadStatusTable_entry {
    /* Index values */
    unsigned int index;

    /* Column values */
    unsigned int c1IQMap;
    unsigned int c2IQMap;
    unsigned int c3IQMap;
    unsigned int c4IQMap;
    unsigned char fwMode;
    unsigned char numberOfFA;
    unsigned int mappingOf1FAStatus;
    unsigned int mappingOf2FAStatus;
    unsigned int mappingOf3FAStatus;
    unsigned int mappingOf4FAStatus;
    unsigned char cvc1;
    unsigned char cvc2;
    unsigned char cvc3;
    unsigned char cvc4;
    unsigned char txFilterBandwidth;
    unsigned char rxFilterBandwidth;
    unsigned char dlRatio;
    unsigned char ulRatio;
    unsigned int vswrShutdownAlarmThreshold;
    unsigned int overpowerShutdownAlarmThreshold;
    unsigned int lowpowerAlarmThreshold;
    unsigned int centerFrequency;
    unsigned int fa1ChannelBandwidthStatus;
    unsigned int fa2ChannelBandwidthStatus;
    unsigned int fa3ChannelBandwidthStatus;
    unsigned int fa4ChannelBandwidthStatus;
    unsigned int tx1stFaFrequency;
    unsigned int tx2ndFaFrequency;
    unsigned int tx3rdFaFrequency;
    unsigned int tx4thFaFrequency;
    unsigned int rx1stFaFrequency;
    unsigned int rx2ndFaFrequency;
    unsigned int rx3rdFaFrequency;
    unsigned int rx4thFaFrequency;
    unsigned int txDelay;
    unsigned int rxDelay;
    unsigned char txPathAFaControl;
    unsigned char txPathBFaControl;
    unsigned char rxPathAFaControl;
    unsigned char rxPathBFaControl;
    unsigned char multiMode;
    unsigned char powerBoost;
    unsigned char filterSwitchingMode;
    unsigned char pathATxOnOrOff;
    unsigned char pathATxEnableOrDisable;
    short pathATemperature;
    short pathATxRFPower;
    unsigned short pathAReturnLoss;
    unsigned int pathAFa1RssiDigitalIQLevel;
    unsigned int pathAFa2RssiDigitalIQLevel;
    unsigned int pathAFa3RssiDigitalIQLevel;
    unsigned int pathAFa4RssiDigitalIQLevel;
    unsigned int pathAFa1TssiDigitalIQLevel;
    unsigned int pathAFa2TssiDigitalIQLevel;
    unsigned int pathAFa3TssiDigitalIQLevel;
    unsigned int pathAFa4TssiDigitalIQLevel;
    unsigned short pathAFa1TxAtten;
    unsigned short pathAFa2TxAtten;
    unsigned short pathAFa3TxAtten;
    unsigned short pathAFa4TxAtten;
    unsigned char pathBTxOnOrOff;
    unsigned char pathBTxEnableOrDisable;
    short pathBTemperature;
    short pathBTxRFPower;
    unsigned short pathBReturnLoss;
    unsigned int pathBFa1RssiDigitalIQLevel;
    unsigned int pathBFa2RssiDigitalIQLevel;
    unsigned int pathBFa3RssiDigitalIQLevel;
    unsigned int pathBFa4RssiDigitalIQLevel;
    unsigned int pathBFa1TssiDigitalIQLevel;
    unsigned int pathBFa2TssiDigitalIQLevel;
    unsigned int pathBFa3TssiDigitalIQLevel;
    unsigned int pathBFa4TssiDigitalIQLevel;
    unsigned short pathBFa1TxAtten;
    unsigned short pathBFa2TxAtten;
    unsigned short pathBFa3TxAtten;
    unsigned short  pathBFa4TxAtten;

    /* Illustrate using a simple linked list */
    int   valid;
    struct radioHeadStatusTable_entry *next;
}radioHeadStatusTableEntry;

#endif /* RADIOHEADSTATUS_H */
