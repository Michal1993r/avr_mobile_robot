/*
 * main.c
 *
 *  Created on: 18 mar 2015
 *      Author: MiCkl
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "UART.h"
#include "SPI_SR.h"
#include "TLC5940.h"

#define left  	0b00000000
#define right 	0b00000000
#define forward PORTB = 0b01010100; PORTD |= (1 << PD7);
#define reverse 0b00000000
#define Break	0b00000000



int main (void) {

	USART_Init( MYUBRR ); // initialize USART
	sei();
	InitSpi();

	DDRB |= ( 1<<PB4 ); // VPROG

	int a;

	// CTC

	TCCR2B |= ( 1 << CS20 );
	TCCR2A |= ( 1 << WGM21 );
	TCCR2A |= ( 1 << COM2B0 );
	DDRD |= ( 1 << PD6 );
	DDRD |= ( 1 << PD5 );

	int b = 0;



	while (1)
	{

		a = USART_Receive();

		switch ( a )
		{
		case 0x41: 	SendSpi( 0xff );	break ;
		case 0x42: 		break ;
		case 0x43: 		break ;
		default: 			break ;
		}

		USART_Transmit(a);


	}

}

