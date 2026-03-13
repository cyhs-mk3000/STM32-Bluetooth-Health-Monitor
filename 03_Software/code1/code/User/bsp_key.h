#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#include "stm32f10x.h"


#define  KEY_ON      0
#define  KEY_OFF     1
//1按键
#define KEY1_GPIO_PORT GPIOB
#define KEY1_GPIO_CLK  RCC_APB2Periph_GPIOB
#define KEY1_GPIO_PIN GPIO_Pin_12
//2按键
#define KEY2_GPIO_PORT GPIOB
#define KEY2_GPIO_CLK  RCC_APB2Periph_GPIOB
#define KEY2_GPIO_PIN GPIO_Pin_13
//3按键
#define KEY3_GPIO_PORT GPIOB
#define KEY3_GPIO_CLK  RCC_APB2Periph_GPIOB
#define KEY3_GPIO_PIN GPIO_Pin_14
//4按键
#define KEY4_GPIO_PORT GPIOB
#define KEY4_GPIO_CLK  RCC_APB2Periph_GPIOB
#define KEY4_GPIO_PIN GPIO_Pin_15
//5按键
#define KEY5_GPIO_PORT GPIOA
#define KEY5_GPIO_CLK  RCC_APB2Periph_GPIOA
#define KEY5_GPIO_PIN GPIO_Pin_8
//6按键
#define KEY6_GPIO_PORT GPIOA
#define KEY6_GPIO_CLK  RCC_APB2Periph_GPIOA
#define KEY6_GPIO_PIN GPIO_Pin_8

//6按键
#define KEY7_GPIO_PORT GPIOA
#define KEY7_GPIO_CLK  RCC_APB2Periph_GPIOA
#define KEY7_GPIO_PIN GPIO_Pin_8

//6按键
#define HY_GPIO_PORT GPIOC
#define HY_GPIO_CLK  RCC_APB2Periph_GPIOC
#define HY_GPIO_PIN GPIO_Pin_15


#define  Get_Key1_Value()   GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN)
#define  Get_Key2_Value()   GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN)
#define  Get_Key3_Value()   GPIO_ReadInputDataBit(KEY3_GPIO_PORT, KEY3_GPIO_PIN)
#define  Get_Key4_Value()   GPIO_ReadInputDataBit(KEY4_GPIO_PORT, KEY4_GPIO_PIN)
#define  Get_Key5_Value()   GPIO_ReadInputDataBit(KEY5_GPIO_PORT, KEY5_GPIO_PIN)
#define  Get_Key6_Value()   GPIO_ReadInputDataBit(KEY6_GPIO_PORT, KEY6_GPIO_PIN)
#define  Get_Key7_Value()   GPIO_ReadInputDataBit(KEY7_GPIO_PORT, KEY7_GPIO_PIN)

#define  Get_HY_Value()   GPIO_ReadInputDataBit(HY_GPIO_PORT, HY_GPIO_PIN)


extern void KEY_GPIO_Config(void);
extern uint8_t Key_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
#endif
