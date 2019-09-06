#ifndef F_CPU
#define F_CPU 16000000

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
	
#include "uart.h"
#include "PWM.h"

uint8_t PWM_set_prescaler(uint16_t presc_value) {
	uint8_t pres_config = 0;
	switch(presc_value) {			//As all CSxx occupy the same position, one can just use the same all the time
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

void PWM_PB5_init(uint16_t prescaler, uint16_t frequency) {
	TCCR1A |= (0<<WGM10) | (1<<WGM11);	 	//Configure fast PWM (mode 14), with value TOP in ICR1
	TCCR1B |= (1<<WGM12) | (1<<WGM13);

	TCCR1B |= PWM_set_prescaler(prescaler);	//Prescaler is set in the PWM output

	TCCR1A |= (0<<COM1A0) | (1<<COM1A1);	//PWM OC1A (channel A) enabled. Non-inverting mode
	DDRB |= (1<<DDB5);						//OC1A physically connected in PB5, so such a pin is defined as output (1=out, 0=in)

	uint16_t TOP = (F_CPU/frequency) / prescaler - 1;
	ICR1 = TOP;								//ICR1 (defined as TOP) is loaded according to the requested frequency
	
	OCR1A = 0x0000;							//Width of the PWM (initialized to 0 = NOT WORKING)
}

void PWM_PE3_init(uint16_t prescaler, uint16_t frequency) {
	TCCR3A |= (0<<WGM30) | (1<<WGM31);
	TCCR3B |= (1<<WGM32) | (1<<WGM33);
	
	TCCR3B |= PWM_set_prescaler(prescaler);

	TCCR3A |= (0<<COM3A0) | (1<<COM3A1);
	DDRE |= (1<<DDE3);

	uint16_t TOP = (F_CPU/frequency) / prescaler - 1;
	ICR3 = TOP;
	
	OCR3A = TOP-1;

	TIMSK3 |= (1<<OCIE3A);
}

void PWM_PL3_init(uint16_t prescaler, uint16_t frequency) {
	TCCR5A |= (0<<WGM50) | (1<<WGM51);
	TCCR5B |= (1<<WGM52) | (1<<WGM53);
	
	TCCR5B |= PWM_set_prescaler(prescaler);

	TCCR5A |= (0<<COM5A0) | (1<<COM5A1);
	//DDRL |= (1<<DDL3);

	uint16_t TOP = (F_CPU/frequency) / prescaler - 1;
	ICR5 = TOP;
	
	OCR5A = TOP-1;

	TIMSK5 |= (1<<OCIE5A);
}

void set_PWMwidth(char* ch_PWM, uint8_t PWM_width) {
	if (ch_PWM == "PB5") {OCR1A = PWM_width;}
	else if (ch_PWM == "PE3") {OCR3A = PWM_width;}
}

#endif // F_CPU