/*
 * main.c
 *
 *  Created on: 27-09-2015
 *      Author: root
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART.h"

uint16_t measure();

int main(){

	ADMUX |= ((1 << REFS1) | (1 << REFS0));
	ADCSRA |= ((1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2));

	USART_Init( MYUBRR );

	double result;
	int d1;
	double f2;
	int d2;
	char temp[45];

	while(1){

		result = measure()*0.0025*4.58;

		d1 = result;
		f2 = result - d1;
		d2 = trunc(f2*10000);

		USART_ClearTerminal();

		USART_ForegroundColor( "BLUE" );

		USART_CursorPosition(5,20);

		USART_TransmitString( "--==POWER SUPPLY ATMEGA8 by MiCkl==--\r\n");

		USART_ForegroundColor( "WHITE" );

		USART_CursorPosition(10,21);

		USART_TransmitString( "Aktualne napiecie: " );

		if ( result <= 4 && result >= 0) USART_ForegroundColor( "GREEN" );
		else if ( result <= 8 && result >= 4) USART_ForegroundColor( "YELLOW" );
		else if ( result <= 12 && result >= 8) USART_ForegroundColor( "RED" );

		sprintf(temp, "%d.%04d [+-0.2] V \r\n", d1, d2);
		USART_TransmitString( temp );
		USART_TransmitString("\r\n");

		_delay_ms(200);
	}


}

uint16_t measure(){

	ADCSRA |= (1 << ADSC);

	while( ADCSRA & (1 << ADSC));

	return ADCW;

}
