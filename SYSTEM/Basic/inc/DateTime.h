#pragma once
#include "Type.h"
#include "Object.h"

class DateTime
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
		
		static DateTime Now();//当前时间
		
		private:
		static const ushort TicksPerMillisecond = 10000;
        static const long TicksPerSecond = TicksPerMillisecond * 1000;
        static const long TicksPerMinute = TicksPerSecond * 60;
        //static const long TicksPerHour = TicksPerMinute * 60;
        //static const long TicksPerDay = TicksPerHour * 24;
    
        // Number of milliseconds per time unit
        static const int MillisPerSecond = 1000;
        static const int MillisPerMinute = MillisPerSecond * 60;
        static const int MillisPerHour = MillisPerMinute * 60;
        static const int MillisPerDay = MillisPerHour * 24;
    
        // Number of days in a non-leap year
        static const int DaysPerYear = 365;
        // Number of days in 4 years
        static const int DaysPer4Years = DaysPerYear * 4 + 1;       // 1461
        // Number of days in 100 years
        static const int DaysPer100Years = DaysPer4Years * 25 - 1;  // 36524
        // Number of days in 400 years
        static const int DaysPer400Years = DaysPer100Years * 4 + 1; // 146097
    
        // Number of days from 1/1/0001 to 12/31/1600
        static const int DaysTo1601 = DaysPer400Years * 4;          // 584388
        // Number of days from 1/1/0001 to 12/30/1899
        static const int DaysTo1899 = DaysPer400Years * 4 + DaysPer100Years * 3 - 367;
        // Number of days from 1/1/0001 to 12/31/1969
        static const int DaysTo1970 = DaysPer400Years * 4 + DaysPer100Years * 3 + DaysPer4Years * 17 + DaysPerYear; // 719,162
        // Number of days from 1/1/0001 to 12/31/9999
        static const int DaysTo10000 = DaysPer400Years * 25 - 366;  // 3652059
    
        static const long MinTicks = 0;
        //static const long MaxTicks = DaysTo10000 * TicksPerDay - 1;
        //static const long MaxMillis = (long)DaysTo10000 * MillisPerDay;
		
		//static const ushort DaysToMonth365[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
        //static const ushort DaysToMonth366[] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};
};
