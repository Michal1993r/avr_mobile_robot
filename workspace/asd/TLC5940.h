/*
 * TLC5940.h
 *
 *  Created on: 12-09-2015
 *      Author: MiCkl
 */

#include <avr/io.h>
#include <avr/interrupt.h>

// PIN CONFIG

#define GSCLK_DDR DDRB
#define GSCLK_PORT PORTB
#define GSCLK_PIN PB0

#define SIN_DDR DDRB
#define SIN_PORT PORTB
#define SIN_PIN PB3

#define SCLK_DDR DDRB
#define SCLK_PORT PORTB
#define SCLK_PIN PB2

#define BLANK_DDR DDRB
#define BLANK_PORT PORTB
#define BLANK_PIN PB5

#define DCPRG_DDR DDRD
#define DCPRG_PORT PORTD
#define DCPRG_PIN PD7

#define VPRG_DDR DDRD
#define VPRG_PORT PORTD
#define VPRG_PIN PD6

#define XLAT_DDR DDRB
#define XLAT_PORT PORTB
#define XLAT_PIN PB1

#define TLC5940_N 1 // number of chips

#if (12*TLC5940_N > 255)
#define dcData_t uint16_t
#else
#define dcData_t uint8_t
#endif

#if (24*TLC5940_N > 255)
#define gsData_t uint16_t
#else
#define gsData_t uint8_t
#endif

#define dcDataSize ((dcData_t)12 * TLC5940_N)
#define gsDataSize ((gsData_t)24 * TLC5940_N)

#if (16 * TLC5940_N > 255)
#define channel_t uint16_t
#else
#define channel_t uint8_t
#endif

#define numChannels ((channel_t)16 * TLC5940_N)

// external variables

extern uint8_t dcData[dcDataSize];
extern uint8_t gsData[gsDataSize];
extern volatile uint8_t gsUpdateFlag;

// state macros

#define setOutput(ddr, pin) ((ddr) |= (1 << (pin)))
#define setLow(port, pin) ((port) &= !(1 << (pin)))
#define setHigh(port, pin) ((port) |= (1 << (pin)))

// Change state pulse

#define pulse(port, pin) do { \
	setHigh((port), (pin)); \
	setLow((port), (pin)); \
} while (0)

#define outputState(port, pin) ((port) & (1 << (pin)))


// Functions

void TLC5940_Init(void); // initialize

void TLC5940_ClockInDC(void);

void TLC5940_SetGS_And_GS_PWM(void);

void TLC5940_SetAllGS (uint16_t);

void TLC5940_SetAllDC (uint8_t);

void TLC5940_SetGS(channel_t, uint16_t);

void TLC5940_SetDC(channel_t, uint16_t);

static inline void TLC5940_SetGSUpdateFlag(void) {
__asm__ volatile ("" ::: "memory");
gsUpdateFlag = 1;
}




