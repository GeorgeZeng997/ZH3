#include "config.h"

void display_test(uint8_t cnt)
{
    switch (cnt)
    {
    case 0:
        lcd_display_picture(COORD_USB_X, COORD_USB_Y, FLASH_PICTURE_IDX_USB);
        break;
    case 1:
        lcd_display_picture(COORD_OPT_X, COORD_OPT_Y, FLASH_PICTURE_IDX_OPT);
        break;
    case 2:
        lcd_display_picture(COORD_COA_X, COORD_COA_Y, FLASH_PICTURE_IDX_COA);
        break;
    case 3:
        lcd_display_picture(COORD_1_0_X, COORD_1_0_Y, FLASH_PICTURE_IDX_1_0);
        break;
    case 4:
        lcd_display_picture(COORD_2_0_X, COORD_2_0_Y, FLASH_PICTURE_IDX_2_0);
        break;
    case 5:
        lcd_display_picture(COORD_BYPASS_X, COORD_BYPASS_Y, FLASH_PICTURE_IDX_BYPASS);
        break;
    case 6:
        lcd_display_picture(COORD_LINE_X, COORD_LINE_Y, FLASH_PICTURE_IDX_LINE);
        break;
    case 7:
        
        break;
    case 8:
        lcd_display_picture(COORD_LO_X, COORD_LO_Y, FLASH_PICTURE_IDX_LO);
        break;
    case 9:
        lcd_display_picture(COORD_RCA_X, COORD_RCA_Y, FLASH_PICTURE_IDX_RCA);
        break;
    case 10:
        lcd_display_picture(COORD_PH_X, COORD_PH_Y, FLASH_PICTURE_IDX_PH);
        break;
    case 11:
        lcd_display_picture(COORD_MUTE_ENABLE_X, COORD_MUTE_ENABLE_Y, FLASH_PICTURE_IDX_MUTE_ENABLE);
        break;
    case 12:
        lcd_display_picture(COORD_MUTE_DISABLE_X, COORD_MUTE_DISABLE_Y, FLASH_PICTURE_IDX_MUTE_DISABLE);
        break;
    case 13:
    lcd_display_picture(COORD_DSD_X, COORD_DSD_Y, FLASH_PICTURE_IDX_DSD);
        
        break;
    case 14:
        lcd_display_picture(COORD_DSD_64_X, COORD_DSD_64_Y, FLASH_PICTURE_IDX_DSD_64);
        break;
    case 15:
        lcd_display_picture(COORD_DSD_128_X, COORD_DSD_128_Y, FLASH_PICTURE_IDX_DSD_128);
        break;
    case 16:
        lcd_display_picture(COORD_DSD_256_X, COORD_DSD_256_Y, FLASH_PICTURE_IDX_DSD_256);
        break;
    case 17:
        lcd_display_picture(COORD_DSD_512_X, COORD_DSD_512_Y, FLASH_PICTURE_IDX_DSD_512);
        break;
    case 18:
        lcd_display_picture(COORD_HZ_X, COORD_HZ_Y, FLASH_PICTURE_IDX_HZ);
        lcd_display_picture(COORD_PCM_44_X, COORD_PCM_44_Y, FLASH_PICTURE_IDX_PCM_44);
        break;
    case 19:
        lcd_display_picture(COORD_PCM_48_X, COORD_PCM_48_Y, FLASH_PICTURE_IDX_PCM_48);
        break;
    case 20:
        lcd_display_picture(COORD_PCM_88_X, COORD_PCM_88_Y, FLASH_PICTURE_IDX_PCM_88);
        break;
    case 21:
        lcd_display_picture(COORD_PCM_96_X, COORD_PCM_96_Y, FLASH_PICTURE_IDX_PCM_96);
        break;
    case 22:
        lcd_display_picture(COORD_PCM_176_X, COORD_PCM_176_Y, FLASH_PICTURE_IDX_PCM_176);
        break;
    case 23:
        lcd_display_picture(COORD_PCM_192_X, COORD_PCM_192_Y, FLASH_PICTURE_IDX_PCM_192);
        break;
    case 24:
        lcd_display_picture(COORD_PCM_352_X, COORD_PCM_352_Y, FLASH_PICTURE_IDX_PCM_352);
        break;
    case 25:
        lcd_display_picture(COORD_PCM_384_X, COORD_PCM_384_Y, FLASH_PICTURE_IDX_PCM_384);
        break;
    case 26:
        lcd_display_picture(COORD_PCM_705_X, COORD_PCM_705_Y, FLASH_PICTURE_IDX_PCM_705);
        break;
    case 27:
        lcd_display_picture(COORD_PCM_768_X, COORD_PCM_768_Y, FLASH_PICTURE_IDX_PCM_768);
        break;
    case 28:
        lcd_display_rectangle(COORD_BYPASS_X,COORD_BYPASS_Y,flash_picture_array[FLASH_PICTURE_IDX_BYPASS].width,
        flash_picture_array[FLASH_PICTURE_IDX_BYPASS].height,YELLOW_FOSI);
        break;
    default:
        break;
    }
}
