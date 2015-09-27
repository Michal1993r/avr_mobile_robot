/*
 * UART.c
 *
 *  Created on: 8 wrz 2014
 *      Author: MiCkl
 */

#include "UART.h"

void USART_Init( unsigned int ubrr )
{
/*Set baud rate */
UBRRH = (unsigned char)(ubrr>>8);
UBRRL = (unsigned char)ubrr;
/*Enable receiver and transmitter */
UCSRB = (1<<RXEN)|(1<<TXEN);
/* Set frame format: 8data, 1stop bit */
UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

void USART_Transmit( unsigned char data )
{
/* Wait for empty transmit buffer */
while ( !( UCSRA & (1<<UDRE)) );
/* Put data into buffer, sends the data */
UDR = data;
}

unsigned char USART_Receive( void )
{
/* Wait for data to be received */
while ( !(UCSRA & (1<<RXC)) );
/* Get and return received data from buffer */
return UDR;
}

void USART_TransmitString( char* s ){

	while( *s ) USART_Transmit( *s++ );

}

void USART_ClearTerminal(){

	USART_Transmit( 0x1B );				// <Esc>
	USART_TransmitString( "[2J");
	USART_Transmit( 0x1B );
	USART_TransmitString( "[H");		// Cursor to home

}

void USART_CursorPosition( int r, int c){

	char temp[20];
	USART_Transmit( 0x1B );				// <Esc>
	sprintf(temp, "[%d;%df", r, c);
	USART_TransmitString( temp );

}

void USART_ForegroundColor( char *s ){

	if ( s == "BLACK"){

			USART_Transmit( 0x1B );
			USART_TransmitString("[30m");

	}

	if ( s == "RED"){

			USART_Transmit( 0x1B );
			USART_TransmitString("[31m");

	}

	if ( s == "GREEN"){

				USART_Transmit( 0x1B );
				USART_TransmitString("[32m");

		}

	if ( s == "YELLOW"){

				USART_Transmit( 0x1B );
				USART_TransmitString("[33m");

		}

	if ( s == "BLUE"){

				USART_Transmit( 0x1B );
				USART_TransmitString("[34m");

		}

	if ( s == "MAGNETA"){

				USART_Transmit( 0x1B );
				USART_TransmitString("[35m");

		}

	if ( s == "CYAN"){

				USART_Transmit( 0x1B );
				USART_TransmitString("[36m");

		}

	if ( s == "WHITE"){

				USART_Transmit( 0x1B );
				USART_TransmitString("[37m");

		}

}
