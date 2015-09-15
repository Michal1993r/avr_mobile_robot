/*
 * TLC5940.c
 *
 *  Created on: 12-09-2015
 *      Author: MiCkl
 */

#include "TLC5940.h"

uint8_t dcData[dcDataSize];

uint8_t gsData[gsDataSize];

volatile uint8_t gsUpdateFlag;

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

void TLC5940_SetAllGS (uint16_t value) {

	uint8_t tmp1 = (value >> 4);
	uint8_t tmp2 = (uint8_t)(value << 4) | (tmp1 >> 4);
	gsData_t i = 0;

	do {

		gsData[i++] = tmp1;				// bits: 11 10 09 08 07 06 05 04
		gsData[i++] = tmp2;				// bits: 03 02 01 00 11 10 09 08
		gsData[i++] = (uint8_t)value;	// bits: 07 06 05 04 03 02 01 00

	} while (i < gsDataSize);

}

void TLC5940_SetAllDC (uint8_t value){

	uint8_t tmp1 = (uint8_t)(value << 2);
	uint8_t tmp2 = (uint8_t)(tmp1 << 2);
	uint8_t tmp3 = (uint8_t)(tmp2 << 2);
	tmp1 |= (value >> 4);
	tmp2 |= (value >> 2);
	tmp3 |= value;
	dcData_t i = 0;

	do {

		dcData[i++] = tmp1;				// bits: 05 04 03 02 01 00 05 04
		dcData[i++] = tmp2;				// bits: 03 02 01 00 05 04 03 02
		dcData[i++] = tmp3;	// bits: 01 00 05 04 03 02 01 00

	} while (i < dcDataSize);

}

void TLC5940_SetGS(channel_t channel, uint16_t value){

	channel = numChannels - 1 - channel;
	uint16_t i = (uint16_t)channel * 3 / 2;

	switch (channel % 2) {
		case 0:
			gsData[i] = (value >> 4);
			i++;
			gsData[i] = (gsData[i] & 0x0F) | (uint8_t)(value << 4);
			break;
		default:
			gsData[i] = (gsData[i] & 0xF0) | (value >> 8);
			i++;
			gsData[i] = (uint8_t)value;
			break;
	}

}

void TLC5940_SetDC(channel_t channel, uint16_t value){

	channel = numChannels - 1 - channel;
	uint16_t i = (uint16_t)channel * 3 / 4;

	switch (channel % 4) {
		case 0:
			dcData[i] = (dcData[i] & 0x03) | (uint8_t)(value << 2);
			break;

		case 1:
			dcData[i] = (dcData[i] & 0xFC) | (uint8_t)(value >> 4);
			i++;
			dcData[i] = (dcData[i] & 0x0F) | (uint8_t)(value << 4);
			break;

		case 2:
			dcData[i] = (dcData[i] & 0xF0) | (uint8_t)(value >> 2);
			i++;
			dcData[i] = (dcData[i] & 0x3F) | (uint8_t)(value << 6);
			break;

		default:
			dcData[i] = (dcData[i] & 0xC0) | (uint8_t)(value);
			break;
	}

}
