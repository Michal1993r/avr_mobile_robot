/*
 * TLC5940.h
 *
 *  Created on: 12-09-2015
 *      Author: MiCkl
 */
#include <avr/io.h>


// PIN CONFIG

#define GSCLK_DDR DDRB
#define GSCLK_PORT PORTB
#define GSCLK_PIN PB0

#define SIN_DDR DDRB
#define SIN_PORT PORTB
#define SIN_PIN PB3

#define SCLK_DDR DDRB
#define SCLK_PORT PORTB
#define SCLK_PIN PB5

#define BLANK_DDR DDRB
#define BLANK_PORT PORTB
#define BLANK_PIN PB2

#define DCPRG_DDR DDRD
#define DCPRG_PORT PORTD
#define DCPRG_PIN PD4

#define VPRG_DDR DDRD
#define VPRG_PORT PORTD
#define VPRG_PIN PD7

#define XLAT_DDR DDRB
#define XLAT_PORT PORTB
#define XLAT_PIN PB1

#define TLC5940_N 1

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
