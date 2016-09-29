/**
 * @file    
 * @section Copyright Powerwave Technologies, Inc., 2010
 * @section This is auto generated file from Excel
 * @section This file contains functions to print ENUM Information into strings
 *
 * Author: Girish Vakati
 * Date: 17-03-2011
 * Description  : Draft version 
 * Generated on : 2/15/2013 2:49:01 PM
 */

#include  "enumstr_oamtables.h"
#include  "enumstr.h"

/*****************************************************************************/
/**Generate the Print  functions For Enums **/
/*****************************************************************************/

/**
 * @section Converts the Enum values received from BSM into strings
 * @param  enumNum  is a number coming from BSM
 */

UINT8* oamIfTypeStr(UINT32 enumNum)
{
  if( enumNum EQ 1)
  {
    return "OAMINTERFACETYPE_CLI";
  }
  if( enumNum EQ 2)
  {
    return "OAMINTERFACETYPE_WEB";
  }
  if( enumNum EQ 3)
  {
    return "OAMINTERFACETYPE_EMS";
  }
 return "UNDEFINED_ENUM";
}
/*****************************************************************************/
/**Fill Table with Function pointers , table Ids and column Ids  **/
/*****************************************************************************/
IN NotiEnumStrInfo c_NotiMsgEnumStrInfoTab[] =
{ /* Action Handler            tableNum colNum  */
  { alarmTypeStr,                      1001, 4   },
  { alarmSeverityStr,                  1001, 6   },
  { alarmCauseStr,                     1001, 7   },
  { oamIfTypeStr,                      1011, 4   },
  { oamIfTypeStr,                      1012, 4   },
  { oamIfTypeStr,                      1013, 4   },
  { swTypeStr,                         1014, 4   },
  { swTypeStr,                         1015, 4   },
  { operationalStateStr,               1019, 4   },
  { adminStateStr,                     1020, 4   },
  { profileIdStr,                      1026, 4   },
  { NULL,                              0,    0   }
};
