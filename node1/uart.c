#include <avr/io.h> 	
#include <stdio.h>		
#include <stdint.h>		

#include "uart.h"		


void USART_init(unsigned int ubrr) {	
	UBRR0H = (unsigned char)(ubrr>>8);  		//MSB establish the Blaudrate of USART0
	UBRR0L = (unsigned char) ubrr; 				//LSB establish the Blaudrate of USART0 
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);				//Receiver (bit RXEN0) and Transmitter (bit TXEN0) of USART0 enabled (register UCSR0B)
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);//UCSZ00 is 3 to activate UCZ00 & UCSZ01 at a time 	
}


void USART_transmit(unsigned char data) {
	while (!(UCSR0A & (1<<UDRE0)));	//Wait for empty transmit buffer in USART0 (bit UDRE0 of register UCSR0A)
	
	UDR0 = data; 					//Put data into buffer
}


unsigned char USART_receive(void) {
	//while(!(UCSR0A & (1<<RXC0))); //Wait for data to be received

	return UDR0;
}