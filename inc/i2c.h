#ifndef __I2C_H
#define __I2C_H

#include "config.h"

#define I2C_FRAME_BUF_SIZE  32
#define I2C_MSG_QU_SIZE     16

#define I2C_4493_ADDR1      0X80
#define I2C_4493_ADDR2      0X82

#define I2C_4493_REG_SELECT_ADDR_ACH    0X00
#define I2C_4493_REG_SELECT_ADDR_BCH    0X01
#define I2C_4493_REG_SETTING_ADDR       0X02

#define I2C_VOL_MUTE       0X00

#define I2C_VOL_BASE       24 //REQUIRE TO LEFT SHIFT 2 BIT TO SEND

#define I2C_VOL_MIN        23
#define I2C_VOL_MAX        55          
#define I2C_4493_VOL_MAX   0x60
typedef enum
{
    I2C_MSG_WRITE_USER_DATA,
    I2C_MSG_READ_USER_DATA,
    I2C_MSG_WRITE_4493_DATA,
    I2C_MSG_ES9039_INIT,
}i2c_msg_t;

typedef struct 
{
    i2c_msg_t msg[I2C_MSG_QU_SIZE];
    uint8_t idx_in;
    uint8_t idx_out;
    uint8_t total;
}i2c_msg_qu_t;


typedef struct 
{
    uint8_t device_addr;
    uint16_t register_addr;
    uint8_t registers;
    uint8_t buf[I2C_FRAME_BUF_SIZE];
    uint8_t buf_length;
    uint8_t buf_idx;
}i2c_frame_t;

typedef enum
{
    I2C_PROC_ST_IDL,

    I2C_PROC_ST_WRITE_USER_DATA,
    I2C_PROC_ST_WRITE_USER_DATA_WAITING,
    I2C_PROC_ST_WRITE_USER_DATA_COMPLETE,

    I2C_PROC_ST_WRITE_4493_RIGHT_CHA_DATA,
    I2C_PROC_ST_WRITE_4493_RIGHT_CHA_DATA_WAITING,
    I2C_PROC_ST_WRITE_4493_RIGHT_CHB_DATA,
    I2C_PROC_ST_WRITE_4493_RIGHT_CHB_DATA_WAITING,
    I2C_PROC_ST_WRITE_4493_LEFT_CHA_DATA,
    I2C_PROC_ST_WRITE_4493_LEFT_CHA_DATA_WAITING,
    I2C_PROC_ST_WRITE_4493_LEFT_CHB_DATA,
    I2C_PROC_ST_WRITE_4493_LEFT_CHB_DATA_WAITING,
    I2C_PROC_ST_WRITE_4493_RIGHT_DATA_COMPLETE,

    I2C_PROC_ST_READ_USER_DATA,
    I2C_PROC_ST_READ_USER_DATA_WAITING,
    I2C_PROC_ST_READ_USER_DATA_COMPLETE,

    I2C_PROC_ST_ES9039_INIT,
    I2C_PROC_ST_ES9039_INIT_WAITING,
    I2C_PROC_ST_ES9039_COMPLETE,
}i2c_proc_st_t;

typedef enum
{
    I2C_ST_IDL,//judge send or recv state here by addr lsb

    I2C_ST_SEND_WRITE_ADDR,
    I2C_ST_SEND_CLEAR_ADDR_FLAG,
    I2C_ST_SEND_REGISTER_ADDR,
    I2C_ST_SEND_REGISTER_ADDR_WAITING,
    I2C_ST_SEND_WRITE_DATA,
    I2C_ST_SEND_STOP,


    I2C_ST_RECV_WRITE_ADDR,
    I2C_ST_RECV_CLEAR_ADDR_FLAG,
    I2C_ST_RECV_REGISTER_ADDR,
    I2C_ST_RECV_REGISTER_ADDR_WAITING,
    I2C_ST_RECV_WRITE_READ_ADDR,
    I2C_ST_RECV_CLEAR_READ_ADDR_FLAG,
    I2C_ST_RECV_TRANSMIT_DATA,
}i2c_st_t;

typedef struct 
{
    uint16_t i2c_write_page_idx;
    uint16_t i2c_write_page_length;
}i2c_blk_t;




void i2c_task(void *p);

void i2c_msg_check_in(i2c_msg_t msg);
void i2c_msg_check_out(void);
void i2c_event_callback(void);
void i2c_error_callback(void);
#endif
