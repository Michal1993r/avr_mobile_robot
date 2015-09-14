/*
 * TLC5940.c
 *
 *  Created on: 12-09-2015
 *      Author: MiCkl
 */

#include "TLC5940.h"

uint8_t dcData[12 * TLC5940_N] = {

		// MSB LSB
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,

};

uint8_t gsData[24 * TLC5940_N] = {

		// MSB LSB
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,




};


void TLC5940_Init(void) {

	setOutput(GSCLK_DDR, GSCLK_PIN);
	setOutput(SCLK_DDR, SCLK_PIN);
	setOutput(DCPRG_DDR, DCPRG_PIN);
	setOutput(VPRG_DDR, VPRG_PIN);
	setOutput(XLAT_DDR, XLAT_PIN);
	setOutput(BLANK_DDR, BLANK_PIN);
	setOutput(SIN_DDR, SIN_PIN);
	setLow(GSCLK_PORT, GSCLK_PIN);
	setLow(SCLK_PORT, SCLK_PIN);
	setLow(DCPRG_PORT, DCPRG_PIN);
	setHigh(VPRG_PORT, VPRG_PIN);
	setLow(XLAT_PORT, XLAT_PIN);
	setHigh(BLANK_PORT, BLANK_PIN);

	SPCR |= (1<<SPE)|(1<<MSTR); // SPI initiate
	SPSR |= (1<<SPI2X); // fosc/2

	// CTC
	TCCR0A = (1 << WGM01);
	TCCR0B = ((1 << CS02) | (1 << CS00)); // prescaler
	OCR0A = 3; // interrupt every 4096 cycles
	TIMSK0 |= (1 << OCIE0A); // enable compare match
}


void TLC5940_ClockInDC(void) {

	setHigh(DCPRG_PORT, DCPRG_PIN);
	setHigh(VPRG_PORT, VPRG_PIN);

	for (dcData_t i=0; i < dcDataSize; i++){

		SPDR = dcData[i]; // start SPI transmission

		while (!( SPSR & ( 1 << SPIF )));

	}

	pulse( XLAT_PORT, XLAT_PIN );

}


void TLC5940_SetGS_And_GS_PWM(void) {
	uint8_t firstCycleFlag = 0;
	if (outputState(VPRG_PORT, VPRG_PIN)) {
		setLow(VPRG_PORT, VPRG_PIN);
		firstCycleFlag = 1;
	}
	uint16_t GSCLK_Counter = 0;
	uint8_t Data_Counter = 0;
	setLow(BLANK_PORT, BLANK_PIN);
	for (;;) {
		if (GSCLK_Counter > 4095) {
			setHigh(BLANK_PORT, BLANK_PIN);
			pulse(XLAT_PORT, XLAT_PIN);
			if (firstCycleFlag) {
				pulse(SCLK_PORT, SCLK_PIN);
				firstCycleFlag = 0;
			}
			break;
		}
		else {
			if (!(Data_Counter > TLC5940_N * 192 - 1)) {
				if (gsData[Data_Counter])
					setHigh(SIN_PORT, SIN_PIN);
				else
					setLow(SIN_PORT, SIN_PIN);
				pulse(SCLK_PORT, SCLK_PIN);
				Data_Counter++;
			}
		}
		pulse(GSCLK_PORT, GSCLK_PIN);
		GSCLK_Counter++;
	}
}
