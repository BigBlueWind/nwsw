/**
 * @file    counter_write_functions.c
 * @details Copyright Powerwave Technologies, Inc., 2011
 * @brief This file contains functions which write counters in pmLog file.
 *
 * @author         : Ramprasad Chinta /Ankur Agrawal
 * Creation Date   : 11-Feb-2011
 * Change History  :
 *
 */
#include  "counter_write_functions.h"

TBOOL writeToFileRrmCounterTable(
  IN RrmCounters          *rrmCounters,
  IO FILE                    *fp
  )
{
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "RrmCounters", CONFIG_TYPE_GROUP);

  setting = config_setting_add(group, "index", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrmCounters->index);
  setting = config_setting_add(group, "numRlFailures", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrmCounters->numRlFailures);

  setting = config_setting_add(group, "numS1SetupRequests", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrmCounters->nS1SetupReq);
  setting = config_setting_add(group, "numIncomingHoSuccessful", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrmCounters->numIncomingHoSuccessful);

  setting = config_setting_add(group, "numIncomingHoFailed", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrmCounters->numIncomingHoFailed);
  setting = config_setting_add(group, "numIncomingHoRejected", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrmCounters->numIncomingHoRejected);
  setting = config_setting_add(group, "numS1apRejected", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrmCounters->numS1apRejected);

  setting = config_setting_add(group, "lastCollectedAt", CONFIG_TYPE_STRING);
  config_setting_set_string(setting, (char*)&rrmCounters->lastCollectedAt);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);
  config_destroy(&writeCfg);
  return TRUE;
}

TBOOL writeToFileMacGenCounterTable(
  IN MacGenCounters          *macGenCounters,
  IO FILE                    *fp
  )
{
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "MacGenCounters", CONFIG_TYPE_GROUP);

  setting = config_setting_add(group, "index", CONFIG_TYPE_INT);
  config_setting_set_int(setting, macGenCounters->index);
  setting = config_setting_add(group, "numHarqFail", CONFIG_TYPE_INT);
  config_setting_set_int(setting, macGenCounters->numHarqFail);
  setting = config_setting_add(group, "numUeCfg", CONFIG_TYPE_INT);
  config_setting_set_int(setting, macGenCounters->numUeCfg);
  setting = config_setting_add(group, "numCellCfg", CONFIG_TYPE_INT);
  config_setting_set_int(setting, macGenCounters->numCellCfg);
  setting = config_setting_add(group, "lastCollectedAt", CONFIG_TYPE_STRING);
  config_setting_set_string(setting, (char*)&macGenCounters->lastCollectedAt);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);
  config_destroy(&writeCfg);
  return TRUE;
}

TBOOL writeToFileMacUplaneCounterTable(
  IN MacUplaneCounters          *macUplaneCounters,
  IO FILE                    *fp
  )
{
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "MacUplaneCounters", CONFIG_TYPE_GROUP);

  setting = config_setting_add(group, "index", CONFIG_TYPE_INT);
  config_setting_set_int(setting, macUplaneCounters->index);
  setting = config_setting_add(group, "numOfSDUTxRlcMacIf", CONFIG_TYPE_INT);
  config_setting_set_int(setting, macUplaneCounters->numOfSDUTxRlcMacIf);
  setting = config_setting_add(group, "numOfSDURxRlcMacIf", CONFIG_TYPE_INT);
  config_setting_set_int(setting, macUplaneCounters->numOfSDURxRlcMacIf);
  setting = config_setting_add(group, "numSDUDropRlcMacIf", CONFIG_TYPE_INT);
  config_setting_set_int(setting, macUplaneCounters->numSDUDropRlcMacIf);
  setting = config_setting_add(group, "numOfPduTxL2L1If", CONFIG_TYPE_INT);
  config_setting_set_int(setting, macUplaneCounters->numOfPduTxL2L1If);
  setting = config_setting_add(group, "numOfPduRxL2L1If", CONFIG_TYPE_INT);
  config_setting_set_int(setting, macUplaneCounters->numOfPduRxL2L1If);
  setting = config_setting_add(group, "numPduDropL2L1If", CONFIG_TYPE_INT);
  config_setting_set_int(setting, macUplaneCounters->numPduDropL2L1If);
  setting = config_setting_add(group, "lastCollectedAt", CONFIG_TYPE_STRING);
  config_setting_set_string(setting, (char*)&macUplaneCounters->lastCollectedAt);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);
  config_destroy(&writeCfg);
  return TRUE;
}

/**
 * @param    fileToWrite : This is the full file path of the configuration file which will be written
 * @details   This function will read Cell configuration table and fill default
 *            configurations to file fileToWrite given as input by caller function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *
 **/
TBOOL writeToFileGtpGenCounterTable(
  IN GtpGenCounters          *gtpGenCounters,
  IO FILE                    *fp
  )
{
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "GtpGenCounters", CONFIG_TYPE_GROUP);

  setting = config_setting_add(group, "index", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->index);
  setting = config_setting_add(group, "numTxMsg", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->numTxMsg);
  setting = config_setting_add(group, "numRxMsg", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->numRxMsg);
  setting = config_setting_add(group, "numActvTnls", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->numActvTnls);
  setting = config_setting_add(group, "numErrInd", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->numErrInd);
  setting = config_setting_add(group, "rxDesgMsg", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->rxDesgMsg);
  setting = config_setting_add(group, "invVerRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->invVerRx);
  setting = config_setting_add(group, "invProtTypeRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->invProtTypeRx);
  setting = config_setting_add(group, "invMsgRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->invMsgRx);
  setting = config_setting_add(group, "invMsgLenRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->invMsgLenRx);
  setting = config_setting_add(group, "invIeOrderRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->invIeOrderRx);
  setting = config_setting_add(group, "invIeLenRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->invIeLenRx);
  setting = config_setting_add(group, "invIeFormatRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->invIeFormatRx);
  setting = config_setting_add(group, "invManIeLenRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->invManIeLenRx);
  setting = config_setting_add(group, "missMandIeRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->missMandIeRx);
  setting = config_setting_add(group, "unSuppExtHdrRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->unSuppExtHdrRx);
  setting = config_setting_add(group, "wrngExtHdrRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->wrngExtHdrRx);
  setting = config_setting_add(group, "msgUnableCompRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->msgUnableCompRx);
  setting = config_setting_add(group, "msgUnableCompTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->msgUnableCompTx);
  setting = config_setting_add(group, "missMandIeTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->missMandIeTx);
  setting = config_setting_add(group, "memAllocFldTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->memAllocFldTx);
  setting = config_setting_add(group, "invMsgTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->invMsgTx);
  setting = config_setting_add(group, "invTeidTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->invTeidTx);
  setting = config_setting_add(group, "invTeidRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->invTeidRx);
  setting = config_setting_add(group, "errSeqNoTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->errSeqNoTx);
  setting = config_setting_add(group, "errSeqNoRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->errSeqNoRx);
  setting = config_setting_add(group, "tpduExcdTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->tpduExcdTx);
  setting = config_setting_add(group, "invDatTypeTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, gtpGenCounters->invDatTypeTx);
  setting = config_setting_add(group, "lastCollectedAt", CONFIG_TYPE_STRING);
  config_setting_set_string(setting, (char*)&gtpGenCounters->lastCollectedAt);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);
  config_destroy(&writeCfg);
  return TRUE;
}

/**
 * @param    fileToWrite : This is the full file path of the configuration file which will be written
 * @details   This function will read Cell configuration table and fill default
 *            configurations to file fileToWrite given as input by caller function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *
 **/
TBOOL writeToFileRlcGenCounterTable(
  IN RlcGenCounters          *rlcGenCounters,
  IO FILE                    *fp
  )
{
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "RlcGenCounters", CONFIG_TYPE_GROUP);

  setting = config_setting_add(group, "index", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rlcGenCounters->index);
  setting = config_setting_add(group, "numUeCfg", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rlcGenCounters->numUeCfg);
  setting = config_setting_add(group, "pdusRecv", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rlcGenCounters->pdusRecv);
  setting = config_setting_add(group, "pdusSent", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rlcGenCounters->pdusSent);
  setting = config_setting_add(group, "pdusRetx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rlcGenCounters->pdusRetx);
  setting = config_setting_add(group, "bytesRecv", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rlcGenCounters->bytesRecv);
  setting = config_setting_add(group, "bytesSent", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rlcGenCounters->bytesSent);
  setting = config_setting_add(group, "unexpPdusRecv", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rlcGenCounters->unexpPdusRecv);
  setting = config_setting_add(group, "errorPdusRecv", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rlcGenCounters->errorPdusRecv);
  setting = config_setting_add(group, "protTimeOut", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rlcGenCounters->protTimeOut);
  setting = config_setting_add(group, "numOfRb", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rlcGenCounters->numOfRb);
  setting = config_setting_add(group, "numSduDisc", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rlcGenCounters->numSduDisc);
  setting = config_setting_add(group, "lastCollectedAt", CONFIG_TYPE_STRING);
  config_setting_set_string(setting, (char*)&rlcGenCounters->lastCollectedAt);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);
  config_destroy(&writeCfg);
  return TRUE;
}

/**
 * @param    fileToWrite : This is the full file path of the configuration file which will be written
 * @details   This function will read Cell configuration table and fill default
 *            configurations to file fileToWrite given as input by caller function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *
 **/
TBOOL writeToFilePdcpGenCounterTable(
  IN PdcpGenCounters         *pdcpGenCounters,
  IO FILE                    *fp
  )
{
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "PdcpGenCounters", CONFIG_TYPE_GROUP);

  setting = config_setting_add(group, "index", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpGenCounters->index);
  setting = config_setting_add(group, "rxPdus", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpGenCounters->rxPdus);
  setting = config_setting_add(group, "txPdus", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpGenCounters->txPdus);
  setting = config_setting_add(group, "errorPdusRecv", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpGenCounters->errorPdusRecv);
  setting = config_setting_add(group, "numSdusDisc", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpGenCounters->numSdusDisc);
  setting = config_setting_add(group, "numCmpFails", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpGenCounters->numCmpFails);
  setting = config_setting_add(group, "numDecmpFails", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpGenCounters->numDecmpFails);
  setting = config_setting_add(group, "numIntgPrtFails", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpGenCounters->numIntgPrtFails);
  setting = config_setting_add(group, "numIntgVrfFails", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpGenCounters->numIntgVrfFails);
  setting = config_setting_add(group, "numCiphFails", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpGenCounters->numCiphFails);
  setting = config_setting_add(group, "numDeciphFails", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpGenCounters->numDeciphFails);
  setting = config_setting_add(group, "numPdusDiscObdTmrExp", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpGenCounters->numPdusDiscObdTmrExp);
  setting = config_setting_add(group, "numSdusDiscObdTmrExp", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpGenCounters->numSdusDiscObdTmrExp);
  setting = config_setting_add(group, "numPktsFrwd", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpGenCounters->numPktsFrwd);
  setting = config_setting_add(group, "numPktsRcvd", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpGenCounters->numPktsRcvd);
  setting = config_setting_add(group, "lastCollectedAt", CONFIG_TYPE_STRING);
  config_setting_set_string(setting, (char*)&pdcpGenCounters->lastCollectedAt);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);
  config_destroy(&writeCfg);
  return TRUE;
}

/**
 * @param    fileToWrite : This is the full file path of the configuration file which will be written
 * @details   This function will read Cell configuration table and fill default
 *            configurations to file fileToWrite given as input by caller function.
 *            If error, will fill same in the argument given by caller function and exit from the function.
 *
 **/
TBOOL writeToFilePdcpUplaneCounterTable(
  IN PdcpUplaneCounters      *pdcpUplaneCounters,
  IO FILE                    *fp
  )
{
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "PdcpUplaneCounters", CONFIG_TYPE_GROUP);

  setting = config_setting_add(group, "index", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpUplaneCounters->index);
  setting = config_setting_add(group, "rxSdus", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpUplaneCounters->rxSdus);
  setting = config_setting_add(group, "txSdus", CONFIG_TYPE_INT);
  config_setting_set_int(setting, pdcpUplaneCounters->txSdus);
  setting = config_setting_add(group, "lastCollectedAt", CONFIG_TYPE_STRING);
  config_setting_set_string(setting, (char*)&pdcpUplaneCounters->lastCollectedAt);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);
  config_destroy(&writeCfg);
  return TRUE;
}

TBOOL writeToFileRrcGenCounterTable(
  IN RrcGenCounters            *rrcGenCounters,
  IO FILE                      *fp
  )
{
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "RrcGenCounters", CONFIG_TYPE_GROUP);

  setting = config_setting_add(group, "index", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcGenCounters->index);
  setting = config_setting_add(group, "numEstbCon", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcGenCounters->numEstbCon);
  setting = config_setting_add(group, "numFailCon", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcGenCounters->numFailCon);
  setting = config_setting_add(group, "numReConfigSucc", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcGenCounters->numReConfigSucc);
  setting = config_setting_add(group, "numReConfigFail", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcGenCounters->numReConfigFail);
  setting = config_setting_add(group, "numUnExptMsg", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcGenCounters->numUnExptMsg);
  setting = config_setting_add(group, "numLwLyFail", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcGenCounters->numLwLyFail);
  setting = config_setting_add(group, "numDecErr", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcGenCounters->numDecErr);
  setting = config_setting_add(group, "numEncErr", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcGenCounters->numEncErr);
  setting = config_setting_add(group, "numPdcpErr", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcGenCounters->numPdcpErr);
  setting = config_setting_add(group, "numRlcErr", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcGenCounters->numRlcErr);
  setting = config_setting_add(group, "lastCollectedAt", CONFIG_TYPE_STRING);
  config_setting_set_string(setting, (char*)&rrcGenCounters->lastCollectedAt);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);
  config_destroy(&writeCfg);
  return TRUE;
}
	
TBOOL writeToFileRrcCellCounterTable(
  IN RrcCellCounters            *rrcCellCounters,
  IO FILE                      *fp
  )
{
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "RrcCellCounters", CONFIG_TYPE_GROUP);

  setting = config_setting_add(group, "index", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcCellCounters->index);
  setting = config_setting_add(group, "cellId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcCellCounters->cellId);
  setting = config_setting_add(group, "pag", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcCellCounters->pag);
  setting = config_setting_add(group, "connSetup", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcCellCounters->connSetup);
  setting = config_setting_add(group, "connReject", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcCellCounters->connReject);
  setting = config_setting_add(group, "connReq", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcCellCounters->connReq);
  setting = config_setting_add(group, "connReestabReq", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcCellCounters->connReestabReq);
  setting = config_setting_add(group, "connReestab", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcCellCounters->connReestab);
  setting = config_setting_add(group, "connReestabRej", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcCellCounters->connReestabRej);
  setting = config_setting_add(group, "numUEs", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcCellCounters->numUEs);
  setting = config_setting_add(group, "hoToEUTRASucc", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcCellCounters->hoToEUTRASucc);
  setting = config_setting_add(group, "hoFromEUTRAFail", CONFIG_TYPE_INT);
  config_setting_set_int(setting, rrcCellCounters->hoFromEUTRAFail);
  setting = config_setting_add(group, "lastCollectedAt", CONFIG_TYPE_STRING);
  config_setting_set_string(setting, (char*)&rrcCellCounters->lastCollectedAt);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);
  config_destroy(&writeCfg);
  return TRUE;
}

TBOOL writeToFileSctpGenCounterTable(
  IN SctpGenCounters         *sctpGenCounters,
  IO FILE                    *fp
  )
{
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "SctpGenCounters", CONFIG_TYPE_GROUP);

  setting = config_setting_add(group, "index", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->index);
  setting = config_setting_add(group, "noInitTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noInitTx);
  setting = config_setting_add(group, "noInitReTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noInitReTx);
  setting = config_setting_add(group, "noInitRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noInitRx);
  setting = config_setting_add(group, "noIAckTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noIAckTx);
  setting = config_setting_add(group, "noIAckRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noIAckRx);
  setting = config_setting_add(group, "noShDwnTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noShDwnTx);
  setting = config_setting_add(group, "noShDwnReTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noShDwnReTx);
  setting = config_setting_add(group, "noShDwnRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noShDwnRx);
  setting = config_setting_add(group, "noShDwnAckTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noShDwnAckTx);
  setting = config_setting_add(group, "noShDwnAckReTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noShDwnAckReTx);
  setting = config_setting_add(group, "noShDwnAckRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noShDwnAckRx);
  setting = config_setting_add(group, "noCookieTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noCookieTx);
  setting = config_setting_add(group, "noCookieReTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noCookieReTx);
  setting = config_setting_add(group, "noCookieRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noCookieRx);
  setting = config_setting_add(group, "noCkAckTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noCkAckTx);
  setting = config_setting_add(group, "noCkAckRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noCkAckRx);
  setting = config_setting_add(group, "noDataTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noDataTx);
  setting = config_setting_add(group, "noDataReTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noDataReTx);
  setting = config_setting_add(group, "noDataRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noDataRx);
  setting = config_setting_add(group, "noDAckTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noDAckTx);
  setting = config_setting_add(group, "noDAckRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noDAckRx);
  setting = config_setting_add(group, "noShDwnCmpltTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noShDwnCmpltTx);
  setting = config_setting_add(group, "noShDwnCmpltRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noShDwnCmpltRx);
  setting = config_setting_add(group, "noHBeatTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noHBeatTx);
  setting = config_setting_add(group, "noHBeatRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noHBeatRx);
  setting = config_setting_add(group, "noHBAckTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noHBAckTx);
  setting = config_setting_add(group, "noHBAckRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noHBAckRx);
  setting = config_setting_add(group, "noAbortTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noAbortTx);
  setting = config_setting_add(group, "noAbortRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noAbortRx);
  setting = config_setting_add(group, "noErrorTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noErrorTx);
  setting = config_setting_add(group, "noErrorRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noErrorRx);
  setting = config_setting_add(group, "noEcneTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noEcneTx);
  setting = config_setting_add(group, "noEcneReTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noEcneReTx);
  setting = config_setting_add(group, "noEcneRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noEcneRx);
  setting = config_setting_add(group, "noCwrTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noCwrTx);
  setting = config_setting_add(group, "noCwrRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noCwrRx);
  setting = config_setting_add(group, "noQueryTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noQueryTx);
  setting = config_setting_add(group, "noQueryReTx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noQueryReTx);
  setting = config_setting_add(group, "noQueryRspRx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, sctpGenCounters->noQueryRspRx);
  setting = config_setting_add(group, "lastCollectedAt", CONFIG_TYPE_STRING);
  config_setting_set_string(setting, (char*)&sctpGenCounters->lastCollectedAt);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);
  config_destroy(&writeCfg);

  return TRUE;
}


TBOOL writeToFileS1apGenCounterTable(
  IN S1apGenCounters         *s1apGenCounters,
  IO FILE                    *fp
  )
{
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "S1apGenCounters", CONFIG_TYPE_GROUP);

  setting = config_setting_add(group, "index", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apGenCounters->index);
  setting = config_setting_add(group, "nmbConn", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apGenCounters->nmbConn);
  setting = config_setting_add(group, "nmbFailConn", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apGenCounters->nmbFailConn);
  setting = config_setting_add(group, "maxMemAlloc", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apGenCounters->maxMemAlloc);
  setting = config_setting_add(group, "nmbDecErr", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apGenCounters->nmbDecErr);
  setting = config_setting_add(group, "nmbEncErr", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apGenCounters->nmbEncErr);
  setting = config_setting_add(group, "nmbAudReq", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apGenCounters->nmbAudReq);
  setting = config_setting_add(group, "nmbAudConn", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apGenCounters->nmbAudConn);
  setting = config_setting_add(group, "nmbAudPeer", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apGenCounters->nmbAudPeer);
  setting = config_setting_add(group, "nmbAudFlc", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apGenCounters->nmbAudFlc);
  setting = config_setting_add(group, "lastCollectedAt", CONFIG_TYPE_STRING);
  config_setting_set_string(setting, (char*)&s1apGenCounters->lastCollectedAt);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);
  config_destroy(&writeCfg);

  return TRUE;

}

TBOOL writeToFileS1apPeerNodeCounterTable(
  IN S1apPeerNodeCounters    *s1apPeerNodeCounters,
  IO FILE                    *fp
  )
{
  config_t         writeCfg;
  config_setting_t *root, *setting, *group;
  config_init(&writeCfg); /* init LibConfig */

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "S1apPeerNodeCounters", CONFIG_TYPE_GROUP);

  setting = config_setting_add(group, "index", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->index);
  setting = config_setting_add(group, "peerId", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->peerId);
  setting = config_setting_add(group, "nmbUnexpEvt", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->nmbUnexpEvt);
  setting = config_setting_add(group, "nmbTRafExp", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->nmbTRafExp);
  setting = config_setting_add(group, "eRABSetupReq", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->eRABSetupReq);
  setting = config_setting_add(group, "eRABSetupResp", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->eRABSetupResp);
  setting = config_setting_add(group, "eRABModifyRequest", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->eRABModifyRequest);
  setting = config_setting_add(group, "eRABModResp", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->eRABModResp);
  setting = config_setting_add(group, "eRABRelCmd", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->eRABRelCmd);
  setting = config_setting_add(group, "eRABRelComp", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->eRABRelComp);
  setting = config_setting_add(group, "eRABRelReq", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->eRABRelReq);
  setting = config_setting_add(group, "initCxtSetup", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->initCxtSetup);
  setting = config_setting_add(group, "initCxtSetupResp", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->initCxtSetupResp);
  setting = config_setting_add(group, "initCxtSetupFail", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->initCxtSetupFail);
  setting = config_setting_add(group, "ueCxtRelReq", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->ueCxtRelReq);
  setting = config_setting_add(group, "ueCxtRelCmd", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->ueCxtRelCmd);
  setting = config_setting_add(group, "ueCxtRelComp", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->ueCxtRelComp);
  setting = config_setting_add(group, "ueCxtModReq", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->ueCxtModReq);
  setting = config_setting_add(group, "ueCxtModResp", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->ueCxtModResp);
  setting = config_setting_add(group, "ueCxtModFail", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->ueCxtModFail);
  setting = config_setting_add(group, "initUEMsg", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->initUEMsg);
  setting = config_setting_add(group, "dwnNASTpt", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->dwnNASTpt);
  setting = config_setting_add(group, "upNASTpt", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->upNASTpt);
  setting = config_setting_add(group, "nonDelIndNAS", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->nonDelIndNAS);
  setting = config_setting_add(group, "errIndtx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->errIndtx);
  setting = config_setting_add(group, "errIndrx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->errIndrx);
  setting = config_setting_add(group, "noDatatx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->noDatatx);
  setting = config_setting_add(group, "noDatarx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->noDatarx);
  setting = config_setting_add(group, "resettx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->resettx);
  setting = config_setting_add(group, "resetrx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->resetrx);
  setting = config_setting_add(group, "resetAcktx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->resetAcktx);
  setting = config_setting_add(group, "resetAckrx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->resetAckrx);
  setting = config_setting_add(group, "setupReq", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->setupReq);
  setting = config_setting_add(group, "setupResp", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->setupResp);
  setting = config_setting_add(group, "setupFail", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->setupFail);
  setting = config_setting_add(group, "paging", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->paging);
  setting = config_setting_add(group, "handReqd", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->handReqd);
  setting = config_setting_add(group, "handCmd", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->handCmd);
  setting = config_setting_add(group, "handPrepFail", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->handPrepFail);
  setting = config_setting_add(group, "handReq", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->handReq);
  setting = config_setting_add(group, "handReqAck", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->handReqAck);
  setting = config_setting_add(group, "handFail", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->handFail);
  setting = config_setting_add(group, "handNtfy", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->handNtfy);
  setting = config_setting_add(group, "pathSwchReq", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->pathSwchReq);
  setting = config_setting_add(group, "pathSwchReqAck", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->pathSwchReqAck);
  setting = config_setting_add(group, "handCan", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->handCan);
  setting = config_setting_add(group, "handCanAck", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->handCanAck);
  setting = config_setting_add(group, "eNBStaTrans", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->eNBStaTrans);
  setting = config_setting_add(group, "mmeStaTrans", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->mmeStaTrans);
  setting = config_setting_add(group, "deactTrace", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->deactTrace);
  setting = config_setting_add(group, "traceStart", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->traceStart);
  setting = config_setting_add(group, "traceFailInd", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->traceFailInd);
  setting = config_setting_add(group, "locReportCntrl", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->locReportCntrl);
  setting = config_setting_add(group, "locReportFailInd", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->locReportFailInd);
  setting = config_setting_add(group, "locReport", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->locReport);
  setting = config_setting_add(group, "enbCfgUpd", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->enbCfgUpd);
  setting = config_setting_add(group, "enbCfgUpdAck", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->enbCfgUpdAck);
  setting = config_setting_add(group, "enbCfgUpdFail", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->enbCfgUpdFail);
  setting = config_setting_add(group, "mmeCfgUpd", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->mmeCfgUpd);
  setting = config_setting_add(group, "mmeCfgUpdAck", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->mmeCfgUpdAck);
  setting = config_setting_add(group, "mmeCfgUpdFail", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->mmeCfgUpdFail);
  setting = config_setting_add(group, "upS1CDMA2000Tunn", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->upS1CDMA2000Tunn);
  setting = config_setting_add(group, "dnS1CDMA2000Tunn", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->dnS1CDMA2000Tunn);
  setting = config_setting_add(group, "ueCapInfoInd", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->ueCapInfoInd);
  setting = config_setting_add(group, "overloadStart", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->overloadStart);
  setting = config_setting_add(group, "overloadStop", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->overloadStop);
  setting = config_setting_add(group, "writeRepWarnReq", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->writeRepWarnReq);
  setting = config_setting_add(group, "writeRepWarnRsp", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->writeRepWarnRsp);
  setting = config_setting_add(group, "enbDirInfoTrans", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->enbDirInfoTrans);
  setting = config_setting_add(group, "mmeDirInfoTrans", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->mmeDirInfoTrans);
  setting = config_setting_add(group, "privMsgtx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->privMsgtx);
  setting = config_setting_add(group, "privMsgrx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->privMsgrx);
  setting = config_setting_add(group, "eNBConfigTfr", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->eNBConfigTfr);
  setting = config_setting_add(group, "mmeConfigTfr", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->mmeConfigTfr);
  setting = config_setting_add(group, "cellTrafficTrace", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->cellTrafficTrace);
  setting = config_setting_add(group, "szDedProcStstx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->szDedProcStstx);
  setting = config_setting_add(group, "szDedProcStsrx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->szDedProcStsrx);
  setting = config_setting_add(group, "szComProcStstx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->szComProcStstx);
  setting = config_setting_add(group, "szComProcStsrx", CONFIG_TYPE_INT);
  config_setting_set_int(setting, s1apPeerNodeCounters->szComProcStsrx);
  setting = config_setting_add(group, "lastCollectedAt", CONFIG_TYPE_STRING);
  config_setting_set_string(setting, (char*)&s1apPeerNodeCounters->lastCollectedAt);

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);
  config_destroy(&writeCfg);

  return TRUE;
}

TBOOL writeToFileEthernetCounterTable(
  IN EthernetCounters    *ethernetCounters,
  IO FILE                *fp
  )
{
  config_t         writeCfg;
  config_setting_t *root, *setting, *group, *ethEntry;
  config_init(&writeCfg); /* init LibConfig */
  UINT32  numElem = 0;

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "EthernetCounters", CONFIG_TYPE_GROUP);

  for(numElem = 0; numElem < MAX_ROWS_IN_ETHERNETCOUNTERS_TABLE; ++numElem)
  {
    if(0 EQ ethernetCounters->ifIndex)
      continue;

    /* Create the new entry. */
    ethEntry = config_setting_add(group, NULL, CONFIG_TYPE_GROUP);

    setting = config_setting_add(ethEntry, "ifIndex", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifIndex);

    setting = config_setting_add(ethEntry, "ifDescr", CONFIG_TYPE_STRING);
    config_setting_set_string(setting, (char*)&ethernetCounters->ifDescr);

    setting = config_setting_add(ethEntry, "ifType", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifType);

    setting = config_setting_add(ethEntry, "ifMtu", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifMtu);

    setting = config_setting_add(ethEntry, "ifSpeed", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifSpeed);

    setting = config_setting_add(ethEntry, "ifPhysAddress", CONFIG_TYPE_STRING);
    config_setting_set_string(setting, (char*)&ethernetCounters->ifPhysAddress);

    setting = config_setting_add(ethEntry, "ifAdminStatus", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifAdminStatus);

    setting = config_setting_add(ethEntry, "ifOperStatus", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifOperStatus);

    setting = config_setting_add(ethEntry, "ifLastChange", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifLastChange);

    setting = config_setting_add(ethEntry, "ifInOctets", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifInOctets);

    setting = config_setting_add(ethEntry, "ifInUcastPkts", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifInUcastPkts);

    setting = config_setting_add(ethEntry, "ifInNUcastPkts", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifInNUcastPkts);

    setting = config_setting_add(ethEntry, "ifInDiscards", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifInDiscards);

    setting = config_setting_add(ethEntry, "ifInErrors", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifInErrors);

    setting = config_setting_add(ethEntry, "ifInUnknownProtos", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifInUnknownProtos);

    setting = config_setting_add(ethEntry, "ifOutOctets", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifOutOctets);

    setting = config_setting_add(ethEntry, "ifOutUcastPkts", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifOutUcastPkts);

    setting = config_setting_add(ethEntry, "ifOutNUcastPkts", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifOutNUcastPkts);

    setting = config_setting_add(ethEntry, "ifOutDiscards", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifOutDiscards);

    setting = config_setting_add(ethEntry, "ifOutErrors", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifOutErrors);

    setting = config_setting_add(ethEntry, "ifOutQLen", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ethernetCounters->ifOutQLen);

    setting = config_setting_add(ethEntry, "lastCollectedAt", CONFIG_TYPE_STRING);
    config_setting_set_string(setting, (char*)&ethernetCounters->lastCollectedAt);

    ethernetCounters++;
  }

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);
  config_destroy(&writeCfg);

  return TRUE;
}

TBOOL writeToFileIpCounterTable(
  IN IpCounters    *ipCounters,
  IO FILE          *fp
  )
{
  config_t         writeCfg;
  config_setting_t *root, *setting, *group, *ipEntry;
  config_init(&writeCfg); /* init LibConfig */
  UINT32 numElem;

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "IpCounters", CONFIG_TYPE_GROUP);

  for(numElem = 0; numElem < MAX_ROWS_IN_IPCOUNTERS_TABLE; ++numElem)
  {
    if( IPSYSTEMSTATSIPVERSION_UNKNOWN EQ ipCounters->ipSystemStatsIPVersion)
      continue;

    /* Create the new entry. */
    ipEntry = config_setting_add(group, NULL, CONFIG_TYPE_GROUP);

    setting = config_setting_add(ipEntry, "ipSystemStatsIPVersion", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsIPVersion);

    setting = config_setting_add(ipEntry, "ipSystemStatsInReceives", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsInReceives);

    setting = config_setting_add(ipEntry, "ipSystemStatsInOctets", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsInOctets);

    setting = config_setting_add(ipEntry, "ipSystemStatsInHdrErrors", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsInHdrErrors);

    setting = config_setting_add(ipEntry, "ipSystemStatsInNoRoutes", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsInNoRoutes);

    setting = config_setting_add(ipEntry, "ipSystemStatsInAddrErrors", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsInAddrErrors);

    setting = config_setting_add(ipEntry, "ipSystemStatsInUnknownProtos", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsInUnknownProtos);

    setting = config_setting_add(ipEntry, "ipSystemStatsInTruncatedPkts", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsInTruncatedPkts);

    setting = config_setting_add(ipEntry, "ipSystemStatsInForwDatagrams", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsInForwDatagrams);

    setting = config_setting_add(ipEntry, "ipSystemStatsReasmReqds", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsReasmReqds);

    setting = config_setting_add(ipEntry, "ipSystemStatsReasmOKs", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsReasmOKs);

    setting = config_setting_add(ipEntry, "ipSystemStatsReasmFails", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsReasmFails);

    setting = config_setting_add(ipEntry, "ipSystemStatsInDiscards", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsInDiscards);

    setting = config_setting_add(ipEntry, "ipSystemStatsInDelivers", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsInDelivers);

    setting = config_setting_add(ipEntry, "ipSystemStatsOutRequests", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsOutRequests);

    setting = config_setting_add(ipEntry, "ipSystemStatsOutNoRoutes", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsOutNoRoutes);

    setting = config_setting_add(ipEntry, "ipSystemStatsOutForwDatagrams", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsOutForwDatagrams);

    setting = config_setting_add(ipEntry, "ipSystemStatsOutDiscards", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsOutDiscards);

    setting = config_setting_add(ipEntry, "ipSystemStatsOutFragReqds", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsOutFragReqds);

    setting = config_setting_add(ipEntry, "ipSystemStatsOutFragOKs", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsOutFragOKs);

    setting = config_setting_add(ipEntry, "ipSystemStatsOutFragFails", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsOutFragFails);

    setting = config_setting_add(ipEntry, "ipSystemStatsOutFragCreates", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsOutFragCreates);

    setting = config_setting_add(ipEntry, "ipSystemStatsOutTransmits", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsOutTransmits);

    setting = config_setting_add(ipEntry, "ipSystemStatsOutOctets", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsOutOctets);

    setting = config_setting_add(ipEntry, "ipSystemStatsInMcastPkts", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsInMcastPkts);

    setting = config_setting_add(ipEntry, "ipSystemStatsInMcastOctets", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsInMcastOctets);

    setting = config_setting_add(ipEntry, "ipSystemStatsOutMcastPkts", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsOutMcastPkts);

    setting = config_setting_add(ipEntry, "ipSystemStatsOutMcastOctets", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsOutMcastOctets);

    setting = config_setting_add(ipEntry, "ipSystemStatsInBcastPkts", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsInBcastPkts);

    setting = config_setting_add(ipEntry, "ipSystemStatsOutBcastPkts", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsOutBcastPkts);

    setting = config_setting_add(ipEntry, "ipSystemStatsDiscontinuityTime", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsDiscontinuityTime);

    setting = config_setting_add(ipEntry, "ipSystemStatsRefreshRate", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ipCounters->ipSystemStatsRefreshRate);

    setting = config_setting_add(ipEntry, "lastCollectedAt", CONFIG_TYPE_STRING);
    config_setting_set_string(setting, (char*)&ipCounters->lastCollectedAt);

    ipCounters++;
  }

  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);
  config_destroy(&writeCfg);

  return TRUE;
}

TBOOL pmhWriteToFileL2CountersTable(
  IO L2Counters           *ptrL2Counters,
  IO FILE                 *fp,
  IO UINT32                numOfRows
  )
{
  config_t         writeCfg;
  config_setting_t *root, *setting, *group, *cfgEntry;
  config_init(&writeCfg); /* init LibConfig */

  root = config_root_setting(&writeCfg);
  group = config_setting_add(root, "l2Counters", CONFIG_TYPE_LIST);

  UINT32 numElem = 0;
  for(numElem = 0; numElem < numOfRows; ++numElem)
  {
    /* Create the new entry */
    cfgEntry = config_setting_add(group, NULL, CONFIG_TYPE_GROUP);

    setting = config_setting_add(cfgEntry, "index", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrL2Counters->index);
    setting = config_setting_add(cfgEntry, "counterId", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrL2Counters->counterId);
    setting = config_setting_add(cfgEntry, "val", CONFIG_TYPE_INT);
    config_setting_set_int(setting, ptrL2Counters->value);
    setting = config_setting_add(cfgEntry, "lastCollectedAt", CONFIG_TYPE_STRING);
    config_setting_set_string(setting, (char*)&ptrL2Counters->lastCollectedAt);

    ptrL2Counters++;
  }
  /* Write the file. If there is an error, report it and exit. */
  config_write(&writeCfg, fp);

  config_destroy(&writeCfg);
  return TRUE;
}

