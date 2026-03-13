#ifndef __BSP_DISPLAY_H__
#define __BSP_DISPLAY_H__
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "sys.h"
#include "bsp_usart.h"
#include "bsp_key.h"
#include "bsp_adc.h"
#include "bsp_beep.h"   
#include "bsp_i2c_gpio.h"
#include "bsp_led.h"
#include "bsp_oled_iic.h" 
#include "bsp_usart2.h"
#include "bsp_timer2.h" 
#include "bsp_timer3.h" 
#include "bsp_wifi.h" 
#include "bsp_adc.h"
#include "bsp_ds18b20.h"
#include "mpu6050.h"
#include "bsp_ds3231.h"
extern void Display_Init(void);
extern void DISPLAY_Main(void);//ÏÔÊ¾º¯Êý
#endif

