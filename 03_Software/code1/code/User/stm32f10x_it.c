/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "sys.h"         //包含需要的头文件
#include "bsp_usart.h"        //包含需要的头文件
#include "bsp_usart2.h"        //包含需要的头文件
#include "bsp_wifi.h"	       //包含需要的头文件
#include "bsp_timer3.h"        //包含需要的头文件
uint8_t UART_BUF;
uint8_t UART2_BUF;
uint8_t UART3_BUF;
uint32_t W25QXX_WRITE_POINTER = 0;
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
char USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
unsigned int SystemTimer = 0;   //用于全局计时的变量           单位秒
uint16_t point1 = 0;

 
uint8_t  MQ_R_12[UART_BUF_LEN];//缓存数据
unsigned char RX_Count = 0;
uint8_t FINISH = 0;//完成标志位

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

// 串口中断服务函数
void DEBUG_USART_IRQHandler(void)
{

	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{		
		if((USART_ReceiveData(DEBUG_USARTx) != '\r') && (USART_ReceiveData(DEBUG_USARTx) != '\n') )
	  UART_BUF = USART_ReceiveData(DEBUG_USARTx);  
   
	}	 
}


/*-------------------------------------------------*/
/*函数名：串口2接收中断函数                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void USART2_IRQHandler(void)   
{                      
	if((USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)){       //如果USART_IT_RXNE标志置位，表示有数据到了，且当前已没有经连接上服务器 进入if分支
			if(USART2->DR){                                 								 								//处于指令配置状态时，非零值才保存到缓冲区	&&(Connect_flag==0)
			Usart2_RxBuff[Usart2_RxCounter]=USART2->DR;                                   //保存到缓冲区	
			Usart2_RxCounter ++;                                                          //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
		}		
	}	
} 

/*-------------------------------------------------*/
/*函数名：定时器3中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){   //如果TIM_IT_Update置位，表示TIM3溢出中断，进入if	
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);      //清除TIM3溢出中断标志 	
	}
}
/*-------------------------------------------------*/
/*函数名：定时器2中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM2_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){  //如果TIM_IT_Update置位，表示TIM2溢出中断，进入if	
		SystemTimer++;                                  //全局计时的变量+1,表示过去了1秒钟
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     //清除TIM2溢出中断标志 	
	}
}
/*-------------------------------------------------*/
/*函数名：定时器1中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM1_UP_IRQHandler(void)
{
	static uint16_t cnt = 0;

	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET){  //如果TIM_IT_Update置位，表示TIM1溢出中断，进入if	
		
			cnt++;	
			if(cnt == 5000)//500mS
				{
					cnt = 0;	
								
				}	
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);     //清除TIM1溢出中断标志 	
	}
}

/*-------------------------------------------------*/
/*函数名：定时器4中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){  //如果TIM_IT_Update置位，表示TIM4溢出中断，进入if	
	
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     //清除TIM4溢出中断标志 	
	
	}
}
//串口中断服务函数
void USART3_IRQHandler(void)
{

	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{		
	   
		UART3_BUF = USART_ReceiveData(USART3);  
	  	MQ_R_12[RX_Count] = UART3_BUF;
				RX_Count++;
				if(RX_Count > (UART_BUF_LEN - 1))
				{
					 RX_Count = 0;
  				 if(MQ_R_12[0] == 0xFF)
					 {
							FINISH = 1;
					 }
					 else
					 {
					   FINISH = 0;
						 memset(MQ_R_12,0,UART_BUF_LEN);
					 }
				}
				
				
	}	 
}





/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
	
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
