//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL version.h
 *
 * @brief It defines version of SYSCORE/PHY libraries
 * @author Mindspeed Technologies
 * @version $Revision: 1.12.76.11 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif


#ifndef _VERSION_H_
#define _VERSION_H_

// Helper macros to create version string
#define cat3(x,y,z)			x##y##z
#define cat5(x,y,z,u,t)			x##y##z##u##t
#define STR(s)				#s
#define TOSTR(v)				STR(v)
#define MAKE_VERS(v)		TOSTR(cat3($Version:\40,v,\40$))
#define MAKE_BUILDOPT(x,y)	TOSTR(cat5($FW_BuildOption:\40,x,\40,y,\40$))
// The version string needs to be compliant to the Unix ident
// program.
// A version string that can be queried by the Unix ident program
// has to be of the format
// $<Tag>: <String> $
// Note the spaces after the : and before the trailing $
// The following macros guarantees that the final version string
// stored in HDVoiceDvtIdentString will always be in the right
// format. To generate a new version string, only the value of
// #define VERSION needs to be changed
//

// The name of this version; change this when needed
// ******************************************
#define VERSION Branch_v1_8_0_2_LowLatency_01_14_13
// ******************************************

const char * SysGetVersion (void);
void PrintSysVersion (void);

#endif // _VERSION_H_

#ifdef __cplusplus
}
#endif

