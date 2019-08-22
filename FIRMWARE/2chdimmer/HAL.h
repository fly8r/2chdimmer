/*
 * HAL.h
 *
 * Created: 01.04.2019 11:53:01
 *  Author: fly8r
 */
#ifndef HAL_H_
#define HAL_H_

#include <avr/io.h>

//------------------------------ Macros
#define HI(x)							((x)>>8)
#define LO(x)							((x)& 0xFF)
#define STATE_ON						1
#define STATE_OFF						0

//------------------------------ System timer configuration for 1mS interrupt
#define SYSTICK_TIME_MS					1
#define SYSTICK_PRESCALER				64UL
#define SYSTICK_OCR_CONST				((F_CPU*SYSTICK_TIME_MS) / (SYSTICK_PRESCALER*1000))
#define SYSTICK_TIMER_COUNTER           TCNT1
#define SYSTICK_TIMER_OCR               OCR1A
#define SYSTICK_TIMER_INIT()            { TCCR1A=0; TCCR1B=(1<<WGM12|1<<CS11|1<<CS10); SYSTICK_TIMER_OCR=SYSTICK_OCR_CONST; SYSTICK_TIMER_COUNTER=0; }
#define SYSTICK_INTERRUPT_ENABLE()      { TIMSK |= 1<<OCIE1A; }
#define SYSTICK_INTERRUPT_DISABLE()     { TIMSK &= ~(1<<OCIE1A); }

//------------------------------ PWM configuration
#define PWM_TIMER_COUNT_REG				TCNT0
#define PWM_TIMER_OCR_CH_A				OCR0A
#define PWM_TIMER_OCR_CH_B				OCR0B
#define PWM_DDR_CH_A					DDRB
#define PWM_PORT_CH_A					PORTB
#define PWM_MASK_CH_A					(1<<2)
#define PWM_DDR_CH_B					DDRD
#define PWM_PORT_CH_B					PORTB
#define PWM_MASK_CH_B					(1<<5)
#define PWM_INIT()						{ PWM_DDR_CH_A|=PWM_MASK_CH_A; PWM_DDR_CH_B|=PWM_MASK_CH_B; TCCR0A=(1<<WGM01|1<<WGM00); TCCR0B=(1<<CS00); }
#define PWM_SET_LEVEL_CH_A(lvl)			{ PWM_TIMER_OCR_CH_A=lvl; PWM_TIMER_COUNT_REG=0; }
#define PWM_SET_LEVEL_CH_B(lvl)			{ PWM_TIMER_OCR_CH_B=lvl; PWM_TIMER_COUNT_REG=0; }
#define PWM_CH_A_ON(lvl)				{ PWM_SET_LEVEL_CH_A(lvl); TCCR0A|=(1<<COM0A1); }
#define PWM_CH_B_ON(lvl)				{ PWM_SET_LEVEL_CH_B(lvl); TCCR0A|=(1<<COM0B1); }	
#define PWM_CH_A_OFF()					{ TCCR0A&=~(1<<COM0A1);  }
#define PWM_CH_B_OFF()					{ TCCR0A&=~(1<<COM0B1);  }

//------------------------------ Encoder configuration
#define ENC_DDR							DDRB
#define ENC_PIN							PINB
#define ENC_MASK						(1<<0|1<<1)
#define ENC_INIT()						{ ENC_DDR &= ~(ENC_MASK); }
#define ENC_STATE						(ENC_PIN & ENC_MASK)

//------------------------------ Encoder button configuration
#define BTN_DDR							DDRD
#define BTN_PIN							PIND
#define BTN_MASK						(1<<6)
#define BTN_INIT()						{ BTN_DDR &= ~(BTN_MASK); }
#define BTN_PRESSED						(!(BTN_PIN & BTN_MASK))

//------------------------------ LED PWM Channel A
#define LED_CH_A_DDR					DDRD
#define LED_CH_A_PORT					PORTD
#define LED_CH_A_MASK					(1<<4)
#define LED_CH_A_INIT()					{ LED_CH_A_DDR|=LED_CH_A_MASK; LED_CH_A_PORT&=~LED_CH_A_MASK; }
#define LED_CH_A_ON()					{ LED_CH_A_PORT|=LED_CH_A_MASK; }	
#define LED_CH_A_OFF()					{ LED_CH_A_PORT&=~LED_CH_A_MASK; }	
	
//------------------------------ LED PWM Channel B
#define LED_CH_B_DDR					DDRD
#define LED_CH_B_PORT					PORTD
#define LED_CH_B_MASK					(1<<3)
#define LED_CH_B_INIT()					{ LED_CH_B_DDR|=LED_CH_B_MASK; LED_CH_B_PORT&=~LED_CH_B_MASK; }
#define LED_CH_B_ON()					{ LED_CH_B_PORT|=LED_CH_B_MASK; }
#define LED_CH_B_OFF()					{ LED_CH_B_PORT&=~LED_CH_B_MASK; }	
	


#endif /* HAL_H_ */