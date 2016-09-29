/**
 * @file    s1ap_err.h
 * @details Copyright Powerwave Technologies, Inc., 2011
 * @details
 * This file contains S1AP errors mapping with BS errors.
 *
 * @author Shweta Polepally
 *
 */

#include "s1ap_cm.h"


void s1apFillErrorValueByRadioNwCauseVal(
  IN EnumSztCauseRadioNw   causeVal,
  IN S1apMgrCb            *s1Cb,
  IO BsErrorValue         *error
  );

void s1apFillErrorValueByTransPortCauseVal(
  IN EnumSztCauseTport   causeVal,
  IN S1apMgrCb           *s1Cb,
  IO BsErrorValue        *error
  );

void s1apFillErrorValueByNasCauseVal(
  IN EnumSztCauseNas     causeVal,
  IN S1apMgrCb           *s1Cb,
  IO BsErrorValue        *error
  );

void s1apFillErrorValueByProtocolCauseVal(
  IN EnumSztCauseProt    causeVal,
  IN S1apMgrCb           *s1Cb,
  IO BsErrorValue        *error
  );

void s1apFillErrorValueByMiscCauseVal(
  IN EnumSztCauseMisc   causeVal,
  IN S1apMgrCb          *s1Cb,
  IO BsErrorValue       *error,
  IO UINT8              *errString
  );
