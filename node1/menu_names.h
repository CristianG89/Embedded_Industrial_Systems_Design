#ifndef MENU_NAMES_H
#define MENU_NAMES_H

#include <avr/pgmspace.h>

/****************************************************************************
This file stores the strings for the menu names in PROGMEM in order to save some space in the internal RAM
****************************************************************************/
const char PROGMEM main_menu_string[] = "---Main Menu---";

const char PROGMEM game_string[] = "Game";

const char PROGMEM highscore_string[] = "Highscore";

const char PROGMEM extras_string[] = "Extras";

const char PROGMEM options_string[] = "Options";

const char PROGMEM screensaver_string[] = "Screensaver";

const char PROGMEM songs_string[] = "Songs";

const char PROGMEM paint_string[] = "Paint";

const char PROGMEM brightness_string[] = "Brightness";

const char PROGMEM low_string[] = "Low";

const char PROGMEM high_string[] = "High";


/****************************************************************************
Pointer table stored in PROGMEM
These are used in order to fetch data to working memory
****************************************************************************/
PGM_P const PROGMEM string_table[] = {		
	main_menu_string,					
	game_string,
	highscore_string,
	extras_string,
	options_string,
	screensaver_string,
	songs_string,
	paint_string,
	brightness_string,
	low_string,
	high_string
};

#endif // FONTS_H_ 