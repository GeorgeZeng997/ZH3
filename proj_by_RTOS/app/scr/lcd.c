#include "config.h"

lcd_frame_t lcd_frame = {0};
lcd_st_t lcd_st = LCD_ST_IDL;
lcd_st_t lcd_last_st = LCD_ST_IDL;

lcd_msg_st_t lcd_msg_st = LCD_MSG_DRAW_RECT;
lcd_msg_st_t lcd_last_msg;

lcd_char_attr_t lcd_char_attr;
lcd_blk_t lcd_blk;

uint8_t lcd_busy = 0;

lcd_msg_qu_t lcd_msg_qu = {0};

uint16_t data_tmp[24];

lcd_attr_t lcd_pic_attr_custom;
lcd_attr_t lcd_pic_attr_null = {0};

uint8_t lcd_continuously = 0;
uint8_t lcd_brightness_value = LCD_DISPLAY_BRIGHTNESS_MAX;

void pic_data_convert()
{
}

void set_lcd_frame_buf(uint8_t *data, uint16_t length)
{
    if (data == NULL || length == 0 || length > LCD_SEND_FRAME_SIZE)
        return;

    for (uint16_t i = 0; i < length; i++)
    {
        lcd_frame.buf[i] = data[i];
    }
    lcd_frame.length = length;
    lcd_frame.idx = 0;
}

void lcd_write_cmd(uint8_t cmd)
{
    uint8_t data[1] = {cmd};
    gpio_bit_reset(GPIO_LCD_DC_PORT, GPIO_LCD_DC_PIN);

    gpio_bit_reset(GPIO_LCD_PORT, GPIO_LCD_CS_PIN);
    // for (int i = 0; i < 1000; i++);

    set_lcd_frame_buf(data, 1);
}
void lcd_write_data(uint8_t *data, uint32_t data_length)
{
    gpio_bit_set(GPIO_LCD_DC_PORT, GPIO_LCD_DC_PIN);
    gpio_bit_reset(GPIO_LCD_PORT, GPIO_LCD_CS_PIN);
    set_lcd_frame_buf(data, data_length);
}
void lcd_write_addr(uint16_t addr1, uint16_t addr2)
{
    uint8_t Byte[4];
    Byte[0] = (addr1 >> 8) & 0xFF;
    Byte[1] = addr1 & 0xFF;
    Byte[2] = (addr2 >> 8) & 0xFF;
    Byte[3] = addr2 & 0xFF;
    lcd_write_data(Byte, 4);
}
void lcd_write_color(uint16_t *data, uint16_t length)
{
    if (data == NULL || length == 0 || length > LCD_SEND_FRAME_SIZE)
        return;
    uint16_t idx_tmp = 0;
    gpio_bit_set(GPIO_LCD_DC_PORT, GPIO_LCD_DC_PIN);
    gpio_bit_reset(GPIO_LCD_PORT, GPIO_LCD_CS_PIN);
    // lcd_write_data((uint8_t*)&data[0],data_length*2);
    for (uint16_t i = 0; i < length; i++)
    {
        lcd_frame.buf[idx_tmp++] = (data[i] >> 8) & 0xff;
        lcd_frame.buf[idx_tmp++] = data[i] & 0xff;
    }
    lcd_frame.length = length * 2;
    lcd_frame.idx = 0;
}

void lcd_draw_fill_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
}

void lcd_msg_check_in(lcd_msg_st_t msg, uint8_t *data, lcd_attr_t pic)
{

    if (lcd_msg_qu.total >= LCD_MSG_QU_FRAME_SIZE)
    {
        return;
    }
    if (xSemaphoreTake(lcd_sema, portMAX_DELAY) == pdTRUE)
    {
        lcd_msg_qu.frame[lcd_msg_qu.idx_in].data = data;
        lcd_msg_qu.frame[lcd_msg_qu.idx_in].msg = msg;
        lcd_msg_qu.frame[lcd_msg_qu.idx_in].pic = pic;
        if (++lcd_msg_qu.idx_in >= LCD_MSG_QU_FRAME_SIZE)
        {
            lcd_msg_qu.idx_in = 0;
        }
        lcd_msg_qu.total++;
    }
    xSemaphoreGive(lcd_sema);
}

void lcd_msg_check_out()
{
    if (xSemaphoreTake(lcd_sema, portMAX_DELAY) == pdTRUE)
    {
        if (++lcd_msg_qu.idx_out >= LCD_MSG_QU_FRAME_SIZE)
        {
            lcd_msg_qu.idx_out = 0;
        }
        lcd_msg_qu.total--;
    }
    xSemaphoreGive(lcd_sema);
}

void lcd_msg_proc()
{
    switch (lcd_msg_qu.frame[lcd_msg_qu.idx_out].msg)
    {
    case LCD_MSG_INIT:
        lcd_st = LCD_ST_INIT;
        break;

    case LCD_MSG_DRAW_RECT:
        lcd_st = LCD_ST_FLASH;
        break;
    case LCD_MSG_SHOW_CHAR:
        lcd_st = LCD_ST_SHOW_CHAR;
        break;
    case LCD_MSG_SHOW_PICTURE:
        lcd_st = LCD_ST_SHOW_PICTURE;
        break;
    case LCD_MSG_FLASH:
        lcd_msg_qu.frame[lcd_msg_qu.idx_out].pic.width = flash_picture_array[lcd_msg_qu.frame[lcd_msg_qu.idx_out].pic.flash_picture_idx].width;
        lcd_msg_qu.frame[lcd_msg_qu.idx_out].pic.height = flash_picture_array[lcd_msg_qu.frame[lcd_msg_qu.idx_out].pic.flash_picture_idx].height;

        lcd_st = LCD_ST_FLASH;
        break;
    case LCD_MSG_BLK_ON:
        lcd_st = LCD_ST_BLK_ON;
        break;
    case LCD_MSG_BLK_SET_BRIGHTNESS:
        lcd_st = LCD_ST_BLK_BRIGHTNESS;
        break;
    case LCD_MSG_BLK_OFF:
        lcd_st = LCD_ST_BLK_OFF;
    case LCD_MSG_INVAL:

        break;

    default:
        break;
    }
    lcd_last_msg = lcd_msg_qu.frame[lcd_msg_qu.idx_out].msg;
    lcd_blk.data = lcd_msg_qu.frame[lcd_msg_qu.idx_out].data;

    lcd_blk.x = lcd_blk.x_part = lcd_msg_qu.frame[lcd_msg_qu.idx_out].pic.x;
    lcd_blk.y = lcd_blk.y_part = lcd_msg_qu.frame[lcd_msg_qu.idx_out].pic.y;
    lcd_blk.width = lcd_msg_qu.frame[lcd_msg_qu.idx_out].pic.width;
    lcd_blk.height = lcd_msg_qu.frame[lcd_msg_qu.idx_out].pic.height;
    lcd_blk.length = lcd_blk.width * lcd_blk.height * 2;
    lcd_blk.color = lcd_msg_qu.frame[lcd_msg_qu.idx_out].pic.color;
    lcd_blk.idx = lcd_blk.height_last = lcd_blk.height_part = 0;
    lcd_blk.flash_picture_idx = lcd_msg_qu.frame[lcd_msg_qu.idx_out].pic.flash_picture_idx;
    lcd_msg_check_out();
}
void lcd_send()
{
    if (!lcd_busy && lcd_frame.length && !lcd_frame.idx && !spi_i2s_flag_get(SPI1, SPI_FLAG_TRANS))
    {
        lcd_busy = 1;
        gpio_bit_reset(GPIO_LCD_PORT, GPIO_LCD_CS_PIN);
        // spi_i2s_interrupt_enable(SPI1, SPI_I2S_INT_TBE);

        lcd_dma_config();
    }
}
void ST7789_WriteCommand(uint8_t data)
{
    lcd_write_cmd(data);
    lcd_send();
    vTaskDelay(10);
}
void ST7789_WriteData(uint8_t data)
{
    uint8_t data_tmp[1] = {data};
    uint16_t data_length_tmp = 1;
    lcd_write_data(data_tmp, data_length_tmp);
    lcd_send();
    vTaskDelay(10);
}

uint16_t lcd_delay = 0;
uint16_t lcd_delay_cnt = 0;
// 1ms proccess
void lcd_task(void *pvParameters)
{

    // lcd_msg_check_in(LCD_MSG_INIT, NULL, lcd_pic_attr_null);

    // lcd_fill_screen(BLACK);

    // lcd_display_rectangle(0,105,240,40,YELLOW_FOSI);
    // lcd_display_rectangle(0,145,240,95,BLACK);
    // lcd_display_picture(0, 0, FLASH_PICTURE_IDX_BACKGROUND);
    // lcd_init();
    // lcd_display_rectangle(0, 0, 280, 135, GREEN);
    // vTaskDelay(100);
    lcd_msg_check_in(LCD_MSG_INIT, NULL, lcd_pic_attr_null);
    // lcd_display_rectangle(0,0,240,240,RED);
    lcd_display_rectangle(0, 0, 240, 240, BLACK);
    // lcd_display_menu(1);
    // lcd_display_picture(0, 0, FLASH_PICTURE_IDX_BACKGROUND_TEST);
    while (1)
    {

        switch (lcd_st)
        {
        case LCD_ST_IDL:
            if (lcd_msg_qu.total)
            {
                lcd_msg_proc();
            }
            break;

        case LCD_ST_INIT:
            lcd_st = LCD_ST_HARD_CS_INIT;
            break;
        case LCD_ST_HARD_CS_INIT:
            gpio_bit_reset(GPIO_LCD_PORT, GPIO_LCD_CS_PIN);
            lcd_st = LCD_ST_HARD_DC_INIT;
            break;
        case LCD_ST_HARD_DC_INIT:
            gpio_bit_reset(GPIO_LCD_DC_PORT, GPIO_LCD_DC_PIN);
            lcd_st = LCD_ST_HARD_RES_SET1;
            break;
        case LCD_ST_HARD_RES_SET1:
            // gpio_bit_set(GPIO_LCD_PORT, GPIO_LCD_RES_PIN);
            //  lcd_delay = LCD_HARD_RES_SET1_DELAY;
            lcd_st = LCD_ST_HARD_RES_SET1_DELAY;
            break;
        case LCD_ST_HARD_RES_SET1_DELAY:
            vTaskDelay(LCD_HARD_RES_SET1_DELAY);
            // if (lcd_delay == 0)
            // {
            lcd_st = LCD_ST_HARD_RES_RESET;
            // }
            break;
        case LCD_ST_HARD_RES_RESET:
            // gpio_bit_reset(GPIO_LCD_PORT, GPIO_LCD_RES_PIN);
            //  lcd_delay = LCD_HARD_RES_RESET_DELAY;
            lcd_st = LCD_ST_HARD_RES_RESET_DELAY;
            break;
        case LCD_ST_HARD_RES_RESET_DELAY:
            vTaskDelay(LCD_HARD_RES_RESET_DELAY);
            // if (lcd_delay == 0)
            // {
            lcd_st = LCD_ST_HARD_RES_SET2;
            // }
            break;
        case LCD_ST_HARD_RES_SET2:
            // gpio_bit_set(GPIO_LCD_PORT, GPIO_LCD_RES_PIN);
            //  lcd_delay = LCD_HARD_RES_SET2_DELAY;
            lcd_st = LCD_ST_HARD_RES_SET2_DELAY;
            break;
        case LCD_ST_HARD_RES_SET2_DELAY:
            vTaskDelay(LCD_HARD_RES_SET2_DELAY);
            // if (lcd_delay == 0)
            // {
            lcd_st = LCD_ST_HARD_BL_INIT;
            // }
            break;
        case LCD_ST_HARD_BL_INIT:
            // gpio_bit_reset(GPIO_LCD_PORT, GPIO_LCD_BLK_PIN);

            gpio_bit_set(GPIO_LCD_PORT, GPIO_LCD_CS_PIN);
            lcd_st = LCD_ST_SOFT_RESET;
            break;
        case LCD_ST_SOFT_RESET:
            if (lcd_busy)
                break;
            lcd_write_cmd(0x01);
            lcd_st = LCD_ST_SOFT_RESET_DELAY;
            // lcd_delay = LCD_SOFT_RESET_DELAY;
            break;
        case LCD_ST_SOFT_RESET_DELAY:
            vTaskDelay(LCD_SOFT_RESET_DELAY);
            // if (lcd_delay == 0)
            //{
            lcd_st = LCD_ST_SLEEP_OUT;
            //}
            break;
        case LCD_ST_SLEEP_OUT:
            if (lcd_busy)
                break;
            lcd_write_cmd(0x11);
            lcd_st = LCD_ST_SLEEP_OUT_DELAY;
            break;
        case LCD_ST_SLEEP_OUT_DELAY:
            vTaskDelay(LCD_SLEEP_OUT_DELAY);
            lcd_st = LCD_ST_PIXEL_FORMAT_CMD;
            break;
        case LCD_ST_PIXEL_FORMAT_CMD:
            if (lcd_busy)
                break;
            lcd_write_cmd(0x3a);
            lcd_st = LCD_ST_PIXEL_FORMAT_DATA;
            break;
        case LCD_ST_PIXEL_FORMAT_DATA:
            if (lcd_busy)
                break;
            {
                uint8_t data_tmp[1] = {0x05};
                uint16_t data_length_tmp = 1;
                lcd_write_data(data_tmp, data_length_tmp);
                lcd_st = LCD_ST_PIXEL_FORMAT_DELAY;
                // lcd_delay = LCD_PIXEL_FORMAT_DELAY;
            }
            break;
        case LCD_ST_PIXEL_FORMAT_DELAY:
            vTaskDelay(LCD_PIXEL_FORMAT_DELAY);
            lcd_st = LCD_ST_DATA_ACCESS_CMD;
            break;
        case LCD_ST_DATA_ACCESS_CMD:
            if (lcd_busy)
                break;
            lcd_write_cmd(0x36);
            lcd_st = LCD_ST_DATA_ACCESS_DATA;
            break;
        case LCD_ST_DATA_ACCESS_DATA:
            if (lcd_busy)
                break;
            {
                uint8_t data_tmp[1] = {0xA0};
                uint16_t data_length_tmp = 1;
                lcd_write_data(data_tmp, data_length_tmp);
                lcd_st = LCD_ST_COLUMN_ADDR_CMD;
                // lcd_st = LCD_ST_DISPLAY_ON;
            }
            break;
        case LCD_ST_COLUMN_ADDR_CMD:
            if (lcd_busy)
                break;
            lcd_write_cmd(0x2a);
            lcd_st = LCD_ST_COLUMN_ADDR_DATA;
            break;
        case LCD_ST_COLUMN_ADDR_DATA:
            if (lcd_busy)
                break;
            {
                uint8_t data_tmp[4] = {0x00, 0x00, 0x00, 0xf0};
                // uint8_t data_tmp[4] = {0x00, 0x34, 0x00, 0xBA};
                uint32_t data_length = 4;
                lcd_write_data(data_tmp, data_length);
                lcd_st = LCD_ST_ROW_ADDR_CMD;
            }
            break;
        case LCD_ST_ROW_ADDR_CMD:
            if (lcd_busy)
                break;
            lcd_write_cmd(0x2b);
            lcd_st = LCD_ST_ROW_ADDR_DATA;
            break;
        case LCD_ST_ROW_ADDR_DATA:
            if (lcd_busy)
                break;
            {
                uint8_t data_tmp[4] = {0x00, 0x00, 0x00, 0xf0};
                // uint8_t data_tmp[4] = {0x00, 0x28, 0x01, 0x17};
                uint32_t data_length = 4;
                lcd_write_data(data_tmp, data_length);
                lcd_st = LCD_ST_AREA_CONFIRM;
            }
            break;
        case LCD_ST_AREA_CONFIRM:
            if (lcd_busy)
                break;
            lcd_write_cmd(0x2c);
            lcd_st = LCD_ST_DISPLAY_INVERSION;
            break;
        case LCD_ST_DISPLAY_INVERSION:
            if (lcd_busy)
                break;
            lcd_write_cmd(0x21);
            // lcd_delay = LCD_DISPLAY_INVERSION_DELAY;
            lcd_st = LCD_ST_DISPLAY_INVERSION_DELAY;
            break;
        case LCD_ST_DISPLAY_INVERSION_DELAY:
            vTaskDelay(LCD_DISPLAY_INVERSION_DELAY);
            // if (lcd_delay == 0)
            //{
            lcd_st = LCD_ST_PORCH_SETTIGN_CMD;
            //}
            break;
        case LCD_ST_PORCH_SETTIGN_CMD:
            if (lcd_busy)
                break;
            lcd_write_cmd(0xb2);
            lcd_st = LCD_ST_PORCH_SETTING_DATA;
            break;
        case LCD_ST_PORCH_SETTING_DATA:
            if (lcd_busy)
                break;
            {
                uint8_t data_tmp[5] = {0x05, 0x05, 0x00, 0x33, 0x33};
                uint32_t data_length = 5;
                lcd_write_data(data_tmp, data_length);
                lcd_st = LCD_ST_PORCH_SETTING_DELAY;
            }
            break;
        case LCD_ST_PORCH_SETTING_DELAY:
            vTaskDelay(10);
            lcd_st = LCD_ST_EXTRA_SETTING;
            break;
        case LCD_ST_EXTRA_SETTING:
        {
            // ST7789_WriteCommand(0xE0);
            // uint8_t gamma_values_contrast[16] = {
            //     0x10, 0x28, 0x30, 0x0A, 0x14, 0x0A, 0x60, 0xF5,
            //     0x38, 0x08, 0x10, 0x06, 0x0E, 0x0A, 0x02, 0x01};

            // // 发送 Gamma 曲线值
            // for (int i = 0; i < 16; i++)
            // {
            //     ST7789_WriteData(gamma_values_contrast[i]);
            // }
            //         ST7789_WriteCommand(0xE1);

            //         // 设置 Gamma 曲线 2 的值
            //         uint8_t gamma_values_2[16] = {
            //             0x0f, 0x1f, 0x2a, 0x22, 0x11, 0x15, 0x2E, 0x50,
            // 0x56, 0x42, 0x25, 0x16, 0x14, 0x19, 0x1D, 0x1B};

            //         // 发送 Gamma 曲线 2 的值
            //         for (int i = 0; i < 16; i++)
            //         {
            //             ST7789_WriteData(gamma_values_2[i]);
            //         }
            //         ST7789_WriteCommand(0x21);
            //     }
        }
            lcd_st = LCD_ST_DISPLAY_MODE;
            break;

        case LCD_ST_DISPLAY_MODE:
            if (lcd_busy)
                break;
            lcd_write_cmd(0x13);
            // lcd_delay = LCD_DISPLAY_MODE_DELAY;
            lcd_st = LCD_ST_DISPLAY_MODE_DELAY;
            break;
        case LCD_ST_DISPLAY_MODE_DELAY:
            vTaskDelay(LCD_DISPLAY_MODE_DELAY);

            lcd_st = LCD_ST_DISPLAY_ON;
            // lcd_st = LCD_ST_IDL;

            break;
        case LCD_ST_DISPLAY_ON:
            if (lcd_busy)
                break;
            lcd_write_cmd(0x29);
            // lcd_delay = LCD_DISPLAY_ON_DELAY;
            lcd_st = LCD_ST_DISPLAY_ON_DELAY;
            break;
        case LCD_ST_DISPLAY_ON_DELAY:
            vTaskDelay(LCD_DISPLAY_ON_DELAY);

            lcd_st = LCD_ST_IDL;
            // lcd_st = LCD_ST_COLUMN_ADDR_CMD;
            // gpio_bit_set(GPIO_LCD_PORT, GPIO_LCD_BLK_PIN);

            break;
        case LCD_ST_SHOW_CHAR:
            break;
        case LCD_ST_DRAW_RECT:
            lcd_last_st = LCD_ST_DRAW_RECT;
            lcd_st = LCD_ST_SHOW_PICTURE;
            break;

        case LCD_ST_SHOW_PICTURE:
            if (lcd_busy)
                break;
            lcd_blk.height_last += lcd_blk.height_part;
            if ((lcd_blk.length - lcd_blk.idx) >= LCD_SEND_FRAME_SIZE)
            {
                lcd_blk.height_part = LCD_SEND_FRAME_SIZE / lcd_blk.width / 2;
            }
            else
            {
                lcd_blk.height_part = (lcd_blk.length - lcd_blk.idx) / lcd_blk.width / 2;
            }
            lcd_st = LCD_ST_SET_RECT_COLUMN_CMD;
            break;
        case LCD_ST_SET_RECT_COLUMN_CMD:
            if (lcd_busy)
                break;
            lcd_write_cmd(0x2A);
            lcd_st = LCD_ST_SET_RECT_COLUMN_DATA;
            break;
        case LCD_ST_SET_RECT_COLUMN_DATA:
            if (lcd_busy)
                break;
            lcd_write_addr(lcd_blk.x, lcd_blk.x + lcd_blk.width - 1);
            lcd_st = LCD_ST_SET_RECT_ROW_CMD;
            break;
        case LCD_ST_SET_RECT_ROW_CMD:
            if (lcd_busy)
                break;
            lcd_write_cmd(0x2B);
            lcd_st = LCD_ST_SET_RECT_ROW_DATA;
            break;
        case LCD_ST_SET_RECT_ROW_DATA:
            if (lcd_busy)
                break;
            lcd_write_addr(lcd_blk.y + lcd_blk.height_last, lcd_blk.y + lcd_blk.height_last + lcd_blk.height_part - 1);
            lcd_st = LCD_ST_SET_RECT_MEMORY_CMD;
            break;
        case LCD_ST_SET_RECT_MEMORY_CMD:
            if (lcd_busy)
                break;
            lcd_write_cmd(0x2C);
            if (lcd_last_msg == LCD_MSG_SHOW_PICTURE)
            {
                lcd_st = LCD_ST_SET_PICTURE_MEMORY_DATA;
            }
            else if (lcd_last_msg == LCD_MSG_DRAW_RECT)
            {
                lcd_st = LCD_ST_SET_RECT_MEMORY;
            }
            break;
        case LCD_ST_SET_PICTURE_MEMORY_DATA:
            if (lcd_busy)
                break;
            gpio_bit_set(GPIO_LCD_DC_PORT, GPIO_LCD_DC_PIN);
            gpio_bit_reset(GPIO_LCD_PORT, GPIO_LCD_CS_PIN);
            for (uint16_t i = 0; i < lcd_blk.height_part * lcd_blk.width; i++)
            {
                lcd_frame.buf[i * 2 + 1] = lcd_blk.data[lcd_blk.idx++];
                lcd_frame.buf[i * 2] = lcd_blk.data[lcd_blk.idx++];
            }
            lcd_st = LCD_ST_SET_FRAME_LENGTH;
            break;
        case LCD_ST_SET_FRAME_LENGTH:
            if (lcd_busy)
                break;
            lcd_frame.length = lcd_blk.height_part * lcd_blk.width * 2;
            lcd_frame.idx = 0;
            if (lcd_blk.idx >= lcd_blk.length)
            {
                lcd_st = LCD_ST_IDL;
            }
            else
            {
                lcd_st = LCD_ST_SHOW_PICTURE;
            }
            break;

        case LCD_ST_FLASH:
            // flash_msg_check_in(NULL,16000,FLASH_MSG_READ_BACKGROUND);
            lcd_st = LCD_ST_FLASH_COLUMN_CMD;
            break;
        case LCD_ST_FLASH_COLUMN_CMD:
            if (lcd_busy)
                break;
            lcd_write_cmd(0x2A);
            lcd_st = LCD_ST_FLASH_COLUMN_DATA;
            break;
        case LCD_ST_FLASH_COLUMN_DATA:
            if (lcd_busy)
                break;
            lcd_write_addr(lcd_blk.x, lcd_blk.x + lcd_blk.width - 1);
            lcd_st = LCD_ST_FLASH_ROW_CMD;
            break;
        case LCD_ST_FLASH_ROW_CMD:
            if (lcd_busy)
                break;
            lcd_write_cmd(0x2B);
            lcd_st = LCD_ST_FLASH_ROW_DATA;
            break;
        case LCD_ST_FLASH_ROW_DATA:
            if (lcd_busy)
                break;
            lcd_write_addr(lcd_blk.y, lcd_blk.y + lcd_blk.height - 1);
            lcd_st = LCD_ST_FLASH_MEMORY_CMD;
            break;
        case LCD_ST_FLASH_MEMORY_CMD:
            if (lcd_busy)
                break;
            lcd_write_cmd(0x2C);

            if (lcd_last_msg == LCD_MSG_FLASH)
            {
                flash_msg_check_in(NULL, lcd_blk.flash_picture_idx, FLASH_MSG_READ_BACKGROUND);

                lcd_st = LCD_ST_FLASH_MEMORY_DATA;
            }
            else if (lcd_last_msg == LCD_MSG_DRAW_RECT)
            {
                lcd_st = LCD_ST_SET_RECT_MEMORY;
            }
            break;
        case LCD_ST_SET_RECT_MEMORY:
        {
            uint16_t i = 0;
            if (lcd_busy)
                break;
            lcd_continuously = 1;
            gpio_bit_set(GPIO_LCD_DC_PORT, GPIO_LCD_DC_PIN);
            gpio_bit_reset(GPIO_LCD_PORT, GPIO_LCD_CS_PIN);
            for (i = 0; i < LCD_SEND_FRAME_SIZE && lcd_blk.length; i++)
            {
                if (i % 2)
                {
                    lcd_frame.buf[i] = lcd_blk.color;
                }
                else
                {
                    lcd_frame.buf[i] = lcd_blk.color >> 8;
                }
                lcd_blk.length--;
            }

            lcd_frame.length = i;
            lcd_frame.idx = 0;

            if (!lcd_blk.length)
            {
                lcd_st = LCD_ST_FLASH_COMPLETE;
            }
        }
        break;
        case LCD_ST_FLASH_MEMORY_DATA:
        {

            if (lcd_busy)
                break;
            lcd_continuously = 1;
            gpio_bit_set(GPIO_LCD_DC_PORT, GPIO_LCD_DC_PIN);
            gpio_bit_reset(GPIO_LCD_PORT, GPIO_LCD_CS_PIN);
            if (!flash_to_lcd.flash_remaining_length && !flash_to_lcd.flash_rx_dma_cplt)
            {
                if (lcd_blk.width * lcd_blk.height > 60 * 63)
                {
                    vTaskDelay(10);
                }
                else
                {
                    vTaskDelay(5);
                }
                lcd_st = LCD_ST_FLASH_COMPLETE;
            }
        }
        break;
        case LCD_ST_FLASH_COMPLETE:
            if (lcd_busy || spi_i2s_flag_get(SPI0, SPI_FLAG_TRANS))
                // if (lcd_busy)
                break;
            gpio_bit_set(GPIO_LCD_PORT, GPIO_LCD_CS_PIN);
            // vTaskDelay(20);
            lcd_continuously = 0;
            lcd_st = LCD_ST_IDL;
            break;
        case LCD_ST_SET_MEMORY_FINISH_DELAY:
            if (lcd_delay == 0)
            {
                lcd_st = LCD_ST_IDL;
            }
            break;
        case LCD_ST_BLK_ON:
            // lcd_display_brightness(user_data.lcd_bright_value);
            lcd_display_brightness(LCD_DISPLAY_BRIGHTNESS_MAX);
            lcd_st = LCD_ST_IDL;
            break;
        case LCD_ST_BLK_OFF:
            lcd_display_brightness(LCD_DISPLAY_BRIGHTNESS_BLACK);
            lcd_st = LCD_ST_IDL;
            break;
        case LCD_ST_BLK_BRIGHTNESS:

            lcd_st = LCD_ST_SET_BRIGHTNESS_CMD;
            break;
        case LCD_ST_SET_BRIGHTNESS_CMD:
            lcd_write_cmd(0X51);
            lcd_st = LCD_ST_SET_BRIGHTNESS_DATA;
            break;
        case LCD_ST_SET_BRIGHTNESS_DATA:
        {
            uint8_t data_tmp[2] = {lcd_brightness_value};
            lcd_write_data(data_tmp, 1);
        }
            lcd_st = LCD_ST_IDL;
            break;
        default:
            break;
        }
        if (!lcd_busy && lcd_frame.length && !lcd_frame.idx && !spi_i2s_flag_get(SPI0, SPI_FLAG_TRANS))
        {
            lcd_busy = 1;
            gpio_bit_reset(GPIO_LCD_PORT, GPIO_LCD_CS_PIN);
            // spi_i2s_interrupt_enable(SPI1, SPI_I2S_INT_TBE);

            lcd_dma_config();
        }
        vTaskDelay(LCD_TASK_DELAY);
    }
}
void lcd_dma_config()
{
    dma_parameter_struct dma_init_struct;
    dma_channel_disable(DMA0, DMA_CH2);
    // dma_struct_para_init(&dma_init_struct);
    // rcu_periph_clock_enable(RCU_DMA0);
    /* configure SPI0 transmit DMA: DMA_CH2 */
    // dma_deinit(DMA0,DMA_CH4);
    dma_init_struct.periph_addr = (uint32_t)&SPI_DATA(SPI0);
    dma_init_struct.memory_addr = (uint32_t)lcd_frame.buf;
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_init_struct.number = lcd_frame.length;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init(DMA0, DMA_CH2, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH2);
    dma_memory_to_memory_disable(DMA0, DMA_CH2);

    dma_channel_enable(DMA0, DMA_CH2);
    spi_dma_enable(SPI0, SPI_DMA_TRANSMIT);
    spi_enable(SPI0);
}

void lcd_dma_callback()
{
    if (dma_interrupt_flag_get(DMA0, DMA_CH2, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH2, DMA_INT_FLAG_FTF);

        lcd_busy = 0;
        lcd_frame.length = 0;
        // while (spi_i2s_flag_get(SPI1, SPI_FLAG_TRANS))
        // {
        // }
        if (!lcd_continuously)
        {
            gpio_bit_set(GPIO_LCD_PORT, GPIO_LCD_CS_PIN);
        }
        else
        {
            flash_to_lcd.flash_rx_dma_cplt = 0;
        }

        spi_i2s_interrupt_disable(SPI0, SPI_I2S_INT_TBE);
    }
}

void lcd_int_callback()
{
    if (RESET != spi_i2s_interrupt_flag_get(SPI0, SPI_I2S_INT_FLAG_TBE))
    {

        spi_i2s_data_transmit(SPI0, lcd_frame.buf[lcd_frame.idx]);

        if (++lcd_frame.idx >= lcd_frame.length)
        {
            lcd_busy = 0;
            lcd_frame.length = 0;
            while (spi_i2s_flag_get(SPI0, SPI_FLAG_TRANS))
            {
            }
            gpio_bit_set(GPIO_LCD_PORT, GPIO_LCD_CS_PIN);
            spi_i2s_interrupt_disable(SPI0, SPI_I2S_INT_TBE);
        }
        else
        {
        }
    }
}
#define X_BIAS 80 // 0X28
#define Y_BIAS 0  // 0X34
void lcd_display_picture(uint16_t x, uint16_t y, flash_picture_idx_t picture_idx)
{
    lcd_attr_t lcd_attr_tmp;
    lcd_attr_tmp.x = x + X_BIAS;
    lcd_attr_tmp.y = y + Y_BIAS;
    lcd_attr_tmp.flash_picture_idx = picture_idx;
    lcd_msg_check_in(LCD_MSG_FLASH, NULL, lcd_attr_tmp);
}
void lcd_fill_screen(uint16_t color)
{
    lcd_attr_t lcd_attr_tmp;
    lcd_attr_tmp.x = 0 + X_BIAS;
    lcd_attr_tmp.y = 0 + Y_BIAS;
    lcd_attr_tmp.width = LCD_WIDTH;
    lcd_attr_tmp.height = LCD_HEIGHT;
    lcd_attr_tmp.color = color;
    lcd_msg_check_in(LCD_MSG_DRAW_RECT, NULL, lcd_attr_tmp);
}
void lcd_display_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
    lcd_attr_t lcd_attr_tmp;
    lcd_attr_tmp.x = x + X_BIAS;
    lcd_attr_tmp.y = y + Y_BIAS;
    lcd_attr_tmp.width = width;
    lcd_attr_tmp.height = height;
    lcd_attr_tmp.color = color;
    lcd_msg_check_in(LCD_MSG_DRAW_RECT, NULL, lcd_attr_tmp);
}

void lcd_display_earse(uint16_t x, uint16_t y, flash_picture_idx_t picture_idx, uint16_t color)
{
    lcd_display_rectangle(x, y, flash_picture_array[picture_idx].width, flash_picture_array[picture_idx].height, color);
}
int8_t last_volume = 0;
void lcd_display_volume(uint8_t volume)
{
    if ((last_volume >= 10 && volume < 10) || (last_volume < 10 && volume >= 10))
    {
        lcd_display_rectangle(COORD_NUM_TENS_X, COORD_NUM_TENS_Y, 100, 64, DAKE_FOSI);
    }
    last_volume = volume;
    // lcd_display_rectangle(COORD_NUM_TENS_X, COORD_NUM_TENS_Y ,80 , 47, DAKE_FOSI);
    uint8_t ones_num = volume % 10;
    uint8_t tens_num = volume / 10;
    if (tens_num > 0)
    {
        lcd_display_picture(COORD_NUM_ONES_X, COORD_NUM_ONES_Y, FLASH_PICTURE_IDX_NUM_0 + ones_num);
        lcd_display_picture(COORD_NUM_TENS_X, COORD_NUM_TENS_Y, FLASH_PICTURE_IDX_NUM_0 + tens_num);
    }
    else
    {
        lcd_display_picture(COORD_NUM_MID_X, COORD_NUM_ONES_Y, FLASH_PICTURE_IDX_NUM_0 + ones_num);
    }
}
int8_t last_bass_volume = 0;
int8_t last_treble_volume = 0;
void lcd_display_bass_dbnum(int8_t volume)
{
    if ((last_bass_volume >= 10 && volume < 10) || (last_bass_volume < 10 && volume >= 10) ||
        (last_bass_volume < 0 && volume >= 0) || (last_bass_volume > -10 && volume <= -10) ||
        (last_bass_volume <= -10 && volume > -10))
    {
        lcd_display_picture(COORD_DB_X, COORD_DB_Y, FLASH_PICTURE_IDX_dB);
    }
    last_bass_volume = volume;
    if (volume < 0)
    {
        volume = -volume;
        lcd_display_picture(COORD_MINUS_X, COORD_MINUS_Y, FLASH_PICTURE_IDX_MINUS);
    }
    else if (last_bass_volume < 0)
    {
        lcd_display_rectangle(COORD_MINUS_X, COORD_MINUS_Y, 20, 30, DAKE_FOSI);
    }
    // lcd_display_rectangle(COORD_NUM_TENS_X, COORD_NUM_TENS_Y ,80 , 47, DAKE_FOSI);
    uint8_t ones_num = volume % 10;
    uint8_t tens_num = volume / 10;
    if (tens_num > 0)
    {
        lcd_display_picture(COORD_LIT_NUM_ONES_X, COORD_LIT_NUM_ONES_Y, FLASH_PICTURE_IDX_LIT_NUM_0 + ones_num);
        lcd_display_picture(COORD_LIT_NUM_TENS_X, COORD_LIT_NUM_TENS_Y, FLASH_PICTURE_IDX_LIT_NUM_0 + tens_num);
    }
    else
    {
        lcd_display_picture(COORD_LIT_NUM_MID_X, COORD_LIT_NUM_ONES_Y, FLASH_PICTURE_IDX_LIT_NUM_0 + ones_num);
    }
}
void lcd_display_treble_dbnum(int8_t volume)
{
    if ((last_treble_volume >= 10 && volume < 10) || (last_treble_volume < 10 && volume >= 10) ||
        (last_treble_volume < 0 && volume >= 0) || (last_treble_volume > -10 && volume <= -10) ||
        (last_treble_volume <= -10 && volume > -10))
    {
        lcd_display_picture(COORD_DB_X, COORD_DB_Y, FLASH_PICTURE_IDX_dB);
    }
    last_treble_volume = volume;
    if (volume < 0)
    {
        volume = -volume;
        lcd_display_picture(COORD_MINUS_X, COORD_MINUS_Y, FLASH_PICTURE_IDX_MINUS);
    }
    else if (last_treble_volume < 0)
    {
        lcd_display_rectangle(COORD_MINUS_X, COORD_MINUS_Y, 20, 30, DAKE_FOSI);
    }
    // lcd_display_rectangle(COORD_NUM_TENS_X, COORD_NUM_TENS_Y ,80 , 47, DAKE_FOSI);
    uint8_t ones_num = volume % 10;
    uint8_t tens_num = volume / 10;
    if (tens_num > 0)
    {
        lcd_display_picture(COORD_LIT_NUM_ONES_X, COORD_LIT_NUM_ONES_Y, FLASH_PICTURE_IDX_LIT_NUM_0 + ones_num);
        lcd_display_picture(COORD_LIT_NUM_TENS_X, COORD_LIT_NUM_TENS_Y, FLASH_PICTURE_IDX_LIT_NUM_0 + tens_num);
    }
    else
    {
        lcd_display_picture(COORD_LIT_NUM_MID_X, COORD_LIT_NUM_ONES_Y, FLASH_PICTURE_IDX_LIT_NUM_0 + ones_num);
    }
}
void lcd_display_mute()
{
    if (menu_misc.mute)
    {
        lcd_display_rectangle(COORD_NUM_TENS_X, COORD_NUM_TENS_Y, 100, 64, DAKE_FOSI);
        lcd_display_picture(COORD_MUTE_X, COORD_MUTE_Y, FLASH_PICTURE_IDX_MUTE);
    }
    else
    {

        lcd_display_rectangle(COORD_NUM_TENS_X, COORD_NUM_TENS_Y, 100, 64, DAKE_FOSI);
        lcd_display_volume(user_data.ph_volume);
    }
}
void lcd_display_menu(uint8_t menu_idx)
{
    user_data_value();
    switch (menu_idx)
    {
    case 0:
        // lcd_display_brightness(LCD_DISPLAY_BRIGHTNESS_BLACK);
        // lcd_fill_screen(BLACK);
        menu0_init();
        
        break;
    case 1:
        menu1_init();
        break;
    case 2:
        menu2_init();
        break;
    case 3:
        menu3_init();
        break;
    case 4:
        menu4_init();
        break;
    case 5:
        menu5_init();
    default:
        break;
    }
}

void lcd_display_brightness(uint16_t bri_value)
{
    if (bri_value > LCD_DISPLAY_BRIGHTNESS_MAX)
    {
        return;
    }
    // lcd_msg_check_in(LCD_MSG_BLK_SET_BRIGHTNESS,NULL,lcd_pic_attr_null);
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_3, bri_value);
    timer_auto_reload_shadow_enable(TIMER0);
}
void menu0_init()
{
    lcd_display_rectangle(0, 0, 240, 240, DAKE_FOSI);
    lcd_display_picture(38, 99, FLASH_PICTURE_IDX_OFF);
    for (int8_t i = 15; i >= 0; i--)
    {
        lcd_display_brightness(i);
        vTaskDelay(100);
    }
    lcd_display_rectangle(0, 0, 240, 240, DAKE_FOSI);
    
}
void menu1_init()
{
    lcd_display_picture(0, 0, FLASH_PICTURE_IDX_MENU_1);
    lcd_display_menu1_update();
}
void menu2_init()
{
    lcd_display_picture(0, 0, FLASH_PICTURE_IDX_MENU_2);
    lcd_display_menu2_update();
}
void menu3_init()
{
    lcd_display_picture(0, 0, FLASH_PICTURE_IDX_MENU_3);
    lcd_display_menu3_update();
}
void menu4_init()
{
    // lcd_display_rectangle(0, 0, 240, 240, DAKE_FOSI);
    lcd_display_picture(0, 0, FLASH_PICTURE_IDX_MENU_4);
    lcd_display_picture(38, 40, FLASH_PICTURE_IDX_LCD_BRIGHTNESS);

    lcd_display_menu4_lcd_brightness_update();
}
void menu5_init()
{
    // lcd_display_rectangle(0, 0, 240, 240, DAKE_FOSI);
    lcd_display_picture(0, 0, FLASH_PICTURE_IDX_MENU_5);
    lcd_display_picture(38, 40, FLASH_PICTURE_IDX_AUTO);

    lcd_display_menu5_auto_update();
}
void lcd_display_menu2_fun()
{
}

void lcd_display_menu2_mode()
{
}

void lcd_display_menu1_update()
{
    switch (user_data.pcm)
    {
    case PCM_ST_44:
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_PCM_44);
        break;
    case PCM_ST_48:
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_PCM_48);
        break;
    case PCM_ST_88:
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_PCM_88);
        break;
    case PCM_ST_96:
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_PCM_96);
        break;
    case PCM_ST_176:
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_PCM_176);
        break;
    case PCM_ST_192:
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_PCM_192);
        break;
    case PCM_ST_352:
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_PCM_352);
        break;
    case PCM_ST_384:
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_PCM_384);
        break;
    case PCM_ST_705:
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_PCM_705);
        break;
    case PCM_ST_768:
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_PCM_768);
        break;
    case DSD_ST_64:
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_DSD_64);
        break;
    case DSD_ST_128:
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_DSD_128);
        break;
    case DSD_ST_256:
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_DSD_256);
        break;
    case DSD_ST_512:
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_DSD_512);
        break;
    default:
        break;
    }

    switch (user_data.input)
    {
    case INPUT_ST_COA:
        lcd_display_picture(COORD_TOP_RIGHT_X, COORD_TOP_RIGHT_Y, FLASH_PICTURE_IDX_COAX);
        break;
    case INPUT_ST_OPT:
        lcd_display_picture(COORD_TOP_RIGHT_X, COORD_TOP_RIGHT_Y, FLASH_PICTURE_IDX_OPT);
        break;
    case INPUT_ST_USB:
        lcd_display_picture(COORD_TOP_RIGHT_X, COORD_TOP_RIGHT_Y, FLASH_PICTURE_IDX_USB);
        break;
    case INPUT_ST_RCA:
        lcd_display_picture(COORD_TOP_RIGHT_X, COORD_TOP_RIGHT_Y, FLASH_PICTURE_IDX_RCA);
        break;
    default:
        break;
    }

    switch (user_data.output)
    {
    case OUTPUT_ST_R_X:
        lcd_display_picture(COORD_BOT_LEFT_X, COORD_BOT_LEFT_Y, FLASH_PICTURE_IDX_R_X);
        break;
    case OUTPUT_ST_LO:
        lcd_display_picture(COORD_BOT_LEFT_X, COORD_BOT_LEFT_Y, FLASH_PICTURE_IDX_LO);
        break;
    case OUTPUT_ST_PH:
        lcd_display_picture(COORD_BOT_LEFT_X, COORD_BOT_LEFT_Y, FLASH_PICTURE_IDX_PO);
        break;
    default:
        break;
    }
    switch (user_data.tone)
    {
    case TONE_ST_NO:
        /* code */
        break;
    case TONE_ST_YES:
        lcd_display_picture(COORD_BOT_RIGHT_X, COORD_BOT_RIGHT_Y, FLASH_PICTURE_IDX_TONE_2);
        break;
    default:
        break;
    }

    lcd_display_volume(user_data.ph_volume);
}

void lcd_display_menu2_input_update();
void lcd_display_menu2_output_update();
void lcd_display_menu2_bass_update();
void lcd_display_menu2_treble_update();
void lcd_display_menu2_update()
{
    if (!menu_misc.mode_f)
    {
        lcd_display_picture(0, 0, FLASH_PICTURE_IDX_MENU_2);
        switch (user_data.fun_st)
        {
        case 0:
            lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_INPUT);
            lcd_display_menu2_input_update();
            break;
        case 1:
            lcd_display_picture(COORD_TOP_RIGHT_X, COORD_TOP_RIGHT_Y, FLASH_PICTURE_IDX_OUTPUT);
            lcd_display_menu2_output_update();
            break;
        case 2:
            lcd_display_picture(COORD_BOT_RIGHT_X, COORD_BOT_RIGHT_Y, FLASH_PICTURE_IDX_BASS);
            lcd_display_menu2_bass_update();
            break;
        case 3:
            lcd_display_picture(COORD_BOT_LEFT_X, COORD_BOT_LEFT_Y, FLASH_PICTURE_IDX_TREBLE);
            lcd_display_menu2_treble_update();
        default:
            break;
        }
    }
    else
    {
        switch (user_data.fun_st)
        {
        case 0:
            lcd_display_menu2_input_update();
            break;
        case 1:
            lcd_display_menu2_output_update();
            break;
        case 2:
            lcd_display_menu2_bass_update();
            break;
        case 3:
            lcd_display_menu2_treble_update();
        default:
            break;
        }
    }
}

void lcd_display_menu2_input_update()
{
    flash_picture_idx_t flash_picture_idx;
    if (menu_misc.mode_f)
    {
        flash_picture_idx = FLASH_PICTURE_IDX_INP_COAX + user_data.input;
    }
    else
    {
        flash_picture_idx = FLASH_PICTURE_IDX_INP_COAX_NA + user_data.input;
    }
    lcd_display_picture(COORD_OPTION_X, COORD_OPTION_Y, flash_picture_idx);
}
void lcd_display_menu2_output_update()
{
    flash_picture_idx_t flash_picture_idx;
    if (menu_misc.mode_f)
    {
        flash_picture_idx = FLASH_PICTURE_IDX_OUT_PO + user_data.output;
    }
    else
    {
        flash_picture_idx = FLASH_PICTURE_IDX_OUT_PO_NA + user_data.output;
    }
    lcd_display_picture(COORD_OPTION_X, COORD_OPTION_Y, flash_picture_idx);
}
void lcd_display_menu2_bass_update()
{
    static uint8_t bass_last_mode_f = 0;

    if (!menu_misc.mode_f)
    {
        lcd_display_picture(COORD_DB_X, COORD_DB_Y, FLASH_PICTURE_IDX_dB_NA);
    }
    if (!bass_last_mode_f && menu_misc.mode_f)
    {
        lcd_display_picture(COORD_DB_X, COORD_DB_Y, FLASH_PICTURE_IDX_dB);
    }
    bass_last_mode_f = menu_misc.mode_f;
    lcd_display_bass_dbnum(user_data.bass);
}

void lcd_display_menu2_treble_update()
{
    static uint8_t treble_last_mode_f = 0;
    if (!menu_misc.mode_f)
    {
        lcd_display_picture(COORD_DB_X, COORD_DB_Y, FLASH_PICTURE_IDX_dB_NA);
    }
    if (!treble_last_mode_f && menu_misc.mode_f)
    {
        lcd_display_picture(COORD_DB_X, COORD_DB_Y, FLASH_PICTURE_IDX_dB);
    }
    treble_last_mode_f = menu_misc.mode_f;
    lcd_display_treble_dbnum(user_data.treble);
}

void lcd_display_menu3_gain_update();
void lcd_display_menu3_uac_update();
void lcd_display_menu3_filt_update();
void lcd_display_menu3_tone_update();
void lcd_display_menu3_update()
{
    static fun_st_t last_fun_st = -1;
    if (!menu_misc.mode_f)
    {
        if (last_fun_st != user_data.fun_st || last_fun_st != FUN_ST_TONE)
        {
            lcd_display_picture(0, 0, FLASH_PICTURE_IDX_MENU_3);
            switch (user_data.fun_st)
            {
            case 4:
                lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_GAIN);
                lcd_display_menu3_gain_update();
                break;
            case 5:
                lcd_display_picture(COORD_TOP_RIGHT_X, COORD_TOP_RIGHT_Y, FLASH_PICTURE_IDX_UAC);
                lcd_display_menu3_uac_update();
                break;
            case 6:
                lcd_display_picture(COORD_BOT_RIGHT_X, COORD_BOT_RIGHT_Y, FLASH_PICTURE_IDX_FILT);
                lcd_display_menu3_filt_update();
                break;
            case 7:
                lcd_display_picture(COORD_BOT_LEFT_X, COORD_BOT_LEFT_Y, FLASH_PICTURE_IDX_MENU3_TONE);
                lcd_display_menu3_tone_update();
            default:
                break;
            }
        }
        last_fun_st = user_data.fun_st;
    }
    else
    {
        switch (user_data.fun_st)
        {
        case 4:
            lcd_display_menu3_gain_update();
            break;
        case 5:
            lcd_display_menu3_uac_update();
            break;
        case 6:
            lcd_display_menu3_filt_update();
            break;
        case 7:
            lcd_display_menu3_tone_update();
        default:
            break;
        }
    }
}
void lcd_display_menu3_gain_update()
{
    flash_picture_idx_t flash_picture_idx;
    if (menu_misc.mode_f)
    {
        flash_picture_idx = FLASH_PICTURE_IDX_GAIN_L + user_data.gain;
    }
    else
    {
        flash_picture_idx = FLASH_PICTURE_IDX_GAIN_L_NA + user_data.gain;
    }
    lcd_display_picture(COORD_OPTION_X, COORD_OPTION_Y, flash_picture_idx);
}
void lcd_display_menu3_uac_update()
{
    flash_picture_idx_t flash_picture_idx;
    if (menu_misc.mode_f)
    {
        flash_picture_idx = FLASH_PICTURE_IDX_UAC_1 + user_data.uac;
    }
    else
    {
        flash_picture_idx = FLASH_PICTURE_IDX_UAC_1_NA + user_data.uac;
    }
    lcd_display_picture(COORD_OPTION_X, COORD_OPTION_Y, flash_picture_idx);
}
void lcd_display_menu3_filt_update()
{
    flash_picture_idx_t flash_picture_idx;
    if (menu_misc.mode_f)
    {
        flash_picture_idx = FLASH_PICTURE_IDX_FILT_0 + user_data.filter;
    }
    else
    {
        flash_picture_idx = FLASH_PICTURE_IDX_FILT_0_NA + user_data.filter;
    }
    lcd_display_picture(COORD_OPTION_X, COORD_OPTION_Y, flash_picture_idx);
}

void lcd_display_menu3_tone_update()
{
    flash_picture_idx_t flash_picture_idx;
    if (menu_misc.mode_f)
    {
        flash_picture_idx = FLASH_PICTURE_IDX_TONE_NO + user_data.tone;
    }
    else
    {
        flash_picture_idx = FLASH_PICTURE_IDX_TONE_NO_NA + user_data.tone;
    }
    lcd_display_picture(COORD_OPTION_X, COORD_OPTION_Y, flash_picture_idx);
}

void lcd_display_menu4_lcd_brightness_update()
{
    flash_picture_idx_t flash_picture_idx;
    flash_picture_idx = FLASH_PICTURE_IDX_2MIN + user_data.lcd_bright;
    lcd_display_picture(COORD_OPTION_X, COORD_OPTION_Y, flash_picture_idx);
    switch (user_data.lcd_bright)
    {
    case 0:
        user_data.lcd_bright_value = LCD_DISPLAY_BRIGHTNESS_LEVEL4;
        lcd_display_brightness(LCD_DISPLAY_BRIGHTNESS_LEVEL4);
        break;
    case 1:
        user_data.lcd_bright_value = LCD_DISPLAY_BRIGHTNESS_LEVEL2;
        lcd_display_brightness(LCD_DISPLAY_BRIGHTNESS_LEVEL2);
        break;
    case 2:
        user_data.lcd_bright_value = LCD_DISPLAY_BRIGHTNESS_LEVEL3;
        lcd_display_brightness(LCD_DISPLAY_BRIGHTNESS_LEVEL3);
        break;
    case 3:
        user_data.lcd_bright_value = LCD_DISPLAY_BRIGHTNESS_LEVEL4;
        lcd_display_brightness(LCD_DISPLAY_BRIGHTNESS_LEVEL4);
        break;
    case 4:
        user_data.lcd_bright_value = LCD_DISPLAY_BRIGHTNESS_MAX;
        lcd_display_brightness(LCD_DISPLAY_BRIGHTNESS_MAX);
        break;
    default:
        break;
    }
}

void lcd_display_menu5_auto_update()
{
    flash_picture_idx_t flash_picture_idx;
    flash_picture_idx = FLASH_PICTURE_IDX_NO + user_data.auto_st;
    lcd_display_picture(COORD_OPTION_X, COORD_OPTION_Y, flash_picture_idx);
}
