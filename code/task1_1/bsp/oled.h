#ifndef __OLED_H
#define __OLED_H

#include "stm32f1xx_hal.h"

/* =====================================================================
 * 0.96 寸 OLED（SSD1306，128x64）HAL 版驱动 —— 由课程资料"4针脚I2C版本"
 * 的标准库驱动裁剪移植而来，仅保留显示功能，底层改成 HAL + 软件 I2C。
 *
 * 硬件约定（与 CubeMX 中 PB8/PB9 的配置一一对应）：
 *   PB8 -> OLED SCL，GPIO 开漏输出（Output Open Drain）+ 上拉
 *   PB9 -> OLED SDA，GPIO 开漏输出（Output Open Drain）+ 上拉
 *   SSD1306 的 7 位地址 0x3C，写方向拼上 0 后为 0x78
 * ===================================================================== */
#define OLED_SCL_PORT       GPIOB
#define OLED_SCL_PIN        GPIO_PIN_8
#define OLED_SDA_PORT       GPIOB
#define OLED_SDA_PIN        GPIO_PIN_9

#define OLED_I2C_ADDR_W     0x78

/* 字符定位：8x16 字符，一共 4 行（Line 1~4），每行 16 列（Column 1~16） */
#define OLED_LINE_MAX       4
#define OLED_COL_MAX        16

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, const char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);


void OLED_ShowChinese(uint8_t Line, uint8_t Column, const uint8_t *Font);
void OLED_ShowCNChar(uint8_t Line, uint8_t Column, uint16_t Code);
void OLED_ShowCNString(uint8_t Line, uint8_t Column, const uint16_t *Str);

#endif /* __OLED_H */
