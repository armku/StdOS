#pragma once
#include "Type.h"
#include "Object.h"

class DateTime:public Object
{
    public:
        ushort Year;
        byte Month;
        byte Day;
        byte Hour;
        byte Minute;
        byte Second;
        ushort Ms;

        DateTime();
        DateTime(ushort year, byte month, byte day);
        DateTime(uint seconds);
        DateTime(const DateTime &value);
        #if 0
            DateTime(DateTime && value);
        #endif 
        //重载等号运算符
        DateTime &operator = (uint seconds);
        DateTime &operator = (const DateTime &value);

        DateTime &Parse(uint seconds);
        DateTime &ParseMs(UInt64 ms);

        uint TotalDays()const;
        uint TotalSeconds()const;
        UInt64 TotalMs()const;

        //获取星期
        byte DayOfWeek()const;
        //取时间日期的日期部分
        DateTime Date()const;		
		
//////////////////////////////////////////
//以下为新内容
public:
		virtual void Show(bool newLine = true)const;
		
				
		static DateTime Now();//当前时间	
		
		
};
