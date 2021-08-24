#include "i2c.h"
#include "ssd1306.h"
#include "font5x8.h"

/**
 * main.c
 */

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	static const uint8_t address = 0x3C;
	i2c_setup(address);
	ssd1306_setup();
	int i;
	while (1) {
		drawString("Hello!", 20, 20);
		update_screen();
		for (i = 30000; i > 0; i--);
	}
}
