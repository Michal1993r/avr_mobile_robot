/*
 * SPI.c
 *
 *  Created on: 29 pa� 2014
 *      Author: MiCkl
 */

#include "SPI_SR.h"


void SendSpiWord( uint16_t word ) {

	SPDR = word>>8;
	while( !(SPSR & (1<<SPIF)) );

	SPDR = word;
	while( !(SPSR & (1<<SPIF)) );



	LT_ON;
	LT_OFF;
}

void SendSpi( uint8_t bajt ) {

	LT_OFF;
	SPDR = bajt;
	while( !(SPSR & (1<<SPIF)) );
	LT_ON;





}


void InitSpi(void) {

	//DDRB |= MOSI|SCK|LT; // piny SPI jako wyj�cia
	SPCR |= (1<<SPE)|(1<<MSTR); // w��cz SPI i ustaw Master
	SPSR |= (1<<SPI2X); // fosc/2
}
