/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 19302 2010-08-13 12:19:42Z dts12 $
 */
#ifndef PDCPGENCOUNTERS_H
#define PDCPGENCOUNTERS_H

/* function declarations */
void init_pdcpGenCounters(void);
void initialize_table_pdcpGenCountersTable(void);

/* column number definitions for table pdcpGenCountersTable */
       #define COLUMN_PDCP_GEN_COUNTERS_INDEX	1
       #define COLUMN_RXPDUS		2
       #define COLUMN_TXPDUS		3
       #define COLUMN_PDCP_GEN_COUNTERS_ERRORPDUSRECV		4
       #define COLUMN_NUMSDUSDISC		5
       #define COLUMN_NUMCMPFAILS		6
       #define COLUMN_NUMDECMPFAILS		7
       #define COLUMN_NUMINTGPRTFAILS		8
       #define COLUMN_NUMINTGVRFFAILS		9
       #define COLUMN_NUMCIPHFAILS		10
       #define COLUMN_NUMDECIPHFAILS		11
       #define COLUMN_NUMPDUSDISCOBDTMREXP		12
       #define COLUMN_NUMSDUSDISCOBDTMREXP		13
       #define COLUMN_NUMPKTSFRWD		14
       #define COLUMN_NUMPKTSRCVD		15
       #define COLUMN_PDCP_GEN_COUNTERS_LASTCOLLECTEDAT  16	

#define PDCP_GEN_COUNTERS_MAX_COL  COLUMN_PDCP_GEN_COUNTERS_LASTCOLLECTEDAT

#define PDCP_GEN_COUNTERS_LCOLLECT_BUF_LEN_MAX 18

    /* Typical data structure for a row entry */
typedef struct pdcpGenCountersTable_entry {
    /* Index values */
    unsigned int index;

    /* Column values */
    unsigned int rxPdus;
    unsigned int txPdus;
    unsigned int errorPdusRecv;
    unsigned int numSdusDisc;
    unsigned int numCmpFails;
    unsigned int numDecmpFails;
    unsigned int numIntgPrtFails;
    unsigned int numIntgVrfFails;
    unsigned int numCiphFails;
    unsigned int numDeciphFails;
    unsigned int numPdusDiscObdTmrExp;
    unsigned int numSdusDiscObdTmrExp;
    unsigned int numPktsFrwd;
    unsigned int numPktsRcvd;
    char lastCollectedAt[PDCP_GEN_COUNTERS_LCOLLECT_BUF_LEN_MAX];
    size_t lastCollectedAt_len;    

    /* Illustrate using a simple linked list */
    int   valid;
    struct pdcpGenCountersTable_entry *next;
}pdcpGenCountersTableEntry;

#endif /* PDCPGENCOUNTERS_H */
