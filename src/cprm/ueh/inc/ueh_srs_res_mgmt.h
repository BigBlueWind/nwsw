/**
 * @file    ueh_srs_res_mgmt.h
 * @brief API for UEH SRS Resource Management
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author         : Lakshmi Narayana Madala
 * Date           :
 * Change History :
 */


#ifndef UEH_SRS_RES_MGMT_H
#define UEH_SRS_RES_MGMT_H

#include "basic_types.h"


#define MAX_SRS_CYCLIC_SHIFT            8
#define MAX_SRS_COMBS                   2
#define MAX_SRS_PERIOD_ENUM             10

typedef struct s_CombSrs
{
  UINT32        cShift[MAX_SRS_CYCLIC_SHIFT];
  UINT16        load;
}CombSrs;

typedef struct s_SfSrsTable
{
  CombSrs       comb[MAX_SRS_COMBS];
  UINT32        load;
}SrsSfTable;

typedef struct s_SfPeriodSfCfgIndxMap
{
  UINT16        sfPeriod;
  UINT16        sfCfgIdxStart;
}SfPeriodSfCfgIndxMap;

typedef struct s_SfPeriodSfIndxTable
{
  SfPeriodSfCfgIndxMap        sfPeriodSfCfgIndxMap[MAX_SRS_PERIOD_ENUM];
}SfPeriodSfIndxTable;

typedef struct s_UeSrsInfo
{
  UINT16 ueSrsBandwidth;
  UINT16 ueSrsHoppingBw;
  UINT16 ueFreqDomainPos;
  UINT8  ueDuration;
  UINT16 ueSrsCfgIndex;
  UINT16 ueTransmissionComb;
  UINT16 ueDedCyclicShift;
}UeSrsInfo;

SINT16 uehGetSrsPeriodicity(void);
SINT16 uehCrmDeinitializeSfSrsTables( void );
SINT16 uehCrmInitializeSfSrsTables( void );
void uehSetSrsPeriodicity(
    SINT16 srsPeriod
    );
SINT16 uehCrmFindLeastLoadedSrsCyclicShift(
    UINT16      sfIndex,
    UINT16      combIndex
    );
SINT16 uehCrmFindSfConfigIndex(
    UINT16      sfIndex
    );
SINT16 uehCrmFindLeastLoadedSrsComb(
    UINT16      sfIndex
    );
SINT16 uehCrmFindLeastLoadedSrsSf(
    UINT16      srsPeriodicity
    );
SINT16 uehCrmFindSfIndex(
    UINT16      sfCfgIndex
    );

#endif /* UEH_SRS_RES_MGMT_H */
