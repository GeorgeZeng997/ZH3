#include "config.h"
//channalA :0
//channalB :1
uint32_t treble_data_trasform(uint8_t treble_data,uint8_t channal)
{
    uint32_t send_data= 0;
    send_data = (1<<15)|(treble_data<<11)|(1<<10)|
}

