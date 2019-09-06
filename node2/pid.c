#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "stdint.h"

#include "pid.h"
#include "uart.h"
#include "PWM.h"
#include "motor.h"

#define KP	0.75
#define KI	0.10
#define KD	0.02
#define SCALING_FACTOR  128		// OPTIMIZED TO 128
#define BYTE_RANGE  36			// max_encoder/255

uint8_t int_timer = 0;

int16_t error = 0;
int16_t p_term = 0;
int16_t d_term = 0;
int32_t i_term = 0;
int32_t output = 0;
int32_t totalError = 0;

void pid_init(pidData_t *pid, uint16_t frequency) {
	PWM_PE3_init(256, frequency);
	
	// Start values for PID controller
	pid->sumError = 0;
	pid->lastProcessValue = 0;
	// Tuning constants for PID loop
	pid->P_Factor = KP * SCALING_FACTOR;
	pid->I_Factor = KI * SCALING_FACTOR;
	pid->D_Factor = KD * SCALING_FACTOR;
	// Limits to avoid overflow
	pid->maxError = MAX_INT / (pid->P_Factor + 1);
	pid->maxSumError = MAX_I_TERM / (pid->I_Factor + 1);
}

int16_t pid_controller(pidData_t *pid_st, uint8_t setPoint, int16_t processValue) {
	if (int_timer == 1) {
		int_timer = 0;

		error = setPoint - processValue/BYTE_RANGE;	//Both values between 0 and 255

		if((error < 10) && (error > -10)) {
			pid_st->sumError = 0;
		}

		// Calculate Pterm and limit error overflow
		if (error > pid_st->maxError) {
			p_term = MAX_INT;
		}
		else if (error < -pid_st->maxError) {
			p_term = -MAX_INT;
		}
		else {
			p_term = (pid_st->P_Factor + (3/error)) * error;
		}	
		
		// Calculate Iterm and limit integral runaway
		totalError = pid_st->sumError + error;
		if(totalError > pid_st->maxSumError) {
			i_term = MAX_I_TERM;
			pid_st->sumError = pid_st->maxSumError;
		}
		else if(totalError < -pid_st->maxSumError) {
			i_term = -MAX_I_TERM;
			pid_st->sumError = -pid_st->maxSumError;
		}
		else {
			pid_st->sumError = totalError;
			i_term = pid_st->I_Factor * pid_st->sumError;
		}

		// Calculate Dterm
		d_term = pid_st->D_Factor * (pid_st->lastProcessValue - processValue);
		pid_st->lastProcessValue = processValue;

		output = ((p_term + i_term + d_term) / SCALING_FACTOR);

		if(output > 120) {
			output = 120;
		}
		else if(output < -120) {
			output = -120;
		}
		else if ((output < 55) && (output > 20)) {
			output = 55;
		}
		else if ((output > -55) && (output < -20)) {
			output = -55;
		}

		//fprintf(UART_p, ", output %4d", output);
		//fprintf(UART_p, ", setPoint %4d", setPoint);
		//fprintf(UART_p, ", processValue %4d \r\n", processValue/BYTE_RANGE);


	}
	
	return((int16_t)output);
}

ISR(TIMER3_COMPA_vect) {	
	int_timer = 1;	//Global variable for internal timer interruption
}