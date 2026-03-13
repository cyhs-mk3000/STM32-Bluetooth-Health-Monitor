#include "bsp_wifi.h"	    

char WiFi_Init_Status = 0;//wifi初始化状态
/*-------------------------------------------------*/
/*函数名：初始化WiFi的复位IO                       */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void WiFi_ResetIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	/*管脚初始化*/
	RCC_APB2PeriphClockCmd(WIFI_RST_GPIO_CLK, ENABLE);
	GPIO_InitStruct.GPIO_Pin = WIFI_RST_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed =   GPIO_Speed_50MHz;
	GPIO_Init(WIFI_RST_GPIO_PORT, &GPIO_InitStruct);
	RESET_IO(1);                                              //复位IO拉高电平
}
/*-------------------------------------------------*/
/*函数名：WiFi发送设置指令                         */
/*参  数：cmd：指令                                */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_SendCmd(char *cmd, int timeout)
{
	WiFi_RxCounter=0;                           //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);     //清空WiFi接收缓冲区 
	WiFi_printf("%s\r\n",cmd);                  //发送指令
	while(timeout--){                           //等待超时时间到0
		delay_ms(100);                          //延时100ms
		if(strstr(WiFi_RX_BUF,"OK"))            //如果接收到OK表示指令成功
			break;       						//主动跳出while循环
		printf("%d ",timeout);               //串口输出现在的超时时间
	}
	printf("\r\n");                          //串口输出信息
	if(timeout<=0)return 1;                     //如果timeout<=0，说明超时时间到了，也没能收到OK，返回1
	else return 0;		         				//反之，表示正确，说明收到OK，通过break主动跳出while
}
/*-------------------------------------------------*/
/*函数名：WiFi模块接收到的内容                     */
/*参  数：dat：接收到的内容                        */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_ReceiveDate(char *dat, int timeout)//WiFi模块接收到的内容 
{
	WiFi_RxCounter=0;                           //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);     //清空WiFi接收缓冲区 
	while(timeout--){                           //等待超时时间到0
		delay_ms(100);                          //延时100ms
		if(strstr(WiFi_RX_BUF,dat))            //如果接收到OK表示指令成功
			break;       						//主动跳出while循环
		printf("%d ",timeout);               //串口输出现在的超时时间
	}
	printf("\r\n");                          //串口输出信息
	if(timeout<=0)return 1;                     //如果timeout<=0，说明超时时间到了，也没能收到OK，返回1
	else return 0;		         				//反之，表示正确，说明收到OK，通过break主动跳出while
}
/*-------------------------------------------------*/
/*函数名：WiFi复位                                 */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_Reset(int timeout)
{
	RESET_IO(0);                                    //复位IO拉低电平
	delay_ms(500);                                  //延时500ms
	RESET_IO(1);                                    //复位IO拉高电平	
	while(timeout--){                               //等待超时时间到0
		delay_ms(100);                              //延时100ms
		if(strstr(WiFi_RX_BUF,"ready"))             //如果接收到ready表示复位成功
			break;       						    //主动跳出while循环
		printf("%d ",timeout);                   //串口输出现在的超时时间
	}
	printf("\r\n");                              //串口输出信息
	if(timeout<=0)return 1;                         //如果timeout<=0，说明超时时间到了，也没能收到ready，返回1
	else return 0;		         				    //反之，表示正确，说明收到ready，通过break主动跳出while
}

char WiFi_Connect_AP()//配置为AP模式
{
	while(WiFi_Init_Status != 9)
	{
		switch(WiFi_Init_Status)
		{
			case 0:
			{
					printf("准备复位模块\r\n");                     //串口提示数据
				if(WiFi_Reset(50)){                                //复位，100ms超时单位，总计5s超时时间
					printf("复位失败，准备重启\r\n");           //返回非0值，进入if，串口提示数据
					WiFi_Init_Status = 0;//重新复位WIFI模块流程
					
				}
				else
				{
					printf("复位成功\r\n");                   //串口提示数据
					WiFi_Init_Status++;//切换下一状态
				}
			}
			break;
			case 1:
			{
					printf("准备设置AP模式\r\n");                  //串口提示数据
					if(WiFi_SendCmd("AT+CWMODE=3",50)){                //设置STA模式，100ms超时单位，总计5s超时时间
						printf("设置AP模式失败，准备重启\r\n");    //返回非0值，进入if，串口提示数据
						WiFi_Init_Status = 1;//重新复位WIFI模块流程
				                         
					}
					else 
					{
						printf("设置STA模式成功\r\n");            //串口提示数据
						WiFi_Init_Status++;//切换下一状态
					}
			}
			break;
			case 2:
			{
					printf("准备取消自动连接\r\n");                 //串口提示数据
					if(WiFi_SendCmd("AT+CWAUTOCONN=0",50)){            //取消自动连接，100ms超时单位，总计5s超时时间
						printf("取消自动连接失败，准备重启\r\n");   //返回非0值，进入if，串口提示数据
							WiFi_Init_Status = 2;//重新复位WIFI模块流程
					                            //返回3
					}
					else 
					{
						printf("取消自动连接成功\r\n");           //串口提示数据
						WiFi_Init_Status++;//切换下一状态
					}
			}
			break;
			case 3:
			{
					printf("准备取消自动连接\r\n");                 //串口提示数据
					if(WiFi_SendCmd("AT+CWAUTOCONN=0",50)){            //取消自动连接，100ms超时单位，总计5s超时时间
						printf("取消自动连接失败，准备重启\r\n");   //返回非0值，进入if，串口提示数据
							WiFi_Init_Status = 3;//重新复位WIFI模块流程
					
					}
					else 
					{
						printf("取消自动连接成功\r\n");           //串口提示数据
						WiFi_Init_Status++;//切换下一状态
					}
			}
			break;
			case 4:
			{
				printf("准备设置WIFI名字密码\r\n");                 //串口提示数据
				if(WiFi_SendCmd("AT+CWSAP=\"CAR000\",\"123456789\",5,3",50)){  //设置WIFI名字密码，100ms超时单位，总计5s超时时间
					printf("设置WIFI名字密码失败\r\n");   //返回非0值，进入if，串口提示数据
						WiFi_Init_Status = 4;//重新复位WIFI模块流程
					                                 //返回3
				}
				else 
				{
					printf("设置WIFI名字密码成功\r\n");           //串口提示数据
					 WiFi_Init_Status++;//切换下一状态
				}
			}
			break;
			case 5:
			{
				printf("准备再次复位模块\r\n");                     //串口提示数据
				if(WiFi_Reset(50)){                                //复位，100ms超时单位，总计5s超时时间
					printf("复位失败，准备重启\r\n");           //返回非0值，进入if，串口提示数据
						WiFi_Init_Status = 5;//重新复位WIFI模块流程
				
				}
				else
				{				
					printf("复位成功\r\n");                   //串口提示数据
					 WiFi_Init_Status++;//切换下一状态
				}
			}
			break;
			case 6:
			{
				delay_ms(1000);
				printf("准备开启多路连接\r\n");                 //串口提示数据
				if(WiFi_SendCmd("AT+CIPMUX=1",50)){                //关闭多路连接，100ms超时单位，总计5s超时时间
					printf("开启多路连接失败，准备重启\r\n");   //返回非0值，进入if，串口提示数据
						WiFi_Init_Status = 6;//重新复位WIFI模块流程
					                                   //返回9
				}
				else 
				{
					printf("开启多路连接成功\r\n");           //串口提示数据
					 WiFi_Init_Status++;//切换下一状态
				}
			}
			break;
				case 7:
			{
					printf("准备设置端口\r\n");                 //串口提示数据
				if(WiFi_SendCmd("AT+CIPSERVER=1,8080",50)){                //关闭多路连接，100ms超时单位，总计5s超时时间
					printf("端口设置失败\r\n");   //返回非0值，进入if，串口提示数据
						WiFi_Init_Status = 7;//重新复位WIFI模块流程
				                                  //返回9
				}
				else 
				{
					printf("端口设置成功\r\n");           //串口提示数据
					 WiFi_Init_Status++;//切换下一状态
				}
			}
			break;
				case 8:
			{
					printf("关闭回显\r\n");                 //串口提示数据
				if(WiFi_SendCmd("ATE0",50)){                //关闭多路连接，100ms超时单位，总计5s超时时间
					printf("关闭回显失败\r\n");   //返回非0值，进入if，串口提示数据
						WiFi_Init_Status = 8;//重新复位WIFI模块流程
		                                     //返回9
				}
				else 
				{
					printf("关闭回显成功\r\n");           //串口提示数据
					 WiFi_Init_Status++;//切换下一状态
				}
			}
			break;
		}
	}
		return 0;//成功
}
void WiFiToApp(char *buffer,uint8_t len)//发送app
{	
	  char tmp[50];
		WiFi_RxCounter=0;                           //WiFi接收数据量变量清零                        
	  memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);     //清空WiFi接收缓冲区 
	  sprintf(tmp,"AT+CIPSEND=0,%d\r\n",len);
	  WiFi_printf(tmp);// AT+CIPSEND= <id>,<length> <id>:需要用于传输连接的id号<length>:数字参数，表明发送数据的长度，最大长度为2048 
//    WiFi_SendCmd("AT+CIPSEND=0,14\r\n",50);// AT+CIPSEND= <id>,<length> <id>:需要用于传输连接的id号<length>:数字参数，表明发送数据的长度，最大长度为2048 
	delay_ms(1);
    WiFi_printf(buffer);//发送数据
//	printf("%s-%s\r\n",tmp,buffer);
}
