#ifndef __BSP_USART2_H
#define __BSP_USART2_H
#include "sys.h"		//包含需要的头文件 
#include "stm32f10x.h"
#include <stdio.h>
#include "bsp_usart.h"





#define USART2_RX_ENABLE     1      //是否开启接收功能  1：开启  0：关闭
#define USART2_TXBUFF_SIZE   512   //定义串口2 发送缓冲区大小 1024字节

#if  USART2_RX_ENABLE                          //如果使能接收功能
#define USART2_RXBUFF_SIZE   512              //定义串口2 接收缓冲区大小 1024字节
extern char Usart2_RxCompleted ;               //外部声明，其他文件可以调用该变量
extern unsigned int Usart2_RxCounter;          //外部声明，其他文件可以调用该变量
extern char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //外部声明，其他文件可以调用该变量
#endif

//串口2-USART2
//    USART2
#define  USART2_CLK                RCC_APB1Periph_USART2
#define  USART2_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  USART2_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  USART2_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  USART2_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  USART2_TX_GPIO_PORT       GPIOA  
#define  USART2_TX_GPIO_PIN        GPIO_Pin_2
#define  USART2_RX_GPIO_PORT       GPIOA
#define  USART2_RX_GPIO_PIN        GPIO_Pin_3

#define  USART2_IRQ                USART2_IRQn
#define  USART2_IRQHandler         USART2_IRQHandler

extern void USART2_Config(void);

#endif  /* __BSP_USART2_H */

