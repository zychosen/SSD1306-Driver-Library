#include <msp430f5529.h>
#include "ssd1306.h"

unsigned char const write_address = 0x3C;

void i2c_init() {
	P3SEL |= BIT0 | BIT1;

	UCB0CTL1 |= UCSWRST;
	UCB0CTL0 = (UCMST | UCMODE_3 | UCSYNC);
	UCB0CTL1 = (UCSSEL_2 | UCSWRST);
	UCB0BR0 = 12;
	UCB0BR1 = 0;
	UCB0I2CSA = write_address;
	UCB0CTL1 &= ~UCSWRST;
}

void main(void) {
	WDTCTL = WDTPW | WDTHOLD;
	i2c_init();
	setup();
	int i;
	while (1) {
		draw_pixel(64, 32, 1);
		update_screen();
		for (i = 30000; i > 0; i--);
		draw_pixel(64, 32, 0);
		update_screen();
		for (i = 30000; i > 0; i--);
	}
}

