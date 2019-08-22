/*
 * swtimers.h
 *
 * Created: 23.01.2019 10:47:06
 *  Author: fly8r
 */
#ifndef SWTIMERS_H
#define SWTIMERS_H

#include <stdio.h>
#include <util/atomic.h>
#include "../../HAL.h"

//#define USE_GLOBAL_TIMERS

#define		SEC 				1000
#define		MIN 				60 * SEC
#define		HOUR 				60 * MIN

// Mas timers counter
#define		MAX_TIMERS			3
// Timers enum
enum TIMERS_ENUM
{
	TIMER_SYSTEM=0,
	TIMER_ENC,
	TIMER_BUTTON,
};

void		InitSystemTimer(void);
void		InitTimers(void);
void		ProcessTimers(uint8_t * tick);
uint16_t	GetTimer(uint8_t Timer);
void		ResetTimer(uint8_t Timer);

#ifdef USE_GLOBAL_TIMERS
#define MAX_GTIMERS 	1

enum GTIMERS_ENUM
{
	GTIMER_1=0,
};

uint16_t	GetGTimer(uint8_t Timer);
void		StopGTimer(uint8_t Timer);
void		StartGTimer(uint8_t Timer);
void		PauseGTimer(uint8_t Timer);
void		ResumeGTimer(uint8_t Timer);
#endif


#endif /* SWTIMERS_H_ */
