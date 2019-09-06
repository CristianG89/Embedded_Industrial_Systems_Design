#ifndef F_CPU
#define F_CPU 4915200	

#include <avr/io.h> 	
#include <stdio.h> 		
#include <avr/interrupt.h>	
	
#include "uart.h"
#include "timer.h"

int8_t int_tim1 = 0;


uint8_t timer_set_prescaler(uint16_t presc_value) {	
	uint8_t pres_config = 0;
	switch(presc_value) {				//As all CSxx occupy the same position, one can just use the same all the time
		case 1:		pres_config = (1<<CS00) | (0<<CS01) | (0<<CS02);	//PWM's clock activated, with prescaler N=1
		break;
		case 8: 	pres_config = (0<<CS00) | (1<<CS01) | (0<<CS02);	//PWM's clock activated, with prescaler N=8
		break;
		case 64: 	pres_config = (1<<CS00) | (1<<CS01) | (0<<CS02);	//PWM's clock activated, with prescaler N=64
		break;
		case 256: 	pres_config = (0<<CS00) | (0<<CS01) | (1<<CS02);	//PWM's clock activated, with prescaler N=256
		break;
		case 1024:	pres_config = (1<<CS00) | (0<<CS01) | (1<<CS02);	//PWM's clock activated, with prescaler N=1024
		break;
		default:	pres_config = (0<<CS00) | (0<<CS01) | (0<<CS02);	//PWM's clock deactivated
	}
	
	return pres_config;
}


void timer1_init(uint16_t prescaler, uint16_t frequency) {		
	TCCR1A |= (0<<WGM10) | (1<<WGM11);			 //Configure fast PWM (mode 14), with value TOP in ICR5
	TCCR1B |= (1<<WGM12) | (1<<WGM13);
	
	TCCR1B |= timer_set_prescaler(prescaler);	//Prescaler is set in the PWM output

	TCCR1A |= (0<<COM1A0) | (1<<COM1A1);		//PWM output in channel A (called OC5A) enabled. it clears on Compare Match, and sets at BOTTOM (non-inverting mode)

	uint16_t TOP = (F_CPU/frequency) / prescaler - 1;
	ICR1 = TOP;									//ICR3 is loaded according to the requested frequency
	
	OCR1A = TOP-1;								//Width of the PWM 

	TIMSK |= (1<<OCIE1A);
}

#endif