#ifndef GAME_NODE2_H
#define GAME_NODE2_H


/****************************************************************************
Racket (motor+PID) initialization for playing
****************************************************************************/
void game_node2_init(void);


/****************************************************************************
The game has finished (stand-by mode)
****************************************************************************/
uint8_t game_node2_over(void);


/****************************************************************************
A new game starts. Buttons, joystick and sliders activate to control the
motor (racket)
****************************************************************************/
void game_node2_play(void);

#endif //GAME_NODE2_H