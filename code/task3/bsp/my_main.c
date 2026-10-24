#include "my_main.h"
#include "stm32_u8g2.h"
#include "test.h"

/* u8g2 全局实例：所有绘制 API 的第一个参数都是它 */
u8g2_t u8g2;

void setup(void)
{
    /* TIM1 在 CubeMX 里只做了初始化，计数器还没启动；
       Tims_delay_us() 靠读 TIM1 计数器做微秒延时，所以必须先启动它 */
    HAL_TIM_Base_Start(&htim1);

    u8g2Init(&u8g2);
}

void loop(void)
{
    u8g2DrawTest(&u8g2);
}
