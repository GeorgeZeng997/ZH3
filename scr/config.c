#include "config.h"

void systick_config()
{
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        /* capture error */
        while (1)
        {
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}
void nvic_config()
{
    // dma interrupt config
    dma_interrupt_enable(DMA0, DMA_CH2, DMA_INT_FTF);
    nvic_irq_enable(DMA0_Channel2_IRQn, 1, 0);

    dma_interrupt_enable(DMA0, DMA_CH3, DMA_INT_FTF);
    nvic_irq_enable(DMA0_Channel3_IRQn, 2, 0);

    // spi0 interrupt config
    nvic_irq_enable(SPI0_IRQn, 3, 0);
    // spi_i2s_interrupt_enable(SPI0, SPI_I2S_INT_TBE);
    // SPI1 INT CONFIG
    nvic_irq_enable(SPI1_IRQn, 4, 0);

    nvic_irq_enable(I2C1_EV_IRQn, 4, 0);
    nvic_irq_enable(I2C1_ER_IRQn, 4, 0);

    nvic_irq_enable(TIMER3_IRQn, 4, 0);

    nvic_irq_enable(USART0_IRQn, 4, 0);
}

void rcu_config()
{
    rcu_ahb_clock_config(RCU_AHB_CKSYS_DIV1);
    rcu_periph_clock_enable(RCU_DMA0);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_I2C1);

    rcu_periph_clock_enable(RCU_ADC0);
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV4);

    rcu_periph_clock_enable(RCU_DMA1);

    rcu_periph_clock_enable(RCU_TIMER5);

    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(GPIO_PWM_RCU_TIMER);
}

void gpio_config()
{
    // // LED CONFIG
    // rcu_periph_clock_enable(RCU_GPIOA);

    // gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    // gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);

    // // KEY CONFIG
    // rcu_periph_clock_enable(RCU_GPIOE);
    // gpio_init(GPIO_KEY_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_KEY0_PIN);
    // gpio_init(GPIO_KEY_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_KEY1_PIN);

    // FLASH CONFIG
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(GPIO_FLASH_CS_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_FLASH_CS_PIN);
    gpio_init(GPIO_FLASH_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_FLASH_MOSI_PIN | GPIO_FLASH_SCK_PIN);
    gpio_init(GPIO_FLASH_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_FLASH_MISO_PIN);
    // gpio_pin_remap_config(GPIO_SPI0_REMAP,ENABLE);
    FLASH_CS_DISABLE;

    // LCD CONFIG
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(GPIO_LCD_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_MOSI_PIN | GPIO_LCD_SCK_PIN);
    gpio_init(GPIO_LCD_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_LCD_MISO_PIN);
    gpio_init(GPIO_LCD_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_CS_PIN); //| GPIO_LCD_RES_PIN );
    gpio_init(GPIO_LCD_DC_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_LCD_DC_PIN);
    gpio_bit_set(GPIO_LCD_PORT, GPIO_LCD_CS_PIN); //| GPIO_LCD_RES_PIN);
    gpio_bit_reset(GPIO_LCD_DC_PORT, GPIO_LCD_DC_PIN);

    // gpio_pin_remap_config(GPIO_TIMER0_FULL_REMAP, ENABLE);
    gpio_init(GPIO_LCD_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    // gpio_bit_set(GPIO_LCD_PORT,GPIO_LCD_BLK_PIN);

    // ENCODER CONFIG
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIO_ENCODER_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_ENCODER_PIN_A | GPIO_ENCODER_PIN_B | GPIO_ENCODER_PIN_PRESS);
    // TEST CONFIG
    // gpio_init(GPIO_TEST_PORT,GPIO_MODE_IPU,GPIO_OSPEED_50MHZ,GPIO_TEST_PIN);

    // I2C CONFIG
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_I2C_SCL_PIN | GPIO_I2C_SDA_PIN);

    // // ADC CONFIG
    // gpio_init(GPIO_ADC_PORT, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_ADC_PIN);

    // // BOOTLOADER CONFIG
    // gpio_init(GPIO_BOOTLOADER_RESET_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_BOOTLOADER_RESET_PIN);
    // gpio_bit_set(GPIO_BOOTLOADER_RESET_PORT, GPIO_BOOTLOADER_RESET_PIN);

    // IR CONFIG
    gpio_init(GPIO_IR_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_IR_PIN);

    // // UART CONFIG
    gpio_init(GPIO_UART_TX_RX_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_UART_TX_PIN);
    gpio_init(GPIO_UART_TX_RX_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_UART_RX_PIN);

    // POWER CONFIG
    gpio_init(GPIO_POWER_ON_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_POWER_ON_PIN);
    gpio_bit_set(GPIO_POWER_ON_PORT, GPIO_POWER_ON_PIN);

    gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);
    // MUTE
    gpio_init(GPIO_MUTE_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_MUTE_PIN);
    gpio_init(GPIO_MUTE_4493_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_MUTE_4493_PIN);
    gpio_init(GPIO_MUTE_PH_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_MUTE_PH_PIN);
    gpio_bit_set(GPIO_MUTE_PORT, GPIO_MUTE_PIN);
    gpio_bit_set(GPIO_MUTE_4493_PORT, GPIO_MUTE_4493_PIN);
    gpio_bit_set(GPIO_MUTE_PH_PORT, GPIO_MUTE_PH_PIN);

    //RCA MODE
    gpio_init(GPIO_RCA_MODE_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_RCA_MODE_PIN);
    gpio_bit_set(GPIO_RCA_MODE_PORT,GPIO_RCA_MODE_PIN);

    //TONE CONFIG
    gpio_init(GPIO_TONE_CS_PORT,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_TONE_CS_PIN);
    gpio_bit_set(GPIO_TONE_CS_PORT,GPIO_TONE_CS_PIN);
}
//mode 2
void tone_config()
{
    rcu_periph_clock_enable(RCU_SPI1);
    spi_parameter_struct flash_spi_init;
    // spi_i2s_deinit(SPI0);
    flash_spi_init.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    flash_spi_init.device_mode = SPI_MASTER;
    flash_spi_init.frame_size = SPI_FRAMESIZE_8BIT;
    flash_spi_init.clock_polarity_phase = SPI_CK_PL_HIGH_PH_1EDGE;
    // flash_spi_init.nss = SPI_NSS_HARD;
    flash_spi_init.nss = SPI_NSS_SOFT;
    flash_spi_init.prescale = SPI_PSC_256;
    flash_spi_init.endian = SPI_ENDIAN_MSB;
    spi_init(SPI1, &flash_spi_init);
    spi_enable(SPI1);
}

void flash_config()
{
    rcu_periph_clock_enable(RCU_SPI1);
    spi_parameter_struct flash_spi_init;
    // spi_i2s_deinit(SPI0);
    flash_spi_init.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    flash_spi_init.device_mode = SPI_MASTER;
    flash_spi_init.frame_size = SPI_FRAMESIZE_8BIT;
    flash_spi_init.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    // flash_spi_init.nss = SPI_NSS_HARD;
    flash_spi_init.nss = SPI_NSS_SOFT;
    flash_spi_init.prescale = SPI_PSC_4;
    flash_spi_init.endian = SPI_ENDIAN_MSB;
    spi_init(SPI1, &flash_spi_init);
    spi_enable(SPI1);
}

void lcd_config()
{
    rcu_periph_clock_enable(RCU_SPI0);
    spi_parameter_struct flash_spi_init;
    // spi_i2s_deinit(SPI1);
    flash_spi_init.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    flash_spi_init.device_mode = SPI_MASTER;
    flash_spi_init.frame_size = SPI_FRAMESIZE_8BIT;
    flash_spi_init.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    // flash_spi_init.nss = SPI_NSS_HARD;
    flash_spi_init.nss = SPI_NSS_SOFT;
    flash_spi_init.prescale = SPI_PSC_8;
    flash_spi_init.endian = SPI_ENDIAN_MSB;
    spi_init(SPI0, &flash_spi_init);
    spi_enable(SPI0);
}
SemaphoreHandle_t lcd_sema;
SemaphoreHandle_t menu_sema;
SemaphoreHandle_t flash_sema;

void semaphore_config()
{
    lcd_sema = xSemaphoreCreateBinary();
    menu_sema = xSemaphoreCreateBinary();
    flash_sema = xSemaphoreCreateBinary();

    xSemaphoreGive(lcd_sema);
    xSemaphoreGive(menu_sema);
    xSemaphoreGive(flash_sema);
}

void pwm_config()
{
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(GPIO_PWM_RCU_TIMER);

    timer_deinit(TIMER0);

    /* TIMER0 configuration */
    timer_initpara.prescaler = 119;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = LCD_DISPLAY_BRIGHTNESS_MAX;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0, &timer_initpara);

    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER0, TIMER_CH_3, &timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_3, 15);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_3, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);

    timer_primary_output_config(GPIO_PWM_TIMER, ENABLE);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER0);

    /* auto-reload preload enable */
    timer_enable(TIMER0);

    // timer_oc_parameter_struct timer_ocintpara;
    // timer_parameter_struct timer_initpara;

    // rcu_periph_clock_enable(GPIO_PWM_RCU_TIMER);

    // timer_deinit(GPIO_PWM_TIMER);

    // /* TIMER0 configuration */
    // timer_initpara.prescaler         = 5999;
    // timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    // timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    // timer_initpara.period            = 15999;
    // timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    // timer_initpara.repetitioncounter = 0;
    // timer_init(GPIO_PWM_TIMER,&timer_initpara);

    //  /* CH1,CH2 and CH3 configuration in PWM mode */
    // timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    // timer_ocintpara.outputnstate = TIMER_CCXN_ENABLE;
    // timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    // timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    // timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    // timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    // timer_channel_output_config(GPIO_PWM_TIMER,TIMER_CH_3,&timer_ocintpara);

    // timer_channel_output_pulse_value_config(GPIO_PWM_TIMER,TIMER_CH_3,3999);
    // timer_channel_output_mode_config(GPIO_PWM_TIMER,TIMER_CH_3,TIMER_OC_MODE_PWM0);
    // timer_channel_output_shadow_config(GPIO_PWM_TIMER,TIMER_CH_3,TIMER_OC_SHADOW_DISABLE);

    // timer_primary_output_config(GPIO_PWM_TIMER,ENABLE);

    // /* auto-reload preload enable */
    // timer_auto_reload_shadow_enable(GPIO_PWM_TIMER);
    // timer_enable(TIMER0);
}
void ir_config()
{
    timer_ic_parameter_struct timer_icinitpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(IR_TIMER_CLK);

    /* timer base configuration for timer IR_TIMER */
    timer_initpara.prescaler = 119;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = 65535;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(IR_TIMER, &timer_initpara);

    timer_icinitpara.icpolarity = TIMER_IC_POLARITY_FALLING;
    timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
    timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;
    timer_icinitpara.icfilter = 0x0;
    timer_input_capture_config(IR_TIMER, IR_TIMER_CHANNAL, &timer_icinitpara);
    timer_auto_reload_shadow_enable(IR_TIMER);
    timer_interrupt_flag_clear(IR_TIMER, TIMER_INT_FLAG_CH3);
    /* channel 0 interrupt enable */
    timer_interrupt_enable(IR_TIMER, TIMER_INT_CH3);
    timer_enable(IR_TIMER);
}

void i2c_config()
{
    i2c_clock_config(I2C1, 5000, I2C_DTCY_2);
    /* configure I2C1 address */
    i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C0_SLAVE_ADDRESS7);
    /* enable I2C1 */
    i2c_enable(I2C1);
    /* enable acknowledge */
    i2c_ack_config(I2C1, I2C_ACK_ENABLE);
}

void usart_config()
{
    rcu_periph_clock_enable(RCU_GPIOA);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* configure USART */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    // usart_dma_receive_config(USART0, USART_RECEIVE_DMA_ENABLE);
    usart_enable(USART0);

    usart_interrupt_enable(USART0, USART_INT_RBNE);
}

// 6.4 second fwdg timer
void wdg_config()
{
    fwdgt_config(FWDGT_DELAY_TIME, FWDGT_PSC_DIV64); // 40K IRC 64 PRESCALE
    fwdgt_enable();
}
