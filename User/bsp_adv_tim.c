#include "bsp.h"

/*函数还没有对CR1->CEN位写1，所以还没有开始启动定时器计数*/
void bsp_Adv_Tim_OutPWM(uint8_t Channel,uint32_t Frequency,float Duty_cycle)
{
      
    GPIO_InitTypeDef  GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef   TIM_OCInitStructure;
    TIM_BDTRInitTypeDef         TIM_BDTRInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    uint32_t uiTIM1CLK=SystemCoreClock;   //TIM1时钟线为系统时钟168M
    uint16_t usPeriod;   //ARR的值
    usPeriod= uiTIM1CLK / Frequency -1;    
    
    
        /*初始化时钟*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC,ENABLE);//使能GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE); 
      
    
    
    if(Channel==1)  //定时器1
    {
        //初始化GPIO
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
        
        /*初始化GPIOE9，连接到TIM_1_CH1*/
        GPIO_Init(GPIOE, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1); 
        
        
        
        /*初始化GPIOE8，连接到TIM_1_CH1N*/
         GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
        GPIO_Init(GPIOE, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_TIM1); 
        
         /*初始化GPIOE11,连接到TIM_1_CH2*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
        GPIO_Init(GPIOE, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1); 
        
        /*初始化GPIOB0,连接到TIM_1_CH2N*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
        GPIO_Init(GPIOB, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM1); 
        
        
        
        /*初始化GPIOE13,连接到TIM_1_CH3*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
        GPIO_Init(GPIOE, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1); 
        
        /*初始化GPIOB0,连接到TIM_1_CH2N*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
        GPIO_Init(GPIOB, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM1); 
        
           /*-----------------------------------------------------------------------
		system_stm32f4xx.c 文件中 void SetSysClock(void) 函数对时钟的配置如下：

		HCLK = SYSCLK / 1     (AHB1Periph)
		PCLK2 = HCLK / 2      (APB2Periph)
		PCLK1 = HCLK / 4      (APB1Periph)

		因为APB1 prescaler != 1, 所以 APB1上的TIM1CLK = PCLK1 x 2 = SystemCoreClock / 2;
		因为APB2 prescaler != 1, 所以 APB2上的TIM1CLK = PCLK2 x 2 = SystemCoreClock;

		APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM6, TIM12, TIM13,TIM14
		APB2 定时器有 TIM1, TIM8 ,TIM9, TIM10, TIM11
        
        ----------------------------------------------------------------------- */
        
    /*配置定时器TIM1的基础参数*/
    TIM_TimeBaseStructure.TIM_Prescaler=0;//预分频系数 0
    TIM_TimeBaseStructure.TIM_Period=usPeriod;  // TIM->ARR  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1; //中心对齐模式1计数，递减时产生中断
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;         //重复溢出计数 对应 TIM->RCR
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //TIM 总线时钟分频
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
     /*配置PWM输出参数*/
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;  //输出PWM1 模式 CNT<CCR ,输出有效状态
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;  //高电平有效   CCER->CCxP=0
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;  //使能通道输出 CCER->CCxE=1
    
    TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;   //高电平有效，N通道须与对应通道同为高或者低才可以互补输出  CCER->CCxNP=0
    TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable; //使能N通道输出   CCER->CCxNE=1
    
     TIM_OCInitStructure.TIM_Pulse=Duty_cycle*(uiTIM1CLK/Frequency-1);
    //Duty_cycle*(uiTIM1CLK/Frequency-1), (uiTIM1CLK/_ulFreq-1)为ARR寄存器的值 
    //设定占空比_ulDutyCycle * ARR 的值写入CCRx 寄存器，通过CCRx 与 ARR 比较输出占空比
    
    TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;  //  TIM_CR2_OIS1  空闲时（MOE=0）通道输出状态     CR2->OISx=0
    TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCIdleState_Reset; //    TIM_CR2_OIS1N  空闲时（MOE=0）通道N输出状态   CR2->OISxN=0
    
    
    /*TIM1 OC1初始化*/
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);    //初始化TIM1 输出比较通道1
    TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);          //输出比较通道1相关预装载寄存器使能  CCME1->OC1PE=1
    
     /*TIM1 OC2初始化*/
      TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;  //输出PWM2 模式 CNT<CCR ,输出无效状态
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);    //初始化TIM1 输出比较通道1
    TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);          //输出比较通道1相关预装载寄存器使能  CCME1->OC1PE=1
    
    
    TIM_OC3Init(TIM1,&TIM_OCInitStructure);    //初始化TIM1 输出比较通道1
    TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);          //输出比较通道1相关预装载寄存器使能  CCME1->OC1PE=1
    
     TIM_ARRPreloadConfig(TIM1, ENABLE);    
    
    /*互补PWM死区与断路设置*/
    TIM_BDTRInitStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Disable;   //关闭自动使能输出，MOE只能由软件置1 BDTR->AOE=0
    TIM_BDTRInitStructure.TIM_Break=TIM_Break_Disable;                       //禁止短路输入  BDTE->BKE=0
    TIM_BDTRInitStructure.TIM_BreakPolarity=TIM_BreakPolarity_High;
    TIM_BDTRInitStructure.TIM_DeadTime=10;// DT*(1/168M)     死区时间       BDTR->DGT=TIM_DeadTime
    TIM_BDTRInitStructure.TIM_LOCKLevel=TIM_LOCKLevel_OFF;   // 关闭锁定
    TIM_BDTRInitStructure.TIM_OSSIState=TIM_OSSIState_Enable;   //空闲模式下的关闭状态选择写1
    TIM_BDTRInitStructure.TIM_OSSRState=TIM_OSSRState_Enable;   //运行模式下的关闭状态选择写1
    TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure); 
    
    TIM_CtrlPWMOutputs(TIM1, ENABLE);  //写入MOE=1;
    
     NVIC_InitStructure.NVIC_IRQChannel =TIM1_UP_TIM10_IRQn ;  //TIM1 比较中断  使用了中心对齐模式1计数，递减时产生中断
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1; //先占优先级 0 级 
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级 3 级 
     NVIC_Init(&NVIC_InitStructure);      
     TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);  
     
     TIM_CCPreloadControl(TIM1,ENABLE);  //CR2->CCPC=1
	/* 下面这句话对于TIM1和TIM8是必须的，对于TIM2-TIM6则不必要 */
    }
    
    if(Channel==8)
    {
        
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
        
         /*初始化GPIOC6，连接到TIM_8_CH1*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
        GPIO_Init(GPIOC, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8); 
        
        /*初始化GPIOA7，连接到TIM_8_CH1N*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
        GPIO_Init(GPIOA, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM8); 
        
        /*初始化GPIOC7，连接到TIM_8_CH2*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
        GPIO_Init(GPIOC, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOC,GPIO_PinSource7, GPIO_AF_TIM8); 
        
        /*初始化GPIOB14，连接到TIM_8_CH2N*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
        GPIO_Init(GPIOB, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOB,GPIO_PinSource14, GPIO_AF_TIM8); 
        
        /*初始化GPIOC8，连接到TIM_8_CHC*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
        GPIO_Init(GPIOC, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOC,GPIO_PinSource8, GPIO_AF_TIM8); 
        
        /*初始化GPIOB15，连接到TIM_8_CH3N*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
        GPIO_Init(GPIOB, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOB,GPIO_PinSource15, GPIO_AF_TIM8); 
        
         /*配置定时器TIM1的基础参数*/
        TIM_TimeBaseStructure.TIM_Prescaler=0;//预分频系数 0
        TIM_TimeBaseStructure.TIM_Period=usPeriod;  // TIM->ARR  
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1; //中心对齐模式1计数，递减时产生中断
        TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;         //重复溢出计数 对应 TIM->RCR
        TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //TIM 总线时钟分频
        TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);  
         
        /*-----------------------------------------------------------------------
		system_stm32f4xx.c 文件中 void SetSysClock(void) 函数对时钟的配置如下：

		HCLK = SYSCLK / 1     (AHB1Periph)
		PCLK2 = HCLK / 2      (APB2Periph)
		PCLK1 = HCLK / 4      (APB1Periph)

		因为APB1 prescaler != 1, 所以 APB1上的TIM1CLK = PCLK1 x 2 = SystemCoreClock / 2;
		因为APB2 prescaler != 1, 所以 APB2上的TIM1CLK = PCLK2 x 2 = SystemCoreClock;

		APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM6, TIM12, TIM13,TIM14
		APB2 定时器有 TIM1, TIM8 ,TIM9, TIM10, TIM11
        
        ----------------------------------------------------------------------- */
        
         
              /*配置PWM输出参数*/
        TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;  //输出PWM1 模式 CNT<CCR ,输出有效状态
        TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;  //高电平有效   CCER->CCxP=0
        TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;  //使能通道输出 CCER->CCxE=1
        
        TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;   //高电平有效，N通道须与对应通道同为高或者低才可以互补输出  CCER->CCxNP=0
        TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable; //使能N通道输出   CCER->CCxNE=1
        
         TIM_OCInitStructure.TIM_Pulse=Duty_cycle*(uiTIM1CLK/Frequency-1);
        //Duty_cycle*(uiTIM1CLK/Frequency-1), (uiTIM1CLK/_ulFreq-1)为ARR寄存器的值 
        //设定占空比_ulDutyCycle * ARR 的值写入CCRx 寄存器，通过CCRx 与 ARR 比较输出占空比
        
        TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;  //  TIM_CR2_OIS1  空闲时（MOE=0）通道输出状态     CR2->OISx=0
        TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCIdleState_Reset; //    TIM_CR2_OIS1N  空闲时（MOE=0）通道N输出状态   CR2->OISxN=0
        
         /*TIM8 OC1初始化*/
        TIM_OC1Init(TIM8, &TIM_OCInitStructure);    //初始化TIM8 输出比较通道1
        TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);          //输出比较通道1相关预装载寄存器使能  CCME1->OC1PE=1
        
         /*TIM8 OC2初始化*/
        TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;  //输出PWM2 模式 CNT<CCR ,输出无效状态
        TIM_OC2Init(TIM8, &TIM_OCInitStructure);         //初始化TIM8 输出比较通道1
        TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);          //输出比较通道1相关预装载寄存器使能  CCME1->OC1PE=1
        
         TIM_ARRPreloadConfig(TIM1, ENABLE);    
        
         /*互补PWM死区与断路设置*/
        TIM_BDTRInitStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Disable;   //关闭自动使能输出，MOE只能由软件置1 BDTR->AOE=0
        TIM_BDTRInitStructure.TIM_Break=TIM_Break_Disable;                       //禁止短路输入  BDTE->BKE=0
        TIM_BDTRInitStructure.TIM_BreakPolarity=TIM_BreakPolarity_High;
        TIM_BDTRInitStructure.TIM_DeadTime=10;// DT*(1/168M)     死区时间       BDTR->DGT=TIM_DeadTime
        TIM_BDTRInitStructure.TIM_LOCKLevel=TIM_LOCKLevel_OFF;   // 关闭锁定
        TIM_BDTRInitStructure.TIM_OSSIState=TIM_OSSIState_Enable;   //空闲模式下的关闭状态选择写1
        TIM_BDTRInitStructure.TIM_OSSRState=TIM_OSSRState_Enable;   //运行模式下的关闭状态选择写1
        TIM_BDTRConfig(TIM8,&TIM_BDTRInitStructure);    

        TIM_CtrlPWMOutputs(TIM8, ENABLE);  //写入MOE=1;
        
        NVIC_InitStructure.NVIC_IRQChannel=TIM8_UP_TIM13_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1; //先占优先级 0 级 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级 3 级 
        NVIC_Init(&NVIC_InitStructure);  
        TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE);
        
        
         TIM_CCPreloadControl(TIM8,ENABLE);  //CR2->CCPC=1
        /* 下面这句话对于TIM1和TIM8是必须的，对于TIM2-TIM6则不必要 */
        
    }
   
    
    
      

}