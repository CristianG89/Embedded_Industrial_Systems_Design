#ifndef F_CPU
#define F_CPU 4915200	

#include <util/delay.h>	
#include <stdio.h>		 
#include <avr/io.h> 	
#include <avr/pgmspace.h>	

#include "uart.h"			
#include "notes_songs.h"	

uint16_t delayValue = 0;
uint16_t numCycles = 0;


uint8_t size1 = 0;
uint8_t noteDuration1 = 0;
uint16_t pauseBetweenNotes1 = 0;


uint8_t size2 = 0;
uint8_t noteDuration2 = 0;
uint16_t pauseBetweenNotes2 = 0;


void buzzer_init() {
	DDRE |= (1 << DDE2);	//Pin 2 output
	PORTE &= ~(1 << PE2);	//Clear pin 2 
}

void buzzer_on() {
	PORTE |= (1 << PE2);	//Set pin 2
}

void buzzer_off() {
	PORTE &= ~(1 << PE2);	//Clear pin 2 
}

void buzz(uint16_t frequency, uint8_t length) {
	delayValue = 1000000 / frequency / 2; 	// delay value between transitions; 1 second's worth of microseconds, 
											//divided by the frequency, split in half since there are two phases to each cycle
	
	numCycles = (frequency * length) / 1000; 	// number of cycles for proper timing multiply frequency (cycles per second), 
												// by the number of seconds to get the total number of cycles to produce

	for (uint16_t i=0; i<numCycles; i++) { 	// for the calculated length of time...
		buzzer_on(); 						// write the buzzer pin high to push out the diaphram
		my_delay_us(delayValue);			// wait for the calculated delay value
		buzzer_off(); 						// write the buzzer pin low to pull back the diaphram
		my_delay_us(delayValue);			// wait again or the calculated delay value
	}
}


void game_song(uint8_t song_num) {		// iterate over the notes of the melody:
	size1 = sizeof(game_melody[song_num]) / sizeof(uint16_t);

	for (uint8_t thisNote=0; thisNote<size1; thisNote++) {
		//to calculate the note duration, take 1 second divided by the note type
		//e.g. quarter note = 1000/4, eighth note = 1000/8, etc.
		noteDuration1 = pgm_read_byte(&(game_tempo[song_num][thisNote]));
		if (noteDuration1 != 0) {
			noteDuration1 = 1000 / noteDuration1;	//Duration in ms!
		} else {
			
			return;		//If noteDuration1 = 0, it means the songs is over
		}

		buzz(pgm_read_word(&(game_melody[song_num][thisNote])), noteDuration1);	//Time in ms!!

		//to distinguish the notes, set a minimum time between them.
		pauseBetweenNotes1 = noteDuration1 * 1.30;	//the note's duration + 30%
		my_delay_ms(pauseBetweenNotes1);
		buzz(0, noteDuration1); //stop the tone playing
	}
}


void play_song(uint8_t song_num) {		// iterate over the notes of the melody:
	if (song_num == 0) {size2 = sizeof(mario_melody) / sizeof(uint16_t);}
	else {size2 = sizeof(adventure_melody) / sizeof(uint16_t);}

	for (uint8_t thisNote = 0; thisNote<size2; thisNote++) {
		//to calculate the note duration, take 1 second divided by the note type
		//e.g. quarter note = 1000/4, eighth note = 1000/8, etc.
		if (song_num == 0) {
			noteDuration2 = 1000 / pgm_read_byte(&(mario_tempo[thisNote]));
			buzz(pgm_read_word(&(mario_melody[thisNote])), noteDuration2);	//Time in ms
		}
		else {
			noteDuration2 = 1000 / pgm_read_byte(&(adventure_tempo[thisNote]));
			buzz(pgm_read_word(&(adventure_melody[thisNote])), noteDuration2);
		}

		//to distinguish the notes, set a minimum time between them.
		pauseBetweenNotes2 = noteDuration2 * 1.30;	//the note's duration + 30%
		my_delay_ms(pauseBetweenNotes2);

		//stop the tone playing:
		buzz(0, noteDuration2);
	}
}


void my_delay_ms(uint16_t n) {	
	while(n--) {
		_delay_ms(1);
	}
}


void my_delay_us(uint16_t n) {	
	while(n--) {
		_delay_us(1);
	}
}


//Given functions for easy implementation of songs
void game_start_song()	{game_song(0);}		//Starting song
void game_finish_song()	{game_song(1);}		//Finishing song

void play_mario() 		{play_song(0);}		//Mario song
void play_adventure()	{play_song(1);}		//Adventure time song

#endif