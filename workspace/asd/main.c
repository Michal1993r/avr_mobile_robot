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

ISR( TIMER0_COMPA_vect);

extern uint8_t gsData[192 * TLC5940_N];
extern uint8_t dcData[96 * TLC5940_N];



int main (void) {

	//USART_Init( MYUBRR ); // initialize USART
	//InitSpi();

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

ISR( TIMER0_COMPA_vect){

	uint8_t firstCycleFlag = 0;
	static uint8_t xlatNeedsPulse = 0;

	setHigh(BLANK_PORT, BLANK_PIN);

	if (outputState(VPRG_PORT, VPRG_PIN)) {

		setLow(VPRG_PORT, VPRG_PIN);
		firstCycleFlag = 1;

	}

	if (xlatNeedsPulse) {

		pulse(XLAT_PORT, XLAT_PIN);
		xlatNeedsPulse = 0;

	}

	if (firstCycleFlag) pulse(SCLK_PORT, SCLK_PIN);

	setLow(BLANK_PORT, BLANK_PIN);

	uint8_t DataCounter = 0;

	for(;;){

		if (!(DataCounter > TLC5940_N * 192 - 1)){

			if (gsData[DataCounter]) setHigh(SIN_PORT, SIN_PIN);
			else setLow(SIN_PORT, SIN_PIN);

			pulse(SCLK_PORT, SCLK_PIN);
			DataCounter++;

		}

		else {

			xlatNeedsPulse = 1;
			break;

		}

	}
}

