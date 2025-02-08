#ifndef __ST_7789_H
#define __ST_7789_H

#include "stdint.h"



#define LCD_WIDTH 240
#define LCD_HEIGHT 135//240

#define rgb565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))

#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0

#define BROWN 0XBC40 
#define BRRED 0XFC07 
#define GRAY 0X8430 
#define GRAY0 0xEF7D
#define GRAY1 0x8410 
#define GRAY2 0x4208


#define DARKBLUE 0X01CF  
#define LIGHTBLUE 0X7D7C 
#define GRAYBLUE 0X5458  


#define LIGHTGREEN 0X841F 
#define LIGHTGRAY 0XEF5B  
#define LGRAY 0XC618      

#define LGRAYBLUE 0XA651 
#define LBBLUE 0X2B12   

#define YELLOW_FOSI 0xe382
#define DAKE_FOSI 0X0000

typedef enum {DIRECTION0, DIRECTION90, DIRECTION180, DIRECTION270} DIRECTION;

typedef enum {
	SCROLL_RIGHT = 1,
	SCROLL_LEFT = 2,
	SCROLL_DOWN = 3,
	SCROLL_UP = 4,
} SCROLL_TYPE_t;

typedef struct
{
    uint16_t width;
    uint16_t height;
    uint16_t offsetx;
    uint16_t offsety;
    uint16_t font_direction;
    uint16_t font_fill;
    uint16_t font_fill_color;
    uint16_t font_underline;
    uint16_t font_underline_color;
    int16_t dc;
    int16_t bl;

    uint8_t use_frame_buffer;
    uint16_t *frame_buffer;
} tft_t;



#endif

