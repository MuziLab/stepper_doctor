#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/PULSE/pulse.h"

void led_init(void);                       /* LED��ʼ���������� */

int main(void)
{
    HAL_Init();                              /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);      /* ����ʱ��, 72Mhz */
    delay_init(72);                          /* ��ʱ��ʼ�� */

    palse_init(5000-1,72-1);                /*���㷽����ǰһ����������װֵ�������������������ڣ�����һ���Ƿ�Ƶ��������������ʱ��*/
    
    
    while(1)
    { 
        palse_set(133);
        delay_ms(1000);
    }
}
