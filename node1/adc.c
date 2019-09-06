#ifndef F_CPU
#define F_CPU 4915200	

#include <stdio.h>			
#include <avr/io.h> 		
#include <avr/interrupt.h>	
#include <util/delay.h>		

#include "adc.h"			

#define adc_init_address 0x1400		//ADC address 

void ADC_init(void){
	MCUCR |= (1 << SRE);	//External SRAM/XMEM Enable 
	SFIOR |= (1 << XMM2);	//External Memory High Mask [PC7 - PC4] 
	
	//cli();
}

uint8_t ADC_read(uint8_t channel) {
	volatile uint8_t *adc = (uint8_t *) adc_init_address;	
	_delay_us(1);					
	adc[channel] = channel;			
	while (PINE & (1<<PE0));		//Wait until ADC conversion process finishes (supposedly 40us according to datasheet)
	
	_delay_us(60);					//(at least 600ns according to datasheet)
	
	return *adc;
}

#endif //F_CPU