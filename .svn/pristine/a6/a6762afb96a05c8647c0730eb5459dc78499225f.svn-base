/**
 * @file    set_advanced_cfg_func.c
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This is auto generated file from Excel
 * @section This file contains SET functions for all tables
 *
 * Author: Madhusudana Rao Kata
 * Date: 11-10-2010
 * Description: Draft version 
 * Generated on : 11/29/2012 12:22:22 PM
 */

 
#include "set_oam_func.h"

/*******************************************************************/
/***************          PRECONFIG TABLE SET FUNCTIONS     ********/
/*******************************************************************/

/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills msgTracestructure.
 * */
void tlv2StructMsgTraceTable(
  IO TlvArray           *tlvArray,
  IO MsgTrace           *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_MSGTRACE_MSGID:
        config->msgId  =  getUint32FromArray(tlv->data);
        break;
      case COL_MSGTRACE_MSGTRACELEVEL:
        config->msgTraceLevel  =  getUint32FromArray(tlv->data);
        break;
      case COL_MSGTRACE_MSGTRACEDIRECTION:
        config->msgTraceDirection  =  getUint32FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills timersConfigstructure.
 * */
void tlv2StructTimersConfigTable(
  IO TlvArray           *tlvArray,
  IO TimersConfig       *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_TIMERSCONFIG_TIMERNAME:
        config->timerName  =  getUint32FromArray(tlv->data);
        break;
      case COL_TIMERSCONFIG_TIMEUNIT:
        config->timeUnit  =  getUint32FromArray(tlv->data);
        break;
      case COL_TIMERSCONFIG_DURATION:
        config->duration  =  getUint32FromArray(tlv->data);
        break;
      case COL_TIMERSCONFIG_DESCRIPTION:
        snprintf( config->description, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_TIMERSCONFIG_MODULEID:
        config->moduleId  =  getUint32FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills processStartstructure.
 * */
void tlv2StructProcessStartTable(
  IO TlvArray           *tlvArray,
  IO ProcessStart       *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_PROCESSSTART_PATH:
        snprintf( config->path, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_PROCESSSTART_EXENAME:
        snprintf( config->exeName, (tlv->len+1), "%s", tlv->data ); 
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills moduleStopstructure.
 * */
void tlv2StructModuleStopTable(
  IO TlvArray           *tlvArray,
  IO ModuleStop         *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_MODULESTOP_MODULEIDSTRING:
        config->moduleIdString  =  getUint32FromArray(tlv->data);
        break;
      case COL_MODULESTOP_MODULEDESC:
        snprintf( config->moduleDesc, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_MODULESTOP_EXITPRIORITY:
        config->exitPriority  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills eventThresholdsstructure.
 * */
void tlv2StructEventThresholdsTable(
  IO TlvArray           *tlvArray,
  IO EventThresholds    *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_EVENTTHRESHOLDS_INDEX:
        config->index  =  getUint32FromArray(tlv->data);
        break;
      case COL_EVENTTHRESHOLDS_CPUUSAGEHIGH:
        config->cpuUsageHigh  =  getUint32FromArray(tlv->data);
        break;
      case COL_EVENTTHRESHOLDS_CPUUSAGENORMAL:
        config->cpuUsageNormal  =  getUint32FromArray(tlv->data);
        break;
      case COL_EVENTTHRESHOLDS_MEMUSAGEHIGH:
        config->memUsageHigh  =  getUint32FromArray(tlv->data);
        break;
      case COL_EVENTTHRESHOLDS_MEMUSAGENORMAL:
        config->memUsageNormal  =  getUint32FromArray(tlv->data);
        break;
      case COL_EVENTTHRESHOLDS_ENBLOADHIGHMBPS:
        config->enbLoadHighMbps  =  getUint32FromArray(tlv->data);
        break;
      case COL_EVENTTHRESHOLDS_ENBLOADNORMALMBPS:
        config->enbLoadNormalMbps  =  getUint32FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills enbAddressesstructure.
 * */
void tlv2StructEnbAddressesTable(
  IO TlvArray           *tlvArray,
  IO EnbAddresses       *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_ENBADDRESSES_BACKHAULPORTMACADDRESS:
        snprintf( config->backhaulPortMacAddress, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBADDRESSES_DEBUGPORTMACADDRESS:
        snprintf( config->debugPortMacAddress, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBADDRESSES_UNIQUEPRODUCTID:
        snprintf( config->uniqueProductId, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBADDRESSES_STATICIPADDRESSUSEDATDEBUGPORT:
        snprintf( config->staticIPAddressUsedAtDebugPort, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBADDRESSES_NETMASKFORDEBUGPORT:
        snprintf( config->netmaskForDebugPort, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBADDRESSES_RADIOHEADIPADDRESS:
        snprintf( config->radioHeadIpAddress, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_ENBADDRESSES_RADIOHEADMACADDRESS:
        snprintf( config->radioHeadMacAddress, (tlv->len+1), "%s", tlv->data ); 
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills udhConfigstructure.
 * */
void tlv2StructUdhConfigTable(
  IO TlvArray           *tlvArray,
  IO UdhConfig          *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_UDHCONFIG_ULINACTIVITYINTERVALINSECS:
        config->ulInactivityIntervalInSecs  =  getUint16FromArray(tlv->data);
        break;
      case COL_UDHCONFIG_UETHROUGHTPUTTIMERINSECS:
        config->ueThroughtputTimerInSecs  =  getUint8FromArray(tlv->data);
        break;
      case COL_UDHCONFIG_MEMORYDIAGCFG:
        config->memoryDiagCfg  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills uehConfigstructure.
 * */
void tlv2StructUehConfigTable(
  IO TlvArray           *tlvArray,
  IO UehConfig          *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_UEHCONFIG_SYSTEMINFOVALUETAG:
        config->systemInfoValueTag  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PRACHRESOURCE:
        config->prachResource  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_SCSPACING:
        config->scSpacing  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_CYCPFXTYPE:
        config->cycPfxType  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_SRSCFGPRD:
        config->srsCfgPrd  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_SRBPOLLRETRANS:
        config->srbPollRetrans  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_SRBPOLLPDU:
        config->srbPollPdu  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_SRBPOLLBYTE:
        config->srbPollByte  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_SRBMAXRETXTHRESH:
        config->srbMaxReTxThresh  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_SRBTREORDRNG:
        config->srbTreordrng  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_SRBTSTATPROHIBIT:
        config->srbTStatProhibit  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_SRB1LOGCHCFGPRIOR:
        config->srb1LogchcfgPrior  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_SRB2LOGCHCFGPRIOR:
        config->srb2LogchcfgPrior  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_MACCFGTTIBUNDLING:
        config->macCfgTtiBundling  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_DELTAMCSENABLED:
        config->deltaMcsEnabled  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_ACCUMULATIONENABLED:
        config->accumulationEnabled  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_CQIPUCCHRESIDX:
        config->cqiPUCCHResIdx  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_CQIPMICONFIGIDX:
        config->cqiPmiConfigIdx  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_RICONFIGIDX:
        config->riConfigIdx  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_SIMULTACKNACKANDCQI:
        config->simultAckNackAndCQI  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_TPCRNTI:
        config->tpcRNTI  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_INDEXOFFORMAT3:
        config->indexOfFormat3  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_SRSPERIODICITY:
        config->srsPeriodicity  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_TRANSMISSIONCOMB:
        config->transmissionComb  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_DEDCYCLICSHIFT:
        config->dedCyclicShift  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_SRPUCCHRESIDX:
        config->srPucchResIdx  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_SRCONFIGIDX:
        config->srConfigIdx  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_DSRTRANSMAX:
        config->dsrTransMax  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_DRBGRP:
        config->drbGrp  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_UECATEGORY:
        config->ueCategory  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_REPFACT:
        config->repFact  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_N1PUCCHANREP:
        config->n1PucchAnRep  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_FORMATIND:
        config->formatInd  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_ANTENNASEL:
        config->antennaSel  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_APERIODICMODE:
        config->aPeriodicMode  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PERIODICMODE:
        config->periodicMode  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PRDICITYENUM:
        config->prdicityEnum  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_CQIOFFST:
        config->cqiOffst  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_CQICFGK:
        config->cqiCfgk  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_MAXULHQTX:
        config->maxUlHqTx  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_UEULPWRTRGCQI:
        config->ueUlPwrTrgCqi  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_ACKNACKREPFACTOR:
        config->ackNackRepFactor  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_MACINST:
        config->macInst  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_MAXUEPERULSF:
        config->maxUePerUlSf  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_MAXUEPERDLSF:
        config->maxUePerDlSf  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_MAXULBWPERUE:
        config->maxUlBwPerUe  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_MAXDLBWPERUE:
        config->maxDlBwPerUe  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_MAXDLRETXBW:
        config->maxDlRetxBw  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_MAXDLUENEWTXPERTTI:
        config->maxDlUeNewTxPerTti  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_MAXULUENEWTXPERTTI:
        config->maxUlUeNewTxPerTti  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_ISCYCLICPREFIXULEXT :
        config->isCyclicPrefixUlExt   =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_ISCYCLICPREFIXDLEXT :
        config->isCyclicPrefixDlExt   =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_CELLMODSCHM:
        config->cellModSchm  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_BCCHPCHRACODERATE:
        config->bcchPchRaCodeRate  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PDCCHCODERATE:
        config->pdcchCodeRate  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_DLCMNCODRATECCCHCQI:
        config->dlCmnCodRateCcchCqi  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PUSCHSUBBNDSTRT:
        config->puschSubBndStrt  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PUSCHNOSUBBNDS:
        config->puschNoSubbnds  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PUSCHSUBBANDSZ:
        config->puschSubBandSz  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PUSCHSUBBANDDMRS:
        config->puschSubBandDmrs  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_ULCMNCODRATECCCHCQI:
        config->ulCmnCodRateCcchCqi  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_ISDLFREQSEL:
        config->isDlFreqSel  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_THRESHOLDCQI:
        config->thresholdCqi  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PREAMBLEFRMT:
        config->preambleFrmt  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_RAWINSZ:
        config->raWinSz  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_RAOCCSZ:
        config->raOccSz  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_RAOCCSFNENUM:
        config->raOccSfnEnum  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PUCCHPWRFMT3RNTIST:
        config->pucchPwrFmt3RntiSt  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PUCCHPWRFMT3RNTISZ:
        config->pucchPwrFmt3RntiSz  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PUCCHPWRFMT3ARNTIST:
        config->pucchPwrFmt3aRntiSt  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PUCCHPWRFMT3ARNTISZ:
        config->pucchPwrFmt3aRntiSz  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PUSCHPWRFMT3RNTIST:
        config->puschPwrFmt3RntiSt  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PUSCHPWRFMT3RNTISZ:
        config->puschPwrFmt3RntiSz  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PUSCHPWRFMT3ARNTIST:
        config->puschPwrFmt3aRntiSt  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PUSCHPWRFMT3ARNTISZ:
        config->puschPwrFmt3aRntiSz  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_UEULBYTERATE:
        config->ueUlByteRate  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PREAMBLESETCFGPRES:
        config->preambleSetCfgPres  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PREAMBLESETCFGSTART:
        config->preambleSetCfgStart  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PREAMBLESETCFGSZ:
        config->preambleSetCfgSz  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_PRACHNUMRES:
        config->prachNumRes  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_CRNTIRTNTIMER:
        config->crntiRtnTimer  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_NUMOFTXANT:
        config->numOfTxAnt  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_STANDALONESRB1TIMER:
        config->standaloneSrb1Timer  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_CONTEXTRELEASETIMER:
        config->contextReleaseTimer  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_MMERELINDTIMER:
        config->mmeRelIndTimer  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_RRCCONNRELL2ACKTIMER:
        config->rrcConnRelL2AckTimer  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_ISUESECENBLE:
        config->isUeSecEnble  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_UECATPOLLBYTEIDX:
        config->ueCatPollByteIdx  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_DUPMODE:
        config->dupMode  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_ULUMSNFIELDLEN:
        config->ulUmSnFieldLen  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_DLUMSNFIELDLEN:
        config->dlUmSnFieldLen  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_UMRLCTREORDERING:
        config->umRlcTReordering  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_ULCPLENGTH:
        config->ulCpLength  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_SRBPRIORITISEDBITRATE:
        config->srbPrioritisedBitRate  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_INTERNALUECONFIGTIMER:
        config->internalUeConfigTimer  =  getUint32FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_UEPWRRECONFTIMERINSEC:
        config->uePwrReconfTimerinSec  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_ISDLLAENABLED:
        config->isDlLaEnabled  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_UEHSTATEAUDITTMRDURINSECS:
        config->uehStateAuditTmrDurInSecs  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_UEHPROCEDUREAUDITTMRDURINSECS:
        config->uehProcedureAuditTmrDurInSecs  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_UEHPUCCHAUDITTMRDURINSECS:
        config->uehPucchAuditTmrDurInSecs  =  getUint16FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_MEMORYDIAGCFG:
        config->memoryDiagCfg  =  getUint8FromArray(tlv->data);
        break;
      case COL_UEHCONFIG_MAXMSG3PERULSF:
        config->maxMsg3PerUlSf  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills egtpConfigstructure.
 * */
void tlv2StructEgtpConfigTable(
  IO TlvArray           *tlvArray,
  IO EgtpConfig         *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_EGTPCONFIG_NMBUSAPS:
        config->nmbUSaps  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_MAXNMBTSAPS:
        config->maxNmbTSaps  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_NMBSRVS:
        config->nmbSrvs  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_TIMERRES:
        config->timerRes  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_RESTHRESHUPPER:
        config->resThreshUpper  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_RESTHRESHLOWER:
        config->resThreshLower  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_EGUT3TIMERENABLED:
        config->eguT3TimerEnabled  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_EGUT3TIMERVAL:
        config->eguT3TimerVal  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_NMBRETRIES:
        config->nmbRetries  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_ECHOTMRENABLED:
        config->echoTmrEnabled  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_ECHOTMRVAL:
        config->echoTmrVal  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_REORDERTMRENABLED:
        config->reOrderTmrEnabled  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_REORDERTMRVAL:
        config->reOrderTmrVal  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_PVTEXTID:
        config->pvtExtId  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_PDUWINDSIZE:
        config->pduWindSize  =  getUint32FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_MAXNUMOFIP:
        config->maxNumOfIp  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_NODEARCH:
        config->nodeArch  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_NMBWORKERTHREAD:
        config->nmbWorkerThread  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_LOADDISTTMRENABLED:
        config->loadDistTmrEnabled  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_LOADDISTTMRVAL:
        config->loadDistTmrVal  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_TINITTMRENABLED:
        config->tInitTmrEnabled  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_TINITTMRVAL:
        config->tInitTmrVal  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_MAXBNDRETRY:
        config->maxBndRetry  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_BNDTMCFGENABLED:
        config->bndTmCfgEnabled  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_BNDTMCFGVAL:
        config->bndTmCfgVal  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_TPAR_U_SOCKPARAM_LISTENQSIZE:
        config->tPar_u_sockParam_listenQSize  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_TPAR_U_SOCKPARAM_NUMOPTS:
        config->tPar_u_sockParam_numOpts  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_IPINFO_ECHOREQPVTEXTVALLENGTH:
        config->ipInfo_echoReqPvtExtValLength  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_IPINFO_ECHOREQPVTEXTVALBUFF:
        snprintf( config->ipInfo_echoReqPvtExtValBuff, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_EGTPCONFIG_IPINFO_ECHORSPPVTEXTVALLENGTH:
        config->ipInfo_echoRspPvtExtValLength  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_IPINFO_ECHORSPPVTEXTVALBUFF:
        snprintf( config->ipInfo_echoRspPvtExtValBuff, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_EGTPCONFIG_IPINFO_ERRINDPVTEXTVALLENGTH:
        config->ipInfo_errIndPvtExtValLength  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_IPINFO_ERRINDPVTEXTVALBUFF:
        snprintf( config->ipInfo_errIndPvtExtValBuff, (tlv->len+1), "%s", tlv->data ); 
        break;
      case COL_EGTPCONFIG_TSERV_TPAR_U_SOCKPARAM_NUMOPTS:
        config->tserv_tPar_u_sockParam_numOpts  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_TSERV_TPAR_U_SOCKPARAM_LISTENQSIZE:
        config->tserv_tPar_u_sockParam_listenQSize  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_OPNSRVRETRYCNT:
        config->opnSrvRetryCnt  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_OPNSRVTMRENABLED:
        config->opnSrvTmrEnabled  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_OPNSRVTMRVAL:
        config->opnSrvTmrVal  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_HOSTNAMEPRESENT:
        config->hostNamePresent  =  getUint8FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_HOSTNAMELEN:
        config->hostNameLen  =  getUint16FromArray(tlv->data);
        break;
      case COL_EGTPCONFIG_GTP_C_OR_U:
        config->gtp_C_Or_U  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills sctpConfigstructure.
 * */
void tlv2StructSctpConfigTable(
  IO TlvArray           *tlvArray,
  IO SctpConfig         *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_SCTPCONFIG_MAXNMBSCTSAPS:
        config->maxNmbSctSaps  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_MAXNMBTSAPS:
        config->maxNmbTSaps  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_MAXNMBASSOC:
        config->maxNmbAssoc  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_MAXNMBENDP:
        config->maxNmbEndp  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_MAXNMBDSTADDR:
        config->maxNmbDstAddr  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_MAXNMBSRCADDR:
        config->maxNmbSrcAddr  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_MAXNMBTXCHUNKS:
        config->maxNmbTxChunks  =  getUint32FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_MAXNMBRXCHUNKS:
        config->maxNmbRxChunks  =  getUint32FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_MAXNMBINSTRMS:
        config->maxNmbInStrms  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_MAXNMBOUTSTRMS:
        config->maxNmbOutStrms  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_INITARWND:
        config->initARwnd  =  getUint32FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_MTUINITIAL:
        config->mtuInitial  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_MTUMININITIAL:
        config->mtuMinInitial  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_MTUMAXINITIAL:
        config->mtuMaxInitial  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_PERFORMMTU:
        config->performMtu  =  getUint8FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_TIMERES:
        config->timeRes  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_MAXINITRETX:
        config->reConfig_MaxInitReTx  =  getUint8FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_MAXASSOCRETX:
        config->reConfig_MaxAssocReTx  =  getUint8FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_MAXPATHRETX:
        config->reConfig_MaxPathReTx  =  getUint8FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_ALTACCEPTFLG:
        config->reConfig_AltAcceptFlg  =  getUint8FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_KEYTM:
        config->reConfig_KeyTm  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_ALPHA:
        config->reConfig_Alpha  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_BETA:
        config->reConfig_Beta  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_TINTTMR:
        config->reConfig_TIntTmr  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_MAXBNDRETRY:
        config->reConfig_MaxBndRetry  =  getUint8FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_MAXACKDELAYTM:
        config->reConfig_MaxAckDelayTm  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_MAXACKDELAYDG:
        config->reConfig_MaxAckDelayDg  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_RTOINITIAL:
        config->reConfig_RtoInitial  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_RTOMIN:
        config->reConfig_RtoMin  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_RTOMAX:
        config->reConfig_RtoMax  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_FREEZETM:
        config->reConfig_FreezeTm  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_BUNDLETIME:
        config->reConfig_BundleTime  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_COOKIELIFE:
        config->reConfig_CookieLife  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_HBEATINTERVALTM:
        config->reConfig_HbeatIntervalTm  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_MAXBURST:
        config->reConfig_MaxBurst  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_MAXHBBURST:
        config->reConfig_MaxHbBurst  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_T5SDOWNGRDTM:
        config->reConfig_T5SdownGrdTm  =  getUint16FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_HANDLEINITFLG:
        config->reConfig_HandleInitFlg  =  getUint8FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_NEGABRTFLG:
        config->reConfig_NegAbrtFlg  =  getUint8FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_HBEATENABLE:
        config->reConfig_HBeatEnable  =  getUint8FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_FLCUPTHR:
        config->reConfig_FlcUpThr  =  getUint32FromArray(tlv->data);
        break;
      case COL_SCTPCONFIG_RECONFIG_FLCLOWTHR:
        config->reConfig_FlcLowThr  =  getUint32FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills s1apConfigstructure.
 * */
void tlv2StructS1apConfigTable(
  IO TlvArray           *tlvArray,
  IO S1apConfig         *config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_S1APCONFIG_NMBSZTSAPS:
        config->nmbSztSaps  =  getUint16FromArray(tlv->data);
        break;
      case COL_S1APCONFIG_NMBSCTSAPS:
        config->nmbSctSaps  =  getUint16FromArray(tlv->data);
        break;
      case COL_S1APCONFIG_NMBCONN:
        config->nmbConn  =  getUint32FromArray(tlv->data);
        break;
      case COL_S1APCONFIG_NMBPEER:
        config->nmbPeer  =  getUint32FromArray(tlv->data);
        break;
      case COL_S1APCONFIG_TIMERES:
        config->timeRes  =  getUint16FromArray(tlv->data);
        break;
      case COL_S1APCONFIG_MAXBNDRETRY:
        config->maxBndRetry  =  getUint8FromArray(tlv->data);
        break;
      case COL_S1APCONFIG_BNDTMCFGENABLED:
        config->bndTmCfgEnabled  =  getUint8FromArray(tlv->data);
        break;
      case COL_S1APCONFIG_BNDTMCFGVAL:
        config->bndTmCfgVal  =  getUint16FromArray(tlv->data);
        break;
      case COL_S1APCONFIG_S1APLOCALPORT:
        config->s1apLocalPort  =  getUint16FromArray(tlv->data);
        break;
      case COL_S1APCONFIG_S1APGUARDTIMER:
        config->s1apGuardTimer  =  getUint16FromArray(tlv->data);
        break;
      case COL_S1APCONFIG_ASSOCCFG_NMBRETRY:
        config->assocCfg_nmbRetry  =  getUint16FromArray(tlv->data);
        break;
      case COL_S1APCONFIG_ASSOCCFG_TMRASSOCRECOV_ENB:
        config->assocCfg_tmrAssocRecov_enb  =  getUint8FromArray(tlv->data);
        break;
      case COL_S1APCONFIG_ASSOCCFG_TMRASSOCRECOV_VAL:
        config->assocCfg_tmrAssocRecov_val  =  getUint16FromArray(tlv->data);
        break;
      case COL_S1APCONFIG_MAXS1SETUPALLOWED:
        config->maxS1SetupAllowed  =  getUint16FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}
/**
 * @param setObj       Set Object Request sent by CIH/WIH/SIH
 * @section This function will get input in TLV Array and fills deviceMonitorConfigstructure.
 * */
void tlv2StructDeviceMonitorConfigTable(
  IO TlvArray           *tlvArray,
  IO DeviceMonitorConfig*config
  ) 
{
  UINT8 i = 0;
  TlvInfo    *tlv = NULL;
 
  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_DEVICEMONITORCONFIG_INDEX:
        config->index  =  getUint32FromArray(tlv->data);
        break;
      case COL_DEVICEMONITORCONFIG_BASEBAND:
        config->baseBand  =  getUint8FromArray(tlv->data);
        break;
      case COL_DEVICEMONITORCONFIG_RADIOHEAD:
        config->radioHead  =  getUint8FromArray(tlv->data);
        break;
      case COL_DEVICEMONITORCONFIG_BACKHAULPORT:
        config->backHaulPort  =  getUint8FromArray(tlv->data);
        break;
      case COL_DEVICEMONITORCONFIG_DEBUGPORT:
        config->debugPort  =  getUint8FromArray(tlv->data);
        break;
      case COL_DEVICEMONITORCONFIG_TEMPSENSOR:
        config->tempSensor  =  getUint8FromArray(tlv->data);
        break;
      case COL_DEVICEMONITORCONFIG_GPS:
        config->gps  =  getUint8FromArray(tlv->data);
        break;
      case COL_DEVICEMONITORCONFIG_CPRI:
        config->cpri  =  getUint8FromArray(tlv->data);
        break;
      case COL_DEVICEMONITORCONFIG_CLOCK:
        config->clock  =  getUint8FromArray(tlv->data);
        break;
      default:
        BS_ERROR_INVALID_COLID_RCVD( tlv->tag.tableId, tlv->tag.rowId, tlv->tag.colId );
        break;
    }
  }
}

