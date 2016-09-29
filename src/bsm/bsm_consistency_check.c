/**
 * @file    bsm_consistency_check.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions for consistency check for all tables.
 *
 * @author:  Shweta Polepally
 * @date:    Nov 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @details This function will return the value of UL_RB based on the UlBandwidth value of
 *          CellConfig Table.
 */
static UINT8 getUlRbsValue()
{
  CellConfig  *cellConfig = (CellConfig*)getFirstRowOfTable(CELLCONFIG);
  switch( cellConfig->ulBandwidth )
  {
    case ULBANDWIDTH_1DOT4MHZ:   return 6;
    case ULBANDWIDTH_3MHZ:       return 15;
    case ULBANDWIDTH_5MHZ:       return 25;
    case ULBANDWIDTH_10MHZ:      return 50;
    case ULBANDWIDTH_15MHZ:      return 75;
    case ULBANDWIDTH_20MHZ:      return 100;
    case ULBANDWIDTH_NOTDEFINED: return FALSE;
  }

  return FALSE;
}

/**
 * @details This function will return the value of UL_RB based on the UlBandwidth value of
 *          CellConfig Table.
 */
static UINT8 getDlRbsValue()
{
  CellConfig  *cellConfig = (CellConfig*)getFirstRowOfTable(CELLCONFIG);
  switch( cellConfig->dlBandwidth )
  {
    case DLBANDWIDTH_1DOT4MHZ:  return 6;
    case DLBANDWIDTH_3MHZ:      return 15;
    case DLBANDWIDTH_5MHZ:      return 25;
    case DLBANDWIDTH_10MHZ:     return 50;
    case DLBANDWIDTH_15MHZ:     return 75;
    case DLBANDWIDTH_20MHZ:     return 100;
  }
  return FALSE;
}

/**
 * @param   numRaPreambles Number of Ra Preambles enum value.
 * @details This function will return the value of numOfPreambles based on numRaPreambles
 *          field of SysInfo2Config Table.
 */
static UINT8 getNumRaPreamblesValue(
  IN NumRaPreambles   numRaPreambles
  )
{
  switch( numRaPreambles )
  {
    case NUMRAPREAMBLES_N4:  return 4;
    case NUMRAPREAMBLES_N8:  return 8;
    case NUMRAPREAMBLES_N12: return 12;
    case NUMRAPREAMBLES_N16: return 16;
    case NUMRAPREAMBLES_N20: return 20;
    case NUMRAPREAMBLES_N24: return 24;
    case NUMRAPREAMBLES_N28: return 28;
    case NUMRAPREAMBLES_N32: return 32;
    case NUMRAPREAMBLES_N36: return 36;
    case NUMRAPREAMBLES_N40: return 40;
    case NUMRAPREAMBLES_N44: return 44;
    case NUMRAPREAMBLES_N48: return 48;
    case NUMRAPREAMBLES_N52: return 52;
    case NUMRAPREAMBLES_N56: return 56;
    case NUMRAPREAMBLES_N60: return 60;
    case NUMRAPREAMBLES_N64: return 64;
  }
  return FALSE;
}

/**
 * @param   sizeRAPreamblesGroupA Size of RA Preambles Group A
 * @details This function will return the actual value of numOfPreambles based on numRaPreamblee enum value.
 */
UINT8 getSizeRAPreamblesGroupAValue(
  IN SizeRAPreamblesGroupA   sizeRAPreamblesGroupA
  )
{
  switch( sizeRAPreamblesGroupA )
  {
    case SIZERAPREAMBLESGROUPA_N4:  return 4;
    case SIZERAPREAMBLESGROUPA_N8:  return 8;
    case SIZERAPREAMBLESGROUPA_N12: return 12;
    case SIZERAPREAMBLESGROUPA_N16: return 16;
    case SIZERAPREAMBLESGROUPA_N20: return 20;
    case SIZERAPREAMBLESGROUPA_N24: return 24;
    case SIZERAPREAMBLESGROUPA_N28: return 28;
    case SIZERAPREAMBLESGROUPA_N32: return 32;
    case SIZERAPREAMBLESGROUPA_N36: return 36;
    case SIZERAPREAMBLESGROUPA_N40: return 40;
    case SIZERAPREAMBLESGROUPA_N44: return 44;
    case SIZERAPREAMBLESGROUPA_N48: return 48;
    case SIZERAPREAMBLESGROUPA_N52: return 52;
    case SIZERAPREAMBLESGROUPA_N56: return 56;
    case SIZERAPREAMBLESGROUPA_N60: return 60;
  }
  return FALSE;
}

/**
 * @param   modificationPeriodCoeff Modification Period Coeff Value/
 * @details This function will return actual value of modificationPeriodCoeff
 *          from modificationPeriodCoeff enum value.
 */
UINT32 getModificationPeriodCoeffValue(
  IN ModificationPeriodCoeff  modificationPeriodCoeff
  )
{
  switch( modificationPeriodCoeff )
  {
    case MODIFICATIONPERIODCOEFF_N2:  return 2;
    case MODIFICATIONPERIODCOEFF_N4:  return 4;
    case MODIFICATIONPERIODCOEFF_N8:  return 8;
    case MODIFICATIONPERIODCOEFF_N16: return 16;
  }
  return FALSE;
}

/**
 * @param   defaultPagCycle  Default Page Cycle.
 * @details This function will return actual value of defaultPagCycle from defaultPagCycle
 *          enum value.
 */
UINT32 getDefaultPagCycle(
  IN DefaultPagCycle  defaultPagCycle
  )
{
  switch( defaultPagCycle )
  {
    case DEFAULTPAGCYCLE_RF32:   return 32;
    case DEFAULTPAGCYCLE_RF64:   return 64;
    case DEFAULTPAGCYCLE_RF128:  return 128;
    case DEFAULTPAGCYCLE_RF256:  return 256;
  }
  return FALSE;
}

/**
 * @param startRntiVal    StartRnti Value.
 * @param maxConnectedUes Max Connected Ues
 * @details This function will add the values and check the value across its valid range
 */
static TBOOL doConsistencyCheckOnCellConfigStartRntiAndMaxConnectedUes(
  IN UINT16   startRntiVal,
  IN UINT16   maxConnectedUes
  )
{
  if((BSM_UEH_MAX_RNTI - UEH_RESERVED_RNTI_POOL_SIZE) < ((startRntiVal - 1) + maxConnectedUes))
  {
    return FALSE;
  }
  return TRUE;
}

/**
 * @param ulEarfcn    Value of ulEarfcn
 * @details This function will perform consistency check on Cell Config UL Earfcn field.
 */
static TBOOL doConsistencyCheckOnCellConfigUlEarfcn(
  IN UINT32  ulEarfcn
  )
{
  BsmOamTableInfo *oamTableInfo = bsmGetTablePtrByTableId(CELLCONFIG);
  if(oamTableInfo EQ NULL)
  {
    /* TODO: log error here */
    return FALSE;
  }
  OamColInfo  *oamColInfo = &oamTableInfo->colInfo[COL_CELLCONFIG_ULEARFCN];

  if ( ulEarfcn >= oamColInfo->colMinVal && ulEarfcn <= oamColInfo->colMaxVal )
  {
    return TRUE;
  }
  else
  {
    /* Check is any special value is available for this field. IF yes, check across that
     * value and then send fail if still not correct */
    if ( ( oamColInfo->isSpecialValuePresent EQ TRUE ) && (oamColInfo->specialValue NOTEQ NULL) )
    {
      UINT32 specialValue = atoi(oamColInfo->specialValue);
      if ( specialValue EQ ulEarfcn )
        return TRUE;
    }
    return FALSE;
  }
}

/**
 * @param    siRetxCnt    Value of siRetxCnt
 * @details This function will perform consistency check on SysInfo1Config siRetxCnt field.
 */
static TBOOL doConsistencyCheckOnSiReTxCnt(
  IN UINT32  siRetxCnt
  )
{
  SysInfo1Config  *sys1Config = (SysInfo1Config*)getFirstRowOfTable(SYSINFO1CONFIG);
  if ( siRetxCnt < sys1Config->siWindowLength )
    return TRUE;
  else
    return FALSE;
}

/**
 * @param   modificationPeriodCoeff  Value from table
 * @param   defaultPagCycle          Value from table
 * @details This function will check the values across each other.
 *          modificationPeriodCoeff *  defaultPagCycle <= 1024
 */
static TBOOL doConsistencyCheckOnSysInfo2ConfigModPeriodCoeffAndDefaultPagCycle(   /* Give some name here */
  IN ModificationPeriodCoeff   modificationPeriodCoeff,
  IN DefaultPagCycle           defaultPagCycle
  )
{
  if ( (getModificationPeriodCoeffValue(modificationPeriodCoeff)*getDefaultPagCycle(defaultPagCycle)) <= 1024 )
    return TRUE;
  else
    return FALSE;
}

/**
 * @param numRaPreambles  Value from table
 * @param sizeRaPreambles Value from table
 * @details This function will check the values across each other
 *           sizeRaPreambles <= numRaPreambles
 */
static TBOOL doConsistencyCheckOnSysInfo2ConfigNumAndSizeofPreambles(   /* Give some name here */
  IN NumRaPreambles           numRaPreambles,
  IN SizeRAPreamblesGroupA    sizeRaPreambles
  )
{
  if ( getSizeRAPreamblesGroupAValue(sizeRaPreambles) <= getNumRaPreamblesValue(numRaPreambles) )
    return TRUE;
  else
    return FALSE;
}

/**
 * @param freqOff  Value from table
 * @param rbsValue Value from table
 * @details This function will perform consistency check.
 */
static TBOOL doConsistencyCheckOnPhyConfigPrachFreqOffset(
  IN UINT8     freqOff,
  IN UINT8     rbsValue
  )
{
  if ( freqOff <= (rbsValue - 6) )
    return TRUE;
  else
    return FALSE;
}

/**
 * @param freqOff  Value from table
 * @param rbsValue Value from table
 * @details This function will perform consistency check.
 */
static TBOOL doConsistencyCheckOnCellConfigAntInfoTxMode(
  IN SINT32     numOfTxAnt,
  IN UINT32     newAntInfoTxMode
  )
{
  if( ( numOfTxAnt EQ 1 ) && ( newAntInfoTxMode EQ ANTINFOTXMODE_TM1 ) )
  {
    return TRUE;
  }
  else if( ( numOfTxAnt EQ 2 ) && ( newAntInfoTxMode EQ ANTINFOTXMODE_TM2 ) )
  {
  	return TRUE;
  }
  else if( ( numOfTxAnt EQ 2 ) && ( newAntInfoTxMode EQ ANTINFOTXMODE_TM3 ) )
  {
    return TRUE;
  }
  return FALSE;
}

static TBOOL doConsistencyCheckOnCellConfigFreqBandInd(
  IN UINT32    newFreqBandInd
  )
{
  if( newFreqBandInd EQ 15  || newFreqBandInd EQ 16 )
  {
	return FALSE;
  }
  return TRUE;
}

/**
 * @param tlvArray      TLV Array information.
 * @param failedTlvInfo Failed TLV information to be filled, if any.
 * @details This function will do consistency check on CELL Config table and fill failedTagInfo
 *          tag in case of error. Remove this function after all MIMO options are supported.
 */
TBOOL bsmMimoFeatureCheckOnCellConfig(
  IO TlvArray    *tlvArray,
  IO TlvInfo     *failedTlvInfo
  )
{
  Index    i;
  TlvInfo  *tlv = NULL;
  clearBytes( failedTlvInfo, sizeof(TlvInfo) );

  TlvInfo  *antInfoTxModeTlvInfo = NULL;
  UINT32    newAntInfoTxMode = 0;
  TBOOL     flagAntInfoTxMode  = FALSE;
  CellConfig  *config    = (CellConfig*)getFirstRowOfTable(CELLCONFIG);
  UehConfig   *uehConfig = (UehConfig*)getFirstRowOfTable(UEHCONFIG);

  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_CELLCONFIG_ANTINFOTXMODE:
      {
        newAntInfoTxMode  = getUint32FromArray(tlv->data);
        antInfoTxModeTlvInfo = tlv;
        if( config->antInfoTxMode NOTEQ newAntInfoTxMode )
          flagAntInfoTxMode = TRUE;
      }
      break;
    }
  }
  if( TRUE EQ flagAntInfoTxMode)
  {
    if( FALSE EQ doConsistencyCheckOnCellConfigAntInfoTxMode( uehConfig->numOfTxAnt, newAntInfoTxMode ))
    {
      copyBytes( antInfoTxModeTlvInfo, sizeof(TlvInfo), failedTlvInfo );
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * @param tlvArray      TLV Array information.
 * @param failedTlvInfo Failed TLV information to be filled, if any.
 * @details This function will do consistency check on CELL Config table and fill failedTagInfo
 *          tag in case of error.
 */
TBOOL bsmConsistencyCheckOnCellConfig(
  IO TlvArray    *tlvArray,
  IO TlvInfo     *failedTlvInfo
  )
{
  Index    i;
  TlvInfo  *tlv = NULL;
  clearBytes( failedTlvInfo, sizeof(TlvInfo) );

  TlvInfo  *startRntiTlvInfo    = NULL;
  TlvInfo  *maxConnectedUesTlvInfo = NULL;
  TlvInfo  *antInfoTxModeTlvInfo = NULL;
  TlvInfo  *freqBandIndTlvInfo = NULL;
  UINT16    newStartRnti    = 0;  /* These two values have dependency on each other */
  UINT16    newMaxConnectedUes = 0;
  UINT32    newAntInfoTxMode = 0;
  UINT32    newFreqBandInd = 0;
  TBOOL     flagNewStartRnti    = FALSE;
  TBOOL     flagMaxConectedUes = FALSE;
  TBOOL     flagAntInfoTxMode  = FALSE;
  TBOOL     flagFreqBandInd = FALSE;
  CellConfig  *config = (CellConfig*)getFirstRowOfTable(CELLCONFIG);
  UehConfig   *uehConfig = (UehConfig*)getFirstRowOfTable(UEHCONFIG);

  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_CELLCONFIG_ULEARFCN:
      {
        UINT32 ulEarfcn  = getUint32FromArray(tlv->data);
        if ( FALSE EQ doConsistencyCheckOnCellConfigUlEarfcn( ulEarfcn ) )
        {
          copyBytes( tlv, sizeof(TlvInfo), failedTlvInfo );
          return FALSE;
        }
      }
      break;
      case COL_CELLCONFIG_STARTRNTI:
      {
        newStartRnti     = getUint16FromArray(tlv->data);
        startRntiTlvInfo = tlv;
        if(config->startRnti NOTEQ newStartRnti)
          flagNewStartRnti = TRUE;
      }
      break;
      case COL_CELLCONFIG_MAXCONNECTEDUES:
      {
        newMaxConnectedUes     = getUint16FromArray(tlv->data);
        maxConnectedUesTlvInfo = tlv;
        if(config->maxConnectedUes NOTEQ newStartRnti)
          flagMaxConectedUes = TRUE;
      }
      break;
      case COL_CELLCONFIG_ANTINFOTXMODE:
      {
        newAntInfoTxMode  = getUint32FromArray(tlv->data);
        antInfoTxModeTlvInfo = tlv;
        if( config->antInfoTxMode NOTEQ newAntInfoTxMode )
          flagAntInfoTxMode = TRUE;
      }
      break;
      case COL_CELLCONFIG_FREQBANDIND:
      {
    	newFreqBandInd = getUint32FromArray(tlv->data);
    	freqBandIndTlvInfo = tlv;
    	if( config->freqBandInd NOTEQ newFreqBandInd )
    		flagFreqBandInd = TRUE;
      }
      break;
    }
  }
  if( TRUE EQ flagAntInfoTxMode)
  {
    if( FALSE EQ doConsistencyCheckOnCellConfigAntInfoTxMode( uehConfig->numOfTxAnt, newAntInfoTxMode ))
    {
      copyBytes( antInfoTxModeTlvInfo, sizeof(TlvInfo), failedTlvInfo );
      return FALSE;
    }
  }
  if ( (TRUE EQ flagNewStartRnti) && (TRUE EQ flagMaxConectedUes) )
  {
    /* if both the values are received, compare them across each other */
    if( FALSE EQ doConsistencyCheckOnCellConfigStartRntiAndMaxConnectedUes( newStartRnti, newMaxConnectedUes) )
    {
      copyBytes( startRntiTlvInfo, sizeof(TlvInfo), failedTlvInfo );
      return FALSE;
    }
  }
  else if((TRUE EQ flagNewStartRnti) && (FALSE EQ flagMaxConectedUes) )
  {
    if( FALSE EQ  doConsistencyCheckOnCellConfigStartRntiAndMaxConnectedUes( newStartRnti, config->maxConnectedUes )  )
    {
      copyBytes( startRntiTlvInfo, sizeof(TlvInfo), failedTlvInfo );
      return FALSE;
    }
  }
  else if((FALSE EQ flagNewStartRnti) && (TRUE EQ flagMaxConectedUes) )
  {
    if( FALSE EQ doConsistencyCheckOnCellConfigStartRntiAndMaxConnectedUes( config->startRnti, newMaxConnectedUes ))
    {
      copyBytes( maxConnectedUesTlvInfo, sizeof(TlvInfo), failedTlvInfo );
      return FALSE;
    }
  }
  if( TRUE EQ flagFreqBandInd )
  {
    if( FALSE EQ doConsistencyCheckOnCellConfigFreqBandInd( newFreqBandInd ) )
    {
      copyBytes( freqBandIndTlvInfo, sizeof(TlvInfo), failedTlvInfo );
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * @param tlvArray      TLV Array information.
 * @param failedTlvInfo Failed TLV information to be filled, if any.
 * @param errInfoStr    Error information string
 * @details This function will do consistency check on CELL Config table and fill failedTagInfo
 *          tag in case of error.
 */
TBOOL bsmConsistencyCheckOnEutraNeighCellConfig(
  IO TlvArray    *tlvArray,
  IO TlvInfo     *failedTlvInfo,
  IO UINT8       *errInfoStr
  )
{
  Index    i;
  TlvInfo  *tlv = NULL;
  clearBytes( failedTlvInfo, sizeof(TlvInfo) );

  CellConfig    *cellConfig   = (CellConfig*)getFirstRowOfTable(CELLCONFIG);
  EnodebConfig  *enodebConfig = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);

  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_EUTRANEIGHCELLCONFIG_NEIGHEUTRAENBID:
      {
        UINT32 neighEutraEnbId  = getUint32FromArray(tlv->data);
        if ( neighEutraEnbId EQ enodebConfig->enodebId)
        {
          BS_ERROR_NEIGH_ENBID_SAME_AS_ENBID(enodebConfig->enodebId);
          copyBytes( tlv, sizeof(TlvInfo), failedTlvInfo );
          snprintf(errInfoStr, MAX_TLV_VALUE_IN_BYTES, "Neighbour ENBID is same as self ENBID = %d", enodebConfig->enodebId);
          return FALSE;
        }
      }
      break;
      case COL_EUTRANEIGHCELLCONFIG_NEIGHEUTRACELLID:
      {
        UINT8 neighEutraCellId  = getUint8FromArray(tlv->data);
        if ( neighEutraCellId EQ cellConfig->cellId )
        {
          BS_ERROR_NEIGH_CELLID_SAME_AS_CELLID(cellConfig->cellId);
          copyBytes( tlv, sizeof(TlvInfo), failedTlvInfo );
          snprintf(errInfoStr, MAX_TLV_VALUE_IN_BYTES, "Neighbour CELLID is same as self CELLID = %d", cellConfig->cellId);
          return FALSE;
        }
      }
      break;
    }
  }
  return TRUE;
}
/**
 * @param tlvArray      TLV Array information.
 * @param failedTlvInfo Failed TLV information to be filled, if any.
 * @details This function will do consistency check on Phy Config table and fill failedTagInfo
 *          tag in case of error.
 */
TBOOL bsmConsistencyCheckOnPhyConfig(
  IO TlvArray      *tlvArray,
  IO TlvInfo       *failedTlvInfo,
  IO BsErrorValue  *bsError
  )
{
  Index    i;
  TlvInfo  *tlv = NULL;
  BsmOamTableInfo *oamTableInfo = bsmGetTablePtrByTableId(PHYCONFIG);
  OamColInfo  *oamColInfo = &oamTableInfo->colInfo[COL_PHYCONFIG_TOTALTXPOWER];

  clearBytes( failedTlvInfo, sizeof(TlvInfo) );

  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);

    switch(tlv->tag.colId)
    {
      case COL_PHYCONFIG_PRACHFREQOFFSET:
      {
        UINT8   freqOff = getUint8FromArray(tlv->data);
        if ( FALSE EQ doConsistencyCheckOnPhyConfigPrachFreqOffset( freqOff, getUlRbsValue() ))
        {
          copyBytes( tlv, sizeof(TlvInfo), failedTlvInfo );
          *bsError = BS_ERR_CONSISTENCY_CHECK_FAILED;
          return FALSE;
        }
      }
      break;
      case COL_PHYCONFIG_TOTALTXPOWER:
      {
        UINT8   txPower = getUint8FromArray(tlv->data);
        if( ( txPower > oamColInfo->colMaxVal) || ( txPower < oamColInfo->colMinVal ) )
        {
          copyBytes( tlv, sizeof(TlvInfo), failedTlvInfo );
          *bsError = BS_ERR_TOTALTXPOWER_NOT_IN_RANGE;
          return FALSE;
        }
      }
      break;
    }
  }
  return TRUE;
}

/**
 * @param tlvArray      TLV Array information.
 * @param failedTlvInfo Failed TLV information to be filled, if any.
 * @details This function will do consistency check on SysInfo2 Config table and fill failedTagInfo
 *          tag in case of error.
 */
TBOOL bsmConsistencyCheckOnSysInfo2Config(
  IO TlvArray    *tlvArray,
  IO TlvInfo     *failedTlvInfo
  )
{
  Index    i;
  TlvInfo  *tlv = NULL;
  TlvInfo  *newModificationPeriodCoeffTlvInfo = NULL;
  TlvInfo  *newDefaultPageCycleTlvInfo        = NULL;
  TlvInfo  *newSizeRaPreamblesTlvInfo         = NULL;
  TlvInfo  *newNumRaPreamblesTlvInfo          = NULL;

  NumRaPreambles           newNumRaPreambles          ;
  SizeRAPreamblesGroupA    newSizeRaPreambles         ;
  ModificationPeriodCoeff  newModificationPeriodCoeff ;
  DefaultPagCycle          newDefaultPageCycle        ;

  /* These flags are required because the below values are
     enum and can have value 0. Hence flags are needed for checking fater decoding is done */
  TBOOL   flagNumRaPreambles          = FALSE;
  TBOOL   flagSizeRaPreambles         = FALSE;
  TBOOL   flagModificationPeriodCoeff = FALSE;
  TBOOL   flagDefaultPageCycle        = FALSE;

  SysInfo2Config  *config = (SysInfo2Config*)getFirstRowOfTable(SYSINFO2CONFIG);

  clearBytes( failedTlvInfo, sizeof(TlvInfo) );

  for ( i=0; i < tlvArray->numTlvs; i++ )
  {
    tlv = &(tlvArray->tlvInfo[i]);
    switch(tlv->tag.colId)
    {
      case COL_SYSINFO2CONFIG_NUMRAPREAMBLES:
      {
        newNumRaPreambles = getUint32FromArray(tlv->data);
        newNumRaPreamblesTlvInfo = tlv;
        if(config->numRaPreambles NOTEQ newNumRaPreambles)
          flagNumRaPreambles = TRUE;
      }
      break;
      case COL_SYSINFO2CONFIG_SIZERAPREAMBLESGROUPA:
      {
        newSizeRaPreambles = getUint32FromArray(tlv->data);
        /* If the below condition fails, this tag information will be sent */
        newSizeRaPreamblesTlvInfo = tlv;
        if(config->sizeRAPreamblesGroupA NOTEQ newSizeRaPreambles)
          flagSizeRaPreambles = TRUE;
      }
      break;
      case COL_SYSINFO2CONFIG_MODIFICATIONPERIODCOEFF:
      {
        newModificationPeriodCoeff        = getUint32FromArray(tlv->data);
        newModificationPeriodCoeffTlvInfo = tlv;
        if(config->modificationPeriodCoeff NOTEQ newModificationPeriodCoeff)
          flagModificationPeriodCoeff = TRUE;
      }
      break;
      case COL_SYSINFO2CONFIG_DEFAULTPAGCYCLE:
      {
        newDefaultPageCycle        = getUint32FromArray(tlv->data);
        newDefaultPageCycleTlvInfo = tlv;
        if(config->defaultPagCycle NOTEQ newDefaultPageCycle)
          flagDefaultPageCycle       = TRUE;
      }
      break;
    }
  }
  /* newSizeRaPreambles and newNumRaPreambles are dependent on each other */
  /* newModificationPeriodCoeff and newDefaultPageCycle are dependent on each other */

  /**************** Check the dependency here - Check 1 *************/
  if ( ( TRUE EQ flagNumRaPreambles ) && (TRUE EQ flagSizeRaPreambles ) )
  {
    if( FALSE EQ doConsistencyCheckOnSysInfo2ConfigNumAndSizeofPreambles( newNumRaPreambles, newSizeRaPreambles ) )
    {
      copyBytes( newSizeRaPreamblesTlvInfo, sizeof(TlvInfo), failedTlvInfo );
      return FALSE;
    }
  }
  if ( ( TRUE EQ flagNumRaPreambles ) && (FALSE EQ flagSizeRaPreambles ) )
  {
    if( FALSE EQ doConsistencyCheckOnSysInfo2ConfigNumAndSizeofPreambles( newNumRaPreambles, config->sizeRAPreamblesGroupA ) )
    {
      copyBytes( newNumRaPreamblesTlvInfo, sizeof(TlvInfo), failedTlvInfo );
      return FALSE;
    }
  }
  if ( ( FALSE EQ flagNumRaPreambles ) && (TRUE EQ flagSizeRaPreambles ) )
  {
    if( FALSE EQ doConsistencyCheckOnSysInfo2ConfigNumAndSizeofPreambles( config->numRaPreambles, newSizeRaPreambles ) )
    {
      copyBytes( newSizeRaPreamblesTlvInfo, sizeof(TlvInfo), failedTlvInfo );
      return FALSE;
    }
  }

  /**************** Check the dependency here - Check 2 *************/
  if ( (flagModificationPeriodCoeff EQ TRUE ) && (flagDefaultPageCycle EQ TRUE ) )
  {
    if( FALSE EQ doConsistencyCheckOnSysInfo2ConfigModPeriodCoeffAndDefaultPagCycle( newModificationPeriodCoeff, newDefaultPageCycle ) )
    {
      copyBytes( newModificationPeriodCoeffTlvInfo, sizeof(TlvInfo), failedTlvInfo );
      return FALSE;
    }
  }
  else if ( ( flagModificationPeriodCoeff EQ TRUE ) && (flagDefaultPageCycle EQ FALSE) )
  {
    if( FALSE EQ doConsistencyCheckOnSysInfo2ConfigModPeriodCoeffAndDefaultPagCycle( newModificationPeriodCoeff, config->defaultPagCycle ) )
    {
      copyBytes( newModificationPeriodCoeffTlvInfo, sizeof(TlvInfo), failedTlvInfo );
      return FALSE;
    }
  }
  else if ( ( flagModificationPeriodCoeff EQ FALSE ) && (flagDefaultPageCycle EQ TRUE) )
  {
    if( FALSE EQ doConsistencyCheckOnSysInfo2ConfigModPeriodCoeffAndDefaultPagCycle( config->modificationPeriodCoeff, newDefaultPageCycle ))
    {
      copyBytes( newDefaultPageCycleTlvInfo, sizeof(TlvInfo), failedTlvInfo );
      return FALSE;
    }
  }
  return TRUE;
}

/**
 *  @details Here in this function, consistency checks on all the required fields will
 *           be carried out.
 * */
TBOOL doConsistencyCheckOnCellConfigTable(
  IN CellConfig  *cellConfig
  )
{
  if ( FALSE EQ doConsistencyCheckOnCellConfigStartRntiAndMaxConnectedUes( cellConfig->startRnti, cellConfig->maxConnectedUes ) )
  {
    BS_ERROR_UNSUPPORTED_PARAMETER( "COL_CELLCONFIG_STARTRNTI ",      cellConfig->startRnti );
    BS_ERROR_UNSUPPORTED_PARAMETER( "COL_CELLCONFIG_MAXCONNECTEDUES", cellConfig->maxConnectedUes );
    return FALSE;
  }

  if ( FALSE EQ doConsistencyCheckOnCellConfigUlEarfcn( cellConfig->ulEarfcn ))
  {
    BS_ERROR_UNSUPPORTED_PARAMETER( "COL_CELLCONFIG_ULEARFCN", cellConfig->ulEarfcn );
    return FALSE;
  }

  if ( FALSE EQ doConsistencyCheckOnSiReTxCnt( cellConfig->siReTxCnt ))
  {
    BS_ERROR_UNSUPPORTED_PARAMETER( "COL_CELLCONFIG_SIRETXCNT", cellConfig->siReTxCnt );
    return FALSE;
  }
  return TRUE;
}

/**
 * @details Here in this function, consistency checks on all the required fields will
 *          be carried out.
 **/
TBOOL doConsistencyCheckOnPhyConfigTable(
  IN PhyConfig  *phyConfig
  )
{
  UINT16    rbCount;
  /* IF Uplink Bandwidth is not Defined Use DlBandwith for UlBandwidth */
  if( FALSE EQ getUlRbsValue() )
  {
    rbCount = getDlRbsValue();
  }
  else
  {
    rbCount = getUlRbsValue();
  }

  if ( FALSE EQ doConsistencyCheckOnPhyConfigPrachFreqOffset( phyConfig->prachFreqOffset, rbCount ) )
  {
    BS_ERROR_UNSUPPORTED_PARAMETER( "COL_PHYCONFIG_PRACHFREQOFFSET", phyConfig->prachFreqOffset );
    return FALSE;
  }
  return TRUE;
}

/**
 * @details Here in this function, consistency checks on all the required fields will
 *          be carried out.
 * */
TBOOL doConsistencyCheckOnSysInfo2ConfigTable(
  IN SysInfo2Config  *sys2Config
  )
{
  if ( FALSE EQ doConsistencyCheckOnSysInfo2ConfigNumAndSizeofPreambles(
                                      sys2Config->numRaPreambles, sys2Config->sizeRAPreamblesGroupA))
  {
    BS_ERROR_UNSUPPORTED_PARAMETERS( "COL_SYSINFO2CONFIG_NUMRAPREAMBLES & COL_SYSINFO2CONFIG_SIZERAPREAMBLESGROUPA", \
                                      sys2Config->numRaPreambles,sys2Config->sizeRAPreamblesGroupA );
    return FALSE;
  }

  if ( FALSE EQ doConsistencyCheckOnSysInfo2ConfigModPeriodCoeffAndDefaultPagCycle(
                                      sys2Config->modificationPeriodCoeff, sys2Config->defaultPagCycle))
  {
    BS_ERROR_UNSUPPORTED_PARAMETERS( "COL_SYSINFO2CONFIG_MODIFICATIONPERIODCOEFF & COL_SYSINFO2CONFIG_DEFAULTPAGCYCLE", \
                                      sys2Config->modificationPeriodCoeff, sys2Config->defaultPagCycle );
    return FALSE;
  }
  return TRUE;
}

/**
 * @param  eutraNeighCellConfig  eutraNeighCellConfig structure.
 * @details Here in this function, consistency checks on all the required fields will
 *          be carried out.
 * */
TBOOL doConsistencyCheckOnEutraNeighCellConfig(
  IN EutraNeighCellConfig  *eutraNeighCellConfig
  )
{
  EnodebConfig  *enodebConfig = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);
  CellConfig    *cellConfig =   (CellConfig*)getFirstRowOfTable(CELLCONFIG);

  if(enodebConfig->enodebId EQ eutraNeighCellConfig->neighEutraEnbId)
  {
    BS_ERROR_NEIGH_ENBID_SAME_AS_ENBID(enodebConfig->enodebId);
    return FALSE;
  }
  if(cellConfig->cellId EQ eutraNeighCellConfig->neighEutraCellId)
  {
    BS_ERROR_NEIGH_CELLID_SAME_AS_CELLID(cellConfig->cellId);
    return FALSE;
  }
  return TRUE;
}

/**
 * @details This function does sanity checks on configuration tables.
 */
TBOOL doConsistencyCheck()
{
  CellConfig  *cellConfig  =  (CellConfig*)getFirstRowOfTable(CELLCONFIG);
  if ( FALSE EQ doConsistencyCheckOnCellConfigTable( cellConfig ) )
  {
    return FALSE;
  }
  PhyConfig     *phyConfig = (PhyConfig*)getFirstRowOfTable(PHYCONFIG);
  if ( FALSE EQ doConsistencyCheckOnPhyConfigTable( phyConfig ) )
  {
    return FALSE;
  }
  SysInfo2Config  *sys2Config = (SysInfo2Config*)getFirstRowOfTable(SYSINFO2CONFIG);
  if ( FALSE EQ doConsistencyCheckOnSysInfo2ConfigTable( sys2Config ))
  {
    return FALSE;
  }
  EutraNeighCellConfig  *eutraNeighCellConfig = (EutraNeighCellConfig*)getFirstRowOfTable(EUTRANEIGHCELLCONFIG);
  if (FALSE EQ doConsistencyCheckOnEutraNeighCellConfig(eutraNeighCellConfig))
  {
    return FALSE;
  }
  return TRUE;
}
