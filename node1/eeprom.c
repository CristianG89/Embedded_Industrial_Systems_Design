#include <avr/io.h>
#include <avr/interrupt.h>

#include "eeprom.h"


void EEPROM_write(uint8_t address, uint8_t data){
	
	cli();
	
  	while(!EECR & (1<<EEWE)); 	// Wait for previous write to finish
  	EEARH &= ~(1 << 0);       	// Set up address registers
  	EEARL = address;
	EEDR = data;              	// Set up data register
	EECR |= (1 << EEMWE);     	// Start write
	EECR |= (1 << EEWE);      	// Start write
     
	sei();
}

uint8_t EEPROM_read(uint8_t address){
	
	cli();
	
	while(!EECR & (1<<EEWE)); // Wait for previous write to finish
 	EEARH &= ~(1 << 0);       // Set up address registers
	EEARL = address;          // Set up address registers
	EECR |= (1 << EERE);      // Start reading by writing EERE
  	uint8_t ret_data;
	ret_data = EEDR;          // Return data from data register
	
	sei();
	
	return ret_data;
	
}

void EEPROM_reset(uint8_t num){
	
	for (uint8_t i = 0; i < num; i++){
		EEPROM_write(i,0);	
	}
		
}
