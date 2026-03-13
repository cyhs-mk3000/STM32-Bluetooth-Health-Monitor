#include "bsp_usart2.h"




#if  USART2_RX_ENABLE                   //如果使能接收功能
char Usart2_RxCompleted = 0;            //定义一个变量 0：表示接收未完成 1：表示接收完成 
unsigned int Usart2_RxCounter = 0;      //定义一个变量，记录串口2总共接收了多少字节的数据
char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //定义一个数组，用于保存串口2接收到的数据   	
#endif

static void USART2_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}
void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	USART2_GPIO_APBxClkCmd(USART2_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	USART2_APBxClkCmd(USART2_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = USART2_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = USART2_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = USART2_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(USART2, &USART_InitStructure);

		
	// 串口中断优先级配置
	USART2_NVIC_Configuration();
	// 使能串口接收中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(USART2, ENABLE);	    
}

//#define  USART_IRQ                USART2_IRQn
//#define  USART_IRQHandler         USART2_IRQHandler

/*-------------------------------------------------*/
/*函数名：串口2 printf函数                         */
/*参  数：char* fmt,...  格式化输出字符串和参数    */
/*返回值：无                                       */
/*-------------------------------------------------*/

__align(8) char USART2_TxBuff[USART2_TXBUFF_SIZE];  

void u2_printf(char* fmt,...) 
{  
	unsigned int i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(USART2_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)USART2_TxBuff);		
	while((USART2->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		USART2->DR = USART2_TxBuff[i];
		while((USART2->SR&0X40)==0);	
	}	
}












