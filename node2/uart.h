#ifndef UART_H
#define UART_H

#define FOSC 16000000				//Clock Speed (Oscillator)
#define BAUD 9600					//USART blaudrate speed
#define MYUBRR (FOSC/16/BAUD)-1		//Speed configuration for USART0


/****************************************************************************
USART initialization 
Set frame format: 8data bits (UCSZ00 & UCSZ01 = 1), 2stop bit (bit USBS = 1)
****************************************************************************/
void USART_init(unsigned int ubrr);


/****************************************************************************
USART transmit of data.
****************************************************************************/
void USART_transmit(unsigned char data);


/****************************************************************************
Get and return received data from buffer
****************************************************************************/
unsigned char USART_receive(void);


/****************************************************************************
These functions open a file to read/write. Necessary to use printf() with USART.
Otherwise one should write to USART manually byte-by-byte via the UDRx register.
****************************************************************************/
static FILE uart_out = FDEV_SETUP_STREAM (USART_transmit, NULL, _FDEV_SETUP_WRITE);
static FILE uart_in = FDEV_SETUP_STREAM (NULL, USART_receive, _FDEV_SETUP_READ);
#define UART_p (&uart_out)

#endif // UART_H