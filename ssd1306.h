#ifndef __SSD1306_H__
#define __SSD1306_H__

#define BLACK 0   ///< Draw 'off' pixels
#define WHITE 1   ///< Draw 'on' pixels
#define INVERSE 2 ///< Invert pixels

#define WIDTH 128
#define HEIGHT 64
#define SIZE (WIDTH*HEIGHT)/8

#define MEMORYMODE 0x20
#define COLUMNADDR 0x21
#define PAGEADDR 0x22
#define SETCONTRAST 0x81
#define CHARGEPUMP 0x8D
#define SEGREMAP 0xA0
#define DISPLAYALLON_RESUME 0xA4
#define NORMALDISPLAY 0xA6
#define INVERTDISPLAY 0xA7
#define SETMULTIPLEX 0xA8
#define DISPLAYOFF 0xAE
#define DISPLAYON 0xAF
#define COMSCANDEC 0xC8
#define SETDISPLAYOFFSET 0xD3
#define SETDISPLAYCLOCKDIV 0xD5
#define SETPRECHARGE 0xD9
#define SETCOMPINS 0xDA
#define SETVCOMDETECT 0xDB
#define SETSTARTLINE 0x40

#define EXTERNALVCC 0x01
#define SWITCHCAPVCC 0x02

#define RIGHT_HORIZONTAL_SCROLL 0x26
#define LEFT_HORIZONTAL_SCROLL 0x27
#define VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A
#define DEACTIVATE_SCROLL 0x2E
#define ACTIVATE_SCROLL 0x2F
#define SET_VERTICAL_SCROLL_AREA 0xA3

void setup(void);
void update_screen(void);
void clear_screen(void);
void draw_pixel(unsigned char x, unsigned char y, unsigned char colour);
void command(unsigned char cmd);

#endif
