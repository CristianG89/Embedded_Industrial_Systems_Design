#ifndef F_CPU
#define F_CPU 4915200	//Clock Speed (Oscillator)

#include <stdio.h>		
#include <avr/io.h> 	
#include <avr/interrupt.h>
#include <util/delay.h>	

#include "uart.h"		
#include "buttons.h"	
#include "can.h"
#include "game_node1.h"
#include "highscore.h"
#include "play_song.h"


//CAN messages defined
packet start_game = {.id=CAN_START_GAME_ID, .length=0x01, .data={0x06}};
packet end_game = {.id=CAN_END_GAME_ID, .length=0x00, .data={}};
packet shoot_ball = {.id=CAN_SHOOT_ID, .length=0x00, .data={}};
packet score;


//gloabal variabels for quiting game 
int player_want_to_play = 1;
int player_option = 1;

uint8_t game_node1_play() {
	OLED_clear_all();
	OLED_update(); 
	OLED_pos(1,25);
	OLED_print_all("GO");
	OLED_pos(1,50);
	OLED_print_all(highscore_get_player_name());
	OLED_pos(5,20);
	OLED_print_all("LEFT=QUIT");
	CAN_send(&start_game);
	int player_want_to_play = 1;
	int player_option = 1;
	
	game_start_song();
	
	while(player_want_to_play) {
		buttons_send_CAN_message();	
		OLED_update();
		player_option = 1;
		

		if (BUTTON_L || CAN_message_recieved()) {	
			_delay_ms(100);
			CAN_send(&end_game);						
			score = CAN_read();	
			fprintf(UART_p, "score: %d\r\n", score.data[0]);
			highscore_save_sram(highscore_get_player_name(), score.data[0]);
			OLED_clear_all();
			game_finish_song();

			while(player_option){
				OLED_update();
				OLED_pos(0,25);
				OLED_print_all("GAME OVER");
				OLED_pos(2,25);
				OLED_print_all("SCORE");
				OLED_pos(2,75);
				OLED_print_number(score.data[0]);
				fprintf(UART_p, "SCORE:  %d\n", score.data[0]);
				OLED_pos(5,20);
				OLED_print_all("RIGHT=RETRY");
				OLED_pos(6,20);
				OLED_print_all("LEFT=QUIT");
				_delay_ms(400);

				if(BUTTON_R){
					OLED_clear_all();
					OLED_update(); 
					OLED_pos(1,25);
					OLED_print_all("GO");
					OLED_pos(1,50);
					OLED_print_all(highscore_get_player_name());
					OLED_pos(5,20);
					OLED_print_all("LEFT=QUIT");
					CAN_send(&start_game);
					player_option = 0;
					game_start_song();
				}

				if(BUTTON_L){
					player_want_to_play = 0;
					player_option = 0;
				}	
			}		
		}
	}
	return 1;
}
#endif //F_CPU