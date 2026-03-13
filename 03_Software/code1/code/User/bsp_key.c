#include "bsp_key.h"

void KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
		RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK, ENABLE);
	GPIO_InitStruct.GPIO_Pin = KEY1_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//设置为输入上拉模式
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(KEY2_GPIO_CLK, ENABLE);
	GPIO_InitStruct.GPIO_Pin = KEY2_GPIO_PIN;
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(KEY3_GPIO_CLK, ENABLE);
	GPIO_InitStruct.GPIO_Pin = KEY3_GPIO_PIN;
	GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStruct);
	
	
	RCC_APB2PeriphClockCmd(KEY4_GPIO_CLK, ENABLE);
	GPIO_InitStruct.GPIO_Pin = KEY4_GPIO_PIN;
	GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStruct);
	
	
	RCC_APB2PeriphClockCmd(KEY5_GPIO_CLK, ENABLE);
	GPIO_InitStruct.GPIO_Pin = KEY5_GPIO_PIN;
	GPIO_Init(KEY5_GPIO_PORT, &GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(KEY6_GPIO_CLK, ENABLE);
	GPIO_InitStruct.GPIO_Pin = KEY6_GPIO_PIN;
	GPIO_Init(KEY6_GPIO_PORT, &GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(KEY7_GPIO_CLK, ENABLE);
	GPIO_InitStruct.GPIO_Pin = KEY7_GPIO_PIN;
	GPIO_Init(KEY7_GPIO_PORT, &GPIO_InitStruct);
	
	
		RCC_APB2PeriphClockCmd(HY_GPIO_CLK, ENABLE);
	GPIO_InitStruct.GPIO_Pin = HY_GPIO_PIN;
	GPIO_Init(HY_GPIO_PORT, &GPIO_InitStruct);
	

}

uint8_t Key_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
	uint8_t i =0;
	if( GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON )
	{
		i++;
		// 松手检测
		while( (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON ) && (i == 2));
		return 1;
	}
	else return 0;
}

