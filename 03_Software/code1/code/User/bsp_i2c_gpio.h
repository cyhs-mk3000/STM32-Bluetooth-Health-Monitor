#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#include <inttypes.h>


/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define GPIO_PORT_I2C	GPIOA		/* GPIO端口 */
#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define I2C_SCL_PIN		GPIO_Pin_12		/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN		GPIO_Pin_15			/* 连接到SDA数据线的GPIO */


/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 0	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
	#define I2C_SCL_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 0 */
	
	#define I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)	/* 读SDA口线状态 */
#else	/* 这个分支选择直接寄存器操作实现IO读写 */
    /*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
	#define I2C_SCL_1()  GPIO_PORT_I2C->BSRR = I2C_SCL_PIN				/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_PORT_I2C->BRR = I2C_SCL_PIN				/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_PORT_I2C->BSRR = I2C_SDA_PIN				/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_PORT_I2C->BRR = I2C_SDA_PIN				/* SDA = 0 */
	
	#define I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
#endif


extern  void Soft_IIC_Start(void);
extern  void Soft_IIC_Stop(void);
extern  void Soft_IIC_Write_Byte(uint8_t _ucByte);
extern  uint8_t Soft_IIC_Read_Byte(void);
extern  uint8_t Soft_IIC_Wait_Ack(void);
extern  void Soft_IIC_Ack(void);
extern  void Soft_IIC_Nack(void);

extern  void SOFT_IIC_GPIO_CFG(void);
extern  void Soft_IIC_Write_Address(uint8_t SlaveAddress,uint8_t add,uint8_t dat);
extern  uint8_t Soft_IIC_Read_Address(uint8_t SlaveAddress,uint8_t add,uint8_t r_add);


#endif

