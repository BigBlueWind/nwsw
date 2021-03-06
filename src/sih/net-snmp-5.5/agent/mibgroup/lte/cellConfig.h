/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 19302 2010-08-13 12:19:42Z dts12 $
 */
#ifndef CELLCONFIG_H
#define CELLCONFIG_H

/* function declarations */
void init_cellConfig(void);
void initialize_table_cellConfigTable(void);

/* column number definitions for table cellConfigTable */
       #define COLUMN_CELLID		1
       #define COLUMN_CELLTYPE		2
       #define COLUMN_PLMNMCC		3
       #define COLUMN_PLMNMNC		4
       #define COLUMN_TAC		5
       #define COLUMN_FREQBANDIND	6
       #define COLUMN_DLBANDWIDTH	7
       #define COLUMN_DLEARFCN		8
       #define COLUMN_ULBANDWIDTH	9
       #define COLUMN_ULEARFCN		10
       #define COLUMN_CELLRESERVEDFOROPERATORUSE	11
       #define COLUMN_MAXCONNECTEDUES			12
       #define COLUMN_ACBARRINGEMERGENCY		13
       #define COLUMN_ACBARRMOSIGENABLED		14
       #define COLUMN_ACBARRMOSIGFACTOR			15
       #define COLUMN_ACBARRMOSIGTIME			16
       #define COLUMN_ACBARRMOSIGSPECIALAC		17
       #define COLUMN_ACBARRMODATAENABLED		18
       #define COLUMN_ACBARRMODATAFACTOR		19
       #define COLUMN_ACBARRMODATATIME			20
       #define COLUMN_ACBARRMODATASPECIALAC		21
       #define COLUMN_STARTRNTI				22
       #define COLUMN_RSRPFILTERCOEFFICIENT		23
       #define COLUMN_CELL_SCHEDULERPROFILEID		24
       #define COLUMN_CELL_PHYPROFILEID			25
       #define COLUMN_PHYSICALCELLID			26
       #define COLUMN_PHYSICALCELLGROUPID		27
       #define COLUMN_PMAX				28
       #define COLUMN_UERESPONSETIMER			29
       #define COLUMN_RRCCONNREJWAITTIME		30
       #define COLUMN_UEINACTIVITYCHECKINTERVALSECS 	31 	
       #define COLUMN_CELL_SYSINFO1PROFILEID		32
       #define COLUMN_CELL_SYSINFO2PROFILEID		33
       #define COLUMN_CELL_SYSINFO3PROFILEID		34
       #define COLUMN_CELL_SIRETXCNT                 	35
       #define COLUMN_CELL_ANTINFOTXMODE         	36
       #define COLUMN_CELL_INITIALULCQI                 37
       #define COLUMN_CELL_PUCCHRESOURCESIZE     	38
       #define COLUMN_CELL_CQIPERIODICITYINDEX   	39
       #define COLUMN_CELL_SRIPERIODICITYINDEX   	40
       #define COLUMN_CELL_UECQIBASEDRLFDETECTIONTIMER  41
       #define COLUMN_CELL_UECFGMACCOLLATEDCQICOUNT     42

#define CELL_CONFIG_MAX_COL           COLUMN_CELL_UECFGMACCOLLATEDCQICOUNT

#define CELL_CONFIG_PLMN_MCC_MAX        4
#define CELL_CONFIG_PLMN_MNC_MAX        4
#define CELL_CONFIG_AC_BAR_MOSIG_MAX    6
#define CELL_CONFIG_AC_BAR_MODATA_MAX   6

    /* Typical data structure for a row entry */
typedef struct cellConfigTable_entry {
    /* Index values */
    unsigned char cellId;

    /* Column values */
    unsigned int cellType;
    char plmnMCC[CELL_CONFIG_PLMN_MCC_MAX];
    size_t plmnMCC_len;
    char plmnMNC[CELL_CONFIG_PLMN_MNC_MAX];
    size_t plmnMNC_len;
    unsigned short tac;
    unsigned char freqBandInd;
    unsigned int dlBandwidth;
    unsigned int dlEarfcn;
    unsigned int ulBandwidth;
    unsigned int ulEarfcn;
    unsigned int cellReservedForOperatorUse;
    unsigned short maxConnectedUes;
    unsigned char acBarringEmergency;
    unsigned char acBarrMoSigEnabled;
    unsigned int acBarrMoSigFactor;
    unsigned int acBarrMoSigTime;
    char acBarrMoSigSpecialAC[CELL_CONFIG_AC_BAR_MOSIG_MAX];
    size_t acBarrMoSigSpecialAC_len;
    unsigned char acBarrMoDataEnabled;
    unsigned int acBarrMoDataFactor;
    unsigned int acBarrMoDataTime;
    char acBarrMoDataSpecialAC[CELL_CONFIG_AC_BAR_MODATA_MAX];
    size_t acBarrMoDataSpecialAC_len;
    unsigned short startRnti;
    unsigned int rsrpFilterCoefficient;
    unsigned char schedulerProfileId;
    unsigned char phyProfileId;
    unsigned char physicalCellId;
    unsigned char physicalCellGroupId;
    char pMax;
    unsigned int  ueResponseTimer;
    unsigned char rrcConnRejWaitTime;
    unsigned int  ueInactivityCheckInterval_secs;
    unsigned char sysInfo1ProfileId;
    unsigned char sysInfo2ProfileId;
    unsigned char sysInfo3ProfileId;
    unsigned char siReTxCnt;
    unsigned int  antInfoTxMode;
    unsigned char initialUlCqi;
    unsigned char pucchResourceSize;
    unsigned int  cqiPeriodicityIndex;
    unsigned int  sriPeriodicityIndex;
    unsigned int  ueCqiBasedRlfDetectionTimer;
    unsigned char ueCfgMacCollatedCqiCount;

    /* Illustrate using a simple linked list */
    int   valid;
    struct cellConfigTable_entry *next;
} cellConfigTableEntry;


#endif /* CELLCONFIG_H */
