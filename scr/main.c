/*
GD32F303RET6
SeriesGD32F303
PackageLQFP64
Max Speed (MHz)120
Flash (Bytes)512K
SRAM (Bytes)64K
I/O up to 55
*/

#include "config.h"


uint8_t screen_cnt = 0;
uint8_t picture_test1 = 0;
uint8_t picture_test2 = 0;

uint8_t lcd_test_f=0;
void LED1_task(void *pvParameters)
{
    while (1)
    {
     //   uart_tx_qu_check_in(XMOS_CMD_IDX_UPDATE_PACK_SUCCESS, uart_tx_frame.buf, uart_package(NULL,0,XMOS_CMD_IDX_UPDATE_PACK_SUCCESS));

    //     static uint8_t bri_val=0;
    //     if(++bri_val>=15)
    //         bri_val = 0;
	// 		//vTaskDelay(1000);
    //     timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_3, bri_val);
    // timer_auto_reload_shadow_enable(TIMER0);
        vTaskDelay(1000);
        //gpio_bit_set(GPIO_MUTE_4493_PORT, GPIO_MUTE_4493_PIN);
        //lcd_display_rectangle(0,0,240,240,RED);
        //lcd_display_picture(0,0,FLASH_PICTURE_IDX_MENU_3);
        
        //gpio_bit_reset(GPIO_LED_PORT, GPIO_LED_BLUE_PIN);
        vTaskDelay(1000);
        //i2c_msg_check_in(I2C_MSG_READ_USER_DATA);
        //i2c_msg_check_in(I2C_MSG_WRITE_4493_DATA);
        //gpio_bit_reset(GPIO_MUTE_4493_PORT, GPIO_MUTE_4493_PIN);
        //lcd_display_rectangle(0,0,240,240,YELLOW);
        //lcd_display_picture(0,0,FLASH_PICTURE_IDX_MENU_4);

        // if(user_data.ph_volume>=99)
        // {
        //     user_data.ph_volume = 0;
        // }
        // else
        // {
        //     menu_msg_check_in(MENU_MSG_FWD);
        // }
        // x++;
        // y++;
        // lcd_display_rectangle(x,y,50,50,RED);
        //gpio_bit_reset(GPIO_BOOTLOADER_RESET_PORT,GPIO_BOOTLOADER_RESET_PIN);
    }
}
int main(void)
{
BaseType_t xReturned;
    // systick_config();
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);

    rcu_config();
    gpio_config();
pwm_config();
    lcd_config();
    flash_config();

    semaphore_config();

    
    i2c_config();
    // wdg_config();
    ir_config();
    usart_config();
    nvic_config();
    

    xTaskCreate(LED1_task, "LED1", configMINIMAL_STACK_SIZE, NULL, LED1_TASK_PRIO, NULL);

    //xTaskCreate(key_task, "KEY_SCAN", configMINIMAL_STACK_SIZE, NULL, KEY_TASK_PRIO, NULL);

    xTaskCreate(flash_task, "FLASH", configMINIMAL_STACK_SIZE, NULL, FLASH_TASK_PRIO, NULL);

    xTaskCreate(lcd_task, "LCD", configMINIMAL_STACK_SIZE, NULL, LCD_TASK_PRIO, NULL);

    xTaskCreate(encoder_task, "ENCODER_TASK", configMINIMAL_STACK_SIZE, NULL, ENCODER_TASK_PRIO, NULL);

    xTaskCreate(encoder_keyscan_task, "ENCODER_KEY_SCAN", configMINIMAL_STACK_SIZE, NULL, ENCODER_KEYSCAN_TASK_PRIO, NULL);

    xTaskCreate(menu_task, "MENU", configMINIMAL_STACK_SIZE, NULL, MENU_TASK_PRIO, NULL);

    xReturned = xTaskCreate(ir_task, "IR", configMINIMAL_STACK_SIZE, NULL, MENU_TASK_PRIO, NULL);
    if(xReturned!=pdPASS)
    {
        int i=0;
    }

    xReturned = xTaskCreate(uart_task,"UART", configMINIMAL_STACK_SIZE*8, NULL, MENU_TASK_PRIO+1, NULL);
    if(xReturned!=pdPASS)
    {
        int i=0;
    }
    xTaskCreate(i2c_task,"I2C",configMINIMAL_STACK_SIZE, NULL, I2C_TASK_PRIO, NULL);

    //xTaskCreate(adc_task,"ADC",configMINIMAL_STACK_SIZE, NULL, MENU_TASK_PRIO, NULL);

    /* start scheduler */
    vTaskStartScheduler();
    while (1)
    {
    }
}
