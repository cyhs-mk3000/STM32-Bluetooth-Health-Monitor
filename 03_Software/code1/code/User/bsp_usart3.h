#ifndef __BSP_USART3_H
#define __BSP_USART3_H

#include "stm32f10x.h"
#include <stdio.h>
#include "bsp_usart.h"



//串口3-USART3
//    USART3
#define  USART3_CLK                RCC_APB1Periph_USART3
#define  USART3_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  USART3_BAUDRATE           38400

// USART GPIO 引脚宏定义
#define  USART3_GPIO_CLK           (RCC_APB2Periph_GPIOB)
#define  USART3_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  USART3_TX_GPIO_PORT       GPIOB   
#define  USART3_TX_GPIO_PIN        GPIO_Pin_10
#define  USART3_RX_GPIO_PORT       GPIOB
#define  USART3_RX_GPIO_PIN        GPIO_Pin_11

#define  USART3_IRQ                USART3_IRQn
#define  USART3_IRQHandler         USART3_IRQHandler

extern void USART3_Config(void);

#endif  /* __BSP_USART3_H */

