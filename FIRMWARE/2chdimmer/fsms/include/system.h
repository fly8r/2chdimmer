/*
 * system.h
 *
 * Created: 01.04.2019 13:39:11
 *  Author: fly8r
 */
#ifndef FSM_SYSTEM_H_
#define FSM_SYSTEM_H_


#include <avr/eeprom.h>
#include <util/atomic.h>
#include "../../global_vars.h"
#include "../../config.h"
#include "../../HAL.h"
#include "../../libs/include/messages.h"
#include "../../libs/include/swtimers.h"

/************************************************************************/
/* FSM states                                                           */
/************************************************************************/
enum FSM_SYSTEM_STATES_ENUM
{
	FSM_SYSTEM_STATE_IDLE=0,
	FSM_SYSTEM_STATE_CH_A_ON,
	FSM_SYSTEM_STATE_CH_B_ON,
	FSM_SYSTEM_STATE_CH_AB_ON,
	FSM_SYSTEM_STATE_CH_A_LVL_SAVED,
	FSM_SYSTEM_STATE_CH_B_LVL_SAVED,
	FSM_SYSTEM_STATE_CH_AB_LVL_SAVED,
};

/************************************************************************/
/* MACROS                                                               */
/************************************************************************/
#define		FSM_SYSTEM_FLASH_COUNT			2
#define		FSM_SYSTEM_FLASH_HALF_PERIOD	300
#define		FSM_SYSTEM_OCR_MIN_VALUE		16
#define		FSM_SYSTEM_OCR_MAX_VALUE		224
#define		FSM_SYSTEM_OCR_DEFAULT_DELTA	4
#define		FSM_SYSTEM_OCR_MAX_DELTA		8

/************************************************************************/
/* External VARS                                                        */
/************************************************************************/
enum PWM_CHANNELS_ENUM
{
	PWM_CH_A=0,
	PWM_CH_B,
	PWM_CH_AB	
};

/************************************************************************/
/* External Functions                                                   */
/************************************************************************/
// Main FSM initialize function
void FSM_SYSTEM_Init(void);
// Processing FSM function
void FSM_SYSTEM_Process(void);



#endif /* FSM_SYSTEM_H_ */
