/**
 * @file    s1ap_cmn_proc.c
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @details This file contains the prototypes of common routines used by S1M, CEM and UEH interface
 *
 * @author Lakshmi Narayana Madala
 *
 */
#include "s1ap_cm.h"
#include "ueh.h"

#ifndef S1AP_CMN_PROC_H
#define S1AP_CMN_PROC_H


void getPlmnIdsFromTac(
    IN SupportedTac *tac,
    IO S8        *plmnId
    );
U32 getS1SetupTmrValFromEnum(
    IN TimeToWait timeToWait
    );
void s1apConOctStringToBcdString(
    IN UINT8 *octStr,
    IN UINT16 strLen,
    IO UINT8 *bcdStr
    );
void s1apGetPlmnIdFromMccMnc(
    IO UINT8  *plmnId,
    IN UINT8  *octMcc,
    IN UINT8  *octMnc
    );
void s1apGetTacFromUint16(
    IO UINT8  *tac,
    IN UINT16  val
    );
void s1apFillEnbIdFromUint32(
    IN UINT8       enbType,
    IO UINT8       *enbId,
    IN UINT32      enbIdVal
    );
void s1apGetTaiFromCfg(
    IO Tai            *tai
    );
void s1apGetEUtranCgiFromCfg(
    IO EUtranCgi      *eUtranCgi
    );
void s1apGetCellIdFromEnbIdAncActCellId(
    IO UINT8  *cmpCellId,
    IN UINT32 enbId,
    IN UINT8  cellId
    );
UehS1ConCb *s1apGetS1apCbFrmEnbId( 
    IN UehCellCb   *cellCb,
    IN S1UeAssocLogConnItem *connItem
    );
#endif /* S1AP_CMN_PROC_H */
