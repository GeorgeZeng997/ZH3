#include "config.h"
flash_st_t flash_st = FLASH_ST_IDL;

flash_save_buf_t flash_save_buf1 = {0};
flash_save_buf_t flash_save_buf2 = {0};
flash_buf_t flash_tx_buf = {0};
flash_buf_t flash_rx_buf = {0};
flash_msg_qu_t flash_msg_qu;

uint8_t dummy[FLASH_PROGRAM_PAGE_MAX] = {0};

flash_to_lcd_t flash_to_lcd = {0};

flash_blk_t flash_blk = {0};
uint8_t flash_safe_cnt = 0;

flash_picture_atrribute_t flash_picture_array[] =
    {
        //{152, 240, 0x00000000},            // DIA0
        //{169, 240, 0x00020000},
        //{152, 135, 0x00000000},           // DIA0
        {135, 135, 0x00000000},
        {240, 240, FLASH_ADDR_MENU_1}, // DIA1_1
        {0, 0, FLASH_ADDR_MENU_1_ADD}, // DIA1_2
        {114, 114, FLASH_ADDR_LO},
        {114, 114, FLASH_ADDR_PO},
        {114, 114, FLASH_ADDR_R_X},
        {114, 114, FLASH_ADDR_TONE},
        {114, 114, FLASH_ADDR_TONE_2},
        {114, 114, FLASH_ADDR_VOL},
        {114, 114, FLASH_ADDR_VOL_2},
        {114, 114, FLASH_ADDR_DSD_64},
        {114, 114, FLASH_ADDR_DSD_128},
        {114, 114, FLASH_ADDR_DSD_256},
        {114, 114, FLASH_ADDR_DSD_512},
        {114, 114, FLASH_ADDR_PCM_44},
        {114, 114, FLASH_ADDR_PCM_48},
        {114, 114, FLASH_ADDR_PCM_88},
        {114, 114, FLASH_ADDR_PCM_96},
        {114, 114, FLASH_ADDR_PCM_176},
        {114, 114, FLASH_ADDR_PCM_192},
        {114, 114, FLASH_ADDR_PCM_352},
        {114, 114, FLASH_ADDR_PCM_384},
        {114, 114, FLASH_ADDR_PCM_705},
        {114, 114, FLASH_ADDR_PCM_768},
        {114, 114, FLASH_ADDR_COAX},
        {114, 114, FLASH_ADDR_OPT},
        {114, 114, FLASH_ADDR_RCA},
        {114, 114, FLASH_ADDR_USB},
        {80, 64, FLASH_ADDR_MUTE},
        {40, 64, FLASH_ADDR_NUM_0},
        {40, 64, FLASH_ADDR_NUM_1},
        {40, 64, FLASH_ADDR_NUM_2},
        {40, 64, FLASH_ADDR_NUM_3},
        {40, 64, FLASH_ADDR_NUM_4},
        {40, 64, FLASH_ADDR_NUM_5},
        {40, 64, FLASH_ADDR_NUM_6},
        {40, 64, FLASH_ADDR_NUM_7},
        {40, 64, FLASH_ADDR_NUM_8},
        {40, 64, FLASH_ADDR_NUM_9},
        {240, 240, FLASH_ADDR_MENU_2},
        {0, 0, FLASH_ADDR_MENU_2_ADD},

        {114, 114, FLASH_ADDR_INPUT},
        {114, 114, FLASH_ADDR_OUTPUT},
        {114, 114, FLASH_ADDR_BASS},
        {114, 114, FLASH_ADDR_TREBLE},
        {120, 30, FLASH_ADDR_INP_COAX},
        {120, 30, FLASH_ADDR_INP_USB},
        {120, 30, FLASH_ADDR_INP_OPT},
        {120, 30, FLASH_ADDR_INP_RCA},
        {120, 30, FLASH_ADDR_OUT_PO},
        {120, 30, FLASH_ADDR_OUT_R_X},
        {120, 30, FLASH_ADDR_OUT_LO},
        {15, 15, FLASH_ADDR_DOT_GRAY},
        {15, 15, FLASH_ADDR_DOT_ORANGE},
        {120, 30, FLASH_ADDR_INP_COAX_NA}, // FLASH_PICTURE_IDX_INP_COAX_NA
        {120, 30, FLASH_ADDR_INP_USB_NA},  // FLASH_PICTURE_IDX_INP_USB_NA
        {120, 30, FLASH_ADDR_INP_OPT_NA},  // FLASH_PICTURE_IDX_INP_OPT_NA
        {120, 30, FLASH_ADDR_INP_RCA_NA},  // FLASH_PICTURE_IDX_INP_RCA_NA
        {120, 30, FLASH_ADDR_OUT_PO_NA},   // FLASH_PICTURE_IDX_OUT_PO_NA
        {120, 30, FLASH_ADDR_OUT_R_X_NA},  // FLASH_PICTURE_IDX_OUT_R_X_NA
        {120, 30, FLASH_ADDR_OUT_LO_NA},   // FLASH_PICTURE_IDX_OUT_LO_NA

        {20, 30, FLASH_ADDR_LIT_NUM_0}, // FLASH_PICTURE_IDX_LIT_NUM_0
        {20, 30, FLASH_ADDR_LIT_NUM_1}, // FLASH_PICTURE_IDX_LIT_NUM_1
        {20, 30, FLASH_ADDR_LIT_NUM_2}, // FLASH_PICTURE_IDX_LIT_NUM_2
        {20, 30, FLASH_ADDR_LIT_NUM_3}, // FLASH_PICTURE_IDX_LIT_NUM_3
        {20, 30, FLASH_ADDR_LIT_NUM_4}, // FLASH_PICTURE_IDX_LIT_NUM_4
        {20, 30, FLASH_ADDR_LIT_NUM_5}, // FLASH_PICTURE_IDX_LIT_NUM_5
        {20, 30, FLASH_ADDR_LIT_NUM_6}, // FLASH_PICTURE_IDX_LIT_NUM_6
        {20, 30, FLASH_ADDR_LIT_NUM_7}, // FLASH_PICTURE_IDX_LIT_NUM_7
        {20, 30, FLASH_ADDR_LIT_NUM_8}, // FLASH_PICTURE_IDX_LIT_NUM_8
        {20, 30, FLASH_ADDR_LIT_NUM_9}, // FLASH_PICTURE_IDX_LIT_NUM_9
        {20, 30, FLASH_ADDR_MINUS},     // FLASH_PICTURE_IDX_MINUS
        {120, 30, FLASH_ADDR_dB},       // FLASH_PICTURE_IDX_dB
        {120, 30, FLASH_ADDR_dB_NA},    // FLASH_PICTURE_IDX_dB_NA
        {240, 240, FLASH_ADDR_MENU_3},
        {0, 0, FLASH_ADDR_MENU_3_ADD},

        {114, 114, FLASH_ADDR_GAIN},
        {114, 114, FLASH_ADDR_UAC},
        {114, 114, FLASH_ADDR_FILT},
        {114, 114, FLASH_ADDR_MENU3_TONE},

        {120, 30, FLASH_ADDR_GAIN_L},    // FLASH_PICTURE_IDX_GAIN_L
        {120, 30, FLASH_ADDR_GAIN_M},    // FLASH_PICTURE_IDX_GAIN_M
        {120, 30, FLASH_ADDR_GAIN_H},    // FLASH_PICTURE_IDX_GAIN_H
        {120, 30, FLASH_ADDR_GAIN_L_NA}, // FLASH_PICTURE_IDX_GAIN_L_NA
        {120, 30, FLASH_ADDR_GAIN_M_NA}, // FLASH_PICTURE_IDX_GAIN_M_NA
        {120, 30, FLASH_ADDR_GAIN_H_NA}, // FLASH_PICTURE_IDX_GAIN_H_NA

        {120, 30, FLASH_ADDR_UAC_1},    // FLASH_PICTURE_IDX_UAC_1
        {120, 30, FLASH_ADDR_UAC_2},    // FLASH_PICTURE_IDX_UAC_2
        {120, 30, FLASH_ADDR_UAC_1_NA}, // FLASH_PICTURE_IDX_UAC_1_NA
        {120, 30, FLASH_ADDR_UAC_2_NA}, // FLASH_PICTURE_IDX_UAC_2_NA

        {120, 30, FLASH_ADDR_TONE_NO},     // FLASH_PICTURE_IDX_TONE_NO
        {120, 30, FLASH_ADDR_TONE_YES},    // FLASH_PICTURE_IDX_TONE_YES
        {120, 30, FLASH_ADDR_TONE_NO_NA},  // FLASH_PICTURE_IDX_TONE_NO_NA
        {120, 30, FLASH_ADDR_TONE_YES_NA}, // FLASH_PICTURE_IDX_TONE_YES_NA

        {120, 30, FLASH_ADDR_FILT_0},    // FLASH_PICTURE_IDX_FILT_0
        {120, 30, FLASH_ADDR_FILT_1},    // FLASH_PICTURE_IDX_FILT_1
        {120, 30, FLASH_ADDR_FILT_2},    // FLASH_PICTURE_IDX_FILT_2
        {120, 30, FLASH_ADDR_FILT_3},    // FLASH_PICTURE_IDX_FILT_3
        {120, 30, FLASH_ADDR_FILT_4},    // FLASH_PICTURE_IDX_FILT_4
        {120, 30, FLASH_ADDR_FILT_5},    // FLASH_PICTURE_IDX_FILT_5
        {120, 30, FLASH_ADDR_FILT_0_NA}, // FLASH_PICTURE_IDX_FILT_0_NA
        {120, 30, FLASH_ADDR_FILT_1_NA}, // FLASH_PICTURE_IDX_FILT_1_NA
        {120, 30, FLASH_ADDR_FILT_2_NA}, // FLASH_PICTURE_IDX_FILT_2_NA
        {120, 30, FLASH_ADDR_FILT_3_NA}, // FLASH_PICTURE_IDX_FILT_3_NA
        {120, 30, FLASH_ADDR_FILT_4_NA}, // FLASH_PICTURE_IDX_FILT_4_NA
        {120, 30, FLASH_ADDR_FILT_5_NA}, // FLASH_PICTURE_IDX_FILT_5_NA

        {164, 41, FLASH_ADDR_AUTO},
        {164, 41, FLASH_ADDR_LCD_BRIGHTNESS},
        {164, 41, FLASH_ADDR_LOGO},
        {164, 41, FLASH_ADDR_OFF},
        {120, 30, FLASH_ADDR_2MIN},
        {120, 30, FLASH_ADDR_PERCENT_25},
        {120, 30, FLASH_ADDR_PERCENT_50},
        {120, 30, FLASH_ADDR_PERCENT_75},
        {120, 30, FLASH_ADDR_PERCENT_100},
        {120, 30, FLASH_ADDR_NO},
        {120, 30, FLASH_ADDR_YES},

        {240, 240, FLASH_ADDR_MENU_4},
        {0, 0, FLASH_ADDR_MENU_4_ADD},
        {240, 240, FLASH_ADDR_MENU_5},
        {0, 0, FLASH_ADDR_MENU_5_ADD},
};

void flash_msg_check_in(uint8_t *data, flash_picture_idx_t flash_picture_idx, flash_msg_st_t flash_msg_st)
{
    if (flash_msg_qu.total >= FLASH_MSG_FRAME_BUF_SIZE)
        return;
    if (xSemaphoreTake(flash_sema, portMAX_DELAY) == pdTRUE)
    {
        if (flash_msg_qu.total < FLASH_MSG_FRAME_BUF_SIZE)
        {
            flash_msg_qu.frame[flash_msg_qu.idx_in].msg = flash_msg_st;
            flash_msg_qu.frame[flash_msg_qu.idx_in].buf_p = data;
            flash_msg_qu.frame[flash_msg_qu.idx_in].flash_picture_idx = flash_picture_idx;
            if(flash_msg_st == FLASH_MSG_WRITE_TONE)
            {
                flash_msg_qu.frame[flash_msg_qu.idx_in].tone_data[0] = data[0];
                flash_msg_qu.frame[flash_msg_qu.idx_in].tone_data[1] = data[1];
            }
            // flash_msg_qu.frame[flash_msg_qu.idx_in].length = 2*flash_picture_array[flash_picture_idx].width*flash_picture_array[flash_picture_idx].height;
            if (++flash_msg_qu.idx_in >= FLASH_MSG_FRAME_BUF_SIZE)
            {
                flash_msg_qu.idx_in = 0;
            }
            flash_msg_qu.total++;
        }
    }
    xSemaphoreGive(flash_sema);
}

void flash_msg_check_out()
{
    if (xSemaphoreTake(flash_sema, portMAX_DELAY) == pdTRUE)
    {
        if (++flash_msg_qu.idx_out >= FLASH_MSG_FRAME_BUF_SIZE)
        {
            flash_msg_qu.idx_out = 0;
        }
        flash_msg_qu.total--;
    }
    xSemaphoreGive(flash_sema);
}

void flash_tx_buf_setting(uint8_t cmd, uint32_t addr, uint8_t *data, uint32_t data_length)
{
    uint16_t offset = 0;
    if (cmd == FLASH_CMD_INVAL && addr == FLASH_ADDR_INVAL && data == NULL)
    {
        return;
    }
    if (cmd != FLASH_CMD_INVAL)
    {
        flash_tx_buf.buf[0] = cmd;

        offset += 1;
    }
    if (addr != FLASH_ADDR_INVAL)
    {
        for (uint16_t i = 0; i < FLASH_ADDR_SIZE; i++)
        {
            flash_tx_buf.buf[offset + i] = ((uint8_t *)&addr)[FLASH_ADDR_SIZE - i - 1];
        }
        offset += FLASH_ADDR_SIZE;
    }
    if (data_length != 0 && data != NULL)
    {
        for (uint16_t i = 0; i < data_length; i++)
        {
            flash_tx_buf.buf[offset + i] = data[i];
        }
        offset += data_length;
    }
    flash_tx_buf.length = offset;
    flash_tx_buf.idx = 0;
}

uint16_t flash_page_total = 0;
uint16_t flash_page_idx = 0;
uint16_t flash_instruction_tmp = 0;
void flash_msg_proc()
{
    switch (flash_msg_qu.frame[flash_msg_qu.idx_out].msg)
    {
    case FLASH_MSG_READ_BACKGROUND:
        flash_st = FLASH_ST_READ_DATA_INSTRUCTION;
        flash_blk.flash_addr_tmp = flash_picture_array[flash_msg_qu.frame[flash_msg_qu.idx_out].flash_picture_idx].addr; // FLASH_ADDR_BACKGROUND;
        break;
    case FLASH_MSG_WRITE_IDX:
        flash_st = FLASH_ST_WRITE_START;
        flash_blk.flash_addr_tmp = flash_picture_array[flash_msg_qu.frame[flash_msg_qu.idx_out].flash_picture_idx].addr;
        break;
    case FLASH_MSG_WRITE_ONLY:
        flash_st = FLASH_ST_PAGE_PROGRAM_WRITE_ENABLE;
        flash_blk.flash_addr_tmp = flash_picture_array[flash_msg_qu.frame[flash_msg_qu.idx_out].flash_picture_idx].addr;
        break;
    case FLASH_MSG_BLOCK_ERASE:
        flash_st = FLASH_ST_ERASE_WRITE_ENABLE;
        flash_blk.flash_addr_tmp = flash_picture_array[flash_msg_qu.frame[flash_msg_qu.idx_out].flash_picture_idx].addr;

        flash_instruction_tmp = FLASH_CMD_BLOCK_ERASE_64;
        break;
    case FLASH_MSG_WRITE_TONE:
        flash_st = TONE_ST_WRITE_START;

        break;
    default:
        break;
    }
    flash_msg_qu.frame[flash_msg_qu.idx_out].length = flash_to_lcd.flash_remaining_length = flash_picture_array[flash_msg_qu.frame[flash_msg_qu.idx_out].flash_picture_idx].width * flash_picture_array[flash_msg_qu.frame[flash_msg_qu.idx_out].flash_picture_idx].height * 2;
    flash_page_total = ((flash_msg_qu.frame[flash_msg_qu.idx_out].length + 255) >> 8);
    flash_page_idx = 0;
    flash_blk.last_msg = flash_msg_qu.frame[flash_msg_qu.idx_out].msg;
}
unsigned short modem_bus_crc16(const unsigned char *buf, unsigned short len, uint16_t crc)
{
    // unsigned short crc = 0xFFFF;
    for (int i = 0; i < len; i++)
    {
        crc ^= (unsigned short)buf[i];
        for (int j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}
void flash_task(void *pvParameters)
{
    #ifdef PICTURE_DL1
    //MENU 1
    flash_msg_check_in(NULL, FLASH_PICTURE_IDX_MENU_1_ADD, FLASH_MSG_BLOCK_ERASE);
    flash_msg_check_in(image_menu_1, FLASH_PICTURE_IDX_MENU_1, FLASH_MSG_WRITE_IDX);
      //BOT LEFT
      flash_msg_check_in(image_LO,FLASH_PICTURE_IDX_LO,FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_PO,FLASH_PICTURE_IDX_PO,FLASH_MSG_WRITE_ONLY);
      flash_msg_check_in(image_R_X,FLASH_PICTURE_IDX_R_X,FLASH_MSG_WRITE_IDX);

    //BOT RIGHT
     flash_msg_check_in(image_TONE,FLASH_PICTURE_IDX_TONE,FLASH_MSG_WRITE_IDX);
     flash_msg_check_in(image_TONE_2,FLASH_PICTURE_IDX_TONE_2,FLASH_MSG_WRITE_ONLY);
     flash_msg_check_in(image_VOL,FLASH_PICTURE_IDX_VOL,FLASH_MSG_WRITE_IDX);
     flash_msg_check_in(image_VOL_2,FLASH_PICTURE_IDX_VOL_2,FLASH_MSG_WRITE_ONLY);
    #endif
    #ifdef PICTURE_DL2
    //TOP LEFT
    flash_msg_check_in(image_DSD_64, FLASH_PICTURE_IDX_DSD_64, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_DSD_128, FLASH_PICTURE_IDX_DSD_128, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_DSD_256, FLASH_PICTURE_IDX_DSD_256, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_DSD_512, FLASH_PICTURE_IDX_DSD_512, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_PCM_44, FLASH_PICTURE_IDX_PCM_44, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_PCM_48, FLASH_PICTURE_IDX_PCM_48, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_PCM_88, FLASH_PICTURE_IDX_PCM_88, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_PCM_96, FLASH_PICTURE_IDX_PCM_96, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_PCM_176, FLASH_PICTURE_IDX_PCM_176, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_PCM_192, FLASH_PICTURE_IDX_PCM_192, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_PCM_352, FLASH_PICTURE_IDX_PCM_352, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_PCM_384, FLASH_PICTURE_IDX_PCM_384, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_PCM_705, FLASH_PICTURE_IDX_PCM_705, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_PCM_768, FLASH_PICTURE_IDX_PCM_768, FLASH_MSG_WRITE_ONLY);
    //TOP RIGHT
    flash_msg_check_in(image_COAX, FLASH_PICTURE_IDX_COAX, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_OPT, FLASH_PICTURE_IDX_OPT, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_RCA, FLASH_PICTURE_IDX_RCA, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_USB, FLASH_PICTURE_IDX_USB, FLASH_MSG_WRITE_ONLY);
    #endif
    #ifdef PICTURE_DL3
    //NUM
    flash_msg_check_in(image_MUTE, FLASH_PICTURE_IDX_MUTE, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_NUM_0, FLASH_PICTURE_IDX_NUM_0, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_NUM_1, FLASH_PICTURE_IDX_NUM_1, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_NUM_2, FLASH_PICTURE_IDX_NUM_2, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_NUM_3, FLASH_PICTURE_IDX_NUM_3, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_NUM_4, FLASH_PICTURE_IDX_NUM_4, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_NUM_5, FLASH_PICTURE_IDX_NUM_5, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_NUM_6, FLASH_PICTURE_IDX_NUM_6, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_NUM_7, FLASH_PICTURE_IDX_NUM_7, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_NUM_8, FLASH_PICTURE_IDX_NUM_8, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_NUM_9, FLASH_PICTURE_IDX_NUM_9, FLASH_MSG_WRITE_ONLY);
    //MENU 2
    flash_msg_check_in(NULL, FLASH_PICTURE_IDX_MENU_2_ADD, FLASH_MSG_BLOCK_ERASE);
    flash_msg_check_in(image_menu_2, FLASH_PICTURE_IDX_MENU_2, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_INPUT, FLASH_PICTURE_IDX_INPUT, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_OUTPUT, FLASH_PICTURE_IDX_OUTPUT, FLASH_MSG_WRITE_ONLY);
    #endif
    #ifdef PICTURE_DL4
    // //OUTPUT INPUT
    flash_msg_check_in(image_BASS, FLASH_PICTURE_IDX_BASS, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_TREBLE, FLASH_PICTURE_IDX_TREBLE, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_INP_COAX, FLASH_PICTURE_IDX_INP_COAX, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_INP_RCA, FLASH_PICTURE_IDX_INP_RCA, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_INP_USB, FLASH_PICTURE_IDX_INP_USB, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_INP_OPT, FLASH_PICTURE_IDX_INP_OPT, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_OUT_PO, FLASH_PICTURE_IDX_OUT_PO, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_OUT_LO, FLASH_PICTURE_IDX_OUT_LO, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_OUT_R_X, FLASH_PICTURE_IDX_OUT_R_X, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_DOT_GRAY, FLASH_PICTURE_IDX_DOT_GRAY, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_DOT_ORANGE, FLASH_PICTURE_IDX_DOT_ORANGE, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_INP_COAX_NA, FLASH_PICTURE_IDX_INP_COAX_NA, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_INP_RCA_NA, FLASH_PICTURE_IDX_INP_RCA_NA, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_INP_USB_NA, FLASH_PICTURE_IDX_INP_USB_NA, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_INP_OPT_NA, FLASH_PICTURE_IDX_INP_OPT_NA, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_OUT_PO_NA, FLASH_PICTURE_IDX_OUT_PO_NA, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_OUT_LO_NA, FLASH_PICTURE_IDX_OUT_LO_NA, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_OUT_R_X_NA, FLASH_PICTURE_IDX_OUT_R_X_NA, FLASH_MSG_WRITE_ONLY);
    #endif
    #ifdef PICTURE_DL5
    //LITTLE NUM
    flash_msg_check_in(image_LIT_NUM_0, FLASH_PICTURE_IDX_LIT_NUM_0, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_LIT_NUM_1, FLASH_PICTURE_IDX_LIT_NUM_1, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_LIT_NUM_2, FLASH_PICTURE_IDX_LIT_NUM_2, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_LIT_NUM_3, FLASH_PICTURE_IDX_LIT_NUM_3, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_LIT_NUM_4, FLASH_PICTURE_IDX_LIT_NUM_4, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_LIT_NUM_5, FLASH_PICTURE_IDX_LIT_NUM_5, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_LIT_NUM_6, FLASH_PICTURE_IDX_LIT_NUM_6, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_LIT_NUM_7, FLASH_PICTURE_IDX_LIT_NUM_7, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_LIT_NUM_8, FLASH_PICTURE_IDX_LIT_NUM_8, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_LIT_NUM_9, FLASH_PICTURE_IDX_LIT_NUM_9, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_MINUS, FLASH_PICTURE_IDX_MINUS, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_dB, FLASH_PICTURE_IDX_dB, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_dB_NA, FLASH_PICTURE_IDX_dB_NA, FLASH_MSG_WRITE_ONLY);
    //MENU 3
    flash_msg_check_in(NULL, FLASH_PICTURE_IDX_MENU_3_ADD, FLASH_MSG_BLOCK_ERASE);
    flash_msg_check_in(image_menu_3, FLASH_PICTURE_IDX_MENU_3, FLASH_MSG_WRITE_IDX);
    #endif
    #ifdef PICTURE_DL6
     flash_msg_check_in(image_GAIN, FLASH_PICTURE_IDX_GAIN, FLASH_MSG_WRITE_IDX);
     flash_msg_check_in(image_UAC, FLASH_PICTURE_IDX_UAC, FLASH_MSG_WRITE_ONLY);
     flash_msg_check_in(image_FILT, FLASH_PICTURE_IDX_FILT, FLASH_MSG_WRITE_IDX);
     flash_msg_check_in(image_MENU3_TONE, FLASH_PICTURE_IDX_MENU3_TONE, FLASH_MSG_WRITE_ONLY);

    flash_msg_check_in(image_GAIN_H, FLASH_PICTURE_IDX_GAIN_H, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_GAIN_H_NA, FLASH_PICTURE_IDX_GAIN_H_NA, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_GAIN_L, FLASH_PICTURE_IDX_GAIN_L, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_GAIN_L_NA, FLASH_PICTURE_IDX_GAIN_L_NA, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_GAIN_M, FLASH_PICTURE_IDX_GAIN_M, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_GAIN_M_NA, FLASH_PICTURE_IDX_GAIN_M_NA, FLASH_MSG_WRITE_ONLY);

    flash_msg_check_in(image_UAC_1, FLASH_PICTURE_IDX_UAC_1, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_UAC_1_NA, FLASH_PICTURE_IDX_UAC_1_NA, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_UAC_2, FLASH_PICTURE_IDX_UAC_2, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_UAC_2_NA, FLASH_PICTURE_IDX_UAC_2_NA, FLASH_MSG_WRITE_ONLY);

    flash_msg_check_in(image_TONE_NO, FLASH_PICTURE_IDX_TONE_NO, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_TONE_NO_NA, FLASH_PICTURE_IDX_TONE_NO_NA, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_TONE_YES, FLASH_PICTURE_IDX_TONE_YES, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_TONE_YES_NA, FLASH_PICTURE_IDX_TONE_YES_NA, FLASH_MSG_WRITE_ONLY);
    
    flash_msg_check_in(image_FILT_0, FLASH_PICTURE_IDX_FILT_0, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_FILT_1, FLASH_PICTURE_IDX_FILT_1, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_FILT_2, FLASH_PICTURE_IDX_FILT_2, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_FILT_3, FLASH_PICTURE_IDX_FILT_3, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_FILT_4, FLASH_PICTURE_IDX_FILT_4, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_FILT_5, FLASH_PICTURE_IDX_FILT_5, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_FILT_0_NA, FLASH_PICTURE_IDX_FILT_0_NA, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_FILT_1_NA, FLASH_PICTURE_IDX_FILT_1_NA, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_FILT_2_NA, FLASH_PICTURE_IDX_FILT_2_NA, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_FILT_3_NA, FLASH_PICTURE_IDX_FILT_3_NA, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_FILT_4_NA, FLASH_PICTURE_IDX_FILT_4_NA, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_FILT_5_NA, FLASH_PICTURE_IDX_FILT_5_NA, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_AUTO, FLASH_PICTURE_IDX_AUTO, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_LCD_brightness, FLASH_PICTURE_IDX_LCD_BRIGHTNESS, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_LOGO, FLASH_PICTURE_IDX_LOGO, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_OFF, FLASH_PICTURE_IDX_OFF, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_2min, FLASH_PICTURE_IDX_2MIN, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(image_percent_25, FLASH_PICTURE_IDX_PERCENT_25, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_percent_50, FLASH_PICTURE_IDX_PERCENT_50, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_percent_75, FLASH_PICTURE_IDX_PERCENT_75, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_percent_100, FLASH_PICTURE_IDX_PERCENT_100, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_YES, FLASH_PICTURE_IDX_YES, FLASH_MSG_WRITE_ONLY);
    flash_msg_check_in(image_NO, FLASH_PICTURE_IDX_NO, FLASH_MSG_WRITE_ONLY);
    #endif
    #ifdef PICTURE_DL7
    flash_msg_check_in(NULL, FLASH_PICTURE_IDX_MENU_4_ADD, FLASH_MSG_BLOCK_ERASE);
    flash_msg_check_in(image_menu4, FLASH_PICTURE_IDX_MENU_4, FLASH_MSG_WRITE_IDX);
    flash_msg_check_in(NULL, FLASH_PICTURE_IDX_MENU_5_ADD, FLASH_MSG_BLOCK_ERASE);
    flash_msg_check_in(image_menu5, FLASH_PICTURE_IDX_MENU_5, FLASH_MSG_WRITE_IDX);
    #endif
    while (1)
    {
        switch (flash_st)
        {
        case FLASH_ST_IDL:
            if (flash_msg_qu.total)
            {
                flash_msg_proc();
            }
            break;
        case FLASH_ST_WRITE_START:
            flash_st = FLASH_ST_ERASE_WRITE_ENABLE;
            break;
        case FLASH_ST_ERASE_WRITE_ENABLE:
        {
            uint8_t cmd = FLASH_CMD_WRITE_ENABLE;
            uint32_t addr = FLASH_ADDR_INVAL;
            uint8_t *data = NULL;
            uint32_t data_length = 0;
            flash_tx_buf_setting(cmd, addr, data, data_length);
            FLASH_CS_ENABLE;
            flash_st = FLASH_ST_ERASE_WRITE_ENABLE_WAIT;
        }
        break;
        case FLASH_ST_ERASE_WRITE_ENABLE_WAIT:
            if (!flash_blk.flash_tx_busy)
            {
                FLASH_CS_DISABLE;
                flash_st = FLASH_ST_ERASE;
            }
            break;
        case FLASH_ST_ERASE:
        {
            uint8_t cmd = FLASH_CMD_BLOCK_ERASE_64;
            uint32_t addr = flash_blk.flash_addr_tmp;
            uint8_t *data = NULL;
            uint32_t data_length = 0;
            flash_tx_buf_setting(cmd, addr, data, data_length);
            FLASH_CS_ENABLE;
            flash_st = FLASH_ST_ERASE_WAIT;
        }
        break;
        case FLASH_ST_ERASE_WAIT:
            if (!flash_blk.flash_tx_busy)
            {
                FLASH_CS_DISABLE;
                flash_st = FLASH_ST_ERASE_Read_Status_Register1_BUSY;
            }
            break;

        case FLASH_ST_ERASE_Read_Status_Register1_BUSY:
        {
            uint8_t cmd = FLASH_CMD_Read_Status_Register1;
            uint32_t addr = FLASH_ADDR_INVAL;
            uint8_t *data = NULL;
            uint32_t data_length = 0;
            flash_tx_buf_setting(cmd, addr, data, data_length);
            FLASH_CS_ENABLE;
            flash_st = FLASH_ST_ERASE_Read_Status_Register1_BUSY_WAIT;
        }
        break;
        case FLASH_ST_ERASE_Read_Status_Register1_BUSY_WAIT:
            if (!flash_blk.flash_tx_busy)
            {
                flash_st = FLASH_ST_ERASE_DUMMY;
            }
            break;
        case FLASH_ST_ERASE_DUMMY:
        {
            uint8_t cmd = FLASH_CMD_INVAL;
            uint32_t addr = FLASH_ADDR_INVAL;
            uint8_t *data = dummy;
            uint32_t data_length = 1;
            flash_tx_buf_setting(cmd, addr, data, data_length);
            spi_i2s_data_receive(SPI1);
            spi_i2s_interrupt_enable(SPI1, SPI_I2S_INT_RBNE);
            flash_rx_buf.idx = 0;
            flash_rx_buf.length = flash_tx_buf.length;
            flash_blk.flash_rx_busy = 1;

            flash_st = FLASH_ST_ERASE_DUMMY_WAIT;
        }
        break;
        case FLASH_ST_ERASE_DUMMY_WAIT:
            if (!flash_blk.flash_tx_busy && !flash_blk.flash_rx_busy)
            {
                if (flash_rx_buf.buf[0] & 0x01)
                {
                    flash_st = FLASH_ST_ERASE_DUMMY;
                }
                else
                {
                    FLASH_CS_DISABLE;
                    if (flash_blk.last_msg == FLASH_MSG_BLOCK_ERASE)
                    {
                        flash_st = FLASH_ST_CHECK_OUT;
                    }
                    else
                    {
                        flash_st = FLASH_ST_PAGE_PROGRAM_WRITE_ENABLE;
                    }
                }
            }
            break;

        case FLASH_ST_PAGE_PROGRAM_WRITE_ENABLE:
        {
            uint8_t cmd = FLASH_CMD_WRITE_ENABLE;
            uint32_t addr = FLASH_ADDR_INVAL;
            uint8_t *data = NULL;
            uint32_t data_length = 0;
            flash_tx_buf_setting(cmd, addr, data, data_length);
            FLASH_CS_ENABLE;
            flash_st = FLASH_ST_PAGE_PROGRAM_WRITE_ENABLE_WAIT;
        }
        break;
        case FLASH_ST_PAGE_PROGRAM_WRITE_ENABLE_WAIT:
            if (!flash_blk.flash_tx_busy)
            {
                FLASH_CS_DISABLE;
                flash_st = FLASH_ST_PAGE_PROGRAM_INSTRUCTION;
            }
            break;
        case FLASH_ST_PAGE_PROGRAM_INSTRUCTION:
        {
            uint8_t cmd = FLASH_CMD_Page_Program;
            uint32_t addr = flash_blk.flash_addr_tmp;
            uint8_t *data = NULL;
            uint32_t data_length = 0;
            flash_tx_buf_setting(cmd, addr, data, data_length);
            FLASH_CS_ENABLE;
            flash_st = FLASH_ST_PAGE_PROGRAM_INSTRUCTION_WAIT;
            flash_blk.flash_rx_busy = 1;
        }
        break;
        case FLASH_ST_PAGE_PROGRAM_INSTRUCTION_WAIT:
            if (!flash_blk.flash_tx_busy)
            {
                flash_st = FLASH_ST_PAGE_PROGRAM_DATA;
            }
            break;
        case FLASH_ST_PAGE_PROGRAM_DATA:
        {
            uint8_t cmd = FLASH_CMD_INVAL;
            uint32_t addr = FLASH_ADDR_INVAL;
            uint8_t *data = &flash_msg_qu.frame[flash_msg_qu.idx_out].buf_p[flash_page_idx * FLASH_PROGRAM_PAGE_MAX];
            uint32_t data_length = flash_msg_qu.frame[flash_msg_qu.idx_out].length - flash_page_idx * FLASH_PROGRAM_PAGE_MAX;

            if (data_length > FLASH_PROGRAM_PAGE_MAX ||
                flash_msg_qu.frame[flash_msg_qu.idx_out].length - flash_page_idx * FLASH_PROGRAM_PAGE_MAX > 65535)
            {
                data_length = FLASH_PROGRAM_PAGE_MAX;
            }
            flash_tx_buf_setting(cmd, addr, data, data_length);
            flash_st = FLASH_ST_PAGE_PROGRAM_DATA_WAIT;
            // flash_blk.flash_rx_busy = 1;
        }
        break;
        case FLASH_ST_PAGE_PROGRAM_DATA_WAIT:
        {
            if (!flash_blk.flash_tx_busy)
            {
                FLASH_CS_DISABLE;
                flash_st = FLASH_ST_PAGE_PROGRAME_Read_Status_Register1_BUSY;
            }
        }
        break;
        case FLASH_ST_PAGE_PROGRAME_Read_Status_Register1_BUSY:
        {
            uint8_t cmd = FLASH_CMD_Read_Status_Register1;
            uint32_t addr = FLASH_ADDR_INVAL;
            uint8_t *data = NULL;
            uint32_t data_length = 0;
            flash_tx_buf_setting(cmd, addr, data, data_length);
            FLASH_CS_ENABLE;
            flash_st = FLASH_ST_PAGE_PROGRAME_Read_Status_Register1_BUSY_WAIT;
        }
        break;
        case FLASH_ST_PAGE_PROGRAME_Read_Status_Register1_BUSY_WAIT:
            if (!flash_blk.flash_tx_busy)
            {
                flash_st = FLASH_ST_PAGE_PROGRAME_DUMMY;
            }
            break;
        case FLASH_ST_PAGE_PROGRAME_DUMMY:
        {
            uint8_t cmd = FLASH_CMD_INVAL;
            uint32_t addr = FLASH_ADDR_INVAL;
            uint8_t *data = dummy;
            uint32_t data_length = 1;
            flash_tx_buf_setting(cmd, addr, data, data_length);
            spi_i2s_data_receive(SPI1);
            spi_i2s_interrupt_enable(SPI1, SPI_I2S_INT_RBNE);
            flash_rx_buf.idx = 0;
            flash_rx_buf.length = flash_tx_buf.length;
            flash_blk.flash_rx_busy = 1;

            flash_st = FLASH_ST_PAGE_PROGRAME_DUMMY_WAIT;
        }
        break;
        case FLASH_ST_PAGE_PROGRAME_DUMMY_WAIT:
            if (!flash_blk.flash_tx_busy && !flash_blk.flash_rx_busy)
            {
                if (flash_rx_buf.buf[0] & 0x01)
                {
                    flash_st = FLASH_ST_PAGE_PROGRAME_DUMMY;
                }
                else
                {
                    if (++flash_page_idx >= flash_page_total)
                    {
                        flash_st = FLASH_ST_CHECK_OUT;
                    }
                    else
                    {
                        FLASH_CS_DISABLE;
                        flash_st = FLASH_ST_PAGE_PROGRAM_WRITE_ENABLE;
                        flash_blk.flash_addr_tmp += 256;
                    }
                }
            }
            break;

        case FLASH_ST_READ_DATA_INSTRUCTION:
        {
            // uint8_t cmd = FLASH_CMD_READ_DATA;
            // uint32_t addr = flash_blk.flash_addr_tmp;

            // uint8_t *data = NULL;
            // uint32_t data_length = 0;
            uint8_t cmd = FLASH_CMD_FAST_READ;
            uint32_t addr = flash_blk.flash_addr_tmp;

            uint8_t *data = dummy;
            uint32_t data_length = 1;
            FLASH_CS_ENABLE;
            flash_tx_buf_setting(cmd, addr, data, data_length);
            flash_st = FLASH_ST_READ_DATA_INSTRUCTION_WAIT;
        }
        break;
        case FLASH_ST_READ_DATA_INSTRUCTION_WAIT:
            if (!flash_blk.flash_tx_busy)
            {
                flash_st = FLASH_ST_READ_DATA_DUMMY;
            }
            break;
        case FLASH_ST_READ_DATA_DUMMY:
        {
            if (flash_to_lcd.flash_rx_dma_cplt)
                break;
            if (flash_to_lcd.flash_remaining_length > LCD_SEND_FRAME_SIZE)
            {
                flash_to_lcd.flash_dma_length = LCD_SEND_FRAME_SIZE;
                flash_to_lcd.flash_remaining_length -= LCD_SEND_FRAME_SIZE;
            }
            else
            {
                flash_to_lcd.flash_dma_length = flash_to_lcd.flash_remaining_length;
                flash_to_lcd.flash_remaining_length = 0;
            }
            flash_blk.flash_rx_busy = 1;
            spi_i2s_data_receive(SPI1);
            // spi_disable(SPI0);
            flash_rx_dma_config();
            flash_tx_dma_config();

            // spi_enable(SPI0);

            flash_st = FLASH_ST_READ_DATA_DUMMY_WAIT;
        }
        break;
        case FLASH_ST_READ_DATA_DUMMY_WAIT:
            if (flash_blk.flash_rx_busy)
                break;
            if (flash_to_lcd.flash_remaining_length)
            {
                if (!spi_i2s_flag_get(SPI1, SPI_FLAG_TRANS))
                    flash_st = FLASH_ST_READ_DATA_DUMMY;
            }
            else
            {
                flash_st = FLASH_ST_CHECK_OUT;
            }
            break;

        case FLASH_ST_CHECK_OUT:
            FLASH_CS_DISABLE;
            flash_msg_check_out();
            flash_st = FLASH_ST_IDL;
            break;
        case TONE_ST_WRITE_START:
            gpio_bit_reset(GPIO_TONE_CS_PORT, GPIO_TONE_CS_PIN);
            tone_config();

            flash_st = TONE_ST_WRITE_DATA;
            break;
        case TONE_ST_WRITE_DATA:
        {
            uint8_t cmd = FLASH_CMD_INVAL;
            uint32_t addr = FLASH_ADDR_INVAL;
            uint8_t *data = flash_msg_qu.frame[flash_msg_qu.idx_in].tone_data;
            uint32_t data_length = 2;
            flash_tx_buf_setting(cmd, addr, data, data_length);
        }
            flash_st = TONE_ST_WRITE_DATA_WAITING;
            break;
        case TONE_ST_WRITE_DATA_WAITING:
            if (flash_blk.flash_tx_busy)
            {
                break;
            }
            flash_st = TONE_ST_WRITE_COMPLETE;
            break;
        case TONE_ST_WRITE_CHA_TREBLE_DATA:
        {
            uint8_t cmd = FLASH_CMD_INVAL;
            uint32_t addr = flash_blk.flash_addr_tmp;
            uint8_t *data = NULL;
            uint32_t data_length = 1;
            flash_tx_buf_setting(cmd, addr, data, data_length);
        }
            flash_st = TONE_ST_WRITE_CHA_TREBLE_DATA_WAITING;
            break;
        case TONE_ST_WRITE_CHA_TREBLE_DATA_WAITING:
            if (!flash_blk.flash_tx_busy)
            {
                flash_st = TONE_ST_WRITE_CHA_BASS_DATA;
            }

            break;
        case TONE_ST_WRITE_CHA_BASS_DATA:

            flash_st = TONE_ST_WRITE_CHA_BASS_DATA_WAITING;
            break;
        case TONE_ST_WRITE_CHA_BASS_DATA_WAITING:
            if (!flash_blk.flash_tx_busy)
            {
                flash_st = TONE_ST_WRITE_CHB_TREBLE_DATA;
            }
            break;
        case TONE_ST_WRITE_CHB_TREBLE_DATA:
        {
            uint8_t cmd = FLASH_CMD_INVAL;
            uint32_t addr = flash_blk.flash_addr_tmp;
            uint8_t *data = NULL;
            uint32_t data_length = 1;
            flash_tx_buf_setting(cmd, addr, data, data_length);
        }
            flash_st = TONE_ST_WRITE_CHB_TREBLE_DATA_WAITING;
            break;
        case TONE_ST_WRITE_CHB_TREBLE_DATA_WAITING:
            if (!flash_blk.flash_tx_busy)
            {
                flash_st = TONE_ST_WRITE_CHB_BASS_DATA;
            }
            break;
        case TONE_ST_WRITE_CHB_BASS_DATA:

            flash_st = TONE_ST_WRITE_CHB_BASS_DATA_WAITING;
            break;
        case TONE_ST_WRITE_CHB_BASS_DATA_WAITING:
            if (!flash_blk.flash_tx_busy)
            {
                flash_st = TONE_ST_WRITE_COMPLETE;
            }
            break;
        case TONE_ST_WRITE_COMPLETE:
            flash_config();
            flash_msg_check_out();
            break;
        default:
            break;
        }
        if (!flash_blk.flash_tx_busy && flash_tx_buf.idx == 0 && flash_tx_buf.length)
        {
            flash_blk.flash_tx_busy = 1;
            spi_i2s_interrupt_enable(SPI1, SPI_I2S_INT_TBE);
        }
        // if(flash_safe_cnt&&--flash_safe_cnt==0)
        // {
        //     flash_config();
        //     flash_st = FLASH_ST_IDL;
        //     flash_blk.flash_rx_busy = flash_blk.flash_tx_busy = 0;
        //     flash_tx_buf.length = flash_tx_buf.idx = 0;
        // }
        vTaskDelay(FLASH_TASK_DELAY);
    }
}

void flash_spi1_callback()
{
    if (spi_i2s_interrupt_flag_get(SPI1, SPI_I2S_INT_FLAG_TBE))
    {
        spi_i2s_data_transmit(SPI1, flash_tx_buf.buf[flash_tx_buf.idx]);
        if (++flash_tx_buf.idx >= flash_tx_buf.length)
        {
            // while(spi_i2s_flag_get(SPI0,SPI_FLAG_TRANS))
            // {
            // 				uint16_t i=0;
            // }
            flash_blk.flash_tx_busy = 0;
            spi_i2s_interrupt_disable(SPI1, SPI_I2S_INT_TBE);
        }
    }
    if (spi_i2s_interrupt_flag_get(SPI1, SPI_I2S_INT_FLAG_RBNE))
    {
        flash_rx_buf.buf[flash_rx_buf.idx] = spi_i2s_data_receive(SPI1);
        if (++flash_rx_buf.idx >= flash_rx_buf.length)
        {
            flash_blk.flash_rx_busy = 0;
            spi_i2s_interrupt_disable(SPI1, SPI_I2S_INT_RBNE);
        }
    }
}

void flash_dma_callback()
{
    if (dma_interrupt_flag_get(DMA0, DMA_CH3, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH3, DMA_INT_FLAG_FTF);

        lcd_busy = 0;
        lcd_frame.length = flash_to_lcd.flash_dma_length;
        lcd_frame.idx = 0;
        flash_to_lcd.flash_rx_dma_cplt = 1;
        flash_blk.flash_rx_busy = 0;
        // for (uint16_t j = 0; j < lcd_frame.length; j += 2)
        // {
        //     uint8_t k = lcd_frame.buf[j + 1];
        //     lcd_frame.buf[j + 1] = lcd_frame.buf[j];
        //     lcd_frame.buf[j] = k;
        // }
        spi_i2s_interrupt_disable(SPI1, SPI_I2S_INT_TBE);
    }
}
void flash_tx_dma_config()
{
    dma_parameter_struct dma_init_struct;
    // dma_struct_para_init(&dma_init_struct);
    dma_channel_disable(DMA0, DMA_CH4);
    /* configure SPI0 transmit DMA: DMA_CH4 */
    // dma_deinit(DMA_CH4);
    dma_init_struct.periph_addr = (uint32_t)&SPI_DATA(SPI1);
    dma_init_struct.memory_addr = (uint32_t)dummy;
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_init_struct.number = flash_to_lcd.flash_dma_length;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_DISABLE;
    dma_init(DMA0, DMA_CH4, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH4);
    dma_memory_to_memory_disable(DMA0, DMA_CH4);

    dma_channel_enable(DMA0, DMA_CH4);
    spi_dma_enable(SPI1, SPI_DMA_TRANSMIT);
    // spi_enable(SPI0);
}

void flash_rx_dma_config()
{
    dma_parameter_struct dma_init_struct;
    // dma_struct_para_init(&dma_init_struct);
    dma_channel_disable(DMA0, DMA_CH3);
    /* configure SPI0 receive DMA: DMA_CH3 */
    // dma_deinit(DMA_CH3);
    dma_init_struct.periph_addr = (uint32_t)&SPI_DATA(SPI1);
    dma_init_struct.memory_addr = (uint32_t)lcd_frame.buf;
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_init_struct.number = flash_to_lcd.flash_dma_length;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init(DMA0, DMA_CH3, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH3);
    dma_memory_to_memory_disable(DMA0, DMA_CH3);

    dma_channel_enable(DMA0, DMA_CH3);
    spi_dma_enable(SPI1, SPI_DMA_RECEIVE);
    // spi_enable(SPI0);
}

void flash_init()
{
    for (uint16_t i = 0; i < sizeof(flash_blk); i++)
    {
        *((uint8_t *)&flash_blk + i) = 0;
    }
}
