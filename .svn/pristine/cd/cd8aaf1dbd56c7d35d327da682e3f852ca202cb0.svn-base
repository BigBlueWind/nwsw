/**
 * @file     cih_tecla.h
 * @details  Copyright Powerwave Technologies, Inc., 2010
 * @brief    This file contains all function definitions and Macros required by CIH module.
 *
 * @author   mkata
 * @date     25-06-2010  Basic version
 */

#ifndef CIH_TECLA_H
#define CIH_TECLA_H

typedef struct s_CihTeclaContext
{
  GetLine    *getLine;
} CihTeclaContext;

/* Global Context */
extern CihTeclaContext     g_CihTeclaContext;

#define getGlobalCihTeclaContextt() (g_CihTeclaContext.getLine)

SignalActions *new_SignalActions(void);
SignalActions *del_SignalActions(SignalActions *si);
int displace_signal_handlers(SignalActions *si, sigset_t *mask,
				    void (*handler)(int));
int reinstate_signal_handlers(SignalActions *si);

/* Return resources, restore the terminal to a usable state and exit */

void cleanup_and_exit(GetLine *gl, SignalActions *si, int status);


/* A signal-aware version of select() */

int demo_sigselect(int n, fd_set *readfds, fd_set *writefds,
			  fd_set *exceptfds, struct timeval *timeout,
			  sigset_t *mask, SignalActions *si);

/* Signal handlers */

void demo_signal_handler(int signo);
void demo_setjmp_handler(int signo);

/*
 * Set the amount of time that gl_get_line() should wait for I/O before
 * returning to let the external event loop continue.
 */
#define DEMO_IO_TIMEOUT 100000000 /* ns => 100ms */
#define DEMO_IO_TIMEOUT_1S 1000000000 /* ns => 1000ms */

/* The timeout handler */
GL_TIMEOUT_FN(demo_timeout_fn);

void cihInitTecla(
  IO GetLine       **gl,
  IO SignalActions **si,
  IO sigset_t      *all_signal_mask
  );
void cihTelcaProcSignals(
  IO GetLine       **gl,
  IO SignalActions **si,
  IO sigset_t      *all_signal_mask,
  IO fd_set        *rfds,
  IO fd_set        *wfds
  );
#endif /*CIH_TECLA_H */
