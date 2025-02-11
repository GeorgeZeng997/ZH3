#ifndef __CONFIG_H
#define __CONFIG_H


#include "gd32f30x.h"
#include "stdint.h"
#include "gd32f30x_gpio.h"
#include "gd32f30x_dma.h"
#include "gd32f30x_spi.h"
#include "gd32f30x_timer.h"
#include "stdio.h"
#include "gd32f30x_usart.h"
#include "gd32f30x_fwdgt.h"
#include "gd32f30x_adc.h"
#include "gd32f30x_dac.h"
#include "gd32f30x_i2c.h"
#include "gd32f30x_rcu.h"
#include <gd32f30x_misc.h>
#include "gd32f30x_fmc.h"
#include "string.h"

#include "flash.h"
#include "lcd.h"
#include "picture.h"
#include "st7789.h"
#include "encoder.h"
#include "menu.h"
#include "config.h"
#include "user_data.h"
#include "test.h"
#include "i2c.h"
#include "adc.h"
#include "ir.h"
#include "xmos.h"
#include "usart.h"
#include "fmc.h"
#include "tone.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#define LED1_TASK_PRIO (tskIDLE_PRIORITY + 2)
#define LCD_TASK_PRIO (tskIDLE_PRIORITY + 10)
#define FLASH_TASK_PRIO (tskIDLE_PRIORITY + 10)
#define KEY_TASK_PRIO (tskIDLE_PRIORITY + 20)
#define ENCODER_KEYSCAN_TASK_PRIO (tskIDLE_PRIORITY +20)
#define ENCODER_TASK_PRIO (tskIDLE_PRIORITY + 19)
#define MENU_TASK_PRIO (tskIDLE_PRIORITY + 9)
#define I2C_TASK_PRIO (tskIDLE_PRIORITY + 18)

#define LCD_TASK_DELAY 1
#define FLASH_TASK_DELAY 1
#define KEY_TASK_DELAY 10
#define ENCODER_KEYSCAN_TASK_DELAY 10
#define ENCODER_TASK_DELAY 1
#define MENU_TASK_DELAY 10
#define I2C_TASK_DELAY 10


#define GPIO_FLASH_PORT             GPIOB
#define GPIO_FLASH_SCK_PIN          GPIO_PIN_13
#define GPIO_FLASH_MISO_PIN         GPIO_PIN_14
#define GPIO_FLASH_MOSI_PIN         GPIO_PIN_15


#define GPIO_FLASH_CS_PORT          GPIOB
#define GPIO_FLASH_CS_PIN           GPIO_PIN_12

#define GPIO_LCD_DC_PORT    GPIOB
#define GPIO_LCD_DC_PIN     GPIO_PIN_5

 

#define LCD_DISPLAY_BRIGHTNESS_MAX   15
#define LCD_DISPLAY_BRIGHTNESS_LEVEL4   (LCD_DISPLAY_BRIGHTNESS_MAX+1)/2
#define LCD_DISPLAY_BRIGHTNESS_LEVEL3   (LCD_DISPLAY_BRIGHTNESS_MAX+1)/4
#define LCD_DISPLAY_BRIGHTNESS_LEVEL2   (LCD_DISPLAY_BRIGHTNESS_MAX+1)/8
#define LCD_DISPLAY_BRIGHTNESS_LEVEL1   (LCD_DISPLAY_BRIGHTNESS_MAX+1)/8
#define LCD_DISPLAY_BRIGHTNESS_BLACK 0

#define GPIO_ENCODER_PORT       GPIOA
#define GPIO_ENCODER_PIN_A      GPIO_PIN_2
#define GPIO_ENCODER_PIN_B      GPIO_PIN_3
#define GPIO_ENCODER_PIN_PRESS  GPIO_PIN_1

#define GPIO_LCD_PORT       GPIOA
// #define GPIO_LCD_RES_PIN    GPIO_PIN_9//RES
// #define GPIO_LCD_DC_PIN     GPIO_PIN_10//DC
#define GPIO_LCD_BLK_PIN    GPIO_PIN_11//BLK
#define GPIO_LCD_CS_PIN     GPIO_PIN_4//CS
#define GPIO_LCD_SCK_PIN    GPIO_PIN_5//SCL
#define GPIO_LCD_MISO_PIN   GPIO_PIN_6//NOT USED
#define GPIO_LCD_MOSI_PIN   GPIO_PIN_7//SDA  

#define GPIO_TONE_CS_PORT      GPIOA
#define GPIO_TONE_CS_PIN    GPIO_PIN_8

#define GPIO_TONE_PORT      GPIOB
#define GPIO_TONE_SCK_PIN   GPIO_PIN_13
#define GPIO_TONE_MOSI_PIN  GPIO_PIN_15
#define GPIO_TONE_MISO_PIN  GPIO_PIN_14 //NOT USED

#define GPIO_IR_PORT             GPIOB
#define GPIO_IR_PIN              GPIO_PIN_9
#define IR_TIMER            TIMER3
#define IR_TIMER_CLK        RCU_TIMER3
//#define IR_IRQ              TIMER0_IRQn
#define IR_TIMER_CHANNAL    TIMER_CH_3

#define GPIO_PWM_TIMER      TIMER0
#define GPIO_PWM_RCU_TIMER  RCU_TIMER0
#define GPIO_PWM_TIMER_CH   TIMER_CH_3


#define GPIO_BOOTLOADER_RESET_PORT   GPIOC
#define GPIO_BOOTLOADER_RESET_PIN    GPIO_PIN_0


#define GPIO_I2C_PORT           GPIOB
#define GPIO_I2C_SCL_PIN            GPIO_PIN_10
#define GPIO_I2C_SDA_PIN            GPIO_PIN_11

#define GPIO_ADC_PORT           GPIOA
#define GPIO_ADC_PIN            GPIO_PIN_0

#define FWDGT_DELAY_TIME        4000

#define I2C0_SLAVE_ADDRESS7     0X82

#define GPIO_UART_TX_RX_PORT    GPIOA
#define GPIO_UART_TX_PIN        GPIO_PIN_9
#define GPIO_UART_RX_PIN        GPIO_PIN_10

#define GPIO_POWER_ON_PORT      GPIOB
#define GPIO_POWER_ON_PIN       GPIO_PIN_6

#define GPIO_MUTE_PORT     GPIOA
#define GPIO_MUTE_PIN      GPIO_PIN_12

#define GPIO_MUTE_4493_PORT     GPIOB
#define GPIO_MUTE_4493_PIN      GPIO_PIN_3

#define GPIO_MUTE_PH_PORT       GPIOB
#define GPIO_MUTE_PH_PIN        GPIO_PIN_0

#define GPIO_RCA_MODE_PORT    GPIOB
#define GPIO_RCA_MODE_PIN       GPIO_PIN_8
#define GPIO_RCA_MODE_ENABLE    1
#define GPIO_RCA_MODE_DISABLE   0



extern SemaphoreHandle_t lcd_sema;
extern SemaphoreHandle_t menu_sema;
extern SemaphoreHandle_t flash_sema;

void systick_config(void);
void rcu_config(void);
void gpio_config(void);

void flash_config(void);
void lcd_config(void);

void nvic_config(void);
void semaphore_config(void);

void pwm_config(void);
void i2c_config(void);
void wdg_config(void);
void ir_config(void);
void usart_config(void);
void tone_config(void);

#endif

