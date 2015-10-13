/*
 * main.c
 *
 *  Created on: 18 mar 2015
 *      Author: MiCkl
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART.h"
#include "TLC5940.h"


#define DefaultVelocity 3100 // Default velocity 0 - 4095

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

#define breaks( velocity ) 	while(gsUpdateFlag); \
				TLC5940_SetAllGS(3500); \
				velocity = DefaultVelocity; \
				TLC5940_SetGSUpdateFlag();

#define coast( velocity )	while(gsUpdateFlag); \
				TLC5940_SetAllGS(0); \
				velocity = DefaultVelocity; \
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
	int b = DefaultVelocity;	// 	Set velocity to default
	char direction;				//	Support variable for the "apply" function,
	int* c = &b;				// 	Pointer used for changing velocity inside main loop

	float per;					// 	Velocity percentage
	int d1, d2;
	double f2;

	char temp[30];				//  Temporary variable needed to use sprintf()

	while (1)					// main loop
	{

		a = USART_Receive();	// Receive bluetooth instruction

		switch ( a ){

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

			breaks( *c );

			break;

		default:				// any other

			coast( *c );

			break ;
		}

// Terminal management

		USART_ClearTerminal();

		per = (float)b / 4095 * 100;

		d1 = per;
		f2 = per - d1;
		d2 = trunc(f2*10000);

		if (d2 > 5000) d1++;

		USART_CursorPosition(17, 31);

		USART_ForegroundColor( "BLUE" );

		USART_TransmitString( "--==Mobile Robot by MiCkl==--");

		USART_ForegroundColor( "WHITE" );

		sprintf( temp, "Velocity: %d%%", d1 );

		USART_CursorPosition(19, 37);

		USART_TransmitString( temp );

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
				coast( *p );
	}


}

