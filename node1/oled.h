#ifndef OLED_H
#define OLED_H

#include <stdio.h>

typedef enum tools{CIRCLE, RECTANGLE, BRUSH} tools_t;	//Types of paint tools


/****************************************************************************
To write a (configuration) command into the OLED
****************************************************************************/
void write_c(uint8_t cmd);		


/****************************************************************************
To print data into the OLED
****************************************************************************/
void write_d(uint8_t cmd);		


/****************************************************************************
Stores data in two-dimensional array that represents the OLED buffer
****************************************************************************/
void write_s(uint8_t data);


/****************************************************************************
//OLED display initialization
****************************************************************************/
void OLED_init(void);


/****************************************************************************
To print JUST one character
****************************************************************************/
void OLED_print_char(char c);	


/****************************************************************************
To print the whole word/sentence (pointer where char starts)
****************************************************************************/
void OLED_print_all(char* word);


/****************************************************************************
Extract each digit from passing parameter. And prints to oled
****************************************************************************/
void OLED_print_number(uint8_t number);


/****************************************************************************
It goes to position 0,0 in OLED
****************************************************************************/
void OLED_home(void);			


/****************************************************************************
To go to a specific line in page in OLED. 
****************************************************************************/
void OLED_goto_line(uint8_t page);		


/****************************************************************************
To go to a specific column in OLED
****************************************************************************/
void OLED_goto_column(uint8_t column);	


/****************************************************************************
Go to requested position by passing page and column as parameters
****************************************************************************/
void OLED_pos(uint8_t page, uint8_t column);	


/****************************************************************************
Clears the entire line/page wich is passed as parameter
****************************************************************************/
void OLED_clear_line(uint8_t line);	


/****************************************************************************
Clears the whole OLED
****************************************************************************/
void OLED_clear_all(void);			


/****************************************************************************
Define brightness level (requested format lvl from 00h to FFh)
****************************************************************************/
void OLED_set_brightness(uint8_t lvl);	


/****************************************************************************
Print an arrow in requested position
****************************************************************************/
void OLED_print_arrow(uint8_t row, uint8_t col); 


/****************************************************************************
Clear an arrow in requested position
****************************************************************************/
void OLED_clear_arrow(uint8_t row, uint8_t col); 


/****************************************************************************
Prints a complete screen saver
****************************************************************************/
void OLED_screen_Saver();	


/****************************************************************************
Loads SRAM content to OLED at frequency set by timer1 init
****************************************************************************/
void OLED_update();


/****************************************************************************
Fills OLED sram data (frame) with a given byte
****************************************************************************/
void OLED_frame_fill(uint8_t data);


/****************************************************************************
Fills frame with characters
****************************************************************************/
void OLED_frame_char_fill(char c);


/****************************************************************************
Draw pixel on oled. X and y are cartesian coordinates, not page and column
****************************************************************************/
void OLED_draw_pixel(uint8_t bit, uint8_t x, uint8_t y);


/****************************************************************************
Draw cross for cursor with X_0 and Y_0 as initial points
****************************************************************************/
void OLED_draw_cursor(uint8_t x0, uint8_t y0);



/****************************************************************************
Removes the cursor from its previous position
****************************************************************************/
void OLED_clear_cursor(uint8_t x0, uint8_t y0);


/****************************************************************************
Drawing rectangle on oled by passing init position X_0 and Y_0 and height Y_E
and length X_E
****************************************************************************/
void OLED_draw_rectangle(uint8_t x0, uint8_t xe, uint8_t y0, uint8_t ye);


/****************************************************************************
Draw circle using the midpoint X_0 and Y_0 with radius r and pixel point bit 
****************************************************************************/
void OLED_draw_circle(uint8_t bit, uint8_t x0, uint8_t y0, uint8_t r);


/****************************************************************************
Draw things onto the oled by pressing RIGHT BUTTON
****************************************************************************/
void OLED_paint();


/****************************************************************************
Handle cursor and its actions in paint function
****************************************************************************/
void OLED_cursor();


/****************************************************************************
//It opens a file to read/write. Necessary to use printf() (and similar functions) with the OLED.
//Otherwise one should write manually to OLED
****************************************************************************/
static FILE oled_out = FDEV_SETUP_STREAM (OLED_print_char, NULL, _FDEV_SETUP_WRITE);
#define OLED_p (&oled_out)

#endif //OLED_H