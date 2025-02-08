#ifndef __UART_H
#define __UART_H

#include "stdint.h"

#define UART_MSG_QU_FRAME_SIZE 8
#define UART_RECV_MESSAGE_SIZE 256
#define UART_SEND_MESSAGE_SIZE 256

#define UART_TXRX_MESSAGE_SIZE 64

#define UART_TXRX_QU_FRAME_SIZE 8

#define EXCHANGEENDIANS(x)  ((((uint16_t)(x) >> 8) & 0x00FF) | (((uint16_t)(x) << 8) & 0xFF00))

typedef enum
{
    UART_MSG_INVAL,
    UART_MSG_SEND_TEST,
} uart_msg_t;





typedef struct
{
    uart_msg_t last_msg;
    uint8_t *buf;
    uint16_t buf_idx;
    uint16_t buf_length;
    uint8_t uart_busy;
} uart_blk_t;

typedef struct
{
    uint8_t *buf;
    uint16_t length;
    uint16_t idx;
} uart_txrx_buf_t;

#pragma pack(push,1)
typedef struct
{
    uint16_t hearder;
    uint8_t version;
    uint8_t command;
    uint8_t data_length;
    uint8_t message_data[UART_TXRX_MESSAGE_SIZE];
} uart_txrx_frame_t;

typedef union 
{
    uart_txrx_frame_t frame;
    uint8_t buf[sizeof(uart_txrx_frame_t)];
}uart_txrx_frame_buf_u;
#pragma pack(pop)

typedef struct 
{
    uart_txrx_frame_buf_u frame[UART_TXRX_QU_FRAME_SIZE];
    uint16_t idx_in;
    uint16_t idx_out;
    uint16_t total;
}uart_txrx_qu_t;

typedef enum
{
    UART_TX_ST_IDL,
    UART_TX_ST_SEND,
    UART_TX_ST_SEND_WAIT,
    UART_TX_ST_COMPLETE,
}uart_tx_st_t;

typedef struct
{
    xmos_cmd_idx_t msg;
    uint8_t buf[sizeof(uart_txrx_frame_t)];
    uint16_t length;
} uart_msg_frame_t;

typedef struct
{
    uart_msg_frame_t frame[UART_MSG_QU_FRAME_SIZE];
    uint16_t idx_in;
    uint16_t idx_out;
    uint16_t total;
} uart_msg_qu_t;
extern uart_txrx_frame_buf_u uart_tx_frame;
void uart_rx_qu_check_in(uart_txrx_frame_buf_u uart_frame);
void uart_tx_qu_check_in(xmos_cmd_idx_t xmos_cmd_idx, uint8_t *data,uint16_t length);
void uart_task(void *p);
uint8_t uart_package(uint8_t *data, uint16_t length, xmos_cmd_idx_t xmos_cmd_idx);

#endif
