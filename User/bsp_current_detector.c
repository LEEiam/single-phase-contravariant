#include "bsp.h" 


#define ADC2_DR_ADDRESS          ((uint32_t)0x4001214C)  //ADC2 DR寄存器硬件地址

uint16_t ADC2_DR_Register_Value[1];

void Bsp_Current_Detector_Init(uint8_t current_channel)
{
    ADC_InitTypeDef        ADC_InitStructure;         
	ADC_CommonInitTypeDef  ADC_CommonInitStructure;  //配置ADC CRR寄存器 结构体
    GPIO_InitTypeDef GPIO_InitStructure;
    DMA_InitTypeDef     DMA_InitStructure;
    /*5A量程电流检测*/
    if(current_channel==5)
    {
         RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使能GPIOA时钟
         RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);   //使能ADC2时钟
         RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);   //使能DMA2时钟
      
         ADC_DeInit();
        
        /*配置GPIOA1为模拟输入，对应ADC2_Channel_1*/
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOA, &GPIO_InitStructure); 
        
        
        /*DMA2连接到ADC2 DR寄存器*/
        DMA_InitStructure.DMA_Channel = DMA_Channel_1;
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC2_DR_ADDRESS;
        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC2_DR_Register_Value;
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  //外设到内存
        DMA_InitStructure.DMA_BufferSize = 1;
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址不递增
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  //内存地址不递增
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据大小 
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //内存地址数据大小
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             //循环模式
        DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
        DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
        DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
        DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
        DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
        
        DMA_Init(DMA2_Stream2, &DMA_InitStructure);
        DMA_Cmd(DMA2_Stream2, ENABLE);
        
         /* ADC Common Configuration --> CRR Register Configuration ----------------------------------------------------------*/
        ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     //独立模式
        ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;                  //预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
        ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     //应用在多重模式下
        ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //两个采样阶段之间的延迟5个时钟(三重模式或双重模式下使用)
        ADC_CommonInit(&ADC_CommonInitStructure);
        
        /*ADC2 Initialize Configuration  --> CR1 and CR2 Register Configuration*/
        ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
        ADC_InitStructure.ADC_ScanConvMode =DISABLE;                                 //使能扫描模式(多通道ADC采集要用扫描模式)
        ADC_InitStructure.ADC_ContinuousConvMode =ENABLE;                           //使能连续转换
        ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //触发源  使用软件触发
        ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;       //定时器事件2触发ADC
        ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      //右对齐
        ADC_InitStructure.ADC_NbrOfConversion = 1;                                  //规则序列通道数量 1
        ADC_Init(ADC2, &ADC_InitStructure);
        
        //ADC2 规则通道1 配置为等级1 15个采样周期
        ADC_RegularChannelConfig(ADC2,ADC_Channel_1,1,ADC_SampleTime_15Cycles);
        
         ADC_Cmd(ADC2, ENABLE);//使能ADC2，对ADC上电
         
         ADC_SoftwareStartConv(ADC2);//ADC2转换开始  ADC2_CR2_SWSTART写1
    }
}

float Single_Current_Convert_Value(uint8_t current_channel)
{
    float Current_Convert_Value,Votage_temp;
    Votage_temp=(float)ADC2_DR_Register_Value[0]*3.0/4095;
    Current_Convert_Value=(Votage_temp*2.0-2.5)/0.185;
    return Current_Convert_Value;
    
}

float Current_Convert_Value(uint8_t current_channel)
{
    
    uint32_t temp=0,i=10;
    float Current_Convert_Value,Votage_temp;
    if(current_channel==5)
    {
        while(i--)
        {    
            temp+=ADC_GetConversionValue(ADC2);  //获得10次ADC_DR寄存器的值
            1;
        }
       
        Votage_temp=(temp*1.0/10)*(3.0/4095);    //求temp平均值后转换出电压值

        Current_Convert_Value = ( Votage_temp*2 - 2.5) / 0.185; 

        return Current_Convert_Value;
    }
		if(current_channel==20)
		{
			
		}
}