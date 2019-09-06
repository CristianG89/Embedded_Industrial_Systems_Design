#ifndef SRAM_H
#define SRAM_H

#include "uart.h"		


/****************************************************************************
External SRAM memory enabled for AT mega162
****************************************************************************/
void SRAM_init(void);		//External SRAM enabled


/****************************************************************************
Address and value as parameters to be written in external  SRAM
****************************************************************************/
void SRAM_write(unsigned int data, unsigned int address);	


/****************************************************************************
Value read from the requested address
****************************************************************************/
unsigned int SRAM_read(unsigned int address);	


/****************************************************************************
Test to write and read in every position of the external RAM memory
****************************************************************************/
void SRAM_test(void);		

#endif //SRAM_H