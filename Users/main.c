#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/PULSE/pulse.h"
#include "./BSP/UART/uart.h"
#include "./BSP/TMC2209/tmc2209.h"

void led_init(void); /* LED��ʼ���������� */
uint8_t rx_buff[4] = {0};
int rx_juedge = 0;

uint8_t heightest_current[4] = {0x00,0x0f,0xff,0xff};//�������ݣ��Ĵ�����0x10
int main(void)
{

    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
		palse_init(5000 - 1, 72 - 1); /*���㷽����ǰһ����������װֵ�������������������ڣ�����һ���Ƿ�Ƶ��������������ʱ��*/
    full_duplex_uart_init(115200);
    //half_duplex_uart_init(115200);
    uart_init(115200);

    uint8_t hello_message[2] = {'a', 'b'};
    uint8_t bye_message[2] = {'5', '5'};
    uart_transmit(hello_message,2,1000);


    while (1)
    {   
        //tmc2209_full_write(0,0x10,heightest_current);
        //tmc2209_full_read(0,0, rx_buff);
        // tmc2209_full_write(0,0,dir_left_data);
        // palse_period_set_us(10000);
        // palse_times_set(250);
        // delay_ms(3000);
        // palse_period_set_us(3000);
        // tmc2209_full_write(0,0,dir_right_data);
        // palse_times_set(250);
        // delay_ms(3000);
        uart_transmit(hello_message,2,1000);
        delay_ms(2000);
        
    }
}
 
