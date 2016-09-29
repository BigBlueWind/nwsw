//-------------------------------------------------------------------------------------------
/** @file cmgr.h
 *
 * @brief The declaration of console manager
 * @author Mindspeed Technologies
 * @version $Revision: 1.11 $
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

#ifndef  _CMGR_H_
#define  _CMGR_H_

#include "4gmx.h"
#include "appids.h"
/******************************************
*
*  Constants of type of terminal
*
*******************************************/
#define     CM_TT_FIFO                      1       /**< FIFO type of terminal, without supportion any editing of input commands                    */

/******************************************
*
*  Constants of type of objects in the tables of menu
*
*******************************************/
#define     CM_OBJ_CMD                      1       /**< Handler of command */
#define     CM_OBJ_SUB                      2       /**< Pointer to submenu */
#define     CM_OBJ_DELIMITER                4       /**< The menu delimiter, it's used in help commands*/
/******************************************
*
*  Constants of common purpose
*
*******************************************/
#define     CM_MAX_COMMAND_SIZE             100     /**< Maximum size of user command                   */
#define     CM_MAX_LOGO                     40      /**< Maximum size of a logo text                    */
#define     CM_MAX_PARAM_COUNT              20      /**< Maximun number of parameters in the command    */
#define     CM_INVALID_INDEX                ((UINT32)-1)
#define     CM_MAX_TEST_COUNT               50      /**< Maximun number of tests in the menu(submenu) */
#define     EOF                             (-1)
#define     CM_MAX_CMD_HISTORY              10      /**< The maximum size of history of commands        */
#define     CM_ERROR_INDEX                  ((UINT8)-1)
#define     CM_TABLEINFO_SIZE               (15)
/******************************************
*
*  Constants of control symbols
*
*******************************************/
#define     CM_CR                           '\r'    /**< Carret return                                          */
#define     CM_LF                           '\n'    /**< Line feed                                              */
#define     CM_COMMAND_BEGINNER             ">"     /**< A symbol of a command beginning                        */
#define     CM_BACKSPACE                    0x8     /**< Code of a backspace symbols                            */
#define     CM_TAB                          '\t'    /**< Code of a tab symbols                                  */
#define     CM_SPACE                        ' '     /**< Code of a space symbols                                */
#define     CM_SPACE_LEX_LIST               " \t"   /**< List of space characters of user input                 */
#define     CM_BREAK_LEX_LIST               ":"     /**< List of characters that are interpreted as lexems      */
#define     CM_CTRL_C                       0x3
#define     CM_CTRL_B                       0x2
#define     CM_CTRL_Z                       26
#define     CM_CTRL_X                       24

/******************************************
*
*  Constants of control symbols IDs
*
*******************************************/
#define     CM_CC_NONE                      0       /**< not a control byte     */
#define     CM_CC_CTRL_C                    3
#define     CM_CC_CTRL_X                    0x18
#define     CM_CC_CTRL_Z                    0x1A
#define     CM_CC_ESC                       0x1b    /**< begin of  control char */
#define     CM_CC_ACCEPT                    0x5B    /**< accept the control char*/
#define     CM_CC_ACCEPT_EX                 0x4F    /**< accept the control char*/
#define     CM_CC_UP                        0x41    /**< up arrow               */
#define     CM_CC_DOWN                      0x42    /**< down arrow             */
#define     CM_CC_LEFT                      0x44    /**< left arrow             */
#define     CM_CC_RIGHT                     0x43    /**< right arrow            */
#define     CM_CC_HOME                      0x48    /**< home arrow             */
#define     CM_CC_END                       0x4B    /**< end arrow              */
#define     CM_CC_F1                        0x50    /**< F1 key                 */
#define     CM_CC_F2                        0x51    /**< F2 key                 */
#define     CM_CC_F3                        0x52    /**< F3 key                 */
#define     CM_CC_F4                        0x53    /**< F4 key                 */

/******************************************
*
*  Constants of error codes
*
*******************************************/
#define     RC_CM_OK                    MX_OK                                   /**< Successfully                          */
#define     RC_CM_PARAM_ERROR           DEF_USRRC(APP_MODULE, CONSOLE_MGR, 1)  /**< Error of input parameter               */
#define     RC_CM_UNSUPPORTED_TT        DEF_USRRC(APP_MODULE, CONSOLE_MGR, 2)  /**< Unsupported type of terminal           */
#define     RC_CM_TT_ERROR              DEF_USRRC(APP_MODULE, CONSOLE_MGR, 3)  /**< Type of terminal error                 */
#define     RC_CM_DEV_CB_ERROR          DEF_USRRC(APP_MODULE, CONSOLE_MGR, 4)  /**< has not pointer to callback function   */
#define     RC_CM_OUT_BUF               DEF_USRRC(APP_MODULE, CONSOLE_MGR, 5)  /**< The command is out of buffer           */
#define     RC_CM_TT_UNSUPPORTED        DEF_USRRC(APP_MODULE, CONSOLE_MGR, 6)  /**< Type of terminal curently unsupported  */
#define     RC_CM_CMD_EMPTY             DEF_USRRC(APP_MODULE, CONSOLE_MGR, 7)  /**< User have input empty command          */
#define     RC_CM_HOTKEY_ERROR          DEF_USRRC(APP_MODULE, CONSOLE_MGR, 8)  /**< The hotkey table has error definition  */
#define     RC_CM_CMD_FORMAT_ERROR      DEF_USRRC(APP_MODULE, CONSOLE_MGR, 9)  /**< The command format error               */
/** @brief The context of command parameters*/

typedef struct
{
    UINT32  DstCpuID;                            /**< The destination CpuID, 0 - by default*/
    char *  CmdName;                             /**< name of command */
    char *  ParamList [CM_MAX_PARAM_COUNT];      /**< an array of pointers to the command's parameters  */
    UINT16  ParamCount;                          /**< Number of parameters in the command */

}CMgrCmdParamCtx, *PCMgrCmdParamCtx;

/** @brief The console history context */

typedef struct
{
    UINT8      LastCmd;             /**< The index of last command of history           */
    UINT8      CmdCount;            /**< The number of commands in thelist of history   */
    char       CmdList [CM_MAX_CMD_HISTORY] [CM_MAX_COMMAND_SIZE + 1];

}CMgrHistoryCtx, *PCMgrHistoryCtx;


typedef UINT8 (*TCMgrDevRead) (LPVOID Data);           /**< type of callback function of reading data from a stream            */
typedef UINT8 (*TCMgrDevWrite)(LPVOID Data, UINT8 ch); /**< type of callback function of writing data in  a stream             */
typedef UINT8 (*TCMgrDevWriteComp)(LPVOID Data);

typedef UINT32 (*TCMgrWriteStr)(LPVOID pCtx, char *);  /**< type of callback function of writing string into stream            */
typedef UINT32 (*TCMgrSetMenu)(LPVOID pCtx, void * pMenuTable, char * pLogoDel,
                              char * pLogoAdd);       /**< Type of function of changing a context of current menu of console  */

/******************************************
*          Declaration of Types           *
*******************************************/

typedef struct
{
    LPVOID       ConsoleCtx;  /**< The console object context, it's used in calling of console API commands */

    TCMgrWriteStr WriteStr;    /**< A pointer to the WRITE function                                    */
    TCMgrSetMenu  SetMenu;     /**< A pointer to the function of changing the context of console menu  */

    void *       MenuTable;   /**< The pointer to the table of current console menu                   */

    char *       TextDescr;   /**< The description of table object                                    */
    void *       UserData;    /**< Pointer to user data                                               */

}CMgrCmdCtx, *PCMgrCmdCtx;

typedef UINT32 (*TCMgrCommand) (CMgrCmdCtx * a, CMgrCmdParamCtx * b);
typedef UINT32 (*TCMgrHotKey)  (UINT32 nHotKey, LPVOID pCtx);

/** @brief The type of menu table */

typedef struct
{
    UINT8          Type;        /**< A type of menu table object CM_OBJ_XXX             */
    const char  *  Name;        /**< A pointer to the name of comand                    */
    void const *   SubMenu;     /**< A pointer to a table of submenu                    */
    TCMgrCommand   ExecMenu;    /**< A pointer to function of executing menu command    */
    char *         TextDescr;   /**< The description of table object                    */
    char *         UsageInfo;   /**< The information about parameters of command        */
    void *         UserData;    /**< A pointer to a some data                           */

}CMgrMenuElm, *PCMgrMenuElm;


/** @brief The structure is describing a hotkey element of hotkeys table */

typedef struct
{
    UINT32       KeyID;         /**< ID of control key ,  see {CM_CC_XXX}                       */
    char*        Command;       /**< The command of console manager assotiated with hotkey      */

    TCMgrHotKey  HotKeyHandler; /**< Pointer to handler of hot key commands                     */
    void *       Data;          /**< Pointer to some user's data                                */

}CMgrHotkeyCtx, *PCMgrHotkeyCtx;


/** @brief  Type of the initialization data of the console object */

typedef struct
{
    LPVOID              CommDevData;
    TCMgrDevRead        cbRead;     /**< Pointer to a function of writing data in the stream  */
    TCMgrDevWrite       cbWrite;    /**< Pointer to a function of reading data from the stream*/
    TCMgrDevWriteComp   cbWriteComp;
    PCMgrMenuElm        MainMenu;   /**< A pointer to the main menu of console manager        */
    PCMgrHotkeyCtx      Hotkey;     /**< The pointer to the table of hotkey commands          */
    UINT32				Device;

}CMgrInitData, *PCMgrInitData;

/** @brief The context of console object, this data is used in all of
           console manager API functions */

typedef struct _CMGR_CTX_
{
    char                szLogo [CM_MAX_LOGO + 1];/**< Text of logo string                     */

    LPVOID              CommDevData;
    TCMgrDevRead        cbRead;     /**< Pointer to a function of writing data in the stream  */
    TCMgrDevWrite       cbWrite;    /**< Pointer to a function of reading data from the stream*/
    TCMgrDevWriteComp   cbWriteComp;
    CMgrMenuElm   *     MainMenu;   /**< A pointer to the main menu of console manager        */
    CMgrHotkeyCtx *     Hotkey;     /**< The pointer to the table of hotkey commands          */

    char                Cmd[CM_MAX_COMMAND_SIZE + 1];
    UINT16              CmdPos;
    CMgrHistoryCtx      History;
    BOOL                IsBreak;
    UINT32              TabTime;
    UINT32				Device;

}CMgrCtx, *PCMgrCtx;

/** @brief The function initializes console object

    @param pLogo [in]      -   the console "LOGO"
    @param pInit [in]      -   the pointer to the initialization data
    @param pCtx  [in/out]  -   the console context

    @return [MXRC] error code RC_CM_XXX
    \ingroup group_lte_services
    */

MXRC CMgrInit (char * pLogo, CMgrInitData * pInit, CMgrCtx * pCtx);

/** @brief The function processes user input of abstract device (mainly UART)
           and performs actions according to the user's command

    @param pCtx [in] - the console context

    @return [MXRC]  error code RC_CM_XXX
    \ingroup group_lte_services
    */

MXRC CMgrProcData (CMgrCtx * pCtx);

/**
    @brief  This function is changing a context of console menu (commands)

    @param pCtx     [in] - a console context
    @param pMenu    [in] - a pointer to the menu table
    @param pLogoDel [in] - a pointer to a string that must be removed from LOGO string
    @param pLogoAdd [in] - a pointer to a string which will be  added to a LOGO string

    @return [UINT32] error code CM_RC_XXX

    \ingroup group_lte_services
*/
MXRC CMgrSetMenu (CMgrCtx* pCtx, void* pMenu, char* pLogoDel, char* pLogoAdd);

/** @brief The function updates input "LOGO"
    @param  pCtx [in] - the context
    @return [MXRC] error code {RC_CM_XXX}
    \ingroup group_lte_services
    */

MXRC CMgrUpdate  (CMgrCtx * pCtx);

/** @biewf The function sends to com DEVICE (mainly UART) an ASCIIZ string
    @param pCtx  [in] - the console context
    @param pData [in] - the pointer to the ASCIIZ string
    @return [UINT32]  a number of bytes sent to the device
    \ingroup group_lte_services
    */

UINT32 CMgrWriteStr(CMgrCtx * pCtx, char * pData);

/** @brief  The function writes description about objects of table by using
            TextDescr table element

    @param pCtx   [in]  - the console context
    @param pTable [in]  -  a pointer to the table

    @return [MXRC] error code
    \ingroup group_lte_services
    */

MXRC  CMgrShowTableInfo (CMgrCtx * pCtx, CMgrMenuElm * pTable);

/** @brief The function is showing usage information about specified command

    @param pCtx     [in] - the console context
    @param pCmdName [in] - a command name
    @param pTable   [in] - a pointer to the table of commands (the current command container)

    @return [MXRC] error code of console manager
    \ingroup group_lte_services
    */

MXRC CMgrShowTableUsageInfo (CMgrCtx * pCtx, char * pCmdName,
                               CMgrMenuElm * pTable);

/** @brief The function cleans console screen
    @param pCtx [in] - the console context
    @return [UINT32] error code
    \ingroup group_lte_services
    */

UINT32  CMgrCleanScr (CMgrCtx * pCtx);

/** @brief Registers dynamic console handler
    @param command [in] - console command
    @param handler [in] - pointer to function to handle this command and its parameters
    @return [UINT32] error code
    *  \ingroup group_lte_services
    */
MXRC CMgrRegisterHandler(const char *command, TCMgrCommand handler);

BOOL CMgrIsBreak (CMgrCtx * pCtx);

/** @brief This function returns console parameter like U32 value

    @param pParams [in] - the console parameters
    @param nIndex  [in] - the parameter index
    @param nDefVal [in] - the default value if parameter is not specified

    @return [UINT32] 32bits value
    \ingroup group_lte_services
    */

UINT32 CMgrGetParam (CMgrCmdParamCtx *pParams, UINT32 nIndex, UINT32 nDefVal);

#endif /*_CMGR_H_*/

#ifdef __cplusplus
}
#endif

