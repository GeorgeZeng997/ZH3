#include "config.h"
/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
// void SVC_Handler(void)
// {
// }

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
// void PendSV_Handler(void)
// {
// }


/*
void SysTick_Handler(void)
{
   

	
}
*/
void DMA0_Channel3_IRQHandler()
{
    flash_dma_callback();
    
}

void DMA0_Channel2_IRQHandler()
{
    lcd_dma_callback();
     
}

void SPI1_IRQHandler(void)
{
    
    flash_spi1_callback();
}

void SPI0_IRQHandler()
{
    lcd_int_callback();
}
void I2C1_EV_IRQHandler(void)
{
    i2c_event_callback();
}
void I2C1_ER_IRQHandler(void)
{
    i2c_error_callback();
}
void TIMER3_IRQHandler(void)
{
    if((RESET != timer_interrupt_flag_get(IR_TIMER, TIMER_INT_CH3))) {
         timer_interrupt_flag_clear(IR_TIMER,TIMER_INT_FLAG_CH3);
       // uint32_t cnt = timer_channel_capture_value_register_read(TIMER0,TIMER_CH_3);
         ir_callback(timer_channel_capture_value_register_read(IR_TIMER,TIMER_CH_3));
        
    }
}
extern void usart0_rx_callback(uint8_t data);
extern uart_txrx_buf_t uart_tx_buf;
extern uart_blk_t uart_blk;
void USART0_IRQHandler()
{
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)){
        /* receive data */
        uint8_t rx_data = usart_data_receive(USART0);
        usart0_rx_callback(rx_data);
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
    }
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_TBE)){
        /* transmit data */
        usart_data_transmit(USART0, uart_tx_buf.buf[uart_tx_buf.idx]);
        if(++uart_tx_buf.idx>=uart_tx_buf.length)
        {
            uart_tx_buf.length = 0;
            uart_tx_buf.idx = 0;
            uart_blk.uart_busy = 0;
            usart_interrupt_disable(USART0,USART_INT_TBE);
        }
    }
}

