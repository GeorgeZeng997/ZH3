#ifndef __TONE_H
#define __TONE_H

#include "stdint.h"

#define TONE_CHANNAL_A_TRE  0
#define TONE_CHANNAL_A_MID  1
#define TONE_CHANNAL_A_BASS 2
#define TONE_CHANNAL_B_TRE  3
#define TONE_CHANNAL_B_MID  4
#define TONE_CHANNAL_B_BASS 5
#define TONE_BYPASS_ENABLE  0
#define TONE_BYPASS_DISABLE 1
#define TONE_MUTE_ENABLE    1
#define TONE_MUTE_DISABLE   0
#define TONE_CHIP_ADDR      0X0C

typedef struct 
{
    uint8_t tone_data;
    uint8_t tone_select_addr;
    uint8_t tone_chip_addr;
}tone_blk_t;

extern tone_blk_t tone_blk;
extern uint8_t tone_data[2];

extern void tone_data_setting(int8_t gain_val,uint8_t bypass,uint8_t mute,uint8_t tone_channal);
#endif
