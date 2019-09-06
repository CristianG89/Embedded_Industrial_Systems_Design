#ifndef F_CPU
#define F_CPU 4915200	

#include <stdio.h>			
#include <avr/io.h> 
#include <stdlib.h>
#include <util/delay.h>			

#include "oled.h"	
#include "buttons.h"
#include "adc.h"
#include "highscore.h"		

int char_selector = 0;
int array_pos = 0;

volatile uint8_t *score_data = (uint8_t *) 0x1C00; 	//OLED Data start address

volatile uint8_t *score_name = (uint8_t *) 0x1C09; 	//OLED Data start address

#define name ( (uint8_t(*)[4]) (score_name) )		// Each name is 4 bytes long.

volatile char* player_name = (uint8_t *) 0x1C30;	// Name of current player (selected by writing it in with create_name())

uint8_t highscores_n = 0;							// Number of recorded scores

void highscore_sram_init(){
	for (uint8_t i=0; i <5; i++){
		score_data[i] = 0x00;
	}
	for (uint8_t j=0; j <50; j++){
		score_name[j] = 0x00;
	}
}

void highscore_save_sram(char* word, uint8_t score){
	uint8_t placement = highscore_check_sram(score);
	if(placement != 255){					// Score is not lower than all other scores
		for (uint8_t j=5; j>placement; j--){
			score_data[j] = score_data[j-1];
			for (uint8_t k=0; k<4; k++){
				name[j][k] = name[j-1][k];	// Name[player ranking][player name character(iterate 0-3)]
			}
		}

		score_data[placement] = score;
		for (uint8_t i=0; i<4; i++){
			name[placement][i] = word[i];
		}
	}

	highscores_n++;
}

uint8_t highscore_check_sram(uint8_t score){
	for (uint8_t i = 0; i < 5;i++){
		if (score > score_data[i]){
			
			return i;
		}
	}
	
	return 255;
}

void highscore_display_sram(){
	OLED_clear_all();
	OLED_home();
	OLED_pos(0,25);
	OLED_print_all("Highscores");
	for (uint8_t i=0; i<highscores_n; i++){
		OLED_pos(i+2,30);
		OLED_print_number(i+1);
		OLED_pos(i+2,40);
		for (uint8_t j=0; j<4; j++){
			OLED_print_char(name[i][j]);	
		}

		OLED_pos(i+2,80);
		OLED_print_number(score_data[i]);
	}
}

void highscore_create_name() {
	OLED_clear_all();
	OLED_update(); 
	clear_name_in_sram();
	char_selector = 0;
	array_pos = 0;
	OLED_pos(3,0);
	OLED_print_all("Name: ");
	_delay_ms(500);
	
	while(!(ADC_read(JOY_LR) >= 240 || ADC_read(JOY_LR) <= 10) ){  
		OLED_update(); 
		OLED_pos(3,40);
		OLED_print_all(player_name);
		OLED_pos(5,30);
		OLED_print_char(letter_select());
		_delay_ms(200);

		if(BUTTON_R && array_pos < 4) {
			player_name[array_pos] = letter_select();
			array_pos += 1;
		}
		
		if(BUTTON_L && array_pos >= 0) {
			player_name[array_pos] = ' ';
			array_pos -= 1;
			player_name[array_pos] = ' ';
			
			if (array_pos <= 0){
				array_pos = 0;
			}
		}
	}
}

char* highscore_get_player_name(){
	
	return player_name;
}

char letter_select(){
	char letter = 'A'; //starts select on A
	if (ADC_read(JOY_DU) >= 240) { //move UP
		if (char_selector <= 0){
			char_selector = 25;
			
			return(letter + char_selector);
		}
		
		char_selector -= 1;
	}
	
	if (ADC_read(JOY_DU) <= 10) { //move DOWN
		if (char_selector >= 25){
			char_selector = 0;
			
			return(letter + char_selector);
		}
		
		char_selector += 1;		
	}
	
	return(letter + char_selector);
}

void clear_name_in_sram(){
	for(int i = 0; i < 4; i++){
		player_name[i] = ' ';	
	}

}

#endif //F_CPU