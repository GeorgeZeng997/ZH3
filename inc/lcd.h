#ifndef __LCD_H
#define __LCD_H

#include "stdint.h"
#include "flash.h"

#define LCD_SEND_FRAME_SIZE (10 * 1024)

#define LCD_SEND_FRAME_MAX_PIC_SIZE (LCD_SEND_FRAME_SIZE / 2)

#define LCD_HARD_RES_SET1_DELAY 100
#define LCD_HARD_RES_RESET_DELAY 100
#define LCD_HARD_RES_SET2_DELAY 100

#define LCD_SOFT_RESET_DELAY 150
#define LCD_SLEEP_OUT_DELAY 255
#define LCD_PIXEL_FORMAT_DELAY 10
#define LCD_DISPLAY_INVERSION_DELAY 10
#define LCD_DISPLAY_MODE_DELAY 10
#define LCD_DISPLAY_ON_DELAY 255

#define LCD_MSG_QU_FRAME_SIZE 64

#define LCD_STRING_POSITION_BT 38
#define LCD_STRING_POSITION_HPF LCD_STRING_POSITION_BT + 38
#define LCD_STRING_POSITION_TONE LCD_STRING_POSITION_HPF + 46
#define LCD_STRING_POSITION_VOL LCD_STRING_POSITION_TONE + 56

typedef enum
{
    LCD_ST_IDL,

    LCD_ST_INIT,
    LCD_ST_HARD_CS_INIT,
    LCD_ST_HARD_DC_INIT,
    LCD_ST_HARD_RES_SET1,
    LCD_ST_HARD_RES_SET1_DELAY,
    LCD_ST_HARD_RES_RESET,
    LCD_ST_HARD_RES_RESET_DELAY,
    LCD_ST_HARD_RES_SET2,
    LCD_ST_HARD_RES_SET2_DELAY,
    LCD_ST_HARD_BL_INIT,

    LCD_ST_SOFT_RESET,
    LCD_ST_SOFT_RESET_DELAY,
    LCD_ST_SLEEP_OUT,
    LCD_ST_SLEEP_OUT_DELAY,
    LCD_ST_PIXEL_FORMAT_CMD,
    LCD_ST_PIXEL_FORMAT_DATA,
    LCD_ST_PIXEL_FORMAT_DELAY,
    LCD_ST_DATA_ACCESS_CMD,
    LCD_ST_DATA_ACCESS_DATA,
    LCD_ST_COLUMN_ADDR_CMD,
    LCD_ST_COLUMN_ADDR_DATA,
    LCD_ST_ROW_ADDR_CMD,
    LCD_ST_ROW_ADDR_DATA,
    LCD_ST_AREA_CONFIRM,
    LCD_ST_DISPLAY_INVERSION,
    LCD_ST_DISPLAY_INVERSION_DELAY,
    LCD_ST_PORCH_SETTIGN_CMD,
    LCD_ST_PORCH_SETTING_DATA,
    LCD_ST_PORCH_SETTING_DELAY,

    LCD_ST_EXTRA_SETTING,

    LCD_ST_DISPLAY_MODE,
    LCD_ST_DISPLAY_MODE_DELAY,
    LCD_ST_DISPLAY_ON,
    LCD_ST_DISPLAY_ON_DELAY,

    LCD_ST_SHOW_CHAR,

    LCD_ST_DRAW_RECT,
    LCD_ST_SHOW_PICTURE,
    LCD_ST_SET_RECT_COLUMN_CMD,
    LCD_ST_SET_RECT_COLUMN_DATA,
    LCD_ST_SET_RECT_ROW_CMD,
    LCD_ST_SET_RECT_ROW_DATA,
    LCD_ST_SET_RECT_MEMORY_CMD,
    LCD_ST_SET_PICTURE_MEMORY_DATA,
    LCD_ST_SET_RECT_MEMORY,

    LCD_ST_SET_FRAME_LENGTH,

    LCD_ST_FLASH,
    LCD_ST_DRAW_FROM_FLASH,
    LCD_ST_FLASH_COLUMN_CMD,
    LCD_ST_FLASH_COLUMN_DATA,
    LCD_ST_FLASH_ROW_CMD,
    LCD_ST_FLASH_ROW_DATA,
    LCD_ST_FLASH_MEMORY_CMD,
    LCD_ST_FLASH_MEMORY_DATA,
    LCD_ST_FLASH_COMPLETE,

    LCD_ST_BLK_OFF,
    LCD_ST_BLK_ON,

    LCD_ST_BLK_BRIGHTNESS,
    LCD_ST_SET_BRIGHTNESS_CMD,
    LCD_ST_SET_BRIGHTNESS_DATA,

    LCD_ST_SET_MEMORY_FINISH_DELAY,
} lcd_st_t;

typedef enum
{
    LCD_MSG_INIT,
    LCD_MSG_DRAW_RECT,

    LCD_MSG_SHOW_CHAR,
    LCD_MSG_SHOW_CHAR_COLOR,
    LCD_MSG_SHOW_CHAR_CN,

    LCD_MSG_SHOW_PICTURE,
    LCD_MSG_SHOW_POING,

    LCD_MSG_FLASH,
    
    LCD_MSG_BLK_OFF,
    LCD_MSG_BLK_ON,
    LCD_MSG_BLK_SET_BRIGHTNESS,

    LCD_MSG_INVAL,
} lcd_msg_st_t;



typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    uint16_t color;
    flash_picture_idx_t flash_picture_idx;
} lcd_attr_t;

typedef struct
{
    lcd_msg_st_t msg;
    uint8_t *data;
    uint32_t length;
    lcd_attr_t pic;
} lcd_msg_frame_t;

typedef struct
{
    lcd_msg_frame_t frame[LCD_MSG_QU_FRAME_SIZE];
    uint16_t idx_in;
    uint16_t idx_out;
    uint16_t total;
} lcd_msg_qu_t;

typedef struct
{
    uint16_t color;
    uint16_t background_color;
    uint16_t start_x;
    uint16_t start_y;
    uint16_t x;
    uint16_t y;
    uint8_t (*str)[16];
} lcd_char_attr_t;

typedef struct
{
    uint8_t *data;
    uint32_t length;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;

    uint32_t idx;
    uint16_t x_part;
    uint16_t y_part;
    uint16_t height_last;
    uint16_t height_part;
    uint16_t color;

    flash_picture_idx_t flash_picture_idx;
} lcd_blk_t;

typedef struct
{
    uint8_t buf[LCD_SEND_FRAME_SIZE];
    uint16_t length;
    uint16_t idx;
} lcd_frame_t;

typedef struct
{
    lcd_frame_t lcd_frame;
    uint16_t idx_in;
    uint16_t idx_out;
    uint16_t total;
} lcd_send_t;


extern lcd_st_t lcd_st;
extern uint16_t lcd_delay;
extern lcd_frame_t lcd_frame;
extern uint8_t lcd_busy;
extern lcd_msg_st_t lcd_msg_st;
extern lcd_attr_t lcd_pic_attr_null;
extern lcd_attr_t lcd_pic_attr_custom;
extern uint8_t lcd_continuously;
extern uint8_t lcd_brightness_value;

void lcd_task(void *pvParameters);

void lcd_msg_check_in(lcd_msg_st_t msg, uint8_t *data, lcd_attr_t pic);
void lcd_int_callback(void);
void lcd_dma_config(void);
void lcd_dma_callback(void);

void lcd_display_picture(uint16_t x,uint16_t y,flash_picture_idx_t picture_idx);
void lcd_fill_screen(uint16_t color);
void lcd_display_rectangle(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t color);
void lcd_display_earse(uint16_t x,uint16_t y,flash_picture_idx_t picture_idx,uint16_t color);
void lcd_display_volume(uint8_t volume);
void lcd_display_bass_dbnum(int8_t volume);
void lcd_display_treble_dbnum(int8_t volume);
void lcd_display_mute(void);
void lcd_display_menu(uint8_t menu_idx);
void lcd_display_brightness(uint16_t bri_value);
void menu0_init(void);
void menu1_init(void);
void menu2_init(void);
void menu3_init(void);
void menu4_init(void);
void menu5_init(void);
void lcd_display_menu2_fun(void);
void lcd_display_menu2_mode(void);

void lcd_display_menu1_update(void);
void lcd_display_menu2_update(void);
void lcd_display_menu3_update(void);


void lcd_display_menu2_input_update(void);
void lcd_display_menu2_output_update(void);
void lcd_display_menu2_bass_update(void);
void lcd_display_menu2_treble_update(void);

void lcd_display_menu3_gain_update(void);
void lcd_display_menu3_uac_update(void);
void lcd_display_menu3_filt_update(void);
void lcd_display_menu3_tone_update(void);

void lcd_display_menu4_lcd_brightness_update(void);
void lcd_display_menu5_auto_update(void);
#endif
