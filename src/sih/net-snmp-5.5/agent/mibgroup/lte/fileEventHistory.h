/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 19302 2010-08-13 12:19:42Z dts12 $
 */
#ifndef FILEEVENTHISTORY_H
#define FILEEVENTHISTORY_H

/* function declarations */
void init_fileEventHistory(void);
void initialize_table_fileEventHistoryTable(void);

/* column number definitions for table fileEventHistoryTable */
       #define COLUMN_FILE_EVENT_HISTORY_INDEX		1
       #define COLUMN_FILE_EVENT_HISTORY_FILENAME		2
       #define COLUMN_FILESTATUS		3
       #define COLUMN_WASSUCCESSFUL		4

#define FILE_EVENT_HISTORY_MAX_COL COLUMN_WASSUCCESSFUL
#define FILE_EVENT_HISTORY_NAME_BUF_LEN_MAX         257

    /* Typical data structure for a row entry */
typedef struct fileEventHistoryTable_entry {
    /* Index values */
    unsigned int index;

    /* Column values */
    char fileName[FILE_EVENT_HISTORY_NAME_BUF_LEN_MAX];
    size_t fileName_len;
    unsigned int fileStatus;
    unsigned char wasSuccessful;

    /* Illustrate using a simple linked list */
    int   valid;
    struct fileEventHistoryTable_entry *next;
}fileEventHistoryTableEntry;

#endif /* FILEEVENTHISTORY_H */