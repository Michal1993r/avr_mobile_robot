/*
 * UART.h
 *
 *  Created on: 8 wrz 2014
 *      Author: MiCkl
 */

#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 19200
#define MYUBRR FOSC/16/BAUD-1

void USART_Init( unsigned int );
void USART_Transmit( unsigned char );
unsigned char USART_Receive( void );
void USART_TransmitString( char* );
void USART_ClearTerminal();
void USART_CursorPosition( int, int);
void USART_ForegroundColor( char* );



#endif /* UART_H_ */
