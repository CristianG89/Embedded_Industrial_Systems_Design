#ifndef F_CPU
#define F_CPU 4915200	

#include <stdio.h>			
#include <avr/io.h> 		
#include <avr/interrupt.h>	
#include <avr/pgmspace.h>	

#include "oled.h"			
#include <util/delay.h>		
#include "fonts.h"			
#include "sram.h"			
#include "buttons.h"
#include "timer.c"

#define MAX_COLUMNS 128		//Number of columns in OLED
#define MAX_PAGES 8			//Width of used fonts
#define MAX_ROWS 64

#define FONTWIDTH 8			//Width of used fonts


volatile uint8_t *oled_cmd = (uint8_t *) 0x1000; 	//OLED Command start address
volatile uint8_t *oled_data = (uint8_t *) 0x1200; 	//OLED Data start address

static uint8_t gen_page, gen_col = 0;

volatile uint8_t *frame_addr = (uint8_t*)0x1800;

#define frame ( (uint8_t(*)[128]) (frame_addr) )	// Two dimensional array representation of OLED screen in SRAM

uint8_t timer1_flag = 0; 			// Timer flag for updating OLED


// Paint variables
tools_t paint_tool = BRUSH;			// Initial tool is a brush
uint8_t cursor_x = 64;				// Cursor is initially in center of screen
uint8_t cursor_y = 32;
uint8_t cursor_x_prev = 64;			// Previous cursor position
uint8_t cursor_y_prev = 32;
uint8_t clear_bits_x = 0xFF;		// Make note of which bits should be cleared after the cursor has moved
uint8_t clear_bits_y = 0xFF;		// Do NOT clear a bit if something else was drawn there beforehand


void write_c(uint8_t cmd) {			
	*oled_cmd = cmd;
}


void write_d(uint8_t cmd) {			
	*oled_data = cmd;
}


void write_s(uint8_t data){			
	frame[gen_page][gen_col] = data;
	gen_col++;						// SRAM pointer behaves like OLED pointer in page mode
	if(gen_col >= 128){
		gen_col = 0;			
	}
}


void OLED_init(void) {	
	write_c(0xae);	//Display is switch OFF
	write_c(0xa1);	//Segment remap (A1=Aligned to the right / A0=Aligned to the left)
	write_c(0xda);	//Sets COM signals pin configuration to match OLED panel hardware layout
	write_c(0x12);	//Value. Alternative
	write_c(0xc8);	//Sets the scan direction of the COM output: COM63~COM0
	write_c(0xa8);	//Switches the default multiplex mode (63) to any multiplex ratio
	write_c(0x3f);  //Value. From 0x10 (16) to 0x3f (63)
	write_c(0xd5);	//Display divide ratio/osc & freq. mode 
	write_c(0x80);	//Value. [7:4]-> Oscillator freq , [3:0]->Display clock
	write_c(0x81);	//Contrast control
	write_c(0x50);	//Value. From 00h (0%) to FFh (100%)
	write_c(0xd9);	//Set Pre-charge period
	write_c(0x21);	//Value. The interval is counted in number of DCLK, where RESET equals 2 DCLKs
	write_c(0x20);	//Set Memory Addressing Mode (AM)
	write_c(0x02);	//Value. 02h->Page AM, 01h->Vertical AM, 00h->Horizontal AM
	write_c(0xdb);	//COM signal deselected voltage level. Adjusts the VCOMH regulator output.
	write_c(0x30);	//Value. 00h->~0.65xVcc, 20h->~0.77xVcc(RESET), 30h->~0.83xVcc
	write_c(0xad);	//Select internal Iref or external Iref to supply the system
	write_c(0x00);	//Value. 00h->External Iref, 01h->Internal Iref
	write_c(0xa4);	//Display outputs according to the GDDRAM contents
	write_c(0xa6);	//A6h->Normal (data 1/0-> pixel ON/OFF), A7h->Inverse (data 1/0-> pixel OFF/ON)
	write_c(0xaf);	//Display is switched ON

	write_c(0xB0);	//Set page Start Address for page Addressing Mode
	write_c(0x00);  //Set Lower Column Start Address for page Addressing Mode
	write_c(0x10);	//Set Higher Column Start Address for page Addressing Mode (NEED TO ASK ABOUT THIS)
	
	OLED_clear_all();//All screen is cleared
	OLED_home();	//Pointer goes to the home position

	timer1_init(256, 60); // Initialize a timer for creating a 60 Hz refresh rate for the OLED
}


void OLED_print_char(char ch) {		
	for(uint8_t i = 0; i < FONTWIDTH; i++){
		//In ASCII, space or ' ' is 32. So taking the char and subtracting it with ' '
		//will give us the "correct" symbol from fonts.h 
		write_s(pgm_read_byte(&(font8[ch - ' '][i])));	//pgm_read_byte(&()) is required to take data from FLASH mem
	}
}


void OLED_print_all(char* word) {	
	uint8_t i = 0;
	while(word[i] != '\0') {		//If the received character is different from NULL (\0),
		OLED_print_char(word[i]);	//such character is printed on the OLED
		i++;
	}
}


void OLED_print_number(uint8_t number){
	uint8_t new_number = number;
	while (new_number > 0){
		if(new_number > 99){	
			uint8_t digit = (number/100) % 10;	
			OLED_print_char('0' + digit);		
			new_number /= 10;
		}
		else if(new_number > 9){	
			uint8_t digit = (number/10) % 10;	
			OLED_print_char('0' + digit);		
			new_number /= 10;			
		}
		else{
			uint8_t digit = number % 10;		
			OLED_print_char('0' + digit);		
			new_number /= 10;					
		}
	}
}


void OLED_home(void) {	
	gen_page = 0;
	gen_col = 0;
	OLED_pos(0, 0);
}


void OLED_goto_line(uint8_t line) {	
	gen_page = line;
	gen_col = 0;
	if (line < MAX_PAGES) {
		write_c(0xB0 + line);	//Set line/page 0-7 (requested format B0h to B7h)
		write_c(0x00);			//Lower nibble of start column address (00h)	
		write_c(0x10);			//Upper nibble of start column address (10h)
	}
}


void OLED_goto_column(uint8_t column) {	
	gen_col = column;
	if (column < MAX_COLUMNS) {			
		uint8_t lower_nibble = (0x0F & column); 				//Set first 4 bits of column address
		uint8_t upper_nibble = 0x10 + (0x0F & (column >> 4));	//Set last 4 bits of column address	
		write_c(lower_nibble); 									//Lower nibble of requested column (requested format 00h to 0Fh)
		write_c(upper_nibble);									//Upper nibble of requested column (requested format 10h to 1Fh)
	}
}


void OLED_pos(uint8_t line, uint8_t column) {	
	gen_page = line;
	gen_col = column;
	OLED_goto_line(line);
	OLED_goto_column(column);	
}


void OLED_clear_line(uint8_t line) {	
	OLED_goto_line(line);
	for (uint8_t i=0; i<MAX_COLUMNS; i++) {
		write_s(0x00);
	}
}


void OLED_clear_all(void) {				
	for(int p=0; p<MAX_PAGES; p++) {	
		OLED_clear_line(p);
	}
	OLED_home();
}


void OLED_set_brightness(uint8_t lvl) {	
	write_c(0x81);		//Set contrast level
	write_c(lvl);		//to the requested level (requested format from 00h to FFh)
}


void OLED_print_arrow(uint8_t row, uint8_t col) {
	OLED_pos(row, col);		
	write_s(0b00011000);
	write_s(0b00011000);
	write_s(0b01111110);
	write_s(0b00111100);
	write_s(0b00011000);
}


void OLED_clear_arrow(uint8_t row, uint8_t col){
	OLED_pos(row, col);		
	write_s(0b00000000);
	write_s(0b00000000);
	write_s(0b00000000);
	write_s(0b00000000);
	write_s(0b00000000);
}


void OLED_screen_Saver() {	
	OLED_clear_all();
	for(uint8_t p=0; p<MAX_PAGES; p++) { 
		gen_page = p;
		for(uint8_t c=0; c<MAX_COLUMNS; c++) {
			write_s(pgm_read_byte(&(screenSaver[p][c])));
		}
	}
}


void OLED_update(){						
	if (timer1_flag == 1){
		for (uint8_t r = 0; r < 8; r++){
			OLED_goto_line(r);
			for (uint8_t c = 0; c < 128; c++){
				write_d(frame[r][c]);
			}
		}
		timer1_flag = 0;
	}
}


void OLED_frame_fill(uint8_t data){		
	for (uint8_t r = 0; r < 8; r++){
		for (uint8_t c = 0; c < 128; c++){
			frame[r][c] = data;
		}
	}
}


void OLED_frame_char_fill(char c){		
	for (uint8_t r = 0; r < 8; r++){
		OLED_goto_line(r);
		OLED_print_all(c);
	}
}


void OLED_draw_pixel(uint8_t bit, uint8_t x, uint8_t y){	
	if (x < MAX_COLUMNS && y < MAX_ROWS){
			uint8_t page = y / 8;					// Rounds down to find page
			uint8_t setbit = y % 8;					// Finds which bit in a column it should set to 1
			if (bit){
				frame[page][x] |= (1 << setbit);	// Only set bit in correct position to 1
			} else {
				frame[page][x] &= ~(1 << setbit);	// Only set bit in correct position to 0
			}
	}
}


void OLED_draw_cursor(uint8_t x0, uint8_t y0){	
	uint8_t page = y0 / 8;								
	uint8_t setbit = y0 % 8;					

	for (uint8_t x = x0-3; x <= (x0 + 3); x++){	// Draw horizontal line
		if (!(frame[page][x] & (1 << setbit))){	
			clear_bits_x |= (1 << (x - x0 + 3));// Bit should be cleared later if nothing was drawn there beforehans (except cursor)
		} else {
			clear_bits_x &= ~(1 << (x - x0 + 3));
		}
		OLED_draw_pixel(1, x, y0);
	}

	for (uint8_t y = y0-3; y <= (y0 + 3); y++){	// Draw vertical line
		page = y / 8;							
		setbit = y % 8;
		if (!(frame[page][x0] & (1 << setbit))){
			clear_bits_y |= (1 << (y - y0 + 3));
		} else {
			clear_bits_y &= ~(1 << (y - y0 + 3));
		}
		OLED_draw_pixel(1, x0, y);
	}
}


void OLED_clear_cursor(uint8_t x0, uint8_t y0){		
	for (uint8_t x = x0-3; x <= (x0 + 3); x++){	// Uses the clear_bits_x/y byte to decide which pixels to set to 0 again
		if (clear_bits_x & (1<< (x - x0 + 3))){	// Only draws black pixels on bits that were set by cursor
			OLED_draw_pixel(0, x, y0);
		}
	}
	for (uint8_t y = y0-3; y <= (y0 + 3); y++){				
		if (clear_bits_y & (1 << (y - y0 + 3))){
			OLED_draw_pixel(0, x0, y);
		}
	}
}

void OLED_draw_rectangle(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height){
	for (uint8_t x = x0; x <= (x0 +width); x++){	// Vertical lines
		OLED_draw_pixel(1, x, y0);
		OLED_draw_pixel(1, x, (y0 + height));
	}
	for (uint8_t y = y0; y <= (y0 +height); y++){	// Horizontal lines
		OLED_draw_pixel(1, x0, y);
		OLED_draw_pixel(1, x0 + width, y);
	}
}


void OLED_draw_circle(uint8_t bit, uint8_t x0, uint8_t y0, uint8_t r){	
	int x = r - 1;											
	int y = 0;											
	int dx = 1;
	int dy = 1;
	int rad_err = dx - (r << 1);

	while(x >= y){								// Make use of 8-symmetry of circle:
		OLED_draw_pixel(bit, x0 + x, y0 + y);	// Octant 1
        OLED_draw_pixel(bit, x0 + y, y0 + x);	// Octant 2
        OLED_draw_pixel(bit, x0 - y, y0 + x);	// Octant 3
        OLED_draw_pixel(bit, x0 - x, y0 + y);	// And so on...
        OLED_draw_pixel(bit, x0 - x, y0 - y);
        OLED_draw_pixel(bit, x0 - y, y0 - x);
        OLED_draw_pixel(bit, x0 + y, y0 - x);
        OLED_draw_pixel(bit, x0 + x, y0 - y);

         if (rad_err <= 0)						// Calculate tolerances
        {
            y++;
            rad_err += dy;
            dy += 2;
        }
        if (rad_err > 0)
        {
            x--;
            dx += 2;
            rad_err += dx - (r << 1);
        }
	}
}


void OLED_paint(){				
	OLED_clear_all();
	// Draw Circle option
	OLED_draw_rectangle(1, 1, 20, 20);
	OLED_draw_circle(1,11,11,7);

	// Draw Rectangle option
	OLED_draw_rectangle(1, 21, 20, 20);
	OLED_draw_rectangle(5,25,12,12);

	// Draw brush option
	OLED_draw_rectangle(1, 41, 20, 20);
	OLED_draw_circle(1, 11, 51, 2);
	OLED_draw_circle(1, 11, 51, 1);

	uint8_t keep_painting = 0x01;

	while(keep_painting){
		if (BUTTON_L){				
			keep_painting = 0x00;
		}
		OLED_cursor();				
		OLED_update();
	}
}


void OLED_cursor(){
	cursor_x_prev = cursor_x;
	cursor_y_prev = cursor_y;

	joy_position_t joy_pos = buttons_get_joy_coord();
	slider_position_t sliders = buttons_get_slider_positions();

	if (joy_pos.XX >= (joy_pos.XX_init + 5) && cursor_x <= MAX_COLUMNS-1){	// Joystick moves right
		cursor_x++;
	}
	if (joy_pos.XX <= (joy_pos.XX_init -40) && cursor_x >= 3){				// Joystick moves left
		cursor_x--;
	}
	if (joy_pos.YY >= (joy_pos.YY_init + 10) && cursor_y >= 1){				// Joystick moves up
		cursor_y--;
	}
	if (joy_pos.YY <= (joy_pos.YY_init -30) && cursor_y <= 63){				// Joystick moves down
		cursor_y++;
	}

	
	OLED_clear_cursor(cursor_x_prev, cursor_y_prev);	// Remove cursor from previous position
	OLED_draw_cursor(cursor_x, cursor_y);

	if(BUTTON_R){
		if(cursor_x < 21){								// Tool selection if cursor is inside the left panel
			if(cursor_y < 21){	
				paint_tool = CIRCLE;
			} else if(cursor_y < 41){
				paint_tool = RECTANGLE;
			} else if(cursor_y < 61){
				paint_tool = BRUSH;
			}
		}
		if(cursor_x > 22){								// Draw with tools if the cursor is inside drawing area
			switch (paint_tool){			
				case BRUSH:
					OLED_draw_circle(1, cursor_x, cursor_y, 2);
					for (uint8_t cx = 2; cx <= 4; cx++){
						clear_bits_x &= ~(1 << cx);		// Make sure the dot is not cleared by cursor remove function
						clear_bits_y &= ~(1 << cx);
					}
					break;
				case CIRCLE:
					OLED_draw_circle(1, cursor_x, cursor_y, sliders.right/5);
					break;
				case RECTANGLE:
					OLED_draw_rectangle(cursor_x, cursor_y, sliders.right/5, sliders.left/5);
					for (uint8_t cx = 3; cx <= 6; cx++){
						clear_bits_x &= ~(1 << cx);		// Make sure corner of rectangle is not cleared by cursor remove function
						clear_bits_y &= ~(1 << cx);
					}
					break;
			}
		}
	}
}


// 60 Hz refresh rate for OLED
ISR(TIMER1_COMPA_vect) {	
	timer1_flag = 1;
}


#endif