/*
    用于产生固定数量的脉冲以驱动步进电机
 */

 
 #include "./main.h"
 
 /******************************************************************************************/
 /* 高级定时器 定义 */
 
  /* TIMX 输出指定个数PWM 定义 
  * 这里输出的PWM通过PC6(TIM8_CH1)输出, 我们用杜邦线连接PC6和PE5, 然后在程序里面将PE5设置成浮空输入
  * 就可以 看到TIM8_CH1控制LED1(GREEN)的亮灭, 亮灭一次表示一个PWM波
  * 默认使用的是TIM8_CH1.
  * 注意: 通过修改这几个宏定义, 可以支持TIM1/TIM8定时器, 任意一个IO口输出指定个数的PWM
  */
 #define PALSE_OUT_GPIO_PORT                     GPIOE
 #define PALSE_OUT_GPIO_PIN                      GPIO_PIN_9
 #define PALSE_GPIO_CLK_ENABLE()                 do{  __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* 配置gpio口的宏*/
 #define PALSE_TIMER                          TIM1
 #define PALSE_TIMER_IRQn                     TIM1_UP_IRQn
 #define PALSE_TIMER_IRQHandler               TIM1_UP_IRQHandler
 #define PALSE_TIMER_CHY                      TIM_CHANNEL_1                           /* 通道 */
 #define PALSE_TIMER_CHY_CCRX                 TIM1->CCR1                            //这个好像没用
 #define PALSE_TIMER_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)  /* TIM时钟使能 */
 #define PALSE_TIMER_CHY_CLK_DISABLE()         do{ __HAL_RCC_TIM1_CLK_DISABLE(); }while(0)  /* TIM 时钟失能 */

 #define PALSE_OUT_GPIO_PORT_2                     GPIOA
 #define PALSE_OUT_GPIO_PIN_2                      GPIO_PIN_0
 #define PALSE_GPIO_CLK_ENABLE_2()                 do{  __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 配置gpio口的宏*/
 #define PALSE_TIMER_2                          TIM2
 #define PALSE_TIMER_IRQn_2                     TIM2_IRQn
 #define PALSE_TIMER_IRQHandler_2               TIM2_IRQHandler
 #define PALSE_TIMER_CHY_2                      TIM_CHANNEL_1                           /* 通道Y */
 #define PALSE_TIMER_CHY_CCRX_2                 TIM2->CCR1                            
 #define PALSE_TIMER_CHY_CLK_ENABLE_2()         do{ __HAL_RCC_TIM2_CLK_ENABLE(); }while(0)  /* TIM 时钟使能 */
 #define PALSE_TIMER_CHY_CLK_DISABLE_2()         do{ __HAL_RCC_TIM2_CLK_DISABLE(); }while(0)

 #define PALSE_OUT_GPIO_PORT_3                     GPIOA
 #define PALSE_OUT_GPIO_PIN_3                      GPIO_PIN_6
 #define PALSE_GPIO_CLK_ENABLE_3()                 do{  __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* 配置gpio口的宏*/
 #define PALSE_TIMER_3                          TIM3
 #define PALSE_TIMER_IRQn_3                     TIM3_IRQn
 #define PALSE_TIMER_IRQHandler_3               TIM3_IRQHandler
 #define PALSE_TIMER_CHY_3                      TIM_CHANNEL_1                           /* 通道Y */
 #define PALSE_TIMER_CHY_CCRX_3                 TIM3->CCR1                             /* 通道Y的输出比较寄存器,不同通道有对应寄存器 */
 #define PALSE_TIMER_CHY_CLK_ENABLE_3()         do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM8 时钟使能 */
 #define PALSE_TIMER_CHY_CLK_DISABLE_3()         do{ __HAL_RCC_TIM3_CLK_DISABLE(); }while(0)

 



 #define STEPPER_A_ROUND              200
 #define SBBDIVISION                  8
 #define PALSE_TIMES_ONE_ROUND         (200*8)
 #define PALSE_TIMES_ONE_MM             (PALSE_TIMES_ONE_ROUND/72)
 /******************************************************************************************/
 
 void palse_init(uint16_t arr, uint16_t psc);   /* 高级定时器 输出指定个数PWM初始化函数 */
 void palse_times_set(uint32_t npwm);                 /* 高级定时器 设置输出PWM的个数 */
 void palse_period_set_us(uint32_t period_time_us);
 void palse_stop(void);

 void palse_init_2(uint16_t arr, uint16_t psc);
 void palse_times_set_2(uint32_t npwm);
 void palse_period_set_us_2(uint32_t period_time_us);
 void palse_stop_2(void);
