//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL appprintf.h
 *
 * @brief This file defines printf and sprintf functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.28 $
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

#ifndef __PRINTF_H_
#define __PRINTF_H_

#include <stdarg.h>
#include <stdio.h>
#include "uartdrv.h"

#define PRINT_DEV_UART      0
#define PRINT_DEV_BUF       1
#define PRINT_DEV_TRACE     2
#define PRINT_DEV_CICPU     3


#define TERM_OPT_RESET_ALL      (1 << 0)
#define TERM_OPT_BRIGHT         (1 << 1)
#define TERM_OPT_DIM            (1 << 2)
#define TERM_OPT_UNDERSCORE     (1 << 3)
#define TERM_OPT_BLINK          (1 << 4)
#define TERM_OPT_REVERSE        (1 << 5)
#define TERM_OPT_HIDDEN         (1 << 6)
#define TERM_OPT_TEXT_BLACK     (1 << 7)
#define TERM_OPT_TEXT_RED       (1 << 8)
#define TERM_OPT_TEXT_GREEN     (1 << 9)
#define TERM_OPT_TEXT_YELLOW    (1 << 10)
#define TERM_OPT_TEXT_BLUE      (1 << 11)
#define TERM_OPT_TEXT_MAGENTA   (1 << 12)
#define TERM_OPT_TEXT_CYAN      (1 << 13)
#define TERM_OPT_TEXT_WHITE     (1 << 14)
#define TERM_OPT_TEXT_BG_BLACK  (1 << 15)
#define TERM_OPT_TEXT_BG_RED    (1 << 16)
#define TERM_OPT_TEXT_BG_GREEN  (1 << 17)
#define TERM_OPT_TEXT_BG_YELLOW (1 << 18)
#define TERM_OPT_TEXT_BG_BLUE   (1 << 19)
#define TERM_OPT_TEXT_BG_MAGENTA (1 <<20)
#define TERM_OPT_TEXT_BG_CYAN   (1 << 21)
#define TERM_OPT_TEXT_BG_WHITE  (1 << 22)

/** @brief This function is designed to print data to the UART port in blocking mode
           it means that system is topped till message is printed out to UART(RS-232) port
           it's mainly designed to be used for debug purpose,
           regular [printf] can be used to printf data in non-blocking mode

    @params [as regular params of ANSI/C printf function]

    @return [the number of printed bytes] */

int uart_printf(const char *, ...);
int uart_force_printf(const char *fmt, ...);


void uart_printf_set_align(UINT32 nNum);
void uart_printf_reset_num(void);
UINT32 uart_printf_get_num(void);
void uart_printf_set_header(char * pHeader);
UINT32 uart_printf_select_dev(UINT32 nDevID, UINT32 param1, UINT32 param2);
UINT32 uart_printf_get_dev(void);
void uart_printf_print_buf(void);
void DisplayCallStack(void);

void set_term_opt (UINT32 nOpt);

extern MXRC __weak PhyDiStopHandler(char* fileName, UINT32 lineNum, char *stopString);

#define stop_printf(...) do {                                                   \
    char fileName[512];                                                        \
    char stopString[512];                                                      \
    UINT32 lineNum;                                                             \
    sprintf(fileName, __FILE__);                                                \
    sprintf(stopString, __VA_ARGS__);                                           \
    lineNum = __LINE__;                                                         \
    PhyDiStopHandler(fileName, lineNum, stopString);                            \
} while (0);

#endif /* printf.h */

#ifdef __cplusplus
}
#endif

