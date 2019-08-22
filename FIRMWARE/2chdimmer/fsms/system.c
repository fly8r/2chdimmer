/*
 * system.c
 *
 * Created: 01.04.2019 13:39:23
 *  Author: fly8r
 */
#include "include/system.h"

/************************************************************************/
/* VARS                                                                 */
/************************************************************************/
volatile	static		uint8_t			FSM_state;
static					uint8_t			delta=FSM_SYSTEM_OCR_DEFAULT_DELTA;
static					uint8_t			flash_count;


/* EEPROM data */
static					settings_data_t	EEMEM	ee_settings;

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void FSM_SYSTEM_SetPWMByChannelStates(void);
void FSM_SYSTEM_LoadSettingsFromEEPROM(void);
void FSM_SYSTEM_SaveSettingsToEEPROM(const uint8_t channel);

void FSM_SYSTEM_Init(void)
{
	// Set default FSM state
	FSM_state = FSM_SYSTEM_STATE_IDLE;
	// Load settings from EEPROM
	FSM_SYSTEM_LoadSettingsFromEEPROM();
	// Load current value from PWM table
	if(device.settings.level.ch_a==0 || device.settings.level.ch_a==0xFF) device.settings.level.ch_a=16;
	if(device.settings.level.ch_b==0 || device.settings.level.ch_b==0xFF) device.settings.level.ch_b=16;
	// Set default channel state flags
	device.state._ch_a = device.state._ch_b = 0;
}

void FSM_SYSTEM_Process(void)
{	
	uint8_t t=0;
	switch(FSM_state) {
		/* Default FSM state */
		case FSM_SYSTEM_STATE_IDLE: {
			/************************************************************************/
			/* Button processing                                                    */
			/************************************************************************/
			if(GetMessage(MSG_BTN_KEY_PRESSED)) { // <- Press message processing
				// Getting press type
				uint8_t *press_type = GetMessageParam(MSG_BTN_KEY_PRESSED);
				// Set channel A state flag
				device.state._ch_a = 1;
				// Processing button press by press time
				if(*press_type == BUTTON_EVENT_LONG_PRESS) {
					// Set channel B state flag
					device.state._ch_b = 1;
					// Goto CHANNEL A&B ON fsm
					FSM_state = FSM_SYSTEM_STATE_CH_AB_ON;
				} else {
					// Goto CHANNEL A ON fsm
					FSM_state = FSM_SYSTEM_STATE_CH_A_ON;
				}
				// Processing PWM by channel states
				FSM_SYSTEM_SetPWMByChannelStates();
				// Flush fsm timer
				ResetTimer(TIMER_SYSTEM);
			} // End MSG_BTN_KEY_PRESSED message processing
			return;
		}
		
		/* PWN Channel A TURN ON state */
		case FSM_SYSTEM_STATE_CH_A_ON: {
			/************************************************************************/
			/* Button processing                                                    */
			/************************************************************************/
			if(GetMessage(MSG_BTN_KEY_PRESSED)) { // <- Press message processing
				// Getting press type
				uint8_t *press_type = GetMessageParam(MSG_BTN_KEY_PRESSED);
				// Processing button press by press time
				if(*press_type == BUTTON_EVENT_LONG_PRESS) { // <- LONG press processing
					// Save PWM level data to EEPROM for channel A
					FSM_SYSTEM_SaveSettingsToEEPROM(PWM_CH_A);
					// Set flashing count
					flash_count = FSM_SYSTEM_FLASH_COUNT * 2;
					// Goto CHANNEL A level saved indication
					FSM_state = FSM_SYSTEM_STATE_CH_A_LVL_SAVED;
				} else {
					// Flush channel A state flag
					device.state._ch_a=0;
					// Set channel B state flag
					device.state._ch_b=1;
					// Processing PWM by channel states
					FSM_SYSTEM_SetPWMByChannelStates();
					// Goto CHANNEL B ON fsm
					FSM_state = FSM_SYSTEM_STATE_CH_B_ON;
				}
				// Flush fsm timer
				ResetTimer(TIMER_SYSTEM);
				return;
			} // End MSG_BTN_KEY_PRESSED message processing
			
			/************************************************************************/
			/* Encoder processing                                                   */
			/************************************************************************/
			if(GetMessage(MSG_ENC_ROTATE)) {
				// Get rotate direction
				int8_t *rotate = GetMessageParam(MSG_ENC_ROTATE);
				// Calculate level delta by level value
				if(device.settings.level.ch_a > (FSM_SYSTEM_OCR_MAX_VALUE-64)) {
					delta = FSM_SYSTEM_OCR_MAX_DELTA;
				} else {
					delta = FSM_SYSTEM_OCR_DEFAULT_DELTA;
				}
				// Rotation processing
				if(*rotate > 0) {
					t = device.settings.level.ch_a + delta;
					if(t >= FSM_SYSTEM_OCR_MAX_VALUE) {
						device.settings.level.ch_a = FSM_SYSTEM_OCR_MAX_VALUE;
					} else {
						device.settings.level.ch_a = t;
					}
				} else {
					t = device.settings.level.ch_a - delta;
					if(t < FSM_SYSTEM_OCR_MIN_VALUE) {
						device.settings.level.ch_a = FSM_SYSTEM_OCR_MIN_VALUE;
					} else {
						device.settings.level.ch_a = t;
					}
				}
				// Set PWM value for channel A
				PWM_SET_LEVEL_CH_A(device.settings.level.ch_a);
			} // End MSG_ENC_ROTATE message processing
			return;
		}
		
		/* Channel A blinking function */
		case FSM_SYSTEM_STATE_CH_A_LVL_SAVED: {
			// Check flashing period
			if(GetTimer(TIMER_SYSTEM) > FSM_SYSTEM_FLASH_HALF_PERIOD) {
				//
				if(!--flash_count) {
					// Set channel A state flag
					device.state._ch_a = 1;
					// Processing PWM by channel states
					FSM_SYSTEM_SetPWMByChannelStates();
					// Return to channel A ON state
					FSM_state = FSM_SYSTEM_STATE_CH_A_ON;
					return;
				}
				// Toggle channel A state
				device.state._ch_a = (device.state._ch_a) ? 0 : 1;
				// Processing PWM by channel states
				FSM_SYSTEM_SetPWMByChannelStates();
				// Flush fsm timer
				ResetTimer(TIMER_SYSTEM);
			}
			break;
		}
		
		/* PWM Channel B TURN ON state */
		case FSM_SYSTEM_STATE_CH_B_ON: {
			/************************************************************************/
			/* Button processing                                                    */
			/************************************************************************/
			if(GetMessage(MSG_BTN_KEY_PRESSED)) { // <- Press message processing
				// Getting press type
				uint8_t *press_type = GetMessageParam(MSG_BTN_KEY_PRESSED);
				// Processing button press by press time
				if(*press_type == BUTTON_EVENT_LONG_PRESS) { // <- LONG press processing
					// Save PWM level data to EEPROM for channel B
					FSM_SYSTEM_SaveSettingsToEEPROM(PWM_CH_B);
					// Set flashing count
					flash_count = FSM_SYSTEM_FLASH_COUNT * 2;
					// Goto CHANNEL B level saved indication
					FSM_state = FSM_SYSTEM_STATE_CH_B_LVL_SAVED;
				} else {
					// Set channel A state flag
					device.state._ch_a=1;
					// Set channel B state flag
					device.state._ch_b=1;
					// Processing PWM by channel states
					FSM_SYSTEM_SetPWMByChannelStates();
					// Goto CHANNEL B ON fsm
					FSM_state = FSM_SYSTEM_STATE_CH_AB_ON;
				}
				// Flush fsm timer
				ResetTimer(TIMER_SYSTEM);
				return;
			} // End MSG_BTN_KEY_PRESSED message processing
			
			/************************************************************************/
			/* Encoder processing                                                   */
			/************************************************************************/
			if(GetMessage(MSG_ENC_ROTATE)) {
				// Get rotate direction
				int8_t *rotate = GetMessageParam(MSG_ENC_ROTATE);
				// Calculate level delta by level value
				if(device.settings.level.ch_b > (FSM_SYSTEM_OCR_MAX_VALUE-64)) {
					delta = FSM_SYSTEM_OCR_MAX_DELTA;
				} else {
					delta = FSM_SYSTEM_OCR_DEFAULT_DELTA;
				}
				// Rotation processing
				if(*rotate > 0) {
					t = device.settings.level.ch_b + delta;
					if(t >= FSM_SYSTEM_OCR_MAX_VALUE) {
						device.settings.level.ch_b = FSM_SYSTEM_OCR_MAX_VALUE;
					} else {
						device.settings.level.ch_b = t;
					}
				} else {
					t = device.settings.level.ch_b - delta;
					if(t < FSM_SYSTEM_OCR_MIN_VALUE) {
						device.settings.level.ch_b = FSM_SYSTEM_OCR_MIN_VALUE;
					} else {
						device.settings.level.ch_b = t;
					}
				}
				// Set PWM value for channel B
				PWM_SET_LEVEL_CH_B(device.settings.level.ch_b);
			} // End MSG_ENC_ROTATE message processing
			break;
		}
		
		/* Channel B blinking function */
		case FSM_SYSTEM_STATE_CH_B_LVL_SAVED: {
			// Check flashing period
			if(GetTimer(TIMER_SYSTEM) > FSM_SYSTEM_FLASH_HALF_PERIOD) {
				//
				if(!--flash_count) {
					// Set channel B state flag
					device.state._ch_b = 1;
					// Processing PWM by channel states
					FSM_SYSTEM_SetPWMByChannelStates();
					// Return to channel A ON state
					FSM_state = FSM_SYSTEM_STATE_CH_B_ON;
					return;
				}
				// Toggle channel B state
				device.state._ch_b = (device.state._ch_b) ? 0 : 1;
				// Processing PWM by channel states
				FSM_SYSTEM_SetPWMByChannelStates();
				// Flush fsm timer
				ResetTimer(TIMER_SYSTEM);
			}
			break;
		}

		/* PWM Channel A&B TURN ON state */
		case FSM_SYSTEM_STATE_CH_AB_ON: {
			/************************************************************************/
			/* Button processing                                                    */
			/************************************************************************/
			if(GetMessage(MSG_BTN_KEY_PRESSED)) { // <- Press message processing
				// Getting press type
				uint8_t *press_type = GetMessageParam(MSG_BTN_KEY_PRESSED);
				// Processing button press by press time
				if(*press_type == BUTTON_EVENT_LONG_PRESS) { // <- LONG press processing
					// Save PWM level data to EEPROM for channel B
					FSM_SYSTEM_SaveSettingsToEEPROM(PWM_CH_AB);
					// Set flashing count
					flash_count = FSM_SYSTEM_FLASH_COUNT * 2;
					// Goto CHANNEL B level saved indication
					FSM_state = FSM_SYSTEM_STATE_CH_AB_LVL_SAVED;
				} else {
					// Flush channel A state flag
					device.state._ch_a=0;
					// Flush channel B state flag
					device.state._ch_b=0;
					// Processing PWM by channel states
					FSM_SYSTEM_SetPWMByChannelStates();
					// Goto CHANNELs A&B OFF fsm
					FSM_state = FSM_SYSTEM_STATE_IDLE;
				}
				// Flush fsm timer
				ResetTimer(TIMER_SYSTEM);
				return;
			} // End MSG_BTN_KEY_PRESSED message processing
			break;
		}
		
		/* Channels A&B blinking function */
		case FSM_SYSTEM_STATE_CH_AB_LVL_SAVED: {
			// Check flashing period
			if(GetTimer(TIMER_SYSTEM) > FSM_SYSTEM_FLASH_HALF_PERIOD) {
				//
				if(!--flash_count) {
					// Set channel A state flag
					device.state._ch_a = 1;
					// Set channel B state flag
					device.state._ch_b = 1;
					// Processing PWM by channel states
					FSM_SYSTEM_SetPWMByChannelStates();
					// Return to channel A ON state
					FSM_state = FSM_SYSTEM_STATE_CH_AB_ON;
					return;
				}
				// Toggle channel A state
				device.state._ch_a = (device.state._ch_a) ? 0 : 1;
				// Toggle channel B state
				device.state._ch_b = (device.state._ch_b) ? 0 : 1;
				// Processing PWM by channel states
				FSM_SYSTEM_SetPWMByChannelStates();
				// Flush fsm timer
				ResetTimer(TIMER_SYSTEM);
			}
			break;
		}
		
		default: break;
	}

}

void FSM_SYSTEM_SetPWMByChannelStates(void) 
{
	/* Channel A state processing*/
	if(device.state._ch_a) {
		// Setup PWM level for channel A and turn on
		PWM_CH_A_ON(device.settings.level.ch_a);
		// Turn on channel LED state for channel A
		LED_CH_A_ON();
	} else {
		// Setup PWM level for channel A and turn off
		PWM_CH_A_OFF();
		// Turn off channel LED state for channel A
		LED_CH_A_OFF();
	}
	/* Channel B state processing*/
	if(device.state._ch_b) {
		// Setup PWM level for channel B and turn on
		PWM_CH_B_ON(device.settings.level.ch_b);
		// Turn on channel LED state for channel B
		LED_CH_B_ON();
	} else {
		// Setup PWM level for channel B and turn off
		PWM_CH_B_OFF();
		// Turn off channel LED state for channel B
		LED_CH_B_OFF();
	}
}

void FSM_SYSTEM_LoadSettingsFromEEPROM(void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		// Waiting for EEPROM ready
		while(!eeprom_is_ready());
		// Read settings from EEPROM
		eeprom_read_block((void *)&device.settings, (void *)&ee_settings, sizeof(ee_settings));
	}
}

void FSM_SYSTEM_SaveSettingsToEEPROM(const uint8_t channel)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		// Waiting for EEPROM ready
		while(!eeprom_is_ready());
		switch(channel) {
			
			case PWM_CH_A: {
				// Store settings to EEPROM for channel A
				eeprom_write_byte(&ee_settings.level.ch_a, device.settings.level.ch_a);
				break;
			}
			
			case PWM_CH_B: {
				// Store settings to EEPROM for channel B
				eeprom_write_byte(&ee_settings.level.ch_b, device.settings.level.ch_b);
				break;
			}
			
			case PWM_CH_AB: {
				// Store settings to EEPROM for both channels
				eeprom_write_block((void *)&device.settings, (void *)&ee_settings, sizeof(device.settings));
				break;
			}
			
			default:break;
		}
		
	}
}