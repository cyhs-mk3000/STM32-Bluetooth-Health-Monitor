#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_usart2.h"
#include "bsp_usart3.h"
#include "bsp_display.h"
#include "bsp_led.h"
#include "bsp_oled_iic.h"
#include "bsp_timer2.h" 
#include "bsp_timer4.h" 
#include "bsp_wifi.h"	
#include "bsp_adc.h"
#include "bsp_wifi.h"	    
#include "bsp_ds18b20.h"
#include "bsp_beep.h"   
#include "mpu6050.h"
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{


	USART_Config();//初始化串口
//	USART2_Config();//初始化串口2
	USART3_Config();//初始化串口3
	KEY_GPIO_Config();//初始化按键
	SOFT_IIC_GPIO_CFG();//初始化IIC管脚
  BEEP_GPIO_Config();
  OLED_Init();//OLED初始化 
	OLED_CLS();//清屏
	
	Display_Init();//初始化显示界面
	 
   DS18B20_Init();//初始化温度传感器
	DS18B20_ReadId ( ucDs18b20Id  ); // 读取 DS18B20 的序列号
	 


   printf("INIT OK\r\n");
	
  /* Infinite loop */
  while (1)
  {
		DISPLAY_Main();//显示函数	
  }
}
