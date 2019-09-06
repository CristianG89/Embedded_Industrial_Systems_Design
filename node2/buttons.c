#include <avr/io.h> 	//Specific IO for AVR micro (all registers defined inside)
#include <stdio.h> 

#include "uart.h"		//Added to use fprintf function
#include "buttons.h"	//Function prototypes
#include "adc.h"

#define Mov23_Pos 190	//Joystick moved 2/3 in positive direction (up or right)
#define Mov23_Neg 64	//Joystick moved 2/3 in negative direction (down or left)

joy_direction_t direction = JOY_NEUTRAL;		//Initialization of joystick direciton	
slider_position_t slider_pos = {0, 0};			//Initialization of slider positions
joy_position_t joy_coord;

//Variables used for digital average filtering:
uint8_t index = 0;
uint8_t n_readings = 4;
uint8_t joyX_readings[4];
uint8_t joyY_readings[4];
uint16_t joyX_sum = 0;
uint16_t joyY_sum = 0;


void buttons_init(void) {
	DDRB |= (1 << DDB0);	//Pin PB0 enabled as output	(BUTTONS)
	DDRB |= (1 << DDB1);	//Pin PB1 enabled as output

	for (uint8_t i = 0; i < n_readings; i++){
		joyX_readings[i] = ADC_read(JOY_LR);	//Fetch initial joystick positions
		joyY_readings[i] = ADC_read(JOY_DU);
		joyX_sum += joyX_readings[i];
		joyY_sum += joyY_readings[i];
	}
	joy_coord.XX_init = joyX_readings[0];		//Update initial joystick positions
	joy_coord.YY_init = joyY_readings[0];
}


void buttons_update_joy_coord() {			
	joyX_sum -= joyX_readings[index];		//Make sure sum is total of only the 4 last measurements
	joyY_sum -= joyY_readings[index];

	joyX_readings[index] = ADC_read(JOY_LR);//Read raw data from joystick
	joyY_readings[index] = ADC_read(JOY_DU);

	joyX_sum += joyX_readings[index];		//Add measurement to total value
	joyY_sum += joyY_readings[index];

	index++;

	if (index >= n_readings){
		index = 0;
	}

	//Calculate and update joystick position with the average of the 4 last measurements
	uint8_t joyX_avg = joyX_sum / n_readings;	
	uint8_t joyY_avg = joyY_sum / n_readings;

	//Make sure coordinates dont change when joystick is centered
	if (joyX_avg != (joy_coord.XX_init + 1) && joyX_avg != (joy_coord.XX_init -1)){
		joy_coord.XX = joyX_avg;													
	}
	
	if (joyY_avg != (joy_coord.YY_init + 1) && joyY_avg != (joy_coord.YY_init -1)){
		joy_coord.YY = joyY_avg;
	}
}

joy_position_t buttons_get_joy_coord(){
	buttons_update_joy_coord();
	
	return joy_coord;
}

void buttons_update_slider_positions(){
	uint8_t slider_right_read = ADC_read(SLIDER_R);	
	uint8_t slider_left_read = ADC_read(SLIDER_L);

	//Too much high frequency noise on sliders => reduce resolution
	if (slider_left_read <= (slider_pos.left - 4) || slider_left_read >= (slider_pos.left + 4)){
		slider_pos.left = ADC_read(SLIDER_L);
	}
	
	if (slider_right_read <= (slider_pos.right - 4) || slider_right_read >= (slider_pos.right + 4)){
		slider_pos.right = ADC_read(SLIDER_R);
	}
}

slider_position_t buttons_get_slider_positions() {
	buttons_update_slider_positions(); 		//Make sure the newest positions are returned
	
	return slider_pos;
}

joy_direction_t buttons_get_joy_direction(int16_t X_coord, int16_t Y_coord) {	//Current joystick direction
	if (X_coord>Mov23_Pos) {			
		if (Y_coord>Mov23_Pos) {		
			if (X_coord>Y_coord) {direction = JOY_RIGHT;}	//The biggest number determines final position
			else {direction = JOY_UP;}
		} 
		else if (Y_coord<Mov23_Neg) {
			if (X_coord>2*Y_coord) {direction = JOY_RIGHT;}
			else {direction = JOY_DOWN;}
		} 
		else {direction = JOY_RIGHT;}
	}
	else if (X_coord<Mov23_Neg) {
		if (Y_coord>Mov23_Pos) {
			if (2*X_coord>Y_coord) {direction = JOY_LEFT;}
			else {direction = JOY_UP;}
		} 
		else if (Y_coord<Mov23_Neg) {
			if (X_coord>Y_coord) {direction = JOY_LEFT;}
			else {direction = JOY_DOWN;}
		} 
		else {direction = JOY_LEFT;}
	}
	else if (Y_coord>Mov23_Pos) {
			direction = JOY_UP;
		} 
	else if (Y_coord<Mov23_Neg) {
			direction = JOY_DOWN;
		}
	else {direction = JOY_NEUTRAL;}		

	return direction;
}