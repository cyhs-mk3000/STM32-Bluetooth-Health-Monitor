#ifndef __BSP_GENERALTIME3_H
#define __BSP_GENERALTIME3_H


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

#define            GENERAL_TIM3                   TIM3
#define            GENERAL_TIM3_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM3_CLK               RCC_APB1Periph_TIM3
#define            GENERAL_TIM3_Period            (10000-1)//10ms
#define            GENERAL_TIM3_Prescaler         71
#define            GENERAL_TIM3_IRQ               TIM3_IRQn
#define            GENERAL_TIM3_IRQHandler        TIM3_IRQHandler

/**************************函数声明********************************/

void Timer3_Init(void);
void TIM3_ENABLE_30S(void);
void TIM3_ENABLE_2S(void);

#endif	/* __BSP_GENERALTIME_H */


