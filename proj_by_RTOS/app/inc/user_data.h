#ifndef __USER_DATA_H
#define __USER_DATA_H

#include "stdint.h"

#define EEPROM_ADDRESS                  0xA0
#define EEPROM_USER_DATA_ADDRESS        0X0000
#define EEPROM_PAGE_SIZE                16
#define FT24C_MSG_QU_SIZE               32
#define FT24C_SEND_BUF_SIZE             32

#define USER_DATA_SAVE_STMP             0XAA
#define USER_DATA_STMP                  0XAAAA
#define USER_DATA_DEFAULT_PLAY_IN       0
#define USER_DATA_DEFAULT_PLAY_OUT      0
#define USER_DATA_DEFAULT_VOLUME        55
#define USER_DATA_DEFAULT_PCM           0
#define USER_DATA_MAX_VOLUME            100
#define USER_DATA_DEFAULT_GAIN          0
#define USER_DATA_DEFAULT_TREBLE        1
#define USER_DATA_DEFAULT_BASS          1
#define USER_DATA_DEFAULT_BYPASS        0
#define USER_DATA_DEFAULT_UAC           0
#define USER_DATA_DEFAULT_BRIGHT        4
#define USER_DATA_DEFAULT_BRIGHT_VALUE  15 
#define USER_DATA_DEFAULT_FILT          0
#define USER_DATA_DEFAULT_TONE          0
#define USER_DATA_DEFAULT_FUN           0
#define USER_DATA_DEFAULT_AUTO          0
typedef enum
{
    INPUT_ST_COA,
    INPUT_ST_USB,
    INPUT_ST_OPT,
    INPUT_ST_RCA,
    INPUT_ST_INVAL,
}input_st_t;

typedef enum
{
    OUTPUT_ST_PH,
    OUTPUT_ST_R_X,
    OUTPUT_ST_LO,
}output_st_t;

typedef enum
{
    GAIN_ST_L,
    GAIN_ST_M,
    GAIN_ST_H,
}gain_st_t;

typedef enum
{
    TREBLE_ST_NEG_10DB, /**< -10 dB treble */
    TREBLE_ST_0DB,     /**< 0 dB treble (no adjustment) */
    TREBLE_ST_POS_10DB, /**< +10 dB treble */
} treble_st_t;

typedef enum
{
    BASS_ST_NEG_12DB, /**< -12 dB bass */
    BASS_ST_0DB,     /**< 0 dB bass (no adjustment) */
    BASS_ST_POS_12DB, /**< +12 dB bass */
} bass_st_t;

typedef enum
{
    BYPASS_ST_YES, /**< Bypass enabled */
    BYPASS_ST_NO,  /**< Bypass disabled */
} bypass_st_t;

typedef enum
{
    UAC_ST_1_0, /**< UAC version 1.0 */
    UAC_ST_2_0, /**< UAC version 2.0 */
} uac_st_t;

typedef enum
{
    LCD_BRIGHT_ST_0, /**< LCD state 0 */
    LCD_BRIGHT_ST_1, /**< LCD state 1 */
    LCD_BRIGHT_ST_2, /**< LCD state 2 */
    LCD_BRIGHT_ST_3, /**< LCD state 3 */
    LCD_BRIGHT_ST_4, /**< LCD state 4 */
} lcd_bright_st_t;

typedef enum
{
    FILT_ST_0, /**< Filter state 0 */
    FILT_ST_1, /**< Filter state 1 */
    FILT_ST_2, /**< Filter state 2 */
    FILT_ST_3, /**< Filter state 3 */
    FILT_ST_4, /**< Filter state 4 */
    FILT_ST_5, /**< Filter state 5 */
} filt_st_t;

typedef enum
{
    DEFAULT_ST_YES, /**< Default state enabled */
    DEFAULT_ST_NO,  /**< Default state disabled */
} default_st_t;

typedef enum
{
    FUN_ST_INPUT,
    FUN_ST_OUTPUT,
    FUN_ST_BASS,
    FUN_ST_TREBLE,
    FUN_ST_GAIN,
    FUN_ST_UAC,
    FUN_ST_FILT,
    FUN_ST_TONE,
    FUN_ST_LCD,
    FUN_ST_AUTO,

    FUN_ST_BYPASS_SUB,
    
    FUN_ST_DEFAULT,
    FUN_ST_OVERFLOW,
} fun_st_t;

typedef enum
{
    PCM_ST_44,
    PCM_ST_48,
    PCM_ST_88,
    PCM_ST_96,
    PCM_ST_176,
    PCM_ST_192,
    PCM_ST_352,
    PCM_ST_384,
    PCM_ST_705,
    PCM_ST_768,
    DSD_ST_64,
    DSD_ST_128,
    DSD_ST_256,
    DSD_ST_512,
}pcm_t;

typedef enum
{
    TONE_ST_NO,
    TONE_ST_YES,
}tone_st;

typedef enum
{
    AUTO_ST_NO,
    AUTO_ST_YES,
}auto_st_t;

#pragma pack(push,1)
typedef struct 
{
    uint16_t stamp;
    input_st_t input;
    output_st_t output;
    pcm_t pcm;
    uint16_t ph_volume;
    
    //uint8_t mute_state;
    gain_st_t gain;
    //treble_st_t treble;
    //bass_st_t bass;
    int8_t treble;
    int8_t bass;
    bypass_st_t bypass;
    uac_st_t uac;
    lcd_bright_st_t lcd_bright;
    uint8_t lcd_bright_value;
    filt_st_t filter;
    tone_st tone;
    //default_st_t default_st;
    fun_st_t fun_st;
    auto_st_t auto_st;

    uint16_t check_sum;
} user_data_t;
typedef struct 
{
    uint8_t stamp;
    input_st_t input;
    output_st_t output;
    pcm_t pcm;
    uint16_t ph_volume;
    gain_st_t gain;
    int8_t treble;
    int8_t bass;
    bypass_st_t bypass;
    uac_st_t uac;
    lcd_bright_st_t lcd_bright;
    filt_st_t filter;
    tone_st tone;
    uint8_t check_sum;
}user_data_save_t;

#pragma pack(pop)

extern user_data_t user_data;
extern const user_data_t user_data_default;
extern user_data_save_t user_data_save;

void ft24c_data_check(void);
void user_data_value(void);
void user_data_save_value(void);
#endif

