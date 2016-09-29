/*
 -- @file    ueEventHistoryLogConfig.h
 -- @section Copyright Powerwave Technologies, Inc., 2010
 --
 -- Author: Powerwave
 -- Generated on : 11/2/2011 7:58:20 PM
 --
*/

#ifndef UEEVENTHISTORYLOGCONFIG_H
#define UEEVENTHISTORYLOGCONFIG_H

/* Function Declarations */
void init_ueEventHistoryLogConfig(void);
void initialize_table_ueEventHistoryLogConfigTable(void);

/* column number definitions for table ueEventHistoryLogConfigTable */
       #define UEEVENTHISTORYLOGCONFIG_UEEVENTHISTORYINDEX             1 
       #define UEEVENTHISTORYLOGCONFIG_CRNTI                           2 
       #define UEEVENTHISTORYLOGCONFIG_UEPROCEDUREBITMAP               3 
       #define UEEVENTHISTORYLOGCONFIG_UEEVENTMEASURES                 4 

#define UEEVENTHISTORYLOGCONFIG_MAX_COL    UEEVENTHISTORYLOGCONFIG_UEEVENTMEASURES

/* OCTET STRING MAX LEN DEFINITIONS */

/* Typical data structure for a row entry */
typedef struct ueEventHistoryLogConfigTable_entry {
    /* Index values */
    unsigned int ueEventHistoryIndex;

    /* Column values */
    unsigned short cRnti;
    unsigned char ueProcedureBitMap;
    unsigned int ueEventMeasures;

    /* Illustrate using a simple linked list */
    int valid;
    struct ueEventHistoryLogConfigTable_entry *next;
} ueEventHistoryLogConfigTableEntry;

#endif /* UEEVENTHISTORYLOGCONFIG_H */
