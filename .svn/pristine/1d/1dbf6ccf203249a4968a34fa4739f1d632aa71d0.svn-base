/*
 -- @file    srsConfigDedicated.h
 -- @section Copyright Powerwave Technologies, Inc., 2010
 --
 -- Author: Powerwave
 -- Generated on : 11/2/2011 7:58:20 PM
 --
*/

#ifndef SRSCONFIGDEDICATED_H
#define SRSCONFIGDEDICATED_H

/* Function Declarations */
void init_srsConfigDedicated(void);
void initialize_table_srsConfigDedicatedTable(void);

/* column number definitions for table srsConfigDedicatedTable */
       #define SRSCONFIGDEDICATED_SRSBANDWIDTH                    1 
       #define SRSCONFIGDEDICATED_SRSHOPPINGBANDWIDTH             2 
       #define SRSCONFIGDEDICATED_FREQDOMAINPOSITION              3 
       #define SRSCONFIGDEDICATED_DURATION                        4 

#define SRSCONFIGDEDICATED_MAX_COL    SRSCONFIGDEDICATED_DURATION

/* OCTET STRING MAX LEN DEFINITIONS */

/* Typical data structure for a row entry */
typedef struct srsConfigDedicatedTable_entry {
    /* Index values */
    unsigned int srsBandwidth;

    /* Column values */
    unsigned int  srsHoppingBandWidth;
    unsigned char frequecyDomainPosition;
    unsigned char duration;

    /* Illustrate using a simple linked list */
    int valid;
    struct srsConfigDedicatedTable_entry *next;
} srsConfigDedicatedTableEntry;

#endif /* SRSCONFIGDEDICATED_H */
