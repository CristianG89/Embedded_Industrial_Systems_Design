#include <avr/io.h> 	
#include <stdio.h> 		
#include <avr/interrupt.h>

#include "spi.h"				

#define CAN_CS DDB4 //Location of Chip Select (CS) pin of CAN controller (MCP2515)

void SPI_init(void) {	
	DDRB = (1<<CAN_CS)|(1<<DDB5)|(0<<DDB6)|(1<<DDB7);	//DDB4 = SS (out), DDB5 = MOSI (out), DDB6 = MISO (in), DDB7 = SCK (out)
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0); //SPE=1 to enable SPI comm, MSTR=1 to be Master (=0 Slave), SPR0/SPR1 to define clock rate (FOSC/16 chosen)
}

void SPI_write(char cData) {	
	SPDR = cData;				//SPDR register (to start transmission) is updated
	//cli();
	while(!(SPSR & (1<<SPIF)));	//Bit SPIF=1 when transmission finishes (inside SPSR register - Status Register)
	//sei();
}

char SPI_read(void) {	
	SPDR = 0x00;				//Set dummy byte in data register (to clean the buffer)
	//cli();
	while(!(SPSR & (1<<SPIF)));	//Waits until the current transmission finishes
	//sei();
	return SPDR;		
}

void SPI_select(void){
	PORTB &= ~(1 << CAN_CS);
}

void SPI_deselect(void){
	PORTB |= (1 << CAN_CS);
}