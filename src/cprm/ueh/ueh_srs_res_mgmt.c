/**
 * @file    ueh_srs_res_mgmt.c
 * @brief This file contains the functions for handling SRS resource management
 *
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 */

#include "ccpu_cmn.h"

/* RRM related includes */
#include "ueh_eut.h"
#include "lueh.h"
#include "lueh.x"
#include "ueh.h"
#include "ueh_crm.h"
#include "ueh_srs_res_mgmt.h"
#include "bs_tables.h"
#include "log.h"

SrsSfTable              *gSrsSfTable;
SfPeriodSfIndxTable     sfIndxTable;

SINT16 uehGetSrsPeriodicity(void)
{
  switch ( smCb.cfgCb.uehConfig.srsPeriodicity)
  {
    case SRSPERIODICITY_SRS2:
      return 2;
    case SRSPERIODICITY_SRS5:
      return 5;
    case SRSPERIODICITY_SRS10:
      return 10;
    case SRSPERIODICITY_SRS20:
      return 20;
    case SRSPERIODICITY_SRS40:
      return 40;
    case SRSPERIODICITY_SRS80:
      return 80;
    case SRSPERIODICITY_SRS160:
      return 160;
    case SRSPERIODICITY_SRS320:
      return 320;
    default:
      return SRSPERIODICITY_SRSINV;
  }
}

void uehSetSrsPeriodicity(
    SINT16 srsPeriod
    )
{
  smCb.cfgCb.uehConfig.srsPeriodicity = srsPeriod;
}

SINT16 uehCrmDeinitializeSfSrsTables( void )
{
  /* step1:
   * Deallocate memory for the SfSrsTable
   * Set the table pointer to NULL
   */
  UINT16        srsPeriodicity = uehGetSrsPeriodicity();

  UEH_FREE( gSrsSfTable, (sizeof(SrsSfTable) * srsPeriodicity) );
  gSrsSfTable = NULL;

  logInfo("SRS Table is freed; for Periodicity:=%d", srsPeriodicity );
  return 0;
}

SINT16 uehCrmInitializeSfSrsTables( void )
{
  SINT16        i=0, j, k;
  SrsSfTable    *srsSfTableEntry;
  UINT16        srsPeriodicity = uehGetSrsPeriodicity();

  if( SRSPERIODICITY_SRSINV EQ srsPeriodicity )
  {
    return 0;
  }

  if( MAX_SRS_PERIOD_ENUM < srsPeriodicity )
  {
    UEH_ERROR_ELMNT_OUT_OF_RANGE("SRS Periodicity", srsPeriodicity, 0, MAX_SRS_PERIOD_ENUM);
    uehSetSrsPeriodicity(SRSPERIODICITY_SRSINV);
  }

  /* step1:
   * Allocate memory for the SfSrsTable
   * Initialise the table
   */
  if(gSrsSfTable) UEH_FREE( gSrsSfTable, (sizeof(SrsSfTable) * srsPeriodicity) );
  UEH_ALLOC( &gSrsSfTable, (sizeof(SrsSfTable) * srsPeriodicity) );
  if(NULL EQ gSrsSfTable)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(sizeof(SrsSfTable)*srsPeriodicity , "SRS table");
    return -1;
  }

  srsSfTableEntry = gSrsSfTable;
  for(i=0; i<srsPeriodicity; i++, srsSfTableEntry++ )
  {
    srsSfTableEntry->load = 0;
    for( j=0; j< MAX_SRS_COMBS; j++ )
    {
      /* Initialise Comb */
      srsSfTableEntry->comb[j].load = 0;
      for( k=0; k< MAX_SRS_CYCLIC_SHIFT; k++ )
      {
        srsSfTableEntry->comb[j].cShift[k] = 0;
      }
    }
  }

  /* Initialize SRS Enum Array */
  sfIndxTable.sfPeriodSfCfgIndxMap[0].sfPeriod = 2;
  sfIndxTable.sfPeriodSfCfgIndxMap[0].sfCfgIdxStart = 0;

  sfIndxTable.sfPeriodSfCfgIndxMap[1].sfPeriod = 5;
  sfIndxTable.sfPeriodSfCfgIndxMap[1].sfCfgIdxStart = 2;

  sfIndxTable.sfPeriodSfCfgIndxMap[2].sfPeriod = 10;
  sfIndxTable.sfPeriodSfCfgIndxMap[2].sfCfgIdxStart = 7;

  sfIndxTable.sfPeriodSfCfgIndxMap[3].sfPeriod = 20;
  sfIndxTable.sfPeriodSfCfgIndxMap[3].sfCfgIdxStart = 17;

  sfIndxTable.sfPeriodSfCfgIndxMap[4].sfPeriod = 40;
  sfIndxTable.sfPeriodSfCfgIndxMap[4].sfCfgIdxStart = 37;

  sfIndxTable.sfPeriodSfCfgIndxMap[5].sfPeriod = 80;
  sfIndxTable.sfPeriodSfCfgIndxMap[5].sfCfgIdxStart = 77;

  sfIndxTable.sfPeriodSfCfgIndxMap[6].sfPeriod = 160;
  sfIndxTable.sfPeriodSfCfgIndxMap[6].sfCfgIdxStart = 157;

  sfIndxTable.sfPeriodSfCfgIndxMap[7].sfPeriod = 320;
  sfIndxTable.sfPeriodSfCfgIndxMap[7].sfCfgIdxStart = 317;

  logInfo("SRS Table is initialized for Periodicity:=%d", srsPeriodicity );
  return 0;
}

SINT16 uehCrmFindSfConfigIndex(
    UINT16      sfIndex
    )
{
  UINT16        srsPeriodicity = uehGetSrsPeriodicity();
  UINT16        i;

  for( i=0; i<MAX_SRS_PERIOD_ENUM; i++ )
  {
    if( srsPeriodicity EQ sfIndxTable.sfPeriodSfCfgIndxMap[i].sfPeriod )
      break;
  }

  return ( sfIndxTable.sfPeriodSfCfgIndxMap[i].sfCfgIdxStart + sfIndex );
}

SINT16 uehCrmFindSfIndex(
    UINT16      sfCfgIndex
    )
{
  UINT16        srsPeriodicity = uehGetSrsPeriodicity();
  UINT16        i;

  for( i=0; i<MAX_SRS_PERIOD_ENUM; i++ )
  {
    if( srsPeriodicity EQ sfIndxTable.sfPeriodSfCfgIndxMap[i].sfPeriod )
      break;
  }

  return ( sfCfgIndex - sfIndxTable.sfPeriodSfCfgIndxMap[i].sfCfgIdxStart );
}

SINT16 uehCrmFindLeastLoadedSrsCyclicShift(
    UINT16      sfIndex,
    UINT16      combIndex
    )
{
  SINT16 indx=0;

  for(indx=0; indx<MAX_SRS_CYCLIC_SHIFT; indx++)
  {
    if( 0 == gSrsSfTable[sfIndex].comb[combIndex].cShift[indx] )
    {
      break;
    }
  }

  return indx;
}


SINT16 uehCrmFindLeastLoadedSrsComb(
    UINT16      sfIndex
    )
{
  SINT16 combIndex=0, leastLoad, lowest;

  leastLoad = gSrsSfTable[sfIndex].comb[0].load;
  for( lowest =0, combIndex=0; combIndex<MAX_SRS_COMBS; combIndex++)
  {
    if( leastLoad > gSrsSfTable[sfIndex].comb[combIndex].load )
    {
      lowest = combIndex;
      break;
    }
  }

  return lowest;
}


SINT16 uehCrmFindLeastLoadedSrsSf(
    UINT16      srsPeriodicity
    )
{
  SINT16 sfIndex=0, leastLoad, lowest;

  leastLoad = gSrsSfTable[0].load;
  for( lowest =0, sfIndex=0; sfIndex<srsPeriodicity; sfIndex++)
  {
    if( leastLoad > gSrsSfTable[sfIndex].load )
    {
      lowest = sfIndex;
      break;
    }
  }

  return lowest;
}

static UINT8 getSrsBandwidthValue(
  IN SrsBandwidth srsBandwidthEnum
  )
{
  switch( srsBandwidthEnum )
  {
  case SRSBANDWIDTH_BW0:  return 0;
  case SRSBANDWIDTH_BW1:  return 1;
  case SRSBANDWIDTH_BW2:  return 2;
  case SRSBANDWIDTH_BW3:  return 3;
  default:
		UEH_ERROR_ELMNT_OUT_OF_RANGE("SrsBandwidth", srsBandwidthEnum, 0, 3);
	  //logError(" Coding Error. Case value not handled :%d ", srsBandwidthEnum );
	  return 255;  /* TODO : Check the error value */
  }
}

static UINT8 getSrsHoppingBandWidthValue(
  IN SrsHoppingBandWidth srsHoppingBandWidthEnum
  )
{
  switch( srsHoppingBandWidthEnum )
  {
	case SRSHOPPINGBANDWIDTH_HBW0:  return 0;
	case SRSHOPPINGBANDWIDTH_HBW1:  return 1;
	case SRSHOPPINGBANDWIDTH_HBW2:  return 2;
	case SRSHOPPINGBANDWIDTH_HBW3:  return 3;
	default:
		UEH_ERROR_ELMNT_OUT_OF_RANGE("SrsHoppingBandWidth", srsHoppingBandWidthEnum, 0, 3);
//	  logError(" Coding Error. Case value not handled :%d ", srsHoppingBandWidthEnum );
	  return 255;  /* TODO : Check the error value */
  }
}

void uehCrmAllocateSrs(
    UehUeCb * ueCb
    )
{
  UINT16      sfIndx, combIndx, csIndx;
  UINT16      srsPeriodicity = uehGetSrsPeriodicity();

  /* IF SRS Periodicity is SRSPERIODICITY_SRSINV, this is a SRS is invalid
   * and the SRs feature is disabled,Don't do any SRS Handling
   */
  if( ( srsPeriodicity EQ SRSPERIODICITY_SRSINV) ||
      (NULL EQ gSrsSfTable) )
  {
    return;
  }


  sfIndx        = uehCrmFindLeastLoadedSrsSf( srsPeriodicity );
  combIndx      = uehCrmFindLeastLoadedSrsComb( sfIndx );
  csIndx        = uehCrmFindLeastLoadedSrsCyclicShift( sfIndx, combIndx );

  /* Update the table,
   * Update the loads */
  gSrsSfTable[sfIndx].comb[combIndx].cShift[csIndx] = ueCb->crnti;
  gSrsSfTable[sfIndx].comb[combIndx].load += 1;
  gSrsSfTable[sfIndx].load +=1;

  /* Fill the UE Srs Parameters */
  ueCb->ueSrsInfo.ueSrsBandwidth  = getSrsBandwidthValue(uehCb.uehCfgCb.srsCfgDedicated.srsBandwidth);
  ueCb->ueSrsInfo.ueSrsHoppingBw  = getSrsHoppingBandWidthValue(uehCb.uehCfgCb.srsCfgDedicated.srsHoppingBandWidth);
  ueCb->ueSrsInfo.ueFreqDomainPos = uehCb.uehCfgCb.srsCfgDedicated.frequecyDomainPosition;
  ueCb->ueSrsInfo.ueDuration      = uehCb.uehCfgCb.srsCfgDedicated.duration;
  ueCb->ueSrsInfo.ueSrsCfgIndex   = uehCrmFindSfConfigIndex(sfIndx);
  ueCb->ueSrsInfo.ueTransmissionComb = combIndx;
  ueCb->ueSrsInfo.ueDedCyclicShift = csIndx;

  logInfo("UE SRS config, SrsBandwidth:%d SrsHoppingBw:%d, FreqDomainPos:%d, Duration:%d, SrsCfgIndex:%d, TransmissionComb:%d, DedCyclicShift:%d",
      ueCb->ueSrsInfo.ueSrsBandwidth, ueCb->ueSrsInfo.ueSrsHoppingBw, ueCb->ueSrsInfo.ueFreqDomainPos,
      ueCb->ueSrsInfo.ueDuration, ueCb->ueSrsInfo.ueSrsCfgIndex, ueCb->ueSrsInfo.ueTransmissionComb, ueCb->ueSrsInfo.ueDedCyclicShift);
}

void uehCrmDeallocateSrs(
    UINT16      sfIndx,
    UINT16      combIndx,
    UINT16      csIndx
    )
{
  UINT16      srsPeriodicity = uehGetSrsPeriodicity();

  /* IF SRS Periodicity is SRSPERIODICITY_SRSINV,
   * this is a special case, Don't do any SRS Handling */
  if( ( srsPeriodicity EQ SRSPERIODICITY_SRSINV) ||
      (NULL EQ gSrsSfTable) )
  {
    return;
  }

  /* Update the table,
   * Update the loads */
  gSrsSfTable[sfIndx].comb[combIndx].cShift[csIndx] = 0;
  gSrsSfTable[sfIndx].comb[combIndx].load -= 1;
  gSrsSfTable[sfIndx].load -=1;
}
