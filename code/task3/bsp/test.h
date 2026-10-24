#ifndef __TEST_H
#define __TEST_H

#include "main.h"
#include "u8g2.h"

/* 博客 4.2 的测试函数 */
void testDrawProcess(u8g2_t *u8g2);          /* 进度条 */
void testShowFont(u8g2_t *u8g2);             /* 字体 */
void testDrawFrame(u8g2_t *u8g2);            /* 空心矩形 */
void testDrawRBox(u8g2_t *u8g2);             /* 实心圆角矩形 */
void testDrawCircle(u8g2_t *u8g2);           /* 圆 */
void testDrawFilledEllipse(u8g2_t *u8g2);    /* 实心椭圆 */

/* 任务三要求的画面 */
void testShowIdName(u8g2_t *u8g2);           /* 学号 + 姓名（16x16 汉字） */
void testScrollLeftRight(u8g2_t *u8g2);      /* 左右滑动 */
void testScrollUpDown(u8g2_t *u8g2);         /* 上下滑动 */
void testAvatarAnimation(u8g2_t *u8g2);      /* 图案（头像）+ 动态效果 */

void u8g2DrawTest(u8g2_t *u8g2);             /* 依次播放全部画面 */

#endif /* __TEST_H */
