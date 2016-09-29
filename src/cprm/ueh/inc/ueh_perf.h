/**
 * @file    ueh_perf.h
 * @brief defines for User Equipment Handler (Abbrev: UEH) performance 
 *    measurements
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author         : Amar Ramteke
 * Date           : 16th Sept 2010
 * Change History :
 */

#ifndef UEH_PERF_H
#define UEH_PERF_H

/* This only for LTE eNB performance Measurement related */

#define UEH_PERF_NO_DRBS 1
#define UEH_PERF_ERAB_ID 1
#define UEH_PERF_ERAB_QCI 1
#define UEH_PERF_MBR_DL   10000
#define UEH_PERF_MBR_UL   2000
#define UEH_PERF_GBR_DL   10000
#define UEH_PERF_GBR_UL   2000

#define UEH_PERF_AGGR_DL   10000
#define UEH_PERF_AGGR_UL   2000

/* This value should be EVTEUDELEGTPTUNEL+1 */
/* But selecting bigger value */
#define UEH_NOTIFY_SATRT_DATA   88

#define UEH_PERF_LOAD_TMR_VAL  2
#define UEH_PERF_TMR_EVNT_LOAD    1
#define UEH_PERF_MAX_TMR      1
#define UEH_PERF_PDCP_SAP_ID   2
#define UEH_PERF_DRB_ID        1
#define UEH_PERF_SDU_SIZE      1450
#define UEH_PERF_NO_SDUS      10 

#endif /* UEH_PERF_H */
