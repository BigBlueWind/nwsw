/*
 * pst_events.h
 *
 *  Created on: Apr 19, 2011
 *      @author: m_sharma
 */

#ifndef PST_EVENTS_H_
#define PST_EVENTS_H_

/* Events corresponding to messages To/from PWAV developed modules
 * These events should not overlap with CCPU-defined events
 */

#define PWAV_START_EVENT        100

#define EVT_START_QUERY                 ( PWAV_START_EVENT + 0 )
#define EVT_START_IND                   ( PWAV_START_EVENT + 1 )

#define EVT_HELLO                       ( PWAV_START_EVENT + 2 )
#define EVT_HELLO_ACK                   ( PWAV_START_EVENT + 3 )

#define EVT_EXIT                        ( PWAV_START_EVENT + 4 )

#define EVT_BSM_CEM_OPSTART             ( PWAV_START_EVENT + 5 )
#define EVT_CEM_BSM_OPSTART_SUCC        ( PWAV_START_EVENT + 6 )
#define EVT_CEM_BSM_OPSTART_FAIL        ( PWAV_START_EVENT + 7 )

#define EVT_BSM_CEM_OPSTOP_CMD          ( PWAV_START_EVENT + 8 )
#define EVT_CEM_BSM_OPSTOP_CMPL         ( PWAV_START_EVENT + 9 )

#define EVT_BSM_CEM_CFG_REQ             ( PWAV_START_EVENT + 10 )
#define EVT_CEM_BSM_CFG_SUCC            ( PWAV_START_EVENT + 11 )
#define EVT_CEM_BSM_CFG_FAIL            ( PWAV_START_EVENT + 12 )

#define EVT_BSM_S1M_CFG_REQ             ( PWAV_START_EVENT + 13 )
#define EVT_S1M_BSM_CFG_SUCC            ( PWAV_START_EVENT + 14 )
#define EVT_S1M_BSM_CFG_FAIL            ( PWAV_START_EVENT + 15 )

#define EVT_BSM_S1M_OPSTART             ( PWAV_START_EVENT + 16 )
#define EVT_S1M_BSM_OPSTART_SUCC        ( PWAV_START_EVENT + 17 )
#define EVT_S1M_BSM_OPSTART_FAIL        ( PWAV_START_EVENT + 18 )

#define EVT_BSM_S1M_OPSTOP_CMD          ( PWAV_START_EVENT + 19 )
#define EVT_S1M_BSM_OPSTOP_CMPL         ( PWAV_START_EVENT + 20 )

#define EVT_S1M_LINK_UP_IND             ( PWAV_START_EVENT + 21 )
#define EVT_S1M_LINK_DOWN_IND           ( PWAV_START_EVENT + 22 )
#define EVT_S1M_LINK_CLOSED_IND         ( PWAV_START_EVENT + 23 )

#define EVT_BSM_UEH_ACTIVE_UE_STATUS_REQ ( PWAV_START_EVENT + 24 )
#define EVT_UEH_BSM_ACTIVE_UE_STATUS_RSP ( PWAV_START_EVENT + 25 )

#define EVT_BSM_UDH_ACTIVE_RAB_STATUS_REQ ( PWAV_START_EVENT + 26 )
#define EVT_UDH_BSM_ACTIVE_RAB_STATUS_RSP ( PWAV_START_EVENT + 27 )

#define EVT_S1M_RESET_IND               ( PWAV_START_EVENT + 28 )

#define EVT_BSM_SM_S1AP_CFG_REQ         ( PWAV_START_EVENT + 29 )
#define EVT_SM_BSM_S1AP_CFG_SUCC        ( PWAV_START_EVENT + 30 )
#define EVT_SM_BSM_S1AP_CFG_FAIL        ( PWAV_START_EVENT + 31 )

#define EVT_BSM_SM_SCTP_CFG_REQ         ( PWAV_START_EVENT + 32 )
#define EVT_SM_BSM_SCTP_CFG_SUCC        ( PWAV_START_EVENT + 33 )
#define EVT_SM_BSM_SCTP_CFG_FAIL        ( PWAV_START_EVENT + 34 )

#define EVT_BSM_SM_EGTP_CFG_REQ         ( PWAV_START_EVENT + 35 )
#define EVT_SM_BSM_EGTP_CFG_SUCC        ( PWAV_START_EVENT + 36 )
#define EVT_SM_BSM_EGTP_CFG_FAIL        ( PWAV_START_EVENT + 37 )

#define EVT_S1AP_LTH_TRC_IND            ( PWAV_START_EVENT + 38 )
#define EVT_RRC_LTH_TRC_IND             ( PWAV_START_EVENT + 39 )
#define EVT_PDCP_LTH_TRC_IND            ( PWAV_START_EVENT + 40 )
#define EVT_RLC_LTH_TRC_IND             ( PWAV_START_EVENT + 41 )
#define EVT_MAC_LTH_TRC_IND             ( PWAV_START_EVENT + 42 )

#define EVT_BSM_SM_S1AP_TRC_REQ          ( PWAV_START_EVENT + 43 )
#define EVT_BSM_SM_RRC_TRC_REQ           ( PWAV_START_EVENT + 44 )
#define EVT_BSM_SM_PDCP_TRC_REQ          ( PWAV_START_EVENT + 45 )
#define EVT_BSM_SM_RLC_TRC_REQ           ( PWAV_START_EVENT + 46 )
#define EVT_BSM_SM_MAC_TRC_REQ           ( PWAV_START_EVENT + 47 )

#define EVT_SM_BSM_S1AP_TRC_CFM            ( PWAV_START_EVENT + 48 )
#define EVT_SM_BSM_RRC_TRC_CFM             ( PWAV_START_EVENT + 49 )
#define EVT_SM_BSM_PDCP_TRC_CFM            ( PWAV_START_EVENT + 50 )
#define EVT_SM_BSM_RLC_TRC_CFM             ( PWAV_START_EVENT + 51 )
#define EVT_SM_BSM_MAC_TRC_CFM             ( PWAV_START_EVENT + 52 )

#define EVT_BSM_UDH_CFG_REQ             ( PWAV_START_EVENT + 53 )
#define EVT_UDH_BSM_CFG_SUCC            ( PWAV_START_EVENT + 54 )
#define EVT_UDH_BSM_CFG_FAIL            ( PWAV_START_EVENT + 55 )

#define EVT_BSM_UDH_OPSTART             ( PWAV_START_EVENT + 56 )
#define EVT_UDH_BSM_OPSTART_SUCC        ( PWAV_START_EVENT + 57 )
#define EVT_UDH_BSM_OPSTART_FAIL        ( PWAV_START_EVENT + 58 )

#define EVT_BSM_UDH_OPSTOP_CMD          ( PWAV_START_EVENT + 59 )
#define EVT_UDH_BSM_OPSTOP_CMPL         ( PWAV_START_EVENT + 60 )

#define EVT_PMH_SM_PDCP_GEN_STAT_REQ    ( PWAV_START_EVENT + 61 )
#define EVT_PMH_SM_PDCP_UPLANE_STAT_REQ ( PWAV_START_EVENT + 62 )

#define EVT_PMH_SM_SCTP_GEN_STAT_REQ    ( PWAV_START_EVENT + 63 )
#define EVT_PMH_SM_GTP_GEN_STAT_REQ     ( PWAV_START_EVENT + 64 )
#define EVT_PMH_SM_RLC_GEN_STAT_REQ     ( PWAV_START_EVENT + 65 )

#define EVT_PMH_SM_RRC_GEN_STAT_REQ     ( PWAV_START_EVENT + 66 )
#define EVT_PMH_SM_RRC_CELL_STAT_REQ    ( PWAV_START_EVENT + 67 )

#define EVT_PMH_SM_S1AP_GEN_STAT_REQ    ( PWAV_START_EVENT + 68 )
#define EVT_PMH_SM_S1AP_PEER_NODE_STAT_REQ      ( PWAV_START_EVENT + 69 )

#define EVT_PMH_SM_MAC_GEN_STAT_REQ     ( PWAV_START_EVENT + 70 )
#define EVT_PMH_SM_MAC_RLCSAP_STAT_REQ  ( PWAV_START_EVENT + 71 )
#define EVT_PMH_SM_MAC_PHYSAP_STAT_REQ  ( PWAV_START_EVENT + 72 )

#define EVT_SM_PMH_PDCP_GEN_STAT_RSP    ( PWAV_START_EVENT + 73 )
#define EVT_SM_PMH_PDCP_UPLANE_STAT_RSP ( PWAV_START_EVENT + 74 )

#define EVT_SM_PMH_SCTP_GEN_STAT_RSP    ( PWAV_START_EVENT + 75 )
#define EVT_SM_PMH_GTP_GEN_STAT_RSP     ( PWAV_START_EVENT + 76 )
#define EVT_SM_PMH_RLC_GEN_STAT_RSP     ( PWAV_START_EVENT + 77 )

#define EVT_SM_PMH_RRC_GEN_STAT_RSP     ( PWAV_START_EVENT + 78 )
#define EVT_SM_PMH_RRC_CELL_STAT_RSP    ( PWAV_START_EVENT + 79 )

#define EVT_SM_PMH_S1AP_GEN_STAT_RSP    ( PWAV_START_EVENT + 80 )
#define EVT_SM_PMH_S1AP_PEER_NODE_STAT_RSP      ( PWAV_START_EVENT + 81 )

#define EVT_SM_PMH_MAC_GEN_STAT_RSP     ( PWAV_START_EVENT + 82 )
#define EVT_SM_PMH_MAC_RLCSAP_STAT_RSP  ( PWAV_START_EVENT + 83 )
#define EVT_SM_PMH_MAC_PHYSAP_STAT_RSP  ( PWAV_START_EVENT + 84 )

#define EVT_BSM_SM_EGTP_TRC_REQ          ( PWAV_START_EVENT + 85 )
#define EVT_SM_BSM_EGTP_TRC_CFM          ( PWAV_START_EVENT + 86 )
#define EVT_EGTP_LTH_TRC_IND             ( PWAV_START_EVENT + 87 )

#define EVT_BSM_UEH_MOD_STATE_REQ       (PWAV_START_EVENT + 88)
#define EVT_UEH_BSM_MOD_STATE_RSP       (PWAV_START_EVENT + 89)

#define EVT_BSM_UDH_MOD_STATE_REQ       (PWAV_START_EVENT + 90)
#define EVT_UDH_BSM_MOD_STATE_RSP       (PWAV_START_EVENT + 91)

#define EVT_BSM_UEH_UE_EVENT_HISTORY_REQ (PWAV_START_EVENT + 92) 
#define EVT_UEH_BSM_UE_EVENT_HISTORY_RSP (PWAV_START_EVENT + 93)

#define EVT_BSM_UDH_RAB_HISTORY_REQ      (PWAV_START_EVENT + 94)
#define EVT_UDH_BSM_RAB_HISTORY_RSP      (PWAV_START_EVENT + 95)

// RRM Changes
#define EVT_PMH_UEH_RRM_CELL_STAT_REQ ( PWAV_START_EVENT + 96 )
#define EVT_UEH_PMH_RRM_CELL_STAT_RSP ( PWAV_START_EVENT + 97 )

#define EVT_BSM_SM_SET_LOG_CFG         ( PWAV_START_EVENT + 98 )

#define EVT_PMH_TFU_DSTATS_CFG_REQ     ( PWAV_START_EVENT + 99)
#define EVT_TFU_PMH_DSTATS_IND         ( PWAV_START_EVENT + 100)

//GTP Debug Counters Messages
#define EVT_PMH_UDH_DSTAT_REQ          ( PWAV_START_EVENT + 101 )
#define EVT_UDH_PMH_DSTAT_RSP          ( PWAV_START_EVENT + 102 )
#define MAX_PWAV_EVENT                EVT_UDH_PMH_DSTAT_RSP /* Add new events above and update this */


#define NUM_PWAV_EVENTS                 MAX_PWAV_EVENT - PWAV_START_EVENT

#endif /* PST_EVENTS_H_ */
