#ifndef F_CPU
#define F_CPU 16000000	

#include <util/delay.h>	
#include <stdio.h>		
#include <avr/io.h> 	
#include <avr/interrupt.h>	

#include "led.h"
#include "uart.h"
#include "adc.h"
#include "buttons.h"
#include "spi.h"
#include "MCP2515.h"
#include "can.h"
#include "PWM.h"
#include "servo.h"
#include "TWI_Master.h"
#include "motor.h"
#include "pid.h"
#include "solenoid.h"
#include "IR.h"
#include "game_node2.h"

int main() {
	cli();

	USART_init(MYUBRR);
	//USART_transmit(USART_receive());	//To make printf() working in USART if not working check usart_Receive() ->while loop
	fprintf(UART_p, "INIT_N2\n\r", 0);

	SPI_init();
	CAN_init();
	TWI_master_init();
	servo_init();			//(connected in PB5)
	ADC_init();
	motor_init();
	solenoid_init();
	IR_init();

	packet can_joystick = {.id=0x12, .length=0x02, .data={0x01,0x02}};

	sei();			// Enable all interrupts

	while(1) {
		
		packet CAN_recieved = CAN_read();

		//fprintf(UART_p, "%d\r\n", CAN_recieved.id);
		fprintf(UART_p, "IR: %d \r\n", ADC_read());

		fprintf(UART_p, "%d\r\n", CAN_recieved.id);

		_delay_ms(200);

		if (CAN_recieved.id == CAN_START_GAME_ID) {		// Can also maybe be moved into an interrupt

			game_node2_play();
		}



		//----------------------------------------------------------------------------------
		//TEST FUNCTIONS UNDER
		

		//game_can_test();
		//fprintf(UART_p, "working \r\n", 0);

		//fprintf(UART_p, "%d\n\r", motor_encoder_max); 
		//_delay_ms(500);
		
		/*fprintf(UART_p, "TCNT0: %4d ", TCNT0);
		if (int_tim8 == 1) {
			int_tim8 = 0;
			fprintf(UART_p, "TIMER!!!!!!!! \r\n", 0);
		}
		_delay_ms(50);*/

		/*packet can_joystick = CAN_read();
		fprintf(UART_p, "JoyX: %4d ", can_joystick.data[0]);
		fprintf(UART_p, "JoyY: %4d \r\n", can_joystick.data[1]);
		fprintf(UART_p, "IR: %4d \r\n", ADC_read());
		move_servo(can_joystick.data[0]);	//Change Servo direction
		int16_t motor_pos = -motor_read_encoder();
		uint8_t setpoint = can_joystick.data[0];
		motor_move(pid_controller(&pidData, setpoint, motor_pos));*/
	}
	return 0;
}
#endif

/* 	dmesg --follow
	lsusb
	ttyACM0 = USB
*/