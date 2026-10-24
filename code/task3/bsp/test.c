#include "test.h"
#include "stm32_u8g2.h"
#include "oled_cn_font.h"
#include <stdio.h>
#include <string.h>

#define SEND_BUFFER_DISPLAY_MS(u8g2, ms) \
    do { u8g2_SendBuffer(u8g2); HAL_Delay(ms); } while (0)

/* =====================================================================
 * 一、博客 4.2 的测试函数
 * ===================================================================== */
void testDrawProcess(u8g2_t *u8g2)
{
    char buff[8];

    for (int i = 10; i <= 80; i += 2)
    {
        u8g2_ClearBuffer(u8g2);

        sprintf(buff, "%d%%", (int)(i / 80.0 * 100));

        u8g2_SetFont(u8g2, u8g2_font_ncenB12_tf);
        u8g2_DrawStr(u8g2, 16, 32, "STM32 U8g2");

        u8g2_SetFont(u8g2, u8g2_font_ncenB08_tf);
        u8g2_DrawStr(u8g2, 100, 49, buff);

        u8g2_DrawRBox(u8g2, 16, 40, i, 10, 4);
        u8g2_DrawRFrame(u8g2, 16, 40, 80, 10, 4);

        u8g2_SendBuffer(u8g2);
    }
    HAL_Delay(300);
}

void testShowFont(u8g2_t *u8g2)
{
    char testStr[14] = "STM32F103C8T6";

    u8g2_ClearBuffer(u8g2);

    u8g2_SetFont(u8g2, u8g2_font_6x12_tf);
    u8g2_DrawStr(u8g2, 0, 12, testStr);
    SEND_BUFFER_DISPLAY_MS(u8g2, 800);

    u8g2_SetFont(u8g2, u8g2_font_ncenB08_tf);
    u8g2_DrawStr(u8g2, 0, 32, testStr);
    SEND_BUFFER_DISPLAY_MS(u8g2, 800);

    u8g2_SetFont(u8g2, u8g2_font_ncenB12_tf);
    u8g2_DrawStr(u8g2, 0, 56, testStr);
    SEND_BUFFER_DISPLAY_MS(u8g2, 900);
}

void testDrawFrame(u8g2_t *u8g2)
{
    int x = 16, y = 32, w = 50, h = 20;

    u8g2_ClearBuffer(u8g2);
    u8g2_SetFont(u8g2, u8g2_font_6x12_tf);
    u8g2_DrawStr(u8g2, 0, 15, "DrawFrame");

    u8g2_DrawFrame(u8g2, x, y, w, h);
    SEND_BUFFER_DISPLAY_MS(u8g2, 900);
    u8g2_DrawFrame(u8g2, x + w + 5, y - 10, w - 20, h + 20);
    SEND_BUFFER_DISPLAY_MS(u8g2, 900);
}

void testDrawRBox(u8g2_t *u8g2)
{
    int x = 16, y = 32, w = 50, h = 20, r = 3;

    u8g2_ClearBuffer(u8g2);
    u8g2_SetFont(u8g2, u8g2_font_6x12_tf);
    u8g2_DrawStr(u8g2, 0, 15, "DrawRBox");

    u8g2_DrawRBox(u8g2, x, y, w, h, r);
    SEND_BUFFER_DISPLAY_MS(u8g2, 900);
    u8g2_DrawRBox(u8g2, x + w + 5, y - 10, w - 20, h + 20, r);
    SEND_BUFFER_DISPLAY_MS(u8g2, 900);
}

void testDrawCircle(u8g2_t *u8g2)
{
    int stx = 0, sty = 16, with = 16, r = 15;

    u8g2_ClearBuffer(u8g2);
    u8g2_SetFont(u8g2, u8g2_font_6x12_tf);
    u8g2_DrawStr(u8g2, 0, 15, "DrawCircle");

    u8g2_DrawCircle(u8g2, stx, sty - 1 + with, r, U8G2_DRAW_UPPER_RIGHT);
    SEND_BUFFER_DISPLAY_MS(u8g2, 400);
    u8g2_DrawCircle(u8g2, stx + with, sty, r, U8G2_DRAW_LOWER_RIGHT);
    SEND_BUFFER_DISPLAY_MS(u8g2, 400);
    u8g2_DrawCircle(u8g2, stx - 1 + with * 3, sty - 1 + with, r, U8G2_DRAW_UPPER_LEFT);
    SEND_BUFFER_DISPLAY_MS(u8g2, 400);
    u8g2_DrawCircle(u8g2, stx - 1 + with * 4, sty, r, U8G2_DRAW_LOWER_LEFT);
    SEND_BUFFER_DISPLAY_MS(u8g2, 400);
    u8g2_DrawCircle(u8g2, stx - 1 + with * 2, sty - 1 + with * 2, r, U8G2_DRAW_ALL);
    SEND_BUFFER_DISPLAY_MS(u8g2, 400);
    u8g2_DrawCircle(u8g2, 32 * 3, 32, 31, U8G2_DRAW_ALL);
    SEND_BUFFER_DISPLAY_MS(u8g2, 700);
}

void testDrawFilledEllipse(u8g2_t *u8g2)
{
    int with = 16;
    int rx = 27, ry = 22;

    u8g2_ClearBuffer(u8g2);
    u8g2_SetFont(u8g2, u8g2_font_6x12_tf);
    u8g2_DrawStr(u8g2, 0, 14, "DrawFilledEllipse");
    u8g2_SendBuffer(u8g2);
    HAL_Delay(500);

    u8g2_DrawFilledEllipse(u8g2, with, with, rx, ry, U8G2_DRAW_LOWER_RIGHT);
    SEND_BUFFER_DISPLAY_MS(u8g2, 500);
    u8g2_DrawFilledEllipse(u8g2, with * 4 - 1, with, rx, ry, U8G2_DRAW_LOWER_LEFT);
    SEND_BUFFER_DISPLAY_MS(u8g2, 500);
    u8g2_DrawFilledEllipse(u8g2, with, with * 4 - 1, rx, ry, U8G2_DRAW_UPPER_RIGHT);
    SEND_BUFFER_DISPLAY_MS(u8g2, 500);
    u8g2_DrawFilledEllipse(u8g2, with * 4 - 1, with * 4 - 1, rx, ry, U8G2_DRAW_UPPER_LEFT);
    SEND_BUFFER_DISPLAY_MS(u8g2, 500);
    u8g2_DrawFilledEllipse(u8g2, 64, 32, rx, ry, U8G2_DRAW_ALL);
    SEND_BUFFER_DISPLAY_MS(u8g2, 900);
}

/* =====================================================================
 * 二、任务三要求的画面
 * ===================================================================== */
/* 画一个 16x16 汉字：把 oled_cn_font.h 里的“页式字模”逐点还原成像素 */
static void draw_cn16(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, uint16_t code)
{
    const OLED_CN_Glyph *g = NULL;
    uint8_t i;

    for (i = 0; i < OLED_CN_NUM; i++)
    {
        if (OLED_CN_Table[i].code == code)
        {
            g = &OLED_CN_Table[i];
            break;
        }
    }
    if (g == NULL)
    {
        return;
    }

    for (uint8_t row = 0; row < 16; row++)
    {
        for (uint8_t col = 0; col < 16; col++)
        {
            if ((g->data[(row / 8) * 16 + col] >> (row % 8)) & 0x01)
            {
                u8g2_DrawPixel(u8g2, x + col, y + row);
            }
        }
    }
}

/* ---- 中英混排工具：ASCII 占 8 像素宽、汉字占 16 像素宽 ---- */
/* 长句：Hello，欢迎来到重庆交通大学物联网205实训室！ （汉字都来自任务一的取模字库） */
static const uint16_t CN_LONG_MSG[] = {
    'H', 'e', 'l', 'l', 'o', 0xFF0C,
    0x6B22, 0x8FCE, 0x6765, 0x5230,                    /* 欢迎来到 */
    0x91CD, 0x5E86, 0x4EA4, 0x901A, 0x5927, 0x5B66,    /* 重庆交通大学 */
    0x7269, 0x8054, 0x7F51,                            /* 物联网 */
    '2', '0', '5',
    0x5B9E, 0x8BAD, 0x5BA4,                            /* 实训室 */
    0xFF01,                                            /* ！ */
    0x0000
};

/* 一串混排文字的总宽度（像素） */
static int mixed_width(const uint16_t *s)
{
    int w = 0;
    while (*s)
    {
        w += (*s < 0x80) ? 8 : 16;
        s++;
    }
    return w;
}

/* 以 (x, y) 为左上角画一串混排文字：y 是汉字顶部，ASCII 基线自动下移 10 像素 */
static void draw_mixed(u8g2_t *u8g2, int x, int y, const uint16_t *s)
{
    u8g2_SetFont(u8g2, u8g2_font_6x12_tf);
    while (*s)
    {
        if (*s < 0x80)
        {
            char c[2] = { (char)(*s), 0 };
            u8g2_DrawStr(u8g2, (u8g2_uint_t)x, (u8g2_uint_t)(y + 10), c);
            x += 8;
        }
        else
        {
            draw_cn16(u8g2, (u8g2_uint_t)x, (u8g2_uint_t)y, *s);
            x += 16;
        }
        s++;
    }
}

/* 学号 + 姓名 */
void testShowIdName(u8g2_t *u8g2)
{
    u8g2_ClearBuffer(u8g2);

    u8g2_SetFont(u8g2, u8g2_font_ncenB12_tf);
    u8g2_DrawStr(u8g2, 2, 24, "ID:");
    u8g2_DrawStr(u8g2, 30, 24, "632407030121");

    u8g2_SetFont(u8g2, u8g2_font_6x12_tf);
    u8g2_DrawStr(u8g2, 2, 54, "Name:");
    draw_cn16(u8g2, 42, 38, 0x9648);   /* 陈 */
    draw_cn16(u8g2, 60, 38, 0x7ACB);   /* 立 */
    draw_cn16(u8g2, 78, 38, 0x5468);   /* 周 */

    u8g2_SendBuffer(u8g2);
    HAL_Delay(2500);
}

/* 左右滑动：中文长句从右往左滚过屏幕 */
void testScrollLeftRight(u8g2_t *u8g2)
{
    int total = mixed_width(CN_LONG_MSG);

    for (int x = 0; x < total + 128; x += 3)
    {
        u8g2_ClearBuffer(u8g2);
        draw_mixed(u8g2, 128 - x, 30, CN_LONG_MSG);
        u8g2_SendBuffer(u8g2);
        HAL_Delay(20);
    }
}

/* 上下滑动：三行中英混排文字向上滚 */
void testScrollUpDown(u8g2_t *u8g2)
{
    static const uint16_t line1[] = {
        'S', 'T', 'M', '3', '2', 'F', '1', '0', '3', 'C', '8', 'T', '6', 0x0000
    };
    static const uint16_t line2[] = {          /* 重庆交通大学 */
        0x91CD, 0x5E86, 0x4EA4, 0x901A, 0x5927, 0x5B66, 0x0000
    };
    static const uint16_t line3[] = {          /* 物联网205实训室 */
        0x7269, 0x8054, 0x7F51, '2', '0', '5', 0x5B9E, 0x8BAD, 0x5BA4, 0x0000
    };
    const uint16_t *lines[3] = { line1, line2, line3 };

    for (int off = 0; off <= 3 * 18; off += 2)
    {
        u8g2_ClearBuffer(u8g2);
        for (int i = 0; i < 3; i++)
        {
            int y = 14 + i * 18 - off;
            if (y > -16 && y < 64)
            {
                draw_mixed(u8g2, 4, y, lines[i]);
            }
        }
        u8g2_SendBuffer(u8g2);
        HAL_Delay(25);
    }
}

/* 头像图案（u8g2 基本图形拼出来）+ 动态效果：浮动、眨眼、星星闪烁 */
static void draw_avatar(u8g2_t *u8g2, u8g2_uint_t cx, u8g2_uint_t cy, uint8_t blink)
{
    u8g2_DrawCircle(u8g2, cx, cy, 20, U8G2_DRAW_ALL);            /* 脸 */
    u8g2_DrawCircle(u8g2, cx, cy - 22, 12,
                    U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT); /* 头发弧 */

    if (blink)
    {
        u8g2_DrawHLine(u8g2, cx - 11, cy - 5, 6);                /* 闭眼 */
        u8g2_DrawHLine(u8g2, cx + 5, cy - 5, 6);
    }
    else
    {
        u8g2_DrawDisc(u8g2, cx - 8, cy - 5, 3, U8G2_DRAW_ALL);   /* 眼睛 */
        u8g2_DrawDisc(u8g2, cx + 8, cy - 5, 3, U8G2_DRAW_ALL);
    }

    u8g2_DrawDisc(u8g2, cx, cy + 2, 1, U8G2_DRAW_ALL);           /* 鼻子 */
    u8g2_DrawCircle(u8g2, cx, cy + 6, 7,
                    U8G2_DRAW_LOWER_LEFT | U8G2_DRAW_LOWER_RIGHT); /* 嘴巴 */
}

void testAvatarAnimation(u8g2_t *u8g2)
{
    for (int t = 0; t < 260; t++)
    {
        int bob = 32 + (t % 40) / 20;        /* 上下浮动 0~1 像素，配合眨眼 */
        int blink = (t % 55) < 5;

        u8g2_ClearBuffer(u8g2);
        draw_avatar(u8g2, 64, (u8g2_uint_t)bob, (uint8_t)blink);

        if (t & 1)
        {
            u8g2_DrawLine(u8g2, 22, 26, 30, 26);
            u8g2_DrawLine(u8g2, 26, 22, 26, 30);
            u8g2_DrawLine(u8g2, 98, 38, 106, 38);
            u8g2_DrawLine(u8g2, 102, 34, 102, 42);
        }
        else
        {
            u8g2_DrawPixel(u8g2, 26, 26);
            u8g2_DrawPixel(u8g2, 102, 38);
        }

        u8g2_SetFont(u8g2, u8g2_font_6x12_tf);
        u8g2_DrawStr(u8g2, 6, 62, "avatar animation");

        u8g2_SendBuffer(u8g2);
        HAL_Delay(30);
    }
}

/* 依次播放全部画面，一轮约 30 秒 */
void u8g2DrawTest(u8g2_t *u8g2)
{
    draw(u8g2);                     /* 博客原版 logo */
    testDrawProcess(u8g2);
    testShowFont(u8g2);
    testDrawFrame(u8g2);
    testDrawRBox(u8g2);
    testDrawCircle(u8g2);
    testDrawFilledEllipse(u8g2);
    testShowIdName(u8g2);
    testScrollLeftRight(u8g2);
    testScrollUpDown(u8g2);
    testAvatarAnimation(u8g2);
}
