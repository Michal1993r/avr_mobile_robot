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

ISR(TIMER0_COMPA_vect);

extern uint8_t gsData[192 * TLC5940_N];  // external variables from TLC5940 library
extern uint8_t dcData[96 * TLC5940_N];

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

	TLC5940_Init();
	TLC5940_ClockInDC();

	sei();

	for(;;){};


	return 0;
}


ISR(TIMER0_COMPA_vect){

	static uint8_t xlatNeedsPulse = 0;

	setHigh(BLANK_PORT, BLANK_PIN);

	if (outputState(VPRG_PORT, VPRG_PIN)) {

		setLow(VPRG_PORT, VPRG_PIN);

		if (xlatNeedsPulse){

			pulse(XLAT_PORT, XLAT_PIN);
			xlatNeedsPulse = 0;

		}

		pulse(SCLK_PORT, SCLK_PIN);

	}

	else

		if (xlatNeedsPulse){

			pulse(XLAT_PORT, XLAT_PIN);
			xlatNeedsPulse = 0;

		}


	setLow(BLANK_PORT, BLANK_PIN);

	for (gsData_t i=0; i < gsDataSize; i++){

		SPDR = gsData[i];
		while (!(SPSR & ( 1 << SPIF)));

	}

	xlatNeedsPulse = 1;

}

