#ifndef __BSP_GENERALTIME4_H
#define __BSP_GENERALTIME4_H


#include "stm32f10x.h"
/*
				CH1     CH2    CH3     CH4
TIM1:	  PA8     PA9    PA10    PA11	
TIM2:   PA0     PA1    PA2     PA3
TIM3:   PA6     PA7    PB0     PB1
TIM4:		PB6     PB7    PB8     PB9

*/
/**************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当需要哪个定时器的时候，只需要把下面的宏定义改成1即可

#define            GENERAL_TIM4                   TIM4
#define            GENERAL_TIM4_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM4_CLK               RCC_APB1Periph_TIM4
#define            GENERAL_TIM4_Period            (1000-1)//1ms
#define            GENERAL_TIM4_Prescaler         71
#define            GENERAL_TIM4_IRQ               TIM4_IRQn 
#define            GENERAL_TIM4_IRQHandler        TIM4_IRQHandler

/**************************函数声明********************************/

void Timer4_Init(void);


#endif	/* __BSP_GENERALTIME_H */


