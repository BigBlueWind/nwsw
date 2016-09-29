#ifndef LTE_PRINTF_H
#define LTE_PRINTF_H

#ifdef MX_ARM_ENABLED
#include "appprintf.h"
#define printf      uart_printf
#endif // MX_ARM_ENABLED

#endif //LTE_PRINTF_H
