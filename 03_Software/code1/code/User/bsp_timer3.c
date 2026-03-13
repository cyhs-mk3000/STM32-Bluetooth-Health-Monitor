#include "bsp_timer3.h" 
#if 0
// 中断优先级配置
static void Timer3_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM3_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */
static void Timer3_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
		// 开启定时器时钟,即内部时钟CK_INT=72M
    GENERAL_TIM3_APBxClock_FUN(GENERAL_TIM3_CLK, ENABLE);	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM3_Period;
	  // 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM3_Prescaler;	
		// 时钟分频因子 ，没用到不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
		// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		
		// 重复计数器的值，没用到不用管
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // 初始化定时器
    TIM_TimeBaseInit(GENERAL_TIM3, &TIM_TimeBaseStructure);
	
		// 清除计数器中断标志位
    TIM_ClearFlag(GENERAL_TIM3, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(GENERAL_TIM3,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(GENERAL_TIM3, ENABLE);
}

void Timer3_Init(void)
{
	Timer3_NVIC_Config();
	Timer3_Mode_Config();		
}
#endif
/*********************************************END OF FILE**********************/

/*-------------------------------------------------*/
/*函数名：定时器3使能30s定时                       */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM3_ENABLE_30S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //定义一个设置定时器的变量
	NVIC_InitTypeDef NVIC_InitStructure;                          //定义一个设置中断的变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);           //使能TIM3时钟	
	TIM_DeInit(TIM3);                                             //定时器3寄存器恢复默认值	
	TIM_TimeBaseInitStructure.TIM_Period = 60000-1; 	          //设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=36000-1;              //设置定时器预分频数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);            //设置TIM3
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);                    //清除溢出中断标志位
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                      //使能TIM3溢出中断    
	TIM_Cmd(TIM3,ENABLE);                                         //开TIM3                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;                 //设置TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;              //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断
}
/*-------------------------------------------------*/
/*函数名：定时器3使能2s定时                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM3_ENABLE_2S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //定义一个设置定时器的变量
	NVIC_InitTypeDef NVIC_InitStructure;                          //定义一个设置中断的变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);           //使能TIM3时钟
	TIM_DeInit(TIM3);                                             //定时器3寄存器恢复默认值	
	TIM_TimeBaseInitStructure.TIM_Period = 20000-1; 	          //设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=7200-1;               //设置定时器预分频数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);            //设置TIM3
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);                    //清除溢出中断标志位
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                      //使能TIM3溢出中断    
	TIM_Cmd(TIM3,ENABLE);                                         //开TIM3                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;                 //设置TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;       //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;              //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断
}


