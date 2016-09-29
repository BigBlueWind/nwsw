/*
 -- @file    sonConfig.h
 -- @section Copyright Powerwave Technologies, Inc., 2010
 --
 -- Author: Powerwave
 -- Generated on : 11/2/2011 7:58:20 PM
 --
*/

#ifndef SONCONFIG_H
#define SONCONFIG_H

/* Function Declarations */
void init_sonConfig(void);
void initialize_table_sonConfigTable(void);

/* column number definitions for table sonConfigTable */
       #define SONCONFIG_SONPROFILEID                    1 
       #define SONCONFIG_INTRAFREQHOTHRESHOLD            2 
       #define SONCONFIG_ICICTHRESHOLD                   3 
       #define SONCONFIG_UELOADCHANGETHRESHOLD           4 

#define SONCONFIG_MAX_COL    SONCONFIG_UELOADCHANGETHRESHOLD

/* OCTET STRING MAX LEN DEFINITIONS */

/* Typical data structure for a row entry */
typedef struct sonConfigTable_entry {
    /* Index values */
    unsigned int sonProfileId;

    /* Column values */
    unsigned int intraFreqHoThreshold;
    unsigned int icicThreshold;
    unsigned int ueLoadChangeThreshold;

    /* Illustrate using a simple linked list */
    int valid;
    struct sonConfigTable_entry *next;
} sonConfigTableEntry;

#endif /* SONCONFIG_H */
