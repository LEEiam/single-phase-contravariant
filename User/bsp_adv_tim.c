#include "bsp.h"

/*������û�ж�CR1->CENλд1�����Ի�û�п�ʼ������ʱ������*/
void bsp_Adv_Tim_OutPWM(uint8_t Channel,uint32_t Frequency,float Duty_cycle)
{
      
    GPIO_InitTypeDef  GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef   TIM_OCInitStructure;
    TIM_BDTRInitTypeDef         TIM_BDTRInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    uint32_t uiTIM1CLK=SystemCoreClock;   //TIM1ʱ����Ϊϵͳʱ��168M
    uint16_t usPeriod;   //ARR��ֵ
    usPeriod= uiTIM1CLK / Frequency -1;    
    
    
        /*��ʼ��ʱ��*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC,ENABLE);//ʹ��GPIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE); 
      
    
    
    if(Channel==1)  //��ʱ��1
    {
        //��ʼ��GPIO
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
        
        /*��ʼ��GPIOE9�����ӵ�TIM_1_CH1*/
        GPIO_Init(GPIOE, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1); 
        
        
        
        /*��ʼ��GPIOE8�����ӵ�TIM_1_CH1N*/
         GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
        GPIO_Init(GPIOE, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_TIM1); 
        
         /*��ʼ��GPIOE11,���ӵ�TIM_1_CH2*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
        GPIO_Init(GPIOE, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1); 
        
        /*��ʼ��GPIOB0,���ӵ�TIM_1_CH2N*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
        GPIO_Init(GPIOB, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM1); 
        
        
        
        /*��ʼ��GPIOE13,���ӵ�TIM_1_CH3*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
        GPIO_Init(GPIOE, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1); 
        
        /*��ʼ��GPIOB0,���ӵ�TIM_1_CH2N*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
        GPIO_Init(GPIOB, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM1); 
        
           /*-----------------------------------------------------------------------
		system_stm32f4xx.c �ļ��� void SetSysClock(void) ������ʱ�ӵ��������£�

		HCLK = SYSCLK / 1     (AHB1Periph)
		PCLK2 = HCLK / 2      (APB2Periph)
		PCLK1 = HCLK / 4      (APB1Periph)

		��ΪAPB1 prescaler != 1, ���� APB1�ϵ�TIM1CLK = PCLK1 x 2 = SystemCoreClock / 2;
		��ΪAPB2 prescaler != 1, ���� APB2�ϵ�TIM1CLK = PCLK2 x 2 = SystemCoreClock;

		APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM6, TIM12, TIM13,TIM14
		APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11
        
        ----------------------------------------------------------------------- */
        
    /*���ö�ʱ��TIM1�Ļ�������*/
    TIM_TimeBaseStructure.TIM_Prescaler=0;//Ԥ��Ƶϵ�� 0
    TIM_TimeBaseStructure.TIM_Period=usPeriod;  // TIM->ARR  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1; //���Ķ���ģʽ1�������ݼ�ʱ�����ж�
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;         //�ظ�������� ��Ӧ TIM->RCR
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //TIM ����ʱ�ӷ�Ƶ
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
     /*����PWM�������*/
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;  //���PWM1 ģʽ CNT<CCR ,�����Ч״̬
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;  //�ߵ�ƽ��Ч   CCER->CCxP=0
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;  //ʹ��ͨ����� CCER->CCxE=1
    
    TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;   //�ߵ�ƽ��Ч��Nͨ�������Ӧͨ��ͬΪ�߻��ߵͲſ��Ի������  CCER->CCxNP=0
    TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable; //ʹ��Nͨ�����   CCER->CCxNE=1
    
     TIM_OCInitStructure.TIM_Pulse=Duty_cycle*(uiTIM1CLK/Frequency-1);
    //Duty_cycle*(uiTIM1CLK/Frequency-1), (uiTIM1CLK/_ulFreq-1)ΪARR�Ĵ�����ֵ 
    //�趨ռ�ձ�_ulDutyCycle * ARR ��ֵд��CCRx �Ĵ�����ͨ��CCRx �� ARR �Ƚ����ռ�ձ�
    
    TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;  //  TIM_CR2_OIS1  ����ʱ��MOE=0��ͨ�����״̬     CR2->OISx=0
    TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCIdleState_Reset; //    TIM_CR2_OIS1N  ����ʱ��MOE=0��ͨ��N���״̬   CR2->OISxN=0
    
    
    /*TIM1 OC1��ʼ��*/
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);    //��ʼ��TIM1 ����Ƚ�ͨ��1
    TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);          //����Ƚ�ͨ��1���Ԥװ�ؼĴ���ʹ��  CCME1->OC1PE=1
    
     /*TIM1 OC2��ʼ��*/
      TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;  //���PWM2 ģʽ CNT<CCR ,�����Ч״̬
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);    //��ʼ��TIM1 ����Ƚ�ͨ��1
    TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);          //����Ƚ�ͨ��1���Ԥװ�ؼĴ���ʹ��  CCME1->OC1PE=1
    
    
    TIM_OC3Init(TIM1,&TIM_OCInitStructure);    //��ʼ��TIM1 ����Ƚ�ͨ��1
    TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);          //����Ƚ�ͨ��1���Ԥװ�ؼĴ���ʹ��  CCME1->OC1PE=1
    
     TIM_ARRPreloadConfig(TIM1, ENABLE);    
    
    /*����PWM�������·����*/
    TIM_BDTRInitStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Disable;   //�ر��Զ�ʹ�������MOEֻ���������1 BDTR->AOE=0
    TIM_BDTRInitStructure.TIM_Break=TIM_Break_Disable;                       //��ֹ��·����  BDTE->BKE=0
    TIM_BDTRInitStructure.TIM_BreakPolarity=TIM_BreakPolarity_High;
    TIM_BDTRInitStructure.TIM_DeadTime=10;// DT*(1/168M)     ����ʱ��       BDTR->DGT=TIM_DeadTime
    TIM_BDTRInitStructure.TIM_LOCKLevel=TIM_LOCKLevel_OFF;   // �ر�����
    TIM_BDTRInitStructure.TIM_OSSIState=TIM_OSSIState_Enable;   //����ģʽ�µĹر�״̬ѡ��д1
    TIM_BDTRInitStructure.TIM_OSSRState=TIM_OSSRState_Enable;   //����ģʽ�µĹر�״̬ѡ��д1
    TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure); 
    
    TIM_CtrlPWMOutputs(TIM1, ENABLE);  //д��MOE=1;
    
     NVIC_InitStructure.NVIC_IRQChannel =TIM1_UP_TIM10_IRQn ;  //TIM1 �Ƚ��ж�  ʹ�������Ķ���ģʽ1�������ݼ�ʱ�����ж�
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1; //��ռ���ȼ� 0 �� 
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ� 3 �� 
     NVIC_Init(&NVIC_InitStructure);      
     TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);  
     
     TIM_CCPreloadControl(TIM1,ENABLE);  //CR2->CCPC=1
	/* ������仰����TIM1��TIM8�Ǳ���ģ�����TIM2-TIM6�򲻱�Ҫ */
    }
    
    if(Channel==8)
    {
        
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
        
         /*��ʼ��GPIOC6�����ӵ�TIM_8_CH1*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
        GPIO_Init(GPIOC, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8); 
        
        /*��ʼ��GPIOA7�����ӵ�TIM_8_CH1N*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
        GPIO_Init(GPIOA, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM8); 
        
        /*��ʼ��GPIOC7�����ӵ�TIM_8_CH2*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
        GPIO_Init(GPIOC, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOC,GPIO_PinSource7, GPIO_AF_TIM8); 
        
        /*��ʼ��GPIOB14�����ӵ�TIM_8_CH2N*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
        GPIO_Init(GPIOB, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOB,GPIO_PinSource14, GPIO_AF_TIM8); 
        
        /*��ʼ��GPIOC8�����ӵ�TIM_8_CHC*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
        GPIO_Init(GPIOC, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOC,GPIO_PinSource8, GPIO_AF_TIM8); 
        
        /*��ʼ��GPIOB15�����ӵ�TIM_8_CH3N*/
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
        GPIO_Init(GPIOB, &GPIO_InitStructure); 
        GPIO_PinAFConfig(GPIOB,GPIO_PinSource15, GPIO_AF_TIM8); 
        
         /*���ö�ʱ��TIM1�Ļ�������*/
        TIM_TimeBaseStructure.TIM_Prescaler=0;//Ԥ��Ƶϵ�� 0
        TIM_TimeBaseStructure.TIM_Period=usPeriod;  // TIM->ARR  
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1; //���Ķ���ģʽ1�������ݼ�ʱ�����ж�
        TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;         //�ظ�������� ��Ӧ TIM->RCR
        TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //TIM ����ʱ�ӷ�Ƶ
        TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);  
         
        /*-----------------------------------------------------------------------
		system_stm32f4xx.c �ļ��� void SetSysClock(void) ������ʱ�ӵ��������£�

		HCLK = SYSCLK / 1     (AHB1Periph)
		PCLK2 = HCLK / 2      (APB2Periph)
		PCLK1 = HCLK / 4      (APB1Periph)

		��ΪAPB1 prescaler != 1, ���� APB1�ϵ�TIM1CLK = PCLK1 x 2 = SystemCoreClock / 2;
		��ΪAPB2 prescaler != 1, ���� APB2�ϵ�TIM1CLK = PCLK2 x 2 = SystemCoreClock;

		APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM6, TIM12, TIM13,TIM14
		APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11
        
        ----------------------------------------------------------------------- */
        
         
              /*����PWM�������*/
        TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;  //���PWM1 ģʽ CNT<CCR ,�����Ч״̬
        TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;  //�ߵ�ƽ��Ч   CCER->CCxP=0
        TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;  //ʹ��ͨ����� CCER->CCxE=1
        
        TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;   //�ߵ�ƽ��Ч��Nͨ�������Ӧͨ��ͬΪ�߻��ߵͲſ��Ի������  CCER->CCxNP=0
        TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable; //ʹ��Nͨ�����   CCER->CCxNE=1
        
         TIM_OCInitStructure.TIM_Pulse=Duty_cycle*(uiTIM1CLK/Frequency-1);
        //Duty_cycle*(uiTIM1CLK/Frequency-1), (uiTIM1CLK/_ulFreq-1)ΪARR�Ĵ�����ֵ 
        //�趨ռ�ձ�_ulDutyCycle * ARR ��ֵд��CCRx �Ĵ�����ͨ��CCRx �� ARR �Ƚ����ռ�ձ�
        
        TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;  //  TIM_CR2_OIS1  ����ʱ��MOE=0��ͨ�����״̬     CR2->OISx=0
        TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCIdleState_Reset; //    TIM_CR2_OIS1N  ����ʱ��MOE=0��ͨ��N���״̬   CR2->OISxN=0
        
         /*TIM8 OC1��ʼ��*/
        TIM_OC1Init(TIM8, &TIM_OCInitStructure);    //��ʼ��TIM8 ����Ƚ�ͨ��1
        TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);          //����Ƚ�ͨ��1���Ԥװ�ؼĴ���ʹ��  CCME1->OC1PE=1
        
         /*TIM8 OC2��ʼ��*/
        TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;  //���PWM2 ģʽ CNT<CCR ,�����Ч״̬
        TIM_OC2Init(TIM8, &TIM_OCInitStructure);         //��ʼ��TIM8 ����Ƚ�ͨ��1
        TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);          //����Ƚ�ͨ��1���Ԥװ�ؼĴ���ʹ��  CCME1->OC1PE=1
        
         TIM_ARRPreloadConfig(TIM1, ENABLE);    
        
         /*����PWM�������·����*/
        TIM_BDTRInitStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Disable;   //�ر��Զ�ʹ�������MOEֻ���������1 BDTR->AOE=0
        TIM_BDTRInitStructure.TIM_Break=TIM_Break_Disable;                       //��ֹ��·����  BDTE->BKE=0
        TIM_BDTRInitStructure.TIM_BreakPolarity=TIM_BreakPolarity_High;
        TIM_BDTRInitStructure.TIM_DeadTime=10;// DT*(1/168M)     ����ʱ��       BDTR->DGT=TIM_DeadTime
        TIM_BDTRInitStructure.TIM_LOCKLevel=TIM_LOCKLevel_OFF;   // �ر�����
        TIM_BDTRInitStructure.TIM_OSSIState=TIM_OSSIState_Enable;   //����ģʽ�µĹر�״̬ѡ��д1
        TIM_BDTRInitStructure.TIM_OSSRState=TIM_OSSRState_Enable;   //����ģʽ�µĹر�״̬ѡ��д1
        TIM_BDTRConfig(TIM8,&TIM_BDTRInitStructure);    

        TIM_CtrlPWMOutputs(TIM8, ENABLE);  //д��MOE=1;
        
        NVIC_InitStructure.NVIC_IRQChannel=TIM8_UP_TIM13_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1; //��ռ���ȼ� 0 �� 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ� 3 �� 
        NVIC_Init(&NVIC_InitStructure);  
        TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE);
        
        
         TIM_CCPreloadControl(TIM8,ENABLE);  //CR2->CCPC=1
        /* ������仰����TIM1��TIM8�Ǳ���ģ�����TIM2-TIM6�򲻱�Ҫ */
        
    }
   
    
    
      

}