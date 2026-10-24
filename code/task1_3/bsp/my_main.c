#include "my_main.h"
#include "scroll_text.h"


#define STUDENT_ID      "632407030121"

/* =====================================================================
 * 任务一(3)：长字符滑动显示
 * ---------------------------------------------------------------------
 * 两种滑动方式各演示一遍，循环播放：
 *   阶段 1：SSD1306 硬件水平滚动（0x26/0x27 + 0x2F）
 *           —— 驱动芯片自己平移显存内容，CPU 不参与；因为显存只有 128 列，
 *              只能滚"一屏宽"的短句（本例 120 像素：Hello，重庆交大）
 *   阶段 2：软件滚屏（取点阵窗口 + 批量写页）
 *           —— 长句 352 像素，超过一屏，只能由 CPU 每帧从长点阵图里
 *              取 128 像素宽的窗口写进显存，靠移动窗口实现滚动
 * ===================================================================== */

/* 从长句点阵图上取 128 像素宽的窗口，写到页 4、页 5（即第 3 行）
 * Offset：窗口左边界的像素坐标（0 ~ 352-128） */
static void OLED_DrawScrollLine(uint16_t Offset)
{
    uint8_t top[128], bottom[128];
    uint8_t s = (uint8_t)(Offset & 0x07);   /* 窗口起点在一个字节里的位偏移 */
    uint16_t i, k = (uint16_t)(Offset >> 3);/* 窗口起点对应的字节下标 */

    for (i = 0; i < 128; i++)
    {
        if (s == 0)
        {
            top[i]    = ScrollTextTop[k + i];
            bottom[i] = ScrollTextBottom[k + i];
        }
        else
        {
            /* 跨字节拼一个像素列：高字节右移 s 位 + 低字节左移 (8-s) 位 */
            top[i]    = (uint8_t)((ScrollTextTop[k + i] >> s) |
                                  ((uint16_t)ScrollTextTop[k + i + 1] << (8 - s)));
            bottom[i] = (uint8_t)((ScrollTextBottom[k + i] >> s) |
                                  ((uint16_t)ScrollTextBottom[k + i + 1] << (8 - s)));
        }
    }

    OLED_WritePageData(4, 0, top, 128);      /* 上半页 */
    OLED_WritePageData(5, 0, bottom, 128);   /* 下半页 */
}

void setup(void)
{
    OLED_Init();

}

void loop(void)
{
    uint8_t buf[128] = {0};
    uint16_t x;

    /* ---------- 阶段 1：硬件水平滚动 ---------- */
    memcpy(buf, HwTextTop, HW_TEXT_WIDTH);
    OLED_WritePageData(4, 0, buf, 128);
    memcpy(buf, HwTextBottom, HW_TEXT_WIDTH);
    OLED_WritePageData(5, 0, buf, 128);

    OLED_ScrollH_Start(4, 5, 1, 0x07);       /* 页 4~5，向右滚，2 帧/列 */
    HAL_Delay(5000);
    OLED_ScrollH_Stop();
    OLED_ScrollH_Start(4, 5, 0, 0x07);       /* 页 4~5，向左滚 */
    HAL_Delay(5000);
    OLED_ScrollH_Stop();
    HAL_Delay(300);

    /* ---------- 阶段 2：软件滚屏（长句往返） ---------- */
    for (x = 0; x <= SCROLL_TEXT_WIDTH - 128; x += 2)   /* 从左段滚到右段 */
    {
        OLED_DrawScrollLine(x);
        HAL_Delay(15);
    }
    for (x = SCROLL_TEXT_WIDTH - 128; x > 0; x -= 2)    /* 再滚回来 */
    {
        OLED_DrawScrollLine(x);
        HAL_Delay(15);
    }
    HAL_Delay(1000);
}
