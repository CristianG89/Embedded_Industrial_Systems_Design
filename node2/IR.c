#ifndef F_CPU
#define F_CPU 16000000	

#include <stdio.h>		
#include <avr/io.h> 	
#include <util/delay.h>	

#include "IR.h"
#include "adc.h"

uint8_t IR_index = 0;
uint8_t IR_init_val = 0;
uint8_t n_IR_readings = 4;
uint8_t IR_readings[4];
uint16_t IR_sum = 0;


void IR_init(){
	_delay_ms(100);
	for (uint8_t i = 0; i < n_IR_readings; i++){
		IR_readings[i] = ADC_read();		// Fill readings with IR initial values
		_delay_ms(100);
		IR_sum += IR_readings[i];			// Initialize sum

	}
	IR_init_val = IR_readings[3];
	fprintf(UART_p, "IR init: %d \r\n", IR_init_val);
}

uint8_t IR_triggered() {
	uint8_t IR_val = IR_read_filtered();

	fprintf(UART_p, "IR avg: %d \r\n", IR_val);

	if (IR_val < (IR_init_val-50)){		// Cutoff for deciding if player missed the ball

		return 1;						
	}
	return 0;						
}

uint8_t IR_read_filtered() {
	IR_sum -= IR_readings[IR_index];	//Make sure sum is total of only the 4 last measurements
	IR_readings[IR_index] = ADC_read();	//Read raw data from photodiode
	IR_sum += IR_readings[IR_index];	//Add measurement to total value
	IR_index++;

	if (IR_index >= n_IR_readings){
		IR_index = 0;
	}
	uint8_t IR_avg = IR_sum / n_IR_readings;	//Calculate and update IR value with the average of the 4 last measurements

	return IR_avg;
}
#endif //F_CPU 