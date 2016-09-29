/**
 * @file    s1ap_cmn_proc.c
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @details This file contains the common routines used by S1M, CEM and UEH interface
 *
 * @author Lakshmi Narayana Madala
 *
 */

#include "ccpu_cmn.h"

/* RRM related includes */
#include "lueh.h"
#include "ueh.h"
#include "lueh.x"
#include "ueh_eut_x.h"
#include "s1ap.h"
#include "s1ap_cm.h"
#include "s1ap_cmn_proc.h"
#include "bs_err.h"
#include "ueh_s1ap_cm.h"
//extern PUBLIC    SmCfgCb smCfgCb;

/**
 * @param   tac         Tracking Area code
 * @param   plmnId      plmnId
 * @details
 * This functions returns the plmnId from the Tracking Area code.
 */
void getPlmnIdsFromTac(
    IN SupportedTac *tac,
    IO S8        *plmnId
    )
{
  UINT16 indx;
  S8 printLoc;

  for( indx=0, printLoc=0; indx< tac->servedBroadcastPlmns.numPlmns; indx++ )
  {
    /* warning: conversion from unsigned to signed conversion ignored */
    sprintf( (plmnId + printLoc ), "%c%c%c ", tac->servedBroadcastPlmns.plmnId[indx][0],
                                              tac->servedBroadcastPlmns.plmnId[indx][1],
                                              tac->servedBroadcastPlmns.plmnId[indx][2]
                                              );
    printLoc+=4;
  }

}

/**
 * @param   timeToWait  Time to Wait
 * @details
 * This functions returns the a integer in seconds corresponding to the enum value.
 */
U32 getS1SetupTmrValFromEnum(
    IN TimeToWait timeToWait
    )
{
  switch( timeToWait)
  {
    case TimeToWait_OneSec:
      return 1;

    case TimeToWait_TwoSec:
      return 2;

    case TimeToWait_FiveSec:
      return 5;

    case TimeToWait_TenSec:
      return 10;

    case TimeToWait_TwentySec:
      return 20;

    case TimeToWait_SixtySec:
      return 60;
  }

  return 0;
}


/**
 * @param   octStr      ASCII String containing the numbers
 * @param   strLen      Size of ASCII string
 * @param   bcdStr      BCD string is returned to user
 * @attention observe that the BCD format has the two bytes swapped
 * 9891 in BCD format will be 8919
 * @details
 * This functions converts the given octet string into a BCD string.
 * Convert the ASCII values to Decimal Value.
 * Swap the bytes and place two values in a single byte.
 */
void s1apConOctStringToBcdString(
    IN UINT8 *octStr,
    IN UINT16 strLen,
    IO UINT8 *bcdStr
    )
{
  UINT16 i, bcdIndx;
  UINT8  bcdLeft, bcdRight;

  for( i=0, bcdIndx=0; i< strLen;  )
  {
    /* Get the first byte, convert to Decimal by subtracting ASCII value, increment index */
    bcdLeft= *(octStr + i);
    bcdLeft = bcdLeft - 48;
    i++;

    /* Get the second byte only when index is less than length
     * get byte, convert to Decimal by subtracting ASCII value, increment index */
    if( i >= strLen )
    {
        *(bcdStr + bcdIndx ) = ( (bcdLeft << 4) |0x00 );
        return;
    }

    bcdRight= *(octStr + i );
    bcdRight = bcdRight - 48;
    i++;

    /* Combine the left half and right half into a BCD string */
    *(bcdStr + bcdIndx ) = ( ( bcdRight << 4) | bcdLeft );

    bcdIndx++;
  }
}

/**
 * @param   plmnId      PLMN ID is returned to user
 * @param   octMcc      MCC in ASCII string
 * @param   octMnc      MNC in ASCII sting
 * @details
 * This functions joins the MCC and MNC, and creates the PLMNID,
 * If the MNC is only two characters then a filler is added
 * so MCC:123 MNC:45 becomes PLMNID:21 f3 54
 * MCC: 123 MNC: 456 becomes PLMNID:21 43 65
 *  ---------------------------------
 *  |  MCC 2           |     MCC1   |
 *  ---------------------------------
 *  |  MNC 1/Filler    |     MCC3   |
 *  ---------------------------------
 *  |  MNC 3           |     MNC2   |
 *  --------------------------------- 
 * @attention PLMNID is in BCD format.
 * @attention MCC1 Means Most Significant Digit of MCC and so on, same is applicable to MNC. 
 * By this we can understand that if the MNC is 2digit value, then filler digit will be added at the Most Significant Digit.
 */
void s1apGetPlmnIdFromMccMnc(
    IO UINT8  *plmnId,
    IN UINT8  *octMcc,
    IN UINT8  *octMnc
    )
{
  UINT16 strLen;
  UINT8  mcc[2], mnc[2];

  mcc[0]=0xFF;
  mcc[1]=0xFF;

  mnc[0]=0xFF;
  mnc[1]=0xFF;

  /* Take the firts byte as is,
   * Get the first 4 bits of MNC, or with the last 4 bits of MCC
   */
  s1apConOctStringToBcdString( octMcc, MAX_SIZE_MCCMNC, mcc );
  /* warning: conversion from unsigned to signed conversion ignored */
  strLen = strlen((const S8*)octMnc);
  if( strLen > MAX_SIZE_MCCMNC )
        strLen = MAX_SIZE_MCCMNC;
  s1apConOctStringToBcdString( octMnc, strLen, mnc );
  *( plmnId + 0) = mcc[0];
  if( strLen == 2 )
  {
    /* Fill the filler and fill the MNC digit 3 */
    *( plmnId + 1) = ( 0xF0 | ( mcc[1] >> 4));
    *( plmnId + 2) = mnc[0];
  }
  else
  {
    /**( plmnId + 1) = ( mcc[1]>>4 | (mnc[1] ) );
    *( plmnId + 2) = ( (mnc[0] << 4) | (mnc[0] >> 4) ); */
    /* BS-1086 Changes */
    *( plmnId + 1) = ( mcc[1]>> 4 | (mnc[0] << 4 ) );
    *( plmnId + 2) = (mnc[1] | mnc[0] >> 4);
  }

  logInfo("plmnid: %x %x %x",*(plmnId + 0),*(plmnId + 1),*(plmnId + 2));
}

/**
 * @param   tac         TAC
 * @param   val         A 16 bit integer
 * @details
 * This functions converts a 16 bit human readable number to TAC
 * @attention TAC has the bytes swapped, so val=12 in tac is 21
 */
void s1apGetTacFromUint16(
    IO UINT8  *tac,
    IN UINT16  val
    )
{
  *( tac + 0 ) = ( val >> 8 ) & 0xFF;
  *( tac + 1) = ( val ) & 0xFF;
}

/**
 * @param   enbType     Type of eNodeB whether a Macro or Micro
 * @param   enbId       eNodeB Value as seen on S1 Interface
 * @param   enbIdVal    eNodeB ID filled by OAM
 * @details
 * This functions packs the data according to the type of eNodeB, If it is a Macro eNodeB
 * The data is in first three bytes, if home eNodeB then it is 28 bits inn size and spans 4 bytes
 */
void s1apFillEnbIdFromUint32(
    IN UINT8       enbType,
    IO UINT8       *enbId,
    IN UINT32      enbIdVal
    )
{
  memset( enbId, 0, sizeof(UINT32) );

  if( EnbType_MacroEnodeB == enbType )
  {
    *( enbId + 0 ) = ( ( enbIdVal >> 12) & 0xFF );
    *( enbId + 1 ) = ( ( enbIdVal >> 4) & 0xFF );
    *( enbId + 2 ) = ( enbIdVal << 4  );
  }
  else
  {
    *( enbId + 0 ) = ( enbIdVal >> 24 );
    *( enbId + 1 ) = ( ( enbIdVal >> 16 ) & 0xFF);
    *( enbId + 2 ) = ( ( enbIdVal >> 8 ) & 0xFF);
    *( enbId + 3 ) = ( ( enbIdVal) & 0xFF);
  }

}

/**
 * @param   tai         Tracking Area Identity
 * @details
 * This functions creates the TAI from the OAM Entered values
 */
void s1apGetTaiFromCfg(
    IO Tai            *tai
    )
{
  MsgBsmS1apConfigReq     *s1apCfgInfo = &(smCb.cfgCb.s1apCfgInfo);//smCfgCb.s1apCfgInfo);

  s1apGetPlmnIdFromMccMnc( tai->plmnIdentity,
                            s1apCfgInfo->plmnMCC,
                            s1apCfgInfo->plmnMNC);
  s1apGetTacFromUint16( tai->tac,
                         s1apCfgInfo->tac );

}

/**
 * @param   cmpCellId   Complete Cell ID
 * @param   enbId       eNodeB Identifier
 * @param   cellId      Cell Identifier
 * @details
 * Complete Cell ID is a combination of 20 bits of Macro eNodeB Id and 8 bits of Cell Id
 * This function combines the enbId and cellId and returns cmpCellId
 */
void s1apGetCellIdFromEnbIdAncActCellId(
    IO UINT8  *cmpCellId,
    IN UINT32 enbId,
    IN UINT8  cellId
    )
{
  memset( cmpCellId, 0, sizeof(UINT32) );

  *( cmpCellId + 0 ) = ( ( enbId >> 12) & 0xFF );
  *( cmpCellId + 1 ) = ( ( enbId >> 4) & 0xFF );
  *( cmpCellId + 2 ) = ( enbId << 4  |  (cellId >> 4) );
  *( cmpCellId + 3 ) = ( cellId << 4 );
}

/**
 * @param   eUtranCgi   EUTRAN Cell Global Identity
 * @details
 * This functions creates the E-UTRAN CGI from the OAM Entered values
 */
void s1apGetEUtranCgiFromCfg(
    IO EUtranCgi      *eUtranCgi
    )
{
  MsgBsmS1apConfigReq     *s1apCfgInfo = &(smCb.cfgCb.s1apCfgInfo);//smCfgCb.s1apCfgInfo);

  s1apGetPlmnIdFromMccMnc( eUtranCgi->plmnIdentity,
                            s1apCfgInfo->plmnMCC,
                            s1apCfgInfo->plmnMNC
                            );
  s1apGetCellIdFromEnbIdAncActCellId( eUtranCgi->cellIdentity,
                            s1apCfgInfo->enodebId,
                            uehCb.uehCfgCb.cellCfg.cellId
                            );

}

/**
 * @param   connItem    UE Associated Logical Connection Item
 * @details
 * This functions searches for UE in the S1AP connection list using the enBUeS1apId from connection Item,
 * If the Item is found then the S1Cb is returned, if the item is not found NULL is returned
 */
UehS1ConCb *s1apGetS1apCbFrmEnbId( 
    IN UehCellCb   *cellCb,
    IN S1UeAssocLogConnItem *connItem
    )
{  
  UehS1ConCb   *s1ConCb = NULLP;
  
  U32          indx = 0;
  //U16          maxS1ConnAllowed = uehGetMaxS1apConnAllowed();

  logInfo("%s","s1apGetS1apCbFrmEnbId");

  for (indx = 0; indx < cellCb->maxRntiPool; indx++)
  {
    UEH_GET_S1CONCB(s1ConCb,indx);
    if( NULL != s1ConCb )
    {
      if( ((TRUE ==  connItem->isEnbUeS1apId) && (s1ConCb->enb_ue_s1ap_id ==  connItem->enbUeS1apId))
        || ((TRUE ==  connItem->isMmeUeS1apId) && (s1ConCb->mme_ue_s1ap_id ==  connItem->mmeUeS1apId)))
      {
        return s1ConCb;
      }
    }
  }
  return NULLP;
}
