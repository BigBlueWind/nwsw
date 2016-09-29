/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 19302 2010-08-13 12:19:42Z dts12 $
 */
#ifndef RABHISTORY_H
#define RABHISTORY_H

/* function declarations */
void init_rabHistory(void);
void initialize_table_rabHistoryTable(void);

/* column number definitions for table rabHistoryTable */
       #define COLUMN_RAB_HISTORY_INDEX		1
       #define COLUMN_INTERNALUEID		2
       #define COLUMN_RABIDENTITY		3
       #define COLUMN_DRBIDENTITY		4
       #define COLUMN_RABQCI		        5
       #define COLUMN_RABSETUPTIME		6
       #define COLUMN_RABRELEASETIME		7
       #define COLUMN_RABRELEASECAUSE		8
       #define COLUMN_RAB_HISTORY_DLGTPTUNNELID	9
       #define COLUMN_RAB_HISTORY_ULGTPTUNNELID   10
       #define COLUMN_RAB_HISTORY_GTPKILOBYTESDL  11
       #define COLUMN_RAB_HISTORY_GTPKILOBYTESUL  12

#define RAB_HISTORY_MAX_COL  COLUMN_RAB_HISTORY_GTPKILOBYTESUL
#define RAB_HISTORY_TIME_MAX_BUF_LEN           18


    /* Typical data structure for a row entry */
typedef struct rabHistoryTable_entry {
    /* Index values */
    unsigned int index;

    /* Column values */
    unsigned int internalUeId;
    unsigned char rabIdentity;
    unsigned char drbIdentity;
    unsigned char rabQci;
    char rabSetupTime[RAB_HISTORY_TIME_MAX_BUF_LEN];
    size_t rabSetupTime_len;
    char rabReleaseTime[RAB_HISTORY_TIME_MAX_BUF_LEN];
    size_t rabReleaseTime_len;
    unsigned int rabReleaseCause;
    unsigned int dlGtpTunnelId;
    unsigned int ulGtpTunnelId;
    unsigned int gtpKiloBytesDL;
    unsigned int gtpKiloBytesUL;

    /* Illustrate using a simple linked list */
    int   valid;
    struct rabHistoryTable_entry *next;
}rabHistoryTableEntry;

#endif /* RABHISTORY_H */