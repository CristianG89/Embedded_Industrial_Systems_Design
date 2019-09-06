#include <avr/io.h>
#include "led.h"

void led_init() {
	DDRA |= (1 << DDA0);	//Pin 0 output
	PORTA |= (1 << PINA0);	//Set pin PA0
}

void led_turn_off() {
	PORTA &= ~(1 << PINA0);	//Clear pin PA0
}

void led_turn_on() {
	PORTA |= (1 << PINA0);	//Set pin PA0
}