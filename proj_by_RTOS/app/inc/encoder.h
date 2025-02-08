#ifndef __ENCODER_H
#define __ENCODER_H

#include "stdint.h"

typedef enum
{
    ENCODER_ST_IDL,
    ENCODER_ST_START,
    ENCODER_ST_FWD,
    ENCODER_ST_REV,
    ENCODER_ST_RLS,

    ENCODER_ST_FWD1,
    ENCODER_ST_FWD2,
    ENCODER_ST_FWD3,

    ENCODER_ST_REV1,
    ENCODER_ST_REV2,
    ENCODER_ST_REV3,

    ENCODER_ST_RLS_FWD,
    ENCODER_ST_RLS_REV,
} encoder_st_t;
#define KEY_CLK_TIME_MIN    5

#define KEY_HLD_TIME_MIN    250

typedef enum
{
    KEY_ST_IDL,
    KEY_ST_PRESS,
    KEY_ST_CLK,
    KEY_ST_SHORT_HOLD,
    KEY_ST_LONG_HOLD,
    KEY_ST_DOUBLE_CLK,
    KEY_ST_TRIPLE_CLK,
    KEY_ST_RELEASE,
}key_st_t;
typedef struct
{
    uint8_t encoder_pin_a_cnt ;
    uint8_t encoder_pin_b_cnt ;
    uint8_t encoder_pin_a ;
    uint8_t encoder_pin_b ;
} encoder_blk_t;


void encoder_task(void *p);
void encoder_init(void);
void encoder_keyscan_task(void *p);

#endif
