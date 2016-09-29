/** @file version.h
 *
 * @brief Defines software bundle version(s)
 * @author Mindspeed Technologies
 * @version $Revision: 1.5 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef VERSION_H_
#define VERSION_H_

//
// Name of the release branch (tag)
//
#define VERSION                 v1_5_1

//
// Numeric version(s)
//
#define U_VERSION_MAJOR         1
#define U_VERSION_MINOR         5
#define U_VERSION_BUILD         1

//
// Helper macros to create version string
// 
// The version string needs to be compliant to the Unix ident program.
// A version string that can be queried by the Unix ident program
// has to be of the format
// $<Tag>: <String> $
// Note the spaces after the : and before the trailing $
// The following macros guarantees that the final version string
// stored in software bundle will always be in the right
// format. To generate a new version string, only the value of
// #define VERSION needs to be changed
//
#define CAT3(x,y,z)             x##y##z
#define STR(s)                  #s
#define TOSTR(v)                STR(v)
#define MAKE_VERS(v)            TOSTR(CAT3($Version:\40,v,\40$))

#endif /* VERSION_H_ */
