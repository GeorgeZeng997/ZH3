#include "config.h"

i2c_msg_qu_t i2c_msg_qu = {0};
i2c_st_t i2c_st = I2C_ST_IDL;
i2c_proc_st_t i2c_proc_st = I2C_PROC_ST_IDL;
i2c_frame_t i2c_frame = {0};

uint8_t i2c_proc_safe_cnt = 0;
uint8_t i2c_busy = 0;

i2c_blk_t i2c_blk = {0};
uint8_t vol_test[1] = {I2C_4493_VOL_MAX};

void i2c_msg_check_in(i2c_msg_t msg)
{
    if (i2c_msg_qu.total > I2C_MSG_QU_SIZE)
    {
        return;
    }
    i2c_msg_qu.msg[i2c_msg_qu.idx_in] = msg;
    if (++i2c_msg_qu.idx_in >= I2C_MSG_QU_SIZE)
    {
        i2c_msg_qu.idx_in = 0;
    }
    i2c_msg_qu.total++;
}

void i2c_set_frame(uint8_t dev_addr, uint16_t reg_addr, uint8_t regs, uint8_t *buf, uint8_t buf_length)
{
    i2c_frame.device_addr = dev_addr;
    i2c_frame.register_addr = reg_addr;
    i2c_frame.registers = regs;
    i2c_frame.buf_length = buf_length;
    for (uint8_t i = 0; i < buf_length; i++)
    {
        i2c_frame.buf[i] = buf[i];
    }
    // i2c_frame.buf_idx = 0;
}

void i2c_msg_check_out()
{
    if (++i2c_msg_qu.idx_out >= I2C_MSG_QU_SIZE)
    {
        i2c_msg_qu.idx_out = 0;
    }
    i2c_msg_qu.total--;
}
uint8_t vol_process(uint8_t vol_data)
{
    uint8_t data_process;
    uint8_t i2c_vol_data = I2C_VOL_MAX + I2C_VOL_BASE;
    if (vol_data > I2C_VOL_MAX)
    {
        vol_data = I2C_VOL_MAX;
    }
    if (vol_data < I2C_VOL_MIN)
    {
        vol_data = I2C_VOL_MIN;
    }
    if (vol_data == I2C_VOL_MIN)
    {
        i2c_vol_data = 0;
    }
    else if (vol_data == I2C_VOL_MAX)
    {
        i2c_vol_data = I2C_VOL_BASE;
    }
    else
    {
        i2c_vol_data -= vol_data;
    }
    i2c_vol_data = i2c_vol_data<<2;
    return i2c_vol_data;
}
void i2c_msg_proc()
{
    switch (i2c_msg_qu.msg[i2c_msg_qu.idx_out])
    {
    case I2C_MSG_READ_USER_DATA:
        i2c_blk.i2c_write_page_idx = 0;
        i2c_blk.i2c_write_page_length = (sizeof(user_data_save) + EEPROM_PAGE_SIZE - 1) / EEPROM_PAGE_SIZE;
        i2c_proc_st = I2C_PROC_ST_READ_USER_DATA;
        break;
    case I2C_MSG_WRITE_USER_DATA:
        i2c_blk.i2c_write_page_idx = 0;
        i2c_blk.i2c_write_page_length = (sizeof(user_data_save) + EEPROM_PAGE_SIZE - 1) / EEPROM_PAGE_SIZE;
        user_data_save_value();
        i2c_proc_st = I2C_PROC_ST_WRITE_USER_DATA;
        break;
    case I2C_MSG_ES9039_INIT:
        i2c_proc_st = I2C_PROC_ST_ES9039_INIT;
        break;
    case I2C_MSG_WRITE_4493_DATA:
        i2c_proc_st = I2C_PROC_ST_WRITE_4493_RIGHT_CHA_DATA;
        vol_test[0] = vol_process(user_data_save.ph_volume);
            break;
        // i2c_proc_st = I2C_PROC_ST_WRITE_4493_LEFT_CHA_DATA;
    default:
        break;
    }
    i2c_msg_check_out();
}

uint8_t i2c_test_data[32] = {0};
void i2c_task(void *p)
{
     i2c_msg_check_in(I2C_MSG_READ_USER_DATA);
    memset(i2c_test_data, 0xbb, sizeof(i2c_test_data));
    while (1)
    {
        switch (i2c_proc_st)
        {
        case I2C_PROC_ST_IDL:
            if (i2c_msg_qu.total)
            {
                i2c_msg_proc();
                i2c_deinit(I2C1);
                i2c_config();
                i2c_st = I2C_ST_IDL;
                i2c_proc_safe_cnt = 100;
            }
            break;
        // WRITE USER DATA
        case I2C_PROC_ST_WRITE_USER_DATA:
        {
            uint16_t check_sum_tmp = 0;
            uint16_t user_data_start = i2c_blk.i2c_write_page_idx * EEPROM_PAGE_SIZE;
            uint16_t user_data_end = user_data_start + EEPROM_PAGE_SIZE;
            if (sizeof(user_data_save) - user_data_start < EEPROM_PAGE_SIZE)
            {
                user_data_end = sizeof(user_data_save);
            }
            for (uint16_t i = 0; i < sizeof(user_data_save) - sizeof(user_data_save.check_sum); i++)
            {
                check_sum_tmp += *(((uint8_t *)&user_data_save) + i);
            }
            user_data_save.check_sum = check_sum_tmp;
            //i2c_set_frame(EEPROM_ADDRESS, EEPROM_USER_DATA_ADDRESS + user_data_start, 2, ((uint8_t *)&user_data_save) + user_data_start, user_data_end - user_data_start);

            i2c_set_frame(EEPROM_ADDRESS, EEPROM_USER_DATA_ADDRESS, 2, ((uint8_t *)&user_data_save), sizeof(user_data_save));

            i2c_interrupt_enable(I2C1, I2C_INT_EV);
            i2c_interrupt_enable(I2C1, I2C_INT_BUF);
            i2c_interrupt_enable(I2C1, I2C_INT_ERR);
            i2c_proc_st = I2C_PROC_ST_WRITE_USER_DATA_WAITING;
        }
        break;
        case I2C_PROC_ST_WRITE_USER_DATA_WAITING:
            if (i2c_busy || i2c_flag_get(I2C1, I2C_FLAG_I2CBSY))
            {
                break;
            }
            //i2c_proc_st = I2C_PROC_ST_WRITE_USER_DATA_COMPLETE;
            if (++i2c_blk.i2c_write_page_idx >= i2c_blk.i2c_write_page_length)
            {
                i2c_proc_st = I2C_PROC_ST_WRITE_USER_DATA_COMPLETE;
            }
            else
            {
                i2c_deinit(I2C1);
                i2c_config();
                i2c_st = I2C_ST_IDL;
                i2c_proc_st = I2C_PROC_ST_WRITE_USER_DATA;
            }
            break;
        case I2C_PROC_ST_WRITE_USER_DATA_COMPLETE:
            i2c_blk.i2c_write_page_idx = 0;
            i2c_proc_safe_cnt = 0;
            i2c_proc_st = I2C_PROC_ST_IDL;
            break;
            // I2C READ
        case I2C_PROC_ST_READ_USER_DATA:
        {
            uint16_t user_data_start = i2c_blk.i2c_write_page_idx * EEPROM_PAGE_SIZE;
            uint16_t user_data_end = user_data_start + EEPROM_PAGE_SIZE;
            if (sizeof(user_data_save) - user_data_start < EEPROM_PAGE_SIZE)
            {
                user_data_end = sizeof(user_data_save) ;
            }
            i2c_set_frame(EEPROM_ADDRESS | 0x01, EEPROM_USER_DATA_ADDRESS+user_data_start, 2, (uint8_t *)(&user_data_save) + user_data_start, user_data_end-user_data_start);
            //i2c_set_frame(EEPROM_ADDRESS | 0x01, EEPROM_USER_DATA_ADDRESS, 2, (uint8_t *)(&user_data_save), 16);
            i2c_interrupt_enable(I2C1, I2C_INT_EV);
            i2c_interrupt_enable(I2C1, I2C_INT_BUF);
            i2c_interrupt_enable(I2C1, I2C_INT_ERR);
            i2c_proc_st = I2C_PROC_ST_READ_USER_DATA_WAITING;
        }
        break;
        case I2C_PROC_ST_READ_USER_DATA_WAITING:
            if (i2c_busy || i2c_flag_get(I2C1, I2C_FLAG_I2CBSY))
            {
                break;
            }
            if (++i2c_blk.i2c_write_page_idx >= i2c_blk.i2c_write_page_length)
            {
                memcpy(((uint8_t *)&user_data_save)+(i2c_blk.i2c_write_page_idx-1) * EEPROM_PAGE_SIZE,i2c_frame.buf,sizeof(user_data_save)-EEPROM_PAGE_SIZE*(i2c_blk.i2c_write_page_idx-1));
               i2c_proc_st = I2C_PROC_ST_READ_USER_DATA_COMPLETE;
            }

            else
            {
                for(uint16_t i=0;i<EEPROM_PAGE_SIZE;i++)
                {
                    *(((uint8_t *)&user_data_save)+(i2c_blk.i2c_write_page_idx-1) * EEPROM_PAGE_SIZE+i) = i2c_frame.buf[i];
                }
                //memcmp(((uint8_t *)&user_data_save)+(i2c_blk.i2c_write_page_idx-1) * EEPROM_PAGE_SIZE,i2c_frame.buf,EEPROM_PAGE_SIZE);
                i2c_deinit(I2C1);
                i2c_config();
                i2c_st = I2C_ST_IDL;
                i2c_proc_st = I2C_PROC_ST_READ_USER_DATA;
            }
            // for (uint16_t i = 0; i < EEPROM_PAGE_SIZE; i++)
            // {
            //     *(((uint8_t *)&user_data_save) + i) = i2c_frame.buf[i];
            // }
            // i2c_proc_st = I2C_PROC_ST_READ_USER_DATA_COMPLETE;
            break;
        case I2C_PROC_ST_READ_USER_DATA_COMPLETE:
            ft24c_data_check();
            i2c_proc_safe_cnt = 0;
            i2c_proc_st = I2C_PROC_ST_IDL;
            break;
        // WRITE 4493 DATA
        case I2C_PROC_ST_WRITE_4493_RIGHT_CHA_DATA:
            i2c_set_frame(I2C_4493_ADDR1, 0X00, 1, vol_test, 1);
            i2c_interrupt_enable(I2C1, I2C_INT_EV);
            i2c_interrupt_enable(I2C1, I2C_INT_BUF);
            i2c_interrupt_enable(I2C1, I2C_INT_ERR);
            i2c_proc_st = I2C_PROC_ST_WRITE_4493_RIGHT_CHA_DATA_WAITING;
            break;
        case I2C_PROC_ST_WRITE_4493_RIGHT_CHA_DATA_WAITING:
            if (i2c_busy || i2c_flag_get(I2C1, I2C_FLAG_I2CBSY))
            {
                break;
            }
            i2c_proc_st = I2C_PROC_ST_WRITE_4493_RIGHT_CHB_DATA;
            break;
        case I2C_PROC_ST_WRITE_4493_RIGHT_CHB_DATA:
            i2c_deinit(I2C1);
            i2c_config();
            i2c_st = I2C_ST_IDL;
            i2c_set_frame(I2C_4493_ADDR1, 0X01, 1, vol_test, 1);
            i2c_interrupt_enable(I2C1, I2C_INT_EV);
            i2c_interrupt_enable(I2C1, I2C_INT_BUF);
            i2c_interrupt_enable(I2C1, I2C_INT_ERR);
            i2c_proc_st = I2C_PROC_ST_WRITE_4493_RIGHT_CHB_DATA_WAITING;
            break;
        case I2C_PROC_ST_WRITE_4493_RIGHT_CHB_DATA_WAITING:
            if (i2c_busy || i2c_flag_get(I2C1, I2C_FLAG_I2CBSY))
            {
                break;
            }
            i2c_proc_st = I2C_PROC_ST_WRITE_4493_LEFT_CHA_DATA;
            break;
        case I2C_PROC_ST_WRITE_4493_LEFT_CHA_DATA:
            i2c_deinit(I2C1);
            i2c_config();
            i2c_st = I2C_ST_IDL;
            i2c_set_frame(I2C_4493_ADDR2, 0X00, 1, vol_test, 1);
            i2c_interrupt_enable(I2C1, I2C_INT_EV);
            i2c_interrupt_enable(I2C1, I2C_INT_BUF);
            i2c_interrupt_enable(I2C1, I2C_INT_ERR);
            i2c_proc_st = I2C_PROC_ST_WRITE_4493_LEFT_CHA_DATA_WAITING;
            break;
        case I2C_PROC_ST_WRITE_4493_LEFT_CHA_DATA_WAITING:
            if (i2c_busy || i2c_flag_get(I2C1, I2C_FLAG_I2CBSY))
            {
                break;
            }
            i2c_proc_st = I2C_PROC_ST_WRITE_4493_LEFT_CHB_DATA;
            break;
        case I2C_PROC_ST_WRITE_4493_LEFT_CHB_DATA:
            i2c_deinit(I2C1);
            i2c_config();
            i2c_st = I2C_ST_IDL;
            i2c_set_frame(I2C_4493_ADDR2, 0X01, 1, vol_test, 1);
            i2c_interrupt_enable(I2C1, I2C_INT_EV);
            i2c_interrupt_enable(I2C1, I2C_INT_BUF);
            i2c_interrupt_enable(I2C1, I2C_INT_ERR);
            i2c_proc_st = I2C_PROC_ST_WRITE_4493_LEFT_CHB_DATA_WAITING;
            break;

        case I2C_PROC_ST_WRITE_4493_LEFT_CHB_DATA_WAITING:
            if (i2c_busy || i2c_flag_get(I2C1, I2C_FLAG_I2CBSY))
            {
                break;
            }
            i2c_proc_st = I2C_PROC_ST_WRITE_4493_RIGHT_DATA_COMPLETE;
            break;
        case I2C_PROC_ST_WRITE_4493_RIGHT_DATA_COMPLETE:
            i2c_proc_safe_cnt = 0;
            i2c_proc_st = I2C_PROC_ST_IDL;
            break;

        // ES9039 INIT
        case I2C_PROC_ST_ES9039_INIT:
        {
            uint8_t buf[1] = {0x02};
#ifdef ES9039
            i2c_set_frame(ES9039_DEVICE_ADDR, 0, 1, buf, 1);
#endif
            i2c_interrupt_enable(I2C1, I2C_INT_EV);
            i2c_interrupt_enable(I2C1, I2C_INT_BUF);
            i2c_interrupt_enable(I2C1, I2C_INT_ERR);
            i2c_proc_st = I2C_PROC_ST_ES9039_INIT_WAITING;
        }
        break;
        case I2C_PROC_ST_ES9039_INIT_WAITING:
            if (i2c_busy || i2c_flag_get(I2C1, I2C_FLAG_I2CBSY))
            {
                break;
            }
            i2c_proc_st = I2C_PROC_ST_ES9039_COMPLETE;
            break;
        case I2C_PROC_ST_ES9039_COMPLETE:
            i2c_proc_st = I2C_PROC_ST_IDL;
            break;
        default:
            break;
        }
        if (!i2c_busy && i2c_frame.buf_length && !i2c_flag_get(I2C1, I2C_FLAG_I2CBSY))
        {
            i2c_busy = 1;
            i2c_start_on_bus(I2C1);
        }
        if (i2c_proc_safe_cnt && --i2c_proc_safe_cnt == 0)
        {
            i2c_stop_on_bus(I2C1);
            i2c_interrupt_disable(I2C1, I2C_INT_ERR);
            i2c_interrupt_disable(I2C1, I2C_INT_BUF);
            i2c_interrupt_disable(I2C1, I2C_INT_EV);
            i2c_busy = 0;
            i2c_frame.buf_length = 0;
            i2c_proc_st = I2C_PROC_ST_IDL;
            i2c_st = I2C_ST_IDL;
        }
        vTaskDelay(I2C_TASK_DELAY);
    }
}

void i2c_event_callback()
{
    switch (i2c_st)
    {
    case I2C_ST_IDL:
        if (i2c_frame.device_addr & 0X01)
        {
            i2c_st = I2C_ST_RECV_WRITE_ADDR;
        }
        else
        {
            i2c_st = I2C_ST_SEND_WRITE_ADDR;
        }
        break;
    // i2c write
    case I2C_ST_SEND_WRITE_ADDR:
        if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_SBSEND))
        {
            /* send slave address */
            i2c_master_addressing(I2C1, i2c_frame.device_addr, I2C_TRANSMITTER);
            i2c_st = I2C_ST_SEND_CLEAR_ADDR_FLAG;
        }
        break;
    case I2C_ST_SEND_CLEAR_ADDR_FLAG:
        if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_ADDSEND))
        {
            /*clear ADDSEND bit */

            i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_ADDSEND);
            i2c_st = I2C_ST_SEND_REGISTER_ADDR;
        }
        break;
    case I2C_ST_SEND_REGISTER_ADDR:
        if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_TBE))
        {
            if (--i2c_frame.registers)
            {
                i2c_data_transmit(I2C1, (i2c_frame.register_addr >> 8));
                i2c_st = I2C_ST_SEND_REGISTER_ADDR;
            }
            else
            {
                uint8_t i2c_reg_data = i2c_frame.register_addr;
                i2c_data_transmit(I2C1, i2c_reg_data);
                i2c_st = I2C_ST_SEND_REGISTER_ADDR_WAITING;
            }
        }
        break;
    case I2C_ST_SEND_REGISTER_ADDR_WAITING:
        if (i2c_flag_get(I2C1, I2C_FLAG_BTC))
        {
            i2c_st = I2C_ST_SEND_WRITE_DATA;
        }
        break;
    case I2C_ST_SEND_WRITE_DATA:
        if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_TBE))
        {
            i2c_data_transmit(I2C1, i2c_frame.buf[i2c_frame.buf_idx]);
            if (i2c_frame.buf_idx == 15)
            {
                int i = 0;
            }
            if (++i2c_frame.buf_idx >= i2c_frame.buf_length)
            {
                i2c_st = I2C_ST_SEND_STOP;
            }
        }
        break;
    case I2C_ST_SEND_STOP:
        i2c_stop_on_bus(I2C1);
        i2c_interrupt_disable(I2C1, I2C_INT_ERR);
        i2c_interrupt_disable(I2C1, I2C_INT_BUF);
        i2c_interrupt_disable(I2C1, I2C_INT_EV);
        i2c_frame.buf_length = 0;
        i2c_frame.buf_idx = 0;
        i2c_busy = 0;
        i2c_st = I2C_ST_IDL;
        break;
    // i2c read
    case I2C_ST_RECV_WRITE_ADDR:
        if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_SBSEND))
        {
            /* send slave address */
            i2c_master_addressing(I2C1, EEPROM_ADDRESS, I2C_TRANSMITTER);
            i2c_st = I2C_ST_RECV_CLEAR_ADDR_FLAG;
        }
        break;
    case I2C_ST_RECV_CLEAR_ADDR_FLAG:
        if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_ADDSEND))
        {
            /*clear ADDSEND bit */
            i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_ADDSEND);
            i2c_st = I2C_ST_RECV_REGISTER_ADDR;
        }
        break;
    case I2C_ST_RECV_REGISTER_ADDR:
        if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_TBE))
        {
            if (--i2c_frame.registers)
            {
                i2c_data_transmit(I2C1, (i2c_frame.register_addr >> 8));
                i2c_st = I2C_ST_RECV_REGISTER_ADDR;
            }
            else
            {
                i2c_data_transmit(I2C1, i2c_frame.register_addr);
                i2c_st = I2C_ST_RECV_REGISTER_ADDR_WAITING;
                i2c_start_on_bus(I2C1);
            }
        }
        break;
    case I2C_ST_RECV_REGISTER_ADDR_WAITING:
        if (i2c_flag_get(I2C1, I2C_FLAG_BTC))
        {
            i2c_st = I2C_ST_RECV_WRITE_READ_ADDR;
        }
        break;
    case I2C_ST_RECV_WRITE_READ_ADDR:
        if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_SBSEND))
        {
            i2c_master_addressing(I2C1, EEPROM_ADDRESS, I2C_RECEIVER);

            // i2c_ack_config(I2C1, I2C_ACK_DISABLE);

            i2c_st = I2C_ST_RECV_CLEAR_READ_ADDR_FLAG;
        }
        break;
    case I2C_ST_RECV_CLEAR_READ_ADDR_FLAG:
        if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_ADDSEND))
        {

            i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_ADDSEND);
            i2c_st = I2C_ST_RECV_TRANSMIT_DATA;
        }
        break;
    case I2C_ST_RECV_TRANSMIT_DATA:
        if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_RBNE))
        {
            if (i2c_frame.buf_idx == i2c_frame.buf_length- 1)
            {
                i2c_ack_config(I2C1, I2C_ACK_DISABLE);
            }
            /* read a byte from the EEPROM */
            //((uint8_t *)&user_data_save)[i2c_frame.buf_idx] = i2c_data_receive(I2C1);
            if (i2c_frame.buf_idx == 31)
            {
                int i = 0;
            }
            i2c_frame.buf[i2c_frame.buf_idx] = i2c_data_receive(I2C1);
            if (++i2c_frame.buf_idx >= i2c_frame.buf_length)
            {
                i2c_stop_on_bus(I2C1);
                i2c_interrupt_disable(I2C1, I2C_INT_ERR);
                i2c_interrupt_disable(I2C1, I2C_INT_BUF);
                i2c_interrupt_disable(I2C1, I2C_INT_EV);
                i2c_busy = 0;
                i2c_st = I2C_ST_IDL;
                i2c_frame.buf_length = 0;
                i2c_frame.buf_idx = 0;
            }
        }
        break;
    default:
        break;
    }
}
void i2c_error_callback()
{
    /* no acknowledge received */
    if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_AERR))
    {
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_AERR);
    }

    /* SMBus alert */
    if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_SMBALT))
    {
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_SMBALT);
    }

    /* bus timeout in SMBus mode */
    if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_SMBTO))
    {
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_SMBTO);
    }

    /* over-run or under-run when SCL stretch is disabled */
    if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_OUERR))
    {
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_OUERR);
    }

    /* arbitration lost */
    if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_LOSTARB))
    {
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_LOSTARB);
    }

    /* bus error */
    if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_BERR))
    {
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_BERR);
    }

    /* CRC value doesn't match */
    if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_PECERR))
    {
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_PECERR);
    }
    i2c_stop_on_bus(I2C1);
    /* disable the I2C1 interrupt */
    i2c_interrupt_disable(I2C1, I2C_INT_ERR);
    i2c_interrupt_disable(I2C1, I2C_INT_BUF);
    i2c_interrupt_disable(I2C1, I2C_INT_EV);
    i2c_frame.buf_length = 0;
    i2c_busy = 0;
    i2c_deinit(I2C1);
    // i2c1_config();
    i2c_st = I2C_ST_IDL;
    i2c_proc_st = I2C_PROC_ST_IDL;
}
