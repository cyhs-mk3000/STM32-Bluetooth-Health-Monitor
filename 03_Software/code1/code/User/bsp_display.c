#include "bsp_display.h"
unsigned int MSGTimer = 0;     //用于上传更新的计时的变量   单位秒


uint8_t keynum = 0;//按键值
uint8_t keycount = 0;
uint8_t set_flag = 0;//显示标志位
uint8_t flash = 0;
char daymax;
/**************************/

uint8_t OLED_DISPLAY_BUF[32];//OLED显示缓存

	
//检测参数设置
//温度

#define TEMPERATURE_MAX 99     //温度采集最大值
#define TEMPERATURE_MIN 0      //温度采集最小值

uint8_t TEMPERATURE_SET_MAX = 40; //温度设置最大值
uint8_t TEMPERATURE_SET_MIN = 20 ; //温度设置最小值

uint8_t temperature = 0;	   //温度


//心率

#define HEART_MAX 200     //心率采集最大值
#define HEART_MIN 0      //心率采集最小值

uint8_t HEART_SET_MAX = 100; //心率设置最大值
uint8_t HEART_SET_MIN = 0 ; //心率设置最小值

uint8_t heart = 0;	   //心率

//高压

#define HPRESS_MAX 200 	   //高压采集最大值
#define HPRESS_MIN 0		   //高压采集最小值
       
uint8_t HPRESS_SET_MAX = 150; //高压设置最大值
uint8_t HPRESS_SET_MIN = 0;  //高压设置最小值

uint8_t HPress = 0;		   //高压

//5.低压

#define LPRESS_MAX 255		   //低压采集最大值
#define LPRESS_MIN 0			   //低压采集最小值
      
uint8_t LPRESS_SET_MAX = 230;	   //低压设置最大值
uint8_t LPRESS_SET_MIN = 60	;	   //低压设置最小值

uint8_t LPress = 0;			   //低压



//血氧

#define SPO2_MAX 99     //血氧采集最大值
#define SPO2_MIN 0      //血氧采集最小值
uint8_t SPO2_SET_MAX = 40; //血氧设置最大值
uint8_t SPO2_SET_MIN = 80 ; //血氧设置最小值

uint8_t spo2 = 0;	   //血样


uint8_t ALARM_STATUS = 0;//异常标志位

 
uint8_t step = 0;//步数
void Display_Init()//OLED显示初始化
{
//  温(0) 度(1) 血(2) 压(3) 心(4) 率(5) 血(6) 氧(7)
	//    定(0) 时(1) 设(2) 置(3)
//	/************第1行显示日期和时间*************/	
  OLED_ShowStr(16,0,"2024-10-16-01",2);
	OLED_ShowStr(0,2,"00C 00:00:00 00%",2);
	
		/***********第2行显示血压*****************/
//	OLED_ShowCN(0,2,2);	
//	OLED_ShowCN(16,2,3);
	OLED_ShowStr(0,2,"000",2);//高压显示位置（左侧x=0）
	
//	OLED_ShowCN(64,2,0);	
//	OLED_ShowCN(80,2,1);
	OLED_ShowStr(104,2,"000",2);//低压显示位置（右侧x=104）
	
	/********************心率**************************/
	OLED_ShowCN(0,4,4);	// 
	OLED_ShowCN(16,4,5);	// 
	OLED_ShowStr(32,4,":000",2);//心率显示位置
	
	OLED_ShowCN(64,4,0);	
	OLED_ShowCN(80,4,1);
	OLED_ShowStr(96,4,":000",2);//温度显示位置
	
  /***********第3行血氧*********************************/
	OLED_ShowCN(0,6,6);	
	OLED_ShowCN(16,6,7);
	OLED_ShowStr(32,6,":000%",2);//血氧显示位置
}

void KEYSCAN()//按键扫描
{
			if((Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)))//按键1数据设置
			{
					delay_ms(100);//检测按键按下，延时100ms进行消抖
					if((Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)))
					{		 
						  keynum=1;		
					}
			}
			if((Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN)))//按键2
			{	
					delay_ms(100);
					if((Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN)) )
					{
						  keynum=2;
	         if(!set_flag)//正常状态下启动心率测量
						{
							 	Usart_SendByte(USART3,0x8A);
							delay_ms(500);//每次发送间隔500ms
								Usart_SendByte(USART3,0x8A);
								delay_ms(500);
							Usart_SendByte(USART3,0x8A);//发送三次相同指令确保可靠度
								memset(	MQ_R_12,0,UART_BUF_LEN);
						}
					}
			}
			if((Key_Scan(KEY3_GPIO_PORT,KEY3_GPIO_PIN)))//按键3
			{
				  delay_ms(100);
				 if((Key_Scan(KEY3_GPIO_PORT,KEY3_GPIO_PIN)))
					{			
							 keynum=3;
							 if(!set_flag)//正常状态下重置步数计数器
							step = 0;
					}								
			}
}


void KEYPROCESS()//按键处理
{
	 KEYSCAN();//按键扫描	
	if(keynum==1)//检测到按建1被按下，开始数值设置
	{
		keycount++;//切换到下一个设置项
		if(keycount == 11){keycount=0;	}//	display_all();Display_Status = 1;
		set_flag = keycount;
		//u1_printf("keycount = %d\r\n",(uint16_t)keycount);
		keynum=0;
	}
	
	 if(flash)  //数字闪烁即代表正在进行对应数值设置
	 {
		flash = 0;
	 }
	 else 
		  {
		flash = 1;
	 }

	// 	u1_printf("Key_Set_Time = %d,keycount  = %d,flash = %d\r\n",(uint16_t)Key_Set_Time,keycount,flash);
	switch(keycount)
	{
	 case 1://分钟调节
	 { 	 
			OLED_DISPLAY_BUF[0] = calendar.minute / 10 + 0x30;//十位转ASCII
		  OLED_DISPLAY_BUF[1] = calendar.minute % 10 + 0x30;//个位转ACSII
		  OLED_DISPLAY_BUF[2] = '\0';//字符串结束符
		  OLED_ShowStr(56,2,OLED_DISPLAY_BUF,2);	//显示数字
		  flash ? OLED_ShowStr(56,2,OLED_DISPLAY_BUF,2) : OLED_ShowStr(56,2,"  ",2);//显示空格，达到闪烁效果
			 
		 if(keynum==2) {keynum=0;   calendar.minute++;if(calendar.minute==60)calendar.minute=0;}//按键2增加
		 if(keynum==3) {keynum=0;   calendar.minute--;if(calendar.minute<=0)calendar.minute=59;}//按键3减少
		 DS3231_Write_Reg(HEX2BCD(1),HEX2BCD(calendar.minute));//写入时钟
	  
	 }  break;
	 case 2: //小时调节
	 {	 
			OLED_DISPLAY_BUF[0] = calendar.hour / 10 + 0x30;
		  OLED_DISPLAY_BUF[1] = calendar.hour % 10 + 0x30;
		  OLED_DISPLAY_BUF[2] = '\0';
		  OLED_ShowStr(32,2,OLED_DISPLAY_BUF,2);	
		  flash ? OLED_ShowStr(32,2,OLED_DISPLAY_BUF,2) : OLED_ShowStr(32,2,"  ",2);
			 
	   if(keynum==2) {keynum=0;   calendar.hour++;if(calendar.hour==24)calendar.hour=0;}
	   if(keynum==3) {keynum=0;   calendar.hour--;if(calendar.hour==24)calendar.hour=0;}
		  DS3231_Write_Reg(HEX2BCD(2),HEX2BCD(calendar.hour));
		
	 }break;	

	 case 3://日调节   
	 {	 
				OLED_DISPLAY_BUF[0] = calendar.day / 10 + 0x30;
				OLED_DISPLAY_BUF[1] = calendar.day % 10 + 0x30;
				OLED_DISPLAY_BUF[2] = '\0';
				OLED_ShowStr(80,0,OLED_DISPLAY_BUF,2);	
				flash ? OLED_ShowStr(80,0,OLED_DISPLAY_BUF,2) : OLED_ShowStr(80,0,"  ",2);
			 
			 if(runnian()){if(calendar.month==2){daymax=29;}}else{if(calendar.month==2){daymax=28;}} 
			 if((calendar.month==1)||(calendar.month==3)||(calendar.month==5)||(calendar.month==7)||(calendar.month==8)||(calendar.month==10)||(calendar.month==12)){daymax=31;}
			 if((calendar.month==4)||(calendar.month==6)||(calendar.month==9)||(calendar.month==11)){daymax=30;}
			 if(keynum==2) {keynum=0;   calendar.day++;if(calendar.day==daymax+1)calendar.day=1;}
			 if(keynum==3) {keynum=0;   calendar.day--;if(calendar.day<=0)calendar.day=daymax;}
			
				DS3231_Write_Reg(HEX2BCD(4),HEX2BCD(calendar.day));
		
	
		 
	 }break;	
	 case 4://月调节   
	 { 	 

					OLED_DISPLAY_BUF[0] = calendar.month / 10 + 0x30;
					OLED_DISPLAY_BUF[1] = calendar.month % 10 + 0x30;
					OLED_DISPLAY_BUF[2] = '\0';
					OLED_ShowStr(56,0,OLED_DISPLAY_BUF,2);	
					flash ? OLED_ShowStr(56,0,OLED_DISPLAY_BUF,2) : OLED_ShowStr(56,0,"  ",2);
				 
				 if(keynum==2) {keynum=0;   calendar.month++;if(calendar.month==13)calendar.month=1;}
				 if(keynum==3) {keynum=0;   calendar.month--;if(calendar.month<=0)calendar.month=12;}
				 DS3231_Write_Reg(HEX2BCD(5),HEX2BCD(calendar.month));
			
			
	 }break;	
	 case 5://年调节   
	 {		
				  OLED_DISPLAY_BUF[0] = calendar.year / 10 + 0x30;
					OLED_DISPLAY_BUF[1] = calendar.year % 10 + 0x30;
					OLED_DISPLAY_BUF[2] = '\0';
					OLED_ShowStr(32,0,OLED_DISPLAY_BUF,2);		
					flash ? OLED_ShowStr(32,0,OLED_DISPLAY_BUF,2) : OLED_ShowStr(32,0,"  ",2);
					
				 if(keynum==2) {keynum=0;   calendar.year++;if(calendar.year==100)calendar.year=0;}
				 if(keynum==3) {keynum=0;   calendar.year--;if(calendar.year<=0)calendar.year=99;}
				 DS3231_Write_Reg(HEX2BCD(6),HEX2BCD(calendar.year));
	
			
	 }break;
	   case 6://高压上限设置HPRESS_SET_MAX
	 {	
		
		  /**********************************/
			OLED_DISPLAY_BUF[0] = HPRESS_SET_MAX / 100 + 0x30;//
		  OLED_DISPLAY_BUF[1] = HPRESS_SET_MAX / 10 % 10 + 0x30;
		  OLED_DISPLAY_BUF[2] = HPRESS_SET_MAX % 10 + 0x30;
		  OLED_DISPLAY_BUF[3] = '\0';
		  flash ? OLED_ShowStr(0,2,(unsigned char*)OLED_DISPLAY_BUF,2) : OLED_ShowStr(0,2,"   ",2);
		  if(keynum==2) {keynum=0; HPRESS_SET_MAX++;if(HPRESS_SET_MAX>=HPRESS_MAX)HPRESS_SET_MAX=HPRESS_MAX;}
	    if(keynum==3) {keynum=0; HPRESS_SET_MAX--;if(HPRESS_SET_MAX==HPRESS_MIN)HPRESS_SET_MAX=HPRESS_MIN;}
		
	 }break;
	 case 7://低压下限设置LPRESS_SET_MIN
		{		

			/*******************************/
			OLED_DISPLAY_BUF[0] = LPRESS_SET_MIN / 100 + 0x30;//
			OLED_DISPLAY_BUF[1] = LPRESS_SET_MIN / 10 % 10 + 0x30;
			OLED_DISPLAY_BUF[2] = LPRESS_SET_MIN % 10 + 0x30;
			OLED_DISPLAY_BUF[3] = '\0';
			flash ? OLED_ShowStr(104,2,(unsigned char*)OLED_DISPLAY_BUF,2) : OLED_ShowStr(104,2,"   ",2);
			if(keynum==2) {keynum=0; LPRESS_SET_MIN++;if(LPRESS_SET_MIN>=LPRESS_MAX)LPRESS_SET_MIN=LPRESS_MAX;}
			if(keynum==3) {keynum=0; LPRESS_SET_MIN--;if(LPRESS_SET_MIN==LPRESS_MIN)LPRESS_SET_MIN=LPRESS_MIN;}

		}break;
	   case 8://心率设置HEART_SET_MAX
	 {		
		
		  /*******************************/
		  OLED_DISPLAY_BUF[0] = HEART_SET_MAX / 100 + 0x30;//
		  OLED_DISPLAY_BUF[1] = HEART_SET_MAX / 10 % 10 + 0x30;
		  OLED_DISPLAY_BUF[2] = HEART_SET_MAX % 10 + 0x30;
		  OLED_DISPLAY_BUF[3] = '\0';
		  flash ? OLED_ShowStr(40,4,(unsigned char*)OLED_DISPLAY_BUF,2) : OLED_ShowStr(40,4,"   ",2);
		  if(keynum==2) {keynum=0; HEART_SET_MAX++;if(HEART_SET_MAX>=HEART_MAX)HEART_SET_MAX=HEART_MAX;}
	    if(keynum==3) {keynum=0; HEART_SET_MAX--;if(HEART_SET_MAX==HEART_MIN)HEART_SET_MAX=HEART_MIN;}
	
	 }break;
	 		 case 9://温度上限
	 {	
			
		  /**********************************/
		  OLED_DISPLAY_BUF[0] = TEMPERATURE_SET_MAX / 100 + 0x30;//
		  OLED_DISPLAY_BUF[1] = TEMPERATURE_SET_MAX / 10 % 10 + 0x30;
		  OLED_DISPLAY_BUF[2] = TEMPERATURE_SET_MAX % 10 + 0x30;
		  OLED_DISPLAY_BUF[3] = '\0';
		  flash ? OLED_ShowStr(104,4,(unsigned char*)OLED_DISPLAY_BUF,2) : OLED_ShowStr(104,4,"   ",2);
		  if(keynum==2) {keynum=0; TEMPERATURE_SET_MAX++;if(TEMPERATURE_SET_MAX>=TEMPERATURE_MAX)TEMPERATURE_SET_MAX=TEMPERATURE_MAX;}
	    if(keynum==3) {keynum=0; TEMPERATURE_SET_MAX--;if(TEMPERATURE_SET_MAX==TEMPERATURE_MIN)TEMPERATURE_SET_MAX=TEMPERATURE_MIN;}
		
	 }break;
	
	 
	   case 10://血氧设置SPO2_SET_MIN
	 {		
		
		  /*******************************/
		  OLED_DISPLAY_BUF[0] = SPO2_SET_MIN / 100 + 0x30;//
		  OLED_DISPLAY_BUF[1] = SPO2_SET_MIN / 10 % 10 + 0x30;
		  OLED_DISPLAY_BUF[2] = SPO2_SET_MIN % 10 + 0x30;
		  OLED_DISPLAY_BUF[3] = '\0';
		  flash ? OLED_ShowStr(40,6,(unsigned char*)OLED_DISPLAY_BUF,2) : OLED_ShowStr(40,6,"   ",2);
		  if(keynum==2) {keynum=0; SPO2_SET_MIN++;if(SPO2_SET_MIN>=SPO2_MAX)SPO2_SET_MIN=SPO2_MAX;}
	    if(keynum==3) {keynum=0; SPO2_SET_MIN--;if(SPO2_SET_MIN==SPO2_MIN)SPO2_SET_MIN=SPO2_MIN;}
	
	 }break; 
	
 }
} 
void SET_DISPLAY()//设置界面显示
{
	
/************************温度*******************************/
	if(keycount == 9)
	{
			OLED_DISPLAY_BUF[0] = TEMPERATURE_SET_MAX / 100 + 0x30;//
			OLED_DISPLAY_BUF[1] = TEMPERATURE_SET_MAX / 10 % 10 + 0x30;
			OLED_DISPLAY_BUF[2] = TEMPERATURE_SET_MAX % 10 + 0x30;
			OLED_DISPLAY_BUF[3] = '\0';
			OLED_ShowStr(104,4,(unsigned char*)OLED_DISPLAY_BUF,2);				//显示温度  x,y : 起始点坐标(x:0~127, y:0~7);
	} else 	if(keycount == 6)
	{
		/************************HPRESS_SET_MAX*******************************/
		OLED_DISPLAY_BUF[0] = HPRESS_SET_MAX / 100 + 0x30;//
		OLED_DISPLAY_BUF[1] = HPRESS_SET_MAX / 10 % 10 + 0x30;
		OLED_DISPLAY_BUF[2] = HPRESS_SET_MAX % 10 + 0x30;
		OLED_DISPLAY_BUF[3] = '\0';
		OLED_ShowStr(0,2,(unsigned char*)OLED_DISPLAY_BUF,2);				//显示HPRESS_SET_MAX
	}else 	if(keycount == 7)
	{
	/*****************LPRESS_SET_MIN*****************************/			
		OLED_DISPLAY_BUF[0] = LPRESS_SET_MIN / 100 + 0x30;//
		OLED_DISPLAY_BUF[1] = LPRESS_SET_MIN / 10 % 10 + 0x30;
		OLED_DISPLAY_BUF[2] = LPRESS_SET_MIN % 10 + 0x30;
		OLED_DISPLAY_BUF[3] = '\0';
		OLED_ShowStr(104,2,(unsigned char*)OLED_DISPLAY_BUF,2) ;
		}
	else 	if(keycount == 8)
	{
/*****************HEART_SET_MAX*****************************/			
			
	OLED_DISPLAY_BUF[0] = HEART_SET_MAX / 100 + 0x30;
	OLED_DISPLAY_BUF[1] = HEART_SET_MAX / 10 % 10 + 0x30;
	OLED_DISPLAY_BUF[2] = HEART_SET_MAX % 10 + 0x30;
	OLED_DISPLAY_BUF[3] = '\0';
	OLED_ShowStr(40,4,(unsigned char*)OLED_DISPLAY_BUF,2);				//显示MQ7  x,y : 起始点坐标(x:0~127, y:0~7);
	}
		else 	if(keycount == 10)
	{
/*****************SPO2_SET_MIN*****************************/			
			
	OLED_DISPLAY_BUF[0] = SPO2_SET_MIN / 100 + 0x30;
	OLED_DISPLAY_BUF[1] = SPO2_SET_MIN / 10 % 10 + 0x30;
	OLED_DISPLAY_BUF[2] = SPO2_SET_MIN % 10 + 0x30;
	OLED_DISPLAY_BUF[3] = '\0';
	OLED_ShowStr(40,6,(unsigned char*)OLED_DISPLAY_BUF,2);				//显示MQ7  x,y : 起始点坐标(x:0~127, y:0~7);
	}
}

void Msg_ToApp()//发送数据到APP
{
	printf("20%d-%d-%d %d:%d:%d\r\n",calendar.year,calendar.month,calendar.day,calendar.hour,calendar.minute,calendar.second);
 	printf("H:%d,L:%d,heart:%d,tmp:%d:so2:%d\r\n",HPress,LPress,heart,temperature,spo2);
}
 


void BEEP_ALARM()//报警
{
	uint8_t i = 0;
	for(i = 0;i < 3;i++)
	{
		BEEP(1);//蜂鸣器报警
		delay_ms(5);
		BEEP(0);//蜂鸣器报警
		delay_ms(5);
	}
}

void ALARM()//异常检测
{
		 if((temperature  > TEMPERATURE_SET_MAX))//过高
			{
				ALARM_STATUS = 1;//体温过高
			}
	   if(HPress > HPRESS_SET_MAX)
			{
				ALARM_STATUS = 2;//高压过高
			}
			 if((LPress < LPRESS_SET_MIN) && (LPress != 0))
			{
				ALARM_STATUS = 3;//低压过低 (排除0值)
			}
		 if((heart  > HEART_SET_MAX))
			{
				ALARM_STATUS = 4;//心率过高
			}
				 if((spo2 < SPO2_SET_MIN) && (spo2 != 0))
			{
				ALARM_STATUS = 5;//血氧过低 (排除0值)  
			}
				if( (heart <= HEART_SET_MAX) && 
					((LPress >= LPRESS_SET_MIN) || (LPress == 0))  && 
				((spo2 >= SPO2_SET_MIN) || (spo2 == 0)) && 
				(HPress <= HPRESS_SET_MAX) && 
				(temperature <= TEMPERATURE_SET_MAX))// 报警清除条件
				{
					ALARM_STATUS = 0;
					BEEP(0);//关闭蜂鸣器
				}
			if(ALARM_STATUS != 0)
			{
				 BEEP_ALARM();//蜂鸣器报警
			}

}
	
void HEART_HLPRESS_RECEIVE()//解析健康数据
{
	if(FINISH)
	{
		FINISH = 0; // 清除完成标志
	
		heart  = MQ_R_12[65];//UART3_BUF;//MQ_R_12[65];//获取心率
		HPress = MQ_R_12[71];//获取高压
	  LPress = MQ_R_12[72];//获取低压
		spo2   = MQ_R_12[66];//获取血氧
			memset(	MQ_R_12,0,UART_BUF_LEN);// 清空接收缓冲区
		
	}
}	
 
void DISPLAY_TIME()//显示时间
{
		DS3231_READ_TIME();//读出日期
		AUTOWEEK();//计算星期
		/*****************显示年月日星期*********************/
		OLED_DISPLAY_BUF[0] = calendar.year / 10 + 0x30;
		OLED_DISPLAY_BUF[1] = calendar.year % 10 + 0x30;
		OLED_DISPLAY_BUF[2] = '\0';
		OLED_ShowStr(32,0,OLED_DISPLAY_BUF,2);		
		/*******************************************************/
		OLED_DISPLAY_BUF[0] = calendar.month / 10 + 0x30;
		OLED_DISPLAY_BUF[1] = calendar.month % 10 + 0x30;
		OLED_DISPLAY_BUF[2] = '\0';
		OLED_ShowStr(56,0,OLED_DISPLAY_BUF,2);	
		/******************************************************/		
		OLED_DISPLAY_BUF[0] = calendar.day / 10 + 0x30;
		OLED_DISPLAY_BUF[1] = calendar.day % 10 + 0x30;
		OLED_DISPLAY_BUF[2] = '\0';
		OLED_ShowStr(80,0,OLED_DISPLAY_BUF,2);		
		/******************************************************/		
		OLED_DISPLAY_BUF[0] = calendar.week / 10 + 0x30;
		OLED_DISPLAY_BUF[1] = calendar.week % 10 + 0x30;
		OLED_DISPLAY_BUF[2] = '\0';
		OLED_ShowStr(104,0,OLED_DISPLAY_BUF,2);		
		
		/*****************显示时分秒*********************/
		OLED_DISPLAY_BUF[0] = calendar.hour / 10 + 0x30;
		OLED_DISPLAY_BUF[1] = calendar.hour % 10 + 0x30;
		OLED_DISPLAY_BUF[2] = '\0';
		OLED_ShowStr(32,2,OLED_DISPLAY_BUF,2);		
		/*******************************************************/
		OLED_DISPLAY_BUF[0] = calendar.minute / 10 + 0x30;
		OLED_DISPLAY_BUF[1] = calendar.minute % 10 + 0x30;
		OLED_DISPLAY_BUF[2] = '\0';
		OLED_ShowStr(56,2,OLED_DISPLAY_BUF,2);	
		/******************************************************/		
		OLED_DISPLAY_BUF[0] = calendar.second / 10 + 0x30;
		OLED_DISPLAY_BUF[1] = calendar.second % 10 + 0x30;
		OLED_DISPLAY_BUF[2] = '\0';
		OLED_ShowStr(80,2,OLED_DISPLAY_BUF,2);
}

void DISPLAY_Main()//显示函数
{
	 	float tmpbuf = 0;
	    KEYPROCESS();//按键处理
	 
	   DISPLAY_TIME();
				MSGTimer++;
				if(MSGTimer > 20)
				{
					MSGTimer = 0;
		
					 Msg_ToApp(); //发送数据到APP//发送数据到APP
				}
   	  if(!set_flag)//正常状态下
			{
				   HEART_HLPRESS_RECEIVE();//解析高低压 
					  tmpbuf = (DS18B20_GetTemp_MatchRom ( ucDs18b20Id )) * 10;//获取温度
				    temperature = ((uint16_t)tmpbuf) / 10;
						/************************temperature*******************************/
			   	  OLED_DISPLAY_BUF[0] = temperature / 100  + 0x30;
						OLED_DISPLAY_BUF[1] = temperature / 10 % 10 + 0x30;
						OLED_DISPLAY_BUF[2] = temperature % 10 + 0x30;
						OLED_DISPLAY_BUF[3] = '\0';
						OLED_ShowStr(104,4,(unsigned char*)OLED_DISPLAY_BUF,2);	//显示温度
						/************************heart*******************************/
						OLED_DISPLAY_BUF[0] = heart / 100  + 0x30;
						OLED_DISPLAY_BUF[1] = heart / 10  % 10 + 0x30;
				    OLED_DISPLAY_BUF[2] = heart % 10 + 0x30;
						OLED_DISPLAY_BUF[3] = '\0';
						OLED_ShowStr(40,4,(unsigned char*)OLED_DISPLAY_BUF,2);//显示心率
				  	/************************HPress*******************************/
						OLED_DISPLAY_BUF[0] = HPress / 100  + 0x30;
						OLED_DISPLAY_BUF[1] = HPress / 10  % 10 + 0x30;
				    OLED_DISPLAY_BUF[2] = HPress % 10 + 0x30;
						OLED_DISPLAY_BUF[3] = '\0';
						OLED_ShowStr(0,2,(unsigned char*)OLED_DISPLAY_BUF,2);	//显示高压
							/************************LPress*******************************/
						OLED_DISPLAY_BUF[0] = LPress / 100  + 0x30;
						OLED_DISPLAY_BUF[1] = LPress / 10  % 10 + 0x30;
				    OLED_DISPLAY_BUF[2] = LPress % 10 + 0x30;
						OLED_DISPLAY_BUF[3] = '\0';
						OLED_ShowStr(104,2,(unsigned char*)OLED_DISPLAY_BUF,2);	//显示低压
						/************************LPress*******************************/
						OLED_DISPLAY_BUF[0] = spo2 / 100  + 0x30;
						OLED_DISPLAY_BUF[1] = spo2 / 10  % 10 + 0x30;
				    OLED_DISPLAY_BUF[2] = spo2 % 10 + 0x30;
						OLED_DISPLAY_BUF[3] = '\0';
						OLED_ShowStr(40,6,(unsigned char*)OLED_DISPLAY_BUF,2);//显示血氧
						ALARM();//异常检测
					}
					else
					{
						SET_DISPLAY();//设置界面显示
					}

}

