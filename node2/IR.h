#ifndef IR_H
#define IR_H

/****************************************************************************
Initial (and basis) reading of IR sensor
****************************************************************************/
void IR_init();

/****************************************************************************
Check if IR sensor detects
****************************************************************************/
uint8_t IR_triggered();

/****************************************************************************
Reads the current value of IR sensor (filtered with the last 4 readings)
****************************************************************************/
uint8_t IR_read_filtered();

#endif //IR_H