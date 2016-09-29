/**
 * @file    cprm_mt.c
 */

#include "bsm.h"

typedef struct s_CprmMtContext
{
  VersionInfo            versionInfo;
  time_t                 firstTime;       /* Time when first time BSM was started */
  UINT16                 configTransId;   /* Transaction ID for enodeB configuration */
  FilePath               configDir;
} CprmMtContext;

CprmMtContext g_CprmMtContext;

#define  getCprmMtCfTransId()    (g_CprmMtContext.configTransId)

/* Global Variables */
EnodebConfig          enbCfg;
MmeConfig             mmeConfig;
CellConfig            cellCfg;
SchedulerConfig       schCfg;
PhyConfig             phyCfg;
SysInfo1Config        sib1Cfg;
SysInfo2Config        sib2Cfg;
SysInfo3Config        sib3Cfg;
QosConfig             qosCfg;
SysInfo4Config        sib4Cfg;

void cprmMtReadConfigParams(
    UINT8 *rootDir
    )
{
  /*Reading the config files */
  UINT8       fullFilePath_mt[1024] ;
  FilePath     configFilePath;
  UINT8 isError_enb_MT = FALSE;

  sprintf( configFilePath, "%s/config/", rootDir );
  memset( &fullFilePath_mt, 0, sizeof(fullFilePath_mt));

  constructFullPath ( configFilePath, "enb_user_config.cfg", fullFilePath_mt );

  readFromFileEnodebConfigTable(fullFilePath_mt, &enbCfg,&isError_enb_MT);
  readFromFileCellConfigTable(fullFilePath_mt,&cellCfg, &isError_enb_MT);
  readFromFileMmeConfigTable(fullFilePath_mt,&mmeConfig, &isError_enb_MT);
  readFromFilePhyConfigTable(fullFilePath_mt,&phyCfg, &isError_enb_MT);
  readFromFileSchedulerConfigTable(fullFilePath_mt,&schCfg, &isError_enb_MT);
  readFromFileSysInfo1ConfigTable(fullFilePath_mt,&sib1Cfg, &isError_enb_MT);
  readFromFileSysInfo2ConfigTable(fullFilePath_mt,&sib2Cfg, &isError_enb_MT);
  readFromFileSysInfo3ConfigTable(fullFilePath_mt,&sib3Cfg, &isError_enb_MT);
  readFromFileQosConfigTable(fullFilePath_mt,&qosCfg, &isError_enb_MT);
}

void cprmMtExpMsg(
    MsgId    msgId
    )
{
  Msg     *msgIn;
  UINT16   ec;

  while(2)
  {
    msgIn = recvMsg(MODID_BSM, &ec);
    if(msgIn != NULL)
    {
      if( msgId != msgIn->msgId )
      {
        logError("Expected Message=%s Received Message=%s",
            getMsgName(msgId), getMsgName(msgIn->msgId) );
        exit(1);
      }

      logInfo("Received Message=%s", getMsgName(msgIn->msgId) );
      freeMsgIn(msgIn);
      break;
    }
  }
}
/**
 * @details This function will fill and send MSG_BSM_UEH_ENB_CONFIG_REQ message to UEH
 */
void cprmMtFillAndSendUehEnbCfgReqMsg()
{
  MsgBsmUehEnbConfigReq   configReq;
  clearBytes( &configReq, sizeof(MsgBsmUehEnbConfigReq) );

  configReq.transId = getCprmMtCfTransId();
  configReq.enodebId = enbCfg.enodebId;

  copyBytes(&cellCfg, sizeof(CellConfig), &configReq.cellCfg);
  copyBytes(&schCfg, sizeof(SchedulerConfig), &configReq.schCfg);
  copyBytes(&phyCfg, sizeof(PhyConfig), &configReq.phyCfg);
  copyBytes(&sib1Cfg, sizeof(SysInfo1Config), &configReq.sib1Cfg);
  copyBytes(&sib2Cfg, sizeof(SysInfo2Config), &configReq.sib2Cfg);
  copyBytes(&sib3Cfg, sizeof(SysInfo3Config), &configReq.sib3Cfg);
  copyBytes(&qosCfg, sizeof(QosConfig), &configReq.qosCfg);

  configReq.sib4Cfg.intraFreqNeighCellQOffsetCell = 15;
  configReq.sib4Cfg.neighEutraPhyCellId = 0;
  createAndSendMsg( MSG_BSM_UEH_ENB_CONFIG_REQ, (UINT8*)&configReq, sizeof(MsgBsmUehEnbConfigReq));

  cprmMtExpMsg( MSG_SCTP_BSM_INIT_CONFIG_SUCC );
  cprmMtExpMsg( MSG_S1AP_BSM_INIT_CONFIG_SUCC );
  cprmMtExpMsg( MSG_EGTP_BSM_INIT_CONFIG_SUCC );
  cprmMtExpMsg( MSG_S1AP_BSM_CONFIG_SUCC );
  cprmMtExpMsg( MSG_UEH_BSM_ENB_CONFIG_SUCC );
}

/**
 * @details This function will fill and send MSG_BSM_SCTP_CONFIG_REQ message to UEH
 */
void cprmMtFillAndSendSctpCfgReqMsg()
{
	MsgBsmSctpInitConfigReq   configReq;
  clearBytes( &configReq, sizeof(MsgBsmSctpInitConfigReq) );

  configReq.transId =  getCprmMtCfTransId();
  copyBytes( &enbCfg.enodebStaticIpAddr, sizeof(enbCfg.enodebStaticIpAddr), &configReq.enodebIpAddr );

  createAndSendMsg( MSG_BSM_SCTP_INIT_CONFIG_REQ, (UINT8*)&configReq, sizeof(MsgBsmSctpInitConfigReq) );

}

/**
 * @details This function will fill and send MSG_BSM_S1AP_CONFIG_REQ message to S1AP
 */
void cprmMtFillAndSendS1apCfgReqMsg()
{
  MsgBsmS1apConfigReq   configReq;
  clearBytes( &configReq, sizeof(MsgBsmS1apConfigReq) );

  configReq.transId = getCprmMtCfTransId();

  //configReq.enbConfig = enbCfg;
  configReq.enodebId = enbCfg.enodebId;
  snprintf( configReq.enodebName, (strlen(enbCfg.enodebName)+1), "%s", enbCfg.enodebName ); 
  configReq.mmeConfig = mmeConfig;
  configReq.tac = cellCfg.tac;
  configReq.maxConnectedUes = cellCfg.maxConnectedUes;
  configReq.defaultPagCycle = sib2Cfg.defaultPagCycle;
  copyBytes( &cellCfg.plmnMCC, sizeof(cellCfg.plmnMCC), &configReq.plmnMCC );
  copyBytes( &cellCfg.plmnMNC, sizeof(cellCfg.plmnMNC), &configReq.plmnMNC );

  createAndSendMsg( MSG_BSM_S1AP_CONFIG_REQ, (UINT8*)&configReq, sizeof(MsgBsmS1apConfigReq));
 }

/**
 * @details This function will fill and send MSG_BSM_EGTP_CONFIG_REQ message to EGTP
 */
void cprmMtFillAndSendS1InitReqMsg()
{

  MsgBsmS1apInitConfigReq   configReq;
  clearBytes( &configReq, sizeof(MsgBsmS1apInitConfigReq) );

  configReq.transId = getCprmMtCfTransId();
  copyBytes( &enbCfg.enodebStaticIpAddr, sizeof(enbCfg.enodebStaticIpAddr), &configReq.enodebStaticIpAddr );
  snprintf( configReq.enodebNetmaskAddr, (strlen(enbCfg.enodebNetmaskAddr)+1), "%s", enbCfg.enodebNetmaskAddr);
  createAndSendMsg( MSG_BSM_S1AP_INIT_CONFIG_REQ, (UINT8*)&configReq, sizeof(MsgBsmS1apInitConfigReq) );
}

/**
 * @details This function will fill and send MSG_BSM_EGTP_CONFIG_REQ message to EGTP
 */
void cprmMtFillAndSendEgtpCfgReqMsg()
{
  MsgBsmEgtpInitConfigReq  configReq;
  clearBytes( &configReq, sizeof(MsgBsmEgtpInitConfigReq) );

  configReq.transId = getCprmMtCfTransId();
  copyBytes( &enbCfg.enodebStaticIpAddr, sizeof(enbCfg.enodebStaticIpAddr), &configReq.enodebIpAddr );

  createAndSendMsg( MSG_BSM_EGTP_INIT_CONFIG_REQ, (UINT8*)&configReq, sizeof(MsgBsmS1apInitConfigReq) );
}

void cprmMtFillAndSendS1OpStart()
{
  createAndSendMsg( MSG_BSM_S1AP_OPSTART_REQ, NULL, 0);
  cprmMtExpMsg( MSG_S1AP_BSM_LINK_UP_IND);
  cprmMtExpMsg( MSG_S1AP_BSM_OPSTART_SUCC );
}

void cprmMtFillAndSendUehOpStart()
{
  createAndSendMsg( MSG_BSM_UEH_OPSTART_REQ, NULL, 0);
  cprmMtExpMsg( MSG_UEH_BSM_OPSTART_SUCC );
}

void cprmMtFillAndSendS1OpStop()
{
  createAndSendMsg( MSG_BSM_S1AP_OPSTOP_REQ, NULL, 0);
  cprmMtExpMsg( MSG_S1AP_BSM_OPSTOP_SUCC );
}

void cprmMtFillAndSendUehOpStop()
{
  createAndSendMsg( MSG_BSM_UEH_OPSTOP_REQ, NULL, 0);
  cprmMtExpMsg( MSG_UEH_BSM_OPSTOP_SUCC );
}

void cprmMtUeActiveStatus()
{
  MsgBsmUehActiveUeStatusReq ueStatusReq;

  ueStatusReq.cmdRef.origin = 1;
  ueStatusReq.cmdRef.cmdRefNum = 2;
  ueStatusReq.cmdRef.instanceId = 3;

  createAndSendMsg(MSG_BSM_UEH_GET_ACTIVE_UE_STATUS_REQ, &ueStatusReq, sizeof(MsgBsmUehActiveUeStatusReq));
  cprmMtExpMsg(MSG_UEH_BSM_GET_ACTIVE_UE_STATUS_RSP);
}

void cprmMtRabActiveStatus()
{
  MsgBsmUdhActiveRabStatusReq rabStatusReq;

  rabStatusReq.cmdRef.origin = 1;
  rabStatusReq.cmdRef.cmdRefNum = 2;
  rabStatusReq.cmdRef.instanceId = 3;

  rabStatusReq.internalUeIdentity = 0;

  createAndSendMsg(MSG_BSM_UDH_GET_ACTIVE_RAB_STATUS_REQ, &rabStatusReq, sizeof(MsgBsmUdhActiveRabStatusReq));
  cprmMtExpMsg(MSG_UDH_BSM_GET_ACTIVE_RAB_STATUS_RSP);
}
void cprmMtStartEnb()
{

  cprmMtFillAndSendSctpCfgReqMsg();
  cprmMtFillAndSendS1InitReqMsg();
  cprmMtFillAndSendEgtpCfgReqMsg();
  cprmMtFillAndSendS1apCfgReqMsg();
  cprmMtFillAndSendUehEnbCfgReqMsg();

  /* Let CPRM configure all the CCPU Layers */
  sleep(5);
  cprmMtFillAndSendS1OpStart();
  cprmMtFillAndSendUehOpStart();
}


void cprmMtStopEnb()
{
  cprmMtFillAndSendUehOpStop();
  sleep(5); /* Let S1 Go down */
  cprmMtFillAndSendS1OpStop();
}

int main(int argc, char *argv[])
{
  UINT8     status;
  UINT16    option;
  UINT8    *exeName = argv[0];
  TimeSpec  tick = { TIMEUNIT_SEC, 1 };

  /* Clear Error chain buffer */
  clearBytes( getModuleContext(), sizeof(ModContext) );

  /* Handle command line arguments before modInit.
   * This allows a user to know the version of the executable even if modInit fails.
   */
  clearBytes(&g_CprmMtContext.configDir[0], sizeof(Filename));
  cmdLineParamHandler( argc, argv, &g_CprmMtContext.configDir[0], &g_CprmMtContext.versionInfo );

  status = modInit( MODID_BSM, exeName, (char*)argv[2], tick );

  if(TRUE NOTEQ status)
  {
    logError("Could not register CPRM_MT Module, Status returned=%d", status);
    emptyErrorChain(&(getModuleContext()->errChain));
    exit(EXIT_FAILURE);
  }

  cprmMtReadConfigParams( argv[2]);

  do
  {
    printf("\n Enter Option ( 0- Exit, 1- Start Enb, 2- Stop Enb 3- UeStatus 4- RabStatus):\n");
    scanf("%d", &option);

    switch( option )
    {
      case 0:
         exit(0);
         break;
      case 1:
        cprmMtStartEnb();
        break;
      case 2:
        cprmMtStopEnb();
        break;
      case 3:
        cprmMtUeActiveStatus();
        break;
      case 4:
        cprmMtRabActiveStatus();
        break;
      default:
        printf("Please enter a valid option\n");
        break;
    }
  }while(1);

  printAndEmptyErrorChain(&(getModuleContext()->errChain));
  return 0;
}

