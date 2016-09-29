/** @file settings.c
 *
 * @brief Read settings
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "resultcodes.h"

RESULTCODE InitSettings(int argc, char *argv[]);

RESULTCODE ReadSettingInt(const char *name, int *value, int deflt);
RESULTCODE ReadSettingStr(const char *name, char const **value, char const *deflt);

#endif /* _SETTINGS_H */
