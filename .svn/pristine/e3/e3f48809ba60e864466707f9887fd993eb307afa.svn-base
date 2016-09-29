//------------------------------------------------------------------------------
/** @file DOX_INTERNAL rootinfo.h
 *
 * @brief  ROOT information: Used to keep table with detailed information about image
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//------------------------------------------------------------------------------

#ifndef _ROOT_INFO_H_
#define _ROOT_INFO_H_

#ifndef WIN32
#include "config.h"
#include "4gmx.h"
#endif

#define ROOTINFO_TAB_SIZE			7000

#define OBJ_TYPE_G_PROC     1   // Global procedure 
#define OBJ_TYPE_S_PROC     2   // Static procedure 
#define OBJ_TYPE_DATA       3   // for future usage (global variables: dump of variables)
#define OBJ_TYPE_FAKE       4

#define OBJ_NAME_SIZE       60

typedef struct _ROOT_INFO_
{
	char    ObjName [OBJ_NAME_SIZE];
	UINT32  ObjAddr;
    UINT32  ObjSize;
    UINT32  ObjType;    
}RootInfo, *PRootInfo;

extern const RootInfo rootinfo[];

#endif // _ROOT_INFO_H_
