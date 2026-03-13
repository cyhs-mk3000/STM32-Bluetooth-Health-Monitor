#ifndef __BSP_ADC_H
#define	__BSP_ADC_H


#include "stm32f10x.h"
#define ADC0_CHANNEL 0
#define ADC1_CHANNEL 1
#define ADC2_CHANNEL 2
#define ADC3_CHANNEL 3
#define ADC4_CHANNEL 4
#define ADC5_CHANNEL 5
#define ADC6_CHANNEL 6
#define ADC7_CHANNEL 7
#define ADC8_CHANNEL 8
#define ADC9_CHANNEL 9


#define ADC0_CHANNEL_PIN GPIO_Pin_0
#define ADC1_CHANNEL_PIN GPIO_Pin_1
#define ADC2_CHANNEL_PIN GPIO_Pin_2
#define ADC3_CHANNEL_PIN GPIO_Pin_3
#define ADC4_CHANNEL_PIN GPIO_Pin_4
#define ADC5_CHANNEL_PIN GPIO_Pin_5
#define ADC6_CHANNEL_PIN GPIO_Pin_6
#define ADC7_CHANNEL_PIN GPIO_Pin_7
#define ADC8_CHANNEL_PIN GPIO_Pin_0
#define ADC9_CHANNEL_PIN GPIO_Pin_1

#define    LIGHT_ADC_GPIO_CLK                  RCC_APB2Periph_GPIOB
#define    LIGHT_ADC_PORT                      GPIOB
#define    LIGHT_ADC_PIN                       GPIO_Pin_0



// ADC GPIO宏定义
// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOB
#define    ADC_PORT                      GPIOB
#define    ADC_PIN                       GPIO_Pin_0


// ADC 编号选择
// 可以是 ADC1/2，如果使用ADC3，中断相关的要改成ADC3的
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_x                         ADC1
#define    ADC_CLK                       RCC_APB2Periph_ADC1



// ADC 中断相关宏定义
#define    ADC_IRQ                       ADC1_2_IRQn
#define    ADC_IRQHandler                ADC1_2_IRQHandler

void ADCx_Init(void);
extern uint16_t read_adc_value(uint8_t ch);
extern uint16_t Caculate(uint8_t ch);
#endif /* __BSP_ADC_H */


