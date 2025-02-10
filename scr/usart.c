#include "config.h"

uart_txrx_frame_buf_u uart_tx_frame = {0};
uart_txrx_frame_buf_u uart_rx_frame = {0};

uart_msg_qu_t uart_msg_qu = {0};

uart_txrx_buf_t uart_tx_buf = {0};

uart_txrx_qu_t uart_rx_qu = {0};
uart_txrx_qu_t uart_tx_qu = {0};

uart_blk_t uart_blk = {0};
uart_tx_st_t uart_tx_st = UART_TX_ST_IDL;

uint8_t uart_update_f = 0;

uint16_t crc16_ccitt(uint8_t *data, uint16_t length)
{
    uint16_t crc = 0x0000;
    uint16_t poly = 0x1021;

    for (uint16_t i = 0; i < length; i++)
    {
        crc ^= (data[i] << 8);
        for (uint16_t j = 0; j < 8; j++)
        {
            if (crc & 0x8000)
            {
                crc = (crc << 1) ^ poly;
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}

uint8_t xmos_checksum(uint8_t *data, uint16_t length)
{
    if (data == NULL)
        return 0;
    uint8_t checksum = 0;
    for (uint16_t i = 0; i < length; i++)
    {
        checksum += data[i];
    }
    return checksum;
}

uint8_t uart_package(uint8_t *data, uint16_t length, xmos_cmd_idx_t xmos_cmd_idx)
{
    uart_txrx_frame_buf_u *frame = &uart_tx_frame;
    uint8_t extra_length = 0;
    uint8_t checksum = 0;
    if (data == NULL && length != 0)
    {
        return 0;
    }
    if (data != NULL && length != 0)
    {
        frame->frame.data_length = length;
        for (uint16_t i = 0; i < length; i++)
        {
            frame->frame.message_data[i] = data[i];
        }
    }
    else
    {
        frame->frame.data_length = strlen(xmos_cmd_array[xmos_cmd_idx].content);
        for (uint16_t i = 0; i < frame->frame.data_length; i++)
        {
            frame->frame.message_data[i] = xmos_cmd_array[xmos_cmd_idx].content[i];
        }
    }
    extra_length = frame->frame.data_length;
    frame->frame.hearder = EXCHANGEENDIANS(XMOS_HEADER);
    frame->frame.version = XMOS_VERSION;
    frame->frame.command = xmos_cmd_array[xmos_cmd_idx].cmd;
    if (xmos_cmd_idx == XMOS_CMD_IDX_UPDATE_PACK_SUCCESS || xmos_cmd_idx == XMOS_CMD_IDX_UPDATE_PACK_FAILURE
        || xmos_cmd_idx == XMOS_CMD_IDX_UPDATE_PACK_RESTART)
    {
        frame->frame.hearder = XMOS_HEADER;
        uint16_t crc_val = crc16_ccitt(frame->buf, 6);
        frame->buf[6] = crc_val >> 8;
        frame->buf[7] = crc_val;
        extra_length = 2;
    }
    else
    {
        checksum = xmos_checksum(frame->buf, XMOS_DEFAULT_LENGTH + extra_length - 1);
        frame->frame.message_data[frame->frame.data_length] = checksum;
    }

    return XMOS_DEFAULT_LENGTH + extra_length;
}

void uart_tx_qu_check_in(xmos_cmd_idx_t xmos_cmd_idx, uint8_t *data, uint16_t length)
{
    if (uart_msg_qu.total > UART_MSG_QU_FRAME_SIZE)
    {
        return;
    }
    uart_msg_qu.frame[uart_msg_qu.idx_in].msg = xmos_cmd_idx;
    memcpy(uart_msg_qu.frame[uart_msg_qu.idx_in].buf, data, length);
    uart_msg_qu.frame[uart_msg_qu.idx_in].length = length;
    if (++uart_msg_qu.idx_in >= UART_MSG_QU_FRAME_SIZE)
    {
        uart_msg_qu.idx_in = 0;
    }
    uart_msg_qu.total++;
}
void uart_tx_qu_check_out()
{
    if (++uart_msg_qu.idx_out >= UART_MSG_QU_FRAME_SIZE)
    {
        uart_msg_qu.idx_out = 0;
    }
    uart_msg_qu.total--;
}

void uart_tx_msg_proc()
{
    if (uart_msg_qu.frame[uart_msg_qu.idx_out].msg == XMOS_CMD_IDX_UPDATE_PACK_SUCCESS || uart_msg_qu.frame[uart_msg_qu.idx_out].msg == XMOS_CMD_IDX_UPDATE_PACK_FAILURE)
    {
        uart_tx_buf.idx = 0;
        uart_tx_buf.length = 0;

        uart_blk.buf = uart_msg_qu.frame[uart_msg_qu.idx_out].buf;
        uart_blk.buf_length = uart_msg_qu.frame[uart_msg_qu.idx_out].length;
        uart_blk.buf_idx = 0;
        uart_blk.uart_busy = 0;
        uart_tx_st = UART_TX_ST_SEND;
    }
    else
    {
        switch (uart_msg_qu.frame[uart_msg_qu.idx_out].msg)
        {
        case XMOS_CMD_IDX_BOOT:
            /* code */
            break;
        default:
            break;
        }
        uart_tx_buf.idx = 0;
        uart_tx_buf.length = 0;

        uart_blk.buf = uart_msg_qu.frame[uart_msg_qu.idx_out].buf;
        uart_blk.buf_length = uart_msg_qu.frame[uart_msg_qu.idx_out].length;
        uart_blk.buf_idx = 0;
        uart_blk.uart_busy = 0;
        uart_tx_st = UART_TX_ST_SEND;
    }

    uart_tx_qu_check_out();
}

void uart_tx_proc()
{
    switch (uart_tx_st)
    {
    case UART_TX_ST_IDL:
        if (uart_msg_qu.total)
        {
            uart_tx_msg_proc();
        }
        break;
    case UART_TX_ST_SEND:
        uart_tx_buf.buf = uart_blk.buf;
        uart_tx_buf.length = uart_blk.buf_length;
        uart_tx_buf.idx = 0;
        uart_tx_st = UART_TX_ST_SEND_WAIT;
        // uart_blk.uart_busy = 1;
        break;
    case UART_TX_ST_SEND_WAIT:
        if (!uart_blk.uart_busy)
        {
            uart_tx_st = UART_TX_ST_COMPLETE;
        }
        break;
    case UART_TX_ST_COMPLETE:
        uart_tx_st = UART_TX_ST_IDL;
        if (uart_update_f == 1)
        {
            NVIC_SystemReset();
        }
        break;
    default:
        break;
    }
    if (!uart_blk.uart_busy && uart_tx_buf.length && !uart_tx_buf.idx)
    {
        uart_blk.uart_busy = 1;
        usart_interrupt_enable(USART0, USART_INT_TBE);
    }
}

void uart_rx_qu_check_in(uart_txrx_frame_buf_u uart_frame)
{
    if (uart_rx_qu.total >= UART_TXRX_QU_FRAME_SIZE)
    {
        return;
    }
    uart_rx_qu.frame[uart_rx_qu.idx_in].frame = uart_frame.frame;
    if (++uart_rx_qu.idx_in >= UART_TXRX_QU_FRAME_SIZE)
    {
        uart_rx_qu.idx_in = 0;
    }
    uart_rx_qu.total++;
}

void uart_rx_qu_check_out()
{
    if (++uart_rx_qu.idx_out >= UART_TXRX_QU_FRAME_SIZE)
    {
        uart_rx_qu.idx_out = 0;
    }
    uart_rx_qu.total--;
}
uint16_t recv_cnt_test = 0;
void uart_rx_proc()
{
    if (uart_rx_qu.total)
    {
        if (uart_rx_qu.frame[uart_rx_qu.idx_out].frame.version == 0x01)
        {
            uint16_t pack_idx = ((uint16_t)uart_rx_qu.frame[uart_rx_qu.idx_out].buf[5] << 8) | uart_rx_qu.frame[uart_rx_qu.idx_out].buf[6];
            uint16_t pack_length = uart_rx_qu.frame[uart_rx_qu.idx_out].buf[3];
            uint16_t recv_crc = (uart_rx_qu.frame[uart_rx_qu.idx_out].buf[38] << 8) + uart_rx_qu.frame[uart_rx_qu.idx_out].buf[39];
            uint16_t calcu_crc = crc16_ccitt(uart_rx_qu.frame[uart_rx_qu.idx_out].buf, 38);

            if (recv_crc == calcu_crc)
            {
                recv_cnt_test++;
                if (pack_idx == 0)
                {
                    fmc_init();
                    fmc_erase_backup_pages();
                    fmc_blk.pack_addr = FMC_BACKUP_START_ADDR;
                    fmc_blk.file_length = ((uint32_t)uart_rx_qu.frame[uart_rx_qu.idx_out].buf[7] << 16) | ((uint32_t)uart_rx_qu.frame[uart_rx_qu.idx_out].buf[8] << 8) | ((uint32_t)uart_rx_qu.frame[uart_rx_qu.idx_out].buf[9]);
                    fmc_blk.file_crc = ((uint16_t)uart_rx_qu.frame[uart_rx_qu.idx_out].buf[10] << 8) | ((uint16_t)uart_rx_qu.frame[uart_rx_qu.idx_out].buf[11]);
                    uart_tx_qu_check_in(XMOS_CMD_IDX_UPDATE_PACK_SUCCESS, uart_tx_frame.buf, uart_package(NULL, 0, XMOS_CMD_IDX_UPDATE_PACK_SUCCESS));
                    
                }
                else if (pack_idx == fmc_blk.pack_idx + 1)
                {
                    fmc_program_pack(&uart_rx_qu.frame[uart_rx_qu.idx_out].buf[8], pack_length, fmc_blk.pack_addr);
                    fmc_blk.pack_addr += pack_length;
                    fmc_blk.file_recv_length += pack_length;
                    fmc_blk.pack_idx = pack_idx;
                    uart_tx_qu_check_in(XMOS_CMD_IDX_UPDATE_PACK_SUCCESS, uart_tx_frame.buf, uart_package(NULL, 0, XMOS_CMD_IDX_UPDATE_PACK_SUCCESS));
                    if (fmc_blk.file_recv_length == fmc_blk.file_length)
                    {
                        uart_update_f = 1;
                        fmc_erase_update_flag(USER_UPDATE_FLAG_ADDR);
                        fmc_program_update_success();
                    }
                }
                else if (pack_idx == fmc_blk.pack_idx )
                {
                    uart_tx_qu_check_in(XMOS_CMD_IDX_UPDATE_PACK_SUCCESS, uart_tx_frame.buf, uart_package(NULL, 0, XMOS_CMD_IDX_UPDATE_PACK_SUCCESS));
                    
                }
                else
                {
                    uart_tx_qu_check_in(XMOS_CMD_IDX_UPDATE_PACK_RESTART, uart_tx_frame.buf, uart_package(NULL, 0, XMOS_CMD_IDX_UPDATE_PACK_RESTART));
                }
            }
            else
            {
                uart_tx_qu_check_in(XMOS_CMD_IDX_UPDATE_PACK_FAILURE, uart_tx_frame.buf, uart_package(NULL, 0, XMOS_CMD_IDX_UPDATE_PACK_FAILURE));
            }
        }
        else if (uart_rx_qu.frame[uart_rx_qu.idx_out].frame.version == 0x00)
        {
            switch (uart_rx_qu.frame[uart_rx_qu.idx_out].frame.command)
            {
            case XMOS_CMD_READ_AUDIO_MODE:
                /* code */
                break;

            default:
                break;
            }
        }

        uart_rx_qu_check_out();
    }
}

void usart0_rx_callback(uint8_t data)
{
    static uint8_t rx_idx = 0, rx_last_idx = 0;

    rx_last_idx = rx_idx;
    if (rx_idx < 5)
    {
        switch (rx_idx)
        {
        case 0:
            if (data == 0xAA)
            {
                rx_idx++;
            }
            break;
        case 1:
            if (data == 0x55)
            {
                rx_idx++;
                uart_rx_frame.frame.hearder = 0x55AA;
            }
            break;
        case 2:
            if (data == 0x00)
            {
                rx_idx++;
                uart_rx_frame.frame.version = 0x00;
            }
            else if (data == 0x01)
            {
                rx_idx++;
                uart_rx_frame.frame.version = 0x01;
                // uart_update_f = 1;
            }
            break;
        case 3:
            uart_rx_frame.frame.command = data;
            rx_idx++;
            break;

        case 4:
            uart_rx_frame.frame.data_length = data;
            // length overflow
            if (data <= UART_TXRX_MESSAGE_SIZE)
                rx_idx++;
            else
                rx_idx = rx_last_idx = 0;
            break;
        default:
            break;
        }
    }
    else
    {
        if (uart_rx_frame.frame.data_length + 5 - rx_idx > 0)
        {
            uart_rx_frame.frame.message_data[rx_idx - 5] = data;
            rx_idx++;
        }
        else
        {

            uart_rx_qu_check_in(uart_rx_frame);
            rx_idx = rx_last_idx = 0;
        }
    }
    // receive wrong data
    if (rx_idx == rx_last_idx)
    {
        rx_idx = rx_last_idx = 0;
    }
}

void uart_task(void *p)
{
    while (1)
    {
        uart_tx_proc();
        uart_rx_proc();
        vTaskDelay(5);
    }
}
