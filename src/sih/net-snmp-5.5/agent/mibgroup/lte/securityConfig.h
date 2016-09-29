/*
 -- @file    securityConfig.h
 -- @section Copyright Powerwave Technologies, Inc., 2010
 --
 -- Author: Powerwave
 -- Generated on : 11/2/2011 7:58:20 PM
 --
*/

#ifndef SECURITYCONFIG_H
#define SECURITYCONFIG_H

/* Function Declarations */
void init_securityConfig(void);
void initialize_table_securityConfigTable(void);

/* column number definitions for table securityConfigTable */
       #define SECURITYCONFIG_INDEX                   1 
       #define SECURITYCONFIG_UUCIPHERING             2 
       #define SECURITYCONFIG_UUINTEGRITYPROTECTION   3 

#define SECURITYCONFIG_MAX_COL    SECURITYCONFIG_UUINTEGRITYPROTECTION

/* OCTET STRING MAX LEN DEFINITIONS */

/* Typical data structure for a row entry */
typedef struct securityConfigTable_entry {
    /* Index values */
    unsigned int index;

    /* Column values */
    unsigned int  uuCiphering;
    unsigned int  uuIntegrityProtection;

    /* Illustrate using a simple linked list */
    int valid;
    struct securityConfigTable_entry *next;
} securityConfigTableEntry;

#endif /* SECURITYCONFIG_H */
