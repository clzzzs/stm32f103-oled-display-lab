#include "my_main.h"


#define STUDENT_ID      "632407030121"

static const uint16_t CN_NAME[] = { 0x9648, 0x7ACB, 0x5468, 0x0000 };  //陈立周

void setup(void)
{
    OLED_Init();                            

    OLED_ShowString(2, 1, "ID:");
    OLED_ShowString(2, 4, STUDENT_ID);     

    OLED_ShowString(3, 1, "Name:");         
    OLED_ShowCNString(3, 6, CN_NAME);     
}

void loop(void)
{
    
}
