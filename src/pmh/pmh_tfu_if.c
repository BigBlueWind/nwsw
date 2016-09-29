#include "ctr_id.h"
#include "basic_types.h"
#include "msg_ids.h"
#include "imc.h"
#include "log.h"
#include "pmh.h"
/**
* @param   cmd STCMD_START or STCMD_STOP 
* @param   interval for reporting
* @details This function creates and sends MSG_PMH_TfU_DSTATS_CFG_REQ to MAC.
* */
void createAndSendTfuDstatsCfgReq(
IN DStatsCmd cmd,
IN UINT32    interval_ms
)
{
  MsgPmhTfuDStatsCfgReq req;

  clearBytes  (&req, sizeof(MsgPmhTfuDStatsCfgReq));
  
  req.cmd = cmd,
  req.interval_ms = interval_ms;
  createAndSendMsg(MSG_PMH_TFU_DSTATS_CFG_REQ, (UINT8 *)&req, sizeof(MsgPmhTfuDStatsCfgReq));
   
}

UINT8 * getIcpuCtrName (
DCounterId id
)
{
   switch (id)
   {
     case  CTRID_MAC_NUM_CRCERR_RX :  return  "MacNumCrcErr_RX";
     case  CTRID_MAC_NUM_RACH_RX   :  return  "MacNumRach_RX";
     case  CTRID_MAC_NUM_RAR_TX    :  return  "MacNumRar_TX";
     case  CTRID_MAC_NUM_MSG3_TX   :  return  "MacNumMsg3_TX ";
     case  CTRID_MAC_NUM_SDU_TX    :  return  "MacNumSdu_TX";
     case  CTRID_MAC_NUM_SDU_RX    :  return  "MacNumSdu_RX";
     case  CTRID_MAC_NUM_HARQ_FAIL :  return  "MacNumHarq_FAIL";

     case  CTRID_RLC_NUM_SDU_RX    :  return  "RlcNumSdu_RX";
     case  CTRID_RLC_NUM_SDU_TX    :  return  "RlcNumSdu_TX";
     case  CTRID_RLC_NUM_SDU_DROP  :  return  "RlcNumSdu_DROP";

     case  CTRID_PDCP_NUM_SDU_RX   :  return  "PdcpNumSdu_RX";
     case  CTRID_PDCP_NUM_SDU_TX   :  return  "PdcpNumSdu_TX"; 
     case  CTRID_PDCP_NUM_SDU_DROP :  return  "PdcpNumSdu_DROP";

     case  CTRID_ICPU_NUM_PKTS_RX  :  return  "IcpuNumPkts_RX";
     case  CTRID_ICPU_NUM_PKTS_TX  :  return  "IcpuNumPkts_TX";

     case  CTRID_MAC_DL_RATE       :  return  "MacDl_RATE";
     case  CTRID_MAC_UL_RATE       :  return  "MacUl_RATE";

     case  CTRID_RLC_DL_RATE       :  return  "RlcDl_RATE";
     case  CTRID_RLC_UL_RATE       :  return  "RlcUl_RATE";  

     case  CTRID_PDCP_DL_RATE      :  return  "PdcpDl_RATE";
     case  CTRID_PDCP_UL_RATE      :  return  "PdcpUl_RATE";

     case  CTRID_TFU_DL_RATE      :  return  "tfuDl_RATE";
     case  CTRID_TFU_UL_RATE      :  return  "tfuUl_RATE";
     case  CTRID_GTPU_QUEUE_LEN   :  return  "GTP_QUEUE_LEN";
     case  CTRID_UDH_QUEUE_LEN    :  return  "UDH_QUEUE_LEN";
     case  CTRID_TUCL_QUEUE_LEN   :  return  "TUCL_QUEUE_LEN";

     default                       :  return "UNKNOWN CTR ID";
  }
}
/**
 * @param   msgIn Message received
 * @param   pmhContext PMH Context
 * @details This function processes MSG_TFU_PMH_DSTATS_IND message receive from CL.
 * */
PmhState pmhProcDstatsInd(
    IN Msg           *msgIn,
    IO PmhContext    *pmhContext
    )
{
  MsgTfuPmhDStatsInd *dStatsInd   = (MsgTfuPmhDStatsInd *)msgIn->data;
  UINT16 index = 0;
  UINT8 *ctrName = NULL;
  UINT32 offset = 0;
  for(index = 0; index < dStatsInd->n;index ++)
  {
     ctrName = getIcpuCtrName(dStatsInd->counterList[index].id);
     offset += snprintf((pmhContext->logBuf + offset), sizeof(pmhContext->logBuf), "%s=%d|",ctrName,dStatsInd->counterList[index].val);
  }
  logInfo("NumCounters:%d",dStatsInd->n);
  logInfo("%s",pmhContext->logBuf);
  return pmhContext->pmhState;
}
