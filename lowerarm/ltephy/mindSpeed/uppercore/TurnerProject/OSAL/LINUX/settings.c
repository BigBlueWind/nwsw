/** @file settings.c
 *
 * @brief Read settings under Linux OS
 * @author Mindspeed Technologies
 * @version $Revision: 1.5 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_LINUX

#include "osal.h"
#include "settings.h"
#include "diags.h"
#include <limits.h>

//
// Limits
//
#define MAXKEYS                     64          // TODO: Use dynamic
#define MAXKEYSIZE                  2048

//
// Internal types
//
typedef enum VarType
{
    VAR_UNKNOWN, VAR_INT, VAR_STR
} VARTYPE;

//
// Internal structures
//
typedef struct KeyTable
{
    char *name;
    char *value;
} KEYTABLE;

//
// Module variables
//
KEYTABLE Keys[MAXKEYS];
char KeyBuffer[MAXKEYSIZE] = "";

/**
 *
 * @param argc
 * @param argv
 * @return
 */
RESULTCODE InitSettings(int argc, char *argv[])
{
    int i, len;
    char buffer[256];
    char *name, *value;
    FILE *file;

    for (i = 0; i < MAXKEYS; i++)
    {
        Keys[i].name = NULL;
        Keys[i].value = NULL;
    }

    // 1st - read command line settings
    i = 1;
    len = 0;
    while (i < argc)
    {
        len += strlen(argv[i]) + 1;
        if (len >= MAXKEYSIZE)
        {
            DiagPrint(DIAG_LEVEL1, "INIT", "Command line too long\n");
            break;
        }
        strcat(KeyBuffer, argv[i]);
        strcat(KeyBuffer, " ");
        i++;
    }

    // 2nd - read ini file settings
    file = fopen("c4k.ini", "r");
    if (file)
    {
        while (fgets(buffer, sizeof(buffer)-1, file) != NULL)
        {
            len += strlen(buffer) + 3;
            if (len >= MAXKEYSIZE)
            {
                DiagPrint(DIAG_LEVEL1, "INIT", "Too many settings defined\n");
                break;
            }
            strcat(KeyBuffer, "-");
            strcat(KeyBuffer, buffer);
        }
        fclose(file);
    }

    // Setting must have form "--var=val"
    i = 0;
    value = KeyBuffer;
    do
    {
        name = strtok(value, "-= \n");
        if (name)
        {
            Keys[i].name = name;
            value = strtok(NULL, "-= \n");
            if (value)
            {
                Keys[i].value = value;
            }
            else
            {
                DiagPrint(DIAG_LEVEL1, "INIT", "Invalid argument in command line [%s]\n",
                        Keys[i].name);
                Keys[i].name = NULL;
                i--;
            }
            value = NULL;
        }
        i++;
    } while ((name != NULL) && (i < MAXKEYS));

    return SUCCESS;
}

/** Read numeric value specified in command line or configuration file
 *
 * @param name Name of the setting
 * @param value Pointer to a variable which will receive the value
 * @param deflt Default value to store in case setting was not specified
 * @return Standard result code
 */
RESULTCODE ReadSettingInt(const char *name, int *value, int deflt)
{
    int i;

    for (i = 0; i < MAXKEYS; i++)
    {
        if (Keys[i].name)
        {
            if (strcasecmp(name, Keys[i].name) == 0)
            {
                *value = strtol(Keys[i].value, NULL, 0);
                if ((*value == 0 && errno == EINVAL) || (*value == LONG_MAX && errno == ERANGE))
                {
                    *value = deflt;
                    return FAILURE;
                }
                break;
            }
        }
        else
        {
            // End of list reached
            *value = deflt;
            return FAILURE;
        }
    }
    return SUCCESS;
}

/**
 *
 * @param name
 * @param value
 * @param deflt
 * @return
 */
RESULTCODE ReadSettingStr(const char *name, char const **value, char const *deflt)
{
    int i;

    for (i = 0; i < MAXKEYS; i++)
    {
        if (Keys[i].name)
        {
            if (strcasecmp(name, Keys[i].name) == 0)
            {
                *value = Keys[i].value;
                break;
            }
        }
        else
        {
            // End of list reached
            *value = deflt;
            return FAILURE;
        }
    }
    return SUCCESS;
}

#endif /* USE_LINUX */
