#include "ir.h"

ir_qu_t ir_qu = {0};
ir_blk_t ir_blk = {0};
uint8_t ir_buf = {0};
ir_body_t ir_body = {0};

void ir_qu_check_in(uint8_t *buf)
{
    if (ir_qu.total < IR_FRAME_SIZE)
    {
        uint8_t i;
        for (i = 0; i < IR_BUF_SIZE; i++)
        {
            ir_qu.frame[ir_qu.idx_in].buf[i] = buf[i];
        }

        if (++ir_qu.idx_in >= IR_FRAME_SIZE)
        {
            ir_qu.idx_in = 0;
        }
        ir_qu.total++;
    }
}

void ir_qu_check_out()
{
    if (++ir_qu.idx_out >= IR_FRAME_SIZE)
    {
        ir_qu.idx_out = 0;
    }
    ir_qu.total--;
}

void ir_callback(uint16_t cnt)
{
    if (!ir_blk.start_edge_f)
    {
        ir_blk.last_capture_cnt = cnt;
        ir_blk.start_edge_f = 1;
    }
    else
    {
        uint16_t pulse_width;
        ir_blk.present_capture_cnt = cnt;
        if (ir_blk.present_capture_cnt < ir_blk.last_capture_cnt)
        {
            pulse_width = ir_blk.present_capture_cnt + 65536 - ir_blk.last_capture_cnt;
        }
        else
        {
            pulse_width = ir_blk.present_capture_cnt - ir_blk.last_capture_cnt;
        }
        if (pulse_width >= BOOT_CODE_MIN && pulse_width <= BOOT_CODE_MAX)
        {
            ir_body.idx = 0;
            ir_blk.start_body_f = 1;
            ir_blk.ir_safe_cnt = IR_SAFE_TIME;
        }
        if (ir_blk.start_body_f == 1)
        {
            if (pulse_width >= BIT_0_MIN && pulse_width <= BIT_0_MAX)
            {
                ir_body.buf[ir_body.idx >> 3] = ir_body.buf[ir_body.idx >> 3] >> 1;
                ++ir_body.idx;
            }
            else if (pulse_width >= BIT_1_MIN && pulse_width <= BIT_1_MAX)
            {
                ir_body.buf[ir_body.idx >> 3] = ir_body.buf[ir_body.idx >> 3] >> 1;
                ir_body.buf[ir_body.idx >> 3] |= (1 << 7);
                ++ir_body.idx;
            }
            if (ir_body.idx >= IR_BUF_SIZE * 8) // should not use ++ir_body.idx here,cuz the second falling edge represents the first bit
            {
                ir_qu_check_in(ir_body.buf);
                ir_blk.start_edge_f = 0;
                ir_blk.start_body_f = 0;
                ir_body.idx = 0;
            }
        }
        if (ir_blk.ir_last_code != 0)
        {
            if (pulse_width >= REPEAT_CODE_MIN && pulse_width <= REPEAT_CODE_MAX)
            {
                ir_blk.ir_safe_cnt = IR_SAFE_TIME;
                if (++ir_blk.repeat_code_cnt >= 4)
                {
                    ir_blk.repeat_code_cnt = 3;
                    switch (ir_blk.ir_last_code)
                    {
                    case IR_CODE_VOL_P:
                        menu_msg_check_in(MENU_MSG_IR_VOL_P);
                        menu_msg_check_in(MENU_MSG_IR_VOL_P);
                        break;
                    case IR_CODE_VOL_N:
                        menu_msg_check_in(MENU_MSG_IR_VOL_N);
                        menu_msg_check_in(MENU_MSG_IR_VOL_N);
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        ir_blk.last_capture_cnt = cnt;
    }
}
// 10ms process
int16_t ir_cnt = 0;
int16_t ir_ex_cnt = 0;
void ir_task(void *p)
{
    while (1)
    {

        if (ir_blk.ir_safe_cnt && --ir_blk.ir_safe_cnt == 0)
        {
            ir_blk.start_body_f = 0;
            ir_blk.ir_last_code = 0;
            ir_blk.start_edge_f = 0;
            ir_blk.repeat_code_cnt = 0;
        }
        if (ir_qu.total)
        {
            ir_ex_cnt++;
            if (ir_qu.frame[ir_qu.idx_out].buf[0] == IR_CODE_USER_H &&
                ir_qu.frame[ir_qu.idx_out].buf[1] == IR_CODE_USER_L &&
                ir_qu.frame[ir_qu.idx_out].buf[2] == (uint8_t)~ir_qu.frame[ir_qu.idx_out].buf[3])
            {
                menu_msg_t msg_ir_in = MENU_MSG_INVAL;
                ir_blk.ir_last_code = 0;
                ir_blk.repeat_code_cnt = 0;
                switch (ir_qu.frame[ir_qu.idx_out].buf[2])
                {
                case IR_CODE_PWR:
                    msg_ir_in = MENU_MSG_IR_PWR;
                    break;

                case IR_CODE_LCD_BRIGHTNESS:
                    msg_ir_in = MENU_MSG_IR_LCD_BRIGHTNESS;
                    break;

                case IR_CODE_MUTE:
                    msg_ir_in = MENU_MSG_IR_MUTE;
                    break;

                case IR_CODE_VOL_P:
                    msg_ir_in = MENU_MSG_IR_VOL_P;
                    ir_blk.ir_last_code = IR_CODE_VOL_P;
                ir_cnt++;
                    break;

                case IR_CODE_VOL_N:
                    msg_ir_in = MENU_MSG_IR_VOL_N;
                    ir_blk.ir_last_code = IR_CODE_VOL_N;
                    ir_cnt--;
                    break;

                case IR_CODE_FUN_P:
                    msg_ir_in = MENU_MSG_IR_FUN_P;
                ir_cnt++;
                    break;

                case IR_CODE_FUN_N:
                    msg_ir_in = MENU_MSG_IR_FUN_N;
                ir_cnt--;
                    break;

                case IR_CODE_CONFIRM:
                    msg_ir_in = MENU_MSG_IR_CONFIRM;
                    break;

                case IR_CODE_INPUT:
                    msg_ir_in = MENU_MSG_IR_INPUT;
                    break;

                case IR_CODE_OUTPUT:
                    msg_ir_in = MENU_MSG_IR_OUTPUT;
                    break;

                case IR_CODE_FILT:
                    msg_ir_in = MENU_MSG_IR_FILT;
                    break;

                case IR_CODE_AUTO:
                    msg_ir_in = MENU_MSG_IR_AUTO;
                    break;

                case IR_CODE_BACK:
                    msg_ir_in = MENU_MSG_IR_BACK;
                    break;

                default:
                    break;
                }
                menu_msg_check_in(msg_ir_in);
            }
            ir_qu_check_out();
        }
        vTaskDelay(10);
    }
}
void ir_init()
{
    uint16_t i;
    for (i = 0; i < sizeof(ir_blk); i++)
    {
        *((uint8_t *)&ir_blk + i) = 0;
    }
    for (i = 0; i < sizeof(ir_qu); i++)
    {
        *((uint8_t *)&ir_qu + i) = 0;
    }
    for (i = 0; i < sizeof(ir_buf); i++)
    {
        *((uint8_t *)&ir_buf + i) = 0;
    }
    for (i = 0; i < sizeof(ir_body); i++)
    {
        *((uint8_t *)&ir_body + i) = 0;
    }
}

void ir_menu_proc(menu_msg_t msg, uint8_t menu_array_p_idx)
{
    switch (menu_array_p_idx)
    {
    case 0:
        ir_menu0(msg);
        break;
    case 1:
        ir_menu1(msg);
        break;
    case 2:
        ir_menu2(msg);
        break;
    case 3:
        ir_menu3(msg);
        break;
    case 4:
        ir_menu4(msg);
        break;
    case 5:
        ir_menu5(msg);
        break;
    default:
        break;
    }
}
void ir_menu0(menu_msg_t msg)
{
    switch (msg)
    {
    case MENU_MSG_IR_PWR:
        lcd_display_picture(38, 99, FLASH_PICTURE_IDX_LOGO);
        for (int8_t i = 0; i <= 15; i++)
        {
            lcd_display_brightness(i);
            vTaskDelay(100);
        }
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx++;
        break;

    case MENU_MSG_IR_LCD_BRIGHTNESS:
        break;

    case MENU_MSG_IR_MUTE:
        break;

    case MENU_MSG_IR_VOL_P:
        break;

    case MENU_MSG_IR_VOL_N:
        break;

    case MENU_MSG_IR_FUN_P:
        break;

    case MENU_MSG_IR_FUN_N:
        break;

    case MENU_MSG_IR_CONFIRM:
        break;

    case MENU_MSG_IR_INPUT:
        break;

    case MENU_MSG_IR_OUTPUT:
        break;

    case MENU_MSG_IR_FILT:
        break;

    case MENU_MSG_IR_AUTO:
        break;

    case MENU_MSG_IR_BACK:
        break;

    default:
        break;
    }
}

void ir_menu1(menu_msg_t msg)
{
    switch (msg)
    {
    case MENU_MSG_IR_PWR:
        lcd_display_menu(0);
        menu_blk.menu_array_p_idx = 0;
        break;

    case MENU_MSG_IR_LCD_BRIGHTNESS:
        lcd_display_menu(4);
        menu_blk.menu_array_p_idx = 4;
        user_data.fun_st = FUN_ST_LCD;
        menu_misc.mode_f = 1;
        break;

    case MENU_MSG_IR_MUTE:
        menu_misc.mute = !menu_misc.mute;
        lcd_display_mute();
        break;

    case MENU_MSG_IR_VOL_P:
        if (menu_misc.mute)
        {
            menu_misc.mute = 0;
            lcd_display_mute();
        }
        else if (user_data.ph_volume < 99)
        {
            lcd_display_volume(++user_data.ph_volume);
            // i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
        }
        break;

    case MENU_MSG_IR_VOL_N:
        if (menu_misc.mute)
        {
            menu_misc.mute = 0;
            lcd_display_mute();
        }
        else if (user_data.ph_volume > 0)
        {
            lcd_display_volume(--user_data.ph_volume);
            // i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
        }
        break;

    case MENU_MSG_IR_FUN_P:
        break;

    case MENU_MSG_IR_FUN_N:
        break;

    case MENU_MSG_IR_CONFIRM:
        menu_blk.menu_array_p_idx = 2;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        //lcd_display_menu(2);
        //lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_INPUT);
        lcd_display_menu2_update();
        break;

    case MENU_MSG_IR_INPUT:
        
        menu_blk.menu_array_p_idx = 2;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 1;
        lcd_display_menu(2);
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_INPUT);
        lcd_display_menu2_update();
        break;

    case MENU_MSG_IR_OUTPUT:
        
        menu_blk.menu_array_p_idx = 2;
        user_data.fun_st = FUN_ST_OUTPUT;
        menu_misc.mode_f = 1;
        lcd_display_menu(2);
        lcd_display_picture(COORD_TOP_RIGHT_X, COORD_TOP_RIGHT_Y, FLASH_PICTURE_IDX_OUTPUT);
        lcd_display_menu2_update();
        
        break;

    case MENU_MSG_IR_FILT:
        menu_blk.menu_array_p_idx = 3;
        user_data.fun_st = FUN_ST_FILT;
        menu_misc.mode_f = 1;
        lcd_display_menu(3);
        lcd_display_picture(COORD_BOT_RIGHT_X, COORD_BOT_RIGHT_Y, FLASH_PICTURE_IDX_FILT);
        lcd_display_menu3_filt_update();
        break;

    case MENU_MSG_IR_AUTO:
        lcd_display_menu(5);
        menu_blk.menu_array_p_idx = 5;
        user_data.fun_st = FUN_ST_AUTO;
        menu_misc.mode_f = 1;
        break;

    case MENU_MSG_IR_BACK:

        break;
    default:
        break;
    }
}

void ir_menu2(menu_msg_t msg)
{
    switch (msg)
    {
    case MENU_MSG_IR_PWR:
        lcd_display_menu(0);
        menu_blk.menu_array_p_idx = 0;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        break;

    case MENU_MSG_IR_LCD_BRIGHTNESS:
        lcd_display_menu(4);
        menu_blk.menu_array_p_idx = 4;
        user_data.fun_st = FUN_ST_LCD;
        menu_misc.mode_f = 1;
        break;

    case MENU_MSG_IR_MUTE:
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx = 1;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        menu_msg_check_in(MENU_MSG_IR_MUTE);
        break;

    case MENU_MSG_IR_VOL_P:
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx = 1;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        menu_msg_check_in(MENU_MSG_IR_VOL_P);
        break;

    case MENU_MSG_IR_VOL_N:
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx = 1;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        menu_msg_check_in(MENU_MSG_IR_VOL_N);
        break;

    case MENU_MSG_IR_FUN_P:
    menu_msg_check_in(MENU_MSG_FWD);
        // menu_name2_fwd();
        // if (user_data.fun_st <= FUN_ST_TREBLE)
        // {
        //     lcd_display_menu2_update();
        // }
        // else
        // {
        //     menu_blk.menu_array_p_idx++;
        //     lcd_display_menu(3);
        // }
        break;

    case MENU_MSG_IR_FUN_N:
    menu_msg_check_in(MENU_MSG_REV);
        // menu_name2_rev();
        // if (user_data.fun_st <= FUN_ST_TREBLE)
        // {
        //     lcd_display_menu2_update();
        // }
        // else
        // {
        //     menu_blk.menu_array_p_idx++;
        //     lcd_display_menu(3);
        // }
        break;

    case MENU_MSG_IR_CONFIRM:
        menu_msg_check_in(MENU_MSG_CLICK);
        break;

    case MENU_MSG_IR_INPUT:
        if (user_data.fun_st == FUN_ST_INPUT && menu_misc.mode_f)
        {
            menu_msg_check_in(MENU_MSG_IR_FUN_P);
        }
        else
        {
            user_data.fun_st = FUN_ST_INPUT;
            menu_misc.mode_f = 1;
            lcd_display_menu(2);
            lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_INPUT);
            lcd_display_menu2_update();
        }
        break;

    case MENU_MSG_IR_OUTPUT:
        if (user_data.fun_st == FUN_ST_OUTPUT && menu_misc.mode_f)
        {
            menu_msg_check_in(MENU_MSG_IR_FUN_P);
        }
        else
        {
            user_data.fun_st = FUN_ST_OUTPUT;
            menu_misc.mode_f = 1;
            lcd_display_menu(2);
            lcd_display_picture(COORD_TOP_RIGHT_X, COORD_TOP_RIGHT_Y, FLASH_PICTURE_IDX_OUTPUT);
            lcd_display_menu2_update();
        }
        break;

    case MENU_MSG_IR_FILT:
        
        menu_blk.menu_array_p_idx = 3;
        user_data.fun_st = FUN_ST_FILT;
        menu_misc.mode_f = 1;
        lcd_display_menu(3);
        lcd_display_picture(COORD_BOT_RIGHT_X, COORD_BOT_RIGHT_Y, FLASH_PICTURE_IDX_FILT);
        lcd_display_menu3_filt_update();
        break;

    case MENU_MSG_IR_AUTO:
        lcd_display_menu(5);
        menu_blk.menu_array_p_idx = 5;
        user_data.fun_st = FUN_ST_AUTO;
        menu_misc.mode_f = 1;
        break;

    case MENU_MSG_IR_BACK:
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx = 1;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        break;

    default:
        break;
    }
}

void ir_menu3(menu_msg_t msg)
{
    switch (msg)
    {
    case MENU_MSG_IR_PWR:

        lcd_display_menu(0);
        menu_blk.menu_array_p_idx = 0;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        break;

    case MENU_MSG_IR_LCD_BRIGHTNESS:
        lcd_display_menu(4);
        menu_blk.menu_array_p_idx = 4;
        user_data.fun_st = FUN_ST_LCD;
        menu_misc.mode_f = 1;
        break;

    case MENU_MSG_IR_MUTE:
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx = 1;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        menu_msg_check_in(MENU_MSG_IR_MUTE);
        break;

    case MENU_MSG_IR_VOL_P:
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx = 1;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        menu_msg_check_in(MENU_MSG_IR_VOL_P);
        break;

    case MENU_MSG_IR_VOL_N:
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx = 1;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        menu_msg_check_in(MENU_MSG_IR_VOL_N);
        break;

    case MENU_MSG_IR_FUN_P:
    menu_msg_check_in(MENU_MSG_FWD);
        // menu_name3_fwd();
        // if (user_data.fun_st <= FUN_ST_TONE)
        // {

        //     lcd_display_menu3_update();
        // }
        // else
        // {
        // }
        break;

    case MENU_MSG_IR_FUN_N:
        menu_msg_check_in(MENU_MSG_REV);
        // menu_name3_rev();

        // if (user_data.fun_st > FUN_ST_TREBLE)
        // {
        //     lcd_display_menu3_update();
        // }
        // else
        // {
        //     menu_blk.menu_array_p_idx--;
        //     lcd_display_menu(2);
        // }
        break;

    case MENU_MSG_IR_CONFIRM:
     menu_msg_check_in(MENU_MSG_CLICK);
        break;

    case MENU_MSG_IR_INPUT:
        lcd_display_menu(2);
        menu_blk.menu_array_p_idx = 2;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 1;
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_INPUT);
        lcd_display_menu2_input_update();
        break;

    case MENU_MSG_IR_OUTPUT:
        lcd_display_menu(2);
        menu_blk.menu_array_p_idx = 2;
        user_data.fun_st = FUN_ST_OUTPUT;
        menu_misc.mode_f = 1;
        lcd_display_picture(COORD_TOP_RIGHT_X, COORD_TOP_RIGHT_Y, FLASH_PICTURE_IDX_OUTPUT);
        lcd_display_menu2_output_update();
        break;

    case MENU_MSG_IR_FILT:
        if (user_data.fun_st == FUN_ST_FILT && menu_misc.mode_f)
        {
            menu_msg_check_in(MENU_MSG_IR_FUN_P);
        }
        else
        {
            user_data.fun_st = FUN_ST_FILT;
            menu_misc.mode_f = 1;
            lcd_display_menu3_filt_update();
        }
        break;

    case MENU_MSG_IR_AUTO:
        lcd_display_menu(5);
        menu_blk.menu_array_p_idx = 5;
        user_data.fun_st = FUN_ST_AUTO;
        menu_misc.mode_f = 1;
        break;

    case MENU_MSG_IR_BACK:
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx = 1;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        break;

    default:
        break;
    }
}

void ir_menu4(menu_msg_t msg)
{
    switch (msg)
    {
    case MENU_MSG_IR_PWR:

        lcd_display_menu(0);
        menu_blk.menu_array_p_idx = 0;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        break;

    case MENU_MSG_IR_LCD_BRIGHTNESS:
        menu_msg_check_in(MENU_MSG_IR_FUN_P);
        break;

    case MENU_MSG_IR_MUTE:
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx = 1;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        menu_msg_check_in(MENU_MSG_IR_MUTE);
        break;

    case MENU_MSG_IR_VOL_P:
        if (++user_data.lcd_bright >= LCD_BRIGHT_ST_4)
        {
            user_data.lcd_bright = LCD_BRIGHT_ST_0;
        }
        lcd_display_menu4_lcd_brightness_update();
        break;

    case MENU_MSG_IR_VOL_N:
if (--user_data.lcd_bright >= LCD_BRIGHT_ST_4)
        {
            user_data.lcd_bright = LCD_BRIGHT_ST_4;
        }
        lcd_display_menu4_lcd_brightness_update();
        break;

    case MENU_MSG_IR_FUN_P:
        menu_msg_check_in(MENU_MSG_FWD);
        break;

    case MENU_MSG_IR_FUN_N:
        menu_msg_check_in(MENU_MSG_REV);
        break;

    case MENU_MSG_IR_CONFIRM:
    menu_msg_check_in(MENU_MSG_CLICK);
        break;

    case MENU_MSG_IR_INPUT:
        menu_blk.menu_array_p_idx = 2;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 1;
        lcd_display_menu(2);
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_INPUT);
        lcd_display_menu2_update();
        break;

    case MENU_MSG_IR_OUTPUT:
        menu_blk.menu_array_p_idx = 2;
        user_data.fun_st = FUN_ST_OUTPUT;
        menu_misc.mode_f = 1;
        lcd_display_menu(2);
        lcd_display_picture(COORD_TOP_RIGHT_X, COORD_TOP_RIGHT_Y, FLASH_PICTURE_IDX_OUTPUT);
        lcd_display_menu2_update();
        break;

    case MENU_MSG_IR_FILT:
        menu_blk.menu_array_p_idx = 3;
        user_data.fun_st = FUN_ST_FILT;
        menu_misc.mode_f = 1;
        lcd_display_menu(3);
        lcd_display_picture(COORD_BOT_RIGHT_X, COORD_BOT_RIGHT_Y, FLASH_PICTURE_IDX_FILT);
        lcd_display_menu3_filt_update();
        break;

    case MENU_MSG_IR_AUTO:
        lcd_display_menu(5);
        menu_blk.menu_array_p_idx = 5;
        user_data.fun_st = FUN_ST_AUTO;
        menu_misc.mode_f = 1;
        break;

    case MENU_MSG_IR_BACK:
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx = 1;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        break;

    default:
        break;
    }
}

void ir_menu5(menu_msg_t msg)
{
    switch (msg)
    {
    case MENU_MSG_IR_PWR:

        lcd_display_menu(0);
        menu_blk.menu_array_p_idx = 0;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        break;

    case MENU_MSG_IR_LCD_BRIGHTNESS:
        lcd_display_menu(4);
        menu_blk.menu_array_p_idx = 4;
        user_data.fun_st = FUN_ST_LCD;
        menu_misc.mode_f = 1;
        break;

    case MENU_MSG_IR_MUTE:
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx = 1;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        menu_msg_check_in(MENU_MSG_IR_MUTE);
        break;

    case MENU_MSG_IR_VOL_P:
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx = 1;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        menu_msg_check_in(MENU_MSG_IR_VOL_P);
        break;

    case MENU_MSG_IR_VOL_N:
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx = 1;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        menu_msg_check_in(MENU_MSG_IR_VOL_N);
        break;

    case MENU_MSG_IR_FUN_P:
        user_data.auto_st = !user_data.auto_st;
        lcd_display_menu5_auto_update();
        break;

    case MENU_MSG_IR_FUN_N:
        user_data.auto_st = !user_data.auto_st;
        lcd_display_menu5_auto_update();
        break;

    case MENU_MSG_IR_CONFIRM:
    menu_msg_check_in(MENU_MSG_CLICK);
        break;

    case MENU_MSG_IR_INPUT:
        
        menu_blk.menu_array_p_idx = 2;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 1;
        lcd_display_menu(2);
        lcd_display_picture(COORD_TOP_LEFT_X, COORD_TOP_LEFT_Y, FLASH_PICTURE_IDX_INPUT);
        lcd_display_menu2_update();
        break;

    case MENU_MSG_IR_OUTPUT:
        menu_blk.menu_array_p_idx = 2;
        user_data.fun_st = FUN_ST_OUTPUT;
        menu_misc.mode_f = 1;
        lcd_display_menu(2);
        lcd_display_picture(COORD_TOP_RIGHT_X, COORD_TOP_RIGHT_Y, FLASH_PICTURE_IDX_OUTPUT);
        lcd_display_menu2_update();
        break;

    case MENU_MSG_IR_FILT:
        menu_blk.menu_array_p_idx = 3;
        user_data.fun_st = FUN_ST_FILT;
        menu_misc.mode_f = 1;
        lcd_display_menu(3);
        lcd_display_picture(COORD_BOT_RIGHT_X, COORD_BOT_RIGHT_Y, FLASH_PICTURE_IDX_FILT);
        lcd_display_menu3_filt_update();
        break;

    case MENU_MSG_IR_AUTO:
        menu_msg_check_in(MENU_MSG_IR_FUN_P);
        break;

    case MENU_MSG_IR_BACK:
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx = 1;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        break;

    default:
        break;
    }
}
