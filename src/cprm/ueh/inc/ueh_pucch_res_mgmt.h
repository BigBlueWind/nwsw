/**
 * @file    ueh_pucch_res_mgmt.h
 * @brief API for UEH PUCCH Resource Management 
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author         : Lalit Chhabra
 * Date           :
 * Change History :
 */


#ifndef PUCCH_RESOURCE_MGMT_H
#define PUCCH_RESOURCE_MGMT_H

#include "basic_types.h"

#define NRBCQI                  1       // This is default value -  not operator configurable
#define CSGAP                   1       // For release 1 this is recommended value - not operator configurable
#define CQIPERIODICITY          20      // operator configurable
#define MAXCSPERRB              12      // this is fixed as per standard - this is for CQI
#define MAXSRICSPERRB           1     // this is fixed as per standard - this is for SRI
#define SRIPERIODICITY          20      // operator configurable
#define CQISRIOFFSET            1       // CQI to SRI sf offset in subframes

/* CQI/PMI and RI tables */
typedef struct s_PucchRegionResCqiRi
{
  SINT32         ueId;                 // -1 means not allocated; valid range = 1..MAXUE
  UINT16         cqiRscIndex;            // cqi-resource-index 0..1023
  UINT16         cqipmiConfigIndex;      // cqi-pmi-config-index
}PucchRegionResCqiRi;

typedef struct s_RegionLoadInfo
{
  SINT16   *sfLoadTable;                // Subframe load per region
  SINT32   regionLoad;                  // region load
}RegionLoadInfo;

typedef enum s_LoadUpdateOperation
{
  UE_ADD,                               // add RI + CQI
  UE_DEL                                // delete RI + CQI
}LoadUpdateOperation;

/* SRI tables */
typedef struct s_PucchRegionResSri
{
  SINT32        ueId;                   // -1 means not allocated; valid range = 1..MAXUE
  UINT16        sriRscIndex;            // sr-PUCCH-ResourceIndex INTEGER (0..2047)
  UINT16        sriConfigIndex;         // sr-ConfigIndex INTEGER (0..155)
}PucchRegionResSri;


typedef struct s_CqipmiriInfo
{
  SINT16        cqiPucchRscIndex;
  SINT16        cqipmiConfigIndex;
  SINT16        riConfigIndex;
}CqipmiriInfo;

typedef struct s_SrInfo
{
  SINT16        srPucchRscIndex;
  SINT16        srConfigIndex;
}SrInfo;
	
typedef struct s_AcParams
{ 
  TBOOL         isAllocated;
  CqipmiriInfo 	cqipmiriInfo;
  SrInfo	srInfo;
}AcParams;

typedef enum s_CqiReportModeAperiodic
{
  rm12,
  rm20,
  rm22,
  rm30,
  rm31,
  spare3,
  spare2,
  spare1
}CqiReportModeAperiodic;

typedef enum s_NomPDSCHRSEPREOffset
{
  val_neg1 = -1,
  val_0,
  val_1,
  val_2,
  val_3,
  val_4,
  val_5,
  val_6
}NomPdschRsePreOffset;

SINT16 uehCrmDeintializeCqiTables(
    SINT16 nrbcqi,
    SINT16 cqiperiodicity
    );
SINT16 uehCrmInitializeCqiTables(
    SINT16 nrbcqi,
    SINT16 cqiperiodicity
    );
SINT16 uehCrmDeinitializeSriTables(
    SINT16 nrbcqi,
    SINT16 sriperiodicity
    );
SINT16 uehCrmInitializeSriTables(
    SINT16 nrbcqi,
    SINT16 sriperiodicity
    );
SINT16 uehCrmFindLeastLoadedCqiRiRegion(void);
SINT16 uehCrmFindLeastLoadedCqiRiSf(
    SINT16 region
    );
SINT16 uehCrmGetFreeCqiRiIndex(
    SINT16 region,
    SINT16 sf
    );
SINT16 uehCrmGetFreePucchCqiPmiRes(void);
SINT16 uehCrmFindLeastLoadedSrRegion(void);
SINT16 uehCrmGetFreeSriIndex(
    SINT16 region,
    SINT16 sfOffset
    );
void uehCrmUpdateCqiRiLoad(
    SINT16              freeIndex,
    LoadUpdateOperation opt
    );
SINT16 uehCrmAllocatePucchCqiRi(
    SINT16 freeIndex,
    SINT16 ueId
    );
SINT16 uehCrmDeallocatePucchCqiRi(
    SINT16 ueid
    );
void uehCrmUpdateSriLoad(
    SINT16              freeIndex,
    LoadUpdateOperation opt
    );
SINT16 uehCrmAllocatePucchSri(
    SINT16 freeIndex,
    SINT16 ueId
    );
void uehCrmUpdateSriLoad(
    SINT16              freeIndex,
    LoadUpdateOperation opt
    );
SINT16 uehCrmAllocatePucchSri(
    SINT16 freeIndex,
    SINT16 ueId
    );
SINT16 uehCrmDeallocatePucchSri(
    SINT16 ueId
    );

SINT16 uehCrmGetCqiConfigIndexStartValue(
    SINT16 periodicity
    );
SINT16 uehCrmGetSriConfigIndexStartValue(
    SINT16 periodicity
    );
PUBLIC SINT16 uehCrmDeinitializePucchResTables(void);
S16 uehCrmInitializePucchResTables(void);
SINT16 uehCrmGetFreeSriCsSf(
    SINT16 regionIndex,
    SINT16 riIndexInSrTable,
    SINT16 cqiIndexInSrTable
    );


#endif /* PUCCH_RESOURCE_MGMT_H */
