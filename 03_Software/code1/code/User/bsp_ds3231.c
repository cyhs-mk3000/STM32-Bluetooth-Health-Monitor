#include "bsp_ds3231.h"

struct calendar calendar;
AlarmClock AlarmClk[CLOCK_NUM];//闹钟
/****************************************************************/
uint8_t HEX2BCD(uint8_t val)    //B码转换为BCD码
{
    uint8_t i,j,k;
    i=val/10;
    j=val%10;		
    k=j+(i<<4);
    return k;
}
static uint8_t BCD2HEX(uint8_t val)    //BCD转换为HEX十进制数
{
    uint8_t temp;
    temp=val&0x0f;
    val>>=4;
//    val&=0x0f;
    val*=10;
    temp+=val;
   
    return temp;
}
 void DS3231_Write_Reg(uint8_t add,uint8_t dat)
{
	Soft_IIC_Write_Address(DS3231_ADDRESS,add,dat);
}
static uint8_t DS3231_Read_Reg(uint8_t add)
{
	return Soft_IIC_Read_Address(DS3231_ADDRESS,add,DS3231_ADDRESS+1);
}
void DS3231_INIT(uint8_t year,uint8_t month,uint8_t day,uint8_t week,uint8_t hour,uint8_t minute,uint8_t second)//初始化时间
{
		DS3231_Write_Reg(HEX2BCD(0),HEX2BCD(second));//秒
		DS3231_Write_Reg(HEX2BCD(1),HEX2BCD(minute));//分
		DS3231_Write_Reg(HEX2BCD(2),HEX2BCD(hour));//时
		DS3231_Write_Reg(HEX2BCD(3),HEX2BCD(week));//星期
		DS3231_Write_Reg(HEX2BCD(4),HEX2BCD(day));//日
		DS3231_Write_Reg(HEX2BCD(5),HEX2BCD(month));//月
		DS3231_Write_Reg(HEX2BCD(6),HEX2BCD(year));//年
}
void DS3231_READ_TIME()//读时分秒星期年月日
{
	
		calendar.second =BCD2HEX((DS3231_Read_Reg(HEX2BCD(0)))&0x7f);// 0111 1111
		calendar.minute =BCD2HEX((DS3231_Read_Reg(HEX2BCD(1)))&0x7f);
		calendar.hour  =BCD2HEX((DS3231_Read_Reg(HEX2BCD(2)))&0x7f);
		calendar.week  =BCD2HEX((DS3231_Read_Reg(HEX2BCD(3)))&0x7f);
		calendar.day   =BCD2HEX((DS3231_Read_Reg(HEX2BCD(4)))&0x7f);
		calendar.month =BCD2HEX((DS3231_Read_Reg(HEX2BCD(5)))&0x7f);
		calendar.year  =BCD2HEX((DS3231_Read_Reg(HEX2BCD(6)))&0x7f);


//		miao=BCD2HEX((Soft_IIC_Read_Address(DS3231_ADDRESS,HEX2BCD(0),DS3231_ADDRESS + 1))&0x7f);
//		fen =BCD2HEX((Soft_IIC_Read_Address(DS3231_ADDRESS,HEX2BCD(1),DS3231_ADDRESS + 1))&0x7f);
//		shi =BCD2HEX((Soft_IIC_Read_Address(DS3231_ADDRESS,HEX2BCD(2),DS3231_ADDRESS + 1))&0x7f);
//		xq  =BCD2HEX((Soft_IIC_Read_Address(DS3231_ADDRESS,HEX2BCD(3),DS3231_ADDRESS + 1))&0x7f);
//		ri  =BCD2HEX((Soft_IIC_Read_Address(DS3231_ADDRESS,HEX2BCD(4),DS3231_ADDRESS + 1))&0x7f);
//		yue =BCD2HEX((Soft_IIC_Read_Address(DS3231_ADDRESS,HEX2BCD(5),DS3231_ADDRESS + 1))&0x7f);
//		nian=BCD2HEX((Soft_IIC_Read_Address(DS3231_ADDRESS,HEX2BCD(6),DS3231_ADDRESS + 1))&0x7f);
}
uint8_t runnian()//闰年判断
{
	if((calendar.year%4==0&&calendar.year%100!=0)||(calendar.year%100==0&&calendar.year%400==0))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void AUTOWEEK() 
{
	int month_tmp = 0,year_tmp = 0,day_tmp = 0,week_tmp = 0;
//	DS3231_READ_TIME();
	month_tmp = calendar.month;
	year_tmp = calendar.year;
	day_tmp = calendar.day;
	if( month_tmp == 1 || month_tmp == 2 )  
    {
        month_tmp += 12;
        if( year_tmp> 0 )
            year_tmp--;
        else
            year_tmp = 4;
    }
	week_tmp = (1+(( day_tmp + 2*month_tmp + 3*(month_tmp+1)/5 + year_tmp + year_tmp/4 ) %7));
	Soft_IIC_Write_Address(DS3231_ADDRESS,HEX2BCD(3),HEX2BCD(week_tmp));
}

