#include "oled.h"
#include "oled_font.h"
#include "oled_cn_font.h"

/* =====================================================================
 * 一、软件 I2C 底层（SCL=PB8，SDA=PB9）
 * ---------------------------------------------------------------------
 * 移植说明（相对课程资料的标准库版本）：
 *   1. 去掉 #include "stm32f10x.h"，改为包含 stm32f1xx_hal.h（见 oled.h）；
 *   2. 去掉 OLED_I2C_Init() 里的 RCC 时钟使能与 GPIO_Init —— 这两件事
 *      已由 CubeMX 生成的 MX_GPIO_Init() 完成（开漏 + 上拉 + 高速）；
 *   3. GPIO_WriteBit() 换成 HAL_GPIO_WritePin()；
 *   4. 上电延时由 1e6 次空循环换成 HAL_Delay(100)；
 *   5. 只被本文件使用的函数加 static，对外接口与课程资料保持一致。
 * ===================================================================== */
/* 软件 I2C 的位延时：HAL_GPIO_WritePin 调用本身只要几十个时钟周期，位间隔太短时
 * 开漏 + 上拉的上升沿来不及爬到高电平，SSD1306 会认不出数据（表现为全黑或花屏）。
 * 这里每次电平变化后插约 1 µs，把 SCL 压到 ~200 kHz，远低于 SSD1306 的 400 kHz 上限。 */
static void OLED_I2C_Delay(void)
{
    for (volatile uint8_t i = 0; i < 12; i++)
    {
        __NOP();
    }
}

static void OLED_W_SCL(uint8_t Level)
{
    HAL_GPIO_WritePin(OLED_SCL_PORT, OLED_SCL_PIN, Level ? GPIO_PIN_SET : GPIO_PIN_RESET);
    OLED_I2C_Delay();
}

static void OLED_W_SDA(uint8_t Level)
{
    HAL_GPIO_WritePin(OLED_SDA_PORT, OLED_SDA_PIN, Level ? GPIO_PIN_SET : GPIO_PIN_RESET);
    OLED_I2C_Delay();
}

/* I2C 起始：SCL 高电平期间 SDA 由高变低 */
static void OLED_I2C_Start(void)
{
    OLED_W_SDA(1);
    OLED_W_SCL(1);
    OLED_W_SDA(0);
    OLED_W_SCL(0);
}

/* I2C 停止：SCL 高电平期间 SDA 由低变高 */
static void OLED_I2C_Stop(void)
{
    OLED_W_SDA(0);
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

/* 发送一个字节，高位在前；第 9 个时钟不读应答（SSD1306 只写不读） */
static void OLED_I2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        OLED_W_SDA(!!(Byte & (0x80 >> i)));
        OLED_W_SCL(1);
        OLED_W_SCL(0);
    }
    OLED_W_SCL(1);      /* 额外一个时钟给从机应答 */
    OLED_W_SCL(0);
}

/* 写命令：0x00 表示后面的字节是命令 */
static void OLED_WriteCommand(uint8_t Command)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(OLED_I2C_ADDR_W);
    OLED_I2C_SendByte(0x00);
    OLED_I2C_SendByte(Command);
    OLED_I2C_Stop();
}

/* 写数据：0x40 表示后面的字节是显示数据 */
static void OLED_WriteData(uint8_t Data)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(OLED_I2C_ADDR_W);
    OLED_I2C_SendByte(0x40);
    OLED_I2C_SendByte(Data);
    OLED_I2C_Stop();
}

/* =====================================================================
 * 二、光标与清屏
 * ===================================================================== */
/* 设置光标：Y 为页地址 0~7，X 为列地址 0~127 */
static void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    OLED_WriteCommand(0xB0 | Y);                    /* 页地址 */
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));    /* 列地址高 4 位 */
    OLED_WriteCommand(0x00 | (X & 0x0F));           /* 列地址低 4 位 */
}

void OLED_Clear(void)
{
    uint8_t j, i;
    for (j = 0; j < 8; j++)
    {
        OLED_SetCursor(j, 0);
        for (i = 0; i < 128; i++)
        {
            OLED_WriteData(0x00);
        }
    }
}

/* =====================================================================
 * 三、字符显示（8x16 点阵）
 * ===================================================================== */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{
    uint8_t i;

    if (Line < 1 || Line > OLED_LINE_MAX || Column < 1 || Column > OLED_COL_MAX)
    {
        return;                                     /* 越界直接忽略，防止花屏 */
    }
    if (Char < ' ' || Char > '~')
    {
        Char = '?';                                 /* 非可见 ASCII 用问号代替 */
    }

    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);        /* 上半页 8 列 */
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i]);
    }
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);    /* 下半页 8 列 */
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);
    }
}

void OLED_ShowString(uint8_t Line, uint8_t Column, const char *String)
{
    while (*String != '\0')
    {
        OLED_ShowChar(Line, Column, *String);
        Column++;
        String++;
        if (Column > OLED_COL_MAX)                  /* 超出一行就不再往右画 */
        {
            break;
        }
    }
}

/* =====================================================================
 * 四、数字显示
 * ===================================================================== */
static uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    return Result;
}

void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0)
    {
        OLED_ShowChar(Line, Column, '+');
        Number1 = (uint32_t)Number;
    }
    else
    {
        OLED_ShowChar(Line, Column, '-');
        Number1 = (uint32_t)(-Number);
    }
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++)
    {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10)
        {
            OLED_ShowChar(Line, Column + i, SingleNumber + '0');
        }
        else
        {
            OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
        }
    }
}

void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
    }
}

/* =====================================================================
 * 五、16x16 汉字（字模来自取模软件，见 oled_cn_font.h）
 * ===================================================================== */
void OLED_ShowChinese(uint8_t Line, uint8_t Column, const uint8_t *Font)
{
    uint8_t i;

    if (Line < 1 || Line > OLED_LINE_MAX || Column < 1 || Column > OLED_COL_MAX - 1)
    {
        return;
    }

    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);        /* 上半页 */
    for (i = 0; i < 16; i++)
    {
        OLED_WriteData(Font[i]);
    }
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);    /* 下半页 */
    for (i = 0; i < 16; i++)
    {
        OLED_WriteData(Font[i + 16]);
    }
}

/* 按 Unicode 码点在字库表中查字模，找不到就显示 '?' */
void OLED_ShowCNChar(uint8_t Line, uint8_t Column, uint16_t Code)
{
    uint8_t i;

    for (i = 0; i < OLED_CN_NUM; i++)
    {
        if (OLED_CN_Table[i].code == Code)
        {
            OLED_ShowChinese(Line, Column, OLED_CN_Table[i].data);
            return;
        }
    }
    OLED_ShowChar(Line, Column, '?');
}

/* 显示一串汉字：Str 是 Unicode 码点数组，以 0 结尾；每个汉字占 2 列宽 */
void OLED_ShowCNString(uint8_t Line, uint8_t Column, const uint16_t *Str)
{
    while (*Str != 0)
    {
        OLED_ShowCNChar(Line, Column, *Str);
        Column += 2;
        Str++;
        if (Column > OLED_COL_MAX - 1)
        {
            break;
        }
    }
}

/* =====================================================================
 * 六、初始化（SSD1306 上电命令序列，与课程资料一致）
 * ===================================================================== */
void OLED_Init(void)
{
    HAL_Delay(100);         /* 等 OLED 电源稳定 */

    OLED_W_SCL(1);
    OLED_W_SDA(1);

    OLED_WriteCommand(0xAE);    /* 关闭显示 */

    OLED_WriteCommand(0xD5);    /* 显示时钟分频比 / 振荡器频率 */
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xA8);    /* 多路复用率 = 64 */
    OLED_WriteCommand(0x3F);

    OLED_WriteCommand(0xD3);    /* 显示偏移 = 0 */
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x40);    /* 显示起始行 = 0 */

    OLED_WriteCommand(0xA1);    /* 左右方向：0xA1 正常 */
    OLED_WriteCommand(0xC8);    /* 上下方向：0xC8 正常 */

    OLED_WriteCommand(0xDA);    /* COM 引脚硬件配置 */
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0x81);    /* 对比度 */
    OLED_WriteCommand(0xCF);

    OLED_WriteCommand(0xD9);    /* 预充电周期 */
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB);    /* VCOMH 取消选择级别 */
    OLED_WriteCommand(0x30);

    OLED_WriteCommand(0xA4);    /* 整个显示打开 */
    OLED_WriteCommand(0xA6);    /* 正常显示（非反色） */

    OLED_WriteCommand(0x8D);    /* 充电泵 */
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0xAF);    /* 开启显示 */

    OLED_Clear();
}
