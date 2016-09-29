/**
 * @file    ueh_pmh_if.h
 * @brief Data structures for CCPU stacks and PMH interface
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author         : Ramprasad Reddy Chinta
 * Date           : 10-02-2011
 * Change History :
 */

#ifndef UEH_PMH_IF_H_
#define UEH_PMH_IF_H_

#include "cmn_types.h"
#include "bs_tables.h"

#define MAX_NUMBER_OF_PROFILE 2

/******** Request messages *************/

typedef struct s_MsgPmhSmMacGenStatReq
{
  UINT32  transId;
  TBOOL   resetAllCounters; /* TRUE = 1 (Reset the counter values), FALSE = 0 (Get the counter values) */
} MsgPmhSmMacGenStatReq;

typedef struct s_MsgPmhSmMacRlcsapStatReq
{
  UINT32  transId;
  TBOOL   resetAllCounters; /* TRUE = 1 (Reset the counter values), FALSE = 0 (Get the counter values) */
} MsgPmhSmMacRlcsapStatReq;

typedef struct s_MsgPmhSmMacPhysapStatReq
{
  UINT32  transId;
  TBOOL   resetAllCounters; /* TRUE = 1 (Reset the counter values), FALSE = 0 (Get the counter values) */
} MsgPmhSmMacPhysapStatReq;

typedef struct s_MsgPmhSmRlcGenStatReq
{
  UINT32  transId;
  TBOOL   resetAllCounters; /* TRUE = 1 (Reset the counter values), FALSE = 0 (Get the counter values) */
} MsgPmhSmRlcGenStatReq;

typedef struct s_MsgPmhSmPdcpGenStatReq
{
  UINT32  transId;
  TBOOL   resetAllCounters; /* TRUE = 1 (Reset the counter values), FALSE = 0 (Get the counter values) */
} MsgPmhSmPdcpGenStatReq;

typedef struct s_MsgPmhSmPdcpUplaneStatReq
{
  UINT32  transId;
  TBOOL   resetAllCounters; /* TRUE = 1 (Reset the counter values), FALSE = 0 (Get the counter values) */
} MsgPmhSmPdcpUplaneStatReq;

typedef struct s_MsgPmhSmGtpGenStatReq
{
  UINT32  transId;
  TBOOL   resetAllCounters; /* TRUE = 1 (Reset the counter values), FALSE = 0 (Get the counter values) */
} MsgPmhSmGtpGenStatReq;

typedef struct s_MsgPmhSmSctpGenStatReq
{
  UINT32  transId;
  TBOOL   resetAllCounters; /* TRUE = 1 (Reset the counter values), FALSE = 0 (Get the counter values) */
} MsgPmhSmSctpGenStatReq;

typedef struct s_MsgPmhSmS1apGenStatReq
{
  UINT32  transId;
  TBOOL   resetAllCounters; /* TRUE = 1 (Reset the counter values), FALSE = 0 (Get the counter values) */
} MsgPmhSmS1apGenStatReq;

typedef struct s_MsgPmhSmS1apPeerNodeStatReq
{
  UINT32     transId;
  UINT32     peerId;
  TBOOL      resetAllCounters; /* TRUE = 1 (Reset the counter values), FALSE = 0 (Get the counter values) */
} MsgPmhSmS1apPeerNodeStatReq;

typedef struct s_MsgPmhSmRrcGenStatReq
{
  UINT32  transId;
  TBOOL   resetAllCounters; /* TRUE = 1 (Reset the counter values), FALSE = 0 (Get the counter values) */
} MsgPmhSmRrcGenStatReq;

typedef struct s_MsgPmhSmRrcCellStatReq
{
  UINT32   transId;
  UINT8    cellId;
  TBOOL    resetAllCounters; /* TRUE = 1 (Reset the counter values), FALSE = 0 (Get the counter values) */
} MsgPmhSmRrcCellStatReq;

typedef struct s_MsgPmhUehRrmStatReq
{
  UINT32  transId;
  TBOOL   resetAllCounters; /* TRUE = 1 (Reset the counter values), FALSE = 0 (Get the counter values) */
} MsgPmhUehRrmStatReq;

/******** Response messages *************/
typedef struct s_MacRlcsapCounters
{
  UINT32                      numOfSDURxRlcMacIf;
  UINT32                      numOfSDUTxRlcMacIf;
  UINT32                      numSDUDropRlcMacIf;
} MacRlcsapCounters;

typedef struct s_MacPhysapCounters
{
  UINT32                      numOfPduRxL2L1If;
  UINT32                      numOfPduTxL2L1If;
  UINT32                      numPduDropL2L1If;
} MacPhysapCounters;

typedef struct s_MsgSmPmhMacGenStatRsp
{
  UINT32               transId;
  MacGenCounters       macGenCounters;
} MsgSmPmhMacGenStatRsp;

typedef struct s_MsgSmPmhMacRlcsapStatRsp
{
  UINT32               transId;
  MacRlcsapCounters    macRlcsapCounters ;
} MsgSmPmhMacRlcsapStatRsp;

typedef struct s_MsgSmPmhMacPhysapStatRsp
{
  UINT32               transId;
  MacPhysapCounters    macPhysapCounters ;
} MsgSmPmhMacPhysapStatRsp;

typedef struct s_MsgSmPmhRlcGenStatRsp
{
  UINT32               transId;
  RlcGenCounters       rlcGenCounters ;
} MsgSmPmhRlcGenStatRsp;

typedef struct s_MsgSmPmhPdcpGenStatRsp
{
  UINT32               transId;
  PdcpGenCounters      pdcpGenCounters ;
} MsgSmPmhPdcpGenStatRsp;

typedef struct s_MsgSmPmhPdcpUplaneStatRsp
{
  UINT32               transId;
  PdcpUplaneCounters   pdcpUplaneCounters ;
} MsgSmPmhPdcpUplaneStatRsp;

typedef struct s_MsgSmPmhGtpGenStatRsp
{
  UINT32               transId;
  GtpGenCounters       gtpGenCounters ;
} MsgSmPmhGtpGenStatRsp;

typedef struct s_MsgSmPmhSctpGenStatRsp
{
  UINT32               transId;
  SctpGenCounters      sctpGenCounters ;
} MsgSmPmhSctpGenStatRsp;

typedef struct s_MsgSmPmhS1apGenStatRsp
{
  UINT32               transId;
  S1apGenCounters      s1apGenCounters ;
} MsgSmPmhS1apGenStatRsp;

typedef struct s_MsgSmPmhS1apPeerNodeStatRsp
{
  UINT32                   transId;
  S1apPeerNodeCounters      s1apPeerNodeCounters ;
} MsgSmPmhS1apPeerNodeStatRsp;

typedef struct s_MsgSmPmhRrcGenStatRsp
{
  UINT32               transId;
  RrcGenCounters       rrcGenCounters ;
} MsgSmPmhRrcGenStatRsp;

typedef struct s_MsgSmPmhRrcCellStatRsp
{
  UINT32               transId;
  RrcCellCounters      rrcCellCounters ;
} MsgSmPmhRrcCellStatRsp;

typedef struct s_MsgUehPmhRrmStatRsp
{
  UINT32       transId;
  RrmCounters  rrmCounters;
} MsgUehPmhRrmStatRsp;

#define SAMPLING_INTEVAL_PROFILE1 SAMPINGINTERVAL_10SECS
#define SAMPLING_INTEVAL_PROFILE2 SAMPINGINTERVAL_5SECS

typedef enum e_SamplingInterval
{
  SAMPINGINTERVAL_5SECS  = 5,
  SAMPINGINTERVAL_10SECS = 10,
  SAMPINGINTERVAL_20SECS = 20,
  SAMPINGINTERVAL_30SECS = 30,
  SAMPINGINTERVAL_60SECS = 60
}SamplingInterval;

typedef enum e_MeasurementStopCause
{
  OPSTOP_RECEIVED = 1,
  MEASUREMENT_COMPLETED,
  OPERATOR_INITIATED,
  REPORTING_TIMER_EXPIRED,
  UNEXPECTED_SEQ_NUM_RCVD,
  SAMPLING_TIMER_EXPIRED,
  FAILURE_FROM_L2,
  ENODEB_LOCK
}MeasurementStopCause;

typedef struct s_MsgPmhUehCounterStartReq
{
  UINT32            transId;
  ProfileId         profileId;
  UINT32            numOfSamples;
  SamplingInterval  samplingInterval;
  UINT32            numOfCounters;
  CounterId         counterId[COUNTERID_MAX];
} MsgPmhUehCounterStartReq;

typedef struct s_MsgUehPmhCounterStartSucc
{
  UINT32            transId;
  ProfileId         profileId;
} MsgUehPmhCounterStartSucc;

typedef struct s_MsgUehPmhCounterStartFail
{
  UINT32            transId;
  ProfileId         profileId;
  ErrorDesc         errorDesc;
} MsgUehPmhCounterStartFail;

typedef struct s_MsgPmhUehCounterStopReq
{
  UINT32               transId;
  ProfileId            profileId;
  MeasurementStopCause cause;
} MsgPmhUehCounterStopReq;

typedef struct s_MsgUehPmhCounterStopSucc
{
  UINT32               transId;
  ProfileId            profileId;
  MeasurementStopCause cause;
} MsgUehPmhCounterStopSucc;

typedef struct s_CounterVal
{
  CounterId         counterId;
  UINT32            val;
} CounterVal;

typedef struct s_MsgUehPmhCounterReportInd
{
  ProfileId         profileId;
  UINT32            sampleSeqNum;
  UINT32            numOfCounters;
  CounterVal        counterVal[86];
  UINT8             collectedAt[MAX_SIZE_L2COUNTERS_LASTCOLLECTEDAT + 1 ];

  //TLV               tlv;           /* TODO Need to decide the format */
} MsgUehPmhCounterReportInd;

typedef struct s_MsgUehPmhCounterStopInd
{
  ProfileId               profileId;
  MeasurementStopCause    measurementStopCause;
} MsgUehPmhCounterStopInd;


#endif /* UEH_PMH_IF_H_ */
