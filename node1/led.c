#include <avr/io.h>			
#include "led.h"			

void led_init() {
	DDRB |= (1 << DDB0);	//Pin B0 output
	PORTB |= (1 << PB0);	//Set pin PB0
}

void led_turn_on() {
	PORTB &= ~(1 << PB0);	//Clear pin PB0
}

void led_turn_off() {
	PORTB |= (1 << PB0);	//Set pin PB0
}