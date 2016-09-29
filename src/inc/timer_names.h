/**
 * @file    timer_names.h
 * @brief Defines the API to print Timer names in the logs
 * timer_names.c implements this method, timer_names.c is generated from shell script.
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @author     : Shweta Polepally
 * Date       :
 * Description:
 */

#ifndef TIMER_NAMES_H
#define TIMER_NAMES_H

#include "bs_tables.h"

inline const UINT8* getTimerName(IN TimerName timerType);

#endif

