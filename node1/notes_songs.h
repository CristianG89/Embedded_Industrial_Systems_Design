#ifndef NOTES_SONGS_H
#define NOTES_SONGS_H

#include <avr/pgmspace.h>

/**************************************************
**************** All musical notes ****************
**************************************************/
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951


/**************************************************
****************** Songs + Tempo ******************
**************************************************/
const uint16_t PROGMEM game_melody[2][4] = {
	{NOTE_E7,NOTE_E7,0,NOTE_E7},		//Start song
	{NOTE_C4,NOTE_C5,NOTE_A3,NOTE_A4},	//Finish song
};
const uint8_t PROGMEM game_tempo[2][4] = {
	{12,12,12,12},
	{12,12,12,12},
};


/**************************************************
****************** MARIO SONG  ********************
**************************************************/
const uint16_t PROGMEM mario_melody[] = {
  NOTE_E7,NOTE_E7,0,NOTE_E7,0,NOTE_C7,NOTE_E7,0,NOTE_G7,0,0,0,NOTE_G6,0,0,
  0,NOTE_C7,0,0,NOTE_G6,0,0,NOTE_E6,0,0,NOTE_A6,0,NOTE_B6,0,NOTE_AS6,
  NOTE_A6,0,NOTE_G6,NOTE_E7,NOTE_G7,NOTE_A7,0,NOTE_F7,NOTE_G7,0,NOTE_E7,0,NOTE_C7,NOTE_D7,NOTE_B6,
  0,0,NOTE_C7,0,0,NOTE_G6,0,0,NOTE_E6,0,0,NOTE_A6,0,NOTE_B6,0,
  NOTE_AS6,NOTE_A6,0,NOTE_G6,NOTE_E7,NOTE_G7,NOTE_A7,0,NOTE_F7,NOTE_G7,0,NOTE_E7,0,NOTE_C7,NOTE_D7,
  NOTE_B6,0,0
};


/**************************************************
****************** MARIO TEMPO  *******************
**************************************************/
const uint8_t PROGMEM mario_tempo[] = {
	12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
	12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
	12,12,9,9,9,12,12,12,12,12,12,12,12,12,12,12,
	12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
	12,12,9,9,9,12,12,12,12,12,12,12,12,12,12,12,12
};


/**************************************************
****************** ADVENTURE SONG  ****************
**************************************************/
const uint16_t PROGMEM adventure_melody[] = {
    NOTE_D5,NOTE_G5,NOTE_G5,NOTE_G5,NOTE_G5,NOTE_FS5,
    NOTE_FS5,NOTE_E5,NOTE_D5,NOTE_E5,NOTE_D5,NOTE_D5,
    NOTE_C5,NOTE_B5,NOTE_A5,NOTE_G4,0,NOTE_C5,NOTE_B5,
    NOTE_A5,NOTE_G4,0,NOTE_G5,0,NOTE_G5,NOTE_G5,0,NOTE_G5,
    NOTE_FS5,0,NOTE_E5,NOTE_E5,NOTE_D5,NOTE_D5,NOTE_C5,
    NOTE_C5,NOTE_C5,NOTE_D5,NOTE_D5,NOTE_A5, NOTE_B5,
    NOTE_A5,NOTE_G4,NOTE_G5
};


/**************************************************
****************** ADVENTURE TEMPO  ***************
**************************************************/
const uint8_t PROGMEM adventure_tempo[] = {
	24,24,12,12,12,24,12,24,24,24,12,24,12,12,12,
	12,24,12,24,24,12,24,24,24,24,12,24,12,24,24,
	24,12,12,24,8,24,24,8,8,24,12,24,24,12 
};


#endif 	//NOTES_SONGS_H