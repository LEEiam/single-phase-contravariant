#ifndef __BSP_ADV_TIM_H
#define __BSP_ADV_TIM_H

#include "stm32f4xx.h"

void bsp_Adv_Tim_OutPWM(uint8_t Channel,uint32_t Frequency,float Duty_cycle);

#endif
