#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/PULSE/pulse.h"

void led_init(void);                       /* LED初始化函数声明 */

int main(void)
{
    HAL_Init();                              /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);      /* 设置时钟, 72Mhz */
    delay_init(72);                          /* 延时初始化 */

    palse_init(5000-1,72-1);                /*计算方法，前一个参数是重装值，决定计数次数（周期），后一个是分频器，决定计数的时间*/
    
    
    while(1)
    { 
        palse_set(133);
        delay_ms(1000);
    }
}
