
#ifndef HCI_PRINT_H
#define	HCI_PRINT_H

#include "appprintf.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define _IPERF_PRINT(...) uart_printf (__VA_ARGS__) 
#define _IPERF_FPRINT(str, a, ...)   uart_printf (__VA_ARGS__)

#ifdef	__cplusplus
}
#endif

#endif	/* HCI_PRINT_H */

