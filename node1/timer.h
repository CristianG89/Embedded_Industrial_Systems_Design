#ifndef TIMER_H
#define TIMER_H


/****************************************************************************
To set the prescaler in the PWM output. Takes prescaler value as paramater
****************************************************************************/
uint8_t timer_set_prescaler(uint16_t presc_value);


/****************************************************************************
Initilaziation of timer 1 with prescaler and frequency as argument
****************************************************************************/
void timer1_init(uint16_t prescaler, uint16_t frequency);

#endif // TIMER_H