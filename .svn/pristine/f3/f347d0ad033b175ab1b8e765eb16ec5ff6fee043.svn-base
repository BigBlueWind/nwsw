/* @file    rrmCounters.h */
/* @section Copyright Powerwave Technologies, Inc., 2010 */
/* This file auto generated on : 12/7/2012 6:50:29 PM*/

#ifndef RRMCOUNTERS_H
#define RRMCOUNTERS_H

/* Function Declarations */
void init_rrmCounters(void);
void initialize_table_rrmCountersTable(void);

/* column number definitions for table rrmCountersTable */
       #define RRMCOUNTERS_INDEX                             1 
       #define RRMCOUNTERS_NUMRLFAILURES                     2 
       #define RRMCOUNTERS_NS1SETUPREQ                       3 
       #define RRMCOUNTERS_NUMINCOMINGHOSUCCESSFUL           4 
       #define RRMCOUNTERS_NUMINCOMINGHOFAILED               5 
       #define RRMCOUNTERS_NUMINCOMINGHOREJECTED             6 
       #define RRMCOUNTERS_NUMS1APREJECTED                   7 
       #define RRMCOUNTERS_NRRCCONNREQ                       8 
       #define RRMCOUNTERS_NRRCCONNSETUP                     9 
       #define RRMCOUNTERS_NRRCCONNSETUPCMPL                 10 
       #define RRMCOUNTERS_NRRCCONNREL                       11 
       #define RRMCOUNTERS_NDLINFOTRF                        12 
       #define RRMCOUNTERS_NULINFOTRF                        13 
       #define RRMCOUNTERS_NULNASTRFMSG                      14 
       #define RRMCOUNTERS_NDLNASTRFMSG                      15 
       #define RRMCOUNTERS_NINITCTXTSETREQ                   16 
       #define RRMCOUNTERS_NINITCTXTSETRSP                   17 
       #define RRMCOUNTERS_NINITCTXTSETFAIL                  18 
       #define RRMCOUNTERS_NRRCRECONFREQ                     19 
       #define RRMCOUNTERS_NRRCRECONFCMPL                    20 
       #define RRMCOUNTERS_NRRCREESTREQ                      21 
       #define RRMCOUNTERS_NUECAPENQ                         22 
       #define RRMCOUNTERS_NUECAPINFO                        23 
       #define RRMCOUNTERS_NUECAPINFOIND                     24 
       #define RRMCOUNTERS_NSECMODECMD                       25 
       #define RRMCOUNTERS_NSECMODECMPL                      26 
       #define RRMCOUNTERS_NSECMODEFAIL                      27 
       #define RRMCOUNTERS_NUECTXTRELCMPL                    28 
       #define RRMCOUNTERS_NUECTXTRELCMD                     29 
       #define RRMCOUNTERS_NUECTXTRELREQ                     30 
       #define RRMCOUNTERS_NUECTXTMODRESP                    31 
       #define RRMCOUNTERS_NS1SETUPFAIL                      32 
       #define RRMCOUNTERS_NPAGING                           33 
       #define RRMCOUNTERS_NRRCCONNREJ                       34 
       #define RRMCOUNTERS_NS1SETUPRSP                       35 
       #define RRMCOUNTERS_NS1RESETTX                        36 
       #define RRMCOUNTERS_NS1RESETRX                        37 
       #define RRMCOUNTERS_LASTCOLLECTEDAT                   38 

#define RRMCOUNTERS_MAX_COL    RRMCOUNTERS_LASTCOLLECTEDAT

/* OCTET STRING MAX LEN DEFINITIONS */
#define RRMCOUNTERS_LASTCOLLECTEDAT_MAX_LEN                   18 

/* Typical data structure for a row entry */
typedef struct rrmCountersTable_entry {
    /* Index values */
    unsigned int index;

    /* Column values */
    unsigned int numRlFailures;
    unsigned int nS1SetupReq;
    unsigned int numIncomingHoSuccessful;
    unsigned int numIncomingHoFailed;
    unsigned int numIncomingHoRejected;
    unsigned int numS1apRejected;
    unsigned int nRrcConnReq;
    unsigned int nRrcConnSetup;
    unsigned int nRrcConnSetupCmpl;
    unsigned int nRrcConnRel;
    unsigned int nDlInfoTrf;
    unsigned int nUlInfoTrf;
    unsigned int nUlNasTrfMsg;
    unsigned int nDlNasTrfMsg;
    unsigned int nInitCtxtSetReq;
    unsigned int nInitCtxtSetRsp;
    unsigned int nInitCtxtSetFail;
    unsigned int nRrcReconfReq;
    unsigned int nRrcReconfCmpl;
    unsigned int nRrcReestReq;
    unsigned int nUeCapEnq;
    unsigned int nUeCapInfo;
    unsigned int nUeCapInfoInd;
    unsigned int nSecModeCmd;
    unsigned int nSecModeCmpl;
    unsigned int nSecModeFail;
    unsigned int nUeCtxtRelCmpl;
    unsigned int nUeCtxtRelCmd;
    unsigned int nUeCtxtRelReq;
    unsigned int nUeCtxtModResp;
    unsigned int nS1SetupFail;
    unsigned int nPaging;
    unsigned int nRrcConnRej;
    unsigned int nS1SetupRsp;
    unsigned int nS1ResetTx;
    unsigned int nS1ResetRx;
    char lastCollectedAt[RRMCOUNTERS_LASTCOLLECTEDAT_MAX_LEN];
    size_t lastCollectedAt_len;

    /* Illustrate using a simple linked list */
    int valid;
    struct rrmCountersTable_entry *next;
} rrmCountersTableEntry;

#endif /* RRMCOUNTERS_H */
