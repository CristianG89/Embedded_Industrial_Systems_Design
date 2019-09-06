#ifndef HIGHSCORE_H
#define HIGHSCORE_H



/****************************************************************************
Initializing empty sram space by setting all addresses to 0x00 
****************************************************************************/
void highscore_sram_init();


/****************************************************************************
Strores given parameter word and corresponding score in sram in correct
position i highscoretable (fixed array of addresses). 
****************************************************************************/
void highscore_save_sram(char* word, uint8_t score);


/****************************************************************************
Checks if paramater score is bigger than score in highscoretable stored in
sram. Will return position to where the score (with name) should be stored   
****************************************************************************/
uint8_t highscore_check_sram(uint8_t score);


/****************************************************************************
Pretty print of the highscore table to oled 
****************************************************************************/
void highscore_display_sram();


/****************************************************************************
Creating name by selecting letters with right button and erase with left button
Saves in player name defined space
****************************************************************************/
void highscore_create_name();


/****************************************************************************
Return player name saved in sram
****************************************************************************/
char* highscore_get_player_name();


/****************************************************************************
Able to select letter with joystick. Starts on 'A'. Going down with joystick
you will selec 'B' then 'C' etc. Returns char - representing letter  
****************************************************************************/
char letter_select(void);


/****************************************************************************
Clears sram space where player name is saved
****************************************************************************/
void clear_name_in_sram();


#endif //HIGHSCORE_H
