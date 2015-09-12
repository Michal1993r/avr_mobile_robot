/*
 * TLC5940.c
 *
 *  Created on: 12-09-2015
 *      Author: MiCkl
 */

#include "TLC5940.h"

uint8_t dcData[96 * TLC5940_N] = {
// MSB LSB
1, 1, 1, 1, 1, 1, // Channel 15
1, 1, 1, 1, 1, 1, // Channel 14
1, 1, 1, 1, 1, 1, // Channel 13
1, 1, 1, 1, 1, 1, // Channel 12
1, 1, 1, 1, 1, 1, // Channel 11
1, 1, 1, 1, 1, 1, // Channel 10
1, 1, 1, 1, 1, 1, // Channel 9
1, 1, 1, 1, 1, 1, // Channel 8
1, 1, 1, 1, 1, 1, // Channel 7
1, 1, 1, 1, 1, 1, // Channel 6
1, 1, 1, 1, 1, 1, // Channel 5
1, 1, 1, 1, 1, 1, // Channel 4
1, 1, 1, 1, 1, 1, // Channel 3
1, 1, 1, 1, 1, 1, // Channel 2
1, 1, 1, 1, 1, 1, // Channel 1
1, 1, 1, 1, 1, 1, // Channel 0
};

uint8_t gsData[192 * TLC5940_N] = {
// MSB LSB
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 15
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 14
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 13
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, // Channel 12
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, // Channel 11
0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, // Channel 10
0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, // Channel 9
0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, // Channel 8
0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, // Channel 7
0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, // Channel 6
0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, // Channel 5
0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 4
0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 3
0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 2
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Channel 1
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // Channel 0
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
}


void TLC5940_ClockInDC(void) {

	setHigh(DCPRG_PORT, DCPRG_PIN);
	setHigh(VPRG_PORT, VPRG_PIN);

	uint8_t Counter = 0;

	for (;;) {

		if (Counter > TLC5940_N * 96 - 1)
 		{
			pulse(XLAT_PORT, XLAT_PIN);
			break;
		}

		else {

			if (dcData[Counter])
				setHigh(SIN_PORT, SIN_PIN);
			else
				setLow(SIN_PORT, SIN_PIN);

			pulse(SCLK_PORT, SCLK_PIN);
			Counter++;
		}
	}
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
