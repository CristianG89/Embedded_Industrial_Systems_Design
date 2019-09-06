#ifndef F_CPU
#define F_CPU 16000000	

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>	

#include "motor.h"
#include "TWI_Master.h"
#include "uart.h"
#include "buttons.h"

// Digital-analog converter address
#define DAC 0x28

// Sample TWI transmission commands
#define TWI_CMD_MASTER_WRITE 0b0
 
motor_dir_t dir = IDLE;
uint8_t message_buffer[3];	//
uint16_t motor_encoder_max = 0;


void motor_init() {
	DDRH |= (1 << PH4); 	//Enable EN pin
	PORTH |= (1 << PH4);	//Set EN pin high to enable motor

	DDRH |= (1 << PH1); 	///Enable DIR pin (direction) 

	DDRH |= (1 << PH5); 	//Enable !OE to output of encoder
	DDRH |= (1 << PH3); 	//Enable SEL to output
	DDRH |= (1 << PH6);		//Enable encoder !RTS pin

	motor_move(0);
	motor_reset_encoder();
}

void motor_calibr_encoder() {	//Left=0	Right=Max_encoder
	motor_move(-100);
	_delay_ms(1500);
	motor_reset_encoder();
	motor_move(0);
	motor_move(100);
	_delay_ms(1500);
	motor_encoder_max = -motor_read_encoder();
}

void motor_reset_encoder(){					//Toggles RST pin on MJ1
	PORTH &= ~(1 << PH6);					//Clear RST pin
	_delay_ms(20);
	PORTH |= (1 << PH6);					//Set RST pin to high
}

int16_t motor_read_encoder() {				//Normal procedure of reading the encoder:
   PORTH &= ~(1 << PH5);		  			// Set !OE low to enable output of encoder
   PORTH &= ~(1 << PH3);		   			// Lower SEL to get high byte
   _delay_ms(20);			  	  			// Wait about 20 microseconds
   uint8_t MSB = PINK;						// Read MSB
   PORTH |= (1 << PH3);						// Set SEL high to get low byte
   _delay_ms(20);			   				// Wait about 20 microseconds
   uint8_t LSB = PINK;			   			// Read LSB
   //motor_reset_encoder();		   			// Toggle !RST to reset encoder
   PORTH |= (1 << PH5);           			// Set !OE high to disable output of encoder
   int16_t encoder_pos = (MSB << 8) + LSB; 	//Process received data.... 

   return encoder_pos;
}

uint16_t motor_get_encoder_max() {
	return motor_encoder_max;
}

void motor_set_direction(joy_direction_t direction) {	// Take in direction from joystick and set motor direction
	switch(direction) {
		case LEFT:
			PORTH &= ~(1 << PH1); //Set direction left  
			break;

		case RIGHT:
			PORTH |= (1 << PH1); //Set direction right
			break;

		default:
			break;
	}
}

void motor_move(int16_t speed) {
	if(speed > 0) {			// Speed value for left direction comes positive
		dir = RIGHT;
	}
	else if (speed < 0) {
		dir = LEFT;
		speed = -speed;		// Speed value for left direction comes negative
	}
	else {
		dir = IDLE;
		//motor_set_speed(0x00);	
	}
	speed = (uint8_t)speed;
	motor_set_direction(dir);
	_delay_ms(1);
	motor_set_speed(speed);
}

void motor_set_speed(uint8_t speed){
	uint8_t adr = 0x50;
	uint8_t cmd = 0x00;
	message_buffer[0] = adr;							// First call consists of TWI slave address
	message_buffer[1] = cmd;							// First byte is write command
	message_buffer[2] = speed;							// Send desired motor speed to DAC
	TWI_start_transceiver_with_data(message_buffer, 3);	// start transmission
}

#endif