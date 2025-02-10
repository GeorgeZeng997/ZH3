#ifndef __IR_H
#define __IR_H

#include "config.h"

#define IR_BUF_SIZE         4
#define IR_FRAME_SIZE       8    

//IR CODE 
#define BOOT_CODE_MIN       (12442 - 1400)//1493)
#define BOOT_CODE_MAX       (12442 + 1400)//1493)
#define BIT_0_MIN           (1041 - 270)
#define BIT_0_MAX           (1041 + 270)
#define BIT_1_MIN           (2073 - 330)
#define BIT_1_MAX           (2073 + 330)


#define REPEAT_CODE_MIN		(10368 - 1444)//1000//1244)
#define REPEAT_CODE_MAX		(10368 + 1444)//1000//1244)

// #define IR_CODE_USER        0x00FE
// #define IR_CODE_USER_H      0x00
// #define IR_CODE_USER_L      0xFE
// #define IR_CODE_PWR         0x03
// #define IR_CODE_MUTE        0x0D
// #define IR_CODE_VOL_DEC     0x23
// #define IR_CODE_VOL_INC     0x21
// #define IR_CODE_AUX1        0x13
// #define IR_CODE_AUX2        0x11
// #define IR_CODE_AUX3        0x0f
// #define IR_CODE_INVAL       0xff

// #define IR_CODE_USER                0xFA01
// #define IR_CODE_USER_H              0xFA
// #define IR_CODE_USER_L              0X01
// #define IR_CODE_PWR                 0x05
// #define IR_CODE_LCD_BRIGHTNESS      0X01
// #define IR_CODE_MUTE                0x02
// #define IR_CODE_VOL_P               0x03
// #define IR_CODE_VOL_N               0x07
// #define IR_CODE_FUN_P               0x06
// #define IR_CODE_FUN_N               0X04
// #define IR_CODE_CONFIRM             0X0A
// #define IR_CODE_INPUT               0X09
// #define IR_CODE_OUTPUT              0X08
// #define IR_CODE_FILT                0X11
// #define IR_CODE_AUTO                0X13
// #define IR_CODE_BACK                0X15


#define IR_CODE_USER                0xEA82
#define IR_CODE_USER_H              0xEA
#define IR_CODE_USER_L              0X82
//#define IR_CODE_USER                0x82EA
//#define IR_CODE_USER_H              0x82
//#define IR_CODE_USER_L              0XEA
#define IR_CODE_PWR                 0x72
#define IR_CODE_LCD_BRIGHTNESS      0X73
#define IR_CODE_MUTE                0x75
#define IR_CODE_VOL_P               0x76
#define IR_CODE_VOL_N               0x79
#define IR_CODE_FUN_P               0x78
#define IR_CODE_FUN_N               0X77
#define IR_CODE_CONFIRM             0X86
#define IR_CODE_INPUT               0X80
#define IR_CODE_OUTPUT              0X81
#define IR_CODE_FILT                0X82
#define IR_CODE_AUTO                0X83
#define IR_CODE_BACK                0X85


// #define IR_CODE_VOL_DEC             0x14
// #define IR_CODE_VOL_INC             0x5E
// #define IR_CODE_AUX_CHANGE          0x23

#define IR_SAFE_TIME        0x20

typedef struct 
{
    uint8_t buf[IR_BUF_SIZE];
}ir_frame_t;


typedef struct 
{
    ir_frame_t frame[IR_FRAME_SIZE];
    uint8_t idx_in;
    uint8_t idx_out;
    uint8_t total;
}ir_qu_t;

typedef struct 
{
    uint8_t start_edge_f;
    uint8_t start_body_f;
    
    uint8_t ir_last_code;
    uint16_t ir_safe_cnt;

    
    uint16_t last_capture_cnt;
    uint16_t present_capture_cnt;
    uint8_t repeat_code_cnt;
}ir_blk_t;

typedef struct 
{
    uint8_t buf[IR_BUF_SIZE];
    uint8_t idx;
}ir_body_t;

void ir_task(void *p);
void ir_init(void);
void ir_callback(uint16_t cnt);
void ir_menu_proc(menu_msg_t msg, uint8_t menu_array_p_idx);
void ir_menu0(menu_msg_t msg);
void ir_menu1(menu_msg_t msg);
void ir_menu2(menu_msg_t msg);
void ir_menu3(menu_msg_t msg);
void ir_menu4(menu_msg_t msg);
void ir_menu5(menu_msg_t msg);

#endif
