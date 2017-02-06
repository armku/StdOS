/******************** (C) COPYRIGHT 2009 www.armjishu.com ************************
* File Name          : calendar.h
* Author             : www.armjishu.com Team
* Version            : V1.0
* Date               : 10/1/2009
* Description      : ��ǿ��������֧��ũ����24�����������������Ĺ���
                          ����ʱ����1970��ΪԪ�꣬��32bit��ʱ��Ĵ���������
                          �е�2100������
*******************************************************************************/

#ifndef __CALENDAR_H
#define __CALENDAR_H
#include "stm32f10x.h"


u8 GetMoonDay(u8 month_p,unsigned short table_addr);
u8 GetChinaCalendar(u16  year,u8 month,u8 day,u8 *p);
void GetSkyEarth(u16 year,u8 *p);
void StrCopy(u8 *target,u8 const *source,u8 no);
void GetChinaCalendarStr(u16 year,u8 month,u8 day,u8 *str);
u8 GetJieQi(u16 year,u8 month,u8 day,u8 *JQdate);
u8 GetJieQiStr(u16 year,u8 month,u8 day,u8 *str);
#endif 
