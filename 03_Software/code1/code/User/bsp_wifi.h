#ifndef __WIFI_H
#define __WIFI_H
#include "sys.h"	    //包含需要的头文件
#include "bsp_usart.h"	    //包含需要的头文件
#include "bsp_usart2.h"	    //包含需要的头文件
/*定义WIFI模块RST管脚*/
#define WIFI_RST_GPIO_PORT GPIOA
#define WIFI_RST_GPIO_CLK  RCC_APB2Periph_GPIOA
#define WIFI_RST_GPIO_PIN GPIO_Pin_4
#define RESET_IO(x)    if(x)\
												GPIO_SetBits(WIFI_RST_GPIO_PORT,WIFI_RST_GPIO_PIN);\
												else		\
												GPIO_ResetBits(WIFI_RST_GPIO_PORT,WIFI_RST_GPIO_PIN)
//#define WiFi_printf(x)       Usart_SendStr(USART2,x)           //串口2控制 WiFi	
#define WiFi_printf       u2_printf           //串口2控制 WiFi
#define WiFi_RxCounter    Usart2_RxCounter    //串口2控制 WiFi
#define WiFi_RX_BUF       Usart2_RxBuff       //串口2控制 WiFi
#define WiFi_RXBUFF_SIZE  USART2_RXBUFF_SIZE  //串口2控制 WiFi


extern void WiFi_ResetIO_Init(void);
extern char WiFi_SendCmd(char *cmd, int timeout);
extern char WiFi_Reset(int timeout);
extern char WiFi_Connect_AP(void);//配置为AP模式
extern void WiFiToApp(char *buffer,uint8_t len);//发送app
extern char WiFi_ReceiveDate(char *dat, int timeout);//WiFi模块接收到的内容 
#endif


