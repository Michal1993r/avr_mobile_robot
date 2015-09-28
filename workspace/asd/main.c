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
#include "TLC5940.h"

// macros defining output states

#define forward( velocity ) while(gsUpdateFlag); \
				TLC5940_SetGS(1, velocity); \
				TLC5940_SetGS(2, 0); \
				TLC5940_SetGS(3, velocity); \
				TLC5940_SetGS(4, 0); \
				TLC5940_SetGS(11, velocity); \
				TLC5940_SetGS(12, 0); \
				TLC5940_SetGS(13, velocity); \
				TLC5940_SetGS(14, 0); \
				TLC5940_SetGSUpdateFlag();

#define reverse( velocity ) while(gsUpdateFlag); \
				TLC5940_SetGS(2, velocity); \
				TLC5940_SetGS(1, 0); \
				TLC5940_SetGS(4, velocity); \
				TLC5940_SetGS(3, 0); \
				TLC5940_SetGS(12, velocity); \
				TLC5940_SetGS(11, 0); \
				TLC5940_SetGS(14, velocity); \
				TLC5940_SetGS(13, 0); \
				TLC5940_SetGSUpdateFlag();

#define left( velocity ) while(gsUpdateFlag); \
				TLC5940_SetGS(3, velocity); \
				TLC5940_SetGS(1, 0); \
				TLC5940_SetGS(2, velocity); \
				TLC5940_SetGS(4, 0); \
				TLC5940_SetGS(11, 0); \
				TLC5940_SetGS(12, velocity); \
				TLC5940_SetGS(13, velocity); \
				TLC5940_SetGS(14, 0); \
				TLC5940_SetGSUpdateFlag();

#define right( velocity ) while(gsUpdateFlag); \
				TLC5940_SetGS(1, velocity); \
				TLC5940_SetGS(2, 0); \
				TLC5940_SetGS(4, velocity); \
				TLC5940_SetGS(3, 0); \
				TLC5940_SetGS(11, velocity); \
				TLC5940_SetGS(12, 0); \
				TLC5940_SetGS(13, 0); \
				TLC5940_SetGS(14, velocity); \
				TLC5940_SetGSUpdateFlag();

#define breaks	while(gsUpdateFlag); \
				TLC5940_SetAllGS(3500); \
				TLC5940_SetGSUpdateFlag();

#define coast	while(gsUpdateFlag); \
				TLC5940_SetAllGS(0); \
				TLC5940_SetGSUpdateFlag();


void apply(char, int*); 		// Function applies change of velocity

int main (void) {

	USART_Init( MYUBRR ); 		// Initialize USART

	TLC5940_Init();				// 	Initialize TLC5940 requirements ( CTC, SPI )
	TLC5940_SetAllGS(0);		// 	Set all outputs low

	sei();						// 	Enable Interrupts

	while(gsUpdateFlag);
	TLC5940_SetGSUpdateFlag();

	int a;
	int b = 2500;				// 	Set default velocity ( 0 - 4095 )
	char direction;				//	Support variable for the "apply" function,
	int* c = &b;				// 	Pointer used for changing velocity inside main loop

	while (1)					// main loop
	{

		a = USART_Receive();	// Receive bluetooth instruction

		switch ( a )
		{
		case 0x41:				// "A" - forward

				forward( *c );
				direction = 'f';

				break;

		case 0x42: 				// "B" - reverse

				reverse( *c );
				direction = 'b';

				break;

		case 0x43:				// "C" - right

				right( *c );
				direction = 'r';

				break;

		case 0x44:				// "D" - left

				left( *c );
				direction = 'l';

				break;

		case 0x61:				// "a" - faster

				*c+= 200;		// value of incrementation
				apply(direction, c);	// apply new value

				break ;

		case 0x62:				// "b" - slower

				*c-= 200;		// value of decrementation
				apply(direction, c);

				break ;

		case 0x63:				// "c" - breaks

			breaks;

			break;

		default:				// any other

			coast;

			break ;
		}



		USART_Transmit(a);


	}


	return 0;
}

void apply( char c, int* p ){

	switch ( c ) {
		case 'f':

				if ( *p > 4095 ) *p = 4095;
				forward( *p );

			break;

		case 'l':

				if ( *p > 4095 ) *p = 4095;
				left( *p );

			break;

		case 'r':

				if ( *p > 4095 ) *p = 4095;
				right( *p );

			break;

		case 'b':

				if ( *p > 4095 ) *p = 4095;
				reverse( *p );

			break;

		default :

				if ( *p > 4095 ) *p = 4095;
				coast;
	}


}
