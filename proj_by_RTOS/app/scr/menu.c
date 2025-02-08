#include "config.h"

uint8_t menu_present = 0;
menu_msg_qu_t menu_msg_qu = {0};

void (*menu_array_p[])(menu_msg_t) =
    {
        menu_name0_proc,
        menu_name1_proc,
        menu_name2_proc,
        menu_name3_proc,
        menu_name4_proc, // LCD
        menu_name5_proc, // AUTO
};

menu_layer_t menu_layer[] =
    {
        {MENU_IDX_NAME1, OPTION_IDX_NAME1},
        {MENU_IDX_NAME2, OPTION_IDX_NAME1},
        {MENU_IDX_NAME3, OPTION_IDX_NAME1},
};

menu_blk_t menu_blk = {0};
menu_misc_t menu_misc = {0};

void menu_msg_check_in(menu_msg_t msg)
{
    if (menu_msg_qu.total >= MENU_MSG_SIZE)
        return;
    if (xSemaphoreTake(menu_sema, portMAX_DELAY) == pdTRUE)
    {
        menu_msg_qu.menu_msg[menu_msg_qu.idx_in] = msg;
        if (++menu_msg_qu.idx_in >= MENU_MSG_SIZE)
        {
            menu_msg_qu.idx_in = 0;
        }
        menu_msg_qu.total++;
    }
    xSemaphoreGive(menu_sema);
}

void menu_msg_check_out()
{
    if (xSemaphoreTake(menu_sema, portMAX_DELAY) == pdTRUE)
    {
        if (++menu_msg_qu.idx_out >= MENU_MSG_SIZE)
        {
            menu_msg_qu.idx_out = 0;
        }
        menu_msg_qu.total--;
    }
    xSemaphoreGive(menu_sema);
}
#define LCD_SLEEP_TIME 45000
#define LCD_BACK_TIME 30000
uint16_t lcd_sleep_cnt = LCD_SLEEP_TIME;
uint16_t lcd_back_cnt = LCD_BACK_TIME;

void menu_task(void *p)
{
    vTaskDelay(1500);
    user_data_value();

    menu_init();
    while (1)
    {
        if (menu_msg_qu.total)
        {
            if (user_data.lcd_bright != LCD_BRIGHT_ST_0 || lcd_sleep_cnt != 0)
            {
                menu_array_p[menu_blk.menu_array_p_idx](menu_msg_qu.menu_msg[menu_msg_qu.idx_out]);
                //i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
            }
            else
            {
                 lcd_display_menu(menu_blk.menu_array_p_idx);
                 vTaskDelay(200);

            }
            if (user_data.lcd_bright == LCD_BRIGHT_ST_0)
            {
                lcd_sleep_cnt = LCD_SLEEP_TIME;

                // lcd_display_brightness(5);
            }
            if (menu_blk.menu_array_p_idx != 1)
            {
                lcd_back_cnt = LCD_BACK_TIME;
            }

            menu_msg_check_out();

            switch (user_data.lcd_bright)
            {
            case 0:
                lcd_display_brightness(LCD_DISPLAY_BRIGHTNESS_LEVEL4);
                break;
            case 1:
                lcd_display_brightness(LCD_DISPLAY_BRIGHTNESS_LEVEL2);
                break;
            case 2:
                lcd_display_brightness(LCD_DISPLAY_BRIGHTNESS_LEVEL3);
                break;
            case 3:
                lcd_display_brightness(LCD_DISPLAY_BRIGHTNESS_LEVEL4);
                break;
            case 4:
                lcd_display_brightness(LCD_DISPLAY_BRIGHTNESS_MAX);
                break;
            default:
                break;
            }
        }
        if (user_data.lcd_bright == LCD_BRIGHT_ST_0 && lcd_sleep_cnt && --lcd_sleep_cnt == 0)
        {
            lcd_display_brightness(0);
            lcd_fill_screen(BLACK);
        }
        if (menu_blk.menu_array_p_idx != 1 && lcd_back_cnt && --lcd_back_cnt == 0)
        {
            menu_blk.menu_array_p_idx = 1 ;
            lcd_display_menu(1);
            menu_misc.mode_f = 0;
            user_data.fun_st = FUN_ST_INPUT;
        }

        vTaskDelay(MENU_TASK_DELAY);
    }
}

void menu_name0_proc(menu_msg_t msg)
{
    ir_menu_proc(msg, 0);
    switch (msg)
    {
    case MENU_MSG_DOUBLE_CLICK:

        break;
    case MENU_MSG_CLICK:
        lcd_display_picture(38, 99, FLASH_PICTURE_IDX_LOGO);
        for (int8_t i = 0; i <= 15; i++)
        {
            lcd_display_brightness(i);
            vTaskDelay(100);
        }
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx++;
        break;
    case MENU_MSG_LONG_HOLD:

        break;
    case MENU_MSG_FWD:

        break;
    case MENU_MSG_REV:
        break;
    default:
        break;
    }
}
extern uint8_t lcd_test_f;
void menu_name1_proc(menu_msg_t msg)
{
    ir_menu_proc(msg, 1);
    switch (msg)
    {
    case MENU_MSG_CLICK:
        menu_blk.menu_array_p_idx++;
        lcd_display_menu(2);

        lcd_test_f = 0;
        break;
    case MENU_MSG_DOUBLE_CLICK:
        if (user_data.output == OUTPUT_ST_PH)
        {
            user_data.output = OUTPUT_ST_R_X;
        }
        else // if (user_data.output == OUTPUT_ST_R_X)
        {
            user_data.output = OUTPUT_ST_PH;
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
        lcd_display_volume(user_data.ph_volume);
        break;

    case MENU_MSG_FWD:
        if (menu_misc.mute)
        {
            menu_misc.mute = 0;
            lcd_display_mute();
        }
        else if (user_data.ph_volume < 99)
        {
            lcd_display_volume(++user_data.ph_volume);
            //user_data_save.ph_volume++;
            i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
            i2c_msg_check_in(I2C_MSG_WRITE_4493_DATA);

        }
        break;
    case MENU_MSG_REV:
        if (menu_misc.mute)
        {
            menu_misc.mute = 0;
            lcd_display_mute();
        }
        else if (user_data.ph_volume > 0)
        {
            lcd_display_volume(--user_data.ph_volume);
            //user_data_save.ph_volume--;
            i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
            i2c_msg_check_in(I2C_MSG_WRITE_4493_DATA);
        }
        break;
    case MENU_MSG_SHORT_HOLD:
        menu_misc.mute = !menu_misc.mute;
        lcd_display_mute();
        break;
    case MENU_MSG_LONG_HOLD:
        lcd_display_menu(0);
        menu_blk.menu_array_p_idx--;
        lcd_test_f = 0;
        break;
    default:
        break;
    }
}
user_data_t user_data_unselect;
void menu_name2_fwd(void);
void menu_name2_rev(void);
void menu_name2_proc(menu_msg_t msg)
{
    ir_menu_proc(msg, 2);
    switch (msg)
    {
    case MENU_MSG_CLICK:
        if(menu_misc.mode_f)
        {
            user_data_save_value();
            i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
        }
        if (menu_misc.mode_f && menu_misc.def == 1)
        {
            user_data = user_data_default;
            i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
            menu_misc.def = 0;
            menu_misc.mode_f = 0;
            menu_misc.mute = 0;
            lcd_display_menu(1);
            menu_blk.menu_array_p_idx--;
        }
        else
        {
            if (menu_misc.mode_f)
            {
                menu_misc.mode_f = 0;
            }
            else
            {
                menu_misc.mode_f = 1;
                user_data_unselect = user_data;
            }
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
        break;
    case MENU_MSG_DOUBLE_CLICK:
        if (menu_misc.mode_f)
        {
            user_data.input = user_data_unselect.input;
            user_data.output = user_data_unselect.output;
            user_data.bass = user_data_unselect.bass;
            user_data.treble = user_data_unselect.treble;
        }
        menu_blk.menu_array_p_idx--;
        lcd_display_menu(1);
        menu_misc.mode_f = 0;
        user_data.fun_st = FUN_ST_INPUT;
        break;
    case MENU_MSG_FWD:
        menu_name2_fwd();
        if (user_data.fun_st <= FUN_ST_TREBLE)
        {
            lcd_display_menu2_update();
        }
        else
        {
            menu_blk.menu_array_p_idx++;
            lcd_display_menu(3);
        }
        // i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
        break;
    case MENU_MSG_REV:
        menu_name2_rev();

        if (user_data.fun_st <= FUN_ST_TREBLE)
        {
            lcd_display_menu2_update();
        }
        else
        {
            menu_blk.menu_array_p_idx++;
            lcd_display_menu(3);
        }
        // i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
        break;
    case MENU_MSG_LONG_HOLD:
        menu_blk.menu_array_p_idx = MENU_IDX_NAME0;
        lcd_display_menu(0);
        break;
    default:
        break;
    }
}
void menu_name2_fwd()
{
    if (menu_misc.mode_f)
    {
        switch (user_data.fun_st)
        {
        case FUN_ST_INPUT:
            if (++user_data.input > INPUT_ST_RCA)
            {
                user_data.input = INPUT_ST_COA;
            }
            break;
        case FUN_ST_OUTPUT:
            if (++user_data.output > OUTPUT_ST_LO)
            {
                user_data.output = OUTPUT_ST_PH;
            }
            break;
        case FUN_ST_TREBLE:
            if (++user_data.treble > 10)
            {
                user_data.treble = -10;
            }
            if (user_data.bass || user_data.treble)
            {
                user_data.tone = TONE_ST_YES;
            }
            break;
        case FUN_ST_BASS:
            if (++user_data.bass > 12)
            {
                user_data.bass = -12;
            }
            if (user_data.bass || user_data.treble)
            {
                user_data.tone = TONE_ST_YES;
            }
            break;
        default:
            break;
        }
        lcd_display_menu2_mode();
    }
    else
    {

        user_data.fun_st++;

        lcd_display_menu2_fun();
        lcd_display_menu2_mode();
        // i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
    }
}
void menu_name2_rev()
{
    if (menu_misc.mode_f)
    {
        switch (user_data.fun_st)
        {
        case FUN_ST_BASS:
            if (--user_data.bass < -12)
            {
                user_data.bass = 12;
            }
            if (user_data.bass || user_data.treble)
            {
                user_data.tone = TONE_ST_YES;
            }
            break;
        case FUN_ST_TREBLE:
            if (--user_data.treble < -10)
            {
                user_data.treble = 10;
            }
            if (user_data.bass || user_data.treble)
            {
                user_data.tone = TONE_ST_YES;
            }
            break;

        case FUN_ST_INPUT:
            if (--user_data.input > INPUT_ST_RCA)
            {
                user_data.input = INPUT_ST_RCA;
            }

            break;

        case FUN_ST_OUTPUT:
            if (--user_data.output > OUTPUT_ST_LO)
            {
                user_data.output = OUTPUT_ST_LO;
            }
            break;

        default:
            break;
        }
        lcd_display_menu2_mode();
    }
    else
    {
        if (user_data.fun_st == FUN_ST_INPUT)
        {
            user_data.fun_st = FUN_ST_GAIN;
        }
        else
        {
            user_data.fun_st--;
        }
        lcd_display_menu2_fun();
        lcd_display_menu2_mode();
        // i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
    }
}

void menu_name3_proc(menu_msg_t msg)
{
    ir_menu_proc(msg, 3);
    switch (msg)
    {
    case MENU_MSG_CLICK:
        if(menu_misc.mode_f)
        {
            user_data_save_value();
            i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
        }
        if (menu_misc.mode_f && menu_misc.def == 1)
        {
            user_data = user_data_default;
            i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
            menu_misc.def = 0;
            menu_misc.mode_f = 0;
            menu_misc.mute = 0;
            lcd_display_menu(1);
            menu_blk.menu_array_p_idx--;
        }
        else
        {
            if (menu_misc.mode_f)
            {
                menu_misc.mode_f = 0;
            }
            else
            {
                menu_misc.mode_f = 1;
                user_data_unselect = user_data;
            }
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
        break;
    case MENU_MSG_DOUBLE_CLICK:
        if (menu_misc.mode_f)
        {
            user_data.gain = user_data_unselect.gain;
            user_data.uac = user_data_unselect.uac;
            user_data.filter = user_data_unselect.filter;
            user_data.tone = user_data_unselect.tone;
        }
        menu_blk.menu_array_p_idx = 1;
        lcd_display_menu(1);
        menu_misc.mode_f = 0;
        user_data.fun_st = FUN_ST_INPUT;
        break;
    case MENU_MSG_FWD:
        menu_name3_fwd();
        if (user_data.fun_st <= FUN_ST_TONE)
        {

            lcd_display_menu3_update();
        }
        else
        {
            menu_blk.menu_array_p_idx--;
            user_data.fun_st = FUN_ST_TREBLE;
            lcd_display_menu(2);
        }
        // i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
        break;
    case MENU_MSG_REV:
        menu_name3_rev();

        if (user_data.fun_st > FUN_ST_TREBLE)
        {
            lcd_display_menu3_update();
        }
        else
        {
            menu_blk.menu_array_p_idx--;
            
            lcd_display_menu(2);
        }
        // i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
        break;
    case MENU_MSG_LONG_HOLD:
        menu_blk.menu_array_p_idx = MENU_IDX_NAME0;
        lcd_display_menu(0);
        break;
    default:
        break;
    }
}
void menu_name3_fwd()
{
    if (menu_misc.mode_f)
    {
        switch (user_data.fun_st)
        {
        case FUN_ST_FILT:
            if (++user_data.filter > FILT_ST_5)
            {
                user_data.filter = FILT_ST_0;
            }
            break;
        case FUN_ST_TONE:
            if (user_data.tone == TONE_ST_NO)
            {
                user_data.tone = TONE_ST_YES;
            }
            else
                user_data.tone = TONE_ST_NO;
            break;
        case FUN_ST_GAIN:
            if (++user_data.gain > GAIN_ST_H)
            {
                user_data.gain = GAIN_ST_L;
            }
            break;

        case FUN_ST_UAC:
            if (++user_data.uac > UAC_ST_2_0)
            {
                user_data.uac = UAC_ST_1_0;
            }
            break;
        default:
            break;
        }
    }
    else
    {
        if (user_data.fun_st < FUN_ST_TONE+1)
        {
            user_data.fun_st++;
        }
        lcd_display_menu2_fun();
        lcd_display_menu2_mode();
        // i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
    }
}

void menu_name3_rev()
{
    if (menu_misc.mode_f)
    {
        switch (user_data.fun_st)
        {

        case FUN_ST_FILT:
            if (--user_data.filter > FILT_ST_5)
            {
                user_data.filter = FILT_ST_5;
            }
            break;
        case FUN_ST_GAIN:
            if (--user_data.gain > GAIN_ST_H)
            {
                user_data.gain = GAIN_ST_H;
            }
            break;

        case FUN_ST_UAC:
            if (--user_data.uac > UAC_ST_2_0)
            {
                user_data.uac = UAC_ST_2_0;
            }
            break;
        case FUN_ST_TONE:
            if (user_data.tone == TONE_ST_NO)
            {
                user_data.tone = TONE_ST_YES;
            }
            else
                user_data.tone = TONE_ST_NO;
            break;
        default:
            break;
        }
        lcd_display_menu2_mode();
    }
    else
    {

        user_data.fun_st--;

        lcd_display_menu2_fun();
        lcd_display_menu2_mode();
        // i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
    }
}
// LCD BRIGHTNESS
void menu_name4_proc(menu_msg_t msg)
{
    ir_menu_proc(msg, 4);
    switch (msg)
    {
    case MENU_MSG_DOUBLE_CLICK:

        break;
    case MENU_MSG_CLICK:
        user_data_save_value();
        i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx = 1;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        
        break;
    case MENU_MSG_LONG_HOLD:

        break;
    case MENU_MSG_FWD:
        if (++user_data.lcd_bright > LCD_BRIGHT_ST_4)
        {
            user_data.lcd_bright = LCD_BRIGHT_ST_0;
        }
        lcd_display_menu4_lcd_brightness_update();
        break;
    case MENU_MSG_REV:
        if (--user_data.lcd_bright > LCD_BRIGHT_ST_4)
        {
            user_data.lcd_bright = LCD_BRIGHT_ST_4;
        }
        lcd_display_menu4_lcd_brightness_update();
        break;
    default:
        break;
    }
}

// auto
void menu_name5_proc(menu_msg_t msg)
{
    ir_menu_proc(msg, 5);
    switch (msg)
    {
    case MENU_MSG_DOUBLE_CLICK:

        break;
    case MENU_MSG_CLICK:
        lcd_display_menu(1);
        menu_blk.menu_array_p_idx = 1;
        user_data.fun_st = FUN_ST_INPUT;
        menu_misc.mode_f = 0;
        break;
    case MENU_MSG_LONG_HOLD:

        break;
    case MENU_MSG_FWD:

    case MENU_MSG_REV:
        user_data.auto_st = !user_data.auto_st;
        lcd_display_menu5_auto_update();
        break;
    default:
        break;
    }
}

void option_deselect(option_idx_t option_idx)
{
    switch (option_idx)
    {
    case OPTION_IDX_NAME1:

        break;
    case OPTION_IDX_NAME2:

        break;
    case OPTION_IDX_NAME3:

        break;
    default:
        break;
    }
}

void option_select(option_idx_t option_idx)
{
    switch (option_idx)
    {
    case OPTION_IDX_NAME1:

        break;
    case OPTION_IDX_NAME2:

        break;
    case OPTION_IDX_NAME3:

        break;
    default:
        break;
    }
}

void menu_init()
{

    menu_blk.menu_array_p_idx = MENU_IDX_NAME1;
    // lcd_fill_screen(BLACK);
    menu1_init();
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
    //  lcd_msg_check_in(LCD_MSG_FLASH, NULL, lcd_pic_attr_custom);
}
