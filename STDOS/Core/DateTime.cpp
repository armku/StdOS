#include "DateTime.h"
#include <stdio.h>

DateTime::DateTime()
{
	this->Year=1970;
	this->Month=1;
	this->Day=1;
	this->Hour=8;
	this->Minute=0;
	this->Second=0;
}
DateTime::DateTime(int year, int month, int day)
{
	this->Year=year;
	this->Month=month;
	this->Day=day;
}
DateTime::DateTime(int seconds)
{
	
}
DateTime::DateTime(const DateTime &value)
{
	
}
#if 0
DateTime::DateTime(DateTime && value)
{
}
#endif
#define FEBRUARY		2
#define STARTOFTIME		1970
#define SECDAY			86400L           /*  一天有多少s */
#define SECYR			(SECDAY * 365)
#define leapyear(year)		((year) % 4 == 0)
#define days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define days_in_month(a) 	(month_days[(a) - 1])
/*星期，生肖用文字ASCII码*/
byte const *WEEK_STR[] = 
{
    "日", "一", "二", "三", "四", "五", "六"
};

//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
bool Is_Leap_Year(ushort year)
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 			   
//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表											 
byte const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const byte mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31}; 
DateTime &DateTime::operator = (int seconds)
{
	ushort daycnt=0;
	uint timecount=0; 
	uint temp=0;
	ushort temp1=0;	  
    timecount=seconds;	 
 	temp=timecount/86400;   //得到天数(秒钟数对应的)
	
	if(daycnt!=temp)//超过一天了
	{	  
		daycnt=temp;
		temp1=1970;	//从1970年开始
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//是闰年
			{
				if(temp>=366)temp-=366;//闰年的秒钟数
				else {temp1++;break;}  
			}
			else temp-=365;	  //平年 
			temp1++;  
		}   
		this->Year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(Is_Leap_Year(this->Year)&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
				else break;
			}
			temp1++;  
		}
		this->Month=temp1+1;	//得到月份
		this->Day=temp+1;  	//得到日期 
	}
	else
	{
		this->Year=1970;
		this->Month=1;
		this->Day=1;
	}
	temp=timecount%86400;     		//得到秒钟数   	   
	this->Hour=temp/3600;     	//小时
	this->Minute=(temp%3600)/60; 	//分钟	
	this->Second=(temp%3600)%60; 	//秒钟
	 	
	return *this;
}

DateTime &DateTime::operator = (const DateTime &value)
{
	return *this;
}
DateTime &DateTime::Parse(int seconds)
{
	return *this;
}
DateTime &DateTime::ParseMs(Int64 ms)
{
	return *this;
}
// 1970/1/1以来天数
int DateTime::TotalDays()const
{
	uint daycount=0;
	
	if(this->Year<1970||this->Year>2099)return 1;	
	for(int t=1970;t<this->Year;t++)	//把所有年份的天数相加
	{
		if(Is_Leap_Year(t))daycount+=366;//闰年的天数
		else daycount+=365;			  //平年的天数
	}
	
	ushort syear=this->Year;
	byte smon=this->Month;
	byte sday=this->Day;
	
	smon-=1;
	for(int t=0;t<smon;t++)	   //把前面月份的天数相加
	{
		daycount+=(uint)mon_table[t];//月份天数相加
		if(Is_Leap_Year(syear)&&t==1)daycount++;//闰年2月份增加一天的秒钟数	   
	}
	daycount+=(uint)(sday-1);//把前面日期的秒钟数相加 
	
	return daycount;
}
// 1970/1/1以来秒数
int DateTime::TotalSeconds()const
{	
	byte hour=this->Hour;
	byte min=this->Minute;
	byte sec=this->Second;
	uint seccount=0;
	
	seccount=this->TotalDays()*86400;
	seccount+=(uint)hour*3600;//小时秒钟数
    seccount+=(uint)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去
	return 	seccount-8*60*60;
}
// 1970/1/1以来毫秒数
Int64 DateTime::TotalMs()const
{	
	return this->TotalSeconds()*1000;
}
byte DateTime::DayOfWeek()const
{
	ushort temp2;
	byte yearH,yearL;
	
	yearH=this->Year/100;	yearL=this->Year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+this->Day+table_week[this->Month -1];
	if (yearL%4==0&&this->Month <3)temp2--;
	return(temp2%7);
}
DateTime DateTime::Date()const
{
	return *this;
}
//当前时间
DateTime DateTime::Now()
{
	DateTime dt;
	dt.Year=2017;
	dt.Month=01;
	dt.Day=29;
	dt.Hour=10;
	dt.Minute=55;
	dt.Second=12;
	return dt;
}
void DateTime::Show(bool newLine)const
{
	printf("%04d-%02d-%02d %02d:%02d:%02d",this->Year,this->Month,this->Day,this->Hour,this->Minute,this->Second);	
}
