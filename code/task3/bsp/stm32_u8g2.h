#ifndef __STM32_U8G2_H
#define __STM32_U8G2_H

#include "main.h"
#include "u8g2.h"
#include "i2c.h"
#include "tim.h"

/* SSD1306 写地址：7 位地址 0x3C 左移一位 */
#define OLED_ADDRESS   0x78
#define OLED_CMD       0x00
#define OLED_DATA      0x40

/* 通信层：硬件 I2C 发送 */
uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

/* GPIO / 延时层：本工程用 TIM1 做微秒延时 */
uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

void Tims_delay_us(uint16_t us);
void u8g2Init(u8g2_t *u8g2);
void draw(u8g2_t *u8g2);

#endif /* __STM32_U8G2_H */