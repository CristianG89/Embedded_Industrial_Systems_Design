#include <stdio.h>
#include <avr/io.h>

#include "adc.h"			

void ADC_init(void) {
	DDRF |= (1 << DDF0);					//Pin PF0 enabled as output
	PORTF &= ~(1 << PF0);					//Clear pin PF0, others unchanged
	ADCSRA |= (1 << ADEN);					//Turn ADC on
	ADMUX |= (1 << REFS1) | (1 << REFS0); 	//Internal 2.56Vref with external capacitor at AREF pin  
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	
}

uint16_t ADC_read() {
	uint16_t data = 0;
	ADCSRA |= (1 << ADSC); 					//Start a new ADC conversion
	while(!(ADCSRA & (1 << ADIF))); 		//Loop until ADC conversion completes
	uint16_t data_l = ADCL;
	uint16_t data_h = (ADCH << 8);
	data = data_h + data_l;
	
	return data;
}