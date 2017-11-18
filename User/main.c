/*
*********************************************************************************************************
*
*	ģ������ : ������ģ�顣
*	�ļ����� : main.c
*	��    �� : V2.0
*	˵    �� : ARM�ٷ�DSP����ֲ
*	�޸ļ�¼ : ����ʵ����Ҫ��Ϊ�����DSP�̵̳ĵ�6�¶�����
*              ʵ��Ŀ�ģ�
*                1. ѧϰ�ٷ�DSP�����ֲ
*              ʵ�����ݣ�
*                1. ���°���K1, ���ڴ�ӡ����arm_abs_f32��������
*                2. ���°���K2, ���ڴ�ӡ����arm_abs_q31��������
*                3. ���°���K3, ���ڴ�ӡ����arm_abs_q15��������
*				 4. ����ʹ��
*                   SecureCRT��V5���������д�������鿴��ӡ��Ϣ��
*              
*		�汾��   ����        ����     ˵��
*		V1.0    2014-08-25  Eric2013  �׷�
*		V2.0    2015-04-24  Eric2013  1. �����̼��⵽V1.5.0
*                                     2. ����BSP�弶֧�ְ� 
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp.h"		/* �ײ�Ӳ������ */
#include "arm_math.h"

/* ���������������̷������� */
#define EXAMPLE_NAME	"V5-906_ARM�ٷ�DSP�����ֲ"
#define EXAMPLE_DATE	"2015-04-24"
#define DEMO_VER		"2.0"

/* �������ļ��ڵ��õĺ������� */
static void PrintfLogo(void);

/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: c�������
*	��    �Σ���
*	�� �� ֵ: �������(���账��)
*********************************************************************************************************
*/
int main(void)
{
    
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2

	bsp_Init();		/* Ӳ����ʼ�� */
	PrintfLogo();	/* ��ӡ������Ϣ������1 */

	bsp_StartAutoTimer(0, 500);	/* ����1��500ms���Զ���װ�Ķ�ʱ�� */
    

	/* ����������ѭ���� */
	while (1)
	{
		bsp_Idle();		/* ���������bsp.c�ļ����û������޸��������ʵ��CPU���ߺ�ι�� */

		if (bsp_CheckTimer(0))	/* �ж϶�ʱ����ʱʱ�� */
		{
                bsp_LedToggle(2);
			
		}

		
	
	}
}

void TIM8_UP_TIM13_IRQHandler(void) 
{
    static uint16_t i=0;
    if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)//���ָ����TIM�жϷ������:TIM �ж�Դ   
    {
        if(TIM_GetCounter(TIM8)>(4200/2))
        {
            TIM_ClearITPendingBit(TIM8, TIM_IT_Update);//���TIMx���жϴ�����λ:TIM �ж�Դ 
            TIM_SetCompare1(TIM8,(uint16_t)(4200.0/2+Open_Loop_Sine[i]*1.4));
            TIM_SetCompare2(TIM8,(uint16_t)(4200.0/2-Open_Loop_Sine[i]*1.4));
            if(++i>=400)
            {
                i=0;
            }
   
        }
    
       else
        {
            TIM_ClearITPendingBit(TIM8, TIM_IT_Update);//���TIMx���жϴ�����λ:TIM �ж�Դ 
        }
    }
    
   
}


/*
*********************************************************************************************************
*	�� �� ��: PrintfLogo
*	����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	/* ���CPU ID */
	{
		/* �ο��ֲ᣺
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
	printf("* ��������   : %s\r\n", EXAMPLE_NAME);	/* ��ӡ�������� */
	printf("* ���̰汾   : %s\r\n", DEMO_VER);		/* ��ӡ���̰汾 */
	printf("* ��������   : %s\r\n", EXAMPLE_DATE);	/* ��ӡ�������� */

	/* ��ӡST�̼���汾����3���������stm32f10x.h�ļ��� */
	printf("* �̼���汾 : V%d.%d.%d (STM32F4xx_StdPeriph_Driver)\r\n", __STM32F4XX_STDPERIPH_VERSION_MAIN,
			__STM32F4XX_STDPERIPH_VERSION_SUB1,__STM32F4XX_STDPERIPH_VERSION_SUB2);
	printf("* \r\n");	/* ��ӡһ�пո� */
	printf("* QQ    : 1295744630 \r\n");
	printf("* ����  : armfly\r\n");
	printf("* Email : armfly@qq.com \r\n");
	printf("* �Ա���: armfly.taobao.com\r\n");
	printf("* Copyright www.armfly.com ����������\r\n");
	printf("*************************************************************\n\r");
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
