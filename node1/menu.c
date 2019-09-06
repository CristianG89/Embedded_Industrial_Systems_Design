#ifndef F_CPU
#define F_CPU 4915200	

#include <util/delay.h>	
#include <stdlib.h>		
#include <stdio.h>		
#include <avr/io.h>		
#include <avr/pgmspace.h>	
#include <string.h>
#include <avr/interrupt.h>

#include "menu.h"		
#include "uart.h"		
#include "oled.h"		
#include "buttons.h"	
#include "adc.h"		
#include "can.h"		
#include "highscore.h"
#include "menu_names.h"

uint8_t pointerUP = 1;	//Arrow position (starts in 1, after title)
uint8_t pointerLR = 0;	//Menu level (menu or sub-menu)

t_menu* current_menu;   //menu the cursor is pointing at all the time

int displayed_lines = 0;
int currsor_io = 0;


//All menu options
t_menu main_menu;
t_menu game;
t_menu highscore;
t_menu extras;
t_menu options;
t_menu screensaver;
t_menu songs;
t_menu paint;
t_menu brightness;
t_menu low;
t_menu high;

char* retrieved_name_buffer[16];			// Buffer for retrieving menu names


void menu_name_retrieve(uint8_t menu_id){	

	strcpy_P(retrieved_name_buffer,(PGM_P)pgm_read_word(&(string_table[menu_id])));
	
	return retrieved_name_buffer;
} 


void menu_system() {
	//Main menu page create
	main_menu = (t_menu){main_menu_id, NULL, &game, NULL}; 		// Menu names are stored in PROGMEM
	game = (t_menu){game_id, &main_menu, NULL, &highscore};			// Use pointers to retireve them
	highscore = (t_menu){highscore_id, &main_menu, NULL, &extras};
	extras = (t_menu){extras_id, &main_menu, &screensaver, &options};
	options = (t_menu){options_id, &main_menu, &brightness, NULL};

	//Extras page create
	screensaver = (t_menu){screensaver_id, &extras, NULL, &songs};
	songs = (t_menu){songs_id, &extras, NULL, &paint};
	paint = (t_menu){paint_id, &extras, NULL, NULL};

	//Options page create
	brightness = (t_menu){brightness_id, &options, &high, NULL};
	high = (t_menu){high_id, &brightness, NULL, &low};
	low = (t_menu){low_id, &brightness, NULL, NULL};
	
	current_menu = &main_menu;
	menu_name_retrieve(main_menu_id);
	print_menu(current_menu);
}


void print_menu(t_menu* menu){
	OLED_clear_all();       		
	displayed_lines = 0;
	menu_name_retrieve(menu->menu_id);			
	fprintf(OLED_p, retrieved_name_buffer, 0);
	uint8_t line = 1;

	menu = menu->children;
	while(menu && (line < 5)) {
		menu_name_retrieve(menu->menu_id);
		displayed_lines++;
		OLED_pos(line,20);
		fprintf(OLED_p, retrieved_name_buffer, 0);
		line++;
		menu = menu->sibling;
	}
	OLED_home();
}

void menu_init(){
	OLED_clear_all();
	menu_system();
}


void menu_move() {			
	OLED_pos(pointerUP, 5);				//currsor_io = 1/0 ->arrow off/on
	if (currsor_io == 0){
		OLED_print_arrow(pointerUP, 5);	
	}

	if (ADC_read(JOY_DU) >= 240) {		//If joystick is moved UP
		OLED_clear_arrow(pointerUP, 5);	
		pointerUP--;					
		if (pointerUP < 1) {
			pointerUP = displayed_lines-1;	
		}
	}
	else if (ADC_read(JOY_DU) <= 10) {	//If joystick is moved DOWN
		OLED_clear_arrow(pointerUP, 5);
		pointerUP++;
		if (pointerUP > displayed_lines) {
			pointerUP = 1;
		}
	}
	else if (ADC_read(JOY_LR) >= 240) {	//If joystick is moved RIGHT
		if (current_menu->children == NULL){
			
			return;
		}
		else{
			OLED_home();
			current_menu = current_menu->children;
			for (int i = 0; i < pointerUP - 1; i++) {
				current_menu = current_menu->sibling;
			}
			
			print_menu(current_menu);
			pointerLR = pointerUP;		
			pointerUP = 1;				
			menu_handler();
		}
		
	}
	else if (ADC_read(JOY_LR) <= 10) {	//If joystick is moved LEFT
		if (pointerLR != 0) {			
			OLED_clear_all();
			pointerLR = 0;				
			pointerUP = 1;				
		}
		
		if (current_menu->parent == NULL){
			
			return;
		}
			
		else{
			currsor_io = 0;
			current_menu = current_menu->parent;
			print_menu(current_menu);
		}
	}
	_delay_ms(100);
}


void menu_handler(void){				
	if(current_menu == &screensaver){
		OLED_clear_all();
		currsor_io = 1;
		OLED_screen_Saver();
	}
	else if (current_menu == &paint){
		OLED_clear_all();
		currsor_io = 1;
		OLED_paint();
	} 
	else if (current_menu == &game){
		OLED_clear_all();
		highscore_create_name();
		if((ADC_read(JOY_LR) <= 60)){
			currsor_io = 1;
			
			return;
		} 
		game_node1_play();
		current_menu = current_menu->parent;
		print_menu(current_menu);
	}
	else if (current_menu == &highscore){
		OLED_clear_all();
		currsor_io = 1;
		highscore_display_sram();
	}
	else if (current_menu == &low){
		OLED_clear_all();
		currsor_io = 0;
		OLED_set_brightness(0x00);
		current_menu = current_menu->parent;
		print_menu(current_menu);
	}
	else if (current_menu == &high){
		OLED_clear_all();
		currsor_io = 0;
		OLED_set_brightness(0xFF);
		current_menu = current_menu->parent;
		print_menu(current_menu);
	}
	else if (current_menu == &songs){
		//play_mario();			// mario song
		play_adventure();		//It is called Adventure time!!
		currsor_io = 0;
		current_menu = current_menu->parent;
		print_menu(current_menu);
	}
}


////////////////////////////////////////////////////////////

/*
void ext_INT0_ON(void) {
	DDRD &= ~(1 << PD2); 	//PD2 set as input (1=output, 0=input). Leave others unchanged

	MCUCR |= (1<<ISC01) | (1<<ISC00);	//Rising edge activation for INT0
	GICR |= (1<<INT0);					//External interrupt 0 enabled (pin PD2)
}

void ext_INT0_OFF(void) {
	DDRD &= ~(1 << PD2); 	//disable pin PD2
	GICR &= ~(1<<INT0);		//External interrupt 0 disabled
}

ISR(EXT_INT0_vect) {	
	;
}*/

#endif //F_CPU*/