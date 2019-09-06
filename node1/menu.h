#ifndef MENU_H
#define MENU_H 


/****************************************************************************
Menus have IDs in order to retrieve their names from PROGMEM
See menu_names.h in order to see what is stored in PROGMEM for menu.c
****************************************************************************/
#define main_menu_id	0x00		
#define game_id			0x01		
#define highscore_id	0x02
#define extras_id		0x03
#define options_id		0x04
#define screensaver_id	0x05
#define songs_id		0x06
#define paint_id		0x07
#define brightness_id	0x08
#define low_id			0x09
#define high_id 		0x0A


/****************************************************************************
 Menu struct. Menu is build as dobbely linked list. All subemenus need to  
 point to a parent. 
****************************************************************************/
typedef struct{
	uint8_t menu_id;
	struct t_menu* parent;
	struct t_menu* children;
	struct t_menu* sibling;
} t_menu;


/****************************************************************************
 Loads menu names from PROGMEM into buffer for printing. Uses menu_id to 
 find correct menu string
****************************************************************************/
void menu_name_retrieve(uint8_t menu_id);


/****************************************************************************
Setting up all the menues and submenues in an linked list. Here you add more
menues to the menu system if necessary
****************************************************************************/
void menu_system();


/****************************************************************************
Pretty print of menu
****************************************************************************/
void print_menu(t_menu* menu);


/****************************************************************************
Menu initialization by clearing screen and setting up menusystem
****************************************************************************/
void menu_init();	//Menu initialization


/****************************************************************************
To manage to go between different menues and submenues by moving arrow on
oled
****************************************************************************/
void menu_move();	


/****************************************************************************
IF-statment-table for menu options. The statments corresponds to wich submenu 
is choosen. Here you add more functionality to each submenu if necessary
****************************************************************************/
void menu_handler(void);




#endif //MENU_H