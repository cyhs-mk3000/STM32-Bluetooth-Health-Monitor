#ifndef _BSP_DS3231_H_
#define _BSP_DS3231_H_
#include "sys.h"
#include "bsp_i2c_gpio.h"

#define DS3231_ADDRESS 0xd0
#define CLOCK_NUM 5
#define CLOCK_TIME 10//s
#define SLEEP_TIME 300//s 贪睡时间5分钟 = 300s
struct calendar
{
	int year;
	int month;
	int day;
	int week;
	int hour;
	int minute;
	int second;
};

typedef struct 
{
	uint8_t hour;//闹钟小时
	uint8_t minute;//闹钟分钟
	uint8_t type;//闹钟类型
	uint8_t enable;//闹钟使能
	uint8_t start;//闹钟启停
	uint8_t AlarmWeek;//闹钟星期
	uint16_t WorkTime;//闹钟工作时间
	uint16_t SleepTimeEn;//闹钟工作时间
	uint16_t SleepTime;//闹钟再次工作时间
}AlarmClock;

extern AlarmClock AlarmClk[CLOCK_NUM];//闹钟
extern struct calendar calendar;
extern uint8_t HEX2BCD(uint8_t val);   //B码转换为BCD码
extern void DS3231_Write_Reg(uint8_t add,uint8_t dat);
extern struct calendar calendar;
extern void DS3231_INIT(uint8_t year,uint8_t month,uint8_t day,uint8_t week,uint8_t hour,uint8_t minute,uint8_t second);//初始化时间
extern void DS3231_READ_TIME(void);
extern uint8_t runnian(void);
extern void AUTOWEEK(void);
#endif
