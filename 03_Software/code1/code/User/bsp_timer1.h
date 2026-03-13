#ifndef __BSP_ADVANCETIME1_H
#define __BSP_ADVANCETIME1_H

#include "stm32f10x.h"

/********************高级定时器TIM参数定义，只限TIM1、8************/
 

#define            ADVANCE_TIM                   TIM1
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM1
#define            ADVANCE_TIM_Period            (100-1)
#define            ADVANCE_TIM_Prescaler         71
#define            ADVANCE_TIM_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_UP_IRQHandler

 
/**************************函数声明********************************/

void Timer1_Init(void);


#endif	/* __BSP_GENERALTIME_H */


