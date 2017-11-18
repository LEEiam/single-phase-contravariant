/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块。
*	文件名称 : main.c
*	版    本 : V2.0
*	说    明 : ARM官方DSP库移植
*	修改记录 : 本期实例主要是为了配合DSP教程的第6章而作。
*              实验目的：
*                1. 学习官方DSP库的移植
*              实验内容：
*                1. 按下按键K1, 串口打印函数arm_abs_f32的输出结果
*                2. 按下按键K2, 串口打印函数arm_abs_q31的输出结果
*                3. 按下按键K3, 串口打印函数arm_abs_q15的输出结果
*				 4. 建议使用
*                   SecureCRT（V5光盘里面有此软件）查看打印信息。
*              
*		版本号   日期        作者     说明
*		V1.0    2014-08-25  Eric2013  首发
*		V2.0    2015-04-24  Eric2013  1. 升级固件库到V1.5.0
*                                     2. 升级BSP板级支持包 
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp.h"		/* 底层硬件驱动 */
#include "arm_math.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"V5-906_ARM官方DSP库的移植"
#define EXAMPLE_DATE	"2015-04-24"
#define DEMO_VER		"2.0"

/* 仅允许本文件内调用的函数声明 */
static void PrintfLogo(void);

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{
    
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2

	bsp_Init();		/* 硬件初始化 */
	PrintfLogo();	/* 打印例程信息到串口1 */

	bsp_StartAutoTimer(0, 500);	/* 启动1个500ms的自动重装的定时器 */
    

	/* 进入主程序循环体 */
	while (1)
	{
		bsp_Idle();		/* 这个函数在bsp.c文件。用户可以修改这个函数实现CPU休眠和喂狗 */

		if (bsp_CheckTimer(0))	/* 判断定时器超时时间 */
		{
                bsp_LedToggle(2);
			
		}

		
	
	}
}

void TIM8_UP_TIM13_IRQHandler(void) 
{
    static uint16_t i=0;
    if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)//检查指定的TIM中断发生与否:TIM 中断源   
    {
        if(TIM_GetCounter(TIM8)>(4200/2))
        {
            TIM_ClearITPendingBit(TIM8, TIM_IT_Update);//清除TIMx的中断待处理位:TIM 中断源 
            TIM_SetCompare1(TIM8,(uint16_t)(4200.0/2+Open_Loop_Sine[i]*1.4));
            TIM_SetCompare2(TIM8,(uint16_t)(4200.0/2-Open_Loop_Sine[i]*1.4));
            if(++i>=400)
            {
                i=0;
            }
   
        }
    
       else
        {
            TIM_ClearITPendingBit(TIM8, TIM_IT_Update);//清除TIMx的中断待处理位:TIM 中断源 
        }
    }
    
   
}


/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	/* 检测CPU ID */
	{
		/* 参考手册：
			32.6.1 MCU device ID code
			33.1 Unique device ID register (96 bits)
		*/
		uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;

		CPU_Sn0 = *(__IO uint32_t*)(0x1FFF7A10);
		CPU_Sn1 = *(__IO uint32_t*)(0x1FFF7A10 + 4);
		CPU_Sn2 = *(__IO uint32_t*)(0x1FFF7A10 + 8);

		printf("\r\nCPU : STM32F407IGT6, LQFP176, UID = %08X %08X %08X\n\r", CPU_Sn2, CPU_Sn1, CPU_Sn0);
	}

	printf("\n\r");
	printf("*************************************************************\n\r");
	printf("* 例程名称   : %s\r\n", EXAMPLE_NAME);	/* 打印例程名称 */
	printf("* 例程版本   : %s\r\n", DEMO_VER);		/* 打印例程版本 */
	printf("* 发布日期   : %s\r\n", EXAMPLE_DATE);	/* 打印例程日期 */

	/* 打印ST固件库版本，这3个定义宏在stm32f10x.h文件中 */
	printf("* 固件库版本 : V%d.%d.%d (STM32F4xx_StdPeriph_Driver)\r\n", __STM32F4XX_STDPERIPH_VERSION_MAIN,
			__STM32F4XX_STDPERIPH_VERSION_SUB1,__STM32F4XX_STDPERIPH_VERSION_SUB2);
	printf("* \r\n");	/* 打印一行空格 */
	printf("* QQ    : 1295744630 \r\n");
	printf("* 旺旺  : armfly\r\n");
	printf("* Email : armfly@qq.com \r\n");
	printf("* 淘宝店: armfly.taobao.com\r\n");
	printf("* Copyright www.armfly.com 安富莱电子\r\n");
	printf("*************************************************************\n\r");
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
