# 实验3 OLED 显示实验（STM32F103C8T6 + 0.96 寸 I2C OLED / SSD1306）

> 平台：STM32F103C8T6 最小系统核心板（Blue Pill）+ 0.96 寸 128×64 I2C OLED（SSD1306）
> 环境：STM32CubeMX 6.16.1（STM32Cube FW_F1 V1.8.7）+ Keil MDK-ARM V5.32 + ST-Link V2 + PCtoLCD2002 取模软件

## 实验内容

| 部分 | 内容 |
|---|---|
| 任务一(1) | 软件 I2C（PB8=SCL、PB9=SDA）驱动 OLED，显示学号 `632407030121` 与姓名「陈立周」（16×16 取模汉字） |
| 任务一(3) | 长字符滑动：SSD1306 硬件水平滚动（`0x26/0x27 + 0x2F`）+ 软件滚屏显示 352 像素长句 |
| 任务二 | **未做**：无逻辑分析仪，示波器（RIGOL DS1202Z-E）操作未展开；简答题第 2 问按理论分析回答 |
| 任务三 | u8g2 移植到 HAL（硬件 I2C2 + TIM1 微秒延时）：demo、学号姓名、上下/左右滑动、头像动画 |

## 目录结构

```
实验3-OLED显示.md            报告正文（Markdown，34 张图，含 6 张动图）
code/                       三个 Keil 工程（task1_1 / task1_3 / task3）
images/                     报告插图（44 个文件，含同名 .mp4）
output/
  ├─ 实验3-OLED显示.pdf      报告 PDF（A4，42 页，5.0 MB）
  ├─ zip/                   四个代码压缩包（任务一(1)、任务一(3)、任务三、全部）
  ├─ report_style.css       PDF 排版样式
  └─ _preview.html          PDF 中间产物
交付说明.md                  提交清单
```

代码工程在 `code/`（同时保留在本地 `expr3/`）：

| 工程 | 说明 |
|---|---|
| `code\task1_1` | 任务一(1)：软件 I2C + 自写 OLED 驱动 + 汉字字库 |
| `code\task1_3` | 任务一(3)：硬件水平滚动 + 软件滚屏 |
| `code\task3`   | 任务三：u8g2 移植（硬件 I2C2 + TIM1） |

## 编译与运行

1. 用 Keil 打开对应工程 `MDK-ARM\*.uvprojx`，F7 编译、F8 用 ST-Link 下载；
2. 接线：
   - 任务一(1) / 任务一(3)：OLED `SCL → PB8`、`SDA → PB9`、`VCC → 3.3 V`、`GND → GND`；
   - 任务三：OLED `SCL → PB10`、`SDA → PB11`（I2C2），其余不变；
3. 任务三的 u8g2 库已裁剪到 `bsp\` 下（30 个文件），无需再下载官方源码即可编译。

## 主要结果

- 任务一(1)：屏上显示 `ID:632407030121` / `Name:陈立周`；汉字 21 个共 672 字节，固件 RO-data 增加约 720 字节。
- 任务一(3)：软件 I2C 约 200 kHz；软件滚屏每帧写 256 字节，约 36 帧/秒，长句单程约 3.1 s。
- 任务三：u8g2 裁剪后 Flash ≈ 35 KB、RAM ≈ 3.1 KB；一轮画面 8 个（logo → 进度条 → 字体 → 图形 → 学号姓名 → 左右滑动 → 上下滑动 → 头像动画）。

## 相关链接

- GitHub 仓库：[clzzzs/stm32f103-oled-display-lab](https://github.com/clzzzs/stm32f103-oled-display-lab)
- 博客（CSDN）：[实验3 OLED 显示实验](https://blog.csdn.net/2401_89763261/article/details/168976415)
