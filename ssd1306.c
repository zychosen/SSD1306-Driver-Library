#include "ssd1306.h"
#include <msp430f5529.h>
#include <string.h>

static unsigned char buffer[SIZE];               // 128x64 pixels and 1024 bytes in total

void setup(void) {
	int i;
	for(i = 100; i > 0; i--);       // changed from 10 to 100
	command(DISPLAYOFF);
	command(SETDISPLAYCLOCKDIV);
	command(0x80);              // first 4 bits = 0, ratio = 1 and next 4 bits = 8 (default)
	command(SETMULTIPLEX);
	command(0x3F);              // vertical size - 1
	command(SETDISPLAYOFFSET);
	command(0x00);
	command(SETSTARTLINE | 0x0);
	command(CHARGEPUMP);
	command(0x14);                  // charge pump enable
	command(MEMORYMODE);
	command(0x00);
	command(SEGREMAP | 0x01);     // some segment remapping idk
	command(COMSCANDEC);
	command(SETCOMPINS);
	command(0x12);              // bruh it's not 0x02
	command(SETCONTRAST);
	command(0xCF);
	command(SETPRECHARGE);
	command(0xF1);
	command(SETVCOMDETECT);
	command(0x40);
	command(DISPLAYALLON_RESUME);
	command(NORMALDISPLAY);
	command(DEACTIVATE_SCROLL);
	command(DISPLAYON);
	clear_screen();
}

void update_screen(void) {
	command(PAGEADDR);
	command(0x0);
	command(0xFF);
	command(COLUMNADDR);
	command(0);
	command(WIDTH - 1);

	unsigned short count = SIZE;
	unsigned char *p = buffer;
	while(UCB0CTL1 & UCTXSTP);
	UCB0CTL1 |= (UCTR | UCTXSTT);
	while(!(UCB0IFG & UCTXIFG));
	UCB0TXBUF = (unsigned char)0x40;
	while(count--){
		while(!(UCB0IFG & UCTXIFG));
		UCB0TXBUF = *p++;
	}
	while(UCB0CTL1 & UCTXSTT);
	UCB0CTL1 |= UCTXSTP;
}

void clear_screen(void) {
	memset(buffer, 0, SIZE);
	update_screen();
}

void draw_pixel(unsigned char x, unsigned char y, unsigned char color) {
	if (y >= HEIGHT || x >= WIDTH)
		return;
	if (color == WHITE) {
		buffer[x + (unsigned short)((y >> 3) << 7)] |= (1 << (y & 7));      // left shift multiplies by 128 and right shift divides by 8
	} else {
		buffer[x + (unsigned short)((y >> 3) << 7)] &= ~(1 << (y & 7));
	}
}

void command(unsigned char cmd) {
	int i;
	while(UCB0CTL1 & UCTXSTP);
	UCB0CTL1 |= (UCTR | UCTXSTT);
	while(!(UCB0IFG & UCTXIFG));
	UCB0TXBUF = (unsigned char)0x00;
	while(!(UCB0IFG & UCTXIFG));
	UCB0TXBUF = cmd;
	while(!(UCB0IFG & UCTXIFG));
	while(UCB0CTL1 & UCTXSTT);
	UCB0CTL1 |= UCTXSTP;
	for(i = 10; i > 0; i--);
}



