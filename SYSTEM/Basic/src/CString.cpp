/*
进行到字符串搜索
 */

#include "CString.h"
#include <stdio.h>
#include <string.h>
#include <CType.h>

String::String(){}
String::~String()
{
    delete []((byte*)(this->_Arr));
}

String::String(const char *str, int len)
{
    if (len < 0)
    {
        this->_Length = strlen(str);
    }
    else
    {
        this->_Length = len;
    }
    this->initCapacity();

    this->Copy(0, str, this->_Length);
}

String::String(char ch)
{
    this->initCapacity();
    ((byte*)(this->_Arr))[0] = ch;
    this->jsLength();
}

//进制字符串
String::String(byte value, byte radix)
{
    this->initCapacity();

    switch (radix)
    {
        case 10:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%d", value);
            break;
        case 16:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%X", value);
            break;
    }
    this->jsLength();
}

//进制字符串
String::String(short value, byte radix)
{
    this->initCapacity();

    switch (radix)
    {
        case 10:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%d", value);
            break;
        case 16:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%X", value);
            break;
    }
    this->jsLength();
}

//进制字符串
String::String(int value, byte radix)
{
    this->initCapacity();

    switch (radix)
    {
        case 10:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%d", value);
            break;
        case 16:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%X", value);
            break;
    }
    this->jsLength();
}

//进制字符串
String::String(uint value, byte radix)
{
    this->initCapacity();

    switch (radix)
    {
        case 10:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%u", value);
            break;
        case 16:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%X", value);
            break;
    }
    this->jsLength();
}

//进制字符串
String::String(Int64 value, byte radix)
{
    this->initCapacity();

    switch (radix)
    {
        case 10:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%lld", value);
            break;
        case 16:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%llX", value);
            break;
    }
    this->jsLength();
}

//进制字符串
String::String(UInt64 value, byte radix)
{
    this->initCapacity();

    switch (radix)
    {
        case 10:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%llu", value);
            break;
        case 16:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%llX", value);
            break;
    }
    this->jsLength();
}

//浮点数
String::String(float value, byte dot)
{
    this->initCapacity();
    switch (dot)
    {
        case 0:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.0f", value);
            break;
        case 1:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.1f", value);
            break;
        case 2:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.2f", value);
            break;
        case 3:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.3f", value);
            break;
        case 4:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.4f", value);
            break;
        case 5:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.5f", value);
            break;
        default:
            break;
    }
    this->jsLength();
}

//浮点数
String::String(double value, byte dot)
{
    this->initCapacity();
    switch (dot)
    {
        case 0:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.0lf", value);
            break;
        case 1:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.1lf", value);
            break;
        case 2:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.2lf", value);
            break;
        case 3:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.3lf", value);
            break;
        case 4:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.4lf", value);
            break;
        case 5:
            snprintf((char*)((byte*)(this->_Arr)), this->mcapacity, "%0.5lf", value);
            break;
        default:
            break;
    }
    this->jsLength();
}

String::String(const String &str1)
{
    #if 1
        this->initCapacity();
    #endif 
    #if 1
        for (int i = 0; i < str1.Length(); i++)
        {
            ((byte*)(this->_Arr))[i] = str1.GetBuffer()[i];
        }
        this->jsLength();
    #endif 
}

String &String::operator = (const char *str)
{
    if (strlen(str) <= this->mcapacity)
    {
        this->_Length = strlen(str);
    }
    else
    {
        this->_Length = this->mcapacity;
    }
    for (int i = 0; i < this->_Length; i++)
    {
        ((byte*)(this->_Arr))[i] = str[i];
    }
    ((byte*)(this->_Arr))[this->_Length] = 0;
    return  *this;
}

String &String::operator = (const String &str)
{
    if (str.Length() <= this->mcapacity)
    {
        this->_Length = str.Length();
    }
    else
    {
        this->_Length = this->mcapacity;
    }
    for (int i = 0; i < this->_Length; i++)
    {
        ((byte*)(this->_Arr))[i] = str.GetBuffer()[i];
    }
    ((byte*)(this->_Arr))[this->_Length] = 0;
    return  *this;
}

bool String::operator == (const char *str)
{
    if (strlen(str) != this->_Length)
    {
        return false;
    }
    for (int i = 0; i < this->_Length; i++)
    {
        if (((byte*)(this->_Arr))[i] != str[i])
        {
            return false;
        }
    }
    return true;
}

bool String::operator == (const String &str1)
{
    if (this->Length() != str1._Length)
    {
        return false;
    }
    for (int i = 0; i < this->_Length; i++)
    {
        if (((byte*)(this->_Arr))[i] != ((byte*)str1._Arr)[i])
        {
            return false;
        }
    }
    return true;
}

String &String::operator += (const DateTime &dt)
{
    if ((this->_Length + 19) < this->Capacity())
    {
        this->_Length += 19;
        snprintf((char*) &((byte*)(this->_Arr))[this->_Length - 1-18], 20, "%04d-%02d-%02d %02d:%02d:%02d", dt.Year, dt.Month, dt.Day, dt.Hour, dt.Minute, dt.Second);
    }
    return  *this;
}

String &String::operator += (const String &str)
{
    if ((str.Length() + this->_Length) < this->mcapacity)
    {
        this->_Length += str.Length();
        this->Copy(this->_Length - str.Length(), str.GetBuffer(), str.Length());
    }

    return  *this;
}

String &String::operator += (const float da)
{
    byte buf[100] = 
    {
        0
    };
    snprintf((char*)buf, 100, "%2.2f", da);
    if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
    {
        this->Copy(this->_Length, buf, strlen((char*)buf));
    }
    this->_Length += strlen((char*)buf);
    return  *this;
}

String &String::Concat(byte value, byte radix)
{
    byte buf[100] = 
    {
        0
    };
    switch (radix)
    {
        case 10:
            snprintf((char*)buf, 100, "%d", value);
            break;
        case 16:
            snprintf((char*)buf, 100, "%X", value);
            break;
        default:
            break;
    }
    if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
    {
        this->Copy(this->_Length, buf, strlen((char*)buf));
    }
    this->_Length += strlen((char*)buf);
    return  *this;
}

String &String::Concat(short value, byte radix)
{
    byte buf[100] = 
    {
        0
    };
    switch (radix)
    {
        case 10:
            snprintf((char*)buf, 100, "%d", value);
            break;
        case 16:
            snprintf((char*)buf, 100, "%X", value);
            break;
        default:
            break;
    }
    if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
    {
        this->Copy(this->_Length, buf, strlen((char*)buf));
    }
    this->_Length += strlen((char*)buf);
    return  *this;
}

String &String::Concat(int value, byte radix)
{
    byte buf[100] = 
    {
        0
    };
    switch (radix)
    {
        case 10:
            snprintf((char*)buf, 100, "%d", value);
            break;
        case 16:
            snprintf((char*)buf, 100, "%X", value);
            break;
        default:
            break;
    }
    if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
    {
        this->Copy(this->_Length, buf, strlen((char*)buf));
    }
    this->_Length += strlen((char*)buf);
    return  *this;
}

String &String::Concat(uint value, byte radix)
{
    byte buf[100] = 
    {
        0
    };
    switch (radix)
    {
        case 10:
            snprintf((char*)buf, 100, "%u", value);
            break;
        case 16:
            snprintf((char*)buf, 100, "%X", value);
            break;
        default:
            break;
    }
    if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
    {
        this->Copy(this->_Length, buf, strlen((char*)buf));
    }
    this->_Length += strlen((char*)buf);
    return  *this;
}

String &String::Concat(Int64 value, byte radix)
{
    byte buf[100] = 
    {
        0
    };
    switch (radix)
    {
        case 10:
            snprintf((char*)buf, 100, "%lld", value);
            break;
        case 16:
            snprintf((char*)buf, 100, "%llX", value);
            break;
        default:
            break;
    }
    if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
    {
        this->Copy(this->_Length, buf, strlen((char*)buf));
    }
    this->_Length += strlen((char*)buf);
    return  *this;
}

String &String::Concat(UInt64 value, byte radix)
{
    byte buf[100] = 
    {
        0
    };
    switch (radix)
    {
        case 10:
            snprintf((char*)buf, 100, "%llu", value);
            break;
        case 16:
            snprintf((char*)buf, 100, "%llX", value);
            break;
        default:
            break;
    }
    if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
    {
        this->Copy(this->_Length, buf, strlen((char*)buf));
    }
    this->_Length += strlen((char*)buf);
    return  *this;
}

String &String::Concat(float value, byte dot)
{
    byte buf[100] = 
    {
        0
    };
    switch (dot)
    {
        case 0:
            snprintf((char*)buf, 100, "%0.0f", value);
            break;
        case 1:
            snprintf((char*)buf, 100, "%1.1f", value);
            break;
        case 2:
            snprintf((char*)buf, 100, "%2.2f", value);
            break;
        case 3:
            snprintf((char*)buf, 100, "%3.3f", value);
            break;
        case 4:
            snprintf((char*)buf, 100, "%4.4f", value);
            break;
        default:
            break;
    }

    if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
    {
        this->Copy(this->_Length, buf, strlen((char*)buf));
    }
    this->_Length += strlen((char*)buf);
    return  *this;
}

String &String::Concat(double value, byte dot)
{
    byte buf[100] = 
    {
        0
    };
    switch (dot)
    {
        case 0:
            snprintf((char*)buf, 100, "%0.0f", value);
            break;
        case 1:
            snprintf((char*)buf, 100, "%1.1f", value);
            break;
        case 2:
            snprintf((char*)buf, 100, "%2.2f", value);
            break;
        case 3:
            snprintf((char*)buf, 100, "%3.3f", value);
            break;
        case 4:
            snprintf((char*)buf, 100, "%4.4f", value);
            break;
        default:
            break;
    }
    if ((this->_Length + strlen((char*)buf)) < this->mcapacity)
    {
        this->Copy(this->_Length, buf, strlen((char*)buf));
    }
    this->_Length += strlen((char*)buf);
    return  *this;
}

byte ToInt(char ch)
{
    ch = toupper(ch);
    if (ch <= '9')
    {
        return ch - '0';
    }
    return ch - 'A' + 10;
}

String String::ToHex()
{
    byte buf[100];
    byte bi = 0;
    byte da;
    int j = 0;
    for (int i = 0; i < 100; i++)
    {
        buf[i] = 0;
    }
    for (int i = 0; i < this->_Length; i++)
    {

        if (isxdigit(((byte*)(this->_Arr))[i]))
        {
            if (bi == 0)
            {
                da = ToInt(((byte*)(this->_Arr))[i]);
                da <<= 4;
                bi = 1;
            }
            else
            {
                da |= ToInt(((byte*)(this->_Arr))[i]);
                buf[j] = da;
                j++;
                bi = 0;
            }
        }
        else
        {
            bi = 0;
        }
    }

    return String((const char*)buf);
}

//数组容量
uint String::Capacity()const
{
    return this->mcapacity;
}

void String::Show(bool newLine)const
{
    printf("%s", ((byte*)(this->_Arr)));
    newLine ? printf("\n"): 0;
}

//初始化容器
void String::initCapacity()
{
    this->_Arr = new byte[0x40];
    this->mcapacity = 0x40 - 1;
    memset(((byte*)(this->_Arr)), 0x40, 0);
}

//计算字符串长度
void String::jsLength()
{
    this->_Length = strlen((char*)((byte*)(this->_Arr)));
}

#ifdef DEBUG
    void String::Test()
	{
		char err[]="String Err\r\n";
		//默认空字符串，使用内部数据区
		String str;
		assert(str.Length() ==0,err);
		assert(str.Capacity() == 0x40 -1,err);
		
		String str1("456");
		assert(str1 == "456",err);
		assert(str1.GetBuffer() == "456",err);
		
		//char err1[] = "String(const String& str)";
		
		
    }
#endif
