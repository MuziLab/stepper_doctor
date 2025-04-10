#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/PULSE/pulse.h"
#include "./BSP/UART/uart.h"
#include "./BSP/TMC2209/tmc2209.h"

void led_init(void);                       /* LED初始化函数声明 */
uint8_t rx_buff[4] = {0};
int rx_juedge = 0;

int main(void)
{

    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 延时初始化 */
    half_duplex_uart_init(115200);
    uart_init(115200);
    palse_init(5000 - 1, 72 - 1); /*计算方法，前一个参数是重装值，决定计数次数（周期），后一个是分频器，决定计数的时间*/
    uint8_t hello_message[2] = {'a', 'b'};
    uint8_t bye_message[2] = {'5', '5'};
    PALSE_TIMER_CHY_CLK_ENABLE();
    // uart_transmit(hello_message,2,100);
    while (1)
    {
        // uart_transmit(hello_message,2,100);
        tmc2209_read(0x00,0x00,rx_buff);

        //判断rxbuff是不是依旧是0
        for (size_t i = 0; i < 4; i++)
        {
            if (rx_buff[i] != 0)
            {
                rx_juedge = 1;
                break;
            }
            
        }
        
        if (rx_juedge == 0)
        {
            uart_transmit(bye_message,2,200);
        }
        
        
        uart_transmit(rx_buff,4,400);
        palse_set(133);
        delay_ms(2000);
    }
}
