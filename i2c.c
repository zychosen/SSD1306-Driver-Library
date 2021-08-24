/*
 * i2c.c
 *
 *  Created on: Aug 22, 2021
 *      Author: zychosen
 */
#include "i2c.h"

void i2c_setup(uint8_t address) {
	/* select i2c mode on Port 3 */
	P3SEL |= BIT0 | BIT1;

	/* configure i2c control register */
	UCB0CTL1 |= UCSWRST;
	UCB0CTL0 = (UCMST | UCMODE_3 | UCSYNC);
	UCB0CTL1 = UCSSEL_2;
	UCB0BR0 = 12;
	UCB0BR1 = 0;
	UCB0I2CSA = address;
	UCB0CTL1 &= ~UCSWRST;
}

inline void i2c_start(void) {
	while(UCB0CTL1 & UCTXSTP);
	UCB0CTL1 |= (UCTR | UCTXSTT);     // configure as transmitter (UCTR = 1)
}

inline void i2c_stop(void) {
	while(!(UCB0IFG & UCTXIFG));
	while(UCB0CTL1 & UCTXSTT);
	UCB0CTL1 |= UCTXSTP;
}

inline void i2c_transmit_multiple_bytes(uint8_t *byte, uint16_t byteCtr) {
	while(!(UCB0IFG & UCTXIFG));
	while (byteCtr--) {
		while(!(UCB0IFG & UCTXIFG));
		UCB0TXBUF = *byte++;
	}
}

inline void i2c_transmit_byte(uint8_t byte) {
	while(!(UCB0IFG & UCTXIFG));
	UCB0TXBUF = byte;
}
