/*
 * main.h
 *
 * Created: 01.04.2019 11:36:37
 *  Author: fly8r
 */
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "global_vars.h"
#include "config.h"
#include "HAL.h"
#include "libs/include/messages.h"
#include "libs/include/swtimers.h"
#include "fsms/include/encoder.h"
#include "fsms/include/button.h"
#include "fsms/include/system.h"


/************************************************************************/
/* VARS                                                                 */
/************************************************************************/
static		uint8_t		systick;		// System tick timer
