//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL ptmdiag.h
 *
 * @brief CoreSight components' registers
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif

#define TRACE_BLOCK_SIZE_PARAID            0xE180
#define TRACE_REQUEST_PARAID               0xE181
#define TRACE_DATA_PARAID                  0xE182
#define POWER_DOWN_PARAID                  0xE183
#define BRANCH_OUTPUT_PARAID               0xE184
#define CYCLE_ACCURATE_PARAID              0xE185
#define CONTEXTID_SIZE_PARAID              0xE186
#define TIMESTAMP_ENABLE_PARAID            0xE187
#define RETURN_STACK_PARAID                0xE188
#define TIMESTAMP_EVENT_PARAID             0xE189
#define SYNC_FREQ_PARAID                   0xE18A
#define FOREC_SYNC_PACKET_PARAID           0xE18B
#define ADDR_COMP_VAL1_PARAID              0xE18C
#define ADDR_COMP_VAL2_PARAID              0xE18D
#define ADDR_COMP_VAL3_PARAID              0xE18E
#define ADDR_COMP_VAL4_PARAID              0xE18F
#define ADDR_COMP_VAL5_PARAID              0xE190
#define ADDR_COMP_VAL6_PARAID              0xE191
#define ADDR_COMP_VAL7_PARAID              0xE192
#define ADDR_COMP_VAL8_PARAID              0xE193
#define CONTEXTID_COMP_VAL_PARAID          0xE194
#define CONTEXTID_COMP_MASK_PARAID         0xE195
#define COUNTER_VAL1_PARAID                0xE196
#define COUNTER_VAL2_PARAID                0xE197
#define COUNTER_ENABLE1_PARAID             0xE198
#define COUNTER_ENABLE2_PARAID             0xE199
#define COUNTER_RELOAD_VAL1_PARAID         0xE19A
#define COUNTER_RELOAD_VAL2_PARAID         0xE19B
#define COUNTER_RELOAD_EVT1_PARAID         0xE19C
#define COUNTER_RELOAD_EVT2_PARAID         0xE19D
#define SEQUENCER_STATE_PARAID             0xE19E
#define SEQ_TRAN_EVT_1TO2_PARAID           0xE19F
#define SEQ_TRAN_EVT_1TO3_PARAID           0xE1A0
#define SEQ_TRAN_EVT_2TO1_PARAID           0xE1A1
#define SEQ_TRAN_EVT_2TO3_PARAID           0xE1A2
#define SEQ_TRAN_EVT_3TO1_PARAID           0xE1A3
#define SEQ_TRAN_EVT_3TO2_PARAID           0xE1A4
#define TRACE_ENABLE_START_PARAID          0xE1A5
#define TRACE_ENABLE_STOP_PARAID           0xE1A6
#define TRACE_ENABLE_ACTIVE_PARAID         0xE1A7
#define TRACE_RANGE_PARAID                 0xE1A8
#define TRACE_RANGE_EXCLUDE_PARAID         0xE1A9
#define TRACE_ENABLE_EVT_PARAID            0xE1AA
#define PTMID_PARAID                       0xE1AB

#define ENABLE_ALL_PTM                     0xFF


MXRC  PtmDiagInit (void);
APIHEC PtmDiagApiHandler (LPVOID pData, ApiHeader * pApi, ApiHeader * pResp);

typedef struct _PTMDIAG_CTX_
{
    HANDLE  hHeap;
    UINT32  TraceBlockSize;
	UINT32  EtbBufSz;
	void    *EtbBufPtr;
}PtmDiagCtx, *PPtmDiagCtx;


#define RC_PTMDIAG_OK               MX_OK
#define RC_PTMDIAG_NO_PTMID         DEF_USRRC(APP_MODULE, PTMDIAG, 1)
#define RC_PTMDIAG_PARAM_ERR        DEF_USRRC(APP_MODULE, PTMDIAG, 2)
#define RC_PTMDIAG_ALLOC_ERR        DEF_USRRC(APP_MODULE, PTMDIAG, 3)
#define RC_PTMDIAG_UNKNOWN_API      DEF_USRRC(APP_MODULE, PTMDIAG, 4)
#define RC_PTMDIAG_UNSUP_TYPE       DEF_USRRC(APP_MODULE, PTMDIAG, 5)    

#ifndef PTMDIAG_H_
#define PTMDIAG_H_

#endif /*CORESIGHT_H_*/

#ifdef __cplusplus
}
#endif

