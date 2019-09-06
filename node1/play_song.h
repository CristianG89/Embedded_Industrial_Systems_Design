#ifndef PLAY_SONG_H
#define PLAY_SONG_H

#include <avr/pgmspace.h>

/****************************************************************************
Buzzer initialization
****************************************************************************/
void buzzer_init(void);		


/****************************************************************************
Buzzer activation
****************************************************************************/
void buzzer_on(void);		


/****************************************************************************
Buzzer deactivation
****************************************************************************/
void buzzer_off(void);		


/****************************************************************************
Reproduces a note
****************************************************************************/
void buzz(uint16_t frequency, uint8_t length);	


/****************************************************************************
Starting song
****************************************************************************/
void game_start_song(void);			


/****************************************************************************
Finishing song
****************************************************************************/
void game_finish_song(void);		


/****************************************************************************
Reproduce a song (in game)
****************************************************************************/
void game_song(uint8_t song_num);	


/****************************************************************************
Mario song
****************************************************************************/
void play_mario(void);				


/****************************************************************************
Adventure time song
****************************************************************************/
void play_adventure(void);			


/****************************************************************************
Reproduce a song (in menu)
****************************************************************************/
void play_song(uint8_t song_num);	


/****************************************************************************
Delay of ms with variable
****************************************************************************/
void my_delay_ms(uint16_t n);		


/****************************************************************************
Delay of us with variable
****************************************************************************/
void my_delay_us(uint16_t n);		


#endif 	//PLAY_SONG_H