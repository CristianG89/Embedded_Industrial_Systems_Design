#ifndef ADC_H
#define ADC_H

#include "uart.h"	

/****************************************************************************
ADC converter initialization
****************************************************************************/
void ADC_init(void);	


/****************************************************************************
Requests a new reading of the ADC converter. 
Can select which channel to read from by passing channel as parameter  
Returns the converted analog to digital value
****************************************************************************/
uint8_t ADC_read(uint8_t channel);	

#endif //ADC_H