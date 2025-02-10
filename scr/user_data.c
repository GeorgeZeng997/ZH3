#include "config.h"

uint8_t ft24c_buf_test[] = {0xaD, 0xaD, 0xaD, 0xaD};

extern uint8_t i2c1_busy;
const user_data_t user_data_default =
    {
        USER_DATA_STMP,             // stamp
        USER_DATA_DEFAULT_PLAY_IN,  // input
        USER_DATA_DEFAULT_PLAY_OUT, // output
        USER_DATA_DEFAULT_PCM,      // pcm dsd sample
        USER_DATA_DEFAULT_VOLUME,   // ph_volume

        // 0,                           // mute_state (0 = unmuted)
        USER_DATA_DEFAULT_GAIN,   // gain (L, M, H)
        USER_DATA_DEFAULT_TREBLE, // treble (±10dB)
        USER_DATA_DEFAULT_BASS,   // bass (±12dB)
        USER_DATA_DEFAULT_BYPASS,
        USER_DATA_DEFAULT_UAC,    // uac (1.0, 2.0)
        USER_DATA_DEFAULT_BRIGHT, // lcd_bright (LCD brightness level)
        USER_DATA_DEFAULT_BRIGHT_VALUE,
        USER_DATA_DEFAULT_FILT, // filter (0 to 5)
        USER_DATA_DEFAULT_TONE,
        USER_DATA_DEFAULT_FUN,
        USER_DATA_DEFAULT_AUTO,
        // 0                            // check_sum (initially set to 0)
};
const user_data_save_t user_data_save_default =
    {
        USER_DATA_SAVE_STMP,        // stamp
        USER_DATA_DEFAULT_PLAY_IN,  // input
        USER_DATA_DEFAULT_PLAY_OUT, // output
        USER_DATA_DEFAULT_PCM,      // pcm dsd sample
        USER_DATA_DEFAULT_VOLUME,   // ph_volume

        // 0,                           // mute_state (0 = unmuted)
        USER_DATA_DEFAULT_GAIN,   // gain (L, M, H)
        USER_DATA_DEFAULT_TREBLE, // treble (±10dB)
        USER_DATA_DEFAULT_BASS,   // bass (±12dB)
        USER_DATA_DEFAULT_BYPASS,
        USER_DATA_DEFAULT_UAC,    // uac (1.0, 2.0)
        USER_DATA_DEFAULT_BRIGHT, // lcd_bright (LCD brightness level)
        USER_DATA_DEFAULT_FILT,   // filter (0 to 5)
        USER_DATA_DEFAULT_TONE,
        USER_DATA_DEFAULT_FUN,
        // 0                            // check_sum (initially set to 0)
};
user_data_t user_data = {
    0};
user_data_save_t user_data_save = {0};
void user_data_value()
{
    user_data.input = user_data_save.input;
    user_data.output = user_data_save.output;
    user_data.pcm = user_data_save.pcm;

    user_data.ph_volume = user_data_save.ph_volume;
    user_data.gain = user_data_save.gain;
    user_data.treble = user_data_save.treble;
    user_data.bass = user_data_save.bass;
    user_data.bypass = user_data_save.bypass;
    user_data.uac = user_data_save.uac;
    user_data.lcd_bright = user_data_save.lcd_bright;
    user_data.filter = user_data_save.filter;
    user_data.tone = user_data_save.tone;
}
void user_data_save_value()
{
    user_data_save.input = user_data.input;
    user_data_save.output = user_data.output;
    user_data_save.pcm = user_data.pcm;

    user_data_save.ph_volume = user_data.ph_volume;
    user_data_save.gain = user_data.gain;
    user_data_save.treble = user_data.treble;
    user_data_save.bass = user_data.bass;
    user_data_save.bypass = user_data.bypass;
    user_data_save.uac = user_data.uac;
    user_data_save.lcd_bright = user_data.lcd_bright;
    user_data_save.filter = user_data.filter;
    user_data_save.tone = user_data.tone;
}
void ft24c_data_check()
{
    uint16_t check_sum_tmp = 0;
    for (uint16_t i = 0; i < sizeof(user_data_save) - sizeof(user_data_save.check_sum); i++)
    {
        check_sum_tmp += *(((uint8_t *)&user_data_save) + i);
    }
    if (user_data_save.stamp != USER_DATA_SAVE_STMP || user_data_save.check_sum != check_sum_tmp)
    {
        user_data_save = user_data_save_default;
        user_data_save.check_sum = 0;
        for (uint16_t i = 0; i < sizeof(user_data_save) - sizeof(user_data_save.check_sum); i++)
        {
            user_data_save.check_sum += *(((uint8_t *)&user_data_save) + i);
        }
        user_data_save = user_data_save;
        i2c_msg_check_in(I2C_MSG_WRITE_USER_DATA);
    }
}
