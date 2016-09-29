/**
 * @file    set_opr_cfg_func.c
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This is auto generated file from Excel
 * @section This file contains SET functions for all tables
 *
 * Author: Madhusudana Rao Kata
 * Date: 11-10-2010
 * Description: Draft version 
 * Generated on : 11-02-2013 14:40:18
 */

 
#include "set_oam_func.h"

/*******************************************************************/
/***************    OPERATOR CONFIG TABLE SET FUNCTIONS     ********/
/*******************************************************************/

/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills enbRegistrationConfigstructure.
 * */
void tlv2StructEnbRegistrationConfigTable(
  IO TlvArray           *tlvArray,
  IO EnbRegistrationConfig
                        *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_ENBREGISTRATIONCONFIG_BSID:
        config->bsId  =  getUint32FromArray(tlv->data);
        break;
      case COL_ENBREGISTRATIONCONFIG_PUBLICDNSSERVER1:
        snprintf( config->publicDNSServer1, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBREGISTRATIONCONFIG_PUBLICDNSSERVER2:
        snprintf( config->publicDNSServer2, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBREGISTRATIONCONFIG_PRIVATEDNSSERVER1:
        snprintf( config->privateDNSServer1, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBREGISTRATIONCONFIG_PRIVATEDNSSERVER2:
        snprintf( config->privateDNSServer2, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBREGISTRATIONCONFIG_SECURITYGW1:
        snprintf( config->securityGW1, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBREGISTRATIONCONFIG_SECURITYGW2:
        snprintf( config->securityGW2, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBREGISTRATIONCONFIG_EMSADDR:
        snprintf( config->emsAddr, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBREGISTRATIONCONFIG_CONFIGUREDLATITUDE:
        snprintf( config->configuredLatitude, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBREGISTRATIONCONFIG_CONFIGUREDLONGITUDE:
        snprintf( config->configuredLongitude, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBREGISTRATIONCONFIG_SNMPCOMMUNITYSTRING:
        snprintf( config->snmpCommunityString, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBREGISTRATIONCONFIG_TIMEANDLOCATIONSOURCETOUSE:
        config->timeAndLocationSourceToUse  =  getUint32FromArray(tlv->data);
        break;
      case COL_ENBREGISTRATIONCONFIG_INITIALLOCALTIME:
        snprintf( config->initialLocalTime, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBREGISTRATIONCONFIG_INITIALLOCALTIMEOFFSETFROMUTC:
        snprintf( config->initialLocalTimeOffsetFromUTC, (tlv->len+1), "%s", tlv->data ); 
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills enodebConfigstructure.
 * */
void tlv2StructEnodebConfigTable(
  IO TlvArray           *tlvArray,
  IO EnodebConfig       *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_ENODEBCONFIG_ENODEBID:
        config->enodebId  =  getUint32FromArray(tlv->data);
        break;
      case COL_ENODEBCONFIG_ENODEBNAME:
        snprintf( config->enodebName, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENODEBCONFIG_USESTATICIPADDRESS:
        config->useStaticIpAddress  =  getUint8FromArray(tlv->data);
        break;
      case COL_ENODEBCONFIG_ENODEBSTATICIPADDR:
        snprintf( config->enodebStaticIpAddr, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENODEBCONFIG_ENODEBNETMASKADDR:
        snprintf( config->enodebNetmaskAddr, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENODEBCONFIG_ENODEBREGISTRATIONACCEPTED:
        config->enodebRegistrationAccepted  =  getUint8FromArray(tlv->data);
        break;
      case COL_ENODEBCONFIG_CONFIGCHANGEACKBYEMS:
        config->configChangeAckByEms  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills mmeConfigstructure.
 * */
void tlv2StructMmeConfigTable(
  IO TlvArray           *tlvArray,
  IO MmeConfig          *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_MMECONFIG_INDEX:
        config->index  =  getUint32FromArray(tlv->data);
        break;
      case COL_MMECONFIG_MMEIPADDR:
        snprintf( config->mmeIpAddr, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_MMECONFIG_ENABLESECUREMMELINK:
        config->enableSecureMmeLink  =  getUint8FromArray(tlv->data);
        break;
      case COL_MMECONFIG_ENABLESECUREGTPTUNNELS:
        config->enableSecureGtpTunnels  =  getUint8FromArray(tlv->data);
        break;
      case COL_MMECONFIG_NUMSCTPSTREAMS:
        config->numSctpStreams  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills cellConfigstructure.
 * */
void tlv2StructCellConfigTable(
  IO TlvArray           *tlvArray,
  IO CellConfig         *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_CELLCONFIG_CELLID:
        config->cellId  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_CELLTYPE:
        config->cellType  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_PLMNMCC:
        snprintf( config->plmnMCC, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_CELLCONFIG_PLMNMNC:
        snprintf( config->plmnMNC, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_CELLCONFIG_TAC:
        config->tac  =  getUint16FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_FREQBANDIND:
        config->freqBandInd  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_DLBANDWIDTH:
        config->dlBandwidth  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_DLEARFCN:
        config->dlEarfcn  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_ULBANDWIDTH:
        config->ulBandwidth  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_ULEARFCN:
        config->ulEarfcn  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_CELLRESERVEDFOROPERATORUSE:
        config->cellReservedForOperatorUse  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_MAXCONNECTEDUES:
        config->maxConnectedUes  =  getUint16FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_ACBARRINGEMERGENCY:
        config->acBarringEmergency  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_ACBARRMOSIGENABLED:
        config->acBarrMoSigEnabled  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_ACBARRMOSIGFACTOR:
        config->acBarrMoSigFactor  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_ACBARRMOSIGTIME:
        config->acBarrMoSigTime  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_ACBARRMOSIGSPECIALAC:
        snprintf( config->acBarrMoSigSpecialAC, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_CELLCONFIG_ACBARRMODATAENABLED:
        config->acBarrMoDataEnabled  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_ACBARRMODATAFACTOR:
        config->acBarrMoDataFactor  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_ACBARRMODATATIME:
        config->acBarrMoDataTime  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_ACBARRMODATASPECIALAC:
        snprintf( config->acBarrMoDataSpecialAC, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_CELLCONFIG_STARTRNTI:
        config->startRnti  =  getUint16FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_RSRPFILTERCOEFFICIENT:
        config->rsrpFilterCoefficient  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_SCHEDULERPROFILEID:
        config->schedulerProfileId  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_PHYPROFILEID:
        config->phyProfileId  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_PHYLAYERID:
        config->phyLayerId  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_PHYCELLIDENTITYGROUP:
        config->phyCellIdentityGroup  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_PMAX:
        config->pMax  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_UERESPONSETIMER:
        config->ueResponseTimer  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_RRCCONNREJWAITTIME:
        config->rrcConnRejWaitTime  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_UEINACTIVITYCHECKINTERVALSECS:
        config->ueInactivityCheckIntervalSecs  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_SYSINFO1PROFILEID:
        config->sysInfo1ProfileId  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_SYSINFO2PROFILEID:
        config->sysInfo2ProfileId  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_SYSINFO3PROFILEID:
        config->sysInfo3ProfileId  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_SIRETXCNT:
        config->siReTxCnt  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_ANTINFOTXMODE:
        config->antInfoTxMode  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_INITIALULCQI:
        config->initialUlCqi  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_PUCCHRESOURCESIZE:
        config->pucchResourceSize  =  getUint8FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_CQIPERIODICITYIDX:
        config->cqiPeriodicityIdx  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_SRIPERIODICITYIDX:
        config->sriPeriodicityIdx  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_UECQIBASEDRLFDETECTIONTIMER:
        config->ueCqiBasedRlfDetectionTimer  =  getUint32FromArray(tlv->data);
        break;
      case COL_CELLCONFIG_UECFGMACCOLLATEDCQICOUNT:
        config->ueCfgMacCollatedCqiCount  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills schedulerConfigstructure.
 * */
void tlv2StructSchedulerConfigTable(
  IO TlvArray           *tlvArray,
  IO SchedulerConfig    *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_SCHEDULERCONFIG_SCHEDULERPROFILEID:
        config->schedulerProfileId  =  getUint8FromArray(tlv->data);
        break;
      case COL_SCHEDULERCONFIG_SCHEDULINGALGORITHM:
        config->schedulingAlgorithm  =  getUint32FromArray(tlv->data);
        break;
      case COL_SCHEDULERCONFIG_MAXDLHARQTX:
        config->maxDlHarqTx  =  getUint8FromArray(tlv->data);
        break;
      case COL_SCHEDULERCONFIG_MAXULHARQTX:
        config->maxUlHarqTx  =  getUint32FromArray(tlv->data);
        break;
      case COL_SCHEDULERCONFIG_MAXMSG4HARQTX:
        config->maxMsg4HarqTx  =  getUint8FromArray(tlv->data);
        break;
      case COL_SCHEDULERCONFIG_PCFICHCFI:
        config->pcfichCfi  =  getUint8FromArray(tlv->data);
        break;
      case COL_SCHEDULERCONFIG_PERIODICPHRTIMER:
        config->periodicPhrTimer  =  getUint32FromArray(tlv->data);
        break;
      case COL_SCHEDULERCONFIG_PROHIBITPHRTIMER:
        config->prohibitPhrTimer  =  getUint32FromArray(tlv->data);
        break;
      case COL_SCHEDULERCONFIG_DLPATHLOSSCHANGE:
        config->dlPathlossChange  =  getUint32FromArray(tlv->data);
        break;
      case COL_SCHEDULERCONFIG_PERIODICBSRTIMER:
        config->periodicBsrTimer  =  getUint32FromArray(tlv->data);
        break;
      case COL_SCHEDULERCONFIG_RETXBSRTIMER:
        config->retxBsrTimer  =  getUint32FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills phyConfigstructure.
 * */
void tlv2StructPhyConfigTable(
  IO TlvArray           *tlvArray,
  IO PhyConfig          *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_PHYCONFIG_PHYPROFILEID:
        config->phyProfileId  =  getUint8FromArray(tlv->data);
        break;
      case COL_PHYCONFIG_PHICHRESOURCE:
        config->phichResource  =  getUint32FromArray(tlv->data);
        break;
      case COL_PHYCONFIG_PRACHFREQOFFSET:
        config->prachFreqOffset  =  getUint8FromArray(tlv->data);
        break;
      case COL_PHYCONFIG_REFERENCESIGNALPOWER:
        config->referenceSignalPower  =  getUint8FromArray(tlv->data);
        break;
      case COL_PHYCONFIG_CELLPHYPB:
        config->cellPhyPb  =  getUint8FromArray(tlv->data);
        break;
      case COL_PHYCONFIG_SRSENABLED:
        config->srsEnabled  =  getUint8FromArray(tlv->data);
        break;
      case COL_PHYCONFIG_ALPHA:
        config->alpha  =  getUint32FromArray(tlv->data);
        break;
      case COL_PHYCONFIG_PONOMINALPUCCH:
        config->poNominalPucch  =  getUint8FromArray(tlv->data);
        break;
      case COL_PHYCONFIG_CELLPHYPA:
        config->cellPhyPa  =  getUint32FromArray(tlv->data);
        break;
      case COL_PHYCONFIG_BETAOFFSETACKINDEX:
        config->betaOffsetAckIndex  =  getUint8FromArray(tlv->data);
        break;
      case COL_PHYCONFIG_BETAOFFSETRIINDEX:
        config->betaOffsetRiIndex  =  getUint8FromArray(tlv->data);
        break;
      case COL_PHYCONFIG_BETAOFFSETCQIINDEX:
        config->betaOffsetCqiIndex  =  getUint8FromArray(tlv->data);
        break;
      case COL_PHYCONFIG_POUEPUSCH:
        config->poUePusch  =  getUint8FromArray(tlv->data);
        break;
      case COL_PHYCONFIG_POUEPUCCH:
        config->poUePucch  =  getUint8FromArray(tlv->data);
        break;
      case COL_PHYCONFIG_PSRSOFFSET:
        config->pSrsOffset  =  getUint8FromArray(tlv->data);
        break;
      case COL_PHYCONFIG_PHICHDURATION:
        config->phichDuration  =  getUint32FromArray(tlv->data);
        break;
      case COL_PHYCONFIG_TOTALTXPOWER:
        config->totalTxPower  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills sysInfo1Configstructure.
 * */
void tlv2StructSysInfo1ConfigTable(
  IO TlvArray           *tlvArray,
  IO SysInfo1Config     *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_SYSINFO1CONFIG_SYSINFO1PROFILEID:
        config->sysInfo1ProfileId  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO1CONFIG_INTRAFREQRESELECTION:
        config->intraFreqReselection  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO1CONFIG_QRXLEVMIN:
        config->qRxLevMin  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO1CONFIG_QRXLEVMINOFFSET:
        config->qRxLevMinOffset  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO1CONFIG_SIB2PERIODICITY:
        config->sib2Periodicity  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO1CONFIG_SIB3PERIODICITY:
        config->sib3Periodicity  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO1CONFIG_SIB4PERIODICITY:
        config->sib4Periodicity  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO1CONFIG_SIWINDOWLENGTH:
        config->siWindowLength  =  getUint32FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills sysInfo2Configstructure.
 * */
void tlv2StructSysInfo2ConfigTable(
  IO TlvArray           *tlvArray,
  IO SysInfo2Config     *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_SYSINFO2CONFIG_SYSINFO2PROFILEID:
        config->sysInfo2ProfileId  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_NUMRAPREAMBLES:
        config->numRaPreambles  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_SIZERAPREAMBLESGROUPA:
        config->sizeRAPreamblesGroupA  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_MESSAGESIZEGROUPA:
        config->messageSizeGroupA  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_MESSAGEPOWEROFFSETGROUPB:
        config->messagePowerOffsetGroupB  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_POWERRAMPINGSTEP:
        config->powerRampingStep  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_PREAMBLEINITRCVDTGTPOWER:
        config->preambleInitRcvdTgtPower  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_PREAMBLETRANSMAX:
        config->preambleTransMax  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_RARSPWINDOWSIZE:
        config->raRspWindowSize  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_MACCONTRESOLUTIONTIMER:
        config->macContResolutionTimer  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_MAXHARQMSG3TX:
        config->maxHarqMsg3Tx  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_MODIFICATIONPERIODCOEFF:
        config->modificationPeriodCoeff  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_DEFAULTPAGCYCLE:
        config->defaultPagCycle  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_NB:
        config->nB  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_ROOTSEQUENCEINDEX:
        config->rootSequenceIndex  =  getUint16FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_PRACHCONFIGINDEX:
        config->prachConfigIndex  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_HIGHSPEEDFLAG:
        config->highSpeedFlag  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_ZEROCORRELATIONZONECONFIG:
        config->zeroCorrelationZoneConfig  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_PUSCHFREQHOPPINGSUBBANDS:
        config->puschFreqHoppingSubBands  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_PUSCHFREQHOPPINGMODE:
        config->puschFreqHoppingMode  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_PUSCHFREQHOPPINGOFFSET:
        config->puschFreqHoppingOffset  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_ULRSGROUPHOPPINGENABLED:
        config->ulRsGroupHoppingEnabled  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_ULRSGROUPPUSCH:
        config->ulRsGroupPusch  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_ULRSSEQHOPPINGENABLED:
        config->ulRsSeqHoppingEnabled  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_ULRSCYCLICSHIFTPUSCH:
        config->ulRsCyclicShiftPusch  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_DELTAPUCCHSHIFT:
        config->deltaPucchShift  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_N1PUCCHAN:
        config->n1PucchAn  =  getUint16FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_SRSBANDWIDTHCONFIG:
        config->srsBandwidthConfig  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_SRSSUBFRMCONFIG:
        config->srsSubfrmConfig  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_SRSSIMULTANEOUSANSRSTX:
        config->srsSimultaneousAnSrsTx  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_PONOMINALPUSCH:
        config->poNominalPusch  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_DELTAFPUCCHFORMAT1:
        config->deltaFPucchFormat1  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_DELTAFPUCCHFORMAT1B:
        config->deltaFPucchFormat1b  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_DELTAFPUCCHFORMAT2:
        config->deltaFPucchFormat2  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_DELTAFPUCCHFORMAT2A:
        config->deltaFPucchFormat2a  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_DELTAFPUCCHFORMAT2B:
        config->deltaFPucchFormat2b  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_DELTAPREAMBLEMSG3:
        config->deltaPreambleMsg3  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_T300:
        config->t300  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_T301:
        config->t301  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_T310:
        config->t310  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_N310:
        config->n310  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_T311:
        config->t311  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_N311:
        config->n311  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_ADDITIONALSPECTRUMEMISSION:
        config->additionalSpectrumEmission  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_TIMEALIGNMENTTIMER:
        config->timeAlignmentTimer  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_NRBCQI:
        config->nRbCqi  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_ENABLE64QAM:
        config->enable64QAM  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO2CONFIG_NCSAN:
        config->nCsAn  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills logConfigstructure.
 * */
void tlv2StructLogConfigTable(
  IO TlvArray           *tlvArray,
  IO LogConfig          *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_LOGCONFIG_MODULEID:
        config->moduleId  =  getUint32FromArray(tlv->data);
        break;
      case COL_LOGCONFIG_LOGMASK1:
        config->logMask1  =  getUint32FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills radioHeadConfigstructure.
 * */
void tlv2StructRadioHeadConfigTable(
  IO TlvArray           *tlvArray,
  IO RadioHeadConfig    *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_RADIOHEADCONFIG_RADIOHEADPROFILEID:
        config->radioHeadProfileId  =  getUint32FromArray(tlv->data);
        break;
      case COL_RADIOHEADCONFIG_TXATTENPATHA:
        config->txAttenPathA  =  getUint16FromArray(tlv->data);
        break;
      case COL_RADIOHEADCONFIG_TXATTENPATHB:
        config->txAttenPathB  =  getUint16FromArray(tlv->data);
        break;
      case COL_RADIOHEADCONFIG_TXDELAY:
        config->txDelay  =  getUint32FromArray(tlv->data);
        break;
      case COL_RADIOHEADCONFIG_RXDELAY:
        config->rxDelay  =  getUint32FromArray(tlv->data);
        break;
      case COL_RADIOHEADCONFIG_VSWRSHUTDOWNALARMTHRESHOLD:
        config->vswrShutdownAlarmThreshold  =  getUint8FromArray(tlv->data);
        break;
      case COL_RADIOHEADCONFIG_OVERPOWERALARMTHRESHOLD:
        config->overpowerAlarmThreshold  =  getUint32FromArray(tlv->data);
        break;
      case COL_RADIOHEADCONFIG_LOWPOWERALARMTHRESHOLD:
        config->lowpowerAlarmThreshold  =  getUint32FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills qosConfigstructure.
 * */
void tlv2StructQosConfigTable(
  IO TlvArray           *tlvArray,
  IO QosConfig          *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_QOSCONFIG_QOSPROFILEID:
        config->qosProfileId  =  getUint32FromArray(tlv->data);
        break;
      case COL_QOSCONFIG_QOSPROFILENAME:
        snprintf( config->qosProfileName, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_QOSCONFIG_QCI:
        config->qci  =  getUint8FromArray(tlv->data);
        break;
      case COL_QOSCONFIG_RLCMODE:
        config->rlcMode  =  getUint32FromArray(tlv->data);
        break;
      case COL_QOSCONFIG_AMRLCTPOLLRETRANSMIT:
        config->amRlcTPollRetransmit  =  getUint32FromArray(tlv->data);
        break;
      case COL_QOSCONFIG_AMRLCPOLLPDU:
        config->amRlcPollPDU  =  getUint32FromArray(tlv->data);
        break;
      case COL_QOSCONFIG_AMRLCPOLLBYTE:
        config->amRlcPollByte  =  getUint32FromArray(tlv->data);
        break;
      case COL_QOSCONFIG_AMRLCMAXRETXTHRESHOLD:
        config->amRlcMaxRetxThreshold  =  getUint32FromArray(tlv->data);
        break;
      case COL_QOSCONFIG_AMRLCTREORDERING:
        config->amRlcTReordering  =  getUint32FromArray(tlv->data);
        break;
      case COL_QOSCONFIG_AMRLCTSTATUSPROHIBIT:
        config->amRlcTStatusProhibit  =  getUint32FromArray(tlv->data);
        break;
      case COL_QOSCONFIG_DISCARDTIMER:
        config->discardTimer  =  getUint32FromArray(tlv->data);
        break;
      case COL_QOSCONFIG_STATUSREPORTREQUIRED:
        config->statusReportRequired  =  getUint8FromArray(tlv->data);
        break;
      case COL_QOSCONFIG_DRBPRIORITY:
        config->drbPriority  =  getUint8FromArray(tlv->data);
        break;
      case COL_QOSCONFIG_DRBPRIORITISEDBITRATE:
        config->drbPrioritisedBitRate  =  getUint32FromArray(tlv->data);
        break;
      case COL_QOSCONFIG_DRBBUCKETSIZEDURATION:
        config->drbBucketSizeDuration  =  getUint32FromArray(tlv->data);
        break;
      case COL_QOSCONFIG_LOGICALCHANNELGROUP:
        config->logicalChannelGroup  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills pmCounterLogConfigstructure.
 * */
void tlv2StructPmCounterLogConfigTable(
  IO TlvArray           *tlvArray,
  IO PmCounterLogConfig *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_PMCOUNTERLOGCONFIG_PMPROFILEID:
        config->pmProfileId  =  getUint32FromArray(tlv->data);
        break;
      case COL_PMCOUNTERLOGCONFIG_SAMPLEINTERVAL:
        config->sampleInterval  =  getUint32FromArray(tlv->data);
        break;
      case COL_PMCOUNTERLOGCONFIG_PMFILECREATIONINTERVAL:
        config->pmFileCreationInterval  =  getUint32FromArray(tlv->data);
        break;
      case COL_PMCOUNTERLOGCONFIG_COLLECTMACGENCOUNTERS:
        config->collectMacGenCounters  =  getUint8FromArray(tlv->data);
        break;
      case COL_PMCOUNTERLOGCONFIG_COLLECTMACUPLANECOUNTERS:
        config->collectMacUplaneCounters  =  getUint8FromArray(tlv->data);
        break;
      case COL_PMCOUNTERLOGCONFIG_COLLECTRLCGENCOUNTERS:
        config->collectRlcGenCounters  =  getUint8FromArray(tlv->data);
        break;
      case COL_PMCOUNTERLOGCONFIG_COLLECTPDCPGENCOUNTERS:
        config->collectPdcpGenCounters  =  getUint8FromArray(tlv->data);
        break;
      case COL_PMCOUNTERLOGCONFIG_COLLECTPDCPUPLANECOUNTERS:
        config->collectPdcpUplaneCounters  =  getUint8FromArray(tlv->data);
        break;
      case COL_PMCOUNTERLOGCONFIG_COLLECTGTPGENCOUNTERS:
        config->collectGtpGenCounters  =  getUint8FromArray(tlv->data);
        break;
      case COL_PMCOUNTERLOGCONFIG_COLLECTSCTPGENCOUNTERS:
        config->collectSctpGenCounters  =  getUint8FromArray(tlv->data);
        break;
      case COL_PMCOUNTERLOGCONFIG_COLLECTS1APGENCOUNTERS:
        config->collectS1apGenCounters  =  getUint8FromArray(tlv->data);
        break;
      case COL_PMCOUNTERLOGCONFIG_COLLECTS1APPEERNODECOUNTERS:
        config->collectS1apPeerNodeCounters  =  getUint8FromArray(tlv->data);
        break;
      case COL_PMCOUNTERLOGCONFIG_COLLECTRRCGENCOUNTERS:
        config->collectRrcGenCounters  =  getUint8FromArray(tlv->data);
        break;
      case COL_PMCOUNTERLOGCONFIG_COLLECTRRCCELLCOUNTERS:
        config->collectRrcCellCounters  =  getUint8FromArray(tlv->data);
        break;
      case COL_PMCOUNTERLOGCONFIG_COLLECTRRMCOUNTERS:
        config->collectRrmCounters  =  getUint8FromArray(tlv->data);
        break;
      case COL_PMCOUNTERLOGCONFIG_COLLECTIPCOUNTERS:
        config->collectIPCounters  =  getUint8FromArray(tlv->data);
        break;
      case COL_PMCOUNTERLOGCONFIG_COLLECTETHERNETCOUNTERS:
        config->collectEthernetCounters  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills eutraHoProfileConfigstructure.
 * */
void tlv2StructEutraHoProfileConfigTable(
  IO TlvArray           *tlvArray,
  IO EutraHoProfileConfig
                        *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_EUTRAHOPROFILECONFIG_HOPROFILEID:
        config->hoProfileId  =  getUint32FromArray(tlv->data);
        break;
      case COL_EUTRAHOPROFILECONFIG_A3OFFSET:
        config->a3Offset  =  getUint8FromArray(tlv->data);
        break;
      case COL_EUTRAHOPROFILECONFIG_REPORTONLEAVE:
        config->reportOnLeave  =  getUint8FromArray(tlv->data);
        break;
      case COL_EUTRAHOPROFILECONFIG_A5THRESHOLD1:
        config->a5Threshold1  =  getUint8FromArray(tlv->data);
        break;
      case COL_EUTRAHOPROFILECONFIG_A5THRESHOLD2:
        config->a5Threshold2  =  getUint8FromArray(tlv->data);
        break;
      case COL_EUTRAHOPROFILECONFIG_HYSTERESIS:
        config->hysteresis  =  getUint8FromArray(tlv->data);
        break;
      case COL_EUTRAHOPROFILECONFIG_TIMETOTRIGGER:
        config->timeToTrigger  =  getUint32FromArray(tlv->data);
        break;
      case COL_EUTRAHOPROFILECONFIG_TRIGGERQUANTITY:
        config->triggerQuantity  =  getUint32FromArray(tlv->data);
        break;
      case COL_EUTRAHOPROFILECONFIG_REPORTQUANTITY:
        config->reportQuantity  =  getUint32FromArray(tlv->data);
        break;
      case COL_EUTRAHOPROFILECONFIG_FILTERCOEFFICIENTRSRQ:
        config->filterCoefficientRsrq  =  getUint32FromArray(tlv->data);
        break;
      case COL_EUTRAHOPROFILECONFIG_FILTERCOEFFICIENTRSRP:
        config->filterCoefficientRsrp  =  getUint32FromArray(tlv->data);
        break;
      case COL_EUTRAHOPROFILECONFIG_REPORTINTERVAL:
        config->reportInterval  =  getUint32FromArray(tlv->data);
        break;
      case COL_EUTRAHOPROFILECONFIG_REPORTAMOUNT:
        config->reportAmount  =  getUint32FromArray(tlv->data);
        break;
      case COL_EUTRAHOPROFILECONFIG_MAXREPORTCELLS:
        config->maxReportCells  =  getUint8FromArray(tlv->data);
        break;
      case COL_EUTRAHOPROFILECONFIG_SUPPORTEDEVENTS:
        config->supportedEvents  =  getUint32FromArray(tlv->data);
        break;
      case COL_EUTRAHOPROFILECONFIG_SMEASURE:
        config->sMeasure  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills eutraNeighCellConfigstructure.
 * */
void tlv2StructEutraNeighCellConfigTable(
  IO TlvArray           *tlvArray,
  IO EutraNeighCellConfig
                        *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_EUTRANEIGHCELLCONFIG_NEIBOURINDEX:
        config->neibourIndex  =  getUint32FromArray(tlv->data);
        break;
      case COL_EUTRANEIGHCELLCONFIG_ISUSED:
        config->isUsed  =  getUint8FromArray(tlv->data);
        break;
      case COL_EUTRANEIGHCELLCONFIG_NEIGHEUTRAENBID:
        config->neighEutraEnbId  =  getUint32FromArray(tlv->data);
        break;
      case COL_EUTRANEIGHCELLCONFIG_NEIGHEUTRACELLID:
        config->neighEutraCellId  =  getUint8FromArray(tlv->data);
        break;
      case COL_EUTRANEIGHCELLCONFIG_NEIGHEUTRAPHYCELLID:
        config->neighEutraPhyCellId  =  getUint16FromArray(tlv->data);
        break;
      case COL_EUTRANEIGHCELLCONFIG_NEIGHEUTRAPLMNMCC:
        snprintf( config->neighEutraPlmnMcc, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_EUTRANEIGHCELLCONFIG_NEIGHEUTRAPLMNMNC:
        snprintf( config->neighEutraPlmnMnc, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_EUTRANEIGHCELLCONFIG_NEIGHEUTRATAC:
        config->neighEutraTac  =  getUint16FromArray(tlv->data);
        break;
      case COL_EUTRANEIGHCELLCONFIG_NEIGHCELLMEASBANDWIDTH:
        config->neighCellMeasBandwidth  =  getUint32FromArray(tlv->data);
        break;
      case COL_EUTRANEIGHCELLCONFIG_CELLINDIVIDUALOFFSET:
        config->cellIndividualOffset  =  getUint32FromArray(tlv->data);
        break;
      case COL_EUTRANEIGHCELLCONFIG_INTRAFREQNEIGHCELLQOFFSETCELL:
        config->intraFreqNeighCellQOffsetCell  =  getUint32FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills sysInfo3Configstructure.
 * */
void tlv2StructSysInfo3ConfigTable(
  IO TlvArray           *tlvArray,
  IO SysInfo3Config     *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_SYSINFO3CONFIG_SYSINFO3PROFILEID:
        config->sysInfo3ProfileId  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO3CONFIG_QHYST:
        config->qHyst  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO3CONFIG_THRESHSERVINGLOW:
        config->threshServingLow  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO3CONFIG_CELLRESELECTIONPRIORITY:
        config->cellReselectionPriority  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO3CONFIG_QRXLEVMIN:
        config->qRxLevMin  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO3CONFIG_PMAX:
        config->pMax  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO3CONFIG_SINTRASEARCH:
        config->sIntraSearch  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO3CONFIG_ALLOWMEASBW:
        config->allowMeasBw  =  getUint32FromArray(tlv->data);
        break;
      case COL_SYSINFO3CONFIG_NEIGHCELLCONFIG:
        config->neighCellConfig  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO3CONFIG_TRESELECTIONEUTRA:
        config->tReselectionEutra  =  getUint8FromArray(tlv->data);
        break;
      case COL_SYSINFO3CONFIG_PRESENCEANTENNAPORT1:
        config->presenceAntennaPort1  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills sonConfigstructure.
 * */
void tlv2StructSonConfigTable(
  IO TlvArray           *tlvArray,
  IO SonConfig          *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_SONCONFIG_SONPROFILEID:
        config->sonProfileId  =  getUint32FromArray(tlv->data);
        break;
      case COL_SONCONFIG_INTRAFREQHOTHRESHOLD:
        config->intraFreqHoThreshold  =  getUint8FromArray(tlv->data);
        break;
      case COL_SONCONFIG_ICICTHRESHOLD:
        config->icicThreshold  =  getUint8FromArray(tlv->data);
        break;
      case COL_SONCONFIG_UELOADCHANGETHRESHOLD:
        config->ueLoadChangeThreshold  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills srsConfigDedicatedstructure.
 * */
void tlv2StructSrsConfigDedicatedTable(
  IO TlvArray           *tlvArray,
  IO SrsConfigDedicated *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_SRSCONFIGDEDICATED_SRSBANDWIDTH:
        config->srsBandwidth  =  getUint32FromArray(tlv->data);
        break;
      case COL_SRSCONFIGDEDICATED_SRSHOPPINGBANDWIDTH:
        config->srsHoppingBandWidth  =  getUint32FromArray(tlv->data);
        break;
      case COL_SRSCONFIGDEDICATED_FREQUECYDOMAINPOSITION:
        config->frequecyDomainPosition  =  getUint8FromArray(tlv->data);
        break;
      case COL_SRSCONFIGDEDICATED_DURATION:
        config->duration  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills securityConfigstructure.
 * */
void tlv2StructSecurityConfigTable(
  IO TlvArray           *tlvArray,
  IO SecurityConfig     *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_SECURITYCONFIG_INDEX:
        config->index  =  getUint32FromArray(tlv->data);
        break;
      case COL_SECURITYCONFIG_UUCIPHERING:
        config->uuCiphering  =  getUint32FromArray(tlv->data);
        break;
      case COL_SECURITYCONFIG_UUINTEGRITYPROTECTION:
        config->uuIntegrityProtection  =  getUint32FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills ueEventHistoryLogConfigstructure.
 * */
void tlv2StructUeEventHistoryLogConfigTable(
  IO TlvArray           *tlvArray,
  IO UeEventHistoryLogConfig
                        *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_UEEVENTHISTORYLOGCONFIG_UEEVENTHISTORYINDEX:
        config->ueEventHistoryIndex  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEEVENTHISTORYLOGCONFIG_CRNTI:
        config->cRnti  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEEVENTHISTORYLOGCONFIG_UEPROCEDUREBITMAP:
        config->ueProcedureBitMap  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEEVENTHISTORYLOGCONFIG_UEEVENTMEASURES:
        config->ueEventMeasures  =  getUint32FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills counterConfigstructure.
 * */
void tlv2StructCounterConfigTable(
  IO TlvArray           *tlvArray,
  IO CounterConfig      *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_COUNTERCONFIG_INDEX:
        config->index  =  getUint32FromArray(tlv->data);
        break;
      case COL_COUNTERCONFIG_COUNTERID:
        config->counterId  =  getUint32FromArray(tlv->data);
        break;
      case COL_COUNTERCONFIG_PROFILEID:
        config->profileId  =  getUint32FromArray(tlv->data);
        break;
      case COL_COUNTERCONFIG_ISENABLED:
        config->isEnabled  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills traceConfigstructure.
 * */
void tlv2StructTraceConfigTable(
  IO TlvArray           *tlvArray,
  IO TraceConfig        *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_TRACECONFIG_TRACEPROFILEID:
        config->traceProfileId  =  getUint32FromArray(tlv->data);
        break;
      case COL_TRACECONFIG_TRACERRC:
        config->traceRRC  =  getUint8FromArray(tlv->data);
        break;
      case COL_TRACECONFIG_TRACES1AP:
        config->traceS1AP  =  getUint8FromArray(tlv->data);
        break;
      case COL_TRACECONFIG_TRACEX2AP:
        config->traceX2AP  =  getUint8FromArray(tlv->data);
        break;
      case COL_TRACECONFIG_TRACERESERVED1:
        config->traceReserved1  =  getUint8FromArray(tlv->data);
        break;
      case COL_TRACECONFIG_TRACERESERVED2:
        config->traceReserved2  =  getUint8FromArray(tlv->data);
        break;
      case COL_TRACECONFIG_TRACERESERVED3:
        config->traceReserved3  =  getUint8FromArray(tlv->data);
        break;
      case COL_TRACECONFIG_TRACEDESTINATION:
        config->traceDestination  =  getUint32FromArray(tlv->data);
        break;
      case COL_TRACECONFIG_TRACEDESTUDPPORT:
        config->traceDestUdpPort  =  getUint16FromArray(tlv->data);
        break;
      case COL_TRACECONFIG_TRACEDESTIPADDRESS:
        snprintf( config->traceDestIPAddress, (tlv->len+1), "%s", tlv->data ); 
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}

