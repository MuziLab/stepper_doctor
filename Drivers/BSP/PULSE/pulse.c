
#include "./BSP/PULSE/pulse.h"
TIM_HandleTypeDef palse_timer_chy_handle; /* 定时器x句柄 */
TIM_HandleTypeDef palse_timer_chy_handle_2;
TIM_HandleTypeDef palse_timer_chy_handle_3;

/* g_npwm_remain表示当前还剩下多少个脉冲要发送
 * 每次最多发送256个脉冲
 */
static uint32_t g_npwm_remain = 0;
static uint32_t g_npwm_remain_2 = 0;
static uint32_t g_npwm_remain_3 = 0;

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *timHandle);

/**
 * @brief       高级定时器TIMX 通道Y 输出指定个数PWM 初始化函数
 * @note
 *              高级定时器的时钟来自APB2, 而PCLK2 = 72Mhz, 我们设置PPRE2不分频, 因此
 *              高级定时器时钟 = 72Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void palse_init(uint16_t arr, uint16_t psc)
{

    // TODO 高级定时器省去了一堆配置，看能用不
    TIM_OC_InitTypeDef timx_oc_npwm_chy; /* 定时器输出 */
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};

    palse_timer_chy_handle.Instance = PALSE_TIMER;                                 /* 定时器x */
    palse_timer_chy_handle.Init.Prescaler = psc;                                   /* 定时器分频 */
    palse_timer_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                  /* 递增计数模式 */
    palse_timer_chy_handle.Init.Period = arr;                                      /* 自动重装载值 */
    palse_timer_chy_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // TODO 这个和cube不同
    palse_timer_chy_handle.Init.RepetitionCounter = 0;                             /* 重复计数器初始值 */
                                                                                   // TODO 少了个division

    if (HAL_TIM_Base_Init(&palse_timer_chy_handle) != HAL_OK)
    {
        Error_Handler();
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&palse_timer_chy_handle, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Init(&palse_timer_chy_handle) != HAL_OK)
    {
        Error_Handler();
    }

    // 此处省去了主从和触发配置

    timx_oc_npwm_chy.OCMode = TIM_OCMODE_PWM1;         /* 模式选择PWM 1*/
    timx_oc_npwm_chy.Pulse = arr / 2;                  /* 设置比较值,此值用来确定占空比 */
                                                       /* 这里默认设置比较值为自动重装载值的一半,即占空比为50% */
    timx_oc_npwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH; /* 输出比较极性为高 */
    timx_oc_npwm_chy.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel(&palse_timer_chy_handle, &timx_oc_npwm_chy, PALSE_TIMER_CHY) != HAL_OK) /* 配置TIMx通道y */
    {
        Error_Handler();
    }
    HAL_TIM_MspPostInit(&palse_timer_chy_handle);
}

/**
 * @brief 多通道定时器，固定通道，固定50%占空比
 *
 */

void palse_init_2(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef timx_oc_npwm_chy; /* 定时器输出 */
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};

    palse_timer_chy_handle_2.Instance = PALSE_TIMER;                                 /* 定时器x */
    palse_timer_chy_handle_2.Init.Prescaler = psc;                                   /* 定时器分频 */
    palse_timer_chy_handle_2.Init.CounterMode = TIM_COUNTERMODE_UP;                  /* 递增计数模式 */
    palse_timer_chy_handle_2.Init.Period = arr;                                      /* 自动重装载值 */
    palse_timer_chy_handle_2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // TODO 这个和cube不同
    palse_timer_chy_handle_2.Init.RepetitionCounter = 0;                             /* 重复计数器初始值 */
                                                                                     // TODO 少了个division

    if (HAL_TIM_Base_Init(&palse_timer_chy_handle_2) != HAL_OK)
    {
        Error_Handler();
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&palse_timer_chy_handle_2, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Init(&palse_timer_chy_handle_2) != HAL_OK)
    {
        Error_Handler();
    }

    // 此处省去了主从和触发配置

    timx_oc_npwm_chy.OCMode = TIM_OCMODE_PWM1;         /* 模式选择PWM 1*/
    timx_oc_npwm_chy.Pulse = arr / 2;                  /* 设置比较值,此值用来确定占空比 */
                                                       /* 这里默认设置比较值为自动重装载值的一半,即占空比为50% */
    timx_oc_npwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH; /* 输出比较极性为高 */
    timx_oc_npwm_chy.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel(&palse_timer_chy_handle_2, &timx_oc_npwm_chy, PALSE_TIMER_CHY) != HAL_OK) /* 配置TIMx通道y */
    {
        Error_Handler();
    }
    HAL_TIM_MspPostInit(&palse_timer_chy_handle_2);
}

/**
 * @brief 多通道定时器，固定通道，固定50%占空比
 *
 */

 void palse_init_3(uint16_t arr, uint16_t psc)
 {
     TIM_OC_InitTypeDef timx_oc_npwm_chy; /* 定时器输出 */
     TIM_ClockConfigTypeDef sClockSourceConfig = {0};
 
     palse_timer_chy_handle_3.Instance = PALSE_TIMER;                                 /* 定时器x */
     palse_timer_chy_handle_3.Init.Prescaler = psc;                                   /* 定时器分频 */
     palse_timer_chy_handle_3.Init.CounterMode = TIM_COUNTERMODE_UP;                  /* 递增计数模式 */
     palse_timer_chy_handle_3.Init.Period = arr;                                      /* 自动重装载值 */
     palse_timer_chy_handle_3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // TODO 这个和cube不同
     palse_timer_chy_handle_3.Init.RepetitionCounter = 0;                             /* 重复计数器初始值 */
                                                                                      // TODO 少了个division
 
     if (HAL_TIM_Base_Init(&palse_timer_chy_handle_3) != HAL_OK)
     {
         Error_Handler();
     }
 
     sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
     if (HAL_TIM_ConfigClockSource(&palse_timer_chy_handle_3, &sClockSourceConfig) != HAL_OK)
     {
         Error_Handler();
     }
     if (HAL_TIM_PWM_Init(&palse_timer_chy_handle_3) != HAL_OK)
     {
         Error_Handler();
     }
 
     // 此处省去了主从和触发配置
 
     timx_oc_npwm_chy.OCMode = TIM_OCMODE_PWM1;         /* 模式选择PWM 1*/
     timx_oc_npwm_chy.Pulse = arr / 2;                  /* 设置比较值,此值用来确定占空比 */
                                                        /* 这里默认设置比较值为自动重装载值的一半,即占空比为50% */
     timx_oc_npwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH; /* 输出比较极性为高 */
     timx_oc_npwm_chy.OCFastMode = TIM_OCFAST_DISABLE;
 
     if (HAL_TIM_PWM_ConfigChannel(&palse_timer_chy_handle_3, &timx_oc_npwm_chy, PALSE_TIMER_CHY) != HAL_OK) /* 配置TIMx通道y */
     {
         Error_Handler();
     }
     HAL_TIM_MspPostInit(&palse_timer_chy_handle_3);
 }




/**
 * @brief       高级定时器TIMX NPWM设置PWM个数
 * @param       rcr: PWM的个数, 1~2^32次方个
 * @retval      无
 */
void palse_times_set(uint32_t npwm)
{
    if (npwm == 0)
        return;
    g_npwm_remain = npwm;                                                   /* 保存脉冲个数 */
    HAL_TIM_GenerateEvent(&palse_timer_chy_handle, TIM_EVENTSOURCE_UPDATE); /* 产生一次更新事件,在中断里面处理脉冲输出 */
    __HAL_TIM_ENABLE(&palse_timer_chy_handle);                              /* 使能定时器TIMX */
}

/**
 * @brief       第二个定时器的脉冲设置
 * @param       rcr: PWM的个数, 1~2^32次方个
 * @retval      无
 */
void palse_times_set_2(uint32_t npwm)
{
    if (npwm == 0)
        return;
    HAL_TIM_PWM_Start(&palse_timer_chy_handle_2, PALSE_TIMER_CHY_2);
    g_npwm_remain_2 = npwm;                                                   /* 保存脉冲个数 */
    HAL_TIM_GenerateEvent(&palse_timer_chy_handle_2, TIM_EVENTSOURCE_UPDATE); /* 产生一次更新事件,在中断里面处理脉冲输出 */
    __HAL_TIM_ENABLE(&palse_timer_chy_handle_2);                              /* 使能定时器TIMX */
}

/**
 * @attention 在1Mhz情况下才能使用
 */

void palse_period_set_us(uint32_t period_time_us)
{
    if (period_time_us <= 0)
    {
        return;
    }
    else
    {
        __HAL_TIM_SET_AUTORELOAD(&palse_timer_chy_handle, period_time_us - 1);
        HAL_TIM_GenerateEvent(&palse_timer_chy_handle, TIM_EVENTSOURCE_UPDATE);
        __HAL_TIM_ENABLE(&palse_timer_chy_handle);
    }
}

/**
 * @attention 在1Mhz情况下才能使用
 */

void palse_period_set_us_2(uint32_t period_time_us)
{
    if (period_time_us <= 0)
    {
        return;
    }
    else
    {
        __HAL_TIM_SET_AUTORELOAD(&palse_timer_chy_handle_2, period_time_us - 1);
        HAL_TIM_GenerateEvent(&palse_timer_chy_handle_2, TIM_EVENTSOURCE_UPDATE);
        __HAL_TIM_ENABLE(&palse_timer_chy_handle_2);
    }
}

void palse_stop(void)
{
    HAL_TIM_PWM_Stop(&palse_timer_chy_handle, PALSE_TIMER_CHY);
}

void palse_stop_2(void)
{
    HAL_TIM_PWM_Stop(&palse_timer_chy_handle_2, PALSE_TIMER_CHY_2);
}

// 统一开启nvic中断和时钟
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle)
{

    if (tim_baseHandle->Instance == PALSE_TIMER)
    {

        /* TIM1 clock enable */
        PALSE_TIMER_CHY_CLK_ENABLE();

        /* TIM1 interrupt Init */
        HAL_NVIC_SetPriority(PALSE_TIMER_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(PALSE_TIMER_IRQn);
    }
    else if (tim_baseHandle->Instance == PALSE_TIMER_2)
    {

        /* TIM2 clock enable */
        PALSE_TIMER_CHY_CLK_ENABLE_2();

        /* TIM2 interrupt Init */
        HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
    }
    else if (tim_baseHandle->Instance == TIM3)
    {

        /* TIM3 clock enable */
        PALSE_TIMER_CHY_CLK_ENABLE_3();

        /* TIM3 interrupt Init */
        HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }
}

// 统一进行gpio配置
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *timHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (timHandle->Instance == PALSE_TIMER)
    {

        PALSE_GPIO_CLK_ENABLE();
        /**TIM1 GPIO Configuration
        PE9     ------> TIM1_CH1
        */
        GPIO_InitStruct.Pin = PALSE_OUT_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(PALSE_OUT_GPIO_PORT, &GPIO_InitStruct);

        // TODO    __HAL_AFIO_REMAP_TIM1_ENABLE();这个应该没必要
    }
    else if (timHandle->Instance == PALSE_TIMER_2)
    {
        /* USER CODE BEGIN TIM2_MspPostInit 0 */

        /* USER CODE END TIM2_MspPostInit 0 */

        PALSE_GPIO_CLK_ENABLE_2();
        /**TIM2 GPIO Configuration
        PA0-WKUP     ------> TIM2_CH1
        */
        GPIO_InitStruct.Pin = PALSE_OUT_GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM2_MspPostInit 1 */

        /* USER CODE END TIM2_MspPostInit 1 */
    }
    else if (timHandle->Instance == PALSE_TIMER_3)
    {
        /* USER CODE BEGIN TIM3_MspPostInit 0 */

        /* USER CODE END TIM3_MspPostInit 0 */

        PALSE_GPIO_CLK_ENABLE_3();
        /**TIM3 GPIO Configuration
        PA6     ------> TIM3_CH1
        */
        GPIO_InitStruct.Pin = PALSE_OUT_GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM3_MspPostInit 1 */

        /* USER CODE END TIM3_MspPostInit 1 */
    }
}

//统一关闭中断和时钟
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==PALSE_TIMER)
  {
  /* USER CODE BEGIN TIM1_MspDeInit 0 */

  /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    PALSE_TIMER_CHY_CLK_DISABLE();

    /* TIM1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(PALSE_TIMER_IRQn);
  /* USER CODE BEGIN TIM1_MspDeInit 1 */

  /* USER CODE END TIM1_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==PALSE_TIMER_2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    PALSE_TIMER_CHY_CLK_DISABLE_2();

    /* TIM2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(PALSE_TIMER_IRQn_2);
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==PALSE_TIMER_3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    PALSE_TIMER_CHY_CLK_DISABLE_3();

    /* TIM3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(PALSE_TIMER_IRQn_3);
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
}

/**
 * @brief       高级定时器TIMX NPWM中断服务函数
 * @param       无
 * @retval      无
 */
void PALSE_TIMER_IRQHandler(void)
{
    uint16_t npwm = 0;

    /* 以下代码没有使用定时器HAL库共用处理函数来处理，而是直接通过判断中断标志位的方式 */
    if (__HAL_TIM_GET_FLAG(&palse_timer_chy_handle, TIM_FLAG_UPDATE) != RESET)
    {
        if (g_npwm_remain >= 256) /* 还有大于256个脉冲需要发送 */
        {
            g_npwm_remain = g_npwm_remain - 256;
            npwm = 256;
        }
        else if (g_npwm_remain % 256) /* 还有位数（不到256）个脉冲要发送 */
        {
            npwm = g_npwm_remain % 256;
            g_npwm_remain = 0; /* 没有脉冲了 */
        }

        if (npwm) /* 有脉冲要发送 */
        {
            PALSE_TIMER->RCR = npwm - 1;                                            /* 设置重复计数寄存器值为npwm-1, 即npwm个脉冲 */
            HAL_TIM_GenerateEvent(&palse_timer_chy_handle, TIM_EVENTSOURCE_UPDATE); /* 产生一次更新事件,在中断里面处理脉冲输出 */
            __HAL_TIM_ENABLE(&palse_timer_chy_handle);                              /* 使能定时器TIMX */
        }
        else
        {
            PALSE_TIMER->CR1 &= ~(1 << 0); /* 关闭定时器TIMX，使用HAL Disable会清除PWM通道信息，此处不用 */
        }

        __HAL_TIM_CLEAR_IT(&palse_timer_chy_handle, TIM_IT_UPDATE); /* 清除定时器溢出中断标志位 */
    }


}

void PALSE_TIMER_IRQHandler_2(void)
{   
    uint16_t npwm = 0;
    if (__HAL_TIM_GET_FLAG(&palse_timer_chy_handle_2, TIM_FLAG_UPDATE) != RESET)
    {
        if (g_npwm_remain_2 >= 256) /* 还有大于256个脉冲需要发送 */
        {
            g_npwm_remain_2 = g_npwm_remain_2 - 256;
            npwm = 256;
        }
        else if (g_npwm_remain_2 % 256) /* 还有位数（不到256）个脉冲要发送 */
        {
            npwm = g_npwm_remain_2 % 256;
            g_npwm_remain_2 = 0; /* 没有脉冲了 */
        }

        if (npwm) /* 有脉冲要发送 */
        {
            PALSE_TIMER->RCR = npwm - 1;                                              /* 设置重复计数寄存器值为npwm-1, 即npwm个脉冲 */
            HAL_TIM_GenerateEvent(&palse_timer_chy_handle_2, TIM_EVENTSOURCE_UPDATE); /* 产生一次更新事件,在中断里面处理脉冲输出 */
            __HAL_TIM_ENABLE(&palse_timer_chy_handle_2);                              /* 使能定时器TIMX */
        }
        else
        {
            PALSE_TIMER->CR1 &= ~(1 << 0); /* 关闭定时器TIMX，使用HAL Disable会清除PWM通道信息，此处不用 */
        }

        __HAL_TIM_CLEAR_IT(&palse_timer_chy_handle_2, TIM_IT_UPDATE); /* 清除定时器溢出中断标志位 */
    }
}

void PALSE_TIMER_IRQHandler_3(void)
{   
    uint16_t npwm = 0;
    if (__HAL_TIM_GET_FLAG(&palse_timer_chy_handle_3, TIM_FLAG_UPDATE) != RESET)
    {
        if (g_npwm_remain_3 >= 256) /* 还有大于256个脉冲需要发送 */
        {
            g_npwm_remain_3 = g_npwm_remain_3 - 256;
            npwm = 256;
        }
        else if (g_npwm_remain_3 % 256) /* 还有位数（不到256）个脉冲要发送 */
        {
            npwm = g_npwm_remain_3 % 256;
            g_npwm_remain_3 = 0; /* 没有脉冲了 */
        }

        if (npwm) /* 有脉冲要发送 */
        {
            PALSE_TIMER->RCR = npwm - 1;                                              /* 设置重复计数寄存器值为npwm-1, 即npwm个脉冲 */
            HAL_TIM_GenerateEvent(&palse_timer_chy_handle_3, TIM_EVENTSOURCE_UPDATE); /* 产生一次更新事件,在中断里面处理脉冲输出 */
            __HAL_TIM_ENABLE(&palse_timer_chy_handle_3);                              /* 使能定时器TIMX */
        }
        else
        {
            PALSE_TIMER->CR1 &= ~(1 << 0); /* 关闭定时器TIMX，使用HAL Disable会清除PWM通道信息，此处不用 */
        }

        __HAL_TIM_CLEAR_IT(&palse_timer_chy_handle_3, TIM_IT_UPDATE); /* 清除定时器溢出中断标志位 */
    }
}
