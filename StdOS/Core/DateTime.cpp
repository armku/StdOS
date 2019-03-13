#include "DateTime.h"

DateTime::DateTime()
{
    this->Init();
}

DateTime::DateTime(int year, int month, int day)
{
	if(year<1970)
	{
		year=1970;
	}
	if(month<1 || month>12)
	{
		month=1;
	}
	if(day<1 || day >31)
	{
		day=1;
	}
    this->Year = year;
    this->Month = month;
    this->Day = day;
	this->Hour=0;
	this->Minute=0;
	this->Second=0;
	this->Ms=0;
}

DateTime::DateTime(int seconds)
{
   this->Parse(seconds);
   this->Ms = 0;

}

DateTime::DateTime(const DateTime &value)
{
	*this=value;
}
//DateTime::DateTime(DateTime&& value)
//{
//  
//}

DateTime &DateTime::operator = (int seconds)
{
    *this=(seconds);

    return  *this;
}
DateTime &DateTime::operator = (const DateTime &value)
{
	if ( *this != value )
  {
  }
    return  *this;
}

DateTime &DateTime::Parse(int seconds)
{
    this->Second = seconds % 60;
    this->Minute = seconds / 60 % 60;
    int minutes = seconds / 60 / 60;
    this->Minute = minutes % 24;
    this->ParseDays(minutes / 24);

    return  *this;
}

DateTime &DateTime::ParseMs(int64_t ms)
{
    this->Parse(ms / 1000);
    this->Ms = ms % 1000;
    return  *this;
}
const int CummulativeDaysForMonth[] = 
{
    0, 0x1F, 0x3B, 0x5A, 0x78, 0x97, 0xB5, 0xD4, 0xF3, 0x111, 0x130, 0x14E, 0x16D
};

DateTime &DateTime::ParseDays(int days)
{
    int v2; 
    signed int daysRemain;
    int v4;
    bool v5;

    this->Year = days / 365;
    if (this->Year < 136)
    {
        this->Year += 1970;
    }
    v2 = (this->Year - 1) / 4 - (this->Year - 1) / 100 + (this->Year - 1) / 400 - 477 + 365 *(this->Year - 1970);
    if (v2 > days)
    {
        --this->Year;
        v2 = (this->Year - 1) / 4 - (this->Year - 1) / 100 + (this->Year - 1) / 400 - 477 + 365 *(this->Year - 1970);
    }
    daysRemain = days - v2;

    this->Month = daysRemain / 31+1;
    v4 = (!(this->Year % 4) && this->Year % 100 || !(this->Year % 400)) && this->Month + 1 > 2;
    if (daysRemain >= v4 + CummulativeDaysForMonth[this->Month])
        ++this->Month;
    v5 = (!(this->Year % 4) && this->Year % 100 || !(this->Year % 400)) && this->Month > 2;
    this->Day = daysRemain - (v5 + (CummulativeDaysForMonth[this->Month - 1])) + 1;
    return  *this;
}
// 1970/1/1以来天数
int DateTime::TotalDays()const
{
    int v1 = (!(this->Year % 4) && this->Year % 100 || !(this->Year % 400))
    && this->Month > 2;
  return CummulativeDaysForMonth[this->Month - 1]
       + v1
       + (this->Year - 1) / 4
       - (this->Year - 1) / 100
       + (this->Year - 1) / 400
       - 477
       + 365 * (this->Year - 1970)
       + this->Day
       - 1;
}
// 1970/1/1以来秒数
int DateTime::TotalSeconds()const
{
    return this->Second
       + 60 * (this->Minute + 60 * (this->Hour + 24 * this->TotalDays()));
}
// 1970/1/1以来毫秒数
int64_t DateTime::TotalMs()const
{
    return 1000 * this->TotalSeconds() + this->Ms;
}

uint8_t DateTime::DayOfWeek()const
{
    int v1; 

  v1 = (!(this->Year % 4) && this->Year % 100 || !(this->Year % 400))
    && this->Month > 2;
  return ((CummulativeDaysForMonth[this->Month - 1]
                          + v1
                          + (this->Year - 1) / 4
                          - (this->Year - 1) / 100
                          + (this->Year - 1) / 400
                          - 477
                          + 365 * (this->Year - 1970)
                          + this->Day
                          + 3)
                         % 7);
}
DateTime DateTime::Date()const
{
     return DateTime(this->Year, this->Month, this->Day);
}
DateTime DateTime::AddYears(int value) const
{
	DateTime ret(*this);
	ret.ParseDays(this->TotalDays()+value);
	return ret;
}
DateTime DateTime::AddMonths(int value) const
{
  DateTime ret(*this);
   int monsum = value + ret.Month;
  int yeartmp = monsum / 12;
  int montmp = monsum % 12;
  if ( monsum % 12 <= 0 )
  {
    yeartmp = yeartmp - 1;
    montmp = montmp + 12;
  }
  ret.Year = ret.Year + yeartmp;
  ret.Month = montmp;
  
	return ret;
}
DateTime DateTime::AddDays(int value) const
{
  DateTime ret(*this);
	
	ret.ParseDays(ret.TotalDays()+value);
	return ret;
}
DateTime DateTime::AddHours(int value) const
{
  return this->AddMinutes(value*3600);
}
DateTime DateTime::AddMinutes(int value) const
{
  return this->AddMinutes(value*60);
}
DateTime DateTime::AddSeconds(int value) const
{
  DateTime ret(*this);
	
	ret.Parse(ret.TotalSeconds()+value);
	return ret;
}
DateTime DateTime::AddMilliseconds(int64_t value) const
{
  DateTime ret(*this);
	
	ret.ParseMs(ret.TotalMs()+value);
	return ret;
}
int DateTime::CompareTo(const DateTime& value) const
{
	int ret=0;
	if(this->Year==value.Year)
	{
		if(this->Month==value.Month)
		{
			if(this->Day==value.Day)
			{
				if(this->Hour==value.Hour)
				{
					if(this->Minute==value.Minute)
					{
						if(this->Second==value.Second)
						{
							if(this->Ms==value.Ms)
							{
								ret=0;
							}
							else
							{
								ret=this->Ms-value.Ms;
							}
						}
						else
						{
							ret=this->Second-value.Second;
						}
					}
					else
					{
						ret=this->Minute-value.Month;
					}
				}
				else
				{
					ret=this->Hour-value.Hour;
				}
			}
			else
			{
				ret=this->Day-value.Day;
			}
		}
		else
		{
			ret=this->Month-value.Month;
		}
	}
	else
	{
		ret=this->Year-value.Year;
	}
	
  return ret;
}
bool operator==	(const DateTime& left, const DateTime& right)
{
	return left.CompareTo(right)==0;
}
bool operator!=	(const DateTime& left, const DateTime& right)
{
	return left.CompareTo(right)!=0;
}
bool operator>	(const DateTime& left, const DateTime& right)
{
	return left.CompareTo(right)>0;
}
bool operator<	(const DateTime& left, const DateTime& right)
{
	return left.CompareTo(right)<0;
}
bool operator>=	(const DateTime& left, const DateTime& right)
{
	return left.CompareTo(right)>=0;
}
bool operator<=	(const DateTime& left, const DateTime& right)
{
	return left.CompareTo(right)<=0;
}

void DateTime::Show(bool newLine)const
{
    StdPrintf("%04d-%02d-%02d %02d:%02d:%02d", this->Year, this->Month, this->Day, this->Hour, this->Minute, this->Second);
	StdPrintf(newLine ? "\n" : "");
}

void DateTime::Init()
{
    this->Year = 1970;
    this->Month = 1;
    this->Day = 1;
    this->Hour = 0;
    this->Minute = 0;
    this->Second = 0;
    this->Ms = 0;
}
