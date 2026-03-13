#include "sys.h"
/*******************************************************************************
* 函 数 名         : delay_ms
* 函数功能           : 延时函数，延时ms
* 输    入         : i
* 输    出         : 无
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
*******************************************************************************/
void delay_ms(uint32_t count)
{
    u32 temp;
    SysTick->LOAD=9000*count;      //设置重装数值, 72MHZ时
    SysTick->CTRL=0X01;        //使能，减到零是无动作，采用外部时钟源
    SysTick->VAL=0;            //清零计数器
    do
    {
        temp=SysTick->CTRL;       //读取当前倒计数值
    }
    while((temp&0x01)&&(!(temp&(1<<16))));    //等待时间到达
    SysTick->CTRL=0;    //关闭计数器
    SysTick->VAL=0;        //清空计数器
}
//延时nus
//nus为要延时的us数.		    								   
void delay_us(uint32_t nus)
{		
	uint32_t temp;	static uint8_t  fac_us=0;		//us延时倍乘数	   
  fac_us=SystemCoreClock/8000000;				//为系统时钟的1/8	
	SysTick->LOAD=nus*fac_us; 					//时间加载	  		 
	SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器	 
}

