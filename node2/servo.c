#include <stdio.h> 
	
#include "PWM.h"
#include "servo.h"

#define MAX_Width  100 	//131	//2.1ms at 50Hz -> Changed for better calibration
#define IDLE_Width 75 	//94	//1.5ms at 50Hz -> Changed for better calibration
#define MIN_Width 56	//0.9ms at 50Hz

uint8_t servo_dir = 0;

void servo_init() {
	PWM_PB5_init(256, 50);		//PWM init. Prescaler N=256, and Frequency 50Hz
	move_servo(IDLE_Width);		//Servo is initialized in the middle position
}

void move_servo(uint8_t width) {
	servo_dir = 0.175*width + MIN_Width;	//Linear relation
	//0.175 = (MAX_Width - MIN_Width) * 255
	
	//MAX and MIN limited for security!
	if (servo_dir > MAX_Width) {servo_dir = MAX_Width;}
	if (servo_dir < MIN_Width) {servo_dir = MIN_Width;}

	set_PWMwidth("PB5", servo_dir);
}