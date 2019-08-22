/*
 * sbtrimmer.c
 *
 * Created: 01.04.2019 11:36:20
 * Author : fly8r
 */
#include "main.h"

/* Functions prototypes */
void InitGPIO(void);

//------------------------------ System tick interrupt
ISR(TIMER1_COMPA_vect)
{
	systick++;
}


int main(void)
{
	// Main initialize
	InitGPIO();
	InitMessages();
	InitTimers();
	// FSMs initialize
	FSM_SYSTEM_Init();
	FSM_ENC_Init();
	FSM_BUTTON_Init();
	// Enable global interrupts
	sei();


    /* Main loop */
    while (1) {
		// Processing software timers queue
		ProcessTimers(&systick);
		// Processing FSMs
		FSM_SYSTEM_Process();
		FSM_ENC_Process();
		FSM_BUTTON_Process();
		// Processing messages queue
		ProcessMessages();
    }
}

//------------------------------ MCU IO initialization
inline void InitGPIO(void)
{
	// Initialize systick timer
	SYSTICK_TIMER_INIT();
	SYSTICK_INTERRUPT_ENABLE();
	// PWM initialization
	PWM_INIT();
	// Encoder IO initialization
	ENC_INIT();
	BTN_INIT();
}