

#ifndef FILL_TLV_FUNC_H
#define FILL_TLV_FUNC_H

#include "all_cmn.h"

/* TLV Fill Functions */

GetCmdType getCmdType(
  IO TagInfo     *tag
  );
TlvInfo* findTlvInArray(
  IN TagInfo   *tag,
  IN TlvArray  *tlvArray
  );
void fillTagLenString(
  IN TableId         tableId,
  IN RowId           rowId,
  IN ColId           colId,
  IN UINT8          *value,
  IN Length          len,
  IO TlvArray       *tlvRspArray
  );
void fillTagLenUint32(
  IN TableId         tableId,
  IN RowId           rowId,
  IN ColId           colId,
  IN UINT32          value,
  IO TlvArray       *tlvRspArray
  );
void fillTagLenUint16(
  IN TableId         tableId,
  IN RowId           rowId,
  IN ColId           colId,
  IN UINT16          value,
  IO TlvArray       *tlvRspArray
  );
void fillTagLenUint8(
  IN TableId         tableId,
  IN RowId           rowId,
  IN ColId           colId,
  IN UINT8           value,
  IO TlvArray       *tlvRspArray
  );
void fillNotiTagLenString(
  IN ColId           colId,
  IN UINT8          *value,
  IN Length          len,
  IO NotiObj        *notiObj
  );
void fillNotiTagLenUint32(
  IN ColId           colId,
  IN UINT32          value,
  IO NotiObj        *notiObj
  );
void fillNotiTagLenUint16(
  IN ColId           colId,
  IN UINT16          value,
  IO NotiObj        *notiObj
  );
void fillNotiTagLenUint8(
  IN ColId           colId,
  IN UINT16          value,
  IO NotiObj        *notiObj
  );
#endif
