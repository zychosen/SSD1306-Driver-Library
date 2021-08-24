/*
 * ssd1306.c
 *
 *  Created on: Aug 22, 2021
 *      Author: zychosen
 */

#include "i2c.h"
#include "ssd1306.h"
#include <string.h>

static uint8_t buffer[SIZE];

void ssd1306_setup(void) {
	int i;
	for(i = 100; i > 0; i--);         // changed from 10 to 100
	send_command(DISPLAYOFF);
	send_command(SETDISPLAYCLOCKDIV);
	send_command(0x80);               // first 4 bits = 0, ratio = 1 and next 4 bits = 8 (default)
	send_command(SETMULTIPLEX);
	send_command(0x3F);               // vertical size - 1
	send_command(SETDISPLAYOFFSET);
	send_command(0x00);
	send_command(SETSTARTLINE | 0x0);
	send_command(CHARGEPUMP);
	send_command(0x14);                  // charge pump enable
	send_command(MEMORYMODE);
	send_command(0x00);
	send_command(SEGREMAP | 0x01);     // some segment remapping idk
	send_command(COMSCANDEC);
	send_command(SETCOMPINS);
	send_command(0x12);              // bruh it's not 0x02
	send_command(SETCONTRAST);
	send_command(0xCF);
	send_command(SETPRECHARGE);
	send_command(0xF1);
	send_command(SETVCOMDETECT);
	send_command(0x40);
	send_command(DISPLAYALLON_RESUME);
	send_command(NORMALDISPLAY);
	send_command(DEACTIVATE_SCROLL);
	send_command(DISPLAYON);
	clear_screen();
}

void update_screen(void) {
	send_command(PAGEADDR);
	send_command(0x0);
	send_command(0xFF);
	send_command(COLUMNADDR);
	send_command(0);
	send_command(WIDTH - 1);

	i2c_start();
	i2c_transmit_byte(0x40);
	i2c_transmit_multiple_bytes(buffer, SIZE);
	i2c_stop();
}

void clear_screen(void) {
	memset(buffer, 0, SIZE);
	update_screen();
}

void draw_pixel(uint8_t x, uint8_t y, uint8_t colour) {
	if (y >= HEIGHT || x >= WIDTH) return;
	if (colour == WHITE) {
		buffer[x + (uint16_t)((y >> 3) << 7)] |= (1 << (y & 7));      // left shift multiplies by 128 and right shift divides by 8
	} else {
		buffer[x + (uint16_t)((y >> 3) << 7)] &= ~(1 << (y & 7));
	}
}

void send_command(uint8_t cmd) {
	int i;

	i2c_start();					    //                 MSB                            7                                    6-0     ----------------
	i2c_transmit_byte((uint8_t)0x00);   // command format: C0(0-only data bytes further)  0-next byte is command else is data  0            <next byte>
	i2c_transmit_byte(cmd);
	i2c_stop();

	/* delay for a bit */
	for(i = 10; i > 0; i--);
}

void setCursor(uint8_t col, uint8_t page) {
	send_command(SET_LCOL_START_ADDRESS | (col & 0x0F));          // lower 4 bits of 8 bit address
	send_command(SET_HCOL_START_ADDRESS | (col & 0x0F));
	send_command(SET_PAGE_START_ADDRESS | (page & 0x07));
}

void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
	int8_t dx = x2 - x1;
	int8_t dy = y2 - y1;

	if(!dy) {
		while (x1 <= x2) {
			draw_pixel(x1, y1, 1);
			x1++;
		}

	} else if(!dx) {
		while(y1 <= y2) {
			draw_pixel(x1, y1, 1);
			y1++;
		}

	} else {
		/* Naive algorithm
		int x,y;
		for(x = x1; x <= x2; x++) {
			y = y1 + dy*(x-x1)/dx;
			draw_pixel(x, y, 1);
		} */

		/* Bresenham's algorithm */
		int D = 2*dy - dx;
		int x, y = y1;
		for(x = x1; x <= x2; x++) {
			draw_pixel(x,y,1);
			if(D > 0) {
				y++;
				D -= 2*dx;
			}
			D += 2*dy;
		}
	}
}


