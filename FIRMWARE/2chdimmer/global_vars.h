/*
 * global_vars.h
 *
 * Created: 01.04.2019 11:52:28
 *  Author: fly8r
 */
#ifndef GLOBAL_VARS_H_
#define GLOBAL_VARS_H_

#include <stdio.h>
#include <avr/pgmspace.h>
#include "config.h"

/************************************************************************/
/* main.h, system.h                                                               */
/************************************************************************/
typedef struct
{
	struct {
		uint8_t	ch_a, ch_b;
	} level;
} settings_data_t;

typedef struct
{
	struct {
		uint8_t _ch_a, _ch_b;
	} state;
	settings_data_t	settings;
} device_data_t;
extern	device_data_t	device;


/************************************************************************/
/* button.h                                                             */
/************************************************************************/
enum BUTTON_EVENTS_ENUM
{
	BUTTON_EVENT_NOT_PRESSED,   // No events
	BUTTON_EVENT_SHORT_PRESS,   // Short press event
	BUTTON_EVENT_LONG_PRESS     // Long press event
};

#endif /* GLOBAL_VARS_H_ */