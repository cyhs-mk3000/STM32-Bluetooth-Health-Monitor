#ifndef _BSP_I2C_H
#define _BSP_I2C_H

#include <inttypes.h>
#include "bsp_usart.h"
#define I2C_WR	0		/* Ð´¿ØÖÆbit */
#define I2C_RD	1		/* ¶Á¿ØÖÆbit */

void MPU6050_IIC_START(void);
void MPU6050_IIC_STOP(void);
void MPU6050_IIC_SEND_BYTE(uint8_t _ucByte);
uint8_t MPU6050_IIC_READ_BYTE(uint8_t ack);
uint8_t MPU6050_IIC_WAIT_ACK(void);
void MPU6050_IIC_ACK(void);
void MPU6050_IIC_NACK(void);
uint8_t MPU6050_IIC_CHECKDEVICE(uint8_t _Address);
void MPU6050_IIC_GPIO_CFG(void);

#endif
