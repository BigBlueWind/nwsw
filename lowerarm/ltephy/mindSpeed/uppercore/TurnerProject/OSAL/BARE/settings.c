/** @file settings.c
 *
 * @brief Read settings under bare metal
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_BARE

#include "osal.h"
#include "settings.h"

RESULTCODE InitSettings(int argc, char *argv[])
{
    return SUCCESS;
}

RESULTCODE ReadSettingInt(const char *name, int *value, int deflt)
{
    *value = deflt;
    return FAILURE;
}

RESULTCODE ReadSettingStr(const char *name, char const **value, char const *deflt)
{
    *value = deflt;
    return FAILURE;
}

#endif /* USE_BARE */
