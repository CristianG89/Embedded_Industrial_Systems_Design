#ifndef ADC_H
#define ADC_H

#include "uart.h"

/****************************************************************************
ADC converter initialization
****************************************************************************/
void ADC_init(void);

/****************************************************************************
Requests a new reading of the ADC converter
Returns converted analog data to digital 
****************************************************************************/
uint16_t ADC_read();

#endif //ADC_H