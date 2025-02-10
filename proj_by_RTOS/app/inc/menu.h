#ifndef __MENU_H
#define __MENU_H

#include "stdint.h"

#define     MENU_SIZE    5
#define     MENU_MSG_SIZE   16

typedef enum
{
    MENU_IDX_NAME0,
    MENU_IDX_NAME1,
    MENU_IDX_NAME2,
    MENU_IDX_NAME3,
}menu_idx_t;

typedef enum
{
    OPTION_IDX_NAME1,
    OPTION_IDX_NAME2,
    OPTION_IDX_NAME3,
    OPTION_IDX_NAME4,
}option_idx_t;


typedef struct 
{
    menu_idx_t menu_idx;
    option_idx_t option_idx;
}menu_layer_t;


typedef enum
{
    MENU_MSG_INVAL,
    MENU_MSG_CLICK,
    MENU_MSG_DOUBLE_CLICK,
    MENU_MSG_TRIPLE_CLICK,
    MENU_MSG_SHORT_HOLD,
    MENU_MSG_LONG_HOLD,
    MENU_MSG_REV,
    MENU_MSG_FWD,
    MENU_MSG_PRESS_REV,
    MENU_MSG_PRESS_FWD,

    MENU_MSG_IR_PWR,
    MENU_MSG_IR_LCD_BRIGHTNESS,
    MENU_MSG_IR_MUTE,
    MENU_MSG_IR_VOL_P,
    MENU_MSG_IR_VOL_N,
    MENU_MSG_IR_FUN_P,
    MENU_MSG_IR_FUN_N,
    MENU_MSG_IR_CONFIRM,
    MENU_MSG_IR_INPUT,
    MENU_MSG_IR_OUTPUT,
    MENU_MSG_IR_FILT,
    MENU_MSG_IR_AUTO,
    MENU_MSG_IR_BACK,
}menu_msg_t;

typedef struct 
{
    menu_msg_t menu_msg[MENU_MSG_SIZE];
    uint8_t idx_in;
    uint8_t idx_out;
    uint8_t total;
}menu_msg_qu_t;

typedef struct 
{
    menu_layer_t menu_layer;
    menu_msg_t menu_msg;
    uint8_t menu_array_p_idx; 
}menu_blk_t;

typedef struct 
{
    uint8_t mute;
    uint8_t def;
    uint8_t mode_f;//0 = unselected,1 = selected
    uint8_t menu2_fun;
    uint8_t power_on;
}menu_misc_t;

extern menu_misc_t menu_misc;
extern menu_blk_t menu_blk;
extern menu_msg_qu_t menu_msg_qu;

void menu_msg_check_in(menu_msg_t msg);
void menu_task(void *p);
void menu_init(void);

void menu_name0_proc(menu_msg_t msg);
void menu_name1_proc(menu_msg_t msg);
void menu_name2_proc(menu_msg_t msg);
void menu_name3_proc(menu_msg_t msg);
void menu_name4_proc(menu_msg_t msg);
void menu_name5_proc(menu_msg_t msg);

void menu_name3_fwd(void);
void menu_name3_rev(void);
void menu_name2_fwd(void);
void menu_name2_rev(void);
#endif

