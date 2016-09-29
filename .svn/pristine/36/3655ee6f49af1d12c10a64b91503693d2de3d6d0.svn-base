/**
 * @file    ueh_pucch_res_mgmt.c
 * @brief This file contains the functions for handling pucch resource management
 *
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Lalit Chhabra, 26 Oct 2010 :
*     a) Created initial version
 */

#include "ccpu_cmn.h"

/* RRM related includes */
#include "ueh_eut.h"
#include "lueh.h"
#include "lueh.x" 
#include "ueh.h" 
#include "ueh_crm.h"
#include "ueh_pucch_res_mgmt.h"
#include "log.h"

/* Globals */
PucchRegionResCqiRi     *pucchCqiResTable;
RegionLoadInfo          *pucchCqiLoadTable;
PucchRegionResSri       *pucchSriResTable;
RegionLoadInfo          *pucchSriLoadTable;
SINT32                   cqipmiConfigIndex_start;
SINT32                   sriConfigIndex_start;

//BS-1664
extern U8  uehGetCqiPeriodicity[];
extern U8  uehGetSriPeriodicity[];


SINT16 uehCrmDeintializeCqiTables(
    SINT16 nrbcqi,
    SINT16 cqiperiodicity
    )
{
  SINT16 i=0, maxCqiSfIndex;

  maxCqiSfIndex = cqipmiConfigIndex_start + cqiperiodicity;

  /* step1:
   * deallocate memory for the resource table
   * Allocate memory for CQI SF load table
   */
  for(i=0; i<nrbcqi; i++)
  {
    if(NULL NOTEQ pucchCqiLoadTable[i].sfLoadTable)
    {
      UEH_FREE( (pucchCqiLoadTable[i].sfLoadTable), sizeof(SINT16)*cqiperiodicity);
    }
  }
  logInfo("%s", "Freed pucchCqiLoadTable.sfLoadTable");

  if(NULL NOTEQ pucchCqiResTable)
  {
    UEH_FREE( pucchCqiResTable, sizeof(PucchRegionResCqiRi) * (cqiperiodicity * nrbcqi * (MAXCSPERRB / CSGAP)));
    logInfo("%s", "Freed pucchCqiResTable");
  }

  if(NULL NOTEQ pucchCqiLoadTable)
  {
    UEH_FREE( pucchCqiLoadTable, sizeof(RegionLoadInfo)*nrbcqi);
    logInfo("%s", "Freed pucchCqiLoadTable");
  }

  return 0;
}

/* This function initialises the CQI tables.
 * It should be called during cell setup procedure.
 * Returns: -1 in case of error 0 in case of success
 */
SINT16 uehCrmInitializeCqiTables(
    SINT16 nrbcqi,
    SINT16 cqiperiodicity
    )
{
  SINT16 i=0, maxCqiSfIndex, regionIndex, sfIndex, csIndex;

  maxCqiSfIndex = cqipmiConfigIndex_start + cqiperiodicity;

  /* step1:
   * Allocate memory for the resource table
   * Allocate memory for CQI SF load table
   */
  if(pucchCqiResTable) 
  {
    for(i=0; i<nrbcqi; i++)
    {
      if(pucchCqiLoadTable[i].sfLoadTable) UEH_FREE(pucchCqiLoadTable[i].sfLoadTable, sizeof(SINT16) * cqiperiodicity);
    }
    UEH_FREE(pucchCqiResTable, sizeof(PucchRegionResCqiRi)*(cqiperiodicity*nrbcqi* (MAXCSPERRB / CSGAP)));
  }
  UEH_ALLOC(&pucchCqiResTable, sizeof(PucchRegionResCqiRi)*(cqiperiodicity*nrbcqi* (MAXCSPERRB / CSGAP)));
  if(NULL EQ pucchCqiResTable)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(sizeof(PucchRegionResCqiRi) * (cqiperiodicity * nrbcqi * (MAXCSPERRB / CSGAP)) , "CQI/PMI Region table");
    return -1;
  }
  if(pucchCqiLoadTable) UEH_FREE(pucchCqiLoadTable, sizeof(RegionLoadInfo)*nrbcqi);
  UEH_ALLOC(&pucchCqiLoadTable, sizeof(RegionLoadInfo)*nrbcqi);
  if(NULL EQ pucchCqiLoadTable)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(sizeof(RegionLoadInfo)*nrbcqi , "CQI/PMI Region Load Info");
    /* Free already allocated memory */
    UEH_FREE( pucchCqiResTable, sizeof(PucchRegionResCqiRi) * (cqiperiodicity * nrbcqi * (MAXCSPERRB / CSGAP)));
    return -1;
  }
  for(i=0; i<nrbcqi; i++) 
  {
    UEH_ALLOC(&(pucchCqiLoadTable[i].sfLoadTable), sizeof(SINT16) * cqiperiodicity);
    if(NULL EQ pucchCqiLoadTable[i].sfLoadTable)
    {
      UEH_ERROR_MEM_ALLOC_FAILED(sizeof(S16) * cqiperiodicity , "SF Load Table for CQI");
      /* Free already allocated memory */
      UEH_FREE( pucchCqiResTable, sizeof(PucchRegionResCqiRi) * (cqiperiodicity * nrbcqi * (MAXCSPERRB / CSGAP)));
      UEH_FREE( pucchCqiLoadTable, sizeof(RegionLoadInfo)*nrbcqi);
      return -1;
    }
  }

  /* step2:
   * Initialise the table
   * initialise CQI SF load tables
   */
  for( regionIndex = 0; regionIndex < nrbcqi; regionIndex++)
  {
    for( sfIndex = 0; sfIndex < cqiperiodicity; sfIndex++)
    {
      pucchCqiLoadTable[regionIndex].sfLoadTable[sfIndex] = 0;
    }
    pucchCqiLoadTable[regionIndex].regionLoad = 0;
  }

  for( i=0, csIndex=0; csIndex<MAXCSPERRB; csIndex+=CSGAP )
  { /* distribution order -> RB, time, CS */
    for( sfIndex=cqipmiConfigIndex_start; sfIndex<maxCqiSfIndex; sfIndex++)
     {
      for( regionIndex=0; regionIndex<(nrbcqi) ;regionIndex++, i++)
      {
        pucchCqiResTable[i].ueId        = -1;
        pucchCqiResTable[i].cqiRscIndex = (regionIndex*MAXCSPERRB) + csIndex;
        pucchCqiResTable[i].cqipmiConfigIndex = sfIndex;
      }
    }
  }
  return 0;
}

SINT16 uehCrmDeinitializeSriTables(
    SINT16 nrbcqi,
    SINT16 sriperiodicity
    )
{
  SINT16 i,  maxSriSfIndex;

  maxSriSfIndex = sriConfigIndex_start + sriperiodicity;

  /* step1:
   * Deallocate memory for SRI load table
   * Deallocate memory for the resource table
   */

  for(i=0; i<nrbcqi; i++)
  {
    if(NULL NOTEQ pucchSriLoadTable[i].sfLoadTable)
    {
      UEH_FREE( (pucchSriLoadTable[i].sfLoadTable), sizeof(SINT16)*sriperiodicity);
    }
  }

  if(NULL NOTEQ pucchSriResTable)
  {
    UEH_FREE( pucchSriResTable, sizeof(PucchRegionResSri)*(sriperiodicity*nrbcqi* (MAXSRICSPERRB/CSGAP)));
  }
  if( NULL NOTEQ pucchSriLoadTable)
  {
    UEH_FREE( pucchSriLoadTable, sizeof(RegionLoadInfo)*nrbcqi);
  }

  return 0;
}

/* This function initialises the SRI tables.
 * It should be called during cell setup procedure.
 * Returns: -1 in case of error 0 in case of success
 */
SINT16 uehCrmInitializeSriTables(
    SINT16 nrbcqi,
    SINT16 sriperiodicity
    )
{
  SINT16 i,  maxSriSfIndex, regionIndex, sfIndex, csIndex;

  maxSriSfIndex = sriConfigIndex_start + sriperiodicity; 

  /* step1:
   * Allocate memory for the resource table
   * Allocate memory for SRI load table */
  if(pucchSriResTable)
  {
    for(i = 0;i < nrbcqi; i++)
    {
      if(pucchSriLoadTable[i].sfLoadTable) UEH_FREE(pucchSriLoadTable[i].sfLoadTable, sizeof(SINT16)*sriperiodicity);
    }
    UEH_FREE(pucchSriResTable, sizeof(PucchRegionResSri)*(sriperiodicity*nrbcqi* (MAXSRICSPERRB/CSGAP)));
  }
  UEH_ALLOC(&pucchSriResTable, sizeof(PucchRegionResSri)*(sriperiodicity*nrbcqi* (MAXSRICSPERRB/CSGAP)));
  if(NULL EQ pucchSriResTable)
  {
    return -1;
  }
  if(pucchSriLoadTable) UEH_FREE(pucchSriLoadTable, sizeof(RegionLoadInfo)*nrbcqi);
  UEH_ALLOC(&pucchSriLoadTable, sizeof(RegionLoadInfo)*nrbcqi);
  if(NULL EQ pucchSriLoadTable)
  {
    /* Free already allocated memory */
    UEH_FREE( pucchSriResTable, sizeof(PucchRegionResSri) * (sriperiodicity*nrbcqi * (MAXSRICSPERRB / CSGAP)));
    return -1;
  }

  for(i = 0;i < nrbcqi; i++) 
  {
    UEH_ALLOC(&(pucchSriLoadTable[i].sfLoadTable), sizeof(SINT16)*sriperiodicity);
    if(NULL EQ pucchSriLoadTable[i].sfLoadTable)
    {
      /* Free already allocated memory */
      UEH_FREE( pucchSriResTable, sizeof(PucchRegionResSri) * (sriperiodicity*nrbcqi * (MAXSRICSPERRB / CSGAP)));
      UEH_FREE( pucchSriLoadTable, sizeof(RegionLoadInfo)*nrbcqi);
      return -1;
    }
  }
  /* step2:
   * Initialise the table
   * initialise CQI SF load tables */
  for( regionIndex=0; regionIndex<nrbcqi; regionIndex++)
  {
    for( sfIndex=0; sfIndex<sriperiodicity; sfIndex++)
    {
      pucchSriLoadTable[regionIndex].sfLoadTable[sfIndex] = 0;
    }
    pucchSriLoadTable[regionIndex].regionLoad = 0;
  }
  for( i=0, csIndex=0; csIndex<MAXSRICSPERRB; csIndex+=CSGAP )
  { /* distribution order -> RB, time, CS */
    for (sfIndex=sriConfigIndex_start;sfIndex<maxSriSfIndex;sfIndex++)
    {
      for (regionIndex=0;regionIndex<(nrbcqi);regionIndex++,i++)
      {
        pucchSriResTable[i].ueId = -1;
        pucchSriResTable[i].sriRscIndex = regionIndex*MAXSRICSPERRB+csIndex;
        pucchSriResTable[i].sriConfigIndex = sfIndex;
      }
    }
  }
  return 0;
}

/* This function finds the first least loaded region from regionLoadTable 
 * Returns: index of least loaded region
 */
SINT16 uehCrmFindLeastLoadedCqiRiRegion(void)
{
  SINT16 regionIndex, lowest;

  for( regionIndex = 0, lowest = 0; regionIndex < uehCb.uehCfgCb.sib2Cfg.nRbCqi; regionIndex++)
  {
    if(pucchCqiLoadTable[lowest].regionLoad > pucchCqiLoadTable[regionIndex].regionLoad)
    {
      lowest = regionIndex;
    }
  }
  return lowest;
}

SINT16 uehCrmFindLeastLoadedCqiRiSf(
    SINT16 region
    )
{
  SINT16 sfIndex=0, lowest;

  for( lowest = 0, sfIndex = 0; sfIndex < uehGetCqiPeriodicity[uehCb.uehCfgCb.cellCfg.cqiPeriodicityIdx]; sfIndex++)
  {
    if(pucchCqiLoadTable[region].sfLoadTable[lowest] > pucchCqiLoadTable[region].sfLoadTable[sfIndex])
    {
      lowest = sfIndex;
    }
  }
  return lowest;
}

/* This function gets the first available CS in the provided region and SF
 * Returns -1 in case of error
 */
SINT16 uehCrmGetFreeCqiRiIndex(
    SINT16 region,
    SINT16 sf
    )
{
  SINT16  indx;
  UINT8   nRbCqi         = uehCb.uehCfgCb.sib2Cfg.nRbCqi;
  SINT16  startIndex     = (sf * nRbCqi) + region;
  SINT16  cqiPeriodicity = uehGetCqiPeriodicity[uehCb.uehCfgCb.cellCfg.cqiPeriodicityIdx];
  
  /*Tack a local variable  for nrbcqi and cqiperiodicity*/
  for( indx = startIndex; indx < nRbCqi * (MAXCSPERRB/CSGAP) * cqiPeriodicity;\
          indx +=  (nRbCqi * cqiPeriodicity) )
  {
    /* search for all CS indices ... */
    if(pucchCqiResTable[indx].ueId == -1)
    {
      logInfo("Free CqiRiIndex:%d", indx);
      return indx;
    }
  }

  UEH_ERROR_RESOURCE_NOT_AVAILABLE("Free CQI PMI Index");
  return -1;
}

/* This function gets the free cqipmiindex from the first least loaded RB + subframe */
SINT16 uehCrmGetFreePucchCqiPmiRes(void)
{
  SINT16 region=0, subframe=0;

  /* Get least loaded region
   * Get least loaded subframe
   * Get the free PUCCH channel based on first available CS for region,sf
   */
  region = uehCrmFindLeastLoadedCqiRiRegion();
  subframe = uehCrmFindLeastLoadedCqiRiSf(region);
  return( uehCrmGetFreeCqiRiIndex(region, subframe) );
}

/* This function finds the first least loaded region from regionLoadTable
 * Returns: index of least loaded region
 */
SINT16 uehCrmFindLeastLoadedSrRegion(void)
{
  SINT16 regionIndex, lowest;

  for(regionIndex = 0, lowest = 0; regionIndex < uehCb.uehCfgCb.sib2Cfg.nRbCqi; regionIndex++)
  {
    if(pucchSriLoadTable[lowest].regionLoad > pucchSriLoadTable[regionIndex].regionLoad)
    {
      lowest = regionIndex;
    }
  }
  logInfo("Least Loaded SRRegion:%d", lowest);
  return lowest;
}

/* This function gets the first available SRI index from the provided region and sf(offset)
 * Returns -1 in case of error
 */
SINT16 uehCrmGetFreeSriIndex(
    SINT16 region,
    SINT16 sfOffset
    )
{
  SINT16 indx;
  UINT8  nRbCqi         = nRbCqi;
  SINT16 startIndex     = (sfOffset * nRbCqi) + region;
  SINT16 sriPeriodicity = uehGetSriPeriodicity[uehCb.uehCfgCb.cellCfg.sriPeriodicityIdx];

  for( indx = startIndex; indx < nRbCqi * (MAXSRICSPERRB/CSGAP) * sriPeriodicity;\
       indx += (nRbCqi * sriPeriodicity))
  {
    /* search for all CS indices ... */
    if(pucchSriResTable[indx].ueId == -1)
    {
      logInfo("Free SRI Index:%d", indx);
      return indx;
    }
  }
  UEH_ERROR_RESOURCE_NOT_AVAILABLE("Free SR Index");
  return -1;
}

/* This function updates the CQI/PMI RI load per region */
void uehCrmUpdateCqiRiLoad(
    SINT16              freeIndex,
    LoadUpdateOperation opt
    )
{
  SINT16 csIndex, sfIndex, regionIndex;

  csIndex = (pucchCqiResTable[freeIndex].cqiRscIndex % MAXCSPERRB) / CSGAP;
  sfIndex = pucchCqiResTable[freeIndex].cqipmiConfigIndex - cqipmiConfigIndex_start;
  regionIndex = pucchCqiResTable[freeIndex].cqiRscIndex / MAXCSPERRB;

  if(UE_ADD EQ opt)
  {
    pucchCqiLoadTable[regionIndex].sfLoadTable[sfIndex]   += 1; // RI
    pucchCqiLoadTable[regionIndex].sfLoadTable[sfIndex+1] += 1; // CQI
    pucchCqiLoadTable[regionIndex].regionLoad += 2;
    logInfo("Add UE | pucchCqiLoadTable| RegionIndex:%d | SfIndex:%d | regionload:%d", regionIndex, sfIndex, pucchCqiLoadTable[regionIndex].regionLoad );
  }
  else
  {
    pucchCqiLoadTable[regionIndex].sfLoadTable[sfIndex]   -= 1;
    pucchCqiLoadTable[regionIndex].sfLoadTable[sfIndex+1] -= 1;
    pucchCqiLoadTable[regionIndex].regionLoad -= 2;
    logInfo("Free UE | pucchCqiLoadTable | RegionIndex:%d | SfIndex:%d | regionload:%d", regionIndex, sfIndex, pucchCqiLoadTable[regionIndex].regionLoad );
  }
}

/* This function allocates a PUCCH resource for CQI/PMI and RI to a UE
 * First the RI is allocated then CQI/PMI index is allocated
 * NOTE: This works only for even value of CQI periodicity
 * This should be OK as we anyhow do not want to use 5ms CQI periodicity 
 * which is the only odd value for CQI periodicity.
 * Returns: -1 in case of failure
 */
SINT16 uehCrmAllocatePucchCqiRi(
    SINT16 freeIndex,
    SINT16 ueId
    )
{
  if(freeIndex NOTEQ -1)
  {
    pucchCqiResTable[freeIndex].ueId = ueId;            // This is the RI allocation
    pucchCqiResTable[freeIndex + uehCb.uehCfgCb.sib2Cfg.nRbCqi].ueId = ueId;     // This is the CQI/PMI allocation
    /* update load info */
    uehCrmUpdateCqiRiLoad( freeIndex,UE_ADD);
    logInfo("Adding a UE:%d at freeIndex:%d in pucchCqiResTable", ueId, freeIndex);
    return 0;
  }

  return -1;
}

/* This function deletes a UE from CQI/PMI RI PUCCH resource table and updates PUCCH load tables
 * Returns: 0 in case of failure
 */
SINT16 uehCrmDeallocatePucchCqiRi(
    SINT16 ueid
    )
{
  SINT16 i;

  for( i = 0; i < (uehGetCqiPeriodicity[uehCb.uehCfgCb.cellCfg.cqiPeriodicityIdx] * uehCb.uehCfgCb.sib2Cfg.nRbCqi * (MAXCSPERRB / CSGAP)); i++)
  {
    if(ueid EQ pucchCqiResTable[i].ueId)
    {
      pucchCqiResTable[i].ueId = -1;            // mark the RI index free
      pucchCqiResTable[i + uehCb.uehCfgCb.sib2Cfg.nRbCqi].ueId = -1;     // mark the CQI index free
      uehCrmUpdateCqiRiLoad( i, UE_DEL);
      logInfo("Freeing a UE:%d at freeIndex:%d in pucchCqiResTable", ueid, i);
      return 0;
    }
  }

  return -1;
}

/* This function updates the SRI load per region */
void uehCrmUpdateSriLoad(
    SINT16              freeIndex,
    LoadUpdateOperation opt
    )
{
  SINT16 csIndex, sfIndex, regionIndex;

  csIndex = (pucchSriResTable[freeIndex].sriRscIndex % MAXSRICSPERRB) / CSGAP;
  sfIndex = pucchSriResTable[freeIndex].sriConfigIndex - sriConfigIndex_start;
  regionIndex = pucchSriResTable[freeIndex].sriRscIndex / MAXSRICSPERRB;

  if(opt EQ UE_ADD)
  {
    pucchSriLoadTable[regionIndex].sfLoadTable[sfIndex] += 1; // SRI
    pucchSriLoadTable[regionIndex].regionLoad           += 1;
    logInfo("Add UE | pucchSriResTable | RegionIndex:%d | SfIndex:%d | regionload:%d", regionIndex, sfIndex,pucchSriLoadTable[regionIndex].regionLoad);
  }
  else
  {
    pucchSriLoadTable[regionIndex].sfLoadTable[sfIndex] -= 1;
    pucchSriLoadTable[regionIndex].regionLoad           -= 1;
    logInfo("Free UE | pucchSriResTable | RegionIndex:%d | SfIndex:%d | regionload:%d", regionIndex, sfIndex,pucchSriLoadTable[regionIndex].regionLoad);
  }
}

/* This function allocates a PUCCH resource for SRI to a UE
 * Returns: -1 in case of failure
 */
SINT16 uehCrmAllocatePucchSri(
    SINT16 freeIndex,
    SINT16 ueId
    )
{
  if(freeIndex != -1)
  {
    pucchSriResTable[freeIndex].ueId = ueId;  // This is the SRI allocation
    uehCrmUpdateSriLoad( freeIndex,UE_ADD );
    logInfo("Adding a UE:%d at freeIndex:%d in pucchSriResTable", ueId, freeIndex);
    return 0;
  }

  return -1;
}

/* This function deletes a UE from SRI PUCCH resource table and updates SRI PUCCH load tables 
 * Returns: 0 in case of failure
 */
SINT16 uehCrmDeallocatePucchSri(
    SINT16 ueId
    )
{
  SINT16 i;

  for( i = 0; i < (uehGetSriPeriodicity[uehCb.uehCfgCb.cellCfg.sriPeriodicityIdx] * uehCb.uehCfgCb.sib2Cfg.nRbCqi * (MAXSRICSPERRB / CSGAP)); i++)
  {
    if(ueId EQ pucchSriResTable[i].ueId)
    {
      pucchSriResTable[i].ueId = -1;    // mark the SRI index free
      uehCrmUpdateSriLoad( i, UE_DEL );
      logInfo("Freeing a UE:%d at freeIndex:%d in pucchSriResTable", ueId, i);
      return 0;
    }
  }

  return -1;
}

/* This function returns the start value for cqi-pmi-ConfigIndex 
 * based on Table 7.2.2-1A of 36.213.
 * returns -1 in case the provided periodicity is invalid.
 */
SINT16 uehCrmGetCqiConfigIndexStartValue(
    SINT16 periodicity
    )
{
  switch(periodicity)
  {
    case 2:
      return 0;
    case 5:
      return 2;
    case 10:
      return 7;
    case 20:
      return 17;
    case 40:
      return 37;
    case 80:
      return 77;
    case 160:
      return 157;
    case 32:
      return 318;
    case 64:
      return 350;
    case 128:
      return 414;
    default:
      return -1;
  }
}

/* This function returns the start value for SR configuration Index   
 * based on Table 10.1-5 of 36.213.
 * returns -1 in case the provided periodicity is invalid.
 */
SINT16 uehCrmGetSriConfigIndexStartValue(
    SINT16 periodicity
    )
{
  switch(periodicity)
  {
    case 5:
      return 0;
    case 10:
      return 5;
    case 20:
      return 15;
    case 40:
      return 35;
    case 80:
      return 75;
    default:
      return -1;
  }
}

PUBLIC SINT16 uehCrmDeinitializePucchResTables(void)
{
  uehCrmDeintializeCqiTables(uehCb.uehCfgCb.sib2Cfg.nRbCqi, uehGetCqiPeriodicity[uehCb.uehCfgCb.cellCfg.cqiPeriodicityIdx]); 
  uehCrmDeinitializeSriTables(uehCb.uehCfgCb.sib2Cfg.nRbCqi, uehGetSriPeriodicity[uehCb.uehCfgCb.cellCfg.sriPeriodicityIdx]);

  return 0;
}

/* This function initializes PUCH CQI and SRI resource/load tables 
 * Return -1 in case of error 0 in case of success
 */
PUBLIC SINT16 uehCrmInitializePucchResTables(void)
{
  cqipmiConfigIndex_start       = uehCrmGetCqiConfigIndexStartValue(uehGetCqiPeriodicity[uehCb.uehCfgCb.cellCfg.cqiPeriodicityIdx]); 
  sriConfigIndex_start          = uehCrmGetSriConfigIndexStartValue(uehGetSriPeriodicity[uehCb.uehCfgCb.cellCfg.sriPeriodicityIdx]);

  if(-1 EQ cqipmiConfigIndex_start)
  {
    UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE("cqiperiodicity", uehGetCqiPeriodicity[uehCb.uehCfgCb.cellCfg.cqiPeriodicityIdx], "2/5/10/20/40/80/160/32/64/128");
    return -1;
  }
  if(-1 EQ uehCrmInitializeCqiTables( uehCb.uehCfgCb.sib2Cfg.nRbCqi, uehGetCqiPeriodicity[uehCb.uehCfgCb.cellCfg.cqiPeriodicityIdx]) )
  {
    return -1;
  }
  if(-1 EQ uehCrmInitializeSriTables( uehCb.uehCfgCb.sib2Cfg.nRbCqi, uehGetSriPeriodicity[uehCb.uehCfgCb.cellCfg.sriPeriodicityIdx]) )
  {
    return -1;
  }

  return 0;
}

/* This function gets the first free CS in the first available SF that is not occupied
 * by CQI/PMI or RI.
 * Returns -1 in case of error
 */
SINT16 uehCrmGetFreeSriCsSf(
    SINT16 regionIndex,
    SINT16 riIndexInSrTable,
    SINT16 cqiIndexInSrTable
    )
{
  SINT16  sfIndex, csIndex, i,srTableIndex;
  UINT8   nRbCqi         = uehCb.uehCfgCb.sib2Cfg.nRbCqi;
  SINT16  sriPeriodicity = uehGetSriPeriodicity[uehCb.uehCfgCb.cellCfg.sriPeriodicityIdx];
  
  /* outer loop is on CS,
   * i.e. try to get first available SF in CS0, then CS1, then CS2 ...
   */
  for( csIndex = 0, i = 0; i < MAXSRICSPERRB/CSGAP; i++, csIndex += (nRbCqi * sriPeriodicity))
  {
    for( sfIndex = 0; sfIndex < sriPeriodicity; sfIndex++)
    {
      srTableIndex = csIndex + regionIndex + (sfIndex * nRbCqi);
      if( pucchSriResTable[srTableIndex].ueId == -1 && \
          ( (pucchCqiResTable[srTableIndex].cqipmiConfigIndex - cqipmiConfigIndex_start) % sriPeriodicity != riIndexInSrTable) && \
          ( (pucchCqiResTable[srTableIndex].cqipmiConfigIndex - cqipmiConfigIndex_start) % sriPeriodicity != cqiIndexInSrTable) )
      {
        logInfo("riIndexInSrTable:%d, cqiIndexInSrTable:%d, I:%d\n", riIndexInSrTable, cqiIndexInSrTable, i );
        return srTableIndex;
      }
    }
  }
  UEH_ERROR_RESOURCE_NOT_AVAILABLE("FreeSriCsSf");
  return -1;
}

/* This function adds a UE to the CQI/PMI/RI and SRI resource tables
 * and updates the CQI/PMI/RI and SRI load tables
 *
 * ON success:
 * - Returns ROK
 * - Fills acParams
 * ON failure:
 * - Returns RFAILED
 * - acParams is untouched
 */
TBOOL uehCrmAddUe(
    S16       ueId,
 IO AcParams *fillme

  )
{
  SINT16 sfIndex=0, regionIndex=0, csIndex=0, cqiIndexInSrTable=0, riIndexInSrTable=0, freeIndex = -1, cqiTableIndex, sriTableIndex;

  freeIndex = uehCrmGetFreePucchCqiPmiRes();
  if(-1 EQ uehCrmAllocatePucchCqiRi(freeIndex,ueId)) RETVALUE(RFAILED);

  /* Now allocate SRI */
  /* first, remember the CQI and RI SF in the SRI table */

    cqiTableIndex = freeIndex;   

    csIndex = (pucchCqiResTable[freeIndex].cqiRscIndex % MAXCSPERRB) / CSGAP; 
    sfIndex = pucchCqiResTable[freeIndex].cqipmiConfigIndex - cqipmiConfigIndex_start;
    regionIndex = pucchCqiResTable[freeIndex].cqiRscIndex / MAXCSPERRB;
    riIndexInSrTable = sfIndex;
    cqiIndexInSrTable = sfIndex+1;
    logInfo("SR|RIIndex=%d|cqiIndex=%d", riIndexInSrTable, cqiIndexInSrTable );
    /* next, find least loaded region. Note that the blocking above does not update the load */
    regionIndex = uehCrmFindLeastLoadedSrRegion();

    /* next, get free sf such that the sf is not used in CQI or RI for same UE */
    freeIndex = uehCrmGetFreeSriCsSf(regionIndex, riIndexInSrTable, cqiIndexInSrTable);

    sriTableIndex = freeIndex;

    if(-1 EQ uehCrmAllocatePucchSri(freeIndex,ueId)) 
    {
      UEH_PROCEDURE_FAILED( "Allocating PUCCH resource", UE, ueId, "No available resource");
      if(-1 EQ uehCrmDeallocatePucchCqiRi(ueId))
      {
        UEH_PROCEDURE_FAILED( "De allocating CQI/PMI/RI ", UE, ueId, "UE ID not found in resource alloc table");
      }
      return RFAILED;
    }

    //fill up Admission Control Params 
    fillme->isAllocated = TRUE;
    fillme->cqipmiriInfo.cqiPucchRscIndex = pucchCqiResTable[cqiTableIndex].cqiRscIndex;
    fillme->cqipmiriInfo.cqipmiConfigIndex = pucchCqiResTable[cqiTableIndex].cqipmiConfigIndex + 1;
    fillme->cqipmiriInfo.riConfigIndex = pucchCqiResTable[cqiTableIndex].cqipmiConfigIndex;
    fillme->srInfo.srPucchRscIndex = pucchSriResTable[sriTableIndex].sriRscIndex;
    fillme->srInfo.srConfigIndex = pucchSriResTable[sriTableIndex].sriConfigIndex;
    return ROK;

}

/* This function deleted a UE resources for CQI/PMI RI and SRI 
 * Note that if CQI/RI deallocation succeeds but SRI deallocation fails, 
 * then roll back (realocation of CQI/RI) is not done.
 * hence this should never fail (to be ensured in CUT phase 
 * Returns 0 in case of error
 */
PUBLIC SINT16 uehCrmDeleteUe(
    SINT16 ueId
    )
{
  UECM_FUNC(ueId,uehCrmDeleteUe,UECM,CRM,"Deleting a Ue");

  if(-1 EQ uehCrmDeallocatePucchCqiRi(ueId))
  {
    UEH_PROCEDURE_FAILED( "De allocating CQI/PMI/RI ", UE, ueId, "UE ID not found in resource alloc table");
    return 0;
  }
  if( -1 EQ uehCrmDeallocatePucchSri(ueId))
  {
    UEH_PROCEDURE_FAILED( "De allocating CQI/PMI/RI ", UE, ueId, "UE ID not found in resource alloc table");
    return 0;
  } 
  return 1;
}

