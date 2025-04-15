#include "./SYSTEM/delay/delay.h"
#include "./BSP/PULSE/pulse.h"
#include "./BSP/UART/uart.h"
#include "./BSP/TMC2209/tmc2209.h"
#include "./main.h"

void SystemClock_Config(void);
uint8_t rx_buff[4] = {0};
int rx_juedge = 0;

uint8_t heightest_current[4] = {0x00,0x0f,0xff,0xff};//电流数据，寄存器是0x10
int main(void)
{

  HAL_Init();                         /* 初始化HAL库 */
  SystemClock_Config();               /* cube生成的时钟函数*/
 // sys_stm32_clock_init(RCC_PLL_MUL9); 
	delay_init(72);
	palse_init(5000 - 1, 72 - 1); /*计算方法，前一个参数是重装值，决定计数次数（周期），后一个是分频器，决定计数的时间*/
  palse_init_2(5000-1,71);
    //full_duplex_uart_init(115200);
  //uart_init(115200);
    //palse_init_2(5000 - 1, 72 - 1);
    
//    uint8_t hello_message[2] = {'a', 'b'};
//    uint8_t bye_message[2] = {'5', '5'};
   // uart_transmit(hello_message,2,1000);
    				
    
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


        //palse_period_set_us_2(10000);
        //palse_times_set_2(250);
      //  palse_period_set_us(10000);
      //  palse_times_set(250);
        palse_times_set(250);
        palse_times_set_2(250);
 //       uart_transmit(hello_message,2,1000);

        delay_ms(3000);
        
    }
}
 

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


 /**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
