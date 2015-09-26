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

#define left  	0b00000000
#define right 	0b00000000
#define forward PORTB = 0b01010100; PORTD |= (1 << PD7);
#define reverse 0b00000000
#define Break	0b00000000


int main (void) {

	//USART_Init( MYUBRR ); // initialize USART

//	while (1)
//	{
//
//		a = USART_Receive();
//
//		switch ( a )
//		{
//		case 0x41: 	SendSpi( 0xff );	break ;
//		case 0x42: 		break ;
//		case 0x43: 		break ;
//		default: 			break ;
//		}
//
//		USART_Transmit(a);
//
//
//	}

	//TLC5940_SetAllDC(255);
	//TLC5940_ClockInDC();

	TLC5940_Init();
	TLC5940_SetAllGS(0);

	sei();

	channel_t i = 0;

	for(;;){

		while(gsUpdateFlag);
		TLC5940_SetAllGS(0);
		//TLC5940_SetGS(i, 4095);
		TLC5940_SetGSUpdateFlag();
		_delay_ms(255);
		i = (i + 1) % numChannels;
	};


	return 0;
}




