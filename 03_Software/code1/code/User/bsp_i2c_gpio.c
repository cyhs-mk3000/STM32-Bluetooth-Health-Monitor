/**
  ******************************************************************************
  * @brief   用gpio模拟i2c总线, 适用于STM32系列CPU。该模块不包括应用层命令帧，仅包括I2C总线基本操作函数。
  ******************************************************************************
  */ 

/*
	应用说明：
	在访问I2C设备前，请先调用 i2c_CheckDevice() 检测I2C设备是否正常

*/
#include "bsp_i2c_gpio.h"
#include "stm32f10x.h"




/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	函 数 名: Soft_IIC_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Soft_IIC_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: Soft_IIC_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Soft_IIC_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}

/*
*********************************************************************************************************
*	函 数 名: Soft_IIC_Write_Byte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void Soft_IIC_Write_Byte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();	
		I2C_SCL_0();
		if (i == 7)
		{
			 I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	函 数 名: Soft_IIC_Read_Byte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t Soft_IIC_Read_Byte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: Soft_IIC_Wait_Ack
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
 uint8_t Soft_IIC_Wait_Ack(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPU释放SDA总线 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	i2c_Delay();
	return re;
}

/*
*********************************************************************************************************
*	函 数 名: Soft_IIC_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Soft_IIC_Ack(void)
{
	I2C_SDA_0();	/* CPU驱动SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: Soft_IIC_Nack
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Soft_IIC_Nack(void)
{
	I2C_SDA_1();	/* CPU驱动SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();	
}


 void Soft_IIC_Write_Address(uint8_t SlaveAddress,uint8_t add,uint8_t dat)
 {
 	Soft_IIC_Start();//起始信号
 	Soft_IIC_Write_Byte(SlaveAddress);//发送器件地址
 	Soft_IIC_Wait_Ack();//等待应答
 	Soft_IIC_Write_Byte(add);//发送寄存器地址
 	Soft_IIC_Wait_Ack();//等待应答
 	Soft_IIC_Write_Byte(dat);//发送数据
 	Soft_IIC_Wait_Ack();//应答
 	Soft_IIC_Stop();//结束通信
 }

 uint8_t Soft_IIC_Read_Address(uint8_t SlaveAddress,uint8_t add,uint8_t r_add)
 {
 	uint8_t bty;
 	Soft_IIC_Start();//起始信号
 	Soft_IIC_Write_Byte(SlaveAddress);//发送器件地址
 	Soft_IIC_Wait_Ack();//等待应答
 	Soft_IIC_Write_Byte(add);//发送寄存器地址
 	Soft_IIC_Wait_Ack();//等待应答

 	Soft_IIC_Start();//起始信号
 	Soft_IIC_Write_Byte(r_add);//发送器件地址
 	Soft_IIC_Wait_Ack();//等待应答
 	bty = Soft_IIC_Read_Byte();
 	Soft_IIC_Nack();
 	Soft_IIC_Stop();//结束通信
 	return bty;
 }


/*
*********************************************************************************************************
*	函 数 名: SOFT_IIC_GPIO_CFG
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SOFT_IIC_GPIO_CFG(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_I2C_PORT, ENABLE);	/* 打开GPIO时钟 */

	
	//PB3，PB4，PA13，PA14，PA15 要想当做普通IO口使用，就必须使用它复用功能中的重映射。因此就需要这样的两步操作：
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);//在时钟配置中打开复用时钟
	/*
	如果你用到所有的五个引脚当做普通IO口，那么上述步骤二中的重映射配置应写为GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); 
	如果你用PB3，PB4，PA15做普通IO，PA13&14用于SWD调试，则重映射配置应写为GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	同理可配置只用PB4可为普通IO口的情况。
	*/
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//用PB3，PB4，PA15做普通IO，PA13&14用于SWD调试
	
	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* 开漏输出 */
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	Soft_IIC_Stop();
}



