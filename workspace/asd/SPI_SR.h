/*
 * SPI_SR.h
 *
 *  Created on: 29 pa� 2014
 *      Author: MiCkl
 */

#include <avr/io.h>
//#include <util/delay.h>

/* hardware SPI */
#define MOSI (1<<PB3)	//   <---- A (SER IN)
#define SCK (1<<PB2)	//   <---- SHIFT CLOCK (SC)
#define LT (1<<PB1)		//	 <---- LATCH CLOCK (LT)

#define LT_ON PORTB |= LT
#define LT_OFF PORTB &= ~LT

void SendSpiWord( uint16_t word );
void SendSpi( uint8_t bajt );
void InitSpi(void);

