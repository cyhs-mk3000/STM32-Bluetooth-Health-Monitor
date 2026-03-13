#include "bsp_adc.h"

static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 打开 ADC IO端口时钟
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );
	
	// 配置 ADC IO 引脚模式
	// 必须为模拟输入
	GPIO_InitStructure.GPIO_Pin = ADC8_CHANNEL_PIN | ADC9_CHANNEL_PIN;//初始化所有用到的管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// 初始化 ADC IO
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);	
	
	
//	// 打开 ADC IO端口时钟
//	ADC_GPIO_APBxClock_FUN ( LIGHT_ADC_GPIO_CLK, ENABLE );
//	
//	// 配置 ADC IO 引脚模式
//	// 必须为模拟输入
//	GPIO_InitStructure.GPIO_Pin = ADC0_CHANNEL_PIN;//初始化所有用到的管脚
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	
//	// 初始化 ADC IO
//	GPIO_Init(LIGHT_ADC_PORT, &GPIO_InitStructure);	
	
}

static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStruct;
	
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//独立模式：ADC1与ADC2独立
	ADC_InitStruct.ADC_ScanConvMode = DISABLE; //禁用通道扫描
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE; //禁用连续转换	
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//不使用外部触发
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; //数据位右对齐
	ADC_InitStruct.ADC_NbrOfChannel = 1; //转换通道数为1
	
	ADC_Init(ADC_x, &ADC_InitStruct); //初始化ADCx
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);//配置ADC时钟为PCLK2的8分频
	//设定指定的ADC通道组,设置转化顺序和采样时间
//	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC_x, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC_x, ADC_Channel_2, 1, ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC_x, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC_x, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
	
	ADC_ITConfig(ADC_x, ADC_IT_EOC, ENABLE);
	ADC_Cmd(ADC_x, ENABLE);//使能ADCx
	
  // ADC开始校准
	ADC_StartCalibration(ADC_x);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADC_x));
	//软件触发ADC转换
	//ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}

//static void ADC_NVIC_Config(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//	// 优先级分组
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

//  // 配置中断优先级
//  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQ;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//}
uint16_t read_adc_value(uint8_t ch)
{
    uint16_t adc_value = 0;
    
    ADC_RegularChannelConfig(ADC_x, ch, 1, ADC_SampleTime_7Cycles5 );   //设置ADC1通道ch的转换周期为7.5个采样周期，采样次序为1

    ADC_SoftwareStartConvCmd(ADC_x, ENABLE);//使能软件触发
    
    while(!ADC_GetFlagStatus(ADC_x, ADC_FLAG_EOC )){};//等待转换完成
        
    adc_value = ADC_GetConversionValue(ADC_x); //获取转换值
  
    return adc_value;
}
/*
ch:通道
*/
uint16_t Caculate(uint8_t ch)
{
	uint8_t i = 0;
	uint16_t ADCRES = 0;
	uint16_t Voltage = 0;
	for(i = 0;i < 16;i++)
	{
			ADCRES = read_adc_value(ch);//获得AD值
		  Voltage += ADCRES;
	}
		Voltage >>= 4;
		//Voltage = (Voltage / (1 << Resolution) * 1.0) * 330;//4.52V
		//Voltage = (Voltage / 4096.0) * 330;//4.52V
	  Voltage >>= 4;
	return 	Voltage;
}
void ADCx_Init(void)
{
	//ADC_NVIC_Config();
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}



















