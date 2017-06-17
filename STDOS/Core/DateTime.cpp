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
#define SECDAY			86400L           /*  һ���ж���s */
#define SECYR			(SECDAY * 365)
#define leapyear(year)		((year) % 4 == 0)
#define days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define days_in_month(a) 	(month_days[(a) - 1])
/*���ڣ���Ф������ASCII��*/
byte const *WEEK_STR[] = 
{
    "��", "һ", "��", "��", "��", "��", "��"
};

//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
bool Is_Leap_Year(ushort year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 			   
//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�											 
byte const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//ƽ����·����ڱ�
const byte mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31}; 
DateTime &DateTime::operator = (int seconds)
{
	ushort daycnt=0;
	uint timecount=0; 
	uint temp=0;
	ushort temp1=0;	  
    timecount=seconds;	 
 	temp=timecount/86400;   //�õ�����(��������Ӧ��)
	
	if(daycnt!=temp)//����һ����
	{	  
		daycnt=temp;
		temp1=1970;	//��1970�꿪ʼ
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//������
			{
				if(temp>=366)temp-=366;//�����������
				else {temp1++;break;}  
			}
			else temp-=365;	  //ƽ�� 
			temp1++;  
		}   
		this->Year=temp1;//�õ����
		temp1=0;
		while(temp>=28)//������һ����
		{
			if(Is_Leap_Year(this->Year)&&temp1==1)//�����ǲ�������/2�·�
			{
				if(temp>=29)temp-=29;//�����������
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
				else break;
			}
			temp1++;  
		}
		this->Month=temp1+1;	//�õ��·�
		this->Day=temp+1;  	//�õ����� 
	}
	else
	{
		this->Year=1970;
		this->Month=1;
		this->Day=1;
	}
	temp=timecount%86400;     		//�õ�������   	   
	this->Hour=temp/3600;     	//Сʱ
	this->Minute=(temp%3600)/60; 	//����	
	this->Second=(temp%3600)%60; 	//����
	 	
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
// 1970/1/1��������
int DateTime::TotalDays()const
{
	uint daycount=0;
	
	if(this->Year<1970||this->Year>2099)return 1;	
	for(int t=1970;t<this->Year;t++)	//��������ݵ��������
	{
		if(Is_Leap_Year(t))daycount+=366;//���������
		else daycount+=365;			  //ƽ�������
	}
	
	ushort syear=this->Year;
	byte smon=this->Month;
	byte sday=this->Day;
	
	smon-=1;
	for(int t=0;t<smon;t++)	   //��ǰ���·ݵ��������
	{
		daycount+=(uint)mon_table[t];//�·��������
		if(Is_Leap_Year(syear)&&t==1)daycount++;//����2�·�����һ���������	   
	}
	daycount+=(uint)(sday-1);//��ǰ�����ڵ���������� 
	
	return daycount;
}
// 1970/1/1��������
int DateTime::TotalSeconds()const
{	
	byte hour=this->Hour;
	byte min=this->Minute;
	byte sec=this->Second;
	uint seccount=0;
	
	seccount=this->TotalDays()*86400;
	seccount+=(uint)hour*3600;//Сʱ������
    seccount+=(uint)min*60;	 //����������
	seccount+=sec;//�������Ӽ���ȥ
	return 	seccount-8*60*60;
}
// 1970/1/1����������
Int64 DateTime::TotalMs()const
{	
	return this->TotalSeconds()*1000;
}
byte DateTime::DayOfWeek()const
{
	ushort temp2;
	byte yearH,yearL;
	
	yearH=this->Year/100;	yearL=this->Year%100; 
	// ���Ϊ21����,�������100  
	if (yearH>19)yearL+=100;
	// ����������ֻ��1900��֮���  
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
//��ǰʱ��
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
