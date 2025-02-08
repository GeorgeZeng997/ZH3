#include "config.h"

encoder_st_t encoder_st = ENCODER_ST_IDL;

encoder_blk_t encoder_blk;

int8_t encoder_cnt = 0;

key_st_t encoder_key_st = KEY_ST_IDL;
// 1ms process
void encoder_task(void *p)
{

    while (1)
    {
        static uint8_t lcd_enc_test=FLASH_PICTURE_IDX_NUM_0;
        encoder_blk.encoder_pin_a = gpio_input_bit_get(GPIO_ENCODER_PORT, GPIO_ENCODER_PIN_A);
        encoder_blk.encoder_pin_b = gpio_input_bit_get(GPIO_ENCODER_PORT, GPIO_ENCODER_PIN_B);
        switch (encoder_st)
        {
        case ENCODER_ST_IDL:
            if (!encoder_blk.encoder_pin_a && encoder_blk.encoder_pin_b)
            {
                encoder_st = ENCODER_ST_FWD1;
            }
            if (encoder_blk.encoder_pin_a && !encoder_blk.encoder_pin_b)
            {
                encoder_st = ENCODER_ST_REV1;
            }
            break;
        case ENCODER_ST_FWD1:
            if (!encoder_blk.encoder_pin_a && !encoder_blk.encoder_pin_b)
            {
                encoder_st = ENCODER_ST_FWD2;
            }
            if (encoder_blk.encoder_pin_a && encoder_blk.encoder_pin_b)
            {
                encoder_st = ENCODER_ST_IDL;
            }
            break;
        case ENCODER_ST_FWD2:
            if (encoder_blk.encoder_pin_a && !encoder_blk.encoder_pin_b)
            {
                encoder_st = ENCODER_ST_RLS_FWD;
            }
            if (encoder_blk.encoder_pin_a && encoder_blk.encoder_pin_b)
            {
                encoder_st = ENCODER_ST_IDL;
            }
            break;

        case ENCODER_ST_REV1:
            if (!encoder_blk.encoder_pin_a && !encoder_blk.encoder_pin_b)
            {
                encoder_st = ENCODER_ST_REV2;
            }
            if (encoder_blk.encoder_pin_a && encoder_blk.encoder_pin_b)
            {
                encoder_st = ENCODER_ST_IDL;
            }
            break;
        case ENCODER_ST_REV2:
            if (!encoder_blk.encoder_pin_a && encoder_blk.encoder_pin_b)
            {
                encoder_st = ENCODER_ST_RLS_REV;
            }
            if (encoder_blk.encoder_pin_a && encoder_blk.encoder_pin_b)
            {
                encoder_st = ENCODER_ST_IDL;
            }
            break;
        case ENCODER_ST_RLS_REV:
            if (encoder_blk.encoder_pin_a && encoder_blk.encoder_pin_b)
            {
                encoder_cnt--;
                encoder_st = ENCODER_ST_IDL;

                if (encoder_key_st == KEY_ST_PRESS || encoder_key_st == KEY_ST_RELEASE)
                {

                    encoder_key_st = KEY_ST_RELEASE;
                }
                else
                {
                    // lcd_display_picture(0,0,FLASH_PICTURE_IDX_MENU_2);
                    // lcd_display_picture(COORD_TOP_RIGHT_X,COORD_TOP_RIGHT_Y,FLASH_PICTURE_IDX_OUTPUT);
                    // lcd_display_picture(COORD_OPTION_X,COORD_OPTION_Y,FLASH_PICTURE_IDX_OUT_PO);
                    // lcd_display_picture(COORD_DOT1_POS_X,COORD_DOT1_POS_Y,FLASH_PICTURE_IDX_DOT_ORANGE);
                    // lcd_display_picture(COORD_DOT2_POS_X,COORD_DOT2_POS_Y,FLASH_PICTURE_IDX_DOT_GRAY);
                    //lcd_display_picture(COORD_TOP_RIGHT_X,COORD_TOP_RIGHT_Y,lcd_enc_test++);
                    menu_msg_check_in(MENU_MSG_REV);
                    encoder_key_st = KEY_ST_RELEASE;
                }
            }
            break;
        case ENCODER_ST_RLS_FWD:
            if (encoder_blk.encoder_pin_a && encoder_blk.encoder_pin_b)
            {
                encoder_cnt++;
                encoder_st = ENCODER_ST_IDL;

                if (encoder_key_st == KEY_ST_PRESS || encoder_key_st == KEY_ST_RELEASE)
                {
                }
                else
                {
                    // lcd_display_picture(0,0,FLASH_PICTURE_IDX_MENU_1);
                    // lcd_display_picture(COORD_NUM_ONES_X,COORD_NUM_ONES_Y,++lcd_enc_test);
                    // lcd_display_picture(0,0,FLASH_PICTURE_IDX_MENU_2);
                    // lcd_display_picture(COORD_TOP_LEFT_X,COORD_TOP_LEFT_Y,FLASH_PICTURE_IDX_INPUT);
                    // lcd_display_picture(COORD_OPTION_X,COORD_OPTION_Y,FLASH_PICTURE_IDX_INP_COAX);
                    // lcd_display_picture(COORD_DOT1_POS_X,COORD_DOT1_POS_Y,FLASH_PICTURE_IDX_DOT_ORANGE);
                    // lcd_display_picture(COORD_DOT2_POS_X,COORD_DOT2_POS_Y,FLASH_PICTURE_IDX_DOT_GRAY);
                    menu_msg_check_in(MENU_MSG_FWD);
                    encoder_key_st = KEY_ST_RELEASE;
                }
            }
            break;
        default:
            break;
        }
        vTaskDelay(ENCODER_TASK_DELAY);
    }
}

void encoder_keyscan_task(void *p)
{

    uint8_t key_cnt = 0;
    while (1)
    {
        uint8_t key_press = !gpio_input_bit_get(GPIO_ENCODER_PORT, GPIO_ENCODER_PIN_PRESS);
        switch (encoder_key_st)
        {
        case KEY_ST_IDL:
            if (key_press)
            {
                key_cnt = 0;
                encoder_key_st = KEY_ST_PRESS;
            }
            break;
        case KEY_ST_PRESS:
            
            if (++key_cnt > 100)
            {
                menu_msg_check_in(MENU_MSG_SHORT_HOLD);
                encoder_key_st = KEY_ST_SHORT_HOLD;
                key_cnt = 0;
            }
            if (!key_press)
            {

                if (key_cnt > KEY_CLK_TIME_MIN)
                {
                    encoder_key_st = KEY_ST_CLK;
                    key_cnt = 0;
                }
                else
                {
                    encoder_key_st = KEY_ST_RELEASE;
                }
            }

            break;
        case KEY_ST_SHORT_HOLD:
            
            if (++key_cnt > KEY_HLD_TIME_MIN-100)
            {
                encoder_key_st = KEY_ST_LONG_HOLD;
            }
            if(!key_press)
            {
                encoder_key_st = KEY_ST_RELEASE;
            }
             
            break;
        case KEY_ST_CLK:
            ++key_cnt;
            if (key_press && key_cnt > 10)
            {
                encoder_key_st = KEY_ST_DOUBLE_CLK;
                key_cnt = 0;
            }
            if (key_cnt >= 30)
            {
                menu_msg_check_in(MENU_MSG_CLICK);
                encoder_key_st = KEY_ST_RELEASE;
            }
            break;
        case KEY_ST_LONG_HOLD:
            // lcd_fill_screen(GREEN);
            // lcd_display_picture(0, 0, FLASH_PICTURE_IDX_DIA);
            menu_msg_check_in(MENU_MSG_LONG_HOLD);
            
            encoder_key_st = KEY_ST_RELEASE;
            break;
        case KEY_ST_DOUBLE_CLK:
            menu_msg_check_in(MENU_MSG_DOUBLE_CLICK);
            encoder_key_st = KEY_ST_RELEASE;
            // ++key_cnt;
            // if (!key_press && key_cnt > 5)
            // {
            //     encoder_key_st = KEY_ST_TRIPLE_CLK;
            //     key_cnt = 0;
            // }
            // if (key_cnt >= 30)
            // {
                
            //     menu_msg_check_in(MENU_MSG_DOUBLE_CLICK);
            //     encoder_key_st = KEY_ST_RELEASE;
            // }

            break;
        case KEY_ST_TRIPLE_CLK:
            ++key_cnt;
            if (key_press && key_cnt > 5)
            {
                menu_msg_check_in(MENU_MSG_TRIPLE_CLICK);
                encoder_key_st = KEY_ST_RELEASE;
            }
            if (key_cnt >= 30)
            {
                menu_msg_check_in(MENU_MSG_DOUBLE_CLICK);
                encoder_key_st = KEY_ST_RELEASE;
            }
            break;
        case KEY_ST_RELEASE:
            if (!key_press)
            {
                encoder_key_st = KEY_ST_IDL;
            }
            break;
        default:
            break;
        }
        vTaskDelay(ENCODER_KEYSCAN_TASK_DELAY);
    }
}

void encoder_init()
{
    encoder_blk.encoder_pin_a_cnt = 0;
    encoder_blk.encoder_pin_b_cnt = 0;
    encoder_blk.encoder_pin_a = 1;
    encoder_blk.encoder_pin_b = 1;

    encoder_st = ENCODER_ST_IDL;
}
