/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 19302 2010-08-13 12:19:42Z dts12 $
 */
#ifndef SCHEDULERCONFIG_H
#define SCHEDULERCONFIG_H

/* function declarations */
void init_schedulerConfig(void);
void initialize_table_schedulerConfigTable(void);

/* column number definitions for table schedulerConfigTable */
       #define COLUMN_SCHEDULERPROFILEID		1
       #define SCHEDULERCONFIG_SCHEDULINGALGORITHM           2 
       #define COLUMN_MAXDLHARQTX		3
       #define COLUMN_MAXULHARQTX		4
       #define COLUMN_MAXMSG4HARQTX		5
       #define COLUMN_PFICHCFI		6
       #define COLUMN_PERIODICPHRTIMER		7
       #define COLUMN_PROHIBITPHRTIMER		8
       #define COLUMN_DLPATHLOSSCHANGE		9
       #define COLUMN_PERIODICBSRTIMER		10
       #define COLUMN_RETXBSRTIMER		11

#define SCHEDULER_CONFIG_MAX_COL   COLUMN_RETXBSRTIMER

    /* Typical data structure for a row entry */
typedef struct schedulerConfigTable_entry {
    /* Index values */
    unsigned char schedulerProfileId;

    /* Column values */
    unsigned int schedulingAlgorithm;
    unsigned char maxDlHarqTx;
    unsigned int  maxUlHarqTx;
    unsigned char maxMsg4HarqTx;
    unsigned char pfichCfi;
    unsigned int periodicPhrTimer;
    unsigned int prohibitPhrTimer;
    unsigned int dlPathlossChange;
    unsigned int periodicBsrTimer;
    unsigned int retxBsrTimer;

/* Illustrate using a simple linked list */
    int   valid;
    struct schedulerConfigTable_entry *next;
} schedulerConfigTableEntry;

#endif /* SCHEDULERCONFIG_H */