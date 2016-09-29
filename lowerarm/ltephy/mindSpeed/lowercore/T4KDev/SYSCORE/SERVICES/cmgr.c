//-------------------------------------------------------------------------------------------
/** @file cmgr.c
 *
 * @brief The implementation of console manager
 * @author Mindspeed Technologies
 * @version $Revision: 1.21 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "cmgr.h"
#include <string.h>
#include <stdio.h>
#include "cline.h"
#include "stream.h"

#define isprint(c) (c>0x1F && c<0x7F)
#define isxdigit(c) ( (c>0x2F && c<0x3A) || (c>0x40 && c<0x47) ||  (c>0x60 && c<0x67))
#define isdigit(c) ((c) >= '0' &&(c) <= '9')

static CMgrCtx  * pMgrContext = NULL;

/*******************************************************************************
*           Implementation of local functions of CONSOLE MANAGER
*******************************************************************************/

int _strlen (const char * pSrc)
{
    int nNum = 0;
    
    while (*pSrc != 0)
    {
        pSrc++;
        nNum++;
    }

    return nNum;
}

int _strcmp (const char * pSrc, char * pDst)
{
    _ASSERT_PTR(pSrc);
    _ASSERT_PTR(pDst);

    while (TRUE)
    {
        if (*pSrc != *pDst)
           return (*pSrc > *pDst) ? 1 : -1;

        if (*pSrc == 0 || *pDst == 0)
        {
            if (*pSrc > *pDst)
                return 1;

            if (*pSrc < *pDst)
                return -1;

            break;
        }

	    pSrc++;
        pDst++;
    }

    return 0;
}

int _is_sub_str (const char * pStr, char * pChSet)
{
    _ASSERT_PTR(pStr);
    _ASSERT_PTR(pChSet);

    while (*pChSet != 0)
    {
        if (*pChSet != *pStr)
            return 0;

        pChSet++;
        pStr++;
    }

    return 1;
}

int _str_match_num(const char * pStr, char * pChSet)
{
    int num = 0;
    
    _ASSERT_PTR(pStr);
    _ASSERT_PTR(pChSet);

    while (*pChSet != 0)
    {
        if (*pChSet != *pStr)
            break;

        pChSet++;
        pStr++;
        num++;
    }

    return num;
}

int _strcpy (char * pDst, const char * pSrc)
{
    _ASSERT_PTR(pSrc);
    _ASSERT_PTR(pDst);

    while (*pSrc)
    {
        *pDst++ = *pSrc++;
    }

    return 0;
}

static UINT8 IsPresent (CMgrCtx * pCtx, char * pCmd)
{
    UINT8 i;

    if (pCmd == NULL)
        return CM_ERROR_INDEX;

    for (i = 0; i < pCtx->History.CmdCount; i++)
    {
        if (!_strcmp ((const char *)pCtx->History.CmdList[i], pCmd ))
            return i;
    }

    return CM_ERROR_INDEX;
}


static MXRC AddHistory (CMgrCtx * pCtx, char * pCmd)
{
    UINT8 nIndex;

    if (pCmd == NULL)
        return RC_CM_PARAM_ERROR;

    if (pCmd [0] == 0)
        return RC_CM_PARAM_ERROR;

    /* to check a command in "history"*/

    if ((nIndex = IsPresent (pCtx, pCmd)) != CM_ERROR_INDEX)
    {
        pCtx->History.LastCmd = nIndex;
        return RC_CM_OK;
    }

    if (pCtx->History.CmdCount < CM_MAX_CMD_HISTORY)
    {
        pCtx->History.LastCmd = pCtx->History.CmdCount;
        pCtx->History.CmdCount++;

        strcpy ((char *)pCtx->History.CmdList [pCtx->History.LastCmd], pCmd);
    }
    else if (pCtx->History.LastCmd == CM_ERROR_INDEX)
    {
        strcpy ((char*)pCtx->History.CmdList[0], pCmd);

        pCtx->History.LastCmd = 0;
        pCtx->History.CmdCount= 1;
    }
    else if (pCtx->History.CmdCount >= CM_MAX_CMD_HISTORY)
    {
        pCtx->History.LastCmd ++;

        if (pCtx->History.LastCmd + 1 >= CM_MAX_CMD_HISTORY)
            pCtx->History.LastCmd = 0;

        strcpy ((char *)pCtx->History.CmdList [pCtx->History.LastCmd], pCmd);
    }
    else
    {
        pCtx->History.LastCmd ++;
        pCtx->History.CmdCount++;

        strcpy ((char *)pCtx->History.CmdList [pCtx->History.LastCmd], pCmd);
    }

    return RC_CM_OK;
}

/*static MXRC GetLastHistory (CMgrCtx * pCtx, char * pCmd)
{
    if (pCtx->History.LastCmd == CM_ERROR_INDEX)
    {
        strcpy (pCmd, "");
        return RC_CM_OK;
    }

    strcpy (pCmd, (char *)pCtx->History.CmdList [pCtx->History.LastCmd]);

    return RC_CM_OK;
}*/


static MXRC GetUpHistory (CMgrCtx * pCtx, char * pCmd)
{
    if (pCtx->History.LastCmd == CM_ERROR_INDEX)
    {
        strcpy (pCmd, "");
        return RC_CM_OK;
    }

    strcpy (pCmd, (char *)pCtx->History.CmdList [pCtx->History.LastCmd]);

    if (pCtx->History.LastCmd == 0)
    {
        pCtx->History.LastCmd = pCtx->History.CmdCount - 1;
    }
    else
    {
        pCtx->History.LastCmd --;
    }

    return RC_CM_OK;
}

static MXRC GetDownHistory (CMgrCtx * pCtx, char * pCmd)
{
    if (pCtx->History.LastCmd == CM_ERROR_INDEX)
    {
        strcpy (pCmd, "");
        return RC_CM_OK;
    }

    strcpy (pCmd, (char *)pCtx->History.CmdList [pCtx->History.LastCmd]);

    if (pCtx->History.LastCmd + 1 >= pCtx->History.CmdCount)
    {
        pCtx->History.LastCmd = 0;
    }
    else
    {
        pCtx->History.LastCmd ++;
    }

    return RC_CM_OK;
}

/** @brief The function sends an ASCIIZ string to the device and sends two
           characters '\r' '\n' if  bNewLN is TRUE

    @param pCtx  [in]   -   the console context
    @param pData [in]   -   pointer to the text, it can be NULL
    @param bNewLN[in]   -   a flag of "new string"

    @return [MXRC] return code {RC_CM_XXX}

    *  \ingroup group_lte_services

    */

static MXRC WriteStr (CMgrCtx * pCtx, char * pData, BOOL bNewLN)
{
    if (pData)
    {
        while (*pData)
        {
            pCtx->cbWrite (pCtx->CommDevData, *pData);
            pData ++;
        }
    }

    if (bNewLN)
    {
        pCtx->cbWrite (pCtx->CommDevData, CM_CR);
        pCtx->cbWrite (pCtx->CommDevData, CM_LF);
    }

    pCtx->cbWriteComp(pCtx->CommDevData);

    return RC_CM_OK;
}


/** @brief The function sends a "LOGO" and ASCIIZ string to the device. If
           bNewLN is TRUE, the function sends two characters '\r' '\n' as well.

    @param pCtx  [in]   -   the console context
    @param pData [in]   -   pointer to the text
    @param bNewLN[in]   -   flag of new string

    @return [UINT32] return code {CM_RC_XXX}

    *  \ingroup group_lte_services

    */

static UINT32 WriteLogoStr (CMgrCtx * pCtx, char * pData, BOOL bNewLN)
{
    UINT32 nRes = RC_CM_OK;

    if ((nRes = WriteStr (pCtx, pCtx->szLogo, FALSE)) != RC_CM_OK)
        return nRes;

    if ((nRes = WriteStr (pCtx, CM_COMMAND_BEGINNER, FALSE)) != RC_CM_OK)
        return nRes;

    if ((nRes = WriteStr (pCtx, pData, bNewLN)) != RC_CM_OK)
        return nRes;

    return RC_CM_OK;
}

/** @brief The function erases exntered console command
    @param pCtx [in] - console context
    @return [UINT32] error code {CM_RC_XXX}

    *  \ingroup group_lte_services

    */

static UINT32 EraseCmd (CMgrCtx * pCtx)
{
    while (pCtx->CmdPos)
    {
        pCtx->cbWrite (pCtx->CommDevData, (UINT8)CM_BACKSPACE);
        pCtx->cbWrite (pCtx->CommDevData, (UINT8)' ');
        pCtx->cbWrite (pCtx->CommDevData, (UINT8)CM_BACKSPACE);

        pCtx->CmdPos--;
    }

    pCtx->Cmd [0] = 0;

    pCtx->cbWriteComp(pCtx->CommDevData);

    return RC_CM_OK;
}

static BOOL InList (char * pList, char nChar)
{
    if (pList == NULL)
        return FALSE;

    while ( *pList)
    {
        if (*pList == nChar)
            return TRUE;

        pList ++;
    }

    return FALSE;
}

/** @brief The function changes current console command by history command

    @param pCtx  [in] - the console context
    @param nFlag [in] - "direction" of histore {CM_CC_XXX}

    @return [UINT32] error code {CM_RC_XXX}

*  \ingroup group_lte_services

*/
static MXRC ReplaceCmdByHistory (CMgrCtx * pCtx, UINT32 nFlag)
{
    EraseCmd(pCtx);  // to erase the current user's input

    switch (nFlag)
    {
        case CM_CC_UP:
            GetUpHistory (pCtx, (char *) pCtx->Cmd);
            break;

        case CM_CC_DOWN:
            GetDownHistory (pCtx, (char *) pCtx->Cmd);
            break;

        default:
            return RC_CM_PARAM_ERROR;
    }

    pCtx->CmdPos = strlen ((char *) pCtx->Cmd);

    WriteStr(pCtx, pCtx->Cmd, FALSE);

    return RC_CM_OK;
}


/** @brief The function parses a string and returns the pointer to the next
           lexem determinated by space list and break list of characters

    @param pData   [in]     -   pointer to text
    @param pnPos   [in/out] -   on input : offset  in the text
                                on output: offset to end of text or end of lexem
    @param pBrList [in]     -   pointer to the list of special characters
    @param pSpList [in]     -   pointer to the list of spacebar characters
    @param pnCount [in]     -   pointer to the variable containing a number of byte of lexem

    @return [char *] pointer to a lexem

*  \ingroup group_lte_services

*/
static char * GetNextLex (char * pData, UINT32 * pnPos,
                          char *pBrList, char * pSpList, UINT32 * pnCount)
{
    char   byte   = 0;
    UINT32 nBegin = (UINT32)-1;

    if (pData == NULL || pBrList == NULL || pSpList == NULL ||
        pnPos == NULL || pnCount == NULL)
    {
        return NULL;
    }

    (*pnCount) = 0;

    while ((byte = pData [(*pnPos)]) != 0)
    {
        if (InList(pBrList, byte))
        {
            if ((*pnCount) == 0)
            {
                (*pnCount) = 1;
                nBegin = (*pnPos);
                (*pnPos)++;

                return pData + nBegin;
            }
            else
                return pData + nBegin;
        }
        else if (InList(pSpList, byte))
        {
            if ((*pnCount) != 0)
            {
                (*pnPos) ++;
                return pData + nBegin;
            }
        }
        else
        {
            if (nBegin == (UINT32)-1)
                nBegin = (*pnPos);

            (*pnCount) ++;
        }

        (*pnPos) ++;

    }

    if ((*pnCount) == 0)
        return NULL;

    return pData + nBegin;
}

/**
    @brief The function parses of user command

    @param pCmdData     [in]    -   a pointer to the user command
    @param pInfo        [out]   -   a pointer to the object contans user command

    @return [MXRC] an error code RC_CM_XXX

*  \ingroup group_lte_services

*/
static MXRC ParsCmd (char * pCmdData, CMgrCmdParamCtx * pInfo)
{
    char * pLex = NULL;
    UINT32 nPos = 0;
    UINT32 nSize= 0;
    UINT32 nParCount = 0;

    _ASSERT_PTR(pCmdData);
    _ASSERT_PTR(pInfo);

    /* TO get the name of console command */

    pLex = GetNextLex (pCmdData, &nPos, CM_BREAK_LEX_LIST,
                        CM_SPACE_LEX_LIST, &nSize);

    if (pLex == NULL)
        return RC_CM_CMD_EMPTY;

    pInfo->DstCpuID = 0;

    if (isdigit(pLex[0]))
    {
        pInfo->DstCpuID = pLex[0] - '0';

        pLex = GetNextLex (pCmdData, &nPos, CM_BREAK_LEX_LIST,
                        CM_SPACE_LEX_LIST, &nSize);

        if (pLex == NULL)
            return RC_CM_CMD_EMPTY;

        if (pLex [0] != ':')
            return RC_CM_CMD_FORMAT_ERROR;

        pLex = GetNextLex (pCmdData, &nPos, CM_BREAK_LEX_LIST,
                        CM_SPACE_LEX_LIST, &nSize);
    }

    if (pLex == NULL)
        return RC_CM_CMD_FORMAT_ERROR;

    /*Set command name*/
    pInfo->CmdName      = pLex;
    pLex [nSize]        = 0;
    pInfo->ParamCount   = 0;

    /*Load command parametrs*/
    while ((pLex = GetNextLex(pCmdData, &nPos,
                                CM_BREAK_LEX_LIST,
                                CM_SPACE_LEX_LIST,
                                &nSize)
           ) != NULL  &&
           (nParCount < CM_MAX_PARAM_COUNT))
    {
        pInfo->ParamList [nParCount++] = pLex;
        pLex [nSize] = 0;
    }

    pInfo->ParamCount = nParCount;

    return RC_CM_OK;
}


/**
    @brief This function scans current table and returns an index to the object
            declared in table as handler for this command

    @param pCtx     [in] - the console command
    @param pCmdName [in] - the command name

    @return [UINT32] index

    *  \ingroup group_lte_services

    */

static UINT32 GetHandlerIndex (CMgrCtx * pCtx, char * pCmdName)
{
    UINT32 nIndex = 0;

    if (pCtx->MainMenu == NULL)
        return CM_INVALID_INDEX;

    if (pCmdName)
    {
        while (pCtx->MainMenu [nIndex].Name)
        {
            if (!_strcmp (pCtx->MainMenu [nIndex].Name, pCmdName))
                return nIndex;

            nIndex ++;
        }
    }
    else
    {
        while (TRUE)
        {
            if (pCtx->MainMenu [nIndex].Name == NULL &&
                pCtx->MainMenu [nIndex].ExecMenu != NULL)
                return nIndex;

            nIndex ++;
        }
    }

    return CM_INVALID_INDEX;
}


/** @brief The function returns a pointer to the handler for unknown command
    @return [CMgrMenuElm*]

    *  \ingroup group_lte_services

    */

static PCMgrMenuElm GetErrHandler (CMgrCtx * pCtx)
{
    UINT32 nIndex = GetHandlerIndex (pCtx, NULL);

    if (nIndex == CM_INVALID_INDEX)
        return NULL;

    if (pCtx->MainMenu [nIndex].Type == CM_OBJ_CMD)
        return &pCtx->MainMenu [nIndex];

    return NULL;
}

/**
    @brief The function adds ASCIIZ string to the logo buffer

    @param pCtx      [in] - the console context
    @param pSubLevel [in] - a pointer to the text

    @note  A result of executing this function:
            origin logo :  LowArm>
                add log :  "MyLevel"
                 result :  LowArm\MyLevel>

    @return [UINT32]  error code

*  \ingroup group_lte_services

*/
static MXRC AddLogo (CMgrCtx * pCtx, char * pSubLevel)
{
    UINT32 nLenLogo = 0;
    UINT32 nLenSub  = 0;

    if (pSubLevel == NULL)
        return RC_CM_PARAM_ERROR;

    nLenLogo = strlen (pCtx->szLogo);
    nLenSub  = strlen (pSubLevel);

    if ((nLenLogo + nLenSub + sizeof (char)) >= CM_MAX_LOGO )
        return RC_CM_OUT_BUF;

    strcat (pCtx->szLogo, "\\");
    strcat (pCtx->szLogo, pSubLevel);

    return RC_CM_OK;
}

/**
    @brief The function cuts some text from the logo buffer

    @param pCtx      [in] - the console context
    @param pSubLevel [in] - a pointer to text

    @note  A result of executing this function:
            origin logo :  LowArm\MyLevel>
                del log :  "MyLevel"
                 result :  LowArm>

    @return [UINT32]  error code

*  \ingroup group_lte_services

*/
static UINT32 DelLogo (CMgrCtx * pCtx, char * pSubLevel)
{
    char * pSubStr = NULL;
    char * pEnd    = NULL;
    UINT32    nSize   = 0;

    if (pSubLevel == NULL)
        return RC_CM_PARAM_ERROR;

    nSize = strlen (pSubLevel);

    pSubStr = (char*)strstr (pCtx->szLogo, pSubLevel);

    if (pSubStr == NULL)
        return RC_CM_OK;

    /* remove backslash*/
    if (pSubStr != pCtx->szLogo)
    {
        if (*(pSubStr - 1) == '\\')
        {
            pSubStr --;
            nSize   ++;
        }
    }

    pEnd = pSubStr + nSize;

    while (*pEnd)
    {
        (*pSubStr) = (*pEnd);

        pSubStr ++;
        pEnd    ++;
    }

    *pSubStr= 0;

    return RC_CM_OK;
}


void CMgrProcTab(CMgrCtx * pCtx)
{
    char * pCmd, *pCompCmd = NULL;
    UINT32 i, num = 0;
    UINT32 time = MxGetTicks(), show = 0;

    if (pCtx->CmdPos == 0)
        return;

    if (time - pCtx->TabTime < MS_TO_TICKS(300))
    {
        show = 1;
    }

    pCtx->TabTime = time;

    //uart_printf ("[%x]", time - pCtx->TabTime);

    // to try to find the command by using 
    // the already typed characters

    pCmd = pCtx->Cmd;

    i = 0;
    while (pCtx->MainMenu[i].Name != NULL)
    {
        if (_is_sub_str(pCtx->MainMenu[i].Name, pCmd))
        {
            if (show)
            {
                if (show == 1)
                    WriteStr (pCtx, NULL, 1);

                show = 2;
                WriteStr (pCtx, (char*)pCtx->MainMenu[i].Name, 1);
            }
            else
            {
                if (pCompCmd != NULL)
                {
                    num = MIN(num, _str_match_num(pCompCmd, (char*)pCtx->MainMenu[i].Name));
                    pCompCmd = (char*)pCtx->MainMenu[i].Name;
                }
                else
                {
                    pCompCmd = (char*)pCtx->MainMenu[i].Name;
                    num = _strlen(pCompCmd);
                }
            }

            //uart_printf ("{%s / %s}:%d\r\n", pCtx->MainMenu[i].Name, pCmd, num);
        }

        i++;
    }

    if (show != 0)
    {
        if (show == 2)
            CMgrUpdate(pCtx);
        
        return;
    }


    if (pCompCmd == NULL)
        return;

    // to skipp already provided characters
    pCompCmd += _strlen(pCmd);
    num -= MIN(num, _strlen(pCmd));

    // to add the rest of symbols
    while (*pCompCmd != 0 && num != 0)
    {
        pCtx->Cmd[pCtx->CmdPos++] = *pCompCmd;
        pCtx->cbWrite (pCtx->CommDevData, *pCompCmd);
        pCompCmd++;
        num --;
    }

    pCtx->Cmd[pCtx->CmdPos] = 0;
    pCtx->cbWriteComp(pCtx->CommDevData);
    
}

/**
    @brief The function is places to a command buffer new character from stream
           and make process a command, if command is compleated

    @param pCtx  [in] - the console manager
    @param nChar [in] - a byte of stream

    @return [MXRC] error code RC_CM_XXX

*  \ingroup group_lte_services

*/
static MXRC ProccessChar (CMgrCtx * pCtx, char nChar)
{
    switch (nChar)
    {
        /* Run command */
        case CM_CR:
            {
                CMgrCmdParamCtx cp;
                UINT32 nRes;

                AddHistory(pCtx, pCtx->Cmd);

                if(pCtx->Device != CLINE_DEVICE_CICPU)
                {
                    pCtx->cbWrite(pCtx->CommDevData, CM_CR);
                    pCtx->cbWrite(pCtx->CommDevData, CM_LF);
                    pCtx->cbWriteComp(pCtx->CommDevData);
                }

                /*  Breaking a user command to
                    a list of parameters */

                nRes = ParsCmd (pCtx->Cmd, &cp);

                switch (nRes)
                {
                    /*If command processing is OK*/

                    case RC_CM_OK:
                        {
                            CMgrCmdCtx  ctx;
                            UINT32 nIndex = GetHandlerIndex (pCtx, cp.CmdName);

                            ctx.ConsoleCtx = pCtx;
                            ctx.WriteStr= (TCMgrWriteStr)CMgrWriteStr;
                            ctx.SetMenu = (TCMgrSetMenu)CMgrSetMenu;

                            /* If have not command in the table of context menu*/
                            if (nIndex == CM_INVALID_INDEX)
                            {
                                CMgrMenuElm * pObj = GetErrHandler (pCtx);

                                if (pObj)
                                {
                                    ctx.UserData = pObj->UserData;
                                    ctx.TextDescr= pObj->TextDescr;
                                    ctx.MenuTable= pCtx->MainMenu;

                                    pObj->ExecMenu (&ctx, &cp);
                                    if(pCtx->Device != CLINE_DEVICE_CICPU)
                                    {	
                                        WriteStr(pCtx, NULL, TRUE);
                                    }
                                }
                            }
                            else
                            {
                                CMgrMenuElm * pObj = &pCtx->MainMenu [nIndex];

                                ctx.UserData = pObj->UserData;
                                ctx.TextDescr= pObj->TextDescr;
                                ctx.MenuTable= pCtx->MainMenu;

                                switch (pObj->Type)
                                {
                                    case CM_OBJ_CMD:
                                        {
                                            if (cp.ParamCount != 0)
                                            {
                                                if (!_strcmp ((const char *) cp.ParamList[0], "/?"))
                                                {
                                                    CMgrShowTableUsageInfo(pCtx, cp.CmdName, pCtx->MainMenu);
                                                    break;
                                                }
                                            }

                                            nRes = pObj->ExecMenu (&ctx, &cp);

                                            if (nRes == RC_CM_PARAM_ERROR)
                                            {
                                                WriteStr(pCtx, pObj->UsageInfo, TRUE);
                                            }
                                            else
                                            {
                                                WriteStr(pCtx, NULL, TRUE);
                                            }

                                            break;
                                        }
                                    case CM_OBJ_SUB:
                                        {
                                            pCtx->MainMenu = (void *)pObj->SubMenu;
                                            AddLogo (pCtx, cp.CmdName);

                                            break;
                                        }
                                    case CM_OBJ_DELIMITER:
                                        {
                                            // just do nothing
                                            break;
                                        }
                                    default:
                                        {
                                            WriteStr(pCtx, "Unknown object type", TRUE);
                                            break;
                                        }
                                }
                            }

                            break;
                        }
                    case RC_CM_CMD_EMPTY:
                        {
                            break;
                        }
                    default:
                        {
                            WriteStr(pCtx, "Parsing Error", TRUE);
                            break;
                        }
                }

                pCtx->CmdPos  = 0;
                pCtx->Cmd [0] = 0x00;

                WriteLogoStr(pCtx, NULL, 0);

                break;
            }

        case CM_BACKSPACE:
            {
                /* If it's a begin of string */
                if (pCtx->CmdPos == 0)
                    break;

                pCtx->Cmd [--pCtx->CmdPos] = 0x0;

                /*Show Echo typing*/
                if(pCtx->Device != CLINE_DEVICE_CICPU)
                {
                    pCtx->cbWrite (pCtx->CommDevData, nChar);
                    pCtx->cbWrite (pCtx->CommDevData, CM_SPACE);
                    pCtx->cbWrite (pCtx->CommDevData, nChar);
                    pCtx->cbWriteComp(pCtx->CommDevData);
                }

                break;
            }

        /* list of non-out symbols*/
        case CM_TAB:
        {
            CMgrProcTab(pCtx);
            break;
        }
        
        case CM_LF:
            break;

        default:
        {
            if (pCtx->CmdPos >= CM_MAX_COMMAND_SIZE)
                return RC_CM_OUT_BUF;

                if (isprint(nChar))
                {
                    pCtx->Cmd [pCtx->CmdPos ++] = nChar;
                    pCtx->Cmd [pCtx->CmdPos   ] = 0x0;

                    if(pCtx->Device != CLINE_DEVICE_CICPU)
                    {
                        /*Show Echo typing*/
                        pCtx->cbWrite (pCtx->CommDevData, nChar);
                        pCtx->cbWriteComp(pCtx->CommDevData);
                    }
                }
            break;
        }
    }

    return RC_CM_OK;
}

/** @brief The function is a part of finite state machine of processing input stream.
           The main goal of function to detext the control bytes of two bytes symbol

    @param pCtx  [in]    -   the console context
    @param nByte [in]    -   input byte of stream

    @return [UINT32]  flags {CM_CF_XXX}

    *  \ingroup group_lte_services

    */

static UINT32  ProcStream (CMgrCtx * pCtx, UINT32 nByte)
{
    UINT8  nFirst = nByte;
    UINT16 nWait;

    if (nByte == CM_CC_CTRL_C )
        return CM_CC_CTRL_C;

    if (nByte == CM_CC_CTRL_X )
        return CM_CC_CTRL_X;

    if (nByte == CM_CC_CTRL_Z )
        return CM_CC_CTRL_Z;

    if (nFirst != CM_CC_ACCEPT && nFirst != CM_CC_ACCEPT_EX && nFirst != CM_CC_ESC)
        return CM_CC_NONE;

    nWait = 0;

    while (nWait < 5000)
    {
        nFirst = pCtx->cbRead(pCtx->CommDevData);

        if (nFirst != 0)
            break;

        nWait ++;
    }

    if (nFirst == 0)
        return CM_CC_ESC;

    if (nFirst != CM_CC_ACCEPT && nFirst != CM_CC_ACCEPT_EX)
        return nFirst;

    nWait = 0;

    while (nWait < 5000)
    {
        nFirst = pCtx->cbRead(pCtx->CommDevData);

        if (nFirst != 0)
            break;

        nWait ++;
    }

    return nFirst;
}

/** @brief The function is processing pressing of user's hotkey

    @param pCtx   [in] - the console context
    @param nHotKey[in] - code of control key {CM_CC_XXX}

    @return [MXRC] an error code

    *  \ingroup group_lte_services

    */

static MXRC ProcHotKey (CMgrCtx * pCtx, UINT32 nHotKey)
{
    UINT16 i = 0;

    _ASSERT_PTR(pCtx);

    if (pCtx->Hotkey == NULL)
        return RC_CM_OK;


    while (pCtx->Hotkey [i].KeyID)
    {
        if (pCtx->Hotkey [i].KeyID == nHotKey)
        {
            if (pCtx->Hotkey [i].Command)
            {
                EraseCmd (pCtx);

                strcpy ((char *) pCtx->Cmd, (char *)pCtx->Hotkey [i].Command);
                pCtx->CmdPos = strlen ((char *)pCtx->Cmd);

                WriteStr(pCtx, (char *)pCtx->Cmd, FALSE);
                return ProccessChar(pCtx, CM_CR);
            }
            else if (pCtx->Hotkey [i].HotKeyHandler)
            {
                CMgrCmdCtx ctx;
                UINT32 nRes;

                ctx.UserData = pCtx->Hotkey [i].Data;
                ctx.WriteStr = (TCMgrWriteStr)CMgrWriteStr;
                ctx.SetMenu  = (TCMgrSetMenu)CMgrSetMenu;

                WriteStr(pCtx, NULL, TRUE);

                nRes = pCtx->Hotkey [i].HotKeyHandler (nHotKey, &ctx);

                WriteLogoStr(pCtx, (char *)pCtx->Cmd, FALSE);

                return nRes;
            }
            else
            {
                return RC_CM_HOTKEY_ERROR;
            }
        }

        i++;
    }

    return RC_CM_OK;
}

/*******************************************************************************
*         Declaration of interface functions of CONSOLE MANAGER
*******************************************************************************/


/** @biewf The function sends to com DEVICE (mainly UART) an ASCIIZ string
    @param pCtx  [in] - the console context
    @param pData [in] - the pointer to the ASCIIZ string
    @return [UINT32]  a number of bytes sent to the device

    *  \ingroup group_lte_services

    */

UINT32 CMgrWriteStr(CMgrCtx * pCtx, char * pData)
{
    UINT32 nCount = 0;

    if (pData == NULL)
        return 0;

    while (*pData)
    {
        if (pCtx->cbWrite (pCtx->CommDevData, *pData) == FALSE)
            break;

        pData ++;
        nCount++;
    }
    
    pCtx->cbWriteComp(pCtx->CommDevData);

    return nCount;
}

/** @brief The function initializes console object

    @param pLogo [in]      -   the console "LOGO"
    @param pInit [in]      -   the pointer to the initialization data
    @param pCtx  [in/out]  -   the console context

    @return [MXRC] error code RC_CM_XXX

    *  \ingroup group_lte_services

    */

MXRC CMgrInit (char * pLogo, CMgrInitData * pInit, CMgrCtx * pCtx)
{
    _ASSERT_PTR(pLogo);
    _ASSERT_PTR(pInit);
    _ASSERT_PTR(pCtx);

    _ASSERT_PTR(pInit->cbRead);
    _ASSERT_PTR(pInit->cbWrite);
    _ASSERT_PTR(pInit->cbWriteComp);

    memset (pCtx, 0, sizeof (CMgrCtx));

    pCtx->cbRead   = pInit->cbRead;
    pCtx->cbWrite  = pInit->cbWrite;
    pCtx->cbWriteComp = pInit->cbWriteComp;
    pCtx->MainMenu = pInit->MainMenu;
    pCtx->Hotkey   = pInit->Hotkey;
    pCtx->CommDevData = pInit->CommDevData;
    pCtx->Device = pInit->Device;

    strcpy (pCtx->szLogo, pLogo);

    pCtx->History.LastCmd = CM_ERROR_INDEX;

    pMgrContext = pCtx;

    return RC_CM_OK;
}

/** Register dynamic menu handler
 *
 * @param handler
 * @return

 *  \ingroup group_lte_services

 */
MXRC CMgrRegisterHandler(const char *command, TCMgrCommand handler)
{
    CMgrMenuElm *pMenu;
    UINT32 commandExist = 0;

    if (pMgrContext == NULL)
        return RC_CM_PARAM_ERROR;

    pMenu = pMgrContext->MainMenu;

    // to find the last element in the table

    while (pMenu->Name != NULL)
    {
        if (_strcmp((const char *)pMenu->Name,(char *) command) == 0)
        {
            commandExist = 1;
            break;
        }
        pMenu++;
    }

    // to move the last element of the table
    // it's a table terminator and mainly used
    // to write an error message if a console command
    // is not found

    if (!commandExist)
    *(pMenu + 1) = *pMenu;

    pMenu->Name = command;
    pMenu->ExecMenu = handler;

    return RC_CM_OK;
}

/** @brief The function processes user input of abstract device (mainly UART)
           and performs actions according to the user's command

    @param pCtx [in] - the console context

    @return [MXRC]  error code RC_CM_XXX

    *  \ingroup group_lte_services

    */

MXRC CMgrProcData (CMgrCtx * pCtx)
{
    UINT8 nChar = 0;
    UINT32 nRes = 0;

    /* processing of data stream */

    while ((nChar = pCtx->cbRead (pCtx->CommDevData)) != 0)
    {
        UINT32 nFlag = ProcStream(pCtx, nChar);

        switch (nFlag)
        {
            case CM_CC_NONE:
                break;

            case CM_CC_CTRL_C:
            case CM_CC_CTRL_X:
            case CM_CC_CTRL_Z:
                {
                    EraseCmd(pCtx);
                    pCtx->IsBreak = TRUE;
                    continue;
                }

            case CM_CC_ESC:
                {
                    EraseCmd(pCtx);
                    return RC_CM_OK;
                }

            case CM_CC_LEFT:
            case CM_CC_RIGHT:
                return RC_CM_OK;

            case CM_CC_UP:
                    return ReplaceCmdByHistory (pCtx, CM_CC_UP);

            case CM_CC_DOWN:
                    return ReplaceCmdByHistory (pCtx, CM_CC_DOWN);

            default:
                return ProcHotKey (pCtx, nFlag);

        }

        if ((nRes = ProccessChar (pCtx, nChar)) != RC_CM_OK)
            return nRes;

    }

    return RC_CM_OK;
}

/**
    @brief  This function is changing a context of console menu (commands)

    @param pCtx     [in] - a console context
    @param pMenu    [in] - a pointer to the menu table
    @param pLogoDel [in] - a pointer to a string that must be removed from LOGO string
    @param pLogoAdd [in] - a pointer to a string which will be  added to a LOGO string

    @return [UINT32] error code CM_RC_XXX

*  \ingroup group_lte_services

*/
MXRC CMgrSetMenu (CMgrCtx* pCtx, void* pMenu, char* pLogoDel, char* pLogoAdd)
{
    if (pMenu == NULL)
        return RC_CM_PARAM_ERROR;

    DelLogo (pCtx, pLogoDel);
    AddLogo (pCtx, pLogoAdd);

    pCtx->MainMenu = pMenu;

    return RC_CM_OK;
}

/** @brief The function updates the pattern of input
    @param pCtx [in] - the console context
    @return [MXRC] an error code {RC_CM_XXX}

    *  \ingroup group_lte_services

    */

MXRC CMgrUpdate  (CMgrCtx * pCtx)
{
    _ASSERT_PTR(pCtx);
    if(pCtx->Device != CLINE_DEVICE_CICPU)
        WriteStr(pCtx, NULL, TRUE);
    WriteLogoStr(pCtx, pCtx->Cmd, FALSE);

    return RC_CM_OK;
}

/** @brief  The function writes description about objects of table by using
            TextDescr table element

    @param pCtx   [in]  - the console context
    @param pTable [in]  -  a pointer to the table

    @return [MXRC] error code

    *  \ingroup group_lte_services

    */

MXRC  CMgrShowTableInfo (CMgrCtx * pCtx, CMgrMenuElm * pTable)
{
    UINT16 i = 0;
    UINT16 nLen = 0;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTable);

    WriteStr (pCtx, "\r\n[Commands]\r\n", TRUE);

    while (pTable [i].Name)
    {
        if ((pTable [i].Type & CM_OBJ_CMD) == CM_OBJ_CMD)
        {
            nLen = strlen (pTable [i].Name);
            WriteStr (pCtx, (char *) pTable [i].Name, FALSE);
        }
        else if ((pTable [i].Type & CM_OBJ_SUB) == CM_OBJ_SUB)
        {
            WriteStr (pCtx, "[", FALSE);
            WriteStr (pCtx, (char *) pTable [i].Name, FALSE);
            WriteStr (pCtx, "]", FALSE);

            nLen = strlen (pTable [i].Name) + 2;
        }

        if ((pTable [i].Type & CM_OBJ_DELIMITER) == CM_OBJ_DELIMITER)
        {
            WriteStr (pCtx, NULL, TRUE);
            WriteStr (pCtx, (char *) pTable [i].TextDescr, TRUE);

            i++;
            continue;
        }
        else if (pTable [i].TextDescr != NULL)
        {
            while ((CM_TABLEINFO_SIZE - nLen) > 0)
            {
                WriteStr (pCtx, " ", FALSE);
                nLen ++;
            }

            WriteStr (pCtx, " - ", FALSE);
            WriteStr (pCtx, pTable [i].TextDescr, FALSE);
        }

        WriteStr (pCtx, NULL, TRUE);
        i++;
        MxDelayTicks(1500000);
    }

    return RC_CM_OK;
}

/** @brief The function is showing usage information about specified command

    @param pCtx     [in] - the console context
    @param pCmdName [in] - a command name
    @param pTable   [in] - a pointer to the table of commands (the current command container)

    @return [MXRC] error code of console manager

    *  \ingroup group_lte_services

    */

MXRC CMgrShowTableUsageInfo (CMgrCtx * pCtx, char * pCmdName,
                               CMgrMenuElm * pTable)
{
    UINT32 i = 0;

    if (pTable == NULL)
        return RC_CM_PARAM_ERROR;

    if (pCmdName == NULL)
    {
        WriteStr (pCtx, "\r\n[Commands usage]\r\n", TRUE);

        while (pTable [i].Name)
        {
            if (pTable [i].UsageInfo != NULL)
            {
                WriteStr (pCtx, pTable [i].UsageInfo, TRUE);
            }

            i++;
        }
    }
    else
    {
        while (pTable [i].Name)
        {
            if (_strcmp (pTable [i].Name, pCmdName) == 0)
            {
                WriteStr (pCtx, pTable [i].UsageInfo, TRUE);
                break;
            }
            i++;
        }
    }

    return RC_CM_OK;
}

/** @brief The function cleans console screen
    @param pCtx [in] - the console context
    @return [UINT32] error code

    *  \ingroup group_lte_services

    */

UINT32  CMgrCleanScr (CMgrCtx * pCtx)
{
    UINT32 nStrCount = 80;

    _ASSERT_PTR(pCtx);

    while (nStrCount > 0)
    {
        WriteStr (pCtx, NULL, TRUE);

        nStrCount --;
    }

    return RC_CM_OK;
}

BOOL CMgrIsBreak (CMgrCtx * pCtx)
{
    BOOL b = pCtx->IsBreak;

    pCtx->IsBreak = FALSE;

    return b;
}

/** @brief This function returns console parameter like U32 value

    @param pParams [in] - the console parameters
    @param nIndex  [in] - the parameter index
    @param nDefVal [in] - the default value if parameter is not specified

    @return [UINT32] 32bits value

    *  \ingroup group_lte_services

    */

UINT32 CMgrGetParam (CMgrCmdParamCtx *pParams, UINT32 nIndex, UINT32 nDefVal)
{
    _ASSERT_PTR (pParams);

    if (nIndex >= pParams->ParamCount)
        return nDefVal;

    return StrToNum (pParams->ParamList[nIndex]);
}

