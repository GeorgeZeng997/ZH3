#include "config.h"

tone_blk_t tone_blk={0};
uint8_t tone_data[2]={0};
void tone_data_setting(int8_t gain_val,uint8_t bypass,uint8_t mute,uint8_t tone_channal)
{
    uint16_t send_data= 0;
    if(gain_val>=0)
    {
        send_data |= (1<<15);
    }
    else
    {
        send_data = -1*send_data;
    }
    send_data |= (gain_val<<11);

    if(bypass == TONE_BYPASS_DISABLE)
    {
        send_data |= (1<<10);
    }

    if(mute == TONE_MUTE_ENABLE)
    {
        send_data |= (1<<9);
    }

    send_data |= (tone_channal<<4);

    send_data |= (TONE_CHIP_ADDR);
    
    tone_data[0] = (uint8_t)(send_data>>8);
    tone_data[1] = (uint8_t)send_data;
}

