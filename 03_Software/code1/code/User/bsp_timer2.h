#ifndef __BSP_GENERALTIME2_H
#define __BSP_GENERALTIME2_H


#include "stm32f10x.h"
/*
				CH1     CH2    CH3     CH4
TIM1:	  PA8     PA9    PA10    PA11	
TIM2:   PA0     PA1    PA2     PA3
TIM3:   PA6     PA7    PB0     PB1
TIM4:		PB6     PB7    PB8     PB9
CK_CLK=TIMxCLK/(PSC+1)---计一个数的频率
1/CK_CLK = 1/TIMxCLK/(PSC+1)-----计一个数的时间
1/CK_CLK * (ARR+1)----- 中断一次的时间ARR为重装值
1/CK_CLK * (ARR+1)*time-----计时总时间
72M/(PSC+1)  PSC == 71则 72M/(PSC+1) = 1M 分之1 = 1us 
计时2S
1/TIMxCLK/(PSC+1) * (ARR+1) = 2S
1/72M/(PSC+1) * (ARR+1) = 2S
1/72M/(7099+1)* (ARR+1) = 2S
					100us -- 10000(1S) 20000(2S)
*/
/**************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当需要哪个定时器的时候，只需要把下面的宏定义改成1即可

#define            GENERAL_TIM2                   TIM2
#define            GENERAL_TIM2_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM2_CLK               RCC_APB1Periph_TIM2
//#define            GENERAL_TIM2_Period            (10000-1)//71:10000~10ms 1000~1ms
//#define            GENERAL_TIM2_Prescaler         71
#define            GENERAL_TIM2_Period            (10000-1)//1S
#define            GENERAL_TIM2_Prescaler         7099

#define            GENERAL_TIM2_IRQ               TIM2_IRQn
#define            GENERAL_TIM2_IRQHandler        TIM2_IRQHandler

/**************************函数声明********************************/

void Timer2_Init(void);


#endif	/* __BSP_GENERALTIME_H */


